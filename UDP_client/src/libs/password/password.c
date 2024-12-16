/**
 * @file password.c
 * @brief Implementation file providing functions for password validation.
 *
 * This file has the signature for functions to:
 * - Validate password type.
 * - Check password length validity.
 * - Control the continuation or termination of password generation.
 *
 * @version 1.0.0
 * @date 2024-12-15
 * @author Cristian Biallo
 */
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>
#include <string.h>
#include "password.h"


/* - - - - - - - - - - - - - - - - - PASSWORD CONTROLS - - - - - - - - - - - - - - - - - */

/**
 * @brief Determines whether password generation should continue based on the specified ending type.
 *
 * This function checks if the current password type differs from a specified ending type.
 * If the two types are identical (case insensitive), generation should stop.
 *
 * @param[in] type The current password type being processed.
 * @param[in] type_for_ending The type used to indicate the end of password generation.
 *
 * @return `true` if `type` is not equal to `type_for_ending`, indicating generation can continue.
 * @return `false` if `type` is equal to `type_for_ending`, signaling generation should stop.
 *
 * @pre `type` and `type_for_ending` must be valid characters.
 * @post The function returns a boolean value indicating whether generation should proceed.
 * @note The comparison is case insensitive.
 */
bool keep_generating(const char type, const char type_for_ending) {
    return tolower(type) != tolower(type_for_ending);
}


/**
 * @brief Checks whether the specified type is among the allowed types for password generation.
 *
 * This function verifies if a given type character is present in a string of allowed types.
 * The comparison is case sensitive.
 *
 * @param[in] allowed_type A null-terminated string containing all valid types.
 * @param[in] type The type to validate.
 *
 * @return `true` if `type` is found in `allowed_type`.
 * @return `false` if `type` is not found in `allowed_type`.
 *
 * @pre `allowed_type` must be a valid null-terminated string.
 * @pre `type` must be a valid character.
 * @post The function returns a boolean indicating whether the type is valid.
 * @warning Passing `NULL` as `allowed_type` results in undefined behavior.
 */
bool control_type(const char *allowed_type, const char type) {
    return strchr(allowed_type, type) != NULL;
}


/**
 * @brief Validates whether the password length is within the allowed range and is a positive integer.
 *
 * This function first checks that the input string `length` contains only numeric characters.
 * It then converts the string to an integer and validates that the integer is within the
 * range defined by `min_length` and `max_length`.
 *
 * @param[in] length A null-terminated string representing the requested password length.
 * @param[in] min_length The minimum allowable length for a password.
 * @param[in] max_length The maximum allowable length for a password.
 *
 * @return `true` if `length` is a numeric string representing a value within the range.
 * @return `false` if `length` is not numeric or falls outside the allowed range.
 *
 * @pre `length` must be a valid null-terminated string.
 * @pre `min_length` and `max_length` must be positive integers, and `min_length < max_length`.
 * @post The function returns a boolean indicating the validity of the password length.
 * @note Leading zeros in the `length` string are ignored during validation.
 * @warning Overflow is not handled explicitly when converting large strings to integers.
 */
bool control_length(const char *length, const int min_length, const int max_length) {
    // Check if all characters in `length` are digits
    for (int i = 0; length[i] != '\0'; i++) {
        if (!isdigit(length[i])) {
            return false;
        }
    }

    // Convert `length` to an integer and validate the range
    int numerical_length = atoi(length);
    return numerical_length >= min_length && numerical_length <= max_length;
}

/* - - - - - - - - - - - - - - - END PASSWORD CONTROLS - - - - - - - - - - - - - - - - */

