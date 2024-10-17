-- 队友头像面板最多的buf数目
MAX_TEAM_GOOD_BUFF	= 13;
MAX_TEAM_DEBUFF		= 8;
FOBIDDEN_SHOW_TEAM_PHOTO_COPY = "狂沙角斗场";

local t_TeamPhotoControl = {};

local isTeamPhotoShow = true

function GetTeamPhotoControl()
	return t_TeamPhotoControl;
end

function SetTeamPhotoShowShow(show)
	isTeamPhotoShow = show ~= false
	if not isTeamPhotoShow then
		local frame = getglobal( "TeamRoleFrame" );
		frame:Hide();
		ClearTeamRoleButtonFrame();
	end
end

t_TeamPhotoControl["canShowTeamPhoto"] = 
function ( self )
	return isTeamPhotoShow;
end

-- 跟随操作
function Appeal( szCaptainName )
	--print( "Appeal--szCaptainName = "..szCaptainName );
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	if mainplayer:isCaptain() then
		return;
	end

	TroopInviteFollowFrame:SetClientString( szCaptainName );
	TroopInviteFollowFrame:Show();
end

--队伍跟随操作确认
function TeamFollowAppeal()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	if not canTeamFollow() then
		return;
	end
	local systemSetting = GameMgr:getUserSystemConfig();
	if systemSetting:getGameOptValue( GAME_OPT_AUTO_ACCEPT_TEAM_FOLLOW ) ~= 0 then	
		TeamManager:Team_OP_AppealTeamFollow();
		setTeamFollowBegin();
		TroopInviteTeamFollowFrame:Hide();
		return;
	end
	TroopInviteTeamFollowFrameBlackButtonInviteFont:SetText( "队长要求你跟随队伍，确定要跟随吗？" );
	TroopInviteTeamFollowFrame:Show();
end

function TroopInviteFollowFrame_AcceptButton_OnClick()
	local szCaptainName = TroopInviteFollowFrame:GetClientString();
	TroopInviteFollowFrame:Hide();
	-- 若在视野范围内没找到队长，则提示不能跟随
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local teamMember = TeamManager:getTeamMemberInfo( szCaptainName );
	if teamMember == nil then
		ShowMidTips( "你当前不在队伍中" )
		return;
	end
	if teamMember.MapID == BEACH_MAP_ID then
		ShowMidTips( "您或跟随目标处于不能跟随位置，跟随失败" );
		return;
	end
	GameFollowManager:beginWeiXing( szCaptainName, CLIENT_CAPTAIN_FOLLOW );
end

function TroopInviteFollowFrame_RufuseOneButton_OnClick()
	TroopInviteFollowFrame:Hide();
end

function TroopInviteFollowFrame_OnHide()
	this:SetClientUserData( 0, 0 );
end

function TroopInviteFollowFrame_OnUpdate()
	local szCaptainName = TroopInviteFollowFrame:GetClientString();
	local teamMember = TeamManager:getTeamMemberInfo( szCaptainName );
	if teamMember == nil or teamMember.OffLineTime ~= 0 then
		TroopInviteFollowFrame:Hide();
	end
end

local TickTime = 0;
function TroopInviteTeamFollowFrame_OnUpdate()
	if TickTime ~= 0 then
		if os.clock() - TickTime > 10 then
			TroopInviteTeamFollowFrame:Hide();
		end
	else
		TickTime = os.clock();
	end	
end

function TroopInviteTeamFollowFrame_OnHide()
	TickTime = 0;
	--this:SetClientUserData( 0, 0 );
end

function TroopInviteTeamFollowFrameAcceptButton_OnClick()
	local szCaptainName = TroopInviteTeamFollowFrame:GetClientString();
	TroopInviteTeamFollowFrame:Hide();
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local teamInfo = TeamManager:getTeamInfo();
	if teamInfo.MemberNum == 0 then
		ShowMidTips( "你还没加入队伍" );
		return;
	end
	if canTeamFollow() then
		TeamManager:Team_OP_AppealTeamFollow();
		setTeamFollowBegin();
		TroopInviteTeamFollowFrame:Hide();
	end
end

function TroopInviteTeamFollowFrameRufuseButton_OnClick()
	TroopInviteTeamFollowFrame:Hide();
end

function GetTeamPhotoBtn( szMemberName )
	for i = 1, MAX_TEAM_MEMBER do
		local btn = getglobal( "TeamRoleButton"..i );
		if btn:IsShown() and CompareRoleName( szMemberName, btn:GetClientString() ) then
			return btn;
		end
	end

	return nil;
end

