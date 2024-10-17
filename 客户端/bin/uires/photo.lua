g_nMainPlayerYes = 4;
g_nMainPlayerNo  = 3;

-- 主角析构调用函数
MAX_TARGET_GOOD_BUF_COUNT	= 16;	
MAX_TARGET_DEBUF_COUNT		= 8;

local szPkName = "";

local t_monsterPhysicTypeTex ={
								[0] = { path = "uires\\ui\\mask\\TouMingTongDao.tga", UV = { x = 0, y = 0, width = 22, height = 22 } },
								[1] = { path = "uires\\ZhuJieMian\\2.dds", UV = { x = 714, y = 492, width = 22, height = 22 } },
								[2] = { path = "uires\\ZhuJieMian\\2.dds", UV = { x = 691, y = 492, width = 22, height = 22 } },
								[3] = { path = "uires\\ZhuJieMian\\2.dds", UV = { x = 737, y = 492, width = 22, height = 22 } },
							};

local t_monsterPinzhiTex ={
							[0] = { x = 28, y = 0, width = 28, height = 16, text = "普通" } ,
							-- 软
							[1] = { x = 28, y = 0, width = 28, height = 16, text = "普通" } ,
							-- 弱
							[2] = { x = 28, y = 0, width = 28, height = 16, text = "普通" },	
							-- 普
							[3] = { x = 56, y = 0, width = 28, height = 16, text = "精英" },
							[4] = { x = 84, y = 0, width = 28, height = 16, text = "BOSS" },	
							[5] = { x = 84, y = 0, width = 28, height = 16, text = "BOSS" },
							-- 精
							[6] = { x = 56, y = 0, width = 28, height = 16, text = "精英" },	
							-- BOSS
							[7] = { x = 84, y = 0, width = 28, height = 16, text = "BOSS" },
							-- 机械
							[8] = { x = 112, y = 0, width = 28, height = 16, text = "机械" },
							[13] = { x = 112, y = 0, width = 28, height = 16, text = "机械" },
							[14] = { x = 112, y = 0, width = 28, height = 16, text = "机械" },
						};

local MAX_BUF_PER_LINE = 8;

local t_PhotoFrameEvents =	{	["GE_ON_ACTOR_ENTER_WORLD"] = {}, ["GE_ACTOR_ATTRIBUTE_CHG"] = {}, ["GE_TEAM_APPLYSUCC"] = {}, 
								["GE_TEAM_LEAVETEAM"] = {},		["GE_TEAM_GET_TEAMINFO"] = {},		["UI_ACTOR_LEVEL_CHG"] = {},
								["GE_ENTER_PLAYERLOGIN"] = {},	["GE_MAIN_JOIN_TEAM_SUCC"] = {},	["GE_MAINPLAYER_CHG_NAME"] = {},
								["GE_TEAM_CHANGE_BOOTY"] = {},	["GE_BATTLE_TO_WORLD"] = {},	["GE_ACTOR_RIDE_SIDE"] = {},
							["GE_MAINPLAYER_DEAD"] = {}, ["GE_MAIN_RIDE_ON"] = {}, ["GE_TMPITME_TMPMACHIN"] = {}, ["GE_MAIN_AUTO_FLY"] = {}, };

function PhotoFrameOnLoad()
	for event, _ in pairs( t_PhotoFrameEvents ) do
		this:RegisterEvent( event );
	end

	t_PhotoFrameEvents.__index = function ( tab, key )
								tab[key] = { func = function () end };
								return tab[key];
							end

	setmetatable( t_PhotoFrameEvents, t_PhotoFrameEvents );
	GiftBtn:setUpdateTime( 3 );	
	PhotoFrameVipBtn:setUpdateTime(1);
end

local t_photoFrameRaceTexture = 
-- 根据职业来设置职业图片
{
	-- 战士
	[g_nCAREER_WARRIOR]			= { path = "uires\\ZhuJieMian\\2.dds", UV = { x = 684, y = 516, width = 20, height = 28 } },
	-- 战士单手  守护者
	[g_nCAREER_WARRIOR_SINGEL]	= { path = "uires\\ZhuJieMian\\3.dds", UV = { x = 490, y = 189, width = 23, height = 26 } },
	-- 战士双手  征服者
	[g_nCAREER_WARRIOR_DOUBLE]	= { path = "uires\\ZhuJieMian\\3.dds", UV = { x = 466, y = 189, width = 23, height = 26 } },

	-- 法师
	[g_nCAREER_MAGICOR]			= { path = "uires\\ZhuJieMian\\2.dds", UV = { x = 352, y = 578, width = 20, height = 28 } },
	-- 法师单手  聚能者
	[g_nCAREER_MAGICOR_SINGEL]	= { path = "uires\\ZhuJieMian\\3.dds", UV = { x = 514, y = 216, width = 23, height = 26 } },
	-- 法师双手  炼魂师
	[g_nCAREER_MAGICOR_DOUBLE]	= { path = "uires\\ZhuJieMian\\3.dds", UV = { x = 538, y = 216, width = 23, height = 26 } },

	-- 枪手
	[g_nCAREER_GUNER]			= { path = "uires\\ZhuJieMian\\2.dds", UV = { x = 705, y = 516, width = 20, height = 28 } },
	-- 枪手单手  猎杀者
	[g_nCAREER_GUNER_SINGEL]	= { path = "uires\\ZhuJieMian\\3.dds", UV = { x = 466, y = 216, width = 23, height = 26 } },
	-- 枪手双手	毁灭者
	[g_nCAREER_GUNER_DOUBLE]	= { path = "uires\\ZhuJieMian\\3.dds", UV = { x = 490, y = 216, width = 23, height = 26 } },

	-- 祭师
	[g_nCAREER_CHURCH]			= { path = "uires\\ZhuJieMian\\2.dds", UV = { x = 663, y = 520, width = 20, height = 28 } },
	-- 祭司单手	圣佑者
	[g_nCAREER_CHURCH_SINGEL]	= { path = "uires\\ZhuJieMian\\3.dds", UV = { x = 538, y = 189, width = 23, height = 26 } },
	-- 祭司双手	驭雷师
	[g_nCAREER_CHURCH_DOUBLE]	= { path = "uires\\ZhuJieMian\\3.dds", UV = { x = 514, y = 189, width = 23, height = 26 } },
};

function PhotoFrame_OnEvent()
	local mainplayer = ActorMgr:getMainPlayer();

	--显示玩家等级与名字逻辑
	local szUserName = mainplayer:getName();
	if (szUserName ~= nill) then
	  CharacterPhotoName:SetText(szUserName);
	end
	local nLevel   = mainplayer:getLv();
	if (nLevel ~= 0) then
	  CharacterLevel:SetText(nLevel);
	end

	local t_textures = 
		{
			["PhotoFrameRaceSignTexture"] = {	path	= t_photoFrameRaceTexture[mainplayer:getRace()].path, 
												UV		= t_photoFrameRaceTexture[mainplayer:getRace()].UV 
											},
		};
	if mainplayer:getRace() > 5 then
		PhotoFrameRaceSignTexture:SetSize(23,26);
	else
		PhotoFrameRaceSignTexture:SetSize(20,28);	
	end
	util.SetTextures( t_textures );
	
	
	SetPhoto( mainplayer:getHair(), mainplayer:getFace(), "hairPhoto", "Photo", mainplayer:getGender() , mainplayer:getHead(),
				"PhotoHairShadowTex", "PhotoBodyTex" );

	if arg1 == "GE_TEAM_GET_TEAMINFO" then
		if not PhotoFrameCaptainButton:IsShown() then			
			if mainplayer:isCaptain() then
				PhotoFrameCaptainButton:Show();				
			end
		end
		if not MainPlayerLeaveTeamBtn:IsShown() then
			MainPlayerLeaveTeamBtn:Show();
		end
		PhotoFrameFollowSignButton:Hide();
		PhotoFrameAppealFollowSignButton:Hide();
		PhotoFrameCancelFollowSignButton:Hide();
		if mainplayer:isCaptain() then			
			local teamInfo = TeamManager:getTeamInfo();
			local uv = PhotoFrameCaptionButtonGetUV( teamInfo.TeamBootyMode );
			PhotoFrameCaptainButton:ChangeNormalTexture( uv.x,uv.y,uv.w,uv.h );				

			if isAnyTeamMemberInFollow() then
				if not PhotoFrameCancelFollowSignButton:IsShown() then
					PhotoFrameCancelFollowSignButton:Show();
					TeamRoleFrameFollowXinShouBtn:Hide();
				end
			else
				if not PhotoFrameAppealFollowSignButton:IsShown() then
					PhotoFrameAppealFollowSignButton:Show();
				end
			end			
		else
			PhotoFrameCaptainButton:Hide()

			if TeamManager:isInTeamFollow() then
				PhotoFrameFollowSignButton:Show();							
			else
				PhotoFrameFollowSignButton:Hide();
			end			
		end			
	end
	
	if arg1 == "GE_TEAM_APPLYSUCC" then
		MainPlayerLeaveTeamBtn:Show( );
		if mainplayer:isCaptain() then
			PhotoFrameCaptainButton:Show();
			PhotoFrameAppealFollowSignButton:Show();
			PhotoFrameCancelFollowSignButton:Hide();
			PhotoFrameFollowSignButton:Hide();
			TeamRoleCaptainSignButton:Hide();
		else
			PhotoFrameAppealFollowSignButton:Hide();
			PhotoFrameCancelFollowSignButton:Hide();
			PhotoFrameFollowSignButton:Hide();
			PhotoFrameCaptainButton:Hide();
		end
	end

	if arg1 == "GE_TEAM_LEAVETEAM" or arg1 == "GE_BATTLE_TO_WORLD" then
		MainPlayerLeaveTeamBtn:Hide();
		PhotoFrameCaptainButton:Hide();
		PhotoFrameAppealFollowSignButton:Hide();
		PhotoFrameCancelFollowSignButton:Hide();
		PhotoFrameFollowSignButton:Hide();
		TroopInviteTeamFollowFrame:Hide();
	end

	if arg1 == "GE_ENTER_PLAYERLOGIN" then
		this:EndMoveFrame();
		this:SetPointEx( "topleft", "UIClient", "topleft", 0, 0 );
		PhotoFrameFollowSignButton:Hide();
		PhotoFrameAppealFollowSignButton:Hide();
		PhotoFrameCancelFollowSignButton:Hide();
		setTeamFollowBegin();
	end

	if arg1 == "GE_MAINPLAYER_CHG_NAME" then
		local mainplayer = ActorMgr:getMainPlayer();
		CharacterPhotoName:SetText( mainplayer:getName() );
	end

	if arg1 == "GE_TEAM_CHANGE_BOOTY" then
		if mainplayer:isCaptain() then
			local teamInfo = TeamManager:getTeamInfo();
			local uv = PhotoFrameCaptionButtonGetUV( teamInfo.TeamBootyMode );
			PhotoFrameCaptainButton:ChangeNormalTexture( uv.x,uv.y,uv.w,uv.h );
		end
	end

	if arg1 == "GE_MAINPLAYER_DEAD" then
		if mainplayer:isInTeam() and ( TeamManager:isInTeamFollow() or isAnyTeamMemberInFollow() ) then
			TeamManager:Team_OP_CancelTeamFollow();
		end
	end

	if arg1 == "GE_ACTOR_RIDE_SIDE" then
		if mainplayer:isInRide() and TeamManager:isInTeamFollow() then
			local pActor = ActorMgr:findActorByName( getCaptain().MemberName );
			if pActor ~= nil and pActor:isInRide() then
				local rider = pActor:getRider();
				if rider:getRideItem().m_nOwnerID == mainplayer:GetID() then
					TeamManager:Team_OP_CancelTeamFollow();	
				end
			end
		end
	end

	if arg1 == "GE_MAIN_RIDE_ON" then 
		if mainplayer:isInRide() and TeamManager:isInTeamFollow() then
			local rider = mainplayer:getRider();
			if not rider:isOwnRide() then
				TeamManager:Team_OP_CancelTeamFollow();	
			end
			local pActor = ActorMgr:findActorByName( getCaptain().MemberName );
			if pActor ~= nil and pActor:isInRide() then
				local rider = pActor:getRider();
				if rider:getRideItem().m_nOwnerID == mainplayer:GetID() then
					TeamManager:Team_OP_CancelTeamFollow();	
				end
			end
		end
	end

	if arg1 == "GE_TMPITME_TMPMACHIN" then
		if mainplayer and TeamManager:isInTeamFollow() then
			TeamManager:Team_OP_CancelTeamFollow();
		end
	end

	if arg1 == "GE_MAIN_JOIN_TEAM_SUCC" then
		if not GameWizard:isDo( GAMEWIZARD_FIRST_IN_TEAM ) then
		local xinShouBtn = getglobal( "PhotoFrameFollowXinShouBtn" );
			if mainplayer:isCaptain() and not xinShouBtn:IsShown() then
				local tex = getglobal( "PhotoFrameAppealFollowSignButtonUVAnimationTex" );				
				local rich = getglobal( xinShouBtn:GetName().."Rich" );
				tex:SetUVAnimation( 30, true);
				local szText = "#c554623点击此图标即可要求队员进入#c0000ff组队跟随状态#c554623，再次点击即可取消#c0000ff组队跟随状态";
				local nSelfDefineExtendWidth = 10;
				rich:SetClientString( szText );
				ReSizeTooltip( rich:GetName(), xinShouBtn:GetName(), nSelfDefineExtendWidth );			
				xinShouBtn:Show();
				local closeBtn = getglobal( xinShouBtn:GetName().."_CloseBtn" );
				closeBtn:SetPointEx( "topright", "PhotoFrameFollowXinShouBtnGuideBtn", "topright", -2, 0 );
			end
		end	
		
		if mainplayer:isCaptain() and GameFollowManager:isWeiXing() then
			local szName = GameFollowManager:weiXingTarget();
			local teamInfo = TeamManager:getTeamInfo();
			for i = 1, teamInfo.MemberNum do
				local teamMember = teamInfo.TeamMember[i-1];
				if teamMember.MemberName == szName then
					GameFollowManager:cancelWeiXing();
					break;
				end
			end			
		end
	end

	if arg1 == "GE_MAIN_AUTO_FLY" then
		if mainplayer:isInTeam() and ( TeamManager:isInTeamFollow() or isAnyTeamMemberInFollow() ) then
			TeamManager:Team_OP_CancelTeamFollow();
		end
	end
