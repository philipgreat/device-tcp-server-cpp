#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "/opt/happy/src/lib/ORACLE_OPER.H"

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

/**********************************************************************
 函数名称: funStatis_Terminal_Useable
 函数功能: 各个酒店终端可用情况日统计
 输入参数:
	pOracle-数据库连接指针
	nLog_File-要写的日志文件
 输出参数: 无
 返 回 值:
	0		统计成功
 	-1		统计失败
 全局变量:
 编写作者: mahezhou
 编写日期: 2009年2月2日
 修改作者:
 修改日期:
 修改目的:
**********************************************************************/
int funStatis_Terminal_Useable(OLFO *pOracle, int nLog_File)
{
	int		nReturn;
	char	strYear_Month_Day[8+1];
	int		nCount = 0;
	//1. 取系统日期的前一天，要统计的就是终端前一天的使用情况
	nReturn = pOracle->ExecSQL("Select to_char(sysdate-1,'yyyymmdd') from Dual", strYear_Month_Day);
	if (nReturn != 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"执行SQL语句[Select to_char(sysdate-1,'yyyymmdd') from Dual]返回[%d]，错误信息:%s\n",
					nReturn, pOracle->getErrorInfo());
		return -1;
	}

	//2. 查看统计表中是否已经有昨天的数据
	nReturn = pOracle->ExecSQL("Select count(*) from TR_Terminal_Status where Statis_Date=:s1",
				strYear_Month_Day, &nCount);
	if (nReturn != 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"执行SQL语句[Select count(*) from TR_Terminal_Status where Statis_Date='%s']返回[%d]，错误信息:%s\n",
					strYear_Month_Day, nReturn, pOracle->getErrorInfo());
		return -1;
	}

	//3. 如果要统计的数据已经统计过了，则不需要再统计，退出此函数
	if (nCount > 0)
	{
		return 0;
	}

	//4. 统计各个酒店终端出租率
	nReturn = pOracle->ExecSQL("Insert Into TR_Terminal_Status(Hotel_ID,Terminal_Status,Terminal_Count,Statis_Date,Statis_Time) Select Hotel_ID,Status,count(*),:s1,sysdate From T_Terminal_Info Group by Hotel_ID,Status",
				strYear_Month_Day);
	if (nReturn < 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"执行SQL语句[Insert Into TR_Terminal_Status(Hotel_ID,Terminal_Status,Terminal_Count,Statis_Date,Statis_Time) Select Hotel_ID,Status,count(*),'%s',sysdate From T_Terminal_Info Group by Hotel_ID,Status]返回[%d]，错误信息:%s\n",
					strYear_Month_Day, nReturn, pOracle->getErrorInfo());
		pOracle->RollBack();
		return -1;
	}

	pOracle->Commit();
	return 0;
}

