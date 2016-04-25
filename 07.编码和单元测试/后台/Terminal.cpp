/*#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#pragma pack(1)
#include "/opt/happy/src/lib/ORACLE_OPER.H"
*/
#include "/opt/happy/src/Terminal.h"

/**********************************************************************
 函数名称: funDeal_Fetch
 函数功能: 处理游标执行结果（失败、数据取完了）
 输入参数:
	pOracle-数据库连接指针
	strFile_Name-所在文件名
	nLine-所在行数
	nReturn_Fetch-fetch执行结果
	nLog_File-要写的日志文件
 输出参数:
 返 回 值:
 全局变量:
 编写作者: mahezhou
 编写日期: 2008年11月3日
 修改作者:
 修改日期:
 修改目的:
**********************************************************************/
int funDeal_Fetch(OLFO *pOracle, char *strFile_Name, int nLine, int nReturn_Fetch, int nLog_File)
{
	if (nReturn_Fetch != 0)
	{
		funLogP(nLog_File, strFile_Name, nLine, 5,
						"执行Fetch()返回[%d]，错误信息:%s\n",
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
 函数名称: funGet_Hotel_Black_List
 函数功能: 获取某个酒店的黑名单最新版本号，及清单
 输入参数:
	pOracle-数据库连接指针
	strHotel_ID-酒店ID
	nLog_File-要写的日志文件
 输出参数:
	pnBlack_Version-酒店的最新黑名单版本号
	strBlack_List-黑名单列表，不同号码之间以半角空格间隔(30*16=480字节)
 返 回 值:
	0		成功
	1		失败
 	-1		SQL执行异常
 全局变量:
 编写作者: mahezhou
 编写日期: 2009年2月11日
 修改作者:
 修改日期:
 修改目的:
**********************************************************************/
int funGet_Hotel_Black_List(OLFO *pOracle, char *strHotel_ID,
				int *pnBlack_Version, char *strBlack_List, int nLog_File)
{
	int		nReturn;
	int		nReturn_Fetch;
	int		nPhone_Number_Count;
	char	strPhone_Number[BLACK_WHITE_PHONE_LEN+1];

	//1. 取酒店的最新黑名单版本号码
	nReturn = pOracle->ExecSQL("Select Update_Time from T_Black_White_Version where Black_White_Type='1' and Org_ID=:s1",
			strHotel_ID, (time_t *)pnBlack_Version);
	if (nReturn < 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"执行SQL语句[Select (Update_Time-To_Date('20090101','yyyymmdd'))*24*3600 from T_Black_White_Version where Black_White_Type='1' and Org_ID='%s']返回[%d]，错误信息:%s\n",
					strHotel_ID, nReturn, pOracle->getErrorInfo());
		return -1;
	}
	else if (nReturn == 1000)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 20,
					"执行SQL语句[Select (Update_Time-To_Date('20090101','yyyymmdd'))*24*3600 from T_Black_White_Version where Black_White_Type='1' and Org_ID='%s']返回[%d]\n",
					strHotel_ID, nReturn);
		return 1;
	}
printf("*pnBlack_Version=[%d]\n", *pnBlack_Version);

	//2. 取酒店的最新黑名单清单(排序的原因是，如果表中的黑名单个数大于约定的30个，则只取最新加入的)
	nReturn = pOracle->ParseSQL("Select Phone_Number from T_Black_White where Black_White_Type='1' and Org_ID=:s1 order by Join_Time desc");
	if (nReturn != 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
						"执行ParseSQL()返回[%d]，错误信息:%s\n",
						nReturn, pOracle->getErrorInfo());
		return -1;
	}

	if ((nReturn = pOracle->Bind(1, strHotel_ID)) != 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
						"执行Bind()返回[%d]，错误信息:%s\n",
						nReturn, pOracle->getErrorInfo());
		return -1;
	}

	if ((nReturn = pOracle->Open()) != 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
						"执行Open()返回[%d]，错误信息:%s\n",
						nReturn, pOracle->getErrorInfo());
		return -1;
	}

	strBlack_List[0] = 0;
	nPhone_Number_Count = 0;
	while ((nReturn_Fetch = pOracle->Fetch()) == 1)
	{
		pOracle->getString(1,	strPhone_Number);
		nPhone_Number_Count++;
		if (nPhone_Number_Count > BLACK_NUM_MAX)	//如果黑名单个数已经大于约定个数，则不再追加
		{
			funLogP(nLog_File, __FILE__, __LINE__, 10,
						"酒店[%s]的黑名单[%s]已经是第[%d]个了，丢弃之\n",
						strHotel_ID, strPhone_Number, nPhone_Number_Count);
		}
		else
		{
			strcat(strBlack_List, strPhone_Number);	//把黑名单号码附到后面
			strcat(strBlack_List, " ");				//增加一个空格
		}
	}
	strBlack_List[strlen(strBlack_List)-1] = 0;
printf("strBlack_List=[%s]\n", strBlack_List);

	return funDeal_Fetch(pOracle, __FILE__, __LINE__, nReturn_Fetch, nLog_File);
}

/**********************************************************************
 函数名称: funGet_HAPPY_White_List
 函数功能: 获取平台的白名单最新版本号，及清单
 输入参数:
	pOracle-数据库连接指针
	nLog_File-要写的日志文件
 输出参数:
	pnWhite_Version-平台的最新白名单版本号
	strWhite_List-白名单列表，不同号码之间以半角空格间隔(30*16=480字节)
 返 回 值:
	0		成功
	1		失败
 	-1		SQL执行异常
 全局变量:
 编写作者: mahezhou
 编写日期: 2009年2月12日
 修改作者:
 修改日期:
 修改目的:
**********************************************************************/
int funGet_HAPPY_White_List(OLFO *pOracle, int *pnWhite_Version, char *strWhite_List, int nLog_File)
{
	int		nReturn;
	int		nReturn_Fetch;
	int		nPhone_Number_Count;
	char	strPhone_Number[BLACK_WHITE_PHONE_LEN+1];

	//1. 取平台的最新白名单版本号码
	nReturn = pOracle->ExecSQL("Select (Update_Time-To_Date('20090101','yyyymmdd'))*24*3600 from T_Black_White_Version where Black_White_Type='0'",
				pnWhite_Version);
	if (nReturn < 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"执行SQL语句[Select (Update_Time-To_Date('20090101','yyyymmdd'))*24*3600 from T_Black_White_Version where Black_White_Type='0']返回[%d]，错误信息:%s\n",
					nReturn, pOracle->getErrorInfo());
		return -1;
	}
	else if (nReturn == 1000)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 20,
					"执行SQL语句[Select (Update_Time-To_Date('20090101','yyyymmdd'))*24*3600 from T_Black_White_Version where Black_White_Type='0']返回[%d]\n",
					nReturn);
		return 1;
	}

	//2. 取平台的最新白名单清单
	nReturn = pOracle->ParseSQL("Select Phone_Number from T_Black_White where Black_White_Type='0' order by Join_Time desc");
	if (nReturn != 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
						"执行ParseSQL()返回[%d]，错误信息:%s\n",
						nReturn, pOracle->getErrorInfo());
		return -1;
	}

	if ((nReturn = pOracle->Open()) != 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
						"执行Open()返回[%d]，错误信息:%s\n",
						nReturn, pOracle->getErrorInfo());
		return -1;
	}

	strWhite_List[0] = 0;
	nPhone_Number_Count = 0;
	while ((nReturn_Fetch = pOracle->Fetch()) == 1)
	{
		pOracle->getString(1,	strPhone_Number);
		nPhone_Number_Count++;
		if (nPhone_Number_Count > WHITE_NUM_MAX)	//如果白名单个数已经大于约定个数，则不再追加
		{
			funLogP(nLog_File, __FILE__, __LINE__, 10,
						"平台的白名单[%s]已经是第[%d]个了，丢弃之\n",
						strPhone_Number, nPhone_Number_Count);
		}
		else
		{
			strcat(strWhite_List, strPhone_Number);	//把白名单号码附到后面
			strcat(strWhite_List, " ");				//增加一个空格
		}
	}
	strWhite_List[strlen(strWhite_List)-1] = 0;
printf("strWhite_List=[%s]\n", strWhite_List);

	return funDeal_Fetch(pOracle, __FILE__, __LINE__, nReturn_Fetch, nLog_File);
}

/**********************************************************************
 函数名称: funGet_Terminal_Attri
 函数功能: 取终端的手机号码、归属酒店、终端状态、预开通距现在分钟数、开通时间、酒店状态、黑白名单版本、参数更新时间
 输入参数:
	pOracle-数据库连接指针
	pTerminal_Attri-终端属性结构指针
	nLog_File-要写的日志文件
 输出参数:
 返 回 值:
	0		成功
	1		失败
 	-1		SQL执行异常
 全局变量:
 编写作者: mahezhou
 编写日期: 2009年2月12日
 修改作者:
 修改日期:
 修改目的:
**********************************************************************/
int	funGet_Terminal_Attri(OLFO *pOracle, stTerminal_Attri *pTerminal_Attri, int nLog_File)
{
	int		nReturn;
	int		nHire_Request_Time=0;

	//1. 取终端的手机号码、归属酒店、终端状态、预开通距现在分钟数、开通时间
	nReturn = pOracle->ExecSQL("Select Msisdn,Hotel_ID,Status,round((sysdate-nvl(Hire_Request_Time,sysdate))*24*60),to_char(Current_Hire_Time,'yyyymmddhh24miss'),Black_Version,White_Version,to_char(Terminal_Parm_Time,'yyyymmddhh24miss') from T_Terminal_Info where Imei_Number=:s1 and Uim_Number=:s2",
				pTerminal_Attri->strImei_Number, pTerminal_Attri->strUim_Number,
				pTerminal_Attri->strMsisdn, pTerminal_Attri->strHotel_ID,
				&(pTerminal_Attri->nTerminal_Status), &nHire_Request_Time,
				pTerminal_Attri->strCurrent_Hire_Time, &pTerminal_Attri->nBlack_Version,
				&pTerminal_Attri->nWhite_Version, pTerminal_Attri->strTerminal_Parm_Time);
	if (nReturn < 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"执行SQL语句[Select Msisdn,Hotel_ID,Status,round((sysdate-nvl(Hire_Request_Time,sysdate))*24*60),to_char(Current_Hire_Time,'yyyymmddhh24miss'),Black_Version,White_Version,to_char(Terminal_Parm_Time,'yyyymmddhh24miss') from T_Terminal_Info where Imei_Number='%s' and Uim_Number='%s']返回[%d]，错误信息:%s\n",
					pTerminal_Attri->strImei_Number, pTerminal_Attri->strUim_Number,
					nReturn, pOracle->getErrorInfo());
		return -1;
	}
	else if (nReturn == 1000)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 20,
					"执行SQL语句[Select Msisdn,Hotel_ID,Status,round((sysdate-nvl(Hire_Request_Time,sysdate))*24*60),to_char(Current_Hire_Time,'yyyymmddhh24miss'),Black_Version,White_Version,to_char(Terminal_Parm_Time,'yyyymmddhh24miss') from T_Terminal_Info where Imei_Number='%s' and Uim_Number='%s']返回[%d]\n",
					pTerminal_Attri->strImei_Number, pTerminal_Attri->strUim_Number,
					nReturn);
		return 1;
	}

	//2. 如果预开通超时，则作为未开通处理，以防预开通之后没有开机，然后终端转交给下一个旅客，下一个旅客开机之后就开通了
	if (nHire_Request_Time > PRE_VALID_MAX_MINUTES)
	{
		pTerminal_Attri->nTerminal_Status = 10;
	}

	//3. 取酒店状态
	nReturn = pOracle->ExecSQL("Select Status from T_Org_Info where Org_ID=:s1 and Org_Type='3'",
				pTerminal_Attri->strHotel_ID, pTerminal_Attri->strHotel_Status);
	if (nReturn < 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"执行SQL语句[Select Status from T_Org_Info where Org_ID='%s' and Org_Type='3']返回[%d]，错误信息:%s\n",
					pTerminal_Attri->strHotel_ID, nReturn, pOracle->getErrorInfo());
		return -1;
	}
	else if (nReturn == 1000)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 10,
					"执行SQL语句[Select Status from T_Org_Info where Org_ID='%s' and Org_Type='3']返回[%d]\n",
					pTerminal_Attri->strHotel_ID, nReturn);
		return 1;
	}
printf("终端上行数据:终端IMEI=[%s],UIM=[%s]-->\n",
	pTerminal_Attri->strImei_Number, pTerminal_Attri->strUim_Number);
printf("\t     手机号码=[%s],归属酒店ID=[%s],终端当前状态=[%d]\n",
	pTerminal_Attri->strMsisdn, pTerminal_Attri->strHotel_ID, pTerminal_Attri->nTerminal_Status);
printf("\t     酒店当前状态=[%s],预开通距现在分钟数=[%d]\n",
	 pTerminal_Attri->strHotel_Status,nHire_Request_Time);
printf("\t     开通时间=[%s],黑名单版本[%d],白名单版本[%d],终端参数更新时间[%s]\n",
	 pTerminal_Attri->strCurrent_Hire_Time, pTerminal_Attri->nBlack_Version,
	 pTerminal_Attri->nWhite_Version, pTerminal_Attri->strTerminal_Parm_Time);
	return 0;
}

