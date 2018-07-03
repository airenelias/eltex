test : main.o variables.o
	gcc -o test main.o variables.o
main.o : main.c variables.h
	gcc -c main.c
variables.o : variables.c
	gcc -c variables.c
clean : 
	rm test main.o variables.o
