#include <QtCore/QCoreApplication>
#include "CalculateNum.h"
#include "Characters.h"
#include "TraverseFolder.h"
#include "SeparateTerm.h"
#include "QtConnectMysql.h"
#include "FindBad.h"
#include "AppConfig.h"
using namespace std;


extern const char *g_pAllWords  = ".\\zAllWords.txt";
extern const char *g_pThanks	= ".\\zThanks.txt";
extern const char *g_pYideGood  = ".\\zYideGood.txt";
extern const char *g_pYideBad	= ".\\zYideBad.txt";
extern const char *g_pYishuGood = ".\\zYishuGood.txt";
extern const char *g_pYishuBad  = ".\\zYishuBad.txt";

extern const char *g_pAllPath   = ".\\zAll";
extern const char *g_pBadPath	= ".\\zBad";
extern const char *g_pGoodPath	= ".\\zGood";
extern const char *g_psBadPath  = ".\\zsBad";
extern const char *g_psGoodPath = ".\\zsGood";

extern const char *g_pCrudeDic	= ".\\zCrudeDic.txt";
extern const char *g_pAmendDic	= ".\\zAmendDic.txt";
extern const char *g_pShortDic  = ".\\zShortDic.txt";
extern const char *g_pTrain		= ".\\zTrain.txt";

extern const char *g_pPreTest	= ".\\zPreTest";	//Ԥ����������ļ���
extern const char *g_pSepTest	= ".\\zSepTest";	//Ԥ�������ִ��ļ���
extern const char *g_pTest		= ".\\zTest.txt";	//Ԥ��������������
extern const char *g_pGoal		= ".\\goal.txt";	//Ԥ����
extern const char *g_pName		= ".\\nameID.txt";	//����keyID����ı���
extern const char *g_pUpdate	= ".\\zzxxzzmap.txt";	//������¼�¼���ļ�

