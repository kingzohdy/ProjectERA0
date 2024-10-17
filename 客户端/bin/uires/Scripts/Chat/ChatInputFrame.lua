SMALL_PRIVATE_FORMAT		= "/m"; --私聊格式
MAX_CHATINSERT_CHAR			= 128;
local EDIT_WIDTH			= 210;
local MAX_SUPPORT_INPUT_CHAT_TYPE = 6;
local MAX_SELF_CHANNEL_SYMBOL_LEN = 2;

local MAX_LA_BA_TEXT_LEN		= 50;
local MAX_OTHER_CHAT_TEXT_LEN	= 128;

local nChatType   = CL_CHAT_SAY;	-- TODO: DEL

local tSelectChatColor;

t_ChannelSymbolConfig =
{
--	频道指令	聊天类型		替换文本
["/f"] = { chatType = CL_CHAT_SAY,		},
["/F"] = { chatType = CL_CHAT_SAY,		},

["/p"] = { chatType = CL_CHAT_TEAM,	},
["/P"] = { chatType = CL_CHAT_TEAM,	},

["/g"] = { chatType = CL_CHAT_CLAN,	},
["/G"] = { chatType = CL_CHAT_CLAN,	},

["/z"] = { chatType = CL_CHAT_CLAN_GROUP,	},
["/Z"] = { chatType = CL_CHAT_CLAN_GROUP,	},

["/w"] = { chatType = CL_CHAT_WORLD,	},
["/W"] = { chatType = CL_CHAT_WORLD,	},

["/m"] = { chatType = CL_CHAT_PRIVATE,	},
["/M"] = { chatType = CL_CHAT_PRIVATE,	},

["/b"] = { chatType = CL_CHAT_BATTLE,	},
["/B"] = { chatType = CL_CHAT_BATTLE,	},

["/l"] = { chatType = CL_CHAT_ITEM,	},
["/L"] = { chatType = CL_CHAT_ITEM,	},

["/s"] = { chatType = CL_CHAT_SPAN_MAP_WORLD,	},
["/S"] = { chatType = CL_CHAT_SPAN_MAP_WORLD,	},

["/k"] = { chatType = CL_CHAT_SPAN_MAP,	},
["/K"] = { chatType = CL_CHAT_SPAN_MAP,	},
};

t_Channel=
{
	--	频道指令	聊天类型			替换文本
	{"/f",			CL_CHAT_SAY,		""		},
	{"/p",			CL_CHAT_TEAM,		""		},
	{"/g",			CL_CHAT_CLAN,		""		},
	{"/z",			CL_CHAT_CLAN_GROUP,	""		},
	{"/w",			CL_CHAT_WORLD,		""		},
	{"/m",			CL_CHAT_PRIVATE,	"/m"	},
	{"/l",			CL_CHAT_ITEM,		""		},

	{"/F",			CL_CHAT_SAY,		""		},
	{"/P",			CL_CHAT_TEAM,		""		},
	{"/G",			CL_CHAT_CLAN,		""		},
	{"/Z",			CL_CHAT_CLAN_GROUP,	""		},
	{"/W",			CL_CHAT_WORLD,		""		},
	{"/M",			CL_CHAT_PRIVATE,	"/m"	},
	{"/L",			CL_CHAT_ITEM,		""		},
};

function GetChatType()
	return nChatType;
end

function SetChatType( nType )
	nChatType = nType;

	local edit = getglobal( "CSChatInsert" );
	if nChatType == CL_CHAT_ITEM or nChatType == CL_CHAT_SPAN then
		local szText = edit:GetText();
		edit:setMaxChar( MAX_LA_BA_TEXT_LEN );
		if string.len( szText ) > MAX_LA_BA_TEXT_LEN then
			edit:SetText( szText );
		end
	else
		edit:setMaxChar( MAX_OTHER_CHAT_TEXT_LEN );
	end

	CurChannelBtnName_text:SetText( t_mainChatFrameConfig[nChatType]["desc"] );
end

local t_ChatInsertFrameControl = { ["privatePlayerName"] = "", ["lastSendWorldMessage"] = 0, };

function GetChatInsertFrameControl()
	return t_ChatInsertFrameControl;
end

t_ChatInsertFrameControl["isSupportIChatType"] =
function ( self, data )
	local nValidateChatType = data["chatType"];
	if nValidateChatType == nil then
		return false;
	end

	for i = 1, MAX_SUPPORT_INPUT_CHAT_TYPE do
		local nDefSupportChatType = t_Channel[i][2];
		if nDefSupportChatType == nValidateChatType then
			return true;
		end
	end

	return false;
end

t_ChatInsertFrameControl["getChannelSymbol"] =
function ( self, data )
	local nChatType = data["chatType"];
	local szSymbol	= "/f";
	for preDefSymbol, data in pairs( t_ChannelSymbolConfig ) do
		if data["chatType"] == nChatType then
			return preDefSymbol;
		end
	end

	return szSymbol;
end

t_ChatInsertFrameControl["getChannelTypeBySymbol"] =
function ( self, data )
	local szSymbol = data["symbol"];
	for preDefSymbol, data in pairs( t_ChannelSymbolConfig ) do
		if szSymbol == preDefSymbol then
			return data["chatType"];
		end
	end

	return CL_CHAT_SAY;
end

t_ChatInsertFrameControl["getPrivatePlayerName"] =
function ( self )
	return self["privatePlayerName"];
end

t_ChatInsertFrameControl["setPrivatePlayerName"] =
function ( self, data )
	self["privatePlayerName"] = GetPlayerName(data["name"]);
end

t_ChatInsertFrameControl["isOneChannelSymbol"] =
function ( self, data )
	local szSrcSymbol = data["text"];
	return t_ChannelSymbolConfig[szSrcSymbol] ~= nil;
end

local t_ChatInsertFrameEvents = 
{ 
	["GE_BATTLE_ENTER"] = {},	 ["GE_BATTLE_LEAVE"] = {},	["GE_SPAN_MAP_ENTER"] = {},	 ["GE_SPAN_MAP_LEAVE"] = {},	
};

t_ChatInsertFrameEvents["GE_BATTLE_ENTER"].func = 
function ()
	SetChatType( CL_CHAT_BATTLE );
	Private_UpdateChatInputFrameUI();
end

t_ChatInsertFrameEvents["GE_BATTLE_LEAVE"].func = 
function ()
	local nChatType = GetChatType();
	if nChatType == CL_CHAT_BATTLE then
		SetChatType( CL_CHAT_SAY );
		Private_UpdateChatInputFrameUI();
	end
end

local isInSpan = false;

t_ChatInsertFrameEvents["GE_SPAN_MAP_ENTER"].func = 
function ()
	SetChatType( CL_CHAT_SPAN_MAP_WORLD );
	Private_UpdateChatInputFrameUI();
	isInSpan = true;
end

t_ChatInsertFrameEvents["GE_SPAN_MAP_LEAVE"].func = 
function ()
	if isInSpan then
		local nChatType = GetChatType();
		if nChatType == CL_CHAT_SPAN_MAP or nChatType == CL_CHAT_SPAN_MAP_WORLD then
			SetChatType( CL_CHAT_SAY );
			Private_UpdateChatInputFrameUI();
		end
		isInSpan = false;
	end
end

function ChatInsertFrame_OnLoad()
	for event, _ in pairs( t_ChatInsertFrameEvents ) do
		this:RegisterEvent( event );
	end

	t_ChatInsertFrameEvents.__index = function ( tab, key )
									tab[key] = { func = function () end };
									return tab[key];
								end
	setmetatable( t_ChatInsertFrameEvents, t_ChatInsertFrameEvents );
end

function ChatInsertFrame_OnEvent()
	t_ChatInsertFrameEvents[arg1].func();
end

-- 聊天内容输入框显示
function ChatInsertFrame_OnShow()
	UpdateChatInsertFrameEnterFocus();

	local nChatType			= GetChatType();
	
	local edit = getglobal( "CSChatInsert" );
	if nChatType == CL_CHAT_ITEM then
		local szText = edit:GetText();
		edit:setMaxChar( MAX_LA_BA_TEXT_LEN );
		if string.len( szText ) > MAX_LA_BA_TEXT_LEN then
			edit:SetText( szText );
		end
	else
		edit:setMaxChar( MAX_OTHER_CHAT_TEXT_LEN );
	end
end

