#pragma once
#include "ui_layoutanchor.h"
#include "ui_texture.h"
#include "ui_gameui.h"
#include "OgreTexture.h"

class UICursor : Ogre::Singleton<UICursor>, public Ogre::LoadWrap
{
public:
	UICursor();
	~UICursor();

	void addCursor(const char *path, const char *name, int time=1000, int row=1, int col=1, int hotspotx=0, int hotspoty=0);
	void setCursor(const char *name);
	const char *getCursor();
	void update(float deltatime);

	void ShowCursor()
	{
		m_bShow = true; 
	}
	void HideCursor()
	{
		m_bShow = false; 
	}
	bool IsShown();
	
	void BeginDrag(const char *path, int id1, int id2, int iNum,bool bCompose, int hotspotx, int hotspoty);
	void EndDrag();
	bool IsInDragState();
	int GetDragUserData1();
	int GetDragUserData2();
	int GetDragUserData3();

	size_t GetCursorIndex(){return m_iCursor;};

	void updateCursor();

	static bool    m_bShow;
	static Ogre::Point2D m_Pos;

private:
	virtual void ResourceLoaded(Ogre::Resource *pres, Ogre::ResourceHandle h);

private:
	struct CursorDesc
	{
		char name[64];
		Ogre::HUIRES hres;
		int frametick;
		int width;
		int height;
		int row;
		int col;
		int hotspotx;
		int hotspoty;
	};
	std::vector<CursorDesc>m_Cursors;
	
	Ogre::TextureData *m_pDragTex;
	Ogre::TextureData *m_pBoxTex;
	Ogre::ResourceHandle	m_hLoad;

	bool m_bCompose;
	int m_hotspotx;
	int m_hotspoty;

	bool m_bDrag;
	int m_iIndex[3]; //0: panel id, 1:index, 3: num

	size_t m_iCursor;
	size_t m_CursorAnimTick;
	size_t m_CursorAnimFrame;
	
private:
};
