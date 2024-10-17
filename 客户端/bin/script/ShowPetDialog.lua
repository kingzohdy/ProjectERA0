--GameWizard:setNpcTalk( nMemID, szTalk, "ChatPop", false, 0, 0, "PopBackArrowTexture", true,0,0,0,8 );
--showpos	PETPHOTO 宠物头像 PETHEAD 宠物头顶 PETBUTTON 宠物按钮
--showtype	CHATPOP 聊天泡泡 CHATTIP 提示
local t_PetDialog = 
	{
		-- 进入地图的时候 1000 表示地图ID interval表示每次发言的间隔时间  rate 表示发言的概率 showtype 表示发言的类型 泡泡或 TIPS showpos表示发言的位置 
		["EnterMap"]		=	{	startclock=0, interval=0, 
									[1000] =	{ rate=100, showpos="PETHEAD", showtype="CHATPOP", text=
																												{ 
																													"哇！时间城耶！听说这里有宠物训练师！",
																												}
												};
									[10200] =	{ rate=100, showpos="PETHEAD", showtype="CHATPOP", text=
																												{ 
																													"这里，冷得让人难受呀！",
																												}
												}
								}; 
		-- 在地图的时候 1000表示地图ID
		["InMap"]			=	{  startclock=0, interval=900,
									[1000] =	{ rate=30, showpos="PETPHOTO", showtype="CHATTIP", text=
																										{ 
																													"主人，难得来一次时间城，带我去找宠物训练师吧！",
																													"时间城太大了，我头都晕了！",
																										}
												}
								}; 
		-- 宠物出征的时候
		["PetCamp"]			=	{ startclock=0, interval=0, rate=60, showpos="PETHEAD", showtype="CHATPOP", text=
																			{ 
																				"你终于肯让我出来透透气了!",
																				"猛宠出笼了，哥要发威了！",
																			}
								};
		-- 宠物未出征的时候
		["PetNotCamp"]		=	{ startclock=0, rate=70, showpos="PETBUTTON", showtype="CHATTIP", interval=120, text=
																								{ 
																									"#petname#：法律已经阻止不了我了，快放我出来！",
																									"#petname#：快放我出来呀，再不放我，我咬你呀！",
																								} 
								};
		-- 宠物死亡的时候
		["PetDead"]			=	{ startclock=0, interval=0, rate=100, showpos="PETHEAD", showtype="CHATPOP",  text=
																			{ 
																				"我……败……了……！",
																				"十八秒钟后，又是一条好汉！如果你还肯放我出来的话！",
																				"哎呀，疼疼疼疼！！！我晕血，我不行了！",
																			}
								};
		-- 宠物长到特定等级的时候 20表示宠物等级
		["PetLv"]			=	{ startclock=0, interval=0, 
									[20] = { rate=100, showpos="PETHEAD", showtype="CHATPOP", text=
																				{ 
																					"哥20级了，手痒了，我们要去收拾谁呢！",
																				}
											}
								};
		-- 宠物每次升级的时候
		["PetLevelUp"]		=	{ startclock=0, interval=0, rate=30, showpos="PETHEAD", showtype="CHATPOP",text=
																			{ 
																				"我已经充满力量了！！！",
																			}
								};
		-- 宠物忠诚度达到某个范围的时候  minloveval=3000 表示最小忠诚度, maxloveval=5000 表示最大忠诚度
		["PetLoveval"]		=	{ startclock=0, interval=600, rate=100, showpos="PETPHOTO", showtype="CHATTIP", text=
																			{ 
																				{ minloveval=3000, maxloveval=5000, text ="你不爱我了，都不喂我吃的了！" },
																			};
								};
		-- 玩家的血量低于某个范围的时候 minhp=10 表示最小血量, maxhp=50 表示最大血量
		["PlayerHpLow"]		=	{ 
									startclock=0, interval=30, rate=100, showpos="PETPHOTO", showtype="CHATTIP",  text =
																				{ 
																					{ minhp=10, maxhp=50, text ="老大，你没血了，还不嗑药，难道要我嘴对嘴喂你么！！" },
																					
																				}
								};
		
		-- 玩家的装备低于某个范围的时候 arm_unuse (耐久度为0 红) 装备不可用 arm_brea（耐久度为0.09）装备损坏
		["PlayerArmDur"]	=	{	startclock=0, interval=1000,
									["arm_unuse"] =	{ rate=100, showpos="PETPHOTO", showtype="CHATTIP", text =
																		{ 
																			"主人，装备红了，快去修呀！你想裸奔么？",
																		}
															};
									["arm_break"] =	{  rate=100, showpos="PETHEAD", showtype="CHATPOP", text =
																{
																	"主人，装备黄了，快去修，穿得破破烂烂，别影响我形象啊！",
																}
													};
								};
		-- 玩家等级达到特定等级的时候 30表示玩家等级
		["PlayerLv"]		=	{   startclock=0, interval=0, 
									[30]	= { text="恭喜主人荣升30级！", rate=100, showpos="PETHEAD", showtype="CHATPOP"};
								};
		-- 玩家静止不动达 duration 秒的时候 
		["PlayerStand"]		=	{
									 startclock=0, interval=600, rate=50, showpos="PETPHOTO", showtype="CHATTIP", duration=60, text =
																							{ 
																								"主人，你想去彩虹岛吗？",
																								"主人，听说这世界上有一种能够让宠物更加完美的东西，你知道是什么吗？",
																							}
								};
		-- 系统活动开始的时候 1 表示活动ID
		["Promotion"]		=	{
									startclock=0, interval=0, [1] = { rate=100, showpos="PETHEAD", showtype="CHATPOP", text="喂喂喂，双倍经验了啊，还不快点干活，杀怪去呀！" };
								};
	}

function GetPetDialog( dialogType )
	if t_PetDialog[dialogType].interval ~= 0 and ( os.clock() - t_PetDialog[dialogType].startclock ) < t_PetDialog[dialogType].interval then
		return nil;
	end
	return t_PetDialog[dialogType]; 
end