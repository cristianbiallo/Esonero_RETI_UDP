/**
 * @file UDP_server.c
 * @brief UDP server implementation in C for handling password generation requests.
 *        Listens for incoming client requests, processes them, and sends back responses.
 * @version 1.0.0
 * @date 2024-12-15
 * @author Cristian Biallo
 */

#if defined WIN32
#include <winsock.h> 		/**< Include Winsock header for Windows */
#else
#include <unistd.h>  		/**< Include UNIX standard header for close() */
#include <sys/socket.h>  	/**< Include socket library for UNIX */
#include <arpa/inet.h>  	/**< Include ARP and Internet address family libraries */
#include <sys/types.h>   	/**< Include for socket types */
#include <netinet/in.h>  	/**< Include for internet address family structures */
#include <netdb.h>  		/**< Include for host and network databases */
#define closesocket close  	/**< Define closesocket to close for UNIX systems */
#endif

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "libs/password/password.h"  /**< Include the header for password generation functions */
#include "libs/protocol/protocol.h"  /**< Include protocol definitions for communication */
#include "libs/utils/utils.h"    	 /**< Include utility functions */


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
 * @brief Sets up the server address structure.
 * @param[out] server_address Pointer to the sockaddr_in structure to configure.
 * @pre `server_address` must be a valid pointer.
 * @post The `server_address` structure is configured with default values.
 */
void setup_server_address(struct sockaddr_in *server_address) {
    memset(server_address, 0, sizeof(*server_address));			/**< Clear the structure */
    server_address->sin_family = AF_INET;						/**< Set address family to AF_INET (IPv4) */
    server_address->sin_port = htons(DEFAULT_PORT);				/**< Set server port, converting to network byte order */
    server_address->sin_addr.s_addr = inet_addr(DEFAULT_IP);	/**< Set server IP address */
}


/**
 * @brief Processes a password generation request and generates the password.
 * @param[in] request Pointer to the PasswordRequest structure containing client input.
 * @param[out] response Pointer to the PasswordResponse structure to store the generated password.
 * @pre `request` and `response` must be valid, initialized pointers.
 * @post The `response` structure is populated with a generated password.
 */
void handle_password_request(const PasswordRequest *request, PasswordResponse *response) {
	int numerical_length = atoi(request->length);
	PasswordType password_type;

	switch (tolower(request->type)) {
		case 'n': password_type = NUMERIC; break;
		case 'a': password_type = ALPHA; break;
		case 'm': password_type = MIXED; break;
		case 's': password_type = SECURE; break;
		case 'u': password_type = UNAMBIGUOUS; break;
		default: password_type = NUMERIC; break;
	}
	generate_password(response->password, password_type, numerical_length);
}


/**
 * @brief Sends a password response to the client.
 * @param[in] server_socket The server's socket descriptor.
 * @param[in] response_msg Pointer to the PasswordResponse structure to send.
 * @param[in] client_address Pointer to the sockaddr_in structure containing the client's address.
 * @return `true` if the response was sent successfully, `false` otherwise.
 * @pre `server_socket` must be a valid UDP socket.
 * @pre `response_msg` and `client_address` must be valid pointers.
 * @post The client receives the password response if successful.
 */
bool send_response(int server_socket, const PasswordResponse *response_msg, const struct sockaddr_in *client_address) {
    if (sendto(server_socket, response_msg, sizeof(*response_msg), 0,
               (struct sockaddr *)client_address, sizeof(*client_address)) != sizeof(*response_msg)) {
        error_handler("Error sending response (Password generated).\n");
        return false;
    }
    return true;
}

/**
 * @brief Receives a password generation request from the client.
 * @param[in] server_socket The server's socket descriptor.
 * @param[out] request_msg Pointer to the PasswordRequest structure to store the client's request.
 * @param[out] client_address Pointer to the sockaddr_in structure to store the client's address.
 * @return `true` if the request was received successfully, `false` otherwise.
 * @pre `server_socket` must be a valid UDP socket.
 * @pre `request_msg` and `client_address` must be valid pointers.
 * @post The `request_msg` and `client_address` structures are populated with client data if successful.
 */
bool receive_request(int server_socket, PasswordRequest *request_msg, struct sockaddr_in *client_address) {
    unsigned int client_address_size = sizeof(*client_address);
    int rcv_msg_size = recvfrom(server_socket, request_msg, sizeof(*request_msg), 0,
                                (struct sockaddr *)client_address, &client_address_size);
    if (rcv_msg_size < 0) {
        error_handler("Error receiving request (Password settings).\n");
        return false;
    }
    return true;
}


/**
 * @brief Entry point for the UDP server program.
 * @return Exit status of the program.
 * @return EXIT_SUCCESS The server executed successfully.
 * @return EXIT_FAILURE An error occurred during execution.
 * @details Initializes the server, listens for client requests, and processes them in an infinite loop.
 */
int main() {

#if defined WIN32
	// Initialize Winsock
	WSADATA wsa_data;  /**< Holds information about the Windows Sockets implementation */
	WORD version_requested = MAKEWORD(2,2);  /**< Request version 2.2 of Winsock */
	int result = WSAStartup(version_requested, &wsa_data);  /**< Initialize Winsock library */
	if (result != NO_ERROR) {
		error_handler("Error at WSAStartup()\nA usable WinSock DLL cannot be found.\n");
		return 0;
	}
#endif


    int server_socket = initialize_socket();
    if (server_socket < 0) {
        clear_winsock();
        return EXIT_FAILURE;
    }

    struct sockaddr_in server_address, client_address;

    setup_server_address(&server_address);

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
    	error_handler("Bind failed.\n");
        closesocket(server_socket);
        clear_winsock();
        return EXIT_FAILURE;
    }

    print_with_color("Server listening...\n\n", BLUE);

    while (true) {
        PasswordRequest request;
        PasswordResponse response;

        if (!receive_request(server_socket, &request, &client_address)) {
            closesocket(server_socket);
            clear_winsock();
            return EXIT_FAILURE;
        }

        print_with_color("New connection from ", GREEN);
        print_with_color(inet_ntoa(client_address.sin_addr), YELLOW);
        print_with_color(":", CYAN);
        printf("%d\n", ntohs(client_address.sin_port));

        handle_password_request(&request, &response);

        if (!send_response(server_socket, &response, &client_address)) {
            closesocket(server_socket);
            clear_winsock();
            return EXIT_FAILURE;
        }

    }
}
