
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

CString GetRelativePathToCurDir(CString path)
{
	char tmp[256] = {0};
	if(0 == GetCurrentDirectory(256, tmp))
		return "";

	CString currdir = tmp;
	currdir.Append("\\");

	int currdirlen = currdir.GetLength();
	int pathlen = path.GetLength();
	if(pathlen + 12 < currdirlen)
		return "";

	CString path2 = path;
	path.MakeLower();
	currdir.MakeLower();

	path.Replace('/', '\\');
	CString left = path.Left(currdirlen);

	if(left != currdir)
		return "";

	CString right = path.Right(pathlen - currdirlen); 
	return right;
}

BOOL RegDelnodeRecurse (HKEY hKeyRoot, LPTSTR lpSubKey)
{
	LPTSTR lpEnd;
	LONG lResult;
	DWORD dwSize;
	TCHAR szName[MAX_PATH];
	HKEY hKey;
	FILETIME ftWrite;

	// First, see if we can delete the key without having
	// to recurse.

	lResult = RegDeleteKey(hKeyRoot, lpSubKey);

	if (lResult == ERROR_SUCCESS) 
		return TRUE;

	lResult = RegOpenKeyEx (hKeyRoot, lpSubKey, 0, KEY_READ, &hKey);

	if (lResult != ERROR_SUCCESS) 
	{
		if (lResult == ERROR_FILE_NOT_FOUND) {
			printf("Key not found.\n");
			return TRUE;
		} 
		else {
			printf("Error opening key.\n");
			return FALSE;
		}
	}

	// Check for an ending slash and add one if it is missing.

	lpEnd = lpSubKey + lstrlen(lpSubKey);

	if (*(lpEnd - 1) != TEXT('\\')) 
	{
		*lpEnd =  TEXT('\\');
		lpEnd++;
		*lpEnd =  TEXT('\0');
	}

	// Enumerate the keys

	dwSize = MAX_PATH;
	lResult = RegEnumKeyEx(hKey, 0, szName, &dwSize, NULL,
		NULL, NULL, &ftWrite);

	if (lResult == ERROR_SUCCESS) 
	{
		do {

			lstrcpy (lpEnd, szName);

			if (!RegDelnodeRecurse(hKeyRoot, lpSubKey)) {
				break;
			}

			dwSize = MAX_PATH;

			lResult = RegEnumKeyEx(hKey, 0, szName, &dwSize, NULL,
				NULL, NULL, &ftWrite);

		} while (lResult == ERROR_SUCCESS);
	}

	lpEnd--;
	*lpEnd = TEXT('\0');

	RegCloseKey (hKey);

	// Try again to delete the key.

	lResult = RegDeleteKey(hKeyRoot, lpSubKey);

	if (lResult == ERROR_SUCCESS) 
		return TRUE;

	return FALSE;
}

BOOL RegDelnode(HKEY hKeyRoot, LPTSTR lpSubKey)
{
	TCHAR szDelKey[2 * MAX_PATH];

	lstrcpy (szDelKey, lpSubKey);
	return RegDelnodeRecurse(hKeyRoot, szDelKey);

}
