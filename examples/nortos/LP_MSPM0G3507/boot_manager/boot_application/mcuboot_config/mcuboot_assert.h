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
#define assert(...) BOOT_LOG_ERR(__VA_ARGS__)
#else
#define assert
#endif

#endif /* MCUBOOT_ASSERT_H */
