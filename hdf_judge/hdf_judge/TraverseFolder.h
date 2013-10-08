/*
�ļ���TraverseFolder.cpp,��TraverseFolder��cpp�ļ�
ʱ�䣺2013��9��
���ߣ�ZYS
�汾��Verson 1.0
����: ʵ�ֶ��ļ����ļ��еĴ�����ɾ�������ƣ����в���
*/
#pragma once
#include<string>
#include<windows.h>
#include<iostream>
#include<fstream>
using namespace std;

class TraverseFolder
{
public:
	TraverseFolder(void);
	TraverseFolder(string strDirect);
	bool DirectList(LPCSTR path,ofstream* pfout);
	bool DirectList(LPCSTR path1,LPCSTR path2);
	/*---------------the following is used for test-------------------*/
	bool CreateFolder(LPCSTR);
	bool CopyfileTofolder(LPCSTR path1,LPCSTR path2);
	bool CopyfolderTofolder(LPCSTR path1,LPCSTR path2);
	bool CutfileTofolder(LPCSTR path1,LPCSTR path2);
	bool CutfolderTofolder(LPCSTR path1,LPCSTR path2);
	bool DeleteFileOne(LPCSTR path);
	bool DeleteFolder(LPCSTR ppath);
	/*---------------------------------------------------------------*/
	~TraverseFolder(void);
private:
	string m_strDir;
};

