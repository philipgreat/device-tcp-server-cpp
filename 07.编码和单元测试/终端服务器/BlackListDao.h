// BlackListDao.h: interface for the BlackListDao class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLACKLISTDAO_H__DE805893_F8FF_470D_BDA1_B6E8A7D7382D__INCLUDED_)
#define AFX_BLACKLISTDAO_H__DE805893_F8FF_470D_BDA1_B6E8A7D7382D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ORACLE_OPER.H"
class BlackListDao  
{
public:
	static int retrieveList(OLFO *oracle,const char *terminalId,char *out,int length);
	static int checkIfUpdated(OLFO *oracle, const char *terminalId,int version);
	BlackListDao();
	virtual ~BlackListDao();
};

#endif // !defined(AFX_BLACKLISTDAO_H__DE805893_F8FF_470D_BDA1_B6E8A7D7382D__INCLUDED_)
