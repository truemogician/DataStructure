#pragma once
#include <stdexcept>
#include "AliasMacro.h"
namespace My{
	CLASS_ALIAS(std::exception, Exception);
	CLASS_ALIAS(std::out_of_range, OutOfRangeException);
	CLASS_ALIAS(std::invalid_argument, InvalidArgumentException);
	CLASS_ALIAS(std::logic_error, LogicErrorException);
	CLASS_ALIAS(std::domain_error, DomainErrorException);
	CLASS_ALIAS(std::length_error, LengthErrorException);
	CLASS_ALIAS(std::runtime_error, RuntimeErrorException);
	CLASS_ALIAS(std::overflow_error, OverflowErrorException);
	CLASS_ALIAS(std::underflow_error, UnderflowErrorException);
	CLASS_ALIAS(std::range_error, RangeErrorException);
}