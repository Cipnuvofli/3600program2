nsh4 : nsh4.o processInput.o listFunctions.o mem.o nshPrep.o
	gcc -o nsh4 nsh4.o processInput.o listFunctions.o mem.o nshPrep.o

nsh4.o : nsh4.c
	gcc -g -c nsh4.c

processInput.o : processInput.c
	gcc -g -c processInput.c

listFunctions.o : listFunctions.c
	gcc -g -c listFunctions.c

mem.o : mem.c
	gcc -g -c mem.c

nshPrep.o : nshPrep.c
	gcc -g -c nshPrep.c

clean:
	rm -f *.o
