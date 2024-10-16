#pragma once

//tolua_begin
enum EEQUIP_POS 
{
	EQUIP_POS_NO			= 0, 
	EQUIP_POS_RIGHTHAND		= 1, 
	EQUIP_POS_HEAD			= 2, 
	EQUIP_POS_BODY			= 3, 
	EQUIP_POS_BACK			= 4, 
	EQUIP_POS_LEFT			= 5, 
	EQUIP_POS_FACE			= 6, 
	EQUIP_POS_SHOULDER		= 7, 
	EQUIP_POS_FASHION_BODY  = 8, 	/* ʱװλ|�� */
	EQUIP_POS_BADGE			= 9, 
	EQUIP_POS_AMULET		= 10, 
	EQUIP_POS_RING			= 11, 	/* ��ָ��λ */
	EQUIP_POS_RING1			= 12, 	/* ��һ����ָ��λ */
	EQUIP_POS_RING2			= 13, 	/* �ڶ�����ָ��λ */
	EQUIP_POS_RING3			= 14, 	/* ��������ָ��λ */
	EQUIP_POS_SHOE			= 15, 
	EQUIP_POS_QIYUE			= 16, 
	EQUIP_POS_NECKLACE		= 17, 	/* ���� */
	EQUIP_POS_SACHET		= 18, 	/* ���� */
	EQUIP_POS_GRAIL			= 19,

    EQUIP_POS_AMULET1       = 20, 	/* ��һ���������λ */
    EQUIP_POS_AMULET2       = 21, 	/* �ڶ����������λ */
	EQUIP_POS_FASHION_HEAD	= 22, 	/* ʱװλ|ͷ */
	EQUIP_POS_FASHION_HAIR	= 23, 	/* ʱװλ|�� */
	EQUIP_POS_FASHION_BACK	= 24, 	/* ʱװλ|�� */
	EQUIP_POS_FASHION_SHOES = 25, 	/* ʱװλ|Ь */
	EQUIP_POS_FASHION_HAND	= 26, 	/* ʱװλ|�� */
	EQUIP_POS_FASHION_LEG	= 27, 	/* ʱװλ|�� */
	EQUIP_POS_FASHION_BODY2 = 28, 	/* ʱװλ|�� */
	EQUIP_POS_FASHION_HEAD2 = 29, 	/* ʱװλ|ͷ */
	EQUIP_POS_FASHION_HAIR2 = 30, 	/* ʱװλ|�� */
	EQUIP_POS_FASHION_BACK2 = 31, 	/* ʱװλ|�� */
	EQUIP_POS_FASHION_SHOES2 = 32, 	/* ʱװλ|Ь */
	EQUIP_POS_FASHION_HAND2 = 33, 	/* ʱװλ|�� */
	EQUIP_POS_FASHION_LEG2 = 34, 	/* ʱװλ|�� */
	EQUIP_POS_FASHION_BODY3 = 35, 	/* ʱװλ|�� */
	EQUIP_POS_FASHION_HEAD3 = 36, 	/* ʱװλ|ͷ */
	EQUIP_POS_FASHION_HAIR3 = 37, 	/* ʱװλ|�� */
	EQUIP_POS_FASHION_BACK3 = 38, 	/* ʱװλ|�� */
	EQUIP_POS_FASHION_SHOES3 = 39, 	/* ʱװλ|Ь */
	EQUIP_POS_FASHION_HAND3 = 40, 	/* ʱװλ|�� */
	EQUIP_POS_FASHION_LEG3 = 41, 	/* ʱװλ|�� */

	EQUIP_POS_END			= 42,
};

enum EEQUIP_POS_ID
{
	POS_ID_HAIR			= 2,
	POS_ID_FACE			= 3,
	POS_ID_CLOSES		= 4,
	POS_ID_LEFTHAND		= 5,
	POS_ID_RIGHTHAND	= 6,
	POS_ID_HAT			= 7,
	POS_ID_SHOULD		= 8,
	POS_ID_BACK			= 9,
	POS_ID_SUIT			= 10,
	POS_ID_FASHION_BODY	= 11,
	POS_ID_MASK			= 12,	
	POS_ID_HEAD			= 13,
	POS_ID_FASHION_HAIR = 14,		// ����avatar����ֻ������ʾ�Ƿ���ʱװ
	POS_ID_FASHION_HEAD = 15,
	POS_ID_FASHION_BACK = 16,
	POS_ID_FASHION_SHOES = 17,
	POS_ID_FASHION_HAND	= 18,
	POS_ID_FASHION_LEG  = 19,
};

