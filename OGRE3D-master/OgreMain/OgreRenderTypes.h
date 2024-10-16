
#ifndef __RenderTypes_H__
#define __RenderTypes_H__

#include "OgrePrerequisites.h"
#include "OgreResource.h"

namespace Ogre
{
	enum RenderUsage
	{
		RU_UI		= 0,
		RU_GENERAL,
		RU_REFLECT,
		RU_SHADOWMAP,
		RU_GLOW,
		RU_DISTORT,
		RU_SHADOWCUBEMAP,
		RU_REFRACT,
		RU_TRANSABLE,
	};

	enum RenderLayer
	{
		RL_BACKGROUND = 0,
		RL_TERRAIN,
		RL_SCENE,
		RL_FRONT
	};


	enum PrimitiveType
	{
		PRIM_POINTLIST = 1,
		PRIM_LINELIST = 2,
		PRIM_LINESTRIP = 3,
		PRIM_TRIANGLELIST = 4,
		PRIM_TRIANGLESTRIP = 5,
		PRIM_TRIANGLEFAN = 6,
		PRIM_FORCE_DWORD = 0x7fffffff
	};

	enum ShaderParamType
	{
		SPT_FLOAT,
		SPT_FLOAT2,
		SPT_FLOAT3,
		SPT_FLOAT4,
		SPT_FLOAT3X3,
		SPT_TEXTURE,
		SPT_COLOR,
		SPT_FLOAT4X4,
		SPT_MACRO
	};

	enum ShaderParamUsage
	{
		SPU_INSTANCE_PARAM = 0,
		SPU_WORLD = SPU_INSTANCE_PARAM,
		SPU_WORLDVIEW,
		SPU_WORLDVIEWPROJ,
		SPU_DIRECTION,

		SPU_ENV_PARAM,
		SPU_AMBIENTLIGHT = SPU_ENV_PARAM,
		SPU_LIGHTDATA,
		SPU_SPECULARCOLOR,
		SPU_MORPH_RANGE_POS,
		SPU_MORPH_RANGE_UV,
		SPU_MORPH_TIME,
		SPU_BONE_MATRIX,

		SPU_FOG_PARAM,
		SPU_DIST_FOG_COLOR,
		SPU_HEIGHT_FOG_COLOR,
		SPU_EYE_POS,
		SPU_EYE_POS_MODEL,
		SPU_SHADOW_CENTER,
		SPU_DEPTH_PROJ,
		SPU_DEPTHMAP,
		SPU_SHADOW_DENSITY,
		SPU_SHADOWCUBEMAP,
		SPU_SHADOWCUBEMAP_POS,
		SPU_TERRAINDECAL,
		SPU_TIME,	
		SPU_REFLECTMAP,
		SPU_REFLECT_MATRIX,
		SPU_FRACMAP,
		SPU_LIGHTMAP,
		SPU_LIGHTMAP_MATRIX,
		SPU_LIGHT_MULTIPLY,
		SPU_AMBLIGHT_MULTIPLY,

		SPU_WATERPARA, 
		SPU_WATERCOLOR,
		SPU_WATERTURB,
		SPU_MODEL_XPARENT,
		SPU_LIGHT_DIR,

		SPU_DECAL_TEXTURE0,
		SPU_DECAL_TEXTURE1,
		SPU_DECAL_TEXTURE2,
		SPU_DECAL_TEXTURE3,
		SPU_DECAL_TEXTURE4,
		SPU_DECAL_TEXTURE5,
		SPU_DECAL_TEXTURE6,
		SPU_DECAL_TEXTURE7,

		SPU_DECAL_MATRIX,
		
		SPU_REFERENCEPOINT_POINT,
		SPU_WARFOG_PARAMETER,
		SPU_WARFOG_COLOR,
		SPU_ENV_PARAM_END,

		SPU_GENERAL_PARAM = 1000,
	};

	enum LightType
	{
		LT_NULL = 0,
		LT_POINT,
		LT_DIRECT,
		LT_SPOT
	};

