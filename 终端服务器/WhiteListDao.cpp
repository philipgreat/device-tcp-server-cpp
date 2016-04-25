// WhiteListDao.cpp: implementation of the WhiteListDao class.
//
//////////////////////////////////////////////////////////////////////

#include "WhiteListDao.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#include "CommonDao.h"

WhiteListDao::WhiteListDao()
{

}

WhiteListDao::~WhiteListDao()
{

}
/**********************************************************************
 函数名称: checkIfUpdated
 函数功能: 检查某个酒店的白名单是否是更新了,检查的办法是测试数据库中的version
	是否大于终端传递过来的版本
 输出参数:
 返 回 值: 0-没有更新,1-更新了，其他的小于0的值，错误
 全局变量:
 编写作者: zhangxilai
 编写日期: 2009年01月23日
 变更日志
**********************************************************************/
int WhiteListDao::checkIfUpdated(OLFO *oracle, const char *terminalId,int version)
{

	//1根据终端取得酒店
	char sql[1024]={0};
	char imei[16];
	char uim[16];
	splitTerminalIds(terminalId,imei,uim);
	
	//2.得到该酒店的白名单版本
	time_t tm=-1;
	int ret=0;
	if(0>(ret=oracle->ExecSQL("select Update_Time from T_Black_White_Version where Black_White_Type=0",&tm)))
	{
		printf("sql: %s %s \n",sql,oracle->getErrorInfo());
		return -4;
	}
	//printf("ret = %d tm=%d, version=%d imei=%s uim=%s\n",ret,tm,version,imei,uim);
	if(ret==1000){
		//没有找到返回未更新
		return 0;
	}
	//找到后比较
	if(version>(int)tm){
		//这种情况还是挺奇怪的，居然发现了终端上的白名单版本比主机的上还新
		//TODO: 记录一个日志
		log(__FILE__,__LINE__,"这种情况还是挺奇怪的，居然发现了终端上的白名单版本比主机的上还新\n");
		return 0;	
	}
	if((int)tm>version){
		return 1;
	}

	return 0;
}

int WhiteListDao::retrieveList(OLFO *oracle, const char *terminalId,char *out,int length)
{
	if(length<1024)
	{
		return -1;
	}
	//使用一个游标
	int ret;
	char sql[1024];
	char imei[16];
	char uim[16];
	splitTerminalIds(terminalId,imei,uim);
	sprintf(sql,"select Phone_Number \
		from T_Black_White \
		where Black_White_Type=0");
	if(0!=(ret=oracle->ParseSQL(sql)))
	{
		return ret;
	}
	if(0!=(ret=oracle->Open()))
	{
		return ret;
	}
	
	int currentPosition=0;
	int counter=0;
	while(true)
	{
		if(counter>=10){
			//控制白名单的数量，多了终端装不下
			break;
		}
		if(checkOracleResultsetError(oracle->Fetch(),oracle)==0){
			break;
		}
		oracle->getString(1,out+currentPosition);
		strcat(out+currentPosition," ");
		currentPosition+=strlen(out+currentPosition);
		
		if(currentPosition>=length){
			break;
		}
		counter++;
	}
	if(strlen(out)>0){
		out[strlen(out)-1]=0;
	}
	oracle->Close();
	return 0;
}
/*

Drop   Table T_Black_White_Version;
Create Table T_Black_White_Version
(	Black_White_Type			Varchar2(1),		--黑白名单类型(0白名单;1黑名单)
	Org_ID						Varchar2(10),		--黑名单所属酒店ID(白名单时此字段填HAPPY)
	Update_Time					Date				--黑白名单最后修改时间
);
Drop   Table T_Black_White;
Create Table T_Black_White
(	Black_White_Type			Varchar2(1),		--黑白名单类型(0白名单;1黑名单)
	Org_ID						Varchar2(10),		--黑名单所属酒店ID(白名单时此字段无意义，填空值)
	Phone_Number				Varchar2(15),		--黑白名单号码
	Black_Type					Number(2),			--黑名单类型ID
	Join_User_ID				Varchar2(16),		--加入操作员ID
	Join_Time					Date,				--加入时间
	Notes						Varchar2(64)		--备注
);

*/


