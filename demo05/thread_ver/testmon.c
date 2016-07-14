#include <signal.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

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

int getCommand = 0;

void * thread_function(void *arg){
	FILE * statusFile;
	char buf[200];

	printf("thread_function :: thread start\n");
	while(1){
		if(isProExist("test")){
			statusFile = popen("ps -ef | grep test$", "r");
			fgets(buf, 200, statusFile);
			pclose(statusFile);
			printf("  log :: %s", buf);
			getCommand = 1;
			sleep(5);
		}
		else{
			getCommand = 1;
		}
	}
}

int main(void){
	int res;
	pthread_t a_thread;
	void *thread_result;

	res = pthread_create(&a_thread, NULL, thread_function, NULL);
	if(res != 0){
		perror("main :: Thread creation failed");
		exit(0);
	}
	printf("main :: Thread creation success\n");

	while(1){
		if(getCommand == 1){
			command(0);
			getCommand = 0;
		}
	}

	return 0;
}
