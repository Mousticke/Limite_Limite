/**
 * \file server.c
 * \brief Projet de MCS3 2016 2017
 * \author BENCHIHA - SALENGRO - LEFORT
 * \version 1
 * \date DECEMBRE - JANVIER
 *
 * Fichier gérant le serveur du projet
 *
 */
#include "server.h"

int cl_count;
int scores_joueurs[MAX_PLAYERS];
player joueurs[MAX_PLAYERS];
bool jeu_en_cours;
bool temps_termine; //Terminé
bool serveur_en_cours; //server en cours
bool fin_du_tour; //On ajouer le tour
bool sigempty; //un joueur n'a plus de carte
bool fin_du_round; //le tour est terminé

fct_ptr dispatcher[] = { add_player, refuse_connection, add_nickname, 0, 0, 0, receive_card, 0, end_round, 0, update_score }; //le but de ceci était d'alléger la fonction main en évitant une grande section de if-else ou bien un switch-case.

int main(int argc, char** argv) {
    int server_socket, max_fd, select_res, i, max_rounds, round_nb = 0;
    struct sockaddr_in my_addr, cl_addr;
    struct timeval timeout = {0, 15000}; //30 milliseconds
    fd_set fds;

    if (argc != 2) {
        fprintf(stderr, "Manque des arguments! Donnez le nombre de round\n");
        return EXIT_FAILURE;
    }
    max_rounds = atoi(argv[1]);

    srand(time(NULL));

    //INITIALISE LES SOCKTES
    for (i = 0; i < MAX_PLAYERS; i++) {
        joueurs[i].socket = 0;
    }

    cl_count = 0;
    jeu_en_cours = FALSE;
    temps_termine = FALSE;
    serveur_en_cours = TRUE;

    //SIGNAUX
    struct sigaction alarm, interrupt;
    memset(&alarm, 0, sizeof(alarm));
    memset(&interrupt, 0, sizeof(interrupt));
    alarm.sa_handler = &alarm_handler;
    interrupt.sa_handler = &interrupt_handler;
    sigaction(SIGALRM, &alarm, NULL);
    sigaction(SIGINT, &interrupt, NULL);

    init_server(&server_socket, &my_addr);
    char message[MESSAGE_SIZE];
    char* msg;
    
    //SERVEUR EN COURS
    while (serveur_en_cours) {
        FD_ZERO(&fds);
        FD_SET(server_socket, &fds);
        max_fd = server_socket + 1;
        int i;
        for (i = 0; i < MAX_PLAYERS; i++) {
            if (joueurs[i].socket > 0) {
                FD_SET(joueurs[i].socket, &fds);
            }
            if (joueurs[i].socket >= max_fd) {
                max_fd = joueurs[i].socket+1;
            }
        }
        if ((select_res = select(max_fd, &fds, NULL, NULL, &timeout)) < 0) {
            if (errno != EINTR) {
                shutdown_server();
                return EXIT_FAILURE;
            }
        }

        if (select_res > 0) {
            if (FD_ISSET(server_socket, &fds)) {
                add_client(server_socket, &cl_addr);
            }
            for (i = 0; i < cl_count; i++) {
                if (FD_ISSET(joueurs[i].socket, &fds)) {
                    msg = message;
                    if (receive_msg(msg, joueurs[i].socket)) {
                        int msg_code = extract_msg_code(&msg);
                            dispatcher[msg_code] (joueurs[i].socket, &msg);
                    } else {
                        remove_player(joueurs, i, FALSE);
                    }
                }
            }
        }
        if (jeu_en_cours) {
            if (fin_du_tour && !sigempty) {
                fin_du_tour = FALSE;
                broadcast_light(ASK, joueurs, cl_count);
            }
            if (fin_du_round) {
                round_nb++;
                if (round_nb == max_rounds) {
                    end_game();
                    round_nb = 0;
                    printf("Game over\n");
                } else {
                    start_round();
                }
            }
            }
    }
    shutdown_socket(server_socket);
    return EXIT_SUCCESS;
}

