// ExtractorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Extractor.h"
#include "ExtractorDlg.h"
#include <Windows.h>
#include "OgrePackageFile.h"
#include "GamePackageFile.h"
#include "UtilCtrl.h"
#include "CIniFile.h"

#include "zlib.h"

#include "patchFile.h"
#include "des.h"
#include <Wininet.h>

#pragma comment(lib, "Wininet.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


unsigned int version_to_uint(const wchar_t *pszVer)
{
	// 1.2.100   1--8bit  2--8bit  100--16bit
	unsigned int iVersion = 0;
	int iSegment = 0;
	const wchar_t *pszTempVer = pszVer;
	int iCount = 0;

	while(pszTempVer)
	{
		iCount ++;
		iSegment = _wtof(pszTempVer);
		if (iSegment < 0)
		{
			return -1;
		}
		if (iCount == 1)
		{
			if (iSegment > 255)
			{
				return -1;
			}

			iVersion += iSegment << 24;
		}
		else if(iCount == 2)
		{
			if (iSegment > 255)
			{
				return -1;
			}
			iVersion += iSegment << 16;
		}
		else if(iCount == 3)
		{
			if (iSegment > 65535)
			{
				return -1;
			}
			iVersion += iSegment;
		}
		else
		{return -1;}

		pszTempVer = wcsstr(pszTempVer, _T("."));
		if (!pszTempVer)
		{
			return iVersion;
		}
		pszTempVer ++;
	}
	return -1;
}

void getFileVersion(LPCTSTR szFileName,LPTSTR szVersion,int nLength)
{
 	wsprintf(szVersion,_T(""));
 	char fileName[256] = {'\0'};

 	char* pszDecryptBuffer	= getDecryptFileBuffer( szFileName, CLIENT_JIEMI );
	if ( pszDecryptBuffer == NULL )
	{
		return;
	}
	CIniFile iniNewVer;
	iniNewVer.Prepare( pszDecryptBuffer );
	iniNewVer.GetString( "version", "ver", "0", fileName, 256 );
 	iniNewVer.Close(); 
 	delete[] pszDecryptBuffer;
	MultiByteToWideChar(
		GetACP(),
		MB_PRECOMPOSED,
		fileName,
		256,
		szVersion,
		nLength );
}

// CExtractorDlg dialog

// ����һ��Ŀ¼��������ȫ����ѹ
const int MAXUNCOMPATH = 11;
static wstring strUnCompress[MAXUNCOMPATH] = {
	_T("character\\"),
	_T("db\\"),
	_T("effect\\"),
	_T("maps\\"),
	_T("scene\\"),
	_T("script\\"),
	_T("shaders\\"),
	_T("texture\\"),
	_T("toolres\\"),
	_T("uires\\"),
	_T("sounds\\")
};

bool isInUnCompressPath( wstring& str )
{
	for( int i = 0; i< MAXUNCOMPATH; i++ )
	{
		if( str.find( strUnCompress[i].c_str() ) == 0 )
		{
			return true;
		}
	}

	return false;
}

inline LPCTSTR GetRegistKeyStr( LPCTSTR Path, LPCTSTR Name )
{
	HKEY hKey = NULL;
	DWORD Len = 1024;
	DWORD Type = REG_SZ;
	static BYTE  Buf[1024];
	RegCreateKey( HKEY_LOCAL_MACHINE, Path, &hKey );
	if( ERROR_SUCCESS != RegQueryValueEx( hKey, Name, NULL, &Type, Buf, &Len ) )
		Buf[0] = 0;
	RegCloseKey(hKey);

	return (LPCTSTR) Buf;
}

bool CreateMultipleDirectory(const CString& szPath)
{
	CString strDir(szPath);//���Ҫ������Ŀ¼�ַ���
	//ȷ����'\'��β�Դ������һ��Ŀ¼
	if (strDir.GetAt(strDir.GetLength()-1)!=_T('\\'))
	{
		strDir.AppendChar(_T('\\'));
	}
	std::vector<CString> vPath;//���ÿһ��Ŀ¼�ַ���
	CString strTemp;//һ����ʱ����,���Ŀ¼�ַ���
	bool bSuccess = false;//�ɹ���־
	//����Ҫ�������ַ���
	for (int i=0;i<strDir.GetLength();++i)
	{
		if (strDir.GetAt(i) != _T('\\')) 
		{//�����ǰ�ַ�����'\\'
			strTemp.AppendChar(strDir.GetAt(i));
		}
		else 
		{//�����ǰ�ַ���'\\'
			vPath.push_back(strTemp);//����ǰ����ַ�����ӵ�������
			strTemp.AppendChar(_T('\\'));
		}
	}

	//�������Ŀ¼������,����ÿ��Ŀ¼
	std::vector<CString>::const_iterator vIter;
	for (vIter = vPath.begin(); vIter != vPath.end(); vIter++) 
	{
		//���CreateDirectoryִ�гɹ�,����true,���򷵻�false
		bSuccess = CreateDirectory(*vIter, NULL) ? true : false;    
	}

	return bSuccess;
}

