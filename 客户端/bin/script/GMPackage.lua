
--------------------------------------------------------------------------------------------------
--快捷指令说明
--------------------------------------------------------------------------------------------------

--17守护 18聚能 19猎杀 20圣佑 33征服 34炼魂 35毁灭 36驭雷

--//cmdpg0 	--加速
--//cmdpg11 --常用必备
--//cmdpg1  --加速
--//cmdpg2  --取消牛xbuff
--//cmdpg3  --各种牛xbuff
--//cmdpg6  --各种钱
--//cmdpgbs1 --1级宝石
--//cmdpgbs2 --2级宝石
--......
--//cmdpgbs7,   --7级宝石

--//cmdpg8  gm套装
--//cmdpgzs1 战士紫装散件	其他职业雷同
--//cmdpgzs2 战士副本套装  	其他职业雷同
--//cmdpgzs3 战士战场套装  	其他职业雷同
--//cmdpgzs4 战士boss套装  	其他职业雷同

--//cmdpg80sh   --80级守护装备  前两字母简称
--//cmdpg80zf	--80级征服装备
--//cmdpg80jn	--80级聚能装备
--......
--//cmdpg80ss   --80级首饰

--//cmdpgfm    --附魔炼金卷轴

--"//cmdpghg",			--回归礼包相关

--//cmdpgpf",   		--配方
--//cmdpgfs1
--//cmdpgfs2
--//cmdpgjs1
--//cmdpgjs2
--//cmdpgqs1
--//cmdpgqs2

--//cmdpgss1  --60级首饰
--//cmdpgss2  --70级首饰

--"//cmdpgqh",			--装备强化

--//cmdpgsz   --时装
--//cmdpgzq   --坐骑
--//cmdpgpet   --部分宠物
--//cmdpgcw   --宠物相关道具

--//cmdpgg  增加积累数

--//cmdpch  增加称号

--//cmdpglv 快速升级，转职

--//cmdpggw  召唤标准怪物

--//cmdpgskill  各种转职技能书

--//cmdpgjz1  各种戒指1
--//cmdpgjz2  各种戒指2

--//cmdpghf1  各种护符1

--//cmdpgxl1  各种项链1

--//cmdpgms1  各种面饰1
--//cmdpgms2  各种面饰2
--//cmdpgms3  各种面饰3

--//cmdpgsj  各种赏金令3
--//cmdpgrl  各种熔炉相关

--//cmdpgcz,    --属性重置齿轮

--//cmdpgbb  快速开背包

--"//cmdpgfw",			--符文
--"//cmdpgjl",			--精灵
--"//cmdpgjx",			--机械

--//cmdpgtest (%d+)    --给目标（ID）上各种状态

--//cmdpgcl  --各种材料

--//cmdpgry  --各种荣誉，声望

--//cmdpgvip  --各种vip
--------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------


-------------------------------------------------
-- g_nCAREER_WARRIOR = 1;  --战士
-- g_nCAREER_MAGICOR = 2;  --法师
-- g_nCAREER_GUNER   = 3;	--枪手
-- g_nCAREER_CHURCH  = 4;	--牧师
-------------------------------------------------


local nTEQid = 
{
	{},		--战士
	{},		--法师
	{},		--枪手
	{},		--牧师

}


