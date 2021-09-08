#pragma once
#include "../List/List.h"
#include "../Queue/Queue.h"
#include "../CommonLibrary/Enum.h"
namespace My {
	namespace Tree {
		using std::move;
		using My::LinearList::LinkList;
		using My::LinearList::Queue;
		template <class MultipleTree, class TValue>
		class IMultipleTree {
		public:
			TValue value;
			LinkList<SharedPointer<MultipleTree>> children;
			WeakPointer<MultipleTree> parent;
			IMultipleTree() :value(), children(), parent() {}
			IMultipleTree(const TValue &value, const WeakPointer<MultipleTree> &parent) :
				value(value), children(), parent(parent) {};
			IMultipleTree(const TValue &value, const LinkList<SharedPointer<MultipleTree>> &children, const WeakPointer<MultipleTree> &parent) :
				value(value), children(children), parent(parent) {}
			IMultipleTree(TValue &&value, LinkList<SharedPointer<MultipleTree>> &&children, WeakPointer<MultipleTree> &&parent) :
				value(move(value)), children(move(children)), parent(move(parent)) {}
			static SharedPointer<MultipleTree> create(const LinkList<TValue> &sequence, const TValue &recursiveIdentifier, const TraversalOrder &order = TraversalOrder::Preorder) {
				MultipleTree::create(sequence, recursiveIdentifier, order);
			}
			static SharedPointer<MultipleTree> create(LinkList<TValue> &&sequence, const TValue &recursiveIdentifier, const TraversalOrder &order = TraversalOrder::Preorder) {
				MultipleTree::create(move(sequence), recursiveIdentifier, order);
			}
			virtual int getSize() const = 0;
			virtual int getHeight() const = 0;
			virtual int getWidth() const = 0;
		};
		template <class T>
		class MultipleTree :public IMultipleTree<MultipleTree<T>, T> {
			using Super = IMultipleTree<MultipleTree<T>, T>;
			using Self = MultipleTree<T>;
			using SelfSharedPointer = My::SharedPointer<Self>;
			using SelfSharedConstPointer = My::SharedPointer<const Self>;
			using SelfWeakPointer = My::WeakPointer<Self>;
		public:
			static inline SelfSharedPointer create(const LinkList<T> &sequence, const T &recursiveIdentifier, const TraversalOrder &order = TraversalOrder::Preorder) {
				return create(LinkList<T>(sequence), recursiveIdentifier, order);
			}
			static SelfSharedPointer create(LinkList<T> &&sequence, const T &recursiveIdentifier, const TraversalOrder &order = TraversalOrder::Preorder) {
				switch (order) {
					case TraversalOrder::Preorder: {
						SelfSharedPointer result = makeShared<Self>(move(sequence[0]));
						SelfSharedPointer cur = result;
						sequence.remove(0);
						T curValue;
						while (!sequence.isEmpty()) {
							curValue = move(sequence[0]);
							sequence.remove(0);
							if (curValue == recursiveIdentifier)
								cur = cur->parent.lock();
							else {
								cur->children.append(makeShared<Self>(move(curValue)));
								cur->children.last()->parent = cur;
								cur = cur->children.last();
							}
						}
						while (!cur->parent.expired())
							cur = cur->parent.lock();
						return move(result);
					}
					case TraversalOrder::Postorder:
						break;
					case TraversalOrder::Levelorder:
						break;
				}
			}
			static SelfSharedPointer getLatestCommonAncestor(const SelfSharedPointer &pTreeA, const SelfSharedPointer &pTreeB) {
				LinkList<SelfSharedPointer>seqA, seqB;
				for (auto i = pTreeA; i; i = i->parent.lock())
					seqA.append(i);
				for (auto i = pTreeB; i; i = i->parent.lock())
					seqB.append(i);
				SelfSharedPointer result = nullptr;
				for (auto i = seqA.begin(true), j = seqB.begin(true); i != seqA.end(true) && j != seqB.end(true); --i, --j) {
					if (*i == *j)
						result = *i;
					else
						break;
				}
				return move(result);
			}
			static SelfSharedConstPointer getLatestCommonAncestor(const SelfSharedConstPointer &pTreeA, const SelfSharedConstPointer &pTreeB) {
				LinkList<SelfSharedConstPointer>seqA(nullptr), seqB(nullptr);
				for (auto i = pTreeA; i; i = i->parent.lock())
					seqA.append(i);
				for (auto i = pTreeB; i; i = i->parent.lock())
					seqB.append(i);
				SelfSharedConstPointer result = nullptr;
				for (auto i = seqA.begin(true), j = seqB.begin(true); i != seqA.end(true) && j != seqB.begin(true); --i, --j) {
					if (*i == *j)
						result = *i;
					else
						break;
				}
				return move(result);
			}

			MultipleTree() :Super() {}
			MultipleTree(const T &value, const SelfWeakPointer &parent = SelfWeakPointer()) :
				Super(value, parent) {}
			MultipleTree(const SelfSharedPointer &pTree, const SelfSharedPointer &parent = nullptr) :Super(pTree->value, parent) {
				for (auto i = pTree->children.begin(); i != pTree->children.end(); ++i) 
					this->children.append(makeShared<Self>(*i, pTree));
			}
			MultipleTree(const Self &tree) :Super(tree.value, tree.children, tree.parent) {}
			MultipleTree(Self &&tree) :Super(move(tree.value), move(tree.children), move(tree.parent)) {}

			int getSize() const override {
				int size = 1;
				for (auto i = this->children.begin(); i != this->children.end(); ++i)
					size += (*i)->getSize();
				return size;
			}
			int getHeight() const override {
				if (this->children.getLength()==0)
					return 1;
				int height = 0;
				for (auto i = this->children.begin(); i != this->children.end(); ++i)
					height = std::max(height, (*i)->getHeight());
				return height + 1;
			}
			int getWidth() const override {
				if (this->children.getLength() == 0)
					return 1;
				int width = 0;
				for (auto i = this->children.begin(); i != this->children.end(); ++i)
					width += (*i)->getWidth();
				return width;
			}

			LinkList<T> getTraversalSequence(const TraversalOrder &order = TraversalOrder::Preorder) {
				if (order == TraversalOrder::Levelorder) {
					LinkList<T> list;
					Queue<Self *> queue;
					queue.push(this);
					while (!queue.empty()) {
						auto top = queue.pop();
						list.append(top->value);
						for (auto i = top->children.begin(); i != top->children.end(); ++i)
							queue.push(i->get());
					}
					return list;
				}
				LinkList<T> list;
				for (auto i = this->children.begin(); i != this->children.end(); ++i)
					list.concat((*i)->getTraversalSequence(order));
				if (order == TraversalOrder::Preorder)
					list.insert(0, this->value);
				else if (order == TraversalOrder::Postorder)
					list.append(this->value);
				return move(list);
			}

		};
	}
}