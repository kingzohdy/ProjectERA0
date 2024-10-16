
#include "stdafx.h"
#include "ResourceReaderBar_Motion.h"
#include "EffectManager.h"
#include "ModelMotionInfo.h"

#include "OgreResourceManager.h"
#include "OgreEntity.h"

#define BTN_PREVIEW		1
#define BTN_EDIT		2
#define BTN_ADD			3

// CChunkDetailBarMotion

IMPLEMENT_DYNAMIC(CChunkDetailBarMotion, CChunkDetailBar)
CChunkDetailBarMotion::CChunkDetailBarMotion()
{
}

CChunkDetailBarMotion::~CChunkDetailBarMotion()
{
}


BEGIN_MESSAGE_MAP(CChunkDetailBarMotion, CChunkDetailBar)
	ON_WM_CREATE()
	ON_BN_CLICKED(BTN_PREVIEW, OnPreview)
	ON_BN_CLICKED(BTN_EDIT, OnEditChunk)
	ON_BN_CLICKED(BTN_ADD, OnAddToMotion)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CChunkDetailBarMotion message handlers


int CChunkDetailBarMotion::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CChunkDetailBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bSucc = m_btnPreview.Create("在当前模型上预览", BS_PUSHBUTTON, CRect(8, 45, 125, 70), this, BTN_PREVIEW);
	assert(bSucc == TRUE);
	m_btnPreview.EnableWinXPTheme();
	m_btnPreview.ShowWindow(SW_SHOW);

	bSucc = m_btnEdit.Create("编辑", BS_PUSHBUTTON, CRect(8, 75, 125, 100), this, BTN_EDIT);
	assert(bSucc == TRUE);
	m_btnEdit.EnableWinXPTheme();
	m_btnEdit.ShowWindow(SW_SHOW);

	bSucc = m_btnAddToCurMotion.Create( "添加到当前 Motion", BS_PUSHBUTTON, CRect(8, 130, 125, 155), this, BTN_ADD );
	m_btnAddToCurMotion.EnableWinXPTheme( );
	m_btnAddToCurMotion.ShowWindow( SW_SHOW );

	ReadChunkInfo();
	return 0;
}

void CChunkDetailBarMotion::ReadChunkInfo()
{
	int nNum = 0;
	Ogre::Resource* pChunk = Ogre::ResourceManager::getSingleton().blockLoad(m_strChunkName.GetBuffer());
	if(IS_KIND_OF(Ogre::EntityMotionData, pChunk))
	{
		Ogre::EntityMotionData *pmotion = static_cast<Ogre::EntityMotionData *>(pChunk);
		for(size_t i=0; i<pmotion->m_Elements.size(); i++)
		{
			Ogre::MOTIONOBJTYPE_T type = pmotion->m_Elements[i]->m_type;
			if(type==Ogre::MOT_ANIM || type==Ogre::MOT_ANIM_ADD)
			{
				nNum++;
			}
		}
	}
	CString strLayers;
	strLayers.Format("%d", nNum);
	m_strInfo = "包含 " + strLayers + " 动画层";
}

void CChunkDetailBarMotion::OnPreview()
{
	int dot = m_strChunkName.ReverseFind('.');
	if(dot >= 0)
	{
		CString strTitle = m_strChunkName.Right(m_strChunkName.GetLength() - dot - 1);
		CEffectMgr::GetInstance()->ClearModelMotion();
		CEffectMgr::GetInstance()->GetModelActor()->playMotion(m_strChunkName.GetBuffer());
	}
}

void CChunkDetailBarMotion::OnEditChunk()
{
	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
	if(pInfo->FindMotionByFileName(m_strChunkName.GetBuffer()) >= 0)
	{
		MessageBox("特效已经打开编辑");
		return;
	}

	bool bIsPublic = false;
	CModelMotionInfo::MOTION* pmo = new CModelMotionInfo::MOTION();
	CString strPublic = g_strPublicMotionAddr;
	strPublic.MakeUpper();
	CString strHeader = m_strChunkName.Left(strPublic.GetLength());
	strHeader.MakeUpper();
	if(strHeader == strPublic)
		bIsPublic = true;
	else
		bIsPublic = false;

	if(true == pInfo->LoadSingleMotionFromFile(pmo, m_strChunkName.GetBuffer()))
	{
		pmo->bIsPublic = bIsPublic;;
		if(pInfo->AddMotion(pmo) < 0)
		{
			assert(0 && "Add motion error");
			delete pmo;
		}
		else
		{
			PostMainFrameMsg(EEM_UPDATA_LIST, UE_MOTION | UE_EFFECT_LIST, 0);
		}
	}
	else
	{
		MessageBox("找不到资源或者资源类型错误");
		delete pmo;
	}
}

void CChunkDetailBarMotion::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect clientRect;
	GetClientRect((LPRECT)clientRect);

	CBrush br(RGB(255, 255, 255));
	dc.FillRect(clientRect, &br);


	CRect textRect(8, 8, clientRect.right, 32);
	CRect textRect2(8, 32, clientRect.right, 56);

	CFont font;
	font.CreatePointFont(120, _T("宋体"));
	CFont* poldfont = (CFont*)dc.SelectObject(&font);

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(0, 0, 0));

	dc.DrawTextEx(m_strChunkName, textRect, DT_WORD_ELLIPSIS | DT_LEFT |DT_END_ELLIPSIS | DT_VCENTER | DT_SINGLELINE, NULL);
	dc.DrawTextEx(m_strInfo, textRect2, DT_WORD_ELLIPSIS | DT_LEFT |DT_END_ELLIPSIS | DT_VCENTER | DT_SINGLELINE, NULL);


	dc.SelectObject(poldfont);

}

void CChunkDetailBarMotion::OnAddToMotion()
{
	char* pchChunk = m_strChunkName.GetBuffer();
	SendMainFrameMsg(EEN_ADD_EFXFILE_TO_MODELMOTION, 0, (LPARAM)pchChunk);
}