#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "practical.h"

int main(int argc, char *argv[]){
	if(argc != 3){
		printf("Usage : %s <Server Address> <Server Port>\n", argv[0]);
		exit(1);
	}

	char *servIP = argv[1];
	in_port_t servPort = atoi(argv[2]);

	// socket() : 소켓 생성
	int sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sock == -1){
		error_handling("socket() error");
	}

	// connect() : 서버에 접속
	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(servIP);
	serv_addr.sin_port = htons(servPort);
	if(connect(sock, &serv_addr, sizeof(serv_addr)) == -1){
		error_handling("connect() error");
	}

	int temp;
	char userName[20];
	struct Message message;
	fd_set status;

	printf("chat client initialized\n");

	// nickname 입력 받기
	printf("Enter nickname : ");
	fflush(stdout);
	temp = read(0, userName, sizeof(userName));
	userName[temp-1] = '\0';
	
	// 클라이언트 접속 정보를 서버에 보내기
	strcpy(message.m_userName, userName);
	strcpy(message.m_buffer, userName);
	strcat(message.m_buffer, "님이 접속하였습니다.");
	getTimeStr(message.m_time);
	write(sock, &message, sizeof(message));

	while(1){
		FD_ZERO(&status); // 0으로 초기화
		FD_SET(0, &status); // stdin을 set
		FD_SET(sock, &status); // socket을 set
		temp = select(sock+1, &status, 0, 0, 0);

		// select() 에러 처리
		if(temp < 0){
			printf("select() error\n");
			write(sock, "/quit", sizeof("/quit"));
			break;
		}

		// stdin에 입력이 들어왔을 때
		if(FD_ISSET(0, &status) == 1){
			temp = read(0, message.m_buffer, sizeof(message.m_buffer));
			message.m_buffer[temp-1] = '\0';
			strcpy(message.m_userName, userName);
			getTimeStr(message.m_time);

			// 입력이 ENQ(Ctrl+E) 이거나 "/quit"일 경우, 접속 종료
			if(message.m_buffer[0] == 0x05 || strcmp(message.m_buffer, "/quit") == 0){
				printf("Terminate Client...\n");
				strcpy(message.m_buffer, "/quit");
				write(sock, &message, sizeof(message));
				break;
			}

			write(sock, &message, sizeof(message));
		}
		// 서버로부터 메세지가 왔을 때
		else if(FD_ISSET(sock, &status) == 1){
			temp = read(sock, &message, sizeof(message));
			if(temp <= 0){
				printf("서버와 접속이 끊겼습니다. Terminate Client...\n", temp);
				break;
			}

			if(strcmp("/quit", message.m_buffer) == 0){
				printf("Server is terminated. Terminate Client...\n");
				break;
			}

			printf("[%s : ", message.m_userName);
			printf("%s ", message.m_buffer);
			printf("(%s)]\n", message.m_time);
		}
		usleep(100000); // 0.1s delay
	} // End of while(1)

	// close() : 소켓 종료
	close(sock);

	return 0;
}
