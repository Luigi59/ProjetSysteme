#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include "socket.h"

int main(int argc, char **argv) {
	const char *message_bienvenue = "Bonjour, bienvenue sur notre serveur\nNous sommes tres heureux de vous recevoir\nJ'adore les sucettes, et les gros calins !\nJe suis Charlie\nC'est l'histoire d'un mec qui rentre dans un cafe, et PLOUF !\nTu connais la blague a deux balles ? PAN PAN !\nJ'ai envie de me suicider parce que c'est cool la mort....... ouais c'est trop cool\nVive les lamasticots !!!\n";
	int res = creer_serveur(8080);
	int socket_client;
	int fils;

	if(res != -1){
		initialiser_signaux();
		while(1){
			
			socket_client = accept(res, NULL, NULL);
			printf("CONNEXION ACCEPTEE\n");
			
			fils = fork();
			if(fils == 0) {				//DEBUT DU FILS
				
				sleep(1);
				write(socket_client, message_bienvenue, strlen(message_bienvenue));
				int quit = 0;			
				while(quit == 0){
					char buf[100];
					int taille;
					taille = read(socket_client, buf, sizeof(buf));
					if(taille == 0) {
						quit = 1;
						exit(0);
					}						
					else if(taille != -1){
						printf("message reçu :-D\n");					
						write(socket_client, buf, taille);
						sleep(1);
						printf("			message envoyé\n");
					} else{
						printf("message pas reçu :-(\n");
					}
				}
			}					//FIN DU FILS
			
		}

	} else {
		printf("fail\n");
		return -1;
	}
	return 0;
}




