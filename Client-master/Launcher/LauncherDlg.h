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
	UI_COMMAND_CONNECT_DISERVE_SUCC,	// 连接Dir serve成功 
	UI_COMMAND_POP_AREA_SELECT,			// 弹出服务器选择框
	
	UI_COMMAND_NEED_UPDATE,				// 版本需要更新
	UI_COMMAND_OLD_VERSION,				// 版本太老,无法更新
	UI_COMMAND_LATEST_VERSION,			// 最新版本
	
	UI_COMMAND_BEGIN_DOWNLOAD,			// 开始下载
	UI_COMMAND_CANCEL_DOWNLOAD,			// 取消下载
	UI_COMMAND_END_DOWNLOAD,			// 下载完毕
	UI_COMMAND_ERROR_UPDATE,			// 安装补丁包失败
	UI_COMMAND_UPDATE_DOWNLOAD_ERROR,	// 下载失败时候报错

	UI_COMMAND_ENTER_GAME,				// 进入游戏
	UI_COMMAND_RESET_EFFECT,			// 设置游戏画面
	UI_COMMAND_CLOSE,					// 关闭窗口
	UI_COMMAND_MINIMIZE,				// 最小化窗口

	UI_COMMAND_FAIL_DIR,				// 连接Dir serve失败
	UI_COMMAND_FAIL_VER,				// 连接version serve失败
	
	UI_COMMAND_RECHECK_VERSION,			// 再次进行版本检测
	UI_COMMAND_OPEN_UPDATE_URL,			// 打开URL

	UI_COMMAND_BEGIN_PATCH,				// 开始安装补丁
	UI_COMMAND_UPDATE_PATCH_SUCCESS,	// 补丁安装成功
	
	UI_COMMAND_COPYFILE_PROGRESS,			// 开始拷贝文件

	UI_COMMAND_DOWNLOAD_ATTACH,			// 下载attach.pkg

	UI_COMMAND_END
};

enum MsgCallbacks
{                                   // lParam的意义
	CbUpdateStartup,                // 开始更新
	CbUpdateProgress,               // 更新进度(0-100)
	CbUpdateDiskFull,               // 更新时磁盘已满
	CbUpdateDone,                   // 更新完成
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
	int expFlag;		//0 不显示，1正式服 2体验服,
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

	// 应用到目标版本，支持升级、降级。。。
	void			ApplyPatch( const char* szEXEPath );
	// 下载补丁文件，如果文件存在，先判断md5是否一致，不一致则续传文件，文件不存在就重新下载
	bool			DownloadPatch(const char *url, const char *checksum);
	void			SetUrl( const char* pszUrl );
	void			SetLocalPath( const char* szPath );
	const char*		GetPatchFilePath() const;
	// 自更新Launcher
	bool			NeedSelfUpdateLauncher();
	// 自更新
	void			SelfUpdateLauncher();

	void			CopyDirFiles();

	void			CalcCopyFileSize( CString srcDir, CString destDir );

	void			DoFinishWork();

	static unsigned char*	getMetaBuf() {return m_pCSMetaBuf; }

	CDownloader*	GetDownload() {return m_pDonwloader;};
private:
	// 连接DirServe
	void			ConnectDirServe();
	// 连接VersionServe
	void			ConnectVersionServe();

	// 设置界面显示的内容
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

	CDownloader*			m_pDonwloader;				// curl下载类
	CSelfExtractor			m_SelfExtractor;			// 解压更新包类
	int						m_nLauncherVersion;			// 登陆器的版本号
	
private:
	int					m_nVersion;					// 当前客户端的版本号

	char                m_szPatchFile[MAX_PATH];	// 当前更新的补丁文件名

	void OpenUrl( const char* url );

private:	
	// 手动下载提示按钮
	CImage				    m_imgSelfDownload[3];
	CButtonST				m_btnSelfDownload;

	// 更新按钮
	CImage				    m_imgUpdate[3];
	CButtonST				m_btnUpdate;

	// 进入游戏按钮
	CImage					m_imgEnterGame[3];
	CButtonST			    m_btnEnterGame;

	// 取消更新按钮
	CImage				    m_imgCancelUpdate[3];
	CButtonST			    m_btnCancelUpdate;
	
	// 注册按钮
	CImage				    m_imgRegister[3];
	CButtonST				m_btnRegister;
	
	// 更新日志
	CImage				    m_imgUpdateLog[3];
	CButtonST				m_btnUpdateLog;

	// 客服按钮
	CImage				    m_imgCustomerService[3];
	CButtonST			    m_btnCustomerService;

	// 官网
	CImage				    m_imgOfficeSite[3];
	CButtonST			    m_btnOfficeSite;
	
	//官方bbs
	CImage					m_imgOfficebbs[3];
	CButtonST			    m_btnOfficebbs;

	//新手指引
	CImage					m_imgGuider[3];
	CButtonST			    m_btnGuider;

	// 画面设置
	CImage				    m_imgResetEffect[3];
	CButtonST			    m_btnResetEffect;

	// 关闭按钮
	CImage				    m_imgClose[3];
	CButtonST			    m_btnClose;

	// 最小化按钮
	CImage				    m_imgMinimize[3];
	CButtonST			    m_btnMinimize;

	//体验服
	CImage					m_imgExpServer[3];
	CButtonST				m_btnExpServer;

	//正式服
	CImage					m_imgNormalServer[3];
	CButtonST				m_btnNormalServer;

	// 激活
	CImage					m_imgActive[3];
	CButtonST				m_btnActive;
	
	CImage					m_bkgImg;
	LauncherChangeLogLayerDlg m_dlgLog;

private:
	long					m_tmDlNotify;				// 上次下载数据通知到达时间
	long					m_tmDlStartup;				// 开始下载的时间
	unsigned int			m_nDlBytes;					// 当前下载字节数
	bool					m_bSlowNotifyed;			// 是否提示过下载速度过慢
	char					m_szSfxPatchUrl[MAX_PATH];	// 手动更新补丁下载url
	string					m_strLauncherUrl;
	string					m_strHomePageUrl;
	string					m_strRegisterUrl;
	
	unsigned int			m_totalLength;				// 总共需要下载的大小
	// 下载进度条
	CClient_Image		    m_imgDowndloadProgress;

	// 更新进度条
	CClient_Image		    m_imgUpdateProgress;
	
public:
	CWebBrowser2			m_ctrlWeb;
	bool					m_bDownloading;
	bool					m_bUpdatingPatch;
	bool					m_bDownloadAttach;
	bool					m_bNeedUpdateSelf;

	bool					m_bConnectDirServer;	//是否在连接dir
	bool					m_SmallClient; // 是否微端
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