function UpdateChatInsertFrameEnterFocus()
	local privateNameEdit	= getglobal( "ChatInsertFrame_PrivateNameEdit" );
	local nChatType			= GetChatType();
	
	if nChatType == CL_CHAT_PRIVATE then
		local t_ChatInsertFrameControl	= GetChatInsertFrameControl();
		local szPrivatePlayerName		= t_ChatInsertFrameControl:getPrivatePlayerName();
		local privateNameEdit			= getglobal( "ChatInsertFrame_PrivateNameEdit" );
		privateNameEdit:SetText( szPrivatePlayerName );
		SetFocusFrame("CSChatInsert");
	end
	Private_UpdateChatInputFrameUI();
end

-- 聊天内容输入框隐藏
function ChatInsertFrame_OnHide()
	CSChatInsert:enableIME( false );
	ClearFocus();
	ClearSetLink();

	local t_hideIcons = { "MainChatFaceFrame" };
	util.HideIcons( t_hideIcons );
end

function ClearChatInsertFram()
	CSChatInsert:enableIME( false );
	ClearFocus();
	ClearSetLink();
end

-- 点击当前频道类型按钮
function ChatInsertFrame_CurChannelBtn_OnClick()
	local frame = getglobal( "ChatInsertChannelMenuFrame" );
	if frame:IsShown() then
		frame:Hide();
	else
		frame:SetClientString( this:GetName() );
		frame:Show();		 
	end
end

-- 聊天输入框输入内容的事件响应	[VK_SPACE] = {},
local t_ChatInSertOnCharEvent = { [VK_SPACE] = {}, [VK_BACK] = {}, [VK_DELETE] = {}, [VK_UP] = {}, [VK_DOWN] = {}, };
local bNeedEraseFirstSpace = false;

t_ChatInSertOnCharEvent[VK_SPACE].func = 
function ( data )
	--[[
	local szText = data["text"];
	if string.len(szText) > 2 then
		return;
	end
	
	for _, oneChannelConfig in ipairs( t_Channel ) do
		if string.sub( szText, 1, 2 ) == oneChannelConfig[1] then
			local nCurChatType = oneChannelConfig[2];
			SetChatType( nCurChatType );
			Private_UpdateChatInputFrameUI();
			CSChatInsert:Clear();
			bNeedEraseFirstSpace = true;
			break;
		end
	end
	--]]
end

t_ChatInSertOnCharEvent[VK_BACK].func = 
function ( data )
	local szText		= data["text"];
	local nCursorPos	= data["arg2"] - 1;
	local nCurSelBegin	= data["arg3"] - 1;
	
	--print( "VK_BACK--nCursorPos = "..nCursorPos.."， nCurSelBegin = "..nCurSelBegin );
	if nCursorPos == nCurSelBegin then
		Private_SamePosBack( { ["text"] = szText, ["selBegin"] = nCursorPos, ["edit"] = data["edit"] } );
	else
		local nSelBegin	= 0;
		local nSelEnd	= 0;
		nSelBegin	= math.min( nCursorPos, nCurSelBegin );
		nSelEnd		= math.max( nCursorPos, nCurSelBegin );
		Private_DiffPosBack( { ["text"] = szText, ["selBegin"] = nSelBegin, ["selEnd"] = nSelEnd, ["edit"] = data["edit"] } );
	end
end

function Private_RoundCursorPos( data )
	local edit		= getglobal( data["edit"] );
	local szText	= edit:GetText();
	local nTextLen	= string.len( szText );
	local nCursorPos= edit:getCursorPos();
	if nCursorPos > nTextLen then
		edit:setCursorPos( nTextLen );
	end

	local nSelBegin= edit:getSelBegin();
	if nSelBegin > nTextLen then
		edit:setSelBegin( nTextLen );
	end
end

function Private_SamePosBack( data )
	local edit		= getglobal( data["edit"] );
	local szText	= data["text"];
	local nCurPos	= data["selBegin"];
	if nCurPos <= 0 then
		return;
	end

	if IsDBCSChar( szText, nCurPos - 1 ) then
		if nCurPos >= 2 then
			nCurPos = nCurPos - 2;
		elseif nCurPos > 0 then
			nCurPos = nCurPos - 1;
		end
	else
		nCurPos = nCurPos - 1;
	end

	nCurPos = nCurPos + 1;
	--print( "nCurPos = "..nCurPos );
	local t_delLinkType = {};
	for i = 1, ChatManager:getNumCacheDisplayText() do
		local oneCacheDisplayTextInfo		= ChatManager:getCacheDisplayText( i - 1 );
		local nLinkBeginPos, nLinkEndPos	= string.find( szText, oneCacheDisplayTextInfo.m_strLinkName, 1, true );
		--[[
		print( "m_strLinkName = "..oneCacheDisplayTextInfo.m_strLinkName.."， szText = "..szText..
				"， nLinkBeginPos = "..nLinkBeginPos.."， nLinkEndPos = "..nLinkEndPos.."\n" );
		--]]
		if ( nLinkBeginPos ~= nil and nLinkEndPos ~= nil ) and ( nCurPos >= nLinkBeginPos and nCurPos <= nLinkEndPos ) then
			local szNewText	= ChatManager:strReplace( szText, oneCacheDisplayTextInfo.m_strLinkName, "" );
			table.insert( t_delLinkType, oneCacheDisplayTextInfo.m_nLinkType );
			szNewText = szNewText..".";
			edit:SetText( szNewText );
			Private_RoundCursorPos( data );
			break;
		end
	end

	for index, deleteLinkType in ipairs( t_delLinkType ) do
		ChatManager:deleteLink( deleteLinkType );
		RemoveOneLinkCache( { ["linkType"] = deleteLinkType } );
	end
end

function Private_DiffPosBack( data )
	local edit		= getglobal( data["edit"] );
	local szText	= data["text"];
	local nSelBegin	= data["selBegin"];
	local nSelEnd	= data["selEnd"];
	nSelBegin		= nSelBegin + 1;
	
	--print( "nSelBegin = "..nSelBegin.."， nSelEnd = "..nSelEnd );
	local t_delLinkType = {};
	for i = 1, ChatManager:getNumCacheDisplayText() do
		local oneCacheDisplayTextInfo		= ChatManager:getCacheDisplayText( i - 1 );
		local nLinkBeginPos, nLinkEndPos	= string.find( szText, oneCacheDisplayTextInfo.m_strLinkName, 1, true );
		--print( "m_strLinkName = "..oneCacheDisplayTextInfo.m_strLinkName.."， szText = "..szText..
		--		"， nLinkBeginPos = "..nLinkBeginPos.."， nLinkEndPos = "..nLinkEndPos.."\n" );

		if ( nLinkBeginPos ~= nil and nLinkEndPos ~= nil ) and ( ( nSelBegin < nLinkBeginPos and nSelEnd >= nLinkBeginPos ) or 
																	nSelBegin >= nLinkBeginPos and nSelBegin <= nLinkEndPos ) then
			table.insert( t_delLinkType, { ["linkType"] = oneCacheDisplayTextInfo.m_nLinkType, ["text"] = oneCacheDisplayTextInfo.m_strLinkName } );
			Private_AdjustEditBackPos( { ["selBegin"] = nSelBegin, ["selEnd"] = nSelEnd, ["linkBegin"] = nLinkBeginPos, ["linkEnd"] = nLinkEndPos,
											["edit"] = data["edit"] } );
		end
	end
	
	if table.getn( t_delLinkType ) == 0 then
		return;
	end

	local szNewText = szText;
	for index, oneDeleteLinkData in ipairs( t_delLinkType ) do
		--szNewText	= ChatManager:strReplace( szNewText, oneDeleteLinkData["text"], "" );
		ChatManager:deleteLink( oneDeleteLinkData["linkType"] );
		RemoveOneLinkCache( { ["linkType"] = oneDeleteLinkData["linkType"] } );
	end
end

function Private_AdjustEditBackPos( data )
	local edit			= getglobal( data["edit"] );
	local nSelBegin		= data["selBegin"];
	local nSelEnd		= data["selEnd"];
	local nLinkBeginPos	= data["linkBegin"];
	local nLinkEndPos	= data["linkEnd"];

	if nSelBegin < nLinkBeginPos and nSelEnd >= nLinkBeginPos then
		edit:setSelBegin( math.min( nSelBegin - 1, edit:getSelBegin() ) );
		edit:setCursorPos( math.max( math.max( nSelEnd, nLinkEndPos ), edit:getCursorPos() ) );
		return;
	end

	if nSelBegin >= nLinkBeginPos and nSelBegin <= nLinkEndPos then
		edit:setSelBegin( math.min( nLinkBeginPos - 1, edit:getSelBegin() ) );
		edit:setCursorPos( math.max( math.max( nSelEnd, nLinkEndPos ), edit:getCursorPos() ) );
		return;
	end
