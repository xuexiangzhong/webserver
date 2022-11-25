//
// Created by flysword on 2022/11/25.
//
#ifndef WEBSERVER_LOCKER_H
#define WEBSERVER_LOCKER_H
#include <semaphore.h>
#include <exception>
#include <pthread.h>

class sem {
private:
    sem_t m_sem;
public:
    sem(){
        if(sem_int(&m_sem,0,0)!= 0){
            throw std::exception();
        }
    }
    sem(int num){
        if(sem_int(&m_sem,0,num)!= 0){
            throw std::exception();
        }
    }
    ~sem(){
        sem_destroy(&m_sem);
    }
};

class locker{
private:
    pthread_mutex_t m_mutex;
    //    pthread_mutex_t m_mutex =PTHREAD_MUTEX_INITIALIZER;//静态初始化写法
public:
    locker(){
        if(pthread_mutex_init(&m_mutex,NULL) != 0){//动态初始化互斥锁
            throw std::exception();
        }
    }
    ~locker()
    {
        pthread_mutex_destroy(&m_mutex);//销毁
    }
    bool lock(){
        return pthread_mutex_lock(&m_mutex) == 0;//加锁
    }
    bool unlock(){
        return pthread_mutex_unlock(&m_mutex) == 0;//解锁
    }
    pthread_mutex_t *get()
    {
        return &m_mutex;
    }
};
class cond{
    cond()
    {
        if (pthread_cond_init(&m_cond, NULL) != 0)//初始化一个条件变量
        {
            //pthread_mutex_destroy(&m_mutex);
            throw std::exception();
        }
    }
    ~cond()
    {
        pthread_cond_destroy(&m_cond);//销毁一个条件变量
    }
private:
    pthread_cond_t  m_cond;//pthread_cond_t 对象 条件变量
};
#endif //WEBSERVER_LOCKER_H
