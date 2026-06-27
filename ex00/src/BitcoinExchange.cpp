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



BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other) {
	if (this != &other)
		_database = other._database;
	return *this;
}

