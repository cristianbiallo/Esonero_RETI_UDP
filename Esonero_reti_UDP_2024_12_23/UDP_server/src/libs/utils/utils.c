/**
 * @file utils.c
 * @brief Implementation of utility functions for printing colored text.
 *
 * This file provides utility functions including:
 *  - ANSI color code management for terminal output.
 *
 * @version 1.1.0
 * @date 2024-12-15
 * @author Cristian Biallo
 */

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "utils.h"

/* - - - - - - - - - - - - - - - - - COLORS - - - - - - - - - - - - - - - - - */

/**
 * @brief Returns the ANSI escape code for the specified color.
 *
 * This function maps a `textColor` enumeration value to the corresponding
 * ANSI escape code for terminal color output.
 *
 * @param[in] color The color to use, as specified in the `textColor` enum.
 * @return A string containing the ANSI escape code for the color.
 *         Defaults to `RESET` if the input is invalid.
 */
const char *generate_ansi_color_code(textColor color) {
    switch(color) {
		case BLACK: 	return "\033[30m";
		case RED:		return "\033[31m";
		case GREEN:		return "\033[32m";
		case YELLOW:	return "\033[33m";
		case BLUE:		return "\033[34m";
		case MAGENTA:	return "\033[35m";
		case CYAN: 		return "\033[36m";
		case WHITE: 	return "\033[37m";
		case RESET: 	return "\033[0m";
		default: 		return "\033[0m";
	}
}

/**
 * @brief Prints a string in the specified color.
 *
 * This function wraps the provided text in ANSI escape codes for the desired
 * color and prints it to the terminal. The color is reset to default afterward.
 *
 * @param[in] text The text to be printed. Must be a valid null-terminated string.
 * @param[in] color The color to apply, as specified in the `textColor` enum.
 *
 * @pre `text` should not be NULL.
 * @post The text is displayed in the terminal with the specified color.
 */
void print_with_color(const char *text, textColor color) {
	if (text == NULL) return;
    printf("%s%s%s", generate_ansi_color_code(color), text, generate_ansi_color_code(RESET));
}

/* - - - - - - - - - - - - - - - - END COLORS - - - - - - - - - - - - - - - - */
