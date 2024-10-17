RANGE_MAIN_SINGLE				= 0; -- 主类型： 个人
RANGE_MAIN_CAREER				= 1; -- 主类型： 职业
RANGE_MAIN_CLAN					= 2; -- 主类型： 公会
RANGE_MAIN_PET					= 3; -- 主类型： 宠物
RANGE_MAIN_COMBAT				= 4; -- 主类型： 副本
RANGE_MAIN_BATTLE				= 5; -- 主类型： 战场
RANGE_MAIN_SPARTA				= 6; -- 主类型：	竞技场 --显示为功勋
RANGE_MAIN_ATTR					= 7; -- 主类型： 属性

RANGE_MAIN_SINGLE_SUB_LEVEL	       = 0; -- 个人 等级
RANGE_MAIN_SINGLE_SUB_MONEY	       = 1; -- 个人 财富
RANGE_MAIN_SINGLE_SUB_PK	       = 2; -- 个人 杀气
RANGE_MAIN_SINGLE_SUB_CLANCT	       = 3; -- 个人 公会贡献 -- 取消此类型显示 13-09-22
RANGE_MAIN_SINGLE_SUB_GENIUS		= 4;	-- 个人 天赋值
RANGE_MAIN_SINGLE_SUB_CHARM			= 5;	-- 个人 魅力
RANGE_MAIN_SINGLE_SUB_CHARM_WEEK	= 6;	-- 个人 周魅力


RANGE_MAIN_CAREER_SUB_WARRIOR	       = 0; -- 职业 战士
RANGE_MAIN_CAREER_SUB_MAGICOR	       = 1; -- 职业 法师
RANGE_MAIN_CAREER_SUB_GUNER	       = 2; -- 职业 枪手
RANGE_MAIN_CAREER_SUB_CHURCH	       = 3; -- 职业 祭司

RANGE_MAIN_CLAN_SUB_DEVELOP	       = 0; -- 公会 繁荣度
RANGE_MAIN_CLAN_SUB_MEM		       = 1; -- 公会 人数
RANGE_MAIN_CLAN_SUB_COMBAT	       = 2; -- 公会 统治时间
RANGE_MAIN_CLAN_SUB_LEVEL	       = 3; -- 公会 等级

RANGE_MAIN_PET_SUB_FIGHT	       = 0; -- 宠物 战斗力
--[[
RANGE_MAIN_COMBAT_SUB_PANSHAN_TIME     = 3 -- 神速盘山道
RANGE_MAIN_COMBAT_SUB_DRAGON_TIME      = 4 -- 神速龙蛋谷
RANGE_MAIN_COMBAT_SUB_MOGUI_TIME       = 5 -- 神速太阳神殿
--]]
RANGE_MAIN_COMBAT_SUB_PANSHAN_NUM      = 6 -- 盘山道通关
RANGE_MAIN_COMBAT_SUB_DRAGON_NUM       = 7 -- 龙蛋谷通关
--RANGE_MAIN_COMBAT_SUB_MOGUI_NUM        = 8 -- 太阳神殿通关
RANGE_MAIN_COMBAT_SUB_BCT              = 9 -- 大地之心

RANGE_MAIN_BATTLE_SUB_SURVIVE_GOD      = 0  -- 生存超神
RANGE_MAIN_BATTLE_SUB_SURVIVE_KILLER   = 1  -- 生存大魔王
RANGE_MAIN_BATTLE_SUB_SURVIVE_NODEAD   = 2  -- 生存不死身
RANGE_MAIN_BATTLE_SUB_LZSURVIVE_GOD    = 3  -- 绿洲风暴超神
RANGE_MAIN_BATTLE_SUB_LZSURVIVE_KILLER = 4  -- 绿洲风暴大魔王
RANGE_MAIN_BATTLE_SUB_LZSURVIVE_NODEAD = 5  -- 绿洲风暴不死身
RANGE_MAIN_BATTLE_SUB_LZSURVIVE_END    = 6  -- 绿洲风暴终结者 
RANGE_MAIN_BATTLE_SUB_LZWIN_NUM		   = 7  -- 绿洲优胜榜单
RANGE_MAIN_BATTLE_SUB_KUANGSHA_WIN_NUM = 8  -- 狂沙优胜榜单

RANGE_MAIN_SPARTA_SUB_FIGHT				= 0 -- 战斗力排行榜 --显示为末世荣耀殿堂

RANGE_MAIN_ATTR_SUB_HP					= 0 --生命值
RANGE_MAIN_ATTR_SUB_MP					= 1 --魔法值
RANGE_MAIN_ATTR_SUB_WULI_ATK			= 2 --物理攻击
RANGE_MAIN_ATTR_SUB_MOFA_ATK			= 3 --魔法攻击
RANGE_MAIN_ATTR_SUB_WULI_DEF			= 4 --物理防御
RANGE_MAIN_ATTR_SUB_MOFA_DEF			= 5 --魔法防御
RANGE_MAIN_ATTR_SUB_BAOJI_LVL			= 6 --暴击等级
RANGE_MAIN_ATTR_SUB_BAOJI_HURT			= 7 --暴击伤害
RANGE_MAIN_ATTR_SUB_DIKANG_LVL			= 8 --抵抗等级
RANGE_MAIN_ATTR_SUB_POFANG_LVL			= 9 --破防等级

local MAX_SHOW_COUNT = 16; -- 每页显示的条数

-- 标签名表
local t_LabelNameList =	{
				{ name="个人",		 bCheck = true,  nType = RANGE_MAIN_SINGLE, nSubType = 0                                          },
				{ name="等级",		 bCheck = true,  nType = RANGE_MAIN_SINGLE, nSubType = RANGE_MAIN_SINGLE_SUB_LEVEL            + 1 },
				{ name="财富",		 bCheck = true,  nType = RANGE_MAIN_SINGLE, nSubType = RANGE_MAIN_SINGLE_SUB_MONEY            + 1 },
				{ name="天赋值",	 bCheck = true,	 nType = RANGE_MAIN_SINGLE,	nSubType = RANGE_MAIN_SINGLE_SUB_GENIUS			  + 1 },
				{ name="杀气",		 bCheck = true,  nType = RANGE_MAIN_SINGLE, nSubType = RANGE_MAIN_SINGLE_SUB_PK               + 1 },
				--{ name="公会贡献",	 bCheck = true,  nType = RANGE_MAIN_SINGLE, nSubType = RANGE_MAIN_SINGLE_SUB_CLANCT           + 1 },
				{ name="魅力",		 bCheck = true,	 nType = RANGE_MAIN_SINGLE, nSubType = RANGE_MAIN_SINGLE_SUB_CHARM			  + 1 },
				{ name="周魅力",	 bCheck = true,  nType = RANGE_MAIN_SINGLE, nSubType = RANGE_MAIN_SINGLE_SUB_CHARM_WEEK       + 1 },

				{ name="职业",		 bCheck = false, nType = RANGE_MAIN_CAREER, nSubType = 0                                          },
				{ name="战士",		 bCheck = true,  nType = RANGE_MAIN_CAREER, nSubType = RANGE_MAIN_CAREER_SUB_WARRIOR          + 1 },
				{ name="法师",		 bCheck = true,  nType = RANGE_MAIN_CAREER, nSubType = RANGE_MAIN_CAREER_SUB_MAGICOR          + 1 },
				{ name="枪手",		 bCheck = true,  nType = RANGE_MAIN_CAREER, nSubType = RANGE_MAIN_CAREER_SUB_GUNER            + 1 },
				{ name="祭司",		 bCheck = true,  nType = RANGE_MAIN_CAREER, nSubType = RANGE_MAIN_CAREER_SUB_CHURCH           + 1 },
				
				{ name="属性",	bCheck = false, nType = RANGE_MAIN_ATTR, nSubType = 0 },
				{ name="生命值",	bCheck = true,	nType = RANGE_MAIN_ATTR, nSubType = RANGE_MAIN_ATTR_SUB_HP	+ 1 },
				{ name="魔法值",	bCheck = true,	nType = RANGE_MAIN_ATTR, nSubType = RANGE_MAIN_ATTR_SUB_MP	+ 1 },
				{ name="物理攻击",	bCheck = true,	nType = RANGE_MAIN_ATTR, nSubType = RANGE_MAIN_ATTR_SUB_WULI_ATK	+ 1 },
				{ name="魔法攻击",	bCheck = true,	nType = RANGE_MAIN_ATTR, nSubType = RANGE_MAIN_ATTR_SUB_MOFA_ATK	+ 1 },
				{ name="物理防御",	bCheck = true,	nType = RANGE_MAIN_ATTR, nSubType = RANGE_MAIN_ATTR_SUB_WULI_DEF	+ 1 },
				{ name="魔法防御",	bCheck = true,	nType = RANGE_MAIN_ATTR, nSubType = RANGE_MAIN_ATTR_SUB_MOFA_DEF	+ 1 },
				{ name="暴击等级",	bCheck = true,	nType = RANGE_MAIN_ATTR, nSubType = RANGE_MAIN_ATTR_SUB_BAOJI_LVL	+ 1 },
				{ name="暴击伤害",	bCheck = true,	nType = RANGE_MAIN_ATTR, nSubType = RANGE_MAIN_ATTR_SUB_BAOJI_HURT	+ 1 },
				{ name="抵抗等级",	bCheck = true,	nType = RANGE_MAIN_ATTR, nSubType = RANGE_MAIN_ATTR_SUB_DIKANG_LVL	+ 1 },
				{ name="破防等级",	bCheck = true,	nType = RANGE_MAIN_ATTR, nSubType = RANGE_MAIN_ATTR_SUB_POFANG_LVL	+ 1 },

				{ name="公会",		 bCheck = false, nType = RANGE_MAIN_CLAN,   nSubType = 0                                          },
				{ name="等级",           bCheck = true,  nType = RANGE_MAIN_CLAN,   nSubType = RANGE_MAIN_CLAN_SUB_LEVEL              + 1 },
				{ name="繁荣度",	 bCheck = true,  nType = RANGE_MAIN_CLAN,   nSubType = RANGE_MAIN_CLAN_SUB_DEVELOP            + 1 },
				{ name="人数",		 bCheck = true,  nType = RANGE_MAIN_CLAN,   nSubType = RANGE_MAIN_CLAN_SUB_MEM                + 1 },
				{ name="统治时间",	 bCheck = true,  nType = RANGE_MAIN_CLAN,   nSubType = RANGE_MAIN_CLAN_SUB_COMBAT             + 1 },

				{ name="宠物",		 bCheck = false, nType = RANGE_MAIN_PET,    nSubType = 0                                          },
				{ name="战斗力",	 bCheck = true,  nType = RANGE_MAIN_PET,    nSubType = RANGE_MAIN_PET_SUB_FIGHT               + 1 },

				{ name="副本",		 bCheck = false, nType = RANGE_MAIN_COMBAT, nSubType = 0                                          },
			--[[	
				{ name="神速盘山道",	 bCheck = true,  nType = RANGE_MAIN_COMBAT, nSubType = RANGE_MAIN_COMBAT_SUB_PANSHAN_TIME     + 1 },
				{ name="神速龙蛋谷",	 bCheck = true,  nType = RANGE_MAIN_COMBAT, nSubType = RANGE_MAIN_COMBAT_SUB_DRAGON_TIME      + 1 },
				{ name="神速太阳神殿",	 bCheck = true,  nType = RANGE_MAIN_COMBAT, nSubType = RANGE_MAIN_COMBAT_SUB_MOGUI_TIME       + 1 },
			--]]
				{ name="盘山道通关",	 bCheck = true,  nType = RANGE_MAIN_COMBAT, nSubType = RANGE_MAIN_COMBAT_SUB_PANSHAN_NUM      + 1 },
				{ name="龙蛋谷通关",     bCheck = true,  nType = RANGE_MAIN_COMBAT, nSubType = RANGE_MAIN_COMBAT_SUB_DRAGON_NUM       + 1 },
				--{ name="太阳神殿通关",   bCheck = true,  nType = RANGE_MAIN_COMBAT, nSubType = RANGE_MAIN_COMBAT_SUB_MOGUI_NUM        + 1 },
				{ name="地心英雄",	 bCheck = true,  nType = RANGE_MAIN_COMBAT, nSubType = RANGE_MAIN_COMBAT_SUB_BCT              + 1 },

				{ name="战场",		 bCheck = false, nType = RANGE_MAIN_BATTLE, nSubType = 0                                          },
				{ name="狂沙超神",	 bCheck = true,  nType = RANGE_MAIN_BATTLE, nSubType = RANGE_MAIN_BATTLE_SUB_SURVIVE_GOD      + 1 },
				{ name="狂沙大魔王",	 bCheck = true,  nType = RANGE_MAIN_BATTLE, nSubType = RANGE_MAIN_BATTLE_SUB_SURVIVE_KILLER   + 1 },
				{ name="狂沙不死神",	 bCheck = true,  nType = RANGE_MAIN_BATTLE, nSubType = RANGE_MAIN_BATTLE_SUB_SURVIVE_NODEAD   + 1 },
				{ name="狂沙优胜",	 bCheck = true,  nType = RANGE_MAIN_BATTLE, nSubType = RANGE_MAIN_BATTLE_SUB_KUANGSHA_WIN_NUM   + 1 },
				{ name="绿洲风暴超神",	 bCheck = true,  nType = RANGE_MAIN_BATTLE, nSubType = RANGE_MAIN_BATTLE_SUB_LZSURVIVE_GOD    + 1 },
				{ name="绿洲风暴大魔王", bCheck = true,  nType = RANGE_MAIN_BATTLE, nSubType = RANGE_MAIN_BATTLE_SUB_LZSURVIVE_KILLER + 1 },
				{ name="绿洲风暴不死神", bCheck = true,  nType = RANGE_MAIN_BATTLE, nSubType = RANGE_MAIN_BATTLE_SUB_LZSURVIVE_NODEAD + 1 },
				{ name="绿洲终结者",	 bCheck = true,  nType = RANGE_MAIN_BATTLE, nSubType = RANGE_MAIN_BATTLE_SUB_LZSURVIVE_END    + 1 },	
				{ name="绿洲风暴优胜",	 bCheck = true,  nType = RANGE_MAIN_BATTLE, nSubType = RANGE_MAIN_BATTLE_SUB_LZWIN_NUM    + 1 },
				
				{ name="功勋",	bCheck = false, nType = RANGE_MAIN_SPARTA, nSubType = 0 },
				{ name="末世荣耀殿堂",	bCheck = true,	nType = RANGE_MAIN_SPARTA, nSubType = RANGE_MAIN_SPARTA_SUB_FIGHT	+ 1 },
			};

