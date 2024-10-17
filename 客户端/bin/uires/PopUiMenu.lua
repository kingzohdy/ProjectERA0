
local nUIMenuButton_Max = 32;

--Ŀ������Ҽ��˵�
function  PlayerMenu_OnFocusLost()
	this:Hide();
end

function  PlayerMenu_OnShow()
	ClearPlayerMenu();
	 for i=1,2 do
		local button = getglobal("PlayerMenu".."Button"..i);
		if (i==1) then
			button:SetPoint("top", "PlayerMenu", "top", 0, 2);
		else
			button:SetPoint("top", "PlayerMenu", "top", 0, 2 + (i-1)*(16 + 6));
		end
		local font = getglobal("PlayerMenu".."Button"..i.."ShortcutText");

		if i == 1 then
			font:SetText("���");
		elseif i == 2 then
			font:SetText("��Ϊ����");
		end
		font:Show();
		button:SetSize(80,16);
		button:Show();

	end
	PlayerMenu:SetPoint("topleft", PlayerMenu:GetClientString(), "bottomleft", 20, 0);
	--PlayerMenu:SetSize(80, (MenuNum*(16+6))+20);
	PlayerMenu:SetSize(80,38);
	--PlayerMenu:Show();
	SetFocusFrame("PlayerMenu");
end

function PlayerMenu_OnEnter()
	--[[
	for i=1,3 do
		local font = getglobal("PlayerMenu".."Button"..i.."ShortcutText");
		font:SetText("���");
		MessageBox("����","���");

		--if (i==1) then
		--	button:SetPoint("top", "PlayerMenu", "top", 0, 12);
		--else
		--	button:SetPoint("top", "PlayerMenu", "top", 0, 12 + (i-1)*(16 + 6));
		--end


		font:Show();
	end
	--]]
end
--Ŀ������Ҽ��˵�
function  PlayerPetMenu_OnFocusLost()
	this:Hide();
end

function ClearPlayerPetMenu()
	for i=1, 1 do
		local button = getglobal("PlayerPetMenu".."Button"..i);
		button:Hide();
		local font = getglobal("PlayerPetMenu".."Button"..i.."ShortcutText");
		font:SetText("");
		font:Hide();
		--font:SetTextColor(255, 255, 255);

		local hiLightTex = getglobal("PlayerPetMenu".."HilightTexture");
		hiLightTex:Hide();
	end
end

function  PlayerPetMenu_OnShow()
	ClearPlayerPetMenu();
	for i=1,1 do
		local button = getglobal("PlayerPetMenu".."Button"..i);
		if (i==1) then
			button:SetPoint("top", "PlayerPetMenu", "top", 0, 12);
		else
			button:SetPoint("top", "PlayerPetMenu", "top", 0, 12 + (i-1)*(16 + 6));
		end
		local font = getglobal("PlayerPetMenu".."Button"..i.."ShortcutText");

		if i == 1 then
			font:SetText("�۲����");
		end
		font:Show();
		button:SetSize(70,16);
		button:Show();
	end
	PlayerPetMenu:SetPoint("topleft", PlayerPetMenu:GetClientString(), "bottomleft", 20, 0);
	PlayerPetMenu:SetSize(40,40);
	SetFocusFrame("PlayerPetMenu");
end

function PlayerPetMenu_OnEnter()

end

function ClearPlayerMenu()
	for i=1, nUIMenuButton_Max do
		local button = getglobal("PlayerMenu".."Button"..i);
		button:Hide();
		local font = getglobal("PlayerMenu".."Button"..i.."ShortcutText");
		font:SetText("");
		font:Hide();
		--font:SetTextColor(255, 255, 255);

		local hiLightTex = getglobal("PlayerMenu".."HilightTexture");
		hiLightTex:Hide();
	end
end

