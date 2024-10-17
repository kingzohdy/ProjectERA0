-- 聊天主面板
local WORLD_PAGE_TYPE	 = 0;
local ZONGHE_PAGE_TYPE	 = 1;
local CLAN_PAGE_TYPE	 = 2;
local TEAM_PAGE_TYPE	 = 3;
local NEAR_PAGE_TYPE	 = 4;
local PRIVATE_PAGE_TYPE	 = 5;
local XINXI_PAGE_TYPE	 = 6;

t_mainChatFrameTextColor = 
{
[CL_CHAT_SAY]		= { ["r"] = 255,	["g"] = 255, ["b"] = 255	},
[CL_CHAT_TEAM]		= { ["r"] = 63,		["g"] = 176, ["b"] = 255	},
[CL_CHAT_PRIVATE]	= { ["r"] = 218,	["g"] = 109, ["b"] = 250	},
[CL_CHAT_CLAN]		= { ["r"] = 122,	["g"] = 211, ["b"] = 43		},
[CL_CHAT_CLAN_GROUP]	= { ["r"] = 0,		["g"] = 255, ["b"] = 255	},
[CL_CHAT_WORLD]		= { ["r"] = 255,	["g"] = 186, ["b"] = 138	},
-- 收费信息
[CL_CHAT_ITEM]		= { ["r"] = 217,	["g"] = 219, ["b"] = 60		},
[CL_CHAT_SPAN]		= { ["r"] = 253,	["g"] = 70,  ["b"] = 34		},
[CL_CHAT_BATTLE]	= { ["r"] = 255,	["g"] = 125, ["b"] = 0		},
[CL_CHAT_MONSTER]	= { ["r"] = 255,	["g"] = 255, ["b"] = 255	},
[CL_CHAT_SYS_MSG]	= { ["r"] = 255,	["g"] = 255, ["b"] = 0		},
[CL_CHAT_ZONG_HE_MSG]	= { ["r"] = 160,	["g"] = 255, ["b"] = 255	},
[CL_CHAT_SPAN_MAP_WORLD]= { ["r"] = 240,	["g"] = 159, ["b"] = 117	},
[CL_CHAT_SPAN_MAP]	= { ["r"] = 0,		["g"] = 255, ["b"] = 255	},
[CL_CHAT_END]		= { ["r"] = 255,	["g"] = 255, ["b"] = 255	},
};

--系统信息
t_sysTextColor = 
{
[SYS_TIP] = { ["r"] = 255,	["g"] = 255, ["b"] = 0	},
};

t_mainChatFrameConfig = 
{
	[CL_CHAT_SAY]		= { ["anim"] = "#A003", ["desc"] = "附近",	["historyRich"] = "ChatHistoryMsgMgrFrame_NearRich",		},
	[CL_CHAT_TEAM]		= { ["anim"] = "#A001", ["desc"] = "队伍",	["historyRich"] = "ChatHistoryMsgMgrFrame_TeamRich"		},
	[CL_CHAT_PRIVATE]	= { ["anim"] = "#A010", ["desc"] = "私聊",	["historyRich"] = "ChatHistoryMsgMgrFrame_PrivateRich"		},
	[CL_CHAT_CLAN]		= { ["anim"] = "#A008", ["desc"] = "公会",	["historyRich"] = "ChatHistoryMsgMgrFrame_ClanRich"		},
	[CL_CHAT_CLAN_GROUP]	= { ["anim"] = "#A019", ["desc"] = "战团",	["historyRich"] = "ChatHistoryMsgMgrFrame_ClanGroupRich"	},
	[CL_CHAT_WORLD]		= { ["anim"] = "#A002", ["desc"] = "世界",	["historyRich"] = "ChatHistoryMsgMgrFrame_WorldRich"		},
	[CL_CHAT_ITEM]		= { ["anim"] = "#A012", ["desc"] = "喇叭",	["historyRich"] = "ChatHistoryMsgMgrFrame_BugleRich"		},
	[CL_CHAT_BATTLE]	= { ["anim"] = "#A007", ["desc"] = "战场",	["historyRich"] = "ChatHistoryMsgMgrFrame_NearRich"		},
	[CL_CHAT_SYS_MSG]	= { ["anim"] = "#A006", ["desc"] = "系统",	["historyRich"] = "ChatHistoryMsgMgrFrame_SystemRich"		},
	[CL_CHAT_XIN_XI_MSG]	= { ["anim"] = "#A005", ["desc"] = "信息",	["historyRich"] = "ChatHistoryMsgMgrFrame_MsgRich"		},
	[CL_CHAT_ZONG_HE_MSG]	= { ["anim"] = "#A011",	["desc"] = "综合",	["historyRich"] = "ChatHistoryMsgMgrFrame_NearRich"		},
	[CL_CHAT_SPAN]		= { ["anim"] = "#A012", ["desc"] = "喇叭",	["historyRich"] = "ChatHistoryMsgMgrFrame_BugleRich"		},
	[CL_CHAT_SPAN_MAP_WORLD]= { ["anim"] = "#A022", ["desc"] = "本服",	["historyRich"] = "ChatHistoryMsgMgrFrame_NearRich"		},
	[CL_CHAT_SPAN_MAP]	= { ["anim"] = "#A023", ["desc"] = "跨服",	["historyRich"] = "ChatHistoryMsgMgrFrame_NearRich"		},
};

-- 切换聊天页面类型
local t_allChatTypePage = 
{
[ZONGHE_PAGE_TYPE]	= { ["rich"] = "MainChatFrame_ZongHeRich",			["btn"] = "MainChatFrame_ZongHeBtn",	
						["chatType"] = CL_CHAT_ZONG_HE_MSG,
					},

[WORLD_PAGE_TYPE]	= { ["rich"] = "MainChatFrame_ChatWorldRich",		["btn"] = "MainChatFrame_WorldBtn",		
						["chatType"] = CL_CHAT_WORLD,
					},

[CLAN_PAGE_TYPE]	= { ["rich"] = "MainChatFrame_ChatClanRich",		["btn"] = "MainChatFrame_ClanBtn",	
						["chatType"] = CL_CHAT_CLAN,
						},

[TEAM_PAGE_TYPE]	= { ["rich"] = "MainChatFrame_ChatTeamRich",		["btn"] = "MainChatFrame_TeamBtn",		
						["chatType"] = CL_CHAT_TEAM,
						},

[NEAR_PAGE_TYPE]	= { ["rich"] = "MainChatFrame_ChatNearRich",		["btn"] = "MainChatFrame_NearBtn",	
						["chatType"] = CL_CHAT_SAY,
						},

[PRIVATE_PAGE_TYPE]	= { ["rich"] = "MainChatFrame_ChatPrivateRich",		["btn"] = "MainChatFrame_PrivateBtn",	
						["chatType"] = CL_CHAT_PRIVATE,
					},

[XINXI_PAGE_TYPE]	= { ["rich"] = "MainChatFrame_ChatXinXiRich",		["btn"] = "MainChatFrame_XinXiBtn",	
						["chatType"] = CL_CHAT_XIN_XI_MSG,
					},
};

function GetPingDaoChatType( dt )
	local szAnimText = dt["anim"];
	for nChatType, data in pairs( t_mainChatFrameConfig ) do
		if data["anim"] == szAnimText then
			return nChatType;
		end
	end

	return nil;
end

local CHAT_BACK_DEFAULT_WIDTH	= 304;
local CHAT_BACK_DEFAULT_HEIGHT	= 186;
local CHAT_SLIDER_DEFAULT_HEIGHT= 146;
local CHAT_SCROODOWN_TO_UP_DIST	= 144;
local CHAT_PINGBI_TO_UP_DIST	= 168;

local CHAT_FRAME_DEFAULT_WIDTH	= 326;
local CHAT_FRAME_DEFAULT_HEIGHT	= 205;

local CHAT_RICH_DEFAULT_WIDTH	= 295;
local CHAT_RICH_DEFAULT_HEIGHT	= 175;
local CHAT_INCR_UNIT			= 50;

local BTN_HOLD_TIME			= 0.15;
local t_MainChatFrameControl = 
{ 
["pageType"] = ZONGHE_PAGE_TYPE, ["defaultChatBackWidth"] = CHAT_BACK_DEFAULT_WIDTH, ["defaultChatBackHeight"] = CHAT_BACK_DEFAULT_HEIGHT,
["defaultChatRichWidth"] = CHAT_RICH_DEFAULT_WIDTH,	["defaultIncrUnit"] = CHAT_INCR_UNIT,	["lastClickScrollBtnTime"] = 0,

-- [pageType] = { startTime = 0, isPlayAnim = true, }
["GetNewMessagePage"] = {},
};

function GetMainChatFrameControl()
	return t_MainChatFrameControl;
end

t_MainChatFrameControl["init"] =
function ( self )
	self["GetNewMessagePage"] = {};
end

t_MainChatFrameControl["isPageHaveNewMessage"] =
function ( self, data )	
	local nPageType				= data["pageType"];
	local t_onePageGetNewInfo	= self["GetNewMessagePage"][nPageType];
	
	return t_onePageGetNewInfo ~= nil;
end

t_MainChatFrameControl["addNewMessage"] =
function ( self, data )
	if self:isPageHaveNewMessage( data ) then
		return;
	end
	
	local nPageType	= data["pageType"];
	self["GetNewMessagePage"][nPageType] = {};
	local t_onePageGetNewInfo			= self["GetNewMessagePage"][nPageType];
	t_onePageGetNewInfo["startTime"]	= GameMgr:getTickTime();
	t_onePageGetNewInfo["isPlayAnim"]	= true;
end

t_MainChatFrameControl["delNewMessage"] =
function ( self, data )
	local nPageType	= data["pageType"];
	self["GetNewMessagePage"][nPageType] = nil;
end

t_MainChatFrameControl["stopPlayNewMessageAnim"] =
function ( self, data )
	if not self:isPageHaveNewMessage( data ) then
		return;
	end
	
	local nPageType				= data["pageType"];
	local t_onePageGetNewInfo	= self["GetNewMessagePage"][nPageType];
	t_onePageGetNewInfo["isPlayAnim"] = false;
end

