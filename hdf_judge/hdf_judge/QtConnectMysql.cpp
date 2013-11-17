#include "QtConnectMysql.h"
#include "AppConfig.h"
#include "Markup.h"
extern AppConfig appConfig;
//---------------------------------------------------------------------------------
// ����: QtConnectMysql::QtConnnectMysql
// ����: ����Ĭ�����ݿ�
//---------------------------------------------------------------------------------
QtConnectMysql::QtConnectMysql(void)
{
	/*m_SQL	   = "QMYSQL";
	m_HostName = "192.168.3.62";
	m_Port	   = 3306;
	m_UserName = "root";
	m_PassWord ="dev";
	m_DataBase ="planet";*/

	/*m_HostName = "localhost";
	m_Port     = 3306;
	m_UserName = "root";
	m_PassWord = "rwxddd";
	m_DataBase = "planet";*/
	m_SQL		= QString::fromStdString( appConfig.GetString(TEXT("m_SQL")) );
	m_HostName  = QString::fromStdString( appConfig.GetString(TEXT("m_HostName")) );
	m_Port		= appConfig.GetInt(TEXT("m_Port") );
	m_UserName	= QString::fromStdString( appConfig.GetString(TEXT("m_UserName")) );
	m_PassWord	= QString::fromStdString( appConfig.GetString(TEXT("m_PassWord")) );
	m_DataBase	= QString::fromStdString( appConfig.GetString(TEXT("m_DataBase")) );

}


//---------------------------------------------------------------------------------
// ����: QtConnectMysql::QtConnnectMysql
// ����: ����ָ�����ݿ�
//---------------------------------------------------------------------------------
QtConnectMysql::QtConnectMysql(QString dataBase)
{
	/*m_SQL	   = "QMYSQL";
	m_HostName = "localhost";
	m_Port     = 3306;
	m_UserName = "root";
	m_PassWord = "rwxddd";*/
	m_SQL		= QString::fromStdString( appConfig.GetString(TEXT("m_SQL")) );
	m_HostName  = QString::fromStdString( appConfig.GetString(TEXT("m_HostName")) );
	m_Port		= appConfig.GetInt(TEXT("m_Port") );
	m_UserName	= QString::fromStdString( appConfig.GetString(TEXT("m_UserName")) );
	m_PassWord	= QString::fromStdString( appConfig.GetString(TEXT("m_PassWord")) );
	m_DataBase  = dataBase;
}

//---------------------------------------------------------------------------------
// ����: QtConnectMysql::QtConnnectMysql
// ����: ����ָ���������˿ڣ��ƶ����ݿ⣬��Ҫ�ʺţ�����
//---------------------------------------------------------------------------------
QtConnectMysql::QtConnectMysql(QString hostName,QString dataBase,QString userName,QString userPass,int nPort)
{
	m_SQL	   = "QMYSQL";
	m_HostName = hostName;
	m_Port     = nPort;
	m_UserName = userName;
	m_PassWord = userPass;
	m_DataBase = dataBase;
}
QSqlDatabase QtConnectMysql::getDataDase() const
{
	return m_db;
}


//---------------------------------------------------------------------------------
// ����: QtConnectMysql::ShowDriver
// ����: ��ʾ���ݿ��е�����dll
//---------------------------------------------------------------------------------
void QtConnectMysql::ShowDriver()
{
    qDebug() << "Available drivers:";  
    QStringList drivers = QSqlDatabase::drivers();  
    foreach(QString driver, drivers)  
		qDebug() << "\t "<< driver;  
    qDebug() << "End";  
}


//---------------------------------------------------------------------------------
// ����: QtConnectMysql::Connect
// ����: �������ݿ�
//---------------------------------------------------------------------------------
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


