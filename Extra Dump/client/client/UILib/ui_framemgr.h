#pragma once
#include "ui_cursor.h"
#include "ui_gameui.h"
#include "ui_scriptfunc.h"
#include <hash_map>
#include <vector>

// 一个表情的相关数据
struct FaceData
{
	int		m_nCode;			// 表情代码，-1表示没有这个表情
	int		m_nStartIdx;		// 在贴图上的开始位置编号
	int		m_nFrameNum;		// 表情帧数
	int		m_nFrameInterval;	// 帧间隔
};

// 表情编号到表情数据的映射
class IFaceCodeMap
{
public:
	// 根据表情编码取得表情数据，返回值如要长期保存需要复制
	virtual FaceData* GetFaceData(int nCode) = 0;
};

class RFaceCodeMap : public IFaceCodeMap
{
private:
	FaceData		m_faceData[1000];

public:
	RFaceCodeMap();
	bool Init(const char* szFileName);
	virtual FaceData* GetFaceData(int nCode);
};

// 表情分为3级数据
// 1级，是包含所有表情数据的整张贴图的数据
// 2级，是所有表情共享的数据，可作为类的静态成员
// 3级，是各个表情单独享有的数据，作为常规数据成员变量
// 一个表情的相关数据
struct PictureData
{
	int		m_nCode;			// 表情代码，-1表示没有这个表情
	int		m_nStartIdx;		// 在贴图上的开始位置编号
	int		m_nFrameNum;		// 表情帧数
	int		m_nFrameInterval;	// 帧间隔
	
	int		m_nTexLeft;			// 图片左边坐标
	int		m_nTexTop;			// 图片上部坐标
	int		m_nTexWidth;		// 图片宽度
	int		m_nTexHeight;		// 图片高度
};

class RPictureCodeMap
{
private:
	std::string					m_configFilePath;	// 图片符号关键数据所在的文件
	std::string					m_resourceFilePath;	// 图片符号美术资源所在的文件
	Ogre::HUIRES				m_hTexRes;			// 图片贴图句柄

	unsigned int				m_nTexWidth;		// 包含所有图片符号的纹理的宽度
	unsigned int				m_nTexHeight;		// 包含所有图片符号的纹理的高度
	float						m_fUUnit;			// 贴图上一个像素的纹理宽度
	float						m_fVUnit;			// 贴图上一个像素的纹理宽度
	std::map<int, PictureData>	m_pictureData;		// 图片数据

public:
	RPictureCodeMap()
	{}
	
	bool Init();
	bool Init( const char* szFileName );
	
	void SetTexRes( Ogre::HUIRES hres )
	{
		m_hTexRes = hres;
	}

	Ogre::HUIRES GetTexRes( )
	{
		return m_hTexRes;
	}
	
	void SetUVUnit( float fUUnit, float fVUnit )
	{
		m_fUUnit = fUUnit;
		m_fVUnit = fVUnit;
	}
	
	float GetUUnit()
	{
		return m_fUUnit;
	}

	float GetVUnit()
	{
		return m_fVUnit;
	}

	void SetTexSize( unsigned int nWidth, unsigned int nHeight )
	{
		m_nTexWidth		= nWidth;
		m_nTexHeight	= nHeight;
	}
	
	unsigned int GetTexWidth( )
	{
		return m_nTexWidth;
	}

	unsigned int GetTexHeight( )
	{
		return m_nTexHeight;
	}

	void SetResourcePath( const char* pszFileName )
	{
		m_resourceFilePath = pszFileName;
	}

	void SetResourcePath( std::string strFileName )
	{
		m_resourceFilePath = strFileName;
	}

	const char* GetResourcePath( )
	{
		return m_resourceFilePath.c_str();
	}

	void SetConfigPath( const char* pszFileName )
	{
		m_configFilePath = pszFileName;
	}

	void SetConfigPath( std::string strFileName )
	{
		m_configFilePath = strFileName;
	}

	const char* GetConfigPath( )
	{
		return m_configFilePath.c_str();
	}

	PictureData* GetPictureData( int nCode );
};

struct UIFont
{
	std::string name;
	std::string type;
	unsigned int height;
	Ogre::HFONT h;
	float scale; //位图字体专用，其他情况都为1
	unsigned int extType;
};

struct tagPopWin 
{
	unsigned int	m_nDuration;
	Ogre::Point2D	m_PopOffset;
	Ogre::Point2D	m_TexOffset;
	bool			m_bBottomTex;
	std::string		m_strArrowTexName;
};

enum UIObjCreateMode
{
	UIOBJ_CREATE_REAL,
	UIOBJ_CREATE_TEMPLATE,
	UIOBJ_CREATE_TEMPLATECHILD
};

class FrameManager {//tolua_export
public:
	FrameManager();
	~FrameManager();

	void	SetScreenWidth(int width)
	{ 
		m_iScreenWidth = width; 
	}

