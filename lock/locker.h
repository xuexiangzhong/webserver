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
    sem()
    {
        if (sem_init(&m_sem, 0, 0) != 0)//初始化
        {
            throw std::exception();
        }
    }
    sem(int num)
    {
        if (sem_init(&m_sem, 0, num) != 0)//初始化
        {
            throw std::exception();
        }
    }
    ~sem()
    {
        sem_destroy(&m_sem);
    }
    bool wait()
    {
        return sem_wait(&m_sem) == 0;//原子操作-1 为0时阻塞
    }
    bool post()
    {
        return sem_post(&m_sem) == 0;//返回0表示成功 原子操作+1
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
private:
    //static pthread_mutex_t m_mutex;
    pthread_cond_t m_cond;//pthread_cond_t 对象 条件变量
public:
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
    bool wait(pthread_mutex_t *m_mutex)//接受信号 和sign通常不是一个线程 阻塞等待一个条件变量
    {
        int ret = 0;
        //pthread_mutex_lock(&m_mutex);
        ret = pthread_cond_wait(&m_cond, m_mutex);
        //pthread_mutex_unlock(&m_mutex);
        return ret == 0;
    }
    bool timewait(pthread_mutex_t *m_mutex, struct timespec t)//限时等待一个条件变量 和sign通常不是一个线程
    {
        int ret = 0;
        //pthread_mutex_lock(&m_mutex);
        ret = pthread_cond_timedwait(&m_cond, m_mutex, &t);
        //pthread_mutex_unlock(&m_mutex);
        return ret == 0;
    }
    bool signal()
    {
        return pthread_cond_signal(&m_cond) == 0;//唤醒至少一个阻塞在条件变量上的线程  和wait通常不是一个线程
    }
    bool broadcast()
    {
        return pthread_cond_broadcast(&m_cond) == 0;//唤醒全部阻塞在条件变量上的线程
    }
};
#endif //WEBSERVER_LOCKER_H
