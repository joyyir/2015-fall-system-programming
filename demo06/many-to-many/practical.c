#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "practical.h"

void error_handling(char *message){
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void getTimeStr(char *destTime){
	time_t tempTime;

	time(&tempTime);
	strftime(destTime, 26, "%Y-%m-%d %H:%M:%S", localtime(&tempTime));
}
