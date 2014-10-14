/*********************************
 * OrMonitor 日志
 * logger.h
*********************************/

#ifndef __LOGGER_H__
#define __LOGGER_H__


/*********************************
 * 定值
*********************************/
#define LOGFILEPATH_MAX    (256)
#define LOGRECORD_MAX      (512)
#define LOGTYPESTR_MAX     (8)

#define LOG_ERR            (0)
#define LOG_WARN           (1)
#define LOG_INFO           (2)
#define LOG_DEBUG          (3)

#define LOG_LEVEL_MIN      (0)
#define LOG_LEVEL_MAX      (3)


/*********************************
 * 变量
*********************************/


/*********************************
 * 函数
*********************************/
BOOL logger_init(char* filepath, int level);
void log_err(char* msg);
void log_wrn(char* msg);
void log_inf(char* msg);
void log_dbg(char* msg);

#endif // __LOGGER_H__