end

t_ChatInSertOnCharEvent[VK_DELETE].func = 
function ( data )
	local szText		= data["text"];
	local nCursorPos	= data["arg2"] - 1;
	local nCurSelBegin	= data["arg3"] - 1;
	
	--print( "VK_DELETE--nCursorPos = "..nCursorPos.."， nCurSelBegin = "..nCurSelBegin );
	if nCursorPos == nCurSelBegin then
		Private_SamePosDelete( { ["text"] = szText, ["selBegin"] = nCursorPos, ["edit"] = data["edit"] } );
	else
		local nSelBegin	= 0;
		local nSelEnd	= 0;
		nSelBegin	= math.min( nCursorPos, nCurSelBegin );
		nSelEnd		= math.max( nCursorPos, nCurSelBegin );
		Private_DiffPosDelete( { ["text"] = szText, ["selBegin"] = nSelBegin, ["selEnd"] = nSelEnd, ["edit"] = data["edit"] } );
	end
end

function Private_SamePosDelete( data )
	local szText	= data["text"];
	local nCurPos	= data["selBegin"];
	local nTextLen	= string.len( szText );
	if nCurPos == nTextLen then
		return;
	end
	
	local edit			= getglobal( data["edit"] );
	nCurPos = nCurPos + 1;
	--print( "nCurPos = "..nCurPos );
	local t_delLinkType = {};
	for i = 1, ChatManager:getNumCacheDisplayText() do
		local oneCacheDisplayTextInfo		= ChatManager:getCacheDisplayText( i - 1 );
		local nLinkBeginPos, nLinkEndPos	= string.find( szText, oneCacheDisplayTextInfo.m_strLinkName, 1, true );
		
		--print( "m_strLinkName = "..oneCacheDisplayTextInfo.m_strLinkName.."， szText = "..szText..
		--		"， nLinkBeginPos = "..nLinkBeginPos.."， nLinkEndPos = "..nLinkEndPos.."\n" );
		
		if ( nLinkBeginPos ~= nil and nLinkEndPos ~= nil ) and ( nCurPos >= nLinkBeginPos and nCurPos <= nLinkEndPos ) then
			local szNewText	= ChatManager:strReplace( szText, oneCacheDisplayTextInfo.m_strLinkName, "" );
			table.insert( t_delLinkType, oneCacheDisplayTextInfo.m_nLinkType );
			edit:SetText( szNewText );
			break;
		end
	end

	for index, deleteLinkType in ipairs( t_delLinkType ) do
		ChatManager:deleteLink( deleteLinkType );
		RemoveOneLinkCache( { ["linkType"] = deleteLinkType } );
	end
end

function Private_DiffPosDelete( data )
	local szText	= data["text"];
	local nSelBegin	= data["selBegin"];
	local nSelEnd	= data["selEnd"];
	nSelBegin		= nSelBegin + 1;
	
	--print( "nSelBegin = "..nSelBegin.."， nSelEnd = "..nSelEnd );
	local t_delLinkType = {};
	for i = 1, ChatManager:getNumCacheDisplayText() do
		local oneCacheDisplayTextInfo		= ChatManager:getCacheDisplayText( i - 1 );
		local nLinkBeginPos, nLinkEndPos	= string.find( szText, oneCacheDisplayTextInfo.m_strLinkName, 1, true );
		--print( "m_strLinkName = "..oneCacheDisplayTextInfo.m_strLinkName.."， szText = "..szText..
		--		"， nLinkBeginPos = "..nLinkBeginPos.."， nLinkEndPos = "..nLinkEndPos.."\n" );

		if ( nLinkBeginPos ~= nil and nLinkEndPos ~= nil ) and ( ( nSelBegin < nLinkBeginPos and nSelEnd >= nLinkBeginPos ) or 
																	nSelBegin >= nLinkBeginPos and nSelBegin <= nLinkEndPos ) then
			table.insert( t_delLinkType, { ["linkType"] = oneCacheDisplayTextInfo.m_nLinkType, ["text"] = oneCacheDisplayTextInfo.m_strLinkName } );
			Private_AdjustEditDeletePos( { ["selBegin"] = nSelBegin, ["selEnd"] = nSelEnd, ["linkBegin"] = nLinkBeginPos, ["linkEnd"] = nLinkEndPos,
											["edit"] = data["edit"] } );
		end
	end
	
	if table.getn( t_delLinkType ) == 0 then
		return;
	end

	--local szNewText = szText;
	for index, oneDeleteLinkData in ipairs( t_delLinkType ) do
		--szNewText	= ChatManager:strReplace( szNewText, oneDeleteLinkData["text"], "" );
		ChatManager:deleteLink( oneDeleteLinkData["linkType"] );
		RemoveOneLinkCache( { ["linkType"] = oneDeleteLinkData["linkType"] } );
	end

	--CSChatInsert:SetText( szNewText );
end

function Private_AdjustEditDeletePos( data )
	local edit			= getglobal( data["edit"] );
	local nSelBegin		= data["selBegin"];
	local nSelEnd		= data["selEnd"];
	local nLinkBeginPos	= data["linkBegin"];
	local nLinkEndPos	= data["linkEnd"];

	if nSelBegin < nLinkBeginPos and nSelEnd >= nLinkBeginPos then
		edit:setSelBegin( math.min( nSelBegin - 1, edit:getSelBegin() ) );
		edit:setCursorPos( math.max( math.max( nSelEnd, nLinkEndPos ), edit:getCursorPos() ) );
		return;
	end

	if nSelBegin >= nLinkBeginPos and nSelBegin <= nLinkEndPos then
		edit:setSelBegin( math.min( nLinkBeginPos - 1, edit:getSelBegin() ) );
		edit:setCursorPos( math.max( math.max( nSelEnd, nLinkEndPos ), edit:getCursorPos() ) );
		return;
	end
end

function DeletChatInputLinkType( nDeleteLinkType, nOpCode )
	--print( "DeletChatInputLinkType--305" );
	local szText			= CSChatInsert:GetText();
	local t_deleteLinkData	= GetOneLinkCache( { ["linkType"] = nDeleteLinkType } );
	if t_deleteLinkData == nil then
	--	print( "DeletChatInputLinkType--t_deleteLinkData == nil--nDeleteLinkType = "..nDeleteLinkType );
		return;
	end

	local szNewText	= ChatManager:strReplace( szText, t_deleteLinkData["originalName"], "" );
	if nOpCode == VK_BACK then
		szNewText = szNewText..".";
	end

	CSChatInsert:SetText( szNewText );

	RemoveOneLinkCache( { ["linkType"] = nDeleteLinkType } );
end

t_ChatInSertOnCharEvent[VK_UP].func = 
function ( data )
	-- 获取上翻下翻历史记录时设置的文本
	local szCacheHistoryText						= CSChatInsert:GetText();
	local t_ClientSelfChatContentParser				= GetClientSelfChatContentParser();
	local nChatType, szText, szPrivatePlayerName	= t_ClientSelfChatContentParser:separateChannelInfoFromText( { ["text"] = szCacheHistoryText } );
	
--	SetChatType( nChatType );

	local t_ChatInsertFrameControl = GetChatInsertFrameControl();
	if nChatType == CL_CHAT_PRIVATE then
		t_ChatInsertFrameControl:setPrivatePlayerName( { ["name"] = szPrivatePlayerName } );
		local nameEdit = getglobal( "ChatInsertFrame_PrivateNameEdit" );
		nameEdit:SetText( szPrivatePlayerName );
	end
	CSChatInsert:SetText( szText );
	ClearSetLink();
	ParserChatHistoryText( szText );
	
	Private_UpdateChatInputFrameUI();
end

t_ChatInSertOnCharEvent[VK_DOWN].func = 
function ( data )
	local eventCode = data["arg4"];
	--print( "VK_DOWN--eventCode = "..eventCode );
	if eventCode ~= EDIT_EVENT_KEY then
		return;
	end

	t_ChatInSertOnCharEvent[VK_UP].func();
end

