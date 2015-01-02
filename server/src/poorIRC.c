/*
 * This file contains functions to set up and run poorIRC server.
 */

#include "poorIRC.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>

int poorIRC_setup(int argc, char **argv, struct poorIRC_config *cfg)
{

	char pflag    = 0;
	int  finished = 0;
	int  c        = 0;

	if((cfg = calloc(1, sizeof(struct poorIRC_config))) == NULL) {

		fprintf(stderr, "Error: calloc() failed with status: %s\n",
				strerror(errno));
		return -1;

	}

	while(!finished) {

		int option_index = 0;

		static struct option long_options[] = {
			{"port-number", required_argument, NULL, 'p'},
			{"debug",       no_argument,       NULL, 'd'},
			{"daemon",      no_argument,       NULL, 'b'},
		};

		c = getopt_long(argc, argv, "p:db", long_options,
				&option_index);

		if(c == -1)
			break;

		switch(c) {

			case 'p':
				cfg->port_no = atoi(optarg);
				pflag = 1;
				break;

			case 'd':
				cfg->mode |= POORIRC_MODE_DEBUG;
				break;

			case 'b':
				cfg->mode |= POORIRC_MODE_BG;
				break;

			case '?':
				break;

			default:
				fprintf(stderr, "Error: getopt() returned char"
						" 0%o\n", c);
				break;

		}

	}

	if(!pflag) {

		fprintf(stderr, "Error: port number must be specified!\n");
		return -1;

	}

	return 0;

}


int poorIRC_init(struct poorIRC_config *cfg, struct poorIRC_server *srv)
{

	if((srv = calloc(1, sizeof(struct poorIRC_server))) == NULL) {

		fprintf(stderr, "Error: calloc() failed with status: %s\n",
				strerror(errno));
		return -1;

	}

	return 0;

}

