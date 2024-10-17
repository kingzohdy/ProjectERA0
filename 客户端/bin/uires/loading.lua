local nLogin_flag			= 1;		--标识本次show loadingFrame是不是在登录时。
local nBegin_time			= 0;
local nEnd_loading_flag			= 0;		--用来标识是否收到END_LOADING_FLAG
local firstLogin;
local nMap_ID				= 2000;		--切换到的地图的ID号
local FIRST_LOGIN_TOTAL_TIME		= 6000;		--登录游戏时进度条的持续时间
local CHG_MAP_TOTAL_TIME		= 6000;		--切换场景时进度条的持续时间
local nSelectMap			= 0;		--随机选取的地图编号
local nSelectTip			= 0;		--随机选取的tip编号
local RANDOM_CHOSE_MAP			= 0;		--用于随机选取地图
local RANDOM_CHOSE_TIP			= 1;		--用于随机选取小提示
local DEFAULT_WINDOW_HEIGHT		= 768;		--默认窗口高度
local DEFAULT_HELPTIP_POSTION_Y		= -20;		--默度helptips距离loading界面底部的高度
local MAX_LOADING_NUM			= 10;

local t_RandomTipsInfo =
	{
		"右侧快捷可以自定义自己需要的道具，技能，方便战斗时候使用.",
		"在小地图上点击相应的位置，人物可以自动到达相应的地方.",
		"人物到达三十级时，可以前往时间城“公会管理员”处，创建自己的公会.",
		"您可以在时间城“公会管理员”处，通过“公会查询”功能，选择要加入的公会",
		"与好友组队在野外冒险，进入副本探索都可以增加好友度.",
		"攻击火药桶会发生爆炸，一定范围内的怪物都会受到波及.",
		"使用冰雷会对目标范围内的怪物的造成冰冻的伤害.",
		"按TAB键会自动选中周围的怪！",
		"如果任务遇到问题，放弃任务重新领取",
	}

local t_RandomMapsInfo =
	{
		"loading.jpg","loading2.jpg",
	}
-- loading结束后需要显示的frame列表
local t_FrameShouldShownInWorld =
	{
		"ChatFrameMenuButton",
		"MainChatFrame",
		"ShortCutFrame",
		"PhotoFrame",
		"QuestLogFrame",
		"PhotoFrame",
		"MinimapFrame",
		"TaskTraceFrame",
		"PartyMemberBuffTooltip",
		"GuideAwardFrame",
		"Shortcut_ThreeFrame",
		"ExtendSkillRightFrame", 
		"ExtendSkillFrame",
		"GameMiddleTipsFrame",
		"ShortCutButtonsFrame",
		"TestWarnFrame",
	}
function LoadingFrame_OnLoad()
	this:setUpdateTime(0);
	t_RandomMapsInfo = {};
	for i=1, MAX_LOADING_NUM do
		local path = "uires\\ui\\loading\\loading" .. i .. ".jpg";
		if IsInExistence( path ) then
			table.insert( t_RandomMapsInfo, i );
		end
	end
end
function ProcessBar_OnLoad()
	this:RegisterEvent( "GE_ENTER_MAP" );
	this:RegisterEvent( "GE_ENTER_PLAYERLOGIN" );
	this:RegisterEvent( "GE_KICKOFF_INFO" );
	this:RegisterEvent( "GE_ENTER_PWORLD" );
	this:setUpdateTime( 0 );
	ProcessButtonIcon:SetPoint("bottom", "LoadingFrame", "bottom", -302, -65 );
end

function getRandomNum( nChoseFlag )
	if nChoseFlag == RANDOM_CHOSE_TIP then
		nSelectTip = math.random( 1, table.getn( t_RandomTipsInfo ) );
	else
		nSelectMap = math.random( 1, table.getn( t_RandomMapsInfo ) );
	end
end

function LoadingFrame_OnShow()
	--GameMgr:ShowLoading(true);
	local nMapInTable_Flag	= 0;
	nBegin_time				= GameMgr:getTickTime();

	-- 随机提取并显示小提示
	getRandomNum( RANDOM_CHOSE_TIP );
	HelpInfoShow:SetText( "小技巧:  "..t_RandomTipsInfo[nSelectTip] );
	HelpInfoShow:Show();
	flogBar:Show();
	ProcessButtonIcon:Show();
end

local t_MapAlphaAddSpeed = {
								[2000] = 0.08,
								[4400] = 0.08
							}
