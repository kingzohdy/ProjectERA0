---------------------------------------------------死亡之星-地图显示阵营枢纽-----------------------------------------
local t_campIcon = {
	"uires\\DiTuLei\\MinimapIcon\\261201.tga",
	"uires\\DiTuLei\\MinimapIcon\\261201.tga",
	"uires\\DiTuLei\\MinimapIcon\\261201.tga",
}
function SetClanWarCampMonMapInfo( monsterId, nHp, mapid, x, y, campId )
	local szCampName = "公会:"..GameBattle:getClanWarCampName( campId - 1 )
	local szName = "Hp:"..nHp.."\n"..szCampName.."\n"..getMonsterName( monsterId )
	local mainplayer = ActorMgr:getMainPlayer()
	local IconPath = (mainplayer:getCampIdx() == campId) and "uires\\DiTuLei\\MinimapIcon\\261200.tga" or t_campIcon[campId]
	AddClanWarCampMonMinimap(  monsterId, szName, mapid, IconPath, 10*x, 10*y )
	AddClanWarCampMon(  monsterId, szName, mapid, IconPath, 10*x, 10*y )
end
function ClearClanWarCampMonMapInfo()
	ClearClanWarCampMon()
	ClearClanWarCampMonMinimap()
end
---------------------------------------------------死亡之星-玩家信息面板----------------------------------------------
local MAX_CLAN_WAR_SHOW_INFO = 12
local t_ClanWarInfoList = {}
local t_ClanWarInfoRemind = {
	["finish"] = { 
	uv = { w = 24, h = 21, x = 576, y = 78,},
	},
	["unFinish"] = { 
	uv = { w = 25, h = 22, x = 604, y = 78, },
	 },	
}
local sortMap = {
	ClanWarInfoItemTitleName = "name",
	ClanWarInfoItemTitleClan = "clan",
	ClanWarInfoItemTitleRace = "raceId",
	ClanWarInfoItemTitleMaxContinuousKill = "maxContinuousKill",
	ClanWarInfoItemTitleAssist = "assist",
	ClanWarInfoItemTitleResult = "result",
	ClanWarInfoItemTitleGroup = "camp",
}
local sortElement = "result"
local sortOrder = true
local function setRimindInfo( battleSummarys )
	local mainplayer = ActorMgr:getMainPlayer();
	local mainplayerName = mainplayer:getName();
	local tempBattleSummarys = battleSummarys
	--要按照
	SortBattleSummary( tempBattleSummarys, true, "result" )
	for i = 1, table.getn( tempBattleSummarys ) do
		if GetPlayerName( tempBattleSummarys[i].name ) == mainplayerName then
			local kill = tempBattleSummarys[i].kill + tempBattleSummarys[i].assist
			local rank = i;
			local killEnough = true
			local rankEnough = true
			if kill < 25 then
				killEnough = false;
			end
			local  PlayerNum = table.getn( tempBattleSummarys );
			if PlayerNum < 10 then
				if rank ~= 1 and rank == PlayerNum then
					rankEnough = false;
				end
			else
				if PlayerNum >=10 and math.floor(rank * 100 / PlayerNum) > 90 then
					rankEnough = false;
				end
			end
			local szInfo = "完成任一项可获得荣誉、奖章奖励：\n"
			if killEnough then
				szInfo = szInfo.."#c46c800\t1、助攻和击杀总数达到25\n"				
				local uv = t_ClanWarInfoRemind["finish"].uv
				ClanWarInfoFrameremind1:SetTexUV( uv.x, uv.y, uv.w, uv.h )
			else
				szInfo = szInfo.."#cff6464\t1、助攻和击杀总数达到25\n"				
				local uv = t_ClanWarInfoRemind["unFinish"].uv
				ClanWarInfoFrameremind1:SetTexUV( uv.x, uv.y, uv.w, uv.h )
			end
			if rankEnough then
				szInfo = szInfo.."#c46c800\t2、战场积分排名达到前90%"
				local uv = t_ClanWarInfoRemind["finish"].uv
				ClanWarInfoFrameremind2:SetTexUV( uv.x, uv.y, uv.w, uv.h )
			else
				szInfo = szInfo.."#cff6464\t2、战场积分排名达到前90%"
				local uv = t_ClanWarInfoRemind["unFinish"].uv
				ClanWarInfoFrameremind2:SetTexUV( uv.x, uv.y, uv.w, uv.h )
			end
			ClanWarInfoFrameremind:SetText( szInfo, 255, 255, 190 )
		end
	end
