#include "header.fx"

float4x4 g_WorldViewProj : WORLDVIEWPROJ;

struct VS_OUTPUT
{
    float4 pos      : POSITION;
    float4 color0   : COLOR0;
    float2 uv0      : TEXCOORD0;
#if MASK_TEXTURE == 1
    float2 uv1		: TEXCOORD1;
#endif
};

VS_OUTPUT VSMain( float4 pos : POSITION,
                   float4 color0 : COLOR,
                   float2 uv0 : TEXCOORD0,
                   float2 uv1 : TEXCOORD1
                          )
{
	VS_OUTPUT output;
    
	output.pos = mul( float4(pos.xyz,1.0f), g_WorldViewProj);
    
	output.color0 = color0;

    
	output.uv0 = uv0;
#if MASK_TEXTURE == 1
	output.uv1 = uv1;
#endif
	return output;
}
