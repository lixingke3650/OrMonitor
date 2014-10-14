OrMonitor
=========

Linux 程序监控工具


----
用于监控服务或后端进程，退出后可自行重启。  
  
#### 使用前提：  
1. 已安装start-stop-daemon工具  
2. 需指定监控进程的pid文件(若监控对象不生成，则由start-stop-daemon自动生成，请参考ormonitor.conf中的配置样例)  
  (程序中默认使用了**start-stop-daemon**的 **-pidfile** 和 **-m** 参数)

#### 执行原理：  
1. 判断pid文件是否存在，不存在则认为监控对象未启动  
2. 读取pid文件中保存的pid号，并判断该pid进程是否存在  
3. 监控周期使用定时器(setitimer)实现  

#### 待完成：  
1. 以守护进程形式启动

2014.10.14
