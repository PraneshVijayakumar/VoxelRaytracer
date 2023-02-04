#ifndef FORGE_ASSERT_H
#define FORGE_ASSERT_H

#include <cassert>
#include <Forge/Source/Core/Logging.h>

#define FORGE_ASSERT(pAssertExpression) \
do \
{ \
assert(pAssertExpression); \
if((pAssertExpression) == false) \
{ \
FORGE_ERROR_LOG("Assertion failed"); \
} \
}while(false)

#define FORGE_STATIC_ASSERT(pAssertExpression) static_assert(pAssertExpression)

#endif