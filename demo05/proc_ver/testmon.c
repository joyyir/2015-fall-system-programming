#include <stdio.h>
#include <sys/types.h>
void printMenu(){
	printf("=================================\n");
	printf(" 2015-2 시스템프로그래밍 Lab #05\n");
	printf("=================================\n");
	printf("	Q : quit\n");
	printf("	K : kill\n");
	printf("	S : start\n");
	printf("	R : restart\n");
	printf("=================================\n");
}

int main(){
	char cmd;
	int running = 1;
	pid_t new_pid;
	new_pid = fork();

	switch(new_pid){
		case -1 :
			printf("fork failed\n");
			break;
		case 0 : /* this is the child */
			printf("this is child\n");
			while(running){
				printMenu();
				printf(">> ");
				scanf("%c", &cmd);
				switch(cmd){
					case 'Q' :
						printf("Exit...\n");
						running = 0;
						break;
					case 'K' :
						
						break;
						case 'S' :
							break;
						case 'R' :
							break;
					default :
						printf("잘못된 명령입니다. 다시 입력하세요.\n");
						break;
				}
			}
			break;
		default : /* this is the parent */
			printf("this is parent2\n");
			while(1){
				sleep(5);
				printf(">> ");
			}
			break;
	}


	return 0;
}
