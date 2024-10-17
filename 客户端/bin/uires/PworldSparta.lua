
local SPARTA_PWORLD_ACTIVITY_ID = 48
-----------------------------------------------------ĩ����ҫ���ý��������Ϣ���------------------------------------------------------

local t_WinStateTex = {
	["��ʤ"] = { x = 771, y = 106, w = 30, h = 16, },
	["ʤ��"] = { x = 802, y = 106, w = 30, h = 16, },
	["ƽ��"] = { x = 740, y = 106, w = 30, h = 16, },
	["ʧ��"] = { x = 833, y = 106, w = 30, h = 16, },
	["����"] = { x = 864, y = 106, w = 30, h = 16, },	
}

function SpartaPlayersFrame_OnLoad()
end

function GetTeamWinSata( teamIdx )
	local VCamp = GameSparta:GetSpartaVCamp()
	local selfCamp = GameSparta:GetSelfCampIdx()
	--ʤ��
	if VCamp == teamIdx then
		local enemyKill = selfCamp == teamIdx and GameSparta:GetEnemyTeamKill() or GameSparta:GetSelfTeamKill()
		if enemyKill == 0 then
			return "��ʤ"
		else
			return "ʤ��"
		end
	--ƽ��
	elseif VCamp == 0 then
		if GameSparta:GetSelfTeamKill() == GameSparta:GetEnemyTeamKill() then
			return "ƽ��"
		end
	--ʧ��
	else
		return "ʧ��"
	end
	
end

function SpartaEnd()
	local mainplayer = ActorMgr:getMainPlayer()
	SpartaPlayersFrame:Show()
	for i = 1, MAX_SPARTA_ROLE_TOLUA do
		local playerBtn = getglobal( "SpartaPlayersFramePlayer"..i )
		local playerInfo = GameSparta:GetSpartaEndInfo( i - 1 );
		local winState = GetTeamWinSata( playerInfo.Camp )
		if playerInfo.Leave == 1 then
			winState = "����"
		end
		local ResultTex = getglobal( playerBtn:GetName().."Result" )
		ResultTex:SetTexUV( t_WinStateTex[winState].x, t_WinStateTex[winState].y, t_WinStateTex[winState].w, t_WinStateTex[winState].h )		
		local NameFont = getglobal( playerBtn:GetName().."Name" )
		local color = { 255,255,190, }
		if mainplayer:getName() == playerInfo.RoleName then
			color = { 250,180,10, }
			--�����Լ���õ�������������ʾ��Ϣ
			local szSpartaInfo = string.format( "���λ�õ�������%d�� ��������߿ɻ��300�㣩��\n#c00ff00���������������ʱ�䣺����19��30-23��00", playerInfo.SpartaGlory )
			SpartaPlayersFrameInfo:SetText( szSpartaInfo,255,255,190 )
		end
		NameFont:SetText( playerInfo.RoleName )
		NameFont:SetTextColor( color[1], color[2], color[3] )
		local HonorFont = getglobal( playerBtn:GetName().."Honor" )
		HonorFont:SetText( "+"..playerInfo.SpartaGlory )
		HonorFont:SetTextColor( color[1], color[2], color[3] )
		local FightFont = getglobal( playerBtn:GetName().."Fight" )
		FightFont:SetText( (playerInfo.FightVal+playerInfo.ChgFightVal) )
		FightFont:SetTextColor( color[1], color[2], color[3] )
		local FightAddFont = getglobal( playerBtn:GetName().."FightAdd" )
		if playerInfo.ChgFightVal >= 0 then
			FightAddFont:SetTextColor( 9,250,255 )
		else
			FightAddFont:SetTextColor( 250,0,0 )
		end
		FightAddFont:SetText( string.format( "(%s%d)", playerInfo.ChgFightVal > 0 and "+" or "", playerInfo.ChgFightVal ) )		
	end
