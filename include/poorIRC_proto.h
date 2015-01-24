#ifndef POORIRC_PROTO_H
#define POORIRC_PROTO_H

#define POORIRC_MSG_MAX_LEN      128
#define POORIRC_NICKNAME_MAX_LEN 16

/*
 * struct poorIRC_message
 * This structure is passed from clients to server.
 */

struct poorIRC_message {

	unsigned char len;
	char          body[POORIRC_MSG_MAX_LEN];

};

struct poorIRC_message_srv {

	unsigned char len;
	char          body[POORIRC_MSG_MAX_LEN + POORIRC_NICKNAME_MAX_LEN];

};

/*
 * struct poorIRC_response
 * Thus structure is used to inform client about the status of request
 */

struct poorIRC_response {

	char status;

};

/*
 * Definitions of various status codes
 */

#define POORIRC_STATUS_OK   0x00
#define POORIRC_STATUS_FAIL 0x01

#endif /* POORIRC_PROTO_H */

