#include <stdexcept>
#include <fstream>
#include <string_view>
#include <ranges>

#include <mc_core/VolumeIO.h>



namespace mc_core
{

	std::vector<uint8_t> loadVolumeData(const std::filesystem::path& loadPath, const VolumeDesc& desc)
	{
		const auto fileSize = std::filesystem::file_size(loadPath);
		const size_t expectedPayloadSize = desc.byteSize();

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

	VolumeDesc parseVolumeDescriptorFromFilepathRaw(const std::filesystem::path& loadPath)
	{
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

		return desc;
	}



}