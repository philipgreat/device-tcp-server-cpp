// AdStatsDao.h: interface for the AdStatsDao class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADSTATSDAO_H__E276B4CE_10B1_4F6D_8865_8ADD8807616D__INCLUDED_)
#define AFX_ADSTATSDAO_H__E276B4CE_10B1_4F6D_8865_8ADD8807616D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ORACLE_OPER.H"
class AdStatsDao  
{
public:
	static int addToStats(OLFO *oracle,const char *terminalId, const char *body);
	AdStatsDao();
	virtual ~AdStatsDao();

};

#endif // !defined(AFX_ADSTATSDAO_H__E276B4CE_10B1_4F6D_8865_8ADD8807616D__INCLUDED_)
