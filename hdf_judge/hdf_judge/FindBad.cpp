/*
文件：FindBad.cpp ，类FindBad的实现文件
时间：2013年10月
作者：ZYS
版本：Version：1.0
功能：找出每个文本中各种词汇的数目来做分析
*/
#include "FindBad.h"
#include <fstream>
using namespace std;

extern const char *g_pAllWords;
extern const char *g_pThanks;
extern const char *g_pYideGood;
extern const char *g_pYideBad;
extern const char *g_pYishuGood;
extern const char *g_pYishuBad;

FindBad::FindBad(void)
{
	m_NumAllWords	= 0;
	m_NumThanks		= 0;
	m_NumYideGood	= 0;
	m_NumYideBad	= 0;
	m_NumYishuGood	= 0;
	m_NumYishuBad	= 0;
	
	InitSet(g_pAllWords,m_AllWords);
	InitSet(g_pThanks,m_Thanks);
	InitSet(g_pYideGood,m_YideGood);
	InitSet(g_pYideBad,m_YideBad);
	InitSet(g_pYishuGood,m_YishuGood);
	InitSet(g_pYishuBad,m_YishuBad);
}

void FindBad::ClearNum()
{
	m_NumAllWords	= 0;
	m_NumThanks		= 0;
	m_NumYideGood	= 0;
	m_NumYideBad	= 0;
	m_NumYishuGood	= 0;
	m_NumYishuBad	= 0;
}

void FindBad::InitSet(LPCSTR pSource,set<string> &mSet)
{
	ifstream fin(pSource,ios::in);
	string str;
	while(fin.good())
	{
		fin>>str;
		mSet.insert(str);
	}
	fin.close();
}

int FindBad::GetAllWordsNum()const
{
	return m_NumAllWords;
}

int FindBad::GetThanksNum()const
{
	return m_NumThanks;
}

int FindBad::GetYideBadNum()const
{
	return m_NumYideBad;
}

int FindBad::GetYideGoodNum()const
{
	return m_NumYideGood;
}

int FindBad::GetYishuBadNum()const
{
	return m_NumYishuBad;
}
int FindBad::GetYishuGoodNum()const
{
	return m_NumYishuGood;
}

// compute the the number of words belong to every kind 
void FindBad::SingleComputeWord(LPCSTR pSource)
{
	ifstream fin(pSource,ios::in);
	string str;
	while(fin.good())
	{
		fin>>str;
		if(m_Thanks.find(str) != m_Thanks.end())
			++m_NumThanks;

		if(m_YideGood.find(str) != m_YideGood.end())
			++m_NumYideGood;
		else if(m_YideBad.find(str) != m_YideBad.end() )     
			++m_NumYideBad;

		if(m_YishuGood.find(str) != m_YishuGood.end())
			++m_NumYishuGood;
		else if(m_YishuBad.find(str) != m_YishuBad.end())
			++m_NumYishuBad;
	}
	fin.close();
}
// amend all texts which has words for doctors to ensure every word is distinctive
void FindBad::BatchAmendATexts()
{
	SingleAmendText(g_pAllWords,m_AllWords);
	SingleAmendText(g_pThanks,m_Thanks);
	SingleAmendText(g_pYideGood,m_YideGood);
	SingleAmendText(g_pYideBad,m_YideBad);
	SingleAmendText(g_pYishuGood,m_YishuGood);
	SingleAmendText(g_pYishuBad,m_YishuBad);
}
// amend the text  which has words for doctors to ensure every word is distinctive
void FindBad::SingleAmendText(LPCSTR pSource,set<string> &mSet)
{
	ofstream fout(pSource,ios::out);
	set<string>::iterator iter;
	for (iter = mSet.begin();iter!=mSet.end();++iter)
	{
		fout<<*iter<<endl;
	}
	fout.close();
}

FindBad::~FindBad(void)
{
}
