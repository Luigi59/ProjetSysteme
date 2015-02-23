#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include "socket.h"

int main(int argc, char **argv) {

	const char *message_bienvenue = "\nBonjour, bienvenue sur notre serveur\nNous sommes tres heureux de vous recevoir\nJ'adore les sucettes, et les gros calins !\nJe suis Charlie\nC'est l'histoire d'un mec qui rentre dans un cafe, et PLOUF !\nTu connais la blague a deux balles ? PAN PAN !\nJ'ai envie de me suicider parce que c'est cool la mort....... ouais c'est trop cool\nVive les lamasticots !!!\n\n";
	int socket_serveur = creer_serveur(8080);
	int socket_client;
	int fils;
	char buf[256];
	int taille;
	FILE * fclient;

	if(socket_serveur != -1){
		initialiser_signaux();
		printf("Server started correctly\n");
		while(1){
			
			socket_client = accept(socket_serveur, NULL, NULL);
			if(socket_client == -1) {
				perror("accept");
			}
			else {
				printf("CONNEXION ACCEPTEE\n");
			
				fils = fork();
				if(fils == 0) {				//DEBUT DU FILS
				
					sleep(1);
					write(socket_client, message_bienvenue, strlen(message_bienvenue));
					fclient = fdopen(socket_client, "w+");
					while(recois_requete(buf, sizeof(buf)/sizeof(buf[0]), fclient)){
						//printf("message reçu :-D\n");					
						//envoie_reponse(fclient, buf);
						int analyse_ligne1 = analyse_premiere_ligne(buf);
						printf("résultat analyse premiere ligne : %d\n", analyse_ligne1);
						//sleep(1);
						//printf("			message envoyé\n");
					}
				}					//FIN DU FILS
			}
			
		}

	} else {
		printf("fail\n");
		return -1;
	}
	return 0;
}