/**********************************************************************
 函数名称: funParse_Terminal_Number
 函数功能: 从终端上行数据包中解析终端惟一标识为IMEI、UIM号码
 输入参数:
	pTerminal_Package-终端数据包指针
	pTerminal_Attri-终端属性结构指针
	nLog_File-要写的日志文件
 输出参数:
 返 回 值:
	0		成功
	其它	失败
 全局变量:
 编写作者: mahezhou
 编写日期: 2009年2月12日
 修改作者:
 修改日期:
 修改目的:
**********************************************************************/
int	funParse_Terminal_Number(stTerminal_Package *pTerminal_Package, stTerminal_Attri *pTerminal_Attri, int nLog_File)
{
	//1. 取出终端上行数据包中的IMEI、UIM号码
	memcpy(pTerminal_Attri->strImei_Number, pTerminal_Package->strTerminal_Number, IMEI_NUMBER_LENGTH);
	memcpy(pTerminal_Attri->strUim_Number,  pTerminal_Package->strTerminal_Number+IMEI_NUMBER_LENGTH,
				UIM_NUMBER_LENGTH);

	//2.判断IMEI、UIM号码的长度是否正确
	if (strlen(pTerminal_Attri->strImei_Number) != IMEI_NUMBER_LENGTH ||
		strlen(pTerminal_Attri->strUim_Number) != UIM_NUMBER_LENGTH)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 10, "数据包中终端惟一标识[%s]长度不对，包体[%s]\n",
					pTerminal_Package->strTerminal_Number,
					pTerminal_Package->strPackage_Body);
		return 1;
	}
/*printf("终端惟一标识=[%s]-->IMEI=[%s],UIM=[%s]\n",
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
 函数名称: funTerminal_DB_Status_To_Terminal
 函数功能: 把数据库中保存的终端状态转换为终端识别的五种状态
 		   从函数funTerminal_Get_Status_Resp中分离而来
 输入参数:
	nTerminal_Status_In_DB-数据库中保存的终端的状态
	strTerminal_Number-终端惟一标识
	nLog_File-要写的日志文件
 输出参数: 无
 返 回 值: 终端识别的5种状态值
 全局变量:
 编写作者: mahezhou
 编写日期: 2009年2月15日
 修改作者:
 修改日期:
 修改目的:
**********************************************************************/
int	funTerminal_DB_Status_To_Terminal(int nTerminal_Status_In_DB, unsigned const char *strTerminal_Number,
										int nLog_File)
{
	if (nTerminal_Status_In_DB == 12)
	{
		return 1;	//终端为开通状态
	}
	else if (nTerminal_Status_In_DB == 11)
	{
		return 4;	//终端为预开通状态
	}
	else if (nTerminal_Status_In_DB == 14)
	{
		return 3;	//终端为复位状态
	}

	else if (nTerminal_Status_In_DB == 0 || nTerminal_Status_In_DB == 10 ||
			nTerminal_Status_In_DB == 21 || nTerminal_Status_In_DB == 22 ||
			nTerminal_Status_In_DB == 13)		//把预退租作为体验模式了
	{
		return 0;	//终端为体验模式
	}

	else if (nTerminal_Status_In_DB == 15 || nTerminal_Status_In_DB == 16 ||
		nTerminal_Status_In_DB == 17 || nTerminal_Status_In_DB == 26 ||
		nTerminal_Status_In_DB == 27 || nTerminal_Status_In_DB == 60)
	{
		return 2;	//终端为关机状态
	}
	else
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"终端[%s]在数据库中保存的状态值为[%d]，不可识别\n",
					strTerminal_Number, nTerminal_Status_In_DB);
		return 0;	//终端状态异常时，默认终端为体验模式
	}
}

/**********************************************************************
 函数名称: funTerminal_Get_Status_Resp
 函数功能: 组装状态同步回包，并给终端回复
 输入参数:
	nSocket_ID-网络连接ID
	pOracle-数据库连接指针
	pTerminal_Package-状态同步数据包结构指针
	pTerminal_Attri-终端属性结构指针
	nLog_File-要写的日志文件
 输出参数:
 返 回 值:
	0		成功
	1		失败
 	-1		SQL执行异常
 全局变量:
 编写作者: mahezhou
 编写日期: 2009年2月13日
 修改作者:
 修改日期:
 修改目的:
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
	//1. 包长度、命令字、流水、终端惟一标识
	*(int *)(strPackage_Body + 0) = 0;									//包的长度(组包结束后再更新)
	*(int *)(strPackage_Body + 4) = TERMINAL_GET_STATUS_RESP;			//状态同步回包命令字
	*(int *)(strPackage_Body + 8) = pTerminal_Package->nSequence_ID;	//包的流水号
	memcpy(strPackage_Body+12, pTerminal_Package->strTerminal_Number, 36);	//终端惟一标识

	//2. 把数据库保存的终端状态转换为终端可以识别的五种状态(0/1/2/3/4)，并组到包中
	nTerminal = funTerminal_DB_Status_To_Terminal(pTerminal_Attri->nTerminal_Status,
													pTerminal_Package->strTerminal_Number, nLog_File);
	*(int *)(strPackage_Body + 48) = nTerminal;	//终端状态

	//3. 系统时间
	*(int *)(strPackage_Body + 52) = time(NULL);

	//4.1 查找这个号码的黑名单信息
	int		nBlack_Version;		//黑名单版本号
	char	strBlack_List[BLACK_NUM_MAX*(BLACK_WHITE_PHONE_LEN+1)];
	nReturn = funGet_Hotel_Black_List(pOracle, pTerminal_Attri->strHotel_ID,
				&nBlack_Version, strBlack_List, nLog_File);
	if (nReturn != 0)
	{
		return nReturn;
	}

	//4.2 查找白名单信息
	int		nWhite_Version;		//白名单版本号
	char	strWhite_List[WHITE_NUM_MAX*(BLACK_WHITE_PHONE_LEN+1)];
	nReturn = funGet_HAPPY_White_List(pOracle, &nWhite_Version, strWhite_List, nLog_File);
	if (nReturn != 0)
	{
		return nReturn;
	}

	//5.1 把黑名单信息组装到回包中
	pch = strPackage_Body + 56;
	*(int *)(pch) = nBlack_Version;							//黑名单版本
	*(int *)(pch+4) = strlen(strBlack_List);				//黑名单字段长度
	memcpy(pch+8, strBlack_List, strlen(strBlack_List)+1);	//黑名单数据
	pch = pch + 4 + 4 + strlen(strBlack_List)+1;
	nPackage_Length = 56 + 4 + 4 + strlen(strBlack_List)+1;

	//5.2 把白名单信息组装到回包中
	*(int *)(pch) = nWhite_Version;							//白名单版本
	*(int *)(pch+4) = strlen(strWhite_List);				//白名单字段长度
	memcpy(pch+8, strWhite_List, strlen(strWhite_List)+1);	//白名单数据
	pch = pch + 4 + 4 + strlen(strWhite_List)+1;
	nPackage_Length = nPackage_Length + 4 + 4 + strlen(strWhite_List)+1;

	//6.回写包长度
	*(int *)(strPackage_Body + 0) = nPackage_Length;

    //7. 给终端回状态同步包
    nReturn = fun_Write(nSocket_ID, strPackage_Body, nPackage_Length, oGateParm.nTermSendDataTime);
    if (nReturn != 0)
    {
		funLogP(nLog_File, __FILE__, __LINE__, 15,
					"给终端[%s]发送状态同步回包，等待时间[%d]，返回[%d]\n",
					pTerminal_Package->strTerminal_Number, oGateParm.nTermSendDataTime, nReturn);
		return 1;
	}
	else
	{
		funLogP(nLog_File, __FILE__, __LINE__, 30,
					"给终端[%s]发送状态同步回包，正确\n",
					pTerminal_Package->strTerminal_Number);
		//8. 在数据库中记录下发给终端的黑白名单版本号
		nReturn = pOracle->ExecSQL("Update T_Terminal_Info set Black_Version=:n1,White_Version=:n2 where Imei_Number=:s1 and Uim_Number=:s2",
					nBlack_Version, nWhite_Version, pTerminal_Attri->strImei_Number, pTerminal_Attri->strUim_Number);
		if (nReturn >1)		//数据出现严重问题
		{
			funLogP(nLog_File, __FILE__, __LINE__, 5,
					"执行SQL语句[Update T_Terminal_Info set Black_Version=%d,White_Version=%d where Imei_Number='%s' and Uim_Number='%s']返回[%d]\n",
					nBlack_Version, nWhite_Version, pTerminal_Attri->strImei_Number, pTerminal_Attri->strUim_Number,
					nReturn);
			pOracle->RollBack();
		}
		else if (nReturn < 0)
		{
			funLogP(nLog_File, __FILE__, __LINE__, 5,
					"执行SQL语句[Update T_Terminal_Info set Black_Version=%d,White_Version=%d where Imei_Number='%s' and Uim_Number='%s']返回[%d]，错误信息:%s\n",
					nBlack_Version, nWhite_Version, pTerminal_Attri->strImei_Number, pTerminal_Attri->strUim_Number,
					nReturn, pOracle->getErrorInfo());
			pOracle->RollBack();
		}
		pOracle->Commit();
		return 0;
	}

}

/**********************************************************************
 函数名称: funTerminal_Get_Status_Deal
 函数功能: 处理终端发起的状态同步包
 输入参数:
	nSocket_ID-网络连接ID
	pOracle-数据库连接指针
	pTerminal_Package-状态同步数据包结构指针
	pTerminal_Attri-终端属性结构指针
	nLog_File-要写的日志文件
 输出参数:
 返 回 值:
	0		成功
	1		失败
 	-1		SQL执行异常
 全局变量:
 编写作者: mahezhou
 编写日期: 2009年2月13日
 修改作者:
 修改日期:
 修改目的:
**********************************************************************/
int	funTerminal_Get_Status_Deal(int nSocket_ID, OLFO *pOracle, stTerminal_Package *pTerminal_Package,
			stTerminal_Attri *pTerminal_Attri, int nLog_File)
{
	int		nReturn;

	stTerminal_Get_Status *p;
	//1. 强制转换变量类型，把指针p指向状态同步包有用的数据
	p = (stTerminal_Get_Status *)pTerminal_Package->strPackage_Body;


	//2. 把终端选择的语言版本同步到数据库中
	nReturn = pOracle->ExecSQL("Update T_Terminal_Info set Language=:s1 where Imei_Number=:s2 and Uim_Number=:s3",
				strArray_Language[p->nLanguage_Version], pTerminal_Attri->strImei_Number,
				pTerminal_Attri->strUim_Number);
	if (nReturn == 0 || nReturn >1)		//走到这一步，肯定应该成功，如果失败，则是一个很严重的问题
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"执行SQL语句[Update T_Terminal_Info set Language='%s' where Imei_Number='%s' and Uim_Number='%s']返回[%d]\n",
					strArray_Language[p->nLanguage_Version], pTerminal_Attri->strImei_Number,
					pTerminal_Attri->strUim_Number, nReturn);
		pOracle->RollBack();
		return 1;		//数据库没有出错，故返回+1
	}
	else if (nReturn < 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"执行SQL语句[Update T_Terminal_Info set Language='%s' where Imei_Number='%s' and Uim_Number='%s']返回[%d]，错误信息:%s\n",
					strArray_Language[p->nLanguage_Version], pTerminal_Attri->strImei_Number,
					pTerminal_Attri->strUim_Number, nReturn, pOracle->getErrorInfo());
		pOracle->RollBack();
		return -1;		//数据库出错，故返回-1
	}
	pOracle->Commit();		//没有事务的必要，故修改后直接提交

	//3.给终端回复状态同步数据包
	nReturn = funTerminal_Get_Status_Resp(nSocket_ID, pOracle, pTerminal_Package, pTerminal_Attri, nLog_File);

	return 0;
}

/**********************************************************************
 函数名称: funTerminal_Get_Status_Main
 函数功能: 处理终端发起的状态同步包
 输入参数:
	nSocket_ID-网络连接ID
	pOracle-数据库连接指针
	pTerminal_Package-终端数据包指针
	pTerminal_Attri-终端属性结构指针
	nLog_File-要写的日志文件
 输出参数:
 返 回 值:
	0		成功
	1		失败
 	-1		SQL执行异常
 全局变量:
 编写作者: mahezhou
 编写日期: 2009年2月12日
 修改作者:
 修改日期:
 修改目的:
**********************************************************************/
int	funTerminal_Get_Status_Main(int nSocket_ID, OLFO *pOracle, stTerminal_Package *pTerminal_Package,
			stTerminal_Attri *pTerminal_Attri, int nLog_File)
{
	int		nReturn;
	stTerminal_Get_Status *p;

	//1. 强制转换变量类型，把指针p指向状态同步包有用的数据
	p = (stTerminal_Get_Status *)pTerminal_Package->strPackage_Body;
	//funTerminal_Get_Status_Print(p);	//打印状态同步包变量的值

	//2. 判断终端的语言版本
	if (p->nLanguage_Version > LANGUAGE_LIST_MAX)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 10, "终端[%s]的语言版本为[%d]，超出预期，人为修改为0，包体[%s]\n",
					pTerminal_Package->strTerminal_Number, p->nLanguage_Version,
					pTerminal_Package->strPackage_Body);
		p->nLanguage_Version = 0;
	}

	//3. 处理状态同步包
	nReturn = funTerminal_Get_Status_Deal(nSocket_ID, pOracle, pTerminal_Package, pTerminal_Attri, nLog_File);
	if (nReturn == 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 30,
						"酒店[%s]终端[%s,%s,%s]状态同步包处理成功\n",
						pTerminal_Attri->strHotel_ID, pTerminal_Attri->strImei_Number,
						pTerminal_Attri->strUim_Number, pTerminal_Attri->strMsisdn);
	}
	else
	{
		funLogP(nLog_File, __FILE__, __LINE__, 20,
						"酒店[%s]终端[%s,%s,%s]状态同步处理失败，返回[%d]\n",
						pTerminal_Attri->strHotel_ID, pTerminal_Attri->strImei_Number,
						pTerminal_Attri->strUim_Number, pTerminal_Attri->strMsisdn, nReturn);
	}

	return nReturn;
}

