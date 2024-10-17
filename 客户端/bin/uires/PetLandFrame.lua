
local szInviteDesc = [[玩家：#G%s#n 希望你能协助他完成基友速配系列任务。建立配对关系您获得以下好处：

  1、全程#cff9600不需要任何操作#n，只需要#cff9600组队跟随#n即可。
  2、在玩家：#G%s#n 交付任务时#cffffbe额外获得奖励#n。

建立配对关系需要物品：
#cff9600时间之钥#n 1个（玩家：#G%s#n 提供）
#cff9600岁月之轮#n 1个（您提供）
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