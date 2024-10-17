-- ȡ���Զ����
-- ���ڴ����е��ϴβ������
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
		-- TODO: ����ÿ�����Ĳ����������߼���������
		--SeteActiveAutoTeamFramePassTime( { startTime = 0, ready = false, autoWarn = true, autoTimeDown = false } );
		local t_hideIcons = { "WarnAutoTeamFailFrame", "WarnTimeCountDownFrame" };
		util.HideIcons( t_hideIcons );
	end

	if SuccessCreatActiveAutoTeamFrame:IsShown() then
		-- TODO: ����ÿ�����Ĳ����������߼���������
		WarnAutoTeamFailFrame:Hide();
	end

	for _, name in ipairs( t_closeFrames ) do
		local frame = getglobal( name );
		if frame:IsShown() then
			frame:Hide();
		end
	end
	
	print( "nOpCode = "..nOpCode.."��t_creatingLastFrame[name] = "..t_creatingLastFrame["name"] );
	if nOpCode == RETURN_TO_LAST_FRAME and t_creatingLastFrame["name"] ~= "" then
		local frame = getglobal( t_creatingLastFrame["name"] );
		frame:Show();	
		SetCurSuoXiaoFrameInfo( { name = t_creatingLastFrame["name"] } );
	end
	
	-- TODO: ������ص������kent����
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
									{ value = t_inviteFinishingCreateControl["name"].."ϣ���ȴ������飬�Ƿ�ͬ�⣿" },

						["ApplyCreateTeamFirstFrameBlackButtonTimeDownFont"] = 
									{ value = math.ceil( MAX_NO_OP_CREATING_TIME ).."s����ӦĬ��ͬ�����ɴ�������" },
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
												"s����ӦĬ��ͬ�����ɴ�������" },
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

-- �жϸ÷�������ɴ��������Ƿ���Ч
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

	ShowMidTips( szRefuseName.."�ܾ�����ɴ���������" );
end

-- ���ȴ��������ͬ�ⰴť
function ApplyCreateTeamFirstFrame_AcceptButton_OnClick()
	local t_inviteFinishingCreateControl = GetInviteFinishingCreateControl();
	if t_inviteFinishingCreateControl:isRequestValid() then
		AutoTeamManager:agreeCreate();
	else
		ShowMidTips( "�ò����������Ѿ����ڣ��ò���ʧЧ" );
	end
	
	ApplyCreateTeamFirstFrame:Hide();
end

-- ���ȴ�������ľܾ���ť
function ApplyCreateTeamFirstFrame_RufuseOneButton_OnClick()
	local t_inviteFinishingCreateControl = GetInviteFinishingCreateControl();
	if t_inviteFinishingCreateControl:isRequestValid() then
		AutoTeamManager:disagreeCreate();
	else
		ShowMidTips( "�ò����������Ѿ����ڣ��ò���ʧЧ" );
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

--  ��ʾȡ���Զ���ӵĿ���ԭ�� ������ʾ
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
	rich:SetText( "�ڵȴ������齨�ڼ䣬����¸��������ߣ������������飬ϵͳ���Զ�ȡ���Զ��������", 255, 229, 189 );
	this:SetPoint( "center", "UIClient", "center", 0, 125 );
	t_WarnAutoTeamFailFrameControl["haveShowTimes"] = t_WarnAutoTeamFailFrameControl["haveShowTimes"] + 1;
end

-- ������ʾ����ʱ��ʼ�����ʾ
function WarnTimeCountDownFrame_OnShow()
	local rich = getglobal( this:GetName().."_DescRich" );
	rich:SetText( "�����������ˣ��㻹δ׼���ã���Ҷ��ڵ��㣬��׼���������������ʱ����ʱδͬ�⣬��Ĭ�����Զ������Զ��������", 255, 229, 189 );
	this:SetPoint( "center", "UIClient", "center", 0, 0 );
end

function ConfirmQuitAutoTeamFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 0, 0 );
end

-- ���ȷ���˳��Զ����
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

-- ���鴴�������
local MAX_BEGIN_TIME_DOWN = 30; -- ����ʱ���30��
-- TODO: ���ߵ�ʱ��Ҫ��ס�������
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
									-- սʿ
									[CLIENT_CAREER_WARRIOR]	= { path = "uires\\TuPianLei\\ZiDongZuDuiAnNiuTuBiao\\zhanshi.tga", 
																UV = { x = 0, y = 0, width = 64, height = 64 }, },
									
									[CLIENT_CAREER_WARRIOR_SINGLEHAND]	= { path = "uires\\TuPianLei\\ZiDongZuDuiAnNiuTuBiao\\zhanshi.tga", 
																UV = { x = 0, y = 0, width = 64, height = 64 }, },
									
									[CLIENT_CAREER_WARRIOR_DOUBLEHAND]	= { path = "uires\\TuPianLei\\ZiDongZuDuiAnNiuTuBiao\\zhanshi.tga", 
																UV = { x = 0, y = 0, width = 64, height = 64 }, },
									
									-- ��ʦ								
									[CLIENT_CAREER_MAGICOR]	= { path = "uires\\TuPianLei\\ZiDongZuDuiAnNiuTuBiao\\fashi.tga",
																UV = { x = 0, y = 0, width = 64, height = 64 },
																},
									
									[CLIENT_CAREER_MAGICOR_SINGLEHAND]	= { path = "uires\\TuPianLei\\ZiDongZuDuiAnNiuTuBiao\\fashi.tga",
																UV = { x = 0, y = 0, width = 64, height = 64 },
																},
									
									[CLIENT_CAREER_MAGICOR_DOUBLEHAND]	= { path = "uires\\TuPianLei\\ZiDongZuDuiAnNiuTuBiao\\fashi.tga",
																UV = { x = 0, y = 0, width = 64, height = 64 },
																},
									
									-- ǹ�� 
									[CLIENT_CAREER_GUNER]		= { path = "uires\\TuPianLei\\ZiDongZuDuiAnNiuTuBiao\\qiangshou.tga",
																UV = { x = 0, y = 0, width = 64, height = 64 },
																},

									[CLIENT_CAREER_GUNER_SINGLEHAND]		= { path = "uires\\TuPianLei\\ZiDongZuDuiAnNiuTuBiao\\qiangshou.tga",
																UV = { x = 0, y = 0, width = 64, height = 64 },
																},

									[CLIENT_CAREER_GUNER_DOUBLEHAND]		= { path = "uires\\TuPianLei\\ZiDongZuDuiAnNiuTuBiao\\qiangshou.tga",
																UV = { x = 0, y = 0, width = 64, height = 64 },
																},
									
									-- ��˾
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
							[btn:GetName() .. "RaceLvFont"]		= { value = memberInfo.Level .. "�� " .. GetCareerName( memberInfo.Career ) },
						};
		util.SetFonts( t_fonts )
		local t_Icons = { btn:GetName() .. "hairPhoto", btn:GetName() .. "Photo", btn:GetName() .. "HairShadowTex", btn:GetName() .. "BodyTex", btn:GetName() .. "BoxTexture" }
		-- ��һλ��ʾ�ӳ����
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

	-- ���ù�ͬ��Ŀ
	local autoTeamData	= GamePromotion:getAutoTeamData( nAutoTeamID );
	local szText = "";
	if autoTeamData.proType == CLT_AUTO_TEAM_ACTIVE then
		szText = "��ͬ��Ŀ��"..PworldDifficulty:getDiffPworldName(autoTeamData.relateID).."����";
	elseif autoTeamData.proType == CLT_AUTO_TEAM_TASK then
		szText = "��ͬ��Ŀ��"..Quest:getQuestName(autoTeamData.relateID).."����";
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
	-- ���ǵ�һ�ο�ʼ�Զ���ӣ����ҽ����Զ���ӵȴ����У���Ҫ�Զ������ȴ��������
	-- ���Ƕӳ��·�����Զ���ӣ���ô�ӳ�ҲҪ�����Զ���ӵȴ����е����
	if not creatingFrame:IsShown() and ( AutoTeamManager:getPreMemberNum() == 0 or 
			( t_allAutoTeamActives:GetContinueAutoTeam() and t_curSuoXiaoFrame["name"] == "SearchTeamFrame" ) ) then
		-- ��ʾ�˳��Զ���ӵĿ���ԭ��
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

	-- ��������������ʼ����ʱ
	-- TODO: ��ʱ����	MAX_TEAM_MEMBER
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
	-- �����ڵ���ʱ����ֹͣ����ʱ
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
	rich:AddText(  "#A018" .. szSpeakerName .."˵:" .. szChatContent, 255, 255, 190 );
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
	rich:AddText(  "#A018��˵:" .. szChatContent, 255, 255, 190 );
	rich:ScrollEnd();
	local slider	= getglobal("CreatingAutoTeamFrame_ScrollBar");
	SetScrollBar( slider:GetName(), rich:GetAccurateViewLines(), rich:GetTextLines() );	
	slider:SetValue( slider:GetMaxValue() );
	util.UpdateRich( rich:GetName(), slider:GetValue() );
