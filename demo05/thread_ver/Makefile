all : testmon


testmon : testmon.c
	gcc -D _REENTRANT testmon.c -o testmon -lpthread

clean :
	rm testmon.o testmon
