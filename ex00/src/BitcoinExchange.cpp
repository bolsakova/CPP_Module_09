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

	if (end == rateStr.c_str() || *end != '\0')
		throw std::runtime_error("Error: bad database format.");

	if (!isValidDate(date))
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

/**
 * @brief Checks whether a date string is valid.
 * 
 * The expected format is: YYYY-MM-DD
 * The function checks the format, verifies that all date parts are numeric,
 * then validates month and day values, including leap years.
 * 
 * @param date Date string to validate.
 * @return true If the date is valid.
 * @return false If the date is invalid.
 */
bool BitcoinExchange::isValidDate(const std::string& date) const {
	if (date.length() != 10)
		return false;
	
	if (date[4] != '-' || date [7] != '-')
		return false;
	
	for (std::size_t i = 0; i < date.length(); ++i) {
		if (i == 4 || i == 7)
			continue;
		if (!std::isdigit(static_cast<unsigned char>(date[i])))
			return false;
	}

	int year = std::atoi(date.substr(0, 4).c_str());
	int month = std::atoi(date.substr(5, 2).c_str());
	int day = std::atoi(date.substr(8, 2).c_str());

	if (year < 0)
		return false;

	if (month < 1 || month > 12)
		return false;
	
	int daysInMonth[] = {
		31, 28, 31, 30, 31, 30,
		31, 31, 30, 31, 30, 31
	};

	bool leapYear = false;

	if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
		leapYear = true;

	if (leapYear && month == 2)
		daysInMonth[1] = 29;
	
	if (day < 1 || day > daysInMonth[month - 1])
		return false;

	return true;
}

/**
 * @brief Validates and converts the Bitcoin amount.
 * 
 * The function converts the input string to a double and verifies that:
 * 	- the entire string represents a valid number
 * 	- the value isn't negative
 * 	- the value doesn't exceed 1000
 * 
 * @param valueStr String representation of the Bitcoin amount.
 * @param value Output parameter that receives the converted value.
 * 
 * @return true if the value is valid.
 * @return false otherwise.
 */
BitcoinExchange::ValueStatus
BitcoinExchange::validateValue(const std::string& valueStr, double& value) const {
	char* end;

	if (valueStr.empty())
		return VALUE_BAD_INPUT;

	value = std::strtod(valueStr.c_str(), &end);

	if (end == valueStr.c_str() || *end != '\0')
		return VALUE_BAD_INPUT;

	if (value < 0)
		return VALUE_NEGATIVE;
	
	if (value > 1000)
		return VALUE_TOO_LARGE;

	return VALUE_OK;
}

/**
 * @brief Returns the exchange rate for a given date.
 * 
 * If the exact date exists in the database, its rate is returned.
 * If the exact date doesn't exist, the closest lower date is used.
 * 
 * @param date Date for which exchange rate is requested.
 * @return Exchange rate for the exact or closest lower date.
 * @throws std::runtime_error If the database is empty or if the requested
 * date is earlier than the first available date.
 */
double BitcoinExchange::getExchangeRate(const std::string& date) const {
	if (_database.empty())
		throw std::runtime_error("Error: empty database.");

	std::map<std::string, double>::const_iterator it = _database.lower_bound(date);

	if (it != _database.end() && it->first == date)
		return it->second;

	if (it == _database.begin())
		throw std::runtime_error("Error: no exchange rate available for this date.");

	--it;
	return it->second;
}

/**
 * @brief Processes the user input file and prints exchange rate results.
 * 
 * The function reads the input file line by line. The first line is treated
 * as a header and skipped. Each next line is parsed, validated and the used
 * to calculate the Bitcoin value according to the database exchange rate.
 * 
 * If a line is invalid, the function prints the required error message and
 * continues processing the next lines.
 * 
 * @param filename Path to the input file.
 * @throws std::runtime_error If the input file cannot be opened.
 */
void BitcoinExchange::processInputFile(const std::string& filename) const {
	std::ifstream file(filename);

	if (!file.is_open())
		throw std::runtime_error("Error: could not open file.");
	
	std::string line;
	std::getline(file, line);

	while (std::getline(file, line)) {
		try {
			std::pair<std::string, std::string> input = parseInputLine(line);
			if (!isValidDate(input.first)) {
				std::cerr << "Error: bad input =>" << input.first << std::endl;
				continue;
			}

			double value;
			if (!isValidValue(input.second, value)) {
				if (!input.second.empty() && input.second[0] == '-')
					std::cerr << "Error: not a positive number." << std::endl;
				else
					std::cerr << "Error: too large a number." << std::endl;
				continue;
			}

			double rate = getExchangeRate(input.first);
			double res = value * rate;

			std::cout << input.first << " => " << value << " = " << res << std::endl;
		} catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
	}
}
