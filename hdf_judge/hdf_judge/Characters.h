/*
文件：Charaters.cpp,类Character的cpp文件
时间：2013年9月
作者：ZYS
版本：Version 1.0
功能：提取每个样本的特征向量，以供libsvm训练
*/
#pragma once
#include<string>
#include<Windows.h>
#include<iostream>
#include<fstream>
#include<sstream>
#include<map>
#include<set>
using namespace std;

class Characters
{
public:
	Characters(void);
	Characters(LPCSTR path);
	void CharY();
	void CharZ(LPCSTR trainPath,ofstream* pfstr,int label);
	void SortFileWords(LPCSTR path,set<string>& strSet);
	void KindofFile(LPCSTR path1,LPCSTR path2,LPCSTR pathRecord);
	void KindtoMap(LPCSTR pathRecord,LPCSTR pathResult,map<string,int,bool (*)(string,string)>& KindMap);
	static bool LessComp(string str1,string str2);
	~Characters(void);
private:
	map<string,int> m_DicMap;
};

