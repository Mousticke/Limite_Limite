/**
 * \file server.c
 * \brief Projet de MCS3 2016 2017
 * \author BENCHIHA - SALENGRO - LEFORT
 * \version 1
 * \date DECEMBRE - JANVIER
 * Fichier gérant les constantes propore au jeu
 */

#ifndef _CONFIG_H
#define _CONFIG_H
typedef int bool; //type boolean
#define TRUE 1
#define FALSE 0

//message codes
//server -> client
#define WAIT 0 //server -> client
#define REFUSE 1 //server -> client
#define DEAL 4 //server -> client
#define ASK 5 //server -> client
#define GIVE 7 //server -> client
#define ROUND 9 //server -> client
#define SCORES 11 //server -> client
#define WINNER 12 // GAME?
#define DEFINE_CARD 13 //server->client

//client -> server
#define NICKNAME 2 //client -> server
#define PLAY 6 //client -> server
#define EMPTY 8 //client -> server
#define SCORE 10 //client -> server

//common
#define DISCONNECT 3 

//ports
#define PORT_DIMOV 17626
#define PORT_DRAGOMIR 18206

#define MAX_PLAYERS 4
#define NAMESIZE 20
#define MESSAGE_SIZE 82


#define NICKNAMES_KEY "nicknames"
#endif
