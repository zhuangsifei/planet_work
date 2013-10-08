#pragma once
#include <QtCore/QCoreApplication>
#include <qstring.h>
#include <qstringlist.h>
#include <qsqlerror.h>
#include <QDebug>
#include <QtSql>
#include <QTextCodec>
#include <string>
#include <iostream>
#include <fstream>
#include <Windows.h>
using namespace std;

class QtConnectMysql
{
public:
	QtConnectMysql(void);
	QSqlDatabase getDataDase() const;
	void ShowDriver();
	void TestData();
	bool Connect();

	bool LoadRecordInfile(QSqlQuery query,QString Id1,QString Id2,QString path1,QString path2);
	//void UseTableModify(QSqlDatabase db,QString table,FindKey &FindWords,LPCSTR upath1,LPCSTR spath2);
	bool UpdateRecord(QSqlDatabase db,QString table,map<string,int,bool (*)(string,string)> KindMap);
	~QtConnectMysql(void);
private:
	QString	m_HostName;
	int		m_Port;
	QString	m_DataBase;
	QString	m_UserName;
	QString	m_PassWord;
	QString	m_SQL;
	QSqlDatabase m_db;
};

