local MAX_PROMOTION_LIST		= 8;

local MAX_PROMOTION_TIPLIST		= 4;

local MAX_CHECK_BTN_NUM			= 5;

local nType						= PROMOTION_ALL;

local nAllScrollPos		= 1;	--全部 正在进行

local nActiveScrollPos	= 1;	-- 活动 正在进行

local nClanScrollPos		= 1;	-- 公会 正在进行

local nDailyTaskScrollPos	= 1;	-- 今日目标 正在进行

local nActivityScrollPos	= 1;	-- 今日活动 正在进行

local nBossScrollPos = 0;	-- 世界boss

local nCurSelIndex	= -1 ;	--当前选中的项

local bFoundSelectPromption = false;

local MAX_DAILY_AWARD_NUM = 4;
local MAX_REWARD_NUM = 6;

local MAX_BOSS_LIST = 7;
local MAX_WORLD_BOSS_ICON = 8;
local ModelAngle = 0;
local t_WorldBoss = {};

--特殊活动引导id
local HAPPY_CARNIVAL_GUID_ID    = 7;
local LIGHT_SHADOW_FIGHT_ID		= 42;
local MACHINE_WAR_ACTIVITY_ID	= 45;
local SPARTA_PWORLD_ACTIVITY_ID	= 48;
local TANK_WAR_ACTIVITY_ID		= 64;

local WHETHER_REAL_BOSS_ID		= 30;
	  COWBOY_BUSY_FINALS_ID		= 32;
local HEAVEN_PIE_ACTIVE_ID		= 36;
local DARK_COMING_ID			= 23;

local LIFE_IS_MOVEMENT_ACTIVITY_ID	= 54;
GUCHENG_BATTLE_ACTIVITY_ID	= 61;     -- 勇闯失落之域
tWeekDayName = {"星期日","星期一","星期二","星期三","星期四","星期五","星期六",};

function SetPromotionCurSelIndex( index )
	nCurSelIndex	= index ;
end

function PromotionFrame_OnLoad()
	this:RegisterEvent( "GE_PROMOTION_UPDATE" );
	this:RegisterEvent( "GE_PLAYER_LEAVEUP" );
	this:RegisterEvent( "GE_ENTER_PLAYERLOGIN" );
	this:RegisterEvent( "BATTLE_LIMIT_INFO_CHG" );
	this:RegisterEvent( "GE_QUEST_DIALOG_ACCEPT" );
	this:RegisterEvent( "GE_QUEST_GETQUESTINFO" );
	this:RegisterEvent( "GE_QUEST_CHANGE" );
	this:RegisterEvent( "GE_PROMOTION_DAILY_INFO_CHG" );
	this:RegisterEvent( "GE_CLAN_ACTIVE_UPDATE" );
	this:RegisterEvent( "GE_STRONG_INFO" );
	this:RegisterEvent( "GE_WORLDCITY_INFO" );
	this:RegisterEvent( "GE_CLAN_SVR_INFO" );
	this:RegisterEvent( "GE_CLAN_DISABLEFRAME" );
	this:RegisterEvent( "GE_BEACH_TIME_UPDATE" );
	for i = 1, MAX_PROMOTION_LIST do
		local btn = getglobal( "PromotionBkg" ..i );
		local t_Tex = {
			btn:GetName() .. "NameBkg", btn:GetName() .. "TimeBkg", btn:GetName() .. "LevelBkg",
			btn:GetName() .. "StateBkg", btn:GetName() .. "ButtonBkg" };
		for j=1,#t_Tex do
			local tex = getglobal( t_Tex[j] )
			if math.mod( i,2 ) == 0 then
				tex:ChangeTextureTemplate( "WhiteCategoryBkg" );
			else
				tex:ChangeTextureTemplate( "BlackCategoryBkg" );
			end
		end
	end

	for i=1, MAX_BOSS_LIST do
		local name = "WorldBossListBossInfo" ..i;
		local t_Tex = {
			name .. "NameBkg", name .. "LevelBkg", name .. "TimeBkg", name .. "AreaBkg", name .. "ButtonBkg" };
		for j=1,#t_Tex do
			local tex = getglobal( t_Tex[j] )
			if math.mod( i,2 ) == 0 then
				tex:ChangeTextureTemplate( "WhiteCategoryBkg" );
			else
				tex:ChangeTextureTemplate( "BlackCategoryBkg" );
			end
		end
	end
end

function PromotionFrame_OnEvent()
	if arg1 == "GE_ENTER_PLAYERLOGIN" then
		GetWorldBossList();
	end
	if arg1 == "GE_PROMOTION_UPDATE" or arg1 == "BATTLE_LIMIT_INFO_CHG" or arg1 == "GE_PLAYER_LEAVEUP" 
		or arg1 == "GE_QUEST_DIALOG_ACCEPT" or arg1 == "GE_QUEST_GETQUESTINFO" or arg1 == "GE_QUEST_CHANGE" 
		or arg1 == "GE_CLAN_ACTIVE_UPDATE" then
		--更新
		if PromotionFrame:IsShown() and nType ~= PRMOTION_BOSS then
			setNewSortPromition(false);
		end
	elseif arg1 == "GE_PROMOTION_DAILY_INFO_CHG" or arg1 == "GE_CLAN_SVR_INFO" or arg1 == "GE_WORLDCITY_INFO" 
		or arg1 == "GE_STRONG_INFO" then
		if PromotionFrame:IsShown() then
			setDailyGoalInfo();
		else
			local num = HadGetDailyRewardNum();
			if num > 0 then
				ActivityShowBtnUVAnimationTex:StartAlphaAmin( 1 );
			end
		end
	end
	if arg1 == "GE_CLAN_DISABLEFRAME" then
		if not DailyRewardFrame:IsShown() then
			local num = HadGetDailyRewardNum();
			if num > 0 then
				PromotionFrameGetRewardBtnUVAnimationTex:SetUVAnimation(80, true)
				PromotionFrameGetRewardBtnUVAnimationTex:Show();
			else
				PromotionFrameGetRewardBtnUVAnimationTex:Hide();
			end
		end
	end
	if arg1 == "GE_QUEST_CHANGE" then
		local mainplayer = ActorMgr:getMainPlayer()
		if mainplayer ~= nil and mainplayer:getLv() >= 15 and not PromotionFrame:IsShown() then
			for i=1,GamePromotion:getDailyTaskNum() do
				local nTaskID =  GamePromotion:getDailyTaskID( i - 1 );
				local taskRunState = Quest:getQuestRunState( nTaskID )
				if taskRunState == CLIENT_TASK_STAT_FINI then
					ActivityShowBtnUVAnimationTex:StartAlphaAmin( 1 )
					break;
				end
			end
		end
	end
	if arg1 == "GE_QUEST_GETQUESTINFO" then
		local mainplayer = ActorMgr:getMainPlayer()
		if mainplayer ~= nil and mainplayer:getLv() >= 15 and not PromotionFrame:IsShown() then
			for i=1,GamePromotion:getDailyTaskNum() do
				local nTaskID =  GamePromotion:getDailyTaskID( i - 1 );
				local taskRunState = Quest:getQuestRunState( nTaskID )
				if taskRunState == CLIENT_TASK_STAT_FINI then
					ActivityShowBtnUVAnimationTex:StartAlphaAmin( 1 )
					break;
				elseif taskRunState == CLIENT_TASK_STAT_NO and Quest:getQusetHaveFinishNumOneDay( nTaskID ) == 0 then
					ActivityShowBtnUVAnimationTex:StartAlphaAmin( 1 )
					break;
				end
			end
			local getWardFlag = GamePromotion:GetDailyAwardFlag();
			for i=1,GamePromotion:GetDailyFiniNum() do
				if i > MAX_DAILY_AWARD_NUM then
					break
				end
				if not bitAnd( 2^i,getWardFlag ) then
					ActivityShowBtnUVAnimationTex:StartAlphaAmin( 1 )
					break;
				end
			end
		end
	end
	if arg1 == "GE_PLAYER_LEAVEUP" then
		local mainplayer = ActorMgr:getMainPlayer()
		if mainplayer:getLv() == 15 and not PromotionFrame:IsShown() then
			ActivityShowBtn_nClick();
		end
	end
	if arg1 == "GE_BEACH_TIME_UPDATE" then 
		if nType == PRMOTION_BUSY then 
			PromotionFrameBusyBtn_OnClick();
		end
	end
end

function CanJoinActive( data )
	return	GamePromotion:getAutoTeamState( data["row"] ) == PROMOTION_OK;
end

local t_promotionFramMutexFrames = { "SuccessCreatTaskAutoTeamFrame", "CreatingAutoTeamFrame", "SearchTeamFrame", 
												"SuccessCreatActiveAutoTeamFrame" };

function PromotionFrameDisSelectAll()
	PromotionFrame_SelectAll:setCheckState(false);
	for i = 1,MAX_CHECK_BTN_NUM do
		local checkbtn = getglobal( "PromotionFrame_CheckButton"..i );
		checkbtn:setCheckState(false);
	end
end

function PromotionFrame_OnShow()
	for _, name in ipairs( t_promotionFramMutexFrames ) do
		local frame = getglobal( name );
		if frame:IsShown() then
			SetCurSuoXiaoFrameInfo( { name = frame:GetName() } );
			frame:Hide();
			if NeedPlayeEffect( { name = frame:GetName() } ) then
				BeginPlayAutoTeamBtnEffect();
			end
		end
	end
	PromotionFrame_Busy:Disable();
	PromotionTodayBtn_OnClick();
	setNewSortPromition( true );
	setDailyGoalInfo();
	--GamePromotion:requestWorldBossInfo();
end

-- 显示今日目标相关信息
function setDailyGoalInfo()
	local finiTaskNum = GamePromotion:GetDailyFiniNum() > MAX_REWARD_NUM and MAX_REWARD_NUM or GamePromotion:GetDailyFiniNum();
	local scale = finiTaskNum / MAX_REWARD_NUM 
	if scale > 1 then scale = 1 end
	local tex = getglobal( "PromotionFrameProgressTex" );
	local font = getglobal( "PromotionFrameTodayFont" );
	if finiTaskNum > 0 then
		tex:SetWidth(math.floor(300 * scale + 0.5))
		tex:Show();
	else
		tex:Hide();
	end
	font:SetText( "完成度：" .. finiTaskNum .. "/" .. MAX_REWARD_NUM );

	if not DailyRewardFrame:IsShown() then
		local num = HadGetDailyRewardNum();
		if num > 0 then
			PromotionFrameGetRewardBtnUVAnimationTex:SetUVAnimation(80, true)
			PromotionFrameGetRewardBtnUVAnimationTex:Show();
		else
			PromotionFrameGetRewardBtnUVAnimationTex:Hide();
		end
	end
end

function PromotionFrame_OnHide()
	nCurSelIndex = -1;
	if DailyRewardFrame:IsShown() then
		DailyRewardFrame:Hide();
	end
end

function PromotionFrameGetRewardBtn_OnClick()
	PromotionFrameGetRewardBtnUVAnimationTex:Hide();
	if DailyRewardFrame:IsShown() then
		DailyRewardFrame:Hide();
	else
		DailyRewardFrame:Show();
	end
end

