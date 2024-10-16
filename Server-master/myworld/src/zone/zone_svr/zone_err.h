#ifndef ZONE_ERR_H
#define ZONE_ERR_H

#include "comm/terr.h"
#include "comm/terrimpl.h"

extern const char* err_get(HERR hErr);

#define ZONE_GET_ERR(idx, lang)		err_get(idx)

#define ZONE_MOD_IDX		0	/* @@mod 0 mmog*/

#define ZONEERR_GM_BADPARAM	1
/* @@error 1 错误的参数:%s */

#define ZONEERR_GM_SUCC			2
/* @@error 2 GM指令 [%s %s]执行成功 */

#define ZONEERR_EXP_DEC			3
/* @@error 3 你被扣除了%d点经验值 */

#define ZONEERR_EXP_ADD			4
/* @@error 4 你获得了%d点经验值 */

#define ZONEERR_ACTION_NOT		5
/* @@error 5 当前状态下不允许进行%s操作 */

#define ZONEERR_SCRIPT_DEBUG	6
/* @@error 6 debug script:%s*/

#define ZONEERR_OP_LIMIT	7
/* @@error 7 操作受限，%d秒只能操作一次 */

#define ZONEERR_DIRTY 8
/* @@error 8 对不起，你输入的信息含有受限词汇*/

#define ZONEERR_NOSPACE 9
/* @@error 9 背包无法容纳更多物品，请整理背包*/

#define ZONEERR_ARM_VALIDEND 10
/* @@error 10 物品%s过了有效期，已被回收*/

#define ZONEERR_AMM 11
/* @@error 11 弹药了不够了*/

#define ZONEERR_TEAM_OP 12
/* @@error 12 组队处理信息已失效*/

#define ZONEERR_CHAT_COMM 13
/* @@error 13 你的发言频率过快*/

#define ZONEERR_CHAT_WORLD 14
/* @@error 14 发言频率超过设定，世界频道%d秒发言一次*/

#define ZONEERR_CHAT_LEVEL 15
/* @@error 15 此频道发言要求最小等级为%d*/

#define ZONEERR_ARM_WASH 16
/* @@error 16 背包中的装备才能洗炼*/

#define ZONEERR_ARM_WASH1 17
/* @@error 17 请先放入需要洗炼的装备*/

#define ZONEERR_ARM_WASH2 18
/* @@error 18 需要天赋值达到150点!*/

#define ZONEERR_ARM_WASH3 19
/* @@error 19 属性全部锁定,无法洗练!*/

#define ZONEERR_ARM_WASH4 20
/* @@error 20 金币不够!*/

#define ZONEERR_ARM_STAR 21
/* @@error 21 背包中的装备才能强化*/

#define ZONEERR_ARM_STAR1 22
/* @@error 22 请先放入需要强化的装备*/

#define ZONEERR_ARM_STAR2 23
/* @@error 23 时装不能强化*/

#define ZONEERR_ARM_STAR3 24
/* @@error 24 装备强化已到上限*/

#define ZONEERR_GEM_UP 25
/* @@error 25 合成宝石失败!*/

#define ZONEERR_GEM_UP1 26
/* @@error 26 升级成功,你获得1个%s */

#define ZONEERR_GEM_INLAY 27
/* @@error 27 不可镶嵌*/

#define ZONEERR_GEM_INLAY1 28
/* @@error 28 装备等级不符，不能镶嵌*/

#define ZONEERR_GEM_INLAY2 29
/* @@error 29 无此宝石*/

#define ZONEERR_GEM_INLAY3 30
/* @@error 30 %s 镶嵌%s 成功*/

#define ZONEERR_GEM_INLAY4 31
/* @@error 31 镶嵌失败*/

#define ZONEERR_GEM_PICK 32
/* @@error 32 缺少宝石摘取符*/

#define ZONEERR_ARM_MOVE 33
/* @@error 33 转移品质的装备只能从背包中获取*/

#define ZONEERR_ARM_MOVE1 34
/* @@error 34 请先放入需要转移品质的装备*/

#define ZONEERR_ARM_MOVE2 35
/* @@error 35 请先放入转移品质的目标装备*/

#define ZONEERR_ARM_MOVE3 36
/* @@error 36 转移装备品质失败*/

#define ZONEERR_ARM_MOVE4 37
/* @@error 37 时装不能进行转星操作*/

#define ZONEERR_ARM_MOVE5 38
/* @@error 38 装备类型不符合,品质转移失败*/

#define ZONEERR_ARM_MOVE6 39
/* @@error 39 装备星级限制,品质转移失败*/

#define ZONEERR_BIND1 40
/* @@error 40 只能绑定背包中的装备*/

#define ZONEERR_BIND2 41
/* @@error 41 请先放入需要绑定的装备*/

#define ZONEERR_BIND3 42
/* @@error 42 请先放入需要绑定的物品*/

#define ZONEERR_BIND4 43
/* @@error 43 该装备死亡必定掉落，不能绑定*/

#define ZONEERR_BIND5 44
/* @@error 44 该物品已经是绑定状态了*/

#define ZONEERR_BIND6 45
/* @@error 45 绑定物品成功*/

#define ZONEERR_UNBIND1 46
/* @@error 46 只能解绑背包中的装备*/

#define ZONEERR_UNBIND2 47
/* @@error 47 请先放入需要解绑的装备*/

#define ZONEERR_UNBIND3 48
/* @@error 48 解绑道具不存在*/

#define ZONEERR_UNBIND4 49
/* @@error 49 解绑失败*/

#define ZONEERR_UNBIND5 50
/* @@error 50 解绑道具不能对该装备使用 */

#define ZONEERR_UNBIND6 51
/* @@error 51 解绑定成功! */

#define ZONEERR_FENJIE1 52
/* @@error 52 背包空间不足,请整理背包! */

#define ZONEERR_NOMONEY 53
/* @@error 53 没有足够的银币!*/

#define ZONEERR_FENJIE2 54
/* @@error 54 分解失败，您所选的装备不能分解 */

#define ZONEERR_SAVE_SUCC 55
/* @@error 55 保存成功! */

#define ZONEERR_SAVE_FAIL 56
/* @@error 56 保存失败! */

#define ZONEERR_GEM_RESET1 57
/* @@error 57 宝石重置失败! */

#define ZONEERR_GEM_POWER 58
/* @@error 58 宝石充能失败!*/

#define ZONEERR_SPAR_SORB 59
/* @@error 59 吸收成功!*/

#define ZONEERR_SPAR_SORB1 60
/* @@error 60 吸收失败!*/

#define ZONEERR_EXP1 61
/* @@error 61 你获得了 %llu经验值(防沉迷中,收益减少50 ％)!*/

#define ZONEERR_EXP2 62
/* @@error 62 你获得了 %llu经验值(%s防沉迷中,收益减少50 ％)!*/

#define ZONEERR_EXP3 63
/* @@error 63 你获得了 %llu经验值(%s%s%s)*/

#define ZONEERR_ATTR1 64
/* @@error 64 你获得了%d%s*/

#define ZONEERR_ATTR2 65
/* @@error 65 你消耗了%d%s*/

#define ZONEERR_BATTLE_SVR1 66
/* @@error 66 跨服状态无法进行副本传送操作*/

#define ZONEERR_BUSY 67
/* @@error 67 服务器忙,请稍等片刻*/

#define ZONEERR_BATTLE_SVR2 68
/* @@error 68 报名成功!*/

#define ZONEERR_CLAN1 69
/* @@error 69 你还没有加入一个公会*/

#define ZONEERR_CLAN2 70
/* @@error 70 你所在的公会已经解散了*/

#define ZONEERR_CLAN3 71
/* @@error 71 你已经被请离出公会了*/

#define ZONEERR_CLAN4 72
/* @@error 72 有人申请加入你所在的公会,请尽快处理*/

#define ZONEERR_CLAN5 73
/* @@error 73 有人申请与你的公会结盟,请尽快处理*/

#define ZONEERR_CLAN6 74
/* @@error 74 有人申请与你的公会敌对,请尽快处理*/

#define ZONEERR_CLAN7 75
/* @@error 75 不能与自己结盟或敌对*/

#define ZONEERR_CLAN8 76
/* @@error 76 你已经申请与对方公会同盟*/

#define ZONEERR_CLAN9 77
/* @@error 77 你已经与对方公会结盟*/

#define ZONEERR_CLAN10 78
/* @@error 78 你与对方公会处于敌对状态*/

#define ZONEERR_CLAN11 79
/* @@error 79 你与对方公会处于敌对状态*/

#define ZONEERR_CLAN26 80
/* @@error 80 你没有权限进行此项公会操作*/

#define ZONEERR_CLAN12 81
/* @@error 81 你已经被GM开除出公会了*/

#define ZONEERR_CLAN13 82
/* @@error 82 当前公会等级下,公会已满,无法加入*/

#define ZONEERR_CLAN14 83
/* @@error 83 公会名不能为空*/

#define ZONEERR_CLAN15 84
/* @@error 84 公会名含有敏感词或非法字符(只能数字英文或中文),请重新输入*/

#define ZONEERR_CLAN16 85
/* @@error 85 这个名字已经有公会使用了*/

#define ZONEERR_CLAN17 86
/* @@error 86 你邀请的玩家不存在或不在线*/

#define ZONEERR_CLAN18 87
/* @@error 87 该玩家刚刚退出了公会，无法再被邀请，请等待%s后再试*/

#define ZONEERR_CLAN19 88
/* @@error 88 当前公会等级下,公会已满,无法再添加成员*/

#define ZONEERR_CLAN20 89
/* @@error 89 你邀请的玩家已经加入了公会*/

#define ZONEERR_CLAN21 90
/* @@error 90 你邀请的玩家正在接受其他邀请，请稍后再试*/

#define ZONEERR_CLAN22 91
/* @@error 91 你不能把自己请出公会*/

#define ZONEERR_CLAN23 92
/* @@error 92 你没有权限将此人请出公会*/

#define ZONEERR_CLAN24 93
/* @@error 93 你已经被请出公会了*/

#define ZONEERR_CLAN25 94
/* @@error 94 会长不允许退出公会*/

#define ZONEERR_CLAN27 96
/* @@error 96 对方不在线,不能接受你转让的公会职位*/

#define ZONEERR_CLAN28 97
/* @@error 97 对方已不在公会中,不能接受你转让的公会职位*/

#define ZONEERR_CLAN29 98
/* @@error 98 不能把公会职位转让给比自己更高的人*/

#define ZONEERR_CLAN30 99
/* @@error 99 当前公会等级下,公会人数已满,无法加入*/

#define ZONEERR_CLAN31 100
/* @@error 100 你已经在申请列表中了*/

#define ZONEERR_CLAN32 101
/* @@error 101 %s 拒绝了你加入公会的请求*/

#define ZONEERR_BATTLE_SVR_OP 102
/* @@error 102 跨服状态无法进行此操作*/

#define ZONEERR_SVR_ERR 103
/* @@error 103 由于服务器内部故障导致公会系统当前不可用*/

#define ZONEERR_CLAN33 104
/* @@error 104 指定的产品不存在, 无法制造*/

#define ZONEERR_CLAN34 105
/* @@error 105  同时制造到达上限, 请等待制造完成再试*/

#define ZONEERR_CLAN35 106
/* @@error 106  资源中心栏位不足, 无法制造*/

#define ZONEERR_CLAN36 107
/* @@error 107  公会资金不够, 无法制造*/

#define ZONEERR_CLAN37 108
/* @@error 108  科技不存在, 无法学习*/

#define ZONEERR_CLAN38 109
/* @@error 109  科技中心等级不够,无法学习*/

#define ZONEERR_CLAN39 110
/* @@error 110  科技: %s 已经在学习中, 不能重复学习*/

#define ZONEERR_CLAN40 111
/* @@error 111  公会科技点数不够, 无法学习*/

#define ZONEERR_CLAN41 112
/* @@error 112  前置科技还未学习*/

#define ZONEERR_CLAN42 113
/* @@error 113  公会资金不足, 无法学习*/

#define ZONEERR_CLAN43 114
/* @@error 114  科技: %s Lv.%d还未学习*/

#define ZONEERR_CLAN44 115
/* @@error 115  科技: %s已经学习过了*/

#define ZONEERR_CLAN45 116
/* @@error 116  最多只能同时学习%d个科技*/

#define ZONEERR_CLAN46 117
/* @@error 117  这项科技并没有在研发中*/

#define ZONEERR_CLAN47 118
/* @@error 118  没有研发在进行中,无法取消研发*/

#define ZONEERR_CLAN48 119
/* @@error 119  科技不存在,无法取消*/

#define ZONEERR_CLAN49 120
/* @@error 120  这项科技并没有在研发中*/

#define ZONEERR_CLAN50 121
/* @@error 121  没有研发在进行中,无法加速研发*/

#define ZONEERR_CLAN51 122
/* @@error 122  科技不存在,无法加速研发*/

#define ZONEERR_CLAN52 123
/* @@error 123  没有物品在制造中,无法取消制造*/

#define ZONEERR_CLAN53 124
/* @@error 124  参数错误,无法取消制造*/

#define ZONEERR_CLAN54 125
/* @@error 125  产品不存在,无法取消制造*/

#define ZONEERR_CLAN55 126
/* @@error 126  没有物品在制造中,无法加速制造*/

#define ZONEERR_CLAN56 127
/* @@error 127  参数错误,无法加速制造*/

#define ZONEERR_CLAN57 128
/* @@error 128  产品不存在,无法加速制造*/

#define ZONEERR_CLAN58 129
/* @@error 129  制造即将完成, 无需加速*/

#define ZONEERR_CLAN59 130
/* @@error 130  公会资金不足以完成加速, 还差%d*/

#define ZONEERR_CLAN60 131
/* @@error 131  兑换数量超出资源中心中的数量*/

#define ZONEERR_CLAN61 132
/* @@error 132  公会贡献度不足!*/

#define ZONEERR_CLAN62 133
/* @@error 133  消耗了%d点公会贡献度*/

#define ZONEERR_CLAN63 134
/* @@error 134  参数错误, 无法继续操作*/

#define ZONEERR_CLAN64 135
/* @@error 135  你没有权限操作这个公会物品*/

#define ZONEERR_CLAN65 136
/* @@error 136  公会资源中心空间不足，无法提交资源%s*/

#define ZONEERR_CLAN66 137
/* @@error 137  制造此物品需要的公会资源不足，无法制造*/

#define ZONEERR_CLAN67 138
/* @@error 138  当前的科技水平不足以制造%s*/

#define ZONEERR_CLAN68 139
/* @@error 139  服务器错误导致购买失败*/

#define ZONEERR_CLAN69 140
/* @@error 140  公会资金不足以购买此物品*/

#define ZONEERR_CLAN70 141
/* @@error 141  公会资源中心栏位不足*/

#define ZONEERR_CLAN71 142
/* @@error 142  你不是公会成员, 不能进行此操作*/

#define ZONEERR_CLAN72 143
/* @@error 143 找不到机械, 不能进行此操作*/

#define ZONEERR_CLAN73 144
/* @@error 144 不是本公会机械, 不能进行此操作*/

#define ZONEERR_CLAN74 145
/* @@error 145 机械回收失败*/

#define ZONEERR_CLAN75 146
/* @@error 146 不是公会成员, 不能提交*/

#define ZONEERR_CLAN76 147
/* @@error 147 此物品无法提交到公会资源中心*/

#define ZONEERR_CLAN77 148
/* @@error 148 公会资源[%s]不足%d, 不可提交!*/

#define ZONEERR_CLAN78 149
/* @@error 149 背包中公会资源[%s]数量比提交数量%d少, 不可提交!*/

#define ZONEERR_CLAN79 150
/* @@error 150 公会资源中心已满, 不能再提交了*/

#define ZONEERR_CLAN80 151
/* @@error 151 错误的背包位置*/

#define ZONEERR_CLAN81 152
/* @@error 152 此物品无法提交到公会资源中心*/

#define ZONEERR_CLAN82 153
/* @@error 153 你向公会资源中心提交了[%s]×%d*/

#define ZONEERR_CLAN83 154
/* @@error 154 公会城市已创建*/

#define ZONEERR_CLAN84 155
/* @@error 155 你的公会现在还没有城市*/

#define ZONEERR_CLAN85 156
/* @@error 156 击杀入侵者积累个人积分兑换奖励，个人积分50 时兑换的奖励翻倍！！*/

#define ZONEERR_CLAN86 157
/* @@error 157 错误的传送地点*/

#define ZONEERR_CLAN87 158
/* @@error 158 思过厅中黑名玩家不能进行此操作*/

#define ZONEERR_CLAN88 159
/* @@error 159 无法进行此操作,公会已经解散!*/

#define ZONEERR_CLAN89 160
/* @@error 160 公会城市不存在*/

#define ZONEERR_CLAN90 161
/* @@error 161 获得%d点积分，你当前已获得%d／50积分*/

#define ZONEERR_COLLECT1 162
/* @@error 162 缺少采集需要的工具*/

#define ZONEERR_COLLECT2 163
/* @@error 163 采集装备耐久不够了*/

#define ZONEERR_COLLECT3 164
/* @@error 164 采集失败!*/

#define ZONEERR_COLLECT4 165
/* @@error 165 本次采集需要装备%s*/

#define ZONEERR_COLLECT5 166
/* @@error 166 %s的耐久度不够了*/

#define ZONEERR_COLLECT6 167
/* @@error 167 本次采集需要掌握技能%s*/

#define ZONEERR_COLLECT7 168
/* @@error 168 本次采集需要相关技能达到%d级*/

#define ZONEERR_COLLECT8 169
/* @@error 169 本次采集需要相关物品%s的数量不够*/

#define ZONEERR_COLLECT9 170
/* @@error 170 本次采集需要相关物品%s*/

#define ZONEERR_COLLECT10 171
/* @@error 171 防沉迷状态下不可采集*/

#define ZONEERR_COLLECT11 172
/* @@error 172 当前状态不能采集*/

#define ZONEERR_COLLECT12 173
/* @@error 173 采集装备耐久不够了*/

#define ZONEERR_COLLECT13 174
/* @@error 174 采集需要相关物品不够了*/

#define ZONEERR_COLLECT14 175
/* @@error 175 您采集车目前已经载满，请提交资源后再进行采集*/

#define ZONEERR_COLLECT15 176
/* @@error 176 该矿产需要建造了采集场后才能采集*/

#define ZONEERR_COLLECT16 177
/* @@error 177 非据点占领公会成员无权占领*/

#define ZONEERR_COLLECT17 178
/* @@error 178 己方公会建筑，目标无效*/

#define ZONEERR_COLLECT18 179
/* @@error 179 采集目标正在进行中*/

#define ZONEERR_COLLECT19 180
/* @@error 180 另一个采集目标正在进行中*/

#define ZONEERR_SKILL1 181
/* @@error 181 此地图不可使用技能*/

#define ZONEERR_SKILL2 182
/* @@error 182 此地图不可使用无双技能*/

#define ZONEERR_SKILL3 183
/* @@error 183 非战斗状态下才能使用此技能*/

#define ZONEERR_SKILL4 184
/* @@error 184 战斗状态下才能使用此技能*/

#define ZONEERR_SKILL5 185
/* @@error 185 切磋不能使用此技能*/

#define ZONEERR_SKILL6 186
/* @@error 186 定身状态不可使用此技能*/

#define ZONEERR_SKILL7 187
/* @@error 187 当前状态不能使用技能*/

#define ZONEERR_SKILL8 188
/* @@error 188 使用临时机械时无法传送*/

#define ZONEERR_SKILL9 189
/* @@error 189 摆摊状态下无法传送*/

#define ZONEERR_SKILL10 190
/* @@error 190 死亡状态下无法传送*/

#define ZONEERR_SKILL11 191
/* @@error 191 交易状态下无法传送*/

#define ZONEERR_SKILL12 192
/* @@error 192 思过厅中黑名玩家无法传送*/

#define ZONEERR_SKILL13 193
/* @@error 193 当前状态无法传送*/

#define ZONEERR_SKILL14 194
/* @@error 194 骑乘机械状态下无法传送*/

#define ZONEERR_SKILL15 195
/* @@error 195 多人骑乘状态无法传送*/

#define ZONEERR_SKILL16 196
/* @@error 196 搭乘状态无法传送*/

#define ZONEERR_SKILL17 197
/* @@error 197 宠物%s 死亡了*/

#define ZONEERR_SKILL18 198
/* @@error 198 您已乘坐在他人的坐骑上*/

#define ZONEERR_SKILL19 199
/* @@error 199 当前状态无法骑乘*/

#define ZONEERR_SKILL20 200
/* @@error 200 暂时没有该骑乘*/

#define ZONEERR_SKILL21 201
/* @@error 201 此地图不可骑乘*/

#define ZONEERR_SKILL22 202
/* @@error 202 骑乘失败,此坐骑只有时间城城主才有权限骑乘*/

#define ZONEERR_SKILL23 203
/* @@error 203 骑乘失败,此坐骑只有时间城占领公会成员才有权限骑乘*/

#define ZONEERR_SKILL24 204
/* @@error 204 该坐骑有效期已到期，无法使用*/

#define ZONEERR_SKILL25 205
/* @@error 205 变身状态下无法骑乘*/

#define ZONEERR_SKILL26 206
/* @@error 206 骑乘%s 成功*/

#define ZONEERR_SKILL27 207
/* @@error 207 骑乘状态无法进行此操作*/

#define ZONEERR_SKILL28 208
/* @@error 208 当前血量不够了*/

#define ZONEERR_SKILL29 209
/* @@error 209 【%s】向你抛出美味小龙虾，你禁不住诱惑，离开了沙滩椅*/

#define ZONEERR_ROLENAME_DUP 210
/* @@error 210 角色名已被使用*/

#define ZONEERR_PWORLD 211
/* @@error 211 活动时间已过，不能进入到副本中*/

#define ZONEERR_PWORLD1 212
/* @@error 212 无法创建副本!*/

#define ZONEERR_PWORLD2 213
/* @@error 213 进入%s 队伍人数不能少于%d人!*/

#define ZONEERR_PWORLD3 214
/* @@error 214 副本创建失败，无法进入，只有队长才可创建副本*/

#define ZONEERR_EXG1 215
/* @@error 215 当前爱心等级下的宠物不能交易*/

#define ZONEERR_EXG2 216
/* @@error 216 宠物出征状态下不能交易*/

#define ZONEERR_EXG3 217
/* @@error 217 背包空间已满,交易失败*/

#define ZONEERR_EXG4 218
/* @@error 218 对方背包已满,交易失败*/

#define ZONEERR_EXG5 219
/* @@error 219 宠物栏已满,交易失败*/

#define ZONEERR_EXG6 220
/* @@error 220 对方宠物栏已满,交易失败*/

#define ZONEERR_EXG7 221
/* @@error 221 距离太远,无法交易*/

#define ZONEERR_EXG8 222
/* @@error 222 对方已经下线*/

#define ZONEERR_EXG9 223
/* @@error 223 对方处于异常登录状态,%d分钟内不能进行此操作*/

#define ZONEERR_EXG10 224
/* @@error 224 对方没有验证安全锁密码*/

#define ZONEERR_EXG11 225
/* @@error 225 同服的玩家才能交易*/

#define ZONEERR_EXG12 226
/* @@error 226 对方正在退出游戏,不能交易*/

#define ZONEERR_EXG13 227
/* @@error 227 安全时间内无法交易,安全时间还剩%ld秒*/

#define ZONEERR_EXG14 228
/* @@error 228 对方在安全时间内无法交易,安全时间还剩%ld秒*/

#define ZONEERR_EXG15 229
/* @@error 229 对方在摆摊状态下无法交易*/

#define ZONEERR_EXG16 230
/* @@error 230 对方在死亡状态下无法交易*/

#define ZONEERR_EXG17 231
/* @@error 231 对方已在交易状态,无法交易*/

#define ZONEERR_EXG18 232
/* @@error 232 您仍处于15秒交易冷却时间，请稍后再试*/

#define ZONEERR_EXG19 233
/* @@error 233 %s拒绝交易*/

#define ZONEERR_EXG20 234
/* @@error 234 交易申请由于玩家%s 下线取消*/

#define ZONEERR_EXG21 235
/* @@error 235 距离太远,交易取消*/

#define ZONEERR_EXG22 236
/* @@error 236 玩家%s :对你的请求无应答,稍后再试*/

#define ZONEERR_EXG23 237
/* @@error 237 对方不在线,无法交易*/

#define ZONEERR_EXG24 238
/* @@error 238 已经处于交易状态*/

#define ZONEERR_EXG25 239
/* @@error 239 交易取消*/

#define ZONEERR_EXG26 240
/* @@error 240 必须先锁定交易*/

#define ZONEERR_EXG27 241
/* @@error 241 交易成功*/

#define ZONEERR_EXG28 242
/* @@error 242 对方下线,交易取消*/

#define ZONEERR_EXG29 243
/* @@error 243 交易%s不足*/

#define ZONEERR_EXG30 244
/* @@error 244 对方交易%s不足*/

#define ZONEERR_EXG31 245
/* @@error 245 对方%s达到上限*/

#define ZONEERR_EXG32 246
/* @@error 246 %s达到上限*/

#define ZONEERR_FAIRY1 247
/* @@error 247 精灵栏位达到上限*/

#define ZONEERR_FAIRY2 248
/* @@error 248 战斗状态中不能装备精灵*/

#define ZONEERR_FAIRY3 249
/* @@error 249 同样的精灵只能拥有一个*/

#define ZONEERR_FAIRY4 250
/* @@error 250 精灵栏位达到上限*/

#define ZONEERR_FAIRY5 251
/* @@error 251 成功获得(%s)精灵*/

#define ZONEERR_FAIRY6 252
/* @@error 252 此副本不允许召唤精灵*/

#define ZONEERR_FAIRY7 253
/* @@error 253 此地图不允许召唤精灵*/

#define ZONEERR_FAIRY8 254
/* @@error 254 (%s)精灵已经是出征状态*/

#define ZONEERR_FAIRY9 255
/* @@error 255 当前状态无法召唤精灵*/

#define ZONEERR_FAIRY10 256
/* @@error 256 您当前处于摆摊状态，无法强化*/

#define ZONEERR_FAIRY11 257
/* @@error 257 您当前处于交易状态，无法强化*/

#define ZONEERR_FAIRY12 258
/* @@error 258 该精灵已是最高品质，无法强化*/

#define ZONEERR_FAIRY13 259
/* @@error 259 当前精灵等级不足，无法强化*/

#define ZONEERR_FAIRY14 260
/* @@error 260 当前精灵不能生产元素卷轴!*/

#define ZONEERR_FAIRY15 261
/* @@error 261 刻印需要%d 魂能值*/

#define ZONEERR_FAIRY16 262
/* @@error 262 缺少对应的道具!*/

#define ZONEERR_FAIRY17 263
/* @@error 263 (%s)精灵召唤成功*/

#define ZONEERR_FAIRY18 264
/* @@error 264 升级失败,(%s)精灵已经满级*/

#define ZONEERR_FAIRY19 265
/* @@error 265 升级失败,角色等级低于%d级*/

#define ZONEERR_FAIRY20 266
/* @@error 266 (%s)精灵正在升级中*/

#define ZONEERR_FAIRY21 267
/* @@error 267 升级(%s)精灵需要%d %s*/

#define ZONEERR_FAIRY22 268
/* @@error 268 升级(%s)精灵需要%d经验*/

#define ZONEERR_FAIRY23 269
/* @@error 269 没有炼化中的精灵*/

#define ZONEERR_FAIRY24 270
/* @@error 270 背包中找不到加速齿轮,无法进行炼化加速*/

#define ZONEERR_FAIRY25 271
/* @@error 271 成功进行炼化加速,炼化时间减少%d分钟*/

#define ZONEERR_FAIRY26 272
/* @@error 272 (%s)精灵没有出征,不能召回*/

#define ZONEERR_FAIRY27 273
/* @@error 273 (%s)收回成功*/

#define ZONEERR_FAIRY28 274
/* @@error 274 %s精灵升级成功,等级提升至%d级*/

#define ZONEERR_FAIRY29 275
/* @@error 275 (%s)精灵正在炼化中,不能注入经验*/

#define ZONEERR_FAIRY30 276
/* @@error 276 注入的炼化经验过多*/

#define ZONEERR_FAIRY31 277
/* @@error 277 经验值不够*/

#define ZONEERR_FAIRY32 278
/* @@error 278 材料不足，无法聚灵*/

#define ZONEERR_FAIRY33 279
/* @@error 279 银币数量不足，无法聚灵*/

#define ZONEERR_FAIRY34 280
/* @@error 280 %s精灵强化成功,星级提升至%d星*/

#define ZONEERR_FAIRY35 281
/* @@error 281 %s精灵强化失败,星级降低至%d星*/

#define ZONEERR_FAIRY36 282
/* @@error 282 %s精灵强化失败*/

#define ZONEERR_GENUIS1 283
/* @@error 283 死亡状态下不能升级天赋!*/

#define ZONEERR_GENUIS2 284
/* @@error 284 角色等级提升到60，开启了天赋系统!*/

#define ZONEERR_GENUIS3 285
/* @@error 285 天赋:%s 已经是最高级了!*/

#define ZONEERR_GENUIS4 286
/* @@error 286 角色等级不足，无法升级!*/

#define ZONEERR_GENUIS5 287
/* @@error 287 经验不足，无法升级!*/

#define ZONEERR_GENUIS6 288
/* @@error 288 %s不足，无法升级!*/

#define ZONEERR_GENUIS7 289
/* @@error 289 需要天赋值达到%d才可升级!*/

#define ZONEERR_GENUIS8 290
/* @@error 290 %s 已升级到%s阶%d级!*/

#define ZONEERR_GM1 291
/* @@error 291 找不到要删除的物品*/

#define ZONEERR_GM2 292
/* @@error 292 你被禁言%d 秒*/