end

-- �����ڸ����еĶӳ�����ģ�1.���ӳ��ڸ��������赯��������洫�ͽ��������棬��Ҫ��������С���������˵��½��棬2.���ӳ��ڸ�������
-- �򵯳����ͽ���
-- �����ϴ��Զ�����н������ˣ������κη�Ӧ���ϴ��Զ�����н���������С�Ľ���Ȳ�Ϊ"CreatingAutoTeamFrame"����
-- �����¼��������ˣ��򵯳��������ͽ���
-- �Զ�Ѱ��ɹ����л���������

-- �ᵯ���Զ���ӵȴ����������Щ�ˣ���ֻ�з����Զ���ӵĶӳ���ɢ����
function UpdateSuccessActiveAutoTeamFrames()
	local mainplayer		= ActorMgr:getMainPlayer();
	local nCurMapID			= MapShower:getRealMapId();		
	local t_curSuoXiaoFrame	= GetCurSuoXiaoFrameInfo(); 
	-- ���Ƕӳ�
	local firstPreMember = AutoTeamManager:getPreMemberByIndex( 0 )
	if mainplayer:isCaptain() or CompareRoleName( mainplayer:getName(), firstPreMember.RoleName ) then
		-- �ж��Ƿ������ľ����ĸ���
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
				ShowMidTips( "�Զ���ӳɹ�" );
			end

			SetCurSuoXiaoFrameInfo( { name = "SearchTeamFrame" } );
		end
	-- ���Ƕ�Ա
	else
		if t_curSuoXiaoFrame["name"] == "CreatingAutoTeamFrame" or t_curSuoXiaoFrame["name"] == "SearchTeamFrame" then
			-- �����ϴ��Զ�����н�������
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
	
	-- ����ϴβ����򿪵����
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
	-- ����Ŀǰ������һ�˵����
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

		-- �ܾ�
		ShowMidTips( "��ѡ������л���Ѿ�������ȡ���Զ����" );
		AutoTeamManager:disagreeAutoTeam();

		local t_hideIcons = { "WarnTimeCountDownFrame", "WarnAutoTeamFailFrame", "CreatingAutoTeamFrame", "ConfirmQuitAutoTeamFrame",  };
		util.HideIcons( t_hideIcons );
		SetCurSuoXiaoFrameInfo( { name = "SearchTeamFrame" } );
	end

	-- �����ж��˵����
	local nAutoTeamID = AutoTeamManager:getAutoTeamID();
	if nAutoTeamID <= 0 then
		return;
	end

	-- ���ù�ͬ��Ŀ
	local autoTeamData = GamePromotion:getAutoTeamData( nAutoTeamID );
	if autoTeamData.proType == CLT_AUTO_TEAM_ACTIVE then
		local startInfo = GamePromotion:getCurrPromitionStartInfo( autoTeamData.nRow );
		if startInfo.startFlag == PROMOTION_RUNING then
			return;
		end
		
		-- �ܾ�
		ShowMidTips( "����Ҫ�μӻ�Ѿ�������ȡ���Զ����" );
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
	-- ����ʱ�����ý�����
	local fPassedTime	= os.clock() - t_preparingTimeCutInfo["startTime"];
	if fPassedTime > MAX_BEGIN_TIME_DOWN then
		processTex:SetSize( 444, 12 );
		t_preparingTimeCutInfo = { startTime = 0, ready = false };
		-- ���Ѿ��ڶ����У���������֤�Ƿ���ͬ��״̬��
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

-- ���������е��Ҽ��˵�ѡ��
local t_creatingAutoTeamRightOp = { "��������", "�۲�Է�", "ȡ��" };
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

-- ����Ƶ����鴴���е�����ְҵͼ��
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
	-- TODO: ����������������Ҽ�
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
t_sex = {	[CLIENT_GENDER_MALE]	= { desc = "��", maxHair = 13,  maxFace = 8,  maxHead = 7, maxClothes = 2, }, 
			[CLIENT_GENDER_FEMALE]	= { desc = "Ů", maxHair = 13,  maxFace = 8,  maxHead = 7, maxClothes = 2, },
		};
local t_prepareState = { [CLT_PRE_MEMBER_NOTDECIDE] = { desc = "δͬ��"}, [CLT_PRE_MEMBER_AGREE] = { desc = "ͬ��"}, };