	int		GetScreenWidth()
	{ 
		return m_iScreenWidth; 
	}

	void	SetScreenHeight(int height)
	{ 
		m_iScreenHeight = height;
	}

	int		GetScreenHeight()
	{
		return m_iScreenHeight; 
	}

	void	SetScreenScaleX(float scale)
	{ 
		m_fScaleX = scale; 
	}

	float	GetScreenScaleX()
	{ 
		return m_fScaleX*m_fAllScale; 
	}

	void	SetScreenScaleY(float scale)
	{ 
		m_fScaleY = scale; 
	}

	float	GetScreenScaleY()
	{ 
		return m_fScaleY*m_fAllScale ; 
	}

	unsigned int GetCalAbsRectFrame();
	UIObject* FindTexture(const std::string &name,const std::string &Texturename);

	//tolua_begin
	UIObject*	FindLayoutFrame(const std::string &name);

	int  GetEditMode()
	{ 
		return m_iEditMode; 
	}
	void	EnableAccelerator(bool enable);
		
	//tolua_end

	void	UpdateChangedFrames();
	void	Render();
	float	GetAllSelfScale();
	void	UpdateAllSelfScale( float fScale );
	void	Update(float deltatime);

	int		OnInputMessage(const Ogre::InputEvent &event);

	//type_name: frame, button, fontstring
	UIObject *CreateObject(const char *type_name, const char *name, const char *template_name=NULL);
	void RegisterObject(UIObject *pobj);

	bool CreateLuaTable(Frame *pFrame);
	bool CreateObjLuaTable(UIObject *pobj, const char *name);
	
	void AddReCalPosFrame( LayoutFrame *pframe );
	void AddRootFrame(Frame *pframe);
	void GetFramePoint(FRAMEPOINT_T pt, int &x, int &y);

	Frame*	FindUIObjectOnPoint(int mousex, int mousey, bool bLevelChanged=true);
	void	FindUIObjectOnPoint( std::vector <Frame*>& mouseOnFrames, int mousex, int mousey, bool bLevelChanged=true );
	void	FindModalFrame( const char *name,std::vector<Frame *> &vModalFrame );//寻找已打开的模态子窗口
	Frame*	GetRootFrame( Frame *pFrame );//获取当前控件的根窗口

	void	SetFocusFrame(Frame *pframe);
	Frame*	GetFocusFrame();
	void	ClearFocus();

	void	AddGameFont(UIFont font);

	void	AddGameAccels(AccelItem accel);

	void	AddDrawItems(LayoutFrame *pObj);

	void	SendEvent(const char *ClientEvent);

	Frame*	GetUIClientFrame();

	void	SetEditMode(int mode){ m_iEditMode = mode; }

	void	SaveXMLFile() {};

	UIObject*	InternalCreateUIObj(const char *type_name, const char *name);

	void	UpdateGameFont(double scale);
	
	// 删除一个聊天泡泡
	void	delPopWindow( const char* pszRoleName );
	// 添加一个聊天泡泡对话
	void	addPopWindow( const char* szPopBack, const char* szRoleName, const char* szText, 
							int nPopOffsetX, int nPopOffsetY, int nDuration, const char* szArrowTexName = "",
								bool bShowTex = false, int nTexOffsetX = 0, int nTexOffsetY = 0 );
	// 刷新泡泡框位置
	void	updatePopWinPos( const char* szRoleName, int x, int y, float fAlpha=1.0f );
	void	updatePopWinPos( const char* szRoleName, float x,float y, float fAlpha=1.0f );
	// 删除某个UI
	void	deleteBindingFrame( const char* pszFrameName );
	void	hideFrame( const char* pszFrameName );
	// 刷新头顶绑定的UI
	void	updateHeadBindingFrame( const char* pszFrameName, int x, int y, int nWidth, int mHeight );
	// 隐藏泡泡框
	void	hidePopWin( const char* szRoleName );
	void	showPopWin( const char* szRoleName );
	
	void	setWndHandle(void* p) { m_wndhandle = p; } 
	void*	getWndHandle() { return m_wndhandle; }

	void	setCursor(const char* name);
	void	setUiCursor(const char* name);

	void	setPictureCodeMap( RPictureCodeMap* pPictureBuffer )
	{
		m_pPictureBuffer = pPictureBuffer;
	}

	RPictureCodeMap*	getPictureCodeMap()
	{
		return m_pPictureBuffer;
	}

	void*	getImeContext();
	void*	getFontInfoByFontType( const char* pszFontName );

	UIObject*	getTemplateObject( std::string template_name );

	void	SetBeginDragFrame(Frame *frame = NULL);
	const UIFont* getUIFontByName( const char* pszFontName );
	const UIFont& getUIFontByIndex( int nIndex );
	int getUIFontIndexByName( const char* pszFontName );
	void AddReCalFrame( LayoutFrame* pFrame );

public:
	float m_fAllScale;
	unsigned int m_CalAbsRectFrame;