t_MainChatFrameControl["getAllNewMessage"] =
function ( self, data )
	return self["GetNewMessagePage"];
end

t_MainChatFrameControl["getPageType"] =
function ( self )
	return self["pageType"];
end

t_MainChatFrameControl["setPageType"] =
function ( self, data )
	self["pageType"] = data["pageType"];
end

t_MainChatFrameControl["convertPageTypeToChatType"] =
function ( self, data )
	local t_onePageTypeConfig = t_allChatTypePage[data["pageType"]];
	if t_onePageTypeConfig == nil then
		return CL_CHAT_ZONG_HE_MSG;
	end

	return t_onePageTypeConfig["chatType"];
end

t_MainChatFrameControl["convertChatTypeToPageType"] =
function ( self, data )
	for pageType, onePageTypeConfig in pairs( t_allChatTypePage ) do
		if onePageTypeConfig["chatType"] == data["chatType"] then
			return pageType;
		end
	end

	return ZONGHE_PAGE_TYPE;
end

local t_MainChatFrameEvents = 
{ 
	["GE_UPDATE_CHATMSG"] = {},				["GE_SET_CHATLINK"] = {},	["GE_UPDATE_SYSMSG"] = {},	["GE_UPDATE_SYS_BULLETIN"] = {},
	["GE_UPDATE_CHATFRAME_BY_CLIENT"] = {},	["GE_ENTER_PLAYERLOGIN"] = {},	["GE_OTHERLAYER_CHG_NAME"] = {},
};

t_MainChatFrameEvents["GE_UPDATE_CHATMSG"].func = 
function ()
	local Msg		= UIMSG:getUIMsg();
	local chatMsg	= Msg.ChatMsg;
	local nType		= Msg.ChatMsg.Type;
	local szSpeaker = Msg.ChatMsg.Speaker;
	local szContent = Msg.ChatMsg.Content;
	local nChatType	= GetChatType();
	
	local friendInfo = GameFriendManager:getFriendInfoByName( CLIENT_RELATION_TYPE_BLACK, szSpeaker );
	if friendInfo ~= nil and not ( nType == CL_CHAT_ITEM or nType == CL_CHAT_TEAM ) then
		return;
	end

	if not chatMsg.isVip then
		szContent = Private_ReplaceFunctionChar( { ["text"] = szContent } );
	end
	
	if string.find(szContent,"#@!",1,true) then
		while string.find(szContent,"#@!",1,true) do
			local nBeginPos		= string.find( szContent, "#@!" );
			local nEndPos		= string.find( szContent, "#", nBeginPos + 1, string.len( szContent ) );
			local context		= string.sub( szContent, nBeginPos + string.len( "#@!" ), nEndPos-1 );
			UpdateMainChatFrame_ByServerChatMsg( nType,context,szSpeaker )
			szContent = string.sub( szContent, nEndPos + 1,string.len(szContent) );
		end
	else
		UpdateMainChatFrame_ByServerChatMsg( nType, szContent, szSpeaker )
	end

	UpdateHistorySliderRange( { ["name"] = GetCurShowHistoryRichText() } );
end