#define ZONEERR_GM3 293
/* @@error 293 装载脚本失败*/

#define ZONEERR_GM4 294
/* @@error 294 装载脚本成功*/

#define ZONEERR_GM5 295
/* @@error 295 宠物[%s]学会了%s*/

#define ZONEERR_GM6 296
/* @@error 296 此副本要求组队*/

#define ZONEERR_GM7 297
/* @@error 297 公会:%s 已被提交删除.*/

#define ZONEERR_GM8 298
/* @@error 298 公会城市重建*/

#define ZONEERR_GM9 299
/* @@error 299 clan:%s, lv:%d, money:%d, stable:%d, active:%d, tech:%d, used-tech:%d, memnum:%d*/

#define ZONEERR_GM10 300
/* @@error 300 conf : %s | %d to %d*/

#define ZONEERR_GM11 301
/* @@error 301 conf : %s = %d*/

#define ZONEERR_GM12 302
/* @@error 302 ID    剩余时间  滚动间隔:公告内容*/

#define ZONEERR_GM13 303
/* @@error 303 %08x  %d  %d:%s */

#define ZONEERR_GM14 304
/* @@error 304 总计 %d 条公告*/

#define ZONEERR_GM15 305
/* @@error 305 成功添加了一条公告 ID = %08x */

#define ZONEERR_GM16 306
/* @@error 306 ID: 称号 */

#define ZONEERR_GM17 307
/* @@error 307 总计%u个称号 */

#define ZONEERR_GM18 308
/* @@error 308 当前服务器时间:%s，服务器开服时间:%s，当前汇率%u */

#define ZONEERR_GM19 309
/* @@error 309 您没有这个权限 */

#define ZONEERR_GM20 310
/* @@error 310 对方不在线 */

#define ZONEERR_MAIL1 311
/* @@error 311 你有一封新邮件，邮箱已满，请及时清理 */

#define ZONEERR_MAIL2 312
/* @@error 312 邮件发送成功*/

#define ZONEERR_MAIL3 313
/* @@error 313 收取物品失败,参数错误*/

#define ZONEERR_MAIL4 314
/* @@error 314 收取物品失败,找不到邮件中的物品*/

#define ZONEERR_MAIL5 315
/* @@error 315 收取物品失败,背包已满*/

#define ZONEERR_MAIL6 316
/* @@error 316 收取%s失败,%s已达上限*/

#define ZONEERR_MAIL7 317
/* @@error 317 背包空间不足，请整理背包！*/

#define ZONEERR_MAIL8 318
/* @@error 318 %s 发送给你的邮件将在下一次登陆时被清除，请注意查收*/

#define ZONEERR_RELATION1 319
/* @@error 319 上天感应到你和%s的缘分,已把你们加为临时好友*/

#define ZONEERR_RELATION2 320
/* @@error 320 不能添加自己为好友*/

#define ZONEERR_RELATION3 321
/* @@error 321 目标在你的黑名单,无法增加好友,请按F键打开黑名单查看*/

#define ZONEERR_RELATION4 322
/* @@error 322 目标在你的宿敌名单,无法增加好友,请按F键打开宿敌名单查看*/

#define ZONEERR_RELATION5 323
/* @@error 323 对方已经在你好友名单中*/

#define ZONEERR_RELATION6 324
/* @@error 324 好友人数达到上限*/

#define ZONEERR_RELATION7 325
/* @@error 325 你已经向对方申请加为好友*/

#define ZONEERR_RELATION8 326
/* @@error 326 %s不在线或属于不同服务器,不能加为好友*/

#define ZONEERR_RELATION9 327
/* @@error 327 %s加你为好友*/

#define ZONEERR_RELATION10 328
/* @@error 328 你拒绝加%s 为好友*/

#define ZONEERR_RELATION11 329
/* @@error 329 %s 好友申请超时，请再次尝试*/

#define ZONEERR_RELATION12 330
/* @@error 330 %s 拒绝加你为好友*/

#define ZONEERR_RELATION13 331
/* @@error 331 %s目前不在线或属于不同服务器,不能加为黑名单*/

#define ZONEERR_RELATION14 332
/* @@error 332 不能把自己添加进黑名单*/

#define ZONEERR_RELATION15 333
/* @@error 333 对方已经在黑名单中*/

#define ZONEERR_RELATION16 334
/* @@error 334 黑名单达到上限,请删除后在添加*/

#define ZONEERR_RELATION17 335
/* @@error 335 当前关系不能把%s添加进黑名单*/

#define ZONEERR_RELATION18 336
/* @@error 336 玩家%s把你添加进黑名单*/

#define ZONEERR_RELATION19 337
/* @@error 337 不能把自己加入宿敌名单*/

#define ZONEERR_RELATION20 338
/* @@error 338 请解除其他关系后在添加宿敌名单*/

#define ZONEERR_RELATION21 339
/* @@error 339 宿敌名单已达到上限,请删除后再添加*/

#define ZONEERR_RELATION22 340
/* @@error 340 %s目前不在线,无法查看装备*/

#define ZONEERR_RELATION23 341
/* @@error 341 玩家%s 把你添加为凶手*/

#define ZONEERR_RELATION24 342
/* @@error 342 已经在宿敌名单中*/

#define ZONEERR_RELATION25 343
/* @@error 343 %s 把你加入宿敌名单*/

#define ZONEERR_RELATION26 344
/* @@error 344 玩家 %s 不在相同的服!*/

#define ZONEERR_RELATION27 345
/* @@error 345 同服的玩家才可加为好友*/

#define ZONEERR_RELATION28 346
/* @@error 346 同服的玩家才可加入黑名单*/

#define ZONEERR_SHOP1 347
/* @@error 347 没有足够的商品可卖了!*/

#define ZONEERR_SHOP2 348
/* @@error 348 一次性购买的东西太多，超过了上限*/

#define ZONEERR_SHOP3 349
/* @@error 349 购买商品失败*/

#define ZONEERR_NOMONEY1 350
/* @@error 350 没有足够的金券*/

#define ZONEERR_SHOP4 351
/* @@error 351 赠送商品成功*/

#define ZONEERR_SHOP5 352
/* @@error 352 购买商品成功*/

#define ZONEERR_NOMONEY2 353
/* @@error 353 没有足够的金币*/

#define ZONEERR_SHOP6 354
/* @@error 354 赠送失败*/

#define ZONEERR_SHOP7 355
/* @@error 355 对方不是你的好友,赠送失败*/

#define ZONEERR_SHOP8 356
/* @@error 356 该角色已被用户删除,赠送失败*/

#define ZONEERR_SHOP9 357
/* @@error 357 该商品还没到上架时间*/

#define ZONEERR_SHOP10 358
/* @@error 358 该商品下架了*/

#define ZONEERR_SHOP11 359
/* @@error 359 参数错误了*/

#define ZONEERR_SHOP12 360
/* @@error 360 交易失败，您出手慢了点，下次请果断出手*/

#define ZONEERR_SHOP13 361
/* @@error 361 不能交易自己的出售单*/

#define ZONEERR_SHOP14 362
/* @@error 362 你的%s将要达到上限，交易失败*/

#define ZONEERR_SHOP15 363
/* @@error 363 不能交易自己的发布单*/

#define ZONEERR_SHOP16 364
/* @@error 364 本服金币交易单量已达到上限，请稍后再试*/

#define ZONEERR_SHOP17 365
/* @@error 365 出售失败，你发布的金币单已到达上限*/

#define ZONEERR_SHOP18 366
/* @@error 366 %s不足, 无法出售*/

#define ZONEERR_SHOP19 367
/* @@error 367 您的银子不足以支付出售的手续费*/

#define ZONEERR_SHOP20 368
/* @@error 368 出售失败，你发布的银币单已到达上限*/

#define ZONEERR_SHOP21 369
/* @@error 369 本服银币交易单量已达到上限，请稍后再试*/

#define ZONEERR_SHOP22 370
/* @@error 370 你的金币将达到上限，撤销失败!*/

#define ZONEERR_SHOP23 371
/* @@error 371 你的银币将达到上限，撤销失败!*/

#define ZONEERR_SHOP24 372
/* @@error 372 交易单不存在*/

#define ZONEERR_SHOP25 373
/* @@error 373 没找到名字为 %s 的好友*/

#define ZONEERR_SHOP26 374
/* @@error 374 金币出售单失败，金币已通过邮件返还，请查收*/

#define ZONEERR_SHOP27 375
/* @@error 375 交易单发布成功!*/

#define ZONEERR_SHOP28 376
/* @@error 376 交易单发布失败，银币通过邮件返还，请查收*/

#define ZONEERR_SHOP29 377
/* @@error 377 撤销银币出售单成功，请按[B]键打开背包查收。*/

#define ZONEERR_SHOP30 378
/* @@error 378 兑换失败,请填入正确的值*/

#define ZONEERR_SHOP31 379
/* @@error 379 查询%s余额失败*/

#define ZONEERR_SHOP32 380
/* @@error 380 你消耗了%u银币购买了%u金币，请按【V】键打开商城查收*/

#define ZONEERR_SHOP33 381
/* @@error 381 您出售的%u金币已经出售成功，请查收邮件*/

#define ZONEERR_SHOP34 382
/* @@error 382 撤销%s出售单成功，请按[V]键打开商城查收*/

#define ZONEERR_SHOP35 383
/* @@error 383 您的金币出售单到期返还了，请通过邮件查收*/

#define ZONEERR_SHOP36 384
/* @@error 384 你消耗了%u金币购买了%u银币（已收税），请按【B】键打开背包查收*/

#define ZONEERR_SHOP37 385
/* @@error 385 您出售的%u银币已经出售成功，请查收邮件*/

#define ZONEERR_SHOP38 386
/* @@error 386 您的银币出售单到期返还了，请通过邮件查收*/

#define ZONEERR_SHOP39 387
/* @@error 387 余额不足，提取金币失败*/

#define ZONEERR_SHOP40 388
/* @@error 388 系统错误，提取金币失败*/

#define ZONEERR_STALL1 389
/* @@error 389 该位置已有摊位，请前往空旷区域*/

#define ZONEERR_STALL2 390
/* @@error 390 严禁在NPC周围摆摊，请前往空旷区域*/

#define ZONEERR_STALL3 391
/* @@error 391 当前爱心等级下的宠物不能交易*/

#define ZONEERR_STALL4 392
/* @@error 392 出征状态下的宠物不能交易*/

#define ZONEERR_STALL5 393
/* @@error 393 角色等级大于等于20级才能摆摊*/

#define ZONEERR_STALL6 394
/* @@error 394 切磋状态下不能摆摊*/

#define ZONEERR_STALL7 395
/* @@error 395 死亡状态下不能摆摊*/

#define ZONEERR_STALL8 396
/* @@error 396 你已经在摆摊了*/

#define ZONEERR_STALL9 397
/* @@error 397 死亡状态下无法查看摊位*/

#define ZONEERR_STALL10 398
/* @@error 398 距离太远无法查看摊位*/

#define ZONEERR_STALL11 399
/* @@error 399 摊主已经下线*/

#define ZONEERR_STALL12 400
/* @@error 400 摊主已经收摊*/

#define ZONEERR_STALL13 401
/* @@error 401 物品已经卖出或下架*/

#define ZONEERR_STALL14 402
/* @@error 402 摊主正在调整此物品*/

#define ZONEERR_STALL15 403
/* @@error 403 物品已经卖出,数量不够*/

#define ZONEERR_STALL16 404
/* @@error 404 摊主%s数量达到携带上限*/

#define ZONEERR_STALL17 405
/* @@error 405 宠物已经卖出或下架*/

#define ZONEERR_STALL18 406
/* @@error 406 摊主正在调整此宠物*/

#define ZONEERR_STALL19 407
/* @@error 407 宠物栏位已满*/

#define ZONEERR_STALL20 408
/* @@error 408 摆摊状态下不能扩充栏位*/

#define ZONEERR_STALL21 409
/* @@error 409 摊位栏已达到上限*/

#define ZONEERR_STALL22 410
/* @@error 410 没有相应的道具来扩充摊位栏*/

#define ZONEERR_STALL23 411
/* @@error 411 摊位容量已满，无法上架更多的物品*/

#define ZONEERR_STALL24 412
/* @@error 412 出售物品与身上携带的金额，不得超出总额上限*/

#define ZONEERR_PAK1 413
/* @@error 413 你获得了%d个%s*/

#define ZONEERR_PAK2 414
/* @@error 414 不能移动到目标栏位*/

#define ZONEERR_PAK3 415
/* @@error 415 商品背包移动是单向的*/

#define ZONEERR_PAK4 416
/* @@error 416 商城背包不能存储任务背包物品*/

#define ZONEERR_PAK5 417
/* @@error 417 不能移动物品*/

#define ZONEERR_PAK6 418
/* @@error 418 商城背包不能存储仓库背包物品*/

#define ZONEERR_PAK7 419
/* @@error 419 摆摊状态不能整理背包*/

#define ZONEERR_PAK8 420
/* @@error 420 交易状态不能整理背包*/

#define ZONEERR_PAK9 421
/* @@error 421 仓库物品已经整理*/

#define ZONEERR_PAK10 422
/* @@error 422 背包物品已经整理*/

#define ZONEERR_PAK11 423
/* @@error 423 坐骑与你职业不相符*/

#define ZONEERR_PAK12 424
/* @@error 424 此坐骑需要最小等级为%d级*/

#define ZONEERR_PAK13 425
/* @@error 425 坐骑性别不符*/

#define ZONEERR_PAK14 426
/* @@error 426 您已经拥有了该坐骑*/

#define ZONEERR_PAK15 427
/* @@error 427 坐骑栏位已满!*/

#define ZONEERR_PAK16 428
/* @@error 428 您已经不可再扩充坐骑栏位了*/

#define ZONEERR_ARM101 429
/* @@error 429 装备等级与洗炼石不符*/

#define ZONEERR_ARM102 430
/* @@error 430 装备洗炼所需的绑定材料不够*/

#define ZONEERR_ARM103 431
/* @@error 431 装备洗炼所需的非绑定的材料不够*/

#define ZONEERR_ARM104 432
/* @@error 432 %s不足，无法进行洗练操作*/

#define ZONEERR_ARM105 433
/* @@error 433 %s不足，无法进行强化操作*/

#define ZONEERR_ARM106 434
/* @@error 434 装备强化所需的绑定宝石不够*/

#define ZONEERR_ARM107 435
/* @@error 435 非绑定装备强化所需的非绑定的宝石不够*/

#define ZONEERR_ARM108 436
/* @@error 436 此升级符只能用于%d级的宝石!*/

#define ZONEERR_ARM109 437
/* @@error 437 四级以上宝石必须通过充能升级!*/

#define ZONEERR_ARM110 438
/* @@error 438 宝石:%s 已经是最高级宝石!*/

#define ZONEERR_ARM111 439
/* @@error 439 镶嵌失败，%s不足*/

#define ZONEERR_ARM112 440
/* @@error 440 装备孔已经满了，不能再打孔了*/

#define ZONEERR_ARM113 441
/* @@error 441 开启宝石孔失败，%s不足*/

#define ZONEERR_ARM114 442
/* @@error 442 很遗憾，打孔失败了，要继续打孔吗？*/

#define ZONEERR_ARM115 443
/* @@error 443 恭喜你打孔成功！*/

#define ZONEERR_ARM116 444
/* @@error 444 需要%d %s，你目前此种货币不足*/

#define ZONEERR_ARM117 445
/* @@error 445 转移品质所需的宝石不足*/

#define ZONEERR_ARM118 446
/* @@error 446 转移品质失败*/

#define ZONEERR_ARM119 447
/* @@error 447 很可惜，星级转移未能成功，请再次尝试*/

#define ZONEERR_ARM120 448
/* @@error 448 恭喜，装备星级转移成功！*/

#define ZONEERR_ARM121 449
/* @@error 449 金券不足,不可以解封*/

#define ZONEERR_ARM122 450
/* @@error 450 金币不足,不可以解封*/

#define ZONEERR_ARM123 451
/* @@error 451 时装不能分解!*/

#define ZONEERR_ARM124 452
/* @@error 452 不可分解高于自身等级20级的装备!*/

#define ZONEERR_ARM125 453
/* @@error 453 时装不能提炼!*/

#define ZONEERR_ARM126 454
/* @@error 454 不可提炼高于自身等级20级的装备!*/

#define ZONEERR_ARM127 455
/* @@error 455 需要天赋值达到100点才能使用晶石提炼!*/

#define ZONEERR_ARM128 456
/* @@error 456 提炼成功!*/

#define ZONEERR_NOENOUGH 457
/* @@error 457 %s 不足!*/

#define ZONEERR_ARM129 458
/* @@error 458 低于%d的宝石不能充能!*/

#define ZONEERR_ARM130 459
/* @@error 459 此宝石不能充能!*/

#define ZONEERR_ARM131 460
/* @@error 460 使用的材料不能重铸装备*/

#define ZONEERR_ARM132 461
/* @@error 461 请选择可以重铸此装备等级的重铸石*/

#define ZONEERR_ARM133 462
/* @@error 462 %s不足，无法进行重铸操作*/

#define ZONEERR_ARM134 463
/* @@error 463 重铸石不够!*/

#define ZONEERR_ARM135 464
/* @@error 464 %s不足，无法进行高级重铸*/

#define ZONEERR_NOMONEY3 465
/* @@error 465 没有足够的绑银*/

#define ZONEERR_ARM136 466
/* @@error 466 晶石已经是最高等级了!*/

#define ZONEERR_ATTR101 467
/* @@error 467 活力值已满*/

#define ZONEERR_ATTR102 468
/* @@error 468 增加%d点活力值*/

#define ZONEERR_ATTR103 469
/* @@error 469 增加%d点活力值(额外获得%d点) */

#define ZONEERR_ATTR104 470
/* @@error 470 消耗%d点活力值 */

#define ZONEERR_ATTR105 471
/* @@error 471 宠物携带数量已增加，可以捕获更多宠物了 */

#define ZONEERR_ATTR106 472
/* @@error 472 角色20级可以使用安全锁功能 */

#define ZONEERR_ATTR107 473
/* @@error 473 你的等级提升到了%d级*/

#define ZONEERR_ATTR108 474
/* @@error 474 由于等级封印的影响，你的经验获取效率下降到%d％*/

#define ZONEERR_PWORLD101 475
/* @@error 475 %s 等级低于副本最小等级要求!*/

#define ZONEERR_PWORLD102 476
/* @@error 476 %s 等级高于副本最大等级要求!*/

#define ZONEERR_PWORLD103 477
/* @@error 477 %s %s完成次数已达到每天完成次数上限!*/

#define ZONEERR_PWORLD104 478
/* @@error 478 %s正在进行中,无法开启其他副本*/

#define ZONEERR_PWORLD105 479
/* @@error 479 战斗状态无法传送*/

#define ZONEERR_PWORLD106 480
/* @@error 480 进入%s 队伍人数不能少于%d人!*/

#define ZONEERR_PWORLD107 481
/* @@error 481 队员%s 不在线或处于跨服状态!*/

#define ZONEERR_PWORLD108 482
/* @@error 482 队伍中只有队长才能开启此副本!*/

#define ZONEERR_PWORLD109 483
/* @@error 483 您至少需要一个空的背包格才能使用该物品*/

#define ZONEERR_PWORLD110 484
/* @@error 484  4小时内只能完成10次战场*/

#define ZONEERR_PWORLD111 485
/* @@error 485 人数不够,活动无法开启,请下次参加*/

#define ZONEERR_PWORLD112 486
/* @@error 486 你有 捣乱者 状态! 不能报名排队!*/

#define ZONEERR_PWORLD113 487
/* @@error 487 副本中不能报名排队!*/

#define ZONEERR_PWORLD114 488
/* @@error 488 此副本不能重复进入*/

#define ZONEERR_PWORLD115 489
/* @@error 489 当前等级不足，不能进入%s*/

#define ZONEERR_PWORLD116 490
/* @@error 490 未到开启时间，不能进入%s*/

#define ZONEERR_PWORLD117 491
/* @@error 491 %s进入次数已达到每天的次数上限*/

#define ZONEERR_PWORLD118 492
/* @@error 492 你的等级不符合，无法进入战场*/

#define ZONEERR_PWORLD119 493
/* @@error 493 你处于 蓄势待发状态，暂时不能进入战场*/

#define ZONEERR_PWORLD120 494
/* @@error 494 %s进入次数已达到每天的次数上限*/

#define ZONEERR_PWORLD121 495
/* @@error 495 思过厅中黑名玩家不能报名战场*/

#define ZONEERR_PWORLD122 496
/* @@error 496 你不在队伍中，无法队伍报名*/

#define ZONEERR_PWORLD123 497
/* @@error 497 你在队伍中，请叫队长报名*/

#define ZONEERR_PWORLD124 498
/* @@error 498 必须由队长报名!*/

#define ZONEERR_PWORLD125 499
/* @@error 499 %s 处于蓄势待发状态,暂时不能报名战场*/

#define ZONEERR_PWORLD126 500
/* @@error 500 %s 有 捣乱者 状态! 不能报名排队!*/

#define ZONEERR_PWORLD127 501
/* @@error 501 %s 等级不符合，无法进入战场*/

#define ZONEERR_PWORLD128 502
/* @@error 502 %s 处于跨服状态中,不能报名*/

#define ZONEERR_PWORLD129 503
/* @@error 503 %s 正在副本中,不能报名*/

#define ZONEERR_PWORLD130 504
/* @@error 504 思过厅中黑名玩家不能报名战场*/

#define ZONEERR_PWORLD131 505
/* @@error 505 %s的%s进入次数已达到每天的次数上限*/

#define ZONEERR_PWORLD132 506
/* @@error 506 %s不在线或不在本地图,不能进行报名*/

#define ZONEERR_PAK101 507
/* @@error 507 你拥有了%s 坐骑 */

#define ZONEERR_PAK102 508
/* @@error 508 您已经学会了此技能*/

#define ZONEERR_PAK103 509
/* @@error 509 当前无宠物出征.无法使用*/

#define ZONEERR_PAK104 510
/* @@error 510 宠物已到最高等级,不能获取经验*/

#define ZONEERR_PAK105 511
/* @@error 511 宠物等级过高,不能获取经验*/

#define ZONEERR_PAK106 512
/* @@error 512 宠物忠诚度已满，不需要再增加*/

#define ZONEERR_PAK107 513
/* @@error 513 死亡状态,无法拾取物品*/

#define ZONEERR_PAK108 514
/* @@error 514 距离太远无法拾取*/

#define ZONEERR_PAK109 515
/* @@error 515 此物品不归你所有，请等待物品属主保护时间过后再拾取*/

#define ZONEERR_PAK110 516
/* @@error 516 当前状态不能拾取物品*/

#define ZONEERR_PAK111 517
/* @@error 517 职业不符*/

#define ZONEERR_PAK112 518
/* @@error 518 等级不符*/

#define ZONEERR_PAK113 519
/* @@error 519 必须先学会 %s  技能*/

#define ZONEERR_PAK114 520
/* @@error 520 性别不符*/

#define ZONEERR_PAK115 521
/* @@error 521 黑名玩家才能穿戴此装备*/

#define ZONEERR_PAK116 522
/* @@error 522 红名玩家才能穿戴此装备*/

#define ZONEERR_PAK117 523
/* @@error 523 杀气值达到%d 才能穿戴此装备*/

#define ZONEERR_PAK118 524
/* @@error 524 使用失败,该物品使用权限为时间城城主*/

#define ZONEERR_PAK119 525
/* @@error 525 使用失败,该物品使用权限为时间城占领公会成员*/

#define ZONEERR_PAK120 526
/* @@error 526 需要天赋达到%d点*/

#define ZONEERR_PAK121 527
/* @@error 527 当前状态不能切换武器*/

#define ZONEERR_PAK122 528
/* @@error 528 你没有学会此技能,不能升级*/

#define ZONEERR_PAK123 529
/* @@error 529 技能等级不符*/

#define ZONEERR_PAK124 530
/* @@error 530 有阻挡无法使用道具*/

#define ZONEERR_PAK125 531
/* @@error 531 目标距离过远无法使用道具*/

#define ZONEERR_PAK126 532
/* @@error 532 只能对玩家使用*/

#define ZONEERR_PAK127 533
/* @@error 533 无法对目标使用*/

#define ZONEERR_PAK128 534
/* @@error 534 目标免疫此操作*/

#define ZONEERR_PAK129 535
/* @@error 535 目标已经有对应的状态,无法再对其使用此物品*/

#define ZONEERR_PAK130 536
/* @@error 536 你已经有相应的状态,无法使用此物品*/

#define ZONEERR_PAK131 537
/* @@error 537 能量已满，无需补充*/

#define ZONEERR_PAK132 538
/* @@error 538 你已经学过这个契约了*/

#define ZONEERR_PAK133 539
/* @@error 539 你的契约已经到上限了,请先清理*/

#define ZONEERR_PAK134 540
/* @@error 540 扩充杂物栏达到上限*/

#define ZONEERR_PAK135 541
/* @@error 541 宠物栏位不可再扩充*/

#define ZONEERR_PAK136 542
/* @@error 542 没有精灵出征*/

#define ZONEERR_PAK137 543
/* @@error 543 该物品已经生效,请稍后使用*/

#define ZONEERR_PAK138 544
/* @@error 544 道具的buff效果目前不能生效，暂时不能使用该道具*/

#define ZONEERR_PAK139 545
/* @@error 545 状态数量已到达上限无法再增加*/

#define ZONEERR_PAK140 546
/* @@error 546 摆摊状态下不能使用此道具*/

#define ZONEERR_PAK141 547
/* @@error 547 当前副本不能使用此道具*/

#define ZONEERR_PAK142 548
/* @@error 548 当前不能使用此道具*/

#define ZONEERR_PAK143 549
/* @@error 549 海滩椅太拥挤*/

#define ZONEERR_PAK144 550
/* @@error 550 只能在海滩上使用*/

#define ZONEERR_PAK145 551
/* @@error 551 你目前没有杀气值,不能使用此道具*/

#define ZONEERR_PAK146 552
/* @@error 552 不能在此地图上使用该物品*/

#define ZONEERR_PAK147 553
/* @@error 553 只能在指定区域内使用该物品*/

#define ZONEERR_PAK148 554
/* @@error 554 当前状态不能使用道具*/

#define ZONEERR_PAK149 555
/* @@error 555 此道具不能在跨服战场使用*/

#define ZONEERR_PAK150 556
/* @@error 556 此副本不能使用恢复性道具*/

#define ZONEERR_PAK151 557
/* @@error 557 物品使用期限到期，无法使用*/

#define ZONEERR_PAK152 558
/* @@error 558 角色名修改必须间隔7天*/

#define ZONEERR_PAK153 559
/* @@error 559 物品每日使用次数已达上限，无法使用*/

#define ZONEERR_PAK154 560
/* @@error 560 等级不符合,不能转职!*/

#define ZONEERR_PAK155 561
/* @@error 561 经验不足,不能转职!*/

#define ZONEERR_PAK156 562
/* @@error 562 战斗状态不可以使用此物品*/

#define ZONEERR_PAK157 563
/* @@error 563 乘坐机械不可以使用此物品*/

#define ZONEERR_PAK158 564
/* @@error 564 绑定物品不能流通*/

#define ZONEERR_PAK159 565
/* @@error 565 %s 正在使用中*/

#define ZONEERR_PAK160 566
/* @@error 566 %s 使用有效期已到*/

#define ZONEERR_PAK161 567
/* @@error 567 物品 %s 不能流通*/

#define ZONEERR_PAK162 568
/* @@error 568 物品 %s 是委托物品,不能流通*/

#define ZONEERR_PAK163 569
/* @@error 569 当前坐骑正在使用，不能丢弃*/

#define ZONEERR_PAK164 570
/* @@error 570 你丢弃了%s  */

#define ZONEERR_PAK165 571
/* @@error 571 距离太远了*/

#define ZONEERR_PAK166 572
/* @@error 572 不在队伍中无法申请上马*/

#define ZONEERR_PAK167 573
/* @@error 573 坐骑人数已满*/

#define ZONEERR_PAK168 574
/* @@error 574 不在同一个队伍中*/

#define ZONEERR_PAK169 575
/* @@error 575 不在同一个队伍中无法邀请对方*/

#define ZONEERR_PAK170 576
/* @@error 576 对方当前状态无法被邀请*/

#define ZONEERR_PAK171 577
/* @@error 577 你已经乘坐在他人的坐骑上了*/

#define ZONEERR_PAK172 578
/* @@error 578 当前状态无法接受骑乘邀请*/

#define ZONEERR_PAK173 579
/* @@error 579 不在同一个队伍中无法乘坐*/

#define ZONEERR_PAK174 580
/* @@error 580 对方没有骑乘*/

#define ZONEERR_PAK175 581
/* @@error 581 不可以乘坐*/

#define ZONEERR_PAK176 582
/* @@error 582 您离坐骑太远了，走近一些吧*/

#define ZONEERR_PAK177 583
/* @@error 583 缺少升级材料*/

#define ZONEERR_PAK178 584
/* @@error 584 %s不足，无法进行坐骑升级*/

#define ZONEERR_PAK179 585
/* @@error 585 无此坐骑*/

#define ZONEERR_PAK180 586
/* @@error 586 该坐骑已经升至最高等级，无需再进行强化*/

#define ZONEERR_PAK181 587
/* @@error 587 恭喜，你的%s成功升至%d级*/

#define ZONEERR_PAK182 588
/* @@error 588 很可惜，%s未能升级成功，速度不变*/

#define ZONEERR_PAK183 589
/* @@error 589 %s 拒绝了你的乘坐申请*/

#define ZONEERR_PAK184 590
/* @@error 590 %s 忽略了你的乘坐申请*/

#define ZONEERR_PAK185 591
/* @@error 591 %s 拒绝了你的乘坐邀请*/