--显示全部
function ShowAll()
	local nNum = GamePromotion:getAllNum();
	local nRuningLineIdx = 1;
	local nTotalRuningCount = 0;
	for i = 1,nNum do
		local nRow =  GamePromotion:getAllRow( i - 1 );
		local nStartInfo = GamePromotion:getCurrPromitionStartInfo( nRow );
		local promotionData = GamePromotion:getPromotionData( nRow );
		local currTypeCheckButton = getglobal( "PromotionFrame_CheckButton"..promotionData.m_nMainRewardType );
		local SelectAllCheckBtn = getglobal("PromotionFrame_SelectAll");
		if GamePromotion:isInDatePeriod( nRow ) and ( SelectAllCheckBtn:GetCheckState() == true or
			currTypeCheckButton == nil or (currTypeCheckButton:GetCheckState() == true) )then
			nTotalRuningCount = nTotalRuningCount + 1;
			if nTotalRuningCount >= nAllScrollPos and nRuningLineIdx <= MAX_PROMOTION_LIST then
				nRuningLineIdx = BondingRuningPromotion( nRow,nRuningLineIdx,nStartInfo );
			end
		end
	end
	ResetRuningPromotion( nTotalRuningCount + 1, nRuningLineIdx, nAllScrollPos );
end
 
--显示公会相关
function ShowClanPro()
	local nNum = GamePromotion:getClanProNum();
	local nRuningLineIdx = 1;
	local nTotalRuningCount = 0;
	for i = 1,nNum do
		local nRow =  GamePromotion:getClanProRow( i - 1 );
		local nStartInfo = GamePromotion:getCurrPromitionStartInfo( nRow );
		local promotionData = GamePromotion:getPromotionData( nRow );
		local currTypeCheckButton = getglobal( "PromotionFrame_CheckButton"..promotionData.m_nMainRewardType );
		local SelectAllCheckBtn = getglobal("PromotionFrame_SelectAll");
		if SelectAllCheckBtn:GetCheckState() == true or
			currTypeCheckButton == nil or (currTypeCheckButton:GetCheckState() == true) then
			nTotalRuningCount = nTotalRuningCount + 1;
			if nTotalRuningCount >= nClanScrollPos and nRuningLineIdx < MAX_PROMOTION_LIST then
				nRuningLineIdx = BondingRuningPromotion( nRow,nRuningLineIdx,nStartInfo );
			end
		end
	end
	ResetRuningPromotion( nTotalRuningCount + 1, nRuningLineIdx, nClanScrollPos );
end

--显示副本
function ShowActivePro()
	local nNum = GamePromotion:getPromotionNum();
	local nRuningLineIdx = 1;
	local nTotalRuningCount = 0;
	for i = 1,nNum do
		local nRow =  GamePromotion:getPromotionRow( i - 1 );
		local nStartInfo = GamePromotion:getCurrPromitionStartInfo( nRow );
		local promotionData = GamePromotion:getPromotionData( nRow );
		local currTypeCheckButton = getglobal( "PromotionFrame_CheckButton"..promotionData.m_nMainRewardType );
		local SelectAllCheckBtn = getglobal("PromotionFrame_SelectAll");
		if SelectAllCheckBtn:GetCheckState() == true or
			currTypeCheckButton == nil or (currTypeCheckButton:GetCheckState() == true) then
			nTotalRuningCount = nTotalRuningCount + 1;
			if nTotalRuningCount >= nActiveScrollPos and nRuningLineIdx <= MAX_PROMOTION_LIST then
				nRuningLineIdx = BondingRuningPromotion( nRow,nRuningLineIdx,nStartInfo );
			end
		end
	end
	ResetRuningPromotion( nTotalRuningCount + 1, nRuningLineIdx, nActiveScrollPos );
end

--显示每日目标
function ShowDailyTaskly()
	local mainplayer = ActorMgr:getMainPlayer()	
	local nNum = GamePromotion:getDailyTaskNum();
	local nRuningLineIdx = 1;
	local nTotalRuningCount = 0;
	if mainplayer ~= nil and mainplayer:getLv() >= 15 then
		for i = 1,nNum do
			local nTaskID =  GamePromotion:getDailyTaskID( i - 1 );
			nTotalRuningCount = nTotalRuningCount + 1;
			if nTotalRuningCount >= nDailyTaskScrollPos and nRuningLineIdx <= MAX_PROMOTION_LIST then
				nRuningLineIdx = BondingRuningPromotion( i - 1,nRuningLineIdx,nil,nTaskID );
			end
		end
	end
	ResetRuningPromotion( nTotalRuningCount + 1, nRuningLineIdx, nDailyTaskScrollPos );
end

--显示每日活动
function ShowActivityPro()
	local nNum = GamePromotion:getAllNum();
	local nRuningLineIdx = 1;
	local nTotalRuningCount = 0;
	for i = 1,nNum do
		local nRow =  GamePromotion:getAllRow( i - 1 );
		local nStartInfo = GamePromotion:getCurrPromitionStartInfo( nRow );
		local promotionData = GamePromotion:getPromotionData( nRow );
		local currTypeCheckButton = getglobal( "PromotionFrame_CheckButton"..promotionData.m_nMainRewardType );
		local SelectAllCheckBtn = getglobal("PromotionFrame_SelectAll");
		if (SelectAllCheckBtn:GetCheckState() == true or
			currTypeCheckButton == nil or currTypeCheckButton:GetCheckState() == true) and nRuningLineIdx <= MAX_PROMOTION_LIST and 
			bitAnd( promotionData.m_nHuoDongShow, 2 ) and nStartInfo.startFlag ~= PROMOTION_NOTTODAY then
			nTotalRuningCount = nTotalRuningCount + 1;
			nRuningLineIdx = BondingRuningPromotion( nRow,nRuningLineIdx,nStartInfo );
		end
	end
	ResetRuningPromotion( nTotalRuningCount + 1, nRuningLineIdx, nActivityScrollPos );
end

--刷新界面
function setNewSortPromition( bClearSel )
	bFoundSelectPromption = false;
	if nType ~= PRMOTION_DAYACT then
		GamePromotion:sortPromition( nType );
		if nType == PROMOTION_ALL then
			ShowAll();
		elseif nType == PRMOTION_ACTIVE then
			ShowActivePro();
		elseif nType == PRMOTION_CLAN then
			ShowClanPro();
		elseif nType == PRMOTION_DAILY_TASK then
			ShowDailyTaskly();
		elseif nType == PRMOTION_BUSY then 
			ShowAll();
		end
	else
		ShowActivityPro();
	end

	if not bFoundSelectPromption then
		PromotionInfoDisCheckAll();
		if PromotionInfo1:IsShown() then
			nCurSelIndex = PromotionInfo1:GetClientUserData(0);
			PromotionInfo1:Checked();
		else
			nCurSelIndex = -1;
		end
		RefreshDescInfo();
	end
	-- 刷新服务器时间
	PromotionFrameClockFont:SetText( "当前时间："..GamePromotion:getSerHour()..":"..string.format("%02d",GamePromotion:getSerMin()) );
end

--点击checkbutton
function PromotionFrame_CheckButtonOnClick()
	PromotionFrameDisSelectAll();
	this:setCheckState( true );
	PromotionFrameRuningScrollBar:SetValue( 0 );
	PromotionFrameRuningOnValueChanged();
	--setNewSortPromition(true);	
end

