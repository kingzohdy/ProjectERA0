#pragma once
#include "ui_common.h"
#include "OgreInputHandler.h"

enum eCursorType
{
	eCursorMoveChange = 0,	//������ƶ���ʱ���ı䡣
	eCursorOnlyOnUi,		//ֻ����UI��
	eCursorNoMoveChange,			//������ƶ���ʱ�򲻻�ı�
};

const int MAX_LEVEL_CAN_MOVE = eCursorNoMoveChange;
class XMLManager;


static int s_CursorLevel[] =
{
	eCursorMoveChange,		//��ͨ
	eCursorMoveChange,		//�޷����
	eCursorMoveChange,			//����
	eCursorMoveChange,		//ʰȡ
	eCursorMoveChange,		//NPC����
	eCursorNoMoveChange,		//����
	eCursorNoMoveChange,		//��ֹ���׵�״̬
	eCursorMoveChange,			//̸��
	eCursorNoMoveChange,		//����
	eCursorMoveChange,		//�ڿ�
	eCursorMoveChange,		//ѧϰ����
	eCursorMoveChange,		//ץ����
	eCursorOnlyOnUi,		//������
	eCursorMoveChange,		//drag
	eCursorOnlyOnUi,		//ʰȡ��ťָ��

	eCursorMoveChange,		//�޷�����
	eCursorMoveChange,		//�޷�����
	eCursorMoveChange,		//�޷�ʰȡ
	eCursorMoveChange,		//�޷��ʼ�
	eCursorMoveChange,		//�޷��ڿ�
	eCursorMoveChange,		//�޷�����
	eCursorMoveChange,		//�޷�̸��
	eCursorMoveChange,		//�޷�ѵ��
};

//-----------------------------------------------------------------------------
//  �ͻ��˿ɵ���UI�ĺ����ӿ�
//-----------------------------------------------------------------------------
class GameUI : public Ogre::InputHandler
{
//�Կͻ��˵Ľӿں���
public:
	GameUI();
	~GameUI();

	virtual int HandleInput(const Ogre::InputEvent &event);

	bool Create( const char *uires, int w, int h, Ogre::UIRenderer* pDisplay, Ogre::ScriptVM *pscriptvm);

	//void Destroy();
	void Update( float deltatime );
	void Update( float deltatime, int nInterval, float fThreshold );
	void Render();
	void SendEvent(const char *event);
	void SetCurrentCursor(const char *state);
	void ShowCursor(bool bShow);
	bool isCursorDrag();
	const char *getCurrentCursor();

	void ShowUIPanel(const char *frameName);
	void HideUIPanel(const char *frameName);

	void UIReceiveMessage(bool bReceive);
//��UI�༭�����õĽӿں���
public:
	void SetEditMode(int mode);
	int GetEditMode();

	bool NewXMLFile(const char *uires);
	bool LoadXMLFile(const char *path);
	bool SaveXMLFile(const char *path);

//��UI�ڲ����õĺ���
public:
	static void InitRootFrames();

private:
	XMLManager *m_pXmlmgr;
	bool m_bReceiveMsg;
};