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
	/*---------------the following is used for test-------------------*/
	bool CreateFolder(LPCSTR);
	bool CopyfileTofolder(LPCSTR path1,LPCSTR path2);
	bool CopyfolderTofolder(LPCSTR path1,LPCSTR path2);
	bool CutfileTofolder(LPCSTR path1,LPCSTR path2);
	bool CutfolderTofolder(LPCSTR path1,LPCSTR path2);
	bool DeleteFileOne(LPCSTR path);
	bool DeleteFolder(LPCSTR ppath);
	bool ClearFolder(LPCSTR path);
	/*---------------------------------------------------------------*/
	~TraverseFolder(void);
private:
	string m_strDir;
};