local CLAN_SPY_ID = 22;
local CLAN_GUARD_ID = 27;
local CLAN_DART_ID 	= 55;    -- 公会商运
--显示正在进行
function BondingRuningPromotion( nRow,nLineIndex,nStartInfo,nTaskID )
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return nLineIndex;
	end
	local ProBtn			= getglobal( "PromotionInfo" .. nLineIndex );
	local NameFont			= getglobal( "PromotionInfo" .. nLineIndex .. "NameFont");
	local TimeFont			= getglobal( "PromotionInfo" .. nLineIndex .. "TimeFont");
	local LevelFont			= getglobal( "PromotionInfo" .. nLineIndex .. "LevelFont");
	local stateTex			= getglobal( "PromotionInfo" .. nLineIndex .. "StateTex");
	local clanTex			= getglobal( "PromotionInfo" .. nLineIndex .. "ClanTex" );
	local mustTex			= getglobal( "PromotionInfo" .. nLineIndex .. "MustTex" );
	local uvTex				= getglobal( "PromotionInfo" .. nLineIndex .. "UVAnimationTex" );
	local ApplyButton		= getglobal( ProBtn:GetName() .. "ApplyButton" );
	
	local t_Color = { 255,255,199 }

	NameFont:SetTextColor( 255, 255, 199 );
	TimeFont:SetTextColor( 255, 255, 199 );
	LevelFont:SetTextColor( 255, 255, 199 );
	mustTex:Hide();
	uvTex:Hide();

	if nTaskID ~= nil then
		-- 获取这个任务的状态
		NameFont:SetText( Quest:getQuestName( nTaskID ) )
		LevelFont:SetText( "等级：≥" .. Quest:getQuestLvLimit(nTaskID) )
		TimeFont:SetText("全天");
		local taskRunState = Quest:getQuestRunState( nTaskID )
		ApplyButton:SetText( "接受任务" );

		if Quest:isQuestMust( nTaskID ) then
			mustTex:Show();
		end

		if taskRunState ~= CLIENT_TASK_STAT_NO then
			-- 未完成
			stateTex:SetTexUV( 968, 218, 54, 24 );
			if taskRunState == CLIENT_TASK_STAT_FINI then
				ApplyButton:SetText( "完成任务" )
				ApplyButton:Enable();
			else
				ApplyButton:Disable();
			end			
		elseif Quest:getQusetHaveFinishNumOneDay( nTaskID ) >= Quest:getQuestMaxCanAcceptNumOneDay( nTaskID ) then
			-- 已完成
			stateTex:SetTexUV( 968,268, 54, 24 );
			ApplyButton:Disable();
			t_Color = { 120,120,120 }
			mustTex:Hide();
		else
			-- 进行中
			stateTex:SetTexUV( 968, 243, 54, 24 );			
			ApplyButton:Enable();
		end

		ApplyButton:SetClientUserData( 0,nTaskID );		
		stateTex:Show();
		ProBtn:SetClientUserData(0,nRow);
		ProBtn:Show();
		clanTex:Hide();
		nLineIndex = nLineIndex + 1 ;
	else
		local promotionData = GamePromotion:getPromotionData( nRow );
		if promotionData.m_nID == WORLD_CITY_ACTIVE_HELP_ID_TOLUA and WorldCityManager:GetCityFightEnableStat() ~= 2 then
			return nLineIndex;
		end
		local nTimeIndex = nStartInfo.timeIdx;
		if nStartInfo.startFlag == PROMOTION_PASSED or nStartInfo.startFlag == PROMOTION_NOTTODAY then
			nTimeIndex = 0;
		end
		if PromotionFrameShow2Hour:GetCheckState() == true and nStartInfo.startFlag == PROMOTION_NOTTODAY then
			return nLineIndex;
		end
		if nStartInfo.startFlag == PROMOTION_COMING and PromotionFrameShow2Hour:GetCheckState() == true and
			not GamePromotion:isInTime( promotionData.m_Time[nTimeIndex].m_szStartTime,60*2 ) then
			return nLineIndex;
		end
		local currTypeCheckButton = getglobal( "PromotionFrame_CheckButton"..promotionData.m_nMainRewardType );
		local SelectAllCheckBtn = getglobal("PromotionFrame_SelectAll");
		if SelectAllCheckBtn:GetCheckState() == true or
			currTypeCheckButton == nil or (currTypeCheckButton:GetCheckState() == true) then
			ProBtn:SetClientUserData(0,nRow);
			stateTex:Hide();
			--推荐 设置
			if promotionData.m_nReCommand > 0 then
				stateTex:SetTexUV( 968, 193, 54, 24 );
				stateTex:Show();
			end
			--等级颜色设置
			NameFont:SetText( GamePromotion:getDisPlayName( nRow ) );
			LevelFont:SetText(  "等级：≥".. promotionData.m_nLevel );
			if MainPlayer:getLv() <	promotionData.m_nLevel then
				t_Color = { 255,0,0 };
				LevelFont:SetTextColor( 255, 0, 0 );
			end
			-- 时间颜色设置
			if nStartInfo.startFlag ~= PROMOTION_RUNING then
				t_Color = { 255,0,0 };
				TimeFont:SetTextColor( 255, 0, 0 );
			end
			-- 时间格式设置（会长开启/全天/ 00:00~00:00）
			local OpenTime = ""
			if promotionData.m_nFlag == CLT_AUTO_TEAM_CLAN and promotionData.m_nTimeShowType == 1 and nStartInfo.startFlag == PROMOTION_RUNING then
				OpenTime = "会长开启"
				t_Color = { 255,0,0 };
				TimeFont:SetTextColor( 255, 0, 0 );
			elseif promotionData.m_szName == "智力竞赛" then  --elmer
				OpenTime = promotionData.m_Time[nTimeIndex].m_szStartTime .."开启";
				clanTex:Hide();
			else
				OpenTime = promotionData.m_Time[nTimeIndex].m_szStartTime .. "-" .. promotionData.m_Time[nTimeIndex].m_szEndTime;
				if (promotionData.m_Time[nTimeIndex].m_szStartTime == "" or promotionData.m_Time[nTimeIndex].m_szStartTime == "00:00" ) 
					and promotionData.m_Time[nTimeIndex].m_szEndTime== "24:00" then
					OpenTime = "全天";
				end
				if promotionData.m_nType == PROMOTION_TYPE_WEEKLY then
					local nextOpenWeek = GamePromotion:getFirstOpenWeek( nRow );
					local szWeek = tWeekDayName[nextOpenWeek+1];
					if OpenTime == "全天" then
						OpenTime = szWeek
					else
						OpenTime = szWeek .. OpenTime
					end
				end

				clanTex:Hide();
				--类似 环组次数
				local nFinishCount = GamePromotion:getFinishCount( nRow )
				local nRepeatCount = GamePromotion:getRepeatCount( nRow )
				if nRepeatCount > 0 and nFinishCount >= nRepeatCount then
					stateTex:SetTexUV( 849, 33, 54, 24 );
					stateTex:Show();
				end
				if ( nRepeatCount ~= 0 and nFinishCount >= nRepeatCount ) then
					t_Color = { 120,120,120 }
				end
			end
			--动态活动信息 设置
			if GamePromotion:GetDynActiveInfo( nRow ) >= 0 then
				if GamePromotion:GetDynActiveInfo( nRow ) == 1 then
					t_Color = { 255,255,199 };
					TimeFont:SetTextColor( 255, 255, 199 );
					uvTex:SetUVAnimation(120, true);
					uvTex:Show();
				end
			else
				--特殊处理情报战和工会枢纽
				local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
				if (promotionData.m_nID == CLAN_SPY_ID and t_centerXinShouBtnControl:getIndexByType( CENTER_CLAN_SPY ) ~= 0)
				or (promotionData.m_nID == CLAN_GUARD_ID and t_centerXinShouBtnControl:getIndexByType( CENTER_CLAN_MACHINE_PWORLD ) ~= 0) 
				or (promotionData.m_nID == CLAN_DART_ID and t_centerXinShouBtnControl:getIndexByType( CLAN_DART_DELIVERY ) ~= 0)then
					t_Color = { 255,255,199 };
					TimeFont:SetTextColor( 255, 255, 199 );
					uvTex:SetUVAnimation(120, true);
					uvTex:Show();
				end
			end

			if promotionData.m_nFlag == CLT_AUTO_TEAM_CLAN then
				clanTex:Show();
			end

			TimeFont:SetText( OpenTime );
		
			if promotionData.m_nFlag == CLT_AUTO_TEAM_ACTIVE then
				if GamePromotion:isBattleActive( nRow ) then
					ApplyButton:SetText( "报名" )
				elseif promotionData.m_bShowInAutoTeam then
					ApplyButton:SetText( "副本传送" )
				else
					ApplyButton:SetText( "寻路" )
				end				
			else
				ApplyButton:SetText( "寻路" )
			end
			-- 设置“我很忙”按钮
			SetBusyBtnFormat( promotionData,nLineIndex,nRow);
			if t_Color[1] == 255 and t_Color[2] == 255 and t_Color[3] == 199 then
				ApplyButton:Enable();
			else
				ApplyButton:Disable();
			end
			if not stateTex:IsShown() then
				stateTex:SetTexUV( 968, 168, 54, 24 )
				stateTex:Show();
			end
			--天降横幅活动特殊处理
			if promotionData.m_nID == HEAVEN_PIE_ACTIVE_ID then 
				ApplyButton:Disable();
			else 
				ApplyButton:Enable();
			end
			ProBtn:Show();			
			nLineIndex = nLineIndex+1;	
		end
	end

	if t_Color[1] == 120 and t_Color[2] == 120 and t_Color[3] == 120 then
		NameFont:SetTextColor( t_Color[1], t_Color[2], t_Color[3] );
		TimeFont:SetTextColor( t_Color[1], t_Color[2], t_Color[3] );
		LevelFont:SetTextColor( t_Color[1], t_Color[2], t_Color[3] );
	end
	if nRow == nCurSelIndex then
		bFoundSelectPromption = true;
		PromotionInfoDisCheckAll();
		ProBtn:Checked();
		RefreshDescInfo();
	end
	return nLineIndex;
end

function PromotionFrameShow2HourOnClick()
	if nType ~= PRMOTION_BOSS then
		setNewSortPromition(true);
	end
end

--清空正在进行的不需要显示的项
function ResetRuningPromotion( nNum, nLineIndex, nCurIndex )
	for i = nLineIndex, MAX_PROMOTION_LIST do
		local ProBtn = getglobal( "PromotionInfo" .. i );
		ProBtn:Hide();
	end
	local slider = getglobal("PromotionFrameRuningScrollBar");
	slider:SetValue( nCurIndex-1 );
	slider:SetValueStep(1);
	
	PromotionFrameRunBackGround_ScrollBackGroundTex:Show();
	if nNum-MAX_PROMOTION_LIST-1 > 0 then
		slider:SetMaxValue( nNum-MAX_PROMOTION_LIST-1 );
		slider:Show();
	else
		slider:SetMaxValue( 0 );
		slider:Hide();
	end
	PromotionFrameRuningScrollUpBtn:Show();
	PromotionFrameRuningScrollDownBtn:Show();
end

function DisCheckPromotionBtn()
	PromotionFrame_All:DisChecked();
	PromotionFrame_Today:DisChecked();
	PromotionFrame_Quest:DisChecked();
	PromotionFrame_Clan:DisChecked();
	PromotionFrame_Activity:DisChecked();
	PromotionFrame_WorldBoss:DisChecked();
	WorldBossList:Hide();
end

function PromotionTodayBtn_OnClick()
	DisCheckPromotionBtn();
	ClearAllBusyAnivation();
	PromotionFrame_Today:Checked();
	nType = PRMOTION_DAILY_TASK;
	PromotionFrameDisSelectAll();
	PromotionFrame_SelectAll:Disable();
	PromotionFrame_SelectAll:ChangeNormalTexture( 149, 300, 21, 21 )
	for i = 1,MAX_CHECK_BTN_NUM do
		local checkbtn = getglobal( "PromotionFrame_CheckButton"..i );
		checkbtn:Disable();
		checkbtn:ChangeNormalTexture( 149, 300, 21, 21 )
	end
	setNewSortPromition(true);
end

function PromotionAllBtn_OnClick()
	ClearAllBusyAnivation();
	DisCheckPromotionBtn();
	PromotionFrame_All:Checked();
	nType = PROMOTION_ALL;
	PromotionFrameDisSelectAll();
	PromotionFrame_SelectAll:setCheckState( true )
	PromotionFrame_SelectAll:Enable();
	PromotionFrame_SelectAll:ChangeNormalTexture( 105, 300, 21, 21 )
	for i = 1,MAX_CHECK_BTN_NUM do
		local checkbtn = getglobal( "PromotionFrame_CheckButton"..i );
		checkbtn:Enable();
		checkbtn:ChangeNormalTexture( 105, 300, 21, 21 )
	end
	setNewSortPromition(true);
end

function PromotionQuest_OnClick()
	ClearAllBusyAnivation();
	DisCheckPromotionBtn();
	PromotionFrame_Quest:Checked();
	nType = PRMOTION_ACTIVE;
	PromotionFrameDisSelectAll();
	PromotionFrame_SelectAll:setCheckState( true )
	PromotionFrame_SelectAll:Enable();
	PromotionFrame_SelectAll:ChangeNormalTexture( 105, 300, 21, 21 )
	for i = 1,MAX_CHECK_BTN_NUM do
		local checkbtn = getglobal( "PromotionFrame_CheckButton"..i );
		checkbtn:Enable();
		checkbtn:ChangeNormalTexture( 105, 300, 21, 21 )
	end
	setNewSortPromition(true);
end

function PromotionClan_OnClick()
	ClearAllBusyAnivation();
	DisCheckPromotionBtn();
	PromotionFrame_Clan:Checked();
	nType = PRMOTION_CLAN;
	PromotionFrameDisSelectAll();
	PromotionFrame_SelectAll:setCheckState( true )
	PromotionFrame_SelectAll:Enable();
	PromotionFrame_SelectAll:ChangeNormalTexture( 105, 300, 21, 21 )
	for i = 1,MAX_CHECK_BTN_NUM do
		local checkbtn = getglobal( "PromotionFrame_CheckButton"..i );
		checkbtn:Enable();
		checkbtn:ChangeNormalTexture( 105, 300, 21, 21 )
	end
	setNewSortPromition(true);
end

function PromotionActivityBtn_OnClick()
	ClearAllBusyAnivation();
	DisCheckPromotionBtn();
	PromotionFrame_Activity:Checked();
	nType = PRMOTION_DAYACT;
	PromotionFrameDisSelectAll();
	PromotionFrame_SelectAll:setCheckState( true )
	PromotionFrame_SelectAll:Enable();
	PromotionFrame_SelectAll:ChangeNormalTexture( 105, 300, 21, 21 )
	for i = 1,MAX_CHECK_BTN_NUM do
		local checkbtn = getglobal( "PromotionFrame_CheckButton"..i );
		checkbtn:Enable();
		checkbtn:ChangeNormalTexture( 105, 300, 21, 21 )
	end
	setNewSortPromition(true);
end