end
function SetClanWarPlayerData()
	t_ClanWarInfoList = {}
	for i = 0, GameBattle:getPlayerNum() - 1 do
		table.insert( t_ClanWarInfoList, {
			idx = i,
			name = GameBattle:getPlayerName( i ),
			clan = GameBattle:getPlayerClan( i ),
			raceId = GameBattle:getPlayerCarrer( i ),
			camp = GameBattle:getPlayerCamp( i ),
			maxContinuousKill = GameBattle:getPlayerMaxContinueKill( i ),
			kill = GameBattle:getPlayerKill( i ),
			assist = GameBattle:getPlayerAssist( i ),
			result = GameBattle:getPlayerPolyFraction( i ),
		} )
	end
	SortBattleSummary( t_ClanWarInfoList,sortOrder,sortElement )
end
function SetClanWarPlayerInfo()
	for i = 1, MAX_CLAN_WAR_SHOW_INFO do
		local btn = getglobal( "ClanWarInfoItem"..i )
		btn:Hide()
	end
	--设置scrollbtn最大值
	if table.getn( t_ClanWarInfoList ) > MAX_CLAN_WAR_SHOW_INFO then
		ClanWarInfoFrameScrollBar:Show()
		ClanWarInfoFrameScrollBar:SetMaxValue( table.getn( t_ClanWarInfoList ) - MAX_CLAN_WAR_SHOW_INFO )
	else
		ClanWarInfoFrameScrollBar:Hide()
		ClanWarInfoFrameScrollBar:SetMaxValue(0);
	end
	ClanWarInfoFrameScrollBar:SetValue(math.min(ClanWarInfoFrameScrollBar:GetMaxValue(), ClanWarInfoFrameScrollBar:GetValue()))
	local endIndex = math.min( table.getn( t_ClanWarInfoList ), MAX_CLAN_WAR_SHOW_INFO + ClanWarInfoFrameScrollBar:GetValue() )

	for i = 1 + ClanWarInfoFrameScrollBar:GetValue(), endIndex do
		local btn = getglobal( "ClanWarInfoItem"..( i - ClanWarInfoFrameScrollBar:GetValue() ) )
		btn:Show()
		local NameFont = getglobal( btn:GetName().."Name" )
		NameFont:SetText( GetPlayerName( t_ClanWarInfoList[i].name ) )
		local clanNameFont = getglobal( btn:GetName().."Clan" )
		clanNameFont:SetText( t_ClanWarInfoList[i].clan )
		local raceFont = getglobal( btn:GetName().."Race" )
		raceFont:SetText( GetCareerName(t_ClanWarInfoList[i].raceId) )
		local groupIcon = getglobal( btn:GetName().."GroupIcon" )
		groupIcon:SetTexture( campIdxIcon[t_ClanWarInfoList[i].camp] )
		local maxContinuousKillFont = getglobal( btn:GetName().."MaxContinuousKill" )
		maxContinuousKillFont:SetText( t_ClanWarInfoList[i].maxContinuousKill )
		local assistFont = getglobal( btn:GetName().."Assist" )
		assistFont:SetText( t_ClanWarInfoList[i].assist )
		local resultFont = getglobal( btn:GetName().."Result" )
		resultFont:SetText( t_ClanWarInfoList[i].result )
	end
