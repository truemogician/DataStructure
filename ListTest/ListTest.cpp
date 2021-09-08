#include "pch.h"
#include "CppUnitTest.h"
#include "..\List\List.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ListTest {
	using namespace My;
	using namespace My::LinearList;
	TEST_CLASS(ListTest) {
		LinkList<int> listA, listB;
	public:
		TEST_METHOD(append) {
			listA.List<int>::append(1, 2, 3, 4);
			Assert::AreEqual(4, listA.getLength());
		}
		TEST_METHOD(insert) {
			listA=LinkList<int>(1, 2, 3, 4, 5);
			listA.insert(0, 0);
			listA.insert(4, 1926);
			Assert::AreEqual(0, listA[0]);
			Assert::AreEqual(1926, listA[4]);
			Assert::AreEqual(7, listA.getLength());
		}
		TEST_METHOD(remove) {
			listA=LinkList<int>(1, 2, 3, 4, 5);
			listA.remove(0);
			listA.remove(1);
			Assert::AreEqual(3, listA.getLength());
			Assert::AreEqual(2, listA[0]);
		}
		TEST_METHOD(concat) {
			listA=LinkList<int>(0, 1, 2, 3, 4, 5);
			listB = LinkList<int>(6, 7, 8, 9);
			listA.concat(std::move(listB));
			Assert::AreEqual(10, listA.getLength());
			Assert::AreEqual(0, listB.getLength());
			for (int i = 0; i < 10; i++)
				Assert::AreEqual(i, listA[i]);
			LinkList<int> listC;
			listA.concat(std::move(listC));
			Assert::AreEqual(10, listA.getLength());
			Assert::AreEqual(0, listC.getLength());
			for (int i = 0; i < 10; i++)
				Assert::AreEqual(i, listA[i]);
		}
		TEST_METHOD(split) {
			listA = LinkList<int>(0, 1, 2, 3, 4, 5);
			auto result = listA.split(2);
			Assert::AreEqual(2, result[0]->getLength());
			Assert::AreEqual(4, result[1]->getLength());
			for (int i = 0; i < 2; i++)
				Assert::AreEqual(i, (*result[0])[i]);
			for (int i = 2; i < 6; i++)
				Assert::AreEqual(i, (*result[1])[i-2]);
		}
		TEST_METHOD(iterator) {
			listA = LinkList<int>(0, 1, 2, 3, 4, 5);
			int counter = 0;
			for (LinkList<int>::Iterator i = listA.begin(); i != listA.end(); ++i, ++counter)
				Assert::AreEqual(counter, *i);
			for (LinkList<int>::Iterator i = listA.begin(); i != listA.end(); ++i) {
				*i *= 2;
			}
			for (int i = 0; i < 6; i++)
				Assert::AreEqual(i * 2, listA[i]);
			const LinkList<int> listC(0, 1, 2, 3);
			counter = 0;
			for (auto i = listC.begin(); i != listC.end(); ++i, ++counter)
				Assert::AreEqual(counter, *i);
		}
		TEST_METHOD(copyConstructor){
			listA = LinkList<int>(0, 1, 2, 3, 4);
			LinkList<int> listC(listA);
			for (auto i = listA.begin(), j = listC.begin(); i != listA.end() && j != listC.end(); ++i, ++j)
				Assert::IsFalse(i==j);
			Assert::AreEqual(5, listA.getLength());
			Assert::AreEqual(5, listC.getLength());
		}
		TEST_METHOD(moveConstructor) {
			listA = LinkList<int>(0, 1, 2, 3, 4);
			LinkList<int> listC(move(listA));
			Assert::IsTrue(listA.isEmpty());
			int counter = 0;
			for (LinkList<int>::Iterator i = listC.begin(); i != listC.end(); ++i, ++counter)
				Assert::AreEqual(counter, *i);
			Assert::AreEqual(0, listA.getLength());
			Assert::AreEqual(5, listC.getLength());
		}
		TEST_METHOD(toArray) {
			listA = LinkList<int>(0, 1, 2, 3, 4);
			auto array = listA.toArray();
			for (int i = 0; i < listA.getLength(); ++i)
				Assert::AreEqual(i, array[i]);
		}
		TEST_METHOD(sort) {
			listA = LinkList<int>(1, 9, 2, 6, 0, 8, 1, 7);
			listB = listA;
			LinkList<int> expectedA(0, 1, 1, 2, 6, 7, 8, 9);
			Assert::IsTrue(expectedA == listA.sort());
			LinkList<int> expectedB(1, 9, 8, 6, 2, 1, 0, 7);
			Assert::IsTrue(expectedB == listB.sort(2, 7, [](const int &a, const int &b) {return a >= b; }));
		}
	};
}
