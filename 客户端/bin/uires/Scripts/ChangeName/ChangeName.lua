function CallScript_ShowChangeNameFrame( nActorId )
	local frame = getglobal( "ChangeRoleNameFrame" );
	if frame:IsShown() then
		return;
	end

	frame:Show();
end

function ChangeRoleNameFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 0, 0 );
	local edit	= getglobal( this:GetName().."_NameEdit" );
	edit:Clear();
	SetFocusFrame( edit:GetName() );
end

function ChangeRoleNameFrame_OnHide()
end

function ChangeRoleNameFrame_ConfirmBtn_OnClick()
	local edit		= getglobal( this:GetParent().."_NameEdit" );
	local szText	= edit:GetText();
	if szText == "" then
		ShowMidTips( "请输入名字" );
		return;
	end

	local mainplayer = ActorMgr:getMainPlayer();
	mainplayer:ChangeRoleName( szText );

	local frame = getglobal( "ChangeRoleNameFrame" );
	frame:Hide();
end

-- 改公会名字
function CallScript_ShowChangeClanNameFrame( nActorId )
	local frame = getglobal( "ChangeClanNameFrame" );
	if frame:IsShown() then
		return;
	end

	frame:Show();
end

function ChangeClanNameFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 0, 0 );
	local edit	= getglobal( this:GetName().."_NameEdit" );
	edit:Clear();
	SetFocusFrame( edit:GetName() );
end

function ChangeClanNameFrame_ConfirmBtn_OnClick()
	local edit		= getglobal( this:GetParent().."_NameEdit" );
	local szText	= edit:GetText();
	if szText == "" then
		ShowMidTips( "请输入公会名字" );
		return;
	end

	local mainplayer = ActorMgr:getMainPlayer();
	mainplayer:ChangeClanName( szText );

	local frame = getglobal( "ChangeClanNameFrame" );
	frame:Hide();
end