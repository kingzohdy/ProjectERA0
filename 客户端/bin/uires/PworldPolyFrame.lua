local MAX_MONSTER_NUM = 500
local LIGHT_SHADOW_FIGHT_ID		= 42;  --光影之战活动助手Id

local MAX_SHOW_PLAYER_NUM = 12		--排行榜面板最多能显示的玩家信息数

local bNotice = false;
local function UpdatePolyFrame()
	if not GamePoly:isInPolyPworld() then
		PworldPolyFrame:Hide()
		return
	end
	if not PworldPolyFrame:IsShown() then
		PworldPolyFrame:Show()
	end
	local polyData = GamePoly:getDoubleMonsterInfo()
	local scale1 = polyData.MonsterCamp1 / MAX_MONSTER_NUM
	local scale2 = polyData.MonsterCamp2 / MAX_MONSTER_NUM
	PworldPolyFrameNum1:SetText( polyData.MonsterCamp1 )
	PworldPolyFrameNum2:SetText( polyData.MonsterCamp2 )
	PworldPolyFrameProgress1:SetSize( scale1 * 101, 9 )
	PworldPolyFrameProgress2:SetSize( scale2 * 101, 9 )
	PworldPolyFrameProgress1:ChangeTextureTemplate( "GreenBarTexTemplate" )
	PworldPolyFrameProgress2:ChangeTextureTemplate( "GreenBarTexTemplate" )	
	PworldPolyFrameKill:SetText( "击杀怪物数量："..GetSelfMonsterKill() )
	if scale1 > scale2 then
		PworldPolyFrameProgress1:ChangeTextureTemplate( "RedBarTexTemplate" )
	elseif scale1 < scale2 then
		PworldPolyFrameProgress2:ChangeTextureTemplate( "RedBarTexTemplate" )
	end
	if not bNotice and polyData.StartTime > 0 and getServerTime().sec - polyData.StartTime - 60*9 >= 0 then
		bNotice = true
		ShowMidTips( "该活动即将结束" )
	end
end

function GetSelfMonsterKill()
	--获取玩家数据
	local polyData = GamePoly:getDoubleMonsterInfo()
	local playerData = polyData.MonsterEndInfo;
	local mainplayer = ActorMgr:getMainPlayer()
	for i = 1, playerData.RoleNum do
		if mainplayer:getName() == playerData.RoleList[i-1].RoleName then
			return playerData.RoleList[i-1].KillMonster;
		end
	end
	return 0;
end

function PworldPolyFrame_OnLoad()
	this:RegisterEvent( "GE_POLY_MONSTER_UPDATE" )
	this:RegisterEvent( "GE_ENTER_PWORLD" )
	this:RegisterEvent( "GE_LEAVE_PWORLD" )
	--GE_REENTER_PWORLD
end

function PworldPolyFrame_OnEvent()
	if arg1 == "GE_POLY_MONSTER_UPDATE" then
		UpdatePolyFrame()
	elseif arg1 == "GE_ENTER_PWORLD" then
		if GamePoly:isInPolyPworld() then
			GamePoly:resetData()
			UpdatePolyFrame()
			CloseSpecialActivityGuid(LIGHT_SHADOW_FIGHT_ID);
		end
	elseif arg1 == "GE_LEAVE_PWORLD" then
		if GamePoly:isInPolyPworld() then
			GamePoly:resetData()
			this:Hide()
		end
	end
end

function PworldPolyFrame_OnHide()
	bNotice = false
end

function SortPlayerByKillNum( t_playerList )
	table.sort( t_playerList, function( a, b )
		return a.KillMonster > b.KillMonster;
	end)
end

local playerList = {}

function GetLightAndShadowCampName( CampIdx )
	if CampIdx == 1 then
		return "光明"
	elseif CampIdx == 2 then
		return "暗影"		
	end
	return ""
end

