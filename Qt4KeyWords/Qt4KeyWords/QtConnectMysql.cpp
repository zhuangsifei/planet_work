#include "QtConnectMysql.h"
#include "AppConfig.h"
QtConnectMysql::QtConnectMysql(void)
{

	m_SQL		= QString::fromStdString( appConfig.GetString(TEXT("m_SQL")) );
	m_HostName  = QString::fromStdString( appConfig.GetString(TEXT("m_HostName")) );
	m_Port		= appConfig.GetInt(TEXT("m_Port") );
	m_UserName	= QString::fromStdString( appConfig.GetString(TEXT("m_UserName")) );
	m_PassWord	= QString::fromStdString( appConfig.GetString(TEXT("m_PassWord")) );
	m_DataBase	= QString::fromStdString( appConfig.GetString(TEXT("m_DataBase")) );

}

QSqlDatabase QtConnectMysql::getDataDase() const
{
	return m_db;
}

void QtConnectMysql::ShowDriver()
{
    qDebug() << "Available drivers:";  
    QStringList drivers = QSqlDatabase::drivers();  
    foreach(QString driver, drivers)  
		qDebug() << "\t "<< driver;  
    qDebug() << "End";  
}

bool QtConnectMysql::Connect()
{
	m_db = QSqlDatabase::addDatabase(m_SQL);
	m_db.setHostName(m_HostName);
	m_db.setPort(m_Port);
	m_db.setDatabaseName(m_DataBase);
	m_db.setUserName(m_UserName);
	m_db.setPassword(m_PassWord);
	if(m_db.open())
	{
		qDebug()<<"open the mysql succefully!"<<m_db.lastError().driverText()<<endl;
		return true;
	}
	else
	{
		qDebug()<<"open the mysql failed!\n";
		return false;
	}

}

void QtConnectMysql::TestData()
{
	QSqlQuery query("SELECT * FROM wb_status WHERE topic_id>0");
	int fieldNum = query.record().indexOf("id");
	int cnt = 0;
	while(query.next())
	{
		QString Qstr = query.value(0).toString();
		qDebug()<<Qstr<<endl;
		cnt ++;
	}
	std::cout<<cnt;
}

