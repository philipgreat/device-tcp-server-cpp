// WtdpPacket.cpp: implementation of the WtdpPacket class.
//
//////////////////////////////////////////////////////////////////////

#include "WtdpPacket.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "wtdp.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
static int sequence=0;
WtdpPacket::WtdpPacket()
{

}

WtdpPacket::~WtdpPacket()
{

}
int WtdpPacket::getLength()
{
	return ((PackageHead*)this->packet)->nLengh;
}
int WtdpPacket::getCommand()
{
	return ((PackageHead*)this->packet)->nType;
}
int WtdpPacket::getSequence()
{
	return ((PackageHead*)this->packet)->nSeqId;
}

void * WtdpPacket::getBody()
{
	return (void*) (this->packet+12);
}
int WtdpPacket::getErrorCode()
{
	return *((int*)(this->packet+12+36));
}
const char * WtdpPacket::getErorInfo()
{
	int errorCode=*((int*)(this->packet+12+36));

	if(errorCode==0){
		return "完全正确";
	}

	if(errorCode==0x9000){
		return "终端内部错误";
	}
	if(errorCode<0x1000){
		return "未知的错误-";
	}
	if(errorCode>0x1030){
		return "未知的错误+";
	}
	
	const char *errorMessages[]={
	"命令格式错误",
"终端不识别的指令",
"不允许操作，不信任的平台",
"终端状态错误，已经是开通状态",
"已经是复位状态",
"黑名单已经清空",
"白名单已经清空",
"黑名单数量超限",
"白名单数量超限",
"SD卡上文件非法",
"命令中包含非法电话号码",
"流水号错误",
"时间格式错误",
"文件内容校验非法",
"无法识别的参数",
"不合法的参数值",
"不识别的语言代码",
"唯一标识在系统中不存在",
"系统主动拒绝了终端的登录",
"系统有致命错误而拒绝了终端登录",
"唯一标识在系统中不存在",
"系统从没收到过终端的开通请求",
"系统有致命错误而拒绝了终端退租",
"用户选择的档次没有定义",
"充值接口目前不可用",
"系统主动拒绝了这台终端的充值",
"系统内部错误导致充值失败",
"用户选择的档次没有定义",
"充值接口目前不可用",
"系统主动拒绝了这台终端的充值",
"系统内部错误导致充值失败"	
	};

	if(errorCode>=0x1000&&errorCode<=0x1009){
		return errorMessages[errorCode-0x1000];
	}
	if(errorCode>=0x1010&&errorCode<=0x1019){
		return errorMessages[errorCode-0x1010+10];
	}
	if(errorCode>=0x1020&&errorCode<=0x1029){
		return errorMessages[errorCode-0x1020+20];
	}
	if(errorCode>=0x1030){
		return errorMessages[errorCode-0x1030+30];
	}

	return "未知的错误，走到空空头了！";
}

WtdpPacket * WtdpPacket::packServiceStartReq(const char *terminalId)
{
	if(strlen(terminalId)>35){
		return NULL;
	}
	ServiceStartRequest *req=(ServiceStartRequest*)this->packet;
	req->suPackHead.nLengh=sizeof(ServiceStartRequest);
	req->suPackHead.nType=0x002;
	req->suPackHead.nSeqId=++sequence;	
	strcpy(req->strTerminalId,terminalId);
	time_t tm;
	time(&tm);
	req->tTermDate=tm;	
	return this;
}

WtdpPacket * WtdpPacket::packServiceStartResp(const char *terminalId,int errorCode)
{
	if(strlen(terminalId)>35){
		return NULL;
	}
	
	ServiceStartResponse *resp=(ServiceStartResponse*)this->packet;
	resp->suPackHead.nLengh=sizeof(ServiceStartResponse);
	resp->suPackHead.nType=0x00000102;
	resp->suPackHead.nSeqId=++sequence;	
	strcpy(resp->strTerminalId,terminalId);
	resp->nErrorCode=errorCode;
	return this;	
}