local t_allLimit = {	[CLT_AUTO_TEAM_LIMIT_WARRIOR]	= { desc = "սʿ", chkBtn = "SearchTeamFrame_AutoTeamRaceChkBtn1" }, 
						[CLT_AUTO_TEAM_LIMIT_MAGICOR]	= { desc = "��ʦ", chkBtn = "SearchTeamFrame_AutoTeamRaceChkBtn2" },
						[CLT_AUTO_TEAM_LIMIT_GUNER]		= { desc = "ǹ��", chkBtn = "SearchTeamFrame_AutoTeamRaceChkBtn3" }, 
						[CLT_AUTO_TEAM_LIMIT_CHUNCH]	= { desc = "��˾", chkBtn = "SearchTeamFrame_AutoTeamRaceChkBtn4" }, 
						[CLT_AUTO_TEAM_LIMIT_LEADER]	= { desc = "�ӳ�", chkBtn = "SearchTeamFrame_AutoTeamRaceChkBtn5" }, 
					};
-- ְҵtips���
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

	local t_fonts = {	["AutoTeamMemberTipsFrameNameFont"]				= { value = "���֣�"..preMemberInfo.RoleName },
						["AutoTeamMemberTipsFrameLevelFont"]			= { value = "�ȼ���"..preMemberInfo.Level.."��" },
						["AutoTeamMemberTipsFrameRaceFont"]				= { value = "ְҵ��"..t_raceList[preMemberInfo.Career] },
						--["AutoTeamMemberTipsFrameAreaFont"]				= { value = "��Ϸ����" },
						["AutoTeamMemberTipsFrameSexFont"]				= { value = "�Ա�"..t_sex[preMemberInfo.Gender]["desc"] },
						--["AutoTeamMemberTipsFrameReadyStateFont"]		= { value = "״̬��"..t_prepareState[preMemberInfo.AgreeStat]["desc"] },
						["AutoTeamMemberTipsFramePreRequestFont"]		= { value = "���Ҫ��"..szRequest },
						["AutoTeamMemberTipsFrameContentFont"]			= { value = "�����Ŀ��"..szCommon },
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

	-- �����Ƿ���ʾͬ��ȡ����ť
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
	
	-- �����Ƿ���ʾ��ɴ�����ť
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

-- �����С��ť
function CreatingAutoTeamFrame_MinizeBtn_OnClick()
	local frame = getglobal( "CreatingAutoTeamFrame" );
	frame:Hide();
	BeginPlayAutoTeamBtnEffect();
end

function CreatingAutoTeamFrame_MinizeBtn_OnEnter()
	local szText = "���ش��ڣ�����˰����������齨���鴰�ڣ���Ҫ��ʱ����С��ͼ�ϵ��Զ���Ӱ�ť���ɴ�";
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

-- �����ɴ�����ť��Ӧ
function CreatingAutoTeamFrame_FinishCreatingBtn_OnClick()
	-- ���ڶ����У����Ƕӳ��Ͳ��е�
	local mainplayer = ActorMgr:getMainPlayer();
	local fCurTime = os.clock();
	local t_lastFinishCreatingInfo = GetLastRequestFinishCreating();
	if t_lastFinishCreatingInfo["haveBegin"] and fCurTime - t_lastFinishCreatingInfo["Time"] < MIN_FINSH_CREATIN_CD then
		ShowMidTips( "�ڷ�����ɴ������"..MIN_FINSH_CREATIN_CD.."s�ڲ����ٴη�����ɴ���������" );
		return;
	end
	--[[
	if AutoTeamManager:getMainPlayerAgreeState() ~= CLT_PRE_MEMBER_AGREE then
		ShowMidTips( "����ͬ���Զ���ӣ����ܷ�����ɴ����Ĳ���" );
		return;
	end
	--]]

	AutoTeamManager:requestFinishCreate();
	SetLastRequestFinishCreating( { Time = os.clock(), haveBegin = true } );
end

-- �����ͬ�ⰴť��Ӧ
function CreatingAutoTeamFrame_AcceptBtn_OnClick()
	AutoTeamManager:agreeAutoTeam();

	local t_hideIcons = { "WarnTimeCountDownFrame", };
	util.HideIcons( t_hideIcons );
end

function CreatingAutoTeamFrame_AcceptBtn_OnEnter()
	local szText = "��Ա��ʱ���г�Ա��ͬ��òŽ�����ӣ���ʱûͬ����Ĭ��ͬ�⣬�򹴵ı�ʾ��ͬ�⣬���ʺű�ʾδͬ�⣬�ڵȴ����ʱ���ڣ��¸��������ߣ�ϵͳ�����Զ�ȡ���������";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function CreatingAutoTeamFrame_AcceptBtn_OnLeave()
	GameTooltip:Hide();
end

-- �����ȡ����ť��Ӧ
function CreatingAutoTeamFrame_RefuseBtn_OnClick()
	--AutoTeamManager:disagreeAutoTeam();
	ConfirmQuitAutoTeamFrame:Show();
end

-- ��ʾ�����Զ���ӳɹ�
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
							[btn:GetName().."LevelFont"]	= { value = teamMember.Level.."��" },
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
		value = "      ע���������г�Ա�����ڶӳ���5�׷�Χ�ڲſɲμӻ����ǰ��#L"..npcInfo.m_szName.."#n����", 
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

-- ǰ�����ϰ�ť
function SuccessCreatTaskAutoTeamFrame_JoinBtn_OnClick()
	local autoTeamData	= GamePromotion:getAutoTeamData( AutoTeamManager:getAutoTeamID() );
	local MaxMapControl = MapShower:GetMaxMapContal();
	MaxMapControl:linkMoveto( autoTeamData.mapID, autoTeamData.npcID );
end

-- �ɹ�����������ʾ
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

-- ��ʾ��Զ���ӳɹ�
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
	-- TODO:Ҫ���õ�ǰ��С�����
	local mainplayer = ActorMgr:getMainPlayer();
	-- ֻ���ڴ��ͽ������������Ƕӳ���ʱ�򣬲�������С���Ϊ�������˽���
	-- ������ͨ���ѣ����Ǳ��ִ��ͽ���
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
							[btn:GetName().."LevelFont"]	= { value = teamMember.Level.."��" },
						};
		util.SetFonts( t_fonts );

		if CompareRoleName( teamMember.MemberName, mainplayer:getName() ) then
			nSelfPosition = teamMember.Position;
		end

		nBtnIndex = nBtnIndex + 1;
	end

	local t_fonts = {	["SuccessCreatActiveAutoTeamFrameSelfPositionBtnFont"]		= { value = "����С��"..t_teamPosition[nSelfPosition]["desc"] },
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
							[btn:GetName().."LevelFont"]	= { value = teamMember.Level.."��" },
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
							[btn:GetName().."LevelFont"]	= { value = teamMember.Level.."��" },
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

	local t_fonts = {	["SuccessCreatActiveAutoTeamFrameSelfPositionBtnFont"]		= { value = "����С��"..t_teamPosition[nSelfPosition]["desc"] },
					};
	util.SetFonts( t_fonts );
end

MAX_AUTO_TEAM_ACTIVELEAVE_TIME = 120;
-- haveWarnTimeEndΪtrue��ʾ�Ѿ��Զ��رչ�һ��
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
						["SuccessCreatActiveAutoTeamFrameDescBtnDescFont"]		= { value = "����㽫�����͵�"..PworldDifficulty:getDiffPworldName(autoTeamData.relateID) },
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
	-- TODO; ֻҪ�޸ĸ���ʱ���ʱ��
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
	-- ÿ��ٰ�Ļ
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

	-- һ�ٰܾ켸�εĻ
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

local t_copyDiffultInfo = { [0] = { desc = "��" }, [1] = { desc = "��ͨ" }, [2] = { desc = "����" }, [3] = { desc = "ר��" }, };
function UpdateActiveOpenTime()
	local nAutoTeamID	= AutoTeamManager:getAutoTeamID();
	local autoTeamData	= GamePromotion:getAutoTeamData( nAutoTeamID );
	assert( autoTeamData.proType == CLT_AUTO_TEAM_ACTIVE );
	-- �Ѷ�Ϊ�����ѣ�ʱ�䣺��ʼʱ��~����ʱ��
	local szText = "�Ѷ�Ϊ��"..t_copyDiffultInfo[AutoTeamManager:getPworldDiffulty()]["desc"].."��ʱ�䣺"..
					( GetPromotionOpenTime( autoTeamData.nRow ) );

	local t_fonts =	{	["SuccessCreatActiveAutoTeamFrameDescBtnCopyInfoFont"]	= { value = szText },
						["SuccessCreatActiveAutoTeamFrameDescBtnDescFont"]	= { value = "�㽫���͵�"..PworldDifficulty:getDiffPworldName(autoTeamData.relateID).."����" },
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

	-- TODO; ���ĳ�ʱ�䵽�˾͹رո����
	local nAutoTeamID	= AutoTeamManager:getAutoTeamID();
	local autoTeamData	= GamePromotion:getAutoTeamData( nAutoTeamID );
	if autoTeamData.proType ~= CLT_AUTO_TEAM_ACTIVE then
		return;
	end

	local startInfo = GamePromotion:getCurrPromitionStartInfo( autoTeamData.nRow );
	if startInfo.startFlag ~= PROMOTION_RUNING then
		ShowMidTips( "�����μӵĻ�Ѿ�����" );
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

-- �˳�����
function SuccessCreatTaskAutoTeamFrame_ExitAutoTeamBtn_OnClick()
	local frame = getglobal( this:GetParent() );
	--frame:Hide();
	MessageBox("��ʾ","ȷ��Ҫ�뿪������");
	MessageBoxFrame:SetClientString("�뿪�Զ����");
	--AutoTeamManager:cancelTranToPWorld();
end

-- ���ϴ���
function SuccessCreatTaskAutoTeamFrame_NowTransBtn_OnClick()
	if TeamFollowHintFrame:IsShown() then
		ShowMidTips( "��Ӹ���״̬��,��ֹ�˲���" );
		return;
	end

	if not AutoTeamManager:canTranToPWorld() then
		return;
	end

	local frame = getglobal( this:GetParent() );
	frame:Hide();
	
	AutoTeamManager:tranToPWorld();
end

-- һ����ȥ
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

-- Ѱ�����
local MAX_SEARCH_NUM = 8;

local t_searchSelCopyInfo = { name = "" };

--select ��ʾ�û�Ƿ�ѡ��	join ��ʾ�Ƿ��ܲμӸû		haveDiff ��ʾ�û�Ƿ����Ѷ�ѡ��
-- { id = , simple = bool, normal = bool, hard = bool, professional = bool?, haveDiff = ?, select = ?, join = ?, name = ?, 
--		row = ?, limitFlag = 0, resetDefault = true },
-- �����Զ�������õ��Ļ����
local t_allAutoTeamActives = { resetDefault = true, limitFlag = 0, continueAutoTeam = false };

function GetAllAutoTeamActivesControl()
	return t_allAutoTeamActives;
end

-- ���� �����ǿ��Խ�������ڲ��ɽ�������档
-- Ȼ������ͨ���ڻ������
-- ����ǰ��ս���ȼ�����
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

-- ��ʼ��
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

-- �Ƿ���Ҫ�����������ѡ��
t_allAutoTeamActives["NeedSetDefault"] =
function ( self )
	return self["resetDefault"];
end

-- �������ѡ��
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

-- ����ΪĬ������
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
			-- �Ƿ��ܹ��μӶ��˸�ɸѡ���ݣ����ݵ�ǰ�������������ĸ�������
			
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

-- ͬ�����»�߼�����
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

		-- �Ƿ��ܹ��μӶ��˸�ɸѡ���ݣ����ݵ�ǰ�������������ĸ�������
		if activeInfo.m_nFlag == CLT_AUTO_TEAM_ACTIVE then
			nQueryDifficulty		= self:getPworldDiffculty();
			local nDefPworld		= PworldDifficulty:getDifficultyPworldId( activeInfo.m_nRelateId, nQueryDifficulty )
			isInTheCorrectPworld	= ( nCurInPworldID == nDefPworld );
		end
		
		if data["row"] == 8 then
			print( "SynchronizePromotionInPworld-- isInTheCorrectPworld = "..t_boolDesc[isInTheCorrectPworld].."�� name = "..data["name"].."--1774" );
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
					..t_boolDesc[isAnyDifficultCanJoin].."�� name = "..data["name"].."--1842" );
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

-- ������пɲμӻ������
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

-- ������пɲμ������ѶȻ������
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

-- ������пɲμ������Ѷ��ұ���ѡ�Ļ������
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


-- ���������ѡ��������
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

-- ���������ѡ���������Ƿ�Ϊ0
t_allAutoTeamActives["isSelectEmpty"] = 
function ( self )
	return self:getSelectNum() == 0;
end

-- �Ƿ����пɲμӵĻ����ѡ����
t_allAutoTeamActives["isAllSelect"] = 
function ( self )
	return self:getSelectNum() == self:getCanJoinNum();
end

-- �Ƿ����пɲμӵĻ����ѡ����
t_allAutoTeamActives["isAllHaveDiffSelect"] = 
function ( self )
	return self:getCanJoinWithDiffAndSelectNum() == self:getCanJoinWithDiffNum();
end

-- �Ƿ����еĻ������ѡ����
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

-- �Ƿ����еļ�ѡ���ѡ��
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

-- �Ƿ����е���ͨѡ���ѡ��
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

-- �Ƿ����е�����ѡ���ѡ��
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

-- �Ƿ����е�ר��ѡ���ѡ��
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

-- ������л����
t_allAutoTeamActives["getTotalNum"] = 
function ( self )
	return table.getn( self );
end

-- ��ѯĳ�����ݽڵ�
t_allAutoTeamActives["getActiveData"] = 
function ( self, nIndex )
	return self[nIndex];
end

-- ����ĳ���ڵ������
t_allAutoTeamActives["setActiveData"] = 
function ( self, nIndex, data )
	self[nIndex] = data;
end

-- �Ƿ��пɲμӵĻ
t_allAutoTeamActives["isCanJoinActiveEmpty"] = 
function ( self )
	return self:getCanJoinNum() == 0;
end

-- �����Ҫ��ѡ�Ѷ��ҿ��ԲμӵĻ����
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

-- �����Ԫ��
t_allAutoTeamActives["insertData"] = 
function ( self, data )
	table.insert( self, {	id			= data["id"],			simple	= data["simple"],	normal	= data["normal"],	hard = data["hard"],
							professional= data["professional"],	haveDiff= data["haveDiff"],	select	= data["select"],	join	= data["join"],		
							name		= data["name"],			row		= data["row"],		teamID	= data["teamID"],
							joinSimple	= data["joinSimple"],	joinNormal		= data["joinNormal"],
							joinHard	= data["joinHard"],		joinProfessional= data["joinProfessional"],	needLv = data["needLv"], finish = data["finish"]
						} );
end

-- ������л����
t_allAutoTeamActives["clear"] = 
function ( self )
	for i = 1, table.getn( self ) do
		table.remove( self );
	end
end

-- ���ݻ���ֻ�ȡ���Ϣ
t_allAutoTeamActives["GetActiveDataByRow"] = 
function ( self, data )
	for _, element in ipairs( self ) do
		if data["row"] == element["row"] then
			return element;
		end
	end
	
	return nil;
end

-- ���ݻ���ֻ�ȡ�����
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

-- �Ƿ��и���ûѡ���Ѷ�
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
	
	-- �Ƿ�Ҫ��ʾ �Ѷ� 
	local t_showIcons = { };
	local t_fonts = {};
	if oneActiveInfo["needLv"] ~= nil then
		t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneActiveInfo["name"],},
					[btn:GetName().."LvFont"]	= { value = oneActiveInfo["needLv"] .. "������" }, }
	else
		if oneActiveInfo["haveDiff"] then			
			t_showIcons =	{	"SearchTeamFrame_CopyInfoAllDifficultyChkBtn"..nBtnIndex, 
								"SearchTeamFrame_CopyInfoSimpleChkBtn"..nBtnIndex, 
								"SearchTeamFrame_CopyInfoNormalChkBtn"..nBtnIndex, 
								"SearchTeamFrame_CopyInfoHardChkBtn"..nBtnIndex,
								"SearchTeamFrame_CopyInfoProfessionalChkBtn"..nBtnIndex,
							};
			
			t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneActiveInfo["name"], },
						[btn:GetName().."SimpleFont"]	= { value = "��" },
						[btn:GetName().."NormalFont"]	= { value = "��ͨ" },
						[btn:GetName().."HardFont"]		= { value = "����" },
						[btn:GetName().."ProfessionalFont"]	= { value = "ר��"	},					
					};
		else
			t_showIcons = { "SearchTeamFrame_CopyInfoAllDifficultyChkBtn"..nBtnIndex, };
			t_fonts = {	[btn:GetName().."NameFont"]	= { value = oneActiveInfo["name"], },
						[btn:GetName().."EventFont"]	= { value = "�����" }, };
		end
	end

	util.ShowIcons( t_showIcons );
	util.SetFonts( t_fonts );
	
	-- ���� �Ѷ� chk button
	local t_chkBtns = {};
	if oneActiveInfo["row"] == 8 then
		print( "SetSearchTeamFrameBtnByContinue--name = "..oneActiveInfo["name"].." �� joinState = "..t_boolDesc[oneActiveInfo["join"]].."--2113" );
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
	
	-- �Ƿ�Ҫ��ʾ �Ѷ� 
	local t_showIcons = { };
	local t_fonts = {};
	if oneActiveInfo["needLv"] ~= nil then
		if oneActiveInfo["haveDiff"] then
			t_fonts = {		[btn:GetName().."NameFont"]		= { value = oneActiveInfo["name"], },
							[btn:GetName().."LvFont"]	= { value = oneActiveInfo["needLv"] .. "������",color = { ["r"] = 255, ["g"] = 0, ["b"] = 0 } } }
		else
			t_fonts = {		[btn:GetName().."NameFont"]		= { value = oneActiveInfo["name"], },
							[btn:GetName().."EventFont"]	= { value = "�����",color = { ["r"] = 255, ["g"] = 0, ["b"] = 0 } }}
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
						[btn:GetName().."SimpleFont"]	= { value = "��" },
						[btn:GetName().."NormalFont"]	= { value = "��ͨ" },
						[btn:GetName().."HardFont"]		= { value = "����" },
						[btn:GetName().."ProfessionalFont"]	= { value = "ר��" },					
					};			
		else
			if oneActiveInfo["finish"] then
				t_fonts = {		[btn:GetName().."NameFont"]		= { value = oneActiveInfo["name"], },
								[btn:GetName().."EventFont"]	= { value = "�����",color = { ["r"] = 120, ["g"] = 120, ["b"] = 120 } }}
			else
				if oneActiveInfo["join"] then
					t_showIcons = { "SearchTeamFrame_CopyInfoAllDifficultyChkBtn"..nBtnIndex, };
					t_fonts = {	[btn:GetName().."NameFont"]	= { value = oneActiveInfo["name"], },
								[btn:GetName().."EventFont"]	= { value = "�����",color = { ["r"] = 0, ["g"] = 255, ["b"] = 0 } } };
				else
					t_fonts = {	[btn:GetName().."NameFont"]	= { value = oneActiveInfo["name"], },
								[btn:GetName().."EventFont"]	= { value = "�����",color = { ["r"] = 255, ["g"] = 0, ["b"] = 0 } } };
				end
			end
		end
	end

	util.ShowIcons( t_showIcons );
	util.SetFonts( t_fonts );
	
	-- ���� �Ѷ� chk button
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

	-- ��鶥����ȫѡ��ť
	
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
	-- �մ򿪽���Ĭ��ѡ�е�һ����������һ���������ܲ���������
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