/**
 * \fn void alarm_handler(int signum)
 * \brief Fonction signal qui regarde s'il y a au moins deux joueurs. Sinon ça quitte le jeu.
 * \param signum numéro du signal
 * \return VOID
 */
void alarm_handler(int signum) {
    printf("Déclangement d'un signal !\n");
    if (signum == SIGALRM) {
        if (cl_count < 2) {
            clear_lobby();
        } else {
            start_game();
        }
        temps_termine = TRUE;
    }
}

/**
 * \fn int find_index(player* joueurs, int socket)
 * \brief Fonction d'indexage des joueurs
 * \param joueurs structure de données défini dans server_utils.h
 * \param socket
 * \return int
 */
int find_index(player* joueurs, int socket) {
    int i;
    for (i = 0; i < MAX_PLAYERS; i++) {
        if (joueurs[i].socket == socket) {
            return i;
        }
    }
    return -1;
}

/**
 * \fn void interrupt_handler(int signum)
 * \brief Fonction de signal d'interruption
 * \param signum 
 * \return VOID
 */
void interrupt_handler(int signum) {
    if (signum == SIGINT) {
        shutdown_server();
    }
}

/**
 * \fn void init_server(int *server_socket,struct sockaddr_in *my_addr)
 * \brief Fonction d'initialisation du serveur
 * \param *server_socket
 * \param *my_addr
 * \return VOID
 */
void init_server(int *server_socket,struct sockaddr_in *my_addr) {

    if ((*server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Problème de création de la socket");
        exit(EXIT_FAILURE);
    }

    memset(my_addr, 1, sizeof(struct sockaddr_in));
    my_addr->sin_family = AF_INET;
    my_addr->sin_port = htons(PORT);
    my_addr->sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(*server_socket, (struct sockaddr *)my_addr, sizeof(*my_addr)) == -1) {
        perror("Problème au niveau du bind");
        exit(EXIT_FAILURE);
    }

    if (listen(*server_socket, BACKLOG)) {
        perror("Problème sur le port d'écoute");
        exit(EXIT_FAILURE);
    }
}

/**
 * \fn add_client(int server_socket, struct sockaddr_in *cl_addr) 
 * \brief Fonction d'ajout d'un client au serveur. Si le jeu est en cours, on refuse la connexion S'il y a 4 personnes on ferme le lobby
 * \param server_socket
 * \param *cl_addr
 * \return VOID
 */
void add_client(int server_socket, struct sockaddr_in *cl_addr) {
    int new_cl_socket;
    int cl_addr_length = sizeof(struct sockaddr_in);
    if ((new_cl_socket = accept(server_socket, (struct sockaddr *)cl_addr, (socklen_t*) &cl_addr_length)) < 0) {
        perror("Erreur de connexion");
        exit(EXIT_FAILURE);
    } else {
        if (jeu_en_cours || cl_count == MAX_PLAYERS) {
            refuse_connection(new_cl_socket);
        } else {
            add_player(new_cl_socket);
        }
    }
}

/**
 * \fn void add_player(int socket) 
 * \brief Fonction qio ajoute un joueur
 * \param socket
 * \return VOID
 */
void add_player(int socket) {
    joueurs[cl_count].played_card = -1;
    joueurs[cl_count].isempty = FALSE;
    joueurs[cl_count++].socket = socket;
    send_int_msg(WAIT, COUNTDOWN, socket);
    if (cl_count == 1) {
        alarm(COUNTDOWN);
    }
}


/**
 * \fn void end_game()
 * \brief Fonction de fin de jeu
 * \param information boolean qui va vérifier si on s'est déconnecté ou pas.
 * \return VOID
 */
void end_game() {
    printf("Score du joueur : %d\n", cl_count);
    int i, windex, highscore = 0;
    for (i = 0; i < cl_count; i++) {
        if (scores_joueurs[i] >= highscore) {
            highscore = scores_joueurs[i];
            windex = i;
        }
    }
    printf("Le gagnant est : %s avec %d points\n", joueurs[windex].nickname, scores_joueurs[windex]);
    send_light_msg(WINNER, joueurs[windex].socket);
    clear_lobby();
}



