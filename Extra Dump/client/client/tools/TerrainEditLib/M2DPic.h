
#pragma once
#include "TerrainEditTypes.h"

namespace TE
{
//2DͼƬ����
struct Pic2DData
{
	void*            texRes;//������Դָ��
	LMVB  mvb;//��Ӧ����BUF
	
	LMVB        vb;//VB
	LMTex       pTex;//����ָ��
	unsigned int dwColor;//��ɫ
	int       nX,nY;//����
	int       nWidth,nHeight;//���,�߶�
	char      filename[128];//�ļ���
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
    //2DͼƬ����
	Pic2DData   m_Data;
	
};

};
