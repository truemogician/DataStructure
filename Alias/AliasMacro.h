#pragma once
#pragma region Function Alias
#define FUNCTION_ALIAS(src,dst) \
template<class... Args>\
inline auto dst(Args&&... args)->decltype(src(std::forward<Args>(args)...)) {\
	return src(std::forward<Args>(args)...);\
}
#pragma endregion

#pragma region Class Alias
#define CLASS_ALIAS(src,dst) typedef src dst
#pragma endregion

#pragma region Templates
#define TEMPLATE_DECLARATION1  class T1
#define TEMPLATE_DECLARATION2  TEMPLATE_DECLARATION1,  class T2
#define TEMPLATE_DECLARATION3  TEMPLATE_DECLARATION2,  class T3
#define TEMPLATE_DECLARATION4  TEMPLATE_DECLARATION3,  class T4
#define TEMPLATE_DECLARATION5  TEMPLATE_DECLARATION4,  class T5
#define TEMPLATE_DECLARATION6  TEMPLATE_DECLARATION5,  class T6
#define TEMPLATE_DECLARATION7  TEMPLATE_DECLARATION6,  class T7
#define TEMPLATE_DECLARATION8  TEMPLATE_DECLARATION7,  class T8
#define TEMPLATE_DECLARATION9  TEMPLATE_DECLARATION8,  class T9
#define TEMPLATE_DECLARATION10 TEMPLATE_DECLARATION9,  class T10
#define TEMPLATE_DECLARATION11 TEMPLATE_DECLARATION10, class T11
#define TEMPLATE_DECLARATION12 TEMPLATE_DECLARATION11, class T12
#define TEMPLATE_DECLARATION13 TEMPLATE_DECLARATION12, class T13
#define TEMPLATE_DECLARATION14 TEMPLATE_DECLARATION13, class T14
#define TEMPLATE_DECLARATION15 TEMPLATE_DECLARATION14, class T15
#define TEMPLATE_DECLARATION16 TEMPLATE_DECLARATION15, class T16
#define TEMPLATE_USAGE1  T1
#define TEMPLATE_USAGE2  TEMPLATE_USAGE1,  T2
#define TEMPLATE_USAGE3  TEMPLATE_USAGE2,  T3
#define TEMPLATE_USAGE4  TEMPLATE_USAGE3,  T4
#define TEMPLATE_USAGE5  TEMPLATE_USAGE4,  T5
#define TEMPLATE_USAGE6  TEMPLATE_USAGE5,  T6
#define TEMPLATE_USAGE7  TEMPLATE_USAGE6,  T7
#define TEMPLATE_USAGE8  TEMPLATE_USAGE7,  T8
#define TEMPLATE_USAGE9  TEMPLATE_USAGE8,  T9
#define TEMPLATE_USAGE10 TEMPLATE_USAGE9,  T10
#define TEMPLATE_USAGE11 TEMPLATE_USAGE10, T11
#define TEMPLATE_USAGE12 TEMPLATE_USAGE11, T12
#define TEMPLATE_USAGE13 TEMPLATE_USAGE12, T13
#define TEMPLATE_USAGE14 TEMPLATE_USAGE13, T14
#define TEMPLATE_USAGE15 TEMPLATE_USAGE14, T15
#define TEMPLATE_USAGE16 TEMPLATE_USAGE15, T16
#define _NAMED1(A) class A
#define _NAMED2(A,B) class A, class B
#define _NAMED3(A,B,C) class A, class B, class C
#define _NAMED4(A,B,C,D) class A, class B, class C, class D
#pragma endregion

#pragma region Common Macro
#define EXPAND(x) x
#define GET_9TH_ARG(_1, _2, _3, _4, _5, _6, _7, _8, N, ...) N
#define COUNT_ARGS(...) EXPAND(GET_9TH_ARG(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1)
#define GET_3TH_ARG(arg1,arg2,arg3, ...) arg3
#define GET_5TH_ARG(arg1,arg2,arg3,arg4,arg5,...) arg5
#define GET_NAMED_DECLARATION(...) EXPAND(GET_5TH_ARG(__VA_ARGS__,_NAMED4,_NAMED3,_NAMED2,_NAMED1))
#pragma endregion

#pragma region Class Template
#define CLASS_TEMPLATE_SINGLE_ALIAS(src, dst)		   template<class T> using dst=src<T>
#define CLASS_TEMPLATE_MULTIPLE_ALIAS(src, dst, COUNT) template<TEMPLATE_DECLARATION##COUNT> using dst=src<TEMPLATE_USAGE##COUNT>
#define CLASS_TEMPLATE_MACRO_CHOOSER(...) \
	EXPAND(GET_3TH_ARG(__VA_ARGS__, CLASS_TEMPLATE_MULTIPLE_ALIAS, CLASS_TEMPLATE_SINGLE_ALIAS, ))
