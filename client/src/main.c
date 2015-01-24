/*
 * This is main file of the IRC-like client.
 */

#include "../../include/poorIRC_proto.h"
#include "../../include/sock_help.h"

#include <stdio.h>
#include <stdlib.h>


int main(int argc, char **argv)
{

	int socket;
	int num;
	size_t ln;

	struct poorIRC_response res;
	struct poorIRC_message  msg;

	printf("Welcome to poorIRC client\n");

	if(argc != 3) {

		fprintf(stderr, "Usage: %s hostname port\n", argv[0]);
		return EXIT_FAILURE;

	}

	if((socket = get_tcp_socket(argv[2], argv[1], SOCKET_CONN)) == -1) {

		fprintf(stderr, "Error: get_tcp_socket() failed\n");
		return EXIT_FAILURE;

	}

	printf("Socket successfully obtained\n");

	while(1) {

		printf("Enter message: ");
		fgets(msg.body, POORIRC_MSG_MAX_LEN, stdin);

		ln = strlen(msg.body) - 1;

		if (msg.body[ln] == '\n')
			msg.body[ln] = '\0';

		msg.len = strlen(msg.body) + 1;

		printf("Trying to send message...\n");

		if(send(socket, &(msg.len), sizeof(msg.len), 0) == -1) {

			fprintf(stderr, "Error: send() failed with status: "
					"%s\n", strerror(errno));

		}


		if(send(socket, &(msg.body), msg.len, 0) == -1) {

			fprintf(stderr, "Error: send() failed with status: "
					"%s\n", strerror(errno));

		}

		printf("Success!\n");
		printf("Waiting for server response...\n");

		if((num = recv(socket, &res, sizeof(res), 0)) == -1) {

			fprintf(stderr, "Error: recv() failed with status: %s\n", strerror(errno));
			return EXIT_FAILURE;

		}

		if(res.status == POORIRC_STATUS_OK) {

			printf("Status OK received\n");

		}

	}

	close(socket);

	return EXIT_SUCCESS;

}

