
function UIMenuButton_OnMouseDown()
	local buttonname = this:GetName();
	--����Ƶ���Ҽ��˵�
	if (string.find(buttonname, "ChannelMenu", 1, true)) then
		local font			= getglobal(buttonname.."ShortcutText");
		local t_NameList	= GetPrivatePlayerNameList();
		local t_ChatInsertFrameControl = GetChatInsertFrameControl();
		local nLastChatType	= GetChatType();

		--����Ǹ���
		if (string.find(font:GetText(), "����", 1, true)) then
			SetChatType( CL_CHAT_SAY );	
		--����Ƕ���
		elseif (string.find(font:GetText(), "����", 1, true)) then
			SetChatType( CL_CHAT_TEAM ); 
		--�����˽��
		elseif (string.find(font:GetText(), "˽��", 1, true)) then
			SetChatType( CL_CHAT_PRIVATE );		 
		--����ǹ���
		elseif (string.find(font:GetText(), "����", 1, true)) then
			SetChatType( CL_CHAT_CLAN );   
		--���������
		elseif (string.find(font:GetText(), "����", 1, true)) then
			SetChatType( CL_CHAT_WORLD );
		elseif (string.find(font:GetText(), "ս��", 1, true)) then
			SetChatType( CL_CHAT_BATTLE );
		elseif (string.find(font:GetText(), "����", 1, true)) then
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
		ChannelMenu:Hide();
		ChatInsertFrame_OnShow();
		local nCurChatType = GetChatType();
		if nLastChatType == CL_CHAT_PRIVATE and nCurChatType ~= CL_CHAT_PRIVATE then
			CSChatInsert:SetText( CSChatInsert:GetText() );
		end

		SetFocusFrame("CSChatInsert");
	end
	
end

