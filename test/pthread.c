/******************************************************************************
 * xmo0 24.04.13
 *
 * A demo for multi-thread safety with POSIX pthread
 *
 * ****************************************************************************
 */

#include <unistd.h> // for usleep()
#include <pthread.h>
#include "../src/log.h"

pthread_mutex_t aMutex;

void *thread_a(void *arg);
void *thread_b(void *arg);
void  log_lock(bool lock, void *udata);

int main()
{
    pthread_mutex_init(&aMutex, NULL);
    log_set_console_level(LOGC_INFO);
    log_set_lock(log_lock, &aMutex); // comment this will cause multi-thread safety issues

    pthread_t tid_a, tid_b;
    if (pthread_create(&tid_a, NULL, thread_a, NULL))
    {
        printf("main -> create thread_a fail\n");
    }
    if (pthread_create(&tid_b, NULL, thread_b, NULL))
    {
        printf("main -> create thread_b fail\n");
    }

    pthread_join(tid_a, NULL);
    pthread_join(tid_b, NULL);

    pthread_mutex_destroy(&aMutex);
    printf("main return!\n");
}

void log_lock(bool lock, void *udata)
{
    pthread_mutex_t *LOCK = (pthread_mutex_t *)(udata);
    if (lock)
    {
        pthread_mutex_lock(LOCK);
    }
    else
    {
        pthread_mutex_unlock(LOCK);
    }
}

void *thread_a(void *arg)
{
    printf("thread_a start, thread id: %ld\n", pthread_self());
    // 因为上面的打印未使用mutex保护，延时以避免跟log_xxx系列的函数输出内容混在一起了
    usleep(1000 * 10);

    for (int i = 0; i < 20; i++)
    {
        log_info("aaaaaaaaaa %d", i);
        usleep(1);
    }
}

void *thread_b(void *arg)
{
    printf("thread_b start, thread id: %ld\n", pthread_self());
    // 因为上面的打印未使用mutex保护，延时以避免跟log_xxx系列的函数输出内容混在一起了
    usleep(1000 * 10);

    for (int i = 0; i < 20; i++)
    {
        log_fatal("bbbbbbbbbb %d", i);
        usleep(1);
    }
}