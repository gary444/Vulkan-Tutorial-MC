#pragma once

#include <memory>
#include <string>
#include <cstdint>
#include <span>
#include <vector>
#include <filesystem>

#include <glm/glm.hpp>

namespace mc_core
{


	struct VolumeDesc {
		glm::uvec3 dims{};
		uint32_t channels{};
		uint32_t bitsPerChannel{};
	};

	class Volume
	{
	public:
		Volume(VolumeDesc desc, std::vector<uint8_t> data);

		static Volume loadRaw(const std::filesystem::path& loadPath);
		
		// TODO
		//static Volume loadSciVis(const std::filesystem::path& loadPath);

		// TODO
		//static Volume makeSphere(glm::uvec3, float radius);

		const VolumeDesc& getDesc() { return desc; }

		// span returns a pointer plus a size
		// const in the type specifier indicates a read only uint8_t view
		std::span<const uint8_t> getData() const { return data; }

		float sample(glm::uvec3 p, uint32_t channel = 0) const;

	private:
		VolumeDesc desc;
		std::vector<uint8_t> data;
	};

	std::vector<uint8_t> loadVolumeData(const std::filesystem::path& loadPath, const VolumeDesc& desc);

	inline std::size_t byteSize(const VolumeDesc& d)
	{
		return std::size_t(d.dims.x) * d.dims.y * d.dims.z * d.channels * d.bitsPerChannel / 8;
	}

	
}
