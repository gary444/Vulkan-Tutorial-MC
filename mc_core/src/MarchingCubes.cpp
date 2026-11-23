#include <array>

#include <glm/glm.hpp>

#include <mc_core/MarchingCubes.h>
#include <mc_core/MarchingCubesTables.h>



namespace mc_core
{

	glm::vec3 interpolateVertexPosition(glm::vec3 vertex0, glm::vec3 vertex1, float value0, float value1, const float isoValue) {

		// make sure direction of interpolation is always the same to avoid floating point errors
		if (vertex1.x < vertex0.x || vertex1.y < vertex0.y || vertex1.z < vertex0.z) { std::swap(vertex0, vertex1); std::swap(value0, value1); }
		const float normalizedIntersectionDistance = (value0 - isoValue) / (value0 - value1);
		return vertex0 + (vertex1 - vertex0) * normalizedIntersectionDistance;
	}

	std::vector<Triangle> extractIsosurfaceAsTriangleSoup(const Volume& volume, float isoValue)
	{
		const auto& desc = volume.getDesc();

		std::vector<Triangle> triangles;
		triangles.reserve(2 * desc.dims.x * desc.dims.y); // low estimate of required space

		for (size_t z = 0; z < desc.dims.z - 1; z++){
			for (size_t y = 0; y < desc.dims.y - 1; y++){
				for (size_t x = 0; x < desc.dims.x - 1; x++){

					const glm::uvec3 cellIndex{ x,y,z };

					std::array<float, numCellVertices> sampledValues{};
					for (size_t vertex = 0; vertex < numCellVertices; vertex++)
					{
						sampledValues[vertex] = volume.sample(cellIndex + vertexOffsets[vertex]);
					}

					uint8_t vertexValueBelowIsosurfaceFlags = 0;
					for (size_t vertex = 0; vertex < numCellVertices; vertex++)
					{
						if (sampledValues[vertex] < isoValue)
						{
							vertexValueBelowIsosurfaceFlags |= (1 << vertex);
						}
					}

					if (0 == vertexValueBelowIsosurfaceFlags || 255 == vertexValueBelowIsosurfaceFlags) {
						continue;
					}

					// get the real vertex positions at each cell vertex
					std::array<glm::vec3, numCellVertices> realVertexPositions{};
					for (size_t vertex = 0; vertex < numCellVertices; vertex++)
					{
						// TODO avoid division in getVoxelCentre()
						realVertexPositions[vertex] = volume.getVoxelCentre(cellIndex + vertexOffsets[vertex]);
					}

					// get edgeTable variable that encodes which of the 12 edges are intersected
					const uint32_t edgeIntersectionFlags = edgeTable[vertexValueBelowIsosurfaceFlags];

					// calculate the vertex positions of the intersected edges
					std::array<glm::vec3, numCellEdges> interpolatedVertices{};
					for (uint32_t edge = 0; edge < numCellEdges; edge++)
					{
						if (edgeIntersectionFlags & (1u << edge)) {

							const uint8_t cellVertices[2] = { edgeToVerticesMapping[edge][0], edgeToVerticesMapping[edge][1] };
							const glm::vec3 vertex0 = realVertexPositions[cellVertices[0]];
							const glm::vec3 vertex1 = realVertexPositions[cellVertices[1]];
							const float value0 = sampledValues[cellVertices[0]];
							const float value1 = sampledValues[cellVertices[1]];

							interpolatedVertices[edge] = interpolateVertexPosition(vertex0, vertex1, value0, value1, isoValue);
						}
					}


					// build the triangles (up to 5) using the triTable
					const uint8_t numTriangles = numVertsTable[vertexValueBelowIsosurfaceFlags] / 3;
					for (size_t triangle = 0; triangle < numTriangles; triangle++)
					{
						uint8_t indices[3];
						for (size_t vertex = 0; vertex < 3; vertex++)
						{
							indices[vertex] = triTable[vertexValueBelowIsosurfaceFlags][triangle * 3 + vertex];
						}

						triangles.push_back({ interpolatedVertices[indices[0]], interpolatedVertices[indices[1]], interpolatedVertices[indices[2]] });
					}
				}
			}
		}

		return triangles;
	}
}
