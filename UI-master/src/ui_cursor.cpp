#pragma warning( disable : 4996 )

#include "ui_cursor.h"
#include "ui_framemgr.h"
#include "OgreRenderSystem.h"
#include "OgreResourceManager.h"

using namespace Ogre;

//-----------------------------------------------------------------------------
// Cursor
//-----------------------------------------------------------------------------
extern FrameManager *g_pFrameMgr;

bool UICursor::m_bShow = true;//永远只有一个cursor，所以作为类的静态变量
Ogre::Point2D UICursor::m_Pos(0, 0);

UICursor::UICursor()
{
	m_iIndex[0] = m_iIndex[1] = 0;
	m_bDrag = false;

	m_iCursor = 0;
	m_CursorAnimTick = 0;
	m_CursorAnimFrame = 0;
	m_hLoad = NULL;

	m_pDragTex	= new TextureData;
	m_pBoxTex	= new TextureData;
	m_pBoxTex->loadFromImageFile( "uires\\texture\\ShuBiaoXiangGuan\\TuoDongTuBiaoShuBiao32.tga" );
	m_pBoxTex->setStatic( false );
}

UICursor::~UICursor()
{
	for(size_t i=0; i<m_Cursors.size(); i++)
	{
		g_pDisplay->ReleaseUIRes(m_Cursors[i].hres);
	}
	OGRE_RELEASE( m_pDragTex );
	OGRE_RELEASE( m_pBoxTex );
	if (m_hLoad)
	{
		breakLoad(m_hLoad);
	}
}

void UICursor::addCursor(const char *path, const char *name, int time, int row, int col, int hotspotx, int hotspoty)
{
	CursorDesc desc;
	assert(strlen(name) < sizeof(desc.name));
	strncpy(desc.name, name,64);

	desc.hres = g_pDisplay->CreateTexture(path, BLEND_ALPHABLEND, &desc.width, &desc.height);
	assert(desc.hres != NULL);
	desc.frametick = time;
	desc.row = row;
	desc.col = col;
	desc.hotspotx = hotspotx;
	desc.hotspoty = hotspoty;

	m_Cursors.push_back(desc);
}

void UICursor::setCursor(const char *name)
{
	if( m_bDrag )
	{
		return;
	}

	if( m_Cursors.empty( ) )
	{
		return;
	}

	size_t i;
	for( i=0; i<m_Cursors.size(); i++ ) 
	{
		if( strcmp( m_Cursors[i].name, name ) == 0 )
		{
			break;
		}
	}
	assert( i < m_Cursors.size( ) );

	if( i == m_Cursors.size( ) ) return;
	m_iCursor			= i;
	m_CursorAnimTick	= 0;
	m_CursorAnimFrame	= 0;

	updateCursor();
}

const char *UICursor::getCursor()
{
	if( m_iCursor > m_Cursors.size() )
	{
		return "";
	}
	return m_Cursors[m_iCursor].name;
}

void UICursor::updateCursor()
{
	if( m_iCursor > m_Cursors.size() )
	{
		return;
	}
	CursorDesc &desc = m_Cursors[m_iCursor];
	int stepx = desc.width/desc.col;
	int stepy = desc.height/desc.row;
	g_pDisplay->setCursor(desc.hres, desc.hotspotx, desc.hotspoty, ((int)m_CursorAnimFrame%desc.col)*stepx, 
							((int)m_CursorAnimFrame/desc.col)*stepy, stepx, stepy);
}

void UICursor::update(float deltatime)
{
	if(m_Cursors.empty()) return;
	if( m_bDrag ) return;

	CursorDesc &desc = m_Cursors[m_iCursor];
	m_CursorAnimTick += TimeToTick(deltatime);
	int frame = ((int)m_CursorAnimTick/desc.frametick)%(desc.row*desc.col);
  	if(frame != m_CursorAnimFrame)
	{
		m_CursorAnimFrame = frame;
		updateCursor();
	}
}

bool UICursor::IsShown()
{
	return m_bShow;
}

void UICursor::BeginDrag(const char *path, int id1, int id2, int iNum,bool bCompose, int hotspotx, int hotspoty)
{
	m_bDrag = true;

	m_iIndex[0] = id1;
	m_iIndex[1] = id2;
	m_iIndex[2] = iNum;
	if (path)
	{
		m_bCompose = bCompose;
		m_hotspotx = hotspotx;
		m_hotspoty = hotspoty;
		if (m_hLoad)
		{
			breakLoad(m_hLoad);
		}
		m_hLoad = backgroundLoad(path, Ogre::ResourceManager::DP_TEXTURE);
	}
}