t_MainChatFrameEvents["GE_SET_CHATLINK"].func = 
function ()
	local friendChatFrame	= GetCurDrawHighChatFrame();
	-- TODO: nChatType要重构
	local nChatType		= GetChatType();
	local Msg			= UIMSG:getUIMsg();
	local SetLink		= Msg.ChatSetLink;

	if SetLink.m_nLinkType == STORE_LINK then
		local name		= SetLink.m_szLinkName;
		local ClientRich	= SetLink.m_szLinkName2ClientRich;
		local ServerRich	= SetLink.m_szLinkName2ServerRich;
		local nLinkType		= SetLink.m_nLinkType;

		if ChatBugleFrame:IsShown() then
			ChatBugleFrame:Hide();
		end

		ChatBugleFrame:Show();
		ChatBugleFrame:SetClientString( name );
		local t_AdWord = { "零纪元好店铺！这里有最好的商品！", "史上最疯狂大减价！只要998！只要998！！", "你是风儿我是沙！买点东西再回家！", };
		CBFChatInsert:SetText( t_AdWord[math.random(1,#t_AdWord)] );
		CBFChatInsert:SelectAllText();
		SetChatType( CL_CHAT_ITEM );
		if IsHaveLaBaItem() then
			local laBaItem = GetContainerOneLaBaItem();
			SetLaBaItem( { grid = laBaItem:getGridIdx(), list = laBaItem:getListType() } );
			LockItem( laBaItem:getListType(), laBaItem:getGridIdx(), laBaItem:getItemId(), "ChatBugleFrame" );
		else
			SetLaBaItem( { grid = -1, list = -1 } );
		end

		local t_LinkMsg = { ["originalName"]		= name,			["clientFormatName"]	= ClientRich, 
							["serveFormamtName"]	= ServerRich,	["linkType"]			= nLinkType 
							};
		table.insert( t_SetLink, t_LinkMsg );
		return;
	end

	if ChatInsertFrame:IsShown() or ChatBugleFrame:IsShown() or ( friendChatFrame ~= nil ) or FriendMutilSendFrame:IsShown() then
		local name		= SetLink.m_szLinkName;
		local ClientRich	= SetLink.m_szLinkName2ClientRich;
		local ServerRich	= SetLink.m_szLinkName2ServerRich;
		local nLinkType		= SetLink.m_nLinkType;

		if ChatBugleFrame:IsShown() then
			CBFChatInsert:SetText(CBFChatInsert:GetText()..SetLink.m_szLinkName);
			SetFocusFrame("CBFChatInsert");
			ChatBugleFrame:AddLevelRecursive();
		-- TODO: 更改这个判断规则
		elseif FriendMutilSendFrame:IsShown() then
			local edit = getglobal( "FriendMutilSendFrame_InputEdit" );
			edit:SetText( edit:GetText()..SetLink.m_szLinkName );
			SetFocusFrame( edit:GetName() );
		elseif ( friendChatFrame ~= nil ) then
			local frame = friendChatFrame;
			if frame ~= nil and frame:IsShown() then
				local edit = getglobal( frame:GetName().."_InputEdit" );
				edit:SetText( edit:GetText()..SetLink.m_szLinkName );
				SetFocusFrame( edit:GetName() );
			end
		elseif ChatInsertFrame:IsShown() then
			local edit = getglobal( "CSChatInsert" );

			if string.len( edit:GetText()..name ) > MAX_CHATINSERT_CHAR then
				return;
			end

			edit:SetText(edit:GetText()..SetLink.m_szLinkName);
			SetFocusFrame( edit:GetName() );
		end
		
		-- { ["originalName"] = , ["clientFormatName"] = , ["serveFormamtName"] = , ["linkType"] = }
		local t_LinkMsg = { ["originalName"]		= name,			["clientFormatName"]	= ClientRich, 
							["serveFormamtName"]	= ServerRich,	["linkType"]			= nLinkType 
							};
		table.insert( t_SetLink, t_LinkMsg );
	
	else
		--如果当前的聊天编辑窗口没有打开，则忽略掉
		ChatManager:deleteLink( SetLink.m_nLinkType );

		-- TODO: 好像没用
		for index, oneLinkData in ipairs( t_SetLink ) do
			if oneLinkData["linkType"] == SetLink.m_nLinkType then
				table.remove( t_SetLink, index );
				break;
			end
		end
		-- TODO: 好像没用
	end
end

t_MainChatFrameEvents["GE_UPDATE_SYSMSG"].func = 
function ()
	local Msg		= UIMSG:getUIMsg();
	local nType		= Msg.SysMsg.Type;
	local szText	= Msg.SysMsg.Msg;
	local nChatType	= GetChatType();
	local historyRich;
	if nType == SYS_TIP or nType == SYS_SVR then
		local szShowText		= "#A006"..szText;
		local nCanPingBiChatType= GetPingDaoChatType( { ["anim"] = "#A006" } );
		local t_color			= t_mainChatFrameTextColor[nCanPingBiChatType];
		UpdatePersonalAndAllChatRich( { ["text"] = szShowText, ["color"] = t_color, ["chatType"] = nCanPingBiChatType } );

		local t_color = t_sysTextColor[SYS_TIP];
		historyRich = getglobal( "ChatHistoryMsgMgrFrame_SystemRich" );
		historyRich:AddText( "#A006"..szText, t_color["r"], t_color["g"], t_color["b"] );

		if nType == SYS_SVR then
			-- 此为红色的
			AddGameSysBulletint( szText );
		elseif nType == SYS_TIP then
			-- 此为绿色
			AddGameSysTip( szText );
		end
	elseif nType == SYS_COMBAT then
		local szShowText		= "#A005"..szText;
		local nCanPingBiChatType= GetPingDaoChatType( { ["anim"] = "#A005" } );
		local t_color			= t_mainChatFrameTextColor[CL_CHAT_TEAM];
		UpdatePersonalAndAllChatRich( { ["text"] = szShowText, ["color"] = t_color, ["chatType"] = nCanPingBiChatType } );

		historyRich = getglobal( "ChatHistoryMsgMgrFrame_MsgRich" );
		historyRich:AddText( szShowText, t_color["r"], t_color["g"], t_color["b"] );
		UpdateHistorySliderRange( { ["name"] = historyRich:GetName() } );
	elseif nType == SYS_MIDDLE then
		AddGameMiddleTips( szText );
	elseif nType == SYS_MIDDLE2 then
		-- 此为白色的
		AddGameMiddleTips2( szText );
	elseif nType == SYS_CHAT then --SYS_WHOOP	
		local szShowText			= "#A011"..szText;
		local nCanPingBiChatType	= GetPingDaoChatType( { ["anim"] = "#A011" } );
		local t_color				= t_mainChatFrameTextColor[nCanPingBiChatType];
		UpdatePersonalAndAllChatRich( { ["text"] = szShowText, ["color"] = t_color, ["chatType"] = nCanPingBiChatType } );
		AddGameSysTip( szText );

	elseif nType == SYS_POP_WINDOW then
		MessageBoxOK( "提示", szText );
		MessageBoxFrame:SetClientString("SYS_POP_WINDOW");
	elseif nType == SYS_BUGLE then
		local szShowText			= "#A012"..szText;
		local nCanPingBiChatType	= GetPingDaoChatType( { ["anim"] = "#A012" } );
		local t_color				= t_mainChatFrameTextColor[nCanPingBiChatType];
		
		UpdatePersonalAndAllChatRich( { ["text"] = szShowText, ["color"] = t_color, ["chatType"] = nCanPingBiChatType } );
	-- or nType == SYS_MSG_ARM	or nType == SYS_MSG_ITEM
	elseif nType == SYS_MSG_TASK  or nType == SYS_MSG_STUFF or nType == SYS_MSG_ITEM or nType == SYS_MSG_ARM then
		-- TODO: 这个函数干么用的？
		AddGetItemMsg( nType, szText );
	elseif nType == SYS_MSG_CHAT_TIP then
		local t_color	= t_mainChatFrameTextColor[CL_CHAT_END];
		UpdatePersonalAndAllChatRich( { ["text"] = szText, ["color"] = t_color, ["chatType"] = CL_CHAT_SAY } );
	
	elseif nType == SYS_TEAM then
		local szShowText			= "#A001"..szText;
		local nCanPingBiChatType= GetPingDaoChatType( { ["anim"] = "#A001" } );
		local t_color			= t_mainChatFrameTextColor[nCanPingBiChatType];
		UpdatePersonalAndAllChatRich( { ["text"] = szShowText, ["color"] = t_color, ["chatType"] = nCanPingBiChatType } );
		--[[
		local t_color = t_sysTextColor[SYS_TEAM];
		historyRich = getglobal( "ChatHistoryMsgMgrFrame_SystemRich" );
		historyRich:AddText( "#A001"..szText, t_color["r"], t_color["g"], t_color["b"] );
		--]]

	elseif nType == SYS_CLAN then
		--这里添加判断玩家是否加入了公会
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer:isInClan() then
			local szShowText			= "#A008"..szText;
			local nCanPingBiChatType	= GetPingDaoChatType( { ["anim"] = "#A008" } );
			local t_color				= t_mainChatFrameTextColor[nCanPingBiChatType];
			UpdatePersonalAndAllChatRich( { ["text"] = szShowText, ["color"] = t_color, ["chatType"] = nCanPingBiChatType } );
			historyRich = getglobal( "ChatHistoryMsgMgrFrame_ClanRich" );
			historyRich:AddText( szShowText, t_color["r"], t_color["g"], t_color["b"] );
		
			-- 此为绿色
			AddGameSysTip( szText );
		end
	elseif nType == SYS_ACTIVE then
		AddGameSysTip( szText );
	elseif nType == SYS_CURR then
		local szShowText			= "#A020"..szText;
		local t_Color = { ["r"] = 255,	["g"] = 255, ["b"] = 255	}
		if t_MainChatFrameControl:getPageType() == WORLD_PAGE_TYPE then
			Private_UpdateWorldChatRich(	{ ["text"] = szShowText, ["color"] = t_Color, ["chatType"] = CL_CHAT_WORLD } );
		elseif t_MainChatFrameControl:getPageType() == CLAN_PAGE_TYPE then
			Private_UpdateClanChatRich(	{ ["text"] = szShowText, ["color"] = t_Color, ["chatType"] = CL_CHAT_CLAN } );
		elseif t_MainChatFrameControl:getPageType() == TEAM_PAGE_TYPE then
			Private_UpdateTeamChatRich(	{ ["text"] = szShowText, ["color"] = t_Color, ["chatType"] = CL_CHAT_TEAM } );
		elseif t_MainChatFrameControl:getPageType() == NEAR_PAGE_TYPE then
			Private_UpdateNearChatRich(	{ ["text"] = szShowText, ["color"] = t_Color, ["chatType"] = CL_CHAT_SAY } );
		elseif t_MainChatFrameControl:getPageType() == PRIVATE_PAGE_TYPE then
			Private_UpdatePrivateChatRich(	{ ["text"] = szShowText, ["color"] = t_Color, ["chatType"] = CL_CHAT_PRIVATE } );
		end 
	elseif ( nType == SYS_SHOW ) then
		local szShowText = "#A020"..szText;
		--  现为炫耀频道
		ShowChatAllBugleMsgFrame( szText );
	end
	-- 若是 炫耀频道  在系统里添加 该频道内容
	if nType == SYS_SHOW then 
		local szShowText		= "#A006"..szText;
		local nCanPingBiChatType= GetPingDaoChatType( { ["anim"] = "#A006" } );
		local t_color			= t_mainChatFrameTextColor[nCanPingBiChatType];
		UpdatePersonalAndAllChatRich( { ["text"] = szShowText, ["color"] = t_color, ["chatType"] = nCanPingBiChatType } );

		local t_color = t_sysTextColor[SYS_TIP];
		historyRich = getglobal( "ChatHistoryMsgMgrFrame_SystemRich" );
		historyRich:AddText( "#A006"..szText, t_color["r"], t_color["g"], t_color["b"] );
	end
	if historyRich ~= nil then
		UpdateHistorySliderRange( { ["name"] = historyRich:GetName() } );
	end
end

t_MainChatFrameEvents["GE_UPDATE_SYS_BULLETIN"].func = 
function ()
	local nBulletinNum = ChatManager:getBulletinNum();
	if nBulletinNum > 0 and not GameBulletintTipsFrame:IsShown() then
		GameBulletintTipsFrame:Show();
	end
end

t_MainChatFrameEvents["GE_UPDATE_CHATFRAME_BY_CLIENT"].func = 
function ()
	local Msg		= UIMSG:getUIMsg();
	local nType		= Msg.ChatReq.Type;
	local sz2Client = Msg.ChatReq.Content;
	
	if not Msg.ChatReq.isVip then
		sz2Client = Private_ReplaceFunctionChar( { ["text"] = sz2Client } );
	end

	if string.find(sz2Client,"#@!",1,true) then
		while string.find(sz2Client,"#@!",1,true) do
			local nBeginPos		= string.find( sz2Client, "#@!" );
			local nEndPos		= string.find( sz2Client, "#", nBeginPos + 1, string.len(sz2Client));
			local context		= string.sub( sz2Client, nBeginPos + string.len( "#@!" ), nEndPos - 1 );
			UpdateMainChatFrame_ByClientChatMsg(context);
			sz2Client = string.sub( sz2Client, nEndPos + 1,string.len(sz2Client) );
		end
	else
		UpdateMainChatFrame_ByClientChatMsg(sz2Client);
	end
end

t_MainChatFrameEvents["GE_ENTER_PLAYERLOGIN"].func = 
function ()
	t_MainChatFrameControl:setPageType( { ["pageType"] = ZONGHE_PAGE_TYPE } );
	UpdateMainChatFrameCheckLabel();
	--Private_SetSlider_MainChatFrame_DefaultPos();
end

t_MainChatFrameEvents["GE_OTHERLAYER_CHG_NAME"].func = 
function ()
	local Msg				= UIMSG:getUIMsg();
	local changeNameData	= Msg.ChangeNameData;

	local szTargetText	= "[".."#L"..changeNameData.szOldName.."#n]";
	local szNewText		= "[".."#L"..changeNameData.szNewName.."#n]";
	for _, oneChatConfig in pairs( t_allChatTypePage ) do
		local rich = getglobal( oneChatConfig["rich"] );
		rich:ReplacePartialText( szTargetText, szNewText );
	end

	Private_Update_MainChatFrame_Slider();
end

function MainChatFrame_OnLoad()
	for event, _ in pairs( t_MainChatFrameEvents ) do
		this:RegisterEvent( event );
	end

	t_MainChatFrameEvents.__index = function ( tab, key )
									tab[key] = { func = function () end };
									return tab[key];
								end
	setmetatable( t_MainChatFrameEvents, t_MainChatFrameEvents );

	SetChatType( CL_CHAT_SAY );
	this:setUpdateTime( 0 );
end

function MainChatFrame_OnEvent()
	if  GameMgr:isInPlayCamerAnim() then
		return;
	end

	t_MainChatFrameEvents[arg1].func();
end

-- 聊天主面板显示
function ShowMainChatFrame_ScrollDownEffect()
	local slider	= getglobal( "MainChatFrame_ScrollBar" );
	if not slider:IsShown() then
		return;
	end

	local tex = getglobal( "MainChatFrame_ScrollDownBtnUVAnimationTex" );
	if tex:IsShown() then
		return;
	end

	tex:SetUVAnimation( 50, true );
end

function MainChatFrame_OnShow()
	LoadMsg();

	local tex = getglobal( "MainChatFrame_ChatBackBtnTex" );
	tex:SetBlendAlpha( 0 );

	local miniBtn	= getglobal( "MainChatFrame_MinimizeBtn" );
	miniBtn:SetBlendAlpha( 0 );

	local t_hideIcons = { "ChatControlFrame", };
	util.HideIcons( t_hideIcons );
	
	local t_showIcons = { "ChatInsertFrame" };
	util.ShowIcons( t_showIcons );
	
	UpdateMainChatFrameCheckLabel();
end

function MainChatFrame_OnHide()
	local t_hideIcons = { "ChatInsertFrame", "MainChatFrame_ScrollDownBtnUVAnimationTex", };
	util.HideIcons( t_hideIcons );
end

-- 设置GM指令开始执行参数
local nGMBeginTime	= 0;
local bGMBegin		= false;
-- GM 指令执行参数
local t_GMExcCmd	= {};
local nGMExcIndex	= 1;

function SetGMExcCmd( szGMCmd )
	if szGMCmd ~= nil and not string.find( szGMCmd,"//cmdpg", 1, true ) then
		table.insert( t_GMExcCmd, szGMCmd );
	end
	if szGMCmd ~= nil and  szGMCmd == "//cmdAutoQuestupdate" then
		table.insert( t_GMExcCmd, szGMCmd );
	end
end

function SetGMCMDBeginExc( nTime )
	nGMBeginTime	= nTime;
	bGMBegin		= true;
	nGMExcIndex		= 1;
end

function IsGMBegin()
	return bGMBegin;
end

function EndGM()
	bGMBegin = false;
end


-- 更新GM指令
function UpdateGMCmd()
	local bGMBegin = IsGMBegin();
	if not bGMBegin then
		return;
	end

	if os.clock() - nGMBeginTime < nGMExcIndex/5 then
		return;
	end

	if nGMExcIndex <= table.getn( t_GMExcCmd ) then
		ChatManager:requestChat( CL_CHAT_SAY, nil, t_GMExcCmd[nGMExcIndex] );
	elseif nGMExcIndex > table.getn( t_GMExcCmd ) then
		nGMBeginTime	= 0;
		nGMExcIndex		= 1;
		t_GMExcCmd		= {};
		EndGM();
	end
	nGMExcIndex = nGMExcIndex + 1;
end

function  MainChatFrame_OnUpdate()
	UpdateGMCmd();
	Private_UpdateChatBackAlpha();
	-- TODO: 这个要去掉
	GameBulletintTipsFrame_OnUpdate();
	UpdateMainChatFrame_PageBtn();
end


local t_pageBtnUVConfig = 
{
["GetNewMessage"] = 
{
--["normal"] = { x = 951, y = 661, width = 36, height = 25, }, ["highlight"] = { x = 988, y = 661, width = 36, height = 25, },

["normal"]		= { ["UV"] = { x = 951, y = 661, width = 36, height = 25, }, ["color"] = { r = 80, g = 40, b = 15 }, }, 
--["normal"]		= { ["UV"] = { x = 166, y = 468, width = 36, height = 25, }, ["color"] = { r = 255, g = 255, b = 190 }, }, 
--["highlight"]	= { x = 166, y = 468, width = 36, height = 25, },
},

["NotGetNewMessage"] = 
{
["normal"]		= { ["UV"] = { x = 166, y = 442, width = 36, height = 25, }, ["color"] = { r = 225, g = 195, b = 150 }, }, 
--["highlight"]	= { x = 166, y = 468, width = 36, height = 25, },
},

};

function Private_CancelOnePageBtnGetNewMessageState( data )	
	local nPageType = data["pageType"];
	if t_allChatTypePage[nPageType]["btn"] ~= "MainChatFrame_XinXiBtn" then
		if not t_MainChatFrameControl:isPageHaveNewMessage( { ["pageType"] = nPageType } ) then
			return;
		end

		t_MainChatFrameControl:delNewMessage( { ["pageType"] = nPageType } );

		local szBtnName = t_allChatTypePage[nPageType]["btn"];
		Private_SetOnePageBtnNotGetNewMessageState( { ["btn"] = szBtnName } );

		local tex = getglobal( szBtnName.."UVAnimationTex" );
		tex:Hide();
	end
end

function Private_SetOnePageBtnNotGetNewMessageState( data )
	if data["btn"] ~= "MainChatFrame_XinXiBtn" then
		local btn = getglobal( data["btn"] );

		local t_normalConfig	= t_pageBtnUVConfig["NotGetNewMessage"]["normal"];
		local t_normalUV		= t_normalConfig["UV"];
		btn:ChangeNormalTexture( t_normalUV["x"], t_normalUV["y"], t_normalUV["width"], t_normalUV["height"] );

		local t_normalColor	= t_normalConfig["color"];
		btn:SetNormalTextColor( t_normalColor["r"], t_normalColor["g"], t_normalColor["b"] );
	end
end

function Private_SetOnePageBtnGetNewMessageState( data )
	local btn = getglobal( data["btn"] );

	local t_normalConfig = t_pageBtnUVConfig["GetNewMessage"]["normal"];

	local t_normalUV		= t_normalConfig["UV"];
	btn:ChangeNormalTexture( t_normalUV["x"], t_normalUV["y"], t_normalUV["width"], t_normalUV["height"] );

	local t_normalColor	= t_normalConfig["color"];
	btn:SetNormalTextColor( t_normalColor["r"], t_normalColor["g"], t_normalColor["b"] );
end

-- CORE: 更新函数
-- 更新聊天页卡闪烁特效
local MAX_PAGE_NEW_MESSAE_ANIM_TIME = 10000;
function UpdateMainChatFrame_PageBtn()
	local t_MainChatFrameControl	= GetMainChatFrameControl();
	local t_allNewMessage			= t_MainChatFrameControl:getAllNewMessage();
	local nCurTime					= GameMgr:getTickTime();

	for pageType, onePageInfo in pairs( t_allNewMessage ) do
		if onePageInfo["isPlayAnim"] and nCurTime - onePageInfo["startTime"] >= MAX_PAGE_NEW_MESSAE_ANIM_TIME then
			t_MainChatFrameControl:stopPlayNewMessageAnim( { ["pageType"] = pageType } );
			local szBtnName = t_allChatTypePage[pageType]["btn"];
			local tex		= getglobal( szBtnName.."UVAnimationTex" );
			tex:Hide();
			Private_SetOnePageBtnGetNewMessageState( { ["btn"] = szBtnName } );
		end
	end
end

-- 用服务器发来的数据来更新聊天显示内容
function UpdateMainChatFrame_ByServerChatMsg( nType,szMsg,szSpeaker )
	local szText	= "";
	local t_color	= t_mainChatFrameTextColor[CL_CHAT_SAY];
	local nChatType	= GetChatType();
	local historyRich;
	
	local oldName = szSpeaker
	szSpeaker = GetPlayerName( szSpeaker )

	if ( nType == CL_CHAT_SAY ) then
		szText	= "#A003".."[".."#L"..szSpeaker.."@@@"..oldName .."#n]".."说:"..szMsg;
		t_color	= t_mainChatFrameTextColor[nType];
		
		historyRich = getglobal( "ChatHistoryMsgMgrFrame_NearRich" );
		historyRich:AddText( "#A003".."[".."#L"..szSpeaker.."@@@"..oldName .."#n]".."说:"..szMsg, t_color["r"], t_color["g"], t_color["b"] );

	elseif ( nType == CL_CHAT_TEAM ) then
		szText	= "#A001".."[".."#L"..szSpeaker.."@@@"..oldName .."#n]".."说:"..szMsg;
		t_color	= t_mainChatFrameTextColor[nType];
		
		historyRich = getglobal( "ChatHistoryMsgMgrFrame_TeamRich" );
		historyRich:AddText( "#A001".."[".."#L"..szSpeaker.."@@@"..oldName .."#n]".."说:"..szMsg, t_color["r"], t_color["g"], t_color["b"] );

	elseif ( nType == CL_CHAT_PRIVATE ) then
		szText	= "#A010".."[".."#L"..szSpeaker.."@@@"..oldName .."#n]".."对你说:"..szMsg;
		t_color	= t_mainChatFrameTextColor[nType];
		
		historyRich = getglobal( "ChatHistoryMsgMgrFrame_PrivateRich" );
		historyRich:AddText( "#A010".."[".."#L"..szSpeaker.."@@@"..oldName .."#n]".."对你说:"..szMsg, t_color["r"], t_color["g"], t_color["b"] );

	elseif ( nType == CL_CHAT_CLAN ) then
		szText	= "#A008".."[".."#L"..szSpeaker.."@@@"..oldName .."#n]".."说:"..szMsg;
		t_color	= t_mainChatFrameTextColor[nType];
		
		historyRich = getglobal( "ChatHistoryMsgMgrFrame_ClanRich" );
		historyRich:AddText( "#A008".."[".."#L"..szSpeaker.."@@@"..oldName .."#n]".."说:"..szMsg, t_color["r"], t_color["g"], t_color["b"] );

	elseif ( nType == CL_CHAT_CLAN_GROUP ) then
		szText	= "#A019".."[".."#L"..szSpeaker.."@@@"..oldName .."#n]".."说:"..szMsg;
		t_color	= t_mainChatFrameTextColor[nType];
		
		historyRich = getglobal( "ChatHistoryMsgMgrFrame_ClanGroupRich" );
		historyRich:AddText( "#A019".."[".."#L"..szSpeaker.."@@@"..oldName .."#n]".."说:"..szMsg, t_color["r"], t_color["g"], t_color["b"] );

	elseif ( nType == CL_CHAT_WORLD ) then
		szText	= "#A002".."[".."#L"..szSpeaker.."@@@"..oldName .."#n]".."说:"..szMsg;
		t_color	= t_mainChatFrameTextColor[nType];

		historyRich = getglobal( "ChatHistoryMsgMgrFrame_WorldRich" );
		historyRich:AddText( szText, t_color["r"], t_color["g"], t_color["b"] );

	elseif ( nType == CL_CHAT_ITEM ) then
		local tmpMsg, storeId = checkStoreLink( szMsg );
		
		szText	= "#A012".."[".."#L"..szSpeaker.."@@@"..oldName .."#n]".."说:"..tmpMsg;
		t_color	= t_mainChatFrameTextColor[nType];
		-- 在聊天主面板上面显示收费信息
		ChatShowBugleMsgFrameShow( szText,nType);
		
		if storeId > 0 then
			UpdateOnlineStoreAd( string.gsub( szText, "#A012", "" ), storeId );
			return;
		else
			historyRich = getglobal( "ChatHistoryMsgMgrFrame_BugleRich" );
			historyRich:AddText( szText, t_color["r"], t_color["g"], t_color["b"] );
		end
	elseif ( nType == CL_CHAT_SPAN ) then 
		local Msg = UIMSG:getUIMsg();
		local worldName = LoginMgr:getWorldNameById( Msg.ChatMsg.WorldID );
		szText	= "#A021".."["..worldName .."]" .. szSpeaker .."说:"..szMsg;
		t_color	= t_mainChatFrameTextColor[nType];
		-- 在聊天主面板上面显示收费信息
		ChatShowBugleMsgFrameShow( szText, nType );

		historyRich = getglobal( "ChatHistoryMsgMgrFrame_BugleRich" );
		historyRich:AddText( szText, t_color["r"], t_color["g"], t_color["b"] );
		UpdateHistorySliderRange( { ["name"] = historyRich:GetName() } );
		return;
	elseif nType == CL_CHAT_BATTLE then
		szText	= "#A007".."[".."#L"..szSpeaker.."@@@"..oldName .."#n]".."说:"..szMsg;
		t_color	= t_mainChatFrameTextColor[nType];

		historyRich = getglobal( "ChatHistoryMsgMgrFrame_NearRich" );
		historyRich:AddText("#A007".."[".."#L"..szSpeaker.."@@@"..oldName .."#n]".."说:"..szMsg, t_color["r"], t_color["g"], t_color["b"] );
	elseif nType == CL_CHAT_SPAN_MAP_WORLD then
		szText	= "#A22".."[".."#L"..szSpeaker.."@@@"..oldName .."#n]".."说:"..szMsg;
		t_color	= t_mainChatFrameTextColor[nType];

		historyRich = getglobal( "ChatHistoryMsgMgrFrame_NearRich" );
		historyRich:AddText("#A022".."[".."#L"..szSpeaker.."@@@"..oldName .."#n]".."说:"..szMsg, t_color["r"], t_color["g"], t_color["b"] );
	elseif nType == CL_CHAT_SPAN_MAP then
		szText	= "#A023".."[".."#L"..szSpeaker.."@@@"..oldName .."#n]".."说:"..szMsg;
		t_color	= t_mainChatFrameTextColor[nType];

		historyRich = getglobal( "ChatHistoryMsgMgrFrame_NearRich" );
		historyRich:AddText("#A023".."[".."#L"..szSpeaker.."@@@"..oldName .."#n]".."说:"..szMsg, t_color["r"], t_color["g"], t_color["b"] );
	end

	UpdatePersonalAndAllChatRich( { ["text"] = szText, ["color"] = t_color, ["chatType"] = nType } );
	if historyRich ~= nil then
		UpdateHistorySliderRange( { ["name"] = historyRich:GetName() } );
	end
end

-- 用客户端本地的数据来更新聊天显示内容
function UpdateMainChatFrame_ByClientChatMsg( szMsg )
	local szText	= "";
	local t_color	= t_mainChatFrameTextColor[CL_CHAT_SAY];
	local nChatType	= GetChatType();
	
	if nChatType == CL_CHAT_ITEM then
		return;
	end
	
	local historyRich;
	if ( nChatType == CL_CHAT_SAY ) then
		szText	= "#A003".."你说:"..szMsg;
		t_color	= t_mainChatFrameTextColor[nChatType];
		
		historyRich = getglobal( "ChatHistoryMsgMgrFrame_NearRich" );
		historyRich:AddText( szText, t_color["r"], t_color["g"], t_color["b"] );

	elseif ( nChatType == CL_CHAT_TEAM ) then		
		szText	= "#A001".."你说:"..szMsg;
		t_color	= t_mainChatFrameTextColor[nChatType];
		
		historyRich = getglobal( "ChatHistoryMsgMgrFrame_TeamRich" );
		historyRich:AddText( szText, t_color["r"], t_color["g"], t_color["b"] );

	elseif ( nChatType == CL_CHAT_PRIVATE ) and szMsg ~= "" then
		local t_ChatInsertFrameControl	= GetChatInsertFrameControl();
		local szPrivatePlayerName		= t_ChatInsertFrameControl:getPrivatePlayerName();
		szText	= "#A010".."你对[".."#L"..szPrivatePlayerName.."#n]说:"..szMsg;
		t_color	= t_mainChatFrameTextColor[nChatType];
		
		historyRich = getglobal( "ChatHistoryMsgMgrFrame_PrivateRich" );
		historyRich:AddText( szText, t_color["r"], t_color["g"], t_color["b"] );

	elseif ( nChatType == CL_CHAT_CLAN ) then
		szText	= "#A008".."你说:"..szMsg;
		t_color	= t_mainChatFrameTextColor[nChatType];

		historyRich = getglobal( "ChatHistoryMsgMgrFrame_ClanRich" );
		historyRich:AddText( szText, t_color["r"], t_color["g"], t_color["b"] );

	elseif ( nChatType == CL_CHAT_CLAN_GROUP ) then
		szText	= "#A019".."你说:"..szMsg;
		t_color	= t_mainChatFrameTextColor[nChatType];

		historyRich = getglobal( "ChatHistoryMsgMgrFrame_ClanGroupRich" );
		historyRich:AddText( szText, t_color["r"], t_color["g"], t_color["b"] );

	elseif ( nChatType == CL_CHAT_WORLD ) then
		szText	= "#A002".."你说:"..szMsg;
		t_color	= t_mainChatFrameTextColor[nChatType];
		
		historyRich = getglobal( "ChatHistoryMsgMgrFrame_WorldRich" );
		historyRich:AddText( szText, t_color["r"], t_color["g"], t_color["b"] );

	elseif nChatType == CL_CHAT_BATTLE then
		szText	= "#A007".."你说:"..szMsg;
		t_color	= t_mainChatFrameTextColor[nChatType];
		
		historyRich = getglobal( "ChatHistoryMsgMgrFrame_NearRich" );
		historyRich:AddText( szText, t_color["r"], t_color["g"], t_color["b"] );

	elseif nChatType == CL_CHAT_SPAN_MAP_WORLD then
		szText	= "#A022".."你说:"..szMsg;
		t_color	= t_mainChatFrameTextColor[nChatType];
		
		historyRich = getglobal( "ChatHistoryMsgMgrFrame_NearRich" );
		historyRich:AddText( szText, t_color["r"], t_color["g"], t_color["b"] );

	elseif nChatType == CL_CHAT_SPAN_MAP then
		szText	= "#A023".."你说:"..szMsg;
		t_color	= t_mainChatFrameTextColor[nChatType];
		
		historyRich = getglobal( "ChatHistoryMsgMgrFrame_NearRich" );
		historyRich:AddText( szText, t_color["r"], t_color["g"], t_color["b"] );

	end
	
	UpdatePersonalAndAllChatRich( { ["text"] = szText, ["color"] = t_color, ["chatType"] = nChatType } );

	if historyRich ~= nil then
		UpdateHistorySliderRange( { ["name"] = historyRich:GetName() } );
	end
end

-- 更新综合信息面板内容和个人信息面板内容
function UpdatePersonalAndAllChatRich( data )
	Private_UpdateZongHeChatRich( data );
	Private_UpdateWorldChatRich( data );
	Private_UpdateClanChatRich(	data );
	Private_UpdateTeamChatRich(	data );
	Private_UpdateNearChatRich(	data );
	Private_UpdatePrivateChatRich( data );
	Private_UpdateXinXiChatRich( data );
end

-- 更据选择的聊天页卡莱切换显示界面
local t_chatLableBtnConfig = 
{};

function UpdateMainChatFrameCheckLabel()
	local nCurPageType		= t_MainChatFrameControl:getPageType();
	local t_onePageConfig	= t_allChatTypePage[nCurPageType];
	if t_onePageConfig == nil then
		assert( "脚本有地方写错" );
		return;
	end

	Private_ResetChatTypeLableBtn();
	Private_HideAllChatRich();

	local btn = getglobal( t_onePageConfig["btn"] );
	btn:Checked();

	local rich = getglobal( t_onePageConfig["rich"] );
	rich:Show();
	Private_SetSlider_MainChatFrame_DefaultPos();
end

-- CORE: 更新函数
function Private_GetCurShowChatTypeRich()
	local nCurShowPageType	= t_MainChatFrameControl:getPageType();
	local rich				= getglobal( t_allChatTypePage[nCurShowPageType]["rich"] );
	return rich;
end

-- 重置聊天类型按钮
function Private_ResetChatTypeLableBtn()
	local t_btns = {	"MainChatFrame_WorldBtn",	"MainChatFrame_ZongHeBtn",	"MainChatFrame_ClanBtn", 
						"MainChatFrame_TeamBtn",	"MainChatFrame_NearBtn",	"MainChatFrame_PrivateBtn",
						"MainChatFrame_XinXiBtn" };
	util.DischeckBtns( t_btns );
end

-- 
function Private_HideAllChatRich()
	for _, oneConfig in pairs( t_allChatTypePage ) do
		local rich = getglobal( oneConfig["rich"] );
		rich:Hide();
	end
end

-- 更新综合聊天面板内容
function Private_IsChatTypeSupportInZongHeChatRich( data )
	local nCanPingBiChatType	= data["chatType"];
	if nCanPingBiChatType == nil then
		return true;
	end

	if ChatManager:isZongHeChannelPingBi( nCanPingBiChatType ) then
		return false;
	end

	return true;
end

function Private_UpdateZongHeChatRich( data )
	if not Private_IsChatTypeSupportInZongHeChatRich( data ) then
		return;
	end

	local text		= data["text"];
	local t_color	= data["color"];
	local rich		= getglobal( "MainChatFrame_ZongHeRich" );
	local dispPos	= rich:GetDispPos();
	local endDisPos = rich:GetEndDispPos();

	rich:AddText( text, t_color["r"], t_color["g"], t_color["b"] );
	
	if not rich:IsShown() then
		return;
	end

	if dispPos == endDisPos then
		Private_SetSlider_MainChatFrame_MaxPos();
	else
		Private_Update_MainChatFrame_Slider();
		ShowMainChatFrame_ScrollDownEffect();
	end
end

-- 更新世界聊天面板内容
function Private_IsChatTypeSupportInWorldChatRich( data )
	local nCanPingBiChatType = data["chatType"];
	if nCanPingBiChatType == nil then
		return true;
	end

	if ChatManager:isWorldChannelPingBi( nCanPingBiChatType ) then
		return false;
	end
	
	return true;
end

function Private_IsChatTypePingBi( data )
	local nPageType	= data["pageType"];
	local nChatType	= data["chatType"];

	if nPageType == ZONGHE_PAGE_TYPE then
		return ChatManager:isZongHeChannelPingBi( nChatType );
	end

	if nPageType == WORLD_PAGE_TYPE then
		return ChatManager:isWorldChannelPingBi( nChatType );
	end

	if nPageType == CLAN_PAGE_TYPE then
		return ChatManager:isClanChannelPingBi( nChatType );
	end

	if nPageType == TEAM_PAGE_TYPE then
		return ChatManager:isTeamChannelPingBi( nChatType );
	end

	if nPageType == NEAR_PAGE_TYPE then
		return ChatManager:isNearChannelPingBi( nChatType );
	end

	if nPageType == PRIVATE_PAGE_TYPE then
		return ChatManager:isPrivateChannelPingBi( nChatType );
	end

	return true;
end

function Private_WarnChatChannelGetNewMessage( data )
	local t_MainChatFrameControl	= GetMainChatFrameControl();
	local nCurPageType				= t_MainChatFrameControl:getPageType();
	local nDestPageType				= data["pageType"];
	if nCurPageType == nDestPageType then
		return;
	end

	local nFitDestPageType = t_MainChatFrameControl:convertChatTypeToPageType( { ["chatType"] = data["chatType"] } );
	if nFitDestPageType ~= nDestPageType then
		return;
	end

	if Private_IsChatTypePingBi( { ["pageType"] = nDestPageType, ["chatType"] = CL_CHAT_WARN_NEW_MESSAGE } ) then
		return;
	end
	
	if t_MainChatFrameControl:isPageHaveNewMessage( { ["pageType"] = nDestPageType } ) then
		return;
	end

	local tex = getglobal( t_allChatTypePage[nDestPageType]["btn"].."UVAnimationTex" );
	tex:SetUVAnimation( 80, true);
	tex:Show();

	t_MainChatFrameControl:addNewMessage( { ["pageType"] = nDestPageType } );
end

function Private_UpdateWorldChatRich( data )
	if not Private_IsChatTypeSupportInWorldChatRich( data ) then
		return;
	end

	local nCanPingBiChatType	= data["chatType"];
	local t_color				= data["color"];
	local text					= data["text"];
	local rich					= getglobal( "MainChatFrame_ChatWorldRich" );
	local dispPos				= rich:GetDispPos();
	local endDisPos				= rich:GetEndDispPos();

	rich:AddText( text, t_color["r"], t_color["g"], t_color["b"] );
	
	if not rich:IsShown() then
		Private_WarnChatChannelGetNewMessage( { ["pageType"] = WORLD_PAGE_TYPE, ["chatType"] = data["chatType"] } );
		return;
	end

	if dispPos == endDisPos then
		Private_SetSlider_MainChatFrame_MaxPos();
	else
		Private_Update_MainChatFrame_Slider();
		ShowMainChatFrame_ScrollDownEffect();
	end
end

-- 更新公会天面板内容
function Private_IsChatTypeSupportInClanChatRich( data )
	local nCanPingBiChatType	= data["chatType"];
	if nCanPingBiChatType == nil then
		return true;
	end
	
	if ChatManager:isClanChannelPingBi( nCanPingBiChatType ) then
		return false;
	end

	return true;
end

function Private_UpdateClanChatRich( data )
	if not Private_IsChatTypeSupportInClanChatRich( data ) then
		return;
	end

	local t_color	= data["color"];
	local text		= data["text"];
	local rich		= getglobal( "MainChatFrame_ChatClanRich" );
	local dispPos	= rich:GetDispPos();
	local endDisPos = rich:GetEndDispPos();

	rich:AddText( text, t_color["r"], t_color["g"], t_color["b"] );
	
	if not rich:IsShown() then
		Private_WarnChatChannelGetNewMessage( { ["pageType"] = CLAN_PAGE_TYPE, ["chatType"] = data["chatType"] } );
		return;
	end

	if dispPos == endDisPos then
		Private_SetSlider_MainChatFrame_MaxPos();
	else
		Private_Update_MainChatFrame_Slider();
		ShowMainChatFrame_ScrollDownEffect();
	end
end

-- 更新队伍聊天面板内容
function Private_IsChatTypeSupportInTeamChatRich( data )
	local nCanPingBiChatType	= data["chatType"];
	if nCanPingBiChatType == nil then
		return true;
	end

	if ChatManager:isTeamChannelPingBi( nCanPingBiChatType ) then
		return false;
	end

	return true;
end

function Private_UpdateTeamChatRich( data )	
	if not Private_IsChatTypeSupportInTeamChatRich( data ) then
		return;
	end
	
	local text		= data["text"];
	local t_color	= data["color"];
	local rich		= getglobal( "MainChatFrame_ChatTeamRich" );
	local dispPos	= rich:GetDispPos();
	local endDisPos = rich:GetEndDispPos();

	rich:AddText( text, t_color["r"], t_color["g"], t_color["b"] );
	
	if not rich:IsShown() then
		Private_WarnChatChannelGetNewMessage( { ["pageType"] = TEAM_PAGE_TYPE, ["chatType"] = data["chatType"] } );
		return;
	end

	if dispPos == endDisPos then
		Private_SetSlider_MainChatFrame_MaxPos();
	else
		Private_Update_MainChatFrame_Slider();
		ShowMainChatFrame_ScrollDownEffect();
	end
end

-- 更新附近面板内容
function Private_IsChatTypeSupportInNearChatRich( data )
	local nCanPingBiChatType	= data["chatType"];
	if nCanPingBiChatType == nil then
		return true;
	end

	if ChatManager:isNearChannelPingBi( nCanPingBiChatType ) then
		return false;
	end

	return true;
end

function Private_UpdateNearChatRich( data )	
	if not Private_IsChatTypeSupportInNearChatRich( data ) then
		return;
	end
	
	local text		= data["text"];
	local t_color	= data["color"];
	local rich		= getglobal( "MainChatFrame_ChatNearRich" );
	local dispPos	= rich:GetDispPos();
	local endDisPos = rich:GetEndDispPos();

	rich:AddText( text, t_color["r"], t_color["g"], t_color["b"] );
	
	if not rich:IsShown() then
		Private_WarnChatChannelGetNewMessage( { ["pageType"] = NEAR_PAGE_TYPE, ["chatType"] = data["chatType"] } );
		return;
	end

	if dispPos == endDisPos then
		Private_SetSlider_MainChatFrame_MaxPos();
	else
		Private_Update_MainChatFrame_Slider();
		ShowMainChatFrame_ScrollDownEffect();
	end
end

-- 更新私聊面板内容
function Private_IsChatTypeSupportInPrivateChatRich( data )
	local nCanPingBiChatType = data["chatType"];
	if nCanPingBiChatType == nil then
		return true;
	end

	if ChatManager:isPrivateChannelPingBi( nCanPingBiChatType ) then
		return false;
	end

	return true;
end

function Private_UpdatePrivateChatRich( data )	
	if not Private_IsChatTypeSupportInPrivateChatRich( data ) then
		return;
	end
	
	local text		= data["text"];
	local t_color	= data["color"];
	local rich		= getglobal( "MainChatFrame_ChatPrivateRich" );
	local dispPos	= rich:GetDispPos();
	local endDisPos = rich:GetEndDispPos();

	rich:AddText( text, t_color["r"], t_color["g"], t_color["b"] );
	
	if not rich:IsShown() then
		Private_WarnChatChannelGetNewMessage( { ["pageType"] = PRIVATE_PAGE_TYPE, ["chatType"] = data["chatType"] } );
		return;
	end

	if dispPos == endDisPos then
		Private_SetSlider_MainChatFrame_MaxPos();
	else
		Private_Update_MainChatFrame_Slider();
		ShowMainChatFrame_ScrollDownEffect();
	end
end

-- 更新聊天信息子面板
function Private_IsChatTypeSupportInXinXiChatRich( data )
	local nCanPingBiChatType = data["chatType"];
	if nCanPingBiChatType == nil then
		return false;
	end

	if nCanPingBiChatType ~= CL_CHAT_XIN_XI_MSG then
		return false;
	end

	return true;
end

function Private_UpdateXinXiChatRich( data )
	if not Private_IsChatTypeSupportInXinXiChatRich( data ) then
		return;
	end

	local text		= data["text"];
	local t_color	= data["color"];
	local rich		= getglobal( "MainChatFrame_ChatXinXiRich" );
	local dispPos	= rich:GetDispPos();
	local endDisPos = rich:GetEndDispPos();

	rich:AddText( text, t_color["r"], t_color["g"], t_color["b"] );
	
	if not rich:IsShown() then
		return;
	end

	if dispPos == endDisPos then
		Private_SetSlider_MainChatFrame_MaxPos();
	else
		Private_Update_MainChatFrame_Slider();
		ShowMainChatFrame_ScrollDownEffect();
	end
end

function Private_IsOpaqueAlpha()
	return isPointInFrame( "MainChatFrame" ) or GetFocusFrameName() == "CSChatInsert";
end

-- TODO: 重构
local fAlpha				 = 0;
function GetChatBackAlpha()
	return fAlpha;
end

function SetChatBackAlpha( fNewAlpha )
	fAlpha = fNewAlpha;
end

-- 更新聊天面板的底板
local nChgAlphaUnit = 0.04;
function Private_UpdateChatBackAlpha()
	-- 当游戏焦点聚焦在聊天输入框的时候
	local fAlpha	= GetChatBackAlpha();
	local tex		= getglobal( "MainChatFrame_ChatBackBtnTex" );
	local miniBtn	= getglobal( "MainChatFrame_MinimizeBtn" );
	if Private_IsOpaqueAlpha() then	
		fAlpha = fAlpha + nChgAlphaUnit;
		if fAlpha > 1 then
			fAlpha = 1;
		end
		tex:SetBlendAlpha(fAlpha);
		miniBtn:SetBlendAlpha(fAlpha);
	else
		fAlpha = fAlpha - nChgAlphaUnit;
		if fAlpha < 0 then
			fAlpha = 0;
		end
		tex:SetBlendAlpha(fAlpha);
		miniBtn:SetBlendAlpha(fAlpha);
		
	end

	SetChatBackAlpha( fAlpha );
end

-- 清除聊天信息按钮
function MainChatFrame_ScrollClearBtn_OnClick()
	local rich = Private_GetCurShowChatTypeRich();
	rich:Clear();
	rich:clearHistory();
end

function  MainChatFrame_ScrollClearBtn_OnEnter()
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "清除当前所有聊天信息", frame = this:GetParent(),
						button = this:GetName() } );
end

-- 综合标签按钮
function MainChatFrame_ZongHeBtn_OnClick()
	t_MainChatFrameControl:setPageType( { ["pageType"] = ZONGHE_PAGE_TYPE } );
	UpdateMainChatFrameCheckLabel();

	local t_hideIcons = { "MainChatFrame_ScrollDownBtnUVAnimationTex", };
	util.HideIcons( t_hideIcons );
end

-- 世界标签按钮
function MainChatFrame_WorldBtn_OnClick()
	t_MainChatFrameControl:setPageType( { ["pageType"] = WORLD_PAGE_TYPE } );
	UpdateMainChatFrameCheckLabel();

	local t_hideIcons = { "MainChatFrame_ScrollDownBtnUVAnimationTex", };
	util.HideIcons( t_hideIcons );
	Private_SynchronizeInput( { ["chatType"] = CL_CHAT_WORLD } );
	Private_CancelOnePageBtnGetNewMessageState( { ["pageType"] = WORLD_PAGE_TYPE } );
end

-- 公会标签按钮
function MainChatFrame_ClanBtn_OnClick()
	t_MainChatFrameControl:setPageType( { ["pageType"] = CLAN_PAGE_TYPE } );
	UpdateMainChatFrameCheckLabel();

	local t_hideIcons = { "MainChatFrame_ScrollDownBtnUVAnimationTex", };
	util.HideIcons( t_hideIcons );
	Private_SynchronizeInput( { ["chatType"] = CL_CHAT_CLAN } );
	Private_CancelOnePageBtnGetNewMessageState( { ["pageType"] = CLAN_PAGE_TYPE } );
end

-- 队伍标签按钮
function MainChatFrame_TeamBtn_OnClick()
	t_MainChatFrameControl:setPageType( { ["pageType"] = TEAM_PAGE_TYPE } );
	UpdateMainChatFrameCheckLabel();

	local t_hideIcons = { "MainChatFrame_ScrollDownBtnUVAnimationTex", };
	util.HideIcons( t_hideIcons );
	Private_SynchronizeInput( { ["chatType"] = CL_CHAT_TEAM } );
	Private_CancelOnePageBtnGetNewMessageState( { ["pageType"] = TEAM_PAGE_TYPE } );
end

-- 附近标签按钮
function MainChatFrame_NearBtn_OnClick()
	t_MainChatFrameControl:setPageType( { ["pageType"] = NEAR_PAGE_TYPE } );
	UpdateMainChatFrameCheckLabel();

	local t_hideIcons = { "MainChatFrame_ScrollDownBtnUVAnimationTex", };
	util.HideIcons( t_hideIcons );
	Private_SynchronizeInput( { ["chatType"] = CL_CHAT_SAY } );
	Private_CancelOnePageBtnGetNewMessageState( { ["pageType"] = NEAR_PAGE_TYPE } );
end

-- 私聊标签按钮
function MainChatFrame_PrivateBtn_OnClick()
	t_MainChatFrameControl:setPageType( { ["pageType"] = PRIVATE_PAGE_TYPE } );
	UpdateMainChatFrameCheckLabel();

	local t_hideIcons = { "MainChatFrame_ScrollDownBtnUVAnimationTex", };
	util.HideIcons( t_hideIcons );
	Private_SynchronizeInput( { ["chatType"] = CL_CHAT_PRIVATE } );
	Private_CancelOnePageBtnGetNewMessageState( { ["pageType"] = PRIVATE_PAGE_TYPE } );
end

-- 信息标签按钮
function MainChatFrame_XinXiBtn_OnClick()
	t_MainChatFrameControl:setPageType( { ["pageType"] = XINXI_PAGE_TYPE } );
	UpdateMainChatFrameCheckLabel();

	local t_hideIcons = { "MainChatFrame_ScrollDownBtnUVAnimationTex", };
	util.HideIcons( t_hideIcons );
end

function MainChatFrame_XinXiBtn_OnEnter()
	SetGameTooltips(this:GetName(), "点击切换到信息频道" );
end

-- 笑脸表情按钮
function MainChatFrame_FaceBtn_OnClick()
	local frame = getglobal("MainChatFaceFrame");
	if frame:IsShown() then
		frame:Hide();
	else
		frame:Show();
		SetFocusFrame("CSChatInsert");
	end
end

function MainChatFrame_FaceBtn_OnEnter()
	SetGameTooltips(this:GetName(), "表情" );
end

-- 动作按钮
function MainChatFrame_ActBtn_OnClick()
	local frame = getglobal("ChatActionFrame");
	if frame:IsShown() then
		frame:Hide();
	else
		frame:Show();
		frame:SetPoint( "BottomLeft", this:GetName(), "TopRight", 0, 0 );
	end
end

function MainChatFrame_ActBtn_OnEnter()
	SetGameTooltips(this:GetName(), "动作" );
end

-- 快捷发言按钮
function MainChatFrame_ShortcutSendMsgBtn_OnClick()	
	local frame = getglobal("ChatShortcutSendMsgFrame");
	if frame:IsShown() then
		frame:Hide();
	else
		frame:Show();
	end
end

function MainChatFrame_ShortcutSendMsgBtn_OnEnter()
	SetGameTooltips(this:GetName(), "打开快捷发言界面" );
end

-- 滚动至最后一行的按钮
function MainChatFrame_ScrollLastBtn_OnClick()
	local rich = Private_GetCurShowChatTypeRich();
	rich:ScrollEnd();
end

-- 向下滚动一行按钮
function MainChatFrame_ScrollDownBtn_OnClick()
	local t_hideIcons = { "MainChatFrame_ScrollDownBtnUVAnimationTex", };
	util.HideIcons( t_hideIcons );

	local slider	= getglobal( "MainChatFrame_ScrollBar" );
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

function MainChatFrame_ScrollDownBtn_OnMouseDown()
	t_MainChatFrameControl["lastClickScrollBtnTime"] = GameMgr:getTickTime();
end

function MainChatFrame_ScrollDownBtn_OnUpdate()
	local nLastClikTime = t_MainChatFrameControl["lastClickScrollBtnTime"];
	local nCurTime = GameMgr:getTickTime();
	if this:GetPushedState() and ( nCurTime - nLastClikTime ) > BTN_HOLD_TIME*1000 then
		MainChatFrame_ScrollDownBtn_OnClick();
		t_MainChatFrameControl["lastClickScrollBtnTime"] = nCurTime;
	end
end

-- 向上滚动一行的按钮
function MainChatFrame_ScrollUpBtn_OnClick()
		local slider = getglobal( "MainChatFrame_ScrollBar" );
	if not slider:IsShown() then
		return;
	end

	local nValue = slider:GetValue() - slider:GetValueStep();
	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end

function MainChatFrame_ScrollUpBtn_OnMouseDown()
	t_MainChatFrameControl["lastClickScrollBtnTime"] = GameMgr:getTickTime();
end

function MainChatFrame_ScrollUpBtn_OnUpdate()
	local nLastClikTime = t_MainChatFrameControl["lastClickScrollBtnTime"];
	local nCurTime = GameMgr:getTickTime();
	if this:GetPushedState() and ( nCurTime - nLastClikTime ) > BTN_HOLD_TIME * 1000 then
		MainChatFrame_ScrollUpBtn_OnClick();
		t_MainChatFrameControl["lastClickScrollBtnTime"] = nCurTime;
	end
end

-- 滚动条滚动
function MainChatFrame_ScrollBar_OnValueChanged()
	local slider	 = getglobal( "MainChatFrame_ScrollBar" );
	local nValue	 = slider:GetValue();
	local nMaxValue	 = slider:GetMaxValue();
	local rich		 = Private_GetCurShowChatTypeRich();
	if nValue <= nMaxValue then
		util.UpdateRich( rich:GetName(), nValue );
	end

	if nValue == nMaxValue then
		local t_hideIcons = { "MainChatFrame_ScrollDownBtnUVAnimationTex", };
		util.HideIcons( t_hideIcons );
	end
end

-- 同步聊天输入框和聊天面板页卡
function Private_SynchronizeInput( data )
	local nLastChatType	= GetChatType();
	local nNewChatType	= data["chatType"];
	if nLastChatType == nNewChatType then
		return;
	end
	
	local t_ChatInsertFrameControl = GetChatInsertFrameControl();
	SetChatType( nNewChatType );
	if nNewChatType == CL_CHAT_ITEM then
		if IsHaveLaBaItem() then
			local laBaItem = GetContainerOneLaBaItem();
			SetLaBaItem( { grid = laBaItem:getGridIdx(), list = laBaItem:getListType() } );
		else
			SetLaBaItem( { grid = -1, list = -1 } );
		end
	elseif nNewChatType == CL_CHAT_PRIVATE then
		t_ChatInsertFrameControl:setPrivatePlayerName( { ["name"] = "" } );
	end

	ChatInsertFrame_OnShow();
	if nLastChatType == CL_CHAT_PRIVATE and nLastChatType ~= CL_CHAT_PRIVATE then
		CSChatInsert:SetText( CSChatInsert:GetText() );
	end
end

-- 将聊天滚动条置为初始位置
function Private_SetSlider_MainChatFrame_DefaultPos()
	local rich		= Private_GetCurShowChatTypeRich();
	local slider	= getglobal("MainChatFrame_ScrollBar");
	SetScrollBar( slider:GetName(), rich:GetAccurateViewLines(), rich:GetTextLines() );	
	slider:SetValue( slider:GetMaxValue() );
	util.UpdateRich( rich:GetName(), slider:GetValue() );
end

-- 更新聊天滚动条位置
function Private_Update_MainChatFrame_Slider()
	local rich		= Private_GetCurShowChatTypeRich();
	local slider	= getglobal("MainChatFrame_ScrollBar");
	SetScrollBar( slider:GetName(), rich:GetAccurateViewLines(), rich:GetTextLines() );
	--util.UpdateRich( rich:GetName(), slider:GetValue() );
end

-- 将聊天滚动条置为最大位置
function Private_SetSlider_MainChatFrame_MaxPos()
	local rich		= Private_GetCurShowChatTypeRich();
	local slider	= getglobal("MainChatFrame_ScrollBar");
	SetScrollBar( slider:GetName(), rich:GetAccurateViewLines(), rich:GetTextLines() );	
	slider:SetValue( slider:GetMaxValue() );
	util.UpdateRich( rich:GetName(), slider:GetValue() );
end

-- 屏蔽按钮
function MainChatFrame_PingBiBtn_OnClick()	
	local frame = getglobal( "ChatPingBiSettingFrame" );
	if frame:IsShown() then
		frame:Hide();
	else
		frame:Show();
	end
end

function MainChatFrame_PingBiBtn_OnEnter()
	local szText = "设置频道信息";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } )
end

-- 向右加长聊天面板按钮
function MainChatFrame_HorizonalLongerBtn_OnClick()
	local chatBackBtn = getglobal( "MainChatFrame_ChatBackBtn" );
	if chatBackBtn:GetWidth() < ( CHAT_BACK_DEFAULT_WIDTH + CHAT_INCR_UNIT * 3 ) then
		for _, oneConfig in pairs( t_allChatTypePage ) do
			local rich = getglobal( oneConfig["rich"] );
			rich:SetWidth( rich:GetWidth() + CHAT_INCR_UNIT );
		end

		chatBackBtn:SetWidth( chatBackBtn:GetWidth() + CHAT_INCR_UNIT + 7 );
		MainChatFrame:SetWidth( MainChatFrame:GetWidth() + CHAT_INCR_UNIT + 7 );
	else
		for _, oneConfig in pairs( t_allChatTypePage ) do
			local rich = getglobal( oneConfig["rich"] );
			rich:SetWidth( CHAT_RICH_DEFAULT_WIDTH );
		end

		MainChatFrame:SetWidth( CHAT_FRAME_DEFAULT_WIDTH );
		chatBackBtn:SetWidth( CHAT_BACK_DEFAULT_WIDTH );
	end
	
	for _, oneConfig in pairs( t_allChatTypePage ) do
		local rich = getglobal( oneConfig["rich"] );
		rich:resizeText( rich:GetWidth() * GetScreenScaleY(), rich:GetHeight() * GetScreenScaleY() );
	end
	
	Private_Update_MainChatFrame_Slider();
