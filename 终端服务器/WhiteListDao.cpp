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
 ��������: checkIfUpdated
 ��������: ���ĳ���Ƶ�İ������Ƿ��Ǹ�����,���İ취�ǲ������ݿ��е�version
	�Ƿ�����ն˴��ݹ����İ汾
 �������:
 �� �� ֵ: 0-û�и���,1-�����ˣ�������С��0��ֵ������
 ȫ�ֱ���:
 ��д����: zhangxilai
 ��д����: 2009��01��23��
 �����־
**********************************************************************/
int WhiteListDao::checkIfUpdated(OLFO *oracle, const char *terminalId,int version)
{

	//1�����ն�ȡ�þƵ�
	char sql[1024]={0};
	char imei[16];
	char uim[16];
	splitTerminalIds(terminalId,imei,uim);
	
	//2.�õ��þƵ�İ������汾
	time_t tm=-1;
	int ret=0;
	if(0>(ret=oracle->ExecSQL("select Update_Time from T_Black_White_Version where Black_White_Type=0",&tm)))
	{
		printf("sql: %s %s \n",sql,oracle->getErrorInfo());
		return -4;
	}
	//printf("ret = %d tm=%d, version=%d imei=%s uim=%s\n",ret,tm,version,imei,uim);
	if(ret==1000){
		//û���ҵ�����δ����
		return 0;
	}
	//�ҵ���Ƚ�
	if(version>(int)tm){
		//�����������ͦ��ֵģ���Ȼ�������ն��ϵİ������汾���������ϻ���
		//TODO: ��¼һ����־
		log(__FILE__,__LINE__,"�����������ͦ��ֵģ���Ȼ�������ն��ϵİ������汾���������ϻ���\n");
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
	//ʹ��һ���α�
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
			//���ư������������������ն�װ����
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
(	Black_White_Type			Varchar2(1),		--�ڰ���������(0������;1������)
	Org_ID						Varchar2(10),		--�����������Ƶ�ID(������ʱ���ֶ���HAPPY)
	Update_Time					Date				--�ڰ���������޸�ʱ��
);
Drop   Table T_Black_White;
Create Table T_Black_White
(	Black_White_Type			Varchar2(1),		--�ڰ���������(0������;1������)
	Org_ID						Varchar2(10),		--�����������Ƶ�ID(������ʱ���ֶ������壬���ֵ)
	Phone_Number				Varchar2(15),		--�ڰ���������
	Black_Type					Number(2),			--����������ID
	Join_User_ID				Varchar2(16),		--�������ԱID
	Join_Time					Date,				--����ʱ��
	Notes						Varchar2(64)		--��ע
);

*/


