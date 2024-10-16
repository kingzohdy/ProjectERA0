
#include "stdafx.h"
#include "Coord.h"
#include "EditorManager.h"
#include "MathUtility.h"

#include "OgreModel.h"
#include "OgreTexture.h"
#include "OgreEntity.h"
#include "OgreCamera.h"

float CCoord::ms_fScale = 1.0f;
COORD_TYPE CCoord::ms_coordtype = CT_POS ;

CCoord::CCoord(void)
{
	m_pCurrType = CT_NONE;
	m_fSize = 0.3f;
	m_highLightArr = COOD_NONE;

	m_pRotMeshX = NULL;
	m_pRotMeshY = NULL;
	m_pRotMeshZ = NULL;

	m_pPosMeshX = NULL;
	m_pPosMeshY = NULL;
	m_pPosMeshZ = NULL;

	m_pTexR = NULL;
	m_pTexG = NULL;
	m_pTexB = NULL;
	m_pTexW = NULL;
	m_pTexA = NULL;

	m_bGeomScale = false;
	m_bEnable = true;

	m_vScale = Ogre::Vector3(1.0f, 1.0f, 1.0f);

	m_pFatherNode = NULL;
	m_FatherAnchor = 0;

	//char* a = (char*)malloc(100);
	//long an = 0;
	//_CrtIsMemoryBlock(a, 100, &an, NULL, NULL);
	//sprintf(a, "%x,coord ", this);

	//_CrtSetBreakAlloc(25760);
}

CCoord::~CCoord(void)
{

	OGRE_RELEASE(m_pRotMeshX);
	OGRE_RELEASE(m_pRotMeshY);
	OGRE_RELEASE(m_pRotMeshZ);

	OGRE_RELEASE(m_pPosMeshX);
	OGRE_RELEASE(m_pPosMeshY);
	OGRE_RELEASE(m_pPosMeshZ);

	OGRE_RELEASE(m_pTexR);
	OGRE_RELEASE(m_pTexG);
	OGRE_RELEASE(m_pTexB);
	OGRE_RELEASE(m_pTexW);
	OGRE_RELEASE(m_pTexA);
}

Ogre::TextureData* LoadTextureFromChunk(const char* name)
{
	Ogre::Resource* pRes = Ogre::ResourceManager::getSingleton().blockLoad(name);
	if(pRes == NULL)	return NULL;

	assert(IS_KIND_OF(Ogre::TextureData, pRes));
	return static_cast<Ogre::TextureData *>(pRes);
}