WtdpPacket * WtdpPacket::packServiceEndReq(const char *terminalId,int startTime)
{
	if(strlen(terminalId)>35){
		return NULL;
	}

	ServiceEndRequest *req=(ServiceEndRequest*)this->packet;
	req->suPackHead.nLengh=sizeof(ServiceEndRequest);
	req->suPackHead.nType=0x003;
	req->suPackHead.nSeqId=++sequence;	
	strcpy(req->strTerminalId,terminalId);

	
	return this;	
}

WtdpPacket * WtdpPacket::packServiceEndResp(const char *terminalId,int errorCode,int startTime,int endTime)
{
	if(strlen(terminalId)>35){
		return NULL;
	}
	
	ServiceEndResponse *resp=(ServiceEndResponse*)this->packet;
	resp->suPackHead.nLengh=sizeof(ServiceEndResponse);
	resp->suPackHead.nType=0x103;
	resp->suPackHead.nSeqId=++sequence;	
	strcpy(resp->strTerminalId,terminalId);

	resp->nErrorCode=errorCode;
	resp->nServiceStartDate=startTime;
	resp->nServiceEndDate=endTime;
	return this;	
}

/*

命令名称	命令编码	回复命令编码
状态同步	0x00000001	0x00000101
用户开通	0x00000002	0x00000102
用户退租	0x00000003	0x00000103
心跳	0x00000004	0x00000104
发送统计数据	0x00000005	0x00000105
话费充值	0x00000006	0x00000106
IP充值	0x00000007	0x00000107
修改系统参数	0x00000011	0x00000111
时间同步	0x00000012	0x00000112
清空黑名单	0x00000013	0x00000113
增加黑名单	0x00000014	0x00000114
清空白名单	0x00000015	0x00000115
增加白名单	0x00000016	0x00000116
命令完成包	0x00000017	0x00000117
广告推送包	0x00000018	0x00000118

*/

WtdpPacket * WtdpPacket::packHeartBeatingReq(const char *terminalId)
{
	if(strlen(terminalId)>35){
		return NULL;
	}
	HeartBeatingRequest *req=(HeartBeatingRequest*)this->packet;
	req->suPackHead.nLengh=sizeof(HeartBeatingRequest);
	req->suPackHead.nType=0x004;
	req->suPackHead.nSeqId=++sequence;
	strcpy(req->strTerminalId,terminalId);
	time_t tm;
	time(&tm);
	req->tTerminalDate=tm;
	return this;
}

WtdpPacket * WtdpPacket::packHeartBeatingResp(const char *terminalId, int status)
{
	if(strlen(terminalId)>35){
		return NULL;
	}
	HeartBeatingResponse *resp=(HeartBeatingResponse*)this->packet;
	resp->suPackHead.nLengh=sizeof(HeartBeatingResponse);
	resp->suPackHead.nType=0x104;
	resp->suPackHead.nSeqId=++sequence;	
	strcpy(resp->strTerminalId,terminalId);
	resp->nStatus=status;
	return this;
}

WtdpPacket * WtdpPacket::packStatisticsDeliverReq(const char *terminalId, const char *content, int length)
{
	if(strlen(terminalId)>35){
		return NULL;
	}
	StatisticsDeliverRequest *req=(StatisticsDeliverRequest*)this->packet;
	req->suPackHead.nLengh=sizeof(StatisticsDeliverRequest);
	//这个是变长的
	req->suPackHead.nType=0x005;
	req->suPackHead.nSeqId=++sequence;
	strcpy(req->strTerminalId,terminalId);
	req->nStatisticsBodyLengh=strlen(content);
	strcpy(req->strStatisticsBody,content);
	return this;
}

WtdpPacket * WtdpPacket::packStatisticsDeliverResp(const char *terminalId, int errorCode)
{
	if(strlen(terminalId)>35){
		return NULL;
	}
	StatisticsDeliverResponse *resp=(StatisticsDeliverResponse*)this->packet;
	resp->suPackHead.nLengh=sizeof(StatisticsDeliverResponse);
	resp->suPackHead.nType=0x00000105;
	resp->suPackHead.nSeqId=++sequence;	
	strcpy(resp->strTerminalId,terminalId);
	resp->nErrorCode=errorCode;

	return this;
}