function SetPlayersInfo(  t_playerList  )
	local mainplayer = ActorMgr:getMainPlayer()
	--清除之前显示的
	for i = 1, MAX_SHOW_PLAYER_NUM do
		local btn = getglobal( "PworldPolyPlayreInfoFrameInfoPlayer"..i )
		local NameFont = getglobal( btn:GetName().."Name" )
		NameFont:SetText( "" )
		local clanName = getglobal( btn:GetName().."ClanName" )
		clanName:SetText( "" )
		local RaceName = getglobal( btn:GetName().."RaceName" )
		RaceName:SetText( "" )
		local CampName = getglobal( btn:GetName().."CampName" )
		CampName:SetText( "" )
		local KillMonsterNum = getglobal( btn:GetName().."KillMonsterNum" )
		KillMonsterNum:SetText( "" )
	end
	--设置scrollbtn最大值
	if table.getn( t_playerList ) > MAX_SHOW_PLAYER_NUM then
		PworldPolyPlayreInfoFrameInfoScrollBar:Show()
		PworldPolyPlayreInfoFrameInfoScrollBar:SetMaxValue( table.getn( t_playerList ) - MAX_SHOW_PLAYER_NUM )
	else
		PworldPolyPlayreInfoFrameInfoScrollBar:Hide()
		PworldPolyPlayreInfoFrameInfoScrollBar:SetMaxValue(0);
	end
	PworldPolyPlayreInfoFrameInfoScrollBar:SetValue(math.min(PworldPolyPlayreInfoFrameInfoScrollBar:GetMaxValue(), PworldPolyPlayreInfoFrameInfoScrollBar:GetValue()))
	local endIndex = math.min( table.getn( t_playerList ), MAX_SHOW_PLAYER_NUM + PworldPolyPlayreInfoFrameInfoScrollBar:GetValue() )
	for i = 1 + PworldPolyPlayreInfoFrameInfoScrollBar:GetValue(), endIndex do
		local color;
		if mainplayer:getName() == t_playerList[i].RoleName then
			color = { 0, 255, 0, }
		else 
			color = { 190, 190, 90, }
		end
		local btn = getglobal( "PworldPolyPlayreInfoFrameInfoPlayer"..( i - PworldPolyPlayreInfoFrameInfoScrollBar:GetValue() ) )
		local NameFont = getglobal( btn:GetName().."Name" )
		NameFont:SetText( t_playerList[i].RoleName )
		NameFont:SetTextColor( color[1], color[2], color[3] );
		local clanName = getglobal( btn:GetName().."ClanName" )
		clanName:SetText( t_playerList[i].ClanName )
		clanName:SetTextColor( color[1], color[2], color[3] );
		local RaceName = getglobal( btn:GetName().."RaceName" )
		RaceName:SetText( GetCareerName( t_playerList[i].Career ) )
		RaceName:SetTextColor( color[1], color[2], color[3] );
		local CampName = getglobal( btn:GetName().."CampName" )
		CampName:SetText( GetLightAndShadowCampName(t_playerList[i].CampIdx) )
		CampName:SetTextColor( color[1], color[2], color[3] );
		local KillMonsterNum = getglobal( btn:GetName().."KillMonsterNum" )
		KillMonsterNum:SetText( t_playerList[i].KillMonster )
		KillMonsterNum:SetTextColor( color[1], color[2], color[3] );
	end
	--清除自己信息
	PworldPolyPlayreInfoFrameSelfInfoRank:SetText( "" )
	PworldPolyPlayreInfoFrameSelfInfoGetExp:SetText( "" )
	PworldPolyPlayreInfoFrameSelfInfoGetMoney:SetText( "" )
	--设置自己信息
	for i = 1, table.getn( t_playerList ) do
		if mainplayer:getName() == t_playerList[i].RoleName then
			PworldPolyPlayreInfoFrameSelfInfoRank:SetText( "第"..i.."名" )
			PworldPolyPlayreInfoFrameSelfInfoGetExp:SetText( "最后获得经验数量："..t_playerList[i].Exp )
			PworldPolyPlayreInfoFrameSelfInfoGetMoney:SetText( "获得银币："..t_playerList[i].Money )
		end
	end
