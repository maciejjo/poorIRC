/*
 * This file contains functions to set up and run poorIRC server.
 */

#include "poorIRC.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

int poorIRC_init(struct poorIRC_config *cfg, struct poorIRC_server *srv)
{

	if((srv = calloc(1, sizeof(struct poorIRC_server))) == NULL) {

		fprintf(stderr, "Error: calloc() failed with status: %s\n",
				strerror(errno));
		return -1;

	}

	return 0;

}

