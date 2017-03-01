/**
 * \file common_utils.h
 * \brief Projet de MCS3 2016 2017
 * \author BENCHIHA - SALENGRO - LEFORT
 * \version 1
 * \date DECEMBRE - JANVIER
 *
 * Fichier gérant les fonctions utilisées par le client et le serveur
 *
 */

#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "../config/config.h"

#define MESSAGE_SIZE 82 //Taille max d'un message


/**
 * \fn void send_prepared_msg( char* msg, int socket);
 * \brief Fonction d'envoie d'un message à une socket
 * \param *msg
 * \param socket
 * \return VOID
 */
void send_prepared_msg( char* msg, int socket);


/**
 * \fn void send_msg( int msg_code, const char* payload, int socket);
 * \brief Fonction d'envoie d'un message à une socket avec le code du message de type string
 * \param msg_code
 * \param *payload
 * \param socket
 * \return VOID
 */
void send_msg( int msg_code, const char* payload, int socket);


/**
 * \fn void send_light_msg( int msg_code, int socket);
 * \brief Fonction d'envoie d'un message à une socket sans le détaille
 * \param msg_code
 * \param socket
 * \return VOID
 */
void send_light_msg( int msg_code, int socket);

/**
 * \fn void send_int_msg( int msg_code, int payload, int socket);
 * \brief Fonction d'envoie d'un message à une socket avec un code de type int
 * \param msg_code
 * \param payload
 * \param socket
 * \return VOID
 */
void send_int_msg( int msg_code, int payload, int socket);


/**
 * \fn int extract_msg_code( char** msg );
 * \brief Fonction qui prend le code message
 * \param **msg
 * \return int
 */
int extract_msg_code( char** msg );


/**
 * \fn int decode_msg_payload( char** raw_payload, int* decoded_payload, int max_elements);
 * \brief Décoder le message passé en paramètre des fonctions.
 * \param **raw_payload
 * \param *decode_payload
 * \param max_elements
 * \return int
 */
int decode_msg_payload( char** raw_payload, int* decoded_payload, int max_elements);

#endif
