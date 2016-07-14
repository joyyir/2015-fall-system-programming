#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "practical.h"

int main(int argc, char *argv[]){
	int i, j;

	if(argc != 2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	in_port_t servPort = atoi(argv[1]);

	// socket()
	// Domain : internet networking
	// Type : stream socket (TCP)
	int serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
		error_handling("socket() error");

	// bind()	
	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET; // Domain
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // IP Address, host to network long
	serv_addr.sin_port = htons(servPort); // Port number, host to network, short
	if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
		close(serv_sock);
		error_handling("bind() error");
	}

	// listen()
	if(listen(serv_sock, 5) == -1){
		close(serv_sock);
		error_handling("listen() error");
	}

	printf("chat server initialized\n");

	fd_set status;
	struct Message message;
	int currUserCnt = 0;
	int clntSock[MAX_USER], tempSock;
	int maxSockNum = serv_sock + 1;
	int temp;
	time_t tempTime;
	unsigned char clntBuffer[512];
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_len = sizeof(clnt_addr);

	while(1){
		FD_ZERO(&status);
		FD_SET(0, &status);
		FD_SET(serv_sock, &status);

		for(i = 0; i < currUserCnt; i++){
			FD_SET(clntSock[i], &status);

			if(maxSockNum <= clntSock[i]){
				maxSockNum = clntSock[i] + 1;
			}
		}

		temp = select(maxSockNum, &status, NULL, NULL, NULL);
		
		if(temp < 0){
			printf("select() error\n");
			strcpy(message.m_userName, "Server");
			strcpy(message.m_buffer, "select() error");

			for(i = 0; i < currUserCnt; i++){
				write(clntSock[i], &message, sizeof(message));
			}
			
			strcpy(message.m_buffer, "/quit");
			for(i = 0; i < currUserCnt; i++){
				write(clntSock[i], &message, sizeof(message));
			}

			break;
		}

		if(FD_ISSET(serv_sock, &status) == 1){
			// accept()
			tempSock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_len);
			printf("----------------------------------------\n");
			printf("accept() success. Socket Number : %d\n", tempSock);

			if(tempSock < 0){
				printf("accept() error\n");
				continue;
			}

			printf("New client connected. IP (port) : %s (%d)\n", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));

			if(currUserCnt >= MAX_USER){
				close(tempSock);
				printf("error : 真 真真 真\n");
				continue;
			}

			clntSock[currUserCnt] = tempSock;
			currUserCnt++;

			printf("A number of current users : %d\n", currUserCnt);
			printf("----------------------------------------\n");
		}
		else if(FD_ISSET(0, &status) == 1){
			temp = read(0, message.m_buffer, sizeof(message.m_buffer));
			message.m_buffer[temp-1] = '\0';
			strcpy(message.m_userName, "Server");

			getTimeStr(message.m_time);

			if(message.m_buffer[0] == 0x05 || strcmp(message.m_buffer, "/quit") == 0){
				strcpy(message.m_buffer, "/quit");

				for(i = 0; i < currUserCnt; i++){
					write(clntSock[i], &message, sizeof(message));
				}
				
				break;
			}

			for(i = 0; i < currUserCnt; i++){
				write(clntSock[i], &message, sizeof(message));
			}
		}
		else{
			for(i = 0; i < currUserCnt; i++){
				if(FD_ISSET(clntSock[i], &status) == 1){
					temp = read(clntSock[i], &message, sizeof(message));
					if(temp <= 0 || strcmp("/quit", message.m_buffer) == 0){
						close(clntSock[i]);
						clntSock[i] = clntSock[currUserCnt-1];
						currUserCnt--;
						strcpy(message.m_buffer, message.m_userName);
						strcpy(message.m_userName, "Server");
						strcat(message.m_buffer, "真 真真真.");
						printf("%s\n", message.m_buffer);
						for(j = 0; j < currUserCnt; j++){
							write(clntSock[j], &message, sizeof(message));
						}
						i--;
						continue;
					}
					if(message.m_buffer[0] == '/'){
						strcpy(message.m_userName, "Server");
						strcpy(message.m_buffer, "Under constructing...");
						write(clntSock[i], &message, sizeof(message));
					}
					else{
						printf("[%s (%s) : ", message.m_userName, inet_ntoa(clnt_addr.sin_addr));
						printf("%s ", message.m_buffer);
						printf("(%s)]\n", message.m_time);
						for(j = 0; j < currUserCnt; j++){
							write(clntSock[j], &message, sizeof(message));
						}
					}
				}
			}
		}
		usleep(100000);
	}// End of while(1)
	
	printf("Terminate Server...\n");
	close(serv_sock);

	for(i = 0; i < currUserCnt; i++){
		close(clntSock[i]);
	}

	return 0;
	//ssize_t numBytesRcvd = recv(clnt_sock, buffer, BUFSIZE, 0);
	//ssize_t numBytesSent = send(clnt_sock, buffer, bufferLen, 0);
}
