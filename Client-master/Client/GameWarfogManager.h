
#ifndef __GameWarfogManager_H__
#define __GameWarfogManager_H__

//////////////////////////////////////////////////////////////////////////
// 战争迷雾
//////////////////////////////////////////////////////////////////////////
#include <Windows.h>
#include "OgreSingleton.h"
#include "GameEngineTypes.h"
#include "GameMsgPrint.h"
#include "SystemSettingConfig.h"
#include <vector>
class Frame;
class Texture;
#define NUMPOINT 1024
struct node
{
	int x,y;
};
class GameWarfogManager //tolua_export
	: public Ogre::Singleton<GameWarfogManager>
{//tolua_export
public:
	GameWarfogManager();
	~GameWarfogManager();
	void init();//进入副本时的初始化
	void clear();//完成或重置副本时清空数据
	void update();
	void updateBlock(float x,float z);//更新3D地形块
	void updateBlockstate(int indeX,int indexZ,int x,int z);//依据索引坐标更新具体的一块block状态
	void updateShowMap(int x,int y);//更新地图的UI显示
	void drawRound(int x,int y);//正负法线画圆法
	void drawPoint(int x,int y,int xh,int yh);//描点
	void initTexture();
	void reset();
	void addMappoint(int x, int y);
	void redrawMap();

	//第一个参数为距离的平方，参考值设定为500000.第二三个参数为雾的浓度取值范围为0到1。0表示没有雾，1表示为全雾。第二个参数为没有探索过地区的雾的浓度参考值1，第三个为探索过不在视野的地区的浓度参考值0.85。后面3个为雾的颜色值，因为shader里面颜色的取值为0到1。所以这里颜色的取值范围也是0到1
	//tolua_begin
	void setWarfog( int wardistance, float outchroma, float postchroma, float r, float g, float b);
	bool ismapbuttonshow(int x, int z);
	//tolua_end
private:
	Ogre::TerrainTile*	m_pCurTile;//总block块指针
	Ogre::TextureData*	m_pTexdata;//地图UI资源指针
	Ogre::SurfaceData*	m_pTexsuface;//地图纹理表面指针
	Texture*			m_pUiTexture;
	int					m_wardistance;//玩家的视野范围
	float               m_outchroma,m_postchroma;//雾的浓度，没有探索过不可见的地区，探索过不可见的区域
	float				m_warcolor[3];//雾的颜色
	std::vector<node>	m_postnode;
	std::vector<Frame*>	m_mapNpcnode;
};//tolua_export

#endif