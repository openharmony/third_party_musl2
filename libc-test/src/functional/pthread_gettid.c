#include <pthread.h>
#include <string.h>
#include "test.h"
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#define TEST(c) ((c) ? 1 : (t_error(#c" failed: %s \n" ,strerror(errno)),0))

static pthread_mutex_t mutex ;

void* pthread_test(void* arg)
{
    *((pid_t *)arg) = getpid();
    pthread_mutex_lock(&mutex);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main(int argc, char const *argv[])
{
    pthread_mutex_init(&mutex, NULL);

    TEST(getpid() == pthread_gettid(pthread_self()));//用通过主线程id获取的进程id与用getpid()获取的进程id比较

    pthread_mutex_lock(&mutex);
    pid_t pid;
    pthread_t t;
    pthread_create(&t,NULL,pthread_test,&pid);//创建线程

    pid_t recv_result = pthread_gettid(t);//利用线程id获取的进程id

    TEST(0 == pthread_join(t,NULL));//结合线程并且将退出的线程在链表中删除

    pthread_mutex_unlock(&mutex);

    TEST(pid == recv_result);//将线程中获取的进程id与通过用线程id获取的进程id进行比较

    return 0;
}