-- �����Զ����
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
		
		print( "ContinueAutoTeam--name = "..oneActiveData["name"].." �� nResult = "..nResult.."--2433" );
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
	-- �մ򿪽���Ĭ��ѡ�е�һ����������һ���������ܲ���������
	UpdateSearchTeamFrame();

	this:SetPoint( "center", "UIClient", "center", 0, 0 );
	SetCurSuoXiaoFrameInfo( { name = this:GetName() } );
end

function SearchTeamFrame_CloseBtn_OnClick()
	SearchTeamFrame:Hide();
	--SetCurSuoXiaoFrameInfo( { name = this:GetParent() } );
end

local t_cannotJoinActive =	{	[PROMOTION_SINGLE_LIMIT]	= { failDesc = "������ӽ��룬Ҫ���˲ſɽ���", str = "PROMOTION_SINGLE_LIMIT" }, 
								[PROMOTION_NOT_START]		= { failDesc = "���û��ʼ", str = "PROMOTION_NOT_START" }, 
								[PROMOTION_LEVEL_LIMIT]		= { failDesc = "�ȼ�����", str = "PROMOTION_LEVEL_LIMIT" },
								[PROMOTION_HAVE_FINISH]		= { failDesc = "��ѽ���", str = "PROMOTION_HAVE_FINISH" },
							};
