// WtdpPacket.h: interface for the WtdpPacket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WTDPPACKET_H__C25D8FBC_EB99_47A4_BCBD_62635A01E153__INCLUDED_)
#define AFX_WTDPPACKET_H__C25D8FBC_EB99_47A4_BCBD_62635A01E153__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class WtdpPacket  
{
private:
	char packet[1024*16];
public:
	const char * getErorInfo();
	int getErrorCode();
	int getBodyLength();
	char * getHead();
	int getSequence();
	int getCommand();
	WtdpPacket * packWhiteListModificationResp(const char *terminalId,int errorCode);
	WtdpPacket * packWhiteListModificationReq(const char *terminalId,int version,const char *content,int length);
	WtdpPacket * packWhiteListResettingResp(const char *terminalId,int errorCode);
	WtdpPacket * packWhiteListResettingReq(const char *terminalId);
	WtdpPacket * packBlackListModificationResp(const char *terminalId,int errorCode);
	WtdpPacket * packBlackListModificationReq(const char *terminalId,int version,const char *content,int length);
	WtdpPacket * packBlackListResettingResp(const char *terminalId,int errorCode);
	WtdpPacket * packBlackListResettingReq(const char *terminalId);
	WtdpPacket * packTimeSyncResp(const char *terminalId);
	WtdpPacket * packTimeSyncReq(const char *terminalId);
	WtdpPacket * packParameterModificationResp(const char *terminalId,int errorCode);
	WtdpPacket * packParameterModificationReq(const char *terminalId,const char *params,int length);
	WtdpPacket * packDisconnectingResp(const char *terminalId);
	WtdpPacket * packDisconnectingReq(const char *terminalId);
	WtdpPacket * packStatusSyncReq(const char *terminalId,int status, int blacklistVersion,int whitelistVersion,int selectedLang);
	WtdpPacket * packStatisticsDeliverResp(const char *terminalId,int errorCode);
	WtdpPacket * packStatisticsDeliverReq(const char *terminalId,const char *content,int length);
	WtdpPacket * packHeartBeatingResp(const char *terminalId,int status);
	WtdpPacket * packHeartBeatingReq(const char *terminalId);
	void * getBody();
	int getLength();
	WtdpPacket * packServiceStartReq(const char *terminalId);
	WtdpPacket * packServiceStartResp(const char *terminalId,int errorCode);
	WtdpPacket * packServiceEndReq(const char *terminalId,int startTime);
	WtdpPacket * packServiceEndResp(const char *terminalId,int errorCode,int startTime,int endTime);

	//广告和充值的打包函数暂时不实现
	
	//StatusSyncRequest

	WtdpPacket();
	virtual ~WtdpPacket();

};

#endif // !defined(AFX_WTDPPACKET_H__C25D8FBC_EB99_47A4_BCBD_62635A01E153__INCLUDED_)