	enum UVAddressMode
	{
		UVADDR_WRAP = 0,
		UVADDR_CLAMP,
		UVADDR_WRAPU_CLAMPV,
		UVADDR_CLAMPU_WRAPV
	};

	struct Viewport
	{
		float left;
		float top;
		float width;
		float height;
		float  minz;
		float  maxz;
	};

	typedef void (*ContextRendererCallback)(void* userdata);

	struct ContextQueDesc
	{
		ContextQueDesc() : ptarget(NULL), clearflags(0), clipflags(0), renderusage(RU_GENERAL), start(0), end(0)
		{
			viewport.left = viewport.top = viewport.minz = 0;
			viewport.width = viewport.height = viewport.maxz = 1.0f;
		}

		RenderTarget *ptarget;
		uint clearflags;
		uint clearcolor;
		float cleardepth;
		uint clearstencil;

		Viewport viewport;

		unsigned int clipflags;
		float clipplane[6][4];

		RenderUsage renderusage;

		size_t start;
		size_t end;
	};

	enum HardwareBufferUsage
	{
		//静态buffer， 创建一次不再修改
		HBU_STATIC = 0,

		//偶尔更新的buffer
		HBU_OCCASION,

		//每桢更新的buffer
		HBU_DYNAMIC,

		HBU_DEPTHSTENCIL,

		//由GPU更新的buffer
		HBU_RENDERTARGET,

		//只给cpu访问的buffer
		HBU_ONLYCPU
	};


	/// Locking options
	enum HardwareBufferLockOpt
	{
		/** Normal mode, ie allows read/write and contents are preserved. */
		HBL_NORMAL,
		/** Discards the <em>entire</em> buffer while locking; this allows optimisation to be 
		performed because synchronisation issues are relaxed. Only allowed on buffers 
		created with the HBU_DYNAMIC flag. 
		*/
		HBL_DISCARD,
		/** Lock the buffer for reading only. Not allowed in buffers which are created with HBU_WRITE_ONLY. 
		Mandatory on statuc buffers, ie those created without the HBU_DYNAMIC flag. 
		*/ 
		HBL_READ_ONLY,
		/** As HBL_NORMAL, except the application guarantees not to overwrite any 
		region of the buffer which has already been used in this frame, can allow
		some optimisation on some APIs. */
		HBL_NO_OVERWRITE

	};

	struct LockResult
	{
		uint stride;
		uint rowpitch;
		uint slicepitch;
	};

	class _OgreExport VertexBuffer : public Resource
	{
		DECLARE_RTTI_VIRTUAL(VertexBuffer)
	public:
		virtual HardwareVertexBuffer *getHBuf() = 0;
	};

	class _OgreExport IndexBuffer : public Resource
	{
		DECLARE_RTTI_VIRTUAL(IndexBuffer)
	public:
		virtual HardwareIndexBuffer *getHBuf() = 0;

		void setVertexRange(size_t start, size_t num)
		{
			m_RefVertStart = start;
			m_RefVertNum = num;
		}

	public:
		size_t m_RefVertStart;
		size_t m_RefVertNum;
	};


	class _OgreExport Texture : public Resource
	{
		DECLARE_RTTI_VIRTUAL(Texture)
	public:
		virtual void getDesc(TextureDesc &desc) = 0;
		virtual HardwareTexture *getHardwareTexture() = 0;
		virtual void *lock(size_t face, size_t level, bool readonly, LockResult &lockresult) = 0;
		virtual void unlock(size_t face, size_t level) = 0;

	protected:
		virtual ~Texture()
		{
		}
	};

	_OgreExport size_t nVertex2nPrimitive(PrimitiveType type, size_t nvert);
	_OgreExport Material *CreateParticleMaterial(int blendtype, Texture *ptexture, Texture *pmasktexture, int diffuse_sample, int mask_sample);
	_OgreExport const ColourValue & GetTransparentColor(const ColourValue &color, float alpha, int blendtype);

}

#endif