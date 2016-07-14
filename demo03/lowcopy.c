#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#define BUF_SIZE 10000

int main(int argc, char* argv[]){
	char block[BUF_SIZE];
	int in, out;
	int nread;
	int bufSize;

	if(argc != 2){
		printf("매개 변수가 올바르지 않습니다.\n");
		exit(0);
	}
	bufSize = atoi(argv[1]);
	in = open("file.in", O_RDONLY);
	out = open("file.out", O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);

	while((nread = read(in, block, bufSize)) > 0)
		write(out, block, nread);

	close(in);
	close(out);

	exit(0);
}
