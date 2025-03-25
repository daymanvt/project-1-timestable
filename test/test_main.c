/**
 * @file test_main.c
 * @brief Main test runner for the timestable project
 *
 * Orchestrates the execution of all test suites.
 *
 * @author Claude
 * @date March 25, 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "test_framework.h"
#include "test_table_operations.h"
#include "test_table_formatter.h"
#include "test_cli.h"

/**
 * @brief Main entry point for test execution
 *
 * @return int EXIT_SUCCESS if all tests pass, EXIT_FAILURE otherwise
 */
int main(void)
{
    int failed_tests = 0;
    TestSuite suites[] = {
        {"Table Operations", run_table_operations_tests},
        {"Table Formatter", run_table_formatter_tests},
        {"Command Line Interface", run_cli_tests}
    };
    int num_suites = sizeof(suites) / sizeof(suites[0]);

    printf("===== TIMESTABLE TEST SUITE =====\n\n");

    for (int i = 0; i < num_suites; i++)
    {
        printf("Running %s tests...\n", suites[i].name);
        int suite_failures = suites[i].run_tests();
        if (suite_failures > 0)
        {
            printf("  %d tests FAILED in %s suite\n", suite_failures, suites[i].name);
            failed_tests += suite_failures;
        }
        else
        {
            printf("  All tests PASSED in %s suite\n", suites[i].name);
        }
        printf("\n");
    }

    printf("===== TEST SUMMARY =====\n");
    if (failed_tests > 0)
    {
        printf("FAILED: %d test(s) failed\n", failed_tests);
        return EXIT_FAILURE;
    }
    else
    {
        printf("SUCCESS: All tests passed\n");
        return EXIT_SUCCESS;
    }
}