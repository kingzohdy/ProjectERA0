local MAX_RECORD_LINE_ONE_PAGE = 30;
MAX_CHAT_FRAME		= 6;
local MAX_RIGHT_FRAME_BTNS = 12;
local t_LabelNameList = {};
-----------------------师徒处理-------------------------------------------------------------
local t_SchoolGroupList = {}



										------------------------------
										--		好友面板		--
										------------------------------
-- 此为聊天窗口和聊天提示窗口管理器
local ChatFrameManager	= {

							newMessageQueue		= {},
							historyMessageQueue = {},

							-- 保存右下角名字列表的队列
							lessenNameQueue = {},
							-- 保存要闪动的名字列表的队列
							blinkNameQueue	= {},
					
							frameInfo =
							{
								["FriendChatFrame1"] = { isShow = false, speakerName = "" },
								["FriendChatFrame2"] = { isShow = false, speakerName = "" },
								["FriendChatFrame3"] = { isShow = false, speakerName = "" },
								["FriendChatFrame4"] = { isShow = false, speakerName = "" },
								["FriendChatFrame5"] = { isShow = false, speakerName = "" },
								["FriendChatFrame6"] = { isShow = false, speakerName = "" },

							}

							};

function ChatFrameManager:initChatManager()
	self.newMessageQueue = {};
	self.historyMessageQueue = {};
	self.lessenNameQueue = {};
	self.blinkNameQueue = {};
	self.frameInfo =
							{
								["FriendChatFrame1"] = { isShow = false, speakerName = "" },
								["FriendChatFrame2"] = { isShow = false, speakerName = "" },
								["FriendChatFrame3"] = { isShow = false, speakerName = "" },
								["FriendChatFrame4"] = { isShow = false, speakerName = "" },
								["FriendChatFrame5"] = { isShow = false, speakerName = "" },
								["FriendChatFrame6"] = { isShow = false, speakerName = "" },

							}
end

-- @brief:	此为用于刷新聊天提示面板
function ChatFrameManager:RefreshChatClue( )
	local frame = getglobal( "FriendFrame_ChatListFrame" );
	if frame:IsShown() then
		UpdateChatListFrame();
	end

	frame = getglobal( "FriendFrame_ChatClueFrame" );
	if frame:IsShown() then
		UpdateChatClueFrame();
	end
end

-- @brief:	此为用于查询聊天窗口和聊天提示窗口对应的聊天对象
-- @return: 聊天对象的名字
-- @param:	szFrameName 所要查询的聊天窗口或聊天提示窗口的名字
function ChatFrameManager:GetSpeakerName( szFrameName )
	-- 返回聊天对象的名字
	return self.frameInfo[szFrameName].speakerName;
end

function ChatFrameManager:SetSpeakerName( szFrameName, szNewSpeakName )
	-- 返回聊天对象的名字
	self.frameInfo[szFrameName].speakerName = szNewSpeakName;
end

-- 根据玩家名字查询当前为他打开的对应聊天窗口
-- @return: 若找到对应的聊天窗口，则返回聊天窗口的名字，否则返回nil
-- 查询
function ChatFrameManager:GetSpeakerFrame( szSpeakerName )
	for szFrameName, data in pairs( self.frameInfo ) do 
		if data.isShow and data.speakerName == szSpeakerName then
			return szFrameName;
		end
	end

	return nil;
end

-- 此为用于检测打开的聊天窗口是否达到上限
-- 查询
function ChatFrameManager:CanOpenNewChatFrame()
	for _, data in pairs( self.frameInfo ) do
		if not data.isShow then
			return true;
		end
	end
	
	return false;
end

-- 查询缩小列表名字信息
function ChatFrameManager:GetLessenNameQueue()
	return self.lessenNameQueue;
end

-- 查询闪烁列表名字信息
-- 查询是否有新消息
function ChatFrameManager:GetBlinkNameQueue()
	return self.blinkNameQueue;
end

-- 查询某个名字是否处于闪动状态
function ChatFrameManager:IsNameBlink( szSpeakerName )
	for _, szName in ipairs( self.blinkNameQueue ) do
		if szSpeakerName == szName then
			return true;
		end
	end

	return false;
end

-- @brief:	此为用于设置聊天窗口和聊天提示窗口对应的聊天对象
-- @return:	void
-- @param:	szFrameName 所要设置的聊天窗口或聊天提示窗口的名字
-- @param:	szSpeakerName 所要设置的聊天对象的名字
function ChatFrameManager:SetFrameInfo( szFrameName, szSpeakerName, bIsShow )
	-- 返回聊天对象的名字
	self.frameInfo[szFrameName].speakerName = szSpeakerName;
	self.frameInfo[szFrameName].isShow		= bIsShow;
end

-- 触发操作: 此为点击聊天提示窗口或者双击好友面板中人物名字的时候响应
-- @brief:	弹出一个聊天窗口
-- @return: void
-- @param:	szSpeakerName 目标聊天对象的名字

--ACTION: 为某个指定玩家打开聊天窗口
function ChatFrameManager:OpenChatFrame( szSpeakerName, nChatType )
	-- 若聊天窗口打开的数目已经达到上限，则给个中央提示
	if not self:CanOpenNewChatFrame() then
		ShowMidTips( "打开的聊天窗口数目已经达到上限，不能再打开新的聊天窗口" );
		return;
	end

	self:DelRoleNameRecord( szSpeakerName );
	-- 打开新的聊天窗口的时候，同时要将闪烁的名字变成不闪烁
	--在好友或者师徒列表
	if GameFriendManager:getFriendInfoByName( szSpeakerName ) ~= nil or GameSchool:IsSelfSchool( szSpeakerName ) then
		DisableBlinkName( szSpeakerName );
	end
	
	-- 弹出新的聊天窗口
	for i = 1, MAX_CHAT_FRAME do
		local szChatFrameName = "FriendChatFrame" .. i;
		-- 弹出第一个未使用的聊天窗口
		if not self.frameInfo[szChatFrameName].isShow then
			local frame = getglobal( szChatFrameName );
			local font	= getglobal( szChatFrameName .. "_ChatDestName" );
			self:SetFrameInfo( szChatFrameName, szSpeakerName, true );
			font:SetText( "与	" .. szSpeakerName .. "对话中");
			frame:Show();
			if nChatType == nil then
				frame:SetClientUserData( 0, 0 )
			else
				frame:SetClientUserData( 0, nChatType )
			end
			break;
		end
	end
end

-- 触发操作: 此为关闭聊天窗口的时候响应
-- @brief:	关闭一个聊天窗口
-- @return: void
-- @param:	szFrameName 被关闭的聊天窗口的名字

--ACTION: 关闭某个指定的聊天窗口
function ChatFrameManager:ShutDownChatFrame( szFrameName )
	--ValidateLinkText( szFrameName );
	-- 清空名字队列中与该聊天窗口对应的聊天对象speaker的所有信息
	local szSpeakerName = self.frameInfo[szFrameName].speakerName;
	self.newMessageQueue[szSpeakerName] = nil;
	
	-- 关闭名字为szFrameName的聊天窗口;
	local frame	= getglobal( szFrameName );
	-- 将与szFrameName聊天窗口的底层状态置为0
	self:SetFrameInfo( szFrameName, "", false );
	frame:Hide();
end

function ValidateLinkText( szFrameName )
	local t_delLinkType = {};
	local edit			= getglobal( szFrameName.."_InputEdit" );
	local szText		= edit:GetText();

	for i = 1, ChatManager:getNumCacheDisplayText() do
		local oneCacheDisplayTextInfo		= ChatManager:getCacheDisplayText( i - 1 );
		local nLinkBeginPos, nLinkEndPos	= string.find( szText, oneCacheDisplayTextInfo.m_strLinkName, 1, true );
		if ( nLinkBeginPos ~= nil and nLinkEndPos ~= nil ) then
			table.insert( t_delLinkType, { ["linkType"] = oneCacheDisplayTextInfo.m_nLinkType, 
						["text"] = oneCacheDisplayTextInfo.m_strLinkName } );
		end
	end

	for index, oneDeleteLinkData in ipairs( t_delLinkType ) do
		ChatManager:deleteLink( oneDeleteLinkData["linkType"] );
		RemoveOneLinkCache( { ["linkType"] = oneDeleteLinkData["linkType"] } );
	end
end

-- 触发操作: 点击缩小聊天窗口按钮的时候响应
-- @brief:	 将一个聊天窗口缩小为聊天提示窗口
-- @return:  void
-- @param:	 szFrameName 被缩小的聊天窗口的名字

--ACTION: 缩小某个指定的聊天窗口
function ChatFrameManager:LessenChatFrame( szChatFrameName )
	local szSpeakerName = self.frameInfo[szChatFrameName].speakerName;
	-- 记录被缩小窗口的人名字到名字队列中
	self:RecordRoleName( szSpeakerName );
	assert( self:GetSpeakerFrame( szSpeakerName ) ~= nil );
	-- 关闭szChatFrameName聊天窗口
	local frame	= getglobal( szChatFrameName );
	table.insert( self.lessenNameQueue, szSpeakerName );
	-- 清空被关闭的聊天窗口的对应状态
	self:SetFrameInfo( szChatFrameName, "", false );
	frame:Hide();
end

-- @brief:	此为用于查询消息队列里面某个聊天对象的聊天信息
-- @return: 消息记录表table
-- @param:	szSpeakerName 所要查询的聊天对象的名字

-- 查询
function ChatFrameManager:GetChatContent( szSpeakerName )
	return	( self.newMessageQueue[szSpeakerName] ~= nil ) and self.newMessageQueue[szSpeakerName].message or {};
end

-- @brief:	此为用于查询某个玩家是否在我新收消息队列里
-- @return: true表示在新手消息里，
function ChatFrameManager:IsNewSpeaker( szSpeakerName )
	return	self.newMessageQueue[szSpeakerName] ~= nil;
end

-- @brief:	此为用于查询消息队列里面某个聊天对象的聊天记录信息
-- @return: 消息记录表table
-- @param:	szSpeakerName 所要查询的聊天对象的名字
function ChatFrameManager:GetHistroy( szSpeakerName )
	return	( self.historyMessageQueue[szSpeakerName] ~= nil ) and self.historyMessageQueue[szSpeakerName].message or {};
end

-- 触发操作: 当收到一个新消息的时候,此为当发消息的人有对应的聊天提示窗口或聊天窗口的时候
-- @brief:	将收到的新消息压进消息队列中
-- @return: void
-- @param:	msg 收到的消息内容
-- @param:	receiveTime 收到消息的时间

--ACTION: 用于接收最新的消息
function ChatFrameManager:ReceiveMessage( szChatContent, szSpeakerName )
	-- 将收到的新消息压进消息队列中
	--[[ [szSpeakerName] = { message = { { receiveTime = ?, chatContent = ? }, ... } }, --]]

	-- 特别处理: 当第一次添加某个人的消息内容的时候,出于Lua语法上的不足进行初始化
	if self.newMessageQueue[szSpeakerName] == nil then
		self.newMessageQueue[szSpeakerName]				= {};
		self.newMessageQueue[szSpeakerName].message		= {};
	end

	if self.historyMessageQueue[szSpeakerName] == nil then
		self.historyMessageQueue[szSpeakerName]			= {};
		self.historyMessageQueue[szSpeakerName].message	= {};
	end
	
	table.insert( self.newMessageQueue[szSpeakerName].message,		szChatContent );
	table.insert( self.historyMessageQueue[szSpeakerName].message,	szChatContent );
end

-- 记录聊天对象的名字
-- 这用于收到新消息的时候，用于改变名字队列的信息
function ChatFrameManager:RecordRoleName( szSpeakerName )
	-- 若该名字有对应打开的聊天窗口
	-- 若已经有对应的聊天窗口显示该聊天对象的消息，则无需将该对象名字压入名字队列中 
	if self:GetSpeakerFrame( szSpeakerName ) ~= nil then
		return;
	end

	local bInRecord = false;
	for _, szName in ipairs( self.lessenNameQueue ) do 
		if szName == szSpeakerName then
			bInRecord = true;
			break;
		end
	end

	if not bInRecord then
		table.insert( self.lessenNameQueue, szSpeakerName );
	end
	
	bInRecord = false;
	for _, szName in ipairs( self.blinkNameQueue ) do 
		if szName == szSpeakerName then
			bInRecord = true;
			break;
		end
	end
	
	if not bInRecord then
		table.insert( self.blinkNameQueue, szSpeakerName );
	end	
end

-- 聊天对象的名字的记录
function ChatFrameManager:DelRoleNameRecord( szSpeakerName )
	for index, name in ipairs( self.lessenNameQueue ) do
		if name == szSpeakerName then
			table.remove( self.lessenNameQueue, index );
			break;
		end
	end

	for index, name in ipairs( self.blinkNameQueue ) do
		if name == szSpeakerName then
			table.remove( self.blinkNameQueue, index );
			break;
		end
	end

	-- 刷新聊天提示
	self:RefreshChatClue();
end

-- 用于下线清空记录用的
function ChatFrameManager:ClearAllMessage()
	self.historyMessageQueue	= {};
	self.newMessageQueue		= {};
	self.lessenNameQueue		= {};
	self.blinkNameQueue			= {};
end

function ClearChatFrameRecordMessage()
	ChatFrameManager:ClearAllMessage();
end

--MODEL2
----------注意:表中的【师傅】,【夫妻】,【徒弟】在MODEL表中是不存在的,只在控件中才设置这些前缀关系
--Desc: 玩家名字列表:CharacterList = { {角色lowID, 角色heightID, "角色名字"} },
------: ListNode = {角色lowID, 角色heightID, "角色名字"}
------: 特别的:师傅列表={ {角色lowID, 角色heightID, "【师傅】角色名字"}} 徒弟列表={ {角色lowID, 角色heightID, "【徒弟】角色名字"}}
--------------------:好友列表 = { {角色heightID, 角色lowID,"【老公】角色名字"}, {角色heightID, 角色lowID,"【老婆】角色名字"}}

--MODEL3
--@Desc: 此为当前点击的玩家信息 对应C++中的tagFriendInfo
local g_CurClickRole = { roleLowId = 0, roleHeightId = 0, roleName = "" };

--定义玩家列表显示行数
local MAX_ROLE_NAME_LINE = 19;

-------------------好友面板--------------------------------------------
local g_friendFrameButtons = { buttonName = { "FriendShipFrameFriendBtn", "FriendShipFrameTempBtn", "FriendShipFrameBrotherBtn",							"FriendShipFramePrenticeBtn",
								"FriendShipFrameKillBtn", "FriendShipFrameEnemyBtn", "FriendShipFrameBlacklistBtn" } };

--@Desc:此为聊天面板中能够显示的聊天记录的最多行数
local MAX_CHAT_LINE = 20;

--------------------添加角色名字框使用到得全局文件变量-----------------------------
local g_nAddType = 0;

--------------------申请好友框使用的文件全局变量------------------------------------
local g_szReqCharacterName = "";--此为申请人的名字

---------------------聊天提示框使用的文件全局变量-----------------------------------

--指示当前所显示的列表类型
local g_nDisplayPageType = CLIENT_RELATION_TYPE_FRIEND;--默认显示好友页面

-- 此为暴露给外部的用于关闭聊天窗口的接口
function CloseChatFrame( szFrameName )
	ChatFrameManager:ShutDownChatFrame( szFrameName );
end

--@Desc:用于更新玩家名字的颜色
--@return: Nothing
function RoleOnLine( )
	-- 收到上下线消息,先刷新列表,因为凶手和临时列表会发生人物数量的变化
	if g_nDisplayPageType == CLIENT_RELATION_TYPE_FRIEND then
		SetScrollBar( "FriendShipFrame_ScrollBar", MAX_ROLE_NAME_LINE, getTotalBtn() );
		local slider = getglobal( "FriendShipFrame_ScrollBar" );
		UpdateFriendNameList( slider:GetValue() + 1 );
		if FriendInfoFrame:IsShown() then
			local frame = getglobal( "FriendInfoFrame" );
			if frame:GetClientString() == "" then 
				ShowMidTips( "错误的玩家信息" );
				FriendInfoFrame:Hide();
				return;
			end
			UpdateFriendInfo( frame:GetClientString() );
		end	
	elseif g_nDisplayPageType == CLIENT_RELATION_TYPE_PRENTICE then
		local slider = getglobal( "FriendShipFrame_ScrollBar" );
		UpdateSchoolNameList( slider:GetValue() + 1 );
	else
		SetFriendFrameScrollBar();
		local slider = getglobal( "FriendShipFrame_ScrollBar" );
		UpdateRoleNameList( slider:GetValue() + 1 );
	end
end

--@brief:	此为用来隐藏所有高亮条的方法
--@return:	Nothing
local function HideAllHighLightTex()
	local tex;

	for i = 1, MAX_ROLE_NAME_LINE do
		tex = getglobal( "FriendShipFrame_CharacterNameBtn" .. i .. "SelectHighLightTex" );
		tex:Hide();
	end
end

--@Desc:用于清空玩家列表当前显示的所有内容
--return: Nothing
function ClearRoleNameList()
	local rich;
	local button;
	local font;
	local chk;

	for i = 1, MAX_ROLE_NAME_LINE do
		rich = getglobal( "FriendShipFrame_CharacterNameBtn" .. i .. "CharacterName" );
		rich:SetClientUserData( 0, 0 );--清空低位信息
		rich:SetClientUserData( 1, 0 );--清空高位信息
		rich:SetClientString( "" );
		--rich:SetText( "" );
		rich:Clear( );
		
		font = getglobal( "FriendShipFrame_CharacterNameBtn" .. i .. "LeaveTimeFont" );
		font:SetText( "" );

		button = getglobal( "FriendShipFrame_CharacterNameBtn" .. i );
		button:SetClientUserData( 0, 0 );--清空低位信息
		button:SetClientUserData( 1, 0 );--清空高位信息
		button:SetClientString( "" );
		button:Hide();
		
		button = getglobal( "FriendShipFrame_CharacterNameBtn" .. i .. "EggsBtn" );
		button:Hide()
		button = getglobal( "FriendShipFrame_CharacterNameBtn" .. i .. "BookBtn" );
		button:Hide()
		button = getglobal( "FriendShipFrame_CharacterNameBtn" .. i .. "GiftBtn" );
		button:Hide()
		button = getglobal( "FriendShipFrame_CharacterNameBtn" .. i .. "FlowerBtn" );
		button:Hide()

		chk = getglobal("FriendShipFrameTitleSignChk" .. i );
		chk:Hide();
	end
	FriendShipFrameSchoolGiftBtn:Hide()

	HideAllHighLightTex();
end

--@Desc:此为用来固定被点击玩家名字高亮条的方法
function ResetHighLightTex()
	local tex;
	local rich;
	local szRelButtonName;
	HideAllHighLightTex();
	local tmpRole = {};
	if g_CurClickRole.roleName == "" then
		return;
	end

	for i = 1, MAX_ROLE_NAME_LINE do
		rich = getglobal( "FriendShipFrame_CharacterNameBtn" .. i .. "CharacterName" );
		szRelButtonName = rich:GetParent();
		tmpRole			= { roleLowId = rich:GetClientUserData(0),  roleHeightId = rich:GetClientUserData(1), roleName = rich:GetClientString()};

		-- 用于从显示文字对应的控件中获得玩家的名字信息
		if g_CurClickRole.roleLowId == tmpRole.roleLowId and g_CurClickRole.roleHeightId == tmpRole.roleHeightId then
			tex = getglobal( "FriendShipFrame_CharacterNameBtn" .. i .. "SelectHighLightTex" );
			tex:Show();
			return;
		end
	end
end

function ResetFriendHighLightTex()
	local tex;
	local rich;
	local szRelButtonName;
	HideAllHighLightTex();
	local tmpRole = {};
	if g_CurClickRole.roleName ~= "" and g_CurClickRole.roleLowId == 0 or g_CurClickRole.roleHeightId == 0 then
		for i = 1, MAX_ROLE_NAME_LINE do
			rich = getglobal( "FriendShipFrame_CharacterNameBtn" .. i .. "CharacterName" );
			szRelButtonName = rich:GetParent();
			tmpRole	= { roleLowId = rich:GetClientUserData(0), roleHeightId = rich:GetClientUserData(1), roleName = rich:GetClientString()};

			-- 用于从显示文字对应的控件中获得玩家的名字信息
			if g_CurClickRole.roleName == tmpRole.roleName then
				tex = getglobal( "FriendShipFrame_CharacterNameBtn" .. i .. "SelectHighLightTex" );
				tex:Show();
				return;
			end			
		end
		return;
	end

	for i = 1, MAX_ROLE_NAME_LINE do
		rich = getglobal( "FriendShipFrame_CharacterNameBtn" .. i .. "CharacterName" );
		szRelButtonName = rich:GetParent();
		tmpRole	= { roleLowId = rich:GetClientUserData(0), roleHeightId = rich:GetClientUserData(1), roleName = rich:GetClientString()};

		-- 用于从显示文字对应的控件中获得玩家的名字信息
		if g_CurClickRole.roleLowId == tmpRole.roleLowId and g_CurClickRole.roleHeightId == tmpRole.roleHeightId then
			tex = getglobal( "FriendShipFrame_CharacterNameBtn" .. i .. "SelectHighLightTex" );
			tex:Show();
			return;
		end
	end
end

--@Desc: 在点击对应对话框提示的时候,名字不再闪烁
function DisableBlinkName( szSpeakerName )
	--让好友面板中闪烁的名字不再闪烁
	local character		= GameFriendManager:getFriendInfoByName( szSpeakerName );
	--如果不在好友列表
	if character == nil then
		--查找师徒列表
		if GameSchool:IsSelfSchool( szSpeakerName ) then
			local character;
			if GameSchool:GetSelfSchoolJobType() == ROLE_SCHOOL_TYPE_STUDENT_TOLUA then
				character = GameSchool:GetSelfTeacher();
			else
				character = GameSchool:GetSelfStudent( szSpeakerName )
			end
			for i = 1, MAX_ROLE_NAME_LINE do
				rich = getglobal( "FriendShipFrame_CharacterNameBtn" .. i .. "CharacterName" );

				if szSpeakerName == rich:GetClientString() then
					rich:Clear( );
					if CLIENT_ONLINE == character.Online  then--若玩家在线,显示绿色
						rich:SetText( szSpeakerName, 255, 254, 189 );
					else--若玩家不在线,显示灰色
						rich:SetText( szSpeakerName, 160, 160, 160 );
					end

					break;
				end
			end
			return
		end
	end
	assert( character ~= nil, "获得了空玩家!" );
	local szPrefixName	= "";
	local rich;

	for i = 1, MAX_ROLE_NAME_LINE do
		rich = getglobal( "FriendShipFrame_CharacterNameBtn" .. i .. "CharacterName" );

		if szSpeakerName == rich:GetClientString() then
			--rich:SetText( "" );
			rich:Clear( );
			szPrefixName = GetRelatioDesc( character );
			--2表示挂机在线
			if CLIENT_ONLINE == character.m_nIsOnline or MUD_ONLINE == character.m_nIsOnline then--若玩家在线,显示绿色
				rich:SetText( szPrefixName .. szSpeakerName, 255, 254, 189 );
				--rich:SetTextColor( 255, 254, 189) ;
			else--若玩家不在线,显示灰色
				rich:SetText( szPrefixName .. szSpeakerName, 160, 160, 160 );
				--rich:SetTextColor( 160, 160, 160 ) ;
			end

			break;
		end
	end
end

--@Desc: 当发送来新消息的时候,让名字闪烁
function BlinkName( szSpeakerName )
	--让好友面板中闪烁的名字闪烁
	local character		= GameFriendManager:getFriendInfoByName( szSpeakerName );
	--如果不在好友列表
	if character == nil then
		--查找师徒列表
		if GameSchool:IsSelfSchool( szSpeakerName ) then
			local character;
			if GameSchool:GetSelfSchoolJobType() == ROLE_SCHOOL_TYPE_STUDENT_TOLUA then
				character = GameSchool:GetSelfTeacher();
			else
				character = GameSchool:GetSelfStudent( szSpeakerName )
			end

			for i = 1, MAX_ROLE_NAME_LINE do
				rich = getglobal( "FriendShipFrame_CharacterNameBtn" .. i .. "CharacterName" );

				if szSpeakerName == rich:GetClientString() then
					rich:Clear( );
					if CLIENT_ONLINE == character.Online  then--若玩家在线,显示绿色
						rich:SetText( szSpeakerName, 255, 254, 189 );
					else--若玩家不在线,显示灰色
						rich:SetText( szSpeakerName, 160, 160, 160 );
					end

					break;
				end
			end
			return
		end
	end
	assert( character ~= nil, "获得了空玩家!" );
	local rich;
	local szPreName = "";

	for i = 1, MAX_ROLE_NAME_LINE do
		rich = getglobal( "FriendShipFrame_CharacterNameBtn" .. i .. "CharacterName" );

		if szSpeakerName == rich:GetClientString() then
			--rich:SetText( "" );
			rich:Clear( );
			szPreName = GetRelatioDesc( character );
			--2表示挂机在线
			if CLIENT_ONLINE == character.m_nIsOnline or MUD_ONLINE == character.m_nIsOnline then--若玩家在线,显示绿色
				rich:SetText( "#b" .. szPreName ..  szSpeakerName, 255, 254, 189 );
			else--若玩家不在线,显示灰色
				rich:SetText( "#b" .. szPreName ..  szSpeakerName, 160, 160, 160 );
			end

			break;
		end
	end
end

--@Desc:设置滚动条的位置
function SetFriendFrameScrollBar( )
	--确定技能学习面板显示的具体技能
	local slider		= getglobal( "FriendShipFrame_ScrollBar" );
	local nMaxViewLine	= MAX_ROLE_NAME_LINE;--此为可视区域最大显示行数
	local nRoleNum		= GameFriendManager:getRelationPlayerCount( g_nDisplayPageType );
	local nNum			= nRoleNum - nMaxViewLine;

	if nNum > 0  then
		slider:SetMaxValue( nNum );
		slider:Show();
	else
		slider:SetMaxValue( 1 );
		slider:Hide();
	end

	slider:SetMinValue( 0 );
	slider:SetValueStep( 1 );


	-- 在删除了列表中的一个关系玩家以后,要特别检测用的,以防止
	if slider:GetValue() > slider:GetMaxValue() then
		slider:SetValue( slider:GetMaxValue() );
	end
end

-- @brief:	用于获得玩家的关系前缀名字
-- @return:	string 玩家与主角的关系描述
-- @param:	character 玩家信息
function GetRelatioDesc( character )
	local szPrefixName		= "";

	-- 获得名字前面的关系前缀名称
	if CLIENT_RELATION_TYPE_MASTER == character.m_nRelationType then
		szPrefixName = "【师傅】";
	elseif CLIENT_RELATION_TYPE_PRENTICE == character.m_nRelationType then
		szPrefixName = "【徒弟】";
	elseif CLIENT_RELATION_TYPE_SPOUSE == character.m_nRelationType then
		if CLIENT_GENDER_MALE == character.m_nGender then
			szPrefixName = "【丈夫】";
		else
			szPrefixName = "【妻子】";
		end
	else
		szPrefixName = "";
	end

	return szPrefixName;
end

--分组表
local t_GroupName = {
				[1]	= "分组一",
				[2]	= "分组二",
				[3]	= "分组三",
				[4]	= "分组四",
				[5]	= "分组五",
}

function GetFriParentNodeInfo( childNodeInfo )
	if childNodeInfo.nSubType == 0 then
		return;
	end
	for i = 1, table.getn( t_LabelNameList ) do
		if t_LabelNameList[i].nSubType== 0 and t_LabelNameList[i].nGroup == childNodeInfo.nGroup then
			return t_LabelNameList[i];
		end
	end
end

function HasChildNode( parentNode )
	if parentNode.nSubType > 0 then
		return 0;
	end
	for i=1,table.getn( t_LabelNameList ) do
		if t_LabelNameList[i].nSubType > 0 and t_LabelNameList[i].nGroup  == parentNode.nGroup then
			return i;
		end
	end
	return 0;
end

function SetFriendNameList()
	local t_TmpList = t_LabelNameList;
	local nMaxRelativeRole = GameFriendManager:getRelationPlayerCount( g_nDisplayPageType );
	GameFriendManager:sortFriendInfo( g_nDisplayPageType );

	t_LabelNameList = {};
	
	for i=1,table.getn( t_GroupName ) do
		table.insert( t_LabelNameList,{ name = t_GroupName[i], bCheck = true, nGroup = i, nSubType = 0 } );
		for j=1,nMaxRelativeRole do
			local friendInfo = GameFriendManager:getFriendInfoByIndex( g_nDisplayPageType, j - 1 );
			if friendInfo.m_nGroupID == i then
				table.insert( t_LabelNameList,{ name = friendInfo.m_szName, bCheck = false, nGroup = friendInfo.m_nGroupID, 
						nSubType = 1, nLow = friendInfo.m_RoleID.nLow, nHeight = friendInfo.m_RoleID.nHeight });
			end
		end
	end
	
	--[[
	local nMaxRelativeRole = 100;
	local n = 1;
	for i=1,table.getn( t_GroupName ) do
		table.insert( t_LabelNameList,{ name = t_GroupName[i], bCheck = false, nGroup = i, nSubType = 0 } );		
		for j=n,nMaxRelativeRole do
		table.insert( t_LabelNameList,{ name = j, bCheck = false, nGroup = i, nSubType = i } );
			if math.mod(j,20) == 0 then
				n = j + 1;
				break;
			end
		end
	end
	--]]
	
	if  table.getn( t_TmpList ) < 1 then
		--t_LabelNameList[1].bCheck = true;
	else
		for i=1, table.getn( t_TmpList ) do
			if t_TmpList[i].nSubType == 0 then
				for j=1, table.getn( t_LabelNameList ) do
					if t_LabelNameList[j].nSubType == 0 and t_TmpList[i].nGroup == t_LabelNameList[j].nGroup then
						t_LabelNameList[j].bCheck = t_TmpList[i].bCheck;
						break;
					end
				end
			end
		end
	end
end

function getTotalBtn()
	local btnCount = 0;
	for i=1, table.getn( t_LabelNameList ) do
		if t_LabelNameList[i].nSubType == 0 then
			btnCount = btnCount + 1;
		elseif t_LabelNameList[i].nSubType >= 0 then
			local firstLevelNode = GetFriParentNodeInfo( t_LabelNameList[i] );
			if firstLevelNode ~= nil and firstLevelNode.bCheck then
				btnCount = btnCount + 1;
			end
		end
	end
	return btnCount;
end

function canShowBtn( nIndex )
	if t_LabelNameList[nIndex].nSubType == 0 then
		return true;
	else
		local firstLevelNode = GetFriParentNodeInfo( t_LabelNameList[nIndex] );
		if firstLevelNode ~= nil and firstLevelNode.bCheck then
			return true;
		else
			return false;
		end
	end
end

