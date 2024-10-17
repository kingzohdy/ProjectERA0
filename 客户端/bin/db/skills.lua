skilldata_table_example=
{
	[101]=
	{
		class_name	="stdskill",
		atk_dist = 100,
		prepare_action = "G_prepare",
		atk_action1 = "G_attack",
		atk_action2 = "",
		hit_action = "hit",
		hit_delaytick = 200,
		hit_delaymode = 0, --0:所有被击对象同时,  1:按距离增加
		hit_delaystep = 100, --按距离增加时, 每1米增加多少tick
		luckhit_action = "luck_hit",
		luckhit_delaytick = 200,
		num_spelleffect = 1, --产生多少个对象
		obj1_createphase = 0, --0:prepare,  1:cast,   2:hit
		obj1_type = 0, --0:固定位置,  1:跟随角色,  2:追踪飞行,  3:自由落体飞行,  4:抛物线飞行
		obj1_res = "effect\\fire.ent",
		obj1_postype = 0,  --0:范围攻击中心,  1:发动角色,  2:被击角色
		obj1_startanchor = 0,
		obj1_startoffsety = 100,
		obj1_startangle = 350,
		obj1_delaytick = 50,
		obj1_intick = 150,
		obj1_outtick = 0,
		obj1_livetick = 1000,
		target_movemode = 0, --0:站立, 1:击退,  2:击飞,  3:旋转
		target_movemaxdist = 0,
		target_movemindist = 0,
		self_movemode = 0, --0:站立, 1:瞬移, 2:冲锋
	},
};

function CreateSkillEffectObject(spell, skilldata, phase)
	for i = 1,skilldata.num_spelleffect do
		prefix = 'obj'..i..'_'
		if skilldata[prefix..'createphase'] == phase then
			effect = spell:createEffectObject(skilldata[prefix..'type'], skilldata[prefix..'res'])
			effect:setStartPos(skilldata[prefix..'postype'], skilldata[prefix..'startanchor'], skilldata[prefix..'startofsety'], skilldata[prefix..'startangle'])
			effect:setLifeTick(skilldata[prefix..'delaytick'], skilldata[prefix..'intick'], skilldata[prefix..'outtick'], skilldata[prefix..'livetick'])
			effect:start()
		end
	end
end

function CallSkillFunc(spell, skillid, funcname)
	skilldata = skilldata_table[skillid]
	skill_funcs = skillfunc_table[skilldata.class_name]
	func = skill_funcs[funcname]

	func(spell, skilldata)
end

function SetSkillDataAttrib(skillid, attribname, attribvalue)
	skilldata = skilldata_table[skillid]
	skilldata[attribname] = attribvalue
end

function GetSkillDataAttrib(skillid, attribname)
	skilldata = skilldata_table[skillid]
	return skilldata[attribname]
end

function serialize(o, indent)
	if indent == nil then indent='' end

	if type(o) == "number" then
		io.write(o)
	elseif type(o) == "string" then
		io.write(string.format("%q", o))
	elseif type(o) == "table" then
		io.write("{\n")
		for k,v in pairs(o) do
			if type(k) == "string" then
				io.write(indent.."	", k, " = ")
			else
				io.write(indent.."	[")
				serialize(k, indent.."	")
				io.write("] = ")
			end
			serialize(v, indent.."	")
			io.write(",\n")
		end
		io.write(indent, "}")
	else
		error("cannot serialize a " .. type(o))
	end
end

function InitSkillData()
	for id,skill in pairs(skilldata_table) do
		SkillMgr:addSkill(id)
	end
end

function SaveSkillData(respath)
	f = io.open(respath, 'w')
	io.output(f)
	io.write("skilldata_table = ")
	serialize(skilldata_table)
	f:close()
end

function NewSkillData(skillid)
	skilldata_table[skillid] = {}
	SkillMgr:addSkill(skillid)
end

skillfunc_table = 
{
stdskill = 
{
	prepare = function(spell, skillres)
		if spell:getAttackDist() > skillres.atk_dist then
			ErrorMessage("超出攻击距离, 不能使用技能")
			return
		end
		spell:playAction(0, skillres.prepare_action)
		CreateSkillEffectObject(spell, skillres, 0)
	end,

	attack = function(spell, skillres)
		spell:playAction(0, skillres.atk_action1)
		CreateSkillEffectObject(spell, skillres, 1)
	end,

	hit = function(spell, skillres)
	end,

	update = function(spell, skillres)
	end,

	onPrepare = function(spell, skillres)
	end,
}
}



