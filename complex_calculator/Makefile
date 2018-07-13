all : directories bin/main.o bin/calc.o
	gcc -o calc bin/main.o bin/calc.o -ldl
bin/main.o : src/main.c include/calc.h
	gcc -o bin/main.o -c src/main.c
bin/calc.o : src/calc.c include/calc.h
	gcc -o bin/calc.o -c src/calc.c
directories :
	mkdir -p bin
clean : 
	rm -rf calc bin/*