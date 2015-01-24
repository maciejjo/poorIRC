/*
 * This file contains functions to set up and run poorIRC server.
 */

#include "poorIRC_server.h"
#include "../../include/sock_help.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include <unistd.h>

#include <sys/mman.h>
#include <semaphore.h>

int poorIRC_setup(int argc, char **argv, struct poorIRC_config **cfg)
{

	char pflag    = 0;
	int  c        = 0;

	printf("Setting up server configuration...\n");

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

	printf("Configuration OK!\n");
	return 0;

}


int poorIRC_init(struct poorIRC_config *cfg, struct poorIRC_server **srv)
{

	printf("Initializing server...\n");
	
	if((*srv = calloc(1, sizeof(struct poorIRC_server))) == NULL) {

		fprintf(stderr, "Error: calloc() failed with status: %s\n",
				strerror(errno));
		return 1;

	}

	if(((*srv)->shared_buf = mmap(NULL, sizeof(struct poorIRC_server_shared_buffer),
					PROT_READ | PROT_WRITE, MAP_ANON |
					MAP_SHARED, -1, 0)) == MAP_FAILED) {

		fprintf(stderr, "Error: mmap() failed with status: %s\n",
				strerror(errno));
		return 1;

	}

	if(((*srv)->shared_lookup = mmap(NULL, sizeof(struct poorIRC_server_client_lookup_table),
					PROT_READ | PROT_WRITE, MAP_ANON |
					MAP_SHARED, -1, 0)) == MAP_FAILED) {

		fprintf(stderr, "Error: mmap() failed with status: %s\n",
				strerror(errno));
		return 1;

	}

	if(sem_init(&((*srv)->shared_buf->buffer_mutex), 1, 1) == -1) {

		fprintf(stderr, "Error: sem_init() failed with status: %s\n",
				strerror(errno));
		return 1;

	}

	if(sem_init(&((*srv)->shared_lookup->lookup_mutex), 1, 1) == -1) {

		fprintf(stderr, "Error: sem_init() failed with status: %s\n",
				strerror(errno));
		return 1;

	}

	(*srv)->config = cfg;

	if(((*srv)->listen_fd = get_tcp_socket((*srv)->config->port_no, NULL, SOCKET_BIND | SOCKET_REUS)) == -1) {

		fprintf(stderr, "Error getting TCP socket!\n");
		return 1;

	}

	if(listen((*srv)->listen_fd, cfg->max_connections) != 0) {

		fprintf(stderr, "Error: listen() failed with status: %s\n",
				strerror(errno));
		return 1;

	}

	printf("Initialization OK!\n");
	return 0;

}

int poorIRC_wait_for_client(struct poorIRC_server *srv)
{


	int i;
	socklen_t sin_size;
	char address_string[INET6_ADDRSTRLEN];

	printf("Entering waiting for client loop.\n");

	while(1) {

		printf("Waiting loop: waiting for next client...\n");

		sin_size = sizeof(srv->client_addr);

		if((srv->client_fd = accept(srv->listen_fd,
			(struct sockaddr *) &(srv->client_addr), 
			&sin_size)) == -1) {

			fprintf(stderr, "Error: accept() failed with status: "
					"%s\n", strerror(errno));
			continue;

		}

		printf("Printing client list...\n");

		sem_wait(&(srv->shared_lookup->lookup_mutex));

		printf("Critical section begin\n");

		for(i = 0; i < srv->shared_lookup->clients_no; i++) {

			printf("%d. Client list loop.\n", i);

			if(srv->shared_lookup->lookup_table[i].active) {
				printf("%d. Nickname: %s, PID: %d\n", i,
					srv->shared_lookup->lookup_table[i].nickname,
					srv->shared_lookup->lookup_table[i].pid
				      );
			}

		}

		printf("Critical section end\n");

		sem_post(&(srv->shared_lookup->lookup_mutex));


		inet_ntop(srv->client_addr.ss_family, 
			get_in_addr((struct sockaddr *) &(srv->client_addr)), 
			address_string, sizeof(address_string));

		printf("Server: Got connection from %s, "
			"forking client process...\n", address_string);

		if(!fork()) {

			poorIRC_serve(srv);

		}



	}

	return 0;

}

