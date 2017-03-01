/**
 * \file common_utils.c
 * \brief Projet de MCS3 2016 2017
 * \author BENCHIHA - SALENGRO - LEFORT
 * \version 1
 * \date DECEMBRE - JANVIER
 *
 * Fichier gérant les fonctions utilisées par le client et le serveur MSG CODE dans config
 *
 */
#include "common_utils.h"


/**
 * \fn void send_prepared_msg( char* msg, int socket);
 * \brief Fonction d'envoie d'un message à une socket
 * \param *msg
 * \param socket
 * \return VOID
 */
void send_prepared_msg(char* pmsg, int socket) {
    if (send(socket, pmsg, MESSAGE_SIZE, 0) == -1) {
        perror("Envoie du message au serveur raté\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * \fn void send_msg( int msg_code, const char* payload, int socket);
 * \brief Fonction d'envoie d'un message à une socket avec le code du message de type string
 * \param msg_code
 * \param *payload
 * \param socket
 * \return VOID
 */
void send_msg(int msg_code, const char* payload, int socket) {
    printf("fonction send_msg : MESSAGE CODE : %d ,\t PAYLOAD (BODY): %s,\t SOCKET : %d\n", msg_code, payload, socket);
    char msg[MESSAGE_SIZE];
    sprintf(msg, "%d %s", msg_code, payload);
    send_prepared_msg(msg, socket);
}

/**
 * \fn void send_light_msg( int msg_code, int socket);
 * \brief Fonction d'envoie d'un message à une socket sans le détaille
 * \param msg_code
 * \param socket
 * \return VOID
 */
void send_light_msg(int msg_code, int socket) {
    printf("fonction send_light_msg : MESSAGE CODE : %d,\t SOCKET : %d\n", msg_code, socket);
    char msg[MESSAGE_SIZE];
    sprintf(msg, "%d", msg_code);
    send_prepared_msg(msg, socket);
}

/**
 * \fn void send_int_msg( int msg_code, int payload, int socket);
 * \brief Fonction d'envoie d'un message à une socket avec un code de type int
 * \param msg_code
 * \param payload
 * \param socket
 * \return VOID
 */
void send_int_msg(int msg_code, int payload, int socket) {
    printf("fonction send_int_msg : MESSAGE CODE : %d,\t PAYLOAD : %d,\t SOCKET : %d\n", msg_code, payload, socket);
    char msg[MESSAGE_SIZE];
    sprintf(msg, "%d %d", msg_code, payload);
    send_prepared_msg(msg, socket);
}

/**
 * \fn int extract_msg_code( char** msg );
 * \brief Fonction qui prend le code message
 * \param **msg
 * \return int
 */
int extract_msg_code(char** msg) {
    //convertir un string en int
    return atoi(strtok_r(*msg, " ", msg));
}

/**
 * \fn int decode_msg_payload( char** raw_payload, int* decoded_payload, int max_elements);
 * \brief Décoder le message passé en paramètre des fonctions.
 * \param **raw_payload
 * \param *decode_payload
 * \param max_elements
 * \return int
 */
int decode_msg_payload(char** raw_payload, int* decoded_payload, int max_elements) {
    int i;
    for (i = 0; i < max_elements; i++) {
        char* token = strtok_r(*raw_payload, " ", raw_payload );
        if (token == NULL) {
            return i;
        }
        *(decoded_payload + i) = atoi(token);
    }
    return i;
}

