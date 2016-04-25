// OraTest.cpp : Defines the entry point for the console application.
//

#include "ORACLE_OPER.H"

#ifdef _WIN32
#define CHECK_NUMBER(a,b) if(true){int expect=(a);int got=(b); if(expect!=got){printf("[X]...%s(%3d), expect<%d> get<%d>: %s != %s\n","oratest.cpp",__LINE__,expect,got,#a,#b);}}
#define CHECK_STRING(a,b) if(true){char *expect=(a);char *got=(b);if(expect==NULL&&got==NULL){}else if(expect==NULL){printf("[X]...%s(%3d), expect<NULL> get<%s>: %s != %s\n","oratest.cpp",__LINE__,(got),#a,#b);}else if(got==NULL){printf("[X]...%s(%3d), expect<%s> get<NULL>: %s != %s\n","oratest.cpp",__LINE__,(expect),#a,#b);}else if(strcmp(expect,got)!=0){printf("[X]...%s(%3d), expect<%s> get<%s>: %s != %s\n","oratest.cpp",__LINE__,(expect),(got),#a,#b);}}
#define TEST_LABLE(a)  printf("%s:%4d:...<%s>...","oratest.cpp",__LINE__,a);printf("\n");
#else //for gcc
#define CHECK_NUMBER(a,b) if(true){int expect=(a);int got=(b); if(expect!=got){printf("[X]...%s(%3d), expect<%d> get<%d>: %s != %s\n",__FUNCTION__,__LINE__,expect,got,#a,#b);}}
#define CHECK_STRING(a,b) if(true){char *expect=(a);char *got=(b);if(expect==NULL&&got==NULL){}else if(expect==NULL){printf("[X]...%s(%3d), expect<NULL> get<%s>: %s != %s\n",__FUNCTION__,__LINE__,(got),#a,#b);}else if(got==NULL){printf("[X]...%s(%3d), expect<%s> get<NULL>: %s != %s\n",__FUNCTION__,__LINE__,(expect),#a,#b);}else if(strcmp(expect,got)!=0){printf("[X]...%s(%3d), expect<%s> get<%s>: %s != %s\n",__FUNCTION__,__LINE__,(expect),(got),#a,#b);}}
#define TEST_LABLE(a)  printf("%s:%4d:...<%s>...%s",__FILE__,__LINE__,a,__FUNCTION__);printf("\n");
#endif


int testmain(OLFO  *oracle,OLFO  *oracle2,OLFO  *oracle3);
int executeUpdate(OLFO  *oracle,const char**arg);

#include "BlackListDao.h"
#include "WhiteListDao.h"
#include "HiringDao.h"
#include "CommonDao.h"
#include "TerminalParameterDao.h"
#include "AdStatsBody.h"
#include "AdStatsDao.h"
#include "TerminalService.h"
#include "SimpleServer.h"
#include "WtdpPacket.h"

/**********************************************************************
 ��������: executeUpdate
 ��������: ����ִ��һ�����������,������ʱ���ύ,
 		   �����и��ر����,����������ʶ����ַ���
 		   �ж��Ƿ��ַ������Ƿ��Ѿ�����,ʹ�õ�ʱ�򲻱���������NULL�ַ�
 �������:
 �� �� ֵ: 0-�ɹ�,����-ʧ��
 ȫ�ֱ���:
 ��д����: zhangxilai
 ��д����: 2008��10��13��
 �����־
**********************************************************************/
int executeUpdate(OLFO  *oracle,const char**arg)
{
	const char **p=arg;
	while(*p!=NULL){
		
		if(0>oracle->ExecSQL((char*)*p)){
			log(__FILE__,__LINE__,"sqlִ�г���,%s: %s\n",oracle->getErrorInfo(),*p);
			oracle->RollBack();
			return -1;
		}
		p++;
	}
	oracle->Commit();
	return 0;
}

int main(int argc, char* argv[])
{

	if(argc<4){
		printf("������ʽӦ���� test <oracleusername> <oraclepassword> <tnsname> \n");
		return 0;
	}
	OLFO oracle[4]={OLFO(OCI_THREADED),OLFO(OCI_THREADED),OLFO(OCI_THREADED),OLFO(OCI_THREADED)};
	int i=0;
	for(i=0;i<4;i++){
		
		if(0!=oracle[i].Connect(argv[1],argv[2],argv[3])){
			printf("Oracle����ʧ��,�����ַ�����%s/%s@%s\n",argv[1],argv[2],argv[3]);
			return 0;
		}
		if(0>oracle[i].ExecSQL("alter session set NLS_DATE_FORMAT='yyyy-mm-dd hh24:mi:ss'")){
			return 0;
		}
	}
	testmain(oracle,oracle+1,oracle+2);
	for(i=0;i<4;i++){
		oracle[i].DisConnect();
	}
	//CHECK_NUMBER(1,0);

	return 0;
}
//alter session set NLS_DATE_FORMAT='mm-dd hh24:mi:ss'
int testHireDaoCheckIfCanBeHired(OLFO  *ora1,OLFO  *ora2,OLFO  *ora3)
{

	TEST_LABLE("����-����Ƿ���Ա�����-����ɹ�");
	const char *args[]={
"delete from T_Terminal_Info",
	NULL};
	executeUpdate(ora3,args);
	//�޴��նˣ������Ա�����
	CHECK_NUMBER(-2,HiringDao::checkIfCanBeHired(ora1,"012345678901234ABCDEFGHIJKLMNO"));
	CHECK_NUMBER(-2,HiringDao::checkIfCanBeFired(ora1,"012345678901234ABCDEFGHIJKLMNO"));

	//
	const char *args2[]={
"delete from T_Terminal_Info",
"insert into T_Terminal_Info(Imei_Number,Uim_Number,status)values('012345678901234','ABCDEFGHIJKLMNO',0)",
	NULL};
	executeUpdate(ora3,args2);
	//״̬���ԣ������Ա�����
	CHECK_NUMBER(0,HiringDao::checkIfCanBeHired(ora1,"012345678901234ABCDEFGHIJKLMNO"));
	CHECK_NUMBER(0,HiringDao::checkIfCanBeFired(ora1,"012345678901234ABCDEFGHIJKLMNO"));

	const char *args3[]={
"delete from T_Terminal_Info",
"insert into T_Terminal_Info(Imei_Number,Uim_Number,status)values('012345678901234','ABCDEFGHIJKLMNO',1010000)",
	NULL};
	executeUpdate(ora3,args3);
	CHECK_NUMBER(1,HiringDao::checkIfCanBeHired(ora1,"012345678901234ABCDEFGHIJKLMNO"));
	const char *args4[]={
"delete from T_Terminal_Info",
"insert into T_Terminal_Info(Imei_Number,Uim_Number,status)values('012345678901234','ABCDEFGHIJKLMNO',1030000)",
	NULL};
	executeUpdate(ora3,args4);
	CHECK_NUMBER(1,HiringDao::checkIfCanBeFired(ora1,"012345678901234ABCDEFGHIJKLMNO"));
	return 0;
}

