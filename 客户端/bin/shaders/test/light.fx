


#if NUM_LIGHT > 0
Light g_Lights[NUM_LIGHT] : LIGHTS;
#endif
float g_Shiness : SPECULAR_POWER;

float4 DoDiffuseLight(Light light, float3 pos, float3 normal)
{
	if(light.type == LT_POINTLIGHT)
	{
		float3 l = light.position.xyz - pos;
		float r2 = dot(l, l);
		l = normalize(l);
		return light.color * max(0, dot(normal, l)) * max( 0, (1.0 - r2 / light.position.w / light.position.w) );
	}
	else if(light.type == LT_DIRLIGHT)
	{
		float3 dir = normalize(light.dir);
		return light.color * max(0, dot(normal, dir));
	}
	else
	{
		return light.color;
	}
}

float DoSpecularLight(Light light, float3 pos, float3 normal)
{
	float3 viewdir = normalize(g_EyePos - pos);
	
	if(light.type == LT_POINTLIGHT)
	{
		float3 l = light.position.xyz - pos;
		float spec = max(dot(viewdir, normalize(l)), 0);
		spec = pow(spec, g_Shiness);

		float r2 = dot(l, l);
		return spec * max(0, (1.0 - r2/light.position.w/light.position.w));
	}
	else if(light.type == LT_DIRLIGHT)
	{
		float spec = max(dot(viewdir, -light.dir.xyz),0.0f);
		spec = pow(spec,g_Shiness);
		return spec;
	}
	else
	{
		return 0;
	}
}

float DoVertexLighting(float3 pos, float3 normal, output float4 shadowed_diffuse, output float4 diffuse)
{
	shadowed_diffuse = float4(0, 0, 0, 0);
	diffuse = float4(0, 0, 0, 0);
	
	for(int i=0; i<NUM_LIGHT; i++)
	{
		if(g_Lights[i].do_shadow)
		{
			shadowed_diffuse += DoDiffuseLight(g_Lights[i], pos, normal);
		}
		else diffuse += DoDiffuseLight(g_Lights[i], pos, normal);
	}

	float specular = 0;
	for(int i=0; i<NUM_LIGHT; i++)
	{
		if(g_Lights[i].do_specular) specular += DoSpecularLight(g_Lights[i], pos, normal);
	}
	
	return specular;
}


#if ATI_NV == 0
float DoSoftShadow(sampler s_DepthMapSampler,float4 uv,float z)
{
	float shadow = 0.0f; 
	for(int i=-1;i<2;i++)
		for(int j=-1;j<2;j++)
		{
			float2 texuv = uv.w * float2(i/1024.0f,j/1024.0f);
			shadow += tex2Dproj( s_DepthMapSampler, uv + float4(texuv,0.0f,0.0f) ).x;//
		}
	return shadow*0.1111f;
}

float DoHardShadow(sampler s_DepthMapSampler,float4 uv,float z )
{
	float shadow = 0.0f; 
	for(int i=-1;i<1;i++)
		for(int j=-1;j<1;j++)
		{
			float2 texuv = uv.w * float2(i/1024.0f,j/1024.0f);
			shadow += tex2Dproj( s_DepthMapSampler, uv + float4(texuv,0.0f,0.0f) ).x;//
		}
	return shadow*0.25f;
}

#elif ATI_NV == 1
float TexShadow(sampler s_DepthMapSampler,float4 uv,float z)
{
	float2 Tex = uv.xy/uv.w;
	
	float4 color = tex2D(s_DepthMapSampler,Tex);
	z *= 256.0f;
	float depth = color.x*255.0f + color.y;
	
	return (  depth < z)?0.0f:1.0f;
}

float DoSoftShadow(sampler s_DepthMapSampler,float4 uv,float z )
{
	float shadow = 0.0f; ; 
	for(int i=-1;i<2;i++)
		for(int j=-1;j<2;j++)
		{
			float2 texuv = uv.w * float2(i/1024.0f,j/1024.0f);
			shadow += TexShadow( s_DepthMapSampler, uv + float4(texuv,0.0f,0.0f),z );//
		}
	return shadow*0.1111f;
}

float DoHardShadow(sampler s_DepthMapSampler,float4 uv,float z )
{
	return TexShadow(s_DepthMapSampler, uv,z);
}

#endif		
 
