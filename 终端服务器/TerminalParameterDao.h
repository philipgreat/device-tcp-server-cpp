// TerminalParameterDao.h: interface for the TerminalParameterDao class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TERMINALPARAMETERDAO_H__F175E25C_D6F9_440E_A1F7_A2736D81639B__INCLUDED_)
#define AFX_TERMINALPARAMETERDAO_H__F175E25C_D6F9_440E_A1F7_A2736D81639B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ORACLE_OPER.H"
class TerminalParameterDao  
{
public:
	static int getParameterExpr(OLFO *oracle,const char *terminalId,char *out,int length);
	TerminalParameterDao();
	virtual ~TerminalParameterDao();

};

#endif // !defined(AFX_TERMINALPARAMETERDAO_H__F175E25C_D6F9_440E_A1F7_A2736D81639B__INCLUDED_)
