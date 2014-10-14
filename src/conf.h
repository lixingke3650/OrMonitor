/*********************************
 * OrMonitor 配置读取与设置
 * conf.h
*********************************/

#ifndef __CONF_H__
#define __CONF_H__


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
#define CONF_LINE_MAX       (256)
#define CONF_NAME_MAX       (16)
#define CONF_PRM_MAX        (256)

#define CONF_NAME           "name"
#define CONF_BIN            "bin"
#define CONF_PARAME         "parame"
#define CONF_PIDFILE        "pidfile"
#define CONF_CYCLE          "cycle"

/*********************************
 * 变量
*********************************/
// 监控对象信息
struct S_ProcObj
{
    char    Name[PROCNAME_MAX];                // 监控程序名
    char    BinFilePath[BINFILEPATH_MAX];      // 程序启动文件
    char    BinPrm[BINPRMPATH_MAX];            // 程序启动参数
    char    PidFilePath[PIDFILEPATH_MAX];      // 监控程序PID文件路径(含pid文件名)
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
BOOL setconf(char* filepath, struct S_MonitObj** omolist);
void freemonitorobj();


#endif // __CONF_H__

