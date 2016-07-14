all : app

app : testmon2
	./testmon2

testmon : testmon2.o
	gcc -o testmon2 testmon2.o

testmon.o : testmon2.c
	gcc -c testmon2.c

clean :
	rm testmon2.o testmon2
