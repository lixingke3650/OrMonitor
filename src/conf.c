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
BOOL setConf(char* filepath, struct S_MonitObj* omolist)
{
    FILE *fp;
    int count_Obj;
    struct S_MonitObj *tmpobj;
    int count_line;
    char linedata[CONF_LINE_MAX];
    char confname[CONF_NAME_MAX];
    char confparam[CONF_PRM_MAX];
    int confnamecount;
    int confparamcount;
    int target;                            // 0：不写入 1：项目名 2：项目参数  


    if ( NULL == (fp = fopen( filepath, "rb" ) ) )
    {
        log_err("conf file not found!");
        return (FALSE);
    }

    count_Obj = 0;

    while (1)
    {
        tmpobj = NULL;

        memset( linedata, 0x0, PID_LINE_MAX );
        if ( NULL == fgets( linedata, CONF_LINE_MAX, fp ) )
        {
            // conf文件结束
            break;
        }

        count_line = 0;
        memset( confname, 0x0, CONF_NAME_MAX );
        memset( confparam, 0x0, CONF_PRM_MAX );
        confnamecount = 0;
        confparamcount = 0;
        target = 1;

        // 行解析开始
        while (1)
        {
            // 行结束
            if (linedata[count_line] == '\0')
            {
                break;
            }

            // 注释
            if (linedata[count_line] == ';' || linedata[count_line] == '#')
            {
                // 行结束
                break;
            }
            // 对象开始 (前一对象结束)
            else if (linedata[count_line] == '[')
            {
                target = 0;

                // 监控对象准备
                if ( tmpobj != NULL )
                {
                    tmpobj->pNext = MonitObj[count_Obj+1];
                    tmpobj = MonitObj[count_Obj+1];
                    count_Obj++;
                }
                else
                {
                    tmpobj = MonitObj[0];
                }
            }
            else if (linedata[count_line] == '=')
            {
                if (target != 2)
                {
                    target = 2;
                    continue;
                }
            }
            
            // 数据写入
            switch (target)
            {
                case 0:
                    break;

                case 1:
                    confname[confnamecount] = linedata[count_line];
                    confnamecount ++;
                    break;

                case 2:
                    confparam[confparamcount] = linedata[count_line];
                    confparamcount ++;
                    break;

                default:
                    log_err("conf-setConf-switch error!");
                    break;
                
            }

            count_line ++;
        }

        //>>>>>>>>>>>
        printf("confname:");
        printf("");
        //<<<<<<<<<<<

        // 未设置对象实例
        if (tmpobj == NULL)
        {
            break;
        }

        if ( 0 == strcmp(CONF_NAME, confname) )
        {
            strcpy(tmpobj->ProcObj.Name, confparam);
        }
        else if ( 0 == strcmp(CONF_BIN, confname) )
        {
            strcpy(tmpobj->ProcObj.BinFilePath, confparam);
        }
        else if ( 0 == strcmp(CONF_PARAME, confname) )
        {
            strcpy(tmpobj->ProcObj.BinPrm, confparam);
        }
        else if ( 0 == strcmp(CONF_PIDFILE, confname) )
        {
            strcpy(tmpobj->ProcObj.PidFilePath, confparam);
        }
        else if ( 0 == strcmp(CONF_CYCLE, confname) )
        {
            tmpobj->ProcObj.Cycle = atoi(confparam);
        }
    }

}



