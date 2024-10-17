-- 新的频道屏蔽设置面板
local t_ChatPingBiSettingFrameEvents = 
{ 
	["GE_ENTER_PLAYERLOGIN"] = {},
};

t_ChatPingBiSettingFrameEvents["GE_ENTER_PLAYERLOGIN"].func = 
function ()
	ChatManager:setNearChannelPingBiOp( CL_CHAT_SPAN_MAP_WORLD, false );
	ChatManager:setNearChannelPingBiOp( CL_CHAT_SPAN_MAP, false );
	if not GameWizard:isDo(GAMEWIZARD_FIRST_SET_NEW_CHAT_PING_BI) then
		Private_SetAllDefaultPingBiOps();
		GameWizard:doStep( GAMEWIZARD_FIRST_SET_NEW_CHAT_PING_BI );
	end
	if not GameWizard:isDo(GAMEWIZARD_FIRST_SET_CLAN_GROUP_PINGBI) then
		ChatManager:setClanChannelPingBiOp( CL_CHAT_CLAN_GROUP, false );
		ChatManager:savePingBiClanChannelsOp();
		ChatManager:setNearChannelPingBiOp( CL_CHAT_BATTLE, false );
		ChatManager:setNearChannelPingBiOp( CL_CHAT_SPAN_MAP_WORLD, false );
		ChatManager:setNearChannelPingBiOp( CL_CHAT_SPAN_MAP, false );
		ChatManager:savePingBiNearChannelsOp();
		ChatManager:setZongHeChannelPingBiOp( CL_CHAT_SYS_MSG, false );
		ChatManager:savePingBiZongHeChannelsOp();
		GameWizard:doStep( GAMEWIZARD_FIRST_SET_CLAN_GROUP_PINGBI );
	end
end

local t_pageTypeBtnMapToPingOp = 
{
[CL_CHAT_ZONG_HE_MSG]	= { ["btn"] = "ChatPingBiSettingFrame_ZongHePageButton"		},
[CL_CHAT_WORLD]			= { ["btn"] = "ChatPingBiSettingFrame_WorldPageButton"		},
[CL_CHAT_CLAN]			= { ["btn"] = "ChatPingBiSettingFrame_ClanPageButton"		},
[CL_CHAT_TEAM]			= { ["btn"] = "ChatPingBiSettingFrame_TeamPageButton"		},
[CL_CHAT_SAY]			= { ["btn"] = "ChatPingBiSettingFrame_NearPageButton"		},
[CL_CHAT_PRIVATE]		= { ["btn"] = "ChatPingBiSettingFrame_PrivatePageButton"	},
};

local t_chkMapToChatPingBiOp = 
{
[CL_CHAT_SYS_MSG]	= { ["chkBtn"] = "ChatPingBiSettingFrameSystemChkBtn",		["btn"] = "ChatPingBiSettingFrameSystemHeadBtn"			},
[CL_CHAT_TEAM]		= { ["chkBtn"] = "ChatPingBiSettingFrameTeamChkBtn",		["btn"] = "ChatPingBiSettingFrameTeamHeadBtn"			},
[CL_CHAT_WORLD]		= { ["chkBtn"] = "ChatPingBiSettingFrameWorldChkBtn",		["btn"] = "ChatPingBiSettingFrameWorldHeadBtn"			},
[CL_CHAT_SAY]		= { ["chkBtn"] = "ChatPingBiSettingFrameNearChkBtn",		["btn"] = "ChatPingBiSettingFrameNearHeadBtn"			},
[CL_CHAT_CLAN]		= { ["chkBtn"] = "ChatPingBiSettingFrameClanChkBtn",		["btn"] = "ChatPingBiSettingFrameClanHeadBtn"			},
[CL_CHAT_PRIVATE]	= { ["chkBtn"] = "ChatPingBiSettingFramePrivateChatChkBtn",	["btn"] = "ChatPingBiSettingFramePrivateChatHeadBtn"		},
[CL_CHAT_BATTLE]	= { ["chkBtn"] = "ChatPingBiSettingFrameBattleChkBtn",		["btn"] = "ChatPingBiSettingFrameBattleHeadBtn"			},
[CL_CHAT_SPAN_MAP_WORLD]	= { ["chkBtn"] = "ChatPingBiSettingFrameBattleChkBtn",		["btn"] = "ChatPingBiSettingFrameBattleHeadBtn"			},
[CL_CHAT_SPAN_MAP]	= { ["chkBtn"] = "ChatPingBiSettingFrameBattleChkBtn",		["btn"] = "ChatPingBiSettingFrameBattleHeadBtn"			},
[CL_CHAT_CLAN_GROUP]	= { ["chkBtn"] = "ChatPingBiSettingFrameClanGroupChkBtn",	["btn"] = "ChatPingBiSettingFrameClanGroupHeadBtn"		},
[CL_CHAT_WARN_NEW_MESSAGE]	= { ["chkBtn"] = "ChatPingBiSettingFrameWarnNewMessageChkBtn",	
								["btn"] = "ChatPingBiSettingFrameWarnNewMessageHeadBtn"	},
};


