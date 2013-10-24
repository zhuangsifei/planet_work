#include <QtCore/QCoreApplication>
#include "CalculateNum.h"
#include "Characters.h"
#include "TraverseFolder.h"
#include "SeparateTerm.h"
#include "QtConnectMysql.h"
#include "FindBad.h"
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

ofstream g_ftest(".\\g_test.txt");
extern const int g_FiltWords = 10;
extern const int g_LeftWords = 800;

int main(int argc, char *argv[])
{
	// 设置字符串的编码方式，这样在转码时也知道从什么码转
	QTextCodec *code = QTextCodec::codecForName("GBK");
	QTextCodec::setCodecForCStrings(code);
	QtConnectMysql Qtcon;
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
	Qtcon.LoadRecordInfile(QueryGood,g_pGoodPath,100);

	QSelect = "SELECT * FROM hdf_status WHERE effect IN ( '不满意' , '还不知道') AND  manner = '不满意'";
	QSqlQuery QueryBad(QSelect);
	Qtcon.LoadRecordInfile(QueryBad,g_pBadPath,100);

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
	//Calc.SortDecn();
}
