// ommand.h: interface for the Command class.
//
//////////////////////////////////////////////////////////////////////

#include <dlfcn.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/ipc.h>
#include "Common.h"
#include "CcssCore.h"
#include "/opt/work/src/CCSS_Common.h"


static char message[1024];//全局消息,用于监控系统内部状态, 用于测试等


/**********************************************************************
 函数名称: getTempMessageBuffer
 函数功能: 取得内容的消息缓冲,用于监控
 输出参数:
 返 回 值: 0-执行成功
 全局变量:
 编写作者: zhangxilai
 编写日期: 2008年10月13日
 变更日志
**********************************************************************/
char *getMessageBufferExpr()
{
	return message;
}
static void log(const char *function,int line,const char *format,...)
{
	printf("%s:%d: ",function,line);
	va_list args;
    va_start(args, format);
    vprintf(format,args);
    va_end(args);
}
static int checkTableSchemaDefinition(OLFO  *oracle)
{
	
	const char *sqls[]={
"select id,message_id,src_number,dest_number,content,priority,start_time,expire_time,return_report,app,task_id,msg_send_resp,msg_dlv_resp,status,create_time,modify_time from comm_msg_task where 1=0",
	NULL
	};
	const char **p=sqls;
	while(*p!=NULL){
		log(__FUNCTION__,__LINE__,"检查表: '%s': \n",*p);		
		if(0>oracle->ExecSQL((char*)*p)){
			log(__FUNCTION__,__LINE__,"检查表结构失败,%s, 请仔细检查表定义\n",oracle->getErrorInfo());	
			exit(-100);
			return -1;
		}
		//log(__FUNCTION__,__LINE__,"...完成\n");
		p++;
	}
	return 0;	
}
void checkEnv(OLFO  *oracle)
{
	checkTableSchemaDefinition(oracle);	
}

static int checkOracleResultsetError(int ret,OLFO  *oracle)
{
	if(ret>=0){
		return ret;
	}
	oracle->Close();
	oracle->DisConnect();
	log(__FUNCTION__,__LINE__,"检查到fetch后的oracle的状态不对,退出 %d=%s\n",ret,oracle->getErrorInfo());
	funLogP(LOG_TERMINAL_POSITION,__FILE__,__LINE__,5,"检查到fetch后的oracle的状态不对,退出 %d=%s\n",ret,oracle->getErrorInfo());
	exit(-100);
	return ret;
}

//src_number,dest_number,content,priority,return_report,expire_time 


#define STATE_UPDATE_SQL_TMPL "update comm_msg_task set status='%u',message_id=%d where id=%u"

static int updateMessageStatus(OLFO  *oracle,int id,int messagId,int status)
{
	if(status<0){
		return -1;		
	}
	if(status>4){
		return -1;		
	}
	
	int ret=0;
	char sql[1024]={0};
	sprintf(sql,STATE_UPDATE_SQL_TMPL,status,messagId,id);
	if(0>(ret=oracle->ExecSQL(sql))){
		log(__FUNCTION__,__LINE__,"发生内部错误%d=%s: %s\n",ret,sql,oracle->getErrorInfo());
		funLogP(LOG_TERMINAL_POSITION,__FILE__,__LINE__,5,"发生内部错误%d=%s: %s\n",ret,sql,oracle->getErrorInfo());
		return ret;
	}
	oracle->Commit();
	return 0;
}