end
function ClanWarInfoFrame_OnLoad()
	for i = 1, 12 do
		local btn = getglobal("ClanWarInfoItem"..i)
		local t_Tex = {
			btn:GetName() .. "NameTex",btn:GetName() .. "ClanTex",btn:GetName() .. "RaceTex",
			btn:GetName() .. "GroupTex",btn:GetName() .. "MaxContinuousKillTex",btn:GetName() .. "AssistTex",
			btn:GetName() .. "ResultTex",
		};
		for j=1,#t_Tex do
			local tex = getglobal( t_Tex[j] )
			if i%2 == 0 then				
				tex:ChangeTextureTemplate( "WhiteCategoryBkg" )
			else
				tex:ChangeTextureTemplate( "BlackCategoryBkg" )
			end
		end		
	end
end
function ClanWarInfoFrame_OnShow()
	sortElement = "result"
	sortOrder = true
	SetClanWarPlayerInfo()
	setRimindInfo( t_ClanWarInfoList )
end
function ClanWarInfoItemTitle_OnClick()
	local name = this:GetName()
	local sort = sortMap[name]
	if sort then
		if sortElement == sort then
			sortOrder = not sortOrder
		else
			sortElement = sort
			sortOrder = true
		end
		SortBattleSummary( t_ClanWarInfoList,sortOrder,sortElement )
	end
	SetClanWarPlayerInfo()
	setRimindInfo( t_ClanWarInfoList )
end
function SetClanWarProgressTime()
	if BattleStarted then
		ClanWarInfoFrameProgressTimeText:SetText("")
		ClanWarInfoFrameWinFont:SetText("")
		ClanWarInfoPostscriptText:SetText("",255,255,255)
		ClanWarInfoFrameWinBrandInfoFont:SetText("")
		ClanWarInfoFrameBrandTex:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" )
		local player = ActorMgr:getMainPlayer()
		if player then
			local diff = getServerTime().sec - player:getPworldStartTime()
			ClanWarInfoFrameProgressTimeText:SetText(string.format("持续时间 %02d:%02d:%02d", math.floor(diff / 3600), math.floor((diff % 3600) / 60), diff % 60))
		end	
	end
end
function ClanWarInfoFrame_OnUpdate()
	SetClanWarProgressTime()
end
function ClanWarInfoFrame_ScrollUpBtn()
	local slider = ClanWarInfoFrameScrollBar
	if not slider:IsShown() then
		return
	end
	local nValue = slider:GetValue() - slider:GetValueStep()
	if nValue >= slider:GetMinValue() then
		slider:SetValue(nValue)
	end
end
function ClanWarInfoFrame_ScrollDownBtn()
	local slider	= ClanWarInfoFrameScrollBar
	if not slider:IsShown() then
		return
	end
	local nValue	= slider:GetValue() + slider:GetValueStep()
	if nValue <= slider:GetMaxValue() then
		slider:SetValue(nValue)
	end
end
function ClanWarInfoFrame_ScrollBarValueChange()
	SetClanWarPlayerInfo()
end

function SetClanWarEndInfo()
	local nIdx = GameBattle:getPlayerIdx(ActorMgr:getMainPlayer():GetID())
	local nExp = GameBattle:getPlayerExp(nIdx)
	if GameBattle:isWinGame() then
		ClanWarInfoFrameWinFont:SetText("胜利！")
	else
		ClanWarInfoFrameWinFont:SetText("失败！")
	end
	local szText = 	"#cffffbe获得经验："..GameBattle:getPlayerExp(nIdx).."\n#cffffbe获得绑银："..GameBattle:getPlayerBull(nIdx)
	ClanWarInfoPostscriptText:SetText(szText,255,255,255)
	local brandNum = GameBattle:getPlayerRewardNum( nIdx )
	local brandFiniNum = GameBattle:getPlayerFiniNum( nIdx )
	local brandTotalFiniNum = GameBattle:getTotalCanFiniNum()
	local brandInfoFont = getglobal( "ClanWarInfoFrameWinBrandInfoFont" )
	brandInfoFont:SetText( "获得奖章："..brandNum.."/3".."      (每天最多可以获得3个奖章）" )
	local brand = GameBattle:getPlayerReward( nIdx )
	local brandTex = getglobal( "ClanWarInfoFrameBrandTex" )
	brandTex:SetTexture(battleRewardIcon[brand])
	if brandNum > 0 then
		brandInfoFont:Show()
		brandTex:Show()
	else
		brandInfoFont:Hide()
		brandTex:Hide()
	end
