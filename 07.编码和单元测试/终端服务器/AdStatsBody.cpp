// AdStatsBody.cpp: implementation of the AdStatsBody class.
//
//////////////////////////////////////////////////////////////////////


#include "AdStatsBody.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
AdStatsBody::AdStatsBody()
{
	
}

AdStatsBody::~AdStatsBody()
{

}
/**********************************************************************
 函数名称: parse
 函数功能: 把广告的样本数据解析开来
 输出参数:
 返 回 值: 0-不是,-1-输入的内容太大了!
 全局变量:
 编写作者: zhangxilai
 编写日期: 2009年01月23日
 变更日志: 
 相关 SQL：
**********************************************************************/
int AdStatsBody::parse(const char *body)
{
	int length=this->length=strlen(body);
	if(length>1024*16){
		return -1;
	}
	memcpy(this->message,body,length);
	this->message[length]=0;

	char *p=this->message;

	while(*p>0){
		switch(*p){
		case ' ' : *p=0;break;
		case '\r': *p=0;break;
		case '\n': *p=0;break;
		default: break;
		}		
		p++;
	}
	
	this->pos=this->message;

	return 0;
	

}
/**********************************************************************
 函数名称: next
 函数功能: 取得下个值,这个算法的效率并不高,不过可以满足要求
 输出参数:
 返 回 值: NULL,没有了,字符串,当前的值
 全局变量:
 编写作者: zhangxilai
 编写日期: 2009年01月23日
 变更日志: 
 相关 SQL：
**********************************************************************/
char * AdStatsBody::next()
{
	if(this->pos-this->message>this->length){
		return NULL;
	}
	if(this->pos==this->message){
		//第一次
		this->pos=this->message+strlen(this->message)+1;
		return this->message;
	}
	char *p=this->pos;
	while(*p==0){
		p++;
		if(p-this->message>this->length){
			return NULL;
		}
	}
	this->pos=p+strlen(p)+1;
	return p;
}

