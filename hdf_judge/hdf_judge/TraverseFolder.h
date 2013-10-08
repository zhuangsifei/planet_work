/*
文件：TraverseFolder.cpp,类TraverseFolder的cpp文件
时间：2013年9月
作者：ZYS
版本：Verson 1.0
功能: 实现对文件，文件夹的创建，删除，复制，剪切操作
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

