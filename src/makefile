main: color.o
	gcc -Wall -pthread color.o -lm -o pics server.c -lpigpiod_if2 -lrt 
	gcc -Wall -o client client.c
color.o: color.c
	gcc -Wall -c color.c