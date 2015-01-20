#include <stdio.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>

int main(int argc, char **argv)
{
	/* Arnold Robbins in the LJ of February ’95 , describing RCS */

	if (argc > 1 && strcmp(argv[1], "-advice") == 0)
	{
		printf ("Don’t Panic !\n");
		return 42;
	}
	printf ("Need an advice ?\n");
	return 0;
}


int create_socket(){
	int socket_serveur = socket(AF_INET, SOCK_STREAM, 0);

	if(socket_serveur == -1){
		perror("socket serveur");
		return -1;
	}
	return config_socket(socket_serveur); 	
}

int config_socket(int socket_serveur){
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET; /* Socket ipv4 */
	saddr.sin_port = htons(8000); /* Port d ’ écoute */
	saddr.sin_addr.s_addr = INADDR_ANY; /* écoute sur toutes les interfaces */

	if(bind(socket_serveur, (struct sockaddr *)&saddr, sizeof(saddr)) == -1){
		perror("bind socket");
		return -1;
	}
	return ecouter(socket_serveur);
}

int ecouter(int socket_serveur){
	if(listen(socket_serveur, 10) == -1){
		perror("ecoute socket");
		return -1;
	}
	return accepter(socket_serveur);
}

int accepter(int socket_serveur){
	int socket_client;
	socket_client = accept(socket_serveur, NULL, NULL);
	if(socket_client == -1){
		perror("accept socket");
	}
	const char *message_bienvenue = "Bonjour, bienvenue sur mon serveur\n";
	write(socket_client, message_bienvenue, strlen( message_bienvenue));
}