/**********************************************************************
 函数名称: funTerminal_Open_Resp
 函数功能: 组装终端开通服务请求回包，并给终端回复
 输入参数:
	nSocket_ID-网络连接ID
	pTerminal_Package-终端开通数据包结构指针
	nError_Code-服务开通执行结果
	nLog_File-要写的日志文件
 输出参数:
 返 回 值:
	0		成功
	1		失败
 全局变量:
 编写作者: mahezhou
 编写日期: 2009年2月14日
 修改作者:
 修改日期:
 修改目的:
**********************************************************************/
int	funTerminal_Open_Resp(int nSocket_ID, stTerminal_Package *pTerminal_Package,
			int nError_Code, int nLog_File)
{
	char	strPackage_Body[PACKAGE_LENTGH_MAX];
	int		nReturn;

	memset(strPackage_Body, 0, sizeof(strPackage_Body));
	//1. 包长度、命令字、流水、终端惟一标识
	*(int *)(strPackage_Body + 0) = 52;							//包的长度，固定，52字节
	*(int *)(strPackage_Body + 4) = TERMINAL_OPEN_RESP;			//开通回包命令字
	*(int *)(strPackage_Body + 8) = pTerminal_Package->nSequence_ID;	//包的流水号
	memcpy(strPackage_Body+12, pTerminal_Package->strTerminal_Number, 36);	//终端惟一标识

	//2. 开通结果
	if (nError_Code == -1)	//数据库出错,特别转换一下
	{
		*(int *)(strPackage_Body + 48) = TERMINAL_ERROR;
	}
	else
	{
		*(int *)(strPackage_Body + 48) = nError_Code;
	}

    //3. 给终端回开通结果包
    nReturn = fun_Write(nSocket_ID, strPackage_Body, 52, oGateParm.nTermSendDataTime);
    if (nReturn != 0)
    {
		funLogP(nLog_File, __FILE__, __LINE__, 15,
					"nSocket_ID=%d，给终端[%s]发送开通结果回包，失败，等待时间[%d]，返回[%d]\n",
					nSocket_ID, pTerminal_Package->strTerminal_Number, oGateParm.nTermSendDataTime, nReturn);
		return 1;
	}
	else
    {
		funLogP(nLog_File, __FILE__, __LINE__, 30,
					"nSocket_ID=%d，给终端[%s]发送开通结果回包，成功，返回[%d]\n",
					nSocket_ID, pTerminal_Package->strTerminal_Number, nReturn);
		return 0;
	}
}

/**********************************************************************
 函数名称: funTerminal_Open_Deal
 函数功能: 处理终端发起的开通请求包
 输入参数:
	nSocket_ID-网络连接ID
	pOracle-数据库连接指针
	pTerminal_Package-开通请求数据包结构指针
	pTerminal_Attri-终端属性结构指针
	nLog_File-要写的日志文件
 输出参数:
 返 回 值:
	0		成功
	1		失败
 	-1		SQL执行异常
 全局变量:
 编写作者: mahezhou
 编写日期: 2009年2月13日
 修改作者:
 修改日期:
 修改目的:
**********************************************************************/
int	funTerminal_Open_Deal(int nSocket_ID, OLFO *pOracle, stTerminal_Package *pTerminal_Package,
			stTerminal_Attri *pTerminal_Attri, int nLog_File)
{
	int		nReturn;
	int		nError_Code;			//开通结果值，为0则表示开通，其它值则表示失败
	char	strCurrent_Time[14+1];	//当前系统时间，记录开通时间
	char	strYear_Month[6+1];
	char	strSql[1024];

	//1.取系统时间，以及当前月份
	funGetCurrentTime(strCurrent_Time, 14);
	memcpy(strYear_Month, strCurrent_Time, 6);
	strYear_Month[6] = 0;

	nError_Code = 0;
	//2.如果终端已经开通，则不能再次执行开通操作
	if (pTerminal_Attri->nTerminal_Status == 12)
	{
		nError_Code = TERMINAL_ALREADY_OPENED;
	}
	//3.如果终端是预开通、未开通状态，则执行开通操作
	else if (pTerminal_Attri->nTerminal_Status == 10 || pTerminal_Attri->nTerminal_Status == 11)
	{
		//3.1 把终端状态修改为已开通
		nReturn = pOracle->ExecSQL("Update T_Terminal_Info set Status=12,Current_Hire_Time=to_date(:s1,'yyyymmddhh24miss'),Hire_Request_Time=NULL Where Imei_Number=:s2 and Uim_Number=:s3 and Status in (10,11)",
								strCurrent_Time, pTerminal_Attri->strImei_Number, pTerminal_Attri->strUim_Number);
		if (nReturn >1)		//数据出现严重问题
		{
			funLogP(nLog_File, __FILE__, __LINE__, 5,
					"执行SQL语句[Update T_Terminal_Info set Status=12,Current_Hire_Time=to_date('%s','yyyymmddhh24miss'),Hire_Request_Time=NULL Where Imei_Number='%s' and Uim_Number='%s' and Status in (10,11)]返回[%d]\n",
					strCurrent_Time, pTerminal_Attri->strImei_Number, pTerminal_Attri->strUim_Number,
					nReturn);
			nError_Code = TERMINAL_ERROR;		//数据库没有出错
		}
		else if (nReturn < 0)
		{
			funLogP(nLog_File, __FILE__, __LINE__, 5,
					"执行SQL语句[Update T_Terminal_Info set Status=12,Current_Hire_Time=to_date('%s','yyyymmddhh24miss'),Hire_Request_Time=NULL Where Imei_Number='%s' and Uim_Number='%s' and Status in (10,11)]返回[%d]，错误信息:%s\n",
					strCurrent_Time, pTerminal_Attri->strImei_Number, pTerminal_Attri->strUim_Number,
					nReturn, pOracle->getErrorInfo());
			nError_Code =  -1;		//数据库出错，故返回-1
		}
		else
		{
			//3.2 把终端开通记录插入到SDR表中(Hire_Src/Status使用'0',是因为以后这两个字段可能修改为字符类型)
			sprintf(strSql, "Insert Into T_SDR_%s(ID,Imei_Number,Uim_Number,Msisdn,Hotel_ID,Room_No,Broker_ID,Hire_Src,Start_Time,Status) select Bill_SEQ.nextval,a.Imei_Number,a.Uim_Number,a.Msisdn,a.Hotel_ID,a.Room_No,c.Org_ID,'0',to_date('%s','yyyymmddhh24miss'),'0' from T_Terminal_Info a,T_Org_Info b,T_Org_Info c where a.Imei_Number='%s' and a.Uim_Number='%s' and a.Status=12 and a.Hotel_ID=b.Org_ID and b.Superior_Org_ID=c.Org_ID",
					strYear_Month, strCurrent_Time, pTerminal_Attri->strImei_Number, pTerminal_Attri->strUim_Number);
			nReturn = pOracle->ExecSQL(strSql);
			if (nReturn == -1)
			{
				funLogP(nLog_File, __FILE__, __LINE__, 5,
						"执行SQL语句[%s]返回[%d]，错误信息:%s\n",
						strSql, nReturn, pOracle->getErrorInfo());
				nError_Code =  -1;		//数据库出错，故返回-1
			}
			else if (nReturn != 1)
			{
				funLogP(nLog_File, __FILE__, __LINE__, 5,
						"执行SQL语句[%s]返回[%d]\n",
						strSql, nReturn);
				nError_Code = TERMINAL_ERROR;		//数据库没有出错
			}
		}
	}
	//4.其它状态，则给终端回复开通失败
	else
	{
		nError_Code = TERMINAL_ERROR;
	}

	//5.给终端回复开通结果数据包
	nReturn = funTerminal_Open_Resp(nSocket_ID, pTerminal_Package, nError_Code, nLog_File);
	if (nError_Code == -1)		//如果数据库出现异常，则返回-1
	{
		return nError_Code;
	}
	else if (nError_Code == 0)	//如果数据库更新正确，则按照给终端的回包结果来返回
	{
		return nReturn;
	}
	else						//如果数据库更新出现错误，则返回这个错误代码
	{
		return nError_Code;
	}
}

/**********************************************************************
 函数名称: funTerminal_Open_Main
 函数功能: 处理终端发起的开通请求包
 输入参数:
	nSocket_ID-网络连接ID
	pOracle-数据库连接指针
	pTerminal_Package-终端数据包指针
	pTerminal_Attri-终端属性结构指针
	nLog_File-要写的日志文件
 输出参数:
 返 回 值:
	0		成功
	1		失败
 	-1		SQL执行异常
 全局变量:
 编写作者: mahezhou
 编写日期: 2009年2月13日
 修改作者:
 修改日期:
 修改目的:
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
						"酒店[%s]终端[%s,%s,%s]开通服务成功\n",
						pTerminal_Attri->strHotel_ID, pTerminal_Attri->strImei_Number,
						pTerminal_Attri->strUim_Number, pTerminal_Attri->strMsisdn);
	}
	else
	{
		pOracle->RollBack();
		funLogP(nLog_File, __FILE__, __LINE__, 20,
						"酒店[%s]终端[%s,%s,%s]开通服务失败,返回[%d]\n",
						pTerminal_Attri->strHotel_ID, pTerminal_Attri->strImei_Number,
						pTerminal_Attri->strUim_Number, pTerminal_Attri->strMsisdn,
						nReturn);
	}

	return nReturn;
}

/**********************************************************************
 函数名称: funTerminal_Quit_Resp
 函数功能: 组装终端退租请求回包，并给终端回复
 输入参数:
	nSocket_ID-网络连接ID
	pTerminal_Package-终端退租数据包结构指针
	strHire_Time-开通时间
	strQuit_Time-退租时间
	nError_Code-退租执行结果
	nLog_File-要写的日志文件
 输出参数:
 返 回 值:
	0		成功
	1		失败
 全局变量:
 编写作者: mahezhou
 编写日期: 2009年2月14日
 修改作者:
 修改日期:
 修改目的:
**********************************************************************/
int	funTerminal_Quit_Resp(int nSocket_ID, stTerminal_Package *pTerminal_Package,
			const char *strHire_Time, const char *strQuit_Time, int nError_Code, int nLog_File)
{
	char	strPackage_Body[PACKAGE_LENTGH_MAX];
	int		nReturn;

	memset(strPackage_Body, 0, sizeof(strPackage_Body));
	//1. 包长度、命令字、流水、终端惟一标识
	*(int *)(strPackage_Body + 0) = 60;							//包的长度，固定，60字节
	*(int *)(strPackage_Body + 4) = TERMINAL_QUIT_RESP;			//退租回包命令字
	*(int *)(strPackage_Body + 8) = pTerminal_Package->nSequence_ID;	//包的流水号
	memcpy(strPackage_Body+12, pTerminal_Package->strTerminal_Number, 36);	//终端惟一标识

	//2. 退租结果
	if (nError_Code == -1)	//数据库出错,特别转换一下
	{
		*(int *)(strPackage_Body + 48) = TERMINAL_ERROR;
	}
	else
	{
		*(int *)(strPackage_Body + 48) = nError_Code;
	}

	//3. 开通时间、退租时间
	*(int *)(strPackage_Body + 52) = funString_To_Time_T(strHire_Time);
	*(int *)(strPackage_Body + 56) = funString_To_Time_T(strQuit_Time);

    //4. 给终端回退租结果包
    nReturn = fun_Write(nSocket_ID, strPackage_Body, 60, oGateParm.nTermSendDataTime);
    if (nReturn != 0)
    {
		funLogP(nLog_File, __FILE__, __LINE__, 15,
					"nSocket_ID=%d，给终端[%s]发送退租结果回包，等待时间[%d]，返回[%d]\n",
					nSocket_ID, pTerminal_Package->strTerminal_Number, oGateParm.nTermSendDataTime, nReturn);
		return 0;	//不论回包是否正确，都作为正确处理，因为是退租
	}

	return 0;
}