end
-----------------------------------------------------ĩ����ҫ���ù����������Ϣ���--------------------------------------------
local MAX_SPARTA_ENEMY_NUM = 3
local t_spartaRaceTexture = 
-- ����ְҵ������ְҵͼƬ
{
	-- սʿ����  �ػ���
	[g_nCAREER_WARRIOR_SINGEL]	= { path = "uires\\ZhuJieMian\\3.dds", UV = { x = 376, y = 488, width = 10, height = 12 } },
	-- սʿ˫��  ������
	[g_nCAREER_WARRIOR_DOUBLE]	= { path = "uires\\ZhuJieMian\\3.dds", UV = { x = 365, y = 488, width = 10, height = 12 } },

	-- ��ʦ����  ������
	[g_nCAREER_MAGICOR_SINGEL]	= { path = "uires\\ZhuJieMian\\3.dds", UV = { x = 394, y = 501, width = 10, height = 12 } },
	-- ��ʦ˫��  ����ʦ
	[g_nCAREER_MAGICOR_DOUBLE]	= { path = "uires\\ZhuJieMian\\3.dds", UV = { x = 405, y = 501, width = 10, height = 12 } },

	-- ǹ�ֵ���  ��ɱ��
	[g_nCAREER_GUNER_SINGEL]	= { path = "uires\\ZhuJieMian\\3.dds", UV = { x = 372, y = 501, width = 10, height = 12 } },
	-- ǹ��˫��	������
	[g_nCAREER_GUNER_DOUBLE]	= { path = "uires\\ZhuJieMian\\3.dds", UV = { x = 383, y = 501, width = 10, height = 12 } },

	-- ��˾����	ʥ����
	[g_nCAREER_CHURCH_SINGEL]	= { path = "uires\\ZhuJieMian\\3.dds", UV = { x = 398, y = 488, width = 10, height = 12 } },
	-- ��˾˫��	Ԧ��ʦ
	[g_nCAREER_CHURCH_DOUBLE]	= { path = "uires\\ZhuJieMian\\3.dds", UV = { x = 387, y = 488, width = 10, height = 12 } },
};
function SpartaWarInfoFrameDropDownBtn_OnClick()
	SpartaWarInfoFrameDropDownBtn:Hide()	
	SpartaWarInfoFrameEnemyMember:Show()
	SpartaWarInfoFrameEnemyMemberCloseUpBtn:Show()
end

function SpartaWarInfoFrameEnemyMemberCloseUpBtn_OnClick()
	SpartaWarInfoFrameDropDownBtn:Show()
	SpartaWarInfoFrameEnemyMember:Hide()
	SpartaWarInfoFrameEnemyMemberCloseUpBtn:Hide()
end

function SpartaWarInfoFrame_OnLoad()
	this:RegisterEvent( "GE_ENTER_PWORLD" )
	this:RegisterEvent( "GE_LEAVE_PWORLD" )
end
local nSpartaPworldLeaveTime = 0
local nSpartaPworldLastTime = 0
function SpartaWarInfoFrame_OnEvent()
	if arg1 == "GE_ENTER_PWORLD" then
		if GameSparta:IsInSpartaPworld() then
			local mainplayer = ActorMgr:getMainPlayer()
			if mainplayer == nil then return end
			if mainplayer:getPworldLeaveTime() < 0 then return end
			nSpartaPworldLeaveTime = mainplayer:getPworldLeaveTime();
			nSpartaPworldLastTime = os.clock()
			SpartaWarInfoFrameTime:SetText(string.format("ʣ��ʱ�䣺%02d:%02d:%02d", math.floor(nSpartaPworldLeaveTime / 3600), math.floor((nSpartaPworldLeaveTime % 3600) / 60), nSpartaPworldLeaveTime % 60))
			SpartaWarInfoFrame:Show()
			SpartaGetReadyFrame:Hide()
			CloseSpecialActivityGuid( SPARTA_PWORLD_ACTIVITY_ID )
		end
	elseif arg1 == "GE_LEAVE_PWORLD" then
		if GameSparta:IsInSpartaPworld() then
			SpartaWarInfoFrame:Hide()
		end
	end
