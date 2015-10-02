/*
 *	SOURCE��		webapp.cpp
 *	COPYRIGHT��	Tencent
 *	DATE��		2006-10-24
 *	AUTHOR��		chrislin
 *
 *	PURPOSE:	class for pet community
 */

#include "webapp.h"
#include <sys/stat.h>
#include <unistd.h>

#ifdef WEBLIB_WITH_FASTCGI
FCGX_Stream *fcgi_in, *fcgi_out, *fcgi_err;
FCGX_ParamArray fcgi_env;
#endif

webapp::webapp()
:m_bGlobalInited(false)
,m_sArgv0(NULL)
{
	m_errno = 0;
	m_iSerialNo = 0;
#ifdef WEBLIB_WITH_FASTCGI
	m_iMaxNo = DEFAULT_MAX_SERIAL_NO;
#else
	m_iMaxNo = 0;
#endif
}

/*
 *	PURPOSE: CGIִ�е�������
 */
void webapp::run()
{
	int ret = 0;

#ifdef WEBLIB_WITH_FASTCGI
	//ȡ�ö������ļ�������޸�ʱ��
	time_t tStart = 0;
	struct stat stStatTmp;
	if(m_sArgv0 != NULL)
	{
		if(stat(m_sArgv0, &stStatTmp) == 0)
		{
			tStart = stStatTmp.st_mtime;
		}
	}

	while (FCGX_Accept(&fcgi_in, &fcgi_out, &fcgi_err, &fcgi_env) >= 0)
	{
		fcgi_ostream oStream(fcgi_out);
		pout = &oStream;
#else
		pout = &cout;
#endif

	{
        webparam param;
        m_param = param.getparam();
        m_cookie = param.getcookie();
        m_env = param.getenv();

        m_param_content = param.get_cont();
        m_cookie_content = param.get_cookie();
	}

	do
	{
		//ȫ�ֳ�ʼ��������ֻ����һ��
		if(!m_bGlobalInited)
		{
			ret = global_init();
			if(0 != ret) break;
			m_bGlobalInited = true;
		}

		ret = check();
		if (0 != ret) break;

		ret = init();
		if (0 != ret) break;

		ret = process();
	}while(false);

	if (ret != 0)
	{
		m_errno = ret;
		error();
	}

#ifdef WEBLIB_WITH_FASTCGI
	++m_iSerialNo;

	//���Ҫ��;�˳����˳�
	//����Ѿ�ִ��m_iMaxNo�Σ�����
	//����������ļ��Ѿ����£�����
	if((0 != quit()) ||
		(m_iMaxNo != NO_LIMIT_SERIAL_NO && m_iSerialNo >= m_iMaxNo) ||
		(tStart != 0 && (stat(m_sArgv0, &stStatTmp) == 0) && tStart != stStatTmp.st_mtime) )
	{
		FCGX_Finish();
		return;
	}
}
#endif

}

