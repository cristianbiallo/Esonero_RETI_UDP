/**
 * @file protocol.h
 * @brief Header file used to define constants, structs, and protocol-specific
 * data structures to support the client communication in the `UDP_client.c` file.
 *
 * This file centralizes the communication parameters, such as buffer size,
 * password constraints, and data structures for request-response handling.
 *
 * @version 1.0.1
 * @date 2024-12-15
 * @author Cristian Biallo
 */

#ifndef PROTOCOL_H
#define PROTOCOL_H

/* - - - - - - - - - - - - - - - - - - - CONSTANTS - - - - - - - - - - - - - - - - */

/**
 * @brief Defines the maximum size of the buffer used for communication.
 *
 * The buffer serves multiple purposes, such as:
 * - Storing server names.
 * - Handling password lengths during requests.
 * - General communication needs between client and server.
 *
 * Using a large buffer size ensures flexibility for various operations,
 * while avoiding memory overflow risks.
 */
#define BUFFER_SIZE 1024        /**< Maximum dimension of the buffer */


/**
 * @brief Maximum allowable length for a generated password.
 *
 * Passwords longer than this value will not be accepted by the client or server.
 * This constant ensures compatibility and usability across different systems.
 */
#define MAX_PASSWORD_LENGTH 32  /**< Maximum password length */

/**
 * @brief Minimum allowable length for a generated password.
 *
 * Passwords shorter than this value are considered insecure and will be rejected.
 */
#define MIN_PASSWORD_LENGTH 6  /**< Minimum allowed password length */


/**
 * @brief Default port number used for client-server communication.
 *
 * The client will connect to the server using this port unless specified otherwise.
 * The default value of 8080 is commonly used for development and testing purposes.
 */
#define DEFAULT_PORT 8080       /**< Default port number for communication */

/* - - - - - - - - - - - - - - - - - - - END CONSTANTS - - - - - - - - - - - - - - - - */

/* - - - - - - - - - - - - - - - - - - STRUCTURES  - - - - - - - - - - - - - - - - - - - - */

/**
 * @struct PasswordRequest
 * @brief Represents the client's request for password generation.
 *
 * This structure is sent from the client to the server and includes:
 * - `type`: Specifies the type of password (e.g., numeric, alphanumeric, etc.).
 * - `length`: A string indicating the desired length of the generated password.
 *
 * @note The `length` is stored as a string to facilitate flexible input validation
 *       and to avoid integer overflow issues during parsing.
 */
typedef struct {
    char type;        				/**< Type of password requested ('n', 'a', 'm', etc.) */
    char length[BUFFER_SIZE];       /**< Desired length of the password as a string */
} PasswordRequest;

/**
 * @struct PasswordResponse
 * @brief Represents the server's response containing the generated password.
 *
 * This structure is sent from the server to the client and includes:
 * - `password`: The actual password generated by the server.
 *
 * @note The `password` field is null-terminated to ensure proper handling as a C string.
 */
typedef struct {
    char password[MAX_PASSWORD_LENGTH + 1];  /**< The generated password */
} PasswordResponse;

/* - - - - - - - - - - - - - - - - - - - END OF STRUCTURES - - - - - - - - - - - - - - - - - - - - - */

#endif // PROTOCOL_H
