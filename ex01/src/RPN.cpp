#include "../inc/RPN.hpp"

#include <cstdlib>
#include <sstream>
#include <stdexcept>

/**
 * @brief Default constructor.
 * 
 * Creates an RPN object. The object itself doesn't store calculation state;
 * the stack is created locally inside evaluate().
 */
RPN::RPN() {}

/**
 * @brief Copy constructor.
 * 
 * Since RPN has no attribute, copying doesn't require any special action.
 * 
 * @param other Object to copy from.
 */
RPN::RPN(const RPN& other) {
	(void)other;
}

/**
 * @brief Destructor.
 * 
 * No manual cleanup is required.
 */
RPN::~RPN() {}

/**
 * @brief Copy assignment operator.
 * 
 * Since RPN has no attribute, there is nothing to copy.
 * 
 * @param other Object to assign from.
 * @return Reference to the current object.
 */
RPN& RPN::operator=(const RPN& other) {
	(void)other;
	return *this;
}

/**
 * @brief Checks whether a token is a supported operator.
 * 
 * Supported operators are: +, -, *, /
 * 
 * @param token Token to check.
 * @return true if the token is an operator.
 * @return false otherwise.
 */
bool RPN::isOperator(const std::string& token) const {
	return (token.length() == 1
		&& (token[0] == '+'
			|| token[0] == '-'
			|| token[0] == '*'
			|| token[0] == '/'));
}
