

local	MAX_BTN_WIDTH = 177;	--dps条最大宽度
local	MAX_BTN_NUM	  = 6;		--最多统计数

local	t_RaceInfoTex = {
						{x=847 , y=363 , w=177 ,h=18 },	--红色
						{x=847 , y=345 , w=177 ,h=18 },	--蓝色
						{x=847 , y=399 , w=177 ,h=18 },	--绿色
						{x=847 , y=381 , w=177 ,h=18 },	--黄色
					}

local COUNT_DAMAGE	= 3;	--统计伤害
local COUNT_HURT	= 4;	--统计承受伤害
local COUNT_DPS		= 5;	--统计伤害每秒

local g_CurType		= COUNT_DAMAGE;	--当前统计类型


local t_CountTypeName =	{[COUNT_DAMAGE]	= "总伤害量",
						 [COUNT_HURT]	= "承受伤害",
						 [COUNT_DPS]	= "每秒伤害" }

function DamageCheckIsInTeam( szName )
	local pActor = ActorMgr:findActorByName( szName );
	return ( (pActor ~= nil and pActor == ActorMgr:getMainPlayer()) or TeamManager:isInTeam( szName ) );
end

function getCountDamagers()
	return GameDamageHealCounter.getNextPlayer, GameDamageHealCounter
end

function getNearBoss()
	return GameDamageHealCounter.getNextBoss, GameDamageHealCounter
end

--排序
function Sort(a,b)
	return a[g_CurType ] > b[g_CurType ];
end

--伤害数据表
local t_DamageInfo = {};

local t_BtnText = {};

function UpdateDamage()
	if not DamageCountFrame:IsShown() then
		return;
	end
	--[[
	for i in getNearBoss() do
		print("boss:", i:getName(), GameDamageHealCounter:getBossInfo(i).targetId)
	end
	]]--
	DamageCountFrameTitle:SetText(t_CountTypeName[g_CurType] );

	for player in getCountDamagers() do
		if player ~= nil then
			if player:getType() == GAT_MAINPLAYER or TeamManager:isInTeam( player:getName() )  then
				local Info = GameDamageHealCounter:getPlayerInfo( player );
				if Info ~= nil then
					--计算秒伤
					local damagePerSec = 0;
					if Info.time > 0 then
						damagePerSec = Info.damage/math.ceil( Info.time );
					end
					t_DamageInfo[player:getName()] = { player:getName(),player:getRace() % 16,Info.damage,Info.hurt,damagePerSec,Info.time };
				end
			end
		end
	end
	local t_Tmp = {};
	t_BtnText = {};

	local nTotalCount = 0;
	for _,info in pairs( t_DamageInfo ) do
		if info ~= nil then
			nTotalCount = nTotalCount + info[g_CurType ];
			table.insert(t_Tmp,info);
		end
	end
	table.sort( t_Tmp,Sort );
	if nTotalCount == 0 then
		for i = 1,MAX_BTN_NUM do
			local btn = getglobal("DamageCountFramePlayer"..i);
			btn:Hide();
		end
		return;
	end
	local index = 0;
	local nFirst = 1;
	for _,info in pairs( t_Tmp ) do
		index = index + 1;	
		local btn = getglobal("DamageCountFramePlayer"..index);
		local tex = getglobal("DamageCountFramePlayer"..index.."RoleTex");
		local percentFont = getglobal("DamageCountFramePlayer"..index.."PercentFont");
		if btn ~= nil and tex ~= nil and percentFont ~= nil then
			if info ~= nil then
				local Scale = info[g_CurType]/nTotalCount;
				if index == 1 then
					nFirst = Scale;
				end
				local count = info[g_CurType];
				if g_CurType == COUNT_DPS then
					count = string.format("%d",info[g_CurType]);
				end
				
				t_BtnText[index] = { info[1],count,string.format("%d",Scale*100) .."%",info[2] } ;
				Scale = Scale / nFirst;				
				if Scale <0.02 and info[g_CurType] > 0 then
					Scale = 0.02;
				end
				tex:SetSize(t_RaceInfoTex[info[2]].w*Scale,18 );
				tex:SetTexUV( t_RaceInfoTex[info[2]].x,t_RaceInfoTex[info[2]].y,t_RaceInfoTex[info[2]].w*Scale,t_RaceInfoTex[info[2]].h );
				
				if string.len(t_BtnText[index][1]) > 2 and btn:GetTextExtentWidth(index.." ".. t_BtnText[index][1].."  "..t_BtnText[index][2] ) > MAX_BTN_WIDTH then
					btn:SetText(index.." ".. string.sub(t_BtnText[index][1],0,2).. "..  " .. t_BtnText[index][2] );
				else
					btn:SetText(index.." ".. t_BtnText[index][1].."  "..t_BtnText[index][2] );
				end
				percentFont:SetText( t_BtnText[index][3] );
				btn:Show();
			end
		end
	end
	--隐藏未使用的button
	for i = index+1,MAX_BTN_NUM do
		local btn = getglobal("DamageCountFramePlayer"..i);
		if btn ~= nil then
			btn:Hide();
		end
	end
