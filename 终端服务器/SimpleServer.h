// SimpleServer.h: interface for the SimpleServer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMPLESERVER_H__358EEAF4_6123_4E9E_A94F_389C25E37288__INCLUDED_)
#define AFX_SIMPLESERVER_H__358EEAF4_6123_4E9E_A94F_389C25E37288__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class SimpleServer  
{
public:
	static void  sleep(int miliseconds);
	static int  start(void *pTask);
	int run(void *pTask);
	SimpleServer();
	virtual ~SimpleServer();

};

#endif // !defined(AFX_SIMPLESERVER_H__358EEAF4_6123_4E9E_A94F_389C25E37288__INCLUDED_)
