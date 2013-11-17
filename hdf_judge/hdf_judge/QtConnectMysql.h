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
using namespace std;

class QtConnectMysql
{
public:
	QtConnectMysql(void);
	QtConnectMysql(QString dataBase);
	QtConnectMysql(QString hostName,QString dataBase,QString userName,QString userPass,int port);
	QSqlDatabase getDataDase() const;
	void ShowDriver();
	void TestData();
	bool Connect();
public:		//old
	bool LoadRecordInfile(QSqlQuery query,QString Id1,QString Id2,QString path1,QString path2);
	bool UpdateRecord(QSqlDatabase db,QString table,map<string,int,bool (*)(string,string)> KindMap);
public:     
	bool LoadRecordInfile(QSqlQuery query,QString pDest,int nNum = 0);
	bool LoadMarkedRecordInfile(QSqlQuery query,QString pDest,int nMarked);
	bool LoadRecordForPredict(QSqlQuery query,QString pDest,int nNum = 0);
public:
	bool LoadFixedRecordInfile(QSqlQuery query,QString pDest,int nNum = 0);
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

