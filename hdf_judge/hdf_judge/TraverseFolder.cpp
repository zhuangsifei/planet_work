/*
�ļ���TraverseFolder.cpp,��TraverseFolder��cpp�ļ�
ʱ�䣺2013��9��
���ߣ�ZYS
�汾��Verson 1.0
����: ʵ�ֶ��ļ����ļ��еĴ�����ɾ�������ƣ����в���
*/
#include "TraverseFolder.h"


TraverseFolder::TraverseFolder(void)
{
}
TraverseFolder::TraverseFolder(string strDirect)
{
	m_strDir = strDirect;
}


//---------------------------------------------------------------------------------
// ����: TraverseFolder::DirectList(LPCSTR path1,LPCSTR path2)
// ����: ��ȡһ���ļ����е������ı���д��һ���ļ����У��ʺ϶�ȡ����ļ���
//---------------------------------------------------------------------------------
bool TraverseFolder::DirectList(LPCSTR path,ofstream &pfout)
{
	WIN32_FIND_DATA FindData;
	HANDLE hError;
	int   nNumFiles=0;
	char FilePathName[MAX_PATH];
	char FullPathName[MAX_PATH];

	strcpy(FilePathName,path);
	strcat(FilePathName,"\\*.*");

	hError = FindFirstFile(FilePathName,&FindData);
	if (hError == INVALID_HANDLE_VALUE)
	{
		cout<<"Serach has Failed\n";
		return false;
	}

	while(::FindNextFile(hError,&FindData) )
	{
		if(strcmp(FindData.cFileName,".") ==0||strcmp(FindData.cFileName,"..") ==0)
		{
			continue;
		}
		wsprintf(FullPathName,"%s\\%s",path,FindData.cFileName);
		nNumFiles++;
		//cout<<nNumFiles<<":"<<FullPathName<<endl;

		ifstream fin(FullPathName);
		pfout<<fin.rdbuf()<<" ";//�ո������ӣ��Ա�֤�ֿ���ͬ���ĵ�
		fin.close();
		
		if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			cout<<"<Dir>";
			DirectList(FullPathName,pfout);
		}
	}
	::FindClose(hError);
	std::cout<<"All the "<<nNumFiles<<" file has been loaded in one txt\n";
	return true;
}


//---------------------------------------------------------------------------------
// ����: TraverseFolder::DirectList(LPCSTR path1,LPCSTR path2)
// ����: ��ȡһ���ļ����е������ı���д�뵽ͬһ���ı��У��ʺ϶�ȡһ���ļ���
//---------------------------------------------------------------------------------
bool TraverseFolder::DirectList(LPCSTR path1,LPCSTR path2)
{
	WIN32_FIND_DATA findData;
	HANDLE hError;
	ifstream fin;
	ofstream fout(path2);
	int nNumFiles = 0;

	char FilePathName[MAX_PATH] = "";
	char FullPathName[MAX_PATH] = "";
	strcpy(FilePathName,path1);
	strcat(FilePathName,"\\*.*");
	hError = FindFirstFile(FilePathName,&findData);
	if(hError == INVALID_HANDLE_VALUE)
	{
		std::cout<<"Can't find this file\n";
		return false;
	}

	while(::FindNextFile(hError,&findData) )
	{
		if(strcmp(findData.cFileName,".") ==0||strcmp(findData.cFileName,"..") ==0)
		{
			continue;
		}
		sprintf(FullPathName,"%s\\%s",path1,findData.cFileName);
		fin.open(FullPathName,ios::in);
		fout<<fin.rdbuf()<<endl;
		fin.close();
		fin.clear();
		nNumFiles++;
	}
	::FindClose(hError);
	fout.close();
	std::cout<<"All the "<<nNumFiles<<" file has been loaded in one txt\n";
	return true;
}



