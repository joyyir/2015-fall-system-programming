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
	if(argc != 2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	in_port_t servPort = atoi(argv[1]);

	// socket() : TCP 서버 소켓 생성
	// Domain : internet networking
	// Type : stream socket (TCP)
	int serv_sock = socket(PF_INET, SOCK_STREAM, 0); // socket descriptor 반환
	if(serv_sock == -1)
		error_handling("socket() error");

	// bind() : 서버가 사용할 포트/주소를 서버소켓과 묶음
	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET; // Domain
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // IP Address, host to network long
	serv_addr.sin_port = htons(servPort); // Port number, host to network, short
	if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
		error_handling("bind() error");
	}

	// listen() : 서버 소켓을 listen 소켓으로 변경
	// 대기 중인 접속 요청을 저장할 socket queue 생성
	// 큐에 대기시킬 수 있는 접속 요청 수는 backlog 파라미터를 사용하여 5로 지정
	if(listen(serv_sock, 5) == -1){
		error_handling("listen() error");
	}

	printf("chat server initialized\n");

	// accept() : 연결 후 생성된 클라이언트 소켓을 리턴
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_len = sizeof(clnt_addr);
	int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_len);
	if(clnt_sock == -1){
		error_handling("accept() error");
	}
	
	// 접속한 클라이언트의 정보를 화면에 출력
	printf("\n%s(%d)님이 들어오셨습니다. 나가려면 (/quit)을 입력하세요.\n", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));

	// Chat 서비스 처리부
	while(1){
		char buffer[BUFSIZE];
		ssize_t numBytesRcvd = recv(clnt_sock, buffer, BUFSIZE, 0);
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

		printf("[접속자님 : %s(%d)] : %s\n", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port), buffer);

		// 서버의 메시지 작성
		printf("[관리자님] : ");
		gets(buffer);

		// 수식 문자열의 계산 결과를 클라이언트에게 전송(send)
		size_t bufferLen = strlen(buffer);
		ssize_t numBytesSent = send(clnt_sock, buffer, bufferLen, 0);
		if(numBytesSent == -1){
			error_handling("send() error");
		}

		if(strncmp(buffer, "/quit", 5) == 0)
			break;
	}

	close(clnt_sock);
	close(serv_sock);

	return 0;
}
