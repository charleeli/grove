/*
 *	SOURCE：		webapp.h
 *	COPYRIGHT：	Tencent
 *	DATE：		2006-10-24
 *	AUTHOR：		chrislin
 *
 *	PURPOSE:
 *		社区cgi
 */

#ifndef _WEB_APP_H_
#define _WEB_APP_H_

/*
 *	our header file
 */
#include "webparam.h"
#include "webpage.h"

/*
 *	system header file
 */
#include <sys/types.h>
#include <sys/shm.h>
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <set>

using namespace std;

/*
 *	class for webapp
 */
class webapp
{
#define WEBOUT (*pout)
public:
	webapp();
	virtual ~webapp()
	{
	}

	/*
	 *	purpose:	全局初始化接口，对象等，只运行一次
	 */
	virtual int global_init() { return 0; }

	/*
	 *	purpose:	合法性验证
	 */
	virtual int check() = 0;

	/*
	 *	purpose:	初始化接口，对象等
	 */
	virtual int init() = 0;

	/*
	 *	purpose:	处理函数
	 */
	virtual int process() = 0;

	/*
	 *	purpose:	运行主体
	 */
	virtual void run();

	/*
	 * purpose: 是否中断fcgi循环
	 */

#ifdef WEBLIB_WITH_FASTCGI
	virtual int quit() = 0;
#endif

	/*
	 *	purpose:	出错处理函数
	 */
	virtual void error() = 0;

	inline void setmaxno(int iMax)
	{
		m_iMaxNo = iMax;
	}

	void SetAutoUpdateName(const char* argv0)
	{
		m_sArgv0 = argv0;
	}

protected:
	webpage m_page;
	map<string, string> m_param;
	map<string, string> m_cookie;
	map<string, string> m_env;

	string m_cookie_content;
	string m_param_content;

	int m_errno;
	string m_errstr;
	ostream* pout;

	int m_iSerialNo;
	int m_iMaxNo;
	bool m_bGlobalInited;

	const char* m_sArgv0;
};
#endif
