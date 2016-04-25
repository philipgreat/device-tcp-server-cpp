// MobileSimulator.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MobileSimulator.h"
#include "MobileSimulatorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMobileSimulatorApp

BEGIN_MESSAGE_MAP(CMobileSimulatorApp, CWinApp)
	//{{AFX_MSG_MAP(CMobileSimulatorApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMobileSimulatorApp construction
#include "Configuration.h"
CMobileSimulatorApp::CMobileSimulatorApp()
{
	Configuration *config=new Configuration();
	if(config->load(".\\config.ini")!=0){
		printf("error load file\n");
		::MessageBox(NULL,
			"请注意，您在程序目录下应该存在一个config.ini文件，否则无法继续处理",
			"告警",
			MB_OK|MB_ICONERROR);
		this->ExitInstance();
		exit(0);
	}
	
	logger=new Logger(".\\terminal.log");
	config->parse();
	config->report();
	strcpy(this->terminalId,config->getValue("terminal.id"));
	strcpy(this->parameters,config->getValue("terminal.params"));	
	strcpy(this->serverIp,config->getValue("termserver.ip"));
	this->serverPort=atoi(config->getValue("termserver.port"));
	delete config;
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMobileSimulatorApp object

CMobileSimulatorApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMobileSimulatorApp initialization

BOOL CMobileSimulatorApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CMobileSimulatorDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

char * CMobileSimulatorApp::getTerminalId()
{
	return this->terminalId;
}

char * CMobileSimulatorApp::getParameters()
{	
	return this->parameters;
}

Logger* CMobileSimulatorApp::getLogger()
{
	return this->logger;
}

char * CMobileSimulatorApp::getServerIp()
{
	return this->serverIp;
}

int CMobileSimulatorApp::getServerPort()
{
	return this->serverPort;
}

int CMobileSimulatorApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	delete this->logger;
	return CWinApp::ExitInstance();
}
