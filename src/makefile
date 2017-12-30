test_build: pcs-server pcs-client
	@echo Build Completed.
	
install: pcs-server pcs-client
	sudo mv pcs-server /bin
	sudo mv pcs-client /bin
	
pcs-server: server.c
	gcc -Wall -pthread -o pcs-server server.c -lpigpiod_if2 -lrt
	
pcs-client: client.c
	gcc -o pcs-client client.c