local cmdTable =
{
	{
		"//cmdpgtest (%d+)",
		0,
		0,
		120,
		"//applystatus %1 1017 1 100000",
		"//applystatus %1 1005 1 100000",
		"//applystatus %1 1005 2 100000",
		"//applystatus %1 1005 3 100000",
		"//applystatus %1 1005 4 100000",
		"//applystatus %1 1202 1 100000",
		"//applystatus %1 1202 2 100000",
	},
	
	{
		"//cmdpglv0",
		0,
		2,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//updateskill 9999 1",
		"//updateskill 9996 1",

	},
	
	{
		"//cmdpg999",
		0,
		1,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//reloadggpackage",

	},
	
	{
		"//cmdpgry",				--各种声望荣誉
		0,
		9,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//respact add 100 10000",
		"//respact add 300 10000",
		"//respact add 301 10000",
		"//respact add 400 10000",
		"//addattr glory=100000",
		"//addattr kuansha=1000",
		"//addattr lvzhou=1000",
		"//addattr schoolval=10000",
		"//addattr sparta=10000",	-- 增加殿堂荣誉

	},	

	{
		"//cmdpgvip",				--各种vip
		0,
		16,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		
		"//additem 5030120	2",--水晶徽章特权[24小时]
		"//additem 5030121	2",--水晶徽章特权卡[3天]
		"//additem 5030122	2",--水晶徽章特权卡[10天]
		"//additem 5030123	2",--水晶会长特权卡[30天]
		"//additem 5030124	2",--翡翠徽章特权卡[24小时]
		"//additem 5030125	2",--翡翠徽章特权卡[3天]
		"//additem 5030126	2",--翡翠徽章特权卡[10天]
		"//additem 5030127	2",--翡翠会长特权卡[30天]
		"//additem 5030128	2",--火焰徽章特权卡[24小时]
		"//additem 5030129	2",--火焰徽章特权卡[3天]
		"//additem 5030130	2",--火焰徽章特权卡[10天]
		"//additem 5030131	2",--火焰会长特权卡[30天]
		"//additem 5030132	2",--银星徽章特权卡[24小时]
		"//additem 5030133	2",--银星徽章特权卡[3天]
		"//additem 5030134	2",--银星徽章特权卡[10天]
		"//additem 5030135	2",--银星会长特权卡[30天]

	},	

	
	{
		"//cmdpgch",    --称号
		0,
		30,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//dsn add 138",
		"//dsn add 139",
		"//dsn add 140",
		"//dsn add 141",
		"//dsn add 142",
		"//dsn add 143",
		"//dsn add 144",
		"//dsn add 145",
		"//dsn add 146",
		"//dsn add 147",
			
		"//dsn add 148",
		"//dsn add 149",
		"//dsn add 150",
		"//dsn add 151",
		"//dsn add 152",
		"//dsn add 153",
		"//dsn add 154",
		"//dsn add 155",
		"//dsn add 156",
		"//dsn add 157",
		
		"//dsn add 158",
		"//dsn add 159",
		"//dsn add 160",
		"//dsn add 161",
		"//dsn add 162",
		"//dsn add 163",
		"//dsn add 164",
		"//dsn add 165",
		"//dsn add 166",
		"//dsn add 167",
	
	},
	
	{
		"//cmdpgpf",   		--配方
		0,
		87,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//paklarge 3 96",
		"//additem 5254006 1",  --铸造
		"//additem 5254007 1",
		"//additem 5254012 1",
		"//additem 5254022 1",
		"//additem 5254023 1",
		"//additem 5254027 1",
		"//additem 5254043 1",
		"//additem 5254044 1",
		"//additem 5254048 1",
		"//additem 5254059 1",
		"//additem 5254087 1",
		"//additem 5254088 1",
		"//additem 5254091 1",
		"//additem 5254092 1",
		"//additem 5254243 1",
		"//additem 5254269 1",
		"//additem 5254270 1",
		"//additem 5254271 1",
		"//additem 5254272 1",
		"//additem 5254273 1",
		"//additem 5254274 1",
		"//additem 5254275 1",
		"//additem 5254276 1",
		"//additem 5254277 1",
		
		"//additem 5255003 1" ,  --炼金
		"//additem 5255017 1",
		"//additem 5255020 1",
		"//additem 5255023 1",
		"//additem 5255036 1",
		"//additem 5255037 1",
		"//additem 5255041 1",
		"//additem 5255053 1",
		
		"//additem 5255075 1",  --附魔
		"//additem 5256007 1",
		"//additem 5256009 1",
		"//additem 5256014 1",
		"//additem 5256015 1",
		"//additem 5256018 1",
		"//additem 5256022 1",
		"//additem 5256023 1",
		"//additem 5256024 1",
		"//additem 5256031 1",
		"//additem 5256033 1",
		"//additem 5256037 1",
		"//additem 5256038 1",
		"//additem 5256041 1",
		"//additem 5256045 1",
		"//additem 5256046 1",
		"//additem 5256061 1",
		"//additem 5256063 1",
		"//additem 5256075 1",
		"//additem 5256076 1",
		"//additem 5256079 1",
		"//additem 5256083 1",
		"//additem 5256084 1",
		"//additem 5256217 1",
		"//additem 5256218 1",
		"//additem 5256240 1",
		"//additem 5256241 1",
		"//additem 5256246 1",
		"//additem 5256247 1",
		"//additem 5256248 1",
		"//additem 5256249 1",
		"//additem 5256250 1",
		"//additem 5256251 1",
		"//additem 5256252 1",
		"//additem 5256253 1",
		"//additem 5256265 1",
		"//additem 5256266 1",
		"//additem 5256267 1",
		"//additem 5256268 1",
		"//additem 5256269 1",
		"//additem 5256270 1",
		"//additem 5256271 1",
		"//additem 5256272 1",
		"//additem 5256278 1",
		"//additem 5256279 1",
		"//additem 5256284 1",
		"//additem 5256285 1",
		"//additem 5256286 1",
		"//additem 5256287 1",
		"//additem 5256288 1",
		"//additem 5256289 1",
		"//additem 5256290 1",
		"//additem 5256291 1",		
		
	},

	{
		"//cmdpg11",
		0,
		32,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//paklarge 3 96",
		"//repairall",
		"//applystatus 0 1005 1 7200",
		"//additem 5060002 10",
		"//additem 5060001 10",
		"//addattr money=10000000",
		"//addattr bull=10000000",
		"//additem 5041000 10",
		"//additem 5251000 7",
		"//additem 5251402 8",
		"//additem 5050200 99",
		"//updateskill 9999 1",
		"//additem 5011001 60",
		"//additem 5011002 60",
		"//additem 5011003 60",
		"//additem 5011004 60",
		"//additem 5010001 60",
		"//additem 5010002 60",
		"//additem 5010003 60",
		"//additem 5010004 60",
		"//additem 5000001 60",
		"//additem 5000002 60",
		"//additem 5000003 60",
		"//additem 5000004 60",
		"//additem 5001001 60",
		"//additem 5001002 60",
		"//additem 5001003 60",
		"//additem 5001004 60",
		"//additem 5063000 200",
		"//additem 5041030 200",
		"//additem 5030109 10",

	},
	
	{
		"//cmdpgbb",			--背包
		0,
		12,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//additem 5251000 7",
		"//additem 5251008 12",
		"//additem 5251001 100",
		"//additem 5251020 2",
		"//additem 5251021 2",
		"//additem 5251022 2",
		"//additem 5251023 2",
		"//additem 5251024 2",
		"//additem 5251025 2",
		"//additem 5251026 2",
		"//additem 5251027 2",
		
	},	
	
	{
		"//cmdpgrl",			--熔炉相关
		0,
		7,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//additem 5052316 100",
		"//additem 5052317 100",
		"//additem 5052318 100",
		"//additem 5052319 100",
		"//additem 5052328 100",
		"//additem 5052310 3000",

	},	
	
	{
		"//cmdpghg",			--回归礼包相关
		0,
		7,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//additem 5052170 3",
		"//additem 5052171 3",
		"//additem 5052172 3",
		"//additem 5052173 3",
		"//additem 5052174 3",
		"//additem 5052175 3",
		"//additem 5052176 3",
		"//additem 5052330 10",
		

	},		
	
	
	{
		"//cmdpgsj",			--赏金令
		0,
		31,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//paklarge 3 96",
		"//additem 5250300 5",
		"//additem 5250301 5",
		"//additem 5250302 5",
		"//additem 5250303 5",
		"//additem 5250304 5",
		"//additem 5250305 5",
		"//additem 5250306 5",
		"//additem 5250307 5",

		"//additem 5041001 2",--加急传送卷
		"//additem 5041001 10",--急传送卷
		
		"//additem 5041002 2",--公会城市传送卷
		"//additem 5041003 2",--队友召唤卷
		"//additem 5041004 2",--时间城传送阵
		"//additem 5041005 2",--牢城传送阵
		"//additem 5041006 2",--普罗圣地传送阵
		"//additem 5041007 2",--法老陵传送阵
		"//additem 5041008 2",--阿努比斯祭坛传送阵
		"//additem 5041009 2",--阿努比斯神殿传送阵
		"//additem 5041010 2",--阿努比斯废墟传送阵
		"//additem 5041011 2",--越雾甬道传送阵
		"//additem 5041012 2",--十三号矿洞传送阵
		"//additem 5041013 2",--传送药水
		"//additem 5041014 2",--空间记忆器Ⅲ
		"//additem 5041015 2",--空间记忆器Ⅳ
		"//additem 5041016 2",--空间记忆器Ⅴ
		"//additem 5041020 2",--追随队长
		"//additem 5041021 2",--绿洲队长召唤券
		"//additem 5041022 2",--公会集合券
		"//additem 5050001 22",--跨服喇叭
		"//additem 5062000 5",--古老藏宝图
		"//additem 5062003 5",--古老藏宝图
		"//additem 5062009 5",--古老藏宝图
		"//additem 5052940 5",--公会资金券
		"//additem 5052941 5",--公会资金券
		"//additem 5052942 5",--公会资金券
		"//additem 5052943 5",--公会资金券
		"//additem 5052944 5",--公会资金券
		"//additem 5052945 5",--公会资金券
		
	
	},		
	
	
	{
		"//cmdpgfw",			--符文
		0,
		14,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//additem 5251205 20",
		"//additem 5251204 20",
		"//additem 5251203 20",
		"//additem 5251202 20",
		"//additem 5251207 20",
		"//additem 5251208 20",
		"//additem 5251209 20",
		"//additem 5251210 20",
		"//additem 5251211 20",
		"//additem 5251212 20",
		"//additem 5251213 20",
		"//additem 5251214 20",
		"//additem 5251215 20",

	},
	
	{
		"//cmdpgjs",			--提炼晶石
		0,
		17,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//additem 5250400 100",		--提炼器I
		"//additem 5250401 100",		--提炼器I
		"//additem 5250402 100",		--提炼器I
		"//additem 5250403 100",		--提炼器I
		"//additem 5250404 100",		--提炼器I
		"//additem 5250405 200",		--提炼器I
		
		"//additem 5241510 20",			--鉴定仪
		"//additem 5241511 20",			--鉴定仪
		"//additem 5241512 20",			--鉴定仪
		"//additem 5241513 20",			--鉴定仪
		"//additem 5241514 20",			--鉴定仪
		"//additem 5241515 20",			--鉴定仪
		
		"//additem 5200050 100",			--转移石
		"//additem 5200051 100",			--转移石
		"//additem 5200052 100",			--转移石
		"//additem 5200053 100",			--转移石
		
		"//additem 5200054 100",			--高阶转移石
		"//additem 5200055 100",			--转移石
		"//additem 5200056 100",			--转移石
		"//additem 5200057 100",			--转移石

	},
		
	{
		"//cmdpgpet",			--宠物
		0,
		72,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//additem 5610100 1",
		"//additem 5610101 1",
		"//additem 5610102 1",
		"//additem 5610103 1",
		"//additem 5610104 1",
		"//additem 5610105 1",
		"//additem 5610106 1",
		"//additem 5610107 1",
		"//additem 5610108 1",
		"//additem 5610109 1",
		
		"//additem 5610110 1",
		"//additem 5610111 1",
		"//additem 5610112 1",
		"//additem 5610113 1",
		"//additem 5610200 1",
		"//additem 5610201 1",
		"//additem 5610202 1",
		"//additem 5610203 1",
		"//additem 5610208 1",
		"//additem 5610209 1",
		
		"//additem 5610210 1",
		"//additem 5610211 1",
		"//additem 5610212 1",
		"//additem 5610213 1",
		"//additem 5610214 1",
		"//additem 5610215 1",
		"//additem 5610216 1",
		"//additem 5610217 1",
		"//additem 5610218 1",
		"//additem 5610219 1",
		
		"//additem 5610300 1",
		"//additem 5610301 1",
		"//additem 5610303 1",
		"//additem 5610304 1",
		"//additem 5610305 1",
		"//additem 5610411 1",
		"//additem 5610412 1",
		"//additem 5610413 1",
		"//additem 5611100 1",
		
		"//additem 5611101 1",
		"//additem 5611102 1",
		"//additem 5611103 1",
		"//additem 5611104 1",
		"//additem 5611105 1",
		"//additem 5611106 1",
		"//additem 5611107 1",
		"//additem 5611108 1",
		"//additem 5611109 1",
		"//additem 5611110 1",
		
		"//additem 5611111 1",
		"//additem 5611112 1",
		"//additem 5611113 1",
		"//additem 5611116 1",
		"//additem 5611117 1",
		"//additem 5611118 1",
		"//additem 5611119 1",
		"//additem 5611120 1",
		"//additem 5611121 1",
		
		"//additem 5610416 1",
		"//additem 5611416 1",
		"//additem 5610142 1",
		"//additem 5610150 1",
		"//additem 5610151 1",
		"//additem 5610152 1",
		"//additem 5610153 1",
		"//additem 5610154 1",
		"//additem 5610155 1",
		"//additem 5610156 1",
		"//additem 5610157 1",
		"//additem 5610158 1",



		

	},	
	
	{
		"//cmdpgzq",			--坐骑
		0,
		35,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//additem 5600200 1",
		"//additem 5600206 1",
		"//additem 5600207 1",
		"//additem 5600208 1",
		"//additem 5600209 1",
		"//additem 5600210 1",
		"//additem 5600211 1",
		"//additem 5600212 1",
		"//additem 5600213 1",
		"//additem 5600214 1",
		"//additem 5600215 1",
		"//additem 5600216 1",
		"//additem 5600217 1",
		"//additem 5600218 1",
		"//additem 5600219 1",
		"//additem 5600300 1",
		"//additem 5600301 1",
		"//additem 5600302 1",
		"//additem 5600000 1",
		"//additem 5600305 1",
		"//additem 5600306 1",
		"//additem 5600307 1",
		"//additem 5600308 1",
		"//additem 5600226 1",
		"//additem 5600227 1",
		"//additem 5600225 1",	    --烈纹-D
		"//additem 5600309 1",        --跑车灰
		"//additem 5600310 1",        --跑车深蓝
		"//additem 5600311 1",        --跑车黑
		"//additem 5600312 1",        --跑车黄
		"//additem 5600313 1",        --跑车深灰
		"//additem 5600314 1",        --跑车绿
		"//additem 5600315 1",        --跑车红		
		"//additem 5234000 200",

	},	

	
	{
		"//cmdpgjl",			--精灵		0,
		0,
		7,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//additem 5620000 1",
		"//additem 5620001 1",
		"//additem 5620004 1",
		"//additem 5620002 1",
		"//additem 5620003 1",
		"//additem 5620005 1",
		"//additem 5251115 1111",
	},
		
	{
		"//cmdpgms1", 			--面饰1
		0,
		102,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//paklarge 3 96",
		"//additem 2411601 1",
		"//additem 2411602 1",
		"//additem 2411603 1",
		"//additem 2413002 1",
		"//additem 2413003 1",
		"//additem 2413004 1",
		"//additem 2413005 1",
		"//additem 2413006 1",
		"//additem 2413007 1",
		"//additem 2413008 1",

		"//additem 2413009 1",
		"//additem 2413010 1",
		"//additem 2413011 1",
		"//additem 2413012 1",
		"//additem 2413013 1",
		"//additem 2414002 1",
		"//additem 2414003 1",
		"//additem 2414004 1",
		"//additem 2414005 1",
		"//additem 2414006 1",

		"//additem 2414007 1",
		"//additem 2414008 1",
		"//additem 2414009 1",
		"//additem 2414010 1",
		"//additem 2414011 1",
		"//additem 2414012 1",
		"//additem 2414013 1",
		"//additem 2415002 1",
		"//additem 2415003 1",
		"//additem 2415004 1",

		"//additem 2415005 1",
		"//additem 2415006 1",
		"//additem 2415007 1",
		"//additem 2415008 1",
		"//additem 2415009 1",
		"//additem 2415010 1",
		"//additem 2415011 1",
		"//additem 2415012 1",
		"//additem 2415013 1",
		"//additem 2417001 1",

		"//additem 2417002 1",
		"//additem 2417003 1",
		"//additem 2417004 1",
		"//additem 2417005 1",
		"//additem 2417006 1",
		"//additem 2417007 1",
		"//additem 2417008 1",
		"//additem 2417009 1",
		"//additem 2417010 1",
		"//additem 2417027 1",

		"//additem 2417028 1",
		"//additem 2417029 1",
		"//additem 2417030 1",
		"//additem 2418003 1",
		"//additem 2418004 1",
		"//additem 2418005 1",
		"//additem 2418006 1",
		"//additem 2418007 1",
		"//additem 2418008 1",
		"//additem 2418009 1",

		"//additem 2418010 1",
		"//additem 2418011 1",
		"//additem 2418012 1",
		"//additem 2418103 1",
		"//additem 2418104 1",
		"//additem 2418105 1",
		"//additem 2418106 1",
		"//additem 2418107 1",
		"//additem 2418108 1",
		"//additem 2418109 1",

		"//additem 2418110 1",
		"//additem 2418111 1",
		"//additem 2418112 1",
		"//additem 2418203 1",
		"//additem 2418204 1",
		"//additem 2418205 1",
		"//additem 2418206 1",
		"//additem 2418207 1",
		"//additem 2418208 1",
		"//additem 2418209 1",

		"//additem 2418210 1",
		"//additem 2418211 1",
		"//additem 2418212 1",
		"//additem 2418303 1",
		"//additem 2418304 1",
		"//additem 2418305 1",
		"//additem 2418306 1",
		"//additem 2418307 1",
		"//additem 2418308 1",
		"//additem 2418309 1",

		"//additem 2418310 1",
		"//additem 2418311 1",
		"//additem 2418312 1",
		"//additem 2419902 1",
		"//additem 2419903 1",
		"//additem 2419904 1",
		"//additem 2419905 1",
		"//additem 2419906 1",
		"//additem 2419907 1",
		"//additem 2419908 1",
	},
	
	{
		"//cmdpgms2",			--面饰2
		0,
		102,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//paklarge 3 96",
		"//additem 2419909 1",
		"//additem 2419910 1",
		"//additem 2419911 1",
		"//additem 2419912 1",
		"//additem 2419913 1",
		"//additem 2421601 1",
		"//additem 2421602 1",
		"//additem 2421603 1",
		"//additem 2423002 1",
		"//additem 2423003 1",

		"//additem 2423004 1",
		"//additem 2423005 1",
		"//additem 2423006 1",
		"//additem 2423007 1",
		"//additem 2423008 1",
		"//additem 2423009 1",
		"//additem 2423010 1",
		"//additem 2423011 1",
		"//additem 2423012 1",
		"//additem 2423013 1",

		"//additem 2424002 1",
		"//additem 2424003 1",
		"//additem 2424004 1",
		"//additem 2424005 1",
		"//additem 2424006 1",
		"//additem 2424007 1",
		"//additem 2424008 1",
		"//additem 2424009 1",
		"//additem 2424010 1",
		"//additem 2424011 1",

		"//additem 2424012 1",
		"//additem 2424013 1",
		"//additem 2425002 1",
		"//additem 2425003 1",
		"//additem 2425004 1",
		"//additem 2425005 1",
		"//additem 2425006 1",
		"//additem 2425007 1",
		"//additem 2425008 1",
		"//additem 2425009 1",

		"//additem 2425010 1",
		"//additem 2425011 1",
		"//additem 2425012 1",
		"//additem 2425013 1",
		"//additem 2427001 1",
		"//additem 2427002 1",
		"//additem 2427003 1",
		"//additem 2427004 1",
		"//additem 2427005 1",
		"//additem 2427006 1",

		"//additem 2427007 1",
		"//additem 2427008 1",
		"//additem 2427009 1",
		"//additem 2427010 1",
		"//additem 2427027 1",
		"//additem 2427028 1",
		"//additem 2427029 1",
		"//additem 2427030 1",
		"//additem 2429902 1",
		"//additem 2429903 1",

		"//additem 2429904 1",
		"//additem 2429905 1",
		"//additem 2429906 1",
		"//additem 2429907 1",
		"//additem 2429908 1",
		"//additem 2429909 1",
		"//additem 2429910 1",
		"//additem 2429911 1",
		"//additem 2429912 1",
		"//additem 2429913 1",

		"//additem 2431601 1",
		"//additem 2431602 1",
		"//additem 2431603 1",
		"//additem 2433002 1",
		"//additem 2433003 1",
		"//additem 2433004 1",
		"//additem 2433005 1",
		"//additem 2433006 1",
		"//additem 2433007 1",
		"//additem 2433008 1",

		"//additem 2433009 1",
		"//additem 2433010 1",
		"//additem 2433011 1",
		"//additem 2433012 1",
		"//additem 2433013 1",
		"//additem 2434002 1",
		"//additem 2434003 1",
		"//additem 2434004 1",
		"//additem 2434005 1",
		"//additem 2434006 1",

		"//additem 2434007 1",
		"//additem 2434008 1",
		"//additem 2434009 1",
		"//additem 2434010 1",
		"//additem 2434011 1",
		"//additem 2434012 1",
		"//additem 2434013 1",
		"//additem 2435002 1",
		"//additem 2435003 1",
		"//additem 2435004 1",
	},	
			
	{
		"//cmdpgms3",			--面饰3
		0,
		102,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//paklarge 3 96",
		"//additem 2435005 1",
		"//additem 2435006 1",
		"//additem 2435007 1",
		"//additem 2435008 1",
		"//additem 2435009 1",
		"//additem 2435010 1",
		"//additem 2435011 1",
		"//additem 2435012 1",
		"//additem 2435013 1",
		"//additem 2437001 1",

		"//additem 2437002 1",
		"//additem 2437003 1",
		"//additem 2437004 1",
		"//additem 2437005 1",
		"//additem 2437006 1",
		"//additem 2437007 1",
		"//additem 2437008 1",
		"//additem 2437009 1",
		"//additem 2437010 1",
		"//additem 2437027 1",

		"//additem 2437028 1",
		"//additem 2437029 1",
		"//additem 2437030 1",
		"//additem 2439902 1",
		"//additem 2439903 1",
		"//additem 2439904 1",
		"//additem 2439905 1",
		"//additem 2439906 1",
		"//additem 2439907 1",
		"//additem 2439908 1",

		"//additem 2439909 1",
		"//additem 2439910 1",
		"//additem 2439911 1",
		"//additem 2439912 1",
		"//additem 2439913 1",
		"//additem 2441601 1",
		"//additem 2441602 1",
		"//additem 2441603 1",
		"//additem 2443002 1",
		"//additem 2443003 1",

		"//additem 2443004 1",
		"//additem 2443005 1",
		"//additem 2443006 1",
		"//additem 2443007 1",
		"//additem 2443008 1",
		"//additem 2443009 1",
		"//additem 2443010 1",
		"//additem 2443011 1",
		"//additem 2443012 1",
		"//additem 2443013 1",

		"//additem 2444002 1",
		"//additem 2444003 1",
		"//additem 2444004 1",
		"//additem 2444005 1",
		"//additem 2444006 1",
		"//additem 2444007 1",
		"//additem 2444008 1",
		"//additem 2444009 1",
		"//additem 2444010 1",
		"//additem 2444011 1",

		"//additem 2444012 1",
		"//additem 2444013 1",
		"//additem 2445002 1",
		"//additem 2445003 1",
		"//additem 2445004 1",
		"//additem 2445005 1",
		"//additem 2445006 1",
		"//additem 2445007 1",
		"//additem 2445008 1",
		"//additem 2445009 1",

		"//additem 2445010 1",
		"//additem 2445011 1",
		"//additem 2445012 1",
		"//additem 2445013 1",
		"//additem 2447001 1",
		"//additem 2447002 1",
		"//additem 2447003 1",
		"//additem 2447004 1",
		"//additem 2447005 1",
		"//additem 2447006 1",

		"//additem 2447007 1",
		"//additem 2447008 1",
		"//additem 2447009 1",
		"//additem 2447010 1",
		"//additem 2447027 1",
		"//additem 2447028 1",
		"//additem 2447029 1",
		"//additem 2447030 1",
		"//additem 2449902 1",
		"//additem 2449903 1",

		"//additem 2449904 1",
		"//additem 2449905 1",
		"//additem 2449906 1",
		"//additem 2449907 1",
		"//additem 2449908 1",
		"//additem 2449909 1",
		"//additem 2449910 1",
		"//additem 2449911 1",
		"//additem 2449912 1",
		"//additem 2449913 1",
	},	
	
	{
		"//cmdpgsz1",
		0,
		120,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		
		--305	上衣
		--345	裤子
		--355	帽子
		--375	鞋子
		--385	手套

		"//packageclear 3",
		"//paklarge 3 96",
		
		"//additem 3050008 1",        --纷争上衣(女)(永久)
			"//additem 3450008 1",        --纷争裤子(女)(永久)
			"//additem 3750008 1",        --纷争鞋子(女)(永久)
			"//additem 3850008 1",        --纷争手套(女)(永久)
			"//additem 3051008 1",        --纷争上衣(男)(永久)
			"//additem 3451008 1",        --纷争裤子(男)(永久)
			"//additem 3751008 1",        --纷争鞋子(男)(永久)
			"//additem 3851008 1",        --纷争手套(男)(永久)
			"//additem 3550003 1",        --文艺贝雷帽(7天)
			"//additem 3050009 1",        --阳光派对上衣(女)(永久)
			"//additem 3450009 1",        --阳光派对裤子(女)(永久)
			"//additem 3750009 1",        --阳光派对鞋子(女)(永久)
			"//additem 3850009 1",        --阳光派对手套(女)(永久)
			"//additem 3051009 1",        --阳光派对上衣(男)(永久)
			"//additem 3451009 1",        --阳光派对裤子(男)(永久)
			"//additem 3751009 1",        --阳光派对鞋子(男)(永久)
			"//additem 3851009 1",        --阳光派对手套(男)(永久)
			"//additem 3550009 1",        --阳光圆顶帽(永久)
			"//additem 3050003 1",        --大侦探上衣(女)(7天)
			"//additem 3450003 1",        --大侦探裤子(女)(7天)
			"//additem 3750003 1",        --大侦探鞋子(女)(7天)
			"//additem 3850003 1",        --大侦探手套(女)(7天)
			"//additem 3051003 1",        --大侦探上衣(男)(7天)
			"//additem 3451003 1",        --大侦探裤子(男)(7天)
			"//additem 3751003 1",        --大侦探鞋子(男)(7天)
			"//additem 3851003 1",        --大侦探手套(男)(7天)
			"//additem 3550008 1",        --爵士礼帽(永久)
			"//additem 3050022 1",        --探长上衣(女)(30天)
			"//additem 3450022 1",        --探长裤子(女)(30天)
			"//additem 3750022 1",        --探长鞋子(女)(30天)
			"//additem 3850022 1",        --探长手套(女)(30天)
			"//additem 3051022 1",        --探长上衣(男)(30天)
			"//additem 3451022 1",        --探长裤子(男)(30天)
			"//additem 3751022 1",        --探长鞋子(男)(30天)
			"//additem 3851022 1",        --探长手套(男)(30天)
			"//additem 3550022 1",        --探长帽子(30天)
			"//additem 3050005 1",        --入侵者战铠(女)(7天)
			"//additem 3450005 1",        --入侵者护腿(女)(7天)
			"//additem 3750005 1",        --入侵者护胫(女)(7天)
			"//additem 3850005 1",        --入侵者护手(女)(7天)
			"//additem 3051005 1",        --入侵者战铠(男)(7天)
			"//additem 3451005 1",        --入侵者护腿(男)(7天)
			"//additem 3751005 1",        --入侵者护胫(男)(7天)
			"//additem 3851005 1",        --入侵者护手(男)(7天)
			"//additem 3550005 1",        --入侵者头盔(7天)
			"//additem 3050012 1",        --波点比基尼(女)(永久)
			"//additem 3450012 1",        --波点泳裤(女)(永久)
			"//additem 3750012 1",        --沙滩高跟鞋(女)(永久)
			"//additem 3451012 1",        --印花泳裤(男)(永久)
			"//additem 3751009 1",        --阳光派对鞋子(男)(永久)
					
			"//additem 3050018 1",        --战神上衣
			"//additem 3450018 1",        --战神裤子
			"//additem 3750018 1",        --战神鞋子
			"//additem 3850018 1",        --战神手套
			"//additem 3050019 1",        --魔导上衣
			"//additem 3450019 1",        --魔导裤子
			"//additem 3750019 1",        --魔导鞋子
			"//additem 3850019 1",        --魔导手套
			"//additem 3050020 1",        --神枪上衣
			"//additem 3450020 1",        --神枪裤子
			"//additem 3750020 1",        --神枪鞋子
			"//additem 3850020 1",        --神枪手套
			"//additem 3050021 1",        --教廷上衣
			"//additem 3450021 1",        --教廷裤子
			"//additem 3750021 1",        --教廷鞋子
			"//additem 3850021 1",        --教廷手套
			"//additem 3050036 1",        --异域上衣(永久)
			"//additem 3450036 1",        --异域裤子(永久)
			"//additem 3750036 1",        --异域鞋子(永久)
			"//additem 3850036 1",        --异域手套(永久)
			"//additem 3050037 1",        --沙鹰上衣(永久)
			"//additem 3450037 1",        --沙鹰裤子(永久)
			"//additem 3550037 1",        --沙鹰护盔(永久)
			"//additem 3750037 1",        --沙鹰鞋子(永久)
			"//additem 3850037 1",        --沙鹰手套(永久)
			"//additem 3050038 1",        --星际上衣(永久)
			"//additem 3450038 1",        --星际裤子(永久)
			"//additem 3550038 1",        --星际礼帽(永久)
			"//additem 3750038 1",        --星际鞋子(永久)
			"//additem 3850038 1",        --星际手套(永久)
			"//additem 3050039 1",        --喵呜上衣(永久)
			"//additem 3450039 1",        --喵呜裤子(永久)
			"//additem 3550039 1",        --喵呜耳帽(永久)
			"//additem 3750039 1",        --喵呜鞋子(永久)
			"//additem 3850039 1",        --喵呜手套(永久)
			"//additem 3050031 1",        --犀利上衣(女)
			"//additem 3450031 1",        --犀利裤子(女)
			"//additem 3550031 1",        --犀利帽子(女)
			"//additem 3750031 1",        --犀利鞋子(女)
			"//additem 3850031 1",        --犀利手套(女)
			"//additem 3050030 1",        --犀利上衣(男)
			"//additem 3450030 1",        --犀利裤子(男)
			"//additem 3550030 1",        --犀利假发(男)
			"//additem 3750030 1",        --犀利鞋子(男)
			"//additem 3850030 1",        --犀利手套(男)
			
			"//additem 3052000 1",        --城主长袍(7天)
			"//additem 3452000 1",        --城主长裤(7天)
			"//additem 3552000 1",        --城主护额(7天)
			"//additem 3752000 1",        --城主马靴(7天)
			"//additem 3852000 1",        --城主手套(7天)
			"//additem 3052001 1",        --城员紧身衣(7天)
			"//additem 3452001 1",        --城员紧身裤(7天)
			"//additem 3552001 1",        --城员通讯耳机(7天)
			"//additem 3752001 1",        --城员动力鞋(7天)
			"//additem 3852001 1",        --城员手套(7天)
			"//additem 3052003 1",        --杀戮者外套(7天)
			"//additem 3452003 1",        --杀戮者长裤(7天)
			"//additem 3552003 1",        --杀戮者面具(7天)
			"//additem 3752003 1",        --杀戮者板靴(7天)
			"//additem 3852003 1",        --杀戮者护手(7天)


	},
	
	{
		"//cmdpgsz2",				--新时装1
		0,
		70,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//paklarge 3 96",
		"//additem 3650002 1",        --天然呆(7天)
		"//additem 3650004 1",        --女王装背饰(7天)
		"//additem 3650007 1",        --铃铛猫(永久)
		"//additem 3650008 1",        --吐纸猫(14天)
		"//additem 3650017 1",        --情人节背饰(30天)
		"//additem 3651002 1",        --异形利爪(7天)
		"//additem 3651003 1",        --重弦(7天)
		"//additem 3651007 1",        --武生靠旗(永久)
		"//additem 3652000 1",        --时光暗翊(7天)
		"//additem 3652001 1",        --时光灰羽(7天)
		"//additem 3652003 1",        --杀戮之翼(7天)
		"//additem 3652004 1",        --漆黑之羽(7天)
		"//additem 3652005 1",        --腥红之翼(7天)
		"//additem 3653000 1",        --小浣熊(7天)
		"//additem 3653001 1",        --小章鱼奇奇(7天)
		"//additem 3653002 1",        --绅士企鹅(7天)
		"//additem 3653003 1",        --小浪人(7天)
		"//additem 3653004 1",        --疯狂小易(7天)
		"//additem 3653006 1",        --萌兜兜(7天)
		"//additem 3653007 1",        --海盗熊猫(7天)
		"//additem 3653008 1",        --天线绿小宝(7天)
		"//additem 3653025 1",        --星辰天使(永久)
		
		"//additem 3050013 38",        --圣诞上衣(7天)
		"//additem 3450013 39",        --圣诞裤子(7天)
		"//additem 3550013 40",        --圣诞帽子(7天)
		"//additem 3750013 41",        --圣诞鞋子(7天)
		"//additem 3850013 42",        --圣诞手套(7天)
		
		"//additem 3050016 43",        --春节上衣(30天)
		"//additem 3450016 44",        --春节裤子(30天)
		"//additem 3550016 45",        --春节头饰(30天)
		"//additem 3750016 46",        --春节鞋子(30天)
		"//additem 3850016 47",        --春节手套(30天)
		
		"//additem 3050017 48",        --情人节上衣(30天)
		"//additem 3450017 49",        --情人节裤子(30天)
		"//additem 3550017 50",        --情人节头饰(30天)
		"//additem 3650017 51",        --情人节背饰(30天)
		"//additem 3750017 52",        --情人节鞋子(30天)
		"//additem 3850017 53",        --情人节手套(30天)
		
		"//additem 3050046 99",        --猫女仆上衣(永久)
		"//additem 3450046 100",        --猫女仆裤子(永久)
		"//additem 3550046 101",        --猫女仆耳帽(永久)
		"//additem 3750046 102",        --猫女仆鞋子(永久)
		"//additem 3850046 103",        --猫女仆手套(永久)
		
		"//additem 3050047 104",        --吸血鬼上衣(永久)
		"//additem 3450047 105",        --吸血鬼裤子(永久)
		"//additem 3750047 106",        --吸血鬼鞋子(永久)
		"//additem 3850047 107",        --吸血鬼手套(永久)
		
		"//additem 3050048 108",        --魅上衣(永久)
		"//additem 3450048 109",        --魅裤子(永久)
		"//additem 3750048 110",        --魅鞋子(永久)
		"//additem 3850048 111",        --魅手套(永久)
		
		
	},
	
	{
		"//cmdpgcw",
		0,
		15,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//paklarge 3 96",
		
		"//additem 5233000 1111",   	--幻化果实
		"//additem 5233500 1111",		--基因果实
		"//additem 5219600 1111",		--遗忘果实
		"//additem 5219601 1111",		--训练果实
		"//additem 5233502 11",		--归元宝玉
		"//additem 5233503 100",		--进阶果实
		"//additem 5052211 10",		--进阶果实凝炼器
		"//additem 5233511 5000",		--奇异精粹
		"//additem 5233512 1000",		--进阶凝露
		"//additem 5230002 20",		--进阶果实
		"//additem 5230003 20",		--进阶果实
		"//additem 5230004 20",		--进阶果实
		"//additem 5230005 20",		--进阶果实

	},	
	
--	{
--		"//cmdpgcwzz",
--		0,
--		6,
--		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
--		
--			"//cmdpgcwzz0"
--			"//cmdpgcwzz1"
--			"//cmdpgcwzz2"
--			"//cmdpgcwzz3"
--			"//cmdpgcwzz4"
--	},	
	
	{
		"//cmdpgcwzz1",
		0,
		6,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		
			"//petaddgenius 0 hp=100",
			"//petaddgenius 0 atk=100",
			"//petaddgenius 0 physidef=100",
			"//petaddgenius 0 icedef=100",
			"//petaddgenius 0 flee=100",
			"//petaddgenius 0 hit=100",
	
	},
	
	{
		"//cmdpgcwzz2",
		0,
		24,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		
			"//petaddgenius 1 hp=100",
			"//petaddgenius 1 atk=100",
			"//petaddgenius 1 physidef=100",
			"//petaddgenius 1 icedef=100",
			"//petaddgenius 1 flee=100",
			"//petaddgenius 1 hit=100",
			
			"//petaddgenius 2 hp=100",
			"//petaddgenius 2 atk=100",
			"//petaddgenius 2 physidef=100",
			"//petaddgenius 2 icedef=100",
			"//petaddgenius 2 flee=100",
			"//petaddgenius 2 hit=100",
			
			"//petaddgenius 3 hp=100",
			"//petaddgenius 3 atk=100",
			"//petaddgenius 3 physidef=100",
			"//petaddgenius 3 icedef=100",
			"//petaddgenius 3 flee=100",
			"//petaddgenius 3 hit=100",
			
			"//petaddgenius 4 hp=100",
			"//petaddgenius 4 atk=100",
			"//petaddgenius 4 physidef=100",
			"//petaddgenius 4 icedef=100",
			"//petaddgenius 4 flee=100",
			"//petaddgenius 4 hit=100",
	
	},
	
	
	{
		"//cmdpgcwzz3",
		0,
		6,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		
			"//petaddgenius 2 hp=100",
			"//petaddgenius 2 atk=100",
			"//petaddgenius 2 physidef=100",
			"//petaddgenius 2 icedef=100",
			"//petaddgenius 2 flee=100",
			"//petaddgenius 2 hit=100",
			
	},
	
	{
		"//cmdpgcwzz4",
		0,
		6,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		
			"//petaddgenius 3 hp=100",
			"//petaddgenius 3 atk=100",
			"//petaddgenius 3 physidef=100",
			"//petaddgenius 3 icedef=100",
			"//petaddgenius 3 flee=100",
			"//petaddgenius 3 hit=100",
			
	},
	
	{
		"//cmdpgcwzz5",
		0,
		6,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		
			"//petaddgenius 4 hp=100",
			"//petaddgenius 4 atk=100",
			"//petaddgenius 4 physidef=100",
			"//petaddgenius 4 icedef=100",
			"//petaddgenius 4 flee=100",
			"//petaddgenius 4 hit=100",

	},
	
	{
		"//cmdpgcl",
		0,
		45,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//paklarge 3 96",
		"//additem 5102001 500",   --原生油
		"//additem 5102002 500",   --低构油
		"//additem 5102003 500",   --光合油		

		"//additem 5101001 500",   --原质矿
		"//additem 5101002 500",   --密英矿
		"//additem 5101003 500",   --热能矿

		"//additem 5103001 500",  --秘境精华
		"//additem 5103002 500",  --宁静精华
		"//additem 5103003 500",  --冷川精华
		"//additem 5103041 500",  --微光碎片	
		"//additem 5103042 500",  --强光碎片
		"//additem 5103043 500",  --幽光碎片
		"//additem 5103021 200",  --灰棱水晶
		"//additem 5103022 200",  --潮露水晶
		"//additem 5103023 200",  --寒冰水晶

		"//additem 5109501 500", -- 漏时之沙
		"//additem 5109502 500",  --地热石心
		"//additem 5109503 500",  --露苔泉眼
		"//additem 5109504 500",  --枯藤腐息
		"//additem 5109505 500",  --黑化魔盒
		"//additem 5109506 500",  --荒芜飞尘
		
		"//additem 5300832 1",  --熟练附魔师徽记
		"//additem 5300833 1",  --资深附魔师徽记
		"//additem 5300836 1",  --熟练炼金师徽记
		"//additem 5300837 1",  --资深炼金师徽记
		"//additem 5300840 1",  --熟练铸造师徽记
		"//additem 5300841 1",  --资深铸造师徽记
		
		"//additem 5109201 100",  --旷岩宝珠
		"//additem 5109202 100",  --旷岩宝珠
		"//additem 5109203 100",  --旷岩宝珠
		"//additem 5109204 100",  --旷岩宝珠
		
		"//additem 5060100 300",  --十字战勋碎片
		"//additem 5060101 300",  --骑士战勋碎片
		"//additem 5060102 300",  --佩剑战勋碎片
		"//additem 5060103 300",  --皇家战勋碎片
		
		"//additem 5060001 10",  --十字战勋
		"//additem 5060002 10",  --骑士战勋
		"//additem 5060004 10",  --佩剑战勋
		"//additem 5060005 10",  --皇家战勋
		
		"//additem 5060200 300",  --皇家战勋碎片
		"//additem 5060201 300",  --皇家战勋碎片
		"//additem 5060202 300",  --皇家战勋碎片
		"//additem 5060203 300",  --皇家战勋碎片
		
	},
	
	{
		"//cmdpgqh",
		0,
		11,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//paklarge 3 96",
		
		"//additem 5200020 8000",   	--强化石
		"//additem 5200034 1000",		--重铸4
		"//additem 5200044 1000",		--洗练5
		"//additem 5200053 11",		--转移1
		"//additem 5200056 11",		--转移1
		"//additem 5200100 5",		--转移1
		"//additem 5200101 5",		--转移1
		"//additem 5200102 5",		--转移1
		"//additem 5200103 5",		--转移1
	
	},	
	
	{
		"//cmdpgcz",    --属性重置齿轮
		0,
		5,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//paklarge 3 96",
		
		"//additem 5200100 100",   	--50级重置齿轮
		"//additem 5200101 100",   	--强化石
		"//additem 5200102 100",   	--强化石
		"//additem 5200103 100",   	--强化石

	
	},	
	
	{
		"//cmdpgqh1",
		0,
		16,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//paklarge 3 96",
		
		"//additem 5200021 3",   	--5200021	究极星辉宝钻·五阶
		"//additem 5200022 3",		--5200022	究极星辉宝钻·六阶
		"//additem 5200023 3",		--5200023	究极星辉宝钻·七阶
		"//additem 5200024 3",		--5200024	究极星辉宝钻·八阶
		"//additem 5200025 3",		--5200025	究极星辉宝钻·九阶
		"//additem 5200039 20",		--5200039	高阶流星重铸石
		"//additem 5200038 20",		--5200039	高阶熔岩重铸石
		"//additem 5200037 20",		--5200039	高阶孔雀重铸石
		"//additem 5200036 20",		--5200039	高阶云母重铸石
		"//additem 5200035 20",		--5200039	高阶石灰重铸石
		"//additem 5200049 100",		--5200049	高阶5级洗炼石
		"//additem 5200048 100",		--5200049	高阶4级洗炼石
		"//additem 5200047 100",		--5200049	高阶3级洗炼石
		"//additem 5200046 100",		--5200049	高阶2级洗炼石
		"//additem 5200045 100",		--5200049	高阶1级洗炼石
		"//additem 5200026 100",		--5200026	高阶星辉
									
	},
	
	{
		"//cmdpgfm",
		0,
		45,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//paklarge 3 96",
		
		"//additem 5240022 20",   	--秘卷:天资附魔
		"//additem 5240023 20",   	--秘卷:天资附魔
		"//additem 5240122 20",   	--秘卷:天资附魔
		"//additem 5240123 20",   	--秘卷:天资附魔
		"//additem 5240322 20",   	--秘卷:天资附魔
		"//additem 5240323 20",   	--秘卷:天资附魔
		"//additem 5240522 20",   	--秘卷:天资附魔
		"//additem 5240523 20",   	--秘卷:天资附魔
		"//additem 5240722 20",   	--秘卷:天资附魔
		"//additem 5240723 20",   	--秘卷:天资附魔
		
		"//additem 5240922 20",   	--秘卷:天资附魔
		"//additem 5240923 20",   	--秘卷:天资附魔
		"//additem 5241022 20",   	--秘卷:天资附魔
		"//additem 5241023 20",   	--秘卷:天资附魔
		"//additem 5241122 20",   	--秘卷:天资附魔
		"//additem 5241123 20",   	--秘卷:天资附魔
		"//additem 5241222 20",   	--秘卷:天资附魔
		"//additem 5241223 20",   	--秘卷:天资附魔
		"//additem 5241322 20",   	--秘卷:天资附魔
		"//additem 5241323 20",   	--秘卷:天资附魔
		
		"//additem 5241456 20",   	--秘卷:天资附魔
		"//additem 5241457 20",   	--秘卷:天资附魔
		"//additem 5241458 20",   	--秘卷:天资附魔
		"//additem 5241459 20",   	--秘卷:天资附魔
		"//additem 5241460 20",   	--秘卷:天资附魔
		"//additem 5241461 20",   	--秘卷:天资附魔
		"//additem 5241462 20",   	--秘卷:天资附魔
		"//additem 5241463 20",   	--秘卷:天资附魔
		"//additem 5241464 20",   	--秘卷:天资附魔
		"//additem 5241465 20",   	--秘卷:天资附魔
		
		"//additem 5241466 20",   	--秘卷:天资附魔
		"//additem 5241467 20",   	--秘卷:天资附魔
		"//additem 5241470 20",   	--秘卷:天资附魔
		"//additem 5241472 20",   	--秘卷:天资附魔
		"//additem 5241473 20",   	--秘卷:天资附魔
		

	},	


	
	{
		"//cmdpg0",
		0,
		4,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//repairall",
		"//applystatus 0 1005 1 7200",
		"//addattr vim=11111",	    --回复精力值
		"//addattr vig=11111",		--回复活力值
	},
		
	{
		"//cmdpg1",
		0,
		1,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//applystatus 0 1017 10 7200",
	
	},	
	

	
	{
		"//cmdpg4",
		0,
		1,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//applystatus 0 1005 3 7200",   --命中率提高100%
	},	

	{
		"//cmdpg6",			--银币，绑银，金券
		0,
		1,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//addattr bull=10000000",   --绑银
		"//addattr money=10000000",   --银币
		"//addattr godcoin=1000000",   --金币
		"//addattr bindgodcoin=1000000",   --金券
		
	},	

	
	{
		"//cmdpgbs1",   --1级宝石
		1,
		21,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//paklarge 3 96",
		"//additem 5210000 500",
		"//additem 5210100 500",
		"//additem 5211000 500",
		"//additem 5211100 500",
		"//additem 5211200 500",
		"//additem 5212000 500",
		"//additem 5212100 500",
		"//additem 5212200 500",
		"//additem 5212400 500",
		"//additem 5213300 500",
		"//additem 5213400 500",
		"//additem 5219400 20",
		
		"//additem 5213410 500",
		"//additem 5213411 5000",
		"//additem 5060014 5000",

	},
	
	{
		"//cmdpgbs2",   --2级宝石
		1,
		20,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//paklarge 3 96",
		"//additem 5211101 50",
		"//additem 5211201 50",
		"//additem 5210001 50",
		"//additem 5212001 50",
		"//additem 5212101 50",
		"//additem 5212201 50",
		"//additem 5213301 50",
		"//additem 5213201 50",
		"//additem 5211001 50",
		"//additem 5210101 50",
		"//additem 5212401 50",
		"//additem 5213401 50",
		"//additem 5213411 5000",
		"//additem 5219401 20",
		
		
	},
	{
		"//cmdpgbs3",   --3级宝石
		1,
		22,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//paklarge 3 96",
		"//additem 5211102 50",
		"//additem 5211202 50",
		"//additem 5210002 50",
		"//additem 5212002 50",
		"//additem 5212102 50",
		"//additem 5212202 50",
		"//additem 5213302 50",
		"//additem 5213202 50",
		"//additem 5211002 50",
		"//additem 5210102 50",
		"//additem 5212402 50",
		"//additem 5213402 50",
		"//additem 5213410 500",
		"//additem 5213411 5000",
		"//additem 5060014 5000",
		"//additem 5219402 20",
		"//additem 5219403 20",
		"//additem 5219404 20",
		"//additem 5219405 20",
		

	},

	{
		"//cmdpgbs4",   --4级宝石
		1,
		18,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//paklarge 3 96",
		"//additem 5211103 50",
		"//additem 5211203 50",
		"//additem 5210003 50",
		"//additem 5212003 50",
		"//additem 5212103 50",
		"//additem 5212203 50",
		"//additem 5213303 50",
		"//additem 5213203 50",
		"//additem 5211003 50",
		"//additem 5210103 50",
		"//additem 5212403 50",
		"//additem 5213403 50",
		"//additem 5213411 5000",
		"//additem 5219403 20",
		"//additem 5219404 20",
		"//additem 5219405 20",
		
	},

	{
		"//cmdpgbs5",   --5级宝石
		1,
		17,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//paklarge 3 96",
		"//additem 5211104 50",
		"//additem 5211204 50",
		"//additem 5210004 50",
		"//additem 5212004 50",
		"//additem 5212104 50",
		"//additem 5212204 50",
		"//additem 5213304 50",
		"//additem 5213204 50",
		"//additem 5211004 50",
		"//additem 5210104 50",
		"//additem 5212404 50",
		"//additem 5213404 50",
		"//additem 5213411 5000",
		"//additem 5219404 20",
		"//additem 5219405 20",
		
	},

	{
		"//cmdpgbs6",   --6级宝石
		1,
		16,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//paklarge 3 96",
		"//additem 5211105 50",
		"//additem 5211205 50",
		"//additem 5210005 50",
		"//additem 5212005 50",
		"//additem 5212105 50",
		"//additem 5212205 50",
		"//additem 5213305 50",
		"//additem 5213205 50",
		"//additem 5211005 50",
		"//additem 5210105 50",
		"//additem 5212405 50",
		"//additem 5213405 50",
		"//additem 5213411 5000",
		"//additem 5219405 20",

	},
	
	{
		"//cmdpgbs7",   --7级宝石
		1,
		15,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//paklarge 3 96",
		"//additem 5210006 50",
		"//additem 5210106 50",
		"//additem 5211006 50",
		"//additem 5211106 50",
		"//additem 5211206 50",
		"//additem 5212006 50",
		"//additem 5212106 50",
		"//additem 5212206 50",
		"//additem 5212406 50",
		"//additem 5213306 50",
		"//additem 5213406 50",
		"//additem 5219200 50",  --宝石摘取符 
		"//additem 5213411 5000",

	},
		
		
	{
		"//cmdpg2",
		0,
		14,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//applystatus 0 1017 10 3",
		"//applystatus 0 1015 1 3",
		"//applystatus 0 1051 1 3",
		"//applystatus 0 1215 7 2",   --攻击提高113
		"//applystatus 0 1107 1 2",   --攻击提高113
		"//applystatus 0 1134 1 2",   --攻击提高113
		"//applystatus 0 1133 1 2",   --攻击提高113
		"//applystatus 0 1132 1 2",   --攻击提高113
		"//applystatus 0 1005 1 2",   --攻击提高113
		"//applystatus 0 1218 1 2",   --攻击提高113
		"//applystatus 0 1104 7 2",   --攻击提高113
		"//applystatus 0 1110 7 2",   --攻击提高113
		"//applystatus 0 3113 1 2",   --天天向上
		"//applystatus 0 3191 1 2",   --天天向上

	},
		
	{
		"//cmdpg3",
		0,
		7,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
--		"//applystatus 0 1051 1 7200",  --无敌
		"//applystatus 0 1215 7 7200",   --伤害1000
		"//applystatus 0 1110 7 7200",   --伤害吸收80%
		"//applystatus 0 1134 3 7200",   --暴击提高1000%
		"//applystatus 0 1133 4 7200",   --暴击率提高100%
		"//applystatus 0 1132 3 7200",   --反击
		"//applystatus 0 1104 7 7200",   --命中率提高100%
		"//applystatus 0 3191 1 7200",   --逆袭
		
	
	},	
--	{
--		"//cmdpg8",   --gm套装
--		1,
--		17,
--		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
--		"//packageclear 3",
--		"//additem 1039999 1",
--		"//additem 1069999 1",
--		"//additem 1109999 1",
--		"//additem 1209999 1",
--		"//additem 1289999 1",
--		"//additem 1509999 1",
--		"//additem 2029999 1",
--		"//additem 2129999 1",
--		"//additem 2229999 1",
--		"//additem 2429999 1",
--		"//additem 2629999 1",
--		"//additem 2719998 1",
--		"//additem 2719999 1",
--		"//additem 2819999 1",
--		"//additem 2919998 1",
--		"//additem 2919999 1",

--	
--	},
	
	{
		"//cmdpghf1",   --护符1
		1,
		37,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//paklarge 3 96",
		"//additem 2713006 1",
		"//additem 2713007 1",
		"//additem 2713008 1",
		"//additem 2713009 1",
		"//additem 2713010 1",
		"//additem 2713011 1",
		"//additem 2713012 1",
		"//additem 2714006 1",
		"//additem 2714007 1",
		"//additem 2714008 1",

		"//additem 2714009 1",
		"//additem 2714010 1",
		"//additem 2714011 1",
		"//additem 2714012 1",
		"//additem 2715006 1",
		"//additem 2715007 1",
		"//additem 2715008 1",
		"//additem 2715009 1",
		"//additem 2715010 1",
		"//additem 2715011 1",

		"//additem 2715012 1",
		"//additem 2718006 1",
		"//additem 2718007 1",
		"//additem 2718008 1",
		"//additem 2718009 1",
		"//additem 2718010 1",
		"//additem 2718011 1",
		"//additem 2718012 1",
		"//additem 2719906 1",
		"//additem 2719907 1",

		"//additem 2719908 1",
		"//additem 2719909 1",
		"//additem 2719910 1",
		"//additem 2719911 1",
		"//additem 2719912 1",

	},
	
	{
		"//cmdpgxl1",   --项链
		1,
		80,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//paklarge 3 96",
		"//additem 2813002 1",
		"//additem 2813003 1",
		"//additem 2813004 1",
		"//additem 2813005 1",
		"//additem 2813006 1",
		"//additem 2813007 1",
		"//additem 2813008 1",
		"//additem 2813009 1",
		"//additem 2813010 1",
		"//additem 2813011 1",

		"//additem 2813012 1",
		"//additem 2813013 1",
		"//additem 2814002 1",
		"//additem 2814003 1",
		"//additem 2814004 1",
		"//additem 2814005 1",
		"//additem 2814006 1",
		"//additem 2814007 1",
		"//additem 2814008 1",
		"//additem 2814009 1",

		"//additem 2814010 1",
		"//additem 2814011 1",
		"//additem 2814012 1",
		"//additem 2814013 1",
		"//additem 2815002 1",
		"//additem 2815003 1",
		"//additem 2815004 1",
		"//additem 2815005 1",
		"//additem 2815006 1",
		"//additem 2815007 1",

		"//additem 2815008 1",
		"//additem 2815009 1",
		"//additem 2815010 1",
		"//additem 2815011 1",
		"//additem 2815012 1",
		"//additem 2815013 1",
		"//additem 2818003 1",
		"//additem 2818004 1",
		"//additem 2818005 1",
		"//additem 2818006 1",

		"//additem 2818007 1",
		"//additem 2818008 1",
		"//additem 2818009 1",
		"//additem 2818010 1",
		"//additem 2818011 1",
		"//additem 2818012 1",
		"//additem 2818103 1",
		"//additem 2818105 1",
		"//additem 2818107 1",
		"//additem 2818109 1",

		"//additem 2818111 1",
		"//additem 2818203 1",
		"//additem 2818204 1",
		"//additem 2818205 1",
		"//additem 2818206 1",
		"//additem 2818207 1",
		"//additem 2818208 1",
		"//additem 2818209 1",
		"//additem 2818210 1",
		"//additem 2818211 1",

		"//additem 2818212 1",
		"//additem 2818304 1",
		"//additem 2818306 1",
		"//additem 2818308 1",
		"//additem 2818310 1",
		"//additem 2818312 1",
		"//additem 2819902 1",
		"//additem 2819903 1",
		"//additem 2819904 1",
		"//additem 2819905 1",

		"//additem 2819906 1",
		"//additem 2819907 1",
		"//additem 2819908 1",
		"//additem 2819909 1",
		"//additem 2819910 1",
		"//additem 2819911 1",
		"//additem 2819912 1",
		"//additem 2819913 1",

	},
	
	
	{
		"//cmdpgjz1",   --戒指1
		1,
		82,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//paklarge 3 96",
		"//additem 2913002 1",
		"//additem 2913003 1",
		"//additem 2913004 1",
		"//additem 2913005 1",
		"//additem 2913006 1",
		"//additem 2913007 1",
		"//additem 2913008 1",
		"//additem 2913009 1",
		"//additem 2913010 1",
		"//additem 2913011 1",

		"//additem 2913012 1",
		"//additem 2913013 1",
		"//additem 2914002 1",
		"//additem 2914003 1",
		"//additem 2914004 1",
		"//additem 2914005 1",
		"//additem 2914006 1",
		"//additem 2914007 1",
		"//additem 2914008 1",
		"//additem 2914009 1",

		"//additem 2914010 1",
		"//additem 2914011 1",
		"//additem 2914012 1",
		"//additem 2914013 1",
		"//additem 2915002 1",
		"//additem 2915003 1",
		"//additem 2915004 1",
		"//additem 2915005 1",
		"//additem 2915006 1",
		"//additem 2915007 1",

		"//additem 2915008 1",
		"//additem 2915009 1",
		"//additem 2915010 1",
		"//additem 2915011 1",
		"//additem 2915012 1",
		"//additem 2915013 1",
		"//additem 2918003 1",
		"//additem 2918004 1",
		"//additem 2918005 1",
		"//additem 2918006 1",

		"//additem 2918007 1",
		"//additem 2918008 1",
		"//additem 2918009 1",
		"//additem 2918010 1",
		"//additem 2918011 1",
		"//additem 2918012 1",
		"//additem 2918103 1",
		"//additem 2918104 1",
		"//additem 2918105 1",
		"//additem 2918106 1",

		"//additem 2918107 1",
		"//additem 2918108 1",
		"//additem 2918109 1",
		"//additem 2918110 1",
		"//additem 2918111 1",
		"//additem 2918112 1",
		"//additem 2918203 1",
		"//additem 2918204 1",
		"//additem 2918205 1",
		"//additem 2918206 1",

		"//additem 2918207 1",
		"//additem 2918208 1",
		"//additem 2918209 1",
		"//additem 2918210 1",
		"//additem 2918211 1",
		"//additem 2918212 1",
		"//additem 2918303 1",
		"//additem 2918305 1",
		"//additem 2918307 1",
		"//additem 2918309 1",

		"//additem 2918311 1",
		"//additem 2919902 1",
		"//additem 2919903 1",
		"//additem 2919904 1",
		"//additem 2919905 1",
		"//additem 2919906 1",
		"//additem 2919907 1",
		"//additem 2919908 1",
		"//additem 2919909 1",
		"//additem 2919910 1",

	},
	
	
	{
		"//cmdpgjz2",   --戒指2
		1,
		88,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//paklarge 3 96",
		"//additem 2919911 1",
		"//additem 2919912 1",
		"//additem 2919913 1",
		"//additem 2923002 1",
		"//additem 2923003 1",
		"//additem 2923004 1",
		"//additem 2923005 1",
		"//additem 2923006 1",
		"//additem 2923007 1",
		"//additem 2923008 1",

		"//additem 2923009 1",
		"//additem 2923010 1",
		"//additem 2923011 1",
		"//additem 2923012 1",
		"//additem 2923013 1",
		"//additem 2924002 1",
		"//additem 2924003 1",
		"//additem 2924004 1",
		"//additem 2924005 1",
		"//additem 2924006 1",

		"//additem 2924007 1",
		"//additem 2924008 1",
		"//additem 2924009 1",
		"//additem 2924010 1",
		"//additem 2924011 1",
		"//additem 2924012 1",
		"//additem 2924013 1",
		"//additem 2925002 1",
		"//additem 2925003 1",
		"//additem 2925004 1",

		"//additem 2925005 1",
		"//additem 2925006 1",
		"//additem 2925007 1",
		"//additem 2925008 1",
		"//additem 2925009 1",
		"//additem 2925010 1",
		"//additem 2925011 1",
		"//additem 2925012 1",
		"//additem 2925013 1",
		"//additem 2928003 1",

		"//additem 2928004 1",
		"//additem 2928005 1",
		"//additem 2928006 1",
		"//additem 2928007 1",
		"//additem 2928008 1",
		"//additem 2928009 1",
		"//additem 2928010 1",
		"//additem 2928011 1",
		"//additem 2928012 1",
		"//additem 2928103 1",

		"//additem 2928104 1",
		"//additem 2928105 1",
		"//additem 2928106 1",
		"//additem 2928107 1",
		"//additem 2928108 1",
		"//additem 2928109 1",
		"//additem 2928110 1",
		"//additem 2928111 1",
		"//additem 2928112 1",
		"//additem 2928203 1",

		"//additem 2928204 1",
		"//additem 2928205 1",
		"//additem 2928206 1",
		"//additem 2928207 1",
		"//additem 2928208 1",
		"//additem 2928209 1",
		"//additem 2928210 1",
		"//additem 2928211 1",
		"//additem 2928212 1",
		"//additem 2928303 1",

		"//additem 2928305 1",
		"//additem 2928307 1",
		"//additem 2928309 1",
		"//additem 2928311 1",
		"//additem 2929902 1",
		"//additem 2929903 1",
		"//additem 2929904 1",
		"//additem 2929905 1",
		"//additem 2929906 1",
		"//additem 2929907 1",

		"//additem 2929908 1",
		"//additem 2929909 1",
		"//additem 2929910 1",
		"//additem 2929911 1",
		"//additem 2929912 1",
		"//additem 2929913 1",


	},
		
	
	{
		"//cmdpgzs1",   --副本套
		1,
		100,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//paklarge 3 96",
		"//additem 1011601 1",
		"//additem 2011601 1",
		"//additem 2111601 1",
		"//additem 2211601 1",
		"//additem 2611601 1",
		"//additem 1061601 1",
		"//additem 1011602 1",
		"//additem 2011602 1",
		"//additem 2111602 1",
		"//additem 2211602 1",
		"//additem 2611602 1",
		"//additem 1061602 1",
		"//additem 1015020 1",
		"//additem 1011605 1",
		"//additem 2011605 1",
		"//additem 2111605 1",
		"//additem 2211605 1",
		"//additem 2611605 1",
		"//additem 1061605 1",
		"//additem 1011603 1",
		"//additem 2011603 1",
		"//additem 2111603 1",
		"//additem 2211603 1",
		"//additem 2611603 1",
		"//additem 1061603 1",
		"//additem 1061607 1",
		"//additem 2011607 1",
		"//additem 2111607 1",
		"//additem 2211607 1",
		"//additem 2611607 1",
		
		"//additem 1011607 1",
		"//additem 1015010 1",
		"//additem 2015010 1",
		"//additem 2115010 1",
		"//additem 2215010 1",
		"//additem 2615010 1",
		"//additem 1065010 1",
		"//additem 1015011 1",
		"//additem 2015011 1",
		"//additem 2115011 1",
		"//additem 2215011 1",
		"//additem 2615011 1",
		"//additem 1065011 1",
		"//additem 1015012 1",
		"//additem 2015012 1",
		"//additem 2115012 1",
		"//additem 2215012 1",
		"//additem 2615012 1",
		"//additem 1065012 1",
		"//additem 2015013 1",
		"//additem 1065013 1",
		"//additem 1015013 1",
		"//additem 2215013 1",
		"//additem 2615013 1",
		"//additem 2115013 1",
		"//additem 1035020 1",
		"//additem 1031605 1",
		"//additem 2011615 1",
		"//additem 2111615 1",
		"//additem 2211615 1",
		"//additem 2611615 1",
		"//additem 1041605 1",
		"//additem 1031603 1",
		"//additem 2011613 1",
		"//additem 2111613 1",
		"//additem 2211613 1",
		"//additem 2611613 1",
		"//additem 1041603 1",
		"//additem 1041607 1",
		"//additem 2211617 1",
		"//additem 2611617 1",
		"//additem 1031607 1",
		"//additem 2011617 1",
		"//additem 2111617 1",
		"//additem 1035010 1",
		"//additem 2015030 1",
		"//additem 2115030 1",
		"//additem 2215030 1",
		"//additem 2615030 1",
		"//additem 1045010 1",
		"//additem 1035011 1",
		"//additem 2015031 1",
		"//additem 2115031 1",
		"//additem 2215031 1",
		"//additem 2615031 1",
		"//additem 1045011 1",
		"//additem 1035012 1",
		"//additem 2015032 1",
		"//additem 2115032 1",
		"//additem 2215032 1",
		"//additem 2615032 1",
		"//additem 1045012 1",
		"//additem 1045013 1",
		"//additem 2015033 1",
		"//additem 1035013 1",
		"//additem 2215033 1",
		"//additem 2115033 1",
		"//additem 2615033 1",
		
	},

	{
		"//cmdpgzs2",   --战场套装
		1,
		68,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//paklarge 3 96",
		"//additem 1015002 1",
		"//additem 2015002 1",
		"//additem 2115002 1",
		"//additem 2215002 1",
		"//additem 2615002 1",
		"//additem 1065002 1",
		"//additem 1015003 1",
		"//additem 2015003 1",
		"//additem 2115003 1",
		"//additem 2215003 1",
		"//additem 2615003 1",
		"//additem 1065003 1",
		"//additem 1015004 1",
		"//additem 2015004 1",
		"//additem 2115004 1",
		"//additem 2215004 1",
		"//additem 2615004 1",
		"//additem 1065004 1",
		"//additem 1015005 1",
		"//additem 2015005 1",
		"//additem 2115005 1",
		"//additem 2215005 1",
		"//additem 2615005 1",
		"//additem 1065005 1",
		"//additem 1015006 1",
		"//additem 2015006 1",
		"//additem 2115006 1",
		"//additem 2215006 1",
		"//additem 2615006 1",
		"//additem 1065006 1",
		"//additem 1015007 1",
		"//additem 2015007 1",
		"//additem 2115007 1",
		"//additem 2215007 1",
		"//additem 2615007 1",
		"//additem 1065007 1",
		"//additem 1015008 1",
		"//additem 2015008 1",
		"//additem 2115008 1",
		"//additem 2215008 1",
		"//additem 2615008 1",
		"//additem 1065008 1",
		"//additem 2015009 1",
		"//additem 2215009 1",
		"//additem 1015009 1",
		"//additem 1065009 1",
		"//additem 2115009 1",
		"//additem 2615009 1",
		"//additem 1035007 1",
		"//additem 2015027 1",
		"//additem 2115027 1",
		"//additem 2215027 1",
		"//additem 2615027 1",
		"//additem 1045007 1",
		"//additem 1035008 1",
		"//additem 2015028 1",
		"//additem 2115028 1",
		"//additem 2215028 1",
		"//additem 2615028 1",
		"//additem 1045008 1",
		"//additem 1035009 1",
		"//additem 2115029 1",
		"//additem 2215029 1",
		"//additem 2015029 1",
		"//additem 1045009 1",
		"//additem 2615029 1",
		
	},
	
	{
		"//cmdpgzs3",   --Boss套装
		1,
		32,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//paklarge 3 96",
		"//additem 1011801 1",
		"//additem 2011801 1",
		"//additem 2111801 1",
		"//additem 2211801 1",
		"//additem 2611801 1",
		"//additem 1061801 1",
		"//additem 1011802 1",
		"//additem 2011802 1",
		"//additem 2111802 1",
		"//additem 2211802 1",
		"//additem 2611802 1",
		"//additem 1061802 1",
		"//additem 2211803 1",
		"//additem 2011803 1",
		"//additem 1011803 1",
		"//additem 1061803 1",
		"//additem 2611803 1",
		"//additem 2111803 1",
		"//additem 1031802 1",
		"//additem 2011822 1",
		"//additem 2111822 1",
		"//additem 2211822 1",
		"//additem 2611822 1",
		"//additem 1041802 1",
		"//additem 2211813 1",
		"//additem 2011813 1",
		"//additem 1041803 1",
		"//additem 1031803 1",
		"//additem 2611813 1",
		"//additem 2111813 1",
	
	
	},	
	
	
	{
		"//cmdpgzs4",   --Boss套装
		1,
		32,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//paklarge 3 96",
		"//additem 1011905 1",
		"//additem 2011905 1",
		"//additem 2111905 1",
		"//additem 2211905 1",
		"//additem 2611905 1",
		"//additem 1061905 1",
		"//additem 1011907 1",
		"//additem 2011907 1",
		"//additem 2111907 1",
		"//additem 2211907 1",
		"//additem 2611907 1",
		"//additem 1061907 1",
		"//additem 2211909 1",
		"//additem 2011909 1",
		"//additem 1011909 1",
		"//additem 1061909 1",
		"//additem 2611909 1",
		"//additem 2111909 1",
		"//additem 1031907 1",
		"//additem 2011927 1",
		"//additem 2111927 1",
		"//additem 2211927 1",
		"//additem 2611927 1",
		"//additem 1041907 1",
		"//additem 1031909 1",
		"//additem 1041909 1",
		"//additem 2211929 1",
		"//additem 2011929 1",
		"//additem 2111929 1",
		"//additem 2611929 1",
	
	
	},	
	
	{
		"//cmdpg80sh",   --80守护
		1,
		74,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//paklarge 3 96",
		--白装
		"//additem 1019909 1",
		"//additem 2119909 1",
		"//additem 2019909 1",
		"//additem 1069909 1",
		"//additem 2219909 1",
		"//additem 2619909 1",
		
		--绿装
		"//additem 1013009 1",
		"//additem 2113009 1",
		"//additem 2013009 1",
		"//additem 1063009 1",
		"//additem 2213009 1",
		"//additem 2613009 1",
		--蓝套
		"//additem 1017009 1",
		"//additem 2117009 1",
		"//additem 2017009 1",
		"//additem 1067009 1",
		"//additem 2217009 1",
		"//additem 2617009 1",
		--蓝装
		"//additem 1014009 1",
		"//additem 2114009 1",
		"//additem 2014009 1",
		"//additem 1064009 1",
		"//additem 2214009 1",
		"//additem 2614009 1",
		--紫装
		"//additem 1011507 1",
		"//additem 2111507 1",
		"//additem 2011507 1",
		"//additem 1061507 1",
		"//additem 2211507 1",
		"//additem 2611507 1",
		--晶装
		"//additem 1011607 1",
		"//additem 2111607 1",
		"//additem 2011607 1",
		"//additem 1061607 1",
		"//additem 2211607 1",
		"//additem 2611607 1",
		--史诗套
		"//additem 1011909 1",
		"//additem 2111909 1",
		"//additem 2011909 1",
		"//additem 1061909 1",
		"//additem 2211909 1",
		"//additem 2611909 1",
		--钻套
		"//additem 1011407 1",
		"//additem 2111407 1",
		"//additem 2011407 1",
		"//additem 1061407 1",
		"//additem 2211407 1",
		"//additem 2611407 1",
		--英雄套
		"//additem 1015009 1",
		"//additem 2115009 1",
		"//additem 2015009 1",
		"//additem 1065009 1",
		"//additem 2215009 1",
		"//additem 2615009 1",		
		--橙装
		"//additem 1016007 1",
		"//additem 2116007 1",
		"//additem 2016007 1",
		"//additem 1066007 1",
		"//additem 2216007 1",
		"//additem 2616007 1",
		--荣耀套
		"//additem 1011803 1",
		"//additem 2111803 1",
		"//additem 2011803 1",
		"//additem 1061803 1",
		"//additem 2211803 1",
		"//additem 2611803 1",


	
	},
	
	{
		"//cmdpg80zf",   --80征服
		1,
		74,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//paklarge 3 96",
		--绿
		"//additem 1033009 1",
		"//additem 2113029 1",
		"//additem 2013029 1",
		"//additem 1043009 1",
		"//additem 2213029 1",
		"//additem 2613029 1",
		--蓝套
		"//additem 1037009 1",
		"//additem 2117029 1",
		"//additem 2017029 1",
		"//additem 1047009 1",
		"//additem 2217029 1",
		"//additem 2617029 1",
		--蓝
		"//additem 1034009 1",
		"//additem 2114029 1",
		"//additem 2014029 1",
		"//additem 1044009 1",
		"//additem 2214029 1",
		"//additem 2614029 1",
		--紫1
		"//additem 1031507 1",
		"//additem 2111517 1",
		"//additem 2011517 1",
		"//additem 1041507 1",
		"//additem 2211517 1",
		"//additem 2611517 1",
		--晶
		"//additem 1031607 1",
		"//additem 2111617 1",
		"//additem 2011617 1",
		"//additem 1041607 1",
		"//additem 2211617 1",
		"//additem 2611617 1",
		--荣耀
		"//additem 1031803 1",
		"//additem 2111813 1",
		"//additem 2011813 1",
		"//additem 1041803 1",
		"//additem 2211813 1",
		"//additem 2611813 1",
		--史诗
		"//additem 1031909 1",
		"//additem 2111929 1",
		"//additem 2011929 1",
		"//additem 1041909 1",
		"//additem 2211929 1",
		"//additem 2611929 1",
		--白
		"//additem 1039909 1",
		"//additem 2119929 1",
		"//additem 2019929 1",
		"//additem 1049909 1",
		"//additem 2219929 1",
		"//additem 2619929 1",
		--钻
		"//additem 1031407 1",
		"//additem 2111417 1",
		"//additem 2011417 1",
		"//additem 1041407 1",
		"//additem 2211417 1",
		"//additem 2611417 1",
		--英雄
		"//additem 1035009 1",
		"//additem 2115029 1",
		"//additem 2015029 1",
		"//additem 1045009 1",
		"//additem 2215029 1",
		"//additem 2615029 1",
		--橙
		"//additem 1036007 1",
		"//additem 2116017 1",
		"//additem 2016017 1",
		"//additem 1046007 1",
		"//additem 2216017 1",
		"//additem 2616017 1",
		

	
	},	
	
	{
		"//cmdpg80hm",   --80毁灭
		1,
		74,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//paklarge 3 96",
		--绿
		"//additem 1253009 1",
		"//additem 2133029 1",
		"//additem 2033029 1",
		"//additem 1263009 1",
		"//additem 1273029 1",
		"//additem 2633029 1",
		--蓝套
		"//additem 1257009 1",
		"//additem 2137029 1",
		"//additem 2037029 1",
		"//additem 1267009 1",
		"//additem 1277029 1",
		"//additem 2637029 1",
		--蓝
		"//additem 1254009 1",
		"//additem 2134029 1",
		"//additem 2034029 1",
		"//additem 1264009 1",
		"//additem 1274029 1",
		"//additem 2634029 1",
		--紫1
		"//additem 1251507 1",
		"//additem 2131517 1",
		"//additem 2031517 1",
		"//additem 1261507 1",
		"//additem 1271517 1",
		"//additem 2631517 1",
		--紫晶
		"//additem 1251607 1",
		"//additem 2131617 1",
		"//additem 2031617 1",
		"//additem 1261607 1",
		"//additem 1271617 1",
		"//additem 2631617 1",
		--荣耀
		"//additem 1251803 1",
		"//additem 2131813 1",
		"//additem 2031813 1",
		"//additem 1261803 1",
		"//additem 1271813 1",
		"//additem 2631813 1",
		--英雄
		"//additem 1255009 1",
		"//additem 2135029 1",
		"//additem 2035029 1",
		"//additem 1265009 1",
		"//additem 1275029 1",
		"//additem 2635029 1",
		--史诗
		"//additem 1251909 1",
		"//additem 2131929 1",
		"//additem 2031929 1",
		"//additem 1261909 1",
		"//additem 1271929 1",
		"//additem 2631929 1",
		--白
		"//additem 1259909 1",
		"//additem 2139929 1",
		"//additem 2039929 1",
		"//additem 1269909 1",
		"//additem 1279929 1",
		"//additem 2639929 1",
		--钻
		"//additem 1031407 1",
		"//additem 2111417 1",
		"//additem 2011417 1",
		"//additem 1041407 1",
		"//additem 2211417 1",
		"//additem 2611417 1",
		--橙
		"//additem 1256007 1",
		"//additem 2136017 1",
		"//additem 2036017 1",
		"//additem 1266007 1",
		"//additem 1276017 1",
		"//additem 2636017 1",

				
	},	
	
	{
		"//cmdpg80ls",   --80猎杀
		1,
		74,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//paklarge 3 96",
		--绿
		"//additem 1203009 1",
		"//additem 2133009 1",
		"//additem 2033009 1",
		"//additem 1283009 1",
		"//additem 1273009 1",
		"//additem 2633009 1",
		--蓝套
		"//additem 1207009 1",
		"//additem 2137009 1",
		"//additem 2037009 1",
		"//additem 1287009 1",
		"//additem 1277009 1",
		"//additem 2637009 1",
		--蓝
		"//additem 1204009 1",
		"//additem 2134009 1",
		"//additem 2034009 1",
		"//additem 1284009 1",
		"//additem 1274009 1",
		"//additem 2634009 1",
		--紫
		"//additem 1201507 1",
		"//additem 2131507 1",
		"//additem 2031507 1",
		"//additem 1281507 1",
		"//additem 1271507 1",
		"//additem 2631507 1",
		--荣耀
		"//additem 1201803 1",
		"//additem 2131803 1",
		"//additem 2031803 1",
		"//additem 1281803 1",
		"//additem 1271803 1",
		"//additem 2631803 1",
		--晶
		"//additem 1201607 1",
		"//additem 2131607 1",
		"//additem 2031607 1",
		"//additem 1281607 1",
		"//additem 1271607 1",
		"//additem 2631607 1",

		--史诗
		"//additem 1201909 1",
		"//additem 2131909 1",
		"//additem 2031909 1",
		"//additem 1281909 1",
		"//additem 1271909 1",
		"//additem 2631909 1",
		--白
		"//additem 1209909 1",
		"//additem 2139909 1",
		"//additem 2039909 1",
		"//additem 1289909 1",
		"//additem 1279909 1",
		"//additem 2639909 1",
		--钻
		"//additem 1201407 1",
		"//additem 2131407 1",
		"//additem 2031407 1",
		"//additem 1281407 1",
		"//additem 1271407 1",
		"//additem 2631407 1",
		--英雄
		"//additem 1205009 1",
		"//additem 2135009 1",
		"//additem 2035009 1",
		"//additem 1285009 1",
		"//additem 1275009 1",
		"//additem 2635009 1",
		--橙
		"//additem 1206007 1",
		"//additem 2136007 1",
		"//additem 2036007 1",
		"//additem 1286007 1",
		"//additem 1276007 1",
		"//additem 2636007 1",
		
	
	},	
	
	{
		"//cmdpg80jn",   --80聚能
		1,
		74,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//paklarge 3 96",
		--绿	
		"//additem 1103009 1",
		"//additem 2123009 1",
		"//additem 2023009 1",
		"//additem 1183009 1",
		"//additem 2223009 1",
		"//additem 2623009 1",
		--蓝套
		"//additem 1107009 1",
		"//additem 2127009 1",
		"//additem 2027009 1",
		"//additem 1187009 1",
		"//additem 2227009 1",
		"//additem 2627009 1",
		--蓝
		"//additem 1104009 1",
		"//additem 2124009 1",
		"//additem 2024009 1",
		"//additem 1184009 1",
		"//additem 2224009 1",
		"//additem 2624009 1",
		--紫
		"//additem 1101507 1",
		"//additem 2121507 1",
		"//additem 2021507 1",
		"//additem 1181507 1",
		"//additem 2221507 1",
		"//additem 2621507 1",
		--晶
		"//additem 1101607 1",
		"//additem 2121607 1",
		"//additem 2021607 1",
		"//additem 1181607 1",
		"//additem 2221607 1",
		"//additem 2621607 1",
		--荣耀
		"//additem 1101803 1",
		"//additem 2121803 1",
		"//additem 2021803 1",
		"//additem 1181803 1",
		"//additem 2221803 1",
		"//additem 2621803 1",

		--史诗
		"//additem 1101909 1",
		"//additem 2121909 1",
		"//additem 2021909 1",
		"//additem 1181909 1",
		"//additem 2221909 1",
		"//additem 2621909 1",
		--白
		"//additem 1109909 1",
		"//additem 2129909 1",
		"//additem 2029909 1",
		"//additem 1189909 1",
		"//additem 2229909 1",
		"//additem 2629909 1",
		--钻	
		"//additem 1101407 1",
		"//additem 2121407 1",
		"//additem 2021407 1",
		"//additem 1181407 1",
		"//additem 2221407 1",
		"//additem 2621407 1",

		--英雄
		"//additem 1105009 1",
		"//additem 2125009 1",
		"//additem 2025009 1",
		"//additem 1185009 1",
		"//additem 2225009 1",
		"//additem 2625009 1",
		--橙
		"//additem 1106007 1",
		"//additem 2126007 1",
		"//additem 2026007 1",
		"//additem 1186007 1",
		"//additem 2226007 1",
		"//additem 2626007 1",
		
	
	},	
	
	{
		"//cmdpg80lh",   --80炼魂
		1,
		74,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//paklarge 3 96",
		--绿
		"//additem 1113009 1",
		"//additem 2123029 1",
		"//additem 2023029 1",
		"//additem 1183029 1",
		"//additem 2223029 1",
		"//additem 2623029 1",
		--蓝套
		"//additem 1117009 1",
		"//additem 2127029 1",
		"//additem 2027029 1",
		"//additem 1187029 1",
		"//additem 2227029 1",
		"//additem 2627029 1",
		--蓝
		"//additem 1114009 1",
		"//additem 2124029 1",
		"//additem 2024029 1",
		"//additem 1184029 1",
		"//additem 2224029 1",
		"//additem 2624029 1",
		--紫
		"//additem 1111507 1",
		"//additem 2121517 1",
		"//additem 2021517 1",
		"//additem 1181517 1",
		"//additem 2221517 1",
		"//additem 2621517 1",
		--晶
		"//additem 1111607 1",
		"//additem 2121617 1",
		"//additem 2021617 1",
		"//additem 1181617 1",
		"//additem 2221617 1",
		"//additem 2621617 1",
		--荣耀
		"//additem 1111803 1",
		"//additem 2121813 1",
		"//additem 2021813 1",
		"//additem 1181813 1",
		"//additem 2221813 1",
		"//additem 2621813 1",
		
		--史诗
		"//additem 1111909 1",
		"//additem 2121929 1",
		"//additem 2021929 1",
		"//additem 1181929 1",
		"//additem 2221929 1",
		"//additem 2621929 1",
		--白
		"//additem 1119909 1",
		"//additem 2129929 1",
		"//additem 2029929 1",
		"//additem 1189929 1",
		"//additem 2229929 1",
		"//additem 2629929 1",
		--钻
		"//additem 1111407 1",
		"//additem 2121417 1",
		"//additem 2021417 1",
		"//additem 1181417 1",
		"//additem 2221417 1",
		"//additem 2621417 1",
		
		--英雄		
		"//additem 1115009 1",
		"//additem 2125029 1",
		"//additem 2025029 1",
		"//additem 1185029 1",
		"//additem 2225029 1",
		"//additem 2625029 1",
		--橙
		"//additem 1116007 1",
		"//additem 2126017 1",
		"//additem 2026017 1",
		"//additem 1186017 1",
		"//additem 2226017 1",
		"//additem 2626017 1",
	
	},	
	
	{
		"//cmdpg80sy",   --80圣佑
		1,
		74,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//paklarge 3 96",
		--绿
		"//additem 1513009 1",
		"//additem 2143009 1",
		"//additem 2043009 1",
		"//additem 1583009 1",
		"//additem 2243009 1",
		"//additem 2643009 1",
		--蓝套
		"//additem 1517009 1",
		"//additem 2147009 1",
		"//additem 2047009 1",
		"//additem 1587009 1",
		"//additem 2247009 1",
		"//additem 2647009 1",
		--蓝
		"//additem 1514009 1",
		"//additem 2144009 1",
		"//additem 2044009 1",
		"//additem 1584009 1",
		"//additem 2244009 1",
		"//additem 2644009 1",
		--紫
		"//additem 1511507 1",
		"//additem 2141507 1",
		"//additem 2041507 1",
		"//additem 1581507 1",
		"//additem 2241507 1",
		"//additem 2641507 1",
		--晶
		"//additem 1511607 1",
		"//additem 2141607 1",
		"//additem 2041607 1",
		"//additem 1581607 1",
		"//additem 2241607 1",
		"//additem 2641607 1",
		--荣耀
		"//additem 1511803 1",
		"//additem 2141803 1",
		"//additem 2041803 1",
		"//additem 1581803 1",
		"//additem 2241803 1",
		"//additem 2641803 1",
		
		--史诗
		"//additem 1511909 1",
		"//additem 2141909 1",
		"//additem 2041909 1",
		"//additem 1581909 1",
		"//additem 2241909 1",
		"//additem 2641909 1",
		--白
		"//additem 1519909 1",
		"//additem 2149909 1",
		"//additem 2049909 1",
		"//additem 1589909 1",
		"//additem 2249909 1",
		"//additem 2649909 1",
		--钻
		"//additem 1511407 1",
		"//additem 2141407 1",
		"//additem 2041407 1",
		"//additem 1581407 1",
		"//additem 2241407 1",
		"//additem 2641407 1",
		
		--英雄
		"//additem 1515009 1",
		"//additem 2145009 1",
		"//additem 2045009 1",
		"//additem 1585009 1",
		"//additem 2245009 1",
		"//additem 2645009 1",
		--橙
		"//additem 1516007 1",
		"//additem 2146007 1",
		"//additem 2046007 1",
		"//additem 1586007 1",
		"//additem 2246007 1",
		"//additem 2646007 1",
	
	},	
	
	{
		"//cmdpg80yl",   --80驭雷
		1,
		74,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//paklarge 3 96",
		
		"//additem 1503009 1",
		"//additem 2143029 1",
		"//additem 2043029 1",
		"//additem 1583029 1",
		"//additem 2243029 1",
		"//additem 2643029 1",

		"//additem 1507009 1",
		"//additem 2147029 1",
		"//additem 2047029 1",
		"//additem 1587029 1",
		"//additem 2247029 1",
		"//additem 2647029 1",

		"//additem 1504009 1",
		"//additem 2144029 1",
		"//additem 2044029 1",
		"//additem 1584029 1",
		"//additem 2244029 1",
		"//additem 2644029 1",

		"//additem 1501507 1",
		"//additem 2141517 1",
		"//additem 2041517 1",
		"//additem 1581517 1",
		"//additem 2241517 1",
		"//additem 2641517 1",

		"//additem 1501607 1",
		"//additem 2141617 1",
		"//additem 2041617 1",
		"//additem 1581617 1",
		"//additem 2241617 1",
		"//additem 2641617 1",

		"//additem 1501803 1",
		"//additem 2141813 1",
		"//additem 2041813 1",
		"//additem 1581813 1",
		"//additem 2241813 1",
		"//additem 2641813 1",

		"//additem 1501909 1",
		"//additem 2141929 1",
		"//additem 2041929 1",
		"//additem 1581929 1",
		"//additem 2241929 1",
		"//additem 2641929 1",

		"//additem 1509909 1",
		"//additem 2149929 1",
		"//additem 2049929 1",
		"//additem 1589929 1",
		"//additem 2249929 1",
		"//additem 2649929 1",
		
		"//additem 1501407 1",
		"//additem 2141417 1",
		"//additem 2041417 1",
		"//additem 1581417 1",
		"//additem 2241417 1",
		"//additem 2641417 1",

		"//additem 1505009 1",
		"//additem 2145029 1",
		"//additem 2045029 1",
		"//additem 1585029 1",
		"//additem 2245029 1",
		"//additem 2645029 1",

		"//additem 1506007 1",
		"//additem 2146017 1",
		"//additem 2046017 1",
		"//additem 1586017 1",
		"//additem 2246017 1",
		"//additem 2646017 1",
				
	},	

	{
		"//cmdpg80ss",   --80首饰
		1,
		58,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//paklarge 3 96",	
		
		"//additem 2417009 1",
		"//additem 2427009 1",
		"//additem 2437009 1",
		"//additem 2447009 1",
		"//additem 2417029 1",
		"//additem 2427029 1",
		"//additem 2437029 1",
		"//additem 2447029 1",

		"//additem 2413009 1",
		"//additem 2414009 1",
		"//additem 2411507 1",
		"//additem 2415009 1",
		"//additem 2419910 1",
		"//additem 2419909 1",

		"//additem 2813009 1",
		"//additem 2814009 1",
		"//additem 2814507 1",
		"//additem 2815009 1",
		"//additem 2819910 1",
		"//additem 2819909 1",

		"//additem 2713008 1",
		"//additem 2714008 1",
		"//additem 2711507 1",
		"//additem 2715008 1",
		"//additem 2715909 1",
		"//additem 2719908 1",

		"//additem 2913009 1",
		"//additem 2914009 1",
		"//additem 2911507 1",
		"//additem 2915009 1",
		"//additem 2919910 1",
		"//additem 2919909 1",

		"//additem 2923009 1",
		"//additem 2924009 1",
		"//additem 2921507 1",
		"//additem 2925009 1",
		"//additem 2921605 1",
		"//additem 2929909 1",
		
		"//additem 2411605 1",
		"//additem 2811605 1",
		"//additem 2711604 1",
		"//additem 2911605 1",
		
		"//additem 2439906 1",
		"//additem 2569909 1",
		"//additem 2539908 1",
		"//additem 2519909 1",

		"//additem 2411909 1",
		"//additem 2811909 1",
		"//additem 2711909 1",
		"//additem 2911909 1",
		"//additem 2911909 1",
		
		"//additem 2416007 1",
		"//additem 2816007 1",
		"//additem 2716007 1",
		"//additem 2916007 1",
		"//additem 2916007 1",



	},	
		
	{
		"//cmdpgqs1",   --枪手副本装备
		1,
		100,
		120,
		"//packageclear 3",
		"//paklarge 3 96",
		"//additem 1201601 1",
		"//additem 2031601 1",
		"//additem 2131601 1",
		"//additem 1271601 1",
		"//additem 2631601 1",
		"//additem 1281601 1",
		"//additem 1201602 1",
		"//additem 2031602 1",
		"//additem 2131602 1",
		"//additem 1271602 1",
		"//additem 2631602 1",
		"//additem 1281602 1",
		"//additem 1201605 1",
		"//additem 1205020 1",
		"//additem 2031605 1",
		"//additem 2131605 1",
		"//additem 1271605 1",
		"//additem 2631605 1",
		"//additem 1281605 1",
		"//additem 1201603 1",
		"//additem 2031603 1",
		"//additem 2131603 1",
		"//additem 1271603 1",
		"//additem 2631603 1",
		"//additem 1281603 1",
		"//additem 1281607 1",
		"//additem 2031607 1",
		"//additem 1201607 1",
		"//additem 1271607 1",
		"//additem 2631607 1",
		"//additem 2131607 1",
		"//additem 1205010 1",
		"//additem 2035010 1",
		"//additem 2135010 1",
		"//additem 1275010 1",
		"//additem 2635010 1",
		"//additem 1285010 1",
		"//additem 1205011 1",
		"//additem 2035011 1",
		"//additem 2135011 1",
		"//additem 1275011 1",
		"//additem 2635011 1",
		"//additem 1285011 1",
		"//additem 1205012 1",
		"//additem 2035012 1",
		"//additem 2135012 1",
		"//additem 1275012 1",
		"//additem 2635012 1",
		"//additem 1285012 1",
		"//additem 1205013 1",
		"//additem 2635013 1",
		"//additem 1285013 1",
		"//additem 2035013 1",
		"//additem 2135013 1",
		"//additem 1275013 1",
		"//additem 1255020 1",
		"//additem 1251605 1",
		"//additem 2031615 1",
		"//additem 2131615 1",
		"//additem 1271615 1",
		"//additem 2631615 1",
		"//additem 1261605 1",
		"//additem 1251603 1",
		"//additem 2031613 1",
		"//additem 2131613 1",
		"//additem 1271613 1",
		"//additem 2631613 1",
		"//additem 1261603 1",
		"//additem 2131617 1",
		"//additem 1261607 1",
		"//additem 2631617 1",
		"//additem 1251607 1",
		"//additem 2031617 1",
		"//additem 1271617 1",
		"//additem 1255010 1",
		"//additem 2035030 1",
		"//additem 2135030 1",
		"//additem 1275030 1",
		"//additem 2635030 1",
		"//additem 1265010 1",
		"//additem 1255011 1",
		"//additem 2035031 1",
		"//additem 2135031 1",
		"//additem 1275031 1",
		"//additem 2635031 1",
		"//additem 1265011 1",
		"//additem 1255012 1",
		"//additem 2035032 1",
		"//additem 2135032 1",
		"//additem 1275032 1",
		"//additem 2635032 1",
		"//additem 1265012 1",
		"//additem 1265013 1",
		"//additem 1275033 1",
		"//additem 1255013 1",
		"//additem 2135033 1",
		"//additem 2035033 1",
		"//additem 2635033 1",

	},
	
	
	{
		"//cmdpgqs2",   --枪手战场装备
		1,
		68,
		120,
		"//packageclear 3",
		"//paklarge 3 96",
		"//additem 1205002 1",
		"//additem 2035002 1",
		"//additem 2135002 1",
		"//additem 1275002 1",
		"//additem 2635002 1",
		"//additem 1205003 1",
		"//additem 2035003 1",
		"//additem 2135003 1",
		"//additem 1275003 1",
		"//additem 2635003 1",
		"//additem 1285003 1",
		"//additem 1205004 1",
		"//additem 2035004 1",
		"//additem 2135004 1",
		"//additem 1275004 1",
		"//additem 2635004 1",
		"//additem 1285004 1",
		"//additem 1205005 1",
		"//additem 2035005 1",
		"//additem 2135005 1",
		"//additem 1275005 1",
		"//additem 2635005 1",
		"//additem 1285005 1",
		"//additem 1205006 1",
		"//additem 2035006 1",
		"//additem 2135006 1",
		"//additem 1275006 1",
		"//additem 2635006 1",
		"//additem 1285006 1",
		"//additem 1205007 1",
		"//additem 2035007 1",
		"//additem 2135007 1",
		"//additem 1275007 1",
		"//additem 2635007 1",
		"//additem 1285007 1",
		"//additem 1205008 1",
		"//additem 2035008 1",
		"//additem 2135008 1",
		"//additem 1275008 1",
		"//additem 2635008 1",
		"//additem 1285008 1",
		"//additem 2135009 1",
		"//additem 1205009 1",
		"//additem 1285009 1",
		"//additem 2635009 1",
		"//additem 2035009 1",
		"//additem 1275009 1",
		"//additem 1255007 1",
		"//additem 2035027 1",
		"//additem 2135027 1",
		"//additem 1275027 1",
		"//additem 2635027 1",
		"//additem 1265007 1",
		"//additem 1255008 1",
		"//additem 2035028 1",
		"//additem 2135028 1",
		"//additem 1275028 1",
		"//additem 2635028 1",
		"//additem 1265008 1",
		"//additem 1275029 1",
		"//additem 2035029 1",
		"//additem 1265009 1",
		"//additem 1255009 1",
		"//additem 2135029 1",
		"//additem 2635029 1",
	},
	
	{
		"//cmdpgqs3",   --枪手Boss装备
		1,
		32,
		120,
		"//packageclear 3",
		"//paklarge 3 96",
		"//additem 1201801 1",
		"//additem 2031801 1",
		"//additem 2131801 1",
		"//additem 1271801 1",
		"//additem 2631801 1",
		"//additem 1281801 1",
		"//additem 1201802 1",
		"//additem 2031802 1",
		"//additem 2131802 1",
		"//additem 1271802 1",
		"//additem 2631802 1",
		"//additem 1281802 1",
		"//additem 2131803 1",
		"//additem 2031803 1",
		"//additem 2631803 1",
		"//additem 1201803 1",
		"//additem 1281803 1",
		"//additem 1271803 1",
		"//additem 1251802 1",
		"//additem 2031822 1",
		"//additem 2131822 1",
		"//additem 1271827 1",
		"//additem 2631822 1",
		"//additem 1261802 1",
		"//additem 1261803 1",
		"//additem 1271813 1",
		"//additem 1251803 1",
		"//additem 2031813 1",
		"//additem 2631813 1",
		"//additem 2131813 1",
		
	},

	{
		"//cmdpgqs4",   --枪手Boss装备
		1,
		32,
		120,
		"//packageclear 3",
		"//paklarge 3 96",
		"//additem 1201905 1",
		"//additem 2031905 1",
		"//additem 2131905 1",
		"//additem 1271905 1",
		"//additem 2631905 1",
		"//additem 1281905 1",
		"//additem 1201907 1",
		"//additem 2031907 1",
		"//additem 2131907 1",
		"//additem 1271907 1",
		"//additem 2631907 1",
		"//additem 1281907 1",
		"//additem 2131909 1",
		"//additem 2031909 1",
		"//additem 2631909 1",
		"//additem 1201909 1",
		"//additem 1281909 1",
		"//additem 1271909 1",
		"//additem 1251907 1",
		"//additem 2031927 1",
		"//additem 2131927 1",
		"//additem 1271927 1",
		"//additem 2631927 1",
		"//additem 1261907 1",
		"//additem 1261909 1",
		"//additem 2031929 1",
		"//additem 2131929 1",
		"//additem 1251909 1",
		"//additem 1271929 1",
		"//additem 2631929 1",
		
	},
	
	{
		"//cmdpgfs1",   --法师副本装备
		1,
		100,
		120,
		"//packageclear 3",
		"//paklarge 3 96",
		"//additem 1101601 1",
		"//additem 2021601 1",
		"//additem 2121601 1",
		"//additem 2221601 1",
		"//additem 2621601 1",
		"//additem 1181601 1",
		"//additem 1101602 1",
		"//additem 2021602 1",
		"//additem 2121602 1",
		"//additem 2221602 1",
		"//additem 2621602 1",
		"//additem 1181602 1",
		"//additem 1101605 1",
		"//additem 1105020 1",
		"//additem 2021605 1",
		"//additem 2121605 1",
		"//additem 2221605 1",
		"//additem 2621605 1",
		"//additem 1181605 1",
		"//additem 1101603 1",
		"//additem 2021603 1",
		"//additem 2121603 1",
		"//additem 2221603 1",
		"//additem 2621603 1",
		"//additem 1181603 1",
		"//additem 2021607 1",
		"//additem 2221607 1",
		"//additem 2121607 1",
		"//additem 1101607 1",
		"//additem 2621607 1",
		"//additem 1181607 1",
		"//additem 1105010 1",
		"//additem 2025010 1",
		"//additem 2125010 1",
		"//additem 2225010 1",
		"//additem 2625010 1",
		"//additem 1185010 1",
		"//additem 1105011 1",
		"//additem 2025011 1",
		"//additem 2125011 1",
		"//additem 2225011 1",
		"//additem 2625011 1",
		"//additem 1185011 1",
		"//additem 1105012 1",
		"//additem 2025012 1",
		"//additem 2125012 1",
		"//additem 2225012 1",
		"//additem 2625012 1",
		"//additem 1185012 1",
		"//additem 2625013 1",
		"//additem 2025013 1",
		"//additem 1105013 1",
		"//additem 2225013 1",
		"//additem 1185013 1",
		"//additem 2125013 1",
		"//additem 1115020 1",
		"//additem 1111605 1",
		"//additem 2021615 1",
		"//additem 2121615 1",
		"//additem 2221615 1",
		"//additem 2621615 1",
		"//additem 1181615 1",
		"//additem 1111603 1",
		"//additem 2021613 1",
		"//additem 2121613 1",
		"//additem 2221613 1",
		"//additem 2621613 1",
		"//additem 1181613 1",
		"//additem 1181617 1",
		"//additem 2221617 1",
		"//additem 2621617 1",
		"//additem 1111607 1",
		"//additem 2021617 1",
		"//additem 2121617 1",
		"//additem 1115010 1",
		"//additem 2025030 1",
		"//additem 2125030 1",
		"//additem 2225030 1",
		"//additem 2625030 1",
		"//additem 1185030 1",
		"//additem 1115011 1",
		"//additem 2025031 1",
		"//additem 2125031 1",
		"//additem 2225031 1",
		"//additem 2625031 1",
		"//additem 1185031 1",
		"//additem 1115012 1",
		"//additem 2025032 1",
		"//additem 2125032 1",
		"//additem 2225032 1",
		"//additem 2625032 1",
		"//additem 1185032 1",
		"//additem 1185033 1",
		"//additem 2025033 1",
		"//additem 1115013 1",
		"//additem 2225033 1",
		"//additem 2125033 1",
		"//additem 2625033 1",
		
		

	},
	
	
	{
		"//cmdpgfs2",   --法师战场装备
		1,
		68,
		120,
		"//packageclear 3",
		"//paklarge 3 96",
		"//additem 1105002 1",
		"//additem 2025002 1",
		"//additem 2125002 1",
		"//additem 2225002 1",
		"//additem 2625002 1",
		"//additem 1185002 1",
		"//additem 1105003 1",
		"//additem 2025003 1",
		"//additem 2125003 1",
		"//additem 2225003 1",
		"//additem 2625003 1",
		"//additem 1185003 1",
		"//additem 1105004 1",
		"//additem 2025004 1",
		"//additem 2125004 1",
		"//additem 2225004 1",
		"//additem 2625004 1",
		"//additem 1185004 1",
		"//additem 1105005 1",
		"//additem 2025005 1",
		"//additem 2125005 1",
		"//additem 2225005 1",
		"//additem 2625005 1",
		"//additem 1185005 1",
		"//additem 1105006 1",
		"//additem 2025006 1",
		"//additem 2125006 1",
		"//additem 2225006 1",
		"//additem 2625006 1",
		"//additem 1185006 1",
		"//additem 1105007 1",
		"//additem 2025007 1",
		"//additem 2125007 1",
		"//additem 2225007 1",
		"//additem 2625007 1",
		"//additem 1185007 1",
		"//additem 1105008 1",
		"//additem 2025008 1",
		"//additem 2125008 1",
		"//additem 2225008 1",
		"//additem 2625008 1",
		"//additem 1185008 1",
		"//additem 2125009 1",
		"//additem 1105009 1",
		"//additem 2225009 1",
		"//additem 2625009 1",
		"//additem 2025009 1",
		"//additem 1185009 1",
		"//additem 1115007 1",
		"//additem 2025027 1",
		"//additem 2125027 1",
		"//additem 2225027 1",
		"//additem 2625027 1",
		"//additem 1185027 1",
		"//additem 1115008 1",
		"//additem 2025028 1",
		"//additem 2125028 1",
		"//additem 2225028 1",
		"//additem 2625028 1",
		"//additem 1185028 1",
		"//additem 1115009 1",
		"//additem 2225029 1",
		"//additem 2025029 1",
		"//additem 1185029 1",
		"//additem 2125029 1",
		"//additem 2625029 1",
		

	},
		
	{
		"//cmdpgfs3",   --法师boss装备
		1,
		32,
		120,
		"//packageclear 3",
		"//paklarge 3 96",
		"//additem 1101801 1",
		"//additem 2021801 1",
		"//additem 2121801 1",
		"//additem 2221801 1",
		"//additem 2621801 1",
		"//additem 1181801 1",
		"//additem 1101802 1",
		"//additem 2021802 1",
		"//additem 2121802 1",
		"//additem 2221802 1",
		"//additem 2621802 1",
		"//additem 1181802 1",
		"//additem 2221803 1",
		"//additem 2021803 1",
		"//additem 1101803 1",
		"//additem 1181803 1",
		"//additem 2621803 1",
		"//additem 2121803 1",
		"//additem 1111802 1",
		"//additem 2021822 1",
		"//additem 2121822 1",
		"//additem 2221822 1",
		"//additem 2621822 1",
		"//additem 1181822 1",
		"//additem 2221813 1",
		"//additem 2021813 1",
		"//additem 1181813 1",
		"//additem 1111803 1",
		"//additem 2621813 1",
		"//additem 2121813 1",

	},	
	
	{
		"//cmdpgfs4",   --法师boss装备
		1,
		32,
		120,
		"//packageclear 3",
		"//paklarge 3 96",
		"//additem 1101905 1",
		"//additem 2021905 1",
		"//additem 2121905 1",
		"//additem 2221905 1",
		"//additem 2621905 1",
		"//additem 1181905 1",
		"//additem 1101907 1",
		"//additem 2021907 1",
		"//additem 2121907 1",
		"//additem 2221907 1",
		"//additem 2621907 1",
		"//additem 1181907 1",
		"//additem 2221909 1",
		"//additem 2021909 1",
		"//additem 1101909 1",
		"//additem 1181909 1",
		"//additem 2621909 1",
		"//additem 2121909 1",
		"//additem 1111907 1",
		"//additem 2021927 1",
		"//additem 2121927 1",
		"//additem 2221927 1",
		"//additem 2621927 1",
		"//additem 1181927 1",
		"//additem 1111909 1",
		"//additem 1181929 1",
		"//additem 2221929 1",
		"//additem 2021929 1",
		"//additem 2121929 1",
		"//additem 2621929 1",

	},
	
	{
		"//cmdpgjs1",   --祭司副本装备
		1,
		100,
		120,
		"//packageclear 3",
		"//paklarge 3 96",
		"//additem 1501601 1",
		"//additem 2041601 1",
		"//additem 2141601 1",
		"//additem 2241601 1",
		"//additem 2641601 1",
		"//additem 1581601 1",
		"//additem 1501602 1",
		"//additem 2041602 1",
		"//additem 2141602 1",
		"//additem 2241602 1",
		"//additem 2641602 1",
		"//additem 1581602 1",
		"//additem 1501605 1",
		"//additem 1515020 1",
		"//additem 2041605 1",
		"//additem 2141605 1",
		"//additem 2241605 1",
		"//additem 2641605 1",
		"//additem 1581605 1",
		"//additem 1511603 1",
		"//additem 2041603 1",
		"//additem 2141603 1",
		"//additem 2241603 1",
		"//additem 2641603 1",
		"//additem 1581603 1",
		"//additem 2041607 1",
		"//additem 2141607 1",
		"//additem 2241607 1",
		"//additem 1511607 1",
		"//additem 2641607 1",
		"//additem 1581607 1",
		"//additem 1515010 1",
		"//additem 2045010 1",
		"//additem 2145010 1",
		"//additem 2245010 1",
		"//additem 2645010 1",
		"//additem 1585010 1",
		"//additem 1515011 1",
		"//additem 2045011 1",
		"//additem 2145011 1",
		"//additem 2245011 1",
		"//additem 2645011 1",
		"//additem 1585011 1",
		"//additem 1515012 1",
		"//additem 2045012 1",
		"//additem 2145012 1",
		"//additem 2245012 1",
		"//additem 2645012 1",
		"//additem 1585012 1",
		"//additem 2045013 1",
		"//additem 1515013 1",
		"//additem 2245013 1",
		"//additem 1585013 1",
		"//additem 2645013 1",
		"//additem 2145013 1",
		"//additem 1505020 1",
		"//additem 1511605 1",
		"//additem 2041615 1",
		"//additem 2141615 1",
		"//additem 2241615 1",
		"//additem 2641615 1",
		"//additem 1581615 1",
		"//additem 1501603 1",
		"//additem 2041613 1",
		"//additem 2141613 1",
		"//additem 2241613 1",
		"//additem 2641613 1",
		"//additem 1581613 1",
		"//additem 1581617 1",
		"//additem 2241617 1",
		"//additem 2641617 1",
		"//additem 1501607 1",
		"//additem 2041617 1",
		"//additem 2141617 1",
		"//additem 1505010 1",
		"//additem 2045030 1",
		"//additem 2145030 1",
		"//additem 2245030 1",
		"//additem 2645030 1",
		"//additem 1585030 1",
		"//additem 1505011 1",
		"//additem 2045031 1",
		"//additem 2145031 1",
		"//additem 2245031 1",
		"//additem 2645031 1",
		"//additem 1585031 1",
		"//additem 1505012 1",
		"//additem 2045032 1",
		"//additem 2145032 1",
		"//additem 2245032 1",
		"//additem 2645032 1",
		"//additem 1585032 1",
		"//additem 1585033 1",
		"//additem 2045033 1",
		"//additem 1505013 1",
		"//additem 2245033 1",
		"//additem 2145033 1",
		"//additem 2645033 1",

		
	},
	
	
	{
		"//cmdpgjs2",   --祭司战场装备
		1,
		68,
		120,
		"//packageclear 3",
		"//paklarge 3 96",
		"//additem 1505002 1",
		"//additem 2045002 1",
		"//additem 2145002 1",
		"//additem 2245002 1",
		"//additem 2645002 1",
		"//additem 1585002 1",
		"//additem 1505003 1",
		"//additem 2045003 1",
		"//additem 2145003 1",
		"//additem 2245003 1",
		"//additem 2645003 1",
		"//additem 1585003 1",
		"//additem 1505004 1",
		"//additem 2045004 1",
		"//additem 2145004 1",
		"//additem 2245004 1",
		"//additem 2645004 1",
		"//additem 1585004 1",
		"//additem 1505005 1",
		"//additem 2045005 1",
		"//additem 2145005 1",
		"//additem 2245005 1",
		"//additem 2645005 1",
		"//additem 1585005 1",
		"//additem 1505006 1",
		"//additem 2045006 1",
		"//additem 2145006 1",
		"//additem 2245006 1",
		"//additem 2645006 1",
		"//additem 1585006 1",
		"//additem 1515007 1",
		"//additem 2045007 1",
		"//additem 2145007 1",
		"//additem 2245007 1",
		"//additem 2645007 1",
		"//additem 1585007 1",
		"//additem 1515008 1",
		"//additem 2045008 1",
		"//additem 2145008 1",
		"//additem 2245008 1",
		"//additem 2645008 1",
		"//additem 1585008 1",
		"//additem 2045009 1",
		"//additem 2245009 1",
		"//additem 1515009 1",
		"//additem 1585009 1",
		"//additem 2645009 1",
		"//additem 2145009 1",
		"//additem 1505007 1",
		"//additem 2045027 1",
		"//additem 2145027 1",
		"//additem 2245027 1",
		"//additem 2645027 1",
		"//additem 1585027 1",
		"//additem 1505008 1",
		"//additem 2045028 1",
		"//additem 2145028 1",
		"//additem 2245028 1",
		"//additem 2645028 1",
		"//additem 1585028 1",
		"//additem 2245029 1",
		"//additem 2045029 1",
		"//additem 1585029 1",
		"//additem 2145029 1",
		"//additem 1505009 1",
		"//additem 2645029 1",
		
	
	},
	
	{
		"//cmdpgjs3",   --祭司boss装备
		1,
		32,
		120,
		"//packageclear 3",
		"//paklarge 3 96",
		"//additem 1501801 1",
		"//additem 2041801 1",
		"//additem 2141801 1",
		"//additem 2241801 1",
		"//additem 2641801 1",
		"//additem 1581801 1",
		"//additem 1501802 1",
		"//additem 2041802 1",
		"//additem 2141802 1",
		"//additem 2241802 1",
		"//additem 2641802 1",
		"//additem 1581802 1",
		"//additem 2641803 1",
		"//additem 1581803 1",
		"//additem 2041803 1",
		"//additem 2241803 1",
		"//additem 1511803 1",
		"//additem 2141803 1",
		"//additem 1501822 1",
		"//additem 2041822 1",
		"//additem 2141822 1",
		"//additem 2241822 1",
		"//additem 2641822 1",
		"//additem 1581822 1",
		"//additem 2241813 1",
		"//additem 2041813 1",
		"//additem 1501803 1",
		"//additem 1581813 1",
		"//additem 2641813 1",
		"//additem 2141813 1",
	
	},
	
	{
		"//cmdpgjs4",   --祭司boss装备
		1,
		32,
		120,
		"//packageclear 3",
		"//paklarge 3 96",
		"//additem 1501905 1",
		"//additem 2041905 1",
		"//additem 2141905 1",
		"//additem 2241905 1",
		"//additem 2641905 1",
		"//additem 1581905 1",
		"//additem 1511907 1",
		"//additem 2041907 1",
		"//additem 2141907 1",
		"//additem 2241907 1",
		"//additem 2641907 1",
		"//additem 1581907 1",
		"//additem 1511909 1",
		"//additem 2241909 1",
		"//additem 2141909 1",
		"//additem 1581909 1",
		"//additem 2641909 1",
		"//additem 2041909 1",
		"//additem 1501907 1",
		"//additem 2041927 1",
		"//additem 2141927 1",
		"//additem 2241927 1",
		"//additem 2641927 1",
		"//additem 1581927 1",
		"//additem 1581929 1",
		"//additem 2241929 1",
		"//additem 2141929 1",
		"//additem 2041929 1",
		"//additem 1501909 1",
		"//additem 2641929 1",

	
	},
		
	{
		"//cmdpgss1",   --60/70级首饰
		1,
		38,
		120,
		"//packageclear 3",
		"//paklarge 3 96",
		"//additem 2413007 1",
		"//additem 2414007 1",
		"//additem 2415007 1",
		"//additem 2419907 1",
		"//additem 2423007 1",
		"//additem 2424007 1",
		"//additem 2425007 1",
		"//additem 2429907 1",
		"//additem 2433007 1",
		"//additem 2434007 1",
		"//additem 2435007 1",
		"//additem 2439907 1",
		"//additem 2443007 1",
		"//additem 2444007 1",
		"//additem 2445007 1",
		"//additem 2449907 1",
		"//additem 2454007 1",
		"//additem 2455007 1",
		"//additem 2813007 1",
		"//additem 2814007 1",
		"//additem 2815007 1",
		"//additem 2819907 1",
		"//additem 2913007 1",
		"//additem 2914007 1",
		"//additem 2915007 1",
		"//additem 2919907 1",
		"//additem 2923007 1",
		"//additem 2924007 1",
		"//additem 2925007 1",
		"//additem 2929907 1",
		"//additem 2713006 1",
		"//additem 2714006 1",
		"//additem 2715006 1",
		"//additem 2718006 1",
		"//additem 2719906 1",
		"//additem 2723007 1",
		"//additem 2724007 1",
		
	},	
	
	{
		"//cmdpgss2",   --60/70级首饰
		1,
		42,
		120,
		"//packageclear 3",
		"//paklarge 3 96",
		"//additem 2411603 1",
		"//additem 2413008 1",
		"//additem 2414008 1",
		"//additem 2415008 1",
		"//additem 2419908 1",
		"//additem 2421603 1",
		"//additem 2423008 1",
		"//additem 2424008 1",
		"//additem 2425008 1",
		"//additem 2429908 1",
		"//additem 2431603 1",
		"//additem 2433008 1",
		"//additem 2434008 1",
		"//additem 2435008 1",
		"//additem 2439908 1",
		"//additem 2441603 1",
		"//additem 2443008 1",
		"//additem 2444008 1",
		"//additem 2445008 1",
		"//additem 2449908 1",
		"//additem 2454008 1",
		"//additem 2455008 1",
		"//additem 2813008 1",
		"//additem 2814008 1",
		"//additem 2815008 1",
		"//additem 2819908 1",
		"//additem 2913008 1",
		"//additem 2914008 1",
		"//additem 2915008 1",
		"//additem 2919908 1",
		"//additem 2923008 1",
		"//additem 2924008 1",
		"//additem 2925008 1",
		"//additem 2929908 1",
		"//additem 2713007 1",
		"//additem 2714007 1",
		"//additem 2715007 1",
		"//additem 2718007 1",
		"//additem 2719907 1",
		"//additem 2723008 1",
		"//additem 2724008 1",
	
		
	},
	
	{
		"//cmdpgg",  		--积累数
		0,
		2,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//addcollectnum 1000",
		"//skillcool",
		
	},
	
	{
		"//cmdpglv",
		0,
		3,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//addattr exp=2100000000",
		"//additem 5030021 1",
		"//cleardayexp",
	
	},
		
	{
		"//cmdpggw",  		--标准怪物
		0,
		2,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//applystatus 0 1051 1 10000",   --无敌
		"//monster 233020 1",
		
	},
	{
		"//cmdpgskill",  		--60技能
		0,
		42,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//additem 5030022 1",
		"//additem 5030023 1",
		"//additem 5030024 1",
		"//additem 5030025 1",
		"//additem 5030026 1",
		"//additem 5030027 1",
		"//additem 5030028 1",
		"//additem 5030029 1",
		"//additem 5030030 1",
		"//additem 5030031 1",
		"//additem 5030032 1",
		"//additem 5030033 1",
		"//additem 5030034 1",
		"//additem 5030035 1",
		"//additem 5030036 1",
		"//additem 5030037 1",
		"//additem 5030043 1",
		"//additem 5030044 1",
		"//additem 5030045 1",
		"//additem 5030046 1",
		"//additem 5030047 1",
		"//additem 5030048 1",
		"//additem 5030049 1",
		"//additem 5030050 1",
		"//additem 5030051 1",
		"//additem 5030052 1",
		"//additem 5030053 1",
		"//additem 5030054 1",
		"//additem 5030055 1",
		"//additem 5030056 1",
		"//additem 5030057 1",
		"//additem 5030058 1",
		"//additem 5030059 1",
		"//additem 5030060 1",
		"//additem 5030061 1",
		"//additem 5030062 1",
		"//additem 5030063 1",
		"//additem 5030064 1",
		"//additem 5030065 1",
		"//additem 5030066 1",
		"//additem 5030067 1",
	
	},
	
	{
		"//cmdpgjx",  		--机械
		0,
		41,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//additem 5051000 99",
		"//additem 5051001 99",
		"//additem 5051002 99",
		"//additem 5051003 99",
		"//additem 5051004 2",
		"//additem 5051005 2",
		"//additem 5051006 99",
		"//additem 5051007 99",
		"//additem 5051008 99",
		"//additem 5051009 99",
		"//additem 5051010 2",
		"//additem 5051011 2",
		"//additem 5051012 99",
		"//additem 5051015 99",
		"//additem 5051017 2",
		"//additem 5051023 2",
		
		"//additem 5051097 2",
		"//additem 5051098 2",
		"//additem 5051099 2",
		
		"//additem 5051100 2",
		"//additem 5051101 2",
		"//additem 5051102 2",
		"//additem 5051103 2",
		"//additem 5051104 2",
		"//additem 5051105 2",
		"//additem 5051106 2",
				
		"//additem 5051036 2",
		"//additem 5051037 2",
		"//additem 5051039 2",
		"//additem 5051040 2",
		"//additem 5051041 2",
		"//additem 5051046 2",
		"//additem 5051047 2",
		"//additem 5051112 2",
		"//additem 5051113 2",
		"//additem 5030005 10",
		"//additem 5030006 10",
		"//additem 5051048 99",
		"//additem 5051049 99",
		"//additem 5060025 99",
		
	
	},
}

