
#include "Coordinate.h"
#include "EditorModel.h"
#include "EditorCamera.h"
#include "TerrainEdit.h"
#include "OgreRay.h"
#include "OgreModel.h"
#include "OgreLines.h"
#include "OgreCamera.h"

#define   AXES_LENGHT   400.0f
#define   AXES_SIZE     20.0f
const float SPHERE_RADIUS = 200.0f;

using namespace Ogre;
namespace TE
{
	Coordinate::Coordinate() : m_CurCoordSelect(NO_AXES_SELECT), m_CurCoordMode(COORD_SELECT), m_pCurBindObject(NULL), m_fScaleObject(1.0f)
	{
	}

	Coordinate::~Coordinate()
	{
	}

	void Coordinate::bindObject(EditorModel *model)
	{
		m_pCurBindObject = model;
	}

	void Coordinate::update(float dtime)
	{
		if(m_pCurBindObject == NULL)
		{
			return;
		}

		Ogre::Vector3 minPos,maxPos;
		minPos = g_TerrainEdit.getSelectCenterPos();

		float axeslen = m_fScaleObject*AXES_LENGHT;
		m_xAxesBox.setRange(minPos, Ogre::Vector3(minPos.x+axeslen, minPos.y+AXES_SIZE, minPos.z+AXES_SIZE));
		m_yAxesBox.setRange(minPos, Ogre::Vector3(minPos.x+AXES_SIZE, minPos.y+axeslen, minPos.z+AXES_SIZE));
		m_zAxesBox.setRange(minPos, Ogre::Vector3(minPos.x+AXES_SIZE, minPos.y+AXES_SIZE, minPos.z+axeslen));

		axeslen *= 0.5f;
		m_xyAxesBox.setRange( minPos, Ogre::Vector3( minPos.x+axeslen, minPos.y+axeslen, minPos.z+AXES_SIZE));
		m_yzAxesBox.setRange( minPos, Ogre::Vector3( minPos.x+AXES_SIZE, minPos.y+axeslen, minPos.z+axeslen));
		m_xzAxesBox.setRange( minPos, Ogre::Vector3( minPos.x+axeslen, minPos.y+AXES_SIZE, minPos.z+axeslen));

		m_Origin = minPos;
	}

	void Coordinate::render()
	{
		if(m_pCurBindObject == NULL) return;

		if(m_CurCoordMode == COORD_MOVE) renderMoveCoord();
		else if(m_CurCoordMode == COORD_ROTATE) renderRotateCoord();
		else if(m_CurCoordMode == COORD_SCALE) renderScaleCoord();
	}

	void Coordinate::renderOneAxis(const Ogre::Vector3 &from, const Ogre::Vector3 &to, Ogre::ColorQuad color, bool selected)
	{
		Ogre::RenderLines *plines = g_TerrainEdit.m_pLinesRenderer;
		if(selected) plines->addLine(from, to, Ogre::ColorQuad(255,255,0));
		else plines->addLine(from, to, color);

		Ogre::Vector3 dir = to-from;
		Normalize(dir);
		Ogre::Vector3 peak = to + dir*20.0f;

		Ogre::Vector3 v1, v2;
		GetPerpendicular(v1, v2, dir);

		Ogre::Vector3 points[10];
		for(int i=0; i<=8; i++)
		{
			Ogre::Quaternion quat;
			quat.setAxisAngle(dir, i*360.0f/8);

			Ogre::Vector3 tmp;
			quat.rotate(tmp, v1);

			points[i+1] = tmp*10.0f + to;
		}
		points[0] = peak;
		plines->addTriangleFan(points, 10, color);
	}

	void Coordinate::renderMoveCoord()
	{
		float len = m_fScaleObject*AXES_LENGHT;

		bool selectx = (m_CurCoordSelect==SELECT_X_AXES || m_CurCoordSelect==SELECT_XY_AXES || m_CurCoordSelect==SELECT_XZ_AXES);
		bool selecty = (m_CurCoordSelect==SELECT_Y_AXES || m_CurCoordSelect==SELECT_XY_AXES || m_CurCoordSelect==SELECT_YZ_AXES);
		bool selectz = (m_CurCoordSelect==SELECT_Z_AXES || m_CurCoordSelect==SELECT_XZ_AXES || m_CurCoordSelect==SELECT_YZ_AXES);

		renderOneAxis(m_Origin, Ogre::Vector3(m_Origin.x+len, m_Origin.y, m_Origin.z), Ogre::ColorQuad(255,0,0), selectx);
		renderOneAxis(m_Origin, Ogre::Vector3(m_Origin.x, m_Origin.y+len, m_Origin.z), Ogre::ColorQuad(0,255,0), selecty);
		renderOneAxis(m_Origin, Ogre::Vector3(m_Origin.x, m_Origin.y, m_Origin.z+len), Ogre::ColorQuad(0,0,255), selectz);
	}

	void Coordinate::renderOneCircle(const Ogre::Vector3 &origin, const Ogre::Vector3 &dir, Ogre::ColorQuad color, bool selected)
	{
		Ogre::Vector3 v1, v2;
		GetPerpendicular(v1, v2, dir);
		
		Ogre::Vector3 points[32];
		for(int i=0; i<32; i++)
		{
			float angle = i*360.0f/32;
			points[i] = origin + (v1*Cos(angle) + v2*Sin(angle))*SPHERE_RADIUS;
		}

		if(selected) color = Ogre::ColorQuad(255,255,0);
		g_TerrainEdit.m_pLinesRenderer->addLineStrip(points, 32, color, true);
	}

