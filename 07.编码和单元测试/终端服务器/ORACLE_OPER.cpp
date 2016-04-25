#include "ORACLE_OPER.H"




void OLFO::ErrorOper(sword status)
{
	switch (status)
	{ 
		case OCI_SUCCESS_WITH_INFO: 
			strcpy(_strErrorInfo,"Error - OCI_SUCCESS_WITH_INFO\n");
			_nErrorCode = -9000;
			break; 
		case OCI_NEED_DATA: 
			strcpy(_strErrorInfo,"Error - OCI_NEED_DATA\n");
			_nErrorCode = -9001;
			break; 
		case OCI_NO_DATA: 
			strcpy(_strErrorInfo,"Error - OCI_NODATA\n");
			_nErrorCode = -9002;
			break; 
		case OCI_ERROR: 
			(void) OCIErrorGet((dvoid *)ErrHP, (ub4) 1, (text *) NULL, &_nErrorCode, 
                        (OraText *)_strErrorInfo, (ub4) sizeof(_strErrorInfo), OCI_HTYPE_ERROR);  
			break; 
		case OCI_INVALID_HANDLE: 
			strcpy(_strErrorInfo,"Error - OCI_INVALID_HANDLE\n");
			_nErrorCode = -9003;
			break; 
		case OCI_STILL_EXECUTING: 
			strcpy(_strErrorInfo,"Error - OCI_STILL_EXECUTE\n");
			_nErrorCode = -9004;
			break; 
		case OCI_CONTINUE: 
			strcpy(_strErrorInfo,"Error - OCI_CONTINUE\n");
			_nErrorCode = -9005;
			break; 
		default: 
			break;
	}
}

int	OLFO::Connect(char	*strUserName,char *strPassWord,char *strLink)
{
	int	nReturn;

	nReturn = OCIHandleAlloc((dvoid*) EnvHP, (dvoid**) &SvcHP, (ub4)OCI_HTYPE_SVCCTX, 
		(size_t)0, (dvoid**)0);
	if(nReturn != OCI_SUCCESS)
	{
		ErrorOper(nReturn);
		return -1;
	}

	nReturn = OCIAttrSet((dvoid*) SvcHP, (ub4)OCI_HTYPE_SVCCTX, (dvoid*) SrvHP, (ub4)0, 
		(ub4)OCI_ATTR_SERVER, ErrHP);
	if(nReturn != OCI_SUCCESS)
	{
		ErrorOper(nReturn);
		return -1;
	}
	
	OCIHandleAlloc((dvoid*) EnvHP, (dvoid**) &AutHP, (ub4)OCI_HTYPE_SESSION, 
		(size_t)0, (dvoid**)0);
	if(nReturn != OCI_SUCCESS)
	{
		ErrorOper(nReturn);
		return -1;
	}

	nReturn = OCIServerAttach(SrvHP, ErrHP, (text *)strLink,
    	(sb4)strlen((char *)strLink), 0);
	if( nReturn != OCI_SUCCESS )
	{
		_nErrorCode = -9007;
		sprintf(_strErrorInfo,\
			"调用函数[自定义错误 -9007];错误信息:初始数据库环境变量时出现错误,错误码为[%d]",\
			nReturn);
		ErrorOper(nReturn);
		return -1;
	}
	
	

	nReturn = OCIAttrSet( AutHP, OCI_HTYPE_SESSION, (dvoid *)strUserName, 
		(ub4)strlen((char *)strUserName), OCI_ATTR_USERNAME, ErrHP );
	if(nReturn != OCI_SUCCESS)
	{
		ErrorOper(nReturn);
		return -1;
	}

	nReturn = OCIAttrSet( AutHP, OCI_HTYPE_SESSION, (dvoid*)strPassWord,
		(ub4)strlen((char *)strPassWord), OCI_ATTR_PASSWORD, ErrHP );
	if(nReturn != OCI_SUCCESS)
	{
		ErrorOper(nReturn);
		return -1;
	}

  	nReturn = OCISessionBegin( SvcHP, ErrHP, AutHP, OCI_CRED_RDBMS, 0 );
  	if( nReturn != OCI_SUCCESS )
	{
		_nErrorCode = -9008;
		sprintf(_strErrorInfo,\
			"调用函数[自定义错误 -9008];错误信息:连接数据库出现错误,错误码为[%d]",\
			nReturn);
		ErrorOper(nReturn);
		return -1;
	}

	nReturn = OCIAttrSet(SvcHP, OCI_HTYPE_SVCCTX, AutHP, (ub4)0, OCI_ATTR_SESSION, ErrHP);
	if(nReturn != OCI_SUCCESS)
	{
		ErrorOper(nReturn);
		return -1;
	}
	
	if( (nReturn = OCIHandleAlloc( (dvoid *) EnvHP,(dvoid **)  &StmtHP,
		OCI_HTYPE_STMT, 0, (dvoid **)0)) != OCI_SUCCESS) 
	{
		_nErrorCode = -9009;
		sprintf(_strErrorInfo,\
			"调用函数[自定义错误 -9009];错误信息:初始数据库环境变量时出现错误,错误码为[%d]",\
			nReturn);
		return -1;
	}

	return 0;
}


void		OLFO::Close()
{
	if(_nDateDataPlace > 0)
	{
		for(;_nDateDataPlace > 0; _nDateDataPlace --)
		{
			delete[] pDateDataTmp[_nDateDataPlace - 1];
		}
	}
	
	if (_nTotalColCount > 0)
	{
		for(int n = 0;n < _nTotalColCount;n ++)
		{
			delete[] pColumnInfo[n].pData;
		}
		delete[] pColumnInfo;
		_nTotalColCount = 0;
	}
	
	_nTotalColCount = 0;
}
	
	
int		OLFO::ParseSQL(char	*strSQL)
{
	int		nReturn;
	
	if(_nDateDataPlace > 0)
	{
		for(;_nDateDataPlace > 0; _nDateDataPlace --)
		{
			delete[] pDateDataTmp[_nDateDataPlace - 1];
		}
	}
	
	nReturn = OCIStmtPrepare (StmtHP, ErrHP, (text *)strSQL,(ub4)strlen((char *)strSQL),OCI_NTV_SYNTAX, _nOCIMode);
    if( nReturn != OCI_SUCCESS)
	{
		ErrorOper(nReturn);
		return -1;
	}

	nReturn = OCIAttrGet(StmtHP, OCI_HTYPE_STMT, &_nSqlType,0, OCI_ATTR_STMT_TYPE, ErrHP);
	if( nReturn != OCI_SUCCESS)
	{
		ErrorOper(nReturn);
		return -1;
	}
	
	return 0;
}


void	OLFO::SetArrLen(int	n)
{
	if(_nDateDataPlace > 0)
	{
		for(;_nDateDataPlace > 0; _nDateDataPlace --)
		{
			delete[] pDateDataTmp[_nDateDataPlace - 1];
		}
	}

	_nLineCount = n;
	
	
}

int		OLFO::Bind(char	*strName,char	*strData,int	nColLen)
{
	int		nReturn;
	ub2		*ArrLen;
	
	OCIBind  *pBind = (OCIBind *) 0;
	
	if(_nLineCount <= 0)
	{
		_nErrorCode = -9014;
		sprintf(_strErrorInfo,"调用函数[自定义错误 -9014];错误信息:在进行数组类型绑定时，没有设置数组的行数，请先调用SetArrLen函数");
		return -1;
	}

	
	
	if(_nLineCount > MAXARRLINE)
	{
		_nErrorCode = -9011;
		sprintf(_strErrorInfo,"调用函数[自定义错误 -9011];错误信息:在进行数组类型绑定时，数组大小超过了最大值");
		return -1;
	}
	
	ArrLen = new ub2[_nLineCount];
	for(int	n = 0;n < _nLineCount;n ++)
	{
		ArrLen[n] = nColLen + 1;
	}
	
	
	
	nReturn = OCIBindByName(StmtHP, &pBind, ErrHP, (text *) strName,(sb4) -1,
		(dvoid *) strData,nColLen + 1, SQLT_STR, (dvoid *) 0, 
		ArrLen, (ub2 *) 0, (ub4) 0, (ub4 *) 0, _nOCIMode);

	if(nReturn != OCI_SUCCESS)
	{
		delete[] ArrLen;
		ErrorOper(nReturn);
		return -1;
	}
	
	nReturn = OCIBindArrayOfStruct(pBind, ErrHP, nColLen + 1, 0, 0, 0);
	if(nReturn != OCI_SUCCESS)
	{
		delete[] ArrLen;
		ErrorOper(nReturn);
		return -2;
	}
	
	delete[] ArrLen;
	return 0;
}

int		OLFO::Bind(char	*strName,time_t	tData[])
{
	int			nReturn;
	int			nDataSzie = 7;
	struct tm 	*pDateTime;
	
	OCIBind  *pBind = (OCIBind *) 0;
	
	if(_nLineCount <= 0)
	{
		_nErrorCode = -9014;
		sprintf(_strErrorInfo,"调用函数[自定义错误 -9014];错误信息:在进行数组类型绑定时，没有设置数组的行数，请先调用SetArrLen函数");
		return -1;
	}

	if(_nLineCount > MAXARRLINE)
	{
		_nErrorCode = -9011;
		sprintf(_strErrorInfo,"调用函数[自定义错误 -9011];错误信息:在进行数组类型绑定时，数组大小超过了最大值");
		return -1;
	}
	
	pDateDataTmp[_nDateDataPlace] = new unsigned char[7*_nLineCount];
	for(int n = 0;n < _nLineCount;n ++)
	{
		pDateTime = localtime(&(tData[n]));
		CToOraDate(pDateDataTmp[_nDateDataPlace] + n*7,pDateTime->tm_year + 1900,pDateTime->tm_mon + 1,\
			pDateTime->tm_mday,pDateTime->tm_hour,pDateTime->tm_min,pDateTime->tm_sec);
	}
	
	
	nReturn = OCIBindByName(StmtHP, &pBind, ErrHP, (text *) strName, -1, 
			(dvoid *) pDateDataTmp[_nDateDataPlace], (sword) nDataSzie, SQLT_DAT, (dvoid *) 0, 
			(ub2 *) 0, (ub2 *) 0, (ub4) 0, (ub4 *) 0, _nOCIMode);
	
	if(nReturn != OCI_SUCCESS)
	{
		delete[] pDateDataTmp[_nDateDataPlace];
		ErrorOper(nReturn);
		return -1;
	}
	
	nReturn = OCIBindArrayOfStruct(pBind, ErrHP, nDataSzie, 0, 0, 0);
	if(nReturn != OCI_SUCCESS)
	{
		delete[] pDateDataTmp[_nDateDataPlace];
		ErrorOper(nReturn);
		return -2;
	}
	_nDateDataPlace ++;
	
	return 0;
}