local t_FontColor = {
			{ r = 255, g = 254 , b = 189 },
			{ r = 255, g = 132 , b = 153 },
			{ r = 255, g = 150 , b = 25  },
			{ r = 255, g = 0   , b = 0   },	}


local t_TypeName = { 
			[RANGE_MAIN_SINGLE] = { 
						[0]                                = "个人",
						[RANGE_MAIN_SINGLE_SUB_LEVEL  + 1] = "等级",
						[RANGE_MAIN_SINGLE_SUB_MONEY  + 1] = "财富",
						[RANGE_MAIN_SINGLE_SUB_PK     + 1] = "杀气",
						[RANGE_MAIN_SINGLE_SUB_CLANCT + 1] = "公会贡献",
						[RANGE_MAIN_SINGLE_SUB_GENIUS + 1] = "天赋值",
						[RANGE_MAIN_SINGLE_SUB_CHARM  + 1] = "魅力",
						[RANGE_MAIN_SINGLE_SUB_CHARM_WEEK	+ 1] = "周魅力",},

			[RANGE_MAIN_CAREER] = {
						[0]                                 = "职业",
						[RANGE_MAIN_CAREER_SUB_WARRIOR + 1] = "战士",
						[RANGE_MAIN_CAREER_SUB_MAGICOR + 1] = "法师",
						[RANGE_MAIN_CAREER_SUB_GUNER   + 1] = "枪手",
						[RANGE_MAIN_CAREER_SUB_CHURCH  + 1] = "祭司",	},
			[RANGE_MAIN_ATTR] = {
						[0] = "属性",
						[RANGE_MAIN_ATTR_SUB_HP 	+ 1] = "生命值",
						[RANGE_MAIN_ATTR_SUB_MP 	+ 1] = "魔法值",
						[RANGE_MAIN_ATTR_SUB_WULI_ATK 	+ 1] = "物理攻击",
						[RANGE_MAIN_ATTR_SUB_MOFA_ATK 	+ 1] = "魔法攻击",
						[RANGE_MAIN_ATTR_SUB_WULI_DEF 	+ 1] = "物理防御",
						[RANGE_MAIN_ATTR_SUB_MOFA_DEF 	+ 1] = "魔法防御",
						[RANGE_MAIN_ATTR_SUB_BAOJI_LVL 	+ 1] = "暴击等级",
						[RANGE_MAIN_ATTR_SUB_BAOJI_HURT 	+ 1] = "暴击伤害",
						[RANGE_MAIN_ATTR_SUB_DIKANG_LVL 	+ 1] = "抵抗等级",
						[RANGE_MAIN_ATTR_SUB_POFANG_LVL 	+ 1] = "破防等级",	},
			[RANGE_MAIN_CLAN]   = {
						[0]                               = "公会",
						[RANGE_MAIN_CLAN_SUB_LEVEL   + 1] = "等级",
						[RANGE_MAIN_CLAN_SUB_DEVELOP + 1] = "繁荣度",
						[RANGE_MAIN_CLAN_SUB_MEM     + 1] = "人数",
						[RANGE_MAIN_CLAN_SUB_COMBAT  + 1] = "公会战",	},

			[RANGE_MAIN_PET]    = {
						[0] = "宠物",
						[RANGE_MAIN_PET_SUB_FIGHT + 1] = "战斗力",	},

			[RANGE_MAIN_COMBAT] = {
						[0] = "副本",
						--[[
						[RANGE_MAIN_COMBAT_SUB_PANSHAN_TIME + 1] = "神速盘山道",
						[RANGE_MAIN_COMBAT_SUB_DRAGON_TIME  + 1] = "神速龙蛋谷",
						[RANGE_MAIN_COMBAT_SUB_MOGUI_TIME   + 1] = "神速太阳神殿",
						--]]
						[RANGE_MAIN_COMBAT_SUB_PANSHAN_NUM  + 1] = "盘山道通关",
						[RANGE_MAIN_COMBAT_SUB_DRAGON_NUM   + 1] = "龙蛋谷通关",
						--[RANGE_MAIN_COMBAT_SUB_MOGUI_NUM    + 1] = "太阳神殿通关",
						[RANGE_MAIN_COMBAT_SUB_BCT          + 1] = "地心英雄",	},
					
			[RANGE_MAIN_BATTLE] = {
						[0] = "战场",
						[RANGE_MAIN_BATTLE_SUB_SURVIVE_GOD      + 1] = "狂沙超神",
						[RANGE_MAIN_BATTLE_SUB_SURVIVE_KILLER   + 1] = "狂沙大魔王",
						[RANGE_MAIN_BATTLE_SUB_SURVIVE_NODEAD   + 1] = "狂沙不死神",
						[RANGE_MAIN_BATTLE_SUB_LZSURVIVE_GOD    + 1] = "绿洲风暴超神",
						[RANGE_MAIN_BATTLE_SUB_LZSURVIVE_KILLER + 1] = "绿洲风暴大魔王",
						[RANGE_MAIN_BATTLE_SUB_LZSURVIVE_NODEAD + 1] = "绿洲风暴不死神",
						[RANGE_MAIN_BATTLE_SUB_LZSURVIVE_END    + 1] = "绿洲风暴终结者",
						[RANGE_MAIN_BATTLE_SUB_KUANGSHA_WIN_NUM    + 1] = "狂沙优胜",
						[RANGE_MAIN_BATTLE_SUB_LZWIN_NUM    + 1] = "绿洲风暴优胜",
							},
			[RANGE_MAIN_SPARTA] = {
						[0] = "功勋",
						[RANGE_MAIN_SPARTA_SUB_FIGHT 	+ 1] = "末世荣耀殿堂",				
							},
			};

