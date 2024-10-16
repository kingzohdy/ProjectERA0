#pragma once
#include "ui_cursor.h"
#include "ui_gameui.h"
#include "ui_scriptfunc.h"
#include <hash_map>
#include <vector>

// һ��������������
struct FaceData
{
	int		m_nCode;			// ������룬-1��ʾû���������
	int		m_nStartIdx;		// ����ͼ�ϵĿ�ʼλ�ñ��
	int		m_nFrameNum;		// ����֡��
	int		m_nFrameInterval;	// ֡���
};

// �����ŵ��������ݵ�ӳ��
class IFaceCodeMap
{
public:
	// ���ݱ������ȡ�ñ������ݣ�����ֵ��Ҫ���ڱ�����Ҫ����
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

// �����Ϊ3������
// 1�����ǰ������б������ݵ�������ͼ������
// 2���������б��鹲������ݣ�����Ϊ��ľ�̬��Ա
// 3�����Ǹ������鵥�����е����ݣ���Ϊ�������ݳ�Ա����
// һ��������������
struct PictureData
{
	int		m_nCode;			// ������룬-1��ʾû���������
	int		m_nStartIdx;		// ����ͼ�ϵĿ�ʼλ�ñ��
	int		m_nFrameNum;		// ����֡��
	int		m_nFrameInterval;	// ֡���
	
	int		m_nTexLeft;			// ͼƬ�������
	int		m_nTexTop;			// ͼƬ�ϲ�����
	int		m_nTexWidth;		// ͼƬ���
	int		m_nTexHeight;		// ͼƬ�߶�
};

class RPictureCodeMap
{
private:
	std::string					m_configFilePath;	// ͼƬ���Źؼ��������ڵ��ļ�
	std::string					m_resourceFilePath;	// ͼƬ����������Դ���ڵ��ļ�
	Ogre::HUIRES				m_hTexRes;			// ͼƬ��ͼ���

	unsigned int				m_nTexWidth;		// ��������ͼƬ���ŵ�����Ŀ��
	unsigned int				m_nTexHeight;		// ��������ͼƬ���ŵ�����ĸ߶�
	float						m_fUUnit;			// ��ͼ��һ�����ص�������
	float						m_fVUnit;			// ��ͼ��һ�����ص�������
	std::map<int, PictureData>	m_pictureData;		// ͼƬ����

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
	float scale; //λͼ����ר�ã����������Ϊ1
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
	void	FindModalFrame( const char *name,std::vector<Frame *> &vModalFrame );//Ѱ���Ѵ򿪵�ģ̬�Ӵ���
	Frame*	GetRootFrame( Frame *pFrame );//��ȡ��ǰ�ؼ��ĸ�����

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
	
	// ɾ��һ����������
	void	delPopWindow( const char* pszRoleName );
	// ���һ���������ݶԻ�
	void	addPopWindow( const char* szPopBack, const char* szRoleName, const char* szText, 
							int nPopOffsetX, int nPopOffsetY, int nDuration, const char* szArrowTexName = "",
								bool bShowTex = false, int nTexOffsetX = 0, int nTexOffsetY = 0 );
	// ˢ�����ݿ�λ��
	void	updatePopWinPos( const char* szRoleName, int x, int y, float fAlpha=1.0f );
	void	updatePopWinPos( const char* szRoleName, float x,float y, float fAlpha=1.0f );
	// ɾ��ĳ��UI
	void	deleteBindingFrame( const char* pszFrameName );
	void	hideFrame( const char* pszFrameName );
	// ˢ��ͷ���󶨵�UI
	void	updateHeadBindingFrame( const char* pszFrameName, int x, int y, int nWidth, int mHeight );
	// �������ݿ�
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

	std::vector<UIFont> m_HMAYFONT;		// ������������
	std::vector<int> m_FONTHEIGHT;		// ���������׼��С

	std::vector<AccelItem> m_Accels;	// ��ݼ�����

	// ��������lua�ļ�
	std::vector<std::string> m_LuaFile;
	std::string m_TocFileName;

	bool	m_bUpdate;	// �ؼ�λ���иı䣬��Ҫ���¼���
	bool	m_bUpdatedGameFont;

	std::vector<LayoutFrame *> m_ReCalPosFrames;	// �ؼ�λ����Ҫ���¼����frame
	std::vector<LayoutFrame *> m_ReDrawFrames;
	
	std::map<std::string, tagPopWin> m_PopWinMap;	// �������ݿ�

	unsigned int	m_lastBeginDragTime;
	int				m_lastBeginDragX;
	int				m_lastBeginDragY;
	Frame *		m_lastBeginDragFrame;

	bool m_bRenderUI;
public:
	// ������ͼ���
	RFaceCodeMap*			m_pCodeMap;				// ������뵽�������ݵ�ӳ�����
	std::string				m_CodeMapName;			// ������Źؼ��������ڵ��ļ�
	std::string				m_PopofaceTexName;		// �������������Դ���ڵ��ļ�
	Ogre::HUIRES			m_hTexRes;				// ������ͼ���

	int						m_nFaceWidth;			// ��Ϊһ������Ŀ��
	int						m_nFaceHeight;			// ��Ϊһ������ĸ߶�
	unsigned int			m_nFaceTexWidth;		// �������б�����ŵ�����Ŀ��
	unsigned int			m_nFaceTexHeight;		// �������б�����ŵ�����ĸ߶�
	float					m_fUWidth;				// һ��������ſ��ռ�����ű��������ȵİٷֱ�(��Ϊ������U����ֵ)
	float					m_fVWidth;				// һ��������Ÿ߶�ռ�����ű�������߶ȵİٷֱ�(��Ϊ������V����ֵ)

	int						m_nFaceNumX;			// ��ͼˮƽ���������Ŀ
	int						m_nFaceNumY;			// ��ͼ��ֱ���������Ŀ
	float					m_fUUnit;				// ��ͼ��һ�����ص�������
	float					m_fVUnit;				// ��ͼ��һ�����ص�������

	lua_State*				m_LS;

	void InitFaceTexture( int width, int height );
	void InitPictureTexture( int width, int height );

	// ͼƬ�����ȡ��
	RPictureCodeMap*		m_pPictureBuffer;

	bool					m_bMouseClickDown;	//�������Ƿ��ڰ���״̬

protected:
	// �����Ϊ3������
	// 1�����ǰ������б������ݵ�������ͼ������
	// 2���������б��鹲������ݣ�����Ϊ��ľ�̬��Ա
	// 3�����Ǹ������鵥�����е����ݣ���Ϊ�������ݳ�Ա����

	// ��ݼ����
	int OnAccelerator(const Ogre::InputEvent &event);
	int ProcessAccelerator(const AccelItem& accel, bool keydown_or_keyup);

	std::vector<AccelItem>& GetFrmAccels();
	//Frame *FindFrameForAccel( const AccelItem& accel);
	bool IsAccelIn(const AccelItem& accel);

private:
	Frame *m_LastFrame;			//�����һ��ָ��Ŀؼ�

	int m_iEditMode; // �༭ģʽ
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
