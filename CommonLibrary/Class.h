#pragma once
namespace My {
	template <class T>
	struct PointerLess {
		bool operator () (const T *const &a, const T *const &b) const{
			return *a < *b || (!(*b < *a) && a < b);
		}
	};
	template <class T>
	struct PointerGreater {
		bool operator () (const T *const &a, const T *const &b) const {
			return *b < *a || (!(*a < *b) && b < a);
		}
	};
	template <class T>
	struct PointerLessEqual {
		bool operator () (const T *const &a, const T *const &b) const {
			return !(*b < *a) && (*a < *b || !(b < a));
		}
	};
	template <class T>
	struct PointerGreaterEqual {
		bool operator () (const T *const &a, const T *const &b) const {
			return !(*a < *b) && (*b < *a || !(a < b));
		}
	};
	template <class Enum, unsigned int Size>
	class Flags {
		typedef unsigned char uchar;
	private:
		uchar flags[(Size + 7) >> 3] = { 0 };
	public:
		Flags() {}
		Flags &set(const Enum &flag, const bool &value) {
			int index = (int)flag;
			value ? flags[index >> 3] |= 1 << (index & 7) : flags[index >> 3] &= ~(1 << (index & 7));
			return *this;
		}
		bool get(const Enum &flag) {
			int index = (int)flag;
			return flags[index >> 3] & (1 << (index & 7));
		}
	};
}