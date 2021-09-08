#include <iostream>
#include <string>
#include "BinaryTree.h"
using namespace My;
using namespace My::Tree;
using std::cin;
using std::cout;
using std::endl;using std::string;
template <class T>
void drawTreeStructure(const SharedPointer<BinaryTree<T>> tree, const int &depth = 0) {
	if (tree->rightChild)
		drawTreeStructure(tree->rightChild, depth + 1);
	for (int i = 0; i < depth; i++)
		cout << "     ";
	cout << tree->value << endl;
	if (tree->leftChild)
		drawTreeStructure(tree->leftChild, depth + 1);
}
int main() {
	string sequence;
	cin >> sequence;
	auto tree = BinaryTree<char>::create(LinkList<char>(sequence.c_str(), sequence.length()), '#');
	cout << tree->getTraversalSequence(TraversalOrder::Preorder) << endl
		<< tree->getTraversalSequence(TraversalOrder::Inorder) << endl
		<< tree->getTraversalSequence(TraversalOrder::Postorder) << endl
		<< tree->getTraversalSequence(TraversalOrder::Levelorder) << endl;
	drawTreeStructure(tree);
	return 0;
}