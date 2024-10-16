#include "resource.h"
#include "profile.h"
#include "win32utils.h"
#include "db_exporter.h"
#include <fstream>
#include <commctrl.h>


const char kProfileName[] = "resdb_snapshot.conf";

int csv2db(const std::string &Connect,const std::string &DBname, const std::string &User,const std::string &Passwd, const std::string &tablename, const std::string &filename);

INT_PTR CALLBACK ConnectDlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK MainDlgProc(HWND, UINT, WPARAM, LPARAM);


INT_PTR WINAPI WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd_line, int show_cmd)
{
	int argc = 0;
	LPWSTR *argv = ::CommandLineToArgvW(::GetCommandLineW(),&argc);

	grav::Profile profile_scope(grav::Linker::INITIALIZER);
	if(!profile_scope.Load(kProfileName))
	{
		MSG_BOX(ERROR, GetActiveWindow()) << "�޷����������ļ�: " << kProfileName;
		return 1;
	}

	if(argc == 3)
	{
		char szfilename[256];
		char sztablename[256];

		sprintf(sztablename,"%ws",argv[1]);
		sprintf(szfilename,"%ws",argv[2]);

		return csv2db(CONF_EXPORT(db_connect),CONF_EXPORT(db_name),CONF_EXPORT(db_user),CONF_EXPORT(db_passwd),sztablename,szfilename);
	}
	
	::grav::w32::kCurrentInstance = instance;

	UNUSED(prev_instance);
	UNUSED(cmd_line);
	UNUSED(show_cmd);

	CoInitialize(NULL);
	InitCommonControls();

	INT_PTR rv = DialogBox(::grav::w32::kCurrentInstance, MAKEINTRESOURCE(IDD_CONNECT),
		                   NULL, ConnectDlgProc);
	if(rv != IDOK)
		return 1;

	if(!profile_scope.Store(kProfileName))
	{
		MSG_BOX(ERROR, GetActiveWindow()) << "�޷�����������Ϣ���ļ�: " << kProfileName;
		return 1;
	}

	rv = DialogBox(::grav::w32::kCurrentInstance, MAKEINTRESOURCE(IDD_MAIN), NULL,
	               MainDlgProc);

	CoUninitialize();
	return rv;
}

int csv2db(const std::string &Connect, const std::string &DBname, const std::string &User,const std::string &Passwd, const std::string &tablename, const std::string &filename)
{
	grav::DBImporter csv_importer;
	char szbuff[102400];

	csv_importer.SetDBConnectionString(Connect);
	csv_importer.SetDBUser(User);
	csv_importer.SetDBPasswd(Passwd);

	if(!csv_importer.Init())
	{
		return -1;
	}

	if(!csv_importer.Connect(DBname))
	{
		//���Ӵ���
		return -1;
	}

	std::ifstream stream(filename.c_str());

	if(!stream.is_open())
	{
		//EDIT_LINE(hwnd_ed_) << "�޷��ָ���: " << table_name << " ������, ��Ϊ�򲻿������ļ�: "
		return 0;
	}

	stream.getline(szbuff, sizeof(szbuff));

	csv_importer.SetTableName(tablename);
	if(!csv_importer.Begin())
	{
		//EDIT_LINE(hwnd_ed_) << "�޷��ָ���: " << table_name << " ������, ��Ϊ�޷���ʼ����.";
		return 0;
	}

	if(!csv_importer.Drop())
	{
		//EDIT_LINE(hwnd_ed_) << "�ָ�������ʧ��, �޷����ԭ��������.";
		return -1;
	}

	while(csv_importer.ImportFromCSV(&stream))
		;

	if(!csv_importer.Succeeded())
	{
		//EDIT_LINE(hwnd_ed_) << "�ָ�������ʧ��.";
		return -1;
	}
	else
	{
		csv_importer.Commit();
	}
	
	return 0;
}