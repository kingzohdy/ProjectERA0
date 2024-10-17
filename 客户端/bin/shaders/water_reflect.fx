#include "header.fx"

float4		g_DeepWaterColor;
float4		g_ShallowWaterColor;
float		g_NumTexRepeat;
float		g_fSpeed;
float		g_fAmp;
texture		g_Normaltexture;
texture		g_refelcttexture;
float4x4	g_WorldViewProj : WORLDVIEWPROJ;

float		g_maxdepth;
float		g_Shallow;
float		g_Depth;

texture		g_WaterDepthTex;
sampler	DepthSampler = sampler_state
{
    Texture = <g_WaterDepthTex>;
    MipFilter = NONE;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU = clamp;
    AddressV = clamp;
};

sampler ReflectSampler = sampler_state
{
    Texture = <g_refelcttexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU = clamp;
    AddressV = clamp;
};

/*
texture         g_RefractTexture;
sampler RefractSampler = sampler_state
{
    Texture = <g_RefractTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU = clamp;
    AddressV = clamp;
};
*/

sampler	NormalSampler = sampler_state
{
    Texture = <g_Normaltexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU = wrap;
    AddressV = wrap;
};

struct VS_INPUT
{
    float4 pos        : POSITION;
    float2 uv0        : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 pos          : POSITION;
    float2 bumpuv       : TEXCOORD0;
    float4 reflectxy_refractyx : TEXCOORD1;
    float W : TEXCOORD2;
    float3 eyevec : TEXCOORD3;
    
#if FOG_HEIGHT>0 || FOG_DISTANCE>0
	float2 fogc	: TEXCOORD4;
#endif
};

VS_OUTPUT VSMain(VS_INPUT input)
{
	VS_OUTPUT output;

	float4 projpos = mul(input.pos, g_WorldViewProj);
	output.pos = projpos;

	//output.bumpuv = input.uv0*g_NumTexRepeat + float2(g_fSpeed*g_fTime, -g_fSpeed*g_fTime)*0.3;
	output.bumpuv = input.uv0;

	float2 reflect_pos = (float2(-projpos.x, -projpos.y) + projpos.w) * 0.5;
	float2 refract_pos = (float2(projpos.x, -projpos.y) + projpos.w) * 0.5;
	output.reflectxy_refractyx = float4( reflect_pos.x, reflect_pos.y, refract_pos.y, refract_pos.x);
	output.W = projpos.w;

	output.eyevec = normalize(g_EyePos.xyz - input.pos.xyz);
	
#if FOG_HEIGHT>0 || FOG_DISTANCE>0
	output.fogc = DoFog(input.pos);
#endif

	return output;
}

float4 PSMain(VS_OUTPUT input) : COLOR0
{
	g_fSpeed *= 0.7f;
	g_NumTexRepeat *= 0.7f;
	//float3 normal = tex2D(NormalSampler, input.bumpuv)*2.0 - 1.0;
	float3 nor1 = tex2D( NormalSampler,g_NumTexRepeat*input.bumpuv + float2(0.3*g_fSpeed*g_fTime,0.0f) );
	float3 nor2 = tex2D( NormalSampler,g_NumTexRepeat*input.bumpuv + float2(-0.3*g_fSpeed*g_fTime,0.5f) );
	//float3 nor3 = tex2D( NormalSampler,g_NumTexRepeat*input.bumpuv + float2(0.3f,0.3*g_fSpeed*g_fTime) );
	//float3 nor4 = tex2D( NormalSampler,g_NumTexRepeat*input.bumpuv + float2(0.7f,-0.3*g_fSpeed*g_fTime) );
	//float3 normal = 0.25*(nor1+nor2+nor3+nor4);
	nor1 = nor1*2.0 - 1.0;
	nor2 = nor2*2.0 - 1.0;
	float3 normal = nor2 - nor1;
	normal *= 0.5f;
	//normal = normal*2.0 - 1.0;

	float alpha = tex2D(DepthSampler,input.bumpuv).a;
	float4 watercolor = lerp(g_ShallowWaterColor, g_DeepWaterColor, alpha);

	float ooW = 1.0f / input.W;
	float4 uv = normal.xyxy*g_fAmp*0.01f + input.reflectxy_refractyx*ooW;

	float4 reflect_color = tex2D(ReflectSampler, uv.xy);

	float3 eyevec = normalize(input.eyevec);
	normal = float3(normal.x*g_fAmp, normal.y*g_fAmp, normal.z);

	/*
	//float4 refract_color = tex2D(RefractSampler, uv.wz);
	//float fNdotV = saturate(dot(eyevec, float3(0, 1.0, 0)));
	float fNdotV = saturate(dot(eyevec, normal.xzy));
	float fFresnel = 1.0 - fNdotV;

	refract_color *= g_DeepWaterColor;
	reflect_color *= g_ShallowWaterColor;
	float4 color = lerp(refract_color, reflect_color, fFresnel);
*/
	float4 color = reflect_color*watercolor;
	
	float diffusecont = dot(g_Lights[0].position.xyz, normal.xzy);
	color += diffusecont*g_SpecularColor*0.15f;

	float3 viewdir = reflect(eyevec, normal.xzy);
	float spec = max(dot(-viewdir, g_Lights[0].position.xyz),0.0f);
	spec = pow(spec,4.0);
	color += spec*g_SpecularColor*0.5;

#if FOG_DISTANCE>0
	color.rgb = lerp(g_DistFogColor, color.rgb, input.fogc.x);
#endif
#if FOG_HEIGHT > 0
	color.rgb = lerp(g_HeightFogColor, color.rgb, input.fogc.y);
#endif
	return 	float4(color.rgb, watercolor.a);
}

technique GeneralTech
<
    int usage = RENDER_USAGE_UI|RENDER_USAGE_GENERAL;
    int lod = 0;
>
{
    pass P1
    {    
        AlphaBlendEnable = TRUE;
	CullMode = CW;
	ZWriteEnable = FALSE;
	SrcBlend = SRCALPHA;
	DestBlend = INVSRCALPHA;

        VertexShader = compile vs_2_0 VSMain();
        PixelShader = compile ps_2_0 PSMain();   
    }
}
