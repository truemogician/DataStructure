#include "pch.h"
#include "CppUnitTest.h"
#include "../AVL/AVL.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace My::Tree;

namespace AVLTest {
	TEST_CLASS(AVLTest) {
		Set<int>set;
		void initialize(int n=7) {
			for (int i = 1; i <= n; ++i)
				set.insert(i);
		}
	public:
		TEST_METHOD(insert) {
			initialize(10);
			Assert::AreEqual<int>(4, set.root->getHeight());
		}
		TEST_METHOD(remove) {
			initialize(3);
			Assert::IsTrue(set.remove(2));
			Assert::IsFalse(set.remove(0));
		}
		TEST_METHOD(find) {
			initialize(7);
			Assert::IsNull(set.find(0));
			Assert::IsNull(set.find(8));
			Assert::AreEqual<int>(4, set.find(4)->value);
		}
		TEST_METHOD(isEmpty) {
			initialize(3);
			for (int i = 1; i <= 3; ++i)
				set.remove(i);
			Assert::IsTrue(set.isEmpty());
		}
		TEST_METHOD(getSize) {
			initialize(100);
			Assert::AreEqual<int>(100, set.getSize());
		}
	};
}
