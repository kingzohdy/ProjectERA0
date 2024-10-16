// script_comp_all.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <windows.h>
#include <stdio.h>
#include <direct.h>



int main(int argc, char *argv[])
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	char szPath[] = ".\\*.*";
	char szCmd[256];
	int iRet;
	
	hFind = FindFirstFile(szPath, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) 
	{
		printf ("Invalid File Handle. GetLastError reports %d\n", 
			GetLastError ());
		return (0);
	} 
	else 
	{
		do 
		{
			if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
			{
				continue;
			}

			if (strstr(FindFileData.cFileName,"mxt") != NULL)
			{
				printf("%s\n",FindFileData.cFileName);

				sprintf(szCmd,"%s %s", "compiler.exe",FindFileData.cFileName);
				//WinExec(szCmd, 0);
				iRet = system(szCmd);
				if (iRet != 0)
				{
					return -1;
				}
			
			}

		} while (FindNextFile(hFind, &FindFileData));

		mkdir(".\\mac");
		mkdir(".\\lst");
		sprintf(szCmd, "move *.mac .\\mac\\");
		system(szCmd);
		sprintf(szCmd, "move *.lst .\\lst\\");
		system(szCmd);

		FindClose(hFind);
		return (1);
	}
}

