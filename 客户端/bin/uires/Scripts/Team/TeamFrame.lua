-- 点击队伍主面板的队伍成员标签按钮响应--4个面板共用
MAX_TEAM_MEMBER			= 6; --队伍人数上限
MAX_APPLY_BTN_NUM		= 15;
MAX_AROUND_BTN_NUM		= 15;
MAX_QIYUE_BTN_NUM		= 6;
MAX_TEAM_QIYUE_BUTTON	= 6;

t_teamPosition = { [TEAM_POSITION_MEMBER] = { desc = "队员" }, [TEAM_POSITION_CAPTAIN] = { desc = "队长" }, };

local t_data =	{ 
					["TeamMemberFrame"]			= { selMember = { name = "" } },
					["TeamApplyListFrame"]			= { selMember = { name = "" } },
					["TeamAroundPlayerFrame"]		= { selMember = { name = "" } },
					["TeamQiYueFrame"]			= { selMember = { name = "" } },
				};

function CheckLable( szFrameName, szDisableBtnName )
	local t_lableBtn = {	szFrameName.."MemberButton", szFrameName.."ApplyListButton", szFrameName.."AroundPlayerButton", 
							szFrameName.."SetQiYueButton", };
	for _, name in ipairs( t_lableBtn ) do
		local btn = getglobal( name );
		btn:Enable();
	end
	
	local btn = getglobal( szDisableBtnName );
	btn:Disable();
end

function isAnyTeamMemberInPworld()
	local teamInfo = TeamManager:getTeamInfo();
	for i = 1, teamInfo.MemberNum do
		local teamMember = teamInfo.TeamMember[i-1];
		local nRealMapID = TeamManager:getMapDefID( teamMember.MapID );
		if nRealMapID >= MIN_COPY_ID then
			return true;
		end
	end

	return false;
end

function getCaptain()
	local member;
	local teamInfo = TeamManager:getTeamInfo();
	if teamInfo.MemberNum == 0 then
		return;
	end
	for i = 1, teamInfo.MemberNum do
		local teamMember = teamInfo.TeamMember[i-1];
		if teamMember.Position == TEAM_POSITION_CAPTAIN then
			member = teamMember;
			break;
		end
	end
	return member;
end

function isAnyTeamMemberInFollow()
	local mainplayer = ActorMgr:getMainPlayer();
	if not mainplayer or not mainplayer:isCaptain() then
		return false;
	end
	local teamInfo = TeamManager:getTeamInfo();
	if teamInfo.MemberNum == 0 then
		return false;
	end
	for i = 1, teamInfo.MemberNum do
		local teamMember = teamInfo.TeamMember[i-1];
		if teamMember.Follow ~= 0 then
			return true;
		end
	end
	return false;
end

function IgnoreAllTeamRequests()
	TeamManager:Team_OP_ApplyRefuseAll();

	local t_teamBlink = GetTeamBlinkPlayers();

	for _, data in ipairs( t_teamBlink ) do
		TeamManager:Team_OP_InviteRefuse( data["name"] );
	end

	initTeamBUtton();

	local t_curInvitePlayer = GetCurInvitePlayer();
	if t_curInvitePlayer["name"] ~= "" then
		TeamManager:Team_OP_InviteRefuse( t_curInvitePlayer["name"]  );
		t_curInvitePlayer:clear();
	end
end

function ClearTeamFrameButton()
	for i = 1, MAX_TEAM_MEMBER do
		local t_hideIcons = { "TeamMemberFrame_MemberButton"..i, 
								"TeamMemberFrame_MemberButton"..i.."_SelectHilightTexture", };
		util.HideIcons( t_hideIcons );
	end
end

-- 保存所更新队员的avatar数据
-- t_teamAvatars = { [name] = { hair = , face = , head =  }, };
local t_teamAvatars = {};

local t_followDestInfo = { mapId = 0, posX = 0, posY = 0 };

local isTeamFollow = false;

function setTeamFollowBegin()
	isTeamFollow = true;
end

function CancelWeiXing()	
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer and not mainplayer:isInFly() then
		mainplayer:CmdMoveStop();
	end
	initTeamData();
end

function CancelTeamFollow()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer and not mainplayer:isInFly() then
		mainplayer:CmdMoveStop();
	end
	initTeamData();
	nLastRequestTime = 0;
	isTeamFollow = false;
end

-- 自动跟随某人
function Follow( nMapID, nPreMapID, nRealX, nRealY, nDist )
	-- 若跟踪目标不在同一张地图，则取消跟随
	--[[
	if nMapID ~= MapShower:getRealMapId() then
		GameFollowManager:cancelWeiXing()
		return
	end
	--]]
	local mainplayer		= ActorMgr:getMainPlayer();
	local maxMapContal		= MapShower:GetMaxMapContal();

	--若主角正在挂机则结束挂机
	if AutoAssist:getAutoAssistBegin() then
		AutoAssistFrame_OnClick();
	end

	if not PhotoFrameStopWeiXingButton:IsShown() then
		PhotoFrameStopWeiXingButton:Show();
	end

	-- 先判断目标角色是否还在原来那张地图的原来的那个点上，若是，则无需重新自动寻路	
	if t_followDestInfo["mapId"] == nMapID and t_followDestInfo["posX"] == nRealX and t_followDestInfo["posY"] == nRealY then
		local actor = ActorMgr:findActorByName( GameFollowManager:weiXingTarget() );
		if actor ~= nil then
			return;
		end
	end
	
	-- 更新跟随目标的位置信息
	t_followDestInfo = { mapId = nMapID, posX = nRealX, posY = nRealY };
	
	--[[
	local fDist	=  ( nRealX - mainplayer:getPlayerX() / 10 ) ^ 2 + (  nRealY - mainplayer:getPlayerZ() / 10 ) ^ 2;
	if MapShower:getRealMapId() == nMapID and fDist < 180^2 then
		return;
	end
	--]]
	maxMapContal:follow( nMapID, nRealX, nRealY, nDist );
end

-- 组队跟随队长
function TeamFollow( nMapID, nCapMapID, nRealX, nRealY, nMapLine, nDist )
	local maxMapContal		= MapShower:GetMaxMapContal();
	local mainplayer		= ActorMgr:getMainPlayer();
	--若主角当前处于普通跟随中则取消普通跟随
	if GameFollowManager:isWeiXing() then
		GameFollowManager:cancelWeiXing();	
	end

	--若主角正在挂机则结束挂机
	if AutoAssist:getAutoAssistBegin() then
		AutoAssistFrame_OnClick();
	end		

	if not isTeamFollow then
		return;
	end
	
	--切换地图后判断主角与队长是不是在同一地图的同一条线，不是则停止组队跟随
	local bEnterMap, bEnterPworld = getEnterMap();
	if bEnterMap then
		resetEnterMap();		
		if nMapID == nCapMapID then
			if not bEnterPworld then
				if nMapID ~= MapShower:getRealMapId() and nMapLine ~= 0 then
					bEnterMap = false;
					TeamManager:Team_OP_CancelTeamFollow();
					return;
				end
			else
				if nMapID ~= MapShower:getRealMapId() and not mainplayer:isInFly() then
					mainplayer:CmdMoveStop();
					return;
				end
			end
			bEnterMap = false;
			bEnterPworld = false;
		end		
	end

	--[[
	战斗状态下若与队长距离不超过12米则不进行寻路
	if mainplayer:isInFight() then
		local dist = ( nRealX - mainplayer:getPlayerX() / 10 ) ^ 2 + ( nRealY - mainplayer:getPlayerZ() / 10 ) ^ 2;
		if dist / ( 100 ^ 2 ) <= 12 ^ 2 then
			return;
		end
	end
	--]]
	
	-- 若视野中可见队长且位置不变，则无需继续寻路跟随	
	if t_followDestInfo["mapId"] == nMapID and t_followDestInfo["posX"] == nRealX and t_followDestInfo["posY"] == nRealY then
		local actor = ActorMgr:findActorByName( getCaptain().MemberName );
		if actor ~= nil then
			return;
		end
	end
	
	-- 更新跟随目标的位置信息
	t_followDestInfo = { mapId = nMapID, posX = nRealX, posY = nRealY };
	--t_followDestInfo = { mapId = MapShower:getRealMapId(), posX = mainplayer:getPlayerX() / 10, posY = mainplayer:getPlayerZ() / 10 };
	
	maxMapContal:follow( nMapID, nRealX, nRealY, nDist );
end

local nLastRequestTime = 0;
function CalTeamMemberDistance()
	if nLastRequestTime ~= 0 and os.clock() - nLastRequestTime < 6 then
		return;
	end
	if not isAnyTeamMemberInFollow() then
		return;
	end
	local teamInfo = TeamManager:getTeamInfo();
	local mainplayer = ActorMgr:getMainPlayer();
	for i = 1, teamInfo.MemberNum do
		local teamMember = teamInfo.TeamMember[i-1];
		if teamMember.Follow ~= 0 then
			local dist = ( mainplayer:getPlayerX() / 10 - teamMember.Pos.X ) ^ 2 +
			( mainplayer:getPlayerZ() / 10 - teamMember.Pos.Y ) ^ 2;
			if dist / ( 100 ^ 2 ) > 20 ^ 2 then
				ShowMidTips("组队跟随的队员中,有队员距离过远,请注意速度,以免掉队!");
				nLastRequestTime = os.clock();
				break;
			end			
		end
	end	
	--[[
	local teamInfo = TeamManager:getTeamInfo();
	local mainplayer = ActorMgr:getMainPlayer();
	local szText = "";
	for i = 1, teamInfo.MemberNum do
		local teamMember = teamInfo.TeamMember[i-1];
		if teamMember.Follow ~= 0 then
			local dist = ( mainplayer:getPlayerX() / 10 - teamMember.Pos.X ) ^ 2 +
			( mainplayer:getPlayerZ() / 10 - teamMember.Pos.Y ) ^ 2;
			if dist / ( 100 ^ 2 ) > 20 ^ 2 then
				szText = szText .. teamMember.MemberName .. "，";
			end			
		end
	end
	if szText ~= "" then
		ShowMidTips( "队员：" .. szText .. "距离过远，请注意速度，以免队员掉队" );
	end
	--]]	
end

function CountTeamFollowNum()
	local num = 0;
	local teamInfo = TeamManager:getTeamInfo();
	for i = 1, teamInfo.MemberNum do
		local teamMember = teamInfo.TeamMember[i-1];
		if teamMember.Follow ~= 0 then
			num = num + 1;
		end
	end
	return num;
end

function initTeamData()
	t_teamAvatars		= {};
	t_followDestInfo	= { mapId = 0, posX = 0, posY = 0 };
end

-- 是否需要重置队员的Avatar
function NeedResetAvatar( teamMember, nIndex )
	if not t_teamAvatars[nIndex] then
		--t_teamAvatars[nIndex] = { name = teamMember.MemberName, hair = teamMember.Hair, face = teamMember.Face, head = teamMember.Head };
		return true;
	end

	-- 若存有这个数据
	local avatarBuffer = t_teamAvatars[nIndex];
	return teamMember.Hair ~= avatarBuffer.hair or teamMember.Face ~= avatarBuffer.face or teamMember.head ~= avatarBuffer.Head or 
			teamMember.MemberName ~= avatarBuffer.name;
end

