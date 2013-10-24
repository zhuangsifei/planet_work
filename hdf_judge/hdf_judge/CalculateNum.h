/*
文件:CalcualteNum.h,类CalculateNum头文件
时间：2013年9月
作者：ZYS
版本：Version 1.0
功能：对输入的字典进行遍历，降低字典的维度
*/
#pragma once
#include<Windows.h>
#include<string>
#include<iostream>
#include<fstream>
#include<vector>
#include<set>
#include<map>
using namespace std;

struct Times		//存放Ai，Bi值得结构体
{
	int Ai;
	int Bi;
	Times()
	{
		Ai = 0;
		Bi = 0;
	}
};
struct strChi		//存放字符串以及互信息的结构体
{
	string str;
	double dbChi;
	strChi(string _str,double _dbChi)
	{
		str = _str;
		dbChi = _dbChi;
	}
	strChi()
	{
		str = " ";
		dbChi = 0.0f;
	}
};

struct ABCD
{
	int Ai;
	int Bi;
	int Ci;
	int Di;
	ABCD()
	{
		Ai=Bi=Ci=Di=0;
	}
	ABCD(int a,int b,int c,int d)
	{
		Ai=a;
		Bi=b;
		Ci=c;
		Di=d;
	}
};

class CalculateNum
{
public:
	typedef vector<ABCD> VT;
	CalculateNum(void);
	CalculateNum(int NumYes ,int NumNo);
	int Traverse(LPCSTR path,string word);
	int Traverse(LPCSTR path);
	Times Calculate(string word,LPCSTR path0,LPCSTR path1);
	void SortDecn(string path,ofstream* fout,LPCSTR path0,LPCSTR path1);
	double CHIGet(int ai,int bi,int ci,int di,int nall);
	void sortCHI(vector<strChi>& vStr,int num,int nFilt);
	// Now find a new way to solve this problem
	void DiscardRepeat(LPCSTR path,set<string> &str);
	void Calculate(LPCSTR path0,LPCSTR path1);
	void Init(LPCSTR path,ofstream& fout);
	void SortDecn(ofstream* fout);
	double GetCHI(ABCD abcd);
	void QuickSort(vector<strChi>& vec,int left,int right);
	~CalculateNum(void);
private:
	int m_NumYes;
	int m_NumNo;
	map<string,VT> m_MapDic;
};

