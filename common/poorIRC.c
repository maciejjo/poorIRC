/*
 * This file contains functions to set up and run poorIRC server.
 */

#include "../include/poorIRC.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include <unistd.h>

void *get_in_addr(struct sockaddr *sa)
{
	if(sa->sa_family == AF_INET)
		return &(((struct sockaddr_in *) sa)->sin_addr);

	else
		return &(((struct sockaddr_in6 *) sa)->sin6_addr);

}

int poorIRC_setup(int argc, char **argv, struct poorIRC_config **cfg)
{

	char pflag    = 0;
	int  c        = 0;

	if((*cfg = calloc(1, sizeof(struct poorIRC_config))) == NULL) {

		fprintf(stderr, "Error: calloc() failed with status: %s\n",
				strerror(errno));
		return -1;

	}

	while(1) {

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
			strncpy((*cfg)->port_no, optarg, 5);
			pflag = 1;
			break;

		case 'd':
			(*cfg)->mode |= POORIRC_MODE_DEBUG;
			break;

		case 'b':
			(*cfg)->mode |= POORIRC_MODE_BG;
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


int poorIRC_init(const struct poorIRC_config *cfg, struct poorIRC_server **srv)
{
	
	int rv  = 0;
	int yes = 1;
	struct addrinfo *p = NULL;

	if((*srv = calloc(1, sizeof(struct poorIRC_server))) == NULL) {

		fprintf(stderr, "Error: calloc() failed with status: %s\n",
				strerror(errno));
		return -1;

	}

	(*srv)->hints.ai_family   = AF_UNSPEC;
	(*srv)->hints.ai_socktype = SOCK_STREAM;
	(*srv)->hints.ai_flags    = AI_PASSIVE;

	if((rv = getaddrinfo(NULL, cfg->port_no, &((*srv)->hints),
					&((*srv)->address_list))) != 0) {

		fprintf(stderr, "Error: getaddrinfo failed with status: %s\n",
				gai_strerror(rv));
		return -1;

	}

	for(p = (*srv)->address_list; p != NULL; p = p->ai_next) {

		if(((*srv)->listen_fd = socket(p->ai_family, p->ai_socktype,
						p->ai_protocol)) == -1) {
			fprintf(stderr, "Error: socket() failed with status: "
					"%s\n", strerror(errno));
			continue;

		}

		if(setsockopt((*srv)->listen_fd, SOL_SOCKET, SO_REUSEADDR, &yes,
						sizeof(int)) != 0) {

			fprintf(stderr, "Error: setsockopt() failed with status: "
					"%s\n", strerror(errno));
			return 1;

		}

		if(bind((*srv)->listen_fd, p->ai_addr, p->ai_addrlen) == -1) {

			fprintf(stderr, "Error: bind() failed with status: "
					"%s\n", strerror(errno));
			close((*srv)->listen_fd);
			continue;

		}

		break;

	}

	if(p == NULL) {

		fprintf(stderr, "Error: failed to bind to any socket\n");
		return 1;

	}

	freeaddrinfo((*srv)->address_list);

	if(listen((*srv)->listen_fd, cfg->max_connections) == -1) {

		fprintf(stderr, "Error: listen() failed with status: %s\n",
				strerror(errno));
		return 1;

	}

	return 0;

}

int poorIRC_serve(const struct poorIRC_config *cfg, struct poorIRC_server **srv) {

	socklen_t sin_size;
	char address_string[INET6_ADDRSTRLEN];

	while(1) {

		sin_size = sizeof((*srv)->client_addr);

		if(((*srv)->client_fd = accept((*srv)->listen_fd,
			(struct sockaddr *) &((*srv)->client_addr), 
			&sin_size)) == -1) {

			fprintf(stderr, "Error: accept() failed with status: "
					"%s\n", strerror(errno));
			continue;

		}

		inet_ntop((*srv)->client_addr.ss_family, 
			get_in_addr((struct sockaddr *) &((*srv)->client_addr)), 
			address_string, sizeof(address_string));

		printf("Server: Got connection from %s\n", address_string);

		if(!fork()) {

			/*
			 * We are the child process
			 */

			close((*srv)->listen_fd);

			if(send((*srv)->client_fd, "Hello world",
					strlen("Hello world"), 0) == -1) {
				
				fprintf(stderr, "Error: send() failed with status: "
						"%s\n", strerror(errno));

			}

			close((*srv)->client_fd);
			return 1;

		}



	}

	return 0;

}
