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

#include "client.h"

int client_socket;
int hand[DECK_SIZE];
int hand_white[DECK_SIZE_WHITE];
int stash[DECK_SIZE];
int cards_in_hand;
int cards_in_stash;


/**
 * \fn void disconnect(bool information)
 * \brief Fonction de déconnexion d'un client
 * \param information boolean qui va vérifier si on s'est déconnecté ou pas.
 * \return VOID
 */
void disconnect(bool information) {
    printf("Au revoir l'ami. \n\n");
    if (information) {
        send_light_msg(DISCONNECT, client_socket);
        close(client_socket);
    }
    exit(EXIT_SUCCESS);
}

/**
 * \fn void interrupt_handler(int signum)
 * \brief Fonction de déconnexion d'un client suita à un controle C
 * \param signum signal vérifiant le controle C
 * \return VOID.
 */
void interrupt_handler(int signum) {
    if (signum == SIGINT) {
        //Ctr+C
        disconnect(TRUE);
    }
}

/**
 * \fn void refill()
 * \brief Fonction qui va remettre des cartes en jeu
 * \param void
 * \return VOID.
 */
void refill() {
    memcpy(hand, stash, cards_in_stash * sizeof(int));
    memset(stash, -1, cards_in_stash * sizeof(int));
    cards_in_hand = cards_in_stash;
    cards_in_stash = 0;

    memcpy(hand_white, stash, cards_in_stash * sizeof(int));
    memset(stash, -1, cards_in_stash * sizeof(int));
    cards_in_hand = cards_in_stash;
    cards_in_stash = 0;
}

/**
 * \fn void clear_cards() 
 * \brief Fonction de nettoyage du buffer au niveau des cartes
 * \param void
 * \return VOID.
 */
void clear_cards() {
    memset(hand, -1, DECK_SIZE * sizeof(int));
    memset(stash, -1, DECK_SIZE * sizeof(int));
    cards_in_hand = 0;
    cards_in_stash = 0;

    memset(hand_white, -1, DECK_SIZE_WHITE * sizeof(int));
    memset(stash, -1, DECK_SIZE_WHITE * sizeof(int));
    cards_in_hand = 0;
    cards_in_stash = 0;
}


/**
 * \fn void print_cards()  
 * \brief Fonction qui va afficher les cartes en jeu
 * \param void
 * \return VOID.
 */
void print_cards() {
    int i;
    printf("Main courante de carte blanches : \n");
    for (i = 0; i < cards_in_hand; i++) {
        printf("%d: %-8s \t", i, get_card_name(hand[i]));
        if(i+1 < cards_in_hand) {
            printf("\t");
        }
       printf("\n");
    }
    printf("\n");
    printf("Tours gagnés : ");
    for (i = 0; i < cards_in_stash; i++) {
        //Justifier à gauche
        printf("%d: %-8s ", i, get_card_name(stash[i]));
        if(i+1 < cards_in_stash ){
            printf("|");
        }
    }
    printf("\n");
}

/**
 * \fn int calculate_score() 
 * \brief Fonction qui va calculer le score
 * \param void
 * \return score.
 */
int calculate_score() {
    printf("CALCUL DU SCORE\n");
    print_cards();
    int score = 0;
    int i;
    for (i = 0; i < cards_in_hand; i++) {
        score += get_card_points(hand[i]);
    }
    for (i = 0; i < cards_in_stash; i++) {
        score += get_card_points(stash[i]);
    }
    return score;
}

/**
 * \fn void receive_message(int client_socket, char** name) 
 * \brief Fonction qui va envoyer ua client un message du serveur en fonction des configs données
 * \param client_socket 
 * \param **name
 * \return VOID.
 */
