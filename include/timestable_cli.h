/**
 * @file timestable_cli.h
 * @brief Command line interface handling
 *
 * This file contains the structures and functions for parsing and
 * handling command line arguments.
 */

#ifndef TIMESTABLE_CLI_H
#define TIMESTABLE_CLI_H

#include <stdbool.h>
#include "timestable_formatter.h"

/**
 * @brief Error codes for command line parsing and validation
 */
typedef enum
{
    CLI_SUCCESS = 0,                 /**< Command line parsed successfully */
    CLI_ERROR_INVALID_MIN,           /**< Invalid minimum value provided */
    CLI_ERROR_INVALID_MAX,           /**< Invalid maximum value provided */
    CLI_ERROR_MIN_GT_MAX,            /**< Minimum value greater than maximum */
    CLI_ERROR_INVALID_TABLE_TYPE,    /**< Invalid table type specified */
    CLI_ERROR_INVALID_OPTION         /**< Unknown or invalid option */
} cli_error_code_t;

/**
 * @brief Table types to display
 */
typedef enum
{
    TABLE_FLAG_MULTIPLICATION = 0x01,     /**< Show multiplication table */
    TABLE_FLAG_DIVISION = 0x02,           /**< Show division table */
    TABLE_FLAG_POWER = 0x04,              /**< Show power table */
    TABLE_FLAG_ALL = 0x07                 /**< Show all tables */
} table_flag_t;

/**
 * @brief Structure to hold error information
 */
typedef struct
{
    cli_error_code_t code;           /**< The error code */
    const char *message;             /**< The corresponding error message */
} cli_error_t;

/**
 * @brief Program options structure
 */
typedef struct
{
    int min_value;                   /**< Minimum value for rows and columns */
    int max_value;                   /**< Maximum value for rows and columns */
    output_format_t format;          /**< Output format (decimal, hex) */
    table_flag_t tables;             /**< Tables to display */
    bool show_help;                  /**< Flag to show help message */
} program_options_t;

/**
 * @brief Parse command line arguments into program options
 *
 * @param argc      Argument count
 * @param argv      Argument values
 * @param options   Pointer to options structure to populate
 * @return          cli_error_t structure with error code and message
 */
cli_error_t cli_parse_args(int argc, char *argv[], program_options_t *options);

/**
 * @brief Print usage information for the program
 *
 * @param program_name  Name of the executable
 */
void cli_print_usage(const char *program_name);

#endif /* TIMESTABLE_CLI_H */