local t_ChatPingBiSettingFrameControl = 
{ 
["pageType"] = CL_CHAT_ZONG_HE_MSG,

["allOption"] = 
{
-- 综合
[CL_CHAT_ZONG_HE_MSG] = {	[CL_CHAT_SYS_MSG]	= { ["isPingBi"] = false },		[CL_CHAT_TEAM]		= { ["isPingBi"] = false },
							[CL_CHAT_WORLD]		= { ["isPingBi"] = false },		[CL_CHAT_SAY]		= { ["isPingBi"] = false },
							[CL_CHAT_CLAN]		= { ["isPingBi"] = false },		[CL_CHAT_PRIVATE]	= { ["isPingBi"] = false },
							[CL_CHAT_BATTLE]	= { ["isPingBi"] = false },		[CL_CHAT_CLAN_GROUP]	= { ["isPingBi"] = false },
							[CL_CHAT_WARN_NEW_MESSAGE]	= { ["isPingBi"] = false },
							[CL_CHAT_SPAN_MAP_WORLD]		= { ["isPingBi"] = false },
							[CL_CHAT_SPAN_MAP]	= { ["isPingBi"] = false },
						},

-- 世界
[CL_CHAT_WORLD]		=	{	[CL_CHAT_SYS_MSG]	= { ["isPingBi"] = false },		[CL_CHAT_TEAM]		= { ["isPingBi"] = false },
							[CL_CHAT_WORLD]		= { ["isPingBi"] = false },		[CL_CHAT_SAY]		= { ["isPingBi"] = false },
							[CL_CHAT_CLAN]		= { ["isPingBi"] = false },		[CL_CHAT_PRIVATE]	= { ["isPingBi"] = false },
							[CL_CHAT_BATTLE]	= { ["isPingBi"] = false },		[CL_CHAT_CLAN_GROUP]	= { ["isPingBi"] = false },
							[CL_CHAT_WARN_NEW_MESSAGE]	= { ["isPingBi"] = false },
							[CL_CHAT_SPAN_MAP_WORLD]		= { ["isPingBi"] = false },
							[CL_CHAT_SPAN_MAP]	= { ["isPingBi"] = false },
						},

-- 公会
[CL_CHAT_CLAN]		=	{	[CL_CHAT_SYS_MSG]	= { ["isPingBi"] = false },		[CL_CHAT_TEAM]		= { ["isPingBi"] = false },
							[CL_CHAT_WORLD]		= { ["isPingBi"] = false },		[CL_CHAT_SAY]		= { ["isPingBi"] = false },
							[CL_CHAT_CLAN]		= { ["isPingBi"] = false },		[CL_CHAT_PRIVATE]	= { ["isPingBi"] = false },
							[CL_CHAT_BATTLE]	= { ["isPingBi"] = false },		[CL_CHAT_CLAN_GROUP]	= { ["isPingBi"] = false },
							[CL_CHAT_WARN_NEW_MESSAGE]	= { ["isPingBi"] = false },
							[CL_CHAT_SPAN_MAP_WORLD]		= { ["isPingBi"] = false },
							[CL_CHAT_SPAN_MAP]	= { ["isPingBi"] = false },
						},

-- 队伍
[CL_CHAT_TEAM]		=	{	[CL_CHAT_SYS_MSG]	= { ["isPingBi"] = false },		[CL_CHAT_TEAM]		= { ["isPingBi"] = false },
							[CL_CHAT_WORLD]		= { ["isPingBi"] = false },		[CL_CHAT_SAY]		= { ["isPingBi"] = false },
							[CL_CHAT_CLAN]		= { ["isPingBi"] = false },		[CL_CHAT_PRIVATE]	= { ["isPingBi"] = false },
							[CL_CHAT_BATTLE]	= { ["isPingBi"] = false },		[CL_CHAT_CLAN_GROUP]	= { ["isPingBi"] = false },
							[CL_CHAT_WARN_NEW_MESSAGE]	= { ["isPingBi"] = false },
							[CL_CHAT_SPAN_MAP_WORLD]		= { ["isPingBi"] = false },
							[CL_CHAT_SPAN_MAP]	= { ["isPingBi"] = false },
						},

-- 附近
[CL_CHAT_SAY]		=	{	[CL_CHAT_SYS_MSG]	= { ["isPingBi"] = false },		[CL_CHAT_TEAM]		= { ["isPingBi"] = false },
							[CL_CHAT_WORLD]		= { ["isPingBi"] = false },		[CL_CHAT_SAY]		= { ["isPingBi"] = false },
							[CL_CHAT_CLAN]		= { ["isPingBi"] = false },		[CL_CHAT_PRIVATE]	= { ["isPingBi"] = false },
							[CL_CHAT_BATTLE]	= { ["isPingBi"] = false },		[CL_CHAT_CLAN_GROUP]	= { ["isPingBi"] = false },
							[CL_CHAT_WARN_NEW_MESSAGE]	= { ["isPingBi"] = false },
							[CL_CHAT_SPAN_MAP_WORLD]		= { ["isPingBi"] = false },
							[CL_CHAT_SPAN_MAP]	= { ["isPingBi"] = false },
						},

-- 私聊
[CL_CHAT_PRIVATE]	=	{	[CL_CHAT_SYS_MSG]	= { ["isPingBi"] = false },		[CL_CHAT_TEAM]		= { ["isPingBi"] = false },
							[CL_CHAT_WORLD]		= { ["isPingBi"] = false },		[CL_CHAT_SAY]		= { ["isPingBi"] = false },
							[CL_CHAT_CLAN]		= { ["isPingBi"] = false },		[CL_CHAT_PRIVATE]	= { ["isPingBi"] = false },
							[CL_CHAT_BATTLE]	= { ["isPingBi"] = false },		[CL_CHAT_CLAN_GROUP]	= { ["isPingBi"] = false },
							[CL_CHAT_WARN_NEW_MESSAGE]	= { ["isPingBi"] = false },
							[CL_CHAT_SPAN_MAP_WORLD]		= { ["isPingBi"] = false },
							[CL_CHAT_SPAN_MAP]	= { ["isPingBi"] = false },
						},
} 

};

