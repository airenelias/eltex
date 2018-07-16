all : directories bin/main.o lib/libfileman.so
	gcc -o fileman bin/main.o -Llib -lfileman -lncurses -g
bin/main.o : src/main.c include/fileman.h
	gcc -o bin/main.o -c src/main.c
lib/libfileman.so : bin/fileman.o
	gcc -o lib/libfileman.so -shared bin/fileman.o
bin/fileman.o : src/fileman.c include/fileman.h
	gcc -o bin/fileman.o -c -fPIC src/fileman.c
directories :
	mkdir -p bin
clean : 
	rm -rf fileman bin/*