DWORD WINAPI ThreadUpdate(LPVOID param)
{
	bool err = false;
	CExtractorDlg* dlg = (CExtractorDlg*) param;
	CString gamePath = _T("");

	TCHAR curDir[MAX_PATH] = {0};
	::GetCurrentDirectory(MAX_PATH,curDir);
	wstring strDir(wcslwr(curDir));
	if( strDir.rfind(_T("\\patch")) == strDir.size() - 6 )
	{
		strDir = strDir.substr(0,strDir.rfind(_T("\\patch")));
	}
	wstring cstrFindDir = strDir + _T("\\version.data");
	CFileFind finder;
	BOOL bFind = finder.FindFile( cstrFindDir.c_str() );
	if( bFind )
	{
		gamePath = strDir.c_str();
	}else{
		gamePath = GetRegistKeyStr( _T("Software\\WanYou YL\\Era Zero"), _T("RootPath") );

	}
	if( gamePath.IsEmpty() )
	{
		dlg->ShowOpenFileDialog(gamePath);
	}else
		dlg->unCompressedPackage( gamePath.GetString() );

	PostMessage(dlg->m_hWnd, WM_NOTIFY_MESSAGE, 800, err);
	return 0;
}

void CExtractorDlg::ShowOpenFileDialog(CString gamePath)
{
	GetDlgItem( IDC_STATIC )->SetWindowText( _T("��ѡ����ϷĿ¼") );

	CArkFileDialog Dlg( TRUE, NULL, _T("ѡ����ϷĿ¼"), 
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR|OFN_NOVALIDATE, _T(""),this);
	gamePath = _T( "C:\\" );
	Dlg.m_ofn.lpstrInitialDir = gamePath;
	if( Dlg.DoModal( gamePath )== IDOK )
	{
		unCompressedPackage( gamePath.GetString() );
	}else{
		EndDialog(IDOK);
	}

}

CExtractorDlg::CExtractorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExtractorDlg::IDD, pParent),
	m_bmUpdateProgress(IDB_BITMAP_PROGRESS_BACK,IDB_BITMAP_PROGRESS_FORE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CExtractorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_PROGRESS, m_bmUpdateProgress);
}

void CExtractorDlg::startUpdate()
{
	DWORD dwThreadId;
	HANDLE hThread = CreateThread(
		NULL,
		0, 
		ThreadUpdate, 
		this, 
		0, 
		&dwThreadId );

	CloseHandle(hThread);
}

