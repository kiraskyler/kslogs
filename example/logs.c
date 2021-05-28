#include <kslogs/kslogs.h>

#include <unistd.h>

int main(int argv, char **argc)
{
    char  path[] = "./";            // 使用目录
    //  char path[] = "./mylog.log";    // 使用文件
    s_kslogs_file kslogs_file;
    printf("************ %lld *************\n", kslogs_getms());
    kslogs_file_init(&kslogs_file, path);
    kslogs_file_start(&kslogs_file);
    KSLOGS_SEND(&kslogs_file, "hello my friend, to %s", path);
    kslogs_file_clean(&kslogs_file);
    printf("************ %lld *************\n", kslogs_getms());
    return 0;
}