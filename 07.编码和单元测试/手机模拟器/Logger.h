// Logger.h: interface for the Logger class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGGER_H__EF529FC7_8B35_456C_B290_93CC7B02E76B__INCLUDED_)
#define AFX_LOGGER_H__EF529FC7_8B35_456C_B290_93CC7B02E76B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
class Logger  
{
private:
	FILE *file;
public:
	void info(const char *file,int line,const char *format,...);
	Logger(const char *fileName);
	virtual ~Logger();

};

#endif // !defined(AFX_LOGGER_H__EF529FC7_8B35_456C_B290_93CC7B02E76B__INCLUDED_)
