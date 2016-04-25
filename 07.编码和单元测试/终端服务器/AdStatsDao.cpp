// AdStatsDao.cpp: implementation of the AdStatsDao class.
//
//////////////////////////////////////////////////////////////////////


#include "AdStatsDao.h"
#include "CommonDao.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AdStatsDao::AdStatsDao()
{

}

AdStatsDao::~AdStatsDao()
{

}
/**********************************************************************
 ��������: addToStats
 ��������: ��ͳ�����ݼ��뵽ͳ����
 �������:
 �� �� ֵ: 0-����,1-��,����С��0��ֵ��ʧ��
 ȫ�ֱ���:
 ��д����: zhangxilai
 ��д����: 2009��01��23��
 �����־: 
 ��� SQL��
**********************************************************************/
/*һ��������ݵ�����
01 01 01
20090119101020 file://zh/path.avi 10
20090119101021 file://zh/path.avi 10
20090119101022 menu://zh/c2 10
drop table 		t_access_log;

Create Table T_Terminal_Info
(	imei_number					varchar2(15),		--�ն˵�15λIMEI��
	uim_number					varchar2(15),		--UIM����15λIMSI
	msisdn						varchar2(11),		--�ֻ�����(13.../15.../18...)
	status						number(8),			--�ն˵�״̬
	last_hire_time				date,				--�ϴο�ͨʱ��
	last_quit_hire_time			date,				--�ϴ�����ʱ��
	hire_request_time			date,				--Ԥ��ͨʱ��
	current_hire_time			date,				--���ο�ͨʱ��
	product_org_id				varchar2(10),		--�ն���������ID
	terminal_type				varchar2(16),		--�ն��ͺ�
	product_date				varchar2(8),		--�ն���������
	maintain_limited_date		varchar2(8),		--���ұ��޽�ֹ����(yyyymmdd)
	join_time					date,				--���ʱ��
	join_user_id				varchar2(16),		--������ԱID
	distribute_time				date,				--���ն˷ַ����Ƶ��ʱ��
	distribute_user_id			varchar2(16),		--���ն˷ַ����Ƶ�Ĳ���ԱID
	hotel_id					varchar2(10),		--�ն˵�ǰ�ַ����ľƵ�ID(�˻ص�ƽ̨���Զ���Ϊ��)
	hotel_receiver				varchar2(32),		--�Ƶ�����������
	content_version				varchar2(30),		--���ݰ汾��
	room_no						varchar2(8),		--�ն˷���ľƵ귿�����
	language					varchar2(2),		--�ն�ѡ�������
	last_hb_time				date				--���һ������ʱ��
) 
*/
#include "AdStatsBody.h"
int AdStatsDao::addToStats(OLFO *oracle, const char *terminalId, const char *body)
{
	
	char imei[16];
	char uim[16];
	splitTerminalIds(terminalId,imei,uim);
	//��������,Ȼ��������ݿ���
	char hotelId[10+1];
	char msisdn[10+1];

	
	//1.�ҵ��Ƶ�ID,MSISDN
	if(0>(oracle->ExecSQL(
		"select hotel_id,msisdn from T_Terminal_Info where imei_number=:s1 and uim_number=:s2",
		imei,uim,hotelId,msisdn)))
	{		
		log(__FILE__,__LINE__,"error info: %s\n",oracle->getErrorInfo());
		return -1;
	}
	//2.���

	AdStatsBody adsbody;
	adsbody.parse(body);
	for(int i=0;i<3;i++){
		if(adsbody.next()==NULL){
			return -2;
		}
		//���Ե�ǰ������ֵ
	}
	char sql[1024]={0};
	while(true){
		char *p1=adsbody.next();
		if(p1==NULL)return 0;//��һ��Ҳû�У��ǿ��Ե�
		char *p2=adsbody.next();
		if(p2==NULL)return -4;//�е�һ��,û�еڶ���,�ǲ��е�
		char *p3=adsbody.next();
		if(p3==NULL)return -5;//�еڶ���,û�е�����,�ǲ��е�
		//ȡ����3������Ϊ�յ�
		sprintf(sql,"insert into t_access_log\
			(id,imei_number,msisdn,content_prefix,language,content_path,click_count,hotel_id,create_time) \
			select \
			seq_ads.nextval,'%s','%s',substr('%s',1,4),substr('%s',8,2),substr('%s',11),'%s','%s',to_date('%s','YYYYMMDDHH24MISS') from dual",
			imei,msisdn,p2,p2,p2,p3,hotelId,p1);
		if(0>(oracle->ExecSQL(sql)))
		{		
			log(__FILE__,__LINE__,"sql=error info: %s\n",oracle->getErrorInfo());
			return -6;
		}
		oracle->Commit();
		

	}

	return 0;
}

/*
	CHECK_STRING("file",CommonDao::getString(ora1,tempbuffer,"select substr('file://zh/path.avi',1,4) from dual"));
	CHECK_STRING("zh",CommonDao::getString(ora1,tempbuffer,"select substr('file://zh/path.avi',8,2) from dual"));
	CHECK_STRING("path.avi",CommonDao::getString(ora1,tempbuffer,"select substr('file://zh/path.avi',11) from dual"));

create table 	t_access_log		--��CMPP���ط������ݻذ���״̬����Ķ�Ӧ��
(
	id				number(9),		--һ��Ωһ�����
	imei_number		varchar2(15),	--�ն˵�15λIMEI��
	msisdn			varchar2(11),	--�ֻ�����
	content_prefix	char(4),		--�����������ڿ���Ϊmenu,file
	language		char(2),		--��λ�����Դ���,������ο�Э��
	content_path	varchar(256),	--����·��
	click_count		number(8),		--�������
	hotel_id		varchar2(10),	--�Ƶ�Id
	create_time		date			--����ʱ��
);

*/