function UpdateFriendNameList( nIndex )
	ClearRoleNameList();
	SetFriendNameList();
	local nCurShowBtn = 1;
	local shownum = 0;
	for i = 1, table.getn( t_LabelNameList ) do
		if nCurShowBtn > MAX_ROLE_NAME_LINE then
			break;
		end
		if canShowBtn( i ) then
			shownum = shownum + 1;
		end
		if shownum >= nIndex then
			local button	= getglobal( "FriendShipFrame_CharacterNameBtn" .. nCurShowBtn );
			local rich	= getglobal( "FriendShipFrame_CharacterNameBtn" .. nCurShowBtn .. "CharacterName" );
			local chk	= getglobal( "FriendShipFrameTitleSignChk" .. nCurShowBtn );
			local EggsBtn = getglobal( "FriendShipFrame_CharacterNameBtn" .. nCurShowBtn .. "EggsBtn" );
			local FlowerBtn = getglobal( "FriendShipFrame_CharacterNameBtn" .. nCurShowBtn .. "FlowerBtn" );
			local LoveSignBtn = getglobal( "FriendShipFrame_CharacterNameBtn" .. nCurShowBtn .. "LoverSignBtn" );
			if t_LabelNameList[i].nSubType == 0 then
				rich:SetText( t_LabelNameList[i].name, 255, 255, 190 );
				rich:SetPoint( "topleft", button:GetName(), "topleft", 0, 0 )
				button:SetClientUserData( 0, 0 );
				button:SetClientUserData( 1, 0 );
				button:SetClientString( "" );
				jbutton=getglobal( button:GetName() .."Tip");
				jbutton:Hide();
				rich:SetClientUserData( 0, 0 );
				rich:SetClientUserData( 1, 0 );
				rich:SetClientUserData( 2, i );
				rich:SetClientString( t_LabelNameList[i].name );
				chk:setCheckState( t_LabelNameList[i].bCheck );
				EggsBtn:Hide()					
				FlowerBtn:Hide()
				LoveSignBtn:Hide()
				button:Show();
				chk:Show();
				nCurShowBtn = nCurShowBtn + 1;
			elseif t_LabelNameList[i].nSubType > 0 then
				local firstLevelNode = GetFriParentNodeInfo( t_LabelNameList[i] );
				if firstLevelNode ~= nil and firstLevelNode.bCheck then
					rich:SetText( t_LabelNameList[i].name, 160, 160, 90 );
					rich:SetPoint( "topleft", button:GetName(), "topleft", 12, 0 );
					--[[
					button:SetClientUserData( 0, 0 );
					button:SetClientUserData( 1, 0 );
					rich:SetClientUserData( 0, 0 );
					rich:SetClientUserData( 1, 0 );
					--]]
					local friendInfo = GameFriendManager:getFriendInfoByName( t_LabelNameList[i].name );
					-- 获得名字前面的关系前缀名称
					szPrefixName = GetRelatioDesc( friendInfo );

					-- 检查是否要闪烁玩家名字
					if ChatFrameManager:IsNameBlink( t_LabelNameList[i].name ) then
						szPrefixName = "#b" .. szPrefixName;
					end

					
					EggsBtn:Show()			
					EggsBtn:Disable()						
					FlowerBtn:Show()
					FlowerBtn:Disable()
					LoveSignBtn:Hide()	
					if friendInfo.m_nIsOnline == CLIENT_ONLINE then
						rich:SetText( t_LabelNameList[i].name, 255, 255, 190 );
						EggsBtn:Enable()
						FlowerBtn:Enable()						
					elseif friendInfo.m_nIsOnline == MUD_ONLINE then
						rich:SetText( t_LabelNameList[i].name, 160, 160, 190 );
						jbutton=getglobal( button:GetName() .."Tip");
						jbutton:Show();
					else
						rich:SetText( t_LabelNameList[i].name, 160, 160, 160 );
						jbutton=getglobal( button:GetName() .."Tip");
						jbutton:Hide();
					end
					if friendInfo.m_nIntimateColorType == INTIMATE_COLOR_LOVERS_TOLUA then
						local LoverSignBtnTex = getglobal( LoveSignBtn:GetName().."Tex" )
						LoverSignBtnTex:SetTexUV( 952,645,20,17 )						
						LoveSignBtn:Show()					
					elseif friendInfo.m_nIntimateColorType == INTIMATE_COLOR_SPOUSE_TOLUA then
						local LoverSignBtnTex = getglobal( LoveSignBtn:GetName().."Tex" )
						LoverSignBtnTex:SetTexUV( 952,627,20,17 )						
						LoveSignBtn:Show()					
					end 
					button:SetClientUserData( 0, t_LabelNameList[i].nLow );
					button:SetClientUserData( 1, t_LabelNameList[i].nHeight );
					button:SetClientString( t_LabelNameList[i].name );
					rich:SetClientUserData( 0, t_LabelNameList[i].nLow );
					rich:SetClientUserData( 1, t_LabelNameList[i].nHeight );
					rich:SetClientUserData( 2, i );
					rich:SetClientString( t_LabelNameList[i].name );

					button:Show();
					chk:Hide();
					nCurShowBtn = nCurShowBtn + 1;
				end
			end
		end
	end
	for i = nCurShowBtn, MAX_ROLE_NAME_LINE do
		local button	= getglobal( "FriendShipFrame_CharacterNameBtn" .. nCurShowBtn );
		local rich	= getglobal( button:GetName() .. "CharacterName" );
		local chk	= getglobal( "FriendShipFrameTitleSignChk" .. nCurShowBtn );
		button:Hide();
		rich:SetText( "", 255, 254, 189 );
		chk:Hide();
	end
	local nMaxRelativeRole	= GameFriendManager:getRelationPlayerCount( CLIENT_RELATION_TYPE_FRIEND );
	FriendShipFrameFriendNumFont:SetText( nMaxRelativeRole .. "/" .. GameFriendManager:getMaxFriendNum() );
	if nMaxRelativeRole == 0 then
		FriendMutilSendFrame_SendBtn:Disable();
	else
		FriendMutilSendFrame_SendBtn:Enable();
	end
	ResetFriendHighLightTex();	
end

--@Desc:更新玩家列表显示内容
--@return: Nothing
function UpdateRoleNameList( nStartTableLine )
	--首先清空玩家列表原来的玩家名字
	ClearRoleNameList();
	if g_nDisplayPageType == CLIENT_RELATION_TYPE_FRIEND then
		return;
	end

	local nTableNum			= nStartTableLine;
	local character			= {};
	local szPrefixName		= "";
	local nMaxRelativeRole	= GameFriendManager:getRelationPlayerCount( g_nDisplayPageType );
	local rich;
	local button;

	-- 重新排序名字
	GameFriendManager:sortFriendInfo( g_nDisplayPageType );
	local t_trackRoleControl	= GetTrackRoleControl();
	local nCurServeTime	= getServeSecondTime();
	for  i = 1, MAX_ROLE_NAME_LINE do
		-- 若所要显示的玩家名字数目超过实际上已有的玩家数目,则退出
		if nTableNum > nMaxRelativeRole then break end

		button		= getglobal( "FriendShipFrame_CharacterNameBtn" .. i );
		rich		= getglobal( button:GetName() .. "CharacterName" );
		character	= GameFriendManager:getFriendInfoByIndex( g_nDisplayPageType, nTableNum - 1 );
		assert( character ~= nil, "获得了空玩家!" );

		-- 获得名字前面的关系前缀名称
		szPrefixName = GetRelatioDesc( character );

		-- 检查是否要闪烁玩家名字
		if ChatFrameManager:IsNameBlink( character.m_szName ) then
			szPrefixName = "#b" .. szPrefixName;
		end

		button:SetClientUserData( 0, character.m_RoleID.nLow );
		button:SetClientUserData( 1, character.m_RoleID.nHeight );
		button:SetClientString( character.m_szName );

		rich:SetClientUserData( 0, character.m_RoleID.nLow );
		rich:SetClientUserData( 1, character.m_RoleID.nHeight );
		rich:SetClientString( character.m_szName );
		rich:SetPoint( "topleft", button:GetName(), "topleft", -20, 0 )

		-- 若玩家在线或者玩家为我的临时好友,则玩家名字显示为绿色
			--2表示挂机在线
		if character.m_nIsOnline == CLIENT_ONLINE then
			rich:SetText( szPrefixName .. character.m_szName, 255, 254, 189 );
		elseif character.m_nIsOnline == MUD_ONLINE then
			rich:SetText( szPrefixName .. character.m_szName, 255, 254, 189 );
			jbutton=getglobal("FriendShipFrame_CharacterNameBtn" .. i .."Tip");
			jbutton:Show();
		else
			rich:SetText( szPrefixName .. character.m_szName, 160, 160, 160 );
			jbutton=getglobal("FriendShipFrame_CharacterNameBtn" .. i .."Tip");
			jbutton:Hide();
		end
		button:Show();
		
		if g_nDisplayPageType == CLIENT_RELATION_TYPE_SU_DI and character.m_nGprsEnd ~= 0 then
			local font	= getglobal( button:GetName().."LeaveTimeFont" );
			local leaveTime = math.max( character.m_nGprsEnd - nCurServeTime, 0 );
			
			t_trackRoleControl:addTrackRole( {	name = character.m_szName, endTime = character.m_nGprsEnd, 
												map = 0, referMap = 0, x = 0, y = 0,
											} 
											);
			if leaveTime <= 60 then
				font:SetText( "剩余定位时间"..leaveTime.."秒" );
			else
				font:SetText( "剩余定位时间"..math.ceil( leaveTime / 60 ).."分" );
			end
		end

		-- 递增计数器,用于访问下一个好友
		nTableNum = nTableNum + 1;
	end

	ResetHighLightTex();--固定原来点击的高亮条

	UpdateSuDiTrackTime();
end

function ClearSuDiTrackTime()
	for i = 1, MAX_ROLE_NAME_LINE do
		local font = getglobal( "FriendShipFrame_CharacterNameBtn" .. i .. "LeaveTimeFont" );
		font:SetText( "" );
	end
end

function UpdateSuDiTrackTime()
	if not FriendShipFrame:IsShown() or g_nDisplayPageType ~= CLIENT_RELATION_TYPE_SU_DI then
		return;
	end
	ClearSuDiTrackTime();
	local t_trackRoleControl	= GetTrackRoleControl();
	local t_roleQueue			= t_trackRoleControl:getAllTrackRoles();
	local nCurServeTime			= getServeSecondTime();

	for  i = 1, MAX_ROLE_NAME_LINE do
		local button		= getglobal( "FriendShipFrame_CharacterNameBtn" .. i );
		local font			= getglobal( button:GetName().."LeaveTimeFont" );
		local playerName	= button:GetClientString();
		local t_oneTrackInfo= t_roleQueue[playerName];
		if t_oneTrackInfo ~= nil then
			local leaveTime = math.max( t_oneTrackInfo["endTime"] - nCurServeTime, 0 );
			if leaveTime <= 60 then
				font:SetText( "剩余定位时间"..leaveTime.."秒" );
			else
				font:SetText( "剩余定位时间"..math.ceil( leaveTime / 60 ).."分" );
			end
		end
	end
end

function FriendDelete_ConfirmFrame_OnShow()
	this:SetPoint( "center", "$parent", "center", 0, 0 );
end

function FriendDelete_ConfirmFrame_ConfirmBtn_OnClick()
	if FriendInfoFrame:IsShown() then
		local frame = getglobal( "FriendInfoFrame" );
		if frame:GetClientString() == "" then 
			ShowMidTips( "错误的玩家信息" );
			FriendInfoFrame:Hide();
			return;
		end
		GameFriendManager:delRelation( CLIENT_RELATION_TYPE_FRIEND, frame:GetClientUserData( 1 ), frame:GetClientUserData( 0 ) );
	else
		GameFriendManager:delRelation( g_nDisplayPageType, g_CurClickRole.roleHeightId, g_CurClickRole.roleLowId );
	end
	PostDeleteAction();
	FriendDelete_ConfirmFrame:Hide();
	FriendInfoFrame:Hide();
end

function FriendDelete_ConfirmFrame_Cancel_OnClick()
	FriendDelete_ConfirmFrame:Hide();
end

function PostDeleteAction()
	--检查当前打开的聊天控件中是否有与被删除的人相关的控件
	local chatFrame;
	local chatRecordFrame;
	local dwindleIcon;
	local chatDisplayRich;
	local chatRecordDisplayRich;
	
	local szFrameName = ChatFrameManager:GetSpeakerFrame( g_CurClickRole.roleName );
 
	-- 检查是否有与玩家对应的聊天窗口,若有则关闭对应的聊天窗口
	if szFrameName ~= nil then
		ChatFrameManager:ShutDownChatFrame( szFrameName );
	end	
	
	-- 检查聊天队列中是否对应的玩家名字
	ChatFrameManager:DelRoleNameRecord( g_CurClickRole.roleName );
	g_CurClickRole = { roleLowId = 0, roleHeightId = 0, roleName = "" };
end

--@Desc:删除某个指定玩家名字
function DeleteCharacterName( )
	-- 若没有选中任何玩家, 不做任何操作
	if ( g_CurClickRole.roleName == "" ) then
		ShowMidTips( "请先选定一个玩家" );
		return;
	end

	-- 若为临时好友
	if g_nDisplayPageType == CLIENT_RELATION_TYPE_TMPFRIEND or g_nDisplayPageType == CLIENT_RELATION_TYPE_XIONG_SHOU then
		GameFriendManager:deleteRole( g_nDisplayPageType, g_CurClickRole.roleName );
	elseif g_nDisplayPageType == CLIENT_RELATION_TYPE_FRIEND then
		FriendDelete_ConfirmFrame:Show();
		return;
	else
		GameFriendManager:delRelation( g_nDisplayPageType, g_CurClickRole.roleHeightId, g_CurClickRole.roleLowId );
	end
	
	PostDeleteAction();
end

--@Desc:加载好友面板时候注册消息
function FriendShipFrame_OnLoad()
	this:RegisterEvent( "GE_FRIEND_GETLIST" );
	this:RegisterEvent( "GE_FRIEND_UPDATAEMOTION" );
	this:RegisterEvent( "GE_FRIEND_GETMSG" );--用于接收与其他玩家的聊天信息
	this:RegisterEvent( "GE_FRIEND_ADDFRIENDINFO" );
	this:RegisterEvent( "GE_TEAM_LEAVETEAM" );
	this:RegisterEvent( "GE_INVITE_PK");
	this:RegisterEvent( "GE_UPDATE_FRIENDCHATFRAME_BY_CLIENT");
	this:RegisterEvent( "GE_ENTER_PLAYERLOGIN" );
	this:RegisterEvent( "GE_WINDOW_GETMSG" );--特定窗口聊天
	this:RegisterEvent( "GE_UPDATE_WINDOWCHATFRAME_BY_CLIENT");
end

-- t_requestAddFriend = { { name, startTime } }
local t_requestAddFriend = {};
-----------------------------------------结婚相关--------------------------------
--结婚相关类型，1表白，2解除关系
PURSUE_INFO_TYPE = 1
REMOVE_RELATION_TYPE = 2
--结婚相关请求，像表白这些，存在此表里
-- t_requestMarryInfo = { { name, nType, nRelationType } }
local t_requestMarryInfo = {}

-- 检测指定玩家是否已经在好友申请列表中
function isRoleInRequestList( szRoleName )
	for _, data in ipairs( t_requestAddFriend ) do
		if data.name == szRoleName then
			ShowMidTips( data.name .. "已向你发出了好友申请" );
			return true;
		end
	end

	-- 再检查该好友是否已经在好友列表
	if GameFriendManager:getPlayerRelation( szRoleName ) == CLIENT_RELATION_TYPE_FRIEND then
		ShowMidTips( szRoleName .. "已经在你的好友列表了" );
		return true;
	end

	return false;
end

-- 更新好友请求信息列表
function UpdateAddFriendList()
	local nCurTime = GameMgr:getTickTime();
	for index, data in ipairs( t_requestAddFriend ) do
		if ( nCurTime - data.startTime ) >= 15000 then
			GameFriendManager:addFriendRes( data.name, 0, 1, true );
			ShowMidTips( "你忽略了" .. data.name .. "对你的好友申请" );
			-- 若当前过期的信息是由正在显示的好友申请面板处理中，则通过关闭面板，移除相关信息
			if data.name == g_szReqCharacterName then
				RequestAddFriendFrame:Hide();
			else
				table.remove( t_requestAddFriend, index );
				
				if index == 1 then
					-- 若是第一个索引的被移除则停止播放淡入淡出特效
					util.stopFace( "EffectFrame_Texture" );
					EffectFrame:Hide();
					-- 若是第一个索引的被移除且申请面板没弹出的状态下且申请列表没空，则表示要重新弹出新的闪光框
					if table.getn( t_requestAddFriend ) > 0 and not RequestAddFriendFrame:IsShown() then
						GuideEffectShow( "", "", "FriendsButton", "玩家"..t_requestAddFriend[1].name.."申请\n加你为好友",
										"uires\\ZhuJieMian\\1.dds", 352, 609, 60, 60, 50, 60, -9, 5, 11, -51, true );
					end
				end
			end
		end
	end
end

-- 快捷栏的好友
function FriendsButtonOnClick()
	-- 关闭队伍邀请提示
	GuideEffectHide( "FriendsButton" );
	FriendsButton:SetGray(false);
	-- 若有人申请加好友，则关闭提示特效，且弹出申请好友面板，而不是好友主界面
	if table.getn( t_requestAddFriend ) > 0 then
		g_szReqCharacterName	= t_requestAddFriend[1].name;
		
		local szAddFriendDesc = "\n#P"..g_szReqCharacterName .. "  申请加你为好友！";
		if GameFriendManager:isPlayerRelation( CLIENT_RELATION_TYPE_BLACK, g_szReqCharacterName ) then
			szAddFriendDesc = g_szReqCharacterName .. "  申请加你为好友，但对方在你的黑名单中，是否同意？";
		elseif GameFriendManager:isPlayerRelation( CLIENT_RELATION_TYPE_SU_DI, g_szReqCharacterName ) then
			szAddFriendDesc = g_szReqCharacterName .. "  申请加你为好友，但对方在你的宿敌名单中，是否同意？";
		end
		
		local t_riches = 
		{ ["RequestAddFriendFrame_DescRich"] = 
			{ 
				value = szAddFriendDesc, 
				color = { r = 255, g = 229, b = 189 } 
			}, 
		};
		util.SetRiches( t_riches );
		
		RequestAddFriendFrame:SetClientString( g_szReqCharacterName );
		RequestAddFriendFrame:Show();
		-- 弹出对应的好友申请面板时候，重置它的初始计时器
	-- 有结婚相关消息，弹出对应的面板
	elseif table.getn( t_requestMarryInfo ) > 0 then
		ShowMarryInfosFrame();
	-- 若没有人申请加好友，则弹出好友主界面，加一条且没有结婚对应消息
	else
		if ( FriendShipFrame:IsShown() ) then
			FriendShipFrame:Hide();
		elseif ( TrustOrderListFrame:IsShown() ) then
			TrustOrderListFrame:Hide();
		else
			FriendShipFrame:SetPoint( "topright", "$parent", "topright", -160, 130 );
			FriendShipFrame:Show();
		end
	end
end

local t_events =	{	["GE_FRIEND_GETLIST"] = {},			["GE_FRIEND_UPDATAEMOTION"] = {},
						["GE_FRIEND_ADDFRIENDINFO"] = {},	["GE_FRIEND_GETMSG"] = {},
						["GE_UPDATE_FRIENDCHATFRAME_BY_CLIENT"] = {},	["GE_FRIEND_CHGNAME"] = {},
					["GE_ENTER_PLAYERLOGIN"] = {}, ["GE_WINDOW_GETMSG"] = {},
					["GE_UPDATE_WINDOWCHATFRAME_BY_CLIENT"] = {}, };
-- 收到更新自己好友列表的消息
t_events["GE_FRIEND_GETLIST"].func = 
function()
	if g_nDisplayPageType == CLIENT_RELATION_TYPE_FRIEND then
		local slider = getglobal( "FriendShipFrame_ScrollBar" );
		SetScrollBar( "FriendShipFrame_ScrollBar", MAX_ROLE_NAME_LINE, getTotalBtn() );
		UpdateFriendNameList( slider:GetValue() + 1 );
	elseif g_nDisplayPageType == CLIENT_RELATION_TYPE_PRENTICE then
		local slider = getglobal( "FriendShipFrame_ScrollBar" );
		UpdateSchoolNameList( slider:GetValue() + 1 );
	else
		SetFriendFrameScrollBar();
		local slider = getglobal( "FriendShipFrame_ScrollBar" );
		UpdateRoleNameList( slider:GetValue() + 1 );
	end
end

t_events["GE_FRIEND_CHGNAME"].func = 
function()
	t_events["GE_FRIEND_GETLIST"].func();
end

-- 收到更新自己的心情签名的消息
t_events["GE_FRIEND_UPDATAEMOTION"].func = 
function()
	if GameFriendManager:getEmotion() ~= "" then
		FriendShipFrame_EmotionEdit:SetText( GameFriendManager:getEmotion() );
		FriendShipFrame_EmotionShowFont:SetText( GameFriendManager:getEmotion() );
	else
		FriendShipFrame_EmotionEdit:SetText( "请输入心情签名" );
		FriendShipFrame_EmotionShowFont:SetText( "请输入心情签名" );
	end
end

-- 收到别人请求加我为好友的消息
t_events["GE_FRIEND_ADDFRIENDINFO"].func = 
function()
	local UImsg				= UIMSG:getUIMsg();
	local addFriendInfo		= UImsg.AddFriendInfo;
	
	if ShowChangeRoleFrame:IsShown() then
		GameFriendManager:addFriendRes( addFriendInfo.m_szReqName, 0, 1 );
		return;
	end

	-- 先判定当前申请加为好友的人是否已经是正在弹出面板的人，若是，则不接收该人的重复申请消息
	if g_szReqCharacterName == addFriendInfo.m_szReqName then
		return;
	end

	-- 若申请列表里面已经有这个人也不接受
	for _, data in ipairs( t_requestAddFriend ) do
		if data.name == addFriendInfo.m_szReqName then
			return;
		end
	end
	
	-- 将申请好友人的信息存储到缓存中
	table.insert( t_requestAddFriend, { name = addFriendInfo.m_szReqName, startTime = GameMgr:getTickTime() } );
	if not CanBeginShortcutAction() then
		return;
	end

	if table.getn( t_requestAddFriend ) == 1 then
		GuideEffectShow( "", "", "FriendsButton", "玩家"..t_requestAddFriend[1].name.."申请\n加你为好友",
						"uires\\ZhuJieMian\\1.dds", 352, 609, 60, 60, 50, 60, -9, 5, 11, -51, true );
	end		
end

t_events["GE_ENTER_PLAYERLOGIN"].func = 
function()
	t_LabelNameList = {};
end

function CheckAddFriendReqestsAfteAnim()
	if not CanBeginShortcutAction() then
		return;
	end

	local nCurTime = GameMgr:getTickTime();
	for index, data in ipairs( t_requestAddFriend ) do
		data["startTime"] = nCurTime;
	end
	
	if table.getn( t_requestAddFriend ) > 0 then
		GuideEffectShow( "", "", "FriendsButton", "玩家"..t_requestAddFriend[1].name.."申请\n加你为好友",
							"uires\\ZhuJieMian\\1.dds", 352, 609, 60, 60, 50, 60, -9, 5, 11, -51, true );
	end	
end

--收到窗口聊天信息
t_events["GE_WINDOW_GETMSG"].func =
function()
	local msg		= UIMSG:getUIMsg();
	local nType		= msg.ChatMsg.Type;
	local szSpeakerName	= msg.ChatMsg.Speaker;
	local szChatContent	= msg.ChatMsg.Content;
	local szFrameName	= "";
	ChatFrameManager:RecordRoleName( szSpeakerName );
	-- 更新消息队列
	local nTime = os.date();
	ChatFrameManager:ReceiveMessage( "#c23f032"..szSpeakerName.." 说：    （"..nTime.."）#n",	szSpeakerName );
	if msg.ChatMsg.IsCluster == 1 then
		ChatFrameManager:ReceiveMessage( "#cf0f000（好友群发消息）：#cfffebd"..szChatContent.."#n",		szSpeakerName );
	else
		ChatFrameManager:ReceiveMessage( "#cfffebd"..szChatContent.."#n",					szSpeakerName );
	end
	-- 若新收到的消息的发送人没有与当前打开的聊天窗口有关系，
	if ChatFrameManager:GetSpeakerFrame( szSpeakerName ) == nil then
		--在好友或者师徒列表
		if GameFriendManager:getFriendInfoByName( szSpeakerName ) ~= nil or GameSchool:IsSelfSchool( szSpeakerName ) then
			-- 让对应的名字闪动
			BlinkName( szSpeakerName );
		end
		-- 若发送者名字列表框也没显示，则 显示提示框
		if FriendFrame_ChatListFrame:IsShown() then
			UpdateChatListFrame();
		else
			FriendFrame_ChatClueFrame:Show();
			FriendFrame_ChatClueFrame:SetClientUserData( 0, nType )
		end
		return;
	end
	
	-- 发送消息的人与当前已打开的某一个聊天窗口A对应
	szFrameName		= ChatFrameManager:GetSpeakerFrame( szSpeakerName );
	local chatFrame	= getglobal( szFrameName );
	local rich		= getglobal( szFrameName .. "_DisplayRich" );
	local slider	= getglobal( szFrameName.."_ScrollBar" );
	rich:AddText( "#c23f032"..szSpeakerName .. " 说：    （"..nTime.."）#n", 254, 242, 216 );
	if msg.ChatMsg.IsCluster == 1 then
		rich:AddText( "#cf0f000（好友群发消息）：#cfffebd"..szChatContent.."#n",254, 242, 216 );		
	else
		rich:AddText( "#cfffebd"..szChatContent.."#n",				254, 242, 216 );
	end

	rich:ScrollEnd();
	local nRichAccurateViewLines	= rich:GetAccurateViewLines();
	local nTextLines				= rich:GetTextLines();
	SetScrollBar( szFrameName.."_ScrollBar", nRichAccurateViewLines, nTextLines );
	slider:SetValue( slider:GetMaxValue() );
	util.UpdateRich( rich:GetName(), slider:GetValue() );
end
	

-- 收到好友发送的消息
t_events["GE_FRIEND_GETMSG"].func = 
function()
	local msg		= UIMSG:getUIMsg();
	local nType		= msg.ChatMsg.Type;
	local szSpeakerName	= msg.ChatMsg.Speaker;
	local szChatContent	= msg.ChatMsg.Content;
	local character		= GameFriendManager:getFriendInfoByName( szSpeakerName );
	local szFrameName	= "";
	
	--[[
	-- 发送消息的人跟我没关系，则将其加为临时好友
	if character == nil then
		GameFriendManager:addTmpFriend( szSpeakerName );
	end
	--]]

	-- 发送消息的人处于我的黑名单中的人， 则不接收消息,什么也不做
	if character ~= nil and character.m_nRelationType == CLIENT_RELATION_TYPE_BLACK then
		return;
	end
	
	ChatFrameManager:RecordRoleName( szSpeakerName );
	-- 更新消息队列
	local nTime = os.date();
	ChatFrameManager:ReceiveMessage( "#c23f032"..szSpeakerName.." 说：    （"..nTime.."）#n",	szSpeakerName );
	if msg.ChatMsg.IsCluster == 1 then
		ChatFrameManager:ReceiveMessage( "#cf0f000（好友群发消息）：#cfffebd"..szChatContent.."#n",		szSpeakerName );
	else
		ChatFrameManager:ReceiveMessage( "#cfffebd"..szChatContent.."#n",					szSpeakerName );
	end
	-- 若新收到的消息的发送人没有与当前打开的聊天窗口有关系，
	if ChatFrameManager:GetSpeakerFrame( szSpeakerName ) == nil then
		-- 让对应的名字闪动
		BlinkName( szSpeakerName );
		-- 若发送者名字列表框也没显示，则 显示提示框
		if FriendFrame_ChatListFrame:IsShown() then
			UpdateChatListFrame();
		else
			FriendFrame_ChatClueFrame:Show();
		end
		return;
	end

	-- 若消息发送者名字列表框处于显示状态，则刷新名字列表
	
	-- 发送消息的人与当前已打开的某一个聊天窗口A对应
	szFrameName		= ChatFrameManager:GetSpeakerFrame( szSpeakerName );
	local chatFrame	= getglobal( szFrameName );
	local rich		= getglobal( szFrameName .. "_DisplayRich" );
	local slider	= getglobal( szFrameName.."_ScrollBar" );
	rich:AddText( "#c23f032"..szSpeakerName .. " 说：    （"..nTime.."）#n", 254, 242, 216 );
	if msg.ChatMsg.IsCluster == 1 then
		rich:AddText( "#cf0f000（好友群发消息）：#cfffebd"..szChatContent.."#n",254, 242, 216 );		
	else
		rich:AddText( "#cfffebd"..szChatContent.."#n",				254, 242, 216 );
	end

	rich:ScrollEnd();
	local nRichAccurateViewLines	= rich:GetAccurateViewLines();
	local nTextLines				= rich:GetTextLines();
	SetScrollBar( szFrameName.."_ScrollBar", nRichAccurateViewLines, nTextLines );
	slider:SetValue( slider:GetMaxValue() );
	util.UpdateRich( rich:GetName(), slider:GetValue() );

	-- 检测聊天记录框是否显示,若处于显示状态,则将聊天信息也更新到聊天信息框上
	--[[local szRecordFrame = szFrameName .. "_RecordFrame";
	local recordFrame	= getglobal( szRecordFrame );

	if recordFrame:IsShown() then
		local recordRich = getglobal( recordFrame:GetName() .. "_RecordDisplayRich" );
		local slider	 = getglobal( szRecordFrame .. "_ScrollBar" );
		recordRich:AddText( szSpeakerName .. "	" .. nTime, 254, 242, 216 );
		recordRich:AddText( szChatContent,				254, 242, 216 );
		DisplayPageNum( chatFrame:GetClientID() );
		SetFriendChatRecordFrameScrollBar( recordFrame:GetName() );
		slider:SetValue( slider:GetMaxValue() );
		-- 再调整richtext的显示内容
		UpdateChatRecordRich( recordRich:GetName(), slider:GetName() );
	end
	--]]
end

t_events["GE_UPDATE_WINDOWCHATFRAME_BY_CLIENT"].func =
function()
	local Msg			= UIMSG:getUIMsg();
	local szChatContent	= Msg.ChatReq.Content;
	local szSpeakerName	= Msg.ChatReq.ToName;
	local mainplayer	= ActorMgr:getMainPlayer();
	local szMyName		= mainplayer:getName();
	local szFrameName	= ChatFrameManager:GetSpeakerFrame( szSpeakerName );
	local inputEdit		= getglobal( szFrameName.."_InputEdit" );
	local chatFrame		= getglobal( szFrameName );

	-- 不允许显示空消息
	if "" == szChatContent then return end

	-- 将我发送的消息压入消息队列中
	local nTime = os.date();
	ChatFrameManager:ReceiveMessage( "#c00faf0你 说：    （"..nTime.."）#n",	szSpeakerName );
	ChatFrameManager:ReceiveMessage( "#cfffebd"..szChatContent.."#n",			szSpeakerName );

	-- 先将发送的消息更新我的聊天面板的聊天记录上
	local displayRich	= getglobal( szFrameName .. "_DisplayRich" );
	local slider		= getglobal( szFrameName .. "_ScrollBar" );
	displayRich:AddText( "#c00faf0你 说：    （"..nTime.."）#n".."#n",	255, 229, 189 );

	displayRich:AddText( "#cfffebd"..szChatContent.."#n",				255, 229, 189 );
	
	displayRich:ScrollEnd();
	local nRichAccurateViewLines	= displayRich:GetAccurateViewLines();
	local nTextLines				= displayRich:GetTextLines();
	SetScrollBar( slider:GetName(), displayRich:GetAccurateViewLines(), displayRich:GetTextLines() );
	slider:SetValue( slider:GetMaxValue() );
	util.UpdateRich( szFrameName.."_DisplayRich", slider:GetValue() );
end

t_events["GE_UPDATE_FRIENDCHATFRAME_BY_CLIENT"].func = 
function()
	local Msg			= UIMSG:getUIMsg();
	local szChatContent	= Msg.ChatReq.Content;
	local szSpeakerName	= Msg.ChatReq.ToName;
	local mainplayer	= ActorMgr:getMainPlayer();
	local szMyName		= mainplayer:getName();
	local szFrameName	= ChatFrameManager:GetSpeakerFrame( szSpeakerName );
	local inputEdit		= getglobal( szFrameName.."_InputEdit" );
	local chatFrame		= getglobal( szFrameName );

	-- 不允许显示空消息
	if "" == szChatContent then return end

	-- 将我发送的消息压入消息队列中
	local nTime = os.date();
	ChatFrameManager:ReceiveMessage( "#c00faf0你 说：    （"..nTime.."）#n",	szSpeakerName );
	ChatFrameManager:ReceiveMessage( "#cfffebd"..szChatContent.."#n",			szSpeakerName );

	-- 先将发送的消息更新我的聊天面板的聊天记录上
	local displayRich	= getglobal( szFrameName .. "_DisplayRich" );
	local slider		= getglobal( szFrameName .. "_ScrollBar" );
	displayRich:AddText( "#c00faf0你 说：    （"..nTime.."）#n".."#n",	255, 229, 189 );

	displayRich:AddText( "#cfffebd"..szChatContent.."#n",				255, 229, 189 );
	
	displayRich:ScrollEnd();
	local nRichAccurateViewLines	= displayRich:GetAccurateViewLines();
	local nTextLines				= displayRich:GetTextLines();
	SetScrollBar( slider:GetName(), displayRich:GetAccurateViewLines(), displayRich:GetTextLines() );
	slider:SetValue( slider:GetMaxValue() );
	util.UpdateRich( szFrameName.."_DisplayRich", slider:GetValue() );
