#include <stdio.h>
#include <time.h>

int main(){
	time_t currentTime;

	while(1){
		time(&currentTime);
		printf("  time :: ");
		printf(ctime(&currentTime));
		sleep(2);
	}
	return 0;
}
