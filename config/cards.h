/**
 * \file cards.h
 * \brief Projet de MCS3 2016 2017
 * \author BENCHIHA - SALENGRO - LEFORT
 * \version 1
 * \date DECEMBRE - JANVIER
 *
 * Fichier gérant les cartes du jeu
 *
 */
#ifndef CARDS_H
#define CARDS_H

#define DECK_SIZE 52
#define DECK_SIZE_WHITE 52

/**
 * Prend le nom des carte
 */
char* get_card_name(int card);

/**
 * Prend le nom des carte
 */
char* get_card_white_name(int card);

/**
 * Chaque carte à des points
 */
int get_card_points(int card);

#endif