ofstream g_ftest(".\\g_test.txt");
extern const int g_FiltWords = 10;	 //���˵�Ƶ�ʻ�
extern const int g_LeftWords = 2000; //����������Ŀ
AppConfig appConfig;				 //���������ļ�
int main(int argc, char *argv[])
{
	// �����ַ����ı��뷽ʽ��������ת��ʱҲ֪����ʲô��ת
	QTextCodec *code = QTextCodec::codecForName("GBK");
	QTextCodec::setCodecForCStrings(code);
	enum mode{Train,Predict};
	//mode dxMode = Train;
	mode dxMode = Predict;

	if ( dxMode == Train)
	{
		//QtConnectMysql Qtcon("192.168.3.62","planet","root","dev",3306);
		QtConnectMysql Qtcon;// ѡ�������ļ��е����ݿ�
		Qtcon.ShowDriver();
		if ( !Qtcon.Connect() )
			return 1;

		//�����������ļ���
		TraverseFolder Travel;
		Travel.CreateFolder(g_pBadPath);
		Travel.CreateFolder(g_pGoodPath);
		Travel.CreateFolder(g_psBadPath);
		Travel.CreateFolder(g_psGoodPath);

		Travel.ClearFolder(g_pBadPath);
		Travel.ClearFolder(g_pGoodPath);
		Travel.ClearFolder(g_psBadPath);
		Travel.ClearFolder(g_psGoodPath);


		//��ȡ�û���������
		QString QSelect("SELECT * FROM hdf_status WHERE effect = '������' AND manner = '������'");
		QSqlQuery QueryGood(QSelect); 
		Qtcon.LoadRecordInfile(QueryGood,g_pGoodPath,5000);
		//Qtcon.LoadFixedRecordInfile(QueryGood,g_pGoodPath,5000);

		QSelect = "SELECT * FROM hdf_status WHERE effect IN ( '������' , '����֪��') AND  manner = '������'";
		QSqlQuery QueryBad(QSelect);
		Qtcon.LoadRecordInfile(QueryBad,g_pBadPath,5000);
		//Qtcon.LoadFixedRecordInfile(QueryBad,g_pBadPath,5000);

		//���������зִ�,ҽѧ����л�ִʶ���Ҫ���
		SeparateTerm Sepa(g_pAllWords,true);
		Sepa.SepInit();
		Sepa.BatchSeparate(g_pBadPath,g_psBadPath);
		Sepa.BatchSeparate(g_pGoodPath,g_psGoodPath);	

		//��ȡ����������������дʵ�
		ofstream  fDic(g_pCrudeDic,ios::trunc);
		Travel.DirectList(g_psBadPath,fDic);
		Travel.DirectList(g_psGoodPath,fDic);
		Travel.DirectOne(g_pAllWords,fDic);
		fDic.clear();
		fDic.close();

		CalculateNum Calc;
		ofstream fDic1(g_pAmendDic,ios::trunc);
		Calc.Init(g_pCrudeDic,fDic1);
		Calc.Calculate(g_psBadPath,g_psGoodPath);

		ofstream fDicShort(g_pShortDic,ios::trunc);
		Calc.SortDecn(&fDicShort);

		//��ȡ��������������
		Characters Chara(g_pShortDic);
		ofstream fTrain(g_pTrain);
		Chara.CharZ(g_psBadPath,fTrain,1); 	//bad  sample
		Chara.CharZ(g_psGoodPath,fTrain,2);	//good sample
		fTrain.close();
		std::cout<<"This is Train process"<<std::endl;
		system(".\\windows\\svm-scale.exe -l 0 -u 1 zTrain.txt>zTrain.scale");    //�������������ţ����������train.scale
		system(".\\windows\\svm-train.exe -c 8.0 -g 0.0078125 -h 0 zTrain.scale");  //����������ѵ�������������train.scale.model
		system(".\\windows\\svm-predict.exe zTrain.scale zTrain.scale.model goal.txt"); //��ѵ��������������Ԥ�⣬���������goal.txt	

		//���������ļ����Լ��ִ��ļ���
		Travel.DeleteFolder(g_pBadPath);
		Travel.DeleteFolder(g_pGoodPath);
		Travel.DeleteFolder(g_psBadPath);
		Travel.DeleteFolder(g_psGoodPath);
		Travel.DeleteFolder(g_pAllPath);
	}
	if (dxMode == Predict)
	{
		QtConnectMysql Qtcon;
		Qtcon.ShowDriver();
		if ( !Qtcon.Connect() )
			return 1;

		//����ѵ��ģʽ�������Լ��ִʺ������
		TraverseFolder Travel;		
		Travel.CreateFolder(g_pPreTest);
		Travel.CreateFolder(g_pSepTest);
		
		//�����ݿ��ȡԤ������
		//QString QSelect("SELECT * FROM hdf_status WHERE kind = 0");
		std::cout<<"Load data from the database now:"<<std::endl;
		QString QSelect("SELECT * FROM duan.hdf_status WHERE kind = 0");
		QSqlQuery QueryPredict(QSelect); 
		qDebug()<<QueryPredict.lastError().text();
		//Qtcon.LoadRecordForPredict(QueryPredict,g_pPreTest);
		//Qtcon.LoadFixedRecordInfile(QueryPredict,g_pPreTest,10000);
		Qtcon.LoadRecordInfile(QueryPredict,g_pPreTest);

		//���������зִ�,ҽѧ����л�ִʶ���Ҫ���
		std::cout<<"Separate the words now"<<std::endl;
		SeparateTerm Sepa(g_pAllWords,true);
		Sepa.SepInit();
		Sepa.BatchSeparate(g_pPreTest,g_pSepTest);

		//����Ԥ����������������
		std::cout<<"Calculate the feature vector now"<<std::endl;
		Characters Chara(g_pShortDic);
		ofstream fTest(g_pTest,ios::out);
		Chara.CharZ(g_pSepTest,fTest);
		fTest.close();

		//�Բ�����������Ԥ��
		std::cout<<"Predict the context now"<<std::endl;
		std::cout<<"This is predict process"<<std::endl;
		system(".\\windows\\svm-scale.exe -l 0 -u 1 zTest.txt>zTest.scale");
		system(".\\windows\\svm-predict.exe zTest.scale zTrain.scale.model goal.txt");

		//��Ԥ����д�����ݿ�
		Chara.KindofFile(g_pSepTest,g_pName);
		
		Chara.ProduceRecordFile(g_pName,g_pGoal,g_pUpdate);
		QueryPredict.exec("create table duan.judge ( id bigint(20) unsigned not null,kind smallint(6) not null,primary key(id) )");
		
		/*QueryPredict.exec("load data local infile 'C:/Users/duan/Desktop/hdf_judge/hdf_judge/zzxxzzmap.txt' replace into table \
				duan.judge fields  terminated by '  ' lines terminated by '\r\n'");*/
		QueryPredict.exec("load data local infile './zzxxzzmap.txt' replace into table duan.judge \
						  fields  terminated by '  ' lines terminated by '\r\n'");

		QueryPredict.exec("update planet.hdf_status,duan.judge set planet.hdf_status.kind = duan.judge.kind \
						  where planet.hdf_status.id = duan.judge.id");

		QueryPredict.exec("drop table duan.judge"); 

		Travel.DeleteFolder(g_pPreTest);	//ɾ������Ԥ���������ļ���
		Travel.DeleteFolder(g_pSepTest);	//ɾ������ִ��������ļ���
	}
	
	
}
