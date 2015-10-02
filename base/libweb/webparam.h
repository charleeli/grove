/*
 *	SOURCE��	webparam.h
 *	COPYRIGHT��	Tencent
 *	DATE��		2006-10-24
 *	AUTHOR��	chrislin
 *
 *	PURPOSE:	
 *		class for getting url param
 *
 *	DATA OBJ:
 *		1��get data by post method or by get method
 *		2��get data by cookie
 *
 *	RESULT:
 *		save to map<string, string>
 */

#ifndef _WEB_PARAM_H_
#define _WEB_PARAM_H_

#include <string>
#include <map>
#include <iostream>
#include "webdef.h"

using namespace std;

class webparam
{
public:
	
	/*
	 *	purpose:	��ʼ��������url������cookie
	 */
	webparam();	


	/*
	 *	purpose:	��ȡ����url���������map�ϣ�Ϊ��ֵ��
	 */
	const map<string, string>& getparam() const
	{
		return m_Params;
	}

	/*
	 *	purpose:	��ȡcookie�������map�ϣ�Ϊ��ֵ��
	 */
	const map<string, string>& getcookie() const
	{
		return m_cookie;
	}

	const map<string, string>& getenv() const
	{
		return m_env;
	}

	/*
	 *	purpose:	��ȡurl��������
	 */
	const string& get_cont() const {return m_strContent;}

	/*
	 *	purpose:	��ȡcookie��������
	 */
	const string& get_cookie() const {return m_strCookies;}

	std::string& ParamFilter(const char * context, std::string & ret_value, bool is_html = true);


	
protected:
	inline string GetRequestMethod();
	
	void GetCgiValue();
	
	void ParseParams();

	void ParseCookies();

	void ParseEnv();

	string UrlDecode(const string strSrc);

	int HexToInt(char ch);

protected:
	string 					m_strContent;
	string 					m_strCookies;
	map<string, string> 	m_Params;
	map<string, string>		m_cookie;
	map<string, string>	    m_env;
};


#endif

