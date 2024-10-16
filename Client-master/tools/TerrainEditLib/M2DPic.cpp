
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
//引导纹理文件(主要用于用UI接口显示纹理图片) ;参数( 文件名, 显示坐标 X,Y ,显示宽度, 高度 );返回( 为真表示成功 )	
bool M2DPic::LoadTexFile(const char *filename,float x,float y ,float width ,float height)
{
	TextureData *ptex = LoadTextureFromFile(filename);

	m_Data.pTex = ptex;
	return true;
};
//设置边框颜色	
void        M2DPic::SetColor(int  color)
{
};
//设置显示坐标	
void        M2DPic::SetPos(float x ,float y )
{
};
//设置宽度高度	
void        M2DPic::SetWidthHeight(float width ,float height)
{
};
//初始化 ; 参数( X,Y,宽度,高度 ) ; 返回( 为真 表示成功)
bool M2DPic::Init(float x,float y,float width,float height)
 {

     return true;
 };
//设置显示纹理
 void M2DPic::SetTexture(LMTex pTex) 
 {
 };
//释放数据
void M2DPic::Release()
{
	OGRE_RELEASE(m_Data.pTex);
};
//渲染
void M2DPic::Render() 
{


};