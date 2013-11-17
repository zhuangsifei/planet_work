#include "AppConfig.h"
#include "Markup.h"


AppConfig::AppConfig(void)
{
	CMarkup markUp;
	if ( !markUp.Load( TEXT(".\\app.config") ) )
	{
		return;
	}
	if ( markUp.FindChildElem( TEXT("connectMySQL") ) )
	{
		markUp.IntoElem();
		while(markUp.FindChildElem( TEXT("add") ) )
		{
			string key = markUp.GetChildAttrib( TEXT("key") );
			string value = markUp.GetChildAttrib( TEXT("value") );
			m_configMap.insert(pair<string,string>(key,value));
		}
		markUp.OutOfElem();
	}
}


AppConfig::~AppConfig(void)
{
}

int AppConfig::GetInt(string key)
{
	if (m_configMap.find(key) != m_configMap.end())
	{
		return atoi( m_configMap[key].c_str() );
	}
	else return 0;
}

string AppConfig::GetString(string key)
{
	if(m_configMap.find(key) != m_configMap.end())
	{
		return m_configMap[key];
	}
	else
		return "";
}
