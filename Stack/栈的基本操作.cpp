#include "Stack.h"
using namespace My;
using std::string;
using std::endl;
int main() {
	int capacity;
	cin >> capacity;
	Stack<int> stack(capacity);
	string cmd;
	int value;
	while (true) {
		cin >> cmd;
		if (cmd == "quit") {
			while (stack)
				cout << stack.pop() << ' ';
			cout << endl;
			break;
		}
		else if (cmd == "push") {
			cin >> value;
			if (stack.length() == capacity)
				cout << "Stack is Full" << endl;
			else
				stack.push(value);
		}
		else if (cmd == "pop") {
			if (stack.empty())
				cout << "Stack is Empty" << endl;
			else
				cout << stack.pop() << endl;
		}
	}
	return 0;
}