end

--@Desc:收到消息刷新面板
function FriendShipFrame_OnEvent()
	if t_events[arg1] == nil then
		return;
	end

	t_events[arg1].func();
end

function FriendShipFrame_OnUpdate()
	local mainplayer = ActorMgr:getMainPlayer()
	FriendShipFrameSelfMeiLiHeadFont:SetText( "魅力值："..mainplayer:getCharm() )
end

--@Desc:此为弹出好友面板的显示方法
--@return: void
function FriendShipFrame_OnShow()		  
	-- 重置窗口坐标
	--FriendShipFrame:SetPoint("topright", "UIClient", "topright", -91, 83 );
	-- 将好友面板上面的6个分类卡都变为enable状态,并将好友分页卡置为可用状态
	ChangePageState( "FriendShipFrameFriendBtn" )
	
	--显示魅力值
	
	local mainplayer = ActorMgr:getMainPlayer()
	FriendShipFrameSelfMeiLiHeadFont:SetText( "魅力值："..mainplayer:getCharm() )
	
	FriendShipFrame_EmotionShowBtn:Show();
	FriendShipFrame_EmotionEdit:Hide();
	-- 设置好友面板默认显示好友列表
	g_nDisplayPageType = CLIENT_RELATION_TYPE_FRIEND
	
	LoginMgr:sendNewPeopleRequest();
	
	local slider = getglobal( "FriendShipFrame_ScrollBar" );
	slider:SetValue( 0 );
	SetScrollBar( "FriendShipFrame_ScrollBar", MAX_ROLE_NAME_LINE, getTotalBtn() );
	UpdateFriendNameList(1);

	-- 设置主角的心情签名
	local editBox	= getglobal( "FriendShipFrame_EmotionEdit" );
	if GameFriendManager:getEmotion() ~= "" then
		editBox:SetText( GameFriendManager:getEmotion() );
		FriendShipFrame_EmotionShowFont:SetText( GameFriendManager:getEmotion() );
	else
		editBox:SetText( "请输入心情签名" );
		FriendShipFrame_EmotionShowFont:SetText( "请输入心情签名" );
	end

	-- 设置好友主面板下面要显示的按钮
	
	local t_showIcons = { "FriendShipFrameAddFriendBtn", "FriendShipFrameMutilSendBtn", "FriendShipFrameFriendNumBtn" };
	util.ShowIcons( t_showIcons );
	
	local t_hideIcons = { "FriendShipFrameAddBlackBtn", "FriendShipFrameZhuiZongBtn", "FriendShipFrameDeleteBtn", };
	util.HideIcons( t_hideIcons );
end

function FriendShipFrame_OnHide()
	LoginMgr:clearNewPeopleInfo();
end

function FriendShipFrame_ModifyEmotion()
	-- 隐藏前次操作中的高亮条和右键选项
	HideAllHighLightTex();
	-- 让原来上一次操作被选中的玩家被取消选定状态
	g_CurClickRole = { roleLowId = 0, roleHeightId = 0, roleName = "" };

	local edit			= getglobal( "FriendShipFrame_EmotionEdit" );
	local szEditEmotion = edit:GetText();
	GameFriendManager:changeEmotion( szEditEmotion );
end

--@brief: 点击页面切换按钮的时候进行一些页面状态设置
--@return: void
function ChangePageState( szButtonName )
	--首先让原来处于disabled状态的button恢复可用的状态
	local button;
	for i = 1, table.getn( g_friendFrameButtons.buttonName ) do
		button = getglobal( g_friendFrameButtons.buttonName[i] );
		if not ( button:IsEnable( ) ) then
			button:Enable();
			button:DisHighlight();
        end
	end

	button = getglobal( szButtonName );
	button:Disable();
	-- 隐藏所有的高亮条
	HideAllHighLightTex();
end

-- 点击分类标签卡后的显示信息
local t_FenLeiLabelInfo =	{
								-- 好友
								["FriendShipFrameFriendBtn"] =	{	relationType = CLIENT_RELATION_TYPE_FRIEND, 
																	showButtons = { "FriendShipFrameAddFriendBtn", "FriendShipFrameMutilSendBtn", "FriendShipFrameFriendNumBtn" },
																	hideButtons = { "FriendShipFrameAddBlackBtn", "FriendShipFrameZhuiZongBtn", "FriendShipFrameDeleteBtn" } 
																},
								-- 临时
								["FriendShipFrameTempBtn"] =	{	relationType = CLIENT_RELATION_TYPE_TMPFRIEND, 
																	showButtons = { "FriendShipFrameDeleteBtn", "FriendShipFrameAddFriendBtn" },
																	hideButtons = { "FriendShipFrameAddBlackBtn", "FriendShipFrameZhuiZongBtn", "FriendShipFrameMutilSendBtn", "FriendShipFrameFriendNumBtn" } 
																},
								-- 结拜
								["FriendShipFrameBrotherBtn"] =	{	relationType = CLIENT_RELATION_TYPE_BROTHER, 
																	showButtons = { },
																	hideButtons = { "FriendShipFrameDeleteBtn", "FriendShipFrameAddFriendBtn",
																					"FriendShipFrameAddBlackBtn", "FriendShipFrameZhuiZongBtn", "FriendShipFrameMutilSendBtn", "FriendShipFrameFriendNumBtn" } 
																},
								-- 师徒
								["FriendShipFramePrenticeBtn"] ={	relationType = CLIENT_RELATION_TYPE_PRENTICE, 
																	showButtons = { },
																	hideButtons = { "FriendShipFrameDeleteBtn", "FriendShipFrameAddFriendBtn",
																					"FriendShipFrameAddBlackBtn", "FriendShipFrameZhuiZongBtn", "FriendShipFrameMutilSendBtn", "FriendShipFrameFriendNumBtn" } 
																},
								-- 宿敌
								["FriendShipFrameKillBtn"]		=	{	relationType = CLIENT_RELATION_TYPE_SU_DI, 
																		showButtons = { "FriendShipFrameDeleteBtn", "FriendShipFrameZhuiZongBtn" },
																		hideButtons = {  "FriendShipFrameAddFriendBtn", "FriendShipFrameAddBlackBtn", "FriendShipFrameMutilSendBtn", "FriendShipFrameFriendNumBtn" } 
																	},
								-- 凶手
								["FriendShipFrameEnemyBtn"]		=	{	relationType = CLIENT_RELATION_TYPE_XIONG_SHOU, 
																		showButtons = { "FriendShipFrameDeleteBtn" },
																		hideButtons = {  "FriendShipFrameAddFriendBtn", "FriendShipFrameAddBlackBtn",
																						"FriendShipFrameZhuiZongBtn", "FriendShipFrameMutilSendBtn", "FriendShipFrameFriendNumBtn" } 
																	},
								-- 黑名单
								["FriendShipFrameBlacklistBtn"]	=	{	relationType = CLIENT_RELATION_TYPE_BLACK, 
																		-- "FriendShipFrameAddBlackBtn"
																		showButtons = { "FriendShipFrameDeleteBtn",  },
																		hideButtons = {  "FriendShipFrameAddFriendBtn", "FriendShipFrameZhuiZongBtn", "FriendShipFrameMutilSendBtn", "FriendShipFrameFriendNumBtn" } 
																	},
							};

local t_doModalFrames = { "FriendDelete_ConfirmFrame" };
function Click_FriendShipFrame_FenLeiLable( )
	for _, name in ipairs( t_doModalFrames ) do
		local frame = getglobal( name );
		if frame:IsShown() then
			ShowMidTips( "请先关闭已经打开的二级确认面板" );
			return;
		end
	end

	-- 设置页面上上一次操作出现的控件显示状态
	ChangePageState( this:GetName() );
	-- 设置当前被选定的角色信息
	g_CurClickRole = { roleLowId = 0, roleHeightId = 0, roleName = "" };
	-- 设置当前选定的页面
	g_nDisplayPageType = t_FenLeiLabelInfo[this:GetName()].relationType;

	-- 设置页面的滚动条显示状态并刷新所显示的玩家信息
	if g_nDisplayPageType == CLIENT_RELATION_TYPE_FRIEND then
		local slider = getglobal( "FriendShipFrame_ScrollBar" );
		slider:SetValue( 0 );
		SetScrollBar( "FriendShipFrame_ScrollBar", MAX_ROLE_NAME_LINE, getTotalBtn() );
		UpdateFriendNameList( slider:GetValue() + 1 );
	elseif g_nDisplayPageType == CLIENT_RELATION_TYPE_PRENTICE then
		local slider = getglobal( "FriendShipFrame_ScrollBar" );
		UpdateSchoolNameList( slider:GetValue() + 1 );
	else
		local slider = getglobal( "FriendShipFrame_ScrollBar" );
		slider:SetValue( 0 );
		SetFriendFrameScrollBar();
		UpdateRoleNameList( slider:GetValue() + 1 );
	end

	for _, szButtonName in ipairs( t_FenLeiLabelInfo[this:GetName()].showButtons ) do
		local btn = getglobal( szButtonName );
		btn:Show();
	end
	
	for _, szButtonName in ipairs( t_FenLeiLabelInfo[this:GetName()].hideButtons ) do
		local btn = getglobal( szButtonName );
		btn:Hide();
	end
	
	--如果打开的是师徒标签页
	if g_nDisplayPageType == CLIENT_RELATION_TYPE_PRENTICE then
		FriendShipFramePrenticeBtnUVAnimationTex:Hide()
		UpdateSchoolRelationShip()
	end
end

function FriendShipFrameNotKaiFaBtn_OnEnter()
	local szText = "功能暂未开放，敬请期待";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

--@Desc:此为点击添加好友的时候响应
--@return: Nothing
function FriendShipFrame_AddFriend_OnClick()
	if FriendAddCharacterFrame:IsShown() then
		return;
	end 
	g_nAddType = CLIENT_RELATION_TYPE_FRIEND;
	
	FriendAddCharacterFrame:Show();
end

--@Desc:此为点击添加黑名单的时候响应
function FriendShipFrame_AddBlack_OnClick()
	if FriendAddCharacterFrame:IsShown() then
		return;
	end 

	g_nAddType = CLIENT_RELATION_TYPE_BLACK;

	FriendAddCharacterFrame:Show();
	FriendAddCharacterFrame_InputEdit:SetTextColor( 255, 229, 189 );
end

--@Desc:此为点击删除的时候响应
--@return: Nothing
function FriendShipFrame_Delete_OnClick()
	DeleteCharacterName();
end

-- 追踪宿敌列表中的玩家
function FriendShipFrameZhuiZongBtn_OnClick()
	-- 若没有选中任何玩家, 不做任何操作
	if g_CurClickRole.roleName == "" or g_CurClickRole.roleName == nil then
		ShowMidTips( "请选择需要定位的玩家名字" );
		return;
	end

	local t_trackRoleControl = GetTrackRoleControl();
	t_trackRoleControl:beginTrack( { name = g_CurClickRole.roleName, btnName = this:GetName(); } );
end

local t_diDuiTipsRelation = { CLIENT_RELATION_TYPE_XIONG_SHOU, CLIENT_RELATION_TYPE_BLACK, CLIENT_RELATION_TYPE_SU_DI };
function isDiDuiRelation( relation )
	for _, relationType in ipairs( t_diDuiTipsRelation ) do
		if relationType == relation then
			return true;
		end
	end

	return false;
end

function FriendShipFrame_CharacterNameBtn_OnEnter()
	-- 若所靠近的按钮没存储任何玩家信息,则什么也不做结束子程序
	local parentFrame	= getglobal( this:GetParent() );
	-- TODO:	这要重构为自动显示高亮条
	local highLightTex = getglobal( parentFrame:GetName() .. "EnterHighLightTex" );
	highLightTex:Show();
end

function FriendShipFrame_CharacterNameBtn_OnLeave()
	local parentFrame	= getglobal( this:GetParent() );	
	-- TODO: 重构为自动出现高亮条而无需手动设置
	local highLightTex = getglobal( parentFrame:GetName() .. "EnterHighLightTex" );
	highLightTex:Hide();
end

function FriendShipFrame_CharacterNameBtn_OnDoubleClick()
	if g_nDisplayPageType == CLIENT_RELATION_TYPE_FRIEND then
		return;
	end
	local rich = getglobal( this:GetParent() .. "CharacterName" );
	if rich:GetClientString() == "" then return end
	-- 若所点击的按钮中不存储任何玩家信息
	if "" == rich:GetClientString( ) then
		return;
	end

	g_CurClickRole = { roleLowId = rich:GetClientUserData( 0 ), roleHeightId = rich:GetClientUserData( 1 ), roleName = rich:GetClientString() };
	ResetHighLightTex();

	if arg1 ~= "LDoubleClick" then
		return;
	end

	-- 若好友不在我新手消息队列里切不在线给个提示，但是不弹出对话框
	local roleInfo = GameFriendManager:getFriendInfoByName( g_CurClickRole.roleName );
	if not ChatFrameManager:IsNewSpeaker( g_CurClickRole.roleName ) and roleInfo.m_nIsOnline == CLIENT_OFFLINE then
		ShowMidTips( "该好友目前不在线,不能发起新的对话" );
		return;
	end

	-- 若点击黑名单中的人物,不弹出对话框
	if g_nDisplayPageType == CLIENT_RELATION_TYPE_BLACK then
		ShowMidTips( "不能与黑名单中的玩家对话" );
		return;
	end

	local szFrameName = "";

	-- 先检查是否有与被点击玩家对应的且正在显示聊天窗口,若有则什么也做就结束子程序
	if ChatFrameManager:GetSpeakerFrame( g_CurClickRole.roleName ) ~= nil then
		return;
	end
	
	-- 若没有与被点击玩家对应的聊天窗口或聊天提示框
	ChatFrameManager:OpenChatFrame( g_CurClickRole.roleName );
end

--@Desc:此为将鼠标靠近玩家挂单列表的名字的时候弹出玩家信息TIPS
function TrustOrderListFrame_CharacterName_OnEnter()
	local tipsFrame		= getglobal( "FriendShipTipsFrame");
	local RoleNameFont	= getglobal(this:GetName().."RoleName")
	local character		= GameFriendManager:getFriendInfoByName( RoleNameFont:GetText() );
	if nil ~= RoleNameFont:GetText( ) and "" ~= RoleNameFont:GetText( ) then--若控件没有任何玩家信息,则不弹出Tips
		util.AdjustTooltipsScreenPos( { tips = "FriendShipTipsFrame" } )
		tipsFrame:SetClientString( RoleNameFont:GetText() );
		tipsFrame:Show();
	end
end

--@Desc:此为点击向上滚动按钮的时候响应
function Fsf_ScrollUpBtn_OnClick()
	--让高亮条固定

	local slider = getglobal( "FriendShipFrame_ScrollBar" );
	if not slider:IsShown() then
		return;
	end

	local nValue = slider:GetValue() - slider:GetValueStep();
	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end

--@Desc:此为点击向下滚动按钮的时候响应
function Fsf_ScrollDownBtn_OnClick()
	--让高亮条固定

	local slider	= getglobal( "FriendShipFrame_ScrollBar" );
	if not slider:IsShown() then
		return;
	end

	local nMaxValue = slider:GetMaxValue();
	local nValue	= slider:GetValue() + slider:GetValueStep();

	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

--@Desc:此为滚动条滚动的时候响应
function Fsf_ScrollBar_OnValueChanged()
	local slider	 = getglobal( "FriendShipFrame_ScrollBar" );
	local nValue	 = slider:GetValue();
	local nMaxValue	 = slider:GetMaxValue();
	
	if g_nDisplayPageType == CLIENT_RELATION_TYPE_FRIEND then
		if nValue <= nMaxValue then
			UpdateFriendNameList( nValue + 1 );
		end
		ResetFriendHighLightTex();
	elseif g_nDisplayPageType == CLIENT_RELATION_TYPE_PRENTICE then
		if nValue <= nMaxValue then
			UpdateSchoolNameList( nValue + 1 );
		end
		ResetHighLightTex();
	else
		if nValue <= nMaxValue then
			UpdateRoleNameList( nValue + 1 );
		end
		--固定高亮条
		ResetHighLightTex();
	end
end

--@Desc:此为点击关闭按钮的时候响应
function Close_FriendShipFrame()
	--首先使所有按钮恢复正常状态
	local button;
	for i = 1, table.getn( g_friendFrameButtons.buttonName ) do
		button = getglobal( g_friendFrameButtons.buttonName[i] );
		if not ( button:IsEnable( ) ) then
			button:Enable();
			button:DisHighlight();
        end
	end

	g_CurClickRole = { roleLowId = 0, roleHeightId = 0, roleName = "" };
	HideAllHighLightTex();

	local frame = getglobal( "FriendShipFrame" );
	frame:Hide();
end

function FriendShipFrameSelfMeiLiBtn_OnEnter()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then return end
	local szInfo = string.format( "本周魅力：%d\n总魅力值：%d", mainplayer:getWeekCharm(), mainplayer:getCharm() )
	SetGameTooltips( this:GetName(), szInfo )
end

function FriendShipFrame_EmotionShowBtn_OnClick()
	local showbtn	= getglobal( "FriendShipFrame_EmotionShowBtn" );
	local edit		= getglobal( "FriendShipFrame_EmotionEdit" );

	showbtn:Hide();
	edit:SetText( GameFriendManager:getEmotion() );
	edit:Show();
	SetFocusFrame( edit:GetName() );
	edit:SelectAllText();
end

function FriendShipFrame_EmotionEdit_OnFocusLost()
	local showbtn			= getglobal( "FriendShipFrame_EmotionShowBtn" );
	local showFont			= getglobal( "FriendShipFrame_EmotionShowFont" );

	showbtn:Show();
	this:Hide();
	showFont:SetText( GameFriendManager:getEmotion() );
	FriendShipFrame_ModifyEmotion();
end

-------------------------------------------------------
--		好友TIPS		--
-------------------------------------------------------
--@Desc:此为弹出好友TIPS时候的显示方法
function FriendShipTipsFrame_OnShow()
	--设置玩家的相关信息
	local szCharacterName	= FriendShipTipsFrame:GetClientString( );
	local character			= GameFriendManager:getFriendInfoByName( g_nDisplayPageType, szCharacterName );

	local tipsContent = {
						["FriendFrame_CharacterName"]		= { [true] = character.m_szName,								[false] = character.m_szName },
						--["FriendFrame_CharacterMemName"]	= { [true] = "备注姓名：" .. character.m_szMemName,				[false] = "备注姓名：" },
						["FriendFrame_CharacterMemName"]	= { [true] = character.m_szMemName,				[false] = "" },
						["FriendFrame_CharacterLevel"]		= { [true] = "等级：" .. character.m_nLevel,					[false] = "等级：" },
						["FriendFrame_CharacterRace"]		= { [true] = "职业：" .. ( t_raceList[character.m_nCareer] or ""),	[false] = "职业：" },
						["FriendFrame_ClanName"]			= { [true] = "公会：" .. character.m_szClanName,				[false] = "公会：" },
						["FriendFrame_Friendliness"]		= { [true] = "友好度：" .. character.m_nBuddyVal,				[false] = "友好度：" },
						};
	
	for name, content in pairs( tipsContent ) do
		local font = getglobal( name );
		--font:SetText( content[g_nDisplayPageType == CLIENT_RELATION_TYPE_FRIEND] );
		font:SetText( content[true] );
	end
	
	FriendFrame_EmotionSign:SetText( "心情签名：" .. character.m_szEmotion, 174, 221, 159 );

	 --显示玩家头像逻辑
	SetPhoto( character.m_nHair, character.m_nFace, "CharacterHairPhoto", "CharacterPhoto", character.m_nGender, character.m_nHead,
				"CharacterHairShadowTex", "CharacterBodyTex" );
end

local t_DiDuiPlayerTipsFrameEvents = { ["GE_FRIEND_GET_REQUIRE_PLAYER_INFO"] = {},  };

t_DiDuiPlayerTipsFrameEvents["GE_FRIEND_GET_REQUIRE_PLAYER_INFO"].func = 
function ()
	if g_nDisplayPageType == CLIENT_RELATION_TYPE_FRIEND then 
		return;
	end
	local frame				= getglobal( "DiDuiPlayerTipsFrame" );
	if not frame:IsShown() then
		frame:Show();
	end

	local requirePlayer	= GameFriendManager:getCurRequireRoleInfo();

	local tipsContent = {
						["DiDuiPlayerTipsFrame_CharacterName"]		= requirePlayer.RoleName,
						["DiDuiPlayerTipsFrame_CharacterLevel"]		= "等级：" .. requirePlayer.Level,
						["DiDuiPlayerTipsFrame_CharacterRace"]		= "职业：" .. ( t_raceList[requirePlayer.Career] or ""),
						["DiDuiPlayerTipsFrame_ClanName"]			= "公会：" .. requirePlayer.ClanName,
						};
	
	for name, content in pairs( tipsContent ) do
		local font = getglobal( name );
		font:SetText( content );
	end
	
	SetPhoto( requirePlayer.Hair, requirePlayer.Face, "DiDuiPlayerTipsFrame_CharacterHairPhoto", "DiDuiPlayerTipsFrame_CharacterPhoto", 
				requirePlayer.Gender, requirePlayer.Head,
				"DiDuiPlayerTipsFrame_CharacterHairShadowTex", "DiDuiPlayerTipsFrame_CharacterBodyTex" );
end

function DiDuiPlayerTipsFrame_OnLoad()
	for event, _ in pairs( t_DiDuiPlayerTipsFrameEvents ) do
		this:RegisterEvent( event );
	end

	t_DiDuiPlayerTipsFrameEvents.__index = function ( tab, key )
										tab[key] = { func = function () end };
										return tab[key];
									end
	setmetatable( t_DiDuiPlayerTipsFrameEvents, t_DiDuiPlayerTipsFrameEvents );
end

function DiDuiPlayerTipsFrame_OnEvent()
	t_DiDuiPlayerTipsFrameEvents[arg1].func();
end

function DiDuiPlayerTipsFrame_OnShow()
	local frame				= getglobal( "DiDuiPlayerTipsFrame" );
	local szCharacterName	= frame:GetClientString( );

	local character			= GameFriendManager:getFriendInfoByName( g_nDisplayPageType, szCharacterName );

	local tipsContent = {
						["DiDuiPlayerTipsFrame_CharacterName"]		= character.m_szName,
						["DiDuiPlayerTipsFrame_CharacterLevel"]		= "等级：" .. character.m_nLevel,
						["DiDuiPlayerTipsFrame_CharacterRace"]		= "职业：" .. ( t_raceList[character.m_nCareer] or ""),
						["DiDuiPlayerTipsFrame_ClanName"]			= "公会：" .. character.m_szClanName,
						};
	
	for name, content in pairs( tipsContent ) do
		local font = getglobal( name );
		font:SetText( content );
	end
	
	SetPhoto( character.m_nHair, character.m_nFace, "DiDuiPlayerTipsFrame_CharacterHairPhoto", "DiDuiPlayerTipsFrame_CharacterPhoto", 
				character.m_nGender, character.m_nHead,
				"DiDuiPlayerTipsFrame_CharacterHairShadowTex", "DiDuiPlayerTipsFrame_CharacterBodyTex" );
end

----------------------------------------------------------
--			申请添加好友框		--
----------------------------------------------------------
--@Desc:此为用于用于显示别人请求添加我为好友添加申请框
function RequestAddFriendFrame_OnShow()
	t_requestAddFriend[1].startTime = GameMgr:getTickTime();
end

function RequestAddFriendFrame_OnHide()
	-- 首先从申请列表删除该玩家信息
	table.remove( t_requestAddFriend, 1 );
	g_szReqCharacterName = "";
	if table.getn( t_requestAddFriend ) > 0  then
		g_szReqCharacterName = t_requestAddFriend[1].name;
		
		local szAddFriendDesc = "\n#P"..g_szReqCharacterName .. "  申请加你为好友！";
		if GameFriendManager:isPlayerRelation( CLIENT_RELATION_TYPE_BLACK, g_szReqCharacterName ) then
			szAddFriendDesc = g_szReqCharacterName .. "  申请加你为好友，但对方在你的黑名单中，是否同意？";
		elseif GameFriendManager:isPlayerRelation( CLIENT_RELATION_TYPE_SU_DI, g_szReqCharacterName ) then
			szAddFriendDesc = g_szReqCharacterName .. "  申请加你为好友，但对方在你的宿敌名单中，是否同意？";
		end
		
		local t_riches = 
		{ ["RequestAddFriendFrame_DescRich"] = 
			{ 
				value = szAddFriendDesc, 
				color = { r = 255, g = 229, b = 189 } 
			}, 
		};
		util.SetRiches( t_riches );

		RequestAddFriendFrame:SetClientString( g_szReqCharacterName );
		RequestAddFriendFrame:Show();
	end
end

--@Desc:此为点击接受按钮时候响应
function FriendShipFrame_Accept_OnClick()
	local  szCharacterName = g_szReqCharacterName;
	local frame	= getglobal( "RequestAddFriendFrame" );

	-- 若为同意加对方为好友
	--if RequestCharacterFrameChkBtn:GetCheckState() then
	GameFriendManager:addFriendRes( szCharacterName, 1, 0 );
		-- 若申请加我为好友的人在我临时列表,当加其为好友后,从临时列表中删除该好友
	GameFriendManager:deleteRole( CLIENT_RELATION_TYPE_TMPFRIEND, szCharacterName );
	--else
	-- 若为不同意加对方为好友,但是同意对方加我
	--	GameFriendManager:addFriendRes( szCharacterName, 1, 0 );
	--end
	RequestAddFriendFrame_DescRich:Clear();
	frame:Hide();
end

--@Desc:此为点击拒绝按钮时候响应
function FriendShipFrame_Refuse_OnClick()
	local frame = getglobal( "RequestAddFriendFrame" );

	GameFriendManager:addFriendRes( frame:GetClientString(), 0, 0 );

	frame:SetClientString("");
	RequestAddFriendFrame_DescRich:Clear();
	frame:Hide();
end

-------------------------------------------------------
--		添加角色名称		--
-------------------------------------------------------

--@Desc:此为显示添加角色名称时候响应
function FriendAddCharacterFrame_OnShow()
	SetFocusFrame("FriendAddCharacterFrame_InputEdit");
	FriendAddCharacterFrame_InputEdit:Clear();
	FriendAddCharacterFrame_InputEdit:SetTextColor( 255, 229, 189 );
end

--@Desc:此为点击接受按钮的时候响应
function FriendAddCharacterFrame_Accept_OnClick()
	local szNewCharacterName = FriendAddCharacterFrame_InputEdit:GetText();
	if "" == szNewCharacterName then return end--若什么名字也没输，则什么也不做

	if CLIENT_RELATION_TYPE_FRIEND == g_nAddType then--若点击的是添加好友
		if not isRoleInRequestList( szNewCharacterName ) then
			GameFriendManager:addFriend( szNewCharacterName, "" );
		end
		
	else--若点击是添加黑名单
		GameFriendManager:addBlack( szNewCharacterName );
	end

	local frame = getglobal( "FriendAddCharacterFrame" );
	frame:Hide();
	FriendAddCharacterFrame_InputEdit:Clear();
	ClearFocus();
end

--@Desc:此为点击取消按钮的时候响应
function FriendAddCharacterFrame_Cancel_OnClick()
	local frame = getglobal( "FriendAddCharacterFrame" );
	frame:Hide();
	FriendAddCharacterFrame_InputEdit:Clear();
end

function FriendAddCharacterFrame_EditBackBtn_OnClick()
	SetFocusFrame( "FriendAddCharacterFrame_InputEdit" );
end

-------------------------------------------------------
--		添加备注名称框		--
-------------------------------------------------------

--@Desc:此为显示添加备注名称框时候响应
function FriendMemFrame_OnShow()
end

--@Desc:此为接受新备注名称时候响应
function FriendMemFrame_Accept_OnClick()
	local szNewMemName	= FriendMemFrame_InputEdit:GetText();
	local szCharacterName	= FriendMemFrame:GetClientString();

	FriendMemFrame:Hide();
	FriendMemFrame_InputEdit:Clear();
	--GameFriendManager:changeMemName( g_CurClickRole.roleName, szNewMemName );
	GameFriendManager:changeMemName( FriendInfoFrame:GetClientString(), szNewMemName );
	ClearFocus();
	if FriendInfoFrame:IsShown() then
		local rich = getglobal( "FriendInfoFrameBkupNameBtnFont" );
		rich:resizeRect( rich:GetTextExtentWidth( szNewMemName ), 16 );
		rich:SetText( szNewMemName, 160, 160, 90 );		
		rich:SetClientUserData( 0, 1 );
		rich:ScrollFirst();
		rich:SetWidth( 100 );
	end
end

--@Desc:此为不接受新备注名称时候响应
function FriendMemFrame_Cancel_OnClick()
	FriendMemFrame:Hide();
	FriendMemFrame_InputEdit:Clear();
end

function FriendMemFrame_EditBackBtn_OnClick()
	SetFocusFrame( "FriendMemFrame_InputEdit" );
end

-------------------------------------------------------
--		聊天面板		--
-------------------------------------------------------
--------------------ChatFrame-----------------------------

local t_FriendChatFrameEvents = 
{ 
	["GE_FRIEND_CHGNAME"] = {},
};

t_FriendChatFrameEvents["GE_FRIEND_CHGNAME"].func = 
function ( data )
	local Msg				= UIMSG:getUIMsg();
	local changeNameData	= Msg.ChangeNameData;
	
	local szFrameName	= data["name"];
	local szSpeakerName	= ChatFrameManager:GetSpeakerName( szFrameName );
	if szSpeakerName ~= changeNameData.szOldName then
		return;
	end
	
	ChatFrameManager:SetSpeakerName( szFrameName, changeNameData.szNewName );
	local font	= getglobal( szFrameName .. "_ChatDestName" );
	font:SetText( "与	" .. changeNameData.szNewName .. "对话中");
end

function FriendChatFrame_OnLoad()
	for event, _ in pairs( t_FriendChatFrameEvents ) do
		this:RegisterEvent( event );
	end

	t_FriendChatFrameEvents.__index = function ( tab, key )
									tab[key] = { func = function () end };
									return tab[key];
								end
	setmetatable( t_FriendChatFrameEvents, t_FriendChatFrameEvents );
end

function FriendChatFrame_OnEvent()
	t_FriendChatFrameEvents[arg1].func( { ["name"] = this:GetName() } );
end

--@Desc:此为显示聊天面板的时候响应
function FriendChatFrame_OnShow()
	-- 清空原来的聊天信息
	local szFrameName		= this:GetName();
	local szSpeakerName		= ChatFrameManager:GetSpeakerName( this:GetName() );
	local chatDisplayRich	= getglobal( szFrameName .. "_DisplayRich" );
	local chatInputEdit		= getglobal( szFrameName .. "_InputEdit" );

	-- 在显示的时候, 确保输入框没有任何输入文字
	chatInputEdit:Clear();
	chatDisplayRich:Clear();

	-- 对于聊天记录显示区则根据消息队列中的记录来决定是否要显示内容
	local chatContent = ChatFrameManager:GetChatContent( szSpeakerName );
	local num = 0;
	for i =1, table.getn( chatContent ) do
		chatDisplayRich:AddText( chatContent[i], 255, 229, 189 );
		num = num + 1;
	end

	-- 设置滚动条及聊天记录显示区的内容
	chatDisplayRich:ScrollEnd();
	local slider = getglobal( szFrameName .. "_ScrollBar" );
	SetScrollBar( szFrameName.."_ScrollBar", chatDisplayRich:GetAccurateViewLines(), chatDisplayRich:GetTextLines() );
	slider:SetValue( slider:GetMaxValue() );
	util.UpdateRich( szFrameName .. "_DisplayRich", slider:GetValue() );

	-- 将焦点聚在输入框上
	SetFocusFrame( this:GetName().."_InputEdit" );
end

function FriendChatFrame_OnHide()
	--[[
	local chatFaceFrame	= getglobal( this:GetName().."_FaceFrame" );
	chatFaceFrame:Hide();
	--]]
	
	ValidateLinkText( this:GetName() );
