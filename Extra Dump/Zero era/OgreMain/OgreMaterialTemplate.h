
#ifndef __MaterialTemplate_H__
#define __MaterialTemplate_H__

#include "OgreXMLData.h"
#include "OgreFixedString.h"
#include "OgreShader.h"
#include "OgreSingleton.h"
#include <map>
#include <vector>

namespace Ogre
{
	union ShaderEnvKey
	{
		struct
		{
			ShaderEnvFlags env;
			MaterialMacro macros;
		};
		uint64 keydata[2];

		ShaderEnvKey()
		{
			keydata[0] = keydata[1] = 0;
		}
	};

	// ShaderEnv
	struct ShaderEnv
	{
		uint Type;
		ShaderEnvKey key;
	};

	class MaterialParam;
	class _OgreExport MaterialTemplate
	{
	public:
		struct ParamDefine
		{
			FixedString name;
			ShaderParamType type;
			float value[16];
		};

	public:
		MaterialTemplate(const FixedString &name);
		virtual ~MaterialTemplate();

		virtual bool init(XMLNode node);

		virtual void compileAllShaders() = 0;

		virtual ShaderTechnique *getShaderTechnique(const ShaderEnvFlags &env, const MaterialMacro &macros, uint lod=0, RenderUsage usage=RU_GENERAL) = 0;
		virtual size_t getRequiredParams(ShaderParamUsage *pbuf, size_t maxbuf, const ShaderEnvFlags &env, const MaterialMacro &macros, uint lod=0, RenderUsage usage=RU_GENERAL) = 0;
		
		const FixedString &getName()
		{
			return m_Name;
		}

		size_t getNumParam()
		{
			return m_Params.size();
		}

		ParamDefine *getParam(size_t i)
		{
			assert(i < m_Params.size());
			return m_Params[i];
		}

		void getDefaultParams(std::vector<MaterialParam*>&params);

		int findParamByName(const FixedString &name);

	public:
		bool m_bTransparent;
		bool m_bRecvLight;
		bool m_bRecvShadow;
		bool m_bCastShadow;
		bool m_bRecvFog;

		std::vector<ShaderEnv> m_ShaderEnvs;

	protected:
		FixedString m_Name;
		std::vector<ParamDefine *>m_Params;
	};

	class _OgreExport MaterialManager : public Singleton<MaterialManager>
	{
	public:
		MaterialManager();
		virtual ~MaterialManager();

		// 设置当前的地图id，用来做shaderchache随着地图加载
		void setCurMapID (int id)
		{
			m_CurMapID = id;
		}

		int getCurMapID ()
		{
			return m_CurMapID;
		}

		bool loadTemplates(const String &path);
		MaterialTemplate *getMtlTemplate(const FixedString &name);

		virtual ShaderTechnique *createRawShader(const FixedString &shader_path) = 0;

		virtual void compileAllShaders();

	protected:
		bool loadOneTemplate(XMLNode mtlnode);
		virtual MaterialTemplate *newMtlTemplate(const FixedString &name) = 0;

		typedef std::map<FixedString, MaterialTemplate *> MaterialTemplateTable;
		typedef std::map<FixedString, MaterialTemplate *>::iterator MaterialTemplateIter;
		MaterialTemplateTable m_MtlTempTable;

		int m_CurMapID;
	};

	class _OgreExport DummyMaterialTemplate : public MaterialTemplate
	{
	public:
		DummyMaterialTemplate(const FixedString &name) : MaterialTemplate(name)
		{
		}

		virtual ShaderTechnique *getShaderTechnique(const ShaderEnvFlags &env, const MaterialMacro &macro, uint lod=0, RenderUsage usage=RU_GENERAL)
		{
			assert(0);
			return NULL;
		}

		virtual size_t getRequiredParams(ShaderParamUsage *pbuf, size_t maxbuf, const ShaderEnvFlags &env, const MaterialMacro &macro, uint lod=0, RenderUsage usage=RU_GENERAL)
		{
			assert(0);
			return 0;
		}

		virtual void compileShaderOnMap (int id) 
		{
			assert(0);
			return;
		}

		virtual void compileAllShaders()
		{
			assert(0);
			return;
		}
	};

	class _OgreExport DummyMaterialManager : public MaterialManager
	{
	public:
		virtual MaterialTemplate *newMtlTemplate(const FixedString &name)
		{
			return new DummyMaterialTemplate(name);
		}
		virtual void precompileShaders()
		{
		}
		virtual ShaderTechnique *createRawShader(const FixedString &shader_path)
		{
			assert(0);	//未实现
			return NULL;
		}
	};
}

#endif