end

function MainPlayerLeaveTeamBtn_OnClick()
	--TeamManager:Team_OP_Exit();
	local frame = getglobal( "ConfirmLeaveTeamFrame" );
	if frame:IsShown() then
		return;
	end

	frame:Show();
end

-- 显示buf的信息
function CoolBuffButton_OnEnter()
	local nID		= this:GetClientUserData( 0 );
	local nLv		= this:GetClientUserData( 1 );
	local nOwnerId	= this:GetClientUserData( 3 );
	local nType		= this:GetClientUserData( 2 );

	if nType == BUFF_TYPE_RIDE then
		local nLevel		= nLv;
		local targetActor	= ActorMgr:FindActor( nOwnerId );
		if targetActor ~= nil and targetActor:getType() == GAT_MAINPLAYER then
			local rider			= targetActor:getRider( );
			local rideInfo		= rider:getRideInfoById( nID  );
			nLevel = rideInfo.nLevel;
		end
		
		ShowRideBuffTips( { rideId = nID, level = nLevel, buffButtonName = this:GetName(), ownerId = nOwnerId, parent = this:GetParent() } );
	elseif nType == BUFF_TYPE_PET_GUAN_ZHI then
		ShowPetGuanZhiTips( { id = nID, level = nLv, ownerId = nOwnerId, button = this:GetName(), parent = this:GetParent() } );
	elseif nType == BUFF_TYPE_PET_HETI then
		ShowPetHetiBuffTips({index = nID, owner = nOwnerId, button = this:GetName(), parent = this:GetParent()})
	elseif nType == BUFF_TYPE_FAIRY then
		ShowPetDiathesisTips( { id = nID, level = nLv, ownerId = nOwnerId, button = this:GetName(), parent = this:GetParent() } );
	elseif  nType == BUFF_TYPE_PET then
		ShowGameTooltip( { rich = "WideGameTooltipRichText", tipsFrame = "WideGameTooltip", text = this:GetClientString(), 
							frame = this:GetName(), button = this:GetName() } );
		SetGameTooltipRelFrame( { this:GetName(), this:GetParent() } );
	else
		local nOverlay = 0;
		if nOwnerId == 0 then
			local memberName	= this:GetClientString();
			local bufInfo		= TeamManager:getOneBuf( memberName, nID, nLv );
			if bufInfo ~= nil then
				nOverlay = bufInfo.Overlay;
			end
		else
			local targetActor = ActorMgr:FindActor( nOwnerId );
			local BufStatus	= targetActor:getBufStatus();
			local buf		= BufStatus:getBufStatus( nID, nLv );
			nOverlay		= buf:getOverlay();
		end

		ShowBuffTips( { statusId = this:GetClientUserData( 0 ), level = this:GetClientUserData( 1 ),
					totalTime = this:GetCoolTotal(), startTime = this:GetCoolStart(), buffButtonName = this:GetName(),
					ownerId = this:GetClientUserData( 3 ), button = this:GetName(), parent = this:GetParent(),
					overlay = nOverlay } );
	end
end

function CoolBuffButton_OnLeave()
	GameTooltip:Hide();
	WideGameTooltip:Hide();
end

function CoolBuffButton_OnClick()
	if arg1 == "LeftButton" then
		return;
	end
	local nType		= this:GetClientUserData( 2 );
	if nType == BUFF_TYPE_PET or nType == BUFF_TYPE_PET_HETI or nType == BUFF_TYPE_FAIRY or nType == BUFF_TYPE_PET_GUAN_ZHI then
		return;
	end

	local mainplayer = ActorMgr:getMainPlayer();
	local nPlayerID = this:GetClientUserData( 3 );
	-- 若所点buf不是主角的buf，则不能取消buf
	if nPlayerID ~= mainplayer:GetID() then
		return;
	end
	
	local nId = this:GetClientUserData( 0 );
	local nLv = this:GetClientUserData( 1 );
	if getRideDef( nId, nLv ) ~= nil then
		CancelSpecialBuffer( { rideId = nId, level = nLv } );
	else
		CancelBuffer( { statusId = nId, level = nLv } );
	end
	
end

function CoolBuffButton_OnUpdate()
end

nAcceptTime = 0;
local timeTick = 0;
local REPEATTIME = 900;

function setTimeTick()
	timeTick = os.clock();
end

function PhotoFrame_OnUpdate()
	local mainplayer = ActorMgr:getMainPlayer();
	if ( mainplayer == nil ) then return end
	if mainplayer:isInFight() then
		PhotoFightTex:Show();
		if not util.isInFadeState( "PhotoFightTex" ) then
			util.UIFrameToFadeIn( "PhotoFightTex", 0, 1, 1000, 100 );
			util.UIFrameToFadeKeep( "PhotoFightTex", 1, 1000 );
			util.UIFrameToFadeOut( "PhotoFightTex", 1, 0, 1000, 100 );
		end
	else
		PhotoFightTex:Hide();
	end		
	if nAcceptTime > 0 and os.clock() - nAcceptTime >= 30 then
		nAcceptTime = 0;
		GameActorPVP:requestSafePKRes( 1,szPkName );
		if PKMessageBoxFrame:IsShown() then
			PKMessageBoxFrame:Hide();
		end
	end
	
	if isNeedRegister() then		
		if not ContinueRegisterUserFrame:IsShown() and os.clock() - timeTick > REPEATTIME then
			ContinueRegisterUserFrame:Show();
		end
	end
end

local MAX_MAIN_RIGHT_OPTION_BTNS = 3;

function HideRightPhotoFrameAllButton()
	for i = 1, MAX_MAIN_RIGHT_OPTION_BTNS do
		local btn = getglobal( "MainPlayerRightBtn" .. i );
		btn:Hide();
	end
end

local t_mainRightOption =	{
["离开队伍"] = { func = 
function ()
	TeamManager:Team_OP_Exit();
	MainPlayerRightMenu:Hide();
end
				},
["分配方式"] = { func =
function ()
	ShowTeamBootyMenu( this:GetName() );	
end
				},

["邀请全队骑乘"] = { func = 
function ()
	-- 首先遍历队伍成员，若在视野范围和指定的距离内，则邀请他们共同骑乘
	MainPlayerRightMenu:Hide();
	local teamInfo = TeamManager:getTeamInfo();
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then
		return;
	end
	local t_cannotInvitePlayers = {};
	local canInviteRide = 
			function ( szName )
				local actor = ActorMgr:findActorByName( szName );
				if actor == nil or actor == ActorMgr:getMainPlayer() then
					if actor ~= ActorMgr:getMainPlayer() then
						table.insert( t_cannotInvitePlayers, szName );
					end
					
					return false;
				end
				
				local mainplayer = ActorMgr:getMainPlayer();
				local dist = util.CalActorsDistance( actor, mainplayer );
				if dist > 5 ^ 2 then
					if actor ~= ActorMgr:getMainPlayer() then
						table.insert( t_cannotInvitePlayers, szName );
					end
					return false;
				end

				return true;
			end

	local rider	= mainplayer:getRider();
	for i = 1, teamInfo.MemberNum do
		local teamMember = teamInfo.TeamMember[i-1];
		local actor = ActorMgr:findActorByName( teamMember.MemberName );
		
		if canInviteRide( teamMember.MemberName ) then
			rider:inviteRide( actor:GetID() );
		end
	end
	
	if table.getn( t_cannotInvitePlayers ) == 0 then
		return;
	end

	ShowMidTips( table.concat( t_cannotInvitePlayers, "，" ).."距离太远，无法邀请骑乘" );
end
		},
							};

function PhotoFrameLevelBtn_OnEnter()
	local mainplayer = ActorMgr:getMainPlayer();
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "等级："..mainplayer:getLv().."级", frame = this:GetParent(),
						button = this:GetName() } );
end

function PhotoFrameRaceBtn_OnEnter()
	local mainplayer = ActorMgr:getMainPlayer();
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "职业："..t_raceList[mainplayer:getRace()], 
						frame = this:GetParent(), button = this:GetName(), vertical = "bottom", horizonal = "right" } );
end

function TargetPhotoFrameRaceBtn_OnEnter()
	local mainplayer = ActorMgr:getMainPlayer();
	local TargetObj	 = ActorMgr:FindActor( mainplayer:GetSelectTarget( ) );
	if TargetObj:getType() == GAT_PLAYER or TargetObj:getType() == GAT_MAINPLAYER then
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "职业："..t_raceList[TargetObj:getRace()], 
							frame = this:GetParent(), button = this:GetName(), vertical = "bottom", horizonal = "right" } );
	end
end
function PhotoFrame_OnShow()
end

function CanShowPhotomFrameXinShou()
	if GameMgr:isInPlayCamerAnim() then
		return false;
	end

	local t_mutexFrames = { "LoadingFrame", "GuideFrame" };
	for _, name in ipairs( t_mutexFrames ) do
		local frame = getglobal( name );
		if frame:IsShown() then
			return false;
		end
	end

	return true;
end

function ShowPhotomFrameXinShou()
	if not CanShowPhotomFrameXinShou() then
		return;
	end
	
	local tex = getglobal( "GiftBtnUVAnimationTex" );
	if tex:IsShown() then
		return;
	end

	local xinShouBtn	= getglobal( "PhotoFrameXinShouBtn" );
	
	local rich			= getglobal( xinShouBtn:GetName().."Rich" );
	tex:SetUVAnimation( 50, true);
	local szText = "您获得了礼包，请点击领取奖励";
	local nSelfDefineExtendWidth = 10;
	rich:SetClientString( szText );
	ReSizeTooltip( rich:GetName(), xinShouBtn:GetName(), nSelfDefineExtendWidth );
	xinShouBtn:Show();
	local closeBtn = getglobal( xinShouBtn:GetName().."_CloseBtn" );
	closeBtn:SetPointEx( "topright", "PhotoFrameXinShouBtnGuideBtn", "topright", -2, 0 );
end

function SetPhotomFrameXinShoBtn()
	local xinShouBtn	= getglobal( "PhotoFrameXinShouBtn" );	
	if xinShouBtn:IsShown() then
		return;
	end

	local rich			= getglobal( xinShouBtn:GetName().."Rich" );
	local szText		= "您获得了礼包，请点击领取奖励";
	local nSelfDefineExtendWidth = 10;
	rich:SetClientString( szText );
	ReSizeTooltip( rich:GetName(), xinShouBtn:GetName(), nSelfDefineExtendWidth );
	xinShouBtn:Show();
end

function HidePhotomFrameXinShou()
	local uvTex	= getglobal( "GiftBtnUVAnimationTex" );
	if not uvTex:IsShown() then
		return;
	end
	
	local t_hideIcons = { "PhotoFrameXinShouBtn", "GiftBtnUVAnimationTex" };
	util.HideIcons( t_hideIcons );
end

local t_PhotoFrameXinShouBtnControl = { ["startTime"] = 0, ["isShowGuide"] = false, 
										MAX_SHOW_GIFT_XIN_SHOU_TI_SHI = 5000, ["opFrame"] = "PhotoFrameXinShouBtn" };
function GetPhotoFrameXinShouBtnControl()
	return t_PhotoFrameXinShouBtnControl;
end

t_PhotoFrameXinShouBtnControl["startNewGiftGuide"] =
function ( self )
	self["startTime"]	= GameMgr:getTickTime();
	self["isShowGuide"] = true;
end

t_PhotoFrameXinShouBtnControl["endNewGiftGuide"] =
function ( self )
	self["startTime"]	= 0;
	self["isShowGuide"] = false;
end

t_PhotoFrameXinShouBtnControl["update"] =
function ( self )
	if not self["isShowGuide"] then
		return;
	end
	
	local nCurTime = GameMgr:getTickTime();
	if nCurTime - self["startTime"] >= self["MAX_SHOW_GIFT_XIN_SHOU_TI_SHI"] then
		self:endNewGiftGuide();
	end
end

t_PhotoFrameXinShouBtnControl["isShowGiftGuide"] =
function ( self )
	return self["isShowGuide"];
end

function PhotoFrameXinShouBtn_OnShow()
	t_PhotoFrameXinShouBtnControl:startNewGiftGuide();
end


function PhotoFrameXinShouBtn_OnUpdate()
	t_PhotoFrameXinShouBtnControl:update();
	if not t_PhotoFrameXinShouBtnControl:isShowGiftGuide() then
		this:Hide();
	end
end

local t_PhotoFrameFollowXinShouBtnControl = { ["startTime"] = 0, ["isShowGuide"] = false, 
										MAX_SHOW_FOLLOW_XIN_SHOU_TI_SHI = 8000, ["opFrame"] = "PhotoFrameFollowXinShouBtn" };
function GetPhotoFrameFollowXinShouBtnControl()
	return t_PhotoFrameFollowXinShouBtnControl;
end

t_PhotoFrameFollowXinShouBtnControl["startNewGiftGuide"] =
function ( self )
	self["startTime"]   = GameMgr:getTickTime();
	self["isShowGuide"] = true;
end

t_PhotoFrameFollowXinShouBtnControl["endNewGiftGuide"] =
function ( self )
	self["startTime"]	= 0;
	self["isShowGuide"] = false;
end

t_PhotoFrameFollowXinShouBtnControl["update"] =
function ( self )
	if not self["isShowGuide"] then
		return;
	end
	
	local nCurTime = GameMgr:getTickTime();
	if nCurTime - self["startTime"] >= self["MAX_SHOW_FOLLOW_XIN_SHOU_TI_SHI"] then
		self:endNewGiftGuide();
	end
