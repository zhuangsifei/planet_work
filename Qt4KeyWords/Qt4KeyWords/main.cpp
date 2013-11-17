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
	Travel.DeleteFolder(".\\zkind1");	   //下载模式下清除先前下载内容
	Travel.DeleteFolder(".\\zkind2");	   //下载模式下清除先前下载内容

	FindKey FindWords(DicPath);				  //分词系统以及set初始化
	FindWords.SetDealMode(FindKey::BatchMode);//选择文本处理方式
	//FindWords.SetMapMode(FindKey::AllDone);	 //选择权重显示模式
	FindWords.SetMapMode(FindKey::UnshowWeight); //选择不显示权重模式
	
	//在文本中显示单词的权重，速度慢，适合测试
	if(FindWords.GetMapMode() == FindKey::ShowWeight)							
	{
		FindWords.FixDic(DicPath,FixPath);			//关键字加权重
		FindWords.AddWords(AddPath,FixPath,DicPath);//增加额外词汇
		FindWords.LoadDic();						//加载词典
		FindWords.MapLoad(FixPath);					//初始化map
	}
	
	//直接在map中修改权重，速度快，不能显示
	if(FindWords.GetMapMode() == FindKey::UnshowWeight)
	{
		FindWords.AddWords(AddPath,DicPath);
		FindWords.LoadDic();
	}
	
	//字典已经保存不用加载
	if(FindWords.GetMapMode() == FindKey::AllDone)
	{
		FindWords.AddWords(AddPath,DicPath);
	}

	//检测数据库驱动类型并连接数据库
	QtConnectMysql Qtcon;
	Qtcon.ShowDriver();
	Qtcon.Connect();
	QSqlQuery query = Qtcon.getDataDase().exec("SELECT * FROM planet.wb_status WHERE topic_id>0 AND kind=0");
	QTextCodec *code = QTextCodec::codecForName("utf8");
	
	
	if(FindWords.GetDealMode() == FindKey::BatchMode)	//修改数据库
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
	
	if(FindWords.GetDealMode() == FindKey::SingleMode)	//批处理文本
	{
		Qtcon.UseTableModify(Qtcon.getDataDase(),"planet.wb_status",FindWords,SingleUnseparate,SingleSeparate);
		Travel.DeleteFileOne(SingleUnseparate);
		Travel.DeleteFileOne(SingleSeparate);
	}
	
	if(FindWords.GetDealMode() == FindKey::LoadMode)	//下载分类结果
	{
		Travel.CreateFolder(zkind1path);
		Travel.CreateFolder(zkind2path);
		Qtcon.LoadRecordInfile(query,"text","id",zkind1path,zkind2path);
	}
	std::cout<<"update the sql OK\n";
	

 //  a.exit();
   return 0;
}
