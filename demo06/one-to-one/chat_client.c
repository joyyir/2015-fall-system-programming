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

	printf("chat client initialized\n");

	while(1){
		// 사용자로부터 수식 문자열 입력(gets)
		printf("[접속자님] : ");
		char buffer[BUFSIZE];
		gets(buffer);
		
		// 서버로 수식 문자열 전송(send)
		size_t bufferLen = strlen(buffer);
		ssize_t numBytesSent = send(sock, buffer, bufferLen, 0);
		if(numBytesSent == -1){
			error_handling("send() error");
		}

		// 종료 조건 검사
		if(strncmp(buffer, "/quit", 5) == 0)
			break;

		// 서버로부터 온 메시지 화면 출력
		ssize_t numBytesRcvd = recv(sock, buffer, BUFSIZE, 0);
		if(numBytesRcvd == -1){
			error_handling("recv() error");
		}
		else if(numBytesRcvd == 0){
			error_handling("peer connection closed");
		}

		buffer[numBytesRcvd] = '\0';

		// 종료 조건 검사
		if(strncmp(buffer, "/quit", 5) == 0)
			break;

		printf("[관리자님 : %s(%s)] : %s\n", argv[1], argv[2], buffer);
	}

	// close() : 소켓 종료
	close(sock);

	return 0;
}
