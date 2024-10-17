function BackToGame_OnClick()
	SystemSelectFrame:Hide();
end

function GameEffectSetting_OnClick()
	local t_mutexFrames = { "SystemSettingFrame", "GameSettingFrame", "MouseSettingFrame" };
	local curShowFrame;
	for _, name in ipairs( t_mutexFrames ) do
		local frame = getglobal( name );
		if frame:IsShown() then
			curShowFrame = frame;
			break;
		end
	end

	for _, name in ipairs( t_mutexFrames ) do
		local frame = getglobal( name );
		frame:Hide();
	end

	if curShowFrame ~= nil then
		SystemSettingFrame:SetPoint( "topleft", curShowFrame:GetName(), "topleft", 0, 0 );
	else
		SystemSettingFrame:SetPoint( "center", "UIClient", "center", 0, 0 );
	end

	SystemSettingFrame:Show();
	SystemSelectFrame:Hide();
end

function GameSetting_OnClick()
	local t_mutexFrames = { "SystemSettingFrame", "GameSettingFrame", "MouseSettingFrame" };
	local curShowFrame;
	for _, name in ipairs( t_mutexFrames ) do
		local frame = getglobal( name );
		if frame:IsShown() then
			curShowFrame = frame;
			break;
		end
	end

	for _, name in ipairs( t_mutexFrames ) do
		local frame = getglobal( name );
		frame:Hide();
	end

	if curShowFrame ~= nil then
		GameSettingFrame:SetPoint( "topleft", curShowFrame:GetName(), "topleft", 0, 0 );
	else
		GameSettingFrame:SetPoint( "center", "UIClient", "center", 0, 0 );
	end

	GameSettingFrame:Show();
	SystemSelectFrame:Hide();
end

function GameHelpInfo_OnClick()
end

function PreLogOutAction()
	CheckRollBackPetXinShouBtn();
	CheckLvXinShouBtn();
	--print( "PreLogOutAction--61" );
	if TradeFrame:IsShown() then
		--print( "PreLogOutAction--拒绝交易" );
		GameExchange:cancelExchange( ROLE_LOGOUT_REJECT );
		ClearTradeFrame();
	end
	
	RejectAllExchangeInvite( ROLE_LOGOUT_REJECT );
	
	IgnoreAllAddFriendRequests();
	IgnoreAllTeamRequests();
end

function BackToRoleSelect_OnClick()
	SaveLastSelectPetInfo();
	-- 玩家选择返回角色选择界面时，停止移动状态，同时打断所有吟唱读条。 
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer ~= nil and not MainPlayer:getEquip():isTmpPatronid() then
		MainPlayer:breakPreAction();
		MainPlayer:breakBTypePre();
	end
	if AutoAssist:getAutoAssistBegin() then
		AutoAssistFrame_OnClick();
	end
	bOpenContainerFlag	= true;
	bCharactorFrameOpenFlag	= true;
	SkillMgr:onStop();
	PreLogOutAction();
	LoginMgr:requestRoleList();
	SystemSelectFrame:Hide();
	ViewLockFrame:Show();
	ShowChangeRoleFrame:Show();
	MessageBoxFrame:Hide();
end

function SelectQuitGame_OnClick()
	SystemSelectFrame:Hide();
	--QuitGameFrame:Show();	
	WarnLeaveGame();
end

function Startroom_OnClick()
	SystemSelectFrame:Hide();
	--QuitGameFrame:Show();	
	MessageBox("提示","是否要离开游戏启动引力空间？");
	MessageBoxFrame:SetClientString("引力空间");
end

function showQuitGameFrame()
	if getGameState() ~= STATE_INWORLD then 
		GameMgr:QuitGame();
	else
		if ViewLockFrame:IsShown() or CameraCoverFrame:IsShown() then
		else
			SetSystemSelectFrameActiveInfo()
			SystemSelectFrame:Show();
			--QuitGameFrame:Show();
		end
	end
end

