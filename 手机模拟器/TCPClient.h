// TCPClient.h: interface for the TCPClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TCPCLIENT_H__0BC202FC_71EA_4C38_8D01_D26A6BCCDF61__INCLUDED_)
#define AFX_TCPCLIENT_H__0BC202FC_71EA_4C38_8D01_D26A6BCCDF61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Exception.h"

class TCPClient  
{
private:
	int timeout;//milin seconds
	int sock;
	unsigned int port;
protected:
	char hostName[1024];

private:
	int waitForPrepared(int method);
	int waitForConnect();
	int waitForWrite();
	int waitForRead();
public:
	void	setTimeout(int timeout);
	int		recv(char *data, int length);
	int		send(const char *data, int length);
	int		send(const void *data, int length);
	void	close();
	int		connect(const char *host, unsigned int port);
	TCPClient();
	virtual ~TCPClient();

};

#ifdef _WIN32
void init_environment();
#endif

#endif // !defined(AFX_TCPCLIENT_H__0BC202FC_71EA_4C38_8D01_D26A6BCCDF61__INCLUDED_)
