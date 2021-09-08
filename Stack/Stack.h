#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
namespace My {
	using std::cin;
	using std::cout;
	using std::end;
	template <class T>
	class Stack {
	private:
		//the last position of the stack
		int tail;
		//the allocated size of the stack
		int capacity;
		//data pointer
		T *list;
	public:
		Stack() :list(nullptr), tail(0), capacity(0) {}
		Stack(int length) :tail(0), capacity(length) {
			list = new T[length];
		}
		//copy constructor
		Stack(Stack<T> &stack) :tail(stack.tail), capacity(stack.capacity) {
			list = new T[capacity];
			for (int i = 0; i < capacity; i++)
				list[i] = stack.list[i];
		}
		//move constructor
		Stack(Stack<T> &&stack) :tail(stack.tail), capacity(stack.capacity), list(stack.list) {
			stack.tail = stack.capacity = 0;
			stack.list = nullptr;
		}
		//adjust the capacity of the stack
		void resize(int size = 0) {
			if (!size)
				size = capacity ? capacity << 1 : 1;
			if (size == tail)
				return;
			if (size > tail) {
				auto oldList = list;
				list = new T[capacity = size];
				if (oldList) {
					memcpy(list, oldList, sizeof(T) * tail);
					delete oldList;
				}
			}
			else if (size < tail)
				tail = size;;
		}

		bool empty() const {
			return tail == 0;
		}

		int length() const {
			return tail;
		}
		
		void clear() {
			tail = 0;
		}
		//push an element into the stack
		void push(const T &value) {
			if (tail == capacity)
				resize();
			list[tail++] = value;
		}
		//pop out the top element of the stack
		T pop() {
			if (empty())
				throw "Empty stack";
			return std::move(list[--tail]);
		}

		T &top() {
			if (empty())
				throw "Empty stack";
			return list[tail - 1];
		}

		T top() const{
			if (empty())
				throw "Empty stack";
			return list[tail - 1];
		}

		operator bool() const {
			return tail > 0;
		}
		bool operator ! () const {
			return tail == 0;
		}

		T operator [] (const int &index) const {
			return list[index];
		}
		T& operator [] (const int &index){
			return list[index];
		}
		Stack &operator = (const Stack &stack) {
			tail = stack.tail;
			capacity = stack.capacity;
			list = new T[capacity];
			for (int i = 0; i < capacity; i++)
				list[i] = stack.list[i];
			return *this;
		}
		Stack &operator = (Stack &&stack) {
			tail = stack.tail;
			capacity = stack.capacity;
			list = stack.list;
			stack.tail = stack.capacity = 0;
			stack.list = nullptr;
		}
		//destructor
		~Stack() {
			if (list)
				delete[] list;
		}
	};

	template<class T>
	std::ostream &operator << (std::ostream &out, const Stack<T> &stack) {
		int length = stack.length();
		for (int i = 0; i < length - 1; i++)
			out << stack[i] << ' ';
		if (length > 0)
			out << stack[length - 1];
		return out;
	}
}