int poorIRC_serve(struct poorIRC_server *srv)
{

	struct poorIRC_message  msg;
	struct poorIRC_response res;

	int num;
	pid_t mypid;


	mypid = getpid();

	res.status = POORIRC_STATUS_OK;

	memset(&msg, 0, sizeof(msg));

	printf("(CHLD %d) In serving function, entering server loop.\n", mypid);

	while(1) {

		printf("(CHLD %d) In server loop\n", mypid);
		
		if((num = recv(srv->client_fd, &(msg.len), sizeof(msg.len), 0)) == -1) {

			fprintf(stderr, "Error: recv() failed with status "
					"%s\n", strerror(errno));
			break;

		}

		if(num == 0) {

			printf("(CHLD %d) Remote host closed connection. Bye!\n", mypid);
			break;

		}

		printf("(CHLD %d) Received length: %d\n", mypid, msg.len);
		printf("(CHLD %d) Receiving message with given length...\n", mypid);

		if((num = recv(srv->client_fd, &(msg.body), msg.len, 0)) == -1) {

			fprintf(stderr, "(CHLD %d) Error: recv() failed with status "
					"%s\n", mypid, strerror(errno));

		}


		printf("(CHLD %d) Got message: \"%s\" - %d bytes.\n", mypid, msg.body, num);

		if(num == 0) {

			printf("(CHLD %d) Remote host closed connection. Bye!\n", mypid);
			break;

		}

		poorIRC_process_message(&msg, srv);

		printf("(CHLD %d) Sending OK status to client.\n", mypid);

		res.status = POORIRC_STATUS_OK;

		if(send(srv->client_fd, &res, sizeof(res), 0) == -1) {
			
			fprintf(stderr, "(CHLD %d) Error: send() failed with status: "
					"%s\n", mypid, strerror(errno));

		}


		printf("(CHLD %d) End of loop.\n", mypid);
	}


	close(srv->client_fd);
	return 0;

}

int poorIRC_process_message(struct poorIRC_message *msg, struct poorIRC_server *srv)
{

	printf("Process message routine\n");

	if(msg->body[0] == '\0' || (msg->body[0] == '/' && msg->body[1] == '\0')) {

		printf("Empty message, ommiting...\n");
		return 0;

	} else if(msg->body[0] == '/') {

		printf("This is a command!\n");
		poorIRC_process_command(msg, srv);

	} else {

		printf("This is an usual message. Broadcasting.\n");
		poorIRC_broadcast_message(msg, srv);

	}

	return 0;

}

int poorIRC_process_command(struct poorIRC_message *msg, struct poorIRC_server *srv)
{

	char *nick;

	if(strncmp(msg->body, "/nick", strlen("/nick")) == 0) {

		printf("Received nick command\n");

		nick = &(msg->body[strlen("/nick ")]);

		if(poorIRC_is_nickname_taken(nick, srv)) {

			printf("Nickname taken\n");

		}

		
		poorIRC_register_client(nick, srv);

		return 0;

	} else {

		printf("Command not recognized!\n");

		return -1;

	}

	return 0;

}

int poorIRC_broadcast_message(struct poorIRC_message *msg, struct poorIRC_server *srv)
{

	printf("Broadcasting message\n");

	sem_wait(&(srv->shared_buf->buffer_mutex));

	srv->shared_buf->buffer.len = msg->len;
	strncpy(srv->shared_buf->buffer.body, msg->body, POORIRC_MSG_MAX_LEN - 1);

	sem_post(&(srv->shared_buf->buffer_mutex));

	return 0;

}

int poorIRC_register_client(char *nickname, struct poorIRC_server *srv)
{

	printf("Registering new client...\n");

	sem_wait(&(srv->shared_lookup->lookup_mutex));

	if(srv->shared_lookup->clients_no > POORIRC_MAX_CLIENTS) {

		sem_post(&(srv->shared_lookup->lookup_mutex));
		fprintf(stderr, "Cannot connect to server, too many clients!\n");
		return -1;

	}

	srv->shared_lookup->lookup_table[srv->shared_lookup->clients_no].pid = getpid();
	strncpy(srv->shared_lookup->lookup_table[srv->shared_lookup->clients_no].nickname, nickname, POORIRC_NICKNAME_MAX_LEN);
	srv->shared_lookup->lookup_table[srv->shared_lookup->clients_no].active = 1;

	srv->shared_lookup->clients_no++;

	sem_post(&(srv->shared_lookup->lookup_mutex));

	return 0;

}

int poorIRC_is_nickname_taken(char *nickname, struct poorIRC_server *srv)
{
	int i = 0;

	return i;

}
