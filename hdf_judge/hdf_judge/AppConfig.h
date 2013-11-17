#pragma once
#include <iostream>
#include <string>
#include <map>
using namespace std;
class AppConfig
{
public:
	AppConfig(void);
	~AppConfig(void);
public:
	int GetInt(string key);
	string GetString(string key);
private:
	map<string,string> m_configMap;
};

extern AppConfig appConfig;
