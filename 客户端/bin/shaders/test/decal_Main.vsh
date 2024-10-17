
#include "header.fx"

float4x4 g_WorldViewProj : WORLDVIEWPROJ;
float4x4 g_DecalMatrix;
float4x4 g_DecalMaskMatrix;

struct VS_OUTPUT
{
    float4 pos      : POSITION;
    float4 color0   : COLOR0;
    float4 uv0      : TEXCOORD0;
#if MASK_TEXTURE==1
    float4 uv1	    : TEXCOORD1;
#endif
};

VS_OUTPUT VSMain( float4 pos : POSITION,
                   float4 color0 : COLOR
                          )
{
	VS_OUTPUT output;
    
	output.pos = mul( float4(pos.xyz,1.0f), g_WorldViewProj);
    
	output.color0 = color0;
	output.uv0 = mul( float4(pos.xyz,1.0f), g_DecalMatrix);
#if MASK_TEXTURE==1
	output.uv1 = mul( float4(pos.xyz,1.0f), g_DecalMaskMatrix);
#endif	
	return output;
}
