-- 取消自动组队
-- 正在创建中的上次操作面板
local t_boolDesc = { [true] = "true", [false] = "false" };
local t_creatingLastFrame = { name = "" };

function SetCreatingLastFrame( data )
	t_creatingLastFrame = data;
end

NO_RETURN_TO_LAST_FRAME = 0;
RETURN_TO_LAST_FRAME	= 1;

local t_closeFrames = { "SuccessCreatActiveAutoTeamFrame", "SuccessCreatTaskAutoTeamFrame", "CreatingAutoTeamFrame" };
function CancelAutoTeam( nOpCode )
	if CreatingAutoTeamFrame:IsShown() then
		-- TODO: 根据每个面板的操作来进行逻辑数据设置
		--SeteActiveAutoTeamFramePassTime( { startTime = 0, ready = false, autoWarn = true, autoTimeDown = false } );
		local t_hideIcons = { "WarnAutoTeamFailFrame", "WarnTimeCountDownFrame" };
		util.HideIcons( t_hideIcons );
	end

	if SuccessCreatActiveAutoTeamFrame:IsShown() then
		-- TODO: 根据每个面板的操作来进行逻辑数据设置
		WarnAutoTeamFailFrame:Hide();
	end

	for _, name in ipairs( t_closeFrames ) do
		local frame = getglobal( name );
		if frame:IsShown() then
			frame:Hide();
		end
	end
	
	print( "nOpCode = "..nOpCode.."，t_creatingLastFrame[name] = "..t_creatingLastFrame["name"] );
	if nOpCode == RETURN_TO_LAST_FRAME and t_creatingLastFrame["name"] ~= "" then
		local frame = getglobal( t_creatingLastFrame["name"] );
		frame:Show();	
		SetCurSuoXiaoFrameInfo( { name = t_creatingLastFrame["name"] } );
	end
	
	-- TODO: 这个返回的情况跟kent对下
	if nOpCode ~= RETURN_TO_LAST_FRAME then
		SetCurSuoXiaoFrameInfo( { name = "SearchTeamFrame" } );
	end

	EndPlayAutoTeamBtnEffect();
	initPreparingTimeCutInfo();
	CreatingAutoTeamFrameChatRich:SetText( "",255,255,255 )
end

function ApplyCreateTeamFirstFrame_OnLoad()
end

function ApplyCreateTeamFirstFrame_OnEvent()
end

local MAX_NO_OP_CREATING_TIME = 30;
function ApplyCreateTeamFirstFrame_OnShow()
	local t_inviteFinishingCreateControl = GetInviteFinishingCreateControl();
	local t_fonts = {	["ApplyCreateTeamFirstFrameBlackButtonDescFont"] = 
									{ value = t_inviteFinishingCreateControl["name"].."希望先创建队伍，是否同意？" },

						["ApplyCreateTeamFirstFrameBlackButtonTimeDownFont"] = 
									{ value = math.ceil( MAX_NO_OP_CREATING_TIME ).."s后不响应默认同意该完成创建请求" },
					};
	util.SetFonts( t_fonts );

	this:SetPoint( "center", "UIClient", "center", 0, 0 );
end

function ApplyCreateTeamFirstFrame_OnUpdate()
	local fCurTime = os.clock();
	local t_inviteFinishingCreateControl = GetInviteFinishingCreateControl();
	if ( fCurTime - t_inviteFinishingCreateControl["Time"] ) >= MAX_NO_OP_CREATING_TIME then
		ApplyCreateTeamFirstFrame_AcceptButton_OnClick();
	else
		local t_fonts = {	["ApplyCreateTeamFirstFrameBlackButtonTimeDownFont"] = 
									{ value = math.ceil( MAX_NO_OP_CREATING_TIME - ( fCurTime - t_inviteFinishingCreateControl["Time"] ) )..
												"s后不响应默认同意该完成创建请求" },
						};
		util.SetFonts( t_fonts );
	end
end

function ApplyCreateTeamFirstFrame_OnHide()
	 SetInviteFinishingCreateInfo( { name = "", Time = 0 } );
end

local t_inviteFinishingCreateControl = { name = "", Time = 0 };

t_inviteFinishingCreateControl["init"] = 
function ( self ) 
	self["name"] = "";
	self["Time"] = 0;
end

-- 判断该发起先完成创建操作是否有效
t_inviteFinishingCreateControl["isRequestValid"] = 
function ( self )
	for i = 1, AutoTeamManager:getPreMemberNum() do
		local memberInfo = AutoTeamManager:getPreMemberByIndex( i - 1 );
		if memberInfo.RoleName == self["name"] then
			return true;
		end
	end

	return false;
end

function GetInviteFinishingCreateControl()
	return t_inviteFinishingCreateControl;
end

function SetInviteFinishingCreateInfo( data )
	t_inviteFinishingCreateControl["name"] = data["name"];
	t_inviteFinishingCreateControl["Time"] = data["Time"];
end

function InviteFinishingCreate( szInviteName )
	SetInviteFinishingCreateInfo( { name = szInviteName, Time = os.clock() } );
	ApplyCreateTeamFirstFrame:Show();
end

function RefuseFinishingCreate( szRefuseName )
	local nPreMemberNum = AutoTeamManager:getPreMemberNum();
	local member = AutoTeamManager:getPreMemberByIndex( 0 );
	local mainplayer = ActorMgr:getMainPlayer();	
	local finishCreatingBtn = getglobal( "CreatingAutoTeamFrame_FinishCreatingBtn" );
	if not finishCreatingBtn:IsShown() then
		return;
	end

	ShowMidTips( szRefuseName.."拒绝了完成创建的邀请" );
end

-- 首先创建队伍的同意按钮
function ApplyCreateTeamFirstFrame_AcceptButton_OnClick()
	local t_inviteFinishingCreateControl = GetInviteFinishingCreateControl();
	if t_inviteFinishingCreateControl:isRequestValid() then
		AutoTeamManager:agreeCreate();
	else
		ShowMidTips( "该操作发起人已经不在，该操作失效" );
	end
	
	ApplyCreateTeamFirstFrame:Hide();
end

-- 首先创建队伍的拒绝按钮
function ApplyCreateTeamFirstFrame_RufuseOneButton_OnClick()
	local t_inviteFinishingCreateControl = GetInviteFinishingCreateControl();
	if t_inviteFinishingCreateControl:isRequestValid() then
		AutoTeamManager:disagreeCreate();
	else
		ShowMidTips( "该操作发起人已经不在，该操作失效" );
	end
	
	ApplyCreateTeamFirstFrame:Hide();
end

function WarnNotSelectDiffFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 0, 0 );
end

function ConfirmDifficultyFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 0, 0 );
end

function ConfirmDifficultyFrame_ConfirmButton_OnClick()
	 ApplyRequestAotoTeam();
	 ConfirmDifficultyFrame:Hide();
end

function ConfirmDifficultyAndCareerFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 0, 0 );
end

function ConfirmDifficultyAndCareerFrame_ConfirmButton_OnClick()
	 ApplyRequestAotoTeam();
	 ConfirmDifficultyAndCareerFrame:Hide();
end

--  提示取消自动组队的可能原因 面板的显示
local t_WarnAutoTeamFailFrameControl = { haveShowTimes = 0 };

function GetWarnAutoTeamFailFrameControl()
	return t_WarnAutoTeamFailFrameControl;
end

t_WarnAutoTeamFailFrameControl["canShow"] = 
function ( self )
	return self["haveShowTimes"] == 0;
end

t_WarnAutoTeamFailFrameControl["init"] = 
function ( self )
	self["haveShowTimes"] = 0;
end

function WarnAutoTeamFailFrame_OnShow()
	local rich = getglobal( this:GetName().."_DescRich" );
	rich:SetText( "在等待队伍组建期间，如果下副本、下线，加入其它队伍，系统会自动取消自动组队请求", 255, 229, 189 );
	this:SetPoint( "center", "UIClient", "center", 0, 125 );
	t_WarnAutoTeamFailFrameControl["haveShowTimes"] = t_WarnAutoTeamFailFrameControl["haveShowTimes"] + 1;
end

-- 副本提示倒计时开始面板显示
function WarnTimeCountDownFrame_OnShow()
	local rich = getglobal( this:GetName().."_DescRich" );
	rich:SetText( "队伍已组满了，你还未准备好！大家都在等你，你准备好了吗？如果倒计时结束时未同意，将默认你自动接受自动组队请求", 255, 229, 189 );
	this:SetPoint( "center", "UIClient", "center", 0, 0 );
end

function ConfirmQuitAutoTeamFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 0, 0 );
end

-- 点击确定退出自动组队
function ConfirmQuitAutoTeamFrame_ConfirmButton_OnClick()
	AutoTeamManager:disagreeAutoTeam();
	ConfirmQuitAutoTeamFrame:Hide();
	--[[
	local t_hideIcons = { "WarnTimeCountDownFrame", "WarnAutoTeamFailFrame", "CreatingAutoTeamFrame", "ConfirmQuitAutoTeamFrame",  };
	util.HideIcons( t_hideIcons );
	
	local frame = getglobal( t_creatingLastFrame["name"] );
	frame:Show();
	SetCreatingLastFrame( { name = "" } );
	EndPlayAutoTeamBtnEffect();
	--]]
end

-- 队伍创建中面板
local MAX_BEGIN_TIME_DOWN = 30; -- 倒计时最多30秒
-- TODO: 下线的时候要记住清空数据
local t_preparingTimeCutInfo = { startTime = 0, ready = false };
function initPreparingTimeCutInfo()
	t_preparingTimeCutInfo = { startTime = 0, ready = false };
end

--[[
local t_autoTeamPreMemState = { [CLT_PRE_MEMBER_NOTDECIDE]	= { path = "uires\\ui\\MianBan  13.dds", UV = { x = 745, y = 378, width = 25, height = 26 }, },   
								[CLT_PRE_MEMBER_AGREE]		= { path = "uires\\ui\\MianBan  13.dds", UV = { x = 773, y = 378, width = 25, height = 26  }, },
							  };
]]--

local t_autoTeamPreMemRace =	{	
									-- 战士
									[CLIENT_CAREER_WARRIOR]	= { path = "uires\\TuPianLei\\ZiDongZuDuiAnNiuTuBiao\\zhanshi.tga", 
																UV = { x = 0, y = 0, width = 64, height = 64 }, },
									
									[CLIENT_CAREER_WARRIOR_SINGLEHAND]	= { path = "uires\\TuPianLei\\ZiDongZuDuiAnNiuTuBiao\\zhanshi.tga", 
																UV = { x = 0, y = 0, width = 64, height = 64 }, },
									
									[CLIENT_CAREER_WARRIOR_DOUBLEHAND]	= { path = "uires\\TuPianLei\\ZiDongZuDuiAnNiuTuBiao\\zhanshi.tga", 
																UV = { x = 0, y = 0, width = 64, height = 64 }, },
									
									-- 法师								
									[CLIENT_CAREER_MAGICOR]	= { path = "uires\\TuPianLei\\ZiDongZuDuiAnNiuTuBiao\\fashi.tga",
																UV = { x = 0, y = 0, width = 64, height = 64 },
																},
									
									[CLIENT_CAREER_MAGICOR_SINGLEHAND]	= { path = "uires\\TuPianLei\\ZiDongZuDuiAnNiuTuBiao\\fashi.tga",
																UV = { x = 0, y = 0, width = 64, height = 64 },
																},
									
									[CLIENT_CAREER_MAGICOR_DOUBLEHAND]	= { path = "uires\\TuPianLei\\ZiDongZuDuiAnNiuTuBiao\\fashi.tga",
																UV = { x = 0, y = 0, width = 64, height = 64 },
																},
									
									-- 枪手 
									[CLIENT_CAREER_GUNER]		= { path = "uires\\TuPianLei\\ZiDongZuDuiAnNiuTuBiao\\qiangshou.tga",
																UV = { x = 0, y = 0, width = 64, height = 64 },
																},

									[CLIENT_CAREER_GUNER_SINGLEHAND]		= { path = "uires\\TuPianLei\\ZiDongZuDuiAnNiuTuBiao\\qiangshou.tga",
																UV = { x = 0, y = 0, width = 64, height = 64 },
																},

									[CLIENT_CAREER_GUNER_DOUBLEHAND]		= { path = "uires\\TuPianLei\\ZiDongZuDuiAnNiuTuBiao\\qiangshou.tga",
																UV = { x = 0, y = 0, width = 64, height = 64 },
																},
									
									-- 祭司
									[CLIENT_CAREER_CHURCH]		= { path = "uires\\TuPianLei\\ZiDongZuDuiAnNiuTuBiao\\jisi.tga",
																UV = { x = 0, y = 0, width = 64, height = 64 },
																},

									[CLIENT_CAREER_CHURCH_SINGLEHAND]		= { path = "uires\\TuPianLei\\ZiDongZuDuiAnNiuTuBiao\\jisi.tga",
																UV = { x = 0, y = 0, width = 64, height = 64 },
																},
									
									[CLIENT_CAREER_CHURCH_DOUBLEHAND]		= { path = "uires\\TuPianLei\\ZiDongZuDuiAnNiuTuBiao\\jisi.tga",
																UV = { x = 0, y = 0, width = 64, height = 64 },
																},
								};

function ClearCreatingAutoTeamFrame()
	for i = 1, MAX_TEAM_MEMBER do
		local btn = getglobal( "CreatingAutoTeamFrameMemberBtn"..i )
		btn:SetClientString("")
		local t_fonts = {	[btn:GetName() .. "ServerNameFont"] = { value = "" },
							[btn:GetName() .. "NameFont"]		= { value = "" },
							[btn:GetName() .. "RaceLvFont"]		= { value = "" },
						}
		util.SetFonts( t_fonts )
		local t_Icons = { btn:GetName() .. "hairPhoto", btn:GetName() .. "Photo", btn:GetName() .. "HairShadowTex", 
							btn:GetName() .. "BodyTex", btn:GetName() .. "CaptionTex", btn:GetName() .. "BoxTexture" }
		util.HideIcons( t_Icons )
	end
	local t_fonts = {	["CreatingAutoTeamFrameCommonCopyBtnFont"] = { value = "" },
						};
	util.SetFonts( t_fonts )
end

function GetPlayerName( szName )
	local pos = string.find( szName,"$",1,true )
	if pos then
		return string.sub( szName,pos + 1 )
	end
	return szName
end
function UpdateCreatingAutoTeamFrame()
	ClearCreatingAutoTeamFrame();
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local nBtnIndex = 1;
	for i = 1, AutoTeamManager:getPreMemberNum() do
		if nBtnIndex > MAX_TEAM_MEMBER then
			break;
		end

		local memberInfo = AutoTeamManager:getPreMemberByIndex( i - 1 );
		local btn = getglobal( "CreatingAutoTeamFrameMemberBtn"..i );
		local serverid = ActorMgr:getServerId( memberInfo.RoleName ) ~= -1 and ActorMgr:getServerId( memberInfo.RoleName ) or mainplayer:GetWorldID();
		local t_fonts = {	[btn:GetName() .. "ServerNameFont"] = { value = LoginMgr:getWorldNameById(serverid) },
							[btn:GetName() .. "NameFont"]		= { value = GetPlayerName(memberInfo.RoleName) },
							[btn:GetName() .. "RaceLvFont"]		= { value = memberInfo.Level .. "级 " .. GetCareerName( memberInfo.Career ) },
						};
		util.SetFonts( t_fonts )
		local t_Icons = { btn:GetName() .. "hairPhoto", btn:GetName() .. "Photo", btn:GetName() .. "HairShadowTex", btn:GetName() .. "BodyTex", btn:GetName() .. "BoxTexture" }
		-- 第一位显示队长标记
		if i == 1 then
			table.insert( t_Icons,btn:GetName() .. "CaptionTex" )
		end

		SetPhoto( memberInfo.Hair, memberInfo.Face, btn:GetName() .. "hairPhoto", btn:GetName() .. "Photo", memberInfo.Gender , memberInfo.Head,
				btn:GetName() .. "HairShadowTex", btn:GetName() .. "BodyTex" );
				
		util.ShowIcons( t_Icons )
		btn:SetClientString( memberInfo.RoleName );
		btn:Show();
		nBtnIndex = nBtnIndex + 1;
	end

	ChangeFinishCreatingBtn();
	
	local nAutoTeamID = AutoTeamManager:getAutoTeamID();
	if nAutoTeamID <= 0 then
		return;
	end

	-- 设置共同项目
	local autoTeamData	= GamePromotion:getAutoTeamData( nAutoTeamID );
	local szText = "";
	if autoTeamData.proType == CLT_AUTO_TEAM_ACTIVE then
		szText = "共同项目："..PworldDifficulty:getDiffPworldName(autoTeamData.relateID).."副本";
	elseif autoTeamData.proType == CLT_AUTO_TEAM_TASK then
		szText = "共同项目："..Quest:getQuestName(autoTeamData.relateID).."任务";
	end

	local t_fonts = {	["CreatingAutoTeamFrameCommonCopyBtnFont"] = { value = szText },
						};
	util.SetFonts( t_fonts );
end

local t_creatingAutoTeamFrameEvents = { ["GE_AUTO_TEAM_PRE_MEM_ADD"]		= {},	["GE_AUTO_TEAM_PRE_MEM_DEC"]	= {}, 
										["GE_AUTO_TEAM_PRE_MEM_STATE_CHG"]	= {},	["GE_AUTO_TEAM_SUCCESS"]		= {},
										["GE_AUTO_TEAM_PRE_MEM_PRE_ADD"]	= {},	["GE_TEAM_APPLYSUCC"]			= {},
										["GE_TEAM_LEAVETEAM"]				= {},	["GE_PROMOTION_UPDATE"]			= {},
										["GE_TEAM_NEW_PLAYER_JOIN"]			= {},	["GE_TEAM_ONE_PLAYER_LEAVE"]	= {},
										["GE_AUTOTEAM_GETMSG"]				= {},	["GE_AUTOTEAM_GETMSG_BY_CLIENT"] = {},
									  };

t_creatingAutoTeamFrameEvents["GE_AUTO_TEAM_PRE_MEM_PRE_ADD"].func = 
function ()
	local creatingFrame			= getglobal( "CreatingAutoTeamFrame" );
	local t_curSuoXiaoFrame		= GetCurSuoXiaoFrameInfo();
	local t_allAutoTeamActives	= GetAllAutoTeamActivesControl();
	-- 若是第一次开始自动组队，并且进入自动组队等待队列，则要自动弹出等待队列面板
	-- 若是队长新发起的自动组队，那么队长也要弹出自动组队等待队列的面板
	if not creatingFrame:IsShown() and ( AutoTeamManager:getPreMemberNum() == 0 or 
			( t_allAutoTeamActives:GetContinueAutoTeam() and t_curSuoXiaoFrame["name"] == "SearchTeamFrame" ) ) then
		-- 提示退出自动组队的可能原因
		local t_WarnAutoTeamFailFrameControl = GetWarnAutoTeamFailFrameControl();
		if t_WarnAutoTeamFailFrameControl:canShow() then
			--WarnAutoTeamFailFrame:Show();
		end

		creatingFrame:Show();
	end
end

t_creatingAutoTeamFrameEvents["GE_PROMOTION_UPDATE"].func = 
function ()
	UpdatePromotionTimeState();
end

t_creatingAutoTeamFrameEvents["GE_TEAM_APPLYSUCC"].func = 
function ()
	local creatingFrame = getglobal( "CreatingAutoTeamFrame" );
	
	if creatingFrame:IsShown() then
		UpdateCreatingAutoTeamFrame();
	end
end

t_creatingAutoTeamFrameEvents["GE_TEAM_LEAVETEAM"].func = 
function ()
	t_creatingAutoTeamFrameEvents["GE_TEAM_APPLYSUCC"].func();
end


t_creatingAutoTeamFrameEvents["GE_AUTO_TEAM_PRE_MEM_ADD"].func = 
function ()
	local creatingFrame = getglobal( "CreatingAutoTeamFrame" );
	
	if creatingFrame:IsShown() then
		UpdateCreatingAutoTeamFrame();
	end
	
	local t_lastRequestFinishCreating = GetLastRequestFinishCreating();
	if t_lastRequestFinishCreating["haveBegin"] then
		t_lastRequestFinishCreating["haveBegin"]	= false;
		t_lastRequestFinishCreating["Time"]			= 0;
	end

	-- 若人数已满，则开始倒计时
	-- TODO: 暂时更改	MAX_TEAM_MEMBER
	if AutoTeamManager:getPreMemberNum() == MAX_TEAM_MEMBER then
		if not creatingFrame:IsShown() then
			creatingFrame:Show();
		end
		
		--local nSelfState = AutoTeamManager:getMainPlayerAgreeState();
		--[[
		t_preparingTimeCutInfo = { startTime = os.clock(), ready = true };
		local processTex = getglobal( "CreatingAutoTeamFrameProcessBtnTex" );
		processTex:SetSize( 444, 14 );
		WarnTimeCountDownFrame:Show();
		]]--
	else
		CreatingAutoTeamFrame_StopTimeCountDown();
	end
end

t_creatingAutoTeamFrameEvents["GE_AUTO_TEAM_PRE_MEM_DEC"].func = 
function ()
	local creatingFrame = getglobal( "CreatingAutoTeamFrame" );
	
	if creatingFrame:IsShown() then
		UpdateCreatingAutoTeamFrame();
	end

	CreatingAutoTeamFrame_StopTimeCountDown();
end

function CreatingAutoTeamFrame_StopTimeCountDown()
	-- 若正在倒计时，则停止倒计时
	if t_preparingTimeCutInfo["ready"] then
		t_preparingTimeCutInfo = { startTime = 0, ready = false };
		local processTex = getglobal( "CreatingAutoTeamFrameProcessBtnTex" );
		processTex:SetSize( 444, 14 );
	end
end

t_creatingAutoTeamFrameEvents["GE_AUTO_TEAM_PRE_MEM_STATE_CHG"].func = 
function ()
	local creatingFrame = getglobal( "CreatingAutoTeamFrame" );
	
	if creatingFrame:IsShown() then
		UpdateCreatingAutoTeamFrame();
	end
end

t_creatingAutoTeamFrameEvents["GE_TEAM_NEW_PLAYER_JOIN"].func = 
function ()
	t_creatingAutoTeamFrameEvents["GE_AUTO_TEAM_PRE_MEM_STATE_CHG"].func();
end

t_creatingAutoTeamFrameEvents["GE_TEAM_ONE_PLAYER_LEAVE"].func = 
function ()
	t_creatingAutoTeamFrameEvents["GE_AUTO_TEAM_PRE_MEM_STATE_CHG"].func();
	CreatingAutoTeamFrame_StopTimeCountDown();
end

t_creatingAutoTeamFrameEvents["GE_AUTOTEAM_GETMSG"].func = 
function ()
	local msg			= UIMSG:getUIMsg();
	local nType			= msg.ChatMsg.Type;
	local szSpeakerName	= msg.ChatMsg.Speaker;
	local szChatContent	= msg.ChatMsg.Content;			
	local rich = getglobal( "CreatingAutoTeamFrameChatRich" )
	rich:AddText(  "#A018" .. szSpeakerName .."说:" .. szChatContent, 255, 255, 190 );
	rich:ScrollEnd();
	local slider	= getglobal("CreatingAutoTeamFrame_ScrollBar");
	SetScrollBar( slider:GetName(), rich:GetAccurateViewLines(), rich:GetTextLines() );
	slider:SetValue( slider:GetMaxValue() );
	util.UpdateRich( rich:GetName(), slider:GetValue() );
end

t_creatingAutoTeamFrameEvents["GE_AUTOTEAM_GETMSG_BY_CLIENT"].func = 
function ()
	local msg			= UIMSG:getUIMsg();
	local nType			= msg.ChatReq.Type;
	local szSpeakerName	= msg.ChatReq.ToName;
	local szChatContent	= msg.ChatReq.Content;			
	local rich = getglobal( "CreatingAutoTeamFrameChatRich" )
	rich:AddText(  "#A018你说:" .. szChatContent, 255, 255, 190 );
	rich:ScrollEnd();
	local slider	= getglobal("CreatingAutoTeamFrame_ScrollBar");
	SetScrollBar( slider:GetName(), rich:GetAccurateViewLines(), rich:GetTextLines() );	
	slider:SetValue( slider:GetMaxValue() );
	util.UpdateRich( rich:GetName(), slider:GetValue() );
