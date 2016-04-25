/*-----------------------------------------------------------------------------------------------------------------*\
|										Oracle Lib For OCI	-- By IceCool											|
|											Ver 0.99 B	 2007.01													|
|用法说明：                                                                                                         |
|公用方法：                                                                                                         |
|1、首先都需要调用Connect进行数据库连接。                                                                           |
|2、完成后使用DisConnect断开数据库连接。      	                                                                    |
|3、线程模式下使用构造函数OLFO(OCI_THREADED)来定义对象                                                              |
|                                                                                                                   |
|一、使用数组向数据库插入数据：                                                                                     |
|1、调用SetArrLen函数对数组大小进行设定，最大为MAXARRLINE（32512）；                                                |
|2、调用ParseSQL对SQL语句进行分析；                                                                                 |
|3、调用Bind进行变量数组绑定，可以对4种数据类型进行绑定：字符串、整形、双倍精度浮点（double）、时间类型（time_t）； |
|4、调用Execute函数进行执行；                                                                                       |
|5、调用Commit提交；																								|
|例：                                                                                                               |
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
|二、使用数组接收从数据库提取的数据：                                                                               |
|1、调用SetArrLen函数对数组大小进行设定，最大为MAXARRLINE（32512）；                                                |
|2、调用ParseSQL对SQL语句进行分析；                                                                                 |
|3、调用Define函数进行输出列与数组的绑定，可以定义4种数据类型的输出：字符串、整形、双倍精度浮点数、                 |
|	和时间类型；                                                                                                    |
|4、调用Execute函数进行执行；                                                                                       |
|例：																												|
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
|三、使用游标获取数据：                                                                                             |
|1、调用ParseSQL对SQL语句进行分析；                                                                                 |
|2、调用Bind进行变量绑定，可以对4种数据类型进行绑定：字符串、整形、双倍精度浮点、时间类型；                         |
|3、调用Open函数打开游标；                                                                                          |
|4、使用Fetch顺序移动游标；                                                                                         |
|5、使用getString、getInt、getFloat、getDateTime分别提取字符串、整形、双倍精度浮点、时间类型的数据；                |
|6、使用Close函数关闭游标；                                                                                         |
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
|四、执行单条非查询SQL语句：                                                                                        |
|1、调用ExecSQL函数，第一个参数是SQL语句，后面依次是和SQL语句中以":"为起始标记的虚拟变量进行绑定的                  |
|	实际变量，变量的类型由SQL语句中绑定的虚拟变量名第一个字符来决定,:s表示字符串，:d表示时间，:f表示                |
|	双倍精度浮点数，:n表示整形数；                                                                                  |
|                                                                                                                   |
|	OLFO	myOHandle;                                                                                              |
|	int		nAge;                                                                                                   |
|	char	strNames[11];                                                                                           |
|	time_t	tCreateDate;                                                                                            |
|	                                                                                                                |
|	nAge = 16;                                                                                                      |
|	myOHandle.Connect("sxlt","sxlt","ccpbs");                                                                       |
|	tCreateDate = time(NULL);																						|
|	strcpy(strNames,"张三");																						|
|	nAge = 16;																										|
|	myOHandle.ExecSQL("Inser Into temp_test(Names,Age,CreateDate) Values(:s1,:n2,:d3)",strNames,nAge,tCreateDate);  |
|	myOHandle.Commit();																								|
|	myHandle.DisConnect();                                                       									|
|                                                                                                                   |
|五、执行单条查询SQL语句：                                                                                          |
|1、调用ExecSQL函数，第一个参数是SQL语句，后面参数分为两部分，前一部分和非查询SQL语句时一样，绑定的                 |
|	输入参数变量后面，是查询SQL输出的结果，这个输出结果中，程序不能进行自动转换。									|
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
|补充说明：																											|
|1、程序实现了time_t和oracle中date类型的无缝转换，请尽量使用time_t类型处理oracle的时间字段；						|
|2、C语言双倍精度浮点类型存在无法精确表示的问题，虽然与oracle的处理中没有问题，但是请尽量少用；						|
|3、本版本为测试版本，有问题请Mail To：icecoolbear@163.com;															|
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
	ub1 		*pData;													//Fetch出一个列的数据内存位置
	ub2			nDataLen;												//这个列的大小
	ub2			nDataType;												//这个列的类型
	ub1			cIsNull;												//这个列在建表语法中是否允许为Null
	char		pColName[ORACLE_MAX_CLOUMN_LEN];						//这个列的列名
	ub2			nNumberMain;											//如果是数字的话，数字长度
	sb1			nNumberAdd;												//如果是数字的话，小数长度
	sb2			nReturnCode;											//判断返回的数据是不是一个Null
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

		int					_nErrorCode;								//本次操作错误代码							
		char				_strErrorInfo[640];							//本次操作的错误信息
		ub2 				_nSqlType;									//SQL语法的类型
		int					_nTotalColCount;							//查询语句一共查询了多少个列
		        			
		void				ErrorOper(sword status);					//错误信息提取程序
		int					_nLineCount;								//在执行数组操作中，数组的大小

		DATA_COLUMN_INFO	*pColumnInfo;								//每一个Fetch操作释放出的信息结构
		unsigned char		*pDateDataTmp[64];							//绑定日期变量时存放中间结果数组
		time_t				*pDateData[64];								//绑定日期变量时存放变量地址数组
		int					_nDateDataPlace;							//绑定日期变量时存放中间结果数组下标
		
		int					_nOCIMode;									//模式
		
		void 	OraDateToC(unsigned char*, int&, int&, int&, int&, int&, int&);
																		//将Oracle的时间字段转换成6个int
		void 	OraDateToC(unsigned char*,char*);						//将Oracle的时间字段转换成一个字符串
		void 	CToOraDate(unsigned char*, int , int , int , int , int , int );
																		//将6个int表示的时间设置成Oracle的时间字段	
	public:
		int		Connect(char *,char *,char *);							//连接数据库的函数
		void	DisConnect();											//断开数据库连接的函数
		
		void	SetArrLen(int);											//设置操作数组的大小，在数组操作前必须调用此函数

		int		ParseSQL(char *);										//分析一个SQL语法
		int		Open();													//打开一个Fetch操作
		int		Fetch();												//一次依次向下提取一行数据
		void	Close();												//释放Fetch存放数据的内存区域
		int		Execute();												//执行一个SQL语句，一般用来执行数组操作
		int		ExecSQL(char *,...);									//执行一个查询和修改的SQL语句，不能执行数组数据
																		//的批量操作，请注意，参数必须先带Where条件中
																		//绑定以:打头的变量，并且使用第一个字s（字符串）、
																		//n（整形数字）、f（浮点数字）来区分不同绑定的
																		//类型。
		int		Commit();												//向数据库提交数据
		int		RollBack();												//向数据库回退数据
		
		int		Bind(char *,char *,int);								//字符串数据根据列名绑定
		int		Bind(char *,int[]);										//整形数组据根据列名绑定
		int		Bind(char *,char *);									//字符串据根据列名绑定
		int		Bind(char *,int&);										//整形数字据根据列名绑定
		int		Bind(char *,double[]);									//浮点数组据根据列名绑定
		int		Bind(char *,double&);									//浮点数字据根据列名绑定
		int		Bind(int,char *,int);									//字符串数据根据位置绑定
		int		Bind(int,int[]);										//整形数组据根据位置绑定
		int		Bind(int,char *);										//字符串据根据位置绑定
		int		Bind(int,int&);											//整形数字据根据位置绑定
		int		Bind(int,double[]);										//浮点数组据根据位置绑定
		int		Bind(int,double&);										//浮点数字据根据位置绑定
		int		Bind(char *,time_t[]);									//时间型变量绑定
		int		Bind(char *,time_t&);									//时间型变量绑定
		int		Bind(int,time_t&);										//时间型变量绑定
		int		Bind(int,time_t[]);										//时间型变量绑定
		
		int		Define(int, char *,int);								//获取字符串数组数据输出
		int		Define(int, int[]);										//获取整形数字数组输出
		int		Define(int, double[]);									//获取浮点数字数组输出
		int		Define(int, time_t[]);									//获取时间数组输出
		
		int		getString(int,char *);									//获取字符串数据输出
		int 	getInt(int, int&);										//获取数字输出
		int 	getFloat(int, double&);									//获取数字输出
		int 	getDateTime(int, time_t&);								//获取日期时间输出
		
		char	*getErrorInfo();
		int		getErrorNo();

		OLFO(int);
		OLFO();
		~OLFO();

};

#endif
