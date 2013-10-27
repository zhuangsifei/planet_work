/*
文件: CalcualteNum.cpp,类CalculateNum的cpp文件
时间：2013年9月
作者：ZYS
版本：Version 1.0
功能：对输入的字典进行遍历，降低字典的维度
*/
#include "CalculateNum.h"

extern ofstream g_ftest;
extern const int g_FiltWords; //词汇过滤阈值
extern const int g_LeftWords; //特征向量维数
CalculateNum::CalculateNum(void)
{
}
CalculateNum::CalculateNum(int NumYes,int NumNo)
{
	m_NumYes = NumYes;
	m_NumNo = NumNo;
}


/******************** Now find a new way to realize this method********************************/


//---------------------------------------------------------------------------------
// 名字: CalculateNum::DiscardRepeat(LPCSTR path,set<string>* str)
// 功能: 去除文本中的重复词汇
//---------------------------------------------------------------------------------
void CalculateNum::DiscardRepeat(LPCSTR path,set<string> &str)
{
	ifstream fin(path);
	string strrp;
	while( fin.good() )
	{
		fin>>strrp;
		str.insert(strrp);
	}
	fin.close();
}


//---------------------------------------------------------------------------------
// 名字: CalculateNum::Init(LPCSTR path,ofstream& fout)
// 功能: 加载包含多有文本分词后的词汇
//---------------------------------------------------------------------------------
void CalculateNum::Init(LPCSTR path,ofstream &fout)
{
	ifstream fin(path); //待去重复的词典
	VT PerWord;		    //每个单词对应两个分类
	PerWord.push_back(ABCD(0,0,0,0));
	PerWord.push_back(ABCD(0,0,0,0));
	string str;
	while(fin.good())
	{
		fin>>str;
		m_MapDic.insert(pair<string,VT>(str,PerWord));
	}
	//The following  is just to show the Dictionary and it can be abandoned
	map<string,VT>::iterator pter;
	for(pter=m_MapDic.begin();pter!=m_MapDic.end();pter++)
	{
		fout<<(pter->first)<<endl;
	}

}


//---------------------------------------------------------------------------------
// 名字: CalculateNum::Calculate(LPCSTR path0,LPCSTR path1)
// 功能: 计算每个词汇的两种类别的Ai，Bi，Ci，Di
//---------------------------------------------------------------------------------
void CalculateNum::Calculate(LPCSTR path0,LPCSTR path1)
{
	WIN32_FIND_DATA findData;
	HANDLE hError;
	char FilePathName[MAX_PATH];
	char FullPathName[MAX_PATH];
	set<string> Setstr;
	set<string>::iterator iter;
	map<string,VT>::iterator pter;
	int NumFiles0 = 0;
	int NumFiles1 = 0;
	//遍历文件path0
	strcpy(FilePathName,path0);
	strcat(FilePathName,"\\*.*");
	hError = FindFirstFile(FilePathName,&findData);
	if(hError==INVALID_HANDLE_VALUE)
	{
		cout<<"Scan files failed\n";
		return;
	}
	while(::FindNextFile(hError,&findData))
	{
		if( strcmp(findData.cFileName,".")==0 || strcmp(findData.cFileName,"..")==0)
		{
			continue;
		}
		sprintf(FullPathName,"%s\\%s",path0,findData.cFileName);
		DiscardRepeat(FullPathName,Setstr);//去掉文本的重复词汇
		
		NumFiles0++;
		for(iter=Setstr.begin();iter!=Setstr.end();iter++)
		{
			pter = m_MapDic.find(*iter);
			(pter->second)[0].Ai++;
		}
		Setstr.clear();
	}
	//遍历文件path1
	strcpy(FilePathName,path1);
	strcat(FilePathName,"\\*.*");
	hError = FindFirstFile(FilePathName,&findData);
	if(hError == INVALID_HANDLE_VALUE)
	{
		cout<<"Scan Files Failed!\n";
		return;
	}
	while(::FindNextFile(hError,&findData))
	{
		if( strcmp(findData.cFileName,".")==0 || strcmp(findData.cFileName,"..")==0)
		{
			continue;
		}
		sprintf(FullPathName,"%s\\%s",path1,findData.cFileName);
		DiscardRepeat(FullPathName,Setstr);//去掉文本的重复词汇
		
		NumFiles1++;
		for(iter=Setstr.begin();iter!=Setstr.end();iter++)
		{
			pter = m_MapDic.find(*iter);
			(pter->second)[1].Ai++;
		}
		Setstr.clear();
	}
	//计算每个词汇的每个类别的Bi，Ci，Di
	for(pter=m_MapDic.begin();pter!=m_MapDic.end();pter++)
	{
		(pter->second)[0].Bi = (pter->second)[1].Ai;
		(pter->second)[0].Ci = NumFiles0 - (pter->second)[0].Ai;
		(pter->second)[0].Di = NumFiles1 - (pter->second)[0].Bi; 

		(pter->second)[1].Bi = (pter->second)[0].Ai;
		(pter->second)[1].Ci = NumFiles1 - (pter->second)[1].Ai;
		(pter->second)[1].Di = NumFiles0 - (pter->second)[1].Bi;
		// The following is only used for test
		g_ftest<<(pter->first)<<"  "<<(pter->second)[0].Ai<<"  "<<(pter->second)[0].Bi<<"  "
			<<(pter->second)[0].Ci<<"  "<<(pter->second)[0].Di<<"  "
			<<(pter->second)[1].Ai<<"  "<<(pter->second)[1].Bi<<"  "
			<<(pter->second)[1].Ci<<"  "<<(pter->second)[1].Di<<"  "<<endl;
	}
}