local t_HeadTitle = {
			[RANGE_MAIN_SINGLE] = {
						[RANGE_MAIN_SINGLE_SUB_LEVEL +1]   = {
											{ text = "排名", width = 46 , xoffset = 2   },
											{ text = "姓名", width = 115, xoffset = 48  },
											{ text = "等级", width = 71 , xoffset = 163 },
											{ text = "职业", width = 60 , xoffset = 234 },
											{ text = "公会", width = 116, xoffset = 294 },
											{ text = "升降", width = 51 , xoffset = 410 },
											{text="",width=0,xoffset=0},
											{text="",width=0,xoffset=0},	},

						[RANGE_MAIN_SINGLE_SUB_MONEY + 1]  = {
											{ text = "排名", width = 46 , xoffset = 2   },
											{ text = "姓名", width = 115, xoffset = 48  },
											{ text = "银币", width = 71 , xoffset = 163 },
											{ text = "职业", width = 60 , xoffset = 234 },
											{ text = "公会", width = 116, xoffset = 294 },
											{ text = "升降", width = 51 , xoffset = 410 },
											{text="",width=0,xoffset=0},
											{text="",width=0,xoffset=0},	},

						[RANGE_MAIN_SINGLE_SUB_PK + 1]	   = {
											{ text = "排名", width = 46 , xoffset = 2   },
											{ text = "姓名", width = 115, xoffset = 48  },
											{ text = "杀气", width = 71 , xoffset = 163 },
											{ text = "职业", width = 60 , xoffset = 234 },
											{ text = "公会", width = 116, xoffset = 294 },
											{ text = "升降", width = 51 , xoffset = 410 },
											{text="",width=0,xoffset=0},
											{text="",width=0,xoffset=0},	},

						[RANGE_MAIN_SINGLE_SUB_CLANCT + 1] = {
											{ text = "排名"    , width = 46 , xoffset=2   },
											{ text = "姓名"    , width = 115, xoffset=48  },
											{ text = "公会贡献", width = 71 , xoffset=163 },
											{ text = "职业"    , width = 60 , xoffset=234 },
											{ text = "公会"    , width = 116, xoffset=294 },
											{ text = "升降"    , width = 51 , xoffset=410 },
											{text="",width=0,xoffset=0},
											{text="",width=0,xoffset=0},	},
						[RANGE_MAIN_SINGLE_SUB_GENIUS + 1] = {
											{ text = "排名"    , width = 46 , xoffset=2   },
											{ text = "姓名"    , width = 115, xoffset=48  },
											{ text = "天赋值", width = 71 , xoffset=163 },
											{ text = "职业"    , width = 60 , xoffset=234 },
											{ text = "公会"    , width = 116, xoffset=294 },
											{ text = "升降"    , width = 51 , xoffset=410 },
											{text="",width=0,xoffset=0},
											{text="",width=0,xoffset=0},	},
						[RANGE_MAIN_SINGLE_SUB_CHARM + 1] = {
											{ text = "排名"    , width = 46 , xoffset=2   },
											{ text = "姓名"    , width = 115, xoffset=48  },
											{ text = "魅力"	   , width = 71 , xoffset=163 },
											{ text = "职业"    , width = 60 , xoffset=234 },
											{ text = "公会"    , width = 116, xoffset=294 },
											{ text = "升降"    , width = 51 , xoffset=410 },
											{text="",width=0,xoffset=0},
											{text="",width=0,xoffset=0},	},
						[RANGE_MAIN_SINGLE_SUB_CHARM_WEEK + 1] = {
											{ text = "排名"    , width = 46 , xoffset=2   },
											{ text = "姓名"    , width = 115, xoffset=48  },
											{ text = "周魅力"	   , width = 71 , xoffset=163 },
											{ text = "职业"    , width = 60 , xoffset=234 },
											{ text = "公会"    , width = 116, xoffset=294 },
											{ text = "升降"    , width = 51 , xoffset=410 },
											{text="",width=0,xoffset=0},
											{text="",width=0,xoffset=0},	}, },

			[RANGE_MAIN_CAREER] = {
						[RANGE_MAIN_CAREER_SUB_WARRIOR + 1] = {
																{text="排名",width=56,xoffset=2},
																{text="姓名",width=115,xoffset=58},
																{text="等级",width=56,xoffset=173},
																{text="职业",width=60,xoffset=229},
																{text="公会",width=116,xoffset=289},
																{text="升降",width=56,xoffset=405},
																{text="",width=0,xoffset=0},
																{text="",width=0,xoffset=0},
																},
							[RANGE_MAIN_CAREER_SUB_MAGICOR + 1] = {
																{text="排名",width=56,xoffset=2},
																{text="姓名",width=115,xoffset=58},
																{text="等级",width=56,xoffset=173},
																{text="职业",width=60,xoffset=229},
																{text="公会",width=116,xoffset=289},
																{text="升降",width=56,xoffset=405},
																{text="",width=0,xoffset=0},
																{text="",width=0,xoffset=0},
																},
							[RANGE_MAIN_CAREER_SUB_GUNER + 1] = {
																{text="排名",width=56,xoffset=2},
																{text="姓名",width=115,xoffset=58},
																{text="等级",width=56,xoffset=173},
																{text="职业",width=60,xoffset=229},
																{text="公会",width=116,xoffset=289},
																{text="升降",width=56,xoffset=405},
																{text="",width=0,xoffset=0},
																{text="",width=0,xoffset=0},
																},
							[RANGE_MAIN_CAREER_SUB_CHURCH + 1] = {
																{text="排名",width=56,xoffset=2},
																{text="姓名",width=115,xoffset=58},
																{text="等级",width=56,xoffset=173},
																{text="职业",width=60,xoffset=229},
																{text="公会",width=116,xoffset=289},
																{text="升降",width=56,xoffset=405},
																{text="",width=0,xoffset=0},
																{text="",width=0,xoffset=0},
																},
								},
			[RANGE_MAIN_ATTR] =	{
							[RANGE_MAIN_ATTR_SUB_HP + 1] = {
																{text="排名",width=56,xoffset=2},
																{text="姓名",width=115,xoffset=58},
																{text="生命值",width=56,xoffset=173},
																{text="职业",width=60,xoffset=229},
																{text="公会",width=116,xoffset=289},
																{text="升降",width=56,xoffset=405},
																{text="",width=0,xoffset=0},
																{text="",width=0,xoffset=0},
																},
							[RANGE_MAIN_ATTR_SUB_MP + 1] = {
																{text="排名",width=56,xoffset=2},
																{text="姓名",width=115,xoffset=58},
																{text="魔法值",width=56,xoffset=173},
																{text="职业",width=60,xoffset=229},
																{text="公会",width=116,xoffset=289},
																{text="升降",width=56,xoffset=405},
																{text="",width=0,xoffset=0},
																{text="",width=0,xoffset=0},
																},
							[RANGE_MAIN_ATTR_SUB_WULI_ATK + 1] = {
																{text="排名",width=56,xoffset=2},
																{text="姓名",width=115,xoffset=58},
																{text="物理攻击",width=56,xoffset=173},
																{text="职业",width=60,xoffset=229},
																{text="公会",width=116,xoffset=289},
																{text="升降",width=56,xoffset=405},
																{text="",width=0,xoffset=0},
																{text="",width=0,xoffset=0},
																},
							[RANGE_MAIN_ATTR_SUB_MOFA_ATK + 1] = {
																{text="排名",width=56,xoffset=2},
																{text="姓名",width=115,xoffset=58},
																{text="魔法攻击",width=56,xoffset=173},
																{text="职业",width=60,xoffset=229},
																{text="公会",width=116,xoffset=289},
																{text="升降",width=56,xoffset=405},
																{text="",width=0,xoffset=0},
																{text="",width=0,xoffset=0},
																},
							[RANGE_MAIN_ATTR_SUB_WULI_DEF + 1] = {
																{text="排名",width=56,xoffset=2},
																{text="姓名",width=115,xoffset=58},
																{text="物理防御",width=56,xoffset=173},
																{text="职业",width=60,xoffset=229},
																{text="公会",width=116,xoffset=289},
																{text="升降",width=56,xoffset=405},
																{text="",width=0,xoffset=0},
																{text="",width=0,xoffset=0},
																},
							[RANGE_MAIN_ATTR_SUB_MOFA_DEF + 1] = {
																{text="排名",width=56,xoffset=2},
																{text="姓名",width=115,xoffset=58},
																{text="魔法防御",width=56,xoffset=173},
																{text="职业",width=60,xoffset=229},
																{text="公会",width=116,xoffset=289},
																{text="升降",width=56,xoffset=405},
																{text="",width=0,xoffset=0},
																{text="",width=0,xoffset=0},
																},
							[RANGE_MAIN_ATTR_SUB_BAOJI_LVL + 1] = {
																{text="排名",width=56,xoffset=2},
																{text="姓名",width=115,xoffset=58},
																{text="暴击等级",width=56,xoffset=173},
																{text="职业",width=60,xoffset=229},
																{text="公会",width=116,xoffset=289},
																{text="升降",width=56,xoffset=405},
																{text="",width=0,xoffset=0},
																{text="",width=0,xoffset=0},
																},
							[RANGE_MAIN_ATTR_SUB_BAOJI_HURT + 1] = {
																{text="排名",width=56,xoffset=2},
																{text="姓名",width=115,xoffset=58},
																{text="暴击伤害",width=56,xoffset=173},
																{text="职业",width=60,xoffset=229},
																{text="公会",width=116,xoffset=289},
																{text="升降",width=56,xoffset=405},
																{text="",width=0,xoffset=0},
																{text="",width=0,xoffset=0},
																},
							[RANGE_MAIN_ATTR_SUB_DIKANG_LVL + 1] = {
																{text="排名",width=56,xoffset=2},
																{text="姓名",width=115,xoffset=58},
																{text="抵抗等级",width=56,xoffset=173},
																{text="职业",width=60,xoffset=229},
																{text="公会",width=116,xoffset=289},
																{text="升降",width=56,xoffset=405},
																{text="",width=0,xoffset=0},
																{text="",width=0,xoffset=0},
																},
							[RANGE_MAIN_ATTR_SUB_POFANG_LVL + 1] = {
																{text="排名",width=56,xoffset=2},
																{text="姓名",width=115,xoffset=58},
																{text="破防等级",width=56,xoffset=173},
																{text="职业",width=60,xoffset=229},
																{text="公会",width=116,xoffset=289},
																{text="升降",width=56,xoffset=405},
																{text="",width=0,xoffset=0},
																{text="",width=0,xoffset=0},
																},
								},
					[RANGE_MAIN_CLAN] =	  {
											[RANGE_MAIN_CLAN_SUB_LEVEL + 1] = {                                     
																				{text="排名",width=56,xoffset=2},
																				{text="公会名",width=115,xoffset=58},
																				{text="等级",width=86,xoffset=173},
																				{text="",width=0,xoffset=0},
																				{text="公会会长",width=146,xoffset=259},
																				{text="升降",width=56,xoffset=405},
																				{text="",width=0,xoffset=0},
																				{text="",width=0,xoffset=0},
																				},

											[RANGE_MAIN_CLAN_SUB_DEVELOP + 1] = {
																				{text="排名",width=56,xoffset=2},
																				{text="公会名",width=115,xoffset=58},
																				{text="繁荣度",width=86,xoffset=173},
																				{text="",width=0,xoffset=0},
																				{text="公会会长",width=146,xoffset=259},
																				{text="升降",width=56,xoffset=405},
																				{text="",width=0,xoffset=0},
																				{text="",width=0,xoffset=0},
																				},
											[RANGE_MAIN_CLAN_SUB_MEM + 1] =  {
																				{text="排名",width=56,xoffset=2},
																				{text="公会名",width=115,xoffset=58},
																				{text="公会人数",width=86,xoffset=173},
																				{text="",width=0,xoffset=0},
																				{text="公会会长",width=146,xoffset=259},
																				{text="升降",width=56,xoffset=405},
																				{text="",width=0,xoffset=0},
																				{text="",width=0,xoffset=0},
																				},
											[RANGE_MAIN_CLAN_SUB_COMBAT + 1] = {
																				{text="排名",width=56,xoffset=2},
																				{text="公会名",width=115,xoffset=58},
																				{text="统治时间",width=86,xoffset=173},
																				{text="",width=0,xoffset=0},
																				{text="公会会长",width=146,xoffset=259},
																				{text="升降",width=56,xoffset=405},
																				{text="",width=0,xoffset=0},
																				{text="",width=0,xoffset=0},
																				},
											},
					[RANGE_MAIN_PET] =	  {
											[RANGE_MAIN_PET_SUB_FIGHT + 1] = {
																				{text="排名",width=56,xoffset=2},
																				{text="名字",width=115,xoffset=58},
																				{text="主人名字",width=96,xoffset=173},
																				{text="等级",width=60,xoffset=269},
																				{text="战斗力",width=76,xoffset=329},
																				{text="升降",width=56,xoffset=405},
																				{text="",width=0,xoffset=0},
																				{text="",width=0,xoffset=0},
																				},
											},
					[RANGE_MAIN_COMBAT] = {
											--[[
											[RANGE_MAIN_COMBAT_SUB_PANSHAN_TIME + 1] = {
																				{text="排名",width=56,xoffset=2},
																				{text="姓名",width=115,xoffset=58},
																				{text="用时",width=86,xoffset=173},
																				{text="副本名",width=146,xoffset=259},
																				{text="",width=0,xoffset=0},
																				{text="升降",width=56,xoffset=405},
																				},
											[RANGE_MAIN_COMBAT_SUB_DRAGON_TIME + 1] = {
																				{text="排名",width=56,xoffset=2},
																				{text="姓名",width=115,xoffset=58},
																				{text="用时",width=86,xoffset=173},
																				{text="副本名",width=146,xoffset=259},
																				{text="",width=0,xoffset=0},
																				{text="升降",width=56,xoffset=405},
																				},
											[RANGE_MAIN_COMBAT_SUB_MOGUI_TIME + 1] = {
																				{text="排名",width=56,xoffset=2},
																				{text="姓名",width=115,xoffset=58},
																				{text="用时",width=86,xoffset=173},
																				{text="副本名",width=146,xoffset=259},
																				{text="",width=0,xoffset=0},
																				{text="升降",width=56,xoffset=405},
																				},
											--]]
											[RANGE_MAIN_COMBAT_SUB_PANSHAN_NUM + 1] = {
																				{text="排名",width=56,xoffset=2},
																				{text="姓名",width=115,xoffset=58},
																				{text="通关次数",width=86,xoffset=173},
																				{text="副本名",width=146,xoffset=259},
																				{text="",width=0,xoffset=0},
																				{text="升降",width=56,xoffset=405},
																				{text="",width=0,xoffset=0},
																				{text="",width=0,xoffset=0},
																				},
											[RANGE_MAIN_COMBAT_SUB_DRAGON_NUM + 1] = {
																				{text="排名",width=56,xoffset=2},
																				{text="姓名",width=115,xoffset=58},
																				{text="通关次数",width=86,xoffset=173},
																				{text="副本名",width=146,xoffset=259},
																				{text="",width=0,xoffset=0},
																				{text="升降",width=56,xoffset=405},
																				{text="",width=0,xoffset=0},
																				{text="",width=0,xoffset=0},
																				},
																				--[[
											[RANGE_MAIN_COMBAT_SUB_MOGUI_NUM + 1] = {
																				{text="排名",width=56,xoffset=2},
																				{text="姓名",width=115,xoffset=58},
																				{text="通关次数",width=86,xoffset=173},
																				{text="副本名",width=146,xoffset=259},
																				{text="",width=0,xoffset=0},
																				{text="升降",width=56,xoffset=405},
																				{text="",width=0,xoffset=0},
																				{text="",width=0,xoffset=0},
																				},
																				--]]
											[RANGE_MAIN_COMBAT_SUB_BCT + 1] = {
																				{text="排名",width=56,xoffset=2},
																				{text="姓名",width=85,xoffset=58},
																				{text="通关层数",width=90,xoffset=143},
																				{text="职业",width=90,xoffset=233},
																				{text="公会",width=82,xoffset=323},
																				{text="升降",width=56,xoffset=405},
																				{text="",width=0,xoffset=0},
																				{text="",width=0,xoffset=0},
																				},
											},
						[RANGE_MAIN_BATTLE] = {

							[RANGE_MAIN_BATTLE_SUB_SURVIVE_GOD + 1] = {
													{text="排名",width=56,xoffset=2},
													{text="姓名",width=115,xoffset=58},
													{text="超神次数",width=86,xoffset=173},
													{text="",width=0,xoffset=0},
													{text="战场名",width=146,xoffset=259},
													{text="升降",width=56,xoffset=405},
													{text="",width=0,xoffset=0},
													{text="",width=0,xoffset=0},
												},

							[RANGE_MAIN_BATTLE_SUB_SURVIVE_KILLER + 1] = {
													{text="排名",width=56,xoffset=2},
													{text="姓名",width=115,xoffset=58},
													{text="累计击杀数",width=86,xoffset=173},
													{text="",width=0,xoffset=0},
													{text="战场名",width=146,xoffset=259},
													{text="升降",width=56,xoffset=405},
													{text="",width=0,xoffset=0},
													{text="",width=0,xoffset=0},
												},

							[RANGE_MAIN_BATTLE_SUB_SURVIVE_NODEAD + 1] = {
													{text="排名",width=56,xoffset=2},
													{text="姓名",width=115,xoffset=58},
													{text="最大连杀数",width=86,xoffset=173},
													{text="",width=0,xoffset=0},
													{text="战场名",width=146,xoffset=259},
													{text="升降",width=56,xoffset=405},
													{text="",width=0,xoffset=0},
													{text="",width=0,xoffset=0},
												},

							[RANGE_MAIN_BATTLE_SUB_LZSURVIVE_GOD + 1] = {
													{text="排名",width=56,xoffset=2},
													{text="姓名",width=115,xoffset=58},
													{text="超神次数",width=86,xoffset=173},
													{text="",width=0,xoffset=0},
													{text="战场名",width=146,xoffset=259},
													{text="升降",width=56,xoffset=405},
													{text="",width=0,xoffset=0},
													{text="",width=0,xoffset=0},
												},

							[RANGE_MAIN_BATTLE_SUB_LZSURVIVE_KILLER + 1] = {
													{text="排名",width=56,xoffset=2},
													{text="姓名",width=115,xoffset=58},
													{text="累计击杀数",width=86,xoffset=173},
													{text="",width=0,xoffset=0},
													{text="战场名",width=146,xoffset=259},
													{text="升降",width=56,xoffset=405},
													{text="",width=0,xoffset=0},
													{text="",width=0,xoffset=0},
												},

							[RANGE_MAIN_BATTLE_SUB_LZSURVIVE_NODEAD + 1] = {
													{text="排名",width=56,xoffset=2},
													{text="姓名",width=115,xoffset=58},
													{text="最大连杀数",width=86,xoffset=173},
													{text="",width=0,xoffset=0},
													{text="战场名",width=146,xoffset=259},
													{text="升降",width=56,xoffset=405},
													{text="",width=0,xoffset=0},
													{text="",width=0,xoffset=0},
												},

							[RANGE_MAIN_BATTLE_SUB_LZSURVIVE_END + 1] = {
													{text="排名",width=56,xoffset=2},
													{text="姓名",width=115,xoffset=58},
													{text="击杀首领数",width=86,xoffset=173},
													{text="",width=0,xoffset=0},
													{text="战场名",width=146,xoffset=259},
													{text="升降",width=56,xoffset=405},
													{text="",width=0,xoffset=0},
													{text="",width=0,xoffset=0},
												},
							[RANGE_MAIN_BATTLE_SUB_KUANGSHA_WIN_NUM + 1] = {
													{text="排名",width=56,xoffset=2},
													{text="姓名",width=115,xoffset=58},
													{text="胜利次数",width=86,xoffset=173},
													{text="",width=0,xoffset=0},
													{text="战场名",width=146,xoffset=259},
													{text="升降",width=56,xoffset=405},
													{text="",width=0,xoffset=0},
													{text="",width=0,xoffset=0},
												},
							[RANGE_MAIN_BATTLE_SUB_LZWIN_NUM + 1] = {
													{text="排名",width=56,xoffset=2},
													{text="姓名",width=115,xoffset=58},
													{text="胜利次数",width=86,xoffset=173},
													{text="",width=0,xoffset=0},
													{text="战场名",width=146,xoffset=259},
													{text="升降",width=56,xoffset=405},
													{text="",width=0,xoffset=0},
													{text="",width=0,xoffset=0},
												},
						},
						[RANGE_MAIN_SPARTA] = {

							[RANGE_MAIN_SPARTA_SUB_FIGHT + 1] = {
													{text="排名",width=56,xoffset=2},
													{text="姓名",width=115,xoffset=58},
													{text="末世功勋",width=60,xoffset=173},
													{text="等级",width=44,xoffset=233},
													{text="职业",width=60,xoffset=277},
													{text="胜率",width=44,xoffset=337},
													{text="逃跑",width=56,xoffset=381},
													{text="升降",width=30,xoffset=437},
												},
						},
					};


