#include"FindKey.h"

FindKey::FindKey(void)
{
	if( !ICTCLAS_Init(".\\API") )
	{
		std::cout<<"ICTCLAS Init Failed\n";
	}
	else
	{
		std::cout<<"ICTCLAS Init successfully!\n";
	}

	m_DicPath = new char[strlen(".\\merge.txt") + 1];
	strcpy(m_DicPath,".\\merge.txt"); //默认词典路径
	m_Mode = BaseMode;
	m_WeightShow = AllDone;
}
FindKey::FindKey(char *p)
{
	if( !ICTCLAS_Init(".\\API") )
	{
		std::cout<<"ICTCLAS Init Failed\n";
	}
	else
	{
		std::cout<<"ICTCLAS Init successfully!\n";
	}

	m_DicPath = new char[strlen(p) + 1];
	strcpy(m_DicPath,p);
	m_Mode = BaseMode;
	m_WeightShow = AllDone;
	ifstream fin(m_DicPath);
	string str;
	while(fin.good())
	{
		fin>>str;
		m_Set.insert(str);
		m_Map.insert(pair<string,int>(str,10));
	}
	fin.close();
	std::cout<<"Load key words in Set successfully\n";

}
FindKey::FindKey(const FindKey& copy)
{
	m_DicPath = new char[ strlen(copy.m_DicPath) + 1];
	strcpy(m_DicPath,copy.m_DicPath);
	m_Set = copy.m_Set;
	m_Map = copy.m_Map;
	m_Mode = copy.m_Mode;
	m_WeightShow = copy.m_WeightShow;
}

FindKey::DealMode FindKey::GetDealMode()
{
	return m_Mode;
}
void FindKey::SetDealMode(DealMode nMode)
{
	m_Mode = nMode;
}

FindKey::MapMode FindKey::GetMapMode()
{
	return m_WeightShow;
}

void FindKey::SetMapMode(MapMode nMode)
{
	m_WeightShow = nMode;
}

void FindKey::LoadDic()
{
	unsigned int nItems= ICTCLAS_ImportUserDictFile(m_DicPath,CODE_TYPE_UNKNOWN);
	if(nItems)
	{
		std::cout<<"Load dictionary successfully!\n";
	}
	else
	{
		std::cout<<"Load dictionary unsuccessfully!\n";
	}

	unsigned int nSuc = ICTCLAS_SaveTheUsrDic();
	if(nSuc ==1)
	{
		std::cout<<"Save dictionary successfully\n";
	}
	else
	{
		std::cout<<"Save dictionary unsuccessfully\n";
	}
}
void FindKey::SeparateWords(LPCSTR path1,LPCSTR path2)
{

	WIN32_FIND_DATA findData;
	HANDLE hError;
	char FilePathName[MAX_PATH];
	char FullPathName1[MAX_PATH];
	char FullPathName2[MAX_PATH];
	strcpy(FilePathName,path1);
	strcat(FilePathName,"\\*.txt");

	hError = FindFirstFileA(FilePathName,(LPWIN32_FIND_DATAA)&findData);
	if(hError == INVALID_HANDLE_VALUE)
	{
		cout<<"Scan files failed!\n";
		exit(1);
	}
	while(::FindNextFile(hError ,&findData))
	{
		sprintf(FullPathName1,"%s\\%s",path1,findData.cFileName);
		sprintf(FullPathName2,"%s\\%s",path2,findData.cFileName);
		ICTCLAS_FileProcess(FullPathName1,FullPathName2,CODE_TYPE_UNKNOWN,0);
	}
}
void FindKey::SingleSeparateWords(LPCSTR path1,LPCSTR path2)
{
	ICTCLAS_FileProcess(path1,path2,CODE_TYPE_UNKNOWN,0);
}

bool FindKey::CheckString(string str)
{
	int nlth = str.length();
	for(int i=0;i<nlth;)
	{
		if( str[i]<0 )
		{
			i+=2;
		}
		else
		{
			return false;
		}
	}
	return true;
}

void FindKey::ClearWords(LPCSTR path,map<string,int>& MapStr)
{
	ifstream fin(path);
	string str;
	pair<map<string,int>::iterator,bool> bp;
	while(fin.good())
	{
		fin>>str;
		if( CheckString(str) )
		{
			bp = MapStr.insert(pair<string,int>(str,1));
			if(!bp.second)
			{
				(bp.first)->second++;
			}
		}
	}
	fin.close();
}

