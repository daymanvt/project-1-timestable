/**
 * @file table_formatter.c
 * @brief Implementation of table formatting functions
 *
 * Functions for printing tables with various formatting options.
 *
 * @author Your Name
 * @date March 24, 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table_formatter.h"

/**
 * @brief Minimum cell width for output
 */
#define MIN_CELL_WIDTH 4

/**
 * @brief Cell padding for visual spacing
 */
#define CELL_PADDING 1

/**
 * @brief Calculate the required cell width for a value based on format
 *
 * @param value The numeric value to calculate width for
 * @param format The output format (decimal, hex, octal)
 * @return int The required width in characters
 */
static int calculate_numeric_width(int value, OutputFormat format)
{
    int width = 0;

    /* Account for sign only in decimal format */
    if ((format == FORMAT_DECIMAL) && (value < 0))
    {
        width = 1; /* For minus sign */
        value = abs(value);
    }

    /* Handle zero case */
    if (value == 0)
    {
        return width + 1;
    }

    /* Count digits based on the format */
    switch (format)
    {
        case FORMAT_DECIMAL:
            while (value > 0)
            {
                width++;
                value /= 10;
            }
            break;

        case FORMAT_HEX:
            width = 2; /* 0x prefix */
            while (value > 0)
            {
                width++;
                value /= 16;
            }
            break;

        case FORMAT_OCTAL:
            width = 2; /* 0 prefix */
            while (value > 0)
            {
                width++;
                value /= 8;
            }
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
static void print_cell(CellValue value, int width, OutputFormat format)
{
    if (value.is_numeric)
    {
        switch (format)
        {
            case FORMAT_DECIMAL:
                printf("%*d", width, value.num_value);
                break;

            case FORMAT_HEX:
                printf("%*s", width, "");
                printf("0x%x", value.num_value);
                break;

            case FORMAT_OCTAL:
                printf("%*s", width, "");
                printf("0%o", value.num_value);
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
 * @param format Output format to use (decimal, hex, octal)
 */
void print_table(int min_value, int max_value, TableOperation operation,
                 const char *title, OutputFormat format)
{
    int row;
    int column;
    int max_width;
    int width;
    /* Calculate the number of rows and columns in the table */
    /* (max_value - min_value + 1) represents the table size */

    printf("\n%s", title);

    if (format == FORMAT_HEX)
    {
        printf(" [Hexadecimal Format]\n");
    }
    else if (format == FORMAT_OCTAL)
    {
        printf(" [Octal Format]\n");
    }
    else
    {
        printf("\n");
    }

    /* Calculate maximum width needed for cell values */
    max_width = MIN_CELL_WIDTH;

    for (row = min_value; row <= max_value; row++)
    {
        for (column = min_value; column <= max_value; column++)
        {
            CellValue value;
            operation(row, column, &value);

            if (value.is_numeric)
            {
                width = calculate_numeric_width(value.num_value, format);
            }
            else
            {
                width = (int)strlen(value.str_value);
            }

            if (width > max_width)
            {
                max_width = width;
            }
        }
    }

    /* Add padding to the max_width */
    max_width += CELL_PADDING;

    /* Print header row */
    printf("%*s |", max_width, "");
    for (column = min_value; column <= max_value; column++)
    {
        switch (format)
        {
            case FORMAT_DECIMAL:
                printf("%*d", max_width, column);
                break;

            case FORMAT_HEX:
                printf("%*s", max_width, "");
                printf("0x%x", column);
                break;

            case FORMAT_OCTAL:
                printf("%*s", max_width, "");
                printf("0%o", column);
                break;
        }
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
        switch (format)
        {
            case FORMAT_DECIMAL:
                printf("%*d |", max_width, row);
                break;

            case FORMAT_HEX:
                printf("%*s", max_width - 2, "");
                printf("0x%x |", row);
                break;

            case FORMAT_OCTAL:
                printf("%*s", max_width - 1, "");
                printf("0%o |", row);
                break;
        }

        /* Print row data */
        for (column = min_value; column <= max_value; column++)
        {
            CellValue value;
            operation(row, column, &value);
            print_cell(value, max_width, format);
        }
        printf("\n");
    }
}