end

function MainChatFrame_HorizonalLongerBtn_OnEnter()
	local szText = "向右调整窗口大小";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } )
end

function InitMainChatFrame()
	local chatBackBtn	= getglobal( "MainChatFrame_ChatBackBtn" );
	local slider		= getglobal( "MainChatFrame_ScrollBar" );
	local scrollDownBtn	= getglobal( "MainChatFrame_ScrollDownBtn" );

	for _, oneConfig in pairs( t_allChatTypePage ) do
		local rich = getglobal( oneConfig["rich"] );
		rich:SetWidth( CHAT_RICH_DEFAULT_WIDTH );
	end

	MainChatFrame:SetWidth( CHAT_FRAME_DEFAULT_WIDTH );
	chatBackBtn:SetWidth( CHAT_BACK_DEFAULT_WIDTH );
	
	ChatShowBugleMsgFrame:SetPoint( "bottomleft", "MainChatFrame", "topleft", 27, 20 );
	MainChatFrame:SetHeight( CHAT_FRAME_DEFAULT_HEIGHT );
	chatBackBtn:SetHeight( CHAT_BACK_DEFAULT_HEIGHT );
	slider:SetHeight( CHAT_SLIDER_DEFAULT_HEIGHT );
	scrollDownBtn:SetPoint( "top", "MainChatFrame_ScrollUpBtn", "bottom", 0, CHAT_SCROODOWN_TO_UP_DIST );
	ChatShowBugleMsgFrame:SetPointEx( "bottomleft", "MainChatFrame", "topleft", 27, 20 );

	for _, oneConfig in pairs( t_allChatTypePage ) do
		local rich = getglobal( oneConfig["rich"] );
		rich:SetHeight( CHAT_RICH_DEFAULT_HEIGHT );
	end

	local t_riches = {	"MainChatFrame_ZongHeRich",		"MainChatFrame_ChatWorldRich",	"MainChatFrame_ChatClanRich", 
						"MainChatFrame_ChatTeamRich",	"MainChatFrame_ChatNearRich",	"MainChatFrame_ChatPrivateRich", 
						"MainChatFrame_ChatXinXiRich", 
					};
	for _, name in ipairs( t_riches ) do
		local rich = getglobal( name );
		rich:Clear();
		rich:clearHistory();
	end
	
	local t_MainChatFrameControl = GetMainChatFrameControl();
	t_MainChatFrameControl:init();

	for _, onePageConfig in pairs( t_allChatTypePage ) do
		Private_SetOnePageBtnNotGetNewMessageState( { ["btn"] = onePageConfig["btn"] } );
	end
