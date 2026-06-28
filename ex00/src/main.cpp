#include "../inc/BitcoinExchange.hpp"

#include <iostream>
#include <exception>

/**
 * @brief Entry point of the btc program.
 * 
 * The program excepts exactly one argument: the input file.
 * It creates a BitcoinExchange object using the default database file
 * "data.csv" and then processes the provided input file.
 * 
 * @param ac Number of command-line arguments.
 * @param av Array of command-line arguments.
 * @return 0 on success, 1 on error.
 */
int main(int ac, char *av[]) {
	if (ac != 2) {
		std::cerr << "Error: could not open file." << std::endl;
		return 1;
	}

	try {
		BitcoinExchange btc;
		btc.processInputFile(av[1]);
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}