#define ZONEERR_PAK186 592
/* @@error 592 %s 忽略了你的乘坐邀请*/

#define ZONEERR_PAK187 593
/* @@error 593 你不可以这么做*/

#define ZONEERR_PAK188 594
/* @@error 594 %s 请你离开了他的坐骑*/

#define ZONEERR_PAK189 595
/* @@error 595 无此坐骑*/

#define ZONEERR_PAK190 596
/* @@error 596 %s不足*/

#define ZONEERR_PAK191 597
/* @@error 597 副手武器不能替换主手武器*/

#define ZONEERR_PAK192 598
/* @@error 598 没有装备主手武器,不能装备副手武器*/

#define ZONEERR_PAK193 599
/* @@error 599 物品已经过期*/

#define ZONEERR_PAK194 600
/* @@error 600 不能把道具交换到失效的栏位中*/

#define ZONEERR_PAK195 601
/* @@error 601 请确保将要删除的栏位中没有物品*/

#define ZONEERR_PAK196 602
/* @@error 602 不能将物品移动到将要删除的栏位中*/

#define ZONEERR_PAK197 603
/* @@error 603 背包栏位增加%d个*/

#define ZONEERR_PAK198 604
/* @@error 604 背包栏位减少%d个*/

#define ZONEERR_PAK199 605
/* @@error 605 成功解锁了一个坐骑栏位*/

#define ZONEERR_PAK200 606
/* @@error 606 解锁坐骑栏位失败*/

#define ZONEERR_PAK201 607
/* @@error 607 宠物[%s]获得了新的技能栏位 */

#define ZONEERR_PAK202 608
/* @@error 608 您的宠物%s处于出战状态，不能进行技能扩充操作 */

#define ZONEERR_PAK203 609
/* @@error 609 宠物%s技能栏扩充已达到上限*/

#define ZONEERR_PAK204 610
/* @@error 610 当前%s不足*/

#define ZONEERR_PAK205 611
/* @@error 611 宠物[%s]获得了%d经验*/

#define ZONEERR_PAK206 612
/* @@error 612 宠物[%s]增加生命值%d*/

#define ZONEERR_PAK207 613
/* @@error 613 %s数量达到上限*/

#define ZONEERR_PAK208 614
/* @@error 614 %s 的有效期已到期，无法使用*/

#define ZONEERR_PAK209 615
/* @@error 615 装备:%s,id:%d,评分:%d*/

#define ZONEERR_PAK210 616
/* @@error 616 %s 使用有效期已到，请注意查看*/

#define ZONEERR_PAK211 617
/* @@error 617 %s 使用有效期小于10 分钟*/

#define ZONEERR_PAK212 618
/* @@error 618 %s 使用有效期小于3 小时*/

#define ZONEERR_PAK213 619
/* @@error 619 %s 使用有效期小于24 小时*/

#define ZONEERR_PAK214 620
/* @@error 620 你不可以学习这个配方*/

#define ZONEERR_PAK215 621
/* @@error 621 你的技能等级不足,无法学习*/

#define ZONEERR_PAK216 622
/* @@error 622 你已经学会了%s*/

#define ZONEERR_PAK217 623
/* @@error 623 背包已满,不能进行捕捉*/

#define ZONEERR_PAK218 624
/* @@error 624 捕捉%s失败*/

#define ZONEERR_PAK219 625
/* @@error 625 当前状态下无法使用*/

#define ZONEERR_PAK220 626
/* @@error 626 你不在队伍中*/

#define ZONEERR_PAK221 627
/* @@error 627 其他队伍成员都不在线或不符合条件*/

#define ZONEERR_PAK222 628
/* @@error 628 在副本中无法使用*/

#define ZONEERR_PAK223 629
/* @@error 629 未知的使用方式*/

#define ZONEERR_PAK224 630
/* @@error 630 错误的使用方式*/

#define ZONEERR_PAK225 631
/* @@error 631 已经达到记录数量上限*/

#define ZONEERR_PAK226 632
/* @@error 632 错误的位置索引*/

#define ZONEERR_PAK227 633
/* @@error 633 因为等级限制,你无法传送到目标地图*/

#define ZONEERR_PAK228 634
/* @@error 634 队长不在线,无法传送*/

#define ZONEERR_PAK229 635
/* @@error 635 队长在副本中,无法传送*/

#define ZONEERR_PAK230 636
/* @@error 636 你是队长,不需要使用此物品*/

#define ZONEERR_PAK231 637
/* @@error 637 当前状态下无法传送*/

#define ZONEERR_PAK232 638
/* @@error 638 因为等级限制,你无法传送到目标地图*/

#define ZONEERR_PAK233 639
/* @@error 639 无法传送到队长所在的地图*/

#define ZONEERR_PAK234 640
/* @@error 640 杂物包扩充栏位增加*/

#define ZONEERR_PAK235 641
/* @@error 641 银行扩充栏位增加*/

#define ZONEERR_PAK236 642
/* @@error 642 开启背包栏位失败,获得了%d个扣带部件*/

#define ZONEERR_PAK237 643
/* @@error 643 杂物包扩充栏位已经全部开启*/

#define ZONEERR_PAK238 644
/* @@error 644 银行扩充栏位已经全部开启*/

#define ZONEERR_PAK239 645
/* @@error 645 请按顺序开启扩充栏位*/

#define ZONEERR_PAK240 646
/* @@error 646 生命值已满，暂时不需要回复*/

#define ZONEERR_PAK241 647
/* @@error 647 银行栏位减少%d个*/

#define ZONEERR_PAK242 648
/* @@error 648 银行栏位增加%d个*/

#define ZONEERR_PAK243 649
/* @@error 649 魔法值已满，暂时不需要回复*/

#define ZONEERR_PAK244 650
/* @@error 650 你已经拥有这个称号*/

#define ZONEERR_PAK245 651
/* @@error 651 你称号数量达到上限*/

#define ZONEERR_PAK246 652
/* @@error 652 没有正在炼化的精灵*/

#define ZONEERR_PAK247 653
/* @@error 653 宠物等级不符*/

#define ZONEERR_PAK248 654
/* @@error 654 该物品正在冷却中*/

#define ZONEERR_PAK249 655
/* @@error 655 你使用了物品 %s*/

#define ZONEERR_PAK250 656
/* @@error 656 %s 对你使用了物品 %s*/

#define ZONEERR_PAK251 657
/* @@error 657 你对 %s 使用了物品 %s*/

#define ZONEERR_PAK252 658
/* @@error 658 你放不下这么多%s*/

#define ZONEERR_PAK253 659
/* @@error 659 这个卷轴不能用于这个部位的装备*/

#define ZONEERR_PAK254 660
/* @@error 660 时装不能刻印*/

#define ZONEERR_PAK255 661
/* @@error 661 你指定的物品不是一件装备*/

#define ZONEERR_PAK256 662
/* @@error 662 装备等级不能低于%d级*/

#define ZONEERR_PAK257 663
/* @@error 663 你指定的物品不是附魔卷轴*/

#define ZONEERR_PAK258 664
/* @@error 664 你指定的物品不是一件装备*/

#define ZONEERR_PAK259 665
/* @@error 665 这件物品的等级没有达到此附魔要求的最小等级(Lv.%d)*/

#define ZONEERR_PAK260 666
/* @@error 666 这件物品的等级已经高于此附魔要求的最大等级(Lv.%d)*/

#define ZONEERR_PAK261 667
/* @@error 667 今天藏宝图使用次数已经达到%d次上限*/

#define ZONEERR_PAK262 668
/* @@error 668 %s 容量增加%d 个*/

#define ZONEERR_PAK263 669
/* @@error 669 需要基础天赋达到%d点*/

#define ZONEERR_COMBAT1 669
/* @@error 669 此坐骑无法在当前地图内骑乘*/

#define ZONEERR_GAY1 670
/* @@error 670 你需要拥有一个队伍，并且必须是队长*/

#define ZONEERR_GAY2 671
/* @@error 671 缺少物品：%s，配对失败*/

#define ZONEERR_GAY3 672
/* @@error 672 你的配对申请已发送，请耐心等待回应*/

#define ZONEERR_GAY4 673
/* @@error 673 当前视野内没有队友，配对申请发送失败*/

#define ZONEERR_GAY5 674
/* @@error 674 %s无应答，你发送的配对邀请超时，请稍后再试*/

#define ZONEERR_GAY6 675
/* @@error 675 %s拒绝了你的邀请，配对失败*/

#define ZONEERR_GAY7 676
/* @@error 676 由于你离开了队伍，配对失败*/

#define ZONEERR_GAY8 677
/* @@error 677 此配对请求已失效*/

#define ZONEERR_GAY9 678
/* @@error 678 由于玩家：%s缺少物品：%s，配对失败*/

#define ZONEERR_GAY10 679
/* @@error 679 %s接受了你的邀请，配对成功*/

#define ZONEERR_GAY11 680
/* @@error 680 你接受了%s的邀请，配对成功*/

#define ZONEERR_MACHINE1 681
/* @@error 681 当前状态无法骑乘*/

#define ZONEERR_MACHINE2 682
/* @@error 682 当前状态下不允许进行乘骑机械操作*/

#define ZONEERR_MACHINE3 683
/* @@error 683 此地图不可骑乘机械*/

#define ZONEERR_MACHINE4 684
/* @@error 684 当前地图不可骑乘此机械*/

#define ZONEERR_MACHINE5 685
/* @@error 685 当前座位有人*/

#define ZONEERR_MACHINE6 686
/* @@error 686 当前机械不可以改变座位*/

#define ZONEERR_MACHINE7 687
/* @@error 687 当前状态无法乘坐机械*/

#define ZONEERR_MACHINE8 688
/* @@error 688 机械与您的距离太远，无法乘骑*/

#define ZONEERR_MACHINE9 689
/* @@error 689 你已经乘坐在他人的机械上了*/

#define ZONEERR_MACHINE10 690
/* @@error 690 对方没有骑乘机械*/

#define ZONEERR_MACHINE11 691
/* @@error 691 上车失败，该机械乘骑人数已满*/

#define ZONEERR_MACHINE12 692
/* @@error 692 单人机械不可邀请*/

#define ZONEERR_MACHINE13 693
/* @@error 693 机械乘骑人数已满，无法邀请*/

#define ZONEERR_MACHINE14 694
/* @@error 694 距离太远了*/

#define ZONEERR_MACHINE15 695
/* @@error 695 %s 拒绝了你的乘坐邀请*/

#define ZONEERR_MACHINE16 696
/* @@error 696 %s 忽略了你的乘坐邀请*/

#define ZONEERR_MACHINE17 697
/* @@error 697 当前状态无法骑乘*/

#define ZONEERR_MACHINE18 698
/* @@error 698 当前状态无法乘坐机械*/

#define ZONEERR_MACHINE19 699
/* @@error 699 %s已经乘上您的机械与您共同作战*/

#define ZONEERR_MACHINE20 700
/* @@error 700 当前货车不能更换*/

#define ZONEERR_MACHINE21 701
/* @@error 701 货车不在附近,无法进行换车*/

#define ZONEERR_MACHINE22 702
/* @@error 702 缺少道具:%s */

#define ZONEERR_MACHINE23 703
/* @@error 703 重新租赁货车,需要支付相应数量的换车合同*/

#define ZONEERR_MACHINE24 704
/* @@error 704 %s请您离开了他的机械*/

#define ZONEERR_MACHINE25 705
/* @@error 705 %s离开了您的机械*/

#define ZONEERR_MACHINE26 706
/* @@error 706 此机械已损坏，无法使用*/

#define ZONEERR_MACHINE27 707
/* @@error 707 您还未使用采集车采集，无法提取资源*/

#define ZONEERR_MACHINE28 708
/* @@error 708 您还没加入公会，无法使用该物品*/

#define ZONEERR_MACHINE29 709
/* @@error 709 占领失败，%s已被占领*/

#define ZONEERR_MACHINE30 710
/* @@error 710 建造失败，采集场只能在10：00-24：00之间建造*/

#define ZONEERR_MACHINE31 711
/* @@error 711 该道具需要乘骑机械后才能使用*/

#define ZONEERR_MACHINE32 712
/* @@error 712 您的机械目前无需恢复能量*/

#define ZONEERR_MACHINE33 713
/* @@error 713 您的机械目前无需进行修理*/

#define ZONEERR_WEATHER1 714
/* @@error 714 权限不足,只有时间城城主才可进行此操作*/

#define ZONEERR_WEATHER2 715
/* @@error 715 已成功将时间城区域设置为%s!*/

#define ZONEERR_VIP1 716
/* @@error 716 该钻不存在,开钻失败*/

#define ZONEERR_VIP2 717
/* @@error 717 您的%s不足，本次购买失败*/

#define ZONEERR_VIP3 718
/* @@error 718 开通 %s 失败了*/

#define ZONEERR_VAL1 719
/* @@error 719 次数最大修正成10w次了*/

#define ZONEERR_MAIL101 720
/* @@error 720 你没有足够的%s发送邮件*/

#define ZONEERR_MAIL102 721
/* @@error 721 您的包裹内没有任何物品*/

#define ZONEERR_MAIL103 722
/* @@error 722 发送邮件所需的道具数量不足*/

#define ZONEERR_MAIL104 723
/* @@error 723 你没有选定用于发送邮件的邮件包裹*/

#define ZONEERR_MAIL105 724
/* @@error 724 您选定的物品不是邮件包裹*/

#define ZONEERR_MAIL106 725
/* @@error 725 发送邮件所需的邮件包裹数量不足*/

#define ZONEERR_MAIL107 726
/* @@error 726 服务器暂忙，邮件发送失败，请稍候再试*/

#define ZONEERR_MAIL108 727
/* @@error 727 找不到收件人，邮件发送失败*/

#define ZONEERR_MAIL109 728
/* @@error 728 邮件删除失败*/

#define ZONEERR_MAIL110 729
/* @@error 729 请输入收件人名称*/

#define ZONEERR_MAIL111 730
/* @@error 730 请输入邮件标题*/

#define ZONEERR_MAIL112 731
/* @@error 731 不能给自己发送邮件*/

#define ZONEERR_MAIL113 732
/* @@error 732 收件人在你的黑名单中，不能发送邮件*/

#define ZONEERR_MAIL114 733
/* @@error 733 收件人在你的宿敌名单中，不能发送邮件*/

#define ZONEERR_MAIL115 734
/* @@error 734 你在对方黑名单中，不能发送邮件*/

#define ZONEERR_MAIL116 735
/* @@error 735 你是收件人的宿敌，不能发送邮件*/

#define ZONEERR_MAIL117 736
/* @@error 736 收件人邮箱已满，邮件发送失败*/

#define ZONEERR_MAIL118 737
/* @@error 737 你已死亡,不能使用邮件*/

#define ZONEERR_TRANS101 738
/* @@error 738 只有时间城主公会成员才能使用此线路*/

#define ZONEERR_TRANS102 739
/* @@error 739 等级达到%d才能使用此线路*/

#define ZONEERR_TRANS103 740
/* @@error 740 此地图太危险了,还是等%d级以后再去吧*/

#define ZONEERR_TMP101 741
/* @@error 741 当前道具不是你的*/

#define ZONEERR_TMP102 742
/* @@error 742 当前不能下临时机械*/

#define ZONEERR_TEAM101 743
/* @@error 743 你更换契约时间间隔太短*/

#define ZONEERR_TEAM102 744
/* @@error 744 死亡状态不能组队*/

#define ZONEERR_TEAM103 745
/* @@error 745 对方是死亡状态*/

#define ZONEERR_TEAM104 746
/* @@error 746 目标已有队伍,无法接受组队邀请*/

#define ZONEERR_TEAM105 747
/* @@error 747 目标在你的宿敌名单中,无法组队,请按F键打开宿敌名单查看*/

#define ZONEERR_TEAM106 748
/* @@error 748 队伍人数已满*/

#define ZONEERR_TEAM107 749
/* @@error 749 队长不在线,请稍后申请*/

#define ZONEERR_TEAM108 750
/* @@error 750 对方已经下线*/

#define ZONEERR_TEAM109 751
/* @@error 751 对方跨服状态不能组队*/

#define ZONEERR_TEAM110 752
/* @@error 752 你不是队长无法邀请%s进入队伍*/

#define ZONEERR_TEAM111 753
/* @@error 753 目标在你的黑名单中,无法组队,请按F键打开黑名单查看*/

#define ZONEERR_TEAM112 754
/* @@error 754 目标在你的宿敌名单中,无法组队,请按F键打开宿敌名单查看*/

#define ZONEERR_TEAM113 755
/* @@error 755 组队成功，你为队长，分配模式为[%s] */

#define ZONEERR_TEAM114 756
/* @@error 756 加入队伍，队长为%s，分配模式为[%s] */

#define ZONEERR_TEAM115 757
/* @@error 757 %s 拒绝了你的组队申请*/

#define ZONEERR_TEAM116 758
/* @@error 758 %s 已经加入其他队伍*/

#define ZONEERR_TEAM117 759
/* @@error 759 玩家 %s 对你的请求无应答,请稍后再试*/

#define ZONEERR_TEAM118 760
/* @@error 760 转让的对象不在线*/

#define ZONEERR_TEAM119 761
/* @@error 761 你已经是队长*/

#define ZONEERR_TEAM120 762
/* @@error 762 当前正在投票过程,请稍后再试*/

#define ZONEERR_TEAM121 763
/* @@error 763 三分钟内只能发起一次投票请求,请稍后再试*/

#define ZONEERR_TEAM122 764
/* @@error 764 无此契约*/

#define ZONEERR_TEAM123 765
/* @@error 765 你已经发送过申请，等待对方回复*/

#define ZONEERR_TEAM124 766
/* @@error 766 你已经发送过邀请，请稍候再试*/

#define ZONEERR_TEAM125 767
/* @@error 767 你在对方黑名单中*/

#define ZONEERR_TEAM126 768
/* @@error 768 你在对方宿敌名单中*/

#define ZONEERR_TEAM127 769
/* @@error 769 你不是队长*/

#define ZONEERR_TEAM128 770
/* @@error 770 你离开了队伍*/

#define ZONEERR_TEAM129 771
/* @@error 771 %s 未被请离队伍*/

#define ZONEERR_TEAM130 772
/* @@error 772 地图内存在需要ROLL点的装备，请ROLL点后再发起投票*/

#define ZONEERR_TEAM131 773
/* @@error 773 不能开除自己*/

#define ZONEERR_TEAM132 774
/* @@error 774 不同服玩家不能组队*/

#define ZONEERR_TEAM133 775
/* @@error 775 队员:%s 目前处于不能组队跟随状态*/

#define ZONEERR_TEAM134 776
/* @@error 776 当前状态不能要求队伍跟随*/

#define ZONEERR_TEAM135 777
/* @@error 777 组队跟随失败*/

#define ZONEERR_TEAM136 778
/* @@error 778 队长处于不可跟随状态*/

#define ZONEERR_TEAM137 779
/* @@error 779 队伍成员 %s 不能使用此线路*/

#define ZONEERR_TEAM138 780
/* @@error 780 队伍成员 %s 不能进入此地图*/

#define ZONEERR_TEAM139 781
/* @@error 781 队伍召唤失效*/

#define ZONEERR_TEAM140 782
/* @@error 782 离开召唤所在战场，传送失败*/

#define ZONEERR_TEAM141 783
/* @@error 783 你不在队伍中*/

#define ZONEERR_TEAM142 784
/* @@error 784 你和队伍召唤发起者不在同一个队伍中*/

#define ZONEERR_TEAM143 785
/* @@error 785 已经超出了队伍召唤的有效时间*/

#define ZONEERR_TEAM144 786
/* @@error 786 当前状态下无法传送*/

#define ZONEERR_TEAM145 787
/* @@error 787 因为等级限制,你无法传送到目标地图*/

#define ZONEERR_RONGLU1 788
/* @@error 788 请到 指定地图 使用此道具*/

#define ZONEERR_RONGLU2 789
/* @@error 789 熔炉能量已满,无需重新放置*/

#define ZONEERR_RONGLU3 790
/* @@error 790 不能使用此熔炉*/

#define ZONEERR_RONGLU4 791
/* @@error 791 魔法熔炉能量已满，可采集获得宝物*/

#define ZONEERR_RONGLU5 792
/* @@error 792 熔炉已放置，不可重铸*/

#define ZONEERR_RONGLU6 793
/* @@error 793 当前没有可重铸的熔炉*/

#define ZONEERR_RESPACT1 794
/* @@error 794 你在%s中的声望增加了%u点*/

#define ZONEERR_RESPACT2 795
/* @@error 795 你在%s中的声望增加了%u点(防沉迷中,收益减少50％)　*/

#define ZONEERR_RESPACT3 796
/* @@error 796 你在%s中的声望已经达到%s */

#define ZONEERR_SECSKILL1 797
/* @@error 797 缺少所需材料*/

#define ZONEERR_SECSKILL2 798
/* @@error 798 你的技能等级不足,无法学习*/

#define ZONEERR_SECSKILL3 799
/* @@error 799 你已经学会了*/

#define ZONEERR_NOHUOBI 800
/* @@error 800 所需货币不足*/

#define ZONEERR_SECSKILL4 801
/* @@error 801 %s还在冷却中*/

#define ZONEERR_SECSKILL5 802
/* @@error 802 你未学会这个科技生产专业*/

#define ZONEERR_SECSKILL6 803
/* @@error 803 不可以从面板学习*/

#define ZONEERR_SECSKILL7 804
/* @@error 804 这个物品不可被分解*/

#define ZONEERR_SECSKILL8 805
/* @@error 805 你没有这个科技生产专业*/

#define ZONEERR_MISC1 806
/* @@error 806 对方不在指定的切磋范围*/

#define ZONEERR_MISC2 807
/* @@error 807 你已经在切磋状态下*/

#define ZONEERR_MISC3 808
/* @@error 808 对方已经在切磋状态下*/

#define ZONEERR_MISC4 809
/* @@error 809 对方摆摊状态下不能切磋*/

#define ZONEERR_MISC5 810
/* @@error 810 对方交易状态下不能切磋*/

#define ZONEERR_MISC6 811
/* @@error 811 对方在无双状态下,不能切磋*/

#define ZONEERR_MISC7 812
/* @@error 812 对方已经死亡*/

#define ZONEERR_MISC107 813
/* @@error 813 距离太远,无法切磋*/

#define ZONEERR_MISC8 814
/* @@error 814 切磋邀请已经取消*/

#define ZONEERR_MISC9 815
/* @@error 815 %s 同意了你的切磋邀请,3秒后开始切磋*/

#define ZONEERR_MISC10 816
/* @@error 816 3秒后开始切磋*/

#define ZONEERR_MISC11 817
/* @@error 817 %s 拒绝了你的切磋邀请*/

#define ZONEERR_MISC12 818
/* @@error 818 此地不允许切磋*/

#define ZONEERR_MISC13 819
/* @@error 819 摆摊状态下不能切磋*/

#define ZONEERR_MISC14 820
/* @@error 820 交易状态下不能切磋*/

#define ZONEERR_MISC15 821
/* @@error 821 死亡状态不允许切磋*/

#define ZONEERR_MISC16 822
/* @@error 822 无双状态下不能切磋*/

#define ZONEERR_MISC17 823
/* @@error 823 切磋取消*/

#define ZONEERR_MISC18 824
/* @@error 824 切磋失败*/

#define ZONEERR_MISC19 825
/* @@error 825 切磋胜利*/

#define ZONEERR_MISC20 826
/* @@error 826 切磋开始*/

#define ZONEERR_MISC21 827
/* @@error 827 你已离开切磋区域,5秒后将失败*/

#define ZONEERR_MISC22 828
/* @@error 828 其他客户端正在进行类似操作,请稍后再试*/

#define ZONEERR_MISC23 829
/* @@error 829 没有对应的礼包信息或礼包已过期*/

#define ZONEERR_MISC24 830
/* @@error 830 数据更新失败,无法完成此操作*/

#define ZONEERR_UNKNOWN 831
/* @@error 831 未知错误*/

#define ZONEERR_MISC25 832
/* @@error 832 当前不能抽奖*/

#define ZONEERR_MISC26 833
/* @@error 833 今天抽奖次数已经用完*/

#define ZONEERR_MISC27 834
/* @@error 834 恭喜你!通过抽奖获得%d个 %s*/

#define ZONEERR_ARM201 835
/* @@error 835 此装备还未鉴定,不能进行此操作*/

#define ZONEERR_BOOTY201 836
/* @@error 836 你从%s身上抽取到%d点魂能*/

#define ZONEERR_BUSY201 837
/* @@error 837 任务正在进行中不能直接完成*/

#define ZONEERR_BUSY202 838
/* @@error 838 未加入公会无法操作*/

#define ZONEERR_BUSY203 839
/* @@error 839 此任务的每日完成次数已达上限*/

#define ZONEERR_BUSY204 840
/* @@error 840 %s环组任务已经在进行中，只有未进行的环组才能直接完成*/

#define ZONEERR_BUSY205 841
/* @@error 841 环组只能直接完成第一次*/

#define ZONEERR_BUSY206 842
/* @@error 842 您已经在副本中，不允许该操作*/

#define ZONEERR_BUSY207 843
/* @@error 843 副本完成次数已达到今日上限*/

#define ZONEERR_BUSY208 844
/* @@error 844 您已经在海滩，不允许该操作*/

#define ZONEERR_BUSY209 845
/* @@error 845 您今天已用过海滩时间，只有未用过海滩时间才可以直接完成*/

#define ZONEERR_BUSY210 846
/* @@error 846 您的替身机器人数量不足*/

#define ZONEERR_BUSY211 847
/* @@error 847 【%s】已完成*/

#define ZONEERR_CITY201 848
/* @@error 848 公会资金不足，无法操作*/

#define ZONEERR_CITY202 849
/* @@error 849 指定的材料不存在,无法操作*/

#define ZONEERR_CITY203 850
/* @@error 850 缺少材料 %s ，无法操作*/

#define ZONEERR_CITY204 851
/* @@error 851 受到等级封印影响，暂无法升级到2级建筑*/

#define ZONEERR_CITY205 852
/* @@error 852 受到等级封印影响，暂无法升级到3级建筑*/

#define ZONEERR_CITY206 853
/* @@error 853 公会资金不足，无法充能*/

#define ZONEERR_CITY207 854
/* @@error 854 公会资金不足，无法修理*/

#define ZONEERR_CITY208 855
/* @@error 855 操作失败，当前操作只有在非战争期间才可进行*/

#define ZONEERR_CITY209 856
/* @@error 856 只有公会会长与副会长有此操作权限*/

#define ZONEERR_CITY210 857
/* @@error 857 您今天已朝拜或扔过鸡蛋了，请明天再来*/

#define ZONEERR_CITY211 858
/* @@error 858 无人竖立城主雕像，活动暂未开启*/

#define ZONEERR_CITY212 859
/* @@error 859 由于新城主未竖立个人雕像，活动暂时关闭*/

#define ZONEERR_CITY213 860
/* @@error 860 成功表达了你对时间城城主的仰慕之情*/

#define ZONEERR_CITY214 861
/* @@error 861 您向城主雕像扔了一枚臭鸡蛋*/

#define ZONEERR_CITY215 862
/* @@error 862 您还未竖立个人雕像，无法领取雕像收益*/

#define ZONEERR_CITY216 863
/* @@error 863 领取失败，只有属于自己的雕像才可领取*/

#define ZONEERR_CITY217 864
/* @@error 864 您今日已领取过雕像收益，请明日再来*/

#define ZONEERR_CITY218 865
/* @@error 865 成功领取昨日雕像收益分成：%d银币*/

#define ZONEERR_CLAN201 866
/* @@error 866 你已经加入了一个公会*/

#define ZONEERR_CLAN202 867
/* @@error 867 你刚刚退出或解散了公会，请等待%s后再创建*/

#define ZONEERR_CLAN203 868
/* @@error 868 申请者: %s 已经下线*/

#define ZONEERR_CLAN204 869
/* @@error 869 你的入会申请已经失效*/

#define ZONEERR_CLAN205 870
/* @@error 870 申请者: %s 的入会申请已经失效*/

#define ZONEERR_CLAN206 871
/* @@error 871 申请者: %s 已经加入了别的公会*/

#define ZONEERR_CLAN207 872
/* @@error 872 公会[%s] 已经不存在, 取消%s */

#define ZONEERR_CLAN208 873
/* @@error 873 公会[%s] 的申请已超时, 从%s列表中删除*/

#define ZONEERR_CLAN209 874
/* @@error 874 公会资金已达上限，不能再增加了*/

#define ZONEERR_CLAN210 875
/* @@error 875 公会安定度已达上限，不能再增加了*/

#define ZONEERR_CLAN211 876
/* @@error 876 公会科技点已达上限，不能再增加了*/

#define ZONEERR_CLAN212 877
/* @@error 877 %s 不是公会 %s 的成员*/

#define ZONEERR_CLAN213 878
/* @@error 878 你的个人贡献度增加了%d */

#define ZONEERR_CLAN214 879
/* @@error 879 你的个人贡献度已达最大值*/

#define ZONEERR_CLAN215 880
/* @@error 880 我们抱歉的通知您, 由于您所在公会人数不足,将在%d天内被系统解散*/

#define ZONEERR_CLAN216 881
/* @@error 881 同服的玩家才可邀请入会*/

#define ZONEERR_CLAN217 882
/* @@error 882 你已经加入了一个公会,无法再创建公会*/

#define ZONEERR_CLAN218 883
/* @@error 883 公会数量已达到上限,无法再创建公会*/

#define ZONEERR_CLAN219 884
/* @@error 884 你刚刚退出或解散了公会，请等待%s后再创建*/

#define ZONEERR_CLAN220 885
/* @@error 885 你的等级未到%d级,现在还不能创建公会*/

