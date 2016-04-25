/*-----------------------------------------------------------------------------------------------------------------*\
|										Oracle Lib For OCI	-- By IceCool											|
|											Ver 0.99 B	 2007.01													|
|�÷�˵����                                                                                                         |
|���÷�����                                                                                                         |
|1�����ȶ���Ҫ����Connect�������ݿ����ӡ�                                                                           |
|2����ɺ�ʹ��DisConnect�Ͽ����ݿ����ӡ�      	                                                                    |
|3���߳�ģʽ��ʹ�ù��캯��OLFO(OCI_THREADED)���������                                                              |
|                                                                                                                   |
|һ��ʹ�����������ݿ�������ݣ�                                                                                     |
|1������SetArrLen�����������С�����趨�����ΪMAXARRLINE��32512����                                                |
|2������ParseSQL��SQL�����з�����                                                                                 |
|3������Bind���б�������󶨣����Զ�4���������ͽ��а󶨣��ַ��������Ρ�˫�����ȸ��㣨double����ʱ�����ͣ�time_t���� |
|4������Execute��������ִ�У�                                                                                       |
|5������Commit�ύ��																								|
|����                                                                                                               |
|	OLFO	myOHandle;                                                                                              |
|	int		nAge[5];                                                                                                |
|	char	strNames[5][11]                                                                                         |
|                                                                                                                   |
|	myOHandle.Connect("sxlt","sxlt","ccpbs");                                                                       |
|	myOHandle.SetArrLen(5);                                                                                         |
|	myOHandle.ParseSQL("Inser Into tmp_test(Names,Age) Values(:s1,:n2)");                                           |
|	myOHandle.Bind("s1",(char *)(strNames[1]),10);                                                                       |
|	myOHandle.Bind("n2",nAge);                                                                                      |
|	myOHandle.Execute();                                                                                            |
|	myOHandle.Commit();                                                                                             |
|	myOHandle.DisConnect();                                                                                         |
|                                                                                                                   |
|����ʹ��������մ����ݿ���ȡ�����ݣ�                                                                               |
|1������SetArrLen�����������С�����趨�����ΪMAXARRLINE��32512����                                                |
|2������ParseSQL��SQL�����з�����                                                                                 |
|3������Define�������������������İ󶨣����Զ���4���������͵�������ַ��������Ρ�˫�����ȸ�������                 |
|	��ʱ�����ͣ�                                                                                                    |
|4������Execute��������ִ�У�                                                                                       |
|����																												|
|	OLFO	myOHandle;                                                                                              |
|	int		nAge[5];                                                                                                |
|	char	strNames[5][11];                                                                                         |
|																													|
|	myOHandle.Connect("sxlt","sxlt","ccpbs");                                                                       |
|	myOHandle.SetArrLen(5);                                                                                         |
|	myOHandle.ParseSQL("Select Names,Age From temp_test");															|
|	myOHandle.Define(1,(char *)strNames,10);																		|
|	myOHandle.Define(2,nAge);                                                                                       |
|	myOHandle.Execute();                                                                                            |
|	myOHandle.DisConnect();                                                                                         |
|                                                                                                                   |
|			                                                                                                        |
|����ʹ���α��ȡ���ݣ�                                                                                             |
|1������ParseSQL��SQL�����з�����                                                                                 |
|2������Bind���б����󶨣����Զ�4���������ͽ��а󶨣��ַ��������Ρ�˫�����ȸ��㡢ʱ�����ͣ�                         |
|3������Open�������αꣻ                                                                                          |
|4��ʹ��Fetch˳���ƶ��αꣻ                                                                                         |
|5��ʹ��getString��getInt��getFloat��getDateTime�ֱ���ȡ�ַ��������Ρ�˫�����ȸ��㡢ʱ�����͵����ݣ�                |
|6��ʹ��Close�����ر��αꣻ                                                                                         |
|                                                                                                                   |
|	int		nAge;                                                                                                   |
|	char	strNames[11];                                                                                           |
|                                                                                                                   |
|	myOHandle.Connect("sxlt","sxlt","ccpbs");                                                                       |
|	myOHandle.ParseSQL("Select Names From temp_test Where Age = :Age");                                             |
|	myOHandle.Bind(1,nAge);                                                                                         |
|	myOHandle.Open();                                                                                               |
|	While(Fetch())                                                                                             |
|	{                                                                                                               |
|		myOHandle.getString(1,(char *) strNames);                                                                   |
|	}                                                                                                               |
|	myOHandle.Close();                                                                                              |
|	myOHandle.DisConnect();                                                                                         |
|                                                                                                                   |
|                                                                                                                   |
|�ġ�ִ�е����ǲ�ѯSQL��䣺                                                                                        |
|1������ExecSQL��������һ��������SQL��䣬���������Ǻ�SQL�������":"Ϊ��ʼ��ǵ�����������а󶨵�                  |
|	ʵ�ʱ�����������������SQL����а󶨵������������һ���ַ�������,:s��ʾ�ַ�����:d��ʾʱ�䣬:f��ʾ                |
|	˫�����ȸ�������:n��ʾ��������                                                                                  |
|                                                                                                                   |
|	OLFO	myOHandle;                                                                                              |
|	int		nAge;                                                                                                   |
|	char	strNames[11];                                                                                           |
|	time_t	tCreateDate;                                                                                            |
|	                                                                                                                |
|	nAge = 16;                                                                                                      |
|	myOHandle.Connect("sxlt","sxlt","ccpbs");                                                                       |
|	tCreateDate = time(NULL);																						|
|	strcpy(strNames,"����");																						|
|	nAge = 16;																										|
|	myOHandle.ExecSQL("Inser Into temp_test(Names,Age,CreateDate) Values(:s1,:n2,:d3)",strNames,nAge,tCreateDate);  |
|	myOHandle.Commit();																								|
|	myHandle.DisConnect();                                                       									|
|                                                                                                                   |
|�塢ִ�е�����ѯSQL��䣺                                                                                          |
|1������ExecSQL��������һ��������SQL��䣬���������Ϊ�����֣�ǰһ���ֺͷǲ�ѯSQL���ʱһ�����󶨵�                 |
|	��������������棬�ǲ�ѯSQL����Ľ��������������У������ܽ����Զ�ת����									|
|	OLFO	myOHandle;                                                                                              |
|	int		nAge;                                                                                                   |
|	char	strNames[11];                                                                                           |
|	time_t	tCreateDate;                                                                                            |
|	                                                                                                                |
|	nAge = 16;                                                                                                      |
|	myOHandle.Connect("sxlt","sxlt","ccpbs");                                                                       |
|	myOHandle.ExecSQL("Select Names,CreateDate From temp_test where age = :n1",nAge,strNames,&tCreateDate);			|
|   myOHandle.DisConnect();                                                       									|
|                                                                                                                   |
|																													|
|����˵����																											|
|1������ʵ����time_t��oracle��date���͵��޷�ת�����뾡��ʹ��time_t���ʹ���oracle��ʱ���ֶΣ�						|
|2��C����˫�����ȸ������ʹ����޷���ȷ��ʾ�����⣬��Ȼ��oracle�Ĵ�����û�����⣬�����뾡�����ã�						|
|3�����汾Ϊ���԰汾����������Mail To��icecoolbear@163.com;															|
|                                                                                                                   |
\*-----------------------------------------------------------------------------------------------------------------*/