function ProcessBar_OnEvent()
	if arg1 == "GE_ENTER_MAP" or arg1 == "GE_ENTER_PLAYERLOGIN" then
		if nEnd_loading_flag == 0 then
			if GameWizard:isDo( GAMEWIZARD_FIRST_ENTERWORLD ) == false then
				GameMgr:playCameraAnim( "db\\Fly\\birth.omod" );
				GameWizard:doStep( GAMEWIZARD_FIRST_ENTERWORLD );
				firstLogin = true;
			else
				firstLogin = false;
			end
		end
		nEnd_loading_flag = 1;
		-- TODO
		LoadingFrame_OnUpdate();
	end

	if arg1 == "GE_KICKOFF_INFO" then
		if LoadingFrame:IsShown() then
			LoadingFrame:Hide();
			ClearFocus();
		end
	end

	if arg1 == "GE_ENTER_MAP" then
		closePopWin();
		local mapid = MapShower:getRealMapId();
		local reginData = getOverlayRegion( mapid );
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer == nil then return end
		local addSpeed = t_MapAlphaAddSpeed[mapid] ~= nil and t_MapAlphaAddSpeed[mapid] or 0.5
		if reginData == nil then
			mainplayer:setOverlayRegion( nil,addSpeed );
		else
			mainplayer:setOverlayRegion(reginData[1],100);
		end
		if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "服务器提示" then
			MessageBoxCancelBtnOnClick();
		end
	end

	if arg1 == "GE_ENTER_PWORLD" then
		if MailHeadFrame:IsShown() then
			MailHeadFrame:Hide();
		end
		if MailSendItemFrame:IsShown() then
			MailSendItemFrame:Hide();
		end
		if MailReadFrame:IsShown() then
			MailReadFrame:Hide();
		end
		if MailSendItemFrame:IsShown() then
			MailSendItemFrame:Hide();
		end
	end
end

local function CheckRoleName()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer ~= nil and string.find( mainplayer:getName(),"*",1,true ) then
		ContainerChangeRoleFrame:SetClientUserData( 0,1 );
		ContainerChangeRoleFrame:Show();
		SetFocusFrame( "ContainerChangeRoleFrame_NameEdit" )
		PlayerContainerFrame:Hide();
	end
end

function LoadingFrame_OnUpdate()
	local helpTips = math.ceil( DEFAULT_HELPTIP_POSTION_Y * LoadingFrame:GetRealHeight() / DEFAULT_WINDOW_HEIGHT )
	HelpInfoShow:SetPoint("bottom", "loadingFrame", "bottom", 0 ,helpTips);

	-- 如果是角色进入游戏
	if nLogin_flag == 1 then
		local scale = ( GameMgr:getTickTime() - nBegin_time ) / FIRST_LOGIN_TOTAL_TIME;
		if scale > 1 then
			scale = 1;
		end
		flogBar:SetSize( scale* 802 , 29 );
		flogBar:SetTexUV(0, 825, scale * 802, 29 );
		flogBar:Show();
		ProcessButtonIcon:SetPoint("bottom", "LoadingFrame", "bottom", -362 + scale * 802, -65 );

		if nEnd_loading_flag == 1 then
			-- 在loading快结束时开始播放游戏动画
			if GameMgr:getTickTime() - nBegin_time >= FIRST_LOGIN_TOTAL_TIME - 50 then
				--RoleInit();
			end
			-- loading结束
			if GameMgr:isMapLoaded() and GameMgr:getTickTime() - nBegin_time >= FIRST_LOGIN_TOTAL_TIME then
				if firstLogin then
					GameMgr:playCameraAnim( "db\\Fly\\birth.omod" );
					ShowCameraCoverFrame( true,1 );
				else
					ShowFramesInWorld();
					-- 这个 伤害统计是否需要继续显示
					if GameUserConfig:getDamageConfigData() == 1 then
						DamageCountFrame:Show();
					end
					--检测vip 网吧
					CheckMainPlayerVipBar();
					CheckRoleName();
					local mainplayer = ActorMgr:getMainPlayer();
					if mainplayer == nil then return end
					if mainplayer:getLv() >= 15 then 
						ActivityLogFrame:Show();
					end
					--[[
					if mainplayer:getLv() >= 15 and LoginMgr:isTodayFirstLogin() then
						ActivityShowBtn_nClick()
					end
					--]]
				end

				nEnd_loading_flag	= 0;
				nLogin_flag		= 0;
				LoadingFrame:Hide();
				--ExtendSkillLeftFrame:Show();
			end
		end
		SetPetDiaologClock();
	-- 如果是角色在游戏中切换场景
	else
		local scale =  ( GameMgr:getTickTime() - nBegin_time ) / CHG_MAP_TOTAL_TIME;
		if scale > 1 then
			scale = 1;
		end
		ProcessButtonIcon:SetPoint( "bottom", "LoadingFrame", "bottom", -362 + scale* 802, -65 );
		flogBar:SetSize(scale * 802 , 29 );
		flogBar:SetTexUV(0, 825, scale * 802, 29 );
		flogBar:Show();
		-- 如果已经loading了指定的时间
		if GameMgr:getTickTime() - nBegin_time >= CHG_MAP_TOTAL_TIME and nEnd_loading_flag == 1 then
			ShowFramesInWorld();
			LoadingFrame:Hide();
		end
	end

	SetLoadingTexture();