end

-- 向上扩展高度
function MainChatFrame_VerticalLongerBtn_OnClick()
	local chatBackBtn	= getglobal( "MainChatFrame_ChatBackBtn" );
	local scrollDownBtn	= getglobal( "MainChatFrame_ScrollDownBtn" );
	local slider		= getglobal( "MainChatFrame_ScrollBar" );
	
	if chatBackBtn:GetHeight() < ( CHAT_BACK_DEFAULT_HEIGHT + CHAT_INCR_UNIT * 3 ) then
		MainChatFrame:SetHeight( MainChatFrame:GetHeight() + CHAT_INCR_UNIT + 6 );
		chatBackBtn:SetHeight( chatBackBtn:GetHeight() + CHAT_INCR_UNIT + 6 );
		slider:SetHeight( slider:GetHeight() + CHAT_INCR_UNIT + 6 );
		scrollDownBtn:SetPoint( "top", "MainChatFrame_ScrollUpBtn", "bottom", 0, scrollDownBtn:GetAnchorOffsetY() + CHAT_INCR_UNIT + 6 );
		for _, oneConfig in pairs( t_allChatTypePage ) do
			local rich = getglobal( oneConfig["rich"] );
			rich:SetHeight( rich:GetHeight() + CHAT_INCR_UNIT + 6 );
		end
	else
		ChatShowBugleMsgFrame:SetPoint( "bottomleft", "MainChatFrame", "topleft", 27, 20 );
		MainChatFrame:SetHeight( CHAT_FRAME_DEFAULT_HEIGHT );
		chatBackBtn:SetHeight( CHAT_BACK_DEFAULT_HEIGHT );
		slider:SetHeight( CHAT_SLIDER_DEFAULT_HEIGHT );
		scrollDownBtn:SetPoint( "top", "MainChatFrame_ScrollUpBtn", "bottom", 0, CHAT_SCROODOWN_TO_UP_DIST );
		
		for _, oneConfig in pairs( t_allChatTypePage ) do
			local rich = getglobal( oneConfig["rich"] );
			rich:SetHeight( CHAT_RICH_DEFAULT_HEIGHT );
		end
	end

	ChatShowBugleMsgFrame:SetPointEx( "bottomleft", "MainChatFrame", "topleft", 27, 20 );

	for _, oneConfig in pairs( t_allChatTypePage ) do
		local rich = getglobal( oneConfig["rich"] );
		rich:resizeText( rich:GetWidth()*GetScreenScaleY(), rich:GetHeight()*GetScreenScaleY() );
	end
	
	Private_Update_MainChatFrame_Slider();
