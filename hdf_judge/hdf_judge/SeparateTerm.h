/*
�ļ�: SeparateTerm.h ��SeparateTerm��ͷ�ļ�
ʱ�䣺2013��9��
���ߣ�ZYS
�汾��Version 1.0
���ܣ����ı�ʵ�ִַ�
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
	SeparateTerm(LPCSTR pDicPath,bool bLoad);
	SeparateTerm::SeparateTerm(const SeparateTerm& copy);
	~SeparateTerm(void);
	void SepInit();
	void LoadDic();
	void SetLoad(bool bLoad);
	bool GetLoad();
public:
	void SingleSeparate(LPCSTR pSource,LPCSTR pDest);
	void SingleClearWord(LPCSTR pSource,LPCSTR pDest);
	void BatchSeparate(LPCSTR pSource,LPCSTR pDest);
	void BatchClearWord(LPCSTR pSource,LPCSTR pDest);
	
	void SepSave(LPCSTR path0,LPCSTR path1);
	
private:
	const char* m_pDicPath;
	bool		m_bLoad;

	
	
};