int testTerminalParameterDaoGetParameterExpr(OLFO  *ora1,OLFO  *ora2,OLFO  *ora3)
{

	TEST_LABLE("����-��ȡϵͳ����-û���κ�ϵͳ����");
	char tempbuffer[1024];
	const char *args[]={
"delete from t_terminal_parm",
	NULL};
	executeUpdate(ora3,args);
	//�޴��նˣ������Ա�����
	CHECK_NUMBER(-1,TerminalParameterDao::getParameterExpr(ora1,"012345678901234ABCDEFGHIJKLMNO",tempbuffer,1024));
	CHECK_STRING("",tempbuffer);
	TEST_LABLE("����-��ȡϵͳ����-��ϵͳ����-*������");

	const char *args1[]={
"delete from t_terminal_parm",
"insert into t_terminal_parm(id,Imei_Number,available,Parameters,create_time,modify_time)values(1,'*',1,'WAP=http:// x=y',sysdate,sysdate)",
	NULL};
	executeUpdate(ora3,args1);
	CHECK_NUMBER(0,TerminalParameterDao::getParameterExpr(ora1,"012345678901234ABCDEFGHIJKLMNO",tempbuffer,1024));
	CHECK_STRING("WAP=http:// x=y",tempbuffer);
	TEST_LABLE("����-��ȡϵͳ����-��ϵͳ����-ƥ��IMEI");

	const char *args2[]={
"delete from t_terminal_parm",
"insert into t_terminal_parm(id,Imei_Number,available,Parameters,create_time,modify_time)values(1,'012345678901234',1,'WAP=http://',sysdate,sysdate)",
	NULL};
	executeUpdate(ora3,args2);
	CHECK_NUMBER(0,TerminalParameterDao::getParameterExpr(ora1,"012345678901234ABCDEFGHIJKLMNO",tempbuffer,1024));
	CHECK_STRING("WAP=http://",tempbuffer);	
	//
	return 0;
}

int testAdStatsBodyParse(OLFO  *ora1,OLFO  *ora2,OLFO  *ora3)
{
	
	TEST_LABLE("����-��������-��ȷ����");
	AdStatsBody adsbody;
	
	adsbody.parse("10 11 12\r\n1 2 3");

	CHECK_STRING("10",adsbody.next());
	CHECK_STRING("11",adsbody.next());
	CHECK_STRING("12",adsbody.next());
	CHECK_STRING("1",adsbody.next());
	CHECK_STRING("2",adsbody.next());
	CHECK_STRING("3",adsbody.next());
	CHECK_STRING(NULL,adsbody.next());
	

	//
	return 0;
}
int testAdStatsDaoAddToStats(OLFO  *ora1,OLFO  *ora2,OLFO  *ora3)
{
	
	TEST_LABLE("����-��������-��ȷ����");
	const char *args[]={
"delete from T_Terminal_Info","delete from t_access_log",
"delete from T_Org_Info","insert into T_Org_Info(Org_ID,Superior_Org_ID)values('hotel','broker')",
"insert into T_Terminal_Info(Imei_Number,Uim_Number,msisdn,status,room_no,hotel_id)values('012345678901234','ABCDEFGHIJKLMNO','13902392590',1010000,'room','hotel')",
	NULL};
	executeUpdate(ora3,args);
	CHECK_NUMBER(0,AdStatsDao::addToStats(ora1,"012345678901234ABCDEFGHIJKLMNO","01 01 01\r\n\
20090119101020 file://zh/path.avi 10\r\n\
20090119101021 file://zh/path.avi 11"));
	CHECK_NUMBER(0,AdStatsDao::addToStats(ora1,"012345678901234ABCDEFGHIJKLMNO","01 01 01\r\n\
20090119101020 file://zh/path.avi 10\r\n\
20090119101021 file://zh/path.avi 11\r\n"));

	//
	return 0;
}

int testHireDaoHire(OLFO  *ora1,OLFO  *ora2,OLFO  *ora3)
{
	char tempbuffer[1024];
	TEST_LABLE("����-���ò���-����ɹ�");
	const char *args[]={
"delete from T_Terminal_Info","delete from T_SDR_200902",
"delete from T_Org_Info","insert into T_Org_Info(Org_ID,Superior_Org_ID)values('hotel','broker')",
"insert into T_Terminal_Info(Imei_Number,Uim_Number,msisdn,status,room_no,hotel_id)values('012345678901234','ABCDEFGHIJKLMNO','13902392590',1010000,'room','hotel')",
	NULL};
	executeUpdate(ora3,args);
	//��ǰ��������
	CHECK_NUMBER(0,HiringDao::hire(ora1,"012345678901234ABCDEFGHIJKLMNO"));
	CHECK_NUMBER(1,CommonDao::getInt(ora1,"select count(*) from T_Terminal_Info"));
	CHECK_STRING("200902",CommonDao::getString(ora1,tempbuffer,"select to_char(Current_Hire_Time,'YYYYMM') from T_Terminal_Info"));
	CHECK_NUMBER(1,CommonDao::getInt(ora1,"select count(*) from T_SDR_200902"));

	CHECK_NUMBER(1030000,CommonDao::getInt(ora1,"select status from T_Terminal_Info where Imei_Number='012345678901234' and uim_number='ABCDEFGHIJKLMNO'"));
	CHECK_NUMBER(0,CommonDao::commit(ora1));
	//CHECK_NUMBER(1010000,CommonDao::getInt(ora1,"select status from T_Terminal_Info where Imei_Number='012345678901234' and uim_number='012345678901234'"));
	
	TEST_LABLE("����-���ò���-���ն˿��Ա�����");
	//
	const char *args2[]={
"delete from T_Terminal_Info",
	NULL};
	
	executeUpdate(ora3,args2);
	//״̬���ԣ������Ա�����
	CHECK_NUMBER(-3,HiringDao::hire(ora1,"012345678901234ABCDEFGHIJKLMNO"));

	
	return 0;
}



int testHiringDaoRestoreTimeoutHireRequest(OLFO  *ora1,OLFO  *ora2,OLFO  *ora3)
{
	//char tempbuffer[1024];
	TEST_LABLE("����-���ò���-����ɹ�");
	const char *args[]={
"delete from T_Terminal_Info","delete from t_hire_restore_log",
"delete from T_Org_Info","insert into T_Org_Info(Org_ID,Superior_Org_ID)values('hotel','broker')",
"insert into T_Terminal_Info(hire_request_time,Imei_Number,Uim_Number,msisdn,status,room_no,hotel_id)values(sysdate-1,'012345678901234','ABCDEFGHIJKLMNO','13902392590',1020000,'room','hotel')",
	NULL};
	executeUpdate(ora3,args);
	//��ǰ��������
	CHECK_NUMBER(0,HiringDao::restoreTimeoutHireRequest(ora1));
	CHECK_NUMBER(1,CommonDao::getInt(ora1,"select count(*) from T_Terminal_Info"));
	CHECK_NUMBER(1,CommonDao::getInt(ora1,"select count(*) from t_hire_restore_log"));
	

	return 0;
}

