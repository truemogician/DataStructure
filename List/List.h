#pragma once
#include "../Alias/SmartPointerAlias.h"
#include "../Alias/FunctionAlias.h"
#include "../Alias/StreamAlias.h"
#include "../Alias/ExceptionAlias.h"
#include "../Alias/StringAlias.h"
#include "../CommonLibrary/Interface.h"
namespace My {
	namespace LinearList {
		using std::move;
		template <class T>
		class List {
		public:
			virtual int getLength() const = 0;
			virtual bool isEmpty() const {
				return getLength() == 0;
			}
			virtual List &append(const T &value) = 0;
			virtual List &append(T &&value) = 0;
			template <class ...Ts>
			List &append(const T &value, const Ts &...args) {
				append(value);
				append(args...);
				return *this;
			}
			template <class ...Ts>
			List &append(T &&value, Ts &&...args) {
				append(move(value));
				append(move(args)...);
				return *this;
			}
			virtual List &insert(const int &index, const T &value) = 0;
			virtual List &insert(const int &index, T &&value) = 0;
			virtual List &remove(const int &index) = 0;
			virtual List &fill(const T &value) = 0;
			virtual List &sort() = 0;
			virtual List &sort(const Function<bool(const T &, const T &)> &comparer) = 0;
			virtual const T &operator [] (const int &index) const = 0;
			virtual T &operator [] (const int &index) = 0;
		};
		template <class T>
		class LinkList : public List<T>, public IMultiContainerOperation<LinkList, T>, public IContainerConversion<LinkList, T> {