end

function SetLoadingTexture()
	-- 设置 LOADING背景图	大小
	local nRealWidth  = loadingimgtopleft:getRelWidth();
	local nRealHeight = loadingimgtopleft:getRelHeight();

	-- 裁剪图片
	local nScale = nRealWidth / GetScreenWidth();
	local nScreenHeight =  GetScreenHeight() * nScale;
	if nScreenHeight > nRealHeight then
		nScale = nRealHeight / GetScreenHeight();
	end

	local nCutWidth		= math.abs( (nRealWidth - GetScreenWidth() * nScale)/2 );   
	local nCutHeight	= math.abs( (nRealHeight - GetScreenHeight() * nScale)/2 ); 
	loadingimgtopleft:SetTexUV( nCutWidth, nCutHeight, (nRealWidth-2*nCutWidth), (nRealHeight-2*nCutHeight) );	

	LoadingBackGround:SetSize((GetScreenWidth()) / (GetScreenScaleY()), (GetScreenHeight()) / (GetScreenScaleY()));
end

function BeginEnterMap( nMap_ID )
	ChangeCursorState("normal");
	if nLogin_flag ~= 1 then
		SetLoadingBackground( nMap_ID );
	end
end

function BeginLoading( nMap_ID)
	nLogin_flag		= 1;
	nEnd_loading_flag	= 0;
	SetLoadingBackground( nMap_ID );
	CharacterSelectFrame:Hide();
	MessageBoxFrame:Hide();
end

function SetLoadingBackground( nMap_ID )
	local n_Exist		= 0;
	nEnd_loading_flag	= 0;
	local texture		= getglobal( "loadingimgtopleft" );
--[[
	local str_MapName	= "";	--登录、切换场景时loading界面的背景的路径
	str_MapName		= "uires\\ui\\loading\\"..nMap_ID..".jpg";
	-- 如果当前正在进入的场景有对应的loading界面，则设置相应的loading背景
	if IsInExistence(str_MapName) then
		texture:SetTexture( str_MapName );
	else
		-- 如果当前正在进入的场景没有对应的loading界面，则设置为默认loading背景
		getRandomNum( RANDOM_CHOSE_MAP );
		str_MapName="uires\\ui\\loading\\loading" .. t_RandomMapsInfo[nSelectMap] .. ".jpg";
		if not IsInExistence(str_MapName) then
			str_MapName = "uires\\ui\\loading\\loading.jpg";
		end
		texture:SetTexture( str_MapName );
	end
--]]
	getRandomNum( RANDOM_CHOSE_MAP );
	str_MapName="uires\\ui\\loading\\loading" .. t_RandomMapsInfo[nSelectMap] .. ".jpg";
	if not IsInExistence(str_MapName) then
		str_MapName = "uires\\ui\\loading\\loading.jpg";
	end
	texture:SetTexture( str_MapName );
	
	LoadingFrame:Show();
	SetReceiveAttr( false );
end

local t_showFramwControl = 
{
["TaskTraceFrame"] = { check = function ()
									return CanShowTaskTraceFrame();
								end },
};
function ShowFramesInWorld()
	for i = 1, table.getn( t_FrameShouldShownInWorld ) do
		local szFrameName	= t_FrameShouldShownInWorld[i];
		local frame			= getglobal( szFrameName );
		local showCondition	= t_showFramwControl[szFrameName] and t_showFramwControl[szFrameName]["check"];
		if frame and not frame:IsShown() then
			if showCondition == nil then
				frame:Show();
			else
				if showCondition() then
					frame:Show();
				end
			end
		end
	end
	LoginBackFrame:Hide();

	SetFocusFrame( "UIClient" );
	-- 设置是否接收属性表变化（等待属性表全部写完）
	SetReceiveAttr( true );	
	PlayerFirstEnterWorld();
	if LoginMgr:isQuickStart() then
		QuickRegisterBtn:Show();
		QuickRegisterBtnUVAminTex:SetUVAnimation( 70,true );
	else
		QuickRegisterBtn:Hide();
	end
