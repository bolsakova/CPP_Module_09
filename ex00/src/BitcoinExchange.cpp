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

/**
 * @brief Parses one line from the CSV database.
 * 
 * The database line is expected to have the following format:
 * 	- data,exchange_rate.
 * Example: 2011-01-03,0.3.
 * The function splits the line by comma, trims both parts and converts
 * the exchange rate from string to double.
 * 
 * @param line One line from the database file.
 * @return std::pair containing date as first and exchange rate as second.
 * @throws std::runtime_error If the line has an invalid format.
 */
std::pair<std::string, double>
BitcoinExchange::parseDatabaseLine(const std::string& line) const {
	std::stringstream ss(line);
	std::string date;
	std::string rateStr;
	char* end;

	if (!std::getline(ss, date, ',') || !std::getline(ss, rateStr))
		throw std::runtime_error("Error: bad database format.");
	
	date = trim(date);
	rateStr = trim(rateStr);

	if (date.empty() || rateStr.empty())
		throw std::runtime_error("Error: bad database format.");
	
	double rate = std::strtod(rateStr.c_str(), &end);

	if (*end != '\0')
		throw std::runtime_error("Error: bad database format.");
	
	return std::make_pair(date, rate);
}

/**
 * @brief Removes leading and trailing whitespace from a string.
 * 
 * This function removes spaces, tabs and other whitespace characters
 * from both ends of the input string.
 * 
 * @param str The string to trim.
 * @return A copy of the string without leading and trailing whitespace.
 */
std::string BitcoinExchange::trim(const std::string& str) const {
	const std::string whitespace = " \t\n\r\f\v";

	std::size_t first = str.find_first_not_of(whitespace);

	if (first == std::string::npos)
		return "";
	
	std::size_t last = str.find_last_not_of(whitespace);
	return str.substr(first, last - first + 1);
}

/**
 * @brief Parses one line from the input file.
 * 
 * The input line is expected to have the following format:
 * 	- date | value
 * Example: 2011-01-03 | 3
 * This function only splits the line into two strings.
 * It doesn't validate the date or the value.
 * 
 * @param line One line from the input file.
 * @return std::pair containing date as first and value string as second.
 * @throws std::runtime_error If the line does not contain exactly one separator.
 */
std::pair<std::string, std::string>
BitcoinExchange::parseInputLine(const std::string& line) const {
	std::size_t separator = line.find('|');

	if (separator == std::string::npos)
		throw std::runtime_error("Error: bad input => " + line);

	if (line.find('|', separator + 1) != std::string::npos)
		throw std::runtime_error("Error: bad input => " + line);
	
	std::string date = trim(line.substr(0, separator));
	std::string valueStr = trim(line.substr(separator + 1));

	if (date.empty() || valueStr.empty())
		throw std::runtime_error("Error: bad input => " + line);
	
	return std::make_pair(date, valueStr);
}
