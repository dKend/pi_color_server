test_build: pcsd pcs
	@echo Build Completed.
	
pcsd: PCSServer.c
	gcc -Wall -pthread -o pcsd PCSServer.c -lpigpiod_if2 -lrt
	
pcs: PCSClient.c assertion.o
	gcc -Wall -o pcs lib/SimpleCTesting/assertion.o PCSClient.c
	rm lib/SimpleCTesting/assertion.o
	
assertion.o:
	make -C lib/SimpleCTesting assertion.o
	