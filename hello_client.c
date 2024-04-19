#include <stdio.h>
#include <stdlib.h>         //표준 라이브러리
#include <string.h>
#include <unistd.h>         //리눅스 시스템 라이브러리
#include <arpa/inet.h>      //"ip" 를 숫자로, "port" 를 숫자(binary>형태로 변환시켜줌
#include <sys/socket.h>     //socket(), bind(), listen(), accept()
#include <stdbool.h>        //1 = true, 0 = false 로 변환 시켜줌

int main(int argc, const char* argv[]){
    if(argc != 3){
        printf("./HELLO_CLIENT 127.0.0.1 9999\r\n");
        return -1;
    }
    int sock = 0;               //클라이언트 소캑 연결
    struct sockaddr_in server_sock;     //서버 소캣에 접근 하기 위해 포트번호와 목적 ip 를 저장해야한다.
    char message[BUFSIZ] = {'\0',};     //512 바이트 배열 초기화
    unsigned int str_length = 0;        //문자열의 길이
    memset(&server_sock, 0, sizeof server_sock);
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1){
        printf("SOCKET() error\r\n");
        return -1;
    }
    server_sock.sin_family = AF_INET;
    server_sock.sin_addr.s_addr = inet_addr(argv[1]);     //127.0.0.1 -> 바이너리 타입으로 변경
    server_sock.sin_port = htons(atoi(argv[2]));         //9999 -> 바이너리 타입으로 변경

    if(connect(sock, (const struct sockaddr*)&server_sock, sizeof server_sock) == -1)
    {
        printf("CONNECT() error\r\n");
    }
    str_length = read(sock, message, sizeof message - 1);       //오류가 발생되면 -1 리턴
    if(str_length == -1){
        printf("READ() error\r\n");
    }
    printf("A Message from server : %s\r\n", message);

    close(sock);

    return 0;
}
                                                                                                                                                                                              30,1          All
