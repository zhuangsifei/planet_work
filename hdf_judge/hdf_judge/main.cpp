#include <QtCore/QCoreApplication>
#include "CalculateNum.h"
#include "Characters.h"
#include "TraverseFolder.h"
#include "SeparateTerm.h"
#include "QtConnectMysql.h"
#include "FindBad.h"
using namespace std;

extern const char *g_pAllWords  = ".\\AllWords.txt";
extern const char *g_pThanks	= ".\\zThanks.txt";
extern const char *g_pYideGood  = ".\\zYideGood.txt";
extern const char *g_pYideBad	= ".\\zYideBad.txt";
extern const char *g_pYishuGood = ".\\zYishuGood.txt";
extern const char *g_pYishuBad  = ".\\zYishuBad.txt";
extern const char *g_pAllPath   = ".\\zAll";
extern const char *g_pBadPath	= ".\\zBad";
extern const char *g_pGoodPath	= ".\\zGood";

ofstream g_ftest(".\\g_test.txt");
extern const int g_FiltWords = 10;
extern const int g_LeftWords = 800;

int main(int argc, char *argv[])
{
	// �����ַ����ı��뷽ʽ��������ת��ʱҲ֪����ʲô��ת
	QTextCodec *code = QTextCodec::codecForName("GBK");
	QTextCodec::setCodecForCStrings(code);
	QtConnectMysql Qtcon;
	Qtcon.ShowDriver();
	if ( !Qtcon.Connect() )
		return 1;
	
	QString QSelect("SELECT * FROM hdf_status WHERE effect = '������' AND manner = '������'");
	QSqlQuery QueryGood(QSelect); 
	Qtcon.LoadRecordInfile(QueryGood,g_pGoodPath,5000);
	
	QSelect = "SELECT * FROM hdf_status WHERE effect = '������' AND manner = '������'";
	QSqlQuery QueryBad(QSelect);
	Qtcon.LoadRecordInfile(QueryBad,g_pBadPath,5000);

}
