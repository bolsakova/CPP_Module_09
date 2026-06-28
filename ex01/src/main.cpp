#include "../inc/RPN.hpp"

#include <exception>
#include <iostream>

/**
 * @brief Entry point of the RPN program.
 * 
 * The program expects exactly one command-line argument containing
 * the Reverse Polish Notation expression.
 * 
 * @param ac Number of command-line arguments.
 * @param av Array of command-line arguments.
 * @return 0 on success, 1 on error.
 */
int main(int ac, char *av[]) {
	if (ac != 2) {
		std::cerr << "Error" << std::endl;
		return 1;
	}

	try {
		RPN rpn;
		int res = rpn.evaluate(av[1]);

		std::cout << res << std::endl;
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}
