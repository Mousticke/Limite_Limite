/**
 * \file server.h
 * \brief Projet de MCS3 2016 2017
 * \author BENCHIHA - SALENGRO - LEFORT
 * \version 1
 * \date DECEMBRE - JANVIER
 *
 * Fichier gérant les variables du serveur
 *
 */

#ifndef SERVERH_H
#define SERVERH_H
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include "../utils/common_utils.h"
#include "../utils/server_utils.h"
#include "../config/cards.h"

#define MIN_PLAYERS 2
#define PORT PORT_DIMOV
#define BUFFER_SIZE 1024
#define BACKLOG 5
#define COUNTDOWN 10 //30 seconds d'attente
#endif

typedef void (*fct_ptr)( );

void init_server(int*, struct sockaddr_in*); //création et bin du socket
void alarm_handler(int); //alarm et time out
void interrupt_handler(int); //fermer le serveur au signal SIGINT 
void shutdown_socket(int); //fermer une socket
void shutdown_server(); //fermer le serveur
void add_client(int, struct sockaddr_in*); //ajouter un client
void add_player(int); //confirmer la connexion et notifer le client
void remove_player(player*, int, bool); //retirer un joueur
void refuse_connection(int); //refuser un client de se connecter
bool receive_msg(char*, int); //gérer les messages
void clear_lobby(); //informer que le jeu est terminé
void add_nickname(int, char**); //enregistrer le pseudo
void start_game(); //commencer le jeu
void deal_cards(); //melanger le jeu et donner les cartes
void start_round(); //commencer un round
void receive_card(int, char**); //recevoir la carte jouer par le joueur
void end_round(int, char**); //terminer un round
void update_score(int, char**); //mettre à jour le score
void create_nicknames_shared_memory(char* nickname);//création d'un mémoire partagé
void end_game();
int find_index(player*, int);