function SetAvartar( data )
	local teamMember	= data["teamMember"];
	local modelView		= data["modelView"];
	local nIndex		= data["index"];
	if not NeedResetAvatar( teamMember, nIndex ) then
		return;
	end
	
	TeamManager:setAvatarAttrib( nIndex - 1 );
	local nHairID = teamMember.Hair + 1;
	for i = 1, teamMember.Avatar.Num do
		local oneEquipInfo = teamMember.Avatar.Arms[i-1];
		if oneEquipInfo.GridIdx == EQUIP_POS_HEAD then
			local headArmid = getArmEquip( EQUIP_POS_HEAD );
			local nHariStyle = getItemDisplayInfoHairStyle( headArmid );
			if nHariStyle == 0 then
				nHairID = 0;
			end
			break;
		end
	end

	local t_modelViews = 
	{ [modelView:GetName()] = 
		{ 

			{ index = 0, grid = POS_ID_HAIR, id = nHairID },  { index = 0, grid = POS_ID_FACE, id = teamMember.Face }, 
			{ index = 0, grid = POS_ID_HEAD, id = teamMember.Head }, 
		}, 
	};
	
	for name, data in pairs( t_modelViews ) do
		local model = getglobal( name );

		for j = 1, CLIENT_MAX_AVATAR_ATTRIB do
			if getAvatarAttrib( j-1 ) ~= 0 then
				model:setAvatarAttrib( 0, j - 1, getAvatarAttrib( j - 1 ),getAvatarStarLv( j - 1 ), j == 1 );
			end
		end

		model:setEntityAvatar( 0 );
		for _, modelData in ipairs( data ) do
			model:setAvatarAttrib( modelData["index"], modelData["grid"], modelData["id"] );
		end

		model:playEntityAnim( 0, 10100, nil );
		model:setEntityRotate( 0, 0 );
		model:setPetScale(0,0.8,0.8,0.8);--设置缩放
		
		model:setEntityPosition(0,0,20,0);
		model:setEntityRotate(0,0);
		model:SetClientUserData( 0,0 );
	end

	t_teamAvatars[nIndex] = { name = teamMember.MemberName, hair = teamMember.Hair, face = teamMember.Face, head = teamMember.Head };
end

function TeamMemberTurnRightBtn_OnMouseDown()
	local model = getglobal( this:GetParent() .. "ModelView" );
	local angel = model:GetClientUserData( 0 );
	angel = angel - 5;
	model:setEntityRotate( 0,angel );
	model:SetClientUserData( 0,angel );
end

function TeamMemberTurnLeftBtn_OnMouseDown()
	local model = getglobal( this:GetParent() .. "ModelView" );
	local angel = model:GetClientUserData( 0 );
	angel = angel + 5;
	model:setEntityRotate( 0,angel );
	model:SetClientUserData( 0,angel );
end

-- 队伍成员标签按钮
function UpdateTeamFrame()
	local teamInfo = TeamManager:getTeamInfo();	
	local mainplayer = ActorMgr:getMainPlayer();
	local btn = getglobal( "TeamMemberFrameSetQiYueButton" );
	if teamInfo.MemberNum > 0 then
		local t_showIcons = { "TeamMemberFrame_CaptainMarkButton" };
		util.ShowIcons( t_showIcons );
		btn:Enable();
	else
		local t_hideIcons = { "TeamMemberFrame_CaptainMarkButton", };
		util.HideIcons( t_hideIcons );
		btn:Disable();
	end
	--[[	注释契约功能
	-- 若有队伍装备了契约，则显示契约图标
	if teamInfo.QiYue ~= 0 then
		-- 设置组队契约图标
		-- TODO: 等待美术那下单
		local t_tgas = { ["TeamMemberFrame_QiYueTex"] = { path = DEFAULT_ITEM_PATH }, };
		util.SetTgas( t_tgas );

		local t_fonts = { ["TeamMemberFrame_QiYueNameFont"] = { value = getQiYueDescName( teamInfo.QiYue ) },};
		util.SetFonts( t_fonts );
		
		local t_showIcons = { "TeamMemberFrame_QiYueHeadBtn", "TeamMemberFrame_QiYuePictureBtn", "TeamMemberFrame_QiYueNameBtn" };
		util.ShowIcons( t_showIcons );
	else
		local t_hideIcons = { "TeamMemberFrame_QiYueHeadBtn", "TeamMemberFrame_QiYuePictureBtn", "TeamMemberFrame_QiYueNameBtn" };
		util.HideIcons( t_hideIcons );
	end
	]]--
	
	ClearTeamFrameButton();
	local selMember = t_data["TeamMemberFrame"]["selMember"];
	local bFoundSel = false;
	local selfTeamInfo = nil;
	for i = 1, teamInfo.MemberNum do
		local teamMember = teamInfo.TeamMember[i-1];
		local t_fonts = {	["TeamMemberFrame_MemberButton"..i.."name"]		= { value = GetPlayerName(teamMember.MemberName) },
							["TeamMemberFrame_MemberButton"..i.."LevelCareer"]	= { value = teamMember.Level .. "级 " .. t_raceList[teamMember.Career] },
							["TeamMemberFrame_MemberButton"..i.."Scene"]	= 
							{ value = MapShower:GetMinMapContal():getMapName( TeamManager:getMapDefID( teamMember.MapID ) ) },
							["TeamMemberFrame_MemberButton"..i.."ServerName"] = {value = LoginMgr:getWorldNameById( ActorMgr:getServerId( teamMember.MemberName ) ) },
						};
		
		util.SetFonts( t_fonts );

		-- 若队员下线
		local memberBtn	= getglobal( "TeamMemberFrame_MemberButton"..i );
		memberBtn:Enable();
		memberBtn:Show();
		if teamMember.OffLineTime ~= CLIENT_MEMBER_ONLINE then
			local t_showIcons = { "TeamMemberFrame_MemberButton"..i.."_OfflineTex" };
			util.ShowIcons( t_showIcons );

			local t_hideIcons = { "TeamMemberFrame_MemberButton"..i.."ModelView" };
			util.HideIcons( t_hideIcons );
		else	
			-- 若队员在线
			local t_hideIcons = { "TeamMemberFrame_MemberButton"..i.."_OfflineTex" };
			util.HideIcons( t_hideIcons );
			
			local t_showIcons = { "TeamMemberFrame_MemberButton"..i.."ModelView" };
			util.ShowIcons( t_showIcons );

			SetAvartar( { teamMember = teamMember, modelView = getglobal( "TeamMemberFrame_MemberButton"..i.."ModelView") , index = i } );
		end
		-- 查看被选定的玩家是否仍在队伍中并且在线
		
		if teamMember.MemberName == selMember["name"] then
			local t_showIcons = { "TeamMemberFrame_MemberButton"..i.."_SelectHilightTexture", };
			util.ShowIcons( t_showIcons );
			bFoundSel = true;
		end

		if teamMember.MemberName == mainplayer:getName() then
			selfTeamInfo = teamMember;
		end
		
		local captainBtn	= getglobal( "TeamMemberFrame_CaptainMarkButton" );
		-- 设置队伍队长标志
		if teamMember.Position == TEAM_POSITION_CAPTAIN then
			captainBtn:SetPoint( "topLeft", memberBtn:GetName(), "topLeft", 4, 37 );
		end		

		memberBtn:Show();
	end
	
	-- 若原来被选定的人不在队伍，则情况被选定的人的信息
	if not bFoundSel then
		t_data["TeamMemberFrame"]["selMember"] = { name = "", };
	end
	
	--local t_captionBtns = {"TeamMemberFrameZhuangRangCaptainButton","TeamMemberFrameJieSanTeamButton","TeamMemberFrameWeiXingButton"}
	local t_captionBtns = {"TeamMemberFrameZhuangRangCaptainButton","TeamMemberFrameJieSanTeamButton"};
	local t_followBtns = { "TeamMemberFrameAutoFollowAccept", "TeamMemberFrameRequestTeamFollowButton", "TeamMemberFrameCancelTeamFollowButton"};
	util.HideIcons( t_followBtns );
	if mainplayer:isCaptain() then
		util.ShowIcons( t_captionBtns );
		TeamMemberFrameAutoFollowAccept:Hide();				
		TeamMemberFrameRequestTeamFollowButton:Show();	
		TeamMemberFrameRequestTeamFollowButton:SetPoint( "topleft", "TeamMemberFrame", "topleft", 35, 476 );		
		TeamMemberFrameCancelTeamFollowButton:Show();
		TeamMemberFrameCancelTeamFollowButton:SetPoint( "topleft", "TeamMemberFrame", "topleft", 173, 476 );
		if not isAnyTeamMemberInFollow() then
			TeamMemberFrameRequestTeamFollowButton:Enable();
			TeamMemberFrameCancelTeamFollowButton:Disable();
		else			
			if CountTeamFollowNum() == teamInfo.MemberNum - 1 then
				TeamMemberFrameRequestTeamFollowButton:Disable();
			else
				TeamMemberFrameRequestTeamFollowButton:Enable();
			end
			TeamMemberFrameCancelTeamFollowButton:Enable();
		end
	else
		util.HideIcons( t_captionBtns );
		TeamMemberFrameAutoFollowAccept:Show();
		if teamInfo.MemberNum > 0 then
			if TeamManager:isInTeamFollow() then
				TeamMemberFrameCancelTeamFollowButton:Show();
				TeamMemberFrameCancelTeamFollowButton:Enable();
				TeamMemberFrameCancelTeamFollowButton:SetPoint( "topleft", "TeamMemberFrame", "topleft", 549, 476 );
			else
				TeamMemberFrameRequestTeamFollowButton:Show();
				TeamMemberFrameRequestTeamFollowButton:Enable();
				TeamMemberFrameRequestTeamFollowButton:SetPoint( "topleft", "TeamMemberFrame", "topleft", 549, 476 );
			end
		end
	end

	if TeamMemberFrameAutoFollowAccept:IsShown() then
		local systemSetting = GameMgr:getUserSystemConfig();
		local isCheck = systemSetting:getGameOptValue( GAME_OPT_AUTO_ACCEPT_TEAM_FOLLOW );
		if isCheck == 0 then
			TeamMemberFrameAutoFollowAccept:setCheckState(false);
		else
			TeamMemberFrameAutoFollowAccept:setCheckState(true);
		end
	end
end

-- 申请列表标签按钮
function ClearApplyList()
	DisCheckAllBtn( "TeamApplyListFrameApplyPlayerInfoButton", MAX_APPLY_BTN_NUM );
	for i = 1, MAX_APPLY_BTN_NUM do
		local btn = getglobal( "TeamApplyListFrameApplyPlayerInfoButton"..i );
		btn:Disable();
		local font = getglobal( btn:GetName().."NameFont" );
		font:SetText( "" );

		font = getglobal( btn:GetName().."LevelFont" );
		font:SetText( "" );

		font = getglobal( btn:GetName().."CareerFont" );
		font:SetText( "" );

		font = getglobal( btn:GetName().."ClanFont" );
		font:SetText( "" );

		font = getglobal( btn:GetName().."WorldFont" );
		font:SetText( "" );
	end
end

bAutoAccept = false

function AutoAcceptApply()
	if bAutoAccept == true then
		for i = 1, TeamManager:getTeamApplyInfoSize() do
			if TeamManager:getTeamApplyInfoSize() > 0 then
				local applyPlayerInfo = TeamManager:getTeamApplyInfoByIndex( 0 );
				TeamManager:Team_OP_ApplyAccept( applyPlayerInfo.RoleID.nHeight, applyPlayerInfo.RoleID.nLow );
			end
		end
	end
	
	t_data["TeamApplyListFrame"]["selMember"] = { name = "" };
	if TeamManager:getTeamApplyInfoSize() > 0 then
		local applyPlayerInfo = TeamManager:getTeamApplyInfoByIndex( 0 );
		t_data["TeamApplyListFrame"]["selMember"] = { name = applyPlayerInfo.RoleName };
	end

	UpdateApplyList()
end

local SORT_BY_TEAM, SORT_BY_NAME, SORT_BY_LEVEL, SORT_BY_CAREER, SORT_BY_CLAN_NAME, SORT_BY_WORLD = 0, 1, 2, 3, 4, 5
local nApplySortWay = SORT_BY_NAME

