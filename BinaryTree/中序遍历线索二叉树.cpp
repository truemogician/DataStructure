#include "ThreadedBinaryTree.h"
using namespace My;
using namespace My::Tree;
using std::cin;
using std::cout;
using std::endl;
using std::string;
using ThreadedTree = ThreadedBinaryTree<char, TraversalOrder::Inorder>;
int main() {
	string sequence;
	cin >> sequence;
	auto tree = ThreadedTree::create(LinkList<char>(sequence.c_str(), sequence.length()), '#');
	char queriedNode;
	cin >> queriedNode;
	SharedPointer<ThreadedTree> target;
	for (auto i = tree->begin(); i != tree->end(); ++i) {
		cout << *i;
		if (*i == queriedNode)
			target = i;
	}
	cout << endl;
	if (target) {
		typename ThreadedTree::Iterator prev(target);
		--prev;
		typename ThreadedTree::Iterator succ(target);
		++succ;
		SharedPointer<ThreadedTree> prior = prev;
		SharedPointer<ThreadedTree> next = succ;
		cout << "succ is ";
		if (next == nullptr)
			cout << "NULL" << endl;
		else
			cout << next->value << next->isRightAThread() << endl;
		cout << "prev is ";
		if (prior == nullptr)
			cout << "NULL" << endl;
		else
			cout << prior->value << prior->isLeftAThread() << endl;
	}
	else
		cout << "Not found" << endl;
	return 0;
}