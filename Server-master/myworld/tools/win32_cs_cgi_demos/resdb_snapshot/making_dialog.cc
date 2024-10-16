#include "resource.h"
#include "db_fetcher.h"
#include "db_exporter.h"
#include "profile.h"
#include "win32utils.h"
#include <shlobj.h>
#include <iomanip>
#include <fstream>

namespace {

bool CheckOrCreateOutputDirectory(const char *path);

class MakeSnapshotThread : public ::grav::w32::PlatformThread::Delegate
{
public:
	MakeSnapshotThread();

	virtual ~MakeSnapshotThread();

	void Prepare(HWND hwnd)
	{
		hwnd_ = hwnd;
		hwnd_ed_ = GetDlgItem(hwnd, IDC_INCOMING_TABLES);
	}

	bool Init();

private:
	virtual void ThreadProcess();

	HWND hwnd_;
	HWND hwnd_ed_;
	::grav::DBFetcher fetcher_;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////

class MakeExportThread : public ::grav::w32::PlatformThread::Delegate
{
public:
	MakeExportThread();

	virtual ~MakeExportThread();

	void Prepare(HWND hwnd)
	{
		hwnd_ = hwnd;
		hwnd_ed_ = GetDlgItem(hwnd, IDC_INCOMING_TABLES);
	}

	bool Init();

private:
	virtual void ThreadProcess();