-- 组队头像
function TeamPhotoBuffButton_OnEnter()
	local nID		= this:GetClientUserData( 0 );
	local nLv		= this:GetClientUserData( 1 );
	local nOwnerId	= this:GetClientUserData( 3 );
	local nType		= this:GetClientUserData( 2 );

	if nType == BUFF_TYPE_RIDE then
		 ShowRideBuffTips( { rideId = nID, level = nLv, buffButtonName = this:GetName(), ownerId = nOwnerId, parent = this:GetParent() } );
		 return;
	elseif nType == BUFF_TYPE_FAIRY then
		ShowPetDiathesisTips( { id = nID, level = nLv, ownerId = nOwnerId, button = this:GetName(), parent = this:GetParent() } );
		return;
	elseif nType == BUFF_TYPE_PET_HETI then
		ShowPetHetiBuffTips({index = nID, owner = nOwnerId, button = this:GetName(), parent = this:GetParent()})
	elseif nType == BUFF_TYPE_PET then
		ShowGameTooltip( { rich = "WideGameTooltipRichText", tipsFrame = "WideGameTooltip", text = this:GetClientString(),  
							frame = this:GetName(), button = this:GetName() } );
		SetGameTooltipRelFrame( { this:GetName(), this:GetParent() } );
	end
	
	local roleBtn		= getglobal( this:GetParent() );
	local szName		= roleBtn:GetClientString();
	local member		= TeamManager:getTeamMemberInfo( szName );
	local statusList	= member.StatusInfo;

	for i = 1, statusList.GoodStatusNum do
		local statusInfo = statusList.GoodStatusList[i-1];
		if statusInfo.StatusID == nID and statusInfo.Level == nLv then
			ShowBuffTips( { statusId = nID, level = nLv, totalTime = statusInfo.Time, startTime = 0, 
							buffButtonName = this:GetName(), ownerId = nOwnerId, button = this:GetName(),
							parent = this:GetParent(), overlay = statusInfo.Overlay } );
			return;
		end
	end

	for i = 1, statusList.BadStatusNum do
		local statusInfo = statusList.BadStatusList[i-1];
		if statusInfo.StatusID == nID and statusInfo.Level == nLv then
			ShowBuffTips( { statusId = nID, level = nLv, totalTime = statusInfo.Time, startTime = 0, 
							buffButtonName = this:GetName(), ownerId = nOwnerId, button = this:GetName(),
							parent = this:GetParent(), overlay = statusInfo.Overlay } );
			return;
		end
	end

	for i = 1, statusList.NoLimitStatusNum do
		local statusInfo = statusList.NolimitStatusList[i-1];
		if statusInfo.StatusID == nID and statusInfo.Level == nLv then
			ShowBuffTips( { statusId = nID, level = nLv, totalTime = statusInfo.Time, startTime = 0, 
							buffButtonName = this:GetName(), ownerId = nOwnerId, button = this:GetName(),
							parent = this:GetParent(), overlay = statusInfo.Overlay } );
			return;
		end
	end
end

function TeamPhotoBuffButton_OnLeave()
	GameTooltip:Hide();
	WideGameTooltip:Hide()
end

-- 根据职业来设置职业图片
t_raceTexture =	
{
	-- 战士
	[g_nCAREER_WARRIOR]		= { path = "uires\\ZhuJieMian\\2.dds", UV = { x = 684, y = 516, width = 20, height = 28 } },
	-- 战士单手	守护者
	[g_nCAREER_WARRIOR_SINGEL]	= { path = "uires\\ZhuJieMian\\3.dds", UV = { x = 490, y = 189, width = 23, height = 26 } },
	-- 战士双手	征服者
	[g_nCAREER_WARRIOR_DOUBLE]	= { path = "uires\\ZhuJieMian\\3.dds", UV = { x = 466, y = 189, width = 23, height = 26 } },

	-- 法师
	[g_nCAREER_MAGICOR]		= { path = "uires\\ZhuJieMian\\2.dds", UV = { x = 352, y = 578, width = 20, height = 28 } },
	-- 法师单手	聚能者
	[g_nCAREER_MAGICOR_SINGEL]	= { path = "uires\\ZhuJieMian\\3.dds", UV = { x = 514, y = 216, width = 23, height = 26 } },
	-- 法师双手	炼魂师
	[g_nCAREER_MAGICOR_DOUBLE]	= { path = "uires\\ZhuJieMian\\3.dds", UV = { x = 538, y = 216, width = 23, height = 26 } },

	-- 枪手
	[g_nCAREER_GUNER]			= { path = "uires\\ZhuJieMian\\2.dds", UV = { x = 705, y = 516, width = 20, height = 28 } },
	-- 枪手单手	猎杀者
	[g_nCAREER_GUNER_SINGEL]	= { path = "uires\\ZhuJieMian\\3.dds", UV = { x = 466, y = 216, width = 23, height = 26 } },
	-- 枪手双手	毁灭者
	[g_nCAREER_GUNER_DOUBLE]	= { path = "uires\\ZhuJieMian\\3.dds", UV = { x = 490, y = 216, width = 23, height = 26 } },

	-- 祭师
	[g_nCAREER_CHURCH]			= { path = "uires\\ZhuJieMian\\2.dds", UV = { x = 663, y = 520, width = 20, height = 28 } },
	-- 祭司单手	圣佑者
	[g_nCAREER_CHURCH_SINGEL]	= { path = "uires\\ZhuJieMian\\3.dds", UV = {  x = 538, y = 189, width = 23, height = 26 } },
	-- 祭司双手	驭雷师
	[g_nCAREER_CHURCH_DOUBLE]	= { path = "uires\\ZhuJieMian\\3.dds", UV = {  x = 514, y = 189, width = 23, height = 26 } },
};

function AddTeamBuffer( data )
	local buffBtn	= getglobal( data["btn"] );
	local status	= data["buff"];
	buffBtn:SetClientUserData( 0, data["id"] );
	buffBtn:SetClientUserData( 1, data["level"] );
	buffBtn:SetClientUserData( 2, BUFF_TYPE_NORMAL );
	buffBtn:SetClientUserData( 3, data["targetId"] );
	buffBtn:SetClientString( data["name"] );
	local statusDef		= getStatusDef( data["id"], data["level"] );
	local buffBackTex	= getglobal( buffBtn:GetName() .. "Ground" );
	if buffBackTex:GetTexture() ~= t_buffBackPath[statusDef.Type][statusDef.SubType] then
		buffBackTex:SetTexture( t_buffBackPath[statusDef.Type][statusDef.SubType] );		
	end
	
	local buffIcon	= getglobal( buffBtn:GetName() .. "Icon" );
	buffIcon:SetTexture( GetBuffIconPath( statusDef.IconID ) );	
	
	local buffOverlay = getglobal( buffBtn:GetName() .. "Overlay" );	
	if buffOverlay ~= nil then
		local nOverlay	= data["overlay"];
		buffOverlay:SetText( ( nOverlay == 0 or nOverlay == 1 ) and "" or nOverlay );
	end
	
	buffBtn:Show();