end

function DamageCountFrame_OnLoad()
	 this:setUpdateTime( 1 );
	 this:RegisterEvent( "GE_ENTER_PWORLD" );
	 this:RegisterEvent( "GE_ACTION_REVIVE" );
	 GameDamageHealCounter:SetFilter("DamageCheckIsInTeam");
end

function DamageCountFrame_OnEvent()
	if arg1 == "GE_ENTER_PWORLD" then
		--清空伤害统计,界面没有打开时，直接清空数据
		if DamageCountFrame:IsShown() then
			DamageCountFrameClearBtn_OnClick();
		else
			DamageCountFrameClearconfirm();
		end
	end
	if arg1 == "GE_ACTION_REVIVE" then
		if MessageBoxFrame:IsShown() and MessageTitle:GetText() == "伤害统计" then
			MessageBoxCancelBtnOnClick();
		end
	end
end

function DamageCountFrame_OnUpdate()
	UpdateDamage();
end

function DamageCountFrame_OnShow()
	this:SetPoint("BOTTOMRIGHT","$parent","BOTTOMRIGHT", -125, -120);
	UpdateDamage();
	GameUserConfig:setDamageConfigData( 1 );
end

function DamageCountFrame_OnHide()
	for i = 1,MAX_BTN_NUM do
		local btn = getglobal( "DamageCountFramePlayer"..i );
		if btn ~= nil then
			btn:Hide();
		end
	end

	DamagePlayerShowSendBtns(false);
	DamagePlayerShowTypeBtns(false);

	GameUserConfig:setDamageConfigData( 0 );
end

function DamageCountFrameClearBtn_OnClick()
	if DamageCountFrame:IsShown() then
		MessageBox("伤害统计","确定清空统计结果吗？");
	end
end

--确定清空数据
function DamageCountFrameClearconfirm()
	GameDamageHealCounter:reset();
	t_DamageInfo = {};
	UpdateDamage();
end

function DamageCountFrameReset()
	DamageCountFrameClearconfirm();
	PrivateToFrame:Hide();
	g_CurType = COUNT_DAMAGE;
end

function CloseBtn_OnEnter()
	SetGameTooltips(this:GetName(),"隐藏面板");
end

function DamageCountFrameClearBtn_OnEnter()
	SetGameTooltips(this:GetName(),"清空数据");
end

function DamagePlayerShowTypeBtns(bShow)
	if bShow then
		TypeHurtBtn:Show();
		TypeDpsBtn:Show();
		TypeDamageBtn:Show();
	else
		TypeHurtBtn:Hide();
		TypeDpsBtn:Hide();
		TypeDamageBtn:Hide();
	end
end

function DamageCountFrameChangeTypeBtn_OnClick()
	if arg1 ~= "LeftButton" then
		return;
	end
	if TypeHurtBtn:IsShown() then
		DamagePlayerShowTypeBtns(false);
	else
		DamagePlayerShowTypeBtns(true);
	end
	DamagePlayerShowSendBtns(false);
end

function TypeHurtBtn_OnClick()
	g_CurType = COUNT_HURT;
	this:DisHighlight();
	DamagePlayerShowTypeBtns(false);
	UpdateDamage();
end

function TypeDpsBtn_OnClick()
	g_CurType = COUNT_DPS;
	this:DisHighlight();
	DamagePlayerShowTypeBtns(false);
	UpdateDamage();
end

function TypeDamageBtn_OnClick()
	g_CurType = COUNT_DAMAGE;
	this:DisHighlight();
	DamagePlayerShowTypeBtns(false);
	UpdateDamage();
end
function DamageCountFrameChangeTypeBtn_OnEnter()
	SetGameTooltips(this:GetName(),"显示所有数据类型");
end

function DamagePlayerBtn_OnEnter()
	local text = t_BtnText[this:GetClientID()][1] .." "..t_raceList[t_BtnText[this:GetClientID()][4] ].." "..t_CountTypeName[g_CurType].." "..t_BtnText[this:GetClientID()][2];
	SetGameTooltips(this:GetName(),text);
end

function DamagePlayerShowSendBtns(bShow)
	if bShow then
		SendNearBtn:Show();
		SendTeamBtn:Show();
		SendPrivateBtn:Show();
		SendGuildBtn:Show();
	else
		SendNearBtn:Hide();
		SendTeamBtn:Hide();
		SendPrivateBtn:Hide();
		SendGuildBtn:Hide();
	end
end

