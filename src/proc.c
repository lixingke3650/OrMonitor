/*********************************
 * OrMonitor 监视处理
 * proc.c
*********************************/

#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <malloc.h>

#include "utility.h"
#include "logger.h"
#include "conf.h"
#include "proc.h"



/*********************************
 * 内部变量
*********************************/
// 启动命令   %PidFilePath %BinFilePath %BinPrm
static char* StartFormat = "start-stop-daemon --start --quiet --pidfile %s -m --exec %s -- %s";

// 监控对象列表头指针
static struct S_MonitObj* MonitObjHead = NULL;

// 各监控对象检测周期最大公约数，即定时器启动间隔 (ms)
static int CycleGcd;

// 监控对象检测重启进程tid
static pthread_t ProcSubTid;

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
BOOL proc_init(struct S_MonitObj* objlist, int* ocycle)
{
    if (objlist == NULL)
    {
        return (FALSE);
    }

    MonitObjHead = objlist;

    if (TRUE != getItimer(&CycleGcd))
    {
        return (FALSE);
    }

    *ocycle = CycleGcd;
    ProcSubTid = -1;

    return (TRUE);
}

/*********************************
 * 子线程终止
*********************************/
BOOL proc_stop()
{
    if ( 0 < ProcSubTid ) 
    {
        // 线程是否生存
        if( 0 != pthread_kill( ProcSubTid, 0 ) )
        {
            // 发送停止信号
            if ( 0 != pthread_cancel( ProcSubTid ) )
            {
                return (FALSE);
            }
        }

        // 线程id重置
        ProcSubTid = -1;
    }

    return( TRUE );
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
 * 程序运行否检测
 *   pid文件是否存在
 *   pid指定的进程是否存在
 *   return: 0-pid文件存在且运行中 1-pid文件不存在或未运行 -1-系统错误
*********************************/
int checkobj(struct S_MonitObj* mo)
{
    FILE *fp;
    char linedata[PID_LINE_MAX];

    // 判断文件是否存在
    if ( 0 != access( mo->ProcObj.PidFilePath, 0 ) )
    {
        return (1);
    }

    // pid获取
    //if ( mo->ProcObj.Pid <= 0 ){
    // 读取pid
    if( (fp = fopen( mo->ProcObj.PidFilePath, "rb" )) == NULL )
    {
        log_err("proc-checkobj-fopen err!");
        return (-1);
    }
    memset( linedata, 0x0, PID_LINE_MAX );
    fgets( linedata, PID_LINE_MAX, fp );
    fclose( fp );
    // log_dbg(linedata);
    if ( (mo->ProcObj.Pid = atoi( linedata )) <= 0 )
    {
        log_err("proc-checkobj-atoi err!");
        return (-1);
    }
    //}

    // 判断pid指定进程是否存在
    if ( kill(mo->ProcObj.Pid, 0) != 0 )
    {
        log_wrn("process no life!");
        // 进程不存在，则删除pid文件
        if ( 0 != remove(mo->ProcObj.PidFilePath) )
        {
            log_err("proc-checkobj-remove err!");
            return (-1);
        }

        return (1);
    }

    // 判断/proc文件夹下是否存在指定的pid文件夹
    // 保留

    return (0);
}

/*********************************
 * 程序启动
*********************************/
BOOL startobj(struct S_MonitObj* mo)
{
    char command[STARTCOMMAND_MAX];

    snprintf(command, STARTCOMMAND_MAX, StartFormat, mo->ProcObj.PidFilePath, mo->ProcObj.BinFilePath, mo->ProcObj.BinPrm);

    //>>
    printf("command: %s\n", command);
    //<<

    if ( 0 > system(command) )
    {
        log_wrn("restart may fail.");
    }

    log_inf("restart success.");

    return (TRUE);
}

/*********************************
 * 检测与再启动
*********************************/
void checkandrestart(struct S_MonitObj* mo)
{
    int ret;

    mo->ProcObj.DownTime -= CycleGcd;
    if (mo->ProcObj.DownTime <= 0)
    {
        ret = checkobj(mo);
        if (ret < 0)
        {
            // 出现系统错误
            log_err("checkobj 函数执行失败！");
        }
        else if (ret > 0)
        {
            // 监视进程停止，再启动
            startobj(mo);
        }
        else
        {
            // 监视进程存在
        }

        // 计时重置
        mo->ProcObj.DownTime = mo->ProcObj.Cycle;
    }
}

/*********************************
 * 定时信号处理
 * 防止处理时间过长影响计时，此处新建线程处理
*********************************/
void* porcthread( void* arg )
{
    int oldstate;
    struct S_MonitObj* tmp;

    // cancel 有效
    pthread_setcancelstate( PTHREAD_CANCEL_ENABLE, &oldstate );
    // cancel 的执行时间 此处为等待取消点(同步)
    pthread_setcanceltype( PTHREAD_CANCEL_DEFERRED, &oldstate );
    // 线程结束后 自动释放内存
    pthread_detach( pthread_self() );

    if ( MonitObjHead == NULL )
    {
        // 线程终止
        pthread_exit( NULL );
    }

    // 对象检测
    tmp = MonitObjHead;
    while(1)
    {
        checkandrestart(tmp);
        if (tmp->pNext == NULL)
        {
            // 线程终止
            pthread_exit( NULL );
        }
        tmp = tmp->pNext;
    }

    // 取消点 设置
    pthread_testcancel();

    // 线程终止
    pthread_exit( NULL );
}

/*********************************
 * 定时信号处理
 * 防止处理时间过长影响计时，此处新建线程处理
*********************************/
void routine()
{
    if ( 0 > ProcSubTid )
    {
        log_err("proc thread starting!");
    }

    if( 0 != pthread_create( &ProcSubTid, NULL, porcthread, NULL ) )
    {
        log_err("proc thread start fail!");
        return;
    }
}


