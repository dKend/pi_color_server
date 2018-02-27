main: pcsd pcs
	@echo Build Completed.
	
install: cleanup_prev_install pcs-server pcs-client
	mv pcsd /bin
	mv pcs /bin
	
pcsd: server.c
	gcc -pthread -o pcsd server.c -lpigpiod_if2 -lrt
	
pcs: client.c
	gcc -Wall -o pcs client.c

test: testPigpiodFunctions.o assertion.o
	gcc -Wall -pthread testPigpiodFunctions.o assertion.o -o test0 -lpigpiod_if2 -lrt

testPigpiodFunctions.o: testPigpiodFunctions.c
	gcc -Wall -pthread -c testPigpiodFunctions.c -lpigpiod_if2 -lrt
	
assertion.o: assertion.c
	gcc -Wall -c assertion.c
	
test1: test_threadList.o assertion.o threadList.o
	gcc -Wall -lpthread test_threadList.o assertion.o threadList.o -o test1
	
test_threadList.o: test_threadList.c
	gcc -Wall -lpthread -c test_threadList.c

threadList.o: threadList.c
	gcc -Wall -c threadList.c
	
cleanup_prev_install: 
	if[ -e /bin/pcs-server ]
		@echo Previous version of pcs-server found.
		rm /bin/pcs-server
	fi
	
	if[ -e /bin/pcsd ]
		@echo Previous version of pcs-server found.
		rm /bin/pcsd
	fi
	
	if[ -e /bin/pcs-client ]
		@echo Previous version of pcs-client found.
		rm /bin/pcs-client	
	fi
	
	if[ -e /bin/pcs ]
		@echo Previous version of pcs-client found.
		rm /bin/pcs	
	fi