end
----------------------------------------------------死亡之星-公会气势面板-------------------------------------------
local CLAN_WAR_MAX_CAMP_NUM = 3
function ClanWarResultFrameProgressBtn_OnEnter()
	local idx = this:GetClientID() - 1
	local szName = GameBattle:getClanWarCampName( idx )	
	if szName == nil or szName == "" then
		return
	end
	SetGameTooltips( this:GetName(), szName )
end
function ClanWarResultFrameHpBtn_OnEnter()
	local idx = this:GetClientID() - 1
	local hp = GameBattle:getClanWarCampHp( idx )	
	SetGameTooltips( this:GetName(), "HP:"..hp )
end
function SetClanWarResultInfo()
	local sumVal = 0
	for i = 1, CLAN_WAR_MAX_CAMP_NUM do
		local val = GameBattle:getClanWarCampVal( i - 1 )
		sumVal = sumVal + ( val > 0 and val or 0 )
	end
	for i = 1, CLAN_WAR_MAX_CAMP_NUM do
		local progresFont = getglobal( "ClanWarResultFrameResultFont"..i )
		local progresTex = getglobal( "ClanWarResultFrameProgressTex"..i )
		local hpProgres = getglobal( "ClanWarResultFrameHp"..i.."Progress" )
		local val = GameBattle:getClanWarCampVal( i - 1 )		
		if val <= 0 or sumVal <= 0 then
			progresTex:SetSize( 0,9 )
			progresFont:SetText( 0 )
		else
			progresTex:SetSize( 121*val/sumVal,9 )
			progresFont:SetText( val )
		end
		local hp = GameBattle:getClanWarCampHp( i - 1 )
		local maxHp = GameBattle:getClanWarCampMaxHp( i - 1 )
		if hp <= 0 or maxHp <= 0 then
			hpProgres:SetSize( 10, 0 )
		else
			hpProgres:SetSize( 10, 14*hp/maxHp )
		end
	end
end
function ClanWarResultFrame_OnShow()
	SetClanWarResultInfo()
end
local function IsClanWarPworld()
	local id = GameBattle:getCurrentBattleId()
	return id == 4 or id == 5
end
function ClanWarResultFrame_OnLoad()
	this:RegisterEvent( "GE_ENTER_PLAYERLOGIN" )
	this:RegisterEvent( "GE_BATTLE_ENTER" )
	this:RegisterEvent( "GE_BATTLE_LEAVE" )
	this:RegisterEvent( "GE_BATTLE_END" )
	this:RegisterEvent( "GE_BATTLE_INFO" )
	this:RegisterEvent( "GE_BATTLE_OBJ_ENTER" )