int testHireDaoFire(OLFO  *ora1,OLFO  *ora2,OLFO  *ora3)
{
	char tempbuffer[1024];
	TEST_LABLE("����-���ú��������-��ʼ����");
	const char *args[]={
"delete from T_Terminal_Info","delete from T_SDR_200902",
"delete from T_Org_Info","insert into T_Org_Info(Org_ID,Superior_Org_ID)values('hotel','broker')",
"insert into T_Terminal_Info(Imei_Number,Uim_Number,msisdn,status,room_no,hotel_id)values('012345678901234','ABCDEFGHIJKLMNO','13902392590',1010000,'room','hotel')",
	NULL};
	executeUpdate(ora3,args);
	//��ǰ��������
	CHECK_NUMBER(0,HiringDao::hire(ora1,"012345678901234ABCDEFGHIJKLMNO"));
	CHECK_NUMBER(1,CommonDao::getInt(ora1,"select count(*) from T_Terminal_Info"));
	CHECK_STRING("200902",CommonDao::getString(ora1,tempbuffer,"select to_char(Current_Hire_Time,'YYYYMM') from T_Terminal_Info"));
	CHECK_NUMBER(0,CommonDao::getInt(ora1,"select status from T_SDR_200902"));

	CHECK_NUMBER(1030000,CommonDao::getInt(ora1,"select status from T_Terminal_Info where Imei_Number='012345678901234' and uim_number='ABCDEFGHIJKLMNO'"));
	CHECK_NUMBER(0,CommonDao::commit(ora1));
	//CHECK_NUMBER(1010000,CommonDao::getInt(ora1,"select status from T_Terminal_Info where Imei_Number='012345678901234' and uim_number='012345678901234'"));
	TEST_LABLE("����-���ú��������-���⴦��");
	CHECK_NUMBER(0,HiringDao::fire(ora1,"012345678901234ABCDEFGHIJKLMNO"));
	CHECK_NUMBER(1,CommonDao::getInt(ora1,"select count(*) from T_Terminal_Info"));
	//�����Ӧ�����Current_Hire_Time��
	CHECK_NUMBER(1,CommonDao::getInt(ora1,"select count(1) from T_Terminal_Info where Current_Hire_Time is null"));
	CHECK_NUMBER(1,CommonDao::getInt(ora1,"select status from T_SDR_200902"));

	CHECK_NUMBER(1010000,CommonDao::getInt(ora1,"select status from T_Terminal_Info where Imei_Number='012345678901234' and uim_number='ABCDEFGHIJKLMNO'"));
	CHECK_NUMBER(0,CommonDao::commit(ora1));
	//�������ٴ�����
	TEST_LABLE("����-���ú��������-�������ٴ�����");
	CHECK_NUMBER(0,HiringDao::hire(ora1,"012345678901234ABCDEFGHIJKLMNO"));
	CHECK_NUMBER(1,CommonDao::getInt(ora1,"select count(*) from T_Terminal_Info"));
	CHECK_STRING("200902",CommonDao::getString(ora1,tempbuffer,"select to_char(Current_Hire_Time,'YYYYMM') from T_Terminal_Info"));
	//Ӧ����2���˵�������
	CHECK_NUMBER(2,CommonDao::getInt(ora1,"select count(*) from T_SDR_200902"));

	CHECK_NUMBER(1030000,CommonDao::getInt(ora1,"select status from T_Terminal_Info where Imei_Number='012345678901234' and uim_number='ABCDEFGHIJKLMNO'"));
	CHECK_NUMBER(0,CommonDao::commit(ora1));

	TEST_LABLE("����-���ú��������-�������ٴ�����");
	CHECK_NUMBER(0,HiringDao::fire(ora1,"012345678901234ABCDEFGHIJKLMNO"));
	CHECK_NUMBER(1,CommonDao::getInt(ora1,"select count(*) from T_Terminal_Info"));
	//�����Ӧ�����Current_Hire_Time��
	CHECK_NUMBER(1,CommonDao::getInt(ora1,"select count(1) from T_Terminal_Info where Current_Hire_Time is null"));
	//���������˵�
	CHECK_NUMBER(2,CommonDao::getInt(ora1,"select count(*) from T_SDR_200902"));

	CHECK_NUMBER(1010000,CommonDao::getInt(ora1,"select status from T_Terminal_Info where Imei_Number='012345678901234' and uim_number='ABCDEFGHIJKLMNO'"));
	CHECK_NUMBER(0,CommonDao::commit(ora1));
	
	for(int i=0;i<10;i++){
		
		CHECK_NUMBER(0,HiringDao::hire(ora1,"012345678901234ABCDEFGHIJKLMNO"));
		CHECK_NUMBER(0,CommonDao::commit(ora1));
		CHECK_NUMBER(0,HiringDao::fire(ora1,"012345678901234ABCDEFGHIJKLMNO"));
		CHECK_NUMBER(0,CommonDao::commit(ora1));

	}
	CHECK_NUMBER(12,CommonDao::getInt(ora1,"select count(*) from T_SDR_200902"));
	return 0;
}


