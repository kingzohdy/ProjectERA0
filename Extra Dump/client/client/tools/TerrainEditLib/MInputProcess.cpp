
#include ".\minputprocess.h"
#include "EditorCamera.h"
#include <Windows.h>

using namespace TE;

//ÿ�β��о���
#define  WALK_HEIGHT  500

MInputProcess  g_MInputProcess;

MInputProcess::MInputProcess(void)
{

}

MInputProcess::~MInputProcess(void)
{
}
//�����û�����; ����( �༭��ָ�� , �û�����ṹָ�� )	
void MInputProcess::UpdateInput( MTerrainEdit* pEditor ,MUserInput* pUserInput )
{
	assert( pEditor );
	assert( pUserInput );

	pUserInput->bLCtrl = m_Inputer.LControlKeyDown();
	pUserInput->bLShift = m_Inputer.LShiftKeyDown();
	pUserInput->bLAlt   = m_Inputer.LMenuKeyDown();
	
	if( !pUserInput->bLCtrl )
	{
		pUserInput->bLCtrl = m_Inputer.RControlKeyDown();
	}

	if( !pUserInput->bLShift )
	{
		pUserInput->bLShift = m_Inputer.RShiftKeyDown();
	}

	if ( !pUserInput->bLAlt )
	{
		pUserInput->bLAlt   = m_Inputer.RMenuKeyDown();
	}

	if( pEditor->GetCamera() )
	{	
		//�Ƿ��ǵ�һ�˳��ӽ�,���������Զ�Ӧ����
		if( pEditor->m_EditOption.nCurCameraTpye == 0 && pEditor->GetCamera()->m_CameraType == 0 )
		{
			pUserInput->bMoveNear    = m_Inputer.KeyDown( 'S');
			pUserInput->bMoveLong    = m_Inputer.KeyDown( 'W');
			pUserInput->bAddAnim    =  m_Inputer.KeyDown( 'A');
			pUserInput->bDecAnim    =  m_Inputer.KeyDown( 'D');
		}
	}
  
	pUserInput->bLockCamera  = m_Inputer.KeyDown( '9');
	
	pUserInput->bSetRoate  =  m_Inputer.KeyDown( 'E');
	pUserInput->bSetScale   = m_Inputer.KeyDown( 'R');

	pUserInput->bZKeyDown = m_Inputer.KeyDown('Z');
	pUserInput->bCKeyDown = m_Inputer.KeyDown('C');
	pUserInput->bVKeyDown = m_Inputer.KeyDown('V');
	pUserInput->bSkeyDown = m_Inputer.KeyDown('S');
};