function PromotionWorldBoss_OnClick()
	ClearAllBusyAnivation();
	DisCheckPromotionBtn();
	PromotionFrame_WorldBoss:Checked();
	nType = PRMOTION_BOSS;
	PromotionFrameDisSelectAll();
	PromotionFrame_SelectAll:Disable();
	PromotionFrame_SelectAll:ChangeNormalTexture( 149, 300, 21, 21 );
	for i = 1,MAX_CHECK_BTN_NUM do
		local checkbtn = getglobal( "PromotionFrame_CheckButton"..i );
		checkbtn:Disable();
		checkbtn:ChangeNormalTexture( 149, 300, 21, 21 )
	end
	if not WorldBossList:IsShown() then
		WorldBossList:Show();
	end
end

function PromotionInfoDisCheckAll()
	for i=1,MAX_PROMOTION_LIST do
		local runingBtn = getglobal("PromotionInfo"..i);
		if runingBtn ~= nil then
			runingBtn:DisChecked();
		end
	end
end

function PromotionInfo_OnClick()
	local nRow = this:GetClientUserData(0);
	if nRow < 0 then
		return;
	end
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end
	PromotionInfoDisCheckAll();
	this:Checked();
	nCurSelIndex = nRow;
	RefreshDescInfo();
end

function PromotionInfoRewardBtn_OnClick()
	local infoBtn = getglobal( this:GetParent() );
	local nRow = infoBtn:GetClientUserData(0);
	if nRow < 0 then
		return;
	end
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end
	PromotionInfoDisCheckAll();
	infoBtn:Checked();
	nCurSelIndex = nRow;
	RefreshDescInfo();
end

function BossInfoDisCheckAll()
	for i=1,MAX_BOSS_LIST do
		local runingBtn = getglobal("WorldBossListBossInfo"..i);
		if runingBtn ~= nil then
			runingBtn:DisChecked();
		end
	end
end

function BossInfo_OnClick()
	BossInfoDisCheckAll();
	this:Checked();
	nCurSelIndex = this:GetClientID() - 1;
	RefreshDescInfo();
end

function GetPromotionDesc( nRow )
	-- 设置活动时间
	local promotionData = GamePromotion:getPromotionData( nRow );
	local startInfo = GamePromotion:getCurrPromitionStartInfo( nRow );
	local timeIndex = startInfo.timeIdx;
	if timeIndex < 0 then
		timeIndex = 0;
	end
	local szText = "#P#cfffebf活动时间#n\n";
	if promotionData.m_szName == "智力竞赛" then  --elmer
		if startInfo.startFlag == PROMOTION_PASSED then
			for i=1,promotionData.m_nTimeCount do
				szText =szText .. "#P#c938259每天：" ..promotionData.m_Time[i-1].m_szStartTime.."开启#n\n";
			end
		else
			for i=1,timeIndex do
				szText =szText .. "#P#c938259每天：" ..promotionData.m_Time[i-1].m_szStartTime.."开启#n\n";
			end
			szText = szText .. "#P#c93ffd5每天：" ..promotionData.m_Time[timeIndex].m_szStartTime.."开启#n\n";
			for i= timeIndex + 2,promotionData.m_nTimeCount do
				szText =szText .. "#P#c938259每天：" ..promotionData.m_Time[i-1].m_szStartTime.."开启#n\n";
			end
		end
	elseif promotionData.m_nType == PROMOTION_TYPE_DAILY then
		if startInfo.startFlag == PROMOTION_PASSED then
			for i=1,promotionData.m_nTimeCount do
				szText =szText .. "#P#c938259每天：" ..promotionData.m_Time[i-1].m_szStartTime.."-"..promotionData.m_Time[i-1].m_szEndTime.."#n\n";
			end
		else
			if promotionData.m_nTimeCount == 0 or ( (promotionData.m_Time[0].m_szStartTime == "" or promotionData.m_Time[0].m_szStartTime=="00:00") and promotionData.m_Time[0].m_szEndTime == "24:00") then
				szText =szText .. "#P#c93ffd5全天#n\n";
			else
				for i=1,timeIndex do
					szText =szText .. "#P#c938259每天：" ..promotionData.m_Time[i-1].m_szStartTime.."-"..promotionData.m_Time[i-1].m_szEndTime.."#n\n";
				end
				szText = szText .. "#P#c93ffd5每天：" ..promotionData.m_Time[timeIndex].m_szStartTime.."-"..promotionData.m_Time[timeIndex].m_szEndTime.."#n\n";
				for i= timeIndex + 2,promotionData.m_nTimeCount do
					szText =szText .. "#P#c938259每天：" ..promotionData.m_Time[i-1].m_szStartTime.."-" ..promotionData.m_Time[i-1].m_szEndTime .."#n\n";
				end
			end
		end
	elseif promotionData.m_nType == PROMOTION_TYPE_WEEKLY then
		--根据星期的不同 区分开来
		for i=1,7 do
			if GamePromotion:isWeekOpen(nRow,i-1) then
				if startInfo.startFlag == PROMOTION_NOTTODAY or startInfo.startFlag == PROMOTION_PASSED or not GamePromotion:isWeekToday(i-1) then
					for idx=1,promotionData.m_nTimeCount do
						szText =szText .. "#P#c938259"..tWeekDayName[i].."：" ..promotionData.m_Time[idx-1].m_szStartTime.."-"..promotionData.m_Time[idx-1].m_szEndTime.."#n\n";
					end
				else					
					for idx=1,timeIndex do
						szText =szText .. "#P#c938259"..tWeekDayName[i].."：" ..promotionData.m_Time[idx-1].m_szStartTime.."-"..promotionData.m_Time[idx-1].m_szEndTime .."#n\n";
					end
					szText =szText .. "#P#c93ffd5"..tWeekDayName[i].."：" ..promotionData.m_Time[timeIndex].m_szStartTime.."-"..promotionData.m_Time[timeIndex].m_szEndTime .."#n\n";
					for idx=timeIndex + 2,promotionData.m_nTimeCount do
						szText =szText .. "#P#c938259"..tWeekDayName[i].."：" ..promotionData.m_Time[idx-1].m_szStartTime.."-"..promotionData.m_Time[idx-1].m_szEndTime.."#n\n";
					end
				end
			end
		end
	end
	
	--活动描述
	szText = szText .. "#P#cfffebf活动描述#n\n" ;
	szText = szText .."#c938259".. promotionData.m_szDesc .."#n";
	return szText;
end

local t_AreaPic = { [2200] = "YinFengXueYuan.jpg", [3110] = "QiMiaoShenTan.jpg", [3210] = "LaoChengShenXunShi.jpg", [3400] = "ShiSanHaoKuangDong.jpg", 
			[3410] = "XianXueBiHuSuo.jpg", [3510] = "MoMingYiJi.jpg",[3217] = "ShiLuoZhiYu.jpg",[3404] = "YinFengXueYuan.jpg",[4500] = "ShiSanHaoKuangDong.jpg", }
local t_BossScale = { 
			[206865] = { 0.4, 25 }, --康斯坦丁
			[206866] = { 0.8, 40 }, --基尔里斯
			[206867] = { 0.2, 40 }, --野人酋长
			[206868] = { 0.15, 30 }, --霸王龙
			[230013] = { 0.8, 40 }, --塔伯特
			[207001] = { 0.22, 40 }, --武装机器人
			[207006] = { 0.22, 40 }, --巡逻机器人
			[200030] = { 0.3, 30 }, --修斯佩恩
			[207008] = { 0.4, 25 }, --亚历斯特
			[206869] = { 0.4, 25 }, --胖监工
			[207013] = { 0.4, 25 }, --獠牙
			};

function RefreshDescInfo()
	for i=1,MAX_REWARD_ITEM_COUNT do
		local itembtn = getglobal("PromotionFrameRewardItem"..i);
		itembtn:Hide();
	end
	if nCurSelIndex < 0 then
		PromotionFrameProDescBkg:Hide();
		PromotionFrameProDescRich:Hide();
		PromotionFrameRewardBkg:Hide();
		PromotionFrameProDescScrollDownBtn:Hide();
		PromotionFrameProDescScrollUpBtn:Hide();
		PromotionFrameProDescScrollBar:Hide();
		PromotionFrameBossDesc:Hide();
		return;
	else
		if nType == PRMOTION_BOSS then
			PromotionFrameProDescBkg:Hide();
			PromotionFrameProDescRich:Hide();
			PromotionFrameProDescScrollDownBtn:Hide();
			PromotionFrameProDescScrollUpBtn:Hide();
			PromotionFrameProDescScrollBar:Hide();
		else
			PromotionFrameBossDesc:Hide();
		end
	end
	--对今日目标处理
	if nType == PRMOTION_DAILY_TASK then
		local nTaskID =  GamePromotion:getDailyTaskID( nCurSelIndex );
		PromotionFrameProDescBkgNameFont:SetText( Quest:getQuestName( nTaskID ) );
		PromotionFrameProDescBkgCountFont:SetText( "完成次数：" .. Quest:getQusetHaveFinishNumOneDay( nTaskID ) .."/" .. Quest:getQuestMaxCanAcceptNumOneDay( nTaskID ) );
		local szText = ReplaceQuestDesc( Quest:getQuestDialog(nTaskID) .. "\n" .. Quest:getQuestRunTip(nTaskID) )
		PromotionFrameProDescRich:SetText( ReplaceConsumeMoneyDesc(szText,nTaskID,0),255,255,255 );
		-- 设置奖励
		local t_taskAwardId = { 5510001,5510000,5511040 }
		for i = 1, #t_taskAwardId do
			local nItemId = t_taskAwardId[i];     -- 获取任务奖励物品
			
			local itemdef		= getItemDef( nItemId );
			local AwardBtn		= getglobal( "PromotionFrameRewardItem"..i);
			local ItemBoxTex	= getglobal( "PromotionFrameRewardItem"..i.."BoxTexture");
			local ItemTex		= getglobal( "PromotionFrameRewardItem"..i.."IconTexture");
			local IconPath = GetItemEquipIconPath() .. itemdef.IconID..".tga";
			-- 如果没找到物品的图标则使用默认图标显示
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			SetEquipImportantEffectUV( itemdef, AwardBtn:GetName() .."UVAnimationTex" );
			ItemTex:SetTexture( IconPath );
			ItemBoxTex:Show();
			AwardBtn:Show();
			AwardBtn:SetClientUserData( 1, itemdef.ItemID );
		end
	elseif nType == PRMOTION_BOSS then
		local btn = getglobal( "WorldBossListBossInfo" .. nCurSelIndex + 1 );
		local monID = btn:GetClientUserData( 0 );
		if monID == 0 then
			return;
		end
		PromotionFrameBossDescName:SetText( getMonsterName(monID) );
		local modelView = getglobal( "PromotionFrameBossDescModelView" );
		local scale = t_BossScale[monID][1];
		modelView:setPetModel( 0, 10100, GetModePathById( monID ) );
		modelView:setPetScale( 0, scale, scale, scale );
		ModelAngle = 0;
		modelView:setEntityRotate( 0, ModelAngle );
		modelView:setEntityPosition( 0, 0, t_BossScale[monID][2], 0 );
		local worldBossDef = GamePromotion:getWorldBossDef( monID );
		local path = "uires\\TuPianLei\\ShiJieLingZhu\\" .. t_AreaPic[worldBossDef.MapID];
		PromotionFrameBossDescArea:SetTexture( path );
		PromotionFrameBossDesc:Show();
		-- 设置奖励
		for i = 1, MAX_WORLD_BOSS_ICON do
			local nItemId = worldBossDef.AwardIcon[i-1];     -- 获取任务奖励物品
			if nItemId == 0 then
				break;
			end
			
			local itemdef		= getItemDef( nItemId );
			local AwardBtn		= getglobal( "PromotionFrameRewardItem"..i);
			local ItemBoxTex	= getglobal( "PromotionFrameRewardItem"..i.."BoxTexture");
			local ItemTex		= getglobal( "PromotionFrameRewardItem"..i.."IconTexture");
			local IconPath = GetItemEquipIconPath() .. itemdef.IconID..".tga";
			-- 如果没找到物品的图标则使用默认图标显示
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			SetEquipImportantEffectUV( itemdef, AwardBtn:GetName() .."UVAnimationTex" );
			ItemTex:SetTexture( IconPath );
			ItemBoxTex:Show();
			AwardBtn:Show();
			AwardBtn:SetClientUserData( 1, itemdef.ItemID );
		end
		PromotionFrameRewardBkg:Show();
		return;
	else
		local nRow = nCurSelIndex;
		local MainPlayer = ActorMgr:getMainPlayer();
		if MainPlayer == nil then
			return;
		end
		local promotionData = GamePromotion:getPromotionData( nRow );		
		PromotionFrameProDescBkgNameFont:SetText( promotionData.m_szName );

		if promotionData.m_nFlag == CLT_AUTO_TEAM_CLAN then
			PromotionFrameProDescBkgCountFont:SetText( "完成次数：-----" );
		else
			local nFinishCount = GamePromotion:getFinishCount( nRow );
			local nRepeatCount = GamePromotion:getRepeatCount( nRow );
			if nRepeatCount == 0 then
				PromotionFrameProDescBkgCountFont:SetText( "完成次数：-----" );
			else
				PromotionFrameProDescBkgCountFont:SetText( "完成次数：" .. nFinishCount .. "/" .. nRepeatCount );
			end			
		end

		PromotionFrameProDescRich:SetText( GetPromotionDesc(nRow),147,130,89 );
		-- 设置奖励
		local nIndex = 1;
		for i=1,promotionData.m_nRewardItemCount do
			local AwardBtn		= getglobal( "PromotionFrameRewardItem"..nIndex);
			local ItemBoxTex	= getglobal( "PromotionFrameRewardItem"..nIndex.."BoxTexture");
			local ItemTex		= getglobal( "PromotionFrameRewardItem"..nIndex.."IconTexture");
			local itemDef = getItemDef( promotionData.m_RewardItem[i-1] );
			if itemDef ~= nil then
				local IconPath = GetItemEquipIconPath() .. itemDef.IconID..".tga";
				-- 如果没找到物品的图标则使用默认图标显示
				if not IsInExistence( IconPath ) then
					IconPath = DEFAULT_ITEM_PATH;
				end
				SetEquipImportantEffectUV( itemDef, AwardBtn:GetName() .."UVAnimationTex" );
				ItemTex:SetTexture( IconPath );
				ItemBoxTex:Show();
				AwardBtn:Show();
				AwardBtn:SetClientUserData( 1, promotionData.m_RewardItem[i-1] );
				nIndex = nIndex + 1;
			end
		end
	end

	local rich = getglobal( "PromotionFrameProDescRich" );
	local slider  = getglobal( "PromotionFrameProDescScrollBar" );  
	local sliderThumbTex = getglobal( "PromotionFrameProDescScrollBar".."ThumbTexture" );
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
	PromotionFrameProDescBkg:Show();
	PromotionFrameProDescRich:Show();
	PromotionFrameRewardBkg:Show();
	PromotionFrameProDescScrollDownBtn:Show();
	PromotionFrameProDescScrollUpBtn:Show();
