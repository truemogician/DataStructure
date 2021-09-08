#pragma once
#include <type_traits>
#include "../Alias/TypeTraitAlias.h"
namespace My{
	namespace Algorithm{
		template <class T>
		struct TypeRange{
			static const T min;
			static const T max;
		};
		#define SPECIALIZE_TYPERANGE(type,minimum,maximum) \
		template <>\
		const type TypeRange<type>::min=minimum;\
		const type TypeRange<type>::max=maximum

		SPECIALIZE_TYPERANGE(char, CHAR_MIN, CHAR_MAX);
		SPECIALIZE_TYPERANGE(unsigned char, 0, UCHAR_MAX);
		SPECIALIZE_TYPERANGE(short, SHRT_MIN, SHRT_MAX);
		SPECIALIZE_TYPERANGE(unsigned short, 0, USHRT_MAX);
		SPECIALIZE_TYPERANGE(int, INT_MIN, INT_MAX);
		SPECIALIZE_TYPERANGE(unsigned int, 0, UINT_MAX);
		SPECIALIZE_TYPERANGE(long long, LLONG_MIN, LLONG_MAX);
		SPECIALIZE_TYPERANGE(unsigned long long, 0, ULLONG_MAX);
		SPECIALIZE_TYPERANGE(float, FLT_MIN, FLT_MAX);
		SPECIALIZE_TYPERANGE(double, DBL_MIN, DBL_MAX);
		SPECIALIZE_TYPERANGE(long double, LDBL_MIN, LDBL_MAX);
		template <class T>
		struct Random{
			typedef unsigned char byte;
			template <class D=T>
			static EnabledType<isArithmetic<D>, D> get(){
				if (isSame<T, bool>)
					return rand() & 1;
				byte bits[sizeof(T)];
				for (byte i = 0; i < sizeof(T); ++i)
					bits[i] = byte(rand()) << 1 | (byte(rand()) & 1);
				T result;
				memcpy(&result, bits, sizeof(T));
				return result;
			}
			template <class D=T>
			static EnabledType<isFloatingPoint<D>, D> get(const T &min, const T &max = TypeRange<T>::max){
				auto range = max - min;
				return min + (T)rand() / RAND_MAX * range;
			}
			template <class D=T>
			static EnabledType<isIntegral<D>, D> get(const T &min, const T &max = TypeRange<T>::max){
				return min + get<MakeUnsigned<T>>() % (max - min + 1);
			}
		};
	}
}