/**********************************************************************
 函数名称: funTerminal_Quit_Deal
 函数功能: 处理终端发起的退租请求包
 输入参数:
	nSocket_ID-网络连接ID
	pOracle-数据库连接指针
	pTerminal_Package-退租请求数据包结构指针
	pTerminal_Attri-终端属性结构指针
	nLog_File-要写的日志文件
 输出参数:
 返 回 值:
	0		成功
	1		失败
 	-1		SQL执行异常
 全局变量:
 编写作者: mahezhou
 编写日期: 2009年2月14日
 修改作者:
 修改日期:
 修改目的:
**********************************************************************/
int	funTerminal_Quit_Deal(int nSocket_ID, OLFO *pOracle, stTerminal_Package *pTerminal_Package,
			stTerminal_Attri *pTerminal_Attri, int nLog_File)
{
	int		nReturn;
	int		nError_Code;			//退租结果值，为0则表示退租成功，其它值则表示失败
	char	strCurrent_Time[14+1];	//当前系统时间，记录退租时间
	char	strYear_Month[6+1];
	char	strSql[1024];

	//1.取系统时间,作为退租的时间
	funGetCurrentTime(strCurrent_Time, 14);

	//2.取开通时间所属月份
	memcpy(strYear_Month, pTerminal_Attri->strCurrent_Hire_Time, 6);
	strYear_Month[6] = 0;

	//3.如果终端没有开通，则不能执行退租操作
	nError_Code = 0;
	if (pTerminal_Attri->nTerminal_Status != 12)
	{
		nError_Code = TERMINAL_NOT_OPENED;
	}
	//4.如果终端是开通状态，则执行退租操作
	else
	{
		//4.1 把终端状态修改为已退租(未开通)
		nReturn = pOracle->ExecSQL("Update T_Terminal_Info set Status=10,Last_Hire_Time=Current_Hire_Time,Last_Quit_Hire_Time=to_date(:s1,'yyyymmddhh24miss'),Current_Hire_Time=null Where Imei_Number=:s2 and Uim_Number=:s3 and Status=12",
								strCurrent_Time, pTerminal_Attri->strImei_Number, pTerminal_Attri->strUim_Number);
		if (nReturn >1)		//数据出现严重问题
		{
			funLogP(nLog_File, __FILE__, __LINE__, 5,
					"执行SQL语句[Update T_Terminal_Info set Status=10,Last_Hire_Time=Current_Hire_Time,Last_Quit_Hire_Time=to_date('%s','yyyymmddhh24miss'),Current_Hire_Time=null Where Imei_Number='%s' and Uim_Number='%s' and Status=12]返回[%d]\n",
					strCurrent_Time, pTerminal_Attri->strImei_Number, pTerminal_Attri->strUim_Number,
					nReturn);
			nError_Code = TERMINAL_ERROR;		//数据库没有出错
		}
		else if (nReturn < 0)
		{
			funLogP(nLog_File, __FILE__, __LINE__, 5,
					"执行SQL语句[Update T_Terminal_Info set Status=10,Last_Hire_Time=Current_Hire_Time,Last_Quit_Hire_Time=to_date('%s','yyyymmddhh24miss'),Current_Hire_Time=null Where Imei_Number='%s' and Uim_Number='%s' and Status=12]返回[%d]，错误信息:%s\n",
					strCurrent_Time, pTerminal_Attri->strImei_Number, pTerminal_Attri->strUim_Number,
					nReturn, pOracle->getErrorInfo());
			nError_Code =  -1;		//数据库出错，故返回-1
		}
		else
		{
			//4.2 修改SDR表的状态(Quit_Src/Status使用'0',是因为以后这两个字段可能修改为字符类型)
			sprintf(strSql, "Update T_SDR_%s set End_Time=to_date('%s','yyyymmddhh24miss'),Quit_Src='0',Status='1' where Imei_Number='%s' and Uim_Number='%s' and Status='0' and to_char(Start_Time,'yyyymmddhh24miss')='%s'",
					strYear_Month, strCurrent_Time,
					pTerminal_Attri->strImei_Number, pTerminal_Attri->strUim_Number,
					pTerminal_Attri->strCurrent_Hire_Time);
			nReturn = pOracle->ExecSQL(strSql);
			if (nReturn == -1)
			{
				funLogP(nLog_File, __FILE__, __LINE__, 5,
						"执行SQL语句[%s]返回[%d]，错误信息:%s\n",
						strSql, nReturn, pOracle->getErrorInfo());
				nError_Code =  -1;		//数据库出错，故返回-1
			}
			else if (nReturn != 1)
			{
				funLogP(nLog_File, __FILE__, __LINE__, 5,
						"执行SQL语句[%s]返回[%d]\n",
						strSql, nReturn);
				nError_Code = TERMINAL_ERROR;		//数据库没有出错
			}
		}
	}

	//5.给终端回复开通结果数据包
	nReturn = funTerminal_Quit_Resp(nSocket_ID, pTerminal_Package, pTerminal_Attri->strCurrent_Hire_Time,
					strCurrent_Time, nError_Code, nLog_File);

	if (nError_Code == -1)		//如果数据库出现异常，则返回-1
	{
		return nError_Code;
	}
	else if (nError_Code == 0)	//如果数据库更新正确，则按照给终端的回包结果来返回
	{
		return nReturn;
	}
	else						//如果数据库更新出现错误，则返回这个错误代码
	{
		return nError_Code;
	}
}

/**********************************************************************
 函数名称: funTerminal_Quit_Main
 函数功能: 处理终端发起的退租请求包
 输入参数:
	nSocket_ID-网络连接ID
	pOracle-数据库连接指针
	pTerminal_Package-终端数据包指针
	pTerminal_Attri-终端属性结构指针
	nLog_File-要写的日志文件
 输出参数:
 返 回 值:
	0		成功
	1		失败
 	-1		SQL执行异常
 全局变量:
 编写作者: mahezhou
 编写日期: 2009年2月14日
 修改作者:
 修改日期:
 修改目的:
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
						"nSocket_ID=%d，酒店[%s]终端[%s,%s,%s]退租成功，这个终端的开通时间为[%s]\n",
						nSocket_ID, pTerminal_Attri->strHotel_ID, pTerminal_Attri->strImei_Number,
						pTerminal_Attri->strUim_Number, pTerminal_Attri->strMsisdn,
						pTerminal_Attri->strCurrent_Hire_Time);
	}
	else
	{
		pOracle->RollBack();
		funLogP(nLog_File, __FILE__, __LINE__, 20,
						"nSocket_ID=%d，酒店[%s]终端[%s,%s,%s]退租失败，返回[%d]\n",
						nSocket_ID, pTerminal_Attri->strHotel_ID, pTerminal_Attri->strImei_Number,
						pTerminal_Attri->strUim_Number, pTerminal_Attri->strMsisdn,
						nReturn);
	}

	return nReturn;
}

/**********************************************************************
 函数名称: funTerminal_Heartbeat_Deal
 函数功能: 处理终端发起的心跳包
 输入参数:
	nSocket_ID-网络连接ID
	pTerminal_Package-心跳包结构指针
	pTerminal_Attri-终端属性结构指针
	nLog_File-要写的日志文件
 输出参数:
 返 回 值:
	0		成功
 全局变量:
 编写作者: mahezhou
 编写日期: 2009年2月15日
 修改作者:
 修改日期:
 修改目的:
**********************************************************************/
int	funTerminal_Heartbeat_Deal(int nSocket_ID, stTerminal_Package *pTerminal_Package,
			stTerminal_Attri *pTerminal_Attri, int nLog_File)
{
	char	strPackage_Body[PACKAGE_LENTGH_MAX];
	int		nTerminal;
	int		nReturn;

	memset(strPackage_Body, 0, sizeof(strPackage_Body));
	//1. 包长度、命令字、流水、终端惟一标识
	*(int *)(strPackage_Body + 0) = 56;									//包的长度，固定，56字节
	*(int *)(strPackage_Body + 4) = TERMINAL_HEARTBEAT_RESP;			//心跳回包命令字
	*(int *)(strPackage_Body + 8) = pTerminal_Package->nSequence_ID;	//包的流水号
	memcpy(strPackage_Body+12, pTerminal_Package->strTerminal_Number, 36);	//终端惟一标识

	//2. 把数据库保存的终端状态转换为终端可以识别的五种状态(0/1/2/3/4)，并组到包中
	nTerminal = funTerminal_DB_Status_To_Terminal(pTerminal_Attri->nTerminal_Status,
													pTerminal_Package->strTerminal_Number, nLog_File);
	*(int *)(strPackage_Body + 48) = nTerminal;	//终端状态

	//3. 系统时间
	*(int *)(strPackage_Body + 52) = time(NULL);

    //4. 给终端回心跳包
    nReturn = fun_Write(nSocket_ID, strPackage_Body, 56, oGateParm.nTermSendDataTime);
    if (nReturn != 0)
    {
		funLogP(nLog_File, __FILE__, __LINE__, 15,
					"nSocket_ID=%d，给终端[%s]发送心跳回包，等待时间[%d]，返回[%d]\n",
					nSocket_ID, pTerminal_Package->strTerminal_Number, oGateParm.nTermSendDataTime, nReturn);
		return 1;
	}

	return 0;
}

/**********************************************************************
 函数名称: funTerminal_Heartbeat_Main
 函数功能: 处理终端发起的心跳包
 输入参数:
	nSocket_ID-网络连接ID
	pOracle-数据库连接指针
	pTerminal_Package-终端数据包指针
	pTerminal_Attri-终端属性结构指针
	nLog_File-要写的日志文件
 输出参数:
 返 回 值:
	0		成功
	1		失败
 	-1		SQL执行异常
 全局变量:
 编写作者: mahezhou
 编写日期: 2009年2月15日
 修改作者:
 修改日期:
 修改目的:
**********************************************************************/
int	funTerminal_Heartbeat_Main(int nSocket_ID, OLFO *pOracle, stTerminal_Package *pTerminal_Package,
			stTerminal_Attri *pTerminal_Attri, int nLog_File)
{
	int		nReturn;

	nReturn = funTerminal_Heartbeat_Deal(nSocket_ID, pTerminal_Package, pTerminal_Attri, nLog_File);
	if (nReturn == 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 30,
						"nSocket_ID=%d，酒店[%s]终端[%s,%s,%s]心跳包处理成功\n",
						nSocket_ID, pTerminal_Attri->strHotel_ID, pTerminal_Attri->strImei_Number,
						pTerminal_Attri->strUim_Number, pTerminal_Attri->strMsisdn);
	}
	else
	{
		funLogP(nLog_File, __FILE__, __LINE__, 20,
						"nSocket_ID=%d，酒店[%s]终端[%s,%s,%s]心跳包处理失败，返回[%d]\n",
						nSocket_ID, pTerminal_Attri->strHotel_ID, pTerminal_Attri->strImei_Number,
						pTerminal_Attri->strUim_Number, pTerminal_Attri->strMsisdn, nReturn);
	}

	return nReturn;
}

/**********************************************************************
 函数名称: funTerminal_Get_Time_Deal
 函数功能: 处理终端发起的时间同步包
 输入参数:
	nSocket_ID-网络连接ID
	pTerminal_Package-时间同步包结构指针
	pTerminal_Attri-终端属性结构指针
	nLog_File-要写的日志文件
 输出参数:
 返 回 值:
	0		成功
	1		失败
 全局变量:
 编写作者: mahezhou
 编写日期: 2009年2月15日
 修改作者:
 修改日期:
 修改目的:
**********************************************************************/
int	funTerminal_Get_Time_Deal(int nSocket_ID, stTerminal_Package *pTerminal_Package,
			stTerminal_Attri *pTerminal_Attri, int nLog_File)
{
	char	strPackage_Body[PACKAGE_LENTGH_MAX];
	int		nReturn;

	memset(strPackage_Body, 0, sizeof(strPackage_Body));
	//1. 包长度、命令字、流水、终端惟一标识
	*(int *)(strPackage_Body + 0) = 52;									//包的长度，固定，56字节
	*(int *)(strPackage_Body + 4) = TERMINAL_GET_TIME_RESP;				//时间同步回包命令字
	*(int *)(strPackage_Body + 8) = pTerminal_Package->nSequence_ID;	//包的流水号
	memcpy(strPackage_Body+12, pTerminal_Package->strTerminal_Number, 36);	//终端惟一标识

	//2. 系统时间
	*(int *)(strPackage_Body + 48) = time(NULL);

    //3. 给终端回时间同步包
    nReturn = fun_Write(nSocket_ID, strPackage_Body, 52, oGateParm.nTermSendDataTime);
    if (nReturn != 0)
    {
		funLogP(nLog_File, __FILE__, __LINE__, 15,
					"nSocket_ID=%d，给终端[%s]发送时间同步回包，等待时间[%d]，返回[%d]\n",
					nSocket_ID, pTerminal_Package->strTerminal_Number, oGateParm.nTermSendDataTime, nReturn);
		return 1;
	}

	return 0;
}

/**********************************************************************
 函数名称: funTerminal_Get_Time_Main
 函数功能: 处理终端发起的时间同步包
 输入参数:
	nSocket_ID-网络连接ID
	pOracle-数据库连接指针
	pTerminal_Package-终端数据包指针
	pTerminal_Attri-终端属性结构指针
	nLog_File-要写的日志文件
 输出参数:
 返 回 值:
	0		成功
	1		失败
 	-1		SQL执行异常
 全局变量:
 编写作者: mahezhou
 编写日期: 2009年2月15日
 修改作者:
 修改日期:
 修改目的:
**********************************************************************/
int	funTerminal_Get_Time_Main(int nSocket_ID, OLFO *pOracle, stTerminal_Package *pTerminal_Package,
			stTerminal_Attri *pTerminal_Attri, int nLog_File)
{
	int		nReturn;

	nReturn = funTerminal_Get_Time_Deal(nSocket_ID, pTerminal_Package, pTerminal_Attri, nLog_File);
	if (nReturn == 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 30,
						"nSocket_ID=%d，酒店[%s]终端[%s,%s,%s]时间同步包处理成功\n",
						nSocket_ID, pTerminal_Attri->strHotel_ID, pTerminal_Attri->strImei_Number,
						pTerminal_Attri->strUim_Number, pTerminal_Attri->strMsisdn);
	}
	else
	{
		funLogP(nLog_File, __FILE__, __LINE__, 25,
						"nSocket_ID=%d，酒店[%s]终端[%s,%s,%s]时间同步包处理失败，返回[%d]\n",
						nSocket_ID, pTerminal_Attri->strHotel_ID, pTerminal_Attri->strImei_Number,
						pTerminal_Attri->strUim_Number, pTerminal_Attri->strMsisdn, nReturn);
	}

	return nReturn;
}

