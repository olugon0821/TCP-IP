#include <iostream> // C++ = stdio.h
#include <cstring> // C++ = string.h
#include <vector> // vector형 자료구조
#include <array> // array => c 배열
#include <sys/socket.h>
#include <arpa/inet.h> // network 값으로 변환 little -> big
#include <pthread.h>
#include <semaphore.h> // mutex
#include "SerialPort.cpp"

void* task(void*); // thread 함수 => task
void* taskSerial(void*); // thread 함수 => Serial 전송용 thread

static pthread_mutex_t mutex; // mutex key 동시 접근 제어
static std::vector<int> client_socket_fds; // client의 fd를 저장하는 자료구조
static int serial_port = 0; // port 저장용 변수

// Char storiged in newtwork state array
static std::array<char,BUFSIZ> tcp_message; // buffer => 임계영역
static std::array<char,BUFSIZ> serial_message; // buffer => 임계영역

int main(int argc, const char* argv[]){
    
    int server_sock_fd{0};
    int client_sock_fd{0};
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    pthread_t pid; // thread id
    
    if(argc != 2){
        std::cout << "./SERVER 9999" << std::endl;
        exit(EXIT_FAILURE);
    }

    serial_port = serialport_init("/dev/ttyACM0", 115200, nullptr);

    if(serial_port == -1){
        std::cout << "Serial connect error" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "Serial Port Connected" << std::endl;

    server_sock_fd = socket(PF_INET, SOCK_STREAM, 0);
    if(server_sock_fd == -1){
        std::cout << "socket() error" << std::endl;
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof server_addr);
    memset(&client_addr, 0, sizeof client_addr);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));
    const int bind_state = bind(server_sock_fd, (const struct sockaddr*)&server_addr, sizeof server_addr);

    if(bind_state == -1){
        std::cout <<"bind() error" << std::endl;
        exit(EXIT_FAILURE);
    }

    const int listen_state = listen(server_sock_fd,5);

    if(listen_state == -1){
        std::cout <<"listen() error" << std::endl;
        exit(EXIT_FAILURE);
    }

    pthread_create(&pid, nullptr, taskSerial, static_cast<void*>(nullptr));

    socklen_t client_sock_addr_size {0ul};
    
    while(true){
        client_sock_addr_size = sizeof client_addr;
        client_sock_fd = accept(server_sock_fd, (struct sockaddr*)&client_addr, (socklen_t*)&client_sock_addr_size);

        if(client_sock_fd ==-1){
            std::cout << "accept() error" << std::endl;
            break;
    }
        std::cout << "Connected from client IP : " << inet_ntoa(client_addr.sin_addr) << std::endl;
        pthread_mutex_lock(&mutex);
        client_socket_fds.push_back(client_sock_fd);
        pthread_mutex_unlock(&mutex);
        pthread_create(&pid, nullptr, task, static_cast<void*>(&client_sock_fd));
        pthread_detach(pid);
    }
    close(server_sock_fd);
    return int(0);
}

void* taskSerial(void* arg){
    while(true){
        const int serial_state = serialport_read_until(serial_port, serial_message.data(), '\n');
        if(!serial_state){
            pthread_mutex_lock(&mutex);
            for(auto fd : client_socket_fds){
                write(fd, serial_message.data(), strlen(serial_message.data()));
            }
            pthread_mutex_unlock(&mutex);
        }

    }
}

void* task(void* arg){
    const int clnt_sock_fd {*(static_cast<int*>(arg))};
    int tcp_str_length {0};
    while((tcp_str_length = read(clnt_sock_fd, tcp_message.data(), BUFSIZ)) != 0)
    {
        pthread_mutex_lock(&mutex);
        serialport_write(serial_port, tcp_message.data());
        pthread_mutex_unlock(&mutex);
    }
    pthread_mutex_lock(&mutex);

    for(auto it {client_socket_fds.begin()}; it != client_socket_fds.end(); ++it){
        if(*it == clnt_sock_fd){
            client_socket_fds.erase(it);
            break;
        }
    }
    pthread_mutex_unlock(&mutex);
    close(clnt_sock_fd);
    return nullptr;
}