	void Coordinate::renderRotateCoord()
	{
		renderOneCircle(m_Origin, Ogre::Vector3(1.0f, 0, 0), Ogre::ColorQuad(255,0,0), m_CurCoordSelect==SELECT_X_AXES);	
		renderOneCircle(m_Origin, Ogre::Vector3(0, 1.0f, 0), Ogre::ColorQuad(0,255,0), m_CurCoordSelect==SELECT_Y_AXES);	
		renderOneCircle(m_Origin, Ogre::Vector3(0, 0, 1.0f), Ogre::ColorQuad(0,0,255), m_CurCoordSelect==SELECT_Z_AXES);	
	}

	void Coordinate::renderScaleCoord()
	{
		renderMoveCoord();
	}

	//²âÊÔXYÃæ
	Ogre::Vector3  TestAndXYFace(const Ogre::Vector3 &pos, const Ogre::Vector3 &dir, float dz)
	{
		Ogre::Vector3  _pos(0,0,0);

		float fk = dz - pos.z  ;
		fk = fk / dir.z ;

		_pos.x = pos.x + fk * dir.x ;
		_pos.y = pos.y + fk * dir.y ;
		_pos.z = dz ;

		return _pos ;
	}
	//²âÊÔXZÃæ
	Ogre::Vector3  TestAndXZFace(const Ogre::Vector3 &pos, const Ogre::Vector3 &dir, float dy)
	{
		Ogre::Vector3  _pos(0,0,0);

		float fk = dy - pos.y ;
		fk = fk / dir.y ;

		_pos.x = pos.x + fk * dir.x ;
		_pos.z = pos.z + fk * dir.z ;
		_pos.y = dy ;

		return _pos ;
	}
	//²âÊÔYZÃæ;
	Ogre::Vector3  TestAndYZFace(const Ogre::Vector3 &pos, const Ogre::Vector3 &dir, float dx)
	{
		Ogre::Vector3  _pos(0,0,0);

		float fk = dx - pos.x  ;
		fk = fk / dir.x ;

		_pos.y = fk * dir.y ;
		_pos.z = fk * dir.z ;
		_pos.x = dx ;

		return _pos ;
	}

	Ogre::Vector3 Coordinate::intersectXY(Ogre::Vector3& pos , Ogre::Vector3&dir)
	{
		return TestAndXYFace(pos, dir, m_xyAxesBox.getMinPos().z);
	}

	Ogre::Vector3 Coordinate::intersectXZ(Ogre::Vector3& pos , Ogre::Vector3&dir)
	{
		return TestAndXZFace( pos, dir , m_xzAxesBox.getMinPos().y);
	}

	Ogre::Vector3 Coordinate::intersectYZ(Ogre::Vector3& pos , Ogre::Vector3&dir)
	{
		return TestAndYZFace( pos, dir , m_yzAxesBox.getMinPos().x);
	}

	Ogre::Vector2 Coordinate::getAxisProjectVector(int i)
	{
		Ogre::Vector2 pt1, pt2;
		g_pCamera->pointWorldToWindow(pt1.x, pt1.y, m_Origin);

		if(i==0) g_pCamera->pointWorldToWindow(pt2.x, pt2.y, m_Origin+Vector3(100.0f, 0, 0));
		else if(i==1) g_pCamera->pointWorldToWindow(pt2.x, pt2.y, m_Origin+Vector3(0, 100.0f, 0));
		else g_pCamera->pointWorldToWindow(pt2.x, pt2.y, m_Origin+Vector3(0, 0, 100.0f));

		pt2 -= pt1;
		return pt2;
	}

	COORDINATE_SELECT Coordinate::intersect(const Ogre::Vector3 &dir, const Ogre::Vector3 &pos, int mode)
	{
		m_CurCoordSelect = NO_AXES_SELECT;

		Ogre::Ray ray;
		ray.m_Dir = dir;
		ray.m_Origin = pos;

		if(m_CurCoordMode==COORD_MOVE || m_CurCoordMode==COORD_SCALE)
		{
			if(ray.intersectBox(&m_xAxesBox)) m_CurCoordSelect = SELECT_X_AXES;
			else if(ray.intersectBox(&m_yAxesBox)) m_CurCoordSelect = SELECT_Y_AXES;
			else if(ray.intersectBox(&m_zAxesBox)) m_CurCoordSelect = SELECT_Z_AXES;
			else if(ray.intersectBox(&m_xzAxesBox))
			{
				m_CurCoordSelect = SELECT_XZ_AXES;
				m_CurPos = TestAndXZFace(pos, dir, m_xzAxesBox.getMinPos().y);
			}
			else if(ray.intersectBox(&m_yzAxesBox))
			{
				m_CurCoordSelect = SELECT_YZ_AXES;
				m_CurPos = TestAndYZFace( pos, dir, m_yzAxesBox.getMinPos().x);
			}
			else if(ray.intersectBox(&m_xyAxesBox))
			{
				m_CurCoordSelect = SELECT_XY_AXES;
				m_CurPos = TestAndXYFace(pos, dir, m_xyAxesBox.getMinPos().z);
			}
		}
		else if(m_CurCoordMode == COORD_ROTATE)
		{
			SphereBound sphere;
			sphere.m_Center = m_Origin;
			sphere.m_Radius = SPHERE_RADIUS;

			float t;
			if(!ray.intersectSphere(&sphere, &t)) return NO_AXES_SELECT;

			Ogre::Vector3 pt = ray.m_Origin + ray.m_Dir*t - m_Origin;

			float width = AXES_SIZE;
			if(Abs(pt.x) < width) m_CurCoordSelect = SELECT_X_AXES;
			else if(Abs(pt.y) < width) m_CurCoordSelect = SELECT_Y_AXES;
			else if(Sqrt(pt.x*pt.x+pt.y*pt.y) > SPHERE_RADIUS-width) m_CurCoordSelect = SELECT_Z_AXES;
		}

		return m_CurCoordSelect;
	}
}