// ���
void CExtractorDlg::unCompressedPackage( LPCTSTR szPath )
{	
	m_bmUpdateProgress.SetInfo(CString(_T("�����ͷŲ����������Ժ�.....")));
	// ��������Ŀ¼
	wstring strPatchPath = wstring( szPath ) + wstring( _T("\\Patch") );
	CreateMultipleDirectory( CString( strPatchPath.c_str() ) );
	// ���ڲ������ͷŵ�����Ŀ¼
	m_SelfExtractor.ReadTOC(m_SelfExtractor.GetThisFileName());	
	//��ѹversion.data
	if(!ExtractorFile(_T("version.data"),strPatchPath.c_str()))
	{
		MessageBox( _T("��ѹ�汾�ļ�ʧ��"), _T("��ʾ"), MB_OK );
		EndDialog(TRUE);
	}

	// �汾�Ƚ�
	wstring newVerFile = strPatchPath + wstring(_T("\\version.data"));
	wstring oldVerFile = wstring( szPath ) + wstring(_T("\\version.data"));
	
	TCHAR szBuf[50] = {_T('\0')};
	getFileVersion(newVerFile.c_str(),szBuf,50 );
	wstring strNewVer(szBuf);
	getFileVersion(oldVerFile.c_str(),szBuf,50);
	wstring strOldVer(szBuf);

	wstring fileName = m_SelfExtractor.GetThisFileName();
	fileName = fileName.substr( fileName.find_last_of(_T("\\"))+1 );
	if( fileName.find(_T("_")) == fileName.npos )
	{
		MessageBox(_T("�����汾��Ϣ���ƻ������������ز���"),_T("��ʾ"),MB_OK );
		EndDialog( TRUE );
		return;
	}
	fileName = fileName.substr(0,fileName.find(_T("_")));
	
	if(  fileName.compare(strOldVer) != 0 )
	{
		MessageBox(_T("�����汾��ͻ��˰汾��һ�£���ѡ����Ϸ·����"),_T("��ʾ"),MB_OK);
		CString gamePath(_T("C:\\"));
		ShowOpenFileDialog(gamePath);
		return;
	}
	if( strOldVer.empty() )
	{
		MessageBox( _T("�ͻ��˰汾�ļ���Ϣ���ƻ������°�װ�ͻ���"), _T("��ʾ"), MB_OK );
		EndDialog( TRUE );
		return;
	}else
	{
		if( version_to_uint( strOldVer.c_str() ) >= version_to_uint(strNewVer.c_str()) )
		{
			MessageBox( _T("�ͻ��˰汾�Ѿ������µ�������£���ѡ����Ϸ·����"), _T("��ʾ"), MB_OK );
			CString gamePath(_T("C:\\"));
			ShowOpenFileDialog(gamePath);
			return;
		}
	}

	wstring strOrgPatch = wstring( szPath ) + wstring( _T("\\data\\update.pkg" ));
	m_bmUpdateProgress.SetInfo(CString(_T("�����ͷŲ����������Ժ�.....")));
	wstring strDataPath = wstring( szPath ) + wstring( _T("\\data") );
	CreateMultipleDirectory( CString( strDataPath.c_str() ) );

	//�жϴ��̿ռ��Ƿ��㹻
	wstring DiskName = strOrgPatch.substr(0,strOrgPatch.find_first_of(_T("\\")));
	__int64 i64FreeBytes;
	BOOL bResult = GetDiskFreeSpaceEx (DiskName.c_str(), 
		NULL,
		NULL,
		(PULARGE_INTEGER)&i64FreeBytes);
	uint64 uPackSize = m_SelfExtractor.GetAllFileSize();
	if (uPackSize > i64FreeBytes )
	{
		MessageBox( _T("���̿ռ䲻��!"), _T("��ʾ"), MB_OK);
		EndDialog(TRUE);
	}
	CSEFileInfo* info = m_SelfExtractor.GetItem(_T("update.pkg"));
	if (info == NULL )
	{
		MessageBox(_T("���²�������"),_T("��ʾ"),MB_OK);
		EndDialog(TRUE);
	}
	CPatchFile patchFile;
	GameFilePackage OrgPackage;
	//
	if(patchFile.open(m_SelfExtractor.GetThisFileName(),info->GetFileOffset(),info->GetFileSize())&&
		OrgPackage.open( strOrgPatch.c_str(), false ) )
	{
		size_t nFileNum = patchFile.getNumFiles();

		m_bmUpdateProgress.SetInfo(CString(_T("���ڸ��¿ͻ���")));
		m_bmUpdateProgress.SetRange(0, nFileNum);
		m_bmUpdateProgress.SetPos(0);
		for (size_t i = 0; i < nFileNum ; i ++ )
		{
			const PkgFileInfo& fileInfo = patchFile.getFileInfo(i);

			float f = (float)(i + 1)/nFileNum*100;

			CString strInfo = _T("���ڸ��¿ͻ���:");
			strInfo.AppendFormat(_T(" %d%%"), (int)f );
			m_bmUpdateProgress.SetInfo(strInfo);
			m_bmUpdateProgress.SetPos(i);

			strInfo = _T("���ڸ����ļ�:");
			strInfo += CString( fileInfo.szPath );
			GetDlgItem( IDC_STATIC )->SetWindowText( strInfo );
			Invalidate(TRUE);
			UpdateWindow();
			
			TCHAR buf[256] = { _T('\0') };
			MultiByteToWideChar(
				936,
				MB_PRECOMPOSED,
				fileInfo.szPath,
				-1,
				buf,
				256 );

			if( !isInUnCompressPath( wstring( buf ) ) )
			{
				if( buf == _T("version.data") )
				{
					continue;
				}
				wstring strPath( buf );
				if( wstring( buf ).find_last_of(_T("\\")) != -1 )
				{
					strPath = wstring( szPath ) + _T("\\") + strPath;
					wstring createPath = strPath.substr( 0, strPath.find_last_of(_T("\\")) );
					CreateMultipleDirectory( CString( createPath.c_str() ) );
				}
				wstring unComPath = wstring( szPath ) + _T("\\") + wstring( buf ) ;
				FILE* fp = _wfopen( unComPath.c_str() , _T("wb") );
				if( fp != NULL )
				{
					long	len		= fileInfo.packsize;
					char*	pbuf	= new char[len];
					if (!patchFile.read(i, pbuf))
					{
						patchFile.close();
						OrgPackage.close();
						MessageBox(_T("��ѹ����!"),_T("��ʾ"),MB_OK);
						EndDialog(TRUE);
					}
					if (fileInfo.flags&1)	//�Ǿ���ѹ����
					{
						uLongf destlen = fileInfo.originsize;
						uchar *pdestbuf = new uchar[fileInfo.originsize];
						if(uncompress(pdestbuf, &destlen,(uchar*)pbuf, fileInfo.packsize) != Z_OK || destlen!=fileInfo.originsize)
						{
							MessageBox(_T("��ѹ����"),_T("��ʾ"),MB_OK);
							EndDialog( TRUE );
						}
						fwrite( pdestbuf, destlen, 1, fp );
						delete pdestbuf;
					}
					else
					{
						fwrite( pbuf, len, 1, fp );
					}
					fclose( fp );
					delete pbuf;
				}else
				{
					patchFile.close();
					OrgPackage.close();
					MessageBox( _T("��ر���Ϸ�ͻ�����ִ�и��³���"), _T("��ʾ"), MB_OK );
					EndDialog( TRUE );
				}
			}
			else
			{
				// �������ļ�ֱ�Ӵ����update.pkg����
				long	len		= fileInfo.packsize;
				char*	pbuf	= new char[len];
				patchFile.read( i, pbuf );
				OrgPackage.appendOrReplaceFile( 
					fileInfo.key1,
					fileInfo.key2, 
					1, 
					pbuf,
					(fileInfo.flags&1)?fileInfo.originsize:fileInfo.packsize,
					(fileInfo.flags&1)?fileInfo.packsize:0);
				delete  pbuf;
			}
		}
	}
	patchFile.close();
	OrgPackage.close();
	// ��󿽱��汾��Ϣ�ļ�
	DeleteFile( oldVerFile.c_str() );
	::CopyFile( newVerFile.c_str(), oldVerFile.c_str(), true );
	DeleteFile(newVerFile.c_str());

	/*
	//������װ����ͳ��
	HINTERNET it = InternetOpen(NULL,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,NULL);
	if( it != NULL )
	{
 		char szBuf[256];
 		sprintf( szBuf,"http://ljy.the9.com/count/manual_patch_setup_ok.html/?version=%s",strNewVer.c_str() );
 		HINTERNET it2 = InternetOpenUrlA( it,szBuf,NULL,0,INTERNET_FLAG_NO_CACHE_WRITE,NULL );
 		InternetCloseHandle(it2);
	}
	InternetCloseHandle(it);
	*/

	m_bmUpdateProgress.SetPos( patchFile.getNumFiles() );
	m_bmUpdateProgress.SetInfo(CString(_T("�ͻ��˸������")));
	GetDlgItem( IDC_BUTTON1 )->SetWindowText( _T("���") );
	Invalidate(TRUE);
	UpdateWindow();
}

void CExtractorDlg::AddCallBack(void* CallbackData, void *userData)
{
	CSEFileInfo* pData = static_cast<CSEFileInfo*>(CallbackData);
}

BEGIN_MESSAGE_MAP(CExtractorDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CExtractorDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CExtractorDlg message handlers

BOOL CExtractorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	startUpdate();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CExtractorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CExtractorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CExtractorDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	EndDialog(TRUE);
}

//��ѹ�ļ�
BOOL CExtractorDlg::ExtractorFile(LPCTSTR szFileName,LPCTSTR szPatchPath)
{
	if (szFileName == NULL)
	{
		return FALSE;
	}
	for (int i = 0; i < m_SelfExtractor.GetFileCount(); i++ )
	{
		CSEFileInfo info = m_SelfExtractor.GetItem(i);
		if (info.GetFilename().CompareNoCase(szFileName)==0 )
		{
			m_SelfExtractor.Extract(i,CString(szPatchPath));
			return TRUE;
		}
	}
	return FALSE;
}