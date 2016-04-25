// HiringDao.cpp: implementation of the HiringDao class.
//
//////////////////////////////////////////////////////////////////////

#include "HiringDao.h"
#include "CommonDao.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


HiringDao::HiringDao()
{

}

HiringDao::~HiringDao()
{

}
/**********************************************************************
 函数名称: hire
 函数功能: 租用终端
 输出参数:
 返 回 值: 0-不是,1-是,其他小于0的值：失败
 全局变量:
 编写作者: zhangxilai
 编写日期: 2009年01月23日
 变更日志
**********************************************************************/
int HiringDao::hire(OLFO *oracle, const char *terminalId)
{
	int ret=1000;
	char sql[1024]={0};
	char imei[16];
	char uim[16];
	splitTerminalIds(terminalId,imei,uim);
	//sprintf(sql,"Select status from T_Terminal_Info where Uim_Number=%s and imei_number=%s")
	//printf("starting...\n");
	if(0>(ret=oracle->ExecSQL(
		"Update T_Terminal_Info set status='1030000',Current_Hire_Time=sysdate,hire_request_time=null \
		where imei_number=:s1 and Uim_Number=:s2 and status=1010000",
		imei,uim)))
	{		
		return -1;
	}

	if(ret==1000){
		return -2;
		//居然没有记录被更新
	}
	//这里不提交,因为还要依赖于外部通信,如果外部通信明确指示通信失败,则无需提交请求
	//生成一个新的SDR,在生成SDR的时候,必须确定SDR在哪个表中
	/*	
	Id: 唯一的序号,从账单号序列中取出
	Imei：欲开通之IMEI号
	room_no: 前台输入,如果是终端租用的话,将没有房间号
	superior_org_Id：根据号码计算出来
	start_time：当前时间,sysdate
	hire_src:0,表示终端
	*/
	char timeExpr[20];
	sprintf(sql,
		"select to_char(Current_Hire_Time,'YYYYMM') \
		from T_Terminal_Info where imei_number='%s' and Uim_Number='%s' and status=1030000",imei,uim);
	if(NULL==CommonDao::getString(oracle,timeExpr,sql)){
		//printf("sql: %s %s \n",sql,oracle->getErrorInfo());
		return -3;
	}

	//先找到其brokerId
	char brokerId[10+1];
	strcpy(brokerId,"unknown");
	if(0>(ret=oracle->ExecSQL("select Superior_Org_ID \
		from T_Org_Info a,T_Terminal_Info b \
		where a.Org_ID=b.hotel_Id and imei_number=:s1 and Uim_Number=:s2",imei,uim,brokerId)))
	{
		printf("sql: %s %s \n",sql,oracle->getErrorInfo());
		return -4;
	}
	//保持详单和状态表的时间等的严格一致
	sprintf(sql,
		"insert into t_sdr_%s(id,Imei_Number,Uim_Number,msisdn,room_no,hotel_id,Broker_ID,start_time,hire_src,status,hire_notes) \
		select SEQ_SDR.NEXTVAL,imei_number,Uim_Number,msisdn,Room_No,hotel_id,'%s',Current_Hire_Time,0,0,'终端发起请求' \
		from T_Terminal_Info  where imei_number='%s' and Uim_Number='%s'",
		timeExpr,brokerId,imei,uim);
	
	if(0>(ret=oracle->ExecSQL(sql)))
	{
		oracle->RollBack();
		//printf("sql: %s %s \n",sql,oracle->getErrorInfo());
		return -5;
	}
	//这里也不提交,因为还要依赖于外部通信,如果外部通信明确指示通信失败,则无需提交请求

	return 0;
}