end

t_PhotoFrameFollowXinShouBtnControl["isShowGiftGuide"] =
function ( self )
	return self["isShowGuide"];
end

function PhotoFrameFollowXinShouBtn_OnShow()
	t_PhotoFrameFollowXinShouBtnControl:startNewGiftGuide();
end

function PhotoFrameFollowXinShouBtn_OnUpdate()
	t_PhotoFrameFollowXinShouBtnControl:update();
	if not t_PhotoFrameFollowXinShouBtnControl:isShowGiftGuide() then
		PhotoFrameFollowXinShouBtn:Hide();
	end
end

function PhotoFrameFollowXinShouBtn_OnHide()
	GameWizard:doStep( GAMEWIZARD_FIRST_IN_TEAM );
end

function PhotoFrame_OnClick()
	if arg1 == "RightButton" then
		local mainplayer	= ActorMgr:getMainPlayer();
		local teamInfo		= TeamManager:getTeamInfo();--获取队伍信息
		-- 若主角在队伍中，则右键选项包括 离开队伍 和 邀请骑乘
		if teamInfo.MemberNum == 0 then
			return;
		end

		--local t_rightOperation = { "离开队伍" };
		local t_rightOperation = {};
		if mainplayer:isCaptain() then
			table.insert( t_rightOperation, "分配方式" );
		end		 
		local rider			= mainplayer:getRider();
		local curUsingRide	= rider:getRideItem();
		local rideDef		= getRideDef( curUsingRide.RideID, curUsingRide.nLevel );
		--  判断主角是否处于多人坐骑上
		if mainplayer:isInRide() and rideDef.RideOnNum > 0 then
			if mainplayer:GetID() == curUsingRide.m_nOwnerID then
				table.insert( t_rightOperation, "邀请全队骑乘" );
			end
		end
		
		if table.getn( t_rightOperation ) == 0 then
			return;
		end

		local frame = getglobal( "MainPlayerRightMenu" );
		HideRightPhotoFrameAllButton();
		for i = 1, math.min( table.getn( t_rightOperation ), MAX_MAIN_RIGHT_OPTION_BTNS ) do
			local btn	= getglobal( "MainPlayerRightBtn" .. i );
			local font	= getglobal( "MainPlayerRightBtn" .. i .. "Font" );
			font:SetText( t_rightOperation[i] );
			font:SetTextColor( 145, 125, 55 );
			btn:Show();
		end

		frame:SetHeight( 14 + table.getn( t_rightOperation ) * 16 );
		frame:Show();		
	end

	if arg1 == "LeftButton" then
		local mainplayer = ActorMgr:getMainPlayer();
		mainplayer:SetSelectTarget( mainplayer:GetID() );
	end
end

function PhotoFrameHP_OnLoad()
	this:RegisterEvent("UI_ACTOR_ATTRIBUTE_CHG");
end

function PhotoFrameMP_OnLoad()
	this:RegisterEvent("UI_ACTOR_ATTRIBUTE_CHG");
end

local bShowHPGuide		= false;
local nShowHPBeginTime	= 0;
function PhotoFrameHP_OnEvent()
	local mainplayer	= ActorMgr:getMainPlayer();
	local eqiup			= mainplayer:getEquip();
	local MaxHp			= mainplayer:getMaxHP();
	local Hp			= ( mainplayer:isDead() ) and 0 or ( ( mainplayer:getHP() >= 0 ) and mainplayer:getHP() or 0 );
	Hp = ( Hp > mainplayer:getMaxHP() ) and mainplayer:getMaxHP() or Hp;

	if( MaxHp ~= 0 ) then
		local hpTex = getglobal( "PhotoFrameHPTex" );
		local scale = Hp / MaxHp;
		HpPercentFont:SetText( Hp .. " / " .. MaxHp );
		hpTex:SetSize( scale * 143, 14 );

		-- 新手指引提示
		if GameWizard:isDo(GAMEWIZARD_FIRST_USEHP) == false and eqiup:getTmpMachin() == 0 and eqiup:getTmpArm() == 0 then
			bShowHPGuide = true;
			nShowHPBeginTime = os.clock();
		end
	end
end

function PhotoFrameHP_OnUpdate()
	if bShowHPGuide and ShortCutFrame:IsShown() then
		if os.clock() - nShowHPBeginTime > 0.5 then
			local mainplayer = ActorMgr:getMainPlayer();
			if ( mainplayer == nil ) then return end

			local MaxHp  = mainplayer:getMaxHP();
			local Hp     = ( mainplayer:getHP() >= 0 ) and mainplayer:getHP() or 0;
			Hp = ( Hp > mainplayer:getMaxHP() ) and mainplayer:getMaxHP() or Hp;

			if Hp < MaxHp*0.5 then
				for i = 1, 10 do
					local shortcutbtn = getglobal( "MagicShortcut"..i );
					if shortcutbtn:GetClientUserData(1) == 5000000 then
						GameWizard:doStep(GAMEWIZARD_FIRST_USEHP);
						local nShortCutNum = i;
						if i == 10 then
							nShortCutNum = 0;
						end
					end
				end
				bShowHPGuide = false;
				nShowHPBeginTime = 0;
			end
		end
	end
end

local bShowMPGuide		= false;
local nShowMPBeginTime	= 0;
function PhotoFrameMP_OnEvent()
	local mainplayer = ActorMgr:getMainPlayer();
	local eqiup = mainplayer:getEquip();

	local MaxMp  = mainplayer:getMaxMP();
	local Mp     = mainplayer:getMP();

	if( MaxMp ~= 0 ) then
		local mpTex = getglobal( "PhotoFrameMPTex" );
		local scale = Mp / MaxMp;
		local nResult = math.ceil( scale * 100 - 0.5 );
		MpPercentFont:SetText( Mp .. " / " .. MaxMp );
		mpTex:SetSize( scale * 143, 14 );

		-- 新手指引提示
		if GameWizard:isDo(GAMEWIZARD_FIRST_USEMP) == false  and eqiup:getTmpMachin() == 0 and eqiup:getTmpArm() == 0  then
			bShowMPGuide = true;
			nShowMPBeginTime = os.clock();
		end
	end
end

function PhotoFrameMP_OnUpdate()
	if bShowMPGuide and ShortCutFrame:IsShown() then
		if os.clock() - nShowMPBeginTime > 0.5 then
			local mainplayer = ActorMgr:getMainPlayer();
			if ( mainplayer == nil ) then return end

			local MaxMp  = mainplayer:getMaxMP();
			local Mp     = mainplayer:getMP();

			if Mp < MaxMp*0.5 then
				for i = 1, 10 do
					local shortcutbtn = getglobal( "MagicShortcut"..i );
					if shortcutbtn:GetClientUserData(1) == 5001000 then
						GameWizard:doStep(GAMEWIZARD_FIRST_USEMP);
						local nShortCutNum = i;
						if i == 10 then
							nShortCutNum = 0;
						end
					end
				end
				bShowMPGuide = false;
				nShowMPBeginTime = 0;
			end
		end
	end

	local mainplayer = ActorMgr:getMainPlayer();
	local MaxMp  = mainplayer:getMaxMP();
	local Mp     = mainplayer:getMP();

	if( MaxMp ~= 0 ) then
		MpPercentFont:SetText( Mp .. " / " .. MaxMp );
	end
end

function DPBtn_OnLoad()
	this:RegisterEvent( "UI_ACTOR_VIG_CHG" );
	this:RegisterEvent( "UI_ACTOR_VIGMAX_CHG" );
	--this:RegisterEvent( "GE_CHANGE_COLLECTTYPE" );
	this:RegisterEvent( "GE_ON_ACTOR_ENTER_WORLD" );
	this:RegisterEvent( "GE_ENTER_PLAYERLOGIN" );
end

COLLECTSKILL_ADDTYPE_AKER = 1; 	--	/* 攻击积累,每次被击积累10点 */
COLLECTSKILL_ADDTYPE_AKEE = 2;	--	/* 被击积累,每次被击积累10点 */
COLLECTSKILL_ADDTYPE_TIME = 3; 	--	/* 战斗时间积累,每秒积累20点 */
COLLECTSKILL_ADDTYPE_SKILL = 4; 	--	/* 战斗时间积累,指定技能增加 */

local t_dpTexturePath = 
{
[COLLECTSKILL_ADDTYPE_AKER]  =	{ texName = "DPBtnGreenTex" },	-- 绿色

[COLLECTSKILL_ADDTYPE_AKEE]  =	{ texName = "DPBtnYellowTex" },	-- 黄色

[COLLECTSKILL_ADDTYPE_TIME]  =	{ texName = "DPBtnPurpleTex" },	-- 紫色

[COLLECTSKILL_ADDTYPE_SKILL]  =	{ texName = "DPBtnGreenTex" },	-- 绿色
};

function DPBtn_OnEvent()
	--[[
	if  arg1 == "GE_CHANGE_COLLECTTYPE" or arg1 == "GE_ON_ACTOR_ENTER_WORLD" or arg1 == "GE_ENTER_PLAYERLOGIN" then
		local t_chgShowIcons = { "DPHeadBtn", "DPBtn" };
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer:getCollectSlotType() == 0 then
			util.HideIcons( t_chgShowIcons );
		else
			util.ShowIcons( t_chgShowIcons );
			
			local t_hideIcons = { "DPBtnGreenTex", "DPBtnYellowTex", "DPBtnPurpleTex" };
			util.HideIcons( t_hideIcons );
			
			local showColorDPTex = getglobal( t_dpTexturePath[mainplayer:getCollectSlotType()]["texName"] );
			showColorDPTex:Show();
			
			local maxSlot	= mainplayer:getCollectSlotMax();
			local curSlot	= mainplayer:getCollectSlotCurr();

			local scale		= curSlot / maxSlot;
			if scale > 1 then scale = 1 end			
			showColorDPTex:SetSize( scale * 100, 7 );

			local t_fonts	= { [this:GetName().."PercentFont"] = { value = curSlot .. " / " .. maxSlot }, };
			util.SetFonts( t_fonts );
		end
	end
	--]]
	if arg1 == "UI_ACTOR_VIG_CHG" or arg1 == "UI_ACTOR_VIGMAX_CHG" or arg1 == "GE_ON_ACTOR_ENTER_WORLD"
		or arg1 == "GE_ENTER_PLAYERLOGIN" then
		local mainplayer = ActorMgr:getMainPlayer();
		
		local t_hideIcons = { "DPBtnGreenTex", "DPBtnYellowTex", "DPBtnPurpleTex" };
		util.HideIcons( t_hideIcons );
			
		local showColorDPTex = getglobal( t_dpTexturePath[3]["texName"] );
		showColorDPTex:Show();
		
		local maxVig	= mainplayer:getVigorMax();
		local curVig	= mainplayer:getVigor();
		
		local scale		= curVig / maxVig;
		if scale > 1 then scale = 1 end			
		showColorDPTex:SetSize( scale * 100, 7 );

		local t_fonts	= { [this:GetName().."PercentFont"] = { value = curVig .. " / " .. maxVig }, };
		util.SetFonts( t_fonts );		
	end
	
end
function DPBtn_OnEnter()
	--[[
	local szText = "组队状态击杀队伍平均等级相差10级以内的怪恢复活力值。同一张地图内队员越多，队长加成越多。";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );--]]
end

function MP_OnEnter()
	local mainplayer = ActorMgr:getMainPlayer();

	local MaxMp  = mainplayer:getMaxMP();
	local Mp     = mainplayer:getMP();
	local Race   = mainplayer:getRace();

	local szText = "魔法值： "..Mp.." / "..MaxMp;
	 
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function MP_OnLeave()
	GameTooltip:Hide();
end

function LeaveTeam_OnClick()
	LeaveTroopButton_OnClick();
end



local MACHINE_WAR_CAMP_HUMAN = 1			--人类阵营编号
local MACHINE_WAR_CAMP_TITAN = 2			--巨人阵营编号
local MACHINE_WAR_CAMP_MONSTER = 3		--怪兽阵营编号
function getMachineWarCampName( CampIdx )
	if CampIdx == MACHINE_WAR_CAMP_HUMAN then
		return "人类"
	elseif CampIdx == MACHINE_WAR_CAMP_TITAN then
		return "机械巨人"
	elseif CampIdx == MACHINE_WAR_CAMP_MONSTER then
		return "机械怪兽"
	end
	return "";
end

-- index按照lua习惯取，若index为0表示无效索引
local t_targetInfo = { isTeam = false, index = 0, name = "" };

function SetTargeInfo( data )
	t_targetInfo = data;
	if t_targetInfo["isTeam"] then
		Target:Show();
	end
end

local t_targetEvents = {	-- 切换目标对象
							["GE_TARGET_CHANGED"] = {}, 
							-- 目标对象归属发生变化
							["GE_CHANGE_MON_BOOTY"] = {},
							-- 离开队伍
							["GE_TEAM_LEAVETEAM"] = {},
							-- 加入队伍
							["GE_TEAM_APPLYSUCC"] = {},
							-- 目标对象属性发生变化
							["UI_TARGET_ATTRIBUTE_CHG"] = {}, 
							-- 目标对象的buff发生变化
							["GE_TARGET_BUF_LIST_UPDATE"] = {},
							-- 目标对象下骑乘
							["GE_TARGET_RIDE_CHG"] = {}, 
							-- 目标对象的出征宠物的战斗模式改变
							["GE_UPDATE_TARGET_PLAYER_PET_FIGHT_MODE"] = {}, 
							-- 目标宠物的战斗模式改变
							["GE_UPDATE_TARGET_PET_FIGHT_MODE"] = {}, 
							-- 目标宠物的被收回
							["GE_TARGET_PET_CALLBACK"] = {}, 
							-- 目标玩家改名字
							["GE_TARGET_CHG_NAME"]	= {}, 
							-- 目标宠物的战斗属性变化
							};


