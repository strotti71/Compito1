CC=gcc -Wall 
CFLAGS= 
DEPS =  fileHandler.c stringHandler.c occorrenza.c Hal_s.c printer.c
c:
	$(CC)  $(CFLAGS) $(DEPS) -g -O0 -o hal