void CCoord::CreateVertex()
{
	m_pRotMeshX = static_cast<Ogre::Model*>(Ogre::createObjectFromResource("toolres\\fxeditor\\rotX.omod"));
	m_pRotMeshY = static_cast<Ogre::Model*>(Ogre::createObjectFromResource("toolres\\fxeditor\\rotY.omod"));
	m_pRotMeshZ = static_cast<Ogre::Model*>(Ogre::createObjectFromResource("toolres\\fxeditor\\rotZ.omod"));

	m_pPosMeshX = static_cast<Ogre::Model*>(Ogre::createObjectFromResource("toolres\\fxeditor\\arrx.omod"));
	m_pPosMeshY = static_cast<Ogre::Model*>(Ogre::createObjectFromResource("toolres\\fxeditor\\arry.omod"));
	m_pPosMeshZ = static_cast<Ogre::Model*>(Ogre::createObjectFromResource("toolres\\fxeditor\\arrz.omod"));

	m_pTexR = LoadTextureFromChunk("toolres\\fxeditor\\texr.otex");
	m_pTexG = LoadTextureFromChunk("toolres\\fxeditor\\texg.otex");
	m_pTexB = LoadTextureFromChunk("toolres\\fxeditor\\texb.otex");
	m_pTexW = LoadTextureFromChunk("toolres\\fxeditor\\texw.otex");
	m_pTexA = LoadTextureFromChunk("toolres\\fxeditor\\texa.otex");


	//m_pPosMeshX->setTextureByID(1, m_pTexA);
	//m_pPosMeshY->setTextureByID(1, m_pTexA);
	//m_pPosMeshZ->setTextureByID(1, m_pTexA);

	//m_pRotMeshX->setTextureByID(1, m_pTexA);
	//m_pRotMeshY->setTextureByID(1, m_pTexA);
	//m_pRotMeshZ->setTextureByID(1, m_pTexA);

	m_pRotMeshX->showSkins(false);
	m_pRotMeshY->showSkins(false);
	m_pRotMeshZ->showSkins(false);

	m_pPosMeshX->showSkins(false);
	m_pPosMeshY->showSkins(false);
	m_pPosMeshZ->showSkins(false);

	if(m_pCurrType == CT_POS)
	{
		m_pPosMeshX->setTextureByID(1, m_highLightArr == COOD_X ? m_pTexW : m_pTexR);
		m_pPosMeshY->setTextureByID(1, m_highLightArr == COOD_Y ? m_pTexW : m_pTexG);
		m_pPosMeshZ->setTextureByID(1, m_highLightArr == COOD_Z ? m_pTexW : m_pTexB);

		m_pPosMeshX->showSkin("arrx_1", true);
		m_pPosMeshX->showSkin("arrx_2", true);
		m_pPosMeshY->showSkin("arrx_1", true);
		m_pPosMeshY->showSkin("arrx_2", true);
		m_pPosMeshZ->showSkin("arrx_1", true);
		m_pPosMeshZ->showSkin("arrx_2", true);
	}
	else if(m_pCurrType == CT_ROTATE)
	{
		m_pRotMeshX->setTextureByID(1, m_highLightArr == COOD_RX ? m_pTexW : m_pTexR);
		m_pRotMeshY->setTextureByID(1, m_highLightArr == COOD_RY ? m_pTexW : m_pTexG);
		m_pRotMeshZ->setTextureByID(1, m_highLightArr == COOD_RZ ? m_pTexW : m_pTexB);

		m_pRotMeshX->showSkins(true);
		m_pRotMeshY->showSkins(true);
		m_pRotMeshZ->showSkins(true);
	}

	else if(m_pCurrType == CT_SCALE)
	{
		m_pPosMeshX->setTextureByID(1, m_highLightArr == COOD_SX ? m_pTexW : m_pTexR);
		m_pPosMeshX->showSkin("arrx_1", true);
		m_pPosMeshX->showSkin("arrx_3", true);

		if(m_bGeomScale == false)
		{
			m_pPosMeshY->setTextureByID(1, m_highLightArr == COOD_SY ? m_pTexW : m_pTexG);
			m_pPosMeshZ->setTextureByID(1, m_highLightArr == COOD_SZ ? m_pTexW : m_pTexB);
			m_pPosMeshY->showSkin("arrx_1", true);
			m_pPosMeshY->showSkin("arrx_3", true);
			m_pPosMeshZ->showSkin("arrx_1", true);
			m_pPosMeshZ->showSkin("arrx_3", true);
		}
	}
}

void CCoord::SetFather(Ogre::SceneNode *pnode, int anchor)
{
	if(pnode) pnode->addRef();
	if(m_pFatherNode) m_pFatherNode->release();

	m_pFatherNode = pnode;
	m_FatherAnchor = anchor;
}

void CCoord::SetPosition(Ogre::Vector3 pos)
{
	if(m_pPosMeshX) m_pPosMeshX->setPosition(pos);
	if(m_pPosMeshY) m_pPosMeshY->setPosition(pos);
	if(m_pPosMeshZ) m_pPosMeshZ->setPosition(pos);

	if(m_pRotMeshX) m_pRotMeshX->setPosition(pos);
	if(m_pRotMeshY) m_pRotMeshY->setPosition(pos);
	if(m_pRotMeshZ) m_pRotMeshZ->setPosition(pos);

}

void CCoord::SetRotation(Ogre::Quaternion rot)
{
	if(m_pPosMeshX) m_pPosMeshX->setRotation(rot);
	if(m_pPosMeshY) m_pPosMeshY->setRotation(rot);
	if(m_pPosMeshZ) m_pPosMeshZ->setRotation(rot);

	if(m_pRotMeshX) m_pRotMeshX->setRotation(rot);
	if(m_pRotMeshY) m_pRotMeshY->setRotation(rot);
	if(m_pRotMeshZ) m_pRotMeshZ->setRotation(rot);
}

void CCoord::SetScale(float fScale)
{
	Ogre::Vector3 vScale(fScale, fScale, fScale);
	SetScale(vScale);
}

