#include <mc_core/volume.h>

namespace mc_core
{
	Volume::Volume(const std::filesystem::path& loadPath) 
	{
		// TODO parse filepath, set member variables
		// Load volume into memory
	}


	float Volume::sample(const glm::uvec3 p, const uint32_t channel) const
	{
		if (bitsPerChannel != 8) {
			throw std::exception("sampling interpretation for this bit depth is not yet supported");
		}

		const uint32_t voxel = p.z * dims.x * dims.y + p.y * dims.x + p.x;
		const uint32_t index = voxel * channels + channel;

		return static_cast<float>(data[index]) / 255.f;
	}

}
