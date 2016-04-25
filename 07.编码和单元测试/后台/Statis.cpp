#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "/opt/happy/src/lib/ORACLE_OPER.H"

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

/**********************************************************************
 ��������: funStatis_Terminal_Useable
 ��������: �����Ƶ��ն˿��������ͳ��
 �������:
	pOracle-���ݿ�����ָ��
	nLog_File-Ҫд����־�ļ�
 �������: ��
 �� �� ֵ:
	0		ͳ�Ƴɹ�
 	-1		ͳ��ʧ��
 ȫ�ֱ���:
 ��д����: mahezhou
 ��д����: 2009��2��2��
 �޸�����:
 �޸�����:
 �޸�Ŀ��:
**********************************************************************/
int funStatis_Terminal_Useable(OLFO *pOracle, int nLog_File)
{
	int		nReturn;
	char	strYear_Month_Day[8+1];
	int		nCount = 0;
	//1. ȡϵͳ���ڵ�ǰһ�죬Ҫͳ�Ƶľ����ն�ǰһ���ʹ�����
	nReturn = pOracle->ExecSQL("Select to_char(sysdate-1,'yyyymmdd') from Dual", strYear_Month_Day);
	if (nReturn != 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"ִ��SQL���[Select to_char(sysdate-1,'yyyymmdd') from Dual]����[%d]��������Ϣ:%s\n",
					nReturn, pOracle->getErrorInfo());
		return -1;
	}

	//2. �鿴ͳ�Ʊ����Ƿ��Ѿ������������
	nReturn = pOracle->ExecSQL("Select count(*) from TR_Terminal_Status where Statis_Date=:s1",
				strYear_Month_Day, &nCount);
	if (nReturn != 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"ִ��SQL���[Select count(*) from TR_Terminal_Status where Statis_Date='%s']����[%d]��������Ϣ:%s\n",
					strYear_Month_Day, nReturn, pOracle->getErrorInfo());
		return -1;
	}

	//3. ���Ҫͳ�Ƶ������Ѿ�ͳ�ƹ��ˣ�����Ҫ��ͳ�ƣ��˳��˺���
	if (nCount > 0)
	{
		return 0;
	}

	//4. ͳ�Ƹ����Ƶ��ն˳�����
	nReturn = pOracle->ExecSQL("Insert Into TR_Terminal_Status(Hotel_ID,Terminal_Status,Terminal_Count,Statis_Date,Statis_Time) Select Hotel_ID,Status,count(*),:s1,sysdate From T_Terminal_Info Group by Hotel_ID,Status",
				strYear_Month_Day);
	if (nReturn < 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"ִ��SQL���[Insert Into TR_Terminal_Status(Hotel_ID,Terminal_Status,Terminal_Count,Statis_Date,Statis_Time) Select Hotel_ID,Status,count(*),'%s',sysdate From T_Terminal_Info Group by Hotel_ID,Status]����[%d]��������Ϣ:%s\n",
					strYear_Month_Day, nReturn, pOracle->getErrorInfo());
		pOracle->RollBack();
		return -1;
	}

	pOracle->Commit();
	return 0;
}