end

function TeamRoleButton_OnEnter()
	local nIndex = this:GetClientString( );
	local member = getTeamMemberByName( this:GetClientString() )
	
	local szText = "";
	if member.OffLineTime ==  CLIENT_MEMBER_ONLINE then
		szText = "#W"..member.Level.."级 "..GetCareerName( member.Career ).."\n".. 
					( MapShower:GetMinMapContal():getMapName( TeamManager:getMapDefID( member.MapID  ) ) );
	else
		szText = "#c787878"..member.Level.."级 "..GetCareerName( member.Career ).."\n".."离线#n";
	end
	-- 显示tips分在线和不在线的情况
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
						button = "cursor", vertical = "bottom", yAdjust = 0 } );
end

-- teamPopManager = { [name] = { text = ?, nShowTime = ?, alpha }, }
local teamPopManager = {};

function GetTeamPopControl()
	return teamPopManager;
end

function clearMemberPop( data )
	local szMemberName = data["name"];
	teamPopManager[szMemberName] = nil;
end

function clearAllMemberPop( )
	teamPopManager = {};
end

function ResetCursorWhenRichHide(data )
	local rich = getglobal( data["rich"] );
	if isPointInFrame( rich:GetName() ) then
		rich:setCursorNormal();
	end
end

function TeamRoleButton_OnUpdate()
	-- 用于关闭弹出时间超过8秒的队伍聊天泡泡框
	local richBack		= getglobal( this:GetName() .. "ChatPopBtn" ); 
	local richText		= getglobal( richBack:GetName() .. "Rich" );
	
	local nCurTime		= GameMgr:getTickTime();
	local szMemberName	= this:GetClientString();
	if not teamPopManager[szMemberName] then
		return;
	end
	
	local fCurAlpha = richBack:GetBackDropBlendAlpha();
	if fCurAlpha == 0 then
		richBack:Hide();
		ResetCursorWhenRichHide( { ["rich"] = richText:GetName() } );
		teamPopManager[szMemberName] = nil;
		return;
	end
	
	if ( nCurTime - teamPopManager[szMemberName].nShowTime >= 100 ) then
		local fNewAlpha = math.max( fCurAlpha - 1 / 30, 0 );
		richBack:SetBackDropBlendAlpha( fNewAlpha );
		richText:SetAlpha( fNewAlpha );
		teamPopManager[szMemberName].alpha		= fNewAlpha;
		teamPopManager[szMemberName].nShowTime	= nCurTime;
	end
end

function TeamRoleButton_OnHide()
	local richBack	= getglobal( this:GetName() .. "ChatPopBtn" );
	local richText	= getglobal( richBack:GetName() .. "Rich" );
	richBack:Hide();
	ResetCursorWhenRichHide( { ["rich"] = richText:GetName() } );

	for i = 1, MAX_TEAM_GOOD_BUFF do
		local buffBtn = getglobal( this:GetName() .. "Buff"..i );
		buffBtn:Hide();
	end
	
	for i = 1, MAX_TEAM_DEBUFF do
		local buffBtn = getglobal( this:GetName() .. "Debuff"..i );
		buffBtn:Hide();
	end
end

-- 当队伍人员发生变动的时候，设置聊天泡泡框的信息
function SetTeamPopRich( button )
	local szMemberName = button:GetClientString();
	if not teamPopManager[szMemberName] then
		return;
	end
	
	local richBack = getglobal( button:GetName() .. "ChatPopBtn" );
	local richText = getglobal( richBack:GetName() .. "Rich" );

	AdjustTeamRichByText( { ["text"] = teamPopManager[szMemberName].text, ["rich"] = richText:GetName(), ["btn"] = richBack:GetName() } );
	richBack:SetBackDropBlendAlpha( teamPopManager[szMemberName].alpha );
	richText:SetAlpha( teamPopManager[szMemberName].alpha );
	richBack:Show();
end

local t_teamRolePhotoEvents =	{	["GE_TEAM_GET_TEAMINFO"]	= {}, ["GE_TARGET_BUF_LIST_UPDATE"] = {},	["GE_TARGET_RIDE_CHG"]		= {}, 
									["GE_UPDATE_CHATMSG"]		= {}, ["GE_TEAM_LEAVETEAM"]			= {},	["GE_ENTER_PLAYERLOGIN"]	= {},
									["GE_TEAM_CHANGE_BOOTY"]	= {}, ["GE_TEAM_ONE_PLAYER_LEAVE"]	= {},	["GE_BATTLE_TO_WORLD"]		= {},
								["GE_MAIN_JOIN_TEAM_SUCC"] = {}, };

t_teamRolePhotoEvents["GE_TEAM_ONE_PLAYER_LEAVE"].func = 
function ()	
	local t_TeamPopControl = GetTeamPopControl();
	for name, _ in pairs( t_TeamPopControl ) do
		if not TeamManager:isInTeam( name ) then
			clearMemberPop( { ["name"] = name } );
			break;
		end
	end
end

t_teamRolePhotoEvents["GE_ENTER_PLAYERLOGIN"].func = 
function ()
	SetTeamPhotoShowShow(true)
end

