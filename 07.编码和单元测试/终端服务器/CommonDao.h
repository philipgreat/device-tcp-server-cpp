// CommonDao.h: interface for the CommonDao class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMONDAO_H__A9077D5B_3DAA_404D_AF19_8C0328DE20FF__INCLUDED_)
#define AFX_COMMONDAO_H__A9077D5B_3DAA_404D_AF19_8C0328DE20FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ORACLE_OPER.H"
/**********************************************************************
 ��������: CommonDao
 ��������: ����һ������񣬱���������ͨ���ն˺�Ѱ�ҾƵ��ID����¼������
		   ���Oracle�ĵ�ǰʱ��
 �������:
 �� �� ֵ: 0-�ɹ�,����-ʧ��
 ȫ�ֱ���:
 ��д����: zhangxilai
 ��д����: 2008��10��13��
 �����־
**********************************************************************/
class CommonDao  
{
public:

	static time_t getDatabaseTime(OLFO *oracle);
	static int rollback(OLFO *oracle);
	static int commit(OLFO * oracle);
	static char * getString(OLFO *oracle,char* outcontent,const char *sql);
	static int getInt(OLFO *oracle,const char *sql);
	CommonDao();
	virtual ~CommonDao();
};

void splitTerminalIds(const char *terminalId,char *imei,char *uim);
int  checkOracleResultsetError(int ret,OLFO  *oracle);
void log(const char *function,int line,const char *format,...);
#endif // !defined(AFX_COMMONDAO_H__A9077D5B_3DAA_404D_AF19_8C0328DE20FF__INCLUDED_)