//---------------------------------------------------------------------------------
// ����: TraverseFolder::CutfileTofolder(LPCSTR path1,LPCSTR path2)
// ����: ��һ���ļ����Ƶ���һ���ļ�����
//---------------------------------------------------------------------------------
bool TraverseFolder::CopyfileTofolder(LPCSTR path1,LPCSTR path2)
{
	char FullPathName[MAX_PATH] = "";
	strcpy(FullPathName,path2);
	strcat(FullPathName,"\\");
	
	//get the file name of path1
	char LongPath[MAX_PATH];
	char *FileName = "";
	char **pFile = &FileName;
	GetFullPathName(path1,MAX_PATH,LongPath,pFile);
	
	//Get the full path and name in path2 folder
	strcat(FullPathName,FileName);
	CopyFile(path1,FullPathName,false);
	return true;
}


//---------------------------------------------------------------------------------
// ����: TraverseFolder::CopyfolderTofolder(LPCSTR path1,LPCSTR path2)
// ����: ��һ���ļ����е������ļ����Ƶ���һ���ļ�����
//---------------------------------------------------------------------------------
bool TraverseFolder::CopyfolderTofolder(LPCSTR path1,LPCSTR path2)
{
	WIN32_FIND_DATA findData;
	HANDLE hSearch;
	char FilePathName[MAX_PATH]  = "";
	char FullPathName1[MAX_PATH] = "";
	char FullPathName2[MAX_PATH] = "";

	strcpy(FilePathName,path1);
	strcat(FilePathName,"\\*.*");
	hSearch = FindFirstFile(FilePathName,&findData);
	if( hSearch == INVALID_HANDLE_VALUE)
	{
		std::cout<<"Serach files failed\n";
		return false;
	}

	while(::FindNextFileA(hSearch,&findData))
	{
		if(strcmp(findData.cFileName,".") ==0||strcmp(findData.cFileName,"..") ==0)
		{
			continue;
		}
		sprintf(FullPathName1,"%s\\%s",path1,findData.cFileName);
		sprintf(FullPathName2,"%s\\%s",path2,findData.cFileName);

		if( !CopyFile(FullPathName1,FullPathName2,false))
			std::cout<<"Copy file"<<findData.cFileName<<"failed\n";
	}
	::FindClose(hSearch);
	return true;
}


//---------------------------------------------------------------------------------
// ����: TraverseFolder::CutfileTofolder(LPCSTR path1,LPCSTR path2)
// ����: ��һ���ļ����е���һ���ļ�����
//---------------------------------------------------------------------------------
bool TraverseFolder::CutfileTofolder(LPCSTR path1,LPCSTR path2)
{
	char FullPathName[MAX_PATH] = "";
	strcpy(FullPathName,path2);
	strcat(FullPathName,"\\");
	
	//get the file name of path1
	char LongPath[MAX_PATH];
	char *FileName = "";
	char **pFile = &FileName;
	GetFullPathName(path1,MAX_PATH,LongPath,pFile);

	//Move the file
	strcat(FullPathName,FileName);
	if( !MoveFileEx(path1,FullPathName,MOVEFILE_REPLACE_EXISTING) )
	{
			std::cout<<"Move file"<<FileName<<"failed\n";
			return false;
	}
	else
	{
		return true;
	}
	
}


//---------------------------------------------------------------------------------
// ����: TraverseFolder::CutfolderTofolder(LPCSTR path1,LPCSTR path2)
// ����: ��һ���ļ����е������ļ����е���һ���ļ�����
//---------------------------------------------------------------------------------
bool TraverseFolder::CutfolderTofolder(LPCSTR path1,LPCSTR path2)
{
	WIN32_FIND_DATA findData;
	HANDLE hSearch;
	char FilePathName[MAX_PATH]  = "";
	char FullPathName1[MAX_PATH] = "";
	char FullPathName2[MAX_PATH] = "";
	strcpy(FilePathName,path1);
	strcat(FilePathName,"\\*.*");

	hSearch = FindFirstFile(FilePathName,&findData);
	if( hSearch ==INVALID_HANDLE_VALUE)
	{
		std::cout<<"Search files failed\n";
		return false;
	}
	while(::FindNextFile(hSearch,&findData))
	{
		if(strcmp(findData.cFileName,".") ==0||strcmp(findData.cFileName,"..") ==0)
		{
			continue;
		}
		sprintf(FullPathName1,"%s\\%s",path1,findData.cFileName);
		sprintf(FullPathName2,"%s\\%s",path2,findData.cFileName);

		if( !MoveFileEx(FullPathName1,FullPathName2,MOVEFILE_REPLACE_EXISTING))
		{
			std::cout<<"Move file"<<findData.cFileName<<"failed\n";
		}
	}
	::FindClose(hSearch);
	return true;
}