int testHireDaoConfirmHiring(OLFO  *ora1,OLFO  *ora2,OLFO  *ora3)
{
	char tempbuffer[1024];
	TEST_LABLE("����-ƽ̨���ú��������-��ʼ����");
	const char *args[]={
"delete from T_Terminal_Info","delete from T_SDR_200902",
"delete from T_Org_Info","insert into T_Org_Info(Org_ID,Superior_Org_ID)values('hotel','broker')",
"insert into T_Terminal_Info(Imei_Number,Uim_Number,msisdn,status,room_no,hotel_id)values('012345678901234','ABCDEFGHIJKLMNO','13902392590',1020000,'room','hotel')",
	NULL};
	executeUpdate(ora3,args);
	//��ǰ��������
	CHECK_NUMBER(0,HiringDao::confirmHiring(ora1,"012345678901234ABCDEFGHIJKLMNO"));
	CHECK_NUMBER(1,CommonDao::getInt(ora1,"select count(*) from T_Terminal_Info"));
	CHECK_STRING("200902",CommonDao::getString(ora1,tempbuffer,"select to_char(Current_Hire_Time,'YYYYMM') from T_Terminal_Info"));
	CHECK_NUMBER(0,CommonDao::getInt(ora1,"select status from T_SDR_200902"));

	CHECK_NUMBER(1030000,CommonDao::getInt(ora1,"select status from T_Terminal_Info where Imei_Number='012345678901234' and uim_number='ABCDEFGHIJKLMNO'"));
	CHECK_NUMBER(0,CommonDao::commit(ora1));
	//CHECK_NUMBER(1010000,CommonDao::getInt(ora1,"select status from T_Terminal_Info where Imei_Number='012345678901234' and uim_number='012345678901234'"));
	TEST_LABLE("����-ƽ̨���ú��������-���⴦��");
	CHECK_NUMBER(0,HiringDao::fire(ora1,"012345678901234ABCDEFGHIJKLMNO"));
	CHECK_NUMBER(1,CommonDao::getInt(ora1,"select count(*) from T_Terminal_Info"));
	//�����Ӧ�����Current_Hire_Time��
	CHECK_NUMBER(1,CommonDao::getInt(ora1,"select count(1) from T_Terminal_Info where Current_Hire_Time is null"));
	CHECK_NUMBER(1,CommonDao::getInt(ora1,"select status from T_SDR_200902"));

	CHECK_NUMBER(1010000,CommonDao::getInt(ora1,"select status from T_Terminal_Info where Imei_Number='012345678901234' and uim_number='ABCDEFGHIJKLMNO'"));
	CHECK_NUMBER(0,CommonDao::commit(ora1));
	//�������ٴ�����
	TEST_LABLE("����-ƽ̨���ú��������-�������ٴ�����");
	CHECK_NUMBER(0,HiringDao::hire(ora1,"012345678901234ABCDEFGHIJKLMNO"));
	CHECK_NUMBER(1,CommonDao::getInt(ora1,"select count(*) from T_Terminal_Info"));
	CHECK_STRING("200902",CommonDao::getString(ora1,tempbuffer,"select to_char(Current_Hire_Time,'YYYYMM') from T_Terminal_Info"));
	//Ӧ����2���˵�������
	CHECK_NUMBER(2,CommonDao::getInt(ora1,"select count(*) from T_SDR_200902"));

	CHECK_NUMBER(1030000,CommonDao::getInt(ora1,"select status from T_Terminal_Info where Imei_Number='012345678901234' and uim_number='ABCDEFGHIJKLMNO'"));
	CHECK_NUMBER(0,CommonDao::commit(ora1));

	TEST_LABLE("����-ƽ̨���ú��������-�������ٴ�����");
	CHECK_NUMBER(0,HiringDao::fire(ora1,"012345678901234ABCDEFGHIJKLMNO"));
	CHECK_NUMBER(1,CommonDao::getInt(ora1,"select count(*) from T_Terminal_Info"));
	//�����Ӧ�����Current_Hire_Time��
	CHECK_NUMBER(1,CommonDao::getInt(ora1,"select count(1) from T_Terminal_Info where Current_Hire_Time is null"));
	//���������˵�
	CHECK_NUMBER(2,CommonDao::getInt(ora1,"select count(*) from T_SDR_200902"));

	CHECK_NUMBER(1010000,CommonDao::getInt(ora1,"select status from T_Terminal_Info where Imei_Number='012345678901234' and uim_number='ABCDEFGHIJKLMNO'"));
	CHECK_NUMBER(0,CommonDao::commit(ora1));
	
	return 0;
}
/*һ��������ݵ�����
01 01 01
20090119101020 file://zh/path.avi 10
20090119101021 file://zh/path.avi 10
20090119101022 menu://zh/c2 10

*/
int testCommonDaoMethods(OLFO  *ora1,OLFO  *ora2,OLFO  *ora3)
{
	BlackListDao dao;
	char tempbuffer[1024];
	TEST_LABLE("����-ͨ�õ����ݿ���ʹ��ܲ���");

	CHECK_NUMBER(39,CommonDao::getDatabaseTime(ora1)/(86400*365));
	CHECK_STRING("string",CommonDao::getString(ora1,tempbuffer,"select 'string' from dual"));
	CHECK_NUMBER(123,CommonDao::getInt(ora1,"select to_number('123') from dual"));
	CHECK_NUMBER(1,CommonDao::getInt(ora1,"select 1/1 from dual"));
	CHECK_NUMBER(2,CommonDao::getInt(ora1,"select 1+1 from dual"));
	CHECK_NUMBER(0,CommonDao::getInt(ora1,"select 1-1 from dual"));
	CHECK_NUMBER(4,CommonDao::getInt(ora1,"select 2*2 from dual"));
	CHECK_NUMBER(20,CommonDao::getInt(ora1,"select 20-0 from dual"));
	CHECK_STRING("st",CommonDao::getString(ora1,tempbuffer,"select substr('string',1,2) from dual"));
	CHECK_STRING("file",CommonDao::getString(ora1,tempbuffer,"select substr('file://zh/path.avi',1,4) from dual"));
	CHECK_STRING("zh",CommonDao::getString(ora1,tempbuffer,"select substr('file://zh/path.avi',8,2) from dual"));
	CHECK_STRING("path.avi",CommonDao::getString(ora1,tempbuffer,"select substr('file://zh/path.avi',11) from dual"));
	CHECK_STRING("******gao",CommonDao::getString(ora1,tempbuffer,"select lpad(rpad('gao',3,'*'),9,'*') from dual"));
	//select lpad(rpad(��gao��,10,��*��),17,��*��)from dual; 
	return 0;
}
/*

Create Table T_Black_White_Version
(	Black_White_Type			Varchar2(1),		--�ڰ���������(0������;1������)
	Org_ID						Varchar2(10),		--�����������Ƶ�ID(������ʱ���ֶ���HAPPY)
	Update_Time					Date				--�ڰ���������޸�ʱ��
);
*/
int testBlackListDaoCheckIfUpdated(OLFO  *ora1,OLFO  *ora2,OLFO  *ora3)
{
	BlackListDao dao;

	TEST_LABLE("����-���������汾-�а汾�����");
	const char *args[]={
"delete from T_Terminal_Info",
"delete from T_Black_White_Version",
"insert into T_Black_White_Version(Black_White_Type,Org_ID,Update_Time)values(1,'hotel',sysdate)",
"insert into T_Terminal_Info(Imei_Number,Uim_Number,msisdn,status,room_no,hotel_id)values('012345678901234','ABCDEFGHIJKLMNO','13902392590',1020000,'room','hotel')",
	NULL};

	executeUpdate(ora3,args);
	CHECK_NUMBER(1,BlackListDao::checkIfUpdated(ora1,"012345678901234ABCDEFGHIJKLMNO",0));
	TEST_LABLE("����-���������汾-û�а汾�����");
	const char *args2[]={
"delete from T_Terminal_Info",
"delete from T_Black_White_Version",
"insert into T_Terminal_Info(Imei_Number,Uim_Number,status,room_no,hotel_id)values('012345678901234','ABCDEFGHIJKLMNO',1020000,'room','hotel')",
	NULL};

	executeUpdate(ora3,args2);
	CHECK_NUMBER(0,BlackListDao::checkIfUpdated(ora1,"012345678901234ABCDEFGHIJKLMNO",0));

	TEST_LABLE("����-���������汾-�汾��ͬ�����");
	const char *args3[]={
"delete from T_Terminal_Info",
"delete from T_Black_White_Version",
"insert into T_Black_White_Version(Black_White_Type,Org_ID,Update_Time)values(1,'hotel',to_date('19700102','yyyymmdd'))",
"insert into T_Terminal_Info(Imei_Number,Uim_Number,msisdn,status,room_no,hotel_id)values('012345678901234','ABCDEFGHIJKLMNO','13902392590',1020000,'room','hotel')",
	NULL};
	executeUpdate(ora3,args3);
	CHECK_NUMBER(0,BlackListDao::checkIfUpdated(ora1,"012345678901234ABCDEFGHIJKLMNO",1*86400*365));

	return 0;
}


