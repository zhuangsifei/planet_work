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

extern const char *g_pPreTest	= ".\\zPreTest";	//预测样本存放文件夹
extern const char *g_pSepTest	= ".\\zSepTest";	//预测样本分词文件夹
extern const char *g_pTest		= ".\\zTest.txt";	//预测样本特征向量
extern const char *g_pGoal		= ".\\goal.txt";	//预测结果
extern const char *g_pName		= ".\\nameID.txt";	//表中keyID存放文本中
extern const char *g_pUpdate	= ".\\zzxxzzmap.txt";	//保存更新记录的文件

ofstream g_ftest(".\\g_test.txt");
extern const int g_FiltWords = 10;	 //过滤低频词汇
extern const int g_LeftWords = 2000; //特征向量数目
AppConfig appConfig;				 //操作配置文件
int main(int argc, char *argv[])
{
	// 设置字符串的编码方式，这样在转码时也知道从什么码转
	QTextCodec *code = QTextCodec::codecForName("GBK");
	QTextCodec::setCodecForCStrings(code);
	enum mode{Train,Predict};
	//mode dxMode = Train;
	mode dxMode = Predict;

	if ( dxMode == Train)
	{
		//QtConnectMysql Qtcon("192.168.3.62","planet","root","dev",3306);
		QtConnectMysql Qtcon;// 选择配置文件中的数据库
		Qtcon.ShowDriver();
		if ( !Qtcon.Connect() )
			return 1;

		//创建并清理文件夹
		TraverseFolder Travel;
		Travel.CreateFolder(g_pBadPath);
		Travel.CreateFolder(g_pGoodPath);
		Travel.CreateFolder(g_psBadPath);
		Travel.CreateFolder(g_psGoodPath);

		Travel.ClearFolder(g_pBadPath);
		Travel.ClearFolder(g_pGoodPath);
		Travel.ClearFolder(g_psBadPath);
		Travel.ClearFolder(g_psGoodPath);


		//获取好坏两种样本
		QString QSelect("SELECT * FROM hdf_status WHERE effect = '很满意' AND manner = '很满意'");
		QSqlQuery QueryGood(QSelect); 
		Qtcon.LoadRecordInfile(QueryGood,g_pGoodPath,5000);
		//Qtcon.LoadFixedRecordInfile(QueryGood,g_pGoodPath,5000);

		QSelect = "SELECT * FROM hdf_status WHERE effect IN ( '不满意' , '还不知道') AND  manner = '不满意'";
		QSqlQuery QueryBad(QSelect);
		Qtcon.LoadRecordInfile(QueryBad,g_pBadPath,5000);
		//Qtcon.LoadFixedRecordInfile(QueryBad,g_pBadPath,5000);

		//对样本进行分词,医学，感谢分词都需要添加
		SeparateTerm Sepa(g_pAllWords,true);
		Sepa.SepInit();
		Sepa.BatchSeparate(g_pBadPath,g_psBadPath);
		Sepa.BatchSeparate(g_pGoodPath,g_psGoodPath);	

		//提取特征向量，生成情感词典
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

		//提取样本的特征向量
		Characters Chara(g_pShortDic);
		ofstream fTrain(g_pTrain);
		Chara.CharZ(g_psBadPath,fTrain,1); 	//bad  sample
		Chara.CharZ(g_psGoodPath,fTrain,2);	//good sample
		fTrain.close();
		std::cout<<"This is Train process"<<std::endl;
		system(".\\windows\\svm-scale.exe -l 0 -u 1 zTrain.txt>zTrain.scale");    //对样本进行缩放，结果保存在train.scale
		system(".\\windows\\svm-train.exe -c 8.0 -g 0.0078125 -h 0 zTrain.scale");  //对样本进行训练，结果保存在train.scale.model
		system(".\\windows\\svm-predict.exe zTrain.scale zTrain.scale.model goal.txt"); //对训练样本自身样本预测，结果保存在goal.txt	

		//清理样本文件夹以及分词文件夹
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

		//清理训练模式中样本以及分词后的样本
		TraverseFolder Travel;		
		Travel.CreateFolder(g_pPreTest);
		Travel.CreateFolder(g_pSepTest);
		
		//从数据库获取预测样本
		//QString QSelect("SELECT * FROM hdf_status WHERE kind = 0");
		std::cout<<"Load data from the database now:"<<std::endl;
		QString QSelect("SELECT * FROM duan.hdf_status WHERE kind = 0");
		QSqlQuery QueryPredict(QSelect); 
		qDebug()<<QueryPredict.lastError().text();
		//Qtcon.LoadRecordForPredict(QueryPredict,g_pPreTest);
		//Qtcon.LoadFixedRecordInfile(QueryPredict,g_pPreTest,10000);
		Qtcon.LoadRecordInfile(QueryPredict,g_pPreTest);

		//对样本进行分词,医学，感谢分词都需要添加
		std::cout<<"Separate the words now"<<std::endl;
		SeparateTerm Sepa(g_pAllWords,true);
		Sepa.SepInit();
		Sepa.BatchSeparate(g_pPreTest,g_pSepTest);

		//计算预测样本的特征向量
		std::cout<<"Calculate the feature vector now"<<std::endl;
		Characters Chara(g_pShortDic);
		ofstream fTest(g_pTest,ios::out);
		Chara.CharZ(g_pSepTest,fTest);
		fTest.close();

		//对测试样本进行预测
		std::cout<<"Predict the context now"<<std::endl;
		std::cout<<"This is predict process"<<std::endl;
		system(".\\windows\\svm-scale.exe -l 0 -u 1 zTest.txt>zTest.scale");
		system(".\\windows\\svm-predict.exe zTest.scale zTrain.scale.model goal.txt");

		//将预测结果写道数据库
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

		Travel.DeleteFolder(g_pPreTest);	//删除保存预测样本的文件夹
		Travel.DeleteFolder(g_pSepTest);	//删除保存分词样本的文件夹
	}
	
	
}
