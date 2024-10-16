
#pragma once

extern CString GetOpenFilePath(BOOL is_open, const char *init_reldir, const char *init_relpath, const char *ext, const char *pfilter, BOOL ret_relative_path);
extern CString GetRelativePathToCurDir(CString path);
extern BOOL RegDelnode(HKEY hKeyRoot, LPTSTR lpSubKey);