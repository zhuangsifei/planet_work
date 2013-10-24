/*
文件：TraverseFolder.cpp,类TraverseFolder的cpp文件
时间：2013年9月
作者：ZYS
版本：Verson 1.0
功能: 实现对文件，文件夹的创建，删除，复制，剪切操作
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
// 名字: TraverseFolder::DirectList(LPCSTR path1,LPCSTR path2)
// 功能: 读取一个文件夹中的所有文本并写入一个文件流中，适合读取多个文件夹
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
		pfout<<fin.rdbuf()<<" ";//空格必须添加，以保证分开不同的文档
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
// 名字: TraverseFolder::DirectList(LPCSTR path1,LPCSTR path2)
// 功能: 读取一个文件夹中的所有文本并写入到同一个文本中，适合读取一个文件夹
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
// 名字: TraverseFolder::CutfileTofolder(LPCSTR path1,LPCSTR path2)
// 功能: 把一个文件复制到另一个文件夹中
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
// 名字: TraverseFolder::CopyfolderTofolder(LPCSTR path1,LPCSTR path2)
// 功能: 把一个文件夹中的所有文件复制到另一个文件夹中
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
// 名字: TraverseFolder::CutfileTofolder(LPCSTR path1,LPCSTR path2)
// 功能: 把一个文件剪切到另一个文件夹中
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
// 名字: TraverseFolder::CutfolderTofolder(LPCSTR path1,LPCSTR path2)
// 功能: 把一个文件夹中的所有文件剪切到另一个文件夹中
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
// 名字: TraverseFolder::DeleteFileone(LPCSTR path)
// 功能: 删除一个文件
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
// 名字: TraverseFolder::DeleteFolder(LPCSTR path)
// 功能: 删除一个文件目录
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
// 名字: TraverseFolder::CreateFolder(LPCSTR path)
// 功能: 创建一个文件目录
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
// 名字: TraverseFolder::ClearFolder(LPCSTR path)
// 功能: 清空一个文件夹中的所有文件
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