function SearchTeamFrame_CopyInfoChkBtn_OnEnter()
	local slider		= getglobal( "SearchTeamFrame_ScrollBar" );
	local copyInfoBtn	= getglobal( "SearchTeamFram_CopyInfoBtn"..this:GetClientID() );
	local nRow			= copyInfoBtn:GetClientUserData( 0 );
	local nameFont		= getglobal( copyInfoBtn:GetName().. "NameFont" );
	copyInfoBtn:Highlight();
	-- Ҫ����tips
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
	-- ���س�tips
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

-- �Ƿ���Ե��ȫѡ��ť
function CanClickAllSelectChkBtnWithoutDifficulty()
	if t_allAutoTeamActives:isCanJoinActiveEmpty() then
		ShowMidTips( "û�п��Խ���Ļ" );
		return false;
	end

	return true;
end

function CanClickAllSelectChkBtnWithDifficulty()
	if t_allAutoTeamActives:isCanJoinActiveEmpty() then
		ShowMidTips( "û�п��Խ���Ļ" );
		return false;
	end

	if t_allAutoTeamActives:getCanJoinWithDiffNum() == 0 then
		ShowMidTips( "û�л��Ҫ��ѡ�Ѷ�" );
		return false;
	end

	return true;
end


-- ���ȫѡ��ť
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
		-- ���ǵ�ǰ���ǿ��Բμӵ�
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
		-- ���ǵ�ǰ���ǿ��Բμӵ�
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