bool FindKey::CompareWords(const set<string>& SetStr,map<string,int>& MapStr)
{
	map<string,int>::iterator iter;
	string str;
	int cnt  = 0;
	int flag = 0;
	for (iter=MapStr.begin();iter!=MapStr.end();iter++)
	{
		str = iter->first;
		if( SetStr.find(str)!=SetStr.end() )
		{
			cnt += iter->second;
			flag ++;
		}

	}
	if(flag == 0 || cnt ==1)
		return false;
	else
		return true;

}
bool FindKey::CompareWords(const map<string,int>& MapDic,map<string,int>& MapStr)
{
	map<string,int>::const_iterator iterd;
	map<string,int>::iterator iters;
	string str;
	int value = 0;
	for(iters=MapStr.begin();iters!=MapStr.end();iters++)
	{
		str = iters->first;
		iterd = MapDic.find(str);
		if( iterd != MapDic.end())
		{
			value += iterd->second;
		}
	}
	if(value>=20)
		return true;
	else
		return false;
}
void FindKey::GetBatchResult(LPCSTR path1,ofstream *pfout)
{
	WIN32_FIND_DATA findData;
	HANDLE hError;
	char FilePathName[MAX_PATH];
	char FullPathName[MAX_PATH];
	strcpy(FilePathName,path1);
	strcat(FilePathName,"\\*.txt");

	map<string,int>MapStr;
	fstream fout;				//遍历每个文件关键词分别写进文件
	map<string,int>::iterator iter;
	TCHAR Clear[MAX_PATH] = TEXT(".\\Clear"); //净词文件
	TCHAR Final[MAX_PATH] = TEXT(".\\Final"); //结果文件
	CreateDirectory(Clear,NULL);
	CreateDirectory(Final,NULL);

	hError = FindFirstFile(FilePathName,&findData);
	if(hError == INVALID_HANDLE_VALUE)
	{
		std::cout<<"Scan files failed!\n";
		exit(1);
	}
	while(::FindNextFile(hError,&findData))
	{
		sprintf(FullPathName,"%s\\%s",path1,findData.cFileName);
		ClearWords(FullPathName,MapStr);
		/*------------------------Load key words in folder-------------------*/
		sprintf(FullPathName,"%s\\%s",Final,findData.cFileName);
		string str;
		fout.open(FullPathName,ios::out);
		int cnt  = 0;
		int flag = 0;
		for (iter=MapStr.begin();iter!=MapStr.end();iter++)
		{
			str = iter->first;
			if(m_Set.find(str)!=m_Set.end())
			{
				flag ++;
				cnt += iter->second;
				fout<<str<<" :"<<iter->second<<endl;
			}
		}
		fout<<"Key words number :"<<cnt<<endl;
		fout<<"Key words kinds :"<<flag<<endl;
		if(flag == 0 || cnt == 1)
		{
			fout<<"This is irrelevant with TCM"<<endl;
			std::cout<<findData.cFileName<<" 无关"<<endl;
			(*pfout)<<2<<endl;  //分词结果输出到文本文件中
		}
		else
		{
			fout<<"This is relevant with TCM"<<endl;
			(*pfout)<<0<<endl;	//分词结果输出到文本文件中
		}
		fout.close();
		/*---------------------------end-------------------------------------*/
		/*if ( CompareWords(m_Set,MapStr) )
		{
			cout<<"1"<<endl;
			(*pfout)<<1<<"  "<<findData.cFileName<<"有关"<<endl;
		}
		else
		{
			cout<<"0"<<endl;
			(*pfout)<<0<<"  "<<findData.cFileName<<"无关"<<endl;
		}*/

		/*----------Load the clear words into the Clear directory------------*/
		
		sprintf(FullPathName,"%s\\%s",Clear,findData.cFileName);
		fout.open(FullPathName,ios::out);
		for (iter=MapStr.begin();iter!=MapStr.end();iter++)
		{
			fout<<iter->first<<": "<<iter->second<<"  ；";
		}
		fout.close();
		/*------------------------end---------------------------------------*/
		MapStr.clear();
	}
	//(*pfout).close();
}

int FindKey::SingleResult1(map<string,int> MapStr)
{
	if( FindKey::CompareWords(m_Set,MapStr) )
		return 1;
	else
		return 2;
}
int FindKey::SingleResult2(map<string,int> MapStr)
{
	if( FindKey::CompareWords(m_Map,MapStr))
		return 1;
	else 
		return 2;
}
void FindKey::FixDic(LPCSTR path1,LPCSTR path2)
{
	ifstream fin(path1,ios::in);
	ofstream fout(path2,ios::trunc);
	string str;
	while(fin.good())
	{
		fin>>str;
		if(str != "")
		{
			str += " 10";
			fout<<str<<endl;
			str.clear();
		}
	}
	fin.close();
	fout.close();
}

