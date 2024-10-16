#pragma once

#include "TerrainEditTypes.h"
#include "OgreLines.h"

namespace TE
{

	//��Χ������
	struct BoxData
	{
		Ogre::Vector3  center;
		Ogre::Vector3  extent;
		float  r,g,b,a;
		unsigned int color;			 
		Ogre::Matrix4      tm;
	};
	//��Χ����Ⱦ����
	struct BoxRenderData
	{
		Ogre::VertexData *           vb;
		Ogre::VertexDeclHandle  *m_pLayout;
		Ogre::ShaderTechnique       *m_pshader;
	};
	//��Χ�ж���
	struct BoxVertex
	{
		Ogre::Vector3  pos;
	};
	//��Χ����,��Ҫʵ�ֻ��ư�Χ��
	class WireframeBox
	{
	public:
		WireframeBox(void);
		~WireframeBox(void);

	public: 
		//��ʼ��( ��ʼλ��, ��Χ )
		bool  Init( Ogre::Vector3 centerPos, Ogre::Vector3 extent );
		//��Ⱦ
		void  Render();
		//�ͷ�
		void   Release();
		//����
		bool   UpdataData();
		//�õ���Χ������
		BoxData*  GetData(){return &m_Data;}
		//������ɫ
		void   SetColor( unsigned int color );
		//���÷�Χ
		bool   SetRange(const Ogre::Vector3 &centerPos, const Ogre::Vector3 &extent);
	private:
		//����SHADER
		bool  LoadHLSLData();
		//������Ⱦ����
		bool  CreateRenderData();	
	private:
		//��Ⱦ����
		BoxRenderData      m_RenderData;
		//BOX����
		BoxData            m_Data;
	};

};
