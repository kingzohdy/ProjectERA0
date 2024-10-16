
#ifndef __Camera_H__
#define __Camera_H__

#include "OgreMovableObject.h"
#include "OgrePlane.h"
#include "OgreRenderTypes.h"

namespace Ogre
{
	class CullResult;
	class _OgreExport Camera : public MovableObject
	{
		DECLARE_RTTI(Camera)
	public:
		Camera();

		void copyMembers(Camera *pobj);
		void mirror( Camera *pobj, const WorldPlane &plane);

		virtual void update(uint dtick);

		void setViewport(float x, float y, float width, float height, float minz=0.0f, float maxz=1.0f );

		void setFov( float f ){ m_Fov = f; }
		void setOrtho( float s ){ m_Fov = 0, m_OrthoSize = s; }
		void setRatio( float r );
		void setDepth( float n, float f ){ m_ZNear=n, m_ZFar=f; }
		void setLookDirect( const WorldPos &eye, const Vector3 &direct, const Vector3 &up );
		void setLookAt( const WorldPos &eye, const WorldPos &goal, const Vector3 &up );
		void setViewTransform( const WorldPos &pos, const Quaternion &rot );
		void moveCamera( const Vector3 &step );
		void rotateCameraLR( float step ); //左右转动相机, 左--正， 右---负
		void rotateCameraUD( float step ); //上下转动相机, 上--正， 下---负

		const WorldPos &getEyeLoc(){ return m_Position; }
		const Vector3 &getEyeDir(){ return m_ViewDir; }

		void setWorldMorph( const Matrix4 &tm ); //作用在以EyeLoc为原点和世界平行的坐标系中, P1 = (P-EyeLoc) * morph * viewtm * projtm

		const Matrix4 &getViewMatrix(); //return pret
		const Matrix4 &getProjectMatrix();
		const Matrix4 getProjectMatrix(float fNearZ, float fFarZ);

		float getZNear() const{return m_ZNear;}
		float getZFar() const{return m_ZFar;}
		float getFov() const{ return m_Fov; }
		float getRatio() const{return m_Ratio;}
		float getViewWidth() const{ return m_Viewport.width; }
		float getViewHeight() const{ return m_Viewport.height; }

		const Viewport &getViewport() const{return m_Viewport;}

		//返回点是否在视口范围内
		void  pointWorldToView( Vector3 &dest, const WorldPos &src );
		void  pointViewToWorld( WorldPos &dest, const Vector3 &src );
		bool  pointWorldToViewport( Vector3 &dest, const WorldPos &src );
		bool  pointWorldToViewport( float &x, float &y, const WorldPos &src );
		void  pointViewportToWorld( WorldPos &dest, float x, float y );
		bool  pointWorldToWindow( float &x, float &y, const WorldPos &src, float *pz=0 );
		void  pointWindowToWorld( WorldPos &dest, float x, float y );
		bool  pointWorldToWindow( Vector3& vScr, const Vector3 &vPos );

		void  getViewRayByScreenPt( WorldRay *pray, float x, float y );

		bool  canSeePointInWorld( const WorldPos &point );

		//plane is in world space,  if pplane==NULL, disable this clipping plane
		//if i==-1,  disable all plane
		//useoblique: 即使有userclipplane, 也使用oblique matrix(只对于 clipplane0)
		void setUserClipPlane( size_t i, const WorldPlane *pplane, bool useoblique=false );

		void getCullFrustum(CullFrustum &frustum);

		CullResult*	m_pCullresult;

	protected:
		Viewport m_Viewport;

		float m_Fov; // the fov in the direction of height
		float m_OrthoSize; //if fov == 0, OrthoSize is the view width
		float m_Ratio; //tg(fovx)/tg(fovy)
		float m_ZNear;
		float m_ZFar;

		Matrix4   m_Projection;
		Matrix4   m_WorldMorph; //在场景进入相机之前, 进行一次morph
		Matrix4   m_View;
		Vector3   m_ViewDir;

		WorldPlane	m_UserClipPlane[6];
		uint		m_UserClipFlags;  //each bit represents one clipping plane
		bool		m_bUseOblique;
		

	protected:
		virtual ~Camera();
		void computeViewport();
		void getViewSizeOnNearPlane( float &hw, float &hh );
	};
}

#endif