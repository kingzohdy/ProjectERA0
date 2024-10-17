local szCaptainName = nil; --队长名字


local t_curInvitePlayer = { name = "", };

t_curInvitePlayer["clear"] = 
function ( self )
	t_curInvitePlayer["name"] = "";
end

function GetCurInvitePlayer()
	return t_curInvitePlayer;
end

function SetCurInvitePlayer( data )
	if t_curInvitePlayer["name"] ~= "" then
		TeamManager:Team_OP_InviteRefuse( t_curInvitePlayer["name"]  );
	end

	t_curInvitePlayer = data;
end

function  TroopInviteFrameOnShow()
	--InvitePlayerNameFont:SetText( t_curInvitePlayer["name"].."  邀请您加入他的队伍");
end

function  TroopInviteFrameOnLoad()
end

function  TroopInviteFrameOnEvent()
end

--拒绝邀请按钮
function  RufuseOneButton_OnClick()
	TeamManager:Team_OP_InviteRefuse( t_curInvitePlayer["name"]  );
	TroopInviteFrame:Hide();
end

--同意邀请按钮
function AcceptButton_OnClick()
	TeamManager:Team_OP_InviteAccept( t_curInvitePlayer["name"] );
	
	TroopInviteFrame:Hide();
end

function TroopInviteFrameOnHide()
	t_curInvitePlayer = { name = "", };
end


function UpdateFollowFramePos()	
	local frame;
	if TeamManager:isInTeamFollow() then
		frame = getglobal( "TeamFollowHintFrame" );
	else
		frame = getglobal( "FollowHintFrame" );		
	end
	--[[
	local nInitFrameWidth	= 129;
	local nInitFrameHeight	= 31;
	local fWidth = nInitFrameWidth	* GetScreenScaleY();
	local fHight = nInitFrameHeight * GetScreenScaleY();
	local fOffset = 10 * ( GetScreenWidth() /DEFAULT_UI_WIDTH );

	-- nWidth最小必须是表情的宽度
	local nWidth = fWidth > 30 and fWidth or 30;
	local nHight = fHight > 20 and fHight or 20;
	x = x - nWidth / 2 - fOffset;
	y = y - nHight / 2;
	frame:SetSize( nWidth,nHight );	
	frame:SetPoint( "topleft", "UIClient", "topleft", x * GetScreenScaleY() - fOffset + 10, y * GetScreenScaleY() - fOffset - 15 );
	--]]
	frame:SetPoint( "center", "UIClient", "center", 0, -90 );
	frame:Show();
end
--[[
local t_FollowHintFrameEvents = { ["GE_MAINPLAYER_SPEED_ZERO"] = {}, };

t_FollowHintFrameEvents["GE_MAINPLAYER_SPEED_ZERO"].func = 
function ()
	if GameFollowManager:isWeiXing() then
		GameFollowManager:cancelWeiXing();
	end
end

function FollowHintFrame_OnLoad()
	for event, _ in pairs( t_FollowHintFrameEvents ) do
		this:RegisterEvent( event );
	end

	t_FollowHintFrameEvents.__index = function ( tab, key )
										tab[key] = { func = function () end };
										return tab[key];
									end
	setmetatable( t_FollowHintFrameEvents, t_FollowHintFrameEvents );

end

function FollowHintFrame_OnEvent()
	t_FollowHintFrameEvents[arg1].func();
end
--]]

function FollowHintFrame_OnUpdate()
	if GameFollowManager:isWeiXing() then
		FollowHintFrame:Show()
		PhotoFrameStopWeiXingButton:Show();
	else
		FollowHintFrame:Hide();
		PhotoFrameStopWeiXingButton:Hide();
	end
end

function TeamFollowHintFrame_OnUpdate()
	if TeamManager:isInTeamFollow() then
		TeamFollowHintFrame:Show();
		FollowHintFrame:Hide();
		PhotoFrameStopWeiXingButton:Hide();
	else
		TeamFollowHintFrame:Hide();
	end
end