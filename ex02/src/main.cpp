#include "../inc/PmergeMe.hpp"

#include <exception>
#include <iostream>

/**
 * @brief Entry point of the PmergeMe program.
 * 
 * The program expects exactly one command-line argument.
 * 
 * @param ac Number of command-line arguments.
 * @param av Array of command-line arguments.
 * @return 0 on success, 1 on error.
 */
int main(int ac, char *av[]) {
	try {
		PmergeMe sorter(ac, av);
		sorter.run();
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
	
	return 0;
}
