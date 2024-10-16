
#pragma once

class EditorManager
{
public:
	EditorManager(void);
	~EditorManager(void);

public:   
	bool init(HINSTANCE hinst, HWND hmainview, HWND hrespreview, HWND hradarview);
	void doFrame(); 

public:
	//保存路径文件
	void    SavePathFile();
	//打开路径文件
	void    OpenPathFile();
	//后退操作
	void    Undo();
	//前进操作
	void    Redo();
	//设置镜头类型
	void    SetCameraType(int mode);
	//设置当前地形层
	void    SetLayer(int layer);
	//设置显示模式
	void    SetShowMode(int mode);
	//设置环境灯
	void    SetAmbientLight();
	//设置直射灯
	void    SetDirectLight();

	void  SetupChooseColor(CHOOSECOLOR *cc,COLORREF *customclrs,HWND hwnd,COLORREF &init);

	//自动计算navimap
	void calNaviMap();
	void copyWalkMaskToFly();
	//export navimap
	void exportMaskFile();
	//import bmp file for mask
	void ImportMaskFile();
	void updateMaskFile(const char* filename);

	//得到镜头的位置
	float GetCameraPosX();
	float GetCameraPosY();
	float GetCameraPosZ();

	//增加一个大场景;参数（文件名）；
	void  AddBulidingRoom(const char* filename);
	//增加一个地图资源
	bool  AddMapSourc();
	//引导半透明层;参数(对应层)
	void  LoadAlphaTex(int layer);
	//保存半透明层;参数(对应层)
	void  SaveAlphaTex( int layer );
	//增加浏览编辑地图
	void  AddViewMayMap();
	//得到绘制信息
	void GetGameDrawInfo(size_t&  drawNum,size_t &drawPra);

public:
	HWND  m_hWnd;
	HWND  m_hwndFullRender;
	//窗口宽度，高度，颜色位
	DWORD m_nWidth,m_nHeight,m_nColorBit;

	bool  m_bShowLiquid;
	bool  m_bShowTerrain;
	bool  m_bShowObject;
	bool  m_bShowQuad;
	bool  m_bShowShadow;

};

extern EditorManager *g_pApp;