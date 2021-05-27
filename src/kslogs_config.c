#include "kslogs_config.h"

#include <sys/time.h>

long long kslogs_getms(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);

    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}
