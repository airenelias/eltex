all : directories bin/main.o bin/libcalc.a
	gcc -o calc bin/main.o -Lbin -lcalc -static
bin/main.o : src/main.c include/calc.h
	gcc -o bin/main.o -c src/main.c
bin/libcalc.a : bin/calc.o
	ar -rc bin/libcalc.a bin/calc.o
bin/calc.o : src/calc.c include/calc.h
	gcc -o bin/calc.o -c src/calc.c 
directories :
	mkdir -p bin
clean : 
	rm -rf calc bin/*