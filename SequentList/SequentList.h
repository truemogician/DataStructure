#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <memory>
#include <utility>
#include <functional>
#include <cstring>
#include <algorithm>
using std::cin;
using std::cout;
using std::end;
using std::shared_ptr;
#define SharedPointer shared_ptr
using std::make_shared;
#define makeShared make_shared
using std::pair;
#define Pair pair
using std::make_pair;
#define makePair make_pair
using std::function;
#define Function function
template <class T>
class SequentList {
private:
	//the last position of the sequent list
	int tail;
	//the allocated size of the sequent list
	int capacity;
	//data pointer
	T *list;
public:
	using MatchFunction = Function<bool(const T &, const T &)>;
	//default matching function, requiring == operator
	static const MatchFunction defaultMatch;
	SequentList() :list(nullptr), tail(0), capacity(0) {}
	SequentList(int length) :tail(0), capacity(length) {
		list = new T[length];
	}
	//copy constructor
	SequentList(SequentList<T> &seq) :tail(seq.tail), capacity(seq.capacity) {
		list = new T[capacity];
		memcpy(list, seq.list, sizeof(T) * capacity);
	}
	//move constructor
	SequentList(SequentList<T> &&seq) :tail(seq.tail), capacity(seq.capacity), list(seq.list) {
		seq.list = nullptr;
	}
	//adjust the capacity of the sequent list
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
	int length() const {
		return tail;
	}
	//fill the sequent list with a specific value
	void fill(const T &value) {
		for (int i = 0; i < tail; i++)
			list[i] = value;
	}
	//add an element to the tail
	void add(const T &value) {
		if (tail == capacity)
			resize();
		list[tail++] = value;
	}
	//insert an element to the ordered sequent list, using binary search
	int orderedInsert(const T &value) {
		if (!list)
			add(value);
		int l = 0, r = tail - 1;
		while (l <= r) {
			int m = l + ((r - l) >> 1);
			if (value <= list[m])
				r = m - 1;
			else
				l = m + 1;
		}
		if (l < tail)
			insert(l, value);
		else
			add(value);
		return l;
	}
	//insert an element to a specific position of the sequent list
	bool insert(int pos, const T &value) {
		if (pos > tail)
			return false;
		if (tail == capacity)
			resize();
		for (int i = (tail++) - 1; i >= pos; i--)
			list[i + 1] = list[i];
		list[pos] = value;
		return true;
	}
	//remove an element at a specific position
	bool removeAt(int pos) {
		if (pos >= tail)
			return false;
		tail--;
		for (int i = pos; i < tail; i++)
			list[i] = list[i + 1];
		return true;
	}
	//remove the first element which matches the given value
	int removeOne(const T &value, MatchFunction match = defaultMatch) {
		auto result = query(value, match);
		if (result == -1)
			return -1;
		removeAt(result);
		return result;
	}
	//remove all the elements which matches the given value
	int removeAll(const T &value, MatchFunction match = defaultMatch) {
		if (!list)
			return -1;
		int count = 0;
		bool *mark = new bool[tail];
		memset(mark, false, sizeof(bool) * tail);
		for (int i = 0; i < tail; i++)
			if (match(list[i], value)) {
				count++;
				mark[i] = true;
			}
		if (!count)
			return 0;
		int offset = 0;
		for (int i = 0; i < tail; i++) {
			if (mark[i])
				offset++;
			else
				list[i - offset] = list[i];
		}
		tail -= count;
		return count;
	}
	//check whether the elements are in increasing order
	bool isOrdered() const {
		if (!list)
			return false;
		for (int i = 0; i < tail - 1; i++)
			if (!(list[i] < list[i + 1]))
				return false;
		return true;
	}
	//query the first element which matches the given value, returning -1 if no elements match
	int query(const T &value, MatchFunction match = defaultMatch) const {
		for (int i = 0; i < tail; i++)
			if (match(list[i], value))
				return i;
		return -1;
	}
	//sort the sequent list, acquring < operator
	void sort() {
		std::sort(list, list + tail);
	}
	//reverse the sequent list
	void reverse() {
		if (!list)
			return;
		for (int i = 0; i < (tail >> 1); i++)
			std::swap(list[i], list[tail - i - 1]);
	}
	//remove the duplicate elements in the sequent list
	void unify() {
		if (!list)
			return;
		int count = 0;
		bool *mark = new bool[tail];
		memset(mark, false, sizeof(bool) * tail);
		for (int i = 0; i < tail; i++)
			for (int j = 0; j < i; j++)
				if (list[i] == list[j]) {
					count += (mark[i] = true);
					break;
				}
		int offset = 0;
		for (int i = 0; i < tail; i++) {
			if (mark[i])
				offset++;
			else
				list[i - offset] = list[i];
		}
		tail -= count;
	}
	//concat two sequent list
	void concat(const SequentList<T> &seq) {
		int len = seq.tail;
		if (tail + len >= capacity)
			resize(tail + len);
		for (int i = 0; i < len; i++)
			list[tail++] = seq[i];
	}
	//merge two ordered sequent list
	void merge(const SequentList<T> &seq) {
		int len = seq.tail;
		if (tail + len > capacity)
			capacity = tail + len;
		T *newList = new T[capacity];
		int k = 0;
		for (int i = 0, j = 0; i < tail || j < len;) {
			for (; i < tail && (list[i] <= seq[j] || j >= len); i++)
				newList[k++] = list[i];
			for (; j < len && (seq[j] <= list[i] || i >= tail); j++)
				newList[k++] = seq[j];
		}
		T *oldList = list;
		list = newList;
		tail = k;
		if (oldList)
			delete oldList;
	}
	operator bool() const {
		return list != nullptr;
	}
	bool operator ! () const {
		return list == nullptr;
	}
	T &operator [] (const int &index) {
		return list[index];
	}
	T operator [] (const int &index) const {
		return list[index];
	}
	SequentList<T> &operator = (const SequentList<T> &seq) {
		tail = seq.tail;
		capacity = seq.capacity;
		list = new T[capacity];
		memcpy(list, seq.list, sizeof(T) * capacity);
		return *this;
	}
	//destructor
	~SequentList() {
		if (list)
			delete list;
	}
};
template <class T>
const Function<bool(const T &, const T &)> SequentList<T>::defaultMatch = [](const T &a, const T &b) {return a == b; };
template<class T>
std::ostream &operator << (std::ostream &out, const SequentList<T> &seq) {
	int length = seq.length();
	for (int i = 0; i < length - 1; i++)
		out << seq[i] << ' ';
	if (length > 0)
		out << seq[length - 1];
	return out;
}