end

function PworldPolyPlayersInfo_ScrollUpBtn()
	local slider = PworldPolyPlayreInfoFrameInfoScrollBar
	if not slider:IsShown() then
		return
	end
	local nValue = slider:GetValue() - slider:GetValueStep()
	if nValue >= slider:GetMinValue() then
		slider:SetValue(nValue)
	end
	SetPlayersInfo( playerList )
end
function PworldPolyPlayersInfo_ScrollDownBtn()
	local slider	= PworldPolyPlayreInfoFrameInfoScrollBar
	if not slider:IsShown() then
		return
	end
	local nValue	= slider:GetValue() + slider:GetValueStep()
	if nValue <= slider:GetMaxValue() then
		slider:SetValue(nValue)
	end
	SetPlayersInfo( playerList )
end
function PworldPolyPlayersInfo_ScrollBarValueChange()
	SetPlayersInfo( playerList )
end

function LightAndShadowEnd()
	--获取玩家数据
	local polyData = GamePoly:getDoubleMonsterInfo()
	local playerData = polyData.MonsterEndInfo;
	--按照杀怪数量进行排序
	playerList = {}
	for i = 1, playerData.RoleNum do
		onePlayer = {
			MemID = playerData.RoleList[i-1].MemID,
			Career = playerData.RoleList[i-1].Career,
			CampIdx = playerData.RoleList[i-1].CampIdx,
			RoleName = playerData.RoleList[i-1].RoleName,
			ClanName = playerData.RoleList[i-1].ClanName,
			KillMonster = playerData.RoleList[i-1].KillMonster,
			Exp = playerData.RoleList[i-1].Exp,
			Money = playerData.RoleList[i-1].Money,
		}
		table.insert( playerList, onePlayer )
	end
	--SortPlayerByKillNum( playerList )
	--设置显示列表
	SetPlayersInfo( playerList );
	PworldPolyPlayreInfoFrame:Show()
end
----------------------------------------------------------------尖峰时刻---------------------------------------------------------------------------------------
local MACHINE_WAR_ACTIVITY_ID = 45

local HUMAN_CAMP_ID = 1
local TITAN_CAMP_ID = 2
local MONSTER_CAMP_ID = 3

local t_MachinWarRemind = {
	"力量微弱,但移动迅速。只要坚持不死,就会获得海量经验!只要坚持到底，就会赢得胜利!",
	"目标:#c00ff00人类、机械怪兽#cffffbe。杀死他们,获得经验,你的阵营也会壮大。仔细研究你的技能,小心巨兽首领!",
	"目标:#c00ff00人类、机械巨人#cffffbe。杀死他们,获得经验,你的阵营也会壮大。善于利用你的技能,小心巨人首领!",
}

function MachineWarInfoFrame_onLoad()
	this:RegisterEvent( "GE_POLY_MACHINE_WAR_UPDATE" )
	this:RegisterEvent( "GE_ENTER_PWORLD" )
	this:RegisterEvent( "GE_LEAVE_PWORLD" )
	this:RegisterEvent( "GE_POLY_MACHINE_WAR_SLEF_CAMP_CHANGE" )
end
function MachineWarInfoFrame_onShow()
	MachineWarInfoFrameHumanNum:SetSize( 66*0, 9 )
	MachineWarInfoFrameTiTanNum:SetSize( 66*0, 9 )
	MachineWarInfoFrameMonsterNum:SetSize( 66*0, 9 )
