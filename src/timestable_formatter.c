/**
 * @file table_formatter.c
 * @brief Implementation of table formatting functions
 *
 * Functions for printing tables with various formatting options.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "timestable_formatter.h"

#define HEX_ZERO_WIDTH 3
#define DECIMAL_ZERO_WIDTH 1
#define MIN_CELL_WIDTH 4
#define CELL_PADDING 1

/**
 * @brief Calculate the required cell width for a value based on format
 *
 * @param value   The maximum numeric value to calculate width for
 * @param format  The output format (decimal, hex)
 * @return int    The required width in characters
 */
static
int calculate_numeric_width(int value, output_format_t format)
{
    int width = 0;

    switch (format) {
        case FORMAT_DECIMAL:
            do {
                width++;
                value /= 10;
            } while (value > 0 || 0 == width); //Handle zero case within the loop.
            break;

        case FORMAT_HEX:
            width = 2; // "0x" prefix
            do {
                width++;
                value /= 16;
            } while (value > 0 || 2 == width); //Handle zero case within the loop.
            break;
    }

    return width;
}

/**
 * @brief Format and print a cell value according to the specified format
 *
 * @param value Cell value to print
 * @param width Width for formatting
 * @param format Output format to use
 */
static void
print_cell(cell_value_t value, int width, output_format_t format)
{
    if (value.is_numeric)
    {
        char buffer[32]; /* Buffer large enough for any integer representation */

        switch (format)
        {
            case FORMAT_DECIMAL:
                sprintf(buffer, "%d", value.num_value);
                printf("%*s", width, buffer);
            break;

            case FORMAT_HEX:
                sprintf(buffer, "0x%x", value.num_value);
                printf("%*s", width, buffer);
            break;
        }
    }
    else
    {
        printf("%*s", width, value.str_value);
    }
}

/**
 * @brief Print a formatted table using the specified operation
 *
 * @param min_value Minimum value for rows and columns
 * @param max_value Maximum value for rows and columns
 * @param operation Function pointer to the operation to perform
 * @param title Title to display for the table
 * @param format Output format to use (decimal, hex)
 */
void
print_table(int min_value,
            int max_value,
            TableOperation operation,
            const char *title,
            output_format_t format)
{
    int row;
    int column;
    int max_width;

    printf("\n%s\n", title);

    /* Calculate maximum width needed based on largest possible value */
    int largest_possible = max_value * max_value; /* Largest value from multiplication */

    /* For extra safety in case of larger operations (like power) */
    if (strcmp(title, POWER_TABLE_TITLE) == 0 && max_value > 0)
    {
        /* For powers, the largest value could be max_value^max_value
           But that would be huge, so let's use a reasonable estimate */
        int max_exponent = (max_value < 8) ? max_value : 8; /* Choose smaller of max_value or 8 */
        largest_possible = (int)pow(max_value, max_exponent);
    }

    max_width = calculate_numeric_width(largest_possible, format);

    /* Ensure we meet minimum width requirement */
    if (max_width < MIN_CELL_WIDTH)
        max_width = MIN_CELL_WIDTH;

    /* Add padding */
    max_width += CELL_PADDING;

    /* Print header row */
    printf("%*s |", max_width, "");
    for (column = min_value; column <= max_value; column++)
    {
        cell_value_t header;
        header.is_numeric = true;
        header.num_value = column;
        print_cell(header, max_width, format);
    }
    printf("\n");

    /* Print separator line */
    for (column = 0; column < max_width; column++)
    {
        printf("-");
    }
    printf("-+");

    for (column = min_value; column <= max_value; column++)
    {
        for (row = 0; row < max_width; row++)
        {
            printf("-");
        }
    }
    printf("\n");

    /* Print table body */
    for (row = min_value; row <= max_value; row++)
    {
        /* Print row label */
        cell_value_t label;
        label.is_numeric = true;
        label.num_value = row;
        print_cell(label, max_width, format);
        printf(" |");

        /* Print row data */
        for (column = min_value; column <= max_value; column++)
        {
            cell_value_t value;
            operation(row, column, &value);
            print_cell(value, max_width, format);
        }
        printf("\n");
    }
}