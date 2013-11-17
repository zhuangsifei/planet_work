#include"FindKey.h"
#include"QtConnectMysql.h"
#include"TraverseFolder.h"
#include"AppConfig.h"
using namespace std;

AppConfig appConfig;
int main(int argc, char *argv[])
{
	//QCoreApplication a(argc, argv);

	char *DicPath    = ".\\merge.txt";
	char *FixPath	 = ".\\mergefix.txt";
	char *AddPath	 = ".\\mergeadd.txt";
	char *ResultPath = ".\\result.txt";
	char *unSeparatePath = ".\\unSeparate";
	char *SeparatePath   = ".\\Separate";
	char *zkind1path	 = ".\\zkind1";
	char *zkind2path	 = ".\\zkind2";
	char *SingleUnseparate    = ".\\SingalUnseparate.txt";
	char *SingleSeparate	  = ".\\SingalSeparate.txt";
	

	//Clear the file to get right result
	TraverseFolder Travel;
	Travel.DeleteFolder(".\\zkind1");	   //����ģʽ�������ǰ��������
	Travel.DeleteFolder(".\\zkind2");	   //����ģʽ�������ǰ��������

	FindKey FindWords(DicPath);				  //�ִ�ϵͳ�Լ�set��ʼ��
	FindWords.SetDealMode(FindKey::BatchMode);//ѡ���ı�����ʽ
	//FindWords.SetMapMode(FindKey::AllDone);	 //ѡ��Ȩ����ʾģʽ
	FindWords.SetMapMode(FindKey::UnshowWeight); //ѡ����ʾȨ��ģʽ
	
	//���ı�����ʾ���ʵ�Ȩ�أ��ٶ������ʺϲ���
	if(FindWords.GetMapMode() == FindKey::ShowWeight)							
	{
		FindWords.FixDic(DicPath,FixPath);			//�ؼ��ּ�Ȩ��
		FindWords.AddWords(AddPath,FixPath,DicPath);//���Ӷ���ʻ�
		FindWords.LoadDic();						//���شʵ�
		FindWords.MapLoad(FixPath);					//��ʼ��map
	}
	
	//ֱ����map���޸�Ȩ�أ��ٶȿ죬������ʾ
	if(FindWords.GetMapMode() == FindKey::UnshowWeight)
	{
		FindWords.AddWords(AddPath,DicPath);
		FindWords.LoadDic();
	}
	
	//�ֵ��Ѿ����治�ü���
	if(FindWords.GetMapMode() == FindKey::AllDone)
	{
		FindWords.AddWords(AddPath,DicPath);
	}

	//������ݿ��������Ͳ��������ݿ�
	QtConnectMysql Qtcon;
	Qtcon.ShowDriver();
	Qtcon.Connect();
	QSqlQuery query = Qtcon.getDataDase().exec("SELECT * FROM planet.wb_status WHERE topic_id>0 AND kind=0");
	QTextCodec *code = QTextCodec::codecForName("utf8");
	
	
	if(FindWords.GetDealMode() == FindKey::BatchMode)	//�޸����ݿ�
	{
		//Qtcon.UseTableModify(Qtcon.getDataDase(),"wb_status",FindWords,SingleUnseparate,SingleSeparate);
		Travel.CreateFolder(unSeparatePath);
		Travel.CreateFolder(SeparatePath);
		Qtcon.LoadRecordInfile(query,unSeparatePath);
		FindWords.SeparateWords(unSeparatePath,SeparatePath);
		FindWords.GetBatchResult(SeparatePath,ResultPath);	
		Qtcon.UpdateRecord(ResultPath,query);

		Travel.DeleteFolder(unSeparatePath);
		Travel.DeleteFolder(SeparatePath);
		Travel.DeleteFileOne(ResultPath);
	}
	
	if(FindWords.GetDealMode() == FindKey::SingleMode)	//�������ı�
	{
		Qtcon.UseTableModify(Qtcon.getDataDase(),"planet.wb_status",FindWords,SingleUnseparate,SingleSeparate);
		Travel.DeleteFileOne(SingleUnseparate);
		Travel.DeleteFileOne(SingleSeparate);
	}
	
	if(FindWords.GetDealMode() == FindKey::LoadMode)	//���ط�����
	{
		Travel.CreateFolder(zkind1path);
		Travel.CreateFolder(zkind2path);
		Qtcon.LoadRecordInfile(query,"text","id",zkind1path,zkind2path);
	}
	std::cout<<"update the sql OK\n";
	

 //  a.exit();
   return 0;
}