end

local t_friendChatLinkText = {	["FriendChatFrame1"] = { text = ""}, ["FriendChatFrame2"] = { text = ""}, ["FriendChatFrame3"] = { text = ""}, 
								["FriendChatFrame4"] = { text = ""}, ["FriendChatFrame5"] = { text = ""}, ["FriendChatFrame6"] = { text = ""},
							};

function ParserFriendChatHistoryText( data ) 
	local szFrameName	= data["frame"];
	local edit			= szFrameName.."_InputEdit"
	local szLinkText	= data["linkText"];
	local nLength		= string.len( szLinkText );
	local nFirstPos		= string.find( szLinkText, "#L", 1, nLength );  
	local nStartPos		= string.find( szLinkText, "@@", 1, nLength ); 
	local nEndPos		= string.find( szLinkText, "#n", 1, nLength );

	if nFirstPos ~= nil and nStartPos ~= nil and nEndPos ~= nil then
		local nId = string.sub( szLinkText, nStartPos + 2, nEndPos - 1 );  
		local szLastText = string.sub( szLinkText, nEndPos + 2, nLength );

		edit:SetText( string.sub(szLinkText,1,nFirstPos-1) );
		--超链接类型
		local linkId		= tonumber(nId)%10;
		--超链接Count
		local linkCountId	= (tonumber(nId) - linkId)/10;

		ChatManager:deleteLink( linkId );
		for index, oneLinkData in ipairs( t_SetLink ) do
			if oneLinkData["linkType"] == linkId then
				table.remove( t_SetLink, index );
				break;
			end
		end

		if linkId == ITEM_LINK or linkId == ARM_LINK then
			ChatManager:setLinkedItemLink(linkId,linkCountId);		
		elseif linkId == MISSION_LINK then 
			ChatManager:setMissionLink( ChatManager:getMissionLink( linkCountId ));
		elseif linkId == POSTION_LINK then 
			ChatManager:setPositionLink( ChatManager:getPostionLink( linkCountId ));
		elseif linkId == SECONDARY_SKILL_LINK then
			ChatManager:setSecondarySkillLink( ChatManager:getSecondarySkillLink( linkCountId ) );
		elseif linkId == PET_LINK then
			ChatManager:setPetLink( ChatManager:getPetLink( linkCountId ) );
		end

		t_friendChatLinkText[szFrameName]["text"] = ( edit:GetText() .. szLastText );
		-- 递归查找下一个链接
		ParserFriendChatHistoryText( { ["frame"] = szFrameName, ["linkText"] = t_friendChatLinkText[szFrameName]["text"], } );
	end
end

local t_FriendInputEditOnCharEvent = { [VK_BACK] = {}, [VK_DELETE] = {}, };

t_FriendInputEditOnCharEvent[VK_BACK].func = 
function ( data )
	local szText		= data["text"];
	local nCursorPos	= data["arg2"] - 1;
	local nCurSelBegin	= data["arg3"] - 1;
	
	if nCursorPos == nCurSelBegin then
		Private_FriendInputEdit_SamePosBack( { ["text"] = szText, ["selBegin"] = nCursorPos, ["edit"] = data["edit"] } );
	else
		local nSelBegin	= 0;
		local nSelEnd	= 0;
		nSelBegin	= math.min( nCursorPos, nCurSelBegin );
		nSelEnd		= math.max( nCursorPos, nCurSelBegin );
		Private_FriendInputEdit_DiffPosBack( {	["text"]	= szText,		["selBegin"]	= nSelBegin, 
												["selEnd"]	= nSelEnd,		["edit"]		= data["edit"] } );
	end
end

function Private_FriendInputEdit_SamePosBack( data )
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
	
	nCurPos = edit:getTextIndexFromRichCharIndex( nCurPos );
	nCurPos = nCurPos + 1;

	local t_delLinkType = {};
	for i = 1, ChatManager:getNumCacheDisplayText() do
		local oneCacheDisplayTextInfo		= ChatManager:getCacheDisplayText( i - 1 );
		local nLinkBeginPos, nLinkEndPos	= string.find( szText, oneCacheDisplayTextInfo.m_strLinkName, 1, true );

		if ( nLinkBeginPos ~= nil and nLinkEndPos ~= nil ) and ( nCurPos >= nLinkBeginPos and nCurPos <= nLinkEndPos ) then
			local szNewText	= ChatManager:strReplace( szText, oneCacheDisplayTextInfo.m_strLinkName, "" );
			table.insert( t_delLinkType, oneCacheDisplayTextInfo.m_nLinkType );
			szNewText		= szNewText..".";
			edit:SetText( szNewText );
			--Private_FriendInputEdit_RoundCursorPos( data );
			break;
		end
	end

	for index, deleteLinkType in ipairs( t_delLinkType ) do
		ChatManager:deleteLink( deleteLinkType );
		RemoveOneLinkCache( { ["linkType"] = deleteLinkType } );
	end
end

function Private_FriendInputEdit_RoundCursorPos( data )
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

function Private_FriendInputEdit_DiffPosBack( data )
	local edit		= getglobal( data["edit"] );
	local szText	= data["text"];
	local nSelBegin	= data["selBegin"];
	local nSelEnd	= data["selEnd"];

	nSelBegin		= edit:getTextIndexFromRichCharIndex( nSelBegin );
	nSelEnd			= edit:getTextIndexFromRichCharIndex( nSelEnd );

	local t_delLinkType = {};
	for i = 1, ChatManager:getNumCacheDisplayText() do
		local oneCacheDisplayTextInfo		= ChatManager:getCacheDisplayText( i - 1 );
		local nLinkBeginPos, nLinkEndPos	= string.find( szText, oneCacheDisplayTextInfo.m_strLinkName, 1, true );

		if ( nLinkBeginPos ~= nil and nLinkEndPos ~= nil ) and ( ( nSelBegin < nLinkBeginPos and nSelEnd >= nLinkBeginPos ) or 
																	nSelBegin >= nLinkBeginPos and nSelBegin <= nLinkEndPos ) then
			table.insert( t_delLinkType, { ["linkType"] = oneCacheDisplayTextInfo.m_nLinkType, ["text"] = oneCacheDisplayTextInfo.m_strLinkName } );
			
			local szNewText	= ChatManager:strReplace( szText, oneCacheDisplayTextInfo.m_strLinkName, "" );
			szNewText		= szNewText..".";
			edit:SetText( szNewText );
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

function Private_FriendInputEdit_AdjustEditBackPos( data )
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

t_FriendInputEditOnCharEvent[VK_DELETE].func = 
function ( data )
	local szText		= data["text"];
	local nCursorPos	= data["arg2"] - 1;
	local nCurSelBegin	= data["arg3"] - 1;
	
	if nCursorPos == nCurSelBegin then
		Private_FriendInputEdit_SamePosDelete( { ["text"] = szText, ["selBegin"] = nCursorPos, ["edit"] = data["edit"] } );
	else
		local nSelBegin	= 0;
		local nSelEnd	= 0;
		nSelBegin	= math.min( nCursorPos, nCurSelBegin );
		nSelEnd		= math.max( nCursorPos, nCurSelBegin );
		Private_FriendInputEdit_DiffPosDelete( { ["text"] = szText, ["selBegin"] = nSelBegin, ["selEnd"] = nSelEnd, ["edit"] = data["edit"] } );
	end
end

function Private_FriendInputEdit_SamePosDelete( data )
	local szText	= data["text"];
	local nCurPos	= data["selBegin"];
	local nTextLen	= string.len( szText );
	if nCurPos == nTextLen then
		return;
	end
	
	local edit		= getglobal( data["edit"] );
	nCurPos = edit:getTextIndexFromRichCharIndex( nCurPos );
	nCurPos = nCurPos + 1;

	local t_delLinkType = {};
	for i = 1, ChatManager:getNumCacheDisplayText() do
		local oneCacheDisplayTextInfo		= ChatManager:getCacheDisplayText( i - 1 );
		local nLinkBeginPos, nLinkEndPos	= string.find( szText, oneCacheDisplayTextInfo.m_strLinkName, 1, true );
		
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

function Private_FriendInputEdit_DiffPosDelete( data )
	local edit		= getglobal( data["edit"] );
	local szText	= data["text"];
	local nSelBegin	= data["selBegin"];
	local nSelEnd	= data["selEnd"];

	nSelBegin = edit:getTextIndexFromRichCharIndex( nSelBegin );

	nSelEnd = edit:getTextIndexFromRichCharIndex( nSelEnd );
	
	local t_delLinkType = {};
	for i = 1, ChatManager:getNumCacheDisplayText() do
		local oneCacheDisplayTextInfo		= ChatManager:getCacheDisplayText( i - 1 );
		local nLinkBeginPos, nLinkEndPos	= string.find( szText, oneCacheDisplayTextInfo.m_strLinkName, 1, true );

		if ( nLinkBeginPos ~= nil and nLinkEndPos ~= nil ) and ( ( nSelBegin < nLinkBeginPos and nSelEnd >= nLinkBeginPos ) or 
																	nSelBegin >= nLinkBeginPos and nSelBegin <= nLinkEndPos ) then
			table.insert( t_delLinkType, { ["linkType"] = oneCacheDisplayTextInfo.m_nLinkType, ["text"] = oneCacheDisplayTextInfo.m_strLinkName } );
			
			local szNewText	= ChatManager:strReplace( szText, oneCacheDisplayTextInfo.m_strLinkName, "" );
			edit:SetText( szNewText );
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
end

function Private_FriendInputEdit_AdjustEditDeletePos( data )
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

function FriendChatFrame_InputEdit_OnChar()
	local szText		= this:GetText();
	local onCharEvent	= t_FriendInputEditOnCharEvent[arg1];

	if onCharEvent ~= nil then
		onCharEvent.func( { ["text"] = szText, ["arg2"] = arg2, ["arg3"] = arg3, ["arg4"] = arg4, ["edit"] = this:GetName() } );
	end
end

function FriendChatFrame_InputEdit_OnQueryChar()
	local szText		= this:GetText();
	local nCursorPos	= arg1;
	local nCurSelBegin	= arg2;
	
	if nCursorPos == nCurSelBegin then
		return Private_FriendInputEdit_SamePosOnQueryChar( { ["text"] = szText, ["selBegin"] = nCursorPos, ["edit"] = this:GetName() } );
	else
		local nSelBegin	= 0;
		local nSelEnd	= 0;
		nSelBegin	= math.min( nCursorPos, nCurSelBegin );
		nSelEnd		= math.max( nCursorPos, nCurSelBegin );
		return Private_FriendInputEdit_DiffPosOnQueryChar( { ["text"] = szText, ["selBegin"] = nSelBegin, ["selEnd"] = nSelEnd,
																["edit"] = this:GetName() } );
	end

	return true;
end

function Private_FriendInputEdit_SamePosOnQueryChar( data )
	local edit		= getglobal( data["edit"] );
	local szText	= data["text"];
	local nCurPos	= data["selBegin"];
	nCurPos			= edit:getTextIndexFromRichCharIndex( nCurPos );

	local nTextLen	= string.len( szText );
	if nCurPos == nTextLen then
		return true;
	end

	nCurPos = nCurPos + 1;

	for i = 1, ChatManager:getNumCacheDisplayText() do
		local oneCacheDisplayTextInfo		= ChatManager:getCacheDisplayText( i - 1 );
		local nLinkBeginPos, nLinkEndPos	= string.find( szText, oneCacheDisplayTextInfo.m_strLinkName, 1, true );
				
		if ( nLinkBeginPos ~= nil and nLinkEndPos ~= nil ) and ( nCurPos > nLinkBeginPos and nCurPos <= nLinkEndPos ) then
			return false;
		end
	end
	
	return true;
end

function Private_FriendInputEdit_DiffPosOnQueryChar( data )
	local edit		= getglobal( data["edit"] );
	local szText	= data["text"];
	local nSelBegin	= data["selBegin"];
	local nSelEnd	= data["selEnd"];

	nSelBegin			= edit:getTextIndexFromRichCharIndex( nSelBegin );
	nSelEnd				= edit:getTextIndexFromRichCharIndex( nSelEnd );
	
	for i = 1, ChatManager:getNumCacheDisplayText() do
		local oneCacheDisplayTextInfo		= ChatManager:getCacheDisplayText( i - 1 );
		local nLinkBeginPos, nLinkEndPos	= string.find( szText, oneCacheDisplayTextInfo.m_strLinkName, 1, true );

		if ( nLinkBeginPos ~= nil and nLinkEndPos ~= nil ) and ( ( nSelBegin < nLinkBeginPos and nSelEnd >= nLinkBeginPos ) or 
																	nSelBegin >= nLinkBeginPos and nSelBegin <= nLinkEndPos ) then
			return false;
		end


	end
	
	return true;
end

--@Desc:此为点击发送按钮的时候响应
function FriendChatFrame_Send_OnClick()
	local mainplayer	= ActorMgr:getMainPlayer();
	local szMyName		= mainplayer:getName();
	local szParentName	= this:GetParent();
	local inputEdit		= getglobal( szParentName.."_InputEdit" );
	local szChatContent = inputEdit:GetText();
	local szSpeakerName	= ChatFrameManager:GetSpeakerName( szParentName );
	local chatFrame		= getglobal( szParentName );

	-- 不允许发送空消息
	if "" == szChatContent then return end
	szChatContent = string.gsub( szChatContent, "#[L]", "**" );
	
	local t_ClientSelfChatContentParser = GetClientSelfChatContentParser();

	if t_ClientSelfChatContentParser:DoClientDefineInstruction( { ["text"] = szChatContent } ) then
		return;
	end
	szChatContent = t_ClientSelfChatContentParser:parseLinkTextForClient( { ["text"] = szChatContent } );

	-- 将消息发给聊天对象	
	sz2Server = t_ClientSelfChatContentParser:parseLinkTextForServe( { ["text"] = szChatContent } );
	local	friendinfo = GameFriendManager:getFriendInfoByName(szSpeakerName);
	if chatFrame:GetClientUserData( 0 ) == 0 then
		ChatManager:requestChat( CL_CHAT_FRIEND, szSpeakerName, sz2Server, CLIENT_CHAT_NOT_SHOW_IN_CHAT_FRAME, friendinfo.m_nIsOnline );
	elseif chatFrame:GetClientUserData( 0 ) == CL_CHAT_WINDOW then
		local character;
		if GameSchool:GetSelfSchoolJobType() == ROLE_SCHOOL_TYPE_STUDENT_TOLUA then
			character = GameSchool:GetSelfTeacher();
			ChatManager:requestChat( CL_CHAT_WINDOW, szSpeakerName, sz2Server, CLIENT_CHAT_NOT_SHOW_IN_CHAT_FRAME, character.Online );
		elseif GameSchool:GetSelfSchoolJobType() == ROLE_SCHOOL_TYPE_TEACHER_TOLUA then
			character = GameSchool:GetSelfStudent( szSpeakerName )
			ChatManager:requestChat( CL_CHAT_WINDOW, szSpeakerName, sz2Server, CLIENT_CHAT_NOT_SHOW_IN_CHAT_FRAME, character.Online );
		else
			ChatManager:requestChat( CL_CHAT_WINDOW, szSpeakerName, sz2Server, CLIENT_CHAT_NOT_SHOW_IN_CHAT_FRAME, 0 );
		end
	end

	-- 清空输入框中的内容
	inputEdit:Clear();
	ClearSetLink();
end

function FriendChatFrame_Send_OnEnter()
	local szText = "按Ctrl+Enter键发送消息，按Enter键换行";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function FriendChatFrame_DisplayRich_OnClick()
	local szCurSelectChatPlayerName = nil;
	if type(arg1) == "number" then return end --如果是表情符，不做处理
	if arg2 ~= "" then
		if tonumber(arg2) == nil then return end
		--超链接类型
		local linkId = tonumber(arg2)%10;
		--超链接Count
		local linkCountId = (tonumber(arg2) - linkId)/10;
		--VK_LSHIFT 160,VK_RSHIFT 161
		if isKeyPressed(160) or isKeyPressed(161) then
			--如果店铺面板显示中，将物品名字复制到搜索栏
			if OnlineStoreFrame:IsShown() then
				local itemDef;
				if linkId == ITEM_LINK then
					local itemLink = ChatManager:getItemLink(linkCountId);			
					if itemLink.m_nItemId > 0 then
						itemDef = getItemDef( itemLink.m_nItemId )
					end
				elseif linkId == ARM_LINK then
					local armLink = ChatManager:getArmLink(linkCountId);
					if getItemType(armLink.m_arm.DefID) == ITEM_SLOTTYPE_AMM then
						--装备的tips
						local item = ChatManager:getItem(armLink.m_arm);
						if item ~= nil then
							itemDef = item:getItemDef()
						end
					end
				end
				if itemDef ~= nil then
					OnlineStoreFrameSearchEdit:SetText( itemDef.Name )
					OnlineStoreFrame:AddLevelRecursive()
					SetFocusFrame( "OnlineStoreFrameSearchEdit" )
					return
				end
			end
		end

		-- 物品超链接
		if linkId == ITEM_LINK then
			--带<>号物品名称转找为不带<>
			local t_hideIcons = { "LinkItemTipsFrame", "ArmItemTipsFrame", };
			util.HideIcons( t_hideIcons );
			local itemLink = ChatManager:getItemLink(linkCountId);
			local item = ChatManager:getSparItem( itemLink )			
			if itemLink.m_nItemId > 0 then
				if item ~= nil then
					ShowItemTips(LUA_ITEMTIPS_TYPE_LINK,LUA_ITEMTIPS_ITEM,item);
				else
					ShowItemTips(LUA_ITEMTIPS_TYPE_LINK, LUA_ITEMTIPS_ITEMDEF, getItemDef(itemLink.m_nItemId) );
				end				
				return;

			end
		elseif linkId == ARM_LINK then
			local armLink = ChatManager:getArmLink(linkCountId);
			if getItemType(armLink.m_arm.DefID) == ITEM_SLOTTYPE_AMM then
				--装备的tips
				local item = ChatManager:getItem(armLink.m_arm);
				if item ~= nil then
					ShowItemTips(LUA_ITEMTIPS_TYPE_LINK,LUA_ITEMTIPS_ITEM,item);
				end
			end
		--位置坐标链接
		elseif linkId == POSTION_LINK  then
			local postionLink = ChatManager:getPostionLink(linkCountId);
			local player = ActorMgr:getMainPlayer();

			--如果mapId跟当前的mapId不相同时,不让寻路
			if ( player == nil ) then return end

			if postionLink.m_nMapId ~= 0 then
				local nId = postionLink.m_nMapId;
				local nX  = postionLink.m_nX;
				local nY  = postionLink.m_nY;
				MapShower:GetMaxMapContal():linkMoveto(nId, nX, nY );
				return;
			end
		--位置任务链接
		elseif linkId == MISSION_LINK  then
			local missionLink = ChatManager:getMissionLink(linkCountId);
			if missionLink.m_MissionId ~= 0 then
				SetShowQuestLogState(missionLink.m_MissionId);
				SetTaskLogFrameTaskLogInfo( missionLink.m_MissionId, missionLink.m_nRecycleID, false );
				TaskLogDescFrame:SetPoint( "center", "$parent", "center", 0, 0 );
				SetShowQuestLogState(0);
				return;
			end
		elseif linkId == SERVER_LINK then
			local postionLink = ChatManager:getSystemLink(linkCountId);
			local player = ActorMgr:getMainPlayer();

			--如果mapId跟当前的mapId不相同时,不让寻路
			if ( player == nil ) then return end

			if postionLink.m_nMapId ~= 0 then
				local nId = postionLink.m_nMapId;
				local nX  = postionLink.m_nX;
				local nY  = postionLink.m_nY;
				MapShower:GetMaxMapContal():linkMoveto(nId, nX, nY );
				return;
			end
		elseif linkId == SECONDARY_SKILL_LINK then			
			SecondarySkillOpenLink( linkCountId );
		elseif linkId == PET_LINK then
			local petLink = ChatManager:getPetLink( linkCountId );
			if petLink.PetWid.nHeight == 0 and petLink.PetWid.nLow == 0 then
				return;
			end
			RankMgr:requestWatchPet( petLink.PetWid.nLow,petLink.PetWid.nHeight,petLink.OwnerName,string.len(petLink.OwnerName) );
		end
	end

	if arg1 ~= "" and arg1 ~= "LeftButton" and arg1 ~= "RightButton" and arg2 == "" then
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer:getName() == arg1 then
			return;
		end

		if arg3 == "RightButton" then
			szCurSelectChatPlayerName = arg1;
			ShowCharacterRightFrame( szCurSelectChatPlayerName );
		end
	end
end

function FriendChatFrame_Send_OnLeave()
	GameTooltip:Hide();
end

--@Desc:此为点击聊天记录按钮的时候响应
function FriendChatFrame_RecordBtn_OnClick()
	local chatRecordFrame	= getglobal( this:GetParent().."_RecordFrame" );

	if chatRecordFrame:IsShown() then
		chatRecordFrame:Hide();
	else
		chatRecordFrame:Show();
	end
end

function FriendChatFrame_RecordBtn_OnEnter()
	local szText = "点击可查看历史聊天记录，该记录下线自动清空";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function FriendChatFrame_RecordBtn_OnLeave()
	GameTooltip:Hide();
end


--@Desc:点击表情按钮时候响应
function FriendChatFrame_SmileFaceBtn_OnClick()
	local chatFrame		= getglobal( this:GetParent() );
	local chatFaceFrame	= getglobal( "FriendChatFaceFrame" );

	-- 若原来就相对于这个面板，则关闭，
	-- 若原来显示的时候相对于另外一个面板，则先关闭后打开
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

function FriendChatFrame_SmileFaceBtn_OnEnter()
	local szText = "选择表情";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function FriendChatFrame_SmileFaceBtn_OnLeave()
	GameTooltip:Hide();
end


function InputEditGround_OnClick()
	local szEditBox = this:GetParent() .. "_InputEdit";
	SetFocusFrame( szEditBox );
end

--@Desc:此为点击向上滚动按钮的时候响应
function FriendChatFrame_ScrollUpBtn_OnClick()
	local displayRich	= getglobal( this:GetParent().."_DisplayRich" );
	local slider		= getglobal( this:GetParent().."_ScrollBar" );
	local nValue		= slider:GetValue() - slider:GetValueStep();

	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end

--@Desc:此为点击向下滚动按钮的时候响应
function FriendChatFrame_ScrollDownBtn_OnClick()
	local displayRich	= getglobal( this:GetParent().."_DisplayRich" );
	local slider		= getglobal( this:GetParent().."_ScrollBar" );
	local nMaxValue		= slider:GetMaxValue();
	local nValue		= slider:GetValue() + slider:GetValueStep();

	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

--@Desc:此为滚动条滚动的时候响应
function FriendChatFrame_ScrollBar_OnValueChanged()
	local slider	 = getglobal( this:GetParent().."_ScrollBar" );
	local nValue	 = slider:GetValue();
	local nMaxValue	 = slider:GetMaxValue();

	if nValue <= nMaxValue then
		util.UpdateRich( this:GetParent().."_DisplayRich", nValue );
	end
end

--@Desc:此为点击关闭按钮的时候响应
function Close_FriendChatFrame()
	ChatFrameManager:ShutDownChatFrame( this:GetParent() );
	local chatRecordFrame = getglobal( this:GetParent().."_RecordFrame" );
	chatRecordFrame:Hide();

	local rich = getglobal( this:GetParent().."_RecordFrame_RecordDisplayRich" );
	rich:Clear();

	local t_delLinkType = {}
	local edit	= getglobal( this:GetParent().."_InputEdit" );
	local szText= edit:GetText();
	for i = 1, ChatManager:getNumCacheDisplayText() do
		local oneCacheDisplayTextInfo		= ChatManager:getCacheDisplayText( i - 1 );
		local nLinkBeginPos, nLinkEndPos	= string.find( szText, oneCacheDisplayTextInfo.m_strLinkName, 1, true );
		if ( nLinkBeginPos ~= nil and nLinkEndPos ~= nil ) then
			table.insert( t_delLinkType, { ["linkType"] = oneCacheDisplayTextInfo.m_nLinkType, 
						["text"] = oneCacheDisplayTextInfo.m_strLinkName } );
		end
	end

	for index, oneDeleteLinkData in ipairs( t_delLinkType ) do
		ChatManager:deleteLink( oneDeleteLinkData["linkType"] );
		RemoveOneLinkCache( { ["linkType"] = oneDeleteLinkData["linkType"] } );
	end
end

--@Desc:此为点击缩小按钮的时候响应
function Dwindle_FriendChatFrame()
	ChatFrameManager:LessenChatFrame( this:GetParent() );
	if not FriendFrame_ChatClueFrame:IsShown() then
		FriendFrame_ChatClueFrame:Show();
	end
end

---------------------笑脸面板---------------------------
----------------------ChatFaceFrame----------------------------
function FriendChatFaceFrame_OnShow()
	local rich = getglobal( "FriendChatFaceFrame_ShowFaceRich" );
	if rich:GetTextLines() > 0 then
		return;		
	end

	util.CompleteShowAllTextOnRich( {	["rich"] = rich:GetName(), ["text"] = FACE_TEXT, 
										["color"] = { r = 255, g = 255, b = 255 }, } );
end

function FriendChatFaceFrame_OnLoad()
	local rich = getglobal( "FriendChatFaceFrame_ShowFaceRich" );
	rich:SetFaceTexture( [[uires\TuPianLei\LiaoTian\BiaoQingTuBiao\popoface.dds]] );
	rich:SetFaceTexUV( t_biaoQingHightUV["x"], t_biaoQingHightUV["y"], t_biaoQingHightUV["width"], t_biaoQingHightUV["height"] );
	rich:setTwoFaceInterval( 3.5, 1 );
end

function FriendChatFaceFrame_OnUpdate()
	local frame = getglobal( this:GetClientString() );
	if not frame:IsShown() then
		this:Hide();
	end
end

function FCF_ShowFaceRich_OnClick()
	-- 当点击笑脸面板中的任意一个笑脸后,关闭笑脸面板
	local editFrame = getglobal( FriendChatFaceFrame:GetClientString() );

	-- 获取所点击的内容
	local inputEdit = getglobal( editFrame:GetName() .. "_InputEdit");
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
	FriendChatFaceFrame:Hide();
end

function FCF_ShowFaceRich_OnEnter()
	if arg2 ~= "RTOT_FACE_ONENTER" then
		return;
	end

	if arg1 < 1 then 
		return;
	end

	--表情代码
	local nCode = arg1;
	--[[
	local szCode;
	if nCode >= 100 then
		szCode = "#"..tostring (nCode );
	elseif nCode >= 10 then
		szCode = "#0"..tostring( nCode );
	elseif nCode > 0 then
		szCode = "#00"..tostring(nCode);
	end
	--]]
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = GetRototFaceTable()[nCode][2], frame = this:GetParent(),
						button = "cursor" } );
end

-------------------------------------------------------
--		聊天记录		--
-------------------------------------------------------
--------------------ChatRecordFrame-----------------------------

-- @brief: 用于更新聊天记录面板上聊天记录信息
function UpdateChatRecordRich( szRichName, szSliderName )
	local rich		= getglobal( szRichName );
	local slider	= getglobal( szSliderName );
	rich:ScrollFirst();

	for i = 1, slider:GetValue() + MAX_RECORD_LINE_ONE_PAGE * ( rich:GetClientUserData( 1 ) - 1 )  do
		rich:ScrollDown();
	end
end

--@Desc:用于获得页面数显示内容
--@Param:当前聊天记录显示的内容的第一行在记录表中所处的行数
function DisplayPageNum( nFrameNum )
	local recordRich		= getglobal( "FriendChatFrame" .. nFrameNum .. "_RecordFrame_RecordDisplayRich" );
	local nTotalRichLine	= recordRich:GetTextLines();
	local nCurLine			= recordRich:GetClientUserData( 0 );
	local pageDis			= getglobal( "FriendChatFrame".. nFrameNum.."_RecordFrame_PageFont" );
	local nCurPage			= recordRich:GetClientUserData( 1 );
	local nTotalPage		= math.ceil( nTotalRichLine / MAX_RECORD_LINE_ONE_PAGE );
	pageDis:SetText( "第" .. ( nCurPage > 0 and nCurPage or 1 ) .. "/" .. ( nTotalPage > 0 and nTotalPage or 1 ) .. "页" );
end

-- 情况所有聊天记录的内容
function initFriendData()
	for i = 1, MAX_CHAT_FRAME do
		local rich = getglobal( "FriendChatFrame"..i.."_DisplayRich" );
		rich:Clear();
		rich:clearHistory();
		rich = getglobal( "FriendChatFrame"..i.."_RecordFrame_RecordDisplayRich" );
		rich:Clear();
		rich:clearHistory();
		local recordBtn = getglobal( "FriendChatFrame"..i.."_RecordFrame" );
		recordBtn:Hide();
	end

	 ChatFrameManager:initChatManager();
	 IgnoreAllAddFriendRequests();

	 t_friendChatLinkText = {	["FriendChatFrame1"] = { text = ""}, ["FriendChatFrame2"] = { text = ""}, ["FriendChatFrame3"] = { text = ""}, 
								["FriendChatFrame4"] = { text = ""}, ["FriendChatFrame5"] = { text = ""}, ["FriendChatFrame6"] = { text = ""},
							};
end

function IgnoreAllAddFriendRequests()
	for index, data in ipairs( t_requestAddFriend ) do
		GameFriendManager:addFriendRes( data.name, 0, 1 );
	 end
	 t_requestAddFriend = {};
end

local t_recordContent = {
						["FriendChatFrame1_RecordFrame"] = { curPage = 1, totalPage = 1 },
						["FriendChatFrame2_RecordFrame"] = { curPage = 1, totalPage = 1 },
						["FriendChatFrame3_RecordFrame"] = { curPage = 1, totalPage = 1 },
						["FriendChatFrame4_RecordFrame"] = { curPage = 1, totalPage = 1 },
						["FriendChatFrame5_RecordFrame"] = { curPage = 1, totalPage = 1 },
						["FriendChatFrame6_RecordFrame"] = { curPage = 1, totalPage = 1 },
						};

function UpdateRecordContent( szFrameName, nCurPage, totalRecord )
	t_recordContent[szFrameName].curPage	= nCurPage;
	t_recordContent[szFrameName].totalPage	= math.ceil( table.getn( totalRecord ) / MAX_RECORD_LINE_ONE_PAGE );

	local font = getglobal( szFrameName .. "_PageFont" );
	font:SetText( "第" .. t_recordContent[szFrameName].curPage .. "/" .. 
			( t_recordContent[szFrameName].totalPage < t_recordContent[szFrameName].curPage and t_recordContent[szFrameName].curPage
			or t_recordContent[szFrameName].totalPage ) .. "页" );
end

function FriendChatRecordFrame_OnShow()
	-- 清空聊天记录面板原来的内容
	ChangePageContent( 1, this:GetName() );
end

--@Desc:此为点击向上按钮的时候响应
function FriendRecordFrame_ScrollUpBtn_OnClick()
	local slider = getglobal( this:GetParent().."_ScrollBar" );
	if not slider:IsShown() then
		return;
	end

	local nValue = slider:GetValue() - slider:GetValueStep();
	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end

--@Desc:此为点击向下滚动按钮的时候响应
function FriendRecordFrame_ScrollDownBtn_OnClick()
	local slider	= getglobal( this:GetParent().."_ScrollBar" );
	if not slider:IsShown() then
		return;
	end
	local nMaxValue = slider:GetMaxValue();
	local nValue	= slider:GetValue() + slider:GetValueStep();

	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

--@Desc:此为滚动条滚动的时候响应
function FriendRecordFrame_ScrollBar_OnValueChanged()
	local nValue	 = this:GetValue();
	local nMaxValue	 = this:GetMaxValue();

	if nValue <= nMaxValue then
		util.UpdateRich( this:GetParent().."_RecordDisplayRich", nValue )
	end
end