#ifndef __OLFO_ICE_VER__
#define __OLFO_ICE_VER__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include <oci.h>
#include <ocidfn.h>
#include <ocidem.h>
#include <stdarg.h>

#define		ORACLE_MAX_CLOUMN_LEN	128
#define		MAXARRLINE				32512

typedef struct {
	ub1 		*pData;													//Fetch��һ���е������ڴ�λ��
	ub2			nDataLen;												//����еĴ�С
	ub2			nDataType;												//����е�����
	ub1			cIsNull;												//������ڽ����﷨���Ƿ�����ΪNull
	char		pColName[ORACLE_MAX_CLOUMN_LEN];						//����е�����
	ub2			nNumberMain;											//��������ֵĻ������ֳ���
	sb1			nNumberAdd;												//��������ֵĻ���С������
	sb2			nReturnCode;											//�жϷ��ص������ǲ���һ��Null
}DATA_COLUMN_INFO;


class	OLFO 
{
	private:
		OCIEnv				*EnvHP;
		OCIServer			*SrvHP;
		OCISvcCtx			*SvcHP;
		OCIError			*ErrHP;
		OCISession			*AutHP;
		OCIStmt				*StmtHP;

		int					_nErrorCode;								//���β����������							
		char				_strErrorInfo[640];							//���β����Ĵ�����Ϣ
		ub2 				_nSqlType;									//SQL�﷨������
		int					_nTotalColCount;							//��ѯ���һ����ѯ�˶��ٸ���
		        			
