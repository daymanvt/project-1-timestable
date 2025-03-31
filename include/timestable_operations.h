/**
 * @file timestable_operations.h
 * @brief Definitions for table cell operations
 *
 * This file contains the structures and function declarations for
 * different operations that can be applied to table cells.
 */

#ifndef TIMESTABLE_OPERATIONS_H
#define TIMESTABLE_OPERATIONS_H

#include <stdbool.h>

/**
 * @brief String constants for table operation titles
 */
#define MULT_TABLE_TITLE   "Multiplication Table (row × column)"
#define DIV_TABLE_TITLE    "Division Table (row ÷ column)"
#define POWER_TABLE_TITLE  "Power Table (row ^ column)"

/**
 * @brief Structure to hold cell value (either numeric or string)
 */
typedef struct
{
    bool is_numeric;             /**< Flag: true if numeric, false if string */
    int num_value;               /**< The numeric value (if applicable) */
    char str_value[8];           /**< The string value (if applicable) */
} cell_value_t;

/**
 * @brief Function pointer type for table operations
 *
 * @param row Row value
 * @param column Column value
 * @param result Pointer to store the result
 */
typedef void (*TableOperation)(int row, int column, cell_value_t *result);

/**
 * @brief Multiplication operation (row × column)
 *
 * @param row Row value
 * @param column Column value
 * @param result Pointer to store the result
 */
void multiply(int row, int column, cell_value_t *result);

/**
 * @brief Division operation (row ÷ column)
 *
 * Sets result to "n/a" for division by zero.
 *
 * @param row Row value (numerator)
 * @param column Column value (denominator)
 * @param result Pointer to store the result
 */
void divide(int row, int column, cell_value_t *result);

/**
 * @brief Power operation (row raised to column power)
 *
 * @param row Row value (base)
 * @param column Column value (exponent)
 * @param result Pointer to store the result
 */
void power(int row, int column, cell_value_t *result);

#endif /* TIMESTABLE_OPERATIONS_H */