/**********************************************************************
 ��������: funStatis_Terminal_Hired
 ��������: �ն˳����ʡ�ʹ������ͳ��
 �������:
	pOracle-���ݿ�����ָ��
	nLog_File-Ҫд����־�ļ�
 �������: ��
 �� �� ֵ:
	0		ͳ�Ƴɹ�
 	-1		ͳ��ʧ��
 ȫ�ֱ���:
 ��д����: mahezhou
 ��д����: 2009��2��2��
 �޸�����:
 �޸�����:
 �޸�Ŀ��:
**********************************************************************/
int funStatis_Terminal_Hired(OLFO *pOracle, int nLog_File)
{
	int		nReturn;
	char	strYear_Month_Day[8+1];
	int		nCount = 0;
	//1. ȡϵͳ���ڵ�ǰһ�죬Ҫͳ�Ƶľ����ն�ǰһ���ʹ�����
	nReturn = pOracle->ExecSQL("Select to_char(sysdate-1,'yyyymmdd') from Dual", strYear_Month_Day);
	if (nReturn != 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"ִ��SQL���[Select to_char(sysdate-1,'yyyymmdd') from Dual]����[%d]��������Ϣ:%s\n",
					nReturn, pOracle->getErrorInfo());
		return -1;
	}

	//2. �鿴ͳ�Ʊ����Ƿ��Ѿ������������
	nReturn = pOracle->ExecSQL("Select count(*) from TR_Terminal_Hired where Statis_Date=:s1",
				strYear_Month_Day, &nCount);
	if (nReturn != 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"ִ��SQL���[Select count(*) from TR_Terminal_Hired where Statis_Date='%s']����[%d]��������Ϣ:%s\n",
					strYear_Month_Day, nReturn, pOracle->getErrorInfo());
		return -1;
	}

	//3. ���Ҫͳ�Ƶ������Ѿ�ͳ�ƹ��ˣ�����Ҫ��ͳ�ƣ��˳��˺���
	if (nCount > 0)
	{
		return 0;
	}

	//4. ͳ�Ƹ����Ƶ��ն˳�����
	nReturn = pOracle->ExecSQL("Insert Into TR_Terminal_Hired(Hotel_ID,Broker_ID,Hire_Count,Statis_Date,Statis_Time) Select Hotel_ID,Broker_ID,count(*),:s1,sysdate from (select Distinct Hotel_ID,Broker_ID,Imei_Number,Uim_Number from V_SDR where Status=0 and Start_Time<to_date(:s2,'yyyymmdd')+1 or (Status!=0 and to_char(End_Time,'yyyymmdd')=:s3)) Group by Hotel_ID,Broker_ID",
				strYear_Month_Day, strYear_Month_Day, strYear_Month_Day);
	if (nReturn < 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"ִ��SQL���[Insert Into TR_Terminal_Hired(Hotel_ID,Broker_ID,Hire_Count,Statis_Date,Statis_Time) Select Hotel_ID,Broker_ID,count(*),'%s',sysdate from (select Distinct Hotel_ID,Broker_ID,Imei_Number,Uim_Number from V_SDR where Status=0 and Start_Time<to_date('%s','yyyymmdd')+1 or (Status!=0 and to_char(End_Time,'yyyymmdd')='%s')) Group by Hotel_ID,Broker_ID]����[%d]��������Ϣ:%s\n",
					strYear_Month_Day, strYear_Month_Day, strYear_Month_Day, nReturn, pOracle->getErrorInfo());
		pOracle->RollBack();
		return -1;
	}

	//5. ͳ�Ƹ����Ƶ��ն�ʹ��Сʱ��
	nReturn = pOracle->ExecSQL("Insert Into TR_Terminal_Hired(Hotel_ID,Broker_ID,Hire_Seconds,Statis_Date,Statis_Time) Select Hotel_ID,Broker_ID,sum(End_Time-Start_Time)*24*3600,:s1,sysdate From (Select Hotel_ID,Broker_ID,(case when Start_Time<to_date(:s2,'yyyymmdd') then to_date(:s3,'yyyymmdd') else Start_Time end) Start_Time,to_date(:s4,'yyyymmdd')+1 End_Time From V_SDR Where Status=0 and Start_Time<to_date(:s5,'yyyymmdd')+1 union all Select Hotel_ID,Broker_ID,(case when Start_Time<to_date(:s6,'yyyymmdd') then to_date(:s7,'yyyymmdd') else Start_Time end) Start_Time,End_Time From V_SDR Where Status!=0 and to_char(End_Time,'yyyymmdd')=:s8) Group by Hotel_ID,Broker_ID",
				strYear_Month_Day, strYear_Month_Day, strYear_Month_Day, strYear_Month_Day,
				strYear_Month_Day, strYear_Month_Day, strYear_Month_Day, strYear_Month_Day);
	if (nReturn < 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"ִ��SQL���[Insert Into TR_Terminal_Hired(Hotel_ID,Broker_ID,Hire_Seconds,Statis_Date,Statis_Time) Select Hotel_ID,Broker_ID,sum(End_Time-Start_Time)*24*3600,:s1,sysdate From (Select Hotel_ID,Broker_ID,(case when Start_Time<to_date(:s2,'yyyymmdd') then to_date(:s3,'yyyymmdd') else Start_Time end) Start_Time,to_date(:s4,'yyyymmdd')+1 End_Time From V_SDR Where Status=0 and Start_Time<to_date(:s5,'yyyymmdd')+1 union all Select Hotel_ID,Broker_ID,(case when Start_Time<to_date(:s6,'yyyymmdd') then to_date(:s7,'yyyymmdd') else Start_Time end) Start_Time,End_Time From V_SDR Where Status!=0 and to_char(End_Time,'yyyymmdd')=:s8) Group by Hotel_ID,Broker_ID]����[%d]��������Ϣ:%s\n",
					strYear_Month_Day, strYear_Month_Day, strYear_Month_Day, strYear_Month_Day,
					strYear_Month_Day, strYear_Month_Day, strYear_Month_Day, strYear_Month_Day,
					nReturn, pOracle->getErrorInfo());
		pOracle->RollBack();
		return -1;
	}

	pOracle->Commit();
	return 0;
}

