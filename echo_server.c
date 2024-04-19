#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, const char* argv[]){

        if(argc != 2){
                printf("%s\r\n","./ECHO_SERVER 9999\r\n");
                return -1;
        }

        int server_sock_fd = 0;
        int client_sock_fd = 0;

        struct sockaddr_in server_addr;
        struct sockaddr_in client_addr;
        memset(&server_addr, 0, sizeof server_addr);
        memset(&client_addr, 0, sizeof client_addr);

        server_sock_fd = socket(PF_INET, SOCK_STREAM, 0); // IPv4, TCP, 0 = TCP
        printf("SERVER FD : %d\r\n", server_sock_fd);

        if(server_sock_fd == -1){
                printf("socket() error\r\n");
                return -1;
        }

        // 번호 받기 IP, PORT
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // ip
        server_addr.sin_port = htons(atoi(argv[1])); // 9999 -> 99_99

        const int bind_state = bind(server_sock_fd, (const struct sockaddr*)&server_addr, sizeof server_addr);

        if(bind_state == -1){
                printf("bind() error\r\n");
                return -1;
        }

        const int listen_state = listen(server_sock_fd, 5);

        if(listen_state == -1){
                printf("listen() error\r\n");
                return -1;
        }

        int client_addr_size = sizeof client_addr;
        char message[BUFSIZ] = {'\0',};

        for(int i = 0; i <5; i++){ // client 5개를 받는다

                //open
                client_sock_fd = accept(server_sock_fd, (struct sockaddr*)&client_addr, (socklen_t*)&client_addr_size);
                printf("CLIENT SOCK FD : %d\r\n", client_sock_fd);
                if(client_sock_fd == -1){
                        printf("accept() error\r\n");
                        return -1;
                }
                printf("Client is connected ...\r\n");

                int str_length = 0;

                while((str_length = read(client_sock_fd, message, sizeof BUFSIZ)) != 0){
                        write(client_sock_fd, message, str_length);
                }
                printf("%d\r\n",str_length);
                close(client_sock_fd);
        }
        close(server_sock_fd);

        return 0;
}
