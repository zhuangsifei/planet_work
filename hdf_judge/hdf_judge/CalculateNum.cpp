/*
�ļ�: CalcualteNum.cpp,��CalculateNum��cpp�ļ�
ʱ�䣺2013��9��
���ߣ�ZYS
�汾��Version 1.0
���ܣ���������ֵ���б����������ֵ��ά��
*/
#include "CalculateNum.h"

extern ofstream g_ftest;
extern const int g_FiltWords; //�ʻ������ֵ
extern const int g_LeftWords; //��������ά��
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
// ����: CalculateNum::DiscardRepeat(LPCSTR path,set<string>* str)
// ����: ȥ���ı��е��ظ��ʻ�
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
// ����: CalculateNum::Init(LPCSTR path,ofstream& fout)
// ����: ���ذ��������ı��ִʺ�Ĵʻ�
//---------------------------------------------------------------------------------
void CalculateNum::Init(LPCSTR path,ofstream &fout)
{
	ifstream fin(path); //��ȥ�ظ��Ĵʵ�
	VT PerWord;		    //ÿ�����ʶ�Ӧ��������
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
// ����: CalculateNum::Calculate(LPCSTR path0,LPCSTR path1)
// ����: ����ÿ���ʻ����������Ai��Bi��Ci��Di
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
	//�����ļ�path0
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
		DiscardRepeat(FullPathName,Setstr);//ȥ���ı����ظ��ʻ�
		
		NumFiles0++;
		for(iter=Setstr.begin();iter!=Setstr.end();iter++)
		{
			pter = m_MapDic.find(*iter);
			(pter->second)[0].Ai++;
		}
		Setstr.clear();
	}
	//�����ļ�path1
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
		DiscardRepeat(FullPathName,Setstr);//ȥ���ı����ظ��ʻ�
		
		NumFiles1++;
		for(iter=Setstr.begin();iter!=Setstr.end();iter++)
		{
			pter = m_MapDic.find(*iter);
			(pter->second)[1].Ai++;
		}
		Setstr.clear();
	}
	//����ÿ���ʻ��ÿ������Bi��Ci��Di
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
// ����: CalculateNum::GetCHI(ABCD abcd)
// ����: ����ÿ���ʻ��CHIֵ
// ��ע����ȡÿ���ʻ��CHIֵ,CHIֵ�ü��㹫ʽΪ��
//		 CHI = Nall*(Ai*Di - Bi*Ci)*(Ai*Di - Bi*Ci)/[(Ai+Bi)(Ci+Di)(Ai+Ci)(Bi+Di)]
//		 ����Nall��Ai+Ci��Bi+Di �ڷ���Ϊ2ʱΪ��ֵ���������ﲻȥ�ܳ���.
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
// ����: CalculateNum::SortDecn(ofstream * fout)
// ����: �Դʻ��CHIֵ���н�������
//---------------------------------------------------------------------------------
void CalculateNum::SortDecn(ofstream * fout)
{
	vector<strChi> Vstr;
	map<string,VT>::iterator pter;
	//����ֻ����������������ÿ�����ʵ���������CHI���
	//double tmp1 = 0;
	//double tmp2 = 0;
	double tmp  = 0;
	for(pter=m_MapDic.begin();pter!=m_MapDic.end();pter++)
	{
		//tmp1 = GetCHI( (pter->second)[0] );
		//tmp2 = GetCHI( (pter->second)[1] );
		//tmp=(tmp1>tmp2)?tmp1:tmp2;
		
		//�Ե�Ƶ�ʻ����
		if ( (pter->second)[0].Ai + (pter->second)[0].Bi <g_FiltWords )
		{
				continue;
		}
		tmp = GetCHI( (pter->second)[0] );
		strChi strTmp(pter->first,tmp);
		Vstr.push_back(strTmp);
	}

	//sortCHI(Vstr,Vstr.size(),g_LeftWords);	//��CHI��������
	int nsize = Vstr.size();
	QuickSort(Vstr,0,nsize);	//���ÿ������򷨽�������
	int nleft = nsize - g_LeftWords;
	if(nleft>0)
	{
		for(int i=0;i<nleft;i++)
			Vstr.pop_back();
		nsize = Vstr.size();
	}
	for(int k=0;k<nsize;k++)	//�ض̵�����������µ��ļ���
	{
		(*fout)<<Vstr[k].str<<"  "<<Vstr[k].dbChi<<endl;
		//(*fout)<<Vstr[k].str<<endl;
	}
	(*fout).close();
}


//---------------------------------------------------------------------------------
// ����: CalculateNum::QuickSort(vector<strChi>& vec,int left,int right)
// ����: ���strChi�ṹ�����͵�Ԫ�صĿ��������㷨
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
