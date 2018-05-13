################################################################################
#	File name for the main source file.
################################################################################
SRC = assertion

$(SRC).o: $(SRC).c
	gcc -Wall -c $(SRC).c

################################################################################
#	Link the assertion object file to a test source
################################################################################
BuildTest: $(SRC).o 
	ifdef TestSource
	gcc -Wall -c $(TestSource).c
	ifdef TestOutputName
	gcc -Wall $(TestSource).o $(SRC).o -o $(TestOutputName)
	else
	gcc -Wall $(TestSource).o $(SRC).o -o Test
	endif
	else
	$(error The TestSource variable was not defined.)
	endif
