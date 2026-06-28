#ifndef RPN_HPP
#define RPN_HPP

#include <stack>
#include <string>

class RPN {
	private:
		// checks whether the token is a supported operator
		bool isOperator(const std::string& token) const;
		// checks whether the token is a valid operand
		bool isNumber(const std::string& token) const;

		// performs an operation using the top two stack elements
		void applyOperator(char op);
	public:
		RPN();
		RPN(const RPN& other);
		~RPN();
		RPN& operator=(const RPN& other);

		// evaluates an RPN expression and returns the result
		int evaluate(const std::string& expression);
};

#endif