end

function MainChatFrame_VerticalLongerBtn_OnEnter()
	local szText = "向上调整窗口大小";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } )
end

-- 历史记录按钮
function MainChatFrame_HistoryBtn_OnClick()
	local frame = getglobal("ChatHistoryMsgMgrFrame");
	if frame:IsShown() then
		frame:Hide();
	else
		frame:Show();
	end
end

function MainChatFrame_HistoryBtn_OnEnter()
	SetGameTooltips(this:GetName(), "打开信息记录界面" );
end

-- 小喇叭按钮
function MainChatFrame_BugleBtn_OnClick()
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();

	--local containitem = container:getItem( CHAT_ITEM_ID );
	local containitem = GetContainerHaveItemResult( CONTAINER_TYPE_SUNDRIES, RESULT_ITEM_CHAT );
	if containitem == nil then
		ShowMidTips("缺少小喇叭道具");
		return;
	end

	local frame = getglobal("ChatBugleFrame");
	if frame:IsShown() then
		frame:Hide();
	else
		SetLaBaItem( { grid = containitem:getGridIdx(), list = containitem:getListType() } );
		LockItem( containitem:getListType(), containitem:getGridIdx(), containitem:getItemId(), "ChatBugleFrame" );
		frame:Show();
		ClearChatInsertFram();
		SetFocusFrame("CBFChatInsert");
	end