int		OLFO::Bind(char	*strName,char	*strData)
{
	int		nReturn;
	OCIBind  *pBind = (OCIBind *) 0;
	
	_nLineCount = 0;
	
	nReturn = OCIBindByName(StmtHP, &pBind, ErrHP, (text *) strName,(sb4) -1,
		(dvoid *) strData,strlen(strData) + 1, SQLT_STR, (dvoid *) 0, 
		(ub2 *) 0, (ub2 *) 0, (ub4) 0, (ub4 *) 0, _nOCIMode);

	if(nReturn != OCI_SUCCESS)
	{
		ErrorOper(nReturn);
		return -1;
	}

	return 0;
}

int		OLFO::Bind(char	*strName,int	pnData[])
{
	int		nReturn;
		
	OCIBind  *pBind = (OCIBind *) 0;
	
	if(_nLineCount <= 0)
	{
		_nErrorCode = -9014;
		sprintf(_strErrorInfo,"调用函数[自定义错误 -9014];错误信息:在进行数组类型绑定时，没有设置数组的行数，请先调用SetArrLen函数");
		return -1;
	}
	
	if(_nLineCount > MAXARRLINE)
	{
		_nErrorCode = -9011;
		sprintf(_strErrorInfo,"调用函数[自定义错误 -9006];错误信息:在进行数组类型绑定时，数组大小超过了最大值");
		return -1;
	}

	nReturn = OCIBindByName(StmtHP, &pBind, ErrHP, (text *) strName, -1, 
			(dvoid *) pnData, (sword) sizeof(int), SQLT_INT, (dvoid *) 0, 
			(ub2 *) 0, (ub2 *) 0, (ub4) 0, (ub4 *) 0, _nOCIMode);
	if(nReturn != OCI_SUCCESS)
	{
		ErrorOper(nReturn);
		return -1;
	}
	
	nReturn = OCIBindArrayOfStruct(pBind, ErrHP, sizeof(int), 0, 0, 0);
	if(nReturn != OCI_SUCCESS)
	{
		ErrorOper(nReturn);
		return -2;
	}
	return 0;
}

int		OLFO::Bind(char	*strName,double	pdData[])
{
	int		nReturn;
		
	OCIBind  *pBind = (OCIBind *) 0;
	
	if(_nLineCount <= 0)
	{
		_nErrorCode = -9014;
		sprintf(_strErrorInfo,"调用函数[自定义错误 -9014];错误信息:在进行数组类型绑定时，没有设置数组的行数，请先调用SetArrLen函数");
		return -1;
	}
	
	if(_nLineCount > MAXARRLINE)
	{
		_nErrorCode = -9011;
		sprintf(_strErrorInfo,"调用函数[自定义错误 -9006];错误信息:在进行数组类型绑定时，数组大小超过了最大值");
		return -1;
	}

	nReturn = OCIBindByName(StmtHP, &pBind, ErrHP, (text *) strName, -1, 
			(dvoid *) pdData, (sword) sizeof(double), SQLT_FLT, (dvoid *) 0, 
			(ub2 *) 0, (ub2 *) 0, (ub4) 0, (ub4 *) 0, _nOCIMode);
	if(nReturn != OCI_SUCCESS)
	{
		ErrorOper(nReturn);
		return -1;
	}
	
	nReturn = OCIBindArrayOfStruct(pBind, ErrHP, sizeof(double), 0, 0, 0);
	if(nReturn != OCI_SUCCESS)
	{
		ErrorOper(nReturn);
		return -2;
	}
	return 0;
}

int		OLFO::Bind(char	*strName,time_t	&tData)
{
	int			nReturn;
	int			nDataSzie = 7;
	struct tm 	*pDateTime;
	
	OCIBind  *pBind = (OCIBind *) 0;
	
	_nLineCount = 0;
	
	pDateDataTmp[_nDateDataPlace] = new unsigned char[7];
	pDateTime = localtime(&tData);
	CToOraDate(pDateDataTmp[_nDateDataPlace],pDateTime->tm_year + 1900,pDateTime->tm_mon + 1,\
		pDateTime->tm_mday,pDateTime->tm_hour,pDateTime->tm_min,pDateTime->tm_sec);
	
	nReturn = OCIBindByName(StmtHP, &pBind, ErrHP, (text *) strName, -1, 
			(dvoid *) pDateDataTmp[_nDateDataPlace], (sword) nDataSzie, SQLT_DAT, (dvoid *) 0, 
			(ub2 *) 0, (ub2 *) 0, (ub4) 0, (ub4 *) 0, _nOCIMode);
	if(nReturn != OCI_SUCCESS)
	{
		delete[] pDateDataTmp[_nDateDataPlace];
		ErrorOper(nReturn);
		return -1;
	}
	_nDateDataPlace ++;
	return 0;
}

int		OLFO::Bind(char	*strName,int	&nData)
{
	int		nReturn;
		
	OCIBind  *pBind = (OCIBind *) 0;
	
	_nLineCount = 0;

	nReturn = OCIBindByName(StmtHP, &pBind, ErrHP, (text *) strName, -1, 
			(dvoid *) &nData, (sword) sizeof(int), SQLT_INT, (dvoid *) 0, 
			(ub2 *) 0, (ub2 *) 0, (ub4) 0, (ub4 *) 0, _nOCIMode);
	if(nReturn != OCI_SUCCESS)
	{
		ErrorOper(nReturn);
		return -1;
	}

	return 0;
}

int		OLFO::Bind(char	*strName,double	&dData)
{
	int		nReturn;
		
	OCIBind  *pBind = (OCIBind *) 0;
	
	_nLineCount = 0;

	nReturn = OCIBindByName(StmtHP, &pBind, ErrHP, (text *) strName, -1, 
			(dvoid *) &dData, (sword) sizeof(double), SQLT_FLT, (dvoid *) 0, 
			(ub2 *) 0, (ub2 *) 0, (ub4) 0, (ub4 *) 0, _nOCIMode);
	if(nReturn != OCI_SUCCESS)
	{
		ErrorOper(nReturn);
		return -1;
	}

	return 0;
}

int		OLFO::Bind(int	nPos,char	*strData,int	nColLen)
{
	int		nReturn;
	ub2		*ArrLen;
	
	
	OCIBind  *pBind = (OCIBind *) 0;
	
	if(_nLineCount <= 0)
	{
		_nErrorCode = -9014;
		sprintf(_strErrorInfo,"调用函数[自定义错误 -9014];错误信息:在进行数组类型绑定时，没有设置数组的行数，请先调用SetArrLen函数");
		return -1;
	}
	
	if(_nLineCount > MAXARRLINE)
	{
		_nErrorCode = -9011;
		sprintf(_strErrorInfo,"调用函数[自定义错误 -9011];错误信息:在进行数组类型绑定时，数组大小超过了最大值");
		return -1;
	}
	
	ArrLen = new ub2[_nLineCount];
	for(int	n = 0;n < _nLineCount;n ++)
	{
		ArrLen[n] = nColLen + 1;
	}
	
	nReturn = OCIBindByPos(StmtHP, &pBind, ErrHP, nPos,
		(dvoid *) strData,nColLen + 1, SQLT_STR, (dvoid *) 0, 
		ArrLen, (ub2 *) 0, (ub4) 0, (ub4 *) 0, _nOCIMode);

	if(nReturn != OCI_SUCCESS)
	{
		delete[] ArrLen;
		ErrorOper(nReturn);
		return -1;
	}
	
	nReturn = OCIBindArrayOfStruct(pBind, ErrHP, nColLen + 1, 0, 0, 0);
	if(nReturn != OCI_SUCCESS)
	{
		delete[] ArrLen;
		ErrorOper(nReturn);
		return -2;
	}
	
	delete[] ArrLen;
	return 0;
}

int		OLFO::Bind(int	nPos,time_t	&tData)
{
	int			nReturn;
	int			nDataSzie = 7;
	struct tm 	*pDateTime;
	
	_nLineCount = 0;
	
	OCIBind  *pBind = (OCIBind *) 0;
	
	pDateDataTmp[_nDateDataPlace] = new unsigned char[7];
	pDateTime = localtime(&tData);
	CToOraDate(pDateDataTmp[_nDateDataPlace],pDateTime->tm_year + 1900,pDateTime->tm_mon + 1,\
		pDateTime->tm_mday,pDateTime->tm_hour,pDateTime->tm_min,pDateTime->tm_sec);
	
	nReturn = OCIBindByPos(StmtHP, &pBind, ErrHP, nPos, 
			(dvoid *) pDateDataTmp[_nDateDataPlace], (sword) nDataSzie, SQLT_DAT, (dvoid *) 0, 
			(ub2 *) 0, (ub2 *) 0, (ub4) 0, (ub4 *) 0, _nOCIMode);
	if(nReturn != OCI_SUCCESS)
	{
		delete[] pDateDataTmp[_nDateDataPlace];
		ErrorOper(nReturn);
		return -1;
	}
	_nDateDataPlace ++;
	return 0;
}

int		OLFO::Bind(int	nPos,char	*strData)
{

	int		nReturn;
	OCIBind  *pBind = (OCIBind *) 0;
	
	_nLineCount = 0;
	
	nReturn = OCIBindByPos(StmtHP, &pBind, ErrHP, (ub4)nPos,
		(dvoid *) strData,strlen(strData) + 1, SQLT_STR, (dvoid *) 0, 
		(ub2 *) 0, (ub2 *) 0, (ub4) 0, (ub4 *) 0, _nOCIMode);

	if(nReturn != OCI_SUCCESS)
	{
		ErrorOper(nReturn);
		return -1;
	}

	return 0;
}

