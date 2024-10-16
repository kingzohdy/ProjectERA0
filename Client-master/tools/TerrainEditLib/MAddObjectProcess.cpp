
#include ".\maddobjectprocess.h"

#include "EditorModel.h"
#include "EditorCamera.h"
#include "MTerrain.h"
#include "Coordinate.h"

using namespace TE;
using namespace Ogre;

MAddObjectProcess::MAddObjectProcess(void)
{	
	m_nProcessType = ADD_OBJECT_EVN ;
}

MAddObjectProcess::~MAddObjectProcess(void)
{
}
//�������; ����( �༭��ָ�� , ��Ӧ�¼���Ϣ ) ����( Ϊ���ʾ�ɹ� )
bool MAddObjectProcess::Process( MTerrainEdit* pEditor , ActionEvent *event )
{
	if ( pEditor->m_bLockModelEdit[ pEditor->m_EditOption.mModelType ] )
	{
		return true;
	}

	switch( event->mouseEven )
	{
	case MouseLDown:
		{			
			AddObjectTitle( pEditor );		
			pEditor->m_select_coordinate = pEditor->GetCoordinate()->intersect ( event->dir , event->pos, 0);
			MoveSelectModel( pEditor , event->dir , event->pos );

			pEditor->SetMoveAxes( event->mx , event->my );	
		}
		break;
	case MouseMove:
		{
			pEditor->MoveSign(event->dir , event->pos );
		}
		break;
	case MouseLUp:
		{
			// AddSaveRedoData();
		}
		break;
	case MouseSewheel:
		{		
			g_Camera.WalkTerrain(event->mz*5.0f,&g_Terrain,2);	
		}
		break;
	}
	return true;
};
//��TILE����һ������; ����( �༭��ָ�� )
void  MAddObjectProcess::AddObjectTitle( MTerrainEdit* pEditor )
{
	if( pEditor->GetEditData() == NULL )
	{
		return;
	}

	if(pEditor->m_SelResData.seltype!=SELECT_RES_MODEL || pEditor->m_SelResData.respath.empty())
	{
		return;
	}

	int index = 0;
	index = pEditor->AddModelName(pEditor->m_SelResData.respath);

	TerrModel*  model = new TerrModel;
	model->GetModelData()->cType = pEditor->m_EditOption.mModelType ;
	strcpy(model->GetModelData()->name ,pEditor->m_SelResData.respath.c_str());
	model->GetModelData()->modelID = index;
    model->Init();
	
	pEditor->m_pCurSelectModel = model;	
	pEditor->m_EditOption.pSelectModel = model;
		 
	model->AttachRoom();

	pEditor->GetEditData()->modelData .modelSet .push_back (model);

	pEditor->GetCoordinate()->bindObject ( model->GetModelData ()->pModel );

	g_Camera.BindObject ( model );
};
// ����ֲ��ģ��; ����( �༭��ָ�� ,�ļ���, ����λ��,���߷���,���� )
void  MAddObjectProcess::AddPlantModelToTitle( MTerrainEdit* pEditor,const char* filename,MVec3 pos,MVec3 dir,float fscale)
{
	if( pEditor->GetEditData() == NULL )
	{
		return;
	}

	if( pEditor->m_SelResData.pmodel == NULL)
	{
		return;
	}

	int   index = pEditor->AddModelName(pEditor->m_SelResData.respath);

	TerrModel*  model = new TerrModel;
	strcpy(model->GetModelData ()->name , pEditor->m_SelResData.respath.c_str());
	model->GetModelData ()->modelID = index;
    model->Init ();
	model->GetModelData ()->pos= pos ;
	
	dir.x = (float)( rand() % 360 );

	if( pEditor->m_EditOption.curMinScale == pEditor->m_EditOption.curMaxScale )
	{
		fscale = 1.0f;
	}
	else
	{
		fscale =  ( pEditor->m_EditOption.curMinScale + rand()%( pEditor->m_EditOption.curMaxScale - pEditor->m_EditOption.curMinScale ) ) / 100.0f;
	}

	model->GetModelData ()->dir = dir;
	model->GetModelData ()->cType = pEditor->m_EditOption.mModelType ;
	model->GetModelData ()->scale = fscale;
	model->ResetMat();
	model->AttachRoom();

	pEditor->GetEditData()->modelData .modelSet .push_back ( model );

}