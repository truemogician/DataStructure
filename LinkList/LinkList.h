#define _CRT_SECURE_NO_WARNINGS
#define SharedPointer shared_ptr
#define makeShared make_shared
#define Pair pair
#define makePair make_pair
#define Function function
#include <cstdio>
#include <iostream>
#include <cstring>
#include <memory>
#include <utility>
#include <functional>
namespace My {
	using std::cin;
	using std::cout;
	using std::endl;
	using std::ostream;
	using std::pair;
	using std::make_pair;
	using std::shared_ptr;
	using std::make_shared;
	using std::function;
	template <class T>
	void sort(T *start, T *end, const Function<bool(const T &, const T &)> &lessThan = [](const T &a, const T &b) {return a < b; }) {
		if (end - start <= 1)
			return;
		T *left = start, *right = end - 1;
		T ref = *start;
		while (left < right) {
			for (; left < right && !lessThan(*right, ref); right--);
			if (left == right)
				break;
			*left = *right;
			left++;
			if (left == right)
				break;
			for (; left < right && lessThan(*left, ref); left++);
			if (left == right)
				break;
			*right = *left;
			right--;
		}
		*left = ref;
		if (left != start)
			sort(start, left, lessThan);
		if (right != end - 1)
			sort(right + 1, end, lessThan);
	}
	template <class T>
	class Node {
	public:
		T value;
		SharedPointer<Node> prior, next;
		Node() :prior(nullptr), next(nullptr) {};
		Node(const T &value, SharedPointer<Node> next = nullptr, SharedPointer<Node> prior = nullptr) :value(value), prior(prior), next(next) {};
	};
	template <class T>
	class LinkList {
		using NodePointer = SharedPointer<Node<T>>;
		using MatchFunction = Function<bool(const T &, const T &)>;
	protected:
		bool circular;
		int size;
		NodePointer at(int index) const {
			if (index < 0 || index >= size)
				return nullptr;
			NodePointer result = this->head;
			for (; index; result = result->next, --index);
			return result;
		}
	public:
		static const MatchFunction defaultMatch;
		static const MatchFunction defaultLessThan;
		NodePointer head, tail;
		LinkList(bool circular = false) :circular(circular), size(0), head(nullptr), tail(nullptr) {};
		virtual NodePointer prepend(const T &value) = 0;
		virtual NodePointer append(const T &value) = 0;
		virtual NodePointer insert(int index, const T &value) = 0;
		Pair<int, NodePointer> query(const T &value, MatchFunction match = defaultMatch) const {
			int index = 0;
			for (auto i = this->head; index < size; i = i->next, ++index)
				if (match(i->value, value))
					return makePair(index, i);
			return makePair(-1, nullptr);
		}
		virtual bool removeAt(int index) = 0;
		virtual int removeOne(const T &value, MatchFunction match = defaultMatch) = 0;
		virtual int removeAll(const T &value, MatchFunction match = defaultMatch) = 0;
		virtual LinkList &unify() = 0;
		virtual LinkList &reverse() = 0;
		virtual LinkList &sort(MatchFunction lessThan = defaultLessThan) = 0;
		inline int length() const {
			return size;
		}
		inline bool isCircular() const {
			return circular;
		}
		NodePointer circularTail() const {
			if (this->size == 0)
				return nullptr;
			auto i = this->head;
			for (; i->next != this->head; i = i->next);
			return i;
		}
		operator bool() const {
			return size > 0;
		}
		bool operator ! () const {
			return size == 0;
		}
		T &operator [] (int index) {
			auto result = at(index);
			if (result == nullptr)
				throw "Out of range";
			return result->value;
		}
		T operator [] (int index) const {
			auto result = at(index);
			if (result == nullptr)
				throw "Out of range";
			return result->value;
		}
	};
	template <class T>
	const Function<bool(const T &, const T &)> LinkList<T>::defaultMatch = [](const T &a, const T &b) {return a == b; };
	template <class T>
	const Function<bool(const T &, const T &)> LinkList<T>::defaultLessThan = [](const T &a, const T &b) {return a < b; };
	template <class T>
	ostream &operator << (ostream &out, const LinkList<T> &lnk) {
		if (!lnk.head)
			return out;
		int length = lnk.length();
		int index = 0;
		auto i = lnk.head;
		for (; index < length - 1; i = i->next, ++index)
			out << i->value << ' ';
		out << i->value;
		return out;
	}
	template <class T>
	class SingleLinkList : public LinkList<T> {
		using NodePointer = SharedPointer<Node<T>>;
		using MatchFunction = Function<bool(const T &, const T &)>;
	private:
		SingleLinkList &clear() {
			this->head = nullptr;
			this->tail = nullptr;
			this->size = 0;
			return *this;
		}
	public:
		SingleLinkList(bool circular = false) :LinkList<T>(circular) {}
		SingleLinkList(const SingleLinkList &lnk) {
			*this = lnk;
		}
		SingleLinkList(SingleLinkList &&lnk) noexcept {
			*this = std::move(lnk);
		}
		NodePointer prepend(const T &value) {
			auto preHead = this->head;
			auto newHead = makeShared<Node<T>>(value, preHead);
			if (this->circular) {
				if (this->size)
					this->circularTail()->next = newHead;
				else
					newHead->next = newHead;
			}
			else if (!this->size)
				this->tail = newHead;
			this->head = newHead;
			this->size++;
			return this->head;
		}
		NodePointer append(const T &value) {
			NodePointer newTail;
			if (this->circular) {
				if (this->size) {
					newTail = makeShared<Node<T>>(value, this->head);
					this->circularTail()->next = newTail;
				}
				else {
					newTail = makeShared<Node<T>>(value, nullptr);
					this->head = newTail->next = newTail;
				}
			}
			else {
				auto preTail = this->tail;
				newTail = makeShared<Node<T>>(value, nullptr);
				if (!this->size)
					this->head = this->tail = newTail;
				else
					preTail->next = this->tail = newTail;
			}
			this->size++;
			return newTail;
		}
		NodePointer insert(int index, const T &value) {
			if (index<0 || index>this->size)
				return nullptr;
			if (!index)
				return prepend(value);
			if (index == this->size)
				return append(value);
			auto predecessor = this->at(index - 1);
			if (!predecessor)
				return nullptr;
			auto curNode = makeShared<Node<T>>(value, predecessor->next);
			this->size++;
			return predecessor->next = curNode;
		}
		NodePointer insert(const NodePointer &predecessor, const T &value) {
			if (predecessor == this->head)
				return prepend(value);
			if (predecessor == (this->circular ? this->circularTail() : this->tail))
				return append(value);
			if (!predecessor)
				return nullptr;
			auto curNode = makeShared<Node<T>>(value, predecessor->next);
			this->size++;
			return predecessor->next = curNode;
		}
		bool removeAt(int index) {
			if (index < 0 || index >= this->size)
				return false;
			if (this->size == 1) {
				this->head = this->tail = nullptr;
				this->size--;
				return true;
			}
			if (!index) {
				auto preHead = this->head;
				this->head = this->head->next;
				if (this->circular)
					this->circularTail()->next = this->head;
				preHead->next = nullptr;
			}
			else {
				auto predecessor = this->at(index - 1);
				auto cur = predecessor->next;
				predecessor->next = cur->next;
				cur->next = nullptr;
			}
			--this->size;
			return true;
		}
		bool removeAt(const NodePointer &predecessor) {
			if (predecessor == (this->circular ? this->circularTail() : this->tail))
				return false;
			if (predecessor == nullptr) {
				if (!this->head)
					return false;
				auto headNext = this->head->next;
				this->head->next = nullptr;
				this->head = headNext;
				if (this->circular)
					this->circularTail()->next = this->head;
				else if (!headNext)
					this->tail = nullptr;
			}
			else {
				auto cur = predecessor->next;
				predecessor->next = cur->next;
				cur->next = nullptr;
			}
			--this->size;
			return true;
		}
		int removeOne(const T &value, MatchFunction match = LinkList<T>::defaultMatch) {
			int index = 0;
			for (auto i = this->head; index < this->size; i = i->next, ++index)
				if (match(i->value, value)) {
					removeAt(index);
					return index;
				}
			return -1;
		}
		int removeAll(const T &value, MatchFunction match = LinkList<T>::defaultMatch) {
			bool *mark = new bool[this->size];
			memset(mark, false, sizeof(bool) * this->size);
			int index = 0;
			int count = 0;
			for (auto i = this->head; index < this->size; i = i->next, ++index)
				if (match(i->value, value)) {
					mark[index] = true;
					++count;
				}
			int length = this->size, offset = 0;
			index = 0;
			for (auto i = this->head; index < length; ++index) {
				if (mark[index]) {
					auto nxt = i->next;
					removeAt(index - offset++);
					i = nxt;
				}
				else
					i = i->next;
			}
			return count;
		}
		SingleLinkList &unify() {
			if (this->size < 2)
				return *this;
			bool *mark = new bool[this->size];
			memset(mark, false, sizeof(bool) * this->size);
			int index = 0;
			for (auto i = this->head; index < this->size; i = i->next, ++index) {
				int subIndex = 0;
				for (auto j = this->head; subIndex < index; j = j->next, ++subIndex)
					if (i->value == j->value) {
						mark[index] = true;
						break;
					}
			}
			int length = this->size, offset = 0;
			index = 0;
			for (auto i = this->head; index < length; ++index) {
				if (mark[index]) {
					auto nxt = i->next;
					removeAt(index - offset++);
					i = nxt;
				}
				else
					i = i->next;
			}
			return *this;
		}
		SingleLinkList &reverse() {
			if (this->size < 2)
				return *this;
			if (this->size == 2) {
				auto preHead = this->head;
				this->head = this->head->next;
				if (!this->circular) {
					this->head->next = this->tail = preHead;
					preHead->next = nullptr;
				}
			}
			else {
				NodePointer pre, cur, post;
				int index = 0;
				for (pre = this->head, cur = pre->next, post = cur->next; index < this->size - 1 + this->circular; pre = cur, cur = post, post = post->next, ++index) {
					cur->next = pre;
					if (post == nullptr || cur == this->head)
						break;
				}
				if (!this->circular) {
					this->head->next = nullptr;
					this->tail = this->head;
				}
				this->head = this->circular ? pre : cur;
			}
			return *this;
		}
		SingleLinkList &sort(MatchFunction lessThan = LinkList<T>::defaultLessThan) {
			if (this->size < 2)
				return *this;
			T *buffer = new T[this->size];
			int index = 0;
			for (auto i = this->head; index < this->size; i = i->next, ++index)
				buffer[index] = i->value;
			My::sort(buffer, buffer + this->size, lessThan);
			index = 0;
			for (auto i = this->head; index < this->size; i = i->next, ++index)
				i->value = buffer[index];
			return *this;
		}
		SingleLinkList &concat(SingleLinkList &&lnk) {
			if (!*this)
				return *this = std::move(lnk);
			if (!lnk)
				return *this;
			this->size += lnk.length();
			this->circular ? this->circularTail()->next = lnk.head : this->tail->next = lnk.head;
			auto tailNext = this->circular ? this->head : nullptr;
			lnk.isCircular() ? lnk.circularTail()->next = tailNext : lnk.tail->next = tailNext;
			if (!this->circular)
				this->tail = lnk.tail;
			lnk.size = 0;
			lnk.head = lnk.tail = nullptr;
			return *this;
		}
		SingleLinkList &concat(SingleLinkList &lnk) {
			return concat(std::move(lnk));
		}
		SharedPointer<SingleLinkList> split(int index) {
			if (index < 0 || index > this->size)
				return nullptr;
			if (!index)
				return makeShared<SingleLinkList>(std::move(*this));
			if (index == this->size)
				return makeShared<SingleLinkList>(SingleLinkList());
			SharedPointer<SingleLinkList> result = makeShared<SingleLinkList>();
			auto newTail = this->at(index - 1);
			result->head = newTail->next;
			result->circular = this->circular;
			result->size = this->size - index;
			this->size = index;
			if (this->circular) {
				newTail->next = this->head;
				this->tail->next = result->head;
				result->tail = this->tail = nullptr;
			}
			else {
				newTail->next = this->tail->next = nullptr;
				result->tail = this->tail;
				this->tail = newTail;
			}
			return result;
		}
		SingleLinkList &operator = (SingleLinkList &&lnk) noexcept {
			this->circular = lnk.circular;
			this->size = lnk.size;
			this->head = lnk.head;
			this->tail = lnk.tail;
			lnk.size = 0;
			lnk.head = nullptr;
			lnk.tail = nullptr;
			return *this;
		}
		SingleLinkList &operator = (const SingleLinkList &lnk) {
			if (!lnk)
				return clear();
			int length = lnk.length();
			this->circular = lnk.isCircular();
			this->size = length;
			auto cur = this->head = makeShared<Node<T>>(lnk.head->value);
			int index = 0;
			for (auto i = lnk.head; index < length - 1; i = i->next, ++index) {
				cur->next = makeShared<Node<T>>(i->next->value);
				cur = cur->next;
			}
			if (this->circular) {
				cur->next->next = this->head;
				this->tail = nullptr;
			}
			else
				this->tail = cur;
			return *this;
		}
		~SingleLinkList() {
			clear();
		}
	};
	template <class T>
	class DoubleLinkList :LinkList<T> {
		
	};
}