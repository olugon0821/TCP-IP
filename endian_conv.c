#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]){
    unsigned short host_port = 0x1234;
    unsigned short net_port;
    unsigned long host_addr = 0x12345678;
    unsigned long net_addr;

    net_port = htons(host_port);
    net_addr = htonl(host_addr);
    
    printf("Host ordered port : %#x \n", host_port);
    printf("Network ordered port : %#x \n", net_port);
    printf("Host ordered address : %#1x \n", host_addr);
    printf("Network ordered address : %#1x \n", net_addr);
    return 0;
}
~~~~~~~~~~~"endian_conv.c" 18L, 523B                                                                             18,1          All