function PopPlayerMenu(name, poppos)
	SetFocusFrame("PlayerMenu");
	ClearPlayerMenu();

	local bCanAddFriend = false;
	local bCanWisper = true;
	--local bCanInviteTeam = Team:CanInvite(name);
	local bCanInviteTeam =true;
	local bCanBlacklist = false;
	local bCanKick = Team:CanKick(name);
	local bCanChangeLeader = Team:CanChangeLeader(name);
	local bCanQuitTeam = Team:CanQuitTeam(name);


	local MenuNum = 0;

	local AddFriendPos = 1;
	local WisperPos = 2;
	local InvitePos = 3;
	local Blacklist = 4;
	local KickPos = 5;
	local ChgLeaderPos = 6;
	local QuitTeamPos = 7

	if(bCanAddFriend == true) then
		MenuNum = MenuNum + 1;
		AddFriendPos = MenuNum;
	end

	if(bCanWisper == true) then
		MenuNum = MenuNum + 1;
		WisperPos = MenuNum;
	end

	if(bCanInviteTeam == true) then
		MenuNum = MenuNum + 1;
		InvitePos = MenuNum;
	end

	if(bCanBlacklist == true) then
		MenuNum = MenuNum + 1;
		BlacklistPos = MenuNum;
	end

	if(bCanKick == true) then
		MenuNum = MenuNum + 1;
		KickPos = MenuNum;
	end

	if(bCanChangeLeader == true) then
		MenuNum = MenuNum + 1;
		ChgLeaderPos = MenuNum;
	end

	if(bCanQuitTeam == true) then
		MenuNum = MenuNum + 1;
		QuitTeamPos = MenuNum;
	end

	if(poppos == "MousePos") then
		PlayerMenu:SetPoint("topleft", "UIClient", "topleft", GetCursorPosX(), GetCursorPosY());
	else
		PlayerMenu:SetPoint("topleft", this:GetName(), "bottomleft", 20, 0);
	end;

	PlayerMenu:SetSize(80, (MenuNum*(16+6))+20);
	for i=1, MenuNum, 1 do
		local button = getglobal("PlayerMenu".."Button"..i);

		if (i==1) then
			button:SetPoint("top", "PlayerMenu", "top", 0, 12);
		else
			button:SetPoint("top", "PlayerMenu", "top", 0, 12 + (i-1)*(16 + 6));
		end

		button:SetSize(80, 16);
		local font = getglobal("PlayerMenu".."Button"..i.."ShortcutText");
		local bHide = false;
		if(i == AddFriendPos and bCanAddFriend == true ) then
			font:SetText("��Ϊ����");
			font:Show();
			button:Show();
		elseif(i == WisperPos and bCanWisper == true) then
			font:SetText("���Ļ�");
			font:Show();
			button:Show();
		elseif(i == InvitePos and bCanInviteTeam == true) then
			font:SetText("���");
			font:Show();
			button:Show();
		elseif(i == BlacklistPos and bCanBlacklist == true) then
			font:SetText("����");
			font:Show();
			button:Show();
		elseif(i == KickPos and bCanKick == true) then
			font:SetText("�߳�����");
			font:Show();
			button:Show();
		elseif(i == ChgLeaderPos and bCanChangeLeader == true) then
			font:SetText("�����ӳ�");
			font:Show();
			button:Show();
		elseif(i == QuitTeamPos and bCanQuitTeam == true) then
			font:SetText("�˳�����");
			font:Show();
			button:Show();
		end;


	end
	PlayerMenu:Show();


end



--�����Ա�Ҽ��˵�
function  TeamMenu_OnFocusLost()
	this:Hide();
end

function  TeamMenu_OnEnter()
end

function ClearTeamMenu()
	for i=1, nUIMenuButton_Max do
		local button = getglobal("TeamMenu".."Button"..i);
		button:Hide();
		local font = getglobal("TeamMenu".."Button"..i.."ShortcutText");
		font:SetText("");
		font:Hide();
		--font:SetTextColor(255, 255, 255);

		local hiLightTex = getglobal("TeamMenu".."HilightTexture");
		hiLightTex:Hide();
	end
end

