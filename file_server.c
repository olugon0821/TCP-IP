#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>

int main(int argc, const char* agrv[]){

        if(argc != 2){
                printf("./FILE_SERVER 9999\r\n");
                return -1;
        }

        FILE* ptr_file = NULL;
        ptr_file = fopen("echo_client.c","rb");

        if(ptr_file == NULL){
                printf("no file \r\n");
                return -1;
        }

        struct sockaddr_in server_addr;
        struct sockaddr_in client_addr;
        memset(&server_addr, 0, sizeof server_addr);
        memset(&client_addr, 0, sizeof client_addr);

        const int sock_fd = socket(PF_INET, SOCK_STREAM, 0);
        printf("SOCK FD : %d\r\n", sock_fd);
        int client_fd = 0;

        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        server_addr.sin_port = htons(atoi(agrv[1]));

        const int bind_state = bind(sock_fd, (const struct sockaddr*)&server_addr, sizeof server_addr);

        if(bind_state == -1){
                printf("bind() error\r\n");
                return -1;
        }

        const int listen_state = listen(sock_fd, 5);

        if(listen_state == -1){
                printf("listen() error\r\n");
                return -1;
        }

        socklen_t client_addr_length = 0u;

        client_fd = accept(sock_fd, (struct sockaddr*)&client_addr, (socklen_t*)&client_addr_length);
        printf("Client FD : %d\r\n", client_fd);

        char buffer[BUFSIZ] = {'\0',};
        unsigned int read_count = 0;

        while(true){
                read_count = fread((void*)buffer, sizeof (char), BUFSIZ, ptr_file);
                if(read_count < BUFSIZ){
                        write(client_fd, buffer, read_count);
                        break;
                }
                write(client_fd, buffer, BUFSIZ);
        }

        shutdown(client_fd, SHUT_WR);

        read(client_fd, buffer, BUFSIZ);
        printf("message from the client : %s\r\n", buffer);

        fclose(ptr_file);
        close(client_fd);
        close(sock_fd);

        return 0;
}
