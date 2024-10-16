
#include "OgrePackageFile.h"
#include "OgreStringUtil.h"
#include <string>
#include <Windows.h>

using namespace std;
using namespace Ogre;

void AddFilesToPackage(Ogre::FilePackage *pkg, const char *dir, bool do_compress)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	char searchname[256];
	sprintf(searchname, "%s\\*.*", dir);
	hFind = FindFirstFile(searchname, &FindFileData);

	while(FindNextFile(hFind, &FindFileData))
	{
		if((FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) == 0)
		{
			char fullpath[256];
			sprintf(fullpath, "%s\\%s", dir, FindFileData.cFileName);
			strlwr(fullpath);
			printf("packfile %s \n",fullpath);
			pkg->appendOrReplaceFile(fullpath, do_compress);
		}
		else
		{
			if(strcmp(FindFileData.cFileName, ".")==0 || strcmp(FindFileData.cFileName, "..")==0)
			{
				continue;
			}

			char childdir[256];
			sprintf(childdir, "%s\\%s", dir, FindFileData.cFileName);
			AddFilesToPackage(pkg, childdir, do_compress);
		}
	}

	FindClose(hFind);
}

//PackFile 目标包文件 子目录1  子目录2  子目录3
//PackFile a.pkf maps scene uires
int main(int argc, const char *argv[])
{
	if(argc < 3)
	{
		printf("格式或者参数不正确：PackFile 目标包文件 子目录1  子目录2  子目录3\n");
		return -1;
	}

	StringUtil::init();

	string szPath = argv[1];
	string szDirectory = "";
	while( szPath.find('\\')!=szPath.npos )
	{
		szDirectory += szPath.substr(0,szPath.find('\\') + 1);
		CreateDirectory(szDirectory.c_str(),NULL );
		szPath = szPath.substr( szPath.find('\\')+1 );
	}

	FilePackage pkg;
	pkg.open(argv[1], false);

	for(int i=2; i<argc; i++)
	{
		AddFilesToPackage(&pkg, argv[i], true);
	}

	pkg.close();
	return 0;
}