function Private_SamePosOnQueryChar( data )
	local szText	= data["text"];
	local nCurPos	= data["selBegin"];
	local nTextLen	= string.len( szText );
	if nCurPos == nTextLen then
		return true;
	end

	nCurPos = nCurPos + 1;
	--print( "nCurPos = "..nCurPos );

	for i = 1, ChatManager:getNumCacheDisplayText() do
		local oneCacheDisplayTextInfo		= ChatManager:getCacheDisplayText( i - 1 );
		local nLinkBeginPos, nLinkEndPos	= string.find( szText, oneCacheDisplayTextInfo.m_strLinkName, 1, true );
		
		--print( "m_strLinkName = "..oneCacheDisplayTextInfo.m_strLinkName.."， szText = "..szText..
		--		"， nLinkBeginPos = "..nLinkBeginPos.."， nLinkEndPos = "..nLinkEndPos.."\n" );
		
		if ( nLinkBeginPos ~= nil and nLinkEndPos ~= nil ) and ( nCurPos > nLinkBeginPos and nCurPos <= nLinkEndPos ) then
		--	print( "Private_SamePosOnQueryChar--false" );
			return false;
		end
	end
	
	--print( "Private_SamePosOnQueryChar--true" );
	return true;
end

function Private_DiffPosOnQueryChar( data )
	local szText	= data["text"];
	local nSelBegin	= data["selBegin"];
	local nSelEnd	= data["selEnd"];
	nSelBegin		= nSelBegin + 1;
	
	--print( "nSelBegin = "..nSelBegin.."， nSelEnd = "..nSelEnd );
	for i = 1, ChatManager:getNumCacheDisplayText() do
		local oneCacheDisplayTextInfo		= ChatManager:getCacheDisplayText( i - 1 );
		local nLinkBeginPos, nLinkEndPos	= string.find( szText, oneCacheDisplayTextInfo.m_strLinkName, 1, true );
	--	print( "m_strLinkName = "..oneCacheDisplayTextInfo.m_strLinkName.."， szText = "..szText..
	--			"， nLinkBeginPos = "..nLinkBeginPos.."， nLinkEndPos = "..nLinkEndPos.."\n" );

		if ( nLinkBeginPos ~= nil and nLinkEndPos ~= nil ) and ( ( nSelBegin < nLinkBeginPos and nSelEnd >= nLinkBeginPos ) or 
																	nSelBegin >= nLinkBeginPos and nSelBegin <= nLinkEndPos ) then
			return false;
		end


	end
	
	return true;
end

function ChatInSert_OnQueryChar()
	local szText		= CSChatInsert:GetText();
	local nCursorPos	= arg1;
	local nCurSelBegin	= arg2;
	if nCursorPos == nCurSelBegin then
		return Private_SamePosOnQueryChar( { ["text"] = szText, ["selBegin"] = nCursorPos } );
	else
		local nSelBegin	= 0;
		local nSelEnd	= 0;
		nSelBegin	= math.min( nCursorPos, nCurSelBegin );
		nSelEnd		= math.max( nCursorPos, nCurSelBegin );
		return Private_DiffPosOnQueryChar( { ["text"] = szText, ["selBegin"] = nSelBegin, ["selEnd"] = nSelEnd } );
	end

	return true;
end

function ChatInSert_OnChar()
	local szText		= CSChatInsert:GetText();
	local onCharEvent	= t_ChatInSertOnCharEvent[arg1];
	if onCharEvent ~= nil then
		onCharEvent.func( { ["text"] = szText, ["arg2"] = arg2, ["arg3"] = arg3, ["arg4"] = arg4, ["edit"] = this:GetName() } );
	end
end

function CSChatInsert_OnUpdate()	
	if bNeedEraseFirstSpace then
		local szText = CSChatInsert:GetText();
		if szText ~= "" and string.sub( szText, 1, 1 ) == " " then
			local szSubText = string.sub( szText,2,string.len( szText ) );
			CSChatInsert:SetText( szSubText );
			bNeedEraseFirstSpace = false;
		end
	end
end

-- 按ESC关闭聊天内容输入框
function ChatInSert_OnEscapePressed()
	-- ESC键是否按下
	CSChatInsert:Clear();
	if ChatInsertFrame:IsShown() then
		SetFocusFrame("UIClient");
		ClearChatInsertFram();
	end
end

-- 在聊天内容输入框中按Enter键

-- 在输入聊天内容的时候客户端本地解析器（不是服务器接受解析器）
local t_ClientSelfChatContentParser = {};

function GetClientSelfChatContentParser()
	return t_ClientSelfChatContentParser;
end

-- 解析输入框的原始文本
t_ClientSelfChatContentParser["validateNeedChangeChannel"] = 
function ( self, data )
	local szText				= data["text"];
	if string.len( szText ) < MAX_SELF_CHANNEL_SYMBOL_LEN then
		return szText;
	end
	
	local szChannelSymbol = string.sub( szText, 1, MAX_SELF_CHANNEL_SYMBOL_LEN );
	if not t_ChatInsertFrameControl:isOneChannelSymbol( { ["text"] = szChannelSymbol } ) then
		return szText;
	end
	
	local t_oneChannelConfig	= t_ChannelSymbolConfig[szChannelSymbol];
	local nNewChatType			= t_oneChannelConfig["chatType"];
	if nNewChatType == CL_CHAT_ITEM then
		if IsHaveLaBaItem() then
			local laBaItem = GetContainerOneLaBaItem();
			SetLaBaItem( { grid = laBaItem:getGridIdx(), list = laBaItem:getListType() } );
		else
			SetLaBaItem( { grid = -1, list = -1 } );
		end
	end

	SetChatType( nNewChatType );
	Private_UpdateChatInputFrameUI();

	szText = string.sub( szText, MAX_SELF_CHANNEL_SYMBOL_LEN + 1 );
	if nNewChatType == CL_CHAT_ITEM then
		if string.len( szText ) > MAX_LA_BA_TEXT_LEN then
			szText = string.sub( szText, 1, MAX_LA_BA_TEXT_LEN );
		end
	end

	return szText;
end

t_ClientSelfChatContentParser["CanSendChatMessage"] = 
function ( self, data )
	local szText = data["text"];
	
	if szText == "" then
		return false;
	end

	local tmpValidateText = szText;
	tmpValidateText = ChatManager:strTrim( tmpValidateText );
	if tmpValidateText == "" then
		return false;
	end
	
	local mainplayer			= ActorMgr:getMainPlayer();	
	local nChatType				= GetChatType();
	local szPrivatePlayerName	= t_ChatInsertFrameControl:getPrivatePlayerName();
	if nChatType == CL_CHAT_PRIVATE and szPrivatePlayerName=="" then
		local szText = "私聊对象不能为空";
		AddGameSysTip( szText );
		SetSystemMessage( szText );
		return false;
	end

	if nChatType == CL_CHAT_PRIVATE and string.sub( szPrivatePlayerName, 1, 1 ) == " " then
		local szText = "输入的名字不合法";
		AddGameSysTip( szText );
		SetSystemMessage( szText );
		return false;
	end

	if nChatType == CL_CHAT_PRIVATE and szPrivatePlayerName== mainplayer:getName() then
		local szText = "私聊对象不能为自己！";
		AddGameSysTip( szText );
		SetSystemMessage( szText );
		Private_QuitSendChatMessage();
		return false;
	end

	if nChatType == CL_CHAT_TEAM and not mainplayer:isInTeam() then
		local szText = "你当前不在队伍中";
		AddGameSysTip( szText );
		SetSystemMessage( szText );
		Private_QuitSendChatMessage();
		return false;
	end

	if nChatType == CL_CHAT_CLAN and not mainplayer:isInClan() then
		local szText = "你当前未加任何公会";
		AddGameSysTip( szText );
		SetSystemMessage( szText );
		Private_QuitSendChatMessage();
		return false;
	end

	if nChatType == CL_CHAT_CLAN_GROUP and not GameClanManager:isInClanGroup() then
		local szText = "你未参加任何战团";
		AddGameSysTip( szText );
		SetSystemMessage( szText );
		Private_QuitSendChatMessage();
		return false;
	end

	if nChatType == CL_CHAT_ITEM and not IsHaveLaBaItem() then
		local szText = "缺少小喇叭道具！";
		AddGameSysTip( szText );
		SetSystemMessage( szText );
		Private_QuitSendChatMessage();
		return false;
	end

	if nChatType == CL_CHAT_WORLD then
		if mainplayer:getLv() < CL_CHAT_WORLD_LEVEL then
			SetSystemMessage( "此频道发言要求最小等级为"..CL_CHAT_WORLD_LEVEL.."级" );
			Private_QuitSendChatMessage();
			return false;
		end
		
		--[[
		if ( GameMgr:getTickTime() - t_ChatInsertFrameControl["lastSendWorldMessage"] ) < CL_CHAT_WORLD_COOL then 
			CSChatInsert:Clear();
			ClearSetLink();
			local szText = "发言频率超过设定，世界频道"..math.floor(CL_CHAT_WORLD_COOL/1000).."秒发言一次";
			SetSystemMessage( szText );
			AddGameSysTip( szText );
			return false;
		else
			t_ChatInsertFrameControl["lastSendWorldMessage"] = GameMgr:getTickTime();
		end
		--]]
	end

	return true;