function  TeamMenu_OnShow()
	ClearTeamMenu();
	local nMainPlayer = TeamMenu:GetClientUserData(3);

	local player = ActorMgr:getMainPlayer();
	local bCaptain = player:isCaptain();


	--����ǵ�����
	if  nMainPlayer == getglobal("g_nMainPlayerYes") then
		if bCaptain then
			for i=1,1 do
				local button = getglobal("TeamMenu".."Button"..i);
				if (i==1) then
					button:SetPoint("top", "TeamMenu", "top", 0, 12);
				else
					button:SetPoint("top", "TeamMenu", "top", 0, 12 + (i-1)*(16 + 6));
				end
				local font = getglobal("TeamMenu".."Button"..i.."ShortcutText");

				if i == 1 then
					font:SetText("��ɢ����");
				end
				font:Show();
				button:SetSize(80,16);
				button:Show();
			end
		else
			for i=1,1 do
				local button = getglobal("TeamMenu".."Button"..i);
				if (i==1) then
					button:SetPoint("top", "TeamMenu", "top", 0, 12);
				else
					button:SetPoint("top", "TeamMenu", "top", 0, 12 + (i-1)*(16 + 6));
				end
				local font = getglobal("TeamMenu".."Button"..i.."ShortcutText");

				if i == 1 then
					font:SetText("�뿪����");
				end
				font:Show();
				button:SetSize(80,16);
				button:Show();
			end
		end
		TeamMenu:SetSize(80,30);
		TeamMenu:SetPoint("topleft", TeamMenu:GetClientString(), "bottomleft", 20, 0);
		SetFocusFrame("TeamMenu");
		TeamMenu:SetClientUserData(3,getglobal("g_nMainPlayerNo"));
	--����ǵ���������
	else
		if bCaptain then
			 for i=1,3 do
				local button = getglobal("TeamMenu".."Button"..i);
				if (i==1) then
					button:SetPoint("top", "TeamMenu", "top", 0, 12);
				else
					button:SetPoint("top", "TeamMenu", "top", 0, 12 + (i-1)*(16 + 6));
				end
				local font = getglobal("TeamMenu".."Button"..i.."ShortcutText");

				if i == 1 then
					font:SetText("�������");
				elseif i == 2 then
					font:SetText("ת�öӳ�");
				elseif i == 3 then
					font:SetText("��Ϊ����")
				end
				font:Show();
				button:SetSize(80,16);
				button:Show();

			end
			TeamMenu:SetSize(80,80);
		else
			 for i=1,1 do
				local button = getglobal("TeamMenu".."Button"..i);
				if (i==1) then
					button:SetPoint("top", "TeamMenu", "top", 0, 12);
				else
					button:SetPoint("top", "TeamMenu", "top", 0, 12 + (i-1)*(16 + 6));
				end
				local font = getglobal("TeamMenu".."Button"..i.."ShortcutText");

				if i == 1 then
					font:SetText("��Ϊ����")
				end
				font:Show();
				button:SetSize(80,16);
				button:Show();

			end
			TeamMenu:SetSize(80,30);
		end
		TeamMenu:SetPoint("topleft", TeamMenu:GetClientString(), "bottomleft", 20, 0);
		--TeamMenu:SetSize(80, (MenuNum*(16+6))+20);
		--TeamMenu:SetSize(80,135);
		--TeamMenu:Show();
		SetFocusFrame("TeamMenu");
	end
end

--���촰������Ҽ��˵�
function ChatMenu_OnFocusLost()
	this:Hide()
end

function ChatMenu_OnEnter()
end

function ClearChatMenu()
	for i=1, nUIMenuButton_Max do
		local button = getglobal("ChatMenu".."Button"..i);
		button:Hide();
		local font = getglobal("ChatMenu".."Button"..i.."ShortcutText");
		font:SetText("");
		font:Hide();
		--font:SetTextColor(255, 255, 255);

		local hiLightTex = getglobal("ChatMenu".."HilightTexture");
		hiLightTex:Hide();
	end
end

function ChatMenu_OnShow()
	ClearChatMenu();
	for i=1,2 do
		local button = getglobal("ChatMenu".."Button"..i);
		if (i==1) then
			button:SetPoint("top", "ChatMenu", "top", 0, 2);
		else
			button:SetPoint("top", "ChatMenu", "top", 0, 2 + (i-1)*(16 + 6));
		end

		local font = getglobal("ChatMenu".."Button"..i.."ShortcutText");
		if i == 1 then
			font:SetText("���");
		elseif i == 2 then
			font:SetText("��Ϊ����");
		end

		font:Show();
		button:SetSize(80,16);
		button:Show();
	end

	ChatMenu:SetSize(80,38);
	ChatMenu:SetPoint("Bottomleft", "PhotoFrame", "topleft", GetCursorPosX(), GetCursorPosY());
	SetFocusFrame("ChatMenu");
end

function ClearMenu()
	for i=1, nUIMenuButton_Max do
		local button = getglobal( this:GetName() .. "Button" .. i );
		button:Hide();
		local font = getglobal( this:GetName() .. "Button" .. i .. "ShortcutText" );
		font:SetText( "" );
		font:Hide();

		local hiLightTex = getglobal( this:GetName() .. "HilightTexture");
		hiLightTex:Hide();
	end
end

--����Ƶ���Ҽ��˵�
function  ChannelMenu_OnFocusLost()
	this:Hide();
end

function  ChannelMenu_OnEnter()
end

