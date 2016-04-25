// CommonDao.h: interface for the CommonDao class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMONDAO_H__A9077D5B_3DAA_404D_AF19_8C0328DE20FF__INCLUDED_)
#define AFX_COMMONDAO_H__A9077D5B_3DAA_404D_AF19_8C0328DE20FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ORACLE_OPER.H"
/**********************************************************************
 函数名称: CommonDao
 函数功能: 处理一般的任务，比如心跳，通过终端号寻找酒店的ID，记录心跳，
		   获得Oracle的当前时间
 输出参数:
 返 回 值: 0-成功,其他-失败
 全局变量:
 编写作者: zhangxilai
 编写日期: 2008年10月13日
 变更日志
**********************************************************************/
class CommonDao  
{
public:

	static time_t getDatabaseTime(OLFO *oracle);
	static int rollback(OLFO *oracle);
	static int commit(OLFO * oracle);
	static char * getString(OLFO *oracle,char* outcontent,const char *sql);
	static int getInt(OLFO *oracle,const char *sql);
	CommonDao();
	virtual ~CommonDao();
};

void splitTerminalIds(const char *terminalId,char *imei,char *uim);
int  checkOracleResultsetError(int ret,OLFO  *oracle);
void log(const char *function,int line,const char *format,...);
#endif // !defined(AFX_COMMONDAO_H__A9077D5B_3DAA_404D_AF19_8C0328DE20FF__INCLUDED_)