function UpdateApplyList()
	local slider		= getglobal( "TeamApplyListFrameScrollBar" );
	local nStartLine	= slider:GetValue();
	local teamInfo = TeamManager:getTeamInfo();
	-- 若处于组队状态，则契约设置按钮可以使用
	local btn = getglobal( "TeamApplyListFrameSetQiYueButton" );
	if teamInfo.MemberNum > 0 then
		btn:Enable();
	else
		btn:Disable();
	end

	SetScrollBar( "TeamApplyListFrameScrollBar", MAX_APPLY_BTN_NUM, TeamManager:getTeamApplyInfoSize() );
	ClearApplyList();
	local t_selMember = t_data["TeamApplyListFrame"]["selMember"];
	local nBtnIndex = 1;
	local t_applyPlayerList = {};

	for i = nStartLine + 1, TeamManager:getTeamApplyInfoSize() do
		if nBtnIndex > MAX_APPLY_BTN_NUM then
			break;
		end

		local applyPlayerInfo = TeamManager:getTeamApplyInfoByIndex( i - 1 );
	
		if applyPlayerInfo ~= nil then
			local serverId  = ActorMgr:getServerId(applyPlayerInfo.RoleName)
			table.insert( t_applyPlayerList, 
					{	["name"]	= applyPlayerInfo.RoleName,
						["level"]	= applyPlayerInfo.Level.."级",
						["levelNum"]	= applyPlayerInfo.Level,
						["career"]	= GetCareerName( applyPlayerInfo.Career ),
						["race"]	= applyPlayerInfo.Career,
						["clan"]	= applyPlayerInfo.ClanName,
						["world"]	= LoginMgr:getWorldNameById(serverId),
						["serverId"]	= serverId,
					} 
				    );
			nBtnIndex = nBtnIndex + 1;
		end
	end

	table.sort( t_applyPlayerList, 
		    function ( lhs, rhs )

			if nApplySortWay == SORT_BY_NAME then

				if lhs["name"] ~= rhs["name"] then
					return lhs["name"] < rhs["name"];
				end

			elseif nApplySortWay == SORT_BY_LEVEL then
			
				if lhs["levelNum"] ~= rhs["levelNum"] then
					return lhs["levelNum"] > rhs["levelNum"];
				end

			elseif nApplySortWay == SORT_BY_CAREER then

				if lhs["race"] ~= rhs["race"] then
					return lhs["race"] < rhs["race"];
				end
			
			elseif nApplySortWay == SORT_BY_CLAN_NAME then

				if lhs["clan"] ~= rhs["clan"] then
					return lhs["clan"] > rhs["clan"];
				end
			
			elseif nApplySortWay == SORT_BY_WORLD then

				if lhs["serverId"] ~= rhs["serverId"] then
					return lhs["serverId"] < rhs["serverId"];
				end

			end

		    end );

	for index, oneApplyPlayerInfo in ipairs( t_applyPlayerList ) do
		local btn = getglobal( "TeamApplyListFrameApplyPlayerInfoButton" .. index );
		local t_fonts = {	[btn:GetName().."NameFont"]   = { value = oneApplyPlayerInfo["name"]   }, 
					[btn:GetName().."LevelFont"]  = { value = oneApplyPlayerInfo["level"]  },
					[btn:GetName().."CareerFont"] = { value = oneApplyPlayerInfo["career"] }, 
					[btn:GetName().."ClanFont"]   = { value = oneApplyPlayerInfo["clan"]   },
					[btn:GetName().."WorldFont"]  = { value = oneApplyPlayerInfo["world"]  },
				};
		util.SetFonts( t_fonts );
		btn:Enable();
		if t_selMember["name"] == oneApplyPlayerInfo["name"] then
			btn:Checked();
		end
	end
	
	local friendInfo  = GameFriendManager:getFriendInfoByName(t_selMember["name"])
	btn		  = getglobal("TeamApplyListFrameAddFriendButton")

	-- 如果是好友
	if friendInfo ~= nil then
		if friendInfo.m_nRelationType == CLIENT_RELATION_TYPE_FRIEND then
			btn:Disable()
		else
			btn:Enable()
		end
	else
		btn:Enable()
	end

end

function TeamApplyNameButton_OnClick()
	local nameButton  = getglobal("TeamApplyListFrameNameButton")
	local levelButton = getglobal("TeamApplyListFrameLevelButton")
	local CareerButton = getglobal("TeamApplyListFrameCareerButton")
	local clanButton  = getglobal("TeamApplyListFrameClanButton")
	local worldButton = getglobal("TeamApplyListFrameWorldButton")

	nameButton:Checked()
	levelButton:DisChecked()
	CareerButton:DisChecked()
	clanButton:DisChecked()
	worldButton:DisChecked()

	nApplySortWay = SORT_BY_NAME
	UpdateApplyList()
end

function TeamApplyLevelButton_OnClick()
	local nameButton   = getglobal("TeamApplyListFrameNameButton")
	local levelButton  = getglobal("TeamApplyListFrameLevelButton")
	local CareerButton = getglobal("TeamApplyListFrameCareerButton")
	local clanButton   = getglobal("TeamApplyListFrameClanButton")
	local worldButton  = getglobal("TeamApplyListFrameWorldButton")

	nameButton:DisChecked()
	levelButton:Checked()
	CareerButton:DisChecked()
	clanButton:DisChecked()
	worldButton:DisChecked()

	nApplySortWay = SORT_BY_LEVEL
	UpdateApplyList()
end

function TeamApplyCareerButton_OnClick()
	local nameButton   = getglobal("TeamApplyListFrameNameButton")
	local levelButton  = getglobal("TeamApplyListFrameLevelButton")
	local CareerButton = getglobal("TeamApplyListFrameCareerButton")
	local clanButton   = getglobal("TeamApplyListFrameClanButton")
	local worldButton  = getglobal("TeamApplyListFrameWorldButton")

	nameButton:DisChecked()
	levelButton:DisChecked()
	CareerButton:Checked()
	clanButton:DisChecked()
	worldButton:DisChecked()

	nApplySortWay = SORT_BY_CAREER
	UpdateApplyList()
end


function TeamApplyClanNameButton_OnClick()
	local nameButton   = getglobal("TeamApplyListFrameNameButton")
	local levelButton  = getglobal("TeamApplyListFrameLevelButton")
	local CareerButton = getglobal("TeamApplyListFrameCareerButton")
	local clanButton   = getglobal("TeamApplyListFrameClanButton")
	local worldButton  = getglobal("TeamApplyListFrameWorldButton")

	nameButton:DisChecked()
	levelButton:DisChecked()
	CareerButton:DisChecked()
	clanButton:Checked()
	worldButton:DisChecked()

	nApplySortWay = SORT_BY_CLAN_NAME
	UpdateApplyList()
end

function TeamApplyWorldButton_OnClick()
	local nameButton   = getglobal("TeamApplyListFrameNameButton")
	local levelButton  = getglobal("TeamApplyListFrameLevelButton")
	local CareerButton = getglobal("TeamApplyListFrameCareerButton")
	local clanButton   = getglobal("TeamApplyListFrameClanButton")
	local worldButton  = getglobal("TeamApplyListFrameWorldButton")

	nameButton:DisChecked()
	levelButton:DisChecked()
	CareerButton:DisChecked()
	clanButton:DisChecked()
	worldButton:Checked()

	nApplySortWay = SORT_BY_WORLD
	UpdateApplyList()
end

-- 周围玩家标签按钮
function ClearAroundList()
	DisCheckAllBtn( "TeamAroundPlayerFrameNearPlayerInfoButton", MAX_APPLY_BTN_NUM );
	for i = 1, MAX_AROUND_BTN_NUM do
		local btn = getglobal( "TeamAroundPlayerFrameNearPlayerInfoButton"..i );
		btn:Disable();
		local font = getglobal( btn:GetName().."NameFont" );
		font:SetText( "" );

		font = getglobal( btn:GetName().."LevelFont" );
		font:SetText( "" );

		font = getglobal( btn:GetName().."CareerFont" );
		font:SetText( "" );

		font = getglobal( btn:GetName().."ClanFont" );
		font:SetText( "" );

		font = getglobal( btn:GetName().."WorldFont" );
		font:SetText( "" );

		local captainTex = getglobal(btn:GetName() .. "CaptainTex");
		local memberTex  = getglobal(btn:GetName() .. "MemberTex");
		captainTex:Hide()
		memberTex:Hide()
	end
end
 
 local t_teamStateDesc = 
 {
 [CLIENT_NO_TEAM]	= { ["desc"] = "空闲" },
 [CLIENT_TEAM_CAPTAIN]	= { ["desc"] = "队长" },
 [CLIENT_TEAM_MEMBER]	= { ["desc"] = "队员" },

 };

local nAroundSortWay = SORT_BY_TEAM

function isTeammate(szName)
	local teamInfo = TeamManager:getTeamInfo();
	
	if teamInfo.MemberNum > 0 then
		for i = 0, teamInfo.MemberNum do
			if teamInfo.TeamMember[i].MemberName == szName then
				return true
			end
		end
	end

	return false
end

