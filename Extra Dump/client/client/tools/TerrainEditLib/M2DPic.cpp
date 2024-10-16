
#include ".\m2dpic.h"

#include "mtexmanager.h"
#include "OgreTexture.h"

using namespace Ogre;
using namespace TE;

M2DPic::M2DPic(void)
{
	memset(&m_Data,0,sizeof(Pic2DData));
}

M2DPic::~M2DPic(void)
{
    Release();
}
//���������ļ�(��Ҫ������UI�ӿ���ʾ����ͼƬ) ;����( �ļ���, ��ʾ���� X,Y ,��ʾ���, �߶� );����( Ϊ���ʾ�ɹ� )	
bool M2DPic::LoadTexFile(const char *filename,float x,float y ,float width ,float height)
{
	TextureData *ptex = LoadTextureFromFile(filename);

	m_Data.pTex = ptex;
	return true;
};
//���ñ߿���ɫ	
void        M2DPic::SetColor(int  color)
{
};
//������ʾ����	
void        M2DPic::SetPos(float x ,float y )
{
};
//���ÿ�ȸ߶�	
void        M2DPic::SetWidthHeight(float width ,float height)
{
};
//��ʼ�� ; ����( X,Y,���,�߶� ) ; ����( Ϊ�� ��ʾ�ɹ�)
bool M2DPic::Init(float x,float y,float width,float height)
 {

     return true;
 };
//������ʾ����
 void M2DPic::SetTexture(LMTex pTex) 
 {
 };
//�ͷ�����
void M2DPic::Release()
{
	OGRE_RELEASE(m_Data.pTex);
};
//��Ⱦ
void M2DPic::Render() 
{


};