end
function UpdateSpartaWarInfo()
	--����˫����ɱ��
	SpartaWarInfoFrameSelfNum:SetText( GameSparta:GetSelfTeamKill() )
	SpartaWarInfoFrameEnemyNum:SetText( GameSparta:GetEnemyTeamKill() )
	--����ʣ��ʱ��
	if nSpartaPworldLeaveTime > 0 then
		local now = os.clock()
		local leaveTime = nSpartaPworldLeaveTime - ( now - nSpartaPworldLastTime )
		nSpartaPworldLastTime = now
		if leaveTime < 0 then
			leaveTime = 0
		end
		if math.floor(leaveTime) == 296 and math.floor( nSpartaPworldLeaveTime ) == 297 then
			playUiEffect(0,0,2,"effect\\entity\\JJCdaojishi.ent")
		end
		SpartaWarInfoFrameTime:SetText(string.format("ʱ�䣺%02d:%02d:%02d", math.floor(leaveTime / 3600), math.floor((leaveTime % 3600) / 60), leaveTime % 60))
		nSpartaPworldLeaveTime = leaveTime
	end
	--�������ʱ������һ����ʾ����so,�ڴ������һ�εĵ�����Ϣ,
	for i = 1, MAX_SPARTA_ENEMY_NUM do
		local btn = getglobal( "SpartaWarInfoFrameEnemyMemberInfo"..i )
		local raceTex = getglobal( btn:GetName().."Race" )
		raceTex:Hide()
		local NameFont = getglobal( btn:GetName().."Name" )
		NameFont:SetText( "������" )
		NameFont:SetTextColor( 255,0,0 )
		local hpTex = getglobal( btn:GetName().."Hp" )
		hpTex:Hide()
		btn:SetClientUserData( 0, 0 )
	end
	--���õ�����Ϣ
	local enemyCount = 0
	for i = 1, ActorMgr:getActorNums() do
		local actor = ActorMgr:getActor( i - 1 )
		if actor and actor:getType() == GAT_PLAYER then
			if not TeamManager:isInTeam( GetPlayerName(actor:getName()) ) then
				enemyCount = enemyCount + 1;
				if enemyCount > MAX_SPARTA_ENEMY_NUM then
					break
				end
				local btn = getglobal( "SpartaWarInfoFrameEnemyMemberInfo"..enemyCount )
				local raceTex = getglobal( btn:GetName().."Race" )
				local raceUV = 	t_spartaRaceTexture[actor:getRace()].UV
				raceTex:SetTexUV( raceUV.x, raceUV.y, raceUV.width, raceUV.height )
				raceTex:Show()				
				local NameFont = getglobal( btn:GetName().."Name" )
				NameFont:SetText( GetPlayerName(actor:getName()) )
				NameFont:SetTextColor( 255,255,190 )
				local hpTex = getglobal( btn:GetName().."Hp" )
				local maxHp = actor:getHP() > actor:getMaxHP() and actor:getHP() or actor:getMaxHP()			
				hpTex:SetSize( 113*actor:getHP()/maxHp, 5 )
				hpTex:Show()
				btn:SetClientUserData( 0, actor:GetID() )
			end
		end
	end
end
function SpartaWarInfoFrame_OnShow()
end
function SpartaWarInfoFrame_OnUpdate()	
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then return end
	UpdateSpartaWarInfo()
end
function SpartaEnemyMemTemplate_OnClick()
	local mainplayer = ActorMgr:getMainPlayer()
	mainplayer:SetSelectTarget( this:GetClientUserData(0) );
end
--------------------------------------------ĩ������׼�����-----------------------------------------------
local t_SpartaReadyTex = {
	[0] = { x = 361, y = 84, w = 48, h = 48 },
	[1] = { x = 312, y = 84, w = 48, h = 48 },
}
local nShowStartTime = 0
function UpdateSpartaGetReady()
	--����׼���󣬵ȴ����������صȴ�����
	SpartaApplyWaitFrame:Hide()
	ExistenceExp_WaitBtn:Hide()
	ExistenceExp_WaitBtn:SetClientString( "" )
	--��ʾ׼������
	if not SpartaGetReadyFrame:IsShown() then
		SpartaGetReadyFrame:Show()
		nShowStartTime = os.clock()
	end
	local mainplayer = ActorMgr:getMainPlayer()
	local readyPlayerNum = 0
	for i = 1, MAX_SPARTA_ROLE_TOLUA do
		local btn = getglobal( "SpartaGetReadyFramePlayer"..i )
		local roleName = GameSparta:GetSpartaRoleName( i - 1 )
		btn:SetClientString( roleName )
		local nReady = GameSparta:IsSpartaRoleReady( i - 1 )
		btn:SetClientUserData( 0, nReady )
		local btnReadyTex = getglobal( btn:GetName().."Tex" )
		btnReadyTex:SetTexUV( t_SpartaReadyTex[nReady].x, t_SpartaReadyTex[nReady].y, t_SpartaReadyTex[nReady].w, t_SpartaReadyTex[nReady].h )
		if mainplayer:getName() == roleName then
			if nReady == 1 then
				SpartaGetReadyFrameReadyBtn:Disable();
			else
				SpartaGetReadyFrameReadyBtn:Enable();
			end
		end
		if nReady == 1 then
			readyPlayerNum = readyPlayerNum + 1
		end
	end
	SpartaGetReadyFrameHasReadyNum:SetText( string.format( "%d/6��׼������", readyPlayerNum ) )