void CCoord::SetScale(Ogre::Vector3 vScale)
{
	m_vScale = vScale;
	if(ms_fScale < 0.001f)
	{
		ms_fScale = 1.0f;
	}
	if(m_pPosMeshX) m_pPosMeshX->setScale(m_vScale * ms_fScale);
	if(m_pPosMeshY) m_pPosMeshY->setScale(m_vScale * ms_fScale);
	if(m_pPosMeshZ) m_pPosMeshZ->setScale(m_vScale * ms_fScale);

	if(m_pRotMeshX) m_pRotMeshX->setScale(m_vScale * ms_fScale);
	if(m_pRotMeshY) m_pRotMeshY->setScale(m_vScale * ms_fScale);
	if(m_pRotMeshZ) m_pRotMeshZ->setScale(m_vScale * ms_fScale);
}

void CCoord::AttachCoordToRoom(Ogre::GameScene* pRoom)
{
	if(m_pPosMeshX)	m_pPosMeshX->attachToScene(pRoom);
	if(m_pPosMeshY)	m_pPosMeshY->attachToScene(pRoom);
	if(m_pPosMeshZ)	m_pPosMeshZ->attachToScene(pRoom);

	if(m_pRotMeshX)	m_pRotMeshX->attachToScene(pRoom);
	if(m_pRotMeshY)	m_pRotMeshY->attachToScene(pRoom);
	if(m_pRotMeshZ)	m_pRotMeshZ->attachToScene(pRoom);
}

void CCoord::DetachCoordFromRoom(Ogre::GameScene* pRoom)
{
	if(m_pPosMeshX)	m_pPosMeshX->detachFromScene();
	if(m_pPosMeshY)	m_pPosMeshY->detachFromScene();
	if(m_pPosMeshZ)	m_pPosMeshZ->detachFromScene();

	if(m_pRotMeshX)	m_pRotMeshX->detachFromScene();
	if(m_pRotMeshY)	m_pRotMeshY->detachFromScene();
	if(m_pRotMeshZ)	m_pRotMeshZ->detachFromScene();
}

void CCoord::BindToModel(Ogre::Entity* pModel, int anchor)
{
	if(m_pPosMeshX)	pModel->bindObject(anchor, m_pPosMeshX, 200);
	if(m_pPosMeshY)	pModel->bindObject(anchor, m_pPosMeshY, 200);
	if(m_pPosMeshZ)	pModel->bindObject(anchor, m_pPosMeshZ, 200);

	if(m_pRotMeshX)	pModel->bindObject(anchor, m_pRotMeshX, 200);
	if(m_pRotMeshY)	pModel->bindObject(anchor, m_pRotMeshY, 200);
	if(m_pRotMeshZ)	pModel->bindObject(anchor, m_pRotMeshZ, 200);
}

void CCoord::UnbindFromModel(Ogre::Entity* pModel)
{
	if(m_pPosMeshX)	pModel->unbindObject(m_pPosMeshX);
	if(m_pPosMeshY)	pModel->unbindObject(m_pPosMeshY);
	if(m_pPosMeshZ)	pModel->unbindObject(m_pPosMeshZ);

	if(m_pRotMeshX)	pModel->unbindObject(m_pRotMeshX);
	if(m_pRotMeshY)	pModel->unbindObject(m_pRotMeshY);
	if(m_pRotMeshZ)	pModel->unbindObject(m_pRotMeshZ);
}