local nCurSelType = RANGE_MAIN_SINGLE;	--当前选中的主类别
local nCurSelSubType = RANGE_MAIN_SINGLE_SUB_LEVEL + 1;	--当前选中的子类别

local nCurPage = 1;	--当前页码
local nMaxPage = 1;	--总页数

local nSelIndex = 0;	--当前选择的项

local bShowSearch = false;

local CurTipsInfo;

local nTopLevel = 0;	--当前个人排行榜最高等级

nLastAcceptTime = 0;

function SortLineFrame_OnLoad()
	this:RegisterEvent("RANK_TIP_SHOW");
	this:RegisterEvent("RANK_NEW_PAGE_DATA");
	this:RegisterEvent("RANK_SEARCH_NO_DATA");
	this:RegisterEvent("MY_RANK_INFO_CHANGE");
	this:RegisterEvent("GE_FRIEND_GETLIST");
	this:RegisterEvent("GE_ENTER_MAP");
	this:RegisterEvent("GE_ENTER_PLAYERLOGIN");
	CurTipsInfo = RankMgr:getMyBestRankTip();	
	for i=1,MAX_SHOW_COUNT do
		local btn = getglobal( "SortInfo"..i );
		if i%2 == 0 then
			btn:SetNormalTextureTemplate( "WhiteCategoryBkg" );
			btn:SetDisableTextureTemplate( "WhiteCategoryBkg" );
		else
			btn:SetNormalTextureTemplate( "BlackCategoryBkg" );
			btn:SetDisableTextureTemplate( "BlackCategoryBkg" );
		end
	end
end

function SortLineFrame_OnEvent()
	if arg1 == "RANK_TIP_SHOW" then
		nLastAcceptTime = os.clock();
		local tips = RankMgr:getMyRankTip();
		if SortInfoMsgFrame:IsShown() then
			SortInfoMsgFrame:Hide();
		end

		--说明没有变化
		if tips.CurrRange == tips.LastRange then
			return;
		end

		if (CurTipsInfo.MainType == -1 and CurTipsInfo.SubType == -1) or CurTipsInfo.CurrRange > tips.CurrRange then
			CurTipsInfo.MainType = tips.MainType;
			CurTipsInfo.SubType = tips.SubType;
			CurTipsInfo.CurrRange = tips.CurrRange;
			CurTipsInfo.LastRange = tips.LastRange;
		end
	elseif arg1 == "RANK_NEW_PAGE_DATA" then
		if nTopLevel == 0 then
			local pageData = RankMgr:getPageData( 1,0,0,false );
			nTopLevel = pageData.Entrys[0].RangeData.Level;
		end
		RefreshPageData(false);	
	elseif arg1 == "RANK_SEARCH_NO_DATA" then
		if nCurSelType == RANGE_MAIN_SINGLE or nCurSelType == RANGE_MAIN_CAREER or nCurSelType == RANGE_MAIN_COMBAT or nCurSelType == RANGE_MAIN_BATTLE then
			ShowMidTips("没有搜索到此名字的玩家");
		elseif nCurSelType == RANGE_MAIN_PET then
			ShowMidTips("没有搜索到此名字的宠物");
		elseif nCurSelType == RANGE_MAIN_CLAN then
			ShowMidTips("没有搜索到此名字的公会");
		end
	elseif arg1 == "MY_RANK_INFO_CHANGE" then
		RefreshPageData(false);
	elseif arg1 == "GE_ENTER_MAP" then
		if SortLineFrame:IsShown() then
			SortLineFrame:Hide();
		end
	elseif arg1 == "GE_ENTER_PLAYERLOGIN" then
		nTopLevel = 0;
		local pageData = RankMgr:getPageData( 1,0,0,true );--获取排行榜第一页信息
	end
end

function SortLineFrame_OnHide()
	bShowSearch = false;
	SortLineFrameInputBtnSearchText:SetText("");
end

function ShowRankTips()
	if CurTipsInfo == nil then
		return
	end
	if CurTipsInfo.MainType == -1 or CurTipsInfo.SubType == -1 then return end
	local szTypeName = t_TypeName[CurTipsInfo.MainType][0] .. t_TypeName[CurTipsInfo.MainType][CurTipsInfo.SubType + 1] .. "排行榜";
	local szText = "你";
	if CurTipsInfo.MainType == RANGE_MAIN_CLAN then
		local ClanViewer	= GameClanManager:getClanViewer();
		local ClanInfo		= ClanViewer.m_clanInfo;
		szText = "你的公会"..ClanInfo.m_szClanName;
	end
	if CurTipsInfo.CurrRange == -1 and CurTipsInfo.LastRange ~= -1 then
		szText = szText.. "在"..szTypeName.."中落榜了";
	elseif CurTipsInfo.CurrRange ~= -1 and CurTipsInfo.LastRange == -1 then
		szText = szText .."登上了"..szTypeName;
	else
		local nPos = math.abs( CurTipsInfo.CurrRange - CurTipsInfo.LastRange );
		if CurTipsInfo.CurrRange < CurTipsInfo.LastRange then
			szText = szText .. "在"..szTypeName.."中排名上升了".. nPos .."位";
		else
			return;
		end		
	end
	szText = szText .."#r#L点击查看详情#n";
	SortInfoMsgFrameTipRich:SetText(szText,255,255,255);
	--在转职的时候，现在显示排行榜的tips
	if not ChangeCareerFrame:IsShown() then
		SortInfoMsgFrame:Show();
	end
end