function ChangePageContent( nCurPage, szRecordFrameName )
	local recordtFrame	= getglobal( szRecordFrameName );
	local chatFrame		= getglobal( recordtFrame:GetParent() );
	local recordRich	= getglobal( szRecordFrameName .. "_RecordDisplayRich" );
	-- 对于聊天记录显示区则根据消息队列中的记录来决定是否要显示内容
	local szSpeakerName	= ChatFrameManager:GetSpeakerName( chatFrame:GetName() );
	local chatHistory	= ChatFrameManager:GetHistroy( szSpeakerName );
	UpdateRecordContent( szRecordFrameName, nCurPage, chatHistory );
	recordRich:Clear();
	local nPassTotalLines = ( t_recordContent[szRecordFrameName].curPage - 1 ) * MAX_RECORD_LINE_ONE_PAGE;
	local nCurRichLine = 1;
	for i = nPassTotalLines + 1, table.getn( chatHistory ) do
		if nCurRichLine > MAX_RECORD_LINE_ONE_PAGE then
			break;
		end
		recordRich:AddText( chatHistory[i], 254, 242, 216 );
		nCurRichLine = nCurRichLine + 1;
	end

	local slider = getglobal( recordtFrame:GetName() .. "_ScrollBar" );
	slider:SetValue( 0 );
	SetScrollBar( slider:GetName(), recordRich:GetAccurateViewLines(), recordRich:GetTextLines() );		
	util.UpdateRich( recordRich:GetName(), slider:GetValue() );
end

--@Desc:此为点击最左箭头时候响应
function FriendChatRecordFrame_Leftest_OnClick()
	local recordtFrame	= getglobal( this:GetParent() );
	local chatFrame		= getglobal( recordtFrame:GetParent() );
	local slider		= getglobal( this:GetParent() .. "_ScrollBar" );
	local recordRich	= getglobal( this:GetParent() .. "_RecordDisplayRich" );
	local nCurPage		= t_recordContent[this:GetParent()].curPage;
	local nTotalPage	= t_recordContent[this:GetParent()].totalPage;

	if nCurPage <= 1 then
		return;
	end
	ChangePageContent( 1, recordtFrame:GetName() );
end

--@Desc:此为点击向左的箭头的时候响应
function FriendChatRecordFrame_Left_OnClick()
	local recordtFrame	= getglobal( this:GetParent() );
	local chatFrame		= getglobal( recordtFrame:GetParent() );
	local slider		= getglobal( this:GetParent() .. "_ScrollBar" );
	local recordRich	= getglobal( this:GetParent() .. "_RecordDisplayRich" );
	local nCurPage		= t_recordContent[this:GetParent()].curPage;
	local nTotalPage	= t_recordContent[this:GetParent()].totalPage;

	if nCurPage <= 1 then
		return;
	end
	
	ChangePageContent( nCurPage - 1, recordtFrame:GetName() );
end

--@Desc:此为点击最右箭头时候响应
function FriendChatRecordFrame_Rightest_OnClick()
	local recordtFrame	= getglobal( this:GetParent() );
	local chatFrame		= getglobal( recordtFrame:GetParent() );
	local slider		= getglobal( this:GetParent() .. "_ScrollBar" );
	local recordRich	= getglobal( this:GetParent() .. "_RecordDisplayRich" );
	local nCurPage		= t_recordContent[this:GetParent()].curPage;
	local nTotalPage	= t_recordContent[this:GetParent()].totalPage;

	if nCurPage >= nTotalPage then
		return;
	end
	
	ChangePageContent( nTotalPage, recordtFrame:GetName() );
end

--@Desc:此为点击向右箭头时候响应
function FriendChatRecordFrame_Right_OnClick()
	local recordtFrame	= getglobal( this:GetParent() );
	local chatFrame		= getglobal( recordtFrame:GetParent() );
	local slider		= getglobal( this:GetParent() .. "_ScrollBar" );
	local recordRich	= getglobal( this:GetParent() .. "_RecordDisplayRich" );
	local nCurPage		= t_recordContent[this:GetParent()].curPage;
	local nTotalPage	= t_recordContent[this:GetParent()].totalPage;

	if nCurPage >= nTotalPage then
		return;
	end
	
	ChangePageContent( nCurPage + 1, recordtFrame:GetName() );
end

-------------右键选项框---------------------
--此为所有右键选项共用的辅助函数
--@Desc:发送私聊
function SendPrivateMessage( szPrivatePlayerName )
	-- 显示聊天框,并设置私聊格式
	local t_ChatInsertFrameControl = GetChatInsertFrameControl();
	t_ChatInsertFrameControl:setPrivatePlayerName( { ["name"] = szPrivatePlayerName } );

	SetChatType(CL_CHAT_PRIVATE);
	UpdateChatInsertFrameEnterFocus();
end

--@Desc:此为用来修改备注名的方法
function ModifyMemName( )
	if FriendMemFrame:IsShown() then
		return;
	end

	FriendMemFrame:Show();
	FriendMemFrame_InputEdit:Clear();
	SetFocusFrame( "FriendMemFrame_InputEdit" );
end

--@Desc: 此为用于向临时列表添加新玩家

-------------------------------------------------------
--		头像右键选项		--
-------------------------------------------------------
function FriendFrame_Character_RighOption_OnLoad()
end

function FriendFrame_RighOption_OnShow()
	SetFocusFrame( this:GetName() );
end

function FriendFrame_RighOption_OnFocusLost()
	this:Hide();
	this:SetClientString( "" );
end

function CanFollow( actor )
	--[[
	if actor == nil then
		ShowMidTips( "所要跟随玩家不在视野范围内" );
		return false;
	end

	if actor:getType() ~= GAT_PLAYER then
		ShowMidTips( "操作失败，当前对象无法跟随" );
		return false;
	end
	
	local relationInfo = GameFriendManager:getFriendInfoByName( actor:getName() );
	
	if relationInfo ~= nil and ( relationInfo.m_nRelationType == CLIENT_RELATION_TYPE_XIONG_SHOU or 
			relationInfo.m_nRelationType == CLIENT_RELATION_TYPE_BLACK ) then

		ShowMidTips( "操作失败，当前对象无法跟随" );
		return false;
	end

	-- 查看玩家是否是敌对阵营
	if actor:isCamp( SKILL_TARGET_ENEMY_PLAYER ) then
		ShowMidTips( "操作失败，当前对象无法跟随" );
		return false;
	end
	--]]
	
	-- 交易状态不能进行跟随
	if TradeFrame:IsShown( ) then
		ShowMidTips( "正在交易，不能进行跟随操作" );
		return false;
	end


	return true;
end

CLIENT_RELATION_TYPE_SELFTEAM	= 257;
CLIENT_RELATION_TYPE_OTHERTEAM	= 258;
-- 新的右键菜单选项
local t_rightOperation =	{
								-- 新人右键选项
								[CLIENT_RELATION_TYPE_NEWCOMER] = { "私聊", "观察玩家", "组队", "加为好友", "跟随", "复制名称", },
								-- 人物头像右键选项
								[CLIENT_RELATION_TYPE_NONE] = { "私聊", "观察玩家", "组队", "交易", "加为好友", "跟随", "加入黑名单", "复制名称", },
								-- 好友右键选项
								[CLIENT_RELATION_TYPE_FRIEND] = { "私聊", "观察玩家", "组队", "交易", "备注姓名", "加入黑名单", "复制名称", "删除",  },
								-- 师傅右键选项
								[CLIENT_RELATION_TYPE_MASTER] = { "私聊", "观察玩家", "组队", "交易", "备注姓名", "复制名称" },
								-- 徒弟右键选项
								[CLIENT_RELATION_TYPE_PRENTICE] = { "私聊", "观察玩家", "组队", "交易", "备注姓名", "复制名称" },
								-- 夫妻右键选项
								[CLIENT_RELATION_TYPE_SPOUSE] = { "私聊", "观察玩家", "组队", "交易", "备注姓名", "复制名称" },
								-- 结拜右键选项
								[CLIENT_RELATION_TYPE_BROTHER] = { "私聊", "观察玩家", "组队", "交易", "备注姓名", "复制名称" },
								-- 凶手右键选项
								[CLIENT_RELATION_TYPE_XIONG_SHOU] = { "私聊", "加入宿敌", "加入黑名单", "复制名称", "删除", },
								-- 追杀右键选项
								[CLIENT_RELATION_TYPE_SU_DI] = { "私聊", "加入黑名单", "复制名称", "删除", },
								-- 黑名单右键选项
								[CLIENT_RELATION_TYPE_BLACK] = { "复制名称", "删除",  },
								-- 临时右键选项
								[CLIENT_RELATION_TYPE_TMPFRIEND] = { "私聊", "观察玩家", "组队", "交易", "加为好友", "加入黑名单", "复制名称", "删除", },
								-- 右键自己队员头像
								[CLIENT_RELATION_TYPE_SELFTEAM] = { "私聊", "观察玩家", "交易", "加为好友", "跟随", "复制名称", },
								-- 右键别人队员头像
								[CLIENT_RELATION_TYPE_OTHERTEAM] = { "私聊", "观察玩家", "交易", "加为好友", "跟随", "复制名称", "组队", 
																	"加入黑名单", },
								["同意入队"] = {}, ["拒绝入队"] = {}, ["清空列表"] = {}, ["刷新列表"] = {},
								["私聊"] = {}, ["观察玩家"] = {},["组队"] = {}, ["交易"] = {},["备注姓名"] = {},
								["删除"] = {}, ["加入黑名单"] = {}, ["复制名称"] = {}, ["加入宿敌"] = {}, ["加为好友"] = {},	["跟随"] = {},
								["切磋"] = {}, ["邀请入会"]	= {},	["邀请骑乘"] = {}, ["踢下骑乘"] = {}, ["转让队长"]	= {},	["请离队伍"]	= {},
								["取消"] = {}, ["宠物融合"] = {},   ["查看资源"] = {}, ["查看公会"] = {}, ["发起对话"]	= {},	["请离投票"]	= {},
								--为宠物保留观察对方
								["观察对方"] = {},
								name = "", relationType = -1, nHeight = 0, nLow = 0, id = 0, frame = "", followType = 0
							};

function GetRightOpertionControl()
	return t_rightOperation;
end

t_rightOperation["发起对话"].func = 
function ( )
	ChatFrameManager:OpenChatFrame( t_rightOperation["name"] );
end

t_rightOperation["取消"].func = 
function ( )
	local frame = getglobal( t_rightOperation["frame"] );
	frame:Hide();
end

t_rightOperation["私聊"].func = function ( )
									SendPrivateMessage( t_rightOperation.name );
								end


t_rightOperation["宠物融合"].func = 
function ()
	local nPetMemID		= t_rightOperation["id"];
	local destPetActor	= ActorMgr:FindActor( nPetMemID );

	local destPetOwner = ActorMgr:FindActor( destPetActor:getOwnerId() );
	local mainplayer	= ActorMgr:getMainPlayer();
	if destPetOwner == nil then
		ShowMidTips( "邀请玩家不在视野内" );
		return;
	end
	
	local dist = util.CalActorsDistance( destPetOwner, mainplayer );
	if dist > CLIENT_MAX_RONG_HE_DIST ^ 2 then
		ShowMidTips( "邀请玩家距离太远" );
		return;
	end

	-- 再检查自己	
	local myFightPet	= GetFightPet( mainplayer );
	if myFightPet == nil then
		ShowMidTips( "请先召唤一只宠物，才能融合" );
		return;
	end
	
	local petPackage	= mainplayer:getPet();
	local petInfo		= petPackage:getPetInfo( myFightPet:getRoleWId() );

	local t_PetRongHeProcessFrameControl = GetPetRongHeProcessFrameControl();
	if t_PetRongHeProcessFrameControl:getRongHePlayerMemID() ~= 0 then
		ShowMidTips( "你正在进行融合，不能再次发出融合邀请" );
		return;
	end
	
	local myPetMgr = mainplayer:getPet()
	if myPetMgr:getPetHeti() ~= -1 then
		ShowMidTips( "宠物合体状态下，不能进行宠物融合" );
		return;
	end
	local itsPetMgr = destPetOwner:getPet()
	if itsPetMgr:getPetHeti() ~= -1 then
		ShowMidTips( "对方处于宠物合体状态，不能进行宠物融合" );
		return;
	end

	-- 检查是否是同类宠物
	if petInfo.PetID ~= destPetActor:getPetDefId() then
		ShowMidTips( "不是同种宠物，不能融合" );
		return;
	end
	
	-- 先检查对方
	-- 检查资质是不是至少是优秀
	if destPetActor:getPetGenius() < CLIENT_PET_GENIUS_B then
		ShowMidTips( "宠物资质至少要优秀，才能融合" );
		return;
	end

	if myFightPet:getPetGenius() < CLIENT_PET_GENIUS_B then
		ShowMidTips( "宠物资质至少要优秀，才能融合" );
		return;
	end
	
	-- 检查双方若有一只宠物是二代宠物，则不能融合
	if petInfo.Hierarchy == 2 then
		ShowMidTips( "二代宠物，不能融合" );
		return;
	end
	
	-- 再看双方宠物的等级是不是至少为50级
	if myFightPet:getLv() < CLIENT_MIN_RONG_HE_LV then
		ShowMidTips( "宠物等级至少要50级，才能融合" );
		return;
	end
	
	if destPetActor:getLv() < CLIENT_MIN_RONG_HE_LV then
		ShowMidTips( "宠物等级至少要50级，才能融合" );
		return;
	end

	local petDef = getPetDef( petInfo.PetID );
	if petDef.BabyPetID == 0 then
		ShowMidTips( "该宠物没下一代，不能融合" );
		return;
	end

	petPackage:inviteFuse( nPetMemID );
end

t_rightOperation["观察玩家"].func = function ( )
									local mainplayer = ActorMgr:getMainPlayer()
									if mainplayer == nil then return end
									local Actor;
									if t_rightOperation["FriendOnline"] ~= nil then
										if t_rightOperation["FriendOnline"] == CLIENT_OFFLINE then
											ShowMidTips("对方不在线")
										else
											GameFriendManager:RequestWatchFriend( t_rightOperation["name"] );
										end
										return;
									end
									if t_rightOperation["watchIgnorDist"] then
										RankMgr:requestWatchPlayer( t_rightOperation["name"] );
										return;
									else
										if t_rightOperation["id"] ~= 0 then
											Actor = ActorMgr:FindActor( t_rightOperation["id"] );
										else
											Actor = ActorMgr:findActorByName( t_rightOperation.name );
										end
									end

									if Actor == nil then
										ShowMidTips("对方不在视野内");
										return;
									end

									if Actor:getType() == GAT_PET then
										local petPackage = mainplayer:getPet();
										petPackage:petWatch( t_rightOperation["id"] );
									end
								end
t_rightOperation["观察对方"].func = function ( )
									local mainplayer = ActorMgr:getMainPlayer()
									if mainplayer == nil then return end
									local Actor;
									if t_rightOperation["FriendOnline"] ~= nil then
										if t_rightOperation["FriendOnline"] == CLIENT_OFFLINE then
											ShowMidTips("对方不在线")
										else
											GameFriendManager:RequestWatchFriend( t_rightOperation["name"] );
										end
										return;
									end
									if t_rightOperation["watchIgnorDist"] then
										RankMgr:requestWatchPlayer( t_rightOperation["name"] );
										return;
									else
										if t_rightOperation["id"] ~= 0 then
											Actor = ActorMgr:FindActor( t_rightOperation["id"] );
										else
											Actor = ActorMgr:findActorByName( t_rightOperation.name );
										end
									end

									if Actor == nil then
										ShowMidTips("对方不在视野内");
										return;
									end

									if Actor:getType() == GAT_PET then
										local petPackage = mainplayer:getPet();
										petPackage:petWatch( t_rightOperation["id"] );
									end
								end

t_rightOperation["组队"].func = function ( )
									TeamManager:Team_OP_Add( t_rightOperation.name );
								end

--[[t_rightOperation["申请入队"].func = function ( )
									TeamManager:Team_OP_Apply( t_rightOperation.name );
								end--]]

t_rightOperation["交易"].func = function ( )
									local mainplayer = ActorMgr:getMainPlayer();
									if mainplayer == nil then 
										return;
									end

									if OnlineStoreFrame:IsShown() then
										return ShowMidTips( "查看店铺中，无法进行交易" );
									end

									local actor	= ActorMgr:findActorByName( t_rightOperation.name );
									if actor == nil then 
										ShowMidTips( "对方下线或者不在视野范围内，申请交易失败" );
										return; 
									end

									GameExchange:inviteExchange( actor:getName() );
								end

t_rightOperation["加为好友"].func = function ( )
									if not isRoleInRequestList( t_rightOperation.name ) then
										--[[
										if GameFriendManager:isPlayerRelation( CLIENT_RELATION_TYPE_BLACK, t_rightOperation.name ) then
											local frame = getglobal( "FriendAddBlackToFriend_ConfirmFrame" );
											frame:SetClientString( t_rightOperation.name );
											frame:Show();
										else
											GameFriendManager:addFriend( t_rightOperation.name, "" );
										end
										--]]

										GameFriendManager:addFriend( t_rightOperation.name, "" );
									end
								end

t_rightOperation["跟随"].func = function ( )
									local mainplayer = ActorMgr:getMainPlayer();
									if mainplayer:isCaptain() then										
										local teamInfo = TeamManager:getTeamInfo();
										for i = 1, teamInfo.MemberNum do
											local teamMember = teamInfo.TeamMember[i-1];
											if teamMember.MemberName == t_rightOperation.name then
												ShowMidTips( "队长不能跟随队员" );
												return;
											end
										end
									else
										if TeamManager:isInTeamFollow() then
											ShowMidTips( "组队跟随中无法进行普通跟随" );
											return;
										end
									end
									if GameFollowManager:isWeiXing() then
										GameFollowManager:cancelWeiXing();
									end
									if t_rightOperation.id == nil or t_rightOperation.id == 0 then
										GameFollowManager:beginWeiXing( t_rightOperation.name, 0, 0 );
									else
										GameFollowManager:beginWeiXing( t_rightOperation.name, t_rightOperation.id, t_rightOperation.followType );
									end
								end

t_rightOperation["切磋"].func = function ( )
									if OnlineStoreFrame:IsShown() then
										return ShowMidTips( "查看店铺中，无法申请切磋" );
									end

									local actor = ActorMgr:findActorByName( t_rightOperation.name );
									if actor ~= nil then
										GameActorPVP:requestSafePK( actor:getName() );
									end
								end

t_rightOperation["备注姓名"].func = function ( )
										ModifyMemName();
									end

t_rightOperation["删除"].func = function ( )
										DeleteCharacterName();
									end

t_rightOperation["加入黑名单"].func = function ( )
										GameFriendManager:addBlack( t_rightOperation.name );
									end

t_rightOperation["复制名称"].func = 
function ( )
	CopyToMemory( GetPlayerName(t_rightOperation.name) );
end

t_rightOperation["加入宿敌"].func = 
function ( )
	if GameFriendManager:isPlayerRelation( CLIENT_RELATION_TYPE_FRIEND, t_rightOperation.name ) then
		ShowMidTips( "请解除好友关系后再添加宿敌名单" );
		return;
	end

	GameFriendManager:addEnem( t_rightOperation.name, t_rightOperation.nHeight, t_rightOperation.nLow );
end

t_rightOperation["邀请骑乘"].func = 
function ( )
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	if mainplayer:isOnMonsterMachine() then
		-- 邀请怪物机械
		local monster = ActorMgr:FindActor( mainplayer:getMonsterMachine().m_playerInfo.m_nMonsterMemID );
		if monster == nil then return end;
		local monMachine = monster:getMonsterMachine();
		if not monMachine:canChangeSide() and monMachine:canRideOn() then
			local actor = ActorMgr:findActorByName( t_rightOperation.name );
			if actor ~= nil then
				mainplayer:requestMachineInvite( actor:GetID() );
			end
		end
	else
		local actor = ActorMgr:findActorByName( t_rightOperation.name );
		local canInviteRide = 
			function ( actor )
				if actor == nil then
					ShowMidTips( "所邀请对象不在视野范围内，不能邀请骑乘" );
					return false;
				end
				
				local mainplayer = ActorMgr:getMainPlayer();
				local dist = util.CalActorsDistance( actor, mainplayer );
				if dist > 5 ^ 2 then
					ShowMidTips( "所邀请对象距离太远，不能邀请骑乘" );
					return false;
				end

				if GameFriendManager:isPlayerRelation( CLIENT_RELATION_TYPE_SU_DI, actor:getName() ) then
					ShowMidTips( "目标在你的宿敌名单中，无法同骑，请按“F”键打开宿敌名单查看" );
					return false;
				end

				return true;
			end
		
		if canInviteRide( actor ) then
			local mainplayer = ActorMgr:getMainPlayer();
			local rider	= mainplayer:getRider();
			rider:inviteRide( actor:GetID() );
		end
	end
end

t_rightOperation["踢下骑乘"].func = 
function ( )
	local actor = ActorMgr:findActorByName( t_rightOperation.name );
	if actor == nil then
		return;
	end

	local targetRider	= actor:getRider();
	local mainplayer	= ActorMgr:getMainPlayer();
	
	-- 若被选定的目标处于主角的骑乘，则主角就可以将他踢下马
	if targetRider:isOnHorse() and targetRider:getRideItem().m_nOwnerID == mainplayer:GetID() then
		local rider	= mainplayer:getRider();
		rider:kickOffPassenger( actor:GetID() );
	end
end

t_rightOperation["邀请入会"].func = 
function ( )
	GameClanManager:clan_Opt_Invite( t_rightOperation.name );
end

t_rightOperation["查看公会"].func = 
function ( )
	local actor = ActorMgr:findActorByName( t_rightOperation.name );
	if actor then
		ClanListFrame:SetPoint( "center", "$parent", "center", 0, 0 );
		ClanListFrame:Show();
		ClanList_SearchClanBtn:Disable();
		GameClanManager:clan_Opt_SearchClanByWID( actor:getClanWID() );
	end
end

t_rightOperation["转让队长"].func = 
function ( )
	TeamManager:Team_OP_CaptainChange( t_rightOperation.name );
end

t_rightOperation["请离队伍"].func = 
function ( )
	TeamManager:Team_OP_Kick( t_rightOperation.name );
end

t_rightOperation["查看资源"].func = 
function ( )
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	WatchMachineResourceFrame:SetClientUserData( 0,mainplayer:GetSelectTarget() );
	WatchMachineResourceFrame:Show( );
end

t_rightOperation["请离投票"].func = t_rightOperation["请离队伍"].func;

t_rightOperation["同意入队"].func = 
function ()
	local applyPlayer = TeamManager:getTeamApplyInfo( t_rightOperation.name )
	TeamManager:Team_OP_ApplyAccept( applyPlayer.RoleID.nHeight, applyPlayer.RoleID.nLow );
	CancelBlinkPlayer( t_rightOperation.name );
	ClearSelMember();
	UpdateApplyList();
end

t_rightOperation["拒绝入队"].func = 
function ()
	local applyPlayer = TeamManager:getTeamApplyInfo( t_rightOperation.name )
	CancelBlinkPlayer( t_rightOperation.name );
	TeamManager:Team_OP_ApplyRefuse( applyPlayer.RoleID.nHeight, applyPlayer.RoleID.nLow );
	ClearSelMember();
	UpdateApplyList();
end

t_rightOperation["清空列表"].func = 
function ()
	TeamManager:Team_OP_ApplyClear();
	CancelAllBlinkApply();
	ClearSelMember();
	UpdateApplyList();
end

t_rightOperation["刷新列表"].func = 
function ()
	TeamManager:updateNearPlayer();
	SelectFirstAroundPlayer();
	UpdateAroundList();
end

-- 若右键选项针对的玩家是队员的话
--	若主角是队长的话，加个提升队长操作
-- 专门用于显示头像右键选项的回调函数
function ShowCharacterRightFrame( szName )
	local frame = getglobal( "FriendFrame_RightOptionFrame" );
	HideRightFrameAllButton();
	local nShowBtn = 0;

	local t_rightOptions = {};
	local member	= TeamManager:getTeamMemberInfo( szName );
	local actor		= ActorMgr:findActorByName( szName );
	-- 若是自己的队员，
	if member ~= nil then
		t_rightOptions = { op = t_rightOperation[CLIENT_RELATION_TYPE_SELFTEAM], relationType = CLIENT_RELATION_TYPE_SELFTEAM };
	-- 若是对方队伍的队员
	elseif actor ~= nil and actor:isInTeam() then
		t_rightOptions = { op = t_rightOperation[CLIENT_RELATION_TYPE_OTHERTEAM], relationType = CLIENT_RELATION_TYPE_OTHERTEAM };
	else
	-- 若没有任何关系
		t_rightOptions = { op = t_rightOperation[CLIENT_RELATION_TYPE_NONE], relationType = CLIENT_RELATION_TYPE_NONE };
	end

	for i = 1, table.getn( t_rightOptions["op"] ) do
		local btn	= getglobal( "FriendFrame_RightBtn" .. i );
		local font	= getglobal( "FriendFrame_RightBtn" .. i .. "Font" );
		font:SetText( t_rightOptions["op"][i] );
		font:SetTextColor( 145, 125, 55 );
		btn:Show();
		btn:Enable();
		nShowBtn = nShowBtn + 1;
	end
	
	SetRightFrameInfo( {  name = szName, relationType = CLIENT_RELATION_TYPE_NONE, nHeight = 0, followType = 0,
								nLow = 0, frame = "FriendFrame_RightOptionFrame", watchIgnorDist = true, id = nId } );
	-- 若目标玩家是队友的话且主角处于骑乘状态，则要多一个 "邀请骑乘" 的右键选项
	local mainplayer	= ActorMgr:getMainPlayer();
	local rider			= mainplayer:getRider();
	local curUsingRide	= rider:getRideItem();
	local rideDef		= getRideDef( curUsingRide.RideID, curUsingRide.nLevel );
	local targetActor	= ActorMgr:findActorByName( szName );
	if targetActor ~= nil then
		local btn	= getglobal( "FriendFrame_RightBtn" .. ( nShowBtn + 1 ) );
		local font	= getglobal( "FriendFrame_RightBtn" .. ( nShowBtn + 1 ) .. "Font" );
		font:SetText( "切磋" );
		font:SetTextColor( 145, 125, 55 );
		btn:Show();
		btn:Enable();
		nShowBtn = nShowBtn + 1;
	end
	-- 若所邀请的人是主角的队友，且主角处于骑乘状态，且主角所用的坐骑是多人坐骑，且被邀请的人不在我的多人坐骑上
	if TeamManager:isInTeam( szName ) and mainplayer:isInRide() and rideDef.RideOnNum > 0 and mainplayer:GetID() == curUsingRide.m_nOwnerID then
		local szTex = "";
		-- 若邀请的人与主角坐在同一个位置上，则不能邀请
		if targetActor ~= nil and targetActor:getRider():getRideItem().m_nOwnerID ~= curUsingRide.m_nOwnerID then
			szTex = "邀请骑乘";
		elseif targetActor ~= nil and targetActor:getRider():getRideItem().m_nOwnerID == curUsingRide.m_nOwnerID then -- 若邀请的队友是主角的骑乘上面
			szTex = "踢下骑乘";
		end
		
		if szTex ~= "" then
			local btn	= getglobal( "FriendFrame_RightBtn" .. ( nShowBtn + 1 ) );
			local font	= getglobal( "FriendFrame_RightBtn" .. ( nShowBtn + 1 ) .. "Font" );
			font:SetText( szTex );
			font:SetTextColor( 145, 125, 55 );
			btn:Show();
			btn:Enable();
			nShowBtn = nShowBtn + 1;
		end
	end

	-- 怪物机械邀请
	if mainplayer:isOnMonsterMachine() then		
		local monster = ActorMgr:FindActor( mainplayer:getMonsterMachine().m_playerInfo.m_nMonsterMemID );
		if monster == nil then return end;
		local monMachine = monster:getMonsterMachine();
		if not monMachine:canChangeSide() and monMachine:canRideOn() then
			local btn	= getglobal( "FriendFrame_RightBtn" .. ( nShowBtn + 1 ) );
			local font	= getglobal( "FriendFrame_RightBtn" .. ( nShowBtn + 1 ) .. "Font" );
			font:SetText( "邀请骑乘" );
			font:SetTextColor( 145, 125, 55 );
			btn:Show();
			btn:Enable();
			nShowBtn = nShowBtn + 1;
		end
	end
	
	-- 若主角是副会长或者会长的时候，出现邀请入会的选项
	if IsInClan() then
		local t_canAddMemberPositoins = { 
			CLAN_POS_VICE_PRESIDENT, CLAN_POS_PRESIDENT,
			CLAN_POS_GROUP_1, CLAN_POS_GROUP_2,
			CLAN_POS_GROUP_3, CLAN_POS_GROUP_4,
			CLAN_POS_GROUP_5, CLAN_POS_GROUP_6, };
		for _, position in ipairs( t_canAddMemberPositoins ) do
			if mainplayer:getClanPosition() == position then
				local btn	= getglobal( "FriendFrame_RightBtn" .. ( nShowBtn + 1 ) );
				local font	= getglobal( "FriendFrame_RightBtn" .. ( nShowBtn + 1 ) .. "Font" );
				font:SetText( "邀请入会" );
				font:SetTextColor( 145, 125, 55 );
				btn:Enable();
				btn:Show();
				nShowBtn = nShowBtn + 1;
				break;
			end
		end
	end
	local clanName = actor and actor:getClanName()
	-- 跨服的时候，不显示查看公会
	if not mainplayer:isOutSide() and clanName and clanName ~= "" then
		local btn	= getglobal( "FriendFrame_RightBtn" .. ( nShowBtn + 1 ) );
		local font	= getglobal( "FriendFrame_RightBtn" .. ( nShowBtn + 1 ) .. "Font" );
		font:SetText( "查看公会" );
		font:SetTextColor( 145, 125, 55 );
		btn:Enable();
		btn:Show();
		nShowBtn = nShowBtn + 1;
	end
	
	-- 若是自己队伍的人
	if t_rightOptions["relationType"] == CLIENT_RELATION_TYPE_SELFTEAM then
		-- 看主角是否是队长，若是，则显示转让队长的选项
		if mainplayer:isCaptain() then
			local t_options = { "转让队长" };
			-- 如果目标在副本里面 显示请离投票
			if member.MapID >= CLIENT_GAME_PWORLD_MAPID then
				table.insert( t_options,"请离投票" );
			else
				table.insert( t_options,"请离队伍" );
			end
			for _, option in ipairs( t_options ) do
				local btn	= getglobal( "FriendFrame_RightBtn" .. ( nShowBtn + 1 ) );
				local font	= getglobal( "FriendFrame_RightBtn" .. ( nShowBtn + 1 ) .. "Font" );
				font:SetText( option );
				font:SetTextColor( 145, 125, 55 );
				btn:Enable();
				btn:Show();
				nShowBtn = nShowBtn + 1;
			end
			-- 如果在怪物机械上面，添加右键菜单
		else
			-- 是否在副本中
			--if MapShower:getRealMapId() >= CLIENT_GAME_PWORLD_MAPID then
				local btn	= getglobal( "FriendFrame_RightBtn" .. ( nShowBtn + 1 ) );
				local font	= getglobal( "FriendFrame_RightBtn" .. ( nShowBtn + 1 ) .. "Font" );
				font:SetText( "请离投票" );
				font:SetTextColor( 145, 125, 55 );
				btn:Enable();
				btn:Show();
				nShowBtn = nShowBtn + 1;
			--end
		end
	end
	
	frame:SetHeight( 14 + nShowBtn * 16 );
	frame:Show();
end

function ShowApplyRightFrame( szName )
	local frame = getglobal( "FriendFrame_RightOptionFrame" );
	HideRightFrameAllButton();

	local t_rightOptions = {};

	local btn	= getglobal( "FriendFrame_RightBtn1");
	local font	= getglobal( "FriendFrame_RightBtn1Font" );
	font:SetText( "同意入队" );
	font:SetTextColor( 145, 125, 55 );
	btn:Show();
	btn:Enable();

	btn	= getglobal( "FriendFrame_RightBtn2");
	font	= getglobal( "FriendFrame_RightBtn2Font" );
	font:SetText( "拒绝入队" );
	font:SetTextColor( 145, 125, 55 );
	btn:Show();
	btn:Enable();

	btn	= getglobal( "FriendFrame_RightBtn3");
	font	= getglobal( "FriendFrame_RightBtn3Font" );
	font:SetText( "加为好友" );
	font:SetTextColor( 145, 125, 55 );
	btn:Show();
	btn:Enable();

	btn	= getglobal( "FriendFrame_RightBtn4");
	font	= getglobal( "FriendFrame_RightBtn4Font" );
	font:SetText( "清空列表" );
	font:SetTextColor( 145, 125, 55 );
	btn:Show();
	btn:Enable();
	
	btn = getglobal( "FriendFrame_RightBtn5" )
	font	= getglobal( "FriendFrame_RightBtn4Font" );
	font:SetText( "观察玩家" );
	font:SetTextColor( 145, 125, 55 );
	btn:Show();
	btn:Enable();
	
	SetRightFrameInfo( { name = szName, relationType = CLIENT_RELATION_TYPE_NONE, nHeight = 0,
								nLow = 0, id = 0, frame = "FriendFrame_RightOptionFrame", watchIgnorDist = true } );
	
	frame:SetHeight( 14 + 5 * 16 );
	frame:Show();