/**********************************************************************
 函数名称: funTerminal_Statistics_Parse_Column
 函数功能: 从指针指向的地址中取一行记录，解析到变量中，记录的字段之间以半角空格分割。行之间以\r\n分割
 输入参数:
	strSource-原始数据指针
	nSize_Max-最大长度
 输出参数:
	strDest0-目标字段0
	strDest1-目标字段1
	strDest2-目标字段2
 返 回 值: 取得的字节数
 全局变量:
 编写作者: mahezhou
 编写日期: 2009年2月15日
 修改作者:
 修改日期:
 修改目的:
**********************************************************************/
int	funTerminal_Statistics_Parse_Column(const char *strSource, char *strDest0, char *strDest1, char *strDest2,
					int nSize_Max)
{
	int		i, j;
	int		nCurrent_Column=0;
	strDest0[0] = 0;
	strDest1[0] = 0;
	strDest2[0] = 0;

	j = 0;		//记录开始位置
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
 函数名称: funTerminal_Statistics_Deal
 函数功能: 处理终端发起的统计数据包
 输入参数:
	nSocket_ID-网络连接ID
	pOracle-数据库连接指针
	pTerminal_Package-统计数据包结构指针
	pTerminal_Attri-终端属性结构指针
	nLog_File-要写的日志文件
 输出参数:
 返 回 值:
	0		成功
	1		失败
 	-1		SQL执行异常
 全局变量:
 编写作者: mahezhou
 编写日期: 2009年2月15日
 修改作者:
 修改日期:
 修改目的:
**********************************************************************/
int	funTerminal_Statistics_Deal(int nSocket_ID, OLFO *pOracle, stTerminal_Package *pTerminal_Package,
			stTerminal_Attri *pTerminal_Attri, int nLog_File)
{
	char	strColumn0[64];		//字段0，内容的版本号
	char	strColumn1[64];		//字段1，写日志程序的版本号
	char	strColumn2[64];		//字段2，日志格式版本号

	int		nData_Size;
	int		nData_Size_Read;
	char	*pch;
	nData_Size = *(int *)(pTerminal_Package->strPackage_Body);

	//1. 取统计数据的第一行
	pch = (char *)pTerminal_Package->strPackage_Body+4;	//把前面4字节越过去
	nData_Size_Read = funTerminal_Statistics_Parse_Column(pch, strColumn0, strColumn1, strColumn2, nData_Size);
	nData_Size = nData_Size - nData_Size_Read;
	pch = pch + nData_Size_Read;
	//2. 循环取统计数据、入库
	char	strDest0[32];	//触发的时间，YYYYMMDDHHMMSS
	char	strDest1[256];	//查阅内容路径
	char	strDest2[32];	//持续时间
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
						"执行SQL语句[%s]返回[%d]，错误信息:%s\n",
						strSql, nReturn, pOracle->getErrorInfo());
		}
		else if (nReturn != 1)
		{
			funLogP(nLog_File, __FILE__, __LINE__, 5,
						"执行SQL语句[%s]返回[%d]\n",
						strSql, nReturn);
		}
		pOracle->Commit();	//这里没有事务的概念了，如果不Commit，将丢失更多的统计数据
	}

	//3. 回包
	char	strPackage_Body[PACKAGE_LENTGH_MAX];
	memset(strPackage_Body, 0, sizeof(strPackage_Body));
	//4. 包长度、命令字、流水、终端惟一标识
	*(int *)(strPackage_Body + 0) = 52;									//包的长度，固定，56字节
	*(int *)(strPackage_Body + 4) = TERMINAL_STATISTICS_RESP;				//时间同步回包命令字
	*(int *)(strPackage_Body + 8) = pTerminal_Package->nSequence_ID;	//包的流水号
	memcpy(strPackage_Body+12, pTerminal_Package->strTerminal_Number, 36);	//终端惟一标识

	//5. 处理结果都按照0对待
	*(int *)(strPackage_Body + 48) = 0;

    //6. 给终端回统计回包
    nReturn = fun_Write(nSocket_ID, strPackage_Body, 52, oGateParm.nTermSendDataTime);
    if (nReturn != 0)
    {
		funLogP(nLog_File, __FILE__, __LINE__, 15,
					"nSocket_ID=%d，给终端[%s]发送统计回包，等待时间[%d]，返回[%d]\n",
					nSocket_ID, pTerminal_Package->strTerminal_Number, oGateParm.nTermSendDataTime, nReturn);
		return 1;
	}

	return 0;
}

/**********************************************************************
 函数名称: funTerminal_Statistics_Main
 函数功能: 处理终端发起的统计数据包
 输入参数:
	nSocket_ID-网络连接ID
	pOracle-数据库连接指针
	pTerminal_Package-终端数据包指针
	pTerminal_Attri-终端属性结构指针
	nLog_File-要写的日志文件
 输出参数:
 返 回 值:
	0		成功
	1		失败
 	-1		SQL执行异常
 全局变量:
 编写作者: mahezhou
 编写日期: 2009年2月15日
 修改作者:
 修改日期:
 修改目的:
**********************************************************************/
int	funTerminal_Statistics_Main(int nSocket_ID, OLFO *pOracle, stTerminal_Package *pTerminal_Package,
			stTerminal_Attri *pTerminal_Attri, int nLog_File)
{
	int		nReturn;

	nReturn = funTerminal_Statistics_Deal(nSocket_ID, pOracle, pTerminal_Package, pTerminal_Attri, nLog_File);
	if (nReturn == 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 30,
						"nSocket_ID=%d，酒店[%s]终端[%s,%s,%s]统计数据包处理成功\n",
						nSocket_ID, pTerminal_Attri->strHotel_ID, pTerminal_Attri->strImei_Number,
						pTerminal_Attri->strUim_Number, pTerminal_Attri->strMsisdn);
	}
	else
	{
		funLogP(nLog_File, __FILE__, __LINE__, 20,
						"nSocket_ID=%d，酒店[%s]终端[%s,%s,%s]统计数据包处理失败，返回[%d]\n",
						nSocket_ID, pTerminal_Attri->strHotel_ID, pTerminal_Attri->strImei_Number,
						pTerminal_Attri->strUim_Number, pTerminal_Attri->strMsisdn, nReturn);
	}

	return nReturn;
}

/**********************************************************************
 函数名称: funTerminal_Data_IS_Old
 函数功能: 判断数据库中记录的下行给终端的黑名单版本、白名单版本、终端参数是否需要更新
 输入参数:
	pOracle-数据库连接指针
	strHotel_ID-终端所属酒店ID
	strImei_Number-终端的IMEI号码
	strUim_Number-终端的UIM号码
	nLog_File-要写的日志文件
 输出参数:
	pnBlack_Is_Old-该终端的黑名单版本已经过期(1表示过期，其它表示不过期)
	pnWhite_Is_Old-该终端的白名单版本已经过期(1表示过期，其它表示不过期)
	pnTerminal_Parm_Is_Old-该终端的参数已经过期(1表示过期，其它表示不过期)
 返 回 值:
	0		成功
	1		失败
 	-1		SQL执行异常
 全局变量:
 编写作者: mahezhou
 编写日期: 2009年2月16日
 修改作者:
 修改日期:
 修改目的:
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

	//1. 查找平台记录的给这个终端下发的黑白名单的最新版本、终端参数最新版本
	nReturn = pOracle->ExecSQL("Select nvl(Black_Version,0),nvl(White_Version,0),to_char(nvl(Terminal_Parm_Time,sysdate-600),'yyyymmddhh24miss') from T_Terminal_Info where Imei_Number=:s1 and Uim_Number=:s2 and Hotel_ID=:s3",
				strImei_Number, strUim_Number, strHotel_ID,
				&nBlack_Version_DB, &nWhite_Version_DB, strTerminal_Parm_Time_DB);
	if (nReturn < 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"执行SQL语句[Select nvl(Black_Version,0),nvl(White_Version,0),to_char(nvl(Terminal_Parm_Time,sysdate-600),'yyyymmddhh24miss') from T_Terminal_Info where Imei_Number='%s' and Uim_Number='%s' and Hotel_ID='%s']返回[%d]，错误信息:%s\n",
					strImei_Number, strUim_Number, strHotel_ID,
					nReturn, pOracle->getErrorInfo());
		return -1;
	}
	else if (nReturn == 1000)	//走到这一步，不应该出现1000的情况的
	{
		funLogP(nLog_File, __FILE__, __LINE__, 10,
					"执行SQL语句[Select nvl(Black_Version,0),nvl(White_Version,0),to_char(nvl(Terminal_Parm_Time,sysdate-600),'yyyymmddhh24miss') from T_Terminal_Info where Imei_Number='%s' and Uim_Number='%s' and Hotel_ID='%s']返回[%d]\n",
					strImei_Number, strUim_Number, strHotel_ID, nReturn);
		return 1;
	}

	//2. 查找酒店的黑名单版本号
	nReturn = pOracle->ExecSQL("Select Update_Time from T_Black_White_Version where Org_ID=:s1 and Black_White_Type='1'",
				strHotel_ID, &nBlack_Version_Real);
	if (nReturn < 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"执行SQL语句[Select Update_Time from T_Black_White_Version where Org_ID='%s' and Black_White_Type='1']返回[%d]，错误信息:%s\n",
					strHotel_ID, nReturn, pOracle->getErrorInfo());
		return -1;
	}
	else if (nReturn == 1000)	//酒店可能没有黑名单
	{
		funLogP(nLog_File, __FILE__, __LINE__, 15,
					"执行SQL语句[Select Update_Time from T_Black_White_Version where Org_ID='%s' and Black_White_Type='1']返回[%d]\n",
					strHotel_ID, nReturn);
		nBlack_Version_Real = 0;
	}


	//3. 查找平台白名单版本号
	nReturn = pOracle->ExecSQL("Select Update_Time from T_Black_White_Version where Black_White_Type='0'",
								&nWhite_Version_Real);
	if (nReturn < 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"执行SQL语句[Select Update_Time from T_Black_White_Version where Black_White_Type='0']返回[%d]，错误信息:%s\n",
					nReturn, pOracle->getErrorInfo());
		return -1;
	}
	else if (nReturn == 1000)	//平台可能没有白名单
	{
		funLogP(nLog_File, __FILE__, __LINE__, 15,
					"执行SQL语句[Select Update_Time from T_Black_White_Version where Black_White_Type='0']返回[%d]\n",
					nReturn);
		nWhite_Version_Real = 0;
	}

	//4. 查找终端参数最新修改时间
	nReturn = pOracle->ExecSQL("Select to_char(Modify_Time,'yyyymmddhh24miss') from T_Terminal_Parm where Available='1' and (Imei_Number='*' or Imei_Number=:s1)",
				strImei_Number, strTerminal_Parm_Time_Real);
	if (nReturn < 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"执行SQL语句[Select to_char(Modify_Time,'yyyymmddhh24miss') from T_Terminal_Parm where Available='1' and (Imei_Number='*' or Imei_Number='%s')]返回[%d]，错误信息:%s\n",
					strImei_Number, nReturn, pOracle->getErrorInfo());
		return -1;
	}
	else if (nReturn == 1000)	//平台可能没有配置该终端的参数信息
	{
		funLogP(nLog_File, __FILE__, __LINE__, 15,
					"执行SQL语句[Select to_char(Modify_Time,'yyyymmddhh24miss') from T_Terminal_Parm where Available='1' and (Imei_Number='*' or Imei_Number='%s')]返回[%d]\n",
					strImei_Number, nReturn);
		strTerminal_Parm_Time_Real[0] = 0;
	}

	//5. 查看这三个参数是否已经失效
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
 函数名称: funUpdate_Terminal_Black_White_Version
 函数功能: 更新数据库中记录的下发给终端的黑白名单版本(注意:本函数是一个事务)
 输入参数:
	pOracle-数据库连接指针
	strImei_Number-终端的IMEI号码
	strUim_Number-终端的UIM号码
	nBlack_Version-黑名单版本号
	nWhite_Version-黑名单版本号
	nLog_File-要写的日志文件
 输出参数:
 返 回 值: 无

 全局变量:
 编写作者: mahezhou
 编写日期: 2009年2月16日
 修改作者:
 修改日期:
 修改目的:
**********************************************************************/
void funUpdate_Terminal_Black_White_Version(OLFO *pOracle, char *strImei_Number, char *strUim_Number,
			int nBlack_Version, int nWhite_Version, int nLog_File)
{
	int		nReturn;

	//1. 更新黑名单
	if (nBlack_Version != -100)
	{
		nReturn = pOracle->ExecSQL("Update T_Terminal_Info set Black_Version=:n1 where Imei_Number=:s1 and Uim_Number=:s2",
						nBlack_Version, strImei_Number, strUim_Number);
		if (nReturn >1)		//数据出现严重问题
		{
			funLogP(nLog_File, __FILE__, __LINE__, 5,
					"执行SQL语句[Update T_Terminal_Info set Black_Version=%d where Imei_Number='%s' and Uim_Number='%s']返回[%d]\n",
					nBlack_Version, strImei_Number, strUim_Number,
					nReturn);
			pOracle->RollBack();
		}
		else if (nReturn < 0)
		{
			funLogP(nLog_File, __FILE__, __LINE__, 5,
					"执行SQL语句[Update T_Terminal_Info set Black_Version=%d where Imei_Number='%s' and Uim_Number='%s']返回[%d]，错误信息:%s\n",
					nBlack_Version, strImei_Number, strUim_Number,
					nReturn, pOracle->getErrorInfo());
			pOracle->RollBack();
		}
		else
		{
			pOracle->Commit();
		}
	}

	//2. 更新白名单
	if (nWhite_Version != -100)
	{
		nReturn = pOracle->ExecSQL("Update T_Terminal_Info set White_Version=:n1 where Imei_Number=:s1 and Uim_Number=:s2",
						nWhite_Version, strImei_Number, strUim_Number);
		if (nReturn >1)		//数据出现严重问题
		{
			funLogP(nLog_File, __FILE__, __LINE__, 5,
					"执行SQL语句[Update T_Terminal_Info set White_Version=%d where Imei_Number='%s' and Uim_Number='%s']返回[%d]\n",
					nWhite_Version, strImei_Number, strUim_Number,
					nReturn);
			pOracle->RollBack();
		}
		else if (nReturn < 0)
		{
			funLogP(nLog_File, __FILE__, __LINE__, 5,
					"执行SQL语句[Update T_Terminal_Info set White_Version=%d where Imei_Number='%s' and Uim_Number='%s']返回[%d]，错误信息:%s\n",
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
 函数名称: funPlatform_Send_Black
 函数功能: 给指定终端下发最新版本的黑名单
 输入参数:
	nSocket_ID-网络连接ID
	pOracle-数据库连接指针
	strHotel_ID-酒店ID
	strImei_Number-终端的IMEI号码
	strUim_Number-终端的UIM号码
	nLog_File-要写的日志文件
 输出参数:
 返 回 值:
	0		成功
	1		失败
 	-1		SQL执行异常
 全局变量:
 编写作者: mahezhou
 编写日期: 2009年2月16日
 修改作者:
 修改日期:
 修改目的:
**********************************************************************/
int	funPlatform_Send_Black(int nSocket_ID, OLFO *pOracle, char *strHotel_ID,
				char *strImei_Number, char *strUim_Number, int nLog_File)
{
	int		nReturn;
	int		nBlack_Version;		//黑名单版本号
	char	strBlack_List[BLACK_NUM_MAX*(BLACK_WHITE_PHONE_LEN+1)];
	int		nBlack_List_Length;	//黑名单字段实际长度
	int		nSequence_ID;
	//1. 获取酒店的黑名单
	nReturn = funGet_Hotel_Black_List(pOracle, strHotel_ID,
				&nBlack_Version, strBlack_List, nLog_File);
	if (nReturn != 0)
	{
		return nReturn;
	}

	//2. 组装更新黑名单数据包，包长度、命令字、流水、终端惟一标识
	char	strPackage_Body[PACKAGE_LENTGH_MAX];
	memset(strPackage_Body, 0, sizeof(strPackage_Body));
	nBlack_List_Length = strlen(strBlack_List);
	*(int *)(strPackage_Body + 0) = 12+36+4+4+nBlack_List_Length;		//包的长度
	*(int *)(strPackage_Body + 4) = PLATFORM_BLACK_UPDATE;				//平台下行的更新黑名单命令字
	nSequence_ID = fun_get_SEQ();										//生成包的流水号
	*(int *)(strPackage_Body + 8) = nSequence_ID;
	memcpy(strPackage_Body+12, strImei_Number, 15);						//终端IMEI
	memcpy(strPackage_Body+27, strUim_Number, 15);						//终端UIM
	*(int *)(strPackage_Body + 48) = nBlack_Version;					//黑名单版本号
	*(int *)(strPackage_Body + 52) = nBlack_List_Length;				//黑名单字段长度
	memcpy(strPackage_Body+56, strBlack_List, nBlack_List_Length);		//黑名单

    //3. 给终端发送黑名单版本更新包
    nReturn = fun_Write(nSocket_ID, strPackage_Body, 12+36+4+4+nBlack_List_Length, oGateParm.nTermSendDataTime);
    if (nReturn != 0)
    {
		funLogP(nLog_File, __FILE__, __LINE__, 15,
					"nSocket_ID=%d，给终端[%s,%s]发送黑名单更新包，等待时间[%d]，返回[%d]\n",
					nSocket_ID, strImei_Number, strUim_Number, oGateParm.nTermSendDataTime, nReturn);
		return 1;
	}
	else
    {
		funLogP(nLog_File, __FILE__, __LINE__, 30,
					"nSocket_ID=%d，给终端[%s,%s]发送黑名单更新包，成功\n",
					nSocket_ID, strImei_Number, strUim_Number);
	}


	//4. 等待更新黑名单回包
	char	strPackage_Resp[512];
	funLogP(nLog_File, __FILE__, __LINE__, 30,
					"nSocket_ID=%d，下行更新黑名单包，流水为[%d]，终端为[%s,%s]\n",
					nSocket_ID, nSequence_ID, strImei_Number, strUim_Number);
	nReturn = fun_Read(nSocket_ID, strPackage_Resp,
							sizeof(strPackage_Resp), oGateParm.nTermReadDataTime);
	if (nReturn <= 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 15, "等待nSocket_ID=%d更新[%s,%s]黑名单回包，失败，函数fun_Read返回[%d]\n",
					nSocket_ID, strImei_Number, strUim_Number, nReturn);
		return 1;
	}
	else
	{
		funLogP(nLog_File, __FILE__, __LINE__, 30, "等待nSocket_ID=%d更新[%s,%s]黑名单回包，成功\n",
					nSocket_ID, strImei_Number, strUim_Number);
	}

	//5. 判断终端回复包是否有误
	if (*(int *)(strPackage_Body + 0) != 52)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 20,
					"nSocket_ID=%d返回的更新[%s,%s]黑名单回包，返回的包长度为[%d]，应该为[%d]\n",
					nSocket_ID, strImei_Number, strUim_Number,
					*(int *)(strPackage_Body + 0), 52);
	}

	if (*(int *)(strPackage_Body + 4) != PLATFORM_BLACK_UPDATE_RESP)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 20,
					"nSocket_ID=%d返回的更新[%s,%s]黑名单回包，返回的命令字为[0X%08x]，应该为[0X%08x]\n",
					nSocket_ID, strImei_Number, strUim_Number,
					*(int *)(strPackage_Body + 4), PLATFORM_BLACK_UPDATE_RESP);
	}

	if (*(int *)(strPackage_Body + 8) != nSequence_ID)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 20,
					"nSocket_ID=%d返回的更新[%s,%s]黑名单回包，返回的流水为[%d]，应该为[%d]\n",
					nSocket_ID, strImei_Number, strUim_Number,
					*(int *)(strPackage_Body + 8), nSequence_ID);
	}

	if (memcmp(strPackage_Body+12, strImei_Number, 15) != 0 ||
		memcmp(strPackage_Body+27, strUim_Number, 15) != 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 20,
					"nSocket_ID=%d返回的更新黑名单回包，返回的终端惟一标识为[%s]，应该为[%s,%s]\n",
					nSocket_ID, strPackage_Body+12, strImei_Number, strUim_Number);
	}

	if (*(int *)(strPackage_Body + 48) != 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 15,
					"nSocket_ID=%d返回的更新[%s,%s]黑名单回包，返回的结果为[%d]，失败\n",
					nSocket_ID, strImei_Number, strUim_Number,
					*(int *)(strPackage_Body + 48), nSequence_ID);
		return 1;
	}
	else
	{
		funLogP(nLog_File, __FILE__, __LINE__, 15,
					"nSocket_ID=%d，更新[%s,%s]黑名单，成功\n",
					nSocket_ID, strImei_Number, strUim_Number);
		funUpdate_Terminal_Black_White_Version(pOracle, strImei_Number, strUim_Number,
					nBlack_Version, -100, nLog_File);
		return 0;
	}
}

