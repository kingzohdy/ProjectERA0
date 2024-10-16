#include ".\mrender.h"

#ifdef _DEBUG001

MRender g_Render;

MRender::MRender(void)
{
	m_pDev = NULL;
}

MRender::~MRender(void)
{
//	MSAFE_RELEASE(m_pDev);
}

   
inline void     MRender::SetLightStatus( int index , bool sign)
{
   m_pDev->LightEnable(index,sign);
};
   
inline void     MRender::SetLight(int index , DWORD type , D3DCOLORVALUE &color, MVec3 &dir, MVec3 &pos,float range)
{
	D3DLIGHT9 light;

    ZeroMemory( &light, sizeof(D3DLIGHT9) );
    light.Type        = (D3DLIGHTTYPE)type;
    light.Diffuse   = color;

    D3DXVec3Normalize( (D3DXVECTOR3*)&light.Direction, &dir );
    light.Position   = pos;
    light.Range        = range;

    m_pDev->SetLight( index, &light ); 
	m_pDev->LightEnable( 0, TRUE );
};
	   
inline void   MRender::SetMaterial(float r,float g,float b)
{
    D3DMATERIAL9 mtrl;   
	ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
    mtrl.Diffuse.r = mtrl.Ambient.r = r;
    mtrl.Diffuse.g = mtrl.Ambient.g = g;
    mtrl.Diffuse.b = mtrl.Ambient.b = b;
    mtrl.Diffuse.a = mtrl.Ambient.a = 1.0; 

	m_pDev->SetMaterial( &mtrl );
};
inline bool    MRender::Begin()
{   
	if( SUCCEEDED( m_pDev->BeginScene() ) )
	{
		return true;
	}
	return false;
};
	
inline void   MRender::End()
{
	m_pDev->EndScene();
    m_pDev->Present( NULL, NULL, NULL, NULL );
};

	
inline bool    MRender::ClearScreen(DWORD backColor)
{
   	m_pDev->Clear ( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,
                        backColor, 1.0f, 0L ); 

	return true;
};
		
	
inline void    MRender::SetZenble(bool sign)
{
   	m_pDev->SetRenderState( D3DRS_ZENABLE, sign );
};
	
inline void   MRender::SetAmbient(DWORD color)
{
    m_pDev->SetRenderState( D3DRS_AMBIENT, color);
};

	
inline void    MRender::SetWorldMatrix(MMatrix *mat)
{
    m_pDev->SetTransform( D3DTS_WORLD, mat);
};
	
inline void    MRender::GetWorldMatrix(MMatrix *mat)
{
   m_pDev->GetTransform( D3DTS_WORLD, mat );
};

	
inline void    MRender::SetAlphaBlend(DWORD blend1,DWORD blend2)
{
		m_pDev->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );
		m_pDev->SetRenderState( D3DRS_SRCBLEND,  blend1);
		m_pDev->SetRenderState( D3DRS_DESTBLEND,  blend2 );

};
 
inline void    MRender::DisableAlphaBlend()
{
		m_pDev->SetRenderState( D3DRS_ALPHABLENDENABLE,   FALSE );
};

   
inline void    MRender::SetTexColor(DWORD stage,DWORD op,DWORD arg1,DWORD arg2)
{
		m_pDev->SetTextureStageState( stage, D3DTSS_COLOROP,   op);	
		m_pDev->SetTextureStageState( stage, D3DTSS_COLORARG1, arg1 );
		m_pDev->SetTextureStageState( stage, D3DTSS_COLORARG2, arg2 );
};
    