end

function SpartaGetReadyFrame_OnShow()
end

function SpartaGetReadyFrame_OnUpdate()
	local now = os.clock()
	if now - nShowStartTime >= 30 then
		SpartaGetReadyFrame:Hide()
		nShowStartTime = 0
	else
		local nLastTime = 30 - math.floor( now - nShowStartTime )
		SpartaGetReadyFrameRemind:SetText( string.format( "��%ds��δ׼����ϵͳ���Զ�׼��", nLastTime ) )
	end
end

function SpartaGetReadyFrameReadyBtn_OnClick()
	GameSparta:SpartaGetReady()
end

---------------------------------------------------------ĩ����ҫ���û�������--------------------------------------
function SpartaPworldGuidFrameTipsButtonOnEnter()
	SetGameTooltips( this:GetName(),"������������������ɴ�ĩ����ҫ���ý��б���" )
end

function SpartaPworldGuidFrameApplyButton_OnClick()
	CloseSpecialActivityGuid( SPARTA_PWORLD_ACTIVITY_ID )
	util.closeParent(this)
	BattleStartFrame:Show()
end

function SpartaPworldGuidFrameCloseButton_OnClick()
	CloseSpecialActivityGuid( SPARTA_PWORLD_ACTIVITY_ID )
	util.closeParent(this)
end

function SpartaPworldGuidFrame_OnShow()
	local szDesc = ""
	if SpartaPworldGuidFrame:GetClientUserData(0) == SPARTA_PWORLD_ACTIVITY_ID then
		szDesc = "#cffffbe�����ʱ�䣺9��00-23��00  �����ȡĩ����ѫ\n#c00ff00����ʱ�䣺19��30-23��00 ��������ȡ��������\n#cff0000ÿ�ܲ��볡�β���10���������100��ĩ����ѫ"
	end
	SpartaPworldGuidFrameDesc:SetText( szDesc, 255,255,190 )
	--��������С
	if SpartaPworldGuidFrameDesc:GetTextLines() > 3 then
		SpartaPworldGuidFrame:SetHeight( 74 + SpartaPworldGuidFrameDesc:GetTextLines() * 16 );
		SpartaPworldGuidFrameDesc:SetHeight( SpartaPworldGuidFrameDesc:GetTextLines() * 16 );
	else
		SpartaPworldGuidFrame:SetHeight( 122 );
		SpartaPworldGuidFrameDesc:SetHeight( 48 );
	end
	SpartaPworldGuidFrameDesc:resizeText( SpartaPworldGuidFrameDesc:GetWidth() * GetScreenScaleY(), SpartaPworldGuidFrameDesc:GetHeight() * GetScreenScaleY() );
end

-----------------------------------------------------ĩ����ҫ���õȴ����----------------------
local t_WaitDesc = {
	[1] = [[#cffffbe ĩ����ҫ�����Ŷ��У�ϵͳ����Ϊ��ѡ����֣�
#cffffbe			�����ĵȴ���]],
}
function SpartaApplyWaitFrame_OnLoad()
	SpartaApplyWaitFrameDesc:setCenterLine(true)
	SpartaApplyWaitFrameDesc:SetText( t_WaitDesc[1], 255,255,190 )
end
function SpartaApplyWaitFrameKnownButton_OnClick()
	util.closeParent(this)
end
function SpartaApplyWaitFrameLeaveButton_OnClick()
	GameSparta:SpartaExitApply()
end
function SpartaApplyResult( nFlag )
	if nFlag == 1 then
		BattleStartFrame:Hide()
		SpartaApplyWaitFrame:Show()
		ExistenceExp_WaitBtn:Show()
		CloseSpecialActivityGuid( SPARTA_PWORLD_ACTIVITY_ID )
		ExistenceExp_WaitBtn:SetClientString( "ĩ����ҫ����" )
	end
end

function SpartaApplyEixtApplyResult( nFlag )
	if nFlag == 1 then
		SpartaGetReadyFrame:Hide()
		SpartaApplyWaitFrame:Hide()
		ExistenceExp_WaitBtn:Hide()
		ExistenceExp_WaitBtn:SetClientString( "" )
	end
end
	