end
local t_MachineWarHideOpt = {
	GAME_OPT_SHOW_ALLPLAYER_AND_PET_NAME,
	GAME_OPT_SHOW_ALLPLAYER_TEAM_DESIGNATION_NAME,
	GAME_OPT_SHOW_MAIN_CLAN_NAME,
	GAME_OPT_SHOW_OTHERPLAYER_CLAN_NAME,
}
function MachineWarInfoFrame_onEvent()
	if arg1 == "GE_POLY_MACHINE_WAR_UPDATE" then
		updateMachineWarInfoFrame()
	elseif arg1 == "GE_POLY_MACHINE_WAR_SLEF_CAMP_CHANGE" then
		local machineWarInfo = GamePoly:getMachineWarInfo();
		if machineWarInfo.SelfCamp > 0 and machineWarInfo.SelfCamp <= table.getn( t_MachinWarRemind ) then
			PworldRemindInfo:Show()
			SetPworldRemindInfo( t_MachinWarRemind[machineWarInfo.SelfCamp] )
		else
			PworldRemindInfo:Hide()
		end
	elseif arg1 == "GE_ENTER_PWORLD" then
		if GamePoly:isInPloyPworld_MachineWar() then
			--清空阵营boss位置信息
			ClearMachineWarBossMinimap()
			ClearMachineWarBoss()
			
			GamePoly:resetMachineWar()
			
			updateMachineWarInfoFrame()
			this:Show()
			
			CloseSpecialActivityGuid( MACHINE_WAR_ACTIVITY_ID )
		end
	elseif arg1 == "GE_LEAVE_PWORLD" then
		if GamePoly:isInPloyPworld_MachineWar() then
			--清空阵营boss位置信息
			ClearMachineWarBossMinimap()
			ClearMachineWarBoss()
			
			GamePoly:resetMachineWar()
			this:Hide();
			MachineWarReadyTimeFrame:Hide()
			nLeaveTime = 0;
			PworldRemindInfo:Hide();
		end
	end
end

function MachineWarInfoFrame_onUpdate()
	updateMachineWarInfoFrame()
end

--更新除了人类阵营外的两个阵营首领的位置
function updateBossMachineWarPos()
	--清空之前的位置信息
	ClearMachineWarBossMinimap()
	ClearMachineWarBoss()
	local MaxMapControl	= MapShower:GetMaxMapContal();
	--巨人首领位置
	local titanPosX, titanPosY = GamePoly:getMachineWarTitanPosX(), GamePoly:getMachineWarTitanPosY()
	AddMachineWarBossMinimap( "机械巨人首领", MaxMapControl:getMapID(), "uires\\ui\\Za\\JiXieJuRenBoss.tga", 10*titanPosX, 10*titanPosY )	
	AddMachineWarBoss( "机械巨人首领", MaxMapControl:getMapID(), "uires\\ui\\Za\\JiXieJuRenBoss.tga", 10*titanPosX, 10*titanPosY )	
	--机械首领位置
	local machinePosX, machinePosY = GamePoly:getMachineWarMachinePosX(), GamePoly:getMachineWarMachinePosY()
	AddMachineWarBossMinimap( "机械怪物首领", MaxMapControl:getMapID(), "uires\\ui\\Za\\JiXieGuaiWuBoss.tga", 10*machinePosX, 10*machinePosY )
	AddMachineWarBoss( "机械怪物首领", MaxMapControl:getMapID(), "uires\\ui\\Za\\JiXieGuaiWuBoss.tga", 10*machinePosX, 10*machinePosY )
end