/**********************************************************************
 函数名称: funPlatform_Send_White
 函数功能: 给指定终端下发最新版本的白名单
 输入参数:
	nSocket_ID-网络连接ID
	pOracle-数据库连接指针
	strImei_Number-终端的IMEI号码
	strUim_Number-终端的UIM号码
	nLog_File-要写的日志文件
 输出参数:
 返 回 值:
	0		成功
	1		失败
 	-1		SQL执行异常
 全局变量:
 编写作者: mahezhou
 编写日期: 2009年2月16日
 修改作者:
 修改日期:
 修改目的:
**********************************************************************/
int	funPlatform_Send_White(int nSocket_ID, OLFO *pOracle,
				char *strImei_Number, char *strUim_Number, int nLog_File)
{
	int		nReturn;
	int		nWhite_Version;		//白名单版本号
	char	strWhite_List[WHITE_NUM_MAX*(BLACK_WHITE_PHONE_LEN+1)];
	int		nWhite_List_Length;	//白名单字段实际长度
	int		nSequence_ID;
	//1. 获取白名单
	nReturn = funGet_HAPPY_White_List(pOracle, &nWhite_Version, strWhite_List, nLog_File);
	if (nReturn != 0)
	{
		return nReturn;
	}

	//2. 组装更新白名单数据包，包长度、命令字、流水、终端惟一标识
	char	strPackage_Body[PACKAGE_LENTGH_MAX];
	memset(strPackage_Body, 0, sizeof(strPackage_Body));
	nWhite_List_Length = strlen(strWhite_List);
	*(int *)(strPackage_Body + 0) = 12+36+4+4+nWhite_List_Length;		//包的长度
	*(int *)(strPackage_Body + 4) = PLATFORM_WHITE_UPDATE;				//平台下行的更新白名单命令字
	nSequence_ID = fun_get_SEQ();										//生成包的流水号
	*(int *)(strPackage_Body + 8) = nSequence_ID;
	memcpy(strPackage_Body+12, strImei_Number, 15);						//终端IMEI
	memcpy(strPackage_Body+27, strUim_Number, 15);						//终端UIM
	*(int *)(strPackage_Body + 48) = nWhite_Version;					//白名单版本号
	*(int *)(strPackage_Body + 52) = nWhite_List_Length;				//白名单字段长度
	memcpy(strPackage_Body+56, strWhite_List, nWhite_List_Length);		//白名单

    //3. 给终端发送白名单版本更新包
    nReturn = fun_Write(nSocket_ID, strPackage_Body, 12+36+4+4+nWhite_List_Length, oGateParm.nTermSendDataTime);
    if (nReturn != 0)
    {
		funLogP(nLog_File, __FILE__, __LINE__, 15,
					"nSocket_ID=%d，给终端[%s,%s]发送白名单更新包，等待时间[%d]，返回[%d]\n",
					nSocket_ID, strImei_Number, strUim_Number, oGateParm.nTermSendDataTime, nReturn);
		return 1;
	}
	else
    {
		funLogP(nLog_File, __FILE__, __LINE__, 30,
					"nSocket_ID=%d，给终端[%s,%s]发送白名单更新包，成功\n",
					nSocket_ID, strImei_Number, strUim_Number);
	}

	//4. 等待更新白名单回包
	char	strPackage_Resp[512];
	funLogP(nLog_File, __FILE__, __LINE__, 30,
					"nSocket_ID=%d，下行更新白名单包，流水为[%d]，终端为[%s,%s]\n",
					nSocket_ID, nSequence_ID, strImei_Number, strUim_Number);
	nReturn = fun_Read(nSocket_ID, strPackage_Resp,
							sizeof(strPackage_Resp), oGateParm.nTermReadDataTime);
	if (nReturn <= 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 15, "等待nSocket_ID=%d更新白名单回包，失败，函数fun_Read返回[%d]\n",
					nSocket_ID, nReturn);
		return 1;
	}
	else
	{
		funLogP(nLog_File, __FILE__, __LINE__, 15, "等待nSocket_ID=%d更新白名单回包，成功\n",
					nSocket_ID);
	}


	//5. 判断终端回复包是否有误
	if (*(int *)(strPackage_Body + 0) != 52)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 20,
					"nSocket_ID=%d返回的更新[%s,%s]白名单回包，返回的包长度为[%d]，应该为[%d]\n",
					nSocket_ID, strImei_Number, strUim_Number,
					*(int *)(strPackage_Body + 0), 52);
	}

	if (*(int *)(strPackage_Body + 4) != PLATFORM_WHITE_UPDATE_RESP)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 20,
					"nSocket_ID=%d返回的更新[%s,%s]白名单回包，返回的命令字为[%d]，应该为[%d]\n",
					nSocket_ID, strImei_Number, strUim_Number,
					*(int *)(strPackage_Body + 4), PLATFORM_WHITE_UPDATE_RESP);
	}

	if (*(int *)(strPackage_Body + 8) != nSequence_ID)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 20,
					"nSocket_ID=%d返回的更新[%s,%s]白名单回包，返回的流水为[%d]，应该为[%d]\n",
					nSocket_ID, strImei_Number, strUim_Number,
					*(int *)(strPackage_Body + 8), nSequence_ID);
	}

	if (memcmp(strPackage_Body+12, strImei_Number, 15) != 0 ||
		memcmp(strPackage_Body+27, strUim_Number, 15) != 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 20,
					"nSocket_ID=%d返回的更新白名单回包，返回的终端惟一标识为[%s]，应该为[%s,%s]\n",
					nSocket_ID, strPackage_Body+12, strImei_Number, strUim_Number);
	}

	if (*(int *)(strPackage_Body + 48) != 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 15,
					"nSocket_ID=%d返回的更新[%s,%s]白名单回包，返回的结果为[%d]，失败\n",
					nSocket_ID, strImei_Number, strUim_Number,
					*(int *)(strPackage_Body + 48), nSequence_ID);
		return 1;
	}
	else
	{
		funLogP(nLog_File, __FILE__, __LINE__, 15,
					"nSocket_ID=%d，更新[%s,%s]白名单，成功\n",
					nSocket_ID, strImei_Number, strUim_Number);
		funUpdate_Terminal_Black_White_Version(pOracle, strImei_Number, strUim_Number,
					-100, nWhite_Version, nLog_File);
		return 0;
	}
}

