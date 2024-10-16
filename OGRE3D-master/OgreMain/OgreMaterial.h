
#ifndef __Material_H__
#define __Material_H__

#include "OgreResource.h"
#include "OgreShader.h"
#include "OgreFixedString.h"

namespace Ogre
{
	class MaterialParam
	{
	public:
		MaterialParam(ShaderParamType type);
		MaterialParam(const MaterialParam &rhs);
		~MaterialParam();

		void reset(ShaderParamType type);

		void *getValueAddr(){ return m_Value; }
		const void *getValueAddr() const{ return m_Value; }
		size_t getValueSize() const;
		void serialize(Archive &ar);

	public:
		ShaderParamType m_Type;
		FixedString m_Name;
		int m_Index;
		int m_ParamID;

		union
		{
			float m_Value[16];
			Texture *m_pTexture;
			int m_IntVal;
		};
	};

	/*
	class _OgreExport MaterialPass
	{
	public:
		void applyShaderParam(ShaderContext *pcontext);

		ShaderComp*		m_pCurShader;
		RenderStates	m_RenderStates;
		SamplerStates	m_SamplerStates;

	protected:
		virtual ~MaterialPass();

	};
	*/

	class _OgreExport Material : public Resource
	{
		DECLARE_RTTI(Material)
	public:
		Material();
		Material(const Material &rhs);
		Material(const FixedString &template_name);

		void setTemplateName(const char *pname);
		void setName(const char *pname);
		FixedString &getName()
		{
			return m_MtlName;
		}

		MaterialTemplate *getMtlTemplate()
		{
			return m_pMtlTemplate;
		}

		// specific for border material
		void setReplaceDiffuseTex (bool bReplace)
		{
			m_bNeedRelpaceDiffuseTexture = bReplace;
		}

		bool needReplaceDiffuseTexture()
		{
			return m_bNeedRelpaceDiffuseTexture;
		}

		Texture* GetParamTexture (FixedString strName);
		int GetParamMacro (FixedString strName);

		// end specific for border material

		void applyShaderParam(ShaderContext *pcontext);

		void setParam(const MaterialParam &param);
		void setParamMacro(const FixedString &param_name, int v);
		void setParamValue(const FixedString &param_name, const void *pdata);
		void setParamTexture(const FixedString &param_name, Texture *ptexture);
		void setParamTextureByID(int param_id, Texture *ptexture);
		void setParamValueByID(int param_id, const void *pdata);

		bool isTransparent () { return m_bTransparent; }

	protected:
		FixedString m_MtlName;
		FixedString m_TemplateName;
		MaterialTemplate *m_pMtlTemplate;
		bool m_bTransparent;
		bool m_bNeedRelpaceDiffuseTexture;

		std::vector<MaterialParam *>m_Params;

	protected:
		virtual ~Material();

	private:
		virtual void _serialize(Archive &ar, int version);
		MaterialParam *findOrNewParam(const FixedString &name);
		void sortParams();//保持params的顺序一致， 避免同样的参数生成不一样的MaterialMacro
	};
}

#endif 