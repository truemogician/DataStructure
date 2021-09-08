#pragma once
#include "BinaryTree.h"
namespace My {
	namespace Tree {
		template <class T, TraversalOrder TOrder>
		class ThreadedBinaryTree {
		public:
			class Iterator;
			class ConstIterator;
		};
		template <class T, TraversalOrder TOrder>
		class ThreadedBinaryTreeBasis :
			public IBinaryTree<ThreadedBinaryTree<T, TOrder>, T> {
			using Super = IBinaryTree<ThreadedBinaryTree<T, TOrder>, T>;
			using Actual = ThreadedBinaryTree<T, TOrder>;
			using ActualSharedPointer = SharedPointer<Actual>;
			using ActualWeakPointer = WeakPointer<Actual>;
			friend class ThreadedBinaryTree<T, TOrder>;
		private:
			unsigned char threadStatus = 0;
		protected:
			inline void setLeftToThread() {
				threadStatus |= 1 << 4;
			}
			inline void setLeftToChild() {
				threadStatus &= 15;
			}
			inline void setRightToThread() {
				threadStatus |= 1;
			}
			inline void setRightToChild() {
				threadStatus &= 15 << 4;
			}
			static ActualSharedPointer createThroughTraversalSequence(LinkList<T> &&sequence, const T &recursiveIdentifier, const TraversalOrder &order = TraversalOrder::Preorder) {
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
				auto result = makeShared<Actual>(rootValue,
					createThroughTraversalSequence(move(sequence), recursiveIdentifier, order),
					createThroughTraversalSequence(move(rightSequence), recursiveIdentifier, order));
				if (result->leftChild)
					result->leftChild->parent = result;
				if (result->rightChild)
					result->rightChild->parent = result;
				return move(result);
			}
			virtual void threadify(ActualSharedPointer &prior) = 0;
		public:
			ThreadedBinaryTreeBasis(const T &value, const ActualSharedPointer &left = nullptr, const ActualSharedPointer &right = nullptr, const ActualWeakPointer &parent = ActualWeakPointer()) :
				Super(value, left, right, parent) {}
			ThreadedBinaryTreeBasis(const ActualSharedPointer &pTree, const ActualSharedPointer &parent = nullptr) :Super(pTree->value, nullptr, nullptr, parent) {
				this->leftChild = pTree->leftChild ? makeShared<Actual>(pTree->leftChild, pTree) : nullptr;
				this->rightChild = pTree->rightChild ? makeShared<Actual>(pTree->rightChild, pTree) : nullptr;
			}
			ThreadedBinaryTreeBasis(const Actual &tree) :Super(tree.value, tree.leftChild, tree.rightChild, tree.parent) {}
			ThreadedBinaryTreeBasis(Actual &&tree) :Super(move(tree.value), move(tree.leftChild), move(tree.rightChild), move(tree.parent)) {}
			inline bool isLeftAThread() const {
				return threadStatus & (1 << 4);
			}
			inline bool isRightAThread() const {
				return threadStatus & 1;
			}
			inline bool hasLeftChild() const {
				return !isLeftAThread() && this->leftChild;
			}
			inline bool hasRightChild() const {
				return !isRightAThread() && this->rightChild;
			}
			static inline ActualSharedPointer create(const LinkList<T> &sequence, const T &recursiveIdentifier, const TraversalOrder &order = TraversalOrder::Preorder) {
				return create(LinkList<T>(sequence), recursiveIdentifier, order);
			}
			static inline ActualSharedPointer create(LinkList<T> &&sequence, const T &recursiveIdentifier, const TraversalOrder &order = TraversalOrder::Preorder) {
				auto result = createThroughTraversalSequence(move(sequence), recursiveIdentifier, order);
				result->threadify();
				return move(result);
			}
			int getSize() const override {
				return (this->leftChild ? this->leftChild->getSize() : 0) +
					(this->rightChild ? this->rightChild->getSize() : 0) + 1;
			}
			int getHeight() const override {
				if (!this->leftChild && !this->rightChild)
					return 1;
				int height = 0;
				if (this->leftChild)
					height = std::max(height, this->leftChild->getHeight());
				if (this->rightChild)
					height = std::max(height, this->rightChild->getHeight());
				return height + 1;
			}
			int getWidth() const override {
				if (!this->leftChild && !this->rightChild)
					return 1;
				int width = 0;
				if (this->leftChild)
					width += this->leftChild->getWidth();
				if (this->rightChild)
					width += this->rightChild->getWidth();
				return width;
			}
			class IteratorBasis : public IContainerIterator <typename Actual::Iterator, T> {
			protected:
				ActualSharedPointer ptr;
			public:
				IteratorBasis(const ActualSharedPointer &ptr) :ptr(ptr) {}
				IteratorBasis(ActualSharedPointer &&ptr) :ptr(move(ptr)) {}
				T *operator -> () const override {
					return &ptr->value;
				};
				T &operator * () const override {
					return ptr->value;
				};
				bool operator == (const typename Actual::Iterator &it) const override {
					return ptr == it.ptr;
				}
				bool operator != (const typename Actual::Iterator &it) const override {
					return ptr != it.ptr;
				}
				operator ActualSharedPointer() {
					return ptr;
				}
			};
			class ConstIteratorBasis :IContainerIterator<typename Actual::ConstIterator, T, false> {
			protected:
				SharedPointer<const Actual> ptr;
			public:
				ConstIteratorBasis(const SharedPointer<const Actual> &ptr) :ptr(ptr) {}
				ConstIteratorBasis(SharedPointer<const Actual> &&ptr) :ptr(move(ptr)) {}
				T *operator -> () const override {
					return &ptr->value;
				};
				T &operator * () const override {
					return ptr->value;
				};
				bool operator == (const typename Actual::ConstIterator &it) const override {
					return ptr == it.ptr;
				}
				bool operator != (const typename Actual::ConstIterator &it) const override {
					return ptr != it.ptr;
				}
				operator SharedPointer<const Actual>() {
					return ptr;
				}
			};
		};
		template <class T>
		class ThreadedBinaryTree<T, TraversalOrder::Preorder> :
			public ThreadedBinaryTreeBasis<T, TraversalOrder::Preorder>,
			public EnableSharedPointerFromThis<ThreadedBinaryTree<T, TraversalOrder::Preorder>> {
			using Super = ThreadedBinaryTreeBasis<T, TraversalOrder::Preorder>;
			using Self = ThreadedBinaryTree<T, TraversalOrder::Preorder>;
			using SelfSharedPointer = SharedPointer<Self>;
			using SelfWeakPointer = WeakPointer<Self>;
		private:
			void threadify(SelfSharedPointer &prior) override {
				if (!this->isLeftAThread() && !this->leftChild) {
					this->setLeftToThread();
					this->leftChild = prior;
				}
				if (prior && !this->isRightAThread() && !prior->rightChild) {
					prior->setRightToThread();
					prior->rightChild = this->shared_from_this();
				}
				prior = this->shared_from_this();
				if (this->hasLeftChild())
					this->leftChild->threadify(prior);
				if (this->hasRightChild())
					this->rightChild->threadify(prior);
			}
		public:
			ThreadedBinaryTree(const T &value, const SelfSharedPointer &left = nullptr, const SelfSharedPointer &right = nullptr, const SelfWeakPointer &parent = SelfWeakPointer()) :
				Super(value, left, right, parent) {}
			ThreadedBinaryTree(const SelfSharedPointer &pTree, const SelfSharedPointer &parent = nullptr) :
				Super(pTree, parent) {}
			ThreadedBinaryTree(const Self &tree) :
				Super(tree) {}
			ThreadedBinaryTree(Self &&tree) :
				Super(move(tree)) {}
			inline void threadify() {
				SelfSharedPointer prior;
				threadify(prior);
				prior->setRightToThread();
			}
			class Iterator :public Super::IteratorBasis {
			public:
				Iterator(const SelfSharedPointer &ptr) :Super::IteratorBasis(ptr) {}
				Iterator(SelfSharedPointer &&ptr) :Super::IteratorBasis(move(ptr)) {}
				Iterator(const Iterator &it) :Super::IteratorBasis(it.ptr) {}
				Iterator(Iterator &&it) noexcept :Super::IteratorBasis(move(it.ptr)) {}
				Iterator &operator ++ () override {
					if (this->ptr->hasLeftChild())
						this->ptr = this->ptr->leftChild;
					else
						this->ptr = this->ptr->rightChild;
					return *this;
				}
				Iterator operator ++ (int) override {
					Iterator result(*this);
					++(*this);
					return move(result);
				}
				Iterator &operator -- () override {
					if (this->ptr->isLeftAThread()) {
						this->ptr = this->ptr->leftChild;
						return *this;
					}
					else if (this->ptr->parent.expired()) {
						this->ptr = nullptr;
						return *this;
					}
					else {
						auto parent = this->ptr->parent.lock();
						bool isRightChild = parent->hasRightChild() && this->ptr == parent->rightChild;
						this->ptr = move(parent);
						if (!isRightChild || this->ptr->isLeftAThread())
							return *this;
						else {
							this->ptr = this->ptr->leftChild;
							while (true) {
								if (this->ptr->hasRightChild())
									this->ptr = this->ptr->rightChild;
								else if (this->ptr->hasLeftChild())
									this->ptr = this->ptr->leftChild;
								else
									break;
							}
						}
					}
					return *this;
				}
				Iterator operator -- (int) override {
					Iterator result(*this);
					--(*this);
					return move(result);
				}
			};
			class ConstIterator :public Super::ConstIteratorBasis {
				ConstIterator(const SharedPointer<const Self> &ptr) :Super::ConstIteratorBasis(ptr) {}
				ConstIterator(SharedPointer<const Self> &&ptr) :Super::ConstIteratorBasis(move(ptr)) {}
				ConstIterator(const ConstIterator &it) :Super::ConstIteratorBasis(it.ptr) {}
				ConstIterator(ConstIterator &&it) :Super::ConstIteratorBasis(move(it.ptr)) {}
				ConstIterator &operator ++ () override {
					if (this->ptr->hasLeftChild())
						this->ptr = constPointerCast<const Self>(this->ptr->leftChild);
					else
						this->ptr = constPointerCast<const Self>(this->ptr->rightChild);
					return *this;
				}
				ConstIterator operator ++ (int) override {
					ConstIterator result(*this);
					++(*this);
					return move(result);
				}
				ConstIterator &operator -- () override {
					if (this->ptr->isLeftAThread()) {
						this->ptr = constPointerCast<const Self>(this->ptr->leftChild);
						return *this;
					}
					else if (this->ptr->parent.expired()) {
						this->ptr = nullptr;
						return *this;
					}
					else {
						auto parent = constPointerCast<const Self>(this->ptr->parent.lock());
						bool isRightChild = parent->hasRightChild() && this->ptr == constPointerCast<const Self>(parent->rightChild);
						this->ptr = move(parent);
						if (!isRightChild || this->ptr->isLeftAThread())
							return *this;
						else {
							this->ptr = constPointerCast<const Self>(this->ptr->leftChild);
							while (true) {
								if (this->ptr->hasRightChild())
									this->ptr = constPointerCast<const Self>(this->ptr->rightChild);
								else if (this->ptr->hasLeftChild())
									this->ptr = constPointerCast<const Self>(this->ptr->leftChild);
								else
									break;
							}
						}
					}
					return *this;
				}
				ConstIterator operator -- (int) override {
					ConstIterator result(*this);
					--(*this);
					return move(result);
				}
			};
			inline Iterator begin() {
				return Iterator(this->shared_from_this());
			}
			inline ConstIterator begin() const {
				return ConstIterator(constPointerCast<const Self>(this->shared_from_this()));
			}
			inline Iterator end() {
				return Iterator(nullptr);
			}
			inline ConstIterator end() const {
				return ConstIterator(nullptr);
			}
		};
		template <class T>
		class ThreadedBinaryTree<T, TraversalOrder::Inorder> :
			public ThreadedBinaryTreeBasis<T, TraversalOrder::Inorder>,
			public EnableSharedPointerFromThis<ThreadedBinaryTree<T, TraversalOrder::Inorder>> {
			using Super = ThreadedBinaryTreeBasis<T, TraversalOrder::Inorder>;
			using Self = ThreadedBinaryTree<T, TraversalOrder::Inorder>;
			using SelfSharedPointer = SharedPointer<Self>;
			using SelfWeakPointer = WeakPointer<Self>;
		private:
			void threadify(SelfSharedPointer &prior) override {
				if (this->hasLeftChild())
					this->leftChild->threadify(prior);
				if (!this->isLeftAThread() && !this->leftChild) {
					this->setLeftToThread();
					this->leftChild = prior;
				}
				if (prior && !prior->isRightAThread() && !prior->rightChild) {
					prior->setRightToThread();
					prior->rightChild = this->shared_from_this();
				}
				prior = this->shared_from_this();
				if (this->hasRightChild())
					this->rightChild->threadify(prior);
			}
		public:
			ThreadedBinaryTree(const T &value, const SelfSharedPointer &left = nullptr, const SelfSharedPointer &right = nullptr, const SelfWeakPointer &parent = SelfWeakPointer()) :
				Super(value, left, right, parent) {}
			ThreadedBinaryTree(const SelfSharedPointer &pTree, const SelfSharedPointer &parent = nullptr) :
				Super(pTree, parent) {}
			ThreadedBinaryTree(const Self &tree) :
				Super(tree) {}
			ThreadedBinaryTree(Self &&tree) :
				Super(move(tree)) {}
			inline void threadify() {
				SelfSharedPointer prior;
				threadify(prior);
				prior->setRightToThread();
			}
			class Iterator :public Super::IteratorBasis {
			public:
				Iterator(const SelfSharedPointer &ptr) :Super::IteratorBasis(ptr) {}
				Iterator(SelfSharedPointer &&ptr) :Super::IteratorBasis(move(ptr)) {}
				Iterator(const Iterator &it) :Super::IteratorBasis(it.ptr) {}
				Iterator(Iterator &&it) noexcept :Super::IteratorBasis(move(it.ptr)) {}
				Iterator &operator ++ () override {
					bool rightThreaded = this->ptr->isRightAThread();
					this->ptr = this->ptr->rightChild;
					if (rightThreaded || !this->ptr)
						return *this;
					while (this->ptr->hasLeftChild())
						this->ptr = this->ptr->leftChild;
					return *this;
				}
				Iterator operator ++ (int) override {
					Iterator result(*this);
					++(*this);
					return move(result);
				}
				Iterator &operator -- () override {
					bool leftThreaded = this->ptr->isLeftAThread();
					this->ptr = this->ptr->leftChild;
					if (leftThreaded || !this->ptr)
						return *this;
					while (this->ptr->hasRightChild())
						this->ptr = this->ptr->rightChild;
					return *this;
				}
				Iterator operator -- (int) override {
					Iterator result(*this);
					--(*this);
					return move(result);
				}
			};
			class ConstIterator :public Super::ConstIteratorBasis {
				ConstIterator(const SharedPointer<const Self> &ptr) :Super::ConstIteratorBasis(ptr) {}
				ConstIterator(SharedPointer<const Self> &&ptr) :Super::ConstIteratorBasis(move(ptr)) {}
				ConstIterator(const ConstIterator &it) :Super::ConstIteratorBasis(it.ptr) {}
				ConstIterator(ConstIterator &&it) :Super::ConstIteratorBasis(move(it.ptr)) {}
				ConstIterator &operator ++ () override {
					bool rightThreaded = this->ptr->isRightAThread();
					this->ptr = constPointerCast<const Self>(this->ptr->rightChild);
					if (rightThreaded || !this->ptr)
						return *this;
					while (this->ptr->hasLeftChild())
						this->ptr = constPointerCast<const Self>(this->ptr->leftChild);
					return *this;
				}
				ConstIterator operator ++ (int) override {
					ConstIterator result(*this);
					++(*this);
					return move(result);
				}
				ConstIterator &operator -- () override {
					bool leftThreaded = this->ptr->isLeftAThread();
					this->ptr = constPointerCast<const Self>(this->ptr->leftChild);
					if (leftThreaded || !this->ptr)
						return *this;
					while (this->ptr->hasRightChild())
						this->ptr = constPointerCast<const Self>(this->ptr->rightChild);
					return *this;
				}
				ConstIterator operator -- (int) override {
					ConstIterator result(*this);
					--(*this);
					return move(result);
				}
			};
			inline Iterator begin() {
				auto ptr = this->shared_from_this();
				while (ptr->hasLeftChild())
					ptr = ptr->leftChild;
				return Iterator(move(ptr));
			}
			inline ConstIterator begin() const {
				auto ptr = this->shared_from_this();
				while (ptr->hasLeftChild())
					ptr = ptr->leftChild;
				return ConstIterator(constPointerCast<const Self>(ptr));
			}
			inline Iterator end() {
				return Iterator(nullptr);
			}
			inline ConstIterator end() const {
				return ConstIterator(nullptr);
			}
		};
		template <class T>
		class ThreadedBinaryTree<T, TraversalOrder::Postorder> :
			public ThreadedBinaryTreeBasis<T, TraversalOrder::Postorder>,
			public EnableSharedPointerFromThis<ThreadedBinaryTree<T, TraversalOrder::Postorder>> {
			using Super = ThreadedBinaryTreeBasis<T, TraversalOrder::Postorder>;
			using Self = ThreadedBinaryTree<T, TraversalOrder::Postorder>;
			using SelfSharedPointer = SharedPointer<Self>;
			using SelfWeakPointer = WeakPointer<Self>;
		private:
			void threadify(SelfSharedPointer &prior) override {
				if (this->hasLeftChild())
					this->leftChild->threadify(prior);
				if (this->hasRightChild())
					this->rightChild->threadify(prior);
				if (!this->isLeftAThread() && !this->leftChild) {
					this->setLeftToThread();
					this->leftChild = prior;
				}
				if (prior && !prior->isRightAThread() && !prior->rightChild) {
					prior->setRightToThread();
					prior->rightChild = this->shared_from_this();
				}
				prior = this->shared_from_this();
			}
		public:
			ThreadedBinaryTree(const T &value, const SelfSharedPointer &left = nullptr, const SelfSharedPointer &right = nullptr, const SelfWeakPointer &parent = SelfWeakPointer()) :
				Super(value, left, right, parent) {}
			ThreadedBinaryTree(const SelfSharedPointer &pTree, const SelfSharedPointer &parent = nullptr) :
				Super(pTree, parent) {}
			ThreadedBinaryTree(const Self &tree) :
				Super(tree) {}
			ThreadedBinaryTree(Self &&tree) :
				Super(move(tree)) {}
			inline void threadify() {
				SelfSharedPointer prior;
				threadify(prior);
			}
			class Iterator :public Super::IteratorBasis {
			public:
				Iterator(const SelfSharedPointer &ptr) :Super::IteratorBasis(ptr) {}
				Iterator(SelfSharedPointer &&ptr) :Super::IteratorBasis(move(ptr)) {}
				Iterator(const Iterator &it) :Super::IteratorBasis(it.ptr) {}
				Iterator(Iterator &&it) noexcept :Super::IteratorBasis(move(it.ptr)) {}
				Iterator &operator ++ () override {
					if (this->ptr->isRightAThread()) {
						this->ptr = this->ptr->rightChild;
						return *this;
					}
					else if (this->ptr->parent.expired()) {
						this->ptr = nullptr;
						return *this;
					}
					else {
						auto parent = this->ptr->parent.lock();
						bool isLeftChild = parent->hasLeftChild() && this->ptr == parent->leftChild;
						this->ptr = move(parent);
						if (!isLeftChild || this->ptr->isRightAThread())
							return *this;
						else {
							this->ptr = this->ptr->rightChild;
							while (true) {
								if (this->ptr->hasLeftChild())
									this->ptr = this->ptr->leftChild;
								else if (this->ptr->hasRightChild())
									this->ptr = this->ptr->rightChild;
								else
									break;
							}
						}
					}
					return *this;
				}
				Iterator operator ++ (int) override {
					Iterator result(*this);
					++(*this);
					return move(result);
				}
				Iterator &operator -- () override {
					if (this->ptr->hasRightChild())
						this->ptr = this->ptr->rightChild;
					else
						this->ptr = this->ptr->leftChild;
					return *this;
				}
				Iterator operator -- (int) override {
					Iterator result(*this);
					--(*this);
					return move(result);
				}
			};
			class ConstIterator :public Super::ConstIteratorBasis {
				ConstIterator(const SharedPointer<const Self> &ptr) :Super::ConstIteratorBasis(ptr) {}
				ConstIterator(SharedPointer<const Self> &&ptr) :Super::ConstIteratorBasis(move(ptr)) {}
				ConstIterator(const ConstIterator &it) :Super::ConstIteratorBasis(it.ptr) {}
				ConstIterator(ConstIterator &&it) :Super::ConstIteratorBasis(move(it.ptr)) {}
				ConstIterator &operator ++ () override {
					if (this->ptr->isRightAThread()) {
						this->ptr = constPointerCast<const Self>(this->ptr->rightChild);
						return *this;
					}
					else if (this->ptr->parent.expired()) {
						this->ptr = nullptr;
						return *this;
					}
					else {
						auto parent = constPointerCast<const Self>(this->ptr->parent.lock());
						bool isLeftChild = parent->hasLeftChild() && this->ptr == constPointerCast<const Self>(parent->leftChild);
						this->ptr = move(parent);
						if (!isLeftChild || this->ptr->isRightAThread())
							return *this;
						else {
							this->ptr = constPointerCast<const Self>(this->ptr->rightChild);
							while (true) {
								if (this->ptr->hasLeftChild())
									this->ptr = constPointerCast<const Self>(this->ptr->leftChild);
								else if (this->ptr->hasRightChild())
									this->ptr = constPointerCast<const Self>(this->ptr->rightChild);
								else
									break;
							}
						}
					}
					return *this;
				}
				ConstIterator operator ++ (int) override {
					ConstIterator result(*this);
					++(*this);
					return move(result);
				}
				ConstIterator &operator -- () override {
					if (this->ptr->hasRightChild())
						this->ptr = constPointerCast<const Self>(this->ptr->rightChild);
					else
						this->ptr = constPointerCast<const Self>(this->ptr->leftChild);
					return *this;
				}
				ConstIterator operator -- (int) override {
					ConstIterator result(*this);
					--(*this);
					return move(result);
				}
			};
			inline Iterator begin() {
				auto ptr = this->shared_from_this();
				while (ptr->hasLeftChild())
					ptr = ptr->leftChild;
				return Iterator(move(ptr));
			}
			inline ConstIterator begin() const {
				auto ptr = this->shared_from_this();
				while (ptr->hasLeftChild())
					ptr = ptr->leftChild;
				return ConstIterator(constPointerCast<const Self>(ptr));
			}
			inline Iterator end() {
				return Iterator(nullptr);
			}
			inline ConstIterator end() const {
				return ConstIterator(nullptr);
			}
		};
	}
}