-- �����ȫѡ
function SearchTeamFrame_SelectAllSimpleChkBtn_OnClick()
	if not CanChangeOption() then
		this:setCheckState( not this:GetCheckState() );
		return;
	end
	
	if this:GetCheckState() and not t_allAutoTeamActives:isAllHaveDiffSelect() then
		this:setCheckState( false );
		ShowMidTips( "���ֿɲμӻû��ѡ�����ȹ�ѡ" );
		return;
	end

	if not CanClickAllSelectChkBtnWithDifficulty() then
		this:setCheckState( false );
		return;
	end
	
	for i = 1, t_allAutoTeamActives:getTotalNum() do
		local oneActiveData = t_allAutoTeamActives:getActiveData( i );
		-- ���ǵ�ǰ���ǿ��Բμӵ�
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
		ShowMidTips( "���ֿɲμӻû��ѡ�����ȹ�ѡ" );
		return;
	end

	if not CanClickAllSelectChkBtnWithDifficulty() then
		return;
	end

	chkBtn:setCheckState( not chkBtn:GetCheckState() );

	for i = 1, t_allAutoTeamActives:getTotalNum() do
		local oneActiveData = t_allAutoTeamActives:getActiveData( i );
		-- ���ǵ�ǰ���ǿ��Բμӵ�
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

-- �����ͨȫѡ
function SearchTeamFrame_SelectAllNormalChkBtn_OnClick()
	if not CanChangeOption() then
		this:setCheckState( not this:GetCheckState() );
		return;
	end
	
	if  this:GetCheckState() and not t_allAutoTeamActives:isAllHaveDiffSelect() then
		ShowMidTips( "���ֿɲμӻû��ѡ�����ȹ�ѡ" );
		this:setCheckState( false );
		return;
	end

	if not CanClickAllSelectChkBtnWithDifficulty() then
		this:setCheckState( false );
		return;
	end
	
	for i = 1, t_allAutoTeamActives:getTotalNum() do
		local oneActiveData = t_allAutoTeamActives:getActiveData( i );
		-- ���ǵ�ǰ���ǿ��Բμӵ�
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
		ShowMidTips( "���ֿɲμӻû��ѡ�����ȹ�ѡ" );
		return;
	end

	if not CanClickAllSelectChkBtnWithDifficulty() then
		return;
	end
	
	
	chkBtn:setCheckState( not chkBtn:GetCheckState() );

	for i = 1, t_allAutoTeamActives:getTotalNum() do
		local oneActiveData = t_allAutoTeamActives:getActiveData( i );
		-- ���ǵ�ǰ���ǿ��Բμӵ�
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

-- �������ȫѡ
function SearchTeamFrame_SelectAllHardChkBtn_OnClick()
	if not CanChangeOption() then
		this:setCheckState( not this:GetCheckState() );
		return;
	end
	
	if this:GetCheckState() and not t_allAutoTeamActives:isAllHaveDiffSelect() then
		this:setCheckState( false );
		ShowMidTips( "���ֿɲμӻû��ѡ�����ȹ�ѡ" );
		return;
	end

	if not CanClickAllSelectChkBtnWithDifficulty() then
		this:setCheckState( false );
		return;
	end
	
	for i = 1, t_allAutoTeamActives:getTotalNum() do
		local oneActiveData = t_allAutoTeamActives:getActiveData( i );
		-- ���ǵ�ǰ���ǿ��Բμӵ�
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
		ShowMidTips( "���ֿɲμӻû��ѡ�����ȹ�ѡ" );
		return;
	end

	if not CanClickAllSelectChkBtnWithDifficulty() then
		return;
	end
	
	chkBtn:setCheckState( not chkBtn:GetCheckState() );
	for i = 1, t_allAutoTeamActives:getTotalNum() do
		local oneActiveData = t_allAutoTeamActives:getActiveData( i );
		-- ���ǵ�ǰ���ǿ��Բμӵ�
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

