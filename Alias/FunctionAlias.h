#pragma once
#include <functional>
#include "AliasMacro.h"
namespace My{
	CLASS_TEMPLATE_ALIAS(std::function, Function);
	CLASS_TEMPLATE_ALIAS(std::less, Less);
	CLASS_TEMPLATE_ALIAS(std::less_equal, LessEqual);
	CLASS_TEMPLATE_ALIAS(std::greater, Greater);
	CLASS_TEMPLATE_ALIAS(std::greater_equal, GreaterEqual);
	CLASS_TEMPLATE_ALIAS(std::equal_to, Equal);
	CLASS_TEMPLATE_ALIAS(std::not_equal_to, NotEqual);
}