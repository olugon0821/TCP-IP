#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>

int main(int argc, const char* argv[]){

        if(argc != 1){
                printf("./ECHO_CLIENT 127.0.0.1 9999");
                return -1;
        }

        const int sock_fd = socket(PF_INET, SOCK_STREAM, 0);

        if(sock_fd == -1){
                printf("socket() error\r\n");
                return -1;
        }

        struct sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof server_addr);

        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr(argv[1]);
        server_addr.sin_port = htons(atoi(argv[2]));

        const int connect_state = connect(sock_fd, (const struct sockaddr*)&server_addr, sizeof server_addr);

        if(connect_state == -1){
                printf("connect() error\r\n");
                return -1;
        }

        printf("Server is Connected ...\r\n");

        char message[BUFSIZ] = {'\0',};

        while(true){
                printf("Input message : Q to quit > ");
                fgets(message,BUFSIZ,stdin); // 문자열 입력
                if((strcmp(message, "q\n") == 0) || (strcmp(message, "Q\n") == 0))
                        break;
                write(sock_fd, message, strlen(message));
                const int str_length = read(sock_fd, message, BUFSIZ -1);
                message[str_length] = '\0';
                printf("Message from the server : %s\r\n", message);
        }
        close(sock_fd);
        return 0;
}
