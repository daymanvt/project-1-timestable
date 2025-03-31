/**
 * @file test_cli.c
 * @brief Implementation of tests for the command line interface
 *
 * Tests for command line argument parsing and option handling.
 *
 * @author Claude
 * @date March 25, 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test_framework.h"
#include "test_cli.h"
#include "timestable_cli.h"

/**
 * @brief Test initialization of program options
 *
 * @return int Number of failed tests
 */
static int test_cli_init_options(void)
{
    int failures = 0;
    program_options_t options;

    /* Initialize options and check default values */
    cli_init_options(&options);

    TEST_ASSERT(options.min_value == 1, "Default min_value should be 1", failures);
    TEST_ASSERT(options.max_value == 10, "Default max_value should be 10", failures);
    TEST_ASSERT(options.format == FORMAT_DECIMAL, "Default format should be decimal", failures);
    TEST_ASSERT(options.tables == TABLE_MULTIPLICATION, "Default table should be multiplication", failures);
    TEST_ASSERT(options.show_help == false, "Default show_help should be false", failures);

    return failures;
}

/**
 * @brief Test error message functionality
 *
 * @return int Number of failed tests
 */
static int test_cli_error_messages(void)
{
    int failures = 0;

    /* Test that each error code has a corresponding message */
    TEST_ASSERT(cli_get_error_message(CLI_SUCCESS) != NULL, "Success should have a message", failures);
    TEST_ASSERT(cli_get_error_message(CLI_ERROR_INVALID_MIN) != NULL, "Invalid min should have a message", failures);
    TEST_ASSERT(cli_get_error_message(CLI_ERROR_INVALID_MAX) != NULL, "Invalid max should have a message", failures);
    TEST_ASSERT(cli_get_error_message(CLI_ERROR_MIN_GT_MAX) != NULL, "Min > max should have a message", failures);
    TEST_ASSERT(cli_get_error_message(CLI_ERROR_INVALID_TABLE_TYPE) != NULL, "Invalid table type should have a message", failures);
    TEST_ASSERT(cli_get_error_message(CLI_ERROR_INVALID_OPTION) != NULL, "Invalid option should have a message", failures);

    /* Test that unknown error codes have a default message */
    TEST_ASSERT(cli_get_error_message(999) != NULL, "Unknown error should have a default message", failures);

    return failures;
}

/**
 * @brief Run all tests for the CLI functions
 *
 * @return int Number of failed tests
 */
int run_cli_tests(void)
{
    int failures = 0;

    RUN_TEST(test_cli_init_options, failures);
    RUN_TEST(test_cli_error_messages, failures);

    return failures;
}