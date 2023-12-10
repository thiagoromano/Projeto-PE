all: client

client: client.o bigNumber.o
	gcc client.o bigNumber.o -o client

client.o: client.c bigNumber.h
	gcc -std=c99 -Wall -Wextra -Wvla -g -c client.c

bigNumber.o: bigNumber.c bigNumber.h
	gcc -std=c99 -Wall -Wextra -Wvla -g -c bigNumber.c