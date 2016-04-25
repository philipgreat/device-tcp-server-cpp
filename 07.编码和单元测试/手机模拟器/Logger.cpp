// Logger.cpp: implementation of the Logger class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MobileSimulator.h"
#include "Logger.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Logger::Logger(const char *fileName)
{
	file=fopen(fileName,"a");
	
	if(file!=NULL){
		return;
	}
	char message[1024];
	sprintf(message,"日志文件'%s'打开错误,一般是权限不够或者磁盘满等致命错误!",fileName);
#ifdef _WIN32
	::MessageBox(NULL,message,"日志文件打开出错",MB_OK|MB_ICONERROR);
#endif
	exit(0);

}

Logger::~Logger()
{
	fclose(this->file);
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
void Logger::info(const char *file,int line,const char *format,...)
{
	char message[2048];

	fprintf(this->file,"%s ",getDateExpr());
	fprintf(this->file,"%s:%4d: ",file,line);
	va_list args;
    va_start(args, format);
    vsprintf(message,format,args);
    va_end(args);	
	fprintf(this->file,message);
	fprintf(this->file,"\n");
	::fflush(this->file);

}
