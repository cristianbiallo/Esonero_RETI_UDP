/**
 * @file utils.c
 * @brief Implementation of utility functions for printing colored text and displaying password menus.
 *
 * This file provides utility functions including:
 *  - ANSI color code management for terminal output.
 *  - Displaying password generator menus and help information.
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

/* - - - - - - - - - - - - - - - - - PASSWORD HELP MENU - - - - - - - - - - - - - - - - */

/**
 * @brief Displays the help menu for the password generator.
 *
 * This menu provides a detailed guide to available commands for password
 * generation, including the length and type options.
 */
void show_help_menu() {
	const char *help_text =
		"\nPassword Generator Help Menu\n"
		"Commands:\n"
		" h        : show this help menu\n"
		" n LENGTH : generate numeric password (digits only)\n"
		" a LENGTH : generate alphabetic password (lowercase letters)\n"
		" m LENGTH : generate mixed password (lowercase letters and numbers)\n"
		" s LENGTH : generate secure password (uppercase, lowercase, numbers, symbols)\n"
		" u LENGTH : generate unambiguous secure password (no similar-looking characters)\n"
		" q        : quit application\n\n"
		" LENGTH must be between 6 and 32 characters\n\n"
		" Ambiguous characters excluded in 'u' option:\n"
		" 0 O o (zero and letters O)\n"
		" 1 l I i (one and letters l, I)\n"
		" 2 Z z (two and letter Z)\n"
		" 5 S s (five and letter S)\n"
		" 8 B (eight and letter B)\n"
		"\nIf the length is absent, a default value is used: 8\n\n";
	print_with_color(help_text,CYAN);
}

/**
 * @brief Displays the password generator's main menu.
 *
 * This menu prompts the user to choose the type and length of the password
 * to generate, with explanations for each type.
 */
void show_password_menu() {
	const char *menu_text =
		"Insert the type of password and its length (between 6 and 32):\n"
		"  n: numeric password (only digits)\n"
		"  a: alphabetic password (only lowercase letters)\n"
		"  m: mixed password (lowercase letters and digits)\n"
		"  s: secure password (uppercase letters, lowercase letters, digits, and symbols)\n"
		"  u: unambiguous secure password (no similar-looking characters)\n"
		"  h: help menu\n"
		"  q: quit application\n"
		"? ";
	print_with_color(menu_text, YELLOW);
}

/* - - - - - - - - - - - END PASSWORD HELP MENU - - - - - - - - - - - */
