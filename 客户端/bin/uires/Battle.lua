--��Ӫ��ɫ��ͼ
campIdxTexture = {
	"uires\\ui\\Za\\LieBiaoDiBan\\HongSeDiBan.tga",
	"uires\\ui\\Za\\LieBiaoDiBan\\LanSeDiBan.tga",
	"uires\\ui\\Za\\LieBiaoDiBan\\HuangSeDiBan.tga";
	
	[-1] = "uires\\ui\\Za\\LieBiaoDiBan\\BaiSeDiBan.tga",
	[0] = "uires\\ui\\mask\\TouMingTongDao.tga",
}

campIdxIcon = {
	"uires\\ui\\Za\\ShengCunDaMaoXianBiaoShi\\HongSeZhenYing.tga",
	"uires\\ui\\Za\\ShengCunDaMaoXianBiaoShi\\LanSeZhenYing.tga",
	"uires\\ui\\Za\\ShengCunDaMaoXianBiaoShi\\HuangSeZhenYing.tga",
}

--��Ӫǰ��ɫ
campIdxTextColor = {
	{255, 169, 169},
	{169, 247, 255},
	{237, 235, 187};
	
	[-1] = {0, 255, 0},
	[0] = {255, 254, 189},
}

battleRewardIcon = 
{
	[ATTR_ID_BATTLE_GLORY] = "uires\\TuPianLei\\WuPin\\5106004.tga",
	[ATTR_ID_AOSHAN_BRAND] = "uires\\TuPianLei\\WuPin\\5052812.tga",
	[ATTR_ID_LIFT_BRAND] = "uires\\TuPianLei\\WuPin\\5052813.tga"
}

local battles
local lastSelectedBattle
local battleNameTotal = 6

local lastPastTime, lastInviteTime, lastWaitTime

local aotuExitTimer

local pworldIdx
local pworldName = ""

local refreshedShowInvite

