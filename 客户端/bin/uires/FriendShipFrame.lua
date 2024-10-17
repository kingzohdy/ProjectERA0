local MAX_RECORD_LINE_ONE_PAGE = 30;
MAX_CHAT_FRAME		= 6;
local MAX_RIGHT_FRAME_BTNS = 12;
local t_LabelNameList = {};
-----------------------ʦͽ����-------------------------------------------------------------
local t_SchoolGroupList = {}



										------------------------------
										--		�������		--
										------------------------------
-- ��Ϊ���촰�ں�������ʾ���ڹ�����
local ChatFrameManager	= {

							newMessageQueue		= {},
							historyMessageQueue = {},

							-- �������½������б�Ķ���
							lessenNameQueue = {},
							-- ����Ҫ�����������б�Ķ���
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

-- @brief:	��Ϊ����ˢ��������ʾ���
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

-- @brief:	��Ϊ���ڲ�ѯ���촰�ں�������ʾ���ڶ�Ӧ���������
-- @return: ������������
-- @param:	szFrameName ��Ҫ��ѯ�����촰�ڻ�������ʾ���ڵ�����
function ChatFrameManager:GetSpeakerName( szFrameName )
	-- ����������������
	return self.frameInfo[szFrameName].speakerName;
end

function ChatFrameManager:SetSpeakerName( szFrameName, szNewSpeakName )
	-- ����������������
	self.frameInfo[szFrameName].speakerName = szNewSpeakName;
end

-- ����������ֲ�ѯ��ǰΪ���򿪵Ķ�Ӧ���촰��
-- @return: ���ҵ���Ӧ�����촰�ڣ��򷵻����촰�ڵ����֣����򷵻�nil
-- ��ѯ
function ChatFrameManager:GetSpeakerFrame( szSpeakerName )
	for szFrameName, data in pairs( self.frameInfo ) do 
		if data.isShow and data.speakerName == szSpeakerName then
			return szFrameName;
		end
	end

	return nil;
end

-- ��Ϊ���ڼ��򿪵����촰���Ƿ�ﵽ����
-- ��ѯ
function ChatFrameManager:CanOpenNewChatFrame()
	for _, data in pairs( self.frameInfo ) do
		if not data.isShow then
			return true;
		end
	end
	
	return false;
end

-- ��ѯ��С�б�������Ϣ
function ChatFrameManager:GetLessenNameQueue()
	return self.lessenNameQueue;
end

-- ��ѯ��˸�б�������Ϣ
-- ��ѯ�Ƿ�������Ϣ
function ChatFrameManager:GetBlinkNameQueue()
	return self.blinkNameQueue;
end

-- ��ѯĳ�������Ƿ�������״̬
function ChatFrameManager:IsNameBlink( szSpeakerName )
	for _, szName in ipairs( self.blinkNameQueue ) do
		if szSpeakerName == szName then
			return true;
		end
	end

	return false;
end

-- @brief:	��Ϊ�����������촰�ں�������ʾ���ڶ�Ӧ���������
-- @return:	void
-- @param:	szFrameName ��Ҫ���õ����촰�ڻ�������ʾ���ڵ�����
-- @param:	szSpeakerName ��Ҫ���õ�������������
function ChatFrameManager:SetFrameInfo( szFrameName, szSpeakerName, bIsShow )
	-- ����������������
	self.frameInfo[szFrameName].speakerName = szSpeakerName;
	self.frameInfo[szFrameName].isShow		= bIsShow;
end

-- ��������: ��Ϊ���������ʾ���ڻ���˫������������������ֵ�ʱ����Ӧ
-- @brief:	����һ�����촰��
-- @return: void
-- @param:	szSpeakerName Ŀ��������������

--ACTION: Ϊĳ��ָ����Ҵ����촰��
function ChatFrameManager:OpenChatFrame( szSpeakerName, nChatType )
	-- �����촰�ڴ򿪵���Ŀ�Ѿ��ﵽ���ޣ������������ʾ
	if not self:CanOpenNewChatFrame() then
		ShowMidTips( "�򿪵����촰����Ŀ�Ѿ��ﵽ���ޣ������ٴ��µ����촰��" );
		return;
	end

	self:DelRoleNameRecord( szSpeakerName );
	-- ���µ����촰�ڵ�ʱ��ͬʱҪ����˸�����ֱ�ɲ���˸
	--�ں��ѻ���ʦͽ�б�
	if GameFriendManager:getFriendInfoByName( szSpeakerName ) ~= nil or GameSchool:IsSelfSchool( szSpeakerName ) then
		DisableBlinkName( szSpeakerName );
	end
	
	-- �����µ����촰��
	for i = 1, MAX_CHAT_FRAME do
		local szChatFrameName = "FriendChatFrame" .. i;
		-- ������һ��δʹ�õ����촰��
		if not self.frameInfo[szChatFrameName].isShow then
			local frame = getglobal( szChatFrameName );
			local font	= getglobal( szChatFrameName .. "_ChatDestName" );
			self:SetFrameInfo( szChatFrameName, szSpeakerName, true );
			font:SetText( "��	" .. szSpeakerName .. "�Ի���");
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

-- ��������: ��Ϊ�ر����촰�ڵ�ʱ����Ӧ
-- @brief:	�ر�һ�����촰��
-- @return: void
-- @param:	szFrameName ���رյ����촰�ڵ�����

--ACTION: �ر�ĳ��ָ�������촰��
function ChatFrameManager:ShutDownChatFrame( szFrameName )
	--ValidateLinkText( szFrameName );
	-- ������ֶ�����������촰�ڶ�Ӧ���������speaker��������Ϣ
	local szSpeakerName = self.frameInfo[szFrameName].speakerName;
	self.newMessageQueue[szSpeakerName] = nil;
	
	-- �ر�����ΪszFrameName�����촰��;
	local frame	= getglobal( szFrameName );
	-- ����szFrameName���촰�ڵĵײ�״̬��Ϊ0
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

-- ��������: �����С���촰�ڰ�ť��ʱ����Ӧ
-- @brief:	 ��һ�����촰����СΪ������ʾ����
-- @return:  void
-- @param:	 szFrameName ����С�����촰�ڵ�����

--ACTION: ��Сĳ��ָ�������촰��
function ChatFrameManager:LessenChatFrame( szChatFrameName )
	local szSpeakerName = self.frameInfo[szChatFrameName].speakerName;
	-- ��¼����С���ڵ������ֵ����ֶ�����
	self:RecordRoleName( szSpeakerName );
	assert( self:GetSpeakerFrame( szSpeakerName ) ~= nil );
	-- �ر�szChatFrameName���촰��
	local frame	= getglobal( szChatFrameName );
	table.insert( self.lessenNameQueue, szSpeakerName );
	-- ��ձ��رյ����촰�ڵĶ�Ӧ״̬
	self:SetFrameInfo( szChatFrameName, "", false );
	frame:Hide();
end

-- @brief:	��Ϊ���ڲ�ѯ��Ϣ��������ĳ����������������Ϣ
-- @return: ��Ϣ��¼��table
-- @param:	szSpeakerName ��Ҫ��ѯ��������������

-- ��ѯ
function ChatFrameManager:GetChatContent( szSpeakerName )
	return	( self.newMessageQueue[szSpeakerName] ~= nil ) and self.newMessageQueue[szSpeakerName].message or {};
end

-- @brief:	��Ϊ���ڲ�ѯĳ������Ƿ�����������Ϣ������
-- @return: true��ʾ��������Ϣ�
function ChatFrameManager:IsNewSpeaker( szSpeakerName )
	return	self.newMessageQueue[szSpeakerName] ~= nil;
end

-- @brief:	��Ϊ���ڲ�ѯ��Ϣ��������ĳ���������������¼��Ϣ
-- @return: ��Ϣ��¼��table
-- @param:	szSpeakerName ��Ҫ��ѯ��������������
function ChatFrameManager:GetHistroy( szSpeakerName )
	return	( self.historyMessageQueue[szSpeakerName] ~= nil ) and self.historyMessageQueue[szSpeakerName].message or {};
end

-- ��������: ���յ�һ������Ϣ��ʱ��,��Ϊ������Ϣ�����ж�Ӧ��������ʾ���ڻ����촰�ڵ�ʱ��
-- @brief:	���յ�������Ϣѹ����Ϣ������
-- @return: void
-- @param:	msg �յ�����Ϣ����
-- @param:	receiveTime �յ���Ϣ��ʱ��

--ACTION: ���ڽ������µ���Ϣ
function ChatFrameManager:ReceiveMessage( szChatContent, szSpeakerName )
	-- ���յ�������Ϣѹ����Ϣ������
	--[[ [szSpeakerName] = { message = { { receiveTime = ?, chatContent = ? }, ... } }, --]]

	-- �ر���: ����һ�����ĳ���˵���Ϣ���ݵ�ʱ��,����Lua�﷨�ϵĲ�����г�ʼ��
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

-- ��¼������������
-- �������յ�����Ϣ��ʱ�����ڸı����ֶ��е���Ϣ
function ChatFrameManager:RecordRoleName( szSpeakerName )
	-- ���������ж�Ӧ�򿪵����촰��
	-- ���Ѿ��ж�Ӧ�����촰����ʾ������������Ϣ�������轫�ö�������ѹ�����ֶ����� 
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

-- �����������ֵļ�¼
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

	-- ˢ��������ʾ
	self:RefreshChatClue();
end

-- ����������ռ�¼�õ�
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
----------ע��:���еġ�ʦ����,�����ޡ�,��ͽ�ܡ���MODEL�����ǲ����ڵ�,ֻ�ڿؼ��в�������Щǰ׺��ϵ
--Desc: ��������б�:CharacterList = { {��ɫlowID, ��ɫheightID, "��ɫ����"} },
------: ListNode = {��ɫlowID, ��ɫheightID, "��ɫ����"}
------: �ر��:ʦ���б�={ {��ɫlowID, ��ɫheightID, "��ʦ������ɫ����"}} ͽ���б�={ {��ɫlowID, ��ɫheightID, "��ͽ�ܡ���ɫ����"}}
--------------------:�����б� = { {��ɫheightID, ��ɫlowID,"���Ϲ�����ɫ����"}, {��ɫheightID, ��ɫlowID,"�����š���ɫ����"}}

--MODEL3
--@Desc: ��Ϊ��ǰ����������Ϣ ��ӦC++�е�tagFriendInfo
local g_CurClickRole = { roleLowId = 0, roleHeightId = 0, roleName = "" };

--��������б���ʾ����
local MAX_ROLE_NAME_LINE = 19;

-------------------�������--------------------------------------------
local g_friendFrameButtons = { buttonName = { "FriendShipFrameFriendBtn", "FriendShipFrameTempBtn", "FriendShipFrameBrotherBtn",							"FriendShipFramePrenticeBtn",
								"FriendShipFrameKillBtn", "FriendShipFrameEnemyBtn", "FriendShipFrameBlacklistBtn" } };

--@Desc:��Ϊ����������ܹ���ʾ�������¼���������
local MAX_CHAT_LINE = 20;

--------------------��ӽ�ɫ���ֿ�ʹ�õ���ȫ���ļ�����-----------------------------
local g_nAddType = 0;

--------------------������ѿ�ʹ�õ��ļ�ȫ�ֱ���------------------------------------
local g_szReqCharacterName = "";--��Ϊ�����˵�����

---------------------������ʾ��ʹ�õ��ļ�ȫ�ֱ���-----------------------------------

--ָʾ��ǰ����ʾ���б�����
local g_nDisplayPageType = CLIENT_RELATION_TYPE_FRIEND;--Ĭ����ʾ����ҳ��

-- ��Ϊ��¶���ⲿ�����ڹر����촰�ڵĽӿ�
function CloseChatFrame( szFrameName )
	ChatFrameManager:ShutDownChatFrame( szFrameName );
end

--@Desc:���ڸ���������ֵ���ɫ
--@return: Nothing
function RoleOnLine( )
	-- �յ���������Ϣ,��ˢ���б�,��Ϊ���ֺ���ʱ�б�ᷢ�����������ı仯
	if g_nDisplayPageType == CLIENT_RELATION_TYPE_FRIEND then
		SetScrollBar( "FriendShipFrame_ScrollBar", MAX_ROLE_NAME_LINE, getTotalBtn() );
		local slider = getglobal( "FriendShipFrame_ScrollBar" );
		UpdateFriendNameList( slider:GetValue() + 1 );
		if FriendInfoFrame:IsShown() then
			local frame = getglobal( "FriendInfoFrame" );
			if frame:GetClientString() == "" then 
				ShowMidTips( "����������Ϣ" );
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

--@brief:	��Ϊ�����������и������ķ���
--@return:	Nothing
local function HideAllHighLightTex()
	local tex;

	for i = 1, MAX_ROLE_NAME_LINE do
		tex = getglobal( "FriendShipFrame_CharacterNameBtn" .. i .. "SelectHighLightTex" );
		tex:Hide();
	end
end

--@Desc:�����������б�ǰ��ʾ����������
--return: Nothing
function ClearRoleNameList()
	local rich;
	local button;
	local font;
	local chk;

	for i = 1, MAX_ROLE_NAME_LINE do
		rich = getglobal( "FriendShipFrame_CharacterNameBtn" .. i .. "CharacterName" );
		rich:SetClientUserData( 0, 0 );--��յ�λ��Ϣ
		rich:SetClientUserData( 1, 0 );--��ո�λ��Ϣ
		rich:SetClientString( "" );
		--rich:SetText( "" );
		rich:Clear( );
		
		font = getglobal( "FriendShipFrame_CharacterNameBtn" .. i .. "LeaveTimeFont" );
		font:SetText( "" );

		button = getglobal( "FriendShipFrame_CharacterNameBtn" .. i );
		button:SetClientUserData( 0, 0 );--��յ�λ��Ϣ
		button:SetClientUserData( 1, 0 );--��ո�λ��Ϣ
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

--@Desc:��Ϊ�����̶������������ָ������ķ���
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

		-- ���ڴ���ʾ���ֶ�Ӧ�Ŀؼ��л����ҵ�������Ϣ
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

			-- ���ڴ���ʾ���ֶ�Ӧ�Ŀؼ��л����ҵ�������Ϣ
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

		-- ���ڴ���ʾ���ֶ�Ӧ�Ŀؼ��л����ҵ�������Ϣ
		if g_CurClickRole.roleLowId == tmpRole.roleLowId and g_CurClickRole.roleHeightId == tmpRole.roleHeightId then
			tex = getglobal( "FriendShipFrame_CharacterNameBtn" .. i .. "SelectHighLightTex" );
			tex:Show();
			return;
		end
	end
end

--@Desc: �ڵ����Ӧ�Ի�����ʾ��ʱ��,���ֲ�����˸
function DisableBlinkName( szSpeakerName )
	--�ú����������˸�����ֲ�����˸
	local character		= GameFriendManager:getFriendInfoByName( szSpeakerName );
	--������ں����б�
	if character == nil then
		--����ʦͽ�б�
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
					if CLIENT_ONLINE == character.Online  then--���������,��ʾ��ɫ
						rich:SetText( szSpeakerName, 255, 254, 189 );
					else--����Ҳ�����,��ʾ��ɫ
						rich:SetText( szSpeakerName, 160, 160, 160 );
					end

					break;
				end
			end
			return
		end
	end
	assert( character ~= nil, "����˿����!" );
	local szPrefixName	= "";
	local rich;

	for i = 1, MAX_ROLE_NAME_LINE do
		rich = getglobal( "FriendShipFrame_CharacterNameBtn" .. i .. "CharacterName" );

		if szSpeakerName == rich:GetClientString() then
			--rich:SetText( "" );
			rich:Clear( );
			szPrefixName = GetRelatioDesc( character );
			--2��ʾ�һ�����
			if CLIENT_ONLINE == character.m_nIsOnline or MUD_ONLINE == character.m_nIsOnline then--���������,��ʾ��ɫ
				rich:SetText( szPrefixName .. szSpeakerName, 255, 254, 189 );
				--rich:SetTextColor( 255, 254, 189) ;
			else--����Ҳ�����,��ʾ��ɫ
				rich:SetText( szPrefixName .. szSpeakerName, 160, 160, 160 );
				--rich:SetTextColor( 160, 160, 160 ) ;
			end

			break;
		end
	end
end

--@Desc: ������������Ϣ��ʱ��,��������˸
function BlinkName( szSpeakerName )
	--�ú����������˸��������˸
	local character		= GameFriendManager:getFriendInfoByName( szSpeakerName );
	--������ں����б�
	if character == nil then
		--����ʦͽ�б�
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
					if CLIENT_ONLINE == character.Online  then--���������,��ʾ��ɫ
						rich:SetText( szSpeakerName, 255, 254, 189 );
					else--����Ҳ�����,��ʾ��ɫ
						rich:SetText( szSpeakerName, 160, 160, 160 );
					end

					break;
				end
			end
			return
		end
	end
	assert( character ~= nil, "����˿����!" );
	local rich;
	local szPreName = "";

	for i = 1, MAX_ROLE_NAME_LINE do
		rich = getglobal( "FriendShipFrame_CharacterNameBtn" .. i .. "CharacterName" );

		if szSpeakerName == rich:GetClientString() then
			--rich:SetText( "" );
			rich:Clear( );
			szPreName = GetRelatioDesc( character );
			--2��ʾ�һ�����
			if CLIENT_ONLINE == character.m_nIsOnline or MUD_ONLINE == character.m_nIsOnline then--���������,��ʾ��ɫ
				rich:SetText( "#b" .. szPreName ..  szSpeakerName, 255, 254, 189 );
			else--����Ҳ�����,��ʾ��ɫ
				rich:SetText( "#b" .. szPreName ..  szSpeakerName, 160, 160, 160 );
			end

			break;
		end
	end
end

--@Desc:���ù�������λ��
function SetFriendFrameScrollBar( )
	--ȷ������ѧϰ�����ʾ�ľ��弼��
	local slider		= getglobal( "FriendShipFrame_ScrollBar" );
	local nMaxViewLine	= MAX_ROLE_NAME_LINE;--��Ϊ�������������ʾ����
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


	-- ��ɾ�����б��е�һ����ϵ����Ժ�,Ҫ�ر����õ�,�Է�ֹ
	if slider:GetValue() > slider:GetMaxValue() then
		slider:SetValue( slider:GetMaxValue() );
	end
end

-- @brief:	���ڻ����ҵĹ�ϵǰ׺����
-- @return:	string ��������ǵĹ�ϵ����
-- @param:	character �����Ϣ
function GetRelatioDesc( character )
	local szPrefixName		= "";

	-- �������ǰ��Ĺ�ϵǰ׺����
	if CLIENT_RELATION_TYPE_MASTER == character.m_nRelationType then
		szPrefixName = "��ʦ����";
	elseif CLIENT_RELATION_TYPE_PRENTICE == character.m_nRelationType then
		szPrefixName = "��ͽ�ܡ�";
	elseif CLIENT_RELATION_TYPE_SPOUSE == character.m_nRelationType then
		if CLIENT_GENDER_MALE == character.m_nGender then
			szPrefixName = "���ɷ�";
		else
			szPrefixName = "�����ӡ�";
		end
	else
		szPrefixName = "";
	end

	return szPrefixName;
end

--�����
local t_GroupName = {
				[1]	= "����һ",
				[2]	= "�����",
				[3]	= "������",
				[4]	= "������",
				[5]	= "������",
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
					-- �������ǰ��Ĺ�ϵǰ׺����
					szPrefixName = GetRelatioDesc( friendInfo );

					-- ����Ƿ�Ҫ��˸�������
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

--@Desc:��������б���ʾ����
--@return: Nothing
function UpdateRoleNameList( nStartTableLine )
	--�����������б�ԭ�����������
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

	-- ������������
	GameFriendManager:sortFriendInfo( g_nDisplayPageType );
	local t_trackRoleControl	= GetTrackRoleControl();
	local nCurServeTime	= getServeSecondTime();
	for  i = 1, MAX_ROLE_NAME_LINE do
		-- ����Ҫ��ʾ�����������Ŀ����ʵ�������е������Ŀ,���˳�
		if nTableNum > nMaxRelativeRole then break end

		button		= getglobal( "FriendShipFrame_CharacterNameBtn" .. i );
		rich		= getglobal( button:GetName() .. "CharacterName" );
		character	= GameFriendManager:getFriendInfoByIndex( g_nDisplayPageType, nTableNum - 1 );
		assert( character ~= nil, "����˿����!" );

		-- �������ǰ��Ĺ�ϵǰ׺����
		szPrefixName = GetRelatioDesc( character );

		-- ����Ƿ�Ҫ��˸�������
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

		-- ��������߻������Ϊ�ҵ���ʱ����,�����������ʾΪ��ɫ
			--2��ʾ�һ�����
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
				font:SetText( "ʣ�ඨλʱ��"..leaveTime.."��" );
			else
				font:SetText( "ʣ�ඨλʱ��"..math.ceil( leaveTime / 60 ).."��" );
			end
		end

		-- ����������,���ڷ�����һ������
		nTableNum = nTableNum + 1;
	end

	ResetHighLightTex();--�̶�ԭ������ĸ�����

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
				font:SetText( "ʣ�ඨλʱ��"..leaveTime.."��" );
			else
				font:SetText( "ʣ�ඨλʱ��"..math.ceil( leaveTime / 60 ).."��" );
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
			ShowMidTips( "����������Ϣ" );
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
	--��鵱ǰ�򿪵�����ؼ����Ƿ����뱻ɾ��������صĿؼ�
	local chatFrame;
	local chatRecordFrame;
	local dwindleIcon;
	local chatDisplayRich;
	local chatRecordDisplayRich;
	
	local szFrameName = ChatFrameManager:GetSpeakerFrame( g_CurClickRole.roleName );
 
	-- ����Ƿ�������Ҷ�Ӧ�����촰��,������رն�Ӧ�����촰��
	if szFrameName ~= nil then
		ChatFrameManager:ShutDownChatFrame( szFrameName );
	end	
	
	-- �������������Ƿ��Ӧ���������
	ChatFrameManager:DelRoleNameRecord( g_CurClickRole.roleName );
	g_CurClickRole = { roleLowId = 0, roleHeightId = 0, roleName = "" };
end

--@Desc:ɾ��ĳ��ָ���������
function DeleteCharacterName( )
	-- ��û��ѡ���κ����, �����κβ���
	if ( g_CurClickRole.roleName == "" ) then
		ShowMidTips( "����ѡ��һ�����" );
		return;
	end

	-- ��Ϊ��ʱ����
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

--@Desc:���غ������ʱ��ע����Ϣ
function FriendShipFrame_OnLoad()
	this:RegisterEvent( "GE_FRIEND_GETLIST" );
	this:RegisterEvent( "GE_FRIEND_UPDATAEMOTION" );
	this:RegisterEvent( "GE_FRIEND_GETMSG" );--���ڽ�����������ҵ�������Ϣ
	this:RegisterEvent( "GE_FRIEND_ADDFRIENDINFO" );
	this:RegisterEvent( "GE_TEAM_LEAVETEAM" );
	this:RegisterEvent( "GE_INVITE_PK");
	this:RegisterEvent( "GE_UPDATE_FRIENDCHATFRAME_BY_CLIENT");
	this:RegisterEvent( "GE_ENTER_PLAYERLOGIN" );
	this:RegisterEvent( "GE_WINDOW_GETMSG" );--�ض���������
	this:RegisterEvent( "GE_UPDATE_WINDOWCHATFRAME_BY_CLIENT");
end

-- t_requestAddFriend = { { name, startTime } }
local t_requestAddFriend = {};
-----------------------------------------������--------------------------------
--���������ͣ�1��ף�2�����ϵ
PURSUE_INFO_TYPE = 1
REMOVE_RELATION_TYPE = 2
--����������������Щ�����ڴ˱���
-- t_requestMarryInfo = { { name, nType, nRelationType } }
local t_requestMarryInfo = {}

-- ���ָ������Ƿ��Ѿ��ں��������б���
function isRoleInRequestList( szRoleName )
	for _, data in ipairs( t_requestAddFriend ) do
		if data.name == szRoleName then
			ShowMidTips( data.name .. "�����㷢���˺�������" );
			return true;
		end
	end

	-- �ټ��ú����Ƿ��Ѿ��ں����б�
	if GameFriendManager:getPlayerRelation( szRoleName ) == CLIENT_RELATION_TYPE_FRIEND then
		ShowMidTips( szRoleName .. "�Ѿ�����ĺ����б���" );
		return true;
	end

	return false;
end

-- ���º���������Ϣ�б�
function UpdateAddFriendList()
	local nCurTime = GameMgr:getTickTime();
	for index, data in ipairs( t_requestAddFriend ) do
		if ( nCurTime - data.startTime ) >= 15000 then
			GameFriendManager:addFriendRes( data.name, 0, 1, true );
			ShowMidTips( "�������" .. data.name .. "����ĺ�������" );
			-- ����ǰ���ڵ���Ϣ����������ʾ�ĺ���������崦���У���ͨ���ر���壬�Ƴ������Ϣ
			if data.name == g_szReqCharacterName then
				RequestAddFriendFrame:Hide();
			else
				table.remove( t_requestAddFriend, index );
				
				if index == 1 then
					-- ���ǵ�һ�������ı��Ƴ���ֹͣ���ŵ��뵭����Ч
					util.stopFace( "EffectFrame_Texture" );
					EffectFrame:Hide();
					-- ���ǵ�һ�������ı��Ƴ����������û������״̬���������б�û�գ����ʾҪ���µ����µ������
					if table.getn( t_requestAddFriend ) > 0 and not RequestAddFriendFrame:IsShown() then
						GuideEffectShow( "", "", "FriendsButton", "���"..t_requestAddFriend[1].name.."����\n����Ϊ����",
										"uires\\ZhuJieMian\\1.dds", 352, 609, 60, 60, 50, 60, -9, 5, 11, -51, true );
					end
				end
			end
		end
	end
end

-- ������ĺ���
function FriendsButtonOnClick()
	-- �رն���������ʾ
	GuideEffectHide( "FriendsButton" );
	FriendsButton:SetGray(false);
	-- ����������Ӻ��ѣ���ر���ʾ��Ч���ҵ������������壬�����Ǻ���������
	if table.getn( t_requestAddFriend ) > 0 then
		g_szReqCharacterName	= t_requestAddFriend[1].name;
		
		local szAddFriendDesc = "\n#P"..g_szReqCharacterName .. "  �������Ϊ���ѣ�";
		if GameFriendManager:isPlayerRelation( CLIENT_RELATION_TYPE_BLACK, g_szReqCharacterName ) then
			szAddFriendDesc = g_szReqCharacterName .. "  �������Ϊ���ѣ����Է�����ĺ������У��Ƿ�ͬ�⣿";
		elseif GameFriendManager:isPlayerRelation( CLIENT_RELATION_TYPE_SU_DI, g_szReqCharacterName ) then
			szAddFriendDesc = g_szReqCharacterName .. "  �������Ϊ���ѣ����Է�������޵������У��Ƿ�ͬ�⣿";
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
		-- ������Ӧ�ĺ����������ʱ���������ĳ�ʼ��ʱ��
	-- �н�������Ϣ��������Ӧ�����
	elseif table.getn( t_requestMarryInfo ) > 0 then
		ShowMarryInfosFrame();
	-- ��û��������Ӻ��ѣ��򵯳����������棬��һ����û�н���Ӧ��Ϣ
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
-- �յ������Լ������б����Ϣ
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

-- �յ������Լ�������ǩ������Ϣ
t_events["GE_FRIEND_UPDATAEMOTION"].func = 
function()
	if GameFriendManager:getEmotion() ~= "" then
		FriendShipFrame_EmotionEdit:SetText( GameFriendManager:getEmotion() );
		FriendShipFrame_EmotionShowFont:SetText( GameFriendManager:getEmotion() );
	else
		FriendShipFrame_EmotionEdit:SetText( "����������ǩ��" );
		FriendShipFrame_EmotionShowFont:SetText( "����������ǩ��" );
	end
end

-- �յ������������Ϊ���ѵ���Ϣ
t_events["GE_FRIEND_ADDFRIENDINFO"].func = 
function()
	local UImsg				= UIMSG:getUIMsg();
	local addFriendInfo		= UImsg.AddFriendInfo;
	
	if ShowChangeRoleFrame:IsShown() then
		GameFriendManager:addFriendRes( addFriendInfo.m_szReqName, 0, 1 );
		return;
	end

	-- ���ж���ǰ�����Ϊ���ѵ����Ƿ��Ѿ������ڵ��������ˣ����ǣ��򲻽��ո��˵��ظ�������Ϣ
	if g_szReqCharacterName == addFriendInfo.m_szReqName then
		return;
	end

	-- �������б������Ѿ��������Ҳ������
	for _, data in ipairs( t_requestAddFriend ) do
		if data.name == addFriendInfo.m_szReqName then
			return;
		end
	end
	
	-- ����������˵���Ϣ�洢��������
	table.insert( t_requestAddFriend, { name = addFriendInfo.m_szReqName, startTime = GameMgr:getTickTime() } );
	if not CanBeginShortcutAction() then
		return;
	end

	if table.getn( t_requestAddFriend ) == 1 then
		GuideEffectShow( "", "", "FriendsButton", "���"..t_requestAddFriend[1].name.."����\n����Ϊ����",
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
		GuideEffectShow( "", "", "FriendsButton", "���"..t_requestAddFriend[1].name.."����\n����Ϊ����",
							"uires\\ZhuJieMian\\1.dds", 352, 609, 60, 60, 50, 60, -9, 5, 11, -51, true );
	end	
end

--�յ�����������Ϣ
t_events["GE_WINDOW_GETMSG"].func =
function()
	local msg		= UIMSG:getUIMsg();
	local nType		= msg.ChatMsg.Type;
	local szSpeakerName	= msg.ChatMsg.Speaker;
	local szChatContent	= msg.ChatMsg.Content;
	local szFrameName	= "";
	ChatFrameManager:RecordRoleName( szSpeakerName );
	-- ������Ϣ����
	local nTime = os.date();
	ChatFrameManager:ReceiveMessage( "#c23f032"..szSpeakerName.." ˵��    ��"..nTime.."��#n",	szSpeakerName );
	if msg.ChatMsg.IsCluster == 1 then
		ChatFrameManager:ReceiveMessage( "#cf0f000������Ⱥ����Ϣ����#cfffebd"..szChatContent.."#n",		szSpeakerName );
	else
		ChatFrameManager:ReceiveMessage( "#cfffebd"..szChatContent.."#n",					szSpeakerName );
	end
	-- �����յ�����Ϣ�ķ�����û���뵱ǰ�򿪵����촰���й�ϵ��
	if ChatFrameManager:GetSpeakerFrame( szSpeakerName ) == nil then
		--�ں��ѻ���ʦͽ�б�
		if GameFriendManager:getFriendInfoByName( szSpeakerName ) ~= nil or GameSchool:IsSelfSchool( szSpeakerName ) then
			-- �ö�Ӧ����������
			BlinkName( szSpeakerName );
		end
		-- �������������б��Ҳû��ʾ���� ��ʾ��ʾ��
		if FriendFrame_ChatListFrame:IsShown() then
			UpdateChatListFrame();
		else
			FriendFrame_ChatClueFrame:Show();
			FriendFrame_ChatClueFrame:SetClientUserData( 0, nType )
		end
		return;
	end
	
	-- ������Ϣ�����뵱ǰ�Ѵ򿪵�ĳһ�����촰��A��Ӧ
	szFrameName		= ChatFrameManager:GetSpeakerFrame( szSpeakerName );
	local chatFrame	= getglobal( szFrameName );
	local rich		= getglobal( szFrameName .. "_DisplayRich" );
	local slider	= getglobal( szFrameName.."_ScrollBar" );
	rich:AddText( "#c23f032"..szSpeakerName .. " ˵��    ��"..nTime.."��#n", 254, 242, 216 );
	if msg.ChatMsg.IsCluster == 1 then
		rich:AddText( "#cf0f000������Ⱥ����Ϣ����#cfffebd"..szChatContent.."#n",254, 242, 216 );		
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
	

-- �յ����ѷ��͵���Ϣ
t_events["GE_FRIEND_GETMSG"].func = 
function()
	local msg		= UIMSG:getUIMsg();
	local nType		= msg.ChatMsg.Type;
	local szSpeakerName	= msg.ChatMsg.Speaker;
	local szChatContent	= msg.ChatMsg.Content;
	local character		= GameFriendManager:getFriendInfoByName( szSpeakerName );
	local szFrameName	= "";
	
	--[[
	-- ������Ϣ���˸���û��ϵ�������Ϊ��ʱ����
	if character == nil then
		GameFriendManager:addTmpFriend( szSpeakerName );
	end
	--]]

	-- ������Ϣ���˴����ҵĺ������е��ˣ� �򲻽�����Ϣ,ʲôҲ����
	if character ~= nil and character.m_nRelationType == CLIENT_RELATION_TYPE_BLACK then
		return;
	end
	
	ChatFrameManager:RecordRoleName( szSpeakerName );
	-- ������Ϣ����
	local nTime = os.date();
	ChatFrameManager:ReceiveMessage( "#c23f032"..szSpeakerName.." ˵��    ��"..nTime.."��#n",	szSpeakerName );
	if msg.ChatMsg.IsCluster == 1 then
		ChatFrameManager:ReceiveMessage( "#cf0f000������Ⱥ����Ϣ����#cfffebd"..szChatContent.."#n",		szSpeakerName );
	else
		ChatFrameManager:ReceiveMessage( "#cfffebd"..szChatContent.."#n",					szSpeakerName );
	end
	-- �����յ�����Ϣ�ķ�����û���뵱ǰ�򿪵����촰���й�ϵ��
	if ChatFrameManager:GetSpeakerFrame( szSpeakerName ) == nil then
		-- �ö�Ӧ����������
		BlinkName( szSpeakerName );
		-- �������������б��Ҳû��ʾ���� ��ʾ��ʾ��
		if FriendFrame_ChatListFrame:IsShown() then
			UpdateChatListFrame();
		else
			FriendFrame_ChatClueFrame:Show();
		end
		return;
	end

	-- ����Ϣ�����������б������ʾ״̬����ˢ�������б�
	
	-- ������Ϣ�����뵱ǰ�Ѵ򿪵�ĳһ�����촰��A��Ӧ
	szFrameName		= ChatFrameManager:GetSpeakerFrame( szSpeakerName );
	local chatFrame	= getglobal( szFrameName );
	local rich		= getglobal( szFrameName .. "_DisplayRich" );
	local slider	= getglobal( szFrameName.."_ScrollBar" );
	rich:AddText( "#c23f032"..szSpeakerName .. " ˵��    ��"..nTime.."��#n", 254, 242, 216 );
	if msg.ChatMsg.IsCluster == 1 then
		rich:AddText( "#cf0f000������Ⱥ����Ϣ����#cfffebd"..szChatContent.."#n",254, 242, 216 );		
	else
		rich:AddText( "#cfffebd"..szChatContent.."#n",				254, 242, 216 );
	end

	rich:ScrollEnd();
	local nRichAccurateViewLines	= rich:GetAccurateViewLines();
	local nTextLines				= rich:GetTextLines();
	SetScrollBar( szFrameName.."_ScrollBar", nRichAccurateViewLines, nTextLines );
	slider:SetValue( slider:GetMaxValue() );
	util.UpdateRich( rich:GetName(), slider:GetValue() );

	-- ��������¼���Ƿ���ʾ,��������ʾ״̬,��������ϢҲ���µ�������Ϣ����
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
		-- �ٵ���richtext����ʾ����
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

	-- ��������ʾ����Ϣ
	if "" == szChatContent then return end

	-- ���ҷ��͵���Ϣѹ����Ϣ������
	local nTime = os.date();
	ChatFrameManager:ReceiveMessage( "#c00faf0�� ˵��    ��"..nTime.."��#n",	szSpeakerName );
	ChatFrameManager:ReceiveMessage( "#cfffebd"..szChatContent.."#n",			szSpeakerName );

	-- �Ƚ����͵���Ϣ�����ҵ��������������¼��
	local displayRich	= getglobal( szFrameName .. "_DisplayRich" );
	local slider		= getglobal( szFrameName .. "_ScrollBar" );
	displayRich:AddText( "#c00faf0�� ˵��    ��"..nTime.."��#n".."#n",	255, 229, 189 );

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

	-- ��������ʾ����Ϣ
	if "" == szChatContent then return end

	-- ���ҷ��͵���Ϣѹ����Ϣ������
	local nTime = os.date();
	ChatFrameManager:ReceiveMessage( "#c00faf0�� ˵��    ��"..nTime.."��#n",	szSpeakerName );
	ChatFrameManager:ReceiveMessage( "#cfffebd"..szChatContent.."#n",			szSpeakerName );

	-- �Ƚ����͵���Ϣ�����ҵ��������������¼��
	local displayRich	= getglobal( szFrameName .. "_DisplayRich" );
	local slider		= getglobal( szFrameName .. "_ScrollBar" );
	displayRich:AddText( "#c00faf0�� ˵��    ��"..nTime.."��#n".."#n",	255, 229, 189 );

	displayRich:AddText( "#cfffebd"..szChatContent.."#n",				255, 229, 189 );
	
	displayRich:ScrollEnd();
	local nRichAccurateViewLines	= displayRich:GetAccurateViewLines();
	local nTextLines				= displayRich:GetTextLines();
	SetScrollBar( slider:GetName(), displayRich:GetAccurateViewLines(), displayRich:GetTextLines() );
	slider:SetValue( slider:GetMaxValue() );
	util.UpdateRich( szFrameName.."_DisplayRich", slider:GetValue() );
end

--@Desc:�յ���Ϣˢ�����
function FriendShipFrame_OnEvent()
	if t_events[arg1] == nil then
		return;
	end

	t_events[arg1].func();
end

function FriendShipFrame_OnUpdate()
	local mainplayer = ActorMgr:getMainPlayer()
	FriendShipFrameSelfMeiLiHeadFont:SetText( "����ֵ��"..mainplayer:getCharm() )
end

--@Desc:��Ϊ��������������ʾ����
--@return: void
function FriendShipFrame_OnShow()		  
	-- ���ô�������
	--FriendShipFrame:SetPoint("topright", "UIClient", "topright", -91, 83 );
	-- ��������������6�����࿨����Ϊenable״̬,�������ѷ�ҳ����Ϊ����״̬
	ChangePageState( "FriendShipFrameFriendBtn" )
	
	--��ʾ����ֵ
	
	local mainplayer = ActorMgr:getMainPlayer()
	FriendShipFrameSelfMeiLiHeadFont:SetText( "����ֵ��"..mainplayer:getCharm() )
	
	FriendShipFrame_EmotionShowBtn:Show();
	FriendShipFrame_EmotionEdit:Hide();
	-- ���ú������Ĭ����ʾ�����б�
	g_nDisplayPageType = CLIENT_RELATION_TYPE_FRIEND
	
	LoginMgr:sendNewPeopleRequest();
	
	local slider = getglobal( "FriendShipFrame_ScrollBar" );
	slider:SetValue( 0 );
	SetScrollBar( "FriendShipFrame_ScrollBar", MAX_ROLE_NAME_LINE, getTotalBtn() );
	UpdateFriendNameList(1);

	-- �������ǵ�����ǩ��
	local editBox	= getglobal( "FriendShipFrame_EmotionEdit" );
	if GameFriendManager:getEmotion() ~= "" then
		editBox:SetText( GameFriendManager:getEmotion() );
		FriendShipFrame_EmotionShowFont:SetText( GameFriendManager:getEmotion() );
	else
		editBox:SetText( "����������ǩ��" );
		FriendShipFrame_EmotionShowFont:SetText( "����������ǩ��" );
	end

	-- ���ú������������Ҫ��ʾ�İ�ť
	
	local t_showIcons = { "FriendShipFrameAddFriendBtn", "FriendShipFrameMutilSendBtn", "FriendShipFrameFriendNumBtn" };
	util.ShowIcons( t_showIcons );
	
	local t_hideIcons = { "FriendShipFrameAddBlackBtn", "FriendShipFrameZhuiZongBtn", "FriendShipFrameDeleteBtn", };
	util.HideIcons( t_hideIcons );
end

function FriendShipFrame_OnHide()
	LoginMgr:clearNewPeopleInfo();
end

function FriendShipFrame_ModifyEmotion()
	-- ����ǰ�β����еĸ��������Ҽ�ѡ��
	HideAllHighLightTex();
	-- ��ԭ����һ�β�����ѡ�е���ұ�ȡ��ѡ��״̬
	g_CurClickRole = { roleLowId = 0, roleHeightId = 0, roleName = "" };

	local edit			= getglobal( "FriendShipFrame_EmotionEdit" );
	local szEditEmotion = edit:GetText();
	GameFriendManager:changeEmotion( szEditEmotion );
end

--@brief: ���ҳ���л���ť��ʱ�����һЩҳ��״̬����
--@return: void
function ChangePageState( szButtonName )
	--������ԭ������disabled״̬��button�ָ����õ�״̬
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
	-- �������еĸ�����
	HideAllHighLightTex();
end

-- ��������ǩ�������ʾ��Ϣ
local t_FenLeiLabelInfo =	{
								-- ����
								["FriendShipFrameFriendBtn"] =	{	relationType = CLIENT_RELATION_TYPE_FRIEND, 
																	showButtons = { "FriendShipFrameAddFriendBtn", "FriendShipFrameMutilSendBtn", "FriendShipFrameFriendNumBtn" },
																	hideButtons = { "FriendShipFrameAddBlackBtn", "FriendShipFrameZhuiZongBtn", "FriendShipFrameDeleteBtn" } 
																},
								-- ��ʱ
								["FriendShipFrameTempBtn"] =	{	relationType = CLIENT_RELATION_TYPE_TMPFRIEND, 
																	showButtons = { "FriendShipFrameDeleteBtn", "FriendShipFrameAddFriendBtn" },
																	hideButtons = { "FriendShipFrameAddBlackBtn", "FriendShipFrameZhuiZongBtn", "FriendShipFrameMutilSendBtn", "FriendShipFrameFriendNumBtn" } 
																},
								-- ���
								["FriendShipFrameBrotherBtn"] =	{	relationType = CLIENT_RELATION_TYPE_BROTHER, 
																	showButtons = { },
																	hideButtons = { "FriendShipFrameDeleteBtn", "FriendShipFrameAddFriendBtn",
																					"FriendShipFrameAddBlackBtn", "FriendShipFrameZhuiZongBtn", "FriendShipFrameMutilSendBtn", "FriendShipFrameFriendNumBtn" } 
																},
								-- ʦͽ
								["FriendShipFramePrenticeBtn"] ={	relationType = CLIENT_RELATION_TYPE_PRENTICE, 
																	showButtons = { },
																	hideButtons = { "FriendShipFrameDeleteBtn", "FriendShipFrameAddFriendBtn",
																					"FriendShipFrameAddBlackBtn", "FriendShipFrameZhuiZongBtn", "FriendShipFrameMutilSendBtn", "FriendShipFrameFriendNumBtn" } 
																},
								-- �޵�
								["FriendShipFrameKillBtn"]		=	{	relationType = CLIENT_RELATION_TYPE_SU_DI, 
																		showButtons = { "FriendShipFrameDeleteBtn", "FriendShipFrameZhuiZongBtn" },
																		hideButtons = {  "FriendShipFrameAddFriendBtn", "FriendShipFrameAddBlackBtn", "FriendShipFrameMutilSendBtn", "FriendShipFrameFriendNumBtn" } 
																	},
								-- ����
								["FriendShipFrameEnemyBtn"]		=	{	relationType = CLIENT_RELATION_TYPE_XIONG_SHOU, 
																		showButtons = { "FriendShipFrameDeleteBtn" },
																		hideButtons = {  "FriendShipFrameAddFriendBtn", "FriendShipFrameAddBlackBtn",
																						"FriendShipFrameZhuiZongBtn", "FriendShipFrameMutilSendBtn", "FriendShipFrameFriendNumBtn" } 
																	},
								-- ������
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
			ShowMidTips( "���ȹر��Ѿ��򿪵Ķ���ȷ�����" );
			return;
		end
	end

	-- ����ҳ������һ�β������ֵĿؼ���ʾ״̬
	ChangePageState( this:GetName() );
	-- ���õ�ǰ��ѡ���Ľ�ɫ��Ϣ
	g_CurClickRole = { roleLowId = 0, roleHeightId = 0, roleName = "" };
	-- ���õ�ǰѡ����ҳ��
	g_nDisplayPageType = t_FenLeiLabelInfo[this:GetName()].relationType;

	-- ����ҳ��Ĺ�������ʾ״̬��ˢ������ʾ�������Ϣ
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
	
	--����򿪵���ʦͽ��ǩҳ
	if g_nDisplayPageType == CLIENT_RELATION_TYPE_PRENTICE then
		FriendShipFramePrenticeBtnUVAnimationTex:Hide()
		UpdateSchoolRelationShip()
	end
end

function FriendShipFrameNotKaiFaBtn_OnEnter()
	local szText = "������δ���ţ������ڴ�";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

--@Desc:��Ϊ�����Ӻ��ѵ�ʱ����Ӧ
--@return: Nothing
function FriendShipFrame_AddFriend_OnClick()
	if FriendAddCharacterFrame:IsShown() then
		return;
	end 
	g_nAddType = CLIENT_RELATION_TYPE_FRIEND;
	
	FriendAddCharacterFrame:Show();
end

--@Desc:��Ϊ�����Ӻ�������ʱ����Ӧ
function FriendShipFrame_AddBlack_OnClick()
	if FriendAddCharacterFrame:IsShown() then
		return;
	end 

	g_nAddType = CLIENT_RELATION_TYPE_BLACK;

	FriendAddCharacterFrame:Show();
	FriendAddCharacterFrame_InputEdit:SetTextColor( 255, 229, 189 );
end

--@Desc:��Ϊ���ɾ����ʱ����Ӧ
--@return: Nothing
function FriendShipFrame_Delete_OnClick()
	DeleteCharacterName();
end

-- ׷���޵��б��е����
function FriendShipFrameZhuiZongBtn_OnClick()
	-- ��û��ѡ���κ����, �����κβ���
	if g_CurClickRole.roleName == "" or g_CurClickRole.roleName == nil then
		ShowMidTips( "��ѡ����Ҫ��λ���������" );
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
	-- ���������İ�ťû�洢�κ������Ϣ,��ʲôҲ���������ӳ���
	local parentFrame	= getglobal( this:GetParent() );
	-- TODO:	��Ҫ�ع�Ϊ�Զ���ʾ������
	local highLightTex = getglobal( parentFrame:GetName() .. "EnterHighLightTex" );
	highLightTex:Show();
end

function FriendShipFrame_CharacterNameBtn_OnLeave()
	local parentFrame	= getglobal( this:GetParent() );	
	-- TODO: �ع�Ϊ�Զ����ָ������������ֶ�����
	local highLightTex = getglobal( parentFrame:GetName() .. "EnterHighLightTex" );
	highLightTex:Hide();
end

function FriendShipFrame_CharacterNameBtn_OnDoubleClick()
	if g_nDisplayPageType == CLIENT_RELATION_TYPE_FRIEND then
		return;
	end
	local rich = getglobal( this:GetParent() .. "CharacterName" );
	if rich:GetClientString() == "" then return end
	-- ��������İ�ť�в��洢�κ������Ϣ
	if "" == rich:GetClientString( ) then
		return;
	end

	g_CurClickRole = { roleLowId = rich:GetClientUserData( 0 ), roleHeightId = rich:GetClientUserData( 1 ), roleName = rich:GetClientString() };
	ResetHighLightTex();

	if arg1 ~= "LDoubleClick" then
		return;
	end

	-- �����Ѳ�����������Ϣ�������в����߸�����ʾ�����ǲ������Ի���
	local roleInfo = GameFriendManager:getFriendInfoByName( g_CurClickRole.roleName );
	if not ChatFrameManager:IsNewSpeaker( g_CurClickRole.roleName ) and roleInfo.m_nIsOnline == CLIENT_OFFLINE then
		ShowMidTips( "�ú���Ŀǰ������,���ܷ����µĶԻ�" );
		return;
	end

	-- ������������е�����,�������Ի���
	if g_nDisplayPageType == CLIENT_RELATION_TYPE_BLACK then
		ShowMidTips( "������������е���ҶԻ�" );
		return;
	end

	local szFrameName = "";

	-- �ȼ���Ƿ����뱻�����Ҷ�Ӧ����������ʾ���촰��,������ʲôҲ���ͽ����ӳ���
	if ChatFrameManager:GetSpeakerFrame( g_CurClickRole.roleName ) ~= nil then
		return;
	end
	
	-- ��û���뱻�����Ҷ�Ӧ�����촰�ڻ�������ʾ��
	ChatFrameManager:OpenChatFrame( g_CurClickRole.roleName );
end

--@Desc:��Ϊ����꿿����ҹҵ��б�����ֵ�ʱ�򵯳������ϢTIPS
function TrustOrderListFrame_CharacterName_OnEnter()
	local tipsFrame		= getglobal( "FriendShipTipsFrame");
	local RoleNameFont	= getglobal(this:GetName().."RoleName")
	local character		= GameFriendManager:getFriendInfoByName( RoleNameFont:GetText() );
	if nil ~= RoleNameFont:GetText( ) and "" ~= RoleNameFont:GetText( ) then--���ؼ�û���κ������Ϣ,�򲻵���Tips
		util.AdjustTooltipsScreenPos( { tips = "FriendShipTipsFrame" } )
		tipsFrame:SetClientString( RoleNameFont:GetText() );
		tipsFrame:Show();
	end
end

--@Desc:��Ϊ������Ϲ�����ť��ʱ����Ӧ
function Fsf_ScrollUpBtn_OnClick()
	--�ø������̶�

	local slider = getglobal( "FriendShipFrame_ScrollBar" );
	if not slider:IsShown() then
		return;
	end

	local nValue = slider:GetValue() - slider:GetValueStep();
	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end

--@Desc:��Ϊ������¹�����ť��ʱ����Ӧ
function Fsf_ScrollDownBtn_OnClick()
	--�ø������̶�

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

--@Desc:��Ϊ������������ʱ����Ӧ
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
		--�̶�������
		ResetHighLightTex();
	end
end

--@Desc:��Ϊ����رհ�ť��ʱ����Ӧ
function Close_FriendShipFrame()
	--����ʹ���а�ť�ָ�����״̬
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
	local szInfo = string.format( "����������%d\n������ֵ��%d", mainplayer:getWeekCharm(), mainplayer:getCharm() )
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
--		����TIPS		--
-------------------------------------------------------
--@Desc:��Ϊ��������TIPSʱ�����ʾ����
function FriendShipTipsFrame_OnShow()
	--������ҵ������Ϣ
	local szCharacterName	= FriendShipTipsFrame:GetClientString( );
	local character			= GameFriendManager:getFriendInfoByName( g_nDisplayPageType, szCharacterName );

	local tipsContent = {
						["FriendFrame_CharacterName"]		= { [true] = character.m_szName,								[false] = character.m_szName },
						--["FriendFrame_CharacterMemName"]	= { [true] = "��ע������" .. character.m_szMemName,				[false] = "��ע������" },
						["FriendFrame_CharacterMemName"]	= { [true] = character.m_szMemName,				[false] = "" },
						["FriendFrame_CharacterLevel"]		= { [true] = "�ȼ���" .. character.m_nLevel,					[false] = "�ȼ���" },
						["FriendFrame_CharacterRace"]		= { [true] = "ְҵ��" .. ( t_raceList[character.m_nCareer] or ""),	[false] = "ְҵ��" },
						["FriendFrame_ClanName"]			= { [true] = "���᣺" .. character.m_szClanName,				[false] = "���᣺" },
						["FriendFrame_Friendliness"]		= { [true] = "�Ѻöȣ�" .. character.m_nBuddyVal,				[false] = "�Ѻöȣ�" },
						};
	
	for name, content in pairs( tipsContent ) do
		local font = getglobal( name );
		--font:SetText( content[g_nDisplayPageType == CLIENT_RELATION_TYPE_FRIEND] );
		font:SetText( content[true] );
	end
	
	FriendFrame_EmotionSign:SetText( "����ǩ����" .. character.m_szEmotion, 174, 221, 159 );

	 --��ʾ���ͷ���߼�
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
						["DiDuiPlayerTipsFrame_CharacterLevel"]		= "�ȼ���" .. requirePlayer.Level,
						["DiDuiPlayerTipsFrame_CharacterRace"]		= "ְҵ��" .. ( t_raceList[requirePlayer.Career] or ""),
						["DiDuiPlayerTipsFrame_ClanName"]			= "���᣺" .. requirePlayer.ClanName,
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
						["DiDuiPlayerTipsFrame_CharacterLevel"]		= "�ȼ���" .. character.m_nLevel,
						["DiDuiPlayerTipsFrame_CharacterRace"]		= "ְҵ��" .. ( t_raceList[character.m_nCareer] or ""),
						["DiDuiPlayerTipsFrame_ClanName"]			= "���᣺" .. character.m_szClanName,
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
--			������Ӻ��ѿ�		--
----------------------------------------------------------
--@Desc:��Ϊ����������ʾ�������������Ϊ������������
function RequestAddFriendFrame_OnShow()
	t_requestAddFriend[1].startTime = GameMgr:getTickTime();
end

function RequestAddFriendFrame_OnHide()
	-- ���ȴ������б�ɾ���������Ϣ
	table.remove( t_requestAddFriend, 1 );
	g_szReqCharacterName = "";
	if table.getn( t_requestAddFriend ) > 0  then
		g_szReqCharacterName = t_requestAddFriend[1].name;
		
		local szAddFriendDesc = "\n#P"..g_szReqCharacterName .. "  �������Ϊ���ѣ�";
		if GameFriendManager:isPlayerRelation( CLIENT_RELATION_TYPE_BLACK, g_szReqCharacterName ) then
			szAddFriendDesc = g_szReqCharacterName .. "  �������Ϊ���ѣ����Է�����ĺ������У��Ƿ�ͬ�⣿";
		elseif GameFriendManager:isPlayerRelation( CLIENT_RELATION_TYPE_SU_DI, g_szReqCharacterName ) then
			szAddFriendDesc = g_szReqCharacterName .. "  �������Ϊ���ѣ����Է�������޵������У��Ƿ�ͬ�⣿";
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

--@Desc:��Ϊ������ܰ�ťʱ����Ӧ
function FriendShipFrame_Accept_OnClick()
	local  szCharacterName = g_szReqCharacterName;
	local frame	= getglobal( "RequestAddFriendFrame" );

	-- ��Ϊͬ��ӶԷ�Ϊ����
	--if RequestCharacterFrameChkBtn:GetCheckState() then
	GameFriendManager:addFriendRes( szCharacterName, 1, 0 );
		-- ���������Ϊ���ѵ���������ʱ�б�,������Ϊ���Ѻ�,����ʱ�б���ɾ���ú���
	GameFriendManager:deleteRole( CLIENT_RELATION_TYPE_TMPFRIEND, szCharacterName );
	--else
	-- ��Ϊ��ͬ��ӶԷ�Ϊ����,����ͬ��Է�����
	--	GameFriendManager:addFriendRes( szCharacterName, 1, 0 );
	--end
	RequestAddFriendFrame_DescRich:Clear();
	frame:Hide();
end

--@Desc:��Ϊ����ܾ���ťʱ����Ӧ
function FriendShipFrame_Refuse_OnClick()
	local frame = getglobal( "RequestAddFriendFrame" );

	GameFriendManager:addFriendRes( frame:GetClientString(), 0, 0 );

	frame:SetClientString("");
	RequestAddFriendFrame_DescRich:Clear();
	frame:Hide();
end

-------------------------------------------------------
--		��ӽ�ɫ����		--
-------------------------------------------------------

--@Desc:��Ϊ��ʾ��ӽ�ɫ����ʱ����Ӧ
function FriendAddCharacterFrame_OnShow()
	SetFocusFrame("FriendAddCharacterFrame_InputEdit");
	FriendAddCharacterFrame_InputEdit:Clear();
	FriendAddCharacterFrame_InputEdit:SetTextColor( 255, 229, 189 );
end

--@Desc:��Ϊ������ܰ�ť��ʱ����Ӧ
function FriendAddCharacterFrame_Accept_OnClick()
	local szNewCharacterName = FriendAddCharacterFrame_InputEdit:GetText();
	if "" == szNewCharacterName then return end--��ʲô����Ҳû�䣬��ʲôҲ����

	if CLIENT_RELATION_TYPE_FRIEND == g_nAddType then--�����������Ӻ���
		if not isRoleInRequestList( szNewCharacterName ) then
			GameFriendManager:addFriend( szNewCharacterName, "" );
		end
		
	else--���������Ӻ�����
		GameFriendManager:addBlack( szNewCharacterName );
	end

	local frame = getglobal( "FriendAddCharacterFrame" );
	frame:Hide();
	FriendAddCharacterFrame_InputEdit:Clear();
	ClearFocus();
end

--@Desc:��Ϊ���ȡ����ť��ʱ����Ӧ
function FriendAddCharacterFrame_Cancel_OnClick()
	local frame = getglobal( "FriendAddCharacterFrame" );
	frame:Hide();
	FriendAddCharacterFrame_InputEdit:Clear();
end

function FriendAddCharacterFrame_EditBackBtn_OnClick()
	SetFocusFrame( "FriendAddCharacterFrame_InputEdit" );
end

-------------------------------------------------------
--		��ӱ�ע���ƿ�		--
-------------------------------------------------------

--@Desc:��Ϊ��ʾ��ӱ�ע���ƿ�ʱ����Ӧ
function FriendMemFrame_OnShow()
end

--@Desc:��Ϊ�����±�ע����ʱ����Ӧ
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

--@Desc:��Ϊ�������±�ע����ʱ����Ӧ
function FriendMemFrame_Cancel_OnClick()
	FriendMemFrame:Hide();
	FriendMemFrame_InputEdit:Clear();
end

function FriendMemFrame_EditBackBtn_OnClick()
	SetFocusFrame( "FriendMemFrame_InputEdit" );
end

-------------------------------------------------------
--		�������		--
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
	font:SetText( "��	" .. changeNameData.szNewName .. "�Ի���");
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

--@Desc:��Ϊ��ʾ��������ʱ����Ӧ
function FriendChatFrame_OnShow()
	-- ���ԭ����������Ϣ
	local szFrameName		= this:GetName();
	local szSpeakerName		= ChatFrameManager:GetSpeakerName( this:GetName() );
	local chatDisplayRich	= getglobal( szFrameName .. "_DisplayRich" );
	local chatInputEdit		= getglobal( szFrameName .. "_InputEdit" );

	-- ����ʾ��ʱ��, ȷ�������û���κ���������
	chatInputEdit:Clear();
	chatDisplayRich:Clear();

	-- ���������¼��ʾ���������Ϣ�����еļ�¼�������Ƿ�Ҫ��ʾ����
	local chatContent = ChatFrameManager:GetChatContent( szSpeakerName );
	local num = 0;
	for i =1, table.getn( chatContent ) do
		chatDisplayRich:AddText( chatContent[i], 255, 229, 189 );
		num = num + 1;
	end

	-- ���ù������������¼��ʾ��������
	chatDisplayRich:ScrollEnd();
	local slider = getglobal( szFrameName .. "_ScrollBar" );
	SetScrollBar( szFrameName.."_ScrollBar", chatDisplayRich:GetAccurateViewLines(), chatDisplayRich:GetTextLines() );
	slider:SetValue( slider:GetMaxValue() );
	util.UpdateRich( szFrameName .. "_DisplayRich", slider:GetValue() );

	-- ����������������
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
		--����������
		local linkId		= tonumber(nId)%10;
		--������Count
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
		-- �ݹ������һ������
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

--@Desc:��Ϊ������Ͱ�ť��ʱ����Ӧ
function FriendChatFrame_Send_OnClick()
	local mainplayer	= ActorMgr:getMainPlayer();
	local szMyName		= mainplayer:getName();
	local szParentName	= this:GetParent();
	local inputEdit		= getglobal( szParentName.."_InputEdit" );
	local szChatContent = inputEdit:GetText();
	local szSpeakerName	= ChatFrameManager:GetSpeakerName( szParentName );
	local chatFrame		= getglobal( szParentName );

	-- �������Ϳ���Ϣ
	if "" == szChatContent then return end
	szChatContent = string.gsub( szChatContent, "#[L]", "**" );
	
	local t_ClientSelfChatContentParser = GetClientSelfChatContentParser();

	if t_ClientSelfChatContentParser:DoClientDefineInstruction( { ["text"] = szChatContent } ) then
		return;
	end
	szChatContent = t_ClientSelfChatContentParser:parseLinkTextForClient( { ["text"] = szChatContent } );

	-- ����Ϣ�����������	
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

	-- ���������е�����
	inputEdit:Clear();
	ClearSetLink();
end

function FriendChatFrame_Send_OnEnter()
	local szText = "��Ctrl+Enter��������Ϣ����Enter������";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function FriendChatFrame_DisplayRich_OnClick()
	local szCurSelectChatPlayerName = nil;
	if type(arg1) == "number" then return end --����Ǳ��������������
	if arg2 ~= "" then
		if tonumber(arg2) == nil then return end
		--����������
		local linkId = tonumber(arg2)%10;
		--������Count
		local linkCountId = (tonumber(arg2) - linkId)/10;
		--VK_LSHIFT 160,VK_RSHIFT 161
		if isKeyPressed(160) or isKeyPressed(161) then
			--������������ʾ�У�����Ʒ���ָ��Ƶ�������
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
						--װ����tips
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

		-- ��Ʒ������
		if linkId == ITEM_LINK then
			--��<>����Ʒ����ת��Ϊ����<>
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
				--װ����tips
				local item = ChatManager:getItem(armLink.m_arm);
				if item ~= nil then
					ShowItemTips(LUA_ITEMTIPS_TYPE_LINK,LUA_ITEMTIPS_ITEM,item);
				end
			end
		--λ����������
		elseif linkId == POSTION_LINK  then
			local postionLink = ChatManager:getPostionLink(linkCountId);
			local player = ActorMgr:getMainPlayer();

			--���mapId����ǰ��mapId����ͬʱ,����Ѱ·
			if ( player == nil ) then return end

			if postionLink.m_nMapId ~= 0 then
				local nId = postionLink.m_nMapId;
				local nX  = postionLink.m_nX;
				local nY  = postionLink.m_nY;
				MapShower:GetMaxMapContal():linkMoveto(nId, nX, nY );
				return;
			end
		--λ����������
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

			--���mapId����ǰ��mapId����ͬʱ,����Ѱ·
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

--@Desc:��Ϊ��������¼��ť��ʱ����Ӧ
function FriendChatFrame_RecordBtn_OnClick()
	local chatRecordFrame	= getglobal( this:GetParent().."_RecordFrame" );

	if chatRecordFrame:IsShown() then
		chatRecordFrame:Hide();
	else
		chatRecordFrame:Show();
	end
end

function FriendChatFrame_RecordBtn_OnEnter()
	local szText = "����ɲ鿴��ʷ�����¼���ü�¼�����Զ����";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function FriendChatFrame_RecordBtn_OnLeave()
	GameTooltip:Hide();
end


--@Desc:������鰴ťʱ����Ӧ
function FriendChatFrame_SmileFaceBtn_OnClick()
	local chatFrame		= getglobal( this:GetParent() );
	local chatFaceFrame	= getglobal( "FriendChatFaceFrame" );

	-- ��ԭ��������������壬��رգ�
	-- ��ԭ����ʾ��ʱ�����������һ����壬���ȹرպ��
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
	local szText = "ѡ�����";
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

--@Desc:��Ϊ������Ϲ�����ť��ʱ����Ӧ
function FriendChatFrame_ScrollUpBtn_OnClick()
	local displayRich	= getglobal( this:GetParent().."_DisplayRich" );
	local slider		= getglobal( this:GetParent().."_ScrollBar" );
	local nValue		= slider:GetValue() - slider:GetValueStep();

	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end

--@Desc:��Ϊ������¹�����ť��ʱ����Ӧ
function FriendChatFrame_ScrollDownBtn_OnClick()
	local displayRich	= getglobal( this:GetParent().."_DisplayRich" );
	local slider		= getglobal( this:GetParent().."_ScrollBar" );
	local nMaxValue		= slider:GetMaxValue();
	local nValue		= slider:GetValue() + slider:GetValueStep();

	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

--@Desc:��Ϊ������������ʱ����Ӧ
function FriendChatFrame_ScrollBar_OnValueChanged()
	local slider	 = getglobal( this:GetParent().."_ScrollBar" );
	local nValue	 = slider:GetValue();
	local nMaxValue	 = slider:GetMaxValue();

	if nValue <= nMaxValue then
		util.UpdateRich( this:GetParent().."_DisplayRich", nValue );
	end
end

--@Desc:��Ϊ����رհ�ť��ʱ����Ӧ
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

--@Desc:��Ϊ�����С��ť��ʱ����Ӧ
function Dwindle_FriendChatFrame()
	ChatFrameManager:LessenChatFrame( this:GetParent() );
	if not FriendFrame_ChatClueFrame:IsShown() then
		FriendFrame_ChatClueFrame:Show();
	end
end

---------------------Ц�����---------------------------
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
	-- �����Ц������е�����һ��Ц����,�ر�Ц�����
	local editFrame = getglobal( FriendChatFaceFrame:GetClientString() );

	-- ��ȡ�����������
	local inputEdit = getglobal( editFrame:GetName() .. "_InputEdit");
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
	FriendChatFaceFrame:Hide();
end

function FCF_ShowFaceRich_OnEnter()
	if arg2 ~= "RTOT_FACE_ONENTER" then
		return;
	end

	if arg1 < 1 then 
		return;
	end

	--�������
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
--		�����¼		--
-------------------------------------------------------
--------------------ChatRecordFrame-----------------------------

-- @brief: ���ڸ��������¼����������¼��Ϣ
function UpdateChatRecordRich( szRichName, szSliderName )
	local rich		= getglobal( szRichName );
	local slider	= getglobal( szSliderName );
	rich:ScrollFirst();

	for i = 1, slider:GetValue() + MAX_RECORD_LINE_ONE_PAGE * ( rich:GetClientUserData( 1 ) - 1 )  do
		rich:ScrollDown();
	end
end

--@Desc:���ڻ��ҳ������ʾ����
--@Param:��ǰ�����¼��ʾ�����ݵĵ�һ���ڼ�¼��������������
function DisplayPageNum( nFrameNum )
	local recordRich		= getglobal( "FriendChatFrame" .. nFrameNum .. "_RecordFrame_RecordDisplayRich" );
	local nTotalRichLine	= recordRich:GetTextLines();
	local nCurLine			= recordRich:GetClientUserData( 0 );
	local pageDis			= getglobal( "FriendChatFrame".. nFrameNum.."_RecordFrame_PageFont" );
	local nCurPage			= recordRich:GetClientUserData( 1 );
	local nTotalPage		= math.ceil( nTotalRichLine / MAX_RECORD_LINE_ONE_PAGE );
	pageDis:SetText( "��" .. ( nCurPage > 0 and nCurPage or 1 ) .. "/" .. ( nTotalPage > 0 and nTotalPage or 1 ) .. "ҳ" );
end

-- ������������¼������
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
	font:SetText( "��" .. t_recordContent[szFrameName].curPage .. "/" .. 
			( t_recordContent[szFrameName].totalPage < t_recordContent[szFrameName].curPage and t_recordContent[szFrameName].curPage
			or t_recordContent[szFrameName].totalPage ) .. "ҳ" );
end

function FriendChatRecordFrame_OnShow()
	-- ��������¼���ԭ��������
	ChangePageContent( 1, this:GetName() );
end

--@Desc:��Ϊ������ϰ�ť��ʱ����Ӧ
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

--@Desc:��Ϊ������¹�����ť��ʱ����Ӧ
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

--@Desc:��Ϊ������������ʱ����Ӧ
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
	-- ���������¼��ʾ���������Ϣ�����еļ�¼�������Ƿ�Ҫ��ʾ����
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

--@Desc:��Ϊ��������ͷʱ����Ӧ
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

--@Desc:��Ϊ�������ļ�ͷ��ʱ����Ӧ
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

--@Desc:��Ϊ������Ҽ�ͷʱ����Ӧ
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

--@Desc:��Ϊ������Ҽ�ͷʱ����Ӧ
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

-------------�Ҽ�ѡ���---------------------
--��Ϊ�����Ҽ�ѡ��õĸ�������
--@Desc:����˽��
function SendPrivateMessage( szPrivatePlayerName )
	-- ��ʾ�����,������˽�ĸ�ʽ
	local t_ChatInsertFrameControl = GetChatInsertFrameControl();
	t_ChatInsertFrameControl:setPrivatePlayerName( { ["name"] = szPrivatePlayerName } );

	SetChatType(CL_CHAT_PRIVATE);
	UpdateChatInsertFrameEnterFocus();
end

--@Desc:��Ϊ�����޸ı�ע���ķ���
function ModifyMemName( )
	if FriendMemFrame:IsShown() then
		return;
	end

	FriendMemFrame:Show();
	FriendMemFrame_InputEdit:Clear();
	SetFocusFrame( "FriendMemFrame_InputEdit" );
end

--@Desc: ��Ϊ��������ʱ�б���������

-------------------------------------------------------
--		ͷ���Ҽ�ѡ��		--
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
		ShowMidTips( "��Ҫ������Ҳ�����Ұ��Χ��" );
		return false;
	end

	if actor:getType() ~= GAT_PLAYER then
		ShowMidTips( "����ʧ�ܣ���ǰ�����޷�����" );
		return false;
	end
	
	local relationInfo = GameFriendManager:getFriendInfoByName( actor:getName() );
	
	if relationInfo ~= nil and ( relationInfo.m_nRelationType == CLIENT_RELATION_TYPE_XIONG_SHOU or 
			relationInfo.m_nRelationType == CLIENT_RELATION_TYPE_BLACK ) then

		ShowMidTips( "����ʧ�ܣ���ǰ�����޷�����" );
		return false;
	end

	-- �鿴����Ƿ��ǵж���Ӫ
	if actor:isCamp( SKILL_TARGET_ENEMY_PLAYER ) then
		ShowMidTips( "����ʧ�ܣ���ǰ�����޷�����" );
		return false;
	end
	--]]
	
	-- ����״̬���ܽ��и���
	if TradeFrame:IsShown( ) then
		ShowMidTips( "���ڽ��ף����ܽ��и������" );
		return false;
	end


	return true;
end

CLIENT_RELATION_TYPE_SELFTEAM	= 257;
CLIENT_RELATION_TYPE_OTHERTEAM	= 258;
-- �µ��Ҽ��˵�ѡ��
local t_rightOperation =	{
								-- �����Ҽ�ѡ��
								[CLIENT_RELATION_TYPE_NEWCOMER] = { "˽��", "�۲����", "���", "��Ϊ����", "����", "��������", },
								-- ����ͷ���Ҽ�ѡ��
								[CLIENT_RELATION_TYPE_NONE] = { "˽��", "�۲����", "���", "����", "��Ϊ����", "����", "���������", "��������", },
								-- �����Ҽ�ѡ��
								[CLIENT_RELATION_TYPE_FRIEND] = { "˽��", "�۲����", "���", "����", "��ע����", "���������", "��������", "ɾ��",  },
								-- ʦ���Ҽ�ѡ��
								[CLIENT_RELATION_TYPE_MASTER] = { "˽��", "�۲����", "���", "����", "��ע����", "��������" },
								-- ͽ���Ҽ�ѡ��
								[CLIENT_RELATION_TYPE_PRENTICE] = { "˽��", "�۲����", "���", "����", "��ע����", "��������" },
								-- �����Ҽ�ѡ��
								[CLIENT_RELATION_TYPE_SPOUSE] = { "˽��", "�۲����", "���", "����", "��ע����", "��������" },
								-- ����Ҽ�ѡ��
								[CLIENT_RELATION_TYPE_BROTHER] = { "˽��", "�۲����", "���", "����", "��ע����", "��������" },
								-- �����Ҽ�ѡ��
								[CLIENT_RELATION_TYPE_XIONG_SHOU] = { "˽��", "�����޵�", "���������", "��������", "ɾ��", },
								-- ׷ɱ�Ҽ�ѡ��
								[CLIENT_RELATION_TYPE_SU_DI] = { "˽��", "���������", "��������", "ɾ��", },
								-- �������Ҽ�ѡ��
								[CLIENT_RELATION_TYPE_BLACK] = { "��������", "ɾ��",  },
								-- ��ʱ�Ҽ�ѡ��
								[CLIENT_RELATION_TYPE_TMPFRIEND] = { "˽��", "�۲����", "���", "����", "��Ϊ����", "���������", "��������", "ɾ��", },
								-- �Ҽ��Լ���Աͷ��
								[CLIENT_RELATION_TYPE_SELFTEAM] = { "˽��", "�۲����", "����", "��Ϊ����", "����", "��������", },
								-- �Ҽ����˶�Աͷ��
								[CLIENT_RELATION_TYPE_OTHERTEAM] = { "˽��", "�۲����", "����", "��Ϊ����", "����", "��������", "���", 
																	"���������", },
								["ͬ�����"] = {}, ["�ܾ����"] = {}, ["����б�"] = {}, ["ˢ���б�"] = {},
								["˽��"] = {}, ["�۲����"] = {},["���"] = {}, ["����"] = {},["��ע����"] = {},
								["ɾ��"] = {}, ["���������"] = {}, ["��������"] = {}, ["�����޵�"] = {}, ["��Ϊ����"] = {},	["����"] = {},
								["�д�"] = {}, ["�������"]	= {},	["�������"] = {}, ["�������"] = {}, ["ת�öӳ�"]	= {},	["�������"]	= {},
								["ȡ��"] = {}, ["�����ں�"] = {},   ["�鿴��Դ"] = {}, ["�鿴����"] = {}, ["����Ի�"]	= {},	["����ͶƱ"]	= {},
								--Ϊ���ﱣ���۲�Է�
								["�۲�Է�"] = {},
								name = "", relationType = -1, nHeight = 0, nLow = 0, id = 0, frame = "", followType = 0
							};

function GetRightOpertionControl()
	return t_rightOperation;
end

t_rightOperation["����Ի�"].func = 
function ( )
	ChatFrameManager:OpenChatFrame( t_rightOperation["name"] );
end

t_rightOperation["ȡ��"].func = 
function ( )
	local frame = getglobal( t_rightOperation["frame"] );
	frame:Hide();
end

t_rightOperation["˽��"].func = function ( )
									SendPrivateMessage( t_rightOperation.name );
								end


t_rightOperation["�����ں�"].func = 
function ()
	local nPetMemID		= t_rightOperation["id"];
	local destPetActor	= ActorMgr:FindActor( nPetMemID );

	local destPetOwner = ActorMgr:FindActor( destPetActor:getOwnerId() );
	local mainplayer	= ActorMgr:getMainPlayer();
	if destPetOwner == nil then
		ShowMidTips( "������Ҳ�����Ұ��" );
		return;
	end
	
	local dist = util.CalActorsDistance( destPetOwner, mainplayer );
	if dist > CLIENT_MAX_RONG_HE_DIST ^ 2 then
		ShowMidTips( "������Ҿ���̫Զ" );
		return;
	end

	-- �ټ���Լ�	
	local myFightPet	= GetFightPet( mainplayer );
	if myFightPet == nil then
		ShowMidTips( "�����ٻ�һֻ��������ں�" );
		return;
	end
	
	local petPackage	= mainplayer:getPet();
	local petInfo		= petPackage:getPetInfo( myFightPet:getRoleWId() );

	local t_PetRongHeProcessFrameControl = GetPetRongHeProcessFrameControl();
	if t_PetRongHeProcessFrameControl:getRongHePlayerMemID() ~= 0 then
		ShowMidTips( "�����ڽ����ںϣ������ٴη����ں�����" );
		return;
	end
	
	local myPetMgr = mainplayer:getPet()
	if myPetMgr:getPetHeti() ~= -1 then
		ShowMidTips( "�������״̬�£����ܽ��г����ں�" );
		return;
	end
	local itsPetMgr = destPetOwner:getPet()
	if itsPetMgr:getPetHeti() ~= -1 then
		ShowMidTips( "�Է����ڳ������״̬�����ܽ��г����ں�" );
		return;
	end

	-- ����Ƿ���ͬ�����
	if petInfo.PetID ~= destPetActor:getPetDefId() then
		ShowMidTips( "����ͬ�ֳ�������ں�" );
		return;
	end
	
	-- �ȼ��Է�
	-- ��������ǲ�������������
	if destPetActor:getPetGenius() < CLIENT_PET_GENIUS_B then
		ShowMidTips( "������������Ҫ���㣬�����ں�" );
		return;
	end

	if myFightPet:getPetGenius() < CLIENT_PET_GENIUS_B then
		ShowMidTips( "������������Ҫ���㣬�����ں�" );
		return;
	end
	
	-- ���˫������һֻ�����Ƕ�����������ں�
	if petInfo.Hierarchy == 2 then
		ShowMidTips( "������������ں�" );
		return;
	end
	
	-- �ٿ�˫������ĵȼ��ǲ�������Ϊ50��
	if myFightPet:getLv() < CLIENT_MIN_RONG_HE_LV then
		ShowMidTips( "����ȼ�����Ҫ50���������ں�" );
		return;
	end
	
	if destPetActor:getLv() < CLIENT_MIN_RONG_HE_LV then
		ShowMidTips( "����ȼ�����Ҫ50���������ں�" );
		return;
	end

	local petDef = getPetDef( petInfo.PetID );
	if petDef.BabyPetID == 0 then
		ShowMidTips( "�ó���û��һ���������ں�" );
		return;
	end

	petPackage:inviteFuse( nPetMemID );
end

t_rightOperation["�۲����"].func = function ( )
									local mainplayer = ActorMgr:getMainPlayer()
									if mainplayer == nil then return end
									local Actor;
									if t_rightOperation["FriendOnline"] ~= nil then
										if t_rightOperation["FriendOnline"] == CLIENT_OFFLINE then
											ShowMidTips("�Է�������")
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
										ShowMidTips("�Է�������Ұ��");
										return;
									end

									if Actor:getType() == GAT_PET then
										local petPackage = mainplayer:getPet();
										petPackage:petWatch( t_rightOperation["id"] );
									end
								end
t_rightOperation["�۲�Է�"].func = function ( )
									local mainplayer = ActorMgr:getMainPlayer()
									if mainplayer == nil then return end
									local Actor;
									if t_rightOperation["FriendOnline"] ~= nil then
										if t_rightOperation["FriendOnline"] == CLIENT_OFFLINE then
											ShowMidTips("�Է�������")
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
										ShowMidTips("�Է�������Ұ��");
										return;
									end

									if Actor:getType() == GAT_PET then
										local petPackage = mainplayer:getPet();
										petPackage:petWatch( t_rightOperation["id"] );
									end
								end

t_rightOperation["���"].func = function ( )
									TeamManager:Team_OP_Add( t_rightOperation.name );
								end

--[[t_rightOperation["�������"].func = function ( )
									TeamManager:Team_OP_Apply( t_rightOperation.name );
								end--]]

t_rightOperation["����"].func = function ( )
									local mainplayer = ActorMgr:getMainPlayer();
									if mainplayer == nil then 
										return;
									end

									if OnlineStoreFrame:IsShown() then
										return ShowMidTips( "�鿴�����У��޷����н���" );
									end

									local actor	= ActorMgr:findActorByName( t_rightOperation.name );
									if actor == nil then 
										ShowMidTips( "�Է����߻��߲�����Ұ��Χ�ڣ����뽻��ʧ��" );
										return; 
									end

									GameExchange:inviteExchange( actor:getName() );
								end

t_rightOperation["��Ϊ����"].func = function ( )
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

t_rightOperation["����"].func = function ( )
									local mainplayer = ActorMgr:getMainPlayer();
									if mainplayer:isCaptain() then										
										local teamInfo = TeamManager:getTeamInfo();
										for i = 1, teamInfo.MemberNum do
											local teamMember = teamInfo.TeamMember[i-1];
											if teamMember.MemberName == t_rightOperation.name then
												ShowMidTips( "�ӳ����ܸ����Ա" );
												return;
											end
										end
									else
										if TeamManager:isInTeamFollow() then
											ShowMidTips( "��Ӹ������޷�������ͨ����" );
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

t_rightOperation["�д�"].func = function ( )
									if OnlineStoreFrame:IsShown() then
										return ShowMidTips( "�鿴�����У��޷������д�" );
									end

									local actor = ActorMgr:findActorByName( t_rightOperation.name );
									if actor ~= nil then
										GameActorPVP:requestSafePK( actor:getName() );
									end
								end

t_rightOperation["��ע����"].func = function ( )
										ModifyMemName();
									end

t_rightOperation["ɾ��"].func = function ( )
										DeleteCharacterName();
									end

t_rightOperation["���������"].func = function ( )
										GameFriendManager:addBlack( t_rightOperation.name );
									end

t_rightOperation["��������"].func = 
function ( )
	CopyToMemory( GetPlayerName(t_rightOperation.name) );
end

t_rightOperation["�����޵�"].func = 
function ( )
	if GameFriendManager:isPlayerRelation( CLIENT_RELATION_TYPE_FRIEND, t_rightOperation.name ) then
		ShowMidTips( "�������ѹ�ϵ��������޵�����" );
		return;
	end

	GameFriendManager:addEnem( t_rightOperation.name, t_rightOperation.nHeight, t_rightOperation.nLow );
end

t_rightOperation["�������"].func = 
function ( )
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	if mainplayer:isOnMonsterMachine() then
		-- ��������е
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
					ShowMidTips( "�������������Ұ��Χ�ڣ������������" );
					return false;
				end
				
				local mainplayer = ActorMgr:getMainPlayer();
				local dist = util.CalActorsDistance( actor, mainplayer );
				if dist > 5 ^ 2 then
					ShowMidTips( "������������̫Զ�������������" );
					return false;
				end

				if GameFriendManager:isPlayerRelation( CLIENT_RELATION_TYPE_SU_DI, actor:getName() ) then
					ShowMidTips( "Ŀ��������޵������У��޷�ͬ��밴��F�������޵������鿴" );
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

t_rightOperation["�������"].func = 
function ( )
	local actor = ActorMgr:findActorByName( t_rightOperation.name );
	if actor == nil then
		return;
	end

	local targetRider	= actor:getRider();
	local mainplayer	= ActorMgr:getMainPlayer();
	
	-- ����ѡ����Ŀ�괦�����ǵ���ˣ������ǾͿ��Խ���������
	if targetRider:isOnHorse() and targetRider:getRideItem().m_nOwnerID == mainplayer:GetID() then
		local rider	= mainplayer:getRider();
		rider:kickOffPassenger( actor:GetID() );
	end
end

t_rightOperation["�������"].func = 
function ( )
	GameClanManager:clan_Opt_Invite( t_rightOperation.name );
end

t_rightOperation["�鿴����"].func = 
function ( )
	local actor = ActorMgr:findActorByName( t_rightOperation.name );
	if actor then
		ClanListFrame:SetPoint( "center", "$parent", "center", 0, 0 );
		ClanListFrame:Show();
		ClanList_SearchClanBtn:Disable();
		GameClanManager:clan_Opt_SearchClanByWID( actor:getClanWID() );
	end
end

t_rightOperation["ת�öӳ�"].func = 
function ( )
	TeamManager:Team_OP_CaptainChange( t_rightOperation.name );
end

t_rightOperation["�������"].func = 
function ( )
	TeamManager:Team_OP_Kick( t_rightOperation.name );
end

t_rightOperation["�鿴��Դ"].func = 
function ( )
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	WatchMachineResourceFrame:SetClientUserData( 0,mainplayer:GetSelectTarget() );
	WatchMachineResourceFrame:Show( );
end

t_rightOperation["����ͶƱ"].func = t_rightOperation["�������"].func;

t_rightOperation["ͬ�����"].func = 
function ()
	local applyPlayer = TeamManager:getTeamApplyInfo( t_rightOperation.name )
	TeamManager:Team_OP_ApplyAccept( applyPlayer.RoleID.nHeight, applyPlayer.RoleID.nLow );
	CancelBlinkPlayer( t_rightOperation.name );
	ClearSelMember();
	UpdateApplyList();
end

t_rightOperation["�ܾ����"].func = 
function ()
	local applyPlayer = TeamManager:getTeamApplyInfo( t_rightOperation.name )
	CancelBlinkPlayer( t_rightOperation.name );
	TeamManager:Team_OP_ApplyRefuse( applyPlayer.RoleID.nHeight, applyPlayer.RoleID.nLow );
	ClearSelMember();
	UpdateApplyList();
end

t_rightOperation["����б�"].func = 
function ()
	TeamManager:Team_OP_ApplyClear();
	CancelAllBlinkApply();
	ClearSelMember();
	UpdateApplyList();
end

t_rightOperation["ˢ���б�"].func = 
function ()
	TeamManager:updateNearPlayer();
	SelectFirstAroundPlayer();
	UpdateAroundList();
end

-- ���Ҽ�ѡ����Ե�����Ƕ�Ա�Ļ�
--	�������Ƕӳ��Ļ����Ӹ������ӳ�����
-- ר��������ʾͷ���Ҽ�ѡ��Ļص�����
function ShowCharacterRightFrame( szName )
	local frame = getglobal( "FriendFrame_RightOptionFrame" );
	HideRightFrameAllButton();
	local nShowBtn = 0;

	local t_rightOptions = {};
	local member	= TeamManager:getTeamMemberInfo( szName );
	local actor		= ActorMgr:findActorByName( szName );
	-- �����Լ��Ķ�Ա��
	if member ~= nil then
		t_rightOptions = { op = t_rightOperation[CLIENT_RELATION_TYPE_SELFTEAM], relationType = CLIENT_RELATION_TYPE_SELFTEAM };
	-- ���ǶԷ�����Ķ�Ա
	elseif actor ~= nil and actor:isInTeam() then
		t_rightOptions = { op = t_rightOperation[CLIENT_RELATION_TYPE_OTHERTEAM], relationType = CLIENT_RELATION_TYPE_OTHERTEAM };
	else
	-- ��û���κι�ϵ
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
	-- ��Ŀ������Ƕ��ѵĻ������Ǵ������״̬����Ҫ��һ�� "�������" ���Ҽ�ѡ��
	local mainplayer	= ActorMgr:getMainPlayer();
	local rider			= mainplayer:getRider();
	local curUsingRide	= rider:getRideItem();
	local rideDef		= getRideDef( curUsingRide.RideID, curUsingRide.nLevel );
	local targetActor	= ActorMgr:findActorByName( szName );
	if targetActor ~= nil then
		local btn	= getglobal( "FriendFrame_RightBtn" .. ( nShowBtn + 1 ) );
		local font	= getglobal( "FriendFrame_RightBtn" .. ( nShowBtn + 1 ) .. "Font" );
		font:SetText( "�д�" );
		font:SetTextColor( 145, 125, 55 );
		btn:Show();
		btn:Enable();
		nShowBtn = nShowBtn + 1;
	end
	-- ����������������ǵĶ��ѣ������Ǵ������״̬�����������õ������Ƕ�������ұ�������˲����ҵĶ���������
	if TeamManager:isInTeam( szName ) and mainplayer:isInRide() and rideDef.RideOnNum > 0 and mainplayer:GetID() == curUsingRide.m_nOwnerID then
		local szTex = "";
		-- �������������������ͬһ��λ���ϣ���������
		if targetActor ~= nil and targetActor:getRider():getRideItem().m_nOwnerID ~= curUsingRide.m_nOwnerID then
			szTex = "�������";
		elseif targetActor ~= nil and targetActor:getRider():getRideItem().m_nOwnerID == curUsingRide.m_nOwnerID then -- ������Ķ��������ǵ��������
			szTex = "�������";
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

	-- �����е����
	if mainplayer:isOnMonsterMachine() then		
		local monster = ActorMgr:FindActor( mainplayer:getMonsterMachine().m_playerInfo.m_nMonsterMemID );
		if monster == nil then return end;
		local monMachine = monster:getMonsterMachine();
		if not monMachine:canChangeSide() and monMachine:canRideOn() then
			local btn	= getglobal( "FriendFrame_RightBtn" .. ( nShowBtn + 1 ) );
			local font	= getglobal( "FriendFrame_RightBtn" .. ( nShowBtn + 1 ) .. "Font" );
			font:SetText( "�������" );
			font:SetTextColor( 145, 125, 55 );
			btn:Show();
			btn:Enable();
			nShowBtn = nShowBtn + 1;
		end
	end
	
	-- �������Ǹ��᳤���߻᳤��ʱ�򣬳�����������ѡ��
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
				font:SetText( "�������" );
				font:SetTextColor( 145, 125, 55 );
				btn:Enable();
				btn:Show();
				nShowBtn = nShowBtn + 1;
				break;
			end
		end
	end
	local clanName = actor and actor:getClanName()
	-- �����ʱ�򣬲���ʾ�鿴����
	if not mainplayer:isOutSide() and clanName and clanName ~= "" then
		local btn	= getglobal( "FriendFrame_RightBtn" .. ( nShowBtn + 1 ) );
		local font	= getglobal( "FriendFrame_RightBtn" .. ( nShowBtn + 1 ) .. "Font" );
		font:SetText( "�鿴����" );
		font:SetTextColor( 145, 125, 55 );
		btn:Enable();
		btn:Show();
		nShowBtn = nShowBtn + 1;
	end
	
	-- �����Լ��������
	if t_rightOptions["relationType"] == CLIENT_RELATION_TYPE_SELFTEAM then
		-- �������Ƿ��Ƕӳ������ǣ�����ʾת�öӳ���ѡ��
		if mainplayer:isCaptain() then
			local t_options = { "ת�öӳ�" };
			-- ���Ŀ���ڸ������� ��ʾ����ͶƱ
			if member.MapID >= CLIENT_GAME_PWORLD_MAPID then
				table.insert( t_options,"����ͶƱ" );
			else
				table.insert( t_options,"�������" );
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
			-- ����ڹ����е���棬����Ҽ��˵�
		else
			-- �Ƿ��ڸ�����
			--if MapShower:getRealMapId() >= CLIENT_GAME_PWORLD_MAPID then
				local btn	= getglobal( "FriendFrame_RightBtn" .. ( nShowBtn + 1 ) );
				local font	= getglobal( "FriendFrame_RightBtn" .. ( nShowBtn + 1 ) .. "Font" );
				font:SetText( "����ͶƱ" );
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
	font:SetText( "ͬ�����" );
	font:SetTextColor( 145, 125, 55 );
	btn:Show();
	btn:Enable();

	btn	= getglobal( "FriendFrame_RightBtn2");
	font	= getglobal( "FriendFrame_RightBtn2Font" );
	font:SetText( "�ܾ����" );
	font:SetTextColor( 145, 125, 55 );
	btn:Show();
	btn:Enable();

	btn	= getglobal( "FriendFrame_RightBtn3");
	font	= getglobal( "FriendFrame_RightBtn3Font" );
	font:SetText( "��Ϊ����" );
	font:SetTextColor( 145, 125, 55 );
	btn:Show();
	btn:Enable();

	btn	= getglobal( "FriendFrame_RightBtn4");
	font	= getglobal( "FriendFrame_RightBtn4Font" );
	font:SetText( "����б�" );
	font:SetTextColor( 145, 125, 55 );
	btn:Show();
	btn:Enable();
	
	btn = getglobal( "FriendFrame_RightBtn5" )
	font	= getglobal( "FriendFrame_RightBtn4Font" );
	font:SetText( "�۲����" );
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
	font:SetText( "���" );
	font:SetTextColor( 145, 125, 55 );
	btn:Show();
	btn:Enable();

	btn	= getglobal( "FriendFrame_RightBtn2");
	font	= getglobal( "FriendFrame_RightBtn2Font" );
	font:SetText( "��Ϊ����" );
	font:SetTextColor( 145, 125, 55 );
	btn:Show();
	btn:Enable();

	btn	= getglobal( "FriendFrame_RightBtn3");
	font	= getglobal( "FriendFrame_RightBtn3Font" );
	font:SetText( "ˢ���б�" );
	font:SetTextColor( 145, 125, 55 );
	btn:Show();
	btn:Enable();
	
	btn = getglobal( "FriendFrame_RightBtn4" )
	font	= getglobal( "FriendFrame_RightBtn4Font" );
	font:SetText( "�۲����" );
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

	local t_rightOptions = { "˽��", "�۲����", "���", "��Ϊ����", "���������", "��������" };

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
	-- ����Ƿ���ͬ�����
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
	font:SetText( "�۲�Է�" );
	font:SetTextColor( 145, 125, 55 );
	btn:Enable();
	btn:Show();
	SetRightFrameInfo( { name = "", relationType = CLIENT_RELATION_TYPE_NONE, nHeight = 0,
								nLow = 0, id = nId, frame = "FriendFrame_RightOptionFrame" } );
	
	local petActor		= ActorMgr:FindActor( nId );
	local mainplayer	= ActorMgr:getMainPlayer();
	-- ���������ǵĳ���
	if false and mainplayer:GetID() ~= petActor:getOwnerId() then
		nShowBtn = nShowBtn + 1;
		btn		= getglobal( "FriendFrame_RightBtn"..nShowBtn );
		font	= getglobal( btn:GetName().."Font" );
		font:SetText( "�����ں�" );
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
	font:SetText( "����" );
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

-- ��Ϊ���RichText����һ���button��ʱ����Ӧ

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
			-- ��������İ�ť�в��洢�κ������Ϣ
			if "" == rich:GetClientString( ) then
				return;
			end

			-- �����Ѳ�����������Ϣ�������в����߸�����ʾ�����ǲ������Ի���
			local roleInfo ;
			local roleInfo = GameFriendManager:getFriendInfoByName( g_CurClickRole.roleName );
			if roleInfo.RoleID == 0 then return end
			if not ChatFrameManager:IsNewSpeaker( g_CurClickRole.roleName ) and roleInfo.Online == CLIENT_OFFLINE then
				ShowMidTips( "�ú���Ŀǰ������,���ܷ����µĶԻ�" );
				return;
			end

			local szFrameName = "";

			-- �ȼ���Ƿ����뱻�����Ҷ�Ӧ����������ʾ���촰��,������ʲôҲ���ͽ����ӳ���
			if ChatFrameManager:GetSpeakerFrame( g_CurClickRole.roleName ) ~= nil then
				return;
			end
			
			-- ��û���뱻�����Ҷ�Ӧ�����촰�ڻ�������ʾ��
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
			ShowMidTips("���ܶ��Լ����в���");
			return;
		end
		
		ResetHighLightTex();
		if arg1 ~= "RightButton" then
			if rich:GetClientString() == "" then return end
			-- ��������İ�ť�в��洢�κ������Ϣ
			if "" == rich:GetClientString( ) then
				return;
			end

			-- �����Ѳ�����������Ϣ�������в����߸�����ʾ�����ǲ������Ի���
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
				ShowMidTips( "�����Ŀǰ������,���ܷ����µĶԻ�" );
				return;
			end

			local szFrameName = "";

			-- �ȼ���Ƿ����뱻�����Ҷ�Ӧ����������ʾ���촰��,������ʲôҲ���ͽ����ӳ���
			if ChatFrameManager:GetSpeakerFrame( g_CurClickRole.roleName ) ~= nil then
				return;
			end
			
			-- ��û���뱻�����Ҷ�Ӧ�����촰�ڻ�������ʾ��
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

				-- �ж�Ŀ������Ƿ��Ƕ��ѣ����Ƕ���
				-- ��ʾ�Ҽ�ѡ���
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
			-- �����Ѳ�����������Ϣ�������в����߸�����ʾ�����ǲ������Ի���
			local roleInfo = GameFriendManager:getFriendInfoByName( g_CurClickRole.roleName );
			if not ChatFrameManager:IsNewSpeaker( g_CurClickRole.roleName ) and roleInfo.m_nIsOnline == CLIENT_OFFLINE then
				ShowMidTips( "�ú���Ŀǰ������,���ܷ����µĶԻ�" );
				return;
			end

			-- ������������е�����,�������Ի���
			if GameFriendManager:isPlayerRelation( CLIENT_RELATION_TYPE_BLACK, g_CurClickRole.roleName ) then
				ShowMidTips( "������������е���ҶԻ�" );
				return;
			end

			local szFrameName = "";

			-- �ȼ���Ƿ����뱻�����Ҷ�Ӧ����������ʾ���촰��,������ʲôҲ���ͽ����ӳ���
			if ChatFrameManager:GetSpeakerFrame( g_CurClickRole.roleName ) ~= nil then
				return;
			end
			
			-- ��û���뱻�����Ҷ�Ӧ�����촰�ڻ�������ʾ��
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
				font:SetText( "����Ի�" );
				font:SetTextColor( 145, 125, 55 );
				btn:Enable();
				btn:Show();
			end
			local character	= GameFriendManager:getFriendInfoByName( g_nDisplayPageType, g_CurClickRole.roleName );
			SetRightFrameInfo( { name = g_CurClickRole.roleName, relationType = g_nDisplayPageType, nHeight = g_CurClickRole.roleHeightId,
										nLow = g_CurClickRole.roleLowId, id = 0, frame = "FriendFrame_RightOptionFrame", watchIgnorDist = true ,
										FriendOnline = character.m_nIsOnline } );

			-- �ж�Ŀ������Ƿ��Ƕ��ѣ����Ƕ���
			-- ��ʾ�Ҽ�ѡ���
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

-- ˢ��������ʾ������ʾ���
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
-- ��Ϊ�����Ҽ�ѡ����ʾ��ʱ����Ӧ
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

-- ���������
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

-- ��������Ϣ��ʱ�������ʾ�������״̬
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
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "�����ռ�", 
								frame = this:GetParent(), button = "cursor" } );
