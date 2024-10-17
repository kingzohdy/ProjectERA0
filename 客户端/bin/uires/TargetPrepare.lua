function TargetPrepareBar_OnLoad()
	this:RegisterEvent("GE_TARGET_CHANGED")
	this:RegisterEvent("GE_OTHER_PREPARE")
	--TargetPrepareBar:Show()
end

function TargetPrepareBar_OnEvent()
	local self = ActorMgr:getMainPlayer()
	if not self then
		return
	end
	local target = ActorMgr:FindActor(self:GetSelectTarget())
	if target and target:getPrepareSkill() ~= 0 then
		TargetPrepareBar:Show()
	else
		TargetPrepareBar:Hide()
	end
end

function TargetPrepareBar_OnUpdate()
	-- 毫秒级更新
	this:setUpdateTime( 0 );
	local self = ActorMgr:getMainPlayer()
	if not self then
		return
	end
	local target = ActorMgr:FindActor(self:GetSelectTarget())
	if target and target:getHP() > 0 and target:getPrepareSkill() ~= 0 then
		local times	= target:getPrepareTime() / 1000
		local total = target:getTotalPrepareTime() / 1000
		if times > total then
			TargetPrepareBar:Hide()
		else
			local scale = times / total
			TargetPrepareBar_Texture:SetTexUV(81 + (1 - scale) * 202, 620, scale * 202, 32 )
			TargetPrepareBar_Texture:SetSize(scale * 126, 32)
		end
	else
		TargetPrepareBar:Hide()
	end
end