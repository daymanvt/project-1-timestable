/**
 * @file timestable_cli.c
 * @brief Implementation of command line interface handling
 *
 * Functions for parsing and processing command line arguments.
 */

#include <stdio.h>                  // printf
#include <stdlib.h>                 // EXIT_FAILURE, EXIT_SUCCESS
#include <string.h>                 // strcmp
#include <errno.h>                  // errno
#include <limits.h>                 // INT_MAX
#include <unistd.h>                 // getopt
#include <getopt.h>                 // getopt
#include <stdbool.h>

#include "colors.h"                 // RED, GRN, YLW, BLU, MAG, CYN, CLR
#include "timestable_cli.h"

#define MAX_TABLE_SIZE 100

static const cli_error_t CLI_ERRORS[] = {
    {CLI_SUCCESS,                   "Success"},
    {CLI_ERROR_INVALID_MIN,         "Invalid minimum value"},
    {CLI_ERROR_INVALID_MAX,         "Invalid maximum value (must be between 0 and 100)"},
    {CLI_ERROR_MIN_GT_MAX,          "Minimum value cannot be greater than maximum value"},
    {CLI_ERROR_INVALID_TABLE_TYPE,  "Invalid table type (use m, d, p, or a)"},
    {CLI_ERROR_INVALID_OPTION,      "Unknown or invalid option"}
};

static const size_t CLI_ERRORS_COUNT = sizeof(CLI_ERRORS) / sizeof(CLI_ERRORS[0]);

/**
 * @brief Parse a string as an integer with error checking
 *
 * @param str       String to parse
 * @param result    Pointer to store the result
 * @param min_val   Minimum allowed value
 * @param max_val   Maximum allowed value
 * @return          bool true if parsing was successful, false otherwise
 */
static
bool parse_integer(const char *str, int *result, int min, int max)
{
    char *endptr    = NULL;
    long value      = 0;
    errno           = 0;

    /* Attempt to convert string to long integer */
    value = strtol(str, &endptr, 10);

    /* Conversion check variables - separated for clarity */
    bool overflow_or_underflow  = ERANGE == errno && (LONG_MAX == value || LONG_MIN == value);
    bool other_error_with_zero  = errno != 0 && value == 0;
    bool no_digits_found        = endptr == str;
    bool has_trailing_chars     = *endptr != '\0';
    bool out_of_range           = value < min || value > max;

    /* Check for conversion errors */
    if (overflow_or_underflow || other_error_with_zero ||
        no_digits_found || has_trailing_chars || out_of_range)
    {
        return false;
    }

    /* Update the result pointer */
    *result = (int)value;
    return true;
}

/**
 * @brief Parse command line arguments into program options
 *
 * @param argc      Argument count
 * @param argv      Argument values
 * @param options   Pointer to options structure to populate
 * @return          cli_error_code_t indicating success or specific error
 */
cli_error_t
cli_parse_args(int argc, char *argv[], program_options_t *options)
{
    char option                 = '\0';
    int temp_value              = 0;
    cli_error_code_t error_code = CLI_SUCCESS;

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
                    error_code = CLI_ERROR_INVALID_MIN;
                    goto exit_function;
                }
                options->min_value = temp_value;
            break;

            case 'M':
                if (!parse_integer(optarg, &temp_value, 0, MAX_TABLE_SIZE))
                {
                    error_code = CLI_ERROR_INVALID_MAX;
                    goto exit_function;
                }
                options->max_value = temp_value;
            break;

            case 't':
                /* Process table type option */
                if (strlen(optarg) != 1)
                {
                    error_code = CLI_ERROR_INVALID_TABLE_TYPE;
                    goto exit_function;
                }

                switch(optarg[0])
                {
                    case 'm':
                        options->tables = TABLE_FLAG_MULTIPLICATION;
                    break;

                    case 'd':
                        options->tables = TABLE_FLAG_DIVISION;
                    break;

                    case 'p':
                        options->tables = TABLE_FLAG_POWER;
                    break;

                    case 'a':
                        options->tables = TABLE_FLAG_ALL;
                    break;

                    default:
                        error_code = CLI_ERROR_INVALID_TABLE_TYPE;
                        goto exit_function;
                }
            break;

            case 'h':
                options->show_help = true;
                goto exit_function;

            default:
                error_code = CLI_ERROR_INVALID_OPTION;
                goto exit_function;
        }
    }

    /* Validate min/max values */
    if (options->min_value > options->max_value)
    {
        error_code = CLI_ERROR_MIN_GT_MAX;
    }

exit_function:
    /* Instead of returning just the error code, return the full error structure */
    for (size_t i = 0; i < CLI_ERRORS_COUNT; i++)
    {
        if (error_code == CLI_ERRORS[i].code)
        {
            return CLI_ERRORS[i];
        }
    }

    /* Default error if somehow we get an unknown code */
    return (cli_error_t){.code = error_code, .message = "Unknown error"};
}

/**
 * @brief Print usage information for the program
 *
 * @param program_name Name of the executable
 */
void
cli_print_usage(const char *program_name)
{
    printf(GRN "Usage: %s [options]\n", program_name);
    printf(YLW "Options:\n");
    printf(YLW "  -x           Display output in hexadecimal format\n");
    printf(YLW "  -m <min>     Minimum value (default: 1, cannot be less than 0)\n");
    printf(YLW "  -M <max>     Maximum value (default: 10, cannot exceed %d)\n", MAX_TABLE_SIZE);
    printf(YLW "  -t <type>    Table type (m=multiplication, d=division, p=power, a=all)\n");
    printf(YLW "  -h           Display this help message\n");
    printf(CLR);
}