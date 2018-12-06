all:
	gcc main.c ac.c -g -Wall -o acexe -lm
	./acexe
