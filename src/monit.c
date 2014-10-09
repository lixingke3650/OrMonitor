/*********************************
 * OrMonitor 启动程序
 * monit.c
*********************************/

#include <stdio.h>
#include <string.h>

#include "utility.h"
#include "logger.h"
#include "proc.h"
#include "itimer.h"
#include "monit.h"


/*********************************
 * main 函数
*********************************/
void main(int arvc, char* argv[])
{
    int cycle = 0;  // 监视周期(ms)

    if ( TRUE != logger_init(LOG_PATH, LOG_DEBUG))
    {
        // log初始化失败
        //log_err("logger_init err!");
        return;
    }

    if ( TRUE != proc_init(MONITLIST_PATH, &cycle))
    {
        log_err("proc_init err!");
        return;
    }

    if ( TRUE != itimer_start(cycle, &routine))
    {
        log_err("itimer_start err!");
        return;
    }

    // 启动成功 log
    log_inf("OrMonitor Start.");

    // 等待定时器
    while (1)
    {
        sleep(WAITTIME);
    }
}