end

t_ClientSelfChatContentParser["TranslateSpecialSymbol"] = 
function ( self, data )
	local szText = data["text"];
	for i, v in pairs(CharPreprocessor or {}) do
		szText = string.gsub(szText, i, v)
	end

	if string.find( szText, "#" ) then
		szText = replaceFaceString( szText );
	end

	return szText;
end

t_ClientSelfChatContentParser["ValidateChatItem"] = 
function ( self )
	local nChatType = GetChatType();
	if nChatType ~= CL_CHAT_ITEM then
		return;
	end

	if IsSetLaBaItem() and ChatBugleFrame:IsShown() then
		ChatBugleFrame:Hide();
	end

	if IsHaveLaBaItem() then
		local laBaItem = GetContainerOneLaBaItem();
		SetLaBaItem( { grid = laBaItem:getGridIdx(), list = laBaItem:getListType() } );
	end
end

t_ClientSelfChatContentParser["parseLinkTextForServe"] = 
function ( self, data )
	local szText	= data["text"];
	for index, oneLinkData in ipairs( t_SetLink ) do
		local src = oneLinkData["clientFormatName"];	--名字
		local des = oneLinkData["serveFormamtName"]; --发向服务端的字符串
		szText = ChatManager:strReplace( szText, src, des );
	end
	return szText;
end

t_ClientSelfChatContentParser["sendTextToServe"] = 
function ( self, data )
	local szText	= data["text"];
	szText			= t_ClientSelfChatContentParser:parseLinkTextForServe( { ["text"] = szText } );
	local nChatType	= GetChatType();

	if nChatType == CL_CHAT_ITEM or nChatType == CL_CHAT_SPAN then
		local t_chatItem = GetLaBaItem();
		ChatManager:requestChat( nChatType, "", szText, t_chatItem["list"], t_chatItem["grid"] );
		SetChatType( CL_CHAT_ITEM );
		if not ChatBugleFrame:IsShown() then
			SetLaBaItem( { grid = -1, list = -1 } );
		else
			ChatBugleFrame:Hide();
		end
		return;
	end
	
	assert( szText ~= "", "上面解析有问题" );

	local t_ChatInsertFrameControl	= GetChatInsertFrameControl();
	local szPrivatePlayerName		= t_ChatInsertFrameControl:getPrivatePlayerName();
	ChatManager:requestChat( nChatType, szPrivatePlayerName, szText );
end

-- 缓存聊天历史记录
t_ClientSelfChatContentParser["parseTextForHistory"] = 
function ( self, data )
	local szText = data["text"];
	if szText == nil then
		return nil;
	end
	
	for index, oneLinkData in ipairs( t_SetLink ) do
		local src	= oneLinkData["originalName"]; --名字
		local des	= oneLinkData["clientFormatName"]; --发向客户端的字符串
		if szTextColor ~= nil then
			szText = string.gsub( szText , src, "#n" .. src .. szTextColor )
		end
		szText = ChatManager:strReplace( szText, src, des );
	end

	if string.find( szText,"#@!",1,true ) then
		while string.find( szText,"#@!",1,true ) do
			local nBeginPos		= string.find( szText, "#@!" );
			local nEndPos		= string.find( szText, "#", nBeginPos + 1, string.len(szText));
			local context		= string.sub( szText, nBeginPos + string.len( "#@!" ), nEndPos - 1 );
			szText = string.sub( szText, nEndPos + 1,string.len(szText) );
		end
	end

	return szText;
end

t_ClientSelfChatContentParser["separatePrivateInfoFromText"] = 
function ( self, data )
	local szText = data["text"];
	local firstPrivatePos, endPrivatePos = string.find( szText, SMALL_PRIVATE_FORMAT );
	
	if endPrivatePos == nil then
		return CL_CHAT_SAY, szText;
	end

	local firstEndNamePos, endEndNamePos = string.find( szText, " ", endPrivatePos + 1 );
	if firstEndNamePos == nil or endEndNamePos == nil then
		local szContent = string.sub( szText, MAX_SELF_CHANNEL_SYMBOL_LEN + 1 );
		return CL_CHAT_SAY, szContent;
	end

	local szPlayerName	= string.sub( szText, endPrivatePos + 1, firstEndNamePos - 1 );
	local szContent		= string.sub( szText, endEndNamePos + 1 );

	return CL_CHAT_PRIVATE, szContent, szPlayerName;
end

t_ClientSelfChatContentParser["separateNotPrivateInfoFromText"] = 
function ( self, data )
	local szText			= data["text"];
	local nSubSymbolText	= string.sub( szText, 1, MAX_SELF_CHANNEL_SYMBOL_LEN );
	local nChatType			= t_ChatInsertFrameControl:getChannelTypeBySymbol( { ["symbol"] = nSubSymbolText  } );
	local szContent			= string.sub( szText, MAX_SELF_CHANNEL_SYMBOL_LEN + 1 );

	return nChatType, szContent;
end

t_ClientSelfChatContentParser["separateChannelInfoFromText"] = 
function ( self, data )
	local szText = data["text"];
	if string.len( szText ) <= MAX_SELF_CHANNEL_SYMBOL_LEN then
		return CL_CHAT_SAY, szText;
	end

	local firstPrivatePos, endPrivatePos = string.find( szText, SMALL_PRIVATE_FORMAT );
	if firstPrivatePos == 1 then
		return self:separatePrivateInfoFromText( { ["text"] = szText } );
	else
		return self:separateNotPrivateInfoFromText( { ["text"] = szText } );
	end
end

t_ClientSelfChatContentParser["cacheChatHistory"] = 
function ( self, data )
	local szOriginalText		= data["originalText"];
	local szParsedText			= data["parsedText"];
	local szAddToHistroyText	= self:parseTextForHistory( { ["text"] = szOriginalText } );
	if szAddToHistroyText == nil then
		szAddToHistroyText = szParsedText;
	end
	
	-- 对于私聊记录我要保存一个解析格式
	local nChatType					= GetChatType();
	local t_ChatInsertFrameControl	= GetChatInsertFrameControl();
	local szPrivatePlayerName		= t_ChatInsertFrameControl:getPrivatePlayerName();
	if nChatType == CL_CHAT_PRIVATE then
		szAddToHistroyText = SMALL_PRIVATE_FORMAT..szPrivatePlayerName.." "..szAddToHistroyText;
	else
		local channelTypeSymbol = t_ChatInsertFrameControl:getChannelSymbol( { ["chatType"] = nChatType } );
		szAddToHistroyText		= channelTypeSymbol..szAddToHistroyText;
	end

	local edit = getglobal( "CSChatInsert" );
	edit:AddStringToHistory( szAddToHistroyText );
end

-- 缓存私聊对象
t_ClientSelfChatContentParser["cachePrivatePlayerInfo"] = 
function ( self )
	local nChatType					= GetChatType();
	if nChatType ~= CL_CHAT_PRIVATE then
		return;
	end

	local t_ChatInsertFrameControl	= GetChatInsertFrameControl();
	local szPrivatePlayerName		= t_ChatInsertFrameControl:getPrivatePlayerName();
	if szPrivatePlayerName == "" then
		return;
	end

	AddPrivatePlayerName(szPrivatePlayerName);
end

-- 解析物品超链接文本
t_ClientSelfChatContentParser["parseLinkTextForClient"] = 
function ( self, data )
	local szText = data["text"];
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer ~= nil and mainplayer:isHaveGMPerm() and GMToolsGetChatColor() ~= nil then
		--颜色
		szText = GMToolsGetChatColor() .. szText .."#n";
	end

	local szTextColor = nil;
	if mainplayer:isColorFontValid() > 0 and tSelectChatColor ~= nil then
		szTextColor = tSelectChatColor;
		szText = szTextColor .. szText;
	else
		tSelectChatColor = nil;
		CSChatInsert:SetTextColor( 255,255,255 );
	end
	for index, oneLinkData in ipairs( t_SetLink ) do
		local src	= oneLinkData["originalName"]; --名字
		local des	= oneLinkData["clientFormatName"]; --发向客户端的字符串
		if szTextColor ~= nil then
			szText = string.gsub( szText , src, "#n" .. src .. szTextColor )
		end
		szText		= ChatManager:strReplace( szText, src, des );
	end
	if szTextColor ~= nil then
		szText = szText .. "#n"
	end
	return szText;
end

