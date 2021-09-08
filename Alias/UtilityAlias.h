#pragma once
#include <utility>
#include "AliasMacro.h"
namespace My{
	using std::move;
	template <class ...TArgs>
	using Tuple = std::tuple<TArgs...>;
	CLASS_TEMPLATE_MULTIPLE_ALIAS(std::pair, Pair, 2);
	template <class TFirst, class TSecond>
	inline Pair<TFirst, TSecond> makePair(TFirst &&first, TSecond &&second){
		return std::make_pair<TFirst, TSecond>(move(first), move(second));
	}
}