void FindKey::AddWords(LPCSTR path1,LPCSTR path2,LPCTSTR path3)
{
	ifstream fin(path1);
	fstream  fout(path2);
	ofstream fdic(path3,ios::app);
	string str;

	fdic<<"\n";	//防止添加词汇追尾
	fout.seekp(0,ios::end);
	fout<<"\n"; //防止追尾
	
	while(fin.good())
	{
		fin>>str;
		if( m_Set.find(str) == m_Set.end())
		{
			m_Set.insert(str);  //扩充m_Set
			fdic<<str<<endl;	//扩充字典以待加载
			str += " 20";
			fout.seekp(0,ios::end);
			fout<<str<<endl;
			str.clear();
			
		}
		else
		{
			fout.seekg(ios::beg);
			fout.seekp(ios::beg);
			fout.clear();
			string fstr;
			while(!fout.eof())
			{
				fout>>fstr;
				int nlength = fout.tellg();
				if(str == fstr)
				{
					fout>> fstr;
					fout.seekp( (int)fout.tellg() - fstr.length());
					fout<<"20";
					break;
				}
				fout>>fstr;
			}
		}

	}
	fin.close();
	fout.close();
	fdic.close();
}
void FindKey::AddWords(LPCSTR path,LPCSTR dicPath)
{
	ifstream fin(path);
	ofstream fdic(dicPath,ios::app);
	string str;

	fdic<<"\n";	//防止添加词汇追尾
	map<string,int>::iterator iter;
	while(fin.good())
	{
		fin>>str;
		iter = m_Map.find(str);
		if(iter == m_Map.end())
		{
			m_Set.insert(str);  //扩充m_Set
			fdic<<str<<endl;	//扩充字典以待加载
			m_Map.insert(pair<string,int>(str,20));
			str.clear();
		}
		else
		{
			iter->second =20;
		}

	}
	fin.close();
	fdic.close();
	std::cout<<"The size of the map is :"<<m_Map.size()<<endl;
}
void FindKey::MapLoad(LPCSTR path)
{
	ofstream fout(".\\1020.txt");
	ifstream fin(path);
	string str1,str2;
	stringstream sstr;
	int value = 0;
	while(fin.good())
	{
		fin>>str1;
		fin>>str2;
		sstr.str(str2);
		sstr>>value;
		sstr.clear();
		fout<<value<<endl;
		m_Map.insert(pair<string,int>(str1,value));
	}
	fin.close();
	fout.close();
	std::cout<<"The size of the map is :"<<m_Map.size()<<endl;
}
void FindKey::TestSet(string str)
{
	
	if(m_Set.find(str)!=m_Set.end())
	{
		cout<<str<<" is in the set\n";
	}
	else
	{
		cout<<str<<"is not in the set\n";
	}
	std::cout<<"The size of the set is:"<<m_Set.size();
}
void FindKey::GetBatchResult(LPCSTR pSeparate,LPCSTR pGoal)
{
	WIN32_FIND_DATA findData;
	HANDLE hError;
	char FilePathName[MAX_PATH];
	char FullPathName[MAX_PATH];
	strcpy(FilePathName,pSeparate);
	strcat(FilePathName,"\\*.txt");

	map<string,int>MapStr;
	ofstream fout(pGoal);
	map<string,int>::iterator iter;
	string str;
	hError = FindFirstFile(FilePathName,&findData);
	if(hError == INVALID_HANDLE_VALUE)
	{
		std::cout<<"Scan files failed!\n";
		exit(1);
	}
	while(::FindNextFile(hError,&findData))
	{
		sprintf(FullPathName,"%s\\%s",pSeparate,findData.cFileName);
		ClearWords(FullPathName,MapStr);
		int kind = SingleResult2(MapStr);
		str = findData.cFileName;
		str.erase(str.length()-4,4);
		fout<<str<<"  ";
		fout<<kind<<endl;
		MapStr.clear();
		str.clear();
	}
	fout.close();
}
FindKey::~FindKey(void)
{
	ICTCLAS_Exit();
	delete m_DicPath;
	m_Set.clear();
	m_Map.clear();
}
