#include "Stack.h"
#include <functional>
#include <map>
#include <exception>
using namespace My;
using std::function;
using std::map;
using std::string;
using std::exception;
using std::make_pair;
using std::endl;
template <class T>
class Operator {
public:
	using Operation = function<T(const T &, const T &)>;
	using Verification = function<bool(const T &, const T &)>;
	char symbol;
	int priority;
	Operation operation;
	Verification verification = [](const T &a, const T &b) {return true; };
	Operator() :symbol(0),priority(-1){}
	Operator(char symbol, int priority) :symbol(symbol), priority(priority) {}
	Operator(char symbol, int priority, const Operation operation) :symbol(symbol), priority(priority) ,operation(operation){}
	bool verify(const T &a, const T &b) {
		return verification(a, b);
	}
	T operate(const T &a, const T &b) {
		return operation(a, b);
	}
	bool operator < (const Operator &opt) const {
		return priority < opt.priority;
	}
	bool operator >= (const Operator &opt) const {
		return priority >= opt.priority;
	}
};
template <class T>
class Expression {
	using Operation = function<T(const T &, const T &)>;
	using Verification = function<bool(const T &, const T &)>;
public:
	Stack<Operator<T>> operators;
	string expression;
	Expression() :expression("") {}
	Expression(int operatorsCount, string expression = "") :operators(operatorsCount), expression(expression) {}
	void addOperator(const Operator<T> &opt) {
		operators.push(opt);
	}
	void addOperator(char symbol, int priority, const Operation &operation) {
		operators.push(Operator<T>(symbol, priority, operation));
	}
	void addOperator(char symbol, int priority, const Operation &operation, const Verification& verification) {
		auto opt = Operator<T>(symbol, priority, operation);
		opt.verification = verification;
		operators.push(opt);
	}
	T calculate() {
		map<char, T> symbolToIndex;
		for (int i = 0; i < operators.length(); i++)
			symbolToIndex.insert(make_pair(operators[i].symbol, i));
		const int initialCapacity = 1 << 5;
		Stack<T>operands(initialCapacity);
		Stack<char>symbols(initialCapacity);
		bool newValue = false;
		T value = 0;
		for (int i = 0; i < expression.length(); i++) {
			if (isdigit(expression[i])) {
				newValue = true;
				value = value * 10 + expression[i] - '0';
			}
			else if (expression[i] == '(')
				symbols.push(expression[i]);
			else if (expression[i] == ')') {
				if (symbols.empty())
					throw exception("Brackets not match");
				if (newValue) {
					operands.push(value);
					value = 0;
					newValue = false;
				}
				char symbol;
				bool bracketMatched = false;
				while (!symbols.empty()) {
					symbol = symbols.pop();
					if (symbol == '(') {
						bracketMatched = true;
						break;
					}
					T rightOperand = operands.pop();
					T leftOperand = operands.pop();
					Operator<T> &curOperator = operators[symbolToIndex[symbol]];
					if (!curOperator.verify(leftOperand, rightOperand)) 
						throw exception("Invalid operands");
					operands.push(curOperator.operate(leftOperand, rightOperand));
				}
				if (!bracketMatched)
					throw exception("Brackets not match");
			}
			else {
				bool isLegalSymbol = false;
				int curIndex;
				for (curIndex = 0; curIndex < 4; curIndex++)
					if (expression[i] == operators[curIndex].symbol) {
						isLegalSymbol = true;
						break;
					}
				if (!isLegalSymbol)
					throw exception("Invalid symbol");
				if (newValue) {
					operands.push(value);
					value = 0;
					newValue = false;
				}
				char symbol;
				while (!symbols.empty()) {
					symbol = symbols.top();
					if (symbol == '(')
						break;
					Operator<T> &curOperator = operators[symbolToIndex[symbol]];
					if (curOperator < operators[symbolToIndex[expression[i]]])
						break;
					symbols.pop();
					T rightOperand = operands.pop();
					T leftOperand = operands.pop();
					if (!curOperator.verify(leftOperand, rightOperand)) 
						throw exception("Invalid operands");
					operands.push(curOperator.operate(leftOperand, rightOperand));
				}
				symbols.push(expression[i]);
			}
		}
		if (newValue)
			operands.push(value);
		while (!symbols.empty()) {
			char symbol = symbols.pop();
			T rightOperand = operands.pop();
			T leftOperand = operands.pop();
			Operator<T> &curOperator = operators[symbolToIndex[symbol]];
			if (!curOperator.verify(leftOperand, rightOperand)) 
				throw exception("Invalid operands");
			operands.push(curOperator.operate(leftOperand, rightOperand));
		}
		return operands.pop();
	}
};
int main() {
	Expression<int> exp(4);
	exp.addOperator('+', 1, [](const int &a, const int &b) {return a + b; });
	exp.addOperator('-', 1, [](const int &a, const int &b) {return a - b; });
	exp.addOperator('*', 2, [](const int &a, const int &b) {return a * b; });
	exp.addOperator('/', 2, [](const int &a, const int &b) {return a / b; }, [](const int &a, const int &b) {return b != 0; });
	string expString;
	cin >> expString;
	exp.expression = expString.substr(0, expString.length() - 1);
	try {
		cout << exp.calculate();
	}
	catch (const exception& ex){
		cout << "ERROR";
	}
	return 0;
}