/* 首先计算出选中轴在屏幕空间上的投影轴l，接着计算鼠标移动的距离在轴l上面的投影的距离，更新轴坐标
** 最后把移动后的坐标转换到3D空间中
*/
float CCoord::GetTrans(float dx, float dy, COOR_ARROW arr)
{
	if(m_pPosMeshX == NULL || m_pPosMeshY == NULL || m_pPosMeshZ == NULL || m_bEnable == false) 
		return 0.0f;
	//把当前位置变换到投影空间，在投影空间中移动，然后把移动的初始点和结束点反变换回模型空间，相减得到偏移
	//移动的时候只取X轴分量

	Ogre::Camera* pcamera = CEnginAgent::GetInstance()->m_pCamera;
	Ogre::Matrix4 tm = m_pPosMeshX->getWorldMatrix() * pcamera->getViewMatrix() * pcamera->getProjectMatrix(); 

	Ogre::Vector3 p1(0.0f, 0.0f, 0.0f), p2;

	if(arr == COOD_X || arr == COOD_RX || arr == COOD_SX) 
		p2 = Ogre::Vector3(1.0f, 0.0f, 0.0f);
	else if(arr == COOD_Y || arr == COOD_RY || arr == COOD_SY) 
		p2 = Ogre::Vector3(0.0f, 1.0f, 0.0f);
	else if(arr == COOD_Z || arr == COOD_RZ || arr == COOD_SZ) 
		p2 = Ogre::Vector3(0.0f, 0.0f, 1.0f);
	else
		return 0.0f;

	Ogre::Vector3 p1p, p2p;
	tm.apply4x4(p1p, p1);
	tm.apply4x4(p2p, p2);
	Ogre::Vector3 coordDir = p2p - p1p;
	float coordZ = coordDir.z;
	coordDir.z = 0.0f;
	Ogre::Normalize(coordDir);
	Ogre::Vector3 mouseDir = Ogre::Vector3(dx , dy,  0.0f);
	if(arr == COOD_RX || arr == COOD_RY || arr == COOD_RZ)
	{
		mouseDir.x = dy;
		mouseDir.y = -dx;
	}
	float dst = 0.0f;
	dst = Ogre::DotProduct(coordDir, mouseDir);


	Ogre::Vector3 endPosp = p1p + coordDir * dst;
	Ogre::Vector3 endPos;
	Ogre::Matrix4 inv;
	inv = tm.inverse();

	inv.apply4x4(endPos, endPosp) ;
	Ogre::Vector3 ognPos = p1 * inv;
	Ogre::Vector3 ret = endPos;

	if(arr == COOD_X || arr == COOD_RX || arr == COOD_SX) 
		return ret.x;
	else if(arr == COOD_Y|| arr == COOD_RY || arr == COOD_SY) 
		return ret.y;
	else if(arr == COOD_Z|| arr == COOD_RZ || arr == COOD_SZ) 
		return ret.z;
	else
		return 0.0f;
}

float CCoord::GetXTrans(float dx, float dy)
{
	return GetTrans(dx, dy, COOD_X);

}

float CCoord::GetYTrans(float dx, float dy)
{
	return GetTrans(dx, dy, COOD_Y);
}

float CCoord::GetZTrans(float dx, float dy)
{
	return GetTrans(dx, dy, COOD_Z);
}


static Ogre::BoxBound GetModelBoxBound(Ogre::Model *pmodel)
{
	Ogre::BoxSphereBound bound;
	pmodel->getLocalBounds(bound);

	return bound.getBox();
}