/**********************************************************************
 函数名称: funUpdate_Terminal_Parm_Version
 函数功能: 更新数据库中记录的下发给终端的终端参数更新时间(注意:本函数是一个事务)
 输入参数:
	pOracle-数据库连接指针
	strImei_Number-终端的IMEI号码
	strUim_Number-终端的UIM号码
	strParm_Modify_Time-参数的更新时间
	nLog_File-要写的日志文件
 输出参数:
 返 回 值: 无

 全局变量:
 编写作者: mahezhou
 编写日期: 2009年2月16日
 修改作者:
 修改日期:
 修改目的:
**********************************************************************/
void funUpdate_Terminal_Parm_Version(OLFO *pOracle, char *strImei_Number, char *strUim_Number,
			char *strParm_Modify_Time, int nLog_File)
{
	int		nReturn;

	nReturn = pOracle->ExecSQL("Update T_Terminal_Info set Terminal_Parm_Time=to_Date(:s1,'yyyymmddhh24miss') where Imei_Number=:s1 and Uim_Number=:s2",
						strParm_Modify_Time, strImei_Number, strUim_Number);
	if (nReturn >1)		//数据出现严重问题
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"执行SQL语句[Update T_Terminal_Info set Terminal_Parm_Time=to_Date('%s','yyyymmddhh24miss') where Imei_Number='%s' and Uim_Number='%s']返回[%d]\n",
					strParm_Modify_Time, strImei_Number, strUim_Number, nReturn);
		pOracle->RollBack();
	}
	else if (nReturn < 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"执行SQL语句[Update T_Terminal_Info set Terminal_Parm_Time=to_Date('%s','yyyymmddhh24miss') where Imei_Number='%s' and Uim_Number='%s']返回[%d]，错误信息:%s\n",
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
 函数名称: funPlatform_Send_Terminal_Parm
 函数功能: 给指定终端下发最新的参数
 输入参数:
	nSocket_ID-网络连接ID
	pOracle-数据库连接指针
	strImei_Number-终端的IMEI号码
	strUim_Number-终端的UIM号码
	nLog_File-要写的日志文件
 输出参数:
 返 回 值:
	0		成功
	1		失败
 	-1		SQL执行异常
 全局变量:
 编写作者: mahezhou
 编写日期: 2009年2月16日
 修改作者:
 修改日期:
 修改目的:
**********************************************************************/
int	funPlatform_Send_Terminal_Parm(int nSocket_ID, OLFO *pOracle,
				char *strImei_Number, char *strUim_Number, int nLog_File)
{
	int		nReturn;
	char	strParm_Modify_Time[14+1];
	char	strParm_List[1024];
	int		nParm_List_Length;	//参数字段实际长度
	int		nSequence_ID;
	//1. 获取终端最新参数
	nReturn = pOracle->ExecSQL("Select to_char(Modify_Time,'yyyymmddhh24miss'),Parameters from T_Terminal_Parm where Available='1' and (Imei_Number='*' or Imei_Number=:s1)",
				strImei_Number, strParm_Modify_Time, strParm_List);
	if (nReturn < 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 5,
					"执行SQL语句[Select to_char(Modify_Time,'yyyymmddhh24miss'),Parameters from T_Terminal_Parm where Available='1' and (Imei_Number='*' or Imei_Number='%s')]返回[%d]，错误信息:%s\n",
					strImei_Number, nReturn, pOracle->getErrorInfo());
		return -1;
	}
	else if (nReturn == 1000)	//平台可能没有配置该终端的参数信息
	{
		funLogP(nLog_File, __FILE__, __LINE__, 15,
					"执行SQL语句[Select to_char(Modify_Time,'yyyymmddhh24miss'),Parameters from T_Terminal_Parm where Available='1' and (Imei_Number='*' or Imei_Number='%s')]返回[%d]\n",
					strImei_Number, nReturn);
		return 1;
	}

	//2. 组装更新终端参数数据包，包长度、命令字、流水、终端惟一标识
	char	strPackage_Body[PACKAGE_LENTGH_MAX];
	memset(strPackage_Body, 0, sizeof(strPackage_Body));
	nParm_List_Length = strlen(strParm_List);
	*(int *)(strPackage_Body + 0) = 12+36+4+nParm_List_Length;		//包的长度
	*(int *)(strPackage_Body + 4) = PLATFORM_MODI_PARAMETER;		//平台下行的更新终端参数命令字
	nSequence_ID = fun_get_SEQ();									//生成包的流水号
	*(int *)(strPackage_Body + 8) = nSequence_ID;
	memcpy(strPackage_Body+12, strImei_Number, 15);						//终端IMEI
	memcpy(strPackage_Body+27, strUim_Number, 15);						//终端UIM
	*(int *)(strPackage_Body + 48) = nParm_List_Length;					//终端参数字段长度
	memcpy(strPackage_Body+52, strParm_List, nParm_List_Length);		//终端参数

    //3. 给终端发送终端参数更新包
    nReturn = fun_Write(nSocket_ID, strPackage_Body, 12+36+4+nParm_List_Length, oGateParm.nTermSendDataTime);
    if (nReturn != 0)
    {
		funLogP(nLog_File, __FILE__, __LINE__, 15,
					"nSocket_ID=%d，给终端[%s,%s]发送终端参数更新包，等待时间[%d]，返回[%d]\n",
					nSocket_ID,strImei_Number, strUim_Number, oGateParm.nTermSendDataTime, nReturn);
		return 1;
	}
	else
    {
		funLogP(nLog_File, __FILE__, __LINE__, 30,
					"nSocket_ID=%d，给终端[%s,%s]发送终端参数更新包，成功\n",
					nSocket_ID,strImei_Number, strUim_Number);
	}

	//4. 等待更新终端参数回包
	char	strPackage_Resp[512];
	funLogP(nLog_File, __FILE__, __LINE__, 30,
					"nSocket_ID=%d，下行更新终端参数包，流水为[%d]，终端为[%s,%s]\n",
					nSocket_ID, nSequence_ID, strImei_Number, strUim_Number);
	nReturn = fun_Read(nSocket_ID, strPackage_Resp,
							sizeof(strPackage_Resp), oGateParm.nTermReadDataTime);
	if (nReturn <= 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 15, "等待nSocket_ID=%d更新终端参数回包，失败，函数fun_Read返回[%d]\n",
					nSocket_ID, nReturn);
		return 1;
	}
	else
	{
		funLogP(nLog_File, __FILE__, __LINE__, 30, "等待nSocket_ID=%d更新终端参数回包，成功\n",
					nSocket_ID);
	}


	//5. 判断终端回复包是否有误
	if (*(int *)(strPackage_Body + 0) != 52)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 20,
					"nSocket_ID=%d返回的更新[%s,%s]终端参数回包，返回的包长度为[%d]，应该为[%d]\n",
					nSocket_ID, strImei_Number, strUim_Number,
					*(int *)(strPackage_Body + 0), 52);
	}

	if (*(int *)(strPackage_Body + 4) != PLATFORM_MODI_PARAMETER_RESP)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 20,
					"nSocket_ID=%d返回的更新[%s,%s]终端参数回包，返回的命令字为[%d]，应该为[%d]\n",
					nSocket_ID, strImei_Number, strUim_Number,
					*(int *)(strPackage_Body + 4), PLATFORM_MODI_PARAMETER_RESP);
	}

	if (*(int *)(strPackage_Body + 8) != nSequence_ID)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 20,
					"nSocket_ID=%d返回的更新[%s,%s]终端参数回包，返回的流水为[%d]，应该为[%d]\n",
					nSocket_ID, strImei_Number, strUim_Number,
					*(int *)(strPackage_Body + 8), nSequence_ID);
	}

	if (memcmp(strPackage_Body+12, strImei_Number, 15) != 0 ||
		memcmp(strPackage_Body+27, strUim_Number, 15) != 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 20,
					"nSocket_ID=%d返回的更新终端参数回包，返回的终端惟一标识为[%s]，应该为[%s,%s]\n",
					nSocket_ID, strPackage_Body+12, strImei_Number, strUim_Number);
	}

	if (*(int *)(strPackage_Body + 48) != 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 15,
					"nSocket_ID=%d返回的更新[%s,%s]终端参数回包，返回的结果为[%d]，失败\n",
					nSocket_ID, strImei_Number, strUim_Number,
					*(int *)(strPackage_Body + 48), nSequence_ID);
		return 1;
	}
	else
	{
		funLogP(nLog_File, __FILE__, __LINE__, 30,
					"nSocket_ID=%d，更新[%s,%s]终端参数，成功\n",
					nSocket_ID, strImei_Number, strUim_Number);
		funUpdate_Terminal_Parm_Version(pOracle, strImei_Number, strUim_Number,
					strParm_Modify_Time, nLog_File);
		return 0;
	}
}

/**********************************************************************
 函数名称: funPlatform_Send_Message_To_Terminal
 函数功能: 终端发送请求断开包之后，按照终端的情况，给终端下行数据
 输入参数:
	nSocket_ID-网络连接ID
	pOracle-数据库连接指针
	pTerminal_Package-断开请求包结构指针
	pTerminal_Attri-终端属性结构指针
	nLog_File-要写的日志文件
 输出参数:
 返 回 值:
	0		成功
	1		失败
 全局变量:
 编写作者: mahezhou
 编写日期: 2009年2月16日
 修改作者:
 修改日期:
 修改目的:
**********************************************************************/
int	funPlatform_Send_Message_To_Terminal(int nSocket_ID, OLFO *pOracle, stTerminal_Package *pTerminal_Package,
			stTerminal_Attri *pTerminal_Attri, int nLog_File)
{
	int		nReturn;
	int		nWhite_Is_Old;
	int		nBlack_Is_Old;
	int		nTerminal_Parm_Is_Old;
	//1.判断终端的黑白名单版本、参数版本是否是最新的
	nReturn = funTerminal_Data_IS_Old(pOracle, pTerminal_Attri->strHotel_ID, pTerminal_Attri->strImei_Number,
						pTerminal_Attri->strUim_Number, &nBlack_Is_Old, &nWhite_Is_Old,
						&nTerminal_Parm_Is_Old, nLog_File);
	if (nReturn == -1 || nReturn == 1)
	{
		return nReturn;
	}

	//2. 需要更新终端的黑名单
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

	//3. 需要更新终端的白名单
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

	//3. 需要更新终端参数
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
 函数名称: funTerminal_Disconnect_Deal
 函数功能: 处理终端发起的断开请求包
 输入参数:
	nSocket_ID-网络连接ID
	pOracle-数据库连接指针
	pTerminal_Package-断开请求包结构指针
	pTerminal_Attri-终端属性结构指针
	nLog_File-要写的日志文件
 输出参数:
 返 回 值:
	0		成功
	1		失败
 全局变量:
 编写作者: mahezhou
 编写日期: 2009年2月15日
 修改作者:
 修改日期:
 修改目的:
**********************************************************************/
int	funTerminal_Disconnect_Deal(int nSocket_ID, OLFO *pOracle, stTerminal_Package *pTerminal_Package,
			stTerminal_Attri *pTerminal_Attri, int nLog_File)
{
	char	strPackage_Body[PACKAGE_LENTGH_MAX];
	int		nReturn_Deal;
	int		nReturn;

	memset(strPackage_Body, 0, sizeof(strPackage_Body));
	//1. 包长度、命令字、流水、终端惟一标识
	*(int *)(strPackage_Body + 0) = 48;									//包的长度，固定，56字节
	*(int *)(strPackage_Body + 4) = TERMINAL_GET_TIME_RESP;				//时间同步回包命令字
	*(int *)(strPackage_Body + 8) = pTerminal_Package->nSequence_ID;	//包的流水号
	memcpy(strPackage_Body+12, pTerminal_Package->strTerminal_Number, 36);	//终端惟一标识

	//2. 给终端下行需要的内容
	nReturn_Deal = funPlatform_Send_Message_To_Terminal(nSocket_ID, pOracle, pTerminal_Package,
								pTerminal_Attri, nLog_File);
	if (nReturn_Deal == 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 30,
					"nSocket_ID=%d，给终端[%s]下行数据成功\n",
					nSocket_ID, pTerminal_Package->strTerminal_Number);
	}
	else
	{
		funLogP(nLog_File, __FILE__, __LINE__, 20,
					"nSocket_ID=%d，给终端[%s]下行数据失败，返回[%d]\n",
					nSocket_ID, pTerminal_Package->strTerminal_Number, nReturn_Deal);
	}

    //3. 给终端回请求断开包
    nReturn = fun_Write(nSocket_ID, strPackage_Body, 48, oGateParm.nTermSendDataTime);
	if (nReturn_Deal == -1)
	{
		 if (nReturn != 0)
		 {
			funLogP(nLog_File, __FILE__, __LINE__, 15,
					"给终端[%s]发送请求断开回包，等待时间[%d]，返回[%d]\n",
					pTerminal_Package->strTerminal_Number, oGateParm.nTermSendDataTime, nReturn);
		}
		return nReturn_Deal;
	}
	else if (nReturn != 0)
    {
		funLogP(nLog_File, __FILE__, __LINE__, 15,
					"给终端[%s]发送请求断开回包，等待时间[%d]，返回[%d]\n",
					pTerminal_Package->strTerminal_Number, oGateParm.nTermSendDataTime, nReturn);
		return 1;
	}
	else
	{
		return 0;
	}
}

/**********************************************************************
 函数名称: funTerminal_Disconnect_Main
 函数功能: 处理终端发起的断开请求包
 输入参数:
	nSocket_ID-网络连接ID
	pOracle-数据库连接指针
	pTerminal_Package-终端数据包指针
	pTerminal_Attri-终端属性结构指针
	nLog_File-要写的日志文件
 输出参数:
 返 回 值:
	0		成功
	1		失败
 	-1		SQL执行异常
 全局变量:
 编写作者: mahezhou
 编写日期: 2009年2月15日
 修改作者:
 修改日期:
 修改目的:
**********************************************************************/
int	funTerminal_Disconnect_Main(int nSocket_ID, OLFO *pOracle, stTerminal_Package *pTerminal_Package,
			stTerminal_Attri *pTerminal_Attri, int nLog_File)
{
	int		nReturn;

	nReturn = funTerminal_Disconnect_Deal(nSocket_ID, pOracle, pTerminal_Package, pTerminal_Attri, nLog_File);
	if (nReturn == 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 30,
						"nSocket_ID=%d，酒店[%s]终端[%s,%s,%s]断开请求包处理成功\n",
						nSocket_ID, pTerminal_Attri->strHotel_ID, pTerminal_Attri->strImei_Number,
						pTerminal_Attri->strUim_Number, pTerminal_Attri->strMsisdn);
	}
	else
	{
		funLogP(nLog_File, __FILE__, __LINE__, 20,
						"nSocket_ID=%d，酒店[%s]终端[%s,%s,%s]断开请求包处理失败，返回[%d]\n",
						nSocket_ID, pTerminal_Attri->strHotel_ID, pTerminal_Attri->strImei_Number,
						pTerminal_Attri->strUim_Number, pTerminal_Attri->strMsisdn, nReturn);
	}

	return nReturn;
}

