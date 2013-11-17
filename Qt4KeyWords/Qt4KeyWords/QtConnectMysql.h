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
#include "FindKey.h"
class QtConnectMysql
{
public:
	QtConnectMysql(void);
	QSqlDatabase getDataDase() const;
	void ShowDriver();
	void TestData();
	bool Connect();
public:
	bool LoadRecordInfile(QSqlQuery query,QString Id1,QString Id2,QString path1,QString path2);
	bool UseQueryModify(QSqlQuery query,FindKey &FindWords,LPCSTR upath1,LPCSTR spath2 );
	void UseTableModify(QSqlDatabase db,QString table,FindKey &FindWords,LPCSTR upath1,LPCSTR spath2);
public:
	bool LoadRecordInfile(QSqlQuery query,QString pDest,int nNum = 0);
	bool UpdateRecord(const char *pUpdate,QSqlQuery &query);
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

