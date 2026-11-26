#pragma once

#include <cstdint>
#include <vector>
#include <filesystem>


#include <mc_core/Volume.h>

namespace mc_core
{
	std::vector<uint8_t> loadVolumeData(const std::filesystem::path& loadPath, const VolumeDesc& desc);

	VolumeDesc parseVolumeDescriptorFromFilepathRaw(const std::filesystem::path& loadPath);

}