
#ifndef __OgreLooseOctree_H__
#define __OgreLooseOctree_H__

#include "OgreVector3.h"
#include <map>

namespace Ogre
{
	class LooseOctree;

	class LooseOctreeNode
	{
	public:
		~LooseOctreeNode();
		void addMovableObject(MovableObject *pobj);
		void removeMovableObject(MovableObject *pobj);

	public:
		Vector3 m_Center;
		float   m_Radius; //the radius of grid( from center to the face )
		float   m_Zone;  //the total size of this grid node  = m_FixSize + m_Zone
		size_t  m_Level;
		int m_EffectObjCount; //�ڴ�node�Լ���node�����ж��ٸ�effectobjs, �Ż�getEffectObjects

		enum{ NUM_CHILD = 8 };
		LooseOctreeNode *m_pChild[NUM_CHILD];
		LooseOctreeNode *m_pFather;
		LooseOctree *m_pTree;

		std::vector<RenderableObject *>m_RenderObjects;
		std::vector<EffectObject *>m_EffectObjs;
	};

	class _OgreExport LooseOctree
	{
	public:
		LooseOctree(GameScene *pscene, size_t maxlevel, const Vector3 &center, float worldsize );
		~LooseOctree();

		LooseOctreeNode *attachObject( MovableObject *pnode );
		void detachObject( MovableObject *pnode );
		void updateObject( MovableObject *pnode );
		void cull(CullResult &result);
		MovableObject *pickObject(IntersectType type, const WorldRay &ray, float &dist, uint pickflags);

		//�õ�Ӱ�쵽bound�ĵƹ��EffectObjes
		void getEffectObjects(std::vector<EffectObject *>&objs, const BoxSphereBound &bound);

		static LooseOctree *getOwnerTree(MovableObject *pnode);
		
		std::map<MovableObject*, LooseOctreeNode*>	m_mapAllObjects;

	private:
		void CleanTree();
		bool CleanOctree_Internal( LooseOctreeNode *proot );
		LooseOctreeNode *NewOneNode( size_t level, const Vector3 &center, float radius, LooseOctreeNode *pfather );
		void DelOneNode( LooseOctreeNode *pnode );
		//�õ��ܷ�sphere����box��octree node�� ���box�� bound_radius����ı߳�
		LooseOctreeNode *GetContainer( const Vector3 &bound_center, float bound_radius, LooseOctreeNode *poctreenode );

		void AddSceneNodeToBuffer( CullResult &result, LooseOctreeNode *pnode);
		void AddSceneNodeToBuffer_Recursive( CullResult &result, LooseOctreeNode *pnode);
		void CullOctreeNodeVisual( CullResult &result, LooseOctreeNode *pnode);

		bool pickObject(IntersectType type, LooseOctreeNode *poctreenode, const Ray &ray, MovableObject *&pscenenode, float &dist, uint pickflags);
		void getEffectObjects(std::vector<EffectObject *>&objs, const BoxSphereBound &bound, LooseOctreeNode *proot);

		LooseOctreeNode *m_pRoot;
		size_t      m_MaxLevel;
		Vector3     m_WorldCenter;
		float       m_WorldSize;//��������Ĵ�С, λ��, һ��������
		size_t      m_CurNodeNum;

		GameScene *m_pScene;
	};

}

#endif