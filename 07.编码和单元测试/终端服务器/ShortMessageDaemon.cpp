#include <dlfcn.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "Common.h"
#include "ShortMessage.h"
#include "/opt/work/src/CCSS_Common.h"
const char *strCommand="/opt/work/bin/smsdaemon";
/*
int		nReturn;
	nReturn = funGet_Buss_Func()->fun_Regedit((char *)strCommand);
	if (nReturn == -1)
	{
		printf("程序[%s]已经注册,Monitor会把这个程序启动的\n", strCommand);
		return 0;
	}

	if (nReturn == -2)
	{
		printf("程序[%s]注册出错，退出\n", strCommand);
		return 0;
	}

	if (nReturn != 0)
	{
		printf("程序[%s]注册返回[%d]，退出\n", strCommand, nReturn);
		return 0;
	}

*/
int main(int argc, char **argv)
{
	/*初始化进程环境*/
	funInit_Main();
	funInit_Print_Parameter_Value();
	//
	OLFO oracle;
	initOracleConnection(&oracle);
	while(true)
	{
		sleep(1);
		int ret=0;
		if((ret=sendShortMessages(&oracle))>0){
			//printf("处理指令最后发出: %s,一共处理 %d 条指令\n",getTempMessageBuffer(),ret);
			//continue;
		}		
		if(ret<0){
			tryToConnectOracle(&oracle, LOG_TERMINAL_POSITION);
		}

					
	}

	oracle.RollBack();
	oracle.Close();
	oracle.DisConnect();
	return 0;
}


