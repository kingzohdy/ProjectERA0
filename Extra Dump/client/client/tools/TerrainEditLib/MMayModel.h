#pragma once


#include "lightmanager.h"

namespace TE
{

class EditorModel
{
public:
	EditorModel(void);
	virtual ~EditorModel(void);

    virtual bool LoadModel(const char* filename  );	
	virtual bool LoadBuildingModel(const char* filename );

	virtual void SetPos(MVec3 pos);
	virtual void SetDir(MVec3 dir);
	virtual void SetScale(float scale);

	virtual void Release();

	void          SetShowModel( MShowModel * pModel ){ m_pModel = pModel ; }

	MShowModel*                        GetShowModel(){ return m_pModel ; }

	void               Update( float time );
	void               UpdateMesh(MVec3 startPos);

	void                UpdateNormal();
    float             GetLinkMeshHeight( int index );
	bool HasLinkMesh(){ return m_pLinkMesh!=NULL; }
	void SetLinkMeshTexture(LMTex ptex);

	void             ClearLightColor();
	void             CountLightColor( LightInfo &lightInfo );

private:
	MShowModel                          *m_pModel;
	Ogre::MeshInstance                 *m_pLinkMesh;

	MVec3                                   m_wPos,m_wSavePos;	
	MVec3                                   m_Dir;	
	float                                   m_Scale;
    //保存的顶点数据
	std::vector<MVec3>                       m_LinkMeshVec;

	enum   MODEL_TYPE
	{
		LINK_MESH = 0 ,
		LINK_TEXTURE ,
		PAINT_TEXTURE_MESH ,
		BRAE_MESH,//斜坡
	};
    //对应的模型类型
	MODEL_TYPE                           m_ucModleType;

public:
	//对应LINKMESH的高度数据
	std::vector<float>                       m_LinkMeshHeight;
    //对应LINKMESH的颜色数据
	std::vector<unsigned int>                m_LinkMeshColor;
private:
	void UpdatePos();
};

};