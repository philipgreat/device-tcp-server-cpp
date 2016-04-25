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
		return "��ȫ��ȷ";
	}

	if(errorCode==0x9000){
		return "�ն��ڲ�����";
	}
	if(errorCode<0x1000){
		return "δ֪�Ĵ���-";
	}
	if(errorCode>0x1030){
		return "δ֪�Ĵ���+";
	}
	
	const char *errorMessages[]={
	"�����ʽ����",
"�ն˲�ʶ���ָ��",
"����������������ε�ƽ̨",
"�ն�״̬�����Ѿ��ǿ�ͨ״̬",
"�Ѿ��Ǹ�λ״̬",
"�������Ѿ����",
"�������Ѿ����",
"��������������",
"��������������",
"SD�����ļ��Ƿ�",
"�����а����Ƿ��绰����",
"��ˮ�Ŵ���",
"ʱ���ʽ����",
"�ļ�����У��Ƿ�",
"�޷�ʶ��Ĳ���",
"���Ϸ��Ĳ���ֵ",
"��ʶ������Դ���",
"Ψһ��ʶ��ϵͳ�в�����",
"ϵͳ�����ܾ����ն˵ĵ�¼",
"ϵͳ������������ܾ����ն˵�¼",
"Ψһ��ʶ��ϵͳ�в�����",
"ϵͳ��û�յ����ն˵Ŀ�ͨ����",
"ϵͳ������������ܾ����ն�����",
"�û�ѡ��ĵ���û�ж���",
"��ֵ�ӿ�Ŀǰ������",
"ϵͳ�����ܾ�����̨�ն˵ĳ�ֵ",
"ϵͳ�ڲ������³�ֵʧ��",
"�û�ѡ��ĵ���û�ж���",
"��ֵ�ӿ�Ŀǰ������",
"ϵͳ�����ܾ�����̨�ն˵ĳ�ֵ",
"ϵͳ�ڲ������³�ֵʧ��"	
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

	return "δ֪�Ĵ����ߵ��տ�ͷ�ˣ�";
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

��������	�������	�ظ��������
״̬ͬ��	0x00000001	0x00000101
�û���ͨ	0x00000002	0x00000102
�û�����	0x00000003	0x00000103
����	0x00000004	0x00000104
����ͳ������	0x00000005	0x00000105
���ѳ�ֵ	0x00000006	0x00000106
IP��ֵ	0x00000007	0x00000107
�޸�ϵͳ����	0x00000011	0x00000111
ʱ��ͬ��	0x00000012	0x00000112
��պ�����	0x00000013	0x00000113
���Ӻ�����	0x00000014	0x00000114
��հ�����	0x00000015	0x00000115
���Ӱ�����	0x00000016	0x00000116
������ɰ�	0x00000017	0x00000117
������Ͱ�	0x00000018	0x00000118

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
	//����Ǳ䳤��
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
