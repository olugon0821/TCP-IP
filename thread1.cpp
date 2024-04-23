  1 #include <unistd.h>
  2 #include <stdio.h>
  3 #include <pthread.h>
  4 void* thread_main(void* arg);
  5
  6 int main(int argc, char* argv[]){
  7
  8     pthread_t t_id;
  9     int thread_param = 5;
 10
 11     if(pthread_create(&t_id, NULL, thread_main, (void*)&thread_param) != 0){
 12         puts("pthread_create() error");
 13         return -1;
 14     }
 15
 16     sleep(10);
 17     puts("end of main");
 18     return 0;
 19 }
 20
 21 void* thread_main(void* arg){
 22     int i;
 23     int cnt = *((int*)arg);
 24     for(i = 0; i < cnt; i++){
 25         sleep(1);
 26         puts("running thread");
 27     }
 28     return NULL;
 29 }
~~~~~~~~~~~~~~                                                                                                                                          16,5          All
