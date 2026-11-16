#pragma once

#include <vector>
#include <mc_core/volume.h>

namespace mc_core 
{
	// define tables / data structs in a separate file

	
	std::vector<float> extractIsosurfaces(const Volume& volume, float isoSurface);


}