function RefreshPageData( bRequestServer )
	if not SortLineFrame:IsShown() or nCurSelSubType == 0 then
		return;
	end
	for i = 1,MAX_SHOW_COUNT do
		local btn = getglobal("SortInfo"..i);
		btn:DisChecked();
		btn:Hide();
	end
	nSelIndex = 0;

	showActorOperateButton( false );

	SetSortIndexRightList();
	local pageData = nil;
	if bShowSearch == false then
		pageData = RankMgr:getPageData( nCurPage,nCurSelType,nCurSelSubType - 1,bRequestServer );
	else
		local szSearchName = SortLineFrameInputBtnSearchText:GetText();
		pageData = RankMgr:getSearchData( szSearchName,nCurPage,nCurSelType,nCurSelSubType - 1,bRequestServer );
	end
	if pageData == nil then
		return;
	end
	-- 不是当前选中的类别
	if pageData.MainType ~= nCurSelType and pageData.SubType ~= nCurSelSubType -1 then
		return
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	nMaxPage = math.floor( (pageData.TotalNum - 1)/MAX_SHOW_COUNT ) + 1;
	if nMaxPage <= 0 then
		nMaxPage = 1;
	end
	
	for i = 1,8 do
		local btnFont		= getglobal("SortLineFrame_ListBackBtnTitleBtn"..i.."Font");
		local btnTex		= getglobal("SortLineFrame_ListBackBtnTitleBtn"..i.."Tex");
		
		btnFont:SetSize( t_HeadTitle[nCurSelType][nCurSelSubType][i].width,20 );
		btnTex:SetSize( t_HeadTitle[nCurSelType][nCurSelSubType][i].width,24 );
		btnFont:SetText(t_HeadTitle[nCurSelType][nCurSelSubType][i].text);
		btnFont:SetPoint("TOPLEFT","SortLineFrame_ListBackBtn","TOPLEFT",t_HeadTitle[nCurSelType][nCurSelSubType][i].xoffset,7);
		btnTex:SetPoint("TOPLEFT","SortLineFrame_ListBackBtn","TOPLEFT",t_HeadTitle[nCurSelType][nCurSelSubType][i].xoffset,2)
		btnTex:Show();
		btnFont:Show();
		for j = 1,16 do
			if i == 1 then
				local rankFont		= getglobal("SortInfo"..j.."SortIndexFont");
				rankFont:SetSize(t_HeadTitle[nCurSelType][nCurSelSubType][i].width,20);
				rankFont:SetPoint("TOPLEFT","SortInfo"..j,"TOPLEFT",t_HeadTitle[nCurSelType][nCurSelSubType][i].xoffset+13,2);
			elseif i == 2 then
				local NameFont		= getglobal("SortInfo"..j.."NameFont");
				NameFont:SetSize(t_HeadTitle[nCurSelType][nCurSelSubType][i].width,20);
				NameFont:SetPoint("TOPLEFT","SortInfo"..j,"TOPLEFT",t_HeadTitle[nCurSelType][nCurSelSubType][i].xoffset+2,2);
			elseif i == 3 then
				local LvFont		= getglobal("SortInfo"..j.."LvFont");
				LvFont:SetSize(t_HeadTitle[nCurSelType][nCurSelSubType][i].width,20);
				if nCurSelType == RANGE_MAIN_SINGLE and nCurSelSubType == RANGE_MAIN_SINGLE_SUB_MONEY + 1 then
					LvFont:SetPoint("TOPLEFT","SortInfo"..j,"TOPLEFT",t_HeadTitle[nCurSelType][nCurSelSubType][i].xoffset,2);
				elseif nCurSelType == RANGE_MAIN_PET or nCurSelType == RANGE_MAIN_ATTR then
					LvFont:SetPoint("TOPLEFT","SortInfo"..j,"TOPLEFT",t_HeadTitle[nCurSelType][nCurSelSubType][i].xoffset,2);
				else
					LvFont:SetPoint("TOPLEFT","SortInfo"..j,"TOPLEFT",t_HeadTitle[nCurSelType][nCurSelSubType][i].xoffset+13,2);
				end
			elseif i == 4 then
				local CareerFont	= getglobal("SortInfo"..j.."CareerFont");
				CareerFont:SetSize(t_HeadTitle[nCurSelType][nCurSelSubType][i].width,20);
				if nCurSelType == RANGE_MAIN_CLAN then
					CareerFont:SetPoint("TOPLEFT","SortInfo"..j,"TOPLEFT",t_HeadTitle[nCurSelType][nCurSelSubType][i].xoffset+2,2);
				else
					CareerFont:SetPoint("TOPLEFT","SortInfo"..j,"TOPLEFT",t_HeadTitle[nCurSelType][nCurSelSubType][i].xoffset+13,2);
				end
			elseif i == 5 then
				local ClanNameFont	= getglobal("SortInfo"..j.."ClanNameFont");
				ClanNameFont:SetSize(t_HeadTitle[nCurSelType][nCurSelSubType][i].width,20);
				ClanNameFont:SetPoint("TOPLEFT","SortInfo"..j,"TOPLEFT",t_HeadTitle[nCurSelType][nCurSelSubType][i].xoffset+13,2);
			elseif i == 6 then
				local SortAreaFont	= getglobal("SortInfo"..j.."SortAreaFont");
				SortAreaFont:SetSize(t_HeadTitle[nCurSelType][nCurSelSubType][i].width,20);
				SortAreaFont:SetPoint("TOPLEFT","SortInfo"..j,"TOPLEFT",t_HeadTitle[nCurSelType][nCurSelSubType][i].xoffset+13,2);
			elseif i == 7 then
				local SortAreaFont	= getglobal("SortInfo"..j.."RunAwayFont");
				SortAreaFont:SetSize(t_HeadTitle[nCurSelType][nCurSelSubType][i].width,20);
				SortAreaFont:SetPoint("TOPLEFT","SortInfo"..j,"TOPLEFT",t_HeadTitle[nCurSelType][nCurSelSubType][i].xoffset,2);
			elseif i == 8 then
				local SortAreaFont	= getglobal("SortInfo"..j.."VRateFont");
				SortAreaFont:SetSize(t_HeadTitle[nCurSelType][nCurSelSubType][i].width,20);
				SortAreaFont:SetPoint("TOPLEFT","SortInfo"..j,"TOPLEFT",t_HeadTitle[nCurSelType][nCurSelSubType][i].xoffset,2);
			end	
		end
		if t_HeadTitle[nCurSelType][nCurSelSubType][i].text == "" then
			btnTex:Hide();
			btnFont:Hide();
		end
	end
	
	if nCurSelType == RANGE_MAIN_SINGLE or nCurSelType == RANGE_MAIN_CAREER then
		for i = 1,pageData.EntryNum do
			if i > MAX_SHOW_COUNT then
				return;
			end
			local btn		= getglobal("SortInfo"..i);
			local rankFont		= getglobal("SortInfo"..i.."SortIndexFont");
			local NameFont		= getglobal("SortInfo"..i.."NameFont");
			local LvFont		= getglobal("SortInfo"..i.."LvFont");
			local CareerFont	= getglobal("SortInfo"..i.."CareerFont");
			local ClanNameFont	= getglobal("SortInfo"..i.."ClanNameFont");
			local SortAreaFont	= getglobal("SortInfo"..i.."SortAreaFont");

			local nColorIndex = 1;		
			--根据和主角的好友关系 改变颜色
			local friendInfo = GameFriendManager:getFriendInfoByName( pageData.Entrys[i-1].RangeData.FirstName );
			if friendInfo ~= nil then
				if friendInfo.m_nRelationType == CLIENT_RELATION_TYPE_FRIEND then
					nColorIndex = 2;
				elseif friendInfo.m_nRelationType == CLIENT_RELATION_TYPE_SU_DI then
					nColorIndex = 4;
				elseif friendInfo.m_nRelationType == CLIENT_RELATION_TYPE_XIONG_SHOU then
					nColorIndex = 4;
				end
			end
			--主角颜色
			if pageData.Entrys[i-1].RangeData.FirstName == mainplayer:getName() then
				nColorIndex = 3;
			end
			rankFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
			rankFont:SetText( pageData.Entrys[i-1].Range );
			NameFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
			NameFont:SetText( pageData.Entrys[i-1].RangeData.FirstName );
			LvFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
			if nCurSelType == RANGE_MAIN_CAREER then
				LvFont:SetText( pageData.Entrys[i-1].RangeData.Level )
			else		
				if nCurSelSubType == RANGE_MAIN_SINGLE_SUB_PK+1 then
					LvFont:SetText( math.floor(pageData.Entrys[i-1].RangeData.RangeVal/10) );
				else
					LvFont:SetText( pageData.Entrys[i-1].RangeData.RangeVal );
				end
			end
			CareerFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
			CareerFont:SetText( t_raceList[ pageData.Entrys[i-1].RangeData.Career ] );
			ClanNameFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
			ClanNameFont:SetText( pageData.Entrys[i-1].RangeData.SecondName );

			local changeSign = "";
			local nChange = 0;
			--说明这个人是刚上榜的
			if pageData.Entrys[i-1].RangeData.LastRange == -1 then
				nChange = pageData.Entrys[i-1].Range - pageData.TotalNum;
			else		
				nChange = pageData.Entrys[i-1].Range - pageData.Entrys[i-1].RangeData.LastRange;
			end
			if nChange < 0 then
				changeSign = "↑";
			elseif nChange > 0 then
				changeSign = "↓";
			end
			SortAreaFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
			SortAreaFont:SetText( changeSign..math.abs( nChange ) );
			if nChange == 0 then
				SortAreaFont:SetText("  -");
			end
			
			btn:SetClientUserData(0,pageData.Entrys[i-1].RangeData.GID.nLow);
			btn:SetClientUserData(1,pageData.Entrys[i-1].RangeData.GID.nHeight);
			btn:Show();
		end
	elseif nCurSelType == RANGE_MAIN_ATTR then
		for i = 1,pageData.EntryNum do
			if i > MAX_SHOW_COUNT then
				return;
			end
			local btn		= getglobal("SortInfo"..i);
			local rankFont		= getglobal("SortInfo"..i.."SortIndexFont");
			local NameFont		= getglobal("SortInfo"..i.."NameFont");
			local LvFont		= getglobal("SortInfo"..i.."LvFont");
			local CareerFont	= getglobal("SortInfo"..i.."CareerFont");
			local ClanNameFont	= getglobal("SortInfo"..i.."ClanNameFont");
			local SortAreaFont	= getglobal("SortInfo"..i.."SortAreaFont");

			local nColorIndex = 1;		
			--根据和主角的好友关系 改变颜色
			local friendInfo = GameFriendManager:getFriendInfoByName( pageData.Entrys[i-1].RangeData.FirstName );
			if friendInfo ~= nil then
				if friendInfo.m_nRelationType == CLIENT_RELATION_TYPE_FRIEND then
					nColorIndex = 2;
				elseif friendInfo.m_nRelationType == CLIENT_RELATION_TYPE_SU_DI then
					nColorIndex = 4;
				elseif friendInfo.m_nRelationType == CLIENT_RELATION_TYPE_XIONG_SHOU then
					nColorIndex = 4;
				end
			end
			--主角颜色
			if pageData.Entrys[i-1].RangeData.FirstName == mainplayer:getName() then
				nColorIndex = 3;
			end
			--排名
			rankFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
			rankFont:SetText( pageData.Entrys[i-1].Range );
			--姓名
			NameFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
			NameFont:SetText( pageData.Entrys[i-1].RangeData.FirstName );
			--对应属性
			LvFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
			--暴击伤害特殊处理下
			if nCurSelSubType == RANGE_MAIN_ATTR_SUB_BAOJI_HURT + 1 then
				local ADJUST_VALUE = 150;
				local fResult	= pageData.Entrys[i-1].RangeData.RangeVal;
				fResult			= fResult / 100;
	
				local interger, fraction = math.modf( fResult );
				interger		= interger + ADJUST_VALUE;
				LvFont:SetText( ( interger.."."..math.floor( fraction * 100 ) ).."%" );
			else
				LvFont:SetText( pageData.Entrys[i-1].RangeData.RangeVal );
			end
			--职业
			CareerFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
			CareerFont:SetText( t_raceList[ pageData.Entrys[i-1].RangeData.Career ] );
			--公会名字
			ClanNameFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
			ClanNameFont:SetText( pageData.Entrys[i-1].RangeData.SecondName );

			local changeSign = "";
			local nChange = 0;
			--说明这个人是刚上榜的
			if pageData.Entrys[i-1].RangeData.LastRange == -1 then
				nChange = pageData.Entrys[i-1].Range - pageData.TotalNum;
			else		
				nChange = pageData.Entrys[i-1].Range - pageData.Entrys[i-1].RangeData.LastRange;
			end
			if nChange < 0 then
				changeSign = "↑";
			elseif nChange > 0 then
				changeSign = "↓";
			end
			SortAreaFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
			SortAreaFont:SetText( changeSign..math.abs( nChange ) );
			if nChange == 0 then
				SortAreaFont:SetText("  -");
			end
			
			btn:SetClientUserData(0,pageData.Entrys[i-1].RangeData.GID.nLow);
			btn:SetClientUserData(1,pageData.Entrys[i-1].RangeData.GID.nHeight);
			btn:Show();
		end
	elseif nCurSelType == RANGE_MAIN_CLAN then
		--公会
		for i = 1,pageData.EntryNum do
			if i > MAX_SHOW_COUNT then
				return;
			end
			local btn		= getglobal("SortInfo"..i);
			local rankFont		= getglobal("SortInfo"..i.."SortIndexFont");
			local NameFont		= getglobal("SortInfo"..i.."NameFont");
			local LvFont		= getglobal("SortInfo"..i.."LvFont");
			local CareerFont	= getglobal("SortInfo"..i.."CareerFont");
			local ClanNameFont	= getglobal("SortInfo"..i.."ClanNameFont");
			local SortAreaFont	= getglobal("SortInfo"..i.."SortAreaFont");
			
			local nColorIndex = 1;
			--我的公会
			if GameClanManager:getClanViewer().m_clanInfo.m_szClanName == pageData.Entrys[i-1].RangeData.FirstName then
				nColorIndex = 3;
			end

			-- 排名
			rankFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
			rankFont:SetText( pageData.Entrys[i-1].Range );
			-- 公会名
			NameFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
			NameFont:SetText( pageData.Entrys[i-1].RangeData.FirstName );
			-- 统计数据种类
			LvFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
			if nCurSelSubType == RANGE_MAIN_CLAN_SUB_COMBAT + 1 then
				LvFont:SetText( pageData.Entrys[i-1].RangeData.RangeVal.."天" );
			else		
				LvFont:SetText( pageData.Entrys[i-1].RangeData.RangeVal );
			end
			-- 为空
			CareerFont:SetText("");
			-- 公会会长
			ClanNameFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
			ClanNameFont:SetText( pageData.Entrys[i-1].RangeData.SecondName );

			local changeSign = "";
			local nChange = 0;
			--说明这个人是刚上榜的
			if pageData.Entrys[i-1].RangeData.LastRange == -1 then
				nChange = pageData.Entrys[i-1].Range - pageData.TotalNum;
			else		
				nChange = pageData.Entrys[i-1].Range - pageData.Entrys[i-1].RangeData.LastRange;
			end
			if nChange < 0 then
				changeSign = "↑";
			elseif nChange > 0 then
				changeSign = "↓";
			end
			SortAreaFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
			SortAreaFont:SetText( changeSign..math.abs( nChange ) );
			if nChange == 0 then
				SortAreaFont:SetText("  -");
			end
			
			btn:SetClientUserData(0,pageData.Entrys[i-1].RangeData.GID.nLow);
			btn:SetClientUserData(1,pageData.Entrys[i-1].RangeData.GID.nHeight);
			btn:Show();
		end
	elseif nCurSelType == RANGE_MAIN_PET then
		--宠物
		for i = 1,pageData.EntryNum do
			if i > MAX_SHOW_COUNT then
				return;
			end
			local btn		= getglobal("SortInfo"..i);
			local rankFont		= getglobal("SortInfo"..i.."SortIndexFont");
			local NameFont		= getglobal("SortInfo"..i.."NameFont");
			local LvFont		= getglobal("SortInfo"..i.."LvFont");
			local CareerFont	= getglobal("SortInfo"..i.."CareerFont");
			local ClanNameFont	= getglobal("SortInfo"..i.."ClanNameFont");
			local SortAreaFont	= getglobal("SortInfo"..i.."SortAreaFont");

			local nColorIndex = 1;
			--根据和主角的好友关系 改变颜色
			local friendInfo = GameFriendManager:getFriendInfoByName( pageData.Entrys[i-1].RangeData.SecondName );
			if friendInfo ~= nil then
				nColorIndex = 2;
			end
			--主角颜色
			if pageData.Entrys[i-1].RangeData.SecondName == mainplayer:getName() then
				nColorIndex = 3;
			end
			
			--排名
			rankFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
			rankFont:SetText( pageData.Entrys[i-1].Range );
			--	名字
			NameFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
			NameFont:SetText( pageData.Entrys[i-1].RangeData.FirstName );
			-- 主人名字
			LvFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
			LvFont:SetText( pageData.Entrys[i-1].RangeData.SecondName );
			--等级
			CareerFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
			CareerFont:SetText( pageData.Entrys[i-1].RangeData.Level );
			--统计数据种类
			ClanNameFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
			ClanNameFont:SetText( pageData.Entrys[i-1].RangeData.RangeVal );

			local changeSign = "";
			local nChange = 0;
			--说明这个人是刚上榜的
			if pageData.Entrys[i-1].RangeData.LastRange == -1 then
				nChange = pageData.Entrys[i-1].Range - pageData.TotalNum;
			else		
				nChange = pageData.Entrys[i-1].Range - pageData.Entrys[i-1].RangeData.LastRange;
			end
			if nChange < 0 then
				changeSign = "↑";
			elseif nChange > 0 then
				changeSign = "↓";
			end
			SortAreaFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
			SortAreaFont:SetText( changeSign..math.abs( nChange ) );
			if nChange == 0 then
				SortAreaFont:SetText("  -");
			end
			
			btn:SetClientUserData(0,pageData.Entrys[i-1].RangeData.GID.nLow);
			btn:SetClientUserData(1,pageData.Entrys[i-1].RangeData.GID.nHeight);
			btn:Show();
		end
	elseif nCurSelType == RANGE_MAIN_COMBAT then
		for i = 1,pageData.EntryNum do
			if i > MAX_SHOW_COUNT then
				return;
			end
			local btn		= getglobal("SortInfo"..i);
			local rankFont		= getglobal("SortInfo"..i.."SortIndexFont");
			local NameFont		= getglobal("SortInfo"..i.."NameFont");
			local LvFont		= getglobal("SortInfo"..i.."LvFont");
			local CareerFont	= getglobal("SortInfo"..i.."CareerFont");
			local ClanNameFont	= getglobal("SortInfo"..i.."ClanNameFont");
			local SortAreaFont	= getglobal("SortInfo"..i.."SortAreaFont");

			local nColorIndex = 1;
			--根据和主角的好友关系 改变颜色
			local friendInfo = GameFriendManager:getFriendInfoByName( pageData.Entrys[i-1].RangeData.FirstName );
			if friendInfo ~= nil and friendInfo.m_nRelationType == CLIENT_RELATION_TYPE_FRIEND then
				nColorIndex = 2;
			end			--主角颜色
			if pageData.Entrys[i-1].RangeData.FirstName == mainplayer:getName() then
				nColorIndex = 3;
			end
			
			--排名
			rankFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
			rankFont:SetText( pageData.Entrys[i-1].Range );
			--	名字
			NameFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
			NameFont:SetText( pageData.Entrys[i-1].RangeData.FirstName );
			-- 统计数据种类
			LvFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
		--[[
			if nCurSelSubType == RANGE_MAIN_COMBAT_SUB_PANSHAN_TIME + 1 or nCurSelSubType == RANGE_MAIN_COMBAT_SUB_DRAGON_TIME + 1 or nCurSelSubType == RANGE_MAIN_COMBAT_SUB_MOGUI_TIME + 1 then
				local totalTime = pageData.Entrys[i-1].RangeData.RangeVal;
				local szTime = "";
				if ( totalTime / 3600 ) >= 1 then
					szTime = math.modf( totalTime / 3600 ) .. "h";
					totalTime = totalTime%3600;
				end
				if ( totalTime / 60 ) >= 1 then
					szTime =szTime .. math.modf( totalTime / 60 ) .. "m";
					totalTime = totalTime%60;
				end		
				szTime = szTime ..totalTime .. "s";
				LvFont:SetText( szTime );
			else		
				LvFont:SetText( pageData.Entrys[i-1].RangeData.RangeVal );
			end
			--]]
			LvFont:SetText( pageData.Entrys[i-1].RangeData.RangeVal );
			CareerFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
			ClanNameFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
			if nCurSelSubType == RANGE_MAIN_COMBAT_SUB_BCT + 1 then
				CareerFont:SetText( t_raceList[ pageData.Entrys[i-1].RangeData.Career ] );
				ClanNameFont:SetText( pageData.Entrys[i-1].RangeData.SecondName );
			else
				--副本名
				CareerFont:SetText( pageData.Entrys[i-1].RangeData.SecondName );
				--副本难度
				ClanNameFont:SetText("")
				--[[
				if pageData.Entrys[i-1].RangeData.Level == 0 then
					ClanNameFont:SetText( "容易" );
				elseif pageData.Entrys[i-1].RangeData.Level == 1 then
					ClanNameFont:SetText("普通");
				elseif pageData.Entrys[i-1].RangeData.Level == 2 then
					ClanNameFont:SetText("困难");
				end
				]]--
			end
			local changeSign = "";
			local nChange = 0;
			--说明这个人是刚上榜的
			if pageData.Entrys[i-1].RangeData.LastRange == -1 then
				nChange = -1;
			else		
				nChange = pageData.Entrys[i-1].Range - pageData.Entrys[i-1].RangeData.LastRange;
			end
			if nChange < 0 then
				changeSign = "↑";
			elseif nChange > 0 then
				changeSign = "↓";
			end
			SortAreaFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
			SortAreaFont:SetText( changeSign..math.abs( nChange ) );
			if nChange == 0 then
				SortAreaFont:SetText("  -");
			end
			
			btn:SetClientUserData(0,pageData.Entrys[i-1].RangeData.GID.nLow);
			btn:SetClientUserData(1,pageData.Entrys[i-1].RangeData.GID.nHeight);
			btn:Show();
		end

	elseif nCurSelType == RANGE_MAIN_BATTLE then

		for i = 1,pageData.EntryNum do
			if i > MAX_SHOW_COUNT then
				return;
			end
			local btn		= getglobal("SortInfo"..i);
			local rankFont		= getglobal("SortInfo"..i.."SortIndexFont");
			local NameFont		= getglobal("SortInfo"..i.."NameFont");
			local LvFont		= getglobal("SortInfo"..i.."LvFont");
			local CareerFont	= getglobal("SortInfo"..i.."CareerFont");
			local ClanNameFont	= getglobal("SortInfo"..i.."ClanNameFont");
			local SortAreaFont	= getglobal("SortInfo"..i.."SortAreaFont");

			local nColorIndex = 1;
			--根据和主角的好友关系 改变颜色
			local friendInfo = GameFriendManager:getFriendInfoByName( pageData.Entrys[i-1].RangeData.FirstName );
			if friendInfo ~= nil and friendInfo.m_nRelationType == CLIENT_RELATION_TYPE_FRIEND then
				nColorIndex = 2;
			end			--主角颜色
			if pageData.Entrys[i-1].RangeData.FirstName == mainplayer:getName() then
				nColorIndex = 3;
			end
			
			-- 排名
			rankFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
			rankFont:SetText( pageData.Entrys[i-1].Range );
			-- 名字
			NameFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
			NameFont:SetText( pageData.Entrys[i-1].RangeData.FirstName );
			-- 统计数据种类
			LvFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
			LvFont:SetText( pageData.Entrys[i-1].RangeData.RangeVal );
			-- 战场名
			ClanNameFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
			ClanNameFont:SetText( pageData.Entrys[i-1].RangeData.SecondName );

			local changeSign = "";
			local nChange = 0;
			--说明这个人是刚上榜的
			if pageData.Entrys[i-1].RangeData.LastRange == -1 then
				nChange = -1;
			else		
				nChange = pageData.Entrys[i-1].Range - pageData.Entrys[i-1].RangeData.LastRange;
			end
			if nChange < 0 then
				changeSign = "↑";
			elseif nChange > 0 then
				changeSign = "↓";
			end
			SortAreaFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
			SortAreaFont:SetText( changeSign..math.abs( nChange ) );
			if nChange == 0 then
				SortAreaFont:SetText("  -");
			end
			
			btn:SetClientUserData(0,pageData.Entrys[i-1].RangeData.GID.nLow);
			btn:SetClientUserData(1,pageData.Entrys[i-1].RangeData.GID.nHeight);
			btn:Show();
		end
	elseif nCurSelType == RANGE_MAIN_SPARTA then

		for i = 1,pageData.EntryNum do
			if i > MAX_SHOW_COUNT then
				return;
			end
			local btn		= getglobal("SortInfo"..i);
			local rankFont		= getglobal("SortInfo"..i.."SortIndexFont");
			local NameFont		= getglobal("SortInfo"..i.."NameFont");
			local LvFont		= getglobal("SortInfo"..i.."LvFont");
			local CareerFont	= getglobal("SortInfo"..i.."CareerFont");
			local ClanNameFont	= getglobal("SortInfo"..i.."ClanNameFont");
			local SortAreaFont	= getglobal("SortInfo"..i.."SortAreaFont");
			local RunAwayFont	= getglobal("SortInfo"..i.."RunAwayFont")
			local VRateFont	= getglobal("SortInfo"..i.."VRateFont")

			local nColorIndex = 1;
			--根据和主角的好友关系 改变颜色
			local friendInfo = GameFriendManager:getFriendInfoByName( pageData.Entrys[i-1].RangeData.FirstName );
			if friendInfo ~= nil and friendInfo.m_nRelationType == CLIENT_RELATION_TYPE_FRIEND then
				nColorIndex = 2;
			end			--主角颜色
			if pageData.Entrys[i-1].RangeData.FirstName == mainplayer:getName() then
				nColorIndex = 3;
			end
			
			-- 排名
			rankFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
			rankFont:SetText( pageData.Entrys[i-1].Range );
			-- 名字
			NameFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
			NameFont:SetText( pageData.Entrys[i-1].RangeData.FirstName);
			-- 末世功勋
			LvFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
			LvFont:SetText( pageData.Entrys[i-1].RangeData.RangeVal );
			--等级
			CareerFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
			CareerFont:SetText( pageData.Entrys[i-1].RangeData.Level )
			-- 职业
			ClanNameFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
			ClanNameFont:SetText( t_raceList[ pageData.Entrys[i-1].RangeData.Career ] );
			-- 胜率
			SortAreaFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
			SortAreaFont:SetText( pageData.Entrys[i-1].RangeData.SecondName );
			-- 逃跑
			RunAwayFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
			RunAwayFont:SetText( pageData.Entrys[i-1].RangeData.Val1 );
			-- 升降
			local changeSign = "";
			local nChange = 0;
			--说明这个人是刚上榜的
			if pageData.Entrys[i-1].RangeData.LastRange == -1 then
				nChange = -1;
			else		
				nChange = pageData.Entrys[i-1].Range - pageData.Entrys[i-1].RangeData.LastRange;
			end
			if nChange < 0 then
				changeSign = "↑";
			elseif nChange > 0 then
				changeSign = "↓";
			end
			VRateFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
			VRateFont:SetText( changeSign..math.abs( nChange ) );
			if nChange == 0 then
				VRateFont:SetText("  -");
			end

			local changeSign = "";
			local nChange = 0;
			--说明这个人是刚上榜的
			if pageData.Entrys[i-1].RangeData.LastRange == -1 then
				nChange = -1;
			else		
				nChange = pageData.Entrys[i-1].Range - pageData.Entrys[i-1].RangeData.LastRange;
			end
			if nChange < 0 then
				changeSign = "↑";
			elseif nChange > 0 then
				changeSign = "↓";
			end
			VRateFont:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b );
			VRateFont:SetText( changeSign..math.abs( nChange ) );
			if nChange == 0 then
				VRateFont:SetText("  -");
			end
			
			btn:SetClientUserData(0,pageData.Entrys[i-1].RangeData.GID.nLow);
			btn:SetClientUserData(1,pageData.Entrys[i-1].RangeData.GID.nHeight);
			btn:Show();
		end

	end

	SortLineFrameCurPageBtnPageFont:SetText( nCurPage .. "/" .. nMaxPage  );

	local szText = "";
	local rankInfo = RankMgr:getCurMyRank(nCurSelType,nCurSelSubType - 1);
	if nCurSelType == RANGE_MAIN_SINGLE or nCurSelType == RANGE_MAIN_ATTR or nCurSelType == RANGE_MAIN_COMBAT or nCurSelType == RANGE_MAIN_BATTLE or nCurSelType == RANGE_MAIN_SPARTA then		
		if rankInfo.CurrRange < 0 then
			szText = "你还没上榜，继续努力吧";
		else
			szText = "你排列在本榜单第" .. rankInfo.CurrRange .."位";
		end
	elseif nCurSelType == RANGE_MAIN_CAREER then
		local race = mainplayer:getRace();	
		if nCurSelSubType ~= race and nCurSelSubType ~= race % 0x10 then
			szText = "你不是一个"..t_raceList[nCurSelSubType];
		else
			if rankInfo.CurrRange < 0 then
				szText = "你还没上榜，继续努力吧";
			else
				szText = "你排列在本榜单第" .. rankInfo.CurrRange .."位";
			end
		end
	elseif nCurSelType == RANGE_MAIN_PET then
		local petPackage = mainplayer:getPet();
		if petPackage:getPetNum() == 0 then
			szText = "你还未拥有宠物";
		else
			if rankInfo.CurrRange < 0 then
				szText = "你的宠物还没上榜，继续努力吧";
			else
				szText = "你的宠物排列在本榜单第" .. rankInfo.CurrRange .."位";
			end			
		end
	elseif nCurSelType == RANGE_MAIN_CLAN then
		if not mainplayer:isInClan() then
			szText = "你还没加入公会";
		else
			if rankInfo.CurrRange < 0 then
				szText = "你的公会还没上榜，继续努力吧";
			else
				szText = "你的公会排列在本榜单第" .. rankInfo.CurrRange .."位";
			end
		end
		
	end
	
	SortLineFrameBackBtnRank:SetText(szText);
	if nCurSelType == RANGE_MAIN_SPARTA then
		if nCurSelSubType - 1 == RANGE_MAIN_SPARTA_SUB_FIGHT then
			SortLineFrameBackBtnTimeFont:SetText("每周参与场次不足10场将会减少100点末世功勋");
			return
		end
	end
	local serverTime = getServerTime();
	serverTime = serverTime.sec + 60*60;
	SortLineFrameBackBtnTimeFont:SetText("排行榜只在整点更新，下次更新时间：" .. os.date("%H",serverTime) .. "点整");
