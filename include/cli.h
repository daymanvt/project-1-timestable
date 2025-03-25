/**
 * @file cli.h
 * @brief Command line interface handling
 *
 * This file contains the structures and functions for parsing and
 * handling command line arguments.
 *
 * @author Your Name
 * @date March 24, 2025
 */

#ifndef CLI_H
#define CLI_H

#include <stdbool.h>
#include "table_formatter.h"

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
} CliErrorCode;

/**
 * @brief Table types to display
 */
typedef enum
{
    TABLE_MULTIPLICATION = 0x01,     /**< Show multiplication table */
    TABLE_DIVISION = 0x02,           /**< Show division table */
    TABLE_POWER = 0x04,              /**< Show power table */
    TABLE_ALL = 0x07                 /**< Show all tables */
} TableType;

/**
 * @brief Program options structure
 */
typedef struct
{
    int min_value;                   /**< Minimum value for rows and columns */
    int max_value;                   /**< Maximum value for rows and columns */
    OutputFormat format;             /**< Output format (decimal, hex, binary) */
    TableType tables;                /**< Tables to display */
    bool show_help;                  /**< Flag to show help message */
} ProgramOptions;

/**
 * @brief Initialize program options with default values
 *
 * @param options Pointer to options structure to initialize
 */
void cli_init_options(ProgramOptions *options);

/**
 * @brief Parse command line arguments into program options
 *
 * @param argc Argument count
 * @param argv Argument values
 * @param options Pointer to options structure to populate
 * @return CliErrorCode indicating success or specific error
 */
CliErrorCode cli_parse_args(int argc, char *argv[], ProgramOptions *options);

/**
 * @brief Get error message for a specific CLI error code
 *
 * @param error_code The error code to get message for
 * @return const char* The corresponding error message
 */
const char *cli_get_error_message(CliErrorCode error_code);

/**
 * @brief Print usage information for the program
 *
 * @param program_name Name of the executable
 */
void cli_print_usage(const char *program_name);

#endif /* CLI_H */