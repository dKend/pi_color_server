################################################################################
#	Source file names
################################################################################
#	Normal source files
SRC_COLOR 		= Color
SRC_ASSERTION 	= assertion
SRC_LIST 		= List
SRC_THREADLIST 	= ThreadList
#	Test source files
SRC_T_ASSERTION	= test_assertion
SRC_T_COLOR		= test_color
SRC_T_LIST		= test_list
SRC_T_THREADLIST= test_threadList

################################################################################
#	gcc compile arguments
################################################################################
GCC_BASIC 		= -Wall -c

################################################################################
#	Dependency Lists
################################################################################
TEST_ASSERTION = $(SRC_ASSERTION).o $(SRC_T_ASSERTION).o
TEST_THREADLIST= $(SRC_ASSERTION).o $(SRC_T_THREADLIST).o $(SRC_THREADLIST).o

main: pcsd pcs
	@echo Build Completed.
	
install: cleanup_prev_install pcsd pcs
	mv pcsd /bin
	mv pcs /bin

pcsd: server.c
	gcc -o pcsd server.c -lpigpiod_if2 -lrt -pthread
	
pcs: client.c
	gcc -Wall -o pcs client.c
	
	
#	Link test sources tests
test_assertion: $(TEST_ASSERTION)
	gcc -Wall $(TEST_ASSERTION) -o $(SRC_T_ASSERTION)
	
test_threadList: $(TEST_THREADLIST)
	gcc -Wall $(TEST_THREADLIST) -o $(SRC_T_THREADLIST) -lpthread
	
test_list: test_list.o assertion.o Lists.o
	gcc -Wall assertion.o dstruct/lists.o test_list.o -o test_list
	
test_color: test_color.o assertion.o Color.o Lists.o
	gcc -Wall Color.o test_color.o assertion.o dstruct/lists.o -o test_color -lm
	
	
#	Compile test source files
test_threadList.o: test_threadList.c 
	gcc $(GCC_BASIC) test_threadList.c
	
test_color.o: test_color.c
	gcc $(GCC_BASIC) test_color.c
	
test_assertion.o: test_assertion.c
	gcc $(GCC_BASIC) test_assertion.c
	
test_list.o: test_list.c
	gcc $(GCC_BASIC) test_list.c


#	Compile non-test source files
assertion.o: assertion.c
	gcc $(GCC_BASIC) assertion.c

threadList.o: threadList.c
	gcc $(GCC_BASIC) threadList.c
	
List.o: List.c
	gcc $(GCC_BASIC) List.c
	
Color.o: Color.c
	gcc $(GCC_BASIC) Color.c

ColorShared: Color.c
	gcc $(GCC_BASIC) -fPIC Color.c -o Color.o
	
libColor.so: ColorShared lists2
	gcc -Wall -shared Color.o dstruct/lists.o -o libColor.so -lm

assertionShared: assertion.c
	gcc $(GCC_BASIC) -fPIC assertion.c -o assertion.o
	
libAssertion.so: assertionShared
	gcc -Wall -shared assertion.o -o assertion.so
	
#cleanup_prev_install: 
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