//---------------------------------------------------------------------------------
// ����: TraverseFolder::DeleteFileone(LPCSTR path)
// ����: ɾ��һ���ļ�
//---------------------------------------------------------------------------------
bool TraverseFolder::DeleteFileOne(LPCSTR path)
{
	if( DeleteFile(path) )
	{
		return true;
	}
	else
	{
		return false;
	}
}


//---------------------------------------------------------------------------------
// ����: TraverseFolder::DeleteFolder(LPCSTR path)
// ����: ɾ��һ���ļ�Ŀ¼
//---------------------------------------------------------------------------------
bool TraverseFolder::DeleteFolder(LPCSTR path)
{
	WIN32_FIND_DATA findData;
	HANDLE hSearch;
	char FilePathName[MAX_PATH] = "";
	char FullPathName[MAX_PATH] = "";

	strcpy(FilePathName,path);
	strcat(FilePathName,"\\*.*");

	hSearch = FindFirstFile(FilePathName,&findData);
	if( hSearch == INVALID_HANDLE_VALUE)
	{
		std::cout<<"cann't search file\n";
		return false;
	}
	while(::FindNextFile(hSearch,&findData))
	{
		if( strcmp(findData.cFileName,".")==0 || strcmp(findData.cFileName,"..")==0)
		{
			continue;
		}
		sprintf(FullPathName,"%s//%s",path,findData.cFileName);
		
		if( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			DeleteFolder(FullPathName);
			RemoveDirectory(FullPathName);
		}
		DeleteFile(FullPathName);
	}
	::FindClose(hSearch);
	RemoveDirectory(path);
	return true;
}


//---------------------------------------------------------------------------------
// ����: TraverseFolder::CreateFolder(LPCSTR path)
// ����: ����һ���ļ�Ŀ¼
//---------------------------------------------------------------------------------
bool TraverseFolder::CreateFolder(LPCSTR path)
{
	if( CreateDirectory(path,NULL) )
		return true;
	else 
	{
		if(GetLastError() == ERROR_ALREADY_EXISTS)
			std::cout<<"the directory is already existing\n";
		if(GetLastError() == ERROR_PATH_NOT_FOUND)
			std::cout<<"one or more directory has not been found\n";
		return false;
	}
	return true;
}


//---------------------------------------------------------------------------------
// ����: TraverseFolder::ClearFolder(LPCSTR path)
// ����: ���һ���ļ����е������ļ�
//---------------------------------------------------------------------------------
bool TraverseFolder::ClearFolder(LPCSTR path)
{
	WIN32_FIND_DATA findData;
	HANDLE hError = NULL;
	char FilePathName[MAX_PATH] = "";
	char FullPathName[MAX_PATH] = "";

	strcpy(FilePathName,path);
	strcat(FilePathName,"\\*.*");

	hError = FindFirstFile(FilePathName,&findData);
	if( hError == INVALID_HANDLE_VALUE )
	{
		std::cout<<"Find file failed"<<std::endl;
		return false;
	}

	while( ::FindNextFile(hError,&findData))
	{
		if( strcmp(findData.cFileName,".")==0 || strcmp(findData.cFileName,"..")==0)
			continue;
		sprintf(FullPathName,"%s\\%s",path,findData.cFileName);
		
		if( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			ClearFolder(FullPathName);
			RemoveDirectory(FullPathName);
		}
		else
		{
			DeleteFile(FullPathName);
		}
	}
	::FindClose(hError);
	return true;
	
}
TraverseFolder::~TraverseFolder(void)
{
}
