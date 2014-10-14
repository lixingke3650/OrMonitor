/*********************************
 * OrMonitor 工具
 * utility.c
*********************************/

#include <stdio.h>

#include "utility.h"


/*********************************
 * 最大公约数
*********************************/
int gcd(int lhs, int rhs)
{
    int a,b,c;

    if (rhs == 0)
    {
        return (0);
    }

    a = lhs;
    b = rhs;
    c = a%b;

    while(c){
        a = b;
        b = c;
        c = a % b;
    }

    return (b);
}

/*********************************
 * pid文件写入
*********************************/
BOOL setpidfile(char* filepath)
{
    if (filepath == NULL)
    {
        return (FALSE);
    }

    FILE *fp = NULL;

    fp = fopen( filepath, "w" );
    if ( fp == NULL )
    {
        // pid 文件创建写入失败
        return (FALSE);
    }
    if ( 0 >= fprintf( fp, "%d\n", getpid() ) )
    {
        fclose( fp );
        return (FALSE);
    }

    fclose( fp );
    return (TRUE);
}

/*********************************
 * 异常情况信息输出 通常用于log不可用时
*********************************/
void outcrash(char* msg)
{
    printf (msg);
}