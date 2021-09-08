#pragma once
namespace My {
	template <template <class> class TContainer, class TValue>
	class IContainerConversion {
	public:
		virtual TValue *toArray() const = 0;
	};
	template <class TIterator, class TValue, bool Mutable = true>
	class IContainerIterator {
	public:
		virtual TValue *operator -> () const = 0;
		virtual TValue &operator * () const = 0;
		virtual TIterator &operator ++ () = 0;
		virtual TIterator operator ++ (int) = 0;
		virtual TIterator &operator -- () = 0;
		virtual TIterator operator -- (int) = 0;
		virtual bool operator == (const TIterator &it) const = 0;
		virtual bool operator != (const TIterator &it) const = 0;
	};
	template <class TIterator, class TValue>
	class IContainerIterator<TIterator, TValue, false> {
	public:
		virtual TValue const *operator -> () const = 0;
		virtual const TValue &operator * () const = 0;
		virtual TIterator &operator ++ () = 0;
		virtual TIterator operator ++ (int) = 0;
		virtual TIterator &operator -- () = 0;
		virtual TIterator operator -- (int) = 0;
		virtual bool operator == (const TIterator &it) const = 0;
		virtual bool operator != (const TIterator &it) const = 0;
	};
	namespace LinearList {
		template <template <class> class TContainer, class TValue>
		class IMultiContainerOperation {
		public:
			virtual TContainer<TValue> &concat(TContainer<TValue> &&list) = 0;
			virtual TContainer<TValue> **split(const int &index) = 0;
		};
	}
}