end

--获取标签页的上级标签
function GetSortCategoryParentNodeInfo( childNodeInfo )
	if childNodeInfo.level == 1 then
		return;
	end
	for i = 1, table.getn( t_LabelNameList ) do
		if t_LabelNameList[i].nSubType== 0 and t_LabelNameList[i].nType == childNodeInfo.nType then
			return t_LabelNameList[i];
		end
	end
end

--刷新标签页
function UpdateSortCategoryList( nStartContentIndex )
	local nCurShowBtn = 1; -- 此为指向按钮的指针
	for nContentIndex = nStartContentIndex, table.getn( t_LabelNameList ) do
		-- 检查按钮指针是否越界
		if nCurShowBtn > 18 then
			break;
		end
		local btn			= getglobal( "SortLineFrame_CategoryBtn" .. nCurShowBtn .. "HighlightBtn" );
		local font			= getglobal( "SortLineFrame_CategoryBtn" .. nCurShowBtn .. "HighlightBtnType" );  
		local subTypeFont		= getglobal( "SortLineFrame_CategoryBtn" .. nCurShowBtn .. "HighlightBtnSubType" );
		local partionTex		= getglobal( "SortLineFrame_CategoryBtn" .. nCurShowBtn .. "HighlightBtnPartionTex" );
		btn:DisChecked();
		
		btn:SetClientUserData( 0, 0 );
		btn:SetClientUserData( 1, 0 );
		btn:SetClientUserData( 2, 0 );
		btn:SetClientString( "" );
		-- 若为1级节点
		if t_LabelNameList[nContentIndex].nSubType == 0 then
			font:SetText( t_LabelNameList[nContentIndex].name );
			subTypeFont:SetText("");
			btn:SetClientUserData( 0, t_LabelNameList[nContentIndex].nType );
			btn:SetClientUserData( 1, t_LabelNameList[nContentIndex].nSubType);
			btn:SetNormalTextureTemplate( "GongYongAnNiuA1NormalTemplate" );
			btn:SetHighlightTextureTemplate( "GongYongAnNiuA1HighTemplate" );
			btn:SetPushedTextureTemplate( "GongYongAnNiuA1PushedTemplate" );
			btn:SetDisableTextureTemplate( "GongYongAnNiuA1DisabledTemplate" );
			font:SetTextColor( 255, 254, 189 );
			--btn:SetOverlayRegion( 0, 0, 92, 26 );
			btn:Show();
			partionTex:Hide();
			nCurShowBtn = nCurShowBtn + 1;
		-- 若为2级节点
		else
			-- 先查看其父节点是否为展开状态
			local firstLevelNode = GetSortCategoryParentNodeInfo( t_LabelNameList[nContentIndex] );
			if nCurSelType == 0 and nCurSelSubType == 0 then
				--nCurSelType		= t_LabelNameList[nContentIndex].nType;
				--nCurSelSubType	= t_LabelNameList[nContentIndex].nSubType; 
				DisCheckAllHighLightBtn( "SortLineFrame_CategoryBtn", 15 );
				btn:DisHighlight();
				btn:Checked();
			elseif nCurSelType == t_LabelNameList[nContentIndex].nType 
				and nCurSelSubType == t_LabelNameList[nContentIndex].nSubType  then
				DisCheckAllHighLightBtn( "SortLineFrame_CategoryBtn", 15 );
				btn:DisHighlight();
				btn:Checked();
			end
			-- 若其父节点为展开状态, 则显示当前的2级节点内容,否则跳过当前2级节点
			if firstLevelNode.bCheck then
				subTypeFont:SetText( t_LabelNameList[nContentIndex].name ); 
				font:SetText("");
				btn:SetClientUserData( 0, t_LabelNameList[nContentIndex].nType );
				btn:SetClientUserData( 1, t_LabelNameList[nContentIndex].nSubType );
				btn:SetNormalTextureTemplate( "TransTextureTemplate" );
				btn:SetHighlightTextureTemplate( "HighlightTextureTemplate" );
				btn:SetPushedTextureTemplate( "PushedTextureTemplate" );
				btn:SetDisableTextureTemplate( "TransTextureTemplate" );
				
				font:SetTextColor( 152, 139, 116 );
				--btn:SetOverlayRegion( -2, 0, 94, 26 );
				btn:Show();
				partionTex:Show();
				nCurShowBtn = nCurShowBtn + 1;
			end
		end
	end
	-- 将没使用的按钮隐藏起来
	for i = nCurShowBtn, 18 do
		local btn	= getglobal( "SortLineFrame_CategoryBtn" .. i .. "HighlightBtn" );
		btn:Hide();
		local font	= getglobal( "SortLineFrame_CategoryBtn" .. i .. "HighlightBtnType" );
		font:SetText( "" );
	end