t_teamRolePhotoEvents["GE_MAIN_JOIN_TEAM_SUCC"].func =
function()
	local mainplayer = ActorMgr:getMainPlayer();
	if not GameWizard:isDo( GAMEWIZARD_FIRST_IN_TEAM ) and not mainplayer:isCaptain() then				
		local xinShouBtn = getglobal( "TeamRoleFrameFollowXinShouBtn" );
		if not xinShouBtn:IsShown() then
			local tex = getglobal( "TeamRoleAppealFollowSignButtonUVAnimationTex" );
			local rich = getglobal( xinShouBtn:GetName().."Rich" );
			tex:SetUVAnimation( 30, true);
			local szText = "#c554623点击队长此图标即可进入#c0000ff组队跟随状态#c554623，再次点击即可取消#c0000ff组队跟随状态";
			local nSelfDefineExtendWidth = 10;
			rich:SetClientString( szText );
			ReSizeTooltip( rich:GetName(), xinShouBtn:GetName(), nSelfDefineExtendWidth );
			xinShouBtn:Show();
			local closeBtn = getglobal( xinShouBtn:GetName().."_CloseBtn" );
			closeBtn:SetPointEx( "topright", "TeamRoleFrameFollowXinShouBtnGuideBtn", "topright", -2, 0 );
		end
	end
end

t_teamRolePhotoEvents["GE_TEAM_GET_TEAMINFO"].func = 
function ()
	local t_TeamPhotoControl = GetTeamPhotoControl();
	if not t_TeamPhotoControl:canShowTeamPhoto() then
		return;
	end

	local frame = getglobal( "TeamRoleFrame" );
	local teamInfo = TeamManager:getTeamInfo();--获取队伍信息

	if teamInfo.MemberNum == 0 then
		frame:Hide();
	elseif not frame:IsShown() then
		frame:Show();
	end

	UpdateTeamRoleInfo();

	if TeamRoleCaptainSignButton:IsShown() then
		local teamInfo = TeamManager:getTeamInfo();
		local uv = PhotoFrameCaptionButtonGetUV( teamInfo.TeamBootyMode );
		TeamRoleCaptainSignButton:ChangeNormalTexture( uv.x,uv.y,uv.w,uv.h );
	end
end

t_teamRolePhotoEvents["GE_TARGET_BUF_LIST_UPDATE"].func =
function ()
	local mainplayer	= ActorMgr:getMainPlayer();
	local TargetObj		= ActorMgr:FindActor( mainplayer:GetSelectTarget() );
	if TargetObj == nil then
		return;
	end

	if TeamManager:isInTeamById( TargetObj:GetID() ) then
		TeamManager:updateActorStatus( TargetObj );
		UpdateTeamRoleInfo();
	end
end

t_teamRolePhotoEvents["GE_TARGET_RIDE_CHG"].func =
function ()
	local mainplayer	= ActorMgr:getMainPlayer();
	local TargetObj		= ActorMgr:FindActor( mainplayer:GetSelectTarget() );
	if TargetObj == nil then
		return;
	end

	if TeamManager:isInTeamById( TargetObj:GetID() ) then
		TeamManager:updateActorRideStatus( TargetObj );
		UpdateTeamRoleInfo();
	end
end

t_teamRolePhotoEvents["GE_UPDATE_CHATMSG"].func = 
function ()
	local msg = UIMSG:getUIMsg();
	-- 若不是队友发来的消息,则不接收
	if msg.ChatMsg.Type ~= CL_CHAT_TEAM then
		return;
	end
	local systemSetting = GameMgr:getUserSystemConfig();
	if systemSetting:getGameOptValue( GAME_OPT_SHOW_OTHERPLAYER_TALK_POP ) == 0 then
		return;
	end

	-- 先找到发送消息对应的头像
	local button;
	local richText;
	local richBack;
	for i = 1, MAX_TEAM_MEMBER do
		button = getglobal( "TeamRoleButton" .. i );
		if button:IsShown() and CompareRoleName( button:GetClientString(), msg.ChatMsg.Speaker ) then
			richBack = getglobal( button:GetName() .. "ChatPopBtn" );
			richText = getglobal( richBack:GetName() .. "Rich" );
			break;
		end
	end
	
	if richText == nil then return end
		
	local szContent = msg.ChatMsg.Content;
	teamPopManager[msg.ChatMsg.Speaker]			= {};
	teamPopManager[msg.ChatMsg.Speaker].text	= "";
	
	if string.find(szContent,"#@!",1,true) then
		--print( "组队发来的聊天信息带有#@!--内容是--"..szContent );
		while string.find(szContent,"#@!",1,true) do
			local nBeginPos		= string.find( szContent, "#@!" );
			local nEndPos		= string.find( szContent, "#", nBeginPos + 1, string.len(szContent));
			local context		= string.sub( szContent, nBeginPos + string.len( "#@!" ), nEndPos-1 );
			teamPopManager[msg.ChatMsg.Speaker].text = teamPopManager[msg.ChatMsg.Speaker].text .. context .."\n";
			szContent = string.sub( szContent, nEndPos + 1,string.len(szContent) );
		end
	else
		teamPopManager[msg.ChatMsg.Speaker].text = szContent;
	end

	szContent = teamPopManager[msg.ChatMsg.Speaker].text;
	if string.find( szContent, "#" ) then
		teamPopManager[msg.ChatMsg.Speaker].text = replaceFaceString( szContent );
		szContent = teamPopManager[msg.ChatMsg.Speaker].text;
	end

	teamPopManager[msg.ChatMsg.Speaker].nShowTime	= GameMgr:getTickTime();
	teamPopManager[msg.ChatMsg.Speaker].alpha		= 1;	

	print( "t_teamRolePhotoEvents--GE_UPDATE_CHATMSG--szContent = "..szContent.."--385" );
	AdjustTeamRichByText( { ["text"] = szContent, ["rich"] = richText:GetName(), ["btn"] = richBack:GetName() } );

	richBack:SetBackDropBlendAlpha( 1 );
	richText:SetAlpha( 1 );
	richBack:Show();
