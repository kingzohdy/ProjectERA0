-- սʿ�ļ��ܱ�
local Skill_Race_WARRIOR	= {
	7000,1010,1034,1032,1012,1014,1016,1018,1022,1024,1026,1028;1120,1116,1210,1212,1035,1219,1222
};

-- ��ʦ�ļ��ܱ�
local Skill_Race_MAGICOR	= {
	7001,2010,2012,2014,2016,2018,2020,2026,2028,2030,2034;2118,2113,2210,2208,2035,2218,2121,2114,2037,2022,2019,2150
};

-- ǹ�ֵļ��ܱ�
local Skill_Race_GUNER		= {
	7002,3010,3014,3016,3020,3022,3026,3032,3034,3040;3112,3117,3211,3212,3037,3104,3213,3217,3011,3218,3106
};

-- ��˾�ļ��ܱ�
local Skill_Race_CHURCH		= {
	7003,4000,4002,4034,4036,4038,4040,4042;4116,4210,4222,4123,4043,4112,4044,4027,4211,4120,4006
};

-- ���ѧ�����ʾ�ļ��ܱ�
local Skill_Show_Learn		= {
	4006,7005,9999,9996,
	10278,10279,10280,10281,10282,10283,10284,10285,10286,10287,10288,10289,10290,10291,10294,10297,
	10390,10352,10395,10396,
	10425,
	10811,10812,10813,10814,10815,10816,10817,10818
};

SKILL_COMMON = {}
for _, i in ipairs(Skill_Show_Learn) do
	SKILL_COMMON[i] = true
end

-- ��ȡ��ǰְҵ�ļ��ܱ�
function GetSkillList( nRace )
	nRace = nRace % 0x10
	if nRace == g_nCAREER_WARRIOR then
		return Skill_Race_WARRIOR;
	elseif nRace == g_nCAREER_MAGICOR then
		return Skill_Race_MAGICOR;
	elseif nRace == g_nCAREER_GUNER then
		return Skill_Race_GUNER;
	elseif nRace == g_nCAREER_CHURCH then
		return Skill_Race_CHURCH;
	end
	return nil;
end