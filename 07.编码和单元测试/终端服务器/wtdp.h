/*
*文件名: wtdp.h,wireless telecommunication device protocol
*作  者：xiongbh@ucans.com.cn
*修  改：zhangxilai@ucans.com.cn
*版  本: 0.1 
*/

#if !defined(__WTTP)
#define __WTTP

#pragma pack(1)

#define MaxPackageSize			20480		//每个包最大大小
#define TerminalIdLength		36			//通讯包中唯一标识（卡唯一标识+终端唯一标识）

#include <time.h>

typedef struct 								//包头
{
	unsigned int	nLengh;
	unsigned int	nType;
	unsigned int	nSeqId;
}PackageHead;


typedef struct 								//系统参数包
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	unsigned int		nParmLengh;
	char				strParmValues[MaxPackageSize - sizeof(PackageHead) - TerminalIdLength - sizeof(int)];	
}ParameterModificationRequest;

typedef struct 								//系统参数包回包
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	int					nErrorCode;
}ParameterModificationResponse;

typedef struct 								//时间同步包
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	time_t				tSystemDate;
}TimeSyncRequest;

typedef struct 								//时间同步回包
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	time_t				tSystemDate;
}TimeSyncResponse;

typedef struct 								//清空黑名单包
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
}BlackListResettingRequest;

typedef struct 								//清空黑名单回包
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	int					nErrorCode;
}BlackListResettingResp;

typedef struct 								//更新黑名单包
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	unsigned int		nBlackListVersion;
	unsigned int		nBlackListLengh;
	char				strBlackList[MaxPackageSize - sizeof(PackageHead) - TerminalIdLength - sizeof(int)*2];
}BlackListModificationRequest;

typedef struct 								//更新黑名单回包
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	int					nErrorCode;
}BlackListModificationResponse;

typedef struct 								//清空白名单包
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
}WhiteListResettingRequest;

typedef struct 								//清空白名单回包
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	int					nErrorCode;
}WhiteListResettingResponse;

typedef struct 								//更新白名单包
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	unsigned int		nWhiteListVersion;
	unsigned int		nWhiteListLengh;
	char				strWhiteList[MaxPackageSize - sizeof(PackageHead) - TerminalIdLength - sizeof(int)*2];
}WhiteListModifycationRequest;

typedef struct 								//更新白名单回包
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	int					nErrorCode;
}WhiteListModifycationResponse;

typedef struct 								//广告推送包
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	unsigned char		cEchoType;
	unsigned char		cSaveType;
	unsigned int		nURLLengh;
	char				strURL[MaxPackageSize - sizeof(PackageHead) - TerminalIdLength - 2 - sizeof(int)];
}AdvertisingPushRequest;

typedef struct 								//广告推送包回包
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	int					nErrorCode;
}AdvertisingPushResponse;

typedef struct 								//请求断开包
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
}DisconnectingRequest;

typedef struct 								//请求断开包回包
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
}DisconnectingResponse;

typedef struct 								//状态同步包
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	unsigned int		nStatus;
	unsigned int		nBlackListVersion;
	unsigned int		nWhiteListVersion;
	unsigned int		nSelectedLanguage;
}StatusSyncRequest;
											//状态同步回包无法定义，只能硬拆
typedef struct 								//用户请求开通包
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	time_t				tTermDate;
}ServiceStartRequest;

typedef struct 								//用户请求开通包回包
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	int					nErrorCode;
}ServiceStartResponse;

typedef struct 								//用户退租包
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
}ServiceEndRequest;

typedef struct 								//用户退租包回包
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	int					nErrorCode;
	time_t				nServiceStartDate;
	time_t				nServiceEndDate;
}ServiceEndResponse;

typedef struct 								//心跳包
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	time_t				tTerminalDate;
}HeartBeatingRequest;

typedef struct 								//心跳包回包
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	unsigned int		nStatus;
	time_t				tSystemDate;
}HeartBeatingResponse;

typedef struct 								//发送统计数据包
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	unsigned int		nStatisticsBodyLengh;
	char				strStatisticsBody[MaxPackageSize - sizeof(PackageHead) - TerminalIdLength - sizeof(int)];
}StatisticsDeliverRequest;

typedef struct 								//发送统计数据包回包
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	int					nErrorCode;
}StatisticsDeliverResponse;

/*话费充值存在变数，未最后定义*/
typedef struct 								//话费充值包
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	unsigned int		nSelectedType;
}ChargingRequest;

typedef struct 								//话费充值包回包
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	int					nErrorCode;
}ChargingResponse;

#endif // !defined(__WTTP)

