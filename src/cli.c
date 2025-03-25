/**
 * @file cli.c
 * @brief Implementation of command line interface handling
 *
 * Functions for parsing and processing command line arguments.
 *
 * @author Your Name
 * @date March 24, 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <getopt.h>
#include "cli.h"

/** Maximum allowed table size */
#define MAX_TABLE_SIZE 100

/**
 * @brief Parse a string as an integer with error checking
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
 * @brief Initialize program options with default values
 *
 * @param options Pointer to options structure to initialize
 */
void cli_init_options(ProgramOptions *options)
{
    options->min_value = 1;
    options->max_value = 10;
    options->format = FORMAT_DECIMAL;
    options->tables = TABLE_MULTIPLICATION;
    options->show_help = false;
}

/**
 * @brief Parse command line arguments into program options
 *
 * @param argc Argument count
 * @param argv Argument values
 * @param options Pointer to options structure to populate
 * @return CliErrorCode indicating success or specific error
 */
CliErrorCode cli_parse_args(int argc, char *argv[], ProgramOptions *options)
{
    int option;
    int temp_value;

    /* Parse command line options */
    while ((option = getopt(argc, argv, "xm:M:t:h")) != -1)
    {
        switch (option)
        {
            case 'x':
                options->format = FORMAT_HEX;
                break;

            case 'm':
                if (!parse_integer(optarg, &temp_value, 0, INT_MAX))
                {
                    return CLI_ERROR_INVALID_MIN;
                }
                options->min_value = temp_value;
                break;

            case 'M':
                if (!parse_integer(optarg, &temp_value, 0, MAX_TABLE_SIZE))
                {
                    return CLI_ERROR_INVALID_MAX;
                }
                options->max_value = temp_value;
                break;

            case 't':
                if (strcmp(optarg, "m") == 0)
                {
                    options->tables = TABLE_MULTIPLICATION;
                }
                else if (strcmp(optarg, "d") == 0)
                {
                    options->tables = TABLE_DIVISION;
                }
                else if (strcmp(optarg, "p") == 0)
                {
                    options->tables = TABLE_POWER;
                }
                else if (strcmp(optarg, "a") == 0)
                {
                    options->tables = TABLE_ALL;
                }
                else
                {
                    return CLI_ERROR_INVALID_TABLE_TYPE;
                }
                break;

            case 'h':
                options->show_help = true;
                return CLI_SUCCESS;

            default:
                return CLI_ERROR_INVALID_OPTION;
        }
    }

    /* Validate min/max values */
    if (options->min_value > options->max_value)
    {
        return CLI_ERROR_MIN_GT_MAX;
    }

    return CLI_SUCCESS;
}

/**
 * @brief Get error message for a specific CLI error code
 *
 * @param error_code The error code to get message for
 * @return const char* The corresponding error message
 */
const char *cli_get_error_message(CliErrorCode error_code)
{
    switch (error_code)
    {
        case CLI_SUCCESS:
            return "Success";
        case CLI_ERROR_INVALID_MIN:
            return "Invalid minimum value";
        case CLI_ERROR_INVALID_MAX:
            return "Invalid maximum value (must be between 0 and 100)";
        case CLI_ERROR_MIN_GT_MAX:
            return "Minimum value cannot be greater than maximum value";
        case CLI_ERROR_INVALID_TABLE_TYPE:
            return "Invalid table type (use m, d, p, or a)";
        case CLI_ERROR_INVALID_OPTION:
            return "Unknown or invalid option";
        default:
            return "Unknown error";
    }
}

/**
 * @brief Print usage information for the program
 *
 * @param program_name Name of the executable
 */
void cli_print_usage(const char *program_name)
{
    printf("Usage: %s [options]\n", program_name);
    printf("Options:\n");
    printf("  -x           Display output in hexadecimal format\n");
    printf("  -m <min>     Minimum value (default: 1, cannot be less than 0)\n");
    printf("  -M <max>     Maximum value (default: 10, cannot exceed %d)\n", MAX_TABLE_SIZE);
    printf("  -t <type>    Table type (m=multiplication, d=division, p=power, a=all)\n");
    printf("  -h           Display this help message\n");
}