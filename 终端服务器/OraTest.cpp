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
 函数名称: executeUpdate
 函数功能: 成批执行一个更新语句组,在最后的时候提交,
 		   这里有个特别设计,就是利用能识别的字符来
 		   判断是否字符数组是否已经结束,使用的时候不必在最后加入NULL字符
 输出参数:
 返 回 值: 0-成功,其他-失败
 全局变量:
 编写作者: zhangxilai
 编写日期: 2008年10月13日
 变更日志
**********************************************************************/
int executeUpdate(OLFO  *oracle,const char**arg)
{
	const char **p=arg;
	while(*p!=NULL){
		
		if(0>oracle->ExecSQL((char*)*p)){
			log(__FILE__,__LINE__,"sql执行出错,%s: %s\n",oracle->getErrorInfo(),*p);
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
		printf("参数形式应该是 test <oracleusername> <oraclepassword> <tnsname> \n");
		return 0;
	}
	OLFO oracle[4]={OLFO(OCI_THREADED),OLFO(OCI_THREADED),OLFO(OCI_THREADED),OLFO(OCI_THREADED)};
	int i=0;
	for(i=0;i<4;i++){
		
		if(0!=oracle[i].Connect(argv[1],argv[2],argv[3])){
			printf("Oracle连接失败,连接字符串是%s/%s@%s\n",argv[1],argv[2],argv[3]);
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

	TEST_LABLE("测试-检查是否可以被租用-处理成功");
	const char *args[]={
"delete from T_Terminal_Info",
	NULL};
	executeUpdate(ora3,args);
	//无此终端，不可以被租用
	CHECK_NUMBER(-2,HiringDao::checkIfCanBeHired(ora1,"012345678901234ABCDEFGHIJKLMNO"));
	CHECK_NUMBER(-2,HiringDao::checkIfCanBeFired(ora1,"012345678901234ABCDEFGHIJKLMNO"));

	//
	const char *args2[]={
"delete from T_Terminal_Info",
"insert into T_Terminal_Info(Imei_Number,Uim_Number,status)values('012345678901234','ABCDEFGHIJKLMNO',0)",
	NULL};
	executeUpdate(ora3,args2);
	//状态不对，不可以被租用
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

	TEST_LABLE("测试-获取系统参数-没有任何系统参数");
	char tempbuffer[1024];
	const char *args[]={
"delete from t_terminal_parm",
	NULL};
	executeUpdate(ora3,args);
	//无此终端，不可以被租用
	CHECK_NUMBER(-1,TerminalParameterDao::getParameterExpr(ora1,"012345678901234ABCDEFGHIJKLMNO",tempbuffer,1024));
	CHECK_STRING("",tempbuffer);
	TEST_LABLE("测试-获取系统参数-有系统参数-*号情形");

	const char *args1[]={
"delete from t_terminal_parm",
"insert into t_terminal_parm(id,Imei_Number,available,Parameters,create_time,modify_time)values(1,'*',1,'WAP=http:// x=y',sysdate,sysdate)",
	NULL};
	executeUpdate(ora3,args1);
	CHECK_NUMBER(0,TerminalParameterDao::getParameterExpr(ora1,"012345678901234ABCDEFGHIJKLMNO",tempbuffer,1024));
	CHECK_STRING("WAP=http:// x=y",tempbuffer);
	TEST_LABLE("测试-获取系统参数-有系统参数-匹配IMEI");

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
	
	TEST_LABLE("测试-检查广告解析-正确解析");
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
	
	TEST_LABLE("测试-检查广告入库-正确解析");
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
	TEST_LABLE("测试-租用测试-处理成功");
	const char *args[]={
"delete from T_Terminal_Info","delete from T_SDR_200902",
"delete from T_Org_Info","insert into T_Org_Info(Org_ID,Superior_Org_ID)values('hotel','broker')",
"insert into T_Terminal_Info(Imei_Number,Uim_Number,msisdn,status,room_no,hotel_id)values('012345678901234','ABCDEFGHIJKLMNO','13902392590',1010000,'room','hotel')",
	NULL};
	executeUpdate(ora3,args);
	//当前可以租用
	CHECK_NUMBER(0,HiringDao::hire(ora1,"012345678901234ABCDEFGHIJKLMNO"));
	CHECK_NUMBER(1,CommonDao::getInt(ora1,"select count(*) from T_Terminal_Info"));
	CHECK_STRING("200902",CommonDao::getString(ora1,tempbuffer,"select to_char(Current_Hire_Time,'YYYYMM') from T_Terminal_Info"));
	CHECK_NUMBER(1,CommonDao::getInt(ora1,"select count(*) from T_SDR_200902"));

	CHECK_NUMBER(1030000,CommonDao::getInt(ora1,"select status from T_Terminal_Info where Imei_Number='012345678901234' and uim_number='ABCDEFGHIJKLMNO'"));
	CHECK_NUMBER(0,CommonDao::commit(ora1));
	//CHECK_NUMBER(1010000,CommonDao::getInt(ora1,"select status from T_Terminal_Info where Imei_Number='012345678901234' and uim_number='012345678901234'"));
	
	TEST_LABLE("测试-租用测试-无终端可以被租用");
	//
	const char *args2[]={
"delete from T_Terminal_Info",
	NULL};
	
	executeUpdate(ora3,args2);
	//状态不对，不可以被租用
	CHECK_NUMBER(-3,HiringDao::hire(ora1,"012345678901234ABCDEFGHIJKLMNO"));

	
	return 0;
}



int testHiringDaoRestoreTimeoutHireRequest(OLFO  *ora1,OLFO  *ora2,OLFO  *ora3)
{
	//char tempbuffer[1024];
	TEST_LABLE("测试-租用测试-处理成功");
	const char *args[]={
"delete from T_Terminal_Info","delete from t_hire_restore_log",
"delete from T_Org_Info","insert into T_Org_Info(Org_ID,Superior_Org_ID)values('hotel','broker')",
"insert into T_Terminal_Info(hire_request_time,Imei_Number,Uim_Number,msisdn,status,room_no,hotel_id)values(sysdate-1,'012345678901234','ABCDEFGHIJKLMNO','13902392590',1020000,'room','hotel')",
	NULL};
	executeUpdate(ora3,args);
	//当前可以租用
	CHECK_NUMBER(0,HiringDao::restoreTimeoutHireRequest(ora1));
	CHECK_NUMBER(1,CommonDao::getInt(ora1,"select count(*) from T_Terminal_Info"));
	CHECK_NUMBER(1,CommonDao::getInt(ora1,"select count(*) from t_hire_restore_log"));
	

	return 0;
}

int testHireDaoFire(OLFO  *ora1,OLFO  *ora2,OLFO  *ora3)
{
	char tempbuffer[1024];
	TEST_LABLE("测试-租用后退租测试-开始租用");
	const char *args[]={
"delete from T_Terminal_Info","delete from T_SDR_200902",
"delete from T_Org_Info","insert into T_Org_Info(Org_ID,Superior_Org_ID)values('hotel','broker')",
"insert into T_Terminal_Info(Imei_Number,Uim_Number,msisdn,status,room_no,hotel_id)values('012345678901234','ABCDEFGHIJKLMNO','13902392590',1010000,'room','hotel')",
	NULL};
	executeUpdate(ora3,args);
	//当前可以租用
	CHECK_NUMBER(0,HiringDao::hire(ora1,"012345678901234ABCDEFGHIJKLMNO"));
	CHECK_NUMBER(1,CommonDao::getInt(ora1,"select count(*) from T_Terminal_Info"));
	CHECK_STRING("200902",CommonDao::getString(ora1,tempbuffer,"select to_char(Current_Hire_Time,'YYYYMM') from T_Terminal_Info"));
	CHECK_NUMBER(0,CommonDao::getInt(ora1,"select status from T_SDR_200902"));

	CHECK_NUMBER(1030000,CommonDao::getInt(ora1,"select status from T_Terminal_Info where Imei_Number='012345678901234' and uim_number='ABCDEFGHIJKLMNO'"));
	CHECK_NUMBER(0,CommonDao::commit(ora1));
	//CHECK_NUMBER(1010000,CommonDao::getInt(ora1,"select status from T_Terminal_Info where Imei_Number='012345678901234' and uim_number='012345678901234'"));
	TEST_LABLE("测试-租用后退租测试-退租处理");
	CHECK_NUMBER(0,HiringDao::fire(ora1,"012345678901234ABCDEFGHIJKLMNO"));
	CHECK_NUMBER(1,CommonDao::getInt(ora1,"select count(*) from T_Terminal_Info"));
	//退租后应该清空Current_Hire_Time和
	CHECK_NUMBER(1,CommonDao::getInt(ora1,"select count(1) from T_Terminal_Info where Current_Hire_Time is null"));
	CHECK_NUMBER(1,CommonDao::getInt(ora1,"select status from T_SDR_200902"));

	CHECK_NUMBER(1010000,CommonDao::getInt(ora1,"select status from T_Terminal_Info where Imei_Number='012345678901234' and uim_number='ABCDEFGHIJKLMNO'"));
	CHECK_NUMBER(0,CommonDao::commit(ora1));
	//退租后的再次租用
	TEST_LABLE("测试-租用后退租测试-退租后的再次租用");
	CHECK_NUMBER(0,HiringDao::hire(ora1,"012345678901234ABCDEFGHIJKLMNO"));
	CHECK_NUMBER(1,CommonDao::getInt(ora1,"select count(*) from T_Terminal_Info"));
	CHECK_STRING("200902",CommonDao::getString(ora1,tempbuffer,"select to_char(Current_Hire_Time,'YYYYMM') from T_Terminal_Info"));
	//应该有2个账单生成了
	CHECK_NUMBER(2,CommonDao::getInt(ora1,"select count(*) from T_SDR_200902"));

	CHECK_NUMBER(1030000,CommonDao::getInt(ora1,"select status from T_Terminal_Info where Imei_Number='012345678901234' and uim_number='ABCDEFGHIJKLMNO'"));
	CHECK_NUMBER(0,CommonDao::commit(ora1));

	TEST_LABLE("测试-租用后退租测试-退租后的再次退租");
	CHECK_NUMBER(0,HiringDao::fire(ora1,"012345678901234ABCDEFGHIJKLMNO"));
	CHECK_NUMBER(1,CommonDao::getInt(ora1,"select count(*) from T_Terminal_Info"));
	//退租后应该清空Current_Hire_Time和
	CHECK_NUMBER(1,CommonDao::getInt(ora1,"select count(1) from T_Terminal_Info where Current_Hire_Time is null"));
	//还是两个账单
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
	TEST_LABLE("测试-平台租用后退租测试-开始租用");
	const char *args[]={
"delete from T_Terminal_Info","delete from T_SDR_200902",
"delete from T_Org_Info","insert into T_Org_Info(Org_ID,Superior_Org_ID)values('hotel','broker')",
"insert into T_Terminal_Info(Imei_Number,Uim_Number,msisdn,status,room_no,hotel_id)values('012345678901234','ABCDEFGHIJKLMNO','13902392590',1020000,'room','hotel')",
	NULL};
	executeUpdate(ora3,args);
	//当前可以租用
	CHECK_NUMBER(0,HiringDao::confirmHiring(ora1,"012345678901234ABCDEFGHIJKLMNO"));
	CHECK_NUMBER(1,CommonDao::getInt(ora1,"select count(*) from T_Terminal_Info"));
	CHECK_STRING("200902",CommonDao::getString(ora1,tempbuffer,"select to_char(Current_Hire_Time,'YYYYMM') from T_Terminal_Info"));
	CHECK_NUMBER(0,CommonDao::getInt(ora1,"select status from T_SDR_200902"));

	CHECK_NUMBER(1030000,CommonDao::getInt(ora1,"select status from T_Terminal_Info where Imei_Number='012345678901234' and uim_number='ABCDEFGHIJKLMNO'"));
	CHECK_NUMBER(0,CommonDao::commit(ora1));
	//CHECK_NUMBER(1010000,CommonDao::getInt(ora1,"select status from T_Terminal_Info where Imei_Number='012345678901234' and uim_number='012345678901234'"));
	TEST_LABLE("测试-平台租用后退租测试-退租处理");
	CHECK_NUMBER(0,HiringDao::fire(ora1,"012345678901234ABCDEFGHIJKLMNO"));
	CHECK_NUMBER(1,CommonDao::getInt(ora1,"select count(*) from T_Terminal_Info"));
	//退租后应该清空Current_Hire_Time和
	CHECK_NUMBER(1,CommonDao::getInt(ora1,"select count(1) from T_Terminal_Info where Current_Hire_Time is null"));
	CHECK_NUMBER(1,CommonDao::getInt(ora1,"select status from T_SDR_200902"));

	CHECK_NUMBER(1010000,CommonDao::getInt(ora1,"select status from T_Terminal_Info where Imei_Number='012345678901234' and uim_number='ABCDEFGHIJKLMNO'"));
	CHECK_NUMBER(0,CommonDao::commit(ora1));
	//退租后的再次租用
	TEST_LABLE("测试-平台租用后退租测试-退租后的再次租用");
	CHECK_NUMBER(0,HiringDao::hire(ora1,"012345678901234ABCDEFGHIJKLMNO"));
	CHECK_NUMBER(1,CommonDao::getInt(ora1,"select count(*) from T_Terminal_Info"));
	CHECK_STRING("200902",CommonDao::getString(ora1,tempbuffer,"select to_char(Current_Hire_Time,'YYYYMM') from T_Terminal_Info"));
	//应该有2个账单生成了
	CHECK_NUMBER(2,CommonDao::getInt(ora1,"select count(*) from T_SDR_200902"));

	CHECK_NUMBER(1030000,CommonDao::getInt(ora1,"select status from T_Terminal_Info where Imei_Number='012345678901234' and uim_number='ABCDEFGHIJKLMNO'"));
	CHECK_NUMBER(0,CommonDao::commit(ora1));

	TEST_LABLE("测试-平台租用后退租测试-退租后的再次退租");
	CHECK_NUMBER(0,HiringDao::fire(ora1,"012345678901234ABCDEFGHIJKLMNO"));
	CHECK_NUMBER(1,CommonDao::getInt(ora1,"select count(*) from T_Terminal_Info"));
	//退租后应该清空Current_Hire_Time和
	CHECK_NUMBER(1,CommonDao::getInt(ora1,"select count(1) from T_Terminal_Info where Current_Hire_Time is null"));
	//还是两个账单
	CHECK_NUMBER(2,CommonDao::getInt(ora1,"select count(*) from T_SDR_200902"));

	CHECK_NUMBER(1010000,CommonDao::getInt(ora1,"select status from T_Terminal_Info where Imei_Number='012345678901234' and uim_number='ABCDEFGHIJKLMNO'"));
	CHECK_NUMBER(0,CommonDao::commit(ora1));
	
	return 0;
}
/*一个广告数据的样本
01 01 01
20090119101020 file://zh/path.avi 10
20090119101021 file://zh/path.avi 10
20090119101022 menu://zh/c2 10

*/
int testCommonDaoMethods(OLFO  *ora1,OLFO  *ora2,OLFO  *ora3)
{
	BlackListDao dao;
	char tempbuffer[1024];
	TEST_LABLE("测试-通用的数据库访问功能测试");

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
	//select lpad(rpad(’gao’,10,’*’),17,’*’)from dual; 
	return 0;
}
/*

Create Table T_Black_White_Version
(	Black_White_Type			Varchar2(1),		--黑白名单类型(0白名单;1黑名单)
	Org_ID						Varchar2(10),		--黑名单所属酒店ID(白名单时此字段填HAPPY)
	Update_Time					Date				--黑白名单最后修改时间
);
*/
int testBlackListDaoCheckIfUpdated(OLFO  *ora1,OLFO  *ora2,OLFO  *ora3)
{
	BlackListDao dao;

	TEST_LABLE("测试-检查黑名单版本-有版本的情况");
	const char *args[]={
"delete from T_Terminal_Info",
"delete from T_Black_White_Version",
"insert into T_Black_White_Version(Black_White_Type,Org_ID,Update_Time)values(1,'hotel',sysdate)",
"insert into T_Terminal_Info(Imei_Number,Uim_Number,msisdn,status,room_no,hotel_id)values('012345678901234','ABCDEFGHIJKLMNO','13902392590',1020000,'room','hotel')",
	NULL};

	executeUpdate(ora3,args);
	CHECK_NUMBER(1,BlackListDao::checkIfUpdated(ora1,"012345678901234ABCDEFGHIJKLMNO",0));
	TEST_LABLE("测试-检查黑名单版本-没有版本的情况");
	const char *args2[]={
"delete from T_Terminal_Info",
"delete from T_Black_White_Version",
"insert into T_Terminal_Info(Imei_Number,Uim_Number,status,room_no,hotel_id)values('012345678901234','ABCDEFGHIJKLMNO',1020000,'room','hotel')",
	NULL};

	executeUpdate(ora3,args2);
	CHECK_NUMBER(0,BlackListDao::checkIfUpdated(ora1,"012345678901234ABCDEFGHIJKLMNO",0));

	TEST_LABLE("测试-检查黑名单版本-版本相同的情况");
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

	TEST_LABLE("测试-检查白名单版本-有版本的情况");
	const char *args[]={
"delete from T_Terminal_Info",
"delete from T_Black_White_Version",
"insert into T_Black_White_Version(Black_White_Type,Org_ID,Update_Time)values(0,'hotel',sysdate)",
"insert into T_Terminal_Info(Imei_Number,Uim_Number,msisdn,status,room_no,hotel_id)values('012345678901234','ABCDEFGHIJKLMNO','13902392590',1020000,'room','hotel')",
	NULL};

	executeUpdate(ora3,args);
	CHECK_NUMBER(1,WhiteListDao::checkIfUpdated(ora1,"012345678901234ABCDEFGHIJKLMNO",0));
	TEST_LABLE("测试-检查白名单版本-没有版本的情况");
	const char *args2[]={
"delete from T_Terminal_Info",
"delete from T_Black_White_Version",
"insert into T_Terminal_Info(Imei_Number,Uim_Number,msisdn,status,room_no,hotel_id)values('012345678901234','ABCDEFGHIJKLMNO','13902392590',1020000,'room','hotel')",
	NULL};

	executeUpdate(ora3,args2);
	CHECK_NUMBER(0,WhiteListDao::checkIfUpdated(ora1,"012345678901234ABCDEFGHIJKLMNO",0));

	TEST_LABLE("测试-检查白名单版本-版本相同的情况");
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
	TEST_LABLE("测试-取得黑名单-没有黑名单");
	const char *args[]={
"delete from T_Terminal_Info","delete from T_Black_White",
"delete from T_Black_White_Version",
"insert into T_Black_White_Version(Black_White_Type,Org_ID,Update_Time)values(1,'hotel',sysdate)",
"insert into T_Terminal_Info(Imei_Number,Uim_Number,msisdn,status,room_no,hotel_id)values('012345678901234','ABCDEFGHIJKLMNO','13902392590',1020000,'room','hotel')",
	NULL};

	executeUpdate(ora3,args);
	CHECK_NUMBER(0,BlackListDao::retrieveList(ora1,"012345678901234ABCDEFGHIJKLMNO",tempbuffer,1024));
	CHECK_STRING("",tempbuffer);
	TEST_LABLE("测试-取得黑名单-有一个黑名单");
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

	TEST_LABLE("测试-取得黑名单-有多个黑名单");
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
	TEST_LABLE("测试-网络请求-租用后退租测试-开始租用");
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
	//发送一个开通包看看
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
	//租用了7秒时间
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
	TEST_LABLE("测试-取得白名单-没有白名单");
	const char *args[]={
"delete from T_Terminal_Info","delete from T_Black_White",
"delete from T_Black_White_Version",
"insert into T_Black_White_Version(Black_White_Type,Org_ID,Update_Time)values(0,'hotel',sysdate)",
"insert into T_Terminal_Info(Imei_Number,Uim_Number,msisdn,status,room_no,hotel_id)values('012345678901234','ABCDEFGHIJKLMNO','13902392590',1020000,'room','hotel')",
	NULL};

	executeUpdate(ora3,args);
	CHECK_NUMBER(0,WhiteListDao::retrieveList(ora1,"012345678901234ABCDEFGHIJKLMNO",tempbuffer,1024));
	CHECK_STRING("",tempbuffer);
	TEST_LABLE("测试-取得白名单-有一个白名单");
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

	TEST_LABLE("测试-取得白名单-有多个白名单");
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
	TEST_LABLE("测试-包命令代码核对-主动请求包");
	WtdpPacket packet;

      CHECK_NUMBER(0x00000001,packet.packStatusSyncReq("012345678901234ABCDEFGHIJKLMNO",0,1,1,1)->getCommand());//状态同步
      CHECK_NUMBER(0x00000002,packet.packServiceStartReq("012345678901234ABCDEFGHIJKLMNO")->getCommand());//用户开通
      CHECK_NUMBER(0x00000003,packet.packServiceEndReq("012345678901234ABCDEFGHIJKLMNO",10)->getCommand());//用户退租
      CHECK_NUMBER(0x00000004,packet.packHeartBeatingReq("012345678901234ABCDEFGHIJKLMNO")->getCommand());//心跳
      CHECK_NUMBER(0x00000005,packet.packStatisticsDeliverReq("012345678901234ABCDEFGHIJKLMNO","content",100)->getCommand());//发送统计数据
      //CHECK_NUMBER(0x00000006,packet.packStatusSyncReq("012345678901234ABCDEFGHIJKLMNO",0,1,1,1)->getCommand());//话费充值
      //CHECK_NUMBER(0x00000007,packet.packStatusSyncReq("012345678901234ABCDEFGHIJKLMNO",0,1,1,1)->getCommand());//IP充值
      CHECK_NUMBER(0x00000011,packet.packParameterModificationReq("012345678901234ABCDEFGHIJKLMNO","content",100)->getCommand());//修改系统参数
      CHECK_NUMBER(0x00000012,packet.packTimeSyncReq("012345678901234ABCDEFGHIJKLMNO")->getCommand());//时间同步
      CHECK_NUMBER(0x00000013,packet.packBlackListResettingReq("012345678901234ABCDEFGHIJKLMNO")->getCommand());//清空黑名单
      CHECK_NUMBER(0x00000014,packet.packBlackListModificationReq("012345678901234ABCDEFGHIJKLMNO",10,"content",100)->getCommand());//增加黑名单
      CHECK_NUMBER(0x00000015,packet.packWhiteListResettingReq("012345678901234ABCDEFGHIJKLMNO")->getCommand());//清空白名单
      CHECK_NUMBER(0x00000016,packet.packWhiteListModificationReq("012345678901234ABCDEFGHIJKLMNO",10,"content",100)->getCommand());//增加白名单
      CHECK_NUMBER(0x00000017,packet.packDisconnectingReq("012345678901234ABCDEFGHIJKLMNO")->getCommand());//命令完成包
      //CHECK_NUMBER(0x00000018,packet.packStatusSyncReq("012345678901234ABCDEFGHIJKLMNO",0,1,1,1)->getCommand());//广告推送包
   TEST_LABLE("测试-包命令代码核对-回包");
	  //CHECK_NUMBER(0x00000101,packet.packStatusSyncResp("012345678901234ABCDEFGHIJKLMNO",0,1,1,1)->getCommand());//状态同步回包
      CHECK_NUMBER(0x00000102,packet.packServiceStartResp("012345678901234ABCDEFGHIJKLMNO",0)->getCommand());//用户开通回包
      CHECK_NUMBER(0x00000103,packet.packServiceEndResp("012345678901234ABCDEFGHIJKLMNO",0,1,2)->getCommand());//用户退租回包
      CHECK_NUMBER(0x00000104,packet.packHeartBeatingResp("012345678901234ABCDEFGHIJKLMNO",0)->getCommand());//心跳回包
      CHECK_NUMBER(0x00000105,packet.packStatisticsDeliverResp("012345678901234ABCDEFGHIJKLMNO",0)->getCommand());//发送统计数据回包
      //CHECK_NUMBER(0x00000106,packet.packHeartBeatingResp("012345678901234ABCDEFGHIJKLMNO",0)->getCommand());//话费充值回包
      //CHECK_NUMBER(0x00000107,packet.packHeartBeatingResp("012345678901234ABCDEFGHIJKLMNO",0)->getCommand());//IP充值回包
      CHECK_NUMBER(0x00000111,packet.packParameterModificationResp("012345678901234ABCDEFGHIJKLMNO",0)->getCommand());//修改系统参数回包
      //CHECK_NUMBER(0x00000112,packet.("012345678901234ABCDEFGHIJKLMNO",0)->getCommand());//时间同步回包
      CHECK_NUMBER(0x00000113,packet.packBlackListResettingResp("012345678901234ABCDEFGHIJKLMNO",0)->getCommand());//清空黑名单回包
      CHECK_NUMBER(0x00000114,packet.packBlackListModificationResp("012345678901234ABCDEFGHIJKLMNO",0)->getCommand());//增加黑名单回包
      CHECK_NUMBER(0x00000115,packet.packWhiteListResettingResp("012345678901234ABCDEFGHIJKLMNO",0)->getCommand());//清空白名单回包
      CHECK_NUMBER(0x00000116,packet.packWhiteListModificationResp("012345678901234ABCDEFGHIJKLMNO",0)->getCommand());//增加白名单回包
      CHECK_NUMBER(0x00000117,packet.packDisconnectingResp("012345678901234ABCDEFGHIJKLMNO")->getCommand());//命令完成包回包
      //CHECK_NUMBER(0x00000118,packet.packHeartBeatingResp("012345678901234ABCDEFGHIJKLMNO",0)->getCommand());//广告推送包回包
	
	//TEST_LABLE("测试-包长度核对-主动请求包");


	return 0;
}
/*
错误码	说明	备注
0x1000	命令格式错误	适用于所有指令
0x1001	终端不识别的指令	适用于未定义的指令
0x1002	不允许操作，不信任的平台	适用于所有命令
0x1003	终端状态错误，已经是开通状态	适用于开通终端
0x1004	已经是复位状态	适用于复位终端
0x1005	黑名单已经清空	适用于清空黑名单
0x1006	白名单已经清空	适用于清空白名单
0x1007	黑名单数量超限	适用于增加黑名单
0x1008	白名单数量超限	适用于增加白名单
0x1009	SD卡上文件非法	适用于校验文件
0x1010	命令中包含非法电话号码	适用于黑白名单操作
0x1011	流水号错误	使用于所有指令
0x1012	时间格式错误	适用于任何时间操作，比如同步时间
0x1013	文件内容校验非法	适用于校验文件
0x1014	无法识别的参数	适用于所有指令
0x1015	不合法的参数值	适用于所有指令
0x1016	不识别的语言代码	适用于设置语言
0x1017	唯一标识在系统中不存在	适用于开通终端
0x1018	系统主动拒绝了终端的登录	适用于开通终端
0x1019	系统有致命错误而拒绝了终端登录	适用于开通终端
0x1020	唯一标识在系统中不存在	适用于退租终端
0x1021	系统从没收到过终端的开通请求	适用于退租终端
0x1022	系统有致命错误而拒绝了终端退租	适用于退租终端
0x1023	用户选择的档次没有定义	适用于话费充值
0x1024	充值接口目前不可用	适用于话费充值
0x1025	系统主动拒绝了这台终端的充值	适用于话费充值
0x1026	系统内部错误导致充值失败	适用于话费充值
0x1027	用户选择的档次没有定义	适用于IP充值
0x1028	充值接口目前不可用	适用于IP充值
0x1029	系统主动拒绝了这台终端的充值	适用于IP充值
0x1030	系统内部错误导致充值失败	适用于IP充值
0x9000	终端内部错误	该错误可以带上中文的错误描述


*/
int testTerminalServiceHiring(OLFO  *ora1,OLFO  *ora2,OLFO  *ora3)
{
	//char tempbuffer[1024];
	TEST_LABLE("测试-租用服务测试-终端没有登记");
	const char *args[]={
"delete from T_Terminal_Info",
"delete from T_SDR_200902",
"delete from T_Org_Info",
	NULL};
	executeUpdate(ora3,args);
	//啥都没有的情况下测试租用
	CHECK_NUMBER(0x1017,TerminalService::hire(ora1,"012345678901234ABCDEFGHIJKLMNO"));
	CHECK_NUMBER(0,TerminalService::commit(ora1));
	//现在这终端已经准备好开通
	TEST_LABLE("测试-租用服务测试-终端登完记并且处于退租状态1010000");
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

	TEST_LABLE("测试-租用服务测试-租用完成之后退租");
	CHECK_NUMBER(0,TerminalService::fire(ora2,"012345678901234ABCDEFGHIJKLMNO"));
	CHECK_NUMBER(0,TerminalService::commit(ora2));
	
	CHECK_NUMBER(0x1021,TerminalService::fire(ora2,"012345678901234ABCDEFGHIJKLMNO"));
	CHECK_NUMBER(0,TerminalService::commit(ora2));
	CHECK_NUMBER(0x1021,TerminalService::fire(ora2,"012345678901234ABCDEFGHIJKLMNO"));
	CHECK_NUMBER(0,TerminalService::commit(ora2));
	//设置其为欲开通状态
	TEST_LABLE("测试-租用服务测试-在欲开通情况进行测试");
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





