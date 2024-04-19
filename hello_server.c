#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //linux Lib
#include <arpa/inet.h>  //ip(host) Lib  (192.168.0.13 9999) >> ip, port로 변경
#include <sys/socket.h> //socket Lib
#include <stdbool.h>    // 1 > true, 0 > false

int main(int argc, const char* args[]){
    int server_sock = 0;  //socket >> VFS(virtual File System)    ->  정수 값         커널을 통과 해서 프로세스에 접근할 다리 넘버
                        //0. stdin, 1. stdout, 2. stderr, 3번부터 소캣 다리로 사용
    int client_sock = 0;
    struct sockaddr_in server_addr;     //server information
    struct sockaddr_in client_addr;     //client information

    socklen_t client_addr_size = 0ul;   //client 주소 사이즈
    // 서버가 클라이언트에게 환영합니다.
    const char* message = "Welcome my Server";
    if(argc != 2){
        printf("you have a wrong command\r\n");
        printf("./HELLO_SERVER 9999\r\n");
        return -1;
    }
    server_sock = socket(PF_INET, SOCK_STREAM, 0);      //(IPv4, TCP, 0)
    if(server_sock == -1){
        printf("SOCKET() error\r\n");
        return -1;
    }
    memset(&server_addr, 0, sizeof server_addr);        //server_addr 사이즈 만큼 0으로 초기화 해주세요
    memset(&clieant_addr, 0, sizeof client_addr);        //client_addr 사이즈 만큼 0으로 초기화

    server_addr.sin_family = AF_INET;       //IPv4
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);    // host to network long       INADDR_ANY = 내 IP 주소
    server_addr.sin_port = htons(atoi(args[1]));        //포트 번호

    int bind_state = bind(server_sock, (const struct sockaddr*)&server_addr, sizeof server_addr);
    if(bind_state == -1){
        printf("BIND() error\r\n");
        return -1;
    }
    int listen_state = listen(server_sock, 5);
    if(listen_state == -1){
        printf("LISTEN() error\r\n");
        return -1;
    }
    client_addr_size = sizeof(client_addr);
    client_sock = accept(server_sock, (struct sockaddr*)&client_addr, (socklen_t*)&client_addr_size);       // blocking
    if(client_sock == -1){
        printf("ACCEPT() error\r\n");
        return -1;
    }
    write(client_sock, message, 18);
    close(client_sock);
    close(server_sock);

    return 0;
}
~~~~~~~~~~~~                                                                                                                                                                                                                                                           40,5          All