static int sendShortMessage(OLFO  *oracle)
{
	//外呼结束的voice_deal_flag不要被选择进来，处理结束的，不要选进来
	//正在试图外呼的，不要选进来,
	//重新拨打次数已经超次了不要选进来
	
	int		id;

	//char 	serviceNumber[11+1]={0};
	char 	spNumber[30+1]={0};
	char	destNumber[11+1]={0};
	char	content[160+1]={0};
	int 	priority;
	int		returnReport;
	time_t	expireTime;
	
	oracle->getInt(1,id);
	oracle->getString(2,spNumber);
	oracle->getString(3,destNumber);
	oracle->getString(4,content);
	oracle->getInt(5,priority);
	oracle->getInt(6,returnReport);
	oracle->getDateTime(7,expireTime);
	
	sprintf(message,"%s=>%s:%s",spNumber,destNumber,content);
	
	int messageId=funGet_Buss_Func()->funSend_Message_TO_Subscriber(spNumber,destNumber,
		content,8 ,60*72,
		returnReport==0?1:2,expireTime);
	
	if(messageId<0){
		//更新为错误状态
		updateMessageStatus(oracle,	id,messageId,4);
		return 0;
	}
	updateMessageStatus(oracle,id,messageId,1);
	//更新为已经发送状态
	//send message to queue 
	return messageId;
}

/*

drop table comm_msg_task; 
--外部定位处理
create table comm_msg_task
(
	id				number(9) 	default 0,	--一个惟一的序号
	message_id		number(9) 	default 0,    --用于发送跟踪的序号，对通信层而言，就是通信任务标记
	src_number		number(21) 	default '1065717066',	--请求发送的主叫号码
	dest_number		number(11) 	default 0,	--请求发送的被叫号码	
	content			varchar2(160) default '0',	--将要发送的短信内容
	priority		number(1) 	default 9,		--0	最高优先级，9，最低优先级，目前就用0和普通优先级，没有最低就是普通
	start_time		date		default sysdate,	--容许开始的时间
	expire_time		date		default	sysdate+3,		--过期时间
	return_report	number(1) 	default 0,	--需要状态报告么	0，不需要1，	需要
	app				varchar2(8) default 'unknown',		--8个字节的应用程序标记,同一程序不同实例认为是不同程序
	task_id			number(9) 	default 0, 	--用于应用程序跟踪某个任务关联的id，对应用层
	msg_send_resp	Number(2) 	default 0,	--返回值
	msg_dlv_resp	number(3) 	default 0,	--状态报告的回复情况
	status			number(1)	default 0,	--当前指令状态0.	初始状态1.	已经发出了2.已经收到状态报告3.	错误终止
	create_time		date 		default	sysdate,		--创建时间
	modify_time		date 		default	sysdate		--修改时间
);


insert into comm_msg_task(id,dest_number,content)values(S_Terminal_Command.nextval,'13691357419','hello world');
commit;

*/
#define _RETRIEVE_MESSAGES_SQL "select \
id,to_char(src_number),to_char(dest_number),content,priority,return_report,expire_time \
from comm_msg_task \
where start_time<=sysdate and status=0"

int	sendShortMessages(OLFO  *oracle)
{
	int ret=0;
	char sql[1024]={0};
	sprintf(sql,_RETRIEVE_MESSAGES_SQL);
	if(0!=(ret=oracle->ParseSQL(sql)))
	{
		log(__FUNCTION__,__LINE__,"执行错误 ret=%d %s\n",ret,oracle->getErrorInfo());
		funLogP(LOG_TERMINAL_POSITION,__FILE__,__LINE__,5,"执行错误 ret=%d %s\n",ret,oracle->getErrorInfo());
		return ret;
	}

	if(0!=(ret=oracle->Open()))
	{
		log(__FUNCTION__,__LINE__,"执行错误 ret=%d %s\n",ret,oracle->getErrorInfo());
		funLogP(LOG_TERMINAL_POSITION,__FILE__,__LINE__,5,"执行错误 ret=%d %s\n",ret,oracle->getErrorInfo());
		return ret;
	}
	ret=0;
	while(true)
	{
		if(checkOracleResultsetError(oracle->Fetch(),oracle)==0){
			break;
		}		
		int		messageId=0;	
		if(0>(messageId=sendShortMessage(oracle)))
		{
			//发送短信失败，终止本轮循环
			oracle->Close();
			return ret;
		}
		//update status here!
		
		
		ret++;
	}
	oracle->Close();
	return ret;

}
