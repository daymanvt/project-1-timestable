/**
 * @file timestable_main.c
 * @brief Main program for the multi-operation tables generator
 *
 * This program generates various operation tables (multiplication, division, power)
 * with configurable ranges and output formats.
 *
 * @author Your Name
 * @date March 24, 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <limits.h>
#include "table_operations.h"
#include "table_formatter.h"

/** Maximum allowed table size */
#define MAX_TABLE_SIZE 100

/**
 * @brief Print usage information for the program
 *
 * @param program_name Name of the executable
 */
static void print_usage(const char *program_name)
{
    printf("Usage: %s [options]\n", program_name);
    printf("Options:\n");
    printf("  -x           Display output in hexadecimal format\n");
    printf("  -o           Display output in octal format\n");
    printf("  -m <min>     Minimum value (default: 1, cannot be less than 0)\n");
    printf("  -M <max>     Maximum value (default: 10, cannot exceed %d)\n", MAX_TABLE_SIZE);
    printf("  -h           Display this help message\n");
}

/**
 * @brief Parse a string as a long integer with error checking
 *
 * @param str String to parse
 * @param result Pointer to store the result
 * @param min_val Minimum allowed value
 * @param max_val Maximum allowed value
 * @return bool true if parsing was successful, false otherwise
 */
static bool parse_integer(const char *str, int *result, int min_val, int max_val)
{
    char *endptr;
    long value;

    /* Reset errno before the call */
    errno = 0;

    /* Attempt to convert string to long integer */
    value = strtol(str, &endptr, 10);

    /* Check for conversion errors */
    if ((errno == ERANGE && (value == LONG_MAX || value == LONG_MIN)) ||
        (errno != 0 && value == 0) ||
        (endptr == str) ||
        (*endptr != '\0') ||
        (value < min_val) ||
        (value > max_val))
    {
        return false;
    }

    *result = (int)value;
    return true;
}

/**
 * @brief Main program entry point
 *
 * @param argc Argument count
 * @param argv Argument values
 * @return int Exit status
 */
int main(int argc, char *argv[])
{
    int min_value = 1;
    int max_value = 10;
    OutputFormat format = FORMAT_DECIMAL;
    int option;
    int temp_value;

    /* Parse command line options */
    while ((option = getopt(argc, argv, "xom:M:h")) != -1)
    {
        switch (option)
        {
            case 'x':
                format = FORMAT_HEX;
                break;

            case 'o':
                format = FORMAT_OCTAL;
                break;

            case 'm':
                if (!parse_integer(optarg, &temp_value, 0, INT_MAX)) //ignore
                {
                    fprintf(stderr, "Error: Invalid minimum value\n");
                    goto error_exit;
                }
                min_value = temp_value;
                break;

            case 'M':
                if (!parse_integer(optarg, &temp_value, 0, MAX_TABLE_SIZE))
                {
                    fprintf(stderr, "Error: Invalid maximum value (must be between 0 and %d)\n",
                            MAX_TABLE_SIZE);
                    goto error_exit;
                }
                max_value = temp_value;
                break;

            case 'h':
                print_usage(argv[0]);
                return EXIT_SUCCESS;

            default:
                print_usage(argv[0]);
                goto error_exit;
        }
    }

    /* Validate min/max values */
    if (min_value > max_value)
    {
        fprintf(stderr, "Error: Minimum value cannot be greater than maximum value\n");
        goto error_exit;
    }

    /* Print multiplication table */
    print_table(min_value, max_value, multiply, MULT_TABLE_TITLE, format);

    /* Print division table */
    // print_table(min_value, max_value, divide, DIV_TABLE_TITLE, format);

    /* Print power table */
    // print_table(min_value, max_value, power, POWER_TABLE_TITLE, format);

    return EXIT_SUCCESS;

error_exit:
    /* Clean up any resources if necessary */
    return EXIT_FAILURE;
}