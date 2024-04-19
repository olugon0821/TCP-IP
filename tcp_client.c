#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, const char* argv[]){


    char message[BUFSIZ] = {'\0'};

    if(argc != 3){
        printf("%s\r\n", "./TCP_CLIENT 192.168.0.13 9999");
        exit(1);
    }
    const int sock_fd = socket(PF_INET, SOCK_STREAM, 0);
    printf("SOCK FD : %d\r\n", sock_fd);                        //3 예상
    if(sock_fd == -1){
        printf("SOCKET() error\r\n");
        exit(1);
    }
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof serv_addr);                    // 구조체 serv_addr 모두 0으로 초기화

    serv_addr.sin_family = PF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
    if(connect(sock_fd, (struct sockaddr*)&serv_addr, sizeof serv_addr) == -1)
    {
        printf("%s\r\n", "connect() error");
        exit(1);
    }
    int read_length = 0;
    int index = 0;
    int str_len = 0;
    while(read_length = read(sock_fd, &message[index++], sizeof(char))){
        if(read_length == -1)   printf("read() error");
        str_len += read_length;
    }
    printf("%s\r\n", message);
    printf("%d\r\n", str_len);
    close(sock_fd);

    return 0;
}
~~~~~~~~~~~~~~~~~~~~~"tcp_client.c" 48L, 1338B                                                                                                                                                                                                                                  38,20         All