end
function ClanWarResultFrame_OnEnvent()
	if arg1 == "GE_ENTER_PLAYERLOGIN" then
		ClanWarResultFrame:Hide()
		GameBattle:setClanBattleApplyInfo( 0, 0 )
	elseif arg1 == "GE_BATTLE_ENTER" then
		if IsClanWarPworld() then
			ClanWarMatchOpenApplyFrame:Hide()
			ClanWarMatchFrame:Hide()
			ClanWarPresentInviteFrame:Hide()
			ClanWarResultFrame:Show()
			ClanWarInfoFrameLeave:Show()
			ClanWarInfoFrameCloseButton:Hide()
		end
	elseif arg1 == "GE_BATTLE_LEAVE" then
		if IsClanWarPworld() then
			ClanWarResultFrame:Hide()
			ClanWarInfoFrameCloseButton:Show()
			SetClanWarProgressTime()
			ClanWarInfoFrameLeave:Hide()
			ClanWarInfoFrameCloseButton:Show()
		end
	elseif arg1 == "GE_BATTLE_END" then
		if IsClanWarPworld() then
			ClanWarResultFrame:Hide()
			SetClanWarEndInfo();
			SetClanWarProgressTime()
			ClanWarInfoFrame:Show()
			ClanWarInfoFrameLeave:Hide()
			ClanWarInfoFrameCloseButton:Show()
		end
	elseif arg1 == "GE_BATTLE_INFO" then
		if IsClanWarPworld() then
			SetClanWarPlayerData()
			SetClanWarPlayerInfo()
			SetClanWarProgressTime()
			setRimindInfo( t_ClanWarInfoList )
			SetClanWarResultInfo()
		end
	elseif arg1 == "GE_BATTLE_OBJ_ENTER" then
		if IsClanWarPworld() then
			ClanWarResultFrame:Show()
		end
	end
end
-------------------------------------------------------------------------------------------------------------------
--当战场选择变化或者面板关闭的时候，关闭所有相关的报名和匹配面板
function CloseClanWarMatchFrames()
	ClanWarMatchOpenApplyFrame:Hide()
end
--------------------------------死亡之星匹配-开启报名面板---------------------------------
function OpenClanWarMatchOpenApply( id )
	ClanWarMatchOpenApplyFrame:SetClientUserData( 0, id )
	ClanWarMatchOpenApplyFrame:Show()
end
function ClanWarMatchOpenApplyFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 0, 0 )
	BattleConfirmFrame:Hide()
	ExistenceExpInviteFrame:Hide()
	ClanWarMatchFrame:Hide()
	SetClanWarOpenApplyMatchInfo()
	SetOpenApplyMatchBtnState()
end
function ClanWarMatchOpenApplyFrameOpenBtn_OnClick()
	local id = ClanWarMatchOpenApplyFrame:GetClientUserData( 0 )
	if GameBattle:isClanBattleOpen( id ) then
		ShowMidTips( "您公会已开启过报名，请点击个人报名加入战场" )
		ClanWarMatchOpenApplyFrame:Hide()
		return
	end
	BattleJoin(2)
end
function SetClanWarOpenApplyMatchInfo()
	local battleId = ClanWarMatchOpenApplyFrame:GetClientUserData( 0 )
	local pworldDef = getPworldDef( GameBattle:getBattlePworldById( battleId ) )
	local minNum = pworldDef.MinPlayerNum/3
	local curNum = GameBattle:getClanWarMatchPlayerNum()
	ClanWarMatchOpenApplyFrameInfo:SetText( string.format( "#cffffbe当前公会报名人数：%d \n#c00ff00(公会报名人数超过%d人即可开始匹配对手)", curNum, minNum ),255,255,190 )