-- ���ר��ȫѡ
function SearchTeamFrame_SelectAllProfessionalChkBtn_OnClick()
	if not CanChangeOption() then
		this:setCheckState( not this:GetCheckState() );
		return;
	end
	
	if this:GetCheckState() and not t_allAutoTeamActives:isAllHaveDiffSelect() then
		this:setCheckState( false );
		ShowMidTips( "���ֿɲμӻû��ѡ�����ȹ�ѡ" );
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

		-- ���ǵ�ǰ���ǿ��Բμӵ�
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
		ShowMidTips( "���ֿɲμӻû��ѡ�����ȹ�ѡ" );
		return;
	end

	if not CanClickAllSelectChkBtnWithDifficulty() then
		return;
	end
	
	
	chkBtn:setCheckState( not chkBtn:GetCheckState() );
	for i = 1, t_allAutoTeamActives:getTotalNum() do
		local oneActiveData = t_allAutoTeamActives:getActiveData( i );
		-- ���ǵ�ǰ���ǿ��Բμӵ�
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

-- �㸱���Ѷ�����ǰ���chk��ť
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

-- �㸱���Ѷȵļ�chk��ť
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
		ShowMidTips("����ֵ����" .. useVig .. "������ʹ���Զ����");
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

-- �㸱���Ѷȵ���ͨchk��ť
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
		ShowMidTips("����ֵ����" .. useVig .. "������ʹ���Զ����");
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

-- �㸱���Ѷȵ�����chk��ť
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
		ShowMidTips("����ֵ����" .. useVig .. "������ʹ���Զ����");
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

-- �㸱���Ѷȵ�ר��chk��ť
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
		ShowMidTips("����ֵ����" .. useVig .. "������ʹ���Զ����");
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

-- ������Ϲ����İ�ť
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

-- ������¹����İ�ť
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

-- Ѱ�����ǰ������ѡ��ť
local t_preConditionBtnMaps = 
{
	["SearchTeamFrame_AutoTeamRaceButton1"] = { career = "սʿ", chkBtn = "SearchTeamFrame_AutoTeamRaceChkBtn1", 
												tips = "սʿϵְҵ����Ϊսʿ��תְ��ķ�������ػ��ߣ�����������������" },	
	["SearchTeamFrame_RaceDescBtn1"]		= { career = "սʿ", chkBtn = "SearchTeamFrame_AutoTeamRaceChkBtn1",
												tips = "սʿϵְҵ����Ϊսʿ��תְ��ķ�������ػ��ߣ�����������������" },

	["SearchTeamFrame_AutoTeamRaceButton2"] = { career = "��ʦ", chkBtn = "SearchTeamFrame_AutoTeamRaceChkBtn2",
												tips = "��ʦϵְҵ����Ϊ��ʦ��תְ��ı�ϵ������ľ����ߣ���ϵ������ĺ��׷�ʦ" }, 
	["SearchTeamFrame_RaceDescBtn2"]		= { career = "��ʦ", chkBtn = "SearchTeamFrame_AutoTeamRaceChkBtn2",
												tips = "��ʦϵְҵ����Ϊ��ʦ��תְ��ı�ϵ������ľ����ߣ���ϵ������ĺ��׷�ʦ" },

	["SearchTeamFrame_AutoTeamRaceButton3"] = { career = "ǹ��", chkBtn = "SearchTeamFrame_AutoTeamRaceChkBtn3",
												tips = "ǹ��ϵְҵ����Ϊǹ�ּ�תְ����ٻ������ɱ�ߣ���Χ������Ļ�����" }, 
	["SearchTeamFrame_RaceDescBtn3"]		= { career = "ǹ��", chkBtn = "SearchTeamFrame_AutoTeamRaceChkBtn3",
												tips = "ǹ��ϵְҵ����Ϊǹ�ּ�תְ����ٻ������ɱ�ߣ���Χ������Ļ�����" },

	["SearchTeamFrame_AutoTeamRaceButton4"] = { career = "��˾", chkBtn = "SearchTeamFrame_AutoTeamRaceChkBtn4",
												tips = "��˾ϵְҵ����Ϊ��˾��תְ����������ʥ���ߣ���ϵ������Ԧ��ʦ" }, 
	["SearchTeamFrame_RaceDescBtn4"]		= { career = "��˾", chkBtn = "SearchTeamFrame_AutoTeamRaceChkBtn4",
												tips = "��˾ϵְҵ����Ϊ��˾��תְ����������ʥ���ߣ���ϵ������Ԧ��ʦ" },

	["SearchTeamFrame_AutoTeamRaceButton5"] = { career = "���뵱�ӳ�", chkBtn = "SearchTeamFrame_AutoTeamRaceChkBtn5" }, 
	["SearchTeamFrame_RaceDescBtn5"]		= { career = "���뵱�ӳ�", chkBtn = "SearchTeamFrame_AutoTeamRaceChkBtn5" },
};

local t_preConditionChkBtnMaps = 
{
	["SearchTeamFrame_AutoTeamRaceChkBtn1"] = { career = "սʿ", enumValue = CLT_AUTO_TEAM_LIMIT_WARRIOR },	

	["SearchTeamFrame_AutoTeamRaceChkBtn2"] = { career = "��ʦ", enumValue = CLT_AUTO_TEAM_LIMIT_MAGICOR }, 

	["SearchTeamFrame_AutoTeamRaceChkBtn3"] = { career = "ǹ��", enumValue = CLT_AUTO_TEAM_LIMIT_GUNER }, 

	["SearchTeamFrame_AutoTeamRaceChkBtn4"] = { career = "��˾", enumValue = CLT_AUTO_TEAM_LIMIT_CHUNCH }, 

	["SearchTeamFrame_AutoTeamRaceChkBtn5"] = { career = "���뵱�ӳ�", enumValue = CLT_AUTO_TEAM_LIMIT_LEADER }, 
};

local t_canChangeOptionFrames = { "ConfirmDifficultyFrame", "ConfirmDifficultyAndCareerFrame" };
function CanChangeOption()
	for _, name in ipairs( t_canChangeOptionFrames ) do
		local frame = getglobal( name );
		if frame:IsShown() then
			ShowMidTips( "���Ƚ���ȷ�ϲ������ܽ��иò���" );
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
-- ս������
function SearchTeamFram_BattleSignUpBtn_OnClick()
	SearchTeamFrame:Hide();
	BattleStartFrame:Show();
end
-- �����
function SearchTeamFram_ActivityAssistBtn_OnClick()
	local serchFrame = getglobal( this:GetParent() );
	PromotionFrame:Show();
end