function updateMachineWarInfoFrame()	
	local machineWarInfo = GamePoly:getMachineWarInfo();
	local totalPlayerzNum = machineWarInfo.Human + machineWarInfo.Titan + machineWarInfo.Monster;
	if totalPlayerzNum > 0 then
		local humanScale = machineWarInfo.Human/totalPlayerzNum
		if humanScale > 0.5 then
			MachineWarInfoFrameHumanNum:ChangeTextureTemplate( "GreenBarTexTemplate" )
		elseif humanScale > 0.25 then
			MachineWarInfoFrameHumanNum:ChangeTextureTemplate( "YellowBarTexTemplate" )
		else
			MachineWarInfoFrameHumanNum:ChangeTextureTemplate( "RedBarTexTemplate" )
		end
		MachineWarInfoFrameHumanNum:SetSize( 66*humanScale, 9 )
		local titanScale = machineWarInfo.Titan/totalPlayerzNum
		if titanScale > 0.5 then
			MachineWarInfoFrameTiTanNum:ChangeTextureTemplate( "GreenBarTexTemplate" )
		elseif titanScale > 0.25 then
			MachineWarInfoFrameTiTanNum:ChangeTextureTemplate( "YellowBarTexTemplate" )
		else
			MachineWarInfoFrameTiTanNum:ChangeTextureTemplate( "RedBarTexTemplate" )
		end
		MachineWarInfoFrameTiTanNum:SetSize( 66*titanScale, 9 )
		local monsterScale = machineWarInfo.Monster/totalPlayerzNum
		if monsterScale > 0.5 then
			MachineWarInfoFrameMonsterNum:ChangeTextureTemplate( "GreenBarTexTemplate" )
		elseif monsterScale > 0.25 then
			MachineWarInfoFrameMonsterNum:ChangeTextureTemplate( "YellowBarTexTemplate" )
		else
			MachineWarInfoFrameMonsterNum:ChangeTextureTemplate( "RedBarTexTemplate" )
		end
		MachineWarInfoFrameMonsterNum:SetSize( 66*monsterScale, 9 )
	end
	MachineWarInfoFrameGetExp:SetText("经验奖励(exp)："..machineWarInfo.ToTalExp)
	
	if machineWarInfo.SelfCamp == HUMAN_CAMP_ID then
		MachineWarInfoFrameHumanChoseBK:Show()
		MachineWarInfoFrameTiTanChoseBK:Hide()
		MachineWarInfoFrameMonsterChoseBK:Hide()
	elseif machineWarInfo.SelfCamp == TITAN_CAMP_ID then
		MachineWarInfoFrameHumanChoseBK:Hide()
		MachineWarInfoFrameTiTanChoseBK:Show()
		MachineWarInfoFrameMonsterChoseBK:Hide()
	elseif machineWarInfo.SelfCamp == MONSTER_CAMP_ID then
		MachineWarInfoFrameHumanChoseBK:Hide()
		MachineWarInfoFrameTiTanChoseBK:Hide()
		MachineWarInfoFrameMonsterChoseBK:Show()
	else
		MachineWarInfoFrameHumanChoseBK:Hide()
		MachineWarInfoFrameTiTanChoseBK:Hide()
		MachineWarInfoFrameMonsterChoseBK:Hide()
	end
	
	local player = ActorMgr:getMainPlayer()
	local leaveTime = GamePoly:getMachineWarTimeOut()*60 - ( getServerTime().sec - player:getPworldStartTime() )
	if leaveTime < 0 then
		leaveTime = 0
		PworldRemindInfo:Hide();
	end
	
	--设置活动开始或结束的倒计时
	if machineWarInfo.StartTime - getServerTime().sec > 0 then
		local timer = machineWarInfo.StartTime - getServerTime().sec
		MachineWarInfoFrameTime:SetText( "				活动准备中" )
		MachineWarReadyTimeFrame:Show()
		local timeProgress = 0
		if GamePoly:getMachineWarReadyTime() > 0 then
			timeProgress = math.ceil(timer)/GamePoly:getMachineWarReadyTime()
		end
		MachineWarReadyTimeFrameTimeProgress:SetSize( 300*timeProgress, 9 )
		MachineWarReadyTimeFrameTime:SetText( timer.."/".. GamePoly:getMachineWarReadyTime() )
		MachineWarReadyTimeFrameTitle:SetText( "活动准备剩余时间" )
	else
		MachineWarReadyTimeFrame:Hide()
		MachineWarInfoFrameTime:SetText(string.format("活动剩余时间：%ds", math.ceil(leaveTime) ))
	end
	
	if leaveTime <= 30 then
		MachineWarReadyTimeFrame:Show()
		if leaveTime <= 0 then
			MachineWarReadyTimeFrame:Hide()
			return;
		end
		local timeProgress = math.ceil(leaveTime)/30
		MachineWarReadyTimeFrameTimeProgress:SetSize( 300*timeProgress, 9 )
		MachineWarReadyTimeFrameTime:SetText( math.ceil(leaveTime).."/30" )
		MachineWarReadyTimeFrameTitle:SetText( "活动结束倒计时" )
	end