end

function ShowAroundRightFrame( szName )
	local frame = getglobal( "FriendFrame_RightOptionFrame" );
	HideRightFrameAllButton();

	local t_rightOptions = {};

	local btn	= getglobal( "FriendFrame_RightBtn1");
	local font	= getglobal( "FriendFrame_RightBtn1Font" );
	font:SetText( "组队" );
	font:SetTextColor( 145, 125, 55 );
	btn:Show();
	btn:Enable();

	btn	= getglobal( "FriendFrame_RightBtn2");
	font	= getglobal( "FriendFrame_RightBtn2Font" );
	font:SetText( "加为好友" );
	font:SetTextColor( 145, 125, 55 );
	btn:Show();
	btn:Enable();

	btn	= getglobal( "FriendFrame_RightBtn3");
	font	= getglobal( "FriendFrame_RightBtn3Font" );
	font:SetText( "刷新列表" );
	font:SetTextColor( 145, 125, 55 );
	btn:Show();
	btn:Enable();
	
	btn = getglobal( "FriendFrame_RightBtn4" )
	font	= getglobal( "FriendFrame_RightBtn4Font" );
	font:SetText( "观察玩家" );
	font:SetTextColor( 145, 125, 55 );
	btn:Show();
	btn:Enable();
	
	SetRightFrameInfo( { name = szName, relationType = CLIENT_RELATION_TYPE_NONE, nHeight = 0,
								nLow = 0, id = 0, frame = "FriendFrame_RightOptionFrame", watchIgnorDist = true } );
	
	frame:SetHeight( 14 + 4 * 16 );
	frame:Show();
end

function ShowStorePlayerRightFrame( szName )
	local frame = getglobal( "FriendFrame_RightOptionFrame" );
	HideRightFrameAllButton();
	local nShowBtn = 0;

	local t_rightOptions = { "私聊", "观察玩家", "组队", "加为好友", "加入黑名单", "复制名称" };

	for i = 1, #t_rightOptions do
		local btn	= getglobal( "FriendFrame_RightBtn" .. i );
		local font	= getglobal( "FriendFrame_RightBtn" .. i .. "Font" );
		font:SetText( t_rightOptions[i] );
		font:SetTextColor( 145, 125, 55 );
		btn:Show();
		btn:Enable();
		nShowBtn = nShowBtn + 1;
	end
	
	SetRightFrameInfo( {  name = szName, relationType = CLIENT_RELATION_TYPE_NONE, nHeight = 0, followType = 0,
								nLow = 0, frame = "FriendFrame_RightOptionFrame", watchIgnorDist = true, id = nId } );
	frame:SetHeight( 14 + nShowBtn * 16 );
	frame:Show();
end

function NeedGrayRongHeOp( nDestPetMemId )
	local destPetActor	= ActorMgr:FindActor( nDestPetMemId );
	if destPetActor == nil then
		return true;
	end

	if destPetActor:getPetGenius() < CLIENT_PET_GENIUS_B then
		return true;
	end

	if destPetActor:getLv() < CLIENT_MIN_RONG_HE_LV then
		return true;
	end

	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local myFightPet	= GetFightPet( mainplayer );
	if myFightPet == nil then
		return true;
	end

	if myFightPet:getPetGenius() < CLIENT_PET_GENIUS_B then
		return true;
	end

	if myFightPet:getLv() < CLIENT_MIN_RONG_HE_LV then
		return true;
	end
	
	local petInfo = petPackage:getPetInfo( myFightPet:getRoleWId() );
	-- 检查是否是同类宠物
	if petInfo.PetID ~= destPetActor:getPetDefId() then
		return true;
	end

	return false;
end

function ShowPetRightFrame( nId )
	local nShowBtn = 0;
	local frame = getglobal( "FriendFrame_RightOptionFrame" );
	HideRightFrameAllButton();
	nShowBtn = nShowBtn + 1;
	local btn	= getglobal( "FriendFrame_RightBtn"..nShowBtn );
	local font	= getglobal( btn:GetName().."Font" );
	font:SetText( "观察对方" );
	font:SetTextColor( 145, 125, 55 );
	btn:Enable();
	btn:Show();
	SetRightFrameInfo( { name = "", relationType = CLIENT_RELATION_TYPE_NONE, nHeight = 0,
								nLow = 0, id = nId, frame = "FriendFrame_RightOptionFrame" } );
	
	local petActor		= ActorMgr:FindActor( nId );
	local mainplayer	= ActorMgr:getMainPlayer();
	-- 若不是主角的宠物
	if false and mainplayer:GetID() ~= petActor:getOwnerId() then
		nShowBtn = nShowBtn + 1;
		btn		= getglobal( "FriendFrame_RightBtn"..nShowBtn );
		font	= getglobal( btn:GetName().."Font" );
		font:SetText( "宠物融合" );
		if NeedGrayRongHeOp( nId ) then
			font:SetTextColor( 160, 160, 160 );
			btn:Disable();
		else
			font:SetTextColor( 145, 125, 55 );
			btn:Enable();
		end
		
		btn:Show();
	end
	frame:SetHeight( 14 + nShowBtn * 16 );
	frame:Show();
end

function ShowConvoyRightFrame( nId, nType )
	local nShowBtn = 0;
	local frame = getglobal( "FriendFrame_RightOptionFrame" );
	HideRightFrameAllButton();
	nShowBtn = nShowBtn + 1;
	local btn = getglobal( "FriendFrame_RightBtn"..nShowBtn );
	local font = getglobal( btn:GetName().."Font" );
	font:SetText( "跟随" );
	font:SetTextColor( 145, 125, 55 );
	btn:Enable();
	btn:Show();
	SetRightFrameInfo( { name = "", relationType = CLIENT_RELATION_TYPE_NONE, nHeight = 0, nLow = 0, id = nId, 
				followType = nType, frame = "FriendFrame_RightOptionFrame" } );		
	frame:SetHeight( 14 + nShowBtn * 16 );
	frame:Show();
end

function HideRightFrameAllButton()
	for i = 1, MAX_RIGHT_FRAME_BTNS do
		local btn = getglobal( "FriendFrame_RightBtn" .. i );
		btn:Hide();
	end
end

function FriendFrame_RightBtn_OnClick()
	local font = getglobal( this:GetName() .. "Font" );
	t_rightOperation[font:GetText()].func();
	local frame = getglobal( this:GetParent() );
	frame:Hide();
end

function FriendFrame_RightBtn_OnEnter()
	local font = getglobal( this:GetName() .. "Font" );
	font:SetTextColor( 255, 254, 189 );
end

function FriendFrame_RightBtn_OnLeave()
	local font = getglobal( this:GetName() .. "Font" );
	font:SetTextColor( 145, 125, 55 );
end

function SetRightFrameInfo( info )
	t_rightOperation.name		= info.name;
	t_rightOperation.relationType	= info.relationType;
	t_rightOperation.nHeight	= info.nHeight;
	t_rightOperation.nLow		= info.nLow;
	t_rightOperation.id		= info.id;
	t_rightOperation.frame		= info.frame;
	t_rightOperation.watchIgnorDist	= info.watchIgnorDist;
	t_rightOperation.FriendOnline	= info.FriendOnline;
	t_rightOperation.followType	= info.followType;
end

-- 此为点击RichText上面一层的button的时候响应

function CanStartSpeakWithPlayer( data )
	local roleInfo = GameFriendManager:getFriendInfoByName( data["name"] );
	if not ChatFrameManager:IsNewSpeaker( data["name"] ) and roleInfo.m_nIsOnline == CLIENT_OFFLINE then
		return false;
	end

	if g_nDisplayPageType == CLIENT_RELATION_TYPE_BLACK then
		return false;
	end

	if ChatFrameManager:GetSpeakerFrame( data["name"] ) ~= nil then
		return;
	end

	return true;
end

function FriendShipFrame_CharacterNameBtn_OnClick()
	if g_nDisplayPageType == CLIENT_RELATION_TYPE_FRIEND or g_nDisplayPageType == CLIENT_RELATION_TYPE_TMPFRIEND then
		local btn = getglobal( this:GetParent() );
		local rich = getglobal( this:GetParent() .. "CharacterName" );
		g_CurClickRole = { roleLowId = rich:GetClientUserData( 0 ), roleHeightId = rich:GetClientUserData( 1 ), roleName = rich:GetClientString() };
		ResetFriendHighLightTex();
		if btn:GetClientString() == "" and rich:GetClientString() ~= "" then
			local nIndex = rich:GetClientUserData( 2 );
			t_LabelNameList[nIndex].bCheck = not t_LabelNameList[nIndex].bCheck;
			SetScrollBar( "FriendShipFrame_ScrollBar", MAX_ROLE_NAME_LINE, getTotalBtn() );
			local slider = getglobal( "FriendShipFrame_ScrollBar" );
			UpdateFriendNameList( slider:GetValue() + 1 );	
			return;
		end
		if arg1 ~= "RightButton" then
			if rich:GetClientString() == "" then return end
			-- 若所点击的按钮中不存储任何玩家信息
			if "" == rich:GetClientString( ) then
				return;
			end

			-- 若好友不在我新手消息队列里切不在线给个提示，但是不弹出对话框
			local roleInfo ;
			local roleInfo = GameFriendManager:getFriendInfoByName( g_CurClickRole.roleName );
			if roleInfo.RoleID == 0 then return end
			if not ChatFrameManager:IsNewSpeaker( g_CurClickRole.roleName ) and roleInfo.Online == CLIENT_OFFLINE then
				ShowMidTips( "该好友目前不在线,不能发起新的对话" );
				return;
			end

			local szFrameName = "";

			-- 先检查是否有与被点击玩家对应的且正在显示聊天窗口,若有则什么也做就结束子程序
			if ChatFrameManager:GetSpeakerFrame( g_CurClickRole.roleName ) ~= nil then
				return;
			end
			
			-- 若没有与被点击玩家对应的聊天窗口或聊天提示框
			ChatFrameManager:OpenChatFrame( g_CurClickRole.roleName );
		else			
			if FriendInfoFrame:IsShown() then
				FriendInfoFrame:Hide();
			end
			FriendInfoFrame:SetPoint( "center", "UIClient", "center", 0, 0 );
			FriendInfoFrame:Show();
		end
	elseif g_nDisplayPageType == CLIENT_RELATION_TYPE_PRENTICE then
		local btn = getglobal( this:GetParent() );
		local rich = getglobal( this:GetParent() .. "CharacterName" );
		if btn:GetClientString() == "" and rich:GetClientString() ~= "" then return end
		g_CurClickRole = { roleLowId = rich:GetClientUserData( 0 ), roleHeightId = rich:GetClientUserData( 1 ), roleName = rich:GetClientString() };
		
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer and mainplayer:getName() == g_CurClickRole.roleName then
			ShowMidTips("不能对自己进行操作");
			return;
		end
		
		ResetHighLightTex();
		if arg1 ~= "RightButton" then
			if rich:GetClientString() == "" then return end
			-- 若所点击的按钮中不存储任何玩家信息
			if "" == rich:GetClientString( ) then
				return;
			end

			-- 若好友不在我新手消息队列里切不在线给个提示，但是不弹出对话框
			local roleInfo;
			if GameSchool:GetSelfSchoolJobType() == ROLE_SCHOOL_TYPE_NONE_TOLUA then
				return
			elseif g_CurClickRole.roleLowId == 0 and g_CurClickRole.roleHeightId == 0 then
				roleInfo = {};
				roleInfo.Online = CLIENT_ONLINE;
			elseif GameSchool:GetSelfSchoolJobType() == ROLE_SCHOOL_TYPE_TEACHER_TOLUA then
				roleInfo = GameSchool:GetSelfStudent( g_CurClickRole.roleName )
			elseif GameSchool:GetSelfSchoolJobType() == ROLE_SCHOOL_TYPE_STUDENT_TOLUA then
				roleInfo = GameSchool:GetSelfTeacher()
			end
			if not ChatFrameManager:IsNewSpeaker( g_CurClickRole.roleName ) and roleInfo.Online == CLIENT_OFFLINE then
				ShowMidTips( "该玩家目前不在线,不能发起新的对话" );
				return;
			end

			local szFrameName = "";

			-- 先检查是否有与被点击玩家对应的且正在显示聊天窗口,若有则什么也做就结束子程序
			if ChatFrameManager:GetSpeakerFrame( g_CurClickRole.roleName ) ~= nil then
				return;
			end
			
			-- 若没有与被点击玩家对应的聊天窗口或聊天提示框
			ChatFrameManager:OpenChatFrame( g_CurClickRole.roleName, CL_CHAT_WINDOW );
		else
			if g_CurClickRole.roleLowId == 0 and g_CurClickRole.roleHeightId == 0 then
				local rich = getglobal( this:GetParent() .. "CharacterName" );
				if rich:GetClientString() == "" then return end
				g_CurClickRole = { roleLowId = rich:GetClientUserData( 0 ), roleHeightId = rich:GetClientUserData( 1 ), roleName = rich:GetClientString() };
				
				local frame = getglobal( "FriendFrame_RightOptionFrame" );
				HideRightFrameAllButton();
				local nOptionNum = table.getn( t_rightOperation[CLIENT_RELATION_TYPE_NEWCOMER] );
				for i = 1, table.getn( t_rightOperation[CLIENT_RELATION_TYPE_NEWCOMER] ) do
					local btn	= getglobal( "FriendFrame_RightBtn" .. i );
					local font	= getglobal( "FriendFrame_RightBtn" .. i .. "Font" );
					font:SetText( t_rightOperation[CLIENT_RELATION_TYPE_NEWCOMER][i] );
					font:SetTextColor( 145, 125, 55 );
					btn:Enable();
					btn:Show();
				end
				
				SetRightFrameInfo( { name = g_CurClickRole.roleName, relationType = CLIENT_RELATION_TYPE_NEWCOMER, nHeight = g_CurClickRole.roleHeightId,
										nLow = g_CurClickRole.roleLowId, id = 0, frame = "FriendFrame_RightOptionFrame", watchIgnorDist = true ,
										FriendOnline = 1 } );

				-- 判断目标玩家是否是队友，若是队友
				-- 显示右键选项框
				frame:SetClientString( g_CurClickRole.roleName );
				frame:SetClientUserData( 0, rich:GetClientUserData( 0 ) );
				frame:SetClientUserData( 1, rich:GetClientUserData( 1 ) );
				frame:SetHeight( 14 + nOptionNum * 16 );
				frame:Show();
			else
				if SchoolInfoFrame:IsShown() then
					SchoolInfoFrame:Hide();
				end
				GameSchool:RequestMemInfo( g_CurClickRole.roleName )
			end
		end
	else
		local rich = getglobal( this:GetParent() .. "CharacterName" );
		if rich:GetClientString() == "" then return end
		g_CurClickRole = { roleLowId = rich:GetClientUserData( 0 ), roleHeightId = rich:GetClientUserData( 1 ), roleName = rich:GetClientString() };
		ResetHighLightTex();
		if arg1 ~= "RightButton" then	
			-- 若好友不在我新手消息队列里切不在线给个提示，但是不弹出对话框
			local roleInfo = GameFriendManager:getFriendInfoByName( g_CurClickRole.roleName );
			if not ChatFrameManager:IsNewSpeaker( g_CurClickRole.roleName ) and roleInfo.m_nIsOnline == CLIENT_OFFLINE then
				ShowMidTips( "该好友目前不在线,不能发起新的对话" );
				return;
			end

			-- 若点击黑名单中的人物,不弹出对话框
			if GameFriendManager:isPlayerRelation( CLIENT_RELATION_TYPE_BLACK, g_CurClickRole.roleName ) then
				ShowMidTips( "不能与黑名单中的玩家对话" );
				return;
			end

			local szFrameName = "";

			-- 先检查是否有与被点击玩家对应的且正在显示聊天窗口,若有则什么也做就结束子程序
			if ChatFrameManager:GetSpeakerFrame( g_CurClickRole.roleName ) ~= nil then
				return;
			end
			
			-- 若没有与被点击玩家对应的聊天窗口或聊天提示框
			ChatFrameManager:OpenChatFrame( g_CurClickRole.roleName );
		else
			local frame = getglobal( "FriendFrame_RightOptionFrame" );
			HideRightFrameAllButton();
			local nOptionNum = table.getn( t_rightOperation[g_nDisplayPageType] );
			for i = 1, table.getn( t_rightOperation[g_nDisplayPageType] ) do
				local btn	= getglobal( "FriendFrame_RightBtn" .. i );
				local font	= getglobal( "FriendFrame_RightBtn" .. i .. "Font" );
				font:SetText( t_rightOperation[g_nDisplayPageType][i] );
				font:SetTextColor( 145, 125, 55 );
				btn:Enable();
				btn:Show();
			end
			
			if CanStartSpeakWithPlayer( { ["name"] = g_CurClickRole["roleName"] } ) then
				nOptionNum		= nOptionNum + 1;
				local nBtnIndex = nOptionNum;
				local btn		= getglobal( "FriendFrame_RightBtn" .. nBtnIndex );
				local font		= getglobal( btn:GetName() .. "Font" );
				font:SetText( "发起对话" );
				font:SetTextColor( 145, 125, 55 );
				btn:Enable();
				btn:Show();
			end
			local character	= GameFriendManager:getFriendInfoByName( g_nDisplayPageType, g_CurClickRole.roleName );
			SetRightFrameInfo( { name = g_CurClickRole.roleName, relationType = g_nDisplayPageType, nHeight = g_CurClickRole.roleHeightId,
										nLow = g_CurClickRole.roleLowId, id = 0, frame = "FriendFrame_RightOptionFrame", watchIgnorDist = true ,
										FriendOnline = character.m_nIsOnline } );

			-- 判断目标玩家是否是队友，若是队友
			-- 显示右键选项框
			frame:SetClientString( g_CurClickRole.roleName );
			frame:SetClientUserData( 0, rich:GetClientUserData( 0 ) );
			frame:SetClientUserData( 1, rich:GetClientUserData( 1 ) );
			frame:SetHeight( 14 + nOptionNum * 16 );
			frame:Show();
		end
	end	
end

function HideAllChatListButton()
		for i = 1, MAX_CHAT_FRAME do
		local btn = getglobal( "FriendFrame_ChatFrameBtn" .. i );
		btn:Hide();
	end
end

-- 刷新聊天提示面板的显示情况
function UpdateChatClueFrame()
	local nameList = ChatFrameManager:GetLessenNameQueue();
	if table.getn( nameList ) == 0 then
		FriendFrame_ChatClueFrame:Hide();
	end
end

function UpdateChatListFrame()
	HideAllChatListButton();
	
	local nameList = ChatFrameManager:GetLessenNameQueue();
	for i = 1, math.min( table.getn( nameList ), MAX_CHAT_FRAME ) do
		local btn	= getglobal( "FriendFrame_ChatFrameBtn" .. i );
		local font	= getglobal( "FriendFrame_ChatFrameBtn" .. i .. "Font" );
		font:SetText( nameList[i] );
		font:SetTextColor( 145, 125, 55 );
		btn:Show();
	end

	FriendFrame_ChatListFrame:SetHeight( 14 + math.min( table.getn( nameList ), MAX_CHAT_FRAME ) * 16 );
end

local t_mutexFrames = { "CreatingAutoTeamRightMenuFrame" };
-- 此为好友右键选项显示的时候响应
function FriendFrame_RightOptionFrame_OnShow()
	util.AdjustTooltipsScreenPos( { tips = this:GetName(), vertical = "bottom"} );
	for _, name in ipairs( t_mutexFrames ) do
		local frame = getglobal( name );
		if frame:IsShown() then
			frame:Hide();
		end
	end
end

function FriendFrame_ChatListFrameBtn_OnShow()
	UpdateChatListFrame();
	util.AdjustTooltipsScreenPos( { tips = "FriendFrame_ChatListFrame" } );
end

function FriendFrame_ChatListFrameBtn_OnHide()
	local nameList = ChatFrameManager:GetLessenNameQueue();
	if table.getn( nameList ) > 0 then
		FriendFrame_ChatClueFrame:Show();
	end
end

-- 最新聊天框
function FriendFrame_ChatFrameBtn_OnClick()
	local font = getglobal( this:GetName() .. "Font" );
	ChatFrameManager:OpenChatFrame( font:GetText() );
	FriendFrame_ChatListFrame:Hide();
end

function FriendFrame_ChatFrameBtn_OnEnter()
	local font = getglobal( this:GetName() .. "Font" );
	font:SetTextColor( 255, 254, 189 );
end

function FriendFrame_ChatFrameBtn_OnLeave()
	local font = getglobal( this:GetName() .. "Font" );
	font:SetTextColor( 145, 125, 55 );
end

function FriendFrame_ChatClueFrame_OnClick()
	local nameList = ChatFrameManager:GetLessenNameQueue();
	if table.getn( nameList ) == 1 then
		if FriendFrame_ChatClueFrame:GetClientUserData( 0 ) == CL_CHAT_WINDOW then
			ChatFrameManager:OpenChatFrame( nameList[1], CL_CHAT_WINDOW );
		else
			ChatFrameManager:OpenChatFrame( nameList[1] );
		end
		return;
	end

	util.AdjustTooltipsScreenPos( { tips = "FriendFrame_ChatListFrame" } );
	FriendFrame_ChatListFrame:Show();
	this:Hide();
end

-- 在有新消息的时候更新提示框的闪动状态
function FriendFrame_ChatClueFrame_OnUpdate()
	local t_newMessageQueue = ChatFrameManager:GetBlinkNameQueue();
	if table.getn( t_newMessageQueue ) == 0 then
		this:DisHighlight();
		return;
	end

	if this:IsHighlight() then
		this:DisHighlight();
	else
		this:Highlight();
	end
end

function FriendShipOrderBtn_OnClick()
	--TrustOrderListFrame:SetPoint( "center", "FriendShipFrame", "center", 0, 0 );
	TrustOrderListFrame:SetPoint( "topleft", "FriendShipFrame", "topleft", 0, 0 );
	FriendShipFrame:Hide();
	TrustOrderListFrame:Show();
	TrustOrderList_OrderBtn:Disable();
end

-- 
function FriendAddBlackToFriend_ConfirmFrame_ConfirmBtn_OnClick()
	local frame	= getglobal( "FriendAddBlackToFriend_ConfirmFrame" );
	local name	= frame:GetClientString();
	GameFriendManager:addFriend( name, "" );
	frame:Hide();
end

local t_allChatFrames = { "FriendChatFrame1", "FriendChatFrame2", "FriendChatFrame3", "FriendChatFrame4", "FriendChatFrame5", "FriendChatFrame6" };
function GetCurDrawHighChatFrame()
	local t_showChatFrame = {};
	for _, name in ipairs( t_allChatFrames ) do
		local frame = getglobal( name );
		if frame:IsShown() then
			table.insert( t_showChatFrame, name );
		end
	end
	
	if table.getn( t_showChatFrame ) == 0 then
		return nil;
	end

	local curDrawFrame = getglobal( t_showChatFrame[1] );
	for i = 2, table.getn( t_showChatFrame ) do
		local frame = getglobal( t_showChatFrame[i] );
		if curDrawFrame:getFrameDrawLevel() < frame:getFrameDrawLevel() then
			curDrawFrame = frame;
		end
	end

	return curDrawFrame;
end

function FriendShipFrame_CharacterNameTemplateTip_OnEter()
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "引力空间", 
								frame = this:GetParent(), button = "cursor" } );
end

function FriendShipFrame_CharacterNameTemplateLoverSign_OnEnter()
	local button = getglobal( this:GetParent() )
	local name = button:GetClientString()
	local friendInfo = GameFriendManager:getFriendInfoByName( name );
	local szInfo = string.format( "当前关系：#G%s#n\n当前亲密度：#G%d#n\n\n如何增加“亲密度”：\n1、双方组队完成野外日常任务\n2、双方组队完成地宫环组任务\n3、双方组队通关简单难度以上副本\n4、双方组队完成“宠趣”任务\n5、双方组队完成“涛声依旧”任务\n6、向好友赠送“甜心礼物”\n#R（某方解除好友，亲密度将被清除）#n",
							friendInfo.m_nIntimateColorType == INTIMATE_COLOR_LOVERS_TOLUA and "情侣" or "夫妻", friendInfo.m_nBuddyVal )
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szInfo, 
								frame = this:GetParent(), button = "cursor" } );
end

function FriendShipFrameTitleSignChk_OnClick()
	local rich = getglobal( "FriendShipFrame_CharacterNameBtn" .. this:GetClientID() .. "CharacterName" );
	local nIndex = rich:GetClientUserData( 2 );
	t_LabelNameList[nIndex].bCheck = not t_LabelNameList[nIndex].bCheck;
	SetScrollBar( "FriendShipFrame_ScrollBar", MAX_ROLE_NAME_LINE, getTotalBtn() );
	local slider = getglobal( "FriendShipFrame_ScrollBar" );	
	if g_nDisplayPageType == CLIENT_RELATION_TYPE_PRENTICE then
		UpdateSchoolNameList( slider:GetValue() + 1 )
	else
		UpdateFriendNameList( slider:GetValue() + 1 );
	end
end

function FriendInfoFrame_OnLoad()
	this:RegisterEvent( "GE_FRIEND_GET_REQUIRE_PLAYER_INFO" );
	this:RegisterEvent( "GE_FRIEND_GETLIST" );
end

function FriendInfoFrame_OnShow()
	local frame = getglobal( "FriendInfoFrame" );	
	frame:SetClientString( g_CurClickRole.roleName );
	if frame:GetClientString() == "" then 
		ShowMidTips( "错误的玩家信息" );
		FriendInfoFrame:Hide();
		return;
	end
	frame:SetClientUserData( 0, g_CurClickRole.roleLowId );
	frame:SetClientUserData( 1, g_CurClickRole.roleHeightId );
	UpdateFriendInfo( frame:GetClientString() );
end

function UpdateFriendInfo( roleName )
	local frame = getglobal( "FriendInfoFrame" );	
	local friendInfo = GameFriendManager:getFriendInfoByName( g_nDisplayPageType, roleName );
	if not friendInfo then
		ShowMidTips( "无法找到该好友信息" );
		FriendInfoFrame:Hide();
		return;
	end

	local rich = getglobal( "FriendInfoFrameNameBtnFont" );	
	rich:resizeRect( rich:GetTextExtentWidth( roleName ),16 )
	rich:SetText( roleName, 160, 160, 90 );	
	rich:SetClientUserData( 0, 1 );
	rich:ScrollFirst();
	rich:SetWidth( 100 );

	rich = getglobal( "FriendInfoFrameBkupNameBtnFont" );
	rich:resizeRect( rich:GetTextExtentWidth( friendInfo.m_szMemName ),16 )
	rich:SetText( friendInfo.m_szMemName, 160, 160, 90 );
	rich:SetClientUserData( 0, 1 );
	rich:ScrollFirst();
	rich:SetWidth( 100 );

	rich = getglobal( "FriendInfoFrameClanBtnFont" );
	rich:resizeRect( rich:GetTextExtentWidth( friendInfo.m_szClanName ),16 )
	rich:SetText( friendInfo.m_szClanName, 160, 160, 90 );
	rich:SetClientUserData( 0, 1 );
	rich:ScrollFirst();
	rich:SetWidth( 100 );
	FriendInfoFrameClanBtn:SetClientString( friendInfo.m_szClanName );

	rich = getglobal( "FriendInfoFrameEmoBtnFont" );
	rich:resizeRect( rich:GetTextExtentWidth( friendInfo.m_szEmotion ),16 )
	rich:SetText( friendInfo.m_szEmotion, 160, 160, 90 );
	rich:SetClientUserData( 0, 1 );
	rich:ScrollFirst();
	rich:SetWidth( 226 );

	local tipsContent = {									
				["FriendInfoFrameRaceBtnFont"]		= { [true] = t_raceList[friendInfo.m_nCareer],			[false] = "" },
				["FriendInfoFramePosBtnFont"]		= { [true] = "当前不在线",					[false] = "" },
				["FriendInfoFrameFriendBtnFont"]	= { [true] = 0,				[false] = "" },
				["FriendInfoFrameTeamBtnFont"]		= { [true] = "当前不在线",					[false] = "" },
				["FriendInfoFrameCharacterPhotoBtnFont"]= { [true] = "Lv" .. friendInfo.m_nLevel,			[false] = "" },
				["FriendInfoFrameQinMiBtnValFont"]	= { [true] = "亲密度：".. friendInfo.m_nBuddyVal,		[false] = "" },
				};
	
	for name, content in pairs( tipsContent ) do
		local font = getglobal( name );
		font:SetText( content[true] );
	end	

	 --显示玩家头像逻辑
	SetPhoto( friendInfo.m_nHair, friendInfo.m_nFace, "FriendInfoFrameCharacterPhotoBtnHairPhoto", "FriendInfoFrameCharacterPhotoBtnPhoto", friendInfo.m_nGender, friendInfo.m_nHead,
				"FriendInfoFrameCharacterPhotoBtnHairShadowTex", "FriendInfoFrameCharacterPhotoBtnBodyTex" );
	
	frame:SetClientUserData( 2, friendInfo.m_nGroupID > 0 and friendInfo.m_nGroupID or 1 );

	if g_nDisplayPageType == CLIENT_RELATION_TYPE_FRIEND then
		GameFriendManager:requirePlayerInfo( roleName, friendInfo.m_nIsOnline );
	else
		FriendInfoFramePosBtnFont:SetText("");
		FriendInfoFrameTeamBtnFont:SetText("");
		FriendInfoFrameFriendBtnFont:SetText("");
	end
end

local t_FriendInfoFrameEvents = { ["GE_FRIEND_GET_REQUIRE_PLAYER_INFO"] = {}, ["GE_FRIEND_GETLIST"] = {}, };

function FriendInfoFrame_OnEvent()	
	t_FriendInfoFrameEvents[arg1].func();
end

t_FriendInfoFrameEvents["GE_FRIEND_GET_REQUIRE_PLAYER_INFO"].func = 
function ()
	local requirePlayer = GameFriendManager:getCurRequireRoleInfo();
	if requirePlayer == nil then return end;
	local MaxMapControl = MapShower:GetMaxMapContal();
	if requirePlayer.Span == 1 then
		FriendInfoFramePosBtnFont:SetText( "跨服中" )
	else
		FriendInfoFramePosBtnFont:SetText( MaxMapControl:getMapName( requirePlayer.Map ) or "未知" );
	end
	FriendInfoFrameFriendBtnFont:SetText( requirePlayer.Charm )
	FriendInfoFrameTeamBtnFont:SetText( requirePlayer.TeamRoleNum > 0 and "已组队" .. requirePlayer.TeamRoleNum .. "人" or "未组队" )	
end

t_FriendInfoFrameEvents["GE_FRIEND_GETLIST"].func = 
function ()
	if not FriendInfoFrame:IsShown() then
		return;
	end
	local frame = getglobal( "FriendInfoFrame" );
	if frame:GetClientString() == "" then 
		FriendInfoFrame:Hide();
		return;
	end
	UpdateFriendInfo( frame:GetClientString() );
end

function FriendInfoFrame_OnHide()
	FriendMemFrame:Hide();
	FriendDelete_ConfirmFrame:Hide();
	FriendSetGroupFrame:Hide();
end