end

function AdjustTeamRichByText( data )
	local szText	= data["text"];
	local rich		= getglobal( data["rich"] );
	local frame		= getglobal( data["btn"] );
	rich:clearHistory();
	rich:resizeRichWidth( 150 * GetScreenScaleY() );
	rich:SetText( szText, 255, 255, 255 );

	local nWidth = 1;
	for i = 1, rich:GetTextLines() do
		local nLineWidth = rich:getLineWidth( i - 1 );
		if nWidth < nLineWidth then
			nWidth = nLineWidth;
		end
	end

	local nHeight = rich:GetTotalHeight();
	nHeight = ( nHeight == 0 ) and 1 or nHeight;
	rich:resizeRect( math.ceil( nWidth + 4 * GetScreenScaleY() ), math.ceil( nHeight + 4 * GetScreenScaleY() ) );

	rich:resizeRect( math.ceil( nWidth + 4 * GetScreenScaleY() ), math.ceil( nHeight + 4 * GetScreenScaleY() ) );
	frame:resizeRect( math.ceil( nWidth + 6 * GetScreenScaleY() ), math.ceil( nHeight + 6 * GetScreenScaleY() ) );
	frame:SetSize( math.ceil( (frame:GetRealWidth()+ 2) / GetScreenScaleY() ),  math.ceil( (frame:GetRealHeight() + 2 ) / GetScreenScaleY() ) );
	rich:SetSize( math.ceil( nWidth + 4 * GetScreenScaleY() ) / GetScreenScaleY(), math.ceil( nHeight + 4 * GetScreenScaleY() ) / GetScreenScaleY() );
	rich:SetText( szText, 255, 255, 255 );
end

t_teamRolePhotoEvents["GE_TEAM_CHANGE_BOOTY"].func = 
function ()
	if TeamRoleCaptainSignButton:IsShown() then
		local teamInfo = TeamManager:getTeamInfo();
		local uv = PhotoFrameCaptionButtonGetUV( teamInfo.TeamBootyMode );
		TeamRoleCaptainSignButton:ChangeNormalTexture( uv.x,uv.y,uv.w,uv.h );
	end
end

t_teamRolePhotoEvents["GE_TEAM_LEAVETEAM"].func =
function ()
	local frame = getglobal( "TeamRoleFrame" );
	frame:Hide();
	ClearTeamRoleButtonFrame();
	local t_TeamPopControl = GetTeamPopControl();
	clearAllMemberPop();
end

t_teamRolePhotoEvents["GE_BATTLE_TO_WORLD"].func =
function ()
	t_teamRolePhotoEvents["GE_TEAM_LEAVETEAM"].func();
end

local t_curSelMember = { name = "" };

function TeamRoleFrame_OnLoad()
	for event, _ in pairs( t_teamRolePhotoEvents ) do
		this:RegisterEvent( event );
	end

	t_teamRolePhotoEvents.__index = function ( tab, key )
								tab[key] = { func = function () end };
								return tab[key];
							end

	setmetatable( t_teamRolePhotoEvents, t_teamRolePhotoEvents );
end

function TeamRoleFrame_OnEvent()
	t_teamRolePhotoEvents[arg1].func();
end

function TeamRoleChildButton_OnClick()
	local roleBtn	= getglobal( this:GetParent() );
	t_curSelMember	= { name = roleBtn:GetClientString() };
	if arg1 == "RightButton" then
		ShowCharacterRightFrame( t_curSelMember["name"] );
	end
	
	-- TODO: 改成不在视野范围内，也能选中队员
	if arg1 == "LeftButton" then
		local mainplayer	= ActorMgr:getMainPlayer();
		local actorName		= roleBtn:GetClientString();
		local actor			= ActorMgr:findActorByName( actorName, GAT_PLAYER );

		if actor ~= nil then
			mainplayer:SetSelectTarget( actor:GetID() );
		else
			local nTeamIndex	= GetTeamIndex( actorName );
			local teamInfo		= TeamManager:getTeamInfo();
			local teamMember	= teamInfo.TeamMember[nTeamIndex-1];
			
			-- 若非下线状态
			
			if teamMember.OffLineTime ~= CLIENT_MEMBER_ONLINE then
				return;
			end

			mainplayer:SetSelectTarget( 0 );

			SetTargeInfo( { isTeam = true, index = nTeamIndex, name = teamMember.MemberName } );
		end
	end
end

function TeamRoleButton_OnClick()
	t_curSelMember = { name = this:GetClientString() };
	if arg1 == "RightButton" then
		ShowCharacterRightFrame( t_curSelMember["name"] );
		
		if GameTooltip:GetClientString() == this:GetName() then
			GameTooltip:SetPoint( "topleft", "FriendFrame_RightOptionFrame", "bottomleft", 2, 2 );
		end
	end
	
	if arg1 == "LeftButton" then
		local mainplayer	= ActorMgr:getMainPlayer();
		local actorName		= this:GetClientString();
		local actor			= ActorMgr:findActorByName( actorName, GAT_PLAYER );

		if actor == nil then
			local nTeamIndex	= GetTeamIndex( actorName );
			local teamInfo		= TeamManager:getTeamInfo();
			local teamMember	= teamInfo.TeamMember[nTeamIndex-1];
			-- 若下线状态
			if teamMember.OffLineTime ~= CLIENT_MEMBER_ONLINE then
				return;
			end

			mainplayer:SetSelectTarget( 0 );
			SetTargeInfo( { isTeam = true, index = nTeamIndex, name = teamMember.MemberName } );
		else
			mainplayer:SetSelectTarget( actor:GetID() );
		end
	end
