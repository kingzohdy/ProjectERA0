SkillScript = {}

function SkillScript.Create(first, ...)
	if first then
		return coroutine.create(first), SkillScript.Create(...)
	end
end

function SkillScript.HideCaster(delay)
	return function(spell)
		local caster
		local id = spell:getOwner()
		caster = ActorMgr:FindActor(id)
		if not caster then
			return
		end
		caster:hide(true)
		sleep(delay)
		caster = ActorMgr:FindActor(id)
		if not caster then
			return
		end
		caster:hide(false)
	end
end

function SkillScript.MoveCasterToTargetPos(delay)
	return function(spell)
		local caster
		local id = spell:getOwner()
		caster = ActorMgr:FindActor(id)
		if not caster then
			return
		end
		local old = caster:GetPosition()
		local target = spell:getTargetPoint()
		caster:SetPosition(target.x / 10, target.z / 10)
		sleep(delay)
		caster = ActorMgr:FindActor(id)
		if not caster then
			return
		end
		caster:SetPosition(old.x / 10, old.z / 10)
	end
end