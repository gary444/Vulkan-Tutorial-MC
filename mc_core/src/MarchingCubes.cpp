#include <mc_core/MarchingCubes.h>
#include <mc_core/MarchingCubesTables.h>


namespace mc_core
{
	constexpr uint32_t maxNumTrianglesInCell = 5;

	std::vector<Triangle> extractIsosurfaces(const Volume& volume, float isoValue)
	{
		const auto& desc = volume.getDesc();

		std::vector<Triangle> triangles;
		triangles.reserve(maxNumTrianglesInCell * desc.dims.x * desc.dims.y * desc.dims.z);

		for (size_t z = 0; z < desc.dims.z - 1; z++){
			for (size_t y = 0; y < desc.dims.y - 1; y++){
				for (size_t x = 0; x < desc.dims.x - 1; x++){

					const glm::uvec3 cellIndex{ x,y,z };

					uint8_t vertexCode = 0;
					for (size_t vertex = 0; vertex < numVertexOffsets; vertex++)
					{
						const float sampledVertexValue = volume.sample(cellIndex + vertexOffsets[vertex]);
						if (sampledVertexValue > isoValue)
						{
							vertexCode |= (1 << vertex);
						}
					}

					if (0 == vertexCode || 255 == vertexCode) {
						continue;
					}

					// TODO create correct triangles

					triangles.push_back({ glm::vec3(0,1,0), glm::vec3(0,0,0), glm::vec3(1,0,1) });

				}
			}
		}




		// TODO
		// implement CPU extraction 

		return triangles;
	}
}
