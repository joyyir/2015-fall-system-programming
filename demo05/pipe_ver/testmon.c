#include <signal.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int isProExist(char * pname){
	char buf[50];
	FILE * tempFile;
	int count;

	sprintf(buf, "pgrep -c %s$", pname); // -c is count option
	tempFile = popen(buf, "r");
	fscanf(tempFile, "%d", &count);
	pclose(tempFile);

	return count;
}

void command(int sig){
	pid_t pid;
	FILE * readFile;
	int testPid;
	char op;

	printf("Type Command (Q, K, S, R) >> ");
	while( (op = getchar()) == EOF || op == '\n');

	switch(op){
		case 'Q' :
			printf("Terminate 'testmon'...\n");
			kill(0, SIGTERM); // kill child process
			waitpid(0, NULL, 0);
			exit(1);
		case 'K' :
			if(isProExist("test")){
				printf("Terminate 'test' by 'testmon'...\n");
				readFile = popen("pgrep test$", "r");
				fscanf(readFile, "%d", &testPid);
				pclose(readFile);
				kill(testPid, SIGTERM);
				waitpid(testPid, NULL, 0);
			}
			else{
				printf("'test' is already terminated\n");
			}
			break;
		case 'S' :
			if(isProExist("test")){
				printf("'test' is already executed\n");
			}
			else{
				printf("Start 'test'...\n");
				// system 함수 쓰면 되지 않을까?
				pid = fork();
				if(pid == -1){
					printf("fork failed\n");
					exit(1);
				}
				else if(pid == 0){ // child process
					execl("./test", "test", NULL);
				}
				else{
					// parent process
				}
			}
			break;
		case 'R' :
			if(isProExist("test") == 1){
				printf("To restart, terminate 'test' first\n");
				readFile = popen("pgrep test$", "r");
				fscanf(readFile, "%d", &testPid);
				pclose(readFile);
				kill(testPid, SIGTERM);
				waitpid(testPid, NULL, 0);
			}
			
			pid = fork();
			if(pid == -1){
				printf("fork failed\n");
				exit(1);
			}
			else if(pid == 0){ // child process
				printf("restart 'test'...\n");
				execl("./test", "test", NULL);
			}
			else{
				// parent process
				sleep(1);
			}
			break;
		default :
			printf("wrong command\n");
			break;
	}
}

int main(void){
	pid_t pid;
	FILE * statusFile;
	char buf[200];
	// 여기부터 수정
	int file_pipes[2];
	int num = 1;

	if(pipe(file_pipes) == 0){
		pid = fork();
		if(pid == -1){
			printf("fork failed\n");
			exit(0);
		}
		else if(pid == 0){ // child process
			printf("child process start\n");
			while(1){
				if(isProExist("test")){
					//sleep(5);
					// system 함수로 대체할 수 없을까?
					statusFile = popen("ps -ef | grep test$", "r");
					fgets(buf, 200, statusFile);
					pclose(statusFile);
					printf("  log :: %s", buf);
					//kill(getppid(), SIGALRM);
					sprintf(buf, "%d", 1);
					write(file_pipes[1], buf, strlen(buf));
					sleep(5);
				}
				else{
					//kill(getppid(), SIGALRM);
					sprintf(buf, "%d", 1);
					write(file_pipes[1], buf, strlen(buf));
				}
			}	
		}
		else{ // parent process
			printf("parent process start\n");
			//signal(SIGALRM, command);
			while(1){
				if(read(file_pipes[0], buf, 200) > 0){
					//sscanf(buf, "%d", &num);
					command(0);
					//num = 0;
				}
			}
		}
	}
	return 0;
}