//---------------------------------------------------------------------------------
// 名字: CalculateNum::GetCHI(ABCD abcd)
// 功能: 计算每个词汇的CHI值
// 附注：获取每个词汇的CHI值,CHI值得计算公式为：
//		 CHI = Nall*(Ai*Di - Bi*Ci)*(Ai*Di - Bi*Ci)/[(Ai+Bi)(Ci+Di)(Ai+Ci)(Bi+Di)]
//		 其中Nall，Ai+Ci，Bi+Di 在分类为2时为定值，所以这里不去管常数.
//---------------------------------------------------------------------------------
double CalculateNum::GetCHI(ABCD abcd)
{
	double Ai = abcd.Ai;
	double Bi = abcd.Bi;
	double Ci = abcd.Ci;
	double Di = abcd.Di;
	double Nall = Ai + Bi + Ci + Di;
	double temp = 0.0f;
	//double Ni = Ai + Ci;
	//double Mi = Bi + Di;
	//double Kr = Ni/(6*Mi);
	////use Kr to fix the CHI algorithm 
	//temp = Nall*(Kr*Ai*Di-Bi*Ci)*(Kr*Ai*Di-Bi*Ci);
	temp = Nall*(Ai*Di-Bi*Ci)*(Ai*Di-Bi*Ci);
	temp/=(Ai + Bi);
	temp/=(Ci + Di);
	temp/=(Ai + Ci);
	temp/=(Bi + Di);
	return temp;
}


//---------------------------------------------------------------------------------
// 名字: CalculateNum::SortDecn(ofstream * fout)
// 功能: 对词汇的CHI值进行降序排列
//---------------------------------------------------------------------------------
void CalculateNum::SortDecn(ofstream * fout)
{
	vector<strChi> Vstr;
	map<string,VT>::iterator pter;
	//对于只有两个分类的情况，每个单词的两个类别的CHI相等
	//double tmp1 = 0;
	//double tmp2 = 0;
	double tmp  = 0;
	for(pter=m_MapDic.begin();pter!=m_MapDic.end();pter++)
	{
		//tmp1 = GetCHI( (pter->second)[0] );
		//tmp2 = GetCHI( (pter->second)[1] );
		//tmp=(tmp1>tmp2)?tmp1:tmp2;
		
		//对低频词汇过滤
		if ( (pter->second)[0].Ai + (pter->second)[0].Bi <g_FiltWords )
		{
				continue;
		}
		tmp = GetCHI( (pter->second)[0] );
		strChi strTmp(pter->first,tmp);
		Vstr.push_back(strTmp);
	}

	//sortCHI(Vstr,Vstr.size(),g_LeftWords);	//对CHI进行排序
	int nsize = Vstr.size();
	QuickSort(Vstr,0,nsize);	//改用快速排序法进行排序
	int nleft = nsize - g_LeftWords;
	if(nleft>0)
	{
		for(int i=0;i<nleft;i++)
			Vstr.pop_back();
		nsize = Vstr.size();
	}
	for(int k=0;k<nsize;k++)	//截短的特征存放在新的文件中
	{
		(*fout)<<Vstr[k].str<<"  "<<Vstr[k].dbChi<<endl;
		//(*fout)<<Vstr[k].str<<endl;
	}
	(*fout).close();
}


//---------------------------------------------------------------------------------
// 名字: CalculateNum::QuickSort(vector<strChi>& vec,int left,int right)
// 功能: 针对strChi结构体类型的元素的快速排序算法
//---------------------------------------------------------------------------------
void CalculateNum::QuickSort(vector<strChi>& vec,int left,int right)
{
	int i,j;
	strChi middle,temp;
	middle = vec[left];
	i = left + 1;
	j = right - 1;

	do{
		while(i<right && vec[i].dbChi>middle.dbChi)
			i++;
		while(j>left && vec[j].dbChi<middle.dbChi)
			j--;
		if( i>=j )
			break;
		temp = vec[i];
		vec[i] = vec[j];
		vec[j] = temp;
		i++;
		j--;
	}while(i<=j);

	vec[left] = vec[j];
	vec[j] = middle;
	if( left < j-1)
		QuickSort(vec,left,j);
	if( right > j+1)
		QuickSort(vec,j+1,right);
}
CalculateNum::~CalculateNum(void)
{
	m_MapDic.clear();
}