int		OLFO::Bind(int	nPos,time_t	tData[])
{
	int			nReturn;
	int			nDataSzie = 7;
	struct tm 	*pDateTime;
	
	OCIBind  *pBind = (OCIBind *) 0;
	
	if(_nLineCount <= 0)
	{
		_nErrorCode = -9014;
		sprintf(_strErrorInfo,"调用函数[自定义错误 -9014];错误信息:在进行数组类型绑定时，没有设置数组的行数，请先调用SetArrLen函数");
		return -1;
	}
	
	if(_nLineCount > MAXARRLINE)
	{
		_nErrorCode = -9011;
		sprintf(_strErrorInfo,"调用函数[自定义错误 -9011];错误信息:在进行数组类型绑定时，数组大小超过了最大值");
		return -1;
	}
	
	pDateDataTmp[_nDateDataPlace] = new unsigned char[7*_nLineCount];
	for(int n = 0;n < _nLineCount;n ++)
	{
		pDateTime = localtime(&(tData[n]));
		CToOraDate(pDateDataTmp[_nDateDataPlace] + n*7,pDateTime->tm_year + 1900,pDateTime->tm_mon + 1,\
			pDateTime->tm_mday,pDateTime->tm_hour,pDateTime->tm_min,pDateTime->tm_sec);
	}
	
	nReturn = OCIBindByPos(StmtHP, &pBind, ErrHP, nPos, 
			(dvoid *) pDateDataTmp[_nDateDataPlace], (sword) nDataSzie, SQLT_DAT, (dvoid *) 0, 
			(ub2 *) 0, (ub2 *) 0, (ub4) 0, (ub4 *) 0, _nOCIMode);
	if(nReturn != OCI_SUCCESS)
	{
		delete[] pDateDataTmp[_nDateDataPlace];
		ErrorOper(nReturn);
		return -1;
	}
	
	nReturn = OCIBindArrayOfStruct(pBind, ErrHP, nDataSzie, 0, 0, 0);
	if(nReturn != OCI_SUCCESS)
	{
		delete[] pDateDataTmp[_nDateDataPlace];
		ErrorOper(nReturn);
		return -2;
	}
	
	_nDateDataPlace ++;
	return 0;
}

int		OLFO::Bind(int	nPos,int	pnData[])
{
	int		nReturn;
		
	OCIBind  *pBind = (OCIBind *) 0;
	
	if(_nLineCount <= 0)
	{
		_nErrorCode = -9014;
		sprintf(_strErrorInfo,"调用函数[自定义错误 -9014];错误信息:在进行数组类型绑定时，没有设置数组的行数，请先调用SetArrLen函数");
		return -1;
	}
	
	if(_nLineCount > MAXARRLINE)
	{
		_nErrorCode = -9011;
		sprintf(_strErrorInfo,"调用函数[自定义错误 -9006];错误信息:在进行数组类型绑定时，数组大小超过了最大值");
		return -1;
	}
	
	nReturn = OCIBindByPos(StmtHP, &pBind, ErrHP, nPos, 
			(dvoid *) pnData, (sword) sizeof(int), SQLT_INT, (dvoid *) 0, 
			(ub2 *) 0, (ub2 *) 0, (ub4) 0, (ub4 *) 0, _nOCIMode);
	if(nReturn != OCI_SUCCESS)
	{
		ErrorOper(nReturn);
		return -1;
	}
	
	nReturn = OCIBindArrayOfStruct(pBind, ErrHP, sizeof(int), 0, 0, 0);
	if(nReturn != OCI_SUCCESS)
	{
		ErrorOper(nReturn);
		return -2;
	}
	
	return 0;
}



int		OLFO::Bind(int	nPos,double	pdData[])
{
	int		nReturn;
		
	OCIBind  *pBind = (OCIBind *) 0;
	
	if(_nLineCount <= 0)
	{
		_nErrorCode = -9014;
		sprintf(_strErrorInfo,"调用函数[自定义错误 -9014];错误信息:在进行数组类型绑定时，没有设置数组的行数，请先调用SetArrLen函数");
		return -1;
	}
	
	if(_nLineCount > MAXARRLINE)
	{
		_nErrorCode = -9011;
		sprintf(_strErrorInfo,"调用函数[自定义错误 -9006];错误信息:在进行数组类型绑定时，数组大小超过了最大值");
		return -1;
	}

	nReturn = OCIBindByPos(StmtHP, &pBind, ErrHP, nPos, 
			(dvoid *) pdData, (sword) sizeof(double), SQLT_FLT, (dvoid *) 0, 
			(ub2 *) 0, (ub2 *) 0, (ub4) 0, (ub4 *) 0, _nOCIMode);
	if(nReturn != OCI_SUCCESS)
	{
		ErrorOper(nReturn);
		return -1;
	}
	
	nReturn = OCIBindArrayOfStruct(pBind, ErrHP, sizeof(double), 0, 0, 0);
	if(nReturn != OCI_SUCCESS)
	{
		ErrorOper(nReturn);
		return -2;
	}
	return 0;
}

int		OLFO::Bind(int	nPos,int	&nData)
{
	int		nReturn;
		
	OCIBind  *pBind = (OCIBind *) 0;
	
	_nLineCount = 0;

	nReturn = OCIBindByPos(StmtHP, &pBind, ErrHP, (ub4) nPos, 
			(dvoid *) &nData, (sword) sizeof(int), SQLT_INT, (dvoid *) 0, 
			(ub2 *) 0, (ub2 *) 0, (ub4) 0, (ub4 *) 0, _nOCIMode);
	if(nReturn != OCI_SUCCESS)
	{
		ErrorOper(nReturn);
		return -1;
	}

	return 0;
}

int		OLFO::Bind(int	nPos,double	&dData)
{
	int		nReturn;
		
	OCIBind  *pBind = (OCIBind *) 0;
	
	_nLineCount = 0;

	nReturn = OCIBindByPos(StmtHP, &pBind, ErrHP, nPos, 
			(dvoid *) &dData, (sword) sizeof(double), SQLT_FLT, (dvoid *) 0, 
			(ub2 *) 0, (ub2 *) 0, (ub4) 0, (ub4 *) 0, _nOCIMode);
	if(nReturn != OCI_SUCCESS)
	{
		ErrorOper(nReturn);
		return -1;
	}

	return 0;
}

int		OLFO::Define(int	nPlace,char	*strData,int	nColLen)
{
	int		nReturn;
	
	OCIDefine *pBind = (OCIDefine *) 0;
	
	if(_nLineCount <= 0)
	{
		_nErrorCode = -9014;
		sprintf(_strErrorInfo,"调用函数[自定义错误 -9014];错误信息:在进行数组类型绑定时，没有设置数组的行数，请先调用SetArrLen函数");
		return -1;
	}
	
	nReturn = OCIDefineByPos(StmtHP,&pBind,ErrHP,nPlace,(dvoid **) strData,\
		nColLen + 1,SQLT_STR,(dvoid *) 0, (ub2 *)0,(ub2 *) 0, _nOCIMode);
	if(nReturn != OCI_SUCCESS)
	{
		ErrorOper(nReturn);
		return -1;
	}
	
	nReturn = OCIDefineArrayOfStruct(pBind, ErrHP, nColLen + 1, 0, 0, 0);
	if(nReturn != OCI_SUCCESS)
	{
		ErrorOper(nReturn);
		return -1;
	}
	
	return 0;
}

int		OLFO::Define(int	nPlace,time_t	tData[])
{
	int			nReturn;

	OCIDefine *pBind = (OCIDefine *) 0;

	if(_nLineCount <= 0)
	{
		_nErrorCode = -9014;
		sprintf(_strErrorInfo,"调用函数[自定义错误 -9014];错误信息:在进行数组类型绑定时，没有设置数组的行数，请先调用SetArrLen函数");
		return -1;
	}
	
	pDateDataTmp[_nDateDataPlace] = new unsigned char[7*_nLineCount];
	pDateData[_nDateDataPlace] = tData;
	
	nReturn = OCIDefineByPos(StmtHP,&pBind,ErrHP,nPlace,(dvoid **) pDateDataTmp[_nDateDataPlace],\
		7,SQLT_DAT,(dvoid *) 0, (ub2 *)0,(ub2 *) 0, _nOCIMode);
	if(nReturn != OCI_SUCCESS)
	{
		delete[] pDateDataTmp[_nDateDataPlace];
		ErrorOper(nReturn);
		return -1;
	}
	
	nReturn = OCIDefineArrayOfStruct(pBind, ErrHP, 7, 0, 0, 0);
	if(nReturn != OCI_SUCCESS)
	{
		delete[] pDateDataTmp[_nDateDataPlace];
		ErrorOper(nReturn);
		return -1;
	}
	_nDateDataPlace ++;
	return 0;
}

int		OLFO::Define(int	nPlace,int	pData[])
{
	int		nReturn;
	
	OCIDefine *pBind = (OCIDefine *) 0;
	
	if(_nLineCount <= 0)
	{
		_nErrorCode = -9014;
		sprintf(_strErrorInfo,"调用函数[自定义错误 -9014];错误信息:在进行数组类型绑定时，没有设置数组的行数，请先调用SetArrLen函数");
		return -1;
	}
	
	nReturn = OCIDefineByPos(StmtHP,&pBind,ErrHP,nPlace,(dvoid **) pData,\
		sizeof(int),SQLT_INT,(dvoid *) 0, (ub2 *)0,(ub2 *) 0, _nOCIMode);
	if(nReturn != OCI_SUCCESS)
	{
		ErrorOper(nReturn);
		return -1;
	}
	
	nReturn = OCIDefineArrayOfStruct(pBind, ErrHP, sizeof(int), 0, 0, 0);
	if(nReturn != OCI_SUCCESS)
	{
		ErrorOper(nReturn);
		return -1;
	}
	return 0;
}

int		OLFO::Define(int	nPlace,double	dData[])
{
	int		nReturn;
	
	OCIDefine *pBind = (OCIDefine *) 0;
	
	if(_nLineCount <= 0)
	{
		_nErrorCode = -9014;
		sprintf(_strErrorInfo,"调用函数[自定义错误 -9014];错误信息:在进行数组类型绑定时，没有设置数组的行数，请先调用SetArrLen函数");
		return -1;
	}
	
	nReturn = OCIDefineByPos(StmtHP,&pBind,ErrHP,nPlace,(dvoid **) dData,\
		sizeof(double),SQLT_FLT,(dvoid *) 0, (ub2 *)0,(ub2 *) 0, _nOCIMode);
	if(nReturn != OCI_SUCCESS)
	{
		ErrorOper(nReturn);
		return -1;
	}
	
	nReturn = OCIDefineArrayOfStruct(pBind, ErrHP, sizeof(double), 0, 0, 0);
	if(nReturn != OCI_SUCCESS)
	{
		ErrorOper(nReturn);
		return -1;
	}
	return 0;
}
	

