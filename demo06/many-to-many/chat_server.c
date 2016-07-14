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
		
		// maxSockNum 갱신
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
			getTimeStr(message.m_time);

			for(i = 0; i < currUserCnt; i++){
				write(clntSock[i], &message, sizeof(message));
			}
			
			strcpy(message.m_buffer, "/quit");
			for(i = 0; i < currUserCnt; i++){
				write(clntSock[i], &message, sizeof(message));
			}

			break;
		}

		// 새로운 클라이언트가 접속했을 때
		if(FD_ISSET(serv_sock, &status) == 1){
			// accept()
			tempSock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_len);
			
			if(tempSock < 0){
				printf("accept() error\n");
				continue;
			}

			printf("----------------------------------------\n");
			printf("accept() success. Socket Number : %d\n", tempSock);
			printf("New client connected. IP (port) : %s (%d)\n", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));

			if(currUserCnt >= MAX_USER){
				close(tempSock);
				printf("error : 최대 접속자 수 초과\n");
				continue;
			}

			clntSock[currUserCnt] = tempSock;
			currUserCnt++;

			printf("A number of current users : %d\n", currUserCnt);
			printf("----------------------------------------\n");
		}
		// stdin에서 입력이 되었을 때
		else if(FD_ISSET(0, &status) == 1){
			temp = read(0, message.m_buffer, sizeof(message.m_buffer));
			message.m_buffer[temp-1] = '\0';
			strcpy(message.m_userName, "Server");
			getTimeStr(message.m_time);

			// ENQ(Ctrl+E) 또는 "/quit"이 입력되었을 때, 클라이언트에게 서버를 종료를 알리고 서버를 종료한다.
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
		// 각 클라이언트로부터 받은 메세지를 확인
		else{
			for(i = 0; i < currUserCnt; i++){
				if(FD_ISSET(clntSock[i], &status) == 1){
					temp = read(clntSock[i], &message, sizeof(message));

					// 클라이언트의 접속이 끊겼거나 클라이언트로부터 "/quit" 명령을 받았을 때, 접속을 해제함
					if(temp <= 0 || strcmp("/quit", message.m_buffer) == 0){
						close(clntSock[i]);
						clntSock[i] = clntSock[currUserCnt-1];
						currUserCnt--;
						strcpy(message.m_buffer, message.m_userName);
						strcpy(message.m_userName, "Server");
						strcat(message.m_buffer, "님이 나갔습니다.");
						getTimeStr(message.m_time);
						printf("%s\n", message.m_buffer);
						for(j = 0; j < currUserCnt; j++){
							write(clntSock[j], &message, sizeof(message));
						}
						i--;
						continue;
					}
					// '/'로 시작하는 명령을 처리하는 부분 (현재 구현한 기능은 없음)
					if(message.m_buffer[0] == '/'){
						strcpy(message.m_userName, "Server");
						strcpy(message.m_buffer, "Under constructing...");
						write(clntSock[i], &message, sizeof(message));
					}
					// 클라이언트로부터 받은 메세지를 출력하고 다른 클라이언트들에게 broadcast
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
		usleep(100000); // 0.1s delay
	} // End of while(1)
	
	printf("Terminate Server...\n");
	close(serv_sock);

	for(i = 0; i < currUserCnt; i++){
		close(clntSock[i]);
	}

	return 0;
	//ssize_t numBytesRcvd = recv(clnt_sock, buffer, BUFSIZE, 0);
	//ssize_t numBytesSent = send(clnt_sock, buffer, bufferLen, 0);
}
