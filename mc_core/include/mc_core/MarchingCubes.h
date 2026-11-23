#pragma once

#include <vector>

#include <mc_core/Structs.h>
#include <mc_core/Volume.h>



namespace mc_core 
{

	
	std::vector<Triangle> extractIsosurfaceAsTriangleSoup(const Volume& volume, float isoSurface);

	// TODO 
	//std::vector<Triangle> extractIsosurfaceAsVertexAndIndexBuffer(const Volume& volume, float isoSurface);

}



