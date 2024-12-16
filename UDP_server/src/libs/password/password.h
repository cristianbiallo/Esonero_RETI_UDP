/**
 * @file password.h
 * @brief Header file providing functions to generate passwords
 *        based on specified criteria.
 * @version 1.0.0
 * @date 2024-12-15
 * @author Cristian Biallo
 */

#ifndef PASSWORD_H_
#define PASSWORD_H_

#include <stdbool.h>


/* - - - - - - - - - - - - - - - - - - - PASSWORD TYPES - - - - - - - - - - - - - - - - - */

/**
 * @enum PasswordType
 * @brief Enumerates the types of passwords that can be generated.
 *
 * This enumeration defines the different formats of passwords that the system can generate:
 * - `NUMERIC`: Generates a password consisting of numeric digits only (0-9).
 * - `ALPHA`: Generates a password using lowercase alphabetic characters (a-z).
 * - `MIXED`: Generates a password with a mix of lowercase alphabetic characters and numeric digits.
 * - `SECURE`: Generates a password using lowercase and uppercase alphabetic characters, digits, and symbols.
 * - `UNAMBIGUOUS`: Generates a secure password excluding ambiguous characters (e.g., O/0, l/1, etc.).
 */
typedef enum {
    NUMERIC,     /**< Numeric password: digits only (0-9) */
    ALPHA,       /**< Lowercase alphabetic password: letters only (a-z) */
    MIXED,       /**< Lowercase alphanumeric password: letters and digits */
    SECURE,      /**< Secure password: uppercase, lowercase, digits, and symbols */
    UNAMBIGUOUS  /**< Unambiguous secure password: excludes similar-looking characters */
} PasswordType;

/* - - - - - - - - - - - - - - - - - - END PASSWORD TYPES - - - - - - - - - - - - - - - - */

/* - - - - - - - - - - - - - - - - - - PASSWORD GENERATION - - - - - - - - - - - - - - - - - - */

/**
 * @brief Generates a password based on the specified type and length.
 *
 * This function creates a password using the specified `type` (NUMERIC, ALPHA, MIXED, SECURE, UNAMBIGUOUS)
 * and the desired length. The generated password is stored in the `password` array, which must
 * be pre-allocated by the caller. The `password` will be null-terminated.
 *
 * ### Password Type Details:
 * - **NUMERIC**: Only numeric digits (e.g., "837261").
 * - **ALPHA**: Only lowercase alphabetic characters (e.g., "qwerty").
 * - **MIXED**: Combination of lowercase alphabetic characters and digits (e.g., "abc123").
 * - **SECURE**: Combination of lowercase/uppercase letters, digits, and symbols (e.g., "Pa$sW0rd!").
 * - **UNAMBIGUOUS**: Similar to SECURE but excludes ambiguous characters like `O`, `0`, `l`, `1` (e.g., "Tg@8%Yk").
 *
 * @param[out] password A pre-allocated array to store the generated password.
 *                      Must have space for at least `length + 1` characters
 *                      (to account for the null terminator).
 * @param[in] type The type of password to generate, as defined in the `PasswordType` enum.
 * @param[in] length The desired length of the generated password (must be > 0).
 *
 * @pre `password` must be a valid pointer to a pre-allocated array.
 * @pre `length` must be greater than zero and less than or equal to the maximum supported length.
 * @post The `password` array will be populated with a null-terminated password of the specified type and length.
 * @note The behavior is undefined if `password` is not allocated or `length` is invalid.
 */
void generate_password(char *password, PasswordType type, int length);

/* - - - - - - - - - - - - - - - - - END PASSWORD GENERATION - - - - - - - - - - - - - - - - - */

#endif /* PASSWORD_H_ */
