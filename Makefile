TARGET= server joueur

CC=gcc
CFLAGS=-Wall -Wextra

normal: $(TARGET)

server: serveur/server.o utils/common_utils.o utils/server_utils.o config/cards.o
	$(CC) $(CFLAGS) serveur/server.o utils/common_utils.o utils/server_utils.o config/cards.o -o server

joueur: client/client.o utils/common_utils.o config/cards.o
	$(CC) $(CFLAGS) client/client.o utils/common_utils.o config/cards.o -o joueur


server.o: serveur/server.c utils/common_utils.h utils/server_utils.h config/cards.h
	$(CC) $(CFLAGS) -c serveur/server.c

client.o: client/client.c utils/common_utils.h config/cards.h
	$(CC) $(CFLAGS) -c client/client.c

common_utils.o:	utils/common_utils.h utils/common_utils.c
	$(CC) $(CFLAGS) -c utils/common_utils.c

server_utils.o: utils/server_utils.h utils/server_utils.c
	$(CC) $(CFLAGS) -c utils/server_utils.c

cards.o: config/cards.h config/cards.c
	$(CC) $(CFLAGS) -c cards.c

clean:
	$(RM) $(TARGET)