-- 这些事件是在目标在视野范围内的时候才会有
t_targetEvents["GE_TARGET_CHANGED"].func = 
function ()
	local targetFrame	= getglobal( "Target" );
	local mainplayer	= ActorMgr:getMainPlayer();
	local nTargetId		= mainplayer:GetSelectTarget();

	-- 若不是由于组队造成被选中的目标ID为0
	if not t_targetInfo["isTeam"] then
		t_targetInfo = { isTeam = false, index = 0, name = "" };
	end

	if nTargetId == 0 then
		--targetFrame:Hide();
		return;
	end
	
	local TargetObj	= ActorMgr:FindActor( nTargetId );
	
	-- 若为临时道具则不显示目标头像
	if TargetObj:getType() == GAT_ITEM then
		local itemDef = getItemDef( TargetObj:getResId() );
		if itemDef.ItemTmp.Type == ITEM_TMP_ITEM then
			return;
		end
	end
	
	t_targetInfo = { isTeam = false, index = 0, name = GetPlayerName(TargetObj:getName()) };
	if mainplayer:IsInSpecialPworldNotShowTitle() and TargetObj:getType() == GAT_MONSTER then
		local ownerId = TargetObj:getOwnerId()
		if ownerId ~= 0 then
			local owner = ActorMgr:FindActor( ownerId )
			t_targetInfo = { isTeam = false, index = 0, name = getMachineWarCampName(owner:getCampIdx()) };
		end
	end
	
	targetFrame:Show();
	local raceTexture = getglobal("TargetPhotoBackground3");
	local targetOfTarget	=	getglobal("TargetOfTarget");
	if ( TargetObj:getType() == GAT_PLAYER or TargetObj:getType() == GAT_MAINPLAYER ) and TargetObj:getRace() > 5 then
		raceTexture:Show();
		targetOfTarget:SetPoint("topleft", "Target", "bottomright", -70, 4 );
	else
		raceTexture:Hide();
		targetOfTarget:SetPoint("topleft", "Target", "bottomright", -70, -4 );
	end
	TargetPhoto:SetGray(false);
	TargetFightRectBackground:SetGray(false);
	if TargetObj:getType() == GAT_MONSTER then
		local gray = TargetObj:hasOtherBooty();
		if gray then
			TargetPhoto:SetGray(true);
			TargetFightRectBackground:SetGray(true);
		end
	end
	OnChangeHitTargetID( TargetObj:getHitTargetID() );
end

t_targetEvents["GE_CHANGE_MON_BOOTY"].func = 
function()
	local msg			= UIMSG:getUIMsg();
	local mainplayer	= ActorMgr:getMainPlayer();
	local nTargetId		= mainplayer:GetSelectTarget();
	if msg.Who.id == nTargetId and msg.Who.type == GAT_MONSTER then
		local actor = ActorMgr:FindActor(nTargetId);
		if actor then
			local gray = actor:hasOtherBooty();
			TargetPhoto:SetGray(gray);
			TargetFightRectBackground:SetGray(gray);
		end
	end
end

t_targetEvents["GE_TEAM_LEAVETEAM"].func = 
function()
	local mainplayer	= ActorMgr:getMainPlayer();
	local nTargetId		= mainplayer:GetSelectTarget();
	local actor = ActorMgr:FindActor(nTargetId);
	if actor then
		local gray = actor:hasOtherBooty();
		TargetPhoto:SetGray(gray);
		TargetFightRectBackground:SetGray(gray);
	end
end

t_targetEvents["GE_TEAM_APPLYSUCC"].func = t_targetEvents["GE_TEAM_LEAVETEAM"].func

-- 在没选中目标情况下也会发该消息：1.刚进游戏的时候，主角那儿发
t_targetEvents["UI_TARGET_ATTRIBUTE_CHG"].func = 
function ()
	local mainplayer	= ActorMgr:getMainPlayer()
	local TargetObj		= ActorMgr:FindActor( mainplayer:GetSelectTarget() );	
	if TargetObj == nil then
		return;
	end

	-- 若为临时道具则不显示
	if TargetObj:getType() == GAT_ITEM then
		local itemDef = getItemDef( TargetObj:getResId() );
		if itemDef.ItemTmp.Type == ITEM_TMP_ITEM then
			return;
		end
	end
	
	UpdateTargetPhotoInfo();
	-- 设置对应目标的头像信息
	ChangeTargetFrameUV( );
end

t_targetEvents["GE_TARGET_BUF_LIST_UPDATE"].func = 
function ()
	UpdateTargetBuffWhenTargetChange();
end

t_targetEvents["GE_TARGET_RIDE_CHG"].func = 
function ()
	t_targetEvents["GE_TARGET_BUF_LIST_UPDATE"].func();
end

t_targetEvents["GE_UPDATE_TARGET_PLAYER_PET_FIGHT_MODE"].func = 
function ()
	-- TODO: 目前只能处理出征一只宠物的情况，若出征多只则无法处理
	local mainplayer = ActorMgr:getMainPlayer()
	local TargetObj  = ActorMgr:FindActor( mainplayer:GetSelectTarget() );
	
	local bNeedUpdateBuff = false;
	local petPackage = TargetObj:getPet();
	for i = 1, PET_MAX_FIGHT_SHI do
		local pet = petPackage:getFightPet( i - 1 );
		-- TODO: 是否要判断宠物已经死亡
		if pet ~= nil then
			local petDiathesis = pet:getPetDiathesis();
			for i = 1, petDiathesis.m_nNum do
				local diathesisDef = SkillMgr:getDiathesisDef( petDiathesis.m_diathesisData[i-1].m_nID, 
																petDiathesis.m_diathesisData[i-1].m_nLevel );
				if diathesisDef ~= nil and diathesisDef.IconPosition == 1 then
					bNeedUpdateBuff = true;
					break;
				end
			end
		end
	end
	
	if bNeedUpdateBuff then
		UpdateTargetBuffWhenTargetChange();
	end
end

t_targetEvents["GE_TARGET_PET_CALLBACK"].func = 
function ()
	-- 判定是否需要更新目标头像的Buff
	local mainplayer	= ActorMgr:getMainPlayer();
	local TargetObj		= ActorMgr:FindActor( mainplayer:GetSelectTarget() );
	local petPackage	= TargetObj:getPet();
	local bFound		= false;
	
	-- TODO: 若改成多只宠物可出征，这需要更改，这儿只能处理一只宠物出征的情况
	for i = 1, PET_MAX_FIGHT_SHI do
		local pet = petPackage:getFightPet( i - 1 );
		-- TODO: 是否要判断宠物已经死亡
		if pet ~= nil and pet:getPetFightMode() == PET_FIGHT_ACTIVE then
			local petDiathesis = pet:getPetDiathesis();
			for i = 1, petDiathesis.m_nNum do
				local nId = petDiathesis.m_diathesisData[i-1].m_nID;
				local nLv = petDiathesis.m_diathesisData[i-1].m_nLevel;
				local diathesisDef = SkillMgr:getDiathesisDef( nId, nLv );
				if diathesisDef ~= nil and diathesisDef.IconPosition == 1 then
					bFound = true;
					break;
				end
			end
		end
	end
	
	if bFound then
		UpdateTargetBuffWhenTargetChange();
	end
end

t_targetEvents["GE_TARGET_CHG_NAME"].func = 
function ()
	local mainplayer	= ActorMgr:getMainPlayer()
	local TargetObj		= ActorMgr:FindActor( mainplayer:GetSelectTarget( ) );
	local TargetObjName = TargetObj:getName()
	if mainplayer:IsInSpecialPworldNotShowTitle() and TargetObj:getType() == GAT_MONSTER then
		local ownerId = TargetObj:getOwnerId()
		if ownerId ~= 0 then
			local owner = ActorMgr:FindActor( ownerId )
			TargetObjName = getMachineWarCampName( owner:getCampIdx() )	
		end	
	end
	local t_fonts = 
	{ 
		["TargetPhotoName"] = { value = TargetObjName,	color = { r = 255, g = 254, b = 191 }, },
	};
	util.SetFonts( t_fonts );
end
		
function Target_OnLoad()
	for event, _ in pairs( t_targetEvents ) do
		this:RegisterEvent( event );
	end

	t_targetEvents.__index = function ( tab, key )
								tab[key] = { func = function () end };
								return tab[key];
							end
	setmetatable( t_targetEvents, t_targetEvents );
end

function Target_OnEvent()
	t_targetEvents[arg1].func();
end

-- 当出现吟唱条的时候动态调整buff按钮的位置
function UpdateTargetBuffPos()
	local frame = getglobal( "TargetPrepareBar" );
	local buffBtn = getglobal( "TargetBuff1" );
	if frame:IsShown() then
		buffBtn:SetPoint( "topleft", "Target", "topleft", 130, 90 );
	else
		buffBtn:SetPoint( "topleft", "Target", "topleft", 130, 70 );
	end

	for i = 1, MAX_TARGET_GOOD_BUF_COUNT do
		addChangedFrames( getglobal( "TargetBuff" .. i ) );	
	end

	for i = 1, MAX_TARGET_DEBUF_COUNT do
		addChangedFrames( getglobal( "TargetDebuff" .. i ) );	
	end
end

-- 目标头像
function Target_OnShow()
	local mainplayer = ActorMgr:getMainPlayer();
	local nTargetId = mainplayer:GetSelectTarget();

	UpdateTargetBuffWhenTargetChange();
	
	-- 设置对应目标的头像信息
	UpdateTargetPhotoInfo();
	-- 设置各个目标头像的美术效果
	ChangeTargetFrameUV();
end

function Target_OnHide()
	SetTargeInfo( { isTeam = false, index = 0, name = "" } );
	TargetOfTarget:Hide();
end

function UpdateMonsterFigthFlag( TargetObj )
	local photoCircleBack	= getglobal( "TargetFightCircleBackground" );
	local photoRectBack		= getglobal( "TargetFightRectBackground" );
	-- 若主角处于战斗状态，则显示战斗状态标志，隐藏等级名字
	if TargetObj:isInFight() then
		photoCircleBack:Show();
		photoRectBack:Show();
		
		if not util.isInFadeState( photoCircleBack:GetName() ) then
			util.UIFrameToFadeIn( photoCircleBack:GetName(), 0, 1, 1000, 100 );
			util.UIFrameToFadeKeep( photoCircleBack:GetName(), 1, 1000 );
			util.UIFrameToFadeOut( photoCircleBack:GetName(), 1, 0, 1000, 100 );
		end
		
		if not util.isInFadeState( photoRectBack:GetName() ) then
			util.UIFrameToFadeIn( photoRectBack:GetName(), 0, 1, 1000, 100 );
			util.UIFrameToFadeKeep( photoRectBack:GetName(), 1, 1000 );
			util.UIFrameToFadeOut( photoRectBack:GetName(), 1, 0, 1000, 100 );
		end

	else
		photoCircleBack:Hide();
		photoRectBack:Hide();
	end
end

function Target_OnUpdate()
	local mainplayer = ActorMgr:getMainPlayer()
	local TargetObj = ActorMgr:FindActor( mainplayer:GetSelectTarget( ) );
	local teamInfo		= TeamManager:getTeamInfo();
	-- 若是由于组队选中则直接关闭
	if TargetObj == nil and not t_targetInfo["isTeam"] then
		-- 若不在队伍中，则隐藏目标头像，否则将选中目标置为0，同时将目标头像信息来源置为队员
		local teamMember;
		local nIndex;
		for i = 1, teamInfo.MemberNum do
			local tmpMember = teamInfo.TeamMember[i-1];
			if tmpMember.MemberName == t_targetInfo["name"] then
				teamMember = tmpMember;
				nIndex = i;
				break;
			end
		end
		
		if teamMember ~= nil then
			SetTargeInfo( { isTeam = true, index = nIndex, name = teamMember.MemberName } );
			mainplayer:RawSetSelectTarget( 0 );
		else
			Target:Hide();	
			mainplayer:SetSelectTarget( 0 );		
		end		
		return;
	end

	if t_targetInfo["isTeam"] then
		-- 若队员下线，则隐藏
		local teamMember = teamInfo.TeamMember[t_targetInfo["index"]-1];
		if teamMember.OffLineTime ~= CLIENT_MEMBER_ONLINE then
			this:Hide();
		else
			UpdateTargetBuffWhenTargetChange();
			-- 设置对应目标的头像信息
			UpdateTargetPhotoInfo();
			-- 设置各个目标头像的美术效果
			ChangeTargetFrameUV();
		end
	end

	-- 若是怪物死亡，则隐藏头像
	if TargetObj ~= nil and TargetObj:getType() == GAT_MONSTER then
		if TargetObj:isDead() then
			Target:Hide();	
		else
			UpdateMonsterFigthFlag( TargetObj );
		end
	end
end

function getTargetInfo()
	local isTeam = t_targetInfo["isTeam"];
	local name = t_targetInfo["name"];
	return isTeam,name;
end

-- @Desc: 此为目标对象的buff发生变化的时候刷新目标对象的Buff
-- 这种情况的刷新方法是若button处于转动状态，则不对其重新设置buff状态，而只对没用到的buff进行新的初始化
-- 对队伍头像的buff同步，只需将目标头像的buf图标的转动逻辑值设置给队伍头像的Buff图标
-- 假buff在每次真Buff改变的时候重新设置
-- 隐藏目标头像的所有buffer
function ClearTargetBuffer()
	for i = 1, MAX_TARGET_GOOD_BUF_COUNT do
		DelCollBuff( "TargetBuff" .. i );	
	end

	for i = 1, MAX_TARGET_DEBUF_COUNT do
		DelCollBuff( "TargetDebuff" .. i );	
	end
end
	