/**********************************************************************
 函数名称: funStatis_Terminal_Hired
 函数功能: 终端出租率、使用率日统计
 输入参数:
	pOracle-数据库连接指针
	nLog_File-要写的日志文件
 输出参数: 无
 返 回 值:
	0		统计成功
 	-1		统计失败
 全局变量:
 编写作者: mahezhou
 编写日期: 2009年2月2日
 修改作者:
 修改日期:
 修改目的:
**********************************************************************/
int funStatis_Terminal_Hired(OLFO *pOracle, int nLog_File)
{
	int		nReturn;
	char	strYear_Month_Day[8+1];
	int		nCount = 0;
	//1. 取系统日期的前一天，要统计的就是终端前一天的使用情况
	nReturn = pOracle->ExecSQL("Select to_char(sysdate-1,'yyyymmdd') from Dual", strYear_Month_Day);
	if (nReturn != 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"执行SQL语句[Select to_char(sysdate-1,'yyyymmdd') from Dual]返回[%d]，错误信息:%s\n",
					nReturn, pOracle->getErrorInfo());
		return -1;
	}

	//2. 查看统计表中是否已经有昨天的数据
	nReturn = pOracle->ExecSQL("Select count(*) from TR_Terminal_Hired where Statis_Date=:s1",
				strYear_Month_Day, &nCount);
	if (nReturn != 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"执行SQL语句[Select count(*) from TR_Terminal_Hired where Statis_Date='%s']返回[%d]，错误信息:%s\n",
					strYear_Month_Day, nReturn, pOracle->getErrorInfo());
		return -1;
	}

	//3. 如果要统计的数据已经统计过了，则不需要再统计，退出此函数
	if (nCount > 0)
	{
		return 0;
	}

	//4. 统计各个酒店终端出租率
	nReturn = pOracle->ExecSQL("Insert Into TR_Terminal_Hired(Hotel_ID,Broker_ID,Hire_Count,Statis_Date,Statis_Time) Select Hotel_ID,Broker_ID,count(*),:s1,sysdate from (select Distinct Hotel_ID,Broker_ID,Imei_Number,Uim_Number from V_SDR where Status=0 and Start_Time<to_date(:s2,'yyyymmdd')+1 or (Status!=0 and to_char(End_Time,'yyyymmdd')=:s3)) Group by Hotel_ID,Broker_ID",
				strYear_Month_Day, strYear_Month_Day, strYear_Month_Day);
	if (nReturn < 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"执行SQL语句[Insert Into TR_Terminal_Hired(Hotel_ID,Broker_ID,Hire_Count,Statis_Date,Statis_Time) Select Hotel_ID,Broker_ID,count(*),'%s',sysdate from (select Distinct Hotel_ID,Broker_ID,Imei_Number,Uim_Number from V_SDR where Status=0 and Start_Time<to_date('%s','yyyymmdd')+1 or (Status!=0 and to_char(End_Time,'yyyymmdd')='%s')) Group by Hotel_ID,Broker_ID]返回[%d]，错误信息:%s\n",
					strYear_Month_Day, strYear_Month_Day, strYear_Month_Day, nReturn, pOracle->getErrorInfo());
		pOracle->RollBack();
		return -1;
	}

	//5. 统计各个酒店终端使用小时数
	nReturn = pOracle->ExecSQL("Insert Into TR_Terminal_Hired(Hotel_ID,Broker_ID,Hire_Seconds,Statis_Date,Statis_Time) Select Hotel_ID,Broker_ID,sum(End_Time-Start_Time)*24*3600,:s1,sysdate From (Select Hotel_ID,Broker_ID,(case when Start_Time<to_date(:s2,'yyyymmdd') then to_date(:s3,'yyyymmdd') else Start_Time end) Start_Time,to_date(:s4,'yyyymmdd')+1 End_Time From V_SDR Where Status=0 and Start_Time<to_date(:s5,'yyyymmdd')+1 union all Select Hotel_ID,Broker_ID,(case when Start_Time<to_date(:s6,'yyyymmdd') then to_date(:s7,'yyyymmdd') else Start_Time end) Start_Time,End_Time From V_SDR Where Status!=0 and to_char(End_Time,'yyyymmdd')=:s8) Group by Hotel_ID,Broker_ID",
				strYear_Month_Day, strYear_Month_Day, strYear_Month_Day, strYear_Month_Day,
				strYear_Month_Day, strYear_Month_Day, strYear_Month_Day, strYear_Month_Day);
	if (nReturn < 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"执行SQL语句[Insert Into TR_Terminal_Hired(Hotel_ID,Broker_ID,Hire_Seconds,Statis_Date,Statis_Time) Select Hotel_ID,Broker_ID,sum(End_Time-Start_Time)*24*3600,:s1,sysdate From (Select Hotel_ID,Broker_ID,(case when Start_Time<to_date(:s2,'yyyymmdd') then to_date(:s3,'yyyymmdd') else Start_Time end) Start_Time,to_date(:s4,'yyyymmdd')+1 End_Time From V_SDR Where Status=0 and Start_Time<to_date(:s5,'yyyymmdd')+1 union all Select Hotel_ID,Broker_ID,(case when Start_Time<to_date(:s6,'yyyymmdd') then to_date(:s7,'yyyymmdd') else Start_Time end) Start_Time,End_Time From V_SDR Where Status!=0 and to_char(End_Time,'yyyymmdd')=:s8) Group by Hotel_ID,Broker_ID]返回[%d]，错误信息:%s\n",
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
 函数名称: funStatis_Terminal_Count
 函数功能: 统计终端入库、分发、收回、出库数量，以及保有量
 输入参数:
	pOracle-数据库连接指针
	nLog_File-要写的日志文件
 输出参数: 无
 返 回 值:
	0		统计成功
 	-1		统计失败
 全局变量:
 编写作者: mahezhou
 编写日期: 2009年2月2日
 修改作者:
 修改日期:
 修改目的:
**********************************************************************/
int funStatis_Terminal_Count(OLFO *pOracle, int nLog_File)
{
	int		nReturn;
	char	strYear_Month_Day[8+1];
	int		nCount = 0;
	//1. 取系统日期的前一天，要统计的就是前一天的终端数
	nReturn = pOracle->ExecSQL("Select to_char(sysdate-1,'yyyymmdd') from Dual", strYear_Month_Day);
	if (nReturn != 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"执行SQL语句[Select to_char(sysdate-1,'yyyymmdd') from Dual]返回[%d]，错误信息:%s\n",
					nReturn, pOracle->getErrorInfo());
		return -1;
	}

	//2. 查看统计表中是否已经有昨天的数据（某天可能没有出、入库数据，但保有量数据肯定是有的）
	//   使用>=的原因是，不能统计前天的数据
	nReturn = pOracle->ExecSQL("Select count(*) from TR_Terminal_In_Out where Statis_Date>=:s1",
				strYear_Month_Day, &nCount);
	if (nReturn != 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"执行SQL语句[Select count(*) from TR_Terminal_In_Out where Statis_Date>='%s']返回[%d]，错误信息:%s\n",
					strYear_Month_Day, nReturn, pOracle->getErrorInfo());
		return -1;
	}

	//3. 如果要统计的数据已经统计过了，则不需要再统计，退出此函数
	if (nCount > 0)
	{
		return 0;
	}

	//4. 统计01入库;02分发;03收回;04报废出库
	nReturn = pOracle->ExecSQL("Insert Into TR_Terminal_In_Out(Statis_Date,Hotel_ID,Change_Type,Terminal_Count,Statis_Time) Select :s1,Hotel_ID,Change_Type,count(*),sysdate from T_Terminal_Change_His where to_char(Change_Time,'yyyymmdd')=:s2 and Change_Type in ('01','02','03','04') Group by Hotel_ID,Change_Type",
				strYear_Month_Day, strYear_Month_Day);
	if (nReturn < 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"执行SQL语句[Insert Into TR_Terminal_In_Out(Statis_Date,Hotel_ID,Change_Type,Terminal_Count,Statis_Time) Select '%s',Hotel_ID,Change_Type,count(*),sysdate from T_Terminal_Change_His where to_char(Change_Time,'yyyymmdd')='%s' and Change_Type in ('01','02','03','04') Group by Hotel_ID,Change_Type]返回[%d]，错误信息:%s\n",
					strYear_Month_Day, strYear_Month_Day, nReturn, pOracle->getErrorInfo());
		pOracle->RollBack();
		return -1;
	}

	//5. 统计终端保有量
	nReturn = pOracle->ExecSQL("Insert Into TR_Terminal_In_Out(Statis_Date,Hotel_ID,Change_Type,Terminal_Count,Statis_Time) Select :s1,Hotel_ID,'00',count(*),sysdate from T_Terminal_Info where Status!=6000000 Group by Hotel_ID",
				strYear_Month_Day);
	if (nReturn < 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"执行SQL语句[Insert Into TR_Terminal_In_Out(Statis_Date,Hotel_ID,Change_Type,Terminal_Count,Statis_Time) Select '%s',Hotel_ID,'00',count(*),sysdate from T_Terminal_Info where Status!=6000000 Group by Hotel_ID]返回[%d]，错误信息:%s\n",
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
		printf("连接数据库成功\n");
	}
	else
	{
		printf("连接数据库失败,信息如下:\n\t错误代码=[%d]\n\t错误信息=%s\n",
					myOracle.getErrorNo(), myOracle.getErrorInfo());
		exit(-1);
	}
	nReturn = funStatis_Terminal_Count(&myOracle, 1);
	if (nReturn != 0)
	{
		printf("统计终端数量失败,返回%d.\n", nReturn);
	}
	else
	{
		printf("统计终端数量成功.\n");
	}

	nReturn = funStatis_Terminal_Hired(&myOracle, 1);
	if (nReturn != 0)
	{
		printf("统计终端使用情况失败,返回%d.\n", nReturn);
	}
	else
	{
		printf("统计终端使用情况成功.\n");
	}


	nReturn = funStatis_Terminal_Useable(&myOracle, 1);
	if (nReturn != 0)
	{
		printf("统计终端可用性失败,返回%d.\n", nReturn);
	}
	else
	{
		printf("统计终端可用性成功.\n");
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
