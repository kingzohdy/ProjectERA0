
function UIMenuButton_OnMouseDown()
	local buttonname = this:GetName();
	--聊天频道右键菜单
	if (string.find(buttonname, "ChannelMenu", 1, true)) then
		local font			= getglobal(buttonname.."ShortcutText");
		local t_NameList	= GetPrivatePlayerNameList();
		local t_ChatInsertFrameControl = GetChatInsertFrameControl();
		local nLastChatType	= GetChatType();

		--如果是附近
		if (string.find(font:GetText(), "附近", 1, true)) then
			SetChatType( CL_CHAT_SAY );	
		--如果是队伍
		elseif (string.find(font:GetText(), "队伍", 1, true)) then
			SetChatType( CL_CHAT_TEAM ); 
		--如果是私聊
		elseif (string.find(font:GetText(), "私聊", 1, true)) then
			SetChatType( CL_CHAT_PRIVATE );		 
		--如果是公会
		elseif (string.find(font:GetText(), "公会", 1, true)) then
			SetChatType( CL_CHAT_CLAN );   
		--如果是世界
		elseif (string.find(font:GetText(), "世界", 1, true)) then
			SetChatType( CL_CHAT_WORLD );
		elseif (string.find(font:GetText(), "战场", 1, true)) then
			SetChatType( CL_CHAT_BATTLE );
		elseif (string.find(font:GetText(), "喇叭", 1, true)) then
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

	--目标玩家右键菜单
	if (string.find(buttonname, "PlayerMenu", 1, true)) then
		local font = getglobal(buttonname.."ShortcutText");

		--如果是邀请入队
		if (string.find(font:GetText(), "组队", 1, true)) then
			TeamManager:Team_OP_Add(Target:GetClientString());
			PlayerMenu:Hide();
			
		--[[如果是申请入队
		elseif (string.find(font:GetText(), "申请入队", 1, true)) then
			TeamManager:Team_OP_Apply(Target:GetClientString());
			PlayerMenu:Hide();
			--]]

		--如果是悄悄话
		elseif (string.find(font:GetText(), "加为好友", 1, true)) then
			
			PlayerMenu:Hide();

		--如果是提升队长
		elseif (string.find(font:GetText(), "提升队长", 1, true)) then

		--如果是踢出队伍
		elseif (string.find(font:GetText(), "踢出队伍", 1, true)) then

		end
	end

	--队伍队员右键菜单
	if (string.find(buttonname, "TeamMenu", 1, true)) then
		local font = getglobal(buttonname.."ShortcutText");
		local szCurSelectTeamMemberName = getglobal("g_szCurSelectTeamMemberName"); 

		--如果是离开队伍
		if (string.find(font:GetText(), "离开队伍", 1, true)) then
			TeamManager:Team_OP_Exit();
			TeamMenu:Hide();
			
		--如果是请离队伍
		elseif (string.find(font:GetText(), "请离队伍", 1, true)) then
			TeamManager:Team_OP_Kick( szCurSelectTeamMemberName );
			TeamMenu:Hide();

		--如果是转让队长
		elseif (string.find(font:GetText(), "转让队长", 1, true)) then
			TeamManager:Team_OP_CaptainChange( szCurSelectTeamMemberName );
			TeamMenu:Hide();

		--如果是解散队伍
		elseif (string.find(font:GetText(), "解散队伍", 1, true)) then
			TeamManager:Team_OP_Destroy();
			TeamMenu:Hide();

		--如果是加为好友
		elseif (string.find(font:GetText(), "加为好友", 1, true)) then
			TeamMenu:Hide();
		end
	end

	--聊天窗口玩家右键菜单
	if (string.find(buttonname, "ChatMenu", 1, true)) then
		local font = getglobal(buttonname.."ShortcutText");
		--local szCurSelectTeamMemberName = getglobal("g_szCurSelectTeamMemberName"); 

		--如果是邀请入队
		if (string.find(font:GetText(), "组队", 1, true)) then
			TeamManager:Team_OP_Add(GetCurSelectChatPlayerName());
			ChatMenu:Hide();
			
		--[[如果是申请入队
		elseif (string.find(font:GetText(), "申请入队", 1, true)) then
			TeamManager:Team_OP_Apply(GetCurSelectChatPlayerName());
			ChatMenu:Hide();--]]

		--如果是加为好友
		elseif (string.find(font:GetText(), "加为好友", 1, true)) then
			local parentFrame = getglobal( this:GetParent() );
			if not isRoleInRequestList( parentFrame:GetClientString() ) then
				GameFriendManager:addFriend( parentFrame:GetClientString(), "" );
			end
			ChatMenu:Hide();
		end
	end
	
	--[[
	--聊天频道右键菜单
	if (string.find(buttonname, "ChannelMenu", 1, true)) then
		local font			= getglobal(buttonname.."ShortcutText");
		local t_NameList	= GetPrivatePlayerNameList();
		local t_ChatInsertFrameControl = GetChatInsertFrameControl();
		local nLastChatType	= GetChatType();

		--如果是附近
		if (string.find(font:GetText(), "附近", 1, true)) then
			SetChatType( CL_CHAT_SAY );	
		--如果是队伍
		elseif (string.find(font:GetText(), "队伍", 1, true)) then
			SetChatType( CL_CHAT_TEAM ); 
		--如果是私聊
		elseif (string.find(font:GetText(), "私聊", 1, true)) then
			SetChatType( CL_CHAT_PRIVATE );		 
		--如果是公会
		elseif (string.find(font:GetText(), "公会", 1, true)) then
			SetChatType( CL_CHAT_CLAN );   
		--如果是世界
		elseif (string.find(font:GetText(), "世界", 1, true)) then
			SetChatType( CL_CHAT_WORLD );
		elseif (string.find(font:GetText(), "战场", 1, true)) then
			SetChatType( CL_CHAT_BATTLE );
		elseif (string.find(font:GetText(), "喇叭", 1, true)) then
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

	--主角头像右键菜单
	if ( string.find( buttonname, "MainPlayerRightMenu", 1, true ) ) then
		local font = getglobal( buttonname.."ShortcutText" );
		-- 如果是离开队伍
		if ( string.find( font:GetText(), "离开队伍", 1, true ) ) then
			
			TeamManager:Team_OP_Exit();
		end
		MainPlayerRightMenu:Hide();
	end
	
	if string.find(buttonname, "ClanApplyMenu", 1, true) then
		local font = getglobal(buttonname.."ShortcutText");		
		if (string.find(font:GetText(), "私聊", 1, true)) then
			ApplyJoinClan_PrivateOnClick();			
		elseif (string.find(font:GetText(), "观察玩家", 1, true)) then
			ApplyJoinClan_DetailOnClick();
		elseif (string.find(font:GetText(), "加为好友", 1, true)) then
			ApplyJoinClan_AddFriendOnClick();
		elseif (string.find(font:GetText(), "批准入会", 1, true)) then
			ApplyJoinClan_ApproveBtn_OnClick();
		elseif (string.find(font:GetText(), "拒绝入会", 1, true)) then
			ApplyJoinClan_DenialBtn_OnClick();
		end
		ClanApplyMenu:Hide();
	end

	if string.find( buttonname, "ClanBasicMenu", 1, true ) then
		local font = getglobal(buttonname.."ShortcutText");		
		if (string.find(font:GetText(), "私聊", 1, true)) then
			Clan_PrivateOnClick();			
		elseif (string.find(font:GetText(), "观察玩家", 1, true)) then
			ClanDetailOnClick();
		elseif (string.find(font:GetText(), "加为好友", 1, true)) then
			ClanAddFriendOnClick();
		elseif (string.find(font:GetText(), "组队", 1, true)) then
			ClanTeamInviteOnClick();
		elseif (string.find(font:GetText(), "设置官阶", 1, true)) then
			Clan_RaiseLVBtn_OnClick();			
		--elseif (string.find(font:GetText(), "降低官阶", 1, true)) then
		--	Clan_FallLVBtn_OnClick();
		elseif (string.find(font:GetText(), "职位转让", 1, true)) then
			Clan_PositionChangeBtn_OnClick();
		elseif (string.find(font:GetText(), "设置昵称", 1, true)) then
			Clan_SetNiChengBtn_OnClick();
		elseif (string.find(font:GetText(), "踢出公会", 1, true)) then
			Clan_DelMemberBtn_OnClick();
		end
		ClanBasicMenu:Hide();
	end

	if string.find( buttonname, "TeamBootyMenu", 1, true ) then
		local font = getglobal(buttonname.."ShortcutText");	
		if ( string.find(font:GetText(), "按需分配", 1, true) ) then
			TeamManager:changeBootyMode( TEAM_BOOTY_ROLL_TOLUA );
		elseif ( string.find(font:GetText(), "随机分配", 1, true) ) then
			TeamManager:changeBootyMode( TEAM_BOOTY_RAND_TOLUA );
		elseif (( string.find(font:GetText(), "自由拾取", 1, true) )) then
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
