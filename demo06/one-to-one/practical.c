#include <stdio.h>
#include <stdlib.h>
#include "practical.h"

void error_handling(char *message){
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