end

function BossModelTurnLeftBtn_OnMouseDown()
	local ModelViewer = getglobal("PromotionFrameBossDescModelView");
	ModelAngle = ModelAngle - 5;
	ModelViewer:setEntityRotate(0,ModelAngle);
end

function BossModelTurnRightBtn_OnMouseDown()
	local ModelViewer = getglobal("PromotionFrameBossDescModelView");
	ModelAngle = ModelAngle + 5;
	ModelViewer:setEntityRotate(0,ModelAngle);
end

function PromotionFrameRuningScrollUpBtnOnClick()
	local slider = getglobal("PromotionFrameRuningScrollBar");
	if not slider:IsShown() then
		return;
	end
	local nValue = slider:GetValue() - slider:GetValueStep();
	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end

function PromotionFrameRuningScrollDownBtnOnClick()
	local slider = getglobal("PromotionFrameRuningScrollBar");
	if not slider:IsShown() then
		return;
	end
	local nValue = slider:GetValue() + slider:GetValueStep();
	local nMaxValue = slider:GetMaxValue();
	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

function PromotionFrameRuningOnValueChanged()
	local slider = getglobal("PromotionFrameRuningScrollBar");
	local nValue = slider:GetValue();
	local nMaxValue = slider:GetMaxValue();
	if nValue <= nMaxValue then
		if nType == PRMOTION_BOSS then
			nBossScrollPos = nValue;
			UpdateWorldBossList();
			return;
		elseif nType == PROMOTION_ALL or nType == PRMOTION_BUSY then
			nAllScrollPos = nValue+1;
		elseif nType == PRMOTION_ACTIVE then
			nActiveScrollPos = nValue+1;
		elseif nType == PRMOTION_CLAN then
			nClanScrollPos = nValue+1;
		elseif nType == PRMOTION_DAILY_TASK then
			nDailyDailyTaskScrollPos = nValue+1;
		elseif nType == PRMOTION_DAYACT then
			nActivityScrollPos = nValue+1;
		end
		setNewSortPromition(true);
	end
end

function PromontionFrameProDescScrollUpBtnOnClick()
	local slider = getglobal("PromotionFrameProDescScrollBar");
	if not slider:IsShown() then
		return;
	end
	local nValue = slider:GetValue() - slider:GetValueStep();
	local nMaxValue = slider:GetMaxValue();
	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end

function PromontionFrameProDescScrollDownBtnOnClick()
	local slider = getglobal("PromotionFrameProDescScrollBar");
	if not slider:IsShown() then
		return;
	end
	local nValue = slider:GetValue() + slider:GetValueStep();
	local nMaxValue = slider:GetMaxValue();
	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

function PromontionFrameProDescOnValueChanged()
	local slider = getglobal("PromotionFrameProDescScrollBar");
	local rich	= getglobal( "PromotionFrameProDescRich" );
	rich:SetDispPos( rich:GetStartDispPos() );
	for i = 1, slider:GetValue() do
		rich:ScrollDown();
	end
end

function promotionSetLimit( pworldid,num )
	GamePromotion:setPworldLimit( pworldid,num );
end

function promotionClearLimit()
	GamePromotion:promotionClearLimit();
end

function PromotionInfoApplyButton_OnClick()
	if nType == PRMOTION_DAILY_TASK then
		local nQuestId = this:GetClientUserData( 0 );
		if this:GetText() == "完成任务" then
			Quest:finishActiveQuest( nQuestId );
		else			
			Quest:acceptQuest( 0, nQuestId );
		end
	else
		if this:GetText() == "报名" then
			BattleOpenBtn_OnClick();
		elseif this:GetText() == "寻路" then
			local parent = getglobal( this:GetParent() );
			local nRow = parent:GetClientUserData( 0 )
			local promotionData = GamePromotion:getPromotionData( nRow );			
			local nLength = string.len( promotionData.m_szNpcName );
			local nFirstPos = string.find( promotionData.m_szNpcName, "@@", 1, nLength );
			local nEndPos = string.find( promotionData.m_szNpcName, "#n", 1, nLength );
			local szLinkInfo = string.sub( promotionData.m_szNpcName, nFirstPos+2, nEndPos - 1 );
			
			local bLinkNpc, nNpcId, nTargetMapId = GetNpcPos( szLinkInfo );
			if bLinkNpc == true then
				MapShower:GetMaxMapContal():linkMoveto( nTargetMapId, nNpcId );
				return;
			end
			local nMapId, nMapPosX, nMapPosY = GetLinkPos( szLinkInfo );
			if nMapId ~= 0 then
				MapShower:GetMaxMapContal():linkMoveto( nMapId, nMapPosX, nMapPosY );
			end
		elseif this:GetText() == "副本传送" then
			local nCurMapID				= MapShower:getRealMapId();
			if nCurMapID >= MIN_COPY_ID then				
				ShowMidTips( "在副本外才能进行副本传送" );
				return;
			end
			AutoPworldManager:requestPortalFrame( 0 );
		end
	end
	PromotionInfoClickForBusy( this:GetName() );
end

function BossInfoApplyButton_OnClick()
	local mapid = this:GetClientUserData( 0 );
	if mapid ~= 0 then
		MapShower:GetMaxMapContal():linkMoveto( mapid, this:GetClientUserData( 1 ), this:GetClientUserData( 2 ) );
	end
end

function PromotionFrameAwardBtn_OnClick()
	local index = this:GetClientID();
	local btn = getglobal( "PromotionFrameAwardBackBtn" .. index );
	if btn:IsEnable() then
		GamePromotion:RequestGetDailyAward( index );
	end
end

local tPromotionFrameAwardName = {
									"一阶奖励",
									"二阶奖励",
									"三阶奖励",
									"超级大奖",
								}
function PromotionFrameAwardBtn_OnEnter()
	local id = this:GetClientID();
	local dailyGoalAward = GamePromotion:getDailyGoalAward( id );
	if dailyGoalAward ~= nil then
		local szText = tPromotionFrameAwardName[id] .. "\n";
		if dailyGoalAward.BindGold > 0 then
			szText = szText .. "#G金券 x " .. dailyGoalAward.BindGold .. "#n\n"
		end
		for i=1,4 do
			if dailyGoalAward.ItemID[i-1] > 0 then
				szText = szText .. getItemDef( dailyGoalAward.ItemID[i-1] ).Name .. " x " .. dailyGoalAward.ItemNum[i-1] .."\n";
			end
			SetGameTooltips( this:GetName(),szText )
		end
	end
end

function PromotionFrameQuickFinishBtn_OnClick()
	ShowMidTips("该功能暂未开放")
end

function PromotionFrameScrollBar_OnMouseWheel()
	local slider = nil;
	if isPointInFrame( "PromotionFrameRunBackGround" ) then
		slider = getglobal("PromotionFrameRuningScrollBar");
	elseif isPointInFrame( "PromotionFrameProDescBkg" ) then
		slider = getglobal("PromotionFrameProDescScrollBar"); 
	end

	if slider == nil or (slider ~= nil and not slider:IsShown()) then
		return;
	end

	local nScrollLine	= arg1;
	local absChange		= math.abs( arg1 );
	-- 若向上滚动
	if arg1 > 0 then
		local nValue = slider:GetValue() - absChange * slider:GetValueStep();
		if nValue >= 0 then
			slider:SetValue( nValue );
		else
			slider:SetValue( 0 );
		end
	-- 若向下滚动
	else
		local nMaxValue = slider:GetMaxValue();
		local nValue	= slider:GetValue() + absChange * slider:GetValueStep();

		if nValue <= nMaxValue then
			slider:SetValue( nValue );
		else
			slider:SetValue( nMaxValue );
		end
	end
end

------------------------------------------------------------

local nMaxOfflineSticklength = 110; --离线经验条尺最大长度

