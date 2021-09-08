#include "Queue.h"
using std::cin;
using std::cout;
using std::endl;
using std::string;
int main() {
	int length;
	cin >> length;
	Queue<int>queue(length);
	while (true) {
		string cmd;
		cin >> cmd;
		if (cmd == "quit") {
			cout << queue << endl;
			break;
		}
		if (cmd == "dequeue") {
			try {
				cout << queue.pop() << endl;
			}
			catch (const std::out_of_range &ex) {
				cout << "Queue is Empty" << endl;
			}
		}
		else if (cmd == "enqueue") {
			int value;
			cin >> value;
			if (queue.capacity() == queue.size())
				cout << "Queue is Full" << endl;
			else
				queue.push(value);
		}
	}
}