inline void    MRender::EnableLight(bool sign)
{
	 m_pDev->SetRenderState ( D3DRS_LIGHTING , sign);
};
inline void    MRender::DisableTexColor(DWORD stage)
{
		m_pDev->SetTextureStageState( stage, D3DTSS_COLOROP,   D3DTOP_DISABLE);	
};

   
inline void    MRender::SetTexAlpha(DWORD stage,DWORD op,DWORD arg1,DWORD arg2)
{
		m_pDev->SetTextureStageState( stage, D3DTSS_ALPHAOP,  op);
		m_pDev->SetTextureStageState( stage, D3DTSS_ALPHAARG1, arg1 );
		m_pDev->SetTextureStageState( stage, D3DTSS_ALPHAARG2, arg2 );
};
 
inline void    MRender::DisableTexAlpha(DWORD stage)
{
		m_pDev->SetTextureStageState( stage, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
};
 

inline void    MRender::SetTexFilter(DWORD stage,DWORD texFilterType)
{
    m_pDev->SetSamplerState( stage, D3DSAMP_MINFILTER, texFilterType );
    m_pDev->SetSamplerState( stage, D3DSAMP_MAGFILTER, texFilterType );
    m_pDev->SetSamplerState( stage, D3DSAMP_MIPFILTER,texFilterType );
};

	
inline void   MRender::SetCullMode(DWORD mode)
{
	m_pDev->SetRenderState( D3DRS_CULLMODE, mode);
};
	
inline void   MRender::SetFillMode(DWORD mode)
{
	m_pDev->SetRenderState( D3DRS_FILLMODE,mode);
};

inline void    MRender::SetAddressUV(DWORD stage,DWORD texAddressType)
{
		m_pDev->SetSamplerState( stage,  D3DSAMP_ADDRESSU,   texAddressType );
		m_pDev->SetSamplerState( stage,  D3DSAMP_ADDRESSV,   texAddressType); 
};
  
 inline void   MRender::SetTexture(DWORD stage,LMTex tex) 
 {		
	 m_pDev->SetTexture(stage,tex);	
 };
  
inline void   MRender::Draw(DWORD drawMode,DWORD FVF,DWORD FVFsize,LMVB vb,int verStart ,int triNum)
 {
	m_pDev->SetFVF(FVF);					
	m_pDev->SetStreamSource(0, vb, 0, FVFsize);             
	m_pDev->DrawPrimitive( 
					D3DPRIMITIVETYPE(drawMode),
					verStart,
					triNum);
 };

inline void   MRender::DrawIndex(DWORD drawMode,DWORD FVF,DWORD FVFsize,LMVB vb,LMIB ib,int verStart,int verNum,int indexStart,int triNum) 
{
       SetVB(vb,FVF,FVFsize);
	   SetIB(ib);
	   DrawIndexTri(drawMode,verStart,verNum,indexStart,triNum);
};

   
inline void     MRender::SetVB(LMVB vb,DWORD FVF,DWORD FVFsize)
{
	m_pDev->SetFVF(FVF);	
	m_pDev->SetStreamSource(0, vb, 0, FVFsize);             

};
   
inline void     MRender::SetIB(LMIB ib)
{
	m_pDev->SetIndices(ib);
};

      
inline void    MRender::SetTexCoordIndex(DWORD stage,DWORD index)
{
	m_pDev->SetTextureStageState( stage, D3DTSS_TEXCOORDINDEX, index );	/// 0号纹理：使用0号纹理索引
	
};
   
inline void    MRender::SetAlphaTest(bool sign)
{
   m_pDev->SetRenderState (D3DRS_ALPHATESTENABLE, sign);
};
inline void      MRender::DrawIndexTri(DWORD drawMode,int verStart,int verNum,int indexStart,int triNum)
{
		
	m_pDev->DrawIndexedPrimitive(D3DPRIMITIVETYPE(drawMode),
									0,
									verStart,
									verNum,		
									indexStart,
									triNum
									);
};
   
inline void      MRender::DrawTri(DWORD drawMode,int verStart,int triNum)
{
	m_pDev->DrawPrimitive( 
					D3DPRIMITIVETYPE(drawMode),
					verStart,
					triNum);
};
#endif // _DEBUG
