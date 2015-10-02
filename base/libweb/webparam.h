/*
 *	SOURCE：	webparam.h
 *	COPYRIGHT：	Tencent
 *	DATE：		2006-10-24
 *	AUTHOR：	chrislin
 *
 *	PURPOSE:	
 *		class for getting url param
 *
 *	DATA OBJ:
 *		1、get data by post method or by get method
 *		2、get data by cookie
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
	 *	purpose:	初始化，解析url参数、cookie
	 */
	webparam();	


	/*
	 *	purpose:	获取所有url参数存放于map上，为名值对
	 */
	const map<string, string>& getparam() const
	{
		return m_Params;
	}

	/*
	 *	purpose:	获取cookie，存放于map上，为名值对
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
	 *	purpose:	获取url环境变量
	 */
	const string& get_cont() const {return m_strContent;}

	/*
	 *	purpose:	获取cookie环境变量
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

