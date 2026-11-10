#include <mc_core/volume.h>

#include <iostream>
#include <fstream>
#include <ranges>
#include <string_view>
#include <charconv>

namespace mc_core
{


	Volume::Volume(const std::filesystem::path& loadPath) 
	{
		parse_volume_properties_from_filename(loadPath.stem().string());
		load_volume(loadPath);
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

	void Volume::parse_volume_properties_from_filename(const std::string& filename)
	{
		std::cout << "Parsing filename: " << filename << std::endl;

		// splits view into subranges (pair of iterators)
		for (const auto part : std::views::split(filename, '_')) {

			const std::string_view token(part.begin(), part.end());
			if (token.size() < 2) continue;

			uint32_t value{};
			const auto [end, ec] = std::from_chars(token.data() + 1, token.data() + token.size(), value);
			if (ec != std::errc{} || end != token.data() + token.size()) continue;

			switch (token[0]) {
				case 'w': dims.x = value; break;
				case 'h': dims.y = value; break;
				case 'd': dims.z = value; break;
				case 'c': channels = value; break;
				case 'b': bitsPerChannel = value; break;
			}
		}

		if (dims == glm::uvec3{} && 0 == channels && 0 == bitsPerChannel) {
			throw std::runtime_error("Volume name was missing an input parameter");
		}
	}

	void Volume::load_volume(const std::filesystem::path& filename) {

		const size_t payloadSize = std::size_t(dims.x) * dims.y * dims.z * channels * bitsPerChannel / 8;
		const auto actualSize = std::filesystem::file_size(filename);

		if (actualSize > payloadSize)
			throw std::runtime_error("File size does not match specified properties");

		std::ifstream file(filename, std::ios::binary);

		if (!file.is_open())
			throw std::runtime_error("Could not open file");

		data.resize(payloadSize);

		if (!file.read(reinterpret_cast<char*>(data.data()), static_cast<std::streamsize>(payloadSize)))
			throw std::runtime_error("Could not read file");
	}


}