/**********************************************************************
 函数名称: funDeal_Terminal_Command_Get_Baseinfo
 函数功能: 按照终端惟一标识查找终端基本情况
 输入参数:
	nSocket_ID-网络连接ID
	pOracle-数据库连接指针
 输出参数:
 返 回 值:
	0		成功
	1		失败
 	-1		SQL执行异常
 全局变量:
 编写作者: mahezhou
 编写日期: 2009年2月14日
 修改作者:
 修改日期:
 修改目的:
**********************************************************************/
int	funDeal_Terminal_Command_Get_Baseinfo(OLFO *pOracle, stTerminal_Package *pTerminal_Package,
			stTerminal_Attri *pTerminal_Attri, int nLog_File)
{
	int		nReturn;
	//1. 解析终端上行数据包中的IMEI、UIM号码
	nReturn = funParse_Terminal_Number(pTerminal_Package, pTerminal_Attri, nLog_File);
	if (nReturn != 0)
	{
		return nReturn;
	}

	//2. 取终端的手机号码、归属酒店、终端状态、预开通距现在分钟数、开通时间、酒店状态、黑白名单、参数更新时间等
	nReturn = funGet_Terminal_Attri(pOracle, pTerminal_Attri, nLog_File);
	if (nReturn != 0)
	{
		return nReturn;
	}

	//3. 如果酒店为空，则断开(这种情况应该不会出现，除非把没有分发到酒店的终端开机登录到了平台)
	if (strlen(pTerminal_Attri->strHotel_ID) == 0)
	{
		funLogP(nLog_File, __FILE__, __LINE__, 10, "终端[%s]归属酒店为空，包体[%s]\n",
					pTerminal_Package->strTerminal_Number,
					pTerminal_Package->strPackage_Body);
		return HOTEL_ID_IS_NULL;
	}

	//4. 如果酒店状态异常，则断开(这种情况应该不会出现，如果出现，则说明系统出现异常)
	if (pTerminal_Attri->strHotel_Status[0] != '0')
	{
		funLogP(nLog_File, __FILE__, __LINE__, 10, "终端[%s]归属酒店[%s]状态为[%s]，异常，包体[%s]\n",
					pTerminal_Package->strTerminal_Number, pTerminal_Attri->strHotel_ID,
					pTerminal_Attri->strHotel_Status, pTerminal_Package->strPackage_Body);
		return HOTEL_STATUS_IS_INVALID;
	}
	return 0;
}


/**********************************************************************
 函数名称: funDeal_Terminal_Command_Main
 函数功能: 处理终端连接到平台的连接
 输入参数:
	nSocket_ID-网络连接ID
	pOracle-数据库连接指针
 输出参数:
 返 回 值:
	0		成功
	1		失败
 	-1		SQL执行异常
 全局变量:
 编写作者: mahezhou
 编写日期: 2009年2月12日
 修改作者:
 修改日期:
 修改目的:
**********************************************************************/
int	funDeal_Terminal_Command_Main(int nSocket_ID, OLFO *pOracle)
{
	stTerminal_Package suTerminal_Package;	//等待接收的数据包
	stTerminal_Package *pTerminal_Package=&suTerminal_Package;
	int		Command_Counter=0;				//命令交互次数
	int		nReturn;
	int		nLog_File;
	nLog_File = 0;

	int		nReturn_Terminal;
	stTerminal_Attri suTerminal_Attri;		//终端上行指令解析结果
	stTerminal_Attri *pTerminal_Attri = &suTerminal_Attri;
	char		strTerminal_Number[36+1];	//记录终端第一次发包的终端惟一标识，第二个包以后要核对是否中间有变化

	while (1)
	{
		Command_Counter++;
		//1.清空结构体，并等待终端上行数据包
		memset(&suTerminal_Package, 0, sizeof(suTerminal_Package));
		memset(&suTerminal_Attri,   0, sizeof(stTerminal_Attri));
		if (Command_Counter == 2)	//模拟退租????????????????
		{
			suTerminal_Package.nLengh = 48;
			suTerminal_Package.nCommand = TERMINAL_QUIT;
			suTerminal_Package.nSequence_ID =200;
			sprintf((char *)suTerminal_Package.strTerminal_Number, "%s%s", "139102325150000", "460013910232515");
			nReturn = 10;
		}
		else if (Command_Counter == 30)	//模拟心跳包?????????
		{
			suTerminal_Package.nLengh = 52;
			suTerminal_Package.nCommand = TERMINAL_HEARTBEAT;
			suTerminal_Package.nSequence_ID = 100;
			sprintf((char *)suTerminal_Package.strTerminal_Number, "%s%s", "139102325150000", "460013910232515");
			*(int *)(suTerminal_Package.strPackage_Body + 0) = time(NULL);
			nReturn = 10;
		}
		else if (Command_Counter == 30)	//模拟时间同步包?????????
		{
			suTerminal_Package.nLengh = 52;
			suTerminal_Package.nCommand = TERMINAL_GET_TIME;
			suTerminal_Package.nSequence_ID = 100;
			sprintf((char *)suTerminal_Package.strTerminal_Number, "%s%s", "139102325150000", "460013910232515");
			*(int *)(suTerminal_Package.strPackage_Body + 0) = time(NULL);
			nReturn = 10;
		}
		else if (Command_Counter == 30)	//模拟话费充值包?????????
		{
			suTerminal_Package.nLengh = 52;
			suTerminal_Package.nCommand = TERMINAL_FEE;
			suTerminal_Package.nSequence_ID = 100;
			sprintf((char *)suTerminal_Package.strTerminal_Number, "%s%s", "139102325150000", "460013910232515");
			*(int *)(suTerminal_Package.strPackage_Body + 0) = 315;
			nReturn = 10;
		}
		else if (Command_Counter == 3)	//模拟统计数据包?????????
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
			sprintf(strStat_Body+nStat_Body_Length, "20090217020202 file:/zh/天涯海角.avi 2009");
			nStat_Body_Length = nStat_Body_Length+41;
			strStat_Body[nStat_Body_Length] = '\r';
			strStat_Body[nStat_Body_Length+1] = '\n';
			nStat_Body_Length = nStat_Body_Length+2;
			sprintf(strStat_Body+nStat_Body_Length, "20090217030303 file:/zh/三亚好风光.avi 2010");
			nStat_Body_Length = nStat_Body_Length+43;
			strStat_Body[nStat_Body_Length] = '\n';
			nStat_Body_Length = nStat_Body_Length+1;
			sprintf(strStat_Body+nStat_Body_Length, "20090217040404 file:/zh/海口的路真宽，姑娘真黑.avi 2010");
			nStat_Body_Length = nStat_Body_Length+55;
			suTerminal_Package.nLengh = 52+nStat_Body_Length;
			suTerminal_Package.nCommand = TERMINAL_STATISTICS;
			suTerminal_Package.nSequence_ID = 100;
			sprintf((char *)suTerminal_Package.strTerminal_Number, "%s%s", "139102325150000", "460013910232515");
			*(int *)(suTerminal_Package.strPackage_Body + 0) = nStat_Body_Length;
			memcpy(suTerminal_Package.strPackage_Body+4, strStat_Body, nStat_Body_Length);
			nReturn = 10;
		}
		else if (Command_Counter == 4)	//模拟请求断开包?????????
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
			funLogP(nLog_File, __FILE__, __LINE__, 15, "等待nSocket_ID=%d发送数据，失败，函数fun_Read返回[%d]\n",
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
					"nSocket_ID=%d发送的第%d个数据包，终端惟一标识成了[%s]，第一个包的惟一标识是[%s]，系统拒绝为之服务\n",
					nSocket_ID, Command_Counter, suTerminal_Package.strTerminal_Number, strTerminal_Number);
			return 1;
		}

		//2.解析出终端的IMEI、UIM，并查找对应的手机号码、酒店、状态、开通时间等基本信息
		nReturn = funDeal_Terminal_Command_Get_Baseinfo(pOracle, pTerminal_Package, pTerminal_Attri, nLog_File);
		if (nReturn != 0)
		{
			return nReturn;
		}

		//3. 按照命令字进行不同的处理
		switch	(pTerminal_Package->nCommand)
		{
			case TERMINAL_GET_STATUS:		//终端状态同步
				if (Command_Counter == 1)
				{
					nLog_File = 1;
				}
				nReturn_Terminal = funTerminal_Get_Status_Main(nSocket_ID, pOracle, pTerminal_Package,
													pTerminal_Attri, nLog_File);
				break;
			case TERMINAL_OPEN:				//终端发起开通
				if (Command_Counter == 1)
				{
					nLog_File = 2;
				}
				nReturn_Terminal = funTerminal_Open_Main(nSocket_ID, pOracle, pTerminal_Package,
													pTerminal_Attri, nLog_File);
				break;
			case TERMINAL_QUIT:				//终端发起退租
				if (Command_Counter == 1)
				{
					nLog_File = 3;
				}
				nReturn_Terminal = funTerminal_Quit_Main(nSocket_ID, pOracle, pTerminal_Package,
													pTerminal_Attri, nLog_File);
				break;
			case TERMINAL_HEARTBEAT:		//终端心跳包
				if (Command_Counter == 1)
				{
					nLog_File = 4;
				}
				nReturn_Terminal = funTerminal_Heartbeat_Main(nSocket_ID, pOracle, pTerminal_Package,
													pTerminal_Attri, nLog_File);
				break;
			case TERMINAL_STATISTICS:		//终端发送统计数据
				if (Command_Counter == 1)
				{
					nLog_File = 5;
				}
				nReturn_Terminal = funTerminal_Statistics_Main(nSocket_ID, pOracle, pTerminal_Package,
													pTerminal_Attri, nLog_File);
				break;

			case TERMINAL_FEE:				//话费充值(一期不做)
				if (Command_Counter == 1)
				{
					nLog_File = 6;
				}
				funLogP(0, __FILE__, __LINE__, 15, "收到终端[%s]的话费充值包，包体[%s]\n",
						pTerminal_Package->strTerminal_Number,
						pTerminal_Package->strPackage_Body);
				break;
			case TERMINAL_IPFEE:			//IP充值(一期不做)
				if (Command_Counter == 1)
				{
					nLog_File = 7;
				}
				funLogP(0, __FILE__, __LINE__, 15, "收到终端[%s]的IP充值包，包体[%s]\n",
						pTerminal_Package->strTerminal_Number,
						pTerminal_Package->strPackage_Body);
				break;
			case TERMINAL_GET_TIME:			//终端时间同步
				if (Command_Counter == 1)
				{
					nLog_File = 8;
				}
				nReturn_Terminal = funTerminal_Get_Time_Main(nSocket_ID, pOracle, pTerminal_Package,
													pTerminal_Attri, nLog_File);
				break;
			case TERMINAL_DISCONNECT:		//终端请求断开
				if (Command_Counter == 1)	//这种情况应该不存在吧？
				{
					nLog_File = 0;
					funLogP(0, __FILE__, __LINE__, 15, "终端[%s]连接到平台的第一条指令就是申请断开，比较奇怪\n",
						pTerminal_Package->strTerminal_Number);
				}
				nReturn_Terminal = funTerminal_Disconnect_Main(nSocket_ID, pOracle, pTerminal_Package,
													pTerminal_Attri, nLog_File);
				break;
			default:						//未预料到的包
				funLogP(0, __FILE__, __LINE__, 15, "收到终端[%s]的命令字为[%d]的数据包，包体[%s]\n",
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
		printf("连接数据库成功\n");
	}
	else
	{
		printf("连接数据库失败,信息如下:\n\t错误代码=[%d]\n\t错误信息=%s\n",
					myOracle.getErrorNo(), myOracle.getErrorInfo());
		exit(-1);
	}

	/*int		nBlack_Version=-1;
	char	strHotel_ID[10+1]="RJ1";
	char	strBlack_List[30*16];
	nReturn = funGet_Hotel_Black_List(pOracle, strHotel_ID, &nBlack_Version, strBlack_List, 1);
	if (nReturn != 0)
	{
		printf("获取酒店[%s]的黑名单情况失败,返回[%d].\n", strHotel_ID, nReturn);
	}
	else
	{
		printf("酒店[%s]的黑名单版本号是[%d],黑名单清单是%s.\n", strHotel_ID, nBlack_Version, strBlack_List);
	}

	int		nWhite_Version=-1;
	char	strWhite_List[30*16];
	nReturn = funGet_HAPPY_White_List(pOracle, &nWhite_Version, strWhite_List, 1);
	if (nReturn != 0)
	{
		printf("获取平台白名单情况失败,返回[%d].\n", nReturn);
	}
	else
	{
		printf("平台的白名单版本号是[%d],白名单清单是%s.\n", nWhite_Version, strWhite_List);
	}

	for (int i=0; i<=LANGUAGE_LIST_MAX; i++)
	{
		printf("Language=%d,[%s]\n", i, strArray_Language[i]);
	}*/

	stTerminal_Package	suTerminal_Package;
	/*1.状态同步
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

	//2.终端开通服务
	memset(&suTerminal_Package, 0, sizeof(suTerminal_Package));
	suTerminal_Package.nLengh = 52;
	suTerminal_Package.nCommand = TERMINAL_OPEN;
	suTerminal_Package.nSequence_ID = 234;
	sprintf((char *)suTerminal_Package.strTerminal_Number, "%s%s", "139102325150000", "460013910232515");
	*(int *)(suTerminal_Package.strPackage_Body +  0) = time(NULL);
	nReturn = funDeal_Terminal_Command_Main(1, pOracle);

	/*3.终端退租
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
