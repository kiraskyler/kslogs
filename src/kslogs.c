#include "kslogs.h"

#include <unistd.h>

int __attribute__((unused)) main(int argc, char** argv)
{
    s_kslogs_file kslogs_file;

    kslogs_file_init(&kslogs_file,  "/home/kira/Desktop/Algorithm/kslogs/src/a.txt");

    kslogs_file_start(&kslogs_file);
    sleep(1);

    KSLOGS_SEND(&kslogs_file, "hello world");
    sleep(1);

    kslogs_file_clean(&kslogs_file);
}