void MInputProcess::Process( MTerrainEdit* pEditor  )
{
	assert( pEditor );
     //�����û�����
     UpdateInput( pEditor , &m_UserInput );
     
	 //������ƶ�,�ƽ�����Զ��ͷ
	 if( m_UserInput.bLAlt && m_UserInput.bLCtrl )
	 {
		 pEditor->m_bAltCtrlMoveCamera = true;
	 }
	 else
	 {
		 pEditor->m_bAltCtrlMoveCamera = false;
		 pEditor->m_AltCtrlMoveY = 0;
	 }
     
	 if(m_UserInput.bMoveNear)//'s'
	{
		if( pEditor->m_EditOption.nCurCameraTpye == 0)
		{
			g_Camera.WalkTerrain( g_Camera.m_CameraVec*(-50.0f),&g_Terrain,WALK_HEIGHT);	
		}
		else
		{
			g_Camera.MoveBack ( 2.0f);
		}
    }
    if(m_UserInput.bMoveLong)//'w'
	{		
		if( pEditor->m_EditOption.nCurCameraTpye == 0)
		{	
			g_Camera.WalkTerrain(g_Camera.m_CameraVec*50.0f,&g_Terrain,WALK_HEIGHT);		
		}
		else
		{
			g_Camera.MoveForward ( 2.0f );
		}
	}
	if(m_UserInput.bAddAnim)//'a'
	{
		if( pEditor->m_EditOption.nCurCameraTpye == 0)
		{
			if( g_Camera.m_CameraType == MCamera::LAND )
			{
				g_Camera.StrafeTerrain ( -200.0f , &g_Terrain , WALK_HEIGHT );
			}
			else
			{
				g_Camera.Strafe ( -2.0f);
			}
		}
		else
		{
				g_Camera.Strafe ( -2.0f);
		}
	}
	 if(m_UserInput.bDecAnim)//'d'
	{	
		if( pEditor->m_EditOption.nCurCameraTpye == 0)
		{				
			if( g_Camera.m_CameraType == MCamera::LAND )
			{
				g_Camera.StrafeTerrain ( 200.0f , &g_Terrain , WALK_HEIGHT );
			}
			else
			{
				g_Camera.Strafe ( 2.0f);
			}
		}
		else
		{
				g_Camera.Strafe ( 2.0f);
		}
	}
	//��Ӧ������̴���
	if (m_UserInput.bRoateXAdd )
	{
		pEditor->Roate ( 0.02f , 0 , 0 );
	}
	 if (m_UserInput.bRoateXDec )
	{
		pEditor->Roate ( -0.02f , 0 , 0 );
	}	
	if (m_UserInput.bRoateYAdd )
	{
		pEditor->Roate ( 0 , 0.02f , 0 );
	}
	if (m_UserInput.bRoateYDec )
	{
		pEditor->Roate (  0 ,-0.02f , 0 );
	}	
	if (m_UserInput.bRoateZAdd )
	{
		pEditor->Roate ( 0 , 0   ,0.02f);
	}
	 if (m_UserInput.bRoateZDec )
	{
		pEditor->Roate (  0 , 0 ,-0.02f );
	}			
    //��ͷ����
    g_Camera.Update ();
	//��Ⲣ���ö�Ӧ�ľ�ͷ����
	if( m_UserInput.bFirstCamera )
	{
		//��һ�˳��ӽ�
		g_TerrainEdit.SetCameraType(0);
	}
	if( m_UserInput.bFreeCamera )
	{
		//�����ӽ�
		g_TerrainEdit.SetCameraType(1);
	}
	if ( m_UserInput.bLockCamera )
	{
		//�̶��ӽ�
		g_TerrainEdit.SetCameraType(2);
	}

	//�Ƿ���ת��ͷ ALT + ����������
	static bool bAlt = false;
	static int  saveCameraType = 1;
	if( m_UserInput.bLAlt )
	{ 	
		if( pEditor->GetCameraType() == 2 )
		{
			saveCameraType = 2;             
		}

	    bAlt = true;

		if( g_Camera.m_CameraType != 0 )
		{
			pEditor->SetCameraType (1);
		}
	}
	else
	{
		if( bAlt )
		{
			bAlt = false;	
	    	if( g_Camera.m_CameraType != 0 )
			pEditor->SetCameraType (saveCameraType);
		}
	}

	static bool bzdown,bcdown,bvdown;
	//���˲��� CTRL + Z 
	if ( m_UserInput.bZKeyDown && m_UserInput.bLCtrl )
	{
		if( !bzdown )
		{
			bzdown = true;
			pEditor->Undo();		
		}
	}
	else
	{
		bzdown = false;
	}

    //�������� CTRL + C 
	if( m_UserInput.bCKeyDown && m_UserInput.bLCtrl )
	{ 
		if(!bcdown)
		{
			bcdown = true;
			pEditor->CopySelectObject();
		}
	}
	else
	{
		bcdown = false;
	}
    //ճ������ CTRL + V 
	if( m_UserInput.bVKeyDown && m_UserInput.bLCtrl )
	{
		if( !bvdown )
		{ 
		    bvdown = true;  
			pEditor->CopySelectObjectToTile();
		}
	}
	else
	{
		bvdown = false;
	}
};