end

function GetTeamIndex( actorName )
	local teamInfo		= TeamManager:getTeamInfo();
	
	for i = 1, teamInfo.MemberNum do
		local teamMember = teamInfo.TeamMember[i-1];
		if CompareRoleName( teamMember.MemberName, actorName ) then
			nTeamIndex = i;
			return i;
		end
	end

	return 0;
end

function SetTeamMemberJiaPetBuffer( teamMember, data )
	local ownPets			= teamMember.PetView;
	local btn				= data["btn"];
	local nGoodBufBtnCount	= data["goodBuffCount"];

	local szText = ""
	for i = 1, ownPets.PetNum do
		if ownPets.PetView[i-1].Passive ~= PET_FIGHT_ACTIVE then
			return nGoodBufBtnCount;
		end
		for j=1, ownPets.PetView[i-1].DiathesisNum do		
			local nId = ownPets.PetView[i-1].Diathesis[j-1].ID;
			local nLv = ownPets.PetView[i-1].Diathesis[j-1].Level;
			local diathesisDef = SkillMgr:getDiathesisDef( nId, nLv );
			if diathesisDef ~= nil and diathesisDef.IconPosition == 1 then
				szText = szText .. string.gsub(diathesisDef.Name,"（被动技能）","：") .. diathesisDef.DiathesisDesc .. "#n\n";
			end
		end
	end
	if szText ~= "" then
		szText = "#cff6e1c宠物技能：#n\n" ..szText
		AddMainPetBuf( btn:GetName().."Buff"..nGoodBufBtnCount, szText );
		nGoodBufBtnCount = nGoodBufBtnCount + 1;
	end
	return nGoodBufBtnCount;
end

-- 队伍成员头像的buff只用作是否显示，无需做成转动效果
function SetTeamPhotoJiaFairyBuffer( data )
	local nGoodBufBtnCount	= data["goodBuffCount"];
	local campFairyId		= data["fairyID"];
	local campFairyLv		= data["fairyLv"];
	print( "campFairyId = "..campFairyId.."，  campFairyLv = "..campFairyLv );
	if campFairyId == 0 then
		return nGoodBufBtnCount;
	end
	
	local MainPlayer	= ActorMgr:getMainPlayer();
	local GameTrump		= MainPlayer:getTrump();
	local TrumpDef		= GameTrump:getFairyDef( campFairyId, campFairyLv );
	
	for i = 1, FAIRY_MAX_SKILL do
		local trumpSkillInfo = TrumpDef.FairySkillList[i-1];
		if trumpSkillInfo.SkillID ~= 0 and trumpSkillInfo.SkillType == DIATHESIS_TYPE then
			local nId = trumpSkillInfo.SkillID;
			local nLv = trumpSkillInfo.SkillLevel;
			local diathesisDef = SkillMgr:getDiathesisDef( trumpSkillInfo.SkillID, trumpSkillInfo.SkillLevel );
			if diathesisDef.IconPosition == 1 then
				AddCoolFairyBuf( data["btn"].."Buff"..nGoodBufBtnCount, nId, nLv, 0 );
				nGoodBufBtnCount = nGoodBufBtnCount + 1;
			end
		end
	end

	return nGoodBufBtnCount;
end

function UpdateTeamPlayerBuf( teamMember, btn )
	ClearTeamBuff( btn );

	-- 先判断当前处理的队友下线的情况
	if teamMember.OffLineTime ~= CLIENT_MEMBER_ONLINE then
		return;
	end
	
	local nGoodBtnCount	= 1;
	local status = teamMember.StatusInfo;
	for i = 1, status.GoodStatusNum do
		if nGoodBtnCount > MAX_TEAM_GOOD_BUFF then
			break;
		end
		
		local goodStatus = status.GoodStatusList[i-1];
		if isCltShowBuff( goodStatus.StatusID, goodStatus.Level ) then
			AddTeamBuffer( { btn = btn:GetName().."Buff"..nGoodBtnCount, id = goodStatus.StatusID, level = goodStatus.Level, targetId = 0, 
							name = teamMember.MemberName } );
			nGoodBtnCount = nGoodBtnCount + 1;
		end
	end

	if teamMember.RideID ~= 0 and nGoodBtnCount <= MAX_TEAM_GOOD_BUFF then
		AddCoolRideBuf( btn:GetName().."Buff"..nGoodBtnCount, teamMember.RideID, teamMember.RideLevel, 0 );
		nGoodBtnCount = nGoodBtnCount + 1;
	end

	if teamMember.FairyID ~= 0 and nGoodBtnCount <= MAX_TEAM_GOOD_BUFF then
		--print( "fairyID = "..teamMember.FairyID.."，  fairyLv = "..teamMember.FairyLevel );
		nGoodBtnCount = SetTeamPhotoJiaFairyBuffer( { goodBuffCount = nGoodBtnCount, fairyID = teamMember.FairyID, 
														fairyLv = teamMember.FairyLevel, btn = btn:GetName() } );
	end
	
	nGoodBtnCount = SetTeamMemberJiaPetBuffer( teamMember, { btn = btn, goodBuffCount = nGoodBtnCount } );

	for i = 1, status.NoLimitStatusNum do
		if nGoodBtnCount > MAX_TEAM_GOOD_BUFF then
			break;
		end
		
		local nolimitStatus = status.NolimitStatusList[i-1];

		if isCltShowBuff( nolimitStatus.StatusID, nolimitStatus.Level ) then
			AddTeamBuffer( { btn = btn:GetName().."Buff"..nGoodBtnCount, id = nolimitStatus.StatusID, level = nolimitStatus.Level, targetId = 0, 
							name = teamMember.MemberName } );
			nGoodBtnCount = nGoodBtnCount + 1;
		end
	end

	-- 再设置DEBUFF 
	local nBadBtnCount	= 1;
	for i = 1, status.BadStatusNum do
		if nBadBtnCount > MAX_TEAM_DEBUFF then
			break;
		end
		
		local badStatus = status.BadStatusList[i-1];
		if isCltShowBuff( badStatus.StatusID, badStatus.Level ) then
			AddTeamBuffer( { btn = btn:GetName().."Debuff"..nBadBtnCount, id = badStatus.StatusID, level = badStatus.Level, targetId = 0,
							name = teamMember.MemberName } );
			nBadBtnCount = nBadBtnCount + 1;
		end
	end
