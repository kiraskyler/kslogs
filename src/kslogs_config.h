#ifndef __KSLOGS_CONFIG_H_
#define __KSLOGS_CONFIG_H_

#include <stdio.h>

// #define     KSLOGS_DEBUG

#ifdef      KSLOGS_DEBUG
    #define     kslogs_dbg_printf(fmt, args...)             (printf("\t%s,\t%s %d %lld >> "fmt"\n", __FILE__, __FUNCTION__, __LINE__, kslogs_getms(), ##args))
#else
    #define     kslogs_dbg_printf(fmt, arg...)              // nothing
#endif

#define     KSLOGS_DIR_CYCLE_FILE_TIME                  (24 * 60 * 60)      // 每隔24小时更换一次文件记录，注：文件名为年月日
#define     KSLOGS_FILE_W_WITH_TS                       (1)                 // 写入文件时带上时间戳
#define     KSLOGS_THREAD_KILL_WAIT                     (3)

extern long long kslogs_getms(void);

#endif