function Target_OnClick()
	if arg1 ~= "RightButton" then
		return;
	end

	local mainplayer	= ActorMgr:getMainPlayer()
	local TargetObj		= ActorMgr:FindActor( mainplayer:GetSelectTarget( ) );
	local nActorType	= t_targetInfo["isTeam"] and GAT_PLAYER or TargetObj:getType();

	-- 显示玩家右键选项
	if nActorType == GAT_PLAYER then
		ShowCharacterRightFrame( t_targetInfo["isTeam"] and t_targetInfo["name"] or TargetObj:getName() );
	elseif nActorType == GAT_PET then
		-- 显示宠物右键选项
		ShowPetRightFrame( TargetObj:GetID() );
	elseif TargetObj:isCollectMachine( ) then
		local monsterMachine = TargetObj:getMonsterMachine(); 
		local monsterMachineInfo = monsterMachine:getMonMachineDef( TargetObj:getResId() );
		if monsterMachineInfo ~= nil and monsterMachineInfo.CollectType ~= 0 then
			-- 显示采集车右键选项
			ShowMachineRightFrame( TargetObj:GetID( ) );
		end
	end
end


-- 宠物头像宠物列表

function PetXian_OnLoad()
	this:RegisterEvent( "GE_TRUMP_ATTRUPDATE" );
end

function IsTrumpCamp()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then 
		return;
	end		
	local GameTrump	= MainPlayer:getTrump();
	for i = 1, GameTrump:getTrumpMaxNum() do
		local trumpInfo = GameTrump:getRoleFairy( i - 1 );
		if trumpInfo.FairyID ~= 0 and GameTrump:isFairyInState( trumpInfo.Stat, FAIRY_STAT_CAMP ) then
			local trumpDef = GameTrump:getFairyDef( trumpInfo.FairyID, trumpInfo.FairyLevel );
			if trumpDef ~= nil then
				-- 设置当前出征法宝的图标与名称
				local IconPath = GetTrumpIconPath()..trumpDef.ModeID..".tga";
				if not IsInExistence( IconPath ) then
					IconPath = GetTrumpIconPath().."000000.tga";
				end
				XPFrame_PhotoTexture:SetTexture( IconPath );
				return true;
			end
			
		end
	end
	return false;
end

function PetXian_OnEvent()
	if arg1 == "GE_TRUMP_ATTRUPDATE" then
		-- 不在精灵变身，面板没有显示的时候， show
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer == nil then return end
		if IsTrumpCamp() then
			if not XPFrame:IsShown() and ( mainplayer:isInStall() or not mainplayer:isOnMorph() )then
				XPFrame:Show();
			end
		else
			XPFrame:Hide();
		end
	end
end

function TargetHP_OnEnter()
	local mainplayer = ActorMgr:getMainPlayer();
	local TargetObj = ActorMgr:FindActor( mainplayer:GetSelectTarget()  );
	local MaxHp  = TargetObj:getMaxHP();
	local nType	 = TargetObj:getType();
	local szText = "";

	local Hp = ( TargetObj:getHP() >= 0 ) and TargetObj:getHP() or 0;
	Hp = ( Hp > TargetObj:getMaxHP() ) and TargetObj:getMaxHP() or Hp;

	if nType == GAT_MONSTER then
		szText = "HP: "..Hp.." / "..TargetObj:getMaxHP();
	elseif nType == GAT_MAINPLAYER or nType == GAT_PLAYER or nType == GAT_PET or nType == GAT_NPC then
		szText = "HP: "..Hp.." / "..TargetObj:getMaxHP();
	end

	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function TargetHP_OnLeave()
	GameTooltip:Hide();
end

function TargetMP_OnEnter()
	local mainplayer = ActorMgr:getMainPlayer();
	local TargetObj = ActorMgr:FindActor( mainplayer:GetSelectTarget()  );
	local nType	 = TargetObj:getType();
	local szText = "";

	if nType == GAT_NPC or nType == GAT_MAINPLAYER or nType == GAT_PLAYER then
		szText = "魔法值："..TargetObj:getMP().." / "..TargetObj:getMaxMP();
	end

	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function TargetMP_OnLeave()
	GameTooltip:Hide();
end

-- 
local t_updateTargetBuffer = { [true] = {}, [false] = {}, };

function SetTeamTargetJiaFairyBuffer( data )
	local nGoodBufBtnCount	= data["goodBuffCount"];
	local campFairyId		= data["fairyID"];
	local campFairyLv		= data["fairyLv"];
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
				AddCoolFairyBuf( "TargetBuff" .. nGoodBufBtnCount, nId, nLv, 0 );
				nGoodBufBtnCount = nGoodBufBtnCount + 1;
			end
		end
	end

	return nGoodBufBtnCount;
end

-- 此为是队员的时候刷新目标头像用的
t_updateTargetBuffer[true].func = 
function ()
	local teamInfo		= TeamManager:getTeamInfo();
	local teamMember	= teamInfo.TeamMember[t_targetInfo["index"]-1];
	local BufStatus		= teamMember.StatusInfo;
	local btn			= GetTeamPhotoBtn( teamMember.MemberName );
	

	local nGoodBtnCount = 1;
	local status = teamMember.StatusInfo;
	for i = 1, status.GoodStatusNum do
		if nGoodBtnCount > MAX_TARGET_GOOD_BUF_COUNT then
			break;
		end

		local goodStatus = status.GoodStatusList[i-1];
		AddTeamBuffer( { btn = "TargetBuff"..nGoodBtnCount, id = goodStatus.StatusID, level = goodStatus.Level, targetId = 0,
							overlay = ( goodStatus.StatusID == CLIENT_DIE_REMIT_BUFFID ) and ( goodStatus.Overlay ) or ( goodStatus.Overlay + 1 ), 
							name = teamMember.MemberName } );
		
		nGoodBtnCount = nGoodBtnCount + 1;
	end
	
	-- 若处于合体状态，则显示合体buff
	local TargetObj = ActorMgr:findActorByName(teamMember.MemberName, GAT_PLAYER)
	if TargetObj then
		local petMgr = TargetObj:getPet();
		local petHetiIdx = petMgr:getPetHeti();
		if petHetiIdx ~= -1 then
			local petActor	= petMgr:getFightPet( petHetiIdx );
			local petInfo 	= petMgr:getPetInfo( petActor ) ;
			if petMgr:isExtraBattlePet(petInfo) then
				AddCoolPetHetiBuf("TargetBuff"..nGoodBtnCount, petHetiIdx, TargetObj:GetID(),true);
			else 
				AddCoolPetHetiBuf("TargetBuff"..nGoodBtnCount, petHetiIdx, TargetObj:GetID(),false);
			end
		
		--	AddCoolPetHetiBuf("TargetBuff"..nGoodBtnCount, petHetiIdx, TargetObj:GetID());
			nGoodBtnCount = nGoodBtnCount + 1;
		end
	end
	
	if teamMember.RideID ~= 0 and nGoodBtnCount <= MAX_TARGET_GOOD_BUF_COUNT then
		AddCoolRideBuf( "TargetBuff"..nGoodBtnCount, teamMember.RideID, teamMember.RideLevel, 0 );
		nGoodBtnCount = nGoodBtnCount + 1;
	end

	if teamMember.FairyID ~= 0 and nGoodBtnCount <= MAX_TARGET_GOOD_BUF_COUNT then
		nGoodBtnCount = SetTeamTargetJiaFairyBuffer( { goodBuffCount = nGoodBtnCount, fairyID = teamMember.FairyID, 
														fairyLv = teamMember.FairyLevel } );
	end

	for i = 1, status.NoLimitStatusNum do
		if nGoodBtnCount > MAX_TARGET_GOOD_BUF_COUNT then
			break;
		end
		
		local nolimitStatus = status.NolimitStatusList[i-1];
		AddTeamBuffer( { btn = "TargetBuff"..nGoodBtnCount, id = nolimitStatus.StatusID, level = nolimitStatus.Level, targetId = 0,
							overlay = ( nolimitStatus.StatusID == CLIENT_DIE_REMIT_BUFFID ) and ( nolimitStatus.Overlay ) or ( nolimitStatus.Overlay + 1 ),
							name = teamMember.MemberName } );
		nGoodBtnCount = nGoodBtnCount + 1;
	end
	
	local nBadBtnCount	= 1;
	for i = 1, status.BadStatusNum do
		if nBadBtnCount > MAX_TARGET_DEBUF_COUNT then
			break;
		end
		
		local badStatus = status.BadStatusList[i-1];
		AddTeamBuffer( { btn = "TargetDebuff" .. nBadBtnCount, id = badStatus.StatusID, level = badStatus.Level, targetId = 0,
							overlay = ( badStatus.StatusID == CLIENT_DIE_REMIT_BUFFID ) and ( badStatus.Overlay ) or ( badStatus.Overlay + 1 ), 
							name = teamMember.MemberName } );
		nBadBtnCount = nBadBtnCount + 1;
	end

	AdJustTargetBuffBtnsPos( nGoodBtnCount - 1, nBadBtnCount - 1 );
end

function SetPetJiaPetBuffer( data )
	local pet				= data["target"];
	local nGoodBufBtnCount	= data["goodBuffCount"];
	local petDiathesis		= pet:getPetDiathesis();
	for i = 1, petDiathesis.m_nNum do
		if nGoodBufBtnCount > MAX_PET_SHI_BUFF_NUM then
			break;
		end
		
		local nId = petDiathesis.m_diathesisData[i-1].m_nID;
		local nLv = petDiathesis.m_diathesisData[i-1].m_nLevel;
		local diathesisDef = SkillMgr:getDiathesisDef( nId, nLv );
		if diathesisDef ~= nil and diathesisDef.IconPosition == 2 then
			AddCoolPetBuf( "TargetBuff" .. nGoodBufBtnCount, nId, nLv, pet:GetID() );
			nGoodBufBtnCount = nGoodBufBtnCount + 1;	
		end
	end

	return nGoodBufBtnCount;
end

function SetPetGuanZhiBuffer( data )
	local nGoodBufBtnCount	= data["goodBuffCount"];
	local pet				= data["target"];
	local guanZhiTrumpID	= GetChuZhengPetTongLingTrumpID( pet );
	if guanZhiTrumpID == 0 then
		return nGoodBufBtnCount;
	end

	local t_oneTrumpGuanZhiConfig = t_petGuanZhiBuff[guanZhiTrumpID];
	if t_oneTrumpGuanZhiConfig == nil then
		return nGoodBufBtnCount;
	end

	AddCoolPetGuanZhiBuf( "TargetBuff" .. nGoodBufBtnCount, t_oneTrumpGuanZhiConfig["StatusID"], t_oneTrumpGuanZhiConfig["level"], pet:GetID() );

	nGoodBufBtnCount = nGoodBufBtnCount + 1;	
	return nGoodBufBtnCount;
end

function SetPlayerJiaFairyBuffer( data )
	local TargetObj			= data["target"];
	local GameTrump			= TargetObj:getTrump();
	local nGoodBufBtnCount	= data["goodBuffCount"];
	local campFairyId		= GameTrump:getCampFairyId();
	local campFairyLv		= GameTrump:getCampFairyLv();
	if campFairyId == 0 then
		return nGoodBufBtnCount;
	end

	local TrumpDef	= GameTrump:getFairyDef( campFairyId, campFairyLv );
	for i = 1, FAIRY_MAX_SKILL do
		local trumpSkillInfo = TrumpDef.FairySkillList[i-1];
		if trumpSkillInfo.SkillID ~= 0 and trumpSkillInfo.SkillType == DIATHESIS_TYPE then
			local nId = trumpSkillInfo.SkillID;
			local nLv = trumpSkillInfo.SkillLevel;
			local diathesisDef = SkillMgr:getDiathesisDef( trumpSkillInfo.SkillID, trumpSkillInfo.SkillLevel );
			if diathesisDef.IconPosition == 1 then
				AddCoolFairyBuf( "TargetBuff" .. nGoodBufBtnCount, nId, nLv, TargetObj:GetID() );
				nGoodBufBtnCount = nGoodBufBtnCount + 1;
			end
		end
	end

	return nGoodBufBtnCount;
end

function SetPlayerJiaPetBuffer( data )
	-- 查看此人出征的宠物对他的影响
	local TargetObj			= data["target"];
	local petPackage		= TargetObj:getPet();
	local nGoodBufBtnCount	= data["goodBuffCount"];

	local szText = ""
	for i = 1, PET_MAX_FIGHT_SHI do
		local pet = petPackage:getFightPet( i - 1 );
		-- TODO: 是否要判断宠物已经死亡
		if pet ~= nil and not pet:isDead() then
			local petDiathesis = pet:getPetDiathesis();
			-- 若宠物不处于主动状态，则不能给主人加buff
			if petDiathesis.m_nFightMode ~= PET_FIGHT_ACTIVE and pet.Passive ~= PET_FIGHT_ACTIVE then
				return nGoodBufBtnCount;
			end
			for j= 1, petDiathesis.m_nNum do				
				local nId = petDiathesis.m_diathesisData[j-1].m_nID;
				local nLv = petDiathesis.m_diathesisData[j-1].m_nLevel;
				local diathesisDef = SkillMgr:getDiathesisDef( nId, nLv );
				if diathesisDef ~= nil and diathesisDef.IconPosition == 1 then
					szText = szText .. string.gsub(diathesisDef.Name,"（被动技能）","：") .. diathesisDef.DiathesisDesc .. "#n\n";
				end
			end
		end
	end
	if szText ~= "" then
		szText = "#cff6e1c宠物技能：#n\n" ..szText
		AddMainPetBuf( "TargetBuff" .. nGoodBufBtnCount, szText );
		nGoodBufBtnCount = nGoodBufBtnCount + 1;
	end

	return nGoodBufBtnCount;
end

