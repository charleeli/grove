/*
 *  SOURCE:		general.h
 *  DATE:		2006-6-26
 *  AUTHOR:		chrislin
 *	PURPOSE:	通用函数
 */
#ifndef _GENERAL_H_
#define _GENERAL_H_

#include "webdef.h"

#include <sys/time.h>
#include <assert.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <stdarg.h>
#include <stdio.h>
#include <vector>

using namespace std;

bool isDigitStr(const string str);
/*
 *	purpose:
 */
void timeValAdd(struct timeval *tv_io, long ms);

/*
 *	purpose:	将16进制字符串转换成10进制数
 */
unsigned int hexStrToUint(string sHex);

/*
 *	purpose:	按格式转成字符串，长度1k
 */
string strFormat(const char *sFormat, ...);

/*
 *	purpose:	根据strMark分割字符串strSrc
 *				如果strSrc为空则返回的vector的size为0，如果strMark为空则直接返回strSrc
 */
vector<string> strSplit(string strSrc, string strMark);


string strReplace(string strSrc, string sub, string rpas);
/*
 *	purpose:	睡眠进程nMilliSecond毫秒
 */
void mSleep(unsigned int  nMilliSecond);

/**
 *@brief 将字符串sText中的所有'<',�'>'替换为'&lgt;', '&gt;'
 *
 *@param sText:待替换字符串
 *@param iFlag:0=格式化文本输入出 else="<,>"标签过滤
 *@return 格式化后的字符串
 */
string htmlFilter(string sText, int iFlag = 0);

/**
 *@brief 设置cookie
 *
 *@param name:cookie名字
 *@param value:cookie设置的值
 *@param expires:过期时间
 *@param domain:域名
 *@param secure:域名
 *@return none
 */
void setCookie( string name, string value, 
			string expires, string path , 
			string domain, short secure = 0);

void doRedirect(string sURL);

/**
 *@bridf:产生随机数a, a 在 [0, iMax) 中;
 */
int petRandom(int iMax );

#endif

