#pragma pack(1)
#define BLACK_NUM_MAX					30		//终端可以容纳的黑名单最大个数
#define WHITE_NUM_MAX					10		//终端可以容纳的白名单最大个数
#define BLACK_WHITE_PHONE_LEN			15		//单个黑白名单号码最大字节数
#define PRE_VALID_MAX_MINUTES			10		//预开通的最大有效时间(分钟)


#define	TERMINAL_NUMBER_LENGTH		36		//终端惟一标识长度(含末尾的0)
#define	UIM_NUMBER_LENGTH			15		//终端UIM卡号长度(固定的)
#define	IMEI_NUMBER_LENGTH			15		//终端IMEI码长度(固定的)
#define	PACKAGE_LENTGH_MAX			512		//与终端交互包的包体最大长度
#define	HOTEL_ID_LENGTH_MAX			10		//酒店ID最大长度

#define	HOTEL_ID_IS_NULL			2		//酒店ID为空，也就是说，终端没有分发到酒店
#define	HOTEL_STATUS_IS_INVALID		3		//酒店状态异常，比如，已经注销


#define	TERMINAL_ALREADY_OPENED		0X1003	//终端发起开通请求时，其状态已经是开通状态
#define	TERMINAL_NOT_OPENED			0X1021	//终端发起退租请求时，其状态不是开通状态

#define	TERMINAL_ERROR				0X2000	//处理终端发起的指令时，一个通用错误代码


typedef struct 			//统一的包结构定义
{
	unsigned int	nLengh;					//包的长度
	unsigned int	nCommand;				//命令字
	unsigned int	nSequence_ID;			//包的流水号
	unsigned char	strTerminal_Number[TERMINAL_NUMBER_LENGTH];	//终端惟一标识(注意：这里没有加一)
	unsigned char	strPackage_Body[PACKAGE_LENTGH_MAX];	//包体
}	stTerminal_Package;

typedef struct 			//终端属性(所属酒店、状态)
{
	char	strImei_Number[IMEI_NUMBER_LENGTH+1];	//终端IMEI码
	char	strUim_Number[UIM_NUMBER_LENGTH+1];		//终端UIM卡号
	char	strHotel_ID[HOTEL_ID_LENGTH_MAX+1];		//酒店ID
	char	strHotel_Status[1+1];					//酒店状态
	char	strMsisdn[11+1];						//终端手机号码
	int		nTerminal_Status;						//终端当前状态
	char	strCurrent_Hire_Time[14+1];				//终端服务开通时间(状态为开通时有效)
	int		nBlack_Version;							//给终端下行的黑名单版本号
	int		nWhite_Version;							//给终端下行的白名单版本号
	char	strTerminal_Parm_Time[14+1];			//给终端下行的终端参数修改时间
}	stTerminal_Attri;

#define TERMINAL_DISCONNECT						0X00000000		//终端请求断开
#define TERMINAL_DISCONNECT_RESP				0X00000100		//终端请求断开回包
#define TERMINAL_GET_STATUS						0X00000001		//终端状态同步
#define TERMINAL_GET_STATUS_RESP				0X00000101		//终端状态同步回包
#define TERMINAL_OPEN							0X00000002		//终端发起开通
#define TERMINAL_OPEN_RESP						0X00000102		//终端发起开通回包
#define TERMINAL_QUIT							0X00000003		//终端发起退租
#define TERMINAL_QUIT_RESP						0X00000103		//终端发起退租回包
#define TERMINAL_HEARTBEAT						0X00000004		//终端心跳包
#define TERMINAL_HEARTBEAT_RESP					0X00000104		//终端心跳回包
#define TERMINAL_STATISTICS						0X00000005		//终端发送统计数据
#define TERMINAL_STATISTICS_RESP				0X00000105		//终端发送统计数据回包
#define TERMINAL_FEE							0X00000006		//话费充值(一期不做)
#define TERMINAL_FEE_RESP						0X00000106		//话费充值回包
#define TERMINAL_IPFEE							0X00000007		//IP充值(一期不做)
#define TERMINAL_IPFEE_RESP						0X00000107		//IP充值回包
#define TERMINAL_GET_TIME						0X00000012		//终端时间同步
#define TERMINAL_GET_TIME_RESP					0X00000112		//终端时间同步回包

#define PLATFORM_MODI_PARAMETER					0X00000011		//修改系统参数
#define PLATFORM_MODI_PARAMETER_RESP			0X00000111		//修改系统参数回包
#define PLATFORM_BLACK_CLEAR					0X00000013		//清空黑名单
#define PLATFORM_BLACK_CLEAR_RESP				0X00000113		//清空黑名单回包
#define PLATFORM_BLACK_UPDATE					0X00000014		//更新黑名单
#define PLATFORM_BLACK_UPDATE_RESP				0X00000114		//更新黑名单回包
#define PLATFORM_WHITE_CLEAR					0X00000015		//清空白名单
#define PLATFORM_WHITE_CLEAR_RESP				0X00000115		//清空白名单回包
#define PLATFORM_WHITE_UPDATE					0X00000016		//更新白名单
#define PLATFORM_WHITE_UPDATE_RESP				0X00000116		//更新白名单回包
#define PLATFORM_PUSH_DVERTISEMENT				0X00000018		//广告推送包
#define PLATFORM_PUSH_DVERTISEMENT_RESP			0X00000118		//广告推送包回包

