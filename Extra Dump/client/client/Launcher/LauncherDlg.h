// LauncherDlg.h : header file
//

#pragma once

#include "afxcmn.h"
#include "afxwin.h"
#include "tinyxml.h"
#include "tinystr.h"
#include "util/utils.h"
#include "curl/Downloader.h"
#include "util/Patcher.h"
#include "util/Utils.h"
#include "DirServeSelector.h"
#include "VersionVerifier.h"
#include "SelfExtractor.h"
#include <string>
#include <map>
#include <list>
#include <atlimage.h>
#include "webbrowser.h"
#include "BtnSt.h"
#include "LauncherChangeLogLayerDlg.h"

#define UPDATE_LAUNCHER_NAME "Launcher.exe"
#define HELPER_UPDATE_LAUNCHER_NAME	"LauncherHelper.exe"
#define SMART_DOWNLOADER_NAME	"SmartDownloader.exe"
//#define HELPER_UPDATE_PATCH_NAME "Patch"
#define HELPER_UPDATE_TMP_PKG_NAME "data\\update_tmp.pkg"

const int CUSTOM_BUTTON_MSG		= 0xDEAD;
const TCHAR pszLoginGameInfo[]	= TEXT( "login.ini" );

enum
{
	UI_COMMAND_CONNECT_DISERVE_SUCC,	// ����Dir serve�ɹ� 
	UI_COMMAND_POP_AREA_SELECT,			// ����������ѡ���
	
	UI_COMMAND_NEED_UPDATE,				// �汾��Ҫ����
	UI_COMMAND_OLD_VERSION,				// �汾̫��,�޷�����
	UI_COMMAND_LATEST_VERSION,			// ���°汾
	
	UI_COMMAND_BEGIN_DOWNLOAD,			// ��ʼ����
	UI_COMMAND_CANCEL_DOWNLOAD,			// ȡ������
	UI_COMMAND_END_DOWNLOAD,			// �������
	UI_COMMAND_ERROR_UPDATE,			// ��װ������ʧ��
	UI_COMMAND_UPDATE_DOWNLOAD_ERROR,	// ����ʧ��ʱ�򱨴�

	UI_COMMAND_ENTER_GAME,				// ������Ϸ
	UI_COMMAND_RESET_EFFECT,			// ������Ϸ����
	UI_COMMAND_CLOSE,					// �رմ���
	UI_COMMAND_MINIMIZE,				// ��С������

	UI_COMMAND_FAIL_DIR,				// ����Dir serveʧ��
	UI_COMMAND_FAIL_VER,				// ����version serveʧ��
	
	UI_COMMAND_RECHECK_VERSION,			// �ٴν��а汾���
	UI_COMMAND_OPEN_UPDATE_URL,			// ��URL

	UI_COMMAND_BEGIN_PATCH,				// ��ʼ��װ����
	UI_COMMAND_UPDATE_PATCH_SUCCESS,	// ������װ�ɹ�
	
	UI_COMMAND_COPYFILE_PROGRESS,			// ��ʼ�����ļ�

	UI_COMMAND_DOWNLOAD_ATTACH,			// ����attach.pkg

	UI_COMMAND_END
};

enum MsgCallbacks
{                                   // lParam������
	CbUpdateStartup,                // ��ʼ����
	CbUpdateProgress,               // ���½���(0-100)
	CbUpdateDiskFull,               // ����ʱ��������
	CbUpdateDone,                   // �������
};

enum StatusCtrl 
{
	scStatus,
	scVersion,
};

enum
{
	UI_NET_VERSION_DISPATCH,
	UI_NET_DIR_DISPATCH,
	UI_NET_CONNECT,
};

enum 
{
	WM_DOWNLOADER = WM_USER + 1,
	WM_SHOWTASK,
	WM_PATCH,
};

struct LauncherNetConfigData
{
	std::string m_strIp;
	unsigned short m_nPort;
	std::vector<DirServerData> m_vData;
};

struct GameNetCfg 
{
	LauncherNetConfigData		m_verConfig;	//version server
	std::vector<LauncherNetConfigData> m_vDirConfig;	//dir server
	std::string					m_DirUrl;
	std::string					m_VersionUrl;
};

