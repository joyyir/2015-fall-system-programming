all : app

app : testmon
	./testmon

testmon : testmon.o
	gcc -o testmon testmon.o

testmon.o : testmon.c
	gcc -c testmon.c

clean :
	rm testmon.o testmon