end

function FriendShipFrame_CharacterNameTemplateLoverSign_OnEnter()
	local button = getglobal( this:GetParent() )
	local name = button:GetClientString()
	local friendInfo = GameFriendManager:getFriendInfoByName( name );
	local szInfo = string.format( "��ǰ��ϵ��#G%s#n\n��ǰ���ܶȣ�#G%d#n\n\n������ӡ����ܶȡ���\n1��˫��������Ұ���ճ�����\n2��˫�������ɵع���������\n3��˫�����ͨ�ؼ��Ѷ����ϸ���\n4��˫�������ɡ���Ȥ������\n5��˫�������ɡ��������ɡ�����\n6����������͡��������\n#R��ĳ��������ѣ����ܶȽ��������#n",
							friendInfo.m_nIntimateColorType == INTIMATE_COLOR_LOVERS_TOLUA and "����" or "����", friendInfo.m_nBuddyVal )
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
		ShowMidTips( "����������Ϣ" );
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
		ShowMidTips( "�޷��ҵ��ú�����Ϣ" );
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
				["FriendInfoFramePosBtnFont"]		= { [true] = "��ǰ������",					[false] = "" },
				["FriendInfoFrameFriendBtnFont"]	= { [true] = 0,				[false] = "" },
				["FriendInfoFrameTeamBtnFont"]		= { [true] = "��ǰ������",					[false] = "" },
				["FriendInfoFrameCharacterPhotoBtnFont"]= { [true] = "Lv" .. friendInfo.m_nLevel,			[false] = "" },
				["FriendInfoFrameQinMiBtnValFont"]	= { [true] = "���ܶȣ�".. friendInfo.m_nBuddyVal,		[false] = "" },
				};
	
	for name, content in pairs( tipsContent ) do
		local font = getglobal( name );
		font:SetText( content[true] );
	end	

	 --��ʾ���ͷ���߼�
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
		FriendInfoFramePosBtnFont:SetText( "�����" )
	else
		FriendInfoFramePosBtnFont:SetText( MaxMapControl:getMapName( requirePlayer.Map ) or "δ֪" );
	end
	FriendInfoFrameFriendBtnFont:SetText( requirePlayer.Charm )
	FriendInfoFrameTeamBtnFont:SetText( requirePlayer.TeamRoleNum > 0 and "�����" .. requirePlayer.TeamRoleNum .. "��" or "δ���" )	
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
	SetGameTooltips( this:GetName(), "#G���ܶȡ�300��˫���ɳ�Ϊ����\n���ܶȡ�2000��˫���ɽ�Ϊ��#n\n\n������ӡ����ܶȡ���\n1��˫��������Ұ���ճ�����\n2��˫�������ɵع���������\n3��˫�����ͨ�ؼ��Ѷ����ϸ���\n4��˫�������ɡ���Ȥ������\n5��˫�������ɡ��������ɡ�����\n6����������͡��������\n#R��ĳ��������ѣ����ܶȽ��������#n" )
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
		ShowMidTips( "����������Ϣ" );
		FriendInfoFrame:Hide();
		return;
	end
	local roleName = frame:GetClientString();
	if GameFriendManager:getPlayerRelation( roleName ) ~= CLIENT_RELATION_TYPE_FRIEND then
		ShowMidTips( "ֻ�ܶԺ������ñ�ע" );
		return;
	end
	FriendMemFrame:Show();
	FriendMemFrame_InputEdit:Clear();
	SetFocusFrame( "FriendMemFrame_InputEdit" );