end

--清空队伍角色列表
function  ClearTeamRoleButtonFrame()
	for i = 1, MAX_TEAM_MEMBER do
		local btn = getglobal("TeamRoleButton"..i);
		btn:Hide();
	end
end

function SetTeamRoleBtn( teamMember, btn )
	local mainplayer = ActorMgr:getMainPlayer();
	local captainBtn = getglobal( "TeamRoleCaptainSignButton" );
	local appealBtn = getglobal( "TeamRoleAppealFollowSignButton" );
	local cancelBtn = getglobal( "TeamRoleCancelFollowSignButton" );
	-- 若要设置的是主角，则不设置队伍头像
	if CompareRoleName( mainplayer:getName(), teamMember.MemberName ) then
		-- 显示队长标志
		if mainplayer:isCaptain() then
			-- TODO
			if captainBtn:IsShown() then
				captainBtn:Hide();
			end
			if appealBtn:IsShown() then
				appealBtn:Hide();
			end
			if cancelBtn:IsShown() then
				cancelBtn:Hide();
			end
		end
		return false;
	end	
	if teamMember.Position == TEAM_POSITION_CAPTAIN then
		captainBtn:SetPoint( "Bottomleft", btn:GetName(), "topleft", 10, 1 );
		captainBtn:Show();
		if TeamManager:isInTeamFollow() then
			appealBtn:Hide();
			cancelBtn:SetPoint( "Bottomleft", btn:GetName(), "topleft", 30, 10 );
			cancelBtn:Show();
			TeamRoleFrameFollowXinShouBtn:Hide();
		else	
			cancelBtn:Hide();
			appealBtn:SetPoint( "Bottomleft", btn:GetName(), "topleft", 30, 10 );
			appealBtn:Show();			
		end		
	end

	btn:SetClientUserData( 3, teamMember.Position ); --设置队长标志
	btn:SetClientString( teamMember.MemberName );

	local followSign = getglobal( btn:GetName().."_FollowSign" );
	if teamMember.Follow ~= 0 then
		followSign:Show();
	else
		followSign:Hide();
	end

	local offlineSignTexture = getglobal( btn:GetName().."_OfflineSign_Texture" );
	-- 若队友下线
	if teamMember.OffLineTime ~= CLIENT_MEMBER_ONLINE then
		offlineSignTexture:Show();
	else
		offlineSignTexture:Hide();
	end

	btn:SetClientUserData( 2, btn:GetClientID() );
	SetPhoto( teamMember.Hair, teamMember.Face, btn:GetName().."TeamRoleHairPhoto",
				btn:GetName().."TeamRoleTexture", teamMember.Gender, teamMember.Head, btn:GetName().."TeamRoleHairShadowTex", 
				btn:GetName().."TeamRoleBodyTex", "uires\\TuPianLei\\TouXiang\\JueSeTouXiang\\ZuDuiTouXiang  MASK.tga" );
	
	local t_photoTextures = {	btn:GetName().."TeamRoleHairPhoto", btn:GetName().."TeamRoleTexture", btn:GetName().."TeamRoleHairShadowTex", 
								btn:GetName().."TeamRoleBodyTex"
							};
	
	local selfMemberInfo	= TeamManager:getTeamMemberInfo( mainplayer:getName() );
	local bGray		= teamMember.MapID ~= selfMemberInfo.MapID;
	for _, name in ipairs( t_photoTextures ) do
		local tex = getglobal( name );
		tex:SetGray( bGray );
	end

	local t_fonts = {	[btn:GetName().."_Name"] = { value = teamMember.MemberName },
						[btn:GetName().."_LVFont"] = { value = teamMember.Level }, };
	util.SetFonts( t_fonts );
	local texture = getglobal( btn:GetName().."TeamRoleTexture" );
	SetTeamPopRich( btn );
	local nIconIndex = btn:GetClientID();
	
	local hpTex = getglobal( btn:GetName().."_HPTex" );
	local scale = teamMember.HP / teamMember.MaxHP;
	local hp = ( teamMember.HP > teamMember.MaxHP ) and teamMember.MaxHP or  teamMember.HP;
	scale = scale > 1 and 1 or scale;
	hpTex:SetSize( scale * 72, 9 );

	local mpTex = getglobal( btn:GetName().."_MPTex" );
	local mp = ( teamMember.MP > teamMember.MaxMP ) and teamMember.MaxMP or  teamMember.MP;
	scale = teamMember.MP / teamMember.MaxMP;
	scale = scale > 1 and 1 or scale;
	mpTex:SetSize( scale * 72, 9 );
	
	local t_fonts = {	[btn:GetName().."_MPFont"] = { value = mp.." / "..teamMember.MaxMP },
						[btn:GetName().."_HPFont"] = { value = hp.." / "..teamMember.MaxHP },};
	util.SetFonts( t_fonts );
	
	local t_textures = 
		{
			[btn:GetName() .. "RaceSignTexture"] = {	path = t_raceTexture[teamMember.Career].path, 
														UV = t_raceTexture[teamMember.Career].UV },
		};
	local teamRaceSignTexture	=	getglobal(btn:GetName() .. "RaceSignTexture");
	if teamMember.Career > 5 then
		teamRaceSignTexture:SetSize(23,26);
	else
		teamRaceSignTexture:SetSize(20,28);	
	end
	util.SetTextures( t_textures );

	if not btn:IsShown() then
		btn:Show();
	end

	-- 同时设置是否显示buff
	UpdateTeamPlayerBuf( teamMember, btn )
	return true;