/**
 * \fn void remove_player(player* joueurs, int index, bool sockopen)
 * \brief Fonction qui retire un joueur
 * \param joueur
 * \param index
 * \param sockopen
 * \return VOID
 */
void remove_player(player* joueurs, int index, bool sockopen) {
    if (sockopen) {
        shutdown_socket(joueurs[index].socket);
    }
    memcpy(joueurs[index].nickname, "\0", NAMESIZE);
    joueurs[index].socket = 0;
    scores_joueurs[index] = 0;
    cl_count--;
    int j;
    for (j = index+1; j <= cl_count; j++) { //on vient de supprimer un joueur, cl_count a été décrémenté 
        joueurs[j-1].socket = joueurs[j].socket;
        sprintf(joueurs[j-1].nickname, "%s", joueurs[j].nickname);
    }
    if (jeu_en_cours && cl_count == 1) {
        end_game();
    }
}

/**
 * \fn void refuse_connection(int socket) 
 * \brief Fonction de refus de connexion. Envoie d'un message avec le code REFUS défini dans config.h
 * \param socket
 * \return VOID
 */
void refuse_connection(int socket) {
    send_light_msg(REFUSE, socket);
}

/**
 * \fn void add_nickname(int socket, char** msg)
 * \brief Fonction qui ajoute le pseudo de 20 char max
 * \param socket 
 * \param **msg
 * \return VOID
 */
void add_nickname(int socket, char** msg) {
    char nickname[NAMESIZE];
    extract_player_nickname(msg, nickname);
    int index = find_index(joueurs, socket);
    sprintf(joueurs[index].nickname, "%s", nickname);
}

/**
 * \fn void deal_cards() 
 * \brief Fonction de distribution des cartes
 * \param void
 * \return VOID
 */
void deal_cards() {
    time_t t;
    srand((unsigned) time(&t));
    int card_random = rand() % 22;
    int cards_per_player = DECK_SIZE / cl_count;
    int dealt_cards[cards_per_player * cl_count];
    int total_dealt_cards = 0;
    int player;
    for (player = 0; player < cl_count; player++) {
        int card;
        int str_length = 0;
        char msg[3* cards_per_player]; //2 caractèress par carte + 1 espace
        char* msg_card = get_card_white_name(card_random);
        msg[0] = '\0';
        for (card = 0; card < cards_per_player; card++) {
            int random_card;
            do {
                random_card = rand_range(DECK_SIZE);
                //choisir une carte tant qu'on n'en trouve pas une qui n'a pas encore été choisie
            } while (array_contains(dealt_cards, random_card, total_dealt_cards));
            //rajouter la carte au message
            str_length += sprintf(msg+str_length, "%d ", random_card);
            dealt_cards[total_dealt_cards++] = random_card;
        }
        //distribuer les cartes choisies au joueur
        send_msg(DEFINE_CARD, msg_card, joueurs[player].socket);
        printf("La carte noire est \n: %s\n", msg_card);
        send_msg(DEAL, msg, joueurs[player].socket);
        printf("Cartes données : \n");
        printf("%s\n", msg);
    }
}

/**
 * \fn clear_lobby()
 * \brief Fonction de nettoyage du lobby après qu'un joueur ait quitté le salon
 * \param VOID
 * \return VOID
 */
void clear_lobby() {
    broadcast_light(DISCONNECT, joueurs, cl_count);
    jeu_en_cours = FALSE;
    while (joueurs[0].socket > 0) {
        remove_player(joueurs, 0, FALSE);
    }
}

/**
 * \fn bool receive_msg(char* msg, int fd)
 * \brief Fonction de Récception de message du serveur
 * \param *msg
 * \param fd
 * \return bool
 */
bool receive_msg(char* msg, int fd) {
    int bytes_received;
    if ((bytes_received = recv(fd, msg, MESSAGE_SIZE, 0)) <= 0) {
        if (bytes_received == 0) {
            printf("Client déconnecté.\n");
        }
        else {
            perror("Ne peut pas recevoir de message");
        }
        return FALSE;
    }
    return TRUE;
}