end

function FriendInfoFrameTalkBtn_OnClick()
	local frame = getglobal( "FriendInfoFrame" );
	if frame:GetClientString() == "" then 
		ShowMidTips( "����������Ϣ" );
		FriendInfoFrame:Hide();
		return;
	end
	local roleName = frame:GetClientString();

	-- �����Ѳ�����������Ϣ�������в����߸�����ʾ�����ǲ������Ի���
	local roleInfo = GameFriendManager:getFriendInfoByName( roleName );
	if not ChatFrameManager:IsNewSpeaker( roleName ) and roleInfo.m_nIsOnline == CLIENT_OFFLINE then
		ShowMidTips( "�ú���Ŀǰ������,���ܷ����µĶԻ�" );
		return;
	end

	local szFrameName = "";

	-- �ȼ���Ƿ����뱻�����Ҷ�Ӧ����������ʾ���촰��,������ʲôҲ���ͽ����ӳ���
	if ChatFrameManager:GetSpeakerFrame( roleName ) ~= nil then
		return;
	end
	
	-- ��û���뱻�����Ҷ�Ӧ�����촰�ڻ�������ʾ��
	ChatFrameManager:OpenChatFrame( roleName );
end

function FriendInfoFrameSctalkBtn_OnClick()
	local frame = getglobal( "FriendInfoFrame" );
	if frame:GetClientString() == "" then 
		ShowMidTips( "����������Ϣ" );
		FriendInfoFrame:Hide();
		return;
	end
	SendPrivateMessage( frame:GetClientString() );