end

function UpdateTeamRoleInfo()
	ClearTeamRoleButtonFrame();
	local teamInfo = TeamManager:getTeamInfo();--获取队伍信息
	local nIconIndex = 1;
	for i = 1, teamInfo.MemberNum do
		if SetTeamRoleBtn( teamInfo.TeamMember[i-1], getglobal( "TeamRoleButton" .. nIconIndex ) ) then
			nIconIndex = nIconIndex + 1;
		end
	end
end

function TeamHP_OnEnter()
	local teamRoleBtn	= getglobal( this:GetParent() );
	local teamMember	= getTeamMemberByName( teamRoleBtn:GetClientString() );
	local mp = ( teamMember.MP > teamMember.MaxMP ) and teamMember.MaxMP or  teamMember.MP;
	local hp = ( teamMember.HP > teamMember.MaxHP ) and teamMember.MaxHP or  teamMember.HP;
	local szMPText		= "魔法值：" .. mp .. " / " .. teamMember.MaxMP;
	local szHPText		= "生命值："..hp .. " / " .. teamMember.MaxHP;

	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szHPText.."\n"..szMPText, frame = this:GetParent(),
						button = this:GetName() } );
end

function TeamMP_OnEnter()
	local teamRoleBtn	= getglobal( this:GetParent() );
	local teamMember	= getTeamMemberByName( teamRoleBtn:GetClientString() );
	local mp = ( teamMember.MP > teamMember.MaxMP ) and teamMember.MaxMP or  teamMember.MP;
	local hp = ( teamMember.HP > teamMember.MaxHP ) and teamMember.MaxHP or  teamMember.HP;
	local szMPText		= "魔法值：" .. mp .. " / " .. teamMember.MaxMP;
	local szHPText		= "生命值："..hp .. " / " .. teamMember.MaxHP;

	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szHPText.."\n"..szMPText, frame = this:GetParent(),
						button = this:GetName() } );
end

function ClearTeamBuff( btn )
	for i = 1, MAX_TEAM_GOOD_BUFF do
		local bufButton	= getglobal( btn:GetName().."Buff"..i );
		bufButton:SetClientUserData( 0, 0 );
		bufButton:SetClientUserData( 1, 0 );
		bufButton:Hide();
	end

	for i = 1, MAX_TEAM_DEBUFF do
		local bufButton	= getglobal( btn:GetName().."Debuff"..i );
		bufButton:SetClientUserData( 0, 0 );
		bufButton:SetClientUserData( 1, 0 );
		bufButton:Hide();
	end
end

function getTeamMemberByName( szName )
	local teamInfo = TeamManager:getTeamInfo();
	for i = 1, teamInfo.MemberNum do
		if CompareRoleName( teamInfo.TeamMember[i-1].MemberName, szName ) then
			return teamInfo.TeamMember[i-1];
		end
	end

	return nil;
end

function TeamRoleFrameChatPop_OnHide()
	if this ~= nil and isPointInFrame( this:GetName() ) then
		this:setCursorNormal();
	end
end

local t_TeamRoleFrameFollowXinShouBtnControl = { ["startTime"] = 0, ["isShowGuide"] = false, 
										MAX_SHOW_FOLLOW_XIN_SHOU_TI_SHI = 8000, ["opFrame"] = "TeamRoleFrameFollowXinShouBtn" };
function GetTeamRoleFrameFollowXinShouBtnControl()
	return t_TeamRoleFrameFollowXinShouBtnControl;
end

t_TeamRoleFrameFollowXinShouBtnControl["startNewGiftGuide"] =
function ( self )
	self["startTime"]   = GameMgr:getTickTime();
	self["isShowGuide"] = true;
end

t_TeamRoleFrameFollowXinShouBtnControl["endNewGiftGuide"] =
function ( self )
	self["startTime"]	= 0;
	self["isShowGuide"] = false;
end

t_TeamRoleFrameFollowXinShouBtnControl["update"] =
function ( self )
	if not self["isShowGuide"] then
		return;
	end
	
	local nCurTime = GameMgr:getTickTime();
	if nCurTime - self["startTime"] >= self["MAX_SHOW_FOLLOW_XIN_SHOU_TI_SHI"] then
		self:endNewGiftGuide();
	end
end

t_TeamRoleFrameFollowXinShouBtnControl["isShowGiftGuide"] =
function ( self )
	return self["isShowGuide"];
end

function TeamRoleFrameFollowXinShouBtn_OnShow()
	t_TeamRoleFrameFollowXinShouBtnControl:startNewGiftGuide();
end

function TeamRoleFrameFollowXinShouBtn_OnUpdate()
	t_TeamRoleFrameFollowXinShouBtnControl:update();
	if not t_TeamRoleFrameFollowXinShouBtnControl:isShowGiftGuide() then
		TeamRoleFrameFollowXinShouBtn:Hide();
	end
end

function TeamRoleFrameFollowXinShouBtn_OnHide()
	GameWizard:doStep( GAMEWIZARD_FIRST_IN_TEAM );
end