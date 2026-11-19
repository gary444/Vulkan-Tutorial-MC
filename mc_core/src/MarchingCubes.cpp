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
					std::vector<float> sampledValues(8);

					for (size_t vertex = 0; vertex < numVertexOffsets; vertex++)
					{
						sampledValues[vertex] = volume.sample(cellIndex + vertexOffsets[vertex]);
						if (sampledValues[vertex] < isoValue)
						{
							vertexCode |= (1 << vertex);
						}
					}

					if (0 == vertexCode || 255 == vertexCode) {
						continue;
					}

					// TODO create correct triangles according to vertex code

					// get edgeTable variable which encodes which of the 12 edges are intersected
					const uint8_t edgeIntersection = edgeTable[vertexCode];

					// get the real vertex positions at each cell vertex
					std::vector<glm::vec3> realVertexPositions(8);
					for (size_t vertex = 0; vertex < numVertexOffsets; vertex++)
					{
						realVertexPositions[vertex] = volume.getVoxelCentre(cellIndex + vertexOffsets[vertex]);
					}


					// calculate the vertex positions of the intersected edges
					std::vector<glm::vec3> interpolatedVertices(12);
					for (size_t edge = 0; edge < interpolatedVertices.size(); edge++)
					{
						if (edgeIntersection & (1 << edge)) {

							const uint8_t cellVertices[2] = { edgeToVerticesMapping[edge][0], edgeToVerticesMapping[edge][1] };
							glm::vec3 realVertexPositions[2]; 
							// TODO
							// calculate vertex position from
							// sampled values at vertices
							// iso-value
							// actual position of vertices (
						}
					}


					// build the triangles (up to 5) using the triTable

					for (size_t triangle = 0; triangle < 5; triangle++)
					{
						uint8_t indices[3];
						for (size_t vertex = 0; vertex < 3; vertex++)
						{
							indices[vertex] = triTable[vertexCode][triangle * 3 + vertex];
						}

						if (indices[0] >= 255)
							break;

						triangles.push_back({ interpolatedVertices[indices[0]], interpolatedVertices[indices[1]], interpolatedVertices[indices[2]] });
					}



					// TODO alternative method that produces index and vertex buffer







					triangles.push_back({ glm::vec3(0,1,0), glm::vec3(0,0,0), glm::vec3(1,0,1) });

				}
			}
		}

		return triangles;
	}
}
