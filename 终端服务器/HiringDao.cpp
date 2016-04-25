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
 ��������: hire
 ��������: �����ն�
 �������:
 �� �� ֵ: 0-����,1-��,����С��0��ֵ��ʧ��
 ȫ�ֱ���:
 ��д����: zhangxilai
 ��д����: 2009��01��23��
 �����־
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
		//��Ȼû�м�¼������
	}
	//���ﲻ�ύ,��Ϊ��Ҫ�������ⲿͨ��,����ⲿͨ����ȷָʾͨ��ʧ��,�������ύ����
	//����һ���µ�SDR,������SDR��ʱ��,����ȷ��SDR���ĸ�����
	/*	
	Id: Ψһ�����,���˵���������ȡ��
	Imei������֮ͨIMEI��
	room_no: ǰ̨����,������ն����õĻ�,��û�з����
	superior_org_Id�����ݺ���������
	start_time����ǰʱ��,sysdate
	hire_src:0,��ʾ�ն�
	*/
	char timeExpr[20];
	sprintf(sql,
		"select to_char(Current_Hire_Time,'YYYYMM') \
		from T_Terminal_Info where imei_number='%s' and Uim_Number='%s' and status=1030000",imei,uim);
	if(NULL==CommonDao::getString(oracle,timeExpr,sql)){
		//printf("sql: %s %s \n",sql,oracle->getErrorInfo());
		return -3;
	}

	//���ҵ���brokerId
	char brokerId[10+1];
	strcpy(brokerId,"unknown");
	if(0>(ret=oracle->ExecSQL("select Superior_Org_ID \
		from T_Org_Info a,T_Terminal_Info b \
		where a.Org_ID=b.hotel_Id and imei_number=:s1 and Uim_Number=:s2",imei,uim,brokerId)))
	{
		printf("sql: %s %s \n",sql,oracle->getErrorInfo());
		return -4;
	}
	//�����굥��״̬���ʱ��ȵ��ϸ�һ��
	sprintf(sql,
		"insert into t_sdr_%s(id,Imei_Number,Uim_Number,msisdn,room_no,hotel_id,Broker_ID,start_time,hire_src,status,hire_notes) \
		select SEQ_SDR.NEXTVAL,imei_number,Uim_Number,msisdn,Room_No,hotel_id,'%s',Current_Hire_Time,0,0,'�ն˷�������' \
		from T_Terminal_Info  where imei_number='%s' and Uim_Number='%s'",
		timeExpr,brokerId,imei,uim);
	
	if(0>(ret=oracle->ExecSQL(sql)))
	{
		oracle->RollBack();
		//printf("sql: %s %s \n",sql,oracle->getErrorInfo());
		return -5;
	}
	//����Ҳ���ύ,��Ϊ��Ҫ�������ⲿͨ��,����ⲿͨ����ȷָʾͨ��ʧ��,�������ύ����

	return 0;
}

