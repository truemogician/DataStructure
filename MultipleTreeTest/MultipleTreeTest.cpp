#include "pch.h"
#include "CppUnitTest.h"
#include "../MultipleTree/MultipleTree.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MultipleTreeTest {
	using namespace My;
	using namespace My::Tree;
	TEST_CLASS(MultipleTreeTest) {
	public:
		TEST_METHOD(create) {
			LinkList<int> list(1, 2, 5, 0, 6, 8, 0, 9, 0, 10, 0, 11, 0, 0, 0, 3, 7, 0, 0, 4, 0);
			auto tree = MultipleTree<int>::create(move(list), 0);
			Assert::AreEqual<int>(11, tree->getSize());
			Assert::AreEqual<int>(4, tree->getHeight());
			Assert::AreEqual<int>(7, tree->getWidth());
			LinkList<int> preExpected(1, 2, 5, 6, 8, 9, 10, 11, 3, 7, 4);
			LinkList<int> levelExpected(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
			LinkList<int> postExpected(5, 8, 9, 10, 11, 6, 2, 7, 3, 4, 1);
			Assert::IsTrue(tree->getTraversalSequence(TraversalOrder::Preorder) == preExpected);
			Assert::IsTrue(tree->getTraversalSequence(TraversalOrder::Postorder) == postExpected);
			Assert::IsTrue(tree->getTraversalSequence(TraversalOrder::Levelorder) == levelExpected);
		}
	};
}
