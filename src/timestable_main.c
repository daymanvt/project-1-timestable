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
#include "table_operations.h"
#include "table_formatter.h"
#include "cli.h"

/**
 * @brief Main program entry point
 *
 * @param argc Argument count
 * @param argv Argument values
 * @return int Exit status
 */
int main(int argc, char *argv[])
{
    ProgramOptions options;
    CliErrorCode error_code;

    /* Initialize options with default values */
    cli_init_options(&options);

    /* Parse command line arguments */
    error_code = cli_parse_args(argc, argv, &options);

    /* Check for errors or help request */
    if (error_code != CLI_SUCCESS)
    {
        fprintf(stderr, "Error: %s\n", cli_get_error_message(error_code));
        cli_print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    if (options.show_help)
    {
        cli_print_usage(argv[0]);
        return EXIT_SUCCESS;
    }

    /* Display requested tables */
    if (options.tables & TABLE_MULTIPLICATION)
    {
        print_table(options.min_value, options.max_value, multiply,
                   MULT_TABLE_TITLE, options.format);
    }

    if (options.tables & TABLE_DIVISION)
    {
        print_table(options.min_value, options.max_value, divide,
                   DIV_TABLE_TITLE, options.format);
    }

    if (options.tables & TABLE_POWER)
    {
        print_table(options.min_value, options.max_value, power,
                   POWER_TABLE_TITLE, options.format);
    }

    return EXIT_SUCCESS;
}