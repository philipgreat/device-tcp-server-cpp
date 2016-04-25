// MobileSimulatorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MobileSimulator.h"
#include "MobileSimulatorDlg.h"
#include "WtdpPacket.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	
	// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//{{AFX_MSG_MAP(CAboutDlg)
// No message handlers
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMobileSimulatorDlg dialog

CMobileSimulatorDlg::CMobileSimulatorDlg(CWnd* pParent /*=NULL*/)
: CDialog(CMobileSimulatorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMobileSimulatorDlg)
	m_terminalId = _T("");
	m_params = _T("");
	m_blacklist = _T("");
	m_whitelist = _T("");
	m_logText = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMobileSimulatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMobileSimulatorDlg)
	DDX_Text(pDX, IDC_TERMINAL_ID, m_terminalId);
	DDX_Text(pDX, IDC_PARAMS, m_params);
	DDX_Text(pDX, IDC_BLACKLIST, m_blacklist);
	DDX_Text(pDX, IDC_WHITELIST, m_whitelist);
	DDX_Text(pDX, IDC_EDIT_LOG, m_logText);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMobileSimulatorDlg, CDialog)
//{{AFX_MSG_MAP(CMobileSimulatorDlg)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDC_BUTTON_HIRE, OnButtonHire)
ON_BN_CLICKED(IDC_BUTTON_FIRE, OnButtonFire)
ON_WM_DESTROY()
ON_BN_CLICKED(IDC_BUTTON_CLEAR_LOG, OnButtonClearLog)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMobileSimulatorDlg message handlers

BOOL CMobileSimulatorDlg::OnInitDialog()
{
	init_environment();
	CDialog::OnInitDialog();
	
	// Add "About..." menu item to system menu.
	
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	TRACE("Executable filename = %s\n", AfxGetApp()->m_pszExeName);



	char title[1024];
	sprintf(title,"商旅无忧终端模拟器-服务器 %s:%d",app->getServerIp(),app->getServerPort());

	this->SetWindowText(title);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMobileSimulatorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMobileSimulatorDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
		
		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		
		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
	
	
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMobileSimulatorDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

int CMobileSimulatorDlg::DoModal() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	//this->UpdateData(TRUE);
	app=((CMobileSimulatorApp*)::AfxGetApp());
	
	this->m_terminalId=app->getTerminalId();
	this->m_params=app->getParameters();
	//this->m_terminalId="sdf";
	
	//this->UpdateData(FALSE);
	getLogger()->info("simulator",__LINE__,"started ");
	getLogger()->info("simulator",__LINE__,"tmid       =%s",this->m_terminalId);	
	getLogger()->info("simulator",__LINE__,"serverip   =%s",app->getServerIp());	
	getLogger()->info("simulator",__LINE__,"serverport =%d",app->getServerPort());
	getLogger()->info("simulator",__LINE__,"params     ='%s'",app->getParameters());
	
	return CDialog::DoModal();
}

#include "wtdp.h"
static int processServerComands(CMobileSimulatorDlg *context,WtdpPacket *p,TCPClient *client)
{

	context->getLogger()->info("simulator",__LINE__,"断开处理");
	p->packDisconnectingReq(context->getApp()->getTerminalId());	
	client->send(p->getHead(),p->getLength());
	while(true){
		

		client->recv(p->getHead(),12);
		int expectlen=p->getLength()-12;
		client->recv((char*)p->getBody(),expectlen);
		context->getLogger()->info("simulator",__LINE__,"收到服务器传递过来的命令: %04x",p->getCommand());
		if(p->getCommand()==0x017){
			//断开确认,关闭就可以了,不用回包
			context->getLogger()->info("simulator",__LINE__,"收到断开确认");
			client->close();	
			return 0;
		}
		
		//不是断开确认
		if(p->getCommand()==0x00000011){
			//修改系统参数
			
			ParameterModificationRequest* req=(ParameterModificationRequest*)p->getHead();
			context->log("simulator",__LINE__,"修改系统参数: %s",req->strParmValues);
			context->getLogger()->info("simulator",__LINE__,"修改系统参数: %s",req->strParmValues);
			context->UpdateData(TRUE);
			context->m_params=req->strParmValues;
			context->UpdateData(FALSE);
			//发送确认,准备收另外一个,好烦哦
			p->packParameterModificationResp(context->getApp()->getTerminalId(),0);
			client->send(p->getHead(),p->getLength());
			continue;
		}
		
		return 0;
	}
	context->getLogger()->info("simulator",__LINE__,"断开处理");
	return 0;

}
void CMobileSimulatorDlg::OnButtonHire() 
{
	// TODO: Add your control notification handler code here
	TCPClient client;
	try{
		this->connectServer(&client);
		WtdpPacket packet;
		//发送一个开通包看看
		
		WtdpPacket * p;
		p=packet.packServiceStartReq(app->getTerminalId());
		
		client.send(p->getHead(),p->getLength());
		getLogger()->info("simulator",__LINE__,"发送开通请求");
		log("simulator",__LINE__,"发送开通请求");;
		client.recv(p->getHead(),12);
		client.recv((char*)p->getBody(),p->getBodyLength());
		
		log("simulator",__LINE__,"收到开通回复 命令: 0x%03x 代码: 0x%04x %s",p->getCommand(),p->getErrorCode(),p->getErorInfo());;
		getLogger()->info("simulator",__LINE__,"收到开通回复 命令: 0x%03x 代码: 0x%04x %s",p->getCommand(),p->getErrorCode(),p->getErorInfo());;
		
		processServerComands(this,&packet,&client);

		
	}catch(Exception &e){
		::MessageBox(NULL,e.getMessage(),"错误,服务器可能没有启动",MB_OK|MB_ICONERROR);
		getLogger()->info("simulator",__LINE__,"网络错误: %s",e.getMessage());
	}
	
}

