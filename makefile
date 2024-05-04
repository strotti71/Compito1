CC=gcc -Wall
CFLAGS= 
DEPS =  fileHandler.c stringHandler.c occorrenza.c compito1.c printer.c
c:
	$(CC)  $(CFLAGS) $(DEPS) -g -O0 -o a.out