/**********************************************************************
 函数名称: fire
 函数功能: 退租终端
 输出参数:
 返 回 值: 0-不是,1-是,其他小于0的值：失败
 全局变量:
 编写作者: zhangxilai
 编写日期: 2009年01月23日
 变更日志
**********************************************************************/
int HiringDao::fire(OLFO *oracle, const char *terminalId)
{
	int ret=1000;
	char sql[1024]={0};
	char imei[16];
	char uim[16];
	splitTerminalIds(terminalId,imei,uim);
	//sprintf(sql,"Select status from T_Terminal_Info where Uim_Number=%s and imei_number=%s")
	if(0>(ret=oracle->ExecSQL(
		"Update T_Terminal_Info set status='1010000',Last_Hire_Time=Current_Hire_Time,\
		Last_Quit_Hire_Time=sysdate\
		where imei_number=:s1 and Uim_Number=:s2  and status=1030000",
		imei,uim)))
	{		
		return -1;
	}

	if(ret==1000){
		return -2;
		//居然没有记录被更新
	}


	//这里不提交,因为还要依赖于外部通信,如果外部通信明确指示通信失败,则无需提交请求
	//找到SDR月表,更新退租的字段,
	char timeExpr[20];
	sprintf(sql,
		"select to_char(Current_Hire_Time,'YYYYMM') \
		from T_Terminal_Info where imei_number='%s' and Uim_Number='%s' and status=1010000",imei,uim);
	if(NULL==CommonDao::getString(oracle,timeExpr,sql)){
		printf("sql: %s %s \n",sql,oracle->getErrorInfo());
		return -3;
	}
	//根据三个条件：status=0，IMEI和UIM找到详单，并更新之
	//更新SDR表以下字段
	//quit_time：退租时间,取sysdate
	//quit_src：0代表终端
	//status：1代表 退租完成
	//找到SDR，更新里面的资料。
	//这里也不提交,因为还要依赖于外部通信,如果外部通信明确指示通信失败,则无需提交请求
	sprintf(sql,"update t_sdr_%s set end_time=sysdate,quit_src=0,status='1',quit_notes='终端退租' where imei_number='%s' and Uim_Number='%s' and status=0",timeExpr,imei,uim);
	//update t_sdr_200902 set end_time=sysdate,quit_src=0,quit_notes='终端退租'     from t_sdr_200902,T_Terminal_Info where t_sdr_200902.imei_number=T_Terminal_Info.imei_number and t_sdr_200902.Uim_Number=T_Terminal_Info.Uim_Number and t_sdr_200902.Current_Hire_Time=T_Terminal_Info.start_time;
	//update t_sdr_200902 set end_time=sysdate,quit_src=0,quit_notes='终端退租'  where EXISTS (select 1 from t_sdr_200902,T_Terminal_Info where t_sdr_200902.imei_number=T_Terminal_Info.imei_number and t_sdr_200902.Uim_Number=T_Terminal_Info.Uim_Number and t_sdr_200902.Current_Hire_Time=T_Terminal_Info.start_time)
	if(0>(ret=oracle->ExecSQL(sql)))
	{
		printf("sql: %s %s \n",sql,oracle->getErrorInfo());
		return -4;
	}
	//已经不是租用状态，当前的租用时间是没有任何意义的，应该清空
	if(0>(ret=oracle->ExecSQL(
		"Update T_Terminal_Info set CURRENT_HIRE_TIME=null,hire_request_time=null \
		where imei_number=:s1 and Uim_Number=:s2  and status=1010000",
		imei,uim)))
	{		
		return -5;
	}
	return 0;

}
/**********************************************************************
 函数名称: checkIfTendToHire
 函数功能: 检查一个终端是否是处于欲开通的状态
 输出参数:
 返 回 值: 0-不是,1-是,其他小于0的值：失败
 全局变量:
 编写作者: zhangxilai
 编写日期: 2009年01月23日
 变更日志
**********************************************************************/
int HiringDao::checkIfTendToHire(OLFO *oracle, const char *terminalId)
{
	int status=-1;//一个不可能的status值
	char imei[16];
	char uim[16];
	splitTerminalIds(terminalId,imei,uim);
	//sprintf(sql,"Select status from T_Terminal_Info where Uim_Number=%s and imei_number=%s")
	if(0>(oracle->ExecSQL("Select status from T_Terminal_Info where imei_number=:s1 and Uim_Number=:s2 ",imei,uim,&status)))
	{		
		return -1;
	}
	
	if(status==1020000){
		return 1;
	}
	return 0;

}
/**********************************************************************
 函数名称: confirmHiring
 函数功能: 在收到设置欲开通状态的时候,确认该终端已经被租用,在确认的时候，
	只是需要租用，并且填写其状态，和从终端发起请求稍有不同。无需填写hire_nots
 输出参数:
 返 回 值: 0-不是,1-是,其他小于0的值：失败
 全局变量:
 编写作者: zhangxilai
 编写日期: 2009年01月23日
 变更日志
**********************************************************************/
int HiringDao::confirmHiring(OLFO *oracle, const char *terminalId)
{
	int ret=1000;
	char sql[1024]={0};
	char imei[16];
	char uim[16];
	splitTerminalIds(terminalId,imei,uim);
	//sprintf(sql,"Select status from T_Terminal_Info where Uim_Number=%s and imei_number=%s")
	if(0>(ret=oracle->ExecSQL(
		"Update T_Terminal_Info set status='1030000',Current_Hire_Time=sysdate \
		where imei_number=:s1 and Uim_Number=:s2 and status=1020000",
		imei,uim)))
	{		
		return -1;
	}

	if(ret==1000){
		return -2;
		//居然没有记录被更新
	}
	//找到月表，生成账单
	char timeExpr[20];
	sprintf(sql,
		"select to_char(Current_Hire_Time,'YYYYMM') \
		from T_Terminal_Info where imei_number='%s' and Uim_Number='%s' and status=1030000",imei,uim);
	if(NULL==CommonDao::getString(oracle,timeExpr,sql)){
		//printf("sql: %s %s \n",sql,oracle->getErrorInfo());
		return -3;
	}
	
	char brokerId[10+1];
	strcpy(brokerId,"unknown");
	if(0>(ret=oracle->ExecSQL("select Superior_Org_ID \
		from T_Org_Info a,T_Terminal_Info b \
		where a.Org_ID=b.hotel_Id and imei_number=:s1 and Uim_Number=:s2",imei,uim,brokerId)))
	{
		//printf("sql: %s %s \n",sql,oracle->getErrorInfo());
		return -4;
	}
	//保持详单和状态表的时间等的严格一致
	sprintf(sql,
		"insert into t_sdr_%s(id,Imei_Number,Uim_Number,room_no,hotel_id,Broker_ID,start_time,hire_src,status,hire_notes) \
		select SEQ_SDR.NEXTVAL,imei_number,Uim_Number,Room_No,hotel_id,'%s',Current_Hire_Time,1,0,'前台发起请求' \
		from T_Terminal_Info  where imei_number='%s' and Uim_Number='%s'",
		timeExpr,brokerId,imei,uim);
	
	if(0>(ret=oracle->ExecSQL(sql)))
	{
		//printf("sql: %s %s \n",sql,oracle->getErrorInfo());
		return -5;
	}

	//这里不提交,因为还要依赖于外部通信,如果外部通信明确指示通信失败,则无需提交请求
	//确认退租需要生成账单
	return 0;
}
/**********************************************************************
 函数名称: checkIfCanBeHired
 函数功能: 检查一个终端是否是可以被租用
 输出参数:
 返 回 值: 0-不是,1-是,其他小于0的值：失败2,该终端未登记
 全局变量:
 编写作者: zhangxilai
 编写日期: 2009年01月23日
 变更日志: 
 相关 SQL：Select status from T_Terminal_Info where Uim_Number=<终端的IMEI+终端的UIM号>
**********************************************************************/
int HiringDao::checkIfCanBeHired(OLFO *oracle, const char* terminalId)
{
	int status=-1;//一个不可能的status值
	char imei[16];
	char uim[16];
	splitTerminalIds(terminalId,imei,uim);
	int ret=0;
	//sprintf(sql,"Select status from T_Terminal_Info where Uim_Number=%s and imei_number=%s")
	if(0>(ret=oracle->ExecSQL("Select status from T_Terminal_Info where imei_number=:s1 and Uim_Number=:s2 ",imei,uim,&status)))
	{		
		return -1;
	}
	if(ret==1000){
		return -2;
	}
	if(status==1030000){
		return -3;
	}
	if(status==1010000){
		return 1;
	}
	return 0;
}
/**********************************************************************
 函数名称: checkIfCanBeFired
 函数功能: 检查一个终端是否是可以退租
 输出参数:
 返 回 值: 0-不是,1-是,其他小于0的值：失败
 全局变量:
 编写作者: zhangxilai
 编写日期: 2009年01月23日
 变更日志: 
 相关 SQL：Select status from T_Terminal_Info where Uim_Number=<终端的IMEI+终端的UIM号>
**********************************************************************/
int HiringDao::checkIfCanBeFired(OLFO *oracle, const char* terminalId)
{
	int status=-1;//一个不可能的status值
	char imei[16];
	char uim[16];
	int ret=0;
	splitTerminalIds(terminalId,imei,uim);
	//sprintf(sql,"Select status from T_Terminal_Info where Uim_Number=%s and imei_number=%s")
	if(0>(ret=oracle->ExecSQL("Select status from T_Terminal_Info where imei_number=:s1 and Uim_Number=:s2 ",imei,uim,&status)))
	{		
		return -1;
	}
	if(ret==1000){
		return -2;
	}
	if(status==1030000){
		return 1;
	}
	if(status==1010000){
		return 0;
	}
	return 0;
}

