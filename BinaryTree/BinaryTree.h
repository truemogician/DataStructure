#pragma once
#include "..\Queue\Queue.h"
#include "..\List\List.h"
#include "..\CommonLibrary\Enum.h"
namespace My {
	namespace Tree {
		using std::move;
		using LinearList::LinkList;
		using LinearList::Queue;

		template <class BinaryTree, class T>
		class IBinaryTree {
			using BinaryTreeSharedPointer = My::SharedPointer<BinaryTree>;
			using BinaryTreeWeakPointer = My::WeakPointer<BinaryTree>;
		public:
			T value;
			BinaryTreeSharedPointer leftChild, rightChild;
			BinaryTreeWeakPointer parent;
			IBinaryTree() :value(), leftChild(nullptr), rightChild(nullptr), parent() {}
			IBinaryTree(const T &value) :value(value), leftChild(nullptr), rightChild(nullptr), parent() {}
			IBinaryTree(T &&value) :value(move(value)), leftChild(nullptr), rightChild(nullptr), parent() {}
			IBinaryTree(const T &value, const BinaryTreeSharedPointer &leftChild, const BinaryTreeSharedPointer &rightChild, const BinaryTreeWeakPointer &parent) :
				value(value), leftChild(leftChild), rightChild(rightChild), parent(parent) {};
			/*static BinaryTreeSharedPointer create(const LinkList<T> &sequence, const T &recursiveIdentifier, const TraversalOrder &order = TraversalOrder::Preorder) {
				BinaryTree::create(sequence, recursiveIdentifier, order);
			}
			static BinaryTreeSharedPointer create(LinkList<T> &&sequence, const T &recursiveIdentifier, const TraversalOrder &order = TraversalOrder::Preorder) {
				BinaryTree::create(move(sequence), recursiveIdentifier, order);
			}*/
			virtual int getSize() const {
				return (this->leftChild ? this->leftChild->getSize() : 0) +
					(this->rightChild ? this->rightChild->getSize() : 0) + 1;
			}
			virtual int getHeight() const {
				if (!this->leftChild && !this->rightChild)
					return 1;
				int height = 0;
				if (this->leftChild)
					height = std::max(height, this->leftChild->getHeight());
				if (this->rightChild)
					height = std::max(height, this->rightChild->getHeight());
				return height + 1;
			}
			virtual int getWidth() const {
				if (!this->leftChild && !this->rightChild)
					return 1;
				int width = 0;
				if (this->leftChild)
					width += this->leftChild->getWidth();
				if (this->rightChild)
					width += this->rightChild->getWidth();
				return width;
			}
		};
		/*template <template <class, class ...> class TTree, class TValue, class ...TArgs>
		class IBinaryTree {
			using Implemented = TTree<TValue, TArgs...>;
			using ImplementedSharedPointer = My::SharedPointer<Implemented>;
			using ImplementedWeakPointer = My::WeakPointer<Implemented>;
		public:
			TValue value;
			ImplementedSharedPointer leftChild, rightChild;
			ImplementedWeakPointer parent;
			IBinaryTree() :value(), leftChild(nullptr), rightChild(nullptr), parent(nullptr) {}
			IBinaryTree(const TValue &value, const ImplementedSharedPointer &leftChild, const ImplementedSharedPointer &rightChild, const ImplementedWeakPointer &parent) :
				value(value), leftChild(leftChild), rightChild(rightChild), parent(parent) {};
			static ImplementedSharedPointer create(const LinkList<TValue> &sequence, const TValue &recursiveIdentifier, const TraversalOrder &order = TraversalOrder::Preorder) {
				Implemented::create(sequence, recursiveIdentifier, order);
			}
			static ImplementedSharedPointer create(LinkList<TValue> &&sequence, const TValue &recursiveIdentifier, const TraversalOrder &order = TraversalOrder::Preorder) {
				Implemented::create(move(sequence), recursiveIdentifier, order);
			}
			virtual int getSize() const = 0;
			virtual int getHeight() const = 0;
			virtual int getWidth() const = 0;
		};
		template <class ...TParams>
		class ParameterWrapper {
		public:
			template <template <class, TParams> class TTree, class TValue, TParams ...params>
			class IBinaryTree {
				using Implemented = TTree<TValue, params...>;
				using ImplementedSharedPointer = SharedPointer<Implemented>;
				using ImplementedWeakPointer = WeakPointer<Implemented>;
			public:
				TValue value;
				ImplementedSharedPointer leftChild, rightChild;
				ImplementedWeakPointer parent;
				IBinaryTree() :value(), leftChild(nullptr), rightChild(nullptr), parent(nullptr) {}
				IBinaryTree(const TValue &value, const ImplementedSharedPointer &leftChild, const ImplementedSharedPointer &rightChild, const ImplementedWeakPointer &parent) :
					value(value), leftChild(leftChild), rightChild(rightChild), parent(parent) {};
				static ImplementedSharedPointer create(const LinkList<TValue> &sequence, const TValue &recursiveIdentifier, const TraversalOrder &order = TraversalOrder::Preorder) {
					Implemented::create(sequence, recursiveIdentifier, order);
				}
				static ImplementedSharedPointer create(LinkList<TValue> &&sequence, const TValue &recursiveIdentifier, const TraversalOrder &order = TraversalOrder::Preorder) {
					Implemented::create(move(sequence), recursiveIdentifier, order);
				}
				virtual int getSize() const = 0;
				virtual int getHeight() const = 0;
				virtual int getWidth() const = 0;
			};
		};*/
		template <class T>
		class BinaryTree :public IBinaryTree<BinaryTree<T>, T> {
			using Super = IBinaryTree<BinaryTree<T>, T>;
			using Self = BinaryTree<T>;
			using SelfSharedPointer = My::SharedPointer<Self>;
			using SelfSharedConstPointer = My::SharedPointer<const Self>;
			using SelfWeakPointer = My::WeakPointer<Self>;
		public:
			static inline SelfSharedPointer create(const LinkList<T> &sequence, const T &recursiveIdentifier, const TraversalOrder &order = TraversalOrder::Preorder) {
				return create(LinkList<T>(sequence), recursiveIdentifier, order);
			}
			static SelfSharedPointer create(LinkList<T> &&sequence, const T &recursiveIdentifier, const TraversalOrder &order = TraversalOrder::Preorder) {
				if (sequence.getLength() == 1) {
					if (*sequence.begin() == recursiveIdentifier)
						return nullptr;
					else
						throw InvalidArgumentException("Invalid sequence");
				}
				LinkList<T> rightSequence;
				T rootValue;
				switch (order) {
					case TraversalOrder::Preorder: {
						rootValue = sequence[0];
						sequence.remove(0);
						int nodeCount = 0, nullCount = 0;
						auto separator = sequence.begin();
						for (; nullCount < nodeCount + 1 && separator != sequence.end(); ++separator) {
							if (*separator == recursiveIdentifier)
								++nullCount;
							else
								++nodeCount;
						}
						rightSequence = move(*sequence.split(separator)[1]);
						break;
					}
					case TraversalOrder::Inorder:
						break;
					case TraversalOrder::Postorder:
						break;
				}
				auto result = makeShared<Self>(rootValue,
					create(move(sequence), recursiveIdentifier, order),
					create(move(rightSequence), recursiveIdentifier, order));
				if (result->leftChild)
					result->leftChild->parent = result;
				if (result->rightChild)
					result->rightChild->parent = result;
				return move(result);
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

			BinaryTree() :Super() {}
			BinaryTree(const T &value, const SelfSharedPointer &left = nullptr, const SelfSharedPointer &right = nullptr, const SelfWeakPointer &parent = SelfWeakPointer()) :
				Super(value, left, right, parent) {}
			BinaryTree(const SelfSharedPointer &pTree, const SelfSharedPointer &parent = nullptr) :Super(pTree->value, nullptr, nullptr, parent) {
				this->leftChild = pTree->leftChild ? makeShared<Self>(pTree->leftChild, pTree) : nullptr;
				this->rightChild = pTree->rightChild ? makeShared<Self>(pTree->rightChild, pTree) : nullptr;
			}
			BinaryTree(const Self &tree) :Super(tree.value, tree.leftChild, tree.rightChild, tree.parent) {}
			BinaryTree(Self &&tree) :Super(move(tree.value), move(tree.leftChild), move(tree.rightChild), move(tree.parent)) {}

			SelfSharedPointer copy(const SelfSharedPointer &parent) const {
				SelfSharedPointer result = makeShared<Self>(this->value, nullptr, nullptr, parent);
				result->leftChild = this->leftChild ? this->leftChild->copy(result) : nullptr;
				result->rightChild = this->rightChild ? this->rightChild->copy(result) : nullptr;
				return move(result);
			}

			LinkList<T> getTraversalSequence(const TraversalOrder &order = TraversalOrder::Preorder) {
				if (order == TraversalOrder::Levelorder) {
					LinkList<T> list;
					Queue<Self *> queue;
					queue.push(this);
					while (!queue.empty()) {
						auto top = queue.pop();
						list.append(top->value);
						if (top->leftChild)
							queue.push(top->leftChild.get());
						if (top->rightChild)
							queue.push(top->rightChild.get());
					}
					return list;
				}
				LinkList<T> list = this->leftChild ? this->leftChild->getTraversalSequence(order) : LinkList<T>();
				int pos;
				if (order == TraversalOrder::Preorder)
					pos = 0;
				else if (order == TraversalOrder::Inorder)
					pos = list.getLength();
				list.concat(this->rightChild ? this->rightChild->getTraversalSequence(order) : LinkList<T>());
				if (order == TraversalOrder::Postorder)
					pos = list.getLength();
				list.insert(pos, this->value);
				return move(list);
			}

		};
	}
}