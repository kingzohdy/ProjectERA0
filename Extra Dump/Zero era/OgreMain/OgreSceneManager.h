
#ifndef __SceneManager_H__
#define __SceneManager_H__

#include "OgreSingleton.h"
#include "OgreVector3.h"

namespace Ogre
{

#define MAX_CONTEXT_NUM		4096		//���context��Ŀ
#define MAX_DYNAMICBUFFER_SIZE	512*512
#define MAX_POOL_NUM			8

	class Shadowmap;
	class ReflectEffect;
	class BloomEffect;
	class DistortEffect;
	class Shadowcubemap;
	struct ColorbitToClient//��D3D����Ⱦ������ݸ��ͻ���
	{
		HardwareTexture* m_pHardTexture;
		unsigned long*   m_pBits;
	};
	class RenderPool
	{
	public:
		RenderPool(size_t context_poolsize, size_t dynbuf_size);
		~RenderPool();
		void reset();

	public:
		int state;
		uint nframe;
		ShaderContextPool *pcontextpool;
		DynamicBufferPool *pbufferpool;
	};

	class _OgreExport SceneManager : public Singleton<SceneManager>
	{
	public:
		SceneManager();
		~SceneManager();

		typedef struct ModelTrans
		{
			bool interSect;//true��ʾ�����ཻ״̬
			bool isUP;//trueʱ͸����++
			uint changeTime;//��Ҫת��͸���ȵ�ʱ��
			float alpha;
		};
		std::map<RenderableObject *, ModelTrans *> m_TransModel; //��Ҫ�ı�͸���ȵ����map, 
		void removeFromTransModel(RenderableObject * obj);

		void setDraw (bool draw)
		{
			m_bDraw = draw;
		}

		bool isDraw ()
		{
			return m_bDraw;
		}
		
		void setScreenSize( int nWidth, int nHeight )
		{
			m_nScreenWidth	= nWidth;
			m_nScreenHeight	= nHeight;
		}

		void getScreenSize( int& nWidth, int& nHeight )
		{
			nWidth	= m_nScreenWidth;
			nHeight	= m_nScreenHeight;
		}

		void addSceneRenderer(int priority, SceneRenderer *prenderer);
		void removeSceneRenderer(SceneRenderer *prenderer);
		SceneRenderer *getSceneRenderer(int priority);

		void setActiveScene(GameScene *pscene)
		{
			m_pActiveScene = pscene;
		}
		GameScene *getActiveScene()
		{
			return m_pActiveScene;
		}

		void doFrame();

		uint getRenderFrame()
		{
			return m_NumRenderFrame;
		}

		uint getDrawFrame()
		{
			return m_NumDrawFrame;
		}

		void addToClientBit(HardwareTexture *pHartTex, unsigned long *pbits);
		void clearToClientBit();
		
		void setDisplayMode(int w, int h);
		void onLostEffect();

		int _drawThreadFunc();

		RenderPool *m_pRenderPool;	//д����Ⱦ����Ļ�����
		RenderPool *m_pDrawPool;	//�ύ��Ⱦ������d3d�Ļ�����

		Shadowmap *m_pShadowMap;
		ReflectEffect *m_pReflect;
		BloomEffect *m_pBloom;
		DistortEffect *m_pDistort;
		Shadowcubemap *m_pSCM;
		float m_referencepoint[16];//�߼���ɴ���Ĳο���,Ŀǰ�����ڴ�����߼����������������
		Vector3 m_warfogparameter;//����Ĳ���
		Vector3 m_warfogcolor;
		std::vector<ColorbitToClient> m_pToClientbit;
		bool m_bEffectNeed;//Ч���Ŀ���Ĭ�Ͽ�������Ҫ���ڸ����һ�

	private:
		void onLostDevice();
		void onRestoreDevice();
		void doRender();
		void doDraw();

	protected:
		GameScene *m_pActiveScene;

		uint m_NumRenderFrame;
		uint m_NumDrawFrame;
		bool m_bQuitRender;
		void *m_hRenderThread;
		void *m_DrawOverSemaphore;
		void *m_NewFrameSemaphore;

		SceneRenderer *m_pRenderers[16];

		enum
		{
			POOL_FREE = 0,
			POOL_DRAWING,
			POOL_RENDERING,
			POOL_RENDER_OVER,
			POOL_DRAW_OVER
		};

		bool m_bDeviceLost;

		bool	m_ChangeDispMode;
		int		m_DispModeWidth;
		int		m_DispModeHeight;

		int		m_nScreenWidth;
		int		m_nScreenHeight;

		bool m_bDraw;
	};
}

#endif