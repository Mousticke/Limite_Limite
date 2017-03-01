/**
 * \file cards.c
 * \brief Projet de MCS3 2016 2017
 * \author BENCHIHA - SALENGRO - LEFORT
 * \version 1
 * \date DECEMBRE - JANVIER
 *
 * Fichier gérant les cartes du jeu
 *
 */
#include "cards.h"


char* card_black_names[] = {
    " Adjectif 1 ___________ ",
    " Adjectif 2 ___________ ",
    " Adjectif 3 ___________ ",
    " Adjectif 4 ___________ ",
    " Adjectif 5 ___________ ",
    " Adjectif 6 ___________ ",
    " Adjectif 7 ___________ ",
    " Adjectif 8 ___________ ",
    " Adjectif 9 ___________ ",
    " Adjectif 10 ___________ ",
    " Adjectif 11 ___________ ",
    " Adjectif 12 ___________ ",
    " Adjectif 13 ___________ ",
    " Adjectif 14 ___________ ",
    " Adjectif 15  ___________ ",
    " Adjectif 16  ___________ ",
    " Adjectif 17 ___________ ",
    " Adjectif 18 ___________ ",
    " Adjectif 19 ___________ ",
    " Adjectif 20 ___________ ",
    " Adjectif 21 ___________ ",
    " Adjectif 22 ___________ ",
    " Adjectif 23 ___________ ",
    " Adjectif 24 ___________ ",
    " Adjectif 25 ___________ ",
    " Adjectif 26 ___________ ",
    " Adjectif 27 ___________ ",
    " Adjectif 28 ___________ ",
    " Adjectif 29 ___________ ",
    " Adjectif 30 ___________ ",
    " Adjectif 31 ___________ ",
    " Adjectif 32 ___________ ",
    " Adjectif 33 ___________ ",
    " Adjectif 34 ___________ ",
    " Adjectif 35 ___________ ",
    " Adjectif 36 ___________ ",
    " Adjectif 37 ___________ ",
    " Adjectif 38 ___________ ",
    " Adjectif 39 ___________ ",
    " Adjectif 40 ___________ ",
    " Adjectif 41 ___________ ",
    " Adjectif 42 ___________ ",
    " Adjectif 43 ___________ ",
    " Adjectif 44 ___________ ",
    " Adjectif 45 ___________ ",
    " Adjectif 46 ___________ ",
    " Adjectif 47 ___________ ",
    " Adjectif 48 ___________ ",
    " Adjectif 49 ___________ ",
    " Adjectif 50 ___________ ",
    " Adjectif 51 ___________ ",
    " Adjectif 52 ___________ "
};

char* card_white_names[] = {
    " Grâce à________ j\'ai réussi mon CTP d\'UOD",
    " ____a condamné les toilettes en 300",
    " A chaque rentrée _____ parle aux nouvelles IG2iennes",
    " Pour notre PIND nous réalison un _____ .",
    " Pour réussir mon semestre j\'ai ________",
    " Grâce aux cours de communication j\'ai appris à _____",
    " Mme Delattre s\'est trouvée une novelle lubie ____",
    " Pour ne pas aller au rattrapages j\'ai _____",
    " ______presque aussi bien que la NDL5 !",
    " La connexion de l\'IG2I est aussi faible que _______ .",
    " Pour le prochain cours avec Mr Delame, nous allons regarder un film sur _______",
    " Les apprentis ne font ça que pour _______ .",
    " _______ c\'est très Data-vision ",
    " Ce qui m\'a donné envie d\'aller à l\'ig2i c\'est _______ ",
    " Le torseur dynamique est utile lors des ______ ",
    " Seuls les membres du BDE peuvent _______.",
    " Pour rembourser Les fournisseurs de let\'sZic nous avons ________",
    " Les cours de Delame ? Même _____ est plus interessant.",
    " _______ a fait un meilleur limousin de Mousticke à la JDN.",
    " _______ est plus poilu que les fesses de Mousticke.",
    " _______ est aussi rapide qu’un PC de l\'IG2I.",
    " C\'est homme sont des _____",
    " C\'est à cause de ____ que je n\'ai pas réussi mon exam",
};

/**
 * \fn char* get_card_name(int card)
 * \brief Fonction donnant le nom de la carte noire
 * \param card
 * \return char*
 */
char* get_card_name(int card) {
    return card_black_names[card];
}

/**
 * \fn char* get_card_white_name(int card)
 * \brief Fonction donnant le nom de la carte blanche
 * \param card
 * \return char*
 */
char* get_card_white_name(int card) {
    return card_white_names[card];
}

/**
 * \fn char* get_card_name(int card)
 * \brief Fonction donnant les points d'une cartes
 * \param card
 * \return int
 */
int get_card_points(int card) {
    if (card > 47) {
        return 1;
    }
    return (card / 4) + 2;
}
