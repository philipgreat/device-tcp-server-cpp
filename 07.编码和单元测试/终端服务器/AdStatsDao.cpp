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
 函数名称: addToStats
 函数功能: 把统计数据加入到统计中
 输出参数:
 返 回 值: 0-不是,1-是,其他小于0的值：失败
 全局变量:
 编写作者: zhangxilai
 编写日期: 2009年01月23日
 变更日志: 
 相关 SQL：
**********************************************************************/
/*一个广告数据的样本
01 01 01
20090119101020 file://zh/path.avi 10
20090119101021 file://zh/path.avi 10
20090119101022 menu://zh/c2 10
drop table 		t_access_log;

Create Table T_Terminal_Info
(	imei_number					varchar2(15),		--终端的15位IMEI号
	uim_number					varchar2(15),		--UIM卡的15位IMSI
	msisdn						varchar2(11),		--手机号码(13.../15.../18...)
	status						number(8),			--终端的状态
	last_hire_time				date,				--上次开通时间
	last_quit_hire_time			date,				--上次退租时间
	hire_request_time			date,				--预开通时间
	current_hire_time			date,				--本次开通时间
	product_org_id				varchar2(10),		--终端生产厂家ID
	terminal_type				varchar2(16),		--终端型号
	product_date				varchar2(8),		--终端生产日期
	maintain_limited_date		varchar2(8),		--厂家保修截止日期(yyyymmdd)
	join_time					date,				--入库时间
	join_user_id				varchar2(16),		--入库操作员ID
	distribute_time				date,				--把终端分发到酒店的时间
	distribute_user_id			varchar2(16),		--把终端分发到酒店的操作员ID
	hotel_id					varchar2(10),		--终端当前分发到的酒店ID(退回到平台则本自动置为空)
	hotel_receiver				varchar2(32),		--酒店领用人姓名
	content_version				varchar2(30),		--内容版本号
	room_no						varchar2(8),		--终端分配的酒店房间号码
	language					varchar2(2),		--终端选择的语言
	last_hb_time				date				--最后一次心跳时间
) 
*/
#include "AdStatsBody.h"
int AdStatsDao::addToStats(OLFO *oracle, const char *terminalId, const char *body)
{
	
	char imei[16];
	char uim[16];
	splitTerminalIds(terminalId,imei,uim);
	//解析数据,然后放入数据库中
	char hotelId[10+1];
	char msisdn[10+1];

	
	//1.找到酒店ID,MSISDN
	if(0>(oracle->ExecSQL(
		"select hotel_id,msisdn from T_Terminal_Info where imei_number=:s1 and uim_number=:s2",
		imei,uim,hotelId,msisdn)))
	{		
		log(__FILE__,__LINE__,"error info: %s\n",oracle->getErrorInfo());
		return -1;
	}
	//2.入库

	AdStatsBody adsbody;
	adsbody.parse(body);
	for(int i=0;i<3;i++){
		if(adsbody.next()==NULL){
			return -2;
		}
		//忽略掉前面三个值
	}
	char sql[1024]={0};
	while(true){
		char *p1=adsbody.next();
		if(p1==NULL)return 0;//第一个也没有，是可以的
		char *p2=adsbody.next();
		if(p2==NULL)return -4;//有第一个,没有第二个,是不行的
		char *p3=adsbody.next();
		if(p3==NULL)return -5;//有第二个,没有第三个,是不行的
		//取到了3个都不为空的
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

create table 	t_access_log		--向CMPP网关发送数据回包和状态报告的对应表
(
	id				number(9),		--一个惟一的序号
	imei_number		varchar2(15),	--终端的15位IMEI号
	msisdn			varchar2(11),	--手机号码
	content_prefix	char(4),		--内容类型现在可以为menu,file
	language		char(2),		--两位的语言代码,具体请参考协议
	content_path	varchar(256),	--内容路径
	click_count		number(8),		--点击次数
	hotel_id		varchar2(10),	--酒店Id
	create_time		date			--创建时间
);

*/
