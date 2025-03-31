/**
 * @file timestable_main.c
 * @brief Main program for the multi-operation tables generator
 *
 * This program generates various operation tables (multiplication, division, power)
 * with configurable ranges and output formats.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "timestable_operations.h"  //*_TITLE, multiply, divide, power
#include "timestable_formatter.h"   // print_table
#include "timestable_cli.h"         // cli_error_code_t, program_options_t, cli_parse_args, cli_get_error_message, cli_print_usage
#include "colors.h"                 // RED, GRN, YLW, BLU, MAG, CYN, CLR

#define DEFAULT_MIN_VALUE 1
#define DEFAULT_MAX_VALUE 10

/**
 * @brief Main program entry point
 *
 * @param  argc  Argument count
 * @param  argv  Argument values
 * @return       int Exit status
 */
int main(int argc, char *argv[])
{
    program_options_t options = {
        .min_value  = DEFAULT_MIN_VALUE,
        .max_value  = DEFAULT_MAX_VALUE,
        .format     = FORMAT_DECIMAL,
        .tables     = TABLE_FLAG_MULTIPLICATION,
        .show_help  = false
    };

    cli_error_t error;

    /* Parse command line arguments */
    error = cli_parse_args(argc, argv, &options);

    /* Check for errors or help request */
    if (error.code != CLI_SUCCESS)
    {
        fprintf(stderr, RED "Error: %s\n", error.message);
        cli_print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    if (options.show_help)
    {
        cli_print_usage(argv[0]);
        return EXIT_SUCCESS;
    }

    /* Display requested tables */
    if (options.tables & TABLE_FLAG_MULTIPLICATION)
    {
        print_table(options.min_value, options.max_value, multiply,
                   MULT_TABLE_TITLE, options.format);
    }

    if (options.tables & TABLE_FLAG_DIVISION)
    {
        print_table(options.min_value, options.max_value, divide,
                   DIV_TABLE_TITLE, options.format);
    }

    if (options.tables & TABLE_FLAG_POWER)
    {
        print_table(options.min_value, options.max_value, power,
                   POWER_TABLE_TITLE, options.format);
    }

    return EXIT_SUCCESS;
}