#define ZONEERR_CLAN221 886
/* @@error 886 创建公会缺少道具，需要%d个%s */

#define ZONEERR_CLAN222 887
/* @@error 887 你缺少创建公会需要的道具 */

#define ZONEERR_CLAN223 888
/* @@error 888 你的%s少于%d,创建公会失败*/

#define ZONEERR_CLAN224 889
/* @@error 889 公会[%s] 的同盟列表已满, 不能再增加新的公会*/

#define ZONEERR_CLAN225 890
/* @@error 890 公会[%s] 的同盟列表已满, 不能再增加新的公会*/

#define ZONEERR_CLAN226 891
/* @@error 891 公会的敌对列表已满, 不能再新其他公会宣战*/

#define ZONEERR_CLAN227 892
/* @@error 892 你成功创建了一个公会*/

#define ZONEERR_CLAN228 893
/* @@error 893 角色 %s 已经不在公会中*/

#define ZONEERR_CLAN229 894
/* @@error 894 你还没有加入一个公会*/

#define ZONEERR_CLAN230 895
/* @@error 895 您已经启动了公会解散倒计时，请%s后再次确认解散*/

#define ZONEERR_CLAN231 896
/* @@error 896 修改公告过于频繁,请于%d秒后再修改*/

#define ZONEERR_CLAN232 897
/* @@error 897 你刚刚退出了公会，无法再加入公会，请等待%s后再试*/

#define ZONEERR_CLAN233 898
/* @@error 898 你申请加入的公会已不存在*/

#define ZONEERR_CLAN234 899
/* @@error 899 你加入公会 %s 的申请已经失效*/

#define ZONEERR_CLAN235 900
/* @@error 900 你已进入公会 %s 的预备会员列表中*/

#define ZONEERR_CLAN236 901
/* @@error 901 无法找到角色%s */

#define ZONEERR_CLAN237 902
/* @@error 902 %s已经加入了其他的公会*/

#define ZONEERR_CLAN238 903
/* @@error 903 你刚刚退出公会,无法再加入公会,请等待%s后再试*/

#define ZONEERR_CLAN239 904
/* @@error 904 %s刚刚退出了公会,无法再加入公会,请等待%s后再试*/

#define ZONEERR_CLAN240 905
/* @@error 905 同盟列表已满*/

#define ZONEERR_CLAN241 906
/* @@error 906 对方公会无人处理此事务*/

#define ZONEERR_CLAN242 907
/* @@error 907 对方公会忙*/

#define ZONEERR_CLAN243 908
/* @@error 908 指定的公会已经不存在*/

#define ZONEERR_CLAN244 909
/* @@error 909 公会[%s] 的同盟列表已满*/

#define ZONEERR_CLAN245 910
/* @@error 910 已向公会[%s] 发送同盟申请*/

#define ZONEERR_CLAN246 911
/* @@error 911 对指定的公会宣战CD还在冷却中*/

#define ZONEERR_CLAN247 912
/* @@error 912 需要%d银币才能向其他公会宣战*/

#define ZONEERR_CLAN248 913
/* @@error 913 指定的公会已经不存在*/

#define ZONEERR_CLAN249 914
/* @@error 914 公会[%s] 的敌对列表已满*/

#define ZONEERR_CLAN250 915
/* @@error 915 指定的公会不在同盟列表中*/

#define ZONEERR_CLAN251 916
/* @@error 916 %s 拒绝了你的入会邀请*/

#define ZONEERR_CLAN252 917
/* @@error 917 %s 没有响应你的入会邀请*/

#define ZONEERR_CLAN253 918
/* @@error 918 %s 拒绝了你的同盟邀请*/

#define ZONEERR_CLAN254 919
/* @@error 919 %s 没有响应你的同盟邀请*/

#define ZONEERR_CLAN255 920
/* @@error 920 查询结果过多,请指定更准确的公会名*/

#define ZONEERR_CLAN256 921
/* @@error 921 公会[%s] 已经是盟友或正在申请盟友或敌对,所以无法再加入盟友列表*/

#define ZONEERR_CLAN257 922
/* @@error 922 公会[%s] 已经不不在申请列表中*/

#define ZONEERR_CLAN258 923
/* @@error 923 公会[%s] 已经不存在了, 从申请列表中移除*/

#define ZONEERR_CLAN259 924
/* @@error 924 公会的结盟请求被[%s] 公会的%s拒绝了*/

#define ZONEERR_CLAN260 925
/* @@error 925 没有占领次数对应的资金*/

#define ZONEERR_CLAN261 926
/* @@error 926 只有占领公会会长有权领取占领资金*/

#define ZONEERR_CLAN262 927
/* @@error 927 你不是时间城城主公会成员*/

#define ZONEERR_CLAN263 928
/* @@error 928 操作失败，只有入会超过2天角色才可进行此操作*/

#define ZONEERR_CLAN264 929
/* @@error 929 今天的占领资金已领取,无法重复领取*/

#define ZONEERR_CLAN265 930
/* @@error 930 没有占领次数对应的资金*/

#define ZONEERR_CLAN266 931
/* @@error 931 尊敬的时间城主，您已成功领取今天的占领资金%d%s */

#define ZONEERR_CLAN267 932
/* @@error 932 找不到指定的公会成员*/

#define ZONEERR_CLAN268 933
/* @@error 933 官衔名称含有敏感词汇*/

#define ZONEERR_CLAN269 934
/* @@error 934 你将官衔设置得太低了*/

#define ZONEERR_CLAN270 935
/* @@error 935 不能给比自己官衔高或相同的人设置官衔*/

#define ZONEERR_CLAN271 936
/* @@error 936 你没有权限将官衔设置得比自己相同或更高*/

#define ZONEERR_CLAN272 937
/* @@error 937 一个公会只能有一个会长*/

#define ZONEERR_CLAN273 938
/* @@error 938 一个公会只能有一个副会长*/

#define ZONEERR_CLAN274 939
/* @@error 939 一个战团只能有一个团长*/

#define ZONEERR_CLAN275 940
/* @@error 940 公会颜色设置太多*/

#define ZONEERR_CLAN276 941
/* @@error 941 公会颜色设置成功*/

#define ZONEERR_CLAN277 942
/* @@error 942 无效的技能或等级*/

#define ZONEERR_CLAN278 943
/* @@error 943 公会资金不足,无法研发此技能*/

//#define ZONEERR_CLAN279 944
/* @@error 944 公会技能已满,无法研究*/

#define ZONEERR_CLAN279 944
/* @@error 944 公会技能无法研发*/

#define ZONEERR_CLAN280 945
/* @@error 945 此技能有效等级比研发等级低,无法继续研发*/

#define ZONEERR_CLAN281 946
/* @@error 946 此技能无法研发*/

#define ZONEERR_CLAN282 947
/* @@error 947 公会还没有研发此技能*/

#define ZONEERR_CLAN283 948
/* @@error 948 此技能无法学习或升级*/

#define ZONEERR_CLAN284 949
/* @@error 949 无效的技能或等级*/

#define ZONEERR_CLAN285 950
/* @@error 950 贡献度不足,无法学习或升级此技能*/

#define ZONEERR_CLAN286 951
/* @@error 951 此技能无法手动学习或升级*/

#define ZONEERR_CLAN287 952
/* @@error 952 公会技能已满,无法学习*/

#define ZONEERR_CLAN288 953
/* @@error 953 未学习的公会技能无法升级*/

#define ZONEERR_CLAN289 954
/* @@error 954 需要学习此技能的前置等级才能升级*/

#define ZONEERR_CLAN290 955
/* @@error 955 无效的地图位置*/

#define ZONEERR_CLAN291 956
/* @@error 956 你不能给自己设置战团*/

#define ZONEERR_CLAN292 957
/* @@error 957 只有普通会员可以加入战团*/

#define ZONEERR_CLAN293 958
/* @@error 958 不可把其它战团成员加到自己战团*/

#define ZONEERR_CLAN294 959
/* @@error 959 该地图不能使用此物品*/

#define ZONEERR_CLAN295 960
/* @@error 960 公会召唤发起者不在线,召唤失败*/

#define ZONEERR_CLAN296 961
/* @@error 961 你不在公会中*/

#define ZONEERR_CLAN297 962
/* @@error 962 你和公会召唤发起者不在同一个公会中*/

#define ZONEERR_CLAN298 963
/* @@error 963 已经超出了公会召唤的有效时间*/

#define ZONEERR_CLAN299 964
/* @@error 964 当前状态下无法传送*/

#define ZONEERR_CLAN300 965
/* @@error 965 因为等级限制,你无法传送到目标地图*/

#define ZONEERR_CLAN301 966
/* @@error 966 你不是会长，无法召唤*/

#define ZONEERR_CLAN302 967
/* @@error 967 Boss已召唤，无法再次召唤*/

#define ZONEERR_CLAN303 968
/* @@error 968 未到开启时间，无法召唤*/

#define ZONEERR_CLAN304 969
/* @@error 969 你已经领过奖励了*/

#define ZONEERR_CLAN305 970
/* @@error 970 Boss未死亡，无法领取*/

#define ZONEERR_CLAN306 971
/* @@error 971 公会产品[%s]的制造已被%s加速了%d秒*/

#define ZONEERR_CLAN307 972
/* @@error 972 公会科技[%s]的研发已被%s加速了%d秒*/

#define ZONEERR_CLAN308 973
/* @@error 973 公会资金不足以完成加速, 还差%d */

#define ZONEERR_CLAN309 974
/* @@error 974 研发即将完成, 无需加速 */

#define ZONEERR_CLAN310 975
/* @@error 975 公会等级已达Lv.%d，无法再提升*/

#define ZONEERR_CLAN311 976
/* @@error 976 需要城市等级达到Lv.%d, 才能提升*/

#define ZONEERR_CLAN312 977
/* @@error 977 公会已升级到Lv%d */

#define ZONEERR_CLAN313 978
/* @@error 978 当前公会等级下(Lv.%d), 公会城市等级不能超过%d */

#define ZONEERR_CLAN314 979
/* @@error 979 当前公会等级下(Lv.%d), 还不能申请公会城市*/

#define ZONEERR_CLAN315 980
/* @@error 980 公会城市副本创建失败*/

#define ZONEERR_CLAN316 981
/* @@error 981 找不到指定的建筑，无法提升等级*/

#define ZONEERR_CLAN317 982
/* @@error 982 此建筑正在建造中... */

#define ZONEERR_CLAN318 983
/* @@error 983 当前公会等级下(Lv.%d), 此建筑等级不能超过%d */

#define ZONEERR_CLAN319 984
/* @@error 984 当前公会等级下(Lv.%d), 还不能建造此建筑 */

#define ZONEERR_CLAN320 985
/* @@error 985 此建筑不能再升级了*/

#define ZONEERR_CLAN321 986
/* @@error 986 你还没有加入一个公会*/

#define ZONEERR_CLAN322 987
/* @@error 987 你的公会现在还没有城市*/

#define ZONEERR_CLAN323 988
/* @@error 988 公会城市副本不存在*/

#define ZONEERR_CLAN324 989
/* @@error 989 进入公会城市副本失败*/

#define ZONEERR_CLAN331 990
/* @@error 990 还需要%d点安定度才可以%s */

#define ZONEERR_CLAN325 991
/* @@error 991 还需要%d公会资金才可以%s */

#define ZONEERR_CLAN326 992
/* @@error 992 还需要%d已消耗科技点才可以%s */

#define ZONEERR_CLAN327 993
/* @@error 993 公会物品[%s]不足, 无法%s */

#define ZONEERR_CLAN328 994
/* @@error 994 公会物品不足, 无法%s */

#define ZONEERR_CLAN329 995
/* @@error 995 建筑%s已不能在升级了*/

#define ZONEERR_CLAN330 996
/* @@error 996 您的公会需要再投入%d%s科技点，才可升级%s */

#define ZONEERR_CLAN332 997
/* @@error 997 您的公会需要再投入%d%s科技点，才可升级%s */

#define ZONEERR_CLAN333 998
/* @@error 998 至少要学习一项%s科技才能建造%s */

#define ZONEERR_CLAN334 999
/* @@error 999 公会城市等级至少要达到Lv.%d, 才能升级%s */

#define ZONEERR_CLAN335 1000
/* @@error 1000 公会城市等级至少要达到Lv.%d, 才能建造%s */

#define ZONEERR_DESIGNATION101 1001
/* @@error 1001 你获得了一个称号[%s] */

#define ZONEERR_DESIGNATION102 1002
/* @@error 1002 你不能拥有更多的称号了 */

#define ZONEERR_DESIGNATION103 1003
/* @@error 1003 你已经持有此称号了 */

#define ZONEERR_DESIGNATION104 1004
/* @@error 1004 这个称号不存在 */

#define ZONEERR_DESIGNATION105 1005
/* @@error 1005 称号[%s] 已过有效期,系统自动删除*/

#define ZONEERR_DESIGNATION106 1006
/* @@error 1006 你还没有这个称号,无法使用*/

#define ZONEERR_DESIGNATION107 1007
/* @@error 1007 你的精力值不足,无法使用这个称号*/

#define ZONEERR_MAP101 1008
/* @@error 1008 你不是公会成员*/

#define ZONEERR_MAP102 1009
/* @@error 1009 成员 %s 不是公会成员*/

#define ZONEERR_MAP103 1010
/* @@error 1010 公会贡献度不够*/

#define ZONEERR_MAP104 1011
/* @@error 1011 成员 %s 公会贡献度不够*/

#define ZONEERR_MAP105 1012
/* @@error 1012 多人骑乘状态，不可进入此副本*/

#define ZONEERR_MAP106 1013
/* @@error 1013 此副本不能重复进入*/

#define ZONEERR_MAP107 1014
/* @@error 1014 此副本要求同一公会*/

#define ZONEERR_MAP108 1015
/* @@error 1015 加入公会当天无法参加此活动*/

#define ZONEERR_MAP109 1016
/* @@error 1016 副本人数已达上限*/

#define ZONEERR_MAP110 1017
/* @@error 1017 活动已关闭*/

#define ZONEERR_MAP111 1018
/* @@error 1018 活动已经开始，无法进行活动报名，请下次准时参加*/

#define ZONEERR_MAP112 1019
/* @@error 1019 参加副本的人数过多，请稍后再试*/

#define ZONEERR_MAP113 1020
/* @@error 1020 此副本要求组队*/

#define ZONEERR_MAP114 1021
/* @@error 1021 你不是队长*/

#define ZONEERR_MAP115 1022
/* @@error 1022 副本未开启，无需重置*/

#define ZONEERR_MAP116 1023
/* @@error 1023 副本中还有人，重置失败*/

#define ZONEERR_MAP117 1024
/* @@error 1024 副本重置成功*/

#define ZONEERR_MAP118 1025
/* @@error 1025 背包已满,物品通过邮件发送,注意查收*/

#define ZONEERR_MAP119 1026
/* @@error 1026 背包已满*/

#define ZONEERR_MAP120 1027
/* @@error 1027 请复活后再进入*/

#define ZONEERR_MAP121 1028
/* @@error 1028 交易状态不可以进入战场*/

#define ZONEERR_MAP122 1029
/* @@error 1029 摆摊状态不可以进入战场*/

#define ZONEERR_MAP123 1030
/* @@error 1030 摆摊状态不可以切换地图*/

#define ZONEERR_MAP124 1031
/* @@error 1031 战斗状态不可以切换地图*/

#define ZONEERR_MAP125 1032
/* @@error 1032 吟唱状态不可以切换地图*/

#define ZONEERR_MAP126 1033
/* @@error 1033 死亡状态下不可以切换地图*/

#define ZONEERR_MAP127 1034
/* @@error 1034 自动飞行状态下不可以切换地图*/

#define ZONEERR_MAP128 1035
/* @@error 1035 交易状态下不可以切换地图*/

#define ZONEERR_MAP129 1036
/* @@error 1036 活力值不足,无法创建副本*/

#define ZONEERR_MAP130 1037
/* @@error 1037 副本 %s 完成次数已达到每天完成的次数上限*/

#define ZONEERR_MAP131 1038
/* @@error 1038 副本 %s 完成次数已达到每周完成的次数上限*/

#define ZONEERR_MAP132 1039
/* @@error 1039 此副本%d分钟内只能进一次*/

#define ZONEERR_MAP133 1040
/* @@error 1040 成员 %s 此副本%d分钟内只能进一次*/

#define ZONEERR_MAP134 1041
/* @@error 1041 思过厅中的黑名玩家不能进入副本*/

#define ZONEERR_MAP135 1042
/* @@error 1042 %s  是思过厅中黑名玩家,不能进入副本*/

#define ZONEERR_MAP136 1043
/* @@error 1043 成员 %s 每日可完成次数已达上限*/

#define ZONEERR_MAP137 1044
/* @@error 1044 成员 %s 每周可完成次数已达上限*/

#define ZONEERR_MAP138 1045
/* @@error 1045 此副本要求等级%d-%d, 你的等级过低,不能进入此副本*/

#define ZONEERR_MAP139 1046
/* @@error 1046 成员 %s 等级过低, 此副本要求等级%d-%d,不能进入此副本*/

#define ZONEERR_MAP140 1047
/* @@error 1047 此副本要求等级%d-%d, 你的等级过高,不能进入此副本*/

#define ZONEERR_MAP141 1048
/* @@error 1048 成员 %s 等级过高, 此副本要求等级%d-%d,不能进入此副本*/

#define ZONEERR_MAP142 1049
/* @@error 1049 %s不在线*/

#define ZONEERR_MAP143 1050
/* @@error 1050 此副本要求等级%d-%d, %s等级不符*/

#define ZONEERR_MAP144 1051
/* @@error 1051 %s已达到 %s副本每天完成的次数上限*/

#define ZONEERR_MAP145 1052
/* @@error 1052 %s 与你不是同一个公会，此副本要求同一公会*/

#define ZONEERR_MAP146 1053
/* @@error 1053 %s公会贡献度不够*/

#define ZONEERR_MAP147 1054
/* @@error 1054 %s 和你不在同一地图中*/

#define ZONEERR_MAP148 1055
/* @@error 1055 %s不在视野中*/

#define ZONEERR_MAP149 1056
/* @@error 1056 副本创建失败，无法进入，只有队长才可创建副本*/

#define ZONEERR_MAP150 1057
/* @@error 1057 进入此地图%d分钟后会传送到时间城*/

#define ZONEERR_MAP151 1058
/* @@error 1058 %s不符合进入地图限制*/

#define ZONEERR_MAP152 1059
/* @@error 1059 地图人数过多，请稍后进入*/

#define ZONEERR_MAP153 1060
/* @@error 1060 本场景不能召唤宠物*/

#define ZONEERR_MAP154 1061
/* @@error 1061 本场景不能使用精灵,退出场景将自动还原*/

#define ZONEERR_MAP155 1062
/* @@error 1062 战斗状态无法传送*/

#define ZONEERR_MAP156 1063
/* @@error 1063 没有进入副本需要的道具*/

#define ZONEERR_MAP157 1064
/* @@error 1064 你不在副本中*/

#define ZONEERR_NPC101 1065
/* @@error 1065 购买本物品需要对应公会科技*/

#define ZONEERR_NPC102 1066
/* @@error 1066 只有时间城占领公会成员可以购买此物品*/

#define ZONEERR_NPC103 1067
/* @@error 1067 只有连续占领了%d次时间城的公会成员才可以购买此物品*/

#define ZONEERR_NPC104 1068
/* @@error 1068 购买本物品需要%s声望达到%s */

#define ZONEERR_NPC105 1069
/* @@error 1069 需要%d  %s，数量不足*/

#define ZONEERR_NPC106 1070
/* @@error 1070 没有相应的兑换道具*/

#define ZONEERR_NPC107 1071
/* @@error 1071 %s不足*/

#define ZONEERR_NPC108 1072
/* @@error 1072 要回购的物品已过期*/

#define ZONEERR_NPC109 1073
/* @@error 1073 此物品不能出售*/

#define ZONEERR_NPC110 1074
/* @@error 1074 货币数量达到上限,此物品不能出售*/

#define ZONEERR_NPC111 1075
/* @@error 1075 这件装备崭新如初，不需要修理*/

#define ZONEERR_NPC112 1076
/* @@error 1076 修理完成,有装备被封印,可找时间城解封大师解封*/

#define ZONEERR_NPC113 1077
/* @@error 1077 您身上的装备耐久度已满，暂时不需要维修*/

#define ZONEERR_NPC114 1078
/* @@error 1078 公会贡献度不足*/

#define ZONEERR_NPC115 1079
/* @@error 1079 该装备不可修理*/

#define ZONEERR_PET101 1080
/* @@error 1080 您的宠物忠诚度过低，请驯服宠物*/

#define ZONEERR_PET102 1081
/* @@error 1081 %s 至少达到%d级才可以出战*/

#define ZONEERR_PET103 1082
/* @@error 1082 出征宠物已到达上限*/

#define ZONEERR_PET104 1083
/* @@error 1083 乘坐机械，宠物不可出战*/

#define ZONEERR_PET105 1084
/* @@error 1084 当前副本宠物不可出战*/

#define ZONEERR_PET106 1085
/* @@error 1085 当前地图宠物不可出战*/

#define ZONEERR_PET107 1086
/* @@error 1086 当前状态宠物无法出战*/

#define ZONEERR_PET108 1087
/* @@error 1087 %s 至少达到%d级才可以出战*/

#define ZONEERR_PET109 1088
/* @@error 1088 宠物%s高于你10级以上，不能出战*/

#define ZONEERR_PET110 1089
/* @@error 1089 您的宠物忠诚度不够，请驯服宠物*/

#define ZONEERR_PET111 1090
/* @@error 1090 出征宠物已到达上限*/

#define ZONEERR_PET112 1091
/* @@error 1091 宠物合体状态不能出征宠物*/

#define ZONEERR_PET113 1092
/* @@error 1092 请放入正确的材料*/

#define ZONEERR_PET114 1093
/* @@error 1093 背包已满,无法放生宠物*/

#define ZONEERR_PET115 1094
/* @@error 1094 您的宠物%s 忠诚度太低，无法出战*/

#define ZONEERR_PET116 1095
/* @@error 1095 您的宠物%s 越来越消极了*/

#define ZONEERR_PET117 1096
/* @@error 1096 您的宠物%s 非常不高兴*/

#define ZONEERR_PET118 1097
/* @@error 1097 您的宠物%s 闷闷不乐*/

#define ZONEERR_PET119 1098
/* @@error 1098 宠物20级才能使用技能书*/

#define ZONEERR_PET120 1099
/* @@error 1099 此技能无法学习*/

#define ZONEERR_PET121 1100
/* @@error 1100 宠物不能学习这个绝技*/

#define ZONEERR_PET122 1101
/* @@error 1101 您的宠物处于出征状态,不能进行技能升级操作*/

#define ZONEERR_PET123 1102
/* @@error 1102 该技能已达到等级上限*/

#define ZONEERR_PET124 1103
/* @@error 1103 缺少技能升级道具:%s */

#define ZONEERR_PET125 1104
/* @@error 1104 宠物命名含有敏感词或非法字符(只能数字英文或中文),请重新输入 */

#define ZONEERR_PET126 1105
/* @@error 1105 您的宠物处于出征状态，不能进行强化操作 */

#define ZONEERR_PET127 1106
/* @@error 1106 对方没有验证安全锁密码*/

#define ZONEERR_PET128 1107
/* @@error 1107 同服的玩家才能融合宠物*/

#define ZONEERR_PET129 1108
/* @@error 1108 对方正在退出游戏无法融合宠物*/

#define ZONEERR_PET130 1109
/* @@error 1109 对方在死亡状态下无法融合宠物*/

#define ZONEERR_PET131 1110
/* @@error 1110 距离太远,无法融合宠物*/

#define ZONEERR_PET132 1111
/* @@error 1111 你的宠物未出征,无法融合宠物*/

#define ZONEERR_PET133 1112
/* @@error 1112 你的宠物不可以进行融合*/

#define ZONEERR_PET134 1113
/* @@error 1113 你的宠物资质未达到优秀以上,无法融合*/

#define ZONEERR_PET135 1114
/* @@error 1114 你的宠物资质不是一代宠物.无法融合*/

#define ZONEERR_PET136 1115
/* @@error 1115 你的宠物等级小于50级,无法融合*/

#define ZONEERR_PET137 1116
/* @@error 1116 对方宠物未出征,无法融合宠物*/

#define ZONEERR_PET138 1117
/* @@error 1117 对方的宠物资质未达到优秀以上,无法融合*/

#define ZONEERR_PET139 1118
/* @@error 1118 对方的宠物资质不是一代宠物,无法融合*/

#define ZONEERR_PET140 1119
/* @@error 1119 对方的宠物等级小于50级,无法融合*/

#define ZONEERR_PET141 1120
/* @@error 1120 双方宠物不是同类宠物,无法融合*/

#define ZONEERR_PET142 1121
/* @@error 1121 你已在融合状态,无法融合宠物*/

#define ZONEERR_PET143 1122
/* @@error 1122 正在处理融合申请,无法融合宠物*/

#define ZONEERR_PET144 1123
/* @@error 1123 对方已在融合状态,无法融合宠物*/

#define ZONEERR_PET145 1124
/* @@error 1124 对方正在处理融合申请,无法融合宠物*/

#define ZONEERR_PET146 1125
/* @@error 1125 您的宠物处于出战状态，不能进行此操作*/

#define ZONEERR_PET147 1126
/* @@error 1126 强化等级已经达到最高*/

#define ZONEERR_PET148 1127
/* @@error 1127 请先强化已激活的星星*/

#define ZONEERR_NOITEM 1128
/* @@error 1128 缺少相应的道具*/

#define ZONEERR_PET149 1129
/* @@error 1129 您的宠物处于出征状态,不能进行技能重置操作*/

#define ZONEERR_PET150 1130
/* @@error 1130 技能格都被锁定,无法重置技能*/

#define ZONEERR_PET151 1131
/* @@error 1131 宠物栏已满*/

#define ZONEERR_PET152 1132
/* @@error 1132 您的宠物:%s已晋升为%s资质*/

#define ZONEERR_PET153 1133
/* @@error 1133 %s不足，不能进行幻化操作*/

#define ZONEERR_PET154 1134
/* @@error 1134 摆摊状态下不能进行幻化操作*/

#define ZONEERR_PET155 1134
/* @@error 1134 您的宠物处于出征状态，不能进行幻化操作*/

#define ZONEERR_PET156 1135
/* @@error 1135 二代宠物，无法幻化*/

#define ZONEERR_PET157 1136
/* @@error 1136 你获得了一个%s */

#define ZONEERR_PET158 1137
/* @@error 1137 当前状态不能使用技能 */

#define ZONEERR_PET159 1138
/* @@error 1138 宠物最低等级要求为%d级 */

#define ZONEERR_PET160 1139
/* @@error 1139 宠物忠诚度低于%d％不能合体 */

#define ZONEERR_PET161 1140
/* @@error 1140 宠物%s高于你10级以上，不能出战了*/

#define ZONEERR_PET162 1141
/* @@error 1141 货架上的宠物无法出战*/

#define ZONEERR_PET163 1142
/* @@error 1142 宠物%s已经学会%s，不能重复学习*/

#define ZONEERR_PET164 1143
/* @@error 1143 宠物%s忠诚度已满*/

#define ZONEERR_PET165 1144
/* @@error 1144 宠物%s忠诚度恢复了%d点*/

#define ZONEERR_PET166 1145
/* @@error 1145 摆摊状态不允许此操作*/

#define ZONEERR_PET167 1146
/* @@error 1146 死亡状态不允许此操作*/

#define ZONEERR_PET168 1147
/* @@error 1147 交易状态不允许此操作*/

#define ZONEERR_PET169 1148
/* @@error 1148 宠物[%s]学会了%s */

#define ZONEERR_PET170 1149
/* @@error 1149 银币不足，无法进行宠物技能升级*/

#define ZONEERR_PET171 1150
/* @@error 1150 宠物[%s]学会了%s%d级*/

#define ZONEERR_PET172 1151
/* @@error 1151 宠物[%s]%s技能等级下降了*/

#define ZONEERR_PET173 1152
/* @@error 1152 提升技能等级失败,技能等级不变*/

#define ZONEERR_PET174 1153
/* @@error 1153 强化材料不足,无法强化*/

#define ZONEERR_PET175 1154
/* @@error 1154 宠物[%s]的技能栏位获得了新的属性*/

#define ZONEERR_PET176 1155
/* @@error 1155 %s拒绝了你的宠物融合申请*/

#define ZONEERR_PET177 1156
/* @@error 1156 宠物融合取消*/

#define ZONEERR_PET178 1157
/* @@error 1157 你获得了一个%s */

#define ZONEERR_PET179 1158
/* @@error 1158 最多只能锁定%d个技能格*/

#define ZONEERR_PET180 1159
/* @@error 1159 此技能格已经被锁定*/

#define ZONEERR_PET181 1160
/* @@error 1160 技能格锁定成功*/

#define ZONEERR_PET182 1161
/* @@error 1161 您的宠物处于出战状态，不能进行技能栏改造操作*/

#define ZONEERR_PET183 1162
/* @@error 1162 你没有锁定技能格*/

#define ZONEERR_PET184 1163
/* @@error 1163 你没有锁定此技能格*/

#define ZONEERR_PET185 1164
/* @@error 1164 技能格解锁成功*/

#define ZONEERR_PET186 1165
/* @@error 1165 你的宠物:%s 领悟了%s 技能*/

#define ZONEERR_PET187 1166
/* @@error 1166 %s 不足，无法进行宠物技能重置*/

#define ZONEERR_PET188 1167
/* @@error 1167 缺少%s，无法进行宠物技能重置*/

#define ZONEERR_PET189 1168
/* @@error 1168 宠物技能重置成功*/

#define ZONEERR_PET190 1169
/* @@error 1169 宠物[%s]的等级提升到了%d级*/