#define CLASS_TEMPLATE_ALIAS(src,...) EXPAND(CLASS_TEMPLATE_MACRO_CHOOSER(__VA_ARGS__)(src,__VA_ARGS__))
#pragma endregion

#pragma region Named Class Template
#define NAMED_CLASS_TEMPLATE_ALIAS(src,dst,...)\
template <EXPAND(GET_NAMED_DECLARATION(__VA_ARGS__)(__VA_ARGS__))>\
using dst=src<EXPAND(__VA_ARGS__)>
#pragma endregion

#pragma region Function Template
#define FUNCTION_TEMPLATE_SINGLE_ALIAS(src,dst) \
template<class T, class... Args>\
inline auto dst(Args&&... args)->decltype(src<T>(std::forward<Args>(args)...)) {\
	return src<T>(std::forward<Args>(args)...);\
}
#define FUNCTION_TEMPLATE_MULTIPLE_ALIAS(src,dst,COUNT) \
template<TEMPLATE_DECLARATION##COUNT, class... Args>\
inline auto dst(Args&&... args)->decltype(src<TEMPLATE_USAGE##COUNT>(std::forward<Args>(args)...)) {\
	return src<TEMPLATE_USAGE##COUNT>(std::forward<Args>(args)...);\
}
#define FUNCTION_TEMPLATE_MACRO_CHOOSER(...) \
	EXPAND(GET_3TH_ARG(__VA_ARGS__, FUNCTION_TEMPLATE_MULTIPLE_ALIAS, FUNCTION_TEMPLATE_SINGLE_ALIAS, ))
#define FUNCTION_TEMPLATE_ALIAS(src,...) EXPAND(FUNCTION_TEMPLATE_MACRO_CHOOSER(__VA_ARGS__)(src,__VA_ARGS__))
#pragma endregion

#pragma region Named Function Template
#define NAMED_FUNCTION_TEMPLATE_ALIAS(src,dst,...)\
template <EXPAND(GET_NAMED_DECLARATION(__VA_ARGS__)(__VA_ARGS__)), class ... Args>\
inline auto dst(Args&&... args)->decltype(src<EXPAND(__VA_ARGS)>(std::forward<Args>(args)...)) {\
	return src<EXPAND(__VA_ARGS__)>(std::forward<Args>(args)...);\
}
#pragma endregion

#pragma region Value
#define VALUE_SINGLE_ALIAS(src, dst) \
template<class T>\
constexpr bool dst=src<T>::value
#define VALUE_MULTIPLE_ALIAS(src, dst, COUNT) \
template<TEMPLATE_DECLARATION##COUNT>\
constexpr bool dst=src<TEMPLATE_USAGE##COUNT>::value
#define VALUE_MACRO_CHOOSER(...) \
	EXPAND(GET_3TH_ARG(__VA_ARGS__, VALUE_MULTIPLE_ALIAS, VALUE_SINGLE_ALIAS, ))
#define VALUE_ALIAS(src,...) EXPAND(VALUE_MACRO_CHOOSER(__VA_ARGS__)(src,__VA_ARGS__))
#pragma endregion

#pragma region Named Value
#define NAMED_VALUE_ALIAS(src, dst, ...) \
template<EXPAND(GET_NAMED_DECLARATION(__VA_ARGS__)(__VA_ARGS__))>\
constexpr bool dst=src<EXPAND(__VA_ARGS__)>::value
#pragma endregion

#pragma region Type
#define TYPE_SINGLE_ALIAS(src, dst) \
template<class T>\
using dst=typename src<T>::type
#define TYPE_MULTIPLE_ALIAS(src, dst, COUNT) \
template<TEMPLATE_DECLARATION##COUNT>\
using dst=typename src<TEMPLATE_USAGE##COUNT>::type
#define TYPE_MACRO_CHOOSER(...) \
	EXPAND(GET_3TH_ARG(__VA_ARGS__, TYPE_MULTIPLE_ALIAS, TYPE_SINGLE_ALIAS, ))
#define TYPE_ALIAS(src,...) EXPAND(TYPE_MACRO_CHOOSER(__VA_ARGS__)(src,__VA_ARGS__))
#pragma endregion

#pragma region Named Type
#define NAMED_TYPE_ALIAS(src, dst,...) \
template<EXPAND(GET_NAMED_DECLARATION(__VA_ARGS__)(__VA_ARGS__))>\
using dst=typename src<EXPAND(__VA_ARGS__)>::type
#pragma endregion