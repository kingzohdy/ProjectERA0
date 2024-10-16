
#include "stdafx.h"
#include "ControlsHelper.h"

CString GetOpenFilePath(BOOL is_open, const char *init_reldir, const char *init_relpath, const char *ext, const char *pfilter, BOOL ret_relative_path)
{
	char curdir[1024] = {0};
	::GetCurrentDirectory(1024, curdir);

	char fullpath[256];
	sprintf(fullpath, "%s\\%s", curdir, init_relpath);
	strcpy(fullpath, init_relpath);

	char initdir[256];
	sprintf(initdir, "%s\\%s", curdir, init_reldir);

	CFileDialog dlg(is_open);
	dlg.GetOFN().lpstrInitialDir = initdir;
	dlg.GetOFN().lpstrDefExt = ext;
	dlg.GetOFN().lpstrFilter = pfilter;
	dlg.GetOFN().lpstrFile = fullpath;
	dlg.GetOFN().nMaxFile = 255;

	CString retstr("");
	if(IDOK == dlg.DoModal())
	{
		if(ret_relative_path)
		{
			char init_path2[256];
			strcpy(init_path2, curdir);
			strupr(init_path2);

			char fullpath2[256];
			strcpy(fullpath2, fullpath);
			strupr(fullpath2);

			const char *p = strstr(fullpath2, init_path2);
			if(p == NULL)
			{
				::AfxMessageBox("需要在初始目录下选择文件");
				retstr = "";
			}
			else retstr = fullpath + strlen(init_path2)+1;
		}
		else retstr = fullpath;
	}

	::SetCurrentDirectory(curdir);
	return retstr;
}
