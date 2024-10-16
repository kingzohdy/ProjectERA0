#include "resource.h"
#include "db_putter.h"
#include "db_exporter.h"
#include "xml_splitter.h"
#include "profile.h"
#include "win32utils.h"
#include <shlobj.h>
#include <iomanip>
#include <fstream>

namespace {

class RestoreThread : public ::grav::w32::PlatformThread::Delegate
{
public:
	RestoreThread();

	virtual ~RestoreThread();

	void Prepare(HWND hwnd)
	{
		hwnd_ = hwnd;
		hwnd_ed_ = GetDlgItem(hwnd, IDC_INCOMING_TABLES);
	}

private:
	virtual void ThreadProcess();

	void RestoreTable(const std::string &full_file_name,
                      const std::string table_name);

	bool LazyInitCSV();

	void RestoreTableFromCSV(const std::string &full_file_name,
                             const std::string table_name);

	bool LazyInitXML();

	void RestoreTableFromXML(const std::string &full_file_name,
                             const std::string table_name);

	HWND hwnd_;
	HWND hwnd_ed_;
	::grav::DBPutter xml_importer_;
	bool xml_importer_initialized_;
	::grav::DBImporter csv_importer_;
	bool csv_importer_initialized_;
};

struct PrivateUserData
{
	std::vector<std::string> *selected_names;
	std::vector<std::string> selected_tables;
	HANDLE worker_thread;
	RestoreThread worker_delegate;
};

#define PRIVATE(hwnd) \
	(reinterpret_cast<PrivateUserData*>(GetWindowLongPtr(hwnd, DWLP_USER)))


/////////////////////////////////////////////////////////////////////////////////////////////////////////

RestoreThread::RestoreThread()
	: hwnd_(NULL)
	, hwnd_ed_(NULL)
	, xml_importer_initialized_(false)
	, csv_importer_initialized_(false)
{
}

//virtual
RestoreThread::~RestoreThread()
{
}

//virtual
void RestoreThread::ThreadProcess()
{
	const std::vector<std::string> &selected_names = *PRIVATE(hwnd_)->selected_names;
	const std::vector<std::string> &selected_tables = PRIVATE(hwnd_)->selected_tables;

	if(selected_names.size() <= 1)
		return;

	if(!(GetFileAttributes(selected_names.at(0).c_str()) & FILE_ATTRIBUTE_DIRECTORY))
		return;

	EnableWindow(GetDlgItem(hwnd_, IDC_START), FALSE);
	EnableWindow(GetDlgItem(hwnd_, IDCANCEL), FALSE);
	std::string full_file_name;
	for(std::vector<std::string>::size_type i = 0; i < selected_tables.size(); ++i)
	{
		full_file_name = selected_names[0] + "\\" + selected_names[i + 1];
		RestoreTable(full_file_name, selected_tables[i]);
	}

	EDIT_LINE(hwnd_ed_) << "����ѡ���ı�ָ����.";
	EnableWindow(GetDlgItem(hwnd_, IDC_START), TRUE);
	EnableWindow(GetDlgItem(hwnd_, IDCANCEL), TRUE);
}

bool RestoreThread::LazyInitCSV()
{
	if(csv_importer_initialized_)
		return true;

	csv_importer_.SetDBConnectionString(CONF_EXPORT(db_connect));
	csv_importer_.SetDBUser(CONF_EXPORT(db_user));
	csv_importer_.SetDBPasswd(CONF_EXPORT(db_passwd));

	if(!csv_importer_.Init())
	{
		MSG_BOX(ERROR, hwnd_) << "���ݿ����ӳ�ʼ��ʧ��.";
		return false;
	}

	if(!csv_importer_.Connect(CONF_EXPORT(db_name)))
	{
		MSG_BOX(ERROR, hwnd_) << "�޷����ӵ����ݿ�: " << CONF_EXPORT(db_name);
		return false;
	}

	EDIT_LINE(hwnd_ed_) << "�Ѿ����ӵ����ݿ�: " << CONF_EXPORT(db_name);
	csv_importer_initialized_ = true;
	return true;
}

void RestoreThread::RestoreTableFromCSV(const std::string &full_file_name,
                                        const std::string table_name)
{
	char szbuff[102400];
	std::ifstream stream(full_file_name.c_str());
	if(!stream.is_open())
	{
		EDIT_LINE(hwnd_ed_) << "�޷��ָ���: " << table_name << " ������, ��Ϊ�򲻿������ļ�: "
		                    << full_file_name << " ����(" << GetLastError() << ")";
		return;
	}

	csv_importer_.SetTableName(table_name);
	if(!csv_importer_.Begin())
	{
		EDIT_LINE(hwnd_ed_) << "�޷��ָ���: " << table_name << " ������, ��Ϊ�޷���ʼ����.";
		return;
	}

	EDIT_LINE(hwnd_ed_) << "�ָ���: " << table_name << " ...";

	if(!csv_importer_.Drop())
	{
		EDIT_LINE(hwnd_ed_) << "�ָ�������ʧ��, �޷����ԭ��������.";
		xml_importer_.Rollback();
		return;
	}

	stream.getline(szbuff, sizeof(szbuff));

	while(csv_importer_.ImportFromCSV(&stream))
		;

	if(!csv_importer_.Succeeded())
	{
		EDIT_LINE(hwnd_ed_) << "�ָ�������ʧ��.";
		csv_importer_.Rollback();
	}
	else
	{
		csv_importer_.Commit();
	}
}

bool RestoreThread::LazyInitXML()
{
	if(xml_importer_initialized_)
		return true;

	xml_importer_.SetDBConnectionString(CONF_EXPORT(db_connect));
	xml_importer_.SetDBUser(CONF_EXPORT(db_user));
	xml_importer_.SetDBPasswd(CONF_EXPORT(db_passwd));

	int version = 0;
	if(!xml_importer_.Init(&version, CONF_EXPORT(tdr_file)))
	{
		MSG_BOX(ERROR, hwnd_) << "��TDR�ļ�: " << CONF_EXPORT(tdr_file) << "ʧ��.";
		return false;
	}

	xml_importer_.SetVersion(version);

	if(!xml_importer_.Connect(CONF_EXPORT(db_name)))
	{
		MSG_BOX(ERROR, hwnd_) << "�޷����ӵ����ݿ�: " << CONF_EXPORT(db_name);
		return false;
	}

	EDIT_LINE(hwnd_ed_) << "�Ѿ����ӵ����ݿ�: " << CONF_EXPORT(db_name) << ", ʹ��Ԫ���ݰ汾��: "
	                    << version;
	xml_importer_initialized_ = true;
	return true;
}

void RestoreThread::RestoreTableFromXML(const std::string &full_file_name,
                                        const std::string table_name)
{
	if(!xml_importer_.Test(table_name))
	{
		EDIT_LINE(hwnd_ed_) << "�޷��ָ���: " << table_name << " ������, ��Ϊ�Ҳ�����Ӧ��Ԫ������Ϣ.";
		return;
	}

	std::ifstream stream(full_file_name.c_str());
	if(!stream.is_open())
	{
		EDIT_LINE(hwnd_ed_) << "�޷��ָ���: " << table_name << " ������, ��Ϊ�򲻿������ļ�: "
		                    << full_file_name << " ����(" << GetLastError() << ")";
		return;
	}

	if(!xml_importer_.Begin(table_name))
	{
		EDIT_LINE(hwnd_ed_) << "�޷��ָ���: " << table_name << " ������, ��Ϊ�޷���ʼ����.";
		return;
	}

	EDIT_LINE(hwnd_ed_) << "�ָ���: " << table_name << " ...";

	if(!xml_importer_.DeleteAll())
	{
		EDIT_LINE(hwnd_ed_) << "�ָ�������ʧ��, �޷����ԭ��������.";
		xml_importer_.Rollback();
		return;
	}

	grav::XMLSplitter splitter(&stream);
	while(splitter.Next())
	{
		if(!xml_importer_.Xml2Bin(splitter.CurrentSegment()))
		{
			EDIT_LINE(hwnd_ed_) << "�ָ�������ʧ��, XML����ʧ��.";
			xml_importer_.Rollback();
			return;
		}

		if(!xml_importer_.InsertRow())
		{
			EDIT_LINE(hwnd_ed_) << "�ָ�������ʧ��, SQL���ִ��ʧ��.";
			xml_importer_.Rollback();
			return;
		}
	}
	xml_importer_.Commit();
}

void RestoreThread::RestoreTable(const std::string &full_file_name,
                                 const std::string table_name)
{
	std::string::size_type dot = full_file_name.rfind(".csv");
	if(dot != std::string::npos && full_file_name.length() - dot == 4)
	{
		EDIT_LINE(hwnd_ed_) << "ʹ��.csv��ʽ";
		if(!LazyInitCSV())
			return;
		RestoreTableFromCSV(full_file_name, table_name);
		return;
	}

	dot = full_file_name.rfind(".xml");
	if(dot != std::string::npos && full_file_name.length() - dot == 4)
	{
		EDIT_LINE(hwnd_ed_) << "ʹ��.xml��ʽ";
		if(!LazyInitXML())
			return;
		RestoreTableFromXML(full_file_name, table_name);
		return;
	}

	EDIT_LINE(hwnd_ed_) << "��֧�ֵĸ�ʽ: " << full_file_name;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

int PerpareIncomingFiles(std::vector<std::string> *selected_names,
                         std::vector<std::string> *selected_tables)
{
	if(selected_names->size() <= 1)
		return -1;

	std::string name;
	for(size_t i = 1; i < selected_names->size(); ++i)
	{
		name = selected_names->at(i);
		name.erase(name.rfind('.'));
		selected_tables->push_back(name);
	}
	return 0;
}

int InitUI(HWND hwnd, const std::vector<std::string> &selected_tables)
{
	std::ostringstream stream;
	stream << "Ԥ��Ҫ�ָ��ı�:\n";
	for(std::vector<std::string>::const_iterator iter = selected_tables.begin();
		iter != selected_tables.end(); ::grav::ignore_result(++iter))
		stream << *iter << ",";
	stream << "\r\n";

	SetDlgItemText(hwnd, IDC_INCOMING_TABLES, stream.str().c_str());
	return 0;
}

int OnRestoreData(HWND hwnd)
{
	if(::grav::w32::PlatformThread::Alive(PRIVATE(hwnd)->worker_thread))
		return -1;

	PRIVATE(hwnd)->worker_thread = 
		::grav::w32::PlatformThread::Start(&PRIVATE(hwnd)->worker_delegate);
	if(!PRIVATE(hwnd)->worker_thread)
	{
		MSG_BOX(ERROR, hwnd) << "�޷��������������߳�.";
		return -1;
	}
	return 0;
}

} // namespace

INT_PTR CALLBACK RestoreDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_INITDIALOG:
		{
			PrivateUserData *pud = new PrivateUserData;

			pud->selected_names = reinterpret_cast<std::vector<std::string>* >(lParam);
			pud->worker_delegate.Prepare(hwnd);
			pud->worker_thread = NULL;

			PerpareIncomingFiles(pud->selected_names, &pud->selected_tables);
			InitUI(hwnd, pud->selected_tables);
			SetWindowLongPtr(hwnd, DWLP_USER, reinterpret_cast<LONG_PTR>(pud));
		}
		break;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDCANCEL:
			if(::grav::w32::PlatformThread::Alive(PRIVATE(hwnd)->worker_thread))
				break;
			EndDialog(hwnd, LOWORD(wParam));
			break;

		case IDC_START:
			OnRestoreData(hwnd);
			break;

		default:
			return FALSE;
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