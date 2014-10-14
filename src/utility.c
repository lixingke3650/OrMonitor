/*********************************
 * OrMonitor 工具
 * utility.c
*********************************/

#include <stdio.h>
#include <signal.h>
#include <sys/time.h>

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


