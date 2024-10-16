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
	EQUIP_POS_FASHION_BODY  = 8, 	/* 时装位|胸 */
	EQUIP_POS_BADGE			= 9, 
	EQUIP_POS_AMULET		= 10, 
	EQUIP_POS_RING			= 11, 	/* 戒指坑位 */
	EQUIP_POS_RING1			= 12, 	/* 第一个戒指坑位 */
	EQUIP_POS_RING2			= 13, 	/* 第二个戒指坑位 */
	EQUIP_POS_RING3			= 14, 	/* 第三个戒指坑位 */
	EQUIP_POS_SHOE			= 15, 
	EQUIP_POS_QIYUE			= 16, 
	EQUIP_POS_NECKLACE		= 17, 	/* 项链 */
	EQUIP_POS_SACHET		= 18, 	/* 香囊 */
	EQUIP_POS_GRAIL			= 19,

    EQUIP_POS_AMULET1       = 20, 	/* 第一个护身符坑位 */
    EQUIP_POS_AMULET2       = 21, 	/* 第二个护身符坑位 */
	EQUIP_POS_FASHION_HEAD	= 22, 	/* 时装位|头 */
	EQUIP_POS_FASHION_HAIR	= 23, 	/* 时装位|发 */
	EQUIP_POS_FASHION_BACK	= 24, 	/* 时装位|背 */
	EQUIP_POS_FASHION_SHOES = 25, 	/* 时装位|鞋 */
	EQUIP_POS_FASHION_HAND	= 26, 	/* 时装位|手 */
	EQUIP_POS_FASHION_LEG	= 27, 	/* 时装位|腿 */
	EQUIP_POS_FASHION_BODY2 = 28, 	/* 时装位|胸 */
	EQUIP_POS_FASHION_HEAD2 = 29, 	/* 时装位|头 */
	EQUIP_POS_FASHION_HAIR2 = 30, 	/* 时装位|发 */
	EQUIP_POS_FASHION_BACK2 = 31, 	/* 时装位|背 */
	EQUIP_POS_FASHION_SHOES2 = 32, 	/* 时装位|鞋 */
	EQUIP_POS_FASHION_HAND2 = 33, 	/* 时装位|手 */
	EQUIP_POS_FASHION_LEG2 = 34, 	/* 时装位|腿 */
	EQUIP_POS_FASHION_BODY3 = 35, 	/* 时装位|胸 */
	EQUIP_POS_FASHION_HEAD3 = 36, 	/* 时装位|头 */
	EQUIP_POS_FASHION_HAIR3 = 37, 	/* 时装位|发 */
	EQUIP_POS_FASHION_BACK3 = 38, 	/* 时装位|背 */
	EQUIP_POS_FASHION_SHOES3 = 39, 	/* 时装位|鞋 */
	EQUIP_POS_FASHION_HAND3 = 40, 	/* 时装位|手 */
	EQUIP_POS_FASHION_LEG3 = 41, 	/* 时装位|腿 */

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
	POS_ID_FASHION_HAIR = 14,		// 不做avatar处理，只是来标示是否有时装
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

static int g_ArmEquipId_List[EQUIP_POS_END];					// 预览模型装备位
static int g_ArmStarLv_List[EQUIP_POS_END];						// 预览模型装备位 星级
static int g_AvatarAttrib[CLIENT_MAX_AVATAR_ATTRIB];									// 预览模型
static int g_AvatarStar[CLIENT_MAX_AVATAR_ATTRIB];									// 预览模型	星级

static int g_AvatarSelect;										// 预览模型第几套

extern int	g_checkSuit( int* EquipList );						// 检测套装
extern void g_AvatarChange();									// 装备位转化
extern int getArmEquip( int index );							// 获取对应格位的装备
extern void setArmEquipIdList( int index, int id, int starLv );	// 设置装备位
extern void clearEquipIdList();									// 清空装备位		
extern int  getAvatarAttrib( int index );						// 获得装备位信息(此数据用于ModelViewer)
extern void setAvatarAttrib( int index, int nvalue );			// 设置装备位信息(此数据用于ModelViewer)

extern int getAvatarStarLv( int index );						//	获得装备位 星级
extern void setAvatarStarLv( int index, int nvalue );			//	设置装备位 星级

extern int getSuitSetId( int id, int index );					// 获取套装的装备ID
extern int getSuitEquipNum( int id );							// 获取套装的装备数量

extern void setAvatarSelectSuit( int nIndex );					// 设置时装选择
extern void g_ConfigFashionFair();

extern int convertEquipPos2PosId( int equipPos );
//tolua_end

// 装备位置同引擎装备位转化
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