#include "../inc/BitcoinExchange.hpp"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

/**
 * @brief Creates a BitcoinExchange object using the default database file.
 * @throws std::runtime_error If the default database file cannot be opened.
 */
BitcoinExchange::BitcoinExchange() {
	loadDatabase("data.csv");
}

/**
 * @brief Creates a BitcoinExchange object using a custom database file.
 * @param databaseFile Path to the CSV database file.
 * @throws std::runtime_error If the database file cannot be opened.
 */
BitcoinExchange::BitcoinExchange(const std::string& databaseFile) {
	loadDatabase(databaseFile);
}

/**
 * @brief Copy constructor.
 * @param other Object to copy from.
 */
BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) : _database(other._database) {}

/**
 * @brief Destructor.
 */
BitcoinExchange::~BitcoinExchange() {}

/**
 * @brief Copy assignment operator.
 * @param other Object to copy from.
 * @return Reference to the current object.
 */
BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other) {
	if (this != &other)
		_database = other._database;
	return *this;
}

/**
 * @brief Loads Bitcoin exchange rates from a CSV database file.
 * 
 * The file is expected to contain lines in the following format:
 * 	- date,exchange_rate
 * The first line is treated as a header and skipped. Every other line is
 * parsed and inserted into the internal std::map.
 * 
 * @param filename Path to the CSV database file.
 * @throws std::runtime_error If the file cannot be opened
 */
void BitcoinExchange::loadDatabase(const std::string& filename) {
	std::ifstream file(filename);

	if (!file.is_open())
		throw std::runtime_error("Error: could not open database.");

	std::string line;
	std::getline(file, line);

	while (std::getline(file, line)) {
		if (line.empty())
			continue;
		std::pair<std::string, double> data = parseDatabaseLine(line);
		_database[data.first] = data.second;
	}
}
