#pragma once
#include "../BinaryTree/BinaryTree.h"
namespace My {
	namespace Tree {
		template <class BinaryTree, class T>
		class IAVL {
		public:
			T value;
			BinaryTree *leftChild = nullptr, *rightChild = nullptr;
			BinaryTree *parent = nullptr;
			IAVL(const T &value = T()) :value(value) {}
			IAVL(T &&value) :value(move(value)) {}
			IAVL(const T &value, BinaryTree *const &leftChild, BinaryTree *const &rightChild, BinaryTree *const &parent) :
				value(value), leftChild(leftChild), rightChild(rightChild), parent(parent) {};
			bool isLeft() const {
				return parent && this == dynamic_cast<IAVL *>(parent->leftChild);
			}
			bool isRight() const {
				return parent && this == dynamic_cast<IAVL *>(parent->rightChild);
			}
			inline BinaryTree *&childOfParent() const{
				return isLeft() ? parent->leftChild : parent->rightChild;
			}
			inline BinaryTree *&sibling() const{
				return isLeft() ? parent->rightChild : parent->leftChild;
			}
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
			virtual void draw(int space = 5, int depth = 0) const {
				if (rightChild)
					rightChild->draw(space, depth + 1);
				for (int i = 0; i < depth * space; ++i)
					putchar(' ');
				std::cout << value << std::endl;
				if (leftChild)
					leftChild->draw(space, depth + 1);
			}
		};

		template <class T, class TComparer>
		class Set;