int		OLFO::Fetch()
{
	int				nReturn;
	
	nReturn = OCIStmtFetch(StmtHP,ErrHP, (ub4)1, (ub4) OCI_FETCH_NEXT, (ub4)_nOCIMode);
	if( nReturn == OCI_NO_DATA)
	{
		_nErrorCode = 0;
		_strErrorInfo[0] = '\0';
		return 0;
	}
	
	if( nReturn != OCI_SUCCESS)
	{
		ErrorOper(nReturn);
		return -1;
	}
/*	
	nReturn = OCIAttrGet((dvoid*) StmtHP, OCI_HTYPE_STMT, \
		(dvoid*) &nTotalRows, (ub4 *)0, OCI_ATTR_ROW_COUNT, ErrHP);
	if( nReturn != OCI_SUCCESS)
	{
		ErrorOper(nReturn);
		return -1;
	}
*/	
	return 1;
}

int		OLFO::Open()
{
	int				nReturn;
	OCIDefine 		*DefNP;
	OCIParam		*PParam;
	int				nCountColumn = 1;
	int				nColNameLength;
	text			*strColName;
	
	if(_nSqlType != OCI_STMT_SELECT)
	{
		_nErrorCode = -9015;
		sprintf(_strErrorInfo,"调用函数[自定义错误 -9015];错误信息:必须是查询操作的SQL才能使用Open函数");
		return -1;
	}

	nReturn = OCIStmtExecute(SvcHP, StmtHP, ErrHP, (ub4)0, (ub4)0, 0, 0, _nOCIMode);
	if( nReturn != OCI_SUCCESS)
	{
		ErrorOper(nReturn);
		return -1;
	}
	
	if (_nTotalColCount > 0)
	{
		for(int n = 0;n < _nTotalColCount;n ++)
		{
			delete[] pColumnInfo[n].pData;
		}
		delete[] pColumnInfo;
	}
	
	nReturn = OCIAttrGet((dvoid *) StmtHP, OCI_HTYPE_STMT, &_nTotalColCount, 0, OCI_ATTR_PARAM_COUNT, ErrHP);
	if( nReturn != OCI_SUCCESS)
	{
		ErrorOper(nReturn);
		return -1;
	}

	pColumnInfo = (DATA_COLUMN_INFO *) new DATA_COLUMN_INFO[_nTotalColCount];
	
	while(nCountColumn <= _nTotalColCount)
	{
		nReturn = OCIParamGet(StmtHP, OCI_HTYPE_STMT, ErrHP, (dvoid **) &PParam, nCountColumn);
		if( nReturn != OCI_SUCCESS)
		{
			ErrorOper(nReturn);
			return -1;
		}
		
		nReturn = OCIAttrGet((dvoid *) PParam,OCI_DTYPE_PARAM, \
			(dvoid *) &strColName,
			(ub4 *) &nColNameLength, OCI_ATTR_NAME, ErrHP);
		if( nReturn != OCI_SUCCESS)
		{
			ErrorOper(nReturn);
			return -1;
		}

		memcpy(pColumnInfo[nCountColumn - 1].pColName,(char *)strColName,nColNameLength);
		pColumnInfo[nCountColumn - 1].pColName[nColNameLength] = '\0';

		nReturn = OCIAttrGet((dvoid *) PParam,OCI_DTYPE_PARAM, \
			(dvoid *) &(pColumnInfo[nCountColumn - 1].nDataLen),
			(ub4 *) 0, OCI_ATTR_DATA_SIZE, ErrHP);
		if( nReturn != OCI_SUCCESS)
		{
			ErrorOper(nReturn);
			return -1;
		}
		
		nReturn = OCIAttrGet((dvoid *) PParam,OCI_DTYPE_PARAM, \
			(dvoid *) &(pColumnInfo[nCountColumn - 1].nDataType),
			(ub4 *) 0, OCI_ATTR_DATA_TYPE, ErrHP);
		if( nReturn != OCI_SUCCESS)
		{
			ErrorOper(nReturn);
			return -1;
		}
		
		nReturn = OCIAttrGet((dvoid *) PParam,OCI_DTYPE_PARAM, \
			(dvoid *) &(pColumnInfo[nCountColumn - 1].cIsNull),
			(ub4 *) 0, OCI_ATTR_IS_NULL, ErrHP);
		if( nReturn != OCI_SUCCESS)
		{
			ErrorOper(nReturn);
			return -1;
		}
		
		switch(pColumnInfo[nCountColumn - 1].nDataType)
		{
			case SQLT_NUM:
				nReturn = OCIAttrGet((dvoid *) PParam,OCI_DTYPE_PARAM, \
					(dvoid *) &(pColumnInfo[nCountColumn - 1].nNumberMain),
					(ub4 *) 0, OCI_ATTR_PRECISION, ErrHP);
				if( nReturn != OCI_SUCCESS)
				{
					ErrorOper(nReturn);
					return -1;
				}
				
				nReturn = OCIAttrGet((dvoid *) PParam,OCI_DTYPE_PARAM, \
					(dvoid *) &(pColumnInfo[nCountColumn - 1].nNumberAdd),
					(ub4 *) 0, OCI_ATTR_SCALE, ErrHP);
				if( nReturn != OCI_SUCCESS)
				{
					ErrorOper(nReturn);
					return -1;
				}
				
				if(pColumnInfo[nCountColumn - 1].nDataLen <= 0)
				{
					pColumnInfo[nCountColumn - 1].nDataLen = 255;
				}
				
				pColumnInfo[nCountColumn - 1].pData = new ub1[pColumnInfo[nCountColumn - 1].nDataLen + 1];
				
				if(pColumnInfo[nCountColumn - 1].nNumberAdd == 0)
				{
					pColumnInfo[nCountColumn - 1].nDataType = SQLT_INT;
				}
				else
				{
					pColumnInfo[nCountColumn - 1].nDataType = SQLT_FLT;
				}
				
				nReturn = OCIDefineByPos(StmtHP,&DefNP,ErrHP,nCountColumn,\
					(dvoid *) (pColumnInfo[nCountColumn - 1].pData),\
					pColumnInfo[nCountColumn - 1].nDataLen + 1,\
					SQLT_STR,\
					(dvoid *) &(pColumnInfo[nCountColumn - 1].nReturnCode),\
					(ub2 *) 0, (ub2 *) 0, _nOCIMode);
				if(nReturn != OCI_SUCCESS)
				{
					ErrorOper(nReturn);
					return -1;
				}	
				break;
			case SQLT_AFC:
			case SQLT_CHR:
				pColumnInfo[nCountColumn - 1].nDataType = SQLT_STR;
				pColumnInfo[nCountColumn - 1].pData = new ub1[pColumnInfo[nCountColumn - 1].nDataLen + 1];
				nReturn = OCIDefineByPos(StmtHP,&DefNP,ErrHP,nCountColumn,\
					(dvoid *) (pColumnInfo[nCountColumn - 1].pData),\
					pColumnInfo[nCountColumn - 1].nDataLen + 1,\
					SQLT_STR,\
					(dvoid *) &(pColumnInfo[nCountColumn - 1].nReturnCode),\
					(ub2 *) 0, (ub2 *) 0, _nOCIMode);
				if(nReturn != OCI_SUCCESS)
				{
					ErrorOper(nReturn);
					return -1;
				}
				break;
			case SQLT_DAT:
				pColumnInfo[nCountColumn - 1].nDataType = SQLT_DAT;
				pColumnInfo[nCountColumn - 1].nDataLen = 7;
				pColumnInfo[nCountColumn - 1].pData = new ub1[pColumnInfo[nCountColumn - 1].nDataLen];
				nReturn = OCIDefineByPos(StmtHP,&DefNP,ErrHP,nCountColumn,\
					(dvoid *) (pColumnInfo[nCountColumn - 1].pData),\
					pColumnInfo[nCountColumn - 1].nDataLen,\
					SQLT_DAT,\
					(dvoid *) &(pColumnInfo[nCountColumn - 1].nReturnCode),\
					(ub2 *) 0, (ub2 *) 0, _nOCIMode);
				if(nReturn != OCI_SUCCESS)
				{
					ErrorOper(nReturn);
					return -1;
				}
				break;
			case SQLT_RDD:
				pColumnInfo[nCountColumn - 1].nDataType = SQLT_RID;
				pColumnInfo[nCountColumn - 1].nDataLen = 18;
				pColumnInfo[nCountColumn - 1].pData = new ub1[pColumnInfo[nCountColumn - 1].nDataLen + 1];
				nReturn = OCIDefineByPos(StmtHP,&DefNP,ErrHP,nCountColumn,\
					(dvoid *) (pColumnInfo[nCountColumn - 1].pData),\
					pColumnInfo[nCountColumn - 1].nDataLen + 1,\
					SQLT_STR,\
					(dvoid *) &(pColumnInfo[nCountColumn - 1].nReturnCode),\
					(ub2 *) 0, (ub2 *) 0, _nOCIMode);
				if(nReturn != OCI_SUCCESS)
				{
					ErrorOper(nReturn);
					return -1;
				}
				break;
			default:
				break;
		}
		nCountColumn ++;
	}
	return 0;
}

