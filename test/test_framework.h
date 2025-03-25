/**
 * @file test_framework.h
 * @brief Simple test framework for unit testing
 *
 * Provides macros and structures for organizing and running tests.
 *
 * @author Claude
 * @date March 25, 2025
 */

#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdio.h>
#include <stdbool.h>

/**
 * @brief Macro to verify a test condition
 *
 * If the condition is false, prints an error message with the file and line
 * number where the failure occurred, and increments the failure count.
 */
#define TEST_ASSERT(condition, message, failures) \
    do { \
        if (!(condition)) { \
            printf("  FAILED: %s (at %s:%d)\n", message, __FILE__, __LINE__); \
            (failures)++; \
        } \
    } while (0)

/**
 * @brief Macro to run a test function and report its status
 *
 * Executes the test function and reports whether it passed or failed.
 */
#define RUN_TEST(test_func, failures) \
    do { \
        int test_failures = test_func(); \
        if (test_failures > 0) { \
            printf("  Test %s FAILED (%d assertions failed)\n", #test_func, test_failures); \
            (failures) += test_failures; \
        } else { \
            printf("  Test %s PASSED\n", #test_func); \
        } \
    } while (0)

/**
 * @brief Represents a test suite containing multiple tests
 */
typedef struct {
    const char *name;        /**< Name of the test suite */
    int (*run_tests)(void);  /**< Function to run all tests in the suite */
} TestSuite;

#endif /* TEST_FRAMEWORK_H */