end

--机械改造胜利确认面板
function MachineWarResult( nFlag, nEndExp, nTotalExp )
	MachineWarResultFrame:Show();
	local szInfo = ""
	if nFlag == 1 then
		MachineWarResultFrameResult:SetText( "胜利" )
		szInfo = string.format( "#cffffbe由于您的坚持与努力，您所在的阵营获得最终胜利！\n共获得经验奖励:#c00ff00%d点#cffffbe(其中阵营胜利奖励:#c00ff00%d点#cffffbe)\n生存奖章：100", nTotalExp, nEndExp )
	else
		MachineWarResultFrameResult:SetText( "失败" )
		szInfo = string.format( "#cffffbe很遗憾，您所在的阵营失败了，下次继续努力哟！\n共获得经验奖励:#c00ff00%d点#cffffbe(其中阵营失败勉励:#c00ff00%d点#cffffbe)\n生存奖章：100", nTotalExp, nEndExp )
	end
	MachineWarResultFrameInfo:SetText( szInfo, 255, 255, 190 );
end
function MachineWarResultFrame_onLoad()
	MachineWarResultFrameInfo:setCenterLine(true)
end
--提示面板
function SetPworldRemindInfo( szInfo )
	PworldRemindInfoWords:SetText( szInfo, 255,255,190 )
end

------------------------------------------------------------坦克大战-右上角统计小面板------------------------------------------------------------
local TANK_WAR_ACTIVITY_ID		= 64;
local t_TankWarCampTex = {
	--蓝色
	{ path = "uires\\ZhuJieMian\\3.dds", x = 948, y = 737, w = 15, h = 15, },
	--红色
	{ path = "uires\\ZhuJieMian\\3.dds", x = 932, y = 737, w = 15, h = 15, },
}
local t_CampName = { "联邦军", "帝国军", }
local MAX_TANK_CAMP_KILL = 200
function TankWarFrameCheckResult_OnClick()
	TankWarPlayreInfoFrame:SetPoint( "center", "UIClient", "center", 0, 0 )
	TankWarPlayreInfoFrame:Show()
end
function TankWarFrame_OnLoad()
	this:RegisterEvent( "GE_ENTER_MAP" )
	this:RegisterEvent( "GE_LEAVE_PWORLD" )
	this:RegisterEvent( "GE_POLY_TANK_UPDATE" )
end
function TankWarFrame_OnEvent()
	if arg1 == "GE_ENTER_MAP" then
		if MapShower:getRealMapId() >= MIN_COPY_ID then
			local pworldDef = getPworldDef( MapShower:getRealMapId()/100 )
			if pworldDef.PolyType == PWORLD_POLY_TANK_TOLUA then
				--进入副本后关闭下方飘移提示
				CloseSpecialActivityGuid( TANK_WAR_ACTIVITY_ID )
				TankWarFrame:Show()
				GamePoly:setTankContineKill( 0 )
				StrongWarContineKillFrame:Show()
				StrongWarContineKillFrame:SetClientUserData(1,TANK_CONTINE_KILL_TYPE);
			else
				StrongWarContineKillFrame:Hide()
				TankWarFrame:Hide()
			end
		else
			StrongWarContineKillFrame:Hide()
			TankWarFrame:Hide()
		end
	elseif arg1 == "GE_POLY_TANK_UPDATE" then
		GetTankPlayers()
		SetTankPlayerInfo()
		SetTankWarInfo()
	end