--离线经验条坐标
local nOfflineExpPosionx	= 60;
local nOfflineExpPosiony	= -20;
local nOfflineExpbPosionx	= -450;
local nOfflineExpbPosiony	= 60;

local nOfflineButtomMode	= 1;
local nOfflineXpMode		= 1;
local EXPEXPMODE		= 1;--经验模式
local DISAPPMODE		= 2;--消失模式
local MOVEMODE			= 3;--移动模式
local BUTTOMSTICKMODE		= 1;--经验条模式
local BUTTOMEXPMODE		= 2;--底部模式
--运算用变量
--移动过程中的当前位置和目标位置
local nOfflineCurretx,nOfflineCurrenty,nOfflineAimx,nOfflineAimy;
--移动过程中的增幅
local nOfflineIncrx,nOfflineIncry;
--增长速率的反比
local nOfflineIcrscale=800;


function OfflineRemindFrameBtn_OnClick()
	local MainPlayer	= ActorMgr:getMainPlayer();
	MainPlayer:requestAcceptOfflineExp();
end

function OfflineRemindFrame_OnLoad()
	this:RegisterEvent( "GE_OFFLINEEXP_GET" );
	this:RegisterEvent( "GE_GETONEOFFLINE_LOST" );
	this:RegisterEvent( "GE_GETONEOFFLINE_SUCCESS" );
end

function OfflineRemindFrame_OnEvent()
	local MainPlayer	= ActorMgr:getMainPlayer();
	if arg1 == "GE_OFFLINEEXP_GET"   then
		this:Show();
	elseif arg1 == "GE_GETONEOFFLINE_LOST"	then
		ShowMidTips("领取离线经验补偿失败,请重新点击");
	elseif arg1 == "GE_GETONEOFFLINE_SUCCESS" then
		ShowMidTips("领取离线经验补偿成功，当前增长经验"..MainPlayer:getOneOfflineExp().."!");
		this:Hide();
	end
end

function OfflineRemindFrame_OnEnter()
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "点击领取一次性挂机经验奖励！", 
								frame = this:GetParent(), button = "cursor" } );
end
--特殊活动引导
local t_centerActiveGuidBtnType = {
	{ btnType = WHETHER_REAL_BOSS ,			activeId = WHETHER_REAL_BOSS_ID,			path ="uires\\ui\\Za\\MoYingChongChong.tga" },
	{ btnType = COWBOY_BUSY_FINALS ,		activeId = COWBOY_BUSY_FINALS_ID,			path ="uires\\ui\\Za\\NiuZaiHenMang.tga" },
	{ btnType = HEAVEN_PIE_ACTIVITY ,		activeId = HEAVEN_PIE_ACTIVE_ID, 			path ="uires\\ui\\Za\\TianJiangHengFu.tga" },
	{ btnType = HAPPY_CARNIVAL_ACTIVITY ,	activeId = HAPPY_CARNIVAL_GUID_ID ,			path ="uires\\ui\\Za\\HuanLeJiaNianHua.tga" },
	{ btnType = DARK_COMING_ACTIVITY ,		activeId = DARK_COMING_ID ,					path ="uires\\TuPianLei\\PiaoYiTiShiTuBiao\\HeiAWuZhi.tga" },
	{ btnType = LIGHT_SHADOW_FIGHT ,		activeId = LIGHT_SHADOW_FIGHT_ID ,			path ="uires\\TuPianLei\\PiaoYiTiShiTuBiao\\GuangYingZhiZhan.tga" },
	{ btnType = MACHINE_WAR_ACTIVITY ,		activeId = MACHINE_WAR_ACTIVITY_ID ,		path ="uires\\ui\\Za\\JianFengShiKe.tga" },
	{ btnType = SPARTA_PWORLD_ACTIVITY ,	activeId = SPARTA_PWORLD_ACTIVITY_ID ,		path ="uires\\ui\\Za\\MoShiRongYu.tga" },
	{ btnType = LIFE_IS_MOVEMENT_ACTIVITY ,	activeId = LIFE_IS_MOVEMENT_ACTIVITY_ID ,	path ="uires\\TuPianLei\\JiNeng\\JiNeng\\40217.tga" },
	{ btnType = GUCHENG_BATTLE_ACTIVITY ,	activeId = GUCHENG_BATTLE_ACTIVITY_ID ,		path ="uires\\TuPianLei\\WuPin\\5231539.tga" },
	{ btnType = TANK_WAR_ACTIVITY ,			activeId = TANK_WAR_ACTIVITY_ID ,			path ="uires\\TuPianLei\\JiNeng\\JiNeng\\7509.tga" },
}

function SetSpecialActivityGuid( nActiveId )
	if nActiveId == COWBOY_BUSY_FINALS_ID then 
		local mainPlayer = ActorMgr:getMainPlayer();
		if mainPlayer == nil then return end
		local container	= mainPlayer:getContainer();
		--“星光大道入场券”id=5052207 
		if container:getItemCount( 5052207 ) == 0 then
			return;
		end
	end
	--坦克大战，等级限制
	if nActiveId == TANK_WAR_ACTIVITY_ID then
		local mainPlayer = ActorMgr:getMainPlayer();
		if mainPlayer == nil then return end
		if mainPlayer:getLv() < 65 then return end
	end
	for i = 1,#t_centerActiveGuidBtnType do
		if t_centerActiveGuidBtnType[i]["activeId"] == nActiveId then
			local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
			if t_centerXinShouBtnControl:getIndexByType( t_centerActiveGuidBtnType[i]["btnType"]  ) == 0 then
				t_centerXinShouBtnControl:ShowExtraBtn( { itemDefId = 0, Type = t_centerActiveGuidBtnType[i]["btnType"], frame = "PromotionFrame", 
									path =t_centerActiveGuidBtnType[i]["path"], index = 0, nActiveID = nActiveId } );
			end
			break;
		end
	end
end 

function SpecialActiveGuidBtn_OnClick( nActiveId )
	local activeDef = GamePromotion:getPromotionData( nActiveId );
	if not PromotionFrame:IsShown() then
		PromotionFrame:Show();
	end
	PromotionAllBtn_OnClick();
	local nNum = GamePromotion:getAllNum();
	local slider = getglobal("PromotionFrameRuningScrollBar");
	for i=1, nNum do
		local nIndex		= GamePromotion:getAllRow( i - 1 );
		local promotionData = GamePromotion:getPromotionData( nIndex );
		if promotionData.m_nID == nActiveId then
			local sliderVal = i - MAX_PROMOTION_LIST > 0 and i - MAX_PROMOTION_LIST or 0 ;
			slider:SetValue( sliderVal );
			local proBtn = getglobal("PromotionInfo"..i - sliderVal );
			PromotionInfoDisCheckAll();
			proBtn:Checked();
			nCurSelIndex = nActiveId ;
			RefreshDescInfo();
			break;
		end
	end
end

function HappyCarnivalGuidFrameHelperBtn_OnClick()
	--  欢乐嘉年华id
	local frame = getglobal("HappyCarnivalGuidFrame");
	SpecialActiveGuidBtn_OnClick( frame:GetClientUserData( 0 ) ) ;
	HappyCarnivalGuidFrame:Hide();
end

function HappyCarnivalGuidFrameWaitButton_OnClick()
	local frame = getglobal("HappyCarnivalGuidFrame");
	CloseSpecialActivityGuid( frame:GetClientUserData( 0 ) );
end

function HappyCarnivalGuidFrameJoinTrans_OnClick()
	local mainPlayer = ActorMgr:getMainPlayer();
	if  mainPlayer == nil then
		return;
	end
	if mainPlayer:isInStall() then
		ShowMidTips("摆摊状态中，暂时无法传送！");
		return;
	end
	if mainPlayer:isInFight() then
		ShowMidTips("战斗状态中，暂时无法传送！");
		return;
	end
	if TradeFrame:IsShown() then
		ShowMidTips( "交易状态中，暂时无法传送" );
		return;
	end
	if mainPlayer:isDead() then 
		ShowMidTips( "死亡状态中，暂时无法传送" );
		return;
	end 
	local 	rider		= mainPlayer:getRider();
	local	riderId		= rider:getRideItem().RideID;
	local	riderLv		= rider:getRideItem().nLevel;
	local	rideDef 	= getRideDef( riderId, riderLv );
	if mainPlayer:isInRide() and rideDef ~= nil and rideDef.RideOnNum >= 1 then
		ShowMidTips("多人坐骑状态下，暂时无法传送！");
		return ;
	end
	if mainPlayer:isOnMonsterMachine() then 
		ShowMidTips("使用机械中，暂时无法传送");
		return;
	end
	
	local frame = getglobal("HappyCarnivalGuidFrame");
	GamePromotion:RequestTransportToNPC( frame:GetClientUserData( 0 ) );
	CloseSpecialActivityGuid( frame:GetClientUserData( 0 ) );
end

function CloseSpecialActivityGuid( nActiveId )	
	for i = 1,#t_centerActiveGuidBtnType do
		if t_centerActiveGuidBtnType[i]["activeId"] == nActiveId then
			local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
			local nIdx = t_centerXinShouBtnControl:getIndexByType( t_centerActiveGuidBtnType[i]["btnType"] );
			if nIdx > 0 then
				t_centerXinShouBtnControl:eraseByIndex( nIdx );
				t_centerXinShouBtnControl:updateCenterBtns();
			end
		end 
	end 
	if nActiveId == HAPPY_CARNIVAL_GUID_ID 
		or nActiveId == LIGHT_SHADOW_FIGHT_ID
		or nActiveId == MACHINE_WAR_ACTIVITY_ID
		or nActiveId == DARK_COMING_ID
		or nActiveId == LIFE_IS_MOVEMENT_ACTIVITY_ID
		or nActiveId == TANK_WAR_ACTIVITY_ID
		 and HappyCarnivalGuidFrame:IsShown() then
		HappyCarnivalGuidFrame:Hide();
	end 
end 

function HappyCarnivalGuidFrame_OnShow()
	local title 	= getglobal("HappyCarnivalGuidFrameTitle");
	local desFont	= getglobal("HappyCarnivalGuidFrameDescFont1");
	if this:GetClientUserData( 0 ) == HAPPY_CARNIVAL_GUID_ID then
		title:SetText("欢乐嘉年华");
		desFont:SetText("“欢乐嘉年华”活动已开启");
	elseif this:GetClientUserData( 0 ) == LIGHT_SHADOW_FIGHT_ID then
		title:SetText("光影之战");
		desFont:SetText("“光影之战”活动已开启");
	elseif this:GetClientUserData( 0 ) == MACHINE_WAR_ACTIVITY_ID then
		title:SetText("尖峰时刻");
		desFont:SetText("“尖峰时刻”活动已开启");
	elseif this:GetClientUserData( 0 ) == LIFE_IS_MOVEMENT_ACTIVITY_ID then
		title:SetText("生命在于运动");
		desFont:SetText("“生命在于运动”活动已开启");
	elseif this:GetClientUserData( 0 ) == DARK_COMING_ID then
		title:SetText("黑暗来袭");
		desFont:SetText("“黑暗来袭”活动已开启");
	elseif this:GetClientUserData( 0 ) == TANK_WAR_ACTIVITY_ID then
		title:SetText("轰炸大战");
		desFont:SetText("“轰炸大战”活动已开启");
	end
	this:SetPoint("center", "$parent", "center", 0, 0 );
end