int		OLFO::Execute()
{
	int			nReturn;
	struct tm 	tmGetDateTime;
	int			nTotalRows = 1;

	if(_nLineCount > 0)
	{
		nReturn = OCIStmtExecute(SvcHP, StmtHP, ErrHP, (ub4) _nLineCount, (ub4) 0, 
			(CONST OCISnapshot *) NULL, (OCISnapshot *) NULL, _nOCIMode);
				
		switch (nReturn)
		{
			case OCI_NO_DATA:
			case OCI_SUCCESS:
				if(_nSqlType == OCI_STMT_SELECT)
				{
					nReturn = OCIAttrGet((dvoid*) StmtHP, OCI_HTYPE_STMT, (dvoid*) &nTotalRows, (ub4 *)0, \
						OCI_ATTR_ROW_COUNT, ErrHP);
					if( nReturn != OCI_SUCCESS)
					{
						if(_nDateDataPlace > 0)
						{
							for(;_nDateDataPlace > 0; _nDateDataPlace --)
							{
								delete[] pDateDataTmp[_nDateDataPlace - 1];
								pDateData[_nDateDataPlace - 1] = NULL;
							}
						}
						ErrorOper(nReturn);
						_nLineCount = 0;
						return -1;
					}
					if(_nDateDataPlace > 0)
					{
						for(;_nDateDataPlace > 0;_nDateDataPlace --)
						{
							for(int n = 0;n < nTotalRows;n ++)
							{
								OraDateToC((unsigned char *)(pDateDataTmp[_nDateDataPlace - 1]) + n*7,tmGetDateTime.tm_year,\
									tmGetDateTime.tm_mon,tmGetDateTime.tm_mday,tmGetDateTime.tm_hour,\
									tmGetDateTime.tm_min,tmGetDateTime.tm_sec);
								tmGetDateTime.tm_year = tmGetDateTime.tm_year - 1900;
								tmGetDateTime.tm_mon --;
								if(((pDateData[_nDateDataPlace - 1][n]) = mktime(&tmGetDateTime)) == -1)
								{
							
									for(int m = 0;m < _nDateDataPlace; m++)
									{
										delete[] pDateDataTmp[m];
										pDateData[m] = NULL;
									}
									_nDateDataPlace = 0;
									_nLineCount = 0;
									_nErrorCode = -9020;
									sprintf(_strErrorInfo,\
										"调用函数[ExecSQL -9020];错误信息:转换时间时，发现了未知错误！");
									return -1;
								}
															
							}
							delete[] pDateDataTmp[_nDateDataPlace - 1];
							pDateData[_nDateDataPlace - 1] = NULL;
						}
					}
				}
				else
				{
					if(_nDateDataPlace > 0)
					{
						for(;_nDateDataPlace > 0; _nDateDataPlace --)
						{
							delete[] pDateDataTmp[_nDateDataPlace - 1];
							pDateData[_nDateDataPlace - 1] = NULL;
						}
					}
				}
				
				_nLineCount = 0;
				return nTotalRows;
				break;
			case OCI_SUCCESS_WITH_INFO:
				if(_nDateDataPlace > 0)
				{
					for(;_nDateDataPlace > 0; _nDateDataPlace --)
					{
						delete[] pDateDataTmp[_nDateDataPlace - 1];
						pDateData[_nDateDataPlace - 1] = NULL;
					}
				}
				_nLineCount = 0;
				return 0;
				break;
			default:
				if(_nDateDataPlace > 0)
				{
					for(;_nDateDataPlace > 0; _nDateDataPlace --)
					{
						delete[] pDateDataTmp[_nDateDataPlace - 1];
						pDateData[_nDateDataPlace - 1] = NULL;
					}
				}
				_nLineCount = 0;
				ErrorOper(nReturn);
				return -1;
				break;
		}
	}
	else
	{
		nReturn = OCIStmtExecute(SvcHP, StmtHP, ErrHP, (ub4) 1, (ub4) 0, 
			(CONST OCISnapshot *) NULL, (OCISnapshot *) NULL, _nOCIMode);
		switch (nReturn)
		{
			case OCI_SUCCESS:
				return 0;
				break;
			case OCI_NO_DATA:
				return 1;
				break;
			default:
				ErrorOper(nReturn);
				return -1;
				break;
		}
	}
}

int		OLFO::RollBack()
{
	int		nReturn;
	nReturn = OCITransRollback(SvcHP, ErrHP, _nOCIMode);
	if (nReturn != OCI_SUCCESS)
	{
		_nErrorCode = -9013;
		sprintf(_strErrorInfo,"调用函数[自定义错误 -9013;错误信息:回滚数据库时出现错误");
		return -1;		
    }
    return 0;
}

int		OLFO::Commit()
{
	int		nReturn;
	nReturn = OCITransCommit(SvcHP, ErrHP, _nOCIMode);
	if (nReturn != OCI_SUCCESS)
	{
		_nErrorCode = -9012;
		sprintf(_strErrorInfo,"调用函数[自定义错误 -9012;错误信息:提交数据库时出现错误");
		return -1;		
    }
    return 0;
}

void	OLFO::DisConnect()
{
	if(_nDateDataPlace > 0)
	{
		for(;_nDateDataPlace > 0; _nDateDataPlace --)
		{
			delete[] pDateDataTmp[_nDateDataPlace - 1];
		}
	}

	if(AutHP)
	{
		OCISessionEnd(SvcHP,ErrHP,AutHP,_nOCIMode);
	}

	if(StmtHP)
	{
		OCIHandleFree(StmtHP,OCI_HTYPE_STMT);
	}
	
	if(AutHP)
	{
		OCIHandleFree(AutHP,OCI_HTYPE_SESSION);
	}
	
	if(SrvHP)
	{
		OCIServerDetach(SrvHP,ErrHP,OCI_DEFAULT);
//		OCIHandleFree(SrvHP,OCI_HTYPE_SERVER);
	}
	
	if(SvcHP)
	{
		OCIHandleFree(SvcHP,OCI_HTYPE_SVCCTX);
	}
/*	
	if(EnvHP)
	{
		OCIHandleFree((dvoid *) EnvHP, OCI_HTYPE_ENV); 
	}
*/
}
	



void 	OLFO::OraDateToC(unsigned char *pDateTime,int &nYY, int &nMM, int &nDD, int &nHH, int &nMI, int &nSS)
{
	if(pDateTime[0] == 0)
	{
		nYY = 0;
		nMM = 0;
		nDD = 0;
		nHH = 0;
		nMI = 0;
		nSS = 0;

		return;
	}

	nYY = ((unsigned int)(unsigned char)abs(pDateTime[0] - 100))*100 + 
		(unsigned int)(unsigned char)abs(pDateTime[1] - 100);
	nMM = pDateTime[2];
	nDD = pDateTime[3];
	nHH = pDateTime[4] - 1;
	nMI = pDateTime[5] - 1;
	nSS = pDateTime[6] - 1;
	return;
}

void 	OLFO::OraDateToC(unsigned char *pDateTime,char *strDataTime)
{
	int nYY,nMM,nDD,nHH,nMI,nSS;
	OraDateToC(pDateTime,nYY,nMM,nDD,nHH,nMI,nSS);
	sprintf(strDataTime,"%04d%02d%02d%02d%02d%02d",nYY,nMM,nDD,nHH,nMI,nSS);
}
	

void 	OLFO::CToOraDate(unsigned char *pDateTime,int nYY, int nMM, int nDD, int nHH, int nMI, int nSS)
{
	pDateTime[0] = (unsigned char)(((unsigned char)(nYY/100)) + 100);
	pDateTime[1] = (unsigned char)((nYY - ((unsigned char)(nYY/100))*100) + 100);
	pDateTime[2] = (unsigned char)nMM;
	pDateTime[3] = (unsigned char)nDD;
	pDateTime[4] = (unsigned char)(nHH + 1);
	pDateTime[5] = (unsigned char)(nMI + 1);
	pDateTime[6] = (unsigned char)(nSS + 1);
	return;
}

int		OLFO::getInt(int	nColumn,int &nData)
{
	if(nColumn <= 0)
	{
		_nErrorCode = -9016;
		sprintf(_strErrorInfo,\
			"调用函数[自定义错误 -9016];错误信息:getString函数的设定列只能大于0");
		return -1;
	}
	if(pColumnInfo[nColumn - 1].nReturnCode == -1)
	{
		nData = 0;
		_nErrorCode = -9070;
		sprintf(_strErrorInfo,\
			"调用函数[自定义警告 -9070];警告信息:getString函数返回了一个NULL值");
		return 1;
	}

	switch(pColumnInfo[nColumn - 1].nDataType)
	{
		case SQLT_UIN:
		case SQLT_INT:
			nData = atoi((char *) pColumnInfo[nColumn - 1].pData);
			break;
		case SQLT_FLT:
			_nErrorCode = -9071;
			sprintf(_strErrorInfo,\
				"调用函数[自定义警告 -9071];警告信息:转换数值时有精度损失");
			nData = (int) atof((char *) pColumnInfo[nColumn - 1].pData);
			return 1;
		default:
			_nErrorCode = -9017;
			sprintf(_strErrorInfo,\
				"调用函数[自定义错误 -9017];错误信息:无法识别的数据类型");
			return -1;
	}
	return 0;
}

int		OLFO::getFloat(int	nColumn,double &fData)
{
	if(nColumn <= 0)
	{
		_nErrorCode = -9016;
		sprintf(_strErrorInfo,\
			"调用函数[自定义错误 -9016];错误信息:getString函数的设定列只能大于0");
		return -1;
	}
	if(pColumnInfo[nColumn - 1].nReturnCode == -1)
	{
		fData = 0;
		_nErrorCode = -9070;
		sprintf(_strErrorInfo,\
			"调用函数[自定义警告 -9070];警告信息:getString函数返回了一个NULL值");
		return 1;
	}

	switch(pColumnInfo[nColumn - 1].nDataType)
	{
		case SQLT_UIN:
		case SQLT_INT:
		case SQLT_FLT:
			fData = atof((char *) pColumnInfo[nColumn - 1].pData);
			break;
		default:
			_nErrorCode = -9017;
			sprintf(_strErrorInfo,\
				"调用函数[自定义错误 -9017];错误信息:无法识别的数据类型");
			return -1;
	}
	return 0;
}


