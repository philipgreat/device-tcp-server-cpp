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
		printf("����[%s]�Ѿ�ע��,Monitor����������������\n", strCommand);
		return 0;
	}

	if (nReturn == -2)
	{
		printf("����[%s]ע������˳�\n", strCommand);
		return 0;
	}

	if (nReturn != 0)
	{
		printf("����[%s]ע�᷵��[%d]���˳�\n", strCommand, nReturn);
		return 0;
	}

*/
int main(int argc, char **argv)
{
	/*��ʼ�����̻���*/
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
			//printf("����ָ����󷢳�: %s,һ������ %d ��ָ��\n",getTempMessageBuffer(),ret);
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


