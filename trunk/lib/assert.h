/**
 * @file assert.h
 * @brief Assert macro with logging function
 * Based on assert.g from RetroBSD
 * @author Daniel Szot
 */

#ifndef _LIBRARY_ASSERT_H_
#define _LIBRARY_ASSERT_H_

#if defined(ASSERT_LOGING)

#define _assert(ex) \
{ \
    if (!(ex)) \
    { \
        ERROR("Assertion (%s) failed: file \"%s\", line %d\n", #ex, __FILE__, __LINE__); \
        while(1); \
    } \
}

#define assert(ex)	_assert(ex)
#else
#define _assert(ex)
#define assert(ex)
# endif

#endif