function DamageCountFrameSendDpsBtn_OnClick()
	if arg1 ~= "LeftButton" then
		return;
	end
	if SendNearBtn:IsShown() then
		DamagePlayerShowSendBtns( false );
	else
		DamagePlayerShowSendBtns( true );
	end

	DamagePlayerShowTypeBtns(false);
end

function DamageCountFrameSendDpsBtn_OnEnter()
	SetGameTooltips( this:GetName(),"广播数据，通知其他人" );
end

function SendNearBtn_OnClick()
	this:DisHighlight();
	SendDpsChat( CL_CHAT_SAY,"" );
	DamagePlayerShowSendBtns();
end

function SendTeamBtn_OnClick()
	this:DisHighlight();
	DamagePlayerShowSendBtns(false);
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	if not TeamManager:isInTeam( mainplayer:getName() ) then
		ShowMidTips("当前没有加入队伍");
		return;
	end
	SendDpsChat( CL_CHAT_TEAM,"" );
end

function SendGuildBtn_OnClick()
	this:DisHighlight();
	DamagePlayerShowSendBtns(false);
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	if not mainplayer:isInClan() then
		ShowMidTips("当前没有加入公会");
		return;
	end
	SendDpsChat( CL_CHAT_CLAN,"" );
end

function SendPrivateBtn_OnClick()
	this:DisHighlight();
	DamagePlayerShowSendBtns(false);
	PrivateName_Edit:SetText("");
	PrivateToFrame:Show();
	SetFocusFrame("PrivateName_Edit");	
end

function PrivateNameAcceptBtnOnClick()
	local szName = PrivateName_Edit:GetText();
	if szName == nil or szName == "" then
		ShowMidTips("请输入私聊对象");
		SetFocusFrame("PrivateName_Edit");
		return;
	end
	SendDpsChat( CL_CHAT_PRIVATE,szName );
	PrivateToFrame:Hide();
	ClearFocus();
end

--在聊天中发送统计结果
function SendDpsChat(chatType,privateName)
	local text = "";
	if chatType == CL_CHAT_PRIVATE then
		local t_ChatInsertFrameControl = GetChatInsertFrameControl();
		t_ChatInsertFrameControl:setPrivatePlayerName( { ["name"] = privateName } );
		text = " #@!（" .. t_CountTypeName[g_CurType] .. "）统计：" ;
		-- text = text .. "/m " .. privateName .. " #@!（" .. t_CountTypeName[g_CurType] .. "）统计：" ;
	else
		text ="#@!" .. "（"..t_CountTypeName[g_CurType] .. "）统计：" ;
	end
	text = text .. "#";
	for i = 1,MAX_BTN_NUM do
		local btn = getglobal("DamageCountFramePlayer"..i);
		if btn ~= nil and btn:IsShown() then
			text = text .. "#@!" .. i..".  " .. t_BtnText[ btn:GetClientID() ][1] .. "  " .. t_BtnText[ btn:GetClientID() ][2] .. "  (" .. t_BtnText[ btn:GetClientID() ][3] .. ")#";
		end
	end
	if table.getn(t_BtnText) == 0 then
		text = text .. "#@! 1 0 0 0 #" ;
	end
	SetChatType( chatType );
	ChatSendMsg( text,false );
end


--[[
--更新附近boss 的目标
local MAX_BOSS_TARGET_NUM = 3;

function UpdateBossTarget()
	local nIndex = 1;
	for boss in getNearBoss() do
		if boss ~= nil then
			local BossBtn = getglobal( "DamageCountFrameBossBtn"..nIndex );
			local TargetBtn = getglobal( "DamageCountFrameBossTargetBtn"..nIndex );
			if BossBtn ~= nil and TargetBtn ~= nil then
				BossBtn:SetText( boss:getName() );
				BossBtn:SetClientUserData( 0,boss:GetID() );
				BossBtn:Show();
				BossTargetTitleString:Show();
				local info = GameDamageHealCounter:getBossInfo(boss);
				if info ~= nil then
					local targetPlayer = ActorMgr:FindActor( info.targetId );
					if targetPlayer ~= nil then
						TargetBtn:SetText( targetPlayer:getName() );
						TargetBtn:SetClientUserData( 0,info.targetId );
						TargetBtn:Show();
					end
				end
			end
			nIndex = nIndex + 1;
		end
	end
	for i = nIndex,MAX_BOSS_TARGET_NUM do
		local BossBtn = getglobal( "DamageCountFrameBossBtn"..i );
		local TargetBtn = getglobal( "DamageCountFrameBossTargetBtn"..i );
		if BossBtn ~= nil then
			BossBtn:Hide();
		end
		if TargetBtn ~= nil then
			TargetBtn:Hide();
		end
	end
	if nIndex == 1 then
		BossTargetTitleString:Hide();
	end
end
]]--

function BossTargetBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	mainplayer:SetSelectTarget( this:GetClientUserData( 0 ) );
end