-- 解析客户端自定义的指令
t_ClientSelfChatContentParser["DoClientDefineInstruction"] = 
function ( self, data )
	local szText = data["text"];
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer ~= nil and mainplayer:isHaveGMPerm() then
		if szText == "//start" then
			BeginAutoRoundWorlld();
			return true;
		end

		if szText == "//stop" then
			StopAutoRoundWorlld();
			return true;
		end

		if szText == "//reloadskill" then
			SkillMgr:reloadSkill();
			return true;
		end
		
		if string.lower(szText) == "//afk" then
			local mainplayer = ActorMgr:getMainPlayer()
			if mainplayer then
				mainplayer:setAFK(true);
			end
			return true;
		end

		if szText == "//reloadggpackage" then		
			dofile("script/GMPackage.lua");
			ShowMidTips("重新加载GM脚本成功")
			return true;
		end

		if szText == "//gm" then
			if not GMToolsFrame:IsShown() then
				GMToolsFrame:Show();
			end
			return true;
		end

		if szText == "//hidemodel" then
			local mainplayer = ActorMgr:getMainPlayer();
			mainplayer:hide( mainplayer:isShow() )
			return true;
		end

		if szText == "//showid" then
			ActorMgr:changeShowPlayerID();
			return true;
		end
		
		if szText == "//makeattach" then
			GameMgr:MakeAttach();
			return true;
		end
		
		if szText == "//makemd5" then
			GameMgr:MakeMD5();
			return true;
		end

		if szText == "//gray" then
			local main = ActorMgr:getMainPlayer();
			main:startGray();
			return true;
		end

		if szText == "//var" then
			SimulateFrame:Show();
			return true;
		end

		if szText == "//varreset" then
			SimulateFrameReset();
			return true;
		end
		local _, _, param = string.find( szText, "^//ghost%s*(.*)" )
		if param then
			if param == "close" then
				GhostSchema.exit()
				TestMode.fpsclose()
				ActorMgr:getMainPlayer():hide(false)
			else
				GhostSchema.enter(tonumber(param) or 0)
				TestMode.fpsopen()
				ActorMgr:getMainPlayer():hide(true)
			end

			return true;
		end
	end
	if ChatActionCheckString( szText ) then
		return true;
	end
end

function Private_UpdateChatRichOnSend( )	
	local curShowRichName	= GetCurShowHistoryRichText();
	UpdateHistorySliderRange( { ["name"] = curShowRichName } );
end

function ChatSendMsg(content, bSaveHistory, szInitText ) 
	-- 验证我是否可以发送
	if not t_ClientSelfChatContentParser:CanSendChatMessage( { ["text"] = content } ) then
		return;
	end

	if t_ClientSelfChatContentParser:DoClientDefineInstruction( { ["text"] = content } ) then
		return;
	end
	szText = t_ClientSelfChatContentParser:parseLinkTextForClient( { ["text"] = content } );
	t_ClientSelfChatContentParser:cachePrivatePlayerInfo();

	-- 保存客户端本地历史发送记录
	if bSaveHistory then
		t_ClientSelfChatContentParser:cacheChatHistory( { ["originalText"] = szInitText, ["parsedText"] = szText } );
	end
		
	t_ClientSelfChatContentParser:sendTextToServe( { ["text"] = szText } );

	ClearSetLink();
end

function CSChatInsert_OnTabPressed()
	local nChatType = GetChatType();
	if nChatType == CL_CHAT_PRIVATE then
		ClearFocus();
		SetFocusFrame( "ChatInsertFrame_PrivateNameEdit" );
	end
end

-- 输入聊天内容框按Enter键盘

function CSChatInsert_OnEnterPressed()	 
	if IsAccelKey() then
		SetAccelKey( false );
		return;
	end

	local frame					= getglobal( "ChatInsertFrame" );
	local edit					= getglobal( "CSChatInsert" );
	-- 如果是聊天指令则不输出
	local szInitConstantText	= edit:GetText();
	-- cAbRGBKYWL
	szInitConstantText			= string.gsub( szInitConstantText, "#[L]", "**" );
	local szText				= szInitConstantText;
	-- 第一步是要验证是否要切换频道
	szText = t_ClientSelfChatContentParser:validateNeedChangeChannel( { ["text"] = szText } );
	szText = t_ClientSelfChatContentParser:TranslateSpecialSymbol( { ["text"] = szText } );
	t_ClientSelfChatContentParser:ValidateChatItem();

	if nChatType == CL_CHAT_PRIVATE then
		local nameEdit = getglobal( "ChatInsertFrame_PrivateNameEdit" );
		t_ChatInsertFrameControl:setPrivatePlayerName( { ["name"] = nameEdit:GetText() } );
	end

	ChatSendMsg( szText, true, szInitConstantText );
	edit:Clear();
	
	Private_QuitSendChatMessage();
	ClearSetLink();
end

local CHAT_REPLACE_MODE = "#[cAbRGBKYW]";
function Private_ReplaceFunctionChar( data )
	local szText = data["text"];
	-- cAbRGBKYWL

	local szFirstReplaceResult = string.gsub( szText, "#L.*#c.*@@", function ( szDestText )
																		print( "szDestText = "..szDestText );
																		szDestText = string.gsub( szDestText, "#c", "gm" );
																		print( "szDestText = "..szDestText );
																		return szDestText;
																	end
											);
	
	szFirstReplaceResult = string.gsub( szFirstReplaceResult, "#[cAbRGBKYW]", "**" );
	szFirstReplaceResult = string.gsub( szFirstReplaceResult, "gm", "#c" );
	--return string.gsub( szText, "#[cAbRGBKYW]", "**" );
	return szFirstReplaceResult;
end

function ShortCutSendMessage( data )
	local szText	= data["text"];
	SetChatType( CL_CHAT_SAY );
	ChatSendMsg( szText, false );
end

local ADJUST_WIDTH					= 3;
local STANDARD_PRIVATE_BTN_WIDTH	= 31;
function Private_AdjustPrivateNameEditSize()
	local nameEdit		= getglobal( "ChatInsertFrame_PrivateNameEdit" );
	local szText		= nameEdit:GetText();
	local nWidth		= nameEdit:GetTextExtentWidth( szText ) / GetScreenScaleY();
	nWidth				= math.max( nWidth, STANDARD_PRIVATE_BTN_WIDTH - ADJUST_WIDTH );
	local backTex		= getglobal( "ChatInsertFrame_PrivateNameBtnBackTex" );
	
	nameEdit:SetWidth( nWidth + ADJUST_WIDTH );
	backTex:SetWidth( nWidth + ADJUST_WIDTH + 3 );

	local nChgContentWidth = nWidth + ADJUST_WIDTH + 3 - STANDARD_PRIVATE_BTN_WIDTH;
	CSChatInsert:SetWidth( EDIT_WIDTH - STANDARD_PRIVATE_BTN_WIDTH - nChgContentWidth );
	CSChatInsert:SetPoint( "left", "CurChannelBtn", "right", 5+STANDARD_PRIVATE_BTN_WIDTH+nChgContentWidth, 0 );
end

function ChatInsertFrame_PrivateNameEdit_OnFinishChar()
	Private_AdjustPrivateNameEditSize();
end

function ChatInsertFrame_PrivateNameEdit_OnEnterPressed()
	ClearFocus();
	SetFocusFrame( "CSChatInsert" );
end

-- 发送按钮
function ChatInsertFrame_SendMessageBtn_OnEnter()	
	SetGameTooltips(this:GetName(), "发送" );
end

-- 颜色设置按钮
function ChatInsertFrame_SetTextColorBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer and mainplayer:isColorFontValid() <= 0 then
		return
	end
	local frame = getglobal( "ChatTextColorSettingFrame" );
	if frame:IsShown() then
		frame:Hide();
	else
		frame:Show();
	end
end

function ChatInsertFrame_SetTextColorBtn_OnEnter()
	local mainplayer = ActorMgr:getMainPlayer()
	local colorFontValidEnd = mainplayer:isColorFontValid()
	if colorFontValidEnd <= 0 then
		SetGameTooltips( this:GetName(),"点击打开文字颜色设置面板（此功能需要在商城购买多彩字体来解锁）" )
	else
		SetGameTooltips(this:GetName(), "点击打开文字颜色设置面板" .. "\n多彩字体剩余" .. math.ceil( colorFontValidEnd/(24*60*60) )  .. "天");
	end
	
end

-- 笑脸按钮
function ChatInsertFrame_FaceBtn_OnClick()
	local frame = getglobal( "ChatFaceAndActionFrame" );
	if frame:IsShown() then
		frame:Hide( );
	else
		frame:Show();
		SetFocusFrame( "CBFChatInsert" );
	end