void receive_message(int client_socket,char** name) {
    char msgFromServer[MESSAGE_SIZE];
    char* msg = msgFromServer;

    int bytesReceived;
    if ((bytesReceived=recv(client_socket,msgFromServer,MESSAGE_SIZE,0))==-1) {
        perror("Reçu");
        exit(EXIT_FAILURE);
    }
    msgFromServer[bytesReceived]='\0';
    fflush(stdout);
    int msg_code = extract_msg_code(&msg);
    if(msg_code == WAIT) {
        send_msg(NICKNAME, *name, client_socket);
    } else if(msg_code == REFUSE) {
        printf("Connexion au serveur refusé. Réssayez plus tard\n");
        close(client_socket);
        exit(-1);
    } else if(msg_code == DISCONNECT) {
        printf("Déconnecter par le serveur\n");
        printf("PERDU AHAHAH\n\n");
        close(client_socket);
        exit(-1);
    } else if(msg_code == ROUND) {
        printf("Ce round est terminé\n");
        int score = calculate_score();
        send_int_msg(SCORE, score, client_socket);
        printf("Votre score est de : %d .. \n", score);
    } else if(msg_code == DEAL) {
        clear_cards();
        printf("Début du round ");
        cards_in_hand = decode_msg_payload(&msg, hand, 7);
        printf("Voici vos cartes : \n");
    }else if(msg_code == DEFINE_CARD){
        printf("Carte noire : \n ");
        cards_in_hand = decode_msg_payload(&msg, hand, 1);
        printf("C'est cette phrase à compléter \n %s : \n", msg);
    } else if(msg_code == ASK) {
        print_cards();
        int choice = -1;
        if (cards_in_hand + cards_in_stash == 1) {
            printf("C'est votre dernière carte à jouer\n");
            send_light_msg(EMPTY, client_socket);
        }
        if (cards_in_hand == 0) {
            refill();
            printf("Vous n'avez plus de carte à jouer\n");
            print_cards();
        }
        int times = 0;
        do {
            if(times > 0) {
                printf("Vous devez jouer une carte\n");
                print_cards();
            }
            printf("Quelle carte voulez vous jouer ?\n");
            if (scanf("%d", &choice) == EOF) {
                //Ctr+D
                disconnect(FALSE);
            }
            times++;
        } while(choice < 0 || choice > cards_in_hand);
        send_int_msg(PLAY, hand[choice], client_socket);
        int i;
        for (i = choice; i < cards_in_hand-1; i++) {
            hand[i] = hand[i+1];
        }
        cards_in_hand--;
        printf("Voici votre nouvelle main: ");
        print_cards();
        printf("\n");
    } else if (msg_code == GIVE){
        int* stash_ptr = stash + cards_in_stash;
        int size = decode_msg_payload(&msg, stash_ptr, MAX_PLAYERS);
        cards_in_stash += size;
        print_cards();
        printf("Vous avez gagné ce tour\n");
    } else if (msg_code == WINNER) {
        printf("Félicitation vous avez gagné ce round\n");
    } else {
        printf("Message d'exception!");
    }
}

/**
 * \fn void create_nickname(char* name)  
 * \brief Fonction qui va créer un pseudo à un joueur
 * \param *name
 * \return VOID.
 */
void create_nickname(char* name) {
    char* request="Entrez un pseudo (20 characters max): ";
    printf("%s", request);
    scanf("%s", name);
    fflush(stdin);
    fflush(stdout);
}

/**
 * \fn void connectToServer(int *client_socket, char* server_ip, struct hostent *host, struct sockaddr_in *server_address)  
 * \brief Fonction qui va nous connecter à un serveur
 * \param *client_socket
 * \param *server_ip
 * \param *host
 * \param *server_address
 * \return VOID.
 */
void connectToServer(int *client_socket,char* server_ip,struct hostent *host,struct sockaddr_in *server_address){
   
    //ADRESSE IP NON VALIDE
    if ((host=gethostbyname(server_ip)) == NULL) {
        perror("Client: Vous n'avez pas saisie d'adresse IP valide \n");
        exit(EXIT_FAILURE);
    }else{
        printf("Adresse IP saisie : ");
        puts(host->h_name);
    }

    //ECHEC CONNEXION
    if ((*client_socket = socket(AF_INET,SOCK_STREAM, 0)) == -1) {
        perror("Client: Création de socket à un problème");
        exit(EXIT_FAILURE);
    }else{
        printf("Création de la socket réussie\n");
    }

    //CONFIGURATION SERVEUR
    server_address->sin_family = AF_INET;
    server_address->sin_port = htons(PORT_DIMOV);
    server_address->sin_addr = *((struct in_addr*)host->h_addr);
    memset(&(server_address->sin_zero), '\0', 8);

    //ECHEC DE CONNEXION
    if (connect(*client_socket, (struct sockaddr *)server_address,sizeof(struct sockaddr)) == -1) {
        perror("Client: Connect() à un problème");
        exit(EXIT_FAILURE);
    }
}

bool fdp_is_valid(int fdp) {
    return fcntl(fdp, F_GETFD) != -1 || errno != EBADF;
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

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Il faut préciser une adresse IP\n");
        return EXIT_FAILURE;
    }
    struct sockaddr_in server_address;// adresse du server
    struct hostent host;
    clear_cards();
    char name[NAMESIZE];
    char* name_ptr = name;
    //LIER AU SIGNAUX
    struct sigaction interrupt;
    memset(&interrupt, 0, sizeof(interrupt));
    interrupt.sa_handler = &interrupt_handler;
    sigaction(SIGINT, &interrupt, NULL);

    //CREATION DU PSEUDO
    create_nickname(name);
    //CONEXION AU SERVEUR
    connectToServer(&client_socket, argv[1], &host, &server_address);
    while (1) {
        if(!fdp_is_valid(client_socket)) {
            printf("Erreur de connexion.\n");
            break;
        }
        printf("Tour en cours, Attente de la réception des cartes\n");
        receive_message(client_socket, &name_ptr);
    }
    close(client_socket);
    return EXIT_SUCCESS;
}