function GetChatPingBiSettingFrameControl()
	return t_ChatPingBiSettingFrameControl;
end

t_ChatPingBiSettingFrameControl["initAllOptionList"] =
function ( self )
	self:initZongHeOptionList();
	self:initClanOptionList();
	self:initWorldOptionList();
	self:initTeamOptionList();
	self:initNearOptionList();
	self:initPrivateOptionList();
end

t_ChatPingBiSettingFrameControl["initZongHeOptionList"] =
function ( self )
	local t_zongHeOptionList = self["allOption"][CL_CHAT_ZONG_HE_MSG];
	for opType, _ in pairs( t_chkMapToChatPingBiOp ) do
		t_zongHeOptionList[opType]["isPingBi"] = ChatManager:isZongHeChannelPingBi( opType );
	end
end

t_ChatPingBiSettingFrameControl["initClanOptionList"] =
function ( self )
	local t_clanOptionList = self["allOption"][CL_CHAT_CLAN];
	for opType, _ in pairs( t_chkMapToChatPingBiOp ) do
		t_clanOptionList[opType]["isPingBi"] = ChatManager:isClanChannelPingBi( opType );
	end
end

t_ChatPingBiSettingFrameControl["initWorldOptionList"] =
function ( self )
	local t_worldOptionList = self["allOption"][CL_CHAT_WORLD];
	for opType, _ in pairs( t_chkMapToChatPingBiOp ) do
		t_worldOptionList[opType]["isPingBi"] = ChatManager:isWorldChannelPingBi( opType );
	end
end

t_ChatPingBiSettingFrameControl["initTeamOptionList"] =
function ( self )
	local t_teamOptionList = self["allOption"][CL_CHAT_TEAM];
	for opType, _ in pairs( t_chkMapToChatPingBiOp ) do
		t_teamOptionList[opType]["isPingBi"] = ChatManager:isTeamChannelPingBi( opType );
	end
end

t_ChatPingBiSettingFrameControl["initNearOptionList"] =
function ( self )
	local t_nearOptionList = self["allOption"][CL_CHAT_SAY];
	for opType, _ in pairs( t_chkMapToChatPingBiOp ) do
		t_nearOptionList[opType]["isPingBi"] = ChatManager:isNearChannelPingBi( opType );
	end
end

t_ChatPingBiSettingFrameControl["initPrivateOptionList"] =
function ( self )
	local t_privateOptionList = self["allOption"][CL_CHAT_PRIVATE];
	for opType, _ in pairs( t_chkMapToChatPingBiOp ) do
		t_privateOptionList[opType]["isPingBi"] = ChatManager:isPrivateChannelPingBi( opType );
	end
