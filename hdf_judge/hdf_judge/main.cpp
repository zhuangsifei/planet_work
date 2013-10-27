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
extern const char *g_pShortDic  = ".\\zShortDic.txt";
extern const char *g_pTrain		= ".\\zTrain.txt";

ofstream g_ftest(".\\g_test.txt");
extern const int g_FiltWords = 10;
extern const int g_LeftWords = 2000;

int main(int argc, char *argv[])
{
	// �����ַ����ı��뷽ʽ��������ת��ʱҲ֪����ʲô��ת
	QTextCodec *code = QTextCodec::codecForName("GBK");
	QTextCodec::setCodecForCStrings(code);
	QtConnectMysql Qtcon;
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

	QSelect = "SELECT * FROM hdf_status WHERE effect IN ( '������' , '����֪��') AND  manner = '������'";
	QSqlQuery QueryBad(QSelect);
	Qtcon.LoadRecordInfile(QueryBad,g_pBadPath,5000);

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

	//ѵ������
	Characters Chara(g_pShortDic);
	ofstream fTrain(g_pTrain);
	Chara.CharZ(g_psBadPath,fTrain,1); 	//bad  sample
	Chara.CharZ(g_psGoodPath,fTrain,2);	//good sample
	fTrain.close();
	system(".\\windows\\svm-scale.exe -l 0 -u 1 zTrain.txt>zTrain.scale");    //�������������ţ����������train.scale
	system(".\\windows\\svm-train.exe  -h 0 zTrain.scale zTrain.scale.model");  //����������ѵ�������������train.scale.model
	//Ԥ��������
	//system(".\\windows\\svm-scale.exe -l 0 -u 1 zTrain.txt>zTrain.scale");		   //�Դ����������ţ����������left.scale
	system(".\\windows\\svm-predict.exe zTrain.scale zTrain.scale.model goal.txt");   //�Դ�������Ԥ�⣬���������result	
}