function UpdateAroundList( )
	local slider		= getglobal( "TeamAroundPlayerFrameScrollBar" );
	local nStartLine	= slider:GetValue();
	local teamInfo = TeamManager:getTeamInfo();
	-- 若处于组队状态，则契约设置按钮可以使用
	local btn = getglobal( "TeamAroundPlayerFrameSetQiYueButton" );
	if teamInfo.MemberNum > 0 then
		btn:Enable();
	else
		btn:Disable();
	end
	
	SetScrollBar( "TeamAroundPlayerFrameScrollBar", MAX_AROUND_BTN_NUM, TeamManager:getNearPlayerCount() );
	local t_selMember = t_data["TeamAroundPlayerFrame"]["selMember"];
	ClearAroundList();
	local t_aroundPlayerList = {};

	local t_teammateList = {};
	local t_captains = {};
	local t_noCaptainPlayer = {};
	local t_noTeamPlayer = {};

	local nBtnIndex = 1;
	for i = nStartLine + 1, TeamManager:getNearPlayerCount() do
		if nBtnIndex > MAX_AROUND_BTN_NUM then
			break;
		end

		local nearActor = TeamManager:getNearPlayer( i - 1 );
		
		if nearActor ~= nil then
			local serverId  = nearActor:getServerId()
			-- 队伍排序
			if nAroundSortWay == SORT_BY_TEAM then		
				-- 该玩家是队长
				if nearActor:isCaptain() then
					-- 是我的队友
					if isTeammate(nearActor:getName()) then			
						-- 放入队友表中
						table.insert( t_teammateList, 
							{	["isInTeam"]	= nearActor:isInTeam(), ["name"] = GetPlayerName(nearActor:getName()), ["level"] = nearActor:getLv(), 
								["isCaptain"]	= nearActor:isCaptain(),
								["race"]	= nearActor:getRace(), 
								["clanName"]	= nearActor:getClanName(),
								["worldName"]   = LoginMgr:getWorldNameById(serverId),
								["serverId"]	= serverId,
							} 
						    );		
					else
						-- 放入队长表中
						table.insert( t_captains, 
							{	["isInTeam"]	= nearActor:isInTeam(), ["name"] = GetPlayerName(nearActor:getName()), ["level"] = nearActor:getLv(), 
								["isCaptain"]	= nearActor:isCaptain(),
								["race"]	= nearActor:getRace(), 
								["clanName"]	= nearActor:getClanName(),
								["worldName"]   = LoginMgr:getWorldNameById(serverId),
								["serverId"]	= serverId,
								["teamID"]	= nearActor:getTeamWID(),
								["teamates"]	= {},
							} 
						    );

					end
				end
			else
				if isTeammate(nearActor:getName()) then
					table.insert( t_teammateList, 
							{	["isInTeam"]	= nearActor:isInTeam(), ["name"] = GetPlayerName(nearActor:getName()), ["level"] = nearActor:getLv(), 
								["isCaptain"]	= nearActor:isCaptain(),
								["race"]	= nearActor:getRace(), 
								["clanName"]	= nearActor:getClanName(),
								["worldName"]   = LoginMgr:getWorldNameById(serverId),
								["serverId"]	= serverId,
							} 
						    );
				else
					table.insert( t_aroundPlayerList, 
							{	["isInTeam"]	= nearActor:isInTeam(), ["name"] = GetPlayerName(nearActor:getName()), ["level"] = nearActor:getLv(), 
								["isCaptain"]	= nearActor:isCaptain(),
								["race"]	= nearActor:getRace(), 
								["clanName"]	= nearActor:getClanName(),
								["worldName"]   = LoginMgr:getWorldNameById(serverId),
								["serverId"]	= serverId,
							} 
						    );
				end
			end

			nBtnIndex = nBtnIndex + 1;
		end
	end

	-- 队伍排序
	if nAroundSortWay == SORT_BY_TEAM then
		nBtnIndex = 1;
		for i = nStartLine + 1, TeamManager:getNearPlayerCount() do
			if nBtnIndex > MAX_AROUND_BTN_NUM then
				break;
			end

			local nearActor = TeamManager:getNearPlayer( i - 1 );

			if nearActor ~= nil then	
				local serverId  = nearActor:getServerId()

				-- 该玩家在队伍中
				if nearActor:isInTeam() then
					-- 该玩家不是队长
					if nearActor:isCaptain() == false then
						-- 是我的队友
						if isTeammate(nearActor:getName()) then
							-- 放入队友表中
							table.insert( t_teammateList, 
								{	["isInTeam"]	= nearActor:isInTeam(), ["name"] = GetPlayerName(nearActor:getName()), ["level"] = nearActor:getLv(), 
									["isCaptain"]	= nearActor:isCaptain(),
									["race"]	= nearActor:getRace(), 
									["clanName"]	= nearActor:getClanName(),
									["worldName"]   = LoginMgr:getWorldNameById(serverId),
									["serverId"]	= serverId,
								} 
							    );	
						-- 不是我的队友
						else
							local bHaveCaptain = false
							for j, captain in ipairs( t_captains ) do			
								-- 是该队长的队友
								if nearActor:isInSameTeam(captain["teamID"]) then
									table.insert( captain["teamates"], 
									{	["isInTeam"]	= nearActor:isInTeam(), ["name"] = GetPlayerName(nearActor:getName()), ["level"] = nearActor:getLv(), 
										["isCaptain"]	= nearActor:isCaptain(),
										["race"]	= nearActor:getRace(), 
										["clanName"]	= nearActor:getClanName(),
										["worldName"]   = LoginMgr:getWorldNameById(serverId),
										["serverId"]	= serverId,
									} 
								    );
								    bHaveCaptain = true
								    break;
								end
							end

							if bHaveCaptain == false then
								-- 不属于任何一个队伍的队员
								table.insert( t_noCaptainPlayer, 
										{	["isInTeam"]	= nearActor:isInTeam(), ["name"] = GetPlayerName(nearActor:getName()), ["level"] = nearActor:getLv(), 
											["isCaptain"]	= nearActor:isCaptain(),
											["race"]	= nearActor:getRace(), 
											["clanName"]	= nearActor:getClanName(),
											["worldName"]   = LoginMgr:getWorldNameById(serverId),
											["serverId"]	= serverId,
										} 
									    );
							end
						end
					end
				else
					-- 不在队伍中放入无队伍表中
					table.insert( t_noTeamPlayer, 
							{	["isInTeam"]	= nearActor:isInTeam(), ["name"] = GetPlayerName(nearActor:getName()), ["level"] = nearActor:getLv(), 
								["isCaptain"]	= nearActor:isCaptain(),
								["race"]	= nearActor:getRace(), 
								["clanName"]	= nearActor:getClanName(),
								["worldName"]   = LoginMgr:getWorldNameById(serverId),
								["serverId"]	= serverId,
							} 
						    );	
				end
			end
			nBtnIndex = nBtnIndex + 1;
		end
	end
	
	if nAroundSortWay ~= SORT_BY_TEAM then
		table.sort( t_teammateList, 
			    function ( lhs, rhs )

				if nAroundSortWay == SORT_BY_NAME then

					if lhs["name"] ~= rhs["name"] then
						return lhs["name"] < rhs["name"];
					end

				elseif nAroundSortWay == SORT_BY_LEVEL then
			
					if lhs["level"] ~= rhs["level"] then
						return lhs["level"] > rhs["level"];
					end

				elseif nAroundSortWay == SORT_BY_CAREER then

					if lhs["race"] ~= rhs["race"] then
						return lhs["race"] < rhs["race"];
					end
			
				elseif nAroundSortWay == SORT_BY_CLAN_NAME then

					if lhs["clanName"] ~= rhs["clanName"] then
						return lhs["clanName"] > rhs["clanName"];
					end

				elseif nAroundSortWay == SORT_BY_WORLD then

					if lhs["serverId"] ~= rhs["serverId"] then
						return lhs["serverId"] < rhs["serverId"];
					end

				end

			    end );

		table.sort( t_aroundPlayerList, 
			function ( lhs, rhs )

				if nAroundSortWay == SORT_BY_NAME then

					if lhs["name"] ~= rhs["name"] then
						return lhs["name"] < rhs["name"];
					end

				elseif nAroundSortWay == SORT_BY_LEVEL then
			
					if lhs["level"] ~= rhs["level"] then
						return lhs["level"] > rhs["level"];
					end

				elseif nAroundSortWay == SORT_BY_CAREER then

					if lhs["race"] ~= rhs["race"] then
						return lhs["race"] < rhs["race"];
					end
			
				elseif nAroundSortWay == SORT_BY_CLAN_NAME then

					if lhs["clanName"] ~= rhs["clanName"] then
						return lhs["clanName"] > rhs["clanName"];
					end

				elseif nAroundSortWay == SORT_BY_WORLD then

					if lhs["serverId"] ~= rhs["serverId"] then
						return lhs["serverId"] < rhs["serverId"];
					end

				end

			end );
	end
	
	local nTeammateNum = 0;
	for index, oneAroundPlayerInfo in ipairs( t_teammateList ) do
		local btn = getglobal( "TeamAroundPlayerFrameNearPlayerInfoButton"..index );
		local captainTex = getglobal(btn:GetName() .. "CaptainTex");
		local memberTex  = getglobal(btn:GetName() .. "MemberTex");

		local font = getglobal(btn:GetName() .. "NameFont")
		font:SetTextColor(0, 255, 0)
		local font = getglobal(btn:GetName() .. "LevelFont")
		font:SetTextColor(0, 255, 0)
		local font = getglobal(btn:GetName() .. "CareerFont")
		font:SetTextColor(0, 255, 0)
		local font = getglobal(btn:GetName() .. "ClanFont")
		font:SetTextColor(0, 255, 0)
		local font = getglobal(btn:GetName() .. "WorldFont")
		font:SetTextColor(0, 255, 0)

		if oneAroundPlayerInfo["isInTeam"] == true then
			if nAroundSortWay == SORT_BY_TEAM then
				if oneAroundPlayerInfo["isCaptain"] == true then
					captainTex:Show()
					memberTex:Hide()
				else
					memberTex:Show()
					captainTex:Hide()
				end
			else
				memberTex:Show()
				captainTex:Hide()
			end
		else
			captainTex:Hide()
			memberTex:Hide()
		end

		local t_fonts = {	[btn:GetName().."NameFont"]	= { value = oneAroundPlayerInfo["name"] }, 
					[btn:GetName().."LevelFont"]	= { value = oneAroundPlayerInfo["level"].."级" },
					[btn:GetName().."CareerFont"]	= { value = GetCareerName( oneAroundPlayerInfo["race"] ) }, 
					[btn:GetName().."ClanFont"]	= { value = oneAroundPlayerInfo["clanName"]  }, 
					[btn:GetName().."WorldFont"]	= { value = oneAroundPlayerInfo["worldName"] }, 
				};
		util.SetFonts( t_fonts );
		btn:Enable();

		if t_selMember["name"] == oneAroundPlayerInfo["name"] then
			btn:Checked();
		end

		nTeammateNum = nTeammateNum + 1
	end

	if nAroundSortWay == SORT_BY_TEAM then
		local nButtonID = nTeammateNum + 1
		-- 显示其他队伍
		for index, captionInfo in ipairs( t_captains ) do
			-- 显示队长
			local btn = getglobal( "TeamAroundPlayerFrameNearPlayerInfoButton"..nButtonID );
			local captainTex = getglobal(btn:GetName() .. "CaptainTex");
			local memberTex  = getglobal(btn:GetName() .. "MemberTex");

			local font = getglobal(btn:GetName() .. "NameFont")
			font:SetTextColor(190, 160, 90)
			local font = getglobal(btn:GetName() .. "LevelFont")
			font:SetTextColor(190, 160, 90)
			local font = getglobal(btn:GetName() .. "CareerFont")
			font:SetTextColor(190, 160, 90)
			local font = getglobal(btn:GetName() .. "ClanFont")
			font:SetTextColor(190, 160, 90)
			local font = getglobal(btn:GetName() .. "WorldFont")
			font:SetTextColor(190, 160, 90)
			
			memberTex:Hide()
			captainTex:Show()

			local t_fonts = {	[btn:GetName().."NameFont"]	= { value = captionInfo["name"] }, 
						[btn:GetName().."LevelFont"]	= { value = captionInfo["level"].."级" },
						[btn:GetName().."CareerFont"]	= { value = GetCareerName( captionInfo["race"] ) }, 
						[btn:GetName().."ClanFont"]	= { value = captionInfo["clanName"]  }, 
						[btn:GetName().."WorldFont"]	= { value = captionInfo["worldName"] }, 
					};
			util.SetFonts( t_fonts );
			btn:Enable();

			if t_selMember["name"] == captionInfo["name"] then
				btn:Checked();
			end

			nButtonID = nButtonID + 1;

			-- 显示队员
			for index, teammateInfo in ipairs( captionInfo["teamates"] ) do
				local btn = getglobal( "TeamAroundPlayerFrameNearPlayerInfoButton"..nButtonID );
				local captainTex = getglobal(btn:GetName() .. "CaptainTex");
				local memberTex  = getglobal(btn:GetName() .. "MemberTex");

				local font = getglobal(btn:GetName() .. "NameFont")
				font:SetTextColor(190, 160, 90)
				local font = getglobal(btn:GetName() .. "LevelFont")
				font:SetTextColor(190, 160, 90)
				local font = getglobal(btn:GetName() .. "CareerFont")
				font:SetTextColor(190, 160, 90)
				local font = getglobal(btn:GetName() .. "ClanFont")
				font:SetTextColor(190, 160, 90)
				local font = getglobal(btn:GetName() .. "WorldFont")
				font:SetTextColor(190, 160, 90)

				captainTex:Hide()
				memberTex:Show()

				local t_fonts = {	[btn:GetName().."NameFont"]	= { value = teammateInfo["name"] }, 
							[btn:GetName().."LevelFont"]	= { value = teammateInfo["level"].."级" },
							[btn:GetName().."CareerFont"]	= { value = GetCareerName( teammateInfo["race"] ) }, 
							[btn:GetName().."ClanFont"]	= { value = teammateInfo["clanName"]  }, 
							[btn:GetName().."WorldFont"]	= { value = teammateInfo["worldName"] }, 
						};
				util.SetFonts( t_fonts );
				btn:Enable();

				if t_selMember["name"] == teammateInfo["name"] then
					btn:Checked();
				end

				nButtonID = nButtonID + 1;
			end
		end

		-- 显示没有队长的队员
		for index, noCaptainPlayerInfo in ipairs( t_noCaptainPlayer ) do
			local btn = getglobal( "TeamAroundPlayerFrameNearPlayerInfoButton"..nButtonID );
			local captainTex = getglobal(btn:GetName() .. "CaptainTex");
			local memberTex  = getglobal(btn:GetName() .. "MemberTex");

			local font = getglobal(btn:GetName() .. "NameFont")
			font:SetTextColor(190, 160, 90)
			local font = getglobal(btn:GetName() .. "LevelFont")
			font:SetTextColor(190, 160, 90)
			local font = getglobal(btn:GetName() .. "CareerFont")
			font:SetTextColor(190, 160, 90)
			local font = getglobal(btn:GetName() .. "ClanFont")
			font:SetTextColor(190, 160, 90)
			local font = getglobal(btn:GetName() .. "WorldFont")
			font:SetTextColor(190, 160, 90)

			captainTex:Hide()
			memberTex:Show()

			local t_fonts = {	[btn:GetName().."NameFont"]	= { value = noCaptainPlayerInfo["name"] }, 
						[btn:GetName().."LevelFont"]	= { value = noCaptainPlayerInfo["level"].."级" },
						[btn:GetName().."CareerFont"]	= { value = GetCareerName( noCaptainPlayerInfo["race"] ) }, 
						[btn:GetName().."ClanFont"]	= { value = noCaptainPlayerInfo["clanName"]  }, 
						[btn:GetName().."WorldFont"]	= { value = noCaptainPlayerInfo["worldName"] }, 
					};
			util.SetFonts( t_fonts );
			btn:Enable();

			if t_selMember["name"] == noCaptainPlayerInfo["name"] then
				btn:Checked();
			end

			nButtonID = nButtonID + 1;
		end

		-- 显示没有队伍的成员
		for index, noTeamPlayerInfo in ipairs( t_noTeamPlayer ) do
			local btn = getglobal( "TeamAroundPlayerFrameNearPlayerInfoButton"..nButtonID );
			local captainTex = getglobal(btn:GetName() .. "CaptainTex");
			local memberTex  = getglobal(btn:GetName() .. "MemberTex");

			local font = getglobal(btn:GetName() .. "NameFont")
			font:SetTextColor(190, 160, 90)
			local font = getglobal(btn:GetName() .. "LevelFont")
			font:SetTextColor(190, 160, 90)
			local font = getglobal(btn:GetName() .. "CareerFont")
			font:SetTextColor(190, 160, 90)
			local font = getglobal(btn:GetName() .. "ClanFont")
			font:SetTextColor(190, 160, 90)
			local font = getglobal(btn:GetName() .. "WorldFont")
			font:SetTextColor(190, 160, 90)

			captainTex:Hide()
			memberTex:Hide()

			local t_fonts = {	[btn:GetName().."NameFont"]	= { value = noTeamPlayerInfo["name"] }, 
						[btn:GetName().."LevelFont"]	= { value = noTeamPlayerInfo["level"].."级" },
						[btn:GetName().."CareerFont"]	= { value = GetCareerName( noTeamPlayerInfo["race"] ) }, 
						[btn:GetName().."ClanFont"]	= { value = noTeamPlayerInfo["clanName"]  }, 
						[btn:GetName().."WorldFont"]	= { value = noTeamPlayerInfo["worldName"] }, 
					};
			util.SetFonts( t_fonts );
			btn:Enable();

			if t_selMember["name"] == noTeamPlayerInfo["name"] then
				btn:Checked();
			end

			nButtonID = nButtonID + 1;
		end
	else
		for index, oneAroundPlayerInfo in ipairs( t_aroundPlayerList ) do
			local btn = getglobal( "TeamAroundPlayerFrameNearPlayerInfoButton"..index + nTeammateNum );
			local captainTex = getglobal(btn:GetName() .. "CaptainTex");
			local memberTex  = getglobal(btn:GetName() .. "MemberTex");

			local font = getglobal(btn:GetName() .. "NameFont")
			font:SetTextColor(190, 160, 90)
			local font = getglobal(btn:GetName() .. "LevelFont")
			font:SetTextColor(190, 160, 90)
			local font = getglobal(btn:GetName() .. "CareerFont")
			font:SetTextColor(190, 160, 90)
			local font = getglobal(btn:GetName() .. "ClanFont")
			font:SetTextColor(190, 160, 90)
			local font = getglobal(btn:GetName() .. "WorldFont")
			font:SetTextColor(190, 160, 90)

			if oneAroundPlayerInfo["isInTeam"] == true then
				if nAroundSortWay == SORT_BY_TEAM then
					if oneAroundPlayerInfo["isCaptain"] == true then
						captainTex:Show()
						memberTex:Hide()
					else
						memberTex:Show()
						captainTex:Hide()
					end
				else
					memberTex:Show()
					captainTex:Hide()
				end
			else
				captainTex:Hide()
				memberTex:Hide()
			end

			local t_fonts = {	[btn:GetName().."NameFont"]	= { value = oneAroundPlayerInfo["name"] }, 
						[btn:GetName().."LevelFont"]	= { value = oneAroundPlayerInfo["level"].."级" },
						[btn:GetName().."CareerFont"]	= { value = GetCareerName( oneAroundPlayerInfo["race"] ) }, 
						[btn:GetName().."ClanFont"]	= { value = oneAroundPlayerInfo["clanName"]  }, 
						[btn:GetName().."WorldFont"]	= { value = oneAroundPlayerInfo["worldName"] }, 
					};
			util.SetFonts( t_fonts );
			btn:Enable();

			if t_selMember["name"] == oneAroundPlayerInfo["name"] then
				btn:Checked();
			end
		end

	end

	local friendInfo  = GameFriendManager:getFriendInfoByName(t_selMember["name"])
	btn		  = getglobal("TeamAroundPlayerFrameAddFriendButton")

	-- 如果是好友
	if friendInfo ~= nil then
		if friendInfo.m_nRelationType == CLIENT_RELATION_TYPE_FRIEND then
			btn:Disable()
		else
			btn:Enable()
		end
	else
		btn:Enable()
	end