function InsterGMPackage( cmdpg )
	table.insert( cmdTable,cmdpg );
end

function CallGMPackage(cmd, limit)
	local mainPlayer = ActorMgr:getMainPlayer();
	local playerType = mainPlayer:getRace();

	for i = 1, table.getn(cmdTable) do
		local str = string.format("^%s$", cmdTable[i][1])
		local result = pack(string.find(cmd, str))
		if result[1] and (not limit or mainPlayer:getLv() <= cmdTable[i][3]) then   --忽略掉职业的判断
--		if result[1] and (playerType == cmdTable[i][2] or cmdTable[i][2] == 0) and (not limit or mainPlayer:getLv() <= cmdTable[i][3]) then
			for j = 5, #cmdTable[i] do
				str = cmdTable[i][j]
				for k = 3, #result do
					str = string.gsub(str, "%%"..(k - 2), result[k])
				end
				if string.find(str, "^//cmdpg") then
					CallGMPackage(str, true)
				else
					SetGMExcCmd(str)
				end
			end
		end
	end
	SetGMCMDBeginExc( os.clock() );
end


------------------------------------机器人特效测试用---------------------------------------------------------
local t_RobotMotion = {"220800_8","352102","282018_18","10291_8","10241_4","203400_8_9999","211200_6","9513_16","10129_2","121000_4"}

function RobotTestGMProc( szMsg,szSpeaker )
	if szMsg == "Hello, world" then
		local actor = ActorMgr:findActorByName( szSpeaker )
		if actor ~= nil then
			local nIdx = math.random( 1,table.getn( t_RobotMotion ) );
			actor:playMotion( t_RobotMotion[nIdx] );
		end
	end
end