function UIMenuButton_OnClick()
	local buttonname = this:GetName();

	--Ŀ������Ҽ��˵�
	if (string.find(buttonname, "PlayerMenu", 1, true)) then
		local font = getglobal(buttonname.."ShortcutText");

		--������������
		if (string.find(font:GetText(), "���", 1, true)) then
			TeamManager:Team_OP_Add(Target:GetClientString());
			PlayerMenu:Hide();
			
		--[[������������
		elseif (string.find(font:GetText(), "�������", 1, true)) then
			TeamManager:Team_OP_Apply(Target:GetClientString());
			PlayerMenu:Hide();
			--]]

		--��������Ļ�
		elseif (string.find(font:GetText(), "��Ϊ����", 1, true)) then
			
			PlayerMenu:Hide();

		--����������ӳ�
		elseif (string.find(font:GetText(), "�����ӳ�", 1, true)) then

		--������߳�����
		elseif (string.find(font:GetText(), "�߳�����", 1, true)) then

		end
	end

	--�����Ա�Ҽ��˵�
	if (string.find(buttonname, "TeamMenu", 1, true)) then
		local font = getglobal(buttonname.."ShortcutText");
		local szCurSelectTeamMemberName = getglobal("g_szCurSelectTeamMemberName"); 

		--������뿪����
		if (string.find(font:GetText(), "�뿪����", 1, true)) then
			TeamManager:Team_OP_Exit();
			TeamMenu:Hide();
			
		--������������
		elseif (string.find(font:GetText(), "�������", 1, true)) then
			TeamManager:Team_OP_Kick( szCurSelectTeamMemberName );
			TeamMenu:Hide();

		--�����ת�öӳ�
		elseif (string.find(font:GetText(), "ת�öӳ�", 1, true)) then
			TeamManager:Team_OP_CaptainChange( szCurSelectTeamMemberName );
			TeamMenu:Hide();

		--����ǽ�ɢ����
		elseif (string.find(font:GetText(), "��ɢ����", 1, true)) then
			TeamManager:Team_OP_Destroy();
			TeamMenu:Hide();

		--����Ǽ�Ϊ����
		elseif (string.find(font:GetText(), "��Ϊ����", 1, true)) then
			TeamMenu:Hide();
		end
	end

	--���촰������Ҽ��˵�
	if (string.find(buttonname, "ChatMenu", 1, true)) then
		local font = getglobal(buttonname.."ShortcutText");
		--local szCurSelectTeamMemberName = getglobal("g_szCurSelectTeamMemberName"); 

		--������������
		if (string.find(font:GetText(), "���", 1, true)) then
			TeamManager:Team_OP_Add(GetCurSelectChatPlayerName());
			ChatMenu:Hide();
			
		--[[������������
		elseif (string.find(font:GetText(), "�������", 1, true)) then
			TeamManager:Team_OP_Apply(GetCurSelectChatPlayerName());
			ChatMenu:Hide();--]]

		--����Ǽ�Ϊ����
		elseif (string.find(font:GetText(), "��Ϊ����", 1, true)) then
			local parentFrame = getglobal( this:GetParent() );
			if not isRoleInRequestList( parentFrame:GetClientString() ) then
				GameFriendManager:addFriend( parentFrame:GetClientString(), "" );
			end
			ChatMenu:Hide();
		end
	end
	
	--[[
	--����Ƶ���Ҽ��˵�
	if (string.find(buttonname, "ChannelMenu", 1, true)) then
		local font			= getglobal(buttonname.."ShortcutText");
		local t_NameList	= GetPrivatePlayerNameList();
		local t_ChatInsertFrameControl = GetChatInsertFrameControl();
		local nLastChatType	= GetChatType();

		--����Ǹ���
		if (string.find(font:GetText(), "����", 1, true)) then
			SetChatType( CL_CHAT_SAY );	
		--����Ƕ���
		elseif (string.find(font:GetText(), "����", 1, true)) then
			SetChatType( CL_CHAT_TEAM ); 
		--�����˽��
		elseif (string.find(font:GetText(), "˽��", 1, true)) then
			SetChatType( CL_CHAT_PRIVATE );		 
		--����ǹ���
		elseif (string.find(font:GetText(), "����", 1, true)) then
			SetChatType( CL_CHAT_CLAN );   
		--���������
		elseif (string.find(font:GetText(), "����", 1, true)) then
			SetChatType( CL_CHAT_WORLD );
		elseif (string.find(font:GetText(), "ս��", 1, true)) then
			SetChatType( CL_CHAT_BATTLE );
		elseif (string.find(font:GetText(), "����", 1, true)) then
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
		ChannelMenu:Hide();
		ChatInsertFrame_OnShow();
		local nCurChatType = GetChatType();
		if nLastChatType == CL_CHAT_PRIVATE and nCurChatType ~= CL_CHAT_PRIVATE then
			CSChatInsert:SetText( CSChatInsert:GetText() );
		end

		SetFocusFrame("CSChatInsert");
	end
	--]]

	--����ͷ���Ҽ��˵�
	if ( string.find( buttonname, "MainPlayerRightMenu", 1, true ) ) then
		local font = getglobal( buttonname.."ShortcutText" );
		-- ������뿪����
		if ( string.find( font:GetText(), "�뿪����", 1, true ) ) then
			
			TeamManager:Team_OP_Exit();
		end
		MainPlayerRightMenu:Hide();
	end
	
	if string.find(buttonname, "ClanApplyMenu", 1, true) then
		local font = getglobal(buttonname.."ShortcutText");		
		if (string.find(font:GetText(), "˽��", 1, true)) then
			ApplyJoinClan_PrivateOnClick();			
		elseif (string.find(font:GetText(), "�۲����", 1, true)) then
			ApplyJoinClan_DetailOnClick();
		elseif (string.find(font:GetText(), "��Ϊ����", 1, true)) then
			ApplyJoinClan_AddFriendOnClick();
		elseif (string.find(font:GetText(), "��׼���", 1, true)) then
			ApplyJoinClan_ApproveBtn_OnClick();
		elseif (string.find(font:GetText(), "�ܾ����", 1, true)) then
			ApplyJoinClan_DenialBtn_OnClick();
		end
		ClanApplyMenu:Hide();
	end

	if string.find( buttonname, "ClanBasicMenu", 1, true ) then
		local font = getglobal(buttonname.."ShortcutText");		
		if (string.find(font:GetText(), "˽��", 1, true)) then
			Clan_PrivateOnClick();			
		elseif (string.find(font:GetText(), "�۲����", 1, true)) then
			ClanDetailOnClick();
		elseif (string.find(font:GetText(), "��Ϊ����", 1, true)) then
			ClanAddFriendOnClick();
		elseif (string.find(font:GetText(), "���", 1, true)) then
			ClanTeamInviteOnClick();
		elseif (string.find(font:GetText(), "���ùٽ�", 1, true)) then
			Clan_RaiseLVBtn_OnClick();			
		--elseif (string.find(font:GetText(), "���͹ٽ�", 1, true)) then
		--	Clan_FallLVBtn_OnClick();
		elseif (string.find(font:GetText(), "ְλת��", 1, true)) then
			Clan_PositionChangeBtn_OnClick();
		elseif (string.find(font:GetText(), "�����ǳ�", 1, true)) then
			Clan_SetNiChengBtn_OnClick();
		elseif (string.find(font:GetText(), "�߳�����", 1, true)) then
			Clan_DelMemberBtn_OnClick();
		end
		ClanBasicMenu:Hide();
	end

	if string.find( buttonname, "TeamBootyMenu", 1, true ) then
		local font = getglobal(buttonname.."ShortcutText");	
		if ( string.find(font:GetText(), "�������", 1, true) ) then
			TeamManager:changeBootyMode( TEAM_BOOTY_ROLL_TOLUA );
		elseif ( string.find(font:GetText(), "�������", 1, true) ) then
			TeamManager:changeBootyMode( TEAM_BOOTY_RAND_TOLUA );
		elseif (( string.find(font:GetText(), "����ʰȡ", 1, true) )) then
			TeamManager:changeBootyMode( TEAM_BOOTY_ALL_TOLUA );
		end
		MainPlayerRightMenu:Hide();
		TeamBootyMenu:Hide();
	end
