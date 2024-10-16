
#pragma once

#include <vector>

#include "OgreLiquid.h"
#include "TerrainEditTypes.h"

namespace TE
{
//液体顶点结构
struct LIQUID_st
{
	MVec3  pos;
	MVec3  normal;
	unsigned int    color;
	MVec2  uv;
};

/*
//液体类
class MLiquid
{
public:
	MLiquid(void);
	~MLiquid(void);

	virtual void render(Ogre::SceneRenderer *prenderer, const Ogre::ShaderEnvData &envdata);

public:
	int m_nGridX,m_nGridZ;
	float m_fTileSize;
	float m_fTexRepeats;
    int  m_nType;
	std::vector <LMTex>   m_pTexSet;
	LMVB                 m_pVB;
	LMIB                 m_pIB;
   	MIInputLayout  m_pLayout;
	Ogre::Material *m_pMtl;
	std::string     m_strFileName;

	bool  m_bTrans;
	float m_fAniTime;
	float m_fHeight;	
	MVec4   m_Color;
	unsigned int m_iShallowColor;    ///< 浅水颜色
	unsigned int m_iDeepColor;       ///< 深水颜色
	int m_iDepth;           ///< 分层高度

public:
	MVec4 GetColor(){return m_Color;};
	void  SetColor(MVec4 color);

	void  SetAniTime(float time);
	bool  LoadHLSLData(const char* filename);

	bool  LoadTexture(const char* filename,int num);
	bool  CreateVBIB(int gx,int gz,float x1,float y1,float z1,float x2,float y2,float z2);
	bool  CreateVBIBEx(int gx,int gz,float x1,float y1,float z1,float x2,float y2,float z2);
	
	void  SetHeight(float y);
	float GetHeight(){return m_fHeight;}

	const char* GetTextureName(){return m_strFileName.c_str ();}

	void  Render();
	void  Release();
	void  ClearTexture();

	unsigned int GetColor(const MVec3& vec3);
	/// 更新顶点数据
	void UpdateData();
};
*/
};