end

function FriendInfoFrameDelFriBtn_OnClick()
	local frame = getglobal( "FriendInfoFrame" );
	if frame:GetClientString() == "" then 
		ShowMidTips( "����������Ϣ" );
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
		ShowMidTips( "����������Ϣ" );
		FriendInfoFrame:Hide();
		return;
	end

	if OnlineStoreFrame:IsShown() then
		return ShowMidTips( "�鿴�����У��޷����н���" );
	end

	local actor = ActorMgr:findActorByName(  frame:GetClientString() );
	if actor == nil then 
		ShowMidTips( "�Է����߻��߲�����Ұ��Χ�ڣ����뽻��ʧ��" );
		return; 
	end

	GameExchange:inviteExchange( actor:getName() );
end

function FriendInfoFrameGrouopBtn_OnClick()
	local frame = getglobal( "FriendInfoFrame" );
	if frame:GetClientString() == "" then 
		ShowMidTips( "����������Ϣ" );
		FriendInfoFrame:Hide();
		return;
	end
	local roleName = frame:GetClientString();
	if GameFriendManager:getPlayerRelation( roleName ) ~= CLIENT_RELATION_TYPE_FRIEND then
		ShowMidTips( "ֻ�ܶԺ��ѽ��з���" );
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
		ShowMidTips( "����������Ϣ" );
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
		ShowMidTips( "����������Ϣ" );
		FriendInfoFrame:Hide();
		return;
	end
	if FriendInfoFramePosBtnFont:GetText() == "��ǰ������" then
		ShowMidTips( "����ҵ�ǰ������" );
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
		ShowMidTips( "����������Ϣ" );
		FriendInfoFrame:Hide();
		return;
	end
	if FriendInfoFramePosBtnFont:GetText() == "��ǰ������" then
		ShowMidTips( "����ҵ�ǰ������" );
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
		ShowMidTips( "����������Ϣ" );
		FriendInfoFrame:Hide();
		return;
	end
	if FriendInfoFramePosBtnFont:GetText() == "��ǰ������" then
		ShowMidTips( "����ҵ�ǰ������" );
		return;
	end
	GameFriendManager:RequestWatchFriend( frame:GetClientString() );
