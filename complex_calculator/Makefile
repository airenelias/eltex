calc : main.o libcalc.a
	gcc -o calc bin/main.o -Lbin -lcalc -static
main.o : main.c calc.h
	gcc -o bin/main.o -c main.c
libcalc.a : calc.o
	ar -rc bin/libcalc.a bin/calc.o
calc.o : calc.c calc.h
	gcc -o bin/calc.o -c calc.c 
clean : 
	rm -f calc bin/libcalc.a bin/*.o