// HiringDao.h: interface for the HiringDao class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HIRINGDAO_H__7356DC05_A7FD_433B_8613_6CF01BC65495__INCLUDED_)
#define AFX_HIRINGDAO_H__7356DC05_A7FD_433B_8613_6CF01BC65495__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ORACLE_OPER.H"
class HiringDao
{
public:
	static int restoreTimeoutHireRequest(OLFO *oracle);
	static int updateLastHeartBeatTime(OLFO *oracle,const char * terminalId);
	static int checkIfCanBeHired(OLFO *oracle,const char* terminalId);	
	static int checkIfCanBeFired(OLFO *oracle,const char* terminalId);
	static int confirmHiring(OLFO *oracle,const char *terminalId);
	static int checkIfTendToHire(OLFO *oracle, const char *terminalId);
	static int fire(OLFO *oracle,const char *terminalId);
	static int hire(OLFO *oracle,const char *terminalId);
	HiringDao();
	virtual ~HiringDao();
};

#endif // !defined(AFX_HIRINGDAO_H__7356DC05_A7FD_433B_8613_6CF01BC65495__INCLUDED_)
