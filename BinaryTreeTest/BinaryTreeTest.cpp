#include "pch.h"
#include "CppUnitTest.h"
#include "..\BinaryTree\BinaryTree.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BinaryTreeTest {
	using namespace My;
	using namespace My::Tree;
	TEST_CLASS(BinaryTreeTest) {
	public:
		TEST_METHOD(create) {
			auto tree = BinaryTree<int>::create(LinkList<int>(0, 1, 2, -1, -1, 3, -1, -1, 4, -1, -1), -1);
			Assert::AreEqual(5, tree->getSize());
		}
		TEST_METHOD(getTraversalSequence) {
			auto tree = BinaryTree<int>::create(LinkList<int>(0, 1, 2, -1, -1, 3, -1, -1, 4, -1, -1), -1);
			auto preSeq = tree->getTraversalSequence(TraversalOrder::Preorder);
			auto inSeq = tree->getTraversalSequence(TraversalOrder::Inorder);
			auto postSeq = tree->getTraversalSequence(TraversalOrder::Postorder);
			auto expectedPreSeq = LinkList<int>(0, 1, 2, 3, 4);
			auto expectedInSeq = LinkList<int>(2, 1, 3, 0, 4);
			auto expectedPostSeq = LinkList<int>(2, 3, 1, 4, 0);
			Assert::IsTrue(expectedPreSeq == preSeq);
			Assert::IsTrue(expectedInSeq == inSeq);
			Assert::IsTrue(expectedPostSeq == postSeq);
		}
		TEST_METHOD(heightAndWidth) {
			auto tree = BinaryTree<int>::create(LinkList<int>(0, 1, 2, -1, -1, 3, -1, -1, 4, 5, -1, -1, 6, -1, -1), -1);
			Assert::AreEqual(3, tree->getHeight());
			Assert::AreEqual(4, tree->getWidth());
		}
		TEST_METHOD(copy) {
			auto tree = BinaryTree<int>::create(LinkList<int>(0, 1, -1, -1, 3, -1, -1), -1);
			auto anotherTree = makeShared<BinaryTree<int>>(tree, nullptr);
			Assert::AreEqual(tree->getSize(), anotherTree->getSize());
			Assert::IsTrue(tree->getTraversalSequence()==anotherTree->getTraversalSequence());
		}
		TEST_METHOD(getCommonAncestor) {
			auto tree = BinaryTree<int>::create(LinkList<int>(0, 1, 2, -1, -1, 3, -1, -1, 4, 5, -1, -1, 6, -1, -1), -1);
			auto nodeA = tree;
			auto nodeB = tree->rightChild;
			auto nodeE = tree->leftChild;
			auto nodeC = tree->leftChild->leftChild;
			auto nodeD = tree->leftChild->rightChild;
			Assert::IsTrue(nodeE == BinaryTree<int>::getLatestCommonAncestor(nodeC, nodeD));
			Assert::IsTrue(nodeA == BinaryTree<int>::getLatestCommonAncestor(nodeC, nodeB));
			Assert::IsTrue(nodeA == BinaryTree<int>::getLatestCommonAncestor(nodeC, nodeA));
			Assert::IsTrue(nodeE == BinaryTree<int>::getLatestCommonAncestor(nodeE, nodeE));
		}
	};
}
