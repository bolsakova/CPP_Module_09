#include "../inc/PmergeMe.hpp"

#include <climits>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <chrono>

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

/**
 * @brief Extracts bigger elements from vector pairs.
 * 
 * Since each pair stores the bigger element in first, this function creates
 * a vector containing only first values.
 * 
 * @param pairs Vector of bigger/smaller pairs.
 * @return Vector containing all bigger elements.
 */
std::vector<int>
PmergeMe::extractVectorBigger(const std::vector<std::pair<int, int> >& pairs) const {
	std::vector<int> bigger;

	for (std::size_t i = 0; i < pairs.size(); ++i)
		bigger.push_back(pairs[i].first);

	return bigger;
}

/**
 * @brief Sorts a vector using the Ford-Johnson merge-insertion algorithm.
 * 
 * The function:
 * 	- creates bigger/smaller pairs
 * 	- recursively sorts bigger elements
 * 	- builds the main chain
 * 	- inserts smaller elements using binary insertion
 * 	- inserts odd element if it exists
 * 
 * @param data Input vector.
 * @return Sorted vector.
 */
std::vector<int>
PmergeMe::sortVector(const std::vector<int>& data) const {
	if (data.size() <= 1)
		return data;

	int odd;
	std::vector<std::pair<int, int> > pairs = makeVectorPairs(data, odd);
	std::vector<int> bigger = extractVectorBigger(pairs);
	std::vector<int> sortedBigger = sortVector(bigger);

	std::vector<int> mainChain = sortedBigger;
	std::vector<std::size_t> order = generateJaconsthalOrder(pairs.size());

	for (std::size_t i = 0; i < order.size(); ++i) {
		std::size_t index = order[i];
		std::vector<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), pairs[index].second);
		mainChain.insert(pos, pairs[index].second);
	}

	if (odd != -1) {
		std::vector<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), odd);
		mainChain.insert(pos, odd);
	}

	return mainChain;
}

/**
 * @brief Creates pairs from deque data.
 * 
 * Elements are grouped two by two. Inside each pair, the bigger element is
 * stored in first and the smallesr element is stored in second.
 * 
 * @param data Input deque.
 * @param odd Output parameter for the unpaired element, or -1 if none.
 * @return Deque of pairs where first is bigger and second is smaller.
 */
std::deque<std::pair<int, int> >
PmergeMe::makeDequePairs(const std::deque<int>& data, int& odd) const {
	std::deque<std::pair<int, int> > pairs;
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

/**
 * @brief Extracts bigger elements from deque pairs.
 * @param pairs Deque of bigger/smaller pairs.
 * @return Deque containing all bigger elements.
 */
std::deque<int>
PmergeMe::extractDequeBigger(const std::deque<std::pair<int, int> >& pairs) const {
	std::deque<int> bigger;

	for (std::size_t i = 0; i < pairs.size(); ++i)
		bigger.push_back(pairs[i].first);

	return bigger;
}

/**
 * @brief Sorts a deque using the Ford-Johnson merge-insertion algorithm.
 * @param data Input deque.
 * @return Sorted deque.
 */
std::deque<int>
PmergeMe::sortDeque(const std::deque<int>& data) const {
	if (data.size() <= 1)
		return data;

	int odd;
	std::deque<std::pair<int, int> > pairs = makeDequePairs(data, odd);
	std::deque<int> bigger = extractDequeBigger(pairs);
	std::deque<int> sortedBigger = sortDeque(bigger);

	std::deque<int> mainChain = sortedBigger;
	std::vector<std::size_t> order = generateJaconsthalOrder(pairs.size());

	for (std::size_t i = 0; i < order.size(); ++i) {
		std::size_t index = order[i];
		std::deque<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), pairs[index].second);
		mainChain.insert(pos, pairs[index].second);
	}

	if (odd != -1) {
		std::deque<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), odd);
		mainChain.insert(pos, odd);
	}

	return mainChain;
}

/**
 * @brief Prints a vector with a label.
 * @param label Text printed before the sequence.
 * @param data Vector to print.
 */
void PmergeMe::printVector(const std::string& label, const std::vector<int>& data) const {
	std::cout << label;

	for (std::size_t i = 0; i < data.size(); ++i)
		std::cout << " " << data[i];

	std::cout << std::endl;
}

/**
 * @brief Prints a deque with a label.
 * @param label Text printed before the sequence.
 * @param data Deque to print.
 */
void PmergeMe::printDeque(const std::string& label, const std::deque<int>& data) const {
	std::cout << label;

	for (std::size_t i = 0; i < data.size(); ++i)
		std::cout << " " << data[i];

	std::cout << std::endl;
}

/**
 * @brief Runs sorting for both containers and prints results.
 * 
 * The function sorts the original input using std::vector and std::deque,
 * measures the execution time for each container and prints the required
 * output.
 */
void PmergeMe::run() const {
	printVector("Before: ", _vectorData);
	
	auto vectorStart = std::chrono::high_resolution_clock::now();
	std::vector<int> sortedVector = sortVector(_vectorData);
	auto vectorEnd = std::chrono::high_resolution_clock::now();
	
	auto dequeStart = std::chrono::high_resolution_clock::now();
	std::deque<int> sortedDeque = sortDeque(_dequeData);
	auto dequeEnd = std::chrono::high_resolution_clock::now();

	printVector("After: ", sortedVector);

	double vectorTime = std::chrono::duration<double, std::micro>(vectorEnd - vectorStart).count();
	double dequeTime = std::chrono::duration<double, std::micro>(dequeEnd - dequeStart).count();

	std::cout << "Time to process a range of " << _vectorData.size()
              << " elements with std::vector : "
              << vectorTime << " us" << std::endl;
	std::cout << "Time to process a range of " << _dequeData.size()
              << " elements with std::deque : "
              << dequeTime << " us" << std::endl;
}

/**
 * @brief Generates insertion order based on Jacobsthal sequence.
 * 
 * The order is used to decide which pending elements should be inserted first.
 * 
 * @param size Number of pending element.
 * @return Vector of indexes in Jacobsthal insertion order.
 */
std::vector<std::size_t> PmergeMe::generateJaconsthalOrder(std::size_t size) const {
	std::vector<std::size_t> order;
	std::vector<std::size_t> jacobsthal;

	if (size == 0)
		return order;

	jacobsthal.push_back(1);
	jacobsthal.push_back(3);

	while (jacobsthal.back() < size) {
		std::size_t n = jacobsthal.size();
		jacobsthal.push_back(jacobsthal[n - 1] + 2 * jacobsthal[n - 2]);
	}

	order.push_back(0);
	std::size_t previous = 1;

	for (std::size_t i = 1; i < jacobsthal.size(); ++i) {
		std::size_t current = jacobsthal[i];

		if (current > size)
			current = size;
		
		for (std::size_t j = current; j > previous; --j)
			order.push_back(j - 1);
		
		previous = current;
		if (previous >= size)
			break;
	}
	return order;
}
