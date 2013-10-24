/*
文件: SeparateTerm.cpp 类SeparateTerm的实现文件
时间：2013年9月
作者：ZYS
版本：Version 1.0
功能：对文本实现分词
*/
#include "SeparateTerm.h"

//---------------------------------------------------------------------------------
// 名字: SeparateTerm::SeparateTerm(LPCSTR pDicPath,bool bLoad)
// 功能: 分词系统构造函数
//---------------------------------------------------------------------------------
SeparateTerm::SeparateTerm(LPCSTR pDicPath,bool bLoad)
{
	m_pDicPath = pDicPath;
	m_bLoad    = bLoad;

	SepInit();
	if(m_bLoad)
	{
		LoadDic();
	}
	else
	{
		std::cout<<"This is using the system's dictionary\n";
	}
}


//---------------------------------------------------------------------------------
// 名字: SeparateTerm::SeparateTerm(const SeparateTerm& copy)
// 功能: 复制构造函数
//---------------------------------------------------------------------------------
SeparateTerm::SeparateTerm(const SeparateTerm& copy)
{
	m_bLoad  = copy.m_bLoad;
	m_pDicPath = new const char[strlen(copy.m_pDicPath) + 1];
	if (m_pDicPath == NULL)
	{
		std::cout<<"No enough space for the copying\n";
	}
	else
	{
		strcpy(const_cast<char*>(m_pDicPath),copy.m_pDicPath);
	}
}


//---------------------------------------------------------------------------------
// 名字: SeparateTerm::SepInit()
// 功能: 初始化分词系统
//---------------------------------------------------------------------------------
void SeparateTerm::SepInit()
{
	if(!ICTCLAS_Init(".\\API"))
	{
		cout<<"ICTCLAS Init Failed\n";
		exit(1);
	}
	else
	{
		cout<<"ICTCLAS Init OK!\n";
	}
}


//---------------------------------------------------------------------------------
// 名字: SeparateTerm::LoadDic()
// 功能: 加载词典 
//---------------------------------------------------------------------------------
void SeparateTerm::LoadDic()
{
	unsigned int nItems= ICTCLAS_ImportUserDictFile(m_pDicPath,CODE_TYPE_UNKNOWN);
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


//---------------------------------------------------------------------------------
// 名字: SeparateTerm::SetLoad()
// 功能: 设置是否加载用户词典
//---------------------------------------------------------------------------------
void SeparateTerm::SetLoad(bool bLoad)
{
	m_bLoad = bLoad;
}


//---------------------------------------------------------------------------------
// 名字: SeparateTerm::GetLoad()
// 功能: 获取用户词典加载模式
//---------------------------------------------------------------------------------
bool SeparateTerm::GetLoad()
{
	return m_bLoad;
}


//---------------------------------------------------------------------------------
// 名字: SeparateTerm::SingleSeparate(LPCSTR pSource,LPCSTR pDest)
// 功能: 把单独一个文本分词后保存在另一个文本中
//---------------------------------------------------------------------------------
void SeparateTerm::SingleSeparate(LPCSTR pSource,LPCSTR pDest)
{
	ICTCLAS_FileProcess(pSource,pDest,CODE_TYPE_UNKNOWN,0);
}

//---------------------------------------------------------------------------------
// 名字: SeparateTerm::BatchSeparate(LPCSTR pSource,LPCSTR pDest)
// 功能: 把一个文件夹中的全部文本分词后保存到另一个文件夹
//---------------------------------------------------------------------------------
void SeparateTerm::BatchSeparate(LPCSTR pSource,LPCSTR pDest)
{
	WIN32_FIND_DATA findData;
	HANDLE hError;
	int nNumFiles;
	char FilePathName[MAX_PATH];
	char FullPathName[MAX_PATH];
	char DestPathName[MAX_PATH];

	strcpy(FilePathName,pSource);
	strcat(FilePathName,"\\*.*");   //加*.*是为了获取所有文件的所有格式
	hError = FindFirstFile(FilePathName,&findData);
	if(hError == INVALID_HANDLE_VALUE)
	{
		cout<<"Search File Failed\n";
		return;
	}


	while(::FindNextFile(hError,&findData))
	{
		if(strcmp(findData.cFileName,".") ==0||strcmp(findData.cFileName,"..") ==0)
		{
			continue;
		}
		sprintf(FullPathName,"%s\\%s",pSource,findData.cFileName);
		sprintf(DestPathName,"%s\\%s",pDest,findData.cFileName);
		
		ICTCLAS_FileProcess(FullPathName,DestPathName,CODE_TYPE_UNKNOWN,0);
	}
}

//---------------------------------------------------------------------------------
// 名字: SeparateTerm::SingleClear(LPCSTR pSource,LPCSTR pDest)
// 功能：把一个分词后的文本做一些处理，比如去除标点，去除非汉语。（功能可以增加）
//---------------------------------------------------------------------------------
void SeparateTerm::SingleClearWord(LPCSTR pSource,LPCSTR pDest)
{
	ifstream fin(pSource,ios::in);
	ofstream fout(pDest,ios::out);
	string str;
	int nLength = 0;
	int i = 0;
	while (fin.good())
	{
		fin>>str;
		nLength = str.length();
		for(i = 0; i<nLength;)
		{
			if(str[i] < 0)
				i += 2;
			else 
				break;
		}
		if( i == nLength )
			fout<<str<<endl;
	}
	fin.close();
	fout.close();
}


//---------------------------------------------------------------------------------
// 名字: SeparateTerm::SingleClear(LPCSTR pSource,LPCSTR pDest)
// 功能：把一个分词后的文本做一些处理，比如去除标点，去除非汉语。（功能可以增加）
//---------------------------------------------------------------------------------
void SeparateTerm::BatchClearWord(LPCSTR pSource,LPCSTR pDest )
{
	WIN32_FIND_DATA findData;
	HANDLE hError = NULL;
	char FilePathName[MAX_PATH] = {'\0'};
	char FullPathName[MAX_PATH] = {'\0'};
	char DestPathName[MAX_PATH] = {'\0'};

	strcpy(FilePathName,pSource);
	strcat(FilePathName,"\\*.*");
	hError = FindFirstFile(FilePathName,&findData);
	if (hError == INVALID_HANDLE_VALUE)
	{
		std::cout<<"Find the first file failed when doing batchClear\n";
		return;
	}

	while( ::FindNextFile(hError,&findData) )
	{
		if (strcmp(findData.cFileName,".") == 0 || strcmp(findData.cFileName,"..") ==0)
			continue;
		sprintf(FullPathName,"%s\\%s",pSource,findData.cFileName);
		sprintf(DestPathName,"%s\\%s",pDest,findData.cFileName);
		SingleClearWord(FullPathName,DestPathName);
	}
 

}


//析构实现分析系统的正常退出
SeparateTerm::~SeparateTerm(void)
{
	ICTCLAS_Exit();
}
