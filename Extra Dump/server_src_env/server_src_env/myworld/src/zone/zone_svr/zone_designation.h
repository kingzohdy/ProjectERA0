#ifndef ZONE_DESIGNATION_H
#define ZONE_DESIGNATION_H


#define ADD_DESIG_NUM_LIMIT -1
#define ADD_DESIG_EXIST -2
#define ADD_DESIG_NO_DEF -3

int g_iDesigVar;

#define GET_DESIG_CHG_CAREER 1 			// 转职获取称号
#define GET_DESIG_MON_KILL 2 				//被怪物杀死N次
#define GET_DESIG_KILL_DUDUNIAO 3 			// 击杀5000只嘟嘟鸟
#define GET_DESIF_ENTER_JAIN_NUM 4 		//进入监狱100次
#define GET_DESIG_USE_FOOD 5 				// 消耗食物5000个
#define GET_DESIG_FRIEND_NUM 6 			// 好友数量达到60
#define GET_DESIG_CUIDI_PAOYOU 7 			// 和崔蒂打炮100次

#define GET_DESIG_EXP_99999	9			// exp最后5个数字是99999
#define GET_DESIG_RIDE_STREN 10			// 强化坐骑达到50级
#define GET_DESIG_WATCH 11					// 查看/被查看100次


// 查询称号定义
const DESIGNATIONDEF* z_designation_def(ZONESVRENV* pstEnv, unsigned int iID);

// 发送角色所有称号列表
int player_designation_info(ZONESVRENV* pstEnv, Player *pstPlayer);

// 称号相关视野广播消息
int player_designation_action(ZONESVRENV* pstEnv, Player *pstPlayer);

// 增加角色的称号
int player_designation_add(ZONESVRENV* pstEnv, Player *pstPlayer,
                           unsigned int iID);

// 设置角色的称号
int player_designation_set(ZONESVRENV* pstEnv, Player *pstPlayer,
                           unsigned int iID);

// 查找角色已有的称号
DESIGNATIONENTRY* player_find_designation(ZONESVRENV* pstEnv, Player *pstPlayer,
                                          unsigned int iID);

// 删除一个角色已有的称号
int player_designation_del(ZONESVRENV* pstEnv, Player *pstPlayer,
                           unsigned int iID);

// 处理客户端发送过来的关于角色称号的操作。
int player_designation(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
                       TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int player_designation_add_in(ZONESVRENV* pstEnv, Player *pstPlayer,
                           unsigned int iID);


int player_des_del_by_time(ZONESVRENV* pstEnv, Player *pstPlayer);
int player_desig_get(ZONESVRENV* pstEnv, Player *pstPlayer, int iType);


#endif //  ZONE_DESIGNATION_H
