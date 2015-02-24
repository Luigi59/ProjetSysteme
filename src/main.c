#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include "socket.h"

int main(int argc, char **argv) {

	const char *message_bienvenue = "\n<kiwi> Bonjour, bienvenue sur notre serveur\n<kiwi> Nous sommes tres heureux de vous recevoir\n<kiwi> J'adore les sucettes, et les gros calins !\n<kiwi> Je suis Charlie\n<kiwi> C'est l'histoire d'un mec qui rentre dans un cafe, et PLOUF !\n<kiwi> Tu connais la blague a deux balles ? PAN PAN !\n<kiwi> J'ai envie de me suicider parce que c'est cool la mort....... ouais c'est trop cool\n<kiwi> Vive les lamasticots !!!\n\n";
	int socket_serveur = creer_serveur(8080);
	int socket_client;
	int fils;
	char buf[256];
	int taille;
	FILE * fclient;
	int analyse_ligne1;
	int ligne1_lue = 0;
	const char *requete_OK = "\r\nHTTP/1.1 200 OK\r\nConnection: close\r\nContent-Length: 8\r\n\r\n200 OK\r\n\r\n";

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
					//write(socket_client, message_bienvenue, strlen(message_bienvenue));
					fclient = fdopen(socket_client, "w+");
					
					while(recois_requete(buf, sizeof(buf)/sizeof(buf[0]), fclient)){
						//printf("message reçu :-D\n");					
						//envoie_reponse(fclient, buf);
						analyse_ligne1 = analyse_premiere_ligne(buf);
						ignore_entete(fclient);
						printf("\trésultat analyse première ligne : %d\n", analyse_ligne1);
						if(analyse_ligne1 == -1) {
							printf("\r\nHTTP/1.1 400 Bad Request\r\n");
							printf("Connection: close\r\n");
							printf("Content-Length: 17\r\n");
							printf("\r\n");
							printf("400 Bad Request\r\n\r\n");
						}
						else {
							fprintf(fclient, "%s", message_bienvenue);
							printf("%s", requete_OK);
						}
						exit(1);
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