end

t_ChatPingBiSettingFrameControl["getPageType"] =
function ( self )
	return self["pageType"];
end

t_ChatPingBiSettingFrameControl["setPageType"] =
function ( self, data )
	self["pageType"] = data["pageType"];
end

t_ChatPingBiSettingFrameControl["getPageTypeByPageBtn"] =
function ( self, data )
	for defPageType, oneConfgiBtn in pairs( t_pageTypeBtnMapToPingOp ) do
		if oneConfgiBtn["btn"] == data["btn"] then
			return defPageType;
		end
	end

	return CL_CHAT_ZONG_HE_MSG;
end

t_ChatPingBiSettingFrameControl["getPageBtnByType"] =
function ( self, data )
	local nPageType = data["pageType"];
	for nDefPageType, oneConfigPageBtn in pairs( t_pageTypeBtnMapToPingOp ) do
		if nDefPageType == nPageType then
			local pageBtn = getglobal( oneConfigPageBtn["btn"] );
			return pageBtn;
		end
	end

	return nil;
end

t_ChatPingBiSettingFrameControl["getOpChkBtnByOpBtn"] =
function ( self, data )
	for _, oneOpConfig in pairs( t_chkMapToChatPingBiOp ) do
		if oneOpConfig["btn"] == data["btn"] then
			return oneOpConfig["chkBtn"];
		end
	end

	return nil;
end

t_ChatPingBiSettingFrameControl["getOpTypeByBtn"] =
function ( self, data )
	for opType, oneOpConfig in pairs( t_chkMapToChatPingBiOp ) do
		if oneOpConfig["btn"] == data["btn"] then
			return opType;
		end
	end

	return CL_CHAT_ZONG_HE_MSG;
end

t_ChatPingBiSettingFrameControl["getOpTypeByChkBtn"] =
function ( self, data )
	for opType, oneOpConfig in pairs( t_chkMapToChatPingBiOp ) do
		if oneOpConfig["chkBtn"] == data["chkBtn"] then
			return opType;
		end
	end

	return CL_CHAT_ZONG_HE_MSG;
end

t_ChatPingBiSettingFrameControl["getOpListByPageType"] =
function ( self, data )
	local nPageType = data["pageType"];
	return self["allOption"][nPageType];
end

function ChatPingBiSettingFrame_OnLoad()
	for event, _ in pairs( t_ChatPingBiSettingFrameEvents ) do
		this:RegisterEvent( event );
	end

	t_ChatPingBiSettingFrameEvents.__index = function ( tab, key )
									tab[key] = { func = function () end };
									return tab[key];
								end
	setmetatable( t_ChatPingBiSettingFrameEvents, t_ChatPingBiSettingFrameEvents );
end

function ChatPingBiSettingFrame_OnEvent()
	t_ChatPingBiSettingFrameEvents[arg1].func();
end

function ChatPingBiSettingFrame_OnShow()
	this:SetPoint( "left", "MainChatFrame", "right", 0, 0 );
	t_ChatPingBiSettingFrameControl:setPageType( { ["pageType"] = CL_CHAT_ZONG_HE_MSG } );
	t_ChatPingBiSettingFrameControl:initAllOptionList();
	UpdateChatPingBiSettingFrame();
	UpdatePageTypeBtns();
end

-- 更新聊天屏蔽面板
function UpdateChatPingBiSettingFrame()
	local nCurPageType = t_ChatPingBiSettingFrameControl:getPageType();
	if nCurPageType == CL_CHAT_ZONG_HE_MSG then
		Private_UpdateChatPingBiSetting_SubZongHeFrame();
	elseif nCurPageType == CL_CHAT_WORLD then
		Private_UpdateChatPingBiSetting_SubWorldFrame();
	elseif nCurPageType == CL_CHAT_CLAN then
		Private_UpdateChatPingBiSetting_SubClanFrame();
	elseif nCurPageType == CL_CHAT_TEAM then
		Private_UpdateChatPingBiSetting_SubTeamFrame();
	elseif nCurPageType == CL_CHAT_SAY then
		Private_UpdateChatPingBiSetting_SubNearFrame();
	elseif nCurPageType == CL_CHAT_PRIVATE then
		Private_UpdateChatPingBiSetting_SubPrivateFrame();
	end

	Private_UpdateChatPingBiSettingFrameBtnNum();
end

