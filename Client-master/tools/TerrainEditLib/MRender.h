#pragma once
#include "igame.h"
#include <windows.h>
/*
namespace MARK
{
class MRender
{
public:
	MRender(void);
	~MRender(void);

	bool    Begin();
	void    End();

	bool    ClearScreen(DWORD backColor);
		
	void    SetZenble(bool sign);
	void   SetAmbient(DWORD color);

	void   SetCullMode(DWORD mode);
	void   SetFillMode(DWORD mode);

	void    SetWorldMatrix(MMatrix *mat);
	void    GetWorldMatrix(MMatrix *mat);

	void    SetAlphaBlend(DWORD blend1,DWORD blend2);
    void    DisableAlphaBlend();

    void    SetTexColor(DWORD stage,DWORD op,DWORD arg1,DWORD arg2);
    void    DisableTexColor(DWORD stage);

    void    SetTexAlpha(DWORD stage,DWORD op,DWORD arg1,DWORD arg2);
    void    DisableTexAlpha(DWORD stage);
 
	void    SetTexFilter(DWORD stage,DWORD texFilterType);
	void    SetAddressUV(DWORD stage,DWORD texAddressType);
   
	void    SetTexture(DWORD stage,LMTex tex) ;
  
   void    SetVB(LMVB vb,DWORD FVF,DWORD FVFsize);
   void    SetIB(LMIB ib);

   void    SetTexCoordIndex(DWORD stage,DWORD index);
   void    SetAlphaTest(bool sign);

   void     DrawIndexTri(DWORD drawMode,int verStart,int verNum,int indexStart,int triNum);
   void     DrawTri(DWORD drawMode,int verStart,int triNum);

   void     Draw(DWORD drawMode,DWORD FVF,DWORD FVFsize,LMVB vb,int verSatrt ,int triNum);
   void     DrawIndex(DWORD drawMode,DWORD FVF,DWORD FVFsize,LMVB vb,LMIB ib,int verStart,int verNum,int indexStart,int triNum) ;

   void     EnableLight(bool sign);

   void     SetLightStatus( int index , bool sign);
   void     SetLight(int index , DWORD type , D3DCOLORVALUE &color, MVec3 &dir, MVec3 &pos,float range);

   void     SetMaterial(float r,float g,float b);
public:

	LMDev    GetDev(){ return m_pDev;};
	void     SetDev(LMDev dev){ m_pDev = dev;}

private:
	LMDev    m_pDev;

};



};
extern MARK::MRender g_Render;
*/

