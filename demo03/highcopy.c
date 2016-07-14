#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#define BUF_SIZE 10000

int main(int argc, char* argv[]){
	char block[BUF_SIZE];
	FILE * in, * out;
	int nread;
	int bufSize;

	if(argc != 2){
		printf("매개변수가 올바르지 않습니다.\n");
		exit(0);
	}
	bufSize = atoi(argv[1]);
	in = fopen("file.in", "r");
	out = fopen("file.out", "w");

	while((nread = fread(block, 1, bufSize, in)) > 0)
		fwrite(block, 1, nread, out);

	fclose(in);
	fclose(out);

	exit(0);
}
