#ifndef __KSLOGS_BASIC_H_
#define __KSLOGS_BASIC_H_

#include <pthread.h>

#include "kslogs_config.h"

// #define     kslogs_DEBUG

typedef struct s_kslogs
{
    char* mess;
    unsigned long int mess_len;
    unsigned long int mess_max;

    pthread_mutex_t lock;
    pthread_cond_t cond;
} s_kslogs;

extern void kslogs_basic_init(s_kslogs* kslogs, char* log_mem, unsigned long int log_mem_max);
extern unsigned long int kslogs_basic_get_mess_len(s_kslogs* kslogs);
extern void kslogs_basic_send(s_kslogs* kslogs, char* mess, unsigned long int mess_len);
extern void kslogs_basic_write(s_kslogs *kslogs, void* parma, void (*write)(void* param,\
                               const char *mess, const unsigned long int mess_len));
extern void kslogs_basic_signal(s_kslogs *kslogs);                      // 可以避免线程死锁

#endif
