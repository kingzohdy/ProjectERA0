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
		--print( "PreLogOutAction--�ܾ�����" );
		GameExchange:cancelExchange( ROLE_LOGOUT_REJECT );
		ClearTradeFrame();
	end
	
	RejectAllExchangeInvite( ROLE_LOGOUT_REJECT );
	
	IgnoreAllAddFriendRequests();
	IgnoreAllTeamRequests();
end

function BackToRoleSelect_OnClick()
	SaveLastSelectPetInfo();
	-- ���ѡ�񷵻ؽ�ɫѡ�����ʱ��ֹͣ�ƶ�״̬��ͬʱ����������������� 
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
	MessageBox("��ʾ","�Ƿ�Ҫ�뿪��Ϸ���������ռ䣿");
	MessageBoxFrame:SetClientString("�����ռ�");
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
	MessageBox("��ʾ","�Ƿ�Ҫ�뿪��Ϸ��");	
	MessageBoxFrame:SetClientString("�뿪��Ϸ");
	if AutoAssist:getAutoAssistBegin() then
		AutoAssistFrame_OnClick();
	end
	SaveLastSelectPetInfo();
end

function SimpleClient_OnClick()
	SystemSelectFrame:Hide();
	MessageBox("�˳���Ϸ","��ȷ��Ҫ�˳���Ϸ����ʼ�һ���");
end

----------------------------------------�˳��������鲿��---------------------------------------
local t_AwardTypeTexUV = {
	--��������
	[1] = { x = 810, y = 284, w = 55, h = 16, },
	--��������
	[2] = { x = 866, y = 284, w = 55, h = 16, },
	--��������
	[3] = { x = 922, y = 284, w = 55, h = 16, },
	--��Ʒװ��
	[4] = { x = 907, y = 203, w = 55, h = 16, },
	--��������
	[5] = { x = 907, y = 220, w = 55, h = 16, },
}
local MAX_EVERY_DAY_ACTIVE = 4
local MAX_EVERY_DAY_ACTIVES_NUM = 10
local MAX_CLAN_AWARD_ACTIVE = 2
local	t_EveryDayActiveInfo = {
		{ name = "��ҹ��\t\t\t\t\t", pworldID = 112, PromotionID = 8, taskID = 0, awardType = 4, LimitNum = 1, doNum = 0, notFinish = 1, level = 3, playerLvLimit = 28, desIndex = 1, },
		{ name = "�ƽ�󾮣�ר�ң�", pworldID = 113, PromotionID = 10, taskID = 0, awardType = 4,  LimitNum = 1, doNum = 0, notFinish = 1, level = 3, playerLvLimit = 50, desIndex = 1, },
		{ name = "ɭ�ֽ�磨ר�ң�", pworldID = 119, PromotionID = 21, taskID = 0, awardType = 4,  LimitNum = 1, doNum = 0, notFinish = 1, level = 3, playerLvLimit = 60, desIndex = 1, },
		{ name = "������أ�ר�ң�", pworldID = 114, PromotionID = 15, taskID = 0, awardType = 4,  LimitNum = 1, doNum = 0, notFinish = 1, level = 3, playerLvLimit = 70, desIndex = 1, },
		{ name = "���֮��\t\t\t\t", pworldID = 300, PromotionID = 9, taskID = 0, awardType = 3,  LimitNum = 1, doNum = 0, notFinish = 1, level = 2, playerLvLimit = 30, desIndex = 1, },
		{ name = "��ţ\t\t\t\t\t\t", pworldID = 0, PromotionID = 14, taskID = 2000, awardType = 3,  LimitNum = 1, doNum = 0, notFinish = 1, level = 2, playerLvLimit = 40, desIndex = 1, },
		{ name = "�´�������\t\t\t", pworldID = 211, PromotionID = 1, taskID = 0, awardType = 1,  LimitNum = 1, doNum = 0, notFinish = 1, level = 1, playerLvLimit = 45, desIndex = 1, },
		{ name = "���⺣̲\t\t\t\t", pworldID = 0, PromotionID = 18, taskID = 30039, awardType = 2,  LimitNum = 1, doNum = 0, notFinish = 1, level = 2, playerLvLimit = 50, desIndex = 1, },
		{ name = "��ɽ��\t\t\t\t\t", pworldID = 101, PromotionID = 3, taskID = 0, awardType = 1,  LimitNum = 1, doNum = 0, notFinish = 1, level = 1, playerLvLimit = 65, desIndex = 1, },
		{ name = "̫�����\t\t\t\t", pworldID = 100, PromotionID = 13, taskID = 0, awardType = 1,  LimitNum = 1, doNum = 0, notFinish = 1, level = 1, playerLvLimit = 120, desIndex = 1, },
	}
local	 t_ClanAwardInfo = {
		{ name = "���ḣ��\t\t\t\t", awardType = 2, LimitNum = 1, doNum = 0, notFinish = 1, level = 1, desIndex = 2, },
		{ name = "�������\t\t\t\t", awardType = 3, LimitNum = 1, doNum = 0, notFinish = 1, level = 1, desIndex = 3, },
	}