//---------------------------------------------------------------------------------
// 名字: QtConnectMysql::LoadRecordInfile
// 功能: 将分好类的两种文本分别下载到zkind1，zkind2，两个文件夹中
//---------------------------------------------------------------------------------
bool QtConnectMysql::LoadRecordInfile(QSqlQuery query,QString Id1,QString Id2,QString path1,QString path2)
{
	int fieldNum = query.record().indexOf(Id1); //this is the text
	int fnameNum = query.record().indexOf(Id2); //this is the key word
	QString path =" ";
	QString Qstr;
	QTextCodec *code = QTextCodec::codecForName("utf8"); //set the code format in UTF-8
	
	while(query.next())
	{
		Qstr = query.value(fieldNum).toString();
		path = query.value(fnameNum).toString();
		path += ".txt";

		int nkind = query.record().value("kind").toInt();
		if(nkind == 1)
			path = path1 + "\\" + path;
		if(nkind == 2)
			path = path2 + "\\" + path;
		else if(nkind !=1 && nkind!=2 )
		{
			std::cout<<"The dataBase has been updated and "<<endl
				<<"You'd better to open the BaseMode and update\n";
			return false;
		}

		QFile file(path);
		if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			qDebug()<<"open file failed\n";
			return false;
		}
		QTextStream out(&file);
		out.setCodec(code);		//set code format
		out<<Qstr<<endl;
		file.close();
	}
	return true;
}
// this function may have some problem and is still in test now
bool QtConnectMysql::UseQueryModify(QSqlQuery query, FindKey &FindWords,LPCSTR upath1,LPCSTR spath2)
{
	int fieldNum = query.record().indexOf("text");
	int fnameNum = query.record().indexOf("id");
	QString Qstr;
	QTextCodec *code = QTextCodec::codecForName("utf8"); 
	
	query.seek(0);
	int nNum = query.size();
	for(int i=0;i<nNum;i++)
	{
		//bool bs = query.seek(i);
		bool bse = query.isSelect();
		fieldNum = query.record().indexOf("text");
		QFile sfile(upath1);
		if(!sfile.open(QIODevice::WriteOnly |QIODevice::Text))
		{
			qDebug()<<"Open file failed\n";
			return false;
		}
		QTextStream sout(&sfile);
		sout.setCodec(code);
		Qstr = query.value(fieldNum).toString();
		sout<<Qstr<<endl;
		FindWords.SingleSeparateWords(upath1,spath2);
		map<string,int> MapStr;
		int nResult = 0;
		FindWords.ClearWords(spath2,MapStr);
		nResult = FindWords.SingleResult2(MapStr);
		//int nKind = query.record().indexOf("kind");
		//query.record().setValue(nKind , nResult);
		int nID = query.record().indexOf("id");
		QString str1;
		QString str2;
		QString str3;
		str1.setNum(nID);
		str2.setNum(nResult);
		str3 = "UPDATE wb_status SET kind =  ";
		str3 = str3 + str2 + " WHERE id = ";
		str3 = str3 + str1;

		query.setForwardOnly(true);
		query.prepare(str3);
		bool bt = query.exec();
		//这个是浪费语句，必须这样才能保证isSelect为true，进而能够调用
		bool btest = query.exec("SELECT * FROM wb_status");
		bool bs = query.seek(i);
		if(bt)
		{
			std::cout<<"Updata database successfully\n";
		}
		else
		{
			std::cout<<"Updata database unsucessfully\n";
		}
		bool ba = query.isActive();
		bool bse1 = query.isSelect();
		std::cout<<nResult<<endl;
	}
	return true;
}
// use this table to predict the result of record whose record's kind=0
void QtConnectMysql::UseTableModify(QSqlDatabase db,QString table,FindKey &FindWords,LPCSTR upath1,LPCSTR spath2)
{
	QTextCodec *code = QTextCodec::codecForName("utf8");
	QString Qstr = " ";
	QString upath = upath1;
	QSqlTableModel qTable(0,db);
	qTable.setTable(table);
	qTable.setFilter("topic_id>0 AND kind=0");
	qTable.select();
	int num = qTable.rowCount();
	for(int i=0;i<num;i++)
	{
		QSqlRecord record = qTable.record(i);
		//QFile sfile(upath1);
		QFile sfile(upath);
		if(!sfile.open(QIODevice::WriteOnly |QIODevice::Text))
		{
			qDebug()<<"Open file failed\n";
			return ;
		}
		QTextStream sout(&sfile);
		sout.setCodec(code);
		Qstr = record.value("text").toString();
		sout<<Qstr<<endl;
		FindWords.SingleSeparateWords(upath1,spath2);
		map<string,int> MapStr;
		int nResult;
		FindWords.ClearWords(spath2,MapStr);
		//this is the first method to get the result
		/*nResult = FindWords.SingleResult1(MapStr);*/

		//this is the second method to get the result
		nResult = FindWords.SingleResult2(MapStr);

		record.setValue("kind",nResult);
		qTable.setRecord(i,record);
	}
	if(qTable.submitAll())
	{
		std::cout<<"修改成功"<<endl;
	}
	else
	{
		std::cout<<"修改不成功"<<endl;
		qDebug()<<qTable.lastError()<<endl;
	}
}

//---------------------------------------------------------------------------------
// 名字: QtConnectMysql::LoadRecordInfile
// 功能: 从数据库读取N条记录写进文本，一个记录一个文本
//---------------------------------------------------------------------------------
bool QtConnectMysql::LoadRecordInfile(QSqlQuery query,QString pDest,int nNum)
{
	QTextCodec *code = QTextCodec::codecForName("UTF-8");
	QString path = "";
	QString Qstr = "";
	int fnameNum = query.record().indexOf("id");
	int fieldNum = query.record().indexOf("text");

	if (nNum == 0)
		nNum = query.size();
	int i=0;
	while(query.next() && (i++<nNum) )
	{
		path = query.value(fnameNum).toString();
		Qstr = query.value(fieldNum).toString();
		path += ".txt";
		path = pDest + "\\" + path;

		QFile file(path);
		if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			qDebug()<<"open file failed\n";
			return false;
		}
		QTextStream out(&file);
		out.setCodec(code);		//set code format
		out<<Qstr<<endl;
		file.close();
	}

	return true;
}

bool QtConnectMysql::UpdateRecord(const char *pUpdate,QSqlQuery &query)
{
	query.exec("create table duan.wb ( id bigint(20) unsigned not null,kind smallint(6) not null,primary key(id) )");
	/*query.exec("load data local infile 'E:/TRD/TRD_work/product/Planet/src/tools/mining/wb.Mining/src/Qt4KeyWords/Qt4KeyWords/result.txt'\
			   replace into table duan.wb fields  terminated by '  ' lines terminated by '\r\n'");*/

	query.exec("load data local infile './result.txt' replace into table duan.wb fields  \
			   terminated by '  ' lines terminated by '\r\n'");

	query.exec("update planet.wb_status,duan.wb set planet.wb_status.kind = duan.wb.kind \
			    where planet.wb_status.id = duan.wb.id");
	query.exec("drop table duan.wb");
	return true;
}
QtConnectMysql::~QtConnectMysql(void)
{
	m_db.close();
}


