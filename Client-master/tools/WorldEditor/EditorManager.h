
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
	//����·���ļ�
	void    SavePathFile();
	//��·���ļ�
	void    OpenPathFile();
	//���˲���
	void    Undo();
	//ǰ������
	void    Redo();
	//���þ�ͷ����
	void    SetCameraType(int mode);
	//���õ�ǰ���β�
	void    SetLayer(int layer);
	//������ʾģʽ
	void    SetShowMode(int mode);
	//���û�����
	void    SetAmbientLight();
	//����ֱ���
	void    SetDirectLight();

	void  SetupChooseColor(CHOOSECOLOR *cc,COLORREF *customclrs,HWND hwnd,COLORREF &init);

	//�Զ�����navimap
	void calNaviMap();
	void copyWalkMaskToFly();
	//export navimap
	void exportMaskFile();
	//import bmp file for mask
	void ImportMaskFile();
	void updateMaskFile(const char* filename);

	//�õ���ͷ��λ��
	float GetCameraPosX();
	float GetCameraPosY();
	float GetCameraPosZ();

	//����һ���󳡾�;�������ļ�������
	void  AddBulidingRoom(const char* filename);
	//����һ����ͼ��Դ
	bool  AddMapSourc();
	//������͸����;����(��Ӧ��)
	void  LoadAlphaTex(int layer);
	//�����͸����;����(��Ӧ��)
	void  SaveAlphaTex( int layer );
	//��������༭��ͼ
	void  AddViewMayMap();
	//�õ�������Ϣ
	void GetGameDrawInfo(size_t&  drawNum,size_t &drawPra);

public:
	HWND  m_hWnd;
	HWND  m_hwndFullRender;
	//���ڿ�ȣ��߶ȣ���ɫλ
	DWORD m_nWidth,m_nHeight,m_nColorBit;

	bool  m_bShowLiquid;
	bool  m_bShowTerrain;
	bool  m_bShowObject;
	bool  m_bShowQuad;
	bool  m_bShowShadow;

};

extern EditorManager *g_pApp;