#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#include "socket.h"


int check(const char * dossier){
    struct stat s;
    int fd = stat(dossier, &s);

    if(fd == -1 )
    {
        perror("No directory found.\n");
        exit(EXIT_FAILURE);
    }

    if (!S_ISDIR(s.st_mode)) 
    {
        perror("Error while opening the directory.\n");
        exit(EXIT_FAILURE);
    }
    return 0;
}

int main(int argc, char **argv) {

	const char * site;

	if(argc < 2) {
		site = "documents";
	}

	else {
		const char * site = argv[1];
		if(check(site) != 0) {
			perror("Unknown error.\n");
			exit(EXIT_FAILURE);
		}
	}

	int socket_serveur = creer_serveur(8080);

	if(socket_serveur != -1){
		initialiser_signaux();
		printf("Server started correctly\n");
		while(1){

			accept_client(socket_serveur, site);
			
		}

	} else {
		printf("fail\n");
		return -1;
	}
	return 0;
}