enum EAMM_POS_ID
{
	AMM_POS_LEFT		= 0,
	AMM_POS_RIGHT		= 1,
	AMM_POS_SHOULDER	= 2,

	AMM_POS_END,
};

enum
{
	CLIENT_MAX_AVATAR_ATTRIB = 32,
};

static int g_ArmEquipId_List[EQUIP_POS_END];					// Ԥ��ģ��װ��λ
static int g_ArmStarLv_List[EQUIP_POS_END];						// Ԥ��ģ��װ��λ �Ǽ�
static int g_AvatarAttrib[CLIENT_MAX_AVATAR_ATTRIB];									// Ԥ��ģ��
static int g_AvatarStar[CLIENT_MAX_AVATAR_ATTRIB];									// Ԥ��ģ��	�Ǽ�

static int g_AvatarSelect;										// Ԥ��ģ�͵ڼ���

extern int	g_checkSuit( int* EquipList );						// �����װ
extern void g_AvatarChange();									// װ��λת��
extern int getArmEquip( int index );							// ��ȡ��Ӧ��λ��װ��
extern void setArmEquipIdList( int index, int id, int starLv );	// ����װ��λ
extern void clearEquipIdList();									// ���װ��λ		
extern int  getAvatarAttrib( int index );						// ���װ��λ��Ϣ(����������ModelViewer)
extern void setAvatarAttrib( int index, int nvalue );			// ����װ��λ��Ϣ(����������ModelViewer)

extern int getAvatarStarLv( int index );						//	���װ��λ �Ǽ�
extern void setAvatarStarLv( int index, int nvalue );			//	����װ��λ �Ǽ�

extern int getSuitSetId( int id, int index );					// ��ȡ��װ��װ��ID
extern int getSuitEquipNum( int id );							// ��ȡ��װ��װ������

extern void setAvatarSelectSuit( int nIndex );					// ����ʱװѡ��
extern void g_ConfigFashionFair();

extern int convertEquipPos2PosId( int equipPos );
//tolua_end

// װ��λ��ͬ����װ��λת��
static int EquipPos_2_PosId[EQUIP_POS_END] = {
	EQUIP_POS_NO,					//EQUIP_POS_NO
	POS_ID_RIGHTHAND,
	POS_ID_HAT,		
	POS_ID_CLOSES,	
	POS_ID_BACK,		
	POS_ID_LEFTHAND,	
	POS_ID_MASK,		
	POS_ID_SHOULD,	
	POS_ID_FASHION_BODY,
	EQUIP_POS_NO,					//EQUIP_POS_BADGE
	EQUIP_POS_NO,					//EQUIP_POS_AMULET
	EQUIP_POS_NO,					//EQUIP_POS_RING
	EQUIP_POS_NO,					//EQUIP_POS_RING1
	EQUIP_POS_NO,					//EQUIP_POS_RING2
	EQUIP_POS_NO,					//EQUIP_POS_RING2
	EQUIP_POS_NO,					//EQUIP_POS_SHOE
	EQUIP_POS_NO,					//EQUIP_POS_QIYUE
	EQUIP_POS_NO,					//EQUIP_POS_NECKLACE
	EQUIP_POS_NO,					//EQUIP_POS_SACHET
	EQUIP_POS_NO,					//EQUIP_POS_GRAIL
	EQUIP_POS_NO,					//EQUIP_POS_AMULET1
	EQUIP_POS_NO,					//EQUIP_POS_AMULET2
	POS_ID_FASHION_HEAD,
	POS_ID_FASHION_HAIR,
	POS_ID_FASHION_BACK,
	POS_ID_FASHION_SHOES,
	POS_ID_FASHION_HAND,
	POS_ID_FASHION_LEG,
	POS_ID_FASHION_BODY,
	POS_ID_FASHION_HEAD,
	POS_ID_FASHION_HAIR,
	POS_ID_FASHION_BACK,
	POS_ID_FASHION_SHOES,
	POS_ID_FASHION_HAND,
	POS_ID_FASHION_LEG,
	POS_ID_FASHION_BODY,
	POS_ID_FASHION_HEAD,
	POS_ID_FASHION_HAIR,
	POS_ID_FASHION_BACK,
	POS_ID_FASHION_SHOES,
	POS_ID_FASHION_HAND,
	POS_ID_FASHION_LEG
};