void CMobileSimulatorDlg::OnButtonFire() 
{
	WtdpPacket packet;
	WtdpPacket * p;
	TCPClient client;
	// TODO: Add your control notification handler code here
	try{	
		this->connectServer(&client);
		p=packet.packServiceEndReq(app->getTerminalId(),10);	
		client.send(p->getHead(),p->getLength());
		getLogger()->info("simulator",__LINE__,"发送退租请求");
		log("simulator",__LINE__,"发送退租请求");
		client.recv(p->getHead(),12);
		client.recv((char*)p->getBody(),p->getBodyLength());
		log("simulator",__LINE__,"收到退租回复 命令: 0x%03x 代码: 0x%04x %s",p->getCommand(),p->getErrorCode(),p->getErorInfo());;
		getLogger()->info("simulator",__LINE__,"收到退租回复 命令: 0x%03x 代码: 0x%04x %s",p->getCommand(),p->getErrorCode(),p->getErorInfo());
		processServerComands(this,&packet,&client);
	}catch(Exception &e){
		::MessageBox(NULL,e.getMessage(),"错误,服务器可能没有启动",MB_OK|MB_ICONERROR);
		getLogger()->info("simulator",__LINE__,"网络错误: %s",e.getMessage());
	}
	
}

void CMobileSimulatorDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	getLogger()->info("simulator",__LINE__,"end tmid=%s params='%s'",this->m_terminalId,this->m_params);
	
	// TODO: Add your message handler code here
}

Logger * CMobileSimulatorDlg::getLogger()
{
	return this->app->getLogger();
}
static char *getDateExpr()
{
	static char expr[14+1];
	time_t now;
	struct tm *p;
	time(&now);
	p=localtime(&now);
	sprintf(expr,"%02u%02u %02u:%02u:%02u",p->tm_mon+1,p->tm_mday,
		p->tm_hour,p->tm_min,p->tm_sec);
	return expr;
}
static char *getShortExpr()
{
	static char expr[14+1];
	time_t now;
	struct tm *p;
	time(&now);
	p=localtime(&now);
	sprintf(expr,"%02u:%02u:%02u",	p->tm_hour,p->tm_min,p->tm_sec);
	return expr;
}
void CMobileSimulatorDlg::log(const char *position, int line, const char *format, ...)
{
	char message[2048];
	char loggerText[2048];
	
	va_list args;
    va_start(args, format);
    vsprintf(message,format,args);
    va_end(args);	
	sprintf(loggerText,"%s %s:%4d %s\r\n",getShortExpr(),position,line,message);
	
	this->UpdateData(TRUE);
	this->m_logText=loggerText+this->m_logText;
	this->UpdateData(FALSE);
	
	
}

void CMobileSimulatorDlg::OnButtonClearLog() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData(TRUE);
	this->m_logText="";
	this->UpdateData(FALSE);
}


void CMobileSimulatorDlg::connectServer(TCPClient *client)
{
	if(client->connect(app->getServerIp(),app->getServerPort())<0){		
		return ;
	}
}

CMobileSimulatorApp* CMobileSimulatorDlg::getApp()
{
	return this->app;
}
