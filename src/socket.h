#ifndef __SOCKET_H__
#define __SOCKET_H__

void send_status(FILE * client, int code, const char * reason_phrase);
void ignore_entete(FILE * fclient);
int premier_mot_GET(const char * buf);
int trois_mots(const char * buf, char * url);
int troisieme_mot_HTTP(const char * buf);
int analyse_premiere_ligne(const char * buf, char * url);
void envoie_reponse(FILE * fclient, const char * phrase);
char * recois_requete(char * buffer , int size , FILE * stream);
void traitement_signal(int sig);
void initialiser_signaux(void);
int ecouter(int socket_serveur);
int config_socket(int socket_serveur);
int creer_serveur(int port);

#endif
