#include "QtConnectMysql.h"


//---------------------------------------------------------------------------------
// ����: QtConnectMysql::QtConnnectMysql
// ����: ����Ĭ�����ݿ�
//---------------------------------------------------------------------------------
QtConnectMysql::QtConnectMysql(void)
{
	m_SQL	   = "QMYSQL";
	/*m_HostName = "192.168.3.62";
	m_Port	   = 3306;
	m_UserName = "root";
	m_PassWord ="dev";
	m_DataBase ="planet";*/
	m_HostName = "localhost";
	m_Port     = 3306;
	m_UserName = "root";
	m_PassWord = "rwxddd";
	m_DataBase = "planet";
}


//---------------------------------------------------------------------------------
// ����: QtConnectMysql::QtConnnectMysql
// ����: ����ָ�����ݿ�
//---------------------------------------------------------------------------------
QtConnectMysql::QtConnectMysql(QString dataBase)
{
	m_SQL	   = "QMYSQL";
	m_HostName = "localhost";
	m_Port     = 3306;
	m_UserName = "root";
	m_PassWord = "rwxddd";
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
//this function load all the texts in one folder,every record use one .txt file
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

//void QtConnectMysql::UseTableModify(QSqlDatabase db,QString table,FindKey &FindWords,LPCSTR upath1,LPCSTR spath2)
//{
//	QTextCodec *code = QTextCodec::codecForName("utf8");
//	QString Qstr = " ";
//	QString upath = upath1;
//	QSqlTableModel qTable(0,db);
//	qTable.setTable(table);
//	qTable.setFilter("topic_id>0");
//	qTable.select();
//	int num = qTable.rowCount();
//	for(int i=0;i<num;i++)
//	{
//		QSqlRecord record = qTable.record(i);
//		//QFile sfile(upath1);
//		QFile sfile(upath);
//		if(!sfile.open(QIODevice::WriteOnly |QIODevice::Text))
//		{
//			qDebug()<<"Open file failed\n";
//			return ;
//		}
//		QTextStream sout(&sfile);
//		sout.setCodec(code);
//		Qstr = record.value("text").toString();
//		sout<<Qstr<<endl;
//		FindWords.SingleSeparateWords(upath1,spath2);
//		map<string,int> MapStr;
//		int nResult;
//		FindWords.ClearWords(spath2,MapStr);
//		//this is the first method to get the result
//		/*nResult = FindWords.SingleResult1(MapStr);*/
//
//		//this is the second method to get the result
//		nResult = FindWords.SingleResult2(MapStr);
//
//		record.setValue("kind",nResult);
//		qTable.setRecord(i,record);
//		sfile.close();
//	}
//	if(qTable.submitAll())
//	{
//		std::cout<<"�޸ĳɹ�"<<endl;
//		//return true;
//	}
//	else
//	{
//		std::cout<<"�޸Ĳ��ɹ�"<<endl;
//		//return false;
//	}
//}

//---------------------------------------------------------------------------------
// ����: QtConnectMysql::UpdateRecord
// ����: �����յĽ��д�ڱ�����棬��������ݱ����죬���Դ˺���Ҫ����������
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
QtConnectMysql::~QtConnectMysql(void)
{
	m_db.close();
}