function FriendInfoFrame_OnUpdate()
	local rich = getglobal( "FriendInfoFrameNameBtnFont" );
	local width = rich:GetWidth()
	if rich:getLineWidth(0) > width then
		local sign = rich:GetClientUserData( 0 )
		local disx = rich:GetDispPosX() + 1 * sign
		rich:SetDispPosX( disx )
		if disx + 100 > rich:getLineWidth(0) + 10 then
			rich:SetClientUserData( 0,-1 )
		end
		if disx <= 0 then
			rich:SetClientUserData( 0,1 )
		end			
	end
	rich = getglobal( "FriendInfoFrameBkupNameBtnFont" );
	width = rich:GetWidth()
	if rich:getLineWidth(0) > width then
		local sign = rich:GetClientUserData( 0 )
		local disx = rich:GetDispPosX() + 1 * sign
		rich:SetDispPosX( disx )
		if disx + 100 > rich:getLineWidth(0) + 10 then
			rich:SetClientUserData( 0,-1 )
		end
		if disx <= 0 then
			rich:SetClientUserData( 0,1 )
		end			
	end
	rich = getglobal( "FriendInfoFrameClanBtnFont" );
	width = rich:GetWidth()
	if rich:getLineWidth(0) > width then
		local sign = rich:GetClientUserData( 0 )
		local disx = rich:GetDispPosX() + 1 * sign
		rich:SetDispPosX( disx )
		if disx + 100 > rich:getLineWidth(0) + 10 then
			rich:SetClientUserData( 0,-1 )
		end
		if disx <= 0 then
			rich:SetClientUserData( 0,1 )
		end			
	end
	rich = getglobal( "FriendInfoFrameEmoBtnFont" );
	local width = rich:GetWidth()
	if rich:getLineWidth(0) > width then
		local sign = rich:GetClientUserData( 0 )
		local disx = rich:GetDispPosX() + 1 * sign
		rich:SetDispPosX( disx )
		if disx + 234 > rich:getLineWidth(0) + 10 then
			rich:SetClientUserData( 0,-1 )
		end
		if disx <= 0 then
			rich:SetClientUserData( 0,1 )
		end			
	end
end

function FriendInfoFrameQinMiBtn_OnEnter()
	SetGameTooltips( this:GetName(), "#G亲密度≥300：双方可成为情侣\n亲密度≥2000：双方可结为夫妇#n\n\n如何增加“亲密度”：\n1、双方组队完成野外日常任务\n2、双方组队完成地宫环组任务\n3、双方组队通关简单难度以上副本\n4、双方组队完成“宠趣”任务\n5、双方组队完成“涛声依旧”任务\n6、向好友赠送“甜心礼物”\n#R（某方解除好友，亲密度将被清除）#n" )
end

function FriendInfoFrameNameCopyBtn_OnClick()
	local frame = getglobal( "FriendInfoFrame" );
	CopyToMemory( frame:GetClientString() );
end

function FriendInfoFrameClanNameCopyBtn_OnClick()
	local btn = getglobal( "FriendInfoFrameClanBtn" );
	CopyToMemory( btn:GetClientString() );
end

function FriendInfoFrameBkupNameEditBtn_OnClick()	
	if FriendMemFrame:IsShown() then
		return;
	end
	local frame = getglobal( "FriendInfoFrame" );
	if frame:GetClientString() == "" then 
		ShowMidTips( "错误的玩家信息" );
		FriendInfoFrame:Hide();
		return;
	end
	local roleName = frame:GetClientString();
	if GameFriendManager:getPlayerRelation( roleName ) ~= CLIENT_RELATION_TYPE_FRIEND then
		ShowMidTips( "只能对好友设置备注" );
		return;
	end
	FriendMemFrame:Show();
	FriendMemFrame_InputEdit:Clear();
	SetFocusFrame( "FriendMemFrame_InputEdit" );
end

function FriendInfoFrameTalkBtn_OnClick()
	local frame = getglobal( "FriendInfoFrame" );
	if frame:GetClientString() == "" then 
		ShowMidTips( "错误的玩家信息" );
		FriendInfoFrame:Hide();
		return;
	end
	local roleName = frame:GetClientString();

	-- 若好友不在我新手消息队列里切不在线给个提示，但是不弹出对话框
	local roleInfo = GameFriendManager:getFriendInfoByName( roleName );
	if not ChatFrameManager:IsNewSpeaker( roleName ) and roleInfo.m_nIsOnline == CLIENT_OFFLINE then
		ShowMidTips( "该好友目前不在线,不能发起新的对话" );
		return;
	end

	local szFrameName = "";

	-- 先检查是否有与被点击玩家对应的且正在显示聊天窗口,若有则什么也做就结束子程序
	if ChatFrameManager:GetSpeakerFrame( roleName ) ~= nil then
		return;
	end
	
	-- 若没有与被点击玩家对应的聊天窗口或聊天提示框
	ChatFrameManager:OpenChatFrame( roleName );
end

function FriendInfoFrameSctalkBtn_OnClick()
	local frame = getglobal( "FriendInfoFrame" );
	if frame:GetClientString() == "" then 
		ShowMidTips( "错误的玩家信息" );
		FriendInfoFrame:Hide();
		return;
	end
	SendPrivateMessage( frame:GetClientString() );
end

function FriendInfoFrameDelFriBtn_OnClick()
	local frame = getglobal( "FriendInfoFrame" );
	if frame:GetClientString() == "" then 
		ShowMidTips( "错误的玩家信息" );
		FriendInfoFrame:Hide();
		return;
	end
	FriendDelete_ConfirmFrame:Show();		
end

function FriendInfoFrameTradeBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then return end
	local frame = getglobal( "FriendInfoFrame" );
	if frame:GetClientString() == "" then 
		ShowMidTips( "错误的玩家信息" );
		FriendInfoFrame:Hide();
		return;
	end

	if OnlineStoreFrame:IsShown() then
		return ShowMidTips( "查看店铺中，无法进行交易" );
	end

	local actor = ActorMgr:findActorByName(  frame:GetClientString() );
	if actor == nil then 
		ShowMidTips( "对方下线或者不在视野范围内，申请交易失败" );
		return; 
	end

	GameExchange:inviteExchange( actor:getName() );
end

function FriendInfoFrameGrouopBtn_OnClick()
	local frame = getglobal( "FriendInfoFrame" );
	if frame:GetClientString() == "" then 
		ShowMidTips( "错误的玩家信息" );
		FriendInfoFrame:Hide();
		return;
	end
	local roleName = frame:GetClientString();
	if GameFriendManager:getPlayerRelation( roleName ) ~= CLIENT_RELATION_TYPE_FRIEND then
		ShowMidTips( "只能对好友进行分组" );
		return;
	end
	if FriendSetGroupFrame:IsShown() then
		FriendSetGroupFrame:Hide();
		return;
	end
	FriendSetGroupFrame:Show();
end

function FriendInfoFrameBlackBtn_OnClick()
	local frame = getglobal( "FriendInfoFrame" );
	if frame:GetClientString() == "" then 
		ShowMidTips( "错误的玩家信息" );
		FriendInfoFrame:Hide();
		return;
	end
	GameFriendManager:addBlack( frame:GetClientString() );
end

function FriendInfoFrameInvTeamBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then return end
	local frame = getglobal( "FriendInfoFrame" );
	if frame:GetClientString() == "" then 
		ShowMidTips( "错误的玩家信息" );
		FriendInfoFrame:Hide();
		return;
	end
	if FriendInfoFramePosBtnFont:GetText() == "当前不在线" then
		ShowMidTips( "该玩家当前不在线" );
		return;
	end
	TeamManager:Team_OP_Add( frame:GetClientString() );
end

--[[
function FriendInfoFrameApealTeamBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then return end
	local frame = getglobal( "FriendInfoFrame" );
	if frame:GetClientString() == "" then 
		ShowMidTips( "错误的玩家信息" );
		FriendInfoFrame:Hide();
		return;
	end
	if FriendInfoFramePosBtnFont:GetText() == "当前不在线" then
		ShowMidTips( "该玩家当前不在线" );
		return;
	end
	TeamManager:Team_OP_Apply( frame:GetClientString() );
end
--]]

function FriendInfoFrameWatchBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then return end
	local frame = getglobal( "FriendInfoFrame" );
	if frame:GetClientString() == "" then 
		ShowMidTips( "错误的玩家信息" );
		FriendInfoFrame:Hide();
		return;
	end
	if FriendInfoFramePosBtnFont:GetText() == "当前不在线" then
		ShowMidTips( "该玩家当前不在线" );
		return;
	end
	GameFriendManager:RequestWatchFriend( frame:GetClientString() );
end

local MAXGROUPNUM = 5;
local t_ChGroupName = { "一", "二", "三", "四", "五" };
function FriendSetGroupFrame_OnShow()
	local frame = getglobal( "FriendInfoFrame" );
	if frame:GetClientString() == "" then 
		ShowMidTips( "错误的玩家信息" );
		FriendInfoFrame:Hide();
		return;
	end
	FriendSetGroupFrameNowFont:SetText( "当前分组：" .. t_ChGroupName[ frame:GetClientUserData( 2 ) ] );	
	for i=1, MAXGROUPNUM do
		local btn = getglobal( "FriendSetGroupFrameGroupBtn" .. i );
		btn:SetClientUserData( 0, i );
	end
end

function FriendSetGroupFrameGroupBtn_OnClick()
	local frame = getglobal( "FriendInfoFrame" );
	if frame:GetClientString() == "" then 
		ShowMidTips( "错误的玩家信息" );
		FriendInfoFrame:Hide();
		return;
	end
	local nGroupID = this:GetClientUserData( 0 );
	if nGroupID == nil or nGroupID <= 0 then
		ShowMidTips( "错误的分组信息" );
		FriendInfoFrame:Hide();
		return;
	end
	frame:SetClientString( frame:GetClientString() );
	frame:SetClientUserData( 0, frame:GetClientUserData( 0 ) );
	frame:SetClientUserData( 1, frame:GetClientUserData( 1 ) );
	GameFriendManager:setFriendGroup( frame:GetClientString(), nGroupID );
	FriendSetGroupFrame:Hide();
end

function FriendShipFrame_MutilSend_OnClick()
	FriendMutilSendFrame:SetPoint( "center", "UIClient", "center", 0, 0 );
	FriendMutilSendFrame:Show();
end

function FriendShipFrameSchoolGiftBtn_OnEnter()
	SetGameTooltips( this:GetName(), "等级成长奖励：\n导师收入学生后，学生每成长到一定的等级时，导师可以领取一份系统奖励\n领取条件：\n学生必须在导师指导下成长10级后，导师才可以领取对应的等级成长奖励" )
end

function FriendMutilSendFrame_OnShow()
	SetFocusFrame( "FriendMutilSendFrame_InputEdit" );
	for i=1, MAXGROUPNUM do
		local chkBtn = getglobal( "FriendMutilSendFrameGroupChkBtn" .. i );
		chkBtn:setCheckState( true );
	end
end

function FriendMutilSendFrame_OnHide()
	FriendMutilSendFrame_InputEdit:SetText( "" );
	ClearSetLink();
	countTime = 0;
end

local countTime = 0;
function FriendMutilSendFrameSendBtn_OnClick()
	local mainplayer	= ActorMgr:getMainPlayer();
	local szParentName	= this:GetParent();
	local inputEdit		= getglobal( szParentName.."_InputEdit" );
	local szChatContent	= inputEdit:GetText();	

	--不允许发送空消息
	if "" == szChatContent then
		ShowMidTips( "发送消息不能为空" );
		return; 
	end
	
	local isCheck = false;
	local t_SelGroup = {};
	for i=1, MAXGROUPNUM do
		local chkBtn = getglobal( "FriendMutilSendFrameGroupChkBtn" .. i );
		if chkBtn:GetCheckState() then
			--table.insert( t_SelGroup, i );
			for j=1,table.getn( t_LabelNameList ) do
				if t_LabelNameList[j].nSubType == 1 and t_LabelNameList[j].nGroup == i then
					table.insert( t_SelGroup, i );
					break;
				end
			end
			isCheck = true;			
		end
	end
	--在引力空间的好友
	local t_MudGroup = {};
	local nMaxRelativeRole = GameFriendManager:getRelationPlayerCount( CLIENT_RELATION_TYPE_FRIEND );
	for i=1,nMaxRelativeRole do
		local friendInfo = GameFriendManager:getFriendInfoByIndex( CLIENT_RELATION_TYPE_FRIEND, i - 1 );
		if friendInfo.m_nIsOnline == MUD_ONLINE then
			table.insert( t_MudGroup, friendInfo.m_RoleID );
		end
	end

	if not isCheck then
		ShowMidTips( "请选择至少一个分组" );
		return;
	end

	if table.getn( t_SelGroup ) == 0 then
		ShowMidTips( "你所选的分组中没有好友" );
		return;
	end

	if countTime > 0 and os.clock() - countTime < 5 then
		ShowMidTips( "消息群发的间隔为5秒，请稍后再发" );
		return; 
	end

	szChatContent = string.gsub( szChatContent, "#[L]", "**" );
	
	local t_ClientSelfChatContentParser = GetClientSelfChatContentParser();

	if t_ClientSelfChatContentParser:DoClientDefineInstruction( { ["text"] = szChatContent } ) then
		return;
	end
	szChatContent = t_ClientSelfChatContentParser:parseLinkTextForClient( { ["text"] = szChatContent } );
	-- 将消息发给聊天对象	
	sz2Server = t_ClientSelfChatContentParser:parseLinkTextForServe( { ["text"] = szChatContent } );

	local groupMsg = ChatManager:getGroupMsg();
	groupMsg.iGroup = table.getn( t_SelGroup );
	for i=1, table.getn( t_SelGroup ) do
		groupMsg.Groups[ i - 1 ] = t_SelGroup[i];
	end
	groupMsg.iMud = table.getn( t_MudGroup );
	for i=1, table.getn( t_MudGroup ) do
		groupMsg.Muds[ i - 1 ] = t_MudGroup[i];
	end
	ChatManager:requestMutilChat( groupMsg, sz2Server );

	-- 清空输入框中的内容
	inputEdit:Clear();
	countTime = os.clock();
	ClearSetLink();		
end

function FriendShipFrameFriendNumBtn_OnEnter()
	local szText = "当前好友/好友上限";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function FriendChatFrame_InviteBtn_OnEnter()
	local szText = "和好友一起组队";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function FriendChatFrame_InviteBtn_OnClick()
	local szParentName	= this:GetParent();
	local szSpeakerName	= ChatFrameManager:GetSpeakerName( szParentName );
	TeamManager:Team_OP_Add( szSpeakerName );
end
--[[
function FriendChatFrame_JoinBtn_OnEnter()
	local szText = "申请加入好友所在的队伍";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function FriendChatFrame_JoinBtn_OnClick()
	local szParentName	= this:GetParent();
	local szSpeakerName	= ChatFrameManager:GetSpeakerName( szParentName );
	TeamManager:Team_OP_Apply( szSpeakerName );
end
--]]
function FriendChatFrame_InputEdit_OnEscapePressed()
	local btn = getglobal( this:GetParent() );
	ChatFrameManager:ShutDownChatFrame( btn:GetName() )
	SetFocusFrame( "UIClient" );
end
---------------------------------------------------送花界面-----------------------------------------------------
local t_FlowerState = {}
local t_FlowerTypeInfo = { [0]={ path="uires\\ui\\rose01.tga", name="玫瑰花", cost=1, effect="effect\\entity\\5012125.ent", }, 
							[1]={ path="uires\\ui\\knx.tga", name="康乃馨", cost=10, effect="effect\\entity\\5012126.ent", }, 
							[2]={ path="uires\\ui\\lsyj.tga", name="蓝色妖姬", cost=100, effect="effect\\entity\\5012127.ent", }
						}
local t_EggsTypeInfo = { [0]={ name="鸡蛋", cost=1, }, [1]={ name="炸弹", cost=10, }, [2]={ name="便便" }, cost=100, }

function ReceiveFlower( nFlag, nType, szAccept, szSend, nNum )
	AddFlowerReport( nFlag, nType, szAccept, szSend, nNum )
end

function SetflowerEffect( nType )
	local t_FlowerEffectPath = { "effect\\entity\\pm_hmg.ent", "effect\\entity\\pm_knx.ent", "effect\\entity\\pm_lsyj.ent" }
	if nType >= table.getn( t_FlowerEffectPath ) then return end
	FlowerEffectFrameModelView:setPetModel(0,0, t_FlowerEffectPath[nType+1] );
	FlowerEffectFrameModelView:setPetScale(0,0.2*1,0.2*1,0.2*1);
	FlowerEffectFrameModelView:setEntityPosition(0,0,200,0);
end
--------------------------------------------鲜花播报频道----------------------------------------------------
local t_FlowerReportList = {}
local MAX_FLOWER_REPORT_LINE = 3
--播报结束时的渐隐控制标识
local endBegin = false
local endBeginTime = 0
--当消耗大于500金币时的延迟显示标识
local keepShow = true
local keepShowBeginTime = 0
local t_ReportTexPath = {
	[0] = {
		[0] = "uires\\TuPianLei\\ShiShengXiTong\\5012125.tga",
		[1] = "uires\\TuPianLei\\ShiShengXiTong\\5012127.tga",
		[2] = "uires\\TuPianLei\\ShiShengXiTong\\5012126.tga",
	},
	[1] = {
		[0] = "uires\\TuPianLei\\ShiShengXiTong\\5012128.tga",
		[1] = "uires\\TuPianLei\\ShiShengXiTong\\5012130.tga",
		[2] = "uires\\TuPianLei\\ShiShengXiTong\\5012129.tga",
	},
}
function AddFlowerReport( nFlag, nType, szAccept, szSend, nNum )
	local mainplayer = ActorMgr:getMainPlayer()
	local oneFlowerReport = { Flag = nFlag, Type = nType, name = nFlag == 0 and t_FlowerTypeInfo[nType].name or t_EggsTypeInfo[nType].name, Accepter = szAccept, Sender = szSend, totalNum = nNum, curNum = nNum, reportTex = t_ReportTexPath[nFlag][nType] }
	table.insert( t_FlowerReportList, oneFlowerReport )
	--如果没有在延迟显示，就立刻停止渐隐，进行下一次播报
	if not keepShow then
		endBegin = false
		endBeginTime = 0
	end
	--小退后再上线，会出现不播报送花信息，所以每次收到消息的时候都要保证鲜花播报频道是显示的
	FlowerReportFrame:Show()
end

function FlowerReportFrame_OnLoad()
	this:setUpdateTime(0.03)
	FlowerReportFrameWords:setCenterLine( true )
end
function FlowerReportFrame_OnUpdate()
	--如果在延迟显示阶段，检查时间，判断是否可以结束延迟显示
	if keepShow then
		if os.clock() - keepShowBeginTime >= 10 then
			keepShow = false
			keepShowBeginTime = 0
			FlowerEffectFrame:Hide()
		end	
	end
	--如果在播报渐隐，检查时间，判断是否要结束渐隐
	if endBegin then
		local timePast = os.clock() - endBeginTime
		local endAlpha = 1 - timePast*100/1000
		if timePast >= 10 then
			endBegin = false
			endBeginTime = 0
			endAlpha = 0		
		end
		FlowerReportFrameWords:SetAlpha( endAlpha )
		FlowerReportFrameFlower1:SetBlendAlpha( endAlpha )
		FlowerReportFrameFlower2:SetBlendAlpha( endAlpha )
		FlowerReportFrameFlower3:SetBlendAlpha( endAlpha )
	end
	--如果列表不为空，且不在延迟显示时间段，则结束渐隐，开始下次播报
	if table.getn( t_FlowerReportList ) ~= 0 then
		if not keepShow then
			endBegin = false
			endBeginTime = 0
			endAlpha = 0		
		end
	end
	if endBegin or keepShow then return end
	if table.getn( t_FlowerReportList ) ~= 0 then
		--如果是此条信息第一次播报
		if t_FlowerReportList[1].totalNum == t_FlowerReportList[1].curNum then
			--砸蛋类型不显示
			if t_FlowerReportList[1].Flag == 0 then
				--判断要显示哪种类型的特效
				local nType = t_FlowerReportList[1].Type
				local nNum = t_FlowerReportList[1].totalNum
				if t_FlowerTypeInfo[nType].cost * nNum < 500 then
					--不会有延迟显示
					keepShow = false
					keepShowBeginTime = 0
					local mainplayer = ActorMgr:getMainPlayer()
					if mainplayer:getName() == t_FlowerReportList[1].Accepter or mainplayer:getName() == t_FlowerReportList[1].Sender then
						playUiEffect( 0,0,1, t_FlowerTypeInfo[nType].effect )
					end
				else
					--记录延迟显示开始时间
					keepShowBeginTime = os.clock()
					FlowerEffectFrame:Show()
					SetflowerEffect( nType )
				end
			end
		end
		--如果此条信息播报完毕
		if t_FlowerReportList[1].curNum == 0 then
			endBegin = true
			endBeginTime = os.clock()
			local nType = t_FlowerReportList[1].Type
			local nNum = t_FlowerReportList[1].totalNum
			--超过500金币，就要延迟显示
			if t_FlowerTypeInfo[nType].cost * nNum >= 500 then
				keepShow = true
			end
			table.remove( t_FlowerReportList, 1 )
			--如果不需要延迟显示
			if not keepShow then
				FlowerEffectFrame:Hide()
			--需要延迟显示，但是播报时间超过了10秒
			elseif keepShow and os.clock() - keepShowBeginTime >= 10 then
				FlowerEffectFrame:Hide()
			end
			return			
		end
		for i = 1, MAX_FLOWER_REPORT_LINE do
			local flowerTex = getglobal( "FlowerReportFrameFlower"..i )
			flowerTex:Hide()
		end
		FlowerReportFrameWords:SetText( "", 255,50,50 )
		FlowerReportFrameWords:SetAlpha( 1.0 )
		FlowerReportFrameFlower1:SetBlendAlpha( 1.0 )
		FlowerReportFrameFlower2:SetBlendAlpha( 1.0 )
		FlowerReportFrameFlower3:SetBlendAlpha( 1.0 )
		t_FlowerReportList[1].curNum = t_FlowerReportList[1].curNum - 1
		--设置要播报的信息
		for i = 1, MAX_FLOWER_REPORT_LINE do
			local flowerTex = getglobal( "FlowerReportFrameFlower"..i )
			local index = t_FlowerReportList[1].totalNum - t_FlowerReportList[1].curNum - (MAX_FLOWER_REPORT_LINE - i)
			local szReport = t_FlowerReportList[1].Flag == 0 and "%s\t赠送给\t%s" or "%s\t对\t%s\t使用了道具"
			if index <= 0 then
				szReport = " \n\n"
				flowerTex:Hide()
			else
				szReport = string.format( szReport, t_FlowerReportList[1].Sender,t_FlowerReportList[1].Accepter )
				local nOffsetX1 = FlowerReportFrameWords:GetTextExtentWidth( szReport )
				flowerTex:SetTexture( t_FlowerReportList[1].reportTex )
				szReport = szReport.."\t\t\t"..index.."\n\n"
				local nOffsetX2 = FlowerReportFrameWords:GetTextExtentWidth( szReport )
				flowerTex:SetPoint( "topleft","FlowerReportFrame", "topleft", 157+nOffsetX1/2 -(nOffsetX2-nOffsetX1)/2, (i-1)*30-15 )
				flowerTex:Show()
			end
			FlowerReportFrameWords:AddText( szReport, 255,255,190 )
		end
	end
end
-------------------------------------------送花下拉列表选择界面---------------------------------------------------
local t_UseFlowerChoseList = { 1, 10, 30, 66, 188, 520, 1314, }
function UseFlowerChoseShowBtnTemplate_OnClick()
	local index = this:GetClientID()
	UseFlowerFrameEdit:SetText( t_UseFlowerChoseList[index] )
	UseFlowerChoseFrame:Hide()
	UseEggsChoseFrame:Hide()
end
--------------------------------------------赠送鲜花界面/使用道具界面-----------------------------------------------
local t_UseFlowerList = {
	[0] = {
		[0] = { x="1008", y="114", w="15", h="15", texOffset = "66", text="玫瑰花\t\t(-1金币，+1魅力值)", cost = 1, },
		[1] = { x="1008", y="130", w="15", h="15", texOffset = "66", text="康乃馨\t\t(-10金币，+15魅力值)", cost = 1, },
		[2] = { x="1008", y="146", w="15", h="15", texOffset = "81", text="蓝色妖姬\t\t(-100金币，+180魅力值)", cost = 1, },
	},
	[1] = {
		[0] = { x="291", y="689", w="15", h="15", texOffset = "51", text="鸡蛋\t\t(-1金币，-1魅力值)", cost = 1, },
		[1] = { x="307", y="689", w="15", h="15", texOffset = "51", text="炸弹\t\t(-10金币，-15魅力值)", cost = 1, },
		[2] = { x="323", y="689", w="15", h="15", texOffset = "51", text="便便\t\t(-100金币，-180魅力值)", cost = 1, },
	},
}

local MAX_FLOWER_TYPE = 3

function OpenUseFlower( nFlag, szName )
	if nFlag == 0 then
		UseFlowerFrameHeadFont:SetText( "送花" )	
		UseFlowerFrameUseBtn:SetText( "赠送" )
		UseFlowerFrameNumFont:SetText( "赠送数量" )
		UseFlowerFrameInfo:SetText( "向好友赠送鲜花会为你的好友增加魅力值." )
	elseif nFlag == 1 then
		UseFlowerFrameHeadFont:SetText( "砸蛋" )
		UseFlowerFrameUseBtn:SetText( "使用" )
		UseFlowerFrameNumFont:SetText( "使用数量" )
		UseFlowerFrameInfo:SetText( "向好友使用道具会降低你的好友魅力值." )
	end
	UseFlowerFrame:SetClientUserData( 0, nFlag )
	UseFlowerFrame:SetClientString( szName )
	UseFlowerFrame:Show()
end

function PresentFlowerTemplate_OnClick()
	for i = 1, MAX_FLOWER_TYPE do
		local btn = getglobal( "UseFlowerFrameItem"..i.."Check" )
		btn:setCheckState( false )
	end
	local index = this:GetClientID()
	local CheckBtn = getglobal( "UseFlowerFrameItem"..index.."Check" )
	CheckBtn:setCheckState( true )
end

function PresentFlowerTemplateCheckBtn_OnClick()
	for i = 1, MAX_FLOWER_TYPE do
		local btn = getglobal( "UseFlowerFrameItem"..i.."Check" )
		btn:setCheckState( false )
	end
	local parent = getglobal( this:GetParent() )
	local index = parent:GetClientID()
	local CheckBtn = getglobal( "UseFlowerFrameItem"..index.."Check" )
	CheckBtn:setCheckState( true )
end	

function UseFlowerFrame_OnShow()
	UseFlowerChoseFrame:Hide()
	UseEggsChoseFrame:Hide()
	UseFlowerFrame:SetPoint( "center", "UIClient", "center", 0,0 )
	local nFlag = UseFlowerFrame:GetClientUserData(0)
	for i = 1, MAX_FLOWER_TYPE do
		local btn = getglobal( "UseFlowerFrameItem"..i )
		local btnTex = getglobal( "UseFlowerFrameItem"..i.."Tex" )
		btnTex:SetTexUV( t_UseFlowerList[nFlag][i-1].x, t_UseFlowerList[nFlag][i-1].y, t_UseFlowerList[nFlag][i-1].w, t_UseFlowerList[nFlag][i-1].h)
		btnTex:SetPoint( "left", btn:GetName(), "left", t_UseFlowerList[nFlag][i-1].texOffset, 0 )
		local btnInfo = getglobal( "UseFlowerFrameItem"..i.."Info" )
		btnInfo:SetText( t_UseFlowerList[nFlag][i-1].text, 255, 255, 190 )
	end
	UseFlowerFrameEdit:SetText( t_UseFlowerChoseList[1] )
	for i = 1, MAX_FLOWER_TYPE do
		local btn = getglobal( "UseFlowerFrameItem"..i.."Check" )
		btn:setCheckState( false )
	end
	UseFlowerFrameItem1Check:setCheckState( true )
end

function UseFlowerFrameUseBtn_OnClick()	
	for i = 1, MAX_FLOWER_TYPE do
		local btn = getglobal( "UseFlowerFrameItem"..i.."Check" )
		if btn:GetCheckState() then
			local szText = UseFlowerFrameEdit:GetText();
			if szText == "" then
				return;
			end
			local itemNum = tonumber( szText );
			if itemNum == nil or itemNum == 0 then
				ShowMidTips( "请选择赠送数量" )
				return
			end
			local nFlag = UseFlowerFrame:GetClientUserData(0)
			local szName = UseFlowerFrame:GetClientString()
			--是否是赠送人自己
			local mainplayer = ActorMgr:getMainPlayer()
			if szName == mainplayer:getName() then
				ShowMidTips( "不可以对自己砸蛋" )
				return
			end
			--先检查是否有足够的金币
			if mainplayer:getGodCoin() < t_UseFlowerList[nFlag][i-1].cost * itemNum then
				MessageBoxForVip( "提示", "已提取#cffff00金币#n不足，请#cffff00提取金币#n后重新操作" )
				MessageBoxFrameForVipAcceptButton:SetText("提取金币");
				UseFlowerFrame:Hide();
				return
			end
			GameSchool:CharmRequest( nFlag, i - 1, szName, itemNum )
		end
	end
	UseFlowerChoseFrame:Hide()
	UseEggsChoseFrame:Hide()
end

function UseFlowerFrameDownBtn_OnClick()
	local nFlag = UseFlowerFrame:GetClientUserData(0)
	local frameName = nFlag == 0 and "UseFlowerChoseFrame" or "UseEggsChoseFrame"
	local frame = getglobal( frameName )
	if frame:IsShown() then
		frame:Hide()
	else
		frame:Show()
	end
end

function UseFlowerFrameEdit_OnUpdate()
	local szText = UseFlowerFrameEdit:GetText();
	local itemNum = tonumber( szText );
	if itemNum == 0 then
		UseFlowerFrameEdit:SetText( "1" )
	end
end
-----------------------------------------------送花，金币不足提醒--------------------------------------
function GodCoinShortFrame_OnLoad()
	GodCoinShortFrameInfo:setCenterLine( true )
	GodCoinShortFrameInfo:SetText( "对不起，您的金币不足，充值后再重新点击赠送吧", 255,255,190 )
end
function GodCoinShortFrameChargeBtn_OnClick()
	GameMgr:openWebSite( "http://pay.ljy0.com" );
end
----------------------------------------------师徒相关处理--------------------------------------------------
function OpenSchoolChatFrame( szName )
	-- 先检查是否有与被点击玩家对应的且正在显示聊天窗口,若有则什么也做就结束子程序
	if ChatFrameManager:GetSpeakerFrame( szName ) ~= nil then
		return;
	end
	
	ChatFrameManager:OpenChatFrame( szName, CL_CHAT_WINDOW )
	SendSchoolWorld( szName )
end
--强制发送收徒邀请
function SendSchoolWorld( szName )
	local mainplayer	= ActorMgr:getMainPlayer();
	local szMyName		= mainplayer:getName();
	local szParentName	= ChatFrameManager:GetSpeakerFrame( szName );
	local inputEdit		= getglobal( szParentName.."_InputEdit" );
	local szChatContent = inputEdit:GetText();
	local szSpeakerName	= ChatFrameManager:GetSpeakerName( szParentName );
	local chatFrame		= getglobal( szParentName );

	szChatContent = "我想收你为我的学生，可以吗？";
	
	local t_ClientSelfChatContentParser = GetClientSelfChatContentParser();

	if t_ClientSelfChatContentParser:DoClientDefineInstruction( { ["text"] = szChatContent } ) then
		return;
	end
	szChatContent = t_ClientSelfChatContentParser:parseLinkTextForClient( { ["text"] = szChatContent } );

	-- 将消息发给聊天对象	
	sz2Server = t_ClientSelfChatContentParser:parseLinkTextForServe( { ["text"] = szChatContent } );
	ChatManager:requestChat( CL_CHAT_WINDOW, szSpeakerName, sz2Server, CLIENT_CHAT_NOT_SHOW_IN_CHAT_FRAME, 0 );

	-- 清空输入框中的内容
	inputEdit:Clear();
	ClearSetLink();
end

