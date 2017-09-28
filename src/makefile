main: client
	gcc -Wall -pthread -o pcs-server server.c -lpigpiod_if2 -lrt
client: client.c
	gcc -Wall -o pcs-client client.c