function HappyCarnivalGuidFrameTipsButtonOnEnter()
	local frame = getglobal("HappyCarnivalGuidFrame");
	local szText;
	if frame:GetClientUserData( 0 ) == HAPPY_CARNIVAL_GUID_ID then
		szText = "－－点击\"马上参加\"按钮，\n可传送到时间城\"奈皮尔\"处参加活动\n－－点击\"活动助手\"按钮，\n可打开\"活动助手\"面板查看活动介绍";
	elseif frame:GetClientUserData( 0 ) == LIGHT_SHADOW_FIGHT_ID then
		szText = "－－点击\"马上参加\"按钮，\n可传送到时间城\"薇薇安\"处参加活动\n－－点击\"活动助手\"按钮，\n可打开\"活动助手\"面板查看活动介绍";
	elseif frame:GetClientUserData( 0 ) == MACHINE_WAR_ACTIVITY_ID then
		szText = "－－点击\"马上参加\"按钮，\n可传送到时间城\"卡丽熙\"处参加活动\n－－点击\"活动助手\"按钮，\n可打开\"活动助手\"面板查看活动介绍";
	elseif frame:GetClientUserData( 0 ) == LIFE_IS_MOVEMENT_ACTIVITY_ID then
		szText = "－－点击\"马上参加\"按钮，\n可传送到时间城\"卡丽熙\"处参加活动\n－－点击\"活动助手\"按钮，\n可打开\"活动助手\"面板查看活动介绍";
	elseif frame:GetClientUserData( 0 ) == DARK_COMING_ID then
		szText = "－－点击\"马上参加\"按钮，\n可传送到\"银风雪原\"参加活动\n－－点击\"活动助手\"按钮，\n可打开\"活动助手\"面板查看活动介绍";
	elseif frame:GetClientUserData( 0 ) == TANK_WAR_ACTIVITY_ID then
		szText = "－－点击\"马上参加\"按钮，\n可传送到时间城\"马里奥\"处参加活动\n－－点击\"活动助手\"按钮，\n可打开\"活动助手\"面板查看活动介绍";
	end
	SetGameTooltips(this:GetName(),szText)
end


function HeavenPieActiveBegin()
	SetSpecialActivityGuid( HEAVEN_PIE_ACTIVE_ID );
end 
function HeavenPieActiveEnd()
	CloseSpecialActivityGuid( HEAVEN_PIE_ACTIVE_ID )
end

----------------------今日目标奖励-------------------------

function HadGetDailyRewardNum()
	local num = GamePromotion:GetDailyFiniNum() > MAX_DAILY_AWARD_NUM and MAX_DAILY_AWARD_NUM or GamePromotion:GetDailyFiniNum();
	local getWardFlag = GamePromotion:GetDailyAwardFlag();
	for i=1,num do
		if not bitAnd( 2^i,getWardFlag ) then
			return num;
		end
	end

	if num >= MAX_DAILY_AWARD_NUM then
		--存在占领时间城却没有占领据点的情况，此处分开处理
		if GamePromotion:GetDailyFiniNum() >= MAX_REWARD_NUM then
			num = MAX_DAILY_AWARD_NUM + 1;
			if not bitAnd( 2^num,getWardFlag ) then
				return num;
			end
		end

		if WorldCityManager:IsWorldCityKeeper(ActorMgr:getMainPlayer()) then
			num = MAX_DAILY_AWARD_NUM + 2;
			if not bitAnd( 2^num,getWardFlag ) then
				return num;
			end
		end
	end
	return 0;
end

function PromotionDailyTips_OnEnter()
	local szText = "已完成今日目标数：" .. GamePromotion:GetDailyFiniNum() .. "/" .. MAX_REWARD_NUM;--GamePromotion:getDailyTaskNum();
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function DailyRewardFrame_OnLoad()
	this:RegisterEvent( "GE_PROMOTION_DAILY_INFO_CHG" );
	this:RegisterEvent( "GE_STRONG_INFO" );
	this:RegisterEvent( "GE_WORLDCITY_INFO" );
	this:RegisterEvent( "GE_CLAN_DISABLEFRAME" );

	local t_tex = { "YiHaoBaoXiang.tga", "ErHaoBaoXiang.tga", "SanHaoBaoXiang.tga", "SiHaoBaoXiang.tga", "JuDianBaoXiang.tga", "ShiJianChengBaoXiang.tga" };
	for i=1, MAX_REWARD_NUM do
		local tex = getglobal( "DailyRewardFrameRewardEntry" .. i .. "ItemIcon" );
		tex:SetTexture( "uires\\TuPianLei\\JinRiMuBiaoJiangLi\\" .. t_tex[i] );
	end
	DailyRewardFrameScrollBar:Hide();
end

function DailyRewardFrame_OnEvent()
	if arg1 == "GE_PROMOTION_DAILY_INFO_CHG" or arg1 == "GE_WORLDCITY_INFO" or arg1 == "GE_STRONG_INFO" 
		or arg1 == "GE_CLAN_DISABLEFRAME" then
		if DailyRewardFrame:IsShown() then
			UpdateDailyRewardFrame();
		end
	end
end

local t_Need = { "", "", "", "", "", "需占领时间城",};
function UpdateDailyRewardFrame()
	local getWardFlag = GamePromotion:GetDailyAwardFlag();
	for i=1, MAX_REWARD_NUM do
		local name = "DailyRewardFrameRewardEntry" .. i;
		local ui = getglobal( name );
		ui:Show();
		ui:SetClientUserData( 0, i );
		local maxNum = i;
		if i > MAX_DAILY_AWARD_NUM then
			maxNum = i ~= 5 and MAX_DAILY_AWARD_NUM or MAX_REWARD_NUM;
		end
		ui = getglobal( name .. "GetBtn" );
		if maxNum > GamePromotion:GetDailyFiniNum() then
			ui:setInputTransparent( true );
			ui:SetGray( true );
			ui = getglobal( name .. "ItemGetTex" );
			ui:Hide();
			ui = getglobal( name .. "GetBtnUVAnimationTex" );
			ui:Hide();
		else
			if bitAnd( 2^i,getWardFlag ) then
				ui:setInputTransparent( true );
				ui:SetGray( true );
				ui = getglobal( name .. "ItemGetTex" );
				ui:Show();
				ui = getglobal( name .. "GetBtnUVAnimationTex" );
				ui:Hide();
			else
				ui:setInputTransparent( false );
				ui:SetGray( false );
				ui = getglobal( name .. "ItemGetTex" );
				ui:Hide();
				ui = getglobal( name .. "GetBtnUVAnimationTex" );
				ui:SetUVAnimation(80, true);
				ui:Show();
			end
		end

		local font = getglobal( name .. "Font2" );
		font:SetText( "完成" .. maxNum .."个" );
		font = getglobal( name .. "Font3" );
		local finiNum = maxNum < GamePromotion:GetDailyFiniNum() and maxNum or GamePromotion:GetDailyFiniNum();		
		font:SetText( finiNum .. "/" .. maxNum );
		if maxNum > GamePromotion:GetDailyFiniNum() then
			font:SetTextColor( 220, 0, 0 );
		else
			font:SetTextColor( 0, 220, 0 );
		end
		font = getglobal( name .. "Font4" );
		font:SetText( t_Need[i] );
		font:SetTextColor( 0, 220, 0 );

		--特殊处理据点和时间城
		if i > MAX_DAILY_AWARD_NUM then
			if i == 5 then
				if GamePromotion:GetDailyFiniNum() < MAX_REWARD_NUM then
					ui = getglobal( name .. "GetBtn" );
					ui:setInputTransparent( true );
					ui:SetGray( true );
					ui = getglobal( name .. "GetBtnUVAnimationTex" );
					ui:Hide();
					font:SetTextColor( 220, 0, 0 );
					font = getglobal( name .. "Font2" );
					font:SetText( "完成" .. maxNum .."个" );
				end
			else
				if not WorldCityManager:IsWorldCityKeeper(ActorMgr:getMainPlayer()) then
					ui = getglobal( name .. "GetBtn" );
					ui:setInputTransparent( true );
					ui:SetGray( true );
					ui = getglobal( name .. "GetBtnUVAnimationTex" );
					ui:Hide();
					font:SetTextColor( 220, 0, 0 );
				end
			end
		end
	end
end

function DailyRewardFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 0, 0 );
	UpdateDailyRewardFrame();
end

function DailyRewardFrameAwardItem_OnEnter()
	local parent = getglobal( this:GetParent() );
	local id = parent:GetClientUserData( 0 );
	if id == 0 then return end;
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return; end 
	
	local num = id;
	if id > MAX_DAILY_AWARD_NUM then
		num = id ~= MAX_DAILY_AWARD_NUM + 1 and MAX_DAILY_AWARD_NUM or MAX_REWARD_NUM;
	end
	local dailyGoalAward = GamePromotion:getDailyGoalAward( id );
	if dailyGoalAward ~= nil then
		local szText = "完成" .. num .. "个今日目标奖励：\n";
		if dailyGoalAward.BindGold > 0 then
			local nBindGoldNum = math.ceil( mainplayer:getLv() * dailyGoalAward.BindGold / 50000 ) * 5;
			szText = szText .. "#G金券 x " ..nBindGoldNum.. "#n\n"
		end
		for i=1,4 do
			if dailyGoalAward.ItemID[i-1] > 0 then
				szText = szText .. getItemDef( dailyGoalAward.ItemID[i-1] ).Name .. " x " .. dailyGoalAward.ItemNum[i-1] .."\n";
			end
			SetGameTooltips( this:GetName(),szText )
		end
	end
end

function DailyRewardFrameGetItem_OnClick()
	local btn = getglobal( this:GetParent() );
	if btn:IsEnable() then
		GamePromotion:RequestGetDailyAward( btn:GetClientUserData( 0 ) );
	end
end

function DailyRewardFrameScrollBar_OnValueChanged()
	--[[
	local slider = getglobal("DailyRewardFrameScrollBar");
	local nValue = slider:GetValue();
	local nMaxValue = slider:GetMaxValue();

	if nValue <= nMaxValue then
		UpdateDailyRewardFrame( nValue + 1 );
	end
	--]]
end

function DailyRewardFrameScrollDownBtn_OnClick()
	--[[
	local slider = getglobal("DailyRewardFrameScrollBar");
	if not slider:IsShown() then
		return;
	end
	local nValue = slider:GetValue() + slider:GetValueStep();
	local nMaxValue = slider:GetMaxValue();
	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
	--]]
end

function DailyRewardFrameScrollUpBtn_OnClick()
	--[[
	local slider = getglobal("DailyRewardFrameScrollBar");
	if not slider:IsShown() then
		return;
	end
	local nValue = slider:GetValue() - slider:GetValueStep();
	if nValue >= 0 then
		slider:SetValue( nValue );
	end
	--]]
end

-------------------------------世界首领-----------------------------------

local bSort = false;

function IsWorldBoss( monsterID )
	for i=1, #t_WorldBoss do
		if t_WorldBoss[i].id == monsterID then
			return true;
		end
	end
	return false;
end

