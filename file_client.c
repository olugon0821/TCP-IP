#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, const char* argv[]){

    FILE* ptr_file = fopen("copied_file.txt", "wb");

    if(ptr_file == NULL){
        printf("File does not make one.\r\n");
        return -1;
    }

    char buffer[BUFSIZ] = {'\0',};

    if(argc != 3){
        printf("%s\r\n","FILE_CLIENT 192.168.0.8 9999");
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

    const int connect_state = connect(sock_fd, (struct sockaddr*)&server_addr, sizeof server_addr);

    if(connect_state == -1){
        printf("connect() error\r\n");
        return -1;
    }

    unsigned int read_count = 0;

    while((read_count = read(sock_fd,buffer,BUFSIZ)) != 0){
        fwrite((void*)buffer, sizeof(char), read_count, ptr_file);
    }

    printf("Server sended a file.\r\n");

    write(sock_fd,"Thank you!", 11);
    fclose(ptr_file);
    close(sock_fd);

    return 0;
}
