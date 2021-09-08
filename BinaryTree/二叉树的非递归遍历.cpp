#include "BinaryTree.h"
#include <iostream>
#include <stdexcept>
using std::cin;
using std::cout;
using std::endl;
using std::string;
using namespace My::Tree;
int main() {
	LinkList<char>seqList;
	int size;
	cin >> size;
	for (int i = 0; i < (size << 1); ++i) {
		string opt;
		cin >> opt;
		if (opt == "push") {
			char value;
			cin >> value;
			seqList.append(value);
		}
		else if (opt == "pop")
			seqList.append('\0');
		else
			throw std::invalid_argument("wrong command");
	}
	seqList.append('\0');
	cout << BinaryTree<char>::create(std::move(seqList), '\0')->
		getTraversalSequence(TraversalOrder::Postorder) << endl;
	return 0;
}