int HiringDao::updateLastHeartBeatTime(OLFO *oracle,const char * terminalId)
{
	char imei[16];
	char uim[16];
	splitTerminalIds(terminalId,imei,uim);
	//sprintf(sql,"Select status from T_Terminal_Info where Uim_Number=%s and imei_number=%s")
	if(0>(oracle->ExecSQL("update T_Terminal_Info \
		set Last_HB_Time=sydate where imei_number=:s1 and Uim_Number=:s2 ",imei,uim)))
	{		
		return -1;
	}
	oracle->Commit();
	return 0;
}
/*
如果一位旅客一个请求开通，但是实际上却又没有开通，结账时不应该进行计费操作，
但是这个终端还应该可以继续租用，这个时候必须让前台执行一个取消开通请求的操作，
不然此终端无法再次开通。设置一个超时时间，这个超时时间通常不超过一个小时，处
于预开通的终端一个小时内没有开通的话，则系统强制该终端退租，并且记录一个系统
强制退租的日志。来自系统的这样退租动作不会产生费用。
利用hire_request_time来确定是否超时。


drop   table t_hire_restore_log;
create table t_hire_restore_log
(	
	imei_number					varchar2(15),		--终端的15位IMEI号
	uim_number					varchar2(15),		--UIM卡的15位IMSI
	msisdn						varchar2(11),		--手机号码(13.../15.../18...)
	hotel_id					varchar2(10),		--终端当前分发到的酒店ID(退回到平台则本自动置为空)
	room_no						varchar2(8),		--终端分配的酒店房间号码
	hire_request_time			Date,				--预开通时间
	create_time					date				--发生退租的时间
);



*/
int HiringDao::restoreTimeoutHireRequest(OLFO *oracle)
{

	int timeoutsenconds=3600;

	//把超出一定时间的欲开通状态的更新，这些记录将被锁定
	//然后把这些状态的终端写入日志
	//更新为退租状态
	//提交
	//符合条件的更新为1070000
	if(0>(oracle->ExecSQL("update T_Terminal_Info set status=1070000 \
		where status=1020000 and (sysdate-Hire_Request_Time)*86400>:n1",timeoutsenconds)))
	{		
		
		return -1;
	}

	if(0>(oracle->ExecSQL("insert into t_hire_restore_log\
		(id,imei_number,uim_number,msisdn,hotel_id,room_no,hire_request_time,create_time)\
		select SEQ_hire_restore_log.nextval,imei_number,uim_number,msisdn,hotel_id,room_no,hire_request_time,sysdate \
		from T_Terminal_Info where status=1070000")))
	{		
		printf("error: %s \n",oracle->getErrorInfo());
		return -2;
	}
	
	if(0>(oracle->ExecSQL("update T_Terminal_Info set status=1010000 \
		where status=1070000",timeoutsenconds)))
	{		
		return -3;
	}
	oracle->Commit();
	//这里一定要提交
	return 0;
}
