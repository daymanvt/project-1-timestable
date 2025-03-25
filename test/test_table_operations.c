/**
 * @file test_table_operations.c
 * @brief Implementation of tests for table operation functions
 *
 * Tests for multiply, divide, and power operations.
 *
 * @author Claude
 * @date March 25, 2025
 */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "test_framework.h"
#include "test_table_operations.h"
#include "table_operations.h"

/**
 * @brief Test the multiply operation
 *
 * Checks that multiplication gives the expected results, including
 * edge cases like zero and negative numbers.
 *
 * @return int Number of failed tests
 */
static int test_multiply(void)
{
    int failures = 0;
    CellValue result;

    /* Test basic multiplication with positive numbers */
    multiply(5, 7, &result);
    TEST_ASSERT(result.is_numeric == true, "Multiplication result should be numeric", failures);
    TEST_ASSERT(result.num_value == 35, "5 * 7 should equal 35", failures);

    /* Test multiplication with zero */
    multiply(10, 0, &result);
    TEST_ASSERT(result.is_numeric == true, "Multiplication result with zero should be numeric", failures);
    TEST_ASSERT(result.num_value == 0, "10 * 0 should equal 0", failures);

    /* Test multiplication with negative numbers */
    multiply(-3, 4, &result);
    TEST_ASSERT(result.is_numeric == true, "Multiplication result with negative should be numeric", failures);
    TEST_ASSERT(result.num_value == -12, "-3 * 4 should equal -12", failures);

    multiply(-5, -6, &result);
    TEST_ASSERT(result.is_numeric == true, "Multiplication result with two negatives should be numeric", failures);
    TEST_ASSERT(result.num_value == 30, "-5 * -6 should equal 30", failures);

    return failures;
}

/**
 * @brief Test the divide operation
 *
 * Checks that division gives the expected results, including
 * division by zero handling.
 *
 * @return int Number of failed tests
 */
static int test_divide(void)
{
    int failures = 0;
    CellValue result;

    /* Test basic division */
    divide(10, 2, &result);
    TEST_ASSERT(result.is_numeric == true, "Division result should be numeric", failures);
    TEST_ASSERT(result.num_value == 5, "10 / 2 should equal 5", failures);

    /* Test division with remainder (integer division) */
    divide(7, 2, &result);
    TEST_ASSERT(result.is_numeric == true, "Division with remainder should be numeric", failures);
    TEST_ASSERT(result.num_value == 3, "7 / 2 should equal 3 (integer division)", failures);

    /* Test division by zero */
    divide(5, 0, &result);
    TEST_ASSERT(result.is_numeric == false, "Division by zero should not be numeric", failures);
    TEST_ASSERT(strcmp(result.str_value, "UDF") == 0, "Division by zero should return UDF string", failures);

    /* Test division with negative numbers */
    divide(-12, 4, &result);
    TEST_ASSERT(result.is_numeric == true, "Division with negative should be numeric", failures);
    TEST_ASSERT(result.num_value == -3, "-12 / 4 should equal -3", failures);

    divide(-15, -3, &result);
    TEST_ASSERT(result.is_numeric == true, "Division with two negatives should be numeric", failures);
    TEST_ASSERT(result.num_value == 5, "-15 / -3 should equal 5", failures);

    return failures;
}

/**
 * @brief Test the power operation
 *
 * Checks that exponentiation gives the expected results, including
 * edge cases like zero and negative numbers.
 *
 * @return int Number of failed tests
 */
static int test_power(void)
{
    int failures = 0;
    CellValue result;

    /* Test basic power operation */
    power(2, 3, &result);
    TEST_ASSERT(result.is_numeric == true, "Power result should be numeric", failures);
    TEST_ASSERT(result.num_value == 8, "2^3 should equal 8", failures);

    /* Test power with base 0 */
    power(0, 5, &result);
    TEST_ASSERT(result.is_numeric == true, "0^5 result should be numeric", failures);
    TEST_ASSERT(result.num_value == 0, "0^5 should equal 0", failures);

    /* Test power with exponent 0 */
    power(7, 0, &result);
    TEST_ASSERT(result.is_numeric == true, "7^0 result should be numeric", failures);
    TEST_ASSERT(result.num_value == 1, "7^0 should equal 1", failures);

    /* Test power with exponent 1 */
    power(5, 1, &result);
    TEST_ASSERT(result.is_numeric == true, "5^1 result should be numeric", failures);
    TEST_ASSERT(result.num_value == 5, "5^1 should equal 5", failures);

    /* Test power with negative base, even exponent */
    power(-3, 2, &result);
    TEST_ASSERT(result.is_numeric == true, "(-3)^2 result should be numeric", failures);
    TEST_ASSERT(result.num_value == 9, "(-3)^2 should equal 9", failures);

    /* Test power with negative base, odd exponent */
    power(-2, 3, &result);
    TEST_ASSERT(result.is_numeric == true, "(-2)^3 result should be numeric", failures);
    TEST_ASSERT(result.num_value == -8, "(-2)^3 should equal -8", failures);

    /* Test power with larger values */
    power(10, 3, &result);
    TEST_ASSERT(result.is_numeric == true, "10^3 result should be numeric", failures);
    TEST_ASSERT(result.num_value == 1000, "10^3 should equal 1000", failures);

    return failures;
}

/**
 * @brief Run all tests for the table operations
 *
 * @return int Number of failed tests
 */
int run_table_operations_tests(void)
{
    int failures = 0;

    RUN_TEST(test_multiply, failures);
    RUN_TEST(test_divide, failures);
    RUN_TEST(test_power, failures);

    return failures;
}