end

function TeamAroundTeamButton_OnClick()
	local teamButton   = getglobal("TeamAroundPlayerFrameTeamButton")
	local nameButton   = getglobal("TeamAroundPlayerFrameNameButton")
	local levelButton  = getglobal("TeamAroundPlayerFrameLevelButton")
	local CareerButton = getglobal("TeamAroundPlayerFrameCareerButton")
	local clanButton   = getglobal("TeamAroundPlayerFrameClanButton")
	local worldButton  = getglobal("TeamAroundPlayerFrameWorldButton")
	
	teamButton:Checked()
	nameButton:DisChecked()
	levelButton:DisChecked()
	CareerButton:DisChecked()
	clanButton:DisChecked()
	worldButton:DisChecked()

	nAroundSortWay = SORT_BY_TEAM
	UpdateAroundList()
end

function TeamAroundNameButton_OnClick()
	local teamButton   = getglobal("TeamAroundPlayerFrameTeamButton")
	local nameButton   = getglobal("TeamAroundPlayerFrameNameButton")
	local levelButton  = getglobal("TeamAroundPlayerFrameLevelButton")
	local CareerButton = getglobal("TeamAroundPlayerFrameCareerButton")
	local clanButton   = getglobal("TeamAroundPlayerFrameClanButton")
	local worldButton  = getglobal("TeamAroundPlayerFrameWorldButton")
	
	teamButton:DisChecked()
	nameButton:Checked()
	levelButton:DisChecked()
	CareerButton:DisChecked()
	clanButton:DisChecked()
	worldButton:DisChecked()

	nAroundSortWay = SORT_BY_NAME
	UpdateAroundList()
end

function TeamAroundLevelButton_OnClick()
	local teamButton   = getglobal("TeamAroundPlayerFrameTeamButton")
	local nameButton   = getglobal("TeamAroundPlayerFrameNameButton")
	local levelButton  = getglobal("TeamAroundPlayerFrameLevelButton")
	local CareerButton = getglobal("TeamAroundPlayerFrameCareerButton")
	local clanButton   = getglobal("TeamAroundPlayerFrameClanButton")
	local worldButton  = getglobal("TeamAroundPlayerFrameWorldButton")
	
	teamButton:DisChecked()
	nameButton:DisChecked()
	levelButton:Checked()
	CareerButton:DisChecked()
	clanButton:DisChecked()
	worldButton:DisChecked()

	nAroundSortWay = SORT_BY_LEVEL
	UpdateAroundList()
end

function TeamAroundCareerButton_OnClick()
	local teamButton   = getglobal("TeamAroundPlayerFrameTeamButton")
	local nameButton   = getglobal("TeamAroundPlayerFrameNameButton")
	local levelButton  = getglobal("TeamAroundPlayerFrameLevelButton")
	local CareerButton = getglobal("TeamAroundPlayerFrameCareerButton")
	local clanButton   = getglobal("TeamAroundPlayerFrameClanButton")
	local worldButton  = getglobal("TeamAroundPlayerFrameWorldButton")
	
	teamButton:DisChecked()
	nameButton:DisChecked()
	levelButton:DisChecked()
	CareerButton:Checked()
	clanButton:DisChecked()
	worldButton:DisChecked()

	nAroundSortWay = SORT_BY_CAREER
	UpdateAroundList()
end

function TeamAroundClanNameButton_OnClick()
	local teamButton   = getglobal("TeamAroundPlayerFrameTeamButton")
	local nameButton   = getglobal("TeamAroundPlayerFrameNameButton")
	local levelButton  = getglobal("TeamAroundPlayerFrameLevelButton")
	local CareerButton = getglobal("TeamAroundPlayerFrameCareerButton")
	local clanButton   = getglobal("TeamAroundPlayerFrameClanButton")
	local worldButton  = getglobal("TeamAroundPlayerFrameWorldButton")
	
	teamButton:DisChecked()
	nameButton:DisChecked()
	levelButton:DisChecked()
	CareerButton:DisChecked()
	clanButton:Checked()
	worldButton:DisChecked()

	nAroundSortWay = SORT_BY_CLAN_NAME
	UpdateAroundList()
end

function TeamAroundWorldButton_OnClick()
	local teamButton   = getglobal("TeamAroundPlayerFrameTeamButton")
	local nameButton   = getglobal("TeamAroundPlayerFrameNameButton")
	local levelButton  = getglobal("TeamAroundPlayerFrameLevelButton")
	local CareerButton = getglobal("TeamAroundPlayerFrameCareerButton")
	local clanButton   = getglobal("TeamAroundPlayerFrameClanButton")
	local worldButton  = getglobal("TeamAroundPlayerFrameWorldButton")
	
	teamButton:DisChecked()
	nameButton:DisChecked()
	levelButton:DisChecked()
	CareerButton:DisChecked()
	clanButton:DisChecked()
	worldButton:Checked()

	nAroundSortWay = SORT_BY_WORLD
	UpdateAroundList()
end


-- 契约设置标签按钮
function ClearQiYueList()
	DisCheckAllBtn( "TeamQiYueButton", MAX_QIYUE_BTN_NUM );
	for i = 1, MAX_QIYUE_BTN_NUM do
		local btn = getglobal( "TeamQiYueButton"..i );
		btn:Disable();

		local font = getglobal( btn:GetName().."NameFont" );
		font:SetText( "" );

		font = getglobal( btn:GetName().."QiYueDescFont" );
		font:SetText( "" );
		
		btn = getglobal( "TeamQiYuePictureBtn"..i );
		btn:Hide();

		local chkBtn = getglobal( "TeamQiYueChkBtn"..i );
		chkBtn:Hide();
	end
end