local	 t_ActivityAwardInfo = {	
		{ name = "����Ŀ�꽱��\t\t", awardType = 3, LimitNum = 4, doNum = 0, notFinish = 1, level = 2, desIndex = 4, },
	}
local t_ClanAwardDes = {
	"�ɴ򿪻�������鿴��ػ�����뽱��",
	"���ḣ��������[ʱ���]#c00ff00�����#n����ȡ����ȡ�����ɻ�ô������齱��", 
	"�������������[ʱ���]#c00ff00�����#n����ȡ����ȡ����ɻ�ô������߽���", 
--	"����Ŀ�꽱����ÿ����ɽ���Ŀ������󣬴򿪻���ֵ���·���ȡ������ť��������ȼ�Ʒ����",
}
function SystemSelectFrame_OnLoad()
end
function SystemSelectFrame_OnShow()
	--������а�ť��ѡ��״̬
	for i = 1, MAX_EVERY_DAY_ACTIVE do
		local btn = getglobal( "SystemSelectFrameEveryDayActiveBtn"..i);
		btn:DisChecked();
	end
	--Ĭ��ѡ���һ��
	SystemSelectFrameEveryDayActiveBtn1:Checked();
end
function InitSystemEscInfoList()
	t_EveryDayActiveInfo = {
		{ name = "��ҹ��\t\t\t\t\t", pworldID = 112, PromotionID = 8, taskID = 0, awardType = 4, LimitNum = 1, doNum = 0, notFinish = 1, level = 3, playerLvLimit = 28, desIndex = 1, },
		{ name = "�ƽ�󾮣�ר�ң�", pworldID = 113, PromotionID = 10, taskID = 0, awardType = 4,  LimitNum = 1, doNum = 0, notFinish = 1, level = 3, playerLvLimit = 50, desIndex = 1, },
		{ name = "ɭ�ֽ�磨ר�ң�", pworldID = 119, PromotionID = 21, taskID = 0, awardType = 4,  LimitNum = 1, doNum = 0, notFinish = 1, level = 3, playerLvLimit = 60, desIndex = 1, },
		{ name = "������أ�ר�ң�", pworldID = 114, PromotionID = 15, taskID = 0, awardType = 4,  LimitNum = 1, doNum = 0, notFinish = 1, level = 3, playerLvLimit = 70, desIndex = 1, },
		{ name = "���֮��\t\t\t\t", pworldID = 300, PromotionID = 9, taskID = 0, awardType = 3,  LimitNum = 1, doNum = 0, notFinish = 1, level = 2, playerLvLimit = 30, desIndex = 1, },
		{ name = "��ţ\t\t\t\t\t\t", pworldID = 0, PromotionID = 14, taskID = 2000, awardType = 3,  LimitNum = 1, doNum = 0, notFinish = 1, level = 2, playerLvLimit = 40, desIndex = 1, },
		{ name = "�´�������\t\t\t", pworldID = 211, PromotionID = 1, taskID = 0, awardType = 1,  LimitNum = 1, doNum = 0, notFinish = 1, level = 1, playerLvLimit = 45, desIndex = 1, },
		{ name = "���⺣̲\t\t\t\t", pworldID = 0, PromotionID = 18, taskID = 30039, awardType = 2,  LimitNum = 1, doNum = 0, notFinish = 1, level = 2, playerLvLimit = 50, desIndex = 1, },
		{ name = "��ɽ��\t\t\t\t\t", pworldID = 101, PromotionID = 3, taskID = 0, awardType = 1,  LimitNum = 1, doNum = 0, notFinish = 1, level = 1, playerLvLimit = 65, desIndex = 1, },
		{ name = "̫�����\t\t\t\t", pworldID = 100, PromotionID = 13, taskID = 0, awardType = 1,  LimitNum = 1, doNum = 0, notFinish = 1, level = 1, playerLvLimit = 120, desIndex = 1, },
	}
	 t_ClanAwardInfo = {
		{ name = "���ḣ��\t\t\t\t", awardType = 2, LimitNum = 1, doNum = 0, notFinish = 1, level = 1, desIndex = 2, },
		{ name = "�������\t\t\t\t", awardType = 3, LimitNum = 1, doNum = 0, notFinish = 1, level = 1, desIndex = 3, },
	}
	t_ActivityAwardInfo = {	
			{ name = "����Ŀ�꽱��\t\t", awardType = 3, LimitNum = 4, doNum = 0, notFinish = 1, level = 2, desIndex = 4, },
	}