end

function LoadingFrame_OnHide()
	--GameMgr:ShowLoading(false);
	CheckAddFriendReqestsAfteAnim();
	CheckTeamReqestsAfterAnim();	

	--把进度条恢复成初始位置
	ProcessButtonIcon:SetPoint("bottom", "LoadingFrame", "bottom", -302, -65 );
	flogBar:SetSize(0 , 29 );
	flogBar:SetTexUV(0, 825,0, 29 );

	if not GameWizard:isDo( GAMEWIZARD_FIRST_POP_MOUSE_HABIT_XINSHOU_FRAME ) then
		ShowMouseHabitFrame();
	end

	ValidateToShowPetPhoto();
	if this:GetName() == "LoadingFrame" then
		loadingimgtopleft:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
		local t_TmpBugle	= GetTmpMsg( SYS_BUGLE );
		local t_TmpContent	= GetTmpMsg( SYS_MIDDLE );
		local t_TmpContent2 = GetTmpMsg( SYS_MIDDLE2 );
		local t_TmpBulletin	= GetTmpMsg( SYS_BULLETIN );
		local t_TmpTip		= GetTmpMsg( SYS_TIP );
		local t_TmpGetItem	= GetTmpMsg( SYS_MSG_ITEM );
		if t_TmpContent ~= nil then
			SetMiddleTips( t_TmpContent );
			GameMiddleTipsFrame:Show();
		end
		if t_TmpContent2 ~= nil then
			SetMiddleTips2( t_TmpContent2 );
		end
		if t_TmpBulletin ~= nil then
			SetBulletin( t_TmpBulletin );
		end
		if t_TmpTip ~= nil then
			SetSysTip( t_TmpTip );
		end
		if t_TmpBugle~= nil then
			SetBugleTip( t_TmpBugle );
			ChatShowBugleMsgFrame:Show();
		end
		if t_TmpGetItem~= nil then
			SetGetItemTip( t_TmpGetItem );
			GameGetItemTipsFrame:Show();
		end
	end
end

local loaded
local lastAlphaTime = 0
local hideAlphaTime = 1
local fadeAlphaTime = 1.5

function MapLoadingMask_OnLoad()
	MapLoadingMaskTex:SetColor(0, 0, 0)
	MapLoadingMask:setUpdateTime(0)
	this:RegisterEvent("GE_MAP_LOAD")
	this:RegisterEvent("GE_ENTER_MAP")
	this:RegisterEvent("GE_BREAK_NETWORK_TO_DIR")
end

function MapLoadingMask_OnEvent()
	if arg1 == "GE_MAP_LOAD" then
		loaded = false
		if not MapLoadingMask:IsShown() then
			MapLoadingMask:Show()
			MapLoadingMaskTex:SetBlendAlpha(0)
		end
		MapLoadingMask:setInputTransparent(false)
		FrameMgr:EnableAccelerator(false)
	elseif arg1 == "GE_ENTER_MAP" then
		loaded = true
		--[[if not MapLoadingMask:IsShown() then
			MapLoadingMask:Show()
			MapLoadingMaskTex:SetBlendAlpha(1)
		end]]
		MapLoadingMask:setInputTransparent(true)
		FrameMgr:EnableAccelerator(true)
	elseif arg1 == "GE_BREAK_NETWORK_TO_DIR" then
		loaded = true
		MapLoadingMask:Hide()
		MapLoadingMask:setInputTransparent(true)
		FrameMgr:EnableAccelerator(true)
	end
end

function MapLoadingMask_OnShow()
	lastAlphaTime = os.clock()
end

function MapLoadingMask_OnHide()
end

function MapLoadingMask_OnUpdate()
	local now = os.clock()
	local diff = now - lastAlphaTime
	local alpha = MapLoadingMaskTex:GetBlendAlpha()
	lastAlphaTime = now
	if loaded then
		alpha = alpha - diff / fadeAlphaTime
		if alpha <= 0 then
			MapLoadingMask:Hide()
			return
		end
	else
		alpha = alpha + diff / hideAlphaTime
		if alpha >= 1 then
			alpha = 1
			GameMgr:mapLoadFaded()
		end
	end
	MapLoadingMaskTex:SetBlendAlpha(alpha)
end