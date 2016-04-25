// Exception.h: interface for the Exception class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXCEPTION_H__050BCD55_40C7_464E_8894_DF4EE4C7FE77__INCLUDED_)
#define AFX_EXCEPTION_H__050BCD55_40C7_464E_8894_DF4EE4C7FE77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Exception  
{
protected:
	char *message;
public:
	virtual void report();
	Exception(const char *message);
	virtual ~Exception();

};

#endif // !defined(AFX_EXCEPTION_H__050BCD55_40C7_464E_8894_DF4EE4C7FE77__INCLUDED_)