end

local MAXGROUPNUM = 5;
local t_ChGroupName = { "һ", "��", "��", "��", "��" };
function FriendSetGroupFrame_OnShow()
	local frame = getglobal( "FriendInfoFrame" );
	if frame:GetClientString() == "" then 
		ShowMidTips( "����������Ϣ" );
		FriendInfoFrame:Hide();
		return;
	end
	FriendSetGroupFrameNowFont:SetText( "��ǰ���飺" .. t_ChGroupName[ frame:GetClientUserData( 2 ) ] );	
	for i=1, MAXGROUPNUM do
		local btn = getglobal( "FriendSetGroupFrameGroupBtn" .. i );
		btn:SetClientUserData( 0, i );
	end
end

function FriendSetGroupFrameGroupBtn_OnClick()
	local frame = getglobal( "FriendInfoFrame" );
	if frame:GetClientString() == "" then 
		ShowMidTips( "����������Ϣ" );
		FriendInfoFrame:Hide();
		return;
	end
	local nGroupID = this:GetClientUserData( 0 );
	if nGroupID == nil or nGroupID <= 0 then
		ShowMidTips( "����ķ�����Ϣ" );
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
	SetGameTooltips( this:GetName(), "�ȼ��ɳ�������\n��ʦ����ѧ����ѧ��ÿ�ɳ���һ���ĵȼ�ʱ����ʦ������ȡһ��ϵͳ����\n��ȡ������\nѧ�������ڵ�ʦָ���³ɳ�10���󣬵�ʦ�ſ�����ȡ��Ӧ�ĵȼ��ɳ�����" )
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

	--�������Ϳ���Ϣ
	if "" == szChatContent then
		ShowMidTips( "������Ϣ����Ϊ��" );
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
	--�������ռ�ĺ���
	local t_MudGroup = {};
	local nMaxRelativeRole = GameFriendManager:getRelationPlayerCount( CLIENT_RELATION_TYPE_FRIEND );
	for i=1,nMaxRelativeRole do
		local friendInfo = GameFriendManager:getFriendInfoByIndex( CLIENT_RELATION_TYPE_FRIEND, i - 1 );
		if friendInfo.m_nIsOnline == MUD_ONLINE then
			table.insert( t_MudGroup, friendInfo.m_RoleID );
		end
	end

	if not isCheck then
		ShowMidTips( "��ѡ������һ������" );
		return;
	end

	if table.getn( t_SelGroup ) == 0 then
		ShowMidTips( "����ѡ�ķ�����û�к���" );
		return;
	end

	if countTime > 0 and os.clock() - countTime < 5 then
		ShowMidTips( "��ϢȺ���ļ��Ϊ5�룬���Ժ��ٷ�" );
		return; 
	end

	szChatContent = string.gsub( szChatContent, "#[L]", "**" );
	
	local t_ClientSelfChatContentParser = GetClientSelfChatContentParser();

	if t_ClientSelfChatContentParser:DoClientDefineInstruction( { ["text"] = szChatContent } ) then
		return;
	end
	szChatContent = t_ClientSelfChatContentParser:parseLinkTextForClient( { ["text"] = szChatContent } );
	-- ����Ϣ�����������	
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

	-- ���������е�����
	inputEdit:Clear();
	countTime = os.clock();
	ClearSetLink();		
