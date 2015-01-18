/*
 * This is main file of the IRC-like server.
 */

#include <stdio.h>
#include <stdlib.h>

/*
#include <poorIRC.h>
*/
#include "../../include/poorIRC.h"

int main(int argc, char **argv)
{

	struct poorIRC_config *server_config   = NULL;
	struct poorIRC_server *server_instance = NULL;


	if(poorIRC_setup(argc, argv, &server_config) != 0) {

		fprintf(stderr, "Server configuration failed, exiting...\n");
		return EXIT_FAILURE;

	}

	if(poorIRC_init(server_config, &server_instance) != 0) {

		fprintf(stderr, "Server initialization failed, exiting...\n");
		return EXIT_FAILURE;

	}

	if(poorIRC_serve(server_config, &server_instance) != 0) {

		fprintf(stderr, "Server stoped working, exiting...\n");
		return EXIT_FAILURE;

	}

	return EXIT_SUCCESS;

}
