
#include "header.fx"

float4x4 g_WorldViewProj : WORLDVIEWPROJ;
float4  g_PosExcursion;
float3  g_Normal = float3(0,1,0);

struct VS_INPUT
{
	float4 pos			: POSITION;
	float4 color			: COLOR0;
	float2 uv0			: TEXCOORD0;
};

struct VS_OUTPUTPS
{
	float4 pos      : POSITION;
	float4 color0        : COLOR0; //color0.xyz=ambient lighting,  color0.w=specular lighting
	float2 uv0      : TEXCOORD0;
	float4 color1        : COLOR1; //color1.xyz=shadowed_lighting,  color1.w=specular lighting
#ifdef LIGHT_SHADOW
	float4 uv1            : TEXCOORD1;
#endif

#if FOG_HEIGHT>0 || FOG_DISTANCE>0
	float2 fogc	: TEXCOORD2;
#endif
	float2 fogh: TEXCOORD5;

};

VS_OUTPUTPS VSMain(VS_INPUT input)
{
	VS_OUTPUTPS output;

	output.uv0 =  input.uv0;

	float4 position = input.pos;
	float dsin = sin(g_PosExcursion.x+input.pos.y);
	float ddsinColor = abs(dsin);

	float v = 1.0 - input.uv0.y;
	position.x += 9*dsin*v;
	output.pos  = mul(position, g_WorldViewProj);

	float colordiff = 0.2*ddsinColor*v;
	colordiff = 0;

	output.color0 = DoLighting(input.pos, float3(0,1,0), 0)-colordiff;
	output.color1 = DoLighting(input.pos, float3(0,1,0), 1);


#ifdef LIGHT_SHADOW
	output.uv1  = CalShadowProjUV(input.pos);
#endif

#if FOG_HEIGHT>0 || FOG_DISTANCE>0
	output.fogc = DoFog(input.pos);
#endif
	if (g_warfogparameter[0]>0)
	{
		float4 guanpos= mul(float4(input.pos.xyz,1.0), g_World);
		float temp=g_warfogparameter[0];
		float temp2;
		int l1,l2;l1=0;l2=0;
		for(int i= 0;i<g_referencepoint[3][3];i++)
		{
			temp2=(guanpos.z-g_referencepoint[l1][l2+1])*(guanpos.z-g_referencepoint[l1][l2+1])+(guanpos.x-g_referencepoint[l1][l2])*(guanpos.x-g_referencepoint[l1][l2]);
			l2=l2+2;
			if (l2>3)
			{
				l2=l2-4;
				l1=l1+1;
			}
			if (temp2<temp) temp=temp2;
		}
		temp = temp/g_warfogparameter[0];
		if (temp > g_warfogparameter[2])
			temp=g_warfogparameter[2];
		output.fogh.xy = float2(temp,temp);
	}

   return output;
}
