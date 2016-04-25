// MobileSimulator.h : main header file for the MOBILESIMULATOR application
//

#if !defined(AFX_MOBILESIMULATOR_H__A0A6DA85_4862_4781_8789_8415D8FA80CC__INCLUDED_)
#define AFX_MOBILESIMULATOR_H__A0A6DA85_4862_4781_8789_8415D8FA80CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMobileSimulatorApp:
// See MobileSimulator.cpp for the implementation of this class
//
#include "Logger.h"
class CMobileSimulatorApp : public CWinApp
{
private:
	char serverIp[36];
	int  serverPort;
	char terminalId[36];
	char parameters[1024];
	Logger *logger;
public:
	int getServerPort();
	char * getServerIp();
	Logger* getLogger();
	char * getParameters();
	char * getTerminalId();
	CMobileSimulatorApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMobileSimulatorApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMobileSimulatorApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOBILESIMULATOR_H__A0A6DA85_4862_4781_8789_8415D8FA80CC__INCLUDED_)