int		OLFO::getString(int	nColumn,char *strData)
{
	if(nColumn <= 0)
	{
		_nErrorCode = -9016;
		sprintf(_strErrorInfo,\
			"调用函数[自定义错误 -9016];错误信息:getString函数的设定列只能大于0");
		return -1;
	}
	if(pColumnInfo[nColumn - 1].nReturnCode == -1)
	{
		strData[0] = '\0';
		_nErrorCode = -9070;
		sprintf(_strErrorInfo,\
			"调用函数[自定义警告 -9016];警告信息:getString函数返回了一个NULL值");
		return 1;
	}
	
	

	switch(pColumnInfo[nColumn - 1].nDataType)
	{
		case SQLT_CHR:
		case SQLT_INT:
		case SQLT_FLT:
		case SQLT_UIN:
		case SQLT_STR:
			strcpy(strData,(char *) pColumnInfo[nColumn - 1].pData);			
			break;
		case SQLT_DAT:
			OraDateToC(pColumnInfo[nColumn - 1].pData,strData);
			break;
		default:
			_nErrorCode = -9017;
			sprintf(_strErrorInfo,\
				"调用函数[自定义错误 -9017];错误信息:无法识别的数据类型[%d]",pColumnInfo[nColumn - 1].nDataType);
			return -1;
	}
	return 0;
}

int		OLFO::getDateTime(int	nColumn,time_t	&tDate)
{
	struct tm tmGetDateTime;
	if(nColumn <= 0)
	{
		_nErrorCode = -9016;
		sprintf(_strErrorInfo,\
			"调用函数[自定义错误 -9016];错误信息:getDateTime函数的设定列只能大于0");
		return -1;
	}
	if(pColumnInfo[nColumn - 1].nReturnCode == -1)
	{
		tDate = '\0';
		_nErrorCode = -9070;
		sprintf(_strErrorInfo,\
			"调用函数[自定义警告 -9016];警告信息:getDateTime函数返回了一个NULL值");
		return 1;
	}

	switch(pColumnInfo[nColumn - 1].nDataType)
	{
		case SQLT_DAT:
			OraDateToC(pColumnInfo[nColumn - 1].pData,tmGetDateTime.tm_year,tmGetDateTime.tm_mon,\
				tmGetDateTime.tm_mday,tmGetDateTime.tm_hour,tmGetDateTime.tm_min,tmGetDateTime.tm_sec);
			tmGetDateTime.tm_year = tmGetDateTime.tm_year - 1900;
			tmGetDateTime.tm_mon --;		
			if((tDate = mktime(&tmGetDateTime)) == -1)
			{
				_nErrorCode = -9020;
				sprintf(_strErrorInfo,\
				"调用函数[ExecSQL -9020];错误信息:转换时间时，发现了未知错误！");
				return -1;
			}
			break;
		default:
			_nErrorCode = -9017;
			sprintf(_strErrorInfo,\
				"调用函数[自定义错误 -9017];错误信息:无法识别的数据类型");
			return -1;
	}
	return 0;
}

