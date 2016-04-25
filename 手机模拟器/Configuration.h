// onfiguration.h: interface for the Configuration class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ONFIGURATION_H__C5DA5F95_3FF7_43C5_BE89_34015DF946C3__INCLUDED_)
#define AFX_ONFIGURATION_H__C5DA5F95_3FF7_43C5_BE89_34015DF946C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Configuration  
{
private:
	char buffer[1024*16];
	char *values[128];
	
public:
	char * getValue(const char *name);
	void report();
	int parse();
	int save(const char *fileName);
	int load(const char *fileName);
	Configuration();
	virtual ~Configuration();

};

#endif // !defined(AFX_ONFIGURATION_H__C5DA5F95_3FF7_43C5_BE89_34015DF946C3__INCLUDED_)
