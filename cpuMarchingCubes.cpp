#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include <filesystem>

#include <mc_core/volume.h>
#include <mc_core/marching_cubes.h>


int main(int argc, char* argv[])
{
	if (argc != 3) {
		std::cerr << "Wrong number of arguments. Required arguments are:\n" << argv[0] << " [volume path] [iso value]" << std::endl;
		return EXIT_FAILURE;
	}

	// read input
	const std::filesystem::path volume_path = argv[1];
	const float iso_value = std::stof(argv[2]);

	// check input

	if (!std::filesystem::is_regular_file(volume_path)) {
		std::cerr << "Volume file " << volume_path << " not found" << std::endl;
		return EXIT_FAILURE;
	}

	if (0.f > iso_value || 1.f < iso_value) {
		std::cerr << "Invalid iso-value: " << iso_value << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "CPU Marching Cubes extraction\n\nVolume: " << volume_path << "\nIso-value: " << iso_value << std::endl;

	mc_core::Volume volume = mc_core::Volume::loadRaw(volume_path, { glm::vec3(0.f), glm::vec3(1.f)});
	std::cout << "Loaded volume" << std::endl;

	auto desc = volume.getDesc();
	std::cout << "Volume size: " << desc.dims.x << "x" << desc.dims.y << "x" << desc.dims.z << std::endl;
	std::cout << "Volume channels: " << desc.channels << std::endl;
	std::cout << "Volume bits per channel: " << desc.bitsPerChannel << std::endl;
	std::cout << "Volume min. extent: [" << desc.extents.min.x << " " << desc.extents.min.y << " " << desc.extents.min.z << "]\n";
	std::cout << "Volume max. extent: [" << desc.extents.max.x << " " << desc.extents.max.y << " " << desc.extents.max.z << "]\n";

	std::cout << "Marching Cubes starting...\n";
	std::vector<float> triangles = mc_core::extractIsosurfaces(volume, iso_value);
	std::cout << "Marching Cubes finished with " << triangles.size() / 9 << " triangles\n";



	// TODO

	// use library to write mesh to output file


	return EXIT_SUCCESS;
}
