/*
*�ļ���: wtdp.h,wireless telecommunication device protocol
*��  �ߣ�xiongbh@ucans.com.cn
*��  �ģ�zhangxilai@ucans.com.cn
*��  ��: 0.1 
*/

#if !defined(__WTTP)
#define __WTTP

#pragma pack(1)

#define MaxPackageSize			20480		//ÿ��������С
#define TerminalIdLength		36			//ͨѶ����Ψһ��ʶ����Ψһ��ʶ+�ն�Ψһ��ʶ��

#include <time.h>

typedef struct 								//��ͷ
{
	unsigned int	nLengh;
	unsigned int	nType;
	unsigned int	nSeqId;
}PackageHead;


typedef struct 								//ϵͳ������
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	unsigned int		nParmLengh;
	char				strParmValues[MaxPackageSize - sizeof(PackageHead) - TerminalIdLength - sizeof(int)];	
}ParameterModificationRequest;

typedef struct 								//ϵͳ�������ذ�
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	int					nErrorCode;
}ParameterModificationResponse;

typedef struct 								//ʱ��ͬ����
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	time_t				tSystemDate;
}TimeSyncRequest;

typedef struct 								//ʱ��ͬ���ذ�
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	time_t				tSystemDate;
}TimeSyncResponse;

typedef struct 								//��պ�������
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
}BlackListResettingRequest;

typedef struct 								//��պ������ذ�
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	int					nErrorCode;
}BlackListResettingResp;

typedef struct 								//���º�������
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	unsigned int		nBlackListVersion;
	unsigned int		nBlackListLengh;
	char				strBlackList[MaxPackageSize - sizeof(PackageHead) - TerminalIdLength - sizeof(int)*2];
}BlackListModificationRequest;

typedef struct 								//���º������ذ�
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	int					nErrorCode;
}BlackListModificationResponse;

typedef struct 								//��հ�������
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
}WhiteListResettingRequest;

typedef struct 								//��հ������ذ�
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	int					nErrorCode;
}WhiteListResettingResponse;

typedef struct 								//���°�������
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	unsigned int		nWhiteListVersion;
	unsigned int		nWhiteListLengh;
	char				strWhiteList[MaxPackageSize - sizeof(PackageHead) - TerminalIdLength - sizeof(int)*2];
}WhiteListModifycationRequest;

typedef struct 								//���°������ذ�
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	int					nErrorCode;
}WhiteListModifycationResponse;

typedef struct 								//������Ͱ�
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	unsigned char		cEchoType;
	unsigned char		cSaveType;
	unsigned int		nURLLengh;
	char				strURL[MaxPackageSize - sizeof(PackageHead) - TerminalIdLength - 2 - sizeof(int)];
}AdvertisingPushRequest;

typedef struct 								//������Ͱ��ذ�
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	int					nErrorCode;
}AdvertisingPushResponse;

typedef struct 								//����Ͽ���
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
}DisconnectingRequest;

typedef struct 								//����Ͽ����ذ�
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
}DisconnectingResponse;

typedef struct 								//״̬ͬ����
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	unsigned int		nStatus;
	unsigned int		nBlackListVersion;
	unsigned int		nWhiteListVersion;
	unsigned int		nSelectedLanguage;
}StatusSyncRequest;
											//״̬ͬ���ذ��޷����壬ֻ��Ӳ��
typedef struct 								//�û�����ͨ��
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	time_t				tTermDate;
}ServiceStartRequest;

typedef struct 								//�û�����ͨ���ذ�
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	int					nErrorCode;
}ServiceStartResponse;

typedef struct 								//�û������
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
}ServiceEndRequest;

typedef struct 								//�û�������ذ�
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	int					nErrorCode;
	time_t				nServiceStartDate;
	time_t				nServiceEndDate;
}ServiceEndResponse;

typedef struct 								//������
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	time_t				tTerminalDate;
}HeartBeatingRequest;

typedef struct 								//�������ذ�
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	unsigned int		nStatus;
	time_t				tSystemDate;
}HeartBeatingResponse;

typedef struct 								//����ͳ�����ݰ�
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	unsigned int		nStatisticsBodyLengh;
	char				strStatisticsBody[MaxPackageSize - sizeof(PackageHead) - TerminalIdLength - sizeof(int)];
}StatisticsDeliverRequest;

typedef struct 								//����ͳ�����ݰ��ذ�
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	int					nErrorCode;
}StatisticsDeliverResponse;

/*���ѳ�ֵ���ڱ�����δ�����*/
typedef struct 								//���ѳ�ֵ��
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	unsigned int		nSelectedType;
}ChargingRequest;

typedef struct 								//���ѳ�ֵ���ذ�
{
	PackageHead			suPackHead;
	char				strTerminalId[TerminalIdLength];
	int					nErrorCode;
}ChargingResponse;

#endif // !defined(__WTTP)

