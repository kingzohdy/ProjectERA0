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
	 case	PANIT_EVN ://刷地表
		 {
			 pProcess = new MPaintTextureProcess;
			 m_EventProcessMap[nTypeProcess] = pProcess;
		 }
		 break;
	 case  CHANGE_HEIGHT_EVN://编辑高度
		 {
			 pProcess = new MModifyHeightProcess;
			 m_EventProcessMap[nTypeProcess] = pProcess;
		 }
		 break;
	 case MOVE_OBJECT_EVN://移动物体
		 {		
			 pProcess = new MMoveObjectProcess;
			 m_EventProcessMap[nTypeProcess] = pProcess;	
		 }
		 break;
	 case ROATE_OBJECT_EVN://旋转物体
		 {
			 pProcess = new MRoateObjectProcess;
			 m_EventProcessMap[nTypeProcess] = pProcess;
		 }
		 break;
	 case SCALE_OBJECT_EVN://缩放物体
		 { 
			 pProcess = new MScaleObjectProcess;
			 m_EventProcessMap[nTypeProcess] = pProcess;
		 }
		 break;
	 case  SELECT_OBJECT_EVN://选择物体
		 {
			 pProcess = new MSelectObjectProcess;
			 m_EventProcessMap[nTypeProcess] = pProcess;
		 }
		 break;
	 case ADD_MORE_OBJECT_EVN://增加大量物体
		 {			 	
			 pProcess = new MAddMoreObjectsProcess;
			 m_EventProcessMap[nTypeProcess] = pProcess;
		 }
		 break;
	 case ADD_OBJECT_EVN://增加物体
		 {		
			 pProcess = new MAddObjectProcess;
			 m_EventProcessMap[nTypeProcess] = pProcess;
		 }
		 break;
	 case ADD_NPC://增加NPC
		 {
			 pProcess = new MAddNpcProcess;
			 m_EventProcessMap[nTypeProcess] = pProcess;
		 }
		 break;
	 case MODIFY_LIQUID://编辑液体
		 {
			 pProcess = new MModifyLiquidProcess;
			 m_EventProcessMap[nTypeProcess] = pProcess;
		 }
		 break;
	 case DECAL_EVN://贴花编辑
		 {
			 pProcess = new MEditDecalProcess;
			 m_EventProcessMap[nTypeProcess] = pProcess;
		 }
		 break;
	 case AREA_EIDT_EVEN://区域编辑
		 {
			 pProcess = new MEditAreaProcess;
			 m_EventProcessMap[nTypeProcess] = pProcess;
		 }
		 break;
	 case SELECT_AREA_EVEN://区域选择
		 {
			 pProcess = new MSelectAreaProcess;
			 m_EventProcessMap[nTypeProcess] = pProcess;
		 }
		 break;
	 case MODIFY_PATH://路径编辑
		 {
			 pProcess = new MEditPathProcess;
			 m_EventProcessMap[nTypeProcess] = pProcess;
		 }
		 break;
	 case AUTOLINK_OBJECT_EVN://自动拼接崖壁
		 {
			 pProcess = new MAutoLinkMeshProcess;
			 m_EventProcessMap[nTypeProcess] = pProcess;
		 }
		 break;
	 case AUTOLINK_TEXTURE_EVN://自动拼接纹理
		 {
			 pProcess = new MAutoLinkTexProcess;
			 m_EventProcessMap[nTypeProcess] = pProcess;
		 }
		 break;
	 case PAINT_TEXTURE_MODEL_EVN://纹理刷地表
		 {
			 pProcess = new MPaintTextureModelProcess;
			 m_EventProcessMap[nTypeProcess] = pProcess;
		 }
		 break;
	 case BRAE_EDIT_EVN://斜坡编辑
		 {		 	
			 pProcess = new MBraeEditProcess;
			 m_EventProcessMap[nTypeProcess] = pProcess;
		 }
		 break;
	 case LIGHT_EDIT_EVN://灯光编辑
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
	 case SOUND_EDIT_EVN://声音编辑
		 {
			 //pProcess = new MSoundEditProcess;
			 //m_EventProcessMap[nTypeProcess] = pProcess;
		 }
		 break;
	 default:
		 {
			LOG_SEVERE( "没有对应事件 ; ID = %d" , nTypeProcess );
			assert( 0 );
		 }
		 break;
	 }

	 if( pProcess == NULL )
	 {		
		 LOG_SEVERE( "事件处理类，创建失败 ; ID = %d" , nTypeProcess );
		 assert( 0 );
	 }

	 return pProcess;

};