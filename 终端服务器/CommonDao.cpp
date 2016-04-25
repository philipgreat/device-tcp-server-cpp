// CommonDao.cpp: implementation of the CommonDao class.
//
//////////////////////////////////////////////////////////////////////

#include "CommonDao.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdarg.h>
void splitTerminalIds(const char *terminalId,char *imei,char *uim)
{
	strncpy(imei,terminalId,15);
	*(imei+15)=0;

	strncpy(uim,terminalId+15,15);
	*(uim+15)=0;
}

int checkOracleResultsetError(int ret,OLFO  *oracle)
{
	if(ret>=0){
		return ret;
	}
	oracle->Close();
	oracle->DisConnect();
	exit(-100);
	return ret;
}


CommonDao::CommonDao()
{

}

CommonDao::~CommonDao()
{

}
/**********************************************************************
 ��������: getString
 ��������: �����ݿ���ȡ��һ��int������
 �������:
 �� �� ֵ: 0-����,1-�ǣ�����С��0��ֵ��ʧ��
 ȫ�ֱ���:
 ��д����: zhangxilai
 ��д����: 2009��01��23��
 �����־: 
 ��� SQL��
**********************************************************************/
int CommonDao::getInt(OLFO *oracle, const char *sql)
{
	int ret=0;
	int count=-99999999;
	ret = oracle->ExecSQL((char*)sql,&count);
	if(ret<0){
		printf("count:%d, sql=%s err=%s\n",count,sql,oracle->getErrorInfo());
		return ret;
	}
	
	return count;
}
/**********************************************************************
 ��������: getString
 ��������: �����ݿ���ͨ����ѯ�õ�һ���ַ����������������һЩ��ȫ�����⣬
			��ҪԴ��OLFO��Ľӿ��������
 �������:
 �� �� ֵ: NULL-ʧ�ܣ���ֵ�ɹ����ú��������ڲ��ԣ���Ӧ����Ϊ���ǵĹ�������
 ȫ�ֱ���:
 ��д����: zhangxilai
 ��д����: 2009��01��23��
 �����־: 
 ��� SQL��
**********************************************************************/
char * CommonDao::getString(OLFO *oracle, char* outcontent, const char *sql)
{
	int ret=oracle->ExecSQL((char*)sql,outcontent);
    if(ret<0){
    	return NULL;
    }
    if(ret==1000){
    	return NULL;
    }
	return outcontent;
}

int CommonDao::commit(OLFO *oracle)
{
	oracle->Commit();
	return 0;
}

int CommonDao::rollback(OLFO *oracle)
{
	oracle->RollBack();
	return 0;
}
/**********************************************************************
 ��������: getDatabaseTime
 ��������: ȡ�����ݿ�ʱ��
 �������:
 �� �� ֵ: 0-����,1-�ǣ�����С��0��ֵ��ʧ��
 ȫ�ֱ���:
 ��д����: zhangxilai
 ��д����: 2009��01��23��
 �����־: 
 ��� SQL��
**********************************************************************/
time_t CommonDao::getDatabaseTime(OLFO *oracle)
{
	time_t tm;
	if(0>(oracle->ExecSQL("select sysdate from dual",&tm)))
	{		
		return -1;
	}
	return tm;
}

void log(const char *position,int line,const char *format,...)
{
	printf("%s:%4d: ",position,line);
	va_list args;
    va_start(args, format);
    vprintf(format,args);
    va_end(args);
}
