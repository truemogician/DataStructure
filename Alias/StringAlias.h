#pragma once
#include <string>
#include "AliasMacro.h"
namespace My{
	using std::string;
	FUNCTION_ALIAS(std::to_string, toString);
}