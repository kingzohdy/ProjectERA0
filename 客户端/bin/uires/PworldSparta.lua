
local SPARTA_PWORLD_ACTIVITY_ID = 48
-----------------------------------------------------末世荣耀殿堂结束玩家信息面板------------------------------------------------------

local t_WinStateTex = {
	["完胜"] = { x = 771, y = 106, w = 30, h = 16, },
	["胜利"] = { x = 802, y = 106, w = 30, h = 16, },
	["平局"] = { x = 740, y = 106, w = 30, h = 16, },
	["失败"] = { x = 833, y = 106, w = 30, h = 16, },
	["逃跑"] = { x = 864, y = 106, w = 30, h = 16, },	
}

function SpartaPlayersFrame_OnLoad()
end

function GetTeamWinSata( teamIdx )
	local VCamp = GameSparta:GetSpartaVCamp()
	local selfCamp = GameSparta:GetSelfCampIdx()
	--胜利
	if VCamp == teamIdx then
		local enemyKill = selfCamp == teamIdx and GameSparta:GetEnemyTeamKill() or GameSparta:GetSelfTeamKill()
		if enemyKill == 0 then
			return "完胜"
		else
			return "胜利"
		end
	--平局
	elseif VCamp == 0 then
		if GameSparta:GetSelfTeamKill() == GameSparta:GetEnemyTeamKill() then
			return "平局"
		end
	--失败
	else
		return "失败"
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
			winState = "逃跑"
		end
		local ResultTex = getglobal( playerBtn:GetName().."Result" )
		ResultTex:SetTexUV( t_WinStateTex[winState].x, t_WinStateTex[winState].y, t_WinStateTex[winState].w, t_WinStateTex[winState].h )		
		local NameFont = getglobal( playerBtn:GetName().."Name" )
		local color = { 255,255,190, }
		if mainplayer:getName() == playerInfo.RoleName then
			color = { 250,180,10, }
			--根据自己获得的荣誉来设置显示信息
			local szSpartaInfo = string.format( "本次获得殿堂荣誉%d点 （今日最高可获得300点）。\n#c00ff00殿堂荣誉奖励获得时间：周日19：30-23：00", playerInfo.SpartaGlory )
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
-----------------------------------------------------末世荣耀殿堂过程中玩家信息面板--------------------------------------------
local MAX_SPARTA_ENEMY_NUM = 3
local t_spartaRaceTexture = 
-- 根据职业来设置职业图片
{
	-- 战士单手  守护者
	[g_nCAREER_WARRIOR_SINGEL]	= { path = "uires\\ZhuJieMian\\3.dds", UV = { x = 376, y = 488, width = 10, height = 12 } },
	-- 战士双手  征服者
	[g_nCAREER_WARRIOR_DOUBLE]	= { path = "uires\\ZhuJieMian\\3.dds", UV = { x = 365, y = 488, width = 10, height = 12 } },

	-- 法师单手  聚能者
	[g_nCAREER_MAGICOR_SINGEL]	= { path = "uires\\ZhuJieMian\\3.dds", UV = { x = 394, y = 501, width = 10, height = 12 } },
	-- 法师双手  炼魂师
	[g_nCAREER_MAGICOR_DOUBLE]	= { path = "uires\\ZhuJieMian\\3.dds", UV = { x = 405, y = 501, width = 10, height = 12 } },

	-- 枪手单手  猎杀者
	[g_nCAREER_GUNER_SINGEL]	= { path = "uires\\ZhuJieMian\\3.dds", UV = { x = 372, y = 501, width = 10, height = 12 } },
	-- 枪手双手	毁灭者
	[g_nCAREER_GUNER_DOUBLE]	= { path = "uires\\ZhuJieMian\\3.dds", UV = { x = 383, y = 501, width = 10, height = 12 } },

	-- 祭司单手	圣佑者
	[g_nCAREER_CHURCH_SINGEL]	= { path = "uires\\ZhuJieMian\\3.dds", UV = { x = 398, y = 488, width = 10, height = 12 } },
	-- 祭司双手	驭雷师
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
			SpartaWarInfoFrameTime:SetText(string.format("剩余时间：%02d:%02d:%02d", math.floor(nSpartaPworldLeaveTime / 3600), math.floor((nSpartaPworldLeaveTime % 3600) / 60), nSpartaPworldLeaveTime % 60))
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
	--设置双方击杀数
	SpartaWarInfoFrameSelfNum:SetText( GameSparta:GetSelfTeamKill() )
	SpartaWarInfoFrameEnemyNum:SetText( GameSparta:GetEnemyTeamKill() )
	--设置剩余时间
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
		SpartaWarInfoFrameTime:SetText(string.format("时间：%02d:%02d:%02d", math.floor(leaveTime / 3600), math.floor((leaveTime % 3600) / 60), leaveTime % 60))
		nSpartaPworldLeaveTime = leaveTime
	end
	--玩家逃跑时，会有一个显示错误，so,在此清除上一次的敌人信息,
	for i = 1, MAX_SPARTA_ENEMY_NUM do
		local btn = getglobal( "SpartaWarInfoFrameEnemyMemberInfo"..i )
		local raceTex = getglobal( btn:GetName().."Race" )
		raceTex:Hide()
		local NameFont = getglobal( btn:GetName().."Name" )
		NameFont:SetText( "已逃跑" )
		NameFont:SetTextColor( 255,0,0 )
		local hpTex = getglobal( btn:GetName().."Hp" )
		hpTex:Hide()
		btn:SetClientUserData( 0, 0 )
	end
	--设置敌人信息
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
--------------------------------------------末世殿堂准备面板-----------------------------------------------
local t_SpartaReadyTex = {
	[0] = { x = 361, y = 84, w = 48, h = 48 },
	[1] = { x = 312, y = 84, w = 48, h = 48 },
}
local nShowStartTime = 0
function UpdateSpartaGetReady()
	--进入准备后，等待结束，隐藏等待界面
	SpartaApplyWaitFrame:Hide()
	ExistenceExp_WaitBtn:Hide()
	ExistenceExp_WaitBtn:SetClientString( "" )
	--显示准备界面
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
	SpartaGetReadyFrameHasReadyNum:SetText( string.format( "%d/6已准备就绪", readyPlayerNum ) )
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
		SpartaGetReadyFrameRemind:SetText( string.format( "若%ds内未准备，系统将自动准备", nLastTime ) )
	end
end

function SpartaGetReadyFrameReadyBtn_OnClick()
	GameSparta:SpartaGetReady()
end

---------------------------------------------------------末世荣耀殿堂活动引导面板--------------------------------------
function SpartaPworldGuidFrameTipsButtonOnEnter()
	SetGameTooltips( this:GetName(),"点击“报名”按键，可打开末世荣耀殿堂进行报名" )
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
		szDesc = "#cffffbe活动开启时间：9：00-23：00  参与获取末世功勋\n#c00ff00奖励时间：19：30-23：00 参与额外获取殿堂荣誉\n#cff0000每周参与场次不足10场将会减少100点末世功勋"
	end
	SpartaPworldGuidFrameDesc:SetText( szDesc, 255,255,190 )
	--设置面板大小
	if SpartaPworldGuidFrameDesc:GetTextLines() > 3 then
		SpartaPworldGuidFrame:SetHeight( 74 + SpartaPworldGuidFrameDesc:GetTextLines() * 16 );
		SpartaPworldGuidFrameDesc:SetHeight( SpartaPworldGuidFrameDesc:GetTextLines() * 16 );
	else
		SpartaPworldGuidFrame:SetHeight( 122 );
		SpartaPworldGuidFrameDesc:SetHeight( 48 );
	end
	SpartaPworldGuidFrameDesc:resizeText( SpartaPworldGuidFrameDesc:GetWidth() * GetScreenScaleY(), SpartaPworldGuidFrameDesc:GetHeight() * GetScreenScaleY() );
end

-----------------------------------------------------末世荣耀殿堂等待面板----------------------
local t_WaitDesc = {
	[1] = [[#cffffbe 末世荣耀殿堂排队中，系统正在为您选择对手，
#cffffbe			请耐心等待！]],
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
		ExistenceExp_WaitBtn:SetClientString( "末世荣耀殿堂" )
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
	
