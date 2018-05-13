main: color.o client.o
	gcc -Wall -pthread color.o -lm client.o -o pcs server.c -lpigpiod_if2 -lrt
color.o: color.c
	gcc -Wall -c color.c
client.o: client.c
	gcc -Wall -c client.c
cln:
	rm pcs
	rm *.o

	