#define ZONEERR_PET191 1170
/* @@error 1170 宠物[%s]获得了%d经验*/

#define ZONEERR_PET192 1171
/* @@error 1171 当前无出征宠物,无法进行宠物合体操作*/

#define ZONEERR_PET193 1172
/* @@error 1172 已经处于宠物合体状态*/

#define ZONEERR_PET194 1173
/* @@error 1173 角色最低等级要求为%d级*/

#define ZONEERR_PLAYER101 1174
/* @@error 1174 您累计在线时间已满%d小时*/

#define ZONEERR_PLAYER102 1175
/* @@error 1175 您已经进入疲劳游戏时间，您的游戏收益将降为正常值的50％，为了您的健康，请尽快下线休息，做适当身体活动，合理安排学习生活*/

#define ZONEERR_PLAYER103 1176
/* @@error 1176 您已进入不健康游戏时间，为了您的健康，请您立即下线休息。如不下线，您的身体将受到损害，您的收益已降为零，直到您的累计下线时间满5小时后，才能恢复正常*/

#define ZONEERR_PLAYER104 1177
/* @@error 1177 该角色还有未交易的发布单,请先撤销再删除角色*/

#define ZONEERR_PLAYER105 1178
/* @@error 1178 摆摊状态下不能领取奖励*/

#define ZONEERR_PLAYER106 1179
/* @@error 1179 交易状态下不能领取奖励*/

#define ZONEERR_PLAYER107 1180
/* @@error 1180 %s 爆满，您进入了%s，您可以通过NPC传送到其他新手村*/

#define ZONEERR_PLAYER108 1181
/* @@error 1181 你的帐号被永久禁言*/

#define ZONEERR_PLAYER109 1182
/* @@error 1182 你的帐号被禁言，还剩 %d 秒*/

#define ZONEERR_PLAYER110 1183
/* @@error 1183 你被永久禁言*/

#define ZONEERR_PLAYER111 1184
/* @@error 1184 你被禁言，还剩 %d 秒*/

#define ZONEERR_PLAYER112 1185
/* @@error 1185 聊天对象%s不在线、不存在或者属于其他服玩家*/

#define ZONEERR_PLAYER113 1186
/* @@error 1186 你密语的对象不存在或者不在线*/

#define ZONEERR_PLAYER114 1187
/* @@error 1187 你在 %s 黑名单中*/

#define ZONEERR_PLAYER115 1188
/* @@error 1188 聊天对象%s 不在线*/

#define ZONEERR_PLAYER116 1189
/* @@error 1189 今日经验值已达到上限*/

#define ZONEERR_PLAYER117 1190
/* @@error 1190 你的经验值已达到等级存储上限，请在角色界面手动升级*/

#define ZONEERR_PLAYER118 1191
/* @@error 1191 你的经验值已达到等级存储上限*/

#define ZONEERR_PLAYER119 1192
/* @@error 1192 由于您是未成年人，将被纳入防沉迷系统，3小时内收益经验正常，3-5小时内收益经验减半，5小时后收益经验为零*/

#define ZONEERR_NOMORE_MONEY1 1193
/* @@error 1193 无法获得更多的绑银*/

#define ZONEERR_NOMORE_MONEY2 1194
/* @@error 1194 无法获得更多的金券*/

#define ZONEERR_PLAYER120 1195
/* @@error 1195 摆摊状态下无法飞行*/

#define ZONEERR_PLAYER121 1196
/* @@error 1196 死亡状态下无法飞行*/

#define ZONEERR_PLAYER122 1197
/* @@error 1197 交易状态下无法飞行*/

#define ZONEERR_PLAYER123 1198
/* @@error 1198 当前状态下无法飞行*/

#define ZONEERR_PLAYER124 1199
/* @@error 1199 骑乘机械状态下无法飞行*/

#define ZONEERR_PLAYER125 1200
/* @@error 1200 跨服状态或战场报名状态不能进行改名操作*/

#define ZONEERR_PLAYER126 1201
/* @@error 1201 公会名没有被系统修改过*/

#define ZONEERR_PLAYER127 1202
/* @@error 1202 你无权修改公会名*/

#define ZONEERR_PLAYER128 1203
/* @@error 1203 公会名相同,不需要修改*/

#define ZONEERR_PLAYER129 1204
/* @@error 1204 公会名已被使用*/

#define ZONEERR_PLAYER130 1205
/* @@error 1205 角色名相同,不需要修改*/

#define ZONEERR_PLAYER131 1206
/* @@error 1206 角色名修改必须间隔7天*/

#define ZONEERR_PLAYER132 1207
/* @@error 1207 当前状态不能跳跃*/

#define ZONEERR_PLAYER133 1208
/* @@error 1208 角色%s不在线，不能观察对方*/

#define ZONEERR_PLAYER134 1209
/* @@error 1209 跟随人数达到上限*/

#define ZONEERR_PLAYER135 1210
/* @@error 1210 您或跟随目标处于不能跟随位置，跟随失败*/

#define ZONEERR_PLAYER136 1211
/* @@error 1211 不同线路，不能跟随*/

#define ZONEERR_DIRTY1 1212
/* @@error 1212 输入含有敏感词或非法字符(只能数字英文或中文),请重新输入*/

#define ZONEERR_PLAYER137 1213
/* @@error 1213 在本服不能跟随其他服的人*/

#define ZONEERR_PLAYER138 1214
/* @@error 1214 目标无法跟随*/

#define ZONEERR_PLAYER139 1215
/* @@error 1215 你正在跟随!*/

#define ZONEERR_PLAYER140 1216
/* @@error 1216 战场不允许领礼物*/

#define ZONEERR_PLAYER141 1217
/* @@error 1217 没有转职相关的物品*/

#define ZONEERR_PLAYER142 1218
/* @@error 1218 你已经转职了*/

#define ZONEERR_PLAYER143 1219
/* @@error 1219 等级不够,不能转职*/

#define ZONEERR_PLAYER144 1220
/* @@error 1220 经验不足,不能转职*/

#define ZONEERR_PLAYER145 1221
/* @@error 1221 转职失败*/

#define ZONEERR_PLAYER146 1222
/* @@error 1222 没有对应的奖励*/

#define ZONEERR_PLAYER147 1223
/* @@error 1223 %s数量达到上限了*/

#define ZONEERR_PLAYER148 1224
/* @@error 1224 媒体次数礼包不存在*/

#define ZONEERR_PLAYER149 1225
/* @@error 1225 此道具已经使用过%d次了，不能再用*/

#define ZONEERR_PLAYER150 1226
/* @@error 1226 注册帐号成功*/

#define ZONEERR_PLAYER151 1227
/* @@error 1227 身份信息填补成功*/

#define ZONEERR_PLAYER152 1228
/* @@error 1228 身份信息填补失败*/

#define ZONEERR_PLAYER153 1229
/* @@error 1229 使用了一个%s */

#define ZONEERR_PLAYER154 1230
/* @@error 1230 你的杀气值过高，将有可能受到巡逻卫兵的攻击*/

#define ZONEERR_PLAYER155 1231
/* @@error 1231 杀气值增加%d，目前杀气值为%d点*/

#define ZONEERR_PLAYER156 1232
/* @@error 1232 你还不能领取此奖励*/

#define ZONEERR_PLAYER157 1233
/* @@error 1233 你已经领取过此奖励*/

#define ZONEERR_PLAYER158 1234
/* @@error 1234 无效的阶段奖励*/

#define ZONEERR_PLAYER159 1235
/* @@error 1235 %s不可以手动升级*/

#define ZONEERR_PLAYER160 1236
/* @@error 1236 需要转职才能升级*/

#define ZONEERR_PLAYER161 1237
/* @@error 1237 升级需要天赋值达到%d点*/

#define ZONEERR_PLAYER162 1238
/* @@error 1238 等级正在封印中，距离解除封印还有%d小时*/

#define ZONEERR_PLAYER163 1239
/* @@error 1239 跨服状态或战场报名状态不能进行改名操作*/

#define ZONEERR_PLAYER164 1240
/* @@error 1240 不能跟随自己*/

#define ZONEERR_PLAYER165 1241
/* @@error 1241 目标无法跟随*/

#define ZONEERR_PLAYER166 1242
/* @@error 1242 你不在副本中*/

#define ZONEERR_PLAYER167 1243
/* @@error 1243 你被%s  杀死了*/

#define ZONEERR_PLAYER168 1244
/* @@error 1244 你杀死了 %s */

#define ZONEERR_PLAYER169 1245
/* @@error 1245 等级不够*/

#define ZONEERR_PLAYER170 1246
/* @@error 1246 预创建奖励你已经领取过了*/

#define ZONEERR_PLAYER171 1247
/* @@error 1247 %d级的奖励你已经领取过了*/

#define ZONEERR_PLAYER172 1248
/* @@error 1248 今天已经领取过本次奖励*/

#define ZONEERR_PLAYER173 1249
/* @@error 1249 今天已经领取过本组奖励*/

#define ZONEERR_PLAYER174 1250
/* @@error 1250 最小等级要求为%d级*/

#define ZONEERR_PLAYER175 1251
/* @@error 1251 最大等级要求为%d级*/

#define ZONEERR_PLAYER176 1252
/* @@error 1252 今天在线时间小于%d分钟*/

#define ZONEERR_PLAYER177 1253
/* @@error 1253 只能领取公会等级对应的福利*/

#define ZONEERR_PLAYER178 1254
/* @@error 1254 等级为1级并且维护资金不足的公会无法领取福利*/

#define ZONEERR_PLAYER179 1255
/* @@error 1255 您的公会昨天未开启活动,或参加活动的成员太少*/

#define ZONEERR_PLAYER180 1256
/* @@error 1256 需要等级达到%d才能领取此福利*/

#define ZONEERR_PLAYER181 1257
/* @@error 1257 需要公会贡献度达到%d才能领取此福利*/

#define ZONEERR_PLAYER182 1258
/* @@error 1258 需要公会城市等级达到%d才能领取此福利*/

#define ZONEERR_NOMORE_CONTRI 1259
/* @@error 1259 你的个人贡献度已达上限，不能再增加了*/

#define ZONEERR_PLAYER183 1260
/* @@error 1260 今天连续在线时间小于%d分钟*/

#define ZONEERR_PLAYER184 1261
/* @@error 1261 今天连续在线时间小于%d分钟*/

#define ZONEERR_PLAYER185 1262
/* @@error 1262 只能在有效时间内领取奖励*/

#define ZONEERR_PLAYER186 1263
/* @@error 1263 只有在排行榜上才能领取奖励*/

#define ZONEERR_PLAYER187 1264
/* @@error 1264 已经领取过奖励*/

#define ZONEERR_PLAYER188 1265
/* @@error 1265 没有排名对应的奖励*/

#define ZONEERR_PLAYER189 1266
/* @@error 1266 背包中缺少GPS定位仪道具*/

#define ZONEERR_PLAYER190 1267
/* @@error 1267 GPS定位仪超过时效*/

#define ZONEERR_PLAYER191 1268
/* @@error 1268 你已经在变身状态下,操作失败*/

#define ZONEERR_PLAYER192 1269
/* @@error 1269 你在摆摊状态下,操作失败*/

#define ZONEERR_PLAYER193 1270
/* @@error 1270 你在自动飞行状态下,操作失败*/

#define ZONEERR_PLAYER194 1271
/* @@error 1271 你在骑乘状态下,操作失败*/

#define ZONEERR_PLAYER195 1272
/* @@error 1272 欢迎来到沙滩，在这里你会获得充足的日光浴经验*/

#define ZONEERR_PLAYER196 1273
/* @@error 1273 您今天还剩余%d分钟的日光浴充足经验时间*/

#define ZONEERR_PLAYER196 1273
/* @@error 1273 您今天还剩余%d分钟的日光浴充足经验时间*/

#define ZONEERR_PLAYER197 1274
/* @@error 1274 您今天的日光浴充足经验时间已用完*/

#define ZONEERR_PLAYER198 1275
/* @@error 1275 你现在正在离开沙滩范围*/

#define ZONEERR_PLAYER199 1276
/* @@error 1276 战争即将开始，非时间城占领公会成员不可在此地图逗留*/

#define ZONEERR_PLAYER200 1277
/* @@error 1277 战斗模式已切换为[公会模式] */

#define ZONEERR_PLAYER201 1278
/* @@error 1278 媒体礼包不存在 */

#define ZONEERR_PLAYER202 1279
/* @@error 1279 在N选1中，请选择一个道具 */

#define ZONEERR_PLAYER203 1280
/* @@error 1280 在当前地图无法使用 */

#define ZONEERR_PLAYER204 1281
/* @@error 1281 消耗了%d点公会贡献度 */

#define ZONEERR_PWORLD201 1282
/* @@error 1282 您已被改造为机械巨人，跟随首领消灭敌对阵营，赢取经验奖励*/

#define ZONEERR_PWORLD202 1283
/* @@error 1283 您已被改造为机械怪兽，跟随首领消灭敌对阵营，赢取经验奖励*/

#define ZONEERR_RANGE201 1284
/* @@error 1284 宠物已与原主人解除关系,无法查看*/

#define ZONEERR_RANGE202 1285
/* @@error 1285 对方是其他服的玩家,本服不允许操作*/

#define ZONEERR_RANGE203 1286
/* @@error 1286 公会不存在了*/

#define ZONEERR_RANGE204 1287
/* @@error 1287 目标无法观察*/

#define ZONEERR_RANGE205 1288
/* @@error 1288 目标跨服中无法观察*/

#define ZONEERR_ROLL201 1289
/* @@error 1289 跨服副本不可以切换分配模式*/

#define ZONEERR_ROLL202 1290
/* @@error 1290 只有队长可以切换分配模式*/

#define ZONEERR_RONGLU201 1291
/* @@error 1291 任务已经完成*/

#define ZONEERR_RONGLU202 1292
/* @@error 1292 缺少道具:%s */

#define ZONEERR_RONGLU203 1293
/* @@error 1293 熔炉已放置，不可重铸 */

#define ZONEERR_RONGLU204 1294
/* @@error 1294 当前没有可重铸的熔炉 */

#define ZONEERR_RONGLU205 1295
/* @@error 1295 你正在重铸熔炉,不允许此操作 */

#define ZONEERR_RONGLU206 1296
/* @@error 1296 当前已经是金熔炉,不允许此操作*/

#define ZONEERR_RUNE201 1297
/* @@error 1297 找不到指定的技能*/

#define ZONEERR_RUNE202 1298
/* @@error 1298 您已死亡，还无法这么做*/

#define ZONEERR_RUNE203 1299
/* @@error 1299 您需要达到%d级，才可执行该操作*/

#define ZONEERR_RUNE204 1300
/* @@error 1300 此技能符文尚未开放*/

#define ZONEERR_RUNE205 1301
/* @@error 1301 您的绑银不够, 共需要%d绑银*/

#define ZONEERR_RUNE206 1302
/* @@error 1302 符文格已开启，可往该技能上镶嵌符文了*/

#define ZONEERR_RUNE207 1303
/* @@error 1303 找不到指定的技能*/

#define ZONEERR_RUNE208 1304
/* @@error 1304 找不到指定的物品*/

#define ZONEERR_RUNE209 1305
/* @@error 1305 您需要达到%d级后，才可执行该操作*/

#define ZONEERR_RUNE210 1306
/* @@error 1306 该格子未开启，您还不能这么做*/

#define ZONEERR_RUNE211 1307
/* @@error 1307 该技能没有对应的符文属性，您不能这么做*/

#define ZONEERR_RUNE212 1308
/* @@error 1308 这件物品不是符文*/

#define ZONEERR_RUNE213 1309
/* @@error 1309 技能符文未被正确配置*/

#define ZONEERR_RUNE214 1310
/* @@error 1310 技能【%s】成功镶嵌了符文*/

#define ZONEERR_RUNE215 1311
/* @@error 1311 你选定的物品不是符文遗忘卷轴*/

#define ZONEERR_RUNE216 1312
/* @@error 1312 该格子没有符文，您还不能这么做*/

#define ZONEERR_RUNE217 1313
/* @@error 1313 需要符文升级道具才能升级*/

#define ZONEERR_RUNE218 1314
/* @@error 1314 符文升级所需要的道具数量不足*/

#define ZONEERR_RUNE219 1315
/* @@error 1315 请放入相同的符文*/

#define ZONEERR_RUNE220 1316
/* @@error 1316 只能升级符文道具*/

#define ZONEERR_RUNE221 1317
/* @@error 1317 %s 已经是最高等级符文*/

#define ZONEERR_RUNE222 1318
/* @@error 1318 未知的符文颜色*/

#define ZONEERR_RUNE223 1319
/* @@error 1319 符文无法升级*/

#define ZONEERR_RUNE224 1320
/* @@error 1320 战斗状态下无法切换符文技能*/

#define ZONEERR_RUNE225 1321
/* @@error 1321 符文切换还在冷却中,请稍候再试*/

#define ZONEERR_RUNE226 1322
/* @@error 1322 无效的符文类型*/

#define ZONEERR_RUNE227 1323
/* @@error 1323 还没镶嵌对应的符文*/

#define ZONEERR_FIGHT_OP 1324
/* @@error 1324 战斗状态下无法进行此操作*/

#define ZONEERR_RUNE228 1325
/* @@error 1325 对应颜色高级符文数量不足*/

#define ZONEERR_RUNE229 1326
/* @@error 1326 没有可以保存的符文额外属性*/

#define ZONEERR_RUNE230 1327
/* @@error 1327 需要物品%s */

#define ZONEERR_SAFE201 1328
/* @@error 1328 账号处于异常登录状态,%d分钟内不能进行此操作 */

#define ZONEERR_SAFE202 1329
/* @@error 1329 请先认证安全锁密码*/

#define ZONEERR_SAFE203 1330
/* @@error 1330 安全锁密码不能为空*/

#define ZONEERR_SAFE204 1331
/* @@error 1331 你已经设置过安全锁密码了*/

#define ZONEERR_SAFE205 1332
/* @@error 1332 安全锁激活,请牢记安全锁密码*/

#define ZONEERR_SAFE206 1333
/* @@error 1333 你没有设置安全锁密码*/

#define ZONEERR_SAFE207 1334
/* @@error 1334 安全锁已经认证过了*/

#define ZONEERR_SAFE208 1335
/* @@error 1335 安全锁认证成功*/

#define ZONEERR_SCRIPT201 1336
/* @@error 1336 精灵栏位达到上限*/

#define ZONEERR_SCRIPT202 1337
/* @@error 1337 同样的精灵只能拥有一个*/

#define ZONEERR_SKILL201 1338
/* @@error 1338 你拥有了技能：%s */

#define ZONEERR_SKILL202 1339
/* @@error 1339 此技能与你职业不符*/

#define ZONEERR_SKILL203 1340
/* @@error 1340 此技能需要最小等级为%d级*/

#define ZONEERR_SKILL204 1341
/* @@error 1341 此技能需要先学习%d级%s */

#define ZONEERR_SKILL205 1342
/* @@error 1342 此技能需要先完成%s任务*/

#define ZONEERR_SKILL206 1343
/* @@error 1343 学习此技能需要:%s */

#define ZONEERR_SKILL207 1344
/* @@error 1344 升级技能所需经验值不够*/

#define ZONEERR_SKILL208 1345
/* @@error 1345 该天赋和已学天赋互斥,不能学习该天赋*/

#define ZONEERR_SKILL209 1346
/* @@error 1346 天赋点不足*/

#define ZONEERR_SKILL210 1347
/* @@error 1347 前置已经使用的天赋点不足,不能学习该层天赋技能*/

#define ZONEERR_SKILL211 1348
/* @@error 1348 此处不能降落*/

#define ZONEERR_SKILL212 1349
/* @@error 1349 骑乘状态不能飞行*/

#define ZONEERR_SKILL213 1350
/* @@error 1350 当前经验不足,不能学习技能*/

#define ZONEERR_SPAN201 1351
/* @@error 1351 答题状态不可跨服操作*/

#define ZONEERR_SPAN202 1352
/* @@error 1352 副本传送状态下不可跨服*/

#define ZONEERR_SPAN203 1353
/* @@error 1353 报名失败，同一帐号只能有一个角色可处于跨服状态*/

#define ZONEERR_SPAN204 1354
/* @@error 1354 报名人数已满,请稍后重试*/

#define ZONEERR_SPAN205 1355
/* @@error 1355 报名失败，同一帐号只能有一个角色可处于跨服状态*/

#define ZONEERR_STATUS201 1356
/* @@error 1356 目标不受任何状态效果影响*/

#define ZONEERR_STATUS202 1357
/* @@error 1357 %s从你身上消失了*/

#define ZONEERR_STATUS203 1358
/* @@error 1358 你获得了%s */

#define ZONEERR_STATUS204 1359
/* @@error 1359 幸运女神降临，您获得了%s状态*/

#define ZONEERR_STRONG201 1360
/* @@error 1360 使用临时机械时无法传送*/

#define ZONEERR_STRONG202 1361
/* @@error 1361 骑乘状态下无法传送*/

#define ZONEERR_STRONG203 1362
/* @@error 1362 入会时间少于12小时,无法领取*/

#define ZONEERR_STRONG204 1363
/* @@error 1363 无法领取,只有占领公会成员才可进行此操作*/

#define ZONEERR_STRONG205 1364
/* @@error 1364 无法领取,占领据点后第二天才能领取*/

#define ZONEERR_STRONG206 1365
/* @@error 1365 权限不足无法领取,只有公会会长可以领取*/

#define ZONEERR_STRONG207 1366
/* @@error 1366 您的公会今日已经领取了此据点占领资金*/

#define ZONEERR_NOMORE_MONEY3 1367
/* @@error 1367 无法获得更多的银币*/

#define ZONEERR_STRONG208 1368
/* @@error 1368 您成功领取了%d银币的占领资金*/

#define ZONEERR_STRONG209 1369
/* @@error 1369 大地之心中不能获得此效果*/

#define ZONEERR_STRONG210 1370
/* @@error 1370 您今天已经领取过此据点的%s效果*/

#define ZONEERR_STRONG211 1371
/* @@error 1371 %s效果领取成功*/

#define ZONEERR_STRONG212 1372
/* @@error 1372 您今天已经领取过此据点公会贡献度*/

#define ZONEERR_STRONG213 1373
/* @@error 1373 竞拍失败,只有公会或副会长有权限参与竞拍*/

#define ZONEERR_STRONG214 1374
/* @@error 1374 竞拍失败,无法对已占领的据点进行竞拍*/

#define ZONEERR_STRONG215 1375
/* @@error 1375 竞拍失败,当前时间不能竞拍*/

#define ZONEERR_STRONG216 1376
/* @@error 1376 您的公会已经参加其他据点竞拍,竞拍失败*/

#define ZONEERR_STRONG217 1377
/* @@error 1377 竞拍提交成功,明日12：00后可在此处查询最终结果*/

#define ZONEERR_STRONG218 1378
/* @@error 1378  [%s]不能设置官衔名称*/

#define ZONEERR_STRONG219 1379
/* @@error 1379 据点战还未开启,无法进入*/

#define ZONEERR_STRONG220 1380
/* @@error 1380 今日已参加%s战,不可进入当前据点战场*/

#define ZONEERR_STRONG221 1381
/* @@error 1381 你没有公会*/

#define ZONEERR_STRONG222 1382
/* @@error 1382 您所在公会无权限进入当前据点战战场*/

#define ZONEERR_STRONG223 1383
/* @@error 1383 非可参战战团成员*/

#define ZONEERR_STRONG224 1384
/* @@error 1384 无法操作,只有占领公会成员才可进行此操作*/

#define ZONEERR_STRONG225 1385
/* @@error 1385 权限不足无法操作,只有会长或副会长有此权限*/

#define ZONEERR_STRONG226 1386
/* @@error 1386 公会资金不足,保存失败*/

#define ZONEERR_STRONG227 1387
/* @@error 1387 公会旗帜外形已更新*/

#define ZONEERR_STRONG228 1388
/* @@error 1388 科技等级不足,保存失败*/

#define ZONEERR_STRONG229 1389
/* @@error 1389 公会宣言已经保存成功*/

#define ZONEERR_STRONG230 1390
/* @@error 1390 公会宣言已经保存成功*/

#define ZONEERR_STRONG231 1391
/* @@error 1391 战斗状态下无法传送*/

#define ZONEERR_STRONG232 1392
/* @@error 1392 你在副本中无法传送*/

#define ZONEERR_NO_POWER_OP 1393
/* @@error 1393 权限不足，无法进行此操作*/

#define ZONEERR_STRONG233 1394
/* @@error 1394 设置成功*/

#define ZONEERR_STRONG234 1395
/* @@error 1395 只有公会或副会长有权限开启战旗荣耀活动*/

#define ZONEERR_STRONG235 1396
/* @@error 1396 占领据点后第二天才能开启战旗荣耀活动*/

#define ZONEERR_STRONG236 1397
/* @@error 1397 此据点今天已经开启过战旗荣耀活动*/

#define ZONEERR_STRONG237 1398
/* @@error 1398 %s 的公会旗帜正沐浴荣耀辉芒,暂时无法开启*/

#define ZONEERR_STRONG238 1399
/* @@error 1399 该时段的公会旗帜似乎无法感应英灵的祝福*/

#define ZONEERR_SVR201 1400
/* @@error 1400 你被禁言 %d 秒*/

#define ZONEERR_SVR202 1401
/* @@error 1401 你被永久禁言*/

#define ZONEERR_SVR203 1402
/* @@error 1402 你的禁言被解除了*/

#define ZONEERR_SVR204 1403
/* @@error 1403 你的帐号禁言被解除了*/

#define ZONEERR_SVR205 1404
/* @@error 1404 你的帐号被禁言 %d 秒*/

#define ZONEERR_SVR206 1405
/* @@error 1405 角色%s不在线，不能观察对方*/

#define ZONEERR_TASK201 1406
/* @@error 1406 未配对成功，无法接取此任务*/

#define ZONEERR_TASK202 1407
/* @@error 1407 接取了同类任务,不能领取任务*/

#define ZONEERR_TASK203 1408
/* @@error 1408 此类环组任务每天完成的次数已达到上限*/

#define ZONEERR_TASK204 1409
/* @@error 1409 此环组任务要求最小等级为%d级*/

#define ZONEERR_TASK205 1410
/* @@error 1410 此环组任务要求最大等级为%d级*/

#define ZONEERR_TASK206 1411
/* @@error 1411 此环组任务每天完成的次数已达到上限*/

#define ZONEERR_TASK207 1412
/* @@error 1412 %s达到上限，不能做此环组任务*/

#define ZONEERR_TASK208 1413
/* @@error 1413 背包里没有相关的任务道具*/

#define ZONEERR_TASK209 1414
/* @@error 1414 不可以放弃此任务*/

#define ZONEERR_TASK210 1415
/* @@error 1415 你已放弃%s任务*/

#define ZONEERR_TASK211 1416
/* @@error 1416 任务已失败，请在任务面板中放弃此任务*/

#define ZONEERR_TASK212 1417
/* @@error 1417 背包已满，奖励已通过邮件发送*/

#define ZONEERR_TASK213 1418
/* @@error 1418 完成本任务需要%s */

#define ZONEERR_TASK214 1419
/* @@error 1419 完成任务[%s] 今天第%d 次 */

#define ZONEERR_TASK215 1420
/* @@error 1420 完成了任务[%s] */

#define ZONEERR_TASK216 1421
/* @@error 1421 防沉迷状态下不可完成任务*/

#define ZONEERR_TASK217 1422
/* @@error 1422 背包无法放入任务奖励，请整理背包后再交任务*/

#define ZONEERR_TASK218 1423
/* @@error 1423 你已经完成任务%s, 任务奖励已自动获得*/

#define ZONEERR_TASK219 1424
/* @@error 1424 %s距离过远，无法共享任务*/

#define ZONEERR_TASK220 1425
/* @@error 1425 %s 现在忙，无法共享任务，请稍后再试*/

#define ZONEERR_TASK221 1426
/* @@error 1426 %s未达到共享任务的等级要求*/

#define ZONEERR_TASK222 1427
/* @@error 1427 %s已接取了该任务，无法共享任务*/

#define ZONEERR_TASK223 1428
/* @@error 1428 %s已达到今天可完成次数上限*/

#define ZONEERR_TASK224 1429
/* @@error 1429 %s非同公会成员，无法共享任务*/

#define ZONEERR_TASK225 1430
/* @@error 1430 %s未完成过该任务前置任务，无法共享任务*/

#define ZONEERR_TASK226 1431
/* @@error 1431 %s处于不可变身状态，无法接受此任务*/

#define ZONEERR_TASK227 1432
/* @@error 1432 %s已经完成过该任务，无法接受此任务*/

#define ZONEERR_TASK228 1433
/* @@error 1433 %s未达到该任务要求，无法共享任务*/

#define ZONEERR_TASK229 1434
/* @@error 1434 跨服组队状态中不可共享任务*/

#define ZONEERR_TASK230 1435
/* @@error 1435 战场中不可共享任务*/

#define ZONEERR_TASK231 1436
/* @@error 1436 这个任务不能共享*/

#define ZONEERR_TASK232 1437
/* @@error 1437 任务已失败，不能共享*/

#define ZONEERR_TASK233 1438
/* @@error 1438 任务已完成，不能共享*/

#define ZONEERR_TASK234 1439
/* @@error 1439 你们不在一个队伍中，不能共享任务*/

#define ZONEERR_TASK235 1440
/* @@error 1440 接取了同类任务,不能领取任务*/

#define ZONEERR_TASK236 1441
/* @@error 1441 存放任务道具的背包已满，请先整理*/

#define ZONEERR_TASK237 1442
/* @@error 1442 你身上任务奖励的货币达到上限，不能领取任务*/

#define ZONEERR_TASK238 1443
/* @@error 1443 该任务需要组队进行*/

