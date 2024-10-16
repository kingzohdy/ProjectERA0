
#pragma once
#include "TerrainEditTypes.h"

namespace TE
{
//2D图片数据
struct Pic2DData
{
	void*            texRes;//纹理资源指针
	LMVB  mvb;//对应顶点BUF
	
	LMVB        vb;//VB
	LMTex       pTex;//纹理指针
	unsigned int dwColor;//颜色
	int       nX,nY;//坐标
	int       nWidth,nHeight;//宽度,高度
	char      filename[128];//文件名
  	MIInputLayout   *pLayout;
	MIShader         *pShader;  // SHADER
};

class M2DPic
{
public:
	M2DPic(void);
	~M2DPic(void);

	virtual bool Init(float x,float y,float widht,float height) ;
	virtual void SetTexture(LMTex pTex) ;

	virtual void Release();
	virtual void Render() ;

	bool        LoadTexFile(const char *filename,float x,float y ,float widht,float hegiht);
	Pic2DData*   GetData(){ return &m_Data ; }

	void        SetColor(int  color);
	void        SetPos(float x ,float y );
	void        SetWidthHeight(float width ,float height);
private:
    //2D图片数据
	Pic2DData   m_Data;
	
};

};
