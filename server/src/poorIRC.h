/*
 * Definitions of structures and functions used by poorIRC server
 */

#ifndef _POORIRC_H
#define _POORIRC_H

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#define POORIRC_MODE_DEBUG 0x01
#define POORIRC_MODE_BG    0x02

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
	char               port_no[6];
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
	struct addrinfo *address_list;
	struct sockaddr_storage client_addr;

};

/*
 * Initialization of server instance, returns 0 on success, and non-zero value
 * on failure. Allocates memory for struct poorIRC_server instance.
 */

int poorIRC_setup(int argc, char **argv, struct poorIRC_config **cfg);
int poorIRC_init(const struct poorIRC_config *cfg, struct poorIRC_server **srv);
int poorIRC_serve(const struct poorIRC_config *cfg, struct poorIRC_server **srv);


#endif /* _POORIRC_H */

