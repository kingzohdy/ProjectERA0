#include ".\meventprocessfactory.h"


#include "MPaintTextureProcess.h"
#include "MModifyHeightProcess.h"

#include "MMoveObjectProcess.h"
#include "MAddObjectProcess.h"

#include "MRoateObjectProcess.h"
#include "MScaleObjectProcess.h"

#include "MSelectObjectProcess.h"
#include "MAddMoreObjectsProcess.h"

#include "MAddNpcProcess.h"
#include "MAutoLinkMeshProcess.h"

#include "MModifyLiquidProcess.h"
#include "MUnknowProcess.h"

#include "MEditDecalProcess.h"
#include "MEditAreaProcess.h"

#include "MEditPathProcess.h"
#include "MSelectAreaProcess.h"

#include "MAutoLinkTexProcess.h"
#include "MPaintTextureModelProcess.h"

#include "MBraeEditProcess.h"
#include "MLightEditProcess.h"

#include "MPlacePlayer.h"

#include "OgreLog.h"

using namespace TE;

MEventProcessFactory g_EventProcessFactory;

MEventProcessFactory::MEventProcessFactory(void)
{
}

MEventProcessFactory::~MEventProcessFactory(void)
{
	Destory();
}

void    MEventProcessFactory::Destory()
{
	EVENT_PROCESS_MAP::iterator  iter = m_EventProcessMap.begin();

	for( ; iter != m_EventProcessMap.end() ; iter ++ )
	{
		OGRE_DELETE ( iter->second );
	}
    
	m_EventProcessMap.clear();
};
	
IEventProcess*   MEventProcessFactory::GetEventProcess( int nTypeProcess )
{
	EVENT_PROCESS_MAP::iterator  iter = m_EventProcessMap.find( nTypeProcess );

	if ( iter != m_EventProcessMap.end() )
	{
		return iter->second ;
	}

	IEventProcess*   pProcess = NULL ;
    switch( nTypeProcess )
	 {
	case UNKOWN_ACTION:
		{
			 pProcess = new MUnknowProcess;
			 m_EventProcessMap[nTypeProcess] = pProcess;
		}
		break;
	 case	PANIT_EVN ://ˢ�ر�
		 {
			 pProcess = new MPaintTextureProcess;
			 m_EventProcessMap[nTypeProcess] = pProcess;
		 }
		 break;
	 case  CHANGE_HEIGHT_EVN://�༭�߶�
		 {
			 pProcess = new MModifyHeightProcess;
			 m_EventProcessMap[nTypeProcess] = pProcess;
		 }
		 break;
	 case MOVE_OBJECT_EVN://�ƶ�����
		 {		
			 pProcess = new MMoveObjectProcess;
			 m_EventProcessMap[nTypeProcess] = pProcess;	
		 }
		 break;
	 case ROATE_OBJECT_EVN://��ת����
		 {
			 pProcess = new MRoateObjectProcess;
			 m_EventProcessMap[nTypeProcess] = pProcess;
		 }
		 break;
	 case SCALE_OBJECT_EVN://��������
		 { 
			 pProcess = new MScaleObjectProcess;
			 m_EventProcessMap[nTypeProcess] = pProcess;
		 }
		 break;
	 case  SELECT_OBJECT_EVN://ѡ������
		 {
			 pProcess = new MSelectObjectProcess;
			 m_EventProcessMap[nTypeProcess] = pProcess;
		 }
		 break;
	 case ADD_MORE_OBJECT_EVN://���Ӵ�������
		 {			 	
			 pProcess = new MAddMoreObjectsProcess;
			 m_EventProcessMap[nTypeProcess] = pProcess;
		 }
		 break;
	 case ADD_OBJECT_EVN://��������
		 {		
			 pProcess = new MAddObjectProcess;
			 m_EventProcessMap[nTypeProcess] = pProcess;
		 }
		 break;
	 case ADD_NPC://����NPC
		 {
			 pProcess = new MAddNpcProcess;
			 m_EventProcessMap[nTypeProcess] = pProcess;
		 }
		 break;
	 case MODIFY_LIQUID://�༭Һ��
		 {
			 pProcess = new MModifyLiquidProcess;
			 m_EventProcessMap[nTypeProcess] = pProcess;
		 }
		 break;
	 case DECAL_EVN://�����༭
		 {
			 pProcess = new MEditDecalProcess;
			 m_EventProcessMap[nTypeProcess] = pProcess;
		 }
		 break;
	 case AREA_EIDT_EVEN://����༭
		 {
			 pProcess = new MEditAreaProcess;
			 m_EventProcessMap[nTypeProcess] = pProcess;
		 }
		 break;
	 case SELECT_AREA_EVEN://����ѡ��
		 {
			 pProcess = new MSelectAreaProcess;
			 m_EventProcessMap[nTypeProcess] = pProcess;
		 }
		 break;
	 case MODIFY_PATH://·���༭
		 {
			 pProcess = new MEditPathProcess;
			 m_EventProcessMap[nTypeProcess] = pProcess;
		 }
		 break;
	 case AUTOLINK_OBJECT_EVN://�Զ�ƴ���±�
		 {
			 pProcess = new MAutoLinkMeshProcess;
			 m_EventProcessMap[nTypeProcess] = pProcess;
		 }
		 break;
	 case AUTOLINK_TEXTURE_EVN://�Զ�ƴ������
		 {
			 pProcess = new MAutoLinkTexProcess;
			 m_EventProcessMap[nTypeProcess] = pProcess;
		 }
		 break;
	 case PAINT_TEXTURE_MODEL_EVN://����ˢ�ر�
		 {
			 pProcess = new MPaintTextureModelProcess;
			 m_EventProcessMap[nTypeProcess] = pProcess;
		 }
		 break;
	 case BRAE_EDIT_EVN://б�±༭
		 {		 	
			 pProcess = new MBraeEditProcess;
			 m_EventProcessMap[nTypeProcess] = pProcess;
		 }
		 break;
	 case LIGHT_EDIT_EVN://�ƹ�༭
		 {
			 pProcess = new MLightEditProcess;
			 m_EventProcessMap[nTypeProcess] = pProcess;
		 }
		 break;
     case PLACE_PLYAER: // place player in scene for previewing
         {
            pProcess = new MPlacePlayer;
            m_EventProcessMap[nTypeProcess] = pProcess;
         }
         break;
	 case SOUND_EDIT_EVN://�����༭
		 {
			 //pProcess = new MSoundEditProcess;
			 //m_EventProcessMap[nTypeProcess] = pProcess;
		 }
		 break;
	 default:
		 {
			LOG_SEVERE( "û�ж�Ӧ�¼� ; ID = %d" , nTypeProcess );
			assert( 0 );
		 }
		 break;
	 }

	 if( pProcess == NULL )
	 {		
		 LOG_SEVERE( "�¼������࣬����ʧ�� ; ID = %d" , nTypeProcess );
		 assert( 0 );
	 }

	 return pProcess;

};