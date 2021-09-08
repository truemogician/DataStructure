#pragma once
#include "../Alias/ExceptionAlias.h"
#include <cstring>
namespace My {
	namespace LinearList {
		using std::move;
		template <class T>
		class Queue {
			typedef unsigned long long ull;
			typedef unsigned int uint;
		private:
			ull l, r;
			uint cap;
			T *data;
		public:
			Queue() {
				data = new T[1];
				l = r = 0;
				cap = 1;
			}
			Queue(const uint &size) {
				data = new T[size];
				l = r = 0;
				cap = size;
			}
			Queue(const Queue &q) :l(q.l), r(q.r), cap(q.cap) {
				data = new T[cap];
				memcpy(data, q.data, sizeof(T) * q.cap);
			}
			Queue(Queue &&q) noexcept :l(q.l), r(q.r), cap(q.cap) {
				data = q.data;
				q.data = nullptr;
				q.l = q.r = q.cap = 0;
			}
			inline bool empty() const {
				return l == r;
			}
			inline uint size() const {
				return (uint)(r - l);
			}
			inline uint capacity() const {
				return cap;
			}
			inline T &head() {
				return data[l % cap];
			}
			inline const T &head() const {
				return data[l % cap];
			}
			inline T &tail() {
				return data[(r - 1 + cap) % cap];
			}
			inline const T &tail() const {
				return data[(r - 1 + cap) % cap];
			}
			void resize(const ull &size) {
				if (size <= cap)
					return;
				T *tmp = new T[size];
				memcpy_s(tmp, sizeof(T) * size, data, sizeof(T) * cap);
				delete data;
				data = tmp;
				cap = size;
			}
			void clear(const T &value = T()) {
				for (int i = 0; i < r - l; i++)
					data[(l + i) % cap] = value;
			}
			Queue &push(const T &&value) {
				if (r - l == cap) {
					T *tmp = new T[(size_t)cap << 1];
					uint separator = cap - l % cap;
					memcpy(tmp, data + l % cap, sizeof(T) * separator);
					memcpy(tmp + separator, data, sizeof(T) * (l % cap));
					delete[] data;
					data = tmp;
					l = 0, r = cap;
					cap <<= 1;
				}
				data[r++ % cap] = value;
				return *this;
			}
			Queue &push(const T &value) {
				return push(std::move(value));
			}
			T pop() {
				if (empty())
					throw OutOfRangeException("Try to pop out an element from an empty queue");
				return move(data[l++ % cap]);
			}
			T &operator [] (const ull &index) {
				if (index > r - l) {
					char message[1 << 6];
					sprintf_s(message, "Request for index %lld, but the length is %lld", index, r - l);
					throw OutOfRangeException(message);
				}
				return data[(l + index) % cap];
			}
			const T &operator [] (const ull &index) const {
				if (index > r - l) {
					char message[1 << 6];
					sprintf_s(message, "Request for index %lld, but the length is %lld", index, r - l);
					throw OutOfRangeException(message);
				}
				return data[(l + index) % cap];
			}
			~Queue() {
				if (data)
					delete[] data;
			}
		};
		template <class T>
		std::ostream &operator << (std::ostream &out, const Queue<T> &q) {
			for (int i = 0; i < (int)q.size() - 1; i++)
				out << q[i] << ' ';
			out << q.tail();
			return out;
		}
	}
}