function SearchTeamFram_ActivityAssistBtn_OnEnter()
	local szText = "����������ֽ���";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function SearchTeamFram_ActivityAssistBtn_OnLeave()
	GameTooltip:Hide();
end

local MIN_MANY_CAREER = 2;
-- Ѱ�����
function CanSearchTeam()
	if IsAnyPworldDiffNotSel() then
		WarnNotSelectDiffFrame:Show();
		return false;
	end

	if t_allAutoTeamActives:isSelectEmpty() then
		ShowMidTips( "������ѡ��һ���������" );
		return;
	end
	
	local mainplayer = ActorMgr:getMainPlayer();	
	if mainplayer:isInTeam() and not mainplayer:isCaptain() then
		ShowMidTips( "���Ѿ��ڶ����У�����ӳ�����Ȩ�޽��иò���" );
		return false;
	end

	if mainplayer:isDead() then
		ShowMidTips( "����״̬���������ò���" );
		return false;
	end

	return true;
end

function SearchTeamFram_SearchTeamBtn_OnClick()
	-- �Ƿ���ѡ��һ��ѡ��
	if not CanSearchTeam() then
		return;
	end

	local t_selInfo		= { careerNum = 0, hardNum = 0, firstHardName = "" };
	local nSelCareer	= false;
	local nHardMode		= 0;
	-- �ȼ��ѡ���ְҵ�Ƿ�̫��
	local t_carrerChkBtns = { "SearchTeamFrame_AutoTeamRaceChkBtn1", "SearchTeamFrame_AutoTeamRaceChkBtn2", 
								"SearchTeamFrame_AutoTeamRaceChkBtn3", "SearchTeamFrame_AutoTeamRaceChkBtn4"
							};
	for _, name in pairs( t_carrerChkBtns ) do
		local chkBtn = getglobal( name );
		if chkBtn:GetCheckState() then
			t_selInfo.careerNum = t_selInfo.careerNum + 1;
		end
	end

	-- �ټ���Ƿ�ѡ�����Ѷȸߵĸ���
	for i = 1, t_allAutoTeamActives:getTotalNum() do
		local data = t_allAutoTeamActives:getActiveData( i );
		if data["haveDiff"] and data["professional"] and data["join"] and data["select"] then
			t_selInfo["hardNum"] = t_selInfo["hardNum"] + 1;
			if t_selInfo["hardNum"] == 1 then
				t_selInfo["firstHardName"] = data["name"];
			end
		end
	end

	-- ��ͬʱ�����Ѷ���ѡ����̫��ְҵ
	if t_selInfo["careerNum"] >= MIN_MANY_CAREER and t_selInfo["hardNum"] > 0 then
		local t_riches = { };
		if t_selInfo["hardNum"] == 1 then
			t_riches = { ["ConfirmDifficultyAndCareerFrame_DescRich"] = { 
							value = ("    #c12e630"..t_selInfo["firstHardName"].."#n�ĸ�����ѡ����".."#cfffebd".."�� ר�� ��#n����ȷ����\n    ��Ҫ��ı���ְҵ����̫�࣬��Ӵ�ƥ�������Ѷȣ���ȷ��Ҫ��ô����"), 
							color = { r = 147, g = 137, b = 89 } }
						};
		else
			t_riches = { ["ConfirmDifficultyAndCareerFrame_DescRich"] = { 
							value = ("    ��ѡ�ĸ�������ר��ģʽ����ȷ����\n    ��Ҫ��ı���ְҵ����̫�࣬��Ӵ�ƥ�������Ѷȣ���ȷ��Ҫ��ô����"), 
							color = { r = 147, g = 137, b = 89 } }
						};
		end

		util.SetRiches( t_riches );
		ConfirmDifficultyAndCareerFrame:Show();
		return;
	end

	-- ��ѡ����̫��ְҵ
	if t_selInfo["careerNum"] >= MIN_MANY_CAREER then
		local t_riches = { ["ConfirmDifficultyFrame_DescRich"] = { 
							value = "    ��Ҫ��ı���ְҵ����̫�࣬��Ӵ�ƥ�������Ѷȣ���ȷ��Ҫ��ô����", 
							color = { r = 147, g = 137, b = 89 } }
						};
		
		util.SetRiches( t_riches );
		ConfirmDifficultyFrame:Show();
		return;
	end

	-- ��ѡ�����Ѷȸߵ�ѡ��

	if t_selInfo["hardNum"] > 0 then
		local t_riches = { };
		
		if t_selInfo["hardNum"] == 1 then
			t_riches = { ["ConfirmDifficultyFrame_DescRich"] = { 
							value = "    #c12e630"..t_selInfo["firstHardName"].."#n�ĸ�����ѡ����".."#cfffebd".."�� ר�� ��#n����ȷ����", 
							color = { r = 147, g = 137, b = 89 } },
						};
		else
			t_riches = { ["ConfirmDifficultyFrame_DescRich"] = { 
							value = "#P��ѡ�ĸ�������ר��ģʽ����ȷ����", 
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
	
	-- ���ø���Ҫ��
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
	{ value = "�ٹ�"..nLeaveTime.."����Զ���ʼ�������̣�" }, 
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
	{	[this:GetName().."BlackButtonDescFont"]	= { value = "�ٹ�"..nLeaveTime.."����Զ���ʼ�������̣�" }, 
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
		-- ѡ��ר��
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
		-- ѡ������
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

-----------------------------�Զ��������----------------------------------

function CreatingAutoTeamFrame_SendMessageBtn_OnClick()
	local mainplayer	= ActorMgr:getMainPlayer();
	local szMyName		= mainplayer:getName();
	local szParentName	= this:GetParent();
	local inputEdit		= getglobal( szParentName.."InputEdit" );
	local szChatContent = inputEdit:GetText();

	-- �������Ϳ���Ϣ
	if "" == szChatContent then return end

	-- ����Ϣ�����������
	local t_ClientSelfChatContentParser = GetClientSelfChatContentParser();
	sz2Server = t_ClientSelfChatContentParser:parseLinkTextForServe( { ["text"] = szChatContent } );	
	ChatManager:requestChat( CL_CHAT_AUTOTEAM, "", sz2Server );

	-- ���������е�����
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
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "ѡ�����", frame = this:GetParent(),
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
	-- �����Ц������е�����һ��Ц����,�ر�Ц�����
	local editFrame = getglobal( CreatingAutoTeamFaceFrame:GetClientString() );
	-- ��ȡ�����������
	local inputEdit = getglobal( editFrame:GetName() .. "InputEdit");
	if arg1 < 1 then 
		return; 
	end

	--�������
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

	--�������
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

-- ����ֵ�Ƿ��㹻���Խ��븱��
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