/**********************************************************************
 ��������: fire
 ��������: �����ն�
 �������:
 �� �� ֵ: 0-����,1-��,����С��0��ֵ��ʧ��
 ȫ�ֱ���:
 ��д����: zhangxilai
 ��д����: 2009��01��23��
 �����־
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
		//��Ȼû�м�¼������
	}


	//���ﲻ�ύ,��Ϊ��Ҫ�������ⲿͨ��,����ⲿͨ����ȷָʾͨ��ʧ��,�������ύ����
	//�ҵ�SDR�±�,����������ֶ�,
	char timeExpr[20];
	sprintf(sql,
		"select to_char(Current_Hire_Time,'YYYYMM') \
		from T_Terminal_Info where imei_number='%s' and Uim_Number='%s' and status=1010000",imei,uim);
	if(NULL==CommonDao::getString(oracle,timeExpr,sql)){
		printf("sql: %s %s \n",sql,oracle->getErrorInfo());
		return -3;
	}
	//��������������status=0��IMEI��UIM�ҵ��굥��������֮
	//����SDR�������ֶ�
	//quit_time������ʱ��,ȡsysdate
	//quit_src��0�����ն�
	//status��1���� �������
	//�ҵ�SDR��������������ϡ�
	//����Ҳ���ύ,��Ϊ��Ҫ�������ⲿͨ��,����ⲿͨ����ȷָʾͨ��ʧ��,�������ύ����
	sprintf(sql,"update t_sdr_%s set end_time=sysdate,quit_src=0,status='1',quit_notes='�ն�����' where imei_number='%s' and Uim_Number='%s' and status=0",timeExpr,imei,uim);
	//update t_sdr_200902 set end_time=sysdate,quit_src=0,quit_notes='�ն�����'     from t_sdr_200902,T_Terminal_Info where t_sdr_200902.imei_number=T_Terminal_Info.imei_number and t_sdr_200902.Uim_Number=T_Terminal_Info.Uim_Number and t_sdr_200902.Current_Hire_Time=T_Terminal_Info.start_time;
	//update t_sdr_200902 set end_time=sysdate,quit_src=0,quit_notes='�ն�����'  where EXISTS (select 1 from t_sdr_200902,T_Terminal_Info where t_sdr_200902.imei_number=T_Terminal_Info.imei_number and t_sdr_200902.Uim_Number=T_Terminal_Info.Uim_Number and t_sdr_200902.Current_Hire_Time=T_Terminal_Info.start_time)
	if(0>(ret=oracle->ExecSQL(sql)))
	{
		printf("sql: %s %s \n",sql,oracle->getErrorInfo());
		return -4;
	}
	//�Ѿ���������״̬����ǰ������ʱ����û���κ�����ģ�Ӧ�����
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
 ��������: checkIfTendToHire
 ��������: ���һ���ն��Ƿ��Ǵ�������ͨ��״̬
 �������:
 �� �� ֵ: 0-����,1-��,����С��0��ֵ��ʧ��
 ȫ�ֱ���:
 ��д����: zhangxilai
 ��д����: 2009��01��23��
 �����־
**********************************************************************/
int HiringDao::checkIfTendToHire(OLFO *oracle, const char *terminalId)
{
	int status=-1;//һ�������ܵ�statusֵ
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
 ��������: confirmHiring
 ��������: ���յ���������ͨ״̬��ʱ��,ȷ�ϸ��ն��Ѿ�������,��ȷ�ϵ�ʱ��
	ֻ����Ҫ���ã�������д��״̬���ʹ��ն˷����������в�ͬ��������дhire_nots
 �������:
 �� �� ֵ: 0-����,1-��,����С��0��ֵ��ʧ��
 ȫ�ֱ���:
 ��д����: zhangxilai
 ��д����: 2009��01��23��
 �����־
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
		//��Ȼû�м�¼������
	}
	//�ҵ��±������˵�
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
	//�����굥��״̬���ʱ��ȵ��ϸ�һ��
	sprintf(sql,
		"insert into t_sdr_%s(id,Imei_Number,Uim_Number,room_no,hotel_id,Broker_ID,start_time,hire_src,status,hire_notes) \
		select SEQ_SDR.NEXTVAL,imei_number,Uim_Number,Room_No,hotel_id,'%s',Current_Hire_Time,1,0,'ǰ̨��������' \
		from T_Terminal_Info  where imei_number='%s' and Uim_Number='%s'",
		timeExpr,brokerId,imei,uim);
	
	if(0>(ret=oracle->ExecSQL(sql)))
	{
		//printf("sql: %s %s \n",sql,oracle->getErrorInfo());
		return -5;
	}

	//���ﲻ�ύ,��Ϊ��Ҫ�������ⲿͨ��,����ⲿͨ����ȷָʾͨ��ʧ��,�������ύ����
	//ȷ��������Ҫ�����˵�
	return 0;
}
/**********************************************************************
 ��������: checkIfCanBeHired
 ��������: ���һ���ն��Ƿ��ǿ��Ա�����
 �������:
 �� �� ֵ: 0-����,1-��,����С��0��ֵ��ʧ��2,���ն�δ�Ǽ�
 ȫ�ֱ���:
 ��д����: zhangxilai
 ��д����: 2009��01��23��
 �����־: 
 ��� SQL��Select status from T_Terminal_Info where Uim_Number=<�ն˵�IMEI+�ն˵�UIM��>
**********************************************************************/
int HiringDao::checkIfCanBeHired(OLFO *oracle, const char* terminalId)
{
	int status=-1;//һ�������ܵ�statusֵ
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
 ��������: checkIfCanBeFired
 ��������: ���һ���ն��Ƿ��ǿ�������
 �������:
 �� �� ֵ: 0-����,1-��,����С��0��ֵ��ʧ��
 ȫ�ֱ���:
 ��д����: zhangxilai
 ��д����: 2009��01��23��
 �����־: 
 ��� SQL��Select status from T_Terminal_Info where Uim_Number=<�ն˵�IMEI+�ն˵�UIM��>
**********************************************************************/
int HiringDao::checkIfCanBeFired(OLFO *oracle, const char* terminalId)
{
	int status=-1;//һ�������ܵ�statusֵ
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
���һλ�ÿ�һ������ͨ������ʵ����ȴ��û�п�ͨ������ʱ��Ӧ�ý��мƷѲ�����
��������ն˻�Ӧ�ÿ��Լ������ã����ʱ�������ǰִ̨��һ��ȡ����ͨ����Ĳ�����
��Ȼ���ն��޷��ٴο�ͨ������һ����ʱʱ�䣬�����ʱʱ��ͨ��������һ��Сʱ����
��Ԥ��ͨ���ն�һ��Сʱ��û�п�ͨ�Ļ�����ϵͳǿ�Ƹ��ն����⣬���Ҽ�¼һ��ϵͳ
ǿ���������־������ϵͳ���������⶯������������á�
����hire_request_time��ȷ���Ƿ�ʱ��


drop   table t_hire_restore_log;
create table t_hire_restore_log
(	
	imei_number					varchar2(15),		--�ն˵�15λIMEI��
	uim_number					varchar2(15),		--UIM����15λIMSI
	msisdn						varchar2(11),		--�ֻ�����(13.../15.../18...)
	hotel_id					varchar2(10),		--�ն˵�ǰ�ַ����ľƵ�ID(�˻ص�ƽ̨���Զ���Ϊ��)
	room_no						varchar2(8),		--�ն˷���ľƵ귿�����
	hire_request_time			Date,				--Ԥ��ͨʱ��
	create_time					date				--���������ʱ��
);



*/
int HiringDao::restoreTimeoutHireRequest(OLFO *oracle)
{

	int timeoutsenconds=3600;

	//�ѳ���һ��ʱ�������ͨ״̬�ĸ��£���Щ��¼��������
	//Ȼ�����Щ״̬���ն�д����־
	//����Ϊ����״̬
	//�ύ
	//���������ĸ���Ϊ1070000
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
	//����һ��Ҫ�ύ
	return 0;
}