function WarnLeaveGame()
	MessageBox("提示","是否要离开游戏？");	
	MessageBoxFrame:SetClientString("离开游戏");
	if AutoAssist:getAutoAssistBegin() then
		AutoAssistFrame_OnClick();
	end
	SaveLastSelectPetInfo();
end

function SimpleClient_OnClick()
	SystemSelectFrame:Hide();
	MessageBox("退出游戏","您确定要退出游戏，开始挂机吗？");
end

----------------------------------------退出界面活动经验部分---------------------------------------
local t_AwardTypeTexUV = {
	--海量经验
	[1] = { x = 810, y = 284, w = 55, h = 16, },
	--大量经验
	[2] = { x = 866, y = 284, w = 55, h = 16, },
	--福利道具
	[3] = { x = 922, y = 284, w = 55, h = 16, },
	--极品装备
	[4] = { x = 907, y = 203, w = 55, h = 16, },
	--大量绑银
	[5] = { x = 907, y = 220, w = 55, h = 16, },
}
local MAX_EVERY_DAY_ACTIVE = 4
local MAX_EVERY_DAY_ACTIVES_NUM = 10
local MAX_CLAN_AWARD_ACTIVE = 2
local	t_EveryDayActiveInfo = {
		{ name = "永夜镇\t\t\t\t\t", pworldID = 112, PromotionID = 8, taskID = 0, awardType = 4, LimitNum = 1, doNum = 0, notFinish = 1, level = 3, playerLvLimit = 28, desIndex = 1, },
		{ name = "黄金矿井（专家）", pworldID = 113, PromotionID = 10, taskID = 0, awardType = 4,  LimitNum = 1, doNum = 0, notFinish = 1, level = 3, playerLvLimit = 50, desIndex = 1, },
		{ name = "森林结界（专家）", pworldID = 119, PromotionID = 21, taskID = 0, awardType = 4,  LimitNum = 1, doNum = 0, notFinish = 1, level = 3, playerLvLimit = 60, desIndex = 1, },
		{ name = "铁翼基地（专家）", pworldID = 114, PromotionID = 15, taskID = 0, awardType = 4,  LimitNum = 1, doNum = 0, notFinish = 1, level = 3, playerLvLimit = 70, desIndex = 1, },
		{ name = "大地之心\t\t\t\t", pworldID = 300, PromotionID = 9, taskID = 0, awardType = 3,  LimitNum = 1, doNum = 0, notFinish = 1, level = 2, playerLvLimit = 30, desIndex = 1, },
		{ name = "赶牛\t\t\t\t\t\t", pworldID = 0, PromotionID = 14, taskID = 2000, awardType = 3,  LimitNum = 1, doNum = 0, notFinish = 1, level = 2, playerLvLimit = 40, desIndex = 1, },
		{ name = "勇闯龙蛋谷\t\t\t", pworldID = 211, PromotionID = 1, taskID = 0, awardType = 1,  LimitNum = 1, doNum = 0, notFinish = 1, level = 1, playerLvLimit = 45, desIndex = 1, },
		{ name = "阳光海滩\t\t\t\t", pworldID = 0, PromotionID = 18, taskID = 30039, awardType = 2,  LimitNum = 1, doNum = 0, notFinish = 1, level = 2, playerLvLimit = 50, desIndex = 1, },
		{ name = "盘山道\t\t\t\t\t", pworldID = 101, PromotionID = 3, taskID = 0, awardType = 1,  LimitNum = 1, doNum = 0, notFinish = 1, level = 1, playerLvLimit = 65, desIndex = 1, },
		{ name = "太阳神殿\t\t\t\t", pworldID = 100, PromotionID = 13, taskID = 0, awardType = 1,  LimitNum = 1, doNum = 0, notFinish = 1, level = 1, playerLvLimit = 120, desIndex = 1, },
	}
