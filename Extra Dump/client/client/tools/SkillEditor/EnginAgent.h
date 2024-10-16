#pragma once
#include "SceneGround.h"
#include "OgreWorldPos.h"
#include "OgreColourValue.h"
#include "OgreSceneRendererPostEffect.h"
#include "GameEngineTypes.h"
#include "CameraCtrl.h"
#include "proto_comm.h"
#include "resdb_meta.h"

class CActorCtrl;

const int MAINPALYER_ID		= 1000;
const int MONSTER_START_ID	= 2000;

const int FRIST_MAP_ID = 3301;

class CEnginAgent
{
public:
	CEnginAgent(void);
	~CEnginAgent(void);

	//singleton
public:
	static CEnginAgent* GetInstance();
	static void CreateInstance();
	static void ReleaseInstance();
private:
	static CEnginAgent* ms_pInst;

public:
	bool LoadConfigFile(Ogre::XMLData *pxmldata);
	bool Initialize(HWND hWnd);
	void DestroyScene();
	
	int	 OnWindowMsg(unsigned int msgid, unsigned int wparam, unsigned int lparam);
	void Update(long lTick);
	void Draw(long lTick);

	void SetRatio(float fRatio) { m_fRatio = fRatio; };
	
	void enterWorld(int mapid, int x, int z, int player_id, int player_resid);
	void enterMap( int mapid, int x, int z );

	void createActor( int monsterId );
	
	void initSkillDyAttrib();
public:
	bool m_bIsInitialized;	// �Ƿ�������

public:
	HWND					m_hWnd;
	Ogre::GameScene*		m_pRoom;
	Ogre::SceneRenderer*	m_pSceneRenderer;
	Ogre::Root*				m_pEngineRoot;

	float					m_fRatio;
	float					m_fSpeed;			// ֡��
	int						m_FrameLimit;		// ֡������
	Ogre::ColorQuad			m_backColor;		// ������ɫ
	
	int						m_nViewportWidth;
	int						m_nViewportHeight;
	// ��Ϸ����ģ��
public:
	GameConfig				m_Config;
	DBDataManager*			m_pDBMgr;
	INetworkManager*		m_pNetMgr;
	GameMsgPrint*			m_pGameMsgPrint;

	FollowCamera*			m_pMainCamera;
	GameMap*				m_pCurMap;
	MainGamePlayer*			m_pMainPlayer; 
	ActorManager*			m_pActorMgr;
	int						m_CurMapID;

	Ogre::UIRenderer*		m_pUIRenderer;
	
	Ogre::InputManager*		m_pInputMgr;		// �������
	CActorCtrl*				m_pActionCtrl;		// �༭���������
	
	SkillManager*			m_pGameSkillMgr;	// ���ܹ�����
};