#define ZONEERR_TASK239 1444
/* @@error 1444 您可能处于临时变身，机械等特殊状态下，无法变身，请稍后再试*/

#define ZONEERR_TASK240 1445
/* @@error 1445 已经有这个任务了,不能重复接取*/

#define ZONEERR_TASK241 1446
/* @@error 1446 可以接的任务已经达到上限*/

#define ZONEERR_TASK242 1447
/* @@error 1447 接任务所需货币不足*/

#define ZONEERR_TASK243 1448
/* @@error 1448 当前状态不能接取商运任务*/

#define ZONEERR_TASK244 1449
/* @@error 1449 接取此任务需要预留一个背包格*/

#define ZONEERR_TASK245 1450
/* @@error 1450 放弃任务%s内无法领取*/

#define ZONEERR_TASK246 1451
/* @@error 1451 接取了任务[%s] */

#define ZONEERR_TASK247 1452
/* @@error 1452 恭喜你触发了[%s] 的隐藏任务*/

#define ZONEERR_TASK248 1453
/* @@error 1453 你的公会资源中心增加了 %s x %d */

#define ZONEERR_TASK249 1454
/* @@error 1454 目前状态不能完成该任务，请先下车*/

#define ZONEERR_TASK250 1455
/* @@error 1455 %s不在线，无法共享任务*/

#define ZONEERR_TASK251 1456
/* @@error 1456 队员:%s ,无法接受共享的任务*/

#define ZONEERR_TASK252 1457
/* @@error 1457 队员:%s ,接受共享的任务成功*/

#define ZONEERR_TASK253 1458
/* @@error 1458 共享任务已失效*/

#define ZONEERR_TASK254 1459
/* @@error 1459 你接受了%s共享的任务[%s] */

#define ZONEERR_TASK255 1460
/* @@error 1460 %s接受了你共享的任务[%s] */

#define ZONEERR_TASK256 1461
/* @@error 1461 %s拒绝了你共享的任务 */

#define ZONEERR_UNREAL201 1462
/* @@error 1462 队友%s  不在线*/

#define ZONEERR_UNREAL202 1463
/* @@error 1463 你已经死亡,无法传送*/

#define ZONEERR_UNREAL203 1464
/* @@error 1464 队友%s  已经死亡,无法传送*/

#define ZONEERR_UNREAL204 1465
/* @@error 1465 队友%s  和你不在同一地图,无法传送*/

#define ZONEERR_UNREAL205 1466
/* @@error 1466 你正在摆摊,无法传送*/

#define ZONEERR_UNREAL206 1467
/* @@error 1467 队友%s  正在摆摊,无法传送*/

#define ZONEERR_UNREAL207 1468
/* @@error 1468 你正在交易,无法传送*/

#define ZONEERR_UNREAL208 1469
/* @@error 1469 队友%s  正在交易,无法传送*/

#define ZONEERR_UNREAL209 1470
/* @@error 1470 你正在切磋,无法传送*/

#define ZONEERR_UNREAL210 1471
/* @@error 1471 队友%s  正在切磋,无法传送*/

#define ZONEERR_UNREAL211 1472
/* @@error 1472 你正在思过厅中,无法传送*/

#define ZONEERR_UNREAL212 1473
/* @@error 1473 队友%s  正在思过厅中,无法传送*/

#define ZONEERR_UNREAL213 1474
/* @@error 1474 你正在骑乘货车状态中,无法传送*/

#define ZONEERR_UNREAL214 1475
/* @@error 1475 队友%s  正在骑乘货车状态中,无法传送*/

#define ZONEERR_UNREAL215 1476
/* @@error 1476 你没有队伍*/

#define ZONEERR_UNREAL216 1477
/* @@error 1477 你不是队长*/

#define ZONEERR_UNREAL217 1478
/* @@error 1478 队长不能低于20级*/

#define ZONEERR_UNREAL218 1479
/* @@error 1479 队伍少于%d人*/

#define ZONEERR_UNREAL219 1480
/* @@error 1480 队友%s  不在线*/

#define ZONEERR_UNREAL220 1481
/* @@error 1481 队伍有效人数少于%d人*/

#define ZONEERR_UNREAL221 1482
/* @@error 1482 你没有保存过%d层的记录*/

#define ZONEERR_UNREAL222 1483
/* @@error 1483 本层杀怪数小于%d％ */

#define ZONEERR_UNREAL223 1484
/* @@error 1484 该场景尚未开放,敬请期待*/

#define ZONEERR_UNREAL224 1485
/* @@error 1485 您已经到达%d层,可以在记忆之门处保存您的记录,下一次将可以带领队伍直接进入幻境%d层*/

#define ZONEERR_UNREAL225 1486
/* @@error 1486 你已经保存过该层的进度*/

#define ZONEERR_UNREAL226 1487
/* @@error 1487 保存成功*/

#define ZONEERR_UNREAL227 1488
/* @@error 1488 目前没有幻境队伍*/

#define ZONEERR_UNREAL228 1489
/* @@error 1489 队长不在线*/

#define ZONEERR_UNREAL229 1490
/* @@error 1490 队长不在幻境中*/

#define ZONEERR_UNREAL230 1491
/* @@error 1491 你是队长，无法传入*/

#define ZONEERR_UNREAL231 1492
/* @@error 1492 进入本层Boss地图需要队长等级达到%d级*/

#define ZONEERR_MUD101 2000
/* @@error 2000 %s添加你为好友*/

#define ZONEERR_MUD102 2001
/* @@error 2001 %s 拒绝加你为好友*/

#define ZONEERR_MUD103 2002
/* @@error 2002 %s 拒绝了你的好友申请*/

#define ZONEERR_MUD104 2003
/* @@error 2003 聊天对象%s 不在线或不存在*/


#define ZONEERR_TEAM301 2100
/* @@error 2100 进入%s被取消*/

#define ZONEERR_TEAM302 2101
/* @@error 2101 %s同意开启%s */

#define ZONEERR_TEAM303 2102
/* @@error 2102 %s拒绝开启%s */

#define ZONEERR_TEAM304 2103
/* @@error 2103 传送失败,%s 已离线*/

#define ZONEERR_TEAM305 2104
/* @@error 2104 %s 处于不可传送状态*/

#define ZONEERR_TEAM306 2105
/* @@error 2105 队长活力值不足，无法完成创建*/

#define ZONEERR_TEAM307 2106
/* @@error 2106 %s此任务每天完成的次数已达到上限*/

#define ZONEERR_TEAM308 2107
/* @@error 2107 %s的等级不符合任务要求*/

#define ZONEERR_TEAM309 2108
/* @@error 2108 %s接取了同类任务,不能领取此任务*/

#define ZONEERR_TEAM310 2109
/* @@error 2109 %s存放任务道具的背包已满，请先整理*/

#define ZONEERR_TEAM311 2110
/* @@error 2110 %s身上任务奖励的货币达到上限，不能领取任务*/

#define ZONEERR_TEAM312 2111
/* @@error 2111 %s可能处于临时变身，机械等特殊状态下，无法变身，请稍后再试*/

#define ZONEERR_TEAM313 2112
/* @@error 2112 %s已经有这个任务了,不能重复接取*/

#define ZONEERR_TEAM314 2113
/* @@error 2113 %s可以接的任务已经达到上限*/

#define ZONEERR_TEAM315 2114
/* @@error 2114 %s接任务所需货币不足*/

#define ZONEERR_TEAM316 2115
/* @@error 2115 队伍人数增加击杀怪物经验提升*/

#define ZONEERR_TEAM317 2116
/* @@error 2116 恭喜玩家%s及他的队员在%s成功击杀到第%d 阶的熔岩巨兽，他和他的队员们获得了大量的经验奖励*/

#define ZONEERR_TEAM318 2117
/* @@error 2117 %s此任务每周完成的次数已达到上限*/

#define ZONEERR_TEAM319 2118
/* @@error 2118 队伍中有成员已完成第九层迷之塔进度,不可再次进入迷之塔*/

#define ZONEERR_BROAD301 2200
/* @@error 2200 玩家%s好运连连，从%s中开到了%sx%d */

#define ZONEERR_BROAD302 2201
/* @@error 2201 %s */

#define ZONEERR_BROAD303 2202
/* @@error 2202 鸣沙陷地据点被占领，时间城争霸战将于下周（第二周的周六20：00）正式开启 */

#define ZONEERR_BROAD304 2203
/* @@error 2203 战争已结束，[%s]公会获得时间城的控制权 */

#define ZONEERR_BROAD305 2204
/* @@error 2204 战争提前结束，[%s]公会提前获得时间城控制权*/

#define ZONEERR_BROAD306 2205
/* @@error 2205 战争已结束，很遗憾，没有任何公会获得时间城控制权！大家再接再厉*/

#define ZONEERR_BROAD307 2206
/* @@error 2206 战争已结束，[%s]公会依然拥有时间城控制权*/

#define ZONEERR_BROAD308 2207
/* @@error 2207 时间城城主争霸战现在正式开启！前往中控中心参加战争*/

#define ZONEERR_BROAD309 2208
/* @@error 2208 尊敬的时间城城主[%s]已经上线*/

#define ZONEERR_BROAD310 2209
/* @@error 2209 尊敬的时间城城主[%s]已经下线*/

#define ZONEERR_BROAD311 2210
/* @@error 2210 %s城主魅力无限，引来众多朝拜者*/

#define ZONEERR_BROAD312 2211
/* @@error 2211 %s城主魅力倍增，朝拜人数激增*/

#define ZONEERR_BROAD313 2212
/* @@error 2212 %s城主人气爆棚，朝拜香火不断*/

#define ZONEERR_BROAD314 2213
/* @@error 2213 %s城主的小手一挥，引粉丝无数*/

#define ZONEERR_BROAD315 2214
/* @@error 2214 %s城主的粉丝已经遍布大江南北*/

#define ZONEERR_BROAD316 2215
/* @@error 2215 %s城主的崇拜者可以排满整个时间城！*/

#define ZONEERR_BROAD317 2216
/* @@error 2216 %s城主的魅力已无人能挡，崇拜者遍布宇宙*/

#define ZONEERR_BROAD318 2217
/* @@error 2217 %s已达到人见人爱，花见花开，车在车爆胎的境界*/

#define ZONEERR_BROAD319 2218
/* @@error 2218 %s城主遭到了众人的唾弃，雕像被扔满了臭鸡蛋*/

#define ZONEERR_BROAD320 2219
/* @@error 2219 %s城主的雕像布满了臭鸡蛋，以无法分辨形象*/

#define ZONEERR_BROAD321 2220
/* @@error 2220 %s城主的雕像现在仅略具人形，已惨不忍睹*/

#define ZONEERR_BROAD322 2221
/* @@error 2221 %s城主的雕像已经被臭鸡蛋淹没，速速围观*/

#define ZONEERR_BROAD323 2222
/* @@error 2222 %s城主已经混到了人见人踹，花开花败，死了都没人爱的境界*/

#define ZONEERR_BROAD324 2223
/* @@error 2223 团结就是力量，玩家[%s]成立了强大的[%s]公会*/

#define ZONEERR_BROAD325 2224
/* @@error 2224 %s公会成功占领%s，全球资源严重不足*/

#define ZONEERR_BROAD326 2225
/* @@error 2225 %s公会成功占领%s，全球资源严重不足*/

#define ZONEERR_BROAD327 2226
/* @@error 2226 %s公会正在建造%s，采集场将在10分钟后建造完成*/

#define ZONEERR_BROAD328 2227
/* @@error 2227 %s的[#L%s@@(%d,%d,%d)#n] 即将损坏，采集场内收集的各种道具都将散落在地上*/

#define ZONEERR_BROAD330 2229
/* @@error 2229 %s经过不懈的努力，将%s升至%d级，他将享受到风一般的速度*/

#define ZONEERR_BROAD331 2230
/* @@error 2230 [%s]的宠物[%s]完成了一次完美强化，真是鸿运当头，羡煞旁人*/

#define ZONEERR_BROAD332 2231
/* @@error 2231 %s 的宠物%s 已晋升为%s资质*/

#define ZONEERR_BROAD333 2232
/* @@error 2232 %s彻底激怒了%s，被一击轰出了十万八千里*/

#define ZONEERR_BROAD334 2233
/* @@error 2233 暴怒下的%s已无人可挡，倒在他脚下的强者已不计其数*/

#define ZONEERR_BROAD335 2234
/* @@error 2234 %s进入了他的杀戮时间，死在其爪下的亡魂已达到100个*/

#define ZONEERR_BROAD336 2235
/* @@error 2235 杀戮机器!，%s一击将%s轰到尸骨无存，成为第200个不幸者*/

#define ZONEERR_BROAD337 2236
/* @@error 2236 一爪破山河，一枪灭星球!，在%s面前一切都是浮云，击杀300个人，小菜一碟*/

#define ZONEERR_BROAD338 2237
/* @@error 2237 无敌杀神!，唯一能形容%s的词，500个亡魂造就了它*/

#define ZONEERR_BROAD340 2239
/* @@error 2239 %s玩家在%s获得了幸运状态，大家快去抢啊*/

#define ZONEERR_BROAD341 2240
/* @@error 2240 强大的%s突破了大地之心的第%d层，向更深的地心进军*/

#define ZONEERR_BROAD342 2241
/* @@error 2241 疾如厉风,快似闪电!%s 以%d分%d.%d秒刷新了大地之心第%d层的记录*/

#define ZONEERR_BROAD343 2242
/* @@error 2242 天降横福活动结束了*/

#define ZONEERR_BROAD344 2243
/* @@error 2243 %s地图发现幸运怪%s，击杀后会有意想不到的收获呦。请速前往*/

#define ZONEERR_BROAD345 2244
/* @@error 2244 重要公告：%s已激活,公会可争夺占领*/

#define ZONEERR_BROAD346 2245
/* @@error 2245 %s公会旗帜已褪去辉芒,战旗荣耀活动暂告一段落*/

#define ZONEERR_BROAD347 2246
/* @@error 2246 [时间城城主] %s  已将时间城变为%s */

#define ZONEERR_BROADMAP301 2300
/* @@error 2300 [%s]公会以迅雷之势成功占领了%s */

#define ZONEERR_BROADMAP302 2301
/* @@error 2301 [%s]勇猛过人，已经将%s防护罩击毁 */

#define ZONEERR_BROADMAP303 2302
/* @@error 2302 [%s]以无人能挡的势头成功击毁了%s建筑 */

#define ZONEERR_BROADMAP304 2303
/* @@error 2303 [%s]公会以强大的力量成功占领了%s */

#define ZONEERR_BROADMAP305 2304
/* @@error 2304 %s 如撒旦在世,惊人的%d连杀*/

#define ZONEERR_BROADMAP306 2305
/* @@error 2305 %s 如恶魔一般,完成了%d连杀*/

#define ZONEERR_BROADMAP307 2306
/* @@error 2306 %s 已超神,完成了%d连杀*/

#define ZONEERR_BROADMAP308 2307
/* @@error 2307 %s 已无人能挡,完成了%d连杀*/

#define ZONEERR_BROADMAP309 2308
/* @@error 2308 %s  如神一般存在,完成了%d连杀*/

#define ZONEERR_BROADMAP310 2309
/* @@error 2309 %s 手起刀落轻松的完成了%d连杀*/

#define ZONEERR_BROADMAP311 2310
/* @@error 2310 [%s]勇猛过人，竟然击杀了%s公会 %s：%s！*/

#define ZONEERR_BROADMAP312 2311
/* @@error 2311 %s 成功终结了%s 的%d连杀! */

#define ZONEERR_BROADMAP313 2312
/* @@error 2312 一名玩家被异化成为机械巨人首领，将带领机械巨人主宰世界 */

#define ZONEERR_BROADMAP314 2313
/* @@error 2313 一名玩家被异化成为了机械怪兽首领，将带领机械怪兽征伐天下 */

#define ZONEERR_BROADMAP315 2314
/* @@error 2314 参加活动人数较少，今日活动关闭 */

#define ZONEERR_BROADMAP316 2315
/* @@error 2315 [%s]公会已经解散,[%s]在本次据点战中获胜!30秒后关闭战场 */

#define ZONEERR_BROADMAP317 2316
/* @@error 2316 [%s]公会攻占了[%s]! */

#define ZONEERR_BROADMAP318 2317
/* @@error 2317 注意：战争时间将在5分钟以后结束! */

#define ZONEERR_BROADMAP319 2318
/* @@error 2318 注意：战争时间将在10分钟以后结束! */

#define ZONEERR_BROADMAP320 2319
/* @@error 2319 注意：战争时间将在20分钟以后结束! */

#define ZONEERR_BROADMAP321 2320
/* @@error 2320 %s公会在本次据点战中获胜!30秒后关闭战场*/

#define ZONEERR_BROADMAP322 2321
/* @@error 2321 战争时间已经结束,%s公会以%d点资源点数获得最终胜利.30秒后关闭战场*/

#define ZONEERR_BROADMAP323 2322
/* @@error 2322 [%s]已放弃当前据点占领权,[%s]在本次据点战中获胜!30秒后关闭战场*/

#define ZONEERR_BROADMAP324 2323
/* @@error 2323 杀死%d个怪物!击杀数量达到%d％,可以进入下一层了*/

#define ZONEERR_BROADMAP325 2324
/* @@error 2324 杀死%d(总共%d)个怪物! */

#define ZONEERR_BROADMAP326 2325
/* @@error 2325 %s击杀了%s */


#define ZONEERR_BROADCAMPMAP301 2400
/* @@error 2400 身为人类的你，活到最后是胜利的唯一条件*/

#define ZONEERR_BROADCAMPMAP302 2401
/* @@error 2401 身为机械巨人的我们，消灭所有敌对势力，方可胜利*/

#define ZONEERR_BROADCAMPMAP303 2402
/* @@error 2402 身为机械怪兽的我们，消灭一切敌对势力，方可胜利*/


#define ZONEERR_LINK301 2500
/* @@error 2500 #L%s#n  经过不懈努力，终于成功将%s 镶嵌到@ITEMLINK@ 上*/

#define ZONEERR_LINK302 2501
/* @@error 2501 攻击！全体向@POSLINK@位置进攻！*/

#define ZONEERR_LINK303 2502
/* @@error 2502 集合！全体向@POSLINK@位置集合！*/

#define ZONEERR_LINK304 2503
/* @@error 2503 %s公会占领@POSLINK@%s，将控制世界矿产资源*/

#define ZONEERR_LINK305 2504
/* @@error 2504 你获得了%d个@ITEMLINK@ */

#define ZONEERR_LINK306 2505
/* @@error 2505 你丢弃了%d个@ITEMLINK@ */

#define ZONEERR_LINK307 2506
/* @@error 2506 %s获得了%d个@ITEMLINK@ */

#define ZONEERR_LINK308 2507
/* @@error 2507 %s 需求@ITEMLINK@ %d 点*/

#define ZONEERR_LINK309 2508
/* @@error 2508 #c00ff00恭喜%s赢得了@ITEMLINK@# */

#define ZONEERR_LINK310 2509
/* @@error 2509 %s 放弃了@ITEMLINK@ */

#define ZONEERR_LINK311 2510
/* @@error 2510 %s 公会开启了@POSLINK@公会旗帜的[战旗荣耀]活动! */

#define ZONEERR_LINK312 2511
/* @@error 2511 %s 开启了@POSLINK@公会旗帜的[战旗荣耀]活动,沐浴神辉将不断获得经验值! */

#define ZONEERR_CLANBROAD201 2600
/* @@error 2600 [%s] 建筑已经建造完成 */

#define ZONEERR_CLANBROAD202 2601
/* @@error 2601 [%s] 建筑已经升级完毕 */

#define ZONEERR_CLANBROAD203 2602
/* @@error 2602 [%s] 能量已经补满 */

#define ZONEERR_CLANBROAD204 2603
/* @@error 2603 [%s] 已经维修完毕 */

#define ZONEERR_CLANBROAD205 2604
/* @@error 2604 [%s] 已经拆除 */

#define ZONEERR_CLANBROAD206 2605
/* @@error 2605 你所在的公会已经被解散了 */

#define ZONEERR_CLANBROAD207 2606
/* @@error 2606 您的公会本次维护费用为：%d，公会剩余的公会资金%d，不足以支付维护费用，公会降到等级%hd*/

#define ZONEERR_CLANBROAD208 2607
/* @@error 2607 您的公会本次维护费用为：%d，公会剩余的公会资金%d，不足以支付维护费用，安定度越低，维护费用越高，如果维护资金不足的情况下持续%s，系统将解散公会 */

#define ZONEERR_CLANBROAD209 2608
/* @@error 2608 您的公会本次维护费用为：%d，公会剩余的公会资金%d，不足以支付维护费用，安定度越低，维护费用越高，公会将在%s后降级，如果此时间内将维护费补齐，则不会降级*/

#define ZONEERR_CLANBROAD210 2609
/* @@error 2609 您的公会本次维护费用为：%d，扣除后当前公会资金为%d，安定度越低，维护费用越高*/

#define ZONEERR_CLANBROAD211 2610
/* @@error 2610 本公会对%s公会的宣战时间结束，解除敌对状态*/

#define ZONEERR_CLANBROAD212 2611
/* @@error 2611 %s公会对本公会的宣战时间结束*/

#define ZONEERR_CLANBROAD213 2612
/* @@error 2612 %s公会在%s与本公会处于宣战状态, 持续2小时*/

#define ZONEERR_CLANBROAD214 2613
/* @@error 2613 公会会长%s已经离线超过%d天，如果会长连续离线30天，那么会长的职位自动转让给贡献度最高并且离线时间不超过 15天的公会成员*/

#define ZONEERR_CLANBROAD215 2614
/* @@error 2614 会长%s连续离线已经超过30天，会长职位自动转让给%s */

#define ZONEERR_CLANBROAD216 2615
/* @@error 2615 %s为公会增加了%u公会资金*/

#define ZONEERR_CLANBROAD217 2616
/* @@error 2616 公会资金增加了%u */

#define ZONEERR_CLANBROAD218 2617
/* @@error 2617 %s为公会增加了%d公会安定度*/

#define ZONEERR_CLANBROAD219 2618
/* @@error 2618 公会安定度增加了%d */

#define ZONEERR_CLANBROAD220 2619
/* @@error 2619 %s为公会增加了%d公会科技点*/

#define ZONEERR_CLANBROAD221 2620
/* @@error 2620 公会科技点增加了%d */

#define ZONEERR_CLANBROAD222 2621
/* @@error 2621 GM 将 %s 请出了公会*/

#define ZONEERR_CLANBROAD223 2622
/* @@error 2622 %s 退出了公会*/

#define ZONEERR_CLANBROAD224 2623
/* @@error 2623 同盟公会: %s已经被GM清除*/

#define ZONEERR_CLANBROAD225 2624
/* @@error 2624 敌对公会: %s已经被GM清除*/

#define ZONEERR_CLANBROAD226 2625
/* @@error 2625 公会的外交关系已经被GM清除*/

#define ZONEERR_CLANBROAD227 2626
/* @@error 2626 %s的会长职位被GM交换给了%s */

#define ZONEERR_CLANBROAD228 2627
/* @@error 2627 %s 加入了公会*/

#define ZONEERR_CLANBROAD229 2628
/* @@error 2628 本公会已与 %s 结盟*/

#define ZONEERR_CLANBROAD230 2629
/* @@error 2629 %s公会向您的公会进行宣战，宣战持续两个小时*/

#define ZONEERR_CLANBROAD231 2630
/* @@error 2630 您的公会对%s公会进行宣战，宣战持续两个小时*/

#define ZONEERR_CLANBROAD232 2631
/* @@error 2631 %s 将 %s 请出了公会*/

#define ZONEERR_CLANBROAD233 2632
/* @@error 2632 %s 退出了公会*/

#define ZONEERR_CLANBROAD234 2633
/* @@error 2633 您所在的公会已经解散了*/

#define ZONEERR_CLANBROAD235 2634
/* @@error 2634 会长%s启动公会解散倒计时，%s之后再次确认将解散公会*/

#define ZONEERR_CLANBROAD236 2635
/* @@error 2635 %s修改了公告*/

#define ZONEERR_CLANBROAD237 2636
/* @@error 2636 %s修改了宣言*/

#define ZONEERR_CLANBROAD238 2637
/* @@error 2637 %s将%s之位让给了%s */

#define ZONEERR_CLANBROAD239 2638
/* @@error 2638 %s 设置了 %s 的昵称 */

#define ZONEERR_CLANBROAD240 2639
/* @@error 2639 公会 %s 退出了同盟 */

#define ZONEERR_CLANBROAD241 2640
/* @@error 2640 公会 %s 退出了同盟*/

#define ZONEERR_CLANBROAD242 2641
/* @@error 2641 %s 已将 %s 设置为 %s */

#define ZONEERR_CLANBROAD243 2642
/* @@error 2642 公会技能%s等级%d研发成功，公会消耗了资金:%d */

#define ZONEERR_CLANBROAD244 2643
/* @@error 2643 你所在的公会已经被GM申请解散*/

#define ZONEERR_CLANBROAD245 2644
/* @@error 2644 公会的金钱等资源被GM修改*/

#define ZONEERR_CLANBROAD246 2645
/* @@error 2645 由于获得了部分能量，公会BOSS冲击波已自动激活，请各位前往公会城市史明威附近合力击杀*/

#define ZONEERR_CLANBROAD247 2646
/* @@error 2646 %s 用采集车提取资源，公会资源中心额外获得 [%s]×%d */

#define ZONEERR_CLANBROAD248 2647
/* @@error 2647  公会制造: %s 制造完成*/

#define ZONEERR_CLANBROAD249 2648
/* @@error 2648 公会科技: %s 研发完成*/

#define ZONEERR_CLANBROAD250 2649
/* @@error 2649 %s 为公会购买了[%s]×%d */

#define ZONEERR_CLANBROAD251 2650
/* @@error 2650 公会资源 %s 已达到上限 */

#define ZONEERR_CLANBROAD252 2651
/* @@error 2651 %s 鬼鬼祟祟的进入了您的公会城市*/

#define ZONEERR_CLANBROAD253 2652
/* @@error 2652 %s 公会的 %s  鬼鬼祟祟的进入了您的公会城市*/

#define ZONEERR_CLANBROAD254 2653
/* @@error 2653 【守护公会枢纽-普通模式】暂告一段落，未参加的成员可参加【守护公会枢纽-联合模式】*/

#define ZONEERR_CLANBROAD255 2654
/* @@error 2654 驱逐入侵活动的公会积分要求已完成，入侵者首领#L肆掠者@@(%d,%d,%d)#n被迫现身，参与中的会员获得公会积分奖励 %d点贡献度！*/

#define ZONEERR_CLANBROAD256 2655
/* @@error 2655 公会城市[#L%s@@(%d,%d,%d)#n] 正遭到攻击，请速去清剿捣乱者*/

#define ZONEERR_CLANBROAD257 2656
/* @@error 2656 公会建筑[%s] 被 %s 摧毁*/

#define ZONEERR_CLANBROAD258 2657
/* @@error 2657 公会建筑[%s] 被 %s 公会的 %s 摧毁*/

#define ZONEERR_CLANBROAD259 2658
/* @@error 2658 %s 将公会从Lv %d提升到了Lv %d */

#define ZONEERR_CLANBROAD260 2659
/* @@error 2659 %s 将公会城市从Lv %d提升到了Lv %d */

#define ZONEERR_CLANBROAD261 2660
/* @@error 2660 %s 开始建造 %s,需要时间: %s */

#define ZONEERR_CLANBROAD262 2661
/* @@error 2661 %s 开始提升 %sLv.%d 的等级,需要时间: %s */

#define ZONEERR_CLANBROAD263 2662
/* @@error 2662 %s 建造完成 */

#define ZONEERR_CLANBROAD264 2663
/* @@error 2663 %sLv.%d 升级完成 */

#define ZONEERR_CLANBROAD265 2664
/* @@error 2664 公会[#L%s@@(%d,%d,%d)#n] 受到严重破坏，请火速支援*/

#define ZONEERR_CLANBROAD266 2665
/* @@error 2665 成功驱逐入侵者！驱逐入侵活动暂告一段落*/

#define ZONEERR_CLANBROAD267 2666
/* @@error 2666 %s%s开启了驱逐入侵活动，找到时间城的#L斯蒂芬斯@@[303006,1000]#n火速传送到公会冶炼岛！*/

#define ZONEERR_CLANBROAD268 2667
/* @@error 2667 您所在公会竞拍%s失败,退还公会资金%d */

#define ZONEERR_CLANBROAD269 2668
/* @@error 2668 您所在公会竞拍%s成功,请所有公会成员于晚上20:30-21:30准时参加据点战 !#cff0000（注意：据点战开战后战团配置将不能再做调整）#n*/

#define ZONEERR_CLANBROAD270 2669
/* @@error 2669 您所在公会竞拍%s成功,因为您所在公会是唯一参加竞拍此据点公会,您的公会现为占领公会*/

#define ZONEERR_CLANBROAD271 2670
/* @@error 2670 您所在公会以竞拍结果第1名竞拍%s成功,系统判定为防守方公会,请所有公会成员于晚上20：30准时参加据点战!#cff0000（注意：据点战开战后战团配置将不能在做调整）#n*/

#define ZONEERR_CLANBROAD272 2671
/* @@error 2671 您所在公会以竞拍结果第2名竞拍%s成功,系统判定为挑战方公会,请所有公会成员于晚上20：30准时参加据点战!#cff0000（注意：据点战开战后战团配置将不能在做调整）#n*/

#define ZONEERR_CLANBROAD273 2672
/* @@error 2672 对方公会放弃了%s或者解散了公会,当前据点无人占领,您的公会现为占领公会*/

#define ZONEERR_CLANBROAD274 2673
/* @@error 2673 %s战副本已经开启*/

