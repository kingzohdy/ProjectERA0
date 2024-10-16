
#pragma once


#include "EditEventHandler.h"

namespace TE
{

	class LinkTileEditHandler : public BaseEditHandler
	{
	public:
		LinkTileEditHandler(void);
		~LinkTileEditHandler(void);
		//处理过程; 参数( 编辑器指针, 响应事件 ) ; 返回 ( 为真表示成功 )
		virtual bool handleEditEvent( TerrainEditMgr* pEditor , ActionEvent *event );
		//后退操作
		virtual void Undo( TerrainEditMgr* pEditor );
	protected:
		//设置自动拼接的标记 ; 参数( 编辑器指针, 射线方向, 射线位置 )
		void  SetAutoLinkMeshSign(  TerrainEditMgr* pEditor ,Ogre::Vector3 &dir ,Ogre::Vector3 &pos);
	};


};