end

-- 若是在副本中的队长发起的，1.若队长在副本内无需弹出这个界面传送进副本界面，且要设置其缩小界面是招人的新界面，2.若队长在副本外面
-- 则弹出传送界面
-- 若是上次自动组队招进来的人，则不作任何反应，上次自动组队招进来的人缩小的界面比不为"CreatingAutoTeamFrame"界面
-- 若是新加入队伍的人，则弹出立即传送界面
-- 自动寻组成功后切换操作界面

-- 会弹出自动组队等待界面的有这些人：是只有发起自动组队的队长和散人吗？
function UpdateSuccessActiveAutoTeamFrames()
	local mainplayer		= ActorMgr:getMainPlayer();
	local nCurMapID			= MapShower:getRealMapId();		
	local t_curSuoXiaoFrame	= GetCurSuoXiaoFrameInfo(); 
	-- 若是队长
	local firstPreMember = AutoTeamManager:getPreMemberByIndex( 0 )
	if mainplayer:isCaptain() or CompareRoleName( mainplayer:getName(), firstPreMember.RoleName ) then
		-- 判断是否是消耗精力的副本
		if nCurMapID < MIN_COPY_ID then			
			local autoTeamData = GamePromotion:getAutoTeamData( AutoTeamManager:getAutoTeamID() );
			local pworldid = PworldDifficulty:getDifficultyPworldId( autoTeamData.relateID, AutoTeamManager:getPworldDiffulty() )
			local pworldDef = getPworldDef( pworldid );
			if pworldDef ~= nil and pworldDef.EnterUseVig > 0 then
				AutoTeamManager:tranToPWorld();
			end
			local frame = getglobal( "SuccessCreatActiveAutoTeamFrame" );
			frame:Show();
		else
			if t_curSuoXiaoFrame["name"] == "SearchTeamFrame" then
				ShowMidTips( "自动组队成功" );
			end

			SetCurSuoXiaoFrameInfo( { name = "SearchTeamFrame" } );
		end
	-- 若是队员
	else
		if t_curSuoXiaoFrame["name"] == "CreatingAutoTeamFrame" or t_curSuoXiaoFrame["name"] == "SearchTeamFrame" then
			-- 若是上次自动组队招进来的人
			if nCurMapID < MIN_COPY_ID then
				local frame = getglobal( "SuccessCreatActiveAutoTeamFrame" );
				frame:Show();
			end
		end
	end
end

t_creatingAutoTeamFrameEvents["GE_AUTO_TEAM_SUCCESS"].func = 
function ()
	local t_hideIcons = { "WarnTimeCountDownFrame", "WarnAutoTeamFailFrame", "CreatingAutoTeamFrame", "SearchTeamFrame" };
	util.HideIcons( t_hideIcons );

	local autoTeamData = GamePromotion:getAutoTeamData( AutoTeamManager:getAutoTeamID() );
	if autoTeamData.proType == CLT_AUTO_TEAM_ACTIVE then
		--[[
		SeteActiveAutoTeamFramePassTime( { startTime = os.clock(), ready = true, autoWarn = true } );
		local frame = getglobal( "SuccessCreatActiveAutoTeamFrame" );
		frame:Show();
		--]]
		UpdateSuccessActiveAutoTeamFrames();
		SetLastRequestFinishCreating( { Time = 0, haveBegin = false } );
	elseif autoTeamData.proType == CLT_AUTO_TEAM_TASK then
		local frame = getglobal( "SuccessCreatTaskAutoTeamFrame" );
		frame:Show();
	end
	
	-- 清空上次操作打开的面板
	SetCreatingLastFrame( { name = "" } );
	local t_allAutoTeamActives = GetAllAutoTeamActivesControl();
	t_allAutoTeamActives:SetResetFlag( true );
	t_allAutoTeamActives:SetContinueAutoTeam( false );
	initPreparingTimeCutInfo();
	CreatingAutoTeamFrameChatRich:SetText( "",255,255,255 )
end

function CreatingAutoTeamFrame_OnLoad()
	for event, _ in pairs( t_creatingAutoTeamFrameEvents ) do
		this:RegisterEvent( event );
	end

	t_creatingAutoTeamFrameEvents.__index = function ( tab, key )
									tab[key] = { func = function () end };
									return tab[key];
								end
	setmetatable( t_creatingAutoTeamFrameEvents, t_creatingAutoTeamFrameEvents );
end

function CreatingAutoTeamFrame_OnEvent()
	t_creatingAutoTeamFrameEvents[arg1].func();
end

function UpdatePromotionTimeState()
	-- 对于目前就主角一人的情况
	if AutoTeamManager:getAutoTeamMemberNum() == 1 then
		local mainplayer	= ActorMgr:getMainPlayer();
		local preMemberInfo = AutoTeamManager:getPreMember( mainplayer:getName() );
		local allApplyInfo	= preMemberInfo.AutoTeamApplyInfo;
		for i = 1, allApplyInfo.AutoTeamIDNum do
			local oneApplyInfo	= allApplyInfo.AutoTeamIDInfos[i-1];
			local autoTeamData	= GamePromotion:getAutoTeamData( oneApplyInfo.AutoTeamID );
			if autoTeamData.proType == CLT_AUTO_TEAM_ACTIVE then
				local startInfo = GamePromotion:getCurrPromitionStartInfo( autoTeamData.nRow );
				if startInfo.startFlag == PROMOTION_RUNING then
					return;
				end
			end
		end

		-- 拒绝
		ShowMidTips( "你选择的所有活动都已经结束，取消自动组队" );
		AutoTeamManager:disagreeAutoTeam();

		local t_hideIcons = { "WarnTimeCountDownFrame", "WarnAutoTeamFailFrame", "CreatingAutoTeamFrame", "ConfirmQuitAutoTeamFrame",  };
		util.HideIcons( t_hideIcons );
		SetCurSuoXiaoFrameInfo( { name = "SearchTeamFrame" } );
	end

	-- 对于有多人的情况
	local nAutoTeamID = AutoTeamManager:getAutoTeamID();
	if nAutoTeamID <= 0 then
		return;
	end

	-- 设置共同项目
	local autoTeamData = GamePromotion:getAutoTeamData( nAutoTeamID );
	if autoTeamData.proType == CLT_AUTO_TEAM_ACTIVE then
		local startInfo = GamePromotion:getCurrPromitionStartInfo( autoTeamData.nRow );
		if startInfo.startFlag == PROMOTION_RUNING then
			return;
		end
		
		-- 拒绝
		ShowMidTips( "你所要参加活动已经结束，取消自动组队" );
		AutoTeamManager:disagreeAutoTeam();
		local t_hideIcons = { "WarnTimeCountDownFrame", "WarnAutoTeamFailFrame", "CreatingAutoTeamFrame" };
		util.HideIcons( t_hideIcons );
		SetCurSuoXiaoFrameInfo( { name = "SearchTeamFrame" } );
		t_preparingTimeCutInfo = { startTime = 0, ready = false };
	end
end

function CreatingAutoTeamFrame_OnUpdate()
	UpdateProcessTex();
end

function UpdateProcessTex()
	if not t_preparingTimeCutInfo["ready"] then
		return;
	end

	local processTex = getglobal( "CreatingAutoTeamFrameProcessBtnTex" );
	-- 根据时间设置进度条
	local fPassedTime	= os.clock() - t_preparingTimeCutInfo["startTime"];
	if fPassedTime > MAX_BEGIN_TIME_DOWN then
		processTex:SetSize( 444, 12 );
		t_preparingTimeCutInfo = { startTime = 0, ready = false };
		-- 若已经在队伍中，则无需验证是否处于同意状态了
		--[[
		local mainplayer = ActorMgr:getMainPlayer();
		if not mainplayer:isInTeam() and not AutoTeamManager:canAgreeAutoTeam() then
			return;
		end
		--]]

		AutoTeamManager:agreeAutoTeam();
		return;
	end

	local fPassScale	= fPassedTime / MAX_BEGIN_TIME_DOWN;
	processTex:SetSize( 444 - fPassScale * processTex:GetWidth(), 14 );
end

function CreatingAutoTeamFrame_OnHide()
end

-- 创建队伍中的右键菜单选项
local t_creatingAutoTeamRightOp = { "复制名称", "观察对方", "取消" };
local t_mutexFrames = { "FriendFrame_RightOptionFrame" };
function CreatingAutoTeamRightMenuFrame_OnShow()
	for i = 1, table.getn( t_creatingAutoTeamRightOp ) do
		local font = getglobal( "CreatingAutoTeamRightMenuFrame_RightBtn"..i.."Font" );
		font:SetText( t_creatingAutoTeamRightOp[i] );
		font:SetTextColor( 145, 125, 55 );
	end
	util.AdjustTooltipsScreenPos( { tips = this:GetName() } );

	for _, name in ipairs( t_mutexFrames ) do
		local frame = getglobal( name );
		if frame:IsShown() then
			frame:Hide();
		end
	end
end

-- 鼠标移到队伍创建中的面板的职业图标
function AutoTeamRaceButton_OnEnter()
	if this:GetClientString() == "" then
		return;
	end
	
	SetAutoTeamTipsInfo( { playerName = this:GetClientString(), frameName = "CreatingAutoTeamFrame", button = this:GetName() } );
	AutoTeamMemberTipsFrame:Show();
	util.AdjustBigtipsScreenPos( { tipsFrame = "AutoTeamMemberTipsFrame", button = this:GetName() } );
end

function AutoTeamRaceButton_OnLeave()
	AutoTeamMemberTipsFrame:Hide();
end

function AutoTeamRaceButton_OnClick()
	if arg1 ~= "RightButton" then
		return;
	end

	local szMemberName = this:GetClientString();
	local mainplayer = ActorMgr:getMainPlayer();	
	-- TODO: 若不用左键，都用右键
	if szMemberName == "" or CompareRoleName( szMemberName, mainplayer:getName() ) then
		return;
	end

	SetRightFrameInfo( { name = szMemberName, relationType = CLIENT_RELATION_TYPE_NONE, nHeight = 0,
								nLow = 0, id = 0, frame = "CreatingAutoTeamRightMenuFrame", watchIgnorDist = true } );
	CreatingAutoTeamRightMenuFrame:Show();
end

local t_autoTeamTipsInfo = { playerName = "", frameName = "CreatingAutoTeamFrame", button = "" };

function SetAutoTeamTipsInfo( data )
	t_autoTeamTipsInfo = data;
end

--									
t_sex = {	[CLIENT_GENDER_MALE]	= { desc = "男", maxHair = 13,  maxFace = 8,  maxHead = 7, maxClothes = 2, }, 
			[CLIENT_GENDER_FEMALE]	= { desc = "女", maxHair = 13,  maxFace = 8,  maxHead = 7, maxClothes = 2, },
		};
local t_prepareState = { [CLT_PRE_MEMBER_NOTDECIDE] = { desc = "未同意"}, [CLT_PRE_MEMBER_AGREE] = { desc = "同意"}, };

local t_allLimit = {	[CLT_AUTO_TEAM_LIMIT_WARRIOR]	= { desc = "战士", chkBtn = "SearchTeamFrame_AutoTeamRaceChkBtn1" }, 
						[CLT_AUTO_TEAM_LIMIT_MAGICOR]	= { desc = "法师", chkBtn = "SearchTeamFrame_AutoTeamRaceChkBtn2" },
						[CLT_AUTO_TEAM_LIMIT_GUNER]		= { desc = "枪手", chkBtn = "SearchTeamFrame_AutoTeamRaceChkBtn3" }, 
						[CLT_AUTO_TEAM_LIMIT_CHUNCH]	= { desc = "祭司", chkBtn = "SearchTeamFrame_AutoTeamRaceChkBtn4" }, 
						[CLT_AUTO_TEAM_LIMIT_LEADER]	= { desc = "队长", chkBtn = "SearchTeamFrame_AutoTeamRaceChkBtn5" }, 
					};
-- 职业tips面板
function AutoTeamMemberTipsFrame_OnShow()
	local preMemberInfo = AutoTeamManager:getPreMember( t_autoTeamTipsInfo["playerName"] );
	local t_nowRequest = {};
	for request, _ in pairs( t_allLimit ) do
		if bitAnd( preMemberInfo.AutoTeamApplyInfo.LimitFlag, request ) then
			table.insert( t_nowRequest, request );
		end
	end
	local szRequest = "";
	for _, request in ipairs( t_nowRequest ) do
		szRequest = szRequest..t_allLimit[request]["desc"].."  ";
	end
	
	local szCommon = "";
	if AutoTeamManager:getAutoTeamID() > 0 then
		local autoTeamData = GamePromotion:getAutoTeamData( AutoTeamManager:getAutoTeamID() );
		if autoTeamData.proType == CLT_AUTO_TEAM_TASK then
			szCommon = szCommon..Quest:getQuestName( autoTeamData.relateID );
		else
			szCommon = szCommon..PworldDifficulty:getDiffPworldName(autoTeamData.relateID);
		end
	end

	local t_fonts = {	["AutoTeamMemberTipsFrameNameFont"]				= { value = "名字："..preMemberInfo.RoleName },
						["AutoTeamMemberTipsFrameLevelFont"]			= { value = "等级："..preMemberInfo.Level.."级" },
						["AutoTeamMemberTipsFrameRaceFont"]				= { value = "职业："..t_raceList[preMemberInfo.Career] },
						--["AutoTeamMemberTipsFrameAreaFont"]				= { value = "游戏区：" },
						["AutoTeamMemberTipsFrameSexFont"]				= { value = "性别："..t_sex[preMemberInfo.Gender]["desc"] },
						--["AutoTeamMemberTipsFrameReadyStateFont"]		= { value = "状态："..t_prepareState[preMemberInfo.AgreeStat]["desc"] },
						["AutoTeamMemberTipsFramePreRequestFont"]		= { value = "组队要求："..szRequest },
						["AutoTeamMemberTipsFrameContentFont"]			= { value = "组队项目："..szCommon },
					};
	util.SetFonts( t_fonts );
	SetPhoto( preMemberInfo.Hair, preMemberInfo.Face, "AutoTeamMemberTipsFramePhotoBtnHairTex", 
				"AutoTeamMemberTipsFramePhotoBtnFaceTex", preMemberInfo.Gender, preMemberInfo.Head, 
				"AutoTeamMemberTipsFramePhotoBtnHairShadowTex", "AutoTeamMemberTipsFramePhotoBtnBodyTex" );
end

function AutoTeamMemberTipsFrame_OnHide()
	t_autoTeamTipsInfo = { playerName = "", frameName = "CreatingAutoTeamFrame", button = "" };
end

function AutoTeamMemberTipsFrame_OnUpdate()
	--GameExchange:extraTest( "t_autoTeamTipsInfo[button] = "..t_autoTeamTipsInfo["button"] );
	local button = getglobal( t_autoTeamTipsInfo["button"] );
	if button:GetClientString() ~= t_autoTeamTipsInfo["playerName"] then
		this:Hide();
	end

	local frame = getglobal( t_autoTeamTipsInfo["frameName"] );
	if not frame:IsShown() then
		this:Hide();
	end
end

function ChangeFinishCreatingBtn()
	local mainplayer		= ActorMgr:getMainPlayer();	
	local finishCreatingBtn = getglobal( "CreatingAutoTeamFrame_FinishCreatingBtn" );

	-- 设置是否显示同意取消按钮
	--local acceptBtn = getglobal( "CreatingAutoTeamFrame_AcceptBtn" );
	local refuseBtn = getglobal( "CreatingAutoTeamFrame_RefuseBtn" );
	--print( "ChangeFinishCreatingBtn--754" );
	if mainplayer:isInTeam() then
		if mainplayer:isCaptain() then
			--acceptBtn:Hide();
			refuseBtn:Show();
		else
			--acceptBtn:Hide();
			refuseBtn:Hide();
		end
	else
		--acceptBtn:Show();
		refuseBtn:Show();
	end

	if AutoTeamManager:getPreMemberNum() <= 1 then
		finishCreatingBtn:Hide();
		return;
	end
	
	-- 设置是否显示完成创建按钮
	local firstPreMember	= AutoTeamManager:getPreMemberByIndex( 0 );

	local needShowfinishCreatingBtn = 
	function ()
		local mainplayer		= ActorMgr:getMainPlayer();	
		--[[
		if mainplayer:isInTeam() then
			local TeamInfo	= TeamManager:getTeamInfo();
			if AutoTeamManager:getPreMemberNum() == TeamInfo.MemberNum then
				return false;
			end
		end
		--]]
		
		local preMemberInfo = AutoTeamManager:getPreMember( mainplayer:getName() );
		local allApplyInfo	= preMemberInfo.AutoTeamApplyInfo;
		--[[
		if bitAnd( allApplyInfo.LimitFlag, CLT_AUTO_TEAM_LIMIT_LEADER ) then
			return true;
		end
		
		for i = 1, AutoTeamManager:getPreMemberNum() do
			local applyMemberInfo = AutoTeamManager:getPreMemberByIndex( i - 1 );
			if bitAnd( applyMemberInfo.AutoTeamApplyInfo.LimitFlag, CLT_AUTO_TEAM_LIMIT_LEADER )  then
				return false;
			end
		end
		--]]
		--print( "firstPreMember.RoleName = "..firstPreMember.RoleName );
		if CompareRoleName( mainplayer:getName(), firstPreMember.RoleName ) then
			return true;
		end

		return false;
	end
	
	if needShowfinishCreatingBtn() then
		finishCreatingBtn:Show();
	else
		finishCreatingBtn:Hide();
	end
end

local t_creatingAutoTeamFrameMutexFrames = { "PromotionFrame", "SuccessCreatTaskAutoTeamFrame", "SearchTeamFrame", 
												"SuccessCreatActiveAutoTeamFrame" };

function CreatingAutoTeamFrame_OnShow()
	for _, frameName in ipairs( t_creatingAutoTeamFrameMutexFrames ) do
		local frame = getglobal( frameName );
		if frame:IsShown() then
			SetCreatingLastFrame( { name = frameName } );
			frame:Hide();
		end
	end

	UpdateCreatingAutoTeamFrame();
	
	--[[
	local processTex = getglobal( "CreatingAutoTeamFrameProcessBtnTex" );
	if t_preparingTimeCutInfo["ready"] then
		UpdateProcessTex();
	else
		processTex:SetSize( 444, 14 );
	end
	]]--
	
	this:SetPoint( "center", "UIClient", "center", 0, -80 );
	SetCurSuoXiaoFrameInfo( { name = this:GetName() } );

	local rich		 = getglobal("CreatingAutoTeamFrameChatRich");
	local slider	= getglobal("CreatingAutoTeamFrame_ScrollBar");
	SetScrollBar( slider:GetName(), rich:GetAccurateViewLines(), rich:GetTextLines() );	
	util.UpdateRich( rich:GetName(), slider:GetValue() );
end

-- 点击缩小按钮
function CreatingAutoTeamFrame_MinizeBtn_OnClick()
	local frame = getglobal( "CreatingAutoTeamFrame" );
	frame:Hide();
	BeginPlayAutoTeamBtnEffect();
end

function CreatingAutoTeamFrame_MinizeBtn_OnEnter()
	local szText = "隐藏窗口：点击此按键可隐藏组建队伍窗口，需要打开时请点击小地图上的自动组队按钮即可打开";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function CreatingAutoTeamFrame_MinizeBtn_OnLeave()
	GameTooltip:Hide();
end

local t_lastRequestFinishCreating = { haveBegin = false, Time = 0 };
local MIN_FINSH_CREATIN_CD = 10;
t_lastRequestFinishCreating["init"] = 
function ( self )
end

function SetLastRequestFinishCreating( data )
	t_lastRequestFinishCreating = data;
end

function GetLastRequestFinishCreating( )
	return t_lastRequestFinishCreating;
end

-- 点击完成创建按钮响应
function CreatingAutoTeamFrame_FinishCreatingBtn_OnClick()
	-- 若在队伍中，则不是队长就不行点
	local mainplayer = ActorMgr:getMainPlayer();
	local fCurTime = os.clock();
	local t_lastFinishCreatingInfo = GetLastRequestFinishCreating();
	if t_lastFinishCreatingInfo["haveBegin"] and fCurTime - t_lastFinishCreatingInfo["Time"] < MIN_FINSH_CREATIN_CD then
		ShowMidTips( "在发起完成创建后的"..MIN_FINSH_CREATIN_CD.."s内不能再次发起完成创建的请求" );
		return;
	end
	--[[
	if AutoTeamManager:getMainPlayerAgreeState() ~= CLT_PRE_MEMBER_AGREE then
		ShowMidTips( "请先同意自动组队，才能发起完成创建的操作" );
		return;
	end
	--]]

	AutoTeamManager:requestFinishCreate();
	SetLastRequestFinishCreating( { Time = os.clock(), haveBegin = true } );
end

-- 点击完同意按钮响应
function CreatingAutoTeamFrame_AcceptBtn_OnClick()
	AutoTeamManager:agreeAutoTeam();

	local t_hideIcons = { "WarnTimeCountDownFrame", };
	util.HideIcons( t_hideIcons );
end

function CreatingAutoTeamFrame_AcceptBtn_OnEnter()
	local szText = "成员满时所有成员需同意好才进行组队，超时没同意者默认同意，打勾的表示已同意，打问号表示未同意，在等待组队时间内，下副本，下线，系统将会自动取消组队请求";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function CreatingAutoTeamFrame_AcceptBtn_OnLeave()
	GameTooltip:Hide();
end

-- 点击完取消按钮响应
function CreatingAutoTeamFrame_RefuseBtn_OnClick()
	--AutoTeamManager:disagreeAutoTeam();
	ConfirmQuitAutoTeamFrame:Show();
end

-- 提示任务自动组队成功
local t_successTaskAutoTeamFrameEvents = { ["GE_TEAM_LEAVETEAM"] = {}, ["GE_TEAM_NEW_PLAYER_JOIN"] = {}, ["GE_TEAM_ONE_PLAYER_LEAVE"] = {}, 
											["GE_TEAM_APPLYSUCC"] = {},["GE_TEAM_GET_TEAMINFO"]	= {},
										};

t_successTaskAutoTeamFrameEvents["GE_TEAM_LEAVETEAM"].func = 
function ()
	local t_curSuoXiaoFrame = GetCurSuoXiaoFrameInfo();
	
	if t_curSuoXiaoFrame["name"] ~= "SuccessCreatTaskAutoTeamFrame" then
		return;
	end
	
	local frame = getglobal( "SuccessCreatTaskAutoTeamFrame" );
	if frame:IsShown() then
		frame:Hide();
	end

	SetCurSuoXiaoFrameInfo( { name = "SearchTeamFrame" } );
end

t_successTaskAutoTeamFrameEvents["GE_TEAM_NEW_PLAYER_JOIN"].func = 
function ()
	local frame = getglobal( "SuccessCreatTaskAutoTeamFrame" );
	if frame:IsShown() then
		UpdateSuccessCreatTaskAutoTeamFrame();
	end
end

t_successTaskAutoTeamFrameEvents["GE_TEAM_ONE_PLAYER_LEAVE"].func = 
function ()
	t_successTaskAutoTeamFrameEvents["GE_TEAM_NEW_PLAYER_JOIN"].func();
end

t_successTaskAutoTeamFrameEvents["GE_TEAM_APPLYSUCC"].func = 
function ()
	t_successTaskAutoTeamFrameEvents["GE_TEAM_NEW_PLAYER_JOIN"].func();
end

t_successTaskAutoTeamFrameEvents["GE_TEAM_GET_TEAMINFO"].func = 
function ()
	t_successTaskAutoTeamFrameEvents["GE_TEAM_NEW_PLAYER_JOIN"].func();
end


function ClearSuccessCreatTaskAutoTeamFrame()
	for i = 1, MAX_TEAM_MEMBER do
		local btn = getglobal( "SuccessCreatTaskAutoTeamFrameMemberInfoBtn"..i );
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = "" },
							[btn:GetName().."RaceFont"]		= { value = "" },
							[btn:GetName().."LevelFont"]	= { value = "" },
						};
		util.SetFonts( t_fonts );
	end
