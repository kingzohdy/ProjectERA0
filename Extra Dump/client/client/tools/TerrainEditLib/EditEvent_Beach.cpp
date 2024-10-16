
#include "EditEvent_Beach.h"
#include "Terrain.h"
#include "EditorCamera.h"
#include "EditorInput.h"
#include "Coordinate.h"

using namespace TE;

BeachEditHandler::BeachEditHandler ()
{
	m_nProcessType = MODIFY_BEACH;
	m_BeachEditOpType = BEACH_OP_ADDPOINT;
}

BeachEditHandler::~BeachEditHandler ()
{

}

bool BeachEditHandler::handleEditEvent (TerrainEditMgr* pEditor , ActionEvent *event)
{
	switch( event->mouseEven )
	{
	case MouseLDown:
		if (BEACH_OP_ADDPOINT == m_BeachEditOpType)
		{
			AddPos(pEditor, event->dir ,event->pos);
		}
		else if (BEACH_OP_SELECT == m_BeachEditOpType)
		{

		}
		break;

	case MouseMove:

		if (g_MInputProcess.isKeyDown(VK_LBUTTON))
		{
			if (BEACH_OP_TRANSLATE == m_BeachEditOpType)
			{
				pEditor->MoveMoveAxes(pEditor->m_select_coordinate, event->mx, event->my, event->pos, event->dir);
			}
		}
		else
		{
			pEditor->m_select_coordinate = pEditor->GetCoordinate()->intersect(event->dir , event->pos , 0);
		}

		break;

	case MouseSewheel:
		g_Camera.WalkTerrain(event->mz*5.0f,&g_Terrain,2);
		break;
	}
	return true;
}

void  BeachEditHandler::Undo( TerrainEditMgr *pEditor )
{
	Ogre::Beach *pBeach = pEditor->getCurrentEditBeach();

	if (pBeach)
	{
		pBeach->removeLastPoint();
	}
}

bool BeachEditHandler::AddPos (TerrainEditMgr* pEditor , Ogre::Vector3 &dir , Ogre::Vector3 &pos)
{
	Ogre::Vector3  pos0;

	if (g_Terrain.TestTerrainAndWater (dir, pos, pos0))
	{
		Ogre::Beach *pCurEditBeach = pEditor->getCurrentEditBeach();

		if (pCurEditBeach)
		{
			pCurEditBeach->addPoint(pos0);

			pCurEditBeach->reBuild();
		}

		return true;
	}

	return false;
}