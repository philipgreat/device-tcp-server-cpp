// MobileSimulatorDlg.h : header file
//

#if !defined(AFX_MOBILESIMULATORDLG_H__F341CE80_7A89_4115_8523_C76490B324A5__INCLUDED_)
#define AFX_MOBILESIMULATORDLG_H__F341CE80_7A89_4115_8523_C76490B324A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMobileSimulatorDlg dialog
#include "TCPClient.h"
class CMobileSimulatorDlg : public CDialog
{
// Construction
private:
	CMobileSimulatorApp *app;
public:
	CMobileSimulatorApp* getApp();
	void connectServer(TCPClient *client);
	void log(const char *position,int line,const char *format,...);
	Logger * getLogger();
	CMobileSimulatorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMobileSimulatorDlg)
	enum { IDD = IDD_MOBILESIMULATOR_DIALOG };
	CString	m_terminalId;
	CString	m_params;
	CString	m_blacklist;
	CString	m_whitelist;
	CString	m_logText;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMobileSimulatorDlg)
	public:
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMobileSimulatorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonHire();
	afx_msg void OnButtonFire();
	afx_msg void OnDestroy();
	afx_msg void OnButtonClearLog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOBILESIMULATORDLG_H__F341CE80_7A89_4115_8523_C76490B324A5__INCLUDED_)
