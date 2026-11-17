
#include <iostream>
#include <fstream>

#include <mc_core/ObjWriter.h>


namespace mc_core
{

	void writeObj(std::filesystem::path writePath, const std::vector<Triangle>& triangles) {

		if (writePath.extension() != ".obj") {
			throw std::runtime_error("Incorrect file extension requested for output file");
		}

		std::ofstream outfile(writePath);

		if (!outfile.is_open()) {
			throw std::runtime_error("Could not create output file");
		}

		outfile << "# Written with file mc_core/ObjWriter.cpp\n";

		for (auto t : triangles) {

			for (size_t vertex = 0; vertex < 3; vertex++)
			{
				outfile << "v " << t.vertices[vertex].x << " " << t.vertices[vertex].y << " " << t.vertices[vertex].z << "\n";
			}
		}

		for (size_t triangleId = 0; triangleId < triangles.size(); triangleId++)
		{
			outfile << "f " << triangleId * 3 + 1 << " " << triangleId * 3 + 2 << " " << triangleId * 3 + 3 << "\n";
		}
	}

}
