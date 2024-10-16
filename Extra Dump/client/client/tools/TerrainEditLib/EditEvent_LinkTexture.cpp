
#include "EditEvent_LinkTexture.h"
#include "EditorInput.h"
#include "LinkTile.h"
#include "EditorCamera.h"
#include "Terrain.h"
#include "MapMark.h"

using namespace Ogre;

namespace TE
{
	LinkTexEditHandler::LinkTexEditHandler(void)
	{
		m_nProcessType = AUTOLINK_TEXTURE_EVN ;
	}

	LinkTexEditHandler::~LinkTexEditHandler(void)
	{
	}
	//���˲���
	void  LinkTexEditHandler::Undo( TerrainEditMgr* pEditor )
	{
		pEditor->GetAutoLinkTex()->UnDo();
	};
	//�������; ����( �༭��ָ��, ��Ӧ�¼� ) ; ���� ( Ϊ���ʾ�ɹ� )
	bool LinkTexEditHandler::handleEditEvent( TerrainEditMgr* pEditor , ActionEvent *event )
	{	
		switch( event->mouseEven )
		{
		case MouseLDown:
			{
				SetAutoLinkMeshSign( pEditor , event->dir , event->pos ) ;
			}
			break;
		case MouseSewheel:
			{	
				g_Camera.WalkTerrain(event->mz*5.0f,&g_Terrain,2);		
			}
			break;
		case MouseMove:
			{
				Ogre::Vector3  pos0;
				if(g_Terrain.TestAndRay( event->dir,event->pos,pos0))
				{			
					int ggx,ggy ;

					float fSize = 2*TERRBLOCK_SIZE / 32.0f ;

					if ( g_Terrain.GetGridX( pos0.x , &ggx ) && g_Terrain.GetGridZ( pos0.z , &ggy ) )
					{
						Ogre::Vector3 chunkPos ;
						g_Terrain.GetGridPos( &chunkPos , ggx , ggy );

						float ddx , ddy ;
						ddx = pos0.x - chunkPos.x ;
						ddy = pos0.z - chunkPos.z ;

						unsigned char  mmx ,mmy ;
						mmx = (unsigned char)( ddx / ( fSize ) );
						mmy = (unsigned char)( ddy / ( fSize ) );

						pos0 = pEditor->GetEditData()->chunkSet[ggy][ggx].GetChunkVec( 2*mmx+1 ,2*mmy+1 );

						if( event->bMouseDown )
						{
							int nblock = g_TerrainEdit.GetEditData()->gw;

							if ( ( ggx*nblock + mmx ) != pEditor->GetAutoLinkMesh()->m_nCurSelectX||
								( ggy*nblock + mmy ) != pEditor->GetAutoLinkMesh()->m_nCurSelectY )
							{       
								SetAutoLinkMeshSign( pEditor , event->dir , event->pos ) ;
							}
						}

					}

					pEditor->m_pSignSet->m_PosSet.clear ();
					pEditor->m_pSignSet->UpdateData ( pos0 , fSize/2.0f , 16 );
				}
			}
			break;
		}

		return true;
	};
	//�����Զ�ƴ�ӵı�� ; ����( �༭��ָ��, ���߷���, ����λ�� )
	void  LinkTexEditHandler::SetAutoLinkMeshSign(  TerrainEditMgr* pEditor ,Ogre::Vector3 &dir ,Ogre::Vector3 &pos)
	{
		Ogre::Vector3  pos0;
		if(g_Terrain.TestAndRay(dir,pos,pos0))
		{
			//�����Ҫɾ���������ñ�־Ϊ��
			if ( g_MInputProcess.GetUserInput()->bLShift != 0 )
			{
				pEditor->GetAutoLinkTex()->SetMapSign( pos0 , 0 );
			}
			else
			{
				pEditor->GetAutoLinkTex()->SetMapSign( pos0 , 1 );
			}
		}
	}
}
