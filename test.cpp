#include <iostream>
#include <semaphore.h>
#include <sys/time.h>
#include <thread>

using namespace  std;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int i=1;
void *thread1(void * junk){
    for( i = 1;i<=6;i++){
        std::cout <<"djh: Line"<<__LINE__ <<",i= "<<i<<std::endl;
        pthread_mutex_lock(&mutex);//加锁
        std::cout <<"thread1: lock "<<__LINE__<<std::endl;
        if(i%3 == 0){
            std::cout <<"thread1: signal 1 "<<__LINE__<<std::endl;
            pthread_cond_signal(&cond);//发送信号
            std::cout <<"thread1: signal 2 "<<__LINE__<<std::endl;
            std::cout <<__FUNCTION__<<"will sleep 1s in Line "<<__LINE__<<std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds (1000));
        }
        pthread_mutex_unlock(&mutex);//解锁
        std::cout <<"thread1: unlock "<<__LINE__<<std::endl;
        std::cout <<__FUNCTION__<<"will sleep 1s in Line "<<__LINE__<<std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds (1000));
    }
}

void *thread2(void * junk){
    while(i<6) {
        std::cout << "djh: Line" << __LINE__ << ",i= " << i << std::endl;
        pthread_mutex_lock(&mutex);//加锁
        std::cout << "thread2: lock " << __LINE__ << std::endl;
        if (i % 3 == 0) {
            std::cout << "thread2: wait 1 " << __LINE__ << std::endl;
            pthread_cond_wait(&cond,&mutex);//解锁等待信号
            std::cout << "thread2: wait 2 " << __LINE__ << std::endl;
        }
        pthread_mutex_unlock(&mutex);//解锁
        std::cout << "thread2: unlock " << __LINE__ << std::endl;
        std::cout << __FUNCTION__ << "will sleep 1s in Line " << __LINE__ << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

int main(int argc, char *argv[])
{
    int time = 5;
    std::thread tread1(thread1,&time);
    std::thread post_tread(thread2,&time);

//    this_thread::sleep_until()
    if(tread1.joinable())
        tread1.join();
//    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    if(post_tread.joinable())
        post_tread.join();
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}