end
function SetOpenApplyMatchBtnState()
	if GameBattle:getClanWarMatchStatus() == PWORLD_SVR_MATCH_CAN_NOT_TOLUA then
		ClanWarMatchOpenApplyFrameInviteBtn:Hide()
		ClanWarMatchOpenApplyFrameOpenBtn:Show()
		ClanWarMatchOpenApplyFrameEnterBtn:Hide()
		ClanWarMatchOpenApplyFrameJoinBtn:Hide()
		ClanWarMatchOpenApplyFrameMatchInfo:SetText( "等待报名中.." )
	elseif GameBattle:getClanWarMatchStatus() == PWORLD_SVR_MATCH_CAN_TOLUA then
		ClanWarMatchOpenApplyFrameInviteBtn:Show()
		ClanWarMatchOpenApplyFrameOpenBtn:Hide()
		ClanWarMatchOpenApplyFrameEnterBtn:Hide()
		ClanWarMatchOpenApplyFrameJoinBtn:Hide()
		ClanWarMatchOpenApplyFrameMatchInfo:SetText( "" )
	elseif GameBattle:getClanWarMatchStatus() == PWORLD_SVR_MATCH_DOING_TOLUA then
		ClanWarMatchOpenApplyFrameEnterBtn:Hide()
		ClanWarMatchOpenApplyFrameJoinBtn:Hide()
		ClanWarMatchOpenApplyFrameMatchInfo:SetText( "正在匹配,请稍候.." )
	elseif GameBattle:getClanWarMatchStatus() == PWORLD_SVR_MATCH_DONE_TOLUA then
		ClanWarMatchOpenApplyFrameInviteBtn:Hide()
		ClanWarMatchOpenApplyFrameOpenBtn:Hide()
		--如果没有受到邀请或邀请已过时，显示个人报名按钮，再次报名
		if not ExistenceExp_WaitBtn:IsShown() then
			ClanWarMatchOpenApplyFrameEnterBtn:Hide()
			ClanWarMatchOpenApplyFrameJoinBtn:Show()
		else
			ClanWarMatchOpenApplyFrameEnterBtn:Show()
			ClanWarMatchOpenApplyFrameJoinBtn:Hide()
		end
		ClanWarMatchOpenApplyFrameMatchInfo:SetText( "" )
	end
end

function ClanWarMatchOpenApplyFrame_OnUpdate()
	SetOpenApplyMatchBtnState()
	SetClanWarOpenApplyMatchInfo()
end

function ClanWarMatchOpenApplyFrameInviteBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer()
	local id = ClanWarMatchOpenApplyFrame:GetClientUserData( 0 )
	GameBattle:requestClanWarApplyInvite( id, mainplayer:getName() )
end

function ClanWarMatchOpenApplyFrameEnterBtn_OnClick()
	GameBattle:EnterPworld()
end

function ClanWarMatchOpenApplyFrameJoinBtn_OnClick()
	local id = ClanWarMatchOpenApplyFrame:GetClientUserData( 0 )
	GameBattle:join(id, 0)
end
function ClanWarMatchOpenApplyFrameCancleApplyBtn_OnClick()
	if GameBattle:getClanWarMatchStatus() == PWORLD_SVR_MATCH_DONE_TOLUA then
		ShowMidTips("公会已成功匹配，目前不可取消公会报名")
		return
	end
	GameBattle:ExitQueue( TEAM_JOIN_CLAN_TOLUA )
	ClanWarMatchFrame:Hide()
	ClanWarMatchOpenApplyFrame:Hide()
	GameBattle:setClanBattleApplyInfo( 0, 0 )
end
--------------------------------------------会长邀请参加死亡之星报名----------------------------
function OpenClanWarPresentInvite( id )
	ClanWarPresentInviteFrame:SetClientUserData( 0, id )
	ClanWarPresentInviteFrameInfo:setCenterLine( true )
	local pworldDef = getPworldDef( GameBattle:getBattlePworldById( id ) )
	ClanWarPresentInviteFrameInfo:SetText( string.format("会长发起#G%s#n公会跨服战场报名\n是否确定报名?", pworldDef.Name), 255, 255, 190 )
	ClanWarPresentInviteFrame:Show()
end
function ClanWarMatchOpenApplyFrameOKBtn_OnClick()
	local id = ClanWarPresentInviteFrame:GetClientUserData(0)
	GameBattle:join(id, 0)
	ClanWarPresentInviteFrame:Hide()
end
function ClanWarMatchOpenApplyFrameCancleBtn_OnClick()
	ClanWarPresentInviteFrame:Hide()
end
-----------------------------------------死亡之星报名-个人报名-----------------------------
function OpenClanWarMatch( pworldIdx )
	ClanWarMatchFrame:SetClientUserData( 0, pworldIdx )
	SetMatchInfo()
	SetMatchBtnState()
	ClanWarMatchFrame:Show()