	HWND hwnd_;
	HWND hwnd_ed_;
	::grav::DBExporter exporter_;
};

struct PrivateUserData
{
	int exported;
	const std::vector<std::string> *selected_tables;
	HANDLE worker_thread;
	MakeSnapshotThread worker_delegate;
	MakeExportThread export_delegate;
};

#define PRIVATE(hwnd) \
	(reinterpret_cast<PrivateUserData*>(GetWindowLongPtr(hwnd, DWLP_USER)))

/////////////////////////////////////////////////////////////////////////////////////////////////////////

MakeSnapshotThread::MakeSnapshotThread()
	: hwnd_(NULL), hwnd_ed_(NULL)
{
}

//virtual
MakeSnapshotThread::~MakeSnapshotThread()
{
}

bool MakeSnapshotThread::Init()
{
	char buf[MAX_PATH];
	GetDlgItemText(hwnd_, IDC_OUTPUT, buf, CCH(buf));
	if(!CheckOrCreateOutputDirectory(buf))
	{
		MSG_BOX(ERROR, hwnd_) << "找不到指定的输出路径.";
		return false;
	}

	fetcher_.SetDBConnectionString(CONF_EXPORT(db_connect));
	fetcher_.SetDBUser(CONF_EXPORT(db_user));
	fetcher_.SetDBPasswd(CONF_EXPORT(db_passwd));

	int version = 0;
	if(!fetcher_.Init(&version, CONF_EXPORT(tdr_file)))
	{
		MSG_BOX(ERROR, hwnd_) << "打开TDR文件: " << CONF_EXPORT(tdr_file) << "失败.";
		return false;
	}

	fetcher_.SetVersion(version);

	if(!fetcher_.Connect(CONF_EXPORT(db_name)))
	{
		MSG_BOX(ERROR, hwnd_) << "无法连接到数据库: " << CONF_EXPORT(db_name);
		return false;
	}

	EDIT_LINE(hwnd_ed_) << "已经连接到数据库: " << CONF_EXPORT(db_name) << ", 使用元数据版本号: "
	                    << version;
	return true;
}

//virtual
void MakeSnapshotThread::ThreadProcess()
{
	const std::vector<std::string> &tables = *PRIVATE(hwnd_)->selected_tables;
	std::string output_prefix;

	output_prefix.resize(MAX_PATH);
	int len = GetDlgItemText(hwnd_, IDC_OUTPUT, &output_prefix[0],
		                     output_prefix.capacity());
	output_prefix.resize(len);
	if(output_prefix[output_prefix.length() - 1] != '\\')
		output_prefix += '\\';

	EnableWindow(GetDlgItem(hwnd_, IDC_CHANGE_OUPUT), FALSE);
	EnableWindow(GetDlgItem(hwnd_, IDC_OUTPUT), FALSE);
	EnableWindow(GetDlgItem(hwnd_, IDC_START), FALSE);
	EnableWindow(GetDlgItem(hwnd_, IDCANCEL), FALSE);
	for(std::vector<std::string>::const_iterator iter = tables.begin();
		iter != tables.end();
		::grav::ignore_result(++iter))
	{
		if(!fetcher_.Test(*iter))
		{
			EDIT_LINE(hwnd_ed_) << "无法创建表: " << *iter << "的快照, 因为找不到对应的元数据信息.";
			continue;
		}

		if(!fetcher_.Query(*iter))
		{
			EDIT_LINE(hwnd_ed_) << "无法查询表: " << *iter;
			continue;
		}

		std::string file_name = output_prefix + *iter + ".xml";
		std::ofstream fs(file_name.c_str());
		if(!fs.is_open())
		{
			EDIT_LINE(hwnd_ed_) << "无法打开文件: " << file_name
			                    << " ; 无法创建" << *iter << "的快照.";
			continue;
		}

		EDIT_LINE(hwnd_ed_) << "创建表: " << *iter << " 的快照...";
		std::string raw_xml;
		while(fetcher_.Fetch())
		{
			if(!fetcher_.Bin2Xml(&raw_xml))
				EDIT_LINE(hwnd_ed_) << "数据转换的XML失败, 创建的快照可能不是完整的.";
			fs << raw_xml;
		}
	}
	EDIT_LINE(hwnd_ed_) << "快照创建完成, 保存在: " << output_prefix;

	EnableWindow(GetDlgItem(hwnd_, IDC_CHANGE_OUPUT), TRUE);
	EnableWindow(GetDlgItem(hwnd_, IDC_OUTPUT), TRUE);
	EnableWindow(GetDlgItem(hwnd_, IDC_START), TRUE);
	EnableWindow(GetDlgItem(hwnd_, IDCANCEL), TRUE);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

MakeExportThread::MakeExportThread()
	: hwnd_(NULL), hwnd_ed_(NULL)
{
}

//virtual
MakeExportThread::~MakeExportThread()
{
}

bool MakeExportThread::Init()
{
	char buf[MAX_PATH];
	GetDlgItemText(hwnd_, IDC_OUTPUT, buf, CCH(buf));
	if(!CheckOrCreateOutputDirectory(buf))
	{
		MSG_BOX(ERROR, hwnd_) << "找不到指定的输出路径.";
		return false;
	}

	exporter_.SetDBConnectionString(CONF_EXPORT(db_connect));
	exporter_.SetDBUser(CONF_EXPORT(db_user));
	exporter_.SetDBPasswd(CONF_EXPORT(db_passwd));

	if(!exporter_.Init() || !exporter_.Connect(CONF_EXPORT(db_name)))
	{
		MSG_BOX(ERROR, hwnd_) << "无法连接到数据库: " << CONF_EXPORT(db_name);
		return false;
	}

	if(CONF_EXPORT(export_format) != "txt" &&
	   CONF_EXPORT(export_format) != "csv")
	{
		MSG_BOX(ERROR, hwnd_) << "选定的格式: " << CONF_EXPORT(export_format) << "目前还不支持";
		return false;
	}

	EDIT_LINE(hwnd_ed_) << "已经连接到数据库: " << CONF_EXPORT(db_name);
	return true;
}

//virtual
void MakeExportThread::ThreadProcess()
{
	const std::vector<std::string> &tables = *PRIVATE(hwnd_)->selected_tables;
	std::string output_prefix;

	output_prefix.resize(MAX_PATH);
	int len = GetDlgItemText(hwnd_, IDC_OUTPUT, &output_prefix[0],
		                     output_prefix.capacity());
	output_prefix.resize(len);
	if(output_prefix[output_prefix.length() - 1] != '\\')
		output_prefix += '\\';

	EnableWindow(GetDlgItem(hwnd_, IDC_CHANGE_OUPUT), FALSE);
	EnableWindow(GetDlgItem(hwnd_, IDC_OUTPUT), FALSE);
	EnableWindow(GetDlgItem(hwnd_, IDC_START), FALSE);
	EnableWindow(GetDlgItem(hwnd_, IDCANCEL), FALSE);

	bool (::grav::DBExporter::*routine)(std::ostream *);
	if(CONF_EXPORT(export_format) == "txt")
		routine = &::grav::DBExporter::Export2TXT;
	else if(CONF_EXPORT(export_format) == "csv")
		routine = &::grav::DBExporter::Export2CSV;
	else
		return; // Noreached;
	std::string ext = CONF_EXPORT(export_format);
	for(std::vector<std::string>::const_iterator iter = tables.begin();
		iter != tables.end();
		::grav::ignore_result(++iter))
	{
		std::string file_name = output_prefix + *iter + "." + ext;
		std::ofstream fs(file_name.c_str());
		if(!fs.is_open())
		{
			EDIT_LINE(hwnd_ed_) << "无法打开文件: " << file_name
			                    << " ; 无法导出" << *iter << "的数据.";
			continue;
		}

		if(CONF_EXPORT(export_format) == "csv")
		{
			if(!exporter_.Fields2CSV(*iter,&fs))
			{
				EDIT_LINE(hwnd_ed_) << "无法创建表头: " << *iter;
				continue;
			}
		}

		if(!exporter_.Query(*iter))
		{
			EDIT_LINE(hwnd_ed_) << "无法查询表: " << *iter;
			continue;
		}

		

		EDIT_LINE(hwnd_ed_) << "导出表: " << *iter << " 的数据...";

		while(exporter_.Fetch())
			(exporter_.*routine)(&fs);
	}
	EDIT_LINE(hwnd_ed_) << "数据导出完成, 保存在: " << output_prefix;

	EnableWindow(GetDlgItem(hwnd_, IDC_CHANGE_OUPUT), TRUE);
	EnableWindow(GetDlgItem(hwnd_, IDC_OUTPUT), TRUE);
	EnableWindow(GetDlgItem(hwnd_, IDC_START), TRUE);
	EnableWindow(GetDlgItem(hwnd_, IDCANCEL), TRUE);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string GenerateDir(int exported)
{
	std::ostringstream stream;

	SYSTEMTIME jiff;
	GetLocalTime(&jiff);

	if(exported)
		stream << CONF_EXPORT(export_prefix) << '\\';
	else
		stream << CONF_EXPORT(output_prefix) << '\\';
	stream.fill('0');
	stream << std::setw(4) << jiff.wYear << '-'
	       << std::setw(2) << jiff.wMonth << '-'
		   << std::setw(2) << jiff.wDay << '_'
		   << std::setw(2) << jiff.wHour
		   << std::setw(2) << jiff.wMinute
		   << std::setw(2) << jiff.wSecond;

	return stream.str();
}

bool CheckOrCreateOutputDirectory(const char *path)
{
	char buf[MAX_PATH];
	memset(buf, 0, sizeof(buf));
	strncpy(buf, path, CCH(buf));

	std::replace(buf, buf + strlen(buf), '\\', '\0');
	std::string touched;
	const char *p = buf;
	do
	{
		touched += p;

		DWORD attr = GetFileAttributes(touched.c_str());
		if(attr == INVALID_FILE_ATTRIBUTES)
		{
			if(!CreateDirectory(touched.c_str(), NULL))
				return false;
		}
		else
		{
			if(!(attr & FILE_ATTRIBUTE_DIRECTORY))
				return false;
		}

		p += (strlen(p) + 1);
		if(*p) touched += '\\';
	}while(*p);

	return true;
}

int InitUI(HWND hwnd, const std::vector<std::string> &selected_tables, int exported)
{
	std::ostringstream stream;

	if(exported)
		stream << "预计要导出的表(格式" << CONF_EXPORT(export_format) << "):\r\n";
	else
		stream << "预计要生成快照的表:\r\n";
	for(std::vector<std::string>::const_iterator iter = selected_tables.begin();
	    iter != selected_tables.end(); ++iter)
		stream << *iter << ", ";
	stream << "\r\n";

	SetWindowText(hwnd, exported ? "导出保存:" : "快照保存:");
	SetDlgItemText(hwnd, IDC_INCOMING_TABLES, stream.str().c_str());
	SetDlgItemText(hwnd, IDC_OUTPUT,
	               GenerateDir(exported).c_str());
	return 0;
}

int OnChangeOutput(HWND hwnd)
{
	BROWSEINFO bi;
	memset(&bi, 0, sizeof(bi));

	bi.hwndOwner = hwnd;

	char buf[MAX_PATH];
	bi.pszDisplayName = buf;
	if(PRIVATE(hwnd)->exported)
		bi.lpszTitle = "选择导出文件存放路径";
	else
		bi.lpszTitle = "选择快照存放路径";
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;

	LPITEMIDLIST id_list = SHBrowseForFolder(&bi);
	if(id_list)
		if(SHGetPathFromIDList(id_list, buf))
			SetDlgItemText(hwnd, IDC_OUTPUT, buf);
	return 0;
}

int OnMake(HWND hwnd)
{
	if(::grav::w32::PlatformThread::Alive(PRIVATE(hwnd)->worker_thread))
		return -1;

	if(PRIVATE(hwnd)->exported)
	{
		if(!PRIVATE(hwnd)->export_delegate.Init())
			return -1;
		PRIVATE(hwnd)->worker_thread = 
			::grav::w32::PlatformThread::Start(&PRIVATE(hwnd)->export_delegate);
		if(!PRIVATE(hwnd)->worker_thread)
		{
			MSG_BOX(ERROR, hwnd) << "无法启动数据导出线程.";
			return -1;
		}
	}
	else
	{
		if(!PRIVATE(hwnd)->worker_delegate.Init())
			return -1;
		PRIVATE(hwnd)->worker_thread = 
			::grav::w32::PlatformThread::Start(&PRIVATE(hwnd)->worker_delegate);
		if(!PRIVATE(hwnd)->worker_thread)
		{
			MSG_BOX(ERROR, hwnd) << "无法启动快照生成线程.";
			return -1;
		}
	}
	return 0;
}

}

INT_PTR CALLBACK MakingDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_INITDIALOG:
		{
			PrivateUserData *pud = new PrivateUserData;

			void **args = reinterpret_cast<void ** >(lParam);
			pud->selected_tables = static_cast<std::vector<std::string>* >(args[0]);
			pud->exported = *static_cast<int*>(args[1]);
			pud->worker_delegate.Prepare(hwnd);
			pud->export_delegate.Prepare(hwnd);
			pud->worker_thread = NULL;

			InitUI(hwnd, *(pud->selected_tables), pud->exported);
			SetWindowLongPtr(hwnd, DWLP_USER, reinterpret_cast<LONG_PTR>(pud));
		}
		break;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDCANCEL:
			if(::grav::w32::PlatformThread::Alive(PRIVATE(hwnd)->worker_thread))
				break;
			EndDialog(hwnd, IDCANCEL);
			break;

		case IDC_CHANGE_OUPUT:
			OnChangeOutput(hwnd);
			break;

		case IDC_START:
			OnMake(hwnd);
			break;
		}
		break;

	case WM_DESTROY:
		delete PRIVATE(hwnd);
		SetWindowLongPtr(hwnd, DWLP_USER, 0);
		break;

	case WM_CLOSE:
		if(::grav::w32::PlatformThread::Alive(PRIVATE(hwnd)->worker_thread))
			break;
		EndDialog(hwnd, IDCANCEL);
		break;

	default:
		return FALSE;
	}

	return TRUE;
}