#define LANGUAGE_LIST_MAX					9		//支持的最大语言版本数
char	strArray_Language[LANGUAGE_LIST_MAX+1][2+1]={"", "ZH", "RU", "KR", "JP", "FR",
		"SP", "IT", "PG", "EN"};

/**********************************************************************
 函数名称: LogP
 函数功能: 记录日志函数
 输入参数: strFile:程序文件名; nLine:行数; Error_Level:错误级别; strFormat:打印格式
 输出参数:
 返 回 值:
 全局变量:
 编写作者: mahezhou
 编写日期: 2008年10月17日
 修改作者:
 修改日期:
 修改目的:
**********************************************************************/
int funLogP(int nLog_File, char *strFile, int nLine, int Error_Level, char *strFormat,...)
{
	va_list		ap;
	time_t		tv;
	struct tm	local_t;
	char		strDate[8+1];

	time(&tv);
	localtime_r(&tv, &local_t);

	sprintf(strDate,"%04d%02d%02d", local_t.tm_year+1900, local_t.tm_mon+1, local_t.tm_mday);
	char		strFile_Name[64];
	sprintf(strFile_Name, "/opt/happy/log/mahezhou_%s.log", strDate);
	FILE *fpw = fopen(strFile_Name, "a");
	fprintf(fpw,"[%d-%02d-%02d %02d:%02d:%02d] [%d] [%d] [%s] [%d]:",
			local_t.tm_year+1900, local_t.tm_mon+1, local_t.tm_mday,
			local_t.tm_hour, local_t.tm_min, local_t.tm_sec,
			Error_Level, getpid(), strFile, nLine);
	fflush(fpw);

	va_start(ap, strFormat);
	vfprintf(fpw, strFormat, ap);
	va_end(ap);
	fclose(fpw);
	return 0;
}

//取得系统时间的前n位
char *funGetCurrentTime(char *strDestTime, int nSize)
{
	time_t ssuTime_T;
	struct tm ssuTm;
	char	strTemp[32];

	time(&ssuTime_T);
	localtime_r(&ssuTime_T, &ssuTm);
	sprintf(strTemp, "%04d%02d%02d%02d%02d%02d", ssuTm.tm_year+1900,
					ssuTm.tm_mon+1, ssuTm.tm_mday,
					ssuTm.tm_hour, ssuTm.tm_min, ssuTm.tm_sec);
	if (nSize > 14)
	{
		strTemp[14] = 0;
	}
	else
	{
		strTemp[nSize] = 0;
	}
	strcpy(strDestTime, strTemp);
	return strDestTime;
}

//取得系统时间后nDays天的前nSize位
char *funGetCurrentTime(char *strDestTime, int nDays, int nSize)
{
	time_t ssuTime_T;
	struct tm ssuTm;
	char	strTemp[32];

	time(&ssuTime_T);
	ssuTime_T = ssuTime_T + nDays*24*60*60;
	localtime_r(&ssuTime_T, &ssuTm);
	sprintf(strTemp, "%04d%02d%02d%02d%02d%02d", ssuTm.tm_year+1900,
					 ssuTm.tm_mon+1, ssuTm.tm_mday,
					 ssuTm.tm_hour, ssuTm.tm_min, ssuTm.tm_sec);
	if (nSize > 14)
	{
		strTemp[14] = 0;
	}
	else
	{
		strTemp[nSize] = 0;
	}
	strcpy(strDestTime, strTemp);
	return strDestTime;
}

/*把字符串转换成time_t结构
  输入：1. 时间字符串，14位
  输出：1. time_t		*/
time_t funString_To_Time_T(const char *strTime)
{
	char strTemp[8];
	struct	tm ssuTm;

	memcpy (strTemp, strTime, 4);
	strTemp[4] = 0;
	ssuTm.tm_year = atoi(strTemp) - 1900;

	memcpy ( strTemp , &strTime[4] , 2 );
	strTemp[2] = 0;
	ssuTm.tm_mon = atoi ( strTemp ) - 1;

	memcpy ( strTemp , &strTime[6] , 2 );
	strTemp[2] = 0;
	ssuTm.tm_mday = atoi(strTemp);

	memcpy(strTemp, &strTime[8], 2 );
	strTemp[2] = 0;
	ssuTm.tm_hour = atoi(strTemp);

	memcpy(strTemp, &strTime[10], 2);
	strTemp[2] = 0;
	ssuTm.tm_min = atoi(strTemp);

	memcpy(strTemp, &strTime[12], 2);
	strTemp[2] = 0;
	ssuTm.tm_sec = atoi(strTemp);

	return mktime(&ssuTm);
}

typedef struct 			//终端状态同步包结构
{
	unsigned int	nTerminal_Status;		//终端的状态
	unsigned int	nTerminal_Time;			//终端当前时间
	unsigned int	nBlack_Version;			//黑名单版本号
	unsigned int	nWhite_Version;			//白名单版本号
	unsigned int	nLanguage_Version;		//用户选择的语言版本
}	stTerminal_Get_Status;

