#pragma once
#include "GameEngineTypes.h"
#include "Item.h"
#include "Equip_Common.h"
#include <map>
#include <vector>

const int TMPAMR_UNFIXDELAY = 1000;// ��ʱ������ʧ�ӳ�

//tolua_begin
int getItemDisplayInfoHairStyle( int armDefId );
//tolua_end

bool IsFashionArm( const ROLEARM& arm );

class Equip//tolua_export
{//tolua_export
public:
	Equip(void);
	~Equip(void);

	static int getMonsterEquip(int resid, int slot);

	void setOwner( GameActor *pActor );
	void changEquip( int GridIndex, const ROLEARM& arm );
	void changEquip( int GridIndex, int armDefId,int nStar = 0 );
	void changAmm( int GridIndex, int ammId );
	void reLoadAllEquip();
	void clearAllEquip();
	int  checkSuit();
	void checkWeapEquip( int pos );
	bool checkAmmUsed( int ammId );

	// ��ȡװ����λ��
	int getEquipPos( int armDefId );
	// ���������������
	int getRightHandEquipType();

	void setTmpOpType(int opType){m_nTmpOpType = opType;};

	//tolua_begin
	Item&	getItem( int GridIndex );

	// ��ȡװ���ĳ�����
	bool getItemLink( int GridIndex );

	// ��ȡ����װ������
	int		getArmType( int pos );
	
	// ������ʱװ��
	void	setTmpArm( int ArmId );
	// ����ʱ����	 
	void	bindTmpArm( GameActor* pActor, int nArmId );
	// ȡ������ʱ����
	void	unBindTmpArm( GameActor* pActor, int nArmId );
	
	// ��ѯ�Ƿ�ʹ����ʱ����
	bool	isUsedTmp();
	// ��ȡ��ʱװ��
	int		getTmpArm();
	// ��ȡ��ʱ��е
	int		getTmpMachin();
	// ʹ����ʱ����
	void	usedTmpArm( int armId=0 );
	// ������ʱ����
	void	unUsedTmpArm();
	// ������ʱ����
	void	looseTmpArm();
	// ��������
	void	hideWeapon();
	// ��ʾ����
	void	showWeapon();

	// ����װװ��
	void send_Equip( int nListType, const Item& item, int wearPos = 0 );
	// ������װ��
	void send_unEquip( int Id, int desListType, int gridIndex, int wearPos );
	// ����װ����ҩ
	void send_EquipAmm( int ammId );
	// ������֯�Ļ�װ�����б�
	void setAvatarAttrib( int nRace, int nGender );
	// ��ȡ����ģ�͵�װ��ģ��
	int getAvatarId( int id );
	// ��ȡ��������������װ��װ������
	int getPlayerSuitEquipNum( int id );
	// ��ö�Ӧ��λ��Ʒ
	int getEquipItemID( int nGridx );
	//�Ƿ�����ʱ��еѲ��
	bool isTmpPatronid();
	int getAvatarStarLv( int id );

	void ShowShoulder( bool bShow );
	void send_SelectFashionSuit( int nSelectIdx );
	void setSelectFashionBag( int nSelect, int nNum );      // ����ʱװ��
	int	 getSelectFashionIndex();							// ���ѡ��ʱװid;
	int  getSelectFashionNum();								// ���ʱװ����;
	// �Ƿ���ɳ̲����
	bool isOnChairTmp();
	//tolua_end
	void CheckAvatarHair();

	

	//��������װ�����ݱ仯
	void SetArmInfo( int GridIndex, const ROLEARM& arm );
public:
	GameActor										*m_Owner;
	Item											m_Arm[EQUIP_POS_END];	
	int												m_ArmEquipId[EQUIP_POS_END];
	int												m_AmmMap[AMM_POS_END];
	int												m_ArmStar[EQUIP_POS_END];
	
	int												m_nFashionSelect;	// ʱװ����ѡ��
	int												m_nFashionNum;		// ʱװ��Ŀ

	int												m_TmpEquip;			// ��ʱ����λ
	int												m_TmpMachin;		// ��ʱ��е

	bool											m_bHideWeap;		// �Ƿ���������
	int												m_nTmpOpType;		// ��ʱ��е�������
	static std::map<int,std::vector<int>>			m_mapSuitSet;		// ���ǰ�����װID������װ����Ʒ
};//tolua_export