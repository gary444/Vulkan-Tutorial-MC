#pragma once

#include <vector>
#include <filesystem>

#include <mc_core/Structs.h>

namespace mc_core 
{

	void writeObj(std::filesystem::path writePath, const std::vector<Triangle>& triangles);

}