local	 t_ClanAwardInfo = {
		{ name = "公会福利\t\t\t\t", awardType = 2, LimitNum = 1, doNum = 0, notFinish = 1, level = 1, desIndex = 2, },
		{ name = "公会礼包\t\t\t\t", awardType = 3, LimitNum = 1, doNum = 0, notFinish = 1, level = 1, desIndex = 3, },
	}
local	 t_ActivityAwardInfo = {	
		{ name = "今日目标奖励\t\t", awardType = 3, LimitNum = 4, doNum = 0, notFinish = 1, level = 2, desIndex = 4, },
	}
local t_ClanAwardDes = {
	"可打开活动助手面板查看相关活动内容与奖励",
	"公会福利：可在[时间城]#c00ff00多德利#n处领取，领取福利可获得大量经验奖励", 
	"公会礼包：可在[时间城]#c00ff00多德利#n处领取，领取礼包可获得大量道具奖励", 
--	"今日目标奖励：每日完成今日目标任务后，打开活动助手点击下方领取奖励按钮，可领金卷等极品福利",
}
function SystemSelectFrame_OnLoad()
end
function SystemSelectFrame_OnShow()
	--清除所有按钮的选种状态
	for i = 1, MAX_EVERY_DAY_ACTIVE do
		local btn = getglobal( "SystemSelectFrameEveryDayActiveBtn"..i);
		btn:DisChecked();
	end
	--默认选择第一个
	SystemSelectFrameEveryDayActiveBtn1:Checked();
end
function InitSystemEscInfoList()
	t_EveryDayActiveInfo = {
		{ name = "永夜镇\t\t\t\t\t", pworldID = 112, PromotionID = 8, taskID = 0, awardType = 4, LimitNum = 1, doNum = 0, notFinish = 1, level = 3, playerLvLimit = 28, desIndex = 1, },
		{ name = "黄金矿井（专家）", pworldID = 113, PromotionID = 10, taskID = 0, awardType = 4,  LimitNum = 1, doNum = 0, notFinish = 1, level = 3, playerLvLimit = 50, desIndex = 1, },
		{ name = "森林结界（专家）", pworldID = 119, PromotionID = 21, taskID = 0, awardType = 4,  LimitNum = 1, doNum = 0, notFinish = 1, level = 3, playerLvLimit = 60, desIndex = 1, },
		{ name = "铁翼基地（专家）", pworldID = 114, PromotionID = 15, taskID = 0, awardType = 4,  LimitNum = 1, doNum = 0, notFinish = 1, level = 3, playerLvLimit = 70, desIndex = 1, },
		{ name = "大地之心\t\t\t\t", pworldID = 300, PromotionID = 9, taskID = 0, awardType = 3,  LimitNum = 1, doNum = 0, notFinish = 1, level = 2, playerLvLimit = 30, desIndex = 1, },
		{ name = "赶牛\t\t\t\t\t\t", pworldID = 0, PromotionID = 14, taskID = 2000, awardType = 3,  LimitNum = 1, doNum = 0, notFinish = 1, level = 2, playerLvLimit = 40, desIndex = 1, },
		{ name = "勇闯龙蛋谷\t\t\t", pworldID = 211, PromotionID = 1, taskID = 0, awardType = 1,  LimitNum = 1, doNum = 0, notFinish = 1, level = 1, playerLvLimit = 45, desIndex = 1, },
		{ name = "阳光海滩\t\t\t\t", pworldID = 0, PromotionID = 18, taskID = 30039, awardType = 2,  LimitNum = 1, doNum = 0, notFinish = 1, level = 2, playerLvLimit = 50, desIndex = 1, },
		{ name = "盘山道\t\t\t\t\t", pworldID = 101, PromotionID = 3, taskID = 0, awardType = 1,  LimitNum = 1, doNum = 0, notFinish = 1, level = 1, playerLvLimit = 65, desIndex = 1, },
		{ name = "太阳神殿\t\t\t\t", pworldID = 100, PromotionID = 13, taskID = 0, awardType = 1,  LimitNum = 1, doNum = 0, notFinish = 1, level = 1, playerLvLimit = 120, desIndex = 1, },
	}
	 t_ClanAwardInfo = {
		{ name = "公会福利\t\t\t\t", awardType = 2, LimitNum = 1, doNum = 0, notFinish = 1, level = 1, desIndex = 2, },
		{ name = "公会礼包\t\t\t\t", awardType = 3, LimitNum = 1, doNum = 0, notFinish = 1, level = 1, desIndex = 3, },
	}
	t_ActivityAwardInfo = {	
			{ name = "今日目标奖励\t\t", awardType = 3, LimitNum = 4, doNum = 0, notFinish = 1, level = 2, desIndex = 4, },
	}
