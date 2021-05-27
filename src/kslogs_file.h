#ifndef __KSLOGS_FILE_H_
#define __KSLOGS_FILE_H_

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>

#include "kslogs_basic.h"
#include "kslogs_config.h"

#define KSLOGS_MEM_MAIN_SIZE         2048

#define KSLOGS_SEND(kslogs_file, fmt, args...)    \
{\
    char kslogs_mess_send[200]; \
    sprintf(kslogs_mess_send, ""fmt"\n",  ##args);\
    kslogs_file_main_send(kslogs_file, kslogs_mess_send, strlen(kslogs_mess_send));\
}

typedef struct s_kslogs_file
{
    s_kslogs    kslogs;
    int         fd;
    char*       file_path;
    pthread_t   thread;
    bool        is_log;
    bool        is_loop;
} s_kslogs_file;

extern void kslogs_file_init(s_kslogs_file* kslogs_file, const char* file_name);
extern void kslogs_file_status_reset(s_kslogs_file* kslogs_file, bool status);
extern void kslogs_file_clean(s_kslogs_file* kslogs_file);
extern void kslogs_file_start(s_kslogs_file* kslogs_file);
extern void kslogs_file_main_send(s_kslogs_file* kslogs_file, char* mess, unsigned long int mess_len);

#endif
