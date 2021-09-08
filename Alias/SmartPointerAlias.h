#pragma once
#include <memory>
#include "AliasMacro.h"
namespace My{
	CLASS_TEMPLATE_ALIAS(std::shared_ptr, SharedPointer);
	CLASS_TEMPLATE_ALIAS(std::unique_ptr, UniquePointer);
	CLASS_TEMPLATE_ALIAS(std::weak_ptr, WeakPointer);
	CLASS_TEMPLATE_ALIAS(std::enable_shared_from_this, EnableSharedPointerFromThis);
	FUNCTION_TEMPLATE_ALIAS(std::const_pointer_cast, constPointerCast);
	FUNCTION_TEMPLATE_ALIAS(std::dynamic_pointer_cast, dynamicPointerCast);
	FUNCTION_TEMPLATE_ALIAS(std::static_pointer_cast, staticPointerCast);
	FUNCTION_TEMPLATE_ALIAS(std::make_shared, makeShared);
	FUNCTION_TEMPLATE_ALIAS(std::make_unique, makeUnique);
}