end
function SetSystemSelectFrameActiveInfo()
	InitSystemEscInfoList();
	local mainplayer = ActorMgr:getMainPlayer();
	--设置显示的四个任务
	for i = 1, table.getn( t_EveryDayActiveInfo ) do
		--任务是否完成
		if t_EveryDayActiveInfo[i].pworldID ~= 0 then
			local pworldID = t_EveryDayActiveInfo[i].pworldID;
			local limitData = PworldLimitInfo:getPworldData(pworldID);
			if limitData.freeTimeUse >= limitData.freeTimeTotal then
				t_EveryDayActiveInfo[i].Show = false;
				t_EveryDayActiveInfo[i].doNum = 1;
			else
				t_EveryDayActiveInfo[i].Show = true;
				t_EveryDayActiveInfo[i].doNum = 0;
			end
		else
			local taskID = t_EveryDayActiveInfo[i].taskID;
			if Quest:getQusetHaveFinishNumOneDay( taskID ) >= Quest:getQuestMaxCanAcceptNumOneDay( taskID ) then
				t_EveryDayActiveInfo[i].Show = false;
				t_EveryDayActiveInfo[i].doNum = 1;
			else
				t_EveryDayActiveInfo[i].Show = true;
				t_EveryDayActiveInfo[i].doNum = 0;
			end
			--特殊处理下海滩任务
			if string.find( t_EveryDayActiveInfo[i].name, "阳光海滩" ) then
				if Quest:getQusetHaveFinishNumOneDay( 30055 ) >= Quest:getQuestMaxCanAcceptNumOneDay( 30055 ) then
					t_EveryDayActiveInfo[i].Show = false;
					t_EveryDayActiveInfo[i].doNum = 1;
				end
				if Quest:getQusetHaveFinishNumOneDay( 30058 ) >= Quest:getQuestMaxCanAcceptNumOneDay( 30058 ) then
					t_EveryDayActiveInfo[i].Show = false;
					t_EveryDayActiveInfo[i].doNum = 1;
				end
			end
		end
		if mainplayer:getLv() < t_EveryDayActiveInfo[i].playerLvLimit then
			t_EveryDayActiveInfo[i].Show = false;
			t_EveryDayActiveInfo[i].doNum = 0;
		end
	end
	--按照优先级进行一次排序
	table.sort( t_EveryDayActiveInfo, function(a, b)
		return a.level < b.level;
		end)
	--将已完成的移动到列表尾部
	local t_tempIndexList = {}
	for i = 1, table.getn( t_EveryDayActiveInfo ) do
		local temp = t_EveryDayActiveInfo[i];
		if t_EveryDayActiveInfo[i] == nil then
			break
		end
		if not temp.Show then
			table.insert( t_tempIndexList, temp );
		end
	end
	for i = 1 ,table.getn( t_tempIndexList ) do
		for j = 1, table.getn( t_EveryDayActiveInfo ) do
			if t_EveryDayActiveInfo[j].name == t_tempIndexList[i].name then
				table.remove( t_EveryDayActiveInfo, j );
				table.insert( t_EveryDayActiveInfo, t_tempIndexList[i] )
			end
		end
	end
	local t_actives = {}
	--挑选等级条件合适的来显示
	for i = 1, table.getn( t_EveryDayActiveInfo ) do
		if 	t_EveryDayActiveInfo[i].playerLvLimit <= mainplayer:getLv() then
			local temp = t_EveryDayActiveInfo[i];
			table.insert( t_actives, temp );
		end
	end
	--清空按钮先前数据
	for i = 1, MAX_EVERY_DAY_ACTIVE do
		local btn = getglobal( "SystemSelectFrameEveryDayActiveBtn"..i );
		btn:SetClientUserData( 0, 0 );
		btn:SetClientUserData( 1, 0 );
		local btnAwardTypeTex = getglobal( "SystemSelectFrameEveryDayActiveBtn"..i.."AwardTypeTex" );
		local btnInfo = getglobal( "SystemSelectFrameEveryDayActiveBtn"..i.."Info" );
		btnAwardTypeTex:SetTexUV( 0, 0, 1, 1 );
		btnInfo:SetText( "", 255,255,190 )
		btn:Hide();
	end
	--设置按钮显示信息
	local num = table.getn( t_actives ) > MAX_EVERY_DAY_ACTIVE and MAX_EVERY_DAY_ACTIVE or table.getn( t_actives )
	for i = 1, num do
		local btn = getglobal( "SystemSelectFrameEveryDayActiveBtn"..i );
		btn:Show();
		btn:SetClientUserData( 0, t_actives[i].PromotionID )
		btn:SetClientUserData( 1, t_actives[i].desIndex )
		local btnAwardTypeTex = getglobal( "SystemSelectFrameEveryDayActiveBtn"..i.."AwardTypeTex" );
		local btnInfo = getglobal( "SystemSelectFrameEveryDayActiveBtn"..i.."Info" );
		local awardType = t_actives[i].awardType
		local uv = t_AwardTypeTexUV[awardType]
		btnAwardTypeTex:SetTexUV( uv.x, uv.y, uv.w, uv.h );
		if t_actives[i].doNum >= t_actives[i].LimitNum then 
			btnInfo:SetText( "\t\t\t\t\t\t"..t_actives[i].name.."\t\t\t\t\t\t"..t_actives[i].doNum.."/"..t_actives[i].LimitNum, 255,255,190 )
		else 
			btnInfo:SetText( "\t\t\t\t\t\t"..t_actives[i].name.."\t\t\t\t\t\t"..t_actives[i].doNum.."/"..t_actives[i].LimitNum, 255,68,25 )
		end
	end
	local levelupDef = getLevelUP( mainplayer:getLv() );
	local szBuildUpExp = "0/0"
	if levelupDef ~= nil then 
		local offExp = mainplayer:getOfflineExp();
		local maxOffExp = levelupDef.MaxOfflineExp;
		szBuildUpExp = offExp.."/"..maxOffExp;
		if offExp == 0 or maxOffExp == 0 then
			SystemSelectFrameBuildUpExp:Hide();
		else
			SystemSelectFrameBuildUpExp:Show();
		end
	else
		SystemSelectFrameBuildUpExp:Hide();
	end
	SystemSelectFrameBuildUpExp:SetText( "剩余养精蓄锐经验："..szBuildUpExp, 255,255,190 )
	local pworldExp = mainplayer:getPworldExp();
	local maxPworldExp = levelupDef.MaxPworldOffExp;
	SystemSelectFrameMakeUpExp:SetText( "剩余活动补偿经验："..pworldExp.."/"..maxPworldExp, 255,255,190 )
	if pworldExp == 0 or maxPworldExp == 0 then
		SystemSelectFrameMakeUpExp:Hide();
	else
		SystemSelectFrameMakeUpExp:Show();
	end
	--设置公会福利信息
	local t_awardInfo = {
		["公会福利"] = { totalNum = 0, finishNum = 0, },
		["公会礼包"] = { totalNum = 0, finishNum = 0, },
	}
	if mainplayer:isInClan() then
		local nNum = GameWizard:GetClanOneDayGift();
		for i = 1, nNum do
			local onedaygift = GameWizard:GetClanOneDayGift( i - 1 );
			local clanLv = GameClanManager:getClanViewer().m_clanInfo.m_nClanLevel
			t_awardInfo[onedaygift.Name].totalNum = t_awardInfo[onedaygift.Name].totalNum + 1;
			if onedaygift.GiftCond.ClanLevel == 0 or onedaygift.GiftCond.ClanLevel == clanLv then				
				local state = CanGetClanAward( i - 1 );
				if state == ALREADY_GET_GIFT then
					t_awardInfo[onedaygift.Name].finishNum = t_awardInfo[onedaygift.Name].finishNum + 1;
				end
			else
				t_awardInfo[onedaygift.Name].finishNum = t_awardInfo[onedaygift.Name].finishNum + 1;
			end
		end
	else
		t_awardInfo["公会福利"].totalNum = 1
		t_awardInfo["公会福利"].finishNum = 0
		t_awardInfo["公会礼包"].totalNum = 1
		t_awardInfo["公会礼包"].finishNum = 0
	end
	if t_awardInfo["公会福利"].totalNum == t_awardInfo["公会福利"].finishNum then
		t_ClanAwardInfo[1].Show = false;
		t_ClanAwardInfo[1].doNum = 1;
	else
		t_ClanAwardInfo[1].Show = true;
		t_ClanAwardInfo[1].doNum = 0;
	end
	if t_awardInfo["公会礼包"].totalNum == t_awardInfo["公会礼包"].finishNum then
		t_ClanAwardInfo[2].Show = false;
		t_ClanAwardInfo[2].doNum = 1;
	else
		t_ClanAwardInfo[2].Show = true;
		t_ClanAwardInfo[2].doNum = 0;
	end
	--设置今日目标信息,今日目标统计六个箱子
	local MAX_REWARD_NUM = 6
	local getWardFlag = GamePromotion:GetDailyAwardFlag();
	for i=1, MAX_REWARD_NUM do
		if bitAnd( 2^i,getWardFlag ) then
			t_ActivityAwardInfo[1].doNum = t_ActivityAwardInfo[1].doNum + 1
		end
	end
	if t_ActivityAwardInfo[1].LimitNum <= t_ActivityAwardInfo[1].doNum then
		t_ActivityAwardInfo[1].Show = false
		t_ActivityAwardInfo[1].doNum = 4
	else
		t_ActivityAwardInfo[1].Show = true
	end
	--按照优先级进行一次排序
	table.sort( t_ClanAwardInfo, function(a, b)
		return a.level < b.level;
		end)
	--将已完成的移动到列表尾部
	local t_tempIndexList = {}
	for i = 1, table.getn( t_ClanAwardInfo ) do
		local temp = t_ClanAwardInfo[i];
		if t_ClanAwardInfo[i] == nil then
			break
		end
		if not temp.Show then
			table.insert( t_tempIndexList, temp );
		end
	end
	for i = 1 ,table.getn( t_tempIndexList ) do
		for j = 1, table.getn( t_ClanAwardInfo ) do
			if t_ClanAwardInfo[j].name == t_tempIndexList[i].name then
				table.remove( t_ClanAwardInfo, j );
				table.insert( t_ClanAwardInfo, t_tempIndexList[i] )
			end
		end
	end
	local t_awards = {};
	--没有公会则去除公会相关
	if not mainplayer:isInClan() then
		for i = 1, table.getn( t_ClanAwardInfo ) do
			if not string.find(  t_ClanAwardInfo[i].name, "公会" ) then
				local temp = t_ClanAwardInfo[i]
				table.insert( t_awards, temp );
			end
		end
	else
		t_awards = t_ClanAwardInfo;
	end

	--等级未达到15级的时候清除今日目标
	if mainplayer:getLv() < 15 then
		for i = 1, table.getn( t_awards ) do
			if string.find(  t_awards[i].name, "今日目标奖励" ) then
				table.remove( t_awards, i );
			end
		end
	end
	--清空按钮先前数据
	for i = 1, MAX_CLAN_AWARD_ACTIVE do
		local btn = getglobal( "SystemSelectFrameClanAwardBtn"..i );
		btn:SetClientUserData( 1, 0 )
		local btnAwardTypeTex = getglobal( "SystemSelectFrameClanAwardBtn"..i.."AwardTypeTex" );
		local btnInfo = getglobal( "SystemSelectFrameClanAwardBtn"..i.."Info" );
		btnAwardTypeTex:SetTexUV( 0, 0, 1, 1 );
		btnInfo:SetText( "", 255,255,190  )
	end
	--设置按钮显示信息
	local awardsNum = table.getn( t_awards ) > MAX_CLAN_AWARD_ACTIVE and MAX_CLAN_AWARD_ACTIVE or table.getn( t_awards )
	for i = 1, awardsNum do		
		local btn = getglobal( "SystemSelectFrameClanAwardBtn"..i );
		btn:SetClientUserData( 1, t_awards[i].desIndex )
		local btnAwardTypeTex = getglobal( "SystemSelectFrameClanAwardBtn"..i.."AwardTypeTex" );
		local btnInfo = getglobal( "SystemSelectFrameClanAwardBtn"..i.."Info" );
		local awardType = t_awards[i].awardType
		local uv = t_AwardTypeTexUV[awardType]
		btnAwardTypeTex:SetTexUV( uv.x, uv.y, uv.w, uv.h );
		if t_awards[i].doNum >= t_awards[i].LimitNum then 
			btnInfo:SetText( "\t\t\t\t\t\t"..t_awards[i].name.."\t\t\t\t\t\t"..t_awards[i].doNum.."/"..t_awards[i].LimitNum, 255,255,190  )
		else 
			btnInfo:SetText( "\t\t\t\t\t\t"..t_awards[i].name.."\t\t\t\t\t\t"..t_awards[i].doNum.."/"..t_awards[i].LimitNum, 255,68,25  )
		end
	end
	-- 第三条 今日目标 单独处理
	local btn = getglobal( "SystemSelectFrameClanAwardBtn"..3 );
	btn:SetClientUserData( 1, t_ActivityAwardInfo[1].desIndex )
	local btnAwardTypeTex = getglobal( btn:GetName().."AwardTypeTex" );
	local btnInfo = getglobal( btn:GetName().."Info" );
	local awardType = t_ActivityAwardInfo[1].awardType
	local uv = t_AwardTypeTexUV[awardType]
	btnAwardTypeTex:SetTexUV( uv.x, uv.y, uv.w, uv.h );
	local rewardBtn = getglobal( btn:GetName().."RewardBtn"  );
	local RewardNum = HadGetDailyRewardNum();
	if RewardNum > 0 then 
		btnInfo:SetText( "\t\t\t\t\t\t"..t_ActivityAwardInfo[1].name.."\t\t\t\t\t\t"..t_ActivityAwardInfo[1].doNum.."/"..t_ActivityAwardInfo[1].LimitNum, 255,68,25  )
		rewardBtn:Enable();
	else 
		btnInfo:SetText( "\t\t\t\t\t\t"..t_ActivityAwardInfo[1].name.."\t\t\t\t\t\t"..t_ActivityAwardInfo[1].doNum.."/"..t_ActivityAwardInfo[1].LimitNum, 255,255,190  )
		rewardBtn:Disable();
	end
	if mainplayer:getLv() < 15 then
		SystemSelectFramePromotion:Hide()
	else
		SystemSelectFramePromotion:Show()
	end