		public:
			class Node {
			public:
				T value;
				WeakPointer<Node> prior;
				SharedPointer<Node> next;
				Node() :value(), prior(), next(nullptr) {}
				Node(const T &value, const SharedPointer<Node> &prior = nullptr, const SharedPointer<Node> &next = nullptr) :
					value(value), prior(prior), next(next) {}
				Node(T &&value, const SharedPointer<Node> &prior = nullptr, const SharedPointer<Node> &next = nullptr) :
					value(move(value)), prior(prior), next(next) {}
				Node(const Node &node) :value(node.value), prior(node.prior), next(node.next) {}
				Node(Node &&node) :value(move(node.value)), prior(move(node.prior)), next(move(node.next)) {}
				Node &operator = (const Node &node) {
					value = node.value;
					prior = node.prior;
					next = node.next;
					return *this;
				}
				Node &operator = (Node &&node) noexcept {
					value = move(node.value);
					prior = move(node.prior);
					next = move(node.next);
					return *this;
				}
			};
			void swapConnection(const SharedPointer<Node> &nodeA, const SharedPointer<Node> &nodeB) {
				if (nodeA == nodeB)
					return;
				bool aPriorToB = nodeA == nodeB->prior.lock();
				bool aNextToB = nodeA == nodeB->next;
				SharedPointer<Node> priorA = nodeA->prior.lock(), nextA = move(nodeA->next);
				nodeA->prior = aPriorToB ? nodeB : move(nodeB->prior);
				nodeA->next = aNextToB ? nodeB : move(nodeB->next);
				nodeB->prior = aNextToB ? nodeA : move(priorA);
				nodeB->next = aPriorToB ? nodeA : move(nextA);
				if (!aPriorToB && !nodeA->prior.expired())
					nodeA->prior.lock()->next = nodeA;
				if (!aNextToB && nodeA->next)
					nodeA->next->prior = nodeA;
				if (!aNextToB && !nodeB->prior.expired())
					nodeB->prior.lock()->next = nodeB;
				if (!aPriorToB && nodeB->next)
					nodeB->next->prior = nodeB;
			}
			void swapValue(SharedPointer<Node> &nodeA, SharedPointer<Node> &nodeB) {
				SharedPointer<Node> tmp = move(nodeB);
				nodeB = move(nodeA);
				nodeA = move(tmp);
			}
			inline void swap(SharedPointer<Node> &nodeA, SharedPointer<Node> &nodeB) {
				swapConnection(nodeA, nodeB);
				swapValue(nodeA, nodeB);
			}
			class ConstIterator : public IContainerIterator<ConstIterator, T, false> {
			private:
				SharedPointer<const Node> pNode;
			public:
				ConstIterator(const Node &node) :pNode(makeShared<Node>(node)) {}
				ConstIterator(Node &&node) :pNode(makeShared<Node>(move(node))) {}
				ConstIterator(const SharedPointer<const Node> &pNode) :pNode(pNode) {}
				ConstIterator(SharedPointer<const Node> &&pNode) :pNode(move(pNode)) {}
				ConstIterator(const ConstIterator &it) :pNode(it.pNode) {}
				ConstIterator(ConstIterator &&it) noexcept :pNode(move(it.pNode)) {}
				T const *operator -> () const override {
					return &pNode->value;
				}
				const T &operator * () const override {
					return pNode->value;
				}
				ConstIterator &operator ++ () override {
					pNode = pNode->next;
					return *this;
				}
				ConstIterator operator ++ (int) override {
					ConstIterator result(*this);
					pNode = pNode->next;
					return result;
				}
				ConstIterator &operator -- () override {
					pNode = pNode->prior.lock();
					return *this;
				}
				ConstIterator operator -- (int) override {
					ConstIterator result(*this);
					pNode = pNode->prior.lock();
					return result;
				}
				bool operator == (const ConstIterator &it) const override {
					return pNode == it.pNode;
				}
				bool operator != (const ConstIterator &it) const override {
					return pNode != it.pNode;
				}
				operator SharedPointer<const Node>() {
					return pNode;
				}
			};
			class Iterator : public IContainerIterator<Iterator, T> {
			private:
				SharedPointer<Node> pNode;
			public:
				Iterator(const Node &node) :pNode(makeShared<Node>(node)) {}
				Iterator(Node &&node) :pNode(makeShared<Node>(move(node))) {}
				Iterator(const SharedPointer<Node> &pNode) :pNode(pNode) {}
				Iterator(SharedPointer<Node> &&pNode) :pNode(move(pNode)) {}
				Iterator(const Iterator &it) :pNode(it.pNode) {}
				Iterator(Iterator &&it) noexcept :pNode(move(it.pNode)) {}
				T *operator -> () const override {
					return &pNode->value;
				}
				T &operator * () const override {
					return pNode->value;
				}
				Iterator &operator ++ () override {
					pNode = pNode->next;
					return *this;
				}
				Iterator operator ++ (int) override {
					Iterator result(*this);
					pNode = pNode->next;
					return result;
				}
				Iterator &operator -- () override {
					pNode = pNode->prior.lock();
					return *this;
				}
				Iterator operator -- (int) override {
					Iterator result(*this);
					pNode = pNode->prior.lock();
					return result;
				}
				bool operator == (const Iterator &it) const override {
					return pNode == it.pNode;
				}
				bool operator != (const Iterator &it) const override {
					return pNode != it.pNode;
				}
				operator ConstIterator() {
					return ConstIterator(constPointerCast<const Node>(pNode));
				}
				operator SharedPointer<Node>() {
					return pNode;
				}
				operator Node *() const {
					return pNode.get();
				}
			};