WtdpPacket * WtdpPacket::packStatusSyncReq(const char *terminalId, int status, int blacklistVersion, int whitelistVersion, int selectedLang)
{
	if(strlen(terminalId)>35){
		return NULL;
	}

	StatusSyncRequest *req=(StatusSyncRequest*)this->packet;
	req->suPackHead.nLengh=sizeof(StatusSyncRequest);
	req->suPackHead.nType=0x00000001;
	req->suPackHead.nSeqId=++sequence;
	strcpy(req->strTerminalId,terminalId);
	req->nStatus=status;
	req->nBlackListVersion=blacklistVersion;
	req->nWhiteListVersion=whitelistVersion;
	req->nSelectedLanguage=selectedLang;
	return this;
}


WtdpPacket * WtdpPacket::packDisconnectingReq(const char *terminalId)
{
	if(strlen(terminalId)>35){
		return NULL;
	}
	DisconnectingRequest *req=(DisconnectingRequest*)this->packet;
	req->suPackHead.nLengh=sizeof(DisconnectingRequest);
	req->suPackHead.nType=0x00000017;
	req->suPackHead.nSeqId=++sequence;
	strcpy(req->strTerminalId,terminalId);

	return this;
}

WtdpPacket * WtdpPacket::packDisconnectingResp(const char *terminalId)
{
	if(strlen(terminalId)>35){
		return NULL;
	}
	DisconnectingResponse *resp=(DisconnectingResponse*)this->packet;
	resp->suPackHead.nLengh=sizeof(DisconnectingResponse);
	resp->suPackHead.nType=0x00000117;
	resp->suPackHead.nSeqId=++sequence;	
	strcpy(resp->strTerminalId,terminalId);

	return this;
}

WtdpPacket * WtdpPacket::packParameterModificationReq(const char *terminalId, const char *params, int length)
{
	if(strlen(terminalId)>35){
		return NULL;
	}
	ParameterModificationRequest *req=(ParameterModificationRequest*)this->packet;
	req->suPackHead.nLengh=sizeof(ParameterModificationRequest);
	req->suPackHead.nType=0x00000011;
	req->suPackHead.nSeqId=++sequence;
	strcpy(req->strTerminalId,terminalId);
	strcpy(req->strParmValues,params);
	return this;
}

WtdpPacket * WtdpPacket::packParameterModificationResp(const char *terminalId, int errorCode)
{
	if(strlen(terminalId)>35){
		return NULL;
	}
	ParameterModificationResponse *resp=(ParameterModificationResponse*)this->packet;
	resp->suPackHead.nLengh=sizeof(ParameterModificationResponse);
	resp->suPackHead.nType=0x00000111;
	resp->suPackHead.nSeqId=++sequence;	
	strcpy(resp->strTerminalId,terminalId);
	resp->nErrorCode=errorCode;

	return this;
}

WtdpPacket * WtdpPacket::packTimeSyncReq(const char *terminalId)
{
	if(strlen(terminalId)>35){
		return NULL;
	}
	TimeSyncRequest *req=(TimeSyncRequest*)this->packet;
	req->suPackHead.nLengh=sizeof(TimeSyncRequest);
	req->suPackHead.nType=0x00000012;
	req->suPackHead.nSeqId=++sequence;
	strcpy(req->strTerminalId,terminalId);	
	time_t tm;
	time(&tm);
	req->tSystemDate=tm;

	return this;
}

WtdpPacket * WtdpPacket::packTimeSyncResp(const char *terminalId)
{
	if(strlen(terminalId)>35){
		return NULL;
	}
	TimeSyncResponse *resp=(TimeSyncResponse*)this->packet;
	resp->suPackHead.nLengh=sizeof(TimeSyncResponse);
	resp->suPackHead.nType=0x104;
	resp->suPackHead.nSeqId=++sequence;	
	strcpy(resp->strTerminalId,terminalId);
	time_t tm;
	time(&tm);
	resp->tSystemDate=tm;
	return this;
}

WtdpPacket * WtdpPacket::packBlackListResettingReq(const char *terminalId)
{
	if(strlen(terminalId)>35){
		return NULL;
	}
	TimeSyncRequest *req=(TimeSyncRequest*)this->packet;
	req->suPackHead.nLengh=sizeof(TimeSyncRequest);
	req->suPackHead.nType=0x00000013;
	req->suPackHead.nSeqId=++sequence;
	strcpy(req->strTerminalId,terminalId);	


	return this;
}

