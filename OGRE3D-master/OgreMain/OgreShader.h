
#ifndef __Shader_H__
#define __Shader_H__

#include "OgreRenderTypes.h"
#include "OgreVector4.h"
#include "OgreMatrix4.h"
#include "OgreColourValue.h"
#include <vector>

namespace Ogre
{
	struct ShaderEnvFlags
	{
		unsigned int nlight : 3;
		unsigned int light0_type : 1;
		unsigned int light0_shadow : 1;
		unsigned int light0_specular : 1;
		unsigned int light1_type : 1;
		unsigned int light1_shadow : 1;
		unsigned int light1_specular : 1;
		unsigned int light2_type : 1;
		unsigned int light2_shadow : 1;
		unsigned int light2_specular : 1;
		unsigned int light3_type : 1;
		unsigned int light3_shadow : 1;
		unsigned int light3_specular : 1;

		unsigned int distfog : 1;
		unsigned int heightfog : 1;
		unsigned int log_shadowmap : 1;
		unsigned int skin_maxinfl : 3;
		unsigned int morph_pos : 1;
		unsigned int morph_uv0 : 1;
		unsigned int model_xparent : 1;

		unsigned int lightmap : 4;
		unsigned int vctype: 4;
		unsigned int transparent:4;
		unsigned int mrt:4; 
		unsigned int shaderlevel:4;
		unsigned int decalblendmode:4;
	};

	struct MaterialMacro
	{
		enum
		{
			MAX_MACRO = 4
		};
		uchar indices[MAX_MACRO];//材质的第几个参数
		uchar value[MAX_MACRO]; //宏的值
	};

	struct SED_LightData
	{
		ColourValue color;
		Vector4 position;
	};

	struct _OgreExport ShaderEnvData
	{
		ShaderEnvData();

		void clearFlags();

		ShaderEnvFlags flags;
		
		SED_LightData lightdata[4];
		ColourValue specular; //specular.a is power
		ColourValue ambient;
		Vector4 fogdist;//(distfog_near, distfog_far, heightfog_near,  heightfog_far)
		ColourValue distfogcolor;
		ColourValue heightfogcolor;

		Texture*		pdepthtexture;
		Texture*		preflecttexture;
		Texture*        prefracttexture;
		Texture*		pscmtexture;
		Vector3			pscmpos;
		Matrix4			depthproj;
		Matrix4			reflectproj;
		Matrix4         refractproj;
		float			shadowdensity;

		Matrix4 decaltm[8];

		Matrix4 view;
		Matrix4 proj;
		Matrix4 viewproj;
		Vector3 eyepos;
		Vector4 shadowcenter;
		float  referencepoint[16];//参考点，目前还只用于辅助实现战争迷雾提供玩家在shader中的世界坐标以此来计算可见度
		Vector3 warfogparameter;//战争迷雾的的参数，第一个为视距，第二个为未走过的地方的迷雾浓度，第二个为走过的地方的迷雾浓度
		Vector3 warfogcolor;

	};

	inline int GetEnvLightType(const ShaderEnvFlags &flags, int index)
	{
		if(index == 0) return flags.light0_type;
		else if(index == 1) return flags.light1_type;
		else if(index == 2) return flags.light2_type;
		else return flags.light3_type;
	}
	inline int GetEnvLightShadow(const ShaderEnvFlags &flags, int index)
	{
		if(index == 0) return flags.light0_shadow;
		else if(index == 1) return flags.light1_shadow;
		else if(index == 2) return flags.light2_shadow;
		else return flags.light3_shadow;
	}
	inline int GetEnvLightSpecular(const ShaderEnvFlags &flags, int index)
	{
		if(index == 0) return flags.light0_specular;
		else if(index == 1) return flags.light1_specular;
		else if(index == 2) return flags.light2_specular;
		else return flags.light3_specular;
	}


