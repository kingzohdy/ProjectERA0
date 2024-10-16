
#ifndef __Model_H__
#define __Model_H__

#include "OgrePrerequisites.h"
#include "OgreResource.h"
#include "OgreRenderableObject.h"
#include "OgreColourValue.h"
#include "OgreVector2.h"
#include "OgreTexture.h"
#include "OgreMultiLoader.h"

namespace Ogre
{
	enum
	{
		INSTANCE_MATERIAL = 1,
		INSTANCE_VB = 2,
		INSTANCE_IB = 4
	};

	struct ModelInstanceData
	{
		float getVertHeight(size_t i)
		{
			return *(float *)((char *)pvertheight + i*vertstride);
		}

		uint getVertColor(size_t i)
		{
			return *(uint *)((char *)pvertcolor + i*vertstride);
		}

		bool tm_used;
		bool abs_vertheight;

		int bright;

		Vector3 pos;
		Quaternion rot;
		Vector3 scale;

		float *pvertheight;
		uint *pvertcolor;
		uint vertstride;

		ModelData *pmodeldata;
	};

	class _OgreExport SubMeshInstance
	{
	public:
		SubMeshInstance(SubMeshData *psubmesh);
		~SubMeshInstance();

		void prepareContext(ShaderContext *pcontext,Material* prendermtl);
		void makeInstance(uint flags);

		void setCanUseStaticLight (bool canUse)
		{
			m_bCanUseStaticLight = canUse;
		}

		bool canUseStaticLight ()
		{
			return m_bCanUseStaticLight;
		}

		void SwitchToStaticLight (bool staticLight);
		bool hasStaticLight ()
		{
			return m_bUseStaticLight;
		}

		void enableUVMask (bool enable, bool stdmtluse)
		{
			m_bEnableUVMask = enable;
			m_bStdEnableUVMask = stdmtluse;
		}

		void setUVMaskTrans (Vector2 uvTrans)
		{
			mTrans = uvTrans;
		}

		void setUVMaskColor (Vector3 color)
		{
			mUVMaskColor = color;
		}

		void setLiuGuangTexture (TextureData *texture)
		{
			if (m_pLiuGuangTexture)
				m_pLiuGuangTexture->release();

			if (texture)
				texture->addRef();
			m_pLiuGuangTexture = texture;
		}

		IndexData *getIndexData();
		VertexData *getVertexData();
		Material *getMaterial();

		bool intersectRay(IntersectType type, const Ray &ray, float *collide_dist);

	public:
		SubMeshData *m_pSubMesh;
		Material *m_pInstMtl;
		IndexData *m_pInstID;
		VertexData *m_pInstVD;
		uint m_NumPrimitive;

		std::vector<unsigned int> m_Colors; // 烘焙的静态光源
		VertexData *m_pInstVDBake;

		float m_ColorType;
		bool m_bEnableDeadEffect;
		float m_fDeadScale;
		Ogre::TextureData *m_pDeadTexture;

	private:
		void makeBakeInstance ();
		bool m_bUseStaticLight;
		bool m_bCanUseStaticLight;

		TextureData *m_pLiuGuangTexture;
		Vector2 mTrans;
		Vector3 mUVMaskColor;
		bool m_bEnableUVMask;
		bool m_bStdEnableUVMask;
	};

	class _OgreExport MeshInstance
	{
	public:
		MeshInstance(MeshData *pmeshdata);
		MeshInstance(std::vector<ModelInstanceData>&instance_data, size_t imesh);
		~MeshInstance();

		void show(bool b)
		{
			m_bShow = b;  
		}

		bool isShow()
		{
			return m_bShow;
		}

		void setColorType (int colorType);

		void enableDeadEffect (bool enable)
		{
			for (size_t i=0; i<m_SubMeshes.size(); i++)
			{
				m_SubMeshes[i]->m_bEnableDeadEffect = enable;
			}
		}

		void setDeadScale (float deadScale)
		{
			for (size_t i=0; i<m_SubMeshes.size(); i++)
			{
				m_SubMeshes[i]->m_fDeadScale = deadScale;
			}
		}

		void enableUVMask (bool enable, bool stdmtlUse)
		{
			for (size_t i=0; i<m_SubMeshes.size(); i++)
			{
				m_SubMeshes[i]->enableUVMask(enable, stdmtlUse);
			}
		}