end

function MainChatFrame_BugleBtn_OnEnter()
	SetGameTooltips(this:GetName(), "打开小喇叭界面" );
end

-- 缩放聊天面板的辅助面板
-- 隐藏按钮
function MainChatFrame_MinimizeBtn_OnClick()
	local t_showIcons = { "ChatControlFrame", };
	util.ShowIcons( t_showIcons );
end

function MainChatFrame_MinimizeBtn_OnEnter()
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "隐藏聊天窗口", frame = this:GetParent(),
						button = this:GetName() } );
end

-- 还原按钮
function ChatControlRestoreBtn_OnClick()
	local t_showIcons = { "MainChatFrame" };
	util.ShowIcons( t_showIcons );
end

function ChatControlRestoreBtn_OnEnter()
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "还原聊天窗口", frame = this:GetParent(),
						button = this:GetName() } );
end

-- 控制聊天面板伸缩的辅助面板
function ChatControlFrame_OnLoad()
	this:setUpdateTime( 0 );
end

function ChatControlFrame_OnShow()
	local t_hideIcons = { "MainChatFrame", };
	util.HideIcons( t_hideIcons );
end

function ChatControlFrame_OnUpdate()
	-- 毫秒级更新
	UpdateGMCmd();
	GameBulletintTipsFrame_OnUpdate();
end

function checkStoreLink( szMsg )
	local _,idx1 = string.find( szMsg, "#L[查看店铺]@@", 1, true );
	if idx1 ~= nil then
		local idx2,idx3 = string.find( szMsg, "#n", idx1, true );
		local storeDesc = "";
		if string.len( szMsg ) == idx3 then
			storeDesc = "欢迎来店参观";
		end
		local linkCountId = ( string.sub( szMsg, idx1+1, idx2-1 ) - STORE_LINK ) / 10;
		local storeLink = ChatManager:getStoreLink( linkCountId );
		return string.gsub( szMsg, "#L%[查看店铺%]@@(%d+)#n", storeDesc ), storeLink.StoreID;
	else
		return szMsg, 0;
	end
end