end

function ChatInsertFrame_FaceBtn_OnEnter()
	SetGameTooltips(this:GetName(), "点击打开表情动作面板" );
end

-- PRIVATE
-- 切换输入框UI
function Private_UpdateChatInputFrameUI()
	local nChatType			= GetChatType();
	local privateNameBtn	= getglobal( "ChatInsertFrame_PrivateNameBtn" );
	local privateNameEdit	= getglobal( "ChatInsertFrame_PrivateNameEdit" );
	local edit				= getglobal( "CSChatInsert" );
	
	if nChatType == CL_CHAT_PRIVATE then
		privateNameBtn:Show();
		privateNameEdit:Show();
		Private_AdjustPrivateNameEditSize();
	else
		privateNameBtn:Hide();
		privateNameEdit:Hide();
		edit:SetWidth( EDIT_WIDTH );
		edit:SetPoint( "left", "CurChannelBtn", "right", 5, 0 );
	end
end

-- 结束输入聊天内容
function Private_QuitSendChatMessage()
	CSChatInsert:Clear();
	ClearChatInsertFram();
	SetFocusFrame("UIClient");
end

-- 频道选择

local t_channelMenuOp;
local t_channelMenuNormalOp 	= { "附近	/f", "队伍	/p", "私聊	/m", "公会	/g", "战团	/z", "世界	/w", "喇叭	/l", };
local t_channelMenuSpanOp 	= { "附近	/f", "队伍	/p", "私聊	/m", "战团	/z", "喇叭	/l", "本服	/s", "跨服	/k", };

local MAX_CHANNEL_MENU_BUTTON = 12;
function ClearChatInsertChannelMenuFrame()
	for i = 1, MAX_CHANNEL_MENU_BUTTON do
		local button = getglobal( "ChatInsertChannelMenuFrame_RightBtn" .. i );
		button:Hide();

		local font		= getglobal( button:GetName() .. "Font" );
		font:SetTextColor( 145, 125, 55 );
	end
end

function ChatInsertChannelMenuFrame_OnShow()
	local nWidth		= 50;
	local t_NameList	= GetPrivatePlayerNameList();
	for i = 1, table.getn( t_NameList ) do
		local nNameWidth = GameTooltipRichText:GetTextExtentWidth( t_NameList[i] );
		if nWidth < nNameWidth then
			nWidth = nNameWidth;
		end
	end

	nWidth = nWidth + 15;

	ClearChatInsertChannelMenuFrame();
	
	local mainplayer 	= ActorMgr:getMainPlayer();
	if mainplayer then
		if mainplayer:isInPVPMap() then
			t_channelMenuOp = t_channelMenuSpanOp;
		else
			t_channelMenuOp = t_channelMenuNormalOp;
		end
	else
		t_channelMenuOp = t_channelMenuNormalOp;
	end
	
	local nBtnIndex		= 1;
	for i = 1, table.getn( t_channelMenuOp ) do
		local button	= getglobal( "ChatInsertChannelMenuFrame_RightBtn"..nBtnIndex );
		local font		= getglobal( button:GetName() .. "Font" );
		
		font:SetText( t_channelMenuOp[i] );
		button:SetWidth( nWidth );
		button:SetNormalRegionAbsWidth( nWidth );
		button:SetOverlayRegionAbsWidth( nWidth );
		button:Show();
		nBtnIndex = nBtnIndex + 1;
	end

	if GameBattle:isRun() then
		local button	= getglobal( "ChatInsertChannelMenuFrame_RightBtn"..nBtnIndex );
		local font		= getglobal( button:GetName() .. "Font" );
		font:SetText( "战场	/b" );
		button:SetWidth( nWidth );
		button:SetNormalRegionAbsWidth( nWidth );
		button:SetOverlayRegionAbsWidth( nWidth );
		button:Show();
		nBtnIndex = nBtnIndex + 1;
	end
	
	for i = 1, table.getn( t_NameList )  do
		local button	= getglobal( "ChatInsertChannelMenuFrame_RightBtn"..nBtnIndex );
		local font		= getglobal( button:GetName() .. "Font" );
		
		font:SetText( t_NameList[i] );
		button:SetWidth( nWidth );
		button:SetNormalRegionAbsWidth( nWidth );
		button:SetOverlayRegionAbsWidth( nWidth );
		button:Show();
		nBtnIndex = nBtnIndex + 1;
	end

	
	this:SetSize( nWidth + 5, nBtnIndex * 16 + ( nBtnIndex - 1 ) * 4 - 3 );
	this:SetPoint("BottomLeft", this:GetClientString(), "TopLeft", 0, 0);
	SetFocusFrame( this:GetName() );
end

function ChatInsertChannelMenuFrame_RightBtn_OnEnter()
	local font = getglobal( this:GetName() .. "Font" );
	font:SetTextColor( 255, 254, 189 );
end

function ChatInsertChannelMenuFrame_RightBtn_OnLeave()
	local font = getglobal( this:GetName() .. "Font" );
	font:SetTextColor( 145, 125, 55 );
end

function ChatInsertChannelMenuFrame_RightBtn_OnMouseDown()
	local font			= getglobal( this:GetName().."Font" );
	local t_NameList	= GetPrivatePlayerNameList();
	local nLastChatType	= GetChatType();
	local t_ChatInsertFrameControl = GetChatInsertFrameControl();

	--如果是附近
	if ( string.find( font:GetText(), "附近", 1, true ) ) then
		SetChatType( CL_CHAT_SAY );	
	--如果是队伍
	elseif ( string.find( font:GetText(), "队伍", 1, true ) ) then
		SetChatType( CL_CHAT_TEAM ); 
	--如果是私聊
	elseif ( string.find( font:GetText(), "私聊", 1, true ) ) then
		SetChatType( CL_CHAT_PRIVATE );		 
	--如果是公会
	elseif ( string.find( font:GetText(), "公会", 1, true ) ) then
		SetChatType( CL_CHAT_CLAN );
	elseif ( string.find( font:GetText(), "战团", 1, true ) ) then
		SetChatType( CL_CHAT_CLAN_GROUP );   
	--如果是世界
	elseif ( string.find( font:GetText(), "世界", 1, true ) ) then
		SetChatType( CL_CHAT_WORLD );
	elseif ( string.find( font:GetText(), "本服", 1, true ) ) then
		SetChatType( CL_CHAT_SPAN_MAP_WORLD );
	elseif ( string.find( font:GetText(), "跨服", 1, true ) ) then
		SetChatType( CL_CHAT_SPAN_MAP );
	elseif ( string.find( font:GetText(), "战场", 1, true ) ) then
		SetChatType( CL_CHAT_BATTLE );
	elseif ( string.find( font:GetText(), "喇叭", 1, true ) ) then	
		if IsSetLaBaItem() then
			local t_chatItem = GetLaBaItem();
			ChatBugleFrame:SetClientUserData( 0, t_chatItem["list"] );
			ChatBugleFrame:SetClientUserData( 1, t_chatItem["grid"] );
		end
		SetChatType( CL_CHAT_ITEM );
		if IsHaveLaBaItem() then
			local laBaItem = GetContainerOneLaBaItem();
			SetLaBaItem( { grid = laBaItem:getGridIdx(), list = laBaItem:getListType() } );
		else
			SetLaBaItem( { grid = -1, list = -1 } );
		end

	elseif font:GetText() == t_NameList[1] then
		t_ChatInsertFrameControl:setPrivatePlayerName( { ["name"] = t_NameList[1] } );
		SetChatType( CL_CHAT_PRIVATE );
	elseif font:GetText() == t_NameList[2] then
		t_ChatInsertFrameControl:setPrivatePlayerName( { ["name"] = t_NameList[2] } );
		SetChatType( CL_CHAT_PRIVATE );		  
	elseif font:GetText() == t_NameList[3] then
		t_ChatInsertFrameControl:setPrivatePlayerName( { ["name"] = t_NameList[3] } );
		SetChatType( CL_CHAT_PRIVATE );	 
	end
	ChatInsertChannelMenuFrame:Hide();
	ChatInsertFrame_OnShow();
	local nCurChatType = GetChatType();
	if nLastChatType == CL_CHAT_PRIVATE and nCurChatType ~= CL_CHAT_PRIVATE then
		CSChatInsert:SetText( CSChatInsert:GetText() );
	end

	SetFocusFrame("CSChatInsert");
end

-- 频道选择


-- 小喇叭输入框
-- 小喇叭
local t_laBaItem = { grid = -1, list = -1 };
function GetLaBaItem()
	return t_laBaItem;
end

