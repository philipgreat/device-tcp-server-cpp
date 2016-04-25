#pragma pack(1)
#define BLACK_NUM_MAX					30		//�ն˿������ɵĺ�����������
#define WHITE_NUM_MAX					10		//�ն˿������ɵİ�����������
#define BLACK_WHITE_PHONE_LEN			15		//�����ڰ�������������ֽ���
#define PRE_VALID_MAX_MINUTES			10		//Ԥ��ͨ�������Чʱ��(����)


#define	TERMINAL_NUMBER_LENGTH		36		//�ն�Ωһ��ʶ����(��ĩβ��0)
#define	UIM_NUMBER_LENGTH			15		//�ն�UIM���ų���(�̶���)
#define	IMEI_NUMBER_LENGTH			15		//�ն�IMEI�볤��(�̶���)
#define	PACKAGE_LENTGH_MAX			512		//���ն˽������İ�����󳤶�
#define	HOTEL_ID_LENGTH_MAX			10		//�Ƶ�ID��󳤶�

#define	HOTEL_ID_IS_NULL			2		//�Ƶ�IDΪ�գ�Ҳ����˵���ն�û�зַ����Ƶ�
#define	HOTEL_STATUS_IS_INVALID		3		//�Ƶ�״̬�쳣�����磬�Ѿ�ע��


#define	TERMINAL_ALREADY_OPENED		0X1003	//�ն˷���ͨ����ʱ����״̬�Ѿ��ǿ�ͨ״̬
#define	TERMINAL_NOT_OPENED			0X1021	//�ն˷�����������ʱ����״̬���ǿ�ͨ״̬

#define	TERMINAL_ERROR				0X2000	//�����ն˷����ָ��ʱ��һ��ͨ�ô������


typedef struct 			//ͳһ�İ��ṹ����
{
	unsigned int	nLengh;					//���ĳ���
	unsigned int	nCommand;				//������
	unsigned int	nSequence_ID;			//������ˮ��
	unsigned char	strTerminal_Number[TERMINAL_NUMBER_LENGTH];	//�ն�Ωһ��ʶ(ע�⣺����û�м�һ)
	unsigned char	strPackage_Body[PACKAGE_LENTGH_MAX];	//����
}	stTerminal_Package;

typedef struct 			//�ն�����(�����Ƶꡢ״̬)
{
	char	strImei_Number[IMEI_NUMBER_LENGTH+1];	//�ն�IMEI��
	char	strUim_Number[UIM_NUMBER_LENGTH+1];		//�ն�UIM����
	char	strHotel_ID[HOTEL_ID_LENGTH_MAX+1];		//�Ƶ�ID
	char	strHotel_Status[1+1];					//�Ƶ�״̬
	char	strMsisdn[11+1];						//�ն��ֻ�����
	int		nTerminal_Status;						//�ն˵�ǰ״̬
	char	strCurrent_Hire_Time[14+1];				//�ն˷���ͨʱ��(״̬Ϊ��ͨʱ��Ч)
	int		nBlack_Version;							//���ն����еĺ������汾��
	int		nWhite_Version;							//���ն����еİ������汾��
	char	strTerminal_Parm_Time[14+1];			//���ն����е��ն˲����޸�ʱ��
}	stTerminal_Attri;

#define TERMINAL_DISCONNECT						0X00000000		//�ն�����Ͽ�
#define TERMINAL_DISCONNECT_RESP				0X00000100		//�ն�����Ͽ��ذ�
#define TERMINAL_GET_STATUS						0X00000001		//�ն�״̬ͬ��
#define TERMINAL_GET_STATUS_RESP				0X00000101		//�ն�״̬ͬ���ذ�
#define TERMINAL_OPEN							0X00000002		//�ն˷���ͨ
#define TERMINAL_OPEN_RESP						0X00000102		//�ն˷���ͨ�ذ�
#define TERMINAL_QUIT							0X00000003		//�ն˷�������
#define TERMINAL_QUIT_RESP						0X00000103		//�ն˷�������ذ�
#define TERMINAL_HEARTBEAT						0X00000004		//�ն�������
#define TERMINAL_HEARTBEAT_RESP					0X00000104		//�ն������ذ�
#define TERMINAL_STATISTICS						0X00000005		//�ն˷���ͳ������
#define TERMINAL_STATISTICS_RESP				0X00000105		//�ն˷���ͳ�����ݻذ�
#define TERMINAL_FEE							0X00000006		//���ѳ�ֵ(һ�ڲ���)
#define TERMINAL_FEE_RESP						0X00000106		//���ѳ�ֵ�ذ�
#define TERMINAL_IPFEE							0X00000007		//IP��ֵ(һ�ڲ���)
#define TERMINAL_IPFEE_RESP						0X00000107		//IP��ֵ�ذ�
#define TERMINAL_GET_TIME						0X00000012		//�ն�ʱ��ͬ��
#define TERMINAL_GET_TIME_RESP					0X00000112		//�ն�ʱ��ͬ���ذ�

#define PLATFORM_MODI_PARAMETER					0X00000011		//�޸�ϵͳ����
#define PLATFORM_MODI_PARAMETER_RESP			0X00000111		//�޸�ϵͳ�����ذ�
#define PLATFORM_BLACK_CLEAR					0X00000013		//��պ�����
#define PLATFORM_BLACK_CLEAR_RESP				0X00000113		//��պ������ذ�
#define PLATFORM_BLACK_UPDATE					0X00000014		//���º�����
#define PLATFORM_BLACK_UPDATE_RESP				0X00000114		//���º������ذ�
#define PLATFORM_WHITE_CLEAR					0X00000015		//��հ�����
#define PLATFORM_WHITE_CLEAR_RESP				0X00000115		//��հ������ذ�
#define PLATFORM_WHITE_UPDATE					0X00000016		//���°�����
#define PLATFORM_WHITE_UPDATE_RESP				0X00000116		//���°������ذ�
#define PLATFORM_PUSH_DVERTISEMENT				0X00000018		//������Ͱ�
#define PLATFORM_PUSH_DVERTISEMENT_RESP			0X00000118		//������Ͱ��ذ�

#define LANGUAGE_LIST_MAX					9		//֧�ֵ�������԰汾��
char	strArray_Language[LANGUAGE_LIST_MAX+1][2+1]={"", "ZH", "RU", "KR", "JP", "FR",
		"SP", "IT", "PG", "EN"};

/**********************************************************************
 ��������: LogP
 ��������: ��¼��־����
 �������: strFile:�����ļ���; nLine:����; Error_Level:���󼶱�; strFormat:��ӡ��ʽ
 �������:
 �� �� ֵ:
 ȫ�ֱ���:
 ��д����: mahezhou
 ��д����: 2008��10��17��
 �޸�����:
 �޸�����:
 �޸�Ŀ��:
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

//ȡ��ϵͳʱ���ǰnλ
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

//ȡ��ϵͳʱ���nDays���ǰnSizeλ
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

/*���ַ���ת����time_t�ṹ
  ���룺1. ʱ���ַ�����14λ
  �����1. time_t		*/
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

typedef struct 			//�ն�״̬ͬ�����ṹ
{
	unsigned int	nTerminal_Status;		//�ն˵�״̬
	unsigned int	nTerminal_Time;			//�ն˵�ǰʱ��
	unsigned int	nBlack_Version;			//�������汾��
	unsigned int	nWhite_Version;			//�������汾��
	unsigned int	nLanguage_Version;		//�û�ѡ������԰汾
}	stTerminal_Get_Status;