//---------------------------------------------------------------------------------
// ����: QtConnectMysql::TestData
// ����: �������ݿ��ܷ�������д
//---------------------------------------------------------------------------------
void QtConnectMysql::TestData()
{
	QSqlQuery query("SELECT * FROM hdf_status WHERE topic_id>0");
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
// ����: QtConnectMysql::LoadRecordInfile
// ����: �����ݿ��ȡN����¼д���ı���һ����¼һ���ı�
//---------------------------------------------------------------------------------
bool QtConnectMysql::LoadRecordInfile(QSqlQuery query,QString pDest,int nNum)
{
	QTextCodec *code = QTextCodec::codecForName("UTF-8");
	QString path = "";
	QString Qstr = "";
	int fnameNum = query.record().indexOf("id");
	int fieldNum = query.record().indexOf("content_text");

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

//---------------------------------------------------------------------------------
// ����: QtConnectMysql::LoadMarkedRecordInfile
// ����: �����ݿ��ȡ����ǵļ�¼��ָ�����ļ���
//---------------------------------------------------------------------------------
bool QtConnectMysql::LoadMarkedRecordInfile(QSqlQuery query,QString pDest,int nMarked)
{
	QTextCodec *code = QTextCodec::codecForName("UTF-8");
	QString path = "";
	QString Qstr = "";
	int fnameNum = query.record().indexOf("id");
	int fieldNum = query.record().indexOf("text");
	int kindNum  = query.record().indexOf("kind");

	while( query.next() )
	{
		if (query.value(kindNum).toInt() != nMarked)
			continue;
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

//---------------------------------------------------------------------------------
// ����: QtConnectMysql::LoadRecordInfile
// ����: ���Ѿ��ֿ����ı����ֱ����ص������������ļ�����
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
		else if(nkind !=1 && nkind !=2)
		{
			std::cout<<"The topic_id record in table has changed and you need update "
				<<"the table first\n";
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
bool QtConnectMysql::LoadFixedRecordInfile(QSqlQuery query,QString pDest,int nNum )
{
	QTextCodec *code = QTextCodec::codecForName("UTF-8");
	QString path = "";
	QString Qstr = "";
	int fnameNum = query.record().indexOf("id");
	int fieldNum = query.record().indexOf("content_text");
	int effectNum = query.record().indexOf("effect");
	int mannerNum = query.record().indexOf("manner");

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
		Qstr = query.value(effectNum).toString();
		if (Qstr == "")
			Qstr = "��";
		Qstr = "Ч��Ϊ" + Qstr;
		out<<Qstr<<endl;

		Qstr = query.value(mannerNum).toString();
		if (Qstr == "")
			Qstr = "��";
		Qstr = "��ֹΪ" + Qstr;
		out<<Qstr<<endl;
		file.close();
	}

	return true;
}

//---------------------------------------------------------------------------------
// ����: QtConnectMysql::UpdateRecord
// ����: �����յĽ��д�ڱ�����棬��������Ѿ����µķ���ȡ��
//---------------------------------------------------------------------------------
bool QtConnectMysql::UpdateRecord(QSqlDatabase db,QString table,map<string,int,bool (*)(string,string)> KindMap)
{

	QSqlTableModel qTable(0,db);
	qTable.setTable(table);
	//qTable.setFilter("kind = 0");
	qTable.select();

	int num = qTable.rowCount();
	int val_test;
	map<string,int,bool (*)(string,string)>::iterator iter;
	std::cout<<"All the line in databse is :"<<num<<endl;
	std::cout<<"The size of map is :"<<KindMap.size()<<endl;
	iter = KindMap.begin();
	for(int i=0;i<num;i++)
	{
		QSqlRecord record = qTable.record(i);
		val_test = iter->second;
		iter++;
		record.setValue("kind",val_test);
		qTable.setRecord(i,record);
	}
	if(qTable.submitAll())
	{
		std::cout<<"�޸ĳɹ�"<<endl;
		return true;
	}
	else
	{
		std::cout<<"�޸Ĳ��ɹ�"<<endl;
		return false;
	}
}

//---------------------------------------------------------------------------------
// ����: QtConnectMysql::LoadRecordForPredict
// ����: �����ݿ��ȡN����¼д���ı���һ����¼һ���ı�������Ԥ���ѵ�����ݲ���һ��
//		 �����������Ҫ��д���ؼ�¼����������Ҫ��������������		 
//---------------------------------------------------------------------------------
bool QtConnectMysql::LoadRecordForPredict(QSqlQuery query,QString pDest,int nNum )
{
	QTextCodec *code = QTextCodec::codecForName("UTF-8");
	QString path = "";
	QString Qstr = "";
	int fnameNum = query.record().indexOf("id");
	int fieldNum = query.record().indexOf("text");

	if (nNum == 0)
		nNum = query.size();
	int i=0;
	//while(query.next() && (i++<nNum) )
	while(query.next() && (i++<10000) )
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

//---------------------------------------------------------------------------------
// ����: QtConnectMysql::UpdateRecord(const char *pUpdate,QSqlQuery &query)
// ����: �����õĽ�����ı����ص��½��ı��У�ͬʱ����hdf_status�Լ��½���� 
//---------------------------------------------------------------------------------
bool QtConnectMysql::UpdateRecord(const char *pUpdate,QSqlQuery &query)
{
	query.exec("create table duan.judge ( id bigint(20) unsigned not null,kind smallint(6) not null,primary key(id) )");
		
	/*query.exec("load data local infile 'C:/Users/duan/Desktop/hdf_judge/hdf_judge/zzxxzzmap.txt' replace into table \
			duan.judge fields  terminated by '  ' lines terminated by '\r\n'");*/
	query.exec("load data local infile './zzxxzzmap.txt' replace into table duan.judge fields  terminated \
				by '  ' lines terminated by '\r\n'");

	query.exec("update planet.hdf_status,duan.judge set planet.hdf_status.kind = duan.judge.kind \
				where planet.hdf_status.id = duan.judge.id");

	query.exec("drop table duan.judge"); 
	return true;
}
QtConnectMysql::~QtConnectMysql(void)
{
	m_db.close();
}



