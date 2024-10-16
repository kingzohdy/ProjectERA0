
#ifndef __SceneRenderer_H__
#define __SceneRenderer_H__

#include "OgreCullFrustum.h"
#include "OgreRenderTypes.h"
#include <vector>

_OgreExport Ogre::WorldPos &getGlobalPos();

namespace Ogre
{
	class BorderGameScene;
	class BackGameScene;
	class DeathGameScene;

	class _OgreExport SceneRenderer
	{
	public:
		SceneRenderer();
		virtual ~SceneRenderer();

		void setShaderLevel(int nShaderLevel){m_nShaderLevel = nShaderLevel;}
		void setSceneDebugger(SceneDebugger* pSceneDebugger){ m_pSceneDebugger = pSceneDebugger; }
		ShaderContext *newContext(RenderLayer layer);
		ShaderContext *newContext(RenderLayer layer, const ShaderEnvData &envdata, Material *pmtl, VertexDeclHandle vertdecl, VertexBuffer *pvb, IndexBuffer *pib, PrimitiveType primtype, size_t numprim, bool transparent);

		DynamicIndexBuffer *newDynamicIB(size_t nindex);
		DynamicVertexBuffer *newDynamicVB(const VertexFormat &vertfmt, size_t nvertex);

		void setCamera(Camera *pcamera)
		{
			m_pCamera = pcamera;
		}

		Camera *getCamera()
		{
			return m_pCamera;
		}

		void setRenderTarget(RenderTarget *ptarget)
		{
			m_pTarget = ptarget;
		}

		void setRenderScene(GameScene *pscene)
		{
			m_pScene = pscene;
		}

		void setClearParams(unsigned int flags, uint color, float depth, uint stencil)
		{
			m_ClearFlags = flags;
			m_ClearColor = color;
			m_ClearDepth = depth;
			m_ClearStencil = stencil;
		}

		void setRenderPool(ShaderContextPool *pcontextpool, DynamicBufferPool *pbufferpool)
		{
			m_pContextPool = pcontextpool;
			m_pDynBufferPool = pbufferpool;
		}

		virtual void onLostDevice()
		{
		}

		virtual void onRestoreDevice()
		{
		}

		virtual void doRender() = 0; //compute context pool

		GameScene	*getGameScene()
		{
			return m_pScene;
		}

	public:
		int		m_CullFlag;
		size_t  m_NumRenderFrame;
		RenderUsage m_CurRenderUsage;

	protected:
		void RenderResult(ShaderEnvData& env, const CullResult* result,RenderTarget* pTarget,
			size_t clearflag,size_t clearcolor,float cleardepth,size_t stencil,
			size_t clipflags,Plane clipplane[],RenderUsage renderusage,uint usertypebits=0xffffffff, int type = -1);

		void AddActorToBackScene (const CullResult* result);

	protected:
		int	   m_nShaderLevel;
		size_t		m_nRenderQuality;
		CullFrustum m_Frustum;
		
		ShaderContextPool *m_pContextPool;
		DynamicBufferPool *m_pDynBufferPool;

		SceneDebugger* m_pSceneDebugger;
		Camera* m_pCamera;   //外部传入的相机
		Camera *m_pCameraForShake; //用于震动的相机
		Camera*	m_pRenderCamera; //用于渲染的相机
		CullResult *m_pBackCullResult;
		CullResult *m_pDeadCullResult;
		Camera*	m_pDepthCamera;
		Camera*	m_pReflectCamera;
		RenderTarget *m_pTarget;
		GameScene *m_pScene;

		uint m_ClearFlags;
		uint m_ClearColor;
		float m_ClearDepth;
		uint m_ClearStencil;

		static bool ms_bBorderBackSceneAlreadyExist;
		bool m_warfogdraw;
	};

	class _OgreExport NormalSceneRenderer : public SceneRenderer
	{
		enum{MAX_SHAKECHANNELS = 16};
	public:
		NormalSceneRenderer();
		~NormalSceneRenderer();

		void clearClipPlane();
		void setClipPlane(uint i, float *plane);

		void shakeCamera(const Vector3 &receive_center);

		virtual void onLostDevice();
		virtual void onRestoreDevice();
		virtual void doRender();

		int allocCameraShakeChannel(float maxdist);
		void freeCameraShakeChannel(int channel);
		void setCameraShake(int channel, float strength, const Vector3 &pos);

	protected:
		uint m_ClipFlags;
		float m_ClipPlane[6][4];

		struct CameraShakeSource
		{
			Vector3 center;
			float strength;
			float maxdist;
		};
		CameraShakeSource m_CameraShakeChannels[MAX_SHAKECHANNELS];

		BorderGameScene *m_pBorderScene;
		BackGameScene *m_pBackGameScene;
		DeathGameScene *m_pDeathGameScene;
	};
}

#endif