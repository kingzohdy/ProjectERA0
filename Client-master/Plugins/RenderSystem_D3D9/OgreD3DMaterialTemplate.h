
#ifndef __D3DMaterialTemplate_H__
#define __D3DMaterialTemplate_H__

#include "OgreD3D9Prerequisites.h"
#include "OgreMaterialTemplate.h"
#include "OgreLocker.h"
#include "Md5.h"

#define OGRE_SHADER_PATH "shaders\\test\\"

namespace Ogre
{
	class CompiledShader;
	class CompiledShaderGroup;
	class D3DShaderTechnique;
	class D3DTech;

	enum COMPILED_TYPE
	{
		COMPILED_EFFECT = 0,
		COMPILED_VS,
		COMPILED_PS
	};

	inline bool operator<(const ShaderEnvKey &key1, const ShaderEnvKey &key2)
	{
		if(key1.keydata[0] < key2.keydata[0]) 
			return true;
		else if(key1.keydata[0] > key2.keydata[0]) 
			return false;
		else 
			return key1.keydata[1] < key2.keydata[1];
	}

	inline bool operator==(const ShaderEnvKey &key1, const ShaderEnvKey &key2)
	{
		return key1.keydata[0] == key2.keydata[0] && 
			key1.keydata[1] == key2.keydata[1];
	}

	union ShaderEnvKey2
	{
		struct
		{
			ShaderEnvFlags env;
			MaterialMacro macros;
			uint64 tmp;
		};
		uint64 keydata[3];

		ShaderEnvKey2()
		{
			keydata[0] = keydata[1] = keydata[2] = 0;
		}
	};

	inline bool operator<(const ShaderEnvKey2 &key1, const ShaderEnvKey2 &key2)
	{
		if(key1.keydata[0] < key2.keydata[0]) 
			return true;
		else if(key1.keydata[0] > key2.keydata[0]) 
			return false;
		else if(key1.keydata[1] < key2.keydata[1])
			return true;
		else if(key1.keydata[1] > key2.keydata[1])
			return false;
		else 
			return key1.keydata[2] < key2.keydata[2];
	}

	inline bool operator==(const ShaderEnvKey2 &key1, const ShaderEnvKey2 &key2)
	{
		return key1.keydata[0] == key2.keydata[0] && 
			key1.keydata[1] == key2.keydata[1] && key1.keydata[2] == key2.keydata[2];
	}

	class D3DMaterialTemplate : public MaterialTemplate
	{
	public:
		struct TechCache
		{
			uint lod;
			uint usage;
			FixedString name;
			std::map<ShaderEnvKey, D3DTech *>techmap;
			D3DTech *proto;
		};

	public:
		D3DMaterialTemplate(D3D9RenderSystem *prs, const FixedString &name);
		~D3DMaterialTemplate();

		virtual bool init(XMLNode node);

		ShaderTechnique *getShaderTechnique_old(const ShaderEnvFlags &env, const MaterialMacro &macros, uint lod=0, RenderUsage usage=RU_GENERAL);
		size_t getRequiredParams_old(ShaderParamUsage *pbuf, size_t maxbuf, const ShaderEnvFlags &env, const MaterialMacro &macros, uint lod=0, RenderUsage usage=RU_GENERAL);

		virtual ShaderTechnique *getShaderTechnique(const ShaderEnvFlags &env, const MaterialMacro &macros, uint lod=0, RenderUsage usage=RU_GENERAL);
		virtual size_t getRequiredParams(ShaderParamUsage *pbuf, size_t maxbuf, const ShaderEnvFlags &env, const MaterialMacro &macros, uint lod=0, RenderUsage usage=RU_GENERAL);

		CompiledShader *getCompiledVSPS(COMPILED_TYPE type, const char *name, const ShaderEnvFlags &env, const MaterialMacro &macros);

		void onLostDevice();
		bool onResetDevice();

		virtual void compileAllShaders();

		void loadCompiledShaders(FILE *fp, CMd5 &md5);
		void saveCompiledShaders(FILE *fp, CMd5 &md5);
		void loadCompiledShadersFromDataStream (DataStream *ds);

		void loadShaderEnvKeysFromDataStream (DataStream *ds);
		void saveShaderEnvKeys (FILE *fp);

	private:
		D3DMaterialTemplate::TechCache *findTechCache(uint lod, RenderUsage usage);

	public:
		CompiledShaderGroup *m_pEffectGroup;

	protected:
		D3D9RenderSystem *m_pRS;
		LockSection m_TechMapLock;

		std::map<FixedString, CompiledShaderGroup *>m_ShaderGroups;
		std::map<ShaderEnvKey2, D3DShaderTechnique *>m_Techniques;

		std::vector<TechCache *>m_TechCache;
	};

	class D3D9MaterialManager : public MaterialManager
	{
	public:
		D3D9MaterialManager(D3D9RenderSystem *prs);
		~D3D9MaterialManager();

		MaterialTemplate *newMtlTemplate(const FixedString &name)
		{
			return new D3DMaterialTemplate(m_pRS, name);
		}

		virtual ShaderTechnique *createRawShader(const FixedString &shader_path)
		{
			return NULL;
		}

		void onLostDevice();
		bool onResetDevice();

		void loadCompiledShaders();
		void saveCompiledShaders();
		void loadShaderEnvKeys ();
		void saveShaderEnvKeys ();

		void loadCompiledSahdersFromDataStream ();
		void saveCompiledSahdersToDataStream ();

	protected:
		D3D9RenderSystem *m_pRS;
	};
}

#endif