end
function TankWarFrame_OnShow()
	SetTankWarInfo()
end
function SetTankWarInfo()
	local selfCamp = GamePoly:getSelfCampInTankWar()
	if  t_TankWarCampTex[selfCamp] ~= nil then
		local uv = t_TankWarCampTex[selfCamp]		
		TankWarFrameSelfCampTex:Show()
		TankWarFrameSelfCampTex:SetTexUV( uv.x, uv.y, uv.w, uv.h )
		TankWarFrameSelfCampName:SetText( t_CampName[selfCamp] )
	else
		TankWarFrameSelfCampTex:Hide()
		TankWarFrameSelfCampName:SetText( "" )
	end
	
	local campKill1Scale = GamePoly:getTankKillCamp1()/MAX_TANK_CAMP_KILL
	local campKill2Scale = GamePoly:getTankKillCamp2()/MAX_TANK_CAMP_KILL
	TankWarFrameProgress1:SetSize( campKill1Scale * 101, 9 )
	TankWarFrameNum1:SetText( GamePoly:getTankKillCamp1() .."/".. MAX_TANK_CAMP_KILL )
	TankWarFrameProgress2:SetSize( campKill2Scale * 101, 9 )
	TankWarFrameNum2:SetText( GamePoly:getTankKillCamp2() .."/".. MAX_TANK_CAMP_KILL )
end
---------------------------------------------------------坦克大战战绩统计面板---------------------------------------------------
local t_TankPlayer = {}
local MAX_SHOW_TANK_PLAYER_NUM = MAX_SHOW_PLAYER_NUM --坦克大战信息面板，一版显示的最多人数
function TankWarEnd()
	GetTankPlayers()
	SetTankPlayerInfo()
	TankWarPlayreInfoFrame:Show()
	GamePoly:setTankContineKill( 0 )
end
function GetTankPlayers()
	t_TankPlayer = {}
	for i = 1, GamePoly:getTankPlayerNum() do
		local onePlayer = GamePoly:getTankPlayerByIdx( i-1 )
		table.insert( t_TankPlayer, onePlayer )
	end
	SortTankPlayerByKillNum( t_TankPlayer )
end
function SortTankPlayerByKillNum( t_TankPlayer )
	table.sort( t_TankPlayer, function( a, b )
		return a.KillMonster > b.KillMonster;
	end)