int		OLFO::ExecSQL(char	*strSQL,...)
{
	va_list		argPtr;
	
	int			nReturn;									
	OCIStmt		*TmpStmt;	
	ub2 		nSqlType;						//操作SQL类型
	int			nCountCol;						//最大查询列数
	OCIDefine 	*DefNP;
	OCIParam	*PParam;
	ub2			nDataType;						//列的数据类型
	ub2			nDataLen;						//列的数据大小
	int			nPos;							//当前绑定的变量个数
	int			nSQLLen;						//SQL语句长度
	
	unsigned char	*pDateTmp[64];				//为date类型存放临时变量的指针数组
	int			nDatePlace = 0;					//date类型临时指针数组的当前值						
	
	dvoid		**pNumberData;					//存放传入参数的地址数组
	ub2			*pTypeArr;						//存放列的对应的数据类型
	char		**pStrPlace;					//存放每个Fetch出的数据存放地址
	sb2			*pReturnCode;					//存放列的是否为空标志
	int			nCurrPoint = 0;					//存放数组（pNumberData、pStrPlace）的当前位置
	int			nTotalRows;						//当前操作的行数，非查询语句才有效
	
	ub2			nNumberMain;					//数字字段长度
	sb1			nNumberAdd;						//数字字段小数长度
	struct tm 	tmGetDateTime;					//存放临时的时间变量
	struct tm 	*pDateTime;						//存放临时的时间变量
	
	dvoid		*pData;							//存放字符型绑定参数的数据地址
	int			*pIntData;						//存放整型绑定参数的数据地址
	double		dDouData;						//存放浮点型绑定参数的数据地址
	time_t		tTimeData;						//存放时间型绑定参数的数据地址
	int			nFlag = 0;
	
	int			nReturnValue = 0;				//返回值，查询语句返回0或一共有几个字段为空，
												//非查询语句返回一共修改了多少条数据

	va_start(argPtr, strSQL);
	
	if( (nReturn = OCIHandleAlloc( (dvoid *) EnvHP,(dvoid **)  &TmpStmt,
		OCI_HTYPE_STMT, 0, (dvoid **)0)) != OCI_SUCCESS) 
	{
		ErrorOper(nReturn);
		return -1;
	}
	
	nReturn = OCIStmtPrepare (TmpStmt, ErrHP, (text *)strSQL,(ub4)strlen((char *)strSQL),OCI_NTV_SYNTAX, _nOCIMode);
    if( nReturn != OCI_SUCCESS)
	{
		OCIHandleFree(TmpStmt,OCI_HTYPE_STMT);
		ErrorOper(nReturn);
		return -1;
	}
	
	nReturn = OCIAttrGet(TmpStmt, OCI_HTYPE_STMT, &nSqlType,0, OCI_ATTR_STMT_TYPE, ErrHP);
	if( nReturn != OCI_SUCCESS)
	{
		OCIHandleFree(TmpStmt,OCI_HTYPE_STMT);
		ErrorOper(nReturn);
		return -1;
	}
	
	nSQLLen = strlen(strSQL);
	nPos  = 0;
	for(int nPlace = 0;nPlace < nSQLLen;nPlace ++)
	{
		
		if(strSQL[nPlace - 1] == '\'')
		{
			if(nFlag == 1)
			{
				nFlag = 0;
			}
			else
			{
				nFlag = 1;
			}
			continue;
		}
			
		if(strSQL[nPlace] == ':')
		{
			if(nFlag == 1)
			{
				continue;
			}

			nPlace ++;
			switch(strSQL[nPlace])
			{
				case 'N':
				case 'n':
					nPos ++;
					OCIBind  *pIntBind;
					pIntBind = (OCIBind *) 0; 
					pIntData = (int*)argPtr;		
					va_arg(argPtr,int);		
					nReturn = OCIBindByPos(TmpStmt, &pIntBind, ErrHP, nPos, 
						(dvoid *) pIntData, (sword) sizeof(int), SQLT_INT, (dvoid *) 0, 
						(ub2 *) 0, (ub2 *) 0, (ub4) 0, (ub4 *) 0, _nOCIMode);
					if(nReturn != OCI_SUCCESS)
					{
						if(nDatePlace > 0)
						{
							for(int n = 0;n < nDatePlace; n++)
							{
								delete[] pDateTmp[n];
							}
							nDatePlace = 0;
						}
						OCIHandleFree(TmpStmt,OCI_HTYPE_STMT);
						ErrorOper(nReturn);
						return -1;
					}
					break;
				case 'F':
				case 'f':
					nPos ++;
					OCIBind  *pFloatBind;
					pFloatBind = (OCIBind *) 0; 
					dDouData = va_arg(argPtr,double);		
					nReturn = OCIBindByPos(TmpStmt, &pFloatBind, ErrHP, nPos, 
						(dvoid *) &dDouData, (sword) sizeof(double), SQLT_FLT, (dvoid *) 0, 
						(ub2 *) 0, (ub2 *) 0, (ub4) 0, (ub4 *) 0, _nOCIMode);
					if(nReturn != OCI_SUCCESS)
					{
						if(nDatePlace > 0)
						{
							for(int n = 0;n < nDatePlace; n++)
							{
								delete[] pDateTmp[n];
							}
							nDatePlace = 0;
						}
						OCIHandleFree(TmpStmt,OCI_HTYPE_STMT);
						ErrorOper(nReturn);
						return -1;
					}
					break;
				case 'S':
				case 's':
					nPos ++;
					OCIBind  *pCharBind;
					pCharBind = (OCIBind *) 0; 
					pData = (dvoid *) va_arg(argPtr,char *);		
					nReturn = OCIBindByPos(TmpStmt, &pCharBind, ErrHP, nPos, 
						pData, (sword) strlen((char *) pData) + 1, SQLT_STR, (dvoid *) 0, 
						(ub2 *) 0, (ub2 *) 0, (ub4) 0, (ub4 *) 0, _nOCIMode);
					if(nReturn != OCI_SUCCESS)
					{
						if(nDatePlace > 0)
						{
							for(int n = 0;n < nDatePlace; n++)
							{
								delete[] pDateTmp[n];
							}
							nDatePlace = 0;
						}
						OCIHandleFree(TmpStmt,OCI_HTYPE_STMT);
						ErrorOper(nReturn);
						return -1;
					}
					break;
				case 'D':
				case 'd':
					nPos ++;
					OCIBind  *pDateBind;
					pDateBind = (OCIBind *) 0; 
					tTimeData = va_arg(argPtr,time_t);
					pDateTmp[nDatePlace] = new unsigned char[7];
					pDateTime = localtime(&tTimeData);
					CToOraDate(pDateTmp[nDatePlace],pDateTime->tm_year + 1900,pDateTime->tm_mon + 1,\
						pDateTime->tm_mday,pDateTime->tm_hour,pDateTime->tm_min,pDateTime->tm_sec);
												
					nReturn = OCIBindByPos(TmpStmt, &pDateBind, ErrHP, nPos, 
						pDateTmp[nDatePlace], (sword) 7, SQLT_DAT, (dvoid *) 0, 
						(ub2 *) 0, (ub2 *) 0, (ub4) 0, (ub4 *) 0, _nOCIMode);
					if(nReturn != OCI_SUCCESS)
					{
						if(nDatePlace > 0)
						{
							for(int n = 0;n < nDatePlace; n++)
							{
								delete[] pDateTmp[n];
							}
							nDatePlace = 0;
						}
						OCIHandleFree(TmpStmt,OCI_HTYPE_STMT);
						ErrorOper(nReturn);
						return -1;
					}
					nDatePlace ++;
					break;
				default:
					if(nDatePlace > 0)
					{
						for(int n = 0;n < nDatePlace; n++)
						{
							delete[] pDateTmp[n];
						}
						nDatePlace = 0;
					}
					OCIHandleFree(TmpStmt,OCI_HTYPE_STMT);
					_nErrorCode = -9019;
					sprintf(_strErrorInfo,\
						"调用函数[ExecSQL -9019];错误信息:在绑定变量时，类型[%c]不吻合，请转换！",strSQL[nPlace]);
					return -1;
					break;
			}
		}
	}

	if(nSqlType == OCI_STMT_SELECT)
	{
		nReturn = OCIStmtExecute(SvcHP, TmpStmt, ErrHP, (ub4)0, (ub4)0, 0, 0, _nOCIMode);
		if( nReturn != OCI_SUCCESS)
		{
			if(nDatePlace > 0)
			{
				for(int n = 0;n < nDatePlace; n++)
				{
					delete[] pDateTmp[n];
				}
				nDatePlace = 0;
			}
			OCIHandleFree(TmpStmt,OCI_HTYPE_STMT);
			ErrorOper(nReturn);
			return -1;
		}
		
		nReturn = OCIAttrGet((dvoid *) TmpStmt, OCI_HTYPE_STMT, &nCountCol, 0, OCI_ATTR_PARAM_COUNT, ErrHP);
		if( nReturn != OCI_SUCCESS)
		{
			if(nDatePlace > 0)
			{
				for(int n = 0;n < nDatePlace; n++)
				{
					delete[] pDateTmp[n];
				}
				nDatePlace = 0;
			}
			OCIHandleFree(TmpStmt,OCI_HTYPE_STMT);
			ErrorOper(nReturn);
			return -1;
		}
		
		pNumberData = new dvoid *[nCountCol];
		pTypeArr = new ub2[nCountCol];
		pStrPlace = new char *[nCountCol];
		pReturnCode = new sb2[nCountCol];
		
		
		for(int nCountColumn = 1;nCountColumn <= nCountCol;nCountColumn ++)
		{
			nReturn = OCIParamGet(TmpStmt, OCI_HTYPE_STMT, ErrHP, (dvoid **) &PParam, nCountColumn);
			if( nReturn != OCI_SUCCESS)
			{
				if(nDatePlace > 0)
				{
					for(int n = 0;n < nDatePlace; n++)
					{
						delete[] pDateTmp[n];
					}
					nDatePlace = 0;
				}
				OCIHandleFree(TmpStmt,OCI_HTYPE_STMT);
				ErrorOper(nReturn);
				return -1;
			}
			
			nReturn = OCIAttrGet((dvoid *) PParam,OCI_DTYPE_PARAM, \
				(dvoid *) &nDataType,(ub4 *) 0, OCI_ATTR_DATA_TYPE, ErrHP);
			if( nReturn != OCI_SUCCESS)
			{
				if(nDatePlace > 0)
				{
					for(int n = 0;n < nDatePlace; n++)
					{
						delete[] pDateTmp[n];
					}
					nDatePlace = 0;
				}
				OCIHandleFree(TmpStmt,OCI_HTYPE_STMT);
				ErrorOper(nReturn);
				return -1;
			}
			
			nReturn = OCIAttrGet((dvoid *) PParam,OCI_DTYPE_PARAM, \
				(dvoid *) &nDataLen,(ub4 *) 0, OCI_ATTR_DATA_SIZE, ErrHP);
			if( nReturn != OCI_SUCCESS)
			{
				if(nDatePlace > 0)
				{
					for(int n = 0;n < nDatePlace; n++)
					{
						delete[] pDateTmp[n];
					}
					nDatePlace = 0;
				}
				OCIHandleFree(TmpStmt,OCI_HTYPE_STMT);
				ErrorOper(nReturn);
				return -1;
			}

			switch(nDataType)
			{
				case SQLT_NUM:
					nReturn = OCIAttrGet((dvoid *) PParam,OCI_DTYPE_PARAM, \
						(dvoid *) &nNumberMain,(ub4 *) 0, OCI_ATTR_PRECISION, ErrHP);
					if( nReturn != OCI_SUCCESS)
					{
						if(nDatePlace > 0)
						{
							for(int n = 0;n < nDatePlace; n++)
							{
								delete[] pDateTmp[n];
							}
							nDatePlace = 0;
						}
						OCIHandleFree(TmpStmt,OCI_HTYPE_STMT);
						ErrorOper(nReturn);
						return -1;
					}

					nReturn = OCIAttrGet((dvoid *) PParam,OCI_DTYPE_PARAM, \
						(dvoid *) &nNumberAdd,(ub4 *) 0, OCI_ATTR_SCALE, ErrHP);
					if( nReturn != OCI_SUCCESS)
					{
						if(nDatePlace > 0)
						{
							for(int n = 0;n < nDatePlace; n++)
							{
								delete[] pDateTmp[n];
							}
							nDatePlace = 0;
						}
						OCIHandleFree(TmpStmt,OCI_HTYPE_STMT);
						ErrorOper(nReturn);
						return -1;
					}
					
					if(nDataLen <= 0)
					{
						nDataLen = 255;
					}

					if(nNumberAdd == 0)
					{
						pNumberData[nCurrPoint] = (dvoid *) va_arg(argPtr,int *);
						pStrPlace[nCurrPoint] = new char[nDataLen + 1];
						pTypeArr[nCurrPoint] = SQLT_INT;
					}
					else
					{
						pNumberData[nCurrPoint] = (dvoid *) va_arg(argPtr,double *);
						pStrPlace[nCurrPoint] = new char[nDataLen + 1];
						pTypeArr[nCurrPoint] = SQLT_FLT;
					}
					
					nReturn = OCIDefineByPos(TmpStmt,&DefNP,ErrHP,nCountColumn,\
						pStrPlace[nCurrPoint],\
						nDataLen + 1,\
						SQLT_STR,\
						(dvoid *) &(pReturnCode[nCurrPoint]),\
						(ub2 *) 0, (ub2 *) 0, _nOCIMode);
	
					if(nReturn != OCI_SUCCESS)
					{
						if(nDatePlace > 0)
						{
							for(int n = 0;n < nDatePlace; n++)
							{
								delete[] pDateTmp[n];
							}
							nDatePlace = 0;
						}
						OCIHandleFree(TmpStmt,OCI_HTYPE_STMT);
						ErrorOper(nReturn);
						return -1;
					}	
					break;
				case SQLT_AFC:
				case SQLT_CHR:
					pStrPlace[nCurrPoint] = (char *) va_arg(argPtr,char *);
					pTypeArr[nCurrPoint] = SQLT_CHR;
					nReturn = OCIDefineByPos(TmpStmt,&DefNP,ErrHP,nCountColumn,\
						(dvoid *) pStrPlace[nCurrPoint],nDataLen + 1,SQLT_STR,\
						(dvoid *) &(pReturnCode[nCurrPoint]),(ub2 *) 0, (ub2 *) 0, _nOCIMode);
					if(nReturn != OCI_SUCCESS)
					{
						if(nDatePlace > 0)
						{
							for(int n = 0;n < nDatePlace; n++)
							{
								delete[] pDateTmp[n];
							}
							nDatePlace = 0;
						}
						OCIHandleFree(TmpStmt,OCI_HTYPE_STMT);
						ErrorOper(nReturn);
						return -1;
					}
					break;
				case SQLT_DAT:
					pTypeArr[nCurrPoint] = SQLT_DAT;
					nDataLen = 7;
					pNumberData[nCurrPoint] = (dvoid *) va_arg(argPtr,time_t *);
					pStrPlace[nCurrPoint] = new char[nDataLen];
					
					nReturn = OCIDefineByPos(TmpStmt,&DefNP,ErrHP,nCountColumn,\
						(dvoid *) pStrPlace[nCurrPoint],nDataLen,SQLT_DAT,\
						(dvoid *) &(pReturnCode[nCurrPoint]),(ub2 *) 0, (ub2 *) 0, _nOCIMode);
					if(nReturn != OCI_SUCCESS)
					{
						if(nDatePlace > 0)
						{
							for(int n = 0;n < nDatePlace; n++)
							{
								delete[] pDateTmp[n];
							}
							nDatePlace = 0;
						}
						OCIHandleFree(TmpStmt,OCI_HTYPE_STMT);
						ErrorOper(nReturn);
						return -1;
					}
					break;
				case SQLT_RDD:
					pTypeArr[nCurrPoint] = SQLT_STR;
					nDataLen = 18;
					pStrPlace[nCurrPoint] = (char *) va_arg(argPtr,char *);
					
					nReturn = OCIDefineByPos(TmpStmt,&DefNP,ErrHP,nCountColumn,\
						(dvoid *) pStrPlace[nCurrPoint],nDataLen + 1,SQLT_STR,\
						(dvoid *) &(pReturnCode[nCurrPoint]),(ub2 *) 0, (ub2 *) 0, _nOCIMode);
					if(nReturn != OCI_SUCCESS)
					{
						if(nDatePlace > 0)
						{
							for(int n = 0;n < nDatePlace; n++)
							{
								delete[] pDateTmp[n];
							}
							nDatePlace = 0;
						}
						OCIHandleFree(TmpStmt,OCI_HTYPE_STMT);
						ErrorOper(nReturn);
						return -1;
					}
					break;
				default:
					if(nDatePlace > 0)
					{
						for(int n = 0;n < nDatePlace; n++)
						{
							delete[] pDateTmp[n];
						}
						nDatePlace = 0;
					}
					OCIHandleFree(TmpStmt,OCI_HTYPE_STMT);
					_nErrorCode = -9018;
					sprintf(_strErrorInfo,\
						"调用函数[ExecSQL -9018];错误信息:在定义列时，类型[%d]不吻合，请转换！",nDataType);
					return -1;
			}
			nCurrPoint ++;
		}

		nReturn = OCIStmtFetch(TmpStmt,ErrHP, (ub4)1, (ub4) OCI_FETCH_NEXT, (ub4)_nOCIMode);
		va_end(argPtr);
		if( nReturn == OCI_NO_DATA)
		{
			if(nDatePlace > 0)
			{
				for(int n = 0;n < nDatePlace; n++)
				{
					delete[] pDateTmp[n];
				}
				nDatePlace = 0;
			}
			OCIHandleFree(TmpStmt,OCI_HTYPE_STMT);
			return 1000;
		}
		if( nReturn != OCI_SUCCESS)
		{
			if(nDatePlace > 0)
			{
				for(int n = 0;n < nDatePlace; n++)
				{
					delete[] pDateTmp[n];
				}
				nDatePlace = 0;
			}
			OCIHandleFree(TmpStmt,OCI_HTYPE_STMT);
			ErrorOper(nReturn);
			return -1;
		}
	
		nCurrPoint --;
		for(;nCurrPoint >= 0;nCurrPoint -- )
		{
			switch(pTypeArr[nCurrPoint])
			{
				case SQLT_UIN:
				case SQLT_INT:
					*((int *)(pNumberData[nCurrPoint])) = atoi(pStrPlace[nCurrPoint]);
					delete[] pStrPlace[nCurrPoint];
					if(pReturnCode[nCurrPoint] == -1)
					{
						nReturnValue ++;
					}
					break;
				case SQLT_FLT:
					*((double *)(pNumberData[nCurrPoint])) = atof(pStrPlace[nCurrPoint]);
					delete[] pStrPlace[nCurrPoint];
					if(pReturnCode[nCurrPoint] == -1)
					{
						nReturnValue ++;
					}
					break;
				case SQLT_DAT:
					OraDateToC((unsigned char *)pStrPlace[nCurrPoint],tmGetDateTime.tm_year,tmGetDateTime.tm_mon,\
						tmGetDateTime.tm_mday,tmGetDateTime.tm_hour,tmGetDateTime.tm_min,tmGetDateTime.tm_sec);
					tmGetDateTime.tm_year = tmGetDateTime.tm_year - 1900;
					tmGetDateTime.tm_mon --;
					if((*((time_t *)(pNumberData[nCurrPoint])) = mktime(&tmGetDateTime)) == -1)
					{
						if(nDatePlace > 0)
						{
							for(int n = 0;n < nDatePlace; n++)
							{
								delete[] pDateTmp[n];
							}
							nDatePlace = 0;
						}
						OCIHandleFree(TmpStmt,OCI_HTYPE_STMT);
						_nErrorCode = -9020;
						sprintf(_strErrorInfo,\
							"调用函数[ExecSQL -9020];错误信息:转换时间时，发现了未知错误！");
						return -1;
					}
					break;
				case SQLT_CHR:
				case SQLT_STR:
					if(pReturnCode[nCurrPoint] == -1)
					{
						nReturnValue ++;
					}
					break;
			}
		}
		delete[] pNumberData;
		delete[] pTypeArr;
		delete[] pStrPlace;
		delete[] pReturnCode;
		
		if(nDatePlace > 0)
		{
			for(int n = 0;n < nDatePlace; n++)
			{
				delete[] pDateTmp[n];
			}
			nDatePlace = 0;
		}
		OCIHandleFree(TmpStmt,OCI_HTYPE_STMT);
		return nReturnValue;
	}
	else
	{
		nReturn = OCIStmtExecute(SvcHP, TmpStmt, ErrHP, (ub4)1, (ub4)0, 0, 0, _nOCIMode);
		if( nReturn != OCI_SUCCESS)
		{
			if(nDatePlace > 0)
			{
				for(int n = 0;n < nDatePlace; n++)
				{
					delete[] pDateTmp[n];
				}
				nDatePlace = 0;
			}
			OCIHandleFree(TmpStmt,OCI_HTYPE_STMT);
			ErrorOper(nReturn);
			return -1;
		}
		
		nReturn = OCIAttrGet((dvoid*) TmpStmt, OCI_HTYPE_STMT, (dvoid*) &nTotalRows, (ub4 *)0, \
			OCI_ATTR_ROW_COUNT, ErrHP);
		if( nReturn != OCI_SUCCESS)
		{
			if(nDatePlace > 0)
			{
				for(int n = 0;n < nDatePlace; n++)
				{
					delete[] pDateTmp[n];
				}
				nDatePlace = 0;
			}
			OCIHandleFree(TmpStmt,OCI_HTYPE_STMT);
			ErrorOper(nReturn);
			return -1;
		}
		
		if(nDatePlace > 0)
		{
			for(int n = 0;n < nDatePlace; n++)
			{
				delete[] pDateTmp[n];
			}
			nDatePlace = 0;
		}
		OCIHandleFree(TmpStmt,OCI_HTYPE_STMT);
		return nTotalRows;
	}
}