#define ZONEERR_CLANBROAD275 2674
/* @@error 2674 %s公会旗帜已褪去辉芒,战旗荣耀活动暂告一段落*/

#define ZONEERR_CLANBROAD276 2675
/* @@error 2675 您所在公会已放弃%s占领权*/

#define ZONEERR_CLANBROAD277 2676
/* @@error 2676 你被%s%s踢出公会城市*/

#define ZONEERR_CLANBROAD278 2677
/* @@error 2677 上周%s %s在公会管理中表现突出,领取了时间城议会发放的%d%s奖励*/

#define ZONEERR_KICK201 2700
/* @@error 2700 您所登录地图人数过多，请稍后登录*/

#define ZONEERR_KICK202 2701
/* @@error 2701 清空任务,请重新登录*/

#define ZONEERR_KICK203 2702
/* @@error 2702 系统时间被修改*/

#define ZONEERR_KICK204 2703
/* @@error 2703 你的角色被冻结 %d 秒*/

#define ZONEERR_KICK205 2704
/* @@error 2704 有来自ip地址为%s的用户使用您的帐号登录*/

#define ZONEERR_KICK206 2705
/* @@error 2705 空闲时间过长*/

#define ZONEERR_KICK207 2706
/* @@error 2706 角色改名成功*/

#define ZONEERR_KICK208 2707
/* @@error 2707 你的角色被冻结 %d 秒*/

#define ZONEERR_KICK209 2708
/* @@error 2708 你的角色被永久冻结了*/

#define ZONEERR_KICK210 2709
/* @@error 2709 你的帐号被永久冻结了*/

#define ZONEERR_KICK211 2710
/* @@error 2710 你的帐号被冻结 %d 秒*/

#define ZONEERR_KICK212 2711
/* @@error 2711 角色被踢下线,请与客服联系*/

#define ZONEERR_KICK213 2712
/* @@error 2712 登录的IP地址被限制*/

#define ZONEERR_KICK214 2713
/* @@error 2713 系统关闭*/


#define ZONEERR_SCRIPT301 2800
/* @@error 2800 你开启地心宝藏,获得%s */

#define ZONEERR_SCRIPT302 2801
/* @@error 2801 账号处于异常登录状态,%d分钟内不能进行此操作*/

#define ZONEERR_SCRIPT303 2802
/* @@error 2802 每日使用次数已经达到上限了*/

#define ZONEERR_SCRIPT304 2803
/* @@error 2803 %s数量达到上限了,添加失败*/

#define ZONEERR_SCRIPT305 2804
/* @@error 2804 本层杀怪数小于%d％*/

#define ZONEERR_SCRIPT306 2805
/* @@error 2805 你突破了等级的极限,经验奖励翻倍*/

#define ZONEERR_SCRIPT307 2806
/* @@error 2807 你突破了等级的极限,开启地心宝藏获得双倍奖励,得到%s和%s */

#define ZONEERR_SCRIPT308 2807
/* @@error 2807 %s 距离太远*/

#define ZONEERR_SCRIPT309 2808
/* @@error 2808 %s 现在忙*/

#define ZONEERR_SCRIPT310 2809
/* @@error 2809 可用次数已经达到上限*/

#define ZONEERR_SCRIPT311 2810
/* @@error 2810 必要材料不足,请检查材料数目*/

#define ZONEERR_DIFF301 2900
/* @@error 2900 %s不在线*/

#define ZONEERR_DIFF302 2901
/* @@error 2901 此副本%d分钟内只能进一次*/

#define ZONEERR_DIFF303 2902
/* @@error 2902 思过厅中的黑名玩家不能进入副本*/

#define ZONEERR_DIFF304 2903
/* @@error 2903 每日可完成次数已达上限*/

#define ZONEERR_DIFF305 2904
/* @@error 2904 每周可完成次数已达上限*/

#define ZONEERR_DIFF306 2905
/* @@error 2905 %s不是公会成员*/

#define ZONEERR_DIFF307 2906
/* @@error 2906 公会贡献度不够*/

#define ZONEERR_DIFF308 2907
/* @@error 2907 等级不足无法进入副本*/

#define ZONEERR_DIFF309 2908
/* @@error 2908 等级过高无法进入副本*/

#define ZONEERR_SAFE301 3000
/* @@error 3000 账号处于异常登录状态,%d分钟内不能进行此操作*/

#define ZONEERR_SAFE302 3001
/* @@error 3001 安全锁密码验证失败*/

#define ZONEERR_SAFE303 3002
/* @@error 3002 你没有设置安全锁密码*/

#define ZONEERR_SAFE304 3003
/* @@error 3003 安全锁密码验证失败*/

#define ZONEERR_SAFE305 3004
/* @@error 3004 安全锁密码不能为空*/

#define ZONEERR_SAFE306 3005
/* @@error 3005 修改密码成功*/


#define ZONEERR_SAFE309 3008
/* @@error 3008 安全锁密码已经取消*/

#define ZONEERR_SAFE310 3009
/* @@error 3009 安全锁重新生效*/

#define ZONEERR_SAFE311 3010
/* @@error 3010 安全锁功能只对%d级玩家开放*/

#define ZONEERR_TASK301 3100
/* @@error 3100 未完成前置任务,不能领取任务*/


#define ZONEERR_PAK701 10000
/* @@error 10000 您的定位符定位成功*/

#define ZONEERR_PAK702 10001
/* @@error 10001 此物品无法在末世荣耀殿堂内使用*/

#define ZONEERR_PAK703 10002
/* @@error 10002 每日正义点获取已达到上限*/

#define ZONEERR_CLAN801 11000
/* @@error 11000 %s%s把%s踢出公会城市*/

#define ZONEERR_ARM901 12000
/* @@error 12000 恭喜%s 的 %s的第%d星达成完美，神兵降世，祝其鸿运再来*/

#define ZONEERR_ARM902 12001
/* @@error 12001 徽章装备不可洗炼*/

#define ZONEERR_ARM903 12002
/* @@error 12002 时装不可洗炼*/

#define ZONEERR_ARM904 12003
/* @@error 12003 此洗炼材料不能用于高级洗炼*/

#define ZONEERR_ARM905 12004
/* @@error 12004 徽章装备不可镶嵌宝石*/

#define ZONEERR_ARM906 12005
/* @@error 12005 徽章装备不可宝石摘取*/

#define ZONEERR_ARM907 12006
/* @@error 12006 徽章装备不可转星*/

#define ZONEERR_ARM908 12007
/* @@error 12007 请拆分为单个宝石再充能*/

#define ZONEERR_ARM909 12008
/* @@error 12008 徽章装备不可重铸*/

#define ZONEERR_ARM910 12009
/* @@error 12009 时装不可重铸*/

#define ZONEERR_ARM911 12010
/* @@error 12010 此类重铸石不能用于高级重铸*/

#define ZONEERR_ARM912 12011
/* @@error 12011 徽章装备不可强化*/

#define ZONEERR_ARM913 12012
/* @@error 12012 徽章装备不可转移属性*/

#define ZONEERR_ARM914 12013
/* @@error 12013 此装备没有需要激活的技能属性*/

#define ZONEERR_ARM915 12014
/* @@error 12014 银币不足,激活失败*/

#define ZONEERR_ARM916 12015
/* @@error 12015 银币不足,无法进行属性重置*/

#define ZONEERR_ARM917 12016
/* @@error 12016 神器出世谁与争锋,%s终于制作出属于他个人的神器:@ITEMLINK@*/

#define ZONEERR_ARM918 12017
/* @@error 12017 恭喜您的%s第%d星达成完美!*/


#define ZONEERR_PLAYER801 13000
/* @@error 13000 你还有未撤销的金币交易单，不能改名*/

#define ZONEERR_PLAYER802 13001
/* @@error 13001 你还有未撤销的银币交易单，不能改名*/

#define ZONEERR_PLAYER803 13002
/* @@error 13002 荣耀殿堂十大战神之%s被英勇的%s击败!*/

#define ZONEERR_PLAYER804 13003
/* @@error 13003 恭喜玩家%s鸿运天降,获得神兵@ITEMLINK@ */

#define ZONEERR_PLAYER805 13004
/* @@error 13004 活力值不足, 无法享受海滩经验 */

#define ZONEERR_BATTLE100 14000
/* @@error 14000 [%s]成功摧毁了[%s]的要塞，获得了这场战争的胜利 */

#define ZONEERR_BATTLE101 14001
/* @@error 14001 战争时间已结束，[%s]艰难的获得了最终胜利*/

#define ZONEERR_BATTLE102 14002
/* @@error 14002 [%s]%s以迅雷不及掩耳之势向粒子控制器注入了能源结晶*/

#define ZONEERR_BATTLE103 14003
/* @@error 14003 你不是公会成员,无法报名此战场*/

#define ZONEERR_BATTLE104 14004
/* @@error 14004 你已经报了名,无法再报名此战场*/

#define ZONEERR_BATTLE105 14005
/* @@error 14005 所在公会没有报名该战场，您还不能独自加入*/

#define ZONEERR_BATTLE106 14006
/* @@error 14006 所在公会已经开启其他战场*/

#define ZONEERR_BATTLE107 14007
/* @@error 14007 所在的公会还没有开启此战场,你无法报名此战场*/

#define ZONEERR_BATTLE108 14008
/* @@error 14008 你不是公会成员,无法开启此战场*/

#define ZONEERR_BATTLE109 14009
/* @@error 14009 熔岩巨兽活动正在进行中，无法报名*/

#define ZONEERR_BATTLE110 14010
/* @@error 14010 已报名末世荣耀殿堂，无法报名其他的战场*/

#define ZONEERR_BATTLE111 14011
/* @@error 14011 %s已报名末世荣耀殿堂，无法进行战场报名*/

#define ZONEERR_BATTLE112 14012
/* @@error 14012 你的权限不足,无法开启此战场*/

#define ZONEERR_BATTLE113 14013
/* @@error 14013 战场已结束,无法进入*/

#define ZONEERR_BATTLE114 14014
/* @@error 14014 未找到对应的公会队列*/

#define ZONEERR_BATTLE115 14015
/* @@error 14015 得到一块能源结晶，速度运往粒子控制器*/

#define ZONEERR_BATTLE116 14016
/* @@error 14016 缺少能源结晶，无法驱动粒子控制器*/

#define ZONEERR_BATTLE117 14017
/* @@error 14017 你已超越现阶段战场的实力，可提升等级挑战下阶段战场*/


#define ZONEERR_CARDBOX100 15000
/* @@error 15000 %s福星高照，从%s中得到了%s x%d*/

#define ZONEERR_ADD_MSG_SHOW 15001
/* @@error 15001 恭喜玩家:%s 得到@ITEMLINK@,真是鸿运当头!*/

#define ZONEERR_NO_VIM 15002
/* @@error 15002 您的精力值不足*/

#define ZONEERR_CLAN_DART1 15003
/* @@error 15003 %s 公会开启了[公会商运]活动,成功商运将额外获得大量经验值!*/

#define ZONEERR_CLAN_DART2 15004
/* @@error 15004 %s 公会结束了繁忙的商运贸易,[公会商运]活动暂告一段落!*/

#define ZONEERR_CLAN_DART3 15005
/* @@error 15005 %s 公会的%s 被 %s 公会摧毁!*/

#define ZONEERR_STORE1 15006
/* @@error 15006 银币不足!*/

#define ZONEERR_STORE2 15007
/* @@error 15007 商铺名已存在!*/

#define ZONEERR_STORE3 15008
/* @@error 15008 死亡中无暇顾及店铺信息!*/

#define ZONEERR_STORE4 15009
/* @@error 15009 战斗中无暇顾及店铺信息!*/

#define ZONEERR_STORE5 15010
/* @@error 15010 摆摊中无暇顾及店铺信息!*/

#define ZONEERR_STORE6 15011
/* @@error 15011 交易中无暇顾及店铺信息!*/

#define ZONEERR_STORE7 15012
/* @@error 15012 切磋中无暇顾及店铺信息!*/

#define ZONEERR_STORE8 15013
/* @@error 15013 搜索到有%d个店铺出售相关商品，请点【有货】店铺可查看!*/

#define ZONEERR_STORE9 15014
/* @@error 15014 未搜到有店铺出售相关商品，请正确输入商品的名字!*/

#define ZONEERR_CLAN_301 15100
/* @@error 15100 今天已经开启过公会商运活动*/

#define ZONEERR_CLAN_302 15101
/* @@error 15101 此时段不能开启公会商运活动*/

#define ZONEERR_COLLECT_301 15201
/* @@error 15201 无法获得更多了*/

#define ZONEERR_DIFF_301 15301
/* @@error 15301 你已经领取过了*/

#define ZONEERR_LOSTTOTEM_301 15401
/* @@error 15401 每天使用次数最多为5次*/

#define ZONEERR_LOSTTOTEM_302 15402
/* @@error 15402 不能在失落之域以外的地图使用*/

#define ZONEERR_MAP_702 15501
/* @@error 15501 熔岩巨兽活动正在进行中，不可进入副本*/

#define ZONEERR_MAP_703 15502
/* @@error 15502 该地图不对新手进行保护，您进入可能会遭受攻击*/

#define ZONEERR_MAP_704 15503
/* @@error 15503 您还未突破本层不能领取突破奖励*/

#define ZONEERR_MAP_705 15504
/* @@error 15504 你已经领取过本层的奖励*/

#define ZONEERR_MISC_301 15601
/* @@error 15601 经过一番努力，你获得了%d个%s，快去跟米尔斯换取银币吧*/

#define ZONEERR_MISC_302 15602
/* @@error 15602 只有在阳光海滩才能养海蚌哟*/

#define ZONEERR_MISC_303 15603
/* @@error 15603 养海蚌必须在特殊的区域才可以哟，比如阳光海滩晒日光浴附近的浅海里（坐标：309 167）*/

#define ZONEERR_MISC_304 15604
/* @@error 15604 小海蚌已经出现海水里了，记得喂它，更要记得来收珍珠哟*/

#define ZONEERR_MISC_305 15605
/* @@error 15605 小海蚌没有那么大胃口哦，喂太多会把它撑坏的哟*/

#define ZONEERR_MISC_306 15606
/* @@error 15606 %s数量不足*/

#define ZONEERR_NPC_301 15700
/* @@error 15700 战斗力不足无法购买*/

#define ZONEERR_PACKAGE_301 15800
/* @@error 15800 当前地图不能使用*/

#define ZONEERR_PACKAGE_302 15801
/* @@error 15801 一帆风顺！加速前进！*/

#define ZONEERR_PACKAGE_303 15802
/* @@error 15802  船撞宝箱上了，减速进行中！*/

#define ZONEERR_PACKAGE_304 15803
/* @@error 15803  宝箱里的宝藏让你眼冒金星，头晕目眩，忘了继续前行！*/

#define ZONEERR_PACKAGE_305 15804
/* @@error 15804  顺手摸到一个宝箱，打开一看竟然白花花的银币呐！*/

#define ZONEERR_PACKAGE_306 15805
/* @@error 15805  这是一个经验宝箱！*/

#define ZONEERR_PACKAGE_307 15806
/* @@error 15806  哗啦啦！一通绑银迎面扑来！*/

#define ZONEERR_PACKAGE_308 15807
/* @@error 15807  传承点数不足，暂时不能佩戴徽章*/

#define ZONEERR_PACKAGE_309 15808
/* @@error 15808  你还没有加入公会,无法使用公会资金券! */

#define ZONEERR_PACKAGE_310 15809
/* @@error 15809  当前地图不可以使用队伍传送卷*/

#define ZONEERR_PACKAGE_311 15810
/* @@error 15810  当前地图不可以使用定位符*/

#define ZONEERR_PACKAGE_312 15811
/* @@error 15811  未加入公会，无法使用*/

#define ZONEERR_PLAYER_301 15900
/* @@error 15900  学生登记已满，请过会再登记! */

#define ZONEERR_PLAYER_302 15901
/* @@error 15901  已经登记的学生不能在登记! */

#define ZONEERR_PLAYER_303 15902
/* @@error 15902  已达到交票金额，无法再购买贸易商品，可请往交付任务*/

#define ZONEERR_PLAYER_304 15903
/* @@error 15903  贸易商人调整了商品的价格，请重新出售商品*/

#define ZONEERR_PLAYER_305 15904
/* @@error 15904  贸易商人调整了商品的价格，请重新购买商品*/

#define ZONEERR_PLAYER_306 15905
/* @@error 15905  商品已卖光，请挑选其他货品，或等待贸易商人上货*/

#define ZONEERR_PLAYER_307 15906
/* @@error 15906  你花费了%d银票购买了%d件贸易商品*/

#define ZONEERR_PLAYER_308 15907
/* @@error 15907  你出售了%d件贸易商品获得%lld银票*/

#define ZONEERR_PLAYER_309 15908
/* @@error 15908  您目前的贸易本金（银票）太少，商会特意为您开通了便利通道，可与任意贸易商人兑换银票*/

#define ZONEERR_PLAYER_310 15909
/* @@error 15909  成功出售贸易商品获得%lld银票*/


#define ZONEERR_ARM801 16001
/* @@error 16001 当前星级已经是完美强化!*/

#define ZONEERR_ZHENGYI_DEC		17001
/* @@error 17001 你消耗了%d正义点数 */

#define ZONEERR_ZHENGYI_ADD		17002
/* @@error 17002 你获得了%d正义点数 */

#define ZONEERR_LIFE_BRAND_DEC		17003
/* @@error 17003 你消耗了%d生存奖章 */

#define ZONEERR_LIFE_BRAND_ADD		17004
/* @@error 17004 你获得了%d生存奖章 */

#define ZONEERR_CLAN_BATTLE_WIN		17005
/* @@error 17005 %s公会在死亡之星跨服战争中胜出，大家为他们欢呼吧 */

#define ZONEERR_CLAN_BATTLE_FAIL		17006
/* @@error 17006 %s公会在死亡之星公会跨服战争中战败，大家为他们加油吧 */

#define ZONEERR_NOTINWAITQUEUE		17007
/* @@error 17007 你不在等待队列中或公会不再等待队列中,无法进入战场 */

#define ZONEERR_PWORLDOVER		17008
/* @@error 17008 战场已结束,无法进入战场 */

#define ZONEERR_CLAN_IN_BATTLE		17009
/* @@error 17009 公会已报名跨服战场,无法解散 */

#define ZONEERR_BAOXIANG200		17100
/* @@error 17100 背包已满，您抽中的道具已通过邮件的方式发送给您，请注意查收 */

#define ZONEERR_TEAM800		17200
/* @@error 17200 队伍已锁定，不可进入此队伍*/

#define ZONEERR_TEAM801		17201
/* @@error 17201 队伍已锁定，不可邀请任何玩家进入队伍*/

#define ZONEERR_TASK801		17300
/* @@error 17300 此活动每周最多可完成%d次,每周一早上六点重置次数,请稍后再试! */

#define ZONEERR_TASK802		17301
/* @@error 17301 已接受环组任务[%s]，请查看任务面板 */

#define ZONEERR_TASK803		17302
/* @@error 17302 该道具只能触发环组的第一个任务，请到npc处接取后续任务 */

#define ZONEERR_SHOP301		17400
/* @@error 17400 此商品只可同时购买一个*/

#define ZONEERR_SHOP302		17401
/* @@error 17401 已拥有此商品*/

#define ZONEERR_SHOP303		17402
/* @@error 17402 您成功激活了成长基金礼包，每日“礼”字内可领取一次金券，持续30天*/

#define ZONEERR_SHOP304		17403
/* @@error 17403 此商品无法赠送*/

#define ZONEERR_SPARTA301		17500
/* @@error 17500 末世荣耀殿堂未开启*/

#define ZONEERR_SPARTA302		17501
/* @@error 17501 未到开启时间，不能报名末世荣耀殿堂*/

#define ZONEERR_SPARTA303		17502
/* @@error 17502 需要3人队伍才能报名末世荣耀殿堂*/

#define ZONEERR_SPARTA304		17503
/* @@error 17503 末世荣耀殿堂队伍报名需要队长才能报名*/

#define ZONEERR_SPARTA305		17504
/* @@error 17504 报名失败，末世荣耀殿堂只能3人队伍才能报名*/

#define ZONEERR_SPARTA306		17505
/* @@error 17505 您已报名战场，无法进行末世荣耀殿堂报名*/

#define ZONEERR_SPARTA307		17506
/* @@error 17506 已报名末世荣耀殿堂，无需重复报名*/

#define ZONEERR_SPARTA308		17507
/* @@error 17507 副本或思过厅中不能报名末世荣耀殿堂*/

#define ZONEERR_SPARTA309		17508
/* @@error 17508 %s处于离线或跨服状态，无法报名末世荣耀殿堂*/

#define ZONEERR_SPARTA310		17509
/* @@error 17509 %s等级不足60级，不能进行竞技场报名*/

#define ZONEERR_SPARTA311		17510
/* @@error 17510 队员%s在报名战场中，无法报名末世荣耀殿堂*/

#define ZONEERR_SPARTA312		17511
/* @@error 17511 %s正在副本或思过厅中，报名失败*/

#define ZONEERR_SPARTA313		17512
/* @@error 17512 %s正处于末世巅峰准备地图或巅峰竞技副本中,无法报名末世荣耀殿堂*/

#define ZONEERR_STORE301		17600
/* @@error 17600 资产不足支付租赁费,店铺已自动收店! */

#define ZONEERR_STORE302		17601
/* @@error 17601 店铺已负资产,将在%d小时%d分钟后自动收店,请及时预存足够的资产! */

#define ZONEERR_STORE303	17602
/* @@error 17602 商品已下架或者已售完! */

#define ZONEERR_STORE304	17603
/* @@error 17603 银币将超出携带上限,无法取出该额度的资产 ! */

#define ZONEERR_STORE305	17604
/* @@error 17604 资产将超出容纳上限,无法存入该额度的银币! */

#define ZONEERR_STORE306   17605
/* @@error 17605 银币将超出携带上限! */

#define ZONEERR_STORE307   17606
/* @@error 17606 查看的店铺不存在! */

#define ZONEERR_SYNCRELATION301   17700
/* @@error 17700 您还没有导师毕业，暂时不能收为学生 */

#define ZONEERR_SYNCRELATION302   17701
/* @@error 17701 您最多可以收5个学生 */

#define ZONEERR_SYNCRELATION303   17702
/* @@error 17702 目标玩家不在周围 */

#define ZONEERR_SYNCRELATION304   17703
/* @@error 17703 等级在30至60之间的玩家才能被收为学生*/

#define ZONEERR_SYNCRELATION305   17704
/* @@error 17704 目标玩家距离过远*/

#define ZONEERR_SYNCRELATION306   17705
/* @@error 17705 您距离过远*/

#define ZONEERR_SYNCRELATION307   17706
/* @@error 17706 您想收的学生已经有其他师生关系了*/

#define ZONEERR_SYNCRELATION308   17707
/* @@error 17707 邀请发送成功，等待对方应答*/

#define ZONEERR_SYNCRELATION309   17708
/* @@error 17708 你的等级已经不适合成为学生*/

#define ZONEERR_SYNCRELATION310   17709
/* @@error 17709 导师距离过远*/

#define ZONEERR_SYNCRELATION311   17710
/* @@error 17710 申请超时*/

#define ZONEERR_SYNCRELATION312   17711
/* @@error 17711 %s拒绝了您的请求*/

#define ZONEERR_SYNCRELATION313   17712
/* @@error 17712 您需要最少500点活力值才能解除师徒关系*/

#define ZONEERR_SYNCRELATION314   17713
/* @@error 17713 此学生今日已经冲入过角色经验! */

#define ZONEERR_SYNCRELATION315   17714
/* @@error 17714 您当前的经验不足 */

#define ZONEERR_SYNCRELATION316   17715
/* @@error 17715 充入角色经验成功，您的学生领取经验后您会获得传承点数 */

#define ZONEERR_SYNCRELATION317   17716
/* @@error 17716 今日领取次数已达上限*/

#define ZONEERR_SYNCRELATION318   17717
/* @@error 17717 您还未到65级暂时不能毕业*/

#define ZONEERR_SYNCRELATION319   17718
/* @@error 17718 您的导师不在附近*/

#define ZONEERR_SYNCRELATION320   17719
/* @@error 17719 您还没有导师毕业，暂时不能收为学生*/

#define ZONEERR_SYNCRELATION321   17720
/* @@error 17720 您的导师本周已经毕业了%d名学生，您暂时不能考核毕业*/

#define ZONEERR_SYNCRELATION322   17721
/* @@error 17721 您的导师不在线，暂时不能考核毕业*/


#define ZONEERR_ATTR801	18000
/* @@error 18000 生存奖章已经达到上限，请您兑换道具后继续获取 */

#define ZONEERR_ATTR802	18001
/* @@error 18001 每日生存奖章获取已达到上限*/

#define ZONEERR_STRONG239 18100
/* @@error 18100 %d级公会最多只能占领%d个据点!*/

#define ZONEERR_STRONG240	18101
/* @@error 18101 【%s】公会开启了【守护公会枢纽-联合模式】活动，任何公会成员都可支援作战，获取丰厚奖励！*/

#define ZONEERR_STRONG241	18102
/* @@error 18102 【%s】公会的【守护公会枢纽-联合模式】活动已暂告一段落*/

#define ZONEERR_STRONG242	18103
/* @@error 18103 每个已占领据点必须分配一个战团，公会没有多余战团可分配！*/

#define ZONEERR_STRONG243	18104
/* @@error 18104 据点正在激战争夺中，暂时不能对据点战团进行调整！*/

#define ZONEERR_STRONG244	18105
/*@@error 18105  今天已参加过任一模式的【守护公会枢纽活动】，无法再次参加!*/

#define ZONEERR_CLAN336 19000
/*@@error 19000 公会等级不足，无法研发此技能*/


#define ZONEERR_CLANPET1 19100
/*@@error 19100 公会成员[%s]已将宠物[%s]寄养在公会城市宠物乐园中,烦请诸位帅哥美女们多多照料*/

#define ZONEERR_CLANPET2 19101
/*@@error 19101 亲爱的玩家:目前银币交易行已有%d笔交易单高于近期交易平均价,性价比超值,请大家踊跃抢购!*/

#define ZONEERR_DIANFENG1 19200
/*@@error 19200 报名失败，参加末世巅峰争霸赛需要等级大于等于65级*/

#define ZONEERR_DIANFENG2 19201
/* @@error 19201 此物品无法在末世巅峰竞技副本中使用*/

#define ZONEERR_DIANFENG3 19202
/*@@error 19202 银币不足%d，无法支付报名费，报名失败*/

#define ZONEERR_DIANFENG4 19203
/*@@error 19203 末世巅峰报名成功，请于本周六准时参加比赛！*/

#define ZONEERR_DIANFENG5 19204
/*@@error 19204 您银币不足%d，无法参与冠军竞猜*/

#define ZONEERR_DIANFENG6 19205
/*@@error 19205 您已错过了末世巅峰争霸赛参赛时间，被视为自动放弃！*/

#define ZONEERR_DIANFENG7 19206
/*@@error 19206 末世巅峰争霸赛报名将在20分钟后结束，需要报名参加的勇士请尽快报名！*/

#define ZONEERR_DIANFENG8 19207
/*@@error 19207 由于对方突然离线，系统将再次为您进行对手选择！*/

#define ZONEERR_DIANFENG9 19208
/*@@error 19208 您已经报名了*/

#define ZONEERR_DIANFENG10 19209
/*@@error 19209  您所在职业组报名人数已满*/

#define ZONEERR_DIANFENG11 19210
/*@@error 19210  有个别职业组报名人数不足%d 人，无法开始冠军竞猜*/

#define ZONEERR_DIANFENG12 19211
/*@@error 19211  参与末世巅峰冠军竞猜人数已满*/

#define ZONEERR_DIANFENG13 19212
/*@@error 19212 您已经竞猜了*/

#define ZONEERR_DIANFENG14 19213
/*@@error 19213 恭喜您获得末世巅峰争霸赛参与奖励，请前往#L卡拉洛克@@[303063,1000]#n处领取奖励！*/

#define ZONEERR_DIANFENG15 19214
/*@@error 19214 %s获得末世巅峰争霸赛冠军!*/

#define ZONEERR_DIANFENG16 19215
/*@@error 19215 %s*/

#define ZONEERR_DIANFENG17 19216
/*@@error 19216  此地图无 法传送到末世巅峰准备场地!*/

#define ZONEERR_DIANFENG18 19217
/*@@error 19217  副本中无法传送!*/

#define ZONEERR_DIANFENG19 19218
/*@@error 19218 跨服状态无法操作*/

#define ZONEERR_DIANFENG20 19219
/*@@error 19219 由于您在比赛中离线时间过长或过于频繁，系统已默认为自动放弃!*/

#define ZONEERR_DIANFENG21 19220
/*@@error 19220 由于对方离线时间过长，您已自动晋级到下一轮比赛！*/

#define ZONEERR_DIANFENG22 19221
/*@@error 19221 提取金币奖励已经通过邮件发送!*/

#define ZONEERR_DIANFENG23 19222
/*@@error 19222 跨服状态或战场报名状态不能进行巅峰传送操作*/

#define ZONEERR_DIANFENG24 19223
/* @@error 19223 末世巅峰准备地图中或巅峰竞技副本中不能报名战场*/

#define ZONEERR_DIANFENG25 19224
/* @@error 19224 %s正在末世巅峰准备地图中或巅峰竞技副本中, 无法报名战场*/

#define ZONEERR_DIANFENG26 19225
/* @@error 19225 末世巅峰争霸赛正在激战中,竞猜功能已关闭*/

#define ZONEERR_DIANFENG27 19226
/* @@error 19226 末世巅峰争霸赛还未开启,竞猜功能暂时关闭*/

#define ZONEERR_DIANFENG28 19227
/* @@error 19227 末世巅峰争霸赛正在激战中,报名功能已关闭*/

