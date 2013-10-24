/*
�ļ�: SeparateTerm.cpp ��SeparateTerm��ʵ���ļ�
ʱ�䣺2013��9��
���ߣ�ZYS
�汾��Version 1.0
���ܣ����ı�ʵ�ִַ�
*/
#include "SeparateTerm.h"

//---------------------------------------------------------------------------------
// ����: SeparateTerm::SeparateTerm(LPCSTR pDicPath,bool bLoad)
// ����: �ִ�ϵͳ���캯��
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
// ����: SeparateTerm::SeparateTerm(const SeparateTerm& copy)
// ����: ���ƹ��캯��
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
// ����: SeparateTerm::SepInit()
// ����: ��ʼ���ִ�ϵͳ
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
// ����: SeparateTerm::LoadDic()
// ����: ���شʵ� 
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
// ����: SeparateTerm::SetLoad()
// ����: �����Ƿ�����û��ʵ�
//---------------------------------------------------------------------------------
void SeparateTerm::SetLoad(bool bLoad)
{
	m_bLoad = bLoad;
}


//---------------------------------------------------------------------------------
// ����: SeparateTerm::GetLoad()
// ����: ��ȡ�û��ʵ����ģʽ
//---------------------------------------------------------------------------------
bool SeparateTerm::GetLoad()
{
	return m_bLoad;
}


//---------------------------------------------------------------------------------
// ����: SeparateTerm::SingleSeparate(LPCSTR pSource,LPCSTR pDest)
// ����: �ѵ���һ���ı��ִʺ󱣴�����һ���ı���
//---------------------------------------------------------------------------------
void SeparateTerm::SingleSeparate(LPCSTR pSource,LPCSTR pDest)
{
	ICTCLAS_FileProcess(pSource,pDest,CODE_TYPE_UNKNOWN,0);
}

//---------------------------------------------------------------------------------
// ����: SeparateTerm::BatchSeparate(LPCSTR pSource,LPCSTR pDest)
// ����: ��һ���ļ����е�ȫ���ı��ִʺ󱣴浽��һ���ļ���
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
	strcat(FilePathName,"\\*.*");   //��*.*��Ϊ�˻�ȡ�����ļ������и�ʽ
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
// ����: SeparateTerm::SingleClear(LPCSTR pSource,LPCSTR pDest)
// ���ܣ���һ���ִʺ���ı���һЩ��������ȥ����㣬ȥ���Ǻ�������ܿ������ӣ�
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
// ����: SeparateTerm::SingleClear(LPCSTR pSource,LPCSTR pDest)
// ���ܣ���һ���ִʺ���ı���һЩ��������ȥ����㣬ȥ���Ǻ�������ܿ������ӣ�
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


//����ʵ�ַ���ϵͳ�������˳�
SeparateTerm::~SeparateTerm(void)
{
	ICTCLAS_Exit();
}
