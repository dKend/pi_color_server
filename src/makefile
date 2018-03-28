main: pcsd pcs
	@echo Build Completed.
	
install: cleanup_prev_install pcsd pcs
	mv pcsd /bin
	mv pcs /bin
	
pcsd: server.c
	gcc -pthread -o pcsd server.c -lpigpiod_if2 -lrt
	
pcs: client.c
	gcc -Wall -o pcs client.c

test_all: testPigpiodFunctions test_assertion test_threadList test_list test_color
	

test: testPigpiodFunctions.o assertion.o
	gcc -Wall -pthread testPigpiodFunctions.o assertion.o -o test0 -lpigpiod_if2 -lrt

testPigpiodFunctions.o: testPigpiodFunctions.c
	gcc -Wall -pthread -c testPigpiodFunctions.c -lpigpiod_if2 -lrt
	
test_assertion: assertion.o test_assertion.o
	gcc -Wall assertion.o test_assertion.o -o test_assertion
	
test_assertion.o: test_assertion.c
	gcc -Wall -c test_assertion.c
	
assertion.o: assertion.c
	gcc -Wall -c assertion.c
	
test_threadList: assertion.o test_threadList.o threadList.o
	gcc -Wall -lpthread test_threadList.o assertion.o threadList.o -o test_threadList
	
test_threadList.o: test_threadList.c 
	gcc -Wall -lpthread -c test_threadList.c

threadList.o: threadList.c
	gcc -Wall -lpthread -c threadList.c
	
test_list: test_list.o assertion.o lists
	gcc -Wall assertion.o dstruct/lists.o test_list.o -o test_list
	
test_list.o: test_list.c
	gcc -Wall -c test_list.c
	
lists:
	make -C dstruct
	
test_color: test_color.o assertion.o color.o lists
	gcc -Wall -lm color.o test_color.o assertion.o dstruct/lists.o -o test_color

test_color.o: test_color.c
	gcc -Wall -c test_color.c
	
Color.o: Color.c
	gcc -Wall -c Color.c

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