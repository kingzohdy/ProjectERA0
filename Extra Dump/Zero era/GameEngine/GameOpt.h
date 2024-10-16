#pragma once
//tolua_begin
enum
{
	GAME_OPT_SHOW_ALLPLAYER_AND_PET_NAME = 0,		// ��ʾ������Ұ����������ֺͳ��������
	GAME_OPT_SHOW_ALLPLAYER_TEAM_DESIGNATION_NAME,	// ��ʾ������Ұ������Ƕ���ͳƺ�
	//GAME_OPT_SHOW_MAINPET_NAME,					// ��ʾ���ǵĳ�������
	GAME_OPT_SHOW_MAIN_CLAN_NAME,					// ��ʾ���ǹ�������
	GAME_OPT_SHOW_MONSTER_NAME,						// ��ʾ��������
	GAME_OPT_SHOW_MONSTER_HP,						// ��ʾ����Ѫ��
	GAME_OPT_SHOW_OTHERPLAYER_HP,					// ��ʾ�������Ѫ��
	//GAME_OPT_SHOW_OTHERPLAYER_PET_HP,				// ��ʾ������ҳ���Ѫ��
	GAME_OPT_SHOW_OTHERPLAYER_CLAN_NAME,			// ��ʾ���ǹ�������
	GAME_OPT_SHOW_MAINPLAYER_AND_TEAM_HP,					// ��ʾ����Ѫ��
	GAME_OPT_SHOW_OTHERPLAYER_TALK_POP,				// ��ʾ������������
	GAME_OPT_SHOW_MAINPLAYER_DAMAGE_SHOW,			// ��ʾðѪ�˺�
	GAME_OPT_SHOW_EQUIP_SHOW,						// ��ʾװ���Ƚ�
	GAME_OPT_REFUSE_RIDE_INVITE,					// �ܾ��������
	GAME_OPT_REFUSE_ADD_FRIEND_REQ,					// �ܾ���������
	GAME_OPT_REFUSE_CLAN_INVITE,					// �ܾ���������
	GAME_OPT_REFUSE_TRADE_REQ,						// �ܾ���������
	GAME_OPT_REFUSE_TEAM_INVITE,					// �ܾ��������
	GAME_OPT_REFUSE_SEE_EQUIP,						// �ܾ��鿴װ��
	GAME_OPT_REFUSE_REFUSE_PK,						// �ܾ��д�����
	GAME_OPT_SWAP_MOUSE,							// �������Ҽ�
	GAME_OPT_SWAP_MOUSE_DEFAULT_SHOW_HAVE_SELECT,	// �´ε�¼�Զ�Ĭ����ѡģʽ
	GAME_OPT_LAST_PET_SELECT,						// �ϴ�ѡ�еĳ���
	GAME_OPT_LOCK_SHORTCUT,							// ���������
	GAME_OPT_FORBIT_SELECT_PET,						// ��ֹѡ�г���
	GAME_OPT_LAST_PET_FIGHT,						// �ϴγ�ս�ĳ���
	GAME_OPT_AUTO_SKILL_RELEASE,                    // �����Զ��ͷ�
	GAME_OPT_AUTO_ACCEPT_TEAM_FOLLOW,				// �Զ�������Ӹ���Ҫ��
	GAME_OPT_SHOW_PERFECT_STAR_EFFECT,				// ��ʾ��������Ч
	GAME_OPT_SHOW_STAR_STONE,						// ��ʾ��Ȩ����
	GAME_OPT_CAMERA_3D,						// ��ʾ��Ȩ����
	//*******************/
	MAX_GAME_SET_OPT = 32,
};

//tolua_end

// ��Ϸ���ù���buffer
class GameSettingConfig
{
public:
	GameSettingConfig();
	void	setGameOpt( int nIndex, char chValue );
	bool	getGameOpt( int nIndex );
	char	getGameOptValue( int nIndex );
	unsigned int getMaxSize()
	{
		return MAX_GAME_SET_OPT;
	}
	char*	getRawBuffer()
	{
		return m_szSettingData;
	}

public:
	char	m_szSettingData[MAX_GAME_SET_OPT];
};