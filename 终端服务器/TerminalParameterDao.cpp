// TerminalParameterDao.cpp: implementation of the TerminalParameterDao class.
//
//////////////////////////////////////////////////////////////////////

#include "TerminalParameterDao.h"
#include "CommonDao.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TerminalParameterDao::TerminalParameterDao()
{

}

TerminalParameterDao::~TerminalParameterDao()
{

}

/*
����ϵͳ�����Ĵ�������Э����û��ϵͳ�����İ汾�ţ��޷������ն˰汾״̬������ϵͳĿǰ��������
ƽ̨�����������ն˲�����t_terminal_param���в���һ����¼�������޸ĵĲ�������ڸñ��У�����ñ��д���һ��ƥ��ļ�¼��ƥ��������£�
1.	ͨ���'*'����ʾ����ǰ���е��ն˲�������Ҫ�޸�
2.	�����IMEI�ţ�ָ����IMEI��UIM���ֻ������������ֻ������޸ġ�
��ϸ��Ϣ��μ��ն˲�����t_terminal_param����
����Ϊ��ϵͳ�����·�������ڼ���ն˵�ÿ�����������ն˲������µĲ�����
���Ϊ*����ÿ���ն˵Ĳ��������¡�
drop table t_terminal_parm;
Create Table t_terminal_parm			--��CMPP���ط������ݻذ���״̬����Ķ�Ӧ��
(
	id			number(9),	--һ��Ωһ�����
	Imei_Number	varchar2(15),	--*����ʾƥ�������ֻ���imeiƥ��ĳ���̶��ֻ�
	available	number(1),	--0�������ã�1������
	Parameters	char(256),	--WPPREF=http:// SVRIP=192.168.1.6
	create_time	date,	--����ʱ��
	modify_time	date	--�޸�ʱ��
);
*/

/**********************************************************************
 ��������: getParameterExpr
 ��������: �����ݿ���ȡ��
 �������:
 �� �� ֵ: 0-�ɹ���С��0-ʧ�ܣ����߿ջ���û�м�¼����������Ҫͬ��ϵͳ����
 ȫ�ֱ���:
 ��д����: zhangxilai
 ��д����: 2009��01��23��
 �����־: 
 ��� SQL��
**********************************************************************/

int TerminalParameterDao::getParameterExpr(OLFO *oracle, const char *terminalId, char *out, int length)
{
	if(length<1024){
		return -1;
	}
	char imei[16];
	char uim[16];
	splitTerminalIds(terminalId,imei,uim);
	int ret = 0;
	char sql[1024];
	*out=0;
	if(0>(ret=oracle->ExecSQL("select Parameters \
		from t_terminal_parm \
		where (imei_number=:s1) and available=1",imei,out)))
	{
		printf("sql: %s %s \n",sql,oracle->getErrorInfo());
		return -2;
	}

	if(ret!=1000){
		return 0;
	}
	//ret=1000,û������һ��,������ƥ���ǺŲ�
	if(0>(ret=oracle->ExecSQL("select Parameters \
		from t_terminal_parm \
		where ( imei_number='*') and available=1",out)))
	{
		printf("sql: %s %s \n",sql,oracle->getErrorInfo());
		return -3;
	}
	
	if(ret==1000){
		//ʵ���Ҳ�����,û���ҵ�
		return -1;
	}
	return 0;
}
