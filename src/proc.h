/*********************************
 * OrMonitor 监视处理
 * proc.h
*********************************/

#ifndef __PORC_H__
#define __PORC_H__

#include <sys/time.h>
#include <pthread.h>

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
// 监控对象信息
struct S_ProcObj
{
    char    Name[PROCNAME_MAX];                // 监控程序名
    char    PidFilePath[PIDFILEPATH_MAX];      // 监控程序PID文件路径(含pid文件名)
    char    BinFilePath[BINFILEPATH_MAX];      // 程序启动文件
    char    BinPrm[BINPRMPATH_MAX];            // 程序启动参数
    pid_t   Pid;                               // 程序运行时pid
    //struct  itimerval It_Time;                // 检测间隔(itimerval)
    int     Cycle;                             // 检测周期(ms)
    int     DownTime;                          // 检测剩余时间(ms)
};

// 监控对象(列表)
struct S_MonitObj
{
    struct S_ProcObj ProcObj;
    struct S_MonitObj* pNext;
};


/*********************************
 * 函数
*********************************/
void routine();
BOOL proc_init(char* filepath, int* ocycle);
BOOL proc_stop();
void freeMonitorObj();

#endif // __PROC_H__