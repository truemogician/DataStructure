#include "pch.h"
#include "CppUnitTest.h"
#include "..\BinaryTree\ThreadedBinaryTree.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ThreadedBinaryTreeTest {
	using namespace My::Tree;
	TEST_CLASS(ThreadedBinaryTreeTest) {
	public:
		TEST_METHOD(preorder) {
			auto threaded = ThreadedBinaryTree<int, TraversalOrder::Preorder>::create(LinkList<int>(1, 2, 3, 0, 0, 4, 5, 0, 0, 0, 6, 0, 7, 8, 0, 0, 9, 0, 0), 0);
			int count = 1;
			for (auto i = threaded->begin(); i != threaded->end(); ++i, ++count) {
				auto value = *i;
				Assert::AreEqual<int>(count, value);
			}
		}
		TEST_METHOD(inorder) {
			auto threaded = ThreadedBinaryTree<int, TraversalOrder::Inorder>::create(LinkList<int>(5, 2, 1, 0, 0, 4, 3, 0, 0, 0, 6, 0, 8, 7, 0, 0, 9, 0, 0), 0);
			int count = 1;
			for (auto i = threaded->begin(); i != threaded->end(); ++i, ++count) {
				auto value = *i;
				Assert::AreEqual<int>(count, value);
			}
		}
		TEST_METHOD(postorder) {
			auto threaded = ThreadedBinaryTree<int, TraversalOrder::Postorder>::create(LinkList<int>(9, 4, 1, 0, 0, 3, 2, 0, 0, 0, 8, 0, 7, 5, 0, 0, 6, 0, 0), 0);
			int count = 1;
			for (auto i = threaded->begin(); i != threaded->end(); ++i, ++count) {
				auto value = *i;
				Assert::AreEqual<int>(count, value);
			}
		}
	};
}
