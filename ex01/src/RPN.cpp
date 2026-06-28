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

/**
 * @brief Checks whether a token is a valid number.
 * 
 * According to the subject, input numbers are always less than 10.
 * Therefore a valid number token must contain exactly one digit.
 * 
 * @param token Token to check.
 * @return true if the token is a one-digit number.
 * @return false otherwise.
 */
bool RPN::isNumber(const std::string& token) const {
	return (token.length() == 1
		&& token[0] >= '0'
		&& token[0] <= '9');
}

/**
 * @brief Applies an operator to the top two elements of the stack.
 * 
 * The function removes two values from the stack, applies the operation
 * in the correct order and pushes the result back.
 * The first popped value is the right operand.
 * The second popped value is the left operand.
 * Example:
 * 	expression: 7 3 -
 * 	a = 3
 * 	b = 7
 * 	result = b - a = 4
 * 
 * @param stack Stack containing operands.
 * @param op Operator to apply.
 * @throws std::runtime_error If there are not enough operands.
 * @throws std::runtime_error If division by zero occurs.
 */
void RPN::applyOperator(std::stack<int>& stack, char op) const {
	if (stack.size() < 2)
		throw std::runtime_error("Error");

	int a = stack.top();
	stack.pop();

	int b = stack.top();
	stack.pop();

	int result = 0;

	if (op == '+')
		result = b + a;
	if (op == '-')
		result = b - a;
	if (op == '*')
		result = b * a;
	if (op == '/') {
		if (a == 0)
			throw std::runtime_error("Error");
		result = b / a;
	}

	stack.push(result);
}

/**
 * @brief Evaluates an RPN expression.
 * 
 * The expression is read token by token from left to right.
 * If the token is a number, it is pushed to the stack.
 * If the token is an operator, th eoperation is applied to the top two
 * stack elements.
 * At the end of evaluation, exactly one value must remain in the stack.
 * This value is the result of the expression.
 * 
 * @param expression RPN expression passes as a string.
 * @return Result of the expression.
 * @throws std::runtime_error If the expression is invalid.
 */
int RPN::evaluate(const std::string& expression) {
	std::stringstream ss(expression);
	std::string token;
	std::stack<int> stack;

	while (ss >> token) {
		if (isNumber(token))
			stack.push(token[0] - '0');
		else if (isOperator(token))
			applyOperator(stack, token[0]);
		else
			throw std::runtime_error("Error");
	}

	if (stack.size() != 1)
		throw std::runtime_error("Error");
	
	return stack.top();
}
