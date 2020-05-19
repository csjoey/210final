all:
	gcc main.c -o main -lsense -lm
clean: 
	rm -f game *.o
