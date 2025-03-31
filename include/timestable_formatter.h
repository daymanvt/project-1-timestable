/**
 * @file timestable_formatter.h
 * @brief Table formatting and output functions
 *
 * Defines output formats and functions for printing formatted tables.
 */

#ifndef TIMESTABLE_FORMATTER_H
#define TIMESTABLE_FORMATTER_H

#include "timestable_operations.h"

/**
 * @brief Output formats for table values
 */
typedef enum
{
    FORMAT_DECIMAL = 0,    /**< Decimal (base 10) output */
    FORMAT_HEX         /**< Hexadecimal (base 16) output */
} output_format_t;

/**
 * @brief Print a formatted table using the specified operation
 *
 * @param min_value  Minimum value for rows and columns
 * @param max_value  Maximum value for rows and columns
 * @param operation  Function pointer to the operation to perform
 * @param title      Title to display for the table
 * @param format     Output format to use (decimal, hex)
 */
void print_table(int min_value, int max_value, TableOperation operation,
                 const char *title, output_format_t format);

#endif /* TIMESTABLE_FORMATTER_H */