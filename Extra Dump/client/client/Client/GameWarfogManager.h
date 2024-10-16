
#ifndef __GameWarfogManager_H__
#define __GameWarfogManager_H__

//////////////////////////////////////////////////////////////////////////
// ս������
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
	void init();//���븱��ʱ�ĳ�ʼ��
	void clear();//��ɻ����ø���ʱ�������
	void update();
	void updateBlock(float x,float z);//����3D���ο�
	void updateBlockstate(int indeX,int indexZ,int x,int z);//��������������¾����һ��block״̬
	void updateShowMap(int x,int y);//���µ�ͼ��UI��ʾ
	void drawRound(int x,int y);//�������߻�Բ��
	void drawPoint(int x,int y,int xh,int yh);//���
	void initTexture();
	void reset();
	void addMappoint(int x, int y);
	void redrawMap();

	//��һ������Ϊ�����ƽ�����ο�ֵ�趨Ϊ500000.�ڶ���������Ϊ���Ũ��ȡֵ��ΧΪ0��1��0��ʾû����1��ʾΪȫ���ڶ�������Ϊû��̽�������������Ũ�Ȳο�ֵ1��������Ϊ̽����������Ұ�ĵ�����Ũ�Ȳο�ֵ0.85������3��Ϊ�����ɫֵ����Ϊshader������ɫ��ȡֵΪ0��1������������ɫ��ȡֵ��ΧҲ��0��1
	//tolua_begin
	void setWarfog( int wardistance, float outchroma, float postchroma, float r, float g, float b);
	bool ismapbuttonshow(int x, int z);
	//tolua_end
private:
	Ogre::TerrainTile*	m_pCurTile;//��block��ָ��
	Ogre::TextureData*	m_pTexdata;//��ͼUI��Դָ��
	Ogre::SurfaceData*	m_pTexsuface;//��ͼ�������ָ��
	Texture*			m_pUiTexture;
	int					m_wardistance;//��ҵ���Ұ��Χ
	float               m_outchroma,m_postchroma;//���Ũ�ȣ�û��̽�������ɼ��ĵ�����̽�������ɼ�������
	float				m_warcolor[3];//�����ɫ
	std::vector<node>	m_postnode;
	std::vector<Frame*>	m_mapNpcnode;
};//tolua_export

#endif