#define ZONEERR_DIANFENG29 19228
/* @@error 19228 末世巅峰争霸赛还未开启,报名功能暂时关闭*/

#define ZONEERR_DIANFENG30 19229
/* @@error 19229 您所选择的职业还没有人报名参赛，请稍后再来！*/

#define ZONEERR_DIANFENG31 19230
/* @@error 19230 此处的魔力磁场干扰了传送，请前往其它地图进行传送!*/

#define ZONEERR_DIANFENG32 19231
/* @@error 19231 %s 处于不可传送地图!*/

#define ZONEERR_DIANFENG33 19232
/* @@error 19232 处于末世巅峰准备地图或巅峰竞技副本中,不可进入副本*/

#define ZONEERR_DIANFENG34 19233
/*@@error 19233 由于对方放弃比赛，您已自动晋级到下一轮比赛！*/

#define ZONEERR_DIANFENG35 19234
/*@@error 19234 保险金不足，停止合成*/

#define ZONEERR_DIANFENG36 19235
/* @@error 19235 处于战斗状态, 不可以传送*/

#define ZONEERR_DIANFENG37 19236
/* @@error 19236 处于定身状态, 不可以传送*/

#define ZONEERR_DIANFENG38 19237
/* @@error 19237 比赛准备地图中无法进行传送，请放弃比赛后再进行传送！*/

#define ZONEERR_DIANFENG39 19238
/*@@error 19238 由于对方放弃比赛，您已自动晋级到下一轮比赛！*/

#define ZONEERR_DIANFENG40 19239
/*@@error 19239 由于对方放弃比赛，系统将再次为您进行对手选择！*/

#define ZONEERR_DIANFENG41 19240
/*@@error 19240 您已自动放弃末世巅峰比赛*/

#define ZONEERR_TANK1 19300
/*@@error 19300 你被敌方的%s击毁*/

#define ZONEERR_TANK2 19301
/*@@error 19301 %s在风骚的操作下，一炮击毁了敌方%d辆战车！*/

#define ZONEERR_TANK3 19302
/*@@error 19302 %s简直为战争而生，一炮击毁了敌方%d辆战车！*/

#define ZONEERR_TANK4 19303
/*@@error 19303 %s写下了新的神话！一炮击毁了敌方%d辆战车！*/

#define ZONEERR_TANK5 19304
/*@@error 19304 %s已超越了地球人！一炮击毁了敌方全队战车！！*/


#define ZONEERR_CLAN_PET1 19401
/*@@error 19401 宠物%s增加%d点潜力值*/

#define ZONEERR_CLAN_PET2 19402
/* @@error 19402 您的%s 不足!不能复活!*/

#define ZONEERR_CLAN_PET3 19403
/* @@error 19403 比赛服务器禁止使用!*/


#define ZONEERR_SHOP_LIMIT1 19501
/* @@error 19501 %d天之内只能购买一次%s*/

#define ZONEERR_SHOP_LIMIT2 19502
/* @@error 19502 该角色已经拥有该道具，请尽快使用后才能购买*/

#define ZONEERR_SHOP_LIMIT3 19503
/* @@error 19503 %s一次只能购买一个*/

#define ZONEERR_SHOP_LIMIT4 19504
/* @@error 19504 %s不能赠送*/


#define ZONEERR_PET195 19505
/* @@error 19505 此道具只能对出战宠物使用，请将宠物状态修改为出战！*/

#define ZONEERR_PET196 19506
/* @@error 19506 只有%s的宠物才能使用%s!*/


#define ZONEERR_WEDDING1 19507
/* @@error 19507 %s并非您的好友,无法向对方赠送%s*/

#define ZONEERR_WEDDING2 19508
/* @@error 19508 您并非%s的好友,无法向对方赠送%s*/

#define ZONEERR_WEDDING3 19509
/* @@error 19509 您与%s的距离太远,无法向对方赠送%s*/

#define ZONEERR_WEDDING4 19510
/* @@error 19510 你们已是情侣关系,无法再向对方赠送%s*/

#define ZONEERR_WEDDING5 19511
/* @@error 19511 你们已是夫妻关系,无法再向对方赠送%s*/

#define ZONEERR_WEDDING6 19512
/* @@error 19512 %s已找到了命中的伴侣,无法再向对方赠送%s*/

#define ZONEERR_WEDDING7 19513
/* @@error 19513 您已拥有了命中的伴侣,无法再向对方赠送%s*/

#define ZONEERR_WEDDING8 19514
/* @@error 19514 对方不在线!*/

#define ZONEERR_WEDDING9 19515
/* @@error 19515 你们当前为情侣,无法将对方从你好友中剔除*/

#define ZONEERR_WEDDING10 19516
/* @@error 19516 你们当前为夫妻,无法将对方从你好友中剔除*/

#define ZONEERR_WEDDING11 19517
/* @@error 19517 很遗憾你仍然单身,暂时无法订制婚礼套餐*/

#define ZONEERR_WEDDING12 19518
/* @@error 19518 暂时无法订制婚礼套餐，需要双方亲密度达到%d*/

#define ZONEERR_WEDDING13 19519
/* @@error 19519 %s似乎并不在场,暂时无法订制婚礼套餐*/

#define ZONEERR_WEDDING14 19520
/* @@error 19520 %s似乎仍未到场,暂时无法订制婚礼套餐*/

#define ZONEERR_WEDDING15 19521
/* @@error 19521 你们已经订制了婚礼套餐,无法再订制婚礼套餐*/

#define ZONEERR_WEDDING17 19523
/* @@error 19523 很遗憾你仍然单身,暂时无法开始婚礼*/

#define ZONEERR_WEDDING18 19524
/* @@error 19524 暂时无法开始举办婚礼，需要双方亲密度达到%d*/

#define ZONEERR_WEDDING19 19525
/* @@error 19525 %s似乎并不在场,暂时无法开始婚礼*/

#define ZONEERR_WEDDING20 19526
/* @@error 19526 %s似乎仍未到场,暂时无法开始婚礼*/

#define ZONEERR_WEDDING21 19527
/* @@error 19527 你们的婚礼已经开始了,无法再重新开始婚礼*/

#define ZONEERR_WEDDING22 19528
/* @@error 19528 %s似乎并不在场,暂时无法举行婚礼庆典*/

#define ZONEERR_WEDDING23 19529
/* @@error 19529 很遗憾你并没有伴侣,无法开始婚礼庆典*/

#define ZONEERR_WEDDING24 19530
/* @@error 19530 似乎有一对新人正在举行婚礼庆典,请结束后再举行婚礼庆典*/

#define ZONEERR_WEDDING25 19531
/* @@error 19531 你正在举行婚礼蜜游中,暂时无法举行婚礼庆典*/

#define ZONEERR_WEDDING26 19532
/* @@error 19532 似乎有一对新人正在举行婚礼巡游,请结束后再举行婚礼巡游*/

#define ZONEERR_WEDDING27 19533
/* @@error 19533 你正在举行婚礼庆典中,暂时无法举行婚礼蜜游*/

#define ZONEERR_WEDDING28 19534
/* @@error 19534 你身上未携带%s , 暂时无法解除关系*/

#define ZONEERR_WEDDING29 19535
/* @@error 19535 %s提出了分手,结束了这段刻骨铭心的感情!*/

#define ZONEERR_WEDDING30 19536
/* @@error 19536 对方身上无%s !*/

#define ZONEERR_WEDDING31 19537
/* @@error 19537 您现在已经有夫妻对象了,无法再订制婚礼套餐*/

#define ZONEERR_WEDDING32 19538
/* @@error 19538 你们还没订制婚礼套餐,不能开始婚礼*/

#define ZONEERR_WEDDING33 19539
/* @@error 19539 很遗憾你并没有伴侣*/

#define ZONEERR_WEDDING34 19540
/* @@error 19540 %s似乎并不在场,暂时无法举行婚礼蜜游*/

#define ZONEERR_WEDDING35 19541
/* @@error 19541 需要订制简约婚礼套餐或者豪华婚礼套餐才可以进行婚礼庆典*/

#define ZONEERR_WEDDING36 19542
/* @@error 19542 需要订制豪华婚礼套餐才可以进行婚礼蜜游*/

#define ZONEERR_WEDDING37 19543
/* @@error 19543 %s委婉拒绝了你的表白*/

#define ZONEERR_WEDDING38 19544
/* @@error 19544 %s拒绝了誓言的承诺,似乎仍未作好踏入婚姻的准备*/

#define ZONEERR_WEDDING39 19545
/* @@error 19545 你正在举行婚礼蜜游中,暂时无法解除关系*/

#define ZONEERR_WEDDING40 19546
/* @@error 19546 你正在举行婚礼庆典中,暂时无法解除关系*/

#define ZONEERR_WEDDING41 19547
/*@@error 19547  我以圣灵、圣父、圣子的名义宣布:%s与%s结为夫妻.现在,新郎可以亲吻新娘了*/

#define ZONEERR_WEDDING42 19548
/* @@error 19548 今天我们聚集,在上帝和来宾的面前,见证%s与%s这对新人神圣的婚礼*/

#define ZONEERR_WEDDING43 19549
/* @@error 19549 %s与%s在阳光海滩举行了隆重的婚礼庆典!*/

#define ZONEERR_WEDDING44 19550
/* @@error 19550 %s与%s将搭载浪漫跑车甜蜜的环游时间城!*/

#define ZONEERR_WEDDING45 19551
/* @@error 19551 在一片欢声笑语中,%s与%s的婚礼庆典圆满结束!*/

#define ZONEERR_WEDDING46 19552
/* @@error 19552 伴随着大家的祝福,%s与%s的婚礼蜜游圆满结束!*/

#define ZONEERR_WEDDING47 19553
/* @@error 19553 %s 接受了%s的追求,从此成为一对相亲相爱的情侣!*/

#define ZONEERR_WEDDING48 19554
/* @@error 19554 您已经有伴侣了*/

#define ZONEERR_WEDDING49 19555
/* @@error 19555 您当前所在的地图暂时无法传送*/

#define ZONEERR_WEDDING50 19556
/* @@error 19556 您暂时无法传送到对方所在的地图*/

#define ZONEERR_WEDDING51 19557
/* @@error 19557 很遗憾您仍然单身!*/

#define ZONEERR_WEDDING52 19558
/* @@error 19558 您们已经举行过了婚礼庆典，无法再举行婚礼庆典!*/

#define ZONEERR_WEDDING53 19559
/* @@error 19559 您们已经举行过了婚礼蜜游，无法再举行婚礼蜜游!*/

#define ZONEERR_WEDDING54 19560
/* @@error 19560 您向%s提出了分手，结束了这段刻骨铭心的感情!*/

#define ZONEERR_WEDDING55 19561
/* @@error 19561 %s只有建立情侣关系后才能穿戴!*/

#define ZONEERR_WEDDING56 19562
/* @@error 19562 %s只有建立夫妻关系后才能穿戴!*/

#define ZONEERR_WEDDING57 19563
/* @@error 19563 %s只有建立情侣或夫妻关系后才能穿戴!*/

#define ZONEERR_WEDDING58 19564
/* @@error 19564 您和%s的亲密度增加了%d点!*/

#define ZONEERR_WEDDING59 19565
/* @@error 19565 您和%s的亲密度减少了%d点!*/

#define ZONEERR_WEDDING60 19566
/* @@error 19566 暂时无法成为情侣，需要双方亲密度达到%d*/

#define ZONEERR_WEDDING61 19567
/* @@error 19567 %s成功订制了裸婚套餐，可向神父申请举行该套餐的婚礼*/

#define ZONEERR_WEDDING62 19568
/* @@error 19568 %s成功订制了简约套餐，可向神父申请举行该套餐的婚礼*/

#define ZONEERR_WEDDING63 19569
/* @@error 19569 %s成功订制了豪华套餐，可向神父申请举行该套餐的婚礼*/

#define ZONEERR_WEDDING64 19570
/* @@error 19570 你交出了记载着曾经的那枚%s，开始了一段新的婚姻*/

#define ZONEERR_WEDDING65 19571
/* @@error 19571 开始婚礼的宣誓仪式前，需要%s 出示订制婚礼时的%s*/

#define ZONEERR_WEDDING66 19572
/* @@error 19572 你和%s的亲密度已满，暂时不需要增加亲密度*/

#define ZONEERR_WEDDING67 19573
/* @@error 19573 %s正在向你表白，无法向对方赠送%s*/

#define ZONEERR_WEDDING68 19574
/* @@error 19574 %s已经找到了情侣，无法再接受您的回应*/

#define ZONEERR_WEDDING69 19575
/* @@error 19575 %s已经找到了伴侣，无法再接受您的回应*/

#define ZONEERR_WEDDING70 19576
/* @@error 19576 您已经找到了情侣，无法再接受%s的表白*/

#define ZONEERR_WEDDING71 19577
/* @@error 19577 您已经找到了伴侣，无法再接受%s的表白*/

#define ZONEERR_WEDDING72 19578
/* @@error 19578 你的伴侣正身处思过厅中，暂时无法使用该技能传送过去*/

#define ZONEERR_WEDDING73 19579
/* @@error 19579 你的伴侣不在身边，暂时无法使用该技能*/

#define ZONEERR_WEDDING74 19580
/* @@error 19580 你现在正处于战场中，暂时无法使用该技能*/

#define ZONEERR_WEDDING75 19581
/* @@error 19581 跑车由太阳能驱动，无需填充能量*/

#define ZONEERR_WEDDING76 19582
/* @@error 19582 你们当前为情侣，无法将对方列入你的黑名单中*/

#define ZONEERR_WEDDING77 19583
/* @@error 19583 你们当前为伴侣，无法将对方列入你的黑名单中*/

#define ZONEERR_WEDDING78 19584
/* @@error 19584 你的伴侣已不幸牺牲，暂时无法使用该技能*/

#define ZONEERR_WEDDING79 19585
/* @@error 19585 你正身处思过厅中，暂时无法使用该技能传送过去*/

#define ZONEERR_WEDDING80 19586
/* @@error 19586 %s并非你的好友，暂时无法接受对方的表白*/

#define ZONEERR_WEDDING81 19587
/* @@error 19587 你并非%s的好友，暂时无法接受对方的表白*/

#define ZONEERR_WEDDING82 19588
/* @@error 19588 你与%s的距离太远，暂时无法接受对方的表白*/

#define ZONEERR_WEDDING83 19589
/* @@error 19589 你恢复了单身生活，订制的套餐清单已废弃回收*/

#define ZONEERR_WEDDING84 19590
/* @@error 19590 开启一段新的婚姻，需要收回%s的%s，请把戒指携带在背包中*/

#define ZONEERR_RONGYAN1 19600
/* @@error 19600 太可惜了，你选择的boss是个冒牌货。把你的愤怒都发泄在这个冒牌货上吧，鞭笞他蹂躏他！！*/

#define ZONEERR_RONGYAN2 19601
/* @@error 19601 队伍解散，熔岩巨兽活动失败*/

#define ZONEERR_RONGYAN3 19602
/* @@error 19602 未能在熔岩巨兽存活时间内击杀，活动失败*/

#define ZONEERR_RONGYAN4 19603
/* @@error 19603 消耗道具:%sx%d!*/

#define ZONEERR_RONGYAN5 19604
/* @@error 19604 此装备不能提炼!*/

#define ZONEERR_RONGYAN6 19605
/* @@error 19605 你不在此队列中,无法进行此操作*/

#define ZONEERR_RONGYAN7 19606
/* @@error 19606 你不在公会队列中,无法进行此操作*/

#define ZONEERR_RONGYAN8 19607
/* @@error 19607 公会队列已不存在,无法进行此操作*/

#define ZONEERR_RONGYAN9 19608
/* @@error 19608 公会队列不是排队状态,无法进行此操作*/

#define ZONEERR_RONGYAN10 19609
/* @@error 19609 找不到对应的排队队列,无法进行此操作*/

#define ZONEERR_RONGYAN11 19610
/* @@error 19610 你没有权限进行此操作*/

#define ZONEERR_RONGYAN12 19611
/* @@error 19611 队列已满,无法报名*/

#define ZONEERR_RONGYAN13 19612
/* @@error 19612 未知的队列单位类型*/

#define ZONEERR_RONGYAN14 19613
/* @@error 19613 您当前迷之塔进度为:第%d层,不能进入第%d层!*/

#define ZONEERR_RONGYAN15 19614
 /* @@error 19614 机械能量消耗殆尽,请尽快补充能量!*/

#define ZONEERR_RONGYAN16 19615
/* @@error 19615 %s开始全力召唤终极熔岩巨兽，%d分钟后将出现在%s，参与击杀将获取巨额经验奖励*/

#define ZONEERR_RONGYAN17 19616
/* @@error 19616 在%s的带领下，成功击杀%d阶终极熔岩巨兽，所有参与者都获得了巨额经验*/

#define ZONEERR_RONGYAN18 19617
/* @@error 19617 今天已经不能再召唤终极熔岩巨兽了*/

#define ZONEERR_RONGYAN19 19618
/* @@error 19618 暂时不能召唤终极熔岩巨兽，请稍后召唤*/

#define ZONEERR_RONGYAN20 19619
/* @@error 19619 终极熔岩巨兽已经出现在%s，请勇士们前往击杀*/

#define ZONEERR_RONGYAN21 19620
/* @@error 19620 只能在12:00-24:00 之间召唤终极熔岩巨兽*/

#define ZONEERR_ARM_UNREAL1 19700
/* @@error 19700 幻化背饰空格中请放入不可升阶背饰*/

#define ZONEERR_ARM_UNREAL2 19701
/* @@error 19701 目标背饰空格中只能填入可升阶背饰*/

#define ZONEERR_ARM_UNREAL3 19702
/* @@error 19702 幻化成功!*/

#define ZONEERR_ARM_UNREAL4 19703
/* @@error 19703 装备已过期，不能进行此操作*/

#define ZONEERR_ARM_UNREAL5  19704
/* @@error 19704 背包中的装备才能幻化*/

#define ZONEERR_ARM_UNREAL6  19705
/* @@error 19705 只能填入背饰道具*/

#define ZONEERR_ARM_UNREAL7  19706
/* @@error 19706 幻化失败!*/

#define ZONEERR_ARM_LEVELUP1  19750
/* @@error 19750 进阶失败，此装备进阶需要%d级!*/

#define ZONEERR_PET_UNBIND 19800
/* @@error 19800 %s不足，不能进行宠物解绑操作*/

#define ZONEERR_SPAN_MAP1 19900
/* @@error 19900 您已进入跨服地图，此地图内战斗模式以服为单位！*/

#define ZONEERR_SPAN_MAP2 19901
/* @@error 19901 %s属于不同服务器或不在线，不能组队*/

#define ZONEERR_SPAN_STRONG1 20000
/* @@error 20000 跨服据点战之能由会长竞拍*/

#define ZONEERR_SPAN_STRONG2 20001
/* @@error 20001 竞拍公会数量达到上限!*/

#define ZONEERR_SPAN_STRONG3 20002
/* @@error 20002 你的公会已经竞拍过此据点了!*/

#define ZONEERR_SPAN_STRONG4 20003
/* @@error 20003 同时只能竞拍%d个跨服据点!*/

#define ZONEERR_SPAN_STRONG5 20004
/* @@error 20004 银币不足!竞拍失败!*/

#define ZONEERR_SPAN_STRONG6 20005
/* @@error 20005 您还未占领任何跨服据点，无法领取占领奖励!*/

#define ZONEERR_SPAN_STRONG7 20006
/* @@error 20006 占领奖励只有会长可领取!*/

#define ZONEERR_SPAN_STRONG8 20007
/* @@error 20007 今天的占领资金已领取,无法重复领取*/

#define ZONEERR_SPAN_STRONG9 20008
/* @@error 20008 领取成功!*/

#define ZONEERR_SPAN_STRONG10 20009
/* @@error 20009 非可参战战团成员!*/

#define ZONEERR_CLAN_PET01 20030
/* @@error 20030 由于工会降级,您寄养的宠物已被收回!*/

#define ZONEERR_CLAN_PET02 20031
/* @@error 20031 宠物已经最高成长等级了!*/

#define ZONEERR_CLAN_PET03 20032
/* @@error 20032 正在出战(合体)的宠物无法被寄养,请先将宠物召回!*/

#define ZONEERR_CLAN_PET04 20033
/* @@error 20033 您慢了一步,此栏位已有宠物入住,请选择其他栏位!*/

#define ZONEERR_CLAN_PET05 20034
/* @@error 20034 同时只能寄养一只宠物!*/

#define ZONEERR_CLAN_PET06 20035
/* @@error 20035 公会寄养栏位已满!*/

#define ZONEERR_CLAN_PET07 20036
/* @@error 20036 公会贡献度不足,无法寄养!*/

#define ZONEERR_CLAN_PET08 20037
/* @@error 20037 银币不足,无法寄养*/

#define ZONEERR_CLAN_PET09 20038
/* @@error 20038 高级寄养时间已满!*/

#define ZONEERR_CLAN_PET10 20039
/* @@error 20039 经验值不足,无法寄养!*/

#define ZONEERR_CLAN_PET11 20040
/* @@error 20040 您的宠物 %s 寄养成功!*/

#define ZONEERR_CLAN_PET12 20041
/* @@error 20041 该宠物已处于“中止寄养”的休眠状态，无法对其喂食、玩耍、引诱！!*/

#define ZONEERR_CLAN_PET13 20042
/* @@error 20042 背包空间不足!*/

#define ZONEERR_CLAN_PET14 20043
/* @@error 20043 为了感谢你的陪伴,%s 拿出一个宝箱送给你,它想和你做好朋友哦!*/

#define ZONEERR_CLAN_PET15 20044
/* @@error 20044 宠物欢乐度提升%d点!*/

#define ZONEERR_CLAN_PET16 20045
/* @@error 20045 取消寄养成功,剩余%d经验值已返还给您,欢迎您再次光临宠物乐园!*/

#define ZONEERR_CLAN_PET17 20046
/* @@error 20046 取消成功!*/

#define ZONEERR_CLAN_PET18 20047
/* @@error 20047 宠物劳动果实已放入您的背包中!*/

#define ZONEERR_CLAN_PET19 20048
/* @@error 20048 宠物欢乐度较低,不能进行工作,请先陪宠物玩耍一会儿吧!*/

#define ZONEERR_CLAN_PET20 20049
/* @@error 20049 时间太短了,请至少预留出90分钟让宠物完成一份工作!*/

#define ZONEERR_CLAN_PET21 20050
/* @@error 20050 宠物打工已经结束,请领取奖励!*/

#define ZONEERR_CLAN_PET22 20051
/* @@error 20051 宠物正在打工!*/

#define ZONEERR_CLAN_PET23 20052
/* @@error 20052 寄养没满60分钟不能引诱!*/

#define ZONEERR_CLAN_PET24 20053
/* @@error 20053 手下留情吧，它已经所剩无几了!*/

#define ZONEERR_CLAN_PET25 20054
/* @@error 20054 您已经引诱过它了哟，换只宠物再引诱吧!*/

#define ZONEERR_CLAN_PET26 20055
/* @@error 20055 银币不足,无法引诱!*/

#define ZONEERR_CLAN_PET27 20056
/* @@error 20056 消耗%d银币,引诱成功!*/

#define ZONEERR_CLAN_PET28 20057
/* @@error 20057 消耗%d银币,引诱成功!宠物欢乐较低,引诱结果翻倍!*/

#define ZONEERR_CLAN_PET29 20058
/* @@error 20058 消耗%d银币,引诱失败!宠物温饱度越低,越容易引诱成功哦!*/

#define ZONEERR_CLAN_PET30 20059
/* @@error 20059 宠物已经很饱了,吃不下更多东西了,请稍后再来喂食!*/

#define ZONEERR_CLAN_PET31 20060
/* @@error 20060 喂食成功,消耗 %s 食物1个!*/

#define ZONEERR_CLAN_PET32 20061
/* @@error 20061 宠物正在寄养中,请前往公会城市取消寄养才可进行其他操作*/

#define ZONEERR_CLAN_PET33 20062
/* @@error 20062 寄养还未结束!*/

#define ZONEERR_CLAN_PET34 20063
/* @@error 20063 宠物主人在线,你不能清理!*/

#define ZONEERR_CLAN_PET35 20064
/* @@error 20064 清理成功!*/

#define ZONEERR_HOME1 20080
/* @@error 20080 缺少道具:%s!*/

#define ZONEERR_CARERR_ALTER1 20200
/* @@error 20200 职业转换成功*/

#define ZONEERR_CARERR_ALTER2 20201
/* @@error 20201 你正在搭载蒸汽机械中，暂时无法转换职业*/

#define ZONEERR_CARERR_ALTER3 20202
/* @@error 20202 你正在使用蒸汽科技中，暂时无法转换职业*/

#define ZONEERR_CARERR_ALTER4 20203
/* @@error 20203 你的身上还穿戴着装备或时装，暂时无法转换职业*/

#define ZONEERR_CARERR_ALTER5 20204
/* @@error 20204 1小时内只能转换职业1次，暂时无法转换职业*/

#define ZONEERR_PET701 20205
/* @@error 20205  此天赋书不能给该出战的宠物使用，请选择对应的宠物！*/

#define ZONEERR_PET702 20206
/* @@error 20206  技能属性不能全部上锁!*/

#define ZONEERR_PET703 20207
/* @@error 20207  此装备没有技能属性!*/

#define ZONEERR_RUNE_1 20300
/* @@error 20300 该符文技能只能通过镶嵌#%s·%s#激活*/

#define ZONE_BOSS_BAOXING_1 20340
/* @@error 20340 等级必须达到%d级才能开启宝箱*/

#define ZONEERR_SHIKONG_1 20350
/* @@error 20350 已超过进入【%s】进入时间，请重新排队！*/

#define ZONEERR_SHIKONG_2 20351
/* @@error 20351 【%s】排队已取消！*/

#define ZONEERR_SHIKONG_3 20352
/* @@error 20352 您不在等待队列中，无法进入*/

#define ZONEERR_SHIKONG_4 20353
/* @@error 20353 交易状态不能进入*/

#define ZONEERR_SHIKONG_5 20354
/* @@error 20354 摆摊状态不能进入*/

#define ZONEERR_SHIKONG_6 20355
/* @@error 20355 机械状态下不能进入*/

#define ZONEERR_SHIKONG_7 20356
/* @@error 20356 %s 等级不符合，无法进入*/

#define ZONEERR_SHIKONG_8 20357
/* @@error 20357 %s已报名末世荣耀殿堂，无法进行报名*/

#define ZONEERR_SHIKONG_9 20358
/* @@error 20358 %s 为思过厅中黑名玩家，不能报名*/

#define ZONEERR_SHIKONG_10 20359
/* @@error 20359 %s正在末世巅峰准备地图中或巅峰竞技副本中, 无法报名*/

#define ZONEERR_SHIKONG_11 20360
/* @@error 20360 你的等级不符合，无法报名*/

#define ZONEERR_SHIKONG_12 20361
/* @@error 20361 报名成功!*/

#define ZONEERR_SHIKONG_13 20362
/* @@error 20362 已报名末世荣耀殿堂，无法进行报名*/

#define ZONEERR_SHIKONG_14 20363
/* @@error 20363 %s不在本地图，报名失败!*/

#define ZONEERR_SHIKONG_15 20364
/* @@error 20364 战斗状态不能进入*/


#define ZONEERR_HOME_01 20400
/* @@error 20400 土地未开垦或者已经种植了植物*/

#define ZONEERR_HOME_02 20401
/* @@error 20401 今天已经领取过了*/

#define ZONEERR_HOME_03 20402
/* @@error 20402 背包空间不足*/

#define ZONEERR_HOME_04 20403
/* @@error 20403 请在自己家园操作*/

#define ZONEERR_HOME_05 20404
/* @@error 20404 银币不够!*/

#define ZONEERR_HOME_06 20405
/* @@error 20405 %s 已将家园设置为关门!*/

#define ZONEERR_HOME_07 20406
/* @@error 20406 你已经拥有了一个家园!*/

#define ZONEERR_HOME_08 20407
/* @@error 20407 %s 已经取消出售!*/

#define ZONEERR_HOME_09 20408
/* @@error 20408 宠物饱食度达到上限!*/

#define ZONEERR_HOME_10 20409
/* @@error 20409 宠物清洁度达到上限!*/

#define ZONEERR_HOME_11 20410
/* @@error 20410 宠物愉快度达到上限 !*/

#define ZONEERR_HOME_12 20411
/* @@error 20411 宠物健康度达到上限!*/

#define ZONEERR_HOME_13 20412
/* @@error 20412 当前等级菜地对应的开垦数达到上限!*/

#define ZONEERR_HOME_14 20413
/* @@error 20413 宠物等级太低,无法收获!*/

#define ZONEERR_HOME_15 20414
/* @@error 20414 当前菜地等级不够，无法开垦*/

#define ZONEERR_HOME_16 20415
/* @@error 20415 菜地等级不够,无法种植该种子*/

#define ZONEERR_SUBJECT_01 20500
/* @@error 20500 背包已满，智力竞赛的奖励已通过邮件的方式发送给您，请注意查收 */

#define ZONEERR_SUBJECT_02 20501
/* @@error 20501 恭喜你获得第%d名,获得%d个%s*/

#define ZONEERR_SUBJECT_03 20502
/* @@error 20502 恭喜玩家%s在智力竞赛中获得第一名,奖励%d个%s*/

#define ZONEERR_SUBJECT_04 20503
/* @@error 20503 已存在经验加成的道具效果，暂时不能使用该道具*/

#define ZONEERR_RUNE231 20510
/* @@error 20510 角色达到%d级，可激活该符文技能*/


#ifdef __cplusplus
extern "C"
{
#endif

	HERR err_init(const char* pszPath, const char* pszLang, int iFlags);
	void err_release(void);

#ifdef __cplusplus
}
#endif

#endif /* ZONE_ERR_H */


