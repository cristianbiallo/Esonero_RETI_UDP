/**
 * @file utils.h
 * @brief Provides utility functions for colored text output and user menu display.
 *
 * This header file declares functions and enumerations that assist in printing
 * colored text to the terminal and displaying password generator menus.
 *
 * @date 2024-12-15
 * @author Cristian Biallo
 * @version 1.1.0
 */

#ifndef UTILS_H_
#define UTILS_H_

/* - - - - - - - - - - - - - - - - - COLORS - - - - - - - - - - - - - - - - - */

/**
 * @enum textColor
 * @brief Defines colors for text output.
 *
 * Enumerates ANSI color codes to specify text colors in terminal output.
 */
typedef enum {
    BLACK,      /**< Black text color */
    RED,        /**< Red text color */
    GREEN,      /**< Green text color */
    YELLOW,     /**< Yellow text color */
    BLUE,       /**< Blue text color */
    MAGENTA,    /**< Magenta text color */
    CYAN,       /**< Cyan text color */
    WHITE,      /**< White text color */
    RESET       /**< Resets to default text color */
} textColor;


/**
 * @brief Prints the specified text in the specified color.
 *
 * This function prints a string with the ANSI color specified by the color
 * parameter. After printing, the text color is reset to default.
 *
 * @param[in] text Pointer to the string to print. Should be null-terminated.
 * @param[in] color The textColor to apply to the text.
 *
 * @pre `text` must be a valid, non-null pointer.
 * @post The text is printed to the console in the specified color.
 */
void print_with_color(const char *text, textColor color);

/* - - - - - - - - - - - - - - - - END COLORS - - - - - - - - - - - - - - - - */

/**
 * @brief Displays the help menu for the password generator.
 *
 * This function prints a detailed help menu that explains the available options
 * and commands for generating passwords.
 *
 * @post A help menu is displayed in the terminal.
 */
void show_help_menu();

/**
 * @brief Displays the password type and length selection menu.
 *
 * This function prints a menu prompting the user to choose a password type
 * and specify its length. The menu provides detailed descriptions for each option.
 *
 * @post A menu is displayed in the terminal, prompting user input.
 */
void show_password_menu();

#endif /* UTILS_H_ */