#define DIST2(x1, y1, x2, y2) (((x1) - (x2)) * ((x1) - (x2)) + ((y1) - (y2)) * ((y1) - (y2)) )
COOR_ARROW  CCoord::SelectArrow(float x, float y)
{

	if(m_pPosMeshX == NULL || m_pPosMeshY == NULL || m_pPosMeshZ == NULL) return COOD_NONE;

	Ogre::Camera* pcamera = CEnginAgent::GetInstance()->m_pCamera;
	Ogre::Matrix4 tm = pcamera->getViewMatrix() * pcamera->getProjectMatrix(); 

	float tmp = -1;
	float tmpX = -1.0f, tmpY = -1.0f, tmpZ = -1.0f; 
	float tmpRX = -1.0f, tmpRY = -1.0f, tmpRZ = -1.0f;
	float tmpSX = -1.0f, tmpSY = -1.0f, tmpSZ = -1.0f;
	bool bx = false, by = false, bz = false, brx = false, bry = false, brz = false, bsx = false, bsy = false, bsz = false;

	if(m_pCurrType == CT_POS)
	{
		bx = C3DMath::CheckTouchBox(m_pPosMeshX->getWorldMatrix(), GetModelBoxBound(m_pPosMeshX),  true, ms_fScale,  x, y, &tmpX);
		by = C3DMath::CheckTouchBox(m_pPosMeshY->getWorldMatrix(), GetModelBoxBound(m_pPosMeshY), true, ms_fScale, x, y, &tmpY);
		bz = C3DMath::CheckTouchBox(m_pPosMeshZ->getWorldMatrix(), GetModelBoxBound(m_pPosMeshZ), true, ms_fScale, x, y, &tmpZ);
	}
	else if(m_pCurrType == CT_ROTATE)
	{
		brx = C3DMath::CheckTouchBox(m_pRotMeshX->getWorldMatrix(), GetModelBoxBound(m_pRotMeshX), false, ms_fScale, x, y, &tmpRX);
		bry = C3DMath::CheckTouchBox(m_pRotMeshY->getWorldMatrix(), GetModelBoxBound(m_pRotMeshY), false, ms_fScale, x, y, &tmpRY);
		brz = C3DMath::CheckTouchBox(m_pRotMeshZ->getWorldMatrix(), GetModelBoxBound(m_pRotMeshZ), false, ms_fScale, x, y, &tmpRZ);

	}
	else if(m_pCurrType == CT_SCALE)
	{
		bsx = C3DMath::CheckTouchBox(m_pRotMeshX->getWorldMatrix(), GetModelBoxBound(m_pPosMeshX), true, ms_fScale, x, y, &tmpSX);
		if(m_bGeomScale == false)
		{
			bsy = C3DMath::CheckTouchBox(m_pRotMeshY->getWorldMatrix(), GetModelBoxBound(m_pPosMeshY), true, ms_fScale, x, y, &tmpSY);
			bsz = C3DMath::CheckTouchBox(m_pRotMeshZ->getWorldMatrix(), GetModelBoxBound(m_pPosMeshZ), true, ms_fScale, x, y, &tmpSZ);
		}
	}

	COOR_ARROW ret = COOD_NONE;
	// --------pos---------------------- [6/12/2008]
	if(bx == true) //x
	{
		tmp = tmpX;
		ret = COOD_X;
	}
	if(by == true && (tmpY < tmp || tmp < 0)) //y
	{
		tmp = tmpY;
		ret = COOD_Y;
	}
	if(bz == true && (tmpZ < tmp || tmp < 0)) //z
	{
		tmp = tmpZ;
		ret = COOD_Z;
	}
	// ---------rot--------------------- [6/12/2008]
	if(brx == true && (tmpRX < tmp || tmp < 0)) // rx
	{
		tmp = tmpRX;
		ret = COOD_RX;
	}

	if(bry == true && (tmpRY < tmp || tmp < 0)) // ry
	{
		tmp = tmpRY;
		ret = COOD_RY;
	}

	if(brz == true && (tmpRZ < tmp || tmp < 0)) //rz
	{
		tmp = tmpRZ;
		ret = COOD_RZ;
	}
	// ----------scale------------------- [6/12/2008]
	if(bsx == true && (tmpSX < tmp || tmp < 0)) // rx
	{
		tmp = tmpSX;
		ret = COOD_SX;
	}

	if(bsy == true && (tmpSY < tmp || tmp < 0)) // ry
	{
		tmp = tmpSY;
		ret = COOD_SY;
	}

	if(bsz == true && (tmpSZ < tmp || tmp < 0)) //rz
	{
		tmp = tmpSZ;
		ret = COOD_SZ;
	}

	return ret;
}

//COOR_ARROW m_highLightArr
void CCoord::UpdateHighLight(float x, float y)
{
	m_highLightArr = SelectArrow(x, y);

	if(m_pPosMeshX == NULL || m_pPosMeshY == NULL || m_pPosMeshZ == NULL 
		|| m_pRotMeshX == NULL || m_pRotMeshY == NULL || m_pRotMeshZ == NULL)
		return;


	if(m_pCurrType == CT_POS)
	{
		m_pPosMeshX->setTextureByID(1, m_highLightArr == COOD_X ? m_pTexW : m_pTexR);
		m_pPosMeshY->setTextureByID(1, m_highLightArr == COOD_Y ? m_pTexW : m_pTexG);
		m_pPosMeshZ->setTextureByID(1, m_highLightArr == COOD_Z ? m_pTexW : m_pTexB);


	}

	else if(m_pCurrType == CT_ROTATE)
	{
		m_pRotMeshX->setTextureByID(1, m_highLightArr == COOD_RX ? m_pTexW : m_pTexR);
		m_pRotMeshY->setTextureByID(1, m_highLightArr == COOD_RY ? m_pTexW : m_pTexG);
		m_pRotMeshZ->setTextureByID(1, m_highLightArr == COOD_RZ ? m_pTexW : m_pTexB);
	}

	else if(m_pCurrType == CT_SCALE)
	{
		m_pPosMeshX->setTextureByID(1, m_highLightArr == COOD_SX ? m_pTexW : m_pTexR);
		if(m_bGeomScale == false)
		{
			m_pPosMeshY->setTextureByID(1, m_highLightArr == COOD_SY ? m_pTexW : m_pTexG);
			m_pPosMeshZ->setTextureByID(1, m_highLightArr == COOD_SZ ? m_pTexW : m_pTexB);
		}
	}
}