end

--复制名字
function SortLineFrame_CopySelectNameToMemory()
	if nSelIndex > 0 then
		local nameFont = getglobal( "SortInfo"..nSelIndex.."NameFont" );
		CopyToMemory( nameFont:GetText() );
	end
end

function SortLineFrame_requestJoinClan()
	--判断是否已经加入公会
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	if mainplayer:isInClan() then
		ShowMidTips("你已经加入一个公会");
	end
	if nSelIndex > 0 then
		local btn = getglobal( "SortInfo"..nSelIndex );
		if nCurSelType == RANGE_MAIN_CLAN then
			RankMgr:requestJoinClan( btn:GetClientUserData(0),btn:GetClientUserData(1) );
		end
	end
	
end

--右键菜单
local t_Contact	= {};
function SetSortIndexRightList()
	t_Contact = {};
	if nCurSelType == RANGE_MAIN_SINGLE or nCurSelType == RANGE_MAIN_ATTR or nCurSelType == RANGE_MAIN_CAREER or nCurSelType == RANGE_MAIN_COMBAT or nCurSelType == RANGE_MAIN_BATTLE or nCurSelType == RANGE_MAIN_SPARTA then
		t_Contact = 
			{			
				[1] = { text = "观察玩家", func = function () 
									SortLineFrame_WatchBtn_OnClick(); 
								end },
				
				[2] = { text = "加为好友", func = function () 
									SortLineFrame_AddFriendBtn_OnClick(); 
								end },
				[3] = { text = "私聊", func = function () 
									SortLineFrame_PrivateBtn_OnClick(); 
								end }, 
				[4] = { text = "复制名字", func = function () 
									SortLineFrame_CopySelectNameToMemory(); 
								end },
			}
	elseif nCurSelType == RANGE_MAIN_PET then
		t_Contact = 
			{
				[1] = { text = "观察", func = function () 
											SortLineFrame_WatchBtn_OnClick(); 
										end },
				[2] = { text = "私聊所有者", func = function () 
									SortLineFrame_PrivateBtn_OnClick(); 
								end },
			}
	elseif nCurSelType == RANGE_MAIN_CLAN then
		t_Contact = 
			{
				[1] = { text = "申请加入公会", func = function () 
								SortLineFrame_requestJoinClan(); 
							end },
			}
	end
	--个人  魅力 特殊添加送花和砸蛋选项
	if nCurSelType == RANGE_MAIN_SINGLE and (nCurSelSubType == RANGE_MAIN_SINGLE_SUB_CHARM + 1 or nCurSelSubType == RANGE_MAIN_SINGLE_SUB_CHARM_WEEK + 1) then
		t_Contact = 
			{			
				[1] = { text = "观察玩家", func = function () 
									SortLineFrame_WatchBtn_OnClick(); 
								end },
				
				[2] = { text = "加为好友", func = function () 
									SortLineFrame_AddFriendBtn_OnClick(); 
								end },
				[3] = { text = "私聊", func = function () 
									SortLineFrame_PrivateBtn_OnClick(); 
								end }, 
				[4] = { text = "复制名字", func = function () 
									SortLineFrame_CopySelectNameToMemory(); 
								end },
				[5] = { text = "送花", func = function () 
									SortLineFrame_FlowerBtn_OnClick(); 
								end },
				[6] = { text = "砸鸡蛋", func = function () 
									SortLineFrame_EggsBtn_OnClick(); 
								end },
			}
	end
end

--点击标签页
function SortLineFrame_CategoryBtn_OnClick()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return
	end

	if this:GetClientUserData(1) >0 then
		nCurSelType	= this:GetClientUserData( 0 );
		nCurSelSubType	= this:GetClientUserData( 1 );
		bShowSearch = false;
	end

	for i = 1, table.getn( t_LabelNameList ) do
		if t_LabelNameList[i].nType == this:GetClientUserData( 0 ) 
			and t_LabelNameList[i].nSubType == this:GetClientUserData( 1 )  then
			t_LabelNameList[i].bCheck = not t_LabelNameList[i].bCheck;	
		elseif this:GetClientUserData( 1 ) == 0 then
			t_LabelNameList[i].bCheck = false;
		end
	end
	if this:GetClientUserData( 1 ) > 0 then
		nCurPage = 1;
		showActorOperateButton( false );
		RefreshPageData(true);
	end	

	UpdateSortCategoryList( 1 );
end

function SortLineFrame_OnShow()
	if LoginMgr:isInBattleWorld() then
		ShowMidTips("战场中不能进行该操作");
		this:Hide();
		return;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer ~= nil and mainplayer:isOutSide() then
		ShowMidTips("跨服不允许该操作");
		this:Hide();
		return;
	end
	UpdateSortCategoryList( 1 );
	this:SetPoint( "center", "$parent", "center", 0, 0 );
	RefreshPageData(true);
	if nCurSelType == 0 and nCurSelSubType == 0 then
		nCurSelSubType = RANGE_MAIN_SINGLE_SUB_LEVEL + 1;
	end	
end

function SortInfo_OnClick()
	--取消其他的选中状态
	for i = 1,MAX_SHOW_COUNT do
		local btn = getglobal("SortInfo"..i);
		btn:DisChecked();
	end
	if arg1 == "RightButton" then
		SortInfoListMenuFrame:Show();
		SortInfoListMenuFrame:SetPoint( "topleft", "$parent", "topleft", GetCursorPosX(), GetCursorPosY() );
	end
	nSelIndex = this:GetClientID();
	this:Checked();
	if nCurSelType == RANGE_MAIN_SINGLE or nCurSelType == RANGE_MAIN_ATTR or nCurSelType == RANGE_MAIN_CAREER or nCurSelType == RANGE_MAIN_COMBAT or nCurSelType == RANGE_MAIN_BATTLE or nCurSelType == RANGE_MAIN_SPARTA then
		showActorOperateButton( true );
	end
end

--翻页
function SortLineFrame_FirstPageBtn_OnClick()
	nCurPage = 1;
	RefreshPageData(true);
end

function SortLineFrame_PrePageBtn_OnClick()
	if nCurPage > 1 then
		nCurPage = nCurPage - 1;
		RefreshPageData(true);
	end
end

function SortLineFrame_NextPageBtn_OnClick()
	if nCurPage < nMaxPage then
		nCurPage = nCurPage + 1;
		RefreshPageData(true);
	end
end

function SortLineFrame_LastPageBtn_OnClick()
	nCurPage = nMaxPage;
	RefreshPageData(true);
end

function SortLineFrame_SearchBtn_OnClick()
	local szSearchText = SortLineFrameInputBtnSearchText:GetText();
	if szSearchText == "" then
		SetFocusFrame("SortLineFrameInputBtnSearchText");
		ShowMidTips("请输入查询内容");
		return
	end
	bShowSearch = true;
	nCurPage = 1;
	nMaxPage = 1;
	RefreshPageData(true);
end

function showActorOperateButton( bShow )
	if bShow then
		SortLineFrameWatchBtn:Show();
		SortLineFramePrivateBtn:Show();
		SortLineFrameAddFriendBtn:Show();
	else
		SortLineFrameWatchBtn:Hide();
		SortLineFramePrivateBtn:Hide();
		SortLineFrameAddFriendBtn:Hide();
	end
end

function SortLineFrame_WatchBtn_OnClick()
	if nSelIndex > 0 then
		local btn = getglobal( "SortInfo"..nSelIndex );
		local nameFont = getglobal( "SortInfo"..nSelIndex.."NameFont" );
		local clanFont = getglobal( "SortInfo"..nSelIndex.."LvFont" );

		if nCurSelType == RANGE_MAIN_SINGLE or nCurSelType == RANGE_MAIN_ATTR or nCurSelType == RANGE_MAIN_CAREER or nCurSelType == RANGE_MAIN_COMBAT or nCurSelType == RANGE_MAIN_BATTLE or nCurSelType == RANGE_MAIN_SPARTA then
			local szName = nameFont:GetText();
			RankMgr:requestWatchPlayer( szName );
		elseif nCurSelType == RANGE_MAIN_PET then
			local nLow = btn:GetClientUserData(0);
			local nHeigh = btn:GetClientUserData(1);
			local szOwnerName = clanFont:GetText();
			RankMgr:requestWatchPet( nLow,nHeigh,szOwnerName,string.len(szOwnerName) );
		else
			ShowMidTips("当前类别下不能观察");
		end
	end
end

function SortLineFrame_PrivateBtn_OnClick()
	if nSelIndex > 0 then
		local nameFont = nil;
		if nCurSelType == RANGE_MAIN_SINGLE or nCurSelType == RANGE_MAIN_ATTR or nCurSelType == RANGE_MAIN_CAREER or nCurSelType == RANGE_MAIN_COMBAT or nCurSelType == RANGE_MAIN_BATTLE or nCurSelType == RANGE_MAIN_SPARTA then
			nameFont = getglobal( "SortInfo"..nSelIndex.."NameFont" );
		elseif nCurSelType == RANGE_MAIN_PET then
			nameFont = getglobal( "SortInfo"..nSelIndex.."LvFont");
		end
		
		if nCurSelType == RANGE_MAIN_SINGLE or nCurSelType == RANGE_MAIN_ATTR or nCurSelType == RANGE_MAIN_CAREER or nCurSelType == RANGE_MAIN_PET or nCurSelType == RANGE_MAIN_COMBAT or nCurSelType == RANGE_MAIN_BATTLE or nCurSelType == RANGE_MAIN_SPARTA then
			local szName = nameFont:GetText();
			local t_ChatInsertFrameControl = GetChatInsertFrameControl();
			t_ChatInsertFrameControl:setPrivatePlayerName( { ["name"] = szName } );
			SetChatType( CL_CHAT_PRIVATE );
			UpdateChatInsertFrameEnterFocus();
			SetFocusFrame("CSChatInsert");
		end
	end
end
--送花
function SortLineFrame_FlowerBtn_OnClick()
	if nSelIndex > 0 then
		local nameFont = getglobal( "SortInfo"..nSelIndex.."NameFont" );
		local szName = nameFont:GetText();
		local mainplayer = ActorMgr:getMainPlayer()
		if mainplayer:getName() == szName then
			ShowMidTips( "不能给自己送花" )
			return 
		end	
		OpenUseFlower( 0, szName )
	end
end
--砸鸡蛋
function SortLineFrame_EggsBtn_OnClick()
	if nSelIndex > 0 then
		local nameFont = getglobal( "SortInfo"..nSelIndex.."NameFont" );
		local szName = nameFont:GetText();
		local mainplayer = ActorMgr:getMainPlayer()
		if mainplayer:getName() == szName then
			ShowMidTips( "不能给自己砸蛋" )
			return 
		end
		OpenUseFlower( 1, szName )
	end
end

function SortLineFrame_AddFriendBtn_OnClick()
	if nSelIndex > 0 then
		local nameFont = getglobal( "SortInfo"..nSelIndex.."NameFont" );
		if nCurSelType == RANGE_MAIN_SINGLE or nCurSelType == RANGE_MAIN_ATTR or nCurSelType == RANGE_MAIN_CAREER or nCurSelType == RANGE_MAIN_COMBAT or nCurSelType == RANGE_MAIN_BATTLE or nCurSelType == RANGE_MAIN_SPARTA then
			local szName = nameFont:GetText();
			GameFriendManager:addFriend( szName,"" );
		end
	end
end


function SortInfotipOnShow()
	local rich = getglobal( "SortInfotipRichText" );
	local frame = getglobal( "SortInfotip" );

	local szText = SortInfotipRichText:GetClientString();
	local nWidth = 1;
	for i = 1, rich:GetTextLines() do
		local nLineWidth = rich:getLineWidth( i - 1 );
		if nWidth < nLineWidth then
			nWidth = nLineWidth;
		end
	end
	local nHeight = rich:GetTotalHeight();
	nHeight = ( nHeight == 0 ) and 1 or nHeight;

	rich:resizeRect( math.ceil( nWidth + 5 * GetScreenScaleY() ), math.ceil( nHeight + 5 * GetScreenScaleY() ) );
	frame:resizeRect( math.ceil( nWidth + 6 * GetScreenScaleY() ), math.ceil( nHeight + 6 * GetScreenScaleY() ) );
	frame:SetSize( math.ceil( frame:GetRealWidth() / GetScreenScaleY() ),  math.ceil(frame:GetRealHeight() / GetScreenScaleY() ) );
end

