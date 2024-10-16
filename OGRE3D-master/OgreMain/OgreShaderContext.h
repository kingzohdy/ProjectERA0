
#ifndef __ShaderContext_H__
#define __ShaderContext_H__

#include "OgreColourValue.h"
#include "OgreShader.h"

namespace Ogre
{
	class _OgreExport ShaderContext
	{
		friend class ShaderContextPool;

	public:
		ShaderContext(ShaderContextPool *pool);
		void reset(ShaderContextPool *pool);

		void applyShaderParam(int framecount);

		void addValueParam(ShaderParamUsage type, const void *pdata, size_t datalen);
		void addRawValueParam(ShaderParamUsage type, const void *pdata, size_t datalen);
		void addTextureParam(ShaderParamUsage type, Texture *ptex);

		void setInstanceEnvData(SceneRenderer *prenderer, RenderableObject *pobject, const ShaderEnvData &envdata, const Matrix4 *pworldtm=NULL);

		void setLayer(RenderLayer layer)
		{
			m_SortIndex = (layer<<16) | (m_SortIndex&0xff00ffff);
		}

		void setTransparent(bool b)
		{
			m_SortIndex = (m_SortIndex&0xffff0000) | (b?1:0);
		}

		void setSkyBox(bool b)
		{
			m_SortIndex = (m_SortIndex&0x7fffffff) | (b ? 0 : 0x80000000);
		}

		void setIB(IndexBuffer *pib);
		void setVB(VertexBuffer *pvb);
		void setMaterial(Material *pmtl);

		bool isTransparent() const{ return m_SortIndex & 1; }

		void draw(ShaderTechnique *pshader);
		void prepareDraw(RenderUsage usage);

		ShaderContext *setDebugInfo(const char *pfilename, long line, RenderableObject *pobject);

	private:
		VertexBuffer *m_pVB;
		IndexBuffer *m_pIB;
		MaterialTemplate *m_pMtlTemplate;
		ShaderTechnique *m_pHardwareShader;

	public:
		ShaderContextPool*	m_pContexPool;

		float					m_EyeDistance;  //和相机的距离

		unsigned int            m_SortIndex;//(layer<<16) | xparent


		VertexDeclHandle m_VertexDecl;

		PrimitiveType m_PrimitiveType;
		size_t m_StartVertexIndex;
		size_t m_NumPrimitive;


		size_t m_RefVertStart;
		size_t m_RefVertNum;

		ShaderEnvFlags	m_ShaderEnvFlags;
		MaterialMacro m_MaterialMacro;

		int m_ShaderLod;

		ColourValue m_InstanceAmbient; //实例本身的环境光

	private:
		size_t					m_nNumValueParams;
		size_t				    m_nValueStart;

		size_t				    m_nTextureStart;
		size_t					m_nNumTextureParams;


#ifdef _DEBUG
		char m_submitFile[256];
		long m_submitLine;
		RenderableObject *m_pRenderable;
#endif

	private:
		void handleShaderParam(ShaderParamUsage usage, const ShaderEnvData &envdata, const Matrix4 *pworldtm);
	};

	class _OgreExport ShaderContextPool
	{
	public:
		struct ValueParam
		{
			ShaderParamUsage	usage;
			size_t				offset;
			size_t				len;
			bool				is_raw;
		};
		struct TexParam
		{
			ShaderParamUsage usage;
			Texture *ptex;
		};

	public:

		ShaderContextPool(size_t pool_size=0);
		~ShaderContextPool();

		static bool LessThan(const ShaderContext *pcontext1, const ShaderContext *pcontext2);
		static bool DisLessThan(const ShaderContext *pcontext1, const ShaderContext *pcontext2);

		void reset();
		ShaderContext *newContext(RenderLayer layer);
		size_t getNumContext()
		{ 
			return m_nUsedContext; 
		}

		ShaderContext *getContext(size_t i){ return m_Contexts[i]; }

		size_t addValueParam(ShaderParamUsage usage, const void *pdata, size_t datalen, bool is_raw);
		size_t addTextureParam(ShaderParamUsage usage, Texture *ptex);

		void applyShaderValueParam(size_t i, ShaderTechnique* pShader);
		void applyTextureParam(size_t i, ShaderTechnique *pshader);

		void sortByRenderTarget();
		void draw();
		int present();

		// 按线框方式绘制所有上下文
		void drawWireframe(const ColourValue& color);

		void drawRange(ShaderTechnique *pshader, size_t begin, size_t end,int framecount);

		void drawQueue(const ContextQueDesc &desc );
		void drawQueue( int nIndex );

		void startQueue(const ContextQueDesc &desc);
		void endQueue();
		int  getQueueSize();

	public:
		void SetContextQueDesc(ContextQueDesc	contextquedesc)
		{
			m_ContextQueDesc = contextquedesc;
		}
		float								m_Time;
	private:

		//实际绘制用的渲染队列
		bool								bShadowContex[2048];
		// 这里面存储着从开始游戏的时候就被创建过的所有shader,这个集合里面的数据在游戏生命周期都不会有数据被删除
		std::vector<ShaderContext *>		m_Contexts;			
		size_t								m_nUsedContext;
		std::vector<ContextQueDesc>         m_ContextQue;		// 里面存储着当前要渲染的所有shader标识
		ContextQueDesc				        m_ContextQueDesc;

		std::vector<unsigned char>			m_ValuePool;
		size_t								m_nUsedValuePool;

		std::vector<ValueParam>				m_ValueParamPool;
		size_t								m_nUsedValueParamPool;

		std::vector<TexParam>				m_TextureParamPool;
		size_t								m_nUsedTextureParamPool;

	};

#ifdef _DEBUG
#define DEBUG_SHADERCONTEXT(pcontext) pcontext->setDebugInfo(__FILE__, __LINE__, this)
#define DEBUG_SHADERCONTEXT_PTR(pcontext, p) pcontext->setDebugInfo(__FILE__, __LINE__, p)
#else
#define DEBUG_SHADERCONTEXT(pcontext)
#define DEBUG_SHADERCONTEXT_PTR(pcontext, p)
#endif
}

#endif