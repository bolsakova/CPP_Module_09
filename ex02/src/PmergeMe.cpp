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

/**
 * @brief Checks whether a string represents a valid positive integer.
 * 
 * The function verifies that the string:
 * 	- isn't empty
 * 	- contains only digits
 * 	- represents a number greater then zero
 * 	- doesn't exceed INT_MAX
 * 
 * @param str Argument string to validate.
 * @return true if the argument is a valid positive integer.
 * @return false otherwise.
 */
bool PmergeMe::isValidPositiveInteger(const std::string& str) const {
	if (str.empty())
		return false;

	for (std::size_t i = 0; i < str.length(); ++i) {
		if (str[i] < '0' || str[i] > '9')
			return false;
	}

	char* end;
	long value = std::strtol(str.c_str(), &end, 10);

	if (*end != '\0')
		return false;
	
	if (value <= 0 || value > INT_MAX)
		return false;

	return true;
}
