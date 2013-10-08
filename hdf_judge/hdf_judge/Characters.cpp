/*
�ļ���Charaters.cpp,��Character��cpp�ļ�
ʱ�䣺2013��9��
���ߣ�ZYS
�汾��Version 1.0
���ܣ���ȡÿ�������������������Թ�libsvmѵ��
*/
#include "Characters.h"

Characters::Characters(void)
{
}
Characters::Characters(LPCSTR path)
{
	ifstream fin(path);
	string str;
	while(fin.good())
	{
		fin>>str;
		m_DicMap.insert(pair<string,int>(str,0));
		fin>>str;
	}
	fin.close();

	// give the sorted number of the string
	map<string,int>::iterator iter;
	int cnt = 0;
	for(iter=m_DicMap.begin();iter!=m_DicMap.end();iter++)
	{
		cnt++;
		iter->second = cnt;
	}
}

//TFIDF�㷨
void Characters::CharY()
{

}

//�Ľ���bool�㷨�����ɷִ��ı�������������д�뵽ͬһ���ı���
void Characters::CharZ(LPCSTR trainPath,ofstream* pfstr,int label)
{
	WIN32_FIND_DATA findData;
	HANDLE hSearch;
	char FilePathName[MAX_PATH];
	char FullPathName[MAX_PATH];
	strcpy(FilePathName,trainPath);
	strcat(FilePathName,".\\*.*");

	hSearch = FindFirstFile(FilePathName,&findData);
	if( hSearch == INVALID_HANDLE_VALUE)
	{
		std::cout<<"Scan files failed\n";
		return;
	}
	while(::FindNextFile(hSearch ,&findData))
	{
		if(strcmp(findData.cFileName,".") ==0||strcmp(findData.cFileName,"..") ==0)
		{
			continue;
		}
		sprintf(FullPathName,"%s\\%s",trainPath,findData.cFileName);
		set<string> strSet;
		SortFileWords(FullPathName,strSet);

		set<string>::iterator iter;
		map<string,int>::iterator pter;
		string strf;
		(*pfstr)<<label<<" ";
		for(iter=strSet.begin();iter!=strSet.end();iter++)
		{
			pter = m_DicMap.find(*iter);
			if(pter != m_DicMap.end() )
			{
				(*pfstr)<<pter->second<<":"<<1<<" ";
			}
			else
			{
				//(*pfstr)<<pter->second<<":"<<0<<" ";
			}
		}
		(*pfstr)<<endl;	
	}
	::FindClose(hSearch);
}

//��һ���ı��еķִʴʻ�д�� set��
void Characters::SortFileWords(LPCSTR path,set<string>& strSet)
{
	ifstream fin(path);
	string str;
	while(fin.good())
	{
		fin>>str;
		strSet.insert(str);
	}
	fin.close();
}
void Characters::KindofFile(LPCSTR path1,LPCSTR path2,LPCSTR pathRecord)
{
	WIN32_FIND_DATA findData;
	HANDLE hSearch;
	ofstream  fout(pathRecord,ios::trunc);
	if(!fout)
		{
			std::cout<<"open file failed\n";
			std::cout<<"error"<<GetLastError();
		}
	
	string strFile;
	char tempFileName[MAX_PATH];
	char FilePathName[MAX_PATH];

	//���������ļ�1����ȡ�ļ�����ID
	strcpy(FilePathName,path1);
	strcat(FilePathName,"\\*.*");
	hSearch = FindFirstFile(FilePathName,&findData);
	if( hSearch == INVALID_HANDLE_VALUE)
	{
		std::cout<<"searc file failed\n";
		exit(1);
	}
	while(::FindNextFile(hSearch,&findData))
	{
		if(strcmp(findData.cFileName ,".")==0 || strcmp(findData.cFileName,"..")==0)
		{
			continue;
		}
		int i = 0;
		
		while(findData.cFileName[i] != '.')//��ȡ�ļ�����
		{
			tempFileName[i] = findData.cFileName[i];
			i++;
		}
		tempFileName[i] = '\0';
		strFile = tempFileName;
		fout<<strFile<<"  "<<endl;
		strFile.clear();
	}

	//���������ļ�2����ȡ�ļ�����ID
	strcpy(FilePathName,path2);
	strcat(FilePathName,"\\*.*");
	hSearch = FindFirstFile(FilePathName,&findData);
	if( hSearch == INVALID_HANDLE_VALUE)
	{
		std::cout<<"searc file failed\n";
		exit(1);
	}
	while(::FindNextFile(hSearch,&findData))
	{
		if(strcmp(findData.cFileName ,".")==0 || strcmp(findData.cFileName,"..")==0)
		{
			continue;
		}
		int i = 0;
		while(findData.cFileName[i] != '.')
		{
			tempFileName[i] = findData.cFileName[i];
			i++;
		}
		tempFileName[i] = '\0';
		strFile = tempFileName;
		fout<<strFile<<"  "<<endl;
		strFile.clear();
	}
	fout.close();
}

//pathRecord: ����ļ�����pathResult:����ļ����о���������������߶�Ӧд��һ��map���档
void Characters::KindtoMap(LPCSTR pathRecord,LPCSTR pathResult,map<string,int,bool (*)(string,string)>& KindMap)
{
	ifstream fin(pathRecord,ios::in);
	ifstream fgoal(pathResult,ios::in);
	string str,name;
	int  kind;
	stringstream sstr;

	while(fin.good())
	{
		fin>>name;
		fgoal>>str;
		sstr.str(str);
		sstr>>kind;
		sstr.clear();
		KindMap.insert(pair<string,int>(name,kind));
	}
	fin.close();
	fgoal.close();

}
bool Characters::LessComp(string str1,string str2)
{
	int L1,L2;
	L1 = str1.length();
	L2 = str2.length();
	if(L1 < L2)
		return true;
	if(L1 > L2)
		return false;
	if(L1 == L2)
	{
		if( str1.compare(str2) <0 )
			return true;
		else 
			return false;
	}
}
Characters::~Characters(void)
{
	m_DicMap.clear();
}