-- 此为是在目标在视野范围内刷新目标头像用的
t_updateTargetBuffer[false].func = 
function ()
	local mainplayer = ActorMgr:getMainPlayer()
	local TargetObj  = ActorMgr:FindActor( mainplayer:GetSelectTarget() );
	
	local nType		 = TargetObj:getType();
	-- 这个判断是出于防止雷达旁边的buff和目标头像的buff由于子程序调用出现的时间误差
	if nType == GAT_MAINPLAYER then
		-- 若主角没有任何buf状态，则清空所有的buf
		TargetObj = mainplayer;
	end

	local BufStatus	= TargetObj:getBufStatus();

	local nGoodBufBtnCount = 1;
	for i=1, MAX_GOOD_STATUS do
		if nGoodBufBtnCount > MAX_TARGET_GOOD_BUF_COUNT then
			break;
		end

		-- 刷洗目标头像的buf
		local Buf		= BufStatus:getGoodStatus( i - 1 );
		local BufIconId	= Buf:getIconId();
		local BufText	= "uires\\图片类\\技能\\Buff\\" .. BufIconId..".tga";
		local btn		= getglobal( "TargetBuff" .. nGoodBufBtnCount );
		-- 当buf第一次显示的时候，则不检测按钮是否处于cooldown状态，若buf已经不是第一帧显示，则要出于同步结束的考虑，
		-- 在最后一帧的时候强制结束buf显示，
		-- 若由于切换目标，造成不是第一次设置，而且btn也不处于
		if Buf:isShow() and Buf:getTotalLife() > 0 then
			assert( Buf:getBufLv() ~= nil, "获得Buf等级为0!!!" );
			local bAddResult = AddCoolBuffer( "TargetBuff" .. nGoodBufBtnCount, GetBuffIconPath( BufIconId ) ,Buf:getLife(), Buf:getTotalLife(), 
									Buf:getBufId(), Buf:getBufLv(), Buf:getBufId() == CLIENT_DIE_REMIT_BUFFID and ( Buf:getOverlay() ) or ( Buf:getOverlay() + 1 ), 
									TargetObj:GetID(), Buf );
			if bAddResult then
				nGoodBufBtnCount = nGoodBufBtnCount + 1;
			end			
		end
	end
	
	-- 若处于合体状态，则显示合体buff
	local petMgr = TargetObj:getPet();
	local petHetiIdx = petMgr:getPetHeti();
	if petHetiIdx ~= -1 then
		local petActor	= petMgr:getFightPet( petHetiIdx );
		local petInfo 	= petMgr:getPetInfo( petActor ) ;
		if petMgr:isExtraBattlePet(petInfo) then
			AddCoolPetHetiBuf("TargetBuff"..nGoodBufBtnCount, petHetiIdx, TargetObj:GetID(),true);
		else 
			AddCoolPetHetiBuf("TargetBuff"..nGoodBufBtnCount, petHetiIdx, TargetObj:GetID(),false);
		end
	--	AddCoolPetHetiBuf("TargetBuff"..nGoodBufBtnCount, petHetiIdx, TargetObj:GetID());
		nGoodBufBtnCount = nGoodBufBtnCount + 1;
	end

	local rider			= TargetObj:getRider();
	local curUsingRider	= rider:getRideItem();
	-- 若当前设置的骑乘信息按钮即为正在被骑的骑乘，则显示特效
	if rider:isOnHorse() and curUsingRider.RideID ~= 0 and nGoodBufBtnCount <= MAX_TARGET_GOOD_BUF_COUNT then
		local rideDef = getRideDef( curUsingRider.RideID, curUsingRider.nLevel );
		AddCoolRideBuf( "TargetBuff" .. nGoodBufBtnCount, curUsingRider.RideID, curUsingRider.nLevel, TargetObj:GetID() );
		nGoodBufBtnCount = nGoodBufBtnCount + 1;
	end
	
	-- 查看此人出征的宠物对他的影响
	
	if TargetObj:getType() == GAT_PLAYER or TargetObj:getType() == GAT_MAINPLAYER then
		nGoodBufBtnCount = SetPlayerJiaPetBuffer( { target = TargetObj, goodBuffCount = nGoodBufBtnCount } );
		nGoodBufBtnCount = SetPlayerJiaFairyBuffer( { target = TargetObj, goodBuffCount = nGoodBufBtnCount } );
	elseif TargetObj:getType() == GAT_PET then
		nGoodBufBtnCount = SetPetJiaPetBuffer( { target = TargetObj, goodBuffCount = nGoodBufBtnCount } );
		nGoodBufBtnCount = SetPetGuanZhiBuffer( { target = TargetObj, goodBuffCount = nGoodBufBtnCount } );
	end

	for i=1, MAX_NOLIMIT_STATUS do
		if nGoodBufBtnCount > MAX_TARGET_GOOD_BUF_COUNT then
			break;
		end

		local Buf		= BufStatus:getNolimitStatus(i-1);
		local BufIconId	= Buf:getIconId();
		local BufText	= "uires\\图片类\\技能\\Buff\\"..BufIconId..".tga";
		local btn		= getglobal( "TargetBuff" .. nGoodBufBtnCount );
		if Buf:isShow() and Buf:getTotalLife() > 0 then
			assert( Buf:getBufLv() ~= nil, "获得Buf等级为0!!!" );
			local bAddResult = AddCoolBuffer( "TargetBuff" .. nGoodBufBtnCount, GetBuffIconPath( BufIconId ), Buf:getLife(), Buf:getTotalLife(), 
							Buf:getBufId(), Buf:getBufLv(), Buf:getBufId() == CLIENT_DIE_REMIT_BUFFID and ( Buf:getOverlay() ) or ( Buf:getOverlay() + 1 ), 
							TargetObj:GetID(), Buf );

			if bAddResult then
				nGoodBufBtnCount = nGoodBufBtnCount + 1;
			end		
		end
	end

	local nBadBufBtnCount		= 1;
	for i=1, MAX_BAD_STATUS do
		if nBadBufBtnCount > MAX_TARGET_DEBUF_COUNT then
			break;
		end

		local Buf		= BufStatus:getBadStatus(i-1);
		local BufIconId	= Buf:getIconId();
		local BufText	= "uires\\图片类\\技能\\Buff\\"..BufIconId..".tga";
		local btn		= getglobal( "TargetDebuff" .. nBadBufBtnCount );
		if Buf:isShow() and Buf:getTotalLife() > 0 then
			assert( Buf:getBufLv() ~= nil, "获得Buf等级为0!!!" );
			local bAddResult = AddCoolBuffer( "TargetDebuff" .. nBadBufBtnCount, GetBuffIconPath( BufIconId ),Buf:getLife(), Buf:getTotalLife(), 
												Buf:getBufId(), Buf:getBufLv(), 
												Buf:getBufId() == CLIENT_DIE_REMIT_BUFFID and ( Buf:getOverlay() ) or ( Buf:getOverlay() + 1 ),
												TargetObj:GetID(), Buf );
			if bAddResult then
				nBadBufBtnCount = nBadBufBtnCount + 1;
			end		
		end
	end
	
	AdJustTargetBuffBtnsPos( nGoodBufBtnCount - 1, nBadBufBtnCount - 1 );
end

-- @Desc: 此为在目标更换的时候更新buf
-- 其更新原理是，若目标角色是主角，则使用主角身上的buf，不使用目标角色的buff；
-- 若是切换目标，第一步是可以清空所有的buff状态，然后再重新设置其buff状态
-- 对队伍头像的buff同步，只需将目标头像的buf图标的转动逻辑值设置给队伍头像的Buff图标
function UpdateTargetBuffWhenTargetChange()
	ClearTargetBuffer();
	t_updateTargetBuffer[t_targetInfo["isTeam"]].func();
end

function IsBtnContainBuff( data )
	local btn = getglobal( data["name"] );
	if not btn:IsShown() then
		return false;
	end

	local nId = btn:GetClientUserData( 0 );
	local nLv = btn:GetClientUserData( 1 );
	-- 由于显示转动效果会比逻辑状态晚一帧
	local buf = data["buffStatus"]:getBufStatus( nId, nLv );
	if buf ~= nil then
		return buf:isShow();
	end

	-- 处于转动状态的btn已经有对应的buf的时候
	-- 但是同时考虑处于转动状态的buf寿命可能没有原Buf长,则要重新设置buf的寿命
	-- 若由于玩家自己手动取消buf，则不能有这个判断

	-- 再考虑到有些buff不是真buff，是假buff，则直接认为不存在，假buff在每次真Buff改变的时候重新设置
	if getRideDef( nId, nLv ) ~= nil then
		return false;
	end

	-- 考虑到有些buff不需要实现转动效果
	if not canShowBuff( nId, nLv ) then
		return true;
	end

	return false;
end

-- 用于设置不同类型目标头像的数据
local t_updateTargetPhotoInfo = 
{ [GAT_ITEM] = {}, [GAT_PLAYER] = {}, [GAT_MAINPLAYER] = {}, [GAT_MONSTER] = {}, [GAT_NPC] = {}, [GAT_PET] = {}, [GAT_FAIRY] = {}
};

t_updateTargetPhotoInfo[GAT_ITEM].func = 
function ( targetActor )
end

t_updateTargetPhotoInfo[GAT_PLAYER].func = 
function ( targetActor )
	local teamInfo = TeamManager:getTeamInfo();
	local teamMember;
	if t_targetInfo["isTeam"] then
		teamMember = teamInfo.TeamMember[t_targetInfo["index"]-1];
	end
	local nHair		= t_targetInfo["isTeam"] and teamMember.Hair or targetActor:getHair();
	local nFace		= t_targetInfo["isTeam"] and teamMember.Face or targetActor:getFace();
	local nGender	= t_targetInfo["isTeam"] and teamMember.Gender or targetActor:getGender();
	local nHead		= t_targetInfo["isTeam"] and teamMember.Head or targetActor:getHead();
	SetPhoto( nHair, nFace, "TargetHairPhoto", "TargetPhoto", nGender, nHead, "TargetHairShadowTex", "TargetBodyTex", 
				"uires\\TuPianLei\\TouXiang\\JueSeTouXiang\\MuBiaoTouXiang  mask.tga" );
	
	local nLv		= t_targetInfo["isTeam"] and teamMember.Level or targetActor:getLv();
	local szName	= t_targetInfo["isTeam"] and teamMember.MemberName or targetActor:getName();
	-- 设置目标对象的等级信息
	local t_fonts = { 
					["TargetLevel"]		= { value = nLv,	color = { r = 255, g = 221, b = 161 }, },
					["TargetPhotoName"] = { value = GetPlayerName( szName ),	color = { r = 255, g = 254, b = 191 }, },
					};
	util.SetFonts( t_fonts );
	
	local bCaptain;
	if t_targetInfo["isTeam"] then
		bCaptain = teamMember.Position == TEAM_POSITION_CAPTAIN;
	else
		bCaptain = targetActor:isCaptain();
	end

	if bCaptain then
		TargetCaptainFlag:Show();
		local uv = PhotoFrameCaptionButtonGetUV( teamInfo.TeamBootyMode );
		TargetCaptainFlag:ChangeNormalTexture( uv.x,uv.y,uv.w,uv.h );		
	else
		TargetCaptainFlag:Hide();
	end

	local t_hideIcons = { "TargetMonsterPinZhiFlagBtn" };
	util.HideIcons( t_hideIcons );
	
	local texPath			= t_photoFrameRaceTexture[ t_targetInfo["isTeam"] and teamMember.Career or targetActor:getRace()].path
	local texUV				= t_photoFrameRaceTexture[ t_targetInfo["isTeam"] and teamMember.Career or targetActor:getRace()].UV 
	local targetRaceTex 	= getglobal("TargetPhotoFrameRaceSignTexture")
	local targetRaceBackTex = getglobal("TargetPhotoFrameRaceSignTextureBack")
	local raceTexture 		= getglobal("TargetPhotoBackground3");
	local targetOfTarget	= getglobal("TargetOfTarget");
	if ( t_targetInfo["isTeam"] and teamMember.Career or targetActor:getRace() ) > 5 then
		targetRaceTex:SetSize(23,26);
		raceTexture:Show();
		targetOfTarget:SetPoint("topleft", "Target", "bottomright", -70, 4 );
	else
		targetRaceTex:SetSize(20,28);	
		raceTexture:Hide();
		targetOfTarget:SetPoint("topleft", "Target", "bottomright", -70, -4 );
	end
	targetRaceTex:SetTexture( texPath )
	targetRaceTex:SetTexUV( texUV.x, texUV.y, texUV.width, texUV.height )
	targetRaceTex:Show()
	targetRaceBackTex:Show()
	
	local monsterPhysicTex = getglobal("MonsterPhsicTypeTex");
	monsterPhysicTex:Hide();	
end

t_updateTargetPhotoInfo[GAT_MAINPLAYER].func = 
function ( targetActor )
	t_updateTargetPhotoInfo[GAT_PLAYER].func( targetActor );
end

local t_monsterNameColor =	
{
	[MONSTER_GREEN]		=	{ r = 156, g = 211, b = 45 },
	[MONSTER_WHITE]		=	{ r = 255, g = 253, b = 223 },
	[MONSTER_YELLOW]	=	{ r = 255, g = 250, b = 83 },
	[MONSTER_ORANGE]	=	{ r = 236, g = 118, b = 13 },
	[MONSTER_RED]		=	{ r = 229, g = 40,  b = 40 },
};

