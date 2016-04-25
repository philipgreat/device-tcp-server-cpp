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
 函数名称: getString
 函数功能: 从数据库中取出一个int型数据
 输出参数:
 返 回 值: 0-不是,1-是，其他小于0的值：失败
 全局变量:
 编写作者: zhangxilai
 编写日期: 2009年01月23日
 变更日志: 
 相关 SQL：
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
 函数名称: getString
 函数功能: 从数据库中通过查询得到一个字符串，这个函数存在一些安全性问题，
			主要源于OLFO类的接口设计问题
 输出参数:
 返 回 值: NULL-失败，有值成功，该函数仅用于测试，不应该作为正是的工作代码
 全局变量:
 编写作者: zhangxilai
 编写日期: 2009年01月23日
 变更日志: 
 相关 SQL：
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
 函数名称: getDatabaseTime
 函数功能: 取得数据库时间
 输出参数:
 返 回 值: 0-不是,1-是，其他小于0的值：失败
 全局变量:
 编写作者: zhangxilai
 编写日期: 2009年01月23日
 变更日志: 
 相关 SQL：
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