function SetLaBaItem( data )
	t_laBaItem = data;
end

function IsSetLaBaItem()
	return t_laBaItem["list"] ~= -1;
end

function ChatBugleFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 0, 0 );
	CBFChatInsert:Clear();
	SetFocusFrame("CBFChatInsert");
	this:SetClientString("");
end

-- 小喇叭设置窗口关闭时清空超链接信息
function ChatBugleFrame_OnHide()
	local mainplayer	= ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local container		= mainplayer:getContainer();

	local t_laBaItem = GetLaBaItem();
	if t_laBaItem["grid"] ~= -1 then
		local oldItem		= container:getItem( t_laBaItem["list"], t_laBaItem["grid"] );
		UnLockItem( t_laBaItem["list"], t_laBaItem["grid"], oldItem:getItemId() );
	end
	
	if this:GetClientUserData( 0 ) ~= 0 then
		local oldItem		= container:getItem( this:GetClientUserData( 0 ), this:GetClientUserData( 1 ) );
		UnLockItem( this:GetClientUserData( 0 ), this:GetClientUserData( 1 ), oldItem:getItemId() );
		this:SetClientUserData( 0, 0 );
		this:SetClientUserData( 1, 0 );
	end

	SetLaBaItem( { grid = -1, list = -1 } )
	ClearSetLink();

	local t_hideIcons = { "ChatFaceFrame" };
	util.HideIcons( t_hideIcons );

	CBFChatInsert:Clear();
end

-- 小喇叭面板的表情按纽
function IsHaveLaBaItem()
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local containitem	= GetContainerHaveItemResult( CONTAINER_TYPE_SUNDRIES, RESULT_ITEM_CHAT );

	if containitem == nil then
		return;
	end

	return containitem ~= nil;
end

function IsHaveChatItem()
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local containitem	= nil;

	local t_laBaItem = GetLaBaItem();
	local resultId = 0;
	if t_laBaItem["grid"] ~= -1 then
		if ChatBugleFrame:GetClientUserData( 0 ) == 0 then
			local item	= container:getItem( t_laBaItem["list"], t_laBaItem["grid"] );
			resultId	= item:getItemDef().Result[0].ResultID;
			if resultId ~= 0 then
				containitem	= GetContainerHaveItemResult( CONTAINER_TYPE_SUNDRIES, resultId );
			end
		else
			local item	= container:getItem( ChatBugleFrame:GetClientUserData( 0 ), ChatBugleFrame:GetClientUserData( 1 ) );
			resultId	= item:getItemDef().Result[0].ResultID;
			if resultId ~= 0 then
				containitem	= GetContainerHaveItemResult( CONTAINER_TYPE_SUNDRIES, resultId );
				SetLaBaItem( { list = ChatBugleFrame:GetClientUserData( 0 ), grid = ChatBugleFrame:GetClientUserData( 1 ) } );
			end
		end
	end

	if containitem == nil then
		return;
	end

	return containitem ~= nil and resultId or false;
end

function GetContainerOneLaBaItem()
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local containitem	= GetContainerHaveItemResult( CONTAINER_TYPE_SUNDRIES, RESULT_ITEM_CHAT );
	return containitem;
end

function CBFFaceBtn_OnClick()
	if not IsHaveChatItem() then
		return ShowMidTips("缺少小喇叭道具！");
	end

	local frame = getglobal( "ChatFaceAndActionFrame" );
	if frame:IsShown() then
		frame:Hide( );
	else
		frame:Show();
		SetFocusFrame("CBFChatInsert");
	end
end

local t_ItemChatType = { [RESULT_ITEM_CHAT] = CL_CHAT_ITEM, [RESULT_SPAN_CHAT] = CL_CHAT_SPAN, };
--小喇叭发送消息
function CBFSendBtn_OnClick()
	local content = ChatManager:strTrim( CBFChatInsert:GetText() );
	if (content ~= "") then
		local chatItem = IsHaveChatItem();
		if not chatItem then
			return ShowMidTips("缺少小喇叭道具！");
		end

		SetChatType( t_ItemChatType[chatItem] );
		Private_UpdateChatInputFrameUI();
		local szText = CBFChatInsert:GetText();
		local szLink = ChatBugleFrame:GetClientString();
		if string.find( szLink, "查看店铺", 1, true ) then
			szText = szLink .. szText;
		end
		ChatSendMsg( szText, false);
		CBFChatInsert:Clear();
		SetFocusFrame("UIClient");
	end
end

function Accelkey_AltGroup( szFrame, nIndex )
	if  not ChatInsertFrame:IsShown() then
		return;
	end

	if szFrame ~= ( "CSChatInsert" ) or nIndex > 54 then
		return;
	end

	nIndex = nIndex - 48 ;

	local edit = getglobal("CSSMFInsertEdit"..nIndex);
	if edit == nil or edit:GetText() == ""  then
		return;
	end

	if ChatShortcutSendMsgFrame:IsShown() then
		ShowMidTips("请先保存快捷发言");
		return;
	end

	local szSrcText = edit:GetText();

	if CSChatInsert:IsAnyTextSelect() then
		Private_ReplaceTextByAccelAlt( { ["text"] = szSrcText } );
	else
		Private_AddTextByAccelAlt( { ["text"] = szSrcText } )
	end
	
	SetFocusFrame("CSChatInsert");
end

function Private_ReplaceTextByAccelAlt( data )
	local szNewText			= data["text"];
	local edit				= getglobal( "CSChatInsert" );
	local szCurAllEditText	= edit:GetText();
	
	local nSelectTextLen	= edit:GetSelctTexLen();
	local nTotalTextLen		= string.len( szCurAllEditText );
	-- 获取原来字符串被保留的字符数目
	local nLevaePreTextLen	= nTotalTextLen - nSelectTextLen;

	local nReplaceLen			= string.len( szNewText );
	-- 获得来源替换字符的长度	
	local nResultSrcReplaceLen	= ( MAX_CHATINSERT_CHAR - nLevaePreTextLen ) - nReplaceLen;
	if nResultSrcReplaceLen < 0 then
		nReplaceLen = nReplaceLen + nResultSrcReplaceLen;
		szNewText = string.sub( szNewText, 1, nReplaceLen );
	end
	
	edit:ReplaceSelectText( szNewText );
end

function Private_AddTextByAccelAlt( data )
	local szNewText = data["text"];
	CSChatInsert:AddText( szNewText );
end

function InitInputFrameData()
	local t_edits = { "CSChatInsert", "CBFChatInsert", "ChatInsertFrame_PrivateNameEdit" };
	for _, name in ipairs( t_edits ) do
		local edit = getglobal( name );
		edit:Clear();
		edit:clearHistory();
	end
	tSelectChatColor = nil;
	CSChatInsert:SetTextColor( 255,255,255 )
end

-- 字体颜色选择面板
local t_btnMapToTextColor =
{
["ChatTextColorSettingFrame_SystemDefaultColorBtn"] = { ["color"] = nil, ["rgb"] = { 255, 255, 255 } },
["ChatTextColorSettingFrame_BlueColorBtn"]			= { ["color"] = "#c007DFF", ["rgb"] = { 0, 125, 255 } },
["ChatTextColorSettingFrame_RedColorBtn"]			= { ["color"] = "#cFF0000", ["rgb"] = { 255, 0, 0 } },
["ChatTextColorSettingFrame_OrangeColorBtn"]		= { ["color"] = "#cFF7D00", ["rgb"] = { 255, 125, 0 } },
["ChatTextColorSettingFrame_PurpleColorBtn"]		= { ["color"] = "#c7D00FF", ["rgb"] = { 125, 0, 255 } },
["ChatTextColorSettingFrame_GreenColorBtn"]			= { ["color"] = "#c00FF00", ["rgb"] = { 0, 255, 0 } },
["ChatTextColorSettingFrame_GrayColorBtn"]			= { ["color"] = "#cA0A0A0", ["rgb"] = { 160, 160, 160 } },
};

function ChatTextColorSettingFrame_OpBtn_OnClick()
	local t_oneColorConfig = t_btnMapToTextColor[this:GetName()];
	CSChatInsert:SetTextColor( t_oneColorConfig["rgb"][1], t_oneColorConfig["rgb"][2], t_oneColorConfig["rgb"][3] )
	tSelectChatColor = t_oneColorConfig["color"];
	ChatTextColorSettingFrame:Hide();
	SetFocusFrame( "CSChatInsert" );
end

function ChatTextColorSettingFrame_OnShow()
	this:SetPoint( "BottomRight", "ChatInsertFrame_SetTextColorBtn", "TopRight", 0, -2 );
end
