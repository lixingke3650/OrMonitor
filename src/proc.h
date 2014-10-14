/*********************************
 * OrMonitor 监视处理
 * proc.h
*********************************/

#ifndef __PORC_H__
#define __PORC_H__

#include <sys/time.h>
#include <pthread.h>

#include "conf.h"

/*********************************
 * 定值
*********************************/
//#define D_PROC_OBJ

#define MONITOROBJ_MAX      (32)
#define PROCNAME_MAX        (64)
#define PIDFILEPATH_MAX     (256)
#define BINFILEPATH_MAX     (256)
#define BINPRMPATH_MAX      (256)
#define STARTCOMMAND_MAX    (512)
#define PID_LINE_MAX        (8)


/*********************************
 * 变量
*********************************/


/*********************************
 * 函数
*********************************/
void routine();
BOOL proc_init(struct S_MonitObj* objlist, int* ocycle);
BOOL proc_stop();
void freeMonitorObj();

#endif // __PROC_H__