end

function UpdateSuccessCreatTaskAutoTeamFrame()
	ClearSuccessCreatTaskAutoTeamFrame();
	local nBtnIndex		= 1;
	local mainplayer	= ActorMgr:getMainPlayer();
	
	local teamInfo		= TeamManager:getTeamInfo();
	local nSelfPosition = TEAM_POSITION_MEMBER;
	for i = 1, teamInfo.MemberNum do
		if nBtnIndex > MAX_TEAM_MEMBER then
			break;
		end

		local teamMember = teamInfo.TeamMember[i-1];
		local btn = getglobal( "SuccessCreatTaskAutoTeamFrameMemberInfoBtn"..nBtnIndex );
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = GetPlayerName(teamMember.MemberName) },
							[btn:GetName().."RaceFont"]		= { value = t_raceList[teamMember.Career] },
							[btn:GetName().."LevelFont"]	= { value = teamMember.Level.."级" },
						};
		util.SetFonts( t_fonts );
		if teamMember.Position == TEAM_POSITION_CAPTAIN then
			local captainBtn = getglobal( "SuccessCreatTaskAutoTeamFrame_CaptainBtn" );
			captainBtn:SetPoint( "topright", btn:GetName(), "topleft", -11, 0 );
		end

		if CompareRoleName( teamMember.MemberName, mainplayer:getName() ) then
			nSelfPosition = teamMember.Position;
		end

		nBtnIndex = nBtnIndex + 1;
	end
	
	local autoTeamData	= GamePromotion:getAutoTeamData( AutoTeamManager:getAutoTeamID() );
	local npcInfo		= MapShower:getNpcInfoById( autoTeamData.mapID, autoTeamData.npcID )
	local t_riches = { ["SuccessCreatTaskAutoTeamFrame_DescRich"] = { 
		value = "      注：队伍所有成员必须在队长的5米范围内才可参加活动，请前往#L"..npcInfo.m_szName.."#n处。", 
		color = { r = 255, g = 184, b = 0 } }, 
		};
	util.SetRiches( t_riches );
	
	
	local t_fonts = {	["SuccessCreatTaskAutoTeamFrameSelfPositionBtnFont"]		= { value = t_teamPosition[nSelfPosition]["desc"] },
						};
	util.SetFonts( t_fonts );
end

function CheckCloseSuccessCreatTaskAutoTeamFrame( nTaskID )
	local t_curSuoXiaoFrame = GetCurSuoXiaoFrameInfo();
	if t_curSuoXiaoFrame["name"] ~= "SuccessCreatTaskAutoTeamFrame" then
		return;
	end

	local nAutoTeamID = AutoTeamManager:getAutoTeamID();
	assert( nAutoTeamID > 0 );
	local autoTeamData = GamePromotion:getAutoTeamData( nAutoTeamID );
	
	if autoTeamData.relateID == nTaskID then
		local frame = getglobal( "SuccessCreatTaskAutoTeamFrame" );
		frame:Hide();
		SetCurSuoXiaoFrameInfo( { name = "SearchTeamFrame" } );
	end
end

function SuccessCreatTaskAutoTeamFrame_OnLoad()
	for event, _ in pairs( t_successTaskAutoTeamFrameEvents ) do
		this:RegisterEvent( event );
	end

	t_successTaskAutoTeamFrameEvents.__index = function ( tab, key )
									tab[key] = { func = function () end };
									return tab[key];
								end
	setmetatable( t_successTaskAutoTeamFrameEvents, t_successTaskAutoTeamFrameEvents );
end

function SuccessCreatTaskAutoTeamFrame_OnEvent()
	t_successTaskAutoTeamFrameEvents[arg1].func();
end

function SuccessCreatTaskAutoTeamFrame_OnHide()
end

-- 前往集合按钮
function SuccessCreatTaskAutoTeamFrame_JoinBtn_OnClick()
	local autoTeamData	= GamePromotion:getAutoTeamData( AutoTeamManager:getAutoTeamID() );
	local MaxMapControl = MapShower:GetMaxMapContal();
	MaxMapControl:linkMoveto( autoTeamData.mapID, autoTeamData.npcID );
end

-- 成功创建队伍提示
local t_successCreatTaskAutoTeamFrameMutexFrames = { "PromotionFrame", "CreatingAutoTeamFrame", "SearchTeamFrame", 
														"SuccessCreatActiveAutoTeamFrame" };
function SuccessCreatTaskAutoTeamFrame_OnShow()
	for _, frameName in ipairs( t_successCreatTaskAutoTeamFrameMutexFrames ) do
		local frame = getglobal( frameName );
		if frame:IsShown() then
			frame:Hide();
		end
	end

	UpdateSuccessCreatTaskAutoTeamFrame();
	SetLastRequestFinishCreating( { Time = 0, haveBegin = false } );
	this:SetPoint( "center", "UIClient", "center", 0, 0 );
	SetCurSuoXiaoFrameInfo( { name = this:GetName() } );
end

function SuccessCreatTaskAutoTeamFrame_CloseBtn_OnClick()
	local frame = getglobal( this:GetParent() );
	frame:Hide();
	BeginPlayAutoTeamBtnEffect();
end

function SuccessCreatTaskAutoTeamFrame_OnClick()
	if ( arg3 == "LeftButton" ) then
		local strLinkItemHref	= arg2;
		local autoTeamData	= GamePromotion:getAutoTeamData( AutoTeamManager:getAutoTeamID() );
		local MaxMapControl = MapShower:GetMaxMapContal();
		MaxMapControl:linkMoveto( autoTeamData.mapID, autoTeamData.npcID );
	end
end

-- 提示活动自动组队成功
local t_successActiveAutoTeamFrameEvents = {	["GE_AUTO_TEAM_REQUEST_TRAN_TO_PWORLD"] = {},	["GE_TEAM_LEAVETEAM"]		= {},
												["GE_TEAM_ONE_PLAYER_LEAVE"]			= {},	["GE_TEAM_NEW_PLAYER_JOIN"] = {}, 
												["GE_TEAM_APPLYSUCC"]					= {},	["GE_TEAM_GET_TEAMINFO"]	= {},
												["GE_AUTO_TEAM_SUCCESS"]				= {},	["GE_AUTO_TEAM_SYNC_SUCC"]	= {},
											};

t_successActiveAutoTeamFrameEvents["GE_AUTO_TEAM_REQUEST_TRAN_TO_PWORLD"].func = 
function ()
	if SuccessCreatActiveAutoTeamFrame:IsShown() then
		SuccessCreatActiveAutoTeamFrame:Hide();
	end
	-- TODO:要设置当前缩小的面板
	local mainplayer = ActorMgr:getMainPlayer();
	-- 只有在传送进副本且主角是队长的时候，才设置缩小面板为发起招人界面
	-- 对于普通队友，则还是保持传送界面
	if mainplayer:isCaptain() then
		--print( "GE_AUTO_TEAM_REQUEST_TRAN_TO_PWORLD--SearchTeamFrame--1114" );
		SetCurSuoXiaoFrameInfo( { name = "SearchTeamFrame" } );
	end
	
	--SeteActiveAutoTeamFramePassTime( { startTime = 0, ready = false, autoWarn = true, autoTimeDown = false } );
end

t_successActiveAutoTeamFrameEvents["GE_TEAM_LEAVETEAM"].func = 
function ()
	local t_curSuoXiaoFrame = GetCurSuoXiaoFrameInfo();
	
	if t_curSuoXiaoFrame["name"] ~= "SuccessCreatActiveAutoTeamFrame" then
		return;
	end
	
	local frame = getglobal( "SuccessCreatActiveAutoTeamFrame" );
	if frame:IsShown() then
		frame:Hide();
	end
	
	local t_ActiveAutoTeamFrameControl = GetActiveAutoTeamFrameControl();
	t_ActiveAutoTeamFrameControl:SetTimeEndFlag( { flag = false } );
	SetCurSuoXiaoFrameInfo( { name = "SearchTeamFrame" } );
	--SeteActiveAutoTeamFramePassTime( { startTime = 0, ready = false, autoWarn = true, autoTimeDown = false } );
end

t_successActiveAutoTeamFrameEvents["GE_TEAM_NEW_PLAYER_JOIN"].func = 
function ()
	--[[
	local frame = getglobal( "SuccessCreatActiveAutoTeamFrame" );
	if frame:IsShown() then
		--print( "t_successActiveAutoTeamFrameEvents--GE_TEAM_NEW_PLAYER_JOIN--1033" );
		UpdateSuccessCreatActiveAutoTeamFrameByTeam();
	end
	--]]
end

t_successActiveAutoTeamFrameEvents["GE_TEAM_ONE_PLAYER_LEAVE"].func = 
function ()
	t_successTaskAutoTeamFrameEvents["GE_TEAM_NEW_PLAYER_JOIN"].func();
end

t_successActiveAutoTeamFrameEvents["GE_TEAM_APPLYSUCC"].func = 
function ()
	--print( "t_successActiveAutoTeamFrameEvents--GE_TEAM_APPLYSUCC" );
	t_successTaskAutoTeamFrameEvents["GE_TEAM_NEW_PLAYER_JOIN"].func();
end

t_successActiveAutoTeamFrameEvents["GE_TEAM_GET_TEAMINFO"].func = 
function ()
	--print( "t_successActiveAutoTeamFrameEvents--GE_TEAM_GET_TEAMINFO--1052" );
	t_successActiveAutoTeamFrameEvents["GE_TEAM_NEW_PLAYER_JOIN"].func();
end

t_successActiveAutoTeamFrameEvents["GE_AUTO_TEAM_SUCCESS"].func = 
function ()
	local frame = getglobal( "SuccessCreatActiveAutoTeamFrame" );
	if not frame:IsShown() then
		return;
	end

	UpdateSuccessCreatActiveAutoTeamFrame();
end

t_successActiveAutoTeamFrameEvents["GE_AUTO_TEAM_SYNC_SUCC"].func = 
function ()
	t_successActiveAutoTeamFrameEvents["GE_AUTO_TEAM_SUCCESS"].func();
end

function SuccessCreatActiveAutoTeamFrame_OnLoad()
	for event, _ in pairs( t_successActiveAutoTeamFrameEvents ) do
		this:RegisterEvent( event );
	end

	t_successActiveAutoTeamFrameEvents.__index = function ( tab, key )
									tab[key] = { func = function () end };
									return tab[key];
								end
	setmetatable( t_successActiveAutoTeamFrameEvents, t_successActiveAutoTeamFrameEvents );
end

function UpdateSuccessCreatActiveAutoTeamFrameByTeam()
	ClearSuccessCreatActiveAutoTeamFrame();
	local nBtnIndex		= 1;
	local mainplayer	= ActorMgr:getMainPlayer();
	local nSelfPosition = TEAM_POSITION_MEMBER;
	local teamInfo		= TeamManager:getTeamInfo();
	--print( "UpdateSuccessCreatActiveAutoTeamFrame--teamInfo.MemberNum = "..teamInfo.MemberNum  );
	
	for i = 1, teamInfo.MemberNum do
		if nBtnIndex > MAX_TEAM_MEMBER then
			break;
		end

		local teamMember = teamInfo.TeamMember[i-1];
		local btn = getglobal( "SuccessCreatActiveAutoTeamFrameMemberInfoBtn"..nBtnIndex );
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = GetPlayerName(teamMember.MemberName) },
							[btn:GetName().."RaceFont"]		= { value = t_raceList[teamMember.Career] },
							[btn:GetName().."LevelFont"]	= { value = teamMember.Level.."级" },
						};
		util.SetFonts( t_fonts );

		if CompareRoleName( teamMember.MemberName, mainplayer:getName() ) then
			nSelfPosition = teamMember.Position;
		end

		nBtnIndex = nBtnIndex + 1;
	end

	local t_fonts = {	["SuccessCreatActiveAutoTeamFrameSelfPositionBtnFont"]		= { value = "你是小队"..t_teamPosition[nSelfPosition]["desc"] },
					};
	util.SetFonts( t_fonts );
end

function SuccessCreatActiveAutoTeamFrame_OnEvent()
	t_successActiveAutoTeamFrameEvents[arg1].func();
end

function SuccessCreatActiveAutoTeamFrame_OnHide()
end

function ClearSuccessCreatActiveAutoTeamFrame()
	for i = 1, MAX_TEAM_MEMBER do
		local btn = getglobal( "SuccessCreatActiveAutoTeamFrameMemberInfoBtn"..i );
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = "" },
							[btn:GetName().."RaceFont"]		= { value = "" },
							[btn:GetName().."LevelFont"]	= { value = "" },
						};
		util.SetFonts( t_fonts );
	end
end

function UpdateSuccessCreatActiveAutoTeamFrame()
	ClearSuccessCreatActiveAutoTeamFrame();
	local nBtnIndex		= 1;
	local mainplayer	= ActorMgr:getMainPlayer();
	local nSelfPosition = TEAM_POSITION_MEMBER;
	local teamInfo		= TeamManager:getTeamInfo();
	--print( "UpdateSuccessCreatActiveAutoTeamFrame--teamInfo.MemberNum = "..teamInfo.MemberNum  );
	--[[
	for i = 1, teamInfo.MemberNum do
		if nBtnIndex > MAX_TEAM_MEMBER then
			break;
		end

		local teamMember = teamInfo.TeamMember[i-1];
		local btn = getglobal( "SuccessCreatActiveAutoTeamFrameMemberInfoBtn"..nBtnIndex );
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = teamMember.MemberName },
							[btn:GetName().."RaceFont"]		= { value = t_raceList[teamMember.Career] },
							[btn:GetName().."LevelFont"]	= { value = teamMember.Level.."级" },
						};
		util.SetFonts( t_fonts );
		if teamMember.Position == TEAM_POSITION_CAPTAIN then
			local captainBtn = getglobal( "SuccessCreatActiveAutoTeamFrame_CaptainBtn" );
			captainBtn:SetPoint( "topright", btn:GetName(), "topleft", -11, 0 );
		end

		if CompareRoleName( teamMember.MemberName, mainplayer:getName() ) then
			nSelfPosition = teamMember.Position;
		end

		nBtnIndex = nBtnIndex + 1;
	end
	--]]
	for i = 1, AutoTeamManager:getAutoTeamMemberNum() do
		local teamMember = AutoTeamManager:getAutoTeamMember( i - 1 );
		local btn = getglobal( "SuccessCreatActiveAutoTeamFrameMemberInfoBtn"..nBtnIndex );
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = teamMember.RoleName },
							[btn:GetName().."RaceFont"]		= { value = t_raceList[teamMember.Career] },
							[btn:GetName().."LevelFont"]	= { value = teamMember.Level.."级" },
						};
		util.SetFonts( t_fonts );

		if CompareRoleName( teamMember.RoleName, mainplayer:getName() ) then
			if i == 1 then
				nSelfPosition = TEAM_POSITION_CAPTAIN;
			else
				nSelfPosition = TEAM_POSITION_MEMBER;
			end
			
		end

		nBtnIndex = nBtnIndex + 1;
	end

	local t_fonts = {	["SuccessCreatActiveAutoTeamFrameSelfPositionBtnFont"]		= { value = "你是小队"..t_teamPosition[nSelfPosition]["desc"] },
					};
	util.SetFonts( t_fonts );
end

MAX_AUTO_TEAM_ACTIVELEAVE_TIME = 120;
-- haveWarnTimeEnd为true表示已经自动关闭过一次
local t_ActiveAutoTeamFrameControl = { haveWarnTimeEnd = false };

function GetActiveAutoTeamFrameControl()
	return t_ActiveAutoTeamFrameControl;
end

t_ActiveAutoTeamFrameControl["SetTimeEndFlag"] = 
function ( self, data )
	self["haveWarnTimeEnd"] = data["flag"];
end

t_ActiveAutoTeamFrameControl["GetTimeEndFlag"] = 
function ( self )
	return self["haveWarnTimeEnd"];
end

local t_passTimeInfo = { ready = false, startTime = 0, autoWarn = false, autoTimeDown = false };

function SeteActiveAutoTeamFramePassTime( data )
	t_passTimeInfo = data;
end

function GeteActiveAutoTeamFramePassTime( )
	return t_passTimeInfo;
end

function UpdateActiveAutoTeamFrameLeaveTime( bFirstShow )
	local t_fonts		= {};
	local t_passTimeInfo = GeteActiveAutoTeamFramePassTime();
	if not t_passTimeInfo["autoTimeDown"] then
		t_fonts =	{	["SuccessCreatActiveAutoTeamFrameDescBtnTimeFont"]		= { value = "" },
						["SuccessCreatActiveAutoTeamFrameDescBtnDescFont"]		= { value = "" },
					};
		util.SetFonts( t_fonts );
		return;
	end

	local fLeaveTime	= math.ceil( math.max( 0, MAX_AUTO_TEAM_ACTIVELEAVE_TIME - ( os.clock() - t_passTimeInfo["startTime"] ) ) );
	if bFirstShow then
		local nAutoTeamID	= AutoTeamManager:getAutoTeamID();
		local autoTeamData	= GamePromotion:getAutoTeamData( nAutoTeamID );
		assert( autoTeamData.proType == CLT_AUTO_TEAM_ACTIVE );

		t_fonts =	{	["SuccessCreatActiveAutoTeamFrameDescBtnTimeFont"]		= { value = fLeaveTime },
						["SuccessCreatActiveAutoTeamFrameDescBtnDescFont"]		= { value = "秒后你将被传送到"..PworldDifficulty:getDiffPworldName(autoTeamData.relateID) },
					};
	else
		t_fonts =	{	["SuccessCreatActiveAutoTeamFrameDescBtnTimeFont"]		= { value = fLeaveTime },
					};
	end
	
	util.SetFonts( t_fonts );
end

local t_successCreatActiveAutoTeamFrameMutexFrames = { "PromotionFrame", "SuccessCreatTaskAutoTeamFrame", "CreatingAutoTeamFrame", 
														"SearchTeamFrame", };

function SuccessCreatActiveAutoTeamFrame_OnShow()
	for _, frameName in ipairs( t_successCreatActiveAutoTeamFrameMutexFrames ) do
		local frame = getglobal( frameName );
		if frame:IsShown() then
			frame:Hide();
		end
	end

	UpdateSuccessCreatActiveAutoTeamFrame();
	SetLastRequestFinishCreating( { Time = 0, haveBegin = false } );
	this:SetPoint( "center", "UIClient", "center", 0, 0 );
	-- TODO; 只要修改更新时间的时机
	--UpdateActiveAutoTeamFrameLeaveTime( true );
	SetCurSuoXiaoFrameInfo( { name = this:GetName() } );
	UpdateActiveOpenTime();
end

function GetPromotionOpenTime( nRow )
	local promotionData = GamePromotion:getPromotionData( nRow );
	local startInfo = GamePromotion:getCurrPromitionStartInfo( nRow );
	local timeIndex = startInfo.timeIdx;
	if timeIndex < 0 then
		timeIndex = 0;
	end

	local nCurHour	= GamePromotion:getSerHour()
	local nCurMin	= GamePromotion:getSerMin()
	local szOpenTimeDesc = "";
	if startInfo.startFlag == PROMOTION_RUNING then
		szOpenTimeDesc	= promotionData.m_Time[timeIndex].m_szStartTime .."-".. promotionData.m_Time[timeIndex].m_szEndTime;
	end
	-- 每天举办的活动
	--[[
	if promotionData.m_nTimeCount == 0 or ( promotionData.m_Time[0].m_nStartHour == 0 and promotionData.m_Time[0].m_nStartMin == 0 and	
		promotionData.m_Time[0].m_nEndHour == 24 and promotionData.m_Time[0].m_nEndMin == 0 ) then
			szOpenTimeDesc = "00:00-24:00";
	else
		szOpenTimeDesc	= promotionData.m_Time[timeIndex];
		for i = 1,promotionData.m_nTimeCount do
			local promotionTimeInfo = promotionData.m_Time[i-1];
			if promotionTimeInfo ~= nil and promotionTimeInfo.m_nStartHour <= nCurHour and promotionTimeInfo.m_nEndHour >= nCurHour and 
				promotionTimeInfo.m_nStartMin <= nCurMin and promotionTimeInfo.m_nEndMin >= nCurMin then

				szOpenTimeDesc = szOpenTimeDesc..promotionTimeInfo.m_nStartHour..":".. 
								string.format("%02d", promotionTimeInfo.m_nStartMin) 
								.."-"..promotionTimeInfo.m_nEndHour..":".. 
								string.format("%02d", promotionTimeInfo.m_nEndMin).."  ";
			end
			
		end
	end

	-- 一周举办几次的活动
	if promotionData.m_nType == PROMOTION_TYPE_WEEKLY then
		for i = 1, 7 do
			if GamePromotion:isWeekOpen( nRow, i - 1 ) and GamePromotion:isWeekToday( i - 1 ) then
				for idx = 1,promotionData.m_nTimeCount do
					local promotionTimeInfo = promotionData.m_Time[idx-1];
					
					if promotionTimeInfo.m_nStartHour <= nCurHour and promotionTimeInfo.m_nEndHour >= nCurHour and 
					promotionTimeInfo.m_nStartMin <= nCurMin and promotionTimeInfo.m_nEndMin >= nCurMin then

						szOpenTimeDesc = szOpenTimeDesc..promotionTimeInfo.m_nStartHour..":"..
											string.format("%02d",promotionTimeInfo.m_nStartMin).."-"..promotionTimeInfo.m_nEndHour..":"..
											string.format("%02d", promotionTimeInfo.m_nEndMin).."  ";
					end
					
				end
			end
		end
	end
	--]]
	return szOpenTimeDesc;
end

local t_copyDiffultInfo = { [0] = { desc = "简单" }, [1] = { desc = "普通" }, [2] = { desc = "困难" }, [3] = { desc = "专家" }, };
function UpdateActiveOpenTime()
	local nAutoTeamID	= AutoTeamManager:getAutoTeamID();
	local autoTeamData	= GamePromotion:getAutoTeamData( nAutoTeamID );
	assert( autoTeamData.proType == CLT_AUTO_TEAM_ACTIVE );
	-- 难度为：困难，时间：开始时间~结束时间
	local szText = "难度为："..t_copyDiffultInfo[AutoTeamManager:getPworldDiffulty()]["desc"].."，时间："..
					( GetPromotionOpenTime( autoTeamData.nRow ) );

	local t_fonts =	{	["SuccessCreatActiveAutoTeamFrameDescBtnCopyInfoFont"]	= { value = szText },
						["SuccessCreatActiveAutoTeamFrameDescBtnDescFont"]	= { value = "你将传送到"..PworldDifficulty:getDiffPworldName(autoTeamData.relateID).."副本" },
					};
	
	util.SetFonts( t_fonts );
end

function SuccessCreatActiveAutoTeamFrame_OnUpdate()
	--[[UpdateActiveAutoTeamFrameLeaveTime( false );

	if ( MAX_AUTO_TEAM_ACTIVELEAVE_TIME - ( os.clock() - t_passTimeInfo["startTime"] ) ) <= 30 then
		t_passTimeInfo["autoWarn"] = false;
	end
	--]]

	local t_ActiveAutoTeamFrameControl = GetActiveAutoTeamFrameControl();
	if t_ActiveAutoTeamFrameControl:GetTimeEndFlag() then
		return;
	end

	-- TODO; 更改成时间到了就关闭该面板
	local nAutoTeamID	= AutoTeamManager:getAutoTeamID();
	local autoTeamData	= GamePromotion:getAutoTeamData( nAutoTeamID );
	if autoTeamData.proType ~= CLT_AUTO_TEAM_ACTIVE then
		return;
	end

	local startInfo = GamePromotion:getCurrPromitionStartInfo( autoTeamData.nRow );
	if startInfo.startFlag ~= PROMOTION_RUNING then
		ShowMidTips( "你所参加的活动已经结束" );
		this:Hide();
		local t_ActiveAutoTeamFrameControl = GetActiveAutoTeamFrameControl();
		t_ActiveAutoTeamFrameControl:SetTimeEndFlag( { flag = true } );
	end
	
	local btn = getglobal( "SuccessCreatActiveAutoTeamFrame_NowTransBtn" );
	if TeamFollowHintFrame:IsShown() then
		btn:SetGray( true );
	else
		btn:SetGray( false );
	end