local MAX_CHANNEL_MENU_BUTTON = 9;
function ClearChannelMenu()
	for i = 1, MAX_CHANNEL_MENU_BUTTON do
		local button = getglobal( "ChannelMenuButton" .. i );
		button:Hide();

		local font = getglobal( button:GetName() .. "ShortcutText" );
		font:SetText( "" );
		font:Hide();
	end

	local hiLightTex = getglobal( "ChannelMenuHilightTexture");
	hiLightTex:Hide();
end

local t_channelMenuOp = { "����	/f", "����	/p", "˽��	/m", "����	/g", "����	/w", "����	/l" };
function  ChannelMenu_OnShow()
	ClearChannelMenu();
	local t_NameList = GetPrivatePlayerNameList();
	local nBtnIndex = 1;
	for i = 1, table.getn( t_channelMenuOp ) do
		local button = getglobal("ChannelMenu".."Button"..nBtnIndex);
		local font = getglobal( button:GetName() .. "ShortcutText" );
		if ( nBtnIndex == 1 ) then
			button:SetPoint("top", "ChannelMenu", "top", 0, 12);
		else
			button:SetPoint("top", "ChannelMenu", "top", 0, 12 + (nBtnIndex-1)*(16 + 6));
		end
		
		font:SetText( t_channelMenuOp[i] );
		font:Show();
		button:SetSize(50,16);
		button:Show();
		nBtnIndex = nBtnIndex + 1;
	end

	if GameBattle:isRun() then
		local button = getglobal("ChannelMenu".."Button"..nBtnIndex);
		local font = getglobal( button:GetName() .. "ShortcutText" );
		button:SetPoint("top", "ChannelMenu", "top", 0, 12 + (nBtnIndex-1)*(16 + 6));
		font:SetText( "ս��	/b" );
		font:Show();
		button:SetSize(50,16);
		button:Show();

		nBtnIndex = nBtnIndex + 1;
	end
	
	for i = 1, table.getn(t_NameList)  do
		local button = getglobal("ChannelMenu".."Button"..nBtnIndex);
		local font = getglobal( button:GetName() .. "ShortcutText" );

		button:SetPoint("top", "ChannelMenu", "top", 0, 12 + (nBtnIndex-1)*(16 + 6));
		
		font:SetText( t_NameList[i] );
		font:Show();
		button:SetSize(50,16);
		button:Show();
		nBtnIndex = nBtnIndex + 1;
	end

	local nWidth = 60;
	for i = 1, 3 do
		if nWidth < GameTooltipRichText:GetTextExtentWidth( t_NameList[i] ) then
			nWidth = GameTooltipRichText:GetTextExtentWidth( t_NameList[i] );
		end
	end
	--ChannelMenu:SetSize( nWidth+5,120 + table.getn(t_NameList)*24 );
	ChannelMenu:SetSize( nWidth+5, ( nBtnIndex - 1 ) * 24 );
	ChannelMenu:SetPoint("BottomLeft", ChannelMenu:GetClientString(), "TopLeft", 0, 0);
	SetFocusFrame("ChannelMenu");
end



function ClearClanApplyMenu()
	for i = 1, 5 do
		local button = getglobal( "ClanApplyMenu" .. "Button" .. i );
		button:Hide();

		local font = getglobal( "ClanApplyMenu" .. "Button" .. i .. "ShortcutText" );
		font:SetText( "" );
		font:Hide();

		local hiLightTex = getglobal( "ClanApplyMenu" .. "HilightTexture");
		hiLightTex:Hide();
	end
end

function  ClanApplyMenu_OnFocusLost()
	this:Hide();
end

function  ClanApplyMenu_OnEnter()
end

function ClanApplyMenu_OnShow()
	ClearClanApplyMenu();
	for i=1,5 do
		local button = getglobal("ClanApplyMenu".."Button"..i);
		if (i==1) then
			button:SetPoint("top", "ClanApplyMenu", "top", 0, 12);
		else
			button:SetPoint("top", "ClanApplyMenu", "top", 0, 12 + (i-1)*(16 + 6));
		end

		local font = getglobal("ClanApplyMenu".."Button"..i.."ShortcutText");
		if i == 1 then
			font:SetText("˽��");
		elseif i == 2 then
			font:SetText("�۲����");
		elseif i == 3 then
			font:SetText("��Ϊ����");
		elseif i == 4 then
			font:SetText("��׼���");
		elseif i == 5 then
			font:SetText("�ܾ����");
		end
		font:Show();
		button:SetSize(60,16);
		button:Show();
	end

	ClanApplyMenu:SetSize( 80,120 );
	ClanApplyMenu:SetPoint("topLeft", "UIParent", "topleft", GetCursorPosX() ,GetCursorPosY() );
	SetFocusFrame("ClanApplyMenu");
