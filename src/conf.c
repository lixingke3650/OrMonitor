/*********************************
 * OrMonitor 配置读取与设置
 * conf.c
*********************************/

#include <stdio.h>
#include <string.h>

#include "utility.h"
#include "conf.h"


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
BOOL setconf(char* filepath, struct S_MonitObj** omolist)
{
    FILE *fp;
    int count_Obj;
    struct S_MonitObj *tmpobj;
    int count_line;
    char linedata[CONF_LINE_MAX];
    char confproc[PROCNAME_MAX];
    char confname[CONF_NAME_MAX];
    char confparam[CONF_PRM_MAX];
    int confprocnamecount;
    int confnamecount;
    int confparamcount;
    int target;                            // 0：不写入 1：项目名 2：项目参数 3：监控对象名


    if ( NULL == (fp = fopen( filepath, "rb" ) ) )
    {
        log_err("conf file not found!");
        return (FALSE);
    }

    count_Obj = 0;
    tmpobj = NULL;

    // 配置文件行循环读取
    while (1)
    {
        memset( linedata, 0x0, CONF_LINE_MAX );
        if ( NULL == fgets( linedata, CONF_LINE_MAX, fp ) )
        {
            // conf文件结束
            break;
        }

        count_line = -1;
        memset( confproc, 0x0, PROCNAME_MAX );
        memset( confname, 0x0, CONF_NAME_MAX );
        memset( confparam, 0x0, CONF_PRM_MAX );
        confprocnamecount = 0;
        confnamecount = 0;
        confparamcount = 0;
        target = 1;

        // 行解析开始
        while (1)
        {
            count_line ++;

            // 行结束
            if (linedata[count_line] == '\0' || linedata[count_line] == NEWLINE[0])
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
                target = 3;
                continue;
            }
            else if (linedata[count_line] == ']')
            {
                //>>>>>>>>>>>
                printf("confproc: %s\n", confproc);
                //<<<<<<<<<<<

                target = 0;

                // 监控对象准备
                if ( tmpobj != NULL )
                {
                    tmpobj->pNext = &(MonitObj[count_Obj+1]);
                    tmpobj = &(MonitObj[count_Obj+1]);
                    count_Obj++;

                    // 超出最大监视对象数
                    if (count_Obj >= MONITOROBJ_MAX)
                    {
                        log_err("MONITOROBJ_MAX error.");
                        close(fp);
                        fp = NULL;
                        return (FALSE);
                    }
                }
                else
                {
                    tmpobj = &(MonitObj[0]);
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
                    continue;

                // 项目名
                case 1:
                    // 项目名不能包含空格
                    if (linedata[count_line] != ' ')
                    {
                        confname[confnamecount] = linedata[count_line];
                        confnamecount ++;
                    }
                    continue;

                // 项目参数
                case 2:
                    confparam[confparamcount] = linedata[count_line];
                    confparamcount ++;
                    continue;

                // 监控对象名
                case 3:
                    confproc[confprocnamecount] = linedata[count_line];
                    confprocnamecount ++;
                    continue;

                default:
                    log_err("conf-setConf-switch error!");
                    continue;
                
            }
        }

        // 未设置对象实例
        if (tmpobj == NULL)
        {
            continue;
        }

        if ( 0 == strcmp(CONF_NAME, confname) )
        {
            //>>>>>>>>>>>
            printf("confname: %s\n", confname);
            printf("confparam: %s\n", confparam);
            //<<<<<<<<<<<
            strcpy(tmpobj->ProcObj.Name, confparam);
        }
        else if ( 0 == strcmp(CONF_BIN, confname) )
        {
            //>>>>>>>>>>>
            printf("confname: %s\n", confname);
            printf("confparam: %s\n", confparam);
            //<<<<<<<<<<<
            strcpy(tmpobj->ProcObj.BinFilePath, confparam);
        }
        else if ( 0 == strcmp(CONF_PARAME, confname) )
        {
            //>>>>>>>>>>>
            printf("confname: %s\n", confname);
            printf("confparam: %s\n", confparam);
            //<<<<<<<<<<<
            strcpy(tmpobj->ProcObj.BinPrm, confparam);
        }
        else if ( 0 == strcmp(CONF_PIDFILE, confname) )
        {
            //>>>>>>>>>>>
            printf("confname: %s\n", confname);
            printf("confparam: %s\n", confparam);
            //<<<<<<<<<<<
            strcpy(tmpobj->ProcObj.PidFilePath, confparam);
        }
        else if ( 0 == strcmp(CONF_CYCLE, confname) )
        {
            //>>>>>>>>>>>
            printf("confname: %s\n", confname);
            printf("confparam: %s\n", confparam);
            //<<<<<<<<<<<
            tmpobj->ProcObj.Cycle = atoi(confparam);
        }
    }

    close(fp);
    fp = NULL;

    if (tmpobj != NULL)
    {
        *omolist = MonitObj;
    }
    return (TRUE);
}

/*********************************
 * 监视对象释放
*********************************/
void freemonitorobj()
{
    /*
    struct S_MonitObj* tmp;
    struct S_MonitObj* nexttmp;
    tmp = MonitObjHead;

    while (1)
    {
        if ( tmp == NULL )
        {
            return;
        }

        nexttmp = tmp->pNext;
        free(tmp);
        tmp = nexttmp;
    }
    */
}
