#include "resource.h"
#include "profile.h"
#include "win32utils.h"
#include "mysql/mysql.h"
#include <shlobj.h>
#include <commctrl.h>
#include <vector>


INT_PTR CALLBACK MakingDlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK RestoreDlgProc(HWND, UINT, WPARAM, LPARAM);

namespace {

int InitUI(HWND hwnd)
{
	HIMAGELIST il = ImageList_Create(16, 16, ILC_MASK | ILC_COLOR32, 0, 0);
	if(!il)
		return -1;

	HICON icon = LoadIcon(::grav::w32::kCurrentInstance, MAKEINTRESOURCE(IDI_TABLE));
	ImageList_AddIcon(il, icon);

	HWND lv = GetDlgItem(hwnd, IDC_TABLES);
	ListView_SetImageList(lv, il, LVSIL_SMALL);

	std::string caption;
	caption.resize(GetWindowTextLength(hwnd));
	GetWindowText(hwnd, &caption[0], caption.length() + 1);
	caption += " 数据库: " + CONF_EXPORT(db_name) + "; 用户: " +
	           CONF_EXPORT(db_user);
	SetWindowText(hwnd, caption.c_str());
	return 0;
}

int InsertTableName(HWND hwnd, HWND lv, const char *name)
{
	UNUSED(hwnd);

	LVITEM lvi;
	memset(&lvi, 0, sizeof(lvi));

	int n = ListView_GetItemCount(lv);
	lvi.mask   = LVIF_TEXT | LVIF_IMAGE;
	lvi.iImage = 0;
	lvi.iItem  = n;
	lvi.pszText = const_cast<char*>(name);
	
	return ListView_InsertItem(lv, &lvi);
}

int FetchTableName(HWND hwnd)
{
	MYSQL *conn = mysql_init(NULL);
	if(!conn)
		goto error;

	if(!mysql_real_connect(
		conn,
		CONF_EXPORT_Z(db_connect),
		CONF_EXPORT_Z(db_user),
		CONF_EXPORT_Z(db_passwd),
		CONF_EXPORT_Z(db_name),
		0,
		NULL,
		CLIENT_COMPRESS))
		goto error;

	int err = mysql_query(conn, "SHOW TABLES");
	if(err)
		goto error;

	MYSQL_RES *res = mysql_store_result(conn);
	if(!res)
		goto error;

	MYSQL_ROW row = NULL;
	HWND lv = GetDlgItem(hwnd, IDC_TABLES);
	while((row = mysql_fetch_row(res)) != NULL)
	{
		if(!row[0])
			continue;
		InsertTableName(hwnd, lv, row[0]);
	}

out:
	if(conn)
		mysql_close(conn);
	mysql_library_end();
	return 0;

error:
	const char *msg = mysql_error(conn);
	MSG_BOX(FATAL, hwnd)
	<< "MySQL数据库错误: " << (msg ? msg : "NULL");
	goto out;
}

int OnMake(HWND hwnd, int exported)
{
	std::vector<std::string> selected_tables;

	HWND lv = GetDlgItem(hwnd, IDC_TABLES);
	int index = ListView_GetNextItem(lv, -1, LVNI_SELECTED);
	if(index < 0)
	{
		MSG_BOX(INFO, hwnd) << "你没有选择任何表, 无法进行操作.";
		return -1;
	}

	LVITEM lvi;
	memset(&lvi, 0, sizeof(lvi));
	lvi.mask = LVIF_TEXT;

	char buf[MAX_PATH];
	lvi.pszText = buf;
	lvi.cchTextMax = sizeof(buf);
	do
	{
		lvi.iItem = index;
		if(ListView_GetItem(lv, &lvi))
			selected_tables.push_back(buf);
	}while((index = ListView_GetNextItem(lv, index, LVNI_SELECTED)) >= 0);

	void *params[2];
	params[0] = &selected_tables;
	params[1] = &exported;
	DialogBoxParam(::grav::w32::kCurrentInstance,
	               MAKEINTRESOURCE(IDD_MAKING),
		           hwnd, MakingDlgProc,
				   reinterpret_cast<LPARAM>(params));
	return 0;
}

int OnMakeAll(HWND hwnd, int exported)
{
	std::vector<std::string> selected_tables;

	HWND lv = GetDlgItem(hwnd, IDC_TABLES);

	LVITEM lvi;
	memset(&lvi, 0, sizeof(lvi));
	lvi.mask = LVIF_TEXT;

	char buf[MAX_PATH];
	lvi.pszText = buf;
	lvi.cchTextMax = sizeof(buf);

	const int n = ListView_GetItemCount(lv);
	for(int i = 0; i < n; ++i)
	{
		lvi.iItem = i;
		if(ListView_GetItem(lv, &lvi))
			selected_tables.push_back(buf);
	}

	void *params[2];
	params[0] = &selected_tables;
	params[1] = &exported;
	DialogBoxParam(::grav::w32::kCurrentInstance,
	               MAKEINTRESOURCE(IDD_MAKING),
		           hwnd, MakingDlgProc,
				   reinterpret_cast<LPARAM>(params));
	return 0;
}

int OnRestore(HWND hwnd)
{
	static const int kBufLen = MAX_PATH * 1024;

	OPENFILENAME ofn;
	char *buf = new char[kBufLen];
	if(!buf)
		return -1;
	memset(buf, 0, kBufLen);

	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFile   = buf;
	ofn.nMaxFile    = kBufLen;
	ofn.hwndOwner   = hwnd;
	ofn.hInstance   = ::grav::w32::kCurrentInstance;
	ofn.lpstrFilter = "CSV Files (*.csv)\0*.csv\0XML Files (*.xml)\0*.xml\0All Files (*.*)\0*.*\0\0";
	ofn.lpstrDefExt = "csv";
	ofn.Flags       = OFN_ALLOWMULTISELECT | OFN_EXPLORER | OFN_HIDEREADONLY | OFN_LONGNAMES;

	::grav::w32::CurrentDirKeeper keeped(::grav::Linker::INITIALIZER);
	if(!GetOpenFileName(&ofn))
	{
		delete[] buf;
		return -1;
	}
	if(!(GetFileAttributes(buf) & FILE_ATTRIBUTE_DIRECTORY))
		*strrchr(buf, '\\') = 0;

	std::vector<std::string> selected_files;
	std::string prefix = buf;
	prefix += '\\';

	const char *p = buf + strlen(buf) + 1;
	selected_files.push_back(buf);
	do
	{
		std::string full_name = prefix + p;

		DWORD attr = GetFileAttributes(full_name.c_str());
		if(attr == INVALID_FILE_ATTRIBUTES)
			goto go_ahead;
		if(attr & FILE_ATTRIBUTE_DIRECTORY)
			goto go_ahead;

		selected_files.push_back(p);
	go_ahead:
		p += (strlen(p) + 1);
	}while(*p);
	delete[] buf;

	DialogBoxParam(::grav::w32::kCurrentInstance,
	               MAKEINTRESOURCE(IDD_RESTORE),
		           hwnd, RestoreDlgProc,
				   reinterpret_cast<LPARAM>(&selected_files));
	return 0;
}

int OnAllRestore(HWND hwnd)
{
	BROWSEINFO bi;
	memset(&bi, 0, sizeof(bi));

	bi.hwndOwner = hwnd;

	char buf[MAX_PATH];
	bi.pszDisplayName = buf;
	bi.lpszTitle = "选择存放快照的文件夹";
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;

	LPITEMIDLIST id_list = SHBrowseForFolder(&bi);
	if(!id_list || !SHGetPathFromIDList(id_list, buf))
		return -1;

	std::vector<std::string> selected_files;
	selected_files.push_back(buf);

	std::string prefix = selected_files.at(0) + "\\";
	std::string pattern = prefix + "*.*";

	WIN32_FIND_DATA ffd;
	memset(&ffd, 0, sizeof(ffd));
	HANDLE fh = FindFirstFile(pattern.c_str(), &ffd);
	if(fh != INVALID_HANDLE_VALUE)
	{
		do
		{
			if((ffd.cFileName[0] == '.' && ffd.cFileName[1] == '\0') ||
			   (ffd.cFileName[0] == '.' && ffd.cFileName[1] == '.' && ffd.cFileName[2] == '\0'))
			   continue;
			selected_files.push_back(ffd.cFileName);
		}while(FindNextFile(fh, &ffd));
		FindClose(fh);
	}

	if(selected_files.size() <= 1)
	{
		MSG_BOX(ERROR, hwnd) << "你所指定的文件夹里没有xml文件";
		return -1;
	}

	DialogBoxParam(::grav::w32::kCurrentInstance,
	               MAKEINTRESOURCE(IDD_RESTORE),
		           hwnd, RestoreDlgProc,
				   reinterpret_cast<LPARAM>(&selected_files));
	return 0;
}

INT_PTR OnCommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int id = LOWORD(wParam);