local battlePanel = {
	"ExistenceExpFrame",
	"BattleSummaryFrame",
	"XuKongBattleFrame",
	"ClanWarInfoFrame",
	"ClanWarInfoFrame",
}
--һЩ����Ļ
local t_SpecialPworldActivityDesc = {
	["ĩ����ҫ����"] = [[#cffffbeĩ����ҫ���ù���
#cffffbe	1.����ȼ���60                      	  2.��սʱ�䣺5����
#cffffbe	3.һ����ɱ���ﵽ10�κ��ʤ        4.��;�뿪��Ϊ����
#c00ff00	5.���� 19��30-23��00 ��������ȡ������������
#c00ff00	6.��һ����ĩ����ҫ���а��߿ɻ�����а�������ͨ���ʼ���ʽ���ţ�]],
}
local SPARTA_PWORLD_ACTIVITY_ID = 48
local t_SpecialPworldActivity = {
	{ id = 0,activeId = 48,name = "ĩ����ҫ����",desc = t_SpecialPworldActivityDesc["ĩ����ҫ����"],win = "",tex = "uires\\DiTuLei\\QuYuDiTu\\603000.dds",level = 60,hasDef = false,player="3:3"},
}

function ResetBattleInfo()
	BattleStarted = false
	lastWaitTime = nil
	lastInviteTime = nil
end

--��ɳ���ر������60������ֻ��ʾ60�����µ��Ǹ���60������ֻ��ʾ60�����ϵ��Ǹ�
--û���ֶ������֣���ʱ��id���㣬���������������������ø���ֶ�
function IsBattleNeedRemove( id )
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then return end
	if id == 1 and mainplayer:getLv() >= 60 then
		return true
	elseif id == 6 and mainplayer:getLv() < 60 then
		return true
	end
	return false
end

function GetBattles()
	--��Ҫ��ʱ���µĿ�ɳ�������ÿ������������
	--if not battles then
	--end
	battles = {}
	--�Ƚ����������б�
	for i = 1, table.getn( t_SpecialPworldActivity ) do
		table.insert( battles, t_SpecialPworldActivity[i] )
	end
	for i = 0, GameBattle:getBattleNum() - 1 do
		local t = {
			id = GameBattle:getBattleID(i),
			activeId = GameBattle:GetBattleActiveId(GameBattle:getBattleID(i));
			name = GameBattle:getBattleName(i),
			desc = GameBattle:getBattleDesc(i),
			win = GameBattle:getBattleWin(i),
			tex = string.format("uires\\DiTuLei\\QuYuDiTu\\%d.dds", GameBattle:getBattleTexture(i)),
			level = GameBattle:getBattleMinLv(i),
			hasDef = true,
		}
		local n = GameBattle:getBattleCamp(i)
		if n ~= 0 then
			local s = {}
			local k = math.floor(GameBattle:getBattlePlayerMax(i) / n + 0.5)
			for m = 1, n do
				table.insert(s, k)
			end
			t.player = table.concat(s, " : ")
		end
		if not IsBattleNeedRemove( GameBattle:getBattleID(i) ) then
			table.insert(battles, t)
		end
	end
	return battles
end

function BattleOpenBtn_OnLoad()
	this:RegisterEvent("GE_ENTER_PLAYERLOGIN")
	this:RegisterEvent("GE_BATTLE_ENTER")
	this:RegisterEvent("GE_BATTLE_LEAVE")
	this:RegisterEvent("GE_BATTLE_END")
	this:RegisterEvent("GE_PLAYER_AFK")
	this:RegisterEvent("GE_PLAYER_LEAVEUP")
	this:RegisterEvent("GE_UPDATE_ROLELIST")
end

function BattleOpenBtn_OnEvent()
	if arg1 == "GE_ENTER_PLAYERLOGIN" then
		this:SetClientString("ս������")
		ResetBattleInfo()
		if aotuExitTimer then
			hangup(aotuExitTimer)
			aotuExitTimer = nil
		end
	elseif arg1 == "GE_BATTLE_ENTER" then
		BattleStarted = true
		this:SetClientString("����鿴ս����Ϣ")
		BattleStartFrame:Hide()
		lastInviteTime = nil
		StrongWarContineKillFrame:Show()
		StrongWarContineKillFrame:SetClientUserData(1,BATTLE_CONTINE_KILL_TYPE);
	elseif arg1 == "GE_BATTLE_LEAVE" then
		BattleStarted = false
		BattleLeaveConfirmFrame:Hide()
		this:SetClientString("ս������")
		if aotuExitTimer then
			hangup(aotuExitTimer)
			aotuExitTimer = nil
		end
		StrongWarContineKillFrame:Hide();
	elseif arg1 == "GE_BATTLE_END" then
		BattleStarted = false
		local panel = getglobal(battlePanel[GameBattle:getCurrentBattleId()])
		if panel then
			panel:Show()
		end
	elseif arg1 == "GE_PLAYER_AFK" then
		if aotuExitTimer then
			hangup(aotuExitTimer)
			aotuExitTimer = nil
		end
		local mainplayer = ActorMgr:getMainPlayer()
		if mainplayer and mainplayer:isAFK() then
			aotuExitTimer = timer(function()
				GameBattle:LeavePworld()
			end, 3 * 60)
		end
	elseif arg1 == "GE_PLAYER_LEAVEUP" then
		battles = {}
		UpdateBattleName()
	elseif arg1 == "GE_UPDATE_ROLELIST" then
		battles = {}
		UpdateBattleName()
	end
end

function ClearBattleTips()
	local text
	for i = 1, 10 do
		text = getglobal("BattleStartFrameTipsCenter"..i)
		text:Hide()
		text = getglobal("BattleStartFrameTipsLeft"..i)
		text:Hide()
	end
	BattleStartFrameTips:Hide()
end

function SetBattleTips(battleId, activeId, parent)
	ClearBattleTips()
	
	local startInfo = GamePromotion:getCurrPromitionStartInfo(activeId)
	local actived = startInfo.startFlag == PROMOTION_RUNING
	local index = startInfo.timeIdx
	if index < 0 then
		index = 0
	end
	
	local offset = 0
	local n = 0
	local promotionData = GamePromotion:getPromotionData(activeId)
	if promotionData.m_nTimeCount == 0 or ((promotionData.m_Time[0].m_szStartTime == "" or promotionData.m_Time[0].m_szStartTime=="00:00") and promotionData.m_Time[0].m_szEndTime == "24:00") then
		n = n + 1
		local text = getglobal("BattleStartFrameTipsCenter"..n)
		text:SetText("������")
		text:SetPoint("top", "BattleStartFrameTipsTitle", "bottom", 0, 4 + (n - 1) * 20)
		text:Show()
		text:SetTextColor(255, 254, 191)
	else
		if promotionData.m_nType == PROMOTION_TYPE_WEEKLY then
			local nextOpenWeek 	= GamePromotion:getFirstOpenWeek( activeId );
			local szWeek 		= tWeekDayName[nextOpenWeek+1];
			n = n + 1;
			local text = getglobal("BattleStartFrameTipsCenter"..n)
			text:SetText( szWeek );
			text:SetPoint("top", "BattleStartFrameTipsTitle", "bottom", 0, 4 + (n - 1) * 20)
			text:Show()
			if startInfo.startFlag == PROMOTION_NOTTODAY then
				text:SetTextColor(255, 0, 0)
			else
				text:SetTextColor(0, 255, 0)
			end
		end
		for i = 0, promotionData.m_nTimeCount - 1 do
			n = n + 1
			local text = getglobal("BattleStartFrameTipsCenter"..n)
			text:SetText(string.format("%s - %s", promotionData.m_Time[i].m_szStartTime, promotionData.m_Time[i].m_szEndTime))
			text:SetPoint("top", "BattleStartFrameTipsTitle", "bottom", 0, 4 + (n - 1) * 20)
			text:Show()
			if index == i then
				if actived then
					text:SetTextColor(0, 255, 0)
				else
					text:SetTextColor(255, 0, 0)
				end
			else
				text:SetTextColor(147, 130, 89)
			end
		end
	end
	
	offset = offset + 10
	if activeId == SPARTA_PWORLD_ACTIVITY_ID then
		n = n + 1
		local text = getglobal("BattleStartFrameTipsCenter"..n)
		text:SetText( "���˶�����ܱ���" )
		text:SetPoint("top", "BattleStartFrameTipsTitle", "bottom", 0, 4 + (n - 1) * 20 + offset)
		text:Show()
		text:SetTextColor(255, 254, 191)
	else
		local info = BattleLimitInfo:getBattleData(battleId)
		local text
		n = n + 1
		text = getglobal("BattleStartFrameTipsCenter"..n)
		text:SetText(string.format("�����ѲμӴ���  %2d", info.Use))
		text:SetPoint("top", "BattleStartFrameTipsTitle", "bottom", 0, 4 + (n - 1) * 20 + offset)
		text:Show()
		text:SetTextColor(255, 254, 191)
		if info.Total ~= 0 then
			n = n + 1
			text = getglobal("BattleStartFrameTipsCenter"..n)
			text:SetText(string.format("���տɲμӴ���  %2d", info.Total))
			text:SetPoint("top", "BattleStartFrameTipsTitle", "bottom", 0, 4 + (n - 1) * 20 + offset)
			text:Show()
			text:SetTextColor(255, 254, 191)
		end
	end
	
	parent = parent or UIClient
	BattleStartFrameTips:SetSize(160, 16 + 20 * n + 10 + offset)
	BattleStartFrameTips:SetPoint("left", parent:GetName(), "right", 0, 0)
	BattleStartFrameTips:Show()
end

function SelectBattleName(id)
	local t = GetBattles()
	local k = t[id + BattleStartFrameScrollBar:GetValue()]
	if not k.hasDef then
		BattleStartFrameFrameTitle1:Hide();
		BattleStartFrameFrameTitle2:Hide();
		BattleStartFrameFrameTitle3:Hide();
		BattleStartFrameFrameTitle4:Hide();
		BattleStartFrame_remind:Hide();
		BattleStartFrame_Desc:SetText("")
		BattleStartFrame_Camp:SetText("")
		BattleStartFrame_MinLv:SetText("")
		BattleStartFrame_Win:SetText("")
		BattleStartFrame_MiniMap:SetTexture(k.tex)	
		BattleStartFrameFrameRichInfo:SetText( k.desc, 255,255,190 )
	else
		BattleStartFrameFrameTitle1:Show();
		BattleStartFrameFrameTitle2:Show();
		BattleStartFrameFrameTitle3:Show();
		BattleStartFrameFrameTitle4:Show();
		BattleStartFrame_remind:Show();
		BattleStartFrame_Desc:SetText(k.desc)
		BattleStartFrame_Camp:SetText(k.player or "")
		BattleStartFrame_MinLv:SetText(string.format("%d��", k.level))
		BattleStartFrame_Win:SetText(k.win)
		BattleStartFrame_MiniMap:SetTexture(k.tex)	
		BattleStartFrameFrameRichInfo:SetText( "", 255,255,190 )
	end
	
	local tex
	if lastSelectedBattle then
		tex = getglobal("BattleStartFrameNameButton"..lastSelectedBattle.."Selected")
		if tex then
			tex:Hide()
		end
	end
	lastSelectedBattle = id
	tex = getglobal("BattleStartFrameNameButton"..lastSelectedBattle.."Selected")
	if tex then
		tex:Show()
	end
	
	if GameBattle:isTeamInBattle(k.id) then
		BattleStartFrameJoin:Enable()
		BattleStartFrameTeamJoin:Enable()
	else
		BattleStartFrameJoin:Enable()
		BattleStartFrameTeamJoin:Disable()
	end
	if not k.hasDef then
		BattleStartFrameTeamJoin:Enable()
		BattleStartFrameJoin:Disable()
	end
	if GameBattle:isNeedClanLeaderOpenApply( k.id ) then
		BattleStartFrameClanOpen:Show()
		BattleStartFrameTeamJoin:Hide()
	else
		BattleStartFrameClanOpen:Hide()
		BattleStartFrameTeamJoin:Show()
	end
end

function UpdateBattleName()
	local v	= BattleStartFrameScrollBar:GetValue()
	local t	= GetBattles()
	local n = #t
	for i = 1, battleNameTotal do
		local btn = getglobal("BattleStartFrameNameButton"..i)
		if btn then
			if i + v > n then
				btn:Hide()
			else
				local name = t[i + v].name
				local id = t[i + v].id
				local info = BattleLimitInfo:getBattleData(id)
				if info.Total > 0 then
					name = string.format("%s(%d/%s)", name, info.Use, info.Total == 0 and "-" or tostring(info.Total))
				end
				local activeId = t[i + v].activeId
				local actived = activeId == 0 or GamePromotion:getCurrPromitionStartInfo(activeId).startFlag == PROMOTION_RUNING
				if actived then
					name = name.."#c00ff00(������)#n"
				else
					local remain
					local now = os.date("*t", getServerTime().sec)
					local promotionData = GamePromotion:getPromotionData(activeId)
					local startInfo = GamePromotion:getCurrPromitionStartInfo(activeId)
					if startInfo.startFlag == PROMOTION_COMING then
						local startTime = promotionData.m_Time[startInfo.timeIdx].m_szStartTime
						local _, _, hour, min = string.find(startTime, "(%d+)\s*:\s*(%d+)")
						hour, min = tonumber(hour), tonumber(min)
						remain = (hour - now.hour) * 60 + min - now.min
						--[[
					elseif startInfo.startFlag == PROMOTION_PASSED then
						local startTime = promotionData.m_Time[0].m_szStartTime
						local _, _, hour, min = string.find(startTime, "(%d+)\s*:\s*(%d+)")
						hour, min = tonumber(hour), tonumber(min)
						remain = (hour - now.hour + 24) * 60 + min - now.min
						--]]
					end
					if remain then
						if remain >= 60 * 24 then
							name = string.format("%s#cff0000(%d��󿪷�)#n", name, math.floor(remain / (60 * 24)))
						elseif remain >= 60 then
							name = string.format("%s#cff0000(%dСʱ�󿪷�)#n", name, math.floor(remain / 60))
						else
							name = string.format("%s#cff0000(%d���Ӻ󿪷�)#n", name, remain)
						end
					else
						name = name.."#cff0000(δ����)#n";
					end
				end
				
				btn:Show()
				local text = getglobal("BattleStartFrameNameButton"..i.."Text")
				if text then
					text:SetText(name, 255, 175, 0)
				end
			end
		end
	end
end

function BattleOpenBtn_OnClick()
	if GameBattle:isRun() then
		local panel = getglobal(battlePanel[GameBattle:getCurrentBattleId()])
		if panel then
			if panel:IsShown() then
				panel:Hide()
			else
				panel:Show()
			end
		end
	else
		if BattleStartFrame:IsShown() then
			BattleStartFrame:Hide()
		else
			BattleStartFrame:Show()
		end
	end
end

function BattleStartFrame_OnShow()
	util.ShowInCenter(this)
	local t = GetBattles()
	local n = #t
	if n == 0 then
		BattleStartFrame:Hide()
		return
	end
	if n > battleNameTotal then
		BattleStartFrameScrollBar:Show()
		BattleStartFrameScrollBar:SetMaxValue(n - battleNameTotal)
		BattleStartFrameScrollBar:SetValue(0)
	else
		BattleStartFrameScrollBar:Hide()
		BattleStartFrameScrollBar:SetMaxValue(0)
		BattleStartFrameScrollBar:SetValue(0)
		UpdateBattleName()
	end
	SelectBattleName(1)
end

function BattleStartFrame_ScrollUpBtn()
	local slider = BattleStartFrameScrollBar
	if not slider:IsShown() then
		return
	end
	local nValue = slider:GetValue() - slider:GetValueStep()
	if nValue >= slider:GetMinValue() then
		slider:SetValue(nValue)
	end
end

function BattleStartFrame_ScrollDownBtn()
	local slider	= BattleStartFrameScrollBar
	if not slider:IsShown() then
		return
	end
	local nValue	= slider:GetValue() + slider:GetValueStep()
	if nValue <= slider:GetMaxValue() then
		slider:SetValue(nValue)
	end
end

function BattleStartFrame_ScrollBarValueChange()
	UpdateBattleName()
end

function BattleName_OnClick()
	SelectBattleName(this:GetClientID())
	CloseClanWarMatchFrames()
end

function BattleName_OnEnter()
	local tex = getglobal(this:GetName().."Highlight")
	if tex then
		tex:Show()
	end
	local t = GetBattles()
	local k = t[this:GetClientID() + BattleStartFrameScrollBar:GetValue()]
	local id = k.activeId
	if id ~= 0 then
		SetBattleTips(k.id, id, this)
	end
end

function BattleName_OnLeave()
	local tex = getglobal(this:GetName().."Highlight")
	if tex then
		tex:Hide()
	end
	ClearBattleTips()
end

function BattleReward_OnEnter()
	util.HightParent(this)
	local id = this:GetClientUserData(0)
	if id ~= 0 then
		ShowItemTips(LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, getItemDef(id))
	end
end

function BattleReward_OnLeave()
	util.HightParent(this, false)
	ItemButton_OnLeave()
end

function BattleJoin( nType )
	if GameSparta:IsInSpartaPworld() then
		ShowMidTips( "���ս��������˲���" )
		return
	end
	if not lastSelectedBattle then
		return
	end
	BattleStartFrame:Hide()
	local n = lastSelectedBattle + BattleStartFrameScrollBar:GetValue()
	local t = GetBattles()
	local id = t[n].id
	if lastWaitTime or lastInviteTime then
		function BattleConfirmFrameOK_OnClick()
			refreshedShowInvite = nil
			BattleConfirmFrame:Hide()
			GameBattle:join(id, nType)
		end
		BattleConfirmFrame:Show()
	else
		GameBattle:join(id, nType)
	end
end

function BattleStartFrameJoin_OnClick()
	local n = lastSelectedBattle + BattleStartFrameScrollBar:GetValue()
	local t = GetBattles()
	--����֮�ǣ�����Ҫ�й�����ܱ���
	if GameBattle:isNeedClanLeaderOpenApply( t[n].id ) then
		local mainplayer = ActorMgr:getMainPlayer()
		if not mainplayer:isInClan() then
			ShowMidTips("�㻹û�м��빫�ᣬ�޷�����")
			return
		end
	end
	BattleJoin(0)
end

function BattleStartFrameTeamJoin_OnClick()
	local n = lastSelectedBattle + BattleStartFrameScrollBar:GetValue()
	local t = GetBattles()
	if not t[n].hasDef then
		if GameSparta:IsInSpartaPworld() then
			ShowMidTips( "���ս��������˲���" )
			return
		end
		GameSparta:SpartaApply()
	else
		BattleJoin(1)
	end
end

function BattleStartFrameClanOpen_OnClick()
	local mainplayer = ActorMgr:getMainPlayer()
	if not mainplayer:isInClan() then
		ShowMidTips("�㻹û�м��빫�ᣬ�޷�����")
		return
	end
	local n = lastSelectedBattle + BattleStartFrameScrollBar:GetValue()
	local t = GetBattles()
	local activeId = t[n].activeId
	local actived = activeId == 0 or GamePromotion:getCurrPromitionStartInfo(activeId).startFlag == PROMOTION_RUNING
	if not actived then
		ShowMidTips( "δ������ʱ�䣬���ܽ���" )
		return		
	end
	local curMatchPworldId = GameBattle:getClanWarMatchPworldID()
	if GameBattle:getBattlePworldById( t[n].id ) ~= curMatchPworldId and curMatchPworldId ~= 0 then
		ShowMidTips( "�ѿ�����ͬ��ս���������ٿ�����ս��" )
		return
	end
	if mainplayer:getClanPosition() == CLAN_POS_PRESIDENT or mainplayer:getClanPosition() == CLAN_POS_VICE_PRESIDENT then
		BattleJoin(2)
	else
		ShowMidTips("ֻ�л᳤/���᳤���ܿ�������")
	end
end

--�Զ����
function BattleStartFrame_AutoTeam_OnClick()
	BattleStartFrame:Hide();
	SearchTeamFrame:Show();
end
--�����
function BattleStartFrame_ActivityAssist_OnClick()
	BattleStartFrame:Hide();	
	PromotionFrame:Show();
end
-- ��������
function ExistenceExpInviteFrame_OnLoad()
	this:RegisterEvent("GE_ENTER_PLAYERLOGIN")
	this:RegisterEvent("GE_EXISTENCEEXP_INVITE")
	this:RegisterEvent("GE_EXISTENCEEXP_WAIT")
	this:RegisterEvent("GE_EXISTENCEEXP_NOWAIT")
end

-- �յ�������Ϣ
function ExistenceExpInviteFrame_OnEvent()
	if arg1 == "GE_ENTER_PLAYERLOGIN" then
		ExistenceExp_WaitBtn:Hide()
	elseif arg1 == "GE_EXISTENCEEXP_WAIT" then
		local Msg = UIMSG:getUIMsg()
		pworldIdx = Msg.PworldInfo.PworldIdx;
		pworldName = Msg.PworldInfo.PworldName;
		lastWaitTime = getServerTime().sec
		ExistenceExp_WaitBtn:SetClientUserData( 0, Msg.PworldInfo.MatchType )
		ExistenceExp_WaitBtn:SetClientUserData( 1, Msg.PworldInfo.MinPlayerNum )
		ExistenceExp_WaitBtn:Show()
		--�����Ҫ����ƥ��״̬������֮��ս��
		if Msg.PworldInfo.MatchType == 1 then
			OpenClanWarMatch( Msg.PworldInfo.PworldID )
		end
	elseif arg1 == "GE_EXISTENCEEXP_INVITE" then
		lastWaitTime = nil
		lastInviteTime = getServerTime().sec
		refreshedShowInvite = true
		ExistenceExp_WaitBtn:Show()
		local Msg = UIMSG:getUIMsg()
		pworldIdx = Msg.PworldInfo.PworldIdx;
		pworldName = Msg.PworldInfo.PworldName;
		if not BattleConfirmFrame:IsShown() then
			ExistenceExpInviteFrameTitleName:SetText("")
			ExistenceExpInviteFrame_DescRich:SetText("", 255, 254, 189)
			ExistenceExpInviteFrame:Show()
		end
	elseif arg1 == "GE_EXISTENCEEXP_NOWAIT" then
		lastWaitTime = nil
		lastInviteTime = nil
		ExistenceExp_WaitBtn:Hide()
		ExistenceExpInviteFrame:Hide()
		ClanWarMatchFrame:Hide()
		ClanWarPresentInviteFrame:Hide()
	end
end

function ExistenceExpInviteFrame_OnShow()
	lastPastTime = -1
	refreshedShowInvite = nil
	ExistenceExpInviteFrame_OnUpdate()
end

function IsExistenceExpInviteOutOfTime()
	if lastWaitTime then
		return false
	elseif lastInviteTime then
		local pastTime = getServerTime().sec - lastInviteTime
		if pastTime < 60 then
			return false
		else
			return true
		end
	else
		return true
	end
end

-- ���뵹��ʱ1����
function ExistenceExpInviteFrame_OnUpdate()
	if lastWaitTime then
		local pastTime = getServerTime().sec - lastWaitTime
		if lastPastTime ~= pastTime then
			lastPastTime = pastTime
			--���������ʾ����
			ExistenceExpInviteFrameTitleName:SetText("�����Ŷ�")
			ExistenceExpInviteFrame_DescRich:SetText(string.format("\b%s�Ŷ��У����Ѿ��ȴ���#cF16F06%2d#n��#cF16F06%2d#n�롣", pworldName, math.floor(pastTime / 60), pastTime % 60), 255, 254, 189)
			ExistenceExpInviteFrame_EnterButton:Hide()
			ExistenceExpInviteFrame_CancelButton:Hide()
			ExistenceExpInviteFrame_OKButton:Show()
			ExistenceExpInviteFrame_LeaveButton:Show()
		end
	elseif lastInviteTime then
		local pastTime = getServerTime().sec - lastInviteTime
		if pastTime < 60 then
			if lastPastTime ~= pastTime then
				lastPastTime = pastTime
				--���������ʾ����
				ExistenceExpInviteFrameTitleName:SetText("�����")
				ExistenceExpInviteFrame_DescRich:SetText(string.format("\b\b�Ƿ����%s(%d)�����뽫��#cF16F06%2d#n���ʧЧ��", pworldName, pworldIdx or 1, 60 - pastTime), 255, 254, 189)
				ExistenceExpInviteFrame_OKButton:Hide()
				ExistenceExpInviteFrame_LeaveButton:Hide()
				ExistenceExpInviteFrame_EnterButton:Show()
				ExistenceExpInviteFrame_CancelButton:Show()
				ClanWarMatchFrame:Hide()
			end
		else
			ExistenceExpInviteFrame:Hide();
			ExistenceExp_WaitBtn:Hide();
			lastInviteTime = nil
		end
	else
		ExistenceExp_WaitBtn:Hide();
		ExistenceExpInviteFrame:Hide()
	end
end

-- ����������밴ť
function ExistenceExpInviteFrame_EnterButton_OnClick()
	ExistenceExpInviteFrame:Hide()
	GameBattle:EnterPworld()
end

-- �����֪���˰�ť
function ExistenceExpInviteFrame_OKButton_OnClick()
	ExistenceExpInviteFrame:Hide()
end

-- ��������ȡ����ť
function ExistenceExpInviteFrame_CancelButton_OnClick()
	ExistenceExpInviteFrame:Hide()
	if not GameWizard:isDo(GAMEWIZARD_FIRST_BLINK_EXIST_INVITE_BTN) then
		GameWizard:doStep(GAMEWIZARD_FIRST_BLINK_EXIST_INVITE_BTN)
		ShowMinimapOpenPworldXinShouBtn(ExistenceExp_WaitBtn, "������ٴδ򿪻����")
	end
end

-- ����뿪���а�ť
function ExistenceExpInviteFrame_LeaveButton_OnClick()
	ExistenceExpInviteFrame:Hide()
	GameBattle:ExitQueue()
end

function ExistenceExp_WaitBtn_OnShow()
	if not GameWizard:isDo(GAMEWIZARD_FIRST_BLINK_EXIST_WAIT_BTN) then
		GameWizard:doStep(GAMEWIZARD_FIRST_BLINK_EXIST_WAIT_BTN)
		ShowMinimapOpenPworldXinShouBtn(ExistenceExp_WaitBtn, "�㱨���μ��˻")
	end
end

function ExistenceExp_WaitBtn_OnUpdate()
	if this:GetClientString() == "ĩ����ҫ����" then
		return
	end
	if IsExistenceExpInviteOutOfTime() then
		ExistenceExp_WaitBtn:Hide();		
	end
end
function ExistenceExp_WaitBtn_OnClick()
	if this:GetClientString() == "ĩ����ҫ����" then
		SpartaApplyWaitFrame:Show()
		return
	end
	if ExistenceExpInviteFrame:IsShown() then
		ExistenceExpInviteFrame:Hide()
	else
		HideMinimapOpenPworldXinShouBtn(this)
		--�����Ҫ����ƥ��״̬������֮��ս�������ڵȴ��Ķ�����
		if ExistenceExp_WaitBtn:GetClientUserData(0) == 1 and lastWaitTime then
			ClanWarMatchFrame:SetClientUserData( 0, pworldIdx )
			ClanWarMatchFrame:Show()
			return
		end
		if not BattleConfirmFrame:IsShown() then
			ExistenceExpInviteFrameTitleName:SetText("")
			ExistenceExpInviteFrame_DescRich:SetText("", 255, 254, 189)
			ExistenceExpInviteFrame:Show()
		end
	end
end

function ExistenceExp_WaitBtn_OnEnter()
	local name = this:GetName()
	if this:GetClientString() == "ĩ����ҫ����" then
		SetGameTooltips(name, "ĩ����ҫ���ö�����")
		return
	end
	if lastWaitTime then
		local pastTime = getServerTime().sec - lastWaitTime
		SetGameTooltips(name, string.format("%s�Ŷ���\n���Ѿ��ȴ���#cF16F06%2d#n��#cF16F06%2d#n��", pworldName, math.floor(pastTime / 60), pastTime % 60))
	elseif lastInviteTime then
		SetGameTooltips(name, "����"..pworldName)
	end
end

function ExistenceExp_WaitBtn_OnLeave()
	GameTooltipRichText:SetText("",255,255,255)
	GameTooltip:Hide()
end

function BattleConfirmFrame_OnShow()
	ClanWarMatchOpenApplyFrame:Hide()
	ClanWarMatchFrame:Hide()
	if ExistenceExpInviteFrame:IsShown() then
		ExistenceExpInviteFrame:Hide()
		if lastInviteTime then
			refreshedShowInvite = true
		end
	end
end

function BattleConfirmFrame_OnHide()
	if refreshedShowInvite then
		ExistenceExpInviteFrameTitleName:SetText("")
		ExistenceExpInviteFrame_DescRich:SetText("", 255, 254, 189)
		ExistenceExpInviteFrame:Show()
	end
end

function BattleLeaveConfirmFrameOK_OnClick()
	util.closeParent(this)
	GameBattle:LeavePworld()
end

function SortBattleSummary( battleSummarys,sortOrder,sortElement )	
	table.sort( battleSummarys,function(a,b)
		if sortOrder then
			if a[sortElement] == b[sortElement] then
				if sortElement == "groupResult" then
					if a["camp"] == b["camp"] then
						return a["idx"] > b["idx"]
					else
						return a["camp"] > b["camp"]
					end
				elseif sortElement == "result" then
					return a["idx"] < b["idx"]
				else
					return a["result"] > b["result"]
				end
			end
			return a[sortElement] > b[sortElement]
		else
			if a[sortElement] == b[sortElement] then
				if sortElement == "groupResult" then
					if a["camp"] == b["camp"] then
						return a["idx"] < b["idx"]
					else
						return a["camp"] < b["camp"]
					end
					return a["idx"] < b["idx"]
				elseif sortElement == "result" then
					return a["idx"] > b["idx"]
				else
					return a["result"] < b["result"]
				end
			end
			return a[sortElement] < b[sortElement]
		end
	end)
end