void UICursor::EndDrag()
{
	m_bDrag = false;
	m_iIndex[0] = m_iIndex[1] = m_iIndex[2] = 0;
	setCursor("normal");
}

bool UICursor::IsInDragState()
{
	return m_bDrag;
}

int UICursor::GetDragUserData1()
{
	return m_iIndex[0];
}

int UICursor::GetDragUserData2()
{
	return m_iIndex[1];
}

int UICursor::GetDragUserData3()
{
	return m_iIndex[2];
}

void UICursor::ResourceLoaded(Ogre::Resource *pres, Ogre::ResourceHandle h)
{
	if (h == m_hLoad)
	{
		m_hLoad = NULL;

		Ogre::TextureData *pDragTex = new Ogre::TextureData;
		if (pres == NULL || !pDragTex->loadFromImageFile((const char *)pres->getResPath()))
		{
			pDragTex->loadFromImageFile("uires\\defaulttexture.bmp");
		}

		TextureDesc desc, boxdesc;
		pDragTex->getDesc(desc);
		m_pBoxTex->getDesc(boxdesc);
		if (m_bCompose)
		{
			pDragTex->setStatic(false);
			LockResult lockresult;
			void *pbits = pDragTex->lock(0, 0, false, lockresult);
			if( desc.format == PF_A8R8G8B8 && boxdesc.width == desc.width && boxdesc.height == desc.height )
			{
				LockResult lockresult2;
				const void *pbits2 = m_pBoxTex->lock(0, 0, true, lockresult2);

				for(uint y=0; y<desc.height; y++)
				{
					const unsigned char *psrcbits = (const unsigned char *)pbits2 + lockresult2.rowpitch*y;
					unsigned char *pdstbits = (unsigned char *)pbits + lockresult.rowpitch*y;

					for(uint x=0; x<desc.width; x++)
					{
						int r = psrcbits[3];

						pdstbits[0] = (unsigned char)((pdstbits[0]*(255-r) + psrcbits[0]*r)/255);
						pdstbits[1] = (unsigned char)((pdstbits[1]*(255-r) + psrcbits[1]*r)/255);
						pdstbits[2] = (unsigned char)((pdstbits[2]*(255-r) + psrcbits[2]*r)/255);
						pdstbits[3] = (pdstbits[3] + r > 255) ? 255 : (pdstbits[3] + r);

						psrcbits += 4;
						pdstbits += 4;
					}
				}
				m_pBoxTex->unlock(0, 0);
			}else if( desc.format == PF_R8G8B8 && boxdesc.width == desc.width && boxdesc.height == desc.height )
			{
				LockResult lockresult2;
				const void *pbits2 = m_pBoxTex->lock(0, 0, true, lockresult2);

				for(uint y=0; y<desc.height; y++)
				{
					const unsigned char *psrcbits = (const unsigned char *)pbits2 + lockresult2.rowpitch*y;
					unsigned char *pdstbits = (unsigned char *)pbits + lockresult.rowpitch*y;

					for(uint x=0; x<desc.width; x++)
					{
						int r = psrcbits[3];

						pdstbits[0] = (unsigned char)((pdstbits[0]*(255-r) + psrcbits[0]*r)/255);
						pdstbits[1] = (unsigned char)((pdstbits[1]*(255-r) + psrcbits[1]*r)/255);
						pdstbits[2] = (unsigned char)((pdstbits[2]*(255-r) + psrcbits[2]*r)/255);
						//pdstbits[3] = (pdstbits[3] + r > 255) ? 255 : (pdstbits[3] + r);
						psrcbits += 4;
						pdstbits += 3;
					}
				}
				m_pBoxTex->unlock(0, 0);
			}
			pDragTex->unlock(0, 0);
			pDragTex->setStatic(true);
		}
		RenderSystem::getSingleton().setCursorProperty( pDragTex, m_hotspotx, m_hotspoty, 0, 0, desc.width, desc.height );
		OGRE_RELEASE(m_pDragTex);
		m_pDragTex = pDragTex;
	}
}