t_updateTargetPhotoInfo[GAT_MONSTER].func = 
function ( targetActor )
	local t_tgas = { ["TargetPhoto"] = { path = GetMonsterIconPath( targetActor:getIconId() ) }, };
	util.SetTgas( t_tgas );
	

	local nSubtype = getMonsterSubtype( targetActor:getResId() );
	if GameBCT:isIn() then
		nSubtype = 6
	end
	if t_monsterPinzhiTex[nSubtype] ~= nil then
		local t_changeUVTexes = { ["MonsterPinzhiFlagTex"] = 
		{	x = t_monsterPinzhiTex[nSubtype].x,			y = t_monsterPinzhiTex[nSubtype].y,
			width = t_monsterPinzhiTex[nSubtype].width, height = t_monsterPinzhiTex[nSubtype].height }, };
		
		for name, data in pairs( t_changeUVTexes ) do
			local tex = getglobal( name );
			tex:SetTexUV( data["x"], data["y"], data["width"], data["height"] );
		end
		
		local t_showIcons = { "TargetMonsterPinZhiFlagBtn", };
		for _, name in ipairs( t_showIcons ) do
			local icon = getglobal( name );
			icon:Show();
		end
	else
		TargetMonsterPinZhiFlagBtn:Hide();
	end
	
	local targetActorName = GetPlayerName(targetActor:getName())
	if targetActor:IsInSpecialPworldNotShowTitle() then
		local ownerId = targetActor:getOwnerId()
		if ownerId ~= 0 then
			local owner = ActorMgr:FindActor( ownerId )
			targetActorName = getMachineWarCampName( owner:getCampIdx() )
		end
	end
	
	local t_fonts = { 
					["TargetLevel"]		= { value = (nSubtype == MON_SUB_TYPE_TOTEM_TOLUA) and "" or targetActor:getLv(),	color = { r = 255, g = 221, b = 161 }, },
					["TargetPhotoName"] = { value = targetActorName,	
											color = t_monsterNameColor[getMonsterNameColor( targetActor:getResId() )], },
					};
	util.SetFonts( t_fonts );	
	
	local targetRaceBackTex = getglobal("TargetPhotoFrameRaceSignTextureBack");
	local targetRaceTex = getglobal("TargetPhotoFrameRaceSignTexture");
	targetRaceTex:Hide();
	targetRaceBackTex:Hide();
	
	
	local monsterPhysicTex = getglobal("MonsterPhsicTypeTex");
	local physictTexPath = t_monsterPhysicTypeTex[targetActor:getMonsterPhysicType()].path;
	local physictTexUV = t_monsterPhysicTypeTex[targetActor:getMonsterPhysicType()].UV;
	monsterPhysicTex:SetTexture( physictTexPath );
	monsterPhysicTex:SetTexUV( physictTexUV.x, physictTexUV.y, physictTexUV.width, physictTexUV.height );
	monsterPhysicTex:Show();
end

t_updateTargetPhotoInfo[GAT_NPC].func = 
function ( targetActor )
	local t_tgas = { ["TargetPhoto"] = { path = GetMonsterIconPath( targetActor:getIconId() ) }, };
	util.SetTgas( t_tgas );
	
	local t_fonts = { 
					["TargetLevel"]		= { value = targetActor:getLv(),	color = { r = 255, g = 221, b = 161 }, },
					["TargetPhotoName"] = { value = targetActor:getName(),	color = { r = 255, g = 254, b = 191 }, },
					};
	util.SetFonts( t_fonts );

	local t_hideIcons = { "TargetMonsterPinZhiFlagBtn" };
	util.HideIcons( t_hideIcons );	
	
	local targetRaceBackTex = getglobal("TargetPhotoFrameRaceSignTextureBack");
	local targetRaceTex = getglobal("TargetPhotoFrameRaceSignTexture");
	targetRaceTex:Hide();
	targetRaceBackTex:Hide();
	
	local monsterPhysicTex = getglobal("MonsterPhsicTypeTex");
	monsterPhysicTex:Hide();
end

t_updateTargetPhotoInfo[GAT_PET].func = 
function ( targetActor )
	local t_tgas = { ["TargetPhoto"] = { path = GetMonsterIconPath( targetActor:getPetIconId() ) }, };
	util.SetTgas( t_tgas );

	local t_fonts = { 
					["TargetLevel"]		= { value = targetActor:getLv(),	color = { r = 255, g = 221, b = 161 }, },
					["TargetPhotoName"] = { value = targetActor:getName(),	color = { r = 255, g = 254, b = 191 }, },
					};
	util.SetFonts( t_fonts );

	local t_hideIcons = { "TargetMonsterPinZhiFlagBtn" };
	util.HideIcons( t_hideIcons );
	
	local targetRaceBackTex = getglobal("TargetPhotoFrameRaceSignTextureBack");
	local targetRaceTex = getglobal("TargetPhotoFrameRaceSignTexture");
	targetRaceTex:Hide();
	targetRaceBackTex:Hide();
	
	local monsterPhysicTex = getglobal("MonsterPhsicTypeTex");
	monsterPhysicTex:Hide();
end

function UpdateTargetPhotoInfo()
	local mainplayer = ActorMgr:getMainPlayer()
	local TargetObj = ActorMgr:FindActor( mainplayer:GetSelectTarget( ) );
	-- 既没有选中队伍头像也没有选中目标
	if not t_targetInfo["isTeam"] and TargetObj == nil then
		return;
	end
	
	local nType = t_targetInfo["isTeam"] and GAT_PLAYER or TargetObj:getType();
	t_updateTargetPhotoInfo[nType].func( TargetObj );
end

-- 用于设置不同类型目标头像的美术UV
local t_setTargetUV = 
{ 
	[GAT_ITEM] = {}, [GAT_PLAYER] = {}, [GAT_MAINPLAYER] = {}, [GAT_MONSTER] = {}, [GAT_NPC] = {}, [GAT_PET] = {}, [GAT_FAIRY] = {}
};

t_setTargetUV[GAT_ITEM].func = 
function ( targetActor )
end

t_setTargetUV[GAT_PLAYER].func = 
function ( targetActor )
	local t_showIcons = { "TargetPhotoBackground2", "TargetMP", "TargetHairPhoto", "TargetHairShadowTex", "TargetBodyTex", };
	util.ShowIcons( t_showIcons );

	-- 设置根据条件判断一些特殊控件
	local teamInfo = TeamManager:getTeamInfo();
	local teamMember;
	if t_targetInfo["isTeam"] then
		teamMember = teamInfo.TeamMember[t_targetInfo["index"]-1];
	end

	local nHp		= t_targetInfo["isTeam"] and teamMember.HP or targetActor:getHP();
	local nMaxHP	= t_targetInfo["isTeam"] and teamMember.MaxHP or targetActor:getMaxHP();
	local hpTex	= getglobal( "TargetHPTex" );
	 
	-- clamp
	nHp = ( nHp >= 0 ) and nHp or 0;
	nHp = ( nHp > nMaxHP ) and nMaxHP or nHp;
	-- 若死亡
	if targetActor ~= nil and targetActor:isDead() then
		nHp = 0;
	end

	local scale = nHp / nMaxHP;
	hpTex:SetSize( scale * 142, 12 );
	
	local mpTex		= getglobal( "TargetMPTex" );
	local nMp		= t_targetInfo["isTeam"] and teamMember.MP or targetActor:getMP();
	local nMaxMP	= t_targetInfo["isTeam"] and teamMember.MaxMP or targetActor:getMaxMP();
	-- clamp
	nMp = ( nMp >= 0 ) and nMp or 0;
	nMp = ( nMp >nMaxMP ) and nMaxMP or nMp;
	scale = nMp / nMaxMP;
	mpTex:SetSize( scale * 142, 12 );

	local t_fonts =	{	["TargetHPPercentFont"] = { value = nHp .. " / " .. nMaxHP }, 
						["TargetMPPercentFont"] = { value = nMp .. " / " .. nMaxMP },
					};
	util.SetFonts( t_fonts );
	
	local bCaptain;
	if t_targetInfo["isTeam"] then
		bCaptain = ( teamMember.Position == TEAM_POSITION_CAPTAIN );
	else
		bCaptain = targetActor:isCaptain();
	end

	if bCaptain then
		TargetCaptainFlag:Show();
		local uv = PhotoFrameCaptionButtonGetUV( teamInfo.TeamBootyMode );
		TargetCaptainFlag:ChangeNormalTexture( uv.x,uv.y,uv.w,uv.h );	
	else
		TargetCaptainFlag:Hide();
	end
end

t_setTargetUV[GAT_MAINPLAYER].func = 
function ( targetActor )
	t_setTargetUV[GAT_PLAYER].func( targetActor );
end

t_setTargetUV[GAT_MONSTER].func = 
function ( targetActor )
	local t_hideIcons = { "TargetCaptainFlag", "TargetHairPhoto", "TargetMP", "TargetHairShadowTex", "TargetBodyTex", "TargetPhotoBackground2", };
	util.HideIcons( t_hideIcons );
	
	local nHp	= ( targetActor:getHP() >= 0 ) and targetActor:getHP() or 0;
	nHp			= ( nHp > targetActor:getMaxHP() ) and targetActor:getMaxHP() or nHp;
	-- 若死亡
	if targetActor ~= nil and targetActor:isDead() then
		nHp = 0;
	end

	local scale = nHp / targetActor:getMaxHP();
	local hpTex	= getglobal( "TargetHPTex" );
	hpTex:SetSize( scale * 142, 12 );

	local t_conditionFonts =	{	["TargetHPPercentFont"] = { value = nHp .. " / " .. targetActor:getMaxHP() }, 
								};
	for name, data in pairs( t_conditionFonts ) do
		local font = getglobal( name );
		font:SetText( data["value"] );
	end

	local t_showIcons = { "TargetHP", };
	for _, name in ipairs( t_showIcons ) do
		local icon = getglobal( name );
		icon:Show();
	end
end

t_setTargetUV[GAT_PET].func = 
function ( targetActor )
	t_setTargetUV[GAT_MONSTER].func( targetActor );
end

t_setTargetUV[GAT_NPC].func = 
function ( targetActor )
	t_setTargetUV[GAT_MONSTER].func( targetActor );
end

-- @brief: 该函数仅仅用于设置控件的位置
function ChangeTargetFrameUV( )
	local mainplayer	= ActorMgr:getMainPlayer();
	local targetActor	= ActorMgr:FindActor( mainplayer:GetSelectTarget() );
	-- 既没有选中队伍头像也没有选中目标
	if not t_targetInfo["isTeam"] and targetActor == nil then
		return;
	end
	
	local nType	= t_targetInfo["isTeam"] and GAT_PLAYER or targetActor:getType();
	t_setTargetUV[nType].func( targetActor );
end

function PetAttackOnClick()
	local slot = PetMgr:GetActiveSlot();
	PetMgr:OnUiSetPetMode(slot , 0);
end

function PetFollowOnClick()
	local slot = PetMgr:GetActiveSlot();
	PetMgr:OnUiSetPetMode(slot , 1);
end

function PetFairyMP_OnEnter()
end

function PetFairyMP_OnLeave()
end

-- @Desc:移入仙子目标头像血条
function PetFairyHP_OnEnter()
	local szText = "魔法值："..this:GetClientUserData(0).."/"..this:GetClientUserData(1);
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function PetFairyHP_OnLeave()
	GameTooltip:Hide();
end

function MainPlayerRightMenu_OnLoad()
	for i = 1, MAX_MAIN_RIGHT_OPTION_BTNS do
		local btn = getglobal( "MainPlayerRightBtn"..i );
		btn:SetOverlayRegion( 0, -3, 95, 22 );
	end
end

-- 主角头像右键选项
function MainPlayerRightMenu_OnShow()
	--util.AdjustTooltipsScreenPos( { tips = this:GetName() } );
	this:SetPoint("topLeft", "UIParent", "topleft", GetCursorPosX() ,GetCursorPosY() );
end

function MainPlayerRightBtn_OnClick()
	local font = getglobal( this:GetName() .. "Font" );
	t_mainRightOption[font:GetText()].func();
	--MainPlayerRightMenu:Hide();
end

function MainPlayerRightBtn_OnEnter()
	local font = getglobal( this:GetName() .. "Font" );
	font:SetTextColor( 255, 254, 189 );
	if font:GetText() == "分配方式" then
		ShowTeamBootyMenu( this:GetName() );
	end
end

function MainPlayerRightBtn_OnLeave()
	local font = getglobal( this:GetName() .. "Font" );
	font:SetTextColor( 145, 125, 55 );
end

function PKBtn_OnEnter()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer:isInPVPMap() and GameActorPVP:getPkMode() == 5 then
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = string.format("当前战斗模式为：%s\n#c00ff00（可自由攻击非己方阵营玩家）#n", this:GetClientString()), frame = this:GetParent(),
						button = this:GetName(), horizonal = "right" } );
	else
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = string.format("当前战斗模式为：%s\n点击此处切换战斗模式\n快捷切换按键：ctrl + A", this:GetClientString()), frame = this:GetParent(),
							button = this:GetName(), horizonal = "right" } );
	end
end

function PKBtn_OnLeave()
	GameTooltipRichText:SetText("",255,255,255);
	GameTooltip:Hide();
end

function GiftBtn_OnEnter()
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "点击打开新手礼包", frame = this:GetParent(),
						button = this:GetName(), horizonal = "right" } );
end

function GiftBtn_OnLeave()
	GameTooltipRichText:SetText("",255,255,255);
	GameTooltip:Hide();
end

function GiftBtn_OnClick()
	if PhotoFrameXinShouBtn:IsShown() then
		PhotoFrameXinShouBtn:Hide();
	end

	local Frame= getglobal( "GuideGiftFrame" );
	if (Frame:IsShown()) then
		Frame:Hide();
	else
		Frame:Show();
	end
end

function PhotoFrameStopWeiXingButton_OnClick()
	if GameFollowManager:isWeiXing() then
		GameFollowManager:cancelWeiXing();
	end
	PhotoFrameStopWeiXingButton:Hide();
end

function PhotoFrameStopWeiXingButton_OnEnter()
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "停止跟随", frame = this:GetParent(),
						button = this:GetName(), horizonal = "right" } );
end

-- 已经获取的礼包数量
local nCanGetGiftNum = 0;

