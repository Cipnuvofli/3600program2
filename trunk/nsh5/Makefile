nsh5 : nsh5.o processInput.o listFunctions.o mem.o nshPrep.o
	gcc -o nsh5 nsh5.o processInput.o listFunctions.o mem.o nshPrep.o

nsh5.o : nsh5.c
	gcc -g -c -w nsh5.c

processInput.o : processInput.c
	gcc -g -c -w processInput.c

listFunctions.o : listFunctions.c
	gcc -g -c -w listFunctions.c

mem.o : mem.c
	gcc -g -c -w mem.c

nshPrep.o : nshPrep.c
	gcc -g -c -w nshPrep.c

clean:
	rm -f *.o
