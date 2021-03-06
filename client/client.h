/**
 * \file client.c
 * \brief Projet de MCS3 2016 2017
 * \author BENCHIHA - SALENGRO - LEFORT
 * \version 1
 * \date DECEMBRE - JANVIER
 *
 * Fichier gérant le client du projet
 *
 */
#ifndef CLIENTH_H
#define CLIENTH_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <time.h>
#include <signal.h>
#include "../config/config.h"
#include "../utils/common_utils.h"
#include "../config/cards.h"

#define PORT PORT_DIMOV
#endif

void interrupt_handler(int);
void disconnect(bool information);
void refill();
void clear_cards();
void print_cards();
int calculate_score();
void receive_message(int clientSocket,char** name);
void createNickname(char *name);
void connectToServer(int *clientSocket,char* serverIP,struct hostent *he,struct sockaddr_in *serverAddress);
bool fdp_is_valid(int fdp);