/**
 * \fn void start_game()
 * \brief Fonction qui met le jeu en marche. Du coup, le jeu est en statut : "En cours"
 * \param VOID 
 * \return VOID
 */
void start_game() {
    start_round();
    jeu_en_cours = TRUE;
}

/**
 * \fn void start_round()
 * \brief Fonction de démarrage du round
 * \param VOID
 * \return VOID
 */
void start_round() {
    deal_cards();
    fin_du_tour = TRUE; //voir fin du main
    fin_du_round = FALSE;
    sigempty = FALSE;
}

/**
 * \fn void shutdown_socket(int socket) 
 * \brief Fonction de fermeture de la socket
 * \param socket
 * \return void
 */
void shutdown_socket(int socket) {
    printf("Fermeture de la socket numéro %d\n", socket);
    if (close(socket) < 0) {
        perror("Socket fermée");
        exit(EXIT_FAILURE);
    }
}

/**
 * \fn void shutdown_server()
 * \brief Fonction de fermeture du serveur
 * \param void
 * \return VOID
 */
void shutdown_server() {
    printf("Serveur fermé\n");
    clear_lobby();
    serveur_en_cours = FALSE;
    
}

/**
 * \fn void receive_card(int socket, char** msg) 
 * \brief Fonction de réception des cartes
 * \param socket
 * \param **msg
 * \return VOID
 */
void receive_card(int socket, char** msg) {
    static int received_cards_count = 0;
    static int str_length = 0;
    static char cards[MAX_PLAYERS * 3];//:
    int player_index = find_index(joueurs, socket);
    int card;
    decode_msg_payload(msg, &card, 1);
    joueurs[player_index].played_card = card;
    received_cards_count++;
    str_length += sprintf(cards+str_length, "%d ", card);
    if (received_cards_count == cl_count) {
        int highest_card_holder, highest_card = 0, i, empty_count = 0, empty_index;
        for (i = 0; i < cl_count; i++) {
            if (joueurs[i].played_card >= highest_card) {
                highest_card = joueurs[i].played_card;
                highest_card_holder = i;
            }
            if (joueurs[i].isempty) {
                empty_count++;
                empty_index = i;
            }
            joueurs[i].played_card = -1;
            joueurs[i].isempty = FALSE;
        }
        send_msg(GIVE, cards, joueurs[highest_card_holder].socket);
        memset(cards, 0, cl_count * sizeof(int));
        received_cards_count = 0;
        highest_card = -1;
        str_length = 0;
        fin_du_tour = TRUE;
        if (empty_count == 1 && empty_index == highest_card_holder) { 
            printf("sauver par le gong\n");
            sigempty = FALSE;
        }
        if (sigempty) {
            broadcast_light(ROUND, joueurs, cl_count);
        }
    }
}

/**
 * \fn end_round(int socket, char** msg) 
 * \brief Fonction terminer un round
 * \param socket
 * \param **msg
 * \return VOID
 */
void end_round(int socket, char** msg) {
    if (!sigempty) {
        printf("Round terminé\n");
        sigempty = TRUE;
    }
    joueurs[find_index(joueurs, socket)].isempty = TRUE;
    printf("msg : %s\n", *msg);
}

/**
 * \fn void update_score(int socket, char** msg)
 * \brief Fonction de mise à jour du score
 * \param socket
 * \param **msg
 * \return VOID
 */
void update_score(int socket, char** msg) {
    static int count = 0;
    int score;
    decode_msg_payload(msg, &score, 1);
    int i;
    for (i = 0; i < cl_count; i++) {
        if (joueurs[i].socket == socket) {
            scores_joueurs[i] += score;
        }
    }
    count++;
    if (count == cl_count) {
        count = 0;
        fin_du_round = TRUE;
        int i;
        for (i = 0; i < cl_count; i++) {
            if (joueurs[i].socket > 0) {
                printf("%s : %d points\n", joueurs[i].nickname, scores_joueurs[i]);
            }
        }
        char buffer[10];
        printf("Appuyer sur une touche pour démarrer le tour\n");
        fgets(buffer, 10, stdin);
    }
}

