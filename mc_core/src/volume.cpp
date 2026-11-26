#include <mc_core/Volume.h>
#include <mc_core/VolumeIO.h>

#include <ranges>
#include <string_view>
#include <charconv>

namespace mc_core
{


	Volume::Volume(VolumeDesc desc, std::vector<uint8_t> _data)
		: desc(desc), data( std::move(_data) )
	{

		const size_t expectedPayloadSize = desc.byteSize();
		if (expectedPayloadSize != data.size())
			throw std::runtime_error("Data payload given to volume doesn't match expected size based on descriptor");

		if (glm::any(glm::equal(glm::uvec3(0), desc.dims))) {
			throw std::runtime_error("Dimension value of zero is not allowed");
		}

		if (0 == desc.channels || 4 < desc.channels) {
			throw std::runtime_error("Invalid channel number provided");
		}

		if (desc.bitsPerChannel != 8) {
			throw std::runtime_error("only bit depth of 8 is supported");
		}

		voxelSize = 1.f / glm::vec3(desc.dims);
	}

	Volume Volume::loadRaw(const std::filesystem::path& loadPath)
	{
		const VolumeDesc desc = mc_core::parseVolumeDescriptorFromFilepathRaw(loadPath);
		auto data = loadVolumeData(loadPath, desc);
		return Volume(desc, std::move(data) );
	}

	uint8_t Volume::sample(const glm::uvec3 p, const uint32_t channel) const
	{
		const size_t voxel = size_t(p.z) * desc.dims.x * desc.dims.y + p.y * desc.dims.x + p.x;
		const size_t index = size_t(voxel) * desc.channels + channel;

		return data[index];
	}

	glm::vec3 Volume::getVoxelCentre(glm::uvec3 p) const
	{
		return voxelSize * (glm::vec3(p) + 0.5f);
	}


}
