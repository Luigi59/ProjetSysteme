#ifndef __SOCKET_H__
#define __SOCKET_H__

//int accepter(int socket_serveur);
char * recois_requete(char * buffer , int size , FILE * stream);
void traitement_signal(int sig);
void initialiser_signaux(void);
int ecouter(int socket_serveur);
int config_socket(int socket_serveur);
int creer_serveur(int port);

#endif