	UNUSED(msg);
	UNUSED(lParam);

	switch(id)
	{
	case IDOK:
	case IDCANCEL:
		EndDialog(hwnd, id);
		break;

	case ID_EXIT:
		EndDialog(hwnd, id);
		break;

	case ID_MAKE_SNAPSHOT:
		OnMake(hwnd, 0);
		break;

	case ID_MAKE_ALL_SNAPSHOT:
		OnMakeAll(hwnd, 0);
		break;

	case ID_MAKE_EXPORT:
		OnMake(hwnd, 1);
		break;

	case ID_MAKE_ALL_EXPORT:
		OnMakeAll(hwnd, 1);
		break;

	case ID_RESTORE:
		OnRestore(hwnd);
		break;

	case ID_ALL_RESTORE:
		OnAllRestore(hwnd);
		break;

	default:
		return FALSE;
	}

	return TRUE;
}

} // namespace


INT_PTR CALLBACK MainDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	UNUSED(lParam);

	switch(msg)
	{
	case WM_INITDIALOG:
		InitUI(hwnd);
		FetchTableName(hwnd);
		break;

	case WM_COMMAND:
		return OnCommand(hwnd, msg, wParam, lParam);

	case WM_CLOSE:
		EndDialog(hwnd, IDCANCEL);
		break;

	default:
		return FALSE;
	}

	return TRUE;
}