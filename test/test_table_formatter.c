/**
 * @file test_table_formatter.c
 * @brief Implementation of tests for table formatting functions
 *
 * Tests for print_table functionality. Since print_table writes to stdout,
 * we use redirection to capture and validate its output.
 *
 * @author Claude
 * @date March 25, 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "test_framework.h"
#include "test_helpers.h"
#include "test_table_formatter.h"
#include "timestable_formatter.h"
#include "timestable_operations.h"

/* Buffer size for capturing stdout */
#define BUFFER_SIZE 4096

/**
 * @brief Mock operation for consistent testing
 *
 * Always returns row + column as the result
 *
 * @param row Row value
 * @param column Column value
 * @param result Pointer to store the result
 */
static void mock_add(int row, int column, cell_value_t *result)
{
    result->is_numeric = true;
    result->num_value = row + column;
    result->str_value[0] = '\0';
}

/**
 * @brief Mock operation that returns non-numeric results
 *
 * Returns a string result based on whether row > column
 *
 * @param row Row value
 * @param column Column value
 * @param result Pointer to store the result
 */
static void mock_string_result(int row, int column, cell_value_t *result)
{
    if (row > column) {
        result->is_numeric = false;
        strcpy(result->str_value, "GT");
    } else if (row < column) {
        result->is_numeric = false;
        strcpy(result->str_value, "LT");
    } else {
        result->is_numeric = false;
        strcpy(result->str_value, "EQ");
    }
}

/* Using the shared capture_stdout function from test_helpers.h */

/**
 * @brief Execute print_table with decimal format
 *
 * For use with capture_stdout
 */
static void execute_print_decimal(void)
{
    print_table(1, 3, mock_add, "Test Addition Table", FORMAT_DECIMAL);
}

/**
 * @brief Execute print_table with hexadecimal format
 *
 * For use with capture_stdout
 */
static void execute_print_hex(void)
{
    print_table(1, 3, mock_add, "Test Addition Table", FORMAT_HEX);
}

/**
 * @brief Execute print_table with string results
 *
 * For use with capture_stdout
 */
static void execute_print_string_results(void)
{
    print_table(1, 3, mock_string_result, "Test String Results", FORMAT_DECIMAL);
}

/**
 * @brief Test table printing with decimal format
 *
 * @return int Number of failed tests
 */
static int test_print_table_decimal(void)
{
    int failures = 0;
    char buffer[BUFFER_SIZE];

    /* Capture the output of print_table with decimal format */
    if (!capture_stdout(execute_print_decimal, buffer, BUFFER_SIZE)) {
        printf("  ERROR: Failed to capture stdout\n");
        return 1;
    }

    /* Check for expected table elements in the output */
    TEST_ASSERT(strstr(buffer, "Test Addition Table") != NULL,
                "Table title should be present in output", failures);

    TEST_ASSERT(strstr(buffer, "1 |") != NULL,
                "Row label '1' should be present in output", failures);

    TEST_ASSERT(strstr(buffer, "2 |") != NULL,
                "Row label '2' should be present in output", failures);

    TEST_ASSERT(strstr(buffer, "3 |") != NULL,
                "Row label '3' should be present in output", failures);

    /* Check that hexadecimal format indicator is NOT present */
    TEST_ASSERT(strstr(buffer, "[Hexadecimal Format]") == NULL,
                "Hexadecimal format indicator should not be present", failures);

    return failures;
}

/**
 * @brief Test table printing with hexadecimal format
 *
 * @return int Number of failed tests
 */
static int test_print_table_hex(void)
{
    int failures = 0;
    char buffer[BUFFER_SIZE];

    /* Capture the output of print_table with hexadecimal format */
    if (!capture_stdout(execute_print_hex, buffer, BUFFER_SIZE)) {
        printf("  ERROR: Failed to capture stdout\n");
        return 1;
    }

    /* Check for expected table elements in the output */
    TEST_ASSERT(strstr(buffer, "Test Addition Table") != NULL,
                "Table title should be present in output", failures);

    TEST_ASSERT(strstr(buffer, "[Hexadecimal Format]") != NULL,
                "Hexadecimal format indicator should be present", failures);

    /* Check for hex-formatted values */
    TEST_ASSERT(strstr(buffer, "0x") != NULL,
                "Hexadecimal values should be present (0x prefix)", failures);

    return failures;
}

/**
 * @brief Test table printing with string results
 *
 * @return int Number of failed tests
 */
static int test_print_table_string_results(void)
{
    int failures = 0;
    char buffer[BUFFER_SIZE];

    /* Capture the output of print_table with string results */
    if (!capture_stdout(execute_print_string_results, buffer, BUFFER_SIZE)) {
        printf("  ERROR: Failed to capture stdout\n");
        return 1;
    }

    /* Check for expected string results in the output */
    TEST_ASSERT(strstr(buffer, "EQ") != NULL,
                "String value 'EQ' should be present in output", failures);

    TEST_ASSERT(strstr(buffer, "GT") != NULL,
                "String value 'GT' should be present in output", failures);

    TEST_ASSERT(strstr(buffer, "LT") != NULL,
                "String value 'LT' should be present in output", failures);

    return failures;
}

/**
 * @brief Run all tests for the table formatter
 *
 * @return int Number of failed tests
 */
int run_table_formatter_tests(void)
{
    int failures = 0;

    RUN_TEST(test_print_table_decimal, failures);
    RUN_TEST(test_print_table_hex, failures);
    RUN_TEST(test_print_table_string_results, failures);

    return failures;
}