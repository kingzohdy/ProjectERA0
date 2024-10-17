-- 聊天记录面板
local t_ChatHistoryMsgMgrFrameControl = { ["pageType"] = CL_CHAT_SYS_MSG };

local t_chatTypeMapToHistroyRich = 
{
[CL_CHAT_SYS_MSG]	= { ["rich"] = "ChatHistoryMsgMgrFrame_SystemRich",	["lableBtn"] = "ChatHistoryMsgMgrFrame_SystemBtn" },
[CL_CHAT_SAY]		= { ["rich"] = "ChatHistoryMsgMgrFrame_NearRich",	["lableBtn"] = "ChatHistoryMsgMgrFrame_NearBtn" },
[CL_CHAT_WORLD]		= { ["rich"] = "ChatHistoryMsgMgrFrame_WorldRich",	["lableBtn"] = "ChatHistoryMsgMgrFrame_WorldBtn" },
[CL_CHAT_CLAN]		= { ["rich"] = "ChatHistoryMsgMgrFrame_ClanRich",	["lableBtn"] = "ChatHistoryMsgMgrFrame_ClanBtn" },
[CL_CHAT_CLAN_GROUP]	= { ["rich"] = "ChatHistoryMsgMgrFrame_ClanGroupRich",	["lableBtn"] = "ChatHistoryMsgMgrFrame_ClanGroupBtn" },
[CL_CHAT_TEAM]		= { ["rich"] = "ChatHistoryMsgMgrFrame_TeamRich",	["lableBtn"] = "ChatHistoryMsgMgrFrame_TeamBtn" },
[CL_CHAT_PRIVATE]	= { ["rich"] = "ChatHistoryMsgMgrFrame_PrivateRich",	["lableBtn"] = "ChatHistoryMsgMgrFrame_PrivateBtn" },
[CL_CHAT_ITEM]		= { ["rich"] = "ChatHistoryMsgMgrFrame_BugleRich",	["lableBtn"] = "ChatHistoryMsgMgrFrame_BugleBtn" },
[CL_CHAT_MSG]		= { ["rich"] = "ChatHistoryMsgMgrFrame_MsgRich",	["lableBtn"] = "ChatHistoryMsgMgrFrame_MsgBtn" },
};

function GetChatHistoryMsgMgrFrameControl()
	return t_ChatHistoryMsgMgrFrameControl;
end

t_ChatHistoryMsgMgrFrameControl["init"] =
function ( self )
	self["pageType"] = CL_CHAT_SYS_MSG;
end

t_ChatHistoryMsgMgrFrameControl["setChatTypeByBtn"] =
function ( self, data )
	local btnName = data["name"];
	for chatType, data in pairs( t_chatTypeMapToHistroyRich ) do
		if btnName == data["lableBtn"] then
			self["pageType"] = chatType;
			return;
		end
	end
	
	self["pageType"] = CL_CHAT_SYS_MSG;
end

t_ChatHistoryMsgMgrFrameControl["getChatTypeByBtn"] =
function ( self, data )
	local btnName = data["name"];
	for chatType, data in pairs( t_chatTypeMapToHistroyRich ) do
		if btnName == data["lableBtn"] then
			return chatType;
		end
	end
	
	return CL_CHAT_SYS_MSG;
end

t_ChatHistoryMsgMgrFrameControl["getChatTypeByRich"] =
function ( self, data )
	local richName = data["name"];
	for chatType, data in pairs( t_chatTypeMapToHistroyRich ) do
		if richName == data["rich"] then
			return chatType;
		end
	end
	
	return CL_CHAT_SYS_MSG;
end

local t_ChatHistoryMsgMgrFrameEvents = 
{ 
	["GE_ENTER_PLAYERLOGIN"] = {},
};

t_ChatHistoryMsgMgrFrameEvents["GE_ENTER_PLAYERLOGIN"].func = 
function ()
	t_ChatHistoryMsgMgrFrameControl:init();
end

function UpdateChatHistoryMsgMgrFrameOnMainplayerDestroy()
	for _, data in pairs( t_chatTypeMapToHistroyRich ) do
		local rich = getglobal( data["rich"] );
		rich:Clear();
		rich:clearHistory();
	end
	t_ChatHistoryMsgMgrFrameControl:init();
end

function ChatHistoryMsgMgrFrame_OnLoad()
	for event, _ in pairs( t_ChatHistoryMsgMgrFrameEvents ) do
		this:RegisterEvent( event );
	end

	t_ChatHistoryMsgMgrFrameEvents.__index = function ( tab, key )
									tab[key] = { func = function () end };
									return tab[key];
								end
	setmetatable( t_ChatHistoryMsgMgrFrameEvents, t_ChatHistoryMsgMgrFrameEvents );
end

function ChatHistoryMsgMgrFrame_OnEvent()
	t_ChatHistoryMsgMgrFrameEvents[arg1].func();
end

function ChatHistoryMsgMgrFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 0, 0 );
	ClearChatHistoryMsgMgrFrame();
	local nCurPageType	= t_ChatHistoryMsgMgrFrameControl["pageType"];

	local rich			= getglobal( t_chatTypeMapToHistroyRich[nCurPageType]["rich"] );
	rich:Show();

	local btn = getglobal( t_chatTypeMapToHistroyRich[nCurPageType]["lableBtn"] );
	btn:Checked();
	
	local slider = getglobal("ChatHistoryMsgMgrFrame_ScrollBar");
	Private_UpdateHistorySlider();	
	util.UpdateRich( rich:GetName(), slider:GetValue() );
end

function ChatHistoryMsgMgrFrame_OnHide()
	if LinkItemTipsFrame:IsShown() then
		if IsInFrameAbs( "LinkItemTipsFrame", "ChatHistoryMsgMgrFrame" ) then
			LinkItemTipsFrame:Hide();
		end
	end
end

function ClearChatHistoryMsgMgrFrame()
	for _, data in pairs( t_chatTypeMapToHistroyRich ) do
		local rich = getglobal( data["rich"] );
		rich:Hide();
	end
	
	for _, data in pairs( t_chatTypeMapToHistroyRich ) do
		local btn = getglobal( data["lableBtn"] );
		btn:DisChecked();
	end
end

-- 聊天记录类型按钮
function ChatHistoryMsgMgrFrame_ChatTypeBtn_OnClick()
	ClearChatHistoryMsgMgrFrame();
	this:Checked();
	t_ChatHistoryMsgMgrFrameControl:setChatTypeByBtn( { ["name"] = this:GetName() } );
	local nCurChatType = t_ChatHistoryMsgMgrFrameControl:getChatTypeByBtn( { ["name"] = this:GetName() } );
	local curShowRich = getglobal( t_chatTypeMapToHistroyRich[nCurChatType]["rich"] );
	curShowRich:Show();

	Private_UpdateHistorySlider();	
end

function ChatHistoryMsgMgrFrame_ClearBtn_OnClick()
	local nCurPageType = t_ChatHistoryMsgMgrFrameControl["pageType"];
	local rich = getglobal( t_chatTypeMapToHistroyRich[nCurPageType]["rich"] );
	rich:Clear();
	rich:clearHistory();
end

-- CORE
function GetCurShowHistoryRichText()
	local nCurHistoryType = t_ChatHistoryMsgMgrFrameControl["pageType"];
	return t_chatTypeMapToHistroyRich[nCurHistoryType]["rich"];
end

function UpdateHistorySliderRange( data )
	local chgRichName		= data["name"];
	local nCurHistoryType	= t_ChatHistoryMsgMgrFrameControl["pageType"];
	local rich				= getglobal( t_chatTypeMapToHistroyRich[nCurHistoryType]["rich"] );
	if rich:GetName() ~= chgRichName then
		return;
	end
	
	SetScrollBar( "ChatHistoryMsgMgrFrame_ScrollBar", rich:GetAccurateViewLines(), rich:GetTextLines() );

	local slider	 = getglobal( "ChatHistoryMsgMgrFrame_ScrollBar" );
	local nValue	 = slider:GetValue();
	local nMaxValue	 = slider:GetMaxValue();

	if nValue <= nMaxValue then
		util.UpdateRich( rich:GetName(), nValue );
	end
end
-- CORE

function Private_UpdateHistorySlider()
	local rich = getglobal( GetCurShowHistoryRichText() );
	local slider		= getglobal("ChatHistoryMsgMgrFrame_ScrollBar");
	slider:SetValue( 0 );
	SetScrollBar( slider:GetName(), rich:GetAccurateViewLines(), rich:GetTextLines() );	
end

--聊天记录点击向上
function ChatHistoryMsgMgrFrame_ScrollUpBtn_OnClick()
	local slider = getglobal( "ChatHistoryMsgMgrFrame_ScrollBar" );
	if not slider:IsShown() then
		return;
	end

	local nValue = slider:GetValue() - slider:GetValueStep();
	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end

--聊天记录点击向下
function ChatHistoryMsgMgrFrame_ScrollDownBtn_OnClick()
	local slider	= getglobal( "ChatHistoryMsgMgrFrame_ScrollBar" );
	if not slider:IsShown() then
		return;
	end

	local nMaxValue = slider:GetMaxValue();
	local nValue	= slider:GetValue() + slider:GetValueStep();
	
	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

--聊天记录拖动条变化值
function ChatHistoryMsgMgrFrame_ScrollBar_OnValueChanged()
	local slider	 = getglobal( "ChatHistoryMsgMgrFrame_ScrollBar" );
	local nValue	 = slider:GetValue();
	local nMaxValue	 = slider:GetMaxValue();
	local rich		 = getglobal( GetCurShowHistoryRichText() );
	if nValue <= nMaxValue then
		util.UpdateRich( rich:GetName(), nValue );
	end
end