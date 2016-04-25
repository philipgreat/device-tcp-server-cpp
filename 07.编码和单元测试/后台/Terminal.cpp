/*#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#pragma pack(1)
#include "/opt/happy/src/lib/ORACLE_OPER.H"
*/
#include "/opt/happy/src/Terminal.h"

/**********************************************************************
 ��������: funDeal_Fetch
 ��������: �����α�ִ�н����ʧ�ܡ�����ȡ���ˣ�
 �������:
	pOracle-���ݿ�����ָ��
	strFile_Name-�����ļ���
	nLine-��������
	nReturn_Fetch-fetchִ�н��
	nLog_File-Ҫд����־�ļ�
 �������:
 �� �� ֵ:
 ȫ�ֱ���:
 ��д����: mahezhou
 ��д����: 2008��11��3��
 �޸�����:
 �޸�����:
 �޸�Ŀ��:
**********************************************************************/
int funDeal_Fetch(OLFO *pOracle, char *strFile_Name, int nLine, int nReturn_Fetch, int nLog_File)
{
	if (nReturn_Fetch != 0)
	{
		funLogP(nLog_File, strFile_Name, nLine, 5,
						"ִ��Fetch()����[%d]��������Ϣ:%s\n",
						nReturn_Fetch, pOracle->getErrorInfo());
		pOracle->Close();
		return -1;
	}
	else
	{
		pOracle->Close();
		return 0;
	}
}

/**********************************************************************
 ��������: funGet_Hotel_Black_List
 ��������: ��ȡĳ���Ƶ�ĺ��������°汾�ţ����嵥
 �������:
	pOracle-���ݿ�����ָ��
	strHotel_ID-�Ƶ�ID
	nLog_File-Ҫд����־�ļ�
 �������:
	pnBlack_Version-�Ƶ�����º������汾��
	strBlack_List-�������б���ͬ����֮���԰�ǿո���(30*16=480�ֽ�)
 �� �� ֵ:
	0		�ɹ�
	1		ʧ��
 	-1		SQLִ���쳣
 ȫ�ֱ���:
 ��д����: mahezhou
 ��д����: 2009��2��11��
 �޸�����:
 �޸�����:
 �޸�Ŀ��:
**********************************************************************/
int funGet_Hotel_Black_List(OLFO *pOracle, char *strHotel_ID,
				int *pnBlack_Version, char *strBlack_List, int nLog_File)
{
	int		nReturn;
	int		nReturn_Fetch;
	int		nPhone_Number_Count;
	char	strPhone_Number[BLACK_WHITE_PHONE_LEN+1];

	//1. ȡ�Ƶ�����º������汾����
	nReturn = pOracle->ExecSQL("Select Update_Time from T_Black_White_Version where Black_White_Type='1' and Org_ID=:s1",
			strHotel_ID, (time_t *)pnBlack_Version);
	if (nReturn < 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"ִ��SQL���[Select (Update_Time-To_Date('20090101','yyyymmdd'))*24*3600 from T_Black_White_Version where Black_White_Type='1' and Org_ID='%s']����[%d]��������Ϣ:%s\n",
					strHotel_ID, nReturn, pOracle->getErrorInfo());
		return -1;
	}
	else if (nReturn == 1000)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 20,
					"ִ��SQL���[Select (Update_Time-To_Date('20090101','yyyymmdd'))*24*3600 from T_Black_White_Version where Black_White_Type='1' and Org_ID='%s']����[%d]\n",
					strHotel_ID, nReturn);
		return 1;
	}
printf("*pnBlack_Version=[%d]\n", *pnBlack_Version);

	//2. ȡ�Ƶ�����º������嵥(�����ԭ���ǣ�������еĺ�������������Լ����30������ֻȡ���¼����)
	nReturn = pOracle->ParseSQL("Select Phone_Number from T_Black_White where Black_White_Type='1' and Org_ID=:s1 order by Join_Time desc");
	if (nReturn != 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
						"ִ��ParseSQL()����[%d]��������Ϣ:%s\n",
						nReturn, pOracle->getErrorInfo());
		return -1;
	}

	if ((nReturn = pOracle->Bind(1, strHotel_ID)) != 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
						"ִ��Bind()����[%d]��������Ϣ:%s\n",
						nReturn, pOracle->getErrorInfo());
		return -1;
	}

	if ((nReturn = pOracle->Open()) != 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
						"ִ��Open()����[%d]��������Ϣ:%s\n",
						nReturn, pOracle->getErrorInfo());
		return -1;
	}

	strBlack_List[0] = 0;
	nPhone_Number_Count = 0;
	while ((nReturn_Fetch = pOracle->Fetch()) == 1)
	{
		pOracle->getString(1,	strPhone_Number);
		nPhone_Number_Count++;
		if (nPhone_Number_Count > BLACK_NUM_MAX)	//��������������Ѿ�����Լ������������׷��
		{
			funLogP(nLog_File, __FILE__, __LINE__, 10,
						"�Ƶ�[%s]�ĺ�����[%s]�Ѿ��ǵ�[%d]���ˣ�����֮\n",
						strHotel_ID, strPhone_Number, nPhone_Number_Count);
		}
		else
		{
			strcat(strBlack_List, strPhone_Number);	//�Ѻ��������븽������
			strcat(strBlack_List, " ");				//����һ���ո�
		}
	}
	strBlack_List[strlen(strBlack_List)-1] = 0;
printf("strBlack_List=[%s]\n", strBlack_List);

	return funDeal_Fetch(pOracle, __FILE__, __LINE__, nReturn_Fetch, nLog_File);
}

/**********************************************************************
 ��������: funGet_HAPPY_White_List
 ��������: ��ȡƽ̨�İ��������°汾�ţ����嵥
 �������:
	pOracle-���ݿ�����ָ��
	nLog_File-Ҫд����־�ļ�
 �������:
	pnWhite_Version-ƽ̨�����°������汾��
	strWhite_List-�������б���ͬ����֮���԰�ǿո���(30*16=480�ֽ�)
 �� �� ֵ:
	0		�ɹ�
	1		ʧ��
 	-1		SQLִ���쳣
 ȫ�ֱ���:
 ��д����: mahezhou
 ��д����: 2009��2��12��
 �޸�����:
 �޸�����:
 �޸�Ŀ��:
**********************************************************************/
int funGet_HAPPY_White_List(OLFO *pOracle, int *pnWhite_Version, char *strWhite_List, int nLog_File)
{
	int		nReturn;
	int		nReturn_Fetch;
	int		nPhone_Number_Count;
	char	strPhone_Number[BLACK_WHITE_PHONE_LEN+1];

	//1. ȡƽ̨�����°������汾����
	nReturn = pOracle->ExecSQL("Select (Update_Time-To_Date('20090101','yyyymmdd'))*24*3600 from T_Black_White_Version where Black_White_Type='0'",
				pnWhite_Version);
	if (nReturn < 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"ִ��SQL���[Select (Update_Time-To_Date('20090101','yyyymmdd'))*24*3600 from T_Black_White_Version where Black_White_Type='0']����[%d]��������Ϣ:%s\n",
					nReturn, pOracle->getErrorInfo());
		return -1;
	}
	else if (nReturn == 1000)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 20,
					"ִ��SQL���[Select (Update_Time-To_Date('20090101','yyyymmdd'))*24*3600 from T_Black_White_Version where Black_White_Type='0']����[%d]\n",
					nReturn);
		return 1;
	}

	//2. ȡƽ̨�����°������嵥
	nReturn = pOracle->ParseSQL("Select Phone_Number from T_Black_White where Black_White_Type='0' order by Join_Time desc");
	if (nReturn != 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
						"ִ��ParseSQL()����[%d]��������Ϣ:%s\n",
						nReturn, pOracle->getErrorInfo());
		return -1;
	}

	if ((nReturn = pOracle->Open()) != 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
						"ִ��Open()����[%d]��������Ϣ:%s\n",
						nReturn, pOracle->getErrorInfo());
		return -1;
	}

	strWhite_List[0] = 0;
	nPhone_Number_Count = 0;
	while ((nReturn_Fetch = pOracle->Fetch()) == 1)
	{
		pOracle->getString(1,	strPhone_Number);
		nPhone_Number_Count++;
		if (nPhone_Number_Count > WHITE_NUM_MAX)	//��������������Ѿ�����Լ������������׷��
		{
			funLogP(nLog_File, __FILE__, __LINE__, 10,
						"ƽ̨�İ�����[%s]�Ѿ��ǵ�[%d]���ˣ�����֮\n",
						strPhone_Number, nPhone_Number_Count);
		}
		else
		{
			strcat(strWhite_List, strPhone_Number);	//�Ѱ��������븽������
			strcat(strWhite_List, " ");				//����һ���ո�
		}
	}
	strWhite_List[strlen(strWhite_List)-1] = 0;
printf("strWhite_List=[%s]\n", strWhite_List);

	return funDeal_Fetch(pOracle, __FILE__, __LINE__, nReturn_Fetch, nLog_File);
}

/**********************************************************************
 ��������: funGet_Terminal_Attri
 ��������: ȡ�ն˵��ֻ����롢�����Ƶꡢ�ն�״̬��Ԥ��ͨ�����ڷ���������ͨʱ�䡢�Ƶ�״̬���ڰ������汾����������ʱ��
 �������:
	pOracle-���ݿ�����ָ��
	pTerminal_Attri-�ն����Խṹָ��
	nLog_File-Ҫд����־�ļ�
 �������:
 �� �� ֵ:
	0		�ɹ�
	1		ʧ��
 	-1		SQLִ���쳣
 ȫ�ֱ���:
 ��д����: mahezhou
 ��д����: 2009��2��12��
 �޸�����:
 �޸�����:
 �޸�Ŀ��:
**********************************************************************/
int	funGet_Terminal_Attri(OLFO *pOracle, stTerminal_Attri *pTerminal_Attri, int nLog_File)
{
	int		nReturn;
	int		nHire_Request_Time=0;

	//1. ȡ�ն˵��ֻ����롢�����Ƶꡢ�ն�״̬��Ԥ��ͨ�����ڷ���������ͨʱ��
	nReturn = pOracle->ExecSQL("Select Msisdn,Hotel_ID,Status,round((sysdate-nvl(Hire_Request_Time,sysdate))*24*60),to_char(Current_Hire_Time,'yyyymmddhh24miss'),Black_Version,White_Version,to_char(Terminal_Parm_Time,'yyyymmddhh24miss') from T_Terminal_Info where Imei_Number=:s1 and Uim_Number=:s2",
				pTerminal_Attri->strImei_Number, pTerminal_Attri->strUim_Number,
				pTerminal_Attri->strMsisdn, pTerminal_Attri->strHotel_ID,
				&(pTerminal_Attri->nTerminal_Status), &nHire_Request_Time,
				pTerminal_Attri->strCurrent_Hire_Time, &pTerminal_Attri->nBlack_Version,
				&pTerminal_Attri->nWhite_Version, pTerminal_Attri->strTerminal_Parm_Time);
	if (nReturn < 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"ִ��SQL���[Select Msisdn,Hotel_ID,Status,round((sysdate-nvl(Hire_Request_Time,sysdate))*24*60),to_char(Current_Hire_Time,'yyyymmddhh24miss'),Black_Version,White_Version,to_char(Terminal_Parm_Time,'yyyymmddhh24miss') from T_Terminal_Info where Imei_Number='%s' and Uim_Number='%s']����[%d]��������Ϣ:%s\n",
					pTerminal_Attri->strImei_Number, pTerminal_Attri->strUim_Number,
					nReturn, pOracle->getErrorInfo());
		return -1;
	}
	else if (nReturn == 1000)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 20,
					"ִ��SQL���[Select Msisdn,Hotel_ID,Status,round((sysdate-nvl(Hire_Request_Time,sysdate))*24*60),to_char(Current_Hire_Time,'yyyymmddhh24miss'),Black_Version,White_Version,to_char(Terminal_Parm_Time,'yyyymmddhh24miss') from T_Terminal_Info where Imei_Number='%s' and Uim_Number='%s']����[%d]\n",
					pTerminal_Attri->strImei_Number, pTerminal_Attri->strUim_Number,
					nReturn);
		return 1;
	}

	//2. ���Ԥ��ͨ��ʱ������Ϊδ��ͨ�����Է�Ԥ��֮ͨ��û�п�����Ȼ���ն�ת������һ���ÿͣ���һ���ÿͿ���֮��Ϳ�ͨ��
	if (nHire_Request_Time > PRE_VALID_MAX_MINUTES)
	{
		pTerminal_Attri->nTerminal_Status = 10;
	}

	//3. ȡ�Ƶ�״̬
	nReturn = pOracle->ExecSQL("Select Status from T_Org_Info where Org_ID=:s1 and Org_Type='3'",
				pTerminal_Attri->strHotel_ID, pTerminal_Attri->strHotel_Status);
	if (nReturn < 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"ִ��SQL���[Select Status from T_Org_Info where Org_ID='%s' and Org_Type='3']����[%d]��������Ϣ:%s\n",
					pTerminal_Attri->strHotel_ID, nReturn, pOracle->getErrorInfo());
		return -1;
	}
	else if (nReturn == 1000)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 10,
					"ִ��SQL���[Select Status from T_Org_Info where Org_ID='%s' and Org_Type='3']����[%d]\n",
					pTerminal_Attri->strHotel_ID, nReturn);
		return 1;
	}
printf("�ն���������:�ն�IMEI=[%s],UIM=[%s]-->\n",
	pTerminal_Attri->strImei_Number, pTerminal_Attri->strUim_Number);
printf("\t     �ֻ�����=[%s],�����Ƶ�ID=[%s],�ն˵�ǰ״̬=[%d]\n",
	pTerminal_Attri->strMsisdn, pTerminal_Attri->strHotel_ID, pTerminal_Attri->nTerminal_Status);
printf("\t     �Ƶ굱ǰ״̬=[%s],Ԥ��ͨ�����ڷ�����=[%d]\n",
	 pTerminal_Attri->strHotel_Status,nHire_Request_Time);
printf("\t     ��ͨʱ��=[%s],�������汾[%d],�������汾[%d],�ն˲�������ʱ��[%s]\n",
	 pTerminal_Attri->strCurrent_Hire_Time, pTerminal_Attri->nBlack_Version,
	 pTerminal_Attri->nWhite_Version, pTerminal_Attri->strTerminal_Parm_Time);
	return 0;
}

/**********************************************************************
 ��������: funParse_Terminal_Number
 ��������: ���ն��������ݰ��н����ն�Ωһ��ʶΪIMEI��UIM����
 �������:
	pTerminal_Package-�ն����ݰ�ָ��
	pTerminal_Attri-�ն����Խṹָ��
	nLog_File-Ҫд����־�ļ�
 �������:
 �� �� ֵ:
	0		�ɹ�
	����	ʧ��
 ȫ�ֱ���:
 ��д����: mahezhou
 ��д����: 2009��2��12��
 �޸�����:
 �޸�����:
 �޸�Ŀ��:
**********************************************************************/
int	funParse_Terminal_Number(stTerminal_Package *pTerminal_Package, stTerminal_Attri *pTerminal_Attri, int nLog_File)
{
	//1. ȡ���ն��������ݰ��е�IMEI��UIM����
	memcpy(pTerminal_Attri->strImei_Number, pTerminal_Package->strTerminal_Number, IMEI_NUMBER_LENGTH);
	memcpy(pTerminal_Attri->strUim_Number,  pTerminal_Package->strTerminal_Number+IMEI_NUMBER_LENGTH,
				UIM_NUMBER_LENGTH);

	//2.�ж�IMEI��UIM����ĳ����Ƿ���ȷ
	if (strlen(pTerminal_Attri->strImei_Number) != IMEI_NUMBER_LENGTH ||
		strlen(pTerminal_Attri->strUim_Number) != UIM_NUMBER_LENGTH)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 10, "���ݰ����ն�Ωһ��ʶ[%s]���Ȳ��ԣ�����[%s]\n",
					pTerminal_Package->strTerminal_Number,
					pTerminal_Package->strPackage_Body);
		return 1;
	}
/*printf("�ն�Ωһ��ʶ=[%s]-->IMEI=[%s],UIM=[%s]\n",
		pTerminal_Package->strTerminal_Number, pTerminal_Attri->strImei_Number, pTerminal_Attri->strUim_Number);*/
	return 0;
}

