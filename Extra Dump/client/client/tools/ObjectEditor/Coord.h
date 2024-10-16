
#pragma once
#include "OgreBaseObject.h"
#include "OgreVector3.h"
#include "OgreVector2.h"
#include "OgreColourValue.h"
#include "OgreResourceManager.h"

class CCoord : public Ogre::BaseObject
{


public:
	CCoord(void);

	~CCoord(void);
	//virtual void SetTransparent(bool bTransparent,float alpha) {};
	//virtual void SetColor(Ogre::ColorQuad color){};
	void CreateVertex();
	//virtual void OnDraw(Ogre::ShaderContextNew *pcontextpool);
	//virtual void Update(Ogre::uint dtick);
	//virtual void Render(Ogre::SceneRenderer *pcontextpool, Ogre::GeomRenderEnv *penv);
	virtual bool NeedUpdate() { return true;};


	float GetXTrans(float dx, float dy);
	float GetYTrans(float dx, float dy);
	float GetZTrans(float dx, float dy);

	float GetTrans(float dx, float dy, COOR_ARROW arr);


	float m_fSize;
	COOR_ARROW SelectArrow(float x, float y);

	COOR_ARROW m_highLightArr;
	void UpdateHighLight(float x, float y);

	void SetFather(Ogre::SceneNode *pnode, int anchor = -1);
	void SetPosition(Ogre::Vector3 pos);
	void SetRotation(Ogre::Quaternion rot);
	void SetScale(float fScale);
	void SetScale(Ogre::Vector3 vScale);
	void AttachCoordToRoom(Ogre::GameScene* pRoom);
	void DetachCoordFromRoom(Ogre::GameScene* pRoom);

	void BindToModel(Ogre::Entity* pModel, int anchor);
	void UnbindFromModel(Ogre::Entity* pModel);

	void UpdateCoord(float dtime);

	bool IsGeomScale() { return m_bGeomScale; };
	void SetGeomScale(bool bGeomScale);

	bool IsEnable()	{ return m_bEnable; };
	void SetEnable(bool bEnable);

	struct Line
	{
		Ogre::Vector3 pos1;
		Ogre::Vector3 pos2;
		Ogre::ColorQuad color;
	};

	struct LineVertex
	{
		Ogre::Vector3 pos;
		Ogre::ColorQuad color;
		Ogre::Vector2 uv;
	};

	//void  FillCube(LineVertex* pLine, Vector3 offset);

	Line arrX;
	Line arrY;
	Line arrZ;

	//Ogre::ICoreMaterial* m_pCoreMtl;
	//Ogre::IInputLayout* m_pLayout;
	//Ogre::Material* m_pMtl;

	Ogre::Model* m_pRotMeshX;
	Ogre::Model* m_pRotMeshY;
	Ogre::Model* m_pRotMeshZ;


	Ogre::Model* m_pPosMeshX;
	Ogre::Model* m_pPosMeshY;
	Ogre::Model* m_pPosMeshZ;

	Ogre::Model* m_pPosMeshXZ;

	Ogre::TextureData* m_pTexR;
	Ogre::TextureData* m_pTexG;
	Ogre::TextureData* m_pTexB;
	Ogre::TextureData* m_pTexY;
	Ogre::TextureData* m_pTexW;
	Ogre::TextureData* m_pTexA;

	COORD_TYPE	m_pCurrType;
	bool		m_bGeomScale;
	bool		m_bEnable;
	Ogre::Vector3 m_vScale;
	static float ms_fScale;
	static COORD_TYPE ms_coordtype;

	Ogre::MovableObject *m_pFatherNode;
	int m_FatherAnchor;
};
