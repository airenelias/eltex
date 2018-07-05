all : reverse.c spiral.c
	gcc -o reverse reverse.c
	gcc -o spiral spiral.c
reverse : reverse.c
	gcc -o reverse reverse.c
spiral : spiral.c
	gcc -o spiral spiral.c
.PHONY : clean
clean : 
	rm -f reverse spiral
