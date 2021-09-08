#include "pch.h"
#include "CppUnitTest.h"
#include "..\Queue\Queue.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace QueueTest {
	using namespace My::LinearList;
	TEST_CLASS(QueueTest) {
	public:
		TEST_METHOD(Constructor) {
			Queue<int>queue(10);
			Assert::AreEqual<int>(10, queue.capacity());
		}
		TEST_METHOD(CopyConstructor) {
			Queue<int>queue(3);
			for (int i = 0; i < 3; i++)
				queue.push(i);
			Queue<int>newQueue(queue);
			bool equal = true;
			for (int i=0; i<3; i++)
				if (queue[i] != newQueue[i]) {
					equal = false;
					break;
				}
			Assert::IsTrue(equal);
		}
		TEST_METHOD(MoveConstructor) {
			Queue<int>queue(3);
			for (int i = 0; i < 3; i++)
				queue.push(i);
			Queue<int>newQueue(std::move(queue));
			Assert::IsTrue(queue.empty());
		}
		TEST_METHOD(Push) {
			Queue<int>queue;
			queue.push(10).push(9).push(8);
			Assert::AreEqual<int>(3, queue.size());
		}
		TEST_METHOD(Pop) {
			Queue<int>queue;
			queue.push(10);
			int value = queue.pop();
			Assert::IsTrue(value == 10 && queue.empty());
		}
		TEST_METHOD(Clear) {
			const int fillValue = 19260817;
			Queue<int>queue;
			queue.push(10);
			queue.push(123456);
			queue.push(9876);
			queue.clear(fillValue);
			bool passed = true;
			for (int i=0; i<queue.size(); i++)
				if (queue[i] != fillValue) {
					passed = false;
					break;
				}
			Assert::IsTrue(passed);
		}
		TEST_METHOD(Resize) {
			Queue<int>queue(3);
			queue.resize(100);
			Assert::AreEqual<int>(100, queue.capacity());
		}
		TEST_METHOD(Exception1) {
			Queue<int>queue;
			bool passed = false;
			try {
				queue.pop();
			}
			catch (const std::out_of_range &ex) {
				passed = true;
			}
			Assert::IsTrue(passed);
		}
		TEST_METHOD(Exception2) {
			Queue<int>queue;
			queue.push(1926);
			bool passed = false;
			try {
				int tmp = queue[817];
				int tmp2 = queue[-2];
			}
			catch (const std::out_of_range &ex) {
				passed = true;
			}
			Assert::IsTrue(passed);
		}
		TEST_METHOD(HeadAndTail) {
			Queue<int>queue;
			for (int i = 0; i < 10; i++)
				queue.push(i);
			Assert::IsTrue(queue.head() == 0 && queue.tail() == 9);
		}
	};
}
