#pragma once

#include "TerrainEditTypes.h"
#include "OgreLines.h"

namespace TE
{

//��Χ������
struct MBoxData
{
	MVec3  center;
	MVec3  extent;
    float  r,g,b,a;
	unsigned int color;			 
	MMatrix      tm;
};
//��Χ����Ⱦ����
struct MBoxRenderData
{
    LMVB           vb;
   	MIInputLayout  *m_pLayout;
	MIShader       *m_pshader;
};
//��Χ�ж���
struct MBoxVec_st
{
	MVec3  pos;
};
//��Χ����,��Ҫʵ�ֻ��ư�Χ��
class MBoxObject
{
public:
	MBoxObject(void);
	~MBoxObject(void);

public: 
	//��ʼ��( ��ʼλ��, ��Χ )
	bool  Init( MVec3 centerPos, MVec3 extent );
	//��Ⱦ
	void  Render();
    //�ͷ�
	void   Release();
	//����
	bool   UpdataData();
    //�õ���Χ������
	MBoxData*  GetData(){return &m_Data;}
	//������ɫ
	void   SetColor( unsigned int color );
    //���÷�Χ
    bool   SetRange(const MVec3 &centerPos, const MVec3 &extent);
private:
	//����SHADER
	bool  LoadHLSLData();
	//������Ⱦ����
    bool  CreateRenderData();	
private:
	//��Ⱦ����
	MBoxRenderData      m_RenderData;
	//BOX����
	MBoxData            m_Data;
};

};