end


function ClearClanBasicMenu()
	for i = 1, 10 do
		local button = getglobal( "ClanBasicMenu" .. "Button" .. i );
		button:Hide();

		local font = getglobal( "ClanBasicMenu" .. "Button" .. i .. "ShortcutText" );
		font:SetText( "" );
		font:Hide();

		local hiLightTex = getglobal( "ClanBasicMenu" .. "HilightTexture");
		hiLightTex:Hide();
	end
end

function ClanBasicMenu_OnShow()
	ClearClanBasicMenu();
	--����Ȩ��Menu
	local t_AllRights = {
		{ desc="˽��", hasRight = true },
		{ desc="�۲����", hasRight = true },
		{ desc="��Ϊ����", hasRight = true },
		{ desc="���", hasRight = true },
		{ desc="���ùٽ�", hasRight = CheckOperateRight() },
		{ desc="ְλת��", hasRight = CheckOperateRight() },
		{ desc="�����ǳ�", hasRight = CheckClanGroupLeaderRight() },
		{ desc="�߳�����", hasRight = CheckClanGroupLeaderRight() },
	}
	--ͳ��ӵ�е�Ȩ��
	local t_Rights = {}
	for i = 1, table.getn( t_AllRights ) do
		if t_AllRights[i].hasRight then
			table.insert( t_Rights, t_AllRights[i].desc )
		end
	end
	for i=1, table.getn( t_Rights ) do
		local button = getglobal("ClanBasicMenu".."Button"..i);
		if (i==1) then
			button:SetPoint("top", "ClanBasicMenu", "top", 0, 2);
		else
			button:SetPoint( "top", "ClanBasicMenu", "top", 0, 2 + (i-1)*22 );
		end

		local font = getglobal("ClanBasicMenu".."Button"..i.."ShortcutText");
		font:SetText( t_Rights[i] );
		font:Show();
		button:SetSize(60,22);
		button:Show();
	end
	ClanBasicMenu:SetSize( 80,6+22*table.getn( t_Rights ) );
	ClanBasicMenu:SetPoint("topLeft", "UIParent", "topleft", GetCursorPosX() ,GetCursorPosY() );
	SetFocusFrame("ClanBasicMenu");
end

function TeamBootyMenu_OnFocusLost()
	this:Hide();
end

local t_TeamBootyText = { "�������","�������","����ʰȡ" };
function TeamBootyMenu_OnLoad()
	for i=1,3 do
		local button = getglobal("TeamBootyMenu".."Button"..i);
		button:SetPoint("top", "TeamBootyMenu", "top", 0, 15+(i-1)*22 );
		local font = getglobal("TeamBootyMenuButton"..i.."ShortcutText");
		font:SetText( t_TeamBootyText[i] );
		font:SetTextColor(145, 125, 55);
		font:Show();
		button:Show();
		button:SetSize(60,16);
	end
	this:SetSize( 80,86 );
end
local t_TeamBootyMode = { [TEAM_BOOTY_RAND_TOLUA] = 2,[TEAM_BOOTY_ROLL_TOLUA] = 1,[TEAM_BOOTY_ALL_TOLUA] = 3 };
function TeamBootyMenu_OnShow()
	for i = 1, 3 do
		local bootyBtnTex = getglobal("TeamBootyMenuButton"..i.."Texture");
		bootyBtnTex:Hide();
	end
	local teamInfo = TeamManager:getTeamInfo();
	local btnTex = getglobal("TeamBootyMenuButton"..t_TeamBootyMode[teamInfo.TeamBootyMode].."Texture");
	btnTex:Show();

	local hiLightTex = getglobal( "TeamBootyMenuHilightTexture");
	hiLightTex:Hide();
	--TeamBootyMenu:SetPoint("topLeft", "UIParent", "topleft", GetCursorPosX() ,GetCursorPosY() );
	SetFocusFrame("TeamBootyMenu");
end

function ShowTeamBootyMenu( szBtnName )
	TeamBootyMenu:Show();
	local btn = getglobal( szBtnName );
	local parent = getglobal( btn:GetParent() );
	TeamBootyMenu:SetPoint("topleft", "UIParent", "topleft",parent:GetRealRight(),parent:GetRealTop() );
end