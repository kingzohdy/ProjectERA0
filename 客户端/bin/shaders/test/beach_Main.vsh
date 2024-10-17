#include "header.fx"

float4x4 g_WorldViewProj : WORLDVIEWPROJ;
float4x4 g_DecalMatrix;
float4x4 g_DecalMaskMatrix;

float g_uvTrans = 0.1f;

struct VS_OUTPUT
{
    float4 pos      : POSITION;
    float4 color0   : COLOR0;
    float4 uv0      : TEXCOORD0;
    float4 uv1      : TEXCOORD1;
};

VS_OUTPUT VSMain( float4 pos : POSITION,
                  float4 color0 : COLOR,
		  float2 uv : TEXCOORD0,
		  float2 uv1 : TEXCOORD1
                          )
{
	VS_OUTPUT output;
    
	output.pos = mul( float4(pos.xyz,1.0f), g_WorldViewProj);
    
	output.color0 = color0;
	output.uv0 = float4(uv.x, uv.y, 1.0f, 1.0f);
	
	output.uv0.y += g_uvTrans;

	output.uv1 = float4(uv1.x, uv1.y, 1.0f, 1.0f);

	return output;
}
