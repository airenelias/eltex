all : directories bin/main.o lib/libtexted.so
	gcc -o texted bin/main.o -Llib -ltexted -lncurses -g
bin/main.o : src/main.c include/texted.h
	gcc -o bin/main.o -c src/main.c
lib/libtexted.so : bin/texted.o
	gcc -o lib/libtexted.so -shared bin/texted.o
bin/texted.o : src/texted.c include/texted.h
	gcc -o bin/texted.o -c -fPIC src/texted.c
directories :
	mkdir -p bin
clean : 
	rm -rf texted bin/*