	int m_iScreenWidth;
	int m_iScreenHeight;

	float m_fScaleX;
	float m_fScaleY;

	int m_iCurMaxFrmLevel;

	std::map<std::string, UIObject *> m_ObjectTable;
	std::map<std::string, UIObject *> m_TemplateTable;

	std::vector<Frame *> m_RootFrames;
	std::vector<LayoutFrame *> m_DrawItems;

	Frame*		m_pFocusFrame;
	Frame*		m_pDragFrame;

	UICursor *m_pCurCursor;

	std::vector<UIFont> m_HMAYFONT;		// 界面字体数组
	std::vector<int> m_FONTHEIGHT;		// 界面字体标准大小

	std::vector<AccelItem> m_Accels;	// 快捷键数组

	// 保存所有lua文件
	std::vector<std::string> m_LuaFile;
	std::string m_TocFileName;

	bool	m_bUpdate;	// 控件位置有改变，需要重新计算
	bool	m_bUpdatedGameFont;

	std::vector<LayoutFrame *> m_ReCalPosFrames;	// 控件位置需要重新计算的frame
	std::vector<LayoutFrame *> m_ReDrawFrames;
	
	std::map<std::string, tagPopWin> m_PopWinMap;	// 聊天泡泡框

	unsigned int	m_lastBeginDragTime;
	int				m_lastBeginDragX;
	int				m_lastBeginDragY;
	Frame *		m_lastBeginDragFrame;

	bool m_bRenderUI;
public:
	// 表情贴图相关
	RFaceCodeMap*			m_pCodeMap;				// 表情编码到表情数据的映射对象
	std::string				m_CodeMapName;			// 表情符号关键数据所在的文件
	std::string				m_PopofaceTexName;		// 表情符号美术资源所在的文件
	Ogre::HUIRES			m_hTexRes;				// 表情贴图句柄

	int						m_nFaceWidth;			// 此为一个表情的宽度
	int						m_nFaceHeight;			// 此为一个表情的高度
	unsigned int			m_nFaceTexWidth;		// 包含所有表情符号的纹理的宽度
	unsigned int			m_nFaceTexHeight;		// 包含所有表情符号的纹理的高度
	float					m_fUWidth;				// 一个表情符号宽度占其整张表情纹理宽度的百分比(即为其纹理U坐标值)
	float					m_fVWidth;				// 一个表情符号高度占其整张表情纹理高度的百分比(即为其纹理V坐标值)

	int						m_nFaceNumX;			// 贴图水平方向表情数目
	int						m_nFaceNumY;			// 贴图垂直方向表情数目
	float					m_fUUnit;				// 贴图上一个像素的纹理宽度
	float					m_fVUnit;				// 贴图上一个像素的纹理宽度

	lua_State*				m_LS;

	void InitFaceTexture( int width, int height );
	void InitPictureTexture( int width, int height );

	// 图片纹理存取器
	RPictureCodeMap*		m_pPictureBuffer;

	bool					m_bMouseClickDown;	//标记鼠标是否在按下状态

protected:
	// 表情分为3级数据
	// 1级，是包含所有表情数据的整张贴图的数据
	// 2级，是所有表情共享的数据，可作为类的静态成员
	// 3级，是各个表情单独享有的数据，作为常规数据成员变量

	// 快捷键相关
	int OnAccelerator(const Ogre::InputEvent &event);
	int ProcessAccelerator(const AccelItem& accel, bool keydown_or_keyup);

	std::vector<AccelItem>& GetFrmAccels();
	//Frame *FindFrameForAccel( const AccelItem& accel);
	bool IsAccelIn(const AccelItem& accel);

private:
	Frame *m_LastFrame;			//鼠标上一次指向的控件

	int m_iEditMode; // 编辑模式
	void* m_wndhandle;
	void* m_hIme;
	bool	m_enableAccel;
};//tolua_export

struct stEventFrameArray
{
	std::vector<Frame *> EventFrameArray;
};

struct tagTexUV
{
	int x;
	int y;
	int w;
	int h;
};

extern stdext::hash_map<std::string, stEventFrameArray> EventMap;

//class LuaStackBalance
//{
//public:
//	LuaStackBalance(lua_State *ls){ m_ls = ls; topindex = lua_gettop(m_ls); };
//	~LuaStackBalance(){lua_settop(m_ls, topindex);};
//public:
//	lua_State *m_ls;
//	int topindex;
//};

enum
{
	ACCEL_CTRL = 0x100,
	ACCEL_SHIFT = 0x200,
	ACCEL_ALT = 0x400,
};

//tolua_begin
bool	IsCtrlPress(int code);
bool	IsShiftPress(int code);
bool	IsAltPress(int code);
int		GetControlKeyCode(bool ctrl, bool shift, bool alt);
//tolua_end

extern FrameManager *g_pFrameMgr;