struct ExpServerCfg
{
	int expFlag;		//0 ����ʾ��1��ʽ�� 2�����,
	string path;
};

struct CopyFileInfo
{
	CString		path;
	CString		destPath;
	__int64		size;
};

// CLauncherDlg dialog
class CLauncherDlg : public CDialog
{
// Construction
public:
	CLauncherDlg(CWnd* pParent = NULL);	// standard constructor
	~CLauncherDlg();

// Dialog Data
	enum { IDD = IDD_LAUNCHER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

public:
	static const GameNetCfg& getNetConfig(){return m_NetCfg; };

// Implementation 
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL	OnInitDialog();
	afx_msg void	OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void	OnClose();
	afx_msg void	OnTimer(UINT_PTR nIDEvent);
	afx_msg void	OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg	LRESULT	OnSetDownloadStatus(WPARAM wParam, LPARAM lParam);
	afx_msg	LRESULT	OnSetPatchStatus(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL	OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()

	afx_msg BOOL	PreTranslateMessage(MSG* pMsg) sealed;

public:
	afx_msg void	OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL	OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void	OnSelfDownloadClick();
	afx_msg void	OnUpdateClick();
	afx_msg void	OnEnterGameClick();
	afx_msg void	OnResetEffectClick();
	afx_msg void	OnRegisterClick();
	afx_msg void	OnUpdateLogClick();
	afx_msg void	OnGuildClick();
	afx_msg void	OnCustomerServiceClick();
	afx_msg void	OnOfficeBbsClick();
	afx_msg void	OnOfficeSiteClick();
	afx_msg void	onCanelUpdateClick();
	afx_msg void	onCloseClick();
	afx_msg void	onMinimizeClick();
	afx_msg void	onExpServerClick();
	afx_msg void	onNormalServerClick();
	afx_msg void	onActiveClick();

	// Ӧ�õ�Ŀ��汾��֧������������������
	void			ApplyPatch( const char* szEXEPath );
	// ���ز����ļ�������ļ����ڣ����ж�md5�Ƿ�һ�£���һ���������ļ����ļ������ھ���������
	bool			DownloadPatch(const char *url, const char *checksum);
	void			SetUrl( const char* pszUrl );
	void			SetLocalPath( const char* szPath );
	const char*		GetPatchFilePath() const;
	// �Ը���Launcher
	bool			NeedSelfUpdateLauncher();
	// �Ը���
	void			SelfUpdateLauncher();

	void			CopyDirFiles();

	void			CalcCopyFileSize( CString srcDir, CString destDir );

	void			DoFinishWork();

	static unsigned char*	getMetaBuf() {return m_pCSMetaBuf; }

	CDownloader*	GetDownload() {return m_pDonwloader;};
private:
	// ����DirServe
	void			ConnectDirServe();
	// ����VersionServe
	void			ConnectVersionServe();

	// ���ý�����ʾ������
	void			InitGuiStyle();

	bool			loadLocalCfg();

	BOOL			sendDumpFile();

	void			CheckExpServer();

	void			CreateNewClient();

	void			WriteSrcServerList();

	void			WriteDestServerList();

	void			CheckSingleInstance();

public:
	static DirServeSelector*	m_pdirServeSelector;
	static VersionVerifier*		m_pversionVerifier;
	static GameNetCfg			m_NetCfg;
	static unsigned char*		m_pCSMetaBuf;
	
private:

	CDownloader*			m_pDonwloader;				// curl������
	CSelfExtractor			m_SelfExtractor;			// ��ѹ���°���
	int						m_nLauncherVersion;			// ��½���İ汾��
	
private:
	int					m_nVersion;					// ��ǰ�ͻ��˵İ汾��

	char                m_szPatchFile[MAX_PATH];	// ��ǰ���µĲ����ļ���

	void OpenUrl( const char* url );

private:	
	// �ֶ�������ʾ��ť
	CImage				    m_imgSelfDownload[3];
	CButtonST				m_btnSelfDownload;

	// ���°�ť
	CImage				    m_imgUpdate[3];
	CButtonST				m_btnUpdate;

	// ������Ϸ��ť
	CImage					m_imgEnterGame[3];
	CButtonST			    m_btnEnterGame;

	// ȡ�����°�ť
	CImage				    m_imgCancelUpdate[3];
	CButtonST			    m_btnCancelUpdate;
	
	// ע�ᰴť
	CImage				    m_imgRegister[3];
	CButtonST				m_btnRegister;
	
	// ������־
	CImage				    m_imgUpdateLog[3];
	CButtonST				m_btnUpdateLog;

	// �ͷ���ť
	CImage				    m_imgCustomerService[3];
	CButtonST			    m_btnCustomerService;

	// ����
	CImage				    m_imgOfficeSite[3];
	CButtonST			    m_btnOfficeSite;
	
	//�ٷ�bbs
	CImage					m_imgOfficebbs[3];
	CButtonST			    m_btnOfficebbs;

	//����ָ��
	CImage					m_imgGuider[3];
	CButtonST			    m_btnGuider;

	// ��������
	CImage				    m_imgResetEffect[3];
	CButtonST			    m_btnResetEffect;

	// �رհ�ť
	CImage				    m_imgClose[3];
	CButtonST			    m_btnClose;

	// ��С����ť
	CImage				    m_imgMinimize[3];
	CButtonST			    m_btnMinimize;

	//�����
	CImage					m_imgExpServer[3];
	CButtonST				m_btnExpServer;

	//��ʽ��
	CImage					m_imgNormalServer[3];
	CButtonST				m_btnNormalServer;

	// ����
	CImage					m_imgActive[3];
	CButtonST				m_btnActive;
	
	CImage					m_bkgImg;
	LauncherChangeLogLayerDlg m_dlgLog;

private:
	long					m_tmDlNotify;				// �ϴ���������֪ͨ����ʱ��
	long					m_tmDlStartup;				// ��ʼ���ص�ʱ��
	unsigned int			m_nDlBytes;					// ��ǰ�����ֽ���
	bool					m_bSlowNotifyed;			// �Ƿ���ʾ�������ٶȹ���
	char					m_szSfxPatchUrl[MAX_PATH];	// �ֶ����²�������url
	string					m_strLauncherUrl;
	string					m_strHomePageUrl;
	string					m_strRegisterUrl;
	
	unsigned int			m_totalLength;				// �ܹ���Ҫ���صĴ�С
	// ���ؽ�����
	CClient_Image		    m_imgDowndloadProgress;

	// ���½�����
	CClient_Image		    m_imgUpdateProgress;
	
public:
	CWebBrowser2			m_ctrlWeb;
	bool					m_bDownloading;
	bool					m_bUpdatingPatch;
	bool					m_bDownloadAttach;
	bool					m_bNeedUpdateSelf;

	bool					m_bConnectDirServer;	//�Ƿ�������dir
	bool					m_SmallClient; // �Ƿ�΢��
	CString					m_SmallClientUrl;
	CString					m_SmallClientAttachMD5;

	ExpServerCfg			m_ExpServerCfg;

	__int64					m_TotalCopySize;
	__int64					m_CopyedSize;
	bool					m_bIsCopyFile;
	std::list<CopyFileInfo>	m_lstCopyFiles;

	CClient_Progress	    m_downloadBarProgress;
	CClient_Progress	    m_updateBarProgress;
private:
	CString			m_strSpeed;
	CString			m_strTimeLeft;
	CString			m_strFileSize;
	int				m_nLastIndex;
	bool			m_bUpdated;
	string			m_strPatchVer;
	HANDLE			m_hInstMutex;

public:
	DECLARE_EVENTSINK_MAP()
	void DocumentCompleteExplorer1(LPDISPATCH pDisp, VARIANT* URL);
	void OnDocumentNewWindow1( LPDISPATCH* ppDisp, BOOL* cancel, unsigned long dwFlags, LPCTSTR bstrUrlContext, LPCTSTR bstrUrl );
};

class CArkFileDialog : public CFileDialog
{
	DECLARE_DYNAMIC(CArkFileDialog)
	CString			m_FloderName;

public:
	CArkFileDialog(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);
	virtual INT_PTR DoModal( CString& RootPath );

protected:
	DECLARE_MESSAGE_MAP()
	virtual void OnFolderChange();
};
