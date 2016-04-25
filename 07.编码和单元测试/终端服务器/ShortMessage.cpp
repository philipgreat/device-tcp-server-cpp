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


static char message[1024];//ȫ����Ϣ,���ڼ��ϵͳ�ڲ�״̬, ���ڲ��Ե�


/**********************************************************************
 ��������: getTempMessageBuffer
 ��������: ȡ�����ݵ���Ϣ����,���ڼ��
 �������:
 �� �� ֵ: 0-ִ�гɹ�
 ȫ�ֱ���:
 ��д����: zhangxilai
 ��д����: 2008��10��13��
 �����־
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
		log(__FUNCTION__,__LINE__,"����: '%s': \n",*p);		
		if(0>oracle->ExecSQL((char*)*p)){
			log(__FUNCTION__,__LINE__,"����ṹʧ��,%s, ����ϸ������\n",oracle->getErrorInfo());	
			exit(-100);
			return -1;
		}
		//log(__FUNCTION__,__LINE__,"...���\n");
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
	log(__FUNCTION__,__LINE__,"��鵽fetch���oracle��״̬����,�˳� %d=%s\n",ret,oracle->getErrorInfo());
	funLogP(LOG_TERMINAL_POSITION,__FILE__,__LINE__,5,"��鵽fetch���oracle��״̬����,�˳� %d=%s\n",ret,oracle->getErrorInfo());
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
		log(__FUNCTION__,__LINE__,"�����ڲ�����%d=%s: %s\n",ret,sql,oracle->getErrorInfo());
		funLogP(LOG_TERMINAL_POSITION,__FILE__,__LINE__,5,"�����ڲ�����%d=%s: %s\n",ret,sql,oracle->getErrorInfo());
		return ret;
	}
	oracle->Commit();
	return 0;
}

static int sendShortMessage(OLFO  *oracle)
{
	//���������voice_deal_flag��Ҫ��ѡ���������������ģ���Ҫѡ����
	//������ͼ����ģ���Ҫѡ����,
	//���²�������Ѿ������˲�Ҫѡ����
	
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
		//����Ϊ����״̬
		updateMessageStatus(oracle,	id,messageId,4);
		return 0;
	}
	updateMessageStatus(oracle,id,messageId,1);
	//����Ϊ�Ѿ�����״̬
	//send message to queue 
	return messageId;
}

/*

drop table comm_msg_task; 
--�ⲿ��λ����
create table comm_msg_task
(
	id				number(9) 	default 0,	--һ��Ωһ�����
	message_id		number(9) 	default 0,    --���ڷ��͸��ٵ���ţ���ͨ�Ų���ԣ�����ͨ��������
	src_number		number(21) 	default '1065717066',	--�����͵����к���
	dest_number		number(11) 	default 0,	--�����͵ı��к���	
	content			varchar2(160) default '0',	--��Ҫ���͵Ķ�������
	priority		number(1) 	default 9,		--0	������ȼ���9��������ȼ���Ŀǰ����0����ͨ���ȼ���û����;�����ͨ
	start_time		date		default sysdate,	--����ʼ��ʱ��
	expire_time		date		default	sysdate+3,		--����ʱ��
	return_report	number(1) 	default 0,	--��Ҫ״̬����ô	0������Ҫ1��	��Ҫ
	app				varchar2(8) default 'unknown',		--8���ֽڵ�Ӧ�ó�����,ͬһ����ͬʵ����Ϊ�ǲ�ͬ����
	task_id			number(9) 	default 0, 	--����Ӧ�ó������ĳ�����������id����Ӧ�ò�
	msg_send_resp	Number(2) 	default 0,	--����ֵ
	msg_dlv_resp	number(3) 	default 0,	--״̬����Ļظ����
	status			number(1)	default 0,	--��ǰָ��״̬0.	��ʼ״̬1.	�Ѿ�������2.�Ѿ��յ�״̬����3.	������ֹ
	create_time		date 		default	sysdate,		--����ʱ��
	modify_time		date 		default	sysdate		--�޸�ʱ��
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
		log(__FUNCTION__,__LINE__,"ִ�д��� ret=%d %s\n",ret,oracle->getErrorInfo());
		funLogP(LOG_TERMINAL_POSITION,__FILE__,__LINE__,5,"ִ�д��� ret=%d %s\n",ret,oracle->getErrorInfo());
		return ret;
	}

	if(0!=(ret=oracle->Open()))
	{
		log(__FUNCTION__,__LINE__,"ִ�д��� ret=%d %s\n",ret,oracle->getErrorInfo());
		funLogP(LOG_TERMINAL_POSITION,__FILE__,__LINE__,5,"ִ�д��� ret=%d %s\n",ret,oracle->getErrorInfo());
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
			//���Ͷ���ʧ�ܣ���ֹ����ѭ��
			oracle->Close();
			return ret;
		}
		//update status here!
		
		
		ret++;
	}
	oracle->Close();
	return ret;

}
