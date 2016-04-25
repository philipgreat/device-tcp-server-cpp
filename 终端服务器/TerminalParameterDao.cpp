// TerminalParameterDao.cpp: implementation of the TerminalParameterDao class.
//
//////////////////////////////////////////////////////////////////////

#include "TerminalParameterDao.h"
#include "CommonDao.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TerminalParameterDao::TerminalParameterDao()
{

}

TerminalParameterDao::~TerminalParameterDao()
{

}

/*
对于系统参数的处理，由于协议中没有系统参数的版本号，无法保持终端版本状态，所以系统目前这样处理。
平台可以生成在终端参数表（t_terminal_param）中插入一条记录，把欲修改的参数存放在该表中，如果该表中存在一条匹配的记录，匹配规则如下：
1.	通配符'*'：表示，当前所有的终端参数都需要修改
2.	具体的IMEI号：指定的IMEI（UIM）手机参数，其它手机不用修改。
详细信息请参见终端参数表t_terminal_param定义
则认为有系统参数下发，这个期间的终端的每次请求都引起终端参数更新的操作。
如果为*就是每个终端的参数表都更新。
drop table t_terminal_parm;
Create Table t_terminal_parm			--向CMPP网关发送数据回包和状态报告的对应表
(
	id			number(9),	--一个惟一的序号
	Imei_Number	varchar2(15),	--*，表示匹配所有手机，imei匹配某个固定手机
	available	number(1),	--0、不可用；1、可用
	Parameters	char(256),	--WPPREF=http:// SVRIP=192.168.1.6
	create_time	date,	--创建时间
	modify_time	date	--修改时间
);
*/

/**********************************************************************
 函数名称: getParameterExpr
 函数功能: 从数据库中取得
 输出参数:
 返 回 值: 0-成功，小于0-失败，或者空或者没有记录，反正不需要同步系统参数
 全局变量:
 编写作者: zhangxilai
 编写日期: 2009年01月23日
 变更日志: 
 相关 SQL：
**********************************************************************/

int TerminalParameterDao::getParameterExpr(OLFO *oracle, const char *terminalId, char *out, int length)
{
	if(length<1024){
		return -1;
	}
	char imei[16];
	char uim[16];
	splitTerminalIds(terminalId,imei,uim);
	int ret = 0;
	char sql[1024];
	*out=0;
	if(0>(ret=oracle->ExecSQL("select Parameters \
		from t_terminal_parm \
		where (imei_number=:s1) and available=1",imei,out)))
	{
		printf("sql: %s %s \n",sql,oracle->getErrorInfo());
		return -2;
	}

	if(ret!=1000){
		return 0;
	}
	//ret=1000,没有再找一次,看看能匹配星号不
	if(0>(ret=oracle->ExecSQL("select Parameters \
		from t_terminal_parm \
		where ( imei_number='*') and available=1",out)))
	{
		printf("sql: %s %s \n",sql,oracle->getErrorInfo());
		return -3;
	}
	
	if(ret==1000){
		//实在找不到了,没有找到
		return -1;
	}
	return 0;
}
