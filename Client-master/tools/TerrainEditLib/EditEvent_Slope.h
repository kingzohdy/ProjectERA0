
#pragma once

#include "EditEventHandler.h"

#include <string>
#include <deque>

namespace TE
{
	//斜坡信息
	struct MBreaInfo
	{
		int gx,gy;//格子位置, 
		int mx,my;//格子中的位置 
		int  x,y ;//地图MAP位置
		TerrModel*   pLModel;//斜坡左边模型
		TerrModel*   pRmodel;//斜坡右边模型

		float  fHeight ;//高度
	};
	//增加斜坡信息
	struct MAddBreaInfo
	{
		int x,y;//x,y坐标
		int nType;//类型
		Ogre::Vector3 dir,pos;//
	};

	class SlopeEditHandler : public BaseEditHandler
	{
	public:
		SlopeEditHandler(void);
		virtual ~SlopeEditHandler(void);
		//处理过程;参数( 编辑器指针, 消息响应事件 ) ; 返回 ( 为真表示成功 )
		virtual bool handleEditEvent( TerrainEditMgr* pEditor , ActionEvent *event );
		//后退操作 ; 参数( 编辑器指针 )
		virtual  void  Undo( TerrainEditMgr *pEditor );
		//保存数据 ; 参数( 编辑器指针)
		virtual  void  SaveData( TerrainEditMgr* pEditor );
		//设置斜坡模型; 参数( 文件名 )
		void           SetBraeModelName( std::string &name );
		//更新
		void           Update();
		//删除当前斜坡; 参数( 编辑器指针 )
		void           DelCurBrae( TerrainEditMgr* pEditor );
		//保存;参数( 文件名 ) ; 返回 ( 为真表示成功 )
		bool           Save( const char* filename );
		//读取;参数( 编辑器指针, 文件名 ) ;返回( 为真表示成功)
		bool           Read( TerrainEditMgr* pEditor , const char* filename );
		//动态引导; 参数( 编辑器指针 )
		void          Loading( TerrainEditMgr* pEditor );
	private:
		//增加斜坡; 参数( 编辑器指针,射线方向,射线位置)
		void          AddBrea( TerrainEditMgr* pEditor , Ogre::Vector3 dir , Ogre::Vector3 pos ) ;
		//删除斜破; 参数( 编辑器指针, 斜坡信息 )
		void          DelBrea(  TerrainEditMgr* pEditor ,MBreaInfo*  pInfo);
		//增加模型; 参数( 文件名,位置 )
		TerrModel*   AddModel( const char* filename , Ogre::Vector3 pos );
		//更新chunk高度; 参数( 格子坐标X,Y ,小格子坐标 X,Y )
		void          UpdateChunkHeight( int x, int y  ,int mx ,int my );
		//更新保存的CHUNK高度 ; 参数( 格子坐标X,Y ,小格子坐标 X,Y , 高度 )
		void          UpdateSaveChunkHeight( int x ,int y ,int mx ,int my , float fHeight );
		//查找斜坡; 参数( 格子坐标X,Y ,小格子坐标 X,Y ) ; 返回( 为真表示找到 )
		bool           FindeBrea( int gx ,int gy ,int mx ,int my );
	private:
		//是否被引导
		bool           m_bLoaded;
		//斜坡最小高度和最大高度
		float         m_fMinHeight,m_fMaxHeight ;
		//当前移动的格子位置 ( gx , gy )
		int            m_nCurMoveGx , m_nCurMoveGy;
		//当前移动的小格子位置( mx , my )
		int            m_nCurMoveMx , m_nCurMoveMy;
		//斜坡信息数组
		typedef std::vector<MBreaInfo>     MBREAINFO_VEC;
		std::vector<MBreaInfo>       m_BreaInfoSet;

		typedef std::vector<MAddBreaInfo>  MADDBREA_VEC;
		//保存增加的斜坡信息
		std::vector<MAddBreaInfo>    m_SaveBreaInfoSet;
		//斜坡信息
		std::vector<MAddBreaInfo>    m_BreaSet;
	};

};