		void				ErrorOper(sword status);					//������Ϣ��ȡ����
		int					_nLineCount;								//��ִ����������У�����Ĵ�С

		DATA_COLUMN_INFO	*pColumnInfo;								//ÿһ��Fetch�����ͷų�����Ϣ�ṹ
		unsigned char		*pDateDataTmp[64];							//�����ڱ���ʱ����м�������
		time_t				*pDateData[64];								//�����ڱ���ʱ��ű�����ַ����
		int					_nDateDataPlace;							//�����ڱ���ʱ����м��������±�
		
		int					_nOCIMode;									//ģʽ
		
		void 	OraDateToC(unsigned char*, int&, int&, int&, int&, int&, int&);
																		//��Oracle��ʱ���ֶ�ת����6��int
		void 	OraDateToC(unsigned char*,char*);						//��Oracle��ʱ���ֶ�ת����һ���ַ���
		void 	CToOraDate(unsigned char*, int , int , int , int , int , int );
																		//��6��int��ʾ��ʱ�����ó�Oracle��ʱ���ֶ�	
	public:
		int		Connect(char *,char *,char *);							//�������ݿ�ĺ���
		void	DisConnect();											//�Ͽ����ݿ����ӵĺ���
		
		void	SetArrLen(int);											//���ò�������Ĵ�С�����������ǰ������ô˺���

		int		ParseSQL(char *);										//����һ��SQL�﷨
		int		Open();													//��һ��Fetch����
		int		Fetch();												//һ������������ȡһ������
		void	Close();												//�ͷ�Fetch������ݵ��ڴ�����
		int		Execute();												//ִ��һ��SQL��䣬һ������ִ���������
		int		ExecSQL(char *,...);									//ִ��һ����ѯ���޸ĵ�SQL��䣬����ִ����������
																		//��������������ע�⣬���������ȴ�Where������
																		//����:��ͷ�ı���������ʹ�õ�һ����s���ַ�������
																		//n���������֣���f���������֣������ֲ�ͬ�󶨵�
																		//���͡�
		int		Commit();												//�����ݿ��ύ����
		int		RollBack();												//�����ݿ��������
		
		int		Bind(char *,char *,int);								//�ַ������ݸ���������
		int		Bind(char *,int[]);										//��������ݸ���������
		int		Bind(char *,char *);									//�ַ����ݸ���������
		int		Bind(char *,int&);										//�������־ݸ���������
		int		Bind(char *,double[]);									//��������ݸ���������
		int		Bind(char *,double&);									//�������־ݸ���������
		int		Bind(int,char *,int);									//�ַ������ݸ���λ�ð�
		int		Bind(int,int[]);										//��������ݸ���λ�ð�
		int		Bind(int,char *);										//�ַ����ݸ���λ�ð�
		int		Bind(int,int&);											//�������־ݸ���λ�ð�
		int		Bind(int,double[]);										//��������ݸ���λ�ð�
		int		Bind(int,double&);										//�������־ݸ���λ�ð�
		int		Bind(char *,time_t[]);									//ʱ���ͱ�����
		int		Bind(char *,time_t&);									//ʱ���ͱ�����
		int		Bind(int,time_t&);										//ʱ���ͱ�����
		int		Bind(int,time_t[]);										//ʱ���ͱ�����
		
		int		Define(int, char *,int);								//��ȡ�ַ��������������
		int		Define(int, int[]);										//��ȡ���������������
		int		Define(int, double[]);									//��ȡ���������������
		int		Define(int, time_t[]);									//��ȡʱ���������
		
		int		getString(int,char *);									//��ȡ�ַ����������
		int 	getInt(int, int&);										//��ȡ�������
		int 	getFloat(int, double&);									//��ȡ�������
		int 	getDateTime(int, time_t&);								//��ȡ����ʱ�����
		
		char	*getErrorInfo();
		int		getErrorNo();

		OLFO(int);
		OLFO();
		~OLFO();

};

#endif
