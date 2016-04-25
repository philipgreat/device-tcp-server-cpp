// WhiteListDao.h: interface for the WhiteListDao class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WHITELISTDAO_H__038F0E1B_62EE_4273_A316_7D901342CABE__INCLUDED_)
#define AFX_WHITELISTDAO_H__038F0E1B_62EE_4273_A316_7D901342CABE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ORACLE_OPER.H"
class WhiteListDao  
{
public:
	static int retrieveList(OLFO *oracle,const char *terminalId,char *out,int length);
	static int checkIfUpdated(OLFO *oracle, const char *terminalId,int version);
	WhiteListDao();
	virtual ~WhiteListDao();

};

#endif // !defined(AFX_WHITELISTDAO_H__038F0E1B_62EE_4273_A316_7D901342CABE__INCLUDED_)
