/*********************************
 * OrMonitor 监视处理
 * proc.c
*********************************/

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <malloc.h>

#include "utility.h"
#include "logger.h"
#include "proc.h"



/*********************************
 * 内部变量
*********************************/
// 监控对象列表头指针
static struct S_MonitObj* MonitObjHead = NULL;

// 各监控对象检测周期最大公约数，即定时器启动间隔 (ms)
static int CycleGcd;

#ifndef D_PROC_OBJ
// 静态申请监视对象
static struct S_MonitObj MonitObj[MONITOROBJ_MAX];
#endif


/*********************************
 * proc 初始化
 *   [in/   ]*filepath: 配置文件路径
 *   [  /out]*ocycle  : 需要的定时器周期(routine认定的周期)
 *   [return]true/false
*********************************/
BOOL proc_init(char* filepath, int* ocycle)
{
    if (TRUE != getMonitorObjOfFile(filepath))
    {
        return (FALSE);
    }

    if (TRUE != getItimer(&CycleGcd))
    {
        return (FALSE);
    }

    *ocycle = CycleGcd;
    return (TRUE);
}

/*********************************
 * 定时器周期计算
*********************************/
BOOL getItimer(int* cycle)
{
    int cycletmp;
    struct S_MonitObj* tmp;

    if (MonitObjHead == NULL)
    {
        return (FALSE);
    }

    tmp = MonitObjHead;
    cycletmp = tmp->ProcObj.Cycle;
    while (1)
    {
        if ( tmp->pNext == NULL)
        {
            *cycle = cycletmp;
            return (TRUE);
        }
        
        cycletmp = gcd(tmp->pNext->ProcObj.Cycle, cycletmp);
        tmp = tmp->pNext;
    }

    return (FALSE);
}

/*********************************
 * 监视配置文件读取
*********************************/
BOOL getMonitorObjOfFile(char* filepath)
{
// 静态申请，不需释放
#ifndef D_PROC_OBJ
    // 文件读取

    // 对象作成
    strcpy(MonitObj[0].ProcObj.Name, "nginx");
    strcpy(MonitObj[0].ProcObj.PidFilePath, "/usr/local/nginx/logs/nginx.pid");
    MonitObj[0].ProcObj.Cycle = 5000;

    strcpy(MonitObj[1].ProcObj.Name, "nginx");
    strcpy(MonitObj[1].ProcObj.PidFilePath, "/usr/local/nginx/logs/nginx.pid");
    MonitObj[1].ProcObj.Cycle = 2000;

    // 登录
    addMoniter(&MonitObj[0]);
    addMoniter(&MonitObj[1]);

// 动态申请内存存储监视对象，程序结束时请调用freeMonitorObj()释放
#else
    // 文件读取

    // 对象申请

    // 对象作成

    // 登录

#endif

    return (TRUE);
}

/*********************************
 * 监视对象释放
*********************************/
void freeMonitorObj()
{
    struct S_MonitObj* tmp;
    struct S_MonitObj* nexttmp;
    tmp = MonitObjHead;

    while (1)
    {
        if ( tmp == NULL )
        {
            return;
        }

        nexttmp = tmp->pNext;
        free(tmp);
        tmp = nexttmp;
    }
}

/*********************************
 * 监视登录
*********************************/
BOOL addMoniter(struct S_MonitObj* po)
{
    struct S_MonitObj* tmp;

    if (po == NULL)
    {
        return (FALSE);
    }

    po->pNext = NULL;

    // 初次添加
    if (MonitObjHead == NULL)
    {
        MonitObjHead = po;
        return (TRUE);
    }

    // 复数添加
    tmp = MonitObjHead;
    while (1)
    {
        if ( tmp->pNext == NULL)
        {
            tmp->pNext = po;
            return (TRUE);
        }
        tmp = tmp->pNext;
    }

    return (FALSE);
}

/*********************************
 * 定时信号处理
*********************************/
void routine()
{
    
}