void funTerminal_Get_Status_Print(stTerminal_Get_Status *p)
{
	printf("nTerminal_Status=%d,nTerminal_Time=%u.\nnBlack_Version=%d,nWhite_Version=%d,nLanguage_Version=%d.\n",
		p->nTerminal_Status, p->nTerminal_Time,
		p->nBlack_Version, p->nWhite_Version, p->nLanguage_Version);
}

/**********************************************************************
 ��������: funTerminal_DB_Status_To_Terminal
 ��������: �����ݿ��б�����ն�״̬ת��Ϊ�ն�ʶ�������״̬
 		   �Ӻ���funTerminal_Get_Status_Resp�з������
 �������:
	nTerminal_Status_In_DB-���ݿ��б�����ն˵�״̬
	strTerminal_Number-�ն�Ωһ��ʶ
	nLog_File-Ҫд����־�ļ�
 �������: ��
 �� �� ֵ: �ն�ʶ���5��״ֵ̬
 ȫ�ֱ���:
 ��д����: mahezhou
 ��д����: 2009��2��15��
 �޸�����:
 �޸�����:
 �޸�Ŀ��:
**********************************************************************/
int	funTerminal_DB_Status_To_Terminal(int nTerminal_Status_In_DB, unsigned const char *strTerminal_Number,
										int nLog_File)
{
	if (nTerminal_Status_In_DB == 12)
	{
		return 1;	//�ն�Ϊ��ͨ״̬
	}
	else if (nTerminal_Status_In_DB == 11)
	{
		return 4;	//�ն�ΪԤ��ͨ״̬
	}
	else if (nTerminal_Status_In_DB == 14)
	{
		return 3;	//�ն�Ϊ��λ״̬
	}

	else if (nTerminal_Status_In_DB == 0 || nTerminal_Status_In_DB == 10 ||
			nTerminal_Status_In_DB == 21 || nTerminal_Status_In_DB == 22 ||
			nTerminal_Status_In_DB == 13)		//��Ԥ������Ϊ����ģʽ��
	{
		return 0;	//�ն�Ϊ����ģʽ
	}

	else if (nTerminal_Status_In_DB == 15 || nTerminal_Status_In_DB == 16 ||
		nTerminal_Status_In_DB == 17 || nTerminal_Status_In_DB == 26 ||
		nTerminal_Status_In_DB == 27 || nTerminal_Status_In_DB == 60)
	{
		return 2;	//�ն�Ϊ�ػ�״̬
	}
	else
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"�ն�[%s]�����ݿ��б����״ֵ̬Ϊ[%d]������ʶ��\n",
					strTerminal_Number, nTerminal_Status_In_DB);
		return 0;	//�ն�״̬�쳣ʱ��Ĭ���ն�Ϊ����ģʽ
	}
}

/**********************************************************************
 ��������: funTerminal_Get_Status_Resp
 ��������: ��װ״̬ͬ���ذ��������ն˻ظ�
 �������:
	nSocket_ID-��������ID
	pOracle-���ݿ�����ָ��
	pTerminal_Package-״̬ͬ�����ݰ��ṹָ��
	pTerminal_Attri-�ն����Խṹָ��
	nLog_File-Ҫд����־�ļ�
 �������:
 �� �� ֵ:
	0		�ɹ�
	1		ʧ��
 	-1		SQLִ���쳣
 ȫ�ֱ���:
 ��д����: mahezhou
 ��д����: 2009��2��13��
 �޸�����:
 �޸�����:
 �޸�Ŀ��:
**********************************************************************/
int	funTerminal_Get_Status_Resp(int nSocket_ID, OLFO *pOracle, stTerminal_Package *pTerminal_Package,
			stTerminal_Attri *pTerminal_Attri, int nLog_File)
{
	char	strPackage_Body[PACKAGE_LENTGH_MAX];
	char	*pch;
	int		nPackage_Length;
	int		nTerminal;
	int		nReturn;

	memset(strPackage_Body, 0, sizeof(strPackage_Body));
	//1. �����ȡ������֡���ˮ���ն�Ωһ��ʶ
	*(int *)(strPackage_Body + 0) = 0;									//���ĳ���(����������ٸ���)
	*(int *)(strPackage_Body + 4) = TERMINAL_GET_STATUS_RESP;			//״̬ͬ���ذ�������
	*(int *)(strPackage_Body + 8) = pTerminal_Package->nSequence_ID;	//������ˮ��
	memcpy(strPackage_Body+12, pTerminal_Package->strTerminal_Number, 36);	//�ն�Ωһ��ʶ

	//2. �����ݿⱣ����ն�״̬ת��Ϊ�ն˿���ʶ�������״̬(0/1/2/3/4)�����鵽����
	nTerminal = funTerminal_DB_Status_To_Terminal(pTerminal_Attri->nTerminal_Status,
													pTerminal_Package->strTerminal_Number, nLog_File);
	*(int *)(strPackage_Body + 48) = nTerminal;	//�ն�״̬

	//3. ϵͳʱ��
	*(int *)(strPackage_Body + 52) = time(NULL);

	//4.1 �����������ĺ�������Ϣ
	int		nBlack_Version;		//�������汾��
	char	strBlack_List[BLACK_NUM_MAX*(BLACK_WHITE_PHONE_LEN+1)];
	nReturn = funGet_Hotel_Black_List(pOracle, pTerminal_Attri->strHotel_ID,
				&nBlack_Version, strBlack_List, nLog_File);
	if (nReturn != 0)
	{
		return nReturn;
	}

	//4.2 ���Ұ�������Ϣ
	int		nWhite_Version;		//�������汾��
	char	strWhite_List[WHITE_NUM_MAX*(BLACK_WHITE_PHONE_LEN+1)];
	nReturn = funGet_HAPPY_White_List(pOracle, &nWhite_Version, strWhite_List, nLog_File);
	if (nReturn != 0)
	{
		return nReturn;
	}

	//5.1 �Ѻ�������Ϣ��װ���ذ���
	pch = strPackage_Body + 56;
	*(int *)(pch) = nBlack_Version;							//�������汾
	*(int *)(pch+4) = strlen(strBlack_List);				//�������ֶγ���
	memcpy(pch+8, strBlack_List, strlen(strBlack_List)+1);	//����������
	pch = pch + 4 + 4 + strlen(strBlack_List)+1;
	nPackage_Length = 56 + 4 + 4 + strlen(strBlack_List)+1;

	//5.2 �Ѱ�������Ϣ��װ���ذ���
	*(int *)(pch) = nWhite_Version;							//�������汾
	*(int *)(pch+4) = strlen(strWhite_List);				//�������ֶγ���
	memcpy(pch+8, strWhite_List, strlen(strWhite_List)+1);	//����������
	pch = pch + 4 + 4 + strlen(strWhite_List)+1;
	nPackage_Length = nPackage_Length + 4 + 4 + strlen(strWhite_List)+1;

	//6.��д������
	*(int *)(strPackage_Body + 0) = nPackage_Length;

    //7. ���ն˻�״̬ͬ����
    nReturn = fun_Write(nSocket_ID, strPackage_Body, nPackage_Length, oGateParm.nTermSendDataTime);
    if (nReturn != 0)
    {
		funLogP(nLog_File, __FILE__, __LINE__, 15,
					"���ն�[%s]����״̬ͬ���ذ����ȴ�ʱ��[%d]������[%d]\n",
					pTerminal_Package->strTerminal_Number, oGateParm.nTermSendDataTime, nReturn);
		return 1;
	}
	else
	{
		funLogP(nLog_File, __FILE__, __LINE__, 30,
					"���ն�[%s]����״̬ͬ���ذ�����ȷ\n",
					pTerminal_Package->strTerminal_Number);
		//8. �����ݿ��м�¼�·����ն˵ĺڰ������汾��
		nReturn = pOracle->ExecSQL("Update T_Terminal_Info set Black_Version=:n1,White_Version=:n2 where Imei_Number=:s1 and Uim_Number=:s2",
					nBlack_Version, nWhite_Version, pTerminal_Attri->strImei_Number, pTerminal_Attri->strUim_Number);
		if (nReturn >1)		//���ݳ�����������
		{
			funLogP(nLog_File, __FILE__, __LINE__, 5,
					"ִ��SQL���[Update T_Terminal_Info set Black_Version=%d,White_Version=%d where Imei_Number='%s' and Uim_Number='%s']����[%d]\n",
					nBlack_Version, nWhite_Version, pTerminal_Attri->strImei_Number, pTerminal_Attri->strUim_Number,
					nReturn);
			pOracle->RollBack();
		}
		else if (nReturn < 0)
		{
			funLogP(nLog_File, __FILE__, __LINE__, 5,
					"ִ��SQL���[Update T_Terminal_Info set Black_Version=%d,White_Version=%d where Imei_Number='%s' and Uim_Number='%s']����[%d]��������Ϣ:%s\n",
					nBlack_Version, nWhite_Version, pTerminal_Attri->strImei_Number, pTerminal_Attri->strUim_Number,
					nReturn, pOracle->getErrorInfo());
			pOracle->RollBack();
		}
		pOracle->Commit();
		return 0;
	}

}

/**********************************************************************
 ��������: funTerminal_Get_Status_Deal
 ��������: �����ն˷����״̬ͬ����
 �������:
	nSocket_ID-��������ID
	pOracle-���ݿ�����ָ��
	pTerminal_Package-״̬ͬ�����ݰ��ṹָ��
	pTerminal_Attri-�ն����Խṹָ��
	nLog_File-Ҫд����־�ļ�
 �������:
 �� �� ֵ:
	0		�ɹ�
	1		ʧ��
 	-1		SQLִ���쳣
 ȫ�ֱ���:
 ��д����: mahezhou
 ��д����: 2009��2��13��
 �޸�����:
 �޸�����:
 �޸�Ŀ��:
**********************************************************************/
int	funTerminal_Get_Status_Deal(int nSocket_ID, OLFO *pOracle, stTerminal_Package *pTerminal_Package,
			stTerminal_Attri *pTerminal_Attri, int nLog_File)
{
	int		nReturn;

	stTerminal_Get_Status *p;
	//1. ǿ��ת���������ͣ���ָ��pָ��״̬ͬ�������õ�����
	p = (stTerminal_Get_Status *)pTerminal_Package->strPackage_Body;


	//2. ���ն�ѡ������԰汾ͬ�������ݿ���
	nReturn = pOracle->ExecSQL("Update T_Terminal_Info set Language=:s1 where Imei_Number=:s2 and Uim_Number=:s3",
				strArray_Language[p->nLanguage_Version], pTerminal_Attri->strImei_Number,
				pTerminal_Attri->strUim_Number);
	if (nReturn == 0 || nReturn >1)		//�ߵ���һ�����϶�Ӧ�óɹ������ʧ�ܣ�����һ�������ص�����
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"ִ��SQL���[Update T_Terminal_Info set Language='%s' where Imei_Number='%s' and Uim_Number='%s']����[%d]\n",
					strArray_Language[p->nLanguage_Version], pTerminal_Attri->strImei_Number,
					pTerminal_Attri->strUim_Number, nReturn);
		pOracle->RollBack();
		return 1;		//���ݿ�û�г����ʷ���+1
	}
	else if (nReturn < 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"ִ��SQL���[Update T_Terminal_Info set Language='%s' where Imei_Number='%s' and Uim_Number='%s']����[%d]��������Ϣ:%s\n",
					strArray_Language[p->nLanguage_Version], pTerminal_Attri->strImei_Number,
					pTerminal_Attri->strUim_Number, nReturn, pOracle->getErrorInfo());
		pOracle->RollBack();
		return -1;		//���ݿ�����ʷ���-1
	}
	pOracle->Commit();		//û������ı�Ҫ�����޸ĺ�ֱ���ύ

	//3.���ն˻ظ�״̬ͬ�����ݰ�
	nReturn = funTerminal_Get_Status_Resp(nSocket_ID, pOracle, pTerminal_Package, pTerminal_Attri, nLog_File);

	return 0;
}

/**********************************************************************
 ��������: funTerminal_Get_Status_Main
 ��������: �����ն˷����״̬ͬ����
 �������:
	nSocket_ID-��������ID
	pOracle-���ݿ�����ָ��
	pTerminal_Package-�ն����ݰ�ָ��
	pTerminal_Attri-�ն����Խṹָ��
	nLog_File-Ҫд����־�ļ�
 �������:
 �� �� ֵ:
	0		�ɹ�
	1		ʧ��
 	-1		SQLִ���쳣
 ȫ�ֱ���:
 ��д����: mahezhou
 ��д����: 2009��2��12��
 �޸�����:
 �޸�����:
 �޸�Ŀ��:
**********************************************************************/
int	funTerminal_Get_Status_Main(int nSocket_ID, OLFO *pOracle, stTerminal_Package *pTerminal_Package,
			stTerminal_Attri *pTerminal_Attri, int nLog_File)
{
	int		nReturn;
	stTerminal_Get_Status *p;

	//1. ǿ��ת���������ͣ���ָ��pָ��״̬ͬ�������õ�����
	p = (stTerminal_Get_Status *)pTerminal_Package->strPackage_Body;
	//funTerminal_Get_Status_Print(p);	//��ӡ״̬ͬ����������ֵ

	//2. �ж��ն˵����԰汾
	if (p->nLanguage_Version > LANGUAGE_LIST_MAX)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 10, "�ն�[%s]�����԰汾Ϊ[%d]������Ԥ�ڣ���Ϊ�޸�Ϊ0������[%s]\n",
					pTerminal_Package->strTerminal_Number, p->nLanguage_Version,
					pTerminal_Package->strPackage_Body);
		p->nLanguage_Version = 0;
	}

	//3. ����״̬ͬ����
	nReturn = funTerminal_Get_Status_Deal(nSocket_ID, pOracle, pTerminal_Package, pTerminal_Attri, nLog_File);
	if (nReturn == 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 30,
						"�Ƶ�[%s]�ն�[%s,%s,%s]״̬ͬ��������ɹ�\n",
						pTerminal_Attri->strHotel_ID, pTerminal_Attri->strImei_Number,
						pTerminal_Attri->strUim_Number, pTerminal_Attri->strMsisdn);
	}
	else
	{
		funLogP(nLog_File, __FILE__, __LINE__, 20,
						"�Ƶ�[%s]�ն�[%s,%s,%s]״̬ͬ������ʧ�ܣ�����[%d]\n",
						pTerminal_Attri->strHotel_ID, pTerminal_Attri->strImei_Number,
						pTerminal_Attri->strUim_Number, pTerminal_Attri->strMsisdn, nReturn);
	}

	return nReturn;
}

/**********************************************************************
 ��������: funTerminal_Open_Resp
 ��������: ��װ�ն˿�ͨ��������ذ��������ն˻ظ�
 �������:
	nSocket_ID-��������ID
	pTerminal_Package-�ն˿�ͨ���ݰ��ṹָ��
	nError_Code-����ִͨ�н��
	nLog_File-Ҫд����־�ļ�
 �������:
 �� �� ֵ:
	0		�ɹ�
	1		ʧ��
 ȫ�ֱ���:
 ��д����: mahezhou
 ��д����: 2009��2��14��
 �޸�����:
 �޸�����:
 �޸�Ŀ��:
**********************************************************************/
int	funTerminal_Open_Resp(int nSocket_ID, stTerminal_Package *pTerminal_Package,
			int nError_Code, int nLog_File)
{
	char	strPackage_Body[PACKAGE_LENTGH_MAX];
	int		nReturn;

	memset(strPackage_Body, 0, sizeof(strPackage_Body));
	//1. �����ȡ������֡���ˮ���ն�Ωһ��ʶ
	*(int *)(strPackage_Body + 0) = 52;							//���ĳ��ȣ��̶���52�ֽ�
	*(int *)(strPackage_Body + 4) = TERMINAL_OPEN_RESP;			//��ͨ�ذ�������
	*(int *)(strPackage_Body + 8) = pTerminal_Package->nSequence_ID;	//������ˮ��
	memcpy(strPackage_Body+12, pTerminal_Package->strTerminal_Number, 36);	//�ն�Ωһ��ʶ

	//2. ��ͨ���
	if (nError_Code == -1)	//���ݿ����,�ر�ת��һ��
	{
		*(int *)(strPackage_Body + 48) = TERMINAL_ERROR;
	}
	else
	{
		*(int *)(strPackage_Body + 48) = nError_Code;
	}

    //3. ���ն˻ؿ�ͨ�����
    nReturn = fun_Write(nSocket_ID, strPackage_Body, 52, oGateParm.nTermSendDataTime);
    if (nReturn != 0)
    {
		funLogP(nLog_File, __FILE__, __LINE__, 15,
					"nSocket_ID=%d�����ն�[%s]���Ϳ�ͨ����ذ���ʧ�ܣ��ȴ�ʱ��[%d]������[%d]\n",
					nSocket_ID, pTerminal_Package->strTerminal_Number, oGateParm.nTermSendDataTime, nReturn);
		return 1;
	}
	else
    {
		funLogP(nLog_File, __FILE__, __LINE__, 30,
					"nSocket_ID=%d�����ն�[%s]���Ϳ�ͨ����ذ����ɹ�������[%d]\n",
					nSocket_ID, pTerminal_Package->strTerminal_Number, nReturn);
		return 0;
	}
}

