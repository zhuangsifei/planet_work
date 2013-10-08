/*
文件: SeparateTerm.h 类SeparateTerm的头文件
时间：2013年9月
作者：ZYS
版本：Version 1.0
功能：对文本实现分词
*/
#pragma once
#include<iostream>
#include<string>
#include<fstream>
#include<windows.h>
#include"API\ICTCLAS50.h"
#pragma comment(lib ,".\\API\\ICTCLAS50.lib")
using namespace std;

class SeparateTerm
{
public:
	enum SaveMode{Load,UnLoad};
	enum SvmMode{Train,Predict};
	SeparateTerm(void);
	SeparateTerm(char* path);
	SeparateTerm(const SeparateTerm& copy);
	void SepInit();
	void LoadDic();
	void SepSave(LPCSTR path0,LPCSTR path1);
	void SetDicMode(SaveMode mode);
	SaveMode GetDicMode();
	void SetSvmMode(SvmMode mode);
	SvmMode GetSvmMode();
	~SeparateTerm(void);
private:
	char *m_DicPath;
	SaveMode m_Save;
	SvmMode m_Svm;
};