/**********************************************************************
 ��������: funStatis_Terminal_Count
 ��������: ͳ���ն���⡢�ַ����ջء������������Լ�������
 �������:
	pOracle-���ݿ�����ָ��
	nLog_File-Ҫд����־�ļ�
 �������: ��
 �� �� ֵ:
	0		ͳ�Ƴɹ�
 	-1		ͳ��ʧ��
 ȫ�ֱ���:
 ��д����: mahezhou
 ��д����: 2009��2��2��
 �޸�����:
 �޸�����:
 �޸�Ŀ��:
**********************************************************************/
int funStatis_Terminal_Count(OLFO *pOracle, int nLog_File)
{
	int		nReturn;
	char	strYear_Month_Day[8+1];
	int		nCount = 0;
	//1. ȡϵͳ���ڵ�ǰһ�죬Ҫͳ�Ƶľ���ǰһ����ն���
	nReturn = pOracle->ExecSQL("Select to_char(sysdate-1,'yyyymmdd') from Dual", strYear_Month_Day);
	if (nReturn != 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"ִ��SQL���[Select to_char(sysdate-1,'yyyymmdd') from Dual]����[%d]��������Ϣ:%s\n",
					nReturn, pOracle->getErrorInfo());
		return -1;
	}

	//2. �鿴ͳ�Ʊ����Ƿ��Ѿ�����������ݣ�ĳ�����û�г���������ݣ������������ݿ϶����еģ�
	//   ʹ��>=��ԭ���ǣ�����ͳ��ǰ�������
	nReturn = pOracle->ExecSQL("Select count(*) from TR_Terminal_In_Out where Statis_Date>=:s1",
				strYear_Month_Day, &nCount);
	if (nReturn != 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"ִ��SQL���[Select count(*) from TR_Terminal_In_Out where Statis_Date>='%s']����[%d]��������Ϣ:%s\n",
					strYear_Month_Day, nReturn, pOracle->getErrorInfo());
		return -1;
	}

	//3. ���Ҫͳ�Ƶ������Ѿ�ͳ�ƹ��ˣ�����Ҫ��ͳ�ƣ��˳��˺���
	if (nCount > 0)
	{
		return 0;
	}

	//4. ͳ��01���;02�ַ�;03�ջ�;04���ϳ���
	nReturn = pOracle->ExecSQL("Insert Into TR_Terminal_In_Out(Statis_Date,Hotel_ID,Change_Type,Terminal_Count,Statis_Time) Select :s1,Hotel_ID,Change_Type,count(*),sysdate from T_Terminal_Change_His where to_char(Change_Time,'yyyymmdd')=:s2 and Change_Type in ('01','02','03','04') Group by Hotel_ID,Change_Type",
				strYear_Month_Day, strYear_Month_Day);
	if (nReturn < 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"ִ��SQL���[Insert Into TR_Terminal_In_Out(Statis_Date,Hotel_ID,Change_Type,Terminal_Count,Statis_Time) Select '%s',Hotel_ID,Change_Type,count(*),sysdate from T_Terminal_Change_His where to_char(Change_Time,'yyyymmdd')='%s' and Change_Type in ('01','02','03','04') Group by Hotel_ID,Change_Type]����[%d]��������Ϣ:%s\n",
					strYear_Month_Day, strYear_Month_Day, nReturn, pOracle->getErrorInfo());
		pOracle->RollBack();
		return -1;
	}

	//5. ͳ���ն˱�����
	nReturn = pOracle->ExecSQL("Insert Into TR_Terminal_In_Out(Statis_Date,Hotel_ID,Change_Type,Terminal_Count,Statis_Time) Select :s1,Hotel_ID,'00',count(*),sysdate from T_Terminal_Info where Status!=6000000 Group by Hotel_ID",
				strYear_Month_Day);
	if (nReturn < 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"ִ��SQL���[Insert Into TR_Terminal_In_Out(Statis_Date,Hotel_ID,Change_Type,Terminal_Count,Statis_Time) Select '%s',Hotel_ID,'00',count(*),sysdate from T_Terminal_Info where Status!=6000000 Group by Hotel_ID]����[%d]��������Ϣ:%s\n",
					strYear_Month_Day, nReturn, pOracle->getErrorInfo());
		pOracle->RollBack();
		return -1;
	}

	pOracle->Commit();
	return 0;
}


int main(int argc, char *argv[])
{
	int		nReturn;
	OLFO	myOracle = OLFO(OCI_THREADED);
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
	nReturn = funStatis_Terminal_Count(&myOracle, 1);
	if (nReturn != 0)
	{
		printf("ͳ���ն�����ʧ��,����%d.\n", nReturn);
	}
	else
	{
		printf("ͳ���ն������ɹ�.\n");
	}

	nReturn = funStatis_Terminal_Hired(&myOracle, 1);
	if (nReturn != 0)
	{
		printf("ͳ���ն�ʹ�����ʧ��,����%d.\n", nReturn);
	}
	else
	{
		printf("ͳ���ն�ʹ������ɹ�.\n");
	}


	nReturn = funStatis_Terminal_Useable(&myOracle, 1);
	if (nReturn != 0)
	{
		printf("ͳ���ն˿�����ʧ��,����%d.\n", nReturn);
	}
	else
	{
		printf("ͳ���ն˿����Գɹ�.\n");
	}


	/*nReturn = myOracle.ExecSQL("insert into T_Black_White_Version(Black_White_Type,Org_ID,Update_Time) values (1,'hotel',to_date('19700101000100','yyyymmddhh24miss'))");
	printf("nReturn=%d\n", nReturn);
	myOracle.Commit();
	time_t t1=0;
	nReturn = myOracle.ExecSQL("Select Update_Time from  T_Black_White_Version where Org_ID='hotel'", &t1);
	printf("nReturn=%d,t1=%u\n", nReturn,t1);
	myOracle.Commit();*/

	myOracle.RollBack();
	myOracle.Close();
	myOracle.DisConnect();


}
