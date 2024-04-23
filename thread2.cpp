#include <stdio.h>
  2 #include <stdlib.h>
  3 #include <string.h>
  4 #include <pthread.h>
  5 #include <unistd.h>
  6 void* thread_main(void* arg){
  7     int count = *((int*)arg);
  8     for(int i = 0; i < count; i++){
  9         sleep(1);
 10         puts("Running Thread\r\n");
 11     }
 12     char* msg = (char*)malloc(sizeof(char)*50);
 13     strcpy(msg, "Hello, I am thread~\r\n");
 14
 15     return (void*)msg;
 16 }
 17
 18 int main(int argc, char* argv[]){
 19     pthread_t pthread_id = 0ul;
 20     int thread_parameter = 5;
 21     const int pthread_state = pthread_create(&pthread_id, NULL, thread_main, (void*)&thread_parameter);
 22
 23     if(pthread_state != 0){
 24         puts("pthread_create() error\r\n");
 25         return -1;
 26     }
 27
 28     void* return_value = NULL;
 29     const int join_state = pthread_join(pthread_id, &return_value);
 30
 31     if(join_state){
 32         puts("join error\r\n");
 33         return -1;
 34     }
 35     printf("Return value from the main thread : %s\r\n", (const char*)return_value);
 36     free(return_value);
 37
 38     return 0;
 39 }
