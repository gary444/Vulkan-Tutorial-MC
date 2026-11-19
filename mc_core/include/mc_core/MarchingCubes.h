#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <mc_core/Structs.h>
#include <mc_core/Volume.h>



namespace mc_core 
{

	constexpr int numVertexOffsets = 8;
	//constexpr glm::uvec3 vertexOffsets [numVertexOffsets] = {
	//	glm::uvec3(0,0,0),
	//	glm::uvec3(0,0,1),
	//	glm::uvec3(0,1,1),
	//	glm::uvec3(0,1,0),
	//	glm::uvec3(1,0,0),
	//	glm::uvec3(1,0,1),
	//	glm::uvec3(1,1,1),
	//	glm::uvec3(1,1,0)
	//};

	// mirrors the offsets used by NVIDIA marching cubes sample
	// https://github.com/tpn/cuda-samples/blob/master/v8.0/2_Graphics/marchingCubes/marchingCubes_kernel.cu
	constexpr glm::uvec3 vertexOffsets[numVertexOffsets] = {
		glm::uvec3(0,0,0),
		glm::uvec3(1,0,0),
		glm::uvec3(1,1,0),
		glm::uvec3(0,1,0),
		glm::uvec3(0,0,1),
		glm::uvec3(1,0,1),
		glm::uvec3(1,1,1),
		glm::uvec3(0,1,1),
	};

	// define tables / data structs in a separate file

	
	std::vector<Triangle> extractIsosurfaces(const Volume& volume, float isoSurface);


}



