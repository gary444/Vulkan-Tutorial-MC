#include <mc_core/volume.h>

#include <iostream>
#include <fstream>
#include <ranges>
#include <string_view>
#include <charconv>

namespace mc_core
{

	Volume::Volume(VolumeDesc desc, std::vector<uint8_t> _data)
		: desc(desc), data( std::move(_data) )
	{

		const size_t expectedPayloadSize = byteSize(desc);
		if (expectedPayloadSize != data.size())
			throw std::runtime_error("Data payload given to volume doesn't match expected size based on descriptor");
	}

	Volume Volume::loadRaw(const std::filesystem::path& loadPath)
	{
		std::cout << loadPath.stem().string() << std::endl;

		// parse filename
		VolumeDesc desc;

		// splits view into subranges (pair of iterators)
		for (const auto part : std::views::split(loadPath.stem().string(), '_')) {


			const std::string_view token(part.begin(), part.end());
						if (token.size() < 2) continue;



			uint32_t value{};
			const auto [end, ec] = std::from_chars(token.data() + 1, token.data() + token.size(), value);
			if (ec != std::errc{} || end != token.data() + token.size()) continue;

			switch (token[0]) {
			case 'w': desc.dims.x = value; break;
			case 'h': desc.dims.y = value; break;
			case 'd': desc.dims.z = value; break;
			case 'c': desc.channels = value; break;
			case 'b': desc.bitsPerChannel = value; break;
			}
		}

		if (0 == desc.dims.x || 0 == desc.dims.y || 0 == desc.dims.z || 0 == desc.channels || 0 == desc.bitsPerChannel) {
			throw std::runtime_error("Volume name was missing an input parameter");
		}

		auto data = loadVolumeData(loadPath, desc);


		return Volume(desc, std::move(data) );
	}

	std::vector<uint8_t> loadVolumeData(const std::filesystem::path& loadPath, const VolumeDesc& desc)
	{
		const auto fileSize = std::filesystem::file_size(loadPath);
		const size_t expectedPayloadSize = byteSize(desc);
		
		if (fileSize != expectedPayloadSize)
			throw std::runtime_error("File size does not match specified properties");

		std::vector<uint8_t> data;

		std::ifstream file(loadPath, std::ios::binary);

		if (!file.is_open())
			throw std::runtime_error("Could not open file");

		data.resize(expectedPayloadSize);

		if (!file.read(reinterpret_cast<char*>(data.data()), static_cast<std::streamsize>(expectedPayloadSize)))
			throw std::runtime_error("Could not read file");

		return data;
	}


	float Volume::sample(const glm::uvec3 p, const uint32_t channel) const
	{
		if (desc.bitsPerChannel != 8) {
			throw std::runtime_error("sampling interpretation for this bit depth is not yet supported");
		}

		const size_t voxel = size_t(p.z) * desc.dims.x * desc.dims.y + p.y * desc.dims.x + p.x;
		const size_t index = size_t(voxel) * desc.channels + channel;

		return static_cast<float>(data[index]) / 255.f;
	}


}
