all : mytest

# Which compiler
CC = gcc

# Where are library files kept
LIB = .

# Options for development
CFLAGS = -g -Wall -ansi

# Options for release
# CFLAG = -O -Wall -ansi

# -g : 바이너리 파일에 디버깅 정보 삽입
# -Wall : 모든 경고 메세지 출력
# -ansi : ansi c 문법으로 문법 검사
# -O : O1 수준으로 최적화

mytest : mytest.o libmine.a
	$(CC) -o mytest mytest.o -L$(LIB) -lmine

libmine.a : foo.o boo.o bar.o
	ar crv libmine.a foo.o boo.o bar.o

foo.o : foo.c foo.h
	$(CC) $(CFLAGS) -c foo.c

boo.o : boo.c boo.h
	$(CC) $(CFLAGS) -c boo.c

bar.o : bar.c bar.h
	$(CC) $(CFLAGS) -c bar.c

mytest.o : mytest.c mylib.h
	gcc -W -Wall -c -o mytest.o mytest.c

clean :
	rm -rf *.o *.a mytest