function SortInfo_OnEnter()
	--判断是否是好友
	local nIndex = this:GetClientID();
	local nameFont = getglobal( "SortInfo"..nIndex.."NameFont" );
	local secondNameFont = getglobal("SortInfo"..nIndex.."LvFont");
	if nCurSelType == RANGE_MAIN_SINGLE or nCurSelType == RANGE_MAIN_ATTR or nCurSelType == RANGE_MAIN_CAREER or nCurSelType == RANGE_MAIN_COMBAT or nCurSelType == RANGE_MAIN_BATTLE then
		local szName = nameFont:GetText();
		local friendInfo = GameFriendManager:getFriendInfoByName( szName );
		if friendInfo ~= nil then
			if friendInfo.m_nRelationType == CLIENT_RELATION_TYPE_FRIEND then
				local szText = szName .. "是你的好友";
				ShowGameTooltip( { rich = "SortInfotipRichText", tipsFrame = "SortInfotip", text = szText, frame =  this:GetParent(),
							button = "cursor" } );
			elseif friendInfo.m_nRelationType == CLIENT_RELATION_TYPE_SU_DI then
				local szText = szName .. "是你的宿敌";
				ShowGameTooltip( { rich = "SortInfotipRichText", tipsFrame = "SortInfotip", text = szText, frame =  this:GetParent(),
							button = "cursor" } );
			elseif friendInfo.m_nRelationType == CLIENT_RELATION_TYPE_XIONG_SHOU then
				local szText = szName .. "是杀你的凶手";
				ShowGameTooltip( { rich = "SortInfotipRichText", tipsFrame = "SortInfotip", text = szText, frame =  this:GetParent(),
							button = "cursor" } );
			end
		end
	elseif nCurSelType == RANGE_MAIN_PET then
		local szName = secondNameFont:GetText();
		local friendInfo = GameFriendManager:getFriendInfoByName( szName );
		if friendInfo ~= nil then
			if friendInfo.m_nRelationType == CLIENT_RELATION_TYPE_FRIEND then
				local szText = szName .. "是你的好友";
				ShowGameTooltip( { rich = "SortInfotipRichText", tipsFrame = "SortInfotip", text = szText, frame =  this:GetParent(),
							button = "cursor" } );
			elseif friendInfo.m_nRelationType == CLIENT_RELATION_TYPE_SU_DI then
				local szText = szName .. "是你的宿敌";
				ShowGameTooltip( { rich = "SortInfotipRichText", tipsFrame = "SortInfotip", text = szText, frame =  this:GetParent(),
							button = "cursor" } );
			elseif friendInfo.m_nRelationType == CLIENT_RELATION_TYPE_XIONG_SHOU then
				local szText = szName .. "是杀你的凶手";
				ShowGameTooltip( { rich = "SortInfotipRichText", tipsFrame = "SortInfotip", text = szText, frame =  this:GetParent(),
							button = "cursor" } );
			end
		end
	elseif nCurSelType == RANGE_MAIN_CLAN then
		local szName = nameFont:GetText();
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer == nil then return end
		if mainplayer:isInClan() then
			local clanInfo = GameClanManager:getClanViewer();
			if szName == clanInfo.m_clanInfo.m_szClanName then
				local szText = szName .. "是你的公会";
				ShowGameTooltip( { rich = "SortInfotipRichText", tipsFrame = "SortInfotip", text = szText, frame =  this:GetParent(),
						button = "cursor" } );
			end
		end
	end
	SortInfotip:SetPoint("center", this:GetName(), "center", 0, 15);
end

function SortInfo_OnLeave()
	SortInfotip:Hide();
end
local nSortInfoMsgTime = 0;
function SortInfoMsgFrame_OnShow()
	local rich = getglobal( "SortInfoMsgFrameTipRich" );
	local frame = getglobal( "SortInfoMsgFrame" );
	nSortInfoMsgTime = getServerTime().sec;
	--[[
	local nWidth = 1;
	for i = 1, rich:GetTextLines() do
		local nLineWidth = rich:getLineWidth( i - 1 );
		if nWidth < nLineWidth then
			nWidth = nLineWidth;
		end
	end
	]]--

	local nHeight = rich:GetTotalHeight();
	nHeight = ( nHeight == 0 ) and 1 or nHeight;
	rich:SetSize( 145,nHeight+10 );
	rich:resizeText( 145,nHeight+10 );

	--rich:resizeRect( math.ceil( nWidth + 14 * GetScreenScaleY() ), math.ceil( nHeight + 14 * GetScreenScaleY() ) );
	--rich:SetSize( math.ceil( nWidth + 14 * GetScreenScaleY() ), math.ceil( nHeight + 14 * GetScreenScaleY() ) );
	--frame:resizeRect( math.ceil( 165 * GetScreenScaleY() ), math.ceil( nHeight + 8 * GetScreenScaleY() ) );
	--frame:SetSize( math.ceil( (165 * GetScreenScaleY() ),  math.ceil( (frame:GetRealHeight() + 16 ) / GetScreenScaleY() ) );
	frame:SetSize( 165,nHeight + 30 );
	
	rich:Show();
	SortInfoMsgFrame:SetPoint( "bottomleft", "RangeBtn", "bottomleft", -170, nHeight );
end

function SortInfoMsgFrame_OnHide()
	CurTipsInfo.MainType = -1;
	CurTipsInfo.SubType = -1;
end
function SortInfoMsgFrame_OnLoad()
	SortInfoMsgFrameTipRich:setCenterLine( true );
	this:setUpdateTime(1);
end
function SortInfoMsgFrame_OnUpdate()
	local curTime = getServerTime().sec;
	if curTime - nSortInfoMsgTime > 10 then 
		this:Hide();
	end
end
function SortInfoMsgFrameRich_OnClick()
	if CurTipsInfo.MainType == -1 then
		return;
	end
	local TipsInfo = CurTipsInfo;
	if TipsInfo ~= nil then --and TipsInfo.CurrRange ~= -1 then
		nCurSelType = TipsInfo.MainType;
		nCurSelSubType = TipsInfo.SubType + 1;
		nCurPage = math.floor( (TipsInfo.CurrRange - 1)/MAX_SHOW_COUNT ) + 1;
		if nCurPage <= 0 then
			nCurPage = 1
		end
		bShowSearch = false;
		SortInfoMsgFrame:Hide();
		for i=1,table.getn( t_LabelNameList ) do
			if t_LabelNameList[i].nType == nCurSelType then
				if t_LabelNameList[i].nSubType == 0 or t_LabelNameList[i].nSubType ==nCurSelSubType then
					t_LabelNameList[i].bCheck = true;
				else
					t_LabelNameList[i].bCheck = false;
				end				
			else
				t_LabelNameList[i].bCheck = false;
			end
		end
		SortLineFrame:Show();
	end
end

function SortLineFrameOpen( nMainType,nSubType )
	nCurSelType = nMainType;
	nCurSelSubType = nSubType + 1;
	nCurPage = 1;
	bShowSearch = false;
	for i=1,table.getn( t_LabelNameList ) do
		if t_LabelNameList[i].nType == nCurSelType then
			if t_LabelNameList[i].nSubType == 0 or t_LabelNameList[i].nSubType ==nCurSelSubType then
				t_LabelNameList[i].bCheck = true;
			else
				t_LabelNameList[i].bCheck = false;
			end				
		else
			t_LabelNameList[i].bCheck = false;
		end
	end
	SortLineFrame:Show();
end

--清空排行榜数据
function ClearRankInfoData()
	--CurTipsInfo.MainType = -1;
	--CurTipsInfo.SubType = -1;
	--nCurSelType = RANGE_MAIN_SINGLE ;
	--nCurSelSubType = RANGE_MAIN_SINGLE_SUB_LEVEL + 1;
	nCurPage = 1;
	nMaxPage = 1;
	for i = 1, table.getn( t_LabelNameList ) do
		if t_LabelNameList[i].nType == nCurSelType 
			and t_LabelNameList[i].nSubType == nCurSelSubType  then
			t_LabelNameList[i].bCheck = not t_LabelNameList[i].bCheck;	
		elseif nCurSelSubType == 0 then
			t_LabelNameList[i].bCheck = false;
		end
	end
	SortInfoMsgFrame:Hide();
	RankMgr:clearInfo();
end

function SortInfoListMenu_OnClick()
	t_Contact[this:GetClientID()].func();
	SortInfoListMenuFrame:Hide();
end

local MAXSHOWCONTACTNUM = 10;
function SortInfoListMenuFrame_OnShow()
	for i=1,MAXSHOWCONTACTNUM do
		local btn = getglobal("SortInfoListMenu"..i);
		btn:DisChecked();
	end
	local nContactNum	= 0;
	local nBtnIndex		= 1;  
	
	for _, data in ipairs(t_Contact) do
		local namebtn	= getglobal( "SortInfoListMenu"..( nBtnIndex ) );
		local namefont	= getglobal( "SortInfoListMenu"..( nBtnIndex ).."Name" );
		namebtn:Show();
		namefont:SetText( data.text );
		nContactNum = nContactNum + 1;
		nBtnIndex = nBtnIndex + 1;
	end

	if nContactNum == 0 then
		ContactListTipsFrame:Hide();
		return;
	end

	for i = nContactNum + 1, 10 do
		local namebtn = getglobal( "SortInfoListMenu"..i );
		namebtn:Hide();
	end

	SetFocusFrame( "SortInfoListMenuFrame" );
	SortInfoListMenuFrame:SetSize(90,nContactNum*16+10);
end

------------------------赶牛排行榜-------------------------

function CowboyRankFrame_OnLoad()
	this:RegisterEvent("GE_COW_FINAL_RANGE_INFO");
end

local MAX_RANKINFOBTN_NUM = 10;
local onRange = false;
local isTakeAward = 0;

function CowboyRankFrame_OnEvent()
	if arg1 == "GE_COW_FINAL_RANGE_INFO" then
		local uiMsg = UIMSG:getUIMsg();
		local finalRangeInfo = uiMsg.CowFinalRangeData;
		local rangeInfo = finalRangeInfo.CowFinalRange;
		if rangeInfo == nil then
			return
		end;
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer == nil then
			return
		end;
		local npcID = NpcDialog:getTalkNpcMemID();

		if finalRangeInfo.OpType == 0 then
			local num = 0;
			onRange = false;
			isTaskAward = 1;

			if not CowboyRankFrame:IsShown() then
				CowboyRankFrame:Show();
				util.testNpcDialogDistance( npcID, CowboyRankFrame );
			end

			for i=1, MAX_RANKINFOBTN_NUM do
				if i > rangeInfo.Num then
					break;
				end
				local btn = getglobal( "CowboyRankFrame_ListBackBtnRankInfo" .. i );
				local sortFont = getglobal( btn:GetName() .. "SortIndexFont" );
				sortFont:SetText( rangeInfo.Members[i-1].Range );
				local nameFont = getglobal( btn:GetName() .. "NameFont" );
				nameFont:SetText( rangeInfo.Members[i-1].Name );
				local lvFont = getglobal( btn:GetName() .. "LvFont" );
				lvFont:SetText( rangeInfo.Members[i-1].Level );
				local careerFont = getglobal( btn:GetName() .. "CareerFont" );
				careerFont:SetText( t_allCareerDesc[rangeInfo.Members[i-1].Career].desc );
				local timeFont = getglobal( btn:GetName() .. "TimeFont" );
				local szTime = os.date("%M", rangeInfo.Members[i-1].Time)..":"..os.date("%S", rangeInfo.Members[i-1].Time);
				timeFont:SetText( szTime );
				if mainplayer:isSameRoleWId( rangeInfo.Members[i-1].RoleID ) then
					num = i;
					onRange = true;
					isTakeAward = rangeInfo.Members[i-1].TakeAward;
					sortFont:SetTextColor( 10, 215, 0 );
					nameFont:SetTextColor( 10, 215, 0 );
					lvFont:SetTextColor( 10, 215, 0 );
					careerFont:SetTextColor( 10, 215, 0 );
					timeFont:SetTextColor( 10, 215, 0 );
				end
			end			

			if onRange then
				RankInfoFramePlayerRankFont:SetText( "你在本榜单第" .. num .. "位" );
				if isTakeAward == 0 then
					RankInfoFrameGetAwardBtn:Enable();
					RankInfoFrameGetAwardBtnFont:SetTextColor( 255, 255, 190 );
				end
			end
		elseif finalRangeInfo.OpType == 1 then
			if not CowboyRankFrame:IsShown() then
				return;
			end
			for i=1, MAX_RANKINFOBTN_NUM do
				if i > rangeInfo.Num then
					break;
				end
				if mainplayer:isSameRoleWId( rangeInfo.Members[i-1].RoleID ) then
					onRange = true;
					isTakeAward = rangeInfo.Members[i-1].TakeAward;
					if isTakeAward == 1 then
						RankInfoFrameGetAwardBtn:Disable();
						RankInfoFrameGetAwardBtnFont:SetTextColor( 120, 120, 120 );
					end
					break;
				end
			end
		end
	end
end

function CowboyRankFrame_OnShow()
	CowboyRankFrame:SetPoint( "center", "UIClient", "center", 0, 0 );

	for i=1, MAX_RANKINFOBTN_NUM do
		local btn = getglobal( "CowboyRankFrame_ListBackBtnRankInfo" .. i );
		local sortFont = getglobal( btn:GetName() .. "SortIndexFont" );
		sortFont:SetText("");
		sortFont:SetTextColor( 255, 255, 255 );
		local nameFont = getglobal( btn:GetName() .. "NameFont" );
		nameFont:SetText("");
		nameFont:SetTextColor( 255, 255, 255 );
		local lvFont = getglobal( btn:GetName() .. "LvFont" );
		lvFont:SetText("");
		lvFont:SetTextColor( 255, 255, 255 );
		local careerFont = getglobal( btn:GetName() .. "CareerFont" );
		careerFont:SetText("");
		careerFont:SetTextColor( 255, 255, 255 );
		local timeFont = getglobal( btn:GetName() .. "TimeFont" );
		timeFont:SetText("");
		timeFont:SetTextColor( 255, 255, 255 );
		if i%2 == 0 then
			btn:SetNormalTextureTemplate( "BlackCategoryBkg" );
		end
	end

	RankInfoFramePlayerRankFont:SetText( "你当前暂未上榜" );
	RankInfoFrameGetAwardBtn:Disable();
	RankInfoFrameGetAwardBtnFont:SetTextColor( 120, 120, 120 );	
end

function RankInfoFrameGetAwardBtn_OnClick()
	if not onRange or isTakeAward ~= 0 then
		return;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer ~= nil then
		mainplayer:requestCowRangeAward();
	end
end

function GetTopOneLv()
	return nTopLevel;
end

--服务器刷新排行榜时刷新服务器最高人物等级
function SetTopOneLv( nLv )
	nTopLevel = nLv;
end