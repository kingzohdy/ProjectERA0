
#ifndef __PhysicsScene_H__
#define __PhysicsScene_H__

#include "OgreBounding.h"
#include "ozcollide/aabbtree_poly.h"

namespace Ogre
{
	class _OgreExport PhysicsScene
	{
	public:
		PhysicsScene();
		~PhysicsScene();

		void *addBSPData(BSPData *pdata, const Matrix4 &tm);//return handle
		void removeBSPData(void *handle);
		bool pick(const Ray &ray, float *collide_dist, Vector3 *pnormal=NULL);
		void buildDecalMesh(const BoxSphereBound &bound, Vector3 *pVB, unsigned short* pIB, int vertexbase, int nFaceLimit, int& nVertexCount, int& nFaceCount);
		
	private:
		struct CollideData
		{
			BoxSphereBound bound;
			std::vector<Vector3>faces;
		};
		std::vector<CollideData *>m_CollideData;
	};

	class _OgreExport PhysicsScene2
	{
	public:
		PhysicsScene2();
		~PhysicsScene2();

		void reset ();

		void *addStaticBSPData(BSPData *pdata, const Matrix4 &tm);
		void calAABBTree();

		void saveData( const FixedString & path );
		int  loadData( const FixedString & path );
		bool pick(const Ray &ray, float *collide_dist, Vector3 *pnormal=NULL);
		void buildDecalMesh(const BoxSphereBound &bound, Vector3 *pVB, unsigned short* pIB, int vertexbase, int nFaceLimit, int& nVertexCount, int& nFaceCount);

	private:
		ozcollide::AABBTreePoly *m_pTree;
		std::vector<ozcollide::Vec3f>m_Verts;
		std::vector<ozcollide::Polygon>m_Polys;
	};
}

#endif