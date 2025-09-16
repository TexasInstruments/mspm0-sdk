/*
 * mcuboot_assert.h
 *
 * Cypress-specific assert() macro redefinition
 *
 */

#ifndef MCUBOOT_ASSERT_H
#define MCUBOOT_ASSERT_H

#if !defined(NDEBUG)
#undef assert
extern void mcubootFail(void);
#define assert(x, ...) \
    if (!(x)) {        \
        mcubootFail(); \
    }
#else
#define assert
#endif

#endif /* MCUBOOT_ASSERT_H */
