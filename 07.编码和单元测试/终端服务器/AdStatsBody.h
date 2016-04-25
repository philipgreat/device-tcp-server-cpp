// AdStatsBody.h: interface for the AdStatsBody class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADSTATSBODY_H__FEEC8FFB_D257_4D2A_BD91_2F903A80B4D3__INCLUDED_)
#define AFX_ADSTATSBODY_H__FEEC8FFB_D257_4D2A_BD91_2F903A80B4D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AdStatsBody  
{
private:
	char *pos;
	int length;
	char message[1024*16];
public:
	char * next();
	int parse(const char *body);
	AdStatsBody();
	virtual ~AdStatsBody();
};

#endif // !defined(AFX_ADSTATSBODY_H__FEEC8FFB_D257_4D2A_BD91_2F903A80B4D3__INCLUDED_)