end

function FriendShipFrameFriendNumBtn_OnEnter()
	local szText = "��ǰ����/��������";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function FriendChatFrame_InviteBtn_OnEnter()
	local szText = "�ͺ���һ�����";
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
	local szText = "�������������ڵĶ���";
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
---------------------------------------------------�ͻ�����-----------------------------------------------------
local t_FlowerState = {}
local t_FlowerTypeInfo = { [0]={ path="uires\\ui\\rose01.tga", name="õ�廨", cost=1, effect="effect\\entity\\5012125.ent", }, 
							[1]={ path="uires\\ui\\knx.tga", name="����ܰ", cost=10, effect="effect\\entity\\5012126.ent", }, 
							[2]={ path="uires\\ui\\lsyj.tga", name="��ɫ����", cost=100, effect="effect\\entity\\5012127.ent", }
						}
local t_EggsTypeInfo = { [0]={ name="����", cost=1, }, [1]={ name="ը��", cost=10, }, [2]={ name="���" }, cost=100, }

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
--------------------------------------------�ʻ�����Ƶ��----------------------------------------------------
local t_FlowerReportList = {}
local MAX_FLOWER_REPORT_LINE = 3
--��������ʱ�Ľ������Ʊ�ʶ
local endBegin = false
local endBeginTime = 0
--�����Ĵ���500���ʱ���ӳ���ʾ��ʶ
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
	--���û�����ӳ���ʾ��������ֹͣ������������һ�β���
	if not keepShow then
		endBegin = false
		endBeginTime = 0
	end
	--С�˺������ߣ�����ֲ������ͻ���Ϣ������ÿ���յ���Ϣ��ʱ��Ҫ��֤�ʻ�����Ƶ������ʾ��
	FlowerReportFrame:Show()
