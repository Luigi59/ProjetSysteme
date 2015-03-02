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

					fclient = fdopen(socket_client, "w+");
					
					while(recois_requete(buf, sizeof(buf)/sizeof(buf[0]), fclient)){

						char *url = malloc(256);
						analyse_ligne1 = analyse_premiere_ligne(buf, url);
						ignore_entete(fclient);
						printf("\trésultat analyse première ligne : %d\n", analyse_ligne1);
						if(analyse_ligne1 == -1) {
							fprintf(fclient, "HTTP/1.1 400 Bad Request\r\n");
							fprintf(fclient, "Connection: close\r\n");
							fprintf(fclient, "Content-Length: 17\r\n");
							fprintf(fclient, "\r\n");
							fprintf(fclient, "400 Bad Request\r\n");
						}
						else if(strcmp(url, "/") == 0) {
							fprintf(fclient, "%s", message_bienvenue);
							printf("HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Length: 8\r\n\r\n200 OK\r\n");
						}
						else {
							printf("NOT FOUND\n");
							fprintf(fclient, "HTTP/1.1 404 Not Found\r\n");
							fprintf(fclient, "Connection: close\r\n");
							fprintf(fclient, "Content-Length: 15\r\n");
							fprintf(fclient, "\r\n");
							fprintf(fclient, "404 Not Found\r\n");
						}
						exit(0);
					}
				}					//FIN DU FILS
				else {
					close(socket_client);
				}
			}
			
		}

	} else {
		printf("fail\n");
		return -1;
	}
	return 0;
}




