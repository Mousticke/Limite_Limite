/**
 * \file server_utils.c
 * \brief Projet de MCS3 2016 2017
 * \author BENCHIHA - SALENGRO - LEFORT
 * \version 1
 * \date DECEMBRE - JANVIER
 *
 * Fichier gérant les fonctions utilisées par le serveur
 *
 */
#include "server_utils.h"

/**
 * \fn broadcast(int msg_code, char* payload, player* recipients, int rcp_count)
 * \brief Diffusion du message envoyé au serveur
 * \param msg_code
 * \param *payload
 * \param *recipients
 * \param rcp_count
 * \return VOID
 */
void broadcast(int msg_code, char* payload, player* recipients, int rcp_count) {
    printf("\n");
    printf("-------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("FONCTION CONCERNEE\t | \tCODE DU MESSAGE\t | \tPAYLOAD\t | \tSOCKET\t | \tPSEUDO\t | \tCARTE DU JOUEUR\t | \tBOOL isempty\t | \tRCP_COUNT\t \n");
    printf("-------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("fonction broadcast\t | \t%d\t | \t%s\t | \t%d\t | \t%s\t | \t%d\t | \t%d\t | \t%d\t \n", msg_code, payload, recipients->socket, recipients->nickname, recipients->played_card, recipients->isempty, rcp_count );
    printf("-------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    char msg[MESSAGE_SIZE];
    sprintf(msg, "%d %s", msg_code, payload);
    int i;
    for (i = 0; i < rcp_count; i++) {
        if (recipients[i].socket != 0) {
            send_prepared_msg(msg, recipients[i].socket);
        }
    }
    printf("\n");
}

/**
 * \fn broadcast_light(int msg_code, player* recipients, int rcp_count)
 * \brief Diffusion du message envoyé au serveur sans le corps du message
 * \param msg_code
 * \param *payload
 * \param *recipients
 * \param rcp_count
 * \return VOID
 */
void broadcast_light(int msg_code, player* recipients, int rcp_count) {
    printf("\n");
    printf("-------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("FONCTION CONCERNEE\t | \tCODE DU MESSAGE\t | \t-----\t | \tSOCKET\t | \tPSEUDO\t | \tCARTE DU JOUEUR\t | \tBOOL isempty\t | \tRCP_COUNT\t \n");
    printf("-------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("fonction broadcast light\t | \t%d\t\t | \t-----\t | \t%d\t | \t%s\t | \t%d\t\t | \t%d\t\t | \t%d\t \n", msg_code, recipients->socket, recipients->nickname, recipients->played_card, recipients->isempty, rcp_count );
    printf("-------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    char msg[MESSAGE_SIZE];
    sprintf(msg, "%d", msg_code);
    int i;
    for (i = 0; i < rcp_count; i++) {
        if (recipients[i].socket != 0) {
            send_prepared_msg(msg, recipients[i].socket);
        }
    }
    printf("\n");
}

/**
 * \fn extract_player_nickname(char** msg, char* nickname)
 * \brief Fonction qui extrait le nom du joueur envoyé au serveur
 * \param signum numéro du signal
 * \return VOID
 */
void extract_player_nickname(char** msg, char* nickname) {
    printf("\n");
    printf("-------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("FONCTION CONCERNEE\t | \tMESSAGE\t | \tPSEUDO\t | \t---\t | \t----\t | \t---\t | \t----\t | \t----\t \n");
    printf("-------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("extract player nickname\t | \t%s\t |  \t%s\t \n", *msg, nickname );
    sprintf(nickname,"%s", strtok_r(*msg, " ", msg));
    printf("\n");
}

/**
 * \fn rand_range(int upper_limit)
 * \brief Fonction qui génère un nombre aléatoire dans le max de upperlimite
 * \param upper_limit
 * \return int
 */
int rand_range(int upper_limit) {
    return (int) (( (double) upper_limit / RAND_MAX) * rand());
}

/**
 * \fn rand_range(int upper_limit)
 * \brief Fonction qui vérifie si une carte est présente dans le tableau
 * \param *haystack
 * \param needle
 * \param length
 * \return bool
 */
bool array_contains(int* haystack, int needle, int length) {
    int* array_ptr = haystack;
    for (; (array_ptr - haystack) < length; array_ptr++) {
        if (*array_ptr == needle) {
            return TRUE;
        }
    }
    return FALSE;
}
