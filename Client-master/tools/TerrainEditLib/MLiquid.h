
#pragma once

#include <vector>

#include "OgreLiquid.h"
#include "TerrainEditTypes.h"

namespace TE
{
//Һ�嶥��ṹ
struct LIQUID_st
{
	MVec3  pos;
	MVec3  normal;
	unsigned int    color;
	MVec2  uv;
};

/*
//Һ����
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
	unsigned int m_iShallowColor;    ///< ǳˮ��ɫ
	unsigned int m_iDeepColor;       ///< ��ˮ��ɫ
	int m_iDepth;           ///< �ֲ�߶�

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
	/// ���¶�������
	void UpdateData();
};
*/
};
