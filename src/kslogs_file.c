#include "kslogs_file.h"

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>
#include <sched.h>

#include "kslogs_basic.h"

void kslogs_file_status_reset(s_kslogs_file* kslogs_file, bool status)
{
    kslogs_basic_is_log(&kslogs_file->kslogs, status);
}

/*
    微妙延迟
*/
static int __attribute__((unused)) kslogs_tools_delay_ms(const unsigned int ms)
{
    char mess[100];
    memset(mess, 0, sizeof(mess));
    sprintf(mess, "sleep %f 1> /dev/null 2>&1", (double)ms / (double)1000);
    return system(mess);
}

void kslogs_file_clean(s_kslogs_file* kslogs_file)
{
    void* ret;
    int thread_wait = 0;

    kslogs_basic_is_log(&kslogs_file->kslogs, false);

    while(true)
    {
        kslogs_basic_signal(&kslogs_file->kslogs);
        int mess_len = kslogs_basic_get_mess_len(&kslogs_file->kslogs);
        if(thread_wait++ > (KSLOGS_THREAD_KILL_WAIT * 50) || mess_len == 0)
            break;
        kslogs_dbg_printf("%d", mess_len);
        sched_yield();
        kslogs_tools_delay_ms(20);
    }

    pthread_cancel(kslogs_file->thread);
    pthread_join(kslogs_file->thread, &ret);
    if(kslogs_file->fd > 0)
    {
        kslogs_dbg_printf("close kslogs_file->fd : %d", kslogs_file->fd);
        close(kslogs_file->fd);
    }
}

void kslogs_file_init(s_kslogs_file* kslogs_file, const char* file_name)
{
    kslogs_file->user_log_path = (char*)file_name;
    kslogs_file->is_loop = true;
    static char log_mem_main[KSLOGS_MEM_MAIN_SIZE];

    pthread_mutex_init(&kslogs_file->file_attr_mutex, NULL);
    kslogs_basic_init(&kslogs_file->kslogs, log_mem_main, KSLOGS_MEM_MAIN_SIZE);
}

/**
 * 判断 file_path 是否为一个文件夹的路径
 * 只会判断一次，当初始路径为目录时，返回true，后续将一直返回true
*/
static bool kslogs_file_path_is_dir(char* file_path)
{
    static bool is_checked = false;
    static bool file_path_is_dir = false;

    if(is_checked == false)
    {
        struct stat file_stat;
        if(stat(file_path, &file_stat) != 0)
        {
            file_path_is_dir = false;
            kslogs_dbg_printf("%s can't reach", file_path);
        }
        else
        {
            if(S_ISDIR(file_stat.st_mode) == true)
                file_path_is_dir = true;
            else
                file_path_is_dir = false;
        }
    }

    return file_path_is_dir;
}

/**
 * 当目录为文件夹类型时，自动按时间生成日志新的目录
 * 如果本身路径不是目录，直接返回，按用户指定的路径为准
 * 如果路径本身是目录，使用按时间作为文件名的日志
 *
 * file_path 原始目录
 * new_file_path 新目录
*/
static bool kslogs_file_switch(char* file_path, char* new_file_path)
{
    if(kslogs_file_path_is_dir(file_path) == false)
    {
        new_file_path = file_path;
        return false;
    }

    static time_t time_new_file = 0;
    static char   time_str[20];
    static char   file_path_new[200];
    static bool   is_first = false;

    time_t time_now = time(NULL);
    new_file_path = file_path_new;

    if((time_now / KSLOGS_DIR_CYCLE_FILE_TIME != time_new_file / KSLOGS_DIR_CYCLE_FILE_TIME) || is_first == false)
    {
        struct tm* time_local;
        time_local = localtime(&time_now);
        kslogs_dbg_printf("%s", asctime(time_local));

        sprintf(time_str, "%04d%02d%02d.log", 1900 + time_local->tm_year, time_local->tm_mon + 1, time_local->tm_mday);
        if(file_path[strlen(file_path) - 1] == '/')
            sprintf(file_path_new, "%s%s", file_path, time_str);
        else
            sprintf(file_path_new, "%s/%s", file_path, time_str);

        time_new_file = time_now;
        is_first = true;

        return true;
    }

    return false;
}

static void kslogs_file_open_file(s_kslogs_file* kslogs_file)
{
    if(pthread_mutex_lock(&kslogs_file->file_attr_mutex) == 0)
    {
        if(kslogs_file_switch(kslogs_file->user_log_path, kslogs_file->w_log_path) == true)
        {
            if(kslogs_file->fd > 0)
                close(kslogs_file->fd);

            kslogs_file->fd = 0;
            kslogs_dbg_printf("new path : %s", kslogs_file->w_log_path);
        }

        if(kslogs_file->fd <= 0)
        {
            umask(0);

            if (kslogs_file_path_is_dir(kslogs_file->user_log_path) == true)
            {
                if (access(kslogs_file->user_log_path, F_OK) != 0)
                    if (mkdir(kslogs_file->user_log_path, 0755) != 0)
                        kslogs_dbg_printf("mkdir %s erro, %d", kslogs_file->user_log_path, errno);
            }

            if(access(kslogs_file->w_log_path, F_OK) == 0)
                kslogs_file->fd = open(kslogs_file->w_log_path, O_RDWR | O_APPEND);
            else
                kslogs_file->fd = open(kslogs_file->w_log_path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

            if(kslogs_file->fd <= 0)
            {
                kslogs_dbg_printf("open %s erro, %d", kslogs_file->w_log_path, errno);
                kslogs_file->fd = 0;
            }
        }

        pthread_mutex_unlock(&kslogs_file->file_attr_mutex);
    }
}

static void kslogs_file_main_write(void* param, const char* mess, const unsigned long int mess_len)
{
    if(mess_len == 0)
        return;

    s_kslogs_file* kslogs_file = param;
    kslogs_dbg_printf("write, len:%ld, %s", mess_len, mess);

    kslogs_file_open_file(kslogs_file);
    if(kslogs_file->fd <= 0)
        return;

    #ifdef KSLOGS_FILE_W_WITH_TS
    time_t tim = time(NULL);
    char time_asc[100];
    sprintf(time_asc, "[ts,%s", asctime(localtime(&tim)));
    sprintf(time_asc + strlen(time_asc) - 1, "]");
    write(kslogs_file->fd, time_asc, strlen(time_asc));
    #endif

    write(kslogs_file->fd, mess, mess_len);
}

static void* kslogs_file_loop(void* arg)
{
    s_kslogs_file* kslogs_file = arg;

    while(kslogs_file->is_loop)
        kslogs_basic_write(&kslogs_file->kslogs, kslogs_file, kslogs_file_main_write);

    return NULL;
}

void kslogs_file_start(s_kslogs_file* kslogs_file)
{
    static bool is_start = false;
    if(is_start == false)
    {
        pthread_create(&kslogs_file->thread, NULL, kslogs_file_loop, kslogs_file);
        sched_yield();
        is_start = true;
    }
}

void kslogs_file_main_send(s_kslogs_file* kslogs_file, char* mess, unsigned long int mess_len)
{
    kslogs_dbg_printf("%s", mess);
    kslogs_basic_send(&kslogs_file->kslogs, mess, mess_len);
}
