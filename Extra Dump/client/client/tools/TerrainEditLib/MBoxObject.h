#pragma once

#include "TerrainEditTypes.h"
#include "OgreLines.h"

namespace TE
{

//包围盒数据
struct MBoxData
{
	MVec3  center;
	MVec3  extent;
    float  r,g,b,a;
	unsigned int color;			 
	MMatrix      tm;
};
//包围盒渲染数据
struct MBoxRenderData
{
    LMVB           vb;
   	MIInputLayout  *m_pLayout;
	MIShader       *m_pshader;
};
//包围盒顶点
struct MBoxVec_st
{
	MVec3  pos;
};
//包围盒类,主要实现绘制包围盒
class MBoxObject
{
public:
	MBoxObject(void);
	~MBoxObject(void);

public: 
	//初始化( 开始位置, 范围 )
	bool  Init( MVec3 centerPos, MVec3 extent );
	//渲染
	void  Render();
    //释放
	void   Release();
	//更新
	bool   UpdataData();
    //得到包围盒数据
	MBoxData*  GetData(){return &m_Data;}
	//设置颜色
	void   SetColor( unsigned int color );
    //设置范围
    bool   SetRange(const MVec3 &centerPos, const MVec3 &extent);
private:
	//引导SHADER
	bool  LoadHLSLData();
	//创建渲染数据
    bool  CreateRenderData();	
private:
	//渲染数据
	MBoxRenderData      m_RenderData;
	//BOX数据
	MBoxData            m_Data;
};

};