function TeamUpdateQiYueList()
	local teamInfo = TeamManager:getTeamInfo();
	if teamInfo.MemberNum > 0 then
		local t_showIcons = { "TeamQiYueFrame_CaptainMarkButton" };
		util.ShowIcons( t_showIcons );
	else
		local t_hideIcons = { "TeamQiYueFrame_CaptainMarkButton", };
		util.HideIcons( t_hideIcons );
	end

	ClearQiYueList();
	local t_selMember = t_data["TeamApplyListFrame"]["selMember"];
	local nBtnIndex = 1;
	for i = 1, teamInfo.MemberNum do
		local teamMember = teamInfo.TeamMember[i-1];
		local btn = getglobal( "TeamQiYueButton"..i );
		btn:Enable();
		local t_fonts = {	[btn:GetName().."NameFont"]			= { value = teamMember.MemberName }, 
							[btn:GetName().."QiYueDescFont"]	= { value = teamMember.Avatar.QiYue ~= 0 and 
																			getQiYueDes( teamMember.Avatar.QiYue ) or "" },
						};
		util.SetFonts( t_fonts );
		
		if t_selMember["name"] == teamMember.MemberName then
			btn:Checked();
		end

		btn = getglobal( "TeamQiYuePictureBtn"..i );
		if teamMember.Avatar.QiYue ~= 0 then
			-- TODO: 获取契约路径
			local t_tgas = { [btn:GetName().."QiYueTex"] = { path = DEFAULT_ITEM_PATH }, };
			util.SetTgas( t_tgas );

			local t_showIcons = { btn:GetName().."QiYueTex",};
			util.ShowIcons( t_showIcons );
		else
			local t_hideIcons = { btn:GetName().."QiYueTex",};
			util.HideIcons( t_hideIcons );
		end
		btn:Show();
		
		local chkBtn = getglobal( "TeamQiYueChkBtn"..i );
		chkBtn:setCheckState( teamMember.QiYueValid == CLIENT_QIYUE_STATE_SELECTED );
		chkBtn:Show();

		-- 检查是否是队长
		local captainBtn = getglobal( "TeamQiYueFrame_CaptainMarkButton" );
		if teamMember.Position == TEAM_POSITION_CAPTAIN then
			captainBtn:SetPoint( "TOPLEFT", "TeamQiYueButton"..i, "TOPLEFT", 12, 20  );
		end
	end
end
								-- 获取队伍信息				离开队伍	GE_BATTLE_TO_WORLD
local t_memberFrameEvents = { ["GE_TEAM_GET_TEAMINFO"] = {}, ["GE_TEAM_LEAVETEAM"] = {}, ["GE_BATTLE_TO_WORLD"] = {}, };
t_memberFrameEvents["GE_TEAM_GET_TEAMINFO"].func = 
function ()
	UpdateTeamFrame();
end

t_memberFrameEvents["GE_TEAM_LEAVETEAM"].func = 
function ()
	local t_hideIcons = { "ChangeNameFrame", };
	util.HideIcons( t_hideIcons );
	t_teamAvatars = {};
	UpdateTeamFrame();
end

t_memberFrameEvents["GE_BATTLE_TO_WORLD"].func = 
function ()
	t_memberFrameEvents["GE_TEAM_LEAVETEAM"].func();
end

function TeamMemberFrame_OnLoad()
	-- 注册消息事件
	for event, _ in pairs( t_memberFrameEvents ) do
		this:RegisterEvent( event );
	end

	t_memberFrameEvents.__index = function ( tab, key )
									tab[key] = { func = function () end };
									return tab[key];
								end
	setmetatable( t_memberFrameEvents, t_memberFrameEvents );
end

function TeamMemberFrame_OnEvent()
	if not this:IsShown() then
		return;
	end
	t_memberFrameEvents[arg1].func();
end

function TeamMemberFrame_OnHide()
	local t_hideIcons = { "ChangeNameFrame", };
	util.HideIcons( t_hideIcons );
end

function TeamMemberFrame_OnShow()
	-- 总是默认选中第一个
	t_teamAvatars = {};
	local teamInfo = TeamManager:getTeamInfo();
	t_data[this:GetName()]["selMember"] = { name = "" };
	for i = 1, teamInfo.MemberNum do
		local teamMember = teamInfo.TeamMember[i-1];
		if teamMember.OffLineTime == CLIENT_MEMBER_ONLINE then
			t_data[this:GetName()]["selMember"] = { name = teamMember.MemberName };
			break;
		end
	end

	CheckLable( this:GetName(), this:GetName().."MemberButton" );
	UpdateTeamFrame();
end

-- TODO
function TeamMemberButton_OnClick()
	local t_teamFrames = { "TeamMemberFrame", "TeamApplyListFrame", "TeamAroundPlayerFrame", "TeamQiYueFrame" };
	for _, name in ipairs( t_teamFrames ) do
		local frame = getglobal( name );
		frame:Hide();
	end
	
	local showFrame		= getglobal( "TeamMemberFrame" );
	local curShowFrame	= getglobal( this:GetParent() );
	showFrame:SetPoint( "topleft", "UIClient", "topleft", curShowFrame:GetRealLeft(), curShowFrame:GetRealTop() );
	showFrame:Show();
end

-- 点击队伍主面板的申请列表标签按钮响应--4个面板共用
function TeamApplyListButton_OnClick()
	local t_teamFrames = { "TeamMemberFrame", "TeamApplyListFrame", "TeamAroundPlayerFrame", "TeamQiYueFrame" };
	for _, name in ipairs( t_teamFrames ) do
		local frame = getglobal( name );
		frame:Hide();
	end
	
	local showFrame		= getglobal( "TeamApplyListFrame" );
	local curShowFrame	= getglobal( this:GetParent() );
	showFrame:SetPoint( "topleft", "UIClient", "topleft", curShowFrame:GetRealLeft(), curShowFrame:GetRealTop() );
	showFrame:Show();
end

-- 点击队伍主面板的周围玩家标签按钮响应--4个面板共用
-- TODO
function TeamAroundPlayerButton_OnClick()
	local t_teamFrames = { "TeamMemberFrame", "TeamApplyListFrame", "TeamAroundPlayerFrame", "TeamQiYueFrame" };
	for _, name in ipairs( t_teamFrames ) do
		local frame = getglobal( name );
		frame:Hide();
	end
	
	local showFrame		= getglobal( "TeamAroundPlayerFrame" );
	local curShowFrame	= getglobal( this:GetParent() );
	showFrame:SetPoint( "topleft", "UIClient", "topleft", curShowFrame:GetRealLeft(), curShowFrame:GetRealTop() );
	showFrame:Show();
end

-- 点击队伍主面板的设置契约标签按钮响应--4个面板共用
-- TODO
function TeamSetQiYueButton_OnClick()
	local t_teamFrames = { "TeamMemberFrame", "TeamApplyListFrame", "TeamAroundPlayerFrame", "TeamQiYueFrame" };
	for _, name in ipairs( t_teamFrames ) do
		local frame = getglobal( name );
		frame:Hide();
	end
	
	local showFrame = getglobal( "TeamQiYueFrame" );
	showFrame:SetPoint( "topleft", this:GetParent(), "topleft", 0, 0 );
	showFrame:Show();
end

-- 此为点击队伍主面板的队伍成员信息按钮的时候响应
function TeamMemberFrame_MemberButton_OnClick()
	local nIndex = this:GetClientID();
	local teamInfo = TeamManager:getTeamInfo();
	local teamMember = teamInfo.TeamMember[nIndex-1];
	t_data["TeamMemberFrame"]["selMember"] = { name = teamMember.MemberName };
	
	for i = 1, MAX_TEAM_MEMBER do
		local tex = getglobal( "TeamMemberFrame_MemberButton"..i.."_SelectHilightTexture" );
		tex:Hide();
	end

	local tex = getglobal( this:GetName().."_SelectHilightTexture" );
	tex:Show();

	if arg1 == "RightButton" then
		ShowCharacterRightFrame( teamMember.MemberName );
	end
end

-- 点击队伍主面板的加为好友按钮响应
function TeamMemberFrame_AddFriendButton_OnClick()
	local teamInfo	= TeamManager:getTeamInfo();

	local t_selMember = t_data[this:GetParent()]["selMember"];
	if t_selMember["name"] == "" then
		ShowMidTips( "请先选择要加为好友的玩家" );
		return;
	end
	
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer:getName() == t_selMember["name"] then
		ShowMidTips( "不能添加自己为好友" );
		return;
	end

	if not isRoleInRequestList( t_selMember["name"] ) then
		GameFriendManager:addFriend( t_selMember["name"], "" );
	end
end

-- 点击队伍主面板的队伍跟随按钮响应
function TeamMemberWeiXingButton_OnClick()
	local teamInfo = TeamManager:getTeamInfo();
	if teamInfo.MemberNum == 0 then
		ShowMidTips( "你还没加入队伍" );
		return;
	end
	
	local mainplayer = ActorMgr:getMainPlayer();
	if not mainplayer:isCaptain() then
		ShowMidTips( "你不是队长，无权进行该操作" );
		return;
	end

	GameFollowManager:appealWeiXing();
end

-- 点击队伍主面板的解散队伍按钮响应
-- TODO: 加个2次确认解散的操作
function TeamMemberFrameJieSanTeamButton_OnClick()
	local teamInfo	= TeamManager:getTeamInfo();
	if teamInfo.MemberNum == 0 then
		ShowMidTips( "你还没加入队伍" );
		return;
	end

	local mainplayer = ActorMgr:getMainPlayer();
	if not mainplayer:isCaptain() then
		ShowMidTips( "你不是队长，无权进行该操作" );
		return;
	end
	
	MessageBox("提示", "确定需要解散队伍？");
	MessageBoxFrame:SetClientString("解散队伍");
end

-- 点击队伍主面板的转让队长按钮响应
-- TODO
function TeamMemberFrame_ZhuangRangCaptainButton_OnClick()
	local teamInfo	= TeamManager:getTeamInfo();
	if teamInfo.MemberNum == 0 then
		ShowMidTips( "你还没加入队伍" );
		return;
	end

	local mainplayer = ActorMgr:getMainPlayer();
	if not mainplayer:isCaptain() then
		ShowMidTips( "你不是队长，无权进行该操作" );
		return;
	end

	local t_selMember = t_data[this:GetParent()]["selMember"];
	if t_selMember["name"] == "" then
		ShowMidTips( "请选择要转让的玩家" );
		return;
	end
	TeamManager:Team_OP_CaptainChange( t_selMember["name"] );
end

-- 自动接受组队跟随勾选框
function TeamMemberFrame_AutoFollowAccept_OnClick()
	local checkBtn = getglobal("TeamMemberFrameAutoFollowAccept")
	local systemSetting = GameMgr:getUserSystemConfig();
	if checkBtn:GetCheckState() then
		systemSetting:setGameOptValue( GAME_OPT_AUTO_ACCEPT_TEAM_FOLLOW, 1 );
	else
		systemSetting:setGameOptValue( GAME_OPT_AUTO_ACCEPT_TEAM_FOLLOW, 0 );
	end
end

--判断当前状态是否能进行组队跟随
function canTeamFollow()
	local mainplayer = ActorMgr:getMainPlayer();
	
	if not mainplayer:isCaptain() then
		local captainInfo = getCaptain();
		local captainRealMapID = TeamManager:getMapDefID( captainInfo.MapID );
		if MapShower:getRealMapId() ~= captainRealMapID then
			ShowMidTips("与队长不在同一地图或线路，无法进行组队跟随");
			return false;
		end

		if mainplayer:isInRide() then
			local rider = mainplayer:getRider();
			if not rider:isOwnRide() then
				ShowMidTips("队员骑乘他人坐骑时无法进行组队跟随");
				return false;
			end
			local pActor = ActorMgr:findActorByName( getCaptain().MemberName );
			if pActor ~= nil and pActor:isInRide() then
				local rider = pActor:getRider();
				if rider:getRideItem().m_nOwnerID == mainplayer:GetID() then
					ShowMidTips("和队长共同骑乘时无法进行组队跟随");
					return false;
				end
			end
		end

		local equip = mainplayer:getEquip()
		if equip:isUsedTmp() then
			local nTmpItemId = equip:getTmpMachin();				
			if nTmpItemId ~= nil or nTmpItemId ~= 0 then
				ShowMidTips("当前状态不能进行组队跟随");
				return false;
			end					
		end				
	end

	if mainplayer:isOnMonsterMachine() then
		ShowMidTips("使用怪物机械时无法进行组队跟随");
		return false;
	end

	local MinMapContal = MapShower:GetMinMapContal();
	if MinMapContal:getMapID() == CLAN_CITY_MAP_ID or MinMapContal:getMapID() == BEACH_MAP_ID or 
		MinMapContal:getMapID() == CARNIVAL_MAP_ID or GameBattle:isRun() then
		ShowMidTips("当前地图禁止进行组队跟随");
		return false;
	end
	return true;
