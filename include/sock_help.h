#ifndef SOCK_HELP_H
#define SOCK_HELP_H

#ifdef __linux__ 
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#elif _WIN32
#include <winsock.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define SOCKET_BIND 0x01
#define SOCKET_CONN 0x02
#define SOCKET_REUS 0x04

void *get_in_addr(struct sockaddr *sa);
int get_tcp_socket(const char *port, char *hostname, char flags);

#endif
