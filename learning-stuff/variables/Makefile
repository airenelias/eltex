variables : main.o variables.o
	gcc -o variables main.o variables.o
main.o : main.c variables.h
	gcc -c main.c
variables.o : variables.c variables.h
	gcc -c variables.c
clean : 
	rm variables main.o variables.o
