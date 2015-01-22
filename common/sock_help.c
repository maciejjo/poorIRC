#include "../include/sock_help.h"

void *get_in_addr(struct sockaddr *sa)
{
	if(sa->sa_family == AF_INET)
		return &(((struct sockaddr_in *) sa)->sin_addr);

	else
		return &(((struct sockaddr_in6 *) sa)->sin6_addr);

}

int get_tcp_socket(const char *port, char *hostname, char flags)
{

	int fd  = 0;
	int rv  = 0;
	int yes = 1;

	char addr_str[INET6_ADDRSTRLEN];

	struct addrinfo hints;
	struct addrinfo *address_list;
	struct addrinfo *p = NULL;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family        = AF_UNSPEC;
	hints.ai_socktype      = SOCK_STREAM;

	if(hostname == NULL) {
		hints.ai_flags = AI_PASSIVE;
	}


	if((rv = getaddrinfo(hostname, port, &hints, &address_list)) != 0) {

		fprintf(stderr, "Error: getaddrinfo failed with status: %s\n",
				gai_strerror(rv));
		return -1;

	}

	for(p = address_list; p != NULL; p = p->ai_next) {

		inet_ntop(p->ai_family, get_in_addr((struct sockaddr *) p->ai_addr),
					addr_str, sizeof(addr_str));

		printf("Trying to connect to %s\n", addr_str);



		if((fd = socket(p->ai_family, p->ai_socktype,
						p->ai_protocol)) == -1) {
			fprintf(stderr, "Error: socket() failed with status: "
					"%s\n", strerror(errno));
			continue;

		}

		if((flags & SOCKET_REUS) && setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes,
						sizeof(int)) != 0) {

			fprintf(stderr, "Error: setsockopt() failed with status: "
					"%s\n", strerror(errno));
			return -1;

		}

		if((flags & SOCKET_BIND) && bind(fd, p->ai_addr, p->ai_addrlen) == -1) {

			fprintf(stderr, "Error: bind() failed with status: "
					"%s\n", strerror(errno));
			close(fd);
			continue;

		}

		if((flags & SOCKET_CONN) && connect(fd, p->ai_addr, p->ai_addrlen) == -1) {

			fprintf(stderr, "Error: connect() failed with status: "
					"%s\n", strerror(errno));
			close(fd);
			continue;

		}

		break;

	}

	if(p == NULL) {

		fprintf(stderr, "Error: failed to bind to any socket\n");
		return -1;

	}

	freeaddrinfo(address_list);

	return fd;

}

