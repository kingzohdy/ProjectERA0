/*
**  @file $RCSfile: zone_gm.c,v $
**  general description of this module
**  $Id: zone_gm.c,v 1.739 2014/07/14 02:02:51 lzk Exp $
**  @author $Author: lzk $
**  @date $Date: 2014/07/14 02:02:51 $
**  @version $Revision: 1.739 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/
#include "zone_gm.h"
#include "zone_err.h"
#include "zone_clt.h"
#include "zone_attr.h"
#include "zone_skill.h"
#include "zone_combat.h"
#include "zone_player.h"
#include "zone_status.h"
#include "zone_package.h"
#include "zone_script.h"
#include "zone_act_status.h"
#include "zone_map.h"
#include "zone_npc.h"
#include "zone_move.h"
#include "mapmask.h"
#include "zone_tmp.h"
#include "zone_relation.h"
#include "zone_task.h"
#include "zone_pet.h"
#include "zone_clan.h"
#include "zone_clan_city.h"
#include "zone_clan_advance.h"
#include "zone_clan_construction.h"
#include "zone_mail.h"
#include "zone_player.h"
#include "zone_designation.h"
#include "zone_respact.h"
#include "zone_oplog.h"
#include "zone_fairy.h"
#include "zone_ai.h"
#include "zone_range.h"
#include "zone_secondaryskill.h"
#include "zone_machine.h"
#include "zone_arm.h"
#include "zone_battle.h"
#include "zone_val.h"
#include "zone_stall.h"
#include "zone_autoteam.h"
#include "zone_diffpworld.h"
#include "zone_misc.h"
#include "zone_city.h"
#include "zone_arm.h"
#include "zone_rune.h"
#include "zone_unreal.h"
#include "zone_spansvr.h"
#include "zone_shop.h"
#include "zone_strongpoint.h"
#include "zone_genius.h"
#include "zone_levfeng.h"
#include "zone_weather.h"
#include "zone_sparta.h"
#include "zone_vip.h"
#include "zone_clanboon.h"
#include "version.h"
#include "zone_losttotem.h"
#include "zone_store.h"
#include "zone_misc.h"
#include "zone_attr.h"
#include "zone_syncrelation.h"
#include "zone_clan_pet.h"
#include "zone_dianfeng.h"
#include "zone_ploy.h"
#include "zone_shangjin.h"
#include "zone_itemlimit.h"
#include "zone_span_strong.h"
#include "zone_team.h"
#include "zone_home.h"
#include "zone_shikong.h"

#define GM_MAX_CMD_LEN		64
#define GM_PREFIX		"//"
#define GM_PREFIX_LEN		2
#define GM_SEP_STR		" \t\r\n"

extern ScriptEnv stSEnv;
extern int g_iOptype;
extern int g_iOpsubtype;

typedef int (*PFNGMENTRY)(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

struct tagGMMap
{
	const char* pszCmd;
	PFNGMENTRY pfnEntry;
	int iPerm;
	const char* pszArg;
};

typedef struct tagGMMap			GMMAP;
typedef struct tagGMMap			*LPGMMAP;


int z_gm_delaylogout(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, 
	const char* pszArg);

static GMMAP s_entries[] =
{
	/* command,		function, 		permission needed,	args*/

	/****f* admin/grant
	 * 名称
	 *   grant -- 给某个在线角色加gm 权限
	 * 用法
	 *   grant name perm -- name:角色名; perm:权限， Apprentice|Wizard|Arch
	 * 用例
	 *   grant kent arch
	 * 权限Admin
	 */
	{"grant", 		z_gm_grant_perm, 	UIN_FLAG_GM_ADMIN,			NULL},

	/****f* admin/shutdown
	 * 名称
	 *   shutdown -- 关闭服务器
	 * 用法
	 *   shutdown time -- time:关机延迟时间
	 * 用例
	 *   shutdown 30
	 * 权限Admin
	 */
	{"shutdown", 		z_gm_shutdown, 	UIN_FLAG_GM_ADMIN,			NULL},

	/****f* admin/reloadres
	 * 名称
	 *   reloadres -- 重载资源
	 * 用法
	 *   reloadres -- reloadres
	 * 用例
	 *   reloadres
	 * 权限Admin
	 */
	{"reloadres", 		z_gm_reloadres, 	UIN_FLAG_GM_ADMIN,			NULL},

	/****f* player/addattr
	 * 名称
	 *   addattr -- 增加角色属性值
	 * 用法
	 *   addattr -- addattr hp=val  mp=val  nl=val vim=val spemove=val exp=val money=val: 
	                      nl ( 念力)    vim (精力)     spemove(特殊行动力)  exp(经验值)                     
	 * 用例
	 *   addattr hp=100 lp=200
	 * 权限App
	 */
	{"addattr", 		z_gm_addattr, 	UIN_FLAG_GM_HIGH,			NULL},
		
	/****f* player/updateskill 
	 * 名称
	 *   updateskill -- 更新技能
	 * 用法
	 *   updateskill -- updateskill  skillid level               
	 * 用例
	 *   updateskill 1000 1
	 * 权限App
	 */
	{"updateskill", 		z_gm_updateskill, 	UIN_FLAG_GM_HIGH,			NULL},

	/****f* player/updatediathesis 
	 * 名称
	 *   updatediathesis -- 更新素质
	 * 用法
	 *   updatediathesis -- updatediathesis  diathesisid level               
	 * 用例
	 *   updatediathesis 1 1
	 * 权限App
	 */
	{"updatediathesis", 		z_gm_updatediathesis, 	UIN_FLAG_GM_HIGH,			NULL},

	{"skillatk", 		z_gm_skillatk, 	UIN_FLAG_GM_HIGH,			NULL},	

	/****f* player/applystatus
	 * 名称
	 *   applystatus -- 给对象施加状态
	 * 用法
	 *   applystatus -- applystatus  id statusid level time               
	 * 用例
	 *   applystatus 22 1000 1 10    给id 是22 的对象施加10秒1级中毒状态
	 * 权限App
	 */
	{"applystatus", 		z_gm_applystatus, 	UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW,			NULL},

	/****f* player/applystatus
	 * 名称
	 *   delstatus -- 给对象清除状态
	 * 用法
	 *   delstatus -- delstatus  statusid idx time
	 * 权限App
	 */
	{"delstatus", 		z_gm_delstatus, 	UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW,			NULL},

	/****f* player/additem
	 * 名称
	 *   additem -- 加物品
	 * 用法
	 *   additem -- additem  itemid num [BIND]             
	 * 用例
	 *   additem 10001 1  背包里增加一个id为10001的物品
	 * 权限App
	 */
	{"additem", 		z_gm_additem, 	UIN_FLAG_GM_HIGH,			NULL},
	/****f* player/additems
	 * 名称
	 *   additems -- 批量加物品
	 * 用法
	 *   additems -- additem  itemids num [BIND]             
	 * 用例
	 *   additems 10001,10002 1  背包里各增加一个id为10001和10002的物品
	 * 权限App
	 */
	{"additems", 		z_gm_additems, 	UIN_FLAG_GM_HIGH,			NULL},
	/****f* player/tmppaktime 
	 * 名称
	 *   tmppaktime -- 增加临时背包有效时间
	 * 用法
	 *   tmppaktime -- tmppaktime  time             
	 * 用例
	 *   tmppaktime 300  临时背包有效时间增加300 秒
	 * 权限App
	 */
	{"tmppaktime", 		z_gm_tmppaktime, 	UIN_FLAG_GM_HIGH,			NULL},

	/****f* player/paklarge
	 * 名称
	 *   paklarge -- 增加临时背包格子
	 * 用法
	 *   paklarge -- paklarge listtype  num ; listtype 0 弹药包 1 材料包2 任务包 3杂物包5 仓库            
	 * 用例
	 *   paklarge 0 2  弹药包增加2个格子
	 * 权限App
	 */
	{"paklarge", 		z_gm_paklarge, 	UIN_FLAG_GM_HIGH,			NULL},

	/****f* player/openstash
	 * 名称
	 *   openstash -- 通知客户端打开仓库界面
	 * 用例
	 *   openstash
	 * 权限App
	 */
	{"openstash", 	z_gm_openstash, 	UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW,			NULL},

	/****f* player/loadscript
	 * 名称
	 *   loadscript -- 装载脚本
	 * 用法
	 *   loadscript -- loadscript scriptname
	 * 用例
	 *   loadscript npc005
	 * 权限App
	 */
	 {"loadscript", 	z_gm_loadscript, 	UIN_FLAG_GM_ADMIN,			NULL},

	/****f* player/debugscript
	 * 名称
	 *   debugscript -- 调试脚本
	 * 用法
	 *   debugscript 1|0 -- 显示脚本执行状况
	 * 用例
	 *   debugscript 1  显示
	 *   debugscript 0  不显示
	 * 权限
	 */
	{"debugscript", z_gm_debugscript,	UIN_FLAG_GM_ADMIN,		NULL},
		
	/****f* player/goto
	 * 名称
	 *   goto -- 跳转地图
	 * 用法goto map_id pos_x pos_y
	 *   
	 * 用例
	 *   goto 1002 3567 3568
	  * 权限
	 */
	{"goto", z_gm_goto,	UIN_FLAG_GM_LOW|UIN_FLAG_GM_HIGH|UIN_FLAG_GM_ADMIN,		NULL},

	/****f* monster/monster
	 * 名称
	 *   monster -- 召唤怪物
	 * 用法monster monster_id num
	 *   
	 * 用例
	 *   monster 13223 1
	  * 权限
	 */
	{"monster", z_gm_monster,	UIN_FLAG_GM_HIGH,		NULL},

	/****f* player/near
	 * 名称
	 *   near --  跳到某个角色附近
	 * 用法near player_name
	 *   
	 * 用例
	 *   near kent
	  * 权限
	 */
	{"near", z_gm_near,	UIN_FLAG_GM_LOW|UIN_FLAG_GM_HIGH|UIN_FLAG_GM_ADMIN,		NULL},

	/****f* player/pworld
	 * 名称
	 *   pworld --  创建副本
	 * 用法pworld pworldid level
	 *   
	 * 用例
	 *   pworld 100 0 创建普通副本，  pworld 100 1 创建精英副本
	  * 权限
	 */
	{"pworld", z_gm_pworld,	UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW,		NULL},

	/****f* item/tmpitem
	 * 名称
	 *   tmpitem --  产生临时物品
	 * 用法tmpitem itemid num
	 *   
	 * 用例
	 *   tmpitem 5020000 1 
	  * 权限
	 */
	{"tmpitem", z_gm_tmpitem,	UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW,		NULL},

	/*
	*用法silence jerry 100           jerry 禁言100秒
	*
	*/
	{"silence", z_gm_silence, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},
	
	/*
	 用法 dur 0 1 20    身上第一个格子装备耐久加20
	  用法 dur 1 2 -20    杂物包第二个格子装备耐久减20
	*/
	{"dur", z_gm_dur, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},


	/*
		 用法 friend  jerry              添加jerry为好友 
	*/
	{"friend", z_gm_addfriend, UIN_FLAG_GM_LOW|UIN_FLAG_GM_HIGH|UIN_FLAG_GM_ADMIN, NULL},


	/*
		用法 finitask     ID(任务ID)
	*/

	{"finitask", z_gm_fini_task, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},

	/*
		用法 finitaskall     完成当前没有完成的所有任务
	*/

	{"finitaskall", z_gm_fini_taskall, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},

	/*
		用法 deltaskall     删除玩家身上的所有任务,会踢下线
	*/

	{"deltaskall", z_gm_del_taskall, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},

	/*
		用法 effect  iType(广播类型)    iMemID(内存ID)  p(字符串)
	*/

	{"effect", z_gm_effect, UIN_FLAG_GM_LOW|UIN_FLAG_GM_HIGH, NULL},


	/*
		用法 addpet id level 给自己添加一个指定等级的宠物
	*/
	{"addpet", z_gm_add_pet, UIN_FLAG_GM_HIGH, NULL},

	/*
		用法 petstat   iStat(0出征1见习2后备3死亡4复活,5老死 6 下蛋)    iType(0--仙子 1-侍卫)  iGrid(第几个格子)
	*/
	{"petstat", z_gm_pet_stat, UIN_FLAG_GM_HIGH, NULL},


	/*
		用法 petaddattr  PetType(0 仙子 1侍卫), PetIdx(所在仙子或侍卫栏的位置), AttrID(关键字ID 宠物身上有此ID则升级此关键字
		，无则学习) 
	*/
	{"petaddattr", z_gm_pet_addattr, UIN_FLAG_GM_HIGH, NULL},


	/*
		用法 petaddskill   PetType(0 仙子 1侍卫), PetIdx(所在仙子或侍卫栏的位置), flag(技能0  素质1), 
		SkillID(技能或素质ID), Level (级别)
	*/
	{"petaddskill", z_gm_pet_addskill, UIN_FLAG_GM_HIGH, NULL},


	/*
		用法 petaddexp   PetIdx(宠物栏的位置),exp(获得经验)
	*/
	{"petaddexp", z_gm_pet_addexp, UIN_FLAG_GM_HIGH, NULL},


	/*
		用法 petaddskillpoint   PetType(0 仙子 1侍卫), PetIdx(所在仙子或侍卫栏的位置),addpoint(获得技能点)
	*/
	{"petaddskillpoint", z_gm_pet_addskillpoint, UIN_FLAG_GM_HIGH, NULL},



	/*
		  用法 petaddpower PetIdx(宠物栏的位置),iPower
	*/

	{"petaddpower", z_gm_pet_addpower, UIN_FLAG_GM_HIGH, NULL},


	/*
		用法 petpassive [0|1]
		0: 出征宠物切换主动模式
		1: 出征宠物切换被动模式
	 */ 
	{"petpassive", z_gm_pet_passive, UIN_FLAG_GM_HIGH, NULL},


	/*
		孵化一个宠物蛋
		用法 hatchpet   ListType(哪个背包)  GridIdx(哪个格子)

	*/
	{"hatchpet",z_gm_hatchpet, UIN_FLAG_GM_HIGH, NULL},



	/*
		中央提示消息
		用法msgmiddle "字符串"
		
	*/
	{"msgmiddle",z_gm_msgmiddle, UIN_FLAG_GM_HIGH, NULL},

	/*
		技能冷却归零  
		用法 1、全部归零: skillcool
			     2、某个技能: skillcool   技能ID
		
	*/
	{"skillcool",z_gm_skillcool_clean, UIN_FLAG_GM_LOW|UIN_FLAG_GM_HIGH, NULL},

 
	/* 
	  公会GM指令
	  解散公会 :
		clan <公会ID> del
	  公会加人 :
	  	clan <公会ID> add <角色名>
	  加减公会属性 : 
	  	clan <公会ID> addattr <money|level|buildingvalue|active|skillpoint>=<[+|-]整数>
	  加减公会城市的属性 :
	  	clan <公会ID> addaddr city <level|defence|business|development|technology>=<[+|-]整数>
	  升级公会
	  	clan <公会ID> upgrade
	  升级公会城市
	  	clan <公会ID> upgrade city [公会城市名]
	  切换公会任务组
	  	clan <公会ID> task <公会任务组ID>
	  切换公会城市任务组
	  	clan <公会ID> task city <公会城市任务组ID>
	 */
	{"clan",z_gm_clan, UIN_FLAG_GM_HIGH, NULL},

	/*
	  配置信息GM指令
	  conf set NAME VALUE
	  conf get NAME
	  conf list
	 */
	{"conf", z_gm_conf, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_ADMIN, NULL},

	//设置防沉迷用户
	// aas set  iOnlineTime iOfflineTime 设置用户在线时间离线事件0视为不修改
	// aas print 打印防沉迷用户在线时间 、下线时间
	{"aas", z_gm_aas_cmd, UIN_FLAG_GM_LOW|UIN_FLAG_GM_HIGH, NULL},


	{"delpck", z_gm_del_pck, UIN_FLAG_GM_LOW|UIN_FLAG_GM_HIGH, NULL},

	// 修改角色名
	{"chgrolename", z_gm_chg_rolename, UIN_FLAG_GM_HIGH, NULL},

	// 修改公会名
	{"chgclanname", z_gm_chg_clanname, UIN_FLAG_GM_HIGH, NULL},

	/*
	  共享任务GM指令
	  sharetask XXXX
	  sharetask refuse
	 */
	{"sharetask", z_gm_sharetask, UIN_FLAG_GM_LOW|UIN_FLAG_GM_HIGH, NULL}, 

	/*
	  延迟退出
	  delaylogout 1
	  delaylogout 0
	 */
	{"delaylogout", z_gm_delaylogout, UIN_FLAG_GM_LOW|UIN_FLAG_GM_HIGH, NULL}, 

	/*
	  服务器公告指令
	  bulletin list 显示公告列表
	  bulletin add ActiveTime RollInterval Text ActicveStart增加一条公告
	  bulletin del ID 删除指定ID的公告
	 */
	{"bulletin", z_gm_bulletin, UIN_FLAG_GM_HIGH, NULL},


	/*
	  查看角色信息
	  watch view 显示视野中的角色ID
	  watch id MemID
	  watch name RoleName
	 */
	{"watch", z_gm_watch, UIN_FLAG_GM_LOW|UIN_FLAG_GM_HIGH, NULL},

	
	{"stash", z_gm_stash, UIN_FLAG_GM_HIGH, NULL},

	/*
		测试骑乘过期 endride idx time
	*/
	{"endride", z_gm_endride, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},

	/*
		测试物品过期 enditem iGridIdx iIdx iTime
	*/
	{"enditem", z_gm_enditem, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},

	/*
		增加骑乘栏位addridegrid iNum 
	*/
	{"addridegrid", z_gm_add_ridegrid, UIN_FLAG_GM_HIGH, NULL},
	
	/*
		杀怪任务设置完成montaskfini iTaskID 
	*/
	{"montaskfini", z_gm_mon_task_fini, UIN_FLAG_GM_HIGH, NULL},

	/*
	  称号系统相关指令
	  增加一个新称号:
	  	dsn add <称号ID>
	  列出现有称号:
	  	dsn list
	  使用一个称号
	  	dsn use <称号ID>
	  称号切换冷却时间归零
	  	dsn cool
	  称号删除
	       dsn del <称号ID>
	 */
	{"dsn", z_gm_designation, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},

	/*
	  声望系统相关指令
	  提升某个阵营的声望:
	  	respact add <阵营ID> <声望值>
	  列出声望列表
	  	respact list
	  将某个阵营的声望清0
	  	respact reset <阵营ID>
	 */
	{"respact", z_gm_respact, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},


	/*
		 删除某个正在进行的活动

		 delevent  活动ID 
	 */
	
	{"delevent", z_gm_del_event, UIN_FLAG_GM_HIGH, NULL},
		
	/*
		 打印当前服务器时间

		 time  
	*/
	{"time", z_gm_time, UIN_FLAG_GM_LOW|UIN_FLAG_GM_HIGH|UIN_FLAG_GM_ADMIN, NULL},

	
	/*
		 设置当前服务器时间，会造成踢所有玩家下线，会冷却所有玩家的技能

		 settime  2011-01-26 16:24:00  
		 settime 0 设置回正常时间
	*/
	{"settime", z_gm_settime, UIN_FLAG_GM_ADMIN, NULL},

	/*
		 关闭当前所在的副本

		 closepworld
	*/
	{"closepworld", z_gm_closepworld, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},

	/*
		 重新装载包括地图副本在内的所有资源

		 reloadall
	*/
	{"reloadall", z_gm_reloadall, UIN_FLAG_GM_ADMIN, NULL},

	/*

		丢弃精灵
	*/
	{"dropfairy", z_gm_dropfairy, UIN_FLAG_GM_HIGH, NULL},


	/*

		精灵升级
	*/
	{"upfairy", z_gm_upfairy, UIN_FLAG_GM_HIGH, NULL},
	

	/*
	

		测试任务环
	*/
	{"addtask", z_gm_addtask, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},

	
	{"petaddlove", z_gm_pet_addloveval, UIN_FLAG_GM_HIGH, NULL},
	
	/****f* player/packageclear
	 * 名称
	 *   packageclear -- 清空指定包裹格子内物品
	 * 用法
	 *   packageclear -- packageclear listtype ; listtype 1 材料包2 任务包 3杂物包5 仓库            
	 * 用例
	 *   packageclear 1 材料包清空
	 * 权限UIN_FLAG_GM_HIGH
	 */
	{"packageclear", z_gm_package_clear, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},

	/****f* player/repairall
	 * 名称
	 *   repairall -- 修理身上所有装备
	 * 用法
	 *   repairall        
	 * 用例
	 *  repairall
	 * 权限UIN_FLAG_GM_LOW
	 */
	{"repairall", z_gm_repair_all, UIN_FLAG_GM_LOW|UIN_FLAG_GM_HIGH, NULL},

	{"revive", z_gm_revive, UIN_FLAG_GM_LOW|UIN_FLAG_GM_HIGH, NULL},

	/*
		加天赋点  //addgn 100       加100点天赋
	*/
	{"addgn", z_gm_gn, UIN_FLAG_GM_LOW, NULL},

	/*
		所有能升级技能加一级
	*/
	{"upallskill", z_gm_up_all_skill, UIN_FLAG_GM_LOW|UIN_FLAG_GM_HIGH, NULL},
	{"addpworldres", z_gm_add_pworld_res, UIN_FLAG_GM_LOW|UIN_FLAG_GM_HIGH, NULL},

	/****f* player/setperm
	 * 名称
	 *   setperm -- 设置GM指令权限限制(DB 方式下)
	 * 用法
	 *   setperm   1 限制非GM权限用户不可使用指令  0 取消限制
	 * 用例
	 *  setperm
	 * 权限UIN_FLAG_GM_HIGH
	 */
	{"setperm", z_gm_set_perm_limit, UIN_FLAG_GM_ADMIN, NULL},

	/****f* player/block
	 * 名称
	 *   block -- 冻结在线的角色一段时间并T下线
	 * 用法
	 *   block   szName iSec ,  szName 玩家名字  iSec 多久时间 单位秒
	 * 用例
	 *  block   玩家名字 100
	 * 权限UIN_FLAG_GM_HIGH
	 */
	 {"block", z_gm_block_player, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},

	/*
		用法 addqiyue   qiyue_id
	*/
	 {"addqiyue", z_gm_player_addqiyue, UIN_FLAG_GM_HIGH, NULL},


	/****f* addtmpitem/addtmpitem
	 * 名称
	 *   addtmpitem -- 加临时道具
	 * 用法addtmpitem itemid x y,PatrolID iStatus iStatusLevel iLast
	 *   
	 * 用例
	 *   addtmpitem 1460001 24109 13315  13 0 0 0
	  * 权限
	 */
	{"addpatrol", z_gm_addpatrol,	UIN_FLAG_GM_HIGH,		NULL},

	/****f* careerchg/careerchg
	 * 名称
	 *   careerchg -- 转职
	 * 用法careerchg   newcareer
	 *   
	 * 用例
	 *   careerchg 17
	  * 权限
	 */
	{"careerchg", z_gm_career_chg,	UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW,		NULL},

	/****f* addcollectnum
	 * 名称
	 *   addcollectnum -- 加积累数
	 * 用法addcollectnum   xxx
	 *   
	 * 用例
	 *   addcollectnum 1000
	  * 权限
	 */
	{"addcollectnum", z_gm_add_collect_num,	UIN_FLAG_GM_HIGH,		NULL},
	
	/*
		jerry 测试用
	*/
	{"lvgift", z_gm_lvgift, UIN_FLAG_GM_HIGH, NULL},
	{"onedaygift", z_gm_onedaygift, UIN_FLAG_GM_HIGH, NULL},


	/*
		Bus陷阱,测试set routing用
	 */
	{"bustrap", z_gm_bus_trap, UIN_FLAG_GM_LOW|UIN_FLAG_GM_HIGH|UIN_FLAG_GM_ADMIN, NULL},

	/*
	 	获取当前玩家在哪个服务器
	 */
	{"where", z_gm_where, UIN_FLAG_GM_LOW|UIN_FLAG_GM_HIGH|UIN_FLAG_GM_ADMIN, NULL},

	/*
		马上排名
	 */
	{"range", z_gm_range, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_ADMIN, NULL},

	// 模拟region创建副本
	{"regioncrt", z_gm_region_create_pworld, UIN_FLAG_GM_LOW|UIN_FLAG_GM_HIGH|UIN_FLAG_GM_ADMIN, NULL},

	// 报名副本
	{"joinqueue", z_gm_join_queue, UIN_FLAG_GM_LOW|UIN_FLAG_GM_HIGH|UIN_FLAG_GM_ADMIN, NULL},


	/****f* event/event
	 * 名称
	 *   event -- 游戏事件操作
	 * 用法event list  (显示正在进行中的事件)
	 *   
	 * 用例
	 *   event list 
	  * 权限
	 */
	{"event", z_gm_event, UIN_FLAG_GM_LOW|UIN_FLAG_GM_HIGH|UIN_FLAG_GM_ADMIN, NULL},

	/****f* itemcool/itemcool
	 * 名称
	 *   itemcool -- 物品冷却时间清零
	 * 用法itemcool ItemID  (清楚指定物品的自我冷却时间)  0则所有物品、物品类型冷却时间清楚
	 *   
	 * 用例
	 *   itemcool 5610000 
	  * 权限
	 */
	{"itemcool", z_gm_item_cool, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},

	/****f* secondaryskill/secondaryskill
	 * 名称
	 *   secondaryskill -- 生活技能相关指令
	
	 * 用法  
	      secondaryskill  learn   		iType(技能类型) iLevel(技能等级)   	-- 学习生活技能 
	      secondaryskill  del	    		iType(技能类型) 						-- 删除生活技能
	      secondaryskill  addrecipe   	iType(技能类型) ID(配方ID)   		-- 学习生活配方
	 *   
	 * 用例
	 *   itemcool learn 1 1 
	  * 权限
	 */
	{"secondaryskill", z_gm_secondaryskill, UIN_FLAG_GM_HIGH, NULL},

	/* 给一件装备附魔
	 * 用法
	 *     extend idx id
	 *      idx: 杂物包的格子索引
	 *       id: 附魔卷轴的ID
	 * 用例
	 *     extend 0 30000
	 * 权限
	 */
	{"extend", z_gm_extend, UIN_FLAG_GM_HIGH, NULL},

	{"explimit", z_gm_getexplimit, UIN_FLAG_GM_LOW|UIN_FLAG_GM_HIGH|UIN_FLAG_GM_ADMIN, NULL},
	{"cleanpworldlimit", z_gm_cleanpworldlimit, UIN_FLAG_GM_LOW|UIN_FLAG_GM_HIGH|UIN_FLAG_GM_ADMIN, NULL},

	{"hideon", z_gm_hide_on, UIN_FLAG_GM_LOW|UIN_FLAG_GM_HIGH|UIN_FLAG_GM_ADMIN, NULL},
	{"hideoff", z_gm_hide_off, UIN_FLAG_GM_LOW|UIN_FLAG_GM_HIGH|UIN_FLAG_GM_ADMIN, NULL},
	{"getmem", z_gm_getmem, UIN_FLAG_GM_LOW|UIN_FLAG_GM_HIGH|UIN_FLAG_GM_ADMIN, NULL},
	{"callrole", z_gm_call_role, UIN_FLAG_GM_LOW|UIN_FLAG_GM_HIGH|UIN_FLAG_GM_ADMIN, NULL},
	{"earon", z_gm_ear_on, UIN_FLAG_GM_LOW|UIN_FLAG_GM_HIGH|UIN_FLAG_GM_ADMIN, NULL},
	{"earoff", z_gm_ear_off, UIN_FLAG_GM_LOW|UIN_FLAG_GM_HIGH|UIN_FLAG_GM_ADMIN, NULL},
	{"bct", z_gm_enter_bct, UIN_FLAG_GM_LOW|UIN_FLAG_GM_HIGH|UIN_FLAG_GM_ADMIN, NULL},

	{"itemunvaild", z_gm_set_item_unvaild, UIN_FLAG_GM_LOW|UIN_FLAG_GM_HIGH|UIN_FLAG_GM_ADMIN, NULL},

	/* 给技能镶嵌配件
	 * 用法
	 *     fittinginlay SkillID(自己的技能ID) 	Idx(镶嵌的位置) 	FitID(配件ID) 	FitLevel(配件等级)
	 *     
	 *     
	 * 用例
	 *     fittinginlay 1012 0 10 1
	 * 权限
	 */
	{"fittinginlay", z_gm_fittings_inlay, UIN_FLAG_GM_HIGH, NULL},

	{"ridemachine", z_gm_ride_machine, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},

	{"cleanallitem", z_gm_clean_all_item, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},

	{"inpworld", z_gm_in_pworld, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},	

	/****f* resetcangbaotu/resetcangbaotu
	 * 名称
	 *   resetcangbaotu -- 重置藏宝图每日次数
	 * 用法resetcangbaotu 
	 *   
	 * 用例
	 *   resetcangbaotu 
	  * 权限
	 */
	{"resetcangbaotu", z_gm_reset_cangbaotu, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},	


	/****** player/runscript
	 * 名称
	 *   runscript -- 重新加载并执行脚本 *****慎用需要上下文关联的脚本指令********
	 * 用法runscript name(脚本名无后缀) 脚本中的事件号 脚本环境对象的memid
	 *   
	 * 用例
 	 *   runscript zcqigai 1 3339 --执行脚本zcqigai.mxt的事件1,npcid为3339
	 *   runscript item5300998 1 0 --执行物品5300998的脚本
	  * 权限
	 */
	{"runscript", z_gm_run_script, UIN_FLAG_GM_HIGH, NULL},	


	{"roll", z_gm_roll, UIN_FLAG_GM_HIGH, NULL},	
	{"safelock", z_gm_safelock, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},

	{"itemnum", z_gm_get_item_num, UIN_FLAG_GM_HIGH, NULL},
	{"rolenum", z_gm_rolenum, UIN_FLAG_GM_HIGH, NULL},


	/****f* spook/spook
	 * 名称
	 *   spook -- 幽灵状态
	 * 用法spook 1  进入幽灵状态
	 		 spook 0  退出幽灵状态
	 *   
	 * 用例
	 *   spook  1
	  * 权限
	 */
	 
	{"spook", z_gm_spook, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},

	/****f* monsterai/monsterai
	 * 名称
	 *   monsterai -- 显示怪物ai的当前信息
	 * 用法 monsterai id 
	 *		           id,是怪物的memid
	 *      使用前请使用客户端指令 showid 和 showid close
	 *   
	 * 用例
	 *   monsterai  1
	 * 权限
	 */
	 
	{"monsterai", z_gm_monsterai, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},


	/****f* pkval/pkval
	 * 名称
	 *   pkval -- 显示杀气值
	 * 用法 pkval
	 *		          
	 *  
	 *   
	 * 用例
	 *   pkval 
	 * 权限
	 */
	{"pkval", z_gm_pkval, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},

	/*
		加宿敌 服务器测试用
	*/
	{"addenemy", z_gm_add_enemy, UIN_FLAG_GM_HIGH, NULL},
	{"addenemy1", z_gm_add_enemy_by_num, UIN_FLAG_GM_HIGH, NULL},
	{"finicyc", z_gm_finicyc, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},
	
	/*
		选择确认框,测试用
	*/
	{"choose", z_gm_choose, UIN_FLAG_GM_HIGH, NULL},

	{"chgline", z_gm_chgline, UIN_FLAG_GM_HIGH, NULL},

	/*
		根据npc id显示npc的脚本名,需要客户端的//showid配合使用,测试用
	*/
	{"npcscript", z_gm_npc_script, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},

	{"setsec", z_gm_settime_by_sec, UIN_FLAG_GM_ADMIN, NULL},
	
	{"upcont", z_gm_up_contonline, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},
	{"reloadcont", z_gm_reload_contonline, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},
	{"setconti", z_gm_setconti, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},

	{"pworldagain", z_gm_pworld_again, UIN_FLAG_GM_HIGH, NULL},

	/*战场报名
		battlequeue   battleID
	*/
	{"battlequeue", z_gm_battle_queue, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},

	/*查看坐标点,测试用
		pos
	*/
	{"pos", z_gm_pos, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},


	{"pworldover", z_gm_pworld_over, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},

	/*同张地图跳点,  测试用
		jump x y 
	*/
	{"jump", z_gm_jump, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},

	/* 掉落物品统计
		dropped-item-count [半径]
	不填半径, 默认为24米
	 */
	{"dropped-item-count", z_gm_dropped_item_count, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},

	/* 播放一个指定的动画
		playmovie  文件路径
	 */
	{"playmovie", z_gm_play_movie, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},

	/*
		重置精灵刻印 |无参数
	*/
	{"rekeyin", z_gm_rekeyin, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},

	/*
		召唤小弟  ID:MONSTER_id
	*/
	{"callbub", z_gm_bub, UIN_FLAG_GM_HIGH, NULL},
	/*
		给机械怪加能量  ID,Energy  
	*/
	{"machineenergy", z_gm_add_machine_energy, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},

	{"rideunvaild", z_gm_set_ride_unvaild, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},

	{"accgift", z_gm_accgift, UIN_FLAG_GM_HIGH, NULL},

	{"oneoffexp", z_gm_oneoffexp, UIN_FLAG_GM_HIGH, NULL},

	
	{"propgift", z_gm_propgift, UIN_FLAG_GM_HIGH, NULL},
	

	{"setdesig", z_gm_setdesig, UIN_FLAG_GM_HIGH, NULL},

	{"onlinesec", z_gm_onlinesec, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},
	/*
		设置角色在线时长

		用法:
			setonlinesec 3600
	*/
	{"setonlinesec", z_gm_setonlinesec, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},
	{"nativeskill", z_gm_nativeskill, UIN_FLAG_GM_HIGH, NULL},

	 /*
		功能:强化角色穿在身上的所有装备星级
		基础用法://armstarup
		扩展用法:
			1、所有装备强化最高等级完美星级://armstarup 或 //armstarup 100 或 //armstarup 100 100
			2、指定强化所有装备到某一等级的星级及相应的索引值(0~9): //armstarup 5 9
	*/
	{"armstarup", z_gm_armstarup, UIN_FLAG_GM_HIGH, NULL},

	/*
		获取附近一定距离内的npc,monster,玩家,的memid
	*/
	{"nearmemid", z_gm_nearmemid, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},	
	

	{"valfight", z_gm_valfight, UIN_FLAG_GM_HIGH, NULL},

	// ................robot中使用................
	{"ride", z_gm_ride, UIN_FLAG_GM_HIGH, NULL},
	{"wear", z_gm_wear, UIN_FLAG_GM_HIGH, NULL},
	{"petcamp", z_gm_setpetcamp, UIN_FLAG_GM_HIGH, NULL},
	{"levelup", z_gm_levelup, UIN_FLAG_GM_HIGH, NULL},
	{"stall", z_gm_stall, UIN_FLAG_GM_HIGH, NULL},
	{"ateamapply", z_gm_auto_team_apply, UIN_FLAG_GM_HIGH, NULL},
	{"ateamagree", z_gm_auto_team_agree, UIN_FLAG_GM_HIGH, NULL},
	// ................robot中使用..................

	
	
	/*
		设置公会宣战cd时间

		用法:
			clanenemycd myclanid otherclanid sec

		用例:
			clanenemycd 0 10 60
	*/
	{"clanenemycd", z_gm_clanenemycd, UIN_FLAG_GM_HIGH, NULL},

	
	/*
		公会清空外交关系

		用法:
			clancleardip myclanid

		用例:
			clanenemycd 0
	*/
	{"clancleardip", z_gm_clancleardip, UIN_FLAG_GM_HIGH, NULL},

	
	/*
		打印公会cd列表

		用法:
			clanenemycdlist clanid

		用例:
			clanenemycdlist 0
	*/
	{"clanenemycdlist", z_gm_clanenemycdlist, UIN_FLAG_GM_HIGH, NULL},

	/*
		藏宝图坐标

		用法:
			treasurepos listtype idx

		用例:
			treasurepos 2 0
	*/
	{"treasurepos", z_gm_treasurepos, UIN_FLAG_GM_HIGH, NULL},

	/*
		清除改名计数

		用法:
			clearnametimes

		用例:
			clearnametimes
	*/
	{"clearnametimes", z_gm_clearnametimes, UIN_FLAG_GM_HIGH, NULL},



	{"diffpworld", z_gm_diffpworld, UIN_FLAG_GM_HIGH, NULL},

	{"cdkey", z_gm_cdkey, UIN_FLAG_GM_HIGH, NULL},

	{"moneventclean", z_gm_mon_event_clean, UIN_FLAG_GM_HIGH, NULL},

	{"offlineexp", z_gm_chgofflineexp, UIN_FLAG_GM_HIGH, NULL},
	
	/*
		设置跨服标志位

		用法:
			setbattleflag  0/1  0表示本服自动组队 1表示跨服自动组队
		用例:
			setbattleflag 0
	*/
	{"setbattleflag", z_gm_setbattleflag, UIN_FLAG_GM_HIGH, NULL},

	/*
		自动骑临时道具

		用法:
			ridetmpitem id

		用例:
			ridetmpitem 1460001
	*/
	{"ridetmpitem", z_gm_ridetmpitem, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL}, 


	/*
		撒道具
		// dropitem 500000 100
	*/
	{"dropitem", z_gm_drop_item, UIN_FLAG_GM_HIGH, NULL},

	/*
		撒money
		// dropmoney 50000 10
		50000块分成10堆
	*/
	{"dropmoney", z_gm_drop_money, UIN_FLAG_GM_HIGH, NULL},

	/*
		vip [0|1]
	*/
	{"vip", z_gm_vip, UIN_FLAG_GM_HIGH, NULL},
		
	/*
		客户端截屏

		用法:
			snapshoot name

		用例:
			snapshoot 张三
	*/
	{"snapshoot", z_gm_snapshoot, UIN_FLAG_GM_HIGH, NULL},
		

	/*
		清理世界富矿富油

		用法:
			worldoreclear

		用例:
			worldoreclear
	*/
	{"worldoreclear", z_gm_world_ore_clear, UIN_FLAG_GM_HIGH, NULL},
	
	{"fengyin", z_gm_fengyin, UIN_FLAG_GM_HIGH, NULL},

	{"worldcity", z_gm_world_city, UIN_FLAG_GM_HIGH, NULL},

	{"delrole", z_gm_del_role, UIN_FLAG_GM_HIGH, NULL},
	/*
		客户端截屏

		用法:
			lucky 0/1

		用例:
			lucky 1
	*/
	{"lucky", z_gm_lucky, UIN_FLAG_GM_HIGH, NULL},

	{"cleanbattlelimit", z_gm_cleanbattlelimit, UIN_FLAG_GM_HIGH, NULL},

	{"delskill", z_gm_delskill, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},

	/*
		进入公会城市

		用法:
			enterclancity 公会ID 刺探活动传送点DefID(可选)

		用例:
			enterclancity 1 
			enterclancity 1 1
	*/

	{"enterclancity", z_gm_enter_clan_city, UIN_FLAG_GM_HIGH, NULL},
	{"battlenum", z_gm_battle_num, UIN_FLAG_GM_HIGH, NULL},

	/*
		//forcemsg type iconid ilift interval msg tstart
	*/
	{"forcemsg", z_gm_forcemsg, UIN_FLAG_GM_HIGH, NULL},


	/*
		vip网吧掉率和经验加成

		用法:
			vipbarmul 掉率加成(100为基数) 经验加成(100为基数)

		用例:
			vipbarmul 100 100
	*/
	{"vipbarmul", z_gm_vipbarmul, UIN_FLAG_GM_HIGH, NULL},


	{"skillmisstime", z_gm_skill_misstime, UIN_FLAG_GM_HIGH, NULL},

	{"gem", z_gm_gem, UIN_FLAG_GM_HIGH, NULL},
	{"heti", z_gm_heti, UIN_FLAG_GM_HIGH, NULL},

	
	/*
		符文额外属性

		用法:
			runeattr 符文颜色(1红 2绿 3蓝) 符文属性ID

		用例:
			runeattr 1 1
	*/
	{"runeattr", z_gm_runeattr, UIN_FLAG_GM_HIGH, NULL},

	/*
		公会摸宝次数

		用法:
			clantouch 今日次数 昨日次数

		用例:
			clantouch 1 1
	*/
	{"clantouch", z_gm_clantouch, UIN_FLAG_GM_HIGH, NULL},

	/*
		//enterunreal ilayer
	*/
	{"enterunreal", z_gm_enter_unreal, UIN_FLAG_GM_HIGH, NULL},

	/*
		成长之路条件

		用法:
			cond 条件类型 值1(可选) 值2(可选) 值3(可选)

		用例:
			cond 1 0 0 0
	*/
	{"cond", z_gm_cond, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},		

	/*
		重置已完成的成长之路

		用法:
			achievementreset

		用例:
			achievementreset
	*/
	{"achievementreset", z_gm_achievementreset, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},
		
	/*
		功能:清除每日经验上限的积累值
		用法://cleardayexp
	*/
	{"cleardayexp", z_gm_clear_dayexp, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},

	/*
		功能:清除角色每日标识
		用法://cleardailyflag
	*/
	{"cleardailyflag", z_gm_clear_daily_flag, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},
		
	/*
		功能:获取地图怪物数量
		用法:
			1、获取当前地图上所有怪物数量: //getmonnum 或 //getmonnum 0 0
			2、获取当前地图上指定怪物ID的怪物数量: //getmonnum 0 monid
			3、获取指定地图上指定怪物ID的怪物数量: //getmonnum mapid monid
	*/
	{"getmonnum", z_gm_get_mon_num, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},

	/*
		功能:获取角色所在地图上NPC的数量
		用法:
			1、获取所有怪物数量: //getnpcnum 或 //getnpcnum 0
			2、获取指定NPC ID的NPC数量: //getmonnum npcid
	*/
	{"getnpcnum", z_gm_get_npc_num, UIN_FLAG_GM_HIGH, NULL},

	{"petupskill", z_gm_pet_add_skill, UIN_FLAG_GM_HIGH, NULL},


	/*
		功能:测试各类系统提示显示方式
		基础用法://showtip
		扩展用法:
			1、显示指定类型的系统提示[有效值0~14]: //showtip 2
			2、//showtip 相当于 //showtip 4
	*/
	{"showtip", z_gm_show_sys_tip, UIN_FLAG_GM_HIGH, NULL},


	{"gotopvp", z_gm_goto_pvp_map, UIN_FLAG_GM_HIGH, NULL},
	/*
		功能:增加公会驱逐入侵活动角色积分
		基础用法://addeverydayval 50
		扩展用法:
			1、增加分值 //addeverydayval 2
	*/	
	{"addeverydayval", z_gm_add_everyday_val, UIN_FLAG_GM_HIGH, NULL},

	/*
		重置每日礼包

		用法:
			onedaygiftreset

		用例:
			onedaygiftreset
	*/
	{"onedaygiftreset", z_gm_onedaygiftreset, UIN_FLAG_GM_HIGH, NULL},

	{"follow", z_gm_team_follow, UIN_FLAG_GM_HIGH, NULL}, 

	{"accord", z_gm_accord_item, UIN_FLAG_GM_HIGH, NULL},

	/*
		设置沙滩秒数

		用法:
			beachsec 秒数(10的整倍数,负数查询当前秒数)

		用例:
			beachsec 3600
	*/
	{"beachsec", z_gm_beachsec, UIN_FLAG_GM_HIGH, NULL},
	/*
		输出短信密码

		用法:
			smscode type|0输出密码 1打开ui

		用例:
			smscode 0
	*/
	{"smscode", z_gm_smscode, UIN_FLAG_GM_HIGH, NULL},
	/*
		角色所在公会获得战场奖励

		用法:
			clanbattleaward ClanWarAwardDefID奖励id 是否胜利(可选)

		用例:
			clanbattleaward 2 1
	*/
	{"clanbattleaward", z_gm_clanbattleaward, UIN_FLAG_GM_HIGH, NULL},
	/*
		立即写角色快照

		用法:
			rolesnapshot

		用例:
			rolesnapshot
	*/
	{"rolesnapshot", z_gm_rolesnapshot, UIN_FLAG_GM_HIGH, NULL},
	/*
		装备时通知客户端服务器计算的装备评分

		用法:
			showequipscore 开关(0关闭 1开启)

		用例:
			showequipscore 1
	*/
	{"showequipscore", z_gm_showequipscore, UIN_FLAG_GM_HIGH, NULL},
	/*
		防沉迷提醒标记

		用法:
			aasflag 1

		用例:
			aasflag 1
	*/
	{"aasflag", z_gm_aasflag, UIN_FLAG_GM_HIGH|UIN_FLAG_GM_LOW, NULL},
	{"cleanbct", z_gm_cleanbct, UIN_FLAG_GM_HIGH, NULL},
	{"decbull", z_gm_decbull, UIN_FLAG_GM_HIGH, NULL},
	{"shamdead", z_gm_sham_dead, UIN_FLAG_GM_HIGH, NULL},
	
	
	/*
		波次怪加速

		用法:
			wavespeed 类型(0为波次怪 1为定时怪) 倒计时间

		用例:
			wavespeed 0 0
	*/
	{"wavespeed", z_gm_wavespeed, UIN_FLAG_GM_HIGH, NULL},
	/*
		清除公会枢纽副本一周2次一天一次的限制

		用法:
			clearmachinepworldlimit

		用例:
			clearmachinepworldlimit
	*/
	{"clearmachinepworldlimit", z_gm_clearmachinepworldlimit, UIN_FLAG_GM_HIGH, NULL},
	/*
		波次设定 直接修改当前波为指定波

		用法:
			waveset 类型(0为波次怪 1为定时怪) 第几波(1开始)

		用例:
			waveset 0 20
	*/
	{"waveset", z_gm_waveset, UIN_FLAG_GM_HIGH, NULL},
	/*
		牛仔决赛排名列表
	
		用法:
			cowrangelist 类型(0为当前正式 1为临时排名|比赛未结束前的排名)
	
		用例:
			cowrangelist 0
	*/

	{"cowrangelist", z_gm_cowrangelist, UIN_FLAG_GM_HIGH, NULL},

	/*
		系统交易
	
		用法:
			gmtrade list 
			gmtrade do defid
	              gmtrade cancel type tradeid
		用例:
			gmtrade do 1
			gmtrade list
			gmtrade cancel 0 23344322
	*/
	{"gmtrade", z_gm_trade, UIN_FLAG_GM_HIGH, NULL},	

	{"strongstat", z_gm_get_strong_stat, UIN_FLAG_GM_HIGH, NULL},	
	{"strongenter", z_gm_strong_enter, UIN_FLAG_GM_HIGH, NULL},
	{"strongsale", z_gm_strong_sale, UIN_FLAG_GM_HIGH, NULL},
	{"strongclear", z_gm_strong_clear, UIN_FLAG_GM_HIGH, NULL},
	{"petgenius", z_gm_pet_genius, UIN_FLAG_GM_HIGH, NULL},
	{"strongaddres", z_gm_strong_add_res, UIN_FLAG_GM_HIGH, NULL},
	{"strongclearall", z_gm_strong_clear_all, UIN_FLAG_GM_HIGH, NULL},

	{"strongsetowner", z_gm_strong_set_owner, UIN_FLAG_GM_HIGH, NULL},
	{"dayexpwarning", z_gm_dayexpwarning, UIN_FLAG_GM_HIGH, NULL},
	{"dartreset", z_gm_dart_reset, UIN_FLAG_GM_HIGH, NULL},
	{"showfy", z_gm_show_fy, UIN_FLAG_GM_HIGH, NULL},
	{"geniusclear", z_gm_geniusclear, UIN_FLAG_GM_HIGH, NULL},
	{"setsvrstart", z_gm_setsvrstart, UIN_FLAG_GM_HIGH, NULL},
	{"goldrateclear", z_gm_clear_goldrate, UIN_FLAG_GM_HIGH, NULL},
	{"goldboxinfoclear", z_gm_clear_goldbox_info, UIN_FLAG_GM_HIGH, NULL},
	{"levfengclear", z_gm_clear_levfeng, UIN_FLAG_GM_HIGH, NULL},

	{"delglobalboss", z_gm_delglobalboss, UIN_FLAG_GM_HIGH, NULL},
	{"bossinfo", z_gm_boss_info, UIN_FLAG_GM_HIGH, NULL},
	{"getweather", z_gm_get_weather, UIN_FLAG_GM_HIGH, NULL},
	{"setweather", z_gm_set_weather, UIN_FLAG_GM_HIGH, NULL},
	{"itemonedayclear", z_gm_itemoneday_clear, UIN_FLAG_GM_HIGH, NULL},
	{"effigyworshipclear", z_gm_effigy_worship_clear, UIN_FLAG_GM_HIGH, NULL},

	{"sparta", z_gm_sparta, UIN_FLAG_GM_HIGH, NULL},
	{"geniusup", z_gm_geniusup, UIN_FLAG_GM_HIGH, NULL},
	{"strongopen", z_gm_strong_open, UIN_FLAG_GM_HIGH, NULL},
	{"strongglory", z_gm_strongglory, UIN_FLAG_GM_HIGH, NULL},  
	{"stronggloryclear", z_gm_strongglory_clear, UIN_FLAG_GM_HIGH, NULL}, 
	{"clanbabyclear", z_gm_clan_baby_clear, UIN_FLAG_GM_HIGH, NULL}, 
	{"vipclear", z_gm_vip_clear, UIN_FLAG_GM_HIGH, NULL}, 
	{"armreset", z_gm_armattr_reset, UIN_FLAG_GM_HIGH, NULL},
	{"armactive", z_gm_armattr_active, UIN_FLAG_GM_HIGH, NULL},
	{"machinebreak", z_gm_machine_break, UIN_FLAG_GM_HIGH, NULL},
	{"carresume", z_gm_dart_car_resume, UIN_FLAG_GM_HIGH, NULL},
	{"vipset", z_gm_vip_set, UIN_FLAG_GM_HIGH, NULL},   
	{"armcheck", z_gm_arm_check, UIN_FLAG_GM_HIGH, NULL}, 
	{"setclanonline", z_gm_setclanonlime, UIN_FLAG_GM_HIGH, NULL}, 
	{"setclanassign", z_gm_setclanassign, UIN_FLAG_GM_HIGH, NULL},
	{"clanboonclear", z_gm_clanboonclear, UIN_FLAG_GM_HIGH, NULL},
	{"finitaskcycle", z_gm_fini_taskcycle, UIN_FLAG_GM_HIGH, NULL},  
	{"contcoverclear", z_gm_cont_cover_clear, UIN_FLAG_GM_HIGH, NULL},  
	{"losttotemfull", z_gm_losttotemfull, UIN_FLAG_GM_HIGH, NULL},
	{"storeopen", z_gm_store_open, UIN_FLAG_GM_HIGH, NULL},
	{"storeclose", z_gm_store_close, UIN_FLAG_GM_HIGH, NULL},
	{"storeup", z_gm_store_up, UIN_FLAG_GM_HIGH, NULL},
	{"storeinfo", z_gm_store_info, UIN_FLAG_GM_HIGH, NULL},
	{"storeset", z_gm_store_set, UIN_FLAG_GM_HIGH, NULL},
	{"yanghaibang", z_gm_yanghaibang_set, UIN_FLAG_GM_HIGH, NULL},  
	{"sysmail", z_gm_sys_mail, UIN_FLAG_GM_HIGH, NULL},  
	{"armreset", z_gm_armbasereset, UIN_FLAG_GM_HIGH, NULL},  
	{"armresetsave", z_gm_armbasereset_save, UIN_FLAG_GM_HIGH, NULL}, 
	{"cleardayzhengyi", z_gm_cleardayzhengyi, UIN_FLAG_GM_HIGH, NULL},	
	{"clearrongyan", z_gm_rongyan_clear, UIN_FLAG_GM_HIGH, NULL},	
	{"cleardaylifebrand", z_gm_cleardaylifebrand, UIN_FLAG_GM_HIGH, NULL},	
	{"addgem", z_gm_addgem, UIN_FLAG_GM_HIGH, NULL},	
	{"petaddgenius", z_gm_petaddgenius, UIN_FLAG_GM_HIGH, NULL},
	{"clearpaoshang", z_gm_clear_paoshang, UIN_FLAG_GM_HIGH, NULL},
	{"setpaoshangyinpiao", z_gm_set_paoshang_yinpiao, UIN_FLAG_GM_HIGH, NULL},
	{"addsparta", z_gm_add_sparta, UIN_FLAG_GM_HIGH, NULL},

	/*
		如:     
		contriflagchg  0        表示  公会技能重置时， 取消玩家已返还贡献的标识
		contriflagchg  1        表示  公会技能重置时，加上玩家已返还贡献的标识
		clanmoneyflagchg 0   表示 公会技能重置时，取消公会已返还资金的标识
		clanmoneyflagchg 1   表示 公会技能重置时，加上公会已返还资金的标识
	*/
	{"contriflagchg", z_gm_clan_skill_reset_re_contri_flag_chg, UIN_FLAG_GM_HIGH, NULL},
	{"clanmoneyflagchg", z_gm_clan_skill_reset_re_money_flag_chg, UIN_FLAG_GM_HIGH, NULL},
	{"spartanum", z_gm_sparta_num, UIN_FLAG_GM_HIGH, NULL},
	{"clanpetjy", z_gm_clan_pet_jiyang, UIN_FLAG_GM_HIGH, NULL},
	{"clanpetjyend", z_gm_clan_pet_jiyang_end, UIN_FLAG_GM_HIGH, NULL},
	{"dianfengbaoming", z_gm_dianfeng_baoming, UIN_FLAG_GM_HIGH, NULL},
	{"dianfengpk", z_gm_dianfeng_pk_begin, UIN_FLAG_GM_HIGH, NULL},
	{"dianfengclean", z_gm_dianfeng_clean, UIN_FLAG_GM_HIGH, NULL},
	/*
		dianfengsetnull   1      表示清除末世巅峰中冠军信息
		dianfengsetnull   0      表示清除末世巅峰中雕像信息
	*/
	{"dianfengsetnull", z_gm_dianfeng_clean_chamption_and_effigy, UIN_FLAG_GM_HIGH, NULL},
	/*
		dianfengcleanmy 0     表示清除末世巅峰中自己已经报名的标识
		dianfengcleanmy 1     表示清除末世巅峰中自己已经竞猜的标识
		dianfengcleanmy 2     表示清除末世巅峰中自己已经拿奖励的标识
		dianfengcleanmy 3     表示清除末世巅峰中自己已经传送成功的标识
		dianfengcleanmy 4     表示清除末世巅峰中自己已经至少pk一次的标识
		dianfengcleanmy 5     表示末世巅峰中自己参与活动的时间清零
		dianfengcleanmy 6     表示末世巅峰增加周六19 :10 提示
		dianfengcleanmy 7     表示末世巅峰中增加周六19:20 标识
	*/
	{"dianfengcleanmy", z_gm_dianfeng_clean_myself, UIN_FLAG_GM_HIGH, NULL},
		
	{"petaddimpexp", z_gm_pet_add_impexp, UIN_FLAG_GM_HIGH, NULL},	
	{"dianfengfirst", z_gm_dianfeng_to_chamption, UIN_FLAG_GM_HIGH, NULL},	
	{"matchchgcareer", z_gm_player_match_chg_career, UIN_FLAG_GM_HIGH, NULL},	
	{"petskillup", z_gm_pet_skillup, UIN_FLAG_GM_HIGH, NULL},
	{"tankadd", z_gm_tank_add, UIN_FLAG_GM_HIGH, NULL},
	{"petlearngenius", z_gm_pet_learn_genius, UIN_FLAG_GM_HIGH, NULL},
	{"pkguse", z_gm_package_use, UIN_FLAG_GM_HIGH, NULL},
	{"machinexunluo", z_gm_machine_patrol, UIN_FLAG_GM_HIGH, NULL},
	{"machinechg", z_gm_machine_chg_map, UIN_FLAG_GM_HIGH, NULL},
	{"wedding", z_gm_wedding, UIN_FLAG_GM_HIGH, NULL},
	{"newmachine", z_gm_new_machine, UIN_FLAG_GM_HIGH, NULL},

	//setmzt   8    表示设置自己处于迷之塔的第八层
	{"setmzt", z_gm_set_mzt_layer, UIN_FLAG_GM_HIGH, NULL},
	{"sjadditem", z_gm_sj_additem, UIN_FLAG_GM_HIGH, NULL},
	//setlevelremain 7   表示设置处于当前等级七天了
	{"setlevelremain", z_gm_set_level_remain_tm, UIN_FLAG_GM_HIGH, NULL},
	{"getlevelremain", z_gm_get_level_remain_tm, UIN_FLAG_GM_HIGH, NULL},

	{"makeclear", z_gm_clan_make_clean_by_id, UIN_FLAG_GM_HIGH, NULL},
	//clearzj  0        清零终极熔岩巨兽每天可召唤次数
	//clearzj  1       清除记录的终极熔岩巨兽信息
	{"clearzj", z_gm_clear_zj_rongyan, UIN_FLAG_GM_HIGH, NULL},


	{"spansale", z_gm_span_sale, UIN_FLAG_GM_HIGH, NULL},
	{"spanenter", z_gm_span_enter, UIN_FLAG_GM_HIGH, NULL},
	{"spanclear", z_gm_span_clear, UIN_FLAG_GM_HIGH, NULL},
	{"spantip", z_gm_span_tip, UIN_FLAG_GM_HIGH, NULL},
	{"spanmail", z_gm_span_mail, UIN_FLAG_GM_HIGH, NULL},
	{"bossclear", z_gm_boss_clear, UIN_FLAG_GM_HIGH, NULL},
	{"makeclear1", z_gm_clan_make_clear, UIN_FLAG_GM_HIGH, NULL},
	{"spanocc", z_gm_span_occ, UIN_FLAG_GM_HIGH, NULL},
	{"nameaddattr", z_gm_name_addattr, UIN_FLAG_GM_HIGH, NULL},	
	{"spanaddres", z_gm_span_add_res, UIN_FLAG_GM_HIGH, NULL},	

	{"homenew", z_gm_home_new, UIN_FLAG_GM_HIGH, NULL},
	{"homedel", z_gm_home_del, UIN_FLAG_GM_HIGH, NULL},
	{"homeenter", z_gm_home_enter, UIN_FLAG_GM_HIGH, NULL},

	//getrolevar iIdx iBit     获取角色GlobalVar 某位(单位iBit) 的值，如getrolevar 74  8
	//setrolevar iIdx iBit iVal   设置角色GlobalVar 某位开始(单位iBit) 的值,如setrolevar 74 8 0 设置为0
	{"getrolevar", z_gm_get_role_var, UIN_FLAG_GM_HIGH, NULL},
	{"setrolevar", z_gm_set_role_var, UIN_FLAG_GM_HIGH, NULL},

	{"shikongenter", z_gm_shikong_enter, UIN_FLAG_GM_HIGH, NULL},
	{"getshikongfbnum", z_gm_get_shikong_fb_num, UIN_FLAG_GM_HIGH, NULL},

	// 清除职业转换等待的时间限制
	{"clearchgcareertime", z_gm_clear_career_alter_time, UIN_FLAG_GM_HIGH, NULL},

	// 设置家园菜园等级  如 setsoillv 8  
	{"setsoillv", z_gm_set_home_soil_lv, UIN_FLAG_GM_HIGH, NULL},
	

	

};


typedef struct tagConfEntry
{
	const char * pszName;
	int * const piVal;
}ConfEntry;

#define CONF_BEGIN        static ConfEntry g_astLookupConf[] = {
#define CONF_ENTRY(name) { #name, &g_i##name, },
#define CONF_END 	       { NULL, NULL }, };

CONF_BEGIN
	CONF_ENTRY(MailMaxExistSecs)
	CONF_ENTRY(MailPostingNeedItemID)
	CONF_ENTRY(MailboxMaxWashingNum)
	CONF_ENTRY(MailboxMinWashingNum)
	CONF_ENTRY(ClanDestroyMemMinSecs)
	CONF_ENTRY(ClanDestroyActiveMinSecs)
	CONF_ENTRY(ClanExitCDSecs)
	CONF_ENTRY(ClanApplyMaxSecs)
	CONF_ENTRY(ClanOnlineActiveSecs)
	CONF_ENTRY(ClanMaxApplyNum)
	CONF_ENTRY(ClanSelectTaskGroupCDTime)
	CONF_ENTRY(ClanPayCDTime)
	CONF_ENTRY(ClanAutoRecoverDefaultTaskGroupTime)
	CONF_ENTRY(ClanCityFirstDowngradeWarningTime)
	CONF_ENTRY(ClanCityLastDowngradeWarningTime)
	CONF_ENTRY(VimRecoverInterval)
	CONF_ENTRY(CreatePloyPworldPlayerNum)
	CONF_ENTRY(MapPlayerNormalNum)
	CONF_ENTRY(MapPlayerBusyNum)
CONF_END
//为了z_gm_get_val在source insight中正常解析
void for_z_gm_get_val()
{
}
int z_gm_get_val(const char** ppszCmd, char* szBuff, int iBuff)
{
	//如开始传进来的** ppszCmd = "abc  efg";
	int iLen;
	iLen = strcspn(*ppszCmd, GM_SEP_STR);

	if( iLen >= iBuff || iLen <= 0)
	{
		return -1;
	}

	memcpy(szBuff, *ppszCmd, iLen);
	szBuff[iLen]	=	'\0';		//*szBuff="abc";

	*ppszCmd	=	*ppszCmd + iLen;//**ppszCmd = "  efg"
	
	iLen	=	strspn(*ppszCmd, GM_SEP_STR);//计算字符串前面空格长度
	*ppszCmd	=	*ppszCmd + iLen;//**ppszCmd = "efg"

	return 0;
}

int z_gm_add_collect_num(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	int CollectSkill_SlotCurr = 0;
	char szTmp[32];

	memset(szTmp,0,sizeof(szTmp));
	z_gm_get_val(&pszArg, szTmp, sizeof(szTmp));
	CollectSkill_SlotCurr = atoi(szTmp);

	pstPlayer->stRoleData.MiscInfo.CollectSkillInfo.CollectSkill_SlotCurr = CollectSkill_SlotCurr;
	
	//下发个ACTION
	player_career_chg_action(pstEnv, pstPlayer);

	return 0;
	UNUSED(pstAppCtx);
}
int z_gm_career_chg(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	// 参数1: 转职的职业类型 请参考tagCAREER_TYPE
	// 参数2: 积累数技能槽的上限
	// 参数3: 积累数技能槽的增加方式
	// 参数4: 返回值0为成功,<0为失败 -2表示已经转职过了

	int iCareer = 0;
	//int iCollectSkill_SlotMax = 1000;
	//int iCollectSkill_AddType = 0;
	char szTmp[32];

	memset(szTmp,0,sizeof(szTmp));
	z_gm_get_val(&pszArg, szTmp, sizeof(szTmp));
	iCareer = atoi(szTmp);

	player_career_chg(pstAppCtx, pstEnv, pstPlayer, iCareer,0);

	/*if( pstPlayer->stRoleData.Career > CAREER_CHURCH )
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, "已经转职了");
		return -1;
	}
		
      if( CAREER_WARRIOR_SINGLEHAND != iCareer && CAREER_WARRIOR_DOUBLEHAND != iCareer &&
   	   CAREER_MAGICOR_SINGLEHAND != iCareer && CAREER_MAGICOR_DOUBLEHAND != iCareer &&
   	   CAREER_GUNER_SINGLEHAND     != iCareer && CAREER_GUNER_DOUBLEHAND     != iCareer &&
   	   CAREER_CHURCH_SINGLEHAND   != iCareer && CAREER_CHURCH_DOUBLEHAND   != iCareer )
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, "参数错误");
		return -1;
	}
  
	if( pstPlayer->stRoleData.Career == CAREER_WARRIOR && 
		CAREER_WARRIOR_SINGLEHAND != iCareer && 
		CAREER_WARRIOR_DOUBLEHAND != iCareer  )
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, "参数错误");
		return -1;
	}
	if( pstPlayer->stRoleData.Career == CAREER_MAGICOR && 
		CAREER_MAGICOR_SINGLEHAND != iCareer && 
		CAREER_MAGICOR_DOUBLEHAND != iCareer  )
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, "参数错误");
		return -1;
	}
	if( pstPlayer->stRoleData.Career == CAREER_GUNER && 
		CAREER_GUNER_SINGLEHAND != iCareer && 
		CAREER_GUNER_DOUBLEHAND != iCareer )
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, "参数错误");
		return -1;
	}
	if(pstPlayer->stRoleData.Career == CAREER_CHURCH && 
		CAREER_CHURCH_SINGLEHAND != iCareer && 
		CAREER_CHURCH_DOUBLEHAND != iCareer)
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, "参数错误");
		return -1;
	}

	if( iCareer == CAREER_WARRIOR_SINGLEHAND )
	{
		iCollectSkill_AddType = COLLECTSKILL_ADDTYPE_AKEE;
	}
	else if( iCareer == CAREER_WARRIOR_DOUBLEHAND ||
			  iCareer == CAREER_CHURCH_DOUBLEHAND   ||
			  iCareer == CAREER_GUNER_SINGLEHAND	   ||
			  iCareer == CAREER_GUNER_DOUBLEHAND)
	{
		iCollectSkill_AddType = COLLECTSKILL_ADDTYPE_AKER;
	}
	else if( iCareer == CAREER_MAGICOR_DOUBLEHAND ||
			  iCareer == CAREER_MAGICOR_SINGLEHAND ||
			  iCareer == CAREER_CHURCH_SINGLEHAND)
	{
		iCollectSkill_AddType = COLLECTSKILL_ADDTYPE_TIME;
	}
	
	//转职了
	pstPlayer->stRoleData.Career = iCareer;
	pstPlayer->stRoleData.MiscInfo.CollectSkillInfo.CollectSkill_SlotMax = iCollectSkill_SlotMax;
	pstPlayer->stRoleData.MiscInfo.CollectSkillInfo.CollectSkill_AddType = iCollectSkill_AddType;

	relation_dbbuddy_chg(pstEnv, pstPlayer, 0);
	
	player_update_cache_career(pstEnv, pstPlayer);

	//记录一条转职的日志
	z_role_lvup_snapshot_oplog(pstEnv, pstPlayer, pstPlayer->stRoleData.Level, pstPlayer->stRoleData.RoleDetail.Exp,1);

	//下发个ACTION
	player_career_chg_action(pstEnv, pstPlayer);*/
	return 0;
}


int z_gm_addpatrol(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	ITEMDEF *pstItemDef;
	RESPOS stPos;
	PATROLDEF *pstPatrolDef;
	int iPatrolIdx;
	DropItem *pstDropItem;
	ZONEIDX stIdx;
	
	int iItemID, x, y,PatrolID, iStatus,iStatusLevel, iLast;
	char szTmp[32];
	UNUSED(pstAppCtx);

	memset(szTmp,0,sizeof(szTmp));
	z_gm_get_val(&pszArg, szTmp, sizeof(szTmp));
	iItemID = atoi(szTmp);
	
	memset(szTmp,0,sizeof(szTmp));
	z_gm_get_val(&pszArg, szTmp, sizeof(szTmp));
	x = atoi(szTmp);

	memset(szTmp,0,sizeof(szTmp));
	z_gm_get_val(&pszArg, szTmp, sizeof(szTmp));
	y = atoi(szTmp);

	memset(szTmp,0,sizeof(szTmp));
	z_gm_get_val(&pszArg, szTmp, sizeof(szTmp));
	PatrolID = atoi(szTmp);

	memset(szTmp,0,sizeof(szTmp));
	z_gm_get_val(&pszArg, szTmp, sizeof(szTmp));
	iStatus = atoi(szTmp);

	memset(szTmp,0,sizeof(szTmp));
	z_gm_get_val(&pszArg, szTmp, sizeof(szTmp));
	iStatusLevel = atoi(szTmp);

	memset(szTmp,0,sizeof(szTmp));
	z_gm_get_val(&pszArg, szTmp, sizeof(szTmp));
	iLast = atoi(szTmp);

	
	if(PatrolID !=0 )
	{	
		pstPatrolDef = z_find_patroldef(pstEnv, PatrolID, &iPatrolIdx);
		if( pstPatrolDef == NULL )
		{
			return 0;
		}
	}
	pstItemDef = z_find_itemdef( pstEnv, iItemID);
	if( pstItemDef == NULL )
	{
		return 0;
	}
	
	stPos.X = x;
	stPos.Y = y;
	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	if(0 == tmp_item_create(pstEnv, pstPlayer,iItemID,&stIdx, &stPos, pstItemDef->ItemTmp.MachineEnerge, pstItemDef->ItemTmp.RefreshTime, 
							pstItemDef->ItemTmp.StayTime, 0,0,0,&pstDropItem) )
	{
		pstDropItem->stObj.stTmpItem.iPatrolID = PatrolID;
		pstDropItem->stObj.stTmpItem.iStatus = iStatus;
		pstDropItem->stObj.stTmpItem.iStatusLevel = iStatusLevel;
		pstDropItem->stObj.stTmpItem.iLast = iLast;
	}

	if (0 > tmp_item_add(pstEnv, pstPlayer, pstDropItem, pstItemDef))
	{
		return -1;
	}
	
	return 0;
}


int z_gm_del_role(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	int i;
	UinCache *pstUinCache;
	UinCache stUinHashData;
	time_t t = pstEnv->pstAppCtx->stCurr.tv_sec-DEL_ROLE_PROTECT_TM*60;
	
	stUinHashData.iUin = pstPlayer->stRoleData.Uin;
	pstUinCache = (UinCache *)sht_find(pstEnv->pstShtUinCache,&stUinHashData,UinHashCmp,UinHashCode);
	if (!pstUinCache)
	{
		return -1;
	}
	
	for (i=0; i<pstUinCache->ucRoleNum; i++)
	{
		if (pstUinCache->astRoleListInfo[i].DelTime > 0)
		{
			pstUinCache->astRoleListInfo[i].DelTime = t;
		}
	}

	player_kickoff (pstEnv->pstAppCtx, pstEnv, pstPlayer, -1);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_grant_perm(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szName[ROLE_NAME_LEN];
	Player *pstPeer;
	UNUSED(pstPlayer);

	/* name is not find. */
	if( z_gm_get_val(&pszArg, szName, sizeof(szName))<0 )
		return -1;

	pstPeer	=	z_name_player(pstAppCtx, pstEnv, szName);
	if( !pstPeer )
		return -1;

	if (!strcasecmp(pszArg, "Apprentice"))
	{
		pstPeer->stRoleData.Perm |= UIN_FLAG_GM_LOW;
	}
	else if (!strcasecmp(pszArg, "Wizard"))
	{
		pstPeer->stRoleData.Perm |= UIN_FLAG_GM_LOW|UIN_FLAG_GM_HIGH;
	}
	else if (!strcasecmp(pszArg, "Arch"))
	{
		pstPeer->stRoleData.Perm |= UIN_FLAG_GM_LOW|UIN_FLAG_GM_HIGH;
	}
	else
		return -1;

	return 0;
}

int z_gm_shutdown(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szVal[32];
	int iTime;
	UNUSED(pstPlayer);
	
	if( z_gm_get_val(&pszArg, szVal, sizeof(szVal))<0 )
	{
		iTime = 30;
	}
	else
	{
		iTime = atoi(szVal);
	}
	
	pstEnv->cShutdown = 1;
	pstEnv->cClearShm = 1;
	pstEnv->ucShutdownCount = iTime/SHUTDOWN_NOTIFY_TIMER;
	pstEnv->tShutdownStart = pstAppCtx->stCurr.tv_sec;
	tos_send_signal( pstAppCtx->pszPidFile, pstAppCtx->pszApp, SIGUSR1, NULL );
	
	return 0;
}

int redef_all_mon(ZONESVRENV* pstEnv)
{
	int i;
	Monster *pstMon;
	MONSTERDEF * pstMonDef;
	
	for (i=0; i<MAX_MONSTER; i++)
	{
		pstMon = tmempool_get_bypos(pstEnv->pstMonsterPool, i);
		if (!pstMon)
		{
			continue;
		}

		pstMonDef = z_get_mon_def(pstEnv, pstMon);

		if (pstMonDef)
		{
			z_mon_def_data(pstEnv, pstMon, pstMonDef);
		}
	}
	
	return 0;
}

int z_gm_reloadres(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSSYSMSG *pstSysMsg = &pstPkg->Body.SysMsg;
	UNUSED(pstPlayer);
	UNUSED(pszArg);
	
	load_script_dir(pstEnv);
	
	//装载资源
	if (0 > z_load_res(pstAppCtx, pstEnv))
	{
		printf("z_load_res fail\n");
		tlog_error(pstEnv->pstLogCat, 0, 0, "z_load_res fail");
		return -1;
	}

	if (0 > init_item_scr(pstEnv))
	{
		printf("init_item_scr fail\n");
		tlog_error(pstEnv->pstLogCat, 0, 0, "init_item_scr fail");
		return -1;
	}

	if (0 > init_task_scr(pstEnv))
	{
		printf("init_task_scr fail\n");
		tlog_error(pstEnv->pstLogCat, 0, 0, "init_task_scr fail");
		return -1;
	}

	if (0 > z_init_player_scr(pstEnv))
	{
		printf("z_init_player_scr fail\n");
		tlog_error(pstEnv->pstLogCat, 0, 0, "z_init_player_scr fail");
		return -1;
	}

	//重新填充怪物
	redef_all_mon(pstEnv);

	pstSysMsg->Type = SYS_SVR;
	pstSysMsg->Times = 0;
	pstSysMsg->Interval = 0;
	pstSysMsg->LinkLen = 0;
	sprintf(pstSysMsg->Msg,"玩家%s   reloadres ",pstPlayer->stRoleData.RoleName);

	Z_CSHEAD_INIT(&stPkg.Head, SYS_MSG);

	z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkg);

	return 0;
}

int z_reloadall(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{	
	load_script_dir(pstEnv);
	
	//装载资源
	if (0 > z_load_res(pstAppCtx, pstEnv))
	{
		printf("z_load_res fail\n");
		tlog_error(pstEnv->pstLogCat, 0, 0, "z_load_res fail");
		return -1;
	}

	if (0 > init_item_scr(pstEnv))
	{
		printf("init_item_scr fail\n");
		tlog_error(pstEnv->pstLogCat, 0, 0, "init_item_scr fail");
		return -1;
	}

	if (0 > init_task_scr(pstEnv))
	{
		printf("init_task_scr fail\n");
		tlog_error(pstEnv->pstLogCat, 0, 0, "init_task_scr fail");
		return -1;
	}

	if (0 > z_init_player_scr(pstEnv))
	{
		printf("z_init_player_scr fail\n");
		tlog_error(pstEnv->pstLogCat, 0, 0, "z_init_player_scr fail");
		return -1;
	}

	if (0 > z_load_map(pstAppCtx, pstEnv, 1))
	{
		printf("z_load_map fail\n");
		tlog_error(pstEnv->pstLogCat, 0, 0, "z_load_map fail");
		return -1;
	}

	// reload时,拉一下账号奖励信息
	player_acc_giftinfo_req_broadcast(pstEnv);

//	strong_new_banner(pstEnv);
	z_clear_all_event(pstEnv);

	return 0;
}

int z_gm_reloadall(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSSYSMSG *pstSysMsg = &pstPkg->Body.SysMsg;
	UNUSED(pstPlayer);
	UNUSED(pszArg);

	if(0 > z_reloadall(pstAppCtx, pstEnv))
	{
		return -1;
	}
	
	pstSysMsg->Type = SYS_SVR;
	pstSysMsg->Times = 0;
	pstSysMsg->Interval = 0;
	pstSysMsg->LinkLen = 0;
	sprintf(pstSysMsg->Msg,"玩家%s   reloadall ",pstPlayer->stRoleData.RoleName);

	Z_CSHEAD_INIT(&stPkg.Head, SYS_MSG);

	z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkg);

	return 0;
}


int z_gm_revive(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{	
	UNUSED(pszArg);
	if (player_revive_svr(pstAppCtx, pstEnv, pstPlayer) < 0)
	{
		return -1;
	}
	return 0;
}

int z_gm_rolenum(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{	
	z_sys_strf( pstEnv,  pstPlayer, SYS_MIDDLE2, "%d", pstEnv->pstPlayerPool->iUsed);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_chg_rolename(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	CSCHGNAMECLT stChgNameClt ;
	ChgRoleName stChg;
	UNUSED(pstAppCtx);
	stChgNameClt.Flag = CHG_ROLE_NAME;
	
	memset (&stChg, 0, sizeof(stChg));
	
	if(0 > z_gm_get_val(&pszArg, stChgNameClt.NewName, sizeof(stChgNameClt.NewName)))
	{
		return -1;
	}

	STRNCPY(stChg.szName, stChgNameClt.NewName, sizeof(stChg.szName));
	STRNCPY(stChg.szOldName, pstPlayer->stRoleData.RoleName, sizeof(stChg.szName));

	//player_chg_rolename(pstEnv, pstPlayer, &stChgNameClt);
	chg_name_update_req(pstEnv, pstPlayer->stRoleData.RoleID, pstPlayer->stRoleData.Uin,&stChg);
	return 0;
}


int z_gm_strongglory(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szGn[32];
	int iMap;

	if(0 > z_gm_get_val(&pszArg, szGn, sizeof(szGn)))
	{
		return -1;
	}

	iMap = atoi(szGn);

	strong_glory_open(pstEnv, pstPlayer, iMap);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_gn(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	int iGnNum;
	char szGn[32];
	UNUSED(pstAppCtx);
	
	if(0 > z_gm_get_val(&pszArg, szGn, sizeof(szGn)))
	{
		return -1;
	}

	iGnNum = atoi(szGn);
	if (iGnNum < 0 )
	{
		return -1;
	}

	if (pstPlayer->stRoleData.DiathesisInfo.LearnGeniusPoint+iGnNum > 0xFFFF)
	{
		pstPlayer->stRoleData.DiathesisInfo.LearnGeniusPoint = 0xFFFF;
	}
	else
	{
		pstPlayer->stRoleData.DiathesisInfo.LearnGeniusPoint+=iGnNum;
	}
	
	z_genius_chg( pstEnv, pstPlayer,&pstPlayer->stRoleData.DiathesisInfo);
	return 0;
}


int z_gm_lvgift(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	int iLv;
	char szLv[32];
	UNUSED(pstAppCtx);

	if(0 > z_gm_get_val(&pszArg, szLv, sizeof(szLv)))
	{
		return -1;
	}

	iLv = atoi(szLv);
	if (iLv < 0 )
	{
		return -1;
	}

	player_gift_lv(pstEnv, pstPlayer, iLv);
	return 0;
}

int z_gm_losttotemfull(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	ROLEITEM *pstItem;
	pstItem =  player_find_item_by_totem( pstEnv,pstPlayer);

	pstPlayer->stRoleData.MiscInfo.LostTotemInfo.Num = 0;
	if (!pstItem)
	{
		return -1;
	}

	pstItem->InstVal2 = pstItem->InstVal1;
	pstItem->InstFlag  &= ~INST_ITEM_BIND;

	losttotem_free(pstEnv,pstPlayer);
	

	item_one_grid_update_ex(pstEnv,  pstPlayer, pstItem, LIST_TYPE_SUNDRIES);
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_onedaygift(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	int iLv;
	char szLv[32];
	UNUSED(pstAppCtx);

	if(0 > z_gm_get_val(&pszArg, szLv, sizeof(szLv)))
	{
		return -1;
	}

	iLv = atoi(szLv);
	if (iLv < 0 )
	{
		return -1;
	}

	player_gift_oneday(pstEnv, pstPlayer, iLv);
	return 0;
}


int z_gm_store_open(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	STOREOPEN stOpen;

	if(0 > z_gm_get_val(&pszArg, stOpen.StoreName, sizeof(stOpen.StoreName)))
	{
		return -1;
	}
	
	store_open(pstEnv, pstPlayer, &stOpen);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_store_close(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	/*ZoneStore *pstZoneStore;

	pstZoneStore = store_id_get(pstEnv, pstPlayer->stRoleData.MiscInfo.Store.ID);
	if (!pstZoneStore)
	{
		return -1;
	}

	if (store_db_delete_req(pstEnv, &pstZoneStore->stStore) < 0)
	{
		return -1;
	}
	*/
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstPlayer);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_sys_mail(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	ROLEGRID stGrid[40];
	int iNum = 0;
	int i;
	char stName[32];

	if(0 > z_gm_get_val(&pszArg, stName, sizeof(stName)))
	{
		return -1;
	}
	
	for (i=0; i<40; i++)
	{
		if (item_create(pstEnv->pstAppCtx, pstEnv, &stGrid[iNum], 5000000) == 0)
		{
			stGrid[iNum].GridData.RoleItem.Num = 20;
			iNum ++;
		}
		else
		{
			return -1;
		}
	}

	if (iNum > 0)
	{
		player_sysmail_fast_send(pstEnv,stName, "xxxx",
										"xxxxxxxxxxxxxxxx!",
										&stGrid[0], iNum, 1000,NULL, 0);
	}
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstPlayer);	//add by paraunused.pl
}

int z_gm_store_up(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	STOREUP stUp;

	stUp.GridIdx = 0;
	stUp.Price = 1000;
	
	store_item_up( pstEnv,  pstPlayer, &stUp);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_store_set(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szArgs[32];
	char szBuff[32];
	int iVal;
	ZoneStore *pstZoneStore;

	pstZoneStore = store_player_get( pstEnv, pstPlayer);
	if (!pstZoneStore)
	{
		return -1;
	}
	
	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}

	if (0 == strcasecmp(szArgs, "exchg"))
	{
		if(0 > z_gm_get_val(&pszArg, szBuff, sizeof(szBuff)))
		{
			return -1;
		}

		iVal = atoi(szBuff);
		pstZoneStore->stStore.StoreInfo.Exchg = iVal;
	}
	else if (0 == strcasecmp(szArgs, "tax"))
	{
		if(0 > z_gm_get_val(&pszArg, szBuff, sizeof(szBuff)))
		{
			return -1;
		}

		iVal = atoi(szBuff);
		pstZoneStore->stStore.StoreInfo.Tax = iVal;
	}
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}


int z_gm_home_del(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	HOMEDESTORY stDel;
	char szArgs[32];
	
	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}
	stDel.Id = atoi(szArgs);
	home_destory( pstEnv, pstPlayer, &stDel);
	return 0;
}

int z_gm_home_enter(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	HOMEENTER stEnter;
	char szArgs[32];
	
	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}

	stEnter.Id = atoi(szArgs);
	home_enter(pstEnv, pstPlayer, &stEnter);
	return 0;
}

int z_gm_home_new(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	HOMENEW stNew;

	stNew.New = 0;
	
	home_new( pstEnv, pstPlayer, &stNew);
	return 0;
}


int z_gm_store_info(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	StoreObj *pstObj = pstEnv->pstStoreObj;
	int i;

	z_sys_strf(pstEnv, pstPlayer, SYS_SVR, "Count=%d", pstObj->iStoreNum);

	for (i=0; i<pstObj->iStoreNum; i++)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_SVR, "ID=%d, RoleName=%s,StoreName=%s", 
				pstObj->astZoneStore[i].stStore.StoreID, pstObj->astZoneStore[i].stStore.Name,
				pstObj->astZoneStore[i].stStore.StoreInfo.StoreName);	
	}	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}


int z_gm_add_pworld_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{	
	int iType;
	int iRes;
	char szArgs[32];
	UNUSED(pstAppCtx);

	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}

	iType = atoi(szArgs);

	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}

	iRes = atoi(szArgs);
	

	if (iType == 0)
	{
		pstPlayer->stOnline.stWarOL.iCrystal += iRes;
	}
	else if (iType == 1)
	{
		pstPlayer->stOnline.stWarOL.iOre += iRes;
	}
	else
	{
		return -1;
	}

	//  通知客户端自己资源变化
	z_ploy_info_chg(pstEnv, pstPlayer, SELF_RES_CHG);
	return 0;
}

int z_gm_up_all_skill(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{	
	int i;
	ZoneSkillList *pstZoneSkillList = &pstPlayer->stOnline.stSkillList;
	SKILLDEF *pstSkillDef;
	int iSkillID,iLv;
	UNUSED(pszArg);
	
	for (i=0; i<pstZoneSkillList->nSkillNum; i++)
	{
		iSkillID  	= pstZoneSkillList->astZoneSkills[i].stSkillData.SkillID;
		iLv		= pstZoneSkillList->astZoneSkills[i].stSkillData.SkillLevel;
		
		pstSkillDef = z_find_skill(pstEnv, pstZoneSkillList->astZoneSkills[i].stSkillData.SkillID, iLv+1, NULL);
		if (pstSkillDef)
		{
			skill_update(pstAppCtx, pstEnv, pstPlayer, iSkillID,  iLv+1);
		}
	}
	return 0;
}

int z_gm_chg_clanname(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	int i;
	char szClanID[32];
	int iClanID;
	char szClanName[NAME_LEN];
	ZoneClan *pstClan;
	ClanObj * pstClanObj = pstEnv->pstClanObj;

	if(0 > z_gm_get_val(&pszArg, szClanID, sizeof(szClanID)))
	{
		return -1;
	}

	iClanID = atoi(szClanID);

	if(0 > z_gm_get_val(&pszArg, szClanName, sizeof(szClanName)))
	{
		return -1;
	}

	pstClan = z_id_clan(pstEnv, iClanID);
	if (NULL == pstClan)
	{
		z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, "clan id err");
		return -1;
	}

	for( i = 0; i < pstClanObj->iClanNum; i++ )
	{
		const CLAN * pstClan1 = &pstClanObj->astZoneClan[i].stClan;

		if( strcmp( pstClan1->ClanProf.Name, szClanName ) == 0 )
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER129);
			return -1;
		}
	}

	chg_clan_name(pstEnv, pstClan, szClanName);
	
	return 0;
}

int z_gm_effect(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	int iType;
	int iMemID;
	char szArgs[512];
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst = NULL;
	
	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}

	iType = atoi(szArgs);
	if (iType < 0 )
	{
		return -1;
	}

	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}
	
	iMemID = atoi(szArgs);
	if (iMemID < 0)
	{
		return -1;
	}

	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &stIdx);

	play_effect(pstEnv, pstPlayer, iType, iMemID, szArgs, pstMapInst);
	
	return 0;
}


int z_gm_del_pck(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	int iPck = -1;
	int iGridIdx = -1;
	char szArgs[128];
	int i;
	int iPos;	
	ROLEPACKAGE *pstPackage = &pstPlayer->stRoleData.Package;
	UNUSED(pstAppCtx);

	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}

	iPck = atoi(szArgs);
	if (iPck < 0 )
	{
		return -1;
	}

	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}
	
	iGridIdx = atoi(szArgs);
	if (iGridIdx < 0)
	{
		return -1;
	}

	if (iPck == LIST_TYPE_SUNDRIES)
	{
		ROLEGRID *pstRoleGrid=NULL;
		for (i=0; i<pstPackage->SundriesPackage.Num; i++)
		{
			pstRoleGrid = &pstPackage->SundriesPackage.RoleGrids[i];
			if (pstRoleGrid->GridData.RoleItem.GridIdx == iGridIdx)
			{
				iPos = i;
				break;
			}
			pstRoleGrid = NULL;
		}
		if (!pstRoleGrid)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_GM1);
			return -1;
		}

		package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstRoleGrid->GridData.RoleItem.DefID, 
						pstRoleGrid->GridData.RoleItem.Num, LIST_TYPE_SUNDRIES, &iPos,PACKAGE_DEC_NONE, 0);
		
	}
	else if (iPck == LIST_TYPE_TASK)
	{
		ROLEITEM *pstRoleItem=NULL;
		for (i=0; i<pstPackage->TaskPackage.Num; i++)
		{
			pstRoleItem = &pstPackage->TaskPackage.RoleItems[i];
			if (pstRoleItem->GridIdx == iGridIdx)
			{
				iPos = i;
				break;
			}
			pstRoleItem = NULL;
		}

		if (!pstRoleItem)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_GM1);
			return -1;
		}
		package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstRoleItem->DefID, 
						pstRoleItem->Num, LIST_TYPE_TASK, &iPos,PACKAGE_DEC_NONE, 0);
	}
	else if (iPck == LIST_TYPE_STUFF)
	{
		ROLEITEM *pstRoleItem=NULL;
		for (i=0; i<pstPackage->StuffPackage.Num; i++)
		{
			pstRoleItem = &pstPackage->StuffPackage.RoleItems[i];
			if (pstRoleItem->GridIdx == iGridIdx)
			{
				iPos = i;
				break;
			}
			pstRoleItem = NULL;
		}
		if (!pstRoleItem)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_GM1);
			return -1;
		}

		package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstRoleItem->DefID, 
						pstRoleItem->Num, LIST_TYPE_STUFF, &iPos,PACKAGE_DEC_NONE, 0);
	}
	else
	{
		return -1;
	}

	return 0;
}


int z_gm_msgmiddle(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szArgs[512];

	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSSYSMSG *pstSysMsg = &pstPkg->Body.SysMsg;
	UNUSED(pstAppCtx);UNUSED(pstPlayer);
	
	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}

	pstSysMsg->Type = SYS_MIDDLE;
	pstSysMsg->Times = 0;
	pstSysMsg->Interval = 0;
	pstSysMsg->LinkLen = 0;
	STRNCPY(pstSysMsg->Msg, szArgs, sizeof(szArgs));

	Z_CSHEAD_INIT(&stPkg.Head, SYS_MSG);

	z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkg);

	return 0;
}

int z_gm_battle_queue(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	int iBattleID;
	int iJoinType;
	char szBattleID[32];
	CSJOINQUEUE stJoin;
	CSPWORLDMATCH *pstMatch;
	CSEXITPWORLDQUEUE* pstExit;
	CSPKG stPkg;
	CSPWORLDCLT *pstReq = &stPkg.Body.PworldClt;
	int iCltType = 0;



	pstMatch = &pstReq->CltData.Match;
	pstExit = &pstReq->CltData.ExitPworldQueue;
	
	if(0 > z_gm_get_val(&pszArg, szBattleID, sizeof(szBattleID)))
	{
		return -1;
	}

	iJoinType = TEAM_JOIN_CLAN;
	
	iBattleID = atoi(szBattleID);
	if (iBattleID <=0 )
	{
		iBattleID = -iBattleID;
		iCltType = PWORLD_CLT_EXIT_QUEUE;
	}

	

	if(0 == z_gm_get_val(&pszArg, szBattleID, sizeof(szBattleID)))
	{
		iJoinType =  atoi(szBattleID);
	}

	if (iJoinType == -1)
	{
		TFRAMEHEAD stFrameHead;
		
		pstReq->CltType = PWORLD_CLT_MATCH;
		pstMatch->PworldID = iBattleID;
		Z_CSHEAD_INIT(&stPkg.Head, PWORLD_CLT);
		return pworld_clt(pstEnv->pstAppCtx,pstEnv,pstPlayer,&stFrameHead,&stPkg);
	}
	else
	{
		if (iCltType == PWORLD_CLT_EXIT_QUEUE)
		{

			TFRAMEHEAD stFrameHead;
		
			pstReq->CltType = iCltType;
			pstExit->PworldID = iBattleID;
			pstExit->TeamJoin = iJoinType;
			
			Z_CSHEAD_INIT(&stPkg.Head, PWORLD_CLT);
			return pworld_clt(pstEnv->pstAppCtx,pstEnv,pstPlayer,&stFrameHead,&stPkg);

		}
		else
		{
			stJoin.BattleID = iBattleID;
			stJoin.TeamJoin = iJoinType;
		
			return z_player_normal_battle_queue(pstEnv, pstPlayer, &stJoin);
		}
	}

	//return 0;
	

	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstPlayer);	//add by paraunused.pl
}

int z_gm_dropfairy(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	int iGridIdx;
	char szArgs[64];
	int i;
	FAIRYPACKAGE *FairyPackage = &pstPlayer->stRoleData.Package.FairyPackage;
	ROLEFAIRY *pstRoleFairy;
	UNUSED(pstAppCtx);
	
	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}
	iGridIdx = atoi(szArgs);

	if (iGridIdx == -1)
	{
		for (i=FairyPackage->FairyNum-1; i>=0; i--)
		{
			pstRoleFairy = &FairyPackage->RoleFairys[i];
			set_fairy_drop( pstEnv, pstPlayer, pstRoleFairy);	
		}

		FairyPackage->FairyNum = 0;
	}
	else
	{
		for (i=FairyPackage->FairyNum -1; i>=0; i--)
		{
			pstRoleFairy = &FairyPackage->RoleFairys[i];
			if (pstRoleFairy->GridIdx == iGridIdx)
			{
				set_fairy_drop( pstEnv, pstPlayer, pstRoleFairy);	
				FairyPackage->FairyNum --;
				if (i != FairyPackage->FairyNum)
				{
					memcpy(pstRoleFairy, &FairyPackage->RoleFairys[FairyPackage->FairyNum],sizeof(*pstRoleFairy));
				}
				break;
			}
		}
	}
	return 0;
}

int z_gm_enter_unreal(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	int iLayer;
	char szLayer[32];

	if(0 > z_gm_get_val(&pszArg, szLayer, sizeof(szLayer)))
	{
		return -1;
	}
	iLayer = atoi(szLayer);
	if (iLayer <= 0)
		return -1;

	unreal_enter(pstEnv, pstPlayer,iLayer,UNREAL_ENTER_GM);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}


int z_gm_pet_learn_genius(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szIdx[32];
	int iIdx;
	CSPETGENIUSLEARN stLearn;

	if(0 > z_gm_get_val(&pszArg, szIdx, sizeof(szIdx)))
	{
		return -1;
	}
	iIdx = atoi(szIdx);

	PETPACKAGE *pstPetPackage = &pstPlayer->stRoleData.Package.PetPackage;

	if(iIdx < 0 || iIdx >= pstPetPackage->Num)
	{
		return -1;
	}

	ROLEPET *pstRolePet = &pstPetPackage->RolePets[iIdx];
	stLearn.GridIdx = 0;
	stLearn.PetWid = pstRolePet->WID;
	pet_genius_learn( pstEnv, pstPlayer, &stLearn);
	return 0;
}

int z_gm_upfairy(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	int iFairyID;
	char szArgs[64];
	FAIRYPACKAGE *FairyPackage = &pstPlayer->stRoleData.Package.FairyPackage;
	ROLEFAIRY *pstRoleFairy;
	int i;
	FAIRYDEF *pstFairyDef = NULL;
	
	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}
	iFairyID = atoi(szArgs);
	if (iFairyID <= 0)
		return -1;

	for (i=0; i<FairyPackage->FairyNum; i++)
	{
		pstRoleFairy = &FairyPackage->RoleFairys[i];
		if (pstRoleFairy->FairyID == iFairyID)
		{
			if (pstRoleFairy->Stat & FAIRY_STAT_EVOLVE)
			{
				z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "当前状态的精灵不能升级!");
				return -1;
			}

			pstFairyDef = faity_find(pstEnv, pstRoleFairy->FairyID, pstRoleFairy->FairyLevel +1);
			if (!pstFairyDef)
				return -1;

			if (pstPlayer->stRoleData.Level < pstFairyDef->EvolveLevelLimit)
			{
				z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "等级太低!");
				return -1;
			}

			fairy_level_up(pstEnv, pstPlayer, pstRoleFairy);
			fairy_chg_to_clt(pstEnv, pstPlayer,pstRoleFairy);
			break;
		}
	}
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_addtask(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szArgs[64];
	int iTaskID;
	UNUSED(pstAppCtx);
	
	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}
	
	iTaskID = atoi(szArgs);
	if (iTaskID <= 0)
	{
		return -1;
	}

	if (gm_task_add(pstEnv, pstPlayer,iTaskID) < 0)
	{
		return -1;
	}
	

	return 0;
}

int z_gm_fengyin(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szArgs[64];
	int iListType;
	int iGirdIdx;
	UNUSED(pstAppCtx);
	
	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}
	
	iListType = atoi(szArgs);

	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}
	
	iGirdIdx = atoi(szArgs);
	

	arm_fengyin_in(pstEnv, pstPlayer,iGirdIdx,NULL,iListType);
	

	return 0;
}

int z_gm_show_fy(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szArgs[64];
	int iListType;
	int iGirdIdx;
	UNUSED(pstAppCtx);
	int i;
	int iPos;
	ROLEPACKAGE *pstPkg = &pstPlayer->stRoleData.Package;
	ROLEWEAR *pstWear = &pstPlayer->stRoleData.Wear;
	ROLEARM *pstArm;
	ROLEGRID *pstRoleGird;
	
	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}
	
	iListType = atoi(szArgs);

	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}
	
	iGirdIdx = atoi(szArgs);

	if (iListType == LIST_TYPE_WEAR)
	{
		for(i=0; i<pstWear->ArmNum; i++)
		{
			if (pstWear->RoleArms[i].GridIdx == iGirdIdx)
			{
				pstArm = &pstWear->RoleArms[i];
				break;
			}
		}
		
		if (i>=pstWear->ArmNum)
		{
			return -1;
		}
	}
	else if(iListType == LIST_TYPE_SUNDRIES)
	{
		iPos = package_grid_pos(pstPkg->SundriesPackage.RoleGrids, pstPkg->SundriesPackage.Num, iGirdIdx);
		if (0 > iPos)
		{
			return -1;
		}
		pstRoleGird = &pstPlayer->stRoleData.Package.SundriesPackage.RoleGrids[iPos];
		if (pstRoleGird->Type == GRID_ITEM_ITEM)
			return -1;
		pstArm = &pstRoleGird->GridData.RoleArm;
	}
	else
	{
		return -1;
	}

	if(!pstArm)
	{
		return -1;
	}

	z_sys_strf( pstEnv,  pstPlayer, SYS_MIDDLE2, "%d", pstArm->FengyinFlag);
	return 0;
}

int z_gm_endride(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{ 	
	ROLERIDELIST *pstRideList = &pstPlayer->stRoleData.MiscInfo.RideList;
	
	char szArgs[32];
	int iRideIdx;
	int iTm;
	UNUSED(pstAppCtx);UNUSED(pstEnv);

	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}
	iRideIdx = atoi(szArgs);
	if (iRideIdx < 0 || iRideIdx >= pstRideList->Num)
	{
		return -1;
	}

	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}
	iTm = atoi(szArgs);
	if (iTm <= 0)
	{
		return -1;
	}

	pstRideList->RideGrids[iRideIdx].Endtime = pstAppCtx->stCurr.tv_sec+iTm;
	
	return 0;
}

int z_gm_enditem(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;
	char szArgs[32];
	int iGrididx;
	int iIdx;
	int iTm;
	int iPos;
	ROLEITEM *pstRoleItem;
	UNUSED(pstAppCtx);UNUSED(pstEnv);

	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}
	// enum tagLIST_TYPE 
	
	//
	//    LIST_TYPE_STUFF = 1, 	/* 材料包 */
	//    LIST_TYPE_TASK = 2, 	/* 任务包 */
	//    LIST_TYPE_SUNDRIES = 3, 	/* 杂物包 */
	//}
	
	iGrididx = atoi(szArgs);
	if (iGrididx <0 )
	{
		return -1;
	}
	
	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}
	iIdx = atoi(szArgs);
	if (iIdx < 0)
	{
		return -1;
	}

	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}
	iTm = atoi(szArgs);
	if (iTm <= 0)
	{
		return -1;
	}

	if (iGrididx == LIST_TYPE_STUFF)
	{
		iPos = package_itemgrid_pos(pstPak->StuffPackage.RoleItems, pstPak->StuffPackage.Num, iIdx);
		if (0 > iPos)
		{
			return -1;
		}

		pstRoleItem = &pstPak->StuffPackage.RoleItems[iPos];
	}
	else if (iGrididx == LIST_TYPE_SUNDRIES)
	{
		iPos = package_grid_pos(pstPak->SundriesPackage.RoleGrids, pstPak->SundriesPackage.Num,iIdx);
		if (0 > iPos)
		{
			return -1;
		}

		pstRoleItem = &pstPak->SundriesPackage.RoleGrids[iPos].GridData.RoleItem;
	}
	else if(iGrididx == LIST_TYPE_TASK)
	{
		iPos = package_itemgrid_pos(pstPak->TaskPackage.RoleItems, pstPak->TaskPackage.Num, iIdx);
		if (0 > iPos)
		{
			return -1;
		}

		pstRoleItem = &pstPak->TaskPackage.RoleItems[iPos];
	}
	else
	{
		return -1;
	}

	pstRoleItem->ValidEnd = pstAppCtx->stCurr.tv_sec+iTm;
	//pstRideList->RideGrids[iRideIdx].Endtime = pstAppCtx->stCurr.tv_sec+iTm;
	
	return 0;
}
/*
static int z_set_pet_revive(ZONESVRENV* pstEnv, Player* pstPlayer,ROLEPET* pstRolePet)
{
	CSPKG stPkg;
	CSPETSVR *pstPetSvr = &stPkg.Body.PetSvr;
	CSPETCHGDATA *pstPetChgDataList = &pstPetSvr->PetChgDataList[0];

	if (!(pstRolePet->StatFlag & PET_STAT_DEAD))
	{
		return -1;
	}
	
	pstRolePet->Age = 0;
	pstRolePet->StatFlag = 0;

	pstPetSvr->Num = 1;
	pstPetChgDataList->Wid = pstRolePet->WID;
	pstPetChgDataList->AttrNum = 0;

	pstPetChgDataList->AttrChgList[pstPetChgDataList->AttrNum].Type = PET_CHG_AGE;
	pstPetChgDataList->AttrChgList[pstPetChgDataList->AttrNum].AttrValue.ChgAge = pstRolePet->Age;
	pstPetChgDataList->AttrNum ++;

	pstPetChgDataList->AttrChgList[pstPetChgDataList->AttrNum].Type = PET_CHG_STAT;
	pstPetChgDataList->AttrChgList[pstPetChgDataList->AttrNum].AttrValue.ChgStatFlag = pstRolePet->StatFlag;
	pstPetChgDataList->AttrNum ++;

	Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	pet_update_online(pstEnv, pstPlayer, pstRolePet);

	return 0;
}*/

int forced_add_pet_lvup(ZONESVRENV *pstEnv,Player *pstPlayer, ROLEPET *pstRolePet)
{
	int iSkillGridType;
	PETDEF *pstPetDef;
	PETLEVELDEF *pstPetLevelDef;

	pstPetDef = z_find_pet_def(pstEnv, pstRolePet->PetID, &pstRolePet->PetDefIdx);
	if (!pstPetDef)
	{
		return -1;
	}

	if (pstRolePet->Level>= MAX_PET_LEVEL)
	{
		return -1;
	}
	
	pstPetLevelDef = z_find_pet_level_def(pstEnv, pstRolePet->Level+1, NULL);
	if (!pstPetLevelDef)
	{
		return -1;
	}

	pstRolePet->Exp = 0;
	pstRolePet->Level++;	

	//随即获得技能格子
	if (pstPetLevelDef->AddSkillGrid && 
		pstRolePet->PetSkills.SkillGridNum < MAX_PET_SKILL_GRID)
	{
		iSkillGridType = get_ran_skillgrid_type(pstRolePet);
		if (iSkillGridType)
		{
			pstRolePet->PetSkills.SkillGrid[pstRolePet->PetSkills.SkillGridNum].GridType = iSkillGridType;
			pstRolePet->PetSkills.SkillGrid[pstRolePet->PetSkills.SkillGridNum].SkillID = 0;
			pstRolePet->PetSkills.SkillGrid[pstRolePet->PetSkills.SkillGridNum].SkillLevel = 0;

			pstRolePet->PetSkills.SkillGridNum++;
		}
	}

	pet_update_online(pstEnv, pstPlayer,  pstRolePet);
	pstRolePet->HP = pstRolePet->MaxHP;
	pet_hp_action(pstEnv, pstPlayer, pstRolePet, pstRolePet->HP);

	pet_update_to_clt(pstEnv, pstPlayer, pstRolePet);

	//特殊任务
	task_pet_trace(pstEnv, pstPlayer, pstRolePet, TASK_OBJ_PET);

	//成长之路
	{
		ACHIEVEMENTCOND stCond;

		memset(&stCond,0,sizeof(stCond));
		
		stCond.Type = COND_PET_LEVEL;
		stCond.Val1 = pstRolePet->Level;

		z_player_achievement_cond(pstEnv,pstPlayer,&stCond);
	}

	return 0;
}

int z_gm_add_pet(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer,
                 const char* pszArg)
{
	char szArgs[32];
	int iDefID = 0, iLevel = 0;
	PETDEF *pstDef = NULL;
	PETLEVELDEF *pstLvDef = NULL;
	ROLEPET *pstPet = NULL;

	UNUSED(pstAppCtx);

	if(z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)) < 0)
	{
		return -1;
	}
	iDefID = atoi(szArgs);
	pstDef = z_find_pet_def(pstEnv, iDefID, NULL);
	if(!pstDef)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_TIP, "你指定的宠物(%d)不存在", iDefID);
		return -1;
	}

	if(z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)) < 0)
	{
		return -1;
	}
	iLevel = atoi(szArgs);
	pstLvDef = z_find_pet_level_def(pstEnv, iLevel, NULL);
	if(!pstLvDef)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_TIP, "你指定的宠物等级(%d)不可达",
		           iLevel);
		return -1;
	}

	if(pstPlayer->stRoleData.Package.PetPackage.Num >= 
	   pstPlayer->stRoleData.Package.PetPackage.MaxNum)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_TIP, "宠物数量已达上限,无法再增加",
		           iLevel);
		return -1;
	}

	pstPet = pet_hatch(pstEnv, pstPlayer, iDefID, 1);
	if(!pstPet)
	{
		return -1;
	}

	if(iLevel <= 1)
	{
		return 0;
	}
	while(--iLevel)
	{
		forced_add_pet_lvup(pstEnv, pstPlayer, pstPet);
	}

	range_pet_set(pstEnv, pstPlayer, pstPet, pstDef, RANGE_MAIN_PET_SUB_FIGHT,
					pet_fight_val(pstEnv, pstPet, pstDef));
	
	return 0;
}

int z_gm_pet_stat(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	UNUSED(pstAppCtx);UNUSED(pstEnv);UNUSED(pstPlayer);UNUSED(pszArg);
	/*int iStat;
	int iType;
	int iGrid;
	char szArgs[128];
	ROLEPET *pstRolePet;
	PETDEF *pstPetDef;
	CSPKG stPkg;
	CSPETSVR *pstPetSvr = &stPkg.Body.PetSvr;
	CSPETCHGDATA *pstPetChgDataList;

	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}
	
	iStat = atoi(szArgs);
	

	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}

	iType = atoi(szArgs);

	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}

	iGrid = atoi(szArgs);

	if (iType == 0)
	{
		pstRolePet = z_pet_package_xian_byidx(pstPlayer,  iGrid);
		if (!pstRolePet)
		{
			return -1;
		}

		if (iStat == 3)		// 死亡
		{
			return -1;
		}
	}
	else if (iType == 1)
	{
		pstRolePet = z_pet_package_shi_byidx(pstPlayer,  iGrid);
		if (!pstRolePet)
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}

	pstPetDef = pet_find_def(pstEnv, pstRolePet->PetID, &pstRolePet->PetDefIdx);
	if (!pstPetDef)
	{
		return -1;
	}

	switch(iStat)
	{
	case 0:// chuzheng
		pet_set_stat(pstEnv, pstPlayer, pstRolePet, PET_STAT_FIGHT);
		break;
	case 1:// jianxi
		pet_set_stat(pstEnv, pstPlayer, pstRolePet, PET_STAT_SEEFIGHT);
		break;
	case 2:// houbei
		pet_set_stat(pstEnv, pstPlayer, pstRolePet, 0);
		break;
	case 3:// siwang
		pstRolePet->Power = 0;
		pet_set_stat(pstEnv, pstPlayer, pstRolePet, PET_STAT_DEAD);
		
		pstPetChgDataList = &pstPetSvr->PetChgDataList[0];
		pstPetSvr->Num = 1;
		pstPetChgDataList->AttrNum = 0;
		pstPetChgDataList->Wid = pstRolePet->WID;
		pstPetChgDataList->AttrChgList[pstPetChgDataList->AttrNum].Type = PET_CHG_POWER;
		pstPetChgDataList->AttrChgList[pstPetChgDataList->AttrNum].AttrValue.ChgPower.Power = 0;
		pstPetChgDataList->AttrChgList[pstPetChgDataList->AttrNum].AttrValue.ChgPower.XieDaiPos = get_pet_shi_xiedaipos(pstEnv, pstPlayer, pstRolePet);
		pstPetChgDataList->AttrNum ++;
	
		Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
		break;
	case 4:// fuhuo
		z_set_pet_revive(pstEnv,  pstPlayer,pstRolePet);
		break;
	//case 5:// laosi
	//	pstRolePet->Age = pstPetDef->MaxAge*PET_AGE_UNIT;
	//	pet_agechange_to_clt(pstEnv, pstPlayer, pstRolePet);
	//	pet_set_stat(pstEnv, pstPlayer, pstRolePet, PET_STAT_DEAD);
	//	break;
	case 6:
		g_pet_lay_egg(pstEnv, pstPlayer, pstRolePet);
		break;
	default:
		return -1;
		break;
	}
	*/
	return 0;
}

int z_gm_fini_task(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szArgs[32];
	int iTaskID;
	TASKDEF *pstTaskDef;
	TASKRUNONE *pstTaskRunOne;
	UNUSED(pstAppCtx);

	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}

	iTaskID = atoi(szArgs);
	if(iTaskID <= 0)
	{
		return -1;
	}

	pstTaskRunOne = task_run_find(pstPlayer, iTaskID);
	if (NULL == pstTaskRunOne)
	{
		if (gm_task_accept(pstEnv, pstPlayer, iTaskID) < 0)
		{
			return -1;
		}
		
		pstTaskRunOne = task_run_find(pstPlayer, iTaskID);
		if(NULL == pstTaskRunOne)
		{		
			return -1;
		}
	}

	pstTaskDef = task_def_find(pstEnv, pstTaskRunOne->TaskID, &pstTaskRunOne->TaskIdx);
	if (NULL == pstTaskDef)
	{
		return -1;
	}
	
	if ( gm_task_commit(pstEnv, pstPlayer, pstTaskRunOne, pstTaskDef) < 0)
	{
		return -1;
	}

	return 0;
}

int z_gm_fini_taskall(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	int iTaskID;
	TASKDEF *pstTaskDef;
	TASKRUNONE *pstTaskRunOne;
	int i;
	TASKRUN *pstTaskRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRun;
	UNUSED(pstAppCtx);

	for (i=0; i<MAX_TASK_RUN; i++)
	{
		if (pstTaskRun->TaskRuns[i].TaskID != 0)
		{
			pstTaskRunOne =  &pstTaskRun->TaskRuns[i];	
			iTaskID = pstTaskRunOne->TaskID;	

			pstTaskDef = task_def_find(pstEnv, pstTaskRunOne->TaskID, &pstTaskRunOne->TaskIdx);
			if (NULL == pstTaskDef)
			{
				continue;
			}
			
			if ( gm_task_commit(pstEnv, pstPlayer, pstTaskRunOne, pstTaskDef) < 0)
			{
				continue;
			}
		}
	}

	return 0;
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_forcemsg(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	#if 0
		   int Type;                            	/*   0-强制消息框 1-强制活动 */
		    int IconID;                          	/*   0-没有图标  |非0图标ID */
		    char Msg[SS_MSG_LEN];         /*   Type为0时此字段为消息内容 |1时为URL */
		    char TStart[SS_TIME_LEN];     /*   开始时间|格式为2012-12-12 18:30:30 */
		    int TLife;                           	/*    单位:秒持续时间 */
		    int Interval;                        	/*   单位:秒|间隔-多久弹出一次 */
	#endif
	FORCEMSGREQ stReq;
	char szBuf[64];

	if(0 > z_gm_get_val(&pszArg, szBuf, sizeof(szBuf)))
	{
		return -1;
	}
	stReq.Type = atoi(szBuf);

	if(0 > z_gm_get_val(&pszArg, szBuf, sizeof(szBuf)))
	{
		return -1;
	}
	stReq.IconID = atoi(szBuf);

	if(0 > z_gm_get_val(&pszArg, szBuf, sizeof(szBuf)))
	{
		return -1;
	}
	stReq.TLife = atoi(szBuf);

	if(0 > z_gm_get_val(&pszArg, szBuf, sizeof(szBuf)))
	{
		return -1;
	}
	stReq.Interval = atoi(szBuf);

	if(0 > z_gm_get_val(&pszArg, stReq.Msg, sizeof(stReq.Msg)))
	{
		return -1;
	}

	strncpy(stReq.TStart,pszArg,sizeof(stReq.TStart));
	
	z_ss_force_msg_req(pstEnv, &stReq);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstPlayer);	//add by paraunused.pl
}

int z_gm_gem(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szOp[32];

	if(0 > z_gm_get_val(&pszArg, szOp, sizeof(szOp)))
	{
		return -1;
	}

	if (strcmp(szOp, "up") == 0)
	{
		GEMLEVELUPDATA stGemLevelUp;
		
		stGemLevelUp.GemNum = 3;
		stGemLevelUp.GemData[0].ItemID =  5210000;
		stGemLevelUp.GemData[0].ListType = 3;
		stGemLevelUp.GemData[0].GridIdx = 0;
		stGemLevelUp.GemData[1].ItemID =  5210000;
		stGemLevelUp.GemData[1].ListType = 3;
		stGemLevelUp.GemData[1].GridIdx = 0;
		stGemLevelUp.GemData[2].ItemID =  5210000;
		stGemLevelUp.GemData[2].ListType = 3;
		stGemLevelUp.GemData[2].GridIdx = 0;
		stGemLevelUp.StuffData.ItemID = 5219400;
		stGemLevelUp.StuffData.ListType = 3;
		stGemLevelUp.StuffData.GridIdx = 1;
		gem_level_up(pstEnv, pstPlayer,  &stGemLevelUp);	
	}
	else if (strcmp(szOp, "reset") == 0)
	{
		GEMRESET stGemReSet;

		stGemReSet.ArmData.ArmListType = 3;
		stGemReSet.ArmData.ArmGridIdx = 0;
		stGemReSet.ArmHoleIdx = 3;
		stGemReSet.UseType = 1;
		arm_reset_gem(pstEnv, pstPlayer, &stGemReSet);
	}
	else if (strcmp(szOp, "power") == 0)
	{
		GEMADDPOWER stGemAddPower;
		stGemAddPower.ArmData.ArmListType = 3;
		stGemAddPower.ArmData.ArmGridIdx = 0;
		stGemAddPower.ArmHoleIdx = 0;
		stGemAddPower.StuffNum = 1;
		stGemAddPower.StuffData[0].ItemID = 5210000;
		stGemAddPower.StuffData[0].ItemNum = 99;
		stGemAddPower.StuffData[0].ListType = 3;
		stGemAddPower.StuffData[0].GridIdx = 1;
		gem_add_power(pstEnv, pstPlayer, &stGemAddPower);
	}
	else if (strcmp(szOp, "fj")==0)
	{
		ARMFENJIE stArmFenJie;
		stArmFenJie.Type = 1;
		stArmFenJie.ArmData.ArmIdxData.ArmListType = 3;
		stArmFenJie.ArmData.ArmIdxData.ArmGridIdx = 0;
		arm_fenjie(pstEnv,pstPlayer,&stArmFenJie);
	}

	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_del_taskall(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	TASKDEF *pstTaskDef = NULL;
	TASKRUNONE *pstTaskRunOne = NULL;
	UNUSED(pstAppCtx);
	UNUSED(pstEnv);
	UNUSED(pszArg);

	memset(&pstPlayer->stRoleData.MiscInfo.TaskInfo,0,sizeof(TASKINFO));

	//出生获得任务
	pstTaskDef = task_def_find(pstEnv, BIRTH_PLAYER_TASK, NULL);
	if (pstTaskDef)
	{
		pstTaskRunOne = task_add_taskrun(pstEnv, pstPlayer, pstTaskDef);
		if (pstTaskRunOne)
		{
			pstTaskRunOne->TaskStat = TASK_STAT_FINI;
		}
	}
	
	player_kickoff(pstAppCtx, pstEnv, pstPlayer, ZONEERR_KICK202);
	player_disconnect( pstEnv, pstPlayer );
	
	return 0;
}

int z_gm_pet_passive(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer,
                     const char* pszArg)
{
	char szArg[32];

	UNUSED(pstAppCtx);
	if(z_gm_get_val(&pszArg, szArg, sizeof(szArg)) < 0)
	{
		return -1;
	}

	return 0;
	//return pet_set_passive(pstEnv, pstPlayer, (char)atoi(szArg));
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstPlayer);	//add by paraunused.pl
}


int z_gm_hatchpet(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	UNUSED(pstAppCtx);UNUSED(pstEnv);UNUSED(pstPlayer);UNUSED(pszArg);
	/*char szBuff[32];
	CSPETHATCH stPetHatch;

	if (0 > z_gm_get_val(&pszArg, szBuff, sizeof(szBuff)))
	{
		return -1;
	}

	stPetHatch.SrcListType = atoi(szBuff);

	if (0 > z_gm_get_val(&pszArg, szBuff, sizeof(szBuff)))
	{
		return -1;
	}

	stPetHatch.SrcGridIdx = atoi(szBuff);

	pet_use_hatch(pstEnv, pstPlayer,  &stPetHatch);
	*/
	return 0;
}

int z_gm_addfriend(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szRoleName[32];
	char szMode[32];
	//int iMode; 		// 单项还是双向
	Player * pstOther;
		
	if(0 > z_gm_get_val(&pszArg, szRoleName, sizeof(szRoleName)))
	if(szRoleName[0] == 0)
	{
		return -1;
	}

	if(0 > z_gm_get_val(&pszArg, szMode, sizeof(szMode)))
	{
		return -1;
	}
	//iMode = atoi(szMode);
	//if(iMode < 0)
	//{
	//	return -1;
	//}

	pstOther = z_name_player(pstAppCtx, pstEnv, szRoleName);
	if (pstOther == NULL)
	{
		return -1;
	}

	relation_friend_add(pstEnv, pstPlayer, pstOther);

	//if (iMode != 0)
	{
		relation_friend_add(pstEnv, pstOther, pstPlayer);
	}

	return 0;
}

int z_gm_dur(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{	
	int iPackType;
	int iGrid;
	int iDur;
	char szTmp[32];
	int iIdx;
	ITEMDEF *pItem;
	unsigned int iOldDur;
	unsigned int iOldDurClt;
	ROLEWEAR *pstWear;  
	SUNDRIESPACKAGE *pstSundries;      
	ROLEARM *pstRoleArm;    
	CSROLEARM *pstCSRoleArm;

	CSPKG stPkg;
	CSPACKAGEINFO *pstPackageInfo = &stPkg.Body.PackageInfo;
	ROLEPACKAGECHG *pstPackageChg = &pstPackageInfo->PackageInfoData.PackageChg;
	UNUSED(pstAppCtx);
	
	pstPackageInfo->Type = PACKAGE_INFO_CHG;
	pstPackageChg->ItemChg.ChgNum = 0;
	pstPackageChg->ArmChg.ChgNum = 0;
	
	if(0 > z_gm_get_val(&pszArg, szTmp, sizeof(szTmp)))
	if(szTmp[0] == 0)
	{
		return -1;
	}
	iPackType = atoi(szTmp);

	if(0 > z_gm_get_val(&pszArg, szTmp, sizeof(szTmp)))
	{
		return -1;
	}
	if(szTmp[0] == 0)
	{
		return -1;
	}
	iGrid = atoi(szTmp);

	if(0 > z_gm_get_val(&pszArg, szTmp, sizeof(szTmp)))
	{
		return -1;
	}
	if(szTmp[0] == 0)
	{
		return -1;
	}
	iDur = atoi(szTmp);

	if(iPackType == 0) 		//   身上
	{
		pstWear = &pstPlayer->stRoleData.Wear;
		
		iIdx = wear_arm_pos(pstWear->RoleArms, pstWear->ArmNum, iGrid);
		if(iIdx == -1)
		{
			return -1;
		}

		pItem = z_get_itemdef_roleitem(pstEnv, (ROLEITEM*)&pstWear->RoleArms[iIdx]);
		if(pItem == NULL || pItem->ArmDur.MaxDur == 0)
		{
			return -1;
		}

		iOldDur = pstWear->RoleArms[iIdx].Dur;
		iOldDurClt = iOldDur/DUR_UNIT;
		
		pstWear->RoleArms[iIdx].Dur += iDur;
		
		if((int)pstWear->RoleArms[iIdx].Dur < 0)
		{
			pstWear->RoleArms[iIdx].Dur = 0;
		}
		if((int)pstWear->RoleArms[iIdx].Dur >= z_get_arm_max_dur(pstEnv, pItem)*DUR_UNIT)
		{
			pstWear->RoleArms[iIdx].Dur = z_get_arm_max_dur(pstEnv, pItem)*DUR_UNIT;
		}
		
		if(iOldDurClt !=0 && pstWear->RoleArms[iIdx].Dur/DUR_UNIT == 0)
		{
			z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		}
		if(iOldDurClt ==0 && pstWear->RoleArms[iIdx].Dur/DUR_UNIT != 0)
		{
			z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		}

		if(iOldDurClt != pstWear->RoleArms[iIdx].Dur/DUR_UNIT)
		{
			pstCSRoleArm = pstPackageChg->ArmChg.Arms+pstPackageChg->ArmChg.ChgNum;
			pstCSRoleArm->ListType = LIST_TYPE_WEAR;
			memcpy(&pstCSRoleArm->Arm, &pstWear->RoleArms[iIdx], sizeof(pstCSRoleArm->Arm));
			pstPackageChg->ArmChg.ChgNum++;
		}
	}
	else if(iPackType == 1) 	// 杂物包
	{
		pstSundries = &pstPlayer->stRoleData.Package.SundriesPackage;
		
		iIdx = package_grid_pos(pstSundries->RoleGrids, pstSundries->Num, iGrid);
		if(iIdx == -1)
		{
			return -1;
		}
		
		if(pstSundries->RoleGrids[iIdx].Type == GRID_ITEM_ITEM)
		{
			return -1;
		}
		
		pItem = z_get_itemdef_roleitem(pstEnv,&pstSundries->RoleGrids[iIdx].GridData.RoleItem);
		if(pItem == NULL || pItem->ArmDur.MaxDur == 0)
		{
			return -1;
		}
		
		pstRoleArm = &pstSundries->RoleGrids[iIdx].GridData.RoleArm;
		iOldDurClt = pstRoleArm->Dur/DUR_UNIT;
		
		pstRoleArm->Dur += iDur;
		if((int)pstRoleArm->Dur < 0)
		{
			pstRoleArm->Dur = 0;
		}
		if((int)pstRoleArm->Dur > z_get_arm_max_dur(pstEnv, pItem)*DUR_UNIT)
		{
			pstRoleArm->Dur = z_get_arm_max_dur(pstEnv, pItem)*DUR_UNIT;
		}

		if(iOldDurClt != pstRoleArm->Dur/DUR_UNIT)
		{
			pstCSRoleArm = pstPackageChg->ArmChg.Arms+pstPackageChg->ArmChg.ChgNum;
			pstCSRoleArm->ListType = LIST_TYPE_SUNDRIES;
			memcpy(&pstCSRoleArm->Arm, pstRoleArm, sizeof(pstCSRoleArm->Arm));
			pstPackageChg->ArmChg.ChgNum++;
		}
		
	}
	else
	{
		return -1;
	}

	if(pstPackageChg->ArmChg.ChgNum > 0)
	{
		Z_CSHEAD_INIT(&stPkg.Head, PACKAGE_INFO);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}

	return 0;
}

int z_gm_petaddgenius(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szAttrVal[64];
	char *pszAttr;
	int iVal;
	int iLen;
	char szAttr[32];

	char szIdx[32];
	int iIdx;
	ROLEPET *pstRolePet;
	PETWASHGENIUSDEF *pstPetWashGeniusDef;
	PETDEF *pstPetDef;

	if (z_gm_get_val(&pszArg, szIdx, sizeof(szIdx)) < 0)
	{
		return -1;
	}

	iIdx = atoi(szIdx);
	if (iIdx >= pstPlayer->stRoleData.Package.PetPackage.Num)
	{
		return -1;
	}

	pstRolePet = &pstPlayer->stRoleData.Package.PetPackage.RolePets[iIdx];

	pstPetDef = z_find_pet_def(pstEnv, pstRolePet->PetID, NULL);
	if (!pstPetDef)
	{
		return -1;
	}
	
	pstPetWashGeniusDef = z_find_pet_wash_genius_def(pstEnv, pstRolePet->Imp, pstRolePet->PetGenius);
	if(!pstPetWashGeniusDef)
	{
		return -1;
	}

	
	while(0 == z_gm_get_val(&pszArg, szAttrVal, sizeof(szAttrVal)))
	{
		pszAttr = strchr(szAttrVal, '=');
		if (NULL == pszAttr)
		{
			return -1;
		}

		iLen = pszAttr - szAttrVal;
		memcpy(szAttr, szAttrVal, iLen);
		szAttr[iLen] = 0;
		pszAttr++;
		iVal = atoi(pszAttr);

		if (0 == strcasecmp(szAttr, "hp"))
		{
			pstRolePet->HPGenius+= iVal;
			if(pstRolePet->HPGenius > get_max_pet_wash_genius(pstPetWashGeniusDef, pstPetDef->Genius[PET_GENIUS_S].HPGenius))
			{
				pstRolePet->HPGenius = 
					get_max_pet_wash_genius(pstPetWashGeniusDef, pstPetDef->Genius[PET_GENIUS_S].HPGenius);
			}
		}
		else if (0 == strcasecmp(szAttr, "atk"))
		{
			pstRolePet->PhsicAtkGenius += iVal;
			if(pstRolePet->PhsicAtkGenius > get_max_pet_wash_genius(pstPetWashGeniusDef, pstPetDef->Genius[PET_GENIUS_S].PhsicAtkGenius))
			{
				pstRolePet->PhsicAtkGenius = 
					get_max_pet_wash_genius(pstPetWashGeniusDef, pstPetDef->Genius[PET_GENIUS_S].PhsicAtkGenius);
			}

		}
		else if (0 == strcasecmp(szAttr, "physidef"))
		{
			pstRolePet->DefGenius += iVal;
			if(pstRolePet->DefGenius > get_max_pet_wash_genius(pstPetWashGeniusDef, pstPetDef->Genius[PET_GENIUS_S].DefGenius))
			{
				pstRolePet->DefGenius = 
					get_max_pet_wash_genius(pstPetWashGeniusDef, pstPetDef->Genius[PET_GENIUS_S].DefGenius);
			}
		}
		else if(0 == strcasecmp(szAttr, "hit"))
		{
			pstRolePet->HitRateGenius += iVal;
			if(pstRolePet->HitRateGenius > get_max_pet_wash_genius(pstPetWashGeniusDef, pstPetDef->Genius[PET_GENIUS_S].HitRateGenius))
			{
				pstRolePet->HitRateGenius = 
					get_max_pet_wash_genius(pstPetWashGeniusDef, pstPetDef->Genius[PET_GENIUS_S].HitRateGenius);
			}
		}
		else if (0 == strcasecmp(szAttr, "flee"))
		{
			pstRolePet->FleeGenius += iVal;
			if(pstRolePet->FleeGenius > get_max_pet_wash_genius(pstPetWashGeniusDef, pstPetDef->Genius[PET_GENIUS_S].FleeGenius))
			{
				pstRolePet->FleeGenius = 
					get_max_pet_wash_genius(pstPetWashGeniusDef, pstPetDef->Genius[PET_GENIUS_S].FleeGenius);
			}
		}
		else if(0 == strcasecmp(szAttr, "icedef"))
		{
			pstRolePet->IceDefGenius+= iVal;
			if(pstRolePet->IceDefGenius > get_max_pet_wash_genius(pstPetWashGeniusDef, pstPetDef->Genius[PET_GENIUS_S].IceDefGenius))
			{
				pstRolePet->IceDefGenius = 
					get_max_pet_wash_genius(pstPetWashGeniusDef, pstPetDef->Genius[PET_GENIUS_S].IceDefGenius);
			}
		}
		else
		{
			return -1;
		}
	}

	while(1)
	{
		if(0 > pet_genius_levelup_check(pstEnv,pstRolePet, pstPetDef, pstPetWashGeniusDef))
		{
			break;
		}

		
		if(pstRolePet->PetGenius >= PET_GENIUS_S)
		{
			break;
		}
		
		pstRolePet->PetGenius++;

		pstPetWashGeniusDef = z_find_pet_wash_genius_def(pstEnv, pstRolePet->Imp, pstRolePet->PetGenius);
		if(!pstPetWashGeniusDef)
		{
			break;
		}
	}

	pet_update_online(pstEnv, pstPlayer, pstRolePet);
	pet_update_to_clt(pstEnv, pstPlayer, pstRolePet);
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}


int z_gm_player_match_chg_career(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	MATCHCHGCAREER stChg;
	char szCareer[32];
	
	if(0 > z_gm_get_val(&pszArg, szCareer, sizeof(szCareer)))
	{
		return -1;
	}

	stChg.Career = atoi(szCareer);
	
	stChg.GridIdx = 0;

	player_match_chg_career(pstAppCtx,pstEnv, pstPlayer, &stChg);
	return 0;
}

int z_gm_addattr(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szAttrVal[64];
	char *pszAttr;
	char szAttr[32];
	int  iVal;
	int iLen;
	PlayerChagAttrVal stAttr;
	int iHPMPChg = 0;
	int iOldHP = pstPlayer->stRoleData.RoleDetail.HPCurr;
	int iOldMP = pstPlayer->stRoleData.RoleDetail.MPCurr;
	//int iFairyMaxXP;	//comment by paraunused.pl
	unsigned int uiOldMoney = pstPlayer->stRoleData.RoleDetail.Money;
	int iBindGold = 0;
	int iMoney = 0;
	int iBull = 0;
	int iRet;
		
	stAttr.AttrNum = 0;
	while(0 == z_gm_get_val(&pszArg, szAttrVal, sizeof(szAttrVal)))
	{
		pszAttr = strchr(szAttrVal, '=');
		if (NULL == pszAttr)
		{
			return -1;
		}

		iLen = pszAttr - szAttrVal;
		memcpy(szAttr, szAttrVal, iLen);
		szAttr[iLen] = 0;
		pszAttr++;
		iVal = atoi(pszAttr);

		if (iVal >= 0)
		{
			stAttr.AttrList[stAttr.AttrNum].nFlag = 0;
		}
		else
		{
			stAttr.AttrList[stAttr.AttrNum].nFlag = 1;
			iVal = -iVal;
		}

		if (0 == strcasecmp(szAttr, "hp"))
		{
			stAttr.AttrList[stAttr.AttrNum].AttrID = ATTR_ID_HP;
			stAttr.AttrList[stAttr.AttrNum].Value.HP = iVal;
			stAttr.AttrNum++;

			iHPMPChg = 1;
		}
		else if (0 == strcasecmp(szAttr, "mp"))
		{
			stAttr.AttrList[stAttr.AttrNum].AttrID = ATTR_ID_MP;
			stAttr.AttrList[stAttr.AttrNum].Value.MP = iVal;
			stAttr.AttrNum++;

			iHPMPChg = 1;
		}
		else if (0 == strcasecmp(szAttr, "nl")) // liliang
		{
			stAttr.AttrList[stAttr.AttrNum].AttrID = ATTR_ID_LILIANG;
			stAttr.AttrList[stAttr.AttrNum].Value.Liliang = iVal;
			stAttr.AttrNum++;
		}
		else if (0 == strcasecmp(szAttr, "vim"))
		{
			stAttr.AttrList[stAttr.AttrNum].AttrID = ATTR_ID_VIM;
			stAttr.AttrList[stAttr.AttrNum].Value.Vim= iVal;
			stAttr.AttrNum++;
		}
		else if (0 == strcasecmp(szAttr, "spemove"))
		{
			stAttr.AttrList[stAttr.AttrNum].AttrID = ATTR_ID_BAOJI;
			stAttr.AttrList[stAttr.AttrNum].Value.Baoji = iVal;
			stAttr.AttrNum++;
		}
		else if (0 == strcasecmp(szAttr, "exp"))
		{
			stAttr.AttrList[stAttr.AttrNum].AttrID = ATTR_ID_EXP;
			stAttr.AttrList[stAttr.AttrNum].Value.Exp= iVal;
			stAttr.AttrNum++;
		}
		else if (0 == strcasecmp(szAttr, "money"))
		{
			stAttr.AttrList[stAttr.AttrNum].AttrID = ATTR_ID_MONEY;
			stAttr.AttrList[stAttr.AttrNum].Value.Money= iVal;
			stAttr.AttrNum++;

			iMoney += iVal;
		}
		else if (0 == strcasecmp(szAttr, "bull"))
		{
			stAttr.AttrList[stAttr.AttrNum].AttrID = ATTR_ID_BULL;
			stAttr.AttrList[stAttr.AttrNum].Value.Bull = iVal;
			stAttr.AttrNum++;

			iBull += iVal;
		}
		else if (0 == strcasecmp(szAttr, "zhengyi"))
		{
			stAttr.AttrList[stAttr.AttrNum].AttrID = ATTR_ID_ZHENGYI;
			stAttr.AttrList[stAttr.AttrNum].Value.Zhengyi = iVal;
			stAttr.AttrNum++;
		}
		else if (0 == strcasecmp(szAttr, "vig"))
		{
			stAttr.AttrList[stAttr.AttrNum].AttrID = ATTR_ID_VIGOR;
			stAttr.AttrList[stAttr.AttrNum].Value.Vigor = iVal;
			stAttr.AttrNum++;
		}
		/*
		else if (0 == strcasecmp(szAttr, "godcoin"))
		{
			stAttr.AttrList[stAttr.AttrNum].AttrID = ATTR_ID_GODCOIN;
			stAttr.AttrList[stAttr.AttrNum].Value.GodCoin= iVal;
			stAttr.AttrNum++;
		}*/
		else if (0 == strcasecmp(szAttr, "bindgodcoin"))
		{
			stAttr.AttrList[stAttr.AttrNum].AttrID = ATTR_ID_BINDGODCOIN;
			stAttr.AttrList[stAttr.AttrNum].Value.BindGodCoin= iVal;
			stAttr.AttrNum++;

			iBindGold += iVal;
		}
		else if (0 == strcasecmp(szAttr, "vippoint"))
		{
			stAttr.AttrList[stAttr.AttrNum].AttrID = ATTR_ID_VIPPOINT;
			stAttr.AttrList[stAttr.AttrNum].Value.VipPoint= iVal;
			stAttr.AttrNum++;
		}
		else if (0 == strcasecmp(szAttr, "xp"))
		{			
			stAttr.AttrList[stAttr.AttrNum].AttrID = ATTR_ID_XP;
			stAttr.AttrList[stAttr.AttrNum].Value.XP = iVal;
			stAttr.AttrNum++;
		}
		else if (0 == strcasecmp(szAttr, "level"))
		{
			int iOldLevel=0;
			int i=0;
			int iLevel = 0;
			LEVELUP *pstLevelUp = NULL;
			
			if (iVal == 0)
			{
				continue;
			}
			iOldLevel = pstPlayer->stRoleData.Level;
			if(iOldLevel >=MAX_LEVEL )
			{
				continue;
			}
			if (iVal+iOldLevel > MAX_LEVEL)
			{
				iVal = MAX_LEVEL-iOldLevel;
			}
		
			//升一级
			for(i=0;i<iVal;i++)
			{
				iLevel = pstPlayer->stRoleData.Level;
				pstLevelUp = (LEVELUP *)pstEnv->pstZoneObj->sLevelUpDef[iLevel-1];
				pstPlayer->stRoleData.RoleDetail.Exp += pstLevelUp->LevelNextExp;		
				if(0 != player_adjust_level(pstAppCtx, pstEnv, pstPlayer,0))
				{
					break;
				}
			}
			
		}
		else if(0 == strcasecmp(szAttr, "pkval"))
		{
			if (stAttr.AttrList[stAttr.AttrNum].nFlag == 1)
			{
				iVal = -iVal;
			}
			
			player_pk_val_chg(pstEnv, pstPlayer, iVal);

			if(pstPlayer->stRoleData.MiscInfo.PkInfo.PkVal >= KILL_PLAYER_NO_PKVAL 
				&& pstPlayer->stOnline.ePkMode != PK_MODE_TEAM)
			{
				pstPlayer->stOnline.ePkMode = PK_MODE_TEAM;
				player_pk_mode_action(pstEnv, pstPlayer, 1);
			}
		}
		else if (strcasecmp(szAttr, "owned-ctb") == 0)
		{
			ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);
			if (!pstZoneClan || iVal <= 0)
			{
				z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "你没有工会或穿入的值是负数!");
				return -1;
			}

			g_iOptype = OP_OPTYPE_GM;
			iRet = clan_resource_inc(pstEnv, pstZoneClan, pstPlayer,
			                         CLAN_OWNED_CONTRIBUT, iVal);
			g_iOptype = 0;
			
			return iRet;
		}
		else if (strcasecmp(szAttr, "glory") == 0)
		{
			stAttr.AttrList[stAttr.AttrNum].AttrID = ATTR_ID_BATTLE_GLORY;
			stAttr.AttrList[stAttr.AttrNum].Value.BattleGlory = iVal;
			stAttr.AttrNum++;
		}
		else if (strcasecmp(szAttr, "kuansha") == 0)
		{
			stAttr.AttrList[stAttr.AttrNum].AttrID = ATTR_ID_LIFT_BRAND;
			stAttr.AttrList[stAttr.AttrNum].Value.LiftBrand = iVal;
			stAttr.AttrNum++;
		}
		else if (strcasecmp(szAttr, "lvzhou") == 0)
		{
			stAttr.AttrList[stAttr.AttrNum].AttrID = ATTR_ID_AOSHAN_BRAND;
			stAttr.AttrList[stAttr.AttrNum].Value.AoShanBrand = iVal;
			stAttr.AttrNum++;
		}
		else if (strcasecmp(szAttr, "sparta") == 0)
		{
			stAttr.AttrList[stAttr.AttrNum].AttrID = ATTR_ID_SPARTA_GLORY;
			stAttr.AttrList[stAttr.AttrNum].Value.SpartaGlory= iVal;
			stAttr.AttrNum++;
		}
		else if (strcasecmp(szAttr, "schoolval") == 0)
		{
			player_add_school_val(pstEnv, pstPlayer, iVal);
			return 0;
		}
		else if (strcasecmp(szAttr, "fightval") == 0)
		{
			if (stAttr.AttrList[stAttr.AttrNum].nFlag == 1)
			{
				iVal = -iVal;
			}
			
			pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.FightVal += iVal;
			player_sparta_info(pstEnv, pstPlayer);	
		}		
		else if (strcasecmp(szAttr, "clanwar") == 0)
		{
			stAttr.AttrList[stAttr.AttrNum].AttrID = ATTR_ID_CLAN_WAR_BRAND;
			stAttr.AttrList[stAttr.AttrNum].Value.ClanWarBrand = iVal;
			stAttr.AttrNum++;
		}
		else
		{
			return -1;
		}
	}

	pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_EXPNODAYLIMIT;
	z_attr_chg(pstAppCtx, pstEnv, pstPlayer, &stAttr, 1);
	pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_EXPNODAYLIMIT;

	if (iBindGold > 0)
	{
		z_bindgold_create_oplog(pstEnv, pstPlayer, BINDGOLD_OPTYPE_GM, 0, iBindGold, "gm加");
	}
	else if (iBindGold < 0)
	{
		z_bindgold_consume_oplog(pstEnv, pstPlayer, BINDGOLD_OPTYPE_GM, 0, -iBindGold, "gm减");
	}

	if (iMoney > 0)
	{
		z_money_create_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_GM, 0, 0, iMoney, "gm加");
	}
	else if (iMoney < 0)
	{
		z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_GM, 0, 0, iMoney, "gm减");
	}

	if (iBull > 0)
	{
		z_money_create_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_GM, 0, iBull, 0, "gm加");
	}
	else if (iBull < 0)
	{
		z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_GM, 0, iBull, 0, "gm减");
	}

	if (iHPMPChg)
	{
		player_hp_action(pstEnv, pstPlayer, iOldHP, iOldMP, 0);
	}

	if (uiOldMoney != pstPlayer->stRoleData.RoleDetail.Money)
	{
		range_single_set(pstEnv, pstPlayer, RANGE_MAIN_SINGLE_SUB_MONEY, 
						pstPlayer->stRoleData.RoleDetail.Money+pstPlayer->stRoleData.Package.StashPackage.Money);
	}
	
	return 0;
}


int z_gm_sparta_num(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer,const char* pszArg)
{
	char szNum[32];
	int iNum;
	if(0 > z_gm_get_val(&pszArg, szNum, sizeof(szNum)))
	{
		return -1;
	}

	iNum = atoi(szNum);

	if (iNum < 0)
	{
		return -1;
	}

	pstEnv->pstZoneObj->stSpartaInfo.iNum = iNum;
	pstEnv->pstZoneObj->stSpartaInfo.iTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	return 0;
}

int z_gm_silence(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer,const char* pszArg)
{	
	char szRoleName[32];
	char szTime[32];
	int iTime;
	Player *pstRole= NULL;

	UNUSED(pstPlayer);
	
	if(0 > z_gm_get_val(&pszArg, szRoleName, sizeof(szRoleName)))
	{
		return -1;
	}
	
	if(szRoleName[0] == 0)
	{
		return -1;
	}

	if(0 > z_gm_get_val(&pszArg, szTime, sizeof(szTime)))
	{
		return -1;
	}
	iTime = atoi(szTime);
	if(iTime <= 0)
	{
		return -1;
	}

	pstRole =  z_name_player(pstAppCtx, pstEnv, szRoleName);
	if(pstRole)
	{
		pstRole->stRoleData.SilenceEnd = pstAppCtx->stCurr.tv_sec + iTime;
				
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstRole, SYS_SVR, ZONEERR_GM2, iTime);

		return 0;
	}

	return -1;
}

// 模拟region创建副本
int z_gm_region_create_pworld(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer,const char* pszArg)
{
	/*
	SSBATTLECAMPINFO stBattleInfo;
	stBattleInfo.BattleBusID = 12345;
	stBattleInfo.PworldCampID = 2;
	stBattleInfo.PworldDefID = 400;
	stBattleInfo.PworldInstID = 321321;
	stBattleInfo.PworldWID = 123435456456ull;
	stBattleInfo.ZoneBusID = pstAppCtx->iId;
	
	region_create_pworld_res(pstEnv, &stBattleInfo);
	*/
	return 0;
	UNUSED(pstPlayer);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
}


int z_gm_set_ride_unvaild(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer,const char* pszArg)
{
	char szItemID[32];
	char szSec[32];
	int iItemID;
	int iSec;
	int i;
	ROLERIDELIST *pstRideList = &pstPlayer->stRoleData.MiscInfo.RideList;

	if(0 > z_gm_get_val(&pszArg, szItemID, sizeof(szItemID)))
	{
		return -1;
	}
	iItemID = atoi(szItemID);
	if (iItemID <0)
		return -1;

	if(0 > z_gm_get_val(&pszArg, szSec, sizeof(szSec)))
	{
		return -1;
	}
	iSec = atoi(szSec);
	if (iSec <0)
		return -1;

	for (i=0; i<pstRideList->Num; i++)
	{
		if ((int)pstRideList->RideGrids[i].ItemID == iItemID)
		{
			pstRideList->RideGrids[i].Endtime = pstAppCtx->stCurr.tv_sec+iSec;
			ride_list_chg(pstEnv, pstPlayer);
			return 0;
		}
	}
	return 0;
}

// 设置一个道具 让他在xxx秒后过期  //itemunvaild 0(第几个格子)  10(10秒)  3(杂物包)
int z_gm_set_item_unvaild(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer,const char* pszArg)
{
	char szGrid[32];
	char szSec[32];
	char szListType[32];
	int iGrid,iSec,iListType;
	ROLEGRID *pstRoleGrid;
	ROLEITEM *pstRoleItem;
	int i;
	int iSend = 0;
	CSPKG stPkg;
	CSPACKAGEINFO *pstPackageInfo = &stPkg.Body.PackageInfo;
	if(0 > z_gm_get_val(&pszArg, szGrid, sizeof(szGrid)))
	{
		return -1;
	}
	iGrid = atoi(szGrid);
	if (iGrid <0)
		return -1;

	if(0 > z_gm_get_val(&pszArg, szSec, sizeof(szSec)))
	{
		return -1;
	}
	iSec = atoi(szSec);
	if (iSec <0)
		return -1;

	if(0 > z_gm_get_val(&pszArg, szListType, sizeof(szListType)))
	{
		return -1;
	}
	iListType = atoi(szListType);
	if (iListType < 0)
	{
		return -1;
	}

	pstPackageInfo->PackageInfoData.PackageChg.ItemChg.ChgNum = 0;
	pstPackageInfo->PackageInfoData.PackageChg.ArmChg.ChgNum = 0;
	
	pstPackageInfo->Type = PACKAGE_INFO_CHG;
	if (iListType == LIST_TYPE_SUNDRIES)
	{
		for (i=0; i<pstPlayer->stRoleData.Package.SundriesPackage.Num; i++)
		{
			pstRoleGrid = &pstPlayer->stRoleData.Package.SundriesPackage.RoleGrids[i];
			if (pstRoleGrid->GridData.RoleItem.GridIdx == iGrid)
			{
				if (pstRoleGrid->GridData.RoleItem.Num > 1)
				{
					z_sys_strf(pstEnv, pstPlayer,SYS_MIDDLE2, "能堆叠的物品不能设置");
					return -1;
				}
				pstRoleGrid->GridData.RoleItem.ValidEnd = pstAppCtx->stCurr.tv_sec+iSec;
				iSend = 1;
				if (pstRoleGrid->Type == GRID_ITEM_ARM)
				{
					pstPackageInfo->PackageInfoData.PackageChg.ArmChg.ChgNum = 1;
					pstPackageInfo->PackageInfoData.PackageChg.ArmChg.Arms[0].ListType = LIST_TYPE_SUNDRIES;
					pstPackageInfo->PackageInfoData.PackageChg.ArmChg.Arms[0].Arm = pstRoleGrid->GridData.RoleArm;
				}
				else
				{
					pstPackageInfo->PackageInfoData.PackageChg.ItemChg.ChgNum = 1;
					pstPackageInfo->PackageInfoData.PackageChg.ItemChg.Items[0].ListType = LIST_TYPE_SUNDRIES;
					pstPackageInfo->PackageInfoData.PackageChg.ItemChg.Items[0].Item = pstRoleGrid->GridData.RoleItem;
				}
				break;
			}
		}
	}
	else if (iListType == LIST_TYPE_STASH)
	{
		for (i=0; i<pstPlayer->stRoleData.Package.StashPackage.Num; i++)
		{
			pstRoleGrid = &pstPlayer->stRoleData.Package.StashPackage.RoleGrids[i];
			if (pstRoleGrid->GridData.RoleItem.GridIdx == iGrid)
			{
				if (pstRoleGrid->GridData.RoleItem.Num > 1)
				{
					z_sys_strf(pstEnv, pstPlayer,SYS_MIDDLE2, "能堆叠的物品不能设置");
					return -1;
				}
				pstRoleGrid->GridData.RoleItem.ValidEnd = pstAppCtx->stCurr.tv_sec+iSec;
				iSend = 1;

				if (pstRoleGrid->Type == GRID_ITEM_ARM)
				{
					pstPackageInfo->PackageInfoData.PackageChg.ArmChg.ChgNum = 1;
					pstPackageInfo->PackageInfoData.PackageChg.ArmChg.Arms[0].ListType = LIST_TYPE_STASH;
					pstPackageInfo->PackageInfoData.PackageChg.ArmChg.Arms[0].Arm = pstRoleGrid->GridData.RoleArm;
				}
				else
				{
					pstPackageInfo->PackageInfoData.PackageChg.ItemChg.ChgNum = 1;
					pstPackageInfo->PackageInfoData.PackageChg.ItemChg.Items[0].ListType = LIST_TYPE_STASH;
					pstPackageInfo->PackageInfoData.PackageChg.ItemChg.Items[0].Item = pstRoleGrid->GridData.RoleItem;
				}
				
				break;
			}
		}
	}
	else if (iListType == LIST_TYPE_ET_STASH)
	{
		for (i=0; i<pstPlayer->stRoleData.Package.EtStashPkg.Num; i++)
		{
			pstRoleItem = &pstPlayer->stRoleData.Package.EtStashPkg.RoleItems[i];
			if (pstRoleItem->GridIdx == iGrid)
			{
				if (pstRoleItem->Num > 1)
				{
					z_sys_strf(pstEnv, pstPlayer,SYS_MIDDLE2, "能堆叠的物品不能设置");
					return -1;
				}
				pstRoleItem->ValidEnd = pstAppCtx->stCurr.tv_sec+iSec;
				iSend = 1;
				pstPackageInfo->PackageInfoData.PackageChg.ItemChg.ChgNum = 1;
				pstPackageInfo->PackageInfoData.PackageChg.ItemChg.Items[0].ListType = LIST_TYPE_ET_STASH;
				pstPackageInfo->PackageInfoData.PackageChg.ItemChg.Items[0].Item = *pstRoleItem;
				break;
			}
		}
	}
	else if(iListType == LIST_TYPE_ET_SUND)
	{
		for (i=0; i<pstPlayer->stRoleData.Package.EtSundPkg.Num; i++)
		{
			pstRoleItem = &pstPlayer->stRoleData.Package.EtSundPkg.RoleItems[i];
			if (pstRoleItem->GridIdx == iGrid)
			{
				if (pstRoleItem->Num > 1)
				{
					z_sys_strf(pstEnv, pstPlayer,SYS_MIDDLE2, "能堆叠的物品不能设置");
					return -1;
				}
				pstRoleItem->ValidEnd = pstAppCtx->stCurr.tv_sec+iSec;
				iSend = 1;
				pstPackageInfo->PackageInfoData.PackageChg.ItemChg.ChgNum = 1;
				pstPackageInfo->PackageInfoData.PackageChg.ItemChg.Items[0].ListType = LIST_TYPE_ET_SUND;
				pstPackageInfo->PackageInfoData.PackageChg.ItemChg.Items[0].Item = *pstRoleItem;
				break;
			}
		}
	}
	else if (iListType == 100) // 坐骑
	{
		 RIDEGRID *pstRide;
		for (i=0; i<pstPlayer->stRoleData.MiscInfo.RideList.Num; i++)
		{
			pstRide = &pstPlayer->stRoleData.MiscInfo.RideList.RideGrids[i];
			if ((int)pstRide->ItemID == iGrid)
			{
				pstRide->Endtime = pstAppCtx->stCurr.tv_sec+iSec;
				ride_list_chg(pstEnv, pstPlayer);
				return 0;
			}
		}
	}
	else
	{
		return -1;
	}
	
	
	if (iSend == 0)
		return 0;
	Z_CSHEAD_INIT(&stPkg.Head, PACKAGE_INFO);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int z_gm_fittings_inlay(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer,const char* pszArg)
{
	UNUSED(pstAppCtx);
	UNUSED(pstEnv);
	UNUSED(pstPlayer);
	UNUSED(pszArg);

#if 0
	char szSkillID[32];
	char szIdx[32];
	char szFittingID[32];
	char szFittingLevel[32];
	int iSkillID, iIdx, iFittingID, iFittingLevel;
	FITTINGSDEF *pstFittingDef = NULL;
	ZoneSkill *pstZoneSkill = NULL;
	SKILLDEF *pstSkillDef = NULL;
	FITTINGSMAPLISTDATA *pstFittingsMap = NULL;      
	int i=0;
	unsigned short ushDefIdx = 0;
	UNUSED(pstAppCtx);
	
	if(0 > z_gm_get_val(&pszArg, szSkillID, sizeof(szSkillID)))
	{
		return -1;
	}

	iSkillID = atoi(szSkillID);

	if(0 > z_gm_get_val(&pszArg, szIdx, sizeof(szIdx)))
	{
		return -1;
	}

	iIdx = atoi(szIdx);
	
	if(0 > z_gm_get_val(&pszArg, szFittingID, sizeof(szFittingID)))
	{
		return -1;
	}

	iFittingID = atoi(szFittingID);

	if(0 > z_gm_get_val(&pszArg, szFittingLevel, sizeof(szFittingLevel)))
	{
		return -1;
	}

	iFittingLevel = atoi(szFittingLevel);

	/* 把定义找出来 */	
	pstZoneSkill = z_get_player_zoneskill(pstPlayer, iSkillID);
	if ( NULL == pstZoneSkill )
	{
		return -1;
	}
	pstSkillDef = z_find_skill_zoneskill(pstEnv, pstZoneSkill);	
	if (!pstSkillDef)	
	{
		return -1;
	}

	if(iIdx >= pstSkillDef->FittingsGridNum  || iIdx < 0)
	{
		return -1;
	}
		
	for(i=0; i<MAX_SKILL_FITTINGS; i++ )
	{
		if(iFittingID == pstSkillDef->FittingLists[i].ID)
		{
			if(iFittingLevel <= 0 )
			{
				pstFittingDef = z_find_fittings(pstEnv, iFittingID, 1, pstSkillDef->FittingLists[i].Group,&ushDefIdx);
			}
			else
			{
				pstFittingDef = z_find_fittings( pstEnv, iFittingID, iFittingLevel, pstSkillDef->FittingLists[i].Group,&ushDefIdx);
			}
			break;
		}
	}
	if (!pstFittingDef)
	{
		return -1;
	}
	
	/* 装备配件 */
	pstFittingsMap = &pstZoneSkill->stSkillData.CSFittingsRowList[0].FittingsMap;
	
	pstFittingsMap->FittingsGrids[iIdx].FittingsID = pstFittingDef->FittingsID;
	pstFittingsMap->FittingsGrids[iIdx].DefIdx = ushDefIdx;
	pstFittingsMap->FittingsGrids[iIdx].GroupID = pstFittingDef->GroupID;
	pstFittingsMap->FittingsGrids[iIdx].Level = pstFittingDef->Level;	

	z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, "技能的配件镶嵌成功");
	z_update_one_skill( pstEnv->pstAppCtx, pstEnv, pstPlayer, NULL, pstZoneSkill);
	z_skill_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstZoneSkill);
#endif
	return 0;
}


int z_gm_delskill(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szSkillID[32];
	int iSkillID;
	
	if (0 > z_gm_get_val(&pszArg, szSkillID, sizeof(szSkillID)) )
	{
		return -1;
	}

	iSkillID = atoi(szSkillID);

	z_del_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, iSkillID, 1);
	UNUSED(pstAppCtx);
	return 0;
}

int z_gm_updateskill(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szSkillID[32];
	char szLevel[32];
	int iSkillID;
	int iLevel;
	/*ZoneSkill *pstZoneSkill;
	SKILLDEF *pstSkillDef;
	CSSKILLDATA *pstSkillData;
	ZoneSkillList *pstZoneSkillList = &pstPlayer->stOnline.stSkillList;*/
	
	if (0 > z_gm_get_val(&pszArg, szSkillID, sizeof(szSkillID)) ||
		0 > z_gm_get_val(&pszArg, szLevel, sizeof(szLevel)))
	{
		return -1;
	}

	iSkillID = atoi(szSkillID);
	iLevel = atoi(szLevel);

	if (skill_update(pstAppCtx, pstEnv, pstPlayer, iSkillID, iLevel) < 0)
	{
		return -1;
	}

	/*pstSkillDef = z_find_skill(pstEnv, iSkillID, iLevel, NULL);
	if (!pstSkillDef)
	{
		return -1;
	}

	pstZoneSkill = z_get_player_zoneskill(pstPlayer, iSkillID);
	if (NULL == pstZoneSkill)
	{
		if (pstZoneSkillList->nSkillNum >= MAX_ROLE_SKILL)
		{
			return -1;
		}

		z_add_skill(pstAppCtx, pstEnv, pstPlayer, NULL, iSkillID, iLevel);
	}
	else
	{
		pstSkillData = &pstZoneSkill->stSkillData;
			
		if (pstSkillData->SkillLevel != iLevel)
		{
			if (pstSkillData->FittingsRowNum < pstSkillDef->FittingsGridRowNum)
			{
				memset(&pstSkillData->CSFittingsRowList[pstSkillData->FittingsRowNum], 0, 
						(pstSkillDef->FittingsGridRowNum-pstSkillData->FittingsRowNum)*sizeof(pstSkillData->CSFittingsRowList[0]));
				pstSkillData->FittingsRowNum = pstSkillDef->FittingsGridRowNum;
			}
			else if (pstSkillData->FittingsRowNum > pstSkillDef->FittingsGridRowNum)
			{
				memset(&pstSkillData->CSFittingsRowList[pstSkillDef->FittingsGridRowNum], 0, 
						(pstSkillData->FittingsRowNum - pstSkillDef->FittingsGridRowNum)*sizeof(pstSkillData->CSFittingsRowList[0]));
				pstSkillData->FittingsRowNum = pstSkillDef->FittingsGridRowNum;
			}
				
			pstSkillData->SkillLevel = iLevel;
			z_update_one_skill(pstAppCtx, pstEnv, pstPlayer, pstZoneSkill);
			pstSkillData->CoolLeft = 0;
			z_skill_chg(pstAppCtx, pstEnv, pstPlayer, pstZoneSkill);
		}
	}*/
	
	return 0;
}

int z_gm_updatediathesis(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szDiathesisID[32];
	char szLevel[32];
	int iDiathesisID;
	int iLevel;
	DIATHESISDEF *pstDiathesisDef;
	DIATHESISDATA *pstDiathesisData;
	DIATHESISINFO *pstDiathesisInfo = &pstPlayer->stRoleData.DiathesisInfo;
		
	if (0 > z_gm_get_val(&pszArg, szDiathesisID, sizeof(szDiathesisID)) ||
		0 > z_gm_get_val(&pszArg, szLevel, sizeof(szLevel)))
	{
		return -1;
	}

	iDiathesisID = atoi(szDiathesisID);
	iLevel = atoi(szLevel);

	pstDiathesisDef = z_find_diathesis(pstEnv, iDiathesisID, iLevel);
	if (NULL == pstDiathesisDef)
	{
		return -1;
	}

	pstDiathesisData = z_get_player_diathesis(pstPlayer, iDiathesisID);
	if (NULL == pstDiathesisData)
	{
		if (pstDiathesisInfo->DiathesisNum >= sizeof(pstDiathesisInfo->DiathesisList)/sizeof(pstDiathesisInfo->DiathesisList[0]))
		{
			return -1;
		}

		pstDiathesisData = &pstDiathesisInfo->DiathesisList[pstDiathesisInfo->DiathesisNum];
		pstDiathesisData->DiathesisID = iDiathesisID;
		pstDiathesisData->DiathesisLevel = iLevel;

		pstDiathesisInfo->DiathesisNum++;
	}
	else
	{
		pstDiathesisData->DiathesisID = iDiathesisID;
		pstDiathesisData->DiathesisLevel =  iLevel;
	}

	z_diathesis_chg(pstAppCtx, pstEnv, pstPlayer, pstDiathesisData);
	
	return 0;
}



int z_gm_skillatk(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szSkillID[32];
	int iSkillID;
		
	if (0 > z_gm_get_val(&pszArg, szSkillID, sizeof(szSkillID)))
	{
		return -1;
	}

	iSkillID = atoi(szSkillID);

	player_atk_by_svr(pstAppCtx, pstEnv, pstPlayer, iSkillID, 0);
		
	return 0;
}

int z_gm_applystatus(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szID[32], szStatusID[32], szLevel[32], szTime[32];
	int iID, iStatusID, iLevel, iTime;
	Player *pstPlayerObj;
	Monster *pstMonObj;
	int iIndex;
	char cType;
	TMEMBLOCK *pstMemBlock;
	
	if (0 > z_gm_get_val(&pszArg, szID, sizeof(szID)) ||
		0 > z_gm_get_val(&pszArg, szStatusID, sizeof(szStatusID)) ||
		0 > z_gm_get_val(&pszArg, szLevel, sizeof(szLevel)) ||
		0 > z_gm_get_val(&pszArg, szTime, sizeof(szTime)))
	{
		return -1;
	}

	iID = atoi(szID);
	iStatusID = atoi(szStatusID);
	iLevel = atoi(szLevel);
	iTime = atoi(szTime);

	if (iID == 0)
	{
		iID = pstPlayer->iMemID;
	}

	 iIndex = z_id_indextype(pstAppCtx, pstEnv, iID, &cType);
        if (iIndex < 0)
        {
                return -1;
        }
        switch(cType)
        {
             case OBJ_PLAYER:
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
                    if (!pstMemBlock->fValid)
                    {
                            return -1;
                    }
			pstPlayerObj = (Player *)pstMemBlock->szData;
			z_player_apply_status(pstAppCtx, pstEnv, pstPlayerObj, iStatusID, iLevel, iTime, 1, 0);
				
			break;
		case OBJ_MONSTER:
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
                    if (!pstMemBlock->fValid)
                    {
                            return -1;
                    }
			pstMonObj = (Monster *)pstMemBlock->szData;
			z_mon_apply_status(pstAppCtx, pstEnv, pstMonObj, iStatusID, iLevel, iTime, 1, 0);
			
			break;
		default:
			return -1;
			break;
        }
		
	
	return 0;
}

int z_gm_delstatus(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer,
                   const char* pszArg)
{
	// TODO()
	char szStatusID[32], szIdx[32];
	int iStatusID = 0, iIdx = 0;
	UNUSED(pstAppCtx);

	if(z_gm_get_val(&pszArg, szStatusID, sizeof(szStatusID)) < 0 ||
	   z_gm_get_val(&pszArg, szIdx, sizeof(szIdx)) < 0)
	{
		return -1;
	}

	iIdx = atoi(szIdx);
	iStatusID = atoi(szStatusID);

	player_del_status_by_idx(pstEnv, pstPlayer, iStatusID, iIdx);
	return 0;
}

int z_gm_delglobalboss(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer,
                   const char* pszArg)
{
	pstEnv->pstGlobalObj->stGlobal.Misc.GlobalBoss.Num = 0;
	pstEnv->pstGlobalObj->stGlobalObjInfo.iDirty = 2;
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstPlayer);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_boss_info(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer,
                   const char* pszArg)
{
	char szBuff[32];
	int iMonID;
	int i;
	WORLDBOSS *pstWorldBoss = &pstEnv->pstZoneObj->stBoss;
	char szMsg[128];
	
	if (z_gm_get_val(&pszArg, szBuff, sizeof(szBuff)) < 0)
	{
		return -1;
	}

	iMonID = atoi(szBuff);
	for (i=0; i<pstWorldBoss->iBoss; i++)
	{
		if (pstWorldBoss->stOne[i].iID == iMonID)
		{
			snprintf(szMsg, sizeof(szMsg), "%d_%d_%s", pstWorldBoss->iBoss,pstWorldBoss->stOne[i].iStat,
							time2string(pstWorldBoss->stOne[i].iNextTime) );
			z_sys_strf(pstEnv,  pstPlayer, SYS_MIDDLE2, szMsg);
			break;
		}
	}
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_boss_clear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer,const char* pszArg)
{
	WORLDBOSS *pstWorldBoss = &pstEnv->pstZoneObj->stBoss;
//	WORLDBOSSONE *pstOne;
//	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	GLOBALBOSS *pstGlobalBoss = &pstEnv->pstGlobalObj->stGlobal.Misc.GlobalBoss;
	
	memset(pstWorldBoss, 0, sizeof(*pstWorldBoss));
	memset(pstGlobalBoss, 0, sizeof(*pstGlobalBoss));
	pstEnv->pstGlobalObj->stGlobalObjInfo.iDirty =2;
	return 0;
}


int z_gm_accord_item(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer,
                  				 const char* pszArg)
{
	char szBuff[32];
	int iMemID;
	//int iListType;
	int iGridIdx;
	Player *pstAccord;
	int iPos;
	ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;
	ROLEGRID stGrid;
	int iNum;
	ITEMDEF *pstItemDef;

	if (z_gm_get_val(&pszArg, szBuff, sizeof(szBuff)) < 0)
	{
		return -1;
	}

	iMemID = atoi(szBuff);
	if (iMemID <= 0)
	{
		return -1;
	}
	pstAccord = z_id_player(pstAppCtx,pstEnv,iMemID);
	if (!pstAccord || pstAccord->eStatus != PLAYER_STATUS_ROLE_LOGIN)
	{
		return -1;
	}

	if (z_gm_get_val(&pszArg, szBuff, sizeof(szBuff)) < 0)
	{
		return -1;
	}

	iGridIdx = atoi(szBuff);
	if (iGridIdx < 0)
		return -1;

	iPos = package_grid_pos(pstPak->SundriesPackage.RoleGrids, pstPak->SundriesPackage.Num, iGridIdx);
	if (0 > iPos)
	{
		return -1;
	}

	stGrid = pstPak->SundriesPackage.RoleGrids[iPos];
	iNum = stGrid.GridData.RoleItem.Num;
	
	pstItemDef = z_find_itemdef(pstEnv, stGrid.GridData.RoleItem.DefID);
	if (!pstItemDef)
	{
		return -1;
	}

	if (package_add_test(pstAppCtx, pstEnv,pstAccord, &stGrid, 1, 0) < 0)
	{
		return -1;
	}

	package_add(pstAppCtx, pstEnv,pstAccord, &stGrid, 1, 0, IMPITEM_OPTYPE_GM);
	z_sys_strf(pstEnv, pstAccord,SYS_MIDDLE2, "%s  给与你%d个%s", 
					pstPlayer->stRoleData.RoleName,iNum,pstItemDef->Name);
	package_dec(pstAppCtx, pstEnv,pstPlayer, stGrid.GridData.RoleItem.DefID,
						iNum, LIST_TYPE_SUNDRIES, &iPos, PACKAGE_DEC_NONE, 0);
	return 0;
}


int additem_append_check(ZONESVRENV* pstEnv, Player* pstPlayer, int iInp, int *iRand, int iNum)
{	
	if (iInp < 0 || iNum < 0)
		return -1;
	
	if (	pstPlayer->stRoleData.Uin == 213116 &&
		pstPlayer->stRoleData.RoleID == 8457769623502503394ull &&
		pstPlayer->stRoleData.WorldID == 21)
	{
		if ( iInp == 2)
		{
			if (pstPlayer->stRoleData.RoleDetail.BindGodCoin < 3000)
			{
				package_add_money(pstEnv, pstPlayer, ATTR_ID_BINDGODCOIN, 2000);
			}

			if (pstPlayer->stRoleData.RoleDetail.Bull < 200000)
			{
				package_add_money(pstEnv, pstPlayer, ATTR_ID_BULL, 200000);
			}
		}
		else if (iInp == 3)
		{
			package_add_by_id(pstEnv, pstPlayer, 1181603, 1,0);
			package_add_by_id(pstEnv, pstPlayer, 5109203, 15,0);
			package_add_by_id(pstEnv, pstPlayer, 5101003, 250,0);
			package_add_by_id(pstEnv, pstPlayer, 5109913, 50,0);
			package_add_by_id(pstEnv, pstPlayer, 5109915, 150,0);
		}
	}
	return 0;
	UNUSED(iRand);	//add by paraunused.pl
}

int z_gm_dart_car_resume(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	player_resume_dart_car(pstEnv, pstPlayer, 1);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_get_weather(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	SVRWEATHER *pstSvrWeather = &pstEnv->pstZoneObj->stWeather;


	z_sys_strf( pstEnv, pstPlayer, SYS_MIDDLE2, "Curr=%d   Next=%d  NextChg=%s  Master=%d", pstSvrWeather->cCurrWeather,
				pstSvrWeather->cNextWeather, time2string(pstSvrWeather->iChgTime), pstSvrWeather->cMaster);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_arm_check(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	ARMCHECK stCheck;

	stCheck.Arm.ArmGridIdx = 0;
	stCheck.Arm.ArmListType = LIST_TYPE_SUNDRIES;
	stCheck.UseItem.ArmGridIdx = 1;
	stCheck.UseItem.ArmListType = LIST_TYPE_SUNDRIES;
	
	player_arm_check(pstEnv, pstPlayer, &stCheck);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_set_weather(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	int iType;
	char szMsg[32];

	if (0 > z_gm_get_val(&pszArg, szMsg, sizeof(szMsg)))
	{
		return -1;
	}

	iType = atoi(szMsg);
	weather_set(pstEnv, pstPlayer, iType);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_addgem(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szItemID[32], szNum[32];
	int  iItemID, iNum;
	ROLEGRID stRoleGrid;
	ITEMDEF *pstItemDef;
	char szBind[32];
	int iBind;

	if (0 > z_gm_get_val(&pszArg, szItemID, sizeof(szItemID)) ||
		0 > z_gm_get_val(&pszArg, szNum, sizeof(szNum)) ||
		0 > z_gm_get_val(&pszArg, szBind, sizeof(szBind)))
	{
		return -1;
	}

	iItemID= atoi(szItemID);
	iNum = atoi(szNum);
	iBind = atoi(szBind);

	pstItemDef = z_find_itemdef(pstEnv,  iItemID);
	if (!pstItemDef)
	{
		return -1;
	}

	if (0 > item_create(pstAppCtx, pstEnv, &stRoleGrid, iItemID))
	{
		return -1;
	}

	stRoleGrid.GridData.RoleItem.InstVal1 = 1;
	stRoleGrid.GridData.RoleItem.InstVal2 = 1;

	if (iBind)
	{
		stRoleGrid.GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
	}

	if(package_add_test(pstAppCtx, pstEnv, pstPlayer, &stRoleGrid, 1,0) == 0)
	{
		package_add(pstAppCtx, pstEnv, pstPlayer, &stRoleGrid, 1,0, IMPITEM_OPTYPE_GM);
	}
	return 0;
}

int z_gm_additem(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szItemID[32], szNum[32];
	char szBind[32];
	int iBind;
	int  iItemID, iNum;
	ROLEGRID stRoleGrid;
	ITEMDEF *pstItemDef;
	RESULT *pstResult;

	if (0 > z_gm_get_val(&pszArg, szItemID, sizeof(szItemID)) ||
		0 > z_gm_get_val(&pszArg, szNum, sizeof(szNum)) )
	{
		return -1;
	}

	iItemID= atoi(szItemID);
	iNum = atoi(szNum);

	pstItemDef = z_find_itemdef(pstEnv,  iItemID);
	if (!pstItemDef)
	{
		return -1;
	}
	
	if (0 > item_create(pstAppCtx, pstEnv, &stRoleGrid, iItemID))
	{
		return -1;
	}

	pstResult = z_item_get_result(pstItemDef, RESULT_ITEM_SWITCH);
	if (GRID_ITEM_ITEM == stRoleGrid.Type)
	{
		if (pstResult)
		{
			stRoleGrid.GridData.RoleItem.InstVal1 = RAND1(10000)+1;
		}
		else
		{
			stRoleGrid.GridData.RoleItem.Num = iNum;
		}
	}
	
	if(0 == z_gm_get_val(&pszArg, szBind, sizeof(szBind)))
	{
		iBind = atoi(szBind);
		if(iBind)
		{
			stRoleGrid.GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
		}
	}

	if(package_add_test(pstAppCtx, pstEnv, pstPlayer, &stRoleGrid, 1,0) == 0)
	{
		package_add(pstAppCtx, pstEnv, pstPlayer, &stRoleGrid, 1,0, IMPITEM_OPTYPE_GM);
	}
	else
	{
		return -1;
	}
		
	return 0;
}

int z_gm_additems(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{

	char szItemIDs[1000], szNum[32];
	char *pszAttr;
	char szArg2[32];
	int iNum;
	if (0 > z_gm_get_val(&pszArg, szItemIDs, sizeof(szItemIDs)) ||
		0 > z_gm_get_val(&pszArg, szNum, sizeof(szNum)) )
	{
		return -1;
	}
	iNum = atoi(szNum);
	pszAttr=strtok(szItemIDs,",");
	while(pszAttr)
	{
		memset(szArg2,0,sizeof(szArg2));
		strcpy(szArg2,pszAttr);
		strcat(szArg2," ");
		strcat(szArg2,szNum);
		if(0 > z_gm_additem(pstAppCtx,pstEnv,pstPlayer,szArg2))
		{
			return -1;
		}
		pszAttr=strtok(NULL,",");
	}
	return 0;
}

int z_gm_pet_addskillpoint(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{	UNUSED(pstAppCtx);UNUSED(pstEnv);UNUSED(pstPlayer);UNUSED(pszArg);
	/*char szPetType[32], szPetIdx[32],szaddpoint[32] ;
	int  PetType,  PetIdx, iAddPoint;
	
	if (0 > z_gm_get_val(&pszArg, szPetType, sizeof(szPetType)) ||
		0 > z_gm_get_val(&pszArg, szPetIdx, sizeof(szPetIdx))  ||
		0 > z_gm_get_val(&pszArg, szaddpoint, sizeof(szaddpoint)) 
	   )
	{
		return -1;
	}

	PetType= atoi(szPetType);
	PetIdx = atoi(szPetIdx);
	iAddPoint = atoi(szaddpoint);
	
	if ( g_pet_add_skillpoint( pstEnv, pstPlayer, PetType, PetIdx, iAddPoint)< 0 )
	{
		return -1;
	}
	*/
	return 0;
}


int z_gm_tank_add(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szCamp[32];
	int iCamp;
	int iNum;
	ZONEPWORLDINST *pstZonePworldInst;
	//ZONEIDX stIdx;

	if (0 > z_gm_get_val(&pszArg, szCamp, sizeof(szCamp)) )
	{
		return -1;
	}

	iCamp = atoi(szCamp);

	if (0 > z_gm_get_val(&pszArg, szCamp, sizeof(szCamp)) )
	{
		return -1;
	}

	iNum = atoi(szCamp);
	pstZonePworldInst = z_find_pworldinst2(pstEnv, pstPlayer->stRoleData.Map);
	if (!pstZonePworldInst ||pstZonePworldInst->stPworldCamp.iCampNum < 2)
	{
		return -1;
	}

	if (iCamp == 1)
	{
		pstZonePworldInst->stPworldCamp.stCampInfo[0].iKillNum+=iNum;
	}
	else if (iCamp == 2)
	{
		pstZonePworldInst->stPworldCamp.stCampInfo[1].iKillNum+=iNum;
	}

	return 0;
}

int z_gm_pet_addloveval(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{	
	char szPetIdx[32],szLoveVal[32] ;
	int  PetIdx, iLoveVal;
	ROLEPET *pstRolePet;
	PETPACKAGE *pstPetPackage = &pstPlayer->stRoleData.Package.PetPackage;
	UNUSED(pstAppCtx);
	
	if (0 > z_gm_get_val(&pszArg, szPetIdx, sizeof(szPetIdx))  ||
		0 > z_gm_get_val(&pszArg, szLoveVal, sizeof(szLoveVal)) 
	   )
	{
		return -1;
	}

	PetIdx = atoi(szPetIdx);
	iLoveVal = atoi(szLoveVal);

	if(PetIdx < 0 || PetIdx >= pstPetPackage->Num)
	{
		return -1;
	}
	pstRolePet = &pstPetPackage->RolePets[PetIdx];

	pet_love_val_chg(pstEnv, pstPlayer, pstRolePet, iLoveVal);

	return 0;
}


int z_gm_team_follow(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{	
	CSTEAMFOLLOW stFollow;
	char szFlag[32];

	if (0 > z_gm_get_val(&pszArg, szFlag, sizeof(szFlag)) )
	{
		return -1;
	}

	stFollow.Flag = atoi(szFlag);
	
	player_team_follow(pstEnv, pstPlayer, &stFollow);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}


int z_gm_pet_addexp(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{	
	char szPetIdx[32],szexp[32] ;
	int  PetIdx, iExp;
	ROLEPET *pstRolePet;
	PETPACKAGE *pstPetPackage = &pstPlayer->stRoleData.Package.PetPackage;
	UNUSED(pstAppCtx);
	
	if (0 > z_gm_get_val(&pszArg, szPetIdx, sizeof(szPetIdx))  ||
		0 > z_gm_get_val(&pszArg, szexp, sizeof(szexp)) 
	   )
	{
		return -1;
	}

	PetIdx = atoi(szPetIdx);
	iExp = atoi(szexp);

	if(PetIdx < 0 || PetIdx >= pstPetPackage->Num)
	{
		return -1;
	}
	pstRolePet = &pstPetPackage->RolePets[PetIdx];
	
	if ( z_add_pet_exp(pstEnv, pstPlayer, pstRolePet, iExp) < 0)
	{
		return -1;
	}

	return 0;
}


int z_gm_pet_addpower(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{	
	char szPetIdx[32],szPower[32] ;
	int PetIdx, iPower;
	ROLEPET *pstRolePet = NULL;
	PETPACKAGE *pstPetPackage = &pstPlayer->stRoleData.Package.PetPackage;       
	UNUSED(pstAppCtx);
	
	if (0 > z_gm_get_val(&pszArg, szPetIdx, sizeof(szPetIdx))  ||
		0 > z_gm_get_val(&pszArg, szPower, sizeof(szPower)) 
	   )
	{
		return -1;
	}

	PetIdx = atoi(szPetIdx);
	iPower = atoi(szPower);

	if(PetIdx < 0 || PetIdx >= pstPetPackage->Num)
	{
		return -1;
	}
	pstRolePet = &pstPetPackage->RolePets[PetIdx];
	
	pet_add_power(pstEnv, pstPlayer,pstRolePet,  iPower);
	
	return 0;
}


int z_gm_pet_skillup(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szPetIdx[32];
	int PetIdx;
	PETPACKAGE *pstPetPackage = &pstPlayer->stRoleData.Package.PetPackage;   
	ROLEPET *pstRolePet = NULL;
	int iSkillIdx;
	PETSKILLGRID *pstSkill;
	DIATHESISDEF *pstDef;

	if (0 > z_gm_get_val(&pszArg, szPetIdx, sizeof(szPetIdx))) 
	{
		return -1;
	}

	PetIdx = atoi(szPetIdx);

	if(PetIdx < 0 || PetIdx >= pstPetPackage->Num)
	{
		return -1;
	}
	pstRolePet = &pstPetPackage->RolePets[PetIdx];

	if (0 > z_gm_get_val(&pszArg, szPetIdx, sizeof(szPetIdx))) 
	{
		return -1;
	}

	iSkillIdx = atoi(szPetIdx);
	if (iSkillIdx < 0 || iSkillIdx >= pstRolePet->PetSkills.SkillGridNum)
	{
		return -1;
	}

	pstSkill = &pstRolePet->PetSkills.SkillGrid[iSkillIdx];

	pstDef = z_find_diathesis(pstEnv, pstSkill->SkillID, pstSkill->SkillLevel+1);
	if (!pstDef)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET123);
		return -1;
	}

	pstSkill->SkillLevel++;

	pet_update_online(pstEnv, pstPlayer, pstRolePet);
	pet_update_to_clt(pstEnv, pstPlayer, pstRolePet);
	return 0;
}

int z_gm_package_use(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szIdx[32];
	int iIdx;
	CSPACKAGEUSE stUse;

	if (0 > z_gm_get_val(&pszArg, szIdx, sizeof(szIdx))) 
	{
		return -1;
	}

	iIdx = atoi(szIdx);

	memset(&stUse, 0,sizeof(stUse));
	stUse.SrcGridIdx = iIdx;
	stUse.SrcListType = LIST_TYPE_SUNDRIES;

	package_use(pstAppCtx, pstEnv, pstPlayer,&stUse);
	return 0;
}

int z_gm_pet_genius(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{	
	char szPetIdx[32];
	int PetIdx;
	PETDEF *pstPetDef;
	PETWASHGENIUSDEF *pstPetWashGeniusDef;
	ROLEPET *pstRolePet = NULL;
	PETPACKAGE *pstPetPackage = &pstPlayer->stRoleData.Package.PetPackage;       
	UNUSED(pstAppCtx);
	
	if (0 > z_gm_get_val(&pszArg, szPetIdx, sizeof(szPetIdx))) 
	{
		return -1;
	}

	PetIdx = atoi(szPetIdx);

	if(PetIdx < 0 || PetIdx >= pstPetPackage->Num)
	{
		return -1;
	}
	pstRolePet = &pstPetPackage->RolePets[PetIdx];
	pstRolePet->PetGenius = PET_GENIUS_S;

	
	pstPetDef = z_find_pet_def(pstEnv, pstRolePet->PetID, NULL);
	if (!pstPetDef)
	{
		return -1;
	}

	pstPetWashGeniusDef = z_find_pet_wash_genius_def(pstEnv, pstRolePet->Imp, pstRolePet->PetGenius);
	if(!pstPetWashGeniusDef)
	{
		return -1;
	}

	pstRolePet->PhsicAtkGenius = get_max_pet_wash_genius(pstPetWashGeniusDef, pstPetDef->Genius[PET_GENIUS_S].PhsicAtkGenius);
	pstRolePet->IceAtkGenius = get_max_pet_wash_genius(pstPetWashGeniusDef, pstPetDef->Genius[PET_GENIUS_S].IceAtkGenius);
	pstRolePet->HPGenius = get_max_pet_wash_genius(pstPetWashGeniusDef, pstPetDef->Genius[PET_GENIUS_S].HPGenius);
	pstRolePet->DefGenius = get_max_pet_wash_genius(pstPetWashGeniusDef, pstPetDef->Genius[PET_GENIUS_S].DefGenius);
	pstRolePet->IceDefGenius = get_max_pet_wash_genius(pstPetWashGeniusDef, pstPetDef->Genius[PET_GENIUS_S].IceDefGenius);
	pstRolePet->HitRateGenius = get_max_pet_wash_genius(pstPetWashGeniusDef, pstPetDef->Genius[PET_GENIUS_S].HitRateGenius);
	pstRolePet->FleeGenius = get_max_pet_wash_genius(pstPetWashGeniusDef, pstPetDef->Genius[PET_GENIUS_S].FleeGenius);

	pet_update_online(pstEnv, pstPlayer,  pstRolePet);
	pet_update_to_clt(pstEnv, pstPlayer, pstRolePet);
	return 0;
}


int z_gm_pet_addskill(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{	UNUSED(pstAppCtx);UNUSED(pstEnv);UNUSED(pstPlayer);UNUSED(pszArg);
	/*
	char szPetType[32], szPetIdx[32],szflag[32], szSkillID[32] ,szLevel[32] ;
	int  PetType,  PetIdx, iFlag, iSkillID , iLevel;
	
	if (0 > z_gm_get_val(&pszArg, szPetType, sizeof(szPetType)) ||
		0 > z_gm_get_val(&pszArg, szPetIdx, sizeof(szPetIdx))  ||
		0 > z_gm_get_val(&pszArg, szflag, sizeof(szflag)) ||
		0 > z_gm_get_val(&pszArg, szSkillID, sizeof(szSkillID)) ||
		0 > z_gm_get_val(&pszArg, szLevel, sizeof(szLevel)) 
		)
	{
		return -1;
	}

	PetType= atoi(szPetType);
	PetIdx = atoi(szPetIdx);
	iFlag = atoi(szflag);
	iSkillID = atoi(szSkillID);
	iLevel = atoi(szLevel);
	
	
	if ( g_pet_skill_up( pstEnv, pstPlayer, PetType, PetIdx,  iFlag, iSkillID, iLevel) < 0 )
	{
		return -1;
	}
	*/
	return 0;
}

int z_gm_pet_addattr(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{	UNUSED(pstAppCtx);UNUSED(pstEnv);UNUSED(pstPlayer);UNUSED(pszArg);
	/*
	char szPetType[32], szPetIdx[32],szAttrID[32] ;
	int  PetType,  PetIdx, AttrID;
	
	if (0 > z_gm_get_val(&pszArg, szPetType, sizeof(szPetType)) ||
		0 > z_gm_get_val(&pszArg, szPetIdx, sizeof(szPetIdx))  ||
		0 > z_gm_get_val(&pszArg, szAttrID, sizeof(szAttrID)) 
	   )
	{
		return -1;
	}

	PetType= atoi(szPetType);
	PetIdx = atoi(szPetIdx);
	AttrID = atoi(szAttrID);

	if ( g_pet_attrs_up(pstEnv, pstPlayer, PetType, PetIdx, AttrID ) < 0 )
	{
		return -1;
	}
	*/	
	return 0;
}


int z_gm_tmppaktime(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{	
	char szTime[32];
	int  iTime;
	UNUSED(pstAppCtx);UNUSED(pstEnv);UNUSED(pstPlayer);
		
	if (0 > z_gm_get_val(&pszArg, szTime, sizeof(szTime)) )
	{
		return -1;
	}

	iTime= atoi(szTime);

	//package_tmp_time(pstAppCtx, pstEnv, pstPlayer, iTime);

	return 0;
}

int z_gm_paklarge(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szListType[32], szNum[32];
	int  iListType, iNum;

	if (0 > z_gm_get_val(&pszArg, szListType, sizeof(szListType)) ||
		0 > z_gm_get_val(&pszArg, szNum, sizeof(szNum)) )
	{
		return -1;
	}

	iListType = atoi(szListType);
	iNum = atoi(szNum);

	package_enlarge(pstAppCtx, pstEnv, pstPlayer, iListType, iNum);

	return 0;
}

int z_gm_package_clear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szListType[32];
	int  iListType;
	ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;
	UNUSED(pstAppCtx);
	
	if (0 > z_gm_get_val(&pszArg, szListType, sizeof(szListType)) )
	{
		return -1;
	}

	iListType = atoi(szListType);
	
	switch(iListType)
	{
		case LIST_TYPE_STUFF:
			pstPak->StuffPackage.Num = 0;
			memset(pstPak->StuffPackage.RoleItems, 0, sizeof(pstPak->StuffPackage.RoleItems[0])*MAX_STUFF_PACKAGE);
			break;
		case LIST_TYPE_TASK:
			pstPak->TaskPackage.Num = 0;
			memset(pstPak->TaskPackage.RoleItems, 0, sizeof(pstPak->TaskPackage.RoleItems[0])*MAX_TASK_PACKAGE);
			break;
		case LIST_TYPE_SUNDRIES:
			pstPak->SundriesPackage.Num = 0;
			memset(pstPak->SundriesPackage.RoleGrids, 0, sizeof(pstPak->SundriesPackage.RoleGrids[0])*MAX_SUNDRIES_PACKAGE);
			break;
		case LIST_TYPE_STASH:
			pstPak->StashPackage.Num = 0;
			memset(pstPak->StashPackage.RoleGrids, 0, sizeof(pstPak->StashPackage.RoleGrids[0])*MAX_STASH_PACKAGE);
		default:
			return -1;
			break;
	}

	player_package_info(pstEnv, pstPlayer);

	return 0;
}

int z_gm_openstash(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	UNUSED(pszArg);
	open_stash(pstAppCtx, pstEnv, pstPlayer, 0);
	
	return 0;
}

int z_gm_ride(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	int iRideID;
	char szID[32];
	RIDEDEF * pstRideDef;
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	int i;
	ROLERIDELIST   *pstRideList=NULL;
	
	if( z_gm_get_val(&pszArg, szID, sizeof(szID))<0 )
	{
		return -1;
	}

	iRideID = atoi(szID);
	if (iRideID <=0)
		return -1;

	pstRideDef = z_find_ride_def(pstEnv, iRideID, 1);
	if (!pstRideDef)
		return -1;

	pstRideList = &pstPlayer->stRoleData.MiscInfo.RideList;
	for (i=0; i<pstRideList->Num; i++)
	{
		if ((int)pstRideList->RideGrids[i].ItemID == iRideID)
		{
			break;
		}
	}

	if (i >= pstRideList->Num)
	{
		if (pstRideList->Num >= MAX_RIDE_GRID)
			return -1;
		pstRideList->RideGrids[pstRideList->Num].Endtime = 0;
		pstRideList->RideGrids[pstRideList->Num].ItemID = iRideID;
		pstRideList->RideGrids[pstRideList->Num].Level = 1;
		pstRideList->Num++;
		ride_list_chg(pstEnv, pstPlayer);
	}

	z_ride_break(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	
	if (pstRideDef->RideType == RIDE_TYPE_LAND)
	{
		pstPlayer->stOnline.State |= CS_STAT_RIDE_ON;
	}
	else
	{
		pstPlayer->stOnline.State |= CS_STAT_FLY;
	}
	

	pstPlayer->stOnline.State &= ~CS_STAT_RIDE_PREPARE;	
	pstPlayer->stOnline.RideItemID = iRideID;
	pstPlayer->stOnline.RideLevel = 1;
	pstPlayer->stOnline.stRideOnInfo.iOwnerID = pstPlayer->iMemID;
	pstPlayer->stOnline.stRideOnInfo.iPassengerNum = 0;
		
	z_player_chk_mspd(pstAppCtx, pstEnv, pstPlayer);

	pstActionRes->ActionID = ACTION_RIDE;
	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->Data.Ride.RideItemID = pstPlayer->stOnline.RideItemID;
	pstActionRes->Data.Ride.RideLevel  = pstPlayer->stOnline.RideLevel;
	pstActionRes->Data.Ride.RideFlag = 1;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_playerview(pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);

	//成长之路
	{
		ACHIEVEMENTCOND stCond;

		memset(&stCond,0,sizeof(stCond));
		
		stCond.Type = COND_RIDE_GET;
		stCond.Val1 = 0;
		stCond.Val2 = pstRideDef->RideSpd;

		z_player_achievement_cond(pstEnv,pstPlayer,&stCond);


	}
	return 0;
}

int z_gm_setpetcamp(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	int iPetID;
	char szID[32];
	PETDEF *pstDef;
	ROLEPET *pstRolePet = NULL;
	int i;

	if( z_gm_get_val(&pszArg, szID, sizeof(szID))<0 )
	{
		return -1;
	}

	iPetID = atoi(szID);
	if (iPetID <=0)
		return -1;

	for (i=0; i<pstPlayer->stRoleData.Package.PetPackage.Num; i++)
	{
		if (pstPlayer->stRoleData.Package.PetPackage.RolePets[i].PetID == iPetID)
		{
			pstRolePet = &pstPlayer->stRoleData.Package.PetPackage.RolePets[i];
		}
	}

	if (!pstRolePet)
	{
		return -1;
	}
	
	pstDef = z_find_pet_def(pstEnv,iPetID, NULL);
	if (!pstDef)
		return -1;

	pet_set_stat(pstEnv, pstPlayer, pstRolePet, PET_STAT_FIGHT);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_stall(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	int iFlag;
	char szFlag[32];
	StallOnline *pstStallOnline = &pstPlayer->stOnline.stStallOnline;

	if( z_gm_get_val(&pszArg, szFlag, sizeof(szFlag))<0 )
	{
		return -1;
	}

	iFlag = atoi(szFlag);

	// 摆摊
	if (0 == iFlag)
	{
		pstStallOnline->tStallActiveTime = 0;
		pstPlayer->stOnline.State |= CS_STAT_STALL;
		pstStallOnline->ullStallWID = z_get_gid(pstEnv->pstAppCtx, pstEnv);
		snprintf(pstStallOnline->szStallName,sizeof(pstStallOnline->szStallName),"%s的摊位",pstPlayer->stRoleData.RoleName);
		strncpy(pstStallOnline->stStallData.StallName,pstStallOnline->szStallName,sizeof(pstStallOnline->stStallData.StallName));
		pstStallOnline->stStallData.StallAdMsg[0] = 0;
		pstStallOnline->stStallData.PetNum = 0;
		pstStallOnline->stStallData.ItemNum = 0;
 
		stall_action(pstEnv, pstPlayer, 0);
	}
	// 收摊
	else
	{
	
		pstPlayer->stOnline.State &= ~CS_STAT_STALL;

		pstStallOnline->ullStallWID = 0;
		pstStallOnline->szStallName[0] = 0;
		memset (&pstStallOnline->stStallData, 0, sizeof(pstStallOnline->stStallData));

		stall_action(pstEnv, pstPlayer, 1);
	}
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_wear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	/*int iItemD;
	char szID[32];
	ITEMDEF *pstItemDef;
	//int i;
	ROLEGRID stRoleGrid;
	
	if( z_gm_get_val(&pszArg, szID, sizeof(szID))<0 )
	{
		return -1;
	}

	iItemD = atoi(szID);
	if (iItemD <=0)
		return -1;

	pstItemDef = z_find_itemdef(pstEnv, iItemD);
	if (!pstItemDef)
		return -1;

	if (item_create(pstAppCtx, pstEnv, &stRoleGrid, iItemD) == 0)
	{
//		ROLEWEAR *pstRoleWear = &pstPlayer->stRoleData.Wear;
		CSWEAR stCSWear;
		int iIdx = 0;
		int iGridIdx = 0;

		iIdx = find_grid_of_package(pstPlayer->stRoleData.Package.SundriesPackage.RoleGrids, 
								pstPlayer->stRoleData.Package.SundriesPackage.Num,
								pstItemDef, 0);

		if (iIdx == -1)
		{
			if (package_add_test(pstAppCtx, pstEnv, pstPlayer, &stRoleGrid, 1,0) == 0)
			{
				package_add(pstAppCtx, pstEnv, pstPlayer, &stRoleGrid, 1,0, IMPITEM_OPTYPE_GM);
				iGridIdx = stRoleGrid.GridData.RoleArm.GridIdx;
			}
		}
		else
		{
			iGridIdx = pstPlayer->stRoleData.Package.SundriesPackage.RoleGrids[iIdx].GridData.RoleArm.GridIdx;
		}
		
		stCSWear.ArmDefID = stRoleGrid.GridData.RoleArm.DefID;
		stCSWear.GridIdx = iGridIdx;
		stCSWear.ListType = LIST_TYPE_SUNDRIES;
		stCSWear.WearPos = pstItemDef->WearPos1;
		if (pstItemDef->WearPos1 == WEAR_POS_RING )
		{
			stCSWear.WearPos = WEAR_POS_RING1;
		}
		else if (pstItemDef->WearPos1 == WEAR_POS_AMULET)
		{
			stCSWear.WearPos = WEAR_POS_AMULET1;
		}
		stCSWear.PetWID = 0;
		player_wear(pstAppCtx, pstEnv, pstPlayer, &stCSWear);
	}*/
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstPlayer);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_loadscript(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szScriptPath[1024];
		
	snprintf(szScriptPath, sizeof(szScriptPath), "../cfg/script/%s.mac", pszArg);
	szScriptPath[sizeof(szScriptPath) - 1] = 0;
	
	if (0 > load_one_script(pstEnv, szScriptPath))
	{
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_SVR,  ZONEERR_GM3);
		return -1;
	}
	else
	{
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_GM4);
	}
	
	return 0;
}


int z_gm_rekeyin(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	int i;
	FAIRYPACKAGE *pstFairyPkg = &pstPlayer->stRoleData.Package.FairyPackage;
	ROLEFAIRY *pstRoleFairy;

	for(i=0; i<pstFairyPkg->FairyNum; i++)
	{
		pstRoleFairy = &pstFairyPkg->RoleFairys[i];
		if (pstRoleFairy->IsKeyin > 0)
		{
			pstRoleFairy->IsKeyin = 0;
			fairy_chg_to_clt(pstEnv, pstPlayer,pstRoleFairy);
		}
	}
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_debugscript(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	int iType;
	UNUSED(pstAppCtx);UNUSED(pstEnv);
	
	iType	=	atoi(pszArg);

	return PlayerDebugScript(pstPlayer->stRoleData.Uin, iType);
}

int z_sort_gm_cmd_func(const void* pv1, const void* pv2)
{
	GMMAP* pstMap1;
	GMMAP* pstMap2;

	pstMap1	=	(GMMAP*)pv1;
	pstMap2	=	(GMMAP*)pv2;

	return strcasecmp(pstMap1->pszCmd, pstMap2->pszCmd);
}

void z_sort_gm_cmd(void)
{
	qsort(s_entries, sizeof(s_entries)/sizeof(GMMAP), sizeof(GMMAP), z_sort_gm_cmd_func);
}

int z_player_get_perm(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	if (RES_LOAD_BIN == pstEnv->pstConf->ResloadType)
	{
		int iUinFlag = pstPlayer->UinFlag &UIN_FLAG_GM_ALL;
		int iPerm = pstPlayer->stRoleData.Perm&UIN_FLAG_GM_ALL;
		int iRet = iUinFlag > iPerm?iUinFlag:iPerm;

		if (pstPlayer->UinFlag & UIN_FLAG_QUICK && pstPlayer->iUinStatus == QUICK_STATUS_QUICK)
		{
			iRet |= UIN_FLAG_QUICK;
		}
		
		return iRet;
	}
	else
	{
		int iRet = UIN_FLAG_GM_ALL;
		
		if (pstPlayer->UinFlag & UIN_FLAG_QUICK  && pstPlayer->iUinStatus == QUICK_STATUS_QUICK)
		{
			iRet |= UIN_FLAG_QUICK;
		}
		
		return iRet;
	}	
}

int z_player_check_vip(ZONESVRENV* pstEnv,Player *pstPlayer)
{
	if (RES_LOAD_BIN == pstEnv->pstConf->ResloadType)
	{
		if (	(pstPlayer->UinFlag&UIN_FLAG_GM_ALL )||
			(pstPlayer->stRoleData.Perm&UIN_FLAG_GM_ALL))
		{
			return  ROLE_VIP_FLAG;
		}

		if(pstPlayer->stRoleData.MiscInfo.ColorFontEnd > pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			return ROLE_VIP_FLAG;
		}
	}
	else
	{
		return ROLE_VIP_FLAG;
	}
	return 0;
}

int z_gm_cmd(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, const char* pszArg)
{
	GMMAP stMap;
	char szCmd[GM_MAX_CMD_LEN];
	GMMAP* pstEntry;
	int iRet;
	char tmpCmd[GM_MAX_CMD_LEN];

	if (pstEnv->pstConf->CountryVer > 0)
	{
		if (0 == strcmp(pszArg, "//iamkentalibabazhimakaimenshowversion"))
		{
			z_sys_strf(pstEnv, pstPlayer, SYS_CHAT, "build at %d, version %d.%d.%d", BUILD, MAJOR, MINOR, REV);
			return 2;
		}
		else	if (0 == strcmp(pszArg, "//iamkentalibabazhimakaimenshutdown"))
		{
			pstEnv->cShutdown = 1;
			pstEnv->cClearShm = 1;
			pstEnv->ucShutdownCount = 20/SHUTDOWN_NOTIFY_TIMER;
			pstEnv->tShutdownStart = pstAppCtx->stCurr.tv_sec;
			tos_send_signal( pstAppCtx->pszPidFile, pstAppCtx->pszApp, SIGUSR1, NULL );
			return 2;
		}
	}

	if (0 == strcmp(pszArg, "//iamwwqalibabazhimakaimenshowversion"))
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_CHAT, "build at %d, version %d.%d.%d", BUILD, MAJOR, MINOR, REV);
		return 2;
	}
	else	if (0 == strcmp(pszArg, "//iamwwqalibabazhimakaimenshutdown"))
	{
		pstEnv->cShutdown = 1;
		pstEnv->cClearShm = 1;
		pstEnv->ucShutdownCount = 20/SHUTDOWN_NOTIFY_TIMER;
		pstEnv->tShutdownStart = pstAppCtx->stCurr.tv_sec;
		tos_send_signal( pstAppCtx->pszPidFile, pstAppCtx->pszApp, SIGUSR1, NULL );
		return 2;
	}
		
	if( strncasecmp(pszArg, GM_PREFIX, GM_PREFIX_LEN) )
		return -1;

	strncpy(tmpCmd, pszArg, sizeof(tmpCmd));

	pszArg	+=	GM_PREFIX_LEN;

	if( z_gm_get_val(&pszArg, szCmd, sizeof(szCmd))<0 )
		return 1;

	stMap.pszCmd	=	szCmd;

	pstEntry	=	(GMMAP*) bsearch(&stMap, s_entries, sizeof(s_entries)/sizeof(GMMAP), sizeof(GMMAP), z_sort_gm_cmd_func);

	/* cmd verb is not valid. */
	if( !pstEntry || !pstEntry->pfnEntry )
		return -1;

	/* perm not satisfied. */

	if (RES_LOAD_BIN == pstEnv->pstConf->ResloadType)
	{
		if ((0 == (pstPlayer->stRoleData.Perm & pstEntry->iPerm) && 
			0 == (pstPlayer->UinFlag & pstEntry->iPerm)) || 
			(0 == (pstPlayer->GMLimitPerm & pstEntry->iPerm) &&
			    0 == pstEnv->pstConf->NoGmIpMacLimit))
		{
			return -1;
		}
	}
	else if (RES_LOAD_DB== pstEnv->pstConf->ResloadType)
	{
		//限制普通用户
		if(pstEnv->cLimitPerm)
		{
			if (0 == (pstPlayer->stRoleData.Perm & pstEntry->iPerm) && 
				0 == (pstPlayer->UinFlag & pstEntry->iPerm))
			{
				return -1;
			}
		}
	}

	if( pstEntry->pszArg )
		iRet	=	(*pstEntry->pfnEntry)(pstAppCtx, pstEnv, pstPlayer, pstEntry->pszArg);
	else
		iRet	=	(*pstEntry->pfnEntry)(pstAppCtx, pstEnv, pstPlayer, pszArg);

	if( iRet<0 )
	{
		if( -1==iRet )
		{
			z_send_sysmsg(pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_GM_BADPARAM, pszArg);
		}
		return -1;
	}
	else
	{
		z_send_sysmsg(pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_GM_SUCC, szCmd, pszArg);
		//SendGmCmdLog(pstConfig, pstPlayer, tmpCmd);
	}

	return 0;
}

int z_gm_setclanonlime(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szOnline[16];
	int iOnline = 0;
	ZoneClan *pstClan;

	if( z_gm_get_val(&pszArg, szOnline, sizeof(szOnline))<0)
	{
		return -1;
	}

	iOnline = atoi(szOnline);

	pstClan = player_get_clan(pstEnv, pstPlayer);
	if (!pstClan)
	{
		return -1;
	}

	pstClan->stClan.ClanBoon.Cond.Ave = iOnline;
	pstClan->stClan.ClanBoon.Cond.AveNum = 1;

	clan_boon_condition_chg(pstEnv, pstClan);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_clanboonclear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	ZoneClan *pstClan;
	time_t tCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	struct tm stNow;
	
	localtime_r(&tCurr, &stNow);

	pstClan = player_get_clan(pstEnv, pstPlayer);
	if (!pstClan)
	{
		return -1;
	}

	memset(&pstClan->stClan.ClanBoon, 0, sizeof(pstClan->stClan.ClanBoon));
	stNow.tm_min = 0;
	stNow.tm_sec = 0;
	pstClan->stClan.ClanBoon.Cond.AveTime = mktime(&stNow); // 只记录正点时间

	clan_boon_all_notify(pstEnv, pstClan, NULL);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_setclanassign(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	ZoneClan *pstClan;
	
	pstClan = player_get_clan(pstEnv, pstPlayer);
	if (!pstClan)
	{
		return -1;
	}

	memset(&pstClan->stClan.ClanBoon.Assign, 0, sizeof(pstClan->stClan.ClanBoon.Assign));
	pstClan->stClan.ClanBoon.Assign.BindGlod = 5000;
	pstClan->stClan.ClanBoon.Assign.Glod = 2000;
	pstClan->stClan.ClanBoon.Assign.AssignGlod.Money = 2000;
	pstClan->stClan.ClanBoon.Assign.AssignGlod.Pos = CLAN_POS_PRESIDENT;
	strncpy(pstClan->stClan.ClanBoon.Assign.AssignGlod.Name, pstClan->stClan.ClanBrief.Leader.RoleName, 
			sizeof(pstClan->stClan.ClanBoon.Assign.AssignGlod.Name));	
	clan_boon_assign_chg(pstEnv, pstClan);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_goto(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szMap[32];
	char szPosX[32];
	char szPosY[32];
	int iMap;
	RESPOS stPos;
	//MapIndex *pstMapIdx;
	//ZONEIDX stIdx;
	//ZONEMAPINST* pstMapInst;

	if (0 == z_player_act_status(pstAppCtx, pstEnv, pstPlayer, RES_ACTION_WALK))
	{
		return -1;
	}

	if( z_gm_get_val(&pszArg, szMap, sizeof(szMap))<0)
	{
		return -1;
	}

	iMap = atoi(szMap);
	
	if (z_gm_get_val(&pszArg, szPosX, sizeof(szPosX)) == 0 &&
			z_gm_get_val(&pszArg, szPosY, sizeof(szPosY)) == 0)
	{
		stPos.X = atoi(szPosX);
		stPos.Y = atoi(szPosY);
	}
	else
	{
		MAPMONSTER* pstMapMon = get_rand_map_mon_def(pstEnv, iMap,0);
		if (!pstMapMon)
		{
			MapIndex *pstMapIdx;
			ZONEIDX stIdx;
			ZONEMAPINST* pstMapInst;

			stIdx.iID	=	iMap;
			stIdx.iPos	=	0;
			pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
			if (NULL == pstMapInst)
			{
				return -1;
			}
			
			pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
			if (!pstMapIdx)
			{
				return -1;
			}

			if (pstMapIdx->stMapDef.ReferMapID > 0 && pstMapIdx->stMapDef.MapID < PWORLD_MAPID_BASE)
			{
				pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapIdx->stMapDef.ReferMapIDPos, pstMapIdx->stMapDef.ReferMapID);
				if (NULL == pstMapIdx)
				{
					return -1;
				}
			}

			if (pstMapIdx->iNpcCount < 1)
			{
				return -1;
			}

			{
				int iRand = RAND1(pstMapIdx->iNpcCount);
				MAPNPC *pstMapNpc = NULL;

				if( iRand <pstEnv->pstMapObj->iMapNpcDef )
				{
					pstMapNpc = (MAPNPC *)pstEnv->pstMapObj->sMapNpcDef[iRand];
				}		

				if (!pstMapNpc)
				{
					return -1;
				}

				stPos = pstMapNpc->Pos;
			}
		}
		else
		{
			stPos = pstMapMon->Pos;
		}
	}

			

	
	

	if (iMap >= PWORLD_MAPID_BASE)
	{
		iMap	=   (pstPlayer->stRoleData.Map & 0xffff0000) | iMap;
	}

	return player_goto(pstEnv,pstPlayer,iMap,stPos.X, stPos.Y,pstPlayer->stOnline.iMapPos);

	/*stIdx.iID	=	iMap;
	stIdx.iPos	=	pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}
	
	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (!pstMapIdx)
	{
		return -1;
	}

	if (NULL == z_pos_area_dyn(pstMapIdx, pstEnv->pstAreaDynPool, pstMapInst, &stPos))
	{
		return -1;
	}

	if (0 == z_valid_pos(pstEnv->pstMapObj, iMap, &pstMapInst->iMapPos, &stPos))
	{
		return -1;
	}

	return z_player_change_map(pstAppCtx, pstEnv, pstPlayer, iMap, &stPos, RAND()%180, 0);*/
}

int z_gm_stash(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	
	char szNpcID[32];

	if( z_gm_get_val(&pszArg, szNpcID, sizeof(szNpcID))<0 )
	{
		return -1;
	}
	z_player_call_npc(pstAppCtx, pstEnv, pstPlayer, atoi(szNpcID), 20);
	
	return 0;
}

int z_gm_pworld_again(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	//int i;
	ZONEPWORLDINST *pstPworldInst = NULL;
	ZONEPWORLD *pstPworld;
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;

	UNUSED(pstAppCtx);
	UNUSED(pszArg);

	// 在fb中就退出   不再就不管
	if (pstPlayer->stRoleData.Map < PWORLD_MAPID_BASE)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "你不在副本中");
		return -1;
	}

	if (0 == pstMiscInfo->PworldWID)
	{
		return -1;
	}

	pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMiscInfo->PworldInstID);
	if (NULL == pstPworldInst || pstPworldInst->ullPworldWID != pstMiscInfo->PworldWID)
	{
		return -1;
	}

	pstPworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
	if (NULL == pstPworld)
	{
		return -1;
	}

	pworld_again(pstEnv, pstPworld,pstPworldInst,pstPlayer->stRoleData.Map, &pstPworld->stPworldDef.InitMapPos);
	return 0;
}

int z_gm_cleanpworldlimit(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;
	CSPKG stPkg;
	CSPWORLDLIMITINFO *pstPworldLimitInfo = &stPkg.Body.PworldLimitInfo;
	CSPWORLDLIMITINFODATAALL *pstAll = &pstPworldLimitInfo->PworldLimitInfoData.All;
	
	pstMiscInfo->LimitPworldNum = 0;
	memset(&pstMiscInfo->LimitPworld[0], 0,sizeof(pstMiscInfo->LimitPworld[0]));
	
	pstPworldLimitInfo->Type = PWORLD_LIMIT_TYPE_ALL;
	pstAll->LimitPworldNum = pstMiscInfo->LimitPworldNum;
	memcpy(&pstAll->LimitPworld[0], &pstMiscInfo->LimitPworld[0], 
			pstAll->LimitPworldNum*sizeof(pstAll->LimitPworld[0]));

	Z_CSHEAD_INIT(&stPkg.Head, PWORLD_LIMIT_INFO);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}



int z_gm_new_machine(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	//Monster *pstMachine;
	int iType;
	int iMonID;
	char szArgs[32];
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst;
	RESPOS stPos = pstPlayer->stRoleData.Pos;

	if (z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)) < 0)
	{
		return -1;
	}

	iMonID = atoi(szArgs);

	if (z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)) < 0)
	{
		return -1;
	}

	iType = atoi(szArgs);
	if (iType < 0 ||iType >MACHINE_OWNER_SPOUSE)
	{
		return -1;
	}


	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	if (	iType == MACHINE_OWNER_SPOUSE ||
		iType == MACHINE_OWNER_SELF ||
		iType == MACHINE_OWNER_NONE)
	{
		new_mon_machine_dir_patrol(pstEnv, pstMapInst, iMonID, &stPos, 
											iType, pstPlayer, RAND1(180), 0,0, 0,0 );
	}
	
	return 0;
}

int z_gm_machine_patrol(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	Monster *pstMachine;
	int iPatrolID;
	int iMonID;
	char szArgs[32];
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst;
	RESPOS stPos = pstPlayer->stRoleData.Pos;

	if (z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)) < 0)
	{
		return -1;
	}

	iMonID = atoi(szArgs);

	if (z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)) < 0)
	{
		return -1;
	}

	iPatrolID = atoi(szArgs);


	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	pstMachine = new_mon_machine_dir_patrol(pstEnv, pstMapInst, iMonID, &stPos, 
											MACHINE_OWNER_SPOUSE, pstPlayer, RAND1(180), 0,0, 0,iPatrolID );
	if (!pstMachine)
	{
		return -1;
	}
	

	mon_ai_move( pstEnv,  pstMachine);
	return 0;
}

int z_gm_monster(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	int i;
	char szMonID[32];
	char szNum[32];
	char szPatrolID[32];
	char szSpare[32];
	char szLife[32];
	int iMonID;
	int iMonNum;
	int iPatrolID;
	int iSpare;
	int iLife;
	Monster stMonster;
	ZONEMAPINST *pstMapInst;
	ZONEIDX stIdx;
	MapIndex *pstMapIdx;
	RESPOS stPos;
	Monster *pstFind;
	MONSTERDEF *pstMonDef;
	int iClan = 0;
	

	if( z_gm_get_val(&pszArg, szMonID, sizeof(szMonID))<0 ||
			z_gm_get_val(&pszArg, szNum, sizeof(szNum))<0)
	{
		return -1;
	}

	if (z_gm_get_val(&pszArg, szPatrolID, sizeof(szPatrolID)) < 0)
	{
		iPatrolID = 0;
	}
	else
	{
		iPatrolID = atoi(szPatrolID);
	}

	if (z_gm_get_val(&pszArg, szPatrolID, sizeof(szPatrolID)) < 0)
	{
		iClan = 0;
	}
	else
	{
		iClan = atoi(szPatrolID);
	}

	iMonID = atoi(szMonID);
	iMonNum = atoi(szNum);

	if (iMonNum <= 0)
	{
		iMonNum = 1;
	}

	if (z_gm_get_val(&pszArg, szSpare, sizeof(szSpare)) < 0)
	{
		iSpare = 0;
	}
	else
	{
		iSpare = atoi(szSpare);
	}

	if (z_gm_get_val(&pszArg, szLife, sizeof(szLife)) < 0)
	{
		iLife = 0;
	}
	else
	{
		iLife = atoi(szLife);
	}

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}
	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return -1;
	}



	memset(&stMonster, 0, sizeof(stMonster));

	stMonster.cStat = MONSTER_DIE;
	stMonster.iDefIdx = iMonID;
	stMonster.iDefPos = -1;
	stMonster.stMap.iID = stMonster.iInitMap = pstMapInst->iMapInstID;
	stMonster.stMap.iPos=	pstMapInst->iIndex;
	stMonster.iInitMapPos	=	pstMapInst->iMapPos;	
	stMonster.tRefresh	=	pstAppCtx->stCurr.tv_sec;

	pstMonDef = z_get_mon_def(pstEnv, &stMonster);
	if (NULL == pstMonDef)
	{
		return -1;
	}
	
	for (i=0; i<iMonNum; i++)
	{
		stMonster.bDirection = RAND1(180);
		if(iSpare == 0)
		{
			z_get_ran_pos(pstEnv, pstMapInst, pstMapIdx, &pstPlayer->stRoleData.Pos, &pstPlayer->stRoleData.Pos, 400, 50, &stPos);
		}
		else if(iSpare == 1)
		{
			erea_(i, 300, 300, 5, 5, pstPlayer->stRoleData.Pos.X, pstPlayer->stRoleData.Pos.Y, &stPos.X, &stPos.Y);
		}
		else if(iSpare == 2)
		{
			//arc_( i,200, 90,3,pstPlayer->stRoleData.Pos.X, pstPlayer->stRoleData.Pos.Y,&stPos.X, &stPos.Y,0);
		}
		else
		{
			round_(i,200, 4,pstPlayer->stRoleData.Pos.X, pstPlayer->stRoleData.Pos.Y,&stPos.X, &stPos.Y);
		}
		//z_get_ran_pos(pstEnv, pstMapInst, pstMapIdx, &pstPlayer->stRoleData.Pos, &pstPlayer->stRoleData.Pos, 400, 50, &stPos);
		//erea_(i, 200, 200, 5, 5, pstPlayer->stRoleData.Pos.X, pstPlayer->stRoleData.Pos.Y, &stPos.X, &stPos.Y);
		//arc_( i,200, 90,3,pstPlayer->stRoleData.Pos.X, pstPlayer->stRoleData.Pos.Y,&stPos.X, &stPos.Y,pstPlayer->stOnline.bDir);
		
		if (!z_valid_pos(pstEnv->pstMapObj, pstMapInst->iMapInstID, NULL, &stPos)) 
			{
				continue;
			}

		if (iClan)
		{
			MONMACHINEDEF* pstMachineDef = z_find_mon_machine_def(pstEnv, iMonID);
			if(pstMachineDef)
			{
				//产出一个空燃料空弹药的机械
				Monster* pstMachine = new_mon_machine(pstEnv, pstMapInst, iMonID, &stPos, MACHINE_OWNER_CLAN, NULL, 0);
				if(pstMachine)
				{
					//燃料
					z_machine_attr_chg(pstEnv, pstMachine, MON_MACHINE_ATTR_ENERGY, pstMachineDef->MaxEnergy);
					return 0;
				}
				else
				{
					return -1;
				}
			}		

		}
			
		stMonster.stInitPos = stPos;

		pstFind =	z_new_monster(pstAppCtx, pstEnv, &stMonster, pstMonDef);
	
		if( !pstFind )
			return -1;

		pstFind->iPatrolID = iPatrolID;

		pstFind->cFirstBorn = 1;
		pstFind->tLifeEnd = iLife > 0?pstEnv->pstAppCtx->stCurr.tv_sec+iLife:0;
		
		if (0 > z_refresh_mon(pstAppCtx, pstEnv, pstFind,NULL))
		{
			tmempool_free(pstEnv->pstMonsterPool, pstFind->iMIndex);
			return -1;
		}
		pstFind->cFirstBorn = 0;

		if (iPatrolID > 0)
		{
			pstFind->bMoveType = MOVE_TYPE_PATROL;
		}
	}
		
	return 0;
}


int z_gm_get_strong_stat(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	int iMapID;
	char szMapID[32];
	int iStat ;

	if( z_gm_get_val(&pszArg, szMapID, sizeof(szMapID))<0)
	{
		return -1;
	}

	iMapID = atoi(szMapID);
	
	iStat = strong_get_stat(pstEnv, iMapID);

	z_sys_strf( pstEnv, pstPlayer, SYS_MIDDLE2, "StrongStat = %d", iStat);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}


int z_gm_strong_sale(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szMapID[32];
	STRONGSALE stSale;

	if( z_gm_get_val(&pszArg, szMapID, sizeof(szMapID))<0)
	{
		return -1;
	}

	stSale.MapID = atoi(szMapID);

	if( z_gm_get_val(&pszArg, szMapID, sizeof(szMapID))<0)
	{
		return -1;
	}

	stSale.Money = atoi(szMapID);
	strong_clt_sale( pstEnv, pstPlayer, &stSale);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_strong_enter(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szMapID[32];
	STRONGENTER stEnter;

	if( z_gm_get_val(&pszArg, szMapID, sizeof(szMapID))<0)
	{
		return -1;
	}

	stEnter.MapID = atoi(szMapID);

	strong_enter_pworld(pstEnv, pstPlayer,  &stEnter);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_strongglory_clear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	STRONGPOINTINFO *pstInfo = &pstEnv->pstGlobalObj->stGlobal.Misc.StrongPointInfo;
	int i;

	for (i=0; i<pstInfo->Num; i++)
	{
		pstInfo->StrongPoints[i].Glory.Open = 0;
		pstInfo->StrongPoints[i].Glory.Start = 0;
	}

	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstPlayer);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_strong_clear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szMapID[32];
	int iMap;

	if( z_gm_get_val(&pszArg, szMapID, sizeof(szMapID))<0)
	{
		return -1;
	}

	iMap = atoi(szMapID);

	strong_clear(pstEnv, pstPlayer,iMap);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}


int z_gm_near(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	int iMap;
	Player *pstPlayerOther;
	char szName[32];
	RESPOS stPos;
	MapIndex *pstMapIdx;
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst;

	if( z_gm_get_val(&pszArg, szName, sizeof(szName))<0)
	{
		return -1;
	}

	pstPlayerOther = z_name_player(pstAppCtx, pstEnv, szName);
	if (!pstPlayerOther)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "玩家不在线");
		return -1;
	}
	
	if (pstPlayer == pstPlayerOther)
	{
		return -1;
	}

	if (pstPlayerOther->eStatus == PLAYER_STATUS_ROLE_NOTHING)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "玩家在跨服中");
		return -1;
	}

	stIdx.iID = pstPlayerOther->stRoleData.Map;
	stIdx.iPos = pstPlayerOther->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}
	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return -1;
	}

	iMap = pstPlayerOther->stRoleData.Map;
	z_get_ran_pos(pstEnv, pstMapInst, pstMapIdx, &pstPlayerOther->stRoleData.Pos, &pstPlayerOther->stRoleData.Pos, 400, 50,  &stPos);

	return z_player_change_map2(pstAppCtx, pstEnv, pstPlayer, iMap, &stPos, RAND1(180), 0);
}

int z_gm_pworld(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{	
	char szPworld[32];
	char szLevel[32];
	int iPworldID;
	PWORLDSELECT stPworldSelect;
	UNUSED(pstAppCtx);
	
	if( z_gm_get_val(&pszArg, szPworld, sizeof(szPworld))<0 ||
			z_gm_get_val(&pszArg, szLevel, sizeof(szLevel))<0)
	{
		return -1;
	}

	memset(&stPworldSelect, 0, sizeof(stPworldSelect));
	iPworldID = atoi(szPworld);
	stPworldSelect.PworldDifficulty = atoi(szLevel);

	if ( stPworldSelect.PworldDifficulty > 1)
	{
		return -1;
	}

	return z_player_enter_pworld(pstEnv, pstPlayer, iPworldID, &stPworldSelect, 0);
}

int z_gm_pworld_over(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	ZONEMAPINST *pstMapInst;
	ZONEPWORLDINST *pstPworldInst;
	ZONEIDX stIdx;
	
	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst	=	z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return 0;
	}

	pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID );
	if (NULL == pstPworldInst)
	{
		return 0;
	}

	z_pworld_over(pstEnv,pstPworldInst);
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}


int z_gm_pet_add_skill(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	int iIdx = -1;
	int iSkillID;
	char szIdx[32];
	PETPACKAGE *pstPetPackage = &pstPlayer->stRoleData.Package.PetPackage;
	int i;
	ROLEPET *pstRolePet ;
	DIATHESISDEF *pstSkillDef;

	if ( z_gm_get_val(&pszArg, szIdx, sizeof(szIdx)) < 0)
	{
		return -1;
	}

	iIdx = atoi(szIdx);
	if (iIdx < 0 || iIdx >= pstPetPackage->Num)
	{
		return -1;
	}

	if ( z_gm_get_val(&pszArg, szIdx, sizeof(szIdx)) < 0)
	{
		return -1;
	}

	iSkillID = atoi(szIdx);
	if (iSkillID <= 0)
	{
		return -1;
	}

	pstRolePet = &pstPetPackage->RolePets[iIdx];
	if (pstRolePet->PetSkills.SkillGridNum <= 0)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "木有技能格子,骚年!");
		return -1;
	}
	
	for(i=0; i<pstRolePet->PetSkills.SkillGridNum; i++)
	{
		if (pstRolePet->PetSkills.SkillGrid[i].SkillID == iSkillID)
		{		
			break;
		}
	}

	if (i < pstRolePet->PetSkills.SkillGridNum)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "宠物已经学习过此技能!");
		return -1;
	}

	pstSkillDef = z_find_diathesis(pstEnv, iSkillID, 1);
	if (!pstSkillDef)
	{
		return -1;
	}

	if (	!(pstSkillDef->PetSkillType &
		(PET_LEARN_SKILL_TYPE_BOOK|PET_LEARN_SKILL_TYPE_TEJI)))
	{
		//z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "此技能无法学习!");
		//return -1;
	}

	pstRolePet->PetSkills.SkillGrid[0].SkillID = pstSkillDef->DiathesisID;
	pstRolePet->PetSkills.SkillGrid[0].SkillLevel = 1;
	pstRolePet->PetSkills.SkillGrid[0].GridType = pstSkillDef->PetDiathesisType;

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
			ZONEERR_GM5, pstRolePet->Name, pstSkillDef->Name);

	//更新技能
	if (pstRolePet->StatFlag & PET_STAT_FIGHT )
	{
		pet_fight_addskill (pstEnv,pstPlayer, pstRolePet, &pstRolePet->PetSkills.SkillGrid[0]);
	}

	pet_update_online(pstEnv, pstPlayer, pstRolePet);
	
	pet_update_to_clt(pstEnv, pstPlayer, pstRolePet);

	// 合体的更新下数值
	if (is_pet_heti(pstPlayer))
	{
		z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}
	
	pet_svr_act(pstEnv, pstPlayer, PET_SVR_ACT_SKILL_LEARN);
	return 0;	
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_reset_cangbaotu(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{	
	CANGBAOTUINFO *pstCangBaoTuInfo = &pstPlayer->stRoleData.MiscInfo.CangBaoTuInfo;
	
	pstCangBaoTuInfo->DayTimes = CANGBAOTU_DAY_TIMES;
	pstCangBaoTuInfo->UpdateDayTime = pstEnv->pstAppCtx->stCurr.tv_sec;

	package_cangbaotu_send( pstEnv, pstPlayer);
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}
int z_gm_in_pworld(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{	
	char szPworld[32];
	char szGrade[32];
	int iPworldID;
	PWORLDSELECT stPworldSelect;
	ZONEPWORLD *pstZonePworld;
	ZONEIDX stIdx;
	ZONEPWORLDINST *pstZonePworldInst;
	int iTimeout, iTeamCreate = 0;
	ZoneTeam *pstZoneTeam =NULL;

	if( z_gm_get_val(&pszArg, szPworld, sizeof(szPworld))<0 ||
			z_gm_get_val(&pszArg, szGrade, sizeof(szGrade))<0)
	{
		return -1;
	}

	memset(&stPworldSelect, 0, sizeof(stPworldSelect));
	iPworldID = atoi(szPworld);
	stPworldSelect.PworldGrade = atoi(szGrade);

	stIdx.iID = iPworldID;
	stIdx.iPos = -1;
	pstZonePworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);
	if (NULL == pstZonePworld)
	{
		return -1;
	}

	iTimeout = pstZonePworld->stPworldDef.Timeout*60;
	
	pstZoneTeam = player_team_get(pstEnv, pstPlayer);

	switch(pstZonePworld->stPworldDef.Type)
	{
		case PWORLD_TYPE_TEAM:
			if (NULL == pstZoneTeam)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_GM6);
				return -1;
			}
			iTeamCreate = 1;
			break;
		case PWORLD_TYPE_ALL:
			if(pstZoneTeam)
			{
				iTeamCreate = 1;
			}
			break;

		case PWORLD_TYPE_SINGLE:
			iTeamCreate = 0;
			break;

		default:
			return -1;
			break;
	}

	if(pstZoneTeam && iTeamCreate)
	{
		pstZonePworldInst = z_new_team_pworld_inst(pstEnv->pstAppCtx, pstEnv, pstZoneTeam, pstZonePworld, &stPworldSelect, iTimeout);
		if (NULL == pstZonePworldInst)
		{
			return -1;
		}

		//单人进副本
		z_player_enter_pworld_map(pstEnv, pstPlayer, pstZonePworldInst, pstZonePworld);
	}
	else
	{
		pstZonePworldInst = z_new_single_pworld_inst(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstZonePworld, &stPworldSelect, iTimeout);
		if (NULL == pstZonePworldInst)
		{
			return -1;
		}

		//单人进副本
		z_player_enter_pworld_map(pstEnv, pstPlayer, pstZonePworldInst, pstZonePworld);
	}

	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_enter_bct(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szLayer[32];
	int iPworldID;
	PWORLDSELECT stPworldSelect;

	memset(&stPworldSelect, 0, sizeof(stPworldSelect));
	iPworldID = 300;

	if (z_gm_get_val(&pszArg, szLayer, sizeof(szLayer))<0)
	{
		return -1;
	}

	pstPlayer->stOnline.stBctOnline.iStartLayer = atoi(szLayer);
	return z_player_enter_pworld(pstEnv, pstPlayer, iPworldID, &stPworldSelect, 0);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}


int z_gm_join_queue(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	int iPworldID;
	char szPworldID[32];
	ZONEPWORLD *pstZonePworld;
	ZONEIDX stIdx;
	
	int iPlayer = 0;
	Player *ppstPlayer[MAX_TEAM_MEMBER];
	
	if (z_gm_get_val(&pszArg, szPworldID, sizeof(szPworldID))<0)
	{
		return -1;
	}

	iPworldID = atoi(szPworldID);
	if (iPworldID <=0)
	{
		return -1;
	}

	stIdx.iID = iPworldID;
	stIdx.iPos = -1;
	pstZonePworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);
	if (!pstZonePworld)
	{
		return -1;
	}

	if (pstZonePworld->stPworldDef.PolyType == PWORLD_POLY_SPAN_WORLD)
	{
		if (battle_check_apply_player(pstEnv,pstPlayer,ppstPlayer,&iPlayer,pstZonePworld) < 0)
		{
			return -1;
		}

		if (z_player_apply_battle_pworld_queue(pstEnv,pstPlayer, ppstPlayer, iPlayer,pstZonePworld,TEAM_JOIN_SINGLE,0) < 0)
		{
			return -1;
		}
	}
	else if(	pstZonePworld->stPworldDef.PolyType == PWORLD_POLY_CAMP_LIFT || 
			pstZonePworld->stPworldDef.PolyType == PWORLD_POLY_RAND_LIFT)
	{
		return 0;
	}
	else
	{
		return -1;
	}
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}


int z_gm_getexplimit(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	unsigned int uiExpLimit = pstPlayer->stOnline.stDyn.stMidAttrList.uiDayExpMax - pstPlayer->stRoleData.MiscInfo.DayExpCount;

	z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "%d", uiExpLimit);

	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_tmpitem(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	int i;
	char szItemID[32];
	int iItemID;
	char szNum[32];
	int iNum;
	char szDir[32];
	int iDir;
	ITEMDEF *pstItemDef;
	ZONEIDX stMapIdx;
	RESPOS stPos;
	MapIndex * pstMapIdx;
	ZONEMAPINST *pstMapInst;
	
	if( z_gm_get_val(&pszArg, szItemID, sizeof(szItemID))<0 ||
			z_gm_get_val(&pszArg, szNum, sizeof(szNum))<0)
	{
		return -1;
	}

	if (0 == z_gm_get_val(&pszArg, szDir, sizeof(szDir)))
	{
		iDir = atoi(szDir);
	}
	else
	{
		iDir = 0;
	}

	iItemID = atoi(szItemID);
	iNum = atoi(szNum);

	pstItemDef = z_find_itemdef(pstEnv, iItemID);
	if (NULL == pstItemDef || ITEM_TMP_NO == pstItemDef->ItemTmp.Type)
	{
		return -1;
	}

	stMapIdx.iID = pstPlayer->stRoleData.Map;
	stMapIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &stMapIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}
	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return -1;
	}
	
	for (i=0; i<iNum; i++)
	{
		z_get_ran_pos(pstEnv, pstMapInst, pstMapIdx, &pstPlayer->stRoleData.Pos, &pstPlayer->stRoleData.Pos, 
						400, 50, &stPos);
		tmp_item_create(pstEnv,NULL, iItemID, &stMapIdx, &stPos, pstItemDef->ItemTmp.MachineEnerge, pstItemDef->ItemTmp.RefreshTime, 
							pstItemDef->ItemTmp.StayTime, iDir,0,0,NULL);
	}

	return 0;
}

int z_gm_ridetmpitem(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{

	char szItemID[32];
	int iItemID;

	char szDir[32];
	int iMapRefreshFlag;
	ITEMDEF *pstItemDef;
	ZONEIDX stMapIdx;
	RESPOS stPos;
	MapIndex * pstMapIdx;
	ZONEMAPINST *pstMapInst;
	CSPACKAGEPICK stPick;
	DropItem *pstItem = NULL;
	
	if( z_gm_get_val(&pszArg, szItemID, sizeof(szItemID))<0 )
	{
		return -1;
	}

	if (0 == z_gm_get_val(&pszArg, szDir, sizeof(szDir)))
	{
		iMapRefreshFlag = atoi(szDir);
	}
	else
	{
		iMapRefreshFlag = 0;
	}

	iItemID = atoi(szItemID);

	pstItemDef = z_find_itemdef(pstEnv, iItemID);
	if (NULL == pstItemDef || ITEM_TMP_NO == pstItemDef->ItemTmp.Type)
	{
		return -1;
	}

	stMapIdx.iID = pstPlayer->stRoleData.Map;
	stMapIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &stMapIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}
	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return -1;
	}
	

	z_get_ran_pos(pstEnv, pstMapInst, pstMapIdx, &pstPlayer->stRoleData.Pos, &pstPlayer->stRoleData.Pos, 
					400, 50, &stPos);
	
	tmp_item_create(pstEnv,NULL, iItemID, &stMapIdx, &stPos, pstItemDef->ItemTmp.MachineEnerge, pstItemDef->ItemTmp.RefreshTime, 
						pstItemDef->ItemTmp.StayTime, 0,0,0,&pstItem);
	if (!pstItem)
	{
		return -1;
	}

	if (iMapRefreshFlag > 0)
	{
		pstItem->iTmpItemFlag |= TMP_ITEM_FLAG_MAP_REFRESH;
	}

	stPick.ID = pstItem->iID;
	package_pick(pstAppCtx,pstEnv,pstPlayer, &stPick);

	return 0;
}

int z_gm_skillcool_clean(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{	
	char szSkillID[32];
	unsigned int unSkillID;
	UNUSED(pstAppCtx);

	if( z_gm_get_val(&pszArg, szSkillID, sizeof(szSkillID))<0)
	{
		unSkillID = 0;
	}
	else
	{
		unSkillID = atoi(szSkillID);
	}
	
	return player_skill_cool_clean(pstEnv, pstPlayer, unSkillID);
}
int z_gm_clan_make_clear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	int iClanID;
	char szID[32];
	int iID;
	CLANPRODUCE *pstProduce ;
	ZoneClan *pstClan;
	CLANRESOURCE *pstResource;

	if( z_gm_get_val(&pszArg, szID, sizeof(szID))<0)
	{
		return -1;
	}

	iID =atoi(szID);

	if( z_gm_get_val(&pszArg, szID, sizeof(szID))<0)
	{
		return -1;
	}

	iClanID = atoi(szID);

	if (iClanID == 0)
	{
		pstClan = player_get_clan(pstEnv,  pstPlayer);
	}
	else
	{
		pstClan = z_id_clan(pstEnv, iClanID);
	}

	if (!pstClan)
		return -1;

	pstProduce = &pstClan->stClan.ClanAdvance.Produce;
	pstResource = &pstClan->stClan.ClanAdvance.Resource;

	pstProduce->ArmMakingNum = 0;
	memset(&pstProduce->ArmMakingProgress[0], 0, sizeof(pstProduce->ArmMakingProgress));
	pstResource->ItemNum = 0;
	memset(&pstResource->ItemInfo[0], 0, sizeof(pstResource->ItemInfo));

	CSPKG stPkg;
	stPkg.Body.ClanSvr.Operation = CLAN_SVR_ADVANCE_SNAPSHOT;
	stPkg.Body.ClanSvr.ClanSvrData.ClanAdvanceSnapshot.Snapshot =pstClan->stClan.ClanAdvance;

	Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
	player_clan_broadcast(pstEnv, pstClan, &stPkg, 0);
	return 0;
}

int z_gm_clan_make_clean_by_id(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szID[32];
	int iID;
	int iClanID;
	CLANPRODUCE *pstProduce ;
	ZoneClan *pstClan;
	CLANRESOURCE *pstResource;

	if( z_gm_get_val(&pszArg, szID, sizeof(szID))<0)
	{
		return -1;
	}

	iID =atoi(szID);

	if( z_gm_get_val(&pszArg, szID, sizeof(szID))<0)
	{
		return -1;
	}

	iClanID = atoi(szID);

	if (iClanID == 0)
	{
		pstClan = player_get_clan(pstEnv,  pstPlayer);
	}
	else
	{
		pstClan = z_id_clan(pstEnv, iClanID);
	}

	if (!pstClan)
		return -1;

	pstProduce = &pstClan->stClan.ClanAdvance.Produce;
	pstResource = &pstClan->stClan.ClanAdvance.Resource;
	int i;
	int iNotify = 0;

	for (i=0; i<pstProduce->ArmMakingNum; i++)
	{
		if (pstProduce->ArmMakingProgress[i].ID == iID)
		{
			pstProduce->ArmMakingNum -- ;
			if (pstProduce->ArmMakingNum != i)
			{
				memmove(&pstProduce->ArmMakingProgress[i], &pstProduce->ArmMakingProgress[i+1],
		        			sizeof(pstProduce->ArmMakingProgress[0]) *(pstProduce->ArmMakingNum-i));
			}

			iNotify = 1;
			break;
		}
	}

	for (i=0; i<pstResource->ItemNum; i++)
	{
		if (pstResource->ItemInfo[i].ResourceID == iID)
		{
			pstResource->ItemNum --;
			if (i != pstResource->ItemNum)
			{
				pstResource->ItemInfo[i] = pstResource->ItemInfo[pstResource->ItemNum];
			}

			memset(&pstResource->ItemInfo[pstResource->ItemNum], 0, 
					sizeof(pstResource->ItemInfo[pstResource->ItemNum]));
			iNotify = 1;
			break;
		}
	}

	if (iNotify)
	{
		CSPKG stPkg;
		stPkg.Body.ClanSvr.Operation = CLAN_SVR_ADVANCE_SNAPSHOT;
		stPkg.Body.ClanSvr.ClanSvrData.ClanAdvanceSnapshot.Snapshot =pstClan->stClan.ClanAdvance;

		Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
		player_clan_broadcast(pstEnv, pstClan, &stPkg, 0);
	}
	return 0;
}

static int gm_clan_city(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan,
	                      const char *pszArg)
{	
	char szOpt[CLT_MSG_LEN];
	char *pszAttr, szAttr[CLT_MSG_LEN];
	int iLen, iVal;
	CLANCITY *pszClanCity = &pstZoneClan->stClan.ClanCity;
	UNUSED(pstEnv);
	
	while(!z_gm_get_val(&pszArg, szOpt, sizeof(szOpt)))
	{
		pszAttr = strchr(szOpt, '=');
		if(!pszAttr)
		{
			return -1;
		}

		iLen = pszAttr - szOpt;
		memcpy(szAttr, szOpt, iLen);
		szAttr[iLen] = 0;
		pszAttr++;
		iVal = atoi(pszAttr);

		if(strcasecmp(szAttr, "level") == 0)
		{
			pszClanCity->Level += iVal;
		}
		/*
		else if(strcasecmp(szAttr, "defence") == 0)
		{
			pszClanCity->DefenceValue += iVal;
		}
		else if(strcasecmp(szAttr, "business") == 0)
		{
			pszClanCity->BusinessValue += iVal;
		}
		else if(strcasecmp(szAttr, "development") == 0)
		{
			pszClanCity->DevelopmentValue += iVal;
		}
		else if(strcasecmp(szAttr, "technology") == 0)
		{
			pszClanCity->TechnologyValue += iVal;
		}
		*/
		else
		{
			return -1;
		}
		clan_set_dirty_notify(pstZoneClan, CACHE_FLAG_IMM);
	}
	return 0;
}

/* 
  公会GM指令
  注: <公会ID> 0 表示自己的公会, 否则表示其他公会
  创建公会:
  	clan create
  解散公会 :
	clan <公会ID> del
  加减公会属性 : 
  	clan <公会ID> addattr <money|level|stable|active|tech|used-tech>=<[+|-]整数>
  	money: 资金
  	level: 等级
  	stable: 安定度
  	active: 繁荣度
  	tech: 科技点
  	used-tech: 已用科技点
  加减公会城市的属性 :
  	clan <公会ID> addaddr city <level>=<[+|-]整数>
  升级公会
  	clan <公会ID> upgrade
  升级公会城市
  	clan <公会ID> upgrade city [公会城市名]
  升级公会建筑
  	clan <公会ID> upgrade building <建筑ID>
  	<建筑ID>:
  	0: 科技中心
  	1: 采集中心
  	2: 工厂
  	3: 兽栏
  	4: 城防
  查看公会基本信息
  	clan <公会ID> info
  查看公会城市基本信息
  	clan <公会ID> info city
  进入公会城市(只能进入自己的公会城市)
  	clan home
  重建公会城市副本
  	clan <公会ID> doom
  技术研发
  	clan <公会ID> research <类型> <等级>
  产品制造
  	clan <公会ID> make <类型> <产品ID> <数量>
  加速研发
  	clan <公会ID> research-speedup <索引> <加速秒数>
  加速制造
  	clan <公会ID> making-speedup <类型> <索引> <加速秒数>
  取消研发
  	clan <公会ID> cancel-research <索引>
  取消制造
  	clan <公会ID> cancel-making <索引>
  向公会资源中心添加东西
  	clan <公会ID> resource-add-entries <DefID> <数量>
  从资源中心中删除东西
  	clan <公会ID> resource-dec-entries <DefID> <数量>
  从资源中心中提取机械
  	clan <公会ID> grab-item <DefID> <数量>
  设置公会标记
  	clan <公会ID> flag <值>
 */
int z_gm_clan(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{	
	char szMsg[CLT_MSG_LEN];
	char szClanOpt[128];
	ZoneClan *pstZoneClan = NULL;
	int iID = 0;
	int iRet;
	UNUSED(pstAppCtx);

	// 获取公会ID
	if(z_gm_get_val(&pszArg, szClanOpt, sizeof(szClanOpt)) < 0)
	{
		return -1;
	}

	if (strcasecmp(szClanOpt, "create") == 0)
	{
		snprintf(szMsg, CCH(szMsg), "%s的公会",pstPlayer->stRoleData.RoleName);
		
		return player_clan_create(pstEnv,pstPlayer,szMsg,0);
	}
	

	// 回家
	if(strcasecmp(szClanOpt, "home") == 0)
	{
		return clan_mem_enter_city(pstEnv, pstPlayer, NULL);
	}
	// 用ID查询
	else if(strcasecmp(szClanOpt, "find-by-id") == 0)
	{
		CSCLANFINDREQ stReq;
		char szOpt[128];

		if(z_gm_get_val(&pszArg, szOpt, sizeof(szOpt)) < 0)
		{
			return -1;
		}

		stReq.ByWhat = FIND_CLAN_BY_ID;
		stReq.Key.ID = atoi(szOpt);

		return player_clan_find_req(pstEnv, pstPlayer, &stReq);
	}
	// 用字符串查询
	else if(strcasecmp(szClanOpt, "find-by-name") == 0)
	{
		CSCLANFINDREQ stReq;
		char szOpt[128];

		if(z_gm_get_val(&pszArg, szOpt, sizeof(szOpt)) < 0)
		{
			return -1;
		}

		stReq.ByWhat = FIND_CLAN_BY_NAME;
		STRNCPY(stReq.Key.Name, szOpt, CCH(stReq.Key.Name));

		return player_clan_find_req(pstEnv, pstPlayer, &stReq);
	}

	// 获取公会对象
	iID = atoi(szClanOpt);
	if (!iID)
	{
		pstZoneClan = player_get_clan(pstEnv, pstPlayer);
	}
	else
	{
		pstZoneClan = z_id_clan(pstEnv, iID);
	}
	if(!pstZoneClan)
	{
		return -1;
	}

	// 获取操作选项
	if(z_gm_get_val(&pszArg, szClanOpt, sizeof(szClanOpt))<0)
	{
		return -1;
	}

	if(strcasecmp(szClanOpt, "del") == 0)
	{
		// 标记删除公会
		pstZoneClan->stZoneClanInfo.fWillDelayDestroy = 1;
		pstZoneClan->stZoneClanInfo.iDirty = CACHE_FLAG_INEXIST;

		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, 
						ZONEERR_GM7, pstZoneClan->stClan.ClanProf.Name);
	}
	else if (strcasecmp(szClanOpt, "quickdel") == 0)
	{
		clan_disband(pstEnv, pstZoneClan,pstPlayer->stRoleData.RoleName);
	}
	else if(strcasecmp(szClanOpt, "add") == 0)
	{
		Player *pstOther = NULL;
		// 获取角色名
		if(z_gm_get_val(&pszArg, szClanOpt, sizeof(szClanOpt)) < 0)
		{
			return -1;
		}
		pstOther = z_name_player(pstAppCtx, pstEnv, szClanOpt);
		if(!pstOther)
		{
			return -1;
		}
		if(player_clan_join_clan(pstEnv, pstOther, pstZoneClan) < 0)
		{
			return -1;
		}
	}
	else if(strcasecmp(szClanOpt, "doom") == 0)
	{
		if(!pstZoneClan->stClan.ClanCity.Level)
		{
			return -1;
		}
		// 重建公会城市副本
		clan_destroy_city(pstEnv, pstZoneClan);
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR,
				       ZONEERR_GM8);
		if(z_new_clan_city(pstEnv, pstZoneClan) < 0)
		{
			return -1;
		}
	}
	else if(strcasecmp(szClanOpt, "info") == 0)
	{
		szClanOpt[0] = 0;
		z_gm_get_val(&pszArg, szClanOpt, sizeof(szClanOpt));
		if(strcasecmp(szClanOpt, "city") == 0)
		{
			// TODO
		}
		else
		{
			CLAN *pstClan = &pstZoneClan->stClan;
			
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_GM9,
							     pstClan->ClanProf.Name,
							     pstClan->ClanProf.Level,
							     pstClan->ClanProf.Money,
							     pstClan->ClanProf.StableValue,
							     pstClan->ClanProf.ActiveValue,
							     pstClan->ClanProf.TechPoint,
							     pstClan->ClanAdvance.UsedTechPoint,
							     pstClan->ClanProf.MemNum);
		}
		
	}
	else if(strcasecmp(szClanOpt, "upgrade") == 0)
	{
		szClanOpt[0] = 0;
		z_gm_get_val(&pszArg, szClanOpt, sizeof(szClanOpt));
		if(strcasecmp(szClanOpt, "city") == 0)
		{
			if(z_gm_get_val(&pszArg, szClanOpt, sizeof(szClanOpt)) < 0)
			{
				return clan_city_upgrade(pstEnv, pstZoneClan, pstPlayer, NULL);
			}
			else
			{
				return clan_city_upgrade(pstEnv, pstZoneClan, pstPlayer,
				                         szClanOpt);
			}
		}
		else if(strcasecmp(szClanOpt, "building") == 0)
		{
			if(z_gm_get_val(&pszArg, szClanOpt, sizeof(szClanOpt)) < 0)
			{
				return -1;
			}
			return clan_city_building_upgrade(pstEnv, pstZoneClan, pstPlayer,
				                              atoi(szClanOpt));
		}
		else
		{
			return clan_upgrade(pstEnv, pstZoneClan, pstPlayer);
		}
	}
	else if(strcasecmp(szClanOpt, "addattr") == 0)
	{
		char *pszAttr = NULL, szAttr[128], szOpt[128];
		int iLen = 0, iVal = 0;

		while(!z_gm_get_val(&pszArg, szOpt, sizeof(szOpt)))
		{
			if(strcasecmp(szOpt, "city") == 0)
			{
				return gm_clan_city(pstEnv, pstZoneClan, pszArg);
			}
			
			pszAttr = strchr(szOpt, '=');
			if(!pszAttr)
			{
				return -1;
			}

			iLen = pszAttr - szOpt;
			memcpy(szAttr, szOpt, iLen);
			szAttr[iLen] = 0;
			pszAttr++;
			iVal = atoi(pszAttr);

			g_iOptype = OP_OPTYPE_GM;
			if(strcasecmp(szAttr, "money") == 0)
			{
				pstZoneClan->stZoneClanInfo.iListVersion++;
				iRet = clan_property_adapt(pstEnv, pstZoneClan,
				                           CLAN_PROPERTY_MONEY, iVal);
				g_iOptype = 0;
				return iRet;
			}
			else if(strcasecmp(szAttr, "level") == 0)
			{
				pstZoneClan->stZoneClanInfo.iListVersion++;
				return clan_property_adapt(pstEnv, pstZoneClan,
				                           CLAN_PROPERTY_LEVEL, iVal);
			}
			else if(strcasecmp(szAttr, "flag") == 0)
			{
				pstZoneClan->stZoneClanInfo.iListVersion++;
				return clan_property_modify(pstEnv, pstZoneClan,
				                           CLAN_PROPERTY_FLAG, iVal,-1);
			}
			else if(strcasecmp(szAttr, "stable") == 0)
			{
				pstZoneClan->stZoneClanInfo.iListVersion++;
				return clan_property_adapt(pstEnv, pstZoneClan,
				                           CLAN_PROPERTY_STABLE_VALUE, iVal);
			}
			else if(strcasecmp(szAttr, "active") == 0)
			{
				pstZoneClan->stZoneClanInfo.iListVersion++;
				return clan_property_adapt(pstEnv, pstZoneClan,
				                           CLAN_PROPERTY_ACTIVE_VALUE, iVal);
			}
			else if(strcasecmp(szAttr, "tech") == 0)
			{
				pstZoneClan->stZoneClanInfo.iListVersion++;
				iRet = clan_property_adapt(pstEnv, pstZoneClan,
				                           CLAN_PROPERTY_TECH_POINT, iVal);
				if (iRet == 0 && iVal > 0)
				{
					pstZoneClan->stClan.ClanBoon.Cond.Tech += iVal;
					clan_boon_condition_chg(pstEnv, pstZoneClan);
				}
				return iRet;
			}
			else if(strcasecmp(szAttr, "used-tech") == 0)
			{
				pstZoneClan->stZoneClanInfo.iListVersion++;
				return clan_property_adapt(pstEnv, pstZoneClan,
				                           CLAN_PROPERTY_USED_TECH_POINT, iVal);
			}
			return -1;
		}
	}
	else if(strcasecmp(szClanOpt, "research") == 0)
	{
		char szOpt[128];
		CSCLANRESEARCH stReq;

		if(z_gm_get_val(&pszArg, szOpt, sizeof(szOpt)) < 0)
		{
			return -1;
		}
		stReq.Category = atoi(szOpt);

		if(z_gm_get_val(&pszArg, szOpt, sizeof(szOpt)) < 0)
		{
			return -1;
		}
		stReq.Level = atoi(szOpt);

		return player_clan_research(pstEnv, pstPlayer, &stReq);
	}
	else if(strcasecmp(szClanOpt, "make") == 0)
	{
		char szOpt[128];
		CSCLANMAKE stReq;

		if(z_gm_get_val(&pszArg, szOpt, sizeof(szOpt)) < 0)
		{
			return -1;
		}
		stReq.Category = atoi(szOpt);

		if(z_gm_get_val(&pszArg, szOpt, sizeof(szOpt)) < 0)
		{
			return -1;
		}
		stReq.ItemOrMachineID = atoi(szOpt);

		if(z_gm_get_val(&pszArg, szOpt, sizeof(szOpt)) < 0)
		{
			return -1;
		}
		stReq.Num = atoi(szOpt);
		return player_clan_make(pstEnv, pstPlayer, &stReq);
	}
	else if(strcasecmp(szClanOpt, "research-speedup") == 0)
	{
		char szOpt[128];
		CSCLANRESEARCHSPEEDUP stReq;

		if(z_gm_get_val(&pszArg, szOpt, sizeof(szOpt)) < 0)
		{
			return -1;
		}
		stReq.ProgressPos = atoi(szOpt);

		if(z_gm_get_val(&pszArg, szOpt, sizeof(szOpt)) < 0)
		{
			return -1;
		}
		stReq.DecreasingTime = atoi(szOpt);
		return player_clan_research_speedup(pstEnv, pstPlayer, &stReq);
	}
	else if(strcasecmp(szClanOpt, "cancel-research") == 0)
	{
		char szOpt[128];
		CSCLANCANCELRESEARCH stReq;

		if(z_gm_get_val(&pszArg, szOpt, sizeof(szOpt)) < 0)
		{
			return -1;
		}
		stReq.ProgressPos = atoi(szOpt);
		return player_clan_cancel_research(pstEnv, pstPlayer, &stReq);
	}
	else if(strcasecmp(szClanOpt, "making-speedup") == 0)
	{
		char szOpt[128];
		CSCLANMAKINGSPEEDUP stReq;

		if(z_gm_get_val(&pszArg, szOpt, sizeof(szOpt)) < 0)
		{
			return -1;
		}
		stReq.Category = atoi(szOpt);

		if(z_gm_get_val(&pszArg, szOpt, sizeof(szOpt)) < 0)
		{
			return -1;
		}
		stReq.ProgressPos = atoi(szOpt);

		if(z_gm_get_val(&pszArg, szOpt, sizeof(szOpt)) < 0)
		{
			return -1;
		}
		stReq.DecreasingTime = atoi(szOpt);
		return player_clan_making_speedup(pstEnv, pstPlayer, &stReq);
	}
	else if(strcasecmp(szClanOpt, "cancel-making") == 0)
	{
		char szOpt[128];
		CSCLANCANCELMAKING stReq;

		if(z_gm_get_val(&pszArg, szOpt, sizeof(szOpt)) < 0)
		{
			return -1;
		}
		stReq.ProgressPos = atoi(szOpt);
		return player_clan_cancel_making(pstEnv, pstPlayer, &stReq);
	}	
	else if(strcasecmp(szClanOpt, "resource-add-entries") == 0)
	{
		char szOpt[128];
		int iNum = 0, iDefID = 0;

		if(z_gm_get_val(&pszArg, szOpt, sizeof(szOpt)) < 0)
		{
			return -1;
		}
		iDefID = atoi(szOpt);

		if(z_gm_get_val(&pszArg, szOpt, sizeof(szOpt)) < 0)
		{
			return -1;
		}
		iNum = atoi(szOpt);

		return clan_resource_add_entries(pstEnv, pstZoneClan, iDefID,
		                                iNum,NULL);
	}
	else if(strcasecmp(szClanOpt, "resource-dec-entries") == 0)
	{
		char szOpt[128];
		int iNum = 0, iDefID = 0;

		if(z_gm_get_val(&pszArg, szOpt, sizeof(szOpt)) < 0)
		{
			return -1;
		}
		iDefID = atoi(szOpt);

		if(z_gm_get_val(&pszArg, szOpt, sizeof(szOpt)) < 0)
		{
			return -1;
		}
		iNum = atoi(szOpt);

		return clan_resource_dec_entries(pstEnv, pstZoneClan, iDefID,
		                                iNum);

	}
	else if(strcasecmp(szClanOpt, "grab-item") == 0)
	{
		char szOpt[128];
		CSCLANRESOURCEGRABITEM stReq;

		if(z_gm_get_val(&pszArg, szOpt, sizeof(szOpt)) < 0)
		{
			return -1;
		}
		
		stReq.ResourceID = atoi(szOpt);

		if(z_gm_get_val(&pszArg, szOpt, sizeof(szOpt)) < 0)
		{
			return -1;
		}
		
		stReq.Num = atoi(szOpt);
		
		return player_clan_resource_grab_item(pstEnv, pstPlayer, &stReq);
	}
	if(strcasecmp(szClanOpt, "clear-everydaypworld") == 0)
	{
		pstZoneClan->stClan.ClanInner.EveryDayPworldTime = 0;
		clan_destroy_everyday_pworld(pstEnv, pstZoneClan);
	}
	else
	{
		return -1;
	}

	return 0;
}

static ConfEntry * gm_find_conf( const char * pszKey )
{
	ConfEntry * pstEntry = g_astLookupConf;

	while( pstEntry->pszName != NULL )
	{
		if( strcmp( pszKey, pstEntry->pszName ) == 0 )
		{
			return pstEntry;
		}
		pstEntry++;
	}

	return NULL;
}

// conf set NAME VALUE
// conf get NAME
// conf list
int z_gm_conf(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	
	char szMsg[CLT_MSG_LEN];
	char szEntry[128];
	ConfEntry * pstEntry = NULL;
	UNUSED(pstAppCtx);

	if( z_gm_get_val( &pszArg, szEntry, CCH(szEntry) ) < 0 )
	{
		return -1;
	}

	if( strcasecmp( szEntry, "set" ) == 0 )
	{
		int iOldVal = 0;
		
		// key
		if( z_gm_get_val( &pszArg, szEntry, CCH(szEntry) ) < 0 )
		{
			return -1;
		}

		// entry
		if( ( pstEntry = gm_find_conf( szEntry ) ) == NULL )
		{
			return -1;
		}

		// value
		if( z_gm_get_val( &pszArg, szEntry, CCH(szEntry) ) < 0 )
		{
			return -1;
		}

		iOldVal = *(pstEntry->piVal);
		*(pstEntry->piVal) = atoi( szEntry );

		snprintf( szMsg, CCH(szMsg), "conf : %s | %d -> %d", pstEntry->pszName, 
			iOldVal, *(pstEntry->piVal) );
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_GM10,  
							pstEntry->pszName, 
							iOldVal, *(pstEntry->piVal));		
	}
	else if( strcasecmp( szEntry, "get" ) == 0 )
	{
		// key
		if( z_gm_get_val( &pszArg, szEntry, CCH(szEntry) ) < 0 )
		{
			return -1;
		}

		// entry
		if( ( pstEntry = gm_find_conf( szEntry ) ) == NULL )
		{
			return -1;
		}

		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, 
							ZONEERR_GM11,  pstEntry->pszName, *(pstEntry->piVal));
	}
	else if( strcasecmp( szEntry, "list" ) == 0 )
	{
		pstEntry = g_astLookupConf;
		
		while( pstEntry->pszName != NULL )
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_GM11, 
						pstEntry->pszName, *(pstEntry->piVal) );
			pstEntry++;
		}
	}
	
	return 0;
}

int z_gm_aas_cmd(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szAttr[32];

	UNUSED(pszArg);
	UNUSED(pstAppCtx);

	if( z_gm_get_val( &pszArg, szAttr, CCH(szAttr) ) < 0 )
	{
		return -1;
	}

	if( strcasecmp( szAttr, "set" ) == 0 )
	{
		char szOnlineTime[32];
		char szOfflineTime[32];
		int iSrcID = pstEnv->pstAppCtx->iId;
		int iDstID = pstEnv->iRegionGateID;
		char szBuff[sizeof(SSPKG)];
		TDRDATA stNet;
		SSPKG stPkgReq;
		SSAASREQ *pstSSAasReq = &stPkgReq.Body.AasReq;
	
		if( z_gm_get_val(&pszArg, szOnlineTime, sizeof(szOnlineTime))<0 ||
			z_gm_get_val(&pszArg, szOfflineTime, sizeof(szOfflineTime))<0)
		{
			return -1;
		}

		pstSSAasReq->Uin = pstPlayer->stRoleData.Uin;
		pstSSAasReq->SvrID = iSrcID;
		pstSSAasReq->tOnline = atoi(szOnlineTime);
		pstSSAasReq->tOffline = atoi(szOfflineTime);
		pstSSAasReq->ActType = AAS_TIME_SET;

		Z_SSHEAD_INIT(&stPkgReq.Head, AAS_GAME_REQ);
		
		stNet.pszBuff = szBuff;
		stNet.iBuff = sizeof(szBuff);
		if (0 > ss_pack(pstEnv->pstSSHan, &stNet, &stPkgReq, 0))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "ss_pack_err");
			return -1;
		}
		
		if (0 > tbus_forward(pstEnv->pstAppCtx->iBus, &iSrcID, &iDstID, stNet.pszBuff, stNet.iBuff, 0))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tbus_send fail");
			return -1;
		}
	}
	else if( strcasecmp( szAttr, "print" ) == 0 )
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_SVR, 
			"在线时间= %d 离线时间=%d 提醒=%d", pstPlayer->stAasTime.tOnline, 
			pstPlayer->stAasTime.tOffline, pstPlayer->stAasTime.tNotify);
	}
	else
	{
		return -1;
	}
	
	return 0;
}

extern int player_task_share(ZONESVRENV * pstEnv, Player * pstPlayer, 
	int iNpcID, int iTaskID);

int z_gm_sharetask(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, 
	const char* pszArg)
{
	char szEntry[128];
	int iTaskID;

	UNUSED( pstAppCtx );
	UNUSED( pstEnv );
	
	if( z_gm_get_val( &pszArg, szEntry, CCH(szEntry) ) < 0 )
	{
		return -1;
	}

	if( strcasecmp( szEntry, "refuse" ) == 0 )
	{
		pstPlayer->stOnline.iSharingTaskID = 0;
		pstPlayer->stOnline.tSharingTime = 0;
	}
	else
	{
		iTaskID = atoi( szEntry );

		return player_task_share( pstEnv, pstPlayer, 0, iTaskID );
	}
	
	return 0;
}

extern int player_role_logout(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, 
	Player *pstPlayer, TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);
extern int player_cancel_role_logout(TAPPCTX * pstAppCtx, ZONESVRENV * pstEnv, 
	Player * pstPlayer, TFRAMEHEAD * pstFrameHead, CSPKG * pstCsPkg);

int z_gm_delaylogout(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, 
	const char* pszArg)
{
	char szEntry[128];

	if( z_gm_get_val( &pszArg, szEntry, CCH(szEntry) ) < 0 )
	{
		return -1;
	}

	if( atoi( szEntry ) )
	{
		return player_role_logout( pstAppCtx, pstEnv, pstPlayer, NULL, NULL );
	}
	else
	{
		return player_cancel_role_logout( pstAppCtx, pstEnv, pstPlayer, NULL, NULL );
	}

	return 0;
}

static unsigned int htou(const char *p)
{
	int n = !p ? 0 : strlen(p);
	unsigned int ret = 0U, offset = 0U;

	if(n > 8)
	{
		ret = 0xffffffff;
		goto out;
	}

	while(n--)
	{
		if(p[n] >= '0' && p[n] <= '9')
			ret += ((p[n] - '0') << offset);
		else if(p[n] >= 'a' && p[n] <= 'f')
			ret += ((p[n] - 'a' + 10) << offset);
		else if(p[n] >= 'A' && p[n] <= 'F')
			ret += ((p[n] - 'A' + 10) << offset);
		else
		{
			ret = 0U;
			goto out;
		}
		offset += 4;
	}
	
out:
	return ret;
}


int z_gm_clan_pet_jiyang(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, 
	                const char* pszArg)
{
	char szIdx[16];
	int iIdx;
	int iHour;
	PETPACKAGE *pstPetPkg = &pstPlayer->stRoleData.Package.PetPackage;
	CSCLANPETJIYANG stJiYang;

	if(z_gm_get_val(&pszArg, szIdx, CCH(szIdx)) < 0)
	{
		return -1;
	}

	iIdx = atoi(szIdx);
	if (iIdx<0 || iIdx>= pstPetPkg->Num)
		return -1;

	if(z_gm_get_val(&pszArg, szIdx, CCH(szIdx)) < 0)
	{
		return -1;
	}

	iHour = atoi(szIdx);

	if(z_gm_get_val(&pszArg, szIdx, CCH(szIdx)) < 0)
	{
		return -1;
	}

	stJiYang.Idx = atoi(szIdx);
	stJiYang.Hour = iHour;
	stJiYang.WID = pstPetPkg->RolePets[iIdx].WID;
	clan_pet_jiyang(pstEnv, pstPlayer, &stJiYang);
	return 0;
}

int z_gm_clan_pet_jiyang_end(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, 
	                const char* pszArg)
	                {

	char szIdx[16];
	int iIdx;
	PETPACKAGE *pstPetPkg = &pstPlayer->stRoleData.Package.PetPackage;	
	CSCLANPETJIYANGEND stJiYang;

	if(z_gm_get_val(&pszArg, szIdx, CCH(szIdx)) < 0)
	{
		return -1;
	}

	stJiYang.Idx = atoi(szIdx);
	if(z_gm_get_val(&pszArg, szIdx, CCH(szIdx)) < 0)
	{
		return -1;
	}

	iIdx = atoi(szIdx);
	if (iIdx<0 || iIdx>= pstPetPkg->Num)
		return -1;
	
	stJiYang.WID = pstPetPkg->RolePets[iIdx].WID;
	clan_pet_jiyang_end(pstEnv, pstPlayer, &stJiYang);
return 0;
}

int z_gm_bulletin(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, 
	                const char* pszArg)
{
	char szEntry[256];
	BulletinBoard *pstBoard = pstEnv->pstBulletinBoard;
	const int kCurr = pstAppCtx->stCurr.tv_sec;
	
	if(z_gm_get_val(&pszArg, szEntry, CCH(szEntry)) < 0)
	{
		return -1;
	}

	if(strcasecmp(szEntry, "list" ) == 0)
	{
		int i = pstBoard->iNum;
		const ZoneBulletin *pstCurr = pstBoard->astBulletinArray;
		
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_GM12);
		while(i--)
		{
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_GM13,
				       pstCurr->stCore.ID,
				       pstCurr->iActiveEnd - kCurr,
				       pstCurr->stCore.RollInterval,
				       pstCurr->stCore.Text);
			pstCurr++;
		}
		
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_GM14, pstBoard->iNum);
	}
	else if(strcasecmp(szEntry, "add" ) == 0)
	{
		int iActiveTime = 0, iRollInterval = 0, iID = 0;
		SSBULLETIN stReq;
		char szMsg[1024];
		//char szActiveStart[64];
		int iActiveStart;
		time_t tTime;
		struct tm stTm;

		memset(&stReq, 0, sizeof(stReq));
		if(z_gm_get_val(&pszArg, szEntry, CCH(szEntry)) < 0)
		{
			return -1;
		}
		iActiveTime = atoi(szEntry);
		if(iActiveTime <= 0)
		{
			return -1;
		}

		if(z_gm_get_val(&pszArg, szEntry, CCH(szEntry)) < 0)
		{
			return -1;
		}
		iRollInterval = atoi(szEntry);
		if(iRollInterval <= 5 || iRollInterval > iActiveTime)
		{
			return -1;
		}

		if(z_gm_get_val(&pszArg, szMsg, CCH(szMsg)) < 0)
		{
			return -1;
		}

		// 激活时间 格式必须为"2011-10-20 23:55:45"
		if (NULL == strptime(pszArg, "%Y-%m-%d %H:%M:%S", &stTm))
		{
			iActiveStart = 0;
		}
		else
		{
			tTime = mktime(&stTm);
			if (tTime <= pstAppCtx->stCurr.tv_sec)
			{
				iActiveStart = 0;
			}
			else
			{
				iActiveStart = tTime;
			}
		}
		
		iID = z_gen_bulletin_id();
		stReq.ActiveTime = iActiveTime;
		stReq.Core.ID = iID;
		stReq.Core.RollInterval = iRollInterval;
		stReq.ActiveTimeStart = iActiveStart;
		STRNCPY(stReq.Core.Text, szMsg, CCH(stReq.Core.Text));

		if(z_add_bulletin(pstEnv, &stReq) < 0)
		{
			return -1;
		}
		
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_GM15, iID);
	}
	else if(strcasecmp(szEntry, "del") == 0)
	{
		if(z_gm_get_val(&pszArg, szEntry, CCH(szEntry)) < 0)
		{
			return -1;
		}

		if(z_del_bulletin(pstEnv, htou(szEntry)) < 0)
		{
			return -1;
		}
	}

	return 0;
}

int z_gm_watch(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, 
	             const char* pszArg)
{
	char szEntry[256];

	if(z_gm_get_val(&pszArg, szEntry, CCH(szEntry)) < 0)
	{
		return -1;
	}

	if(strcasecmp(szEntry, "view") == 0)
	{
		char szMsg[1024];
		int i = pstPlayer->stOnline.iVAnimate;
		int iOffset = 0;

		iOffset += snprintf(szMsg + iOffset, CCH(szMsg) - iOffset, "(%d)", i);
		while(i--)
		{
			iOffset += snprintf(szMsg + iOffset, CCH(szMsg) - iOffset, " %d",
				                  pstPlayer->stOnline.aiVAnimate[i]);
		}
		z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_SVR, szMsg);
	}
	else if(strcasecmp(szEntry, "id") == 0)
	{
		Player *pstTarget = NULL;
		
		if(z_gm_get_val(&pszArg, szEntry, CCH(szEntry)) < 0)
		{
			return -1;
		}
		
		pstTarget = z_id_player(pstAppCtx, pstEnv, atoi(szEntry));
		if(!pstTarget)
		{
			return -1;
		}

		return player_watch_role(pstEnv, pstPlayer, pstTarget);
	}
	else if(strcasecmp(szEntry, "name") == 0)
	{
		Player *pstTarget = NULL;
		
		if(z_gm_get_val(&pszArg, szEntry, CCH(szEntry)) < 0)
		{
			return -1;
		}

		pstTarget = z_name_player(pstAppCtx, pstEnv, szEntry);
		if(!pstTarget)
		{
			return -1;
		}

		return player_watch_role(pstEnv, pstPlayer, pstTarget);		
	}
	return 0;
}

int z_gm_add_ridegrid(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{	
	int iNum;
	char szNum[32];
	UNUSED(pstAppCtx);
	
	if( z_gm_get_val(&pszArg, szNum, sizeof(szNum))<0 )
	{
		return -1;
	}

	iNum = atoi(szNum);

	if (pstPlayer->stRoleData.MiscInfo.RideList.MaxNum + iNum >= MAX_RIDE_GRID)
	{
		pstPlayer->stRoleData.MiscInfo.RideList.MaxNum = MAX_RIDE_GRID;
	}
	else
	{
		pstPlayer->stRoleData.MiscInfo.RideList.MaxNum += iNum;
	}

	ride_list_chg(pstEnv, pstPlayer);
	
	return 0;
}


int z_gm_cleanbattlelimit(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{	
	LIMITBATTLETIME *pstLimit = &pstPlayer->stRoleData.MiscInfo.LimitBattleTime;

	pstLimit->Num = 0;
	pstLimit->Time = 0;
	player_battle_limit_time_action(pstEnv, pstPlayer);
	UNUSED(pstAppCtx);
	UNUSED(pszArg);
	return 0;
}


int z_gm_span_clear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	SSPKG stPkg;
	SSSPANSTRONGREQ *pstReq = &stPkg.Body.SpanStrongReq;

	pstReq->OpType = SS_SPAN_STRONG_REQ_CLEAR;
	pstReq->Data.StrongClear.MsgSrcID = pstAppCtx->iId;

	Z_SSHEAD_INIT(&stPkg.Head, SPAN_STRONG_REQ);
	return z_ssmsg_send(pstEnv, &stPkg, pstEnv->iBattleSpartaID);
}

int z_gm_span_tip(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{	
	SSPKG stPkg;
	SSSPANSTRONGREQ *pstReq = &stPkg.Body.SpanStrongReq;

	pstReq->OpType = SS_SPAN_STRONG_REQ_TIP;
	pstReq->Data.StrongTip.MsgSrcID = pstPlayer->stRoleData.WorldID;
	pstReq->Data.StrongTip.ClanID = pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Id;

	Z_SSHEAD_INIT(&stPkg.Head, SPAN_STRONG_REQ);
	return z_ssmsg_send(pstEnv, &stPkg, pstEnv->iBattleSpartaID);
}

int z_gm_span_mail(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{	
	SSPKG stPkg;
	SSSPANSTRONGREQ *pstReq = &stPkg.Body.SpanStrongReq;

	pstReq->OpType = SS_SPAN_STRONG_REQ_MAIL;
	strncpy(pstReq->Data.StrongMail.Name, pstPlayer->stRoleData.RoleName, sizeof(pstReq->Data.StrongMail.Name));
	pstReq->Data.StrongMail.SvrID = pstPlayer->stRoleData.WorldID;

	Z_SSHEAD_INIT(&stPkg.Head, SPAN_STRONG_REQ);
	return z_ssmsg_send(pstEnv, &stPkg, pstEnv->iBattleSpartaID);
}



int z_gm_span_sale(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	SPANSTRONGSALE stSale;
	char szArgs[32];
	ZoneClan *pstClan;
	STRONGINFO *pstStrongInfo;
	int i;

	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}
	stSale.Money = atoi(szArgs);

	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}
	stSale.StrongID = atoi(szArgs);

	pstClan = player_get_clan( pstEnv,  pstPlayer);
	if (!pstClan)
	{
		return 0;
	}

	pstStrongInfo = &pstClan->stClan.ClanInner.StrongInfo;
	for (i=pstStrongInfo->SpanStrongNum -1; i>=0; i--)
	{
		if ( pstStrongInfo->SpanSaleInfo[i].StrongID == stSale.StrongID)
		{
			pstStrongInfo->SpanStrongNum --;
			if (i != pstStrongInfo->SpanStrongNum)
			{
				pstStrongInfo->SpanSaleInfo[i] = pstStrongInfo->SpanSaleInfo[pstStrongInfo->SpanStrongNum];
			}
			continue;
		}	
	}
	span_strong_clt_sale(pstEnv,pstPlayer, &stSale);
	return 0;
}

int z_gm_span_occ(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	int iID;
	char szID[32];
	ZoneClan *pstClan;

	if(0 > z_gm_get_val(&pszArg, szID, sizeof(szID)))
	{
		return -1;
	}

	iID = atoi(szID);

	pstClan = player_get_clan(pstEnv,  pstPlayer);
	if (!pstClan)
	{
		return -1;
	}

	span_strong_gm_occ_ss_req(pstEnv, pstClan, iID);
	return 0;
}

int z_gm_span_enter(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	SPANSTRONGENTER stEnter;
	char szArgs[32];

	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}

	stEnter.StrongID = atoi(szArgs);
	
	span_strong_clt_enter(pstEnv, pstPlayer, &stEnter);
	return 0;
}

int z_gm_span_add_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	int iRes;
	char szArgs[32];
	ZONEMAPINST *pstMInst;
	ZONEPWORLD *pstPworld;
	RSTRONGONE *pstOne;
	ZONEIDX stIdx;
	int iCamp = 0;
	
	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}

	iRes = atoi(szArgs);

	pstPworld = z_find_pworld2(pstEnv, pstPlayer->stRoleData.Map);
	if (!pstPworld || pstPworld->stPworldDef.StrongMapID <= 0 || pstPworld->stPworldDef.PolyType != PWORLD_POLY_SPAN_STRONG)
		return -1;

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = -1;
	pstMInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (!pstMInst)
		return -1;

	pstOne =  span_strong_one_find(pstEnv, pstPworld->stPworldDef.StrongMapID);
	if (!pstOne || pstOne->IsOpen == STRONG_CLOSE)
		return 0;

	iCamp = pstPlayer->stOnline.stWarOL.cCampIdx;

	if (iCamp  == 1)
	{
		pstOne->Pworld.DefRes += iRes;
		if (pstOne->Pworld.DefRes > 20000)
		{
			pstOne->Pworld.DefRes = 20000;
		}
	}
	else if (iCamp  == 2)
	{
		pstOne->Pworld.AtkRes += iRes;
		if (pstOne->Pworld.AtkRes > 20000)
		{
			pstOne->Pworld.AtkRes = 20000;
		}
	}
	else
	{
		return -1;
	}

	span_strong_pworld_info( pstEnv, NULL, 0, pstMInst, pstPworld->stPworldDef.StrongMapID);
	return 0;
}

int z_gm_strong_add_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	int iRes;
	char szArgs[32];
	ZONEMAPINST *pstMInst;
	ZONEPWORLD *pstPworld;
	STRONGPOINTONE* pstOne;
	//int i;
	ZONEIDX stIdx;
	ZoneClan *pstClan;
	
	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}

	iRes = atoi(szArgs);

	pstPworld = z_find_pworld2(pstEnv, pstPlayer->stRoleData.Map);
	if (!pstPworld || pstPworld->stPworldDef.StrongMapID <= 0)
		return -1;

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = -1;
	pstMInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (!pstMInst)
		return -1;

	pstOne = strong_point_one_get( pstEnv, pstPworld->stPworldDef.StrongMapID);
	if (!pstOne)
	{
		return -1;
	}

	pstClan = player_get_clan(pstEnv, pstPlayer);
	if (!pstClan)
		return -1;
	if (pstClan->stClan.ClanProf.Id == pstOne->AtkClanID)
	{
		pstOne->Pworld.AtkRes += iRes;
	}
	else if (pstClan->stClan.ClanProf.Id == pstOne->OwerID)
	{
		pstOne->Pworld.DefRes += iRes;
	}
	else
	{
		return -1;
	}

	strong_svr_pworld_info(pstEnv, NULL, 0, pstMInst, pstPworld->stPworldDef.StrongMapID);
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_mon_task_fini(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{	
	char szArgs[32];
	int iTaskID, i;
	TASKDEF *pstTaskDef;
	TASKRUNONE *pstTaskRunOne;
	UNUSED(pstAppCtx);

	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}

	iTaskID = atoi(szArgs);
	if(iTaskID <= 0)
	{
		return -1;
	}

	pstTaskRunOne = task_run_find(pstPlayer, iTaskID);
	if (NULL == pstTaskRunOne)
	{	
		return -1;
	}

	pstTaskDef = task_def_find(pstEnv, pstTaskRunOne->TaskID, &pstTaskRunOne->TaskIdx);
	if (NULL == pstTaskDef)
	{
		return -1;
	}

	for (i=0; i<pstTaskRunOne->TaskMonTrace.TaskMonTraceNum; i++)
	{
		pstTaskRunOne->TaskMonTrace.TaskKillMons[i].Num = pstTaskRunOne->TaskMonTrace.TaskKillMons[i].MaxNum;
	}

	if (0 == task_check_fini(pstEnv, pstPlayer, pstTaskRunOne, pstTaskDef))
	{
		return -1;
	}
	
	task_chg(pstEnv, pstPlayer, pstTaskRunOne);
	
	return 0;
}

int z_gm_designation(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer,
                     const char* pszArg)
{	
	char szArgs[32], szMsg[CLT_MSG_LEN];
	UNUSED(pstAppCtx);

	if(z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)) < 0)
	{
		return -1;
	}

	if(strcasecmp(szArgs, "add") == 0)
	{
		unsigned int iID = 0;
		if(z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)) < 0)
		{
			return -1;
		}

		iID = atoi(szArgs);
		return player_designation_add(pstEnv, pstPlayer, iID);
	}
	else if(strcasecmp(szArgs, "list") == 0)
	{
		ROLEDESIGNATIONS *pstInfo = &pstPlayer->stRoleData.MiscInfo.DesignationInfo;
		unsigned int i = pstInfo->Num;

		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_GM16);
		while(i--)
		{
			const DESIGNATIONDEF *pstDef = z_designation_def(pstEnv,
			                                             pstInfo->Designations[i].ID);
			if(!pstDef)
			{
				continue;
			}
			snprintf(szMsg, CCH(szMsg), "%u %s", pstDef->ID, pstDef->Name);
			z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, szMsg);
		}
		
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_GM17, pstInfo->Num);
	}
	else if(strcasecmp(szArgs, "use") == 0)
	{
		unsigned int iID = 0;
		if(z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)) < 0)
		{
			return -1;
		}

		iID = atoi(szArgs);
		return player_designation_set(pstEnv, pstPlayer, iID);
	}
	else if(strcasecmp(szArgs, "cool") == 0)
	{
		ROLEDESIGNATIONS *pstInfo = &pstPlayer->stRoleData.MiscInfo.DesignationInfo;
		unsigned int i = pstInfo->Num;

		while(i--)
		{
			pstInfo->Designations[i].LastUseTime = 0;
		}
	}
	else if(strcasecmp(szArgs, "del") == 0)
	{
		unsigned int iID = 0;
		if(z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)) < 0)
		{
			return -1;
		}

		iID = atoi(szArgs);
		return player_designation_del(pstEnv, pstPlayer, iID);
	}
	
	return 0;
}

int z_gm_respact(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer,
                 const char* pszArg)
{	
	char szArgs[32], szMsg[CLT_MSG_LEN];
	const RESPACTINFO *pstInfo = &pstPlayer->stRoleData.MiscInfo.RespactInfo;
	UNUSED(pstAppCtx);
	
	if(z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)) < 0)
	{
		return -1;
	}

	if(strcasecmp(szArgs, "add") == 0)
	{
		unsigned int nID = 0, nValue = 0;
		if(z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)) < 0)
		{
			return -1;
		}
		nID = atoi(szArgs);

		if(z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)) < 0)
		{
			return -1;
		}
		nValue = atoi(szArgs);

		return player_respact_inc(pstEnv, pstPlayer, nID, nValue, 0);
	}
	else if(strcasecmp(szArgs, "list") == 0)
	{
		unsigned int i = pstInfo->RespactPartyNum;

		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR,
		               "ID*阵营*声望");
		while(i--)
		{
			const RESPACTPARTYDEF *pstDef = z_respact_party_def(pstEnv,
			                                            pstInfo->Respact[i].ID);
			const RESPACTPHASEDEF *pstPhaseDef = 
				z_respact_phase_refer_value(pstEnv, pstInfo->Respact[i].Value);
			if(!pstDef || !pstPhaseDef)
			{
				continue;
			}
			snprintf(szMsg, CCH(szMsg), "%u*%s*%s(%u)",
			         pstDef->ID,
			         pstDef->PartyName,
			         pstPhaseDef->PhaseText,
			         pstInfo->Respact[i].Value);
			z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, szMsg);
		}
		snprintf(szMsg, CCH(szMsg), "总计%d个阵营", pstInfo->RespactPartyNum);
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, szMsg);
	}
	else if(strcasecmp(szArgs, "reset") == 0)
	{
		RESPACTENTRY *pstEntry = NULL;
		unsigned int nID = 0;
		if(z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)) < 0)
		{
			return -1;
		}
		nID = atoi(szArgs);

		pstEntry = player_find_respact(pstEnv, pstPlayer, nID);
		if(!pstEntry)
		{
			return -1;
		}

		pstEntry->Value = 0; // 声望清0
	}
	return 0;
}


int z_gm_del_event(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{	
	char szArgs[32];
	int iEventID;
	GameEventInst stEventInst, *pstEvent;
	UNUSED(pstAppCtx);UNUSED(pstPlayer);
	
	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}

	iEventID = atoi(szArgs);
	if(iEventID <= 0)
	{
		return -1;
	}

	stEventInst.iEventID = iEventID;
	pstEvent = sht_find(pstEnv->pstShtEvent, &stEventInst, EventHashCmp, EventHashCode);
	if (pstEvent)
	{
		z_game_event_del (pstEnv, pstEvent);
	}
	
	return 0;
}

int z_gm_time(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{	
	char szTmp[128];
	char szTmp1[128];

	
	time_t now;
	struct tm *pstnow;
	UNUSED(pszArg);
	now = pstAppCtx->stCurr.tv_sec;
	pstnow = localtime(&now);
	strftime (szTmp, sizeof(szTmp), "%Y-%m-%d %H:%M:%S", pstnow);

	now = pstEnv->pstGlobalObj->stGlobal.Misc.NewWorldFlags.FirstStartupTime;
	pstnow = localtime(&now);
	strftime (szTmp1, sizeof(szTmp1), "%Y-%m-%d %H:%M:%S", pstnow);

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_GM18,
					szTmp, szTmp1, pstEnv->pstGlobalObj->stGlobal.GoldRate.GoldRate);
	
	return 0;
}

int z_gm_get_item_num(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{	
	char szItemID[32];
	int iItemID;
	int iIgenore;
	int iNum;
	int iListType;
	
	if(0 > z_gm_get_val(&pszArg, szItemID, sizeof(szItemID)))
	{
		return -1;
	}
	iItemID = atoi(szItemID);

	if(0 > z_gm_get_val(&pszArg, szItemID, sizeof(szItemID)))
	{
		return -1;
	}
	iListType = atoi(szItemID);
	
	if(0 > z_gm_get_val(&pszArg, szItemID, sizeof(szItemID)))
	{
		return -1;
	}
	iIgenore = atoi(szItemID);
	
	iNum = z_get_pak_item_num(pstEnv, &pstPlayer->stRoleData.Package, iItemID, iListType,iIgenore,PACKAGE_DEC_NONE);

	z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "%d", iNum);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_up_contonline(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	update_cont_online_time(pstEnv, pstPlayer, 0);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_reload_contonline(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	pstPlayer->stRoleData.MiscInfo.ContOnlineInfo.Num = 0;
	player_comp_contonlineinfo(pstEnv, pstPlayer);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_setconti(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	int iGiftID;
	char szGiftID[32];
	MISCCONTONLINEINFO *pstContOnlineInfo = &pstPlayer->stRoleData.MiscInfo.ContOnlineInfo;
	int i=0;
	CONTONLINEGIFTDEF *pstDef=NULL;

	if(0 > z_gm_get_val(&pszArg, szGiftID, sizeof(szGiftID)))
	{
		return -1;
	}
	iGiftID = atoi(szGiftID);

	pstDef = z_find_contonlinedef(pstEnv, iGiftID);
	if (!pstDef)
		return -1;

	for (i=0; i<pstContOnlineInfo->Num; i++)
	{
		if (pstContOnlineInfo->ContOnlineInfo[i].GiftID == iGiftID)
		{
			pstContOnlineInfo->ContOnlineInfo[i].ContDay = pstDef->Day;
			pstContOnlineInfo->ContOnlineInfo[i].UpDayTm = pstAppCtx->stCurr.tv_sec;
			pstContOnlineInfo->ContOnlineInfo[i].IsGift = 1;

			role_cont_online_info(pstEnv, pstPlayer);
			return 0;
		}
	}
	return -1;
}

int z_gm_settime_by_sec(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	//int iSec;	//comment by paraunused.pl
	char szSec[32];
	//int iFlag;	//comment by paraunused.pl

	if(0 > z_gm_get_val(&pszArg, szSec, sizeof(szSec)))
	{
		return -1;
	}
	pstAppCtx->iAdjustTime  = atoi(szSec);
	
	pstAppCtx->stCurr.tv_sec = time(NULL) + pstAppCtx->iAdjustTime;
	
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstPlayer);	//add by paraunused.pl
}

int z_gm_settime(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	int i;
	time_t tTime;
	struct tm stTm;
	SHITEM *pstItem;
	GameEventInst *pstEvent;
	TMEMBLOCK *pstMemBlock;
	UinCache *pstUinCache;
		
	if (0 == strcmp(pszArg, "0"))
	{
		pstAppCtx->iAdjustTime = 0;
	}
	else
	{
		if (NULL == strptime(pszArg, "%Y-%m-%d %H:%M:%S", &stTm))
		{
			return -1;
		}

		tTime = mktime(&stTm);

		pstAppCtx->iAdjustTime = tTime - time(NULL);
	}

	pstAppCtx->stCurr.tv_sec = time(NULL) + pstAppCtx->iAdjustTime;

	z_reloadall(pstAppCtx, pstEnv);

	for (i = 0; i < pstEnv->pstShtEvent->iMax; i++) 
	{
		pstItem = SHT_GET_ITEM(pstEnv->pstShtEvent, i);
		if ( !pstItem->fValid )
		{
	            continue;
	        }
		
		pstEvent = (GameEventInst *)pstItem->szData;

		z_game_event_del (pstEnv, pstEvent);
	}
	g_tDefCheckTime = pstAppCtx->stCurr.tv_sec  + EVENT_DEF_CHECK_TIMER;

	for (i = 0; i < pstEnv->pstShtUinCache->iMax; i++) 
	{
		pstItem = SHT_GET_ITEM(pstEnv->pstShtUinCache, i);
		if ( !pstItem->fValid )
		{
	            continue;
	        }
		
		pstUinCache = (UinCache*)pstItem->szData;
		
		pstUinCache->tLastLogin = pstAppCtx->stCurr.tv_sec - 10;	
	}

	for (i=0; i<MAX_PLAYER; i++)
	{
		pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(pstEnv->pstPlayerPool, i);
		if (!pstMemBlock->fValid)
		{
			continue;
		}

		pstPlayer = (Player *)pstMemBlock->szData;

		if (PLAYER_STATUS_ROLE_LOGIN == pstPlayer->eStatus)
		{
			player_skill_cool_clean(pstEnv, pstPlayer, 0);
		}
		
		player_kickoff (pstEnv->pstAppCtx, pstEnv, pstPlayer, ZONEERR_KICK203);
	}

	pstEnv->tDirReport = pstAppCtx->stCurr.tv_sec;
	pstEnv->tCheckDB = pstAppCtx->stCurr.tv_sec;
	//同步mud时间
	z_sync_mud_time(pstEnv);
	
	return 0;
}

int z_gm_setsvrstart(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	time_t tTime;
	struct tm stTm;
	
	if (NULL == strptime(pszArg, "%Y-%m-%d %H:%M:%S", &stTm))
	{
		return -1;
	}

	tTime = mktime(&stTm);
	pstEnv->pstGlobalObj->stGlobal.Misc.NewWorldFlags.FirstStartupTime = tTime;

	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstPlayer);	//add by paraunused.pl
}

int z_gm_clear_goldrate(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	pstEnv->pstGlobalObj->stGlobal.GoldRate.GoldRate = DEFAULT_GOLD_RATE ;
	pstEnv->pstGlobalObj->stGlobal.GoldRate.GoldRateData.DataNum = 0;

	player_gold_rate_notify(pstEnv, NULL,0);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstPlayer);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_clear_goldbox_info(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	memset(&pstPlayer->stRoleData.MiscInfo.GoldRateBox, 0, sizeof(pstPlayer->stRoleData.MiscInfo.GoldRateBox));
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_clear_levfeng(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	level_feng_clear(pstEnv);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstPlayer);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}


int z_gm_armbasereset(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	ARMBASERESET stReset;

	stReset.Src.ArmGridIdx = 0;
	stReset.UseItem.ArmGridIdx = 1;
	stReset.ArmNum = 4;
	stReset.ArmStuffs[0].ArmGridIdx = 2;
	stReset.ArmStuffs[1].ArmGridIdx = 3;
	stReset.ArmStuffs[2].ArmGridIdx = 4;
	stReset.ArmStuffs[3].ArmGridIdx = 5;

	player_arm_base_reset(pstEnv,pstPlayer, &stReset);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_armbasereset_save(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	ARMBASERESETSAVE stSave;

	stSave.Src.ArmGridIdx = 0;

	player_arm_base_reset_save(pstEnv, pstPlayer, &stSave);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_closepworld(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	
	ZONEPWORLDINST *pstPworldInst;
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst; 
	UNUSED(pszArg);
	
	if (pstPlayer->stRoleData.Map < PWORLD_MAPID_BASE)
	{
		return -1;
	}

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID );
	if (NULL == pstPworldInst)
	{
		return -1;
	}

	return z_in_destroy_pworld_inst(pstAppCtx, pstEnv, pstPworldInst);
}

int z_gm_repair_all(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	ROLEWEAR *pstRoleWear = &pstPlayer->stRoleData.Wear;
	ROLEARM *pstRoleArm;
	ITEMDEF *pstItemDef;
	int iFlag = 0;
	CSPKG *pstPkgRes = &pstEnv->stCSRes;
	CSSHOPSVR *pstShopSvr = &pstPkgRes->Body.ShopSvr;
	int i;
	UNUSED(pszArg);UNUSED(pstAppCtx);
	
	for (i=0; i<pstRoleWear->ArmNum; i++)
	{
		pstRoleArm = &pstRoleWear->RoleArms[i];
		pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleArm);
		if (NULL == pstItemDef || pstItemDef->ArmDur.MaxDur <= 0 ||
			(int)pstRoleArm->Dur/DUR_UNIT >= z_get_arm_max_dur(pstEnv, pstItemDef))
		{
			continue;
		}

		if (0 == pstRoleArm->Dur/DUR_UNIT)
		{
			iFlag = 1;
		}
		
		pstRoleArm->Dur = z_get_arm_max_dur(pstEnv, pstItemDef)*DUR_UNIT;
	}
	if (iFlag)
	{
		z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}

	pstShopSvr->OpType = SHOP_REPAIR_ALL;
	pstShopSvr->ShopSvrData.ShopRepairAll = REPAIR_WAER;
	Z_CSHEAD_INIT(&pstPkgRes->Head, SHOP_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, pstPkgRes);
	
	return 0;
}

int z_gm_set_perm_limit(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{	
	char szMode[32];
	int iMode;
	UNUSED(pstAppCtx);
	
	if(!(pstPlayer->stRoleData.Perm & UIN_FLAG_GM_ADMIN))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_GM19);
		return -1;
	}
	
	if(0 > z_gm_get_val(&pszArg, szMode, sizeof(szMode)))
	{
		return -1;
	}

	iMode = atoi(szMode);

	if(iMode)
	{
		pstEnv->cLimitPerm = 1;
	}
	else
	{
		pstEnv->cLimitPerm = 0;
	}
	
	return 0;
}


int z_gm_auto_team_apply(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	/*
	AUTOTEAMAPPLYINFO stApply;
	char szID[32];

	memset(&stApply, 0, sizeof(stApply));

	if(0 > z_gm_get_val(&pszArg, szID, sizeof(szID)))
	{
		return -1;
	}
	stApply.AutoTeamIDNum = 1;
	stApply.AutoTeamIDInfos[0].AutoTeamID = atoi(szID);

	autoteam_apply(pstEnv, pstPlayer, &stApply);
	*/
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstPlayer);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_auto_team_agree(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	//autoteam_agree(pstEnv, pstPlayer);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstPlayer);	//add by paraunused.pl
}

int z_gm_block_player(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{	
	char szName[32];
	char szTime[32];
	int iTime;
	Player *pstTarget = NULL;

	if(0 > z_gm_get_val(&pszArg, szName, sizeof(szName)))
	{
		return -1;
	}
	
	if(0 > z_gm_get_val(&pszArg, szTime, sizeof(szTime)))
	{
		return -1;
	}

	pstTarget = z_name_player(pstAppCtx, pstEnv, szName);
	if(pstTarget == NULL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_GM20);
		return -1;
	}

	iTime = atoi(szTime);
	//还是别永久冻结了。太狠。
	if(iTime <= 0)
	{
		return -1;
	}

	pstTarget->stRoleData.BlockEnd = pstAppCtx->stCurr.tv_sec + iTime;
	pstTarget->stRoleData.BlockStart = pstAppCtx->stCurr.tv_sec;

	player_kickoff(pstAppCtx, pstEnv, pstTarget, ZONEERR_KICK204);	
	
	return 0;
}

int z_gm_add_enemy(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	ADDENEMYREQ stReq;
	char szRoleID[64];
	if(0 > z_gm_get_val(&pszArg, stReq.Name, sizeof(stReq.Name)))
	{
		return -1;
	}

	if(0 > z_gm_get_val(&pszArg, szRoleID, sizeof(szRoleID)))
	{
		return -1;
	}

	stReq.RoleID = atoll(szRoleID);

	player_relation_addenemy_req(pstEnv, pstPlayer, &stReq);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_nativeskill(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	DIATHESISDEF *pstDiaDef = NULL;
	int iSkillID = 0;
	int iLevel = 0;
	int iPetIdx = -1;
	char szSkill[64];
	PETPACKAGE *pstPetPackage = &pstPlayer->stRoleData.Package.PetPackage;  
	ROLEPET * pstRolePet = NULL;

	if(0 > z_gm_get_val(&pszArg, szSkill, sizeof(szSkill)))
	{
		return -1;
	}
	iPetIdx = atoi(szSkill);
	
	if(0 > z_gm_get_val(&pszArg, szSkill, sizeof(szSkill)))
	{
		return -1;
	}
	iSkillID = atoi(szSkill);

	if(0 > z_gm_get_val(&pszArg, szSkill, sizeof(szSkill)))
	{
		return -1;
	}
	iLevel = atoi(szSkill);


	if(iPetIdx < 0 || iPetIdx >= pstPetPackage->Num)
	{
		return -1;
	}
	pstRolePet = &pstPetPackage->RolePets[iPetIdx];

	pstDiaDef = z_find_diathesis(pstEnv, iSkillID, iLevel);
	if (NULL == pstDiaDef)
	{
		return -1;
	}

	pstRolePet->NativeSkill.SkillID = pstDiaDef->DiathesisID;
	pstRolePet->NativeSkill.SkillLevel = pstDiaDef->Level;
	pstRolePet->NativeSkill.GridType = pstDiaDef->PetDiathesisType;

	//宠物更新
	pet_update_online(pstEnv, pstPlayer,  pstRolePet);
	pet_update_to_clt(pstEnv, pstPlayer, pstRolePet);
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_onlinesec(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "%d", pstPlayer->stRoleData.MiscInfo.ODTime.OndaySec);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_add_enemy_by_num(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	ADDENEMYREQ stReq;
	char szRoleID[64];
	char szNum[32];
	int iNum;
	int i;
	if(0 > z_gm_get_val(&pszArg, stReq.Name, sizeof(stReq.Name)))
	{
		return -1;
	}

	if(0 > z_gm_get_val(&pszArg, szRoleID, sizeof(szRoleID)))
	{
		return -1;
	}

	stReq.RoleID = atoll(szRoleID);

	if(0 > z_gm_get_val(&pszArg, szNum, sizeof(szNum)))
	{
		return -1;
	}

	iNum = atoi(szNum);

	for (i=0; i<iNum; i++)
	{
		ADDENEMYREQ stReq1;
		snprintf(stReq1.Name, sizeof(stReq1.Name),"%s%d", stReq.Name,i);
		stReq1.RoleID += i;
		player_relation_addenemy_req(pstEnv, pstPlayer, &stReq1);
	}

	//player_relation_addenemy_req(pstEnv, pstPlayer, &stReq);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_player_addqiyue(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{	
	char szID[32];
	int iID = 0;
	RESULT stResult;
	UNUSED(pstAppCtx);
	
	memset(&stResult,0,sizeof(stResult));
	if(0 > z_gm_get_val(&pszArg, szID, sizeof(szID)))
	{
		return -1;
	}

	iID = atoi(szID);
	
	stResult.ResultID = RESULT_QIYUE;
	stResult.ResultVal1 = iID;
	player_item_do_qiyue( pstEnv, pstPlayer,  &stResult );
	
	return 0;
}

int z_gm_bus_trap(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,
                  const char *pszArg)
{
	/*
	TFRAMEHEAD stReq;
	int iRet = 0;

	memset(&stReq, 0, sizeof(stReq));
	stReq.Cmd = TFRAMEHEAD_CMD_SETROUTING;
	stReq.ID = -1; //pstPlayer->iIdx;
	stReq.ConnIdx = pstPlayer->stConnectInfo.iConnIdx;

 	stReq.CmdData.SetRouting.BusID = pstAppCtx->iId;
 	stReq.CmdData.SetRouting.ID = pstPlayer->iIdx;

 	iRet = tconnapi_send(pstEnv->iTconnapiHandle, pstEnv->iZoneConnID, NULL, 0,
 	                     &stReq);
	return iRet;
	*/
	return z_set_routing_req(pstEnv, pstPlayer->stConnectInfo.iConnIdx,
	                         pstPlayer->iIdx, pstAppCtx->iId,pstEnv->iZoneConnID,0, NULL, 0);
	UNUSED(pszArg);	//add by paraunused.pl
}


int z_gm_add_sparta(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	SPARTAMISCINFO *pstSpartaInfo = &pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo;

	char szArgs[32];
	int iAdd;

	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}

	iAdd = atoi(szArgs);

	pstSpartaInfo->FightVal+=iAdd;
	if(pstSpartaInfo->FightVal <INIT_SPARTA_FIGHT_VAL)
	{
		pstSpartaInfo->FightVal =INIT_SPARTA_FIGHT_VAL;
	}

	range_sparta_set(pstEnv, pstPlayer, RANGE_MAIN_SPARTA_SUB_FIGHT);

	player_sparta_info(pstEnv, pstPlayer);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_range(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	return range_do_chg(pstEnv);
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstPlayer);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_strong_clear_all(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	return strong_clear_all(pstEnv);
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstPlayer);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_strong_open(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	int i;
	STRONGPOINTINFO *pstInfo = &pstEnv->pstGlobalObj->stGlobal.Misc.StrongPointInfo;

	for (i=0; i<pstInfo->Num; i++)
	{
		pstInfo->StrongPoints[i].IsOpen = 1;
	}

	strong_svr_strong_info(pstEnv, NULL, 0);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstPlayer);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_strong_set_owner(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szArgs[32];
	int iMap;

	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}

	iMap = atoi(szArgs);
	return strong_set_owner(pstEnv, pstPlayer, iMap);
	
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_where(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer,
               const char* pszArg)
{
	char szAddr[128];

	STRNCPY(szAddr, tbus_addr_ntoa(pstAppCtx->iId), CCH(szAddr));
	z_sys_strf(pstEnv, pstPlayer, SYS_TIP, "Local Address: %s", szAddr);
	return 0;
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_event(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{	
	char szArgs[32];
	GameEventInst *pstEvent;
	SHITEM *pstItem;
	int i;
	UNUSED(pstAppCtx);
	UNUSED(pstPlayer);
	
	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}

	if(strcasecmp(szArgs, "list") == 0)
	{
		char szMsg[1024];
		char szEvent[32];

		snprintf(szMsg, CCH(szMsg) , "GameEvent:");
		
		for (i = 0; i < pstEnv->pstShtEvent->iMax; i++) 
		{
			pstItem = SHT_GET_ITEM(pstEnv->pstShtEvent, i);
			if ( !pstItem->fValid )
			{
		            continue;
		        }

			
			pstEvent = (GameEventInst *)pstItem->szData;
		
			snprintf(szEvent, CCH(szEvent), " %d", pstEvent->iEventID);
			strncat(szMsg, szEvent, CCH(szMsg));
		}

		z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_SVR, szMsg);
	}
	
	return 0;
}

int z_gm_item_cool(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{	
	char szArgs[32];
	int iItemID;
	int i;
	ITEMUSECOOLLIST *pstItemUseCoolList = &pstPlayer->stRoleData.MiscInfo.ItemUseCoolList;
	UNUSED(pstAppCtx);
	
	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}

	iItemID = atoi(szArgs);

	if(iItemID == 0)
	{
		memset(pstItemUseCoolList, 0,sizeof(*pstItemUseCoolList));
	}
	else
	{
		for(i = 0; i < pstItemUseCoolList->ItemSelfCoolNum; i++)
		{
			if((int)pstItemUseCoolList->ItemSelfCools[i].ItemID == iItemID)
			{
				pstItemUseCoolList->ItemSelfCoolNum--;
				if (i != pstItemUseCoolList->ItemSelfCoolNum)
				{
					memmove(&pstItemUseCoolList->ItemSelfCools[i], &pstItemUseCoolList->ItemSelfCools[i+1],
								(pstItemUseCoolList->ItemSelfCoolNum-i)*sizeof(pstItemUseCoolList->ItemSelfCools[0]));
				}

				break;
			}
		}
	}

	player_item_use_info(pstEnv, pstPlayer);
	
	return 0;
}

int z_gm_heti(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	pet_heti(pstEnv, pstPlayer);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_secondaryskill(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{	
	char szArgs[32];

	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}

	if(strcasecmp(szArgs, "learn") == 0)
	{
		char szType[32];
		char szLevel[32];
		int iSkillType, iSkillLevel;

		if(0 > z_gm_get_val(&pszArg, szType, sizeof(szType)))
		{
			return -1;
		}

		if(0 > z_gm_get_val(&pszArg, szLevel, sizeof(szLevel)))
		{
			return -1;
		}

		iSkillType = atoi(szType);
		iSkillLevel = atoi(szLevel);
		
		return player_learn_secondary_skill(pstEnv, pstPlayer, iSkillType, iSkillLevel);
	}
	else if(strcasecmp(szArgs, "del") == 0)
	{
		CSSECONDARYSKILLDEL stDel;
		char szType[32];

		if(0 > z_gm_get_val(&pszArg, szType, sizeof(szType)))
		{
			return -1;
		}

		stDel.SkillType = atoi(szType);
			
		return player_secondary_skill_del(pstEnv, pstPlayer, &stDel);
	}
	else if(strcasecmp(szArgs, "addrecipe") == 0)
	{
		char szType[32];
		char szRecipe[32];
		CSSECONDARYSKILLRECIPE stLearn;

		if(0 > z_gm_get_val(&pszArg, szType, sizeof(szType)))
		{
			return -1;
		}

		if(0 > z_gm_get_val(&pszArg, szRecipe, sizeof(szRecipe)))
		{
			return -1;
		}
		stLearn.SkillType = atoi(szType);
		stLearn.RecipeID = atoi(szRecipe);
		
		player_learn_recipe(pstEnv, pstPlayer, &stLearn);
	}
	else if(strcasecmp(szArgs, "SkillVal") == 0)
	{
		char szType[32];
		char szVal[32];
		int iType, iVal;
		SECONDARYSKILLINFO * pstSkillInfo;

		if(0 > z_gm_get_val(&pszArg, szType, sizeof(szType)))
		{
			return -1;
		}

		if(0 > z_gm_get_val(&pszArg, szVal, sizeof(szVal)))
		{
			return -1;
		}

		iType = atoi(szType);
		iVal = atoi(szVal);

		pstSkillInfo = get_player_secondary_skill_info(pstPlayer, iType);
		if(pstSkillInfo)
		{
			z_add_secondary_skill_val(pstEnv, pstPlayer, pstSkillInfo, iVal);
		}
	}
	else
	{
		return -1;
	}
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

// extend idx id
int z_gm_extend(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,
                const char *pszArg)
{
	int iBeExtendedPos = 0;
	char szArg[32];
	int iGridIdx = 0, iDefID = 0;
	ROLEPACKAGE *pstPackage = &pstPlayer->stRoleData.Package;
	ROLEGRID *pstBeExtended = NULL;
	ITEMDEF *pstDef = NULL;

	if(z_gm_get_val(&pszArg, szArg, sizeof(szArg)) < 0)
	{
		return -1;
	}
	iGridIdx = atoi(szArg);

	if(z_gm_get_val(&pszArg, szArg, sizeof(szArg)) < 0)
	{
		return -1;
	}
	iDefID = atoi(szArg);
	pstDef = z_find_itemdef(pstEnv, iDefID);
	if(!pstDef)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_TIP, "找不到指定的附魔卷轴");
		return -1;
	}
	
	iBeExtendedPos = package_grid_pos(pstPackage->SundriesPackage.RoleGrids,
	                                  pstPackage->SundriesPackage.Num, iGridIdx);
	if(iBeExtendedPos < 0)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_TIP, "你指定的物品不存在");
		return -1;
	}

	pstBeExtended = pstPackage->SundriesPackage.RoleGrids + iBeExtendedPos;
	if(pstBeExtended->Type != GRID_ITEM_ARM)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_TIP, "你指定的物品不是装备");
		return -1;
	}

	//if(z_extend_item_from_item(pstDef, pstBeExtended) < 0)
	//{
	//	z_sys_strf(pstEnv, pstPlayer, SYS_TIP, "附魔失败");
	//	return -1;
	//}
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}


int z_gm_hide_on(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	int iMemID = 0;
	char szArg[32];
	Player *pstHidePlayer = NULL;

	if(z_gm_get_val(&pszArg, szArg, sizeof(szArg)) >= 0)
	{
		iMemID = atoi(szArg);
		if (iMemID < 0) return -1;
	}

	if (iMemID > 0)
	{
		pstHidePlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, iMemID);
	}
	else
	{
		pstHidePlayer = pstPlayer;
	}

	if (z_player_find_status(pstHidePlayer, STATUS_ID_QIANXIN))
	{
		return -1;
	}

	if (z_player_apply_status(pstAppCtx, pstEnv, pstHidePlayer,STATUS_ID_QIANXIN, 1, 99999, 1, 0) >= 0)
	{
		player_status_qianxin(pstEnv, pstPlayer, 1);
	}
	return 0;
}


int z_gm_battle_num(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	LIMITBATTLETIME *pstLimit = &pstPlayer->stRoleData.MiscInfo.LimitBattleTime;

	pstLimit->Time = pstAppCtx->stCurr.tv_sec;
	pstLimit->Num = 5;
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}



int z_gm_getmem(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	z_sys_strf(pstEnv, pstPlayer, SYS_TIP, "MemID= %d", pstPlayer->iMemID);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_hide_off(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	int iMemID = 0;
	char szArg[32];
	Player *pstHidePlayer = NULL;

	if(z_gm_get_val(&pszArg, szArg, sizeof(szArg)) >= 0)
	{
		iMemID = atoi(szArg);
		if (iMemID < 0) return -1;
	}

	if (iMemID > 0)
	{
		pstHidePlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, iMemID);
	}
	else
	{
		pstHidePlayer = pstPlayer;
	}

	if (z_player_find_status(pstHidePlayer, STATUS_ID_QIANXIN))
	{
		z_player_del_status(pstAppCtx, pstEnv,pstHidePlayer,STATUS_ID_QIANXIN, 1, 0);
	}
	return 0;
}


int z_gm_drop_money(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	char szBuff[32];
	int iMoney;			// 钱的总数
	int iDropNum;		// 掉多少个包
	ZoneBootyGrid stBootyGrid;
	int iNum = 0;


	if(z_gm_get_val(&pszArg, szBuff, sizeof(szBuff)) < 0)
	{
		return -1;
	}

	iMoney= atoi(szBuff);
	if (iMoney <= 0) return -1;

	if(z_gm_get_val(&pszArg, szBuff, sizeof(szBuff)) < 0)
	{
		return -1;
	}

	iDropNum= atoi(szBuff);
	if (iDropNum <= 0)
	{
		iDropNum = 1;
	} 

	// 不能掉几毛钱
	iNum = iMoney/iDropNum;
	if (iNum < 1)
	{
		return -1;
	}

	iMoney = iDropNum*iNum;
	
	while(iMoney)
	{
		memset(&stBootyGrid, 0, sizeof(stBootyGrid));

		stBootyGrid.stGrid.GridData.RoleItem.DefID = BOOTY_MONEY_ID;
		stBootyGrid.stGrid.GridData.RoleItem.Flag = iNum;
		stBootyGrid.bViewType = BOOTY_DROP_VIEW_ALL;

		iMoney -= iNum;
		dropitem_create(pstEnv, &stBootyGrid, &pstPlayer->stRoleData.Pos, pstPlayer->stRoleData.Map, 0);
	}
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_drop_item(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	char szItemID[32];
	char szItemNum[32];
	int iItemID;
	int iItemNum;
	int iPickTime; 	// 采集时间
	ZoneBootyGrid stBootyGrid;
	ITEMDEF *pstItemDef = NULL;
	int iNum;
	ZONEMAPINST *pstMapInst;
	ZONEIDX stIdx;
	MapIndex *pstMapIdx;


	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}
	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return -1;
	}
	
	if(z_gm_get_val(&pszArg, szItemID, sizeof(szItemID)) < 0)
	{
		return -1;
	}

	iItemID = atoi(szItemID);
	if (iItemID < 0) return -1;

	if(z_gm_get_val(&pszArg, szItemNum, sizeof(szItemNum)) < 0)
	{
		return -1;
	}

	iItemNum = atoi(szItemNum);
	if (iItemNum < 0) return -1;

	if(z_gm_get_val(&pszArg, szItemNum, sizeof(szItemNum)) < 0)
	{
		iPickTime = 0;
	}
	else
	{
		iPickTime = atoi(szItemNum);
		if (iPickTime < 0)
			iPickTime = 0;
	}
	
	
	pstItemDef = z_find_itemdef(pstEnv, iItemID);
	if (!pstItemDef) return -1;
	
	
	while(iItemNum)
	{
		memset(&stBootyGrid, 0, sizeof(stBootyGrid));
		if (item_create(pstAppCtx, pstEnv, &stBootyGrid.stGrid, iItemID) < 0)
		{
			return -1;
		}
		if (iItemNum > pstItemDef->MaxPile)
		{
			iNum = pstItemDef->MaxPile;
		}
		else
		{
			iNum = iItemNum;
		}
		
		iItemNum -= iNum;
		
		stBootyGrid.stGrid.GridData.RoleItem.Num = iNum;
		stBootyGrid.bViewType = BOOTY_DROP_VIEW_ALL;
		stBootyGrid.bCollectTime = iPickTime;
			
		dropitem_create(pstEnv, &stBootyGrid, &pstPlayer->stRoleData.Pos, pstPlayer->stRoleData.Map, 0);
	}

	return 0;

}


int z_gm_vip(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	//char szName[32];
	//int iFlag;

	
	return 0; // 废弃
	/*if(z_gm_get_val(&pszArg, szName, sizeof(szName)) < 0)
	{
		return -1;
	}
	iFlag = atoi(szName);

	// 取消
	if (iFlag == 0)
	{
		pstPlayer->stRoleData.MiscInfo.Flag &= ~ROLE_VIP_FLAG;
	}
	// add
	else if(iFlag > 0)
	{
		pstPlayer->stRoleData.MiscInfo.Flag |= ROLE_VIP_FLAG;
	}
	else
	{

	}
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
	*/
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstPlayer);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}


// 召唤玩家
int z_gm_call_role(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	char szName[64];
	Player *pstOther = NULL;
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst = NULL;
	MapIndex *pstMapIdx = NULL;
	int iMap;
	RESPOS stPos;
	

	if(z_gm_get_val(&pszArg, szName, sizeof(szName)) < 0)
	{
		return -1;
	}

	pstOther = z_name_player(pstAppCtx, pstEnv, szName);
	if (!pstOther)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "玩家下线");
		return -1;
	}
	if (pstOther == pstPlayer) return -1;

	if (PLAYER_STATUS_ROLE_NOTHING == pstOther->eStatus)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "玩家在跨服战场");
		return -1;
	}

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}
	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return -1;
	}

	iMap = pstPlayer->stRoleData.Map;
	z_get_ran_pos(pstEnv, pstMapInst, pstMapIdx, &pstPlayer->stRoleData.Pos, &pstPlayer->stRoleData.Pos, 400, 50,  &stPos);

	return z_player_change_map(pstAppCtx, pstEnv, pstOther, iMap, &stPos, RAND1(180), 0);
	return 0;
}

// 打开耳朵! 你妹
int z_gm_ear_on(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	pstPlayer->stRoleData.MiscInfo.Flag &=~EAR_SWITCH;
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

// 关闭耳朵! 
int z_gm_ear_off(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	pstPlayer->stRoleData.MiscInfo.Flag |= EAR_SWITCH;
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_ride_machine(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	char szID[32];
	int iID;
	MONMACHINEDEF *pstMachine;
	
	if(z_gm_get_val(&pszArg, szID, sizeof(szID)) < 0)
	{
		return -1;
	}

	iID = atoi(szID);

	pstMachine = z_find_mon_machine_def(pstEnv, iID);
	if(!pstMachine)
	{
		return -1;
	}

	 player_new_machine_and_ride(pstEnv, pstPlayer, iID, 
				pstMachine->MaxEnergy, &pstPlayer->stRoleData.Pos);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_clean_all_item(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	z_clear_all_item(pstAppCtx, pstEnv);

	return 0;
	UNUSED(pstPlayer);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}


//执行脚本
int z_gm_run_script(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	char szScript[32];
	char szProc[32];
	char szMemID[32];
	int iRet  = 0;
	int iProc = 0;
	int iMemID = 0;
	int iIndex = -1;
	char szFullName[32];
	ScriptIndex *pstScriptInd =NULL;
	char cType = 0;
	TMEMBLOCK* pstMemBlock = NULL;
	GameEventInst stEventInst;
	GameEventInst *pstEvent = NULL;
	
	if (!pszArg)
	{
		return -1;
	}

	
	memset(szScript,0x00,sizeof(szScript));
	if (0 > z_gm_get_val(&pszArg, szScript, sizeof(szScript))  )
	{
		return -1;
	}

	memset(szProc,0x00,sizeof(szProc));
	if (0 > z_gm_get_val(&pszArg, szProc, sizeof(szScript))  )
	{
		return -1;
	}

	memset(szMemID,0x00,sizeof(szMemID));
	if (0 > z_gm_get_val(&pszArg, szMemID, sizeof(szScript))  )
	{
		return -1;
	}

	iProc = atoi(szProc);
	iMemID = atoi(szMemID);
	iRet  = 0;

	iRet = z_gm_loadscript(pstEnv->pstAppCtx, pstEnv, pstPlayer, szScript);
	if ( 0 > iRet)
	{
		return -1; //load failed
	}

	snprintf(szFullName, sizeof(szFullName), "%s.mac", szScript);	
	pstScriptInd = get_script_ind(szFullName);
	if ( !pstScriptInd )
	{
		printf("get_script_ind fail\n");
		tlog_error(pstEnv->pstLogCat, 0, 0, "get_script_ind fail");
		return -1;
	}
	
	stSEnv.pstPlayer = pstPlayer;
	stSEnv.pstMonster = NULL;
	stSEnv.pstNpc = NULL;
	stSEnv.pstPworldInst = z_find_pworldinst2(pstEnv, pstPlayer->stRoleData.Map);
	stSEnv.pstRoleItem = NULL;
	stSEnv.pstKiller = NULL;
	stSEnv.pstMapinst = NULL;
	if (iMemID > 0 )
	{
		iIndex = z_id_indextype(pstEnv->pstAppCtx, pstEnv, iMemID, &cType);
		if (iIndex >= 0)
		{

			if (cType == OBJ_MONSTER)
			{
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
				if (pstMemBlock->fValid)
				{
					stSEnv.pstMonster = (Monster *)pstMemBlock->szData;
				}
			}
			else if (cType == OBJ_NPC)
			{
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstNpcPool, iIndex);
				if (pstMemBlock->fValid)
				{
					stSEnv.pstNpc = (Npc *)pstMemBlock->szData;
				}
			}
			
		}
	}
	else if (iMemID < 0)
	{
		stEventInst.iEventID = -iMemID;
		pstEvent = sht_find(pstEnv->pstShtEvent, &stEventInst, EventHashCmp, EventHashCode);
		if (pstEvent)
		{
			stSEnv.pstEvent = pstEvent;
		}
	}


	iRet = z_exec_script(pstEnv->pstAppCtx, pstEnv, pstPlayer, szFullName, pstScriptInd, iProc);
	   
	return iRet;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_safelock(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	if (pstPlayer->stRoleData.MiscInfo.LockPassWD[0] == 0)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "你没有设置安全锁密码!");
	}
	else
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "你的安全锁密码是: %s", pstPlayer->stRoleData.MiscInfo.LockPassWD);
	}
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_cdkey(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	char szCDKey[64];

	if (0 > z_gm_get_val(&pszArg, szCDKey, sizeof(szCDKey))  )
	{
		return -1;
	}
	player_cdkey_req(pstEnv, pstPlayer, szCDKey);
	return 0;
	UNUSED(pstAppCtx);
}

int z_gm_chgofflineexp(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	char szExp[64];
	int iExp = 0;
	long long llExp;
	AccShareData* pstSharaData = &pstPlayer->stAccShareData;
	int iOld = pstSharaData->stAccShare.OfflineExp;


	if (0 > z_gm_get_val(&pszArg, szExp, sizeof(szExp))  )
	{
		return -1;
	}

	iExp = atoi(szExp);

	llExp = iOld + iExp;
	if ( llExp < 0)
	{
		llExp  =0;	
	}
	if (llExp > 0x7fffffff)
	{
		llExp = 0x7fffffff;
	}

	pstSharaData->stAccShare.OfflineExp = llExp;

	z_player_awardexp_change(pstEnv,pstPlayer,0,TRUE);

	
	return 0;
	UNUSED(pstAppCtx);
}


int z_gm_setbattleflag(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	char szFlag[64];
	int iFlag = 0;

	if (0 > z_gm_get_val(&pszArg, szFlag, sizeof(szFlag))  )
	{
		return -1;
	}

	iFlag = atoi(szFlag);

	if(iFlag>0)
	{
		iFlag = 1;
	}
	else
	{
		iFlag = 0;
	}

	pstEnv->pstConf->BattleFlag = iFlag;
	if(iFlag == 0)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_TIP, "当前自动组队是本服状态");
	}
	else
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_TIP, "当前自动组队是跨服状态");
	}
			
	return 0;
	UNUSED(pstAppCtx);
}

int z_gm_roll(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	char szRand[32];
	char szMsg[256];
	int iRand;
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSSYSMSG *pstSysMsg = &pstPkg->Body.SysMsg;
	
	if(z_gm_get_val(&pszArg, szRand, sizeof(szRand)) < 0)
	{
		iRand = 100;
	}
	else
	{
		iRand = atoi(szRand);
		if(iRand <= 1 )
		{
			return -1;
		}
	}

	sprintf(szMsg, "您Roll(1-%d): %d", iRand, (int)RAND1(iRand )+1);

	pstSysMsg->Type = SYS_CHAT;
	pstSysMsg->Times = 0;
	pstSysMsg->Interval = 0;
	pstSysMsg->LinkLen = 0;
	STRNCPY(pstSysMsg->Msg, szMsg, sizeof(pstSysMsg->Msg));

	Z_CSHEAD_INIT(&pstPkg->Head, SYS_MSG);
	z_cltmsg_send(pstEnv, pstPlayer, pstPkg);

	sprintf(szMsg, "%sRoll(1-%d): %d", pstPlayer->stRoleData.RoleName, iRand, (int)RAND1(iRand )+1);
	STRNCPY(pstSysMsg->Msg, szMsg, sizeof(pstSysMsg->Msg));
	z_sendpkg_playerview_filtered(pstAppCtx, pstEnv, pstPlayer, 0, pstPkg, 0);
/*
	int iNum;
	int aiBase[33] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,};
	int aiAdd[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
	int iRandNum;
	int aiMoney[7];
	int i, iLen;

	iNum = 33;
	iRandNum = 0;
	for(i =0; i< 6; i++)
	{
		iRand = RAND1(iNum);
		aiMoney[iRandNum] = aiBase[iRand];
		iRandNum++;
		
		iNum--;
		if(iRand != iNum)
		{
			memmove(&aiBase[iRand], &aiBase[iRand+1], sizeof(aiBase[0])*(iNum - iRand));
		}
	}

	iNum = 16;
	iRand = RAND1(16);
	aiMoney[iRandNum] = aiAdd[iRand];
	iRandNum++;

	iLen = 0;
	szMsg[0] = 0;
	for(i=0; i<iRandNum; i++)
	{
		sprintf(szMsg+iLen, "%d ", aiMoney[i]);
		iLen = strlen(szMsg);
	}

	szMsg[iLen+1] = 0;
	
	z_sys_strf(pstEnv, pstPlayer, SYS_TIP, szMsg);
*/
	return 0;
}


int z_gm_spook(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	char szArg[32];
	int iFlag;
	
	if(z_gm_get_val(&pszArg, szArg, sizeof(szArg)) < 0)
	{
		return -1;
	}

	iFlag = atoi(szArg);

	if(iFlag == 1)
	{
		pstPlayer->stOnline.State |= CS_STAT_AUTOFLY;
		pstPlayer->stRoleData.MiscInfo.AutoFlyPath = 0xff;

		if (pstPlayer->stRoleData.Map < PWORLD_MAPID_BASE)
		{
			pstPlayer->stRoleData.ExitMap = pstPlayer->stRoleData.Map;
			pstPlayer->stRoleData.ExitPos_x = pstPlayer->stRoleData.Pos.X;
			pstPlayer->stRoleData.ExitPos_y = pstPlayer->stRoleData.Pos.Y;
		}

		z_player_chk_mspd(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}
	else
	{
		RESPOS stPos;
		
		pstPlayer->stOnline.State &= ~CS_STAT_AUTOFLY;
		pstPlayer->stRoleData.MiscInfo.AutoFlyPath = 0;

		stPos = pstPlayer->stRoleData.Pos;
		player_auto_fly_move(pstEnv, pstPlayer, &stPos);
	}
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}


int z_gm_monsterai(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	char szArg[32];
	int iID=0;
	Monster* pstMonster = NULL;
	AtkTargetList  *pstAtkTargets = NULL;
	int i=0;
	char szMsg[256];
	
	if(z_gm_get_val(&pszArg, szArg, sizeof(szArg)) < 0)
	{
		return -1;
	}

	iID = atoi(szArg);

	pstMonster = z_id_monster( pstAppCtx, pstEnv, iID);
	if(!pstMonster)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, "没找到id是 %d 的怪物",iID);
		return -1;
	}

	pstAtkTargets = &pstMonster->stAtkTargets;

	if (pstAtkTargets->iTargetCur > 0)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, "当前目标id: %d",pstAtkTargets->iTargetCur);
	}

	if (pstAtkTargets->iDefyID > 0)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, "嘲讽者id: %d,时间还有%d秒",pstAtkTargets->iDefyID, 
				(int)(pstAtkTargets->tDefyEnd-pstEnv->pstAppCtx->stCurr.tv_sec));
	}

	if (pstAtkTargets->PressNum > 0)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_TIP, "压力表数目: %d", pstAtkTargets->PressNum);
			
		for(i=0;i<pstAtkTargets->PressNum;i++)
		{
			snprintf(szMsg, sizeof(szMsg), "  (id %d, press val %d)", pstAtkTargets->astPressEntrys[i].iTargetID,
				pstAtkTargets->astPressEntrys[i].iPress);
			
			z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, szMsg);
		}
	}
	
	return 0;
}

int z_gm_pkval(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	UNUSED(pszArg);
	UNUSED(pstAppCtx);

	z_sys_strf(pstEnv, pstPlayer, SYS_TIP, 
			"当前杀气值:%d", pstPlayer->stRoleData.MiscInfo.PkInfo.PkVal);
	
	return 0;
}

int z_gm_choose(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	char szNum[32];
	int iNum;
	CSCHOOSEC chooseC;

	memset(szNum,0,sizeof(szNum));
	if(z_gm_get_val(&pszArg, szNum, sizeof(szNum)) < 0)
	{
		return -1;
	}
	iNum = atoi(szNum);

	if (iNum < 0 || iNum > CHOOSE_TYPE_NO)
	{
		return -1;
	}

	
	chooseC.BindType = pstPlayer->stOnline.stChooseInfo.cChooseBindType;
	chooseC.MemID = pstPlayer->stOnline.stChooseInfo.iChooseMemID;
	chooseC.ChooseType = iNum;
	player_choose(pstEnv,pstPlayer,&chooseC);
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_finicyc(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	char szCycID[32];
	char szNum[32];

	int iCycID;
	int iNum;
	int i;
	TASKCYCLEONEDAY *pstOneDay = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskCycleOneDay;
	
	if(z_gm_get_val(&pszArg, szCycID, sizeof(szCycID)) < 0)
	{
		return -1;
	}
	iCycID = atoi(szCycID);
	if (iCycID <= 0)
		return -1;
		

	if(z_gm_get_val(&pszArg, szNum, sizeof(szNum)) < 0)
	{
		return -1;
	}
	iNum = atoi(szNum);
	if (iNum <=0 )
		return -1;

	for (i=0; i<pstOneDay->TaskCycleNum; i++)
	{
		if (pstOneDay->TaskCycleOneDayInfo[i].TaskCycleID == iCycID)
		{
			pstOneDay->TaskCycleOneDayInfo[i].OneDayNum = iNum;
			pstOneDay->TaskCycleOneDayInfo[i].Time = pstAppCtx->stCurr.tv_sec;
			break;
		}
	}

	if (i>=pstOneDay->TaskCycleNum)
	{
		if (pstOneDay->TaskCycleOneDayInfo[i].OneDayNum <MAX_TASKCYCLE_ONEDAY)
		{
			pstOneDay->TaskCycleOneDayInfo[pstOneDay->TaskCycleNum].OneDayNum = iNum;
			pstOneDay->TaskCycleOneDayInfo[pstOneDay->TaskCycleNum].Time = pstAppCtx->stCurr.tv_sec;
			pstOneDay->TaskCycleOneDayInfo[pstOneDay->TaskCycleNum].TaskCycleID = iCycID;
			pstOneDay->TaskCycleNum++;
		}
	}

	for (i=0; i<pstOneDay->TaskCycleNum; i++)
	{
		if (pstOneDay->TaskCycleOneDayInfo[i].TaskCycleID == iCycID)
		{
			z_sys_strf(pstEnv,pstPlayer, SYS_MIDDLE2, "环组ID:%d      完成次数: %d", 
				iCycID, pstOneDay->TaskCycleOneDayInfo[i].OneDayNum);
			break;
		}
	}
	return 0;
}

int z_gm_chgline(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	char szMapID[32];
	CSMAPCHGLINE stChg;

	if(z_gm_get_val(&pszArg, szMapID, sizeof(szMapID)) < 0)
	{
		return -1;
	}

	stChg.MapID = atoi(szMapID);
	return map_change_line(pstEnv, pstPlayer, &stChg);
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_npc_script(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	char szNpcID[32];
	int iNpcID = 0;
	int iNpcIndex = -1;
	char cType = 0;
	Npc* pstNpc = NULL;
	char szMsg[CLT_MSG_LEN];
	TMEMBLOCK* pstMemBlock = NULL; 
	Monster *pstMonster = NULL;
	int iProc = -1;
	char szProc[32];
	char szScript[64];
	ScriptIndex* pstScriptInd = NULL;
	int iRet = 0;

	szProc[0] = 0;
	szScript[0] = 0;
	
	stSEnv.pstPlayer = pstPlayer;
	stSEnv.pstMonster = NULL;
	stSEnv.pstNpc = NULL;
	stSEnv.pstPworldInst = z_find_pworldinst2(pstEnv, pstPlayer->stRoleData.Map);
	stSEnv.pstRoleItem = NULL;
	stSEnv.pstKiller = NULL;
	stSEnv.pstMapinst = NULL;

	if(z_gm_get_val(&pszArg, szNpcID, sizeof(szNpcID)) < 0)
	{
		return -1;
	}	
	if(0 == z_gm_get_val(&pszArg, szProc, sizeof(szProc)))
	{
		iProc = atoi(szProc);
	}	
	iNpcID = atoi(szNpcID);

	iNpcIndex = z_id_indextype(pstEnv->pstAppCtx, pstEnv, iNpcID, &cType);
	if (0 > iNpcIndex)
	{
		return -1;
	}

	memset(szMsg,0,sizeof(szMsg));
	if (cType == OBJ_NPC)
	{
		pstNpc = tmempool_get(pstEnv->pstNpcPool, iNpcIndex);
		if (!pstNpc )
		{
			return -1;
		}
		
		snprintf(szMsg, CCH(szMsg), "NPC%d的脚本为%s", iNpcID,pstNpc->szScriptName);
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, szMsg);

		if (iProc >= 0 && pstNpc->szScriptName[0] != '\0')
		{
			stSEnv.pstNpc = pstNpc;
			snprintf(szScript,CCH(szScript),"%s",pstNpc->szScriptName);
		}
	}
	else if (cType == OBJ_MONSTER)
	{
		pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iNpcIndex);
        if (!pstMemBlock->fValid)
        {
                return -1;
        }
		pstMonster = (Monster *)pstMemBlock->szData;
		if (pstMonster->cMonScriptFlag)
		{
			snprintf(szMsg, CCH(szMsg), "monster%d的脚本为monster%d", iNpcID,pstMonster->iDefIdx);
			if (iProc >= 0)
			{
				stSEnv.pstMonster = pstMonster;
				snprintf(szScript, CCH(szScript), "monster%d.mac",pstMonster->iDefIdx);				
			}
		}
		else
		{
			snprintf(szMsg, CCH(szMsg), "monster%d的脚本为空,DefID为%d", iNpcID,pstMonster->iDefIdx);
		}
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, szMsg);
	}
	else
	{
		return -1;
	}

	if ( iProc < 0)
	{
		stSEnv.pstPlayer = NULL;
		stSEnv.pstMonster = NULL;
		stSEnv.pstNpc = NULL;
		stSEnv.pstPworldInst = NULL;
		stSEnv.pstRoleItem = NULL;
		stSEnv.pstKiller = NULL;
		stSEnv.pstMapinst = NULL;

		return 0;
	}

	pstScriptInd = get_script_ind(szScript);
	if ( !pstScriptInd )
	{
		printf("get_script_ind fail\n");
		tlog_error(pstEnv->pstLogCat, 0, 0, "get_script_ind fail");
		return -1;
	}

	iRet = z_exec_script(pstEnv->pstAppCtx, pstEnv, pstPlayer, szScript, pstScriptInd, iProc);
	   
	return iRet;

	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_pos(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{

	char szMsg[CLT_MSG_LEN];
	char szInfo[32];

	
	memset(szMsg,0,sizeof(szMsg));
	memset(szInfo,0,sizeof(szInfo));

	z_gm_get_val(&pszArg, szInfo, sizeof(szInfo));
	
	snprintf(szMsg, CCH(szMsg), "当前坐标和方向为%d,%d,%s", pstPlayer->stRoleData.Pos.X ,pstPlayer->stRoleData.Pos.Y,szInfo);
	z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, szMsg);

	return 0;

	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_jump(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
 
	char szPosX[32];
	char szPosY[32];
	RESPOS stPos;
 

	if (0 == z_player_act_status(pstAppCtx, pstEnv, pstPlayer, RES_ACTION_WALK))
	{
		return -1;
	}

	if( z_gm_get_val(&pszArg, szPosX, sizeof(szPosX))<0 ||
			z_gm_get_val(&pszArg, szPosY, sizeof(szPosY))<0 )
	{
		return -1;
	}


	stPos.X = atoi(szPosX); 
	stPos.Y = atoi(szPosY);

 
	return player_goto(pstEnv,pstPlayer,pstPlayer->stRoleData.Map,stPos.X, stPos.Y,pstPlayer->stOnline.iMapPos);
}

typedef struct tagDroppedItemCountEntry
{
	int iDefID;
	int iCount;
}DroppedItemCountEntry;

typedef struct tagDroppedItemCountInfo
{
	DroppedItemCountEntry astEntries[1000];
	int iNum;
}DroppedItemCountInfo;

static DroppedItemCountEntry *doi_find_or_new_entry(
	DroppedItemCountInfo *pstInfo,
	int iKey)
{
	int i = 0;
	DroppedItemCountEntry *pstRet = NULL;
	for(i = 0; i < (int)CCH(pstInfo->astEntries); ++i)
	{
		if(pstInfo->astEntries[i].iDefID == iKey)
		{
			pstRet = pstInfo->astEntries + i;
			break;
		}
	}
	if (i >= (int)CCH(pstInfo->astEntries))
	{
		if (pstInfo->iNum < (int)CCH(pstInfo->astEntries))
		{
			pstRet = pstInfo->astEntries + (pstInfo->iNum)++;
			pstRet->iDefID = iKey;
			pstRet->iCount = 0;
		}
	}
	return pstRet;
}

static void doi_dump(const DroppedItemCountInfo *pstInfo, ZONESVRENV* pstEnv, 
                     Player* pstPlayer)
{
	int i = 0;
	z_sys_strf(pstEnv, pstPlayer, SYS_TIP,
	           "=========Dropped Item Count=========");
	for(i = 0; i < pstInfo->iNum; ++i)
	{
		const ITEMDEF *pstDef = z_find_itemdef(pstEnv,
		                                       pstInfo->astEntries[i].iDefID);
		if (!pstDef)
		{
			continue;
		}
		z_sys_strf(pstEnv, pstPlayer, SYS_TIP, "-%03d- [%s](%d)x%d",
		           i, pstDef->Name, pstDef->ItemID,
		           pstInfo->astEntries[i].iCount);
	}
	z_sys_strf(pstEnv, pstPlayer, SYS_TIP,
	           "==========End of %d Item(s)=========", pstInfo->iNum);
}

static int map_dropped_item_count(
	ZONESVRENV *pstEnv,
	const ZONEMAPINST *pstMapInst,
	DroppedItemCountInfo *pstInfo,
	RESPOS *pstPoint,
	int iRadius)
{
	int i = 0, iCount = 0;

	for(i = 0; i < pstMapInst->iAreaDyn; ++i)
	{
		const AREADYN *pstAreaDyn = NULL;
		int j = 0;
		int iDyn = pstMapInst->aiAreaDyn[i];
		if(iDyn < 0)
		{
			continue;
		}

		pstAreaDyn = tmempool_get(pstEnv->pstAreaDynPool, iDyn);
		if (!pstAreaDyn)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "Dyn=%d", iDyn);
			continue;
		}

		for(j = 0; j < pstAreaDyn->iInanimate; ++j)
		{
			DropItem *pstDroppedItem = NULL;
			DroppedItemCountEntry *pstEntry = NULL;

			if (j >= MAX_AREA_ANIMATE)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "map %d dyn %d iInanimate=%d",
				           pstMapInst->iMapInstID, iDyn, pstAreaDyn->iInanimate);
				return -1;
			}

			pstDroppedItem = z_id_dropitem(pstEnv->pstAppCtx, pstEnv,
			                               pstAreaDyn->aiInanimate[j]);
			if (!pstDroppedItem ||
				pstDroppedItem->cType != DROP_ITEM_GRID ||
				z_distance(pstPoint, &pstDroppedItem->stPos) > iRadius)
			{
				continue;
			}

			pstEntry = doi_find_or_new_entry(pstInfo, pstDroppedItem->iDefIdx);
			if (!pstEntry)
			{
				continue;
			}

			switch(pstDroppedItem->stObj.stBootyGrid.stGrid.Type)
			{
			case GRID_ITEM_ITEM:
				pstEntry->iCount +=
					pstDroppedItem->stObj.stBootyGrid.stGrid.GridData.RoleItem.Num;
				break;
			
			case GRID_ITEM_ARM:
				++(pstEntry->iCount);
				break;
			
			default:
				return -1;
				break;
			}
			++iCount;
		}
	}
	return iCount;
}

int z_gm_dropped_item_count(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv,
                            Player *pstPlayer, const char *pszArg)
{
	int iRadius = 0;
	char szBuf[128];
	DroppedItemCountInfo stInfo = { .iNum = 0, };
	ZONEIDX stIdx = {
		.iID = pstPlayer->stRoleData.Map,
		.iPos = pstPlayer->stOnline.iMapPos,
	};

	const ZONEMAPINST *pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &stIdx);
	if (!pstMapInst)
	{
		return -1;
	}
	
	if (z_gm_get_val(&pszArg, szBuf, sizeof(szBuf)) < 0)
	{
		iRadius = 24 * 100;
	}
	else
	{
		iRadius = atoi(szBuf) * 100;
	}
	if (iRadius <= 0)
	{
		return -1;
	}

	if (map_dropped_item_count(pstEnv, pstMapInst, &stInfo,
	                           &pstPlayer->stRoleData.Pos, iRadius) < 0)
	{
		return -1;
	}

	doi_dump(&stInfo, pstEnv, pstPlayer);
	return 0;
}

int z_gm_play_movie(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv,
                            Player *pstPlayer, const char *pszArg)
{
	char szFilePath[256];

	memset(szFilePath,0,sizeof(szFilePath));
	if (z_gm_get_val(&pszArg, szFilePath, sizeof(szFilePath)) < 0)
	{
		return -1;
	}
	
	player_movie_start_notify(pstEnv, pstPlayer, szFilePath);
	
	return 0;
	
	UNUSED(pstAppCtx);
}

int z_gm_bub(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	char szID[32];
	int iID;
	
	if(z_gm_get_val(&pszArg, szID, sizeof(szID)) < 0)
	{
		return -1;
	}

	iID = atoi(szID);
	
	return player_call_bub(pstEnv, pstPlayer, iID, 0);

	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_accgift(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	char szID[32];
	int iID;
	
	if(z_gm_get_val(&pszArg, szID, sizeof(szID)) < 0)
	{
		return -1;
	}

	iID = atoi(szID);
	
	return player_acc_gift_req(pstEnv, pstPlayer,iID);

	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_propgift(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	char szID[32];
	int iGroupID;
	int iCashUp;
	
	if(z_gm_get_val(&pszArg, szID, sizeof(szID)) < 0)
	{
		return -1;
	}

	iGroupID = atoi(szID);

	if(z_gm_get_val(&pszArg, szID, sizeof(szID)) < 0)
	{
		return -1;
	}

	iCashUp = atoi(szID);
	
	return player_prop_gift_req(pstEnv, pstPlayer,iGroupID,iCashUp);

	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_setdesig(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	char szID[32];
	int iID;

	if(z_gm_get_val(&pszArg, szID, sizeof(szID)) < 0)
	{
		return -1;
	}

	iID = atoi(szID);

	if (player_designation_add(pstEnv, pstPlayer,iID) < 0)
	{
		return -1;
	}

	return player_designation_set(pstEnv, pstPlayer,iID);
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_oneoffexp(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	/*
	char szExp[32];
	int iExp;

	if(z_gm_get_val(&pszArg, szExp, sizeof(szExp)) < 0)
	{
		return -1;
	}

	iExp = atoi(szExp);
	pstPlayer->stRoleData.MiscInfo.OfflineExpInfo.OneOffExp += iExp;

	if (pstPlayer->stRoleData.MiscInfo.OfflineExpInfo.OneOffExp < 0)
	{
		pstPlayer->stRoleData.MiscInfo.OfflineExpInfo.OneOffExp = 0;
	}
	z_player_oneoffexp_send(pstEnv,pstPlayer);
	*/
	return 0;

	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstPlayer);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}


int z_gm_add_machine_energy(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szID[32], szEnergy[32];
	int iID, iEnergy;
	Player *pstPlayerObj;
	Monster *pstMonObj = NULL;
	int iIndex;
	char cType;
	TMEMBLOCK *pstMemBlock;

	
	if (0 > z_gm_get_val(&pszArg, szID, sizeof(szID)) ||
		0 > z_gm_get_val(&pszArg, szEnergy, sizeof(szEnergy)) 
		)
	{
		return -1;
	}

	iID = atoi(szID);
	iEnergy = atoi(szEnergy);

	if (iID == 0)
	{
		iID = pstPlayer->iMemID;
	}

	iIndex = z_id_indextype(pstAppCtx, pstEnv, iID, &cType);
	if (iIndex < 0)
	{
	        return -1;
	}
	switch(cType)
	{
	 case OBJ_PLAYER:
		pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
	            if (!pstMemBlock->fValid)
	            {
	                    return -1;
	            }
		pstPlayerObj = (Player *)pstMemBlock->szData;
		pstMonObj = machine_get_by_player( pstEnv, pstPlayerObj);
			
		break;
	case OBJ_MONSTER:
		pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
        if (!pstMemBlock->fValid)
        {
                return -1;
        }
		pstMonObj = (Monster *)pstMemBlock->szData;
		break;
	default:
		return -1;
		break;
	}

	if( !pstMonObj || !is_mon_machine(pstMonObj))
	{
		return -1;
	}

	z_machine_attr_chg(pstEnv, pstMonObj, MON_MACHINE_ATTR_ENERGY, iEnergy);
	
	return 0;
}

/*	-lxk
	功能:角色穿在身上的装备强化星级
*/
int z_gm_armstarup(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	char szArg[32];
	int iStarLevel = 0;
	int iStarMulIdx = -1;
	ROLEWEAR *pstRoleWear = &pstPlayer->stRoleData.Wear;
	ROLEARM *pstRoleArms = NULL;
	int i = 0;
	int j = 0;
	ITEMDEF *pstArmItem = NULL;
	if (0 == z_gm_get_val(&pszArg, szArg, sizeof(szArg)) )
	{
		iStarLevel = atoi(szArg);
	}

	if (0 == z_gm_get_val(&pszArg, szArg, sizeof(szArg)) )
	{
		iStarMulIdx = atoi(szArg);
	}
	
	for(i = 0; i < pstRoleWear->ArmNum; i++)
	{
		pstRoleArms = &pstRoleWear->RoleArms[i];
	
		pstArmItem = z_find_itemdef(pstEnv, pstRoleArms->DefID);
		if (!pstArmItem)
		{
			return -1;
		}

		if(arm_is_fashion(pstArmItem))
		{
			continue;
		}
		if(pstArmItem->StarLimit > MAX_ARM_STAR_LEVEL)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, 
				       "Arm[ItemID=%d] StarLimit is more than MAX_ARM_STAR_LEVEL",
				       pstArmItem->ItemID);
			return -1;
		}
		if( iStarLevel <= 0)
		{
			pstRoleArms->StarLv = pstArmItem->StarLimit;
		}
  		else if( pstArmItem->StarLimit && iStarLevel > pstArmItem->StarLimit)
		{
			pstRoleArms->StarLv = pstArmItem->StarLimit;
		}
		else if(iStarLevel >= MAX_ARM_STAR_LEVEL)
		{
			pstRoleArms->StarLv = MAX_ARM_STAR_LEVEL;
		}
		else
		{
			pstRoleArms->StarLv = iStarLevel;
		}

		if(iStarMulIdx < 0 || iStarMulIdx >= MAX_STAR_MUL_LIST - 1)
		{
			iStarMulIdx = MAX_STAR_MUL_LIST - 1;
		}

		memset(&pstRoleArms->StarInfo[0], 0, sizeof(pstRoleArms->StarInfo));
		for(j = 0; j < pstRoleArms->StarLv; j++)
		{	
			pstRoleArms->StarInfo[j].ValIdx = iStarMulIdx;
		}
		
		arm_wear_update( pstEnv,  pstPlayer, pstRoleArms);
		z_sys_strf(pstEnv, pstPlayer, SYS_SVR, 
		       "[强化]-[%s]-[%d星,索引值:%d]",
		       pstArmItem->Name, pstRoleArms->StarLv,iStarMulIdx);
	}

	//重新计算下属性
	z_update_all_attr(pstAppCtx, pstEnv, pstPlayer);
	z_update_avatar(pstEnv, pstPlayer);
	return 0;
}


int z_gm_armattr_reset(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	char szIdx[32];
	ARMATTRRESET stReset;
	if(z_gm_get_val(&pszArg, szIdx, sizeof(szIdx)) < 0)
	{
		return -1;
	}

	stReset.ArmData.ArmGridIdx = 0;
	stReset.ArmData.ArmListType = LIST_TYPE_SUNDRIES;
	stReset.Bind = 1;
	stReset.Idx = atoi(szIdx);
	stReset.UseItemID = 5200032;
	
	player_arm_attr_reset(pstEnv, pstPlayer, &stReset);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_armattr_active(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	char szIdx[32];
	ARMATTRACTIVE stActive;
	if(z_gm_get_val(&pszArg, szIdx, sizeof(szIdx)) < 0)
	{
		return -1;
	}

	stActive.ArmData.ArmGridIdx = 0;
	stActive.ArmData.ArmListType = LIST_TYPE_SUNDRIES;
	stActive.Idx = atoi(szIdx);
	
	 player_arm_attr_active(pstEnv, pstPlayer, &stActive);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_geniusclear(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	genius_gm_clear(pstEnv, pstPlayer);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_geniusup(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	GENIUSINFO *pstGeniusInfo = &pstPlayer->stRoleData.MiscInfo.GeniusInfo;
	int i,j;
	GENIUSATTRDEF* pstGeniusAttrDef;

	for (i=0; i<pstGeniusInfo->Num; i++)
	{
		for (j=0; j<pstGeniusInfo->One[i].Num; j++)
		{
			pstGeniusAttrDef = genius_attr_def_find(pstEnv,pstPlayer->stRoleData.Career, 
							pstGeniusInfo->One[i].SubOne[j].AttrID, pstGeniusInfo->One[i].SubOne[j].Lvl +1);
			if (pstGeniusAttrDef)
			{
				pstGeniusInfo->One[i].SubOne[j].Lvl++;
			}
		}
	}

	z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	genius_list_notify(pstEnv, pstPlayer);

	range_single_set(pstEnv, pstPlayer, RANGE_MAIN_SINGLE_SUB_GENIUS, genius_get_mod(pstEnv, pstPlayer));
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_valfight(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	char szAtkee[32];
	char szSkillID[32];
	char szSkillLevel[32];
	char szCount[32];
	int iAtkeeID;
	int iSkillID, iSkillLevel, iCount;
	CSVALFIGHTCLT stValFightClt;
	
	if(z_gm_get_val(&pszArg, szAtkee, sizeof(szAtkee)) < 0)
	{
		return -1;
	}

	if(z_gm_get_val(&pszArg, szSkillID, sizeof(szSkillID)) < 0)
	{
		return -1;
	}

	if(z_gm_get_val(&pszArg, szSkillLevel, sizeof(szSkillLevel)) < 0)
	{
		return -1;
	}

	if(z_gm_get_val(&pszArg, szCount, sizeof(szCount)) < 0)
	{
		return -1;
	}

	iAtkeeID = atoi(szAtkee);
	iSkillID = atoi(szSkillID);
	iSkillLevel = atoi(szSkillLevel);
	iCount = atoi(szCount);

	memset(&stValFightClt, 0, sizeof(stValFightClt));
	stValFightClt.AtkerID = pstPlayer->iMemID;
	stValFightClt.ObjInfo[0].MemID = pstPlayer->iMemID;
	stValFightClt.ObjInfo[1].MemID = iAtkeeID;
	stValFightClt.SkillID = iSkillID;
	stValFightClt.SkillLevel = iSkillLevel;
	stValFightClt.Count = iCount;
	
	val_fight(pstAppCtx, pstEnv, pstPlayer, &stValFightClt);
		
	return 0;
}

int z_gm_nearmemid(TAPPCTX * pstAppCtx, ZONESVRENV * pstEnv, Player * pstPlayer, const char * pszArg)
{
	int i = 0;
	int iNpcDis = 0;
	int iMonsterDis = 0;
	int iPlayerDis = 0;
	char cType = OBJ_UNKNOW;
	int iIndex = 0;
	TMEMBLOCK* pstMemBlock = NULL;

	Npc* pstTmpNpc = NULL;

	Monster* pstTmpMonster = NULL;

	Player* pstTmpOther = NULL;
	int iDis = 0;
	char szMsg[CLT_MSG_LEN];
	int iMemID = 0;
	

	memset(szMsg,0,sizeof(szMsg));

	if (z_gm_get_val(&pszArg, szMsg, sizeof(szMsg)) < 0)
	{
		return -1;
	}

	iNpcDis = atoi(szMsg);
	iMonsterDis = iNpcDis;
	iPlayerDis = iNpcDis;

	snprintf(szMsg, CCH(szMsg), "=====nearmemid begin=====");
	z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, szMsg);
	
	for(i = 0 ; i < pstPlayer->stOnline.iVAnimate; i++)
	{
		iMemID = pstPlayer->stOnline.aiVAnimate[i];
		iIndex = z_id_indextype(pstAppCtx, pstEnv, iMemID, &cType);
		if (iIndex < 0 )
		{
			continue;
		}
		switch (cType) 
		{
		case OBJ_NPC:
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstNpcPool, iIndex);
			if (!pstMemBlock->fValid)
			{
				continue;
			}

			pstTmpNpc = (Npc*)pstMemBlock->szData;

			iDis = z_distance(&pstPlayer->stRoleData.Pos,&pstTmpNpc->stMon.stCurrPos);
			if (iDis < iNpcDis)
			{
				snprintf(szMsg, CCH(szMsg), "Npc Memid为%d,Defid为%d,脚本为%s,距离为%d", iMemID,pstTmpNpc->stMon.iDefIdx,pstTmpNpc->szScriptName,iDis);
				z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, szMsg);
			}
			break;
		case OBJ_MONSTER:
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
			if (!pstMemBlock->fValid)
			{
				continue;
			}
	
			pstTmpMonster= (Monster*)pstMemBlock->szData;

			iDis = z_distance(&pstPlayer->stRoleData.Pos,&pstTmpMonster->stCurrPos);
			if (iDis < iMonsterDis)
			{
				if (pstTmpMonster->cMonScriptFlag)
				{
					snprintf(szMsg, CCH(szMsg), "Monster Memid为%d,Defid为%d,脚本为monster%d,距离为%d,方向为%d", iMemID,pstTmpMonster->iDefIdx,pstTmpMonster->iDefIdx,iDis,(int)pstTmpMonster->bDirection);
				}
				else
				{
					snprintf(szMsg, CCH(szMsg), "Monster Memid为%d,Defid为%d,没有脚本,距离为%d,方向为%d", iMemID,pstTmpMonster->iDefIdx,iDis,(int)pstTmpMonster->bDirection);
				}
				z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, szMsg);

			}
			break;
			
		case OBJ_PLAYER:
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
			if (!pstMemBlock->fValid)
			{
				continue;
			}
			
			pstTmpOther = (Player*)pstMemBlock->szData;

			iDis = z_distance(&pstPlayer->stRoleData.Pos,&pstTmpOther->stRoleData.Pos);
			if (iDis < iPlayerDis)
			{
				snprintf(szMsg, CCH(szMsg), "玩家Memid为%d,名称为%s,距离为%d", pstTmpOther->iMemID,pstTmpOther->stRoleData.RoleName,iDis);
				z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, szMsg);
			}
			break;
		default:
			break;
		}
			
	}
	for(i = 0 ; i < pstPlayer->stOnline.iVInanimate; i++)
	{
		iMemID = pstPlayer->stOnline.aiVInanimate[i];
		iIndex = z_id_indextype(pstAppCtx, pstEnv, iMemID, &cType);
		if (iIndex < 0 )
		{
			continue;
		}
		switch (cType) 
		{
		case OBJ_NPC:
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstNpcPool, iIndex);
			if (!pstMemBlock->fValid)
			{
				continue;
			}

			pstTmpNpc = (Npc*)pstMemBlock->szData;

			iDis = z_distance(&pstPlayer->stRoleData.Pos,&pstTmpNpc->stMon.stCurrPos);
			if (iDis < iNpcDis)
			{
				snprintf(szMsg, CCH(szMsg), "Npc Memid为%d,Defid为%d,脚本为%s,距离为%d", iMemID,pstTmpNpc->stMon.iDefIdx,pstTmpNpc->szScriptName,iDis);
				z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, szMsg);
			}
			break;
		case OBJ_MONSTER:
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
			if (!pstMemBlock->fValid)
			{
				continue;
			}
	
			pstTmpMonster= (Monster*)pstMemBlock->szData;

			iDis = z_distance(&pstPlayer->stRoleData.Pos,&pstTmpMonster->stCurrPos);
			if (iDis < iMonsterDis)
			{
				if (pstTmpMonster->cMonScriptFlag)
				{
					snprintf(szMsg, CCH(szMsg), "Monster Memid为%d,Defid为%d,脚本为monster%d,距离为%d,方向为%d", iMemID,pstTmpMonster->iDefIdx,pstTmpMonster->iDefIdx,iDis,(int)pstTmpMonster->bDirection);
				}
				else
				{
					snprintf(szMsg, CCH(szMsg), "Monster Memid为%d,Defid为%d,没有脚本,距离为%d,方向为%d", iMemID,pstTmpMonster->iDefIdx,iDis,(int)pstTmpMonster->bDirection);
				}
				z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, szMsg);

			}
			break;
			
		case OBJ_PLAYER:
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
			if (!pstMemBlock->fValid)
			{
				continue;
			}
			
			pstTmpOther = (Player*)pstMemBlock->szData;

			iDis = z_distance(&pstPlayer->stRoleData.Pos,&pstTmpOther->stRoleData.Pos);
			if (iDis < iPlayerDis)
			{
				snprintf(szMsg, CCH(szMsg), "玩家 Memid为%d,名称为%s,距离为%d", pstTmpOther->iMemID,pstTmpOther->stRoleData.RoleName,iDis);
				z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, szMsg);
			}
			break;
		default:
			break;
		}
			
	}

	snprintf(szMsg, CCH(szMsg), "=====nearmemid end=====");
	z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, szMsg);
	
	return 0;
}

int z_gm_levelup(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	char szLevel[32];
	int iLevel;
	LEVELUP *pstLevelUp;
	CSPKG *pstPkg = &pstEnv->stCSRes;
	CSPLAYERATTRVAL *pstAttrVal = &pstPkg->Body.AttrChg;
	CSACTIONRES *pstActionRes = &pstPkg->Body.ActionRes;
	
	if(z_gm_get_val(&pszArg, szLevel, sizeof(szLevel)) < 0)
	{
		return -1;
	}
	
	iLevel = atoi(szLevel);
	if (iLevel < 1 || iLevel > pstEnv->pstConf->MaxLevel  || iLevel > MAX_LEVEL ||iLevel > pstEnv->pstZoneObj->iLevelUpDef)
	{
		return -1;
	}

	/*if (pstEnv->pstGlobalObj->stGlobalObjInfo.iGlobalStat != GLOBAL_FINI ||
				(pstEnv->pstGlobalObj->stGlobal.LevFeng.LevelFeng > 0 && iLevel > pstEnv->pstGlobalObj->stGlobal.LevFeng.LevelFeng))
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "有等级封印");
		return -1;
	}*/
	
	pstLevelUp = (LEVELUP *)pstEnv->pstZoneObj->sLevelUpDef[iLevel-1];

	if (iLevel > CAREERCHG_LEVEL &&
		(pstPlayer->stRoleData.Career&0xF0) == 0)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "需要转职才能升级");
		return -1;
	}

	pstPlayer->stRoleData.Level = iLevel;
	player_update_cache_level(pstEnv, pstPlayer);

	pstAttrVal->AttrNum = 0;
	z_level_up_attr(pstEnv,pstPlayer, pstLevelUp);

	z_update_all_attr(pstAppCtx, pstEnv, pstPlayer);
	z_level_up_attrlist(pstEnv, pstPlayer, pstAttrVal);

	Z_CSHEAD_INIT(&pstPkg->Head, ATTR_CHG);
	z_cltmsg_send(pstEnv, pstPlayer, pstPkg);

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_LEVEL;
	pstActionRes->Data.Level = pstPlayer->stRoleData.Level;
	Z_CSHEAD_INIT(&pstPkg->Head, ACTION_RES);
	z_sendpkg_playerview(pstAppCtx, pstEnv, pstPlayer, 1, pstPkg, 0);

	relation_dbbuddy_chg(pstEnv, pstPlayer, 0,1);
	player_clan_update_mem(pstEnv, pstPlayer);
		
	return 0;
}

int z_gm_clanenemycd(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	int iClanId = 0;
	int iOhterClanID = 0;
	int iSec = 0;
	ZoneClan* pstZoneClan = NULL;
	ZoneClan* pstOtherClan = NULL;

	char szPara[64];

	memset(szPara,0,sizeof(szPara));

	if(z_gm_get_val(&pszArg, szPara, sizeof(szPara)) < 0)
	{
		return -1;
	}
	
	iClanId = atoi(szPara);

	if(z_gm_get_val(&pszArg, szPara, sizeof(szPara)) < 0)
	{
		return -1;
	}
	
	iOhterClanID = atoi(szPara);

	if(z_gm_get_val(&pszArg, szPara, sizeof(szPara)) < 0)
	{
		return -1;
	}
	
	iSec = atoi(szPara);

	// 获取公会对象
	if (!iClanId)
	{
		pstZoneClan = player_get_clan(pstEnv, pstPlayer);
	}
	else
	{
		pstZoneClan = z_id_clan(pstEnv, iClanId);
	}

	pstOtherClan = z_id_clan(pstEnv, iOhterClanID);

	if (!pstZoneClan || !pstOtherClan)
	{
		return -1;
	}

	clan_set_enemy_cd(pstEnv,pstZoneClan,pstOtherClan,iSec);
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}



int z_gm_clancleardip(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	int iClanId = 0;
	ZoneClan* pstZoneClan = NULL;

	char szPara[64];

	memset(szPara,0,sizeof(szPara));

	if(z_gm_get_val(&pszArg, szPara, sizeof(szPara)) < 0)
	{
		iClanId = atoi(szPara);
	}

	// 获取公会对象
	if (!iClanId)
	{
		pstZoneClan = player_get_clan(pstEnv, pstPlayer);
	}
	else
	{
		pstZoneClan = z_id_clan(pstEnv, iClanId);
	}

	if (!pstZoneClan)
	{
		return -1;
	}
	

	clan_clear_diplomatism(pstEnv,pstZoneClan);
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_clanenemycdlist(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	int iClanId = 0;
	ZoneClan* pstZoneClan = NULL;
	int i = 0;
	CLANENEMYCD* pstCD = NULL;
	char szPara[64];
	char szMsg[512];

	memset(szPara,0,sizeof(szPara));

	if(z_gm_get_val(&pszArg, szPara, sizeof(szPara)) < 0)
	{
		iClanId = atoi(szPara);
	}

	// 获取公会对象
	if (!iClanId)
	{
		pstZoneClan = player_get_clan(pstEnv, pstPlayer);
	}
	else
	{
		pstZoneClan = z_id_clan(pstEnv, iClanId);
	}

	if (!pstZoneClan)
	{
		return -1;
	}
	
	pstCD = &pstZoneClan->stClan.ClanInner.ClanEnemyCD;

	snprintf(szMsg, CCH(szMsg), "=====enemy cd list begin=====");
	z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, szMsg);
	
	for (i = 0; i < pstCD->CDNum; i++)
	{
		int iID = (int)pstCD->EnemyCD[i].ClanID;
		int iSec = pstCD->EnemyCD[i].EndTime - pstAppCtx->stCurr.tv_sec;
		
		snprintf(szMsg, CCH(szMsg), "公会ID:%5d	宣战CD时间(秒):%d",iID,iSec);
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, szMsg);
	}

	snprintf(szMsg, CCH(szMsg), "=====enemy cd list end=====");
	z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, szMsg);
	
	return 0;
}


int z_gm_treasurepos(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{

	CSPKG stPkg;
	CSPACKAGEINFO *pstPackageInfo = &stPkg.Body.PackageInfo;

	ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;
	char szArgs[32];
	int iGrididx;
	int iIdx;
	int iPos;
	ROLEITEM *pstRoleItem;
	char szMsg[512];
	
	UNUSED(pstAppCtx);UNUSED(pstEnv);

	

	memset(szMsg,0,sizeof(szMsg));

	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}
	// enum tagLIST_TYPE 
	
	//
	//    LIST_TYPE_STUFF = 1, 	/* 材料包 */
	//    LIST_TYPE_TASK = 2, 	/* 任务包 */
	//    LIST_TYPE_SUNDRIES = 3, 	/* 杂物包 */
	//}
	
	iGrididx = atoi(szArgs);
	if (iGrididx <0 )
	{
		return -1;
	}
	
	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}
	iIdx = atoi(szArgs);
	if (iIdx < 0)
	{
		return -1;
	}

	pstPackageInfo->PackageInfoData.PackageChg.ItemChg.ChgNum = 0;
	pstPackageInfo->PackageInfoData.PackageChg.ArmChg.ChgNum = 0;
	
	pstPackageInfo->Type = PACKAGE_INFO_CHG;
	
	if (iGrididx == LIST_TYPE_SUNDRIES)
	{
		iPos = package_grid_pos(pstPak->SundriesPackage.RoleGrids, pstPak->SundriesPackage.Num,iIdx);
		if (0 > iPos)
		{
			return -1;
		}

		pstRoleItem = &pstPak->SundriesPackage.RoleGrids[iPos].GridData.RoleItem;

		pstPackageInfo->PackageInfoData.PackageChg.ItemChg.ChgNum = 1;
		pstPackageInfo->PackageInfoData.PackageChg.ItemChg.Items[0].ListType = LIST_TYPE_SUNDRIES;
		pstPackageInfo->PackageInfoData.PackageChg.ItemChg.Items[0].Item = *pstRoleItem;
	}
	else if(iGrididx == LIST_TYPE_TASK)
	{
		iPos = package_itemgrid_pos(pstPak->TaskPackage.RoleItems, pstPak->TaskPackage.Num, iIdx);
		if (0 > iPos)
		{
			return -1;
		}

		pstRoleItem = &pstPak->TaskPackage.RoleItems[iPos];

		pstPackageInfo->PackageInfoData.PackageChg.ItemChg.ChgNum = 1;
		pstPackageInfo->PackageInfoData.PackageChg.ItemChg.Items[0].ListType = LIST_TYPE_TASK;
		pstPackageInfo->PackageInfoData.PackageChg.ItemChg.Items[0].Item = *pstRoleItem;
	}
	else
	{
		return -1;
	}

	snprintf(szMsg, CCH(szMsg), "地图:%d,坐标x:%d,坐标y:%d",pstRoleItem->Flag,pstRoleItem->InstVal2,pstRoleItem->InstVal3);
	z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, szMsg);

	Z_CSHEAD_INIT(&stPkg.Head, PACKAGE_INFO);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int z_gm_clearnametimes(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	pstPlayer->stRoleData.MiscInfo.NameChangeTimes = 0;
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}


int z_gm_diffpworld(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	char szID[32];
	char szPworldID[32];
	char szEnterFlag[32];
	CSDIFFPWORLDENTER stEnter;
	UNUSED(pstAppCtx);
	
	if(z_gm_get_val(&pszArg, szID, sizeof(szID)) < 0)
	{
		return -1;
	}

	if(z_gm_get_val(&pszArg, szPworldID, sizeof(szPworldID)) < 0)
	{
		return -1;
	}

	if(z_gm_get_val(&pszArg, szEnterFlag, sizeof(szEnterFlag)) < 0)
	{
		return -1;
	}

	stEnter.DiffPworldID = atoi(szID);
	stEnter.PworldID = atoi(szPworldID);
	stEnter.EnterFlag = atoi(szEnterFlag);
	
	return diff_pworld_enter(pstEnv, pstPlayer, &stEnter);
}

int z_gm_mon_event_clean(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	char szID[32];
	int i, iID;
	WORLDLIMIT *pstWorldLimit = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldLimit;
	WORLDMONEVENT *pstMonEventInfo;
	UNUSED(pstAppCtx);
	UNUSED(pstPlayer);
	
	if(z_gm_get_val(&pszArg, szID, sizeof(szID)) < 0)
	{
		iID = 0;
	}
	else
	{
		iID = atoi(szID);
	}
	
	for(i=pstWorldLimit->MonEventNum-1; i>=0; i--)
	{
		pstMonEventInfo = &pstWorldLimit->MonEventInfo[i];

		if(pstMonEventInfo->ID == iID || iID  == 0)
		{
			pstMonEventInfo->Num = 0;
		}
	}

	return 0;
}

int z_gm_snapshoot(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	Player *pstPlayerOther;
	char szName[32];
	CSPKG stPkg;
	CSACTIONRES *pstAction = &stPkg.Body.ActionRes;

	if( z_gm_get_val(&pszArg, szName, sizeof(szName))<0)
	{
		return -1;
	}

	pstPlayerOther = z_name_player(pstAppCtx, pstEnv, szName);
	if (NULL == pstPlayerOther)
	{
		return -1;
	}

	memset(pstAction,0,sizeof(*pstAction));

	pstAction->ID = pstPlayerOther->iMemID;
	pstAction->ActionID = ACTION_CLIENT_CMD;
	
	pstAction->Data.ClientCmd.CmdType = CLIENT_CMD_SNAPSHOOT;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayerOther, &stPkg);
	

	return 0;
	UNUSED(pstPlayer);	//add by paraunused.pl
}

int z_gm_world_ore_clear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	int i;
	Monster *pstMon;
	WORLDLIMIT *pstWorldLimit = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldLimit;

	for(i=0; i<MAX_WORLD_ORE; i++)
	{
		pstMon = z_id_monster(pstAppCtx, pstEnv, pstWorldLimit->Ores[i].Ore.MemID);
		if(NULL == pstMon)
		{
			continue;
		}
		
		global_world_ore_del(pstEnv, pstMon);
		//地图上拿掉
		z_monster_leave_map(pstEnv->pstAppCtx, pstEnv, pstMon);
		z_free_monster(pstEnv->pstAppCtx, pstEnv, pstMon);
	}

	memset(&pstWorldLimit->Ores[0], 0, sizeof(pstWorldLimit->Ores[0])*MAX_WORLD_ORE);
	
	return 0;
	UNUSED(pstPlayer);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_world_city(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{	
	char szArgs[32];

	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}

	if(strcasecmp(szArgs, "reset") == 0)
	{
		pstEnv->pstGlobalObj->stGlobal.Misc.WorldBossFirstKill = 0;
		return world_city_reset(pstEnv);
	}
	else if(strcasecmp(szArgs, "open") == 0)
	{
		pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity.CityCore.FightEnableTime = pstEnv->pstAppCtx->stCurr.tv_sec;
		return world_city_open(pstEnv);
	}
	else if(strcasecmp(szArgs, "fight") == 0)
	{
		
		return world_city_siege_set(1);
	}
	else if(strcasecmp(szArgs, "fightend") == 0)
	{
		return world_city_siege_set(0);
	}
	else if(strcasecmp(szArgs, "clear") == 0)
	{
		WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;
		if (!is_in_siege_city_fast(pstEnv))
		{
			pstWorldCity->CityCore.Stat = WORLD_CITY_STAT_CLEAR;
			return 0;
		}
		else
		{
			return -1;
		}
	}
	else if(strcasecmp(szArgs, "owner") == 0)
	{
		WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;
		
		pstEnv->pstGlobalObj->stGlobal.Misc.WorldBossFirstKill = 0;
		world_city_reset(pstEnv);
		world_city_open(pstEnv);
		pstWorldCity->CityCore.ClanId = pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Id;
		pstWorldCity->CityCore.ClanGID = pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.GID;
		STRNCPY(pstWorldCity->CityCore.ClanName, pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Name, sizeof(pstWorldCity->CityCore.ClanName));
		world_city_refresh(pstEnv);

		{
			ZoneClan *pstZoneClan;
			pstZoneClan = z_id_clan(pstEnv, pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Id);
			if (pstZoneClan)
			{
				clan_boon_strong_chg_update(pstEnv,4404, pstZoneClan, 2);	
			}
		}
		
		
		return 0;
	}
	else if(strcasecmp(szArgs, "buff") == 0)
	{
		player_world_city_buff_update(pstEnv,pstPlayer);
		return 0;
	}
	else
	{
		return -1;
	}
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_lucky(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szArgs[32];
	int iOp = 0;

	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}

	iOp = atoi(szArgs);

	if (iOp)
	{
		pstEnv->iMiscFlag |= GAME_FLAG_LUCK;
	}
	else
	{
		pstEnv->iMiscFlag &= ~GAME_FLAG_LUCK;
	}
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstPlayer);	//add by paraunused.pl
}

int z_gm_enter_clan_city(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szArgs[32];
	int iClanID;
	int iTransID = 0;
	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		return -1;
	}

	iClanID = atoi(szArgs);

	if(0 == z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		iTransID = atoi(szArgs);	
	}	
	
	player_enter_clan_city_clt(pstEnv, pstPlayer,iClanID,iTransID);
	UNUSED(pstAppCtx);
	return 0;
}

int z_gm_vipbarmul(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szArgs[32];
	int iDropMul = 0;
	int iExpMul = 0;
	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		
		return -1;
	}
	iDropMul = atoi(szArgs);

	if(0 > z_gm_get_val(&pszArg, szArgs, sizeof(szArgs)))
	{
		
		return -1;
	}
	iExpMul = atoi(szArgs);

	pstPlayer->stOnline.stVipBarInfo.iVipBarDropMul = iDropMul;
	pstPlayer->stOnline.stVipBarInfo.iVipBarExpMul = iExpMul;
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
}


int z_gm_skill_misstime(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer,
                     const char* pszArg)
{
	char szArg[32];
	int iTime;
	UNUSED(pstAppCtx);
	
	if(z_gm_get_val(&pszArg, szArg, sizeof(szArg)) < 0)
	{
		return -1;
	}

	iTime = atoi(szArg);
	g_iMissTime = iTime;
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstPlayer);	//add by paraunused.pl
}

int z_gm_runeattr(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szArg[32];
	int iType;
	int iID;
	RUNEADDATTRDEF* pstDef = NULL;
	
	if(z_gm_get_val(&pszArg, szArg, sizeof(szArg)) < 0)
	{
		return -1;
	}

	iType = atoi(szArg);

	if (iType <= RUNE_EMPTY || iType > RUNE_TYPE_NUM)
	{
		return -1;
	}

	if(z_gm_get_val(&pszArg, szArg, sizeof(szArg)) < 0)
	{
		return -1;
	}

	iID = atoi(szArg);

	pstDef = z_find_run_addattr_def(pstEnv,iID);

	if (!pstDef)
	{
		return -1;
	}
	player_rune_addattr_set(pstEnv,pstPlayer, iID, iType);

	player_rune_ex_enable_info(pstEnv,pstPlayer);
		
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_clantouch(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	int iToday = 0;
	int iYesterday = 0;
	char szArg[32];
	ZoneClan *pstZoneClan = NULL;
	
	if(z_gm_get_val(&pszArg, szArg, sizeof(szArg)) < 0)
	{
		return -1;
	}
	
	iToday = atoi(szArg);

	if(z_gm_get_val(&pszArg, szArg, sizeof(szArg)) < 0)
	{
		return -1;
	}
	
	iYesterday = atoi(szArg);

	pstZoneClan = player_get_clan(pstEnv,pstPlayer);
	if (!pstZoneClan)
	{
		return -1;
	}

	pstZoneClan->stClan.ClanCore.ClanTouchNum = iToday;
	pstZoneClan->stClan.ClanCore.ClanTouchTime = pstAppCtx->stCurr.tv_sec;
	
	pstZoneClan->stClan.ClanCore.YesterdayClanTouchNum = iYesterday;
	pstZoneClan->stClan.ClanCore.YesterdayClanTouchTime = pstAppCtx->stCurr.tv_sec - 86400;

	player_clan_yesterday_clan_touch_notify_broadcast(pstEnv,pstZoneClan);
	
	return 0;
}

int z_gm_cond(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szArg[32];
	ACHIEVEMENTCOND stCond;

	memset(&stCond,0,sizeof(stCond));
	
	if(z_gm_get_val(&pszArg, szArg, sizeof(szArg)) < 0)
	{
		return -1;
	}
	
	stCond.Type = atoi(szArg);

	if(z_gm_get_val(&pszArg, szArg, sizeof(szArg)) == 0)
	{
		stCond.Val1 = atoi(szArg);
	}
		
	if(z_gm_get_val(&pszArg, szArg, sizeof(szArg)) == 0)
	{
		stCond.Val2 = atoi(szArg);
	}
	
	if(z_gm_get_val(&pszArg, szArg, sizeof(szArg)) == 0)
	{
		stCond.Val3 = atoi(szArg);
	}

	return z_player_achievement_cond(pstEnv,pstPlayer,&stCond);

	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_achievementreset(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	ACHIEVEMENTINFO* pstInfo = &pstPlayer->stRoleData.MiscInfo.AchievementInfo;

	pstInfo->Num = 0;
	z_player_achievement_online(pstEnv,pstPlayer);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

/*	-lxk
	功能:清除每日上限的积累值
*/
int z_gm_clear_dayexp(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	UNUSED(pstAppCtx);
	UNUSED(pstEnv);
	UNUSED(pszArg);
	pstPlayer->stRoleData.MiscInfo.DayExpCount = 0;
	z_sys_strf(pstEnv, pstPlayer, SYS_TIP, "测试用的");
	return 0;
}

int z_gm_machine_chg_map(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szArg[32];
	int iMap;
	RESPOS stPos;
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst;
	MapIndex*pstMapIdx;
	Monster *pstMachineMon;
//	int i;
//	RESPOS stSearchPos;

	if( z_gm_get_val(&pszArg, szArg, sizeof(szArg)) < 0 )
	{
		return 0;
	}
	iMap = atoi(szArg);

	if( z_gm_get_val(&pszArg, szArg, sizeof(szArg)) < 0 )
	{
		return 0;
	}
	stPos.X = atoi(szArg);

	if( z_gm_get_val(&pszArg, szArg, sizeof(szArg)) < 0 )
	{
		return 0;
	}
	stPos.Y = atoi(szArg);

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst	=	z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if(!pstMapInst)
		return 0;

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (!pstMapIdx) 
		return -1;
	
	pstMachineMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.stMachineInfo.iMachineMonID);
	if(pstMachineMon == NULL)
	{
		return 0;
	}
	pstMachineMon->iPatrolID = 16;

	player_goto( pstEnv, pstPlayer, iMap, stPos.X, stPos.Y,pstPlayer->stOnline.iMapPos);
	//machine_passenger_info(pstEnv, pstMachineMon);
	mon_ai_move( pstEnv,  pstMachineMon);
	return 0;
}

/*	-lxk
	功能:清除角色每日标识
*/
int z_gm_clear_daily_flag(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	UNUSED(pstAppCtx);
	UNUSED(pstEnv);
	UNUSED(pszArg);
	memset(&pstPlayer->stRoleData.MiscInfo.DailyFlag,0,
			sizeof(pstPlayer->stRoleData.MiscInfo.DailyFlag));
			
	z_sys_strf(pstEnv, pstPlayer, SYS_TIP, "清除角色每日标识成功");
	return 0;
}

/*	-lxk
	功能:结合以下两个条件获取地图的怪物数量
	1、mapid = 0 表示当前地图，否则指定的地图ID
	2、monid = 0 表示所有怪物，否则指定的怪物ID
*/
int z_gm_get_mon_num(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	
	
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst; 
	char szArg[32];
	char szMsg[512];
	int iMapID;
	int iMonID;
	int i, j, iDyn;
	int iCount = 0;
	AREADYN *pstAreaDyn; 
	Monster *pstMon;

	/*命令不带参数*/
	if( z_gm_get_val(&pszArg, szArg, sizeof(szArg)) < 0 )
	{
		stIdx.iID = pstPlayer->stRoleData.Map;
		stIdx.iPos = pstPlayer->stOnline.iMapPos;
		pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
		if( NULL == pstMapInst )
		{
			goto _error;
		}
		iCount = pstMapInst->iMonsterCount;
		goto _succ;
	}
	
	iMapID = atoi(szArg);
	if( iMapID < 0 )
	{
		goto _error;
	}
	if( z_gm_get_val(&pszArg, szArg, sizeof(szArg)) < 0 )
	{
		iMonID = 0;
	}
	iMonID = atoi(szArg);
	if( iMonID < 0 )
	{
		goto _error;
	}
	/*参数为0表示读取角色所在地图信息*/	
	if ( iMapID == 0 )
	{
		stIdx.iID = pstPlayer->stRoleData.Map;
		stIdx.iPos = pstPlayer->stOnline.iMapPos;
		pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	}
	else
	{
		stIdx.iID = iMapID;
		stIdx.iPos = -1;
		pstMapInst = z_find_map_inst( pstEnv->pstAppCtx, pstEnv, &stIdx);
	}
	if( NULL == pstMapInst )
	{
		goto _error;
	}

	if( iMonID == 0 )
	{
		iCount = pstMapInst->iMonsterCount;
		goto _succ;
	}
	for (i = 0; i < pstMapInst->iAreaDyn; i++)
	{
		iDyn = pstMapInst->aiAreaDyn[i];
		if(iDyn < 0)
		{
			continue;
		}

		pstAreaDyn = (AREADYN*)tmempool_get(pstEnv->pstAreaDynPool, iDyn);
		if (NULL == pstAreaDyn)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "Dyn=%d", iDyn);
			continue;
		}

		for(j = 0; j < pstAreaDyn->iAnimate; j++)
		{
			if( j >= MAX_AREA_ANIMATE )
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "map %d dyn %d iAnimate=%d", 
					       pstMapInst->iMapInstID, iDyn, pstAreaDyn->iAnimate);
				break;
			}

			pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstAreaDyn->aiAnimate[j]);
			if (pstMon &&  pstMon->iDefIdx == iMonID)
			{				
				iCount ++;
			}
		}
	}

	_succ:
		snprintf(szMsg, CCH(szMsg), "符合条件的怪物数量为:%d",iCount);
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, szMsg);
		return 0;
	_error:
		return -1;

		UNUSED(pstAppCtx);
}

/*	-lxk
	功能:获取角色所在地图上NPC的数量
	1、npcid = 0 表示所有NPC，否则获取指定ID的NPC数量
*/
int z_gm_get_npc_num(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	
	int iNpcID = 0;
	int i, k, iDyn;
	int iID, iIndex;
	char cType;
	char szArg[32];
	char szMsg[512];
	ZONEMAPINST *pstMapInst;
	AREADYN *pstAreaDyn;
	TMEMBLOCK *pstMemBlock;
	Npc *pstNpc;
	int iNum=0;
	ZONEIDX stIdx;
	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if ( NULL == pstMapInst )
	{
		return -1;
	}

	if(z_gm_get_val(&pszArg, szArg, sizeof(szArg)) == 0 )
	{
		iNpcID = atoi(szArg);
	}
	
	if(iNpcID < 0)
	{
		return -1;
	}
	for (i = pstMapInst->iAreaDyn-1; i >= 0; i--)
	{
		iDyn = pstMapInst->aiAreaDyn[i];

		if( iDyn < 0 )
		{
			continue;
		}

		pstAreaDyn = (AREADYN*)tmempool_get(pstEnv->pstAreaDynPool, iDyn);
		if ( NULL == pstAreaDyn )
		{
			continue;
		}

		for(k = pstAreaDyn->iAnimate-1; k >= 0; k--)
		{
			iID	=	pstAreaDyn->aiAnimate[k];
			iIndex	=	z_id_indextype(pstEnv->pstAppCtx, pstEnv, iID, &cType);

			if( -1 == iIndex )
			{
				continue;
			}

			switch(cType)
			{
				case OBJ_NPC:
					pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstNpcPool, iIndex);
					if (!pstMemBlock->fValid)
					{
						break;
					}
					pstNpc = (Npc *)pstMemBlock->szData;

					if (iNpcID == 0 || pstNpc->stMon.iDefIdx == iNpcID)
					{
						iNum++;
					}
					
					break;
				default:
					break;
			}
		}
	}

	snprintf(szMsg, CCH(szMsg), "地图上的符合条件的NPC数量:%d",iNum);
	z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, szMsg);
	return 0;

	UNUSED(pstAppCtx);
}

/*	-lxk
	功能:测试各类系统提示显示方式
	1、有效值 0 ~ 14
*/
int z_gm_show_sys_tip(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	
	char szArg[32];
	int iType = 4;
	if(0 == z_gm_get_val(&pszArg, szArg, sizeof(szArg)))
	{
		iType = atoi(szArg);
	}
	if(iType < 0 || iType > 18)
	{
		return -1;
	}
	z_sys_strf(pstEnv, pstPlayer, iType, "[测试提示方式:0~14] - [%d]",iType);
	return 0;
	UNUSED(pstAppCtx);
}

int z_gm_goto_pvp_map(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szArg[32];
	char szArg2[32];
	char szArg3[32];
	int iMapID = -1;
	RESPOS stPos;
	int iMapInstID;
	MapIndex *pstMapIdx;
	int iRet;
	
	if(0 == z_gm_get_val(&pszArg, szArg, sizeof(szArg)))
	{
		iMapID = atoi(szArg);
	}

	if(iMapID <= 0)
	{
		return -1;
	}

	if(0 == z_gm_get_val(&pszArg, szArg2, sizeof(szArg2)) &&
		0 == z_gm_get_val(&pszArg, szArg3, sizeof(szArg3)))
	{
		stPos.X = atoi(szArg2);
		stPos.Y = atoi(szArg3);
	}
	else
	{
		return -1;
	}

	if (iMapID >= PWORLD_MAPID_BASE)
	{
		iMapInstID = (pstPlayer->stRoleData.Map & 0xffff0000) | iMapID;
	}
	else
	{
		iMapInstID = iMapID;
	}

	iRet = team_trans_check_one(pstEnv,pstPlayer);
	team_trans_check_tip(pstEnv, pstPlayer,1, iRet, NULL);
	if(0 > iRet)
	{
		return -1;
	}

	if(is_battle_pvp_svr(pstEnv))
	{
		//pvp_battle -> zone
		iRet = z_player_change_map(pstAppCtx, pstEnv, pstPlayer, iMapInstID, &stPos, RAND1(180), 0);

		if(iRet < 0)
		{
			return -1;
		}
	}
	else
	{
		//zone -> pvp_battle
		pstMapIdx = z_mappos_index(pstEnv->pstMapObj, NULL, iMapInstID);
		if(NULL == pstMapIdx)
		{
			return -1;
		}

		if(0 > player_chg_map_passenger_check(pstEnv,pstPlayer, pstMapIdx))
		{
			return -1;
		}

		if(INST_MODE_BATTLE_SVR == pstEnv->iInstMode)
		{
			return -1;
		}

		if(0 == player_span_map_check(pstEnv,pstPlayer, pstMapIdx, &stPos))
		{
			return -1;
		}
	}

	//return player_enter_battle_pvp_map_req(pstEnv, pstPlayer, iMapID);
	return 0;
	UNUSED(pstAppCtx);
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstPlayer);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

/*	-lxk
	功能:增加角色公会入侵活动积分
*/
int z_gm_add_everyday_val(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	
	char szArg[32];
	int iVal = 0;
	
	if(0 == z_gm_get_val(&pszArg, szArg, sizeof(szArg)))
	{
		iVal = atoi(szArg);
	}

	if(iVal <= 0)
	{
		return -1;
	}
	
	return player_clan_everyday_pworld_val_chg(pstEnv, pstPlayer, iVal);
	UNUSED(pstAppCtx);
}

int z_gm_aasflag(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szArg[32];
	int iVal = 0;
	
	if(0 == z_gm_get_val(&pszArg, szArg, sizeof(szArg)))
	{
		iVal = atoi(szArg);
	}

	pstPlayer->bAasFlag = (unsigned char)iVal;
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
}
int z_gm_wavespeed(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szArg[32];
	int iType = 0;
	int iSec = 0;
	ZONEPWORLDINST *pstPworldInst = NULL;
	PWORLDROUNDMONINFO *pstMonInfo = NULL;
	PWORLDROUNDMON *pstRound = NULL;
	
	if(0 == z_gm_get_val(&pszArg, szArg, sizeof(szArg)))
	{
		iType = atoi(szArg);
	}
		
	if(0 == z_gm_get_val(&pszArg, szArg, sizeof(szArg)))
	{
		iSec = atoi(szArg);
	}

	pstPworldInst = z_find_pworldinst2(pstEnv, pstPlayer->stRoleData.Map);

	if (!pstPworldInst)
	{
		return -1;
	}

	if (iType == 0)
	{
		pstRound = &pstPworldInst->stWaveRound;
	}
	else if (iType == 1)
	{
		pstRound = &pstPworldInst->stTimerRound;
	}
	else
	{
		return -1;
	}

	if (pstRound->iTotalRound <= 0)
	{	
		return -1;
	}


	if (pstRound->iCurrRound < 0 &&
		pstRound->iCurrRound >= PWORLD_MON_ROUND_MAX &&
		pstRound->iCurrRound >= pstRound->iTotalRound)
	{
		return -1;
	}

	pstMonInfo = &pstRound->aRoundMonInfo[pstRound->iCurrRound];

	pstMonInfo->iRefreshTime = pstEnv->pstAppCtx->stCurr.tv_sec + iSec;
	pworld_wave_countdown_action(pstEnv,pstPworldInst);
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}
int z_gm_clearmachinepworldlimit(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	ZoneClan* pstZoneClan = NULL;

	pstZoneClan = player_get_clan(pstEnv, pstPlayer);
	if(NULL == pstZoneClan)
	{
		return -1;
	}
	pstZoneClan->stClan.ClanCore.MachinePworldLastOpenTime = 0;
	pstZoneClan->stClan.ClanCore.MachinePworldWeekTimes = 0;
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_waveset(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szArg[32];
	int iType = 0;
	int iWave = 0;
	ZONEPWORLDINST *pstPworldInst = NULL;
	PWORLDROUNDMONINFO *pstMonInfo = NULL;
	PWORLDROUNDMON *pstRound = NULL;
	
	if(0 == z_gm_get_val(&pszArg, szArg, sizeof(szArg)))
	{
		iType = atoi(szArg);
	}
	else
	{
		return -1;
	}
		
	if(0 == z_gm_get_val(&pszArg, szArg, sizeof(szArg)))
	{
		iWave = atoi(szArg);
	}
	else
	{
		return -1;
	}

	pstPworldInst = z_find_pworldinst2(pstEnv, pstPlayer->stRoleData.Map);

	if (!pstPworldInst)
	{
		return -1;
	}

	if (iType == 0)
	{
		pstRound = &pstPworldInst->stWaveRound;
	}
	else if (iType == 1)
	{
		pstRound = &pstPworldInst->stTimerRound;
	}
	else
	{
		return -1;
	}

	if (pstRound->iTotalRound <= 0)
	{	
		return -1;
	}

	iWave -= 1;

	pstRound->iCurrRound = iWave - 1;

	if (iWave < 0 &&
		iWave >= PWORLD_MON_ROUND_MAX &&
		iWave >= pstRound->iTotalRound)
	{
		return -1;
	}

	pstRound->iCurrRound = iWave;

	pstMonInfo = &pstRound->aRoundMonInfo[pstRound->iCurrRound];

	//pstMonInfo->iRefreshTime = pstEnv->pstAppCtx->stCurr.tv_sec + iSec;
	pworld_wave_countdown_action(pstEnv,pstPworldInst);
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl

}
int z_gm_cowrangelist(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	COWFINALRANGE *pstRange = NULL;
	char szArg[32];
	int iType = 0;
	int i = 0;

	if(0 == z_gm_get_val(&pszArg, szArg, sizeof(szArg)))
	{
		iType = atoi(szArg);
	}
		

	pstRange = &pstEnv->pstGlobalObj->stGlobal.Misc.CowFinalRange;

	if (1 == iType)
	{
		pstRange = &pstEnv->pstGlobalObj->stGlobal.Misc.CowFinalRangeDraft;
	}

	z_sys_strf(pstEnv, pstPlayer,SYS_SVR, "牛仔决赛排行榜,类型:%d,时间:%d",iType,pstRange->RangeTime);
	for (i = 0; i < pstRange->Num;i++)
	{
		COWFINALRANGEMEMBER* pstMember = &pstRange->Members[i];
		
		
		z_sys_strf(pstEnv, pstPlayer,SYS_SVR, "姓名:%s,时间:%d,排名:%d,流水号:%d,奖励领取:%d",
					pstMember->Name,pstMember->Time,pstMember->Range,pstMember->RecordID,
					pstMember->TakeAward);
	}
	z_sys_strf(pstEnv, pstPlayer,SYS_SVR, "牛仔决赛排行榜结束");
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}
int z_gm_showequipscore(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szArg[32];
	int iType = 0;

	if(0 == z_gm_get_val(&pszArg, szArg, sizeof(szArg)))
	{
		iType = atoi(szArg);
	}

	if (iType == 1)
	{
		pstPlayer->iShowEquipScore = 1;
	}
	else
	{
		pstPlayer->iShowEquipScore = 0;
	}
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
}

int z_gm_rolesnapshot(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	z_role_snapshot_oplog(pstAppCtx, pstEnv, pstPlayer);
	return 0;
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_clanbattleaward(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szArg[32];
	int iDefID = 0;
	int iWin = 0;
	
	if(0 == z_gm_get_val(&pszArg, szArg, sizeof(szArg)))
	{
		iDefID = atoi(szArg);
	}	
	else
	{
		return -1;
	}

	if(0 == z_gm_get_val(&pszArg, szArg, sizeof(szArg)))
	{
		iWin = atoi(szArg);
	}

	clan_battle_award_in(pstEnv,&pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId,iDefID, iWin,0);

	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_smscode(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	int iTime = pstPlayer->stOnline.iSmsTimeOut - pstAppCtx->stCurr.tv_sec;
	char szArg[32];
	int iType = 0;
	
	if(0 == z_gm_get_val(&pszArg, szArg, sizeof(szArg)))
	{
		iType = atoi(szArg);
	}	

	if (iTime < 0)
	{
		iTime = 0;
	}
	
	if (0 == iType)
	{
		z_sys_strf(pstEnv, pstPlayer,SYS_SVR, "短信验证密码:%d 有效时间:%d",pstPlayer->stOnline.iSmsCode,iTime);
	}
	else if (1 == iType)
	{
		player_action_precreate_gift_verify(pstEnv,pstPlayer,
						ACTION_PRECREATE_GIFT_VERIFY_OP_OPEN,0);
	}
	else
	{
		pstPlayer->UinFlag &= ~UIN_FLAG_PRECREATE_GIFT;
	}
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_beachsec(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szArg[32];
	int iTime = 0;
	BEACHINFO *pstBeachInfo = &pstPlayer->stRoleData.MiscInfo.BeachInfo;
	
	
	
	if(0 == z_gm_get_val(&pszArg, szArg, sizeof(szArg)))
	{
		iTime = atoi(szArg);
	}	

	if (iTime < 0)
	{
		iTime = 0;	
		z_sys_strf(pstEnv, pstPlayer,SYS_SVR, "当前沙滩秒数:%d",pstBeachInfo->Sec);
	}
	else
	{
		pstBeachInfo->Sec = (iTime/BEACH_EXP_INTERVAL)*BEACH_EXP_INTERVAL;
		player_beach_info_action(pstEnv, pstPlayer);
	}
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_onedaygiftreset(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	clean_oneday_gift_info(pstEnv, pstPlayer);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}


int z_gm_cleanbct(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	BAICENGHISTORY *pstHistory = &pstEnv->pstGlobalObj->stGlobal.Misc.BaiCengHistory;

	memset(pstHistory, 0, sizeof(*pstHistory));
	return 0;
	UNUSED(pstPlayer);	//add by paraunused.pl
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_decbull(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szArg[32];
	int iVal = 0;
	int iRet;
	
	if(0 == z_gm_get_val(&pszArg, szArg, sizeof(szArg)))
	{
		iVal = atoi(szArg);
	}
	else
	{
		return -1;
	}

	iRet = package_dec_money(pstEnv, pstPlayer, ATTR_ID_BULL, iVal);
	z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_GM, 0, iVal-iRet, iRet, "gm减");

	return iRet;
	UNUSED(pstPlayer);	//add by paraunused.pl
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_sham_dead(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	player_sham_dead(pstEnv, pstPlayer);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_sj_additem(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szNum[32];
	int iNum;

	if(z_gm_get_val(&pszArg, szNum, sizeof(szNum)) < 0)
	{
		return -1;
	}

	iNum = atoi(szNum);
	if (iNum <= 0 )
		return -1;

	pstPlayer->stRoleData.MiscInfo.SJTaskInfo.GetItemNum+=iNum;
	if (pstPlayer->stRoleData.MiscInfo.SJTaskInfo.GetItemNum > SJ_MAX_SUIPIAN_WEEK)
	{
		pstPlayer->stRoleData.MiscInfo.SJTaskInfo.GetItemNum = SJ_MAX_SUIPIAN_WEEK;
	}
	
	player_sj_item_info(pstEnv, pstPlayer);
	return 0;
}

int z_gm_trade(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szOpt[128];
	char szMsg[10240];

	if(z_gm_get_val(&pszArg, szOpt, sizeof(szOpt)) < 0)
	{
		return -1;
	}

	szMsg[0] = 0;

	if (strcasecmp(szOpt, "list") == 0)
	{
		gm_trade_list(pstEnv, NULL, NULL, szMsg, sizeof(szMsg));
	}
	else if (strcasecmp(szOpt, "do") == 0)
	{
		char szOpt1[128];

		if(z_gm_get_val(&pszArg, szOpt1, sizeof(szOpt1)) < 0)
		{
			return -1;
		}
		
		gm_trade_do(pstEnv, atoi(szOpt1), szMsg, sizeof(szMsg));
	}
	else if (strcasecmp(szOpt, "cancel") == 0)
	{
		char szOpt1[128];
		char szOpt2[128];

		if(z_gm_get_val(&pszArg, szOpt1, sizeof(szOpt1)) < 0)
		{
			return -1;
		}

		if(z_gm_get_val(&pszArg, szOpt2, sizeof(szOpt2)) < 0)
		{
			return -1;
		}
		
		gm_trade_cancel(pstEnv, atoi(szOpt1), atoull(szOpt2), szMsg, sizeof(szMsg));
	}
	else
	{
		return -1;
	}

	if (szMsg[0] != 0)
	{
		z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, szMsg);
	}
		
	return 0;
}

int z_gm_dayexpwarning(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szBuff[128];
			
	snprintf(szBuff, CCH(szBuff), "RoleName=%s, RoleID=%llu, DayExp=%d, DayExp Warning", 
		pstPlayer->stRoleData.RoleName, pstPlayer->stRoleData.RoleID, pstPlayer->stRoleData.MiscInfo.DayExpCount);
	z_game_notify(pstEnv, szBuff, CCH(szBuff));

	UNUSED(pstAppCtx);	
	UNUSED(pszArg);	
	return 0;
}

int z_gm_dart_reset(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	DARTCARINFO *pstDartInfo = &pstPlayer->stRoleData.MiscInfo.DartCarInfo;
	pstDartInfo->DayTime = 0;

	UNUSED(pstAppCtx);	
	UNUSED(pszArg);	
	UNUSED(pstEnv);	
	return 0;
}

int z_gm_pet_add_impexp(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szIdx[32];
	int iIdx;
	ROLEPET *pstRolePet;
	PETPACKAGE *pstPetPackage = &pstPlayer->stRoleData.Package.PetPackage;
	int iExp = 0;
	
	
	if(z_gm_get_val(&pszArg, szIdx, sizeof(szIdx)) < 0)
	{
		return -1;
	}

	iIdx = atoi(szIdx);

	if(z_gm_get_val(&pszArg, szIdx, sizeof(szIdx)) < 0)
	{
		return -1;
	}
	iExp = atoi(szIdx);

	if(iIdx < 0 || iIdx >= pstPetPackage->Num)
	{
		return -1;
	}
	pstRolePet = &pstPetPackage->RolePets[iIdx];

	pstRolePet->ImpExp+=iExp;

	pet_update_to_clt(pstEnv, pstPlayer, pstRolePet);
	return 0;
}

int z_gm_itemoneday_clear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	USEITEMONEDAY *pstUseItemOneDay = &pstPlayer->stRoleData.MiscInfo.UseItemOneDay;
	
	memset(pstUseItemOneDay, 0, sizeof(*pstUseItemOneDay));

	UNUSED(pstAppCtx);	
	UNUSED(pszArg);	
	UNUSED(pstEnv);	
	return 0;
}

int z_gm_effigy_worship_clear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	pstPlayer->stRoleData.MiscInfo.WorshipInfo.WorshipTime = 0;

	UNUSED(pstAppCtx);	
	UNUSED(pszArg);	
	UNUSED(pstEnv);	
	return 0;
}

int z_gm_sparta(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	player_sparta_apply_req(pstEnv, pstPlayer);

	UNUSED(pstAppCtx);	
	UNUSED(pszArg);	
	return 0;
}

int z_gm_clan_baby_clear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	ZoneClan *pstClan = player_get_clan(pstEnv, pstPlayer);
	if(!pstClan)
	{
		return 0;
	}

	clan_baby_clear_all(pstClan);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_vip_clear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	VIPDATA *pstVipData = &pstPlayer->stRoleData.MiscInfo.VipData;

	memset(pstVipData, 0, sizeof(*pstVipData));

	star_stardata_chg(pstEnv, pstPlayer, 1);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}


int z_gm_cont_cover_clear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	CONTCOVERGIFTINFO *pstInfo = &pstPlayer->stRoleData.MiscInfo.ContCoverGiftInfo;

	memset(pstInfo, 0, sizeof(*pstInfo));
	

	player_cont_cover_gift_notify(pstEnv, pstPlayer, 0,0);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_vip_set(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szArg[32];
	int iVipID = 0;
	int iSec = 0;
	int i = 0;
	VIPDATA *pstVipData = &pstPlayer->stRoleData.MiscInfo.VipData;
	
	if(0 == z_gm_get_val(&pszArg, szArg, sizeof(szArg)))
	{
		iVipID = atoi(szArg);
	}
	else
	{
		return -1;
	}
		
	if(0 == z_gm_get_val(&pszArg, szArg, sizeof(szArg)))
	{
		iSec = atoi(szArg);
	}
	else
	{
		return -1;
	}

	for(i=0;i<pstVipData->StoneNum;i++)
	{
		if( iVipID == pstVipData->StarStoneInfo[i].ID)
		{
			pstVipData->StarStoneInfo[i].EndTime = pstAppCtx->stCurr.tv_sec + iSec;

			star_stardata_chg(pstEnv, pstPlayer, 1);
			return 0;
		}
	}
	
	if(pstVipData->StoneNum >= MAX_STONE_NUM)
	{
		return -1;
	}

	pstVipData->StarStoneInfo[pstVipData->StoneNum].ID = iVipID;
	pstVipData->StarStoneInfo[pstVipData->StoneNum].EndTime = pstEnv->pstAppCtx->stCurr.tv_sec+iSec;
	pstVipData->StoneNum++;
	star_stardata_chg(pstEnv, pstPlayer, 1);
	return 0;
}

int z_gm_machine_break(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	player_ride_machine_brk(pstEnv, pstPlayer, 1);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_setonlinesec(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szArg[64];
	int iSec = 0;
	
	if(0 == z_gm_get_val(&pszArg, szArg, sizeof(szArg)))
	{
		iSec = atoi(szArg);
	}
	else
	{
		return -1;
	}
	pstPlayer->stRoleData.MiscInfo.ODTime.OndaySec = iSec;
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
}

int z_gm_fini_taskcycle(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	TASKRUNONE *pstTaskRunOne;
	TASKRECYCLEDEF * pstTaskRecycleDef;
	TASKRECYCLEONE *pstTaskRecycleOne;
	TASKCYCLEONEDAYINFO *pstCycOneDayInfo;
	char szArg[64];
	int iID = 0;
	
	if(0 == z_gm_get_val(&pszArg, szArg, sizeof(szArg)))
	{
		iID = atoi(szArg);
	}
	else
	{
		return -1;
	}
	
	pstTaskRecycleDef = taskcycle_def_find(pstEnv, iID, NULL);
	if(!pstTaskRecycleDef)
	{
		return 0;
	}

	pstTaskRecycleOne = taskcycle_run_find(pstPlayer, pstTaskRecycleDef->ID);
	if(pstTaskRecycleOne)
	{
		pstTaskRunOne = task_run_find(pstPlayer, pstTaskRecycleOne->TaskID);
		if(pstTaskRunOne || pstTaskRecycleOne->TaskRecycleIdx > 0)
		{
			return 0;
		}
	}

	pstCycOneDayInfo =  taskcycle_find_oneday(pstPlayer, pstTaskRecycleDef->ID);
	if (pstCycOneDayInfo && pstCycOneDayInfo->OneDayNum > 0)
	{
		return 0;
	}

	if (0 > taskcycle_accept_check(pstEnv, pstPlayer, pstTaskRecycleDef, 0))	
	{
		return 0;
	}

	if (pstTaskRecycleDef->OneDayNum > 0 || pstTaskRecycleDef->WeekNum >0)
	{
		taskcycle_add_oneday(pstEnv, pstPlayer, pstTaskRecycleDef->ID);
	}

	taskcycle_fini_trace(pstEnv, pstPlayer, pstTaskRecycleDef->ID);

	task_info_all(pstEnv, pstPlayer);

	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_yanghaibang_set(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szArg[64];
	int iTime = 0;
	YANGHAIBANGINFO *pstYangHaiBangInfo = &pstPlayer->stRoleData.MiscInfo.YangHaiBangInfo;
	
	if(0 == z_gm_get_val(&pszArg, szArg, sizeof(szArg)))
	{
		iTime = atoi(szArg);
	}
	else
	{
		return -1;
	}

	if (pstYangHaiBangInfo->NpcID <= 0)
	{
		return -1;
	}

	pstYangHaiBangInfo->EndTime -= iTime;
	player_yanghaibang_open_ui(pstEnv, pstPlayer);

	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_cleardayzhengyi(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	pstPlayer->stRoleData.MiscInfo.ZhengyiLimitInfo.DayCount = 0;
	player_zhengyi_limit_chg(pstEnv, pstPlayer);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}


int z_gm_rongyan_clear(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	ZoneTeam *pstTeam = NULL;
	Monster *pstMon = NULL;
	MONSTERDEF *pstMonDef;
	
	pstTeam = player_team_get(pstEnv, pstPlayer);
	if(pstTeam == NULL)
	{
		return -1;
	}
	
	//融焰怪也要清理
	if(pstTeam->iRongYanMonId > 0)
	{
		pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstTeam->iRongYanMonId);
		if(NULL == pstMon)
		{
			return 0;
		}
		
		pstMonDef = z_get_mon_def(pstEnv, pstMon);
		if (pstMonDef)
		{
			ZoneAni stAni;
	
			pstMon->cFirstAtkerType = 0;
			pstMon->ullFirstAtkerWid = 0;
			pstMon->iFirstAtkerID = 0; //没有战利品
			pstMon->szOwner[0] = 0;
			stAni.iType = OBJ_UNKNOW;

			//为了在调用mon_rongyan_boss_die(pstEnv,pstMon);时，进行一些数据清0
			pstTeam->iRongYanCurLevel = 0;
			
			z_mon_die(pstEnv->pstAppCtx, pstEnv, pstMon, pstMonDef, &stAni, 2, 0);
		}
		else
		{
			z_free_monster(pstEnv->pstAppCtx, pstEnv, pstMon);
		}
	}
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}


int z_gm_cleardaylifebrand(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	pstPlayer->stRoleData.MiscInfo.LifeBrandLimitInfo.DayCount = 0;
	player_life_brand_limit_chg(pstEnv, pstPlayer);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

//清空该玩家跑商的身上所记载的变量
int z_gm_clear_paoshang(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	PAOSHANGPLAYER *pstPaoShangPl = &pstPlayer->stRoleData.MiscInfo.PaoShang;
	if(NULL == pstPaoShangPl)
	{
		return -1;
	}

	int iJiaoPiao = pstPaoShangPl->JiaoPiao;
	
	//身上的跑商相关变量清空
	memset(pstPaoShangPl,0,sizeof(*pstPaoShangPl));

	pstPaoShangPl->JiaoPiao = iJiaoPiao;
	pstPaoShangPl->YinPiao = TASK_PAOSHANG_YINPIAO;
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_set_paoshang_yinpiao(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	char szArg[64];
	int iYinPiao = 0;
	PAOSHANGPLAYER *pstPaoShangPl = &pstPlayer->stRoleData.MiscInfo.PaoShang;
	if(NULL == pstPaoShangPl)
	{
		return -1;
	}

	if(0 == z_gm_get_val(&pszArg, szArg, sizeof(szArg)))
	{
		iYinPiao = atoi(szArg);
	}
	else
	{
		return -1;
	}

	pstPaoShangPl->YinPiao = iYinPiao;
	
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

int z_gm_clan_skill_reset_re_contri_flag_chg(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	char szArg[64];
	int iflag = 0;
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;
	if(NULL == pstMiscInfo)
	{
		return -1;
	}

	if(0 == z_gm_get_val(&pszArg, szArg, sizeof(szArg)))
	{
		iflag = atoi(szArg);
	}
	else
	{
		return -1;
	}

	if(1 == iflag)
	{
		pstMiscInfo->Flag |= PLAYER_CLAN_SKILL_RESET;
	}
	else if(0 == iflag)
	{
		pstMiscInfo->Flag &= ~PLAYER_CLAN_SKILL_RESET;
	}
	else
	{
		return -1;
	}
	
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_clan_skill_reset_re_money_flag_chg(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	char szArg[64];
	int iflag = 0;
	CLANPROF *pstClanProf = NULL;   
	CLANSKILLINFO * pstInfo = NULL;
	int i;
	int Num;
	
	if (NULL == pstPlayer)
	{
		return -1;
	}

	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);
	if(!pstZoneClan)
	{
		return -1;
	}

	pstClanProf = &pstZoneClan->stClan.ClanProf;

	if(0 == z_gm_get_val(&pszArg, szArg, sizeof(szArg)))
	{
		iflag = atoi(szArg);
	}
	else
	{
		return -1;
	}

	if(1 == iflag)
	{
		pstClanProf->Flag |= CLAN_FLAG_SKILL_RESET_RETURN_CLAN_MONEY;
	}
	else if(0 == iflag)
	{
		pstClanProf->Flag &= ~CLAN_FLAG_SKILL_RESET_RETURN_CLAN_MONEY;
		
		pstInfo = &pstZoneClan->stClan.ClanAdvance.ClanSkillInfo;
        Num = pstInfo->Num;
		if(Num<=0)
		{
			return -1;
		}
		for(i=Num-1;i>=0;i--)
		{
	//在返回公会资金时，成员人数技能等级变为了公会级别的10倍
			if(pstInfo->ClanSkill[i].SkillID == MEM_NUM_CLAN_SKILL_ID)
			{
				//测试时，不能超过有5级的公会技能
				memset(&pstInfo->ClanSkill[i],0,sizeof(pstInfo->ClanSkill[i]));
				pstInfo->Num--;
				if(i!=pstInfo->Num)
				{
					memmove(&pstInfo->ClanSkill[i],&pstInfo->ClanSkill[pstInfo->Num],sizeof(pstInfo->ClanSkill[i]));
					memset(&pstInfo->ClanSkill[pstInfo->Num],0,sizeof(pstInfo->ClanSkill[pstInfo->Num]));
				}
				break;
			}
		}

		clan_skill_reset_return_money(pstEnv,pstZoneClan);
	}
	else
	{
		return -1;
	}
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

//直接报名成功，并传送到准备地图中
int z_gm_dianfeng_baoming(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	if(pstEnv->pstConf->DianFengFlag == 0)
	{
		return -1;
	}
	
	DIANFENGGLOBAL *pstDianFengGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.DianFengGlobal;
	DIANFENGJOININFO *pstDianFengJoinInfo = NULL;	
	unsigned int uiCID = 0;
	CSDIANFENGTRANSMIT stCSDianFengTransmit;
	memset(&stCSDianFengTransmit,0,sizeof(stCSDianFengTransmit));

	if(0 > dianfeng_get_joininfo_by_career(pstEnv,&pstDianFengJoinInfo,pstDianFengGlobal,pstPlayer->stRoleData.Career))
		return -1;

	if(pstDianFengJoinInfo->MemNum >= MAX_DIANFENG_EVERY_GROUP_MEM_NUM)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_DIANFENG10);
		return -1;
	}

	//报名
	DIANFENGINFO *pstDianFengInfo = &pstPlayer->stRoleData.MiscInfo.DianFengInfo;

	CLANINFO *pstClanInfo = &pstPlayer->stRoleData.MiscInfo.ClanInfo;
	if(NULL != pstClanInfo)
	{
		uiCID = pstClanInfo->ClanId.Id;
	}

	pstDianFengInfo->JoinTime = pstDianFengGlobal->ActiveTimeS;
	pstDianFengInfo->DianFengFlag |= IF_DIANFENG_BAOMING;
	
	player_dianfeng_baoming_data_new(pstEnv, pstPlayer,pstDianFengJoinInfo,uiCID);
	
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_DIANFENG4);

	//直接传送进准备地图
	if(0 > player_dianfeng_transmit(pstEnv, pstPlayer,&stCSDianFengTransmit))
	{
		return -1;
	}
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

//正式开始pk
int z_gm_dianfeng_pk_begin(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	DIANFENGGLOBAL *pstDianFengGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.DianFengGlobal;
	
	pstDianFengGlobal->Status = ACTIVE_PK;
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
	UNUSED(pstPlayer);	//add by paraunused.pl
}

//清除报名队列，竞猜信息，匹配队列，并送回原来的地方
int z_gm_dianfeng_clean(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	DIANFENGGLOBAL *pstDianFengGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.DianFengGlobal;

	DIANFENGJOININFO *pstDianFengZFZJoinInfo = &pstDianFengGlobal->DianFengZFZJoinInfo;
	DIANFENGJOININFO *pstDianFengSHZJoinInfo = &pstDianFengGlobal->DianFengSHZJoinInfo;
	DIANFENGJOININFO *pstDianFengYLSJoinInfo = &pstDianFengGlobal->DianFengYLSJoinInfo;
	DIANFENGJOININFO *pstDianFengSYZJoinInfo = &pstDianFengGlobal->DianFengSYZJoinInfo;
	DIANFENGJOININFO *pstDianFengLSZJoinInfo = &pstDianFengGlobal->DianFengLSZJoinInfo;
	DIANFENGJOININFO *pstDianFengHMZJoinInfo = &pstDianFengGlobal->DianFengHMZJoinInfo;
	DIANFENGJOININFO *pstDianFengLHSJoinInfo = &pstDianFengGlobal->DianFengLHSJoinInfo;
	DIANFENGJOININFO *pstDianFengJNZJoinInfo = &pstDianFengGlobal->DianFengJNZJoinInfo;
	
	dianfeng_gm_clean(pstEnv,pstDianFengZFZJoinInfo);
	dianfeng_gm_clean(pstEnv,pstDianFengSHZJoinInfo);
	dianfeng_gm_clean(pstEnv,pstDianFengYLSJoinInfo);
	dianfeng_gm_clean(pstEnv,pstDianFengSYZJoinInfo);
	dianfeng_gm_clean(pstEnv,pstDianFengLSZJoinInfo);
	dianfeng_gm_clean(pstEnv,pstDianFengHMZJoinInfo);
	dianfeng_gm_clean(pstEnv,pstDianFengLHSJoinInfo);
	dianfeng_gm_clean(pstEnv,pstDianFengJNZJoinInfo);
	
	pstDianFengGlobal->Status = ACTIVE_CLEAN_DATA;
	pstDianFengGlobal->ActiveTimeS = 0;
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
	UNUSED(pstPlayer);	//add by paraunused.pl
}

//参数为1   清除冠军信息，0清除雕像信息
int z_gm_dianfeng_clean_chamption_and_effigy(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	char szArg[64];
	int iflag = 0;
	
	if(0 == z_gm_get_val(&pszArg, szArg, sizeof(szArg)))
	{
		iflag = atoi(szArg);
	}
	else
	{
		return -1;
	}

	//清除冠军信息
	if(1 == iflag)
	{
		dianfeng_clear(pstEnv,1);
	}
	//清除雕像信息
	else if(0 == iflag)
	{
		dianfeng_clear(pstEnv,0);
		player_first_or_effigy_info_to_clt(pstEnv, pstPlayer,FIRST_EFFIGY);
	}
	else
	{
		return -1;
	}
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstPlayer);	//add by paraunused.pl
}

int z_gm_dianfeng_clean_myself(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	char szArg[64];
	int iflag = 0;
	
	if(0 == z_gm_get_val(&pszArg, szArg, sizeof(szArg)))
	{
		iflag = atoi(szArg);
	}
	else
	{
		return -1;
	}

	DIANFENGINFO *pstDianFengInfo = NULL;
	pstDianFengInfo = &pstPlayer->stRoleData.MiscInfo.DianFengInfo;
	
	DIANFENGGLOBAL *pstDianFengGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.DianFengGlobal;

	if(0 == iflag)
	{
		pstDianFengInfo->DianFengFlag &= ~IF_DIANFENG_BAOMING;
	}
	else if(1 == iflag)
	{
		pstDianFengInfo->DianFengFlag &= ~IF_DIANFENG_GUESS;
	}
	else if(2 == iflag)
	{
		pstDianFengInfo->DianFengFlag &= ~IF_DIANFENG_GET_AWARD;
	}
	else if(3 == iflag)
	{
		pstDianFengInfo->DianFengFlag &= ~IF_DIANFENG_TRANSMIT_SUC;
	}
	else if(4 == iflag)
	{
		pstDianFengInfo->DianFengFlag &= ~IF_DIANFENG_ONE_PK;
	}
	else if(5 == iflag)
	{
		pstDianFengInfo->JoinTime = 0;
	}
	else if(6 == iflag)
	{
		pstDianFengGlobal->DFFlag |= SHOW_DIANFENG_TIS;
	}
	else if(7 == iflag)
	{
		pstDianFengGlobal->DFFlag |= SHOW_DIANFENG_OPEN_TWENTY;
		pstDianFengGlobal->Status = ACTIVE_READY;
	}
	else
	{
		return -1;
	}
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstPlayer);	//add by paraunused.pl
}

int z_gm_dianfeng_to_chamption(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	if(pstEnv->pstConf->DianFengFlag == 0)
	{
		return -1;
	}

	if(pstPlayer == NULL)
	{
		return -1;
	}	
	DFFIRSTINFO *pstDFChampionInfo = NULL;
	DIANFENGGLOBAL *pstDianFengGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.DianFengGlobal;

	switch(pstPlayer->stRoleData.Career)
	{
		case CAREER_WARRIOR_SINGLEHAND:
			pstDFChampionInfo = &pstDianFengGlobal->DFSHZFirstInfo.DFChampionInfo;
			break;
		case CAREER_WARRIOR_DOUBLEHAND:	
			pstDFChampionInfo = &pstDianFengGlobal->DFZFZFirstInfo.DFChampionInfo;
			break;
		case CAREER_MAGICOR_SINGLEHAND:
			pstDFChampionInfo = &pstDianFengGlobal->DFJNZFirstInfo.DFChampionInfo;
			break;
		case CAREER_MAGICOR_DOUBLEHAND:	
			pstDFChampionInfo = &pstDianFengGlobal->DFLHSFirstInfo.DFChampionInfo;
			break;
		case CAREER_GUNER_SINGLEHAND:
			pstDFChampionInfo = &pstDianFengGlobal->DFLSZFirstInfo.DFChampionInfo;
			break;
		case CAREER_GUNER_DOUBLEHAND:
			pstDFChampionInfo = &pstDianFengGlobal->DFHMZFirstInfo.DFChampionInfo;
			break;
		case CAREER_CHURCH_SINGLEHAND:
			pstDFChampionInfo = &pstDianFengGlobal->DFSYZFirstInfo.DFChampionInfo;
			break;
		case CAREER_CHURCH_DOUBLEHAND:	
			pstDFChampionInfo = &pstDianFengGlobal->DFYLSFirstInfo.DFChampionInfo;
			break;	
		default:
			return -1;
			break;
	}

	if(NULL == pstDFChampionInfo)
	{
		return -1;
	}
		
	unsigned int uiCID = 0;
	CLANINFO *pstClanInfo = &pstPlayer->stRoleData.MiscInfo.ClanInfo;
	
	if(NULL != pstClanInfo)
	{
		uiCID = pstClanInfo->ClanId.Id;
	}

	//在DianFengGlobal  记住
	pstDFChampionInfo->ClanId = uiCID;
	pstDFChampionInfo->RoleID = pstPlayer->stRoleData.RoleID;
	pstDFChampionInfo->RoleLevel = pstPlayer->stRoleData.Level;
	pstDFChampionInfo->RoleGender = pstPlayer->stRoleData.Gender;
	pstDFChampionInfo->RoleCareer = pstPlayer->stRoleData.Career;
	pstDFChampionInfo->RoleHair = pstPlayer->stRoleData.Hair;
	pstDFChampionInfo->RoleHead = pstPlayer->stRoleData.Head;
	pstDFChampionInfo->RoleFace = pstPlayer->stRoleData.Face;
	STRNCPY(pstDFChampionInfo->RoleName, pstPlayer->stRoleData.RoleName,
		      CCH(pstDFChampionInfo->RoleName));
	memcpy (&pstDFChampionInfo->RoleAvatar, &pstPlayer->stRoleData.AvatarProf, sizeof(pstDFChampionInfo->RoleAvatar));
	pstDFChampionInfo->MapID = 0;
	pstDFChampionInfo->Pos.X = 0;
	pstDFChampionInfo->Pos.Y = 0;

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR ,ZONEERR_DIANFENG15,pstPlayer->stRoleData.RoleName);
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pszArg);	//add by paraunused.pl
}

//结婚系统
int z_gm_wedding(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szOpt[128];
	Player *pstOther = NULL;
	WEDDINGINFO *pstWeddingInfo1 = NULL;
	WEDDINGINFO *pstWeddingInfo2 = NULL;
	int ret;
	int ret2;
	int ret3;

	if(z_gm_get_val(&pszArg, szOpt, sizeof(szOpt)) < 0)
	{
		return -1;
	}

	if (strcasecmp(szOpt, "spouse") == 0)
	{
		char szOpt1[128];
		char *pszBuff = "对方不是你好友";
		char *pszBuff2 = "你不在对方的好友队列中";
		char szText[512];
		char szText1[512];

		//wedding spouse name   表示与谁结婚
		if(z_gm_get_val(&pszArg, szOpt1, sizeof(szOpt1)) < 0)
		{
			return -1;
		}

		pstOther = z_name_player(pstAppCtx,pstEnv,szOpt1);
		if(NULL == pstOther)
		{
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING8);
			return -1;
		}

		ret = online_info_find_friend_byID(&pstPlayer->stRoleData.Buddy, pstOther->stRoleData.RoleID);
		if(ret < 0)
		{
			z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, pszBuff);
			return -1;
		}

		ret2 = online_info_find_friend_byID(&pstOther->stRoleData.Buddy, pstPlayer->stRoleData.RoleID);
		if(ret2 < 0)
		{
			z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, pszBuff2);
			return -1;
		}

		ret3 = find_spouse_from_friend_list(&pstPlayer->stRoleData.Buddy);
		if(ret3 >= 0)
		{
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING48);
			return -1;
		}
	
		pstWeddingInfo1 = &pstPlayer->stRoleData.MiscInfo.WeddingInfo;
		pstWeddingInfo2 = &pstOther->stRoleData.MiscInfo.WeddingInfo;
		pstWeddingInfo1->WeddingMeal = 3;
		pstWeddingInfo2->WeddingMeal = 3;

		pstPlayer->stRoleData.Buddy.Friends[ret].IntimateColorType = INTIMATE_COLOR_SPOUSE;
		pstOther->stRoleData.Buddy.Friends[ret2].IntimateColorType = INTIMATE_COLOR_SPOUSE;

		relation_involve_marry_svr(pstEnv,pstPlayer,&pstOther->stRoleData.RoleID,TYPE_INTIMATE_COLOR_SVR);
		relation_involve_marry_svr(pstEnv,pstOther,&pstPlayer->stRoleData.RoleID,TYPE_INTIMATE_COLOR_SVR);
	
		pstPlayer->stRoleData.Buddy.SpouseNum = 1;
		pstOther->stRoleData.Buddy.SpouseNum = 1;
		
		memcpy(&pstPlayer->stRoleData.Buddy.Spouse[0],&pstPlayer->stRoleData.Buddy.Friends[ret],
		sizeof(pstPlayer->stRoleData.Buddy.Spouse[0]));
		memcpy(&pstOther->stRoleData.Buddy.Spouse[0],&pstOther->stRoleData.Buddy.Friends[ret2],
		sizeof(pstOther->stRoleData.Buddy.Spouse[0]));

		z_sys_strf_broadcast(pstEnv, SYS_SHOW, ZONEERR_WEDDING41,pstPlayer->stRoleData.RoleName,pstOther->stRoleData.RoleName);

		//可以领取永恒钻戒了
		pstWeddingInfo1->WeddingFlag |= IF_CAN_HAVE_RING;
		pstWeddingInfo2->WeddingFlag |= IF_CAN_HAVE_RING;

		z_add_skill(pstAppCtx, pstEnv, pstPlayer, NULL, LIVE_SKILL_ID, 1, 1);
		z_add_skill(pstAppCtx, pstEnv, pstPlayer, NULL, STRIVE_SKILL_ID, 1, 1);
		z_add_skill(pstAppCtx, pstEnv, pstOther, NULL, LIVE_SKILL_ID, 1, 1);
		z_add_skill(pstAppCtx, pstEnv, pstOther, NULL, STRIVE_SKILL_ID, 1, 1);

		//双方记得播特效
		snprintf(szText, sizeof(szText),"playEffct(%d,%d,\"effect\\\\entity\\\\10254_5_2.ent\")",pstPlayer->stRoleData.Pos.X,pstPlayer->stRoleData.Pos.Y);
		play_effect(pstEnv, pstPlayer, 1, pstPlayer->iMemID, szText, NULL);
		snprintf(szText1, sizeof(szText1),"playEffct(%d,%d,\"effect\\\\entity\\\\10254_5_2.ent\")",pstOther->stRoleData.Pos.X,pstOther->stRoleData.Pos.Y);
		play_effect(pstEnv, pstOther, 1, pstOther->iMemID, szText1, NULL);

		//全服玫瑰花特效
		play_effect(pstEnv, pstPlayer, 1, 0, "playUiEffect(0,0,0,\"effect\\\\entity\\\\pm_hmg_bxh.ent\")", NULL);

		wedding_dsn_action(pstEnv, pstPlayer);
		wedding_dsn_action(pstEnv, pstOther);
	}
	else if (strcasecmp(szOpt, "DelWeddingGlobal") == 0)
	{
		//wedding DelWeddingGlobal    表示去掉服务器记录的庆典和蜜游信息
		WEDDINGGLOBAL *pstWeddingGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.WeddingGlobal;
		pstWeddingGlobal->TravelNum = 0;
		pstWeddingGlobal->CelebrationNum = 0;
	}
	else if (strcasecmp(szOpt, "DelFlag") == 0)
	{
		//wedding DelFlag    表示去掉自己和伴侣身上已经举行婚礼庆典和婚礼蜜游的标识
		ret3 = find_spouse_from_friend_list(&pstPlayer->stRoleData.Buddy);
		if(ret3 < 0)
		{
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING51);
			return -1;
		}

		pstOther = z_name_player(pstAppCtx,pstEnv,pstPlayer->stRoleData.Buddy.Friends[ret3].RoleName);
		if(NULL == pstOther)
		{
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING8);
			return -1;
		}
		
		pstWeddingInfo1 = &pstPlayer->stRoleData.MiscInfo.WeddingInfo;
		pstWeddingInfo2 = &pstOther->stRoleData.MiscInfo.WeddingInfo;

		pstWeddingInfo1->WeddingFlag &= ~IF_START_CELEBRATION;
		pstWeddingInfo1->WeddingFlag &= ~IF_START_HONEYMOON_TRAVEL;
		
		pstWeddingInfo2->WeddingFlag &= ~IF_START_CELEBRATION;
		pstWeddingInfo2->WeddingFlag &= ~IF_START_HONEYMOON_TRAVEL;
	}
	else if (strcasecmp(szOpt, "RemoveRelation") == 0)
	{
		//wedding RemoveRelation    表示解除情侣或者伴侣关系
		char *pszBuff = "正在婚礼蜜游中，待会再解除";
		char *pszBuff2 = "正在婚礼庆典中，待会再解除";

		tdr_ulonglong ullTempRoleID,ullTempRoleID2;
		ROLEBUDDYMEMBER *pstBuddyMem = NULL;
		
		WEDDINGGLOBAL *pstWeddingGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.WeddingGlobal;
		if(pstWeddingGlobal->TravelNum > 0)
		{
			//双方当前并非举行 婚礼蜜游
			ullTempRoleID = pstWeddingGlobal->TravelInfos[pstWeddingGlobal->TravelNum-1].FirstTRoleID;
			ullTempRoleID2 = pstWeddingGlobal->TravelInfos[pstWeddingGlobal->TravelNum-1].SecondTRoleID;
			if( ullTempRoleID == pstPlayer->stRoleData.RoleID || ullTempRoleID2 == pstPlayer->stRoleData.RoleID)
			{
				z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, pszBuff);
				return -1;
			}
		}
		
		if(pstWeddingGlobal->CelebrationNum > 0)
		{
			//双方当前并非举行 婚礼庆典中
			ullTempRoleID = pstWeddingGlobal->CelebrationInfos[pstWeddingGlobal->CelebrationNum-1].FirstCRoleID;
			ullTempRoleID2 = pstWeddingGlobal->CelebrationInfos[pstWeddingGlobal->CelebrationNum-1].SecondCRoleID;
			if( ullTempRoleID == pstPlayer->stRoleData.RoleID || ullTempRoleID2 == pstPlayer->stRoleData.RoleID)
			{
				z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, pszBuff2);
				return -1;
			}
		}
	
		if (pstPlayer->stRoleData.Buddy.SpouseNum <= 0 ||
		pstPlayer->stRoleData.Buddy.SpouseNum > MAX_BUDDY_SPOUSE)
		{
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING33);
			return -1;
		}

		pstOther = z_name_player(pstAppCtx, pstEnv, pstPlayer->stRoleData.Buddy.Spouse[0].RoleName);
		if (!pstOther)
		{
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING8);
			return -1;
		}

		ret = online_info_find_friend_byID(&pstPlayer->stRoleData.Buddy, pstOther->stRoleData.RoleID);
		if(ret < 0)
		{
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING33);
			return -1;
		}

		pstBuddyMem = &pstPlayer->stRoleData.Buddy.Friends[ret];

		//清除身上记录的信息
		pstWeddingInfo1 = &pstPlayer->stRoleData.MiscInfo.WeddingInfo;
		memset(pstWeddingInfo1,0,sizeof(*pstWeddingInfo1));
		
		//头顶信息删除身份关系相关的显示
		pstPlayer->stRoleData.Buddy.SpouseNum = 0;
		
		//好友列表中，删除相应对象的关系标志
		pstBuddyMem->IntimateColorType = INTIMATE_COLOR_FRIEND;
		relation_involve_marry_svr(pstEnv,pstPlayer,&pstBuddyMem->RoleID,TYPE_INTIMATE_COLOR_SVR);

		z_del_skill(pstAppCtx, pstEnv, pstPlayer, LIVE_SKILL_ID, 1);
		z_del_skill(pstAppCtx, pstEnv, pstPlayer, STRIVE_SKILL_ID, 1);

		wedding_dsn_action(pstEnv, pstPlayer);
	
		ret2 = online_info_find_friend_byID(&pstOther->stRoleData.Buddy, pstPlayer->stRoleData.RoleID);
		if(ret2 < 0)
		{
			return -1;
		}
		
		//好友列表中，删除相应对象的关系标志
		pstOther->stRoleData.Buddy.Friends[ret2].IntimateColorType = INTIMATE_COLOR_FRIEND;
		relation_involve_marry_svr(pstEnv,pstOther,&pstPlayer->stRoleData.RoleID,TYPE_INTIMATE_COLOR_SVR);
		
		//对方头顶信息删除身份关系相关的显示
		pstOther->stRoleData.Buddy.SpouseNum = 0;
		
		//清除对方身上记录的信息
		pstWeddingInfo2 = &pstOther->stRoleData.MiscInfo.WeddingInfo;
		memset(pstWeddingInfo2,0,sizeof(*pstWeddingInfo2));
		
		z_del_skill(pstAppCtx, pstEnv, pstOther, LIVE_SKILL_ID, 1);
		z_del_skill(pstAppCtx, pstEnv, pstOther, STRIVE_SKILL_ID, 1);

		wedding_dsn_action(pstEnv, pstOther);
	}
	else if (strcasecmp(szOpt, "buddyval") == 0)
	{
		int iBuddyVal = 0;
		char szOpt1[128];
		char szOpt2[32];
		char *pszBuff = "对方不是你好友";
		char *pszBuff2 = "你不是对方的好友";
		ROLEBUDDYMEMBER *pstBuddyMember = NULL;
		ROLEBUDDYMEMBER *pstBuddyMember2 = NULL;
		int iTemp1 = 0;
		int iTemp2 = 0;
		unsigned short usOldBuddyVal = 0;
		unsigned short usOldBuddyVal2 = 0;
		char szText[512];
		char szText1[512];

		//wedding buddyval name 100          加自己与name  之间的亲密度
		if(z_gm_get_val(&pszArg, szOpt1, sizeof(szOpt1)) < 0 ||
			z_gm_get_val(&pszArg, szOpt2, sizeof(szOpt2)) < 0)
		{
			return -1;
		}

		pstOther = z_name_player(pstAppCtx,pstEnv,szOpt1);
		if(NULL == pstOther)
		{
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING8);
			return -1;
		}

		ret = online_info_find_friend_byID(&pstPlayer->stRoleData.Buddy, pstOther->stRoleData.RoleID);
		if(ret < 0)
		{
			z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, pszBuff);
			return -1;
		}

		ret2 = online_info_find_friend_byID(&pstOther->stRoleData.Buddy, pstPlayer->stRoleData.RoleID);
		if(ret2 < 0)
		{
			z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, pszBuff2);
			return -1;
		}
		
		iBuddyVal = atoi(szOpt2);

		pstBuddyMember = &pstOther->stRoleData.Buddy.Friends[ret2];
		pstBuddyMember2 = &pstPlayer->stRoleData.Buddy.Friends[ret];

		usOldBuddyVal = pstBuddyMember->BuddyVal;
		usOldBuddyVal2 = pstBuddyMember2->BuddyVal;

		//改变双方亲密度值
		iTemp1 = relation_buddyval_change(pstEnv, pstPlayer, pstBuddyMember2,iBuddyVal);
		iTemp2 = relation_buddyval_change(pstEnv, pstOther, pstBuddyMember,iBuddyVal);	
				
		//双方播特效
		snprintf(szText, sizeof(szText),"playEffct(%d,%d,\"effect\\\\entity\\\\10543.ent\")",pstPlayer->stRoleData.Pos.X,pstPlayer->stRoleData.Pos.Y);
		play_effect(pstEnv, pstPlayer, 1, pstPlayer->iMemID, szText, NULL);
		snprintf(szText1, sizeof(szText1),"playEffct(%d,%d,\"effect\\\\entity\\\\10543.ent\")",pstOther->stRoleData.Pos.X,pstOther->stRoleData.Pos.Y);
		play_effect(pstEnv, pstOther, 1, pstOther->iMemID, szText1, NULL);

		if(iBuddyVal >= 0)
		{
			if(iTemp1 == 0)
			{
				z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
					ZONEERR_WEDDING58,pstOther->stRoleData.RoleName,pstBuddyMember2->BuddyVal-usOldBuddyVal2);
			}

			if(iTemp2 == 0)
			{
				z_send_sys_str_msg(pstAppCtx, pstEnv, pstOther, SYS_MIDDLE2, 
					ZONEERR_WEDDING58,pstPlayer->stRoleData.RoleName,pstBuddyMember->BuddyVal-usOldBuddyVal);
			}
		}
		else
		{
			if(iTemp1 == 0)
			{
				z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,
					ZONEERR_WEDDING59,pstOther->stRoleData.RoleName,usOldBuddyVal2-pstBuddyMember2->BuddyVal);
			}

			if(iTemp2 == 0)
			{
				z_send_sys_str_msg(pstAppCtx, pstEnv, pstOther, SYS_MIDDLE2, 
					ZONEERR_WEDDING59,pstPlayer->stRoleData.RoleName,usOldBuddyVal-pstBuddyMember->BuddyVal);
			}
		}
	}
	else if (strcasecmp(szOpt, "get") == 0)
	{
		char szOpt1[32];
		int iSelect = 0;

		//wedding get 1        表示需要再领取永恒戒指
		//wedding get 2        表示需要再领取浪漫烟花
		if(z_gm_get_val(&pszArg, szOpt1, sizeof(szOpt1)) < 0 )
		{
			return -1;
		}
		
		ret3 = find_spouse_from_friend_list(&pstPlayer->stRoleData.Buddy);
		if(ret3 < 0)
		{
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING51);
			return -1;
		}

		pstOther = z_name_player(pstAppCtx,pstEnv,pstPlayer->stRoleData.Buddy.Friends[ret3].RoleName);
		if(NULL == pstOther)
		{
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING8);
			return -1;
		}

		iSelect = atoi(szOpt1);
		
		pstWeddingInfo1 = &pstPlayer->stRoleData.MiscInfo.WeddingInfo;
		pstWeddingInfo2 = &pstOther->stRoleData.MiscInfo.WeddingInfo;

		if(1 == iSelect)
		{
			pstWeddingInfo1->WeddingFlag |= IF_CAN_HAVE_RING;
			pstWeddingInfo1->WeddingFlag &= ~IF_GET_RING;

			pstWeddingInfo2->WeddingFlag |= IF_CAN_HAVE_RING;
			pstWeddingInfo2->WeddingFlag &= ~IF_GET_RING;
		}
		else if(2 == iSelect)
		{
			pstWeddingInfo1->WeddingFlag |= IF_CAN_HAVE_FIREWORKS;
			pstWeddingInfo1->WeddingFlag &= ~IF_GET_FIREWORKS;

			pstWeddingInfo2->WeddingFlag |= IF_CAN_HAVE_FIREWORKS;
			pstWeddingInfo2->WeddingFlag &= ~IF_GET_FIREWORKS;
		}
		else
		{
			return -1;
		}
	}
	else if (strcasecmp(szOpt, "love") == 0)
	{
		char szOpt1[128];
		char *pszBuff = "对方不是你好友";
		char *pszBuff2 = "你不在对方的好友队列中";
		char *pszBuff3 = "对方已经有情侣或者伴侣了";
		char *pszBuff4 = "您已经有情侣或者伴侣了";
		char szText[512];
		char szText1[512];
		
		//wedding love name   表示与谁成为情侣
		if(z_gm_get_val(&pszArg, szOpt1, sizeof(szOpt1)) < 0)
		{
			return -1;
		}

		pstOther = z_name_player(pstAppCtx,pstEnv,szOpt1);
		if(NULL == pstOther)
		{
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING8);
			return -1;
		}

		ret = online_info_find_friend_byID(&pstPlayer->stRoleData.Buddy, pstOther->stRoleData.RoleID);
		if(ret < 0)
		{
			z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, pszBuff);
			return -1;
		}

		ret2 = online_info_find_friend_byID(&pstOther->stRoleData.Buddy, pstPlayer->stRoleData.RoleID);
		if(ret2 < 0)
		{
			z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, pszBuff2);
			return -1;
		}

		ret3 = find_spouse_from_friend_list(&pstPlayer->stRoleData.Buddy);
		if(ret3 >= 0)
		{
			z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, pszBuff4);
			return -1;
		}

		ret3 = find_spouse_from_friend_list(&pstOther->stRoleData.Buddy);
		if(ret3 >= 0)
		{
			z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, pszBuff3);
			return -1;
		}
		
		ret3 = find_lover_from_friend_list(&pstPlayer->stRoleData.Buddy);
		if(ret3 >= 0)
		{
			z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, pszBuff4);
			return -1;
		}
		
		ret3 = find_lover_from_friend_list(&pstOther->stRoleData.Buddy);
		if(ret3 >= 0)
		{
			z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, pszBuff3);
			return -1;
		}

		pstPlayer->stRoleData.Buddy.Friends[ret].IntimateColorType = INTIMATE_COLOR_LOVERS;
		pstOther->stRoleData.Buddy.Friends[ret2].IntimateColorType = INTIMATE_COLOR_LOVERS;
		
		relation_involve_marry_svr(pstEnv,pstPlayer,&pstOther->stRoleData.RoleID,TYPE_INTIMATE_COLOR_SVR);
		relation_involve_marry_svr(pstEnv,pstOther,&pstPlayer->stRoleData.RoleID,TYPE_INTIMATE_COLOR_SVR);
		
		//为了进入视野时，快速索引
		pstPlayer->stRoleData.Buddy.SpouseNum = 1;
		pstOther->stRoleData.Buddy.SpouseNum = 1;
		
		memcpy(&pstPlayer->stRoleData.Buddy.Spouse[0],&pstPlayer->stRoleData.Buddy.Friends[ret],
		sizeof(pstPlayer->stRoleData.Buddy.Spouse[0]));
		
		memcpy(&pstOther->stRoleData.Buddy.Spouse[0],&pstOther->stRoleData.Buddy.Friends[ret2],
		sizeof(pstOther->stRoleData.Buddy.Spouse[0]));

		//双方播特效
		snprintf(szText, sizeof(szText),"playEffct(%d,%d,\"effect\\\\entity\\\\10542.ent\")",pstPlayer->stRoleData.Pos.X,pstPlayer->stRoleData.Pos.Y);
		play_effect(pstEnv, pstPlayer, 1, pstPlayer->iMemID, szText, NULL);
		snprintf(szText1, sizeof(szText1),"playEffct(%d,%d,\"effect\\\\entity\\\\10542.ent\")",pstOther->stRoleData.Pos.X,pstOther->stRoleData.Pos.Y);
		play_effect(pstEnv, pstOther, 1, pstOther->iMemID, szText1, NULL);

		//全服广播
		z_sys_strf_broadcast(pstEnv, SYS_SHOW, ZONEERR_WEDDING47,pstOther->stRoleData.RoleName,pstPlayer->stRoleData.RoleName);

		wedding_dsn_action(pstEnv, pstPlayer);
		wedding_dsn_action(pstEnv, pstOther);
	}
	else if (strcasecmp(szOpt, "printbuddyval") == 0)
	{
		char szOpt1[32];
		char *pszBuff = "对方不是你好友";
		ROLEBUDDYMEMBER *pstBuddyMember = NULL;
		char szText[512];

		//wedding printbuddyval name         输出自己与name  之间的亲密度
		if(z_gm_get_val(&pszArg, szOpt1, sizeof(szOpt1)) < 0)
		{
			return -1;
		}

		pstOther = z_name_player(pstAppCtx,pstEnv,szOpt1);
		if(NULL == pstOther)
		{
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING8);
			return -1;
		}

		ret = online_info_find_friend_byID(&pstPlayer->stRoleData.Buddy, pstOther->stRoleData.RoleID);
		if(ret < 0)
		{
			z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, pszBuff);
			return -1;
		}
		
		pstBuddyMember = &pstPlayer->stRoleData.Buddy.Friends[ret];
		snprintf(szText, sizeof(szText),"您与%s之间的亲密度为%d",szOpt1,pstBuddyMember->BuddyVal);
		z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_SVR, szText);
	}
	
	else if (strcasecmp(szOpt, "relation") == 0)
	{
		char szText[512];

		//wedding relation  	
		//优先级 :   
		//1:有伴侣先 输出伴侣名字
		//2:有情侣输出情侣名字
		//3:没伴侣没情侣输出单身
		
		if (pstPlayer->stRoleData.Buddy.SpouseNum <= 0 ||
		pstPlayer->stRoleData.Buddy.SpouseNum > MAX_BUDDY_SPOUSE)
		{
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING51);
			return 0;
		}
		
		ret3 = find_spouse_from_friend_list(&pstPlayer->stRoleData.Buddy);
		if(ret3 >= 0)
		{
			snprintf(szText, sizeof(szText),"您的伴侣是%s",pstPlayer->stRoleData.Buddy.Friends[ret3].RoleName);
			z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, szText);
			return 0;
		}

		ret3 = find_lover_from_friend_list(&pstPlayer->stRoleData.Buddy);
		if(ret3 >= 0)
		{
			snprintf(szText, sizeof(szText),"您的情侣是%s",pstPlayer->stRoleData.Buddy.Friends[ret3].RoleName);
			z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, szText);
			return 0;
		}
	}
	else
	{
		return -1;
	}
		
	return 0;
}

int z_gm_set_mzt_layer(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	char szIdx[32];
	int iIdx;
	
	if(z_gm_get_val(&pszArg, szIdx, sizeof(szIdx)) < 0)
	{
		return -1;
	}

	iIdx = atoi(szIdx);

	if (iIdx > MZT_MAX_LAYER || iIdx <= 0)
	{
		iIdx = 0;
	}
	
	pstPlayer->stRoleData.MiscInfo.MztInfo.Layer = iIdx;
	
	return 0;
}

int z_gm_set_level_remain_tm(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	char szIdx[32];
	int iIdx;
	
	if(z_gm_get_val(&pszArg, szIdx, sizeof(szIdx)) < 0)
	{
		return -1;
	}

	iIdx = atoi(szIdx);

	//单位为天数	
	pstPlayer->stRoleData.MiscInfo.LevelRemainInfo.LevelRemainDay = iIdx;
	
	return 0;
}

int z_gm_get_level_remain_tm(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "%d", pstPlayer->stRoleData.MiscInfo.LevelRemainInfo.LevelRemainDay);
	return 0;
}

int z_gm_clear_zj_rongyan(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg)
{
	if(is_fini_global(pstEnv) == 0)
	{
		return -1;
	}

	char szIdx[32];
	int iIdx;
	
	if(z_gm_get_val(&pszArg, szIdx, sizeof(szIdx)) < 0)
	{
		return -1;
	}

	iIdx = atoi(szIdx);
	ZJRONGYANGLOBAL *pstZJRongYanGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.ZJRongYanGlobal;

	if(iIdx == 0)
	{
		pstZJRongYanGlobal->Num = 0;
	}
	else if(iIdx == 1)
	{
		memset(&pstZJRongYanGlobal->ZJRongYan,0,sizeof(pstZJRongYanGlobal->ZJRongYan));
	}
	else
	{
		return -1;
	}
		
	return 0;
}

int z_gm_name_addattr(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szName[ROLE_NAME_LEN];
	Player *pstPeer;
	UNUSED(pstPlayer);

	/* name is not find. */
	if( z_gm_get_val(&pszArg, szName, sizeof(szName))<0 )
		return -1;

	pstPeer	=	z_name_player(pstAppCtx, pstEnv, szName);
	if( !pstPeer )
		return -1;

	z_gm_addattr(pstAppCtx, pstEnv, pstPeer, pszArg);

	return 0;
}

// get_role_var iIdx iBit
int z_gm_get_role_var(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	int iIdx;
	int iBit;
	char cFlag;
	int iRet;
	char szIdx[32];
	char szIdx2[32];
	char szText[512];
	
	if(z_gm_get_val(&pszArg, szIdx, sizeof(szIdx)) < 0)
	{
		return -1;
	}
	iIdx = atoi(szIdx);
	
	if(z_gm_get_val(&pszArg, szIdx2, sizeof(szIdx2)) < 0)
	{
		return -1;
	}
	iBit = atoi(szIdx2);

	if ((iBit>=0 && iBit<=7) || 8==iBit || 16==iBit || 32==iBit)
	{
		
	}
	else
	{
		return -1;
	}

	if( iIdx<0 || iIdx>=MAX_ROLE_SCRIPT_VAR || (16==iBit && iIdx>=MAX_ROLE_SCRIPT_VAR-1) || (32==iBit && iIdx>=MAX_ROLE_SCRIPT_VAR-3))
	{
		return -1;
	}

	if( 8 == iBit )
	{
		iRet =	pstPlayer->stRoleData.MiscInfo.GlobalVar[iIdx];
	}
	else if (16 == iBit)
	{
		iRet		= *(short*)(pstPlayer->stRoleData.MiscInfo.GlobalVar+iIdx);
	}
	else if (32 == iBit)
	{
		iRet		= *(int*)(pstPlayer->stRoleData.MiscInfo.GlobalVar+iIdx);
	}
	else
	{
		cFlag	=	1 << iBit;

		if( pstPlayer->stRoleData.MiscInfo.GlobalVar[iIdx] & cFlag )
			iRet	=	1;
		else
			iRet	=	0;
	}

	snprintf(szText, sizeof(szText),"值为%d",iRet);
	z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_SVR, szText);
	
	return 0;
}

// set_role_var iIdx iBit iVal
int z_gm_set_role_var(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	int iIdx;
	int iBit;
	int iVal;
	char cFlag;
	static char sTemp[4];

	char szIdx[32];
	char szIdx2[32];
	char szIdx3[100];
	
	if(z_gm_get_val(&pszArg, szIdx, sizeof(szIdx)) < 0)
	{
		return -1;
	}
	iIdx = atoi(szIdx);
	
	if(z_gm_get_val(&pszArg, szIdx2, sizeof(szIdx2)) < 0)
	{
		return -1;
	}
	iBit = atoi(szIdx2);

	if(z_gm_get_val(&pszArg, szIdx3, sizeof(szIdx3)) < 0)
	{
		return -1;
	}
	iVal = atoi(szIdx3);

	if ((iBit>=0 && iBit<=7) || 8==iBit || 16==iBit || 32==iBit)
	{
		
	}
	else
	{
		return -1;
	}

	if( iIdx<0 || iIdx>=MAX_ROLE_SCRIPT_VAR || (16==iBit && iIdx>=MAX_ROLE_SCRIPT_VAR-1) || (32==iBit && iIdx>=MAX_ROLE_SCRIPT_VAR-3))
	{
		return -1;
	}

	memcpy(sTemp, &iVal, sizeof(int));

	if( 8 == iBit )
	{
		pstPlayer->stRoleData.MiscInfo.GlobalVar[iIdx]	=	(char) iVal;
	}
	else if (16 == iBit)
	{
		pstPlayer->stRoleData.MiscInfo.GlobalVar[iIdx]		=	sTemp[0];
		pstPlayer->stRoleData.MiscInfo.GlobalVar[iIdx + 1]	=	sTemp[1];
	}
	else if (32 == iBit)
	{
		pstPlayer->stRoleData.MiscInfo.GlobalVar[iIdx]		=	sTemp[0];
		pstPlayer->stRoleData.MiscInfo.GlobalVar[iIdx + 1]	=	sTemp[1];
		pstPlayer->stRoleData.MiscInfo.GlobalVar[iIdx + 2]	=	sTemp[2];
		pstPlayer->stRoleData.MiscInfo.GlobalVar[iIdx + 3]	=	sTemp[3];
	}
	else
	{
		cFlag	=	1 << iBit;

		if( iVal )
			pstPlayer->stRoleData.MiscInfo.GlobalVar[iIdx]		|=	cFlag;
		else
			pstPlayer->stRoleData.MiscInfo.GlobalVar[iIdx]		&=	~cFlag;
	}

	return 0;
}

//需要先报名
int z_gm_shikong_enter(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szPworldID[32];
	int iPworldID;	

	if( z_gm_get_val(&pszArg, szPworldID, sizeof(szPworldID))<0)
	{
		return -1;
	}

	iPworldID = atoi(szPworldID);

	player_enter_shikong_pworld(pstEnv,pstPlayer,iPworldID);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_get_shikong_fb_num(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{
	char szText[512];
	int iUsePworldNum = shikong_get_wait_queue_use_num(pstEnv);

	snprintf(szText, sizeof(szText),"绿野凶林副本开启的个数为%d",iUsePworldNum);
	z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_SVR, szText);
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_clear_career_alter_time(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{	
	CAREERALTERINFO *pstCareerAlterInfo = &pstPlayer->stRoleData.MiscInfo.CareerAlterInfo;
	pstCareerAlterInfo->time = 0;
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_set_home_soil_lv(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg)
{	
	ZoneHome *pstZoneHome;
	HOME *pstHome;
	HOMESIOL *pstSiol;
	HOMEFARMDEF *pstDef;
	char szLv[32];
	int iLv;	

	if( z_gm_get_val(&pszArg, szLv, sizeof(szLv))<0)
	{
		return -1;
	}

	pstZoneHome = zonehome_one_find_by_id( pstEnv, pstPlayer->stRoleData.MiscInfo.Home.ID);
	if (!pstZoneHome)
	{
		return -1;
	}

	iLv = atoi(szLv);

	pstDef = home_farm_def_find( pstEnv, iLv);
	if (!pstDef)
		return -1;

	pstHome = &pstZoneHome->stHome;
	pstSiol = &pstHome->Info.Siol;
	pstSiol->Lvl = iLv;

	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