/**********************************************************************
 ��������: funTerminal_Open_Deal
 ��������: �����ն˷���Ŀ�ͨ�����
 �������:
	nSocket_ID-��������ID
	pOracle-���ݿ�����ָ��
	pTerminal_Package-��ͨ�������ݰ��ṹָ��
	pTerminal_Attri-�ն����Խṹָ��
	nLog_File-Ҫд����־�ļ�
 �������:
 �� �� ֵ:
	0		�ɹ�
	1		ʧ��
 	-1		SQLִ���쳣
 ȫ�ֱ���:
 ��д����: mahezhou
 ��д����: 2009��2��13��
 �޸�����:
 �޸�����:
 �޸�Ŀ��:
**********************************************************************/
int	funTerminal_Open_Deal(int nSocket_ID, OLFO *pOracle, stTerminal_Package *pTerminal_Package,
			stTerminal_Attri *pTerminal_Attri, int nLog_File)
{
	int		nReturn;
	int		nError_Code;			//��ͨ���ֵ��Ϊ0���ʾ��ͨ������ֵ���ʾʧ��
	char	strCurrent_Time[14+1];	//��ǰϵͳʱ�䣬��¼��ͨʱ��
	char	strYear_Month[6+1];
	char	strSql[1024];

	//1.ȡϵͳʱ�䣬�Լ���ǰ�·�
	funGetCurrentTime(strCurrent_Time, 14);
	memcpy(strYear_Month, strCurrent_Time, 6);
	strYear_Month[6] = 0;

	nError_Code = 0;
	//2.����ն��Ѿ���ͨ�������ٴ�ִ�п�ͨ����
	if (pTerminal_Attri->nTerminal_Status == 12)
	{
		nError_Code = TERMINAL_ALREADY_OPENED;
	}
	//3.����ն���Ԥ��ͨ��δ��ͨ״̬����ִ�п�ͨ����
	else if (pTerminal_Attri->nTerminal_Status == 10 || pTerminal_Attri->nTerminal_Status == 11)
	{
		//3.1 ���ն�״̬�޸�Ϊ�ѿ�ͨ
		nReturn = pOracle->ExecSQL("Update T_Terminal_Info set Status=12,Current_Hire_Time=to_date(:s1,'yyyymmddhh24miss'),Hire_Request_Time=NULL Where Imei_Number=:s2 and Uim_Number=:s3 and Status in (10,11)",
								strCurrent_Time, pTerminal_Attri->strImei_Number, pTerminal_Attri->strUim_Number);
		if (nReturn >1)		//���ݳ�����������
		{
			funLogP(nLog_File, __FILE__, __LINE__, 5,
					"ִ��SQL���[Update T_Terminal_Info set Status=12,Current_Hire_Time=to_date('%s','yyyymmddhh24miss'),Hire_Request_Time=NULL Where Imei_Number='%s' and Uim_Number='%s' and Status in (10,11)]����[%d]\n",
					strCurrent_Time, pTerminal_Attri->strImei_Number, pTerminal_Attri->strUim_Number,
					nReturn);
			nError_Code = TERMINAL_ERROR;		//���ݿ�û�г���
		}
		else if (nReturn < 0)
		{
			funLogP(nLog_File, __FILE__, __LINE__, 5,
					"ִ��SQL���[Update T_Terminal_Info set Status=12,Current_Hire_Time=to_date('%s','yyyymmddhh24miss'),Hire_Request_Time=NULL Where Imei_Number='%s' and Uim_Number='%s' and Status in (10,11)]����[%d]��������Ϣ:%s\n",
					strCurrent_Time, pTerminal_Attri->strImei_Number, pTerminal_Attri->strUim_Number,
					nReturn, pOracle->getErrorInfo());
			nError_Code =  -1;		//���ݿ�����ʷ���-1
		}
		else
		{
			//3.2 ���ն˿�ͨ��¼���뵽SDR����(Hire_Src/Statusʹ��'0',����Ϊ�Ժ��������ֶο����޸�Ϊ�ַ�����)
			sprintf(strSql, "Insert Into T_SDR_%s(ID,Imei_Number,Uim_Number,Msisdn,Hotel_ID,Room_No,Broker_ID,Hire_Src,Start_Time,Status) select Bill_SEQ.nextval,a.Imei_Number,a.Uim_Number,a.Msisdn,a.Hotel_ID,a.Room_No,c.Org_ID,'0',to_date('%s','yyyymmddhh24miss'),'0' from T_Terminal_Info a,T_Org_Info b,T_Org_Info c where a.Imei_Number='%s' and a.Uim_Number='%s' and a.Status=12 and a.Hotel_ID=b.Org_ID and b.Superior_Org_ID=c.Org_ID",
					strYear_Month, strCurrent_Time, pTerminal_Attri->strImei_Number, pTerminal_Attri->strUim_Number);
			nReturn = pOracle->ExecSQL(strSql);
			if (nReturn == -1)
			{
				funLogP(nLog_File, __FILE__, __LINE__, 5,
						"ִ��SQL���[%s]����[%d]��������Ϣ:%s\n",
						strSql, nReturn, pOracle->getErrorInfo());
				nError_Code =  -1;		//���ݿ�����ʷ���-1
			}
			else if (nReturn != 1)
			{
				funLogP(nLog_File, __FILE__, __LINE__, 5,
						"ִ��SQL���[%s]����[%d]\n",
						strSql, nReturn);
				nError_Code = TERMINAL_ERROR;		//���ݿ�û�г���
			}
		}
	}
	//4.����״̬������ն˻ظ���ͨʧ��
	else
	{
		nError_Code = TERMINAL_ERROR;
	}

	//5.���ն˻ظ���ͨ������ݰ�
	nReturn = funTerminal_Open_Resp(nSocket_ID, pTerminal_Package, nError_Code, nLog_File);
	if (nError_Code == -1)		//������ݿ�����쳣���򷵻�-1
	{
		return nError_Code;
	}
	else if (nError_Code == 0)	//������ݿ������ȷ�����ո��ն˵Ļذ����������
	{
		return nReturn;
	}
	else						//������ݿ���³��ִ����򷵻�����������
	{
		return nError_Code;
	}
}

/**********************************************************************
 ��������: funTerminal_Open_Main
 ��������: �����ն˷���Ŀ�ͨ�����
 �������:
	nSocket_ID-��������ID
	pOracle-���ݿ�����ָ��
	pTerminal_Package-�ն����ݰ�ָ��
	pTerminal_Attri-�ն����Խṹָ��
	nLog_File-Ҫд����־�ļ�
 �������:
 �� �� ֵ:
	0		�ɹ�
	1		ʧ��
 	-1		SQLִ���쳣
 ȫ�ֱ���:
 ��д����: mahezhou
 ��д����: 2009��2��13��
 �޸�����:
 �޸�����:
 �޸�Ŀ��:
**********************************************************************/
int	funTerminal_Open_Main(int nSocket_ID, OLFO *pOracle, stTerminal_Package *pTerminal_Package,
			stTerminal_Attri *pTerminal_Attri, int nLog_File)
{
	int		nReturn;

	nReturn = funTerminal_Open_Deal(nSocket_ID, pOracle, pTerminal_Package, pTerminal_Attri, nLog_File);
	if (nReturn == 0)
	{
		pOracle->Commit();
		funLogP(nLog_File, __FILE__, __LINE__, 30,
						"�Ƶ�[%s]�ն�[%s,%s,%s]��ͨ����ɹ�\n",
						pTerminal_Attri->strHotel_ID, pTerminal_Attri->strImei_Number,
						pTerminal_Attri->strUim_Number, pTerminal_Attri->strMsisdn);
	}
	else
	{
		pOracle->RollBack();
		funLogP(nLog_File, __FILE__, __LINE__, 20,
						"�Ƶ�[%s]�ն�[%s,%s,%s]��ͨ����ʧ��,����[%d]\n",
						pTerminal_Attri->strHotel_ID, pTerminal_Attri->strImei_Number,
						pTerminal_Attri->strUim_Number, pTerminal_Attri->strMsisdn,
						nReturn);
	}

	return nReturn;
}

/**********************************************************************
 ��������: funTerminal_Quit_Resp
 ��������: ��װ�ն���������ذ��������ն˻ظ�
 �������:
	nSocket_ID-��������ID
	pTerminal_Package-�ն��������ݰ��ṹָ��
	strHire_Time-��ͨʱ��
	strQuit_Time-����ʱ��
	nError_Code-����ִ�н��
	nLog_File-Ҫд����־�ļ�
 �������:
 �� �� ֵ:
	0		�ɹ�
	1		ʧ��
 ȫ�ֱ���:
 ��д����: mahezhou
 ��д����: 2009��2��14��
 �޸�����:
 �޸�����:
 �޸�Ŀ��:
**********************************************************************/
int	funTerminal_Quit_Resp(int nSocket_ID, stTerminal_Package *pTerminal_Package,
			const char *strHire_Time, const char *strQuit_Time, int nError_Code, int nLog_File)
{
	char	strPackage_Body[PACKAGE_LENTGH_MAX];
	int		nReturn;

	memset(strPackage_Body, 0, sizeof(strPackage_Body));
	//1. �����ȡ������֡���ˮ���ն�Ωһ��ʶ
	*(int *)(strPackage_Body + 0) = 60;							//���ĳ��ȣ��̶���60�ֽ�
	*(int *)(strPackage_Body + 4) = TERMINAL_QUIT_RESP;			//����ذ�������
	*(int *)(strPackage_Body + 8) = pTerminal_Package->nSequence_ID;	//������ˮ��
	memcpy(strPackage_Body+12, pTerminal_Package->strTerminal_Number, 36);	//�ն�Ωһ��ʶ

	//2. ������
	if (nError_Code == -1)	//���ݿ����,�ر�ת��һ��
	{
		*(int *)(strPackage_Body + 48) = TERMINAL_ERROR;
	}
	else
	{
		*(int *)(strPackage_Body + 48) = nError_Code;
	}

	//3. ��ͨʱ�䡢����ʱ��
	*(int *)(strPackage_Body + 52) = funString_To_Time_T(strHire_Time);
	*(int *)(strPackage_Body + 56) = funString_To_Time_T(strQuit_Time);

    //4. ���ն˻���������
    nReturn = fun_Write(nSocket_ID, strPackage_Body, 60, oGateParm.nTermSendDataTime);
    if (nReturn != 0)
    {
		funLogP(nLog_File, __FILE__, __LINE__, 15,
					"nSocket_ID=%d�����ն�[%s]�����������ذ����ȴ�ʱ��[%d]������[%d]\n",
					nSocket_ID, pTerminal_Package->strTerminal_Number, oGateParm.nTermSendDataTime, nReturn);
		return 0;	//���ۻذ��Ƿ���ȷ������Ϊ��ȷ������Ϊ������
	}

	return 0;
}

/**********************************************************************
 ��������: funTerminal_Quit_Deal
 ��������: �����ն˷�������������
 �������:
	nSocket_ID-��������ID
	pOracle-���ݿ�����ָ��
	pTerminal_Package-�����������ݰ��ṹָ��
	pTerminal_Attri-�ն����Խṹָ��
	nLog_File-Ҫд����־�ļ�
 �������:
 �� �� ֵ:
	0		�ɹ�
	1		ʧ��
 	-1		SQLִ���쳣
 ȫ�ֱ���:
 ��д����: mahezhou
 ��д����: 2009��2��14��
 �޸�����:
 �޸�����:
 �޸�Ŀ��:
**********************************************************************/
int	funTerminal_Quit_Deal(int nSocket_ID, OLFO *pOracle, stTerminal_Package *pTerminal_Package,
			stTerminal_Attri *pTerminal_Attri, int nLog_File)
{
	int		nReturn;
	int		nError_Code;			//������ֵ��Ϊ0���ʾ����ɹ�������ֵ���ʾʧ��
	char	strCurrent_Time[14+1];	//��ǰϵͳʱ�䣬��¼����ʱ��
	char	strYear_Month[6+1];
	char	strSql[1024];

	//1.ȡϵͳʱ��,��Ϊ�����ʱ��
	funGetCurrentTime(strCurrent_Time, 14);

	//2.ȡ��ͨʱ�������·�
	memcpy(strYear_Month, pTerminal_Attri->strCurrent_Hire_Time, 6);
	strYear_Month[6] = 0;

	//3.����ն�û�п�ͨ������ִ���������
	nError_Code = 0;
	if (pTerminal_Attri->nTerminal_Status != 12)
	{
		nError_Code = TERMINAL_NOT_OPENED;
	}
	//4.����ն��ǿ�ͨ״̬����ִ���������
	else
	{
		//4.1 ���ն�״̬�޸�Ϊ������(δ��ͨ)
		nReturn = pOracle->ExecSQL("Update T_Terminal_Info set Status=10,Last_Hire_Time=Current_Hire_Time,Last_Quit_Hire_Time=to_date(:s1,'yyyymmddhh24miss'),Current_Hire_Time=null Where Imei_Number=:s2 and Uim_Number=:s3 and Status=12",
								strCurrent_Time, pTerminal_Attri->strImei_Number, pTerminal_Attri->strUim_Number);
		if (nReturn >1)		//���ݳ�����������
		{
			funLogP(nLog_File, __FILE__, __LINE__, 5,
					"ִ��SQL���[Update T_Terminal_Info set Status=10,Last_Hire_Time=Current_Hire_Time,Last_Quit_Hire_Time=to_date('%s','yyyymmddhh24miss'),Current_Hire_Time=null Where Imei_Number='%s' and Uim_Number='%s' and Status=12]����[%d]\n",
					strCurrent_Time, pTerminal_Attri->strImei_Number, pTerminal_Attri->strUim_Number,
					nReturn);
			nError_Code = TERMINAL_ERROR;		//���ݿ�û�г���
		}
		else if (nReturn < 0)
		{
			funLogP(nLog_File, __FILE__, __LINE__, 5,
					"ִ��SQL���[Update T_Terminal_Info set Status=10,Last_Hire_Time=Current_Hire_Time,Last_Quit_Hire_Time=to_date('%s','yyyymmddhh24miss'),Current_Hire_Time=null Where Imei_Number='%s' and Uim_Number='%s' and Status=12]����[%d]��������Ϣ:%s\n",
					strCurrent_Time, pTerminal_Attri->strImei_Number, pTerminal_Attri->strUim_Number,
					nReturn, pOracle->getErrorInfo());
			nError_Code =  -1;		//���ݿ�����ʷ���-1
		}
		else
		{
			//4.2 �޸�SDR���״̬(Quit_Src/Statusʹ��'0',����Ϊ�Ժ��������ֶο����޸�Ϊ�ַ�����)
			sprintf(strSql, "Update T_SDR_%s set End_Time=to_date('%s','yyyymmddhh24miss'),Quit_Src='0',Status='1' where Imei_Number='%s' and Uim_Number='%s' and Status='0' and to_char(Start_Time,'yyyymmddhh24miss')='%s'",
					strYear_Month, strCurrent_Time,
					pTerminal_Attri->strImei_Number, pTerminal_Attri->strUim_Number,
					pTerminal_Attri->strCurrent_Hire_Time);
			nReturn = pOracle->ExecSQL(strSql);
			if (nReturn == -1)
			{
				funLogP(nLog_File, __FILE__, __LINE__, 5,
						"ִ��SQL���[%s]����[%d]��������Ϣ:%s\n",
						strSql, nReturn, pOracle->getErrorInfo());
				nError_Code =  -1;		//���ݿ�����ʷ���-1
			}
			else if (nReturn != 1)
			{
				funLogP(nLog_File, __FILE__, __LINE__, 5,
						"ִ��SQL���[%s]����[%d]\n",
						strSql, nReturn);
				nError_Code = TERMINAL_ERROR;		//���ݿ�û�г���
			}
		}
	}

	//5.���ն˻ظ���ͨ������ݰ�
	nReturn = funTerminal_Quit_Resp(nSocket_ID, pTerminal_Package, pTerminal_Attri->strCurrent_Hire_Time,
					strCurrent_Time, nError_Code, nLog_File);

	if (nError_Code == -1)		//������ݿ�����쳣���򷵻�-1
	{
		return nError_Code;
	}
	else if (nError_Code == 0)	//������ݿ������ȷ�����ո��ն˵Ļذ����������
	{
		return nReturn;
	}
	else						//������ݿ���³��ִ����򷵻�����������
	{
		return nError_Code;
	}
}

