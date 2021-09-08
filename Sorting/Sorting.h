#pragma once
#include <cmath>
#include <cstring>
#include "../Alias/FunctionAlias.h"
namespace My{
	namespace Algorithm{
		using std::swap;
		using std::move;
		template <class T, class TComparer = Less<T>>
		class Sorting{
		private:
			static const TComparer less;
			static void merge(T *begin, T *end, T *tmp){
				if (end - begin <= 1)
					return;
				T *middle = begin + ((end - begin) >> 1);
				merge(begin, middle, tmp);
				merge(middle, end, tmp);
				T *frontCursor = begin, *backCursor = middle;
				for (T *cursor = tmp; cursor != tmp + (end - begin); ++cursor){
					if (frontCursor != middle && (backCursor == end || less(*frontCursor, *backCursor))){
						*cursor = std::move(*frontCursor);
						++frontCursor;
					}
					else{
						*cursor = std::move(*backCursor);
						++backCursor;
					}
				}
				memmove(begin, tmp, sizeof(T) * (end - begin));
			}
			static void adjustHeap(T *begin, T *root, T *end){
				T tmp = move(*root);
				while (root < end){
					T *child = begin + ((root - begin) << 1 | 1);
					if (child + 1 < end && less(*child, *(child + 1)))
						++child;
					if (child >= end || !less(tmp, *child))
						break;
					*root = move(*child);
					root = child;
				}
				*root = move(tmp);
			}
		public:
			static void bubble(T *begin, T *end){
				for (T *i = end; i != begin; --i){
					for (T *j = begin + 1; j != i; ++j){
						if (!less(*(j - 1), *j))
							swap(*(j - 1), *j);
					}
				}
			}
			static void selection(T *begin, T *end){
				for (T *i = begin; i != end; ++i){
					T *min = i;
					for (T *j = i + 1; j != end; ++j)
						if (less(*j, *min))
							min = j;
					if (min != i)
						swap(*i, *min);
				}
			}
			static void insertion(T *begin, T *end){
				for (T *i = begin + 1; i != end; ++i){
					for (T *j = i - 1; j >= begin && less(*(j + 1), *j); --j)
						swap(*j, *(j + 1));
				}
			}
			static void shell(T *begin, T *end){
				if (end - begin <= 1)
					return;
				int count = (int)floor((log(5 * (end - begin) + 4) - log(9)) / log(2.25)) + 1;
				int *gaps = new int[count];
				double base = 1;
				for (int i = 0; i < count; ++i){
					gaps[i] = (int)ceil(0.2 * (9 * base - 4));
					base *= 2.25;
				}
				for (int i = count - 1; i >= 0; --i){
					int &gap = gaps[i];
					for (int j = gap; j < end - begin; ++j){
						for (T *k = begin + j; k - gap >= begin && less(*k, *(k - gap)); k -= gap)
							swap(*k, *(k - gap));
					}
				}
			}
			static void merge(T *begin, T *end){
				if (end - begin <= 1)
					return;
				T *tmp = new T[end - begin];
				merge(begin, end, tmp);
			}
			static void heap(T *begin, T *end){
				if (end - begin <= 1)
					return;
				for (T *i = begin + ((end - begin) >> 1); i >= begin; --i)
					adjustHeap(begin, i, end);
				for (T *i = end - 1; i > begin; --i){
					swap(*begin, *i);
					adjustHeap(begin, begin, i);
				}
			}
			static void quick(T *begin, T *end){
				if (end - begin <= 1)
					return;
				T *left = begin, *right = end - 1;
				T benchmark = *begin;
				while (left < right){
					for (; left < right && !less(*right, benchmark); --right);
					if (left == right)
						break;
					*left = *right;
					++left;
					if (left == right)
						break;
					for (; left < right && less(*left, benchmark); ++left);
					if (left == right)
						break;
					*right = *left;
					--right;
				}
				*left = benchmark;
				if (left != begin)
					quick(begin, left);
				if (right != end - 1)
					quick(right + 1, end);
			}
		};
		template <class T, class TComparer>
		const TComparer Sorting<T, TComparer>::less = TComparer();
	}
}