end

-- 退出队列
function SuccessCreatTaskAutoTeamFrame_ExitAutoTeamBtn_OnClick()
	local frame = getglobal( this:GetParent() );
	--frame:Hide();
	MessageBox("提示","确定要离开队伍吗？");
	MessageBoxFrame:SetClientString("离开自动组队");
	--AutoTeamManager:cancelTranToPWorld();
end

-- 马上传送
function SuccessCreatTaskAutoTeamFrame_NowTransBtn_OnClick()
	if TeamFollowHintFrame:IsShown() then
		ShowMidTips( "组队跟随状态下,禁止此操作" );
		return;
	end

	if not AutoTeamManager:canTranToPWorld() then
		return;
	end

	local frame = getglobal( this:GetParent() );
	frame:Hide();
	
	AutoTeamManager:tranToPWorld();
end

-- 一会再去
function SuccessCreatTaskAutoTeamFrame_WaitMinutesBtn_OnClick()
	local frame = getglobal( this:GetParent() );
	frame:Hide();
	BeginPlayAutoTeamBtnEffect();
end

function SuccessCreatActiveAutoTeamFrame_CloseBtn_OnClick()
	local frame = getglobal( this:GetParent() );
	frame:Hide();
	BeginPlayAutoTeamBtnEffect();
end

-- 寻求组队
local MAX_SEARCH_NUM = 8;

local t_searchSelCopyInfo = { name = "" };

--select 表示该活动是否被选中	join 表示是否能参加该活动		haveDiff 表示该活动是否有难度选项
-- { id = , simple = bool, normal = bool, hard = bool, professional = bool?, haveDiff = ?, select = ?, join = ?, name = ?, 
--		row = ?, limitFlag = 0, resetDefault = true },
-- 所有自动组队中用到的活动数据
local t_allAutoTeamActives = { resetDefault = true, limitFlag = 0, continueAutoTeam = false };

function GetAllAutoTeamActivesControl()
	return t_allAutoTeamActives;
end

-- 排序 首先是可以进入的排在不可进入的上面。
-- 然后是普通排在活动的上面
-- 最后是按照进入等级排列
t_allAutoTeamActives["Sort"] =
function ( self )
	table.sort( self, 
				function ( lhs, rhs )
					local t_boolToNum = { [true] = 1, [false] = 0 };
					if t_boolToNum[lhs["finish"]] == t_boolToNum[rhs["finish"]] then
						if t_boolToNum[lhs["join"]] == t_boolToNum[rhs["join"]] then
							if t_boolToNum[lhs["haveDiff"]] == t_boolToNum[rhs["haveDiff"]] then
								local lActiveInfo = GamePromotion:getPromotionData( lhs["row"] );
								local rActiveInfo = GamePromotion:getPromotionData( rhs["row"] );
								return lActiveInfo.m_nLevel < rActiveInfo.m_nLevel;
							else
								return t_boolToNum[lhs["haveDiff"]] > t_boolToNum[rhs["haveDiff"]];
							end						
						else
							return t_boolToNum[lhs["join"]] > t_boolToNum[rhs["join"]];
						end
					else
						return t_boolToNum[lhs["finish"]] < t_boolToNum[rhs["finish"]];
					end
				end
			);
end


t_allAutoTeamActives["SetResetFlag"] =
function ( self, flag )
	self["resetDefault"] = flag;
end

t_allAutoTeamActives["AddLimitFlag"] =
function ( self, limit )
	if self["limitFlag"] == nil then
		self["limitFlag"] = 0;
	end

	if limit >= 0 then
		self["limitFlag"] = bitOr( self["limitFlag"], limit );
	else
		self["limitFlag"] = bitNotAnd( self["limitFlag"], limit );
	end	
end

t_allAutoTeamActives["GetLimitFlag"] =
function ( self )
	return self["limitFlag"];
end

-- 初始化
t_allAutoTeamActives["Init"] =
function ( self )
	self["resetDefault"]		= true;
	self["limitFlag"]			= 0;
	self["continueAutoTeam"]	= false;
end

t_allAutoTeamActives["SetContinueAutoTeam"] =
function ( self, flag )
	self["continueAutoTeam"] = flag;
end

t_allAutoTeamActives["GetContinueAutoTeam"] =
function ( self, flag )
	return self["continueAutoTeam"];
end

-- 是否需要重置面板所有选项
t_allAutoTeamActives["NeedSetDefault"] =
function ( self )
	return self["resetDefault"];
end

-- 清楚所有选项
t_allAutoTeamActives["ClearAllChoice"] =
function ( self )
	self:clear();
	local teamInfo = TeamManager:getTeamInfo();
	for  i = 1, GamePromotion:getAllNum() do
		local nRow		= GamePromotion:getAllRow( i - 1 );
		local nResult	= GamePromotion:getAutoTeamStateByNoCountLimit( nRow );
		local startInfo = GamePromotion:getCurrPromitionStartInfo( nRow );
		
		-- and nResult ~= PROMOTION_NOT_START	PROMOTION_NOT_TODAY
		if nResult ~= PROMOTION_NOT_NEED_AUTOTEAM and nResult ~= PROMOTION_NOT_TODAY then
			local activeInfo = GamePromotion:getPromotionData( nRow );
			local isCurSimpleCanJoin	= not PworldDifficulty:isPworldLimit( activeInfo.m_nRelateId, PworldDifficulty.EASY );
			local isCurNormalCanJoin	= not PworldDifficulty:isPworldLimit( activeInfo.m_nRelateId, PworldDifficulty.GENERAL );
			local isCurHardCanJoin		= not PworldDifficulty:isPworldLimit( activeInfo.m_nRelateId, PworldDifficulty.HARD );
			local isCurProfessionalCanJoin	= not PworldDifficulty:isPworldLimit( activeInfo.m_nRelateId, PworldDifficulty.EXPERT );
			
			local bJoin = ( nResult == PROMOTION_OK );
			local nNeedLv = nResult == PROMOTION_LEVEL_LIMIT and activeInfo.m_nLevel or nil
			for i = 1, teamInfo.MemberNum do
				if teamInfo.TeamMember[i-1].Level <  activeInfo.m_nLevel then
					nNeedLv = activeInfo.m_nLevel;
					bJoin = false;
					break;
				end
			end
			if nRow == 8 then
				print( "ClearAllChoice--"..t_boolDesc[bJoin].."--1577" );
			end
			if not HaveDifficultyChoice( { activeType = activeInfo.m_nFlag, id = activeInfo.m_nRelateId } ) then
				nResult = GamePromotion:getAutoTeamState( nRow )
			end
			self:insertData( {	name		= activeInfo.m_szName,	simple		= false,	
								normal		= false,				hard		= false,				
								professional= false,				id			= activeInfo.m_nRelateId,	
								select		= false,				join		= bJoin,
								haveDiff	= HaveDifficultyChoice( { activeType = activeInfo.m_nFlag, id = activeInfo.m_nRelateId } ),
								row			= nRow,	
								teamID		= activeInfo.m_nAutoTeamID,
								joinSimple	= isCurSimpleCanJoin,
								joinNormal	= isCurNormalCanJoin,
								joinHard	= isCurHardCanJoin,
								joinProfessional= isCurProfessionalCanJoin,
								needLv		= nNeedLv,
								finish		= GamePromotion:getAutoTeamState( nRow ) == PROMOTION_HAVE_FINISH and not HaveDifficultyChoice( { activeType = activeInfo.m_nFlag, id = activeInfo.m_nRelateId } ),
								} 
							);
		end
	end

	self:Sort();

	self["limitFlag"]		= 0;
	self["resetDefault"]	= false;
end

t_allAutoTeamActives["getOneDifficultyOption"] = 
function ( self, data )
	local curCanJoinState	= data["curState"];
	local lastSelectState	= data["lastSelectState"];
	if not curCanJoinState then
		return false;
	end

	return data["lastSelectState"];
end

-- 重置为默认配置
t_allAutoTeamActives["SetDefault"] =
function ( self )
	self:clear();
	local nCurMapID	= MapShower:getRealMapId();
	local nPworldID = 0;
	if nCurMapID >= MIN_COPY_ID then
		nPworldID = math.ceil( nCurMapID / 100 );
	end
	local teamInfo = TeamManager:getTeamInfo();
	for  i = 1, GamePromotion:getAllNum() do
		local nRow		= GamePromotion:getAllRow( i - 1 );
		local nResult	= GamePromotion:getAutoTeamStateByNoCountLimit( nRow );
		local startInfo = GamePromotion:getCurrPromitionStartInfo( nRow );
		
		-- and nResult ~= PROMOTION_NOT_START	PROMOTION_NOT_TODAY
		if nResult ~= PROMOTION_NOT_NEED_AUTOTEAM and nResult ~= PROMOTION_NOT_TODAY then
			local activeInfo	= GamePromotion:getPromotionData( nRow );
			local bCanJoin		= true;
			-- 是否能够参加多了个筛选依据，根据当前主角自身所处的副本场景
			
			if activeInfo.m_nFlag == CLT_AUTO_TEAM_ACTIVE and nPworldID > 0 then
				if nPworldID == activeInfo.m_nRelateId then
					bCanJoin = true;
				else
					bCanJoin = false;
				end
			end
			
			local isCurSimpleCanJoin	= not PworldDifficulty:isPworldLimit( activeInfo.m_nRelateId, PworldDifficulty.EASY );
			local canJoinSimple			= self:getOneDifficultyOption( { ["lastSelectState"] = true,["curState"] = isCurSimpleCanJoin, } );
			
			local isCurNormalCanJoin	= not PworldDifficulty:isPworldLimit( activeInfo.m_nRelateId, PworldDifficulty.GENERAL );
			local canJoinNormal			= self:getOneDifficultyOption( { ["lastSelectState"] = true,["curState"] = isCurNormalCanJoin } );
			
			local isCurHardCanJoin	= not PworldDifficulty:isPworldLimit( activeInfo.m_nRelateId, PworldDifficulty.HARD );
			local canJoinHard		= self:getOneDifficultyOption( {	["lastSelectState"] = false, ["curState"] = isCurHardCanJoin } );
			
			local isCurProfessionalCanJoin	= not PworldDifficulty:isPworldLimit( activeInfo.m_nRelateId, PworldDifficulty.EXPERT );
			local canJoinProfessional		= self:getOneDifficultyOption( { ["lastSelectState"] = false, ["curState"] = isCurProfessionalCanJoin } );
			
			local isAnyDifficultCanJoin		= isCurSimpleCanJoin or isCurNormalCanJoin or isCurHardCanJoin or isCurProfessionalCanJoin;

			if not HaveDifficultyChoice( { activeType = activeInfo.m_nFlag, id = activeInfo.m_nRelateId } ) then
				nResult = GamePromotion:getAutoTeamState( nRow )
			end
			local bJoin = ( nResult == PROMOTION_OK ) and bCanJoin and isAnyDifficultCanJoin;
			if nRow == 8 then
				print( "SetDefault--"..t_boolDesc[bJoin].."--1657" );
			end
			local nNeedLv = nResult == PROMOTION_LEVEL_LIMIT and activeInfo.m_nLevel or nil
			for i = 1, teamInfo.MemberNum do
				if teamInfo.TeamMember[i-1].Level <  activeInfo.m_nLevel then
					nNeedLv = activeInfo.m_nLevel;
					bCanJoin = false
					break;
				end
			end

			if not CanJoinPworldUseVig( activeInfo.m_nRelateId, PworldDifficulty.EASY ) then
				canJoinSimple = false;
			end
			if not CanJoinPworldUseVig( activeInfo.m_nRelateId, PworldDifficulty.GENERAL ) then
				canJoinNormal = false;
			end
			if not CanJoinPworldUseVig( activeInfo.m_nRelateId, PworldDifficulty.HARD ) then
				canJoinHard = false;
			end
			if not CanJoinPworldUseVig( activeInfo.m_nRelateId, PworldDifficulty.EXPERT ) then
				canJoinProfessional = false;
			end
			
			self:insertData( {	name		= activeInfo.m_szName,
								id			= activeInfo.m_nRelateId,	
								select		= true,					
								haveDiff	= HaveDifficultyChoice( { activeType = activeInfo.m_nFlag, id = activeInfo.m_nRelateId } ),
								row			= nRow,	
								teamID		= activeInfo.m_nAutoTeamID,
								
								simple		= canJoinSimple and CanJoinPworldUseVig( activeInfo.m_nRelateId, PworldDifficulty.EASY ),	
								normal		= canJoinNormal and CanJoinPworldUseVig( activeInfo.m_nRelateId, PworldDifficulty.GENERAL ),
								hard		= canJoinHard and CanJoinPworldUseVig( activeInfo.m_nRelateId, PworldDifficulty.HARD ),
								professional= canJoinProfessional and CanJoinPworldUseVig( activeInfo.m_nRelateId, PworldDifficulty.EXPERT ),
								join		= ( nResult == PROMOTION_OK ) and bCanJoin and isAnyDifficultCanJoin,

								joinSimple	= isCurSimpleCanJoin,
								joinNormal	= isCurNormalCanJoin,
								joinHard	= isCurHardCanJoin,
								joinProfessional= isCurProfessionalCanJoin,
								needLv		= nNeedLv,
								finish		= GamePromotion:getAutoTeamState( nRow ) == PROMOTION_HAVE_FINISH and not HaveDifficultyChoice( { activeType = activeInfo.m_nFlag, id = activeInfo.m_nRelateId } ) ,
								} 
							);
		end
	end

	self:Sort();

	self["limitFlag"]			= 0;
	self["resetDefault"]		= false;
	self["continueAutoTeam"]	= false;
end

-- 同步更新活动逻辑数据
t_allAutoTeamActives["getPworldDiffculty"] =
function ( self, data )
	local nAutoTeamID = AutoTeamManager:getAutoTeamID();
	if nAutoTeamID > 0 then
		return AutoTeamManager:getPworldDiffulty();
	end
	
	local nCurMapID	= MapShower:getRealMapId();
	local nPworldID = 0;
	if nCurMapID >= MIN_COPY_ID then
		nPworldID = math.ceil( nCurMapID / 100 );
	end

	return PworldDifficulty:getDifficulty( nPworldID );
end

t_allAutoTeamActives["SynchronizePromotion"] =
function ( self )
	local nCurMapID	= MapShower:getRealMapId();

	if nCurMapID >= MIN_COPY_ID then
		self:SynchronizePromotionInPworld();
	else
		self:SynchronizePromotionNotInPworld();
	end

	self:Sort();
end

t_allAutoTeamActives["SynchronizePromotionInPworld"] =
function ( self )
	local nAutoTeamID	= AutoTeamManager:getAutoTeamID();
	local nCurMapID		= MapShower:getRealMapId();
	local nCurInPworldID= math.ceil( nCurMapID / 100 );
	
	for index, data in ipairs( self ) do
		local activeInfo			= GamePromotion:getPromotionData( data["row"] );
		local isInTheCorrectPworld	= false;
		local nQueryDifficulty		= CLT_PWORLD_END;

		-- 是否能够参加多了个筛选依据，根据当前主角自身所处的副本场景
		if activeInfo.m_nFlag == CLT_AUTO_TEAM_ACTIVE then
			nQueryDifficulty		= self:getPworldDiffculty();
			local nDefPworld		= PworldDifficulty:getDifficultyPworldId( activeInfo.m_nRelateId, nQueryDifficulty )
			isInTheCorrectPworld	= ( nCurInPworldID == nDefPworld );
		end
		
		if data["row"] == 8 then
			print( "SynchronizePromotionInPworld-- isInTheCorrectPworld = "..t_boolDesc[isInTheCorrectPworld].."， name = "..data["name"].."--1774" );
		end

		local isCurSimpleCanJoin		= not PworldDifficulty:isPworldLimit( activeInfo.m_nRelateId, PworldDifficulty.EASY );
		local isCurNormalCanJoin		= not PworldDifficulty:isPworldLimit( activeInfo.m_nRelateId, PworldDifficulty.GENERAL );		
		local isCurHardCanJoin			= not PworldDifficulty:isPworldLimit( activeInfo.m_nRelateId, PworldDifficulty.HARD );		
		local isCurProfessionalCanJoin	= not PworldDifficulty:isPworldLimit( activeInfo.m_nRelateId, PworldDifficulty.EXPERT );		

		self:setActiveData( index,
							{	name		= data["name"],				
								id			= data["id"],				
								select		= isInTheCorrectPworld,					
								join		= isInTheCorrectPworld,
								haveDiff	= data["haveDiff"],
								row			= data["row"], 
								teamID		= data["teamID"],

								simple		= isInTheCorrectPworld and ( nQueryDifficulty == CLT_PWORLD_SIMPLE ),
								normal		= isInTheCorrectPworld and ( nQueryDifficulty == CLT_PWORLD_NORMAL ),
								hard		= isInTheCorrectPworld and ( nQueryDifficulty == CLT_PWORLD_HARD ),
								professional= isInTheCorrectPworld and ( nQueryDifficulty == CLT_PWORLD_PROFESSIONAL ),

								joinSimple		= isInTheCorrectPworld and ( nQueryDifficulty == CLT_PWORLD_SIMPLE ),
								joinNormal		= isInTheCorrectPworld and ( nQueryDifficulty == CLT_PWORLD_NORMAL ),
								joinHard		= isInTheCorrectPworld and ( nQueryDifficulty == CLT_PWORLD_HARD ),
								joinProfessional= isInTheCorrectPworld and ( nQueryDifficulty == CLT_PWORLD_PROFESSIONAL ),
							} 
						);
	end
end

t_allAutoTeamActives["SynchronizePromotionNotInPworld"] =
function ( self )
	local teamInfo = TeamManager:getTeamInfo();
	for index, data in ipairs( self ) do
		local activeInfo			= GamePromotion:getPromotionData( data["row"] );		
		local isCurSimpleCanJoin	= not PworldDifficulty:isPworldLimit( activeInfo.m_nRelateId, PworldDifficulty.EASY );
		local canJoinSimple			= self:getOneDifficultyOption( { ["lastSelectState"] = data["simple"], ["curState"] = isCurSimpleCanJoin, } );
		
		local isCurNormalCanJoin	= not PworldDifficulty:isPworldLimit( activeInfo.m_nRelateId, PworldDifficulty.GENERAL );
		local canJoinNormal			= self:getOneDifficultyOption( { ["lastSelectState"] = data["normal"], ["curState"] = isCurNormalCanJoin } );
		
		local isCurHardCanJoin		= not PworldDifficulty:isPworldLimit( activeInfo.m_nRelateId, PworldDifficulty.HARD );
		local canJoinHard			= self:getOneDifficultyOption( {	["lastSelectState"] = data["hard"], ["curState"] = isCurHardCanJoin } );
		
		local isCurProfessionalCanJoin	= not PworldDifficulty:isPworldLimit( activeInfo.m_nRelateId, PworldDifficulty.EXPERT );
		local canJoinProfessional		= self:getOneDifficultyOption( { ["lastSelectState"] = data["professional"], ["curState"] = isCurProfessionalCanJoin } );
		
		local isAnyDifficultCanJoin		= isCurSimpleCanJoin or isCurNormalCanJoin or isCurHardCanJoin or isCurProfessionalCanJoin;
		
		local nResult = GamePromotion:getAutoTeamStateByNoCountLimit( data["row"] )
		local isResultOK
		if data["haveDiff"] then
			isResultOK = ( nResult == PROMOTION_OK );
		else
			isResultOK = ( GamePromotion:getAutoTeamState( data["row"] ) == PROMOTION_OK );
		end
		local bJoin			= ( isResultOK and isAnyDifficultCanJoin );

		if data["row"] == 8 then
			print( "SynchronizePromotionNotInPworld--"..t_boolDesc[bJoin].."--1840" );
			print( "SynchronizePromotionNotInPworld--isResultOK = "..t_boolDesc[isResultOK].."isAnyDifficultCanJoin = "
					..t_boolDesc[isAnyDifficultCanJoin].."， name = "..data["name"].."--1842" );
		end
		
		local bSelect = data["select"];
		if not bJoin then
			bSelect = false;
		end

		local nNeedLv = nResult == PROMOTION_LEVEL_LIMIT and activeInfo.m_nLevel or nil
		for i = 1, teamInfo.MemberNum do
			if teamInfo.TeamMember[i-1].Level <  activeInfo.m_nLevel then
				nNeedLv = activeInfo.m_nLevel;
				bJoin = false;
				break;
			end
		end

		self:setActiveData( index,
							{	name		= data["name"],				
								id			= data["id"],				
								select		= bSelect,					
								join		= bJoin,
								haveDiff	= data["haveDiff"],
								row			= data["row"], 
								teamID		= data["teamID"],

								simple		= canJoinSimple and CanJoinPworldUseVig( activeInfo.m_nRelateId, PworldDifficulty.EASY ),	
								normal		= canJoinNormal and CanJoinPworldUseVig( activeInfo.m_nRelateId, PworldDifficulty.GENERAL ),
								hard		= canJoinHard and CanJoinPworldUseVig( activeInfo.m_nRelateId, PworldDifficulty.HARD ),
								professional= canJoinProfessional and CanJoinPworldUseVig( activeInfo.m_nRelateId, PworldDifficulty.EXPERT ),

								joinSimple		= isCurSimpleCanJoin,
								joinNormal		= isCurNormalCanJoin,
								joinHard		= isCurHardCanJoin,
								joinProfessional= isCurProfessionalCanJoin,
								needLv			= nNeedLv,
								finish			= GamePromotion:getAutoTeamState( data["row"] ) == PROMOTION_HAVE_FINISH and not data["haveDiff"],
							} 
						);
	end
end

-- 获得所有可参加活动的数量
t_allAutoTeamActives["getCanJoinNum"] = 
function ( self )
	local nNum = 0;
	for _, data in ipairs( self ) do
		if data["join"] then
			nNum = nNum + 1;
		end
	end

	return nNum;
end

-- 获得所有可参加且有难度活动的数量
t_allAutoTeamActives["getCanJoinWithDiffNum"] = 
function ( self )
	local nNum = 0;
	for _, data in ipairs( self ) do
		if data["join"] and  data["haveDiff"] then
			nNum = nNum + 1;
		end
	end

	return nNum;
end

-- 获得所有可参加且有难度且被勾选的活动的数量
t_allAutoTeamActives["getCanJoinWithDiffAndSelectNum"] = 
function ( self )
	local nNum = 0;
	for _, data in ipairs( self ) do
		if data["join"] and  data["haveDiff"] and data["select"] then
			nNum = nNum + 1;
		end
	end

	return nNum;
end


-- 获得所有已选择活动的数量
t_allAutoTeamActives["getSelectNum"] = 
function ( self )
	local nNum = 0;
	for _, data in ipairs( self ) do
		if data["join"] and data["select"] then
			nNum = nNum + 1;
		end
	end

	return nNum;
end

-- 获得所有已选择活动的数量是否为0
t_allAutoTeamActives["isSelectEmpty"] = 
function ( self )
	return self:getSelectNum() == 0;
end

-- 是否所有可参加的活动都勾选上了
t_allAutoTeamActives["isAllSelect"] = 
function ( self )
	return self:getSelectNum() == self:getCanJoinNum();
end

-- 是否所有可参加的活动都勾选上了
t_allAutoTeamActives["isAllHaveDiffSelect"] = 
function ( self )
	return self:getCanJoinWithDiffAndSelectNum() == self:getCanJoinWithDiffNum();
end

-- 是否所有的活动都被勾选上了
t_allAutoTeamActives["isAllActiveSelect"] = 
function ( self )
	if self:getCanJoinNum() == 0 then
		return false;
	end

	for _, data in ipairs( self ) do
		if data["join"] and not data["select"] then
			return false;
		end
	end

	return true;
end

-- 是否所有的简单选项被勾选了
t_allAutoTeamActives["isAllSimpleSelect"] = 
function ( self )
	if self:getCanJoinNum() == 0 then
		return false;
	end
	
	for _, data in ipairs( self ) do
		if data["join"] and data["haveDiff"] and data["joinSimple"] and not data["simple"] then
			return false;
		end
	end

	return true;
end