		template <class T, class TComparer = std::less<T>>
		class AVL : public IAVL<AVL<T, TComparer>, T> {
			using Super = IAVL<AVL<T, TComparer>, T>;
			using Comparison = Function<bool(const T &, const T &)>;
			friend class Set<T, TComparer>;
		protected:
			static const TComparer less;
			static const Comparison greater;
			static const Comparison equal;
			int height = 1;
			const int &updateHeight() {
				return height = 1 + std::max(this->leftChild ? this->leftChild->height : 0, this->rightChild ? this->rightChild->height : 0);
			}
			inline int getBalanceFactor() const {
				return (this->rightChild ? this->rightChild->height : 0) - (this->leftChild ? this->leftChild->height : 0);
			}
			inline bool isUnbalanced() const {
				int factor = getBalanceFactor();
				return factor < -1 || factor>1;
			}
			inline AVL *childOrSelf() {
				return this->leftChild ? this->leftChild : (this->rightChild ? this->rightChild : this);
			}
			AVL *&insertPosition(const T &value, AVL *&parent) {
				AVL *result = this;
				do {
					auto &child = less(value, result->value) ? result->leftChild : result->rightChild;
					if (!child) {
						parent = result;
						return child;
					}
					else
						result = child;
				} while (true);
			}
			AVL * const &insertPosition(const T &value, const AVL *&parent) const{
				const AVL *result = this;
				do {
					auto &child = less(value, result->value) ? result->leftChild : result->rightChild;
					if (!child) {
						parent = result;
						return child;
					}
					else
						result = child;
				} while (true);
			}
			inline AVL *&insertPosition(const T &value) {
				AVL *tmp;
				return insertPosition(value, tmp);
			}
			inline AVL * const &insertPosition(const T &value) const{
				const AVL *tmp;
				return insertPosition(value, tmp);
			}
			void rotateLeft(AVL *&root) {
				if (!this->rightChild)
					throw LogicErrorException("Right child cannot be null");
				auto right = this->rightChild, parent = this->parent;
				auto rightLeft = right ? right->leftChild : nullptr;

				if (parent)
					this->childOfParent() = right;
				this->parent = right;
				this->rightChild = rightLeft;
				right->parent = parent;
				right->leftChild = this;
				if (rightLeft)
					rightLeft->parent = this;

				this->updateHeight();
				right->updateHeight();
				if (parent)
					parent->updateHeight();
				else
					root = right;
			}
			void rotateRight(AVL *&root) {
				if (!this->leftChild)
					throw LogicErrorException("Left child cannot be null");
				auto left = this->leftChild, parent = this->parent;
				auto leftRight = left ? left->rightChild : nullptr;

				if (parent)
					this->childOfParent() = left;
				this->parent = left;
				this->leftChild = leftRight;
				left->parent = parent;
				left->rightChild = this;
				if (leftRight)
					leftRight->parent = this;

				this->updateHeight();
				left->updateHeight();
				if (parent)
					parent->updateHeight();
				else
					root = left;
			}
			inline void rotateRightLeft(AVL *&root) {
				if (!this->rightChild)
					throw LogicErrorException("Right child cannot be null");
				this->rightChild->rotateRight(root);
				this->rotateLeft(root);
			}
			inline void rotateLeftRight(AVL *&root) {
				if (!this->leftChild)
					throw LogicErrorException("Left child cannot be null");
				this->leftChild->rotateLeft(root);
				this->rotateRight(root);
			}
			void rebalance(unsigned char optCode, AVL *&root) {
				switch (optCode) {
					case 0: {
						this->rotateRight(root);
						break;
					}
					case 1: {
						this->rotateRightLeft(root);
						break;
					}
					case 2: {
						this->rotateLeftRight(root);
						break;
					}
					case 3: {
						this->rotateLeft(root);
						break;
					}
				}
			}
		public:
			AVL() :Super() {}
			AVL(const T &value) :Super(value) {}
			AVL(T &&value) :Super(move(value)) {}
			int getHeight() const override {
				return height;
			}
			const AVL *insert(T &&value, AVL *&root) {
				AVL *parent;
				auto &result = insertPosition(value, parent);
				result = new AVL(move(value));
				result->parent = parent;
				AVL *tail = result, *middle = parent, *head = middle->parent;
				middle->updateHeight();
				unsigned char pattern = tail == middle->rightChild;
				while (head) {
					head->updateHeight();
					pattern = (pattern << 1 | (middle == head->rightChild)) & 3;
					if (head->isUnbalanced()) {
						head->rebalance(pattern, root);
						if (pattern == 0 || pattern == 3)
							head = middle->parent;
						else {
							auto tmp = head;
							head = tail->parent;
							middle = tail;
							tail = tmp;
						}
					}
					else {
						tail = middle;
						middle = head;
						head = head->parent;
					}
				}
				return result;
			}
			inline const AVL *insert(const T &value, AVL *&root) {
				auto copy(value);
				return insert(move(copy), root);
			}
			AVL *findLessEqual(const T &value) {
				AVL *result;
				insertPosition(value, result);
				while (result && greater(result->value, value))
					result = result->parent;
				return result;
			}
			const AVL *findLessEqual(const T &value) const{
				const AVL *result;
				insertPosition(value, result);
				while (result && greater(result->value, value))
					result = result->parent;
				return result;
			}
			int count(const T &value) const {
				int result = 0;
				const AVL *ptr = findLessEqual(value);
				while (ptr && equal(value, ptr->value)) {
					++result;
					ptr = ptr->parent;
				}
				return result;
			}
			bool remove(const T &value, AVL *&root) {
				auto result = findLessEqual(value);
				if (!result || !equal(result->value, value))
					return false;
				auto left = result->leftChild, right = result->rightChild;
				auto target = result;
				if (left || right) {
					target = left ? left : right;
					while (target->rightChild)
						target = target->rightChild;
					std::swap(result->value, target->value);
					if (target->leftChild) {
						std::swap(target->value, target->leftChild->value);
						target = target->leftChild;
					}
				}
				AVL *pre = target;
				AVL *cur = target->parent;
				if (!cur)
					root = nullptr;
				else{
					target->height = 0;
					bool shorter = true;
					int preFactor, curFactor;
					while (cur && shorter) {
						curFactor = cur->getBalanceFactor();
						preFactor = curFactor + (pre == cur->leftChild ? -1 : 1);
						if (preFactor == 0) {
							shorter = false;
							pre = cur;
							cur = cur->parent;
							pre->updateHeight();
						}
						else {
							if (std::abs(curFactor) < std::abs(preFactor)) {
								pre = cur;
								cur = cur->parent;
								pre->updateHeight();
							}
							else {
								auto sibling = pre->sibling();
								unsigned char optCode = sibling->isRight();
								optCode = optCode << 1 | optCode;
								if (preFactor * sibling->getBalanceFactor() < 0)
									optCode ^= 2;
								if (sibling->getBalanceFactor() == 0)
									shorter = false;
								cur->rebalance(optCode, root);
								pre = cur->parent;
								cur = pre->parent;
							}
						}
					}
					target->childOfParent() = nullptr;
				}
				delete target;
				return true;
			}
		};
		template<class T, class TComparer>
		const TComparer AVL<T, TComparer>::less = TComparer();
		template<class T, class TComparer>
		const Function<bool(const T &, const T &)> AVL<T, TComparer>::greater = [](const T &a, const T &b) {
			return AVL<T, TComparer>::less(b, a);
		};
		template<class T, class TComparer>
		const Function<bool(const T &, const T &)> AVL<T, TComparer>::equal = [](const T &a, const T &b) {
			return !AVL<T, TComparer>::less(a, b) && !AVL<T, TComparer>::less(b, a);
		};

		template <class T, class TComparer = std::less<T>>
		class Set {
			using Tree = AVL<T, TComparer>;
		public:
			Tree *root;
		public:
			Set() :root(nullptr) {}
			int getSize() const {
				return root ? root->getSize() : 0;
			}
			bool isEmpty() const {
				return root == nullptr;
			}
			const Tree *insert(T &&value) {
				if (root)
					return root->insert(move(value), root);
				else
					return root = new Tree(move(value));
			}
			inline const Tree *insert(const T &value) {
				T copy(value);
				return insert(move(copy));
			}
			const Tree *find(const T &value) const {
				if (!root)
					return nullptr;
				auto result = root->findLessEqual(value);
				if (result && Tree::equal(value, result->value))
					return result;
				return nullptr;
			}
			const Tree *findLess(const T &value) const {
				return root ? root->findLess(value) : nullptr;
			}
			int count(const T &value) const {
				return root ? root->count(value) : 0;
			}
			bool remove(const T &value) {
				return root ? root->remove(value, root) : false;
			}
		};
	}
}