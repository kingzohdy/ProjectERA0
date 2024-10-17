-- 精灵自动施放绑定技 列表
local t_XianBindSkill = { 8002 };

function IsXianBindSkill( nSkillId )
	for i=1, table.getn(t_XianBindSkill) do
		if t_XianBindSkill[i] == nSkillId then
			return true;
		end
	end
	return false;
end