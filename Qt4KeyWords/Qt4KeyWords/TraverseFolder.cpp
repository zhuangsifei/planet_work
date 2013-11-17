#include "TraverseFolder.h"


TraverseFolder::TraverseFolder(void)
{
}
TraverseFolder::TraverseFolder(string strDirect)
{
	m_strDir = strDirect;
}

//the following is some functions to operate the file
//����һ���ļ�������һ���ļ����У����ļ����ֲ���
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

// ����һ���ļ����е��ı��ļ�����һ���ļ������е��ı��ļ�
bool TraverseFolder::CopyfolderTofolder(LPCSTR path1,LPCSTR path2)
{
	WIN32_FIND_DATA findData;
	HANDLE hSearch;
	char FilePathName[MAX_PATH];
	char FullPathName1[MAX_PATH];
	char FullPathName2[MAX_PATH];

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
//����һ���ļ�����һ���ļ���
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
//����һ���ļ��е�����һ���ļ���
bool TraverseFolder::CutfolderTofolder(LPCSTR path1,LPCSTR path2)
{
	WIN32_FIND_DATA findData;
	HANDLE hSearch;
	char FilePathName[MAX_PATH];
	char FullPathName1[MAX_PATH];
	char FullPathName2[MAX_PATH];
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
//ɾ��һ���ļ�
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
//ɾ��һ��Ŀ¼
bool TraverseFolder::DeleteFolder(LPCSTR path)
{
	WIN32_FIND_DATA findData;
	HANDLE hSearch;
	char FilePathName[MAX_PATH];
	char FullPathName[MAX_PATH];

	strcpy(FilePathName,path);
	strcat(FilePathName,"\\*.*");

	hSearch = FindFirstFile(FilePathName,&findData);
	if( hSearch == INVALID_HANDLE_VALUE)
	{
		//std::cout<<"cann't search file\n";
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
	::RemoveDirectory(path);
	return true;
}
//����һ��Ŀ¼
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
	}
	return false;
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
