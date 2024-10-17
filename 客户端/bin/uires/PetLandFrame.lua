
local szInviteDesc = [[��ң�#G%s#n ϣ������Э������ɻ�������ϵ�����񡣽�����Թ�ϵ��������ºô���

  1��ȫ��#cff9600����Ҫ�κβ���#n��ֻ��Ҫ#cff9600��Ӹ���#n���ɡ�
  2������ң�#G%s#n ��������ʱ#cffffbe�����ý���#n��

������Թ�ϵ��Ҫ��Ʒ��
#cff9600ʱ��֮Կ#n 1������ң�#G%s#n �ṩ��
#cff9600����֮��#n 1�������ṩ��
]]

function OnReciveGayInvite( roleName )
	FindGayMessageBoxFrame:SetClientString( roleName )
	szDesc = string.format( szInviteDesc, roleName,roleName,roleName )
	FindGayMessageBoxFrameRich:SetText( szDesc, 255,255,190 )
	FindGayMessageBoxFrame:Show()
end

function FindGayMessageBoxFrameOkBtn_OnClick()
	local roleName = FindGayMessageBoxFrame:GetClientString();
	GamePetLand:replyGayInvite( roleName,1 )
	FindGayMessageBoxFrame:Hide()
end

function FindGayMessageBoxFrameCancelBtn_OnClick()
	local roleName = FindGayMessageBoxFrame:GetClientString();
	GamePetLand:replyGayInvite( roleName,0 )
	FindGayMessageBoxFrame:Hide()
end

function FindGayMessageBoxFrame_OnLoad()
	this:setUpdateTime( 3 )
end

function FindGayMessageBoxFrame_OnShow()
	this:SetClientUserData( 0, GameMgr:getTickTime() )
end
function FindGayMessageBoxFrame_OnUpdate()
	local tick = GameMgr:getTickTime() - this:GetClientUserData( 0 )
	if tick > 2 * 60 * 1000 then
		local roleName = FindGayMessageBoxFrame:GetClientString();
		GamePetLand:replyGayInviteTimeOut( roleName )
		FindGayMessageBoxFrame:Hide()
	end
end