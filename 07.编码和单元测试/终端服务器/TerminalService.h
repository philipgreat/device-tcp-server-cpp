// TerminalService.h: interface for the TerminalService class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TERMINALSERVICE_H__774E6651_9C2F_4F93_BF87_923C2D209F06__INCLUDED_)
#define AFX_TERMINALSERVICE_H__774E6651_9C2F_4F93_BF87_923C2D209F06__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ORACLE_OPER.H"
class TerminalService  
{
public:
	static int getTerminalParameter(OLFO *oracle, const char *terminalId,char*out,int length);
	static int rollback(OLFO *oracle);
	static int fire(OLFO *oracle,const char *terminalId);
	static int commit(OLFO *oracle);
	static int hire(OLFO *oracle,const char *teminalId);
	TerminalService();
	virtual ~TerminalService();

};

#endif // !defined(AFX_TERMINALSERVICE_H__774E6651_9C2F_4F93_BF87_923C2D209F06__INCLUDED_)
