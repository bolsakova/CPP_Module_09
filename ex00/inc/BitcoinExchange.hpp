#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>
#include <string>
#include <utility>

class BitcoinExchange {
	private:
		enum ValueStatus {
			VALUE_OK,
			VALUE_BAD_INPUT,
			VALUE_NEGATIVE,
			VALUE_TOO_LARGE
		};

		// stores historical Bitcoin exchange rates (date -> rate)
		std::map<std::string, double> _database;

		// loads the CSV database into the map
		void loadDatabase(const std::string& filename);

		// parses one line of the CSV database
		std::pair<std::string, double> parseDatabaseLine(const std::string& line) const;
		// parses one line of the input file
		std::pair<std::string, std::string> parseInputLine(const std::string& line) const;
	
		// checks whether the date has a valid format and value
		bool isValidDate(const std::string& date) const;
		// converts and validates the Bitcoin amount
		ValueStatus validateValue(const std::string& valueStr, double& value) const;

		// removes leading and trailing whitespace
		std::string trim(const std::string& str) const;
	public:
		BitcoinExchange();
		BitcoinExchange(const std::string& databaseFile);
		BitcoinExchange(const BitcoinExchange& other);
		~BitcoinExchange();
		BitcoinExchange& operator=(const BitcoinExchange& other);

		// processes the user input file
		void processInputFile(const std::string& filename) const;
		// returns the exchange rate for the given date
		double getExchangeRate(const std::string& date) const;
};

#endif