/**********************************************************************
 ��������: funTerminal_Quit_Main
 ��������: �����ն˷�������������
 �������:
	nSocket_ID-��������ID
	pOracle-���ݿ�����ָ��
	pTerminal_Package-�ն����ݰ�ָ��
	pTerminal_Attri-�ն����Խṹָ��
	nLog_File-Ҫд����־�ļ�
 �������:
 �� �� ֵ:
	0		�ɹ�
	1		ʧ��
 	-1		SQLִ���쳣
 ȫ�ֱ���:
 ��д����: mahezhou
 ��д����: 2009��2��14��
 �޸�����:
 �޸�����:
 �޸�Ŀ��:
**********************************************************************/
int	funTerminal_Quit_Main(int nSocket_ID, OLFO *pOracle, stTerminal_Package *pTerminal_Package,
			stTerminal_Attri *pTerminal_Attri, int nLog_File)
{
	int		nReturn;

	nReturn = funTerminal_Quit_Deal(nSocket_ID, pOracle, pTerminal_Package, pTerminal_Attri, nLog_File);
	if (nReturn == 0)
	{
		pOracle->Commit();
		funLogP(nLog_File, __FILE__, __LINE__, 30,
						"nSocket_ID=%d���Ƶ�[%s]�ն�[%s,%s,%s]����ɹ�������ն˵Ŀ�ͨʱ��Ϊ[%s]\n",
						nSocket_ID, pTerminal_Attri->strHotel_ID, pTerminal_Attri->strImei_Number,
						pTerminal_Attri->strUim_Number, pTerminal_Attri->strMsisdn,
						pTerminal_Attri->strCurrent_Hire_Time);
	}
	else
	{
		pOracle->RollBack();
		funLogP(nLog_File, __FILE__, __LINE__, 20,
						"nSocket_ID=%d���Ƶ�[%s]�ն�[%s,%s,%s]����ʧ�ܣ�����[%d]\n",
						nSocket_ID, pTerminal_Attri->strHotel_ID, pTerminal_Attri->strImei_Number,
						pTerminal_Attri->strUim_Number, pTerminal_Attri->strMsisdn,
						nReturn);
	}

	return nReturn;
}

/**********************************************************************
 ��������: funTerminal_Heartbeat_Deal
 ��������: �����ն˷����������
 �������:
	nSocket_ID-��������ID
	pTerminal_Package-�������ṹָ��
	pTerminal_Attri-�ն����Խṹָ��
	nLog_File-Ҫд����־�ļ�
 �������:
 �� �� ֵ:
	0		�ɹ�
 ȫ�ֱ���:
 ��д����: mahezhou
 ��д����: 2009��2��15��
 �޸�����:
 �޸�����:
 �޸�Ŀ��:
**********************************************************************/
int	funTerminal_Heartbeat_Deal(int nSocket_ID, stTerminal_Package *pTerminal_Package,
			stTerminal_Attri *pTerminal_Attri, int nLog_File)
{
	char	strPackage_Body[PACKAGE_LENTGH_MAX];
	int		nTerminal;
	int		nReturn;

	memset(strPackage_Body, 0, sizeof(strPackage_Body));
	//1. �����ȡ������֡���ˮ���ն�Ωһ��ʶ
	*(int *)(strPackage_Body + 0) = 56;									//���ĳ��ȣ��̶���56�ֽ�
	*(int *)(strPackage_Body + 4) = TERMINAL_HEARTBEAT_RESP;			//�����ذ�������
	*(int *)(strPackage_Body + 8) = pTerminal_Package->nSequence_ID;	//������ˮ��
	memcpy(strPackage_Body+12, pTerminal_Package->strTerminal_Number, 36);	//�ն�Ωһ��ʶ

	//2. �����ݿⱣ����ն�״̬ת��Ϊ�ն˿���ʶ�������״̬(0/1/2/3/4)�����鵽����
	nTerminal = funTerminal_DB_Status_To_Terminal(pTerminal_Attri->nTerminal_Status,
													pTerminal_Package->strTerminal_Number, nLog_File);
	*(int *)(strPackage_Body + 48) = nTerminal;	//�ն�״̬

	//3. ϵͳʱ��
	*(int *)(strPackage_Body + 52) = time(NULL);

    //4. ���ն˻�������
    nReturn = fun_Write(nSocket_ID, strPackage_Body, 56, oGateParm.nTermSendDataTime);
    if (nReturn != 0)
    {
		funLogP(nLog_File, __FILE__, __LINE__, 15,
					"nSocket_ID=%d�����ն�[%s]���������ذ����ȴ�ʱ��[%d]������[%d]\n",
					nSocket_ID, pTerminal_Package->strTerminal_Number, oGateParm.nTermSendDataTime, nReturn);
		return 1;
	}

	return 0;
}

/**********************************************************************
 ��������: funTerminal_Heartbeat_Main
 ��������: �����ն˷����������
 �������:
	nSocket_ID-��������ID
	pOracle-���ݿ�����ָ��
	pTerminal_Package-�ն����ݰ�ָ��
	pTerminal_Attri-�ն����Խṹָ��
	nLog_File-Ҫд����־�ļ�
 �������:
 �� �� ֵ:
	0		�ɹ�
	1		ʧ��
 	-1		SQLִ���쳣
 ȫ�ֱ���:
 ��д����: mahezhou
 ��д����: 2009��2��15��
 �޸�����:
 �޸�����:
 �޸�Ŀ��:
**********************************************************************/
int	funTerminal_Heartbeat_Main(int nSocket_ID, OLFO *pOracle, stTerminal_Package *pTerminal_Package,
			stTerminal_Attri *pTerminal_Attri, int nLog_File)
{
	int		nReturn;

	nReturn = funTerminal_Heartbeat_Deal(nSocket_ID, pTerminal_Package, pTerminal_Attri, nLog_File);
	if (nReturn == 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 30,
						"nSocket_ID=%d���Ƶ�[%s]�ն�[%s,%s,%s]����������ɹ�\n",
						nSocket_ID, pTerminal_Attri->strHotel_ID, pTerminal_Attri->strImei_Number,
						pTerminal_Attri->strUim_Number, pTerminal_Attri->strMsisdn);
	}
	else
	{
		funLogP(nLog_File, __FILE__, __LINE__, 20,
						"nSocket_ID=%d���Ƶ�[%s]�ն�[%s,%s,%s]����������ʧ�ܣ�����[%d]\n",
						nSocket_ID, pTerminal_Attri->strHotel_ID, pTerminal_Attri->strImei_Number,
						pTerminal_Attri->strUim_Number, pTerminal_Attri->strMsisdn, nReturn);
	}

	return nReturn;
}

/**********************************************************************
 ��������: funTerminal_Get_Time_Deal
 ��������: �����ն˷����ʱ��ͬ����
 �������:
	nSocket_ID-��������ID
	pTerminal_Package-ʱ��ͬ�����ṹָ��
	pTerminal_Attri-�ն����Խṹָ��
	nLog_File-Ҫд����־�ļ�
 �������:
 �� �� ֵ:
	0		�ɹ�
	1		ʧ��
 ȫ�ֱ���:
 ��д����: mahezhou
 ��д����: 2009��2��15��
 �޸�����:
 �޸�����:
 �޸�Ŀ��:
**********************************************************************/
int	funTerminal_Get_Time_Deal(int nSocket_ID, stTerminal_Package *pTerminal_Package,
			stTerminal_Attri *pTerminal_Attri, int nLog_File)
{
	char	strPackage_Body[PACKAGE_LENTGH_MAX];
	int		nReturn;

	memset(strPackage_Body, 0, sizeof(strPackage_Body));
	//1. �����ȡ������֡���ˮ���ն�Ωһ��ʶ
	*(int *)(strPackage_Body + 0) = 52;									//���ĳ��ȣ��̶���56�ֽ�
	*(int *)(strPackage_Body + 4) = TERMINAL_GET_TIME_RESP;				//ʱ��ͬ���ذ�������
	*(int *)(strPackage_Body + 8) = pTerminal_Package->nSequence_ID;	//������ˮ��
	memcpy(strPackage_Body+12, pTerminal_Package->strTerminal_Number, 36);	//�ն�Ωһ��ʶ

	//2. ϵͳʱ��
	*(int *)(strPackage_Body + 48) = time(NULL);

    //3. ���ն˻�ʱ��ͬ����
    nReturn = fun_Write(nSocket_ID, strPackage_Body, 52, oGateParm.nTermSendDataTime);
    if (nReturn != 0)
    {
		funLogP(nLog_File, __FILE__, __LINE__, 15,
					"nSocket_ID=%d�����ն�[%s]����ʱ��ͬ���ذ����ȴ�ʱ��[%d]������[%d]\n",
					nSocket_ID, pTerminal_Package->strTerminal_Number, oGateParm.nTermSendDataTime, nReturn);
		return 1;
	}

	return 0;
}

/**********************************************************************
 ��������: funTerminal_Get_Time_Main
 ��������: �����ն˷����ʱ��ͬ����
 �������:
	nSocket_ID-��������ID
	pOracle-���ݿ�����ָ��
	pTerminal_Package-�ն����ݰ�ָ��
	pTerminal_Attri-�ն����Խṹָ��
	nLog_File-Ҫд����־�ļ�
 �������:
 �� �� ֵ:
	0		�ɹ�
	1		ʧ��
 	-1		SQLִ���쳣
 ȫ�ֱ���:
 ��д����: mahezhou
 ��д����: 2009��2��15��
 �޸�����:
 �޸�����:
 �޸�Ŀ��:
**********************************************************************/
int	funTerminal_Get_Time_Main(int nSocket_ID, OLFO *pOracle, stTerminal_Package *pTerminal_Package,
			stTerminal_Attri *pTerminal_Attri, int nLog_File)
{
	int		nReturn;

	nReturn = funTerminal_Get_Time_Deal(nSocket_ID, pTerminal_Package, pTerminal_Attri, nLog_File);
	if (nReturn == 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 30,
						"nSocket_ID=%d���Ƶ�[%s]�ն�[%s,%s,%s]ʱ��ͬ��������ɹ�\n",
						nSocket_ID, pTerminal_Attri->strHotel_ID, pTerminal_Attri->strImei_Number,
						pTerminal_Attri->strUim_Number, pTerminal_Attri->strMsisdn);
	}
	else
	{
		funLogP(nLog_File, __FILE__, __LINE__, 25,
						"nSocket_ID=%d���Ƶ�[%s]�ն�[%s,%s,%s]ʱ��ͬ��������ʧ�ܣ�����[%d]\n",
						nSocket_ID, pTerminal_Attri->strHotel_ID, pTerminal_Attri->strImei_Number,
						pTerminal_Attri->strUim_Number, pTerminal_Attri->strMsisdn, nReturn);
	}

	return nReturn;
}

/**********************************************************************
 ��������: funTerminal_Statistics_Parse_Column
 ��������: ��ָ��ָ��ĵ�ַ��ȡһ�м�¼�������������У���¼���ֶ�֮���԰�ǿո�ָ��֮����\r\n�ָ�
 �������:
	strSource-ԭʼ����ָ��
	nSize_Max-��󳤶�
 �������:
	strDest0-Ŀ���ֶ�0
	strDest1-Ŀ���ֶ�1
	strDest2-Ŀ���ֶ�2
 �� �� ֵ: ȡ�õ��ֽ���
 ȫ�ֱ���:
 ��д����: mahezhou
 ��д����: 2009��2��15��
 �޸�����:
 �޸�����:
 �޸�Ŀ��:
**********************************************************************/
int	funTerminal_Statistics_Parse_Column(const char *strSource, char *strDest0, char *strDest1, char *strDest2,
					int nSize_Max)
{
	int		i, j;
	int		nCurrent_Column=0;
	strDest0[0] = 0;
	strDest1[0] = 0;
	strDest2[0] = 0;

	j = 0;		//��¼��ʼλ��
	//printf("nSize_Max=%d\n", nSize_Max);
	for (i=0; i<=nSize_Max; i++)
	{
		if (strSource[i] == '\r' || strSource[i] == '\n' || strSource[i] == ' ' || i == nSize_Max)
		{
			switch (nCurrent_Column)
			{
				case 0:
					memcpy(strDest0, strSource+j, i-j);
					strDest0[i-j] = 0;
					break;
				case 1:
					memcpy(strDest1, strSource+j, i-j);
					strDest1[i-j] = 0;
					break;
				case 2:
					memcpy(strDest2, strSource+j, i-j);
					strDest2[i-j] = 0;
					break;
			}
			nCurrent_Column++;
			j = i + 1;
			if ((i+1) == nSize_Max)
			{
				i++;
				break;
			}
			else if (strSource[i] == '\n')
			{
				i++;
				break;
			}
			else  if (strSource[i] == '\r' && strSource[i+1] == '\n')
			{
				i = i + 2;
				break;
			}
		}
		if (nCurrent_Column == 3)
		{
			break;
		}
	}

	if (strcmp(strDest0, "-") == 0)
	{
		strDest0[0] = 0;
	}

	if (strcmp(strDest1, "-") == 0)
	{
		strDest1[0] = 0;
	}

	if (strcmp(strDest2, "-") == 0)
	{
		strDest2[0] = 0;
	}
	return i;
}