end

function CancelTeamFollowState()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer:isInTeam() and ( TeamManager:isInTeamFollow() or isAnyTeamMemberInFollow() ) then
		CancelTeamFollow();
	end
end

--请求组队跟随按钮
function TeamMemberFrame_RequestTeamFollowButton_OnClick()
	local teamInfo = TeamManager:getTeamInfo();
	if teamInfo.MemberNum == 0 then
		ShowMidTips( "你还没加入队伍" );
		return;
	end
	if canTeamFollow() then
		TeamManager:Team_OP_AppealTeamFollow();
		setTeamFollowBegin();
	end	
end

--取消组队跟随按钮
function TeamMemberFrame_CancelTeamFollowButton_OnClick()
	local teamInfo = TeamManager:getTeamInfo();
	if teamInfo.MemberNum == 0 then
		ShowMidTips( "你还没加入队伍" );
		return;
	end
	if TeamManager:isInTeamFollow() or isAnyTeamMemberInFollow() then
		TeamManager:Team_OP_CancelTeamFollow();
	end
end

-- 更改队伍名字
-- 确定更改队伍名字
function ChangeNameConfirm_OnClick()
	local edit = getglobal( "ChangeName_EditBox" );
	local szChangeName = edit:GetText();
	if szChangeName ~= nil then
		TeamManager:Team_OP_ChangeName(szChangeName);
	end
	ChangeNameFrame:Hide();
	edit:Clear();
end

--取消更改名字
function ChangeNameCancel_OnClick()
	ChangeNameFrame:Hide();
	ChangeName_EditBox:Clear();
end

function ChangeNameFrame_OnShow()
	local edit = getglobal( "ChangeName_EditBox" );
	edit:Clear();
	SetFocusFrame( "ChangeName_EditBox" );
	this:SetPoint( "center", "TeamMemberFrame", "center", 0, 0 );
end

-- 点击队伍主面板的改变队伍名称按钮响应
function TeamMemberChangeNameButton_OnClick()
	local teamInfo	= TeamManager:getTeamInfo();
	if teamInfo.MemberNum == 0 then
		ShowMidTips( "你还没加入队伍" );
		return;
	end

	local mainplayer = ActorMgr:getMainPlayer();
	if not mainplayer:isCaptain() then
		ShowMidTips( "你不是队长，无权进行该操作" );
		return;
	end

	ChangeNameFrame:Show();
	SetFocusFrame("ChangeName_EditBox");
end

-- 点击队伍主面板的请离队伍按钮响应
-- TODO
function TeamMemberFrame_KickOffTeamButton_OnClick()
	local teamInfo	= TeamManager:getTeamInfo();
	if teamInfo.MemberNum == 0 then
		ShowMidTips( "你还没加入队伍" );
		return;
	end
	local mainplayer = ActorMgr:getMainPlayer();
--[[
	if not mainplayer:isCaptain() then
		ShowMidTips( "你不是队长，无权进行该操作" );
		return;
	end
]]--
	local parent = getglobal(this:GetParent());
	local nIndex = parent:GetClientID();
	local teamInfo = TeamManager:getTeamInfo();
	local teamMember = teamInfo.TeamMember[nIndex-1];

	if teamMember.MemberName == "" then
		ShowMidTips( "请选择要踢出队伍的玩家" );
		return;
	end

	if mainplayer:getName() == teamMember.MemberName then
		ShowMidTips( "不能将自己踢出队伍" );
		return;
	end

	if teamInfo.MemberNum == 2 then
		MessageBox("请离队伍", "队伍人数少于2人队伍将被解散，确定需要解散队伍？");
		MessageBoxFrame:SetClientString(teamMember.MemberName);
	else
		TeamManager:Team_OP_Kick( teamMember.MemberName );
	end
end

-- 点击队伍主面板的离开队伍按钮响应
-- TODO
function TeamMemberFrame_LeaveTeamButton_OnClick()
	local teamInfo	= TeamManager:getTeamInfo();
	if teamInfo.MemberNum == 0 then
		ShowMidTips( "你还没加入队伍" );
		return;
	end
	
	local frame = getglobal( "ConfirmLeaveTeamFrame" );
	if frame:IsShown() then
		return;
	end
	frame:Show();
	--TeamManager:Team_OP_Exit();
end

-- 鼠标移入契约图标的时候响应
-- TODO
function TeamMemberFrame_QiYuePictureBtn_OnEnter()
	local teamInfo = TeamManager:getTeamInfo();
	
	-- 若成员没装备契约,则不显示该契约的tips
	if teamInfo.QiYue == 0 then
		return;
	end

	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = getQiYueDescName( teamInfo.QiYue ), 
						frame = "SetQiYueButtonFrame", button = this:GetName() } );
end

-- 鼠标离开契约图标的时候响应
-- TODO
function TeamMemberFrame_QiYuePictureBtn_OnLeave()
end

-- 申请列表面板
local t_applyFrameEvents = { ["GE_TEAM_GET_APPLY"] = {}, };
t_applyFrameEvents["GE_TEAM_GET_APPLY"].func = 
function ()
	if ShowChangeRoleFrame:IsShown() then
		return;
	end

	UpdateApplyList();
end

function TeamApplyListFrame_OnLoad()
	for event, _ in pairs( t_applyFrameEvents ) do
		this:RegisterEvent( event );
	end

	t_applyFrameEvents.__index = function ( tab, key )
									tab[key] = { func = function () end };
									return tab[key];
								end
	setmetatable( t_applyFrameEvents, t_applyFrameEvents );

	for i = 1, MAX_APPLY_BTN_NUM do
		if i % 2 == 0 then
			local btn = getglobal("TeamApplyListFrameApplyPlayerInfoButton" .. i)
			local tex = getglobal(btn:GetName() .. "NameTex")
			tex:Hide()
			tex = getglobal(btn:GetName() .. "LevelTex")
			tex:Hide()
			tex = getglobal(btn:GetName() .. "CareerTex")
			tex:Hide()
			tex = getglobal(btn:GetName() .. "ClanTex")
			tex:Hide()
			tex = getglobal(btn:GetName() .. "WorldTex")
			tex:Hide()
		end
	end
end

function TeamApplyListFrame_OnEvent()
	if not this:IsShown() then
		return;
	end
	t_applyFrameEvents[arg1].func();

	if arg1 == "GE_TEAM_GET_APPLY" then
		AutoAcceptApply()
	end
end

function TeamApplyListFrame_OnHide()
end

function TeamApplyListFrame_OnShow()
	local nameButton   = getglobal("TeamApplyListFrameNameButton")
	local levelButton  = getglobal("TeamApplyListFrameLevelButton")
	local CareerButton = getglobal("TeamApplyListFrameCareerButton")
	local clanButton   = getglobal("TeamApplyListFrameClanButton")
	local worldButton  = getglobal("TeamApplyListFrameWorldButton")

	nameButton:Checked()
	levelButton:DisChecked()
	CareerButton:DisChecked()
	clanButton:DisChecked()
	worldButton:DisChecked()

	TeamApplyNameButton_OnClick()

	local friendBtn		= getglobal("TeamApplyListFrameAddFriendButton")
	friendBtn:Enable()
	t_data[this:GetName()]["selMember"] = { name = "" };
	if TeamManager:getTeamApplyInfoSize() > 0 then
		local applyPlayerInfo = TeamManager:getTeamApplyInfoByIndex( 0 );
		t_data[this:GetName()]["selMember"] = { name = applyPlayerInfo.RoleName };

		local btn = getglobal("TeamApplyListFrameApplyPlayerInfoButton1")
		btn:Checked()
		
		local friendInfo  = GameFriendManager:getFriendInfoByName(applyPlayerInfo.RoleName)
		btn		  = getglobal("TeamApplyListFrameAddFriendButton")

		-- 如果是好友
		if friendInfo ~= nil then
			if friendInfo.m_nRelationType == CLIENT_RELATION_TYPE_FRIEND then
				btn:Disable()
			else
				btn:Enable()
			end
		else
			btn:Enable()
		end
	end

	CheckLable( this:GetName(), this:GetName().."ApplyListButton" );
	local slider = getglobal( "TeamApplyListFrameScrollBar" );
	slider:SetValue( 0 );
	UpdateApplyList();
end

-- 点击向上滚动按钮的时候响应
function TeamApplyListFrame_ScrollUpBtn_OnClick()
	local slider = getglobal( "TeamApplyListFrameScrollBar" );
	if not slider:IsShown() then
		return;
	end

	local nValue = slider:GetValue() - slider:GetValueStep();
	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end

-- 点击向下滚动按钮的时候响应
function TeamApplyListFrame_ScrollDownBtn_OnClick()
	local slider = getglobal( "TeamApplyListFrameScrollBar" );
	if not slider:IsShown() then
		return;
	end

	local nValue	= slider:GetValue() + slider:GetValueStep();
	local nMaxValue = slider:GetMaxValue();
	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

function TeamApplyListFrame_ScrollBar_OnValueChanged()
	local slider	= getglobal( "TeamApplyListFrameScrollBar" );
	local nValue	= slider:GetValue();
	local nMaxValue = slider:GetMaxValue();

	if nValue <= nMaxValue then
		UpdateApplyList();
	end
end

function TeamApplyListFrame_AutoAcceptCheckButton_OnClick()
	local checkBtn = getglobal("TeamApplyListFrameAutoAccept")
	if checkBtn:GetCheckState() == true then
		bAutoAccept = true
		AutoAcceptApply()
	else
		bAutoAccept = false
	end
end

function ClearSelMember()
	local t_selMember = t_data["TeamApplyListFrame"]["selMember"];
	local nId = -1;

	for i = 1, TeamManager:getTeamApplyInfoSize() do
		if TeamManager:getTeamApplyInfoByIndex(i - 1).RoleName == t_selMember["name"] then
			nId = i;
			break;
		end
	end

	nId = nId + 1;
	if nId < TeamManager:getTeamApplyInfoSize() then
		local playerInfo = TeamManager:getTeamApplyInfoByIndex( nId );
		if playerInfo ~= nil then
			t_selMember["name"] = playerInfo.RoleName;
		else
			t_selMember["name"] = "";
		end
	else
		t_selMember["name"] = "";
	end

end

-- 点击拒绝全部按钮的时候响应
function TeamApplyListFrame_RefuseAllButton_OnClick()
	TeamManager:Team_OP_ApplyClear();
	CancelAllBlinkApply();
	ClearSelMember();
	UpdateApplyList();
end

-- 点击拒绝某个人入队按钮的时候响应
function TeamApplyListFrame_RefuseResButton_OnClick()
	local t_selMember = t_data[this:GetParent()]["selMember"];
	if t_selMember["name"] == "" then
		ShowMidTips( "请先选定玩家" );
		return;
	end
	
	local applyPlayer = TeamManager:getTeamApplyInfo( t_selMember["name"] )
	CancelBlinkPlayer( t_selMember["name"] );
	TeamManager:Team_OP_ApplyRefuse( applyPlayer.RoleID.nHeight, applyPlayer.RoleID.nLow );
	ClearSelMember();
	UpdateApplyList();
end