end
function SetSystemSelectFrameActiveInfo()
	InitSystemEscInfoList();
	local mainplayer = ActorMgr:getMainPlayer();
	--������ʾ���ĸ�����
	for i = 1, table.getn( t_EveryDayActiveInfo ) do
		--�����Ƿ����
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
			--���⴦���º�̲����
			if string.find( t_EveryDayActiveInfo[i].name, "���⺣̲" ) then
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
	--�������ȼ�����һ������
	table.sort( t_EveryDayActiveInfo, function(a, b)
		return a.level < b.level;
		end)
	--������ɵ��ƶ����б�β��
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
	--��ѡ�ȼ��������ʵ�����ʾ
	for i = 1, table.getn( t_EveryDayActiveInfo ) do
		if 	t_EveryDayActiveInfo[i].playerLvLimit <= mainplayer:getLv() then
			local temp = t_EveryDayActiveInfo[i];
			table.insert( t_actives, temp );
		end
	end
	--��հ�ť��ǰ����
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
	--���ð�ť��ʾ��Ϣ
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
	SystemSelectFrameBuildUpExp:SetText( "ʣ�����������飺"..szBuildUpExp, 255,255,190 )
	local pworldExp = mainplayer:getPworldExp();
	local maxPworldExp = levelupDef.MaxPworldOffExp;
	SystemSelectFrameMakeUpExp:SetText( "ʣ���������飺"..pworldExp.."/"..maxPworldExp, 255,255,190 )
	if pworldExp == 0 or maxPworldExp == 0 then
		SystemSelectFrameMakeUpExp:Hide();
	else
		SystemSelectFrameMakeUpExp:Show();
	end
	--���ù��ḣ����Ϣ
	local t_awardInfo = {
		["���ḣ��"] = { totalNum = 0, finishNum = 0, },
		["�������"] = { totalNum = 0, finishNum = 0, },
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
		t_awardInfo["���ḣ��"].totalNum = 1
		t_awardInfo["���ḣ��"].finishNum = 0
		t_awardInfo["�������"].totalNum = 1
		t_awardInfo["�������"].finishNum = 0
	end
	if t_awardInfo["���ḣ��"].totalNum == t_awardInfo["���ḣ��"].finishNum then
		t_ClanAwardInfo[1].Show = false;
		t_ClanAwardInfo[1].doNum = 1;
	else
		t_ClanAwardInfo[1].Show = true;
		t_ClanAwardInfo[1].doNum = 0;
	end
	if t_awardInfo["�������"].totalNum == t_awardInfo["�������"].finishNum then
		t_ClanAwardInfo[2].Show = false;
		t_ClanAwardInfo[2].doNum = 1;
	else
		t_ClanAwardInfo[2].Show = true;
		t_ClanAwardInfo[2].doNum = 0;
	end
	--���ý���Ŀ����Ϣ,����Ŀ��ͳ����������
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
	--�������ȼ�����һ������
	table.sort( t_ClanAwardInfo, function(a, b)
		return a.level < b.level;
		end)
	--������ɵ��ƶ����б�β��
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
	--û�й�����ȥ���������
	if not mainplayer:isInClan() then
		for i = 1, table.getn( t_ClanAwardInfo ) do
			if not string.find(  t_ClanAwardInfo[i].name, "����" ) then
				local temp = t_ClanAwardInfo[i]
				table.insert( t_awards, temp );
			end
		end
	else
		t_awards = t_ClanAwardInfo;
	end

	--�ȼ�δ�ﵽ15����ʱ���������Ŀ��
	if mainplayer:getLv() < 15 then
		for i = 1, table.getn( t_awards ) do
			if string.find(  t_awards[i].name, "����Ŀ�꽱��" ) then
				table.remove( t_awards, i );
			end
		end
	end
	--��հ�ť��ǰ����
	for i = 1, MAX_CLAN_AWARD_ACTIVE do
		local btn = getglobal( "SystemSelectFrameClanAwardBtn"..i );
		btn:SetClientUserData( 1, 0 )
		local btnAwardTypeTex = getglobal( "SystemSelectFrameClanAwardBtn"..i.."AwardTypeTex" );
		local btnInfo = getglobal( "SystemSelectFrameClanAwardBtn"..i.."Info" );
		btnAwardTypeTex:SetTexUV( 0, 0, 1, 1 );
		btnInfo:SetText( "", 255,255,190  )
	end
	--���ð�ť��ʾ��Ϣ
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
	-- ������ ����Ŀ�� ��������
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
	--������а�ť��ѡ��״̬
	for i = 1, MAX_EVERY_DAY_ACTIVE do
		local btn = getglobal( "SystemSelectFrameEveryDayActiveBtn"..i);
		btn:DisChecked();
	end
	--���õ�ǰ��ťΪѡ��״̬
	this:Checked();
end
function SystemActivetyBtn_OnDoubleClick()
	if string.find( this:GetName(), "ClanAwardBtn", 1, true ) then return end
	--������а�ť��ѡ��״̬
	for i = 1, MAX_EVERY_DAY_ACTIVE do
		local btn = getglobal( "SystemSelectFrameEveryDayActiveBtn"..i);
		btn:DisChecked();
	end
	--���õ�ǰ��ťΪѡ��״̬
	this:Checked();
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer:getLv() < 15 then
		ShowMidTips( "15��������Ҳſ�ʹ�û����" );
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
		ShowMidTips( "15��������Ҳſ�ʹ�û����" );
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