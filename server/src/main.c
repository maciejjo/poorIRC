/*
 * This is main file of the IRC-like server.
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{

	unsigned short i;

	printf("Welcome to poorIRC. The shitty IRC rip-off\n");
	printf("Number of arguments: %d\n", argc - 1);

	for(i = 0; i < argc; i++) {

		if(i == 0)
			printf("Command: %s\n", argv[i]);
		else
			printf("Argument %2u: %s\n", i, argv[i]);

	}

	return EXIT_SUCCESS;

}

