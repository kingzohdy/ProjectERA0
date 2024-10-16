#pragma once

#include "TerrainEditTypes.h"
#include "OgreLines.h"

namespace TE
{

	//包围盒数据
	struct BoxData
	{
		Ogre::Vector3  center;
		Ogre::Vector3  extent;
		float  r,g,b,a;
		unsigned int color;			 
		Ogre::Matrix4      tm;
	};
	//包围盒渲染数据
	struct BoxRenderData
	{
		Ogre::VertexData *           vb;
		Ogre::VertexDeclHandle  *m_pLayout;
		Ogre::ShaderTechnique       *m_pshader;
	};
	//包围盒顶点
	struct BoxVertex
	{
		Ogre::Vector3  pos;
	};
	//包围盒类,主要实现绘制包围盒
	class WireframeBox
	{
	public:
		WireframeBox(void);
		~WireframeBox(void);

	public: 
		//初始化( 开始位置, 范围 )
		bool  Init( Ogre::Vector3 centerPos, Ogre::Vector3 extent );
		//渲染
		void  Render();
		//释放
		void   Release();
		//更新
		bool   UpdataData();
		//得到包围盒数据
		BoxData*  GetData(){return &m_Data;}
		//设置颜色
		void   SetColor( unsigned int color );
		//设置范围
		bool   SetRange(const Ogre::Vector3 &centerPos, const Ogre::Vector3 &extent);
	private:
		//引导SHADER
		bool  LoadHLSLData();
		//创建渲染数据
		bool  CreateRenderData();	
	private:
		//渲染数据
		BoxRenderData      m_RenderData;
		//BOX数据
		BoxData            m_Data;
	};

};
