#pragma once
#include<iostream>
#include<string>
#include<fstream>
#include<map>
#include<set>
#include<sstream>
#include<windows.h>
#include"API\ICTCLAS50.h"
#pragma comment(lib,".\\API\\ICTCLAS50.lib")

using namespace std;
class FindKey
{
public:
	enum DealMode{BatchMode,SingleMode,LoadMode};
	enum MapMode{ShowWeight,UnshowWeight,AllDone};
	FindKey(void);
	FindKey(char *p);
	FindKey(const FindKey& copy);
	DealMode GetDealMode();
	void SetDealMode(DealMode nMode);
	MapMode GetMapMode();
	void SetMapMode(MapMode nMode);
public:
	void LoadDic();
	void SeparateWords(LPCSTR path1,LPCSTR path2);
	void ClearWords(LPCSTR path,map<string,int>& MapStr);
	bool CheckString(string str);
	bool CompareWords(const set<string>& SetStr, map<string,int>& MapStr);
	bool CompareWords(const map<string,int>& MapDic,map<string,int>& MapStr );
	void GetBatchResult(LPCSTR path,ofstream *pfout);
	void GetBatchResult(LPCSTR pSeparate,LPCSTR pGoal);
public:
	void FixDic(LPCSTR path1,LPCSTR path2);
	void MapLoad(LPCSTR path);
	void SingleSeparateWords(LPCSTR path1,LPCSTR path2);
	void AddWords(LPCSTR path,LPCSTR path2,LPCSTR path3);//use to show the weight of words but is slow
	void AddWords(LPCSTR path,LPCSTR dicPath);			 //not show words weight but is fast
	void TestSet(string str);
	int  SingleResult1(map<string,int> MapStr);
	int  SingleResult2(map<string,int> Mapstr);
	~FindKey(void);
private:
	char *m_DicPath;
	set<string> m_Set;
	map<string,int> m_Map;
	DealMode m_Mode;
	MapMode m_WeightShow;

};