-- 是否所有的普通选项被勾选了
t_allAutoTeamActives["isAllNormalSelect"] = 
function ( self )
	if self:getCanJoinNum() == 0 then
		return false;
	end

	for _, data in ipairs( self ) do
		if data["join"] and data["haveDiff"] and data["joinNormal"] and not data["normal"] then
			return false;
		end
	end

	return true;
end

-- 是否所有的困难选项被勾选了
t_allAutoTeamActives["isAllHardSelect"] = 
function ( self )
	if self:getCanJoinNum() == 0 then
		return false;
	end

	for _, data in ipairs( self ) do
		if data["join"] and data["haveDiff"] and data["joinHard"] and not data["hard"] then
			return false;
		end
	end

	return true;
end

-- 是否所有的专家选项被勾选了
t_allAutoTeamActives["isAllProfessionalSelect"] = 
function ( self )
	if self:getCanJoinNum() == 0 then
		return false;
	end

	for _, data in ipairs( self ) do
		if data["join"] and data["haveDiff"] and data["joinProfessional"] and not data["professional"] then
			return false;
		end
	end

	return true;
end

-- 获得所有活动数量
t_allAutoTeamActives["getTotalNum"] = 
function ( self )
	return table.getn( self );
end

-- 查询某个数据节点
t_allAutoTeamActives["getActiveData"] = 
function ( self, nIndex )
	return self[nIndex];
end

-- 更新某个节点的数据
t_allAutoTeamActives["setActiveData"] = 
function ( self, nIndex, data )
	self[nIndex] = data;
end

-- 是否有可参加的活动
t_allAutoTeamActives["isCanJoinActiveEmpty"] = 
function ( self )
	return self:getCanJoinNum() == 0;
end

-- 获得需要勾选难度且可以参加的活动数量
t_allAutoTeamActives["getCanJoinWithDiffNum"] = 
function ( self )
	local nNum = 0;
	for _, data in ipairs( self ) do
		if data["join"] and data["haveDiff"] then
			nNum = nNum + 1;
		end
	end

	return nNum;
end

-- 添加新元素
t_allAutoTeamActives["insertData"] = 
function ( self, data )
	table.insert( self, {	id			= data["id"],			simple	= data["simple"],	normal	= data["normal"],	hard = data["hard"],
							professional= data["professional"],	haveDiff= data["haveDiff"],	select	= data["select"],	join	= data["join"],		
							name		= data["name"],			row		= data["row"],		teamID	= data["teamID"],
							joinSimple	= data["joinSimple"],	joinNormal		= data["joinNormal"],
							joinHard	= data["joinHard"],		joinProfessional= data["joinProfessional"],	needLv = data["needLv"], finish = data["finish"]
						} );
end

-- 清空所有活动数据
t_allAutoTeamActives["clear"] = 
function ( self )
	for i = 1, table.getn( self ) do
		table.remove( self );
	end
end

-- 根据活动名字获取活动信息
t_allAutoTeamActives["GetActiveDataByRow"] = 
function ( self, data )
	for _, element in ipairs( self ) do
		if data["row"] == element["row"] then
			return element;
		end
	end
	
	return nil;
end

-- 根据活动名字获取活动索引
t_allAutoTeamActives["GetActiveIndexByRow"] = 
function ( self, data )
	for index, element in ipairs( self ) do
		if data["row"] == element["row"] then
			return index;
		end
	end
	
	assert( false );
	return -1;
end

-- 是否有副本没选中难度
function IsAnyPworldDiffNotSel()
	for i = 1, t_allAutoTeamActives:getTotalNum() do
		local data = t_allAutoTeamActives:getActiveData( i );
		if data["select"] and data["join"] and data["haveDiff"] and not data["simple"] and not data["normal"] and not data["hard"] 
			and not data["professional"] then
			return true;
		end
	end

	return false;
end

function GetSearchSelCopyInfo()
	return t_searchSelCopyInfo;
end

function SetSearchSelCopyInfo( data )
	t_searchSelCopyInfo = data;
end

function ClearSearchTeamFram()
	for i = 1, MAX_SEARCH_NUM do
		local btn		= getglobal( "SearchTeamFram_CopyInfoBtn"..i );
		local nameFont	= getglobal( "SearchTeamFram_CopyInfoBtn"..i.."NameFont" );
		nameFont:SetClientID( 0 );
		btn:DisChecked();
		btn:Disable();
		btn:SetClientUserData( 0, 0 );
		
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = ""  },
							[btn:GetName().."SimpleFont"]	= { value = ""	},
							[btn:GetName().."NormalFont"]	= { value = ""	},
							[btn:GetName().."HardFont"]		= { value = ""	},
							[btn:GetName().."ProfessionalFont"]	= { value = ""	},
							[btn:GetName().."EventFont"]	= { value = "" },
							[btn:GetName().."LvFont"]		= { value = "" },
						};
		util.SetFonts( t_fonts );
		
		local t_disableBtns =	{	"SearchTeamFrame_CopyInfoAllDifficultyChkBtn"..i, 
									"SearchTeamFrame_CopyInfoSimpleChkBtn"..i, 
									"SearchTeamFrame_CopyInfoNormalChkBtn"..i, 
									"SearchTeamFrame_CopyInfoHardChkBtn"..i,
									"SearchTeamFrame_CopyInfoProfessionalChkBtn"..i,
								};
		for _, name in ipairs( t_disableBtns ) do
			btn	= getglobal( name );
			btn:Disable();
		end
		
		local t_hideIcons =	{	"SearchTeamFrame_CopyInfoAllDifficultyChkBtn"..i, 
								"SearchTeamFrame_CopyInfoSimpleChkBtn"..i, 
								"SearchTeamFrame_CopyInfoNormalChkBtn"..i, 
								"SearchTeamFrame_CopyInfoHardChkBtn"..i,
								"SearchTeamFrame_CopyInfoProfessionalChkBtn"..i,
								};
		util.HideIcons( t_hideIcons );
	end
	
	local t_chkBtns = 
	{	["SearchTeamFrame_SelectAllSimpleChkBtn"]	= { state = false }, 
		["SearchTeamFrame_SelectAllNormalChkBtn"]	= { state = false },
		["SearchTeamFrame_SelectAllHardChkBtn"]		= { state = false },
		["SearchTeamFrame_SelectAllProfessionalChkBtn"]= { state = false },
		["SearchTeamFrame_SelectAllChkBtn"]			= { state = false },
	};
	util.SetChkState( t_chkBtns );
	
	local t_disableBtns =	{	"SearchTeamFrame_SelectAllSimpleChkBtn", 
								"SearchTeamFrame_SelectAllNormalChkBtn", 
								"SearchTeamFrame_SelectAllHardChkBtn", 
								"SearchTeamFrame_SelectAllProfessionalChkBtn",
								"SearchTeamFrame_SelectAllChkBtn",
							};
	util.DisableBtns( t_disableBtns );
end

function HaveDifficultyChoice( data )
	if data["activeType"] == CLT_AUTO_TEAM_TASK then
		return false;
	end

	local id = data.id
	local has = false
	for i = 1, PworldDifficulty.MAXNUM - 1 do
		if PworldDifficulty:getDifficultyPworldId(id, i) ~= 0 then
			has = true
			break
		end
	end
	return has
end

function SetSearchTeamFrameBtnByContinue( data )
	local btn			= getglobal( data["name"] );
	local oneActiveInfo	= data["oneActiveInfo"];	
	local nameFont		= getglobal( btn:GetName().."NameFont" );
	nameFont:SetClientID( oneActiveInfo["id"] );
	btn:SetClientUserData( 0, oneActiveInfo["row"] );
	
	local nBtnIndex = btn:GetClientID();
	
	local t_enableBtns =	{	--"SearchTeamFrame_CopyInfoAllDifficultyChkBtn"..nBtnIndex, 
								--"SearchTeamFrame_CopyInfoSimpleChkBtn"..nBtnIndex, 
								--"SearchTeamFrame_CopyInfoNormalChkBtn"..nBtnIndex, 
								--"SearchTeamFrame_CopyInfoHardChkBtn"..nBtnIndex,
								"SearchTeamFram_CopyInfoBtn"..nBtnIndex
							};
	util.EnableBtns( t_enableBtns );
	
	-- 是否要显示 难度 
	local t_showIcons = { };
	local t_fonts = {};
	if oneActiveInfo["needLv"] ~= nil then
		t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneActiveInfo["name"],},
					[btn:GetName().."LvFont"]	= { value = oneActiveInfo["needLv"] .. "级副本" }, }
	else
		if oneActiveInfo["haveDiff"] then			
			t_showIcons =	{	"SearchTeamFrame_CopyInfoAllDifficultyChkBtn"..nBtnIndex, 
								"SearchTeamFrame_CopyInfoSimpleChkBtn"..nBtnIndex, 
								"SearchTeamFrame_CopyInfoNormalChkBtn"..nBtnIndex, 
								"SearchTeamFrame_CopyInfoHardChkBtn"..nBtnIndex,
								"SearchTeamFrame_CopyInfoProfessionalChkBtn"..nBtnIndex,
							};
			
			t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneActiveInfo["name"], },
						[btn:GetName().."SimpleFont"]	= { value = "简单" },
						[btn:GetName().."NormalFont"]	= { value = "普通" },
						[btn:GetName().."HardFont"]		= { value = "困难" },
						[btn:GetName().."ProfessionalFont"]	= { value = "专家"	},					
					};
		else
			t_showIcons = { "SearchTeamFrame_CopyInfoAllDifficultyChkBtn"..nBtnIndex, };
			t_fonts = {	[btn:GetName().."NameFont"]	= { value = oneActiveInfo["name"], },
						[btn:GetName().."EventFont"]	= { value = "活动副本" }, };
		end
	end

	util.ShowIcons( t_showIcons );
	util.SetFonts( t_fonts );
	
	-- 设置 难度 chk button
	local t_chkBtns = {};
	if oneActiveInfo["row"] == 8 then
		print( "SetSearchTeamFrameBtnByContinue--name = "..oneActiveInfo["name"].." ， joinState = "..t_boolDesc[oneActiveInfo["join"]].."--2113" );
	end
	
	if oneActiveInfo["join"] then		
		t_chkBtns =	{	["SearchTeamFrame_CopyInfoAllDifficultyChkBtn"..nBtnIndex]  = { state = oneActiveInfo["select"] }, 
						["SearchTeamFrame_CopyInfoSimpleChkBtn"..nBtnIndex]			= { state = oneActiveInfo["simple"] },
						["SearchTeamFrame_CopyInfoNormalChkBtn"..nBtnIndex]			= { state = oneActiveInfo["normal"] },
						["SearchTeamFrame_CopyInfoHardChkBtn"..nBtnIndex]			= { state = oneActiveInfo["hard"] },
						["SearchTeamFrame_CopyInfoProfessionalChkBtn"..nBtnIndex]	= { state = oneActiveInfo["professional"] },
					};
		
		local t_canJoinColor	= { r = 255, g = 254, b = 189 };
		local t_canNotJoinColor = { r = 147, g = 130, b = 89 };

		t_fonts = {	[btn:GetName().."NameFont"]			= { color ={ r = 255, g = 254, b = 189 }, },
					[btn:GetName().."SimpleFont"]		= { color = ( oneActiveInfo["joinSimple"] and t_canJoinColor or t_canNotJoinColor ) },
					[btn:GetName().."NormalFont"]		= { color = ( oneActiveInfo["joinNormal"] and t_canJoinColor or t_canNotJoinColor ) },
					[btn:GetName().."HardFont"]			= { color = ( oneActiveInfo["joinHard"] and t_canJoinColor or t_canNotJoinColor ) },
					[btn:GetName().."ProfessionalFont"]	= { color = ( oneActiveInfo["joinProfessional"] and t_canJoinColor or t_canNotJoinColor ) },
				};
	else
		t_chkBtns =	{	["SearchTeamFrame_CopyInfoAllDifficultyChkBtn"..nBtnIndex]  = { state = false }, 
						["SearchTeamFrame_CopyInfoSimpleChkBtn"..nBtnIndex]			= { state = false },
						["SearchTeamFrame_CopyInfoNormalChkBtn"..nBtnIndex]			= { state = false },
						["SearchTeamFrame_CopyInfoHardChkBtn"..nBtnIndex]			= { state = false },
						["SearchTeamFrame_CopyInfoProfessionalChkBtn"..nBtnIndex]	= { state = false },
					};

		t_fonts = {	[btn:GetName().."NameFont"]		= { color ={ r = 147, g = 130, b = 89 }, },
					[btn:GetName().."SimpleFont"]	= { color ={ r = 147, g = 130, b = 89 }, },
					[btn:GetName().."NormalFont"]	= { color ={ r = 147, g = 130, b = 89 }, },
					[btn:GetName().."HardFont"]		= { color ={ r = 147, g = 130, b = 89 }, },
					[btn:GetName().."ProfessionalFont"]	= { color ={ r = 147, g = 130, b = 89 }, },
				};
	end
		
	if oneActiveInfo["name"] == t_searchSelCopyInfo["name"] then
		btn:Checked();
	end
	
	util.SetChkState( t_chkBtns );
	for name, data in pairs( t_fonts ) do
		local font = getglobal( name );
		font:SetTextColor( data["color"]["r"], data["color"]["g"], data["color"]["b"] );
	end
end

function UpdateSearchTeamFrameByContinue()
	local slider = getglobal( "SearchTeamFrame_ScrollBar" );
	local nListStartIndex = 1;
	if slider:IsShown() then
		nListStartIndex = slider:GetValue() + 1;
	end

	local nBtnIndex = 1;
	for i = 1, t_allAutoTeamActives:getTotalNum() do
		if nBtnIndex > MAX_SEARCH_NUM then
			break;
		end

		local data = t_allAutoTeamActives:getActiveData( i );
		SetSearchTeamFrameBtnByContinue( { name = "SearchTeamFram_CopyInfoBtn"..nBtnIndex, oneActiveInfo = data } );
		nBtnIndex = nBtnIndex + 1;
	end

	local nLimit = t_allAutoTeamActives:GetLimitFlag();
	for request, data in pairs( t_allLimit ) do
		local chkBtn = getglobal( data["chkBtn"] );
		chkBtn:setCheckState( bitAnd( request, nLimit ) );
	end
end

function SetSearchTeamFrameBtnByNormal( data )
	local btn			= getglobal( data["name"] );
	local oneActiveInfo	= data["oneActiveInfo"];	
	local nameFont		= getglobal( btn:GetName().."NameFont" );
	nameFont:SetClientID( oneActiveInfo["id"] );
	btn:SetClientUserData( 0, oneActiveInfo["row"] );
	
	local nBtnIndex = btn:GetClientID();
	local t_enableBtns =	{	"SearchTeamFrame_CopyInfoAllDifficultyChkBtn"..nBtnIndex, 
								"SearchTeamFrame_CopyInfoSimpleChkBtn"..nBtnIndex, 
								"SearchTeamFrame_CopyInfoNormalChkBtn"..nBtnIndex, 
								"SearchTeamFrame_CopyInfoHardChkBtn"..nBtnIndex,
								"SearchTeamFrame_CopyInfoProfessionalChkBtn"..nBtnIndex,
								"SearchTeamFram_CopyInfoBtn"..nBtnIndex
							};
	for _, name in ipairs( t_enableBtns ) do
		local enableBtn = getglobal( name );
		enableBtn:Enable();
	end
	
	-- 是否要显示 难度 
	local t_showIcons = { };
	local t_fonts = {};
	if oneActiveInfo["needLv"] ~= nil then
		if oneActiveInfo["haveDiff"] then
			t_fonts = {		[btn:GetName().."NameFont"]		= { value = oneActiveInfo["name"], },
							[btn:GetName().."LvFont"]	= { value = oneActiveInfo["needLv"] .. "级副本",color = { ["r"] = 255, ["g"] = 0, ["b"] = 0 } } }
		else
			t_fonts = {		[btn:GetName().."NameFont"]		= { value = oneActiveInfo["name"], },
							[btn:GetName().."EventFont"]	= { value = "活动副本",color = { ["r"] = 255, ["g"] = 0, ["b"] = 0 } }}
		end
	else
		if oneActiveInfo["haveDiff"] then		
			t_showIcons =	{	"SearchTeamFrame_CopyInfoAllDifficultyChkBtn"..nBtnIndex, 
								"SearchTeamFrame_CopyInfoSimpleChkBtn"..nBtnIndex, 
								"SearchTeamFrame_CopyInfoNormalChkBtn"..nBtnIndex, 
								"SearchTeamFrame_CopyInfoHardChkBtn"..nBtnIndex,
								"SearchTeamFrame_CopyInfoProfessionalChkBtn"..nBtnIndex,
							};
			
			t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneActiveInfo["name"], },
						[btn:GetName().."SimpleFont"]	= { value = "简单" },
						[btn:GetName().."NormalFont"]	= { value = "普通" },
						[btn:GetName().."HardFont"]		= { value = "困难" },
						[btn:GetName().."ProfessionalFont"]	= { value = "专家" },					
					};			
		else
			if oneActiveInfo["finish"] then
				t_fonts = {		[btn:GetName().."NameFont"]		= { value = oneActiveInfo["name"], },
								[btn:GetName().."EventFont"]	= { value = "活动副本",color = { ["r"] = 120, ["g"] = 120, ["b"] = 120 } }}
			else
				if oneActiveInfo["join"] then
					t_showIcons = { "SearchTeamFrame_CopyInfoAllDifficultyChkBtn"..nBtnIndex, };
					t_fonts = {	[btn:GetName().."NameFont"]	= { value = oneActiveInfo["name"], },
								[btn:GetName().."EventFont"]	= { value = "活动副本",color = { ["r"] = 0, ["g"] = 255, ["b"] = 0 } } };
				else
					t_fonts = {	[btn:GetName().."NameFont"]	= { value = oneActiveInfo["name"], },
								[btn:GetName().."EventFont"]	= { value = "活动副本",color = { ["r"] = 255, ["g"] = 0, ["b"] = 0 } } };
				end
			end
		end
	end

	util.ShowIcons( t_showIcons );
	util.SetFonts( t_fonts );
	
	-- 设置 难度 chk button
	local t_chkBtns = {};
	if oneActiveInfo["join"] then		
		t_chkBtns =	{	["SearchTeamFrame_CopyInfoAllDifficultyChkBtn"..nBtnIndex]  = { state = oneActiveInfo["select"] }, 
						["SearchTeamFrame_CopyInfoSimpleChkBtn"..nBtnIndex]			= { state = oneActiveInfo["simple"] },
						["SearchTeamFrame_CopyInfoNormalChkBtn"..nBtnIndex]			= { state = oneActiveInfo["normal"] },
						["SearchTeamFrame_CopyInfoHardChkBtn"..nBtnIndex]			= { state = oneActiveInfo["hard"] },
						["SearchTeamFrame_CopyInfoProfessionalChkBtn"..nBtnIndex]	= { state = oneActiveInfo["professional"] },
					};
		
		local t_canJoinColor	= { r = 255, g = 254, b = 189 };
		local t_canNotJoinColor = { r = 147, g = 130, b = 89 };

		t_fonts = {	[btn:GetName().."NameFont"]			= { color ={ r = 255, g = 254, b = 189 }, },
					[btn:GetName().."SimpleFont"]		= { color = ( oneActiveInfo["joinSimple"] and t_canJoinColor or t_canNotJoinColor ) },
					[btn:GetName().."NormalFont"]		= { color = ( oneActiveInfo["joinNormal"] and t_canJoinColor or t_canNotJoinColor ) },
					[btn:GetName().."HardFont"]			= { color = ( oneActiveInfo["joinHard"] and t_canJoinColor or t_canNotJoinColor ) },
					[btn:GetName().."ProfessionalFont"]	= { color = ( oneActiveInfo["joinProfessional"] and t_canJoinColor or t_canNotJoinColor ) },
				};
	else
		t_chkBtns =	{	["SearchTeamFrame_CopyInfoAllDifficultyChkBtn"..nBtnIndex]  = { state = false }, 
						["SearchTeamFrame_CopyInfoSimpleChkBtn"..nBtnIndex]			= { state = false },
						["SearchTeamFrame_CopyInfoNormalChkBtn"..nBtnIndex]			= { state = false },
						["SearchTeamFrame_CopyInfoHardChkBtn"..nBtnIndex]			= { state = false },
						["SearchTeamFrame_CopyInfoProfessionalChkBtn"..nBtnIndex]	= { state = false },
					};

		t_fonts = {	[btn:GetName().."NameFont"]		= { color ={ r = 147, g = 130, b = 89 }, },
					[btn:GetName().."SimpleFont"]	= { color ={ r = 147, g = 130, b = 89 }, },
					[btn:GetName().."NormalFont"]	= { color ={ r = 147, g = 130, b = 89 }, },
					[btn:GetName().."HardFont"]		= { color ={ r = 147, g = 130, b = 89 }, },
					[btn:GetName().."ProfessionalFont"]	= { color ={ r = 147, g = 130, b = 89 }, },
				};
	end
		
	if oneActiveInfo["name"] == t_searchSelCopyInfo["name"] then
		btn:Checked();
	end
	
	util.SetChkState( t_chkBtns );
	for name, data in pairs( t_fonts ) do
		local font = getglobal( name );
		font:SetTextColor( data["color"]["r"], data["color"]["g"], data["color"]["b"] );
	end
end

function UpdateSearchTeamFrameByNormal()
	local slider = getglobal( "SearchTeamFrame_ScrollBar" );
	local nListStartIndex = 1;
	if slider:IsShown() then
		nListStartIndex = slider:GetValue() + 1;
	end
	
	local nBtnIndex = 1;
	for i = 1, t_allAutoTeamActives:getTotalNum() do
		if nBtnIndex > MAX_SEARCH_NUM then
			break;
		end

		local data = t_allAutoTeamActives:getActiveData( i );
		SetSearchTeamFrameBtnByNormal( { name = "SearchTeamFram_CopyInfoBtn"..nBtnIndex, oneActiveInfo = data } );
		nBtnIndex = nBtnIndex + 1;
	end

	-- 检查顶部的全选按钮
	
	local t_chkSynchronize = { "SearchTeamFrame_SelectAllSimpleChkBtn", "SearchTeamFrame_SelectAllNormalChkBtn", 
								"SearchTeamFrame_SelectAllHardChkBtn", "SearchTeamFrame_SelectAllProfessionalChkBtn" };
	for _, name in ipairs( t_chkSynchronize ) do
		local chkBtn = getglobal( name );
		chkBtn:setCheckState( chkBtn:GetCheckState() );
	end
	
	local bAllSimpleSel = t_allAutoTeamActives:isAllSimpleSelect();
	local bAllNormalSel = t_allAutoTeamActives:isAllNormalSelect();
	local bAllHardSel	= t_allAutoTeamActives:isAllHardSelect();
	local bAllProfessionalSel= t_allAutoTeamActives:isAllProfessionalSelect();
	local bAllActiveSel	= t_allAutoTeamActives:isAllActiveSelect();
	local t_chkBtns = 
	{	["SearchTeamFrame_SelectAllSimpleChkBtn"]	= { state = bAllSimpleSel }, 
		["SearchTeamFrame_SelectAllNormalChkBtn"]	= { state = bAllNormalSel },
		["SearchTeamFrame_SelectAllHardChkBtn"]		= { state = bAllHardSel },
		["SearchTeamFrame_SelectAllProfessionalChkBtn"]	= { state = bAllProfessionalSel },
		["SearchTeamFrame_SelectAllChkBtn"]			= { state = ( bAllSimpleSel and bAllNormalSel and bAllHardSel and bAllProfessionalSel 
																	and bAllActiveSel ) },
	};
	util.SetChkState( t_chkBtns );
	

	local nLimit = t_allAutoTeamActives:GetLimitFlag();
	for request, data in pairs( t_allLimit ) do
		local chkBtn = getglobal( data["chkBtn"] );
		chkBtn:setCheckState( bitAnd( request, nLimit ) );
	end

	local t_enableBtns =	{	"SearchTeamFrame_SelectAllSimpleChkBtn", 
								"SearchTeamFrame_SelectAllNormalChkBtn", 
								"SearchTeamFrame_SelectAllHardChkBtn", 
								"SearchTeamFrame_SelectAllProfessionalChkBtn",
								"SearchTeamFrame_SelectAllChkBtn",
								};
	util.EnableBtns( t_enableBtns );