		void setUVMaskTrans (Vector2 uvTrans)
		{
			for (size_t i=0; i<m_SubMeshes.size(); i++)
			{
				m_SubMeshes[i]->setUVMaskTrans(uvTrans);
			}
		}

		void setUVMaskColor (Vector3 color)
		{
			for (size_t i=0; i<m_SubMeshes.size(); i++)
			{
				m_SubMeshes[i]->setUVMaskColor(color);
			}
		}

		void setLiuGuangTexture (Ogre::TextureData *texture)
		{
			for (size_t i=0; i<m_SubMeshes.size(); i++)
			{
				m_SubMeshes[i]->setLiuGuangTexture(texture);
			}
		}

		size_t getNumSubMesh()
		{
			return m_SubMeshes.size();
		}

		SubMeshInstance *getSubMesh(size_t i)
		{
			if (i >= m_SubMeshes.size() || i<0)
				return NULL;

			return m_SubMeshes[i];
		}

		SubMeshInstance *findSubMeshByMaterial(const FixedString &name);

		size_t getNumVertex();

		const FixedString &getName();

		void prepareContext(ShaderContext *pcontext,Material* prendermtl);

		void mergeSubMeshVertex(SubMeshInstance *psubmesh, std::vector<ModelInstanceData>&instance_data, size_t imesh, size_t isubmesh, std::vector<size_t>&num_meshvert, size_t nvertex, std::vector<size_t>&offset );
		void mergeSubMeshIndex(SubMeshInstance *psubmesh, std::vector<ModelInstanceData>&instance_data, size_t imesh, size_t isubmesh, std::vector<size_t>&num_meshvert, size_t nvertex);
		void makeInstance(uint flags);

		void getLocalBounds(BoxBound &box);

		void setTexture(const FixedString &texture_name, Texture *ptexture);

	public:
		MeshData *m_pMeshData;
		bool m_bShow;

		std::vector<SubMeshInstance *>m_SubMeshes;
	};

	struct ModelAnchor;
	class ModelAnchorNode
	{
	public:
		ModelAnchorNode(ModelAnchor *panchor);

		ModelAnchor *m_pAnchor;
	};

	// 播放混合动画的参数
	struct AnimPlayInfo
	{
		int		nNumSequences;
		int		nSequence[4];
		int		nPriority[4];
	};

	class Model;
	class _OgreExport TextureDataLoader : public MultiLoader
	{
	private:
		Model *m_pModel;

	public:
		TextureDataLoader() : m_pModel(NULL) {}
		virtual ~TextureDataLoader() {}

		void setModel(Model *model);

	protected:
		virtual TextureData * getTextureData(size_t, TextureData **) = 0;

	private:
		virtual void	onComplete(size_t size, Resource **recs, ResourceHandle *hs);
	};

	class _OgreExport Model : public GeomNode, public LoadWrap
	{
		DECLARE_RTTI(Model)

	public:
		Model();
		Model(ModelData *pmodeldata);
		Model(std::vector<ModelInstanceData>&instance_data);

		void enableDeadEffect (bool enable)
		{
			for (int i=0; i<(int)m_Meshes.size(); i++)
			{
				m_Meshes[i]->enableDeadEffect(enable);
			}
		}

		void setDeadScale (float deadScale)
		{
			for (int i=0; i<(int)m_Meshes.size(); i++)
			{
				m_Meshes[i]->setDeadScale(deadScale);
			}
		}

		virtual void enableUVMask (bool enable, bool stdmtluse)
		{
			for (int i=0; i<(int)m_Meshes.size(); i++)
			{
				m_Meshes[i]->enableUVMask(enable, stdmtluse);
			}
		}

		void setUVMaskSpeed(Vector2 uvSpeed)
		{
			m_UVSpeed = uvSpeed;
		}

		void setUVMaskColor (Vector3 color)
		{
			for (int i=0; i<(int)m_Meshes.size(); i++)
			{
				m_Meshes[i]->setUVMaskColor(color);
			}
		}

		virtual void setLiuGuangTexture (TextureData *texture)
		{
			for (int i=0; i<(int)m_Meshes.size(); i++)
			{
				m_Meshes[i]->setLiuGuangTexture(texture);
			}
		}

