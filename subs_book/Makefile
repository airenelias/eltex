book : main.o book.o
	gcc -o book main.o book.o
main.o : main.c book.h
	gcc -c main.c
book.o : book.c book.h
	gcc -c book.c
clean :
	rm -f book main.o book.o