function GetWorldBossList()
	t_WorldBoss = {};
	local num = GamePromotion:getWorldBossNum();
	for i=1, num do
		local worldBossDef = GamePromotion:getWorldBossDefByIndex( i - 1 );
		if worldBossDef ~= nil then
			table.insert( t_WorldBoss, { id = worldBossDef.MonID, state = 0, freshTime = 0, lv = getMonsterLevel(worldBossDef.MonID) } );
		end
	end
	
	--让光标固定在等级最低的boss位置
	table.sort( t_WorldBoss, 
	function ( lhs, rhs )
		if lhs.lv ~= rhs.lv then
			return lhs.lv < rhs.lv;
		end
	end);
	GamePromotion:requestWorldBossInfo();
end

function SortWorldBossListByLv()
	if table.getn( t_WorldBoss ) <= 2 then
		return;
	end

	table.sort( t_WorldBoss, 
	function ( lhs, rhs )
		if lhs.lv ~= rhs.lv then
			return lhs.lv < rhs.lv;
		else
			--特殊处理开采机BOSS排序
			if lhs.id == 206865 then
				return lhs.id < rhs.id
			end
			if lhs.id == 207001 then
				return lhs.id > rhs.id
			end
		end
	end
	);
end

function SortWorldBossListByTime()
	if table.getn( t_WorldBoss ) <= 2 then
		return;
	end

	table.sort( t_WorldBoss, 
	function ( lhs, rhs )
		if lhs.state ~= rhs.state then
			return lhs.state > rhs.state;
		else
			if lhs.state > 0 then
				return lhs.lv < rhs.lv;
			end
			return lhs.freshTime < rhs.freshTime;
		end
	end
	);
end
function UpdateBossInfo()
	local bossInfo = LoginMgr:getBossInfo();
	for i=1, bossInfo.Num do
		if i > #t_WorldBoss then
			break;
		end
		for j=1, #t_WorldBoss do
			if t_WorldBoss[j].id == bossInfo.One[i-1].MonID then
				t_WorldBoss[j].state = bossInfo.One[i-1].Stat;
				t_WorldBoss[j].freshTime = bossInfo.One[i-1].FreshTime;
				break;
			end
		end
	end
	SortBossListFresh();
end

function UpdateWorldBossList()
	local name = "WorldBossListBossInfo";
	local nCurSel = nCurSelIndex + 1;
	local btn = getglobal( name .. nCurSelIndex + 1 );
	local nLastSel = btn ~= nil and btn:GetClientUserData( 0 ) or 0;
	local nColorType;
	
	for i=1, MAX_BOSS_LIST do
		local index = i + nBossScrollPos;
		if index > #t_WorldBoss then
			btn = getglobal( name .. i );
			btn:Disable();
			btn:SetClientUserData( 0, 0 );
			local font = getglobal( name .. i .. "NameFont" );
			font:SetText( "" );
			font = getglobal( name .. i .. "LevelFont" );
			font:SetText( "" );
			font = getglobal( name .. i .. "TimeFont" );
			font:SetText( "" );
			font = getglobal( name .. i .. "AreaFont" );
			font:SetText( "" );
			btn = getglobal( name .. i .. "ApplyButton" );
			btn:SetClientUserData( 0, 0 );
			btn:SetClientUserData( 1, 0 );
			btn:SetClientUserData( 2, 0 );
			btn:Hide();
		else
			local id = t_WorldBoss[index].id;
			btn = getglobal( name .. i );
			btn:Enable();
			btn:SetClientUserData( 0, id );

			if nLastSel ~= 0 and nLastSel == id then
				nCurSel = i;
			end

			nColorType = COLOR_WHITE;
			local font = getglobal( name .. i .. "TimeFont" );
			local worldBossDef = GamePromotion:getWorldBossDef( id );
			if t_WorldBoss[index].state == 1 then
				font:SetText( "已出现" );
				nColorType = COLOR_WEAK_GREEN;
			else
				local serverTime = getServerTime().sec;
				if worldBossDef.FreshType == 2 then
					if t_WorldBoss[index].freshTime ~= 0 then
						font:SetText( GetFreshTime( t_WorldBoss[index].freshTime ) );						
						if t_WorldBoss[index].freshTime - serverTime < 1800 then
							nColorType = COLOR_YELLOW;
						end
					else
						local killID = GamePromotion:getDieFreshMonsterID( id );
						if killID ~= 0 then
							font:SetText( "需击杀" .. getMonsterName( killID ) );
							nColorType = COLOR_WHITE;
						end
					end
				else
					font:SetText( GetFreshTime( t_WorldBoss[index].freshTime ) );
					if t_WorldBoss[index].freshTime - serverTime < 1800 then
						nColorType = COLOR_YELLOW;
					end
				end
			end

			local color = GetColor()[nColorType];
			font:SetTextColor( color[1], color[2], color[3] );
			font = getglobal( name .. i .. "NameFont" );
			font:SetText( getMonsterName(id) );
			font:SetTextColor( color[1], color[2], color[3] );
			font = getglobal( name .. i .. "LevelFont" );
			font:SetText( t_WorldBoss[index].lv );
			font:SetTextColor( color[1], color[2], color[3] );
			font = getglobal( name .. i .. "AreaFont" );
			font:SetText( LoginMgr:getMapName( worldBossDef.MapID ) );
			font:SetTextColor( color[1], color[2], color[3] );
			btn = getglobal( name .. i .. "ApplyButton" );
			btn:SetClientUserData( 0, worldBossDef.MapID );
			btn:SetClientUserData( 1, worldBossDef.X );
			btn:SetClientUserData( 2, worldBossDef.Y );
			btn:Show();
		end
	end
	
	BossInfoDisCheckAll();
	if nCurSelIndex ~= nCurSel - 1 then
		nCurSelIndex = nCurSel - 1;
		btn = getglobal( name .. nCurSel );
		btn:Checked();
	else
		btn = getglobal( name .. nCurSelIndex + 1 );
		btn:Checked();
		RefreshDescInfo();
	end
end

function GetFreshTime( nTime )
	local szTime = "";
	local serverTime = getServerTime().sec
	if os.date( "%d", nTime ) ~= os.date( "%d", serverTime ) then
		local wDay = GamePromotion:GetWeekDay( nTime );
		szTime = tWeekDayName[wDay + 1] .. os.date("%H", nTime) .. ":" .. os.date("%M", nTime) .. "刷新";
	else
		szTime = os.date("%H", nTime) .. ":" .. os.date("%M", nTime) .. "左右出现";
	end
	return szTime;
end

function WorldBossList_OnShow()
	for i = 1, MAX_PROMOTION_LIST do
		local ProBtn = getglobal( "PromotionInfo" .. i );
		ProBtn:Hide();
	end
	PromotionFrameRuningScrollBar:SetValue( 0 );
	SetScrollBar( "PromotionFrameRuningScrollBar", MAX_BOSS_LIST, #t_WorldBoss );
	nCurSelIndex = 0;
	UpdateWorldBossList();
	GamePromotion:requestWorldBossInfo();
end

function WorldBossList_OnHide()
	bSort = false;
end

function WorldBossListTimeSort_OnClick()
	bSort = not bSort;
	SortBossListFresh();
end

function SortBossListFresh()
	if bSort then
		SortWorldBossListByTime();
	else
		SortWorldBossListByLv();
	end
	if WorldBossList:IsShown() then
		UpdateWorldBossList();
	end
end
local clickFlag = false;
--ApplyBtn"我很忙"标记设置
function SetBusyBtnFormat( ProData,nLIndex,nRow )
	local ProBtn		= getglobal( "PromotionInfo" .. nLIndex );
	local ApplyButton	= getglobal( ProBtn:GetName() .. "ApplyButton" );
	local AniTexture 	= getglobal( ApplyButton:GetName().."UVAnimationTex")
	if  nType ~= PRMOTION_BUSY then 
		return;	
	end
	clickFlag = false;
	if ProData.m_nBusyID > 0  then 
		ApplyButton:SetText("立刻完成");
		ApplyButton:SetClientUserData( 1,ProData.m_nBusyID );
		if CheckBusyConditions( nRow,ProData.m_nBusyID ) then 
			AniTexture:SetUVAnimation(80, true);
			AniTexture:Show();
		else 
			AniTexture:Hide();
		end
	else 
		ApplyButton:SetClientUserData( 1, 0 );
		AniTexture:Hide();
	end
end

function PromotionInfoClickForBusy( btnName )
	local btn 		= getglobal( btnName );
	local infoBtn 	= getglobal( this:GetParent() );
	local nRow 		= infoBtn:GetClientUserData(0);
	local busyId	= btn:GetClientUserData(1);
	clickFlag		= true;
	if nType == PRMOTION_BUSY  and btn:GetText() == "立刻完成" then 
		if nRow < 0 then
			return;
		end
		PromotionInfoDisCheckAll();
		infoBtn:Checked();
		nCurSelIndex = nRow;
		RefreshDescInfo();
		--检查可进行“我很忙”的条件
		if CheckBusyConditions(nRow ,busyId ) then 
			local frame = getglobal("IsBusyFrame");
			frame:SetClientUserData( 0,busyId );
			frame:Show();
		end
	end
end
local PROMOTION_BEACH_ID	= 18;
function CheckBusyConditions(nRow,nBusyId)
	local promotionData = GamePromotion:getPromotionData( nRow );
	local finiTimes		= GamePromotion:getFinishCount( nRow );
	local repeatTimes	= GamePromotion:getRepeatCount( nRow );
	local beachTime		= BusyMgr:GetBeachTime();
	
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return false; end
		
	if  MainPlayer:getLv() < promotionData.m_nLevel then 
		if clickFlag then
			ShowMidTips( "等级必须达到活动要求" );
		end
		return false;
	end 
	if nRow == PROMOTION_BEACH_ID then 
		if  beachTime > 0 then
			if clickFlag then  
				ShowMidTips( "只有未进行的活动可以直接完成" );
			end
			return false;
		end
	else
		if finiTimes >= repeatTimes then 
			if clickFlag then
				ShowMidTips( "该活动已完成，只有未进行的活动可以直接完成" );
			end
			return false;
		elseif finiTimes < repeatTimes and finiTimes >= 1 then 
			if clickFlag then
				ShowMidTips( "该活动已完成一部分，只有未进行的活动可以直接完成" );
			end
			return false;
		end 
	end
	return true;
end

function ClearAllBusyAnivation()
	for i = 1 ,MAX_PROMOTION_LIST do
		local Tex = getglobal("PromotionInfo"..i.."ApplyButtonUVAnimationTex");
		Tex:Hide();
	end
end

function PromotionFrameBusyBtn_OnClick()
	DisCheckPromotionBtn();
	PromotionFrame_All:Checked();
	nType = PRMOTION_BUSY;
	nAllScrollPos = 1;
	PromotionFrameDisSelectAll();
	PromotionFrame_SelectAll:setCheckState( true )
	PromotionFrame_SelectAll:Enable();
	PromotionFrame_SelectAll:ChangeNormalTexture( 105, 300, 21, 21 )
	for i = 1,MAX_CHECK_BTN_NUM do
		local checkbtn = getglobal( "PromotionFrame_CheckButton"..i );
		checkbtn:Enable();
		checkbtn:ChangeNormalTexture( 105, 300, 21, 21 )
	end
	setNewSortPromition(true);
end
function PromotionFrameBusyBtnOnEnter()
	local szText = "点击显示可以快捷完成的活动。\n经验副本和海滩可在活动助手中找到，公会事务、赏金、别针和环组类任务则需要与任务NPC对话选择。\n";
	SetGameTooltips(this:GetName(),szText)
end