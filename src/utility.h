/*********************************
 * OrMonitor 工具
 * utility.h
*********************************/

#ifndef __UTILITY_H__
#define __UTILITY_H__


/*********************************
 * 定值
*********************************/
#define u_int       unsigned int

#define BOOL        int
#define TRUE        (1)
#define FALSE       (0) 

#define NEWLINE     "\n"

/*********************************
 * 变量
*********************************/


/*********************************
 * 函数
*********************************/
void outcrash(char* msg);
int gcd(int lhs, int rhs);
BOOL setpidfile(char* filepath);

#endif // __UTILITY_H__

