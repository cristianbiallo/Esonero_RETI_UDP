/**
 * @file password.h
 * @brief Header file providing functions for password validation.
 *
 * This file defines functions to:
 * - Validate password type.
 * - Check password length validity.
 * - Control the continuation or termination of password generation.
 *
 * @version 1.0.0
 * @date 2024-12-15
 * @author Cristian Biallo
 */

#ifndef PASSWORD_H_
#define PASSWORD_H_

#include <stdbool.h>

/* - - - - - - - - - - - - - - - - - - PASSWORD CONTROLS - - - - - - - - - - - - - - - - - - */

/**
 * @brief Determines whether password generation should continue.
 *
 * Compares the current password type (`type`) to a termination type (`type_for_ending`).
 * Password generation continues only if they differ.
 *
 * @param[in] type The current type of password being processed.
 * @param[in] type_for_ending The type signaling termination of generation.
 * @pre `type` and `type_for_ending` should be valid single characters.
 * @return `true` if `type` is different from `type_for_ending`, allowing generation to continue.
 * @return `false` if `type` matches `type_for_ending`, signaling the end of generation.
 */
bool keep_generating(const char type, const char type_for_ending);


/**
 * @brief Validates the requested password type against allowed values.
 *
 * Ensures that the `type` is among a predefined set of allowed characters.
 * Examples of valid types might include numeric ('n'), alphabetic ('a'), mixed ('m'), etc.
 *
 * @param[in] allowed_type A string containing valid password types (e.g., "nams").
 * @param[in] type The specific password type to validate.
 * @pre `allowed_type` should not be NULL and must be null-terminated.
 * @pre `type` should be a single character.
 * @return `true` if `type` is present in `allowed_type`.
 * @return `false` if `type` is not in `allowed_type`.
 */
bool control_type(const char *allowed_type, const char type);


/**
 * @brief Verifies if the requested password length is valid.
 *
 * Ensures the `length` string represents a positive integer within the specified range
 * (`[min_length, max_length]`). Handles invalid or non-numeric input gracefully.
 *
 * @param[in] length A string representing the desired password length.
 * @param[in] min_length The minimum allowable password length.
 * @param[in] max_length The maximum allowable password length.
 * @pre `length` should be a null-terminated numeric string.
 * @return `true` if `length` is numeric and within the valid range.
 * @return `false` if `length` is invalid, negative, or out of range.
 */
bool control_length(const char *length, const int min_length, const int max_length);

/* - - - - - - - - - - - - - - - - - END PASSWORD CONTROLS - - - - - - - - - - - - - - - - - */

#endif /* PASSWORD_H_ */
