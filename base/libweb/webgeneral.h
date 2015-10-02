/*
 *  SOURCE:		general.h
 *  DATE:		2006-6-26
 *  AUTHOR:		chrislin
 *	PURPOSE:	ͨ�ú���
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
 *	purpose:	��16�����ַ���ת����10������
 */
unsigned int hexStrToUint(string sHex);

/*
 *	purpose:	����ʽת���ַ���������1k
 */
string strFormat(const char *sFormat, ...);

/*
 *	purpose:	����strMark�ָ��ַ���strSrc
 *				���strSrcΪ���򷵻ص�vector��sizeΪ0�����strMarkΪ����ֱ�ӷ���strSrc
 */
vector<string> strSplit(string strSrc, string strMark);


string strReplace(string strSrc, string sub, string rpas);
/*
 *	purpose:	˯�߽���nMilliSecond����
 */
void mSleep(unsigned int  nMilliSecond);

/**
 *@brief ���ַ���sText�е�����'<',�1�7'>'�滻Ϊ'&lgt;', '&gt;'
 *
 *@param sText:���滻�ַ���
 *@param iFlag:0=��ʽ���ı������ else="<,>"��ǩ����
 *@return ��ʽ������ַ���
 */
string htmlFilter(string sText, int iFlag = 0);

/**
 *@brief ����cookie
 *
 *@param name:cookie����
 *@param value:cookie���õ�ֵ
 *@param expires:����ʱ��
 *@param domain:����
 *@param secure:����
 *@return none
 */
void setCookie( string name, string value, 
			string expires, string path , 
			string domain, short secure = 0);

void doRedirect(string sURL);

/**
 *@bridf:���������a, a �� [0, iMax) ��;
 */
int petRandom(int iMax );

#endif