end
function SetTankPlayerInfo()
		
	local mainplayer = ActorMgr:getMainPlayer()
	--清空上一次显示
	for i = 1, MAX_SHOW_TANK_PLAYER_NUM do
		local btn = getglobal( "TankWarPlayreInfoFrameInfoPlayer"..i )
		local btnName = getglobal( btn:GetName().."Name" )
		btnName:SetText( "" )
		local btnClan = getglobal( btn:GetName().."ClanName" )
		btnClan:SetText( "" )
		local btnRace = getglobal( btn:GetName().."RaceName" )
		btnRace:SetText( "" )
		local btnCamp = getglobal( btn:GetName().."CampName" )
		btnCamp:SetText( "" )
		local btnKill = getglobal( btn:GetName().."KillMonsterNum" )
		btnKill:SetText( "" )
	end
	TankWarPlayreInfoFrameSelfInfoRank:SetText( "" )
	TankWarPlayreInfoFrameSelfInfoGetExp:SetText( "" )
	TankWarPlayreInfoFrameSelfInfoGetMoney:SetText( "" )
	
	--设置scrollbtn最大值
	if table.getn( t_TankPlayer ) > MAX_SHOW_TANK_PLAYER_NUM then
		TankWarPlayreInfoFrameInfoScrollBar:Show()
		TankWarPlayreInfoFrameInfoScrollBar:SetMaxValue( table.getn( t_TankPlayer ) - MAX_SHOW_TANK_PLAYER_NUM )
	else
		TankWarPlayreInfoFrameInfoScrollBar:Hide()
		TankWarPlayreInfoFrameInfoScrollBar:SetMaxValue(0);
	end
	TankWarPlayreInfoFrameInfoScrollBar:SetValue(math.min(TankWarPlayreInfoFrameInfoScrollBar:GetMaxValue(), TankWarPlayreInfoFrameInfoScrollBar:GetValue()))
	local endIndex = math.min( table.getn( t_TankPlayer ), MAX_SHOW_TANK_PLAYER_NUM + TankWarPlayreInfoFrameInfoScrollBar:GetValue() )
	for i = 1 + TankWarPlayreInfoFrameInfoScrollBar:GetValue(), endIndex do
		local color = { 255,255,190, }
		if t_TankPlayer[i].RoleName == mainplayer:getName() then
			color = { 0,255,0, }
		end
		local btn = getglobal( "TankWarPlayreInfoFrameInfoPlayer"..( i - TankWarPlayreInfoFrameInfoScrollBar:GetValue() ) )
		local btnName = getglobal( btn:GetName().."Name" )
		btnName:SetText( t_TankPlayer[i].RoleName )
		btnName:SetTextColor( color[1], color[2], color[3] )
		local btnClan = getglobal( btn:GetName().."ClanName" )
		btnClan:SetText( t_TankPlayer[i].ClanName )
		btnClan:SetTextColor( color[1], color[2], color[3] )
		local btnRace = getglobal( btn:GetName().."RaceName" )
		btnRace:SetText( GetCareerName( t_TankPlayer[i].Career ) )
		btnRace:SetTextColor( color[1], color[2], color[3] )
		local btnCamp = getglobal( btn:GetName().."CampName" )
		local CampIdx = t_TankPlayer[i].CampIdx
		btnCamp:SetText( t_CampName[CampIdx] )
		btnCamp:SetTextColor( color[1], color[2], color[3] )
		local btnKill = getglobal( btn:GetName().."KillMonsterNum" )
		btnKill:SetText( t_TankPlayer[i].KillMonster )
		btnKill:SetTextColor( color[1], color[2], color[3] )
	end
	
	--设置自己的战绩
	for i = 1, table.getn( t_TankPlayer ) do
		if t_TankPlayer[i].RoleName == mainplayer:getName() then
			color = { 0,255,0, }
			TankWarPlayreInfoFrameSelfInfoRank:SetText( "第"..i.."名" )
			TankWarPlayreInfoFrameSelfInfoGetExp:SetText( "获得经验：".. t_TankPlayer[i].Exp )
			TankWarPlayreInfoFrameSelfInfoGetMoney:SetText( "获得银币：".. t_TankPlayer[i].Money )
		end
	end
end
function TankWarPlayersInfo_ScrollUpBtn()
	local slider = TankWarPlayreInfoFrameInfoScrollBar
	if not slider:IsShown() then
		return
	end
	local nValue = slider:GetValue() - slider:GetValueStep()
	if nValue >= slider:GetMinValue() then
		slider:SetValue(nValue)
	end
	SetTankPlayerInfo()
end
function TankWarPlayersInfo_ScrollDownBtn()
	local slider	= TankWarPlayreInfoFrameInfoScrollBar
	if not slider:IsShown() then
		return
	end
	local nValue	= slider:GetValue() + slider:GetValueStep()
	if nValue <= slider:GetMaxValue() then
		slider:SetValue(nValue)
	end
	SetTankPlayerInfo()
end
function TankWarPlayersInfo_ScrollBarValueChange()
	SetTankPlayerInfo()
end