end
function SystemActivetyBtn_OnClick()
	if string.find( this:GetName(), "ClanAwardBtn", 1, true ) then return end
	--清除所有按钮的选种状态
	for i = 1, MAX_EVERY_DAY_ACTIVE do
		local btn = getglobal( "SystemSelectFrameEveryDayActiveBtn"..i);
		btn:DisChecked();
	end
	--设置当前按钮为选种状态
	this:Checked();
end
function SystemActivetyBtn_OnDoubleClick()
	if string.find( this:GetName(), "ClanAwardBtn", 1, true ) then return end
	--清除所有按钮的选种状态
	for i = 1, MAX_EVERY_DAY_ACTIVE do
		local btn = getglobal( "SystemSelectFrameEveryDayActiveBtn"..i);
		btn:DisChecked();
	end
	--设置当前按钮为选种状态
	this:Checked();
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer:getLv() < 15 then
		ShowMidTips( "15级以上玩家才可使用活动助手" );
		return
	end
	local nRelateID = this:GetClientUserData( 0 );
	if not PromotionFrame:IsShown() then
		PromotionFrame:Show();
		SystemSelectFrame:Hide();
	end
	PromotionAllBtn_OnClick();
	local nNum = GamePromotion:getAllNum();
	local slider = getglobal("PromotionFrameRuningScrollBar");
	for i=1, nNum do
		local nIndex		= GamePromotion:getAllRow( i - 1 );
		local promotionData = GamePromotion:getPromotionData( nIndex );
		if promotionData.m_nID == nRelateID then
			local MAX_PROMOTION_LIST = 8;
			local sliderVal = i - MAX_PROMOTION_LIST > 0 and i - MAX_PROMOTION_LIST or 0 ;
			slider:SetValue( sliderVal );
			local proBtn = getglobal("PromotionInfo"..i - sliderVal );
			PromotionInfoDisCheckAll();
			proBtn:Checked();
			SetPromotionCurSelIndex( promotionData.m_nID );
			RefreshDescInfo();
			break;
		end
	end
