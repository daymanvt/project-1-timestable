/**
 * @file table_formatter.h
 * @brief Table formatting and output functions
 *
 * Defines output formats and functions for printing formatted tables.
 *
 * @author Your Name
 * @date March 24, 2025
 */

#ifndef TABLE_FORMATTER_H
#define TABLE_FORMATTER_H

#include "table_operations.h"

/**
 * @brief Output formats for table values
 */
typedef enum
{
    FORMAT_DECIMAL,    /**< Decimal (base 10) output */
    FORMAT_HEX,        /**< Hexadecimal (base 16) output */
    FORMAT_OCTAL       /**< Octal (base 8) output */
} OutputFormat;

/**
 * @brief Print a formatted table using the specified operation
 *
 * @param min_value Minimum value for rows and columns
 * @param max_value Maximum value for rows and columns
 * @param operation Function pointer to the operation to perform
 * @param title Title to display for the table
 * @param format Output format to use (decimal, hex, octal)
 */
void print_table(int min_value, int max_value, TableOperation operation,
                 const char *title, OutputFormat format);

#endif /* TABLE_FORMATTER_H */