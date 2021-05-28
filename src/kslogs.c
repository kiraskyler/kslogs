#include "kslogs.h"

#include <unistd.h>

int __attribute__((unused)) main(int argc, char **argv)
{
    s_kslogs_file kslogs_file;
    kslogs_file_init(&kslogs_file,  "/home/kira/Desktop/Algorithm/kslogs/src/a.log");    // 按目录，将会将每天的日志保存到单独的文件
    kslogs_file_start(&kslogs_file);
    KSLOGS_SEND(&kslogs_file, "hello world 0");
    KSLOGS_SEND(&kslogs_file, "hello world 1");
    KSLOGS_SEND(&kslogs_file, "hello world 2");
    KSLOGS_SEND(&kslogs_file, "hello world 3");
    kslogs_file_clean(&kslogs_file);
}