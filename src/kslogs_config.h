#ifndef __KSLOGS_CONFIG_H_
#define __KSLOGS_CONFIG_H_

#include <stdio.h>

// #define     KSLOGS_DEBUG

#ifdef      KSLOGS_DEBUG
#define     kslogs_dbg_printf(fmt, args...)             (printf("\t%s,\t%s,%d %lld >> "fmt"\n", __FILE__, __FUNCTION__, __LINE__, kslogs_getms(), ##args))
#else
#define     kslogs_dbg_printf(fmt, arg...)              // nothing
#endif

extern long long kslogs_getms(void);

#endif