int testWhiteListDaoCheckIfUpdated(OLFO  *ora1,OLFO  *ora2,OLFO  *ora3)
{
	BlackListDao dao;

	TEST_LABLE("����-���������汾-�а汾�����");
	const char *args[]={
"delete from T_Terminal_Info",
"delete from T_Black_White_Version",
"insert into T_Black_White_Version(Black_White_Type,Org_ID,Update_Time)values(0,'hotel',sysdate)",
"insert into T_Terminal_Info(Imei_Number,Uim_Number,msisdn,status,room_no,hotel_id)values('012345678901234','ABCDEFGHIJKLMNO','13902392590',1020000,'room','hotel')",
	NULL};

	executeUpdate(ora3,args);
	CHECK_NUMBER(1,WhiteListDao::checkIfUpdated(ora1,"012345678901234ABCDEFGHIJKLMNO",0));
	TEST_LABLE("����-���������汾-û�а汾�����");
	const char *args2[]={
"delete from T_Terminal_Info",
"delete from T_Black_White_Version",
"insert into T_Terminal_Info(Imei_Number,Uim_Number,msisdn,status,room_no,hotel_id)values('012345678901234','ABCDEFGHIJKLMNO','13902392590',1020000,'room','hotel')",
	NULL};

	executeUpdate(ora3,args2);
	CHECK_NUMBER(0,WhiteListDao::checkIfUpdated(ora1,"012345678901234ABCDEFGHIJKLMNO",0));

	TEST_LABLE("����-���������汾-�汾��ͬ�����");
	const char *args3[]={
"delete from T_Terminal_Info",
"delete from T_Black_White_Version",
"insert into T_Black_White_Version(Black_White_Type,Org_ID,Update_Time)values(0,'hotel',to_date('19700102','yyyymmdd'))",
"insert into T_Terminal_Info(Imei_Number,Uim_Number,msisdn,status,room_no,hotel_id)values('012345678901234','ABCDEFGHIJKLMNO','13902392590',1020000,'room','hotel')",
	NULL};
	executeUpdate(ora3,args3);
	CHECK_NUMBER(0,WhiteListDao::checkIfUpdated(ora1,"012345678901234ABCDEFGHIJKLMNO",1*86400*365));

	return 0;
}

int testBlackListDaoRetrieveList(OLFO  *ora1,OLFO  *ora2,OLFO  *ora3)
{
	BlackListDao dao;
	char tempbuffer[1024]={0};
	TEST_LABLE("����-ȡ�ú�����-û�к�����");
	const char *args[]={
"delete from T_Terminal_Info","delete from T_Black_White",
"delete from T_Black_White_Version",
"insert into T_Black_White_Version(Black_White_Type,Org_ID,Update_Time)values(1,'hotel',sysdate)",
"insert into T_Terminal_Info(Imei_Number,Uim_Number,msisdn,status,room_no,hotel_id)values('012345678901234','ABCDEFGHIJKLMNO','13902392590',1020000,'room','hotel')",
	NULL};

	executeUpdate(ora3,args);
	CHECK_NUMBER(0,BlackListDao::retrieveList(ora1,"012345678901234ABCDEFGHIJKLMNO",tempbuffer,1024));
	CHECK_STRING("",tempbuffer);
	TEST_LABLE("����-ȡ�ú�����-��һ��������");
	const char *args2[]={
"delete from T_Terminal_Info","delete from T_Black_White",
"delete from T_Black_White_Version",
"insert into T_Black_White_Version(Black_White_Type,Org_ID,Update_Time)values(1,'hotel',sysdate)",
"insert into T_Black_White(Black_White_Type,Phone_Number,Org_ID)values(1,'10086','hotel')",
"insert into T_Terminal_Info(Imei_Number,Uim_Number,msisdn,status,room_no,hotel_id)values('012345678901234','ABCDEFGHIJKLMNO','13902392590',1020000,'room','hotel')",
	NULL};

	executeUpdate(ora3,args2);
	CHECK_NUMBER(0,BlackListDao::retrieveList(ora1,"012345678901234ABCDEFGHIJKLMNO",tempbuffer,1024));
	CHECK_STRING("10086",tempbuffer);

	TEST_LABLE("����-ȡ�ú�����-�ж��������");
	const char *args3[]={
"delete from T_Terminal_Info","delete from T_Black_White",
"delete from T_Black_White_Version",
"insert into T_Black_White_Version(Black_White_Type,Org_ID,Update_Time)values(1,'hotel',sysdate)",
"insert into T_Black_White(Black_White_Type,Phone_Number,Org_ID,Join_Time)values(1,'10086','hotel',sysdate)",
"insert into T_Black_White(Black_White_Type,Phone_Number,Org_ID,Join_Time)values(1,'10000','hotel',sysdate)",
"insert into T_Black_White(Black_White_Type,Phone_Number,Org_ID,Join_Time)values(1,'13902382590','hotel',sysdate)",
"insert into T_Terminal_Info(Imei_Number,Uim_Number,msisdn,status,room_no,hotel_id)values('012345678901234','ABCDEFGHIJKLMNO','13902392590',1020000,'room','hotel')",
	NULL};

	executeUpdate(ora3,args3);
	CHECK_NUMBER(0,BlackListDao::retrieveList(ora1,"012345678901234ABCDEFGHIJKLMNO",tempbuffer,1024));
	CHECK_STRING("10086 10000 13902382590",tempbuffer);


	return 0;
}
// Windows Header Files:


// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include "TCPClient.h"
int testSimpelServer(OLFO  *ora1,OLFO  *ora2,OLFO  *ora3)
{
	TEST_LABLE("����-��������-���ú��������-��ʼ����");
	const char *args[]={
"delete from T_Terminal_Info","delete from T_SDR_200902",
"delete from T_Org_Info","insert into T_Org_Info(Org_ID,Superior_Org_ID)values('hotel','broker')",
"insert into T_Terminal_Info(Imei_Number,Uim_Number,msisdn,status,room_no,hotel_id)values('012345678901234','ABCDEFGHIJKLMNO','13902392590',1010000,'room','hotel')",
	NULL};
	executeUpdate(ora3,args);
	CHECK_NUMBER(1,HiringDao::checkIfCanBeHired(ora1,"012345678901234ABCDEFGHIJKLMNO"));
	SimpleServer server;
	server.run((void*)ora1);
	//SimpleServer::start((void*)ora2);
	//SimpleServer::sleep(600000);

#ifdef _WIN32
	WtdpPacket packet;
	//����һ����ͨ������
	TCPClient client;
	if(client.connect("127.0.0.1",7000)<0){		
		return -1;
	}
	WtdpPacket * p;
	p=packet.packServiceStartReq("012345678901234ABCDEFGHIJKLMNO");
	client.send(p->getHead(),p->getLength());	
	client.recv(p->getHead(),12);
	client.recv((char*)p->getBody(),p->getBodyLength());
	client.close();
	
	SimpleServer::sleep(7000);
	if(client.connect("127.0.0.1",7000)<0){		
		return -1;
	}
	p=packet.packServiceEndReq("012345678901234ABCDEFGHIJKLMNO",10);
	client.send(p->getHead(),p->getLength());
	client.recv(p->getHead(),12);
	client.recv((char*)p->getBody(),p->getBodyLength());
	client.close();

	SimpleServer::sleep(300);
	CHECK_NUMBER(1,CommonDao::getInt(ora1,"select status from T_SDR_200902"));
	CHECK_NUMBER(7,CommonDao::getInt(ora1," select (END_TIME-start_time)*86400 from T_SDR_200902"));
	//������7��ʱ��
#endif
	
	
	return 0;
}
/*

	try{
		if(client.connect("127.0.0.1",7000)<0){
		
			return -1;
		}
		client.send("test",4);
		client.close();
	}catch(Exception &e){
		e.report();
	}

*/
int testWhiteListDaoRetrieveList(OLFO  *ora1,OLFO  *ora2,OLFO  *ora3)
{
	BlackListDao dao;
	char tempbuffer[1024]={0};
	TEST_LABLE("����-ȡ�ð�����-û�а�����");
	const char *args[]={
"delete from T_Terminal_Info","delete from T_Black_White",
"delete from T_Black_White_Version",
"insert into T_Black_White_Version(Black_White_Type,Org_ID,Update_Time)values(0,'hotel',sysdate)",
"insert into T_Terminal_Info(Imei_Number,Uim_Number,msisdn,status,room_no,hotel_id)values('012345678901234','ABCDEFGHIJKLMNO','13902392590',1020000,'room','hotel')",
	NULL};

	executeUpdate(ora3,args);
	CHECK_NUMBER(0,WhiteListDao::retrieveList(ora1,"012345678901234ABCDEFGHIJKLMNO",tempbuffer,1024));
	CHECK_STRING("",tempbuffer);
	TEST_LABLE("����-ȡ�ð�����-��һ��������");
	const char *args2[]={
"delete from T_Terminal_Info","delete from T_Black_White",
"delete from T_Black_White_Version",
"insert into T_Black_White_Version(Black_White_Type,Org_ID,Update_Time)values(1,'hotel',sysdate)",
"insert into T_Black_White(Black_White_Type,Phone_Number,Org_ID)values(0,'10086','hotel')",
"insert into T_Terminal_Info(Imei_Number,Uim_Number,msisdn,status,room_no,hotel_id)values('012345678901234','ABCDEFGHIJKLMNO','13902392590',1020000,'room','hotel')",
	NULL};

	executeUpdate(ora3,args2);
	CHECK_NUMBER(0,WhiteListDao::retrieveList(ora1,"012345678901234ABCDEFGHIJKLMNO",tempbuffer,1024));
	CHECK_STRING("10086",tempbuffer);

	TEST_LABLE("����-ȡ�ð�����-�ж��������");
	const char *args3[]={
"delete from T_Terminal_Info","delete from T_Black_White",
"delete from T_Black_White_Version",
"insert into T_Black_White_Version(Black_White_Type,Org_ID,Update_Time)values(1,'hotel',sysdate)",
"insert into T_Black_White(Black_White_Type,Phone_Number,Org_ID,Join_Time)values(0,'10086','hotel',sysdate)",
"insert into T_Black_White(Black_White_Type,Phone_Number,Org_ID,Join_Time)values(0,'10000','hotel',sysdate)",
"insert into T_Black_White(Black_White_Type,Phone_Number,Org_ID,Join_Time)values(0,'13902382590','hotel',sysdate)",
"insert into T_Terminal_Info(Imei_Number,Uim_Number,msisdn,status,room_no,hotel_id)values('012345678901234','ABCDEFGHIJKLMNO','13902392590',1020000,'room','hotel')",
	NULL};

	executeUpdate(ora3,args3);
	CHECK_NUMBER(0,WhiteListDao::retrieveList(ora1,"012345678901234ABCDEFGHIJKLMNO",tempbuffer,1024));
	CHECK_STRING("10086 10000 13902382590",tempbuffer);


	return 0;
}
int testWtdpPacketCommandsSpec(OLFO  *ora1,OLFO  *ora2,OLFO  *ora3)
{
	TEST_LABLE("����-���������˶�-���������");
	WtdpPacket packet;

      CHECK_NUMBER(0x00000001,packet.packStatusSyncReq("012345678901234ABCDEFGHIJKLMNO",0,1,1,1)->getCommand());//״̬ͬ��
      CHECK_NUMBER(0x00000002,packet.packServiceStartReq("012345678901234ABCDEFGHIJKLMNO")->getCommand());//�û���ͨ
      CHECK_NUMBER(0x00000003,packet.packServiceEndReq("012345678901234ABCDEFGHIJKLMNO",10)->getCommand());//�û�����
      CHECK_NUMBER(0x00000004,packet.packHeartBeatingReq("012345678901234ABCDEFGHIJKLMNO")->getCommand());//����
      CHECK_NUMBER(0x00000005,packet.packStatisticsDeliverReq("012345678901234ABCDEFGHIJKLMNO","content",100)->getCommand());//����ͳ������
      //CHECK_NUMBER(0x00000006,packet.packStatusSyncReq("012345678901234ABCDEFGHIJKLMNO",0,1,1,1)->getCommand());//���ѳ�ֵ
      //CHECK_NUMBER(0x00000007,packet.packStatusSyncReq("012345678901234ABCDEFGHIJKLMNO",0,1,1,1)->getCommand());//IP��ֵ
      CHECK_NUMBER(0x00000011,packet.packParameterModificationReq("012345678901234ABCDEFGHIJKLMNO","content",100)->getCommand());//�޸�ϵͳ����
      CHECK_NUMBER(0x00000012,packet.packTimeSyncReq("012345678901234ABCDEFGHIJKLMNO")->getCommand());//ʱ��ͬ��
      CHECK_NUMBER(0x00000013,packet.packBlackListResettingReq("012345678901234ABCDEFGHIJKLMNO")->getCommand());//��պ�����
      CHECK_NUMBER(0x00000014,packet.packBlackListModificationReq("012345678901234ABCDEFGHIJKLMNO",10,"content",100)->getCommand());//���Ӻ�����
      CHECK_NUMBER(0x00000015,packet.packWhiteListResettingReq("012345678901234ABCDEFGHIJKLMNO")->getCommand());//��հ�����
      CHECK_NUMBER(0x00000016,packet.packWhiteListModificationReq("012345678901234ABCDEFGHIJKLMNO",10,"content",100)->getCommand());//���Ӱ�����
      CHECK_NUMBER(0x00000017,packet.packDisconnectingReq("012345678901234ABCDEFGHIJKLMNO")->getCommand());//������ɰ�
      //CHECK_NUMBER(0x00000018,packet.packStatusSyncReq("012345678901234ABCDEFGHIJKLMNO",0,1,1,1)->getCommand());//������Ͱ�
   TEST_LABLE("����-���������˶�-�ذ�");
	  //CHECK_NUMBER(0x00000101,packet.packStatusSyncResp("012345678901234ABCDEFGHIJKLMNO",0,1,1,1)->getCommand());//״̬ͬ���ذ�
      CHECK_NUMBER(0x00000102,packet.packServiceStartResp("012345678901234ABCDEFGHIJKLMNO",0)->getCommand());//�û���ͨ�ذ�
      CHECK_NUMBER(0x00000103,packet.packServiceEndResp("012345678901234ABCDEFGHIJKLMNO",0,1,2)->getCommand());//�û�����ذ�
      CHECK_NUMBER(0x00000104,packet.packHeartBeatingResp("012345678901234ABCDEFGHIJKLMNO",0)->getCommand());//�����ذ�
      CHECK_NUMBER(0x00000105,packet.packStatisticsDeliverResp("012345678901234ABCDEFGHIJKLMNO",0)->getCommand());//����ͳ�����ݻذ�
      //CHECK_NUMBER(0x00000106,packet.packHeartBeatingResp("012345678901234ABCDEFGHIJKLMNO",0)->getCommand());//���ѳ�ֵ�ذ�
      //CHECK_NUMBER(0x00000107,packet.packHeartBeatingResp("012345678901234ABCDEFGHIJKLMNO",0)->getCommand());//IP��ֵ�ذ�
      CHECK_NUMBER(0x00000111,packet.packParameterModificationResp("012345678901234ABCDEFGHIJKLMNO",0)->getCommand());//�޸�ϵͳ�����ذ�
      //CHECK_NUMBER(0x00000112,packet.("012345678901234ABCDEFGHIJKLMNO",0)->getCommand());//ʱ��ͬ���ذ�
      CHECK_NUMBER(0x00000113,packet.packBlackListResettingResp("012345678901234ABCDEFGHIJKLMNO",0)->getCommand());//��պ������ذ�
      CHECK_NUMBER(0x00000114,packet.packBlackListModificationResp("012345678901234ABCDEFGHIJKLMNO",0)->getCommand());//���Ӻ������ذ�
      CHECK_NUMBER(0x00000115,packet.packWhiteListResettingResp("012345678901234ABCDEFGHIJKLMNO",0)->getCommand());//��հ������ذ�
      CHECK_NUMBER(0x00000116,packet.packWhiteListModificationResp("012345678901234ABCDEFGHIJKLMNO",0)->getCommand());//���Ӱ������ذ�
      CHECK_NUMBER(0x00000117,packet.packDisconnectingResp("012345678901234ABCDEFGHIJKLMNO")->getCommand());//������ɰ��ذ�
      //CHECK_NUMBER(0x00000118,packet.packHeartBeatingResp("012345678901234ABCDEFGHIJKLMNO",0)->getCommand());//������Ͱ��ذ�
	
	//TEST_LABLE("����-�����Ⱥ˶�-���������");


	return 0;
}
/*
������	˵��	��ע
0x1000	�����ʽ����	����������ָ��
0x1001	�ն˲�ʶ���ָ��	������δ�����ָ��
0x1002	����������������ε�ƽ̨	��������������
0x1003	�ն�״̬�����Ѿ��ǿ�ͨ״̬	�����ڿ�ͨ�ն�
0x1004	�Ѿ��Ǹ�λ״̬	�����ڸ�λ�ն�
0x1005	�������Ѿ����	��������պ�����
0x1006	�������Ѿ����	��������հ�����
0x1007	��������������	���������Ӻ�����
0x1008	��������������	���������Ӱ�����
0x1009	SD�����ļ��Ƿ�	������У���ļ�
0x1010	�����а����Ƿ��绰����	�����ںڰ���������
0x1011	��ˮ�Ŵ���	ʹ��������ָ��
0x1012	ʱ���ʽ����	�������κ�ʱ�����������ͬ��ʱ��
0x1013	�ļ�����У��Ƿ�	������У���ļ�
0x1014	�޷�ʶ��Ĳ���	����������ָ��
0x1015	���Ϸ��Ĳ���ֵ	����������ָ��
0x1016	��ʶ������Դ���	��������������
0x1017	Ψһ��ʶ��ϵͳ�в�����	�����ڿ�ͨ�ն�
0x1018	ϵͳ�����ܾ����ն˵ĵ�¼	�����ڿ�ͨ�ն�
0x1019	ϵͳ������������ܾ����ն˵�¼	�����ڿ�ͨ�ն�
0x1020	Ψһ��ʶ��ϵͳ�в�����	�����������ն�
0x1021	ϵͳ��û�յ����ն˵Ŀ�ͨ����	�����������ն�
0x1022	ϵͳ������������ܾ����ն�����	�����������ն�
0x1023	�û�ѡ��ĵ���û�ж���	�����ڻ��ѳ�ֵ
0x1024	��ֵ�ӿ�Ŀǰ������	�����ڻ��ѳ�ֵ
0x1025	ϵͳ�����ܾ�����̨�ն˵ĳ�ֵ	�����ڻ��ѳ�ֵ
0x1026	ϵͳ�ڲ������³�ֵʧ��	�����ڻ��ѳ�ֵ
0x1027	�û�ѡ��ĵ���û�ж���	������IP��ֵ
0x1028	��ֵ�ӿ�Ŀǰ������	������IP��ֵ
0x1029	ϵͳ�����ܾ�����̨�ն˵ĳ�ֵ	������IP��ֵ
0x1030	ϵͳ�ڲ������³�ֵʧ��	������IP��ֵ
0x9000	�ն��ڲ�����	�ô�����Դ������ĵĴ�������


*/
int testTerminalServiceHiring(OLFO  *ora1,OLFO  *ora2,OLFO  *ora3)
{
	//char tempbuffer[1024];
	TEST_LABLE("����-���÷������-�ն�û�еǼ�");
	const char *args[]={
"delete from T_Terminal_Info",
"delete from T_SDR_200902",
"delete from T_Org_Info",
	NULL};
	executeUpdate(ora3,args);
	//ɶ��û�е�����²�������
	CHECK_NUMBER(0x1017,TerminalService::hire(ora1,"012345678901234ABCDEFGHIJKLMNO"));
	CHECK_NUMBER(0,TerminalService::commit(ora1));
	//�������ն��Ѿ�׼���ÿ�ͨ
	TEST_LABLE("����-���÷������-�ն˵���ǲ��Ҵ�������״̬1010000");
	const char *args1[]={
"delete from T_Terminal_Info",
"delete from T_Org_Info",
"insert into T_Org_Info(Org_ID,Superior_Org_ID)values('hotel','broker')",
"insert into T_Terminal_Info(Imei_Number,Uim_Number,msisdn,status,room_no,hotel_id)values('012345678901234','ABCDEFGHIJKLMNO','13902392590',1010000,'room','hotel')",
	NULL};
	executeUpdate(ora3,args1);
	CHECK_NUMBER(0,TerminalService::hire(ora1,"012345678901234ABCDEFGHIJKLMNO"));
	ora1->Commit();
	CHECK_NUMBER(0,TerminalService::commit(ora1));
	CHECK_NUMBER(1030000,CommonDao::getInt(ora1,"select status from T_Terminal_Info"));
	CHECK_NUMBER(1030000,CommonDao::getInt(ora2,"select status from T_Terminal_Info"));
	
	CHECK_NUMBER(0x1003,TerminalService::hire(ora1,"012345678901234ABCDEFGHIJKLMNO"));
	CHECK_NUMBER(0,TerminalService::commit(ora1));
	CHECK_NUMBER(0x1003,TerminalService::hire(ora1,"012345678901234ABCDEFGHIJKLMNO"));
	CHECK_NUMBER(0,TerminalService::commit(ora1));

	TEST_LABLE("����-���÷������-�������֮������");
	CHECK_NUMBER(0,TerminalService::fire(ora2,"012345678901234ABCDEFGHIJKLMNO"));
	CHECK_NUMBER(0,TerminalService::commit(ora2));
	
	CHECK_NUMBER(0x1021,TerminalService::fire(ora2,"012345678901234ABCDEFGHIJKLMNO"));
	CHECK_NUMBER(0,TerminalService::commit(ora2));
	CHECK_NUMBER(0x1021,TerminalService::fire(ora2,"012345678901234ABCDEFGHIJKLMNO"));
	CHECK_NUMBER(0,TerminalService::commit(ora2));
	//������Ϊ����ͨ״̬
	TEST_LABLE("����-���÷������-������ͨ������в���");
	const char *args2[]={
"update T_Terminal_Info set status='1020000' where imei_number='012345678901234'",
	NULL};
	executeUpdate(ora3,args2);
	CHECK_NUMBER(0x1018,TerminalService::hire(ora1,"012345678901234ABCDEFGHIJKLMNO"));
	CHECK_NUMBER(0,TerminalService::commit(ora1));	

	return 0;
}

