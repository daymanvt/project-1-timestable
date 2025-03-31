/**
 * @file table_operations.c
 * @brief Implementation of table cell operations
 *
 * Contains implementations of the various operations that can be
 * performed on table cells (multiplication, division, power).
 */

#include <string.h>
#include <math.h>
#include "timestable_operations.h"

/**
 * @brief String constant for division by zero
 */
#define UNDEF_STRING "UDF"

/**
 * @brief Multiplication operation (row × column)
 *
 * @param row Row value
 * @param column Column value
 * @param result Pointer to store the result
 */
void multiply(int row, int column, cell_value_t *result)
{
    result->is_numeric = true;
    result->num_value = row * column;
    result->str_value[0] = '\0';
}

/**
 * @brief Division operation (row ÷ column)
 *
 * Sets result to "n/a" for division by zero.
 *
 * @param row Row value (numerator)
 * @param column Column value (denominator)
 * @param result Pointer to store the result
 */
void
divide(int row, int column, cell_value_t *result)
{
    if (0 == column)
    {
        result->is_numeric = false;
        strcpy(result->str_value, UNDEF_STRING);
    }
    else
    {
        result->is_numeric    = true;
        result->num_value     = row / column;
        result->str_value[0]  = '\0';
    }
}

/**
 * @brief Power operation (row raised to column power)
 *
 * @param row Row value (base)
 * @param column Column value (exponent)
 * @param result Pointer to store the result
 */
void
power(int row, int column, cell_value_t *result)
{
    result->is_numeric      = true;
    result->num_value       = (int)pow(row, column);
    result->str_value[0]    = '\0';
}