end

function UpdateSearchTeamFrame()
	ClearSearchTeamFram();
	
	if t_allAutoTeamActives:GetContinueAutoTeam() then
		UpdateSearchTeamFrameByContinue();
	else
		UpdateSearchTeamFrameByNormal();
	end
end

local t_SearchTeamFramEvents = { ["GE_PROMOTION_UPDATE"] = {}, ["GE_PLAYER_LEAVEUP"] = {}, ["GE_LEAVE_PWORLD"] = {}, ["GE_ENTER_PWORLD"] = {}, 
								["GE_AUTO_TEAM_RESET"] = {}, ["UI_ACTOR_VIG_CHG"] = {} };

t_SearchTeamFramEvents["GE_PROMOTION_UPDATE"].func = 
function ()
	if not SearchTeamFrame:IsShown() then
		return;
	end

	t_allAutoTeamActives:SynchronizePromotion();
	UpdateSearchTeamFrame();
end

t_SearchTeamFramEvents["GE_LEAVE_PWORLD"].func = 
function ()
	t_SearchTeamFramEvents["GE_PROMOTION_UPDATE"].func();
end

t_SearchTeamFramEvents["GE_ENTER_PWORLD"].func = 
function ()
	t_SearchTeamFramEvents["GE_PROMOTION_UPDATE"].func();
end

t_SearchTeamFramEvents["GE_PLAYER_LEAVEUP"].func = 
function ()
	t_SearchTeamFramEvents["GE_PROMOTION_UPDATE"].func();
end

t_SearchTeamFramEvents["UI_ACTOR_VIG_CHG"].func = 
function ()
	t_SearchTeamFramEvents["GE_PROMOTION_UPDATE"].func();
end

t_SearchTeamFramEvents["GE_AUTO_TEAM_RESET"].func = 
function ()
	InitAutoTeamData();
	
	local t_hideIcons = { "SuccessCreatTaskAutoTeamFrame","CreatingAutoTeamFrame", "SuccessCreatActiveAutoTeamFrame", "ApplyCreateTeamFirstFrame",
						"WarnNotSelectDiffFrame", "ConfirmDifficultyFrame", "ConfirmDifficultyAndCareerFrame", "WarnAutoTeamFailFrame",
						"WarnTimeCountDownFrame", "ConfirmQuitAutoTeamFrame", "AutoTeamEnterPworldTimeDownFrame" };
	util.HideIcons( t_hideIcons );

	SetSearchSelCopyInfo( { name = "" } );
	-- 刚打开界面默认选中第一个副本，第一个副本可能不满足条件
	SetCurSuoXiaoFrameInfo( { name = "SearchTeamFrame" } );
	
	local frame = getglobal( "SearchTeamFrame" );
	if frame:IsShown() then
		local slider = getglobal( "SearchTeamFrame_ScrollBar" );
		--slider:SetValue( 0 );
		SetScrollBar( slider:GetName(), MAX_SEARCH_NUM, t_allAutoTeamActives:getTotalNum() );

		t_allAutoTeamActives:SetDefault();
		UpdateSearchTeamFrame();
	else
		t_allAutoTeamActives:Init();
	end
end

function SearchTeamFrame_OnLoad()
	for event, _ in pairs( t_SearchTeamFramEvents ) do
		this:RegisterEvent( event );
	end

	t_SearchTeamFramEvents.__index = function ( tab, key )
										tab[key] = { func = function () end };
										return tab[key];
									end
	setmetatable( t_SearchTeamFramEvents, t_SearchTeamFramEvents );

	t_allAutoTeamActives:AddLimitFlag( 0 );
	t_allAutoTeamActives:SetResetFlag( true );
	t_allAutoTeamActives:SetContinueAutoTeam( false );
end

function SearchTeamFram_OnEvent()
	t_SearchTeamFramEvents[arg1].func();
end

local t_searchTeamFrameMutexFrames = {  "PromotionFrame", "SuccessCreatTaskAutoTeamFrame", "CreatingAutoTeamFrame", 
										"SuccessCreatActiveAutoTeamFrame" };

-- 继续自动组队
function ContinueAutoTeam()
	t_allAutoTeamActives:ClearAllChoice();
	t_allAutoTeamActives:SetContinueAutoTeam( true );
	t_allAutoTeamActives:SetResetFlag( false );

	local nAutoTeamID = AutoTeamManager:getAutoTeamID();
	local nDifficulty = AutoTeamManager:getPworldDiffulty();
	for i = 1, t_allAutoTeamActives:getTotalNum() do
		local oneActiveData = t_allAutoTeamActives:getActiveData( i );
		local nResult		= GamePromotion:getAutoTeamStateByNoCountLimit( oneActiveData["row"] );
		local bCanJoin		= ( nResult == PROMOTION_OK );
		
		print( "ContinueAutoTeam--name = "..oneActiveData["name"].." ， nResult = "..nResult.."--2433" );
		if oneActiveData["teamID"] == nAutoTeamID and bCanJoin then
			t_allAutoTeamActives:setActiveData( i, 
				{	id			= oneActiveData["id"],					simple	= ( nDifficulty == CLT_PWORLD_SIMPLE ),	
					normal		= ( nDifficulty == CLT_PWORLD_NORMAL ),	hard	= ( nDifficulty == CLT_PWORLD_HARD ),
					professional= ( nDifficulty == CLT_PWORLD_PROFESSIONAL ),
					haveDiff	= oneActiveData["haveDiff"],			select	= true,	
					join		= bCanJoin,								name	= oneActiveData["name"],		
					row			= oneActiveData["row"],					teamID	= oneActiveData["teamID"],
					joinSimple	= oneActiveData["joinSimple"],			joinNormal		= oneActiveData["joinNormal"],
					joinHard	= oneActiveData["joinHard"],			joinProfessional= oneActiveData["joinProfessional"],
				} 
			);
		end
	end
end

function SearchTeamFram_OnShow()
	for _, frameName in ipairs( t_searchTeamFrameMutexFrames ) do
		local frame = getglobal( frameName );
		if frame:IsShown() then
			frame:Hide();
		end
	end

	SetSearchSelCopyInfo( { name = "" } );

	if t_allAutoTeamActives:NeedSetDefault() then
		print( "SearchTeamFram_OnShow--SetDefault" );
		t_allAutoTeamActives:SetDefault();
	else
		print( "SearchTeamFram_OnShow--SynchronizePromotion" );
		t_allAutoTeamActives:SynchronizePromotion();
	end
	
	local slider = getglobal( "SearchTeamFrame_ScrollBar" );
	slider:SetValue( 0 );
	SetScrollBar( slider:GetName(), MAX_SEARCH_NUM, t_allAutoTeamActives:getTotalNum() );
	-- 刚打开界面默认选中第一个副本，第一个副本可能不满足条件
	UpdateSearchTeamFrame();

	this:SetPoint( "center", "UIClient", "center", 0, 0 );
	SetCurSuoXiaoFrameInfo( { name = this:GetName() } );
end

function SearchTeamFrame_CloseBtn_OnClick()
	SearchTeamFrame:Hide();
	--SetCurSuoXiaoFrameInfo( { name = this:GetParent() } );
end

local t_cannotJoinActive =	{	[PROMOTION_SINGLE_LIMIT]	= { failDesc = "不能组队进入，要单人才可进入", str = "PROMOTION_SINGLE_LIMIT" }, 
								[PROMOTION_NOT_START]		= { failDesc = "活动还没开始", str = "PROMOTION_NOT_START" }, 
								[PROMOTION_LEVEL_LIMIT]		= { failDesc = "等级不够", str = "PROMOTION_LEVEL_LIMIT" },
								[PROMOTION_HAVE_FINISH]		= { failDesc = "活动已结束", str = "PROMOTION_HAVE_FINISH" },
							};
function SearchTeamFrame_CopyInfoChkBtn_OnEnter()
	local slider		= getglobal( "SearchTeamFrame_ScrollBar" );
	local copyInfoBtn	= getglobal( "SearchTeamFram_CopyInfoBtn"..this:GetClientID() );
	local nRow			= copyInfoBtn:GetClientUserData( 0 );
	local nameFont		= getglobal( copyInfoBtn:GetName().. "NameFont" );
	copyInfoBtn:Highlight();
	-- 要弹出tips
	local oneActiveData = t_allAutoTeamActives:GetActiveDataByRow( { row = nRow } );
	if oneActiveData == nil then
		return;
	end

	local nRow		= oneActiveData["row"];
	local nResult	= GamePromotion:getAutoTeamState( nRow );

	ShowGameTooltip( { rich = "TempGameTooltipRichText", tipsFrame = "TempGameTooltip", text = GetPromotionDesc( nRow ), 
							frame = this:GetParent(), button = "freedom", conflictFrame = this:GetName(),
							yAdjust = 0 , ["verticalReferencePoint"]	= "vertical", ["vertical"] = "bottom" } );
end

function SearchTeamFrame_CopyInfoChkBtn_OnLeave()
	local copyInfoBtn = getglobal( "SearchTeamFram_CopyInfoBtn"..this:GetClientID() );
	copyInfoBtn:DisHighlight();
	-- 隐藏出tips
	GameTooltip:Hide();
	TempGameTooltip:Hide();
end

function SearchTeamFram_CopyInfoBtn_OnClick()
	local nameFont = getglobal( this:GetName().."NameFont" );
	 t_searchSelCopyInfo = { name = nameFont:GetText() };
	 DisCheckAllBtn( "SearchTeamFram_CopyInfoBtn", MAX_SEARCH_NUM );
	 this:Checked();
end

local t_headBtnsMap = 
{
	["SearchTeamFrame_SelectAllHeadBtn"]		= { chkBtn = "SearchTeamFrame_SelectAllChkBtn" },
	["SearchTeamFrame_SelectAll_SimpleBtn"]		= { chkBtn = "SearchTeamFrame_SelectAllSimpleChkBtn" },
	["SearchTeamFrame_SelectAll_NormalBtn"]		= { chkBtn = "SearchTeamFrame_SelectAllNormalChkBtn" },
	["SearchTeamFrame_SelectAll_HardBtn"]		= { chkBtn = "SearchTeamFrame_SelectAllHardChkBtn" },
	["SearchTeamFrame_SelectAll_ProfessionalBtn"]= { chkBtn = "SearchTeamFrame_SelectAllProfessionalChkBtn" },
};

-- 是否可以点击全选按钮
function CanClickAllSelectChkBtnWithoutDifficulty()
	if t_allAutoTeamActives:isCanJoinActiveEmpty() then
		ShowMidTips( "没有可以进入的活动" );
		return false;
	end

	return true;
end

function CanClickAllSelectChkBtnWithDifficulty()
	if t_allAutoTeamActives:isCanJoinActiveEmpty() then
		ShowMidTips( "没有可以进入的活动" );
		return false;
	end

	if t_allAutoTeamActives:getCanJoinWithDiffNum() == 0 then
		ShowMidTips( "没有活动需要勾选难度" );
		return false;
	end

	return true;
end


-- 点击全选按钮
function SearchTeamFrame_SelectAllChkBtn_OnClick()
	if not CanChangeOption() then
		this:setCheckState( not this:GetCheckState() );
		return;
	end

	if not CanClickAllSelectChkBtnWithoutDifficulty() then
		this:setCheckState( false );
		return;
	end
	
	for i = 1, t_allAutoTeamActives:getTotalNum() do
		local oneActiveData = t_allAutoTeamActives:getActiveData( i );
		-- 若是当前主角可以参加的
		if oneActiveData["join"] then
			local isCurSimpleCanJoin	= oneActiveData["joinSimple"];
			local canJoinSimple			= t_allAutoTeamActives:getOneDifficultyOption( {	["lastSelectState"] = this:GetCheckState(), 
																			["curState"]		= isCurSimpleCanJoin, } );

			local isCurNormalCanJoin	= oneActiveData["joinNormal"];
			local canJoinNormal			= t_allAutoTeamActives:getOneDifficultyOption( {	["lastSelectState"] = this:GetCheckState(), 
																			["curState"]		= isCurNormalCanJoin } );

			local isCurHardCanJoin	= oneActiveData["joinHard"];
			local canJoinHard		= t_allAutoTeamActives:getOneDifficultyOption( {	["lastSelectState"] = this:GetCheckState(),
																		["curState"]		= isCurHardCanJoin } );

			local isCurProfessionalCanJoin	= oneActiveData["joinProfessional"];
			local canJoinProfessional		= t_allAutoTeamActives:getOneDifficultyOption( {	["lastSelectState"] = this:GetCheckState(),
																				["curState"] = isCurProfessionalCanJoin } );

			if not CanJoinPworldUseVig( oneActiveData["id"], PworldDifficulty.EASY ) then
				canJoinSimple = false;
			end
			if not CanJoinPworldUseVig( oneActiveData["id"], PworldDifficulty.GENERAL ) then
				canJoinNormal = false;
			end
			if not CanJoinPworldUseVig( oneActiveData["id"], PworldDifficulty.HARD ) then
				canJoinHard = false;
			end
			if not CanJoinPworldUseVig( oneActiveData["id"], PworldDifficulty.EXPERT ) then
				canJoinProfessional = false;
			end
			
			t_allAutoTeamActives:setActiveData( i, 
				{	id		= oneActiveData["id"],			simple		= canJoinSimple,				normal		= canJoinNormal,	
					hard	= canJoinHard,					professional= canJoinProfessional,			haveDiff	= oneActiveData["haveDiff"],	
					select	= this:GetCheckState(),			join		= oneActiveData["join"],		name		= oneActiveData["name"],		
					row		= oneActiveData["row"],			teamID		= oneActiveData["teamID"],
					joinSimple	= oneActiveData["joinSimple"],	joinNormal		= oneActiveData["joinNormal"],
					joinHard	= oneActiveData["joinHard"],	joinProfessional= oneActiveData["joinProfessional"],
				} 
			);
		end
	end

	UpdateSearchTeamFrame();

	local t_childAllSelectChkBtns = { "SearchTeamFrame_SelectAllSimpleChkBtn", "SearchTeamFrame_SelectAllNormalChkBtn", 
										"SearchTeamFrame_SelectAllHardChkBtn", "SearchTeamFrame_SelectAllProfessionalChkBtn" };
	for _, name in ipairs( t_childAllSelectChkBtns ) do
		local chkBtn = getglobal( name );
		chkBtn:setCheckState( this:GetCheckState() );
	end
end

function SearchTeamFrame_SelectAllHeadBtn_OnClick()
	if not CanChangeOption() then
		return;
	end

	if not CanClickAllSelectChkBtnWithoutDifficulty() then
		return;
	end
	
	local chkBtn = getglobal( t_headBtnsMap[this:GetName()]["chkBtn"] );
	chkBtn:setCheckState( not chkBtn:GetCheckState() );
	for i = 1, t_allAutoTeamActives:getTotalNum() do
		local oneActiveData = t_allAutoTeamActives:getActiveData( i );
		-- 若是当前主角可以参加的
		if oneActiveData["join"] then
			local isCurSimpleCanJoin	= oneActiveData["joinSimple"];
			local canJoinSimple			= t_allAutoTeamActives:getOneDifficultyOption( {	["lastSelectState"] = chkBtn:GetCheckState(), 
																			["curState"]		= isCurSimpleCanJoin, } );

			local isCurNormalCanJoin	= oneActiveData["joinNormal"];
			local canJoinNormal			= t_allAutoTeamActives:getOneDifficultyOption( {	["lastSelectState"] = chkBtn:GetCheckState(), 
																			["curState"]		= isCurNormalCanJoin } );

			local isCurHardCanJoin	= oneActiveData["joinHard"];
			local canJoinHard		= t_allAutoTeamActives:getOneDifficultyOption( {	["lastSelectState"] = chkBtn:GetCheckState(),
																		["curState"]		= isCurHardCanJoin } );

			local isCurProfessionalCanJoin	= oneActiveData["joinProfessional"];
			local canJoinProfessional		= t_allAutoTeamActives:getOneDifficultyOption( {	["lastSelectState"] = chkBtn:GetCheckState(),
																				["curState"] = isCurProfessionalCanJoin } );
			if not CanJoinPworldUseVig( oneActiveData["id"], PworldDifficulty.EASY ) then
				canJoinSimple = false;
			end
			if not CanJoinPworldUseVig( oneActiveData["id"], PworldDifficulty.GENERAL ) then
				canJoinNormal = false;
			end
			if not CanJoinPworldUseVig( oneActiveData["id"], PworldDifficulty.HARD ) then
				canJoinHard = false;
			end
			if not CanJoinPworldUseVig( oneActiveData["id"], PworldDifficulty.EXPERT ) then
				canJoinProfessional = false;
			end
			
			t_allAutoTeamActives:setActiveData( i, 
				{	id		= oneActiveData["id"],			simple		= canJoinSimple,				normal	= canJoinNormal,	
					hard	= canJoinHard,					professional= canJoinProfessional,			haveDiff= oneActiveData["haveDiff"],	
					select	= chkBtn:GetCheckState(),		join		= oneActiveData["join"],		name	= oneActiveData["name"],		
					row		= oneActiveData["row"],			teamID	= oneActiveData["teamID"],
					joinSimple	= oneActiveData["joinSimple"],	joinNormal		= oneActiveData["joinNormal"],
					joinHard	= oneActiveData["joinHard"],	joinProfessional= oneActiveData["joinProfessional"],
				} 
			);
		end
	end

	UpdateSearchTeamFrame();

	local t_childAllSelectChkBtns = { "SearchTeamFrame_SelectAllSimpleChkBtn", "SearchTeamFrame_SelectAllNormalChkBtn", 
										"SearchTeamFrame_SelectAllHardChkBtn", "SearchTeamFrame_SelectAllProfessionalChkBtn" };
	for _, name in ipairs( t_childAllSelectChkBtns ) do
		local chkBtn = getglobal( name );
		chkBtn:setCheckState( chkBtn:GetCheckState() );
	end
end

-- 点击简单全选
function SearchTeamFrame_SelectAllSimpleChkBtn_OnClick()
	if not CanChangeOption() then
		this:setCheckState( not this:GetCheckState() );
		return;
	end
	
	if this:GetCheckState() and not t_allAutoTeamActives:isAllHaveDiffSelect() then
		this:setCheckState( false );
		ShowMidTips( "部分可参加活动没勾选，请先勾选" );
		return;
	end

	if not CanClickAllSelectChkBtnWithDifficulty() then
		this:setCheckState( false );
		return;
	end
	
	for i = 1, t_allAutoTeamActives:getTotalNum() do
		local oneActiveData = t_allAutoTeamActives:getActiveData( i );
		-- 若是当前主角可以参加的
		if oneActiveData["join"] then
			local canJoinSimple = this:GetCheckState();
			if not oneActiveData["joinSimple"] then
				canJoinSimple = false;
			end

			if not CanJoinPworldUseVig( oneActiveData["id"], PworldDifficulty.EASY ) then
				canJoinSimple = false;
			end

			t_allAutoTeamActives:setActiveData( i, 
				{	id		= oneActiveData["id"],				simple		= canJoinSimple,				normal	= oneActiveData["normal"],	
					hard	= oneActiveData["hard"],			professional= oneActiveData["professional"],haveDiff= oneActiveData["haveDiff"],	
					select	= oneActiveData["select"],			join		= oneActiveData["join"],		name	= oneActiveData["name"],		
					row		= oneActiveData["row"],				teamID		= oneActiveData["teamID"],
					joinSimple	= oneActiveData["joinSimple"],	joinNormal		= oneActiveData["joinNormal"],
					joinHard	= oneActiveData["joinHard"],	joinProfessional= oneActiveData["joinProfessional"],
				} 
			);
		end
	end

	UpdateSearchTeamFrame();
end

function SearchTeamFrame_SelectAll_SimpleBtn_OnClick()
	if not CanChangeOption() then
		return;
	end
	
	local chkBtn = getglobal( t_headBtnsMap[this:GetName()]["chkBtn"] );
	if not chkBtn:GetCheckState() and not t_allAutoTeamActives:isAllHaveDiffSelect() then
		ShowMidTips( "部分可参加活动没勾选，请先勾选" );
		return;
	end

	if not CanClickAllSelectChkBtnWithDifficulty() then
		return;
	end

	chkBtn:setCheckState( not chkBtn:GetCheckState() );

	for i = 1, t_allAutoTeamActives:getTotalNum() do
		local oneActiveData = t_allAutoTeamActives:getActiveData( i );
		-- 若是当前主角可以参加的
		local canJoinSimple = chkBtn:GetCheckState();
		if not oneActiveData["joinSimple"] then
			canJoinSimple = false;
		end

		if not CanJoinPworldUseVig( oneActiveData["id"], PworldDifficulty.EASY ) then
			canJoinSimple = false;
		end

		if oneActiveData["join"] then
			t_allAutoTeamActives:setActiveData( i, 
				{	id		= oneActiveData["id"],				simple		= canJoinSimple,				normal	= oneActiveData["normal"],	
					hard	= oneActiveData["hard"],			professional= oneActiveData["professional"],haveDiff= oneActiveData["haveDiff"],	
					select	= oneActiveData["select"],			join		= oneActiveData["join"],		name	= oneActiveData["name"],		row		= oneActiveData["row"],
					teamID	= oneActiveData["teamID"],
					joinSimple	= oneActiveData["joinSimple"],	joinNormal		= oneActiveData["joinNormal"],
					joinHard	= oneActiveData["joinHard"],	joinProfessional= oneActiveData["joinProfessional"],
				} 
			);
		end
	end

	UpdateSearchTeamFrame();
end

-- 点击普通全选
function SearchTeamFrame_SelectAllNormalChkBtn_OnClick()
	if not CanChangeOption() then
		this:setCheckState( not this:GetCheckState() );
		return;
	end
	
	if  this:GetCheckState() and not t_allAutoTeamActives:isAllHaveDiffSelect() then
		ShowMidTips( "部分可参加活动没勾选，请先勾选" );
		this:setCheckState( false );
		return;
	end

	if not CanClickAllSelectChkBtnWithDifficulty() then
		this:setCheckState( false );
		return;
	end
	
	for i = 1, t_allAutoTeamActives:getTotalNum() do
		local oneActiveData = t_allAutoTeamActives:getActiveData( i );
		-- 若是当前主角可以参加的
		local canJoinNormal = this:GetCheckState();
		if not oneActiveData["joinNormal"] then
			canJoinNormal = false;
		end

		if not CanJoinPworldUseVig( oneActiveData["id"], PworldDifficulty.GENERAL ) then
			canJoinNormal = false;
		end

		if oneActiveData["join"] then
			t_allAutoTeamActives:setActiveData( i, 
				{	id		= oneActiveData["id"],				simple		= oneActiveData["simple"],		normal	= canJoinNormal,	
					hard	= oneActiveData["hard"],			professional= oneActiveData["professional"],haveDiff= oneActiveData["haveDiff"],
					select	= oneActiveData["select"],			join		= oneActiveData["join"],		name	= oneActiveData["name"],		
					row		= oneActiveData["row"],				teamID		= oneActiveData["teamID"],
					joinSimple	= oneActiveData["joinSimple"],	joinNormal		= oneActiveData["joinNormal"],
					joinHard	= oneActiveData["joinHard"],	joinProfessional= oneActiveData["joinProfessional"],
				} 
			);
		end
	end

	UpdateSearchTeamFrame();
end