int testmain(OLFO  *ora1,OLFO  *ora2,OLFO  *ora3)
{
	//CHECK_NUMBER(1,sizeof(AlarmingMessage));
	//CHECK_STRING("10655088",ora1->getErrorInfo());
	//testTimeRestriction

	if(false){
		
		testCommonDaoMethods(ora1,ora2,ora3);		
		testHireDaoCheckIfCanBeHired(ora1,ora2,ora3);
		testHireDaoHire(ora1,ora2,ora3);
		
		//testHireDaoConfirmHiring
		testHireDaoConfirmHiring(ora1,ora2,ora3);
		testBlackListDaoCheckIfUpdated(ora1,ora2,ora3);
		testBlackListDaoRetrieveList(ora1,ora2,ora3);
		testWhiteListDaoCheckIfUpdated(ora1,ora2,ora3);
		testWhiteListDaoRetrieveList(ora1,ora2,ora3);
		
		testHiringDaoRestoreTimeoutHireRequest(ora1,ora2,ora3);
		testAdStatsBodyParse(ora1,ora2,ora3);
		testAdStatsDaoAddToStats(ora1,ora2,ora3);
		testHireDaoFire(ora1,ora2,ora3);
		//
	}
	//testTerminalParameterDaoGetParameterExpr(ora1,ora2,ora3);
	//testWtdpPacketCommandsSpec(ora1,ora2,ora3);
	//testTerminalServiceHiring(ora1,ora2,ora3);
	testSimpelServer(ora1,ora2,ora3);

	if(false){
		
	
	}

	return 0;
}

