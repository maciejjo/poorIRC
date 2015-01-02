/*
 * This is main file of the IRC-like server.
 */

#include <stdio.h>
#include <stdlib.h>

#include "poorIRC.h"

int main(int argc, char **argv)
{

	struct poorIRC_config *server_config   = NULL;
	struct poorIRC_server *server_instance = NULL;

	unsigned short i = 0;


	printf("Welcome to poorIRC. The shitty IRC rip-off\n");
	printf("Number of arguments: %d\n", argc - 1);

	for(i = 0; i < argc; i++) {

		if(i == 0)
			printf("Command: %s\n", argv[i]);
		else
			printf("Argument %2u: %s\n", i, argv[i]);

	}

	if(poorIRC_setup(argc, argv, server_config) != 0) {

		fprintf(stderr, "Server configuration failed, exiting...\n");
		return EXIT_FAILURE;

	}

	if(poorIRC_init(server_config, server_instance) != 0) {

		fprintf(stderr, "Server initialization failed, exiting...\n");
		return EXIT_FAILURE;

	}

	return EXIT_SUCCESS;

}

