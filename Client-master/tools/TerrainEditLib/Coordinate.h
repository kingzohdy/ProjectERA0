
#ifndef __COORDINATE_H__
#define __COORDINATE_H__

#include "TerrainEditTypes.h"

#include "OgreLines.h"

namespace TE
{
	enum COORD_MODE
	{
		COORD_MOVE = 0,
		COORD_ROTATE,
		COORD_SCALE,
		COORD_SELECT
	};
	class Coordinate
	{
	public:
		Coordinate();
		~Coordinate();

		void bindObject(EditorModel *model);
		void update(float dtime);
		void render();
		COORDINATE_SELECT intersect(const Ogre::Vector3 &dir, const Ogre::Vector3 &pos, int mode);//model=0:xyz axis,   1:rotate circle

		void setCoordMode(COORD_MODE mode){m_CurCoordMode = mode;}
		COORD_MODE getCoordMode(){return m_CurCoordMode;}
		COORDINATE_SELECT getCoordSelect(){return m_CurCoordSelect;}

		Ogre::Vector3 intersectXY(Ogre::Vector3& pos , Ogre::Vector3&dir);
		Ogre::Vector3 intersectXZ(Ogre::Vector3& pos , Ogre::Vector3&dir);
		Ogre::Vector3 intersectYZ(Ogre::Vector3& pos , Ogre::Vector3&dir);

		Ogre::Vector2 getAxisProjectVector(int i);

	private:
		void renderMoveCoord();
		void renderRotateCoord();
		void renderScaleCoord();

		void renderOneAxis(const Ogre::Vector3 &from, const Ogre::Vector3 &to, Ogre::ColorQuad color, bool selected);
		void renderOneCircle(const Ogre::Vector3 &origin, const Ogre::Vector3 &dir, Ogre::ColorQuad color, bool selected);

	private:
		COORD_MODE m_CurCoordMode;
		COORDINATE_SELECT m_CurCoordSelect;
		EditorModel *m_pCurBindObject;
		//对应的 x ,y , z 轴的包围盒
		Ogre::BoxBound m_xAxesBox;
		Ogre::BoxBound m_yAxesBox;
		Ogre::BoxBound m_zAxesBox;
		//对应 xy , yz , xz 面的包围盒
		Ogre::BoxBound m_xyAxesBox;
		Ogre::BoxBound m_yzAxesBox;
		Ogre::BoxBound m_xzAxesBox;
		float m_fScaleObject;
		Ogre::Vector3 m_Origin;

	public:
		//当前位置
		Ogre::Vector3 m_CurPos;
	};
}

#endif