end

function UIMenuButton_OnEnter()
	local buttonname = this:GetName();
	if (string.find(buttonname, "PlayerMenu", 1, true)) then
		local hiLightTex = getglobal("PlayerMenu".."HilightTexture");
		hiLightTex:SetSize(this:GetWidth()-17, 25);
		hiLightTex:SetPoint("center", buttonname, "center", 0, 0);
		hiLightTex:Show();
	elseif (string.find(buttonname, "PlayerPetMenu", 1, true)) then
		local hiLightTex = getglobal("PlayerPetMenu".."HilightTexture");
		hiLightTex:SetSize(PlayerPetMenu:GetWidth()-12, 25);
		hiLightTex:SetPoint("center", buttonname, "center", 0, 0);
		hiLightTex:Show();
	elseif (string.find(buttonname, "ChatMenu", 1, true)) then
		local hiLightTex = getglobal("ChatMenu".."HilightTexture");
		hiLightTex:SetSize(ChatMenu:GetWidth()-17, 25);
		hiLightTex:SetPoint("center", buttonname, "center", 0, 0);
		hiLightTex:Show();
	elseif (string.find(buttonname, "TeamMenu", 1, true)) then
		local hiLightTex = getglobal("TeamMenu".."HilightTexture");
		hiLightTex:SetSize(TeamMenu:GetWidth()-17, 25);
		hiLightTex:SetPoint("center", buttonname, "center", 0, 0);
		hiLightTex:Show();
	elseif (string.find(buttonname, "ChannelMenu", 1, true)) then
		local hiLightTex = getglobal("ChannelMenu".."HilightTexture");
		hiLightTex:SetSize( ChannelMenu:GetWidth()-2, 17 );
		hiLightTex:SetPoint("center", buttonname, "center", 0, 0);
		hiLightTex:Show();
	elseif ( string.find( buttonname, "MainPlayerRightMenu", 1, true ) ) then
		local hiLightTex = getglobal( "MainPlayerRightMenu" .. "HilightTexture" );
		hiLightTex:SetSize( MainPlayerRightMenu:GetWidth() - 2, 25 );
		hiLightTex:SetPoint( "center", buttonname, "center", 0, 0 );
		hiLightTex:Show( );
	elseif ( string.find( buttonname, "ClanApplyMenu", 1, true ) ) then
		local hiLightTex = getglobal( "ClanApplyMenu" .. "HilightTexture" );
		hiLightTex:SetSize( ClanApplyMenu:GetWidth() - 2, 25 );
		hiLightTex:SetPoint( "center", buttonname, "center", 0, 0 );
		hiLightTex:Show( );
	elseif ( string.find( buttonname, "ClanBasicMenu", 1, true ) ) then	
		local hiLightTex = getglobal( "ClanBasicMenu" .. "HilightTexture" );
		hiLightTex:SetSize( ClanBasicMenu:GetWidth() - 2, 25 );
		hiLightTex:SetPoint( "center", buttonname, "center", 0, 0 );
		hiLightTex:Show( );
	elseif ( string.find( buttonname,"TeamBootyMenu", 1,true ) ) then
		local hiLightTex = getglobal( "TeamBootyMenu" .. "HilightTexture" );
		hiLightTex:SetSize( TeamBootyMenu:GetWidth() - 2, 25 );
		hiLightTex:SetPoint( "center", buttonname, "center", 0, 0 );
		hiLightTex:Show( );
	end
end

function  UIMenuButton_OnLeave()
	
end