/**********************************************************************
 ��������: funTerminal_Statistics_Deal
 ��������: �����ն˷����ͳ�����ݰ�
 �������:
	nSocket_ID-��������ID
	pOracle-���ݿ�����ָ��
	pTerminal_Package-ͳ�����ݰ��ṹָ��
	pTerminal_Attri-�ն����Խṹָ��
	nLog_File-Ҫд����־�ļ�
 �������:
 �� �� ֵ:
	0		�ɹ�
	1		ʧ��
 	-1		SQLִ���쳣
 ȫ�ֱ���:
 ��д����: mahezhou
 ��д����: 2009��2��15��
 �޸�����:
 �޸�����:
 �޸�Ŀ��:
**********************************************************************/
int	funTerminal_Statistics_Deal(int nSocket_ID, OLFO *pOracle, stTerminal_Package *pTerminal_Package,
			stTerminal_Attri *pTerminal_Attri, int nLog_File)
{
	char	strColumn0[64];		//�ֶ�0�����ݵİ汾��
	char	strColumn1[64];		//�ֶ�1��д��־����İ汾��
	char	strColumn2[64];		//�ֶ�2����־��ʽ�汾��

	int		nData_Size;
	int		nData_Size_Read;
	char	*pch;
	nData_Size = *(int *)(pTerminal_Package->strPackage_Body);

	//1. ȡͳ�����ݵĵ�һ��
	pch = (char *)pTerminal_Package->strPackage_Body+4;	//��ǰ��4�ֽ�Խ��ȥ
	nData_Size_Read = funTerminal_Statistics_Parse_Column(pch, strColumn0, strColumn1, strColumn2, nData_Size);
	nData_Size = nData_Size - nData_Size_Read;
	pch = pch + nData_Size_Read;
	//2. ѭ��ȡͳ�����ݡ����
	char	strDest0[32];	//������ʱ�䣬YYYYMMDDHHMMSS
	char	strDest1[256];	//��������·��
	char	strDest2[32];	//����ʱ��
	char	strSql[1024];
	int		nReturn;
	while (nData_Size > 3)
	{
		nData_Size_Read = funTerminal_Statistics_Parse_Column(pch, strDest0, strDest1, strDest2, nData_Size);
		nData_Size = nData_Size - nData_Size_Read;
		pch = pch + nData_Size_Read;

		sprintf(strSql, "Insert Into T_Access_Log(ID,Imei_Number,Uim_Number,Msisdn,Content_Version,Log_Program_Version,Log_Format_Version,Content_Prefix,Language,Content_Path,Settle_Seconds,Hotel_ID,Create_Time) values(Bill_SEQ.nextval,'%s','%s','%s','%s','%s','%s',substr('%s',1,4),substr('%s',7,2),'%s',%s,'%s',to_date('%s','yyyymmddhh24miss'))",
						pTerminal_Attri->strImei_Number, pTerminal_Attri->strUim_Number, pTerminal_Attri->strMsisdn,
						strColumn0, strColumn1, strColumn2, strDest1, strDest1, strDest1, strDest2,
						pTerminal_Attri->strHotel_ID, strDest0);
		nReturn = pOracle->ExecSQL(strSql);
		if (nReturn == -1)
		{
			funLogP(nLog_File, __FILE__, __LINE__, 5,
						"ִ��SQL���[%s]����[%d]��������Ϣ:%s\n",
						strSql, nReturn, pOracle->getErrorInfo());
		}
		else if (nReturn != 1)
		{
			funLogP(nLog_File, __FILE__, __LINE__, 5,
						"ִ��SQL���[%s]����[%d]\n",
						strSql, nReturn);
		}
		pOracle->Commit();	//����û������ĸ����ˣ������Commit������ʧ�����ͳ������
	}

	//3. �ذ�
	char	strPackage_Body[PACKAGE_LENTGH_MAX];
	memset(strPackage_Body, 0, sizeof(strPackage_Body));
	//4. �����ȡ������֡���ˮ���ն�Ωһ��ʶ
	*(int *)(strPackage_Body + 0) = 52;									//���ĳ��ȣ��̶���56�ֽ�
	*(int *)(strPackage_Body + 4) = TERMINAL_STATISTICS_RESP;				//ʱ��ͬ���ذ�������
	*(int *)(strPackage_Body + 8) = pTerminal_Package->nSequence_ID;	//������ˮ��
	memcpy(strPackage_Body+12, pTerminal_Package->strTerminal_Number, 36);	//�ն�Ωһ��ʶ

	//5. ������������0�Դ�
	*(int *)(strPackage_Body + 48) = 0;

    //6. ���ն˻�ͳ�ƻذ�
    nReturn = fun_Write(nSocket_ID, strPackage_Body, 52, oGateParm.nTermSendDataTime);
    if (nReturn != 0)
    {
		funLogP(nLog_File, __FILE__, __LINE__, 15,
					"nSocket_ID=%d�����ն�[%s]����ͳ�ƻذ����ȴ�ʱ��[%d]������[%d]\n",
					nSocket_ID, pTerminal_Package->strTerminal_Number, oGateParm.nTermSendDataTime, nReturn);
		return 1;
	}

	return 0;
}

/**********************************************************************
 ��������: funTerminal_Statistics_Main
 ��������: �����ն˷����ͳ�����ݰ�
 �������:
	nSocket_ID-��������ID
	pOracle-���ݿ�����ָ��
	pTerminal_Package-�ն����ݰ�ָ��
	pTerminal_Attri-�ն����Խṹָ��
	nLog_File-Ҫд����־�ļ�
 �������:
 �� �� ֵ:
	0		�ɹ�
	1		ʧ��
 	-1		SQLִ���쳣
 ȫ�ֱ���:
 ��д����: mahezhou
 ��д����: 2009��2��15��
 �޸�����:
 �޸�����:
 �޸�Ŀ��:
**********************************************************************/
int	funTerminal_Statistics_Main(int nSocket_ID, OLFO *pOracle, stTerminal_Package *pTerminal_Package,
			stTerminal_Attri *pTerminal_Attri, int nLog_File)
{
	int		nReturn;

	nReturn = funTerminal_Statistics_Deal(nSocket_ID, pOracle, pTerminal_Package, pTerminal_Attri, nLog_File);
	if (nReturn == 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 30,
						"nSocket_ID=%d���Ƶ�[%s]�ն�[%s,%s,%s]ͳ�����ݰ�����ɹ�\n",
						nSocket_ID, pTerminal_Attri->strHotel_ID, pTerminal_Attri->strImei_Number,
						pTerminal_Attri->strUim_Number, pTerminal_Attri->strMsisdn);
	}
	else
	{
		funLogP(nLog_File, __FILE__, __LINE__, 20,
						"nSocket_ID=%d���Ƶ�[%s]�ն�[%s,%s,%s]ͳ�����ݰ�����ʧ�ܣ�����[%d]\n",
						nSocket_ID, pTerminal_Attri->strHotel_ID, pTerminal_Attri->strImei_Number,
						pTerminal_Attri->strUim_Number, pTerminal_Attri->strMsisdn, nReturn);
	}

	return nReturn;
}

/**********************************************************************
 ��������: funTerminal_Data_IS_Old
 ��������: �ж����ݿ��м�¼�����и��ն˵ĺ������汾���������汾���ն˲����Ƿ���Ҫ����
 �������:
	pOracle-���ݿ�����ָ��
	strHotel_ID-�ն������Ƶ�ID
	strImei_Number-�ն˵�IMEI����
	strUim_Number-�ն˵�UIM����
	nLog_File-Ҫд����־�ļ�
 �������:
	pnBlack_Is_Old-���ն˵ĺ������汾�Ѿ�����(1��ʾ���ڣ�������ʾ������)
	pnWhite_Is_Old-���ն˵İ������汾�Ѿ�����(1��ʾ���ڣ�������ʾ������)
	pnTerminal_Parm_Is_Old-���ն˵Ĳ����Ѿ�����(1��ʾ���ڣ�������ʾ������)
 �� �� ֵ:
	0		�ɹ�
	1		ʧ��
 	-1		SQLִ���쳣
 ȫ�ֱ���:
 ��д����: mahezhou
 ��д����: 2009��2��16��
 �޸�����:
 �޸�����:
 �޸�Ŀ��:
**********************************************************************/
int	funTerminal_Data_IS_Old(OLFO *pOracle, char *strHotel_ID, char *strImei_Number, char *strUim_Number,
		int *pnBlack_Is_Old, int *pnWhite_Is_Old, int *pnTerminal_Parm_Is_Old, int nLog_File)
{
	int		nReturn;
	time_t	nBlack_Version_DB;
	time_t	nWhite_Version_DB;
	char	strTerminal_Parm_Time_DB[14+1];

	time_t	nBlack_Version_Real;
	time_t	nWhite_Version_Real;
	char	strTerminal_Parm_Time_Real[14+1];

	//1. ����ƽ̨��¼�ĸ�����ն��·��ĺڰ����������°汾���ն˲������°汾
	nReturn = pOracle->ExecSQL("Select nvl(Black_Version,0),nvl(White_Version,0),to_char(nvl(Terminal_Parm_Time,sysdate-600),'yyyymmddhh24miss') from T_Terminal_Info where Imei_Number=:s1 and Uim_Number=:s2 and Hotel_ID=:s3",
				strImei_Number, strUim_Number, strHotel_ID,
				&nBlack_Version_DB, &nWhite_Version_DB, strTerminal_Parm_Time_DB);
	if (nReturn < 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"ִ��SQL���[Select nvl(Black_Version,0),nvl(White_Version,0),to_char(nvl(Terminal_Parm_Time,sysdate-600),'yyyymmddhh24miss') from T_Terminal_Info where Imei_Number='%s' and Uim_Number='%s' and Hotel_ID='%s']����[%d]��������Ϣ:%s\n",
					strImei_Number, strUim_Number, strHotel_ID,
					nReturn, pOracle->getErrorInfo());
		return -1;
	}
	else if (nReturn == 1000)	//�ߵ���һ������Ӧ�ó���1000�������
	{
		funLogP(nLog_File, __FILE__, __LINE__, 10,
					"ִ��SQL���[Select nvl(Black_Version,0),nvl(White_Version,0),to_char(nvl(Terminal_Parm_Time,sysdate-600),'yyyymmddhh24miss') from T_Terminal_Info where Imei_Number='%s' and Uim_Number='%s' and Hotel_ID='%s']����[%d]\n",
					strImei_Number, strUim_Number, strHotel_ID, nReturn);
		return 1;
	}

	//2. ���ҾƵ�ĺ������汾��
	nReturn = pOracle->ExecSQL("Select Update_Time from T_Black_White_Version where Org_ID=:s1 and Black_White_Type='1'",
				strHotel_ID, &nBlack_Version_Real);
	if (nReturn < 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"ִ��SQL���[Select Update_Time from T_Black_White_Version where Org_ID='%s' and Black_White_Type='1']����[%d]��������Ϣ:%s\n",
					strHotel_ID, nReturn, pOracle->getErrorInfo());
		return -1;
	}
	else if (nReturn == 1000)	//�Ƶ����û�к�����
	{
		funLogP(nLog_File, __FILE__, __LINE__, 15,
					"ִ��SQL���[Select Update_Time from T_Black_White_Version where Org_ID='%s' and Black_White_Type='1']����[%d]\n",
					strHotel_ID, nReturn);
		nBlack_Version_Real = 0;
	}


	//3. ����ƽ̨�������汾��
	nReturn = pOracle->ExecSQL("Select Update_Time from T_Black_White_Version where Black_White_Type='0'",
								&nWhite_Version_Real);
	if (nReturn < 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"ִ��SQL���[Select Update_Time from T_Black_White_Version where Black_White_Type='0']����[%d]��������Ϣ:%s\n",
					nReturn, pOracle->getErrorInfo());
		return -1;
	}
	else if (nReturn == 1000)	//ƽ̨����û�а�����
	{
		funLogP(nLog_File, __FILE__, __LINE__, 15,
					"ִ��SQL���[Select Update_Time from T_Black_White_Version where Black_White_Type='0']����[%d]\n",
					nReturn);
		nWhite_Version_Real = 0;
	}

	//4. �����ն˲��������޸�ʱ��
	nReturn = pOracle->ExecSQL("Select to_char(Modify_Time,'yyyymmddhh24miss') from T_Terminal_Parm where Available='1' and (Imei_Number='*' or Imei_Number=:s1)",
				strImei_Number, strTerminal_Parm_Time_Real);
	if (nReturn < 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"ִ��SQL���[Select to_char(Modify_Time,'yyyymmddhh24miss') from T_Terminal_Parm where Available='1' and (Imei_Number='*' or Imei_Number='%s')]����[%d]��������Ϣ:%s\n",
					strImei_Number, nReturn, pOracle->getErrorInfo());
		return -1;
	}
	else if (nReturn == 1000)	//ƽ̨����û�����ø��ն˵Ĳ�����Ϣ
	{
		funLogP(nLog_File, __FILE__, __LINE__, 15,
					"ִ��SQL���[Select to_char(Modify_Time,'yyyymmddhh24miss') from T_Terminal_Parm where Available='1' and (Imei_Number='*' or Imei_Number='%s')]����[%d]\n",
					strImei_Number, nReturn);
		strTerminal_Parm_Time_Real[0] = 0;
	}

	//5. �鿴�����������Ƿ��Ѿ�ʧЧ
	if (nBlack_Version_Real > nBlack_Version_DB)
	{
		*pnBlack_Is_Old = 1;
	}
	else
	{
		*pnBlack_Is_Old = 0;
	}

	if (nWhite_Version_Real > nWhite_Version_DB)
	{
		*pnWhite_Is_Old = 1;
	}
	else
	{
		*pnWhite_Is_Old = 0;
	}

	if (strcmp(strTerminal_Parm_Time_Real, strTerminal_Parm_Time_DB) >= 0)
	{
		*pnTerminal_Parm_Is_Old = 1;
	}
	else
	{
		*pnTerminal_Parm_Is_Old = 0;
	}

	return 0;
}

/**********************************************************************
 ��������: funUpdate_Terminal_Black_White_Version
 ��������: �������ݿ��м�¼���·����ն˵ĺڰ������汾(ע��:��������һ������)
 �������:
	pOracle-���ݿ�����ָ��
	strImei_Number-�ն˵�IMEI����
	strUim_Number-�ն˵�UIM����
	nBlack_Version-�������汾��
	nWhite_Version-�������汾��
	nLog_File-Ҫд����־�ļ�
 �������:
 �� �� ֵ: ��

 ȫ�ֱ���:
 ��д����: mahezhou
 ��д����: 2009��2��16��
 �޸�����:
 �޸�����:
 �޸�Ŀ��:
**********************************************************************/
void funUpdate_Terminal_Black_White_Version(OLFO *pOracle, char *strImei_Number, char *strUim_Number,
			int nBlack_Version, int nWhite_Version, int nLog_File)
{
	int		nReturn;

	//1. ���º�����
	if (nBlack_Version != -100)
	{
		nReturn = pOracle->ExecSQL("Update T_Terminal_Info set Black_Version=:n1 where Imei_Number=:s1 and Uim_Number=:s2",
						nBlack_Version, strImei_Number, strUim_Number);
		if (nReturn >1)		//���ݳ�����������
		{
			funLogP(nLog_File, __FILE__, __LINE__, 5,
					"ִ��SQL���[Update T_Terminal_Info set Black_Version=%d where Imei_Number='%s' and Uim_Number='%s']����[%d]\n",
					nBlack_Version, strImei_Number, strUim_Number,
					nReturn);
			pOracle->RollBack();
		}
		else if (nReturn < 0)
		{
			funLogP(nLog_File, __FILE__, __LINE__, 5,
					"ִ��SQL���[Update T_Terminal_Info set Black_Version=%d where Imei_Number='%s' and Uim_Number='%s']����[%d]��������Ϣ:%s\n",
					nBlack_Version, strImei_Number, strUim_Number,
					nReturn, pOracle->getErrorInfo());
			pOracle->RollBack();
		}
		else
		{
			pOracle->Commit();
		}
	}

	//2. ���°�����
	if (nWhite_Version != -100)
	{
		nReturn = pOracle->ExecSQL("Update T_Terminal_Info set White_Version=:n1 where Imei_Number=:s1 and Uim_Number=:s2",
						nWhite_Version, strImei_Number, strUim_Number);
		if (nReturn >1)		//���ݳ�����������
		{
			funLogP(nLog_File, __FILE__, __LINE__, 5,
					"ִ��SQL���[Update T_Terminal_Info set White_Version=%d where Imei_Number='%s' and Uim_Number='%s']����[%d]\n",
					nWhite_Version, strImei_Number, strUim_Number,
					nReturn);
			pOracle->RollBack();
		}
		else if (nReturn < 0)
		{
			funLogP(nLog_File, __FILE__, __LINE__, 5,
					"ִ��SQL���[Update T_Terminal_Info set White_Version=%d where Imei_Number='%s' and Uim_Number='%s']����[%d]��������Ϣ:%s\n",
					nWhite_Version, strImei_Number, strUim_Number,
					nReturn, pOracle->getErrorInfo());
			pOracle->RollBack();
		}
		else
		{
			pOracle->Commit();
		}
	}
}