function Private_UpdateChatPingBiSettingFrameBtnNum()
	local nCurPageType = t_ChatPingBiSettingFrameControl:getPageType();

	local t_icons = { "ChatPingBiSettingFrameWarnNewMessageChkBtn", "ChatPingBiSettingFrameWarnNewMessageHeadBtn", };
	
	if nCurPageType == CL_CHAT_ZONG_HE_MSG then
		util.HideIcons( t_icons );
	else
		util.ShowIcons( t_icons );
	end
end

-- 更新选定标签按钮
function ResetPageTypeBtns()
	for _, oneConfigPageBtn in pairs( t_pageTypeBtnMapToPingOp ) do
		local pageBtn = getglobal( oneConfigPageBtn["btn"] );
		pageBtn:DisChecked();
	end
end

function UpdatePageTypeBtns()
	ResetPageTypeBtns();
	local nCurPageType	= t_ChatPingBiSettingFrameControl:getPageType();
	local pageBtn		= t_ChatPingBiSettingFrameControl:getPageBtnByType( { ["pageType"] = nCurPageType } );
	if pageBtn == nil then
		return;
	end

	pageBtn:Checked();
end

-- 屏蔽面板的类型按钮
function ChatPingBiSettingFrame_PageTypeButton_OnClick()
	local nPageType = t_ChatPingBiSettingFrameControl:getPageTypeByPageBtn( { ["btn"] = this:GetName() } );
	t_ChatPingBiSettingFrameControl:setPageType( { ["pageType"] = nPageType } );
	UpdateChatPingBiSettingFrame();
	UpdatePageTypeBtns();
end

-- 屏蔽选项抬头按钮
function ChatPingBiSettingFrame_OpBtn_OnClick()
	local szChkBtnName = t_ChatPingBiSettingFrameControl:getOpChkBtnByOpBtn( { ["btn"] = this:GetName() } );
	if szChkBtnName == nil then
		return;
	end

	local chkBtn = getglobal( szChkBtnName );
	chkBtn:setCheckState( not chkBtn:GetCheckState() );

	local nCurPageType	= t_ChatPingBiSettingFrameControl:getPageType();
	local t_curOpList	= t_ChatPingBiSettingFrameControl:getOpListByPageType( { ["pageType"] = nCurPageType } );
	local nCurOpType	= t_ChatPingBiSettingFrameControl:getOpTypeByBtn( { ["btn"] = this:GetName() } );
	t_curOpList[nCurOpType]["isPingBi"] = not chkBtn:GetCheckState();
end

function ChatPingBiSettingFrame_OpChkBtn_OnClick()
	local nCurPageType	= t_ChatPingBiSettingFrameControl:getPageType();
	local t_curOpList	= t_ChatPingBiSettingFrameControl:getOpListByPageType( { ["pageType"] = nCurPageType } );
	local nCurOpType	= t_ChatPingBiSettingFrameControl:getOpTypeByChkBtn( { ["chkBtn"] = this:GetName() } );
	t_curOpList[nCurOpType]["isPingBi"] = not this:GetCheckState();
end

-- 恢复当前屏蔽子面板的默认选项
function ChatPingBiSettingFrame_OneChatChannelDefaultButton_OnClick()
	local nCurPageType = t_ChatPingBiSettingFrameControl:getPageType();
	if nCurPageType == CL_CHAT_ZONG_HE_MSG then
		Private_SetZongHeDefaultPingBiOps();
	elseif nCurPageType == CL_CHAT_WORLD then
		Private_SetWorldDefaultPingBiOps();
	elseif nCurPageType == CL_CHAT_CLAN then
		Private_SetClanDefaultPingBiOps();
	elseif nCurPageType == CL_CHAT_TEAM then
		Private_SetTeamDefaultPingBiOps();
	elseif nCurPageType == CL_CHAT_SAY then
		Private_SetNearDefaultPingBiOps();
	elseif nCurPageType == CL_CHAT_PRIVATE then
		Private_SetPrivateDefaultPingBiOps();
	end

	UpdateChatPingBiSettingFrame();
end

-- 应用当前屏蔽子面板的已勾选的选项
function ChatPingBiSettingFrame_OneChatChannelApplyCurrentButton_OnClick()
	ApplyCurrentPingBiSetting_ChatPingBiSettingFrame();
	ShowMidTips( "设置成功" );
end

-- 全部恢复默认
function ChatPingBiSettingFrame_AllChannelDefaultButton_OnClick()
	ChatPingBiSetting_AllDefaultFrame:Show();
end

