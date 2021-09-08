#include "ThreadedBinaryTree.h"
using namespace My;
using namespace My::Tree;
using std::cin;
using std::cout;
using std::endl;
using std::string;
template <class T, TraversalOrder TOrder>
void drawTreeStructure(const SharedPointer<ThreadedBinaryTree<T, TOrder>> tree, const int &depth = 0) {
	if (tree->hasRightChild())
		drawTreeStructure(tree->rightChild, depth + 1);
	for (int i = 0; i < depth; i++)
		cout << "     ";
	cout << tree->value << tree->isLeftAThread() << tree->isRightAThread() << endl;
	if (tree->hasLeftChild())
		drawTreeStructure(tree->leftChild, depth + 1);
}
int main() {
	string sequence;
	cin >> sequence;
	auto tree = ThreadedBinaryTree<char,TraversalOrder::Preorder>::create(LinkList<char>(sequence.c_str(), sequence.length()), '#');
	drawTreeStructure<char, TraversalOrder::Preorder>(tree);
	for (auto i = tree->begin(); i != tree->end(); ++i)
		cout << *i;
	cout << endl;
	return 0;
}