/**
 * @file UDP_client.c
 * @brief UDP client for requesting password generation from a remote server.
 * @details The client connects to a password generation server via UDP, sends a request specifying the desired
 * password type and length, and receives the generated password in response.
 * @version 1.0.1
 * @date 2024-12-15
 * @author Cristian Biallo
 */

#if defined WIN32
#include <winsock.h>  		/**< Include Winsock header for Windows */
#else
#include <unistd.h> 	 	/**< Include UNIX standard header for close() */
#include <sys/socket.h> 	/**< Include socket library for UNIX */
#include <arpa/inet.h>   	/**< Include ARP and Internet address family libraries */
#include <sys/types.h>   	/**< Include for socket types */
#include <netinet/in.h> 	/**< Include for internet address family structures */
#include <netdb.h>  		/**< Include for host and network databases */
#define closesocket close   /**< Define closesocket to close for UNIX systems */
#endif

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "libs/password/password.h"  /**< Include password control functions */
#include "libs/protocol/protocol.h"  /**< Include protocol header for message structures and communication formats */
#include "libs/utils/utils.h"	     /**< Include the utils.h library for utility functions */


/**
 * @brief Cleans up the Winsock library (Windows only).
 * @details This function ensures the proper termination of the Winsock library to release resources.
 */
void clear_winsock() {
#if defined WIN32
	WSACleanup();  /**< Cleans up the Winsock DLL */
#endif
}

/**
 * @brief Prints an error message to the console in magenta color.
 * @param[in] error_message The error message to be displayed.
 * @note On Windows, the program pauses for 3 seconds after displaying the error.
 */
void error_handler(const char *error_message) {
    print_with_color(error_message, MAGENTA); /**< Display the error message */
#if defined WIN32
	Sleep(3000);  /**< Pause for 3 seconds */
#endif
}

/**
 * @brief Creates and initializes a UDP socket.
 * @details This function creates a socket for communication using UDP.
 * @return >=0 The socket descriptor if successful.
 * @return -1 An error occurred while creating the socket.
 */
int initialize_socket() {
    int created_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (created_socket < 0) {
        error_handler("Error creating socket.\n");
    }
    return created_socket;
}

/**
 * @brief Resolves the server's hostname to an IP address.
 * @details This function uses `gethostbyname` to resolve the server's hostname and populate the address structure.
 * @param[in] server_name The hostname of the server to resolve.
 * @param[out] server_address Pointer to a sockaddr_in structure to store the resolved address.
 * @return true The hostname was successfully resolved.
 * @return false An error occurred during resolution.
 */
bool resolve_server_address(const char *server_name, struct sockaddr_in *server_address) {
    struct hostent *host = gethostbyname(server_name);
    if (host == NULL) {
        error_handler("Error resolving host\n");
        return false;
    }
    memset(server_address, 0, sizeof(struct sockaddr_in));			/**< Clear the structure */
    server_address->sin_family = AF_INET;							/**< Set address family to AF_INET (IPv4) */
    server_address->sin_addr = *((struct in_addr *)host->h_addr);	/**< Set server IP address */
    server_address->sin_port = htons(DEFAULT_PORT);					/**< Set server port, converting to network byte order */
    return true;
}

/**
 * @brief Reads user input for password generation parameters.
 * @details Displays a menu and prompts the user to enter the password type and length. Validates the input.
 * @param[out] password_request Pointer to a PasswordRequest structure to store user input.
 * @return true User input is valid.
 * @return false User input is invalid.
 */
bool handle_user_input(PasswordRequest *password_request) {
    char input[BUFFER_SIZE];
    int arguments;

    do {
        show_password_menu();				/**< Display the menu */
        fgets(input, sizeof(input), stdin);	/**< Read user input for password type and length */
        input[BUFFER_SIZE - 1] = '\0';		/**< Ensure null termination for the length string */

        arguments = sscanf(input, " %c %s %s", &password_request->type, password_request->length, input);
        password_request->length[BUFFER_SIZE - 1] = '\0';

        if (tolower(password_request->type) == 'h') {
            show_help_menu();	/**< Display help menu */
        }
    } while (tolower(password_request->type) == 'h');


    if (arguments == 1) {
        strcpy(password_request->length, "8"); /**< Default password length */
    } else if (arguments != 2) {
        print_with_color("Invalid input. Please enter a valid type and length.\n", RED);
        return false;
    }

    if (!control_type("namsuq", password_request->type)) {
    	print_with_color("Bad request: the type inserted is not valid.\n", RED);
    	return false;
    }

    if (!control_length(password_request->length, MIN_PASSWORD_LENGTH, MAX_PASSWORD_LENGTH)) {
    	print_with_color("Bad request: the length for the password is not valid.\n", RED);
    	return false;
    }

    return true;
}