/**********************************************************************
 ��������: funPlatform_Send_Black
 ��������: ��ָ���ն��·����°汾�ĺ�����
 �������:
	nSocket_ID-��������ID
	pOracle-���ݿ�����ָ��
	strHotel_ID-�Ƶ�ID
	strImei_Number-�ն˵�IMEI����
	strUim_Number-�ն˵�UIM����
	nLog_File-Ҫд����־�ļ�
 �������:
 �� �� ֵ:
	0		�ɹ�
	1		ʧ��
 	-1		SQLִ���쳣
 ȫ�ֱ���:
 ��д����: mahezhou
 ��д����: 2009��2��16��
 �޸�����:
 �޸�����:
 �޸�Ŀ��:
**********************************************************************/
int	funPlatform_Send_Black(int nSocket_ID, OLFO *pOracle, char *strHotel_ID,
				char *strImei_Number, char *strUim_Number, int nLog_File)
{
	int		nReturn;
	int		nBlack_Version;		//�������汾��
	char	strBlack_List[BLACK_NUM_MAX*(BLACK_WHITE_PHONE_LEN+1)];
	int		nBlack_List_Length;	//�������ֶ�ʵ�ʳ���
	int		nSequence_ID;
	//1. ��ȡ�Ƶ�ĺ�����
	nReturn = funGet_Hotel_Black_List(pOracle, strHotel_ID,
				&nBlack_Version, strBlack_List, nLog_File);
	if (nReturn != 0)
	{
		return nReturn;
	}

	//2. ��װ���º��������ݰ��������ȡ������֡���ˮ���ն�Ωһ��ʶ
	char	strPackage_Body[PACKAGE_LENTGH_MAX];
	memset(strPackage_Body, 0, sizeof(strPackage_Body));
	nBlack_List_Length = strlen(strBlack_List);
	*(int *)(strPackage_Body + 0) = 12+36+4+4+nBlack_List_Length;		//���ĳ���
	*(int *)(strPackage_Body + 4) = PLATFORM_BLACK_UPDATE;				//ƽ̨���еĸ��º�����������
	nSequence_ID = fun_get_SEQ();										//���ɰ�����ˮ��
	*(int *)(strPackage_Body + 8) = nSequence_ID;
	memcpy(strPackage_Body+12, strImei_Number, 15);						//�ն�IMEI
	memcpy(strPackage_Body+27, strUim_Number, 15);						//�ն�UIM
	*(int *)(strPackage_Body + 48) = nBlack_Version;					//�������汾��
	*(int *)(strPackage_Body + 52) = nBlack_List_Length;				//�������ֶγ���
	memcpy(strPackage_Body+56, strBlack_List, nBlack_List_Length);		//������

    //3. ���ն˷��ͺ������汾���°�
    nReturn = fun_Write(nSocket_ID, strPackage_Body, 12+36+4+4+nBlack_List_Length, oGateParm.nTermSendDataTime);
    if (nReturn != 0)
    {
		funLogP(nLog_File, __FILE__, __LINE__, 15,
					"nSocket_ID=%d�����ն�[%s,%s]���ͺ��������°����ȴ�ʱ��[%d]������[%d]\n",
					nSocket_ID, strImei_Number, strUim_Number, oGateParm.nTermSendDataTime, nReturn);
		return 1;
	}
	else
    {
		funLogP(nLog_File, __FILE__, __LINE__, 30,
					"nSocket_ID=%d�����ն�[%s,%s]���ͺ��������°����ɹ�\n",
					nSocket_ID, strImei_Number, strUim_Number);
	}


	//4. �ȴ����º������ذ�
	char	strPackage_Resp[512];
	funLogP(nLog_File, __FILE__, __LINE__, 30,
					"nSocket_ID=%d�����и��º�����������ˮΪ[%d]���ն�Ϊ[%s,%s]\n",
					nSocket_ID, nSequence_ID, strImei_Number, strUim_Number);
	nReturn = fun_Read(nSocket_ID, strPackage_Resp,
							sizeof(strPackage_Resp), oGateParm.nTermReadDataTime);
	if (nReturn <= 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 15, "�ȴ�nSocket_ID=%d����[%s,%s]�������ذ���ʧ�ܣ�����fun_Read����[%d]\n",
					nSocket_ID, strImei_Number, strUim_Number, nReturn);
		return 1;
	}
	else
	{
		funLogP(nLog_File, __FILE__, __LINE__, 30, "�ȴ�nSocket_ID=%d����[%s,%s]�������ذ����ɹ�\n",
					nSocket_ID, strImei_Number, strUim_Number);
	}

	//5. �ж��ն˻ظ����Ƿ�����
	if (*(int *)(strPackage_Body + 0) != 52)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 20,
					"nSocket_ID=%d���صĸ���[%s,%s]�������ذ������صİ�����Ϊ[%d]��Ӧ��Ϊ[%d]\n",
					nSocket_ID, strImei_Number, strUim_Number,
					*(int *)(strPackage_Body + 0), 52);
	}

	if (*(int *)(strPackage_Body + 4) != PLATFORM_BLACK_UPDATE_RESP)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 20,
					"nSocket_ID=%d���صĸ���[%s,%s]�������ذ������ص�������Ϊ[0X%08x]��Ӧ��Ϊ[0X%08x]\n",
					nSocket_ID, strImei_Number, strUim_Number,
					*(int *)(strPackage_Body + 4), PLATFORM_BLACK_UPDATE_RESP);
	}

	if (*(int *)(strPackage_Body + 8) != nSequence_ID)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 20,
					"nSocket_ID=%d���صĸ���[%s,%s]�������ذ������ص���ˮΪ[%d]��Ӧ��Ϊ[%d]\n",
					nSocket_ID, strImei_Number, strUim_Number,
					*(int *)(strPackage_Body + 8), nSequence_ID);
	}

	if (memcmp(strPackage_Body+12, strImei_Number, 15) != 0 ||
		memcmp(strPackage_Body+27, strUim_Number, 15) != 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 20,
					"nSocket_ID=%d���صĸ��º������ذ������ص��ն�Ωһ��ʶΪ[%s]��Ӧ��Ϊ[%s,%s]\n",
					nSocket_ID, strPackage_Body+12, strImei_Number, strUim_Number);
	}

	if (*(int *)(strPackage_Body + 48) != 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 15,
					"nSocket_ID=%d���صĸ���[%s,%s]�������ذ������صĽ��Ϊ[%d]��ʧ��\n",
					nSocket_ID, strImei_Number, strUim_Number,
					*(int *)(strPackage_Body + 48), nSequence_ID);
		return 1;
	}
	else
	{
		funLogP(nLog_File, __FILE__, __LINE__, 15,
					"nSocket_ID=%d������[%s,%s]���������ɹ�\n",
					nSocket_ID, strImei_Number, strUim_Number);
		funUpdate_Terminal_Black_White_Version(pOracle, strImei_Number, strUim_Number,
					nBlack_Version, -100, nLog_File);
		return 0;
	}
}

/**********************************************************************
 ��������: funPlatform_Send_White
 ��������: ��ָ���ն��·����°汾�İ�����
 �������:
	nSocket_ID-��������ID
	pOracle-���ݿ�����ָ��
	strImei_Number-�ն˵�IMEI����
	strUim_Number-�ն˵�UIM����
	nLog_File-Ҫд����־�ļ�
 �������:
 �� �� ֵ:
	0		�ɹ�
	1		ʧ��
 	-1		SQLִ���쳣
 ȫ�ֱ���:
 ��д����: mahezhou
 ��д����: 2009��2��16��
 �޸�����:
 �޸�����:
 �޸�Ŀ��:
**********************************************************************/
int	funPlatform_Send_White(int nSocket_ID, OLFO *pOracle,
				char *strImei_Number, char *strUim_Number, int nLog_File)
{
	int		nReturn;
	int		nWhite_Version;		//�������汾��
	char	strWhite_List[WHITE_NUM_MAX*(BLACK_WHITE_PHONE_LEN+1)];
	int		nWhite_List_Length;	//�������ֶ�ʵ�ʳ���
	int		nSequence_ID;
	//1. ��ȡ������
	nReturn = funGet_HAPPY_White_List(pOracle, &nWhite_Version, strWhite_List, nLog_File);
	if (nReturn != 0)
	{
		return nReturn;
	}

	//2. ��װ���°��������ݰ��������ȡ������֡���ˮ���ն�Ωһ��ʶ
	char	strPackage_Body[PACKAGE_LENTGH_MAX];
	memset(strPackage_Body, 0, sizeof(strPackage_Body));
	nWhite_List_Length = strlen(strWhite_List);
	*(int *)(strPackage_Body + 0) = 12+36+4+4+nWhite_List_Length;		//���ĳ���
	*(int *)(strPackage_Body + 4) = PLATFORM_WHITE_UPDATE;				//ƽ̨���еĸ��°�����������
	nSequence_ID = fun_get_SEQ();										//���ɰ�����ˮ��
	*(int *)(strPackage_Body + 8) = nSequence_ID;
	memcpy(strPackage_Body+12, strImei_Number, 15);						//�ն�IMEI
	memcpy(strPackage_Body+27, strUim_Number, 15);						//�ն�UIM
	*(int *)(strPackage_Body + 48) = nWhite_Version;					//�������汾��
	*(int *)(strPackage_Body + 52) = nWhite_List_Length;				//�������ֶγ���
	memcpy(strPackage_Body+56, strWhite_List, nWhite_List_Length);		//������

    //3. ���ն˷��Ͱ������汾���°�
    nReturn = fun_Write(nSocket_ID, strPackage_Body, 12+36+4+4+nWhite_List_Length, oGateParm.nTermSendDataTime);
    if (nReturn != 0)
    {
		funLogP(nLog_File, __FILE__, __LINE__, 15,
					"nSocket_ID=%d�����ն�[%s,%s]���Ͱ��������°����ȴ�ʱ��[%d]������[%d]\n",
					nSocket_ID, strImei_Number, strUim_Number, oGateParm.nTermSendDataTime, nReturn);
		return 1;
	}
	else
    {
		funLogP(nLog_File, __FILE__, __LINE__, 30,
					"nSocket_ID=%d�����ն�[%s,%s]���Ͱ��������°����ɹ�\n",
					nSocket_ID, strImei_Number, strUim_Number);
	}

	//4. �ȴ����°������ذ�
	char	strPackage_Resp[512];
	funLogP(nLog_File, __FILE__, __LINE__, 30,
					"nSocket_ID=%d�����и��°�����������ˮΪ[%d]���ն�Ϊ[%s,%s]\n",
					nSocket_ID, nSequence_ID, strImei_Number, strUim_Number);
	nReturn = fun_Read(nSocket_ID, strPackage_Resp,
							sizeof(strPackage_Resp), oGateParm.nTermReadDataTime);
	if (nReturn <= 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 15, "�ȴ�nSocket_ID=%d���°������ذ���ʧ�ܣ�����fun_Read����[%d]\n",
					nSocket_ID, nReturn);
		return 1;
	}
	else
	{
		funLogP(nLog_File, __FILE__, __LINE__, 15, "�ȴ�nSocket_ID=%d���°������ذ����ɹ�\n",
					nSocket_ID);
	}


	//5. �ж��ն˻ظ����Ƿ�����
	if (*(int *)(strPackage_Body + 0) != 52)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 20,
					"nSocket_ID=%d���صĸ���[%s,%s]�������ذ������صİ�����Ϊ[%d]��Ӧ��Ϊ[%d]\n",
					nSocket_ID, strImei_Number, strUim_Number,
					*(int *)(strPackage_Body + 0), 52);
	}

	if (*(int *)(strPackage_Body + 4) != PLATFORM_WHITE_UPDATE_RESP)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 20,
					"nSocket_ID=%d���صĸ���[%s,%s]�������ذ������ص�������Ϊ[%d]��Ӧ��Ϊ[%d]\n",
					nSocket_ID, strImei_Number, strUim_Number,
					*(int *)(strPackage_Body + 4), PLATFORM_WHITE_UPDATE_RESP);
	}

	if (*(int *)(strPackage_Body + 8) != nSequence_ID)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 20,
					"nSocket_ID=%d���صĸ���[%s,%s]�������ذ������ص���ˮΪ[%d]��Ӧ��Ϊ[%d]\n",
					nSocket_ID, strImei_Number, strUim_Number,
					*(int *)(strPackage_Body + 8), nSequence_ID);
	}

	if (memcmp(strPackage_Body+12, strImei_Number, 15) != 0 ||
		memcmp(strPackage_Body+27, strUim_Number, 15) != 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 20,
					"nSocket_ID=%d���صĸ��°������ذ������ص��ն�Ωһ��ʶΪ[%s]��Ӧ��Ϊ[%s,%s]\n",
					nSocket_ID, strPackage_Body+12, strImei_Number, strUim_Number);
	}

	if (*(int *)(strPackage_Body + 48) != 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 15,
					"nSocket_ID=%d���صĸ���[%s,%s]�������ذ������صĽ��Ϊ[%d]��ʧ��\n",
					nSocket_ID, strImei_Number, strUim_Number,
					*(int *)(strPackage_Body + 48), nSequence_ID);
		return 1;
	}
	else
	{
		funLogP(nLog_File, __FILE__, __LINE__, 15,
					"nSocket_ID=%d������[%s,%s]���������ɹ�\n",
					nSocket_ID, strImei_Number, strUim_Number);
		funUpdate_Terminal_Black_White_Version(pOracle, strImei_Number, strUim_Number,
					-100, nWhite_Version, nLog_File);
		return 0;
	}
}

/**********************************************************************
 ��������: funUpdate_Terminal_Parm_Version
 ��������: �������ݿ��м�¼���·����ն˵��ն˲�������ʱ��(ע��:��������һ������)
 �������:
	pOracle-���ݿ�����ָ��
	strImei_Number-�ն˵�IMEI����
	strUim_Number-�ն˵�UIM����
	strParm_Modify_Time-�����ĸ���ʱ��
	nLog_File-Ҫд����־�ļ�
 �������:
 �� �� ֵ: ��

 ȫ�ֱ���:
 ��д����: mahezhou
 ��д����: 2009��2��16��
 �޸�����:
 �޸�����:
 �޸�Ŀ��:
**********************************************************************/
void funUpdate_Terminal_Parm_Version(OLFO *pOracle, char *strImei_Number, char *strUim_Number,
			char *strParm_Modify_Time, int nLog_File)
{
	int		nReturn;

	nReturn = pOracle->ExecSQL("Update T_Terminal_Info set Terminal_Parm_Time=to_Date(:s1,'yyyymmddhh24miss') where Imei_Number=:s1 and Uim_Number=:s2",
						strParm_Modify_Time, strImei_Number, strUim_Number);
	if (nReturn >1)		//���ݳ�����������
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"ִ��SQL���[Update T_Terminal_Info set Terminal_Parm_Time=to_Date('%s','yyyymmddhh24miss') where Imei_Number='%s' and Uim_Number='%s']����[%d]\n",
					strParm_Modify_Time, strImei_Number, strUim_Number, nReturn);
		pOracle->RollBack();
	}
	else if (nReturn < 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"ִ��SQL���[Update T_Terminal_Info set Terminal_Parm_Time=to_Date('%s','yyyymmddhh24miss') where Imei_Number='%s' and Uim_Number='%s']����[%d]��������Ϣ:%s\n",
					strParm_Modify_Time, strImei_Number, strUim_Number,
					nReturn, pOracle->getErrorInfo());
		pOracle->RollBack();
	}
	else
	{
		pOracle->Commit();
	}
}

