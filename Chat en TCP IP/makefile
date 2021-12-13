FLAGS=-Wall -pedantic

all: chat

chat: common.h client.c server.c
	gcc client.c -o client -pthread $(FLAGS)
	gcc server.c -o serveur $(FLAGS)

clean:
	rm -f client server