/**
 * @brief Sends a password generation request to the server.
 * @details The function sends the PasswordRequest structure to the specified server address.
 * @param[in] client_socket The socket descriptor.
 * @param[in] password_request Pointer to the PasswordRequest structure.
 * @param[in] server_address Pointer to the sockaddr_in structure of the server.
 * @return true The request was sent successfully.
 * @return false An error occurred while sending the request.
 */
bool send_request(int client_socket, const PasswordRequest *password_request, const struct sockaddr_in *server_address) {
    if (sendto(client_socket, password_request, sizeof(*password_request), 0,
               (struct sockaddr *)server_address, sizeof(*server_address)) != sizeof(*password_request)) {
        error_handler("Error sending request (Password settings).\n");
        return false;
    }
    return true;
}

/**
 * @brief Receives a password generation response from the server.
 * @param[in] client_socket The socket descriptor.
 * @param[out] response_msg Pointer to the PasswordResponse structure to store the server response.
 * @param[in] server_address Pointer to the sockaddr_in structure of the server.
 * @return true The response was received successfully.
 * @return false An error occurred while receiving the response.
 */
bool receive_response(int client_socket, PasswordResponse *response_msg, struct sockaddr_in *server_address) {
    unsigned int server_address_size = sizeof(*server_address);
    int rcv_msg_size = recvfrom(client_socket, response_msg, sizeof(*response_msg), 0,
                                (struct sockaddr *)server_address, &server_address_size);
    if (rcv_msg_size < 0) {
        error_handler("Error receiving response (Password generation response).\n");
        return false;
    }
    return true;
}


/**
 * @brief Main function for the UDP client.
 * @details This function initializes the socket, resolves the server address, and handles the communication loop
 * with the password generation server. The loop terminates when the user decides to quit.
 * @return EXIT_SUCCESS Program completed successfully.
 * @return EXIT_FAILURE An error occurred during execution.
 */
int main() {

#if defined WIN32
	// Initialize Winsock
	WSADATA wsa_data;  /**< Holds information about the Windows Sockets implementation */
	WORD version_requested = MAKEWORD(2,2);  /**< Request version 2.2 of Winsock */
	int result = WSAStartup(version_requested, &wsa_data);  /**< Initialize Winsock library */
	if (result != NO_ERROR) {
		error_handler("Error at WSAStartup()\nA usable WinSock DLL cannot be found.\n");
		return EXIT_FAILURE;
	}
#endif

    struct sockaddr_in server_address; 		/**< Structure to hold the server address */

	// Resolve the server address
    if (!resolve_server_address("passwdgen.uniba.it", &server_address)) {
        clear_winsock();
        return EXIT_FAILURE;
    }

    // Initialize the UDP socket
    int client_socket = initialize_socket();
    if (client_socket < 0) {
        clear_winsock();
        return EXIT_FAILURE;
    }

    PasswordRequest password_request;	/**< Structure to hold password request (type and length) */
    PasswordResponse response_msg;		/**< Structure to hold server's response */

    // Start password generation loop
    while(true) {
    	// Handle user input for password type and length
        if (!handle_user_input(&password_request)) {
            continue;	/**< If input is invalid, re-prompt the user */
        }

        // Check if the user wants to quit
        if(!keep_generating(password_request.type, 'q')) {
        	break;
        }

        // Send the password request to the server
        if (!send_request(client_socket, &password_request, &server_address)) {
            closesocket(client_socket);
            clear_winsock();
            return EXIT_FAILURE;
        }

        // Receive the password response from the server
        if (!receive_response(client_socket, &response_msg, &server_address)) {
            closesocket(client_socket);
            clear_winsock();
            return EXIT_FAILURE;
        }

        // Display the generated password
		print_with_color("Password generated: ", GREEN);
		print_with_color(response_msg.password, GREEN);
		printf("\n\n");

    }

    // Close the connection and clean up
    closesocket(client_socket); /**< Close the socket */
    clear_winsock();            /**< Clean up Winsock */
#if defined WIN32
    Sleep(3000); /**< Pause for 3 seconds before exiting */
#endif
    return EXIT_SUCCESS;
}
