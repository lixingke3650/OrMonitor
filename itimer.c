/*********************************
 * OrMonitor 定时器
 * itimer.c
*********************************/

#include <stdio.h>
#include <signal.h>
#include <sys/time.h>

#include "utility.h"
#include "itimer.h"

struct  itimerval It_Time;        // 定时器启动间隔(itimerval)


/*********************************
 * 信号处理函数
*********************************/
void sigroutine(int sig)
{
    switch (sig){
        case SIGALRM:
            printf("<-sig-> SIGALRM\n");
            break;

        default:
            break;
    }
}

/*********************************
 * 定时时间计算 (msec -> sec usec)
*********************************/
BOOL getItimerTime(int timems, int* sec, int* usec)
{
    *sec = timems / 1000;
    *usec = timems % 1000 * 1000;

    return (TRUE);
}

/*********************************
 * 初始化
*********************************/
void itimer_init(int msec)
{
    int sec, usec;

    getItimerTime(msec, &sec, &usec);

    // 每次定时开始时自动装载的定时时间
    It_Time.it_interval.tv_sec = sec;
    It_Time.it_interval.tv_usec = msec;
    // 初始定时时间
    It_Time.it_value.tv_sec = FIRST_TIME_SEC;
    It_Time.it_value.tv_usec = FIRST_TIME_MSEC * 1000;
}

/*********************************
 * 定时器开始
*********************************/
BOOL itimer_start(int msec)
{
    // 定时时间设定
    itimer_init(msec);

    // 信号处理注册
    signal(SIGALRM, sigroutine);

    // 定时器启动
    setitimer(ITIMER_REAL, &It_Time, NULL);

    return (TRUE);
}

/*********************************
 * 定时器停止
*********************************/
BOOL itimer_stop()
{
    // 定时时间设定
    itimer_init(0);

    // 信号处理注册
    signal(SIGALRM, sigroutine);

    // 定时器启动
    setitimer(ITIMER_REAL, &It_Time, NULL);

    return (TRUE);
}

/*********************************
 * 测试用主函数
*********************************/
/*
int main(int argc, char* argv[])
{
    // itimerval结构体
    // 定时时间
    struct itimerval it_time;

    // 每次定时开始时自动装载的定时时间
    it_time.it_interval.tv_sec = 1;
    it_time.it_interval.tv_usec = 0;
    // 初始定时时间
    it_time.it_value.tv_sec = 0;
    it_time.it_value.tv_usec = 100000; // 0.1S

    // 信号处理注册
    signal(SIGALRM, sigroutine);

    // 定时器启动
    setitimer(ITIMER_REAL, &it_time, NULL);

    // 无限循环等待信号
    while (1)
    {
        sleep(10000);
    }
}
*/
