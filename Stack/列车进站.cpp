#include "Stack.h"
#include <cstring>
#include <map>
using namespace My;
using std::string;
using std::map;
using std::endl;
int main() {
	string sequence;
	cin >> sequence;
	map<char, int> index;
	for (int i = 0; i < sequence.length(); i++)
		index[sequence[i]] = i;
	Stack<char>stack((int)sequence.length());
	string test;
	while (cin >> test) {
		for (int i = 0,j=0; i < test.length(); i++) {
			if (stack.empty() || index[test[i]] > index[stack.top()]) {
				while (j<sequence.length() && (stack.empty() || stack.top() != test[i]))
					stack.push(sequence[j++]);
				if (j > sequence.length())
					break;
			}
			if (stack.top() != test[i])
				break;
			stack.pop();
		}
		cout << (stack.empty() ? "yes" : "no") << endl;
		stack.clear();
	}
}