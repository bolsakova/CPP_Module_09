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

/**
 * @brief Parses command-line arguments into vector and deque.
 * 
 * Each argument must be a valid positive integer. The same values are stored
 * in both containers so the same algorithm can later be tested with different
 * data structures.
 * 
 * @param ac Number of command-line arguments.
 * @param av Command-line arguments.
 * @throws std::runtime_error If there are no numbers or no argument is invalid.
 */
void PmergeMe::parseInput(int ac, char *av[]) {
	if (ac < 2)
		throw std::runtime_error("Error");
		
	for (int i = 1; i < ac; ++i) {
		std::string arg(av[i]);
		if (!isValidPositiveInteger(arg))
			throw std::runtime_error("Error");
		int value = std::atoi(arg.c_str());
		_vectorData.push_back(value);
		_dequeData.push_back(value);
	}
}

/**
 * @brief Creates pairs from vector data.
 * 
 * Elements are grouped two by two. Inside eachpair, the bigger element is
 * stored in first and the smaller element is stored in second.
 * If the number of elements is odd, the last element is stored separately
 * in the odd parameter.
 * 
 * @param data Input vector.
 * @param odd Output parameter for the unpaired element, or -1 if none.
 * @return Vector of pairs where first is bigger and second smaller.
 */
std::vector<std::pair<int, int> >
PmergeMe::makeVectorPairs(const std::vector<int>& data, int& odd) const {
	std::vector<std::pair<int, int> > pairs;
	odd = -1;

	for (std::size_t i = 0; i + 1 < data.size(); i += 2) {
		if (data[i] > data[i + 1])
			pairs.push_back(std::make_pair(data[i], data[i + 1]));
		else
			pairs.push_back(std::make_pair(data[i + 1], data[i]));
	}

	if (data.size() % 2 != 0)
		odd = data.back();
	
	return pairs;
}

std::vector<int>
PmergeMe::sortVector(const std::vector<int>& data) const {

}

std::deque<int>
PmergeMe::sortDeque(const std::deque<int>& data) const {

}
