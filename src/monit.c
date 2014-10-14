/*********************************
 * OrMonitor 启动程序
 * monit.c
*********************************/

#include <stdio.h>
#include <string.h>

#include "utility.h"
#include "logger.h"
#include "conf.h"
#include "proc.h"
#include "itimer.h"
#include "monit.h"


/*********************************
 * main 函数
*********************************/
void main(int arvc, char* argv[])
{
    int cycle = 0;  // 监视周期(ms)
    struct S_MonitObj* monitobj = NULL;

    if ( TRUE != setpidfile(PIDFILE_PATH) )
    {
        // pid 文件写入失败
        outcrash("setpidfile error.");

        return;
    }

    if ( TRUE != logger_init(LOG_PATH, LOG_DEBUG))
    {
        // log初始化失败
        outcrash("logger_init error.");

        return;
    }
    log_inf("logger_init success.");

    if ( TRUE != setconf(CONF_PATH, &monitobj) )
    {
        log_err("setconf error.");
        goto enderr;
    }
    log_inf("setconf success.");

    if ( TRUE != proc_init(monitobj, &cycle))
    {
        log_err("proc_init error.");
        goto enderr;
    }
    log_inf("proc_init success.");

    if ( TRUE != itimer_start(cycle, &routine))
    {
        log_err("itimer_start error.");
        goto enderr;
    }
    log_inf("itimer_start success. **cycle: %d", cycle);

    // 启动成功 log
    log_inf("*** OrMonitor Start Success.");

    // 等待定时器
    while (1)
    {
        sleep(WAITTIME);
    }
    return;

enderr:
    log_err("*** OrMonitor Start fail.");
    return;
}