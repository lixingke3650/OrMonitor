/*********************************
 * OrMonitor 日志
 * logger.c
*********************************/

#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>

#include "utility.h"
#include "logger.h"


/*********************************
 * 内部变量
*********************************/
static int LOG_LEVEL;
static char LogFilePath[LOGFILEPATH_MAX];
static pthread_mutex_t LogFileMutex = PTHREAD_MUTEX_INITIALIZER;


/*********************************
 * 初始化
*********************************/
BOOL logger_init(char* filepath, int level)
{
    FILE *fp = NULL;
    
    if (level < LOG_LEVEL_MIN || level > LOG_LEVEL_MAX)
    {
        return (FALSE);
    }

    LOG_LEVEL = level;
    strcpy(LogFilePath, filepath);

    // log init newline
    // IO互斥锁 锁定
    pthread_mutex_lock(&LogFileMutex);
    // 写入文件
    fp = fopen( filepath, "ab" );
    if ( fp == NULL ){
        return;
    }
    fprintf( fp, "\n\n" );
    fclose( fp );
    // IO互斥锁 释放
    pthread_mutex_unlock(&LogFileMutex);

    return (TRUE);
}

/*********************************
 * 写入日志文件
*********************************/
void log_write(char* filepath, int logtype, char* msg)
{
    FILE *fp = NULL;
    struct tm *w_time;
    struct timeval tv;
    char logtypestr[LOGTYPESTR_MAX];
    char record[LOGRECORD_MAX];

    // 日志标识字串
    switch (logtype)
    {
        case LOG_ERR:
            strcpy( logtypestr, "ERR  " );
            break;

        case LOG_WARN:
            strcpy( logtypestr, "WARN " );
            break;

        case LOG_INFO:
            strcpy( logtypestr, "INFO " );
            break;

        case LOG_DEBUG:
            strcpy( logtypestr, "DEBGU" );
            break;

        default:
            strcpy( logtypestr, "OTHER" );
            break;
    }

    // 日志写入时间获取
    gettimeofday(&tv, NULL);
    w_time = localtime( &tv.tv_sec );

    // 日志生成
    snprintf( record, LOGRECORD_MAX,
             "[%04d-%02d-%02d %02d:%02d:%02d.%03ld]  [%s]  %s%s",
             w_time->tm_year + 1900,
             w_time->tm_mon + 1,
             w_time->tm_mday,
             w_time->tm_hour,
             w_time->tm_min,
             w_time->tm_sec,
             tv.tv_usec/1000,
             logtypestr,
             msg,
             NEWLINE );

    // IO互斥锁 锁定
    pthread_mutex_lock(&LogFileMutex);
    // 写入文件
    fp = fopen( filepath, "ab" );
    if ( fp == NULL ){
        return;
    }
    fprintf( fp, "%s", record );
    fclose( fp );
    // IO互斥锁 释放
    pthread_mutex_unlock(&LogFileMutex);

    return;
}

/*********************************
 * err 日志
*********************************/
void log_err(char* msg)
{
    if (LOG_ERR > LOG_LEVEL)
    {
        return;
    }

    log_write(LogFilePath, LOG_ERR, msg);
}

/*********************************
 * warn 日志
*********************************/
void log_wrn(char* msg)
{
    if (LOG_WARN > LOG_LEVEL)
    {
        return;
    }

    log_write(LogFilePath, LOG_WARN, msg);
}

/*********************************
 * info 日志
*********************************/
void log_inf(char* msg)
{
    if (LOG_INFO > LOG_LEVEL)
    {
        return;
    }

    log_write(LogFilePath, LOG_INFO, msg);
}

/*********************************
 * debug 日志
*********************************/
void log_dbg(char* msg)
{
    if (LOG_DEBUG > LOG_LEVEL)
    {
        return;
    }

    log_write(LogFilePath, LOG_DEBUG, msg);
}