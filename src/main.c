#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include "socket.h"

int main(int argc, char **argv) {

	int socket_serveur = creer_serveur(8080);

	if(socket_serveur != -1){
		initialiser_signaux();
		printf("Server started correctly\n");
		while(1){

			accept_client(socket_serveur);
			
		}

	} else {
		printf("fail\n");
		return -1;
	}
	return 0;
}