/*
select Update_Time from T_Black_White_Version a,T_Terminal_Info b where a.Black_White_Type=1 and b.Imei_Number='012345678901234' and b.Uim_Number='ABCDEFGHIJKLMNO' and a.Org_ID=b.hotel_id;
select Phone_Number 
		from T_Black_White a,T_Terminal_Info b 
		where a.Black_White_Type=1 and b.Imei_Number='012345678901234' and b.Uim_Number='ABCDEFGHIJKLMNO' and a.Org_ID=b.hotel_id;

select ID,IMEI_NUMBER,MSISDN,click_count,hotel_id,create_time from t_access_log;
select * from t_sdr_200902;

select * from t_access_log;
select * from t_sdr_200902;
select * from t_hire_restore_log;

select ID||'|'||IMEI_NUMBER||'|'||UIM_NUMBER||'|'|| MSISDN||'|'|| HOTEL_ID||'|'||ROOM_NO||'|'||BROKER_ID||'|'||START_TIME||'|'||HIRE_SRC||'|'||HIRE_CONFIRM_OPErator||'|'||HIRE_CONFIRM_TIME||'|'||HIRE_NOTES||'|'||END_TIME||'|'||QUIT_SRC||'|'||QUIT_NOTES||'|'||STATUS from t_sdr_200902;


insert into T_VPT_Alarm(alarm_id)values(1);
update T_VPT_Alarm set sm_flag='9',vpt_msisdn='13671385839',service_msisdn='13671385839',second_msisdn='13000000000',lock_msisdn='13691357419',Called_Number0='13671385839',Called_Number1='',Called_Number2='',Called_Number3='',Called='0',Voice_Deal_Flag='9',insert_time=to_date('2008-10-11','YYYY-MM-DD') where alarm_id=1;
commit;

delete from  T_VPT_Alarm where alarm_id=1;
commit;

select * from T_vpt_Alarm_200810;

*/