end
function SystemActivetyBtn_OnEnter()
	local des = ""
	local desIndex = this:GetClientUserData( 1 );
	if desIndex == 0 then return end
	des = t_ClanAwardDes[desIndex];
	SetGameTooltips( this:GetName().."TipsPoint", des )
end

function SystemSelectFramePromotion_OnClick()
	local CurActiveIndex = 0;
	for i = 1, MAX_EVERY_DAY_ACTIVE do
		local btn = getglobal( "SystemSelectFrameEveryDayActiveBtn"..i);
		if btn:IsChecked() then
			CurActiveIndex = i;
		end
	end
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer:getLv() < 15 then
		ShowMidTips( "15级以上玩家才可使用活动助手" );
		return
	end
	local btn = getglobal( "SystemSelectFrameEveryDayActiveBtn"..CurActiveIndex )
	local nRelateID = btn:GetClientUserData( 0 );
	if not PromotionFrame:IsShown() then
		PromotionFrame:Show();
		SystemSelectFrame:Hide();
	end
	PromotionAllBtn_OnClick();
	local nNum = GamePromotion:getAllNum();
	local slider = getglobal("PromotionFrameRuningScrollBar");
	for i=1, nNum do
		local nIndex		= GamePromotion:getAllRow( i - 1 );
		local promotionData = GamePromotion:getPromotionData( nIndex );
		if promotionData.m_nID == nRelateID then
			local MAX_PROMOTION_LIST = 8;
			local sliderVal = i - MAX_PROMOTION_LIST > 0 and i - MAX_PROMOTION_LIST or 0 ;
			slider:SetValue( sliderVal );
			local proBtn = getglobal("PromotionInfo"..i - sliderVal );
			PromotionInfoDisCheckAll();
			proBtn:Checked();
			SetPromotionCurSelIndex( promotionData.m_nID );
			RefreshDescInfo();
			break;
		end
	end
end

function SystemActivetyBtnRewardBtn_OnEnter()

end

function SystemSelectFrameConRewardBtn_OnClick()
	FlopRewardFrame:Show();
	SystemSelectFrame:Hide();
end
function SystemSelectFrameCheckConRewardBtn_OnClick()
	CheckInRewardFrame:Show();
	SystemSelectFrame:Hide();
end
function SystemSelectFrameClanAwardBtn_OnClick()
	ActivityShowBtn_nClick();
	SystemSelectFrame:Hide();
end