/**********************************************************************
 ��������: funPlatform_Send_Terminal_Parm
 ��������: ��ָ���ն��·����µĲ���
 �������:
	nSocket_ID-��������ID
	pOracle-���ݿ�����ָ��
	strImei_Number-�ն˵�IMEI����
	strUim_Number-�ն˵�UIM����
	nLog_File-Ҫд����־�ļ�
 �������:
 �� �� ֵ:
	0		�ɹ�
	1		ʧ��
 	-1		SQLִ���쳣
 ȫ�ֱ���:
 ��д����: mahezhou
 ��д����: 2009��2��16��
 �޸�����:
 �޸�����:
 �޸�Ŀ��:
**********************************************************************/
int	funPlatform_Send_Terminal_Parm(int nSocket_ID, OLFO *pOracle,
				char *strImei_Number, char *strUim_Number, int nLog_File)
{
	int		nReturn;
	char	strParm_Modify_Time[14+1];
	char	strParm_List[1024];
	int		nParm_List_Length;	//�����ֶ�ʵ�ʳ���
	int		nSequence_ID;
	//1. ��ȡ�ն����²���
	nReturn = pOracle->ExecSQL("Select to_char(Modify_Time,'yyyymmddhh24miss'),Parameters from T_Terminal_Parm where Available='1' and (Imei_Number='*' or Imei_Number=:s1)",
				strImei_Number, strParm_Modify_Time, strParm_List);
	if (nReturn < 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"ִ��SQL���[Select to_char(Modify_Time,'yyyymmddhh24miss'),Parameters from T_Terminal_Parm where Available='1' and (Imei_Number='*' or Imei_Number='%s')]����[%d]��������Ϣ:%s\n",
					strImei_Number, nReturn, pOracle->getErrorInfo());
		return -1;
	}
	else if (nReturn == 1000)	//ƽ̨����û�����ø��ն˵Ĳ�����Ϣ
	{
		funLogP(nLog_File, __FILE__, __LINE__, 15,
					"ִ��SQL���[Select to_char(Modify_Time,'yyyymmddhh24miss'),Parameters from T_Terminal_Parm where Available='1' and (Imei_Number='*' or Imei_Number='%s')]����[%d]\n",
					strImei_Number, nReturn);
		return 1;
	}

	//2. ��װ�����ն˲������ݰ��������ȡ������֡���ˮ���ն�Ωһ��ʶ
	char	strPackage_Body[PACKAGE_LENTGH_MAX];
	memset(strPackage_Body, 0, sizeof(strPackage_Body));
	nParm_List_Length = strlen(strParm_List);
	*(int *)(strPackage_Body + 0) = 12+36+4+nParm_List_Length;		//���ĳ���
	*(int *)(strPackage_Body + 4) = PLATFORM_MODI_PARAMETER;		//ƽ̨���еĸ����ն˲���������
	nSequence_ID = fun_get_SEQ();									//���ɰ�����ˮ��
	*(int *)(strPackage_Body + 8) = nSequence_ID;
	memcpy(strPackage_Body+12, strImei_Number, 15);						//�ն�IMEI
	memcpy(strPackage_Body+27, strUim_Number, 15);						//�ն�UIM
	*(int *)(strPackage_Body + 48) = nParm_List_Length;					//�ն˲����ֶγ���
	memcpy(strPackage_Body+52, strParm_List, nParm_List_Length);		//�ն˲���

    //3. ���ն˷����ն˲������°�
    nReturn = fun_Write(nSocket_ID, strPackage_Body, 12+36+4+nParm_List_Length, oGateParm.nTermSendDataTime);
    if (nReturn != 0)
    {
		funLogP(nLog_File, __FILE__, __LINE__, 15,
					"nSocket_ID=%d�����ն�[%s,%s]�����ն˲������°����ȴ�ʱ��[%d]������[%d]\n",
					nSocket_ID,strImei_Number, strUim_Number, oGateParm.nTermSendDataTime, nReturn);
		return 1;
	}
	else
    {
		funLogP(nLog_File, __FILE__, __LINE__, 30,
					"nSocket_ID=%d�����ն�[%s,%s]�����ն˲������°����ɹ�\n",
					nSocket_ID,strImei_Number, strUim_Number);
	}

	//4. �ȴ������ն˲����ذ�
	char	strPackage_Resp[512];
	funLogP(nLog_File, __FILE__, __LINE__, 30,
					"nSocket_ID=%d�����и����ն˲���������ˮΪ[%d]���ն�Ϊ[%s,%s]\n",
					nSocket_ID, nSequence_ID, strImei_Number, strUim_Number);
	nReturn = fun_Read(nSocket_ID, strPackage_Resp,
							sizeof(strPackage_Resp), oGateParm.nTermReadDataTime);
	if (nReturn <= 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 15, "�ȴ�nSocket_ID=%d�����ն˲����ذ���ʧ�ܣ�����fun_Read����[%d]\n",
					nSocket_ID, nReturn);
		return 1;
	}
	else
	{
		funLogP(nLog_File, __FILE__, __LINE__, 30, "�ȴ�nSocket_ID=%d�����ն˲����ذ����ɹ�\n",
					nSocket_ID);
	}


	//5. �ж��ն˻ظ����Ƿ�����
	if (*(int *)(strPackage_Body + 0) != 52)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 20,
					"nSocket_ID=%d���صĸ���[%s,%s]�ն˲����ذ������صİ�����Ϊ[%d]��Ӧ��Ϊ[%d]\n",
					nSocket_ID, strImei_Number, strUim_Number,
					*(int *)(strPackage_Body + 0), 52);
	}

	if (*(int *)(strPackage_Body + 4) != PLATFORM_MODI_PARAMETER_RESP)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 20,
					"nSocket_ID=%d���صĸ���[%s,%s]�ն˲����ذ������ص�������Ϊ[%d]��Ӧ��Ϊ[%d]\n",
					nSocket_ID, strImei_Number, strUim_Number,
					*(int *)(strPackage_Body + 4), PLATFORM_MODI_PARAMETER_RESP);
	}

	if (*(int *)(strPackage_Body + 8) != nSequence_ID)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 20,
					"nSocket_ID=%d���صĸ���[%s,%s]�ն˲����ذ������ص���ˮΪ[%d]��Ӧ��Ϊ[%d]\n",
					nSocket_ID, strImei_Number, strUim_Number,
					*(int *)(strPackage_Body + 8), nSequence_ID);
	}

	if (memcmp(strPackage_Body+12, strImei_Number, 15) != 0 ||
		memcmp(strPackage_Body+27, strUim_Number, 15) != 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 20,
					"nSocket_ID=%d���صĸ����ն˲����ذ������ص��ն�Ωһ��ʶΪ[%s]��Ӧ��Ϊ[%s,%s]\n",
					nSocket_ID, strPackage_Body+12, strImei_Number, strUim_Number);
	}

	if (*(int *)(strPackage_Body + 48) != 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 15,
					"nSocket_ID=%d���صĸ���[%s,%s]�ն˲����ذ������صĽ��Ϊ[%d]��ʧ��\n",
					nSocket_ID, strImei_Number, strUim_Number,
					*(int *)(strPackage_Body + 48), nSequence_ID);
		return 1;
	}
	else
	{
		funLogP(nLog_File, __FILE__, __LINE__, 30,
					"nSocket_ID=%d������[%s,%s]�ն˲������ɹ�\n",
					nSocket_ID, strImei_Number, strUim_Number);
		funUpdate_Terminal_Parm_Version(pOracle, strImei_Number, strUim_Number,
					strParm_Modify_Time, nLog_File);
		return 0;
	}
}

/**********************************************************************
 ��������: funPlatform_Send_Message_To_Terminal
 ��������: �ն˷�������Ͽ���֮�󣬰����ն˵���������ն���������
 �������:
	nSocket_ID-��������ID
	pOracle-���ݿ�����ָ��
	pTerminal_Package-�Ͽ�������ṹָ��
	pTerminal_Attri-�ն����Խṹָ��
	nLog_File-Ҫд����־�ļ�
 �������:
 �� �� ֵ:
	0		�ɹ�
	1		ʧ��
 ȫ�ֱ���:
 ��д����: mahezhou
 ��д����: 2009��2��16��
 �޸�����:
 �޸�����:
 �޸�Ŀ��:
**********************************************************************/
int	funPlatform_Send_Message_To_Terminal(int nSocket_ID, OLFO *pOracle, stTerminal_Package *pTerminal_Package,
			stTerminal_Attri *pTerminal_Attri, int nLog_File)
{
	int		nReturn;
	int		nWhite_Is_Old;
	int		nBlack_Is_Old;
	int		nTerminal_Parm_Is_Old;
	//1.�ж��ն˵ĺڰ������汾�������汾�Ƿ������µ�
	nReturn = funTerminal_Data_IS_Old(pOracle, pTerminal_Attri->strHotel_ID, pTerminal_Attri->strImei_Number,
						pTerminal_Attri->strUim_Number, &nBlack_Is_Old, &nWhite_Is_Old,
						&nTerminal_Parm_Is_Old, nLog_File);
	if (nReturn == -1 || nReturn == 1)
	{
		return nReturn;
	}

	//2. ��Ҫ�����ն˵ĺ�����
	if (nBlack_Is_Old == 1)
	{
		nReturn = funPlatform_Send_Black(nSocket_ID, pOracle, pTerminal_Attri->strHotel_ID,
										pTerminal_Attri->strImei_Number,
										pTerminal_Attri->strUim_Number, nLog_File);
		if (nReturn == -1)
		{
			return nReturn;
		}
	}

	//3. ��Ҫ�����ն˵İ�����
	printf("nWhite_Is_Old=%d\n", nWhite_Is_Old);
	if (nWhite_Is_Old == 1)
	{
		nReturn = funPlatform_Send_White(nSocket_ID, pOracle,
										pTerminal_Attri->strImei_Number,
										pTerminal_Attri->strUim_Number, nLog_File);
		if (nReturn == -1)
		{
			return nReturn;
		}
	}

	//3. ��Ҫ�����ն˲���
	printf("nWhite_Is_Old=%d\n", nWhite_Is_Old);

	if (nWhite_Is_Old == 1)
	{
		nReturn = funPlatform_Send_Terminal_Parm(nSocket_ID, pOracle,
										pTerminal_Attri->strImei_Number,
										pTerminal_Attri->strUim_Number, nLog_File);
		if (nReturn == -1)
		{
			return nReturn;
		}
	}

	return 0;

}


/**********************************************************************
 ��������: funTerminal_Disconnect_Deal
 ��������: �����ն˷���ĶϿ������
 �������:
	nSocket_ID-��������ID
	pOracle-���ݿ�����ָ��
	pTerminal_Package-�Ͽ�������ṹָ��
	pTerminal_Attri-�ն����Խṹָ��
	nLog_File-Ҫд����־�ļ�
 �������:
 �� �� ֵ:
	0		�ɹ�
	1		ʧ��
 ȫ�ֱ���:
 ��д����: mahezhou
 ��д����: 2009��2��15��
 �޸�����:
 �޸�����:
 �޸�Ŀ��:
**********************************************************************/
int	funTerminal_Disconnect_Deal(int nSocket_ID, OLFO *pOracle, stTerminal_Package *pTerminal_Package,
			stTerminal_Attri *pTerminal_Attri, int nLog_File)
{
	char	strPackage_Body[PACKAGE_LENTGH_MAX];
	int		nReturn_Deal;
	int		nReturn;

	memset(strPackage_Body, 0, sizeof(strPackage_Body));
	//1. �����ȡ������֡���ˮ���ն�Ωһ��ʶ
	*(int *)(strPackage_Body + 0) = 48;									//���ĳ��ȣ��̶���56�ֽ�
	*(int *)(strPackage_Body + 4) = TERMINAL_GET_TIME_RESP;				//ʱ��ͬ���ذ�������
	*(int *)(strPackage_Body + 8) = pTerminal_Package->nSequence_ID;	//������ˮ��
	memcpy(strPackage_Body+12, pTerminal_Package->strTerminal_Number, 36);	//�ն�Ωһ��ʶ

	//2. ���ն�������Ҫ������
	nReturn_Deal = funPlatform_Send_Message_To_Terminal(nSocket_ID, pOracle, pTerminal_Package,
								pTerminal_Attri, nLog_File);
	if (nReturn_Deal == 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 30,
					"nSocket_ID=%d�����ն�[%s]�������ݳɹ�\n",
					nSocket_ID, pTerminal_Package->strTerminal_Number);
	}
	else
	{
		funLogP(nLog_File, __FILE__, __LINE__, 20,
					"nSocket_ID=%d�����ն�[%s]��������ʧ�ܣ�����[%d]\n",
					nSocket_ID, pTerminal_Package->strTerminal_Number, nReturn_Deal);
	}

    //3. ���ն˻�����Ͽ���
    nReturn = fun_Write(nSocket_ID, strPackage_Body, 48, oGateParm.nTermSendDataTime);
	if (nReturn_Deal == -1)
	{
		 if (nReturn != 0)
		 {
			funLogP(nLog_File, __FILE__, __LINE__, 15,
					"���ն�[%s]��������Ͽ��ذ����ȴ�ʱ��[%d]������[%d]\n",
					pTerminal_Package->strTerminal_Number, oGateParm.nTermSendDataTime, nReturn);
		}
		return nReturn_Deal;
	}
	else if (nReturn != 0)
    {
		funLogP(nLog_File, __FILE__, __LINE__, 15,
					"���ն�[%s]��������Ͽ��ذ����ȴ�ʱ��[%d]������[%d]\n",
					pTerminal_Package->strTerminal_Number, oGateParm.nTermSendDataTime, nReturn);
		return 1;
	}
	else
	{
		return 0;
	}
}

/**********************************************************************
 ��������: funTerminal_Disconnect_Main
 ��������: �����ն˷���ĶϿ������
 �������:
	nSocket_ID-��������ID
	pOracle-���ݿ�����ָ��
	pTerminal_Package-�ն����ݰ�ָ��
	pTerminal_Attri-�ն����Խṹָ��
	nLog_File-Ҫд����־�ļ�
 �������:
 �� �� ֵ:
	0		�ɹ�
	1		ʧ��
 	-1		SQLִ���쳣
 ȫ�ֱ���:
 ��д����: mahezhou
 ��д����: 2009��2��15��
 �޸�����:
 �޸�����:
 �޸�Ŀ��:
**********************************************************************/
int	funTerminal_Disconnect_Main(int nSocket_ID, OLFO *pOracle, stTerminal_Package *pTerminal_Package,
			stTerminal_Attri *pTerminal_Attri, int nLog_File)
{
	int		nReturn;

	nReturn = funTerminal_Disconnect_Deal(nSocket_ID, pOracle, pTerminal_Package, pTerminal_Attri, nLog_File);
	if (nReturn == 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 30,
						"nSocket_ID=%d���Ƶ�[%s]�ն�[%s,%s,%s]�Ͽ����������ɹ�\n",
						nSocket_ID, pTerminal_Attri->strHotel_ID, pTerminal_Attri->strImei_Number,
						pTerminal_Attri->strUim_Number, pTerminal_Attri->strMsisdn);
	}
	else
	{
		funLogP(nLog_File, __FILE__, __LINE__, 20,
						"nSocket_ID=%d���Ƶ�[%s]�ն�[%s,%s,%s]�Ͽ����������ʧ�ܣ�����[%d]\n",
						nSocket_ID, pTerminal_Attri->strHotel_ID, pTerminal_Attri->strImei_Number,
						pTerminal_Attri->strUim_Number, pTerminal_Attri->strMsisdn, nReturn);
	}

	return nReturn;
}

/**********************************************************************
 ��������: funDeal_Terminal_Command_Get_Baseinfo
 ��������: �����ն�Ωһ��ʶ�����ն˻������
 �������:
	nSocket_ID-��������ID
	pOracle-���ݿ�����ָ��
 �������:
 �� �� ֵ:
	0		�ɹ�
	1		ʧ��
 	-1		SQLִ���쳣
 ȫ�ֱ���:
 ��д����: mahezhou
 ��д����: 2009��2��14��
 �޸�����:
 �޸�����:
 �޸�Ŀ��:
**********************************************************************/
int	funDeal_Terminal_Command_Get_Baseinfo(OLFO *pOracle, stTerminal_Package *pTerminal_Package,
			stTerminal_Attri *pTerminal_Attri, int nLog_File)
{
	int		nReturn;
	//1. �����ն��������ݰ��е�IMEI��UIM����
	nReturn = funParse_Terminal_Number(pTerminal_Package, pTerminal_Attri, nLog_File);
	if (nReturn != 0)
	{
		return nReturn;
	}

	//2. ȡ�ն˵��ֻ����롢�����Ƶꡢ�ն�״̬��Ԥ��ͨ�����ڷ���������ͨʱ�䡢�Ƶ�״̬���ڰ���������������ʱ���
	nReturn = funGet_Terminal_Attri(pOracle, pTerminal_Attri, nLog_File);
	if (nReturn != 0)
	{
		return nReturn;
	}

	//3. ����Ƶ�Ϊ�գ���Ͽ�(�������Ӧ�ò�����֣����ǰ�û�зַ����Ƶ���ն˿�����¼����ƽ̨)
	if (strlen(pTerminal_Attri->strHotel_ID) == 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 10, "�ն�[%s]�����Ƶ�Ϊ�գ�����[%s]\n",
					pTerminal_Package->strTerminal_Number,
					pTerminal_Package->strPackage_Body);
		return HOTEL_ID_IS_NULL;
	}

	//4. ����Ƶ�״̬�쳣����Ͽ�(�������Ӧ�ò�����֣�������֣���˵��ϵͳ�����쳣)
	if (pTerminal_Attri->strHotel_Status[0] != '0')
	{
		funLogP(nLog_File, __FILE__, __LINE__, 10, "�ն�[%s]�����Ƶ�[%s]״̬Ϊ[%s]���쳣������[%s]\n",
					pTerminal_Package->strTerminal_Number, pTerminal_Attri->strHotel_ID,
					pTerminal_Attri->strHotel_Status, pTerminal_Package->strPackage_Body);
		return HOTEL_STATUS_IS_INVALID;
	}
	return 0;
}