-- 点击同意入队按钮的时候响应
function TeamApplyListFrame_AcceptButton_OnClick()
	local t_selMember = t_data[this:GetParent()]["selMember"];
	if t_selMember["name"] == "" then
		ShowMidTips( "请先选定玩家" );
		return;
	end

	local applyPlayer = TeamManager:getTeamApplyInfo( t_selMember["name"] )
	TeamManager:Team_OP_ApplyAccept( applyPlayer.RoleID.nHeight, applyPlayer.RoleID.nLow );
	CancelBlinkPlayer( t_selMember["name"] );
	ClearSelMember();
	UpdateApplyList();
end

-- 点击申请玩家信息按钮的时候响应
function TeamApplyPlayerInfoButton_OnClick()
	DisCheckAllBtn( "TeamApplyListFrameApplyPlayerInfoButton", MAX_APPLY_BTN_NUM );
	this:Checked();
	local nameFont = getglobal( this:GetName().."NameFont" );
	t_data[this:GetParent()]["selMember"] = { name = nameFont:GetText() };

	local t_selMember = t_data[this:GetParent()]["selMember"];
	local friendInfo  = GameFriendManager:getFriendInfoByName(t_selMember["name"])
	local btn         = getglobal("TeamApplyListFrameAddFriendButton")

	if arg1 == "RightButton" then
		ShowApplyRightFrame( nameFont:GetText() )
	end

	-- 如果是好友
	if friendInfo ~= nil then
		if friendInfo.m_nRelationType == CLIENT_RELATION_TYPE_FRIEND then
			btn:Disable()
		else
			btn:Enable()
		end
	else
		btn:Enable()
	end
end

-- 周围玩家面板
function TeamAroundPlayerFrame_OnLoad()
	for i = 1, MAX_AROUND_BTN_NUM do
		local captainTex = getglobal("TeamAroundPlayerFrameNearPlayerInfoButton" .. i .. "CaptainTex")
		local memberTex  = getglobal("TeamAroundPlayerFrameNearPlayerInfoButton" .. i .. "MemberTex")
			captainTex:Hide()
			memberTex:Hide()
		if i % 2 == 0 then
			local btn = getglobal("TeamAroundPlayerFrameNearPlayerInfoButton" .. i)
			local tex = getglobal(btn:GetName() .. "TeamTex")
			tex:Hide()
			tex = getglobal(btn:GetName() .. "NameTex")
			tex:Hide()
			tex = getglobal(btn:GetName() .. "LevelTex")
			tex:Hide()
			tex = getglobal(btn:GetName() .. "CareerTex")
			tex:Hide()
			tex = getglobal(btn:GetName() .. "ClanTex")
			tex:Hide()
			tex = getglobal(btn:GetName() .. "WorldTex")
			tex:Hide()
		end
	end
end

function TeamAroundPlayerFrame_OnEvent()
end

function TeamAroundPlayerFrame_OnHide()
end

function TeamAroundPlayerFrame_OnShow()
	local teamButton   = getglobal("TeamAroundPlayerFrameTeamButton")
	local nameButton   = getglobal("TeamAroundPlayerFrameNameButton")
	local levelButton  = getglobal("TeamAroundPlayerFrameLevelButton")
	local CareerButton = getglobal("TeamAroundPlayerFrameCareerButton")
	local clanButton   = getglobal("TeamAroundPlayerFrameClanButton")
	local worldButton  = getglobal("TeamAroundPlayerFrameWorldButton")
	
	teamButton:Checked()
	nameButton:DisChecked()
	levelButton:DisChecked()
	CareerButton:DisChecked()
	clanButton:DisChecked()
	worldButton:DisChecked()

	TeamAroundTeamButton_OnClick()

	local friendBtn = getglobal("TeamAroundPlayerFrameAddFriendButton")
	friendBtn:Enable()
	-- 总是默认选中第一个
	TeamManager:updateNearPlayer();
	t_data[this:GetName()]["selMember"] = { name = "" };
	if TeamManager:getNearPlayerCount() > 0 then
		local nearActor = TeamManager:getNearPlayer( 0 );
		t_data[this:GetName()]["selMember"] = { name = nearActor:getName() };
	
		local btn = getglobal("TeamAroundPlayerFrameNearPlayerInfoButton1")
		btn:Checked()
		
		local friendInfo  = GameFriendManager:getFriendInfoByName(nearActor:getName())
		btn		  = getglobal("TeamAroundPlayerFrameAddFriendButton")

		-- 如果是好友
		if friendInfo ~= nil then
			if friendInfo.m_nRelationType == CLIENT_RELATION_TYPE_FRIEND then
				btn:Disable()
			else
				btn:Enable()
			end
		else
			btn:Enable()
		end
	end

	CheckLable( this:GetName(), this:GetName().."AroundPlayerButton" );
	local slider = getglobal( "TeamAroundPlayerFrameScrollBar" );
	slider:SetValue( 0 );
	UpdateAroundList();
end

-- 点击向上滚动按钮的时候响应
function  TeamAroundPlayerFrame_ScrollUpBtn_OnClick()
	local slider = getglobal( "TeamAroundPlayerFrameScrollBar" );
	if not slider:IsShown() then
		return;
	end

	local nValue = slider:GetValue() - slider:GetValueStep();
	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end

-- 点击向下滚动按钮的时候响应
function TeamAroundPlayerFrame_ScrollDownBtn_OnClick()
	local slider = getglobal( "TeamAroundPlayerFrameScrollBar" );
	if not slider:IsShown() then
		return;
	end

	local nValue	= slider:GetValue() + slider:GetValueStep();
	local nMaxValue = slider:GetMaxValue();
	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

function TeamAroundPlayerFrame_ScrollBar_OnValueChanged()
	local slider	= getglobal( "TeamAroundPlayerFrameScrollBar" );
	local nValue	= slider:GetValue();

	local nMaxValue = slider:GetMaxValue();

	if nValue <= nMaxValue then
		UpdateAroundList();
	end
end

function SelectFirstAroundPlayer()
	t_data["TeamAroundPlayerFrame"]["selMember"] = { name = "" };
	if TeamManager:getNearPlayerCount() > 0 then
		local nearActor = TeamManager:getNearPlayer( 0 );
		t_data["TeamAroundPlayerFrame"]["selMember"] = { name = nearActor:getName() };
	end
end

-- 刷新按钮
function TeamAroundPlayerFrame_RefurbishButton_OnClick()
	TeamManager:updateNearPlayer();
	SelectFirstAroundPlayer();
	UpdateAroundList();
end

-- 邀请入队按钮
function TeamAroundPlayerFrame_InviteButton_OnClick()
	local t_selMember = t_data[this:GetParent()]["selMember"];
	if t_selMember["name"] == "" then
		ShowMidTips( "请先选定玩家" );
		return;
	end

	TeamManager:Team_OP_Add( t_selMember["name"] );
end

-- [[申请入队按钮
function TeamAroundPlayerFrame_ApplyButton_OnClick()
	local t_selMember = t_data[this:GetParent()]["selMember"];
	if t_selMember["name"] == "" then
		ShowMidTips( "请先选定玩家" );
		return;
	end
	
	TeamManager:Team_OP_Apply( t_selMember["name"] );
end
--]]
function TeamNearPlayerInfoButton_OnClick()
	local nameFont = getglobal( this:GetName().."NameFont" );

	if arg1 == "RightButton" then
		ShowAroundRightFrame( nameFont:GetText() )
	end	

	DisCheckAllBtn( "TeamAroundPlayerFrameNearPlayerInfoButton", MAX_AROUND_BTN_NUM );
	this:Checked();
	
	t_data[this:GetParent()]["selMember"] = { name = nameFont:GetText() };

	local friendInfo  = GameFriendManager:getFriendInfoByName(nameFont:GetText())
	local btn         = getglobal("TeamAroundPlayerFrameAddFriendButton")

	-- 如果是好友
	if friendInfo ~= nil then
		if friendInfo.m_nRelationType == CLIENT_RELATION_TYPE_FRIEND then
			btn:Disable()
		else
			btn:Enable()
		end
	else
		btn:Enable()
	end
end

-- 组队契约面板
function TeamQiYueFrame_OnClick()
	DisCheckAllBtn( "TeamQiYueButton", MAX_TEAM_QIYUE_BUTTON );
	this:Checked();
end

-- 鼠标移入契约图标按钮
function TeamQiYuePictureBtn_OnEnter()
	local nIndex		= this:GetClientID();
	local teamInfo		= TeamManager:getTeamInfo();
	local teamMember	= teamInfo.TeamMember[nIndex-1];
	local nQiYueID		= teamMember.Avatar.QiYue;
	-- 若成员没装备契约,则不显示该契约的tips
	if nQiYueID == 0 then
		return;
	end

	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = getQiYueDescName( nQiYueID ), 
						frame =	"TeamQiYueFrame", button = this:GetName() } );
end

-- 鼠标移出契约图标按钮
function TeamQiYuePictureBtn_OnLeave()
	local btn = getglobal( "TeamQiYueButton"..this:GetClientID() );
	btn:DisHighlight();
end

-- 选定契约的check按钮
function TeamQiYueChkBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	-- 是否勾选的状态根据服务器的数据来设置，不能客户端自动设置
	this:setCheckState( not this:GetCheckState() );

	if not mainplayer:isCaptain() then
		ShowMidTips( "你不是队长，无权进行该操作" );
		return;
	end

	local nIndex		= this:GetClientID();
	local teamInfo		= TeamManager:getTeamInfo();
	local teamMember	= teamInfo.TeamMember[nIndex-1];
	if teamMember.Avatar.QiYue == 0 then
		ShowMidTips( "该队员没有装备契约" );
		return;
	end
	
	TeamManager:switchQiYue( teamMember.QiYueValid == CLIENT_QIYUE_STATE_SELECTED and "" or teamMember.MemberName );
end

function TeamQiYueChkBtn_OnEnter()
	local btn = getglobal( "TeamQiYueButton"..this:GetClientID() );
	btn:Highlight();
end

function TeamQiYueChkBtn_OnLeave()
	local btn = getglobal( "TeamQiYueButton"..this:GetClientID() );
	btn:DisHighlight();
end

local t_qiYueFrameEvents = { ["GE_TEAM_GET_TEAMINFO"] = {}, ["GE_TEAM_LEAVETEAM"] = {}, };
t_qiYueFrameEvents["GE_TEAM_GET_TEAMINFO"].func = 
function ()
	TeamUpdateQiYueList();
end

t_qiYueFrameEvents["GE_TEAM_LEAVETEAM"].func = 
function ()
	TeamQiYueFrame:Hide();
end


function TeamQiYueFrame_OnLoad()
	for i = 1, MAX_TEAM_QIYUE_BUTTON do
		local btn = getglobal( "TeamQiYueButton"..i );
		btn:SetOverlayRegion( -5, -14, 879, 82 );
	end

	for event, _ in pairs( t_qiYueFrameEvents ) do
		this:RegisterEvent( event );
	end

	t_qiYueFrameEvents.__index = function ( tab, key )
									tab[key] = { func = function () end };
									return tab[key];
								end
	setmetatable( t_qiYueFrameEvents, t_qiYueFrameEvents );
end

function TeamQiYueFrame_OnEvent()
	if not this:IsShown() then
		return;
	end
	t_qiYueFrameEvents[arg1].func();
end

function TeamQiYueFrame_OnHide()
end

function TeamQiYueFrame_OnShow()
	-- 总是默认选中第一个
	local teamInfo = TeamManager:getTeamInfo();
	t_data[this:GetName()]["selMember"] = { name = "" };
	if teamInfo.MemberNum > 0 then
		local teamMember = teamInfo.TeamMember[0];
		t_data[this:GetName()]["selMember"] = { name = teamMember.TeamName };
	end

	CheckLable( this:GetName(), this:GetName().."SetQiYueButton" );
	TeamUpdateQiYueList();
end
