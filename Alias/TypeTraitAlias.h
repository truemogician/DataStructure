#pragma once
#include <type_traits>
#include "AliasMacro.h"
namespace My{
	VALUE_ALIAS(std::is_void, isVoid);
	VALUE_ALIAS(std::is_null_pointer, isNullPointer);
	VALUE_ALIAS(std::is_integral, isIntegral);
	VALUE_ALIAS(std::is_floating_point, isFloatingPoint);
	VALUE_ALIAS(std::is_array, isArray);
	VALUE_ALIAS(std::is_enum, isEnum);
	VALUE_ALIAS(std::is_union, isUnion);
	VALUE_ALIAS(std::is_class, isClass);
	VALUE_ALIAS(std::is_function, isFunction);
	VALUE_ALIAS(std::is_pointer, isPointer);
	VALUE_ALIAS(std::is_lvalue_reference, isLvalueReference);
	VALUE_ALIAS(std::is_rvalue_reference, isRvalueReference);
	VALUE_ALIAS(std::is_member_object_pointer, isMemberObjectPointer);
	VALUE_ALIAS(std::is_member_function_pointer, isMemberFunctionPointer);
	VALUE_ALIAS(std::is_fundamental, isFundamental);
	VALUE_ALIAS(std::is_arithmetic, isArithmetic);
	VALUE_ALIAS(std::is_scalar, isScalar);
	VALUE_ALIAS(std::is_object, isObject);
	VALUE_ALIAS(std::is_compound, isCompound);
	VALUE_ALIAS(std::is_reference, isReference);
	VALUE_ALIAS(std::is_member_pointer, isMemberPointer);
	VALUE_ALIAS(std::is_const, isConst);
	VALUE_ALIAS(std::is_volatile, isVolatile);
	VALUE_ALIAS(std::is_trivial, isTrivial);
	VALUE_ALIAS(std::is_trivially_copyable, isTriviallyCopyable);
	VALUE_ALIAS(std::is_standard_layout, isStandardLayout);
	VALUE_ALIAS(std::is_empty, isEmpty);
	VALUE_ALIAS(std::is_polymorphic, isPolymorphic);
	VALUE_ALIAS(std::is_abstract, isAbstract);
	VALUE_ALIAS(std::is_final, isFinal);
	VALUE_ALIAS(std::is_signed, isSigned);
	VALUE_ALIAS(std::is_unsigned, isUnsigned);
	NAMED_VALUE_ALIAS(std::is_base_of, isBaseOf, TBase, TDerived);
	NAMED_VALUE_ALIAS(std::is_same, isSame, T1, T2);
	TYPE_ALIAS(std::remove_const, RemoveConst);
	TYPE_ALIAS(std::remove_volatile, RemoveVolatile);
	TYPE_ALIAS(std::remove_cv, RemoveConstVolatile);
	TYPE_ALIAS(std::add_const, AddConst);
	TYPE_ALIAS(std::add_volatile, AddVolatile);
	TYPE_ALIAS(std::add_cv, AddConstVolatile);
	TYPE_ALIAS(std::remove_reference, RemoveReference);
	TYPE_ALIAS(std::add_lvalue_reference, AddLvalueReference);
	TYPE_ALIAS(std::add_rvalue_reference, AddRvalueReference);
	TYPE_ALIAS(std::remove_pointer, RemovePointer);
	TYPE_ALIAS(std::add_pointer, AddPointer);
	TYPE_ALIAS(std::make_signed, MakeSigned);
	TYPE_ALIAS(std::make_unsigned, MakeUnsigned);
	template <bool Condition, class T=void>
	using EnabledType = typename std::enable_if<Condition, T>::type;
}