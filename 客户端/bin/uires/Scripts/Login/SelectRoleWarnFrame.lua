-- Public
local t_warnOp = 
{
["确定删除角色"] = 
{ func = 
function () 
	LoginMgr:requestDeleteSelectedRole(); 
end },
["账号不存在"] =
{ func = 
function ()
	if LoginNameEdit:GetText() ~= "" then
		LoginNameEdit:SetClientString( LoginNameEdit:GetText() );
	end
	RegisterUserFrame:Show();
	RegisterUserFrame:SetClientUserData( 0,0 )	
end
}
};

local t_SelectRoleWarnFrameControl = { ["opDesc"] = "" };

function GetSelectRoleWarnFrameControl()
	return t_SelectRoleWarnFrameControl;
end

function Login_MessageBox( data )
	local t_fonts = {	["SelectRoleWarnFrameDescRich"]	= { value = data["text"],color = { r = 50, g = 30, b = 10 } },
					};
	util.SetRiches( t_fonts );
	
	local confirmBtn = getglobal( "SelectRoleWarnFrame_ConfirmBtn" );
	confirmBtn:SetPoint( "topleft", "SelectRoleWarnFrame", "topleft", 58, 101 );

	local cancelBtn = getglobal( "SelectRoleWarnFrame_CancelBtn" );
	cancelBtn:SetPoint( "topleft", "SelectRoleWarnFrame", "topleft", 182, 101 );
	cancelBtn:Show();
	t_SelectRoleWarnFrameControl["opDesc"] = data["opDesc"];

	if data["lock"] then
		LoginLockFrame:Show();
	else
		LoginLockFrame:Hide();
	end

	SelectRoleWarnFrame:Show();
end

function Login_MessageBoxOK( data )
	local t_fonts = {	["SelectRoleWarnFrameDescRich"]	= { value = data["text"],color = { r = 50, g = 30, b = 10 }  },
					};
	util.SetRiches( t_fonts );
	
	local confirmBtn = getglobal( "SelectRoleWarnFrame_ConfirmBtn" );
	confirmBtn:SetPoint( "topleft", "SelectRoleWarnFrame", "topleft", 120, 101 );

	local cancelBtn = getglobal( "SelectRoleWarnFrame_CancelBtn" );
	cancelBtn:Hide();

	t_SelectRoleWarnFrameControl["opDesc"] = data["opDesc"];

	if data["lock"] then
		LoginLockFrame:Show();
	else
		LoginLockFrame:Hide();
	end

	SelectRoleWarnFrame:Show();
end

-- 确定按钮
function SelectRoleWarnFrame_ConfirmBtn_OnClick()
	local t_processConfig = t_warnOp[t_SelectRoleWarnFrameControl["opDesc"]];
	if t_processConfig ~= nil then
		local processFunc = t_processConfig["func"];
		processFunc();
	end

	SelectRoleWarnFrame:Hide();
end

function SelectRoleWarnFrame_OnHide()
	t_SelectRoleWarnFrameControl["opDesc"] = "";

	local t_hideIcons = { "LoginLockFrame", };
	util.HideIcons( t_hideIcons );
end


-- Lock
function LoginLockFrame_OnLoad()
	local tex	= getglobal( this:GetName().."Tex" );
	tex:SetColor( 0, 0, 0 );
	tex:SetBlendAlpha( 0.6 );
end