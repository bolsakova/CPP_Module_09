#include "../inc/PmergeMe.hpp"

#include <climits>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

/**
 * @brief Default Constructor.
 */
PmergeMe::PmergeMe() {}

/**
 * @brief Constructor that parses command-line arguments.
 * @param ac Number of command-line arguments.
 * @param av Command-line arguments.
 * @throws std::runtime_error If input is invalid.
 */
PmergeMe::PmergeMe(int ac, char *av[]) {
	parseInput(ac, av);
}

/**
 * @brief Copy constructor.
 * @param other Object to copy from.
 */
PmergeMe::PmergeMe(const PmergeMe& other) : _vectorData(other._vectorData), _dequeData(other._dequeData) {}

/**
 * @brief Destructor.
 */
PmergeMe::~PmergeMe() {}

/**
 * @brief Copy assignment operator.
 * @param other Object to assign from.
 * @return Reference to the current object.
 */
PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
	if (this != &other) {
		_vectorData = other._vectorData;
		_dequeData = other._dequeData;
	}
	return *this;
}
