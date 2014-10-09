/*********************************
 * OrMonitor 配置读取与设置
 * conf.c
*********************************/

#include <stdio.h>

#include "conf.h"
#include "proc.h"


/*********************************
 * 内部变量
*********************************/
#ifndef D_PROC_OBJ
// 静态申请监视对象
static struct S_MonitObj MonitObj[MONITOROBJ_MAX];
#endif

/*********************************
 * 配置文件读取并存入监控对象
*********************************/
bool setConf(struct S_MonitObj* head, struct S_MonitObj* buf)
{

}