end

function FlowerReportFrame_OnLoad()
	this:setUpdateTime(0.03)
	FlowerReportFrameWords:setCenterLine( true )
end
function FlowerReportFrame_OnUpdate()
	--������ӳ���ʾ�׶Σ����ʱ�䣬�ж��Ƿ���Խ����ӳ���ʾ
	if keepShow then
		if os.clock() - keepShowBeginTime >= 10 then
			keepShow = false
			keepShowBeginTime = 0
			FlowerEffectFrame:Hide()
		end	
	end
	--����ڲ������������ʱ�䣬�ж��Ƿ�Ҫ��������
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
	--����б�Ϊ�գ��Ҳ����ӳ���ʾʱ��Σ��������������ʼ�´β���
	if table.getn( t_FlowerReportList ) ~= 0 then
		if not keepShow then
			endBegin = false
			endBeginTime = 0
			endAlpha = 0		
		end
	end
	if endBegin or keepShow then return end
	if table.getn( t_FlowerReportList ) ~= 0 then
		--����Ǵ�����Ϣ��һ�β���
		if t_FlowerReportList[1].totalNum == t_FlowerReportList[1].curNum then
			--�ҵ����Ͳ���ʾ
			if t_FlowerReportList[1].Flag == 0 then
				--�ж�Ҫ��ʾ�������͵���Ч
				local nType = t_FlowerReportList[1].Type
				local nNum = t_FlowerReportList[1].totalNum
				if t_FlowerTypeInfo[nType].cost * nNum < 500 then
					--�������ӳ���ʾ
					keepShow = false
					keepShowBeginTime = 0
					local mainplayer = ActorMgr:getMainPlayer()
					if mainplayer:getName() == t_FlowerReportList[1].Accepter or mainplayer:getName() == t_FlowerReportList[1].Sender then
						playUiEffect( 0,0,1, t_FlowerTypeInfo[nType].effect )
					end
				else
					--��¼�ӳ���ʾ��ʼʱ��
					keepShowBeginTime = os.clock()
					FlowerEffectFrame:Show()
					SetflowerEffect( nType )
				end
			end
		end
		--���������Ϣ�������
		if t_FlowerReportList[1].curNum == 0 then
			endBegin = true
			endBeginTime = os.clock()
			local nType = t_FlowerReportList[1].Type
			local nNum = t_FlowerReportList[1].totalNum
			--����500��ң���Ҫ�ӳ���ʾ
			if t_FlowerTypeInfo[nType].cost * nNum >= 500 then
				keepShow = true
			end
			table.remove( t_FlowerReportList, 1 )
			--�������Ҫ�ӳ���ʾ
			if not keepShow then
				FlowerEffectFrame:Hide()
			--��Ҫ�ӳ���ʾ�����ǲ���ʱ�䳬����10��
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
		--����Ҫ��������Ϣ
		for i = 1, MAX_FLOWER_REPORT_LINE do
			local flowerTex = getglobal( "FlowerReportFrameFlower"..i )
			local index = t_FlowerReportList[1].totalNum - t_FlowerReportList[1].curNum - (MAX_FLOWER_REPORT_LINE - i)
			local szReport = t_FlowerReportList[1].Flag == 0 and "%s\t���͸�\t%s" or "%s\t��\t%s\tʹ���˵���"
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
-------------------------------------------�ͻ������б�ѡ�����---------------------------------------------------
local t_UseFlowerChoseList = { 1, 10, 30, 66, 188, 520, 1314, }
function UseFlowerChoseShowBtnTemplate_OnClick()
	local index = this:GetClientID()
	UseFlowerFrameEdit:SetText( t_UseFlowerChoseList[index] )
	UseFlowerChoseFrame:Hide()
	UseEggsChoseFrame:Hide()
end
--------------------------------------------�����ʻ�����/ʹ�õ��߽���-----------------------------------------------
local t_UseFlowerList = {
	[0] = {
		[0] = { x="1008", y="114", w="15", h="15", texOffset = "66", text="õ�廨\t\t(-1��ң�+1����ֵ)", cost = 1, },
		[1] = { x="1008", y="130", w="15", h="15", texOffset = "66", text="����ܰ\t\t(-10��ң�+15����ֵ)", cost = 1, },
		[2] = { x="1008", y="146", w="15", h="15", texOffset = "81", text="��ɫ����\t\t(-100��ң�+180����ֵ)", cost = 1, },
	},
	[1] = {
		[0] = { x="291", y="689", w="15", h="15", texOffset = "51", text="����\t\t(-1��ң�-1����ֵ)", cost = 1, },
		[1] = { x="307", y="689", w="15", h="15", texOffset = "51", text="ը��\t\t(-10��ң�-15����ֵ)", cost = 1, },
		[2] = { x="323", y="689", w="15", h="15", texOffset = "51", text="���\t\t(-100��ң�-180����ֵ)", cost = 1, },
	},
}

local MAX_FLOWER_TYPE = 3

function OpenUseFlower( nFlag, szName )
	if nFlag == 0 then
		UseFlowerFrameHeadFont:SetText( "�ͻ�" )	
		UseFlowerFrameUseBtn:SetText( "����" )
		UseFlowerFrameNumFont:SetText( "��������" )
		UseFlowerFrameInfo:SetText( "����������ʻ���Ϊ��ĺ�����������ֵ." )
	elseif nFlag == 1 then
		UseFlowerFrameHeadFont:SetText( "�ҵ�" )
		UseFlowerFrameUseBtn:SetText( "ʹ��" )
		UseFlowerFrameNumFont:SetText( "ʹ������" )
		UseFlowerFrameInfo:SetText( "�����ʹ�õ��߻ή����ĺ�������ֵ." )
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
				ShowMidTips( "��ѡ����������" )
				return
			end
			local nFlag = UseFlowerFrame:GetClientUserData(0)
			local szName = UseFlowerFrame:GetClientString()
			--�Ƿ����������Լ�
			local mainplayer = ActorMgr:getMainPlayer()
			if szName == mainplayer:getName() then
				ShowMidTips( "�����Զ��Լ��ҵ�" )
				return
			end
			--�ȼ���Ƿ����㹻�Ľ��
			if mainplayer:getGodCoin() < t_UseFlowerList[nFlag][i-1].cost * itemNum then
				MessageBoxForVip( "��ʾ", "����ȡ#cffff00���#n���㣬��#cffff00��ȡ���#n�����²���" )
				MessageBoxFrameForVipAcceptButton:SetText("��ȡ���");
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
-----------------------------------------------�ͻ�����Ҳ�������--------------------------------------
function GodCoinShortFrame_OnLoad()
	GodCoinShortFrameInfo:setCenterLine( true )
	GodCoinShortFrameInfo:SetText( "�Բ������Ľ�Ҳ��㣬��ֵ�������µ�����Ͱ�", 255,255,190 )
