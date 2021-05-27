#include "kslogs_file.h"

#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>

#include "kslogs_basic.h"

void kslogs_file_status_reset(s_kslogs_file* kslogs_file, bool status)
{
    kslogs_file->is_log = status;
}

void kslogs_file_clean(s_kslogs_file* kslogs_file)
{
    void *ret;
    bool need_wait = false;
    kslogs_file->is_loop = false;
    int log_main_mess_len;
    if ((log_main_mess_len = kslogs_basic_get_mess_len(&kslogs_file->kslogs)) > 0)
    {
        kslogs_dbg_printf("%d", log_main_mess_len);
        need_wait = true;
    }
    if (need_wait == true)
        sleep(1);
    kslogs_basic_signal(&kslogs_file->kslogs);
    pthread_cancel(kslogs_file->thread);
    pthread_join(kslogs_file->thread, &ret);
    if (kslogs_file->fd > 0)
    {
        kslogs_dbg_printf("close kslogs_file->fd : %d", kslogs_file->fd);
        close(kslogs_file->fd);
    }
}

void kslogs_file_init(s_kslogs_file* kslogs_file, const char* file_name)
{
    kslogs_file->file_path = (char* )file_name;
    kslogs_file->is_log = true;
    kslogs_file->is_loop = true;

    static char log_mem_main[KSLOGS_MEM_MAIN_SIZE];
    kslogs_basic_init(&kslogs_file->kslogs, log_mem_main, KSLOGS_MEM_MAIN_SIZE);
}

static void kslogs_file_open_file(int *fd, char *file_path)
{
    if (*fd <= 0)
    {
        umask(0);
        if (access(file_path, F_OK) == 0)
            *fd = open(file_path, O_RDWR | O_APPEND);
        else
            *fd = open(file_path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if (*fd <= 0)
        {
            kslogs_dbg_printf("open %s, %d", file_path, errno);
            *fd = 0;
            return;
        }
    }
}

static void kslogs_file_main_write(void* param, const char *mess, const unsigned long int mess_len)
{
    s_kslogs_file* kslogs_file = param;
    kslogs_dbg_printf("write, %s", mess);
    kslogs_file_open_file(&kslogs_file->fd, kslogs_file->file_path);
    if (kslogs_file->fd <= 0)
        return;
    time_t tim = time(NULL);
    char time_asc[100];
    sprintf(time_asc, "[ts,%s", asctime(localtime(&tim)));
    sprintf(time_asc + strlen(time_asc) - 1, "]");
    write(kslogs_file->fd, time_asc, strlen(time_asc));
    write(kslogs_file->fd, mess, mess_len);
}

static void *kslogs_file_loop(void *arg)
{
    s_kslogs_file* kslogs_file = arg;

    while(kslogs_file->is_loop)
        kslogs_basic_write(&kslogs_file->kslogs, kslogs_file, kslogs_file_main_write);

    return NULL;
}

void kslogs_file_start(s_kslogs_file* kslogs_file)
{
    static bool is_start = false;

    if (is_start == false)
    {
        pthread_create(&kslogs_file->thread, NULL, kslogs_file_loop, kslogs_file);
        is_start = true;
    }
}

void kslogs_file_main_send(s_kslogs_file* kslogs_file, char *mess, unsigned long int mess_len)
{
    if (kslogs_file->is_log)
    {
        kslogs_dbg_printf(" %s", mess);
        kslogs_basic_send(&kslogs_file->kslogs, mess, mess_len);
    }
}
