/*
 * Definitions of structures and functions used by poorIRC server
 */

#ifndef _POORIRC_H
#define _POORIRC_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

/*
 * struct poorIRC_config
 * Structure holding configuration of poorIRC server.
 *
 * @max_connections - number of clients able to join at once.
 *
 * @mode - char representing operation mode (server can operate on different
 * modes - eg. daemon or not)
 *
 * @port_no - port on which TCP port should the server operate.
 */

struct poorIRC_config {

	unsigned short int max_connections;
	unsigned char      mode;
	unsigned int       port_no;
	/* ... */

};

/*
 * struct poorIRC_server
 * This structure represents an instance of a server. It contains data required
 * for server operation (socket file descriptors, IP configuration, server
 * config, etc.) 
 */

struct poorIRC_server {

	struct poorIRC_config *config;

	int listen_fd;
	int client_fd;
	
	struct addrinfo hints;
	struct addrinfo serv_info;

};

#endif /* _POORIRC_H */

