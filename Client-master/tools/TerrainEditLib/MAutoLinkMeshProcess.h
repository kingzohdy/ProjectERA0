
#pragma once


#include "EditEventHandler.h"

namespace TE
{

class MAutoLinkMeshProcess
		:public IEventProcess
{
public:
	MAutoLinkMeshProcess(void);
	~MAutoLinkMeshProcess(void);
    //处理过程; 参数( 编辑器指针, 响应事件 ) ; 返回 ( 为真表示成功 )
	virtual bool Process( MTerrainEdit* pEditor , ActionEvent *event );
    //后退操作
	virtual void Undo( MTerrainEdit* pEditor );
protected:
	//设置自动拼接的标记 ; 参数( 编辑器指针, 射线方向, 射线位置 )
	void  SetAutoLinkMeshSign(  MTerrainEdit* pEditor ,MVec3 &dir ,MVec3 &pos);
};


};