#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <deque>
#include <string>
#include <vector>
#include <utility>

class PmergeMe {
	private:
		// original input stored in vector and deque
		std::vector<int> _vectorData;
		std::deque<int> _dequeData;

		// parses command-line args into both containers
		void parseInput(int ac, char *av[]);

		// checks whether one arg is a valid positive integer
		bool isValidPositiveInteger(const std::string& str) const;

		// helpers to make pairs in vector and extract bigger elements
		std::vector<std::pair<int, int> >
		makeVectorPairs(const std::vector<int>& data, int& odd) const;

		std::vector<int>
		extractVectorBigger(const std::vector<std::pair<int, int> >& pairs) const;
		
		// sorts vector and deque using Ford-Johnson algorithm
		std::vector<int>
		sortVector(const std::vector<int>& data) const;

		std::deque<int>
		sortDeque(const std::deque<int>& data) const;

		// generates Jacobsthal insertion order
		std::vector<std::size_t> generateJaconsthalOrder(std::size_t size) const;

		// prints vector and deque sequence
		void printVector(const std::string& label, const std::vector<int>& data) const;
		void printDeque(const std::string& label, const std::deque<int>& data) const;
	public:
		PmergeMe();
		PmergeMe(int ac, char *av[]);
		PmergeMe(const PmergeMe& other);
		~PmergeMe();
		PmergeMe& operator=(const PmergeMe& other);

		// runs sorting for both containers and prints results
		void run() const;
};

#endif
