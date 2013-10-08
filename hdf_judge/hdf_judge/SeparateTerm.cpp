/*
文件: SeparateTerm.cpp 类SeparateTerm的实现文件
时间：2013年9月
作者：ZYS
版本：Version 1.0
功能：对文本实现分词
*/
#include "SeparateTerm.h"


SeparateTerm::SeparateTerm(void)
{
	std::cout<<"This is using the system dictionary\n";
	m_Save = Load;
	m_Svm = Train;
}

SeparateTerm::SeparateTerm(char *path)
{
	m_DicPath = path;
	m_Save = Load;
	m_Svm = Train;
	std::cout<<"This is using the use's dictionary\n";
}
SeparateTerm::SeparateTerm(const SeparateTerm& copy)
{
	m_DicPath = new char[strlen(copy.m_DicPath) + 1];
	strcpy(m_DicPath,copy.m_DicPath);
}
//分词系统初始化
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
void SeparateTerm::LoadDic()
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
void SeparateTerm::SepSave(LPCSTR path0,LPCSTR path1)
{
	WIN32_FIND_DATA findData;
	HANDLE hError;
	int nNumFiles;
	char FilePathName[MAX_PATH];
	char FullPathName[MAX_PATH];
	char DestPathName[MAX_PATH];
	

	strcpy(FilePathName,path0);
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
		sprintf(FullPathName,"%s\\%s",path0,findData.cFileName);
		sprintf(DestPathName,"%s\\%s",path1,findData.cFileName);
		
		ICTCLAS_FileProcess(FullPathName,DestPathName,CODE_TYPE_UNKNOWN,0);
	}
}

void SeparateTerm::SetDicMode(SaveMode mode)
{
	m_Save = mode;
}
SeparateTerm::SaveMode SeparateTerm::GetDicMode()
{
	return m_Save;
}
void SeparateTerm::SetSvmMode(SvmMode mode)
{
	m_Svm = mode;
}
SeparateTerm::SvmMode SeparateTerm::GetSvmMode()
{
	return m_Svm;
}
//析构实现分析系统的正常退出
SeparateTerm::~SeparateTerm(void)
{
	ICTCLAS_Exit();
}