WtdpPacket * WtdpPacket::packBlackListResettingResp(const char *terminalId, int errorCode)
{
	if(strlen(terminalId)>35){
		return NULL;
	}
	BlackListResettingResp *resp=(BlackListResettingResp*)this->packet;
	resp->suPackHead.nLengh=sizeof(BlackListResettingResp);
	resp->suPackHead.nType=0x00000113;
	resp->suPackHead.nSeqId=++sequence;	
	strcpy(resp->strTerminalId,terminalId);
	resp->nErrorCode=errorCode;


	return this;
}

WtdpPacket * WtdpPacket::packBlackListModificationReq(const char *terminalId, int version, const char *content, int length)
{
	if(strlen(terminalId)>35){
		return NULL;
	}
	//TODO
	BlackListModificationRequest *req=(BlackListModificationRequest*)this->packet;
	req->suPackHead.nLengh=sizeof(TimeSyncRequest);
	req->suPackHead.nType=0x00000014;
	req->suPackHead.nSeqId=++sequence;
	return this;
}

WtdpPacket * WtdpPacket::packBlackListModificationResp(const char *terminalId, int errorCode)
{
	if(strlen(terminalId)>35){
		return NULL;
	}
	BlackListModificationResponse *resp=(BlackListModificationResponse*)this->packet;
	resp->suPackHead.nLengh=sizeof(BlackListModificationResponse);
	resp->suPackHead.nType=0x00000114;
	resp->suPackHead.nSeqId=++sequence;	
	strcpy(resp->strTerminalId,terminalId);
	resp->nErrorCode=errorCode;

	return this;
}

WtdpPacket * WtdpPacket::packWhiteListResettingReq(const char *terminalId)
{
	if(strlen(terminalId)>35){
		return NULL;
	}
	WhiteListResettingRequest *req=(WhiteListResettingRequest*)this->packet;
	req->suPackHead.nLengh=sizeof(WhiteListResettingRequest);
	req->suPackHead.nType=0x00000015;
	req->suPackHead.nSeqId=++sequence;
	strcpy(req->strTerminalId,terminalId);	
	return this;
}

WtdpPacket * WtdpPacket::packWhiteListResettingResp(const char *terminalId, int errorCode)
{
	if(strlen(terminalId)>35){
		return NULL;
	}
	WhiteListResettingResponse *resp=(WhiteListResettingResponse*)this->packet;
	resp->suPackHead.nLengh=sizeof(WhiteListResettingResponse);
	resp->suPackHead.nType=0x00000115;
	resp->suPackHead.nSeqId=++sequence;	
	strcpy(resp->strTerminalId,terminalId);
	resp->nErrorCode=errorCode;
	return this;
}

WtdpPacket * WtdpPacket::packWhiteListModificationReq(const char *terminalId, int version, const char *content, int length)
{
	if(strlen(terminalId)>35){
		return NULL;
	}
	WhiteListModifycationRequest *req=(WhiteListModifycationRequest*)this->packet;
	req->suPackHead.nLengh=sizeof(WhiteListModifycationRequest);
	req->suPackHead.nType=0x00000016;
	req->suPackHead.nSeqId=++sequence;
	//0x00000016
	return this;
}

WtdpPacket * WtdpPacket::packWhiteListModificationResp(const char *terminalId, int errorCode)
{
	if(strlen(terminalId)>35){
		return NULL;
	}
	WhiteListModifycationResponse *resp=(WhiteListModifycationResponse*)this->packet;
	resp->suPackHead.nLengh=sizeof(WhiteListModifycationResponse);
	resp->suPackHead.nType=0x00000116;
	resp->suPackHead.nSeqId=++sequence;	
	strcpy(resp->strTerminalId,terminalId);
	resp->nErrorCode=errorCode;

	return this;
}



char * WtdpPacket::getHead()
{
	return (char *)this->packet;
}

int WtdpPacket::getBodyLength()
{
	return this->getLength()-12;
}