function SearchTeamFrame_SelectAll_NormalBtn_OnClick()
	if not CanChangeOption() then
		return;
	end
	
	local chkBtn = getglobal( t_headBtnsMap[this:GetName()]["chkBtn"] );
	if not chkBtn:GetCheckState() and not t_allAutoTeamActives:isAllHaveDiffSelect() then
		ShowMidTips( "部分可参加活动没勾选，请先勾选" );
		return;
	end

	if not CanClickAllSelectChkBtnWithDifficulty() then
		return;
	end
	
	
	chkBtn:setCheckState( not chkBtn:GetCheckState() );

	for i = 1, t_allAutoTeamActives:getTotalNum() do
		local oneActiveData = t_allAutoTeamActives:getActiveData( i );
		-- 若是当前主角可以参加的
		local canJoinNormal = chkBtn:GetCheckState();
		if not oneActiveData["joinNormal"] then
			canJoinNormal = false;
		end

		if not CanJoinPworldUseVig( oneActiveData["id"], PworldDifficulty.GENERAL ) then
			canJoinNormal = false;
		end

		if oneActiveData["join"] then
			t_allAutoTeamActives:setActiveData( i, 
				{	id		= oneActiveData["id"],				simple		= oneActiveData["simple"],		normal	= canJoinNormal,	
					hard	= oneActiveData["hard"],			professional= oneActiveData["professional"],haveDiff= oneActiveData["haveDiff"],
					select	= oneActiveData["select"],			join		= oneActiveData["join"],		name	= oneActiveData["name"],		
					row		= oneActiveData["row"],				teamID		= oneActiveData["teamID"],
					joinSimple	= oneActiveData["joinSimple"],	joinNormal		= oneActiveData["joinNormal"],
					joinHard	= oneActiveData["joinHard"],	joinProfessional= oneActiveData["joinProfessional"],
				} 
			);
		end
	end

	UpdateSearchTeamFrame();
end

-- 点击困难全选
function SearchTeamFrame_SelectAllHardChkBtn_OnClick()
	if not CanChangeOption() then
		this:setCheckState( not this:GetCheckState() );
		return;
	end
	
	if this:GetCheckState() and not t_allAutoTeamActives:isAllHaveDiffSelect() then
		this:setCheckState( false );
		ShowMidTips( "部分可参加活动没勾选，请先勾选" );
		return;
	end

	if not CanClickAllSelectChkBtnWithDifficulty() then
		this:setCheckState( false );
		return;
	end
	
	for i = 1, t_allAutoTeamActives:getTotalNum() do
		local oneActiveData = t_allAutoTeamActives:getActiveData( i );
		-- 若是当前主角可以参加的
		local canJoinHard = this:GetCheckState();
		if not oneActiveData["joinHard"] then
			canJoinHard = false;
		end

		if not CanJoinPworldUseVig( oneActiveData["id"], PworldDifficulty.HARD ) then
			canJoinHard = false;
		end

		if oneActiveData["join"] then
			t_allAutoTeamActives:setActiveData( i, 
				{	id		= oneActiveData["id"],			simple		= oneActiveData["simple"],		normal	= oneActiveData["normal"],	
					hard	= canJoinHard,					professional= oneActiveData["professional"],haveDiff= oneActiveData["haveDiff"],	
					select	= oneActiveData["select"],		join		= oneActiveData["join"],		name	= oneActiveData["name"],		
					row		= oneActiveData["row"],			teamID	= oneActiveData["teamID"],
					joinSimple	= oneActiveData["joinSimple"],	joinNormal		= oneActiveData["joinNormal"],
					joinHard	= oneActiveData["joinHard"],	joinProfessional= oneActiveData["joinProfessional"],
				} 
			);
		end
	end

	UpdateSearchTeamFrame();
end

function SearchTeamFrame_SelectAll_HardBtn_OnClick()
	if not CanChangeOption() then
		return;
	end
	
	local chkBtn = getglobal( t_headBtnsMap[this:GetName()]["chkBtn"] );
	if not chkBtn:GetCheckState() and not t_allAutoTeamActives:isAllHaveDiffSelect() then
		ShowMidTips( "部分可参加活动没勾选，请先勾选" );
		return;
	end

	if not CanClickAllSelectChkBtnWithDifficulty() then
		return;
	end
	
	chkBtn:setCheckState( not chkBtn:GetCheckState() );
	for i = 1, t_allAutoTeamActives:getTotalNum() do
		local oneActiveData = t_allAutoTeamActives:getActiveData( i );
		-- 若是当前主角可以参加的
		local canJoinHard = chkBtn:GetCheckState();
		if not oneActiveData["joinHard"] then
			canJoinHard = false;
		end

		if not CanJoinPworldUseVig( oneActiveData["id"], PworldDifficulty.HARD ) then
			canJoinHard = false;
		end

		if oneActiveData["join"] then
			t_allAutoTeamActives:setActiveData( i, 
				{	id		= oneActiveData["id"],			simple		= oneActiveData["simple"],		normal	= oneActiveData["normal"],	
					hard	= canJoinHard,					professional= oneActiveData["professional"],haveDiff= oneActiveData["haveDiff"],	
					select	= oneActiveData["select"],		join		= oneActiveData["join"],		name	= oneActiveData["name"],		
					row		= oneActiveData["row"],			teamID	= oneActiveData["teamID"],
					joinSimple	= oneActiveData["joinSimple"],joinNormal		= oneActiveData["joinNormal"],
					joinHard	= oneActiveData["joinHard"],	joinProfessional= oneActiveData["joinProfessional"],
				} 
			);
		end
	end

	UpdateSearchTeamFrame();
end

-- 点击专家全选
function SearchTeamFrame_SelectAllProfessionalChkBtn_OnClick()
	if not CanChangeOption() then
		this:setCheckState( not this:GetCheckState() );
		return;
	end
	
	if this:GetCheckState() and not t_allAutoTeamActives:isAllHaveDiffSelect() then
		this:setCheckState( false );
		ShowMidTips( "部分可参加活动没勾选，请先勾选" );
		return;
	end

	if not CanClickAllSelectChkBtnWithDifficulty() then
		this:setCheckState( false );
		return;
	end
	
	
	for i = 1, t_allAutoTeamActives:getTotalNum() do
		local oneActiveData = t_allAutoTeamActives:getActiveData( i );
		local canJoinProfessional = this:GetCheckState();
		if not oneActiveData["joinProfessional"] then
			canJoinProfessional = false;
		end

		if not CanJoinPworldUseVig( oneActiveData["id"], PworldDifficulty.EXPERT ) then
			canJoinProfessional = false;
		end

		-- 若是当前主角可以参加的
		if oneActiveData["join"] then
			t_allAutoTeamActives:setActiveData( i, 
				{	id		= oneActiveData["id"],			simple		= oneActiveData["simple"],		normal		= oneActiveData["normal"],	
					hard	= oneActiveData["hard"],		professional= canJoinProfessional,			haveDiff	= oneActiveData["haveDiff"],	
					select	= oneActiveData["select"],		join	= oneActiveData["join"],			name		= oneActiveData["name"],		
					row		= oneActiveData["row"],			teamID	= oneActiveData["teamID"],
					joinSimple	= oneActiveData["joinSimple"],	joinNormal		= oneActiveData["joinNormal"],
					joinHard	= oneActiveData["joinHard"],	joinProfessional= oneActiveData["joinProfessional"],
				} 
			);
		end
	end

	UpdateSearchTeamFrame();
end

function SearchTeamFrame_SelectAll_ProfessionalBtn_OnClick()
	if not CanChangeOption() then
		return;
	end
	
	local chkBtn = getglobal( t_headBtnsMap[this:GetName()]["chkBtn"] );
	if not chkBtn:GetCheckState() and not t_allAutoTeamActives:isAllHaveDiffSelect() then
		ShowMidTips( "部分可参加活动没勾选，请先勾选" );
		return;
	end

	if not CanClickAllSelectChkBtnWithDifficulty() then
		return;
	end
	
	
	chkBtn:setCheckState( not chkBtn:GetCheckState() );
	for i = 1, t_allAutoTeamActives:getTotalNum() do
		local oneActiveData = t_allAutoTeamActives:getActiveData( i );
		-- 若是当前主角可以参加的
		if oneActiveData["join"] then
			local canJoinProfessional = chkBtn:GetCheckState();
			if not oneActiveData["joinProfessional"] then
				canJoinProfessional = false;
			end

			if not CanJoinPworldUseVig( oneActiveData["id"], PworldDifficulty.EXPERT ) then
				canJoinProfessional = false;
			end
			t_allAutoTeamActives:setActiveData( i, 
				{	id		= oneActiveData["id"],			simple		= oneActiveData["simple"],		normal		= oneActiveData["normal"],	
					hard	= oneActiveData["hard"],		professional= canJoinProfessional,		haveDiff	= oneActiveData["haveDiff"],	
					select	= oneActiveData["select"],		join		= oneActiveData["join"],		name		= oneActiveData["name"],		
					row		= oneActiveData["row"],			teamID		= oneActiveData["teamID"],
					joinSimple	= oneActiveData["joinSimple"],	joinNormal		= oneActiveData["joinNormal"],
					joinHard	= oneActiveData["joinHard"],	joinProfessional= oneActiveData["joinProfessional"],
				} 
			);
		end
	end

	UpdateSearchTeamFrame();
end

-- 点副本难度名字前面的chk按钮
function SearchTeamFrame_CopyInfoAllDifficultyChkBtn_OnClick()
	if not CanChangeOption() then
		this:setCheckState( not this:GetCheckState() );
		return;
	end

	local copyInfoBtn		= getglobal( "SearchTeamFram_CopyInfoBtn"..this:GetClientID() );
	local nameFont			= getglobal( "SearchTeamFram_CopyInfoBtn"..this:GetClientID().."NameFont" );
	local nRow				= copyInfoBtn:GetClientUserData( 0 );
	local t_oneActiveInfo	= t_allAutoTeamActives:GetActiveDataByRow( { row = nRow } );
	if not t_oneActiveInfo["join"] then
		this:setCheckState( false );
		return;
	end
	
	local nIndex = t_allAutoTeamActives:GetActiveIndexByRow( { row = nRow } );
	t_allAutoTeamActives:setActiveData( nIndex, 
										{	name	= t_oneActiveInfo["name"],			simple	= false,					
											normal	= false,							hard	= false,
											professional= false,	
											id		= t_oneActiveInfo["id"],		
											haveDiff= t_oneActiveInfo["haveDiff"],		join = t_oneActiveInfo["join"],	
											select	= this:GetCheckState(),
											row		= t_oneActiveInfo["row"],			teamID	= t_oneActiveInfo["teamID"],
											joinSimple	= t_oneActiveInfo["joinSimple"],joinNormal	= t_oneActiveInfo["joinNormal"],
											joinHard	= t_oneActiveInfo["joinHard"],	joinProfessional= t_oneActiveInfo["joinProfessional"],
										} );
	UpdateSearchTeamFrame();
end

-- 点副本难度的简单chk按钮
function SearchTeamFrame_CopyInfoSimpleChkBtn_OnClick()
	if not CanChangeOption() then
		this:setCheckState( not this:GetCheckState() );
		return;
	end

	local copyInfoBtn		= getglobal( "SearchTeamFram_CopyInfoBtn"..this:GetClientID() );
	local nameFont			= getglobal( "SearchTeamFram_CopyInfoBtn"..this:GetClientID().."NameFont" );
	local nRow				= copyInfoBtn:GetClientUserData( 0 );
	local t_oneActiveInfo	= t_allAutoTeamActives:GetActiveDataByRow( { row = nRow } );

	if not t_oneActiveInfo["join"] then
		print( "SearchTeamFrame_CopyInfoSimpleChkBtn_OnClick--t_oneActiveInfo[join] = "..t_boolDesc[t_oneActiveInfo["join"]].."--3126" );
		this:setCheckState( false );
		return;
	end

	if not t_oneActiveInfo["joinSimple"] then
		print( "SearchTeamFrame_CopyInfoSimpleChkBtn_OnClick--t_oneActiveInfo[joinSimple] = "..t_boolDesc[t_oneActiveInfo["joinSimple"]].."--3130" );
		this:setCheckState( false );
		return;
	end
	
	local canJoin,useVig = CanJoinPworldUseVig( t_oneActiveInfo["id"],PworldDifficulty.EASY );
	if not canJoin then
		ShowMidTips("活力值不足" .. useVig .. "，不能使用自动组队");
		this:setCheckState( false );
		return;
	end	
	
	local bSelect = t_oneActiveInfo["select"];
	if not t_oneActiveInfo["select"] and this:GetCheckState() then
		bSelect = true;
	end

	local nIndex = t_allAutoTeamActives:GetActiveIndexByRow( { row = nRow } );
	t_allAutoTeamActives:setActiveData( nIndex, 
										{	name	= t_oneActiveInfo["name"],			simple		= this:GetCheckState(),	
											normal	= t_oneActiveInfo["normal"],		hard		= t_oneActiveInfo["hard"],	
											professional= t_oneActiveInfo["professional"],	
											id		= t_oneActiveInfo["id"],			haveDiff	= t_oneActiveInfo["haveDiff"],	
											join	= t_oneActiveInfo["join"],			select		= bSelect,
											row		= t_oneActiveInfo["row"],			teamID		= t_oneActiveInfo["teamID"],
											joinSimple	= t_oneActiveInfo["joinSimple"],joinNormal	= t_oneActiveInfo["joinNormal"],
											joinHard	= t_oneActiveInfo["joinHard"],	joinProfessional= t_oneActiveInfo["joinProfessional"],
										} );
	UpdateSearchTeamFrame();
end

-- 点副本难度的普通chk按钮
function SearchTeamFrame_CopyInfoNormalChkBtn_OnClick()
	if not CanChangeOption() then
		this:setCheckState( not this:GetCheckState() );
		return;
	end

	local copyInfoBtn		= getglobal( "SearchTeamFram_CopyInfoBtn"..this:GetClientID() );
	local nameFont			= getglobal( "SearchTeamFram_CopyInfoBtn"..this:GetClientID().."NameFont" );
	local nRow				= copyInfoBtn:GetClientUserData( 0 );
	local t_oneActiveInfo	= t_allAutoTeamActives:GetActiveDataByRow( { row = nRow } );

	if not t_oneActiveInfo["join"] then
		this:setCheckState( false );
		return;
	end

	if not t_oneActiveInfo["joinNormal"] then
		this:setCheckState( false );
		return;
	end

	local bSelect = t_oneActiveInfo["select"];
	if not t_oneActiveInfo["select"] and this:GetCheckState() then
		bSelect = true;
	end

	local canJoin,useVig = CanJoinPworldUseVig( t_oneActiveInfo["id"],PworldDifficulty.GENERAL );
	if not canJoin then
		ShowMidTips("活力值不足" .. useVig .. "，不能使用自动组队");
		this:setCheckState( false );
		return;
	end	

	local nIndex = t_allAutoTeamActives:GetActiveIndexByRow( { row = nRow } );
	t_allAutoTeamActives:setActiveData( nIndex, 
										{	name	= t_oneActiveInfo["name"],			simple		= t_oneActiveInfo["simple"],	
											normal	= this:GetCheckState(),				hard		= t_oneActiveInfo["hard"],		
											professional= t_oneActiveInfo["professional"],	
											id		= t_oneActiveInfo["id"],			haveDiff	= t_oneActiveInfo["haveDiff"],	
											join	= t_oneActiveInfo["join"],			select		= bSelect,
											row		= t_oneActiveInfo["row"],			teamID		= t_oneActiveInfo["teamID"],
											joinSimple	= t_oneActiveInfo["joinSimple"],joinNormal	= t_oneActiveInfo["joinNormal"],
											joinHard	= t_oneActiveInfo["joinHard"],	joinProfessional= t_oneActiveInfo["joinProfessional"],
										} );
	UpdateSearchTeamFrame();
end

-- 点副本难度的困难chk按钮
function SearchTeamFrame_CopyInfoHardChkBtn_OnClick()
	if not CanChangeOption() then
		this:setCheckState( not this:GetCheckState() );
		return;
	end

	local copyInfoBtn		= getglobal( "SearchTeamFram_CopyInfoBtn"..this:GetClientID() );
	local nameFont			= getglobal( "SearchTeamFram_CopyInfoBtn"..this:GetClientID().."NameFont" );
	local nRow				= copyInfoBtn:GetClientUserData( 0 );
	local t_oneActiveInfo	= t_allAutoTeamActives:GetActiveDataByRow( { row = nRow } );

	if not t_oneActiveInfo["join"] then
		this:setCheckState( false );
		return;
	end

	if not t_oneActiveInfo["joinHard"] then
		this:setCheckState( false );
		return;
	end
	
	local bSelect = t_oneActiveInfo["select"];
	if not t_oneActiveInfo["select"] and this:GetCheckState() then
		bSelect = true;
	end

	local canJoin,useVig = CanJoinPworldUseVig( t_oneActiveInfo["id"],PworldDifficulty.HARD );
	if not canJoin then
		ShowMidTips("活力值不足" .. useVig .. "，不能使用自动组队");
		this:setCheckState( false );
		return;
	end	

	local nIndex = t_allAutoTeamActives:GetActiveIndexByRow( { row = nRow } );
	t_allAutoTeamActives:setActiveData( nIndex, 
										{	name	= t_oneActiveInfo["name"],			simple		= t_oneActiveInfo["simple"],	
											normal	= t_oneActiveInfo["normal"],		hard		= this:GetCheckState(),	
											professional= t_oneActiveInfo["professional"],	
											id		= t_oneActiveInfo["id"],			haveDiff	= t_oneActiveInfo["haveDiff"],	
											join	= t_oneActiveInfo["join"],			select		= bSelect,
											row		= t_oneActiveInfo["row"],			teamID		= t_oneActiveInfo["teamID"],
											joinSimple	= t_oneActiveInfo["joinSimple"],joinNormal	= t_oneActiveInfo["joinNormal"],
											joinHard	= t_oneActiveInfo["joinHard"],	joinProfessional= t_oneActiveInfo["joinProfessional"],
										} );
	UpdateSearchTeamFrame();
end

-- 点副本难度的专家chk按钮
function SearchTeamFrame_CopyInfoProfessionalChkBtn_OnClick()
	if not CanChangeOption() then
		this:setCheckState( not this:GetCheckState() );
		return;
	end

	local copyInfoBtn		= getglobal( "SearchTeamFram_CopyInfoBtn"..this:GetClientID() );
	local nameFont			= getglobal( copyInfoBtn:GetName().."NameFont" );
	local nRow				= copyInfoBtn:GetClientUserData( 0 );
	local t_oneActiveInfo	= t_allAutoTeamActives:GetActiveDataByRow( { row = nRow } );

	if not t_oneActiveInfo["join"] then
		this:setCheckState( false );
		return;
	end

	if not t_oneActiveInfo["joinProfessional"] then
		this:setCheckState( false );
		return;
	end
	
	local bSelect = t_oneActiveInfo["select"];
	if not t_oneActiveInfo["select"] and this:GetCheckState() then
		bSelect = true;
	end

	local canJoin,useVig = CanJoinPworldUseVig( t_oneActiveInfo["id"],PworldDifficulty.EXPERT );
	if not canJoin then
		ShowMidTips("活力值不足" .. useVig .. "，不能使用自动组队");
		this:setCheckState( false );
		return;
	end	

	local nIndex = t_allAutoTeamActives:GetActiveIndexByRow( { row = nRow } );
	t_allAutoTeamActives:setActiveData( nIndex, 
										{	name	= t_oneActiveInfo["name"],			simple		= t_oneActiveInfo["simple"],	
											normal	= t_oneActiveInfo["normal"],		hard		= t_oneActiveInfo["hard"],	
											professional= this:GetCheckState(),	
											id		= t_oneActiveInfo["id"],			haveDiff	= t_oneActiveInfo["haveDiff"],	
											join	= t_oneActiveInfo["join"],			select		= bSelect,
											row		= t_oneActiveInfo["row"],			teamID		= t_oneActiveInfo["teamID"],
											joinSimple	= t_oneActiveInfo["joinSimple"],joinNormal	= t_oneActiveInfo["joinNormal"],
											joinHard	= t_oneActiveInfo["joinHard"],	joinProfessional= t_oneActiveInfo["joinProfessional"],
										} );
	UpdateSearchTeamFrame();
end

-- 点击向上滚动的按钮
function SearchTeamFrame_ScrollUpBtn_OnClick()
	local slider = getglobal( "SearchTeamFrame_ScrollBar" );
	if not slider:IsShown() then
		return;
	end

	local nValue = slider:GetValue() - slider:GetValueStep();
	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end

-- 点击向下滚动的按钮
function SearchTeamFrame_ScrollDownBtn_OnClick()
	local slider	= getglobal( "SearchTeamFrame_ScrollBar" );
	if not slider:IsShown() then
		return;
	end

	local nMaxValue = slider:GetMaxValue();
	local nValue	= slider:GetValue() + slider:GetValueStep();

	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

function SearchTeamFrame_ScrollBar_OnValueChanged()
	local slider	 = getglobal( "SearchTeamFrame_ScrollBar" );
	local nValue	 = slider:GetValue();
	local nMaxValue	 = slider:GetMaxValue();

	if nValue <= nMaxValue then
		UpdateSearchTeamFrame();
	end
end

-- 寻求队伍前置条件选择按钮
local t_preConditionBtnMaps = 
{
	["SearchTeamFrame_AutoTeamRaceButton1"] = { career = "战士", chkBtn = "SearchTeamFrame_AutoTeamRaceChkBtn1", 
												tips = "战士系职业，分为战士及转职后的防御类的守护者，攻击输出类的征服者" },	
	["SearchTeamFrame_RaceDescBtn1"]		= { career = "战士", chkBtn = "SearchTeamFrame_AutoTeamRaceChkBtn1",
												tips = "战士系职业，分为战士及转职后的防御类的守护者，攻击输出类的征服者" },

	["SearchTeamFrame_AutoTeamRaceButton2"] = { career = "法师", chkBtn = "SearchTeamFrame_AutoTeamRaceChkBtn2",
												tips = "法师系职业，分为法师及转职后的冰系控制类的聚能者，火系暴发类的红炎法师" }, 
	["SearchTeamFrame_RaceDescBtn2"]		= { career = "法师", chkBtn = "SearchTeamFrame_AutoTeamRaceChkBtn2",
												tips = "法师系职业，分为法师及转职后的冰系控制类的聚能者，火系暴发类的红炎法师" },

	["SearchTeamFrame_AutoTeamRaceButton3"] = { career = "枪手", chkBtn = "SearchTeamFrame_AutoTeamRaceChkBtn3",
												tips = "枪手系职业，分为枪手及转职后的召唤类的猎杀者，范围攻击类的毁灭者" }, 
	["SearchTeamFrame_RaceDescBtn3"]		= { career = "枪手", chkBtn = "SearchTeamFrame_AutoTeamRaceChkBtn3",
												tips = "枪手系职业，分为枪手及转职后的召唤类的猎杀者，范围攻击类的毁灭者" },

	["SearchTeamFrame_AutoTeamRaceButton4"] = { career = "祭司", chkBtn = "SearchTeamFrame_AutoTeamRaceChkBtn4",
												tips = "祭司系职业，分为祭司及转职后的治疗类的圣佑者，法系输出类的驭雷师" }, 
	["SearchTeamFrame_RaceDescBtn4"]		= { career = "祭司", chkBtn = "SearchTeamFrame_AutoTeamRaceChkBtn4",
												tips = "祭司系职业，分为祭司及转职后的治疗类的圣佑者，法系输出类的驭雷师" },

	["SearchTeamFrame_AutoTeamRaceButton5"] = { career = "我想当队长", chkBtn = "SearchTeamFrame_AutoTeamRaceChkBtn5" }, 
	["SearchTeamFrame_RaceDescBtn5"]		= { career = "我想当队长", chkBtn = "SearchTeamFrame_AutoTeamRaceChkBtn5" },
};

local t_preConditionChkBtnMaps = 
{
	["SearchTeamFrame_AutoTeamRaceChkBtn1"] = { career = "战士", enumValue = CLT_AUTO_TEAM_LIMIT_WARRIOR },	

	["SearchTeamFrame_AutoTeamRaceChkBtn2"] = { career = "法师", enumValue = CLT_AUTO_TEAM_LIMIT_MAGICOR }, 

	["SearchTeamFrame_AutoTeamRaceChkBtn3"] = { career = "枪手", enumValue = CLT_AUTO_TEAM_LIMIT_GUNER }, 

	["SearchTeamFrame_AutoTeamRaceChkBtn4"] = { career = "祭司", enumValue = CLT_AUTO_TEAM_LIMIT_CHUNCH }, 

	["SearchTeamFrame_AutoTeamRaceChkBtn5"] = { career = "我想当队长", enumValue = CLT_AUTO_TEAM_LIMIT_LEADER }, 
};

