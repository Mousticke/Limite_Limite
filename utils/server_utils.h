/**
 * \file server_utils.h
 * \brief Projet de MCS3 2016 2017
 * \author BENCHIHA - SALENGRO - LEFORT
 * \version 1
 * \date DECEMBRE - JANVIER
 *
 * Fichier gérant les fonctions utilisées par le serveur
 *
 */
#include "../config/config.h"
#include "../utils/common_utils.h"

/**
 * \struct player
 * \brief Objet chaîne de caractères.
 *
 * player est une structure définissant un joueurs connecté
 */

typedef struct player {
    int socket;
    char nickname[NAMESIZE];
    int played_card;
    bool isempty;
} player;

void broadcast( int msg_code, char* payload, player* recipients, int rcp_count ); //sends a message (code and body) to all players
void broadcast_light( int msg_code, player* recipients, int rcp_count ); //send a message code to all player
void extract_player_nickname( char** msg, char* nickname);
int rand_range( int upper_limit );
bool array_contains( int* haystack, int needle, int length );