end
function GodCoinShortFrameChargeBtn_OnClick()
	GameMgr:openWebSite( "http://pay.ljy0.com" );
end
----------------------------------------------ʦͽ��ش���--------------------------------------------------
function OpenSchoolChatFrame( szName )
	-- �ȼ���Ƿ����뱻�����Ҷ�Ӧ����������ʾ���촰��,������ʲôҲ���ͽ����ӳ���
	if ChatFrameManager:GetSpeakerFrame( szName ) ~= nil then
		return;
	end
	
	ChatFrameManager:OpenChatFrame( szName, CL_CHAT_WINDOW )
	SendSchoolWorld( szName )
end
--ǿ�Ʒ�����ͽ����
function SendSchoolWorld( szName )
	local mainplayer	= ActorMgr:getMainPlayer();
	local szMyName		= mainplayer:getName();
	local szParentName	= ChatFrameManager:GetSpeakerFrame( szName );
	local inputEdit		= getglobal( szParentName.."_InputEdit" );
	local szChatContent = inputEdit:GetText();
	local szSpeakerName	= ChatFrameManager:GetSpeakerName( szParentName );
	local chatFrame		= getglobal( szParentName );

	szChatContent = "��������Ϊ�ҵ�ѧ����������";
	
	local t_ClientSelfChatContentParser = GetClientSelfChatContentParser();

	if t_ClientSelfChatContentParser:DoClientDefineInstruction( { ["text"] = szChatContent } ) then
		return;
	end
	szChatContent = t_ClientSelfChatContentParser:parseLinkTextForClient( { ["text"] = szChatContent } );

	-- ����Ϣ�����������	
	sz2Server = t_ClientSelfChatContentParser:parseLinkTextForServe( { ["text"] = szChatContent } );
	ChatManager:requestChat( CL_CHAT_WINDOW, szSpeakerName, sz2Server, CLIENT_CHAT_NOT_SHOW_IN_CHAT_FRAME, 0 );

	-- ���������е�����
	inputEdit:Clear();
	ClearSetLink();
end

function SetSchoolRelation()
	t_SchoolGroupList = {}
	--������ʾ�б�
	if GameSchool:GetSelfSchoolJobType() == ROLE_SCHOOL_TYPE_NONE_TOLUA then
		table.insert( t_SchoolGroupList,{ name = "�ҵĵ�ʦ", bCheck = true, nGroup = 0, nSubType = 0} );
		table.insert( t_SchoolGroupList,{ name = "�ҵ�ѧ��", bCheck = true, nGroup = 1, nSubType = 0 } );
	elseif GameSchool:GetSelfSchoolJobType() == ROLE_SCHOOL_TYPE_TEACHER_TOLUA then
		table.insert( t_SchoolGroupList,{ name = "�ҵĵ�ʦ", bCheck = true, nGroup = 0, nSubType = 0} );
		table.insert( t_SchoolGroupList,{ name = "�ҵ�ѧ��", bCheck = true, nGroup = 1, nSubType = 0 } );
		for i = 1, GameSchool:GetSelfStudentNum() do
			local studentInfo = GameSchool:GetSelfStudentByIndex( i - 1 )
			table.insert( t_SchoolGroupList,{ name = studentInfo.RoleName, bCheck = false, nGroup = 1, 
					nSubType = 1, nLow = studentInfo.RoleID.nLow, nHeight = studentInfo.RoleID.nHeight });
		end
	elseif GameSchool:GetSelfSchoolJobType() == ROLE_SCHOOL_TYPE_STUDENT_TOLUA then
		table.insert( t_SchoolGroupList,{ name = "�ҵĵ�ʦ", bCheck = true, nGroup = 0, nSubType = 0} );
		local teacherInfo = GameSchool:GetSelfTeacher()
		table.insert( t_SchoolGroupList,{ name = teacherInfo.RoleName, bCheck = false, nGroup = 0, 
				nSubType = 1, nLow = teacherInfo.RoleID.nLow, nHeight = teacherInfo.RoleID.nHeight });
		table.insert( t_SchoolGroupList,{ name = "�ҵ�ѧ��", bCheck = true, nGroup = 1, nSubType = 0 } );
	end
	
	table.insert( t_SchoolGroupList,{ name = "�����Ƽ�",     bCheck = true, nGroup = 2, nSubType = 0 } );
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
	--�����ǰ��ʾ����ʦͽ��ϵ��ǩ
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
---------------------------------ʦͽ��Ϣ���
function DelSchoolOk( nHeight, nLow, szName )
	GameSchool:DelSchool( nHeight, nLow, szName )
	SchoolInfoFrame:Hide()
end
function SchoolInfoFrameDelSchoolBtn_OnClick()
	if MessageBoxFrame:IsShown() and MessageTitle:GetText() == "���ʦͽ��ϵ" then return end
	local frame = getglobal( "SchoolInfoFrame" );
	local szName = frame:GetClientString()
	if frame:GetClientString() == "" then 
		ShowMidTips( "����������Ϣ" );
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
		MessageBox( "���ʦͽ��ϵ", "ϵͳ��ʾ�������ʦ����ϵ��۳���"..SCHOOL_DEL_MONEY_NUM_TOLUA.."�������Ƿ�ȷ�Ͻ��ʦ����ϵ?" )
		MessageBoxFrame:SetClientString( szName )
		MessageBoxFrame:SetClientUserData( 0, schoolInfo.RoleID.nHeight )
		MessageBoxFrame:SetClientUserData( 1, schoolInfo.RoleID.nLow )
	end
end
function SchoolInfoFrameInvTeamBtn_OnClick()
	if MessageBoxFrame:IsShown() and MessageTitle:GetText() == "���ʦͽ��ϵ" then return end
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then return end
	local frame = getglobal( "SchoolInfoFrame" );
	if frame:GetClientString() == "" then 
		ShowMidTips( "����������Ϣ" );
		SchoolInfoFrame:Hide();
		return;
	end
	if SchoolInfoFramePosBtnFont:GetText() == "��ǰ������" then
		ShowMidTips( "����ҵ�ǰ������" );
		return;
	end
	TeamManager:Team_OP_Add( frame:GetClientString() );
end
function SchoolInfoFrameWatchBtn_OnClick()
	if MessageBoxFrame:IsShown() and MessageTitle:GetText() == "���ʦͽ��ϵ" then return end
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then return end
	local frame = getglobal( "SchoolInfoFrame" );
	if frame:GetClientString() == "" then 
		ShowMidTips( "����������Ϣ" );
		SchoolInfoFrame:Hide();
		return;
	end
	if SchoolInfoFramePosBtnFont:GetText() == "��ǰ������" then
		ShowMidTips( "����ҵ�ǰ������" );
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
		ShowMidTips( "�޷��ҵ���ʦͽ��Ϣ" );
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
	local relation = GameSchool:GetSelfSchoolJobType() == ROLE_SCHOOL_TYPE_STUDENT_TOLUA and "��ʦ"	or "ѧ��"
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
				["SchoolInfoFramePosBtnFont"]		= { [true] = "��ǰ������",					[false] = "" },
				["SchoolInfoFrameMeiLiBtnFont"]		= { [true] = schoolInfo.MeiLi,					[false] = "" },
				["SchoolInfoFrameTeamBtnFont"]		= { [true] = "��ǰ������",					[false] = "" },
				["SchoolInfoFrameCharacterPhotoBtnFont"]= { [true] = "Lv" .. schoolInfo.Level,			[false] = "" },
				};
	
	for name, content in pairs( tipsContent ) do
		local font = getglobal( name );
		font:SetText( content[true] );
	end	
	
	local MaxMapControl = MapShower:GetMaxMapContal();
	SchoolInfoFramePosBtnFont:SetText( MaxMapControl:getMapName( schoolInfo.Map ) or "δ֪" );
	SchoolInfoFrameTeamBtnFont:SetText( schoolInfo.TeamNum > 0 and "�����" .. schoolInfo.TeamNum .. "��" or "δ���" )

	 --��ʾ���ͷ���߼�
	SetPhoto( schoolInfo.Hair, schoolInfo.Face, "SchoolInfoFrameCharacterPhotoBtnHairPhoto", "SchoolInfoFrameCharacterPhotoBtnPhoto", schoolInfo.Gender, schoolInfo.Head,
				"SchoolInfoFrameCharacterPhotoBtnHairShadowTex", "SchoolInfoFrameCharacterPhotoBtnBodyTex" );
end
function OpenSchoolInfo( szName, nLowId, nHeightId )
	local frame = getglobal( "SchoolInfoFrame" );	
	frame:SetClientString( szName );
	if szName == "" then 
		ShowMidTips( "����������Ϣ" );
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
	if MessageBoxFrame:IsShown() and MessageTitle:GetText() == "���ʦͽ��ϵ" then
		MessageBoxFrame:Hide()
	end
end
function SchoolInfoFrameTalkBtn_OnClick()
	if MessageBoxFrame:IsShown() and MessageTitle:GetText() == "���ʦͽ��ϵ" then return end
	local frame = getglobal( "SchoolInfoFrame" );
	if frame:GetClientString() == "" then 
		ShowMidTips( "����������Ϣ" );
		SchoolInfoFrame:Hide();
		return;
	end
	local roleName = frame:GetClientString();

	-- �����Ѳ�����������Ϣ�������в����߸�����ʾ�����ǲ������Ի���
	local roleInfo;
	if GameSchool:GetSelfSchoolJobType() == ROLE_SCHOOL_TYPE_STUDENT_TOLUA then
		roleInfo = GameSchool:GetSelfTeacher()
	else
		roleInfo = GameSchool:GetSelfStudent( roleName )		
	end
	if not ChatFrameManager:IsNewSpeaker( roleName ) and roleInfo.Online == CLIENT_OFFLINE then
		ShowMidTips( "�����Ŀǰ������,���ܷ����µĶԻ�" );
		return;
	end

	local szFrameName = "";

	-- �ȼ���Ƿ����뱻�����Ҷ�Ӧ����������ʾ���촰��,������ʲôҲ���ͽ����ӳ���
	if ChatFrameManager:GetSpeakerFrame( roleName ) ~= nil then
		return;
	end
	
	-- ��û���뱻�����Ҷ�Ӧ�����촰�ڻ�������ʾ��
	ChatFrameManager:OpenChatFrame( roleName, CL_CHAT_WINDOW );
end
function SchoolInfoFrameSctalkBtn_OnClick()
	if MessageBoxFrame:IsShown() and MessageTitle:GetText() == "���ʦͽ��ϵ" then return end
	local frame = getglobal( "SchoolInfoFrame" );
	if frame:GetClientString() == "" then 
		ShowMidTips( "����������Ϣ" );
		SchoolInfoFrame:Hide();
		return;
	end
	SendPrivateMessage( frame:GetClientString() );
end

function SchoolInfoFrameTradeBtn_OnClick()
	if MessageBoxFrame:IsShown() and MessageTitle:GetText() == "���ʦͽ��ϵ" then return end
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then return end
	local frame = getglobal( "SchoolInfoFrame" );
	if frame:GetClientString() == "" then 
		ShowMidTips( "����������Ϣ" );
		SchoolInfoFrame:Hide();
		return;
	end

	local actor	= ActorMgr:findActorByName(  frame:GetClientString() );
	if actor == nil then 
		ShowMidTips( "�Է����߻��߲�����Ұ��Χ�ڣ����뽻��ʧ��" );
		return; 
	end

	GameExchange:inviteExchange( actor:getName() );
end
------------------------------------------------------------���ѽ���������ť���飬�����������ĸ���ť����-----------------------------------------
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
		SetGameTooltips( this:GetName(),"��ÿ��ֻ�ܸ�ѧ��дһ�ε�ʦ�ĵ�" )
	else
		SetGameTooltips( this:GetName(),"�����컹����Ϊ����ѧ��дһ�ε�ʦ�ĵ�" )
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
		--����Ҫ��ͬһ�죬����Ҫ�����ڵ�����������Ϊ������������Ÿ�����Ϣ��
		if now.year == infoTime.year and now.month == infoTime.month and now.day == infoTime.day and now.hour >= 6 then
			this:SetGrayEx( true )
		else
			this:SetGrayEx( false )
		end
	else		
		this:SetGrayEx( false )
	end
end

local MIN_SCHOOL_GIFT_GROW_LEVE = 10		--���ʦͽ�ɳ�������ͽ�ܵ���С�ɳ��ȼ�
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
		szInfo = szInfo..szColor..t_SchoolGiftLevelList[i].."���ɳ�����"
	end
	SetGameTooltips( this:GetName(), szInfo )
end
function FriendShipFrame_CharacterNameTemplateFlowerBtn_OnClick()
	local button = getglobal( this:GetParent() )
	local name = button:GetClientString()
	if name == "" or name == nil then return end	
	OpenUseFlower( 0, name )
end
-----------------------------�ɹ�����ʦͽ�������
function SchoolAddStudent( szName, szTeacherName )
	local mainplayer = ActorMgr:getMainPlayer()
	local name = szName
	if mainplayer:getName() == szName then
		name = szTeacherName
	end
	if not FriendShipFrame:IsShown() then		
		GuideEffectShow( "", "", "FriendsButton", "���Ѿ��ɹ���"..name.."����ʦ����ϵ",
						"uires\\ZhuJieMian\\1.dds", 352, 609, 60, 60, 50, 60, -9, 5, 11, -51, true );
	end
	if not FriendShipFramePrenticeBtn:IsEnable() then return end
	FriendShipFramePrenticeBtnUVAnimationTex:Show()
	FriendShipFramePrenticeBtnUVAnimationTex:SetUVAnimation( 48, true )
end

-----------------------------------------������--------------------------------

function UpdateMarryInfoList()
	for index, data in ipairs( t_requestMarryInfo ) do
		local szInfo = ""
		if data.nType == PURSUE_INFO_TYPE then
			szInfo = data.name.."����������"
		elseif data.nType == REMOVE_RELATION_TYPE then
			szInfo = "���"..data.name.."ϣ�����������¹�ϵ"
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
		szMarryInfoDesc = "\n"..mainplayer:getName()..",��ϲ���㣬����#cff08ff��һ��#n�ɣ�";
		PursueFrame_DescRich:setCenterLine( true )
		PursueFrame_DescRich:SetText( szMarryInfoDesc, 255, 229, 189 )		
		PursueFrame:SetClientString( g_szReqCharacterName );
		PursueFrame:Show();
	elseif t_requestMarryInfo[1].nType == REMOVE_RELATION_TYPE then
		RemoveRelationFramePlayerName:SetText( g_szReqCharacterName..":" )
		szMarryInfoDesc = "\n"..mainplayer:getName()..",�Բ����������ǻ���#R�ֿ���#n��\n#R�������ϵֻ�赥��������ɣ�#n";
		RemoveRelationFrame_DescRich:setCenterLine( true )
		RemoveRelationFrame_DescRich:SetText( szMarryInfoDesc, 255, 229, 189 )		
		RemoveRelationFrame:SetClientString( g_szReqCharacterName );
		RemoveRelationFrame:Show();
	end
	table.remove( t_requestMarryInfo, 1 )
end
function AddMarryInfo( szName, Type)
	--���У������µ���Ϣ����
	for i = 1, table.getn( t_requestMarryInfo ) do
		if t_requestMarryInfo[i].name == szName and t_requestMarryInfo[i].nType == Type then
			return
		end
	end
	table.insert( t_requestMarryInfo, { name = szName, nType = Type, nRelationType = GameFriendManager:getFriendInfoByName(szName).m_nIntimateColorType, } )
end
--------������
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
--------�����ϵ���
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
------���Զ���ȷ�����
local t_WiddingName = { "����ײ�", "��Լ�ײ�", "�����ײ�" }
function OpenOathUIFrame( Gender, szName, szOrderName, widdingMeal )
	OathUIFrame_DescRich:setCenterLine( true )
	local szInfo =  "#G��"..szOrderName.."������"..t_WiddingName[widdingMeal].."��#n\n".."���Ƿ�Ը������ǰ��"..( Gender == 1 and "��" or "��" ).."��Ϊ����\n����ƶ���������ۼ��ཡ����"
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
-------�����ϵ���ѵ���
function OpenRemoveRelationCostFrame( nRelationType )
	RemoveRelationCostFrame_DescRich:setCenterLine( true )
	local szName = "���"..(nRelationType == INTIMATE_COLOR_LOVERS_TOLUA and "����" or "����" ).."��Ҫ����1��#G�����������#n\n#R�������ϵֻ�赥��������ɣ�#n"
	if nRelationType == INTIMATE_COLOR_LOVERS_TOLUA then
		szName = "#Rȷ�������ǽ������̹����ĵĸ��飡#n\n�Ƿ�ȷ��������¹�ϵ��"
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