end
function ClanWarMatchFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 0, 0 )
	BattleConfirmFrame:Hide()
	ExistenceExpInviteFrame:Hide()
	ClanWarMatchOpenApplyFrame:Hide()	
	SetMatchInfo()
	SetMatchBtnState()
end
function ClanWarMatchFrame_OnUpdate()
	SetMatchInfo()
	SetMatchBtnState()
end
function SetMatchInfo()
	local pworldDef = getPworldDef( ClanWarMatchFrame:GetClientUserData( 0 ) )
	local minNum = pworldDef.MinPlayerNum/3
	local curNum = GameBattle:getClanWarMatchPlayerNum()
	ClanWarMatchFrameInfo:SetText( string.format( "#cffffbe当前公会报名人数：%d \n#c00ff00(公会报名人数超过%d人即可开始匹配对手)", curNum, minNum ),255,255,190 )
end
function SetMatchBtnState()
	local mainplayer = ActorMgr:getMainPlayer()
	ClanWarMatchFrameClanCancleApplyBtn:Disable()
	if mainplayer:getClanPosition() == CLAN_POS_PRESIDENT or mainplayer:getClanPosition() == CLAN_POS_VICE_PRESIDENT then
		ClanWarMatchFrameClanCancleApplyBtn:Enable()
	end
	if GameBattle:getClanWarMatchStatus() == PWORLD_SVR_MATCH_CAN_NOT_TOLUA then
		ClanWarMatchFrameMatchInfo:SetText( "等待报名中.." )
	elseif GameBattle:getClanWarMatchStatus() == PWORLD_SVR_MATCH_CAN_TOLUA then
		ClanWarMatchFrameMatchInfo:SetText( "" )
	elseif GameBattle:getClanWarMatchStatus() == PWORLD_SVR_MATCH_DOING_TOLUA then
		ClanWarMatchFrameMatchInfo:SetText( "正在匹配,请稍候.." )
	elseif GameBattle:getClanWarMatchStatus() == PWORLD_SVR_MATCH_DONE_TOLUA then
		ClanWarMatchFrameMatchInfo:SetText( "匹配成功,可进入战场" )
		ClanWarMatchFrameClanCancleApplyBtn:Disable()
	end
end
function ClanWarMatchFrameClanCancleApplyBtn_OnClick()
	if GameBattle:getClanWarMatchStatus() == PWORLD_SVR_MATCH_DONE_TOLUA then
		ShowMidTips("公会已成功匹配，目前不可取消公会报名")
		return
	end
	GameBattle:ExitQueue( TEAM_JOIN_CLAN_TOLUA )
	ClanWarMatchFrame:Hide()
	ClanWarMatchOpenApplyFrame:Hide()
end
function ClanWarMatchFrameClanCancleApplyBtn_OnEnter()
	SetGameTooltips( this:GetName(),"您整个公会将离开死亡之星的队列" )
end
function ClanWarMatchFrameCancleApplyBtn_OnClick()
	GameBattle:ExitQueue( )
	ClanWarMatchFrame:Hide()
	ClanWarMatchOpenApplyFrame:Hide()
end
function ClanWarMatchFrameCancleApplyBtn_OnEnter()
	SetGameTooltips( this:GetName(),"您个人将离开死亡之星的队列" )
end
--------------------------------------------------狂沙战场排行榜右键界面--------------------------------------
function ClanWarInfoItemTemplate_OnClick()
	if arg1 == "RightButton" then
		if not GameBattle:isRun() then return end
		local n = this:GetClientID()
		local index = ClanWarInfoFrameScrollBar:GetValue() + n
		local playerName = t_ClanWarInfoList[index].name
		if playerName == nil or playerName == "" then return end
		BattlePlayerInfoRightMenuFrame:Show()
		BattlePlayerInfoRightMenuFrame:SetClientString( playerName )
		BattlePlayerInfoRightMenuFrame:SetPoint( "topleft", "$parent", "topleft", GetCursorPosX(), GetCursorPosY() )
	end
end
