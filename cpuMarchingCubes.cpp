#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include <mc_core/volume.h>


int main()
{
	int r = mc_core::getInt();

	std::cout << "hello world " << r << std::endl;

	return EXIT_SUCCESS;
}
