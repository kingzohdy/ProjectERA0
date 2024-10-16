
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
		int m_EffectObjCount; //在此node以及子node里面有多少个effectobjs, 优化getEffectObjects

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

		//得到影响到bound的灯光等EffectObjes
		void getEffectObjects(std::vector<EffectObject *>&objs, const BoxSphereBound &bound);

		static LooseOctree *getOwnerTree(MovableObject *pnode);
		
		std::map<MovableObject*, LooseOctreeNode*>	m_mapAllObjects;

	private:
		void CleanTree();
		bool CleanOctree_Internal( LooseOctreeNode *proot );
		LooseOctreeNode *NewOneNode( size_t level, const Vector3 &center, float radius, LooseOctreeNode *pfather );
		void DelOneNode( LooseOctreeNode *pnode );
		//得到能放sphere或者box的octree node， 如果box， bound_radius是最长的边长
		LooseOctreeNode *GetContainer( const Vector3 &bound_center, float bound_radius, LooseOctreeNode *poctreenode );

		void AddSceneNodeToBuffer( CullResult &result, LooseOctreeNode *pnode);
		void AddSceneNodeToBuffer_Recursive( CullResult &result, LooseOctreeNode *pnode);
		void CullOctreeNodeVisual( CullResult &result, LooseOctreeNode *pnode);

		bool pickObject(IntersectType type, LooseOctreeNode *poctreenode, const Ray &ray, MovableObject *&pscenenode, float &dist, uint pickflags);
		void getEffectObjects(std::vector<EffectObject *>&objs, const BoxSphereBound &bound, LooseOctreeNode *proot);

		LooseOctreeNode *m_pRoot;
		size_t      m_MaxLevel;
		Vector3     m_WorldCenter;
		float       m_WorldSize;//整个世界的大小, 位置, 一个正方体
		size_t      m_CurNodeNum;

		GameScene *m_pScene;
	};

}

#endif