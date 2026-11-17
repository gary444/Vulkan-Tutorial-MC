#include <mc_core/marching_cubes.h>
#include <mc_core/marching_cubes_tables.h>


namespace mc_core
{
	constexpr uint32_t maxNumTrianglesInCell = 5;

	std::vector<Triangle> extractIsosurfaces(const Volume& volume, float isoSurface)
	{
		const auto& desc = volume.getDesc();

		std::vector<Triangle> triangles;
		triangles.reserve(maxNumTrianglesInCell * desc.dims.x * desc.dims.y * desc.dims.z);

		triangles.push_back({ glm::vec3(0,1,0), glm::vec3(0,0,0), glm::vec3(1,0,1) } );

		// TODO
		// implement CPU extraction 

		return triangles;
	}
}
