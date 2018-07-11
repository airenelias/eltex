all : directories main.o libcalc.a
	gcc -o calc bin/main.o -Lbin -lcalc -static
main.o : main.c calc.h
	gcc -o bin/main.o -c main.c
libcalc.a : calc.o
	ar -rc bin/libcalc.a bin/calc.o
calc.o : calc.c calc.h
	gcc -o bin/calc.o -c calc.c 
directories :
	mkdir -p bin
clean : 
	rm -rf calc bin