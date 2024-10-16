#include "EditEventHandlerMgr.h"
#include "EditEvent_TextureBrush.h"
#include "EditEvent_HeightMap.h"
#include "EditEvent_ModelMove.h"
#include "EditEvent_ModelBrush.h"
#include "EditEvent_ModelRotate.h"
#include "EditEvent_ModelScale.h"
#include "EditEvent_ModelSelect.h"
#include "EditEvent_ModelRegionBrush.h"
#include "EditEvent_NpcBrush.h"
#include "EditEvent_LinkTile.h"
#include "EditEvent_WaterBrush.h"
#include "EditEvent_Decal.h"
#include "EditEvent_MapRegion.h"
#include "EditEvent_MapPath.h"
#include "EditEvent_MapRegionSelect.h"
#include "EditEvent_LinkTexture.h"
#include "EditEvent_TexModelBrush.h"
#include "EditEvent_Slope.h"
#include "EditEvent_ObjectBrush.h"
#include "MapPlayerBrower.h"
#include "EditEvent_Beach.h"

#include "OgreLog.h"

namespace TE
{
	class MUnknowProcess
		:public BaseEditHandler
	{
	public:
		MUnknowProcess(void){}
		~MUnknowProcess(void){}

		virtual bool handleEditEvent( TerrainEditMgr* pEditor , ActionEvent *event )
		{
			return true;
		}
	};

	EditHandlerManager g_EventProcessFactory;

	EditHandlerManager::EditHandlerManager(void)
	{
	}

	EditHandlerManager::~EditHandlerManager(void)
	{
		Destory();
	}

	void    EditHandlerManager::Destory()
	{
		EVENT_PROCESS_MAP::iterator  iter = m_EventProcessMap.begin();

		for( ; iter != m_EventProcessMap.end() ; iter ++ )
		{
			OGRE_DELETE ( iter->second );
		}

		m_EventProcessMap.clear();
	};

	BaseEditHandler*   EditHandlerManager::GetEventProcess( int nTypeProcess )
	{
		EVENT_PROCESS_MAP::iterator  iter = m_EventProcessMap.find( nTypeProcess );

		if ( iter != m_EventProcessMap.end() )
		{
			return iter->second ;
		}

		BaseEditHandler*   pProcess = NULL ;
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
				pProcess = new TextureBrushEditHandler;
				m_EventProcessMap[nTypeProcess] = pProcess;
			}
			break;
		case  CHANGE_HEIGHT_EVN://编辑高度
			{
				pProcess = new HeightMapEditHandler;
				m_EventProcessMap[nTypeProcess] = pProcess;
			}
			break;
		case ADD_MORE_OBJECT_EVN://增加大量物体
			{			 	
				pProcess = new ModelRegionBrushEditHandler;
				m_EventProcessMap[nTypeProcess] = pProcess;
			}
			break;
		case MODIFY_LIQUID://编辑液体
			{
				pProcess = new WaterBrushEditHandler;
				m_EventProcessMap[nTypeProcess] = pProcess;
			}
			break;
		case MODIFY_BEACH:
			{
				pProcess = new BeachEditHandler;
				m_EventProcessMap[nTypeProcess] = pProcess;
			}
			break;
		case DECAL_EVN://贴花编辑
			{
				pProcess = new DecalEditHandler;
				m_EventProcessMap[nTypeProcess] = pProcess;
			}
			break;
		case AREA_EIDT_EVEN://区域编辑
			{
				pProcess = new RegionEditHandler;
				m_EventProcessMap[nTypeProcess] = pProcess;
			}
			break;
		case SELECT_AREA_EVEN://区域选择
			{
				pProcess = new RegionSelectEditHandler;
				m_EventProcessMap[nTypeProcess] = pProcess;
			}
			break;
		case MODIFY_PATH://路径编辑
			{
				pProcess = new PathEditHandler;
				m_EventProcessMap[nTypeProcess] = pProcess;
			}
			break;
		case AUTOLINK_OBJECT_EVN://自动拼接崖壁
			{
				pProcess = new LinkTileEditHandler;
				m_EventProcessMap[nTypeProcess] = pProcess;
			}
			break;
		case AUTOLINK_TEXTURE_EVN://自动拼接纹理
			{
				pProcess = new LinkTexEditHandler;
				m_EventProcessMap[nTypeProcess] = pProcess;
			}
			break;
		case PAINT_TEXTURE_MODEL_EVN://纹理刷地表
			{
				pProcess = new TexModelBrushEditHandler;
				m_EventProcessMap[nTypeProcess] = pProcess;
			}
			break;
		case BRAE_EDIT_EVN://斜坡编辑
			{		 	
				pProcess = new SlopeEditHandler;
				m_EventProcessMap[nTypeProcess] = pProcess;
			}
			break;
		case PLACE_PLYAER: // place player in scene for previewing
			{
				pProcess = new MapPlayerBrower;
				m_EventProcessMap[nTypeProcess] = pProcess;
			}
			break;
		case SOUND_EDIT_EVN://声音编辑
			{
				//pProcess = new MSoundEditProcess;
				//m_EventProcessMap[nTypeProcess] = pProcess;
			}
			break;
		case EDITEVENT_OBJECTBRUSH:
			{
				pProcess = new ObjectBrushEditHandler;
				m_EventProcessMap[nTypeProcess] = pProcess;
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

	}
}
