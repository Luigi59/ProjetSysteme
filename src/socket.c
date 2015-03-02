#include <stdio.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

const char *message_bienvenue = "\n<kiwi> Bonjour, bienvenue sur notre serveur\n<kiwi> Nous sommes tres heureux de vous recevoir\n<kiwi> J'adore les sucettes, et les gros calins !\n<kiwi> Je suis Charlie\n<kiwi> C'est l'histoire d'un mec qui rentre dans un cafe, et PLOUF !\n<kiwi> Tu connais la blague a deux balles ? PAN PAN !\n<kiwi> J'ai envie de me suicider parce que c'est cool la mort....... ouais c'est trop cool\n<kiwi> Vive les lamasticots !!!\n\n";

int socket_client;
int fils;
int taille;
FILE * fclient;
int analyse_ligne1;


void ignore_entete(FILE * fclient) {
	char * buf = malloc(256);
	fgets(buf, 256, fclient);
	while (strncmp(buf, "\r\n", 2) != 0 && strncmp(buf, "\n", 1) != 0) {
		fflush(stdout); /* on vide le flux */
        fgets(buf, 256, fclient);
	}
}

int premier_mot_GET(const char * buf) {
	if(strncmp(buf, "GET ", 4) != 0)
		return -1;
	return 0;
}

int trois_mots(const char * buf, char * url) {
	int nb_espace = 0;
	int position_espace1 = 0;
	int position_espace2;
	int i;
	for(i=0; i<strlen(buf); ++i) {
		if(buf[i] == ' ') {
			++nb_espace;
			if(nb_espace > 2)
				return -1;
			if(position_espace1 == 0)
				position_espace1 = i;
			else
				position_espace2 = i;
		}
	}
	if(position_espace2 - position_espace1 < 2)
		return -1;
	char tmp[256];
	for(i = 0; i<256; ++i) 
		tmp[i] = '\0';

	strncpy(url, &buf[position_espace1+1], position_espace2 - position_espace1 - 1);
	url = tmp;
	printf("%s\n", url);
	return 0;
}

int troisieme_mot_HTTP(const char * buf) {
	int i = 0;
	int nb_espace = 0;
	while(nb_espace < 2){
        	if(buf[i] == ' ')
			++nb_espace;
        	++i;
	}
	if(strncmp(&buf[i], "HTTP/1.1", 8) != 0 && strncmp(&buf[i], "HTTP/1.0", 8) != 0)
		return -1;
	return 0;
}

int analyse_premiere_ligne(const char * buf, char * url) {
	if(buf[0] == ' ' || premier_mot_GET(buf)!=0 || trois_mots(buf, url)!=0 || troisieme_mot_HTTP(buf)!=0)
		return -1;
	return 0;
}

/* serveur ---> client */
void envoie_reponse(FILE * fclient, const char * phrase) {
	fprintf(fclient, "<kiwi> %s", phrase);
}

/* client ---> serveur */
char * recois_requete(char * buffer , int size , FILE * stream) {

	/* Réinitialisation du buffer */
	int i;
	for(i = 0; i<size; ++i) {
		buffer[i] = '\0';
	}

	fgets(buffer, size, stream); /* Lecture des donnees envoyees par le client */
	printf("<kiwi> %s", buffer);
	fflush(stdout); /* Vider les buffers du flux */

	/* Quitte si l'on envoie une donnee vide */
	if(buffer[0] == '\0')
		exit(1);

	return buffer;
}

void traitement_signal(int sig){


	int statut;
/*	printf("Signal %d reçu \n", sig);
	if(waitpid(sig) == -1) {
		perror("zombie");
		return;
	}
	*/
	if(sig==SIGCHLD)
		while(waitpid(-1,&statut,WNOHANG)>0)
		{
			if (WIFSIGNALED(statut))
			{
				printf("Terminé par sigal %d\n", WTERMSIG(statut));
			}
		}

}

void initialiser_signaux(void){
	struct sigaction sa;
	sa.sa_handler = traitement_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART ;
	if(sigaction(SIGCHLD, &sa, NULL) == -1){
		perror("sigaction(SIGCHLD)");
	}
}

int ecouter(int socket_serveur){
	if(listen(socket_serveur, 10) == -1){
		perror("ecoute socket");
		return -1;
	}
	return socket_serveur;
}


int config_socket(int socket_serveur){
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET; /* Socket ipv4 */
	saddr.sin_port = htons(8080); /* Port d ’ écoute */
	saddr.sin_addr.s_addr = INADDR_ANY; /* écoute sur toutes les interfaces */
	if(bind(socket_serveur, (struct sockaddr *)&saddr, sizeof(saddr)) == -1){
		perror("bind socket");
		return -1;
	}
	return ecouter(socket_serveur);
}


int creer_serveur(int port){
	int socket_serveur = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_serveur == -1){
		perror("socket serveur");
		return -1;
	}
	int optval = 1;
	if (setsockopt(socket_serveur, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1)
		perror ("Can not set SO_REUSEADDR option");
	return config_socket(socket_serveur); 	
}


void accept_client(int socket_serveur) {

	socket_client = accept(socket_serveur, NULL, NULL);
	if(socket_client == -1) {
		perror("accept");
	}
	else {
		printf("CONNEXION ACCEPTEE\n");
		fils = fork();
		if(fils == 0) {				//DEBUT DU FILS

			fclient = fdopen(socket_client, "w+");
			char buf[256];

			while(recois_requete(buf, sizeof(buf)/sizeof(buf[0]), fclient)){

				printf("%s\n", buf);
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