function getCanGetGiftNum()
	local nNum = 0;	
	local nLVNum = 0;
	for i = 1, GameWizard:getOneDayGiftNum() do
		local onedaygift	= GameWizard:getOneDayGiftDef( i - 1 );
		local nOnLineTm		= onedaygift.GiftCond.OnlineTM;
		local nRet			= CanGetAward( onedaygift.GiftCond.LVMin, nOnLineTm, onedaygift.GiftID, "OneDayGift" );	
		if nRet == CAN_GET_GIFT then
			nNum = nNum + 1;
		end
	end
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return nNum;
	end	
	for i = 1, GameWizard:getLVGiftNum() do
		local lvgift	= GameWizard:getLVGiftDef( i - 1 );
		local nRet		= CanGetAward( lvgift.LV, 0, 0, "LVGift" );
		
		if nRet == CAN_GET_GIFT then
			nNum = nNum + 1;
			nLVNum = nLVNum + 1;
		end
	end
	local ContOnlineInfo = GameWizard:getContOnlineInfo();
	for i=1,ContOnlineInfo.Num do
		if i + 1 >= 5 then
			break;
		end
		local info = ContOnlineInfo.ContOnlineInfo[i-1];
		if info.IsGift == CAN_GET_GIFT then
			nNum = nNum + 1;
		end
	end
	local accGiftInfo = GameWizard:getAccGiftInfo();
	if accGiftInfo.Count > 0 then
		for i=1,accGiftInfo.Count do
			if (i - 1) >= MAX_ACC_GIFT_LIST_TOLUA then
				break;
			end			
			if accGiftInfo.AccGifts[i-1].Flag == ACCGIFT_CAN_GET_TOLUA then
				nNum = nNum + 1;
			end			
		end
	end
	local propGiftInfo = GameWizard:getPropGiftInfo();
	for i=1,propGiftInfo.Count do
		local sum = 0;
		local money = propGiftInfo.PropsGift[i-1].CashNow;
		for j=1,propGiftInfo.DefCount do
			if propGiftInfo.PropsDef[j-1].GroupID == propGiftInfo.PropsGift[i-1].GroupID and 
				GameWizard:compareValue( money, propGiftInfo.PropsDef[j-1].CashUp ) >= 0 then
				sum = sum + 1;
			end
		end
		
		 if sum - propGiftInfo.PropsGift[i-1].GiftGetNum > 0 then
			nNum = nNum + sum - propGiftInfo.PropsGift[i-1].GiftGetNum;
		 end
	end
	if GameWizard:CanGetPreCreateGift() then
		nNum = nNum + 1;
	end
	if GameWizard:IsVipNetBarUser() then
		local nCount = GameWizard:GetVipNetBarGiftNum();
		for i=1,nCount do
			local BarGift = GameWizard:GetVipNetBarGift( i-1 );
			local nRet = CanGetAward( i-1, 0, 0, "VipNetBarGift" )
			if nRet == CAN_GET_GIFT then
				nNum = nNum + 1;
			end
		end
	end
	if GiftCenterGuideCheckInButton:IsShown() then 
		nNum = nNum + 1;
	end
	nNum = nNum + GameWizard:getContiNum();
	return nNum,nLVNum;
end

function GiftBtn_OnUpdate()
	local nNum,nLVNum = getCanGetGiftNum();
	if nNum > 0 and CanShowPhotomFrameXinShou() then
		ShowPhotomFrameXinShou()
	else
		HidePhotomFrameXinShou();
	end
	if nNum > nCanGetGiftNum then
		SetPhotomFrameXinShoBtn();
	end
	nCanGetGiftNum = nNum;
	ShowLvGiftFrameBtn( nLVNum );
	ShowMysRewardGiftFrame();
end

function InviteSafePK( roleName )
	PKMessageBoxFrame:Show();
	PKMessageBoxFrameText:SetText("#P"..roleName.."邀请你切磋，是否同意？",215,190,132)
	PKMessageBoxFrame:SetClientString(roleName);
	nAcceptTime = os.clock();
	szPkName = roleName;
end

function CancelInvitePK()
	if PKMessageBoxFrame:IsShown() then
		PKMessageBoxFrame:Hide();
	end
end

--队长标记，当前分配模式
local t_BootyName = { [TEAM_BOOTY_RAND_TOLUA] = "随机分配",[TEAM_BOOTY_ROLL_TOLUA] = "按需分配",[TEAM_BOOTY_ALL_TOLUA] = "自由拾取" };
local t_BootyUVTex = { [TEAM_BOOTY_RAND_TOLUA] = {x=173,y=288,w=19,h=12},[TEAM_BOOTY_ROLL_TOLUA] = {x=197,y=362,w=19,h=12},[TEAM_BOOTY_ALL_TOLUA] = {x=197,y=375,w=19,h=12} };

function PhotoFrameCaptionButtonGetUV( nBootyType )
	return t_BootyUVTex[ nBootyType ];
end

function PhotoFrameCaptainButton_OnEnter()
	local teamInfo = TeamManager:getTeamInfo();
	if teamInfo.MemberNum > 0 then
		SetGameTooltips( this:GetName(),"分配方式："..t_BootyName[ teamInfo.TeamBootyMode ] .. "\n#G队伍内人数越多获得经验越高" );
	end
end

--------------------	怪物目标的目标		----------------

function TargetOfTarget_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local obj = ActorMgr:FindActor( mainplayer:GetSelectTarget() );
	if obj == nil then return end
	mainplayer:SetSelectTarget( obj:getHitTargetID() );
end

function TargetOfTarget_OnUpdate()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then 
		this:Hide();
		return;
	end
	local obj = ActorMgr:FindActor( mainplayer:GetSelectTarget() );
	if obj == nil then
		this:Hide();
		return;
	end
	local target = ActorMgr:FindActor( obj:getHitTargetID() );
	if target == nil then
		this:Hide();
		return;
	end
end

function OnChangeHitTargetID( nTargetID )
	local actor = ActorMgr:FindActor( nTargetID );
	if actor == nil then
		TargetOfTarget:Hide();
		return;
	end
	if not TargetOfTarget:IsShown() then
		TargetOfTarget:Show();
	end
	UpdateTargetOfTargetFrame(actor:GetID());
end

function UpdateTargetOfTargetFrame( actorid )
	local actor = ActorMgr:FindActor( actorid );
	if actor == nil then return end
	--TargetOfTarget_LVFont:SetText( actor:getLv() );
	TargetOfTarget_NameFont:SetText( GetPlayerName(actor:getName()) );
	if actor:getType() == GAT_PLAYER or actor:getType() == GAT_MAINPLAYER then
		TargetOfTargetPhoto:Hide();
		local t_showIcons = { "TargetOfTargetRoleHairPhoto", "TargetOfTargetRoleTexture", "TargetOfTargetRoleHairShadowTex", "TargetOfTargetRoleBodyTex", };
		util.ShowIcons( t_showIcons );
		SetPhoto( actor:getHair(), actor:getFace(), "TargetOfTargetRoleHairPhoto","TargetOfTargetRoleTexture",
				actor:getGender(), actor:getHead(), "TargetOfTargetRoleHairShadowTex", 
				"TargetOfTargetRoleBodyTex", "uires\\TuPianLei\\TouXiang\\JueSeTouXiang\\ZuDuiTouXiang  MASK.tga" );
	else
		local t_hideIcons = { "TargetOfTargetRoleHairPhoto", "TargetOfTargetRoleTexture", "TargetOfTargetRoleHairShadowTex", "TargetOfTargetRoleBodyTex", };
		util.HideIcons( t_hideIcons );
		TargetOfTargetPhoto:Show();
		if actor:getType() == GAT_PET then
			TargetOfTargetPhoto:SetTexture( GetMonsterIconPath( actor:getPetIconId()) );
		else
			TargetOfTargetPhoto:SetTexture( GetMonsterIconPath( actor:getIconId()) );
		end		
	end
	local scale = ( actor:getMaxHP() == 0 and 0 ) or ( actor:getHP() / actor:getMaxHP() );
	local hp = ( actor:getHP() > actor:getMaxHP() ) and actor:getMaxHP() or actor:getHP();
	scale = scale > 1 and 1 or scale;
	TargetOfTarget_HPTex:SetSize( scale * 72, 6 );

	local mp = ( actor:getMP() > actor:getMaxMP() ) and actor:getMaxMP() or  actor:getMP();
	scale = ( actor:getMaxMP() == 0 ) and 0 or ( actor:getMP() / actor:getMaxMP() );
	scale = scale > 1 and 1 or scale;
	TargetOfTarget_MPTex:SetSize( scale * 72, 6 );
	--TargetOfTarget_HPFont:SetText( hp.." / "..actor:getMaxHP() )
	--TargetOfTarget_MPFont:SetText( mp.." / "..actor:getMaxMP() )
end

function TargetofTargetHP_OnEnter()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local obj = ActorMgr:FindActor( mainplayer:GetSelectTarget() );
	if obj == nil then return end
	local target = ActorMgr:FindActor( obj:getHitTargetID() );
	if target == nil then end

	local szText = "生命值："..target:getHP() .. " / " .. target:getMaxHP();
	szText = szText .. "\n 魔法值：" .. target:getMP() .. " / " .. target:getMaxMP();

	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function TargetofTarget_OnEnter()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local obj = ActorMgr:FindActor( mainplayer:GetSelectTarget() );
	if obj == nil then return end
	local target = ActorMgr:FindActor( obj:getHitTargetID() );
	if target == nil then end

	local szText = "生命值："..target:getHP() .. " / " .. target:getMaxHP();
	szText = szText .. "\n魔法值：" .. target:getMP() .. " / " .. target:getMaxMP();

	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

-- 确定离开队伍面板
function ConfirmLeaveTeamFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 0, 0 );
end

function ConfirmLeaveTeamFrame_ConfirmButton_OnClick()
	TeamManager:Team_OP_Exit();
	ConfirmLeaveTeamFrame:Hide();
end


-- 采集车右键事件，显示菜单 2012.04.12
function ShowMachineRightFrame( nId )
	local nShowBtn = 1;
	local frame = getglobal( "FriendFrame_RightOptionFrame" );
	HideRightFrameAllButton();
	local btn	= getglobal( "FriendFrame_RightBtn"..nShowBtn );
	local font	= getglobal( btn:GetName().."Font" );
	font:SetText("查看资源");
	font:SetTextColor( 145, 125, 55 );
	btn:Enable();
	btn:Show();
	frame:SetHeight( 14 + nShowBtn * 16 );
	frame:Show();
end

function QuickRegisterBtn_OnClick()
	if RegisterUserFrame:IsShown() then
		RegisterUserFrame:Hide();
	else
		RegisterUserFrame:Show();
		RegisterUserFrame:SetClientUserData( 0,1 )
	end
end

function QuickRegisterBtn_OnEnter()
	SetGameTooltips( this:GetName(),"注册账号" );
end

function PhotoFrameAppealFollowButton_OnEnter()
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "组队跟随", frame = this:GetParent(),
						button = this:GetName(), horizonal = "right" } );
end

function PhotoFrameAppealFollowButton_OnClick()
	local teamInfo = TeamManager:getTeamInfo();
	if teamInfo.MemberNum == 0 then
		ShowMidTips( "你还没加入队伍" );
		return;
	end
	if canTeamFollow() then
		TeamRoleAppealFollowSignButtonUVAnimationTex:SetUVAnimation( 30, false );
		PhotoFrameAppealFollowSignButtonUVAnimationTex:SetUVAnimation( 30, false );
		TeamManager:Team_OP_AppealTeamFollow();
		setTeamFollowBegin();
		TroopInviteTeamFollowFrame:Hide();
	end	
end

function PhotoFrameCancelFollowButton_OnEnter()
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "取消跟随", frame = this:GetParent(),
						button = this:GetName(), horizonal = "right" } );
end

function PhotoFrameCancelFollowButton_OnClick()
	local teamInfo = TeamManager:getTeamInfo();
	if teamInfo.MemberNum == 0 then
		ShowMidTips( "你还没加入队伍" );
		return;
	end
	if TeamManager:isInTeamFollow() or isAnyTeamMemberInFollow() then
		TeamManager:Team_OP_CancelTeamFollow();
	end
end

function PhotoFrameFollowButton_OnEnter()
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "正在组队跟随", frame = this:GetParent(),
						button = this:GetName(), horizonal = "right" } );
end

function TargetMonsterPinZhiFlagBtn_OnEnter()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local obj = ActorMgr:FindActor( mainplayer:GetSelectTarget() );
	if obj == nil or obj:getType() ~= GAT_MONSTER then return end
	local subType = getMonsterSubtype( obj:getResId() )
	if subType == MON_SUB_TYPE_JI_TOLUA or subType == MON_SUB_TYPE_MACHINE_TOLUA or subType == MON_SUB_TYPE_CLAN_FACTORY_TOLUA then
		SetGameTooltips(this:GetName(), "机械类，需要使用机械或特殊武器攻击才能造成较高的伤害" );
	end
end

function PhotoFrameVipBtn_OnClick()
	if not VipFrame:IsShown() then
		VipFrame:Show()
	else
		VipFrame:Hide()
	end
end

function PhotoFrameVipBtn_OnEnter()
	local szText = "未购买特权";
	if VipManager:haveOpenVip() then
		szText = "当前VIP特权：\n";
		for i=1,8 do
			local def = VipManager:getStarStoneDefByIdx( i-1 );
			if def~= nil and VipManager:isEnableVip( def.ID ) then
				szText = szText .. "、" .. def.Name .. "特权";
			end
		end
	end

	local key = GetKeyMap("打开VIP界面");
	if key > 0 then
		szText = szText .. string.format("\n打开VIP界面 ( 快捷键 %s )", KeyToString(key));
	end
	SetGameTooltips( this:GetName(),  string.gsub( szText, "、", "", 1 ) );
end

local miniAutoTick = 0;
function AutoQuestSetEffectTimeTick()
	miniAutoTick = os.clock();
end

function PhotoFrameVipBtn_OnUpdate()
	-- 借用一下  小海蚌处理
	UpdateHaiBangEndTime();
	
	if not VipManager:isEnableVip( STAR_STONE_FIRE ) then
		return;
	end

	if AutoFiniQuestMiniFrame:IsShown() and miniAutoTick ~= 0 then
		if os.clock() - miniAutoTick > 8 then
			AutoFiniQuestMiniFrameUVAnimationTex:Hide();
			miniAutoTick = 0;
		end
	end

	updateAutoRecovery();
	updateAutoFiniQuest();
end