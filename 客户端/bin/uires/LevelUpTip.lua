local ClanPworlTotalTime = 0
local t_ClanPworld = {
					[1] = [[
					
#c461e0a	活动名称：驱逐入侵#c554623(公会活动)
#c461e0a	活动NPC：#L#cff3c00斯蒂芬斯@@[303006,1000]#n#c554623(时间城)
#c461e0a	活动地图：公会冶炼岛
#c461e0a	人物等级：≥10级]],
					[2] = [[#c554623	●击杀"入侵者"获得个人积分：



#c554623	●个人积分可兑换<个人积分奖励>

#c554623	●完成公会积分要求，发放<公会积分奖励>
#c0000ff	获取方式：发放时活动中的会员自动获取



#c554623	 ●击杀#L#cff3c00"肆掠者"#n,可领取<肆掠者宝箱奖励>
#c0000ff	提示:使用机械可对"肆掠者"造成巨大伤害
#c0a6400	 <肆掠者宝箱奖励>
#c461e0a	 人物等级≥10:重铸石、洗炼石
#c461e0a	 人物等级≥61:星辉宝钻

#c554623	 ●活动结束,将保留剩余的"个人积分"]],
					[3] = [[	
#c554623	●个人积分=#c0a640050#c554623,奖励翻倍提升
#c554623	●个人积分>#cff000050#c554623,奖励大幅减少
#c0000ff	(兑换个人积分奖励,增加"公会积分")
]],
					}

local t_LevelUp = {
				[130]		= [[#cd44800·宠物·				
#c6f5220    [10级]完成宠物教学任务，就可以携带更多外形多样能力强大的宠物
#c6f5220    起始任务名：[教学]初识宠物
#cd44800·地图·
#c6f5220    时间城是我们最后的家园，您可以去领略一下天空之城的辉煌，您还可以：
#c6f5220    加入公会[#L公会管理员-斯蒂芬斯@@[303006,1000]#n]
#c6f5220    学习技能[#L技能训练大师-康拉德@@[306000,1000]#n]
#c6f5220    您可以到细语丘陵继续进行冒险旅程。]],

				[150]		= [[#cd44800·副本-战鼓村·
#c6f5220    BOSS：赫巴贡酋长
#c6f5220    进入条件：接受任务-[主线]拯救首领、等级≥15
#c6f5220    副本入口：#L幻影[细语丘陵]@@[303005,2100]#n
#c6f5220    推荐等级：18~20级
#c6f5220    副本难度：低
#c6f5220    副本奖励：大量经验、蓝色品质装备
#cd44800·急你所急航空·
#c6f5220    时间城的急你所急航空欢迎您的光顾，在这里你只要花些许银币便能享受优质的航空服务，穿梭于时间城各区域中。
#c6f5220    航空NPC：急你所急航空-#L细语丘陵@@[333056,2100]#n
#cd44800·地图·
#c6f5220    您可以继续在细语丘陵进行冒险旅程。]],


				[250]		= [[
#cd44800·副本—永夜谷·
#c6f5220    BOSS：狼王杰克、吸血鬼伯爵亚库里斯、黑暗魔法师康斯坦丁
#c6f5220    副本入口：永夜指引-#L博尔特[银风雪原]@@[302028,2200]#n
#c6f5220    推荐等级：29~35级
#c6f5220    副本奖励：30级套装（蓝色）、专家难度产出紫色装备]],


				[300]	=[[#cd44800·公会·
#c6f5220    [30级]您可以建立自己的公会。如果您已经加入公会，可接受公会专属任务。
#c6f5220    NPC：公会管理员-#L斯蒂芬斯[时间城]@@[303006,1000]#n
#cd44800·坐骑·
#c6f5220    [30级]帮助阿诺德赢得一场运动会，你就会获得一只提升移动速度的坐骑
#c6f5220    起始任务名：[教学]运动会
#cd44800·新玩法-龙蛋谷·
#c6f5220    次数限制：1次/每天
#c6f5220    活动入口：龙蛋谷开发商-#L哈蒙德[时间城]@@[302046,1000]#n
#c6f5220    活动奖励：大量经验、装备强化道具、宠物幻化道具
#cd44800·新玩法-盘山道·
#c6f5220    次数限制：1次/每天
#c6f5220    活动入口：盘山道指挥官-#L卡奥逊[时间城]@@[303007,1000]#n
#c6f5220    活动奖励：大量银币、宠物技能书、装备强化道具
#cd44800·新玩法-商运·
#c6f5220    [30级]在基尔特商会-#L亨利[时间城]@@[302041,1000]#n接受商运任务，完成商运可获得大量经验、银币。
#cd44800·地图·
#c6f5220    [30级]您可以前往流金沙漠开始冒险旅程。]],


				[400]	=[[#cd44800·新玩法-狂沙角斗场
#c6f5220    次数限制：不限制
#c6f5220    活动入口：#L狂沙角斗场-罗宾逊[时间城]@@[302047,1000]#n
#c6f5220    活动奖励：大量PVP装备，生存奖章，优胜奖章
#cd44800·新玩法—欢乐嘉年华·
#c6f5220    开放时间：请留意活动助手提示
#c6f5220    活动入口：欢乐嘉年华-#L奈皮尔[时间城]@@[302045,1000]#n
#c6f5220    活动奖励：变身药水、宠物幻化道具、坐骑	

#cd44800·地图·
#c6f5220    [40级]您可以前往落日荒野开始冒险旅程。
#c6f5220    [45级]可以到十三号矿洞领取环组任务，获得低级藏宝图。]],



				[450]	=[[
#cd44800·新副本-黄金矿井·
#c6f5220    推荐等级：45~55级
#c6f5220    活动入口：#L阿隆克[落日荒野]@@[302035,2400]#n
#c6f5220    活动奖励：50级套装（蓝色）、专家难度产出紫色装备
#cd44800·地图·
#c6f5220    [45级]可以到十三号矿洞领取环组任务，可获得低级藏宝图。]],

				[500]	=[[#cd44800·职业坐骑·
#c6f5220    [50级]职业坐骑是各职业特有的坐骑，你现在可以向阿诺德咨询有关职业坐骑的信息，他将会引导你如何去获取。
#c6f5220    接受任务NPC：坐骑商人-#L阿诺德[时间城]@@[302001,1000]#n
#c6f5220    推荐等级：50级
#cd44800·转职·
#c6f5220    最低等级：[55级]
#c6f5220    任务名称：英雄之路
#c6f5220    接任务NPC：技能训练大师-#L康拉德[时间城]@@[306000,1000]#n
#c6f5220    转职是每位英雄成长中的必经之路，转职后你的能力将得到质的飞跃。
#cd44800·地图·
#c6f5220    [50级]您可以前往越雾雨林开始冒险旅程。
#c6f5220    [55级]可以到越雾甬道领取环组任务，可获得藏宝图。]],

				[580]	=[[#cd44800·精灵·
#c6f5220    [58级]拥有精灵后可获得精灵强大的能力，并召唤精灵附体！时间城大行政官阿尔弗雷德将指引你如何获得第一只精灵——雷神！
#c6f5220    推荐等级：60级
#c6f5220    起始任务名：[精灵]你和精灵有个约会
#c6f5220    接受任务NPC：#L时间城大行政官-阿尔弗雷德@@[306013,1000]#n[时间城]  
]],


				[600]	=[[#cd44800·新玩法-绿洲风暴战场
#c6f5220    次数限制：不限制
#c6f5220    活动入口：打开活动助手报名，或者点击右上图标报名参加
#c6f5220    活动奖励：大量PVP装备，生存奖章，战魂徽章]],


				[700]	=[[#cd44800·副本-太阳神殿·
#c6f5220    次数限制：1次/每天
#c6f5220    活动入口：#L安迪尔-费斯[时间城]@@[304061,1000]#n
#c6f5220    活动奖励：装备强化宝石、坐骑强化石、扩展背包等珍稀道具
				]],

				[130]	=[[#cd44800·怒风荒漠·
#c6f5220    30级后你可以到怒风荒漠继续冒险，体验更加丰富多彩的任务。
#cd44800·龙蛋谷副本·
#c6f5220    30级后，你可以通过时间城的#L哈蒙德@@[302046,1000]#n到龙蛋谷副本进行战斗，获得更多的经验和更好的装备。
#cd44800·运镖护送活动·
#c6f5220    30级后，你可以通过时间城的#L亨利@@[302041,1000]#n参加运镖护送活动，获得更多的金钱和经验。
#cd44800·狂沙角斗场·
#c6f5220    30级后，你可以通过时间城的#L罗宾逊@@[302047,1000]#n报名参加狂沙角斗场多人竞技的PVP活动，获得更多的经验和更好的装备及炫酷的坐骑。
#cd44800·阿努比斯神殿·
#c6f5220    30级后，你可以到阿努比斯神殿进行战斗，获得更多的经验。
#cd44800·精灵·
#c6f5220    [10级]完成精灵教学任务，可获得一只拥有强大技能的“雷神”精灵
#c6f5220    起始任务名：[教学]你和精灵有个约会
#c6f5220    接任务NPC：#L罗伯特[时间城]@@[302003,1000]#n]],


				[130]	=[[#cd44800·落日荒野·
#c6f5220    40级后你可以到落日荒野继续冒险，体验更加丰富多彩的任务。
#cd44800·黄金矿井副本·
#c6f5220    40级后，你可以通过#L里奥@@[302026,2400]#n到黄金矿井副本进行战斗，获得力量徽章，力量徽章可在时间城的#L梅杰@@[302033,1000]#n处兑换第二套职业套装。
#cd44800·十三号矿洞·
#c6f5220    40级后，你可以到十三号矿洞进行战斗，获得更多的经验。]],


				[131]	=[[#cd44800·玛雅雨林·
#c6f5220    50级后你可以到玛雅雨林继续冒险，体验更加丰富多彩的任务。
#cd44800·玛雅甬道·
#c6f5220    50级后，你可以到玛雅甬道进行战斗，获得更多的经验。]],



				[130]	= [[#cd44800·时间城·
#c6f5220    你可以传送到时间城，在#L安东尼@@[304001,2000]#n 处可进行传送。
#cd44800·精灵·
#c6f5220    15级可以到时间城xxxxxxxxxxxxx。
#cd44800·宠物·
#c6f5220    15级可以到时间城xxxxxxxxxxxxx
#cd44800·彩虹岛·
#c6f5220    15级可以到时间城xxxxxxxxxxxxx]],
				};
			
--公会驱逐个人积分	
local AwardVal = 0
--公会驱逐公会积分
local AwardClanVal =0
local AwardClanMaxVal = 0
local ShowBoss = 0

function setLevelUpTips( nlevel )
	if t_LevelUp[ nlevel ] ~= nil then
		local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
		t_centerXinShouBtnControl:ShowExtraBtn( { itemDefId = 0, Type = CENTER_XINSHOU_BTN_LEVEL, frame = "LevelUpTipFrame", 
												path ="uires\\TuPianLei\\PiaoYiTiShiTuBiao\\!.tga", level = nlevel } );
	end
end

function setClanPworldTips()
	local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
	if t_centerXinShouBtnControl:getIndexByType( CENTER_CLAN_PWORLD ) == 0 then
		t_centerXinShouBtnControl:ShowExtraBtn( { itemDefId = 0, Type = CENTER_CLAN_PWORLD, frame = "ClanPworldTipFrame", 
												path ="uires\\TuPianLei\\PiaoYiTiShiTuBiao\\12.tga", index = 0 } );
	end
end

function setClanPworldTipsFrameInfo( clanContri, clanMaxVal, clanMoney, clanVal, val, endTime, playerMoney, playerExp, showBoss )
	local mainplayer = ActorMgr:getMainPlayer();
	local levelupdef = getLevelUP( mainplayer:getLv() );
	local maxClanMoney = 1500 * 10/(510-50*10);
	local maxPlayerExp = levelupdef.ContributeExp / 200;
	ClanPworldTipFrameClanMoney:SetText( "公会资金："..clanMoney.."/"..maxClanMoney );
	ClanPworldTipFramePersonalExp:SetText( "个人经验："..playerExp.."/"..maxPlayerExp );
	AwardVal = val;
	AwardClanVal = clanVal;
	AwardClanMaxVal = clanMaxVal;
	ShowBoss = showBoss
	local ClanViewer		= GameClanManager:getClanViewer();
	local ClanInfo			= ClanViewer.m_clanInfo;
	local clanLevelDef		= getClanLevelDef( ClanInfo.m_nClanLevel);
	local PersonalGetValFontString = getglobal("ClanPworldTipFramePersonalGetVal")
	if clanVal >= clanMaxVal then
		local PersonalGetValTextFontString = getglobal("ClanPworldTipFramePersonalGetValText");
		PersonalGetValTextFontString:SetTextColor( 128,128,128 );
		PersonalGetValFontString:SetTextColor( 128,128,128 );
		local szInfo = clanLevelDef.ClanValContri
		PersonalGetValFontString:SetText( szInfo );
	else
		PersonalGetValFontString:SetText( clanLevelDef.ClanValContri );
		PersonalGetValFontString:SetTextColor( 70,30,10 );	
	end
	ClanPworlTotalTime = endTime;
	SetEndTime();
end
function SetClockNum( num )
	if num >9 then
		return tostring( num );
	else
		return "0"..num;
	end
end
function SetEndTime()
	if ClanPworlTotalTime == 0 then
		return;
	end
	local timer = ClanPworlTotalTime - getServerTime().sec;
	if timer <= 0 then
		ClanPworldGameEnd();
		return ;
	end
	local nHour = math.floor( timer/3600 )
	local nMinute =  math.floor( ( timer - nHour*3600 ) / 60 )
	local nSecond = math.ceil( timer - nHour*3600 - nMinute*60 )
	local ClockTime = "";
	if nHour > 0 then 
		ClockTime = SetClockNum( nHour )..":"..SetClockNum( nMinute )..":"..SetClockNum( nSecond )
	else 
		ClockTime = SetClockNum( nMinute )..":"..SetClockNum( nSecond );
	end
	local EndTimeFontString	= getglobal("ClanPworldTipFrameEndTime")
	EndTimeFontString:SetText( ClockTime );
end
function ClanPworldTipFrame_onUpdate()	
	local ClanViewer		= GameClanManager:getClanViewer();
	local ClanInfo			= ClanViewer.m_clanInfo;
	local clanLevelDef	= getClanLevelDef( ClanInfo.m_nClanLevel);
	local PersonalGetValFontString = getglobal("ClanPworldTipFramePersonalGetVal")
	if AwardClanVal >= AwardClanMaxVal then
		local PersonalGetValTextFontString = getglobal("ClanPworldTipFramePersonalGetValText");
		PersonalGetValTextFontString:SetTextColor( 128,128,128 );
		PersonalGetValFontString:SetTextColor( 128,128,128 );
		PersonalGetValFontString:SetText( clanLevelDef.ClanValContri );
	else
		PersonalGetValFontString:SetText( clanLevelDef.ClanValContri );
		PersonalGetValFontString:SetTextColor( 70,30,10 );	
	end
	if IsClanPworldBegin() then
		ClanPworldTipFrameClanPworldStateRunning:SetGray( false );
		ClanPworldTipFrameClanPworldStateStateFont:SetText( "进行中" );
		ClanPworldTipFrameGetScore:Enable();
		ClanPworldTipFrameEnterClanPworldIsland:Enable();
		SetEndTime();
	end
end

function LevelUpTipFrame_ScrollBar_OnValueChanged()
	local slider = getglobal("LevelUpTipFrameScrollBar");
	local rich	= getglobal( "LevelUpTipFrameSecondRich" );
	rich:SetDispPos( rich:GetStartDispPos() );
	for i = 1, slider:GetValue() do
		rich:ScrollDown();
	end
end
function ClanPworldTipFrame_onHide()
	SetClanPworldFrameShow( false )
end
function ClanPworldTipFrame_onShow()
	local InfoRich = getglobal( "ClanPworldTipFrameSecondRichInfo" );
	InfoRich:SetLinkTextColor( 10, 100, 0);
	InfoRich:SetText( t_ClanPworld[1],111,82,32 );
	local GiftRich = getglobal( "ClanPworldTipFrameSecondRichGift" );
	GiftRich:SetLinkTextColor( 10, 100, 0);
	GiftRich:SetText( t_ClanPworld[3],111,82,32 );
	local RoleRich = getglobal( "ClanPworldTipFrameSecondRichRole" );
	RoleRich:SetLinkTextColor( 10, 100, 0);
	RoleRich:SetText( t_ClanPworld[2],111,82,32 );	
	this:SetPoint( "center", "UIClient", "center", 0, 0 );
	if IsClanPworldBegin() then
		ClanPworldTipFrameClanPworldStateRunning:SetGray( false );
		ClanPworldTipFrameClanPworldStateStateFont:SetText( "进行中" );
		ClanPworldTipFrameGetScore:Enable();
		ClanPworldTipFrameEnterClanPworldIsland:Enable();
		SetEndTime();
	end
	SetClanPworldFrameShow( true );
end
function ShowClanPworldFrame()
	ClanPworldTipFrameClanPworldStateRunning:SetGray( true );
	ClanPworldTipFrameClanPworldStateStateFont:SetText( "未开启" );
	ClanPworldTipFrameGetScore:Disable();
	ClanPworldTipFrameEnterClanPworldIsland:Disable();
	ClanPworldTipFrameEndTime:SetText( "" );
	ClanPworldTipFrame:Show();
end
function HideClanPworldFrame()
	local ClanPworldTipFrame = getglobal( "ClanPworldTipFrame" );
	ClanPworldTipFrame:Hide();
end
function ClanPworldTipFrameAwardBtn_onClick()
	if AwardVal > 0 then
		GameClanManager:clan_Opt_PworldAward();
	else
		ShowMidTips( "无兑换奖励所需的个人积分" );
	end
end
function ClanPworldIslandShowInfoFrameExChange_onEnter()
	SetGameTooltips( this:GetName(), "点击兑换<个人积分奖励>\n个人积分=#c00ff0050#cffffbe,奖励翻倍提升\n个人积分>#cff000050#cffffbe,奖励大幅减少" );
end
function ClanPworldIslandShowInfoFramekulou_onClick()
	if not GameClanManager:IsInClanPworldIsland() then
		ShowMidTips( "您当前不在公会冶炼岛" );
		return;
	end
	
	MapShower:GetMaxMapContal():linkMoveto( CLAN_EVERYDAY_PWORLD_MAP_ID_LUA, 16700, 19000 );
end
function ClanPworldIslandShowInfoFramekulou_onEnter()
	SetGameTooltips( this:GetName(), "点击前往“肆掠者”出现位置\n#c00ff00出现要求：完成公会积分要求" );
end
function SecondRichRole_OnClick()
	if not GameClanManager:IsInClanPworldIsland() then
		ShowMidTips( "您当前不在公会冶炼岛" );
		return;
	end
	
	MapShower:GetMaxMapContal():linkMoveto( CLAN_EVERYDAY_PWORLD_MAP_ID_LUA, 16700, 19000 );
end
function ClanPworldTipFrameEnterClanPworldIsland_onClick()
	--[[
	if GameClanManager:IsInClanPworldIsland() then
		ShowMidTips( "您当前已在公会冶炼岛" );
		return;
	end
	--]]
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer:getLv() < 10 then
		ShowMidTips( "人物等级未达到活动要求，暂时无法参加" );
		return;
	end
	local nCurMapID;
	local MaxMapControl	= MapShower:GetMaxMapContal();
	if MaxMapControl:getMapID() == 0 then
		nCurMapID = MapShower:getRealMapId();
	else
		nCurMapID = MaxMapControl:getMapID();
	end
	if nCurMapID >= MIN_COPY_ID then
		ShowMidTips( "当前副本不能传送" );
		return;
	end
	GameClanManager:clan_Opt_EnterClanPworldIsland();
end

function ShowLevelUpTipFrame( nlevel )
	if t_LevelUp[ nlevel ] ~= nil then
		LevelUpTipFrameFirstLineFont:SetText("恭喜您升到".. nlevel .."级，您将体验到以下内容");
		local rich = getglobal("LevelUpTipFrameSecondRich");
		local slider  = getglobal( "LevelUpTipFrameScrollBar" );  
		local sliderThumbTex = getglobal( "LevelUpTipFrameScrollBar".."ThumbTexture" );
		rich:SetLinkTextColor(10,110,0);
		rich:SetText( t_LevelUp[ nlevel ],111,82,32 );
		local nViewLine	= rich:GetViewLines();
		local nAllLine	= rich:GetTextLines();
		rich:SetDispPos(rich:GetStartDispPos());
		slider:SetValue(0);
		slider:Hide();
		sliderThumbTex:Hide();
		if nAllLine - nViewLine > 0 then
			local nMaxValue = nAllLine - nViewLine; 
			slider:Show();
			slider:SetMaxValue( nMaxValue );
			slider:SetMinValue(0);
			slider:SetValueStep(1);
		end
		LevelUpTipFrame:Show();
	end
end
function ClanPworldIslandShowInfoFrame_onUpdate()
	local szColor = "#c888888";
	if AwardVal == 50 then
		szColor = "#c00ff00";
	elseif AwardVal > 50 then
		szColor = "#cff0000";
	end
	ClanPworldIslandShowInfoFramePersonalVal:SetText( szColor..AwardVal.."#cffffbe/50", 255, 120, 120 )
	local szInfo = ( AwardClanVal >= AwardClanMaxVal ) and "#c888888"..AwardClanVal.."#cffffbe/"..AwardClanMaxVal or "#cff0000"..AwardClanVal.."#cffffbe/"..AwardClanMaxVal
	ClanPworldIslandShowInfoFrameClanVal:SetText( szInfo, 255, 120, 120 )
	if AwardVal >= 50 then
		local tex = getglobal( "ClanPworldIslandShowInfoFrameExChange_Texture" )
		tex:SetUVAnimation( 48, true );
		tex:Show();
	else
		local tex = getglobal( "ClanPworldIslandShowInfoFrameExChange_Texture" )
		tex:Hide();
	end

	if ShowBoss > 0 then
		local tex = getglobal( "ClanPworldIslandShowInfoFramekulou_Texture" )
		tex:SetUVAnimation( 48, true );
		tex:Show();
	else
		local tex = getglobal( "ClanPworldIslandShowInfoFramekulou_Texture" )
		tex:Hide();
	end
end
function LevelUpTipFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 0, 0 );
end

local t_Message =
{

				[1]   =  { "公会建立",
			      [[
#cd44800·公会1级·
		#c6f5220公会建立，会员数量上限50人。
		#c6f5220现在你可以发展属于自己的公会了，让你的朋友都加入进来吧。
#cd44800·内容·
		#c6f5220现在你可以在#L公会管理员-斯蒂芬斯@@[303006,1000]#n接取奖励丰富的日常任务。
		#c6f5220可以在#L公会管理员-斯蒂芬斯@@[303006,1000]#n接取公会环组任务，根据任务类型的不同可以获得公会的不同属性值。

#cd44800·小提示·
		#c6f5220如果想让自己公会更容易被关注，那么就提高公会自身的安定度和人数吧,人数和在线率可关联繁荣度增减
		#c6f5220除了公会拥有的基本功能外，还可以自己编辑各个官阶的名称，会长可以对每个公会成员设置有趣的昵称当做备注
		
		#c6f5220注：公会人数少于10人，持续7天后，将被系统回收，回收后清空公会所有信息]],
						},
			
				[2]   =  { "公会升级",
			      [[
#cd44800·公会2级·
		#c6f5220公会到达2级，会员数量上限至80人。
		#c6f5220现在你可以去升级科技中心了，科技中心到达2级后，有更多更好的科技可以学习。

#cd44800·小提示·
		#c6f5220想让自己公会被更容易被关注，那么就提高公会自身的安定度和成员的在线率吧
	
		#c6f5220注：公会人数少于10人，持续7天后，将被系统回收，回收后清空公会所有信息]],
	
						},	
						
				[3]   =  { "公会升级",
			      [[
#cd44800·公会3级·
		#c6f5220公会到达3级，会员数量上限至120人。
		#c6f5220公会有新的科技可以研究，可在公会科技处查看。
		#c6f5220公会自动参与BOSS养成计划，现在可以前往公会城市对BOSS灌注能量

#cd44800·小提示·
		#c6f5220想让自己公会被更容易被关注，那么就提高公会自身的安定度和成员的在线率吧
	
		#c6f5220注：公会人数少于10人，持续7天后，将被系统回收，回收后清空公会所有信息]],
	
						},
								
				[4]   =  { "公会升级",
			      [[
#cd44800·公会4级·
		#c6f5220公会到达4级，会员数量上限至160人。
		#c6f5220公会有新的科技可以研究，可在公会科技处查看。

#cd44800·小提示·
		#c6f5220想让自己公会被更容易被关注，那么就提高公会自身的安定度和成员的在线率吧
	
		#c6f5220注：公会人数少于10人，持续7天后，将被系统回收，回收后清空公会所有信息]],
	
						},
				[5]   =  { "公会升级",
			      [[
#cd44800·公会5级·
		#c6f5220公会到达5级，会员数量上限至200人。
		#c6f5220公会有新的科技可以研究，可在公会科技处查看。

#cd44800·小提示·
		#c6f5220想让自己公会被更容易被关注，那么就提高公会自身的安定度和成员的在线率吧
	
		#c6f5220注：公会人数少于10人，持续7天后，将被系统回收，回收后清空公会所有信息]],
	
						},
														
						
						
----[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[ ----公会城市

				[10]		= {"公会城市建立",
								[[
#cd44800·公会城市1级·
		#c6f5220您的公会所有成员拥有了进入本公会专属城市的权利。
		#c6f5220#L公会管理员-斯蒂芬斯@@[303006,1000]#n传送进入公会城市地图。
		#c6f5220#L城市行政长官-克里斯提尼@@[304051,60000]#n处建设科技中心
		#c6f5220然后可在#L城市行政长官-克里斯提尼@@[304051,60000]#n处建设工厂
#cd44800·建筑·     
		#c6f5220科技中心#L科技研发-琼恩@@[304052,60000]#n：学习各种公会科技的唯一途径。
		#c6f5220工厂#L工厂制造-罗伯@@[12800,16200]#n：学习了对应的科技后就来工厂制造拉风的道具吧。
		#c6f5220资源中心：制造的蒸汽科技道具、机械、采集资源的存放处。
#cd44800·地图·
		#c6f5220您的公会城市属于本公会成员的专属地图哦，您可以自由出入，参加有趣的任务和活动。
		#c6f5220在这里，也可以暂时放松下你的心情，看看美丽的风景。
#cd44800·小提示·
		#c6f5220地图最初是封闭的，需要建设科技中心后才可到达更多的区域。]],
								},
							
				[11]		= {"公会城市升级",
								[[
#cd44800·公会城市2级·

		#c6f5220#L城市行政长官-克里斯提尼@@[304051,60000]#n处建设工厂
#cd44800·建筑·     
		#c6f5220科技中心#L科技研发-琼恩@@[304052,60000]#n：学习制造科技
		#c6f5220工厂制造#L工厂管理员@@[304055,60000]#n：当学习制造科技后，即可建设工厂，可以制造采集车消耗的能量以及机械等物品。

#cd44800·地图·
		#c6f5220在这里，可以暂时放松下你的心情，看看美丽的风景哦。]],
							},			
								
							
------[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[---科技中心
			   [20]   =  { "科技中心",
		      [[
#cd44800·科技中心1级·
		#c6f5220科技中心建设成功，可在#L科技研发-琼恩@@[304053,60000]#n处学习以下科技类型
		#c6f5220采集科技：采集科技包括普通采集车制造等
		#c6f5220补给道具：采集车没能量了，没血量了，学习对应的科技制造对应的道具：油壶-补充能量，机械扳手-恢复机械血量
		#c6f5220蒸汽科技：腐蚀弹、40MM火箭筒 等等
#cd44800·小提示·
		#c6f5220科技可以多个同时学习，省时省力	]],

						},	
---[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[---采集中心				
			 [30]   =  { "采集中心",
			      [[
#cd44800·采集中心1级·
		#c6f5220制造采集车：可以制造4种不同的基础型采集车，用于采集各种不同的资源
		#c6f5220进入采集场：可以驾驶采集车进入采集场，也可以只身前往，但里面有厉害的矿工要当心哦。
					 <任何公会成员、公会采集车都可进入此地图>
					 
		#c6f5220#L采集中心管理@@[304053,60000]#n升级采集中心]],
						},			

			 [31]   =  { "采集中心升级",
			      [[
#cd44800·采集中心2级·
		#c6f5220可学习更高的采集科技，增强采集车的各种属性。
		#c6f5220制造采集车：可以制造4种不同的基础型采集车，用于采集各种不同的资源
		#c6f5220进入采集场：可以驾驶采集车进入采集场，也可以只身前往，但里面有厉害的矿工要当心哦。
					  <任何公会成员、公会采集车都可进入此地图>
								  
		#c6f5220#L采集中心管理@@[304053,60000]#n升级采集中心]],
						},			
							
				
----[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[----工厂

			 [40]   =  { "工厂",
			      [[
#cd44800·工厂·
		#c6f5220现在你的公会可以制造一些机械和蒸汽科技武器了，但需要先学会科技
		#c6f5220采集车：油矿采集车组件、矿石采集车组件
		#c6f5220蒸汽科技：油壶、机械扳手、腐蚀弹、40MM火箭筒]],
						},			


			 [180]   =  { "尖峰时刻",
			      [[
#cd44800·怎么获得奖励·
		#c6f5220人类：每存活10秒获得1次经验奖励。
		#c6f5220机械怪物：击杀人类、击杀机械巨人可获得奖励
		#c6f5220机械巨人：击杀人类、击杀机械怪物可获得奖励		
#cd44800·阵营胜负规则·
		#c6f5220人类胜利：活动结束时，人类只要有1人存活，则人类胜利
		#c6f5220机械怪物胜利：活动结束时，没有人类存活，并且机械怪物数量 > 机械巨人数量即为胜利
		#c6f5220机械巨人胜利：活动结束时，没有人类存活，并且机械巨人数量 > 机械怪物数量即为胜利
#cd44800·小提示·
		#c6f5220人类外形虽然弱小，但可以利用灵活的步伐存活到最后
		#c6f5220准备时间时，可以躲避在偏僻的角落，因为你不知道开始后谁是敌人！谁是朋友！
		
						]],
						},	
						
						
			 [200]   =  { "绿洲沙漠战场",
			      [[
#cd44800·目标·
		#c6f5220击杀敌方阵营指挥官即可获得胜利#n
#cd44800·玩法·		
		#c6f5220双方的指挥官每隔几秒就会受到敌方飞机的空袭，占领中央的干扰器可使己方指挥官避免空袭伤害#n
#cd44800·偷袭·			
		#c6f5220双方各自有飞行器，每次可乘坐1人，可到地方基地偷袭。但飞行器每隔10秒才会刷新一架
#cd44800·复活点·	
		#c6f5220中央有两处动态复活点可占领，占领后，可在离自己最近的复活点复活#n
#cd44800·小提示·			
		#c6f5220要时刻注意己方指挥官的血量和提示#n]],
						},

			[210]   =  { "虚空要塞战场",
			      [[
#cd44800·目标·
		#c6f5220将粒子控制器驱动到敌方要塞位置，可摧毁地方要塞获得最终胜利#n
#cd44800·玩法·		
		#c6f5220能源结晶：在地图中可寻找到能源结晶，采集后点击粒子控制器可提交能源。#n
		#c6f5220粒子控制器：在地图中央位置，不同阵营提交能源结晶后，会向对方阵营前进，移动到敌方要塞位置后，可毁灭敌方要塞！#n
#cd44800·特别提示·	
		#c6f5220角色死亡后，自身携带的能源结晶会消失，注意劫杀敌方阵营携带能源结晶玩家。#n
#cd44800·注意·			
		#c6f5220要时刻注意粒子控制器位置，即时劫杀敌方阵营玩家与持续提供能源结晶驱动粒子控制器移动。#n]],
						},


			 [300]   =  { "混乱元素",
			      [[
		#c6f5220随着一阵能量波动，混乱元素出现在#c0181f9【银风雪原】#n。#n	
		
#c6f5220请携带【蒸汽核心】找到混乱元素，与之对话来收集能量!#n
#c6f5220收集完能量后在#L阿尔弗雷德[时间城]@@[306013,1000]#n处提交获得银币奖励。#n
		
#c6f5220混乱元素会在每天的11:00-16:00和18:00-23:00随机出现。#n
#c6f5220每次出现持续8分钟。#n
#c6f5220用于收集能量的蒸汽核心可以通过【商运】活动获得。#n]],["path"] = "uires\\TuPianLei\\PiaoYiTiShiTuBiao\\HunLuanYuanSu.tga",
						},						

			 [301]   =  { "混乱元素",
			      [[
		#c6f5220随着一阵能量波动，混乱元素出现在#c0181f9【十三号矿洞】#n。#n	
		
#c6f5220请携带【蒸汽核心】找到混乱元素，与之对话来收集能量!#n
#c6f5220收集完能量后在#L阿尔弗雷德[时间城]@@[306013,1000]#n处提交获得银币奖励。#n
		
#c6f5220混乱元素会在每天的11:00-16:00和18:00-23:00随机出现。#n
#c6f5220每次出现持续8分钟。#n
#c6f5220用于收集能量的蒸汽核心可以通过【商运】活动获得。#n]],["path"] = "uires\\TuPianLei\\PiaoYiTiShiTuBiao\\HunLuanYuanSu.tga",
						},	
						
			 [302]   =  { "混乱元素",
			      [[
		#c6f5220随着一阵能量波动，混乱元素出现在#c0181f9【阿努比斯神殿】#n。#n	

#c6f5220请携带【蒸汽核心】找到混乱元素，与之对话来收集能量!#n
#c6f5220收集完能量后在#L阿尔弗雷德[时间城]@@[306013,1000]#n处提交获得银币奖励。#n
		
#c6f5220混乱元素会在每天的11:00-16:00和18:00-23:00随机出现。#n
#c6f5220每次出现持续8分钟。#n
#c6f5220用于收集能量的蒸汽核心可以通过【商运】活动获得。#n]],["path"] = "uires\\TuPianLei\\PiaoYiTiShiTuBiao\\HunLuanYuanSu.tga",
						},	

						
			 [310]   =  { "光影之战",
			      [[
    #c6f5220怪物出现，勇士们，向前冲！！#n	
    #c6f5220活动开始，光明区出现#c0181f9暗影之魂#n，暗影区出现#c0181f9圣光之魂#n，10秒内未击杀#c0181f9暗影之魂#n变身为#c0181f9暗影真身#n，#c0181f9圣光之魂#n变身为#c0181f9圣光真身#n。真身具有高伤害的攻击能力。
	#c6f5220光明区击杀任意一只怪，暗影区将刷出一只#c0181f9圣光之魂#n；暗影区杀任意一只怪，光明区将刷出一只#c0181f9暗影之魂#n。#n
    #c6f5220活动结束后，剩余怪物数量少的一方获胜，并给与经验与银币奖励奖励。#Y全场击杀怪物数量排名前60%的玩家可获得银币奖励#n。#n
    #c6f5220活动于每天的12：00-12:10 和16:30-16:40开启，持续10分钟。通过#G薇薇安[时间城]#n进入活动。#n]],

						},	
			[320]    =  { "死亡之星",
			      [[			
    #c6f5220当场中只剩下一个公会枢纽时，枢纽所属的公会胜出，最先被摧毁枢纽的公会为第3名。
    #c6f5220当场中剩余枢纽数＞1，则按照枢纽血量多少排列名次，血量最多的公会胜出。
#G目标#n
    #cff0080击杀敌方公会成员、摧毁敌方公会枢纽#n
#G偷袭#n
     #c6f5220每个阵营各自有飞行器，每次可乘坐1人，可到顺时针方向的公会进行偷袭，可乘坐所有阵营的免费飞行器，免费飞行器往返一次需要15秒
#G小提示#n
    #c6f5220要时刻注意已方公会枢纽的血量]],
						},	
			 [410]   =  { "转职最完全攻略",
[[#c6f5220转职任务——帮助雷蒙盖顿脱离梦境的副本，分为5个步骤：#n
#c6f5220#cFF0080注：以下步骤需按顺序进行，若出现无法进行的情况，请放弃此任务重新接取#n
		
#Y步骤一：#n
    与#L阿克萨@@[306020,4402]#n对话进入幻境
    此幻境在勇士岛（用#L船票@@[303023,1000]#n进入）

#Y步骤二：（第一关）#n
    找到#L雷蒙盖顿的身体@@(50900,4300,20900)#n
    等你靠近他的身体，他的灵魂就会出来见你了
    跟灵魂对话#G接取“梦里相见”任务#n
    使用#G任务背包中的匕首#n完成任务
    恭喜你，你进入下一关了

#Y步骤三：（第二关）#n
    与你身后的#L哈托尔@@(50900,5500,9700)#n对话，做出选择
    选择一：#G签订契约#n（推荐）
      签订契约后哈托尔会变成机械
      左键点击，你就会和哈托尔合体，同时获得它的强力技能
      在哈托尔的帮助下，下一关你就会轻松一些
    选择二：不签订
      雷蒙盖顿会帮你击败哈托尔
      然后你需要独自面对下面的挑战

#Y步骤三：（第三关）#n
    无论你的选择是什么，你都要#L 面临@@(50900,5400,7900)#n挑战
    #G击败机器人I号、机器人II号及传说中的机器人首领威震天#n

#Y步骤四：（第四关）#n
    帮助雷蒙盖顿#G守护#L信念@@(50900,19900,8500)#n#G使它血量回满#n
      #cFF4e00如果你的职业是战士、法师、枪手：
        1、你需要击杀所有怪物
        2、信念会缓慢回血
        3、信念血量回满后坚持3秒
      #c6d00d2如果你的职业是祭司：
        1、信念不会自动回血
        2、所以请使用圣光术给信念加血
        3、刷怪时优先击杀怪物
        4、给信念加满血后坚持3秒

#Y步骤五：#n
与#L雷蒙盖顿的灵魂@@(50900,20200,20700)#n对话离开幻境，完成任务

恭喜你，到了这一步，你的使命就完成了！快拿着学识之书转职去吧，记得一定要选好职业哟！

						]],
						},	
						
						
			 [415]   =  { "公会BOSS养成计划-激活BOSS",
			      [[
#c6f5220公会养成计划的BOSS-冲击波已在公会城市中激活，请大家尽快前往，击杀后会获得非常丰厚的奖励哟！#n	
#c6f5220BOSS激活后3个小时未击杀会自动消失！#n
#c6f5220激活后，系统自动领养下周的BOSS。#n
		
#c6f5220击杀时在BOSS附近的本公会成员皆可获得大量经验奖励；公会自动获得公会资金。#n

						]],
						},	


			[1000]   =  { "时间城争霸战",
			      [[
#cd44800·简介·
		#c6f5220 时间城争霸战已激活，将于下周（第二周的周六20：00）正式开启。#n
		#c6f5220 胜利公会，将成为时间城的主人，享受诸多特权与待遇。详细信息可通过时间城内的帕金斯处查询。#n
#cd44800·战争规则·		
		#c6f5220 战争时间：争霸战开启时间：每周六晚20：00；争霸战结束时间：每周六晚21：30。#n
		#c6f5220 战争地图：整个战争将在中控中心地图进行，可通过时间城内帕金斯处进入。#n
		#c6f5220 战争规则：#n
		#c6f5220 		 1.只有据点占领公会可在中控中心地图中争夺建筑控制权；#n
		#c6f5220 		 2.在战争地图中，争夺占领、摧毁或成功守护城防建筑都可获得战争积分。#n
		#c6f5220 胜负条件：#n
		#c6f5220 		 在战争结束时，根据各公会战争积分判定胜负，积分排名第1公会，获得最终胜利，其他情况：#n
		#c6f5220 		 1.如防守方公会与其他公会分数相同，则防守方胜利；#n
		#c6f5220 		 2.如进攻方公会之间出现平分，则按照分数达到时间，靠前者获胜。#n
		#c6f5220 		 重要提示：在中控中心中，争夺3座终端控制器控制权可获得大量战争积分！#n
#cd44800·占领时间城公会奖励·			
		#c6f52201.占领公会会长每天可领取一次占领资金（大量银币）。
		2.所有当前公会成员，在时间城NPC处购买物品、维修装备，可享受一定折扣优惠。
		3.所有当前公会成员，在[急你所急航空]世界航线传送中，拥有更多的地图传送点。
		4.时间城城主（占领公会会长）可在时间城中心出塑造个人雕像。
		5.当前占领公会的公会名特殊化。
		6.特别注意：以上为主要特权与奖励。（连续占领时间城的时间越久，拥有的特权与奖励越多）。#n]],
						},	
			[2001] = { "一级据点开启",
			      [[
#cd44800据点简介：
	1级据点：[冰岛]、[银风雪原]、[流金沙漠]据点已开启。所有公会可在对应地图的[据点行政官]沙克处进行竞拍并争夺据点。
据点规则：
	竞拍：
		竞拍时间：每周一、三开启竞拍（第二天中午公布结果，晚上开启据点争夺战）
		竞拍花费：使用公会资金进行竞拍。
		竞拍目的：竞拍胜利公会，将获得参加据点战的权利。
	战争：
		战争时间：每周二、周四晚20：30-21：30分为据点战争夺时间。
		战争规则：战争以公会为单位，1对1形式在封闭的战争副本中进行。
		副本内共有四个资源争夺点，占领资源点后己方公会增长资源。
		战争副本内，PK无惩罚，不增加PK值。
	战争结束：战争时间内，最先获得20000资源点公会为胜利公会。
		如战争时间结束时，双方公会均未获得20000资源点，则资源点数多的一方获得战争的胜利。
		最终胜利公会为当前据点的占领公会，享受占领特权
		注：如双方获得资源相同，则判定防守方公会胜利。
据点奖励：
	会长每日可领取一次占领资金奖励（大量银币）
	公会成员每日可领取一次公会贡献度奖励
	公会成员每日可领取一次"光之荣耀"效果奖励
	在据点所在地图指定地点竖立个性公会旗帜等等奖励
	注：据点等级越高，可领取的奖励越丰厚#n]],
						},
			[2002] = { "二级据点开启",
			      [[
#cd44800据点简介：
	2级据点：[落日荒野]、[越雾雨林]据点已开启。所有公会可在对应地图的[据点行政官]沙克处进行竞拍并争夺据点。
据点规则：
	竞拍：
		竞拍时间：每周一、三开启竞拍（第二天中午公布结果，晚上开启据点争夺战）
		竞拍花费：使用公会资金进行竞拍。
		竞拍目的：竞拍胜利公会，将获得参加据点战的权利。
	战争：
		战争时间：每周二、周四晚20：30-21：30分为据点战争夺时间。
		战争规则：战争以公会为单位，1对1形式在封闭的战争副本中进行。
		副本内共有四个资源争夺点，占领资源点后己方公会增长资源。
		战争副本内，PK无惩罚，不增加PK值。
	战争结束：战争时间内，最先获得20000资源点公会为胜利公会。
		如战争时间结束时，双方公会均未获得20000资源点，则资源点数多的一方获得战争的胜利。
		最终胜利公会为当前据点的占领公会，享受占领特权
		注：如双方获得资源相同，则判定防守方公会胜利。
据点奖励：
	会长每日可领取一次占领资金奖励（大量银币）
	公会成员每日可领取一次公会贡献度奖励
	公会成员每日可领取一次"暗之荣耀"效果奖励
	在据点所在地图指定地点竖立个性公会旗帜等等奖励
	注：据点等级越高，可领取的奖励越丰厚#n]],
						},
			[2003] = { "三级据点开启",
			      [[
#cd44800据点简介：
	3级据点：[鸣沙陷地]据点已开启。所有公会可在对应地图的[据点行政官]沙克处进行竞拍并争夺据点。
据点规则：
	竞拍：
		竞拍时间：每周一、三开启竞拍（第二天中午公布结果，晚上开启据点争夺战）
		竞拍花费：使用公会资金进行竞拍。
		竞拍目的：竞拍胜利公会，将获得参加据点战的权利。
	战争：
		战争时间：每周二、周四晚20：30-21：30分为据点战争夺时间。
		战争规则：战争以公会为单位，1对1形式在封闭的战争副本中进行。
		副本内共有四个资源争夺点，占领资源点后己方公会增长资源。
		战争副本内，PK无惩罚，不增加PK值。
	战争结束：战争时间内，最先获得20000资源点公会为胜利公会。
		如战争时间结束时，双方公会均未获得20000资源点，则资源点数多的一方获得战争的胜利。
		最终胜利公会为当前据点的占领公会，享受占领特权
		注：如双方获得资源相同，则判定防守方公会胜利。
据点奖励：
	会长每日可领取一次占领资金奖励（大量银币）
	公会成员每日可领取一次公会贡献度奖励
	公会成员每日可领取一次"血之荣耀"效果奖励
	在据点所在地图指定地点竖立个性公会旗帜等等奖励
	注：据点等级越高，可领取的奖励越丰厚#n]],
						},
			
--			-- 44级等级封印提示
--			[10044] = { "解除封印（44级）",
--						[[
--						
--	服务器当前已解除44级封印，玩家们可以继续提升等级，体验更多、更丰富的游戏内容
--						]]
--						},
--			-- 44级等级封印提示
--			[10049] = { "解除封印（49级）",
--						[[
--						
--	服务器当前已解除49级封印，玩家们可以继续提升等级，体验更多、更丰富的游戏内容
--						]]
--						},
--			-- 44级等级封印提示
--			[10054] = { "解除封印（54级）",
--						[[
--						
--	服务器当前已解除54级封印，玩家们可以继续提升等级，体验更多、更丰富的游戏内容
--						]]
--						},
--			-- 44级等级封印提示
--			[10059] = { "解除封印（59级）",
--						[[
--						
--	服务器当前已解除59级封印，玩家们可以继续提升等级，体验更多、更丰富的游戏内容
--						]]
--						},
--			-- 44级等级封印提示
--			[10064] = { "解除封印（64级）",
--						[[
--						
--	服务器当前已解除64级封印，玩家们可以继续提升等级，体验更多、更丰富的游戏内容
--						]]
--						},
--			-- 44级等级封印提示
--			[10069] = { "解除封印（69级）",
--						[[
--						
--	服务器当前已解除69级封印，玩家们可以继续提升等级，体验更多、更丰富的游戏内容
--						]]
--						},
--			-- 44级等级封印提示
--			[10074] = { "解除封印（74级）",
--						[[
--						
--	服务器当前已解除74级封印，玩家们可以继续提升等级，体验更多、更丰富的游戏内容
--						]]
--						},
--						-- 49级等级封印提示
			[10061] = { "解除封印（61级）",
						[[
	服务器当前已解除61级封印，玩家们可以继续提升等级，体验更多、更丰富的游戏内容

						]]
						},
			[10062] = { "解除封印（62级）",
						[[
	服务器当前已解除62级封印，玩家们可以继续提升等级，体验更多、更丰富的游戏内容

						]]
						},
			[10063] = { "解除封印（63级）",
						[[
	服务器当前已解除63级封印，玩家们可以继续提升等级，体验更多、更丰富的游戏内容

						]]
						},
			[10064] = { "解除封印（64级）",
						[[
	服务器当前已解除64级封印，玩家们可以继续提升等级，体验更多、更丰富的游戏内容

						]]
						},
			[10065] = { "解除封印（65级）",
						[[
	服务器当前已解除65级封印，玩家们可以继续提升等级，体验更多、更丰富的游戏内容

						]]
						},
			[10066] = { "解除封印（66级）",
						[[
	服务器当前已解除66级封印，玩家们可以继续提升等级，体验更多、更丰富的游戏内容

						]]
						},
			[10067] = { "解除封印（67级）",
						[[
	服务器当前已解除67级封印，玩家们可以继续提升等级，体验更多、更丰富的游戏内容

						]]
						},
			[10068] = { "解除封印（68级）",
						[[
	服务器当前已解除68级封印，玩家们可以继续提升等级，体验更多、更丰富的游戏内容

						]]
						},
			[10069] = { "解除封印（69级）",
						[[
	服务器当前已解除69级封印，玩家们可以继续提升等级，体验更多、更丰富的游戏内容

						]]
						},
			[10070] = { "解除封印（70级）",
						[[
	服务器当前已解除70级封印，玩家们可以继续提升等级，体验更多、更丰富的游戏内容

						]]
						},
			[10071] = { "解除封印（71级）",
						[[
	服务器当前已解除71级封印，玩家们可以继续提升等级，体验更多、更丰富的游戏内容

						]]
						},
			[10072] = { "解除封印（72级）",
						[[
	服务器当前已解除72级封印，玩家们可以继续提升等级，体验更多、更丰富的游戏内容

						]]
						},
			[10073] = { "解除封印（73级）",
						[[
	服务器当前已解除73级封印，玩家们可以继续提升等级，体验更多、更丰富的游戏内容

						]]
						},
			[10074] = { "解除封印（74级）",
						[[
	服务器当前已解除74级封印，玩家们可以继续提升等级，体验更多、更丰富的游戏内容

						]]
						},
			[10075] = { "解除封印（75级）",
						[[
	服务器当前已解除75级封印，玩家们可以继续提升等级，体验更多、更丰富的游戏内容

						]]
						},
			[10076] = { "解除封印（76级）",
						[[
	服务器当前已解除76级封印，玩家们可以继续提升等级，体验更多、更丰富的游戏内容

						]]
						},
			[10077] = { "解除封印（77级）",
						[[
	服务器当前已解除77级封印，玩家们可以继续提升等级，体验更多、更丰富的游戏内容

						]]
						},
			[10078] = { "解除封印（78级）",
						[[
	服务器当前已解除78级封印，玩家们可以继续提升等级，体验更多、更丰富的游戏内容

						]]
						},
			[10079] = { "解除封印（79级）",
						[[
	服务器当前已解除79级封印，玩家们可以继续提升等级，体验更多、更丰富的游戏内容

						]]
						},
--
}

function SetMessageTips( nIdx )
	if t_Message[ nIdx ] ~= nil then
		local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
		t_centerXinShouBtnControl:ShowExtraBtn( { itemDefId = 0, Type = CENTER_XINSHOW_BTN_MSG, frame = "LevelUpTipFrame", 
												path =t_Message[nIdx].path ~= nil and t_Message[nIdx].path or "uires\\TuPianLei\\PiaoYiTiShiTuBiao\\!.tga", index = nIdx } );
	end
end

function CloseMessageTips( nIdx )
	if t_Message[ nIdx ] ~= nil then
		local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
		t_centerXinShouBtnControl:eraseMsgBtnByIndex( { itemDefId = 0, Type = CENTER_XINSHOW_BTN_MSG, frame = "LevelUpTipFrame", 
												path =t_Message[nIdx].path ~= nil and t_Message[nIdx].path or "uires\\TuPianLei\\PiaoYiTiShiTuBiao\\!.tga", index = nIdx } );
		t_centerXinShouBtnControl:updateCenterBtns();
	end
end
     
function ShowLevelUpMsgTip( nIdx )
	if t_Message[ nIdx ] ~= nil then
		LevelUpTipFrameFirstLineFont:SetText( t_Message[nIdx][1] );
		local rich = getglobal("LevelUpTipFrameSecondRich");
		local slider  = getglobal( "LevelUpTipFrameScrollBar" );  
		local sliderThumbTex = getglobal( "LevelUpTipFrameScrollBar".."ThumbTexture" );
		rich:SetLinkTextColor(10,110,0);
		rich:SetText( t_Message[nIdx][2],111,82,32 );
		local nViewLine	= rich:GetViewLines();
		local nAllLine	= rich:GetTextLines();
		rich:SetDispPos(rich:GetStartDispPos());
		slider:SetValue(0);
		slider:Hide();
		sliderThumbTex:Hide();
		if nAllLine - nViewLine > 0 then
			local nMaxValue = nAllLine - nViewLine; 
			slider:Show();
			slider:SetMaxValue( nMaxValue );
			slider:SetMinValue(0);
			slider:SetValueStep(1);
		end
		LevelUpTipFrame:Show();
	end
end

function WolrdCityBattleOpen()
	local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
	t_centerXinShouBtnControl:ShowExtraBtn( { itemDefId = 0, Type = CENTER_XINSHOW_BTN_MSG, frame = "LevelUpTipFrame", 
												path ="uires\\TuPianLei\\PiaoYiTiShiTuBiao\\!.tga", index = 1000 } )
end

local isClosed = true;

function SetClanOpenTouchTips()
	local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
	if t_centerXinShouBtnControl:getIndexByType( CENTER_CLAN_OPEN_TOUCH ) == 0 then
		t_centerXinShouBtnControl:ShowExtraBtn( { itemDefId = 0, Type = CENTER_CLAN_OPEN_TOUCH, frame = "ClanOpenTouchTipFrame", 
												path ="uires\\TuPianLei\\PiaoYiTiShiTuBiao\\12.tga", index = 0 } );
	end
	isClosed = false;
end

function ClanOpenTouchGameEnd()
	local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
	local nIdx = t_centerXinShouBtnControl:getIndexByType( CENTER_CLAN_OPEN_TOUCH );	
	if nIdx > 0 then
		t_centerXinShouBtnControl:eraseByIndex(  nIdx );
		t_centerXinShouBtnControl:updateCenterBtns();
	end
	if ClanOpenTouchTipFrame:IsShown() then
		ClanOpenTouchTipFrame:Hide();
	end
	isClosed = true;
end

local szClanOpenTouchAwaardDesc = [[
					
#c461e0a每次摸宝奖励经验：%d

#c461e0a挖宝有几率获得：
]]

function ClanOpenTouchTipFrame_OnLoad()
	local szText = [[
					
#c5546231、跟随探测器寻找宝藏
2、注意探测器提示信息发现宝藏
3、发现宝藏时可用挖掘器在探测器8米半径范围内挖宝
4、挖出大宝箱可以摸宝，经验丰厚
5、探测器死亡活动结束




	1、尽量多组织会员摸宝，摸宝越多，公会活动的额外奖励越多
	2、注意保护探测器的安全，提防宝藏守护者和外来人员的破坏#n]]

	InfoRich = getglobal( "ClanOpenTouchTipFrameSecondRichRole" )
	InfoRich:SetText( szText,111,82,32 );

	szText = string.format( szClanOpenTouchAwaardDesc,0 );
	InfoRich = getglobal( "ClanOpenTouchTipFrameSecondRichGift" )
	InfoRich:SetText( szText,111,82,32 );

	local tAwardItem = { 5233000,5233500,5050510,5060014,5510002 }
	for i=1,5 do
		local itemDef = getItemDef( tAwardItem[i] );
		local btn = getglobal( "ClanOpenTouchTipFrameAwardItemBtn" .. i );
		local ItemBackTex = getglobal( btn:GetName().."BackTexture");
		local ItemBoxTex = getglobal( btn:GetName().."BoxTexture");
		local UVAnimationTex = getglobal( btn:GetName().."UVAnimationTex");
		local ItemTex = getglobal( btn:GetName().."IconTexture");
		ItemBackTex:Show();
		ItemBoxTex:Show();
		local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH;
		end
		ItemTex:SetTexture( IconPath );
		SetEquipImportantEffectUV( itemDef, UVAnimationTex:GetName() );
		btn:SetClientUserData( 0,tAwardItem[i] )
	end

	this:RegisterEvent( "GE_ENTER_MAP" );
end

function ShowClanOpenTouchTipFrame()		
	ClanOpenTouchTipFrame:Show();
end

function ClanOpenTouchTipFrame_OnShow()
	local sText;
	if not isClosed then
	sText = [[
					
#c461e0a活动名称：宝藏#c554623(公会活动)
#c461e0a活动NPC：葛雷乔伊#c554623(公会城市)
#c461e0a活动地图：公会城市
#c461e0a人物等级：不限制
#c461e0a活动状态：#n   #cffffbe进行中#n]];
	ClanOpenTouchTipFrameRunning:SetGray( false );
	ClanOpenTouchTipFrameGoBtn:Enable();
	else
	sText = [[
					
#c461e0a活动名称：宝藏#c554623(公会活动)
#c461e0a活动NPC：葛雷乔伊#c554623(公会城市)
#c461e0a活动地图：公会城市
#c461e0a人物等级：不限制
#c461e0a活动状态：#n   #cffffbe未开启#n]];
	ClanOpenTouchTipFrameRunning:SetGray( true );
	ClanOpenTouchTipFrameGoBtn:Disable();
	--#L葛雷乔伊@@[304056,60000]
	end
	local InfoRich = getglobal( "ClanOpenTouchTipFrameSecondRichInfo" );
	InfoRich:SetText( sText,111,82,32 );

	this:SetPoint( "center", "UIClient", "center", 0, 0 );
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local levelupdef = getLevelUP( mainplayer:getLv() );
	local nExp = math.floor( levelupdef.ContributeExp / 100 )
	szText = string.format( szClanOpenTouchAwaardDesc,nExp );
	InfoRich = getglobal( "ClanOpenTouchTipFrameSecondRichGift" )
	InfoRich:SetText( szText,111,82,32 );	
end

function ClanOpenTouchTipFrame_OnHide()
end

function ClanOpenTouchTipFrameAwardItemBtn_OnEnter()
	local itemId = this:GetClientUserData( 0 )
	ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, getItemDef(itemId) )
end

local bClanCityMapAutoMove = false;
function ClanOpenTouchTipFrameGoBtn_OnClick()
	
	local ClanViewer = GameClanManager:getClanViewer();
	if MapShower:getRealMapId() == CLAN_CITY_MAP_ID and GameClanManager:getCurClanCityId() == ClanViewer.m_clanInfo.m_uClanId then
		-- 走过去
		MapShower:GetMaxMapContal():linkMoveto( MapShower:getRealMapId(), 12700, 29200 );
	else		
		local nCurMapID;
		local MaxMapControl	= MapShower:GetMaxMapContal();
		if MaxMapControl:getMapID() == 0 then
			nCurMapID = MapShower:getRealMapId();
		else
			nCurMapID = MaxMapControl:getMapID();
		end
		if nCurMapID >= MIN_COPY_ID then
			ShowMidTips( "当前副本不能传送" );
			return;
		end
		 --传送
		 GameClanManager:clan_Opt_EnterCity( ClanViewer.m_clanInfo.m_uClanId )
		 bClanCityMapAutoMove = true;
	end
end

function ClanOpenTouchTipFrame_OnEvent()
	if arg1 == "GE_ENTER_MAP" then
		if bClanCityMapAutoMove then
			bClanCityMapAutoMove = false;
			MapShower:GetMaxMapContal():linkMoveto( MapShower:getRealMapId(), 12700, 29200 );
		end
	end
end
-------------------------------------------------公会-情报攻防战--------------------------------------------
local IntellingenceInfo = {
	[1] = [[#c461e0a	活动名称：情报攻防战#c554623(公会活动)
#c461e0a	活动NPC：情报官#c554623(公会城市)
#c461e0a	活动地图：公会城市
#c461e0a	人物等级：20级]],
	[2] = [[#c461e0a	大量经验值、绑银	
#c461e0a	大量公会贡献、安定度]],
	[3] = [[#c461e0a	●夺取情报任务：	
#c461e0a	1.前往目标公会城市，从情报兵身上获取3份情报。
#c461e0a	2.击杀“防”守的玩家，也有机会获取情报。
#c461e0a	3.死亡不会掉落已经收集的情报，在时间城复活任务也不会失败。

#c461e0a	●情报拦截任务：
#c461e0a	1.从其它公会成员的手中夺回5份丢失的情报。
#c461e0a	2.被“攻”方玩家击杀不会掉落已经收集的情报！
#c461e0a	3.击杀前来夺取情报的玩家可以获得奖励。]],
}
local MAX_CITY_CHOICE_NUM = 8;
local MAX_CITYPOS_CHOICE_NUM = 8;
function setClanSPYTips()
	local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
	if t_centerXinShouBtnControl:getIndexByType( CENTER_CLAN_SPY ) == 0 then
		t_centerXinShouBtnControl:ShowExtraBtn( { itemDefId = 0, Type = CENTER_CLAN_SPY, frame = "ClanIntellingenceFrame", 
												path ="uires\\TuPianLei\\PiaoYiTiShiTuBiao\\12.tga", index = 0 } );
	end
end
function ClanIntellingenceFrame_onShow()
	ClanIntellingenceFrameSecondRichInfo:SetText( IntellingenceInfo[1], 255, 255, 255 );
	ClanIntellingenceFrameSecondRichGift:SetText( IntellingenceInfo[2], 255, 255, 255 );
	ClanIntellingenceFrameSecondRichRole:SetText( IntellingenceInfo[3], 255, 255, 255 );
end
function ClanIntellingenceFrameEnterBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer:getLv() < CLAN_SPY_LEVEL_LIMIT_MIN then
		ShowMidTips( "人物等级未达到活动要求，暂时无法参加" );
		return;
	end
	local nCurMapID;
	local MaxMapControl	= MapShower:GetMaxMapContal();
	if MaxMapControl:getMapID() == 0 then
		nCurMapID = MapShower:getRealMapId();
	else
		nCurMapID = MaxMapControl:getMapID();
	end
	if nCurMapID >= MIN_COPY_ID then
		ShowMidTips( "您当前处于副本中或已在公会城市,无法传送" );
		return;
	end
	GameClanManager:Clan_Opt_GoToClanIntellingenceNPC();
end	
--公会城市选择相关
local t_AvailableCityList = {
	[1] = {},
	[2] = {},
	[3] = {},
	[4] = {},
	[5] = {},
	[6] = {},
	[7] = {},
	[8] = {},
}
local SPY_MAX_CLAN_CITY = 8;
local Chose_City_ID = 0;
--公会地点选择相关
local SPY_MAX_CLAN_POS = 8;
local Chose_Pos_ID = 0;
local CLAN_SPY_TASK_ID = 3681;
function ChoseBtn_onClick()
	if this:GetParent() == "ChoseClanCityFrame" then
		for i = 1, MAX_CITY_CHOICE_NUM do
			local btn = getglobal( this:GetParent().."Choice"..i )
			btn:DisChecked();
		end
		Chose_City_ID = this:GetClientUserData( 0 );
	elseif this:GetParent() == "ChoseCityPosFrame" then
		for i = 1, MAX_CITYPOS_CHOICE_NUM do
			local btn = getglobal( this:GetParent().."Choice"..i )
			btn:DisChecked();
		end
		Chose_Pos_ID = this:GetClientID();
	end
	this:Checked();
end
function OpenChoseClanCityFrame( npcId )
	ChoseClanCityFrame:SetClientUserData( 0, npcId );
	ChoseClanCityFrame:Show();
end
function ChoseClanCityFrame_OnShow()
	if ChoseCityPosFrame:IsShown() then
		ChoseCityPosFrame:Hide();
	end
	local npcid = this:GetClientUserData( 0 );
	util.testNpcDialogDistance( npcid, ChoseClanCityFrame );
	for i = 1 , SPY_MAX_CLAN_CITY do
		local btn = getglobal( "ChoseClanCityFrameChoice"..i );
		btn:SetClientUserData( 0, 0 );
		btn:DisChecked();
		local btnShow = getglobal( btn:GetName().."Text" );
		btnShow:SetText( "" );
	end
	local nClanTotalNum	= GameClanManager:getClanTotalNum();
	--设置可潜入的公会列表
	local t_CanAttackClanList = {};
	local SetClanNum = 0;
	while nClanTotalNum > 0 do
		local OneClanInfo = GameClanManager:getOneClanInfoByIndex( nClanTotalNum - 1 );
		local clanInfo = OneClanInfo.m_ClanInfo;
		if clanInfo.m_Flag % 2 == 1 then
			local MyClanViewer = GameClanManager:getClanViewer();
			local MyClanInfo = MyClanViewer.m_clanInfo;
			if MyClanInfo.m_uClanId ~= clanInfo.m_uClanId then
				SetClanNum = SetClanNum + 1;
				table.insert( t_CanAttackClanList, clanInfo );
				if SetClanNum > 8 then return end
			end
		end
		nClanTotalNum = nClanTotalNum -1;
	end
	--按照繁荣度为公会排序，从高到低
	table.sort( t_CanAttackClanList, function ( a, b )
				return a.m_nActiveValue > b.m_nActiveValue;
	end )
	for i = 1 , SetClanNum do
		local clanInfo = t_CanAttackClanList[i];
		local btn = getglobal( "ChoseClanCityFrameChoice"..i );
		btn:SetClientUserData( 0, clanInfo.m_uClanId );
		local btnShow = getglobal( btn:GetName().."Text" );
		btnShow:SetText( clanInfo.m_szClanName );
	end
	--默认选择第一个
	local btn = getglobal( "ChoseClanCityFrameChoice"..1 )
	btn:Checked();
	Chose_City_ID = btn:GetClientUserData( 0 );
end
function ChoseClanCityFrame_OnUpdate()
	if not Quest:isQuestRun( CLAN_SPY_TASK_ID ) then
		this:Hide();
	end
end
function ChoseClanCityFrameOKBtn_onClick()
	if Chose_City_ID > 0 then
		ChoseClanCityFrame:Hide();
		ChoseCityPosFrame:Show();
	else
		ShowMidTips( "请选择公会城市" );
	end
end
function ChoseClanCityFrameCancleBtn_onClick()
	ChoseClanCityFrame:Hide();
end
function ChoseCityPosFrame_OnShow()
	if ChoseClanCityFrame:IsShown() then
		ChoseClanCityFrame:Hide();
	end
	local npcid = ChoseClanCityFrame:GetClientUserData( 0 );
	util.testNpcDialogDistance( npcid, ChoseCityPosFrame );
	Chose_Pos_ID = 0;
	for i = 1, SPY_MAX_CLAN_POS do
		local btn = getglobal( this:GetName().."Choice"..i );	
		btn:DisChecked();
		local ClanSpyTransDef = GameClanManager:getClanSpyTransDef( i );
		local btnShow = getglobal( this:GetName().."Choice"..i.."Text" );
		btnShow:SetText( ClanSpyTransDef.Name );
	end
	local btn = getglobal( this:GetName().."Choice1" )
	btn:Checked();
	Chose_Pos_ID = btn:GetClientID();
end
function ChoseCityPosFrame_OnUpdate()
	if not Quest:isQuestRun( CLAN_SPY_TASK_ID ) then
		this:Hide();
	end
end
function ChoseCityPosFrameOKBtn_onClick()
	if Chose_Pos_ID > 0 and Chose_Pos_ID < 9 then
		ChoseCityPosFrame:Hide();
		GameClanManager:Clan_Opt_GoToClanCityPos( Chose_City_ID, Chose_Pos_ID );
	else
		ShowMidTips( "请选择公会城市地点" );
	end
end
function ChoseCityPosFrameCancleBtn_onClick()
	ChoseCityPosFrame:Hide();
end
-----------------------------------------------公会活动-守护公会枢纽----------------------------------------------------
local ClanMachinePworldInfo = {
	[1] = [[#c461e0a	活动名称：守护公会枢纽#c554623(公会活动)
#c461e0a	活动NPC：克利斯提尼#L#c00a400(公会城市)@@[303006,1000]#n#c554623
#c461e0a	活动地图：公会枢纽大厅
#c461e0a	联合模式：%s公会开启]],
	[2] = [[#c461e0a	每波奖励#G(开启活动的公会可获得)#n
	
	
	
#c461e0a	敌兵掉落



#c00a400	（击杀敌兵所有人可获得经验值）]],
	[3] = [[#c461e0a	●成功守护：歼灭敌方30波袭击
#c461e0a	●守护失败：公会枢纽被敌方摧毁

#c461e0a	●普通模式：已方公会成员可参加
#c461e0a	●联合模式：所有公会成员可参加

#c461e0a	取胜窍门：
#c461e0a	1.使用蒸汽机械可对敌方造成可观伤害
#c461e0a	2.[公会枢纽大厅]提供炮弹出售
#c461e0a	3.[公会枢纽大厅]提供机械强化
#c461e0a	4.[公会枢纽大厅]提供机甲兽维修





#c0000ff	友情提示：
#c0000ff			当天只能参加1次[守护公会枢纽]
]],
}
--计时小面板数据记录
local newStarTime = {}
local CurWaveNum = {}
local CurWaveTimer = {}
local CurRaidTimer = {}

function SetClanMachinePworld()
	local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
	if t_centerXinShouBtnControl:getIndexByType( CENTER_CLAN_MACHINE_PWORLD ) == 0 then
		t_centerXinShouBtnControl:ShowExtraBtn( { itemDefId = 0, Type = CENTER_CLAN_MACHINE_PWORLD, frame = "ClanMachinePworldFrame", 
												path ="uires\\TuPianLei\\PiaoYiTiShiTuBiao\\12.tga", index = 0 } );
	end
end
function ClanMachinePworldFrame_OnShow()
	local mainplayer = ActorMgr:getMainPlayer()
	if not mainplayer then return end;
	local clanName = "暂无"
	if mainplayer:getUnionMachinePworldClanID() ~= 0 then
		clanName = "#c00a400"..GameClanManager:clanGetClanNameByID( mainplayer:getUnionMachinePworldClanID() ).."#n#c461e0a"
	end
	local szInfo = string.format( ClanMachinePworldInfo[1], clanName )
	ClanMachinePworldFrameSecondRichInfo:SetText( szInfo, 255, 255, 255 );
	ClanMachinePworldFrameSecondRichGift:SetText( ClanMachinePworldInfo[2], 255, 255, 255 );
	ClanMachinePworldFrameSecondRichRole:SetText( ClanMachinePworldInfo[3], 255, 255, 255 );
end
function SetClanMachineBtnState()
	--更新下自己公会的守护活动和其他公会是否开启了联合模式
	local mainplayer = ActorMgr:getMainPlayer()
	--有公会开启了守护枢纽-普通模式，激活对应按钮
	if not ClanMachinePworldFrameEnterBtn:IsEnable() and mainplayer:getNormalMachinePworldClanID() ~= 0 then
		ClanMachinePworldFrameEnterBtn:Enable()
	end
	if ClanMachinePworldFrameEnterBtn:IsEnable() and mainplayer:getNormalMachinePworldClanID() == 0 then
		ClanMachinePworldFrameEnterBtn:Disable()
	end
	--有公会开启了守护枢纽-联合模式，激活对应按钮
	if not ClanMachinePworldFrameEnterUnionBtn:IsEnable() and mainplayer:getUnionMachinePworldClanID() ~= 0 then
		ClanMachinePworldFrameEnterUnionBtn:Enable()
	end
	if ClanMachinePworldFrameEnterUnionBtn:IsEnable() and mainplayer:getUnionMachinePworldClanID() == 0 then
		ClanMachinePworldFrameEnterUnionBtn:Disable()
	end
end
function ClanMachinePworldFrame_OnUpdate()
	local mainplayer = ActorMgr:getMainPlayer()
	local clanName = "暂无"
	if mainplayer:getUnionMachinePworldClanID() ~= 0 then
		clanName = "#c00a400"..GameClanManager:clanGetClanNameByID( mainplayer:getUnionMachinePworldClanID() ).."#n#c461e0a"
	end
	local szInfo = string.format( ClanMachinePworldInfo[1], clanName )
	ClanMachinePworldFrameSecondRichInfo:SetText( szInfo, 255, 255, 255 );
end
--因为新的联合模式，助手上的显示信息，使用其他的函数来显示波数
function ClanMachinePworldTimeShow( nPworldId )
	local MainPlayer = ActorMgr:getMainPlayer()
	if MainPlayer and MainPlayer:getMapRealId() == nPworldId * 100 then
		ClanMachinePworldTimeShowFrameNormalAttack:SetText( "第"..CurWaveNum[nPworldId].."波："..CurWaveTimer[nPworldId].."秒" );
		ClanMachinePworldTimeShowFrameRaiderAattack:SetText( "奇袭："..CurRaidTimer[nPworldId].."秒" );
	end
	--ClanMachinePworldFrameWavesInfo:SetText("敌方袭击："..CurWaveNum[nPworldId].."/30波");
end
--因为新的联合模式，助手上的显示信息使用本函数显示波数
function SetClanMachineWavesInfo( nflag, nWave )
	--联合模式
	if nflag == 1 then
		ClanMachinePworldFrameUnionWavesInfo:SetText("联合模式："..nWave.."/30波");
	else
		ClanMachinePworldFrameWavesInfo:SetText("普通模式："..nWave.."/30波");
	end
end
function ClanMachinePworldFrameEnterBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer:getNormalMachinePworldClanID() == 0 then
		ShowMidTips( "公会暂未开启【守护公会枢纽-普通模式】，暂时无法参加" )
		return
	end
	local nCurMapID;
	local MaxMapControl	= MapShower:GetMaxMapContal();
	if MaxMapControl:getMapID() == 0 then
		nCurMapID = MapShower:getRealMapId();
	else
		nCurMapID = MaxMapControl:getMapID();
	end
	if nCurMapID >= MIN_COPY_ID then
		ShowMidTips( "您当前处于副本中或已在公会城市,无法传送" );
		return;
	end
	GameClanManager:Clan_Opt_EnterClanMachinePworld( mainplayer:getClanID() );
end
function ClanMachinePworldFrameEnterUnionBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer:getUnionMachinePworldClanID() == 0 then
		ShowMidTips( "当前无公会开启【守护公会枢纽-联合模式】，暂时无法参加" )
		return
	end
	local nCurMapID;
	local MaxMapControl	= MapShower:GetMaxMapContal();
	if MaxMapControl:getMapID() == 0 then
		nCurMapID = MapShower:getRealMapId();
	else
		nCurMapID = MaxMapControl:getMapID();
	end
	if nCurMapID >= MIN_COPY_ID then
		ShowMidTips( "您当前处于副本中或已在公会城市,无法传送" );
		return;
	end
	GameClanManager:Clan_Opt_EnterClanMachinePworld( mainplayer:getUnionMachinePworldClanID() );
end
local t_ClanMachinePworldAwardItemIDList = { 5510003, 5510006, 5510004, 5511046, 5234000, 5051101, 5510000, }
function ClanMachinePworldFrameAwardItemBtn_OnShow()
	local itemId				= t_ClanMachinePworldAwardItemIDList[this:GetClientID()]
	if itemId == nil then return end
	local itemdef				= getItemDef( itemId );
	if itemdef == nil then return end
	local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
	-- 如果没找到物品的图标则使用默认图标显示
	if not IsInExistence( IconPath ) then
		IconPath = DEFAULT_ITEM_PATH;
	end
	local IconTexture = getglobal( this:GetName().."IconTexture" )
	IconTexture:SetTexture(IconPath)
end
function ClanMachinePworldFrameAwardItemBtn_OnEnter()
	local itemId				= t_ClanMachinePworldAwardItemIDList[this:GetClientID()]
	if itemId == nil then return end
	local itemdef				= getItemDef( itemId );
	if itemdef == nil then return end
	ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, itemdef )
end
---------------------------------------------守护公会枢纽-计时小面板--------------------------------------------------
local CLAN_MACHINE_PWORLD_MAP_ID = 60200 --公会枢纽地图ID
local THE_DREAM_PWORLD_MAP_ID = 21900 --筑梦空间地图ID
local ITERATION_PWORLD_MAP_ID = 25000 --文字活动地踢ID
local nIterationType = 0;
local nIterationNum = 0;
function ClanMachinePworldTimeShowFrame_OnUpdate()
	local MainPlayer = ActorMgr:getMainPlayer()
	local idx = MainPlayer and MainPlayer:getMapRealId() / 100 or 0;
	if idx == 0 or newStarTime[idx] == nil then return end;

	if getServerTime().sec - newStarTime[idx] >= 1 then
		CurWaveTimer[idx] = CurWaveTimer[idx] - 1
		if CurWaveTimer[idx] < 0 then
			CurWaveTimer[idx] = 0;
		end
		CurRaidTimer[idx] = CurRaidTimer[idx] - 1
		if CurRaidTimer[idx] < 0 then
			CurRaidTimer[idx] = 0;
		end
		if idx == ITERATION_PWORLD_MAP_ID / 100 then
			if nIterationType ~= 2 then
				ClanMachinePworldTimeShowFrameNormalAttack:SetText( "第"..nIterationNum.."题："..CurWaveTimer[idx].."秒" );
				if nIterationType == 0 then
					ClanMachinePworldTimeShowFrameRaiderAattack:SetText( "词语准备中···" );
				elseif nIterationType == 1 then
					ClanMachinePworldTimeShowFrameRaiderAattack:SetText( "请答题···" );
				end
			end
		else
			ClanMachinePworldTimeShowFrameNormalAttack:SetText( "第"..CurWaveNum[idx].."波："..CurWaveTimer[idx].."秒" );
			if idx == CLAN_MACHINE_PWORLD_MAP_ID / 100 then
				ClanMachinePworldTimeShowFrameRaiderAattack:SetText( "奇袭："..CurRaidTimer[idx].."秒" );
			elseif idx == THE_DREAM_PWORLD_MAP_ID /100 then
				ClanMachinePworldTimeShowFrameRaiderAattack:SetText( "怪物来袭···" );
			end
		end
		newStarTime[idx] = getServerTime().sec;
	end
end
------------------------------------------------据点战下方漂移泡泡-----------------------------------------------------
function StrongPointActive( lv )
	local nIdx = 2000 + lv;
	if t_Message[ nIdx ] ~= nil then
		local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
		t_centerXinShouBtnControl:ShowExtraBtn( { itemDefId = 0, Type = CENTER_XINSHOW_BTN_MSG, frame = "LevelUpTipFrame", 
												path ="uires\\TuPianLei\\PiaoYiTiShiTuBiao\\!.tga", index = nIdx } );
	end
end
--------------------------------------------------筑梦空间--------------------------------------------------------------
function TheDreamTimeShow( nPworldId )
	local MainPlayer = ActorMgr:getMainPlayer()
	if MainPlayer and MainPlayer:getMapRealId() == nPworldId * 100 then
		ClanMachinePworldTimeShowFrameNormalAttack:SetText( "第"..CurWaveNum[nPworldId].."波："..CurWaveTimer[nPworldId].."秒" );
		ClanMachinePworldTimeShowFrameRaiderAattack:SetText( "怪物来袭···" );
	end
end
--------------------------------------------------文字活动--------------------------------------------------------------
local isStarted = false;
function isIterationStarted()
	return isStarted;
end

function setIterationStarted(bStart)
	isStarted = bStart;
end

function UpdateIterationTimeShow( type, leftTime, num )
	local MainPlayer = ActorMgr:getMainPlayer()
	local index = MainPlayer and MainPlayer:getMapRealId() / 100 or 0;
	nIterationType = type;
	nIterationNum = num;
	CurWaveTimer[index] = leftTime;
	CurRaidTimer[index] = CurWaveTimer[index];
	newStarTime[index] = getServerTime().sec;
	if MainPlayer then --and MainPlayer:getMapRealId() == nPworldId * 100 then
		if type ~= 2 then
			ClanMachinePworldTimeShowFrameNormalAttack:SetText( "第"..num.."题："..CurWaveTimer[index].."秒" );
			if type == 0 then
				ClanMachinePworldTimeShowFrameRaiderAattack:SetText( "词语准备中···" );
			elseif type == 1 then
				ClanMachinePworldTimeShowFrameRaiderAattack:SetText( "请答题···" );
			end
		end
	end
	isStarted = true
end

function UpdatePworldWaveInfo( nWaveNum, nWaveTimer, nRaidTimer, nPworldId )
	if nWaveNum <= 0 or nWaveTimer < 0 then return end
	CurWaveNum[nPworldId] = nWaveNum
	CurWaveTimer[nPworldId] = nWaveTimer
	CurRaidTimer[nPworldId] = nRaidTimer >= 0 and nRaidTimer or 0
	newStarTime[nPworldId] = getServerTime().sec;
	if nPworldId == CLAN_MACHINE_PWORLD_MAP_ID / 100 then
		ClanMachinePworldTimeShow( nPworldId );
	elseif nPworldId == THE_DREAM_PWORLD_MAP_ID /100 then
		TheDreamTimeShow( nPworldId );
	end
end

------------------------------------------------公会商运 部分-----------------------------------------------------------
local t_ClanDartDeliveryInfo = {
					[1] = [[
#c461e0a活动名称：公会商运#c554623(公会活动)
#c461e0a活动NPC：#L亨利@@[302041,1000]#n#c554623(时间城)
#c461e0a人物等级：≥55级]],
					[2] = [[
					
#c461e0a●完成【#c0a6400运送货车#n#c461e0a】任务，可额外获得大量经验值

#c461e0a●运送的货车档次越高，获得的奖励越多


#c0a6400温馨提示：
#c0a6400		只要在活动期间接取【运送货车】任务，活动
#c0a6400结束后完成同样可获得活动奖励。
						]],
					 			}
local AWARD_ITEM_ID = 5510000
local nDartTime 	= 0;			--活动结束时间
local CLAN_DART_ACTIVE_ID = 55
function SetClanDartInfo( nEndTime )
	nDartTime = nEndTime;
	local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
	if nEndTime > 0 then 	
		if t_centerXinShouBtnControl:getIndexByType( CLAN_DART_DELIVERY ) == 0 then
			t_centerXinShouBtnControl:ShowExtraBtn( { itemDefId = 0, Type = CLAN_DART_DELIVERY, frame = "ClanDartDeliveryFrame", 
													path ="uires\\TuPianLei\\PiaoYiTiShiTuBiao\\12.tga", index = 0,nActiveID = CLAN_DART_ACTIVE_ID } );
		end
	--	t_centerXinShouBtnControl:updateCenterBtns();
	else 
		CloseCenterClanDartButton();     -- 关闭新手引导按钮
	end
end
-- 关闭新手引导按钮
function CloseCenterClanDartButton()
	local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
	local nIdx = t_centerXinShouBtnControl:getIndexByType( CLAN_DART_DELIVERY );
	if nIdx > 0 then	
		t_centerXinShouBtnControl:eraseByIndex( nIdx );
		t_centerXinShouBtnControl:updateCenterBtns();
	end
	if ClanDartDeliveryFrame:IsShown() then 
		ClanDartDeliveryFrame:Hide();
	end
end

function ClanDartDeliveryFrame_OnShow()
	local curTime 	= getServerTime().sec;
	local nTme		= nDartTime - curTime; 
	local font		= getglobal("ClanDartDeliveryFrameEndTimeFont");
	
	if nTme > 0 then 
		local timeFont = SetDartEndTimeText( nTme );
		font:SetText( "剩余时间："..timeFont );
	else 	
		font:SetText( "剩余时间：00:00" );
	end
end

function SetDartEndTimeText( nTime )
	local szText = "";
	local hour 		=  math.floor( nTime / 3600 );
	local minute 	=  math.floor( nTime / 60 ) ;
	local second 	= nTime;
	if hour > 0 then	
		minute = math.floor( ( nTime - hour * 3600 )/60 );
		second = nTime - hour * 3600 - minute* 60;
		local hourFont = SetClockNum( hour );
		local minFont  = SetClockNum( minute ); 
		local secFont  = SetClockNum( second );	
		szText = szText..hourFont..":"..minFont..":"..secFont;
	elseif minute > 0 then 
		second = nTime - minute* 60;
		local minFont  = SetClockNum( minute ); 
		local secFont  = SetClockNum( second );	
		szText = szText..minFont..":"..secFont;
	else 
		local secFont  = SetClockNum( second );	
		szText = szText.."00:"..secFont;
	end
	return szText;
end

function ClanDartDeliveryFrame_OnLoad()
	this:setUpdateTime(1);
	this:RegisterEvent("GE_CLAN_DISABLEFRAME");
	ClanDartDeliveryFrameLeftRichInfo:SetLinkTextColor( 10, 100, 0);
	ClanDartDeliveryFrameLeftRichInfo:SetText( t_ClanDartDeliveryInfo[1],255,255,255 )
	ClanDartDeliveryFrameRightRichInfo:SetText( t_ClanDartDeliveryInfo[2],255,255,255 )
	local itemdef = getItemDef( AWARD_ITEM_ID );
	local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
	-- 如果没找到物品的图标则使用默认图标显示
	if not IsInExistence( IconPath ) then
		IconPath = DEFAULT_ITEM_PATH;
	end
	ClanDartDeliveryFrameAwardItemBtnIconTexture:SetTexture(IconPath)
end

function ClanDartDeliveryFrame_OnEvent()
	if arg1 == "GE_CLAN_DISABLEFRAME" then
		CloseCenterClanDartButton();
		nDartTime = 0;
	end
end

function ClanDartDeliveryFrame_OnUpdate()
	local curTime 	= getServerTime().sec;
	local nTme		= nDartTime - curTime; 
	local font		= getglobal("ClanDartDeliveryFrameEndTimeFont");
	if nTme > 0 then 
		local timeFont = SetDartEndTimeText( nTme );
		font:SetText( "剩余时间："..timeFont );
	else 	
		font:SetText( "剩余时间：00:00" );
	end
end

-- 马上参加
function ClanDartDeliveryFrameOkBtn_OnClick()
	if CanNotTransportOtherMap() then 
		return;
	end
	
	local frame = getglobal("ClanDartDeliveryFrame");
	GamePromotion:RequestTransportToNPC( frame:GetClientUserData( 0 ) );
	if ClanDartDeliveryFrame:IsShown() then 
		ClanDartDeliveryFrame:Hide();
	end
--	CloseCenterClanDartButton();
end
function CanNotTransportOtherMap()
	local mainPlayer = ActorMgr:getMainPlayer();
	if  mainPlayer == nil then
		return true;
	end
	if mainPlayer:isInStall() then
		ShowMidTips("摆摊状态中，暂时无法传送！");
		return true;
	end
	if mainPlayer:isInFight() then
		ShowMidTips("战斗状态中，暂时无法传送！");
		return true;
	end
	if TradeFrame:IsShown() then
		ShowMidTips( "交易状态中，暂时无法传送" );
		return true;
	end
	if mainPlayer:isDead() then 
		ShowMidTips( "死亡状态中，暂时无法传送" );
		return true;
	end 
	local 	rider		= mainPlayer:getRider();
	local	riderId		= rider:getRideItem().RideID;
	local	riderLv		= rider:getRideItem().nLevel;
	local	rideDef 	= getRideDef( riderId, riderLv );
	if mainPlayer:isInRide() and rideDef ~= nil and rideDef.RideOnNum >= 1 then
		ShowMidTips("多人坐骑状态下，暂时无法传送！");
		return true;
	end
	if mainPlayer:isOnMonsterMachine() then 
		ShowMidTips("使用机械中，暂时无法传送");
		return true;
	end
	local nCurMapID;
	local MaxMapControl	= MapShower:GetMaxMapContal();
	if MaxMapControl:getMapID() == 0 then
		nCurMapID = MapShower:getRealMapId();
	else
		nCurMapID = MaxMapControl:getMapID();
	end
	if nCurMapID >= MIN_COPY_ID then
		ShowMidTips( "当前副本不能传送" );
		return true;
	end
	return false;
end
function ClanDartDeliveryFrameAwardItemBtn_OnEnter()
	local itemdef = getItemDef( AWARD_ITEM_ID );
	if itemdef == nil then return end
	ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, itemdef )
end