		private:
			inline LinkList **split(const int &index, const SharedPointer<Node> &pNode) {
				LinkList *another = new LinkList;
				another->length = length - index;
				length = index;
				SharedPointer<Node> newTail = pNode->prior.lock();
				another->headNode->next = pNode;
				another->tailNode->prior = tailNode->prior;
				tailNode->prior = newTail;
				newTail->next = tailNode;
				pNode->prior = another->headNode;
				return new LinkList * [2]{ this,another };
			}
		protected:
			int length;
			SharedPointer<Node> headNode, tailNode;
			SharedPointer<Node> at(const int &index) const {
				if (index < 0 || index >= length)
					throw OutOfRangeException("Index equals " + toString(index) + " , but should be within [0," + toString(length) + "]");
				int count = 0;
				if (index < (length >> 1)) {
					for (SharedPointer<Node> i = headNode->next; i != tailNode; i = i->next)
						if (count++ == index)
							return i;
				}
				else {
					for (SharedPointer<Node> i = tailNode->prior.lock(); i != headNode; i = i->prior.lock())
						if (count++ == length - index - 1)
							return i;
				}
				return nullptr;
			}
			int pos(const SharedPointer<Node> &pNode) const {
				int index = 0;
				for (SharedPointer<Node> i = headNode->next; i != tailNode; i = i->next, ++index)
					if (i == pNode)
						return index;
				throw InvalidArgumentException("Node not found");
			}
			//open interval
			void sort(const SharedPointer<Node> &begin, const SharedPointer<Node> &end, const Function<bool(const T &, const T &)> &comparer) {
				if (begin->next == end->prior.lock() || begin->next == end || begin == end)
					return;
				SharedPointer<Node> left = begin->next, right = end->prior.lock();
				SharedPointer<Node> benchmark = makeShared<Node>();
				swap(benchmark, left);
				while (left != right) {
					for (; left != right && !comparer(right->value, benchmark->value); right = right->prior.lock());
					if (left == right)
						break;
					swap(left, right);
					left = left->next;
					if (left == right)
						break;
					for (; left != right && comparer(left->value, benchmark->value); left = left->next);
					if (left == right)
						break;
					swap(left, right);
					right = right->prior.lock();
				}
				swap(left, benchmark);
				right = left;
				if (left != begin->next)
					sort(begin, left, comparer);
				if (right != end->prior.lock())
					sort(right, end, comparer);
			}
		public:
			LinkList() :length(0), headNode(makeShared<Node>()), tailNode(makeShared<Node>()) {
				headNode->next = tailNode;
				tailNode->prior = headNode;
			}
			template <class ...Ts>
			LinkList(const T &value, const Ts & ...values) : LinkList() {
				append(value);
				if (sizeof...(Ts) > 0)
					List<T>::append(values...);
			}
			template <class ...Ts>
			LinkList(T &&value, Ts && ...values) : LinkList() {
				append(move(value));
				if (sizeof...(Ts) > 0)
					List<T>::append(move(values)...);
			}
			LinkList(const T *const &array, const int &length) :LinkList() {
				for (int i = 0; i < length; i++)
					append(array[i]);
			}
			LinkList(T *&&array, const int &length) :LinkList() {
				for (int i = 0; i < length; i++)
					append(move(array[i]));
			}
			LinkList(const LinkList &list) :length(list.length), headNode(makeShared<Node>(list.headNode->value)), tailNode(makeShared<Node>(list.tailNode->value)) {
				SharedPointer<Node> cur = headNode;
				for (SharedPointer<Node> i = list.headNode->next; i != list.tailNode; i = i->next) {
					cur->next = makeShared<Node>(i->value, cur);
					cur = cur->next;
				}
				cur->next = tailNode;
				tailNode->prior = cur;
			}
			LinkList(LinkList &&list) noexcept :length(list.length), headNode(move(list.headNode)), tailNode(move(list.tailNode)) {
				list.length = 0;
			}
			LinkList &operator = (const LinkList &list) {
				length = list.length;
				headNode = makeShared<Node>(list.headNode->value);
				tailNode = makeShared<Node>(list.tailNode->value);
				SharedPointer<Node> cur = headNode;
				for (SharedPointer<Node> i = list.headNode->next; i != list.tailNode; i = i->next) {
					cur->next = makeShared<Node>(i->value, cur);
					cur = cur->next;
				}
				cur->next = tailNode;
				tailNode->prior = cur;
				return *this;
			}
			LinkList &operator = (LinkList &&list) noexcept {
				length = list.length;
				list.length = 0;
				headNode = move(list.headNode);
				tailNode = move(list.tailNode);
				return *this;
			}
			inline Iterator begin(const bool &reverse = false) {
				return reverse ? Iterator(tailNode->prior.lock()) : Iterator(headNode->next);
			}
			inline ConstIterator begin(const bool &reverse = false) const {
				return reverse ? ConstIterator(tailNode->prior.lock()) : ConstIterator(headNode->next);
			}
			inline Iterator end(const bool &reverse = false) {
				return reverse ? Iterator(headNode) : Iterator(tailNode);
			}
			inline ConstIterator end(const bool &reverse = false) const {
				return reverse ? ConstIterator(headNode) : ConstIterator(tailNode);
			}
			inline int getLength() const override {
				return length;
			}
			inline T &first() {
				return headNode->next->value;
			}
			inline const T &first() const {
				return headNode->next->value;
			}
			inline T &last() {
				return tailNode->prior.lock()->value;
			}
			inline const T &last() const{
				return tailNode->prior.lock()->value;
			}
			LinkList &insert(const SharedPointer<Node> &pre, const T &value) {
				if (pre == nullptr)
					throw InvalidArgumentException("Predecessor node must not be null");
				++length;
				SharedPointer<Node> post = pre->next;
				pre->next = makeShared<Node>(value, pre, post);
				post->prior = pre->next;
				return *this;
			}
			LinkList &insert(const SharedPointer<Node> &pre, T &&value) {
				if (pre == nullptr)
					throw InvalidArgumentException("Predecessor node must not be null");
				++length;
				SharedPointer<Node> post = pre->next;
				pre->next = makeShared<Node>(move(value), pre, post);
				post->prior = pre->next;
				return *this;
			}
			inline LinkList &append(const T &value) override {
				return insert(tailNode->prior.lock(), value);
			}
			inline LinkList &append(T &&value) override {
				return insert(tailNode->prior.lock(), move(value));
			}
			inline LinkList &insert(const int &index, const T &value) override {
				return index == length ? append(value) : insert(at(index)->prior.lock(), value);
			}
			inline LinkList &insert(const int &index, T &&value) override {
				return index == length ? append(move(value)) : insert(at(index)->prior.lock(), move(value));
			}
			LinkList &remove(const SharedPointer<Node> &target) {
				--length;
				target->prior.lock()->next = target->next;
				target->next->prior = target->prior;
				target->prior = target->next = nullptr;
				return *this;
			}
			inline LinkList &remove(const int &index) override {
				return remove(at(index));
			}
			LinkList &fill(const T &value) override {
				for (auto i = headNode->next; i != tailNode; i = i->next)
					i->value = value;
				return *this;
			}
			inline LinkList &sort() override {
				sort(headNode, tailNode, [](const T &a, const T &b) {return a < b; });
				return *this;
			}
			inline LinkList &sort(const Function<bool(const T &, const T &)> &comparer) {
				sort(headNode, tailNode, comparer);
				return *this;
			}
			//left close right open
			inline LinkList &sort(const int &startIndex, const int &endIndex = length,
				const Function<bool(const T &, const T &)> &comparer = [](const T &a, const T &b) {return a < b; }) {
				if (startIndex<0 || endIndex>length) {
					char message[1 << 6];
					sprintf_s(message, "Range should be within [0,%d), but request for [%d,%d)", length, startIndex, endIndex);
					throw OutOfRangeException(message);
				}
				sort(at(startIndex)->prior.lock(), at(endIndex - 1)->next, comparer);
				return *this;
			}
			LinkList &concat(LinkList &&list) override {
				length += list.length;
				SharedPointer<Node> oldTail = tailNode->prior.lock();
				SharedPointer<Node> oldHead = list.headNode->next;
				oldTail->next = oldHead;
				oldHead->prior = oldTail;
				tailNode = move(list.tailNode);
				list.length = 0;
				list.headNode = nullptr;
				return *this;
			}
			inline LinkList **split(const SharedPointer<Node> &pNode) {
				return split(pos(pNode), pNode);
			}
			inline LinkList **split(const int &index) override {
				return split(index, at(index));
			}
			T *toArray() const override {
				if (length == 0)
					return nullptr;
				T *array = new T[length];
				int count = 0;
				for (auto i = headNode->next; i != tailNode; i = i->next)
					array[count++] = i->value;
				return array;
			}
			inline const T &operator [] (const int &index) const override {
				return at(index)->value;
			}
			inline T &operator [] (const int &index) override {
				return at(index)->value;
			}
			bool operator == (const LinkList &list) const {
				if (length != list.length)
					return false;
				for (auto i = headNode->next, j = list.headNode->next; i != tailNode && j != list.tailNode; i = i->next, j = j->next)
					if (i->value != j->value)
						return false;
				return true;
			}
		};
		template <class T>
		OutStream &operator << (OutStream &out, const LinkList<T> &list) {
			for (auto i = list.begin(); i != list.end(); ++i) {
				if (i == list.begin())
					out << *i;
				else
					out << ' ' << *i;
			}
			return out;
		}
		template <>
		OutStream &operator << (OutStream &out, const LinkList<char> &list) {
			for (auto i = list.begin(); i != list.end(); ++i)
				out << *i;
			return out;
		}
	}
}