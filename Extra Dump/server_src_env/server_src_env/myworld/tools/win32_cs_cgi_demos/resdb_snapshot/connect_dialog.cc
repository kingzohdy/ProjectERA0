#include "resource.h"
#include "profile.h"
#include "win32utils.h"

namespace {

int StoreEntry(HWND hwnd, UINT id, std::string *import)
{
	import->clear();
	import->resize(GetWindowTextLength(GetDlgItem(hwnd, id)));
	return GetDlgItemText(hwnd, id, &import->at(0), import->length() + 1);
}

int ChangeTdrFilePath(HWND hwnd)
{
	OPENFILENAME ofn;
	char buf[MAX_PATH];

	memset(&ofn, 0, sizeof(ofn));
	memset(buf, 0, sizeof(buf));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFile   = buf;
	ofn.nMaxFile    = sizeof(buf);
	ofn.hwndOwner   = hwnd;
	ofn.hInstance   = ::grav::w32::kCurrentInstance;
	ofn.lpstrFilter = "Tdr Files (*.tdr)\0*.tdr\0All Files (*.*)\0*.*\0\0";
	ofn.lpstrDefExt = "tdr";
	ofn.Flags       = OFN_HIDEREADONLY;

	::grav::w32::CurrentDirKeeper keeped(::grav::Linker::INITIALIZER);
	if(!GetOpenFileName(&ofn))
		return -1;

	SetDlgItemText(hwnd, IDC_TDR_FILE, buf);
	return 0;
}

} // namespace

INT_PTR CALLBACK ConnectDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	UNUSED(lParam);

	switch(msg)
	{
	case WM_INITDIALOG:
		SetDlgItemText(hwnd, IDC_CONNECTION_STRING, CONF_EXPORT_Z(db_connect));
		SetDlgItemText(hwnd, IDC_DB_NAME, CONF_EXPORT_Z(db_name));
		SetDlgItemText(hwnd, IDC_USER, CONF_EXPORT_Z(db_user));
		SetDlgItemText(hwnd, IDC_PASSWD, CONF_EXPORT_Z(db_passwd));
		SetDlgItemText(hwnd, IDC_TDR_FILE, CONF_EXPORT_Z(tdr_file));
		break;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
		case IDCANCEL:
			EndDialog(hwnd, LOWORD(wParam));
			break;

		case IDC_CHANGE_TDR_FILE:
			ChangeTdrFilePath(hwnd);
			break;
		}
		break;

	case WM_DESTROY:
		StoreEntry(hwnd, IDC_CONNECTION_STRING, CONF_IMPORT(db_connect));
		StoreEntry(hwnd, IDC_DB_NAME, CONF_IMPORT(db_name));
		StoreEntry(hwnd, IDC_USER, CONF_IMPORT(db_user));
		StoreEntry(hwnd, IDC_PASSWD, CONF_IMPORT(db_passwd));
		StoreEntry(hwnd, IDC_TDR_FILE, CONF_IMPORT(tdr_file));
		break;

	case WM_CLOSE:
		EndDialog(hwnd, IDCANCEL);
		break;

	default:
		return FALSE;
	}

	return TRUE;
}