local t_canChangeOptionFrames = { "ConfirmDifficultyFrame", "ConfirmDifficultyAndCareerFrame" };
function CanChangeOption()
	for _, name in ipairs( t_canChangeOptionFrames ) do
		local frame = getglobal( name );
		if frame:IsShown() then
			ShowMidTips( "请先进行确认操作才能进行该操作" );
			return false;
		end
	end

	return true;
end

function SearchTeamFrame_AutoTeamRaceBtn_OnClick()
	if arg1 ~= "LeftButton" then
		return;
	end

	if not CanChangeOption() then
		return;
	end

	local chkBtn = getglobal( t_preConditionBtnMaps[this:GetName()]["chkBtn"] );
	chkBtn:setCheckState( not chkBtn:GetCheckState() );
	if chkBtn:GetCheckState() then
		t_allAutoTeamActives:AddLimitFlag( t_preConditionChkBtnMaps[chkBtn:GetName()]["enumValue"] );
	else
		t_allAutoTeamActives:AddLimitFlag( -t_preConditionChkBtnMaps[chkBtn:GetName()]["enumValue"] );
	end
end

function SearchTeamFrame_AutoTeamRaceBtn_OnEnter()
	local szText = t_preConditionBtnMaps[this:GetName()]["tips"];
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function SearchTeamFrame_AutoTeamRaceBtn_OnLeave()
	GameTooltip:Hide();
end

function SearchTeamFrame_AutoTeamRaceChkBtn_OnClick()
	if not CanChangeOption() then
		this:setCheckState( not this:GetCheckState() );
		return;
	end
	
	if this:GetCheckState() then
		t_allAutoTeamActives:AddLimitFlag( t_preConditionChkBtnMaps[this:GetName()]["enumValue"] );
	else
		t_allAutoTeamActives:AddLimitFlag( -t_preConditionChkBtnMaps[this:GetName()]["enumValue"] );
	end
end
-- 战场报名
function SearchTeamFram_BattleSignUpBtn_OnClick()
	SearchTeamFrame:Hide();
	BattleStartFrame:Show();
end
-- 活动助手
function SearchTeamFram_ActivityAssistBtn_OnClick()
	local serchFrame = getglobal( this:GetParent() );
	PromotionFrame:Show();
end

function SearchTeamFram_ActivityAssistBtn_OnEnter()
	local szText = "点击进入活动助手界面";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function SearchTeamFram_ActivityAssistBtn_OnLeave()
	GameTooltip:Hide();
end

local MIN_MANY_CAREER = 2;
-- 寻求组队
function CanSearchTeam()
	if IsAnyPworldDiffNotSel() then
		WarnNotSelectDiffFrame:Show();
		return false;
	end

	if t_allAutoTeamActives:isSelectEmpty() then
		ShowMidTips( "请至少选择一个活动或任务" );
		return;
	end
	
	local mainplayer = ActorMgr:getMainPlayer();	
	if mainplayer:isInTeam() and not mainplayer:isCaptain() then
		ShowMidTips( "你已经在队伍中，必须队长才有权限进行该操作" );
		return false;
	end

	if mainplayer:isDead() then
		ShowMidTips( "死亡状态，不可做该操作" );
		return false;
	end

	return true;
end

function SearchTeamFram_SearchTeamBtn_OnClick()
	-- 是否有选中一个选项
	if not CanSearchTeam() then
		return;
	end

	local t_selInfo		= { careerNum = 0, hardNum = 0, firstHardName = "" };
	local nSelCareer	= false;
	local nHardMode		= 0;
	-- 先检查选择的职业是否太多
	local t_carrerChkBtns = { "SearchTeamFrame_AutoTeamRaceChkBtn1", "SearchTeamFrame_AutoTeamRaceChkBtn2", 
								"SearchTeamFrame_AutoTeamRaceChkBtn3", "SearchTeamFrame_AutoTeamRaceChkBtn4"
							};
	for _, name in pairs( t_carrerChkBtns ) do
		local chkBtn = getglobal( name );
		if chkBtn:GetCheckState() then
			t_selInfo.careerNum = t_selInfo.careerNum + 1;
		end
	end

	-- 再检查是否选择了难度高的副本
	for i = 1, t_allAutoTeamActives:getTotalNum() do
		local data = t_allAutoTeamActives:getActiveData( i );
		if data["haveDiff"] and data["professional"] and data["join"] and data["select"] then
			t_selInfo["hardNum"] = t_selInfo["hardNum"] + 1;
			if t_selInfo["hardNum"] == 1 then
				t_selInfo["firstHardName"] = data["name"];
			end
		end
	end

	-- 若同时既有难度又选择了太多职业
	if t_selInfo["careerNum"] >= MIN_MANY_CAREER and t_selInfo["hardNum"] > 0 then
		local t_riches = { };
		if t_selInfo["hardNum"] == 1 then
			t_riches = { ["ConfirmDifficultyAndCareerFrame_DescRich"] = { 
							value = ("    #c12e630"..t_selInfo["firstHardName"].."#n的副本你选择了".."#cfffebd".."【 专家 】#n，你确定吗？\n    你要求的必有职业数量太多，会加大匹配队伍的难度，你确定要这么多吗？"), 
							color = { r = 147, g = 137, b = 89 } }
						};
		else
			t_riches = { ["ConfirmDifficultyAndCareerFrame_DescRich"] = { 
							value = ("    所选的副本中有专家模式，你确定吗？\n    你要求的必有职业数量太多，会加大匹配队伍的难度，你确定要这么多吗？"), 
							color = { r = 147, g = 137, b = 89 } }
						};
		end

		util.SetRiches( t_riches );
		ConfirmDifficultyAndCareerFrame:Show();
		return;
	end

	-- 若选择了太多职业
	if t_selInfo["careerNum"] >= MIN_MANY_CAREER then
		local t_riches = { ["ConfirmDifficultyFrame_DescRich"] = { 
							value = "    你要求的必有职业数量太多，会加大匹配队伍的难度，你确定要这么多吗？", 
							color = { r = 147, g = 137, b = 89 } }
						};
		
		util.SetRiches( t_riches );
		ConfirmDifficultyFrame:Show();
		return;
	end

	-- 若选择了难度高的选项

	if t_selInfo["hardNum"] > 0 then
		local t_riches = { };
		
		if t_selInfo["hardNum"] == 1 then
			t_riches = { ["ConfirmDifficultyFrame_DescRich"] = { 
							value = "    #c12e630"..t_selInfo["firstHardName"].."#n的副本你选择了".."#cfffebd".."【 专家 】#n，你确定吗？", 
							color = { r = 147, g = 137, b = 89 } },
						};
		else
			t_riches = { ["ConfirmDifficultyFrame_DescRich"] = { 
							value = "#P所选的副本中有专家模式，你确定吗？", 
							color = { r = 147, g = 137, b = 89 } },
						};
		end

		util.SetRiches( t_riches );
		ConfirmDifficultyFrame:Show();
		return;
	end

	ApplyRequestAotoTeam();
end

function ApplyRequestAotoTeam()
	AutoTeamManager:cancelAllSelectPWorld();
	AutoTeamManager:cancelAllCareerAndCapRequest();
	for name, data in pairs( t_preConditionChkBtnMaps ) do
		local chkBtn = getglobal( name );
		AutoTeamManager:addCareerAndCapRequest( data["enumValue"], chkBtn:GetCheckState() );
	end
	
	-- 设置副本要求
	for i = 1, t_allAutoTeamActives:getTotalNum() do
		local data = t_allAutoTeamActives:getActiveData( i );
		if data["join"] and data["select"] then
			if data["haveDiff"] then
				AutoTeamManager:addSelectPWorld(	data["teamID"], 
													data["simple"] and CLT_AUTO_TEAM_LIMIT_EASY or 0, 
													data["normal"] and CLT_AUTO_TEAM_LIMIT_NORM or 0, 
													data["hard"] and CLT_AUTO_TEAM_LIMIT_DIFF or 0,
													data["professional"] and CLIENT_AUTO_TEAM_LIMIT_NIGHTMARE or 0 
												);
			else
				AutoTeamManager:addSelectPWorld( data["teamID"], 0, 0, 0, 0 );
			end
		end
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer:isCaptain() then
		AutoTeamManager:addCareerAndCapRequest( CLT_AUTO_TEAM_LIMIT_LEADER,true );
	end
	AutoTeamManager:requestAutoTeam();
	t_allAutoTeamActives:SetResetFlag( false );
end

local t_curSuoXiaoFrame = { name = "SearchTeamFrame" };

function GetCurSuoXiaoFrameInfo()
	return t_curSuoXiaoFrame;
end

function SetCurSuoXiaoFrameInfo( data )
	t_curSuoXiaoFrame = data;
	
	if data["name"] == "SearchTeamFrame" then
		EndPlayAutoTeamBtnEffect();
	end
end

local t_AutoTeamEnterPworldTimeDownFrameControl = { totalTime = 0, startTime = 0 };
function auteamTimeDown( nLeaveTime )
	t_AutoTeamEnterPworldTimeDownFrameControl = { totalTime = nLeaveTime, startTime = os.clock() };
	local t_fonts = {	["AutoTeamEnterPworldTimeDownFrameBlackButtonDescFont"]	= 
	{ value = "再过"..nLeaveTime.."秒会自动开始副本进程？" }, 
					};
	util.SetFonts( t_fonts );
	AutoTeamEnterPworldTimeDownFrame:Show();
end

function AutoTeamEnterPworldTimeDownFrame_ConfirmButton_OnClick()
	AutoTeamManager:cleanSafeStatus();
	AutoTeamEnterPworldTimeDownFrame:Hide();
end

function AutoTeamEnterPworldTimeDownFrame_CancelButton_OnClick()
	local nRealMapID = MapShower:getRealMapId();
	GameDynPWorld:exitDynPWorld( nRealMapID );
	AutoTeamEnterPworldTimeDownFrame:Hide();
end

local t_AutoTeamEnterPworldTimeDownFrameEvents = {
										["GE_TEAM_LEAVETEAM"] = {},	
									  };

t_AutoTeamEnterPworldTimeDownFrameEvents["GE_TEAM_LEAVETEAM"].func = 
function ()
	AutoTeamEnterPworldTimeDownFrame:Hide();
end

function AutoTeamEnterPworldTimeDownFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 0, 0 );
end

function AutoTeamEnterPworldTimeDownFrame_OnLoad()
	for event, _ in pairs( t_AutoTeamEnterPworldTimeDownFrameEvents ) do
		this:RegisterEvent( event );
	end

	t_AutoTeamEnterPworldTimeDownFrameEvents.__index = function ( tab, key )
									tab[key] = { func = function () end };
									return tab[key];
								end
	setmetatable( t_AutoTeamEnterPworldTimeDownFrameEvents, t_AutoTeamEnterPworldTimeDownFrameEvents );
end

function AutoTeamEnterPworldTimeDownFrame_OnEvent()
	t_AutoTeamEnterPworldTimeDownFrameEvents[arg1].func();
end

function AutoTeamEnterPworldTimeDownFrame_OnUpdate()
	local nCurTime		= os.clock();
	local nPassTime		= math.floor( nCurTime - t_AutoTeamEnterPworldTimeDownFrameControl["startTime"] );
	if nPassTime >= t_AutoTeamEnterPworldTimeDownFrameControl["totalTime"] then
		this:Hide();
		AutoTeamManager:cleanSafeStatus();
		return;
	end
	
	local nLeaveTime = t_AutoTeamEnterPworldTimeDownFrameControl["totalTime"] - nPassTime;
	local t_fonts = 
	{	[this:GetName().."BlackButtonDescFont"]	= { value = "再过"..nLeaveTime.."秒会自动开始副本进程？" }, 
	};
	util.SetFonts( t_fonts );
end

function AutoTeamEnterPworldTimeDownFrame_OnHide()
	t_AutoTeamEnterPworldTimeDownFrameControl = { Time = 0 };
end

function InitAutoTeamData()
	local t_chkBtns =	{	["SearchTeamFrame_SelectAllChkBtn"]			= { state = false }, 
							["SearchTeamFrame_SelectAllSimpleChkBtn"]	= { state = false }, 
							["SearchTeamFrame_SelectAllNormalChkBtn"]	= { state = false }, 
							["SearchTeamFrame_SelectAllHardChkBtn"]		= { state = false },
							["SearchTeamFrame_SelectAllProfessionalChkBtn"]	= { state = false },
						};
	util.SetChkState( t_chkBtns );
	t_allAutoTeamActives:Init();
	t_searchSelCopyInfo = { name = "" };
	t_lastRequestFinishCreating = { haveBegin = false, Time = 0 };
	t_preparingTimeCutInfo = { startTime = 0, ready = false };
	t_autoTeamTipsInfo = { playerName = "", frameName = "CreatingAutoTeamFrame", button = "" };
	t_inviteFinishingCreateControl:init();
	
	t_creatingLastFrame = { name = "" };
	t_curSuoXiaoFrame = { name = "SearchTeamFrame" };

	--SeteActiveAutoTeamFramePassTime( { ready = false, startTime = 0, autoWarn = false, autoTimeDown = false, autoTimeDown = false } );
	t_lastRequestFinishCreating = { haveBegin = false, Time = 0 };

	local t_WarnAutoTeamFailFrameControl = GetWarnAutoTeamFailFrameControl();
	t_WarnAutoTeamFailFrameControl:init();

	local t_ActiveAutoTeamFrameControl = GetActiveAutoTeamFrameControl();
	t_ActiveAutoTeamFrameControl:SetTimeEndFlag( { flag = false } );
end

local t_shortcutAutoTeamFrames = { "SuccessCreatTaskAutoTeamFrame", "CreatingAutoTeamFrame", "SearchTeamFrame", 
									"SuccessCreatActiveAutoTeamFrame" };

function NeedPlayeEffect( data )
	local t_needPlayEffectAutoTeamFrames = { "SuccessCreatTaskAutoTeamFrame", "CreatingAutoTeamFrame",
											"SuccessCreatActiveAutoTeamFrame" };

	for _, needPlayEffectFrame in ipairs( t_needPlayEffectAutoTeamFrames ) do
		if needPlayEffectFrame == data["name"] then
			return true;
		end
	end

	return false;
end

function CallAutoTeamFrame()
	local frame = getglobal( t_curSuoXiaoFrame["name"] );
	if frame:IsShown() then
		
		frame:Hide();
		if NeedPlayeEffect( { name = t_curSuoXiaoFrame["name"] } ) then
			BeginPlayAutoTeamBtnEffect();
		end
	else
		frame:Show();
		if NeedPlayeEffect( { name = t_curSuoXiaoFrame["name"] } ) then
			EndPlayAutoTeamBtnEffect();
		end
	end
end

function PromotionQuickCreateAutoTeam( nAutoTeamID )
	if not CanChangeOption() then
		return
	end
	AutoTeamManager:cancelAllSelectPWorld();
	AutoTeamManager:cancelAllCareerAndCapRequest();

	CallAutoTeamFrame();
	t_allAutoTeamActives:ClearAllChoice();
	local autoTeamData	= GamePromotion:getAutoTeamData( nAutoTeamID );
	local t_oneActiveInfo	= t_allAutoTeamActives:GetActiveDataByRow( { row = nAutoTeamID } );
	if t_oneActiveInfo["haveDiff"] then
		-- 选择专家
		for i=1,MAX_SEARCH_NUM do
			local checkbtn = getglobal( "SearchTeamFrame_CopyInfoProfessionalChkBtn" .. i );
			local btn = getglobal( "SearchTeamFram_CopyInfoBtn" .. i )
			if btn:GetClientUserData( 0 ) == nAutoTeamID then				
				if t_oneActiveInfo["join"] and t_oneActiveInfo["joinProfessional"] then
					checkbtn:setCheckState( true )
					local nIndex = t_allAutoTeamActives:GetActiveIndexByRow( { row = nAutoTeamID } );
					t_allAutoTeamActives:setActiveData( nIndex, 
														{	name	= t_oneActiveInfo["name"],			simple		= t_oneActiveInfo["simple"],	
															normal	= t_oneActiveInfo["normal"],		hard		= t_oneActiveInfo["hard"],	
															professional= checkbtn:GetCheckState(),
															id		= t_oneActiveInfo["id"],			haveDiff	= t_oneActiveInfo["haveDiff"],	
															join	= t_oneActiveInfo["join"],			select		= true,
															row		= t_oneActiveInfo["row"],			teamID		= t_oneActiveInfo["teamID"],
															joinSimple	= t_oneActiveInfo["joinSimple"],joinNormal	= t_oneActiveInfo["joinNormal"],
															joinHard	= t_oneActiveInfo["joinHard"],	joinProfessional= t_oneActiveInfo["joinProfessional"],
														} );
				end
				UpdateSearchTeamFrame();			
				break;
			end		
		end
	else
		-- 选择名字
		for i=1,MAX_SEARCH_NUM do			
			local checkbtn = getglobal( "SearchTeamFrame_CopyInfoAllDifficultyChkBtn" .. i )
			local btn = getglobal( "SearchTeamFram_CopyInfoBtn" .. i )
			if btn:GetClientUserData( 0 ) == nAutoTeamID then
				if t_oneActiveInfo["join"] then
					checkbtn:setCheckState( true )
					local nIndex = t_allAutoTeamActives:GetActiveIndexByRow( { row = nAutoTeamID } );
					t_allAutoTeamActives:setActiveData( nIndex, 
														{	name	= t_oneActiveInfo["name"],			simple	= false,					
															normal	= false,							hard	= false,
															professional= false,	
															id		= t_oneActiveInfo["id"],		
															haveDiff= t_oneActiveInfo["haveDiff"],		join = t_oneActiveInfo["join"],	
															select	= true,
															row		= t_oneActiveInfo["row"],			teamID	= t_oneActiveInfo["teamID"],
															joinSimple	= t_oneActiveInfo["joinSimple"],joinNormal	= t_oneActiveInfo["joinNormal"],
															joinHard	= t_oneActiveInfo["joinHard"],	joinProfessional= t_oneActiveInfo["joinProfessional"],
														} );
				end
				UpdateSearchTeamFrame();			
				break;
			end
		end
	end
end

-----------------------------自动组队聊天----------------------------------

function CreatingAutoTeamFrame_SendMessageBtn_OnClick()
	local mainplayer	= ActorMgr:getMainPlayer();
	local szMyName		= mainplayer:getName();
	local szParentName	= this:GetParent();
	local inputEdit		= getglobal( szParentName.."InputEdit" );
	local szChatContent = inputEdit:GetText();

	-- 不允许发送空消息
	if "" == szChatContent then return end

	-- 将消息发给聊天对象
	local t_ClientSelfChatContentParser = GetClientSelfChatContentParser();
	sz2Server = t_ClientSelfChatContentParser:parseLinkTextForServe( { ["text"] = szChatContent } );	
	ChatManager:requestChat( CL_CHAT_AUTOTEAM, "", sz2Server );

	-- 清空输入框中的内容
	inputEdit:Clear();
end

function CreatingAutoTeamFrame_FaceBtn_OnClick()
	local chatFrame		= getglobal( this:GetParent() );
	local chatFaceFrame	= getglobal( "CreatingAutoTeamFaceFrame" );
	
	if chatFaceFrame:IsShown() then
		local szRelFrame = chatFaceFrame:GetClientString();
		if chatFrame:GetName() == szRelFrame then
			chatFaceFrame:Hide();
		else
			chatFaceFrame:SetPoint( "BottomRight", this:GetName(), "BottomLeft", 0, 0 );
			chatFaceFrame:SetClientString( this:GetParent() );
			chatFaceFrame:Show();
		end
	else
		chatFaceFrame:SetPoint( "BottomRight", this:GetName(), "BottomLeft", 0, 0 );
		chatFaceFrame:SetClientString( this:GetParent() );
		chatFaceFrame:Show();
	end
end

function CreatingAutoTeamFrame_FaceBtn_OnEnter()
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "选择表情", frame = this:GetParent(),
						button = this:GetName() } );
end

function CreatingAutoTeamFaceFrame_OnShow()
	local rich = getglobal( "CreatingAutoTeamFaceFrameShowFaceRich" );
	if rich:GetTextLines() > 0 then
		return;		
	end

	util.CompleteShowAllTextOnRich( {	["rich"] = rich:GetName(), ["text"] = FACE_TEXT, 
										["color"] = { r = 255, g = 255, b = 255 }, } );
end

function CreatingAutoTeamFaceFrame_OnLoad()
	local rich = getglobal( "CreatingAutoTeamFaceFrameShowFaceRich" );
	rich:SetFaceTexture( [[uires\TuPianLei\LiaoTian\BiaoQingTuBiao\popoface.dds]] );
	rich:SetFaceTexUV( t_biaoQingHightUV["x"], t_biaoQingHightUV["y"], t_biaoQingHightUV["width"], t_biaoQingHightUV["height"] );
	rich:setTwoFaceInterval( 3.5, 1 );
end

function CreatingAutoTeamFaceFrame_OnUpdate()
	local frame = getglobal( this:GetClientString() );
	if not frame:IsShown() then
		this:Hide();
	end
end

function CreatingAutoTeamFaceFrameShowFaceRich_OnClick()
	-- 当点击笑脸面板中的任意一个笑脸后,关闭笑脸面板
	local editFrame = getglobal( CreatingAutoTeamFaceFrame:GetClientString() );
	-- 获取所点击的内容
	local inputEdit = getglobal( editFrame:GetName() .. "InputEdit");
	if arg1 < 1 then 
		return; 
	end

	--表情代码
	local nCode = arg1;
	local szCode;
	if nCode >= 100 then
		szCode = "#"..tostring(nCode);
	elseif nCode >= 10 then
		szCode = "#0"..tostring(nCode);
	elseif nCode > 0 then
		szCode = "#00"..tostring(nCode);
	end

	inputEdit:SetText( inputEdit:GetText()..szCode );
	--inputEdit:SetText( szCode );
	SetFocusFrame( inputEdit:GetName() );
end

function CreatingAutoTeamFaceFrameShowFaceRich_OnEnter()
	if arg2 ~= "RTOT_FACE_ONENTER" then
		return;
	end

	if arg1 < 1 then 
		return;
	end

	--表情代码
	local nCode = arg1;
	local szCode;
	if nCode >= 100 then
		szCode = "#"..tostring (nCode );
	elseif nCode >= 10 then
		szCode = "#0"..tostring( nCode );
	elseif nCode > 0 then
		szCode = "#00"..tostring(nCode);
	end
	
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = GetFaceDesc( szCode ), frame = this:GetParent(),
						button = "cursor" } );
end

function CreatingAutoTeamFrame_ScrollUpBtn_OnClick()
	local slider = getglobal( "CreatingAutoTeamFrame_ScrollBar" );
	if not slider:IsShown() then
		return;
	end
	local nValue = slider:GetValue();
	if nValue == 0 then
		return;
	end
	slider:SetValue( 0 );
end

function CreatingAutoTeamFrame_ScrollDownBtn_OnClick()
	local slider	= getglobal( "CreatingAutoTeamFrame_ScrollBar" );
	if not slider:IsShown() then
		return;
	end
	local nValue	= slider:GetValue();
	local nMaxValue = slider:GetMaxValue();
	if nValue == nMaxValue then
		return;
	end
	slider:SetValue( nMaxValue );
end

function CreatingAutoTeamFrame_ScrollBar_OnValueChanged()
	local slider	 = getglobal( "CreatingAutoTeamFrame_ScrollBar" );
	local nValue	 = slider:GetValue();
	local nMaxValue	 = slider:GetMaxValue();
	local rich		 = getglobal("CreatingAutoTeamFrameChatRich");
	if nValue <= nMaxValue then
		util.UpdateRich( rich:GetName(), nValue );
	end	
end

-- 精力值是否足够可以进入副本
function CanJoinPworldUseVig( diffPworldId, difficulty )
	local pworldId = PworldDifficulty:getDifficultyPworldId( diffPworldId,difficulty );
	if pworldId > 0 then
		local pworldDef = getPworldDef( pworldId )
		if pworldDef ~= nil and pworldDef.EnterUseVig > 0 then
			local mainplayer = ActorMgr:getMainPlayer()
			if mainplayer:getVigor() < pworldDef.EnterUseVig then			
				return false,pworldDef.EnterUseVig;
			end
		end
	end
	return true;
end