function SetSchoolRelation()
	t_SchoolGroupList = {}
	--设置显示列表
	if GameSchool:GetSelfSchoolJobType() == ROLE_SCHOOL_TYPE_NONE_TOLUA then
		table.insert( t_SchoolGroupList,{ name = "我的导师", bCheck = true, nGroup = 0, nSubType = 0} );
		table.insert( t_SchoolGroupList,{ name = "我的学生", bCheck = true, nGroup = 1, nSubType = 0 } );
	elseif GameSchool:GetSelfSchoolJobType() == ROLE_SCHOOL_TYPE_TEACHER_TOLUA then
		table.insert( t_SchoolGroupList,{ name = "我的导师", bCheck = true, nGroup = 0, nSubType = 0} );
		table.insert( t_SchoolGroupList,{ name = "我的学生", bCheck = true, nGroup = 1, nSubType = 0 } );
		for i = 1, GameSchool:GetSelfStudentNum() do
			local studentInfo = GameSchool:GetSelfStudentByIndex( i - 1 )
			table.insert( t_SchoolGroupList,{ name = studentInfo.RoleName, bCheck = false, nGroup = 1, 
					nSubType = 1, nLow = studentInfo.RoleID.nLow, nHeight = studentInfo.RoleID.nHeight });
		end
	elseif GameSchool:GetSelfSchoolJobType() == ROLE_SCHOOL_TYPE_STUDENT_TOLUA then
		table.insert( t_SchoolGroupList,{ name = "我的导师", bCheck = true, nGroup = 0, nSubType = 0} );
		local teacherInfo = GameSchool:GetSelfTeacher()
		table.insert( t_SchoolGroupList,{ name = teacherInfo.RoleName, bCheck = false, nGroup = 0, 
				nSubType = 1, nLow = teacherInfo.RoleID.nLow, nHeight = teacherInfo.RoleID.nHeight });
		table.insert( t_SchoolGroupList,{ name = "我的学生", bCheck = true, nGroup = 1, nSubType = 0 } );
	end
	
	table.insert( t_SchoolGroupList,{ name = "新生推荐",     bCheck = true, nGroup = 2, nSubType = 0 } );
	local newPeopleInfo = LoginMgr:getNewPeopleInfo();
	if newPeopleInfo.Num <= MAX_NEW_PEOPLE_NUM_TOLUA then
		for i = 1, newPeopleInfo.Num do
			local peopleInfo = newPeopleInfo.PeopleInfo[i - 1];
			table.insert( t_SchoolGroupList,{ name = peopleInfo.RoleName, bCheck = false, nGroup = 2, 
					nSubType = 1, nLow = 0, nHeight = 0, nNewcomer = true });
		end
	end
	SetScrollBar( "FriendShipFrame_ScrollBar", MAX_ROLE_NAME_LINE, #t_SchoolGroupList );
	local slider = getglobal( "FriendShipFrame_ScrollBar" );
	UpdateSchoolNameList( slider:GetValue() + 1 )
end

function UpdateSchoolRelationShip()
	if g_nDisplayPageType == CLIENT_RELATION_TYPE_PRENTICE then
		SetSchoolRelation()
		local slider = getglobal( "FriendShipFrame_ScrollBar" );
		slider:SetValue( 0 );
		UpdateSchoolNameList( slider:GetValue() + 1 );
	end
end
function SetSchoolNameList()
	--如果当前显示的是师徒关系标签
	if g_nDisplayPageType == CLIENT_RELATION_TYPE_PRENTICE then
		t_LabelNameList = t_SchoolGroupList
	end
end
function UpdateSchoolNameList( nIndex )
	ClearRoleNameList();
	SetSchoolNameList();
	FriendShipFrameSchoolGiftBtn:Show()
	local nCurShowBtn = 1;
	local shownum = 0;
	for i = 1, table.getn( t_LabelNameList ) do
		if nCurShowBtn > MAX_ROLE_NAME_LINE then
			break;
		end
		if canShowBtn( i ) then
			shownum = shownum + 1;
		end
		if shownum >= nIndex then
			local button	= getglobal( "FriendShipFrame_CharacterNameBtn" .. nCurShowBtn );
			local rich	= getglobal( "FriendShipFrame_CharacterNameBtn" .. nCurShowBtn .. "CharacterName" );
			local chk	= getglobal( "FriendShipFrameTitleSignChk" .. nCurShowBtn );
			local BookBtn = getglobal( "FriendShipFrame_CharacterNameBtn" .. nCurShowBtn .. "BookBtn" );
			local GiftBtn = getglobal( "FriendShipFrame_CharacterNameBtn" .. nCurShowBtn .. "GiftBtn" );
			local FlowerBtn = getglobal( "FriendShipFrame_CharacterNameBtn" .. nCurShowBtn .. "FlowerBtn" );
			local LoveSignBtn = getglobal( "FriendShipFrame_CharacterNameBtn" .. nCurShowBtn .. "LoverSignBtn" );
			LoveSignBtn:Hide()
			if t_LabelNameList[i].nSubType == 0 then
				rich:SetText( t_LabelNameList[i].name, 255, 255, 190 );
				rich:SetPoint( "topleft", button:GetName(), "topleft", 0, 0 )
				button:SetClientUserData( 0, 0 );
				button:SetClientUserData( 1, 0 );
				button:SetClientString( "" );
				jbutton=getglobal( button:GetName() .."Tip");
				jbutton:Hide();
				rich:SetClientUserData( 0, 0 );
				rich:SetClientUserData( 1, 0 );
				rich:SetClientUserData( 2, i );
				rich:SetClientString( t_LabelNameList[i].name );
				chk:setCheckState( t_LabelNameList[i].bCheck );
				button:Show();
				chk:Show();
				nCurShowBtn = nCurShowBtn + 1;
			elseif t_LabelNameList[i].nSubType > 0 then
				local firstLevelNode = GetFriParentNodeInfo( t_LabelNameList[i] );
				if firstLevelNode ~= nil and firstLevelNode.bCheck then
					rich:SetText( t_LabelNameList[i].name, 160, 160, 90 );
					rich:SetPoint( "topleft", button:GetName(), "topleft", 12, 0 );
					local character;
					if t_LabelNameList[i].nNewcomer then
						character = {};
						character.Online = CLIENT_ONLINE;
					elseif	GameSchool:GetSelfSchoolJobType() == ROLE_SCHOOL_TYPE_STUDENT_TOLUA then
						character = GameSchool:GetSelfTeacher();
					else
						character = GameSchool:GetSelfStudent( t_LabelNameList[i].name )
					end
					FlowerBtn:Disable()
					if character.Online == CLIENT_ONLINE then
						rich:SetText( t_LabelNameList[i].name, 255, 255, 190 );
						FlowerBtn:Enable()
					elseif character.Online == MUD_ONLINE then
						rich:SetText( t_LabelNameList[i].name, 160, 160, 190 );
						jbutton=getglobal( button:GetName() .."Tip");
						jbutton:Show();
					else
						rich:SetText( t_LabelNameList[i].name, 160, 160, 160 );
						jbutton=getglobal( button:GetName() .."Tip");
						jbutton:Hide();
					end
					button:SetClientUserData( 0, t_LabelNameList[i].nLow );
					button:SetClientUserData( 1, t_LabelNameList[i].nHeight );
					button:SetClientString( t_LabelNameList[i].name );
					rich:SetClientUserData( 0, t_LabelNameList[i].nLow );
					rich:SetClientUserData( 1, t_LabelNameList[i].nHeight );
					rich:SetClientUserData( 2, i );
					rich:SetClientString( t_LabelNameList[i].name );
					if not t_LabelNameList[i].nNewcomer then
						BookBtn:Show()
					end
					if GameSchool:GetSelfSchoolJobType() == ROLE_SCHOOL_TYPE_STUDENT_TOLUA then
						if not t_LabelNameList[i].nNewcomer then
							FlowerBtn:Show()
						end
					end
					

					button:Show();
					chk:Hide();
					nCurShowBtn = nCurShowBtn + 1;
				end
			end
		end
	end
	for i = nCurShowBtn, MAX_ROLE_NAME_LINE do
		local button	= getglobal( "FriendShipFrame_CharacterNameBtn" .. nCurShowBtn );
		local rich	= getglobal( button:GetName() .. "CharacterName" );
		local chk	= getglobal( "FriendShipFrameTitleSignChk" .. nCurShowBtn );
		button:Hide();
		rich:SetText( "", 255, 254, 189 );
		chk:Hide();
	end
	local nMaxRelativeRole	= GameFriendManager:getRelationPlayerCount( CLIENT_RELATION_TYPE_FRIEND );
	FriendShipFrameFriendNumFont:SetText( nMaxRelativeRole .. "/" .. GameFriendManager:getMaxFriendNum() );
	if nMaxRelativeRole == 0 then
		FriendMutilSendFrame_SendBtn:Disable();
	else
		FriendMutilSendFrame_SendBtn:Enable();
	end
	ResetFriendHighLightTex();	
end
---------------------------------师徒信息面板
function DelSchoolOk( nHeight, nLow, szName )
	GameSchool:DelSchool( nHeight, nLow, szName )
	SchoolInfoFrame:Hide()
end
function SchoolInfoFrameDelSchoolBtn_OnClick()
	if MessageBoxFrame:IsShown() and MessageTitle:GetText() == "解除师徒关系" then return end
	local frame = getglobal( "SchoolInfoFrame" );
	local szName = frame:GetClientString()
	if frame:GetClientString() == "" then 
		ShowMidTips( "错误的玩家信息" );
		SchoolInfoFrame:Hide();
		return;
	end
	local schoolInfo;
	if GameSchool:GetSelfSchoolJobType() == ROLE_SCHOOL_TYPE_STUDENT_TOLUA then
		schoolInfo = GameSchool:GetSelfTeacher()
	else
		schoolInfo = GameSchool:GetSelfStudent( szName )		
	end
	if schoolInfo then
		MessageBox( "解除师徒关系", "系统提示单方解除师生关系会扣除您"..SCHOOL_DEL_MONEY_NUM_TOLUA.."绑银，是否确认解除师生关系?" )
		MessageBoxFrame:SetClientString( szName )
		MessageBoxFrame:SetClientUserData( 0, schoolInfo.RoleID.nHeight )
		MessageBoxFrame:SetClientUserData( 1, schoolInfo.RoleID.nLow )
	end
end
function SchoolInfoFrameInvTeamBtn_OnClick()
	if MessageBoxFrame:IsShown() and MessageTitle:GetText() == "解除师徒关系" then return end
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then return end
	local frame = getglobal( "SchoolInfoFrame" );
	if frame:GetClientString() == "" then 
		ShowMidTips( "错误的玩家信息" );
		SchoolInfoFrame:Hide();
		return;
	end
	if SchoolInfoFramePosBtnFont:GetText() == "当前不在线" then
		ShowMidTips( "该玩家当前不在线" );
		return;
	end
	TeamManager:Team_OP_Add( frame:GetClientString() );
end
function SchoolInfoFrameWatchBtn_OnClick()
	if MessageBoxFrame:IsShown() and MessageTitle:GetText() == "解除师徒关系" then return end
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then return end
	local frame = getglobal( "SchoolInfoFrame" );
	if frame:GetClientString() == "" then 
		ShowMidTips( "错误的玩家信息" );
		SchoolInfoFrame:Hide();
		return;
	end
	if SchoolInfoFramePosBtnFont:GetText() == "当前不在线" then
		ShowMidTips( "该玩家当前不在线" );
		return;
	end
	GameFriendManager:RequestWatchFriend( frame:GetClientString() );
end
function UpdateSchoolInfo( szName )
	local frame = getglobal( "SchoolInfoFrame" );	
	local schoolInfo;
	if GameSchool:GetSelfSchoolJobType() == ROLE_SCHOOL_TYPE_STUDENT_TOLUA then
		schoolInfo = GameSchool:GetSelfTeacher()
	else
		schoolInfo = GameSchool:GetSelfStudent( szName )		
	end
	if not schoolInfo then
		ShowMidTips( "无法找到该师徒信息" );
		SchoolInfoFrame:Hide();
		return;
	end

	local rich = getglobal( "SchoolInfoFrameNameBtnFont" );	
	rich:resizeRect( rich:GetTextExtentWidth( szName ),16 )
	rich:SetText( szName, 160, 160, 90 );	
	rich:SetClientUserData( 0, 1 );
	rich:ScrollFirst();
	rich:SetWidth( 100 );

	rich = getglobal( "SchoolInfoFrameBkupNameBtnFont" );
	local relation = GameSchool:GetSelfSchoolJobType() == ROLE_SCHOOL_TYPE_STUDENT_TOLUA and "导师"	or "学生"
	rich:resizeRect( rich:GetTextExtentWidth( relation ),16 )
	rich:SetText( relation, 160, 160, 90 );
	rich:SetClientUserData( 0, 1 );
	rich:ScrollFirst();
	rich:SetWidth( 100 );

	rich = getglobal( "SchoolInfoFrameClanBtnFont" );
	rich:resizeRect( rich:GetTextExtentWidth( "" ),16 )
	rich:SetText( "", 160, 160, 90 );
	rich:SetClientUserData( 0, 1 );
	rich:ScrollFirst();
	rich:SetWidth( 100 );
	SchoolInfoFrameClanBtn:SetClientString( "" );

	rich = getglobal( "SchoolInfoFrameEmoBtnFont" );
	rich:resizeRect( rich:GetTextExtentWidth( "" ),16 )
	rich:SetText( "", 160, 160, 90 );
	rich:SetClientUserData( 0, 1 );
	rich:ScrollFirst();
	rich:SetWidth( 226 );
	
	rich = getglobal( "SchoolInfoFrameClanBtnFont" );
	rich:resizeRect( rich:GetTextExtentWidth( schoolInfo.ClanName ),16 )
	rich:SetText( "", 160, 160, 90 );
	rich:SetClientUserData( 0, 1 );
	rich:ScrollFirst();
	rich:SetWidth( 226 );
	

	local tipsContent = {									
				["SchoolInfoFrameRaceBtnFont"]		= { [true] = t_raceList[schoolInfo.Career],			[false] = "" },
				["SchoolInfoFramePosBtnFont"]		= { [true] = "当前不在线",					[false] = "" },
				["SchoolInfoFrameMeiLiBtnFont"]		= { [true] = schoolInfo.MeiLi,					[false] = "" },
				["SchoolInfoFrameTeamBtnFont"]		= { [true] = "当前不在线",					[false] = "" },
				["SchoolInfoFrameCharacterPhotoBtnFont"]= { [true] = "Lv" .. schoolInfo.Level,			[false] = "" },
				};
	
	for name, content in pairs( tipsContent ) do
		local font = getglobal( name );
		font:SetText( content[true] );
	end	
	
	local MaxMapControl = MapShower:GetMaxMapContal();
	SchoolInfoFramePosBtnFont:SetText( MaxMapControl:getMapName( schoolInfo.Map ) or "未知" );
	SchoolInfoFrameTeamBtnFont:SetText( schoolInfo.TeamNum > 0 and "已组队" .. schoolInfo.TeamNum .. "人" or "未组队" )

	 --显示玩家头像逻辑
	SetPhoto( schoolInfo.Hair, schoolInfo.Face, "SchoolInfoFrameCharacterPhotoBtnHairPhoto", "SchoolInfoFrameCharacterPhotoBtnPhoto", schoolInfo.Gender, schoolInfo.Head,
				"SchoolInfoFrameCharacterPhotoBtnHairShadowTex", "SchoolInfoFrameCharacterPhotoBtnBodyTex" );
end
function OpenSchoolInfo( szName, nLowId, nHeightId )
	local frame = getglobal( "SchoolInfoFrame" );	
	frame:SetClientString( szName );
	if szName == "" then 
		ShowMidTips( "错误的玩家信息" );
		return;
	end
	frame:SetClientUserData( 0, nLowId );
	frame:SetClientUserData( 1, nHeightId );
	UpdateSchoolInfo( szName )
	SchoolInfoFrame:SetPoint( "center", "UIClient", "center", 0, 0 );
	SchoolInfoFrame:Show()
end
function SchoolInfoFrame_OnShow()
	UpdateSchoolInfo( SchoolInfoFrame:GetClientString() );
end
function SchoolInfoFrame_OnUpdate()
	local rich = getglobal( "SchoolInfoFrameNameBtnFont" );
	local width = rich:GetWidth()
	if rich:getLineWidth(0) > width then
		local sign = rich:GetClientUserData( 0 )
		local disx = rich:GetDispPosX() + 1 * sign
		rich:SetDispPosX( disx )
		if disx + 100 > rich:getLineWidth(0) + 10 then
			rich:SetClientUserData( 0,-1 )
		end
		if disx <= 0 then
			rich:SetClientUserData( 0,1 )
		end			
	end
	rich = getglobal( "SchoolInfoFrameClanBtnFont" );
	width = rich:GetWidth()
	if rich:getLineWidth(0) > width then
		local sign = rich:GetClientUserData( 0 )
		local disx = rich:GetDispPosX() + 1 * sign
		rich:SetDispPosX( disx )
		if disx + 100 > rich:getLineWidth(0) + 10 then
			rich:SetClientUserData( 0,-1 )
		end
		if disx <= 0 then
			rich:SetClientUserData( 0,1 )
		end			
	end
	rich = getglobal( "SchoolInfoFrameEmoBtnFont" );
	local width = rich:GetWidth()
	if rich:getLineWidth(0) > width then
		local sign = rich:GetClientUserData( 0 )
		local disx = rich:GetDispPosX() + 1 * sign
		rich:SetDispPosX( disx )
		if disx + 234 > rich:getLineWidth(0) + 10 then
			rich:SetClientUserData( 0,-1 )
		end
		if disx <= 0 then
			rich:SetClientUserData( 0,1 )
		end			
	end
	local frame = getglobal( "SchoolInfoFrame" );	
	UpdateSchoolInfo( frame:GetClientString() );
end
function SchoolInfoFrame_OnHide()
	if MessageBoxFrame:IsShown() and MessageTitle:GetText() == "解除师徒关系" then
		MessageBoxFrame:Hide()
	end
end
function SchoolInfoFrameTalkBtn_OnClick()
	if MessageBoxFrame:IsShown() and MessageTitle:GetText() == "解除师徒关系" then return end
	local frame = getglobal( "SchoolInfoFrame" );
	if frame:GetClientString() == "" then 
		ShowMidTips( "错误的玩家信息" );
		SchoolInfoFrame:Hide();
		return;
	end
	local roleName = frame:GetClientString();

	-- 若好友不在我新手消息队列里切不在线给个提示，但是不弹出对话框
	local roleInfo;
	if GameSchool:GetSelfSchoolJobType() == ROLE_SCHOOL_TYPE_STUDENT_TOLUA then
		roleInfo = GameSchool:GetSelfTeacher()
	else
		roleInfo = GameSchool:GetSelfStudent( roleName )		
	end
	if not ChatFrameManager:IsNewSpeaker( roleName ) and roleInfo.Online == CLIENT_OFFLINE then
		ShowMidTips( "该玩家目前不在线,不能发起新的对话" );
		return;
	end

	local szFrameName = "";

	-- 先检查是否有与被点击玩家对应的且正在显示聊天窗口,若有则什么也做就结束子程序
	if ChatFrameManager:GetSpeakerFrame( roleName ) ~= nil then
		return;
	end
	
	-- 若没有与被点击玩家对应的聊天窗口或聊天提示框
	ChatFrameManager:OpenChatFrame( roleName, CL_CHAT_WINDOW );
end
function SchoolInfoFrameSctalkBtn_OnClick()
	if MessageBoxFrame:IsShown() and MessageTitle:GetText() == "解除师徒关系" then return end
	local frame = getglobal( "SchoolInfoFrame" );
	if frame:GetClientString() == "" then 
		ShowMidTips( "错误的玩家信息" );
		SchoolInfoFrame:Hide();
		return;
	end
	SendPrivateMessage( frame:GetClientString() );
end

function SchoolInfoFrameTradeBtn_OnClick()
	if MessageBoxFrame:IsShown() and MessageTitle:GetText() == "解除师徒关系" then return end
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then return end
	local frame = getglobal( "SchoolInfoFrame" );
	if frame:GetClientString() == "" then 
		ShowMidTips( "错误的玩家信息" );
		SchoolInfoFrame:Hide();
		return;
	end

	local actor	= ActorMgr:findActorByName(  frame:GetClientString() );
	if actor == nil then 
		ShowMidTips( "对方下线或者不在视野范围内，申请交易失败" );
		return; 
	end

	GameExchange:inviteExchange( actor:getName() );
end
------------------------------------------------------------好友界面新增按钮，书，花，蛋，礼四个按钮处理-----------------------------------------
function FriendShipFrame_CharacterNameTemplateEggsBtn_OnClick()
	local button = getglobal( this:GetParent() )
	local name = button:GetClientString()
	if name == "" or name == nil then return end	
	OpenUseFlower( 1, name )
end

function FriendShipFrame_CharacterNameTemplateBookBtn_OnClick()
	local UVAnimationTex = getglobal( this:GetName().."UVAnimationTex" )
	UVAnimationTex:Hide()
	local buttonTex = getglobal( this:GetNormalTexture() )
	if buttonTex:IsGray() then return end
	local button = getglobal( this:GetParent() )
	local name = button:GetClientString()
	if name == "" or name == nil then return end	
	local jobType = GameSchool:GetSelfSchoolJobType()
	if jobType == ROLE_SCHOOL_TYPE_STUDENT_TOLUA then
		local teacherInfo = GameSchool:GetSelfTeacher()
		if teacherInfo == nil then return end
		OpenStudentGained( jobType, teacherInfo.RoleID.nHeight, teacherInfo.RoleID.nLow, name )
	elseif jobType == ROLE_SCHOOL_TYPE_TEACHER_TOLUA then
		local studentInfo = GameSchool:GetSelfStudent( name )
		if studentInfo == nil then return end
		OpenTeacherGained( jobType, studentInfo.RoleID.nHeight, studentInfo.RoleID.nLow, name  )
	end
end
function FriendShipFrame_CharacterNameTemplateBookBtn_OnEnter()
	local jobType = GameSchool:GetSelfSchoolJobType()
	if jobType == ROLE_SCHOOL_TYPE_STUDENT_TOLUA then return end
	local button = getglobal( this:GetParent() )
	local buttonTex = getglobal( this:GetNormalTexture() )
	if buttonTex:IsGray() then
		SetGameTooltips( this:GetName(),"您每天只能给学生写一次导师心得" )
	else
		SetGameTooltips( this:GetName(),"您今天还可以为您的学生写一次导师心得" )
	end
end
function FriendShipFrame_CharacterNameTemplateBookBtn_OnShow()
	local button = getglobal( this:GetParent() )
	local name = button:GetClientString()
	if name == "" or name == nil then return end	
	local jobType = GameSchool:GetSelfSchoolJobType()
	if jobType == ROLE_SCHOOL_TYPE_TEACHER_TOLUA then
		local studentInfo = GameSchool:GetSelfStudent( name )
		if studentInfo == nil then return end
		local now = os.date( "*t", getServerTime().sec )
		local infoTime = os.date( "*t", studentInfo.DayTime )
		if now.year == infoTime.year and now.month == infoTime.month and now.day == infoTime.day then
			this:SetGrayEx( true )
		else
			this:SetGrayEx( false )
		end
		local UVAnimationTex = getglobal( this:GetName().."UVAnimationTex" )
		UVAnimationTex:Hide()
	else
		local canGetExp = GameSchool:GetSelfCanAwardTeacherGainedExp()
		local UVAnimationTex = getglobal( this:GetName().."UVAnimationTex" )
		if canGetExp > 0 and not UVAnimationTex:IsShown() then
			UVAnimationTex:Show()
			UVAnimationTex:SetUVAnimation( 48, true )
		elseif canGetExp <= 0 then			
			UVAnimationTex:Hide()
		end
		this:SetGrayEx( false )
	end
end
function FriendShipFrame_CharacterNameTemplateBookBtn_OnUpdate()
	local button = getglobal( this:GetParent() )
	local name = button:GetClientString()
	if name == "" or name == nil then return end	
	local jobType = GameSchool:GetSelfSchoolJobType()
	if jobType == ROLE_SCHOOL_TYPE_TEACHER_TOLUA then
		local studentInfo = GameSchool:GetSelfStudent( name )
		if studentInfo == nil then return end
		local now = os.date( "*t", getServerTime().sec )
		local infoTime = os.date( "*t", studentInfo.DayTime )
		--不仅要求同一天，而且要求是在当天的六点后，因为服务器是六点才更新消息的
		if now.year == infoTime.year and now.month == infoTime.month and now.day == infoTime.day and now.hour >= 6 then
			this:SetGrayEx( true )
		else
			this:SetGrayEx( false )
		end
	else		
		this:SetGrayEx( false )
	end
end

local MIN_SCHOOL_GIFT_GROW_LEVE = 10		--获得师徒成长奖励，徒弟的最小成长等级
local t_SchoolGiftLevelList = { 45,50,54,56,58,60,63,65,70, }
function FriendShipFrame_CharacterNameTemplateGiftBtn_OnEnter()
	local button = getglobal( this:GetParent() )
	local name = button:GetClientString()
	if name == "" or name == nil then return end	
	local character = GameSchool:GetSelfStudent( name )
	local szInfo = ""
	for i = 1, table.getn( t_SchoolGiftLevelList ) do
		local szColor = i==1 and "#c00ff00" or "\n#c00ff00"
		if character.JoinLevel > t_SchoolGiftLevelList[i] then
			szColor = i==1 and "#c808080" or "\n#c808080"
		end
		szInfo = szInfo..szColor..t_SchoolGiftLevelList[i].."级成长奖励"
	end
	SetGameTooltips( this:GetName(), szInfo )
end
function FriendShipFrame_CharacterNameTemplateFlowerBtn_OnClick()
	local button = getglobal( this:GetParent() )
	local name = button:GetClientString()
	if name == "" or name == nil then return end	
	OpenUseFlower( 0, name )
end
-----------------------------成功加入师徒后的引导
function SchoolAddStudent( szName, szTeacherName )
	local mainplayer = ActorMgr:getMainPlayer()
	local name = szName
	if mainplayer:getName() == szName then
		name = szTeacherName
	end
	if not FriendShipFrame:IsShown() then		
		GuideEffectShow( "", "", "FriendsButton", "您已经成功与"..name.."建立师生关系",
						"uires\\ZhuJieMian\\1.dds", 352, 609, 60, 60, 50, 60, -9, 5, 11, -51, true );
	end
	if not FriendShipFramePrenticeBtn:IsEnable() then return end
	FriendShipFramePrenticeBtnUVAnimationTex:Show()
	FriendShipFramePrenticeBtnUVAnimationTex:SetUVAnimation( 48, true )
end

-----------------------------------------结婚相关--------------------------------

function UpdateMarryInfoList()
	for index, data in ipairs( t_requestMarryInfo ) do
		local szInfo = ""
		if data.nType == PURSUE_INFO_TYPE then
			szInfo = data.name.."正在向你表白"
		elseif data.nType == REMOVE_RELATION_TYPE then
			szInfo = "玩家"..data.name.."希望和你解除伴侣关系"
		end
		if not RequestAddFriendFrame:IsShown() then
			GuideEffectShow( "", "", "FriendsButton", szInfo,
							"uires\\ZhuJieMian\\1.dds", 352, 609, 60, 60, 50, 60, -9, 5, 11, -51, true );
			return
		end
	end
end
function ShowMarryInfosFrame()
	local mainplayer = ActorMgr:getMainPlayer()
	g_szReqCharacterName	= t_requestMarryInfo[1].name;
		
	local szMarryInfoDesc = "";
	if t_requestMarryInfo[1].nType == PURSUE_INFO_TYPE then
		PursueFramePlayerName:SetText( g_szReqCharacterName..":" )
		szMarryInfoDesc = "\n"..mainplayer:getName()..",我喜欢你，我们#cff08ff在一起#n吧！";
		PursueFrame_DescRich:setCenterLine( true )
		PursueFrame_DescRich:SetText( szMarryInfoDesc, 255, 229, 189 )		
		PursueFrame:SetClientString( g_szReqCharacterName );
		PursueFrame:Show();
	elseif t_requestMarryInfo[1].nType == REMOVE_RELATION_TYPE then
		RemoveRelationFramePlayerName:SetText( g_szReqCharacterName..":" )
		szMarryInfoDesc = "\n"..mainplayer:getName()..",对不起，我想我们还是#R分开吧#n。\n#R（解除关系只需单方提出即可）#n";
		RemoveRelationFrame_DescRich:setCenterLine( true )
		RemoveRelationFrame_DescRich:SetText( szMarryInfoDesc, 255, 229, 189 )		
		RemoveRelationFrame:SetClientString( g_szReqCharacterName );
		RemoveRelationFrame:Show();
	end
	table.remove( t_requestMarryInfo, 1 )
end
function AddMarryInfo( szName, Type)
	--已有，则将最新的消息存下
	for i = 1, table.getn( t_requestMarryInfo ) do
		if t_requestMarryInfo[i].name == szName and t_requestMarryInfo[i].nType == Type then
			return
		end
	end
	table.insert( t_requestMarryInfo, { name = szName, nType = Type, nRelationType = GameFriendManager:getFriendInfoByName(szName).m_nIntimateColorType, } )
end
--------表白相关
function PursueFrame_Accept_OnClick()
	local szName = PursueFrame:GetClientString()
	GameFriendManager:ActToPursue( szName, 1 )
	util.closeParent(this)
end
function PursueFrame_Refuse_OnClick()
	local szName = PursueFrame:GetClientString()
	GameFriendManager:ActToPursue( szName, 0 )
	util.closeParent(this)
end
function PursueFrame_OnLoad()
end
function PursueFrame_OnHide()
	PursueFrame:SetClientString( "" )
end
--------解除关系相关
function RemoveRelationFrame_Accept_OnClick()
	util.closeParent(this)
end
function RemoveRelationFrame_Refuse_OnClick()
	util.closeParent(this)
end
function RemoveRelationFrame_OnLoad()
end
function RemoveRelationFrame_OnHide()
	RemoveRelationFrame:SetClientString( "" )
end
------誓言二级确认面板
local t_WiddingName = { "裸婚套餐", "简约套餐", "豪华套餐" }
function OpenOathUIFrame( Gender, szName, szOrderName, widdingMeal )
	OathUIFrame_DescRich:setCenterLine( true )
	local szInfo =  "#G（"..szOrderName.."订制了"..t_WiddingName[widdingMeal].."）#n\n".."你是否愿意与面前的"..( Gender == 1 and "他" or "她" ).."结为夫妻\n无论贫贱富贵，无论疾苦健康。"
	OathUIFrame_DescRich:SetText( szInfo, 255,229,190 )
	OathUIFrame:SetClientString( szName )
	OathUIFrame:Show()
end
function OathUIFrame_Accept_OnClick()
	local szName = OathUIFrame:GetClientString()
	GameFriendManager:ActToMarryOath( szName, 1 )
	util.closeParent(this)
end
function OathUIFrame_Refuse_OnClick()
	local szName = OathUIFrame:GetClientString()
	GameFriendManager:ActToMarryOath( szName, 0 )
	util.closeParent(this)
end
function OathUIFrame_OnLoad()
	OathUIFrame_DescRich:setCenterLine( true )
end
function OathUIFrame_OnHide()
end
-------解除关系花费弹窗
function OpenRemoveRelationCostFrame( nRelationType )
	RemoveRelationCostFrame_DescRich:setCenterLine( true )
	local szName = "解除"..(nRelationType == INTIMATE_COLOR_LOVERS_TOLUA and "情侣" or "夫妻" ).."需要消耗1把#G“无情剪刀”#n\n#R（解除关系只需单方提出即可）#n"
	if nRelationType == INTIMATE_COLOR_LOVERS_TOLUA then
		szName = "#R确定后你们将结束刻骨铭心的感情！#n\n是否确定解除情侣关系？"
	end
	RemoveRelationCostFrame_DescRich:SetText( szName, 255,225,190 )
	RemoveRelationCostFrame:Show()
end
function RemoveRelationCostFrame_OK_OnClick()
	local szName = RemoveRelationFrame:GetClientString()
	GameFriendManager:ActToRemoveRelationCost( szName )
	util.closeParent(this)
end
function RemoveRelationCostFrame_Cancle_OnClick()
	util.closeParent(this)
end
function RemoveRelationCostFrame_OnLoad()
	RemoveRelationCostFrame_DescRich:setCenterLine(true)
end
function RemoveRelationCostFrame_OnHide()
end