		// 模型能否使用静态光照，没有被烘焙的不使用
		virtual void setCanUseStaticLight (bool canUseStaticLight);
		bool canUseStaticLight ()
		{
			return m_bCanUseStaticLight;
		}

		virtual void update(uint dtick);
		virtual void resetUpdate(bool pause, uint tick=0xffffffff);
		virtual void render(SceneRenderer* prenderer, const ShaderEnvData &envdata);
		virtual bool intersectRay(IntersectType type, const Ray &ray, float *collide_dist);
		virtual void updateWorldCache();

		MeshInstance *findMesh(const FixedString &name);
		void applyAnimation(AnimPlayTrack *ptracks[], size_t count);

		ModelAnchor *getBindPoint(uint id);

		SkeletonInstance *getSkeletonInstance()
		{
			return m_pSkeleton;
		}

		void getLocalBounds(BoxSphereBound &bound);

		size_t getNumSkin();
		MeshInstance *getIthSkin(size_t i);
		void showSkins(bool b);
		void showSkin(const FixedString &name, bool b);
		void setTextureByID(int id, Texture *ptex);
		void setTextureByID(int id, const char *path);
		void setTextureByID(int id, TextureDataLoader *loader);
		void setTextureData(Texture *ptex, TextureDataLoader *loader);
		void clearTextureByID(int id);
		void setTexture(const FixedString &texture_name, Texture *ptexture);
		void resetTexture(const FixedString &respath); //for old use

		uint getSeqDuration(int seq);
		//如果没有这个seq， 返回false
		bool playAnim(int seq, float weight=1.0f, float speed=1.0f);
		void stopAnim();
		void stopAnim(int seq);
		bool hasAnim(int seq);
		bool hasAnimPlaying(int seq);
		void AddAnimation(Ogre::FixedString& filename);
		void AddAnimation(Resource *pres);

		void rebuildBounding();

		ModelData *getModelData()
		{
			return m_pModelData;
		}

		AnimationPlayer *getAnimPlayer()
		{
			return m_pAnimPlayer;
		}

		virtual Matrix4 getAnchorWorldMatrix(int id);

		bool hasAnchor(int id);
		size_t getAnchors(std::vector<int>&anchors);

		void setInstanceAmbient(const ColourValue &color)
		{
			m_InstanceAmbient = color;
		}

		void render(SceneRenderer* prenderer, const ShaderEnvData &envdata, Material *pmaterial);

		bool checkBonesSize();

	protected:
		ModelData *m_pModelData;
		SkeletonInstance *m_pSkeleton;
		std::vector<MeshInstance *>m_Meshes;
		std::vector<ModelAnchorNode *>m_Anchors;
		std::map<ResourceHandle, int>		m_TexLoadID;
		std::map<int, ResourceHandle>		m_IDTexLoad;
		std::map<TextureDataLoader *, int>	m_TexsLoadID;
		std::map<int, TextureDataLoader *>	m_IDTexsLoad;

		AnimationPlayer *m_pAnimPlayer;
		BoxSphereBound m_LocalBound;

		ColourValue m_InstanceAmbient;
		int m_SubmeshCounter; //submesh渲染的累积计数，在z比较时做一个固定的顺序。

		bool m_bCanUseStaticLight;

		Ogre::Vector2 m_UVSpeed;
		Ogre::Vector2 m_UVTrans;

	protected:
		virtual ~Model();
		void renderMesh(SceneRenderer* prenderer, const ShaderEnvData &envdata, MeshInstance *pmesh, Material *pmtl);
		void renderStaticMesh(SceneRenderer* prenderer, const ShaderEnvData &envdata, MeshInstance *pmesh, Material *pmtl);
		void renderSkinMesh(SceneRenderer* prenderer, const ShaderEnvData &envdata, MeshInstance *pmesh, Material *pmtl);
		void prepareContextForMesh(ShaderContext *pcontext, const ShaderEnvData &envdata, MeshInstance *pmesh, SubMeshInstance *psubmesh,Material* prendermtl);
		void createInstanceData(std::vector<ModelInstanceData>&instance_data);

		void ResourceLoaded(Resource *rec, ResourceHandle h);
	};

	_OgreExport Model *loadModelFromFile(const FixedString &path);
}

#endif