char*	OLFO::getErrorInfo()
{
	if(strlen(_strErrorInfo) > 1)
	{
		if((_strErrorInfo[strlen(_strErrorInfo) - 2] == '\r')||(_strErrorInfo[strlen(_strErrorInfo) - 2] == '\n'))
		{
			_strErrorInfo[strlen(_strErrorInfo) - 2] = '\0';
		}
		if((_strErrorInfo[strlen(_strErrorInfo) - 1] == '\r')||(_strErrorInfo[strlen(_strErrorInfo) - 1] == '\n'))
		{
			_strErrorInfo[strlen(_strErrorInfo) - 1] = '\0';
		}
	}
	return _strErrorInfo;
}

int		OLFO::getErrorNo()
{
	return _nErrorCode;
}

OLFO::OLFO(int nMode)
{
	int		nReturn;

	_nOCIMode = nMode;
	AutHP = (OCISession *) 0;
	_nDateDataPlace = 0;
	_nLineCount = 0;
	AutHP = NULL;
	StmtHP = NULL;
	SrvHP = NULL;
	SvcHP = NULL;
	ErrHP = NULL;
	EnvHP = NULL;
	
	if((nReturn = OCIEnvCreate( (OCIEnv **) &EnvHP, (ub4) nMode,
			(dvoid *) 0, (dvoid * (*)(dvoid *,size_t)) 0,
			(dvoid * (*)(dvoid *, dvoid *, size_t)) 0,
			(void (*)(dvoid *, dvoid *)) 0, (size_t) 0, (dvoid **) 0))
		!= OCI_SUCCESS)
	{
		_nErrorCode = -1;
		sprintf(_strErrorInfo,\
			"调用函数[自定义错误 -1];错误信息:初始数据库环境变量时出现错误,错误码为[%d]",\
			nReturn);
		ErrorOper(nReturn);
		exit(-1);
	}
	
	nReturn = OCIHandleAlloc( (dvoid *) EnvHP,(dvoid **) &ErrHP,OCI_HTYPE_ERROR,\
		(size_t) 0, (dvoid **) 0);
	
	
	nReturn = OCIHandleAlloc( (dvoid *) EnvHP,(dvoid **) &SrvHP,OCI_HTYPE_SERVER,\
		(size_t) 0, (dvoid **) 0);

	_nErrorCode = 0;
	_nTotalColCount = 0;
}


OLFO::OLFO()
{
	int		nReturn;			
	_nOCIMode = OCI_DEFAULT;
	AutHP = (OCISession *) 0;
	_nDateDataPlace = 0;
	_nLineCount = 0;
	AutHP = NULL;
	StmtHP = NULL;
	SrvHP = NULL;
	SvcHP = NULL;
	ErrHP = NULL;
	EnvHP = NULL;
	
	
	
	
	if((nReturn = OCIEnvCreate( (OCIEnv **) &EnvHP, (ub4) _nOCIMode,
			(dvoid *) 0, (dvoid * (*)(dvoid *,size_t)) 0,
			(dvoid * (*)(dvoid *, dvoid *, size_t)) 0,
			(void (*)(dvoid *, dvoid *)) 0, (size_t) 0, (dvoid **) 0))
		!= OCI_SUCCESS)
	{
		_nErrorCode = -1;
		sprintf(_strErrorInfo,\
			"调用函数[自定义错误 -1];错误信息:初始数据库环境变量时出现错误,错误码为[%d]",\
			nReturn);
	}
  	
	OCIHandleAlloc( (dvoid *) EnvHP,(dvoid **) &ErrHP,OCI_HTYPE_ERROR,\
		(size_t) 0, (dvoid **) 0);

	OCIHandleAlloc( (dvoid *) EnvHP,(dvoid **) &SrvHP,OCI_HTYPE_SERVER,\
		(size_t) 0, (dvoid **) 0);

	_nErrorCode = 0;
	_nTotalColCount = 0;
}



OLFO::~OLFO()
{

	if(_nDateDataPlace > 0)
	{
		for(;_nDateDataPlace > 0; _nDateDataPlace --)
		{
			delete[] pDateDataTmp[_nDateDataPlace - 1];
		}
	}

	if(AutHP)
	{
		OCISessionEnd(SvcHP,ErrHP,AutHP,_nOCIMode);
	}

	if(StmtHP)
	{
		OCIHandleFree(StmtHP,OCI_HTYPE_STMT);
	}
	
	if(AutHP)
	{
		OCIHandleFree(AutHP,OCI_HTYPE_SESSION);
	}
	
	if(SrvHP)
	{
		OCIServerDetach(SrvHP,ErrHP,OCI_DEFAULT);
		OCIHandleFree(SrvHP,OCI_HTYPE_SERVER);
	}
	
	if(SvcHP)
	{
		OCIHandleFree(SvcHP,OCI_HTYPE_SVCCTX);
	}
	
	if(ErrHP)
	{
		OCIHandleFree(ErrHP,OCI_HTYPE_ERROR);
	}
	
	if(EnvHP)
	{
		OCIHandleFree((dvoid *) EnvHP, OCI_HTYPE_ENV); 
	}
}




/*
int	main()
{
	OLFO	myora;
	int		nReturn;
	char	strNames[5][11];
	
	nReturn = myora.Connect("sxlt","sxlt","ccpbs");

	myora.SetArrLen(5);
	nReturn = myora.ParseSQL("Select eeee From xbh_temp_01");
	nReturn = myora.Define(1,(char *)strNames,10);
	nReturn = myora.Execute();
	myora.DisConnect();
	
	return 0;
}
*/
/*
int  main()
{
	OLFO	mytext;
	int		number,no,nReturn;
	char	name[5][32]={"songxuan","kimi","juventus","tiger","xbh"};
	char 	strNames[5][20];
	int 	nAge;
	
	no=22;
		
	number=mytext.Connect("hainan","hainan","ccpbs");
	if(number==0)
	{
		printf("登陆数据库成功\n");
	}
	else
	{
		printf("登陆数据库失败\n");
		exit(-1);
	}

	mytext.ExecSQL("select to_number('12.3','99.99') from dual",&nAge);
	
	return 1;
}
*/
