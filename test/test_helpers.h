/**
 * @file test_helpers.h
 * @brief Helper functions for testing
 *
 * Provides utility functions to assist with testing.
 *
 * @author Claude
 * @date March 25, 2025
 */

#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief Captures stdout during function execution
 *
 * Redirects stdout to a pipe, executes the provided function,
 * and captures the output in the provided buffer.
 *
 * @param func Function to execute with captured stdout
 * @param buffer Buffer to store captured output
 * @param buffer_size Size of the buffer
 * @return bool true on success, false on error
 */
bool capture_stdout(void (*func)(void), char *buffer, size_t buffer_size);

#endif /* TEST_HELPERS_H */