	inline void SetEnvLightFlags(ShaderEnvFlags &flags, int lighttype, bool specular, bool shadow)
	{
		if(flags.nlight == 0)
		{
			flags.light0_type = lighttype;
			flags.light0_specular = specular;
			flags.light0_shadow = shadow;
		}
		else if(flags.nlight == 1)
		{
			flags.light1_type = lighttype;
			flags.light1_specular = specular;
			flags.light1_shadow = shadow;
		}
		else if(flags.nlight == 2)
		{
			flags.light2_type = lighttype;
			flags.light2_specular = specular;
			flags.light2_shadow = shadow;
		}
		else
		{
			flags.light3_type = lighttype;
			flags.light3_specular = specular;
			flags.light3_shadow = shadow;
		}
	}

	/*
	class _OgreExport ShaderParamBlock
	{
	public:
		size_t addLocalParam(size_t localindex, const void *pdata, size_t datalen);
		size_t addEnvParam(ShaderEnvData envtype, const void *pdata, size_t datalen);

	protected:
		struct Parameter
		{
			ShaderParamUsage envtype;
			int localindex;

			void *pobject;
			size_t offset;
			size_t len;
		};

		std::vector<char>m_Buffer;
		std::vector<Parameter>m_Params;

	private:
	};*/

	class ShaderTechnique
	{
	public:
		virtual size_t begin() = 0;
		virtual void end() = 0;
		virtual void beginPass( size_t ipass ) = 0;
		virtual void endPass() = 0;

		virtual void setConstant(const char *pname, const void *pdata, size_t nbytes) = 0;
		virtual void setConstant( ShaderParamUsage type, const void *pdata, size_t nbytes) = 0;
		virtual void setTexture( const char *pname, HardwareTexture *ptexture ) = 0;
		virtual void setTexture(ShaderParamUsage type, HardwareTexture *ptexture) = 0;
		virtual void setStreamInput(VertexDeclHandle vert_decl, HardwareVertexBuffer **ppvb, size_t *poffsetbytes=NULL) = 0;
		virtual void setStreamInput(VertexDeclHandle vert_decl, HardwareVertexBuffer *pvb, size_t offset=0) = 0;
		virtual void setStreamInput(VertexDeclHandle vert_decl, HardwareVertexBuffer **ppvb,int nNumInstance,size_t *poffsetbytes) =0;
		virtual void commitChanges() = 0;
		virtual void draw(PrimitiveType primtype, size_t start_vert, size_t nprimitive) = 0;
		virtual void draw(PrimitiveType primtype, size_t start_vert, size_t nprimitive, HardwareIndexBuffer *pib, size_t refvert_start, size_t refvert_num) = 0;
		virtual size_t getRequiredParams(ShaderParamUsage *pbuf, size_t maxbuf) = 0;

	protected:
		virtual ~ShaderTechnique(){}
	};

	class ShaderComp
	{
	public:
		virtual size_t begin() = 0;
		virtual void end() = 0;
		virtual void setVSConstant() = 0;
		virtual void setPSConstant() = 0;
		virtual void setTexture() = 0;
		virtual void setStreamInput(VertexDeclHandle vert_decl, HardwareVertexBuffer **ppvb, size_t *poffsetbytes=NULL) = 0;
		virtual void setStreamInput(VertexDeclHandle vert_decl, HardwareVertexBuffer *pvb, size_t offset=0) = 0;
		virtual void setStreamInput(VertexDeclHandle vert_decl, HardwareVertexBuffer **ppvb,int nNumInstance,size_t *poffsetbytes) =0;
		virtual void draw(PrimitiveType primtype, size_t start_vert, size_t nprimitive) = 0;
		virtual void draw(PrimitiveType primtype, size_t start_vert, size_t nprimitive, HardwareIndexBuffer *pib, size_t refvert_start, size_t refvert_num) = 0;	
	};

	class RenderStates
	{

	};

	class SamplerStates
	{

	};

}

#endif