void CCoord::UpdateCoord(float dtime)
{
	Ogre::uint dtick = Ogre::TimeToTick(dtime);

	if(m_pFatherNode)
	{
		Ogre::Matrix4 tm = m_pFatherNode->getAnchorWorldMatrix(m_FatherAnchor);

		m_pPosMeshX->update(dtick);
		m_pPosMeshY->update(dtick);
		m_pPosMeshZ->update(dtick);
		m_pRotMeshX->update(dtick);
		m_pRotMeshY->update(dtick);
		m_pRotMeshZ->update(dtick);
	}

	if(m_pCurrType != ms_coordtype)
	{
		//m_pPosMeshX->setTextureByID(1, m_pTexA);
		//m_pPosMeshY->setTextureByID(1, m_pTexA);
		//m_pPosMeshZ->setTextureByID(1, m_pTexA);

		m_pRotMeshX->showSkins(false);
		m_pRotMeshY->showSkins(false);
		m_pRotMeshZ->showSkins(false);

		m_pPosMeshX->showSkins(false);
		m_pPosMeshY->showSkins(false);
		m_pPosMeshZ->showSkins(false);

		if(ms_coordtype == CT_POS)
		{
			m_pPosMeshX->setTextureByID(1, m_highLightArr == COOD_X ? m_pTexW : m_pTexR);
			m_pPosMeshY->setTextureByID(1, m_highLightArr == COOD_Y ? m_pTexW : m_pTexG);
			m_pPosMeshZ->setTextureByID(1, m_highLightArr == COOD_Z ? m_pTexW : m_pTexB);

			m_pPosMeshX->showSkin("arrx_1", true);
			m_pPosMeshX->showSkin("arrx_2", true);
			m_pPosMeshY->showSkin("arrx_1", true);
			m_pPosMeshY->showSkin("arrx_2", true);
			m_pPosMeshZ->showSkin("arrx_1", true);
			m_pPosMeshZ->showSkin("arrx_2", true);
		}
		else if(ms_coordtype == CT_ROTATE)
		{
			m_pRotMeshX->setTextureByID(1, m_highLightArr == COOD_RX ? m_pTexW : m_pTexR);
			m_pRotMeshY->setTextureByID(1, m_highLightArr == COOD_RY ? m_pTexW : m_pTexG);
			m_pRotMeshZ->setTextureByID(1, m_highLightArr == COOD_RZ ? m_pTexW : m_pTexB);

			m_pRotMeshX->showSkins(true);
			m_pRotMeshY->showSkins(true);
			m_pRotMeshZ->showSkins(true);
		}
		else if(ms_coordtype == CT_SCALE)
		{
			m_pPosMeshX->setTextureByID(1, m_highLightArr == COOD_SX ? m_pTexW : m_pTexR);
			m_pPosMeshX->showSkin("arrx_1", true);
			m_pPosMeshX->showSkin("arrx_3", true);

			if(m_bGeomScale == false)
			{
				m_pPosMeshY->setTextureByID(1, m_highLightArr == COOD_SY ? m_pTexW : m_pTexG);
				m_pPosMeshZ->setTextureByID(1, m_highLightArr == COOD_SZ ? m_pTexW : m_pTexB);
				m_pPosMeshY->showSkin("arrx_1", true);
				m_pPosMeshY->showSkin("arrx_3", true);
				m_pPosMeshZ->showSkin("arrx_1", true);
				m_pPosMeshZ->showSkin("arrx_3", true);
			}
		}

		m_pCurrType = ms_coordtype;
	}

	SetScale(m_vScale);
}

void CCoord::SetGeomScale(bool bGeomScale)
{
	if(bGeomScale != m_bGeomScale)
	{
		m_bGeomScale = bGeomScale;

		if(m_pPosMeshX == NULL || m_pPosMeshY == NULL || m_pPosMeshZ == NULL 
			|| m_pRotMeshX == NULL || m_pRotMeshY == NULL || m_pRotMeshZ == NULL)
			return;

		if(ms_coordtype != CT_SCALE)
			return;

		if(m_bGeomScale == true)
		{
			m_pPosMeshY->showSkins(false);
			m_pPosMeshZ->showSkins(false);
		}
		else
		{
			m_pPosMeshY->showSkin("arrx_1", true);
			m_pPosMeshY->showSkin("arrx_3", true);

			m_pPosMeshZ->showSkin("arrx_1", true);
			m_pPosMeshZ->showSkin("arrx_3", true);
		}
	}
}

void CCoord::SetEnable(bool bEnable)
{
	m_bEnable = bEnable;
}