-- 确认
function ChatPingBiSettingFrame_AllChannelConfirmButton_OnClick()
	Private_ApplyCurrentPingBiSetting_SubZongHeFrame();
	Private_ApplyCurrentPingBiSetting_SubWorldFrame();
	Private_ApplyCurrentPingBiSetting_SubClanFrame();
	Private_ApplyCurrentPingBiSetting_SubTeamFrame();
	Private_ApplyCurrentPingBiSetting_SubNearFrame();
	Private_ApplyCurrentPingBiSetting_SubPrivateFrame();
	ShowMidTips( "设置成功" );
	ChatPingBiSettingFrame:Hide();
end

function ApplyCurrentPingBiSetting_ChatPingBiSettingFrame()
	local nCurPageType = t_ChatPingBiSettingFrameControl:getPageType();
	if nCurPageType == CL_CHAT_ZONG_HE_MSG then
		Private_ApplyCurrentPingBiSetting_SubZongHeFrame();
	elseif nCurPageType == CL_CHAT_WORLD then
		Private_ApplyCurrentPingBiSetting_SubWorldFrame();
	elseif nCurPageType == CL_CHAT_CLAN then
		Private_ApplyCurrentPingBiSetting_SubClanFrame();
	elseif nCurPageType == CL_CHAT_TEAM then
		Private_ApplyCurrentPingBiSetting_SubTeamFrame();
	elseif nCurPageType == CL_CHAT_SAY then
		Private_ApplyCurrentPingBiSetting_SubNearFrame();
	elseif nCurPageType == CL_CHAT_PRIVATE then
		Private_ApplyCurrentPingBiSetting_SubPrivateFrame();
	end
end
-- CORE:
-- 应用当前勾选的选项
-- 应用综合子面板的当前选项
function Private_ApplyCurrentPingBiSetting_SubZongHeFrame()
	local t_zongHeOptionList = t_ChatPingBiSettingFrameControl["allOption"][CL_CHAT_ZONG_HE_MSG];
	for defPageType, oneConfgiBtn in pairs( t_chkMapToChatPingBiOp ) do
		local chkBtn = getglobal( oneConfgiBtn["chkBtn"] );
		ChatManager:setZongHeChannelPingBiOp( defPageType, t_zongHeOptionList[defPageType]["isPingBi"] );
	end

	ChatManager:savePingBiZongHeChannelsOp();
	t_ChatPingBiSettingFrameControl:initZongHeOptionList();
end

-- 应用世界子面板的当前选项
function Private_ApplyCurrentPingBiSetting_SubWorldFrame()
	local t_worldOptionList = t_ChatPingBiSettingFrameControl["allOption"][CL_CHAT_WORLD];
	for defPageType, oneConfgiBtn in pairs( t_chkMapToChatPingBiOp ) do
		local chkBtn = getglobal( oneConfgiBtn["chkBtn"] );
		ChatManager:setWorldChannelPingBiOp( defPageType, t_worldOptionList[defPageType]["isPingBi"] );
	end

	ChatManager:savePingBiWorldChannelsOp();
	t_ChatPingBiSettingFrameControl:initWorldOptionList();
end

-- 应用公会子面板的当前选项
function Private_ApplyCurrentPingBiSetting_SubClanFrame()
	local t_clanOptionList = t_ChatPingBiSettingFrameControl["allOption"][CL_CHAT_CLAN];
	for defPageType, oneConfgiBtn in pairs( t_chkMapToChatPingBiOp ) do
		local chkBtn = getglobal( oneConfgiBtn["chkBtn"] );
		ChatManager:setClanChannelPingBiOp( defPageType, t_clanOptionList[defPageType]["isPingBi"] );
	end

	ChatManager:savePingBiClanChannelsOp();
	t_ChatPingBiSettingFrameControl:initClanOptionList();
end

-- 应用队伍子面板的当前选项
function Private_ApplyCurrentPingBiSetting_SubTeamFrame()
	local t_teamOptionList = t_ChatPingBiSettingFrameControl["allOption"][CL_CHAT_TEAM];
	for defPageType, oneConfgiBtn in pairs( t_chkMapToChatPingBiOp ) do
		local chkBtn = getglobal( oneConfgiBtn["chkBtn"] );
		ChatManager:setTeamChannelPingBiOp( defPageType, t_teamOptionList[defPageType]["isPingBi"] );
	end

	ChatManager:savePingBiTeamChannelsOp();
	t_ChatPingBiSettingFrameControl:initTeamOptionList();
end

-- 应用附近子面板的当前选项
function Private_ApplyCurrentPingBiSetting_SubNearFrame()
	local t_nearOptionList = t_ChatPingBiSettingFrameControl["allOption"][CL_CHAT_SAY];
	for defPageType, oneConfgiBtn in pairs( t_chkMapToChatPingBiOp ) do
		local chkBtn = getglobal( oneConfgiBtn["chkBtn"] );
		ChatManager:setNearChannelPingBiOp( defPageType, t_nearOptionList[defPageType]["isPingBi"] );
	end

	ChatManager:savePingBiNearChannelsOp();
	t_ChatPingBiSettingFrameControl:initNearOptionList();
end

-- 应用私聊子面板的当前选项
function Private_ApplyCurrentPingBiSetting_SubPrivateFrame()
	local t_privateOptionList = t_ChatPingBiSettingFrameControl["allOption"][CL_CHAT_PRIVATE];
	for defPageType, oneConfgiBtn in pairs( t_chkMapToChatPingBiOp ) do
		local chkBtn = getglobal( oneConfgiBtn["chkBtn"] );
		ChatManager:setPrivateChannelPingBiOp( defPageType, t_privateOptionList[defPageType]["isPingBi"] );
	end

	ChatManager:savePingBiPrivateChannelsOp();
	t_ChatPingBiSettingFrameControl:initPrivateOptionList();
end

-- 更新聊天屏蔽综合子面板
function Private_UpdateChatPingBiSetting_SubZongHeFrame()
	local t_zongHeOptionList = t_ChatPingBiSettingFrameControl["allOption"][CL_CHAT_ZONG_HE_MSG];
	for opType, data in pairs( t_chkMapToChatPingBiOp ) do
		local chkBtn = getglobal( data["chkBtn"] );
		chkBtn:setCheckState( not t_zongHeOptionList[opType]["isPingBi"] );
	end
end

-- 更新聊天屏蔽世界子面板
function Private_UpdateChatPingBiSetting_SubWorldFrame()
	local t_worldOptionList = t_ChatPingBiSettingFrameControl["allOption"][CL_CHAT_WORLD];
	for opType, data in pairs( t_chkMapToChatPingBiOp ) do
		local chkBtn = getglobal( data["chkBtn"] );
		chkBtn:setCheckState( not t_worldOptionList[opType]["isPingBi"] );
	end
end

-- 更新聊天屏蔽公会子面板
function Private_UpdateChatPingBiSetting_SubClanFrame()
	local t_clanOptionList = t_ChatPingBiSettingFrameControl["allOption"][CL_CHAT_CLAN];
	for opType, data in pairs( t_chkMapToChatPingBiOp ) do
		local chkBtn = getglobal( data["chkBtn"] );
		chkBtn:setCheckState( not t_clanOptionList[opType]["isPingBi"] );
	end
end

-- 更新聊天屏蔽队伍子面板
function Private_UpdateChatPingBiSetting_SubTeamFrame()
	local t_teamOptionList = t_ChatPingBiSettingFrameControl["allOption"][CL_CHAT_TEAM];
	for opType, data in pairs( t_chkMapToChatPingBiOp ) do
		local chkBtn = getglobal( data["chkBtn"] );
		chkBtn:setCheckState( not t_teamOptionList[opType]["isPingBi"] );
	end
end

-- 更新聊天屏蔽附近子面板
function Private_UpdateChatPingBiSetting_SubNearFrame()
	local t_nearOptionList = t_ChatPingBiSettingFrameControl["allOption"][CL_CHAT_SAY];
	for opType, data in pairs( t_chkMapToChatPingBiOp ) do
		local chkBtn = getglobal( data["chkBtn"] );
		chkBtn:setCheckState( not t_nearOptionList[opType]["isPingBi"] );
	end
end

-- 更新聊天屏蔽私聊子面板
function Private_UpdateChatPingBiSetting_SubPrivateFrame()
	local t_privateOptionList = t_ChatPingBiSettingFrameControl["allOption"][CL_CHAT_PRIVATE];
	for opType, data in pairs( t_chkMapToChatPingBiOp ) do
		local chkBtn = getglobal( data["chkBtn"] );
		chkBtn:setCheckState( not t_privateOptionList[opType]["isPingBi"] );
	end
end

-- 将所有聊天频道屏蔽选项设置为默认
function Private_SetAllDefaultPingBiOps()
	Private_SetZongHeDefaultPingBiOps();
	Private_SetWorldDefaultPingBiOps();
	Private_SetClanDefaultPingBiOps();
	Private_SetTeamDefaultPingBiOps();
	Private_SetNearDefaultPingBiOps();
	Private_SetPrivateDefaultPingBiOps();
end

-- 将综合聊天频道屏蔽选项设置为默认
function Private_SetZongHeDefaultPingBiOps()
	for i = 1, CL_CHAT_END do
		ChatManager:setZongHeChannelPingBiOp( i - 1, true );
	end

	local t_defaultZongHeOp = { CL_CHAT_SYS_MSG, CL_CHAT_TEAM, CL_CHAT_WORLD, CL_CHAT_SAY, CL_CHAT_CLAN, CL_CHAT_PRIVATE,  CL_CHAT_ZONG_HE_MSG,
								CL_CHAT_WARN_NEW_MESSAGE, CL_CHAT_BATTLE, CL_CHAT_CLAN_GROUP, CL_CHAT_SPAN_MAP, CL_CHAT_SPAN_MAP_WORLD };
	for _, opType in ipairs( t_defaultZongHeOp ) do
		ChatManager:setZongHeChannelPingBiOp( opType, false );
	end

	ChatManager:savePingBiZongHeChannelsOp();
	t_ChatPingBiSettingFrameControl:initZongHeOptionList();
end

-- 将世界聊天频道屏蔽选项设置为默认
function Private_SetWorldDefaultPingBiOps()
	for i = 1, CL_CHAT_END do
		ChatManager:setWorldChannelPingBiOp( i - 1, true );
	end

	local t_defaultWorldOp = { CL_CHAT_WORLD, CL_CHAT_PRIVATE, };
	for _, opType in ipairs( t_defaultWorldOp ) do
		ChatManager:setWorldChannelPingBiOp( opType, false );
	end

	ChatManager:savePingBiWorldChannelsOp();

	t_ChatPingBiSettingFrameControl:initWorldOptionList();
end

-- 将公会聊天频道屏蔽选项设置为默认
function Private_SetClanDefaultPingBiOps()
	for i = 1, CL_CHAT_END do
		ChatManager:setClanChannelPingBiOp( i - 1, true );
	end

	local t_defaultClanOp = { CL_CHAT_CLAN, CL_CHAT_PRIVATE, CL_CHAT_WARN_NEW_MESSAGE, CL_CHAT_CLAN_GROUP, };
	for _, opType in ipairs( t_defaultClanOp ) do
		ChatManager:setClanChannelPingBiOp( opType, false );
	end

	ChatManager:savePingBiClanChannelsOp();
	t_ChatPingBiSettingFrameControl:initClanOptionList();
end

-- 将队伍聊天频道屏蔽选项设置为默认
function Private_SetTeamDefaultPingBiOps()
	for i = 1, CL_CHAT_END do
		ChatManager:setTeamChannelPingBiOp( i - 1, true );
	end

	local t_defaultTeamOp = { CL_CHAT_TEAM, CL_CHAT_PRIVATE, CL_CHAT_WARN_NEW_MESSAGE, };
	for _, opType in ipairs( t_defaultTeamOp ) do
		ChatManager:setTeamChannelPingBiOp( opType, false );
	end

	ChatManager:savePingBiTeamChannelsOp();

	t_ChatPingBiSettingFrameControl:initTeamOptionList();
end

-- 将附近聊天频道屏蔽选项设置为默认
function Private_SetNearDefaultPingBiOps()
	for i = 1, CL_CHAT_END do
		ChatManager:setNearChannelPingBiOp( i - 1, true );
	end

	local t_defaultNearOp = { CL_CHAT_SAY, CL_CHAT_PRIVATE, CL_CHAT_BATTLE, CL_CHAT_SPAN_MAP_WORLD, CL_CHAT_SPAN_MAP};
	for _, opType in ipairs( t_defaultNearOp ) do
		ChatManager:setNearChannelPingBiOp( opType, false );
	end

	ChatManager:savePingBiNearChannelsOp();

	t_ChatPingBiSettingFrameControl:initNearOptionList();
end

-- 将私聊聊天频道屏蔽选项设置为默认
function Private_SetPrivateDefaultPingBiOps()
	for i = 1, CL_CHAT_END do
		ChatManager:setPrivateChannelPingBiOp( i - 1, true );
	end

	local t_defaultPrivateOp = { CL_CHAT_PRIVATE, CL_CHAT_WARN_NEW_MESSAGE, };
	for _, opType in ipairs( t_defaultPrivateOp ) do
		ChatManager:setPrivateChannelPingBiOp( opType, false );
	end

	ChatManager:savePingBiPrivateChannelsOp();

	t_ChatPingBiSettingFrameControl:initPrivateOptionList();
end

-- CORE:
-- 点击确认按钮
function ChatPingBiSetting_AllDefaultFrame_ConfirmButton_OnClick()
	Private_SetAllDefaultPingBiOps();
	UpdateChatPingBiSettingFrame();
	ChatPingBiSetting_AllDefaultFrame:Hide();
end

function ChatPingBiSetting_AllDefaultFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 0, 0 );
end