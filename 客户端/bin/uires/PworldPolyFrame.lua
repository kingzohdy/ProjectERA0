local MAX_MONSTER_NUM = 500
local LIGHT_SHADOW_FIGHT_ID		= 42;  --��Ӱ֮ս�����Id

local MAX_SHOW_PLAYER_NUM = 12		--���а�����������ʾ�������Ϣ��

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
	PworldPolyFrameKill:SetText( "��ɱ����������"..GetSelfMonsterKill() )
	if scale1 > scale2 then
		PworldPolyFrameProgress1:ChangeTextureTemplate( "RedBarTexTemplate" )
	elseif scale1 < scale2 then
		PworldPolyFrameProgress2:ChangeTextureTemplate( "RedBarTexTemplate" )
	end
	if not bNotice and polyData.StartTime > 0 and getServerTime().sec - polyData.StartTime - 60*9 >= 0 then
		bNotice = true
		ShowMidTips( "�û��������" )
	end
end

function GetSelfMonsterKill()
	--��ȡ�������
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
		return "����"
	elseif CampIdx == 2 then
		return "��Ӱ"		
	end
	return ""
end

function SetPlayersInfo(  t_playerList  )
	local mainplayer = ActorMgr:getMainPlayer()
	--���֮ǰ��ʾ��
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
	--����scrollbtn���ֵ
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
	--����Լ���Ϣ
	PworldPolyPlayreInfoFrameSelfInfoRank:SetText( "" )
	PworldPolyPlayreInfoFrameSelfInfoGetExp:SetText( "" )
	PworldPolyPlayreInfoFrameSelfInfoGetMoney:SetText( "" )
	--�����Լ���Ϣ
	for i = 1, table.getn( t_playerList ) do
		if mainplayer:getName() == t_playerList[i].RoleName then
			PworldPolyPlayreInfoFrameSelfInfoRank:SetText( "��"..i.."��" )
			PworldPolyPlayreInfoFrameSelfInfoGetExp:SetText( "����þ���������"..t_playerList[i].Exp )
			PworldPolyPlayreInfoFrameSelfInfoGetMoney:SetText( "������ң�"..t_playerList[i].Money )
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
	--��ȡ�������
	local polyData = GamePoly:getDoubleMonsterInfo()
	local playerData = polyData.MonsterEndInfo;
	--����ɱ��������������
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
	--������ʾ�б�
	SetPlayersInfo( playerList );
	PworldPolyPlayreInfoFrame:Show()
end
----------------------------------------------------------------���ʱ��---------------------------------------------------------------------------------------
local MACHINE_WAR_ACTIVITY_ID = 45

local HUMAN_CAMP_ID = 1
local TITAN_CAMP_ID = 2
local MONSTER_CAMP_ID = 3

local t_MachinWarRemind = {
	"����΢��,���ƶ�Ѹ�١�ֻҪ��ֲ���,�ͻ��ú�������!ֻҪ��ֵ��ף��ͻ�Ӯ��ʤ��!",
	"Ŀ��:#c00ff00���ࡢ��е����#cffffbe��ɱ������,��þ���,�����ӪҲ��׳����ϸ�о���ļ���,С�ľ�������!",
	"Ŀ��:#c00ff00���ࡢ��е����#cffffbe��ɱ������,��þ���,�����ӪҲ��׳������������ļ���,С�ľ�������!",
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
			--�����Ӫbossλ����Ϣ
			ClearMachineWarBossMinimap()
			ClearMachineWarBoss()
			
			GamePoly:resetMachineWar()
			
			updateMachineWarInfoFrame()
			this:Show()
			
			CloseSpecialActivityGuid( MACHINE_WAR_ACTIVITY_ID )
		end
	elseif arg1 == "GE_LEAVE_PWORLD" then
		if GamePoly:isInPloyPworld_MachineWar() then
			--�����Ӫbossλ����Ϣ
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

--���³���������Ӫ���������Ӫ�����λ��
function updateBossMachineWarPos()
	--���֮ǰ��λ����Ϣ
	ClearMachineWarBossMinimap()
	ClearMachineWarBoss()
	local MaxMapControl	= MapShower:GetMaxMapContal();
	--��������λ��
	local titanPosX, titanPosY = GamePoly:getMachineWarTitanPosX(), GamePoly:getMachineWarTitanPosY()
	AddMachineWarBossMinimap( "��е��������", MaxMapControl:getMapID(), "uires\\ui\\Za\\JiXieJuRenBoss.tga", 10*titanPosX, 10*titanPosY )	
	AddMachineWarBoss( "��е��������", MaxMapControl:getMapID(), "uires\\ui\\Za\\JiXieJuRenBoss.tga", 10*titanPosX, 10*titanPosY )	
	--��е����λ��
	local machinePosX, machinePosY = GamePoly:getMachineWarMachinePosX(), GamePoly:getMachineWarMachinePosY()
	AddMachineWarBossMinimap( "��е��������", MaxMapControl:getMapID(), "uires\\ui\\Za\\JiXieGuaiWuBoss.tga", 10*machinePosX, 10*machinePosY )
	AddMachineWarBoss( "��е��������", MaxMapControl:getMapID(), "uires\\ui\\Za\\JiXieGuaiWuBoss.tga", 10*machinePosX, 10*machinePosY )
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
	MachineWarInfoFrameGetExp:SetText("���齱��(exp)��"..machineWarInfo.ToTalExp)
	
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
	
	--���û��ʼ������ĵ���ʱ
	if machineWarInfo.StartTime - getServerTime().sec > 0 then
		local timer = machineWarInfo.StartTime - getServerTime().sec
		MachineWarInfoFrameTime:SetText( "				�׼����" )
		MachineWarReadyTimeFrame:Show()
		local timeProgress = 0
		if GamePoly:getMachineWarReadyTime() > 0 then
			timeProgress = math.ceil(timer)/GamePoly:getMachineWarReadyTime()
		end
		MachineWarReadyTimeFrameTimeProgress:SetSize( 300*timeProgress, 9 )
		MachineWarReadyTimeFrameTime:SetText( timer.."/".. GamePoly:getMachineWarReadyTime() )
		MachineWarReadyTimeFrameTitle:SetText( "�׼��ʣ��ʱ��" )
	else
		MachineWarReadyTimeFrame:Hide()
		MachineWarInfoFrameTime:SetText(string.format("�ʣ��ʱ�䣺%ds", math.ceil(leaveTime) ))
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
		MachineWarReadyTimeFrameTitle:SetText( "���������ʱ" )
	end
end

--��е����ʤ��ȷ�����
function MachineWarResult( nFlag, nEndExp, nTotalExp )
	MachineWarResultFrame:Show();
	local szInfo = ""
	if nFlag == 1 then
		MachineWarResultFrameResult:SetText( "ʤ��" )
		szInfo = string.format( "#cffffbe�������ļ����Ŭ���������ڵ���Ӫ�������ʤ����\n����þ��齱��:#c00ff00%d��#cffffbe(������Ӫʤ������:#c00ff00%d��#cffffbe)\n���潱�£�100", nTotalExp, nEndExp )
	else
		MachineWarResultFrameResult:SetText( "ʧ��" )
		szInfo = string.format( "#cffffbe���ź��������ڵ���Ӫʧ���ˣ��´μ���Ŭ��Ӵ��\n����þ��齱��:#c00ff00%d��#cffffbe(������Ӫʧ������:#c00ff00%d��#cffffbe)\n���潱�£�100", nTotalExp, nEndExp )
	end
	MachineWarResultFrameInfo:SetText( szInfo, 255, 255, 190 );
end
function MachineWarResultFrame_onLoad()
	MachineWarResultFrameInfo:setCenterLine(true)
end
--��ʾ���
function SetPworldRemindInfo( szInfo )
	PworldRemindInfoWords:SetText( szInfo, 255,255,190 )
end

------------------------------------------------------------̹�˴�ս-���Ͻ�ͳ��С���------------------------------------------------------------
local TANK_WAR_ACTIVITY_ID		= 64;
local t_TankWarCampTex = {
	--��ɫ
	{ path = "uires\\ZhuJieMian\\3.dds", x = 948, y = 737, w = 15, h = 15, },
	--��ɫ
	{ path = "uires\\ZhuJieMian\\3.dds", x = 932, y = 737, w = 15, h = 15, },
}
local t_CampName = { "�����", "�۹���", }
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
				--���븱����ر��·�Ʈ����ʾ
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
---------------------------------------------------------̹�˴�սս��ͳ�����---------------------------------------------------
local t_TankPlayer = {}
local MAX_SHOW_TANK_PLAYER_NUM = MAX_SHOW_PLAYER_NUM --̹�˴�ս��Ϣ��壬һ����ʾ���������
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
	--�����һ����ʾ
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
	
	--����scrollbtn���ֵ
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
	
	--�����Լ���ս��
	for i = 1, table.getn( t_TankPlayer ) do
		if t_TankPlayer[i].RoleName == mainplayer:getName() then
			color = { 0,255,0, }
			TankWarPlayreInfoFrameSelfInfoRank:SetText( "��"..i.."��" )
			TankWarPlayreInfoFrameSelfInfoGetExp:SetText( "��þ��飺".. t_TankPlayer[i].Exp )
			TankWarPlayreInfoFrameSelfInfoGetMoney:SetText( "������ң�".. t_TankPlayer[i].Money )
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
