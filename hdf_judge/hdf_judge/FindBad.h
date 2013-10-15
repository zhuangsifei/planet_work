/*
文件：FindBad.h ，类FindBad的头文件
时间：2013年10月
作者：ZYS
版本：Version：1.0
功能：找出每个文本中各种词汇的数目来做分析
*/
#pragma once
#include <Windows.h>
#include <string>
#include <set>
#include <map>
using namespace std;

class FindBad
{
public:
	FindBad(void);
	FindBad(int mt,int mydg,int mydb,int mysg,int mysb);
	void InitSet(LPCSTR pSource,set<string> &mSet);
	void ClearNum();
	int GetAllWordsNum()const;
	int GetThanksNum()const;
	int GetYideGoodNum()const;
	int GetYideBadNum()const;
	int GetYishuGoodNum()const;
	int GetYishuBadNum()const;
public:
	void BatchSeparateWord(LPCSTR pSource,LPCSTR pDest);
	void BatchAmendATexts();
	//void Bat
public:
	void SingleSeparateWord(LPCSTR pSource,LPCSTR pDest);
	void SingleComputeWord(LPCSTR pSource);
	void SingleAmendText(LPCSTR pSource,set<string> &mSet);
	~FindBad(void);
private:
	set<string> m_AllWords;
	set<string> m_Thanks;
	set<string> m_YideGood;
	set<string> m_YideBad;
	set<string> m_YishuGood;
	set<string> m_YishuBad;
	int m_NumAllWords;
	int m_NumThanks;
	int m_NumYideGood;
	int m_NumYideBad;
	int m_NumYishuGood;
	int m_NumYishuBad;
	
};

