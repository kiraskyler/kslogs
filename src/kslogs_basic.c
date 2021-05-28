#include "kslogs_basic.h"

#include <stdio.h>
#include <string.h>

void kslogs_basic_init(s_kslogs *kslogs, char *log_mem, unsigned long int log_mem_max)
{
    kslogs->mess = log_mem;
    kslogs->mess_len = 0;
    kslogs->mess_max = log_mem_max;
    kslogs->is_log = true;
    pthread_mutex_init(&(kslogs->lock), NULL);
    pthread_cond_init(&(kslogs->cond), NULL);
}

void kslogs_basic_is_log(s_kslogs *kslogs, bool is_log)
{
    if(pthread_mutex_lock(&(kslogs->lock)) == 0)
    {
        kslogs->is_log = is_log;
        pthread_mutex_unlock(&(kslogs->lock));
    }
}

unsigned long int kslogs_basic_get_mess_len(s_kslogs *kslogs)
{
    if(kslogs == NULL)
        return 0;
    return kslogs->mess_len;
}

void kslogs_basic_signal(s_kslogs *kslogs)
{
    pthread_cond_signal(&kslogs->cond);
}

void kslogs_basic_send(s_kslogs *kslogs, char *mess, unsigned long int mess_len)
{
    if(kslogs == NULL)
        return;
    if(mess_len > kslogs->mess_max)
        return;
    if(kslogs->is_log == false)
        return;
    kslogs_dbg_printf("send");
    if(pthread_mutex_lock(&(kslogs->lock)) == 0)
    {
        kslogs_dbg_printf("send lock");
        if(mess_len + kslogs->mess_len >= kslogs->mess_max)                     // 不够空间了，把之前的清空
            kslogs->mess_len = 0;
        memcpy((void *)(kslogs->mess + kslogs->mess_len), (void *)mess, mess_len);
        kslogs->mess_len += mess_len;
        pthread_mutex_unlock(&(kslogs->lock));
        pthread_cond_signal(&(kslogs->cond));
        kslogs_dbg_printf("send unlock");
    }
}

void kslogs_basic_write(s_kslogs *kslogs, void *param, void (*write)(void *param, \
                        const char *mess, const unsigned long int mess_len))
{
    kslogs_dbg_printf("write start");
    if(pthread_mutex_lock(&kslogs->lock) == 0)
    {
        kslogs_dbg_printf("write lock");
        if(pthread_cond_wait(&kslogs->cond, &kslogs->lock) == 0)
        {
            kslogs_dbg_printf("write cond");
            write(param, kslogs->mess, kslogs->mess_len);
            memset(kslogs->mess, 0, kslogs->mess_len);
            kslogs->mess_len = 0;
            pthread_mutex_unlock(&(kslogs->lock));
            kslogs_dbg_printf("write unlock");
        }
    }
}