/**********************************************************************
 ��������: funDeal_Terminal_Command_Main
 ��������: �����ն����ӵ�ƽ̨������
 �������:
	nSocket_ID-��������ID
	pOracle-���ݿ�����ָ��
 �������:
 �� �� ֵ:
	0		�ɹ�
	1		ʧ��
 	-1		SQLִ���쳣
 ȫ�ֱ���:
 ��д����: mahezhou
 ��д����: 2009��2��12��
 �޸�����:
 �޸�����:
 �޸�Ŀ��:
**********************************************************************/
int	funDeal_Terminal_Command_Main(int nSocket_ID, OLFO *pOracle)
{
	stTerminal_Package suTerminal_Package;	//�ȴ����յ����ݰ�
	stTerminal_Package *pTerminal_Package=&suTerminal_Package;
	int		Command_Counter=0;				//���������
	int		nReturn;
	int		nLog_File;
	nLog_File = 0;

	int		nReturn_Terminal;
	stTerminal_Attri suTerminal_Attri;		//�ն�����ָ��������
	stTerminal_Attri *pTerminal_Attri = &suTerminal_Attri;
	char		strTerminal_Number[36+1];	//��¼�ն˵�һ�η������ն�Ωһ��ʶ���ڶ������Ժ�Ҫ�˶��Ƿ��м��б仯

	while (1)
	{
		Command_Counter++;
		//1.��սṹ�壬���ȴ��ն��������ݰ�
		memset(&suTerminal_Package, 0, sizeof(suTerminal_Package));
		memset(&suTerminal_Attri,   0, sizeof(stTerminal_Attri));
		if (Command_Counter == 2)	//ģ������????????????????
		{
			suTerminal_Package.nLengh = 48;
			suTerminal_Package.nCommand = TERMINAL_QUIT;
			suTerminal_Package.nSequence_ID =200;
			sprintf((char *)suTerminal_Package.strTerminal_Number, "%s%s", "139102325150000", "460013910232515");
			nReturn = 10;
		}
		else if (Command_Counter == 30)	//ģ��������?????????
		{
			suTerminal_Package.nLengh = 52;
			suTerminal_Package.nCommand = TERMINAL_HEARTBEAT;
			suTerminal_Package.nSequence_ID = 100;
			sprintf((char *)suTerminal_Package.strTerminal_Number, "%s%s", "139102325150000", "460013910232515");
			*(int *)(suTerminal_Package.strPackage_Body + 0) = time(NULL);
			nReturn = 10;
		}
		else if (Command_Counter == 30)	//ģ��ʱ��ͬ����?????????
		{
			suTerminal_Package.nLengh = 52;
			suTerminal_Package.nCommand = TERMINAL_GET_TIME;
			suTerminal_Package.nSequence_ID = 100;
			sprintf((char *)suTerminal_Package.strTerminal_Number, "%s%s", "139102325150000", "460013910232515");
			*(int *)(suTerminal_Package.strPackage_Body + 0) = time(NULL);
			nReturn = 10;
		}
		else if (Command_Counter == 30)	//ģ�⻰�ѳ�ֵ��?????????
		{
			suTerminal_Package.nLengh = 52;
			suTerminal_Package.nCommand = TERMINAL_FEE;
			suTerminal_Package.nSequence_ID = 100;
			sprintf((char *)suTerminal_Package.strTerminal_Number, "%s%s", "139102325150000", "460013910232515");
			*(int *)(suTerminal_Package.strPackage_Body + 0) = 315;
			nReturn = 10;
		}
		else if (Command_Counter == 3)	//ģ��ͳ�����ݰ�?????????
		{
			char	strStat_Body[1024];
			int		nStat_Body_Length;
			memset(strStat_Body, 0, sizeof(strStat_Body));
			strcpy(strStat_Body, "ContentV1.0 ProgramV2.0 FormatV3.0");
			nStat_Body_Length = 34;
			strStat_Body[34] = '\n';
			nStat_Body_Length = nStat_Body_Length+1;
			sprintf(strStat_Body+nStat_Body_Length, "20090217010101 menu:/zh/C2 2001");
			nStat_Body_Length = nStat_Body_Length+31;
			strStat_Body[nStat_Body_Length] = '\r';
			strStat_Body[nStat_Body_Length+1] = '\n';
			nStat_Body_Length = nStat_Body_Length+2;
			sprintf(strStat_Body+nStat_Body_Length, "20090217020202 file:/zh/���ĺ���.avi 2009");
			nStat_Body_Length = nStat_Body_Length+41;
			strStat_Body[nStat_Body_Length] = '\r';
			strStat_Body[nStat_Body_Length+1] = '\n';
			nStat_Body_Length = nStat_Body_Length+2;
			sprintf(strStat_Body+nStat_Body_Length, "20090217030303 file:/zh/���Ǻ÷��.avi 2010");
			nStat_Body_Length = nStat_Body_Length+43;
			strStat_Body[nStat_Body_Length] = '\n';
			nStat_Body_Length = nStat_Body_Length+1;
			sprintf(strStat_Body+nStat_Body_Length, "20090217040404 file:/zh/���ڵ�·����������.avi 2010");
			nStat_Body_Length = nStat_Body_Length+55;
			suTerminal_Package.nLengh = 52+nStat_Body_Length;
			suTerminal_Package.nCommand = TERMINAL_STATISTICS;
			suTerminal_Package.nSequence_ID = 100;
			sprintf((char *)suTerminal_Package.strTerminal_Number, "%s%s", "139102325150000", "460013910232515");
			*(int *)(suTerminal_Package.strPackage_Body + 0) = nStat_Body_Length;
			memcpy(suTerminal_Package.strPackage_Body+4, strStat_Body, nStat_Body_Length);
			nReturn = 10;
		}
		else if (Command_Counter == 4)	//ģ������Ͽ���?????????
		{
			suTerminal_Package.nLengh = 48;
			suTerminal_Package.nCommand = TERMINAL_DISCONNECT;
			suTerminal_Package.nSequence_ID = 100;
			sprintf((char *)suTerminal_Package.strTerminal_Number, "%s%s", "139102325150000", "460013910232515");
			nReturn = 10;
		}
		else
		{
			nReturn = fun_Read(nSocket_ID, (char *)&suTerminal_Package,
							sizeof(suTerminal_Package), oGateParm.nTermReadDataTime);
		}
		if (nReturn <= 0)
		{
			funLogP(nLog_File, __FILE__, __LINE__, 15, "�ȴ�nSocket_ID=%d�������ݣ�ʧ�ܣ�����fun_Read����[%d]\n",
					nSocket_ID, nReturn);
			return 1;
		}

		if (Command_Counter == 1)
		{
			memcpy(strTerminal_Number, suTerminal_Package.strTerminal_Number, 36);
			strTerminal_Number[36] = 0;
		}
		else if (memcmp(strTerminal_Number, suTerminal_Package.strTerminal_Number, 36) != 0)
		{
			funLogP(nLog_File, __FILE__, __LINE__, 15,
					"nSocket_ID=%d���͵ĵ�%d�����ݰ����ն�Ωһ��ʶ����[%s]����һ������Ωһ��ʶ��[%s]��ϵͳ�ܾ�Ϊ֮����\n",
					nSocket_ID, Command_Counter, suTerminal_Package.strTerminal_Number, strTerminal_Number);
			return 1;
		}

		//2.�������ն˵�IMEI��UIM�������Ҷ�Ӧ���ֻ����롢�Ƶꡢ״̬����ͨʱ��Ȼ�����Ϣ
		nReturn = funDeal_Terminal_Command_Get_Baseinfo(pOracle, pTerminal_Package, pTerminal_Attri, nLog_File);
		if (nReturn != 0)
		{
			return nReturn;
		}

		//3. ���������ֽ��в�ͬ�Ĵ���
		switch	(pTerminal_Package->nCommand)
		{
			case TERMINAL_GET_STATUS:		//�ն�״̬ͬ��
				if (Command_Counter == 1)
				{
					nLog_File = 1;
				}
				nReturn_Terminal = funTerminal_Get_Status_Main(nSocket_ID, pOracle, pTerminal_Package,
													pTerminal_Attri, nLog_File);
				break;
			case TERMINAL_OPEN:				//�ն˷���ͨ
				if (Command_Counter == 1)
				{
					nLog_File = 2;
				}
				nReturn_Terminal = funTerminal_Open_Main(nSocket_ID, pOracle, pTerminal_Package,
													pTerminal_Attri, nLog_File);
				break;
			case TERMINAL_QUIT:				//�ն˷�������
				if (Command_Counter == 1)
				{
					nLog_File = 3;
				}
				nReturn_Terminal = funTerminal_Quit_Main(nSocket_ID, pOracle, pTerminal_Package,
													pTerminal_Attri, nLog_File);
				break;
			case TERMINAL_HEARTBEAT:		//�ն�������
				if (Command_Counter == 1)
				{
					nLog_File = 4;
				}
				nReturn_Terminal = funTerminal_Heartbeat_Main(nSocket_ID, pOracle, pTerminal_Package,
													pTerminal_Attri, nLog_File);
				break;
			case TERMINAL_STATISTICS:		//�ն˷���ͳ������
				if (Command_Counter == 1)
				{
					nLog_File = 5;
				}
				nReturn_Terminal = funTerminal_Statistics_Main(nSocket_ID, pOracle, pTerminal_Package,
													pTerminal_Attri, nLog_File);
				break;

			case TERMINAL_FEE:				//���ѳ�ֵ(һ�ڲ���)
				if (Command_Counter == 1)
				{
					nLog_File = 6;
				}
				funLogP(0, __FILE__, __LINE__, 15, "�յ��ն�[%s]�Ļ��ѳ�ֵ��������[%s]\n",
						pTerminal_Package->strTerminal_Number,
						pTerminal_Package->strPackage_Body);
				break;
			case TERMINAL_IPFEE:			//IP��ֵ(һ�ڲ���)
				if (Command_Counter == 1)
				{
					nLog_File = 7;
				}
				funLogP(0, __FILE__, __LINE__, 15, "�յ��ն�[%s]��IP��ֵ��������[%s]\n",
						pTerminal_Package->strTerminal_Number,
						pTerminal_Package->strPackage_Body);
				break;
			case TERMINAL_GET_TIME:			//�ն�ʱ��ͬ��
				if (Command_Counter == 1)
				{
					nLog_File = 8;
				}
				nReturn_Terminal = funTerminal_Get_Time_Main(nSocket_ID, pOracle, pTerminal_Package,
													pTerminal_Attri, nLog_File);
				break;
			case TERMINAL_DISCONNECT:		//�ն�����Ͽ�
				if (Command_Counter == 1)	//�������Ӧ�ò����ڰɣ�
				{
					nLog_File = 0;
					funLogP(0, __FILE__, __LINE__, 15, "�ն�[%s]���ӵ�ƽ̨�ĵ�һ��ָ���������Ͽ����Ƚ����\n",
						pTerminal_Package->strTerminal_Number);
				}
				nReturn_Terminal = funTerminal_Disconnect_Main(nSocket_ID, pOracle, pTerminal_Package,
													pTerminal_Attri, nLog_File);
				break;
			default:						//δԤ�ϵ��İ�
				funLogP(0, __FILE__, __LINE__, 15, "�յ��ն�[%s]��������Ϊ[%d]�����ݰ�������[%s]\n",
						pTerminal_Package->strTerminal_Number, pTerminal_Package->nCommand,
						pTerminal_Package->strPackage_Body);
		}
	}
	return nReturn_Terminal;
}

int main_1(int argc, char *argv[])
{
	int		nReturn;
	OLFO	myOracle = OLFO(OCI_THREADED);
	OLFO	*pOracle=&myOracle;
	nReturn = myOracle.Connect("happy", "happy", "carsmart");
	if	(nReturn == 0)
	{
		printf("�������ݿ�ɹ�\n");
	}
	else
	{
		printf("�������ݿ�ʧ��,��Ϣ����:\n\t�������=[%d]\n\t������Ϣ=%s\n",
					myOracle.getErrorNo(), myOracle.getErrorInfo());
		exit(-1);
	}

	/*int		nBlack_Version=-1;
	char	strHotel_ID[10+1]="RJ1";
	char	strBlack_List[30*16];
	nReturn = funGet_Hotel_Black_List(pOracle, strHotel_ID, &nBlack_Version, strBlack_List, 1);
	if (nReturn != 0)
	{
		printf("��ȡ�Ƶ�[%s]�ĺ��������ʧ��,����[%d].\n", strHotel_ID, nReturn);
	}
	else
	{
		printf("�Ƶ�[%s]�ĺ������汾����[%d],�������嵥��%s.\n", strHotel_ID, nBlack_Version, strBlack_List);
	}

	int		nWhite_Version=-1;
	char	strWhite_List[30*16];
	nReturn = funGet_HAPPY_White_List(pOracle, &nWhite_Version, strWhite_List, 1);
	if (nReturn != 0)
	{
		printf("��ȡƽ̨���������ʧ��,����[%d].\n", nReturn);
	}
	else
	{
		printf("ƽ̨�İ������汾����[%d],�������嵥��%s.\n", nWhite_Version, strWhite_List);
	}

	for (int i=0; i<=LANGUAGE_LIST_MAX; i++)
	{
		printf("Language=%d,[%s]\n", i, strArray_Language[i]);
	}*/

	stTerminal_Package	suTerminal_Package;
	/*1.״̬ͬ��
	memset(&suTerminal_Package, 0, sizeof(suTerminal_Package));
	suTerminal_Package.nLengh = 68;
	suTerminal_Package.nCommand = TERMINAL_GET_STATUS;
	suTerminal_Package.nSequence_ID = 123;
	sprintf((char *)suTerminal_Package.strTerminal_Number, "%s%s", "139102325110000", "460013910232511");
	*(int *)(suTerminal_Package.strPackage_Body +  0) = 0;
	*(int *)(suTerminal_Package.strPackage_Body +  4) = time(NULL);
	*(int *)(suTerminal_Package.strPackage_Body +  8) = 102;
	*(int *)(suTerminal_Package.strPackage_Body + 12) = 203;
	*(int *)(suTerminal_Package.strPackage_Body + 16) = 2;
	nReturn = funDeal_Terminal_Command_Main(1, pOracle);*/

	//2.�ն˿�ͨ����
	memset(&suTerminal_Package, 0, sizeof(suTerminal_Package));
	suTerminal_Package.nLengh = 52;
	suTerminal_Package.nCommand = TERMINAL_OPEN;
	suTerminal_Package.nSequence_ID = 234;
	sprintf((char *)suTerminal_Package.strTerminal_Number, "%s%s", "139102325150000", "460013910232515");
	*(int *)(suTerminal_Package.strPackage_Body +  0) = time(NULL);
	nReturn = funDeal_Terminal_Command_Main(1, pOracle);

	/*3.�ն�����
	memset(&suTerminal_Package, 0, sizeof(suTerminal_Package));
	suTerminal_Package.nLengh = 48;
	suTerminal_Package.nCommand = TERMINAL_QUIT;
	suTerminal_Package.nSequence_ID = 345;
	sprintf((char *)suTerminal_Package.strTerminal_Number, "%s%s", "139102325110000", "460013910232511");
	nReturn = funDeal_Terminal_Command_Main(1, pOracle);*/


	/*nReturn = myOracle.ExecSQL("insert into T_Black_White_Version(Black_White_Type,Org_ID,Update_Time) values (1,'hotel',to_date('19700101000100','yyyymmddhh24miss'))");
	printf("nReturn=%d\n", nReturn);
	myOracle.Commit();
	time_t t1=0;
	nReturn = myOracle.ExecSQL("Select Update_Time from  T_Black_White_Version where Org_ID='hotel'", &t1);
	printf("nReturn=%d,t1=%u\n", nReturn,t1);

	char strDay[14+1];
	nReturn = myOracle.ExecSQL("Select to_char(Update_Time,'yyyymmddhh24miss') from  T_Black_White_Version where Org_ID='hotel'", strDay);
	printf("nReturn=%d,strDay=%s\n", nReturn,strDay);*/

	myOracle.Commit();

	myOracle.RollBack();
	myOracle.Close();
	myOracle.DisConnect();
	return 0;
}
