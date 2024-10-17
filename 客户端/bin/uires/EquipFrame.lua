--@Desc:����ע����������������Ϣ
--@return: Nothing
local EquipRealX		= 0;
local EquipRealY		= 0;  
local MAX_FASHION_BAG_NUM = 3;
LEVEUP_LIMIT_LV	= 30;
MAX_ATTR_FONT = 25;	
MAX_SUIT_NUM = 6;	 
MAX_ATTR_NUM = 20;
MAX_EQUIP_ATTR_BTN = 17;
SUIT_ATTR	= 56;
PARENT_NODE	= 0;
CHILD_NODE	= 1;
local NONE_ATTR = -1;
bCharactorFrameOpenFlag = false;
local ModelAngle = 0;
MAX_NORMAL_ATTACK_ATTR_NUM	= 8;
MAX_YUAN_SU_ATTACK_ATTR_NUM	= 5;

MAX_NORMAL_DEFEND_ATTR_NUM	= 6;
MAX_YUAN_SU_DEFEND_ATTR_NUM	= 5;

MAX_FU_ZHU_ATTR_NUM	= 4;

MAX_BASE_ATTR_NUM	= 7;

MAX_SUIT_ATTR_NUM	= 16;
MAX_BOTTOM_LEAVE_SPACE = 5;

local t_mainAttrConfig = 
{
-- /* �ػ��� */	����
[CLIENT_CAREER_WARRIOR_SINGLEHAND]	= { ["btn"] = "RoleAttrBtn_BaseAttrBtn3", attrID = ATTR_ID_TILI, ["careerDesc"] = "�ػ���", },

-- /* ������ */	����
[CLIENT_CAREER_WARRIOR_DOUBLEHAND]	= { ["btn"] = "RoleAttrBtn_BaseAttrBtn1", attrID = ATTR_ID_LILIANG, ["careerDesc"] = "������", },

-- /* ������ */	��֪
[CLIENT_CAREER_MAGICOR_SINGLEHAND]	= { ["btn"] = "RoleAttrBtn_BaseAttrBtn6", attrID = ATTR_ID_MINJIE, ["careerDesc"] = "������", },

-- /* ����ʦ */	����
[CLIENT_CAREER_MAGICOR_DOUBLEHAND]	= { ["btn"] = "RoleAttrBtn_BaseAttrBtn2", attrID = ATTR_ID_ZHILI, ["careerDesc"] = "����ʦ", },

-- /* ��ɱ�� */	����
[CLIENT_CAREER_GUNER_SINGLEHAND]	= { ["btn"] = "RoleAttrBtn_BaseAttrBtn1", attrID = ATTR_ID_LILIANG, ["careerDesc"] = "��ɱ��", },

-- /* ������ */	����
[CLIENT_CAREER_GUNER_DOUBLEHAND]	= { ["btn"] = "RoleAttrBtn_BaseAttrBtn5", attrID = ATTR_ID_FANGYU, ["careerDesc"] = "������", },

-- /* ʥ���� */	����
[CLIENT_CAREER_CHURCH_SINGLEHAND]	= { ["btn"] = "RoleAttrBtn_BaseAttrBtn4", attrID = ATTR_ID_JINGSHEN, ["careerDesc"] = "ʥ����", },

-- /* Ԧ��ʦ */	����
[CLIENT_CAREER_CHURCH_DOUBLEHAND]	= { ["btn"] = "RoleAttrBtn_BaseAttrBtn2", attrID = ATTR_ID_ZHILI, ["careerDesc"] = "Ԧ��ʦ", },
};

local bEquipFrameShowFashion = false;

function Equip_Test_OnShow()
	--ShowEquipXinShouBtn( { btn = "EquipFrameLvXinShouBtn", text = "������������Ч��" } );
	local nNum = 16;
	RoleAttrBtnSuitBackGroundButton:SetHeight( nNum * 16 );
	if nNum * 16 > 32 then
		local size = 456 - 32 - MAX_BOTTOM_LEAVE_SPACE +  nNum * 16;
		RoleAttrBtn:SetHeight( size );
	end
end

function GetRoleAttr( nAttrID, actor )
	if nAttrID < 0 then
		return "";
	end
	
	--local MainPlayer = ActorMgr:getMainPlayer();
	--if MainPlayer == nil then return end

	local nValue = actor:getShortAttriValule( nAttrID );
	if nAttrID == ATTR_ID_DAMAGE then
		nValue = math.ceil(nValue/100);
	elseif nAttrID == ATTR_ID_MSPD then
		nValue = math.ceil(nValue/400);
	end
	return nValue;
end

function GetRoleBaseAttr( nAttrID, actor )
	if nAttrID < 0 then
		return "";
	end

	--local MainPlayer = ActorMgr:getMainPlayer();
	local nValue = actor:getBaseAttriValue( nAttrID );
	
	if nAttrID == ATTR_ID_MSPD then
		nValue = math.ceil(nValue/400);
	end
	return nValue;
end

function GetMulAndAddRoleAttr( data, actor )
	local nAddAttrID = data["add"];
	local nMulAttrID = data["mul"];
	--local MainPlayer = ActorMgr:getMainPlayer();

	local nAddValue = actor:getShortAttriValule( nAddAttrID );
	local nMulValue = actor:getShortAttriValule( nMulAttrID );
	return nAddValue + nMulValue * nAddValue;
end

function GetRoleMulAndAddBaseAttr( data, actor )
	local nAddAttrID = data["add"];
	local nMulAttrID = data["mul"];
	--local MainPlayer = ActorMgr:getMainPlayer();

	local nAddValue = actor:getBaseAttriValue( nAddAttrID );
	local nMulValue = actor:getBaseAttriValue( nMulAttrID );
	return nAddValue + nMulValue * nAddValue;
end

function GetMulAndAddRoleAttrDesc( data, actor )
	local nAddAttrID = data["add"];
	local nMulAttrID = data["mul"];
	
	local nAddValue = actor:getShortAttriValule( nAddAttrID );
	local nMulValue = actor:getShortAttriValule( nMulAttrID );
	if nMulValue == 0 then
		return nAddValue;
	else
		return nAddValue.." + "..nMulValue.."%" ;
	end
end

function GetRoleBaoJiDamageAttrDesc( nAttrID, actor )
	local nValue = actor:getShortAttriValule( nAddAttrID );
	return nValue.."%";
end

function GetRoleHPandMPAttrDesc( nAttrID, actor )
	if nAttrID == ATTR_ID_MPMAX then
		local nValue	= actor:getShortAttriValule( ATTR_ID_MP );
		local nMaxValue	= actor:getShortAttriValule( ATTR_ID_MPMAX );
		return nValue.."/"..nMaxValue;
	elseif nAttrID == ATTR_ID_HPMAX then
		local nValue	= actor:getShortAttriValule( ATTR_ID_HP );
		local nMaxValue	= actor:getShortAttriValule( ATTR_ID_HPMAX );
		return nValue.."/"..nMaxValue;
	end
end

function GetRoleMovementAttrDesc( nAttrID, actor )
	if nAttrID < 0 then
		return "";
	end

	local nValue = GetRoleCurMovementAttr( nAttrID, actor )
	nValue = math.ceil( nValue * 100 / CLIENT_BASE_MSPD);
	return nValue.."%";
end

function GetRoleBaseMovementAttr( nAttrID, actor )
	return CLIENT_BASE_MSPD;
end

function GetRoleCurMovementAttr( nAttrID, actor )
	return actor:getSpeed();
end

function GetRoleAttrPercentDesc( nAttrID, actor )
	if nAttrID < 0 then
		return "";
	end

	local nValue = actor:getIntAttriValue( nAttrID );
	return nValue.."%";
end

function GetRoleAttrPercent( nAttrID, actor )
	if nAttrID < 0 then
		return 0;
	end
	return actor:getIntAttriValue( nAttrID );
end

--[[
1�������� =  Э�鷢�͵ı�����+ ��������/��������+�����ֿ���/3��
LevelUP�� �����ֿ� MonDefHeavyHit
--]]

function GetRoleAttrCurBaoJiLv( nAttrID, actor )
	if nAttrID < 0 then
		return 0;
	end
	local nBaoJiLvValue		= actor:getShortAttriValule( nAttrID );
	local BaoJiLiValue		= actor:getShortAttriValule( ADD_ATTR_RESULT_BASE_ATTR_HEAVYHIT );
	local levelUpDef		= getLevelUP( actor:getLv() );
	if levelUpDef == nil then
		return nBaoJiLvValue;
	end
	local baoJiDiKangValue	= levelUpDef.MonDefHeavyHit;
	local fResult			= nBaoJiLvValue + ( BaoJiLiValue / ( BaoJiLiValue + baoJiDiKangValue ) / 2  );
	
	return math.ceil( fResult );
end

function GetRoleAttrBaoJiLvPercentDesc( nAttrID, actor )
	if nAttrID < 0 then
		return "";
	end

	local nBaoJiLvValue		= actor:getIntAttriValue( nAttrID );
	local BaoJiLiValue		= actor:getIntAttriValue( ATTR_ID_BASE_ATTR_HEAVY );
	local levelUpDef		= getLevelUP( actor:getLv() );
	if levelUpDef == nil then
		--print( "GetRoleAttrBaoJiLvPercentDesc--levelUpDef == nil" );
		return nBaoJiLvValue.."%";
	end
	local baoJiDiKangValue	= levelUpDef.MonDefHeavyHit;
	local fResult			= nBaoJiLvValue + ( BaoJiLiValue / ( BaoJiLiValue + baoJiDiKangValue ) / 2  ) * 100;
	fResult					= math.min( fResult, 100 );
	local interger, fraction = math.modf( fResult );
	--[[
	print( "ATTR_ID_HIT_RATE--Э�鷢�ͱ����ʻ���ֵ = "..actor:getBaseAttriValue( nAttrID )..", Э�鷢�ͱ����ʵ�ǰֵ = "..
			nBaoJiLvValue..",�����ֿ� ="..baoJiDiKangValue..", ������ = "..BaoJiLiValue.."�� result = "..fResult );
	--]]
	return ( interger.."."..math.floor( fraction * 100 ) ).."%";
end

function GetRoleAttrBaoJiLevelTipsDesc( nAttrID, actor )
	return "���ӹ��������ļ���\n��ͬ�ȼ����﹥���������ʣ�"..GetRoleAttrBaoJiLvPercentDesc( ATTR_ID_HEAVY, actor );
end

function GetRoleAttrShanBiLevelTipsDesc( nAttrID, actor )
	local levelUpDef	= getLevelUP( actor:getLv() );
	local szShanBiLv	= "0";
	if levelUpDef ~= nil then
		--������=0.4-��������*0.3/���ܵȼ�
		local fResult = 0.4 - levelUpDef.MonLevelHit * 0.3 / actor:getShortAttriValule( nAttrID );
		local dodgeVal = actor:getShortAttriValule(ATTR_ID_DODGE);
		--print( "fResult = "..fResult );
		fResult		= fResult * 100 + dodgeVal;
		fResult		= math.min( fResult, 100 );
		fResult		= math.max( fResult, 0 );
		szShanBiLv	= string.format( "%.2f", fResult ).."%"
	end

	return "���ӱ�����ʱ���ܵļ���\n��ͬ�����﹥�����ܼ���Ϊ��"..szShanBiLv .. "\n���ܵȼ�Խ�ߣ��ܵ�����ʱѡ����С�����ļ�������";
end

function GetRoleAttrFanShangLevelTipsDesc( nAttrID, actor )
	local nFanShangLevel	= actor:getShortAttriValule( nAttrID );
	local nLevel			= actor:getLv();

	--���˱��� = ���˵ȼ�/(���˵ȼ�*2+����ȼ�^2)
	local fResult = nFanShangLevel / ( nFanShangLevel * 2 + nLevel ^ 2 );
	fResult		= fResult * 100;
	fResult		= math.min( fResult, 100 );
	fResult		= math.max( fResult, 0 );
	return "�������˺�������������˺���Դ\n���˱���Ϊ��"..string.format( "%.2f", fResult ).."%";
end

function GetRoleAttrDiKangLevelTipsDesc( nAttrID, actor )
	-- �ֿ�����=�ֿ��ȼ�/(�ֿ��ȼ�+����ȼ�^2/5)

	local nDiKangLevel	= actor:getShortAttriValule( nAttrID );
	local nLevel		= actor:getLv();
	local fResult		= nDiKangLevel / ( nDiKangLevel + nLevel ^ 2 / 5 );
	fResult		= fResult * 100;
	fResult		= math.min( fResult, 100 );
	fResult		= math.max( fResult, 0 );

	return "�����ܵ�������ҹ���ʱ�����˺�\n�ֿ�����Ϊ��"..string.format( "%.2f", fResult ).."%";
end

function GetRoleAttrPoShangLevelTipsDesc( nAttrID, actor )
	-- �Ʒ�����=�Ʒ��ȼ�/(�Ʒ��ȼ�*3+����ȼ�^2/5)

	local nPoFangLevel	= actor:getShortAttriValule( nAttrID );
	local nLevel		= actor:getLv();
	local fResult		= nPoFangLevel / ( nPoFangLevel + nLevel ^ 2 / 5 );
	fResult		= fResult * 100;
	fResult		= math.min( fResult, 100 );
	fResult		= math.max( fResult, 0 );
	return "���ӹ����������ʱ��ɵ��˺�\n�����ֿ��ȼ�Ϊ0����������˺�����Ϊ��"..string.format( "%.2f", fResult ).."%";
end

function GetRoleAttrHealHpPercentDesc( nAttrID, actor )
	local fResult	= actor:getShortAttriValule( nAttrID );
	--print( "GetRoleAttrHealHpPercentDesc--nAttrID = "..nAttrID.."��fResult =  "..fResult );
	fResult			= fResult / 100;
	fResult			= math.min( fResult, 100 );
	fResult			= math.max( fResult, 0 );
	return string.format( "%.2f", fResult ).."%";
end

--[[
2�������� =  Э�鷢�͵������� + ��������/ �ο���������
LevelUP�� �ο������� UIBaseFlee

--]]

function GetRoleAttrCurMingZhongLv( nAttrID, actor )
	if nAttrID < 0 then
		return 0;
	end

	local nMingZhongLvValue		= actor:getShortAttriValule( nAttrID );
	local MingZhongLiValue		= actor:getShortAttriValule( ATTR_ID_HIT );
	local levelUpDef			= getLevelUP( actor:getLv() );
	if levelUpDef == nil then
		return nMingZhongLvValue;
	end
	local MingZhongBaseValue	= levelUpDef.UIBaseFlee;
	local fResult				= nMingZhongLvValue + ( MingZhongLiValue / MingZhongBaseValue );
	
	return math.ceil( fResult );
end

function GetRoleAttrMingZhongLvPercentDesc( nAttrID, actor )
	if nAttrID < 0 then
		return "";
	end

	local nMingZhongLvValue		= actor:getIntAttriValue( nAttrID );
	local fResult				= nMingZhongLvValue / 100;
	fResult						= math.min( fResult, 100 );
	if fResult == 100 then
		return "100.0%"
	else
		return string.format( "%.2f", fResult ).."%"
	end
end

function GetRoleAttrMingZhongLevelTipsDesc( nAttrID, actor )
	return "���ӹ������еļ���\n��ͬ�ȼ����﹥�����м��ʣ�"..GetRoleAttrMingZhongLvPercentDesc( ATTR_ID_HIT_RATE, actor ) .."\n���еȼ�Խ�ߣ�ѡ����󹥻��ļ���Խ��";
end

function GetRoleAttrBaoJiShangHaiPercentDesc( nAttrID, actor )
	if nAttrID < 0 then
		return "";
	end
	local ADJUST_VALUE = 150;
	local fResult	= actor:getShortAttriValule( nAttrID );
	fResult			= fResult / 100;
	if fResult > 0 then 
		return ( fResult + ADJUST_VALUE ).."%";
	else 
		return ADJUST_VALUE.."%"
	end
end

function GetRoleAttrLiLiangTipsDesc( nAttrID, actor )
	local t_changeCareerControl = GetChangeCareerControl();
	local nRace					= actor:getRace();
	local szExtraDesc			= "";

	if t_changeCareerControl:isChangeCareer( { ["race"] = nRace } ) then
		local nMainAttrID = t_mainAttrConfig[nRace]["attrID"];
		if nMainAttrID == nAttrID then
			szExtraDesc = "\n#G"..t_mainAttrConfig[nRace]["careerDesc"].."��������#n\n1��������������2��������";
		end
	end

	return "1����������5��������"..szExtraDesc;
end

function GetRoleAttrZhiLiTipsDesc( nAttrID, actor )
	local t_changeCareerControl = GetChangeCareerControl();
	local nRace					= actor:getRace();
	local szExtraDesc			= "";

	if t_changeCareerControl:isChangeCareer( { ["race"] = nRace } ) then
		local nMainAttrID = t_mainAttrConfig[nRace]["attrID"];
		if nMainAttrID == nAttrID then
			szExtraDesc = "\n#G"..t_mainAttrConfig[nRace]["careerDesc"].."��������#n\n1��������������2��ħ������";
		end
	end

	return "1����������5��ħ������"..szExtraDesc;
end

function GetRoleAttrTiZhiTipsDesc( nAttrID, actor )
	local t_changeCareerControl = GetChangeCareerControl();
	local nRace					= actor:getRace();
	local szExtraDesc			= "";

	if t_changeCareerControl:isChangeCareer( { ["race"] = nRace } ) then
		local nMainAttrID = t_mainAttrConfig[nRace]["attrID"];
		if nMainAttrID == nAttrID then
			szExtraDesc = "\n#G"..t_mainAttrConfig[nRace]["careerDesc"].."��������#n\n1�����ʶ�������2������ֵ��2���������";
		end
	end

	return "1����������25������ֵ\n������ս���У�����ֵÿ3��ظ�Ч��"..szExtraDesc;
end

function GetRoleAttrJingShenTipsDesc( nAttrID, actor )
	local t_changeCareerControl = GetChangeCareerControl();
	local nRace					= actor:getRace();
	local szExtraDesc			= "";

	if t_changeCareerControl:isChangeCareer( { ["race"] = nRace } ) then
		local nMainAttrID = t_mainAttrConfig[nRace]["attrID"];
		if nMainAttrID == nAttrID then
			szExtraDesc = "\n#G"..t_mainAttrConfig[nRace]["careerDesc"].."��������#n\n1�㾫���������2��ħ������";
			local nVal = GetRoleAttr( nAttrID,actor )
			local lv = actor:getLv()
			szExtraDesc = szExtraDesc .. "\n" .. nVal .. "�㾫���������Ƽӳ�" .. math.floor(nVal/(nVal*2+lv*lv/20)*100) .. "%"
		end
	end

	return "1�㾫������25��ħ��ֵ\n������ս���У�ħ��ֵÿ3��ظ�Ч��"..szExtraDesc;
end

function GetRoleAttrMinJieTipsDesc( nAttrID, actor )
	local t_changeCareerControl = GetChangeCareerControl();
	local nRace					= actor:getRace();
	local szExtraDesc			= "";

	if t_changeCareerControl:isChangeCareer( { ["race"] = nRace } ) then
		local nMainAttrID = t_mainAttrConfig[nRace]["attrID"];
		if nMainAttrID == nAttrID then
			szExtraDesc = "\n#G"..t_mainAttrConfig[nRace]["careerDesc"].."��������#n\n1�����ݶ�������2��������";
		end
	end

	return "1����������5���������"..szExtraDesc;
end

function GetRoleAttrGanZhiTipsDesc( nAttrID, actor )
	local t_changeCareerControl = GetChangeCareerControl();
	local nRace					= actor:getRace();
	local szExtraDesc			= "";

	if t_changeCareerControl:isChangeCareer( { ["race"] = nRace } ) then
		local nMainAttrID = t_mainAttrConfig[nRace]["attrID"];
		if nMainAttrID == nAttrID then
			szExtraDesc = "\n#G"..t_mainAttrConfig[nRace]["careerDesc"].."��������#n\n1���֪��������2��ħ������";
		end
	end

	return "1���֪����5��ħ������"..szExtraDesc;
end

t_RoleAttrInfo =
{
-- getBaseAttrFunc = , getCurAttrFunc = , getAttrDescFunc = 
["��������"] = 
{
	[CLIENT_CAREER_WARRIOR] = 
	{
		[1] = { name = "����\b",		attrID = ATTR_ID_LILIANG,	desc = "1����������5��������", 
				getAttrTipsDescFunc = GetRoleAttrLiLiangTipsDesc },
		[2] = { name = "����\b",		attrID = ATTR_ID_ZHILI,		desc = "1����������5��ħ������", 
				getAttrTipsDescFunc = GetRoleAttrZhiLiTipsDesc },
		[3] = { name = "����\b",		attrID = ATTR_ID_TILI,		desc = "1����������25������ֵ\n������ս���У�����ֵÿ3��ظ�Ч��", 
				getAttrTipsDescFunc = GetRoleAttrTiZhiTipsDesc },
		[4] = { name = "����\b",		attrID = ATTR_ID_JINGSHEN,	desc = "1�㾫������25��ħ��ֵ\n������ս���У�ħ��ֵÿ3��ظ�Ч��", 
				getAttrTipsDescFunc = GetRoleAttrJingShenTipsDesc },
		[5] = { name = "����\b",		attrID = ATTR_ID_FANGYU,	desc = "1����������5���������", 
				getAttrTipsDescFunc = GetRoleAttrMinJieTipsDesc },
		[6] = { name = "��֪\b",		attrID = ATTR_ID_MINJIE,	desc = "1���֪����5��ħ������", 
				getAttrTipsDescFunc = GetRoleAttrGanZhiTipsDesc },
		[7] = { name = "�ƶ��ٶ�\b",	attrID = ATTR_ID_MSPD,		desc = "��ɫ��ǰ���ƶ��ٶ�", getAttrDescFunc = GetRoleMovementAttrDesc,
				getBaseAttrFunc = GetRoleBaseMovementAttr,			getCurAttrFunc = GetRoleCurMovementAttr },

		maxElemNum = 7,
	},
},

["��������"] = 
{
	[CLIENT_CAREER_WARRIOR] = 
	{	
		[1] = { name = "������\b",	attrID = ATTR_ID_PHSIC_ATK,			maxAttrID = ATTR_ID_PHSIC_MAX_ATK,	desc = "������������ɵ��˺�", getCurAttrFunc = GetRoleAttrPercent },
		[2] = { name = "�����ȼ�\b",	attrID = ATTR_ID_BASE_ATTR_HEAVY,	desc = "���ӹ��������ļ���\n��ͬ�ȼ�������ɱ����ļ��ʣ�",
				getAttrTipsDescFunc = GetRoleAttrBaoJiLevelTipsDesc },

		[3] = { name = "�����˺�\b",	attrID = ATTR_ID_HEAVY_VAL,			desc = "���������ͨ�˺��İٷֱ�", 
				getAttrDescFunc = GetRoleAttrBaoJiShangHaiPercentDesc },
		--[[
		[4] = { name = "������\b",		attrID = ATTR_ID_HEAVY,				desc = "��ͬ�ȼ�������ɱ����ļ���", 
				-- getCurAttrFunc = GetRoleAttrCurBaoJiLv
				getAttrDescFunc = GetRoleAttrBaoJiLvPercentDesc,			 
				},
		--]]

		[4] = { name = "���еȼ�\b",	attrID = ATTR_ID_HIT,				desc = "���ӹ������еļ���\n��ͬ�ȼ����﹥�����еļ��ʣ�", 
				getAttrTipsDescFunc = GetRoleAttrMingZhongLevelTipsDesc },
		--[[
		[6] = { name = "������\b",		attrID = ATTR_ID_HIT_RATE,			desc = "��ͬ�ȼ����﹥�����еļ���", 
				-- getCurAttrFunc = GetRoleAttrCurMingZhongLv  
				getAttrDescFunc = GetRoleAttrMingZhongLvPercentDesc,		
				},
		--]]

		[5] = { name = "�����˺��ӳ�\b",attrID = ATTR_ID_PHSIC_HURT,		desc = "��������˺��������Ӱٷֱ�", getAttrDescFunc = GetRoleAttrPercentDesc, getCurAttrFunc = GetRoleAttrPercent },
		[6] = { name = "ħ���˺��ӳ�\b",attrID = ATTR_ID_ICE_HURT,			desc = "���ħ���˺��������Ӱٷֱ�", getAttrDescFunc = GetRoleAttrPercentDesc, getCurAttrFunc = GetRoleAttrPercent },
		maxElemNum = 6,
	},

	[CLIENT_CAREER_MAGICOR] = 
	{
		[1] = { name = "ħ������\b",	attrID = ATTR_ID_ICE_ATK,			maxAttrID = ATTR_ID_MAGIC_MAX_ATK,	desc = "����ħ��������ɵ��˺�", getCurAttrFunc = GetRoleAttrPercent },
		[2] = { name = "�����ȼ�\b",	attrID = ATTR_ID_BASE_ATTR_HEAVY,	desc = "���ӹ��������ļ���\n��ͬ�ȼ�������ɱ����ļ��ʣ�",
				getAttrTipsDescFunc = GetRoleAttrBaoJiLevelTipsDesc },

		[3] = { name = "�����˺�\b",	attrID = ATTR_ID_HEAVY_VAL,			desc = "���ӱ�����ɵ��˺�", 
				getAttrDescFunc = GetRoleAttrBaoJiShangHaiPercentDesc },
		
		--[[
		[4] = { name = "������\b",		attrID = ATTR_ID_HEAVY,				desc = "��ͬ�ȼ�������ɱ����ļ���", 
				-- getCurAttrFunc = GetRoleAttrCurBaoJiLv  
				getAttrDescFunc = GetRoleAttrBaoJiLvPercentDesc,			
				},
		--]]

		[4] = { name = "���еȼ�\b",	attrID = ATTR_ID_HIT,				desc = "���ӹ������еļ���\n��ͬ�ȼ����﹥�����еļ��ʣ�", 
				getAttrTipsDescFunc = GetRoleAttrMingZhongLevelTipsDesc },

		--[[
		[6] = { name = "������\b",		attrID = ATTR_ID_HIT_RATE,			desc = "��ͬ�ȼ����﹥�����еļ���", 
				-- getCurAttrFunc = GetRoleAttrCurMingZhongLv
				getAttrDescFunc = GetRoleAttrMingZhongLvPercentDesc,		  },
		
		--]]

		[5] = { name = "�����˺��ӳ�\b",attrID = ATTR_ID_PHSIC_HURT,		desc = "��������˺��������Ӱٷֱ�",getAttrDescFunc = GetRoleAttrPercentDesc, getCurAttrFunc = GetRoleAttrPercent },
		[6] = { name = "ħ���˺��ӳ�\b",attrID = ATTR_ID_ICE_HURT,			desc = "���ħ���˺��������Ӱٷֱ�", getAttrDescFunc = GetRoleAttrPercentDesc, getCurAttrFunc = GetRoleAttrPercent },
		maxElemNum = 6,
	},
},

["��������"] = 
{
	[CLIENT_CAREER_WARRIOR] = 
	{
		[1] = { name = "�������\b",	attrID = ATTR_ID_PHSIC_DEF,			desc = "�������Խ�ߣ������ܵ��������˺�Խ��", getCurAttrFunc = GetRoleAttrPercent },
		[2] = { name = "ħ������\b",	attrID = ATTR_ID_ICE_DEF,			desc = "ħ������Խ�ߣ������ܵ���ħ���˺�Խ��", getCurAttrFunc = GetRoleAttrPercent },
		[3] = { name = "���ܵȼ�\b",	attrID = ATTR_ID_FLEE,				desc = "����������", 
				getAttrTipsDescFunc = GetRoleAttrShanBiLevelTipsDesc
				},

		[4] = { name = "�����ֿ�\b",	attrID = ATTR_ID_BASE_ATTR_DEFHEAVY,desc = "�����ܵ������ļ���", },
		[5] = { name = "�����˺�����\b",attrID = ATTR_ID_DEF_PHSIC_HURT,	desc = "ֱ�Ӽ����ܵ��������˺�", },
		[6] = { name = "ħ���˺�����\b",attrID = ATTR_ID_DEF_ICE_HURT,		desc = "ֱ�Ӽ����ܵ���ħ���˺�", },

		maxElemNum = 6,
	},
},

["Ԫ�ع�����"] = 
{
	[CLIENT_CAREER_WARRIOR] = 
	{
		[1] = { name = "��Ԫ���˺�\b",	attrID = ATTR_ID_FIRE_ATK,			desc = "����ʱ���ӷ�Ԫ���˺�", },
		[2] = { name = "��Ԫ���˺�\b",	attrID = ATTR_ID_WATER_ATK,			desc = "����ʱ������Ԫ���˺�", },
		[3] = { name = "��Ԫ���˺�\b",	attrID = ATTR_ID_THUNDER_ATK,		desc = "����ʱ���ӵ�Ԫ���˺�", },
		[4] = { name = "��Ԫ���˺�\b",	attrID = ATTR_ID_LIGHT_ATK,			desc = "����ʱ���ӹ�Ԫ���˺�", },
		[5] = { name = "��Ԫ���˺�\b",	attrID = ATTR_ID_NIGHT_ATK,			desc = "����ʱ���Ӱ�Ԫ���˺�", },
		maxElemNum = 5,
	},
},

["Ԫ�ط�����"] = 
{
	[CLIENT_CAREER_WARRIOR] = 
	{
		[1] = { name = "��Ԫ�ؿ���\b",	attrID = ATTR_ID_FIRE_DEF,			desc = "�����ܵ��ķ�Ԫ���˺�", },
		[2] = { name = "��Ԫ�ؿ���\b",	attrID = ATTR_ID_WATER_DEF,			desc = "�����ܵ�����Ԫ���˺�", },
		[3] = { name = "��Ԫ�ؿ���\b",	attrID = ATTR_ID_THUNDER_DEF,		desc = "�����ܵ��ĵ�Ԫ���˺�", },
		[4] = { name = "��Ԫ�ؿ���\b",	attrID = ATTR_ID_LIGHT_DEF,			desc = "�����ܵ��Ĺ�Ԫ���˺�", },
		[5] = { name = "��Ԫ�ؿ���\b",	attrID = ATTR_ID_NIGHT_DEF,			desc = "�����ܵ��İ�Ԫ���˺�", },
		maxElemNum = 5,
	},
},

["��������"] = 
{
	[CLIENT_CAREER_WARRIOR] = 
	{
		[1] = { name = "���˵ȼ�\b",	attrID = ATTR_ID_REFLECTION_HP,			desc = "�������˺�������������˺���Դ", 
				getAttrTipsDescFunc = GetRoleAttrFanShangLevelTipsDesc },

		[2] = { name = "�ֿ��ȼ�\b",	attrID = ATTR_ID_DEF_HP,			desc = "�����ܵ�������ҹ���ʱ�����˺�", 
				getAttrTipsDescFunc = GetRoleAttrDiKangLevelTipsDesc },

		[3] = { name = "�Ʒ��ȼ�\b",	attrID = ATTR_ID_IGNORE_DEF,		desc = "����ʱ����Ŀ��һ�����ʷ���ֵ",
				getAttrTipsDescFunc = GetRoleAttrPoShangLevelTipsDesc },

		[4] = { name = "���Ƽӳ�\b",	attrID = ATTR_ID_HEAL_HP_MUL,			desc = "ʩ�����Ƽ���ʱ��������������Ч��", 
				getAttrDescFunc = GetRoleAttrHealHpPercentDesc, },

		maxElemNum = 4,
	},
},

};

function InitRoleAttrInfoTable()
	t_RoleAttrInfo["��������"][CLIENT_CAREER_MAGICOR]	= t_RoleAttrInfo["��������"][CLIENT_CAREER_WARRIOR];
	t_RoleAttrInfo["��������"][CLIENT_CAREER_CHURCH]	= t_RoleAttrInfo["��������"][CLIENT_CAREER_WARRIOR];
	t_RoleAttrInfo["��������"][CLIENT_CAREER_GUNER]		= t_RoleAttrInfo["��������"][CLIENT_CAREER_WARRIOR];

	t_RoleAttrInfo["��������"][CLIENT_CAREER_CHURCH]	= t_RoleAttrInfo["��������"][CLIENT_CAREER_MAGICOR];
	t_RoleAttrInfo["��������"][CLIENT_CAREER_GUNER]		= t_RoleAttrInfo["��������"][CLIENT_CAREER_WARRIOR];

	t_RoleAttrInfo["��������"][CLIENT_CAREER_MAGICOR]	= t_RoleAttrInfo["��������"][CLIENT_CAREER_WARRIOR];
	t_RoleAttrInfo["��������"][CLIENT_CAREER_CHURCH]	= t_RoleAttrInfo["��������"][CLIENT_CAREER_WARRIOR];
	t_RoleAttrInfo["��������"][CLIENT_CAREER_GUNER]		= t_RoleAttrInfo["��������"][CLIENT_CAREER_WARRIOR];

	t_RoleAttrInfo["Ԫ�ع�����"][CLIENT_CAREER_MAGICOR]		= t_RoleAttrInfo["Ԫ�ع�����"][CLIENT_CAREER_WARRIOR];
	t_RoleAttrInfo["Ԫ�ع�����"][CLIENT_CAREER_CHURCH]		= t_RoleAttrInfo["Ԫ�ع�����"][CLIENT_CAREER_WARRIOR];
	t_RoleAttrInfo["Ԫ�ع�����"][CLIENT_CAREER_GUNER]		= t_RoleAttrInfo["Ԫ�ع�����"][CLIENT_CAREER_WARRIOR];

	t_RoleAttrInfo["Ԫ�ط�����"][CLIENT_CAREER_MAGICOR]		= t_RoleAttrInfo["Ԫ�ط�����"][CLIENT_CAREER_WARRIOR];
	t_RoleAttrInfo["Ԫ�ط�����"][CLIENT_CAREER_CHURCH]		= t_RoleAttrInfo["Ԫ�ط�����"][CLIENT_CAREER_WARRIOR];
	t_RoleAttrInfo["Ԫ�ط�����"][CLIENT_CAREER_GUNER]		= t_RoleAttrInfo["Ԫ�ط�����"][CLIENT_CAREER_WARRIOR];

	t_RoleAttrInfo["��������"][CLIENT_CAREER_MAGICOR]		= t_RoleAttrInfo["��������"][CLIENT_CAREER_WARRIOR];
	t_RoleAttrInfo["��������"][CLIENT_CAREER_CHURCH]		= t_RoleAttrInfo["��������"][CLIENT_CAREER_WARRIOR];
	t_RoleAttrInfo["��������"][CLIENT_CAREER_GUNER]			= t_RoleAttrInfo["��������"][CLIENT_CAREER_WARRIOR];
end

function EquipFrameOnLoad()
	--this:RegisterEvent("GE_CONTAINER_UPDATE");
	this:RegisterEvent("UI_ACTOR_ATTRIBUTE_CHG");
	this:RegisterEvent("GE_MAIN_EQUIP_CHANGE");
	this:RegisterEvent("GE_ITEM_VALID_END");
	--ע���������������Ӧ�Ļص�����
	ItemFirstCallBack( "EquipFrame" , EquipArmFuncion );
	for i=1,22 do
		local bagButton = getglobal( "EquipFrameEquip"..i);
		bagButton:SetClientUserData( 0, DRAG_WEARFORM );
	end	
	InitRoleAttrInfoTable();
end

function EquipArmFuncion( used_Item, nType, nIndex )
	local MainPlayer		= ActorMgr:getMainPlayer();
	local container			= MainPlayer:getContainer();
	local Eqiup				= MainPlayer:getEquip();
	if Eqiup == nil or used_Item:getItemType() ~= ITEM_SLOTTYPE_AMM then
		return true;
	end
	if used_Item:isValidEnd() then
		return true;
	end
	-- װ����ָ���������
	local nWearPos = 0;
	if( used_Item:isRing() ) then
		nWearPos = GetEquipRingPos();
	end
	
	if used_Item:isAmuleft() then
		nWearPos = GetEquipXianlianPos();
	end
	local itemDef = used_Item:getItemDef();
	if itemDef == nil then
		return true;
	end
	if IsFashionItem( itemDef ) then 
		nWearPos = GetFashionSuitPos( itemDef.WearPos1 );
	end
	if GetArmWearLv( itemDef, used_Item ) > MainPlayer:getLv() then
		ShowMidTips("��ǰ�ȼ�����װ��");
		return true;
	end

	if itemDef.WearPre.GeniusLimit > 0 and GeniusMgr:getGeniusTypeValue(0) < itemDef.WearPre.GeniusLimit then
		ShowMidTips("��ǰ�����츳ֵ���㣬����װ��");
		return true;
	end
	local race = MainPlayer:getRace()
	local must = itemDef.WearPre.Career
	if must ~= MainPlayer:getRace() and must ~= race % 0x10 and itemDef.WearPre.Career ~= 0  then
		if getItemType( itemDef.ItemID ) == ITEM_SLOTTYPE_AMM then
			ShowMidTips("ְҵ����������װ��");
		end
		return;
	end

	if not used_Item:isItemBind() and itemDef.ArmBind == ITEM_WEAR_BIND then
		MessageBoxFrame:SetClientUserData( 0, itemDef.ItemID );
		MessageBoxFrame:SetClientUserData( 1, nWearPos );
		MessageBoxFrame:SetClientUserData( 2, used_Item:getListType());
		MessageBoxFrame:SetClientUserData( 3, used_Item:getGridIdx());

		MessageBox("��װ��","�Ƿ�װ��"..itemDef.Name.."?(װ�����)");
		return;
	end

	container:usedItem( nType, nIndex, nWearPos );
end

--@Desc:������Ӧ���������������¼���Ϣ
--@return: Nothing
function EquipFrameOnEvent()
	if arg1 == "GE_ITEM_VALID_END" then
		local frame = getglobal( "EquipFrame" );
		if frame:IsShown() then
			SetEquipInfo();
		end
	end
	
	if ( arg1 == "GE_MAIN_EQUIP_CHANGE" ) then
		local frame = getglobal( "EquipFrame" );
		if frame:IsShown() then
			 UpdateModelView();
			 SetEquipInfo();
			 UpdateEquipFrameWearEquipAttrStarButtonInfo();
		end
	end
	
	if ( arg1 == "UI_ACTOR_ATTRIBUTE_CHG" ) then
		if EquipFrame:IsShown() then
			UpdateRoleAttrBtn();
		end
	end
end

function ShowEquipGuide()
	if not GameWizard:isDo(GAMEWIZARD_FIRST_EQUIP_TO_EQUIP_FRAME) then
		local MainPlayer = ActorMgr:getMainPlayer();
		local Eqiup = MainPlayer:getEquip();
		local t_firstArms = GuideFirstArm();
		local itemId = t_firstArms[MainPlayer:getRace() % 0x10]["armId"];
		for i=1, MAXEQUIPPOS do
			local bagButton		= getglobal( "EquipFrameEquip"..i);
			local Item			= Eqiup:getItem( bagButton:GetClientID() );
			-- TODO: and Quest:isQuestRun( 1056 )
			if ( Item:getItemId() == itemId ) then
				GameWizard:doStep(GAMEWIZARD_FIRST_EQUIP_TO_EQUIP_FRAME);

				local xinShouBtn = getglobal( "EquipFrameEffectFrame" );
				xinShouBtn:SetPoint("bottomleft", bagButton:GetName(), "bottomleft", -7, 9 );
				local animationTexture = getglobal( "EquipFrameEffectFrameUVAnimationTex");
				animationTexture:SetUVAnimation( 30, true );
				xinShouBtn:Show();
			end
		end
	end
	
	-- �ƺű�ǩ
	if GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_MAIN_BTN_GET_CHENG_HAO ) and 
		not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_CHANRACTER_FRAME_LABLE_BTN_GET_CHENG_HAO ) then
		ShowEquipXinShouBtn( { btn = "EquipFrameLabelXinShouBtn", text = "����򿪳ƺ����" } );
	end

	-- ������ť
	local xinShouBtn	= getglobal( "EquipFrameLvXinShouBtn" );
	if xinShouBtn:IsShown() then
		return;
	end
	local MainPlayer	= ActorMgr:getMainPlayer();
	local nCurExp		= MainPlayer:getExp();
	local nLevelNextExp = MainPlayer:getLvExp( MainPlayer:getLv() );
	if not ( MainPlayer:getLv() >= LEVEUP_LIMIT_LV and nCurExp >= nLevelNextExp ) then
		return;
	end

	if MainPlayer:getLv() == LEVEUP_LIMIT_LV + 1 then
		if not GameWizard:isDo(GAMEWIZARD_FIRST_SELF_LEVEL_UP) then
			GameWizard:doStep(GAMEWIZARD_FIRST_SELF_LEVEL_UP);
			HideCharacterButtonLevelButton();
		end
	end

	if MainPlayer:getLv() == LEVEUP_LIMIT_LV + 2 then
		if not GameWizard:isDo(GAMEWIZARD_SECOND_SELF_LEVEL_UP) then
			GameWizard:doStep(GAMEWIZARD_SECOND_SELF_LEVEL_UP);
			HideCharacterButtonLevelButton();
		end
	end
	
	if not GameWizard:isDo(GAMEWIZARD_FIRST_SELF_LEVEL_UP) then
		if not GameWizard:isDo(GAMEWIZARD_FIRST_CLOSE_SELF_LEVEL_UP) then
			ShowEquipXinShouBtn( { btn = "EquipFrameLvXinShouBtn", text = "����ֶ�����" } );
		end
		return;
	end
	
	if not GameWizard:isDo(GAMEWIZARD_SECOND_SELF_LEVEL_UP) then
		if not GameWizard:isDo(GAMEWIZARD_SECOND_CLOSE_SELF_LEVEL_UP) then
			ShowEquipXinShouBtn( { btn = "EquipFrameLvXinShouBtn", text = "����ֶ�����" } );
		end
		return;
	end
end

function CheckLvXinShouBtn()
	if not GameWizard:isDo(GAMEWIZARD_FIRST_SELF_LEVEL_UP) and GameWizard:isDo(GAMEWIZARD_FIRST_CLOSE_SELF_LEVEL_UP) then
		GameWizard:undoStep(GAMEWIZARD_FIRST_CLOSE_SELF_LEVEL_UP);
	end

	if not GameWizard:isDo(GAMEWIZARD_SECOND_SELF_LEVEL_UP) and GameWizard:isDo(GAMEWIZARD_SECOND_CLOSE_SELF_LEVEL_UP) then
		GameWizard:undoStep(GAMEWIZARD_SECOND_CLOSE_SELF_LEVEL_UP);
	end
end

function EquipFrameLvXinShouBtn_CloseBtn_OnClick()
	EquipFrameLvXinShouBtn:Hide();
	if not GameWizard:isDo(GAMEWIZARD_FIRST_CLOSE_SELF_LEVEL_UP) then
		GameWizard:doStep(GAMEWIZARD_FIRST_CLOSE_SELF_LEVEL_UP);
		return;
	end

	if not GameWizard:isDo(GAMEWIZARD_SECOND_CLOSE_SELF_LEVEL_UP) then
		GameWizard:doStep(GAMEWIZARD_SECOND_CLOSE_SELF_LEVEL_UP);
		return;
	end
end

function ShowEquipXinShouBtn( data )
	local xinShouBtn	= getglobal( data["btn"] );
	local rich			= getglobal( xinShouBtn:GetName().."Rich" );
	
	local nSelfDefineExtendWidth = 14;
	rich:SetClientString( data["text"] );
	ReSizeTooltip( rich:GetName(), xinShouBtn:GetName(), nSelfDefineExtendWidth );
	local animationTexture	= getglobal( data["btn"].."UVAnimationTex");
	animationTexture:SetUVAnimation( 50, true );
	xinShouBtn:Show();
end

function EquipOnShow()
	--�Ϸ��� ��ɫ����
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	if string.find( mainplayer:getName(),"*",1,true ) then
		ContainerChangeRoleFrame:SetClientUserData( 0,1 );
		ContainerChangeRoleFrame:Show();
		EquipFrame:Hide();
		return;
	end
	RoleAttrBtn_AdvanceAttrLabelBtn:Checked();
	RoleAttrBtn_SpecialAttrLabelBtn:DisChecked();

	RoleAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont1:SetText("��");
	RoleAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont2:SetText("��");
	RoleAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont3:SetText("��");
	RoleAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont4:SetText("��");
	RoleAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont5:SetText("");
	RoleAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont1:SetText("��");
	RoleAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont2:SetText("��");
	RoleAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont3:SetText("��");
	RoleAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont4:SetText("��");
	RoleAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont5:SetText("");

	EquipFrameEquip1UVAnimationTex:Show();
	EquipFrameEquip1UVAnimationTex:SetUVAnimation(50, true);
	local t_frame = {"RespactFrame", "EquipDesignationFrame", "SparFrame"};
	local bShow = false;
	for _, szFrame in ipairs( t_frame ) do
		local frame = getglobal( szFrame );
		if frame:IsShown() then
			bShow = true;
			frame:Hide();
			break;
		end
	end

	--if not bShow then
		-- ���ô�������	
		--EquipFrame:SetPoint("topleft", "$parent", "topleft", 55, 163 );
	--end
	if bCharactorFrameOpenFlag then
		EquipFrame:SetPoint("topleft", "$parent", "topleft", 32, 163 );
		bCharactorFrameOpenFlag = false;
	end
	local frame 		= getglobal("EquipFrame");
	local nScreenWidth	= GetScreenWidth();
	local nScreenHeight	= GetScreenHeight();
	if frame:GetRealTop() < 0
	    or frame:GetRealLeft() < 0
	    or frame:GetRealBottom() > nScreenHeight
	    or frame:GetRealRight() > nScreenWidth then
		EquipFrame:SetPoint("topleft", "$parent", "topleft", 32, 163 );
	end
	ModelAngle = 0;
	SetEquipInfo();
	ShowEquipGuide();
	
	local t_showIcons = { "RoleAttrBtn", };
	util.ShowIcons( t_showIcons );
	EquipFrame_ShouSuoBtn:Show();
	EquipFrame_ZhanKaiBtn:Hide();
	ShowFashionFrames( false );
	EquipFrame_FashionBtn:DisChecked();
	EquipFrame_EquipBtn:Checked();	
	EquipFrameDaoBaBtn:Hide()

	UpdateEquipFrameWearEquipAttrStarButtonInfo();
	EquipFrameTitleFont:SetText( "װ ��" );
end

function IsMaxExp()
	--��ǰ����
	local MainPlayer = ActorMgr:getMainPlayer();
	local nCurExp = MainPlayer:getExp();
	if GetExpUV().pilelv == 3 then
		return true;
	end
	return false;
end

-- �����ҵ���һ���������
function GetNextLvMaxExp( nTotalLv )
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then 
		return;
	end
	
	local  nLv = MainPlayer:getLv();
	local nTotalExp = 0 
	for i = 1, (nTotalLv) do
		nTotalExp = nTotalExp + MainPlayer:getLvExp( nLv + i - 1 )
	end

	return nTotalExp;
end

function ClearEqipGrid()
	for i=1, MAXEQUIPPOS do
		local bagButton		= getglobal( "EquipFrameEquip"..i);
		local ItemTex		= getglobal( bagButton:GetName().."IconTexture");
		ItemTex:SetGray( false );
	end
end

function SetEquipInfo()
	-- �ر�������ʾ��Ч
	GuideEffectHide();
	ClearEqipGrid();

	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	local Eqiup = MainPlayer:getEquip();
	local t_BaoShiXiangQianFrameControl = GetBaoShiXiangQianFrameControl();
	local lockGrid = -1;
	local lockList = t_BaoShiXiangQianFrameControl["equipInfo"]["list"];
	if lockList == CONTAINER_TYPE_WEAR then
		lockGrid = t_BaoShiXiangQianFrameControl["equipInfo"]["grid"]
	end
	for i=1, MAXEQUIPPOS do
		local bagButton			= getglobal( "EquipFrameEquip"..i);
		local UVAnimationTex	= getglobal( bagButton:GetName().."UVAnimationTex");
		local ItemTex			= getglobal( bagButton:GetName().."IconTexture");
		local ItemBoxTex		= getglobal( bagButton:GetName().."BoxTexture");
		local redTexture		= getglobal( bagButton:GetName().."RedTexture");
		local bindTexture		= getglobal( bagButton:GetName().."BindTexture");
		local validTime			= getglobal( bagButton:GetName().."ValidTimeTex" );

		local nWearPos 			= GetFashionSuitPos( bagButton:GetClientID() );
		local Item				= Eqiup:getItem( nWearPos );

		if( Item:getItemId() ~= 0 ) then
			local itemdef = Item:getItemDef();

			SetEquipImportantEffectUV( itemdef, UVAnimationTex:GetName(),Item); -- ����װ����Ч
			bagButton:SetClientUserData(3,CONTAINER_TYPE_WEAR);

			local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
			-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			ItemTex:SetTexture( IconPath );

			ItemBoxTex:Show();
			if lockGrid ~= -1 and lockGrid == bagButton:GetClientID() then
				ItemTex:SetGray( true );
			end
			if Item:isItemBind() then
				bindTexture:Show();
			else
				bindTexture:Hide();
			end
			if Item:isValidEnd() then
				validTime:Show();
			else
				validTime:Hide();
			end
			if itemdef.WearPre.ClanLimit == 1 and (not WorldCityManager:IsWorldCityKeeper(ActorMgr:getMainPlayer()) 
				or ActorMgr:getMainPlayer():getClanPosition() ~= CLAN_POS_PRESIDENT) then
				ItemTex:SetGray( true )
			end
			if itemdef.WearPre.ClanLimit == 2 and not WorldCityManager:IsWorldCityKeeper( ActorMgr:getMainPlayer() ) then
				ItemTex:SetGray( true )
			end
		else
			UVAnimationTex:Hide();
			ItemBoxTex:Hide();
			bindTexture:Hide();
			validTime:Hide()
			ItemTex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
		end
	end
	-- ʱװ����ť����
	local nFashionNum = Eqiup:getSelectFashionNum();
	for j = 1, MAX_FASHION_BAG_NUM do 
		local fashionBtn = getglobal( "EquipFrame_FashionBag"..j )
		local fashionTex = getglobal( "EquipFrame_FashionBag"..j.."Icon");
		if j > nFashionNum + 1 then 
			fashionTex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
			fashionBtn:SetClientUserData(0, 1);
			fashionBtn:SetNormalTexture( "uires\\ZhuJieMian\\3.dds", "blend" );
			fashionBtn:SetPushedTexture( "uires\\ZhuJieMian\\3.dds", "blend" );
			fashionBtn:SetHightlightTexture( "uires\\ZhuJieMian\\3.dds", "blend" );
			fashionBtn:ChangeNormalTexture( 216,772,44,44 );
			fashionBtn:ChangeHighlightTexture( 261,772,44,44 );
			fashionBtn:ChangePushedTexture( 216,817,44,44 );
		--	fashionBtn:Disable();
		else
			fashionTex:SetTexture("uires\\ZhuJieMian\\3.dds");
			fashionBtn:SetNormalTexture( "uires\\ui\\mask\\TouMingTongDao.tga", "blend" );
			fashionBtn:SetPushedTexture( "uires\\ZhuJieMian\\1.dds", "blend" );
			fashionBtn:SetHightlightTexture( "uires\\ZhuJieMian\\1.dds", "blend" );
			fashionBtn:ChangeHighlightTexture( 794,697,44,44 );
			fashionBtn:ChangePushedTexture( 794,607,44,44 );
			if HasLeastOneFashionSuit( j ) then
				fashionTex:SetTexUV(911, 118, 44,44 );
				fashionBtn:SetClientUserData(0, 2);
			else 
				fashionTex:SetTexUV(716, 56, 44,44 );
				fashionBtn:SetClientUserData(0, 3);
			end
		--	fashionBtn:Enable();
		end
	end
	--��ʾ�����Ҫ��Ϣ
	local nLv		= MainPlayer:getLv();
	local szName	= MainPlayer:getName();
	
	local nCurExp		= MainPlayer:getExp();
	local nLevelNextExp = MainPlayer:getLvExp( MainPlayer:getLv() );
	EquipFrame_CurExp:SetText( BigInt2Str(nCurExp) );
	EquipFrame_NextUpLvExp:SetText( nLevelNextExp );

	--����ͷ���Ƿ���ʾ��Ԥ��ģ����
	local bShowHead = MainPlayer:isShowHead();
	EquipFrame_DisHat:setCheckState(bShowHead);
	--���������Ƿ���ʾ��Ԥ��ģ����
	local bShowFace = MainPlayer:isShowFace();
	EquipFrame_DisFace:setCheckState(bShowFace);
	--����ʱװ�Ƿ���ʾ��Ԥ��ģ����
	local bShowFashion = MainPlayer:isShowFashion();
	EquipFrame_DisFashion:setCheckState(bShowFashion);
	local bShowBack = MainPlayer:isShowBack();
	EquipFrame_DisBack:setCheckState(bShowBack);

	UpdateModelView();

	if RoleAttrBtn:IsShown() then
		UpdateRoleAttrBtn();
	else
		RoleAttrBtn:Show();
	end
end
local MAX_FASHION_EQUIP_NUM = 7  --ʱװ����
--���� ʱװλ|�ء�ʱװλ|ͷ��ʱװλ|����ʱװλ|����ʱװλ|Ь��ʱװλ|�֡�ʱװλ|�� 
local t_fashionPos = {
					[1] = { 8, 22, 23, 24, 25, 26, 27 },
					[2] = { 28, 29, 30, 31, 32, 33, 34 },
					[3] = { 35, 36, 37, 38, 39, 40, 41 },
					}
-- ��ʱװ��������һ�� ʱװ
function HasLeastOneFashionSuit( nIndex )
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return false end
	local Eqiup = MainPlayer:getEquip();
	for i=1, MAX_FASHION_EQUIP_NUM do 
		local Item	= Eqiup:getItem( t_fashionPos[nIndex][i] );
		if Item:getItemId() ~= 0 then 
			return true;	
		end
	end
	return false;
end

function EquipOnUpdate()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	if GameClanManager:clanGetClanNameByWid( MainPlayer:getDaoBaClanWid() ) == "" then
		 EquipFrameDaoBaBtn:Hide()
	else
		 EquipFrameDaoBaBtn:Show()
	end
end

function EquipOnHide()
	local t_hideIcons = { "EquipFrameEffectFrame", "EquipFrameLvXinShouBtn", "EquipFrameLabelXinShouBtn" };
	util.HideIcons( t_hideIcons );
	if NormalItemTipsFrame:IsShown() then
		if IsInFrameAbs( "NormalItemTipsFrame", "EquipFrame" ) then
			NormalItemTipsFrame:Hide();
		end
	end
end

function RoleActiveButton_OnLoad()
end

function RoleActiveButton_OnEvent()
end

function RoleActiveButton_OnEnter()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	--SetGameTooltips( this:GetName(), "������"..MainPlayer:getVIM().." / "..MainPlayer:getMaxVIM() );
end

function RoleActiveButton_OnLeave()
	GameTooltip:Hide();
end

function ShowFashionFrames( bShow )
	DisCheckAllBtn( "EquipFrame_FashionBag", MAX_FASHION_BAG_NUM );
	if bShow then
		for i=1,7 do
			local tex = getglobal( "EquipFrameLeftEquipbackground" .. i );
			tex:Hide();
			tex = getglobal( "EquipFrameRightEquipbackground" .. i );
			tex:Hide();
		end
		for i=1,16 do
			local btn = getglobal( "EquipFrameEquip" .. i );
			btn:Hide();
		end
		-- ����
		EquipFrameCheckBkg1:Hide();
		EquipFrameCheckBkg2:Hide();
		EquipFrameExpBkg:Hide();
		EquipFrame_CurExpDesc:Hide();
		EquipFrame_NextUpLvExpDesc:Hide();
		EquipFrame_CurExp:Hide();
		EquipFrame_NextUpLvExp:Hide();
		EquipFrame_DisHat:Hide();
		EquipFrame_DisFashion:Hide();
		EquipFrame_UpLvBtn:Hide();
		EquipFrame_DisFace:Hide();

		EquipFrameFashionBkg:Show();
		for i=1,9 do
			local tex = getglobal( "EquipFrameFashionItemBkg" ..i );
			tex:Show();
		end
		EquipFrameEquip8:Show();
		EquipFrameEquip17:Show();
		EquipFrameEquip18:Show();
		EquipFrameEquip19:Show();
		EquipFrameEquip20:Show();
		EquipFrameEquip21:Show();
		EquipFrameEquip22:Show();
		EquipFrame_ZhanKaiBtn:Hide();
		EquipFrame_ShouSuoBtn:Hide();
		EquipFrame_DisFashion:Show();
		EquipFrame_DisBack:Show();
		EquipFrame_FashionBag1:Show();
		EquipFrame_FashionBag2:Show();
		EquipFrame_FashionBag3:Show();
		local MainPlayer = ActorMgr:getMainPlayer();
		if MainPlayer == nil then return end
		local Eqiup = MainPlayer:getEquip();
		local index = Eqiup:getSelectFashionIndex() + 1;
		local fBagBtn = getglobal( "EquipFrame_FashionBag"..index);
		fBagBtn:Checked();
	else
		for i=1,7 do
			local tex = getglobal( "EquipFrameLeftEquipbackground" .. i );
			tex:Show();
			tex = getglobal( "EquipFrameRightEquipbackground" .. i );
			tex:Show();
		end
		for i=1,16 do
			local btn = getglobal( "EquipFrameEquip" .. i );
			btn:Show();
		end
		-- ����
		EquipFrameCheckBkg1:Show();
		EquipFrameCheckBkg2:Show();
		EquipFrameExpBkg:Show();
		EquipFrame_CurExpDesc:Show();
		EquipFrame_NextUpLvExpDesc:Show();
		EquipFrame_CurExp:Show();
		EquipFrame_NextUpLvExp:Show();
		EquipFrame_DisHat:Show();
		EquipFrame_DisFashion:Show();
		EquipFrame_UpLvBtn:Show();
		EquipFrame_DisFashion:Hide();
		EquipFrame_DisBack:Hide();
		EquipFrame_DisFace:Show();
		EquipFrameFashionBkg:Hide();
		for i=1,9 do
			local tex = getglobal( "EquipFrameFashionItemBkg" ..i );
			tex:Hide();
		end
		EquipFrameEquip8:Hide();
		EquipFrameEquip17:Hide();
		EquipFrameEquip18:Hide();
		EquipFrameEquip19:Hide();
		EquipFrameEquip20:Hide();
		EquipFrameEquip21:Hide();
		EquipFrameEquip22:Hide();
		EquipFrame_FashionBag1:Hide();
		EquipFrame_FashionBag2:Hide();
		EquipFrame_FashionBag3:Hide();
		if RoleAttrBtn:IsShown() then
			EquipFrame_ShouSuoBtn:Show();
			EquipFrame_ZhanKaiBtn:Hide();
		else
			EquipFrame_ShouSuoBtn:Hide();
			EquipFrame_ZhanKaiBtn:Show();
		end		
	end
	bEquipFrameShowFashion = bShow;
	UpdateModelView();
	EquipFrameEquip7:Hide();
end

function EquipBtn_OnClick()
	local desframe	= getglobal( "EquipFrame" );
	if desframe:IsShown() and not bEquipFrameShowFashion then
		return;
	end

	local parentframe = getglobal( this:GetParent() );
	
	local deschenghaobtn	= getglobal( desframe:GetName().."_ChengHaoBtn" );
	local desfashionbtn		= getglobal( desframe:GetName().."_FashionBtn" );  
	local desshengwangbtn	= getglobal( desframe:GetName().."_ShengWangBtn" );
	local deseNengYuanBtn	= getglobal( desframe:GetName().."_NengYuanBtn" );
	local desequipbtn		= getglobal( desframe:GetName().."_EquipBtn" );
	
	parentframe:Hide();
	desframe:Show();
	desequipbtn:Checked();
	deschenghaobtn:DisChecked(); 
	desfashionbtn:DisChecked();
	desshengwangbtn:DisChecked();
	deseNengYuanBtn:DisChecked();	  	
	--��ʾ������
	EquipFrameTitleFont:SetText( "װ ��" );
	desframe:SetPoint("topleft", "UIClient", "topleft",parentframe:GetRealLeft(), parentframe:GetRealTop());
	ShowFashionFrames( false );
end

function FashionBtn_OnClick()
	local desframe	= getglobal( "EquipFrame" );
	if desframe:IsShown() and bEquipFrameShowFashion then
		return;
	end
	local parentframe = getglobal( this:GetParent() );
	
	local deschenghaobtn	= getglobal( desframe:GetName().."_ChengHaoBtn" );
	local desfashionbtn		= getglobal( desframe:GetName().."_FashionBtn" );  
	local desshengwangbtn	= getglobal( desframe:GetName().."_ShengWangBtn" );
	local deseNengYuanBtn	= getglobal( desframe:GetName().."_NengYuanBtn" );
	local desequipbtn		= getglobal( desframe:GetName().."_EquipBtn" );
	
	parentframe:Hide();
	desframe:Show();
	desequipbtn:DisChecked();
	deschenghaobtn:DisChecked(); 
	desfashionbtn:Checked();
	desshengwangbtn:DisChecked();
	deseNengYuanBtn:DisChecked();
	--��ʾ������
	EquipFrameTitleFont:SetText( "ʱ װ" );
	desframe:SetPoint("topleft", "UIClient", "topleft",parentframe:GetRealLeft(), parentframe:GetRealTop());
	ShowFashionFrames( true );
	EquipFrameLvXinShouBtn:Hide();
end

function NengYuanBtn_OnClick()
	
	local parentframe = getglobal( this:GetParent() );
	local desframe	= getglobal( "SparFrame" );  
	local deschenghaobtn	= getglobal( desframe:GetName().."_ChengHaoBtn" );
	local desfashionbtn		= getglobal( desframe:GetName().."_FashionBtn" );  
	local desshengwangbtn	= getglobal( desframe:GetName().."_ShengWangBtn" );
	local deseNengYuanBtn	= getglobal( desframe:GetName().."_NengYuanBtn" );
	local desequipbtn		= getglobal( desframe:GetName().."_EquipBtn" );
	
	parentframe:Hide();
	desframe:Show();
	desequipbtn:DisChecked();
	deschenghaobtn:DisChecked(); 
	desfashionbtn:DisChecked();
	desshengwangbtn:DisChecked();
	deseNengYuanBtn:Checked();	  	

	desframe:SetPoint("topleft", "UIClient", "topleft",parentframe:GetRealLeft(), parentframe:GetRealTop());
end

function JinglingBtn_OnClick()
	local parentframe = getglobal( this:GetParent() );
	local desframe	= getglobal( "TrumpFrame" );  
	local deschenghaobtn	= getglobal( desframe:GetName().."_ChengHaoBtn" );
	local desfashionbtn		= getglobal( desframe:GetName().."_FashionBtn" );  
	local desshengwangbtn	= getglobal( desframe:GetName().."_ShengWangBtn" );
	local deseNengYuanBtn	= getglobal( desframe:GetName().."_NengYuanBtn" );
	local desequipbtn		= getglobal( desframe:GetName().."_EquipBtn" );	 
	
	parentframe:Hide();
	desframe:Show();		     	
	desequipbtn:DisChecked();
	deschenghaobtn:DisChecked(); 
	desfashionbtn:DisChecked();
	desshengwangbtn:DisChecked();
	deseNengYuanBtn:DisChecked();		

	desframe:SetPoint("topleft", "UIClient", "topleft",parentframe:GetRealLeft(), parentframe:GetRealTop());
end

function ChengHaoBtn_OnClick()
	local btn = getglobal( "EquipFrameLabelXinShouBtn");
	if this:GetParent() == "EquipFrame" and btn:IsShown() then
		GameWizard:doStep( GAMEWIZARD_FIRST_BLINK_CHANRACTER_FRAME_LABLE_BTN_GET_CHENG_HAO );
		btn:Hide();
	end
	
	local desframe			= getglobal( "EquipDesignationFrame" );
	if desframe:IsShown() then
		return;
	end

	local parentframe		= getglobal( this:GetParent() );
	local deschenghaobtn	= getglobal( desframe:GetName().."_ChengHaoBtn" );
	local desfashionbtn		= getglobal( desframe:GetName().."_FashionBtn" );  
	local desshengwangbtn	= getglobal( desframe:GetName().."_ShengWangBtn" );
	local deseNengYuanBtn	= getglobal( desframe:GetName().."_NengYuanBtn" );
	local desequipbtn		= getglobal( desframe:GetName().."_EquipBtn" );	  
	
	parentframe:Hide();
	desframe:Show();
	deschenghaobtn:Checked();
	desfashionbtn:DisChecked();
	desshengwangbtn:DisChecked();
	deseNengYuanBtn:DisChecked();	
	desequipbtn:DisChecked();
	desframe:SetPoint("topleft", "UIClient", "topleft",parentframe:GetRealLeft(), parentframe:GetRealTop());
end

function ShengWangBtn_OnClick()
	local desframe	= getglobal( "RespactFrame" );  
	if desframe:IsShown() then
		return;
	end

	local parentframe = getglobal( this:GetParent() );
	
	local deschenghaobtn	= getglobal( desframe:GetName().."_ChengHaoBtn" );
	local desfashionbtn		= getglobal( desframe:GetName().."_FashionBtn" );  
	local desshengwangbtn	= getglobal( desframe:GetName().."_ShengWangBtn" );
	local deseNengYuanBtn	= getglobal( desframe:GetName().."_NengYuanBtn" );
	local desequipbtn		= getglobal( desframe:GetName().."_EquipBtn" );	  

	parentframe:Hide();
	desframe:Show();
	desequipbtn:DisChecked();
	deschenghaobtn:DisChecked(); 
	desfashionbtn:DisChecked();
	desshengwangbtn:Checked();
	deseNengYuanBtn:DisChecked();	

	desframe:SetPoint("topleft", "UIClient", "topleft",parentframe:GetRealLeft(), parentframe:GetRealTop());
end

function EquipFrame_PetBtn_OnClick()
	RideFrame:Hide();
	EquipFrame:Hide();
	EquipRealX	= EquipFrame:GetRealLeft();
	EquipRealY	= EquipFrame:GetRealTop();
	PetFrame:Show();
end

--@desc:�����ת��ťʱ����ת��Ԥ��ģ��
--@return: Nothing
function EquipFrame_TurnLeftBtn_OnMouseDown()
	local ModelViewer = getglobal("EquipFrame_ModelView");
	ModelAngle = ModelAngle - 5;
	ModelViewer:setEntityRotate(0,ModelAngle);
end

--@desc:�������ת��ťʱ����ת��Ԥ��ģ��
--@return: Nothing
function EquipFrame_TurnRightBtn_OnMouseDown()
	local ModelViewer = getglobal("EquipFrame_ModelView");
	ModelAngle = ModelAngle + 5;
	ModelViewer:setEntityRotate(0,ModelAngle);
end

function RoleAttrBtn_CloseBtn_OnClick()
	RoleAttrBtn:Hide();
end

--@desc:����Ԥ��ģ������װ��
--@Param: [in]nIndex			ָ����ɫ�����Ե�ö��ֵ
--@return:[out]nValue			����ָ�����Եľ�����ֵ

function RoleAttrBtn_OnShow()
	RoleAttrBtn_AdvanceAttrLabelBtn:Checked();
	--RoleAttrBtn_AdvanceAttrLabelBtn:Highlight();

	RoleAttrBtn_SpecialAttrLabelBtn:DisChecked();
	--RoleAttrBtn_SpecialAttrLabelBtn:DisHighlight();

	UpdateRoleAttrBtn();
	
	local t_showIcons = { "EquipFrame_ShouSuoBtn", };
	if not bEquipFrameShowFashion then
		util.ShowIcons( t_showIcons );
	else
		util.HideIcons( t_showIcons );
	end
	local t_hideIcons = { "EquipFrame_ZhanKaiBtn", };
	util.HideIcons( t_hideIcons );
end

function RoleAttrBtn_OnHide()
	local t_showIcons = { "EquipFrame_ZhanKaiBtn", };
	if not bEquipFrameShowFashion then
		util.ShowIcons( t_showIcons );
	else
		util.HideIcons( t_showIcons );
	end

	local t_hideIcons = { "EquipFrame_ShouSuoBtn", };
	util.HideIcons( t_hideIcons );
end
	
-- ���»�������
function ClearBaseAttr()
	for i = 1, MAX_BASE_ATTR_NUM do
		local btn = getglobal( "RoleAttrBtn_BaseAttrBtn"..i );
		btn:Hide();
	end
end

function UpdateBaseAttr()
	ClearBaseAttr();

	local mainplayer	= ActorMgr:getMainPlayer();
	local initRace		= GetInitRace( mainplayer:getRace() );
	
	local t_attrList	= t_RoleAttrInfo["��������"][initRace];
	local maxAttrNum	= t_attrList["maxElemNum"];
	for i = 1, maxAttrNum do
		local oneElem		= t_attrList[i];
		local funcGetBase	= oneElem["getBaseAttrFunc"] ~= nil and oneElem["getBaseAttrFunc"]	or GetRoleBaseAttr;
		local funcGetCur	= oneElem["getCurAttrFunc"] ~= nil	and oneElem["getCurAttrFunc"]	or GetRoleAttr;

		local nCurValue		= funcGetCur( oneElem["attrID"],	mainplayer );
		local nBaseValue	= funcGetBase( oneElem["attrID"],	mainplayer );
		--print( "ATTR_ID = "..oneElem["attrID"].."�� nCurValue = "..nCurValue.."�� nBaseValue = "..nBaseValue );

		local t_color	= { r = 255, g = 254, b = 189 };
		if nCurValue > nBaseValue then
			t_color	= { r = 15, g = 255, b = 0 };
		elseif nCurValue < nBaseValue then
			t_color	= { r = 255, g = 0, b = 0 };
		end

		local btn = getglobal( "RoleAttrBtn_BaseAttrBtn"..i );
		
		local funcGetTips = oneElem["getAttrTipsDescFunc"];
		if funcGetTips ~= nil then
			btn:SetClientString( funcGetTips( oneElem["attrID"], mainplayer ) );
		else
			btn:SetClientString( oneElem["desc"] );
		end

		local funcGetDesc = oneElem["getAttrDescFunc"];
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneElem["name"] }, 
							[btn:GetName().."ValueFont"]	= 
							{	value = funcGetDesc ~= nil and funcGetDesc( oneElem["attrID"], mainplayer ) or nCurValue,	
								color = t_color, 
							},
						};
		util.SetFonts( t_fonts );
		btn:Show();
	end
	
	local mainAttrFlagBtn		= getglobal( "RoleAttrBtnMainAttrFlagButton" );
	local t_changeCareerControl = GetChangeCareerControl();
	local nRace					= mainplayer:getRace();
	if t_changeCareerControl:isChangeCareer( { ["race"] = nRace } ) then
		local szAttrBtn = t_mainAttrConfig[nRace]["btn"];
		mainAttrFlagBtn:SetPoint( "left", szAttrBtn, "left", 35, 0 );
		mainAttrFlagBtn:Show();
	else
		mainAttrFlagBtn:Hide();
	end
end

-- ������ͨ��������
function ClearNormalAttackAttr()
	for i = 1, MAX_NORMAL_ATTACK_ATTR_NUM do
		local btn = getglobal( "RoleAttrBtn_AttackAttrBtn"..i );
		btn:Hide();
	end
end

function UpdateNormalAttackAttr()
	ClearNormalAttackAttr();

	local mainplayer	= ActorMgr:getMainPlayer();
	local initRace		= GetInitRace( mainplayer:getRace() );
	
	local t_attrList	= t_RoleAttrInfo["��������"][initRace];
	local maxAttrNum	= t_attrList["maxElemNum"];

	for i = 1, maxAttrNum do
		local oneElem		= t_attrList[i];

		local funcGetBase	= oneElem["getBaseAttrFunc"] ~= nil and oneElem["getBaseAttrFunc"]	or GetRoleBaseAttr;
		local funcGetCur	= oneElem["getCurAttrFunc"] ~= nil	and oneElem["getCurAttrFunc"]	or GetRoleAttr;

		local nCurValue		= funcGetCur( oneElem["attrID"],	mainplayer );
		local nBaseValue	= funcGetBase( oneElem["attrID"],	mainplayer );

		local t_color	= { r = 255, g = 254, b = 189 };
		if nCurValue > nBaseValue then
			t_color	= { r = 15, g = 255, b = 0 };
		elseif nCurValue < nBaseValue then
			t_color	= { r = 255, g = 0, b = 0 };
		end

		local nCurMaxValue  = nil;
		local nBaseMaxValue = nil;

		if oneElem["maxAttrID"] ~= nil then
			nCurMaxValue = funcGetCur( oneElem["maxAttrID"],	mainplayer );
			nBaseMaxValue = funcGetBase( oneElem["maxAttrID"],	mainplayer );
			if nCurMaxValue < nBaseMaxValue then
				t_color	= { r = 255, g = 0, b = 0 };
			end
			nCurValue = nCurValue .. "-" .. nCurMaxValue
		end		

		local btn = getglobal( "RoleAttrBtn_AttackAttrBtn"..i );

		local funcGetTips = oneElem["getAttrTipsDescFunc"];
		if funcGetTips ~= nil then
			btn:SetClientString( funcGetTips( oneElem["attrID"], mainplayer ) );
		else
			btn:SetClientString( oneElem["desc"] );
		end
		
		local funcGetDesc = oneElem["getAttrDescFunc"];
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneElem["name"] }, 
							[btn:GetName().."ValueFont"]	= 
							{	value = funcGetDesc ~= nil and funcGetDesc( oneElem["attrID"], mainplayer ) or nCurValue,	
								color = t_color, 
							},
						};
		util.SetFonts( t_fonts );
		btn:Show();
	end
end

-- ������ͨ��������
function ClearNormalDefendAttr()
	for i = 1, MAX_NORMAL_DEFEND_ATTR_NUM do
		local btn = getglobal( "RoleAttrBtn_DefendAttrBtn"..i );
		btn:Hide();
	end
end

function UpdateNormalDefendAttr()
	ClearNormalDefendAttr();

	local mainplayer	= ActorMgr:getMainPlayer();
	local initRace		= GetInitRace( mainplayer:getRace() );
	
	local t_attrList	= t_RoleAttrInfo["��������"][initRace];
	local maxAttrNum	= t_attrList["maxElemNum"];

	for i = 1, maxAttrNum do
		local oneElem		= t_attrList[i];
		local funcGetBase	= oneElem["getBaseAttrFunc"] ~= nil and oneElem["getBaseAttrFunc"]	or GetRoleBaseAttr;
		local funcGetCur	= oneElem["getCurAttrFunc"] ~= nil	and oneElem["getCurAttrFunc"]	or GetRoleAttr;

		local nCurValue		= funcGetCur( oneElem["attrID"],	mainplayer );
		local nBaseValue	= funcGetBase( oneElem["attrID"],	mainplayer );

		local t_color	= { r = 255, g = 254, b = 189 };
		if nCurValue > nBaseValue then
			t_color	= { r = 15, g = 255, b = 0 };
		elseif nCurValue < nBaseValue then
			t_color	= { r = 255, g = 0, b = 0 };
		end

		if oneElem["maxAttrID"] ~= nil then
			nCurMaxValue = funcGetCur( oneElem["maxAttrID"],	mainplayer );
			nBaseMaxValue = funcGetBase( oneElem["maxAttrID"],	mainplayer );
			if nCurMaxValue < nBaseMaxValue then
				t_color	= { r = 255, g = 0, b = 0 };
			end
			nCurValue = nCurValue .. "-" .. nCurMaxValue
		end

		local btn			= getglobal( "RoleAttrBtn_DefendAttrBtn"..i );

		local funcGetTips = oneElem["getAttrTipsDescFunc"];
		if funcGetTips ~= nil then
			btn:SetClientString( funcGetTips( oneElem["attrID"], mainplayer ) );
		else
			btn:SetClientString( oneElem["desc"] );
		end
		local funcGetDesc	= oneElem["getAttrDescFunc"];
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneElem["name"] }, 
							[btn:GetName().."ValueFont"]	= 
							{	value = funcGetDesc ~= nil and funcGetDesc( oneElem["attrID"], mainplayer ) or nCurValue,	
								color = t_color, 
							},
						};
		util.SetFonts( t_fonts );
		btn:Show();
	end
end

-- �������⹥������
function ClearYuanSuAttackAttr()
	for i = 1, MAX_YUAN_SU_ATTACK_ATTR_NUM do
		local btn = getglobal( "RoleAttrBtn_YuanSuAttackAttrBtn"..i );
		btn:Hide();
	end
end

function UpdateSpecialAttackAttr()
	ClearYuanSuAttackAttr();

	local mainplayer	= ActorMgr:getMainPlayer();
	local initRace		= GetInitRace( mainplayer:getRace() );
	
	local t_attrList	= t_RoleAttrInfo["Ԫ�ع�����"][initRace];
	local maxAttrNum	= t_attrList["maxElemNum"];

	for i = 1, maxAttrNum do
		local oneElem		= t_attrList[i];
		local funcGetBase	= oneElem["getBaseAttrFunc"] ~= nil and oneElem["getBaseAttrFunc"]	or GetRoleBaseAttr;
		local funcGetCur	= oneElem["getCurAttrFunc"] ~= nil	and oneElem["getCurAttrFunc"]	or GetRoleAttr;

		local nCurValue		= funcGetCur( oneElem["attrID"],	mainplayer );
		local nBaseValue	= funcGetBase( oneElem["attrID"],	mainplayer );

		local t_color	= { r = 255, g = 254, b = 189 };
		if nCurValue > nBaseValue then
			t_color	= { r = 15, g = 255, b = 0 };
		elseif nCurValue < nBaseValue then
			t_color	= { r = 255, g = 0, b = 0 };
		end

		if oneElem["maxAttrID"] ~= nil then
			nCurMaxValue = funcGetCur( oneElem["maxAttrID"],	mainplayer );
			nBaseMaxValue = funcGetBase( oneElem["maxAttrID"],	mainplayer );
			if nCurMaxValue < nBaseMaxValue then
				t_color	= { r = 255, g = 0, b = 0 };
			end
			nCurValue = nCurValue .. "-" .. nCurMaxValue
		end

		local btn = getglobal( "RoleAttrBtn_YuanSuAttackAttrBtn"..i );
		btn:SetClientString( oneElem["desc"] );
		local funcGetDesc = oneElem["getAttrDescFunc"];
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneElem["name"] }, 
							[btn:GetName().."ValueFont"]	= 
							{	value = funcGetDesc ~= nil and funcGetDesc( oneElem["attrID"], mainplayer ) or nCurValue,	
								color = t_color, 
							},
						};
		util.SetFonts( t_fonts );
		btn:Show();
	end
end

-- ���������������
function ClearYuanSuDefendAttr()
	for i = 1, MAX_YUAN_SU_DEFEND_ATTR_NUM do
		local btn = getglobal( "RoleAttrBtn_YuanSuDefendAttrBtn"..i );
		btn:Hide();
	end
end

function UpdateSpecialDefendAttr()
	ClearYuanSuDefendAttr();

	local mainplayer	= ActorMgr:getMainPlayer();
	local initRace		= GetInitRace( mainplayer:getRace() );
	
	local t_attrList	= t_RoleAttrInfo["Ԫ�ط�����"][initRace];
	local maxAttrNum	= t_attrList["maxElemNum"];

	for i = 1, maxAttrNum do
		local oneElem		= t_attrList[i];
		local funcGetBase	= oneElem["getBaseAttrFunc"] ~= nil and oneElem["getBaseAttrFunc"]	or GetRoleBaseAttr;
		local funcGetCur	= oneElem["getCurAttrFunc"] ~= nil	and oneElem["getCurAttrFunc"]	or GetRoleAttr;

		local nCurValue		= funcGetCur( oneElem["attrID"],	mainplayer );
		local nBaseValue	= funcGetBase( oneElem["attrID"],	mainplayer );

		local t_color	= { r = 255, g = 254, b = 189 };
		if nCurValue > nBaseValue then
			t_color	= { r = 15, g = 255, b = 0 };
		elseif nCurValue < nBaseValue then
			t_color	= { r = 255, g = 0, b = 0 };
		end

		if oneElem["maxAttrID"] ~= nil then
			nCurMaxValue = funcGetCur( oneElem["maxAttrID"],	mainplayer );
			nBaseMaxValue = funcGetBase( oneElem["maxAttrID"],	mainplayer );
			if nCurMaxValue < nBaseMaxValue then
				t_color	= { r = 255, g = 0, b = 0 };
			end
			nCurValue = nCurValue .. "-" .. nCurMaxValue
		end

		local btn			= getglobal( "RoleAttrBtn_YuanSuDefendAttrBtn"..i );
		btn:SetClientString( oneElem["desc"] );
		local funcGetDesc	= oneElem["getAttrDescFunc"];
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneElem["name"] }, 
							[btn:GetName().."ValueFont"]	= 
							{	value = funcGetDesc ~= nil and funcGetDesc( oneElem["attrID"], mainplayer ) or nCurValue,	
								color = t_color, 
							},
						};
		util.SetFonts( t_fonts );
		btn:Show();
	end
end

function ClearFuZhuAttr()
	for i = 1, MAX_FU_ZHU_ATTR_NUM do
		local btn = getglobal( "RoleAttrBtn_FuZhuAttrBtn"..i );
		btn:Hide();
	end
end

function UpdateFuZhuAttr()
	ClearFuZhuAttr();

	local mainplayer	= ActorMgr:getMainPlayer();
	local initRace		= GetInitRace( mainplayer:getRace() );
	
	local t_attrList	= t_RoleAttrInfo["��������"][initRace];
	local maxAttrNum	= t_attrList["maxElemNum"];

	for i = 1, maxAttrNum do
		local oneElem		= t_attrList[i];
		local funcGetBase	= oneElem["getBaseAttrFunc"] ~= nil and oneElem["getBaseAttrFunc"]	or GetRoleBaseAttr;
		local funcGetCur	= oneElem["getCurAttrFunc"] ~= nil	and oneElem["getCurAttrFunc"]	or GetRoleAttr;

		local nCurValue		= funcGetCur( oneElem["attrID"],	mainplayer );
		local nBaseValue	= funcGetBase( oneElem["attrID"],	mainplayer );

		local t_color	= { r = 255, g = 254, b = 189 };
		if nCurValue > nBaseValue then
			t_color	= { r = 15, g = 255, b = 0 };
		elseif nCurValue < nBaseValue then
			t_color	= { r = 255, g = 0, b = 0 };
		end

		if oneElem["maxAttrID"] ~= nil then
			nCurMaxValue = funcGetCur( oneElem["maxAttrID"],	mainplayer );
			nBaseMaxValue = funcGetBase( oneElem["maxAttrID"],	mainplayer );
			if nCurMaxValue < nBaseMaxValue then
				t_color	= { r = 255, g = 0, b = 0 };
			end
			nCurValue = nCurValue .. "-" .. nCurMaxValue
		end

		local btn = getglobal( "RoleAttrBtn_FuZhuAttrBtn"..i );

		local funcGetTips = oneElem["getAttrTipsDescFunc"];
		if funcGetTips ~= nil then
			btn:SetClientString( funcGetTips( oneElem["attrID"], mainplayer ) );
		else
			btn:SetClientString( oneElem["desc"] );
		end

		local funcGetDesc	= oneElem["getAttrDescFunc"];
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneElem["name"] }, 
							[btn:GetName().."ValueFont"]	= 
							{	value = funcGetDesc ~= nil and funcGetDesc( oneElem["attrID"], mainplayer ) or nCurValue,	
								color = t_color, 
							},
						};
		util.SetFonts( t_fonts );
		btn:Show();
	end
end

-- ������װ����
function ClearSuitAttr()
	for i = 1, MAX_SUIT_ATTR_NUM do
		local btn = getglobal( "RoleAttrBtn_SuitAttrBtn"..i );
		btn:Hide();
	end
	
	local t_hideIcons = { "RoleAttrBtnSuitBackGroundButton", };
	util.HideIcons( t_hideIcons );
end

function UpdateSuitAttr()
	ClearSuitAttr();
	
	local t_SuitList	= {};
	local mainplayer	= ActorMgr:getMainPlayer();
	local Eqiup			= mainplayer:getEquip();

	local isHaveSuitFunc = 
	function ( itemDef )
		for _, data in ipairs( t_SuitList ) do
			if itemDef.SuitSetID == data["suitID"] then
				return true;
			end
		end

		return false;
	end

	for i = 1, MAXEQUIPPOS do
		local bagButton	= getglobal( "EquipFrameEquip"..i);
		local Item		= Eqiup:getItem( bagButton:GetClientID() );
		local itemDef	= Item:getItemDef();
		if itemDef ~= nil and itemDef.SuitSetID > 0 and not Item:isValidEnd() then
			if not isHaveSuitFunc( itemDef ) then
				local SuitSet		= getSuitSet( itemDef.SuitSetID );
				local t_SuitColor	= GetItemNameColor(itemDef);

				local t_oneSuitElem = 
				{	name	= SuitSet.Name,
					value	= Eqiup:getPlayerSuitEquipNum( itemDef.SuitSetID ) .. "/"..getSuitEquipNum( itemDef.SuitSetID ),
					color	= { r = t_SuitColor[1], g = t_SuitColor[2], b = t_SuitColor[3] },
					suitID	= itemDef.SuitSetID,
				};
				table.insert( t_SuitList, t_oneSuitElem );
			end
		end
	end
	
	-- ������װ����ť����
	for i = 1, table.getn( t_SuitList ) do
		local oneSuitData	= t_SuitList[i];
		local btn			= getglobal( "RoleAttrBtn_SuitAttrBtn"..i );
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneSuitData["name"],	color = oneSuitData["color"],  }, 
							[btn:GetName().."ValueFont"]	= {	value = oneSuitData["value"],	color = oneSuitData["color"], },
						};
		util.SetFonts( t_fonts );
		btn:Show();
	end
	
	local ONE_SUIT_BTN_HEIGHT			= 16;
	local EXTEND_FRAME_SIZE_LIMIT_NUM	= 2;
	local nSuitNum	= table.getn( t_SuitList );
	if nSuitNum > 0 then
		RoleAttrBtnSuitBackGroundButton:Show();
		RoleAttrBtnSuitBackGroundButton:SetHeight( math.max( EXTEND_FRAME_SIZE_LIMIT_NUM, table.getn( t_SuitList ) ) * ONE_SUIT_BTN_HEIGHT );
	end
	
	if nSuitNum > EXTEND_FRAME_SIZE_LIMIT_NUM then
		local size = 456 - 32 - MAX_BOTTOM_LEAVE_SPACE +  nSuitNum * 16;
		RoleAttrBtn:SetHeight( size );
	else
		RoleAttrBtn:SetHeight( 456 );
	end
	
end

-- �߼����Ա�ǩ
function RoleAttrBtn_AdvanceAttrLabelBtn_OnClick()
	RoleAttrBtn_AdvanceAttrLabelBtn:Checked();
	--RoleAttrBtn_AdvanceAttrLabelBtn:Highlight();

	RoleAttrBtn_SpecialAttrLabelBtn:DisChecked();
	--RoleAttrBtn_SpecialAttrLabelBtn:DisHighlight();

	UpdateNormalAttackAttr();
	UpdateNormalDefendAttr();

	Private_ShowEquipAdvanceAttr();

	RoleAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont1:SetText("��");
	RoleAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont2:SetText("��");
	RoleAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont3:SetText("��");
	RoleAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont4:SetText("��");
	RoleAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont5:SetText("");
	RoleAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont1:SetText("��");
	RoleAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont2:SetText("��");
	RoleAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont3:SetText("��");
	RoleAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont4:SetText("��");
	RoleAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont5:SetText("");

end

-- �������Ա�ǩ
function RoleAttrBtn_SpecialAttrLabelBtn_OnClick()
	RoleAttrBtn_SpecialAttrLabelBtn:Checked();
	--RoleAttrBtn_SpecialAttrLabelBtn:Highlight();

	RoleAttrBtn_AdvanceAttrLabelBtn:DisChecked();
	--RoleAttrBtn_AdvanceAttrLabelBtn:DisHighlight();
	
	Private_ShowEquipYuanSuAttr();

	UpdateSpecialAttackAttr();
	UpdateSpecialDefendAttr();
	UpdateFuZhuAttr();
end

function RoleAttrBtn_NodeBtn_OnEnter()
	local szText = this:GetClientString();
	if szText == "" then
		return;
	end
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = "cursor", vertical = "top", color = {255,150,0} } );
end

function RoleAttrBtn_NodeBtn_OnLeave()
	GameTooltip:Hide();
end

function UpdateRoleAttrBtn()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then
		return;
	end

	local nCurExp		= mainplayer:getExp();
	local nLevelNextExp = mainplayer:getLvExp( mainplayer:getLv() );
	if mainplayer:getLv() >= LEVEUP_LIMIT_LV and nCurExp >= nLevelNextExp then
		EquipFrame_UpLvBtn:Enable();
	else
		EquipFrame_UpLvBtn:Disable();
	end
	local levelUpDef = getLevelUP( mainplayer:getLv() )
	-- ˢ����һ���UI
	local nLv		= mainplayer:getLv();
	local t_fonts = 
	{ 
		--["RoleActiveButtonFontText"]	= { value = mainplayer:getVIM().." / "..mainplayer:getMaxVIM() }, 
		["RoleAttrNameFont"]			= { value = mainplayer:getName() }, 
		["RoleAttrLvFont"]				= { value = "�ȼ���"..mainplayer:getLv().."��" }, 
		["RoleAttrRaceFont"]			= { value = "ְҵ��"..GetCareerName( mainplayer:getRace() ) }, 
		["RoleAttrGenuisFont"]			= { value =  nLv >= 60 and ("���츳ֵ��" .. GeniusMgr:getAllGeniusValue() ) or "" , color = (GeniusMgr:getAllGeniusValue() >= levelUpDef.FzMod) and {r=255,g=150,b=0} or {r=255,g=0,b=0} },
		["EquipFrame_CurExp"]			= { value = BigInt2Str(nCurExp) }, 
		["EquipFrame_NextUpLvExp"]		= { value = nLevelNextExp }, 
	};
	util.SetFonts( t_fonts );
	
	-- ����HP��MP��
	local MAX_HP_LENGTH = 152;
	--��ʾ������
	-- ����HP
	local maxHp		= mainplayer:getMaxHP();
	local hp		= mainplayer:getHP();
	hp				= math.max( math.min( hp, maxHp ), 0 );
	local fHpScale	= math.max( math.min( hp / maxHp, 1 ), 0 );
	local hpBtn		= getglobal( "RoleAttrBtnFirstBaseLifeAttrButton" );
	local hpTex		= getglobal(  hpBtn:GetName().."Tex" );
	hpTex:SetSize(  fHpScale * MAX_HP_LENGTH, 11 );
	
	-- ����MP
	local maxMp		= mainplayer:getMaxMP();
	local mp		= mainplayer:getMP();
	mp				= math.max( math.min( mp, maxMp ), 0 );
	local fMpScale	= math.max( math.min( mp / maxMp, 1 ), 0 );
	local mpBtn		= getglobal( "RoleAttrBtnFirstBaseMagicAttrButton" );
	local mpTex		= getglobal(  mpBtn:GetName().."Tex" );
	mpTex:SetSize( fMpScale * MAX_HP_LENGTH, 11 );

	local t_fonts = {	[hpBtn:GetName().."PercentFont"] = { value = hp.."/"..maxHp }, 
						[mpBtn:GetName().."PercentFont"] = { value = mp.."/"..maxMp },
					};
	util.SetFonts( t_fonts );

	UpdateBaseAttr();
	-- ��ѡ��ĸ߼�����	IsEnable
	if RoleAttrBtn_AdvanceAttrLabelBtn:IsChecked() then
		UpdateNormalAttackAttr();
		UpdateNormalDefendAttr();
		Private_ShowEquipAdvanceAttr();
	else
		UpdateSpecialAttackAttr();
		UpdateSpecialDefendAttr();
		UpdateFuZhuAttr();
		Private_ShowEquipYuanSuAttr();
	end

	UpdateSuitAttr();
end

function Private_ShowEquipAdvanceAttr()
	ClearYuanSuAttackAttr();
	ClearYuanSuDefendAttr();
	ClearFuZhuAttr();

	local t_hideIcons = { "RoleAttrBtnSecondBig_YuanSuAttackAttrBackGroundButton", "RoleAttrBtnSecondBig_YuanSuDefendAttrBackGroundButton",	
							"RoleAttrBtnSecondBig_FuZhuAttrBackGroundButton" };
	util.HideIcons( t_hideIcons );

	local t_showIcons = { "RoleAttrBtnSecondBig_FirstChildAttrBackGroundButton", "RoleAttrBtnSecondBig_SecondChildAttrBackGroundButton" };
	util.ShowIcons( t_showIcons );
end

function Private_ShowEquipYuanSuAttr()
	ClearNormalAttackAttr();
	ClearNormalDefendAttr();

	local t_showIcons = { "RoleAttrBtnSecondBig_YuanSuAttackAttrBackGroundButton", "RoleAttrBtnSecondBig_YuanSuDefendAttrBackGroundButton",	
							"RoleAttrBtnSecondBig_FuZhuAttrBackGroundButton" };
	util.ShowIcons( t_showIcons );

	local t_hideIcons = { "RoleAttrBtnSecondBig_FirstChildAttrBackGroundButton", "RoleAttrBtnSecondBig_SecondChildAttrBackGroundButton" };
	util.HideIcons( t_hideIcons );
end

--@desc:���������ť����Ӧ�¼�
--@return: Nothing
function EquipFrame_UpLv_OnClick()
	local lvXinShouBtn	= getglobal( "EquipFrameLvXinShouBtn" );
	
	if lvXinShouBtn:IsShown() then
		if not GameWizard:isDo(GAMEWIZARD_FIRST_SELF_LEVEL_UP) then
			GameWizard:doStep(GAMEWIZARD_FIRST_SELF_LEVEL_UP);
			HideCharacterButtonLevelButton();
		elseif not GameWizard:isDo(GAMEWIZARD_SECOND_SELF_LEVEL_UP) then
			GameWizard:doStep(GAMEWIZARD_SECOND_SELF_LEVEL_UP);
			HideCharacterButtonLevelButton();
		end
	end
	
	lvXinShouBtn:Hide();
	
	local MainPlayer = ActorMgr:getMainPlayer();
	--��ǰ����
	local nCurExp = MainPlayer:getExp();
	--�������辭��
	local levelUpDef = getLevelUP( MainPlayer:getLv() )
	local nLevelNextExp = levelUpDef.LevelNextExp;

	if nCurExp >= nLevelNextExp then
		if MainPlayer:getLv() >= 60 then
			if GeniusMgr:getAllGeniusValue() < levelUpDef.FzMod then
				AddGameMiddleTips( "������Ҫ���츳ֵ�ﵽ" .. levelUpDef.FzMod .. "��" );
				return
			end
		end
		local nLv,nTime = GetFengYinData()
		if nLv > 0 and MainPlayer:getLv() >= nLv then
			local dt = math.floor( (nTime - getServerTime().sec)/3600 )
			local szTime = ""
			if dt < 1 then
				szTime = math.ceil((nTime - getServerTime().sec)/60) .. "����"
			else
				szTime = dt .. "Сʱ"
			end
			AddGameMiddleTips( "�ȼ����ڷ�ӡ�У���������ӡ����" .. szTime )
			return
		end
		MainPlayer:levelUp();
		local mainPlayer = ActorMgr:getMainPlayer();
		EquipFrame_CurExp:SetText(mainPlayer:getExp());
		EquipFrame_NextUpLvExp:SetText(mainPlayer:getLvExp( mainPlayer:getLv() ));
	else
		AddGameMiddleTips( GetMidTipsList()[MID_EQUIP_NOTENOUGHEXP] );
	end
end

function EquipFrameUpLv_OnEnter()
	local szText = ""
	local MainPlayer = ActorMgr:getMainPlayer()
		if MainPlayer:getLv() >= 60 then
			local levelUpDef = getLevelUP( MainPlayer:getLv() )
			if levelUpDef.FzMod > 0 then
				szText = "������Ҫ���츳ֵ��" .. levelUpDef.FzMod .. "\n"
			end
		end
	local szText = szText .. GetFengYinLv()
	if szText ~= "" then
		SetGameTooltips( this:GetName(), szText );
	end
end

--@desc:����Ԥ��ģ������ʱװ��ʾ
--@return: Nothing
function EquipFrame_DisBack_OnClick()
	local bState = this:GetCheckState();
	local MainPlayer	= ActorMgr:getMainPlayer();
	MainPlayer:setShowBack(bState);
end

function EquipFrame_DisFashion_OnClick()
	local bState = this:GetCheckState();
	local MainPlayer	= ActorMgr:getMainPlayer();
	MainPlayer:setShowFashion(bState);
end

--@desc:����Ԥ��ģ������ͷ����ʾ
--@return: Nothing
function EquipFrame_DisHat_OnClick()
	local bState = this:GetCheckState();	
	local MainPlayer	= ActorMgr:getMainPlayer();
	MainPlayer:setShowHead(bState);
end
function EquipFrame_DisFace_OnClick()
	local bState = this:GetCheckState();	
	local MainPlayer	= ActorMgr:getMainPlayer();
	MainPlayer:setShowFace(bState);
end
function EquipFrame_DisHat_OnEnter()
--	SetGameTooltips( this:GetName(), "ͷ��������һͬ��ʾ������ " );
end
--@desc:����Ԥ��ģ������װ��
--@Param: [in]nPosId			ָ����ɫ��װ������
--@Param: [in]bState			ָ��װ����ѡ���״̬
--@return: Nothing
function UpdateModelView()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	local Eqiup = MainPlayer:getEquip();
	local ModelViewer = getglobal("EquipFrame_ModelView");
	local nRace = MainPlayer:getRace();
	local nGender = MainPlayer:getGender();
	Eqiup:setAvatarAttrib( nRace, nGender );
	setAvatarAttrib( POS_ID_HAIR,MainPlayer:getHair() + 1 );
	setAvatarAttrib( POS_ID_FACE,MainPlayer:getFace());
	setAvatarAttrib( POS_ID_HEAD,MainPlayer:getHead());
	if bEquipFrameShowFashion then
		--��ʾʱװ
		local nWearPos = GetFashionSuitPos( EQUIP_POS_FASHION_BODY );
		if not Eqiup:getItem(nWearPos):isValidEnd() then
			setAvatarAttrib( POS_ID_FASHION_BODY,Eqiup:getItem(nWearPos):getItemId() );
		end
		nWearPos = GetFashionSuitPos( EQUIP_POS_FASHION_HEAD );
		if not Eqiup:getItem(nWearPos):isValidEnd() then
			setAvatarAttrib( POS_ID_FASHION_HEAD,Eqiup:getItem(nWearPos):getItemId() );
		end
		nWearPos = GetFashionSuitPos( EQUIP_POS_FASHION_BACK );
		if not Eqiup:getItem(nWearPos):isValidEnd() then
			setAvatarAttrib( POS_ID_FASHION_BACK,Eqiup:getItem(nWearPos):getItemId() );
		end
		nWearPos = GetFashionSuitPos( EQUIP_POS_FASHION_SHOES );
		if not Eqiup:getItem(nWearPos):isValidEnd() then
			setAvatarAttrib( POS_ID_FASHION_SHOES,Eqiup:getItem(nWearPos):getItemId() );
		end
		nWearPos = GetFashionSuitPos( EQUIP_POS_FASHION_HAND );
		if not Eqiup:getItem(nWearPos):isValidEnd() then
			setAvatarAttrib( POS_ID_FASHION_HAND,Eqiup:getItem(nWearPos):getItemId() );
		end
		nWearPos = GetFashionSuitPos( EQUIP_POS_FASHION_LEG );
		if not Eqiup:getItem(nWearPos):isValidEnd() then
			setAvatarAttrib( POS_ID_FASHION_LEG,Eqiup:getItem(nWearPos):getItemId() );
		end
		setAvatarAttrib( POS_ID_FASHION_HAIR,1 );
	else
		-- 4403 ��̲��ͼid
		setAvatarAttrib( POS_ID_FASHION_BODY,0);
		setAvatarAttrib( POS_ID_FASHION_HEAD,0);
		--setAvatarAttrib( POS_ID_FASHION_BACK,0);
		setAvatarAttrib( POS_ID_FASHION_SHOES,0);
		setAvatarAttrib( POS_ID_FASHION_HAND,0);
		setAvatarAttrib( POS_ID_FASHION_LEG,0);
		setAvatarAttrib( POS_ID_FASHION_HAIR,0);
	end
	g_ConfigFashionFair();
	for j=1, CLIENT_MAX_AVATAR_ATTRIB do
		if Eqiup:getAvatarId( j-1 ) ~= 0 then
			if j == 1 then
				ModelViewer:setAvatarAttrib(0, j-1, Eqiup:getAvatarId( j-1 ),Eqiup:getAvatarStarLv( j-1 ), true );
			else
				ModelViewer:setAvatarAttrib(0, j-1, Eqiup:getAvatarId( j-1 ),Eqiup:getAvatarStarLv( j-1 ) );
			end
			--���λû���Ҫ��������ʾ
			if (j-1) == POS_ID_FASHION_BACK and Eqiup:getItem(EQUIP_POS_FASHION_BACK):getUnRealBackID() > 0 then
				ModelViewer:setAvatarAttrib(0, j-1, Eqiup:getItem(EQUIP_POS_FASHION_BACK):getUnRealBackID(),Eqiup:getAvatarStarLv( j-1 ) );
			end
		end
	end
	ModelViewer:setEntityAvatar( 0 );
	ModelViewer:setEntityRotate(0,ModelAngle);	
	
	local leftarmid		= getAvatarAttrib( POS_ID_LEFTHAND );
	local rightarmid	= getAvatarAttrib( POS_ID_RIGHTHAND );	
	
	local t_AvartarControl	= GetAvartarControl();
	local nAnimID = t_AvartarControl:getAvatarAnim( {	["race"] = GetInitRace( nRace ), ["leftHandItem"] = leftarmid, 
														["rightHandItem"] = rightarmid } );
	ModelViewer:playEntityAnim(0, nAnimID, nil );
	--����СЩ
	if nGender == 1 then
		ModelViewer:setPetScale(0,0.8,0.8,0.8);--��������
	else
		ModelViewer:setPetScale(0,0.9,0.9,0.9);--��������
	end
	ModelViewer:setEntityPosition(0,0,20,0);
end

-- ��ȡ��ָװ��λ
function GetEquipRingPos()
	local MainPlayer	= ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end

	local Eqiup			= MainPlayer:getEquip();
	if Eqiup == nil then
		return;
	end

	local bfound = false;
	local nWearPos = 0;
	for i = 11, 12 do
		local ringbtn = getglobal( "EquipFrameEquip"..i );
		local item = Eqiup:getItem( ringbtn:GetClientID() );
		if item:getItemId() == 0 then
			nWearPos = ringbtn:GetClientID();
			bfound = true;
			break;
		end
	end
	if not bfound then
		nWearPos = EquipFrameEquip11:GetClientID();
	end
	return nWearPos;
end

function GetEquipXianlianPos()
	local MainPlayer	= ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end

	local Eqiup			= MainPlayer:getEquip();
	if Eqiup == nil then
		return;
	end

	local bfound = false;
	local nWearPos = 0;
	for i = 13, 14 do
		local ringbtn = getglobal( "EquipFrameEquip"..i );
		local item = Eqiup:getItem( ringbtn:GetClientID() );
		if item:getItemId() == 0 then
			nWearPos = ringbtn:GetClientID();
			bfound = true;
			break;
		end
	end
	if not bfound then
		nWearPos = EquipFrameEquip13:GetClientID();
	end
	return nWearPos;
end

--1�����֣�2��ͷ����3���£�5�����֣�6���棬7���磬12/13����ָ��15���㣬17��������20/21������
local t_perfectEuqipIdx={ 1, 2, 3, 5, 6, 7, 12, 13, 15, 17, 20, 21, }
local MAX_ARM_HIDE_ATTR_RESULT = 15;
local MAX_ARM_PERFECT_ITEM = 12;

local MIN_ARM_STAR_NUM = 3
local MID_ARM_STAR_NUM = 6
local MAX_ARM_STAR_NUM = 9

local szTipsText = "";
local szNextTips = "";

function UpdateEquipFrameWearEquipAttrStarButtonInfo()
	local perfectStarTex = getglobal( "EquipFrameWearEquipAttrStarButtonTex" );
	local MainPlayer = ActorMgr:getMainPlayer();
	local equip = MainPlayer:getEquip();
	local EquipStarNum = 12;
	for i = 1, MAX_ARM_PERFECT_ITEM do
		local item = equip:getItem( t_perfectEuqipIdx[i] );
		if not item:isArmFengYin() then
			local perfectStarNum = item:getArmPefectStarNum();
			if perfectStarNum < EquipStarNum then
				EquipStarNum = perfectStarNum;
			end 
		end
	end
	szTipsText = "#c0096ff			����ǿ����������";
	szNextTips = "#c787878			��һ�׶���װ��"
	local lastInfo = "";
	local bGray = false;
	if EquipStarNum < MIN_ARM_STAR_NUM then
		szTipsText = szTipsText.."\n#c787878	12������3��װ��(δ����)  \n";
		EquipStarNum = 3;
		bGray = true;
		lastInfo = "\n\n	������Ч��";
		perfectStarTex:SetTexUV( 1002, 147, 22, 20 );
		perfectStarTex = getglobal( "SparFrameWearEquipAttrStarButtonTex" );
		perfectStarTex:SetTexUV( 1002, 147, 22, 20 );
	elseif EquipStarNum == MAX_ARM_STAR_NUM then
		szTipsText = szTipsText.."\n#cffff00	12������9��װ��#cff9600(�Ѽ���)\t#cffff00\n";
		lastInfo = "\n\n#cb464ff	�Ѽ�����߽�Ч��";
		perfectStarTex:SetTexUV( 979, 126, 22, 20 );
		perfectStarTex = getglobal( "SparFrameWearEquipAttrStarButtonTex" );
		perfectStarTex:SetTexUV( 979, 126, 22, 20 );
	else
		szTipsText = szTipsText.."\n#cffff00\t 12������" .. EquipStarNum .. "��װ��#cff9600(�Ѽ���)\t#cffff00\n";
		lastInfo = "\n\n#cff9600 ��һ����װ��12������" .. EquipStarNum + 1 .. "��װ��";
		perfectStarTex:SetTexUV( 1002, 126, 22, 20 );	
		perfectStarTex = getglobal( "SparFrameWearEquipAttrStarButtonTex" );
		perfectStarTex:SetTexUV( 1002, 126, 22, 20 );
	end
	local armHideAttrDef = getArmHideAttrDef( MainPlayer:getRace(), EquipStarNum );
	for i = 1, MAX_ARM_HIDE_ATTR_RESULT do
		if armHideAttrDef.Result[i-1].ResultID ~= 0 then
			local resultConfig = GetOneItemResultConfig( { ["resultID"] = armHideAttrDef.Result[i-1].ResultID } )
			if resultConfig ~= nil then
				local val = ( armHideAttrDef.Result[i-1].ResultID == RESULT_POFANG_LEVEL or armHideAttrDef.Result[i-1].ResultID == RESULT_DIKANG_LEVEL ) and math.floor(armHideAttrDef.Result[i-1].ResultVal1 / 10 * math.floor(MainPlayer:getLv() / 10) ^ 2 / 3) or armHideAttrDef.Result[i-1].ResultVal1;
				local desc = ( armHideAttrDef.Result[i-1].ResultID == RESULT_POFANG_LEVEL or armHideAttrDef.Result[i-1].ResultID == RESULT_DIKANG_LEVEL ) and resultConfig[2] .. string.format( "(Lv%d)", math.floor(MainPlayer:getLv() /10)*10 ) or resultConfig[2];
				szTipsText = szTipsText .. "\n		" .. ( ( not bGray and i > 9 ) and "#c5af0ff" or "" ) .. desc .. ":+" ..val .. (IsPercentResultID(armHideAttrDef.Result[i-1].ResultID) and "%" or "")
			end
		end
	end

	if not bGray and EquipStarNum < MAX_ARM_STAR_NUM then
		EquipStarNum = EquipStarNum + 1;
		szNextTips = szNextTips .. "\n\t 12������" .. EquipStarNum .. "��װ��(δ����)  \n";
		armHideAttrDef = getArmHideAttrDef( MainPlayer:getRace(), EquipStarNum );
		for i = 1, MAX_ARM_HIDE_ATTR_RESULT do
		if armHideAttrDef.Result[i-1].ResultID ~= 0 then
				local resultConfig = GetOneItemResultConfig( { ["resultID"] = armHideAttrDef.Result[i-1].ResultID } )
				if resultConfig ~= nil then
					local val = ( armHideAttrDef.Result[i-1].ResultID == RESULT_POFANG_LEVEL or armHideAttrDef.Result[i-1].ResultID == RESULT_DIKANG_LEVEL ) and math.floor(armHideAttrDef.Result[i-1].ResultVal1 / 10 * math.floor(MainPlayer:getLv() / 10) ^ 2 / 3) or armHideAttrDef.Result[i-1].ResultVal1;
					local desc = ( armHideAttrDef.Result[i-1].ResultID == RESULT_POFANG_LEVEL or armHideAttrDef.Result[i-1].ResultID == RESULT_DIKANG_LEVEL ) and resultConfig[2] .. string.format( "(Lv%d)", math.floor(MainPlayer:getLv() /10)*10 ) or resultConfig[2];
					szNextTips = szNextTips .. "\n		" .. desc .. ":+" ..val .. (IsPercentResultID(armHideAttrDef.Result[i-1].ResultID) and "%" or "")
				end
			end
		end
		EquipFrameWearEquipAttrStarButton:SetClientUserData( 0, 1 );
		SparFrameWearEquipAttrStarButton:SetClientUserData( 0, 1 );
	else
		szTipsText = szTipsText..lastInfo;
		EquipFrameWearEquipAttrStarButton:SetClientUserData( 0, 0 );
		SparFrameWearEquipAttrStarButton:SetClientUserData( 0, 0 );
	end
end

function EquipFrameWearEquipAttrStarButton_onEnter()
	if this:GetClientUserData(0) > 0 then
		ShowPerfectStarTip( szTipsText, szNextTips, this:GetName() );
	else
		ShowGameTooltip( { rich = "WideGameTooltipRichText", tipsFrame = "WideGameTooltip", text = szTipsText, frame = this:GetParent(),
							button =  this:GetName() } );
	end
end

function EquipFrameWearEquipAttrStarButton_onLeave()
	WideGameTooltip:Hide();
	PerfectStarTip:Hide();
end

function EquipFrameDaoBaBtn_OnEnter()
	local MainPlayer = ActorMgr:getMainPlayer();
	local clanName = GameClanManager:clanGetClanNameByWid( MainPlayer:getDaoBaClanWid() )
	--local name = MainPlayer:getDaoBaName()
	
	if MainPlayer:getDaoBaEnd() == 0 then
		return;
	end
	local timer = MainPlayer:getDaoBaEnd() - getServerTime().sec;
	if timer <= 0 then
		return ;
	end
	local nHour = math.floor( timer/3600 )
	local nMinute =  math.floor( ( timer - nHour*3600 ) / 60 )
	--local nSecond = math.ceil( timer - nHour*3600 - nMinute*60 )
	local ClockTime = SetClockNum( nHour )..":"..SetClockNum( nMinute )
	
	local szShowInfo = "#c30ffff\t\t\t�˺۽���֤������ʱ�̣�\n\n#cffffbeս���е�˫����Ϊ�ݵ㹫��ĳ�Ա��\n�˺���ս����ʱ����������ʰ������\n\n#cffffbe��#cff0000"..clanName.."#cffffbe����ĳ�Ա����\n���ܻ�õĹ��ṱ�׶Ƚ��ܵ�����\n\n#c00ff00����ʱ��:"..ClockTime.."\n\n#cff7404��ɱ�Է������Ա������ǰ����"
	SetGameTooltips( this:GetName(),  szShowInfo)
end

function RoleAttrGenuis_OnEnter()
	if RoleAttrGenuisFont:GetText() == "" then
		return;
	end

	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then return end;

	local geniusCareerDef = GeniusMgr:getGeniusCareerDef( mainplayer:getRace() )
	local minLevel = getMaxTianFuLv();
	if geniusCareerDef ~= nil then
		for i=1,MAX_GENIUS_SUBTYPE_TOLUA do
			local attrId = geniusCareerDef.Types[0].AttrIDs[i-1]
			if attrId > 0 then
				local lv = GeniusMgr:getAttrLv(0,attrId);
				minLevel = minLevel > lv and lv or minLevel;
			end
		end
	end

	UpdateTianFuLockTips( 1, minLevel, this:GetName(), mainplayer );
	TianFuLockTipsFrame:Show();
end

function ShowPerfectStarTip( szDesc, szNextDesc, szBtnName )
	local frame = getglobal( "PerfectStarTip" );
	local rich = getglobal( "PerfectStarTipRichText" );
	local nextRich = getglobal( "PerfectStarTipRichNextText" );
	local btn = getglobal( "PerfectStarTipMove" );
	rich:SetText( szDesc, 255, 255, 190 );
	nextRich:SetText( szNextDesc, 255, 255, 190 );

	local nWidth = 1;
	for i = 1, nextRich:GetTextLines() do
		local nLineWidth = nextRich:getLineWidth( i - 1 );
		if nWidth < nLineWidth then
			nWidth = nLineWidth;
		end
	end
	nWidth = nWidth + 6;
	local nHeight = nextRich:GetTotalHeight();
	nHeight = ( nHeight == 0 ) and 1 or nHeight;

	nextRich:SetPoint( "topleft", "PerfectStarTipRichText", "topleft", nWidth, 0 );
	frame:resizeRect( nWidth*2 + 4, nHeight + 8 );
	frame:SetSize( math.ceil( (frame:GetRealWidth()+ 2) / GetScreenScaleY() ),  math.ceil( (frame:GetRealHeight() + 2 ) / GetScreenScaleY() ) );
	btn:SetPoint( "topleft", "PerfectStarTipRichText", "topleft", nWidth - 8, nHeight/2 - 12 );

	--��������λ��
	if szBtnName ~= nil then
		local nWidth    = PerfectStarTip:GetWidth();
	
		
		local nHeight   = PerfectStarTip:GetHeight();

		local curSelectBtn = getglobal(szBtnName);
		local nRealTop  = curSelectBtn:GetRealTop();
		local nRight    = curSelectBtn:GetRight();

		if ( nRealTop - nHeight ) > 0 then
			if ( nRight + nWidth < GetScreenWidth() ) then
				PerfectStarTip:SetPoint("BottomLeft",szBtnName,"TopRight",0,-10);
			else
				PerfectStarTip:SetPoint("BottomRight",szBtnName,"TopLeft",0,-10);
			end
		else
			if ( nRight + nWidth < GetScreenWidth() ) then
				PerfectStarTip:SetPoint("TopLeft",szBtnName,"BottomRight",0,10);
			else
				PerfectStarTip:SetPoint("TopRight",szBtnName,"BottomLeft",0,10);
			end
		end
	end

	PerfectStarTip:Show();
end
-- ���ʱװλ
function GetFashionSuitPos( nAmmPos )
	local nWearPos 	 = 0;
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return nWearPos;
	end

	local Eqiup = MainPlayer:getEquip();
	if Eqiup == nil then
		return nWearPos;
	end
	local nSelectSuit = Eqiup:getSelectFashionIndex();
	if nAmmPos == EQUIP_POS_FASHION_BODY then 
		if nSelectSuit == 0 then 
			nWearPos = EQUIP_POS_FASHION_BODY;
		elseif nSelectSuit == 1 then 
			nWearPos = EQUIP_POS_FASHION_BODY2 ;
		else 
			nWearPos = EQUIP_POS_FASHION_BODY3 ;
		end
	elseif nAmmPos >= EQUIP_POS_FASHION_HEAD then
		nWearPos = nAmmPos + nSelectSuit * 7; 
	else 
		nWearPos = nAmmPos;
	end
	return nWearPos;
end

function EquipFrame_FashionBag_OnClick()
	if this:IsChecked() then 
		return;
	end
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	local Eqiup = MainPlayer:getEquip();
	local nFashionNum = Eqiup:getSelectFashionNum();
	if this:GetClientID() > nFashionNum + 1 then 
		MessageBoxForVip( "��ʾ", "����ʱװ��λ��Ҫ����һ��ʱװ��λ��չ���ߣ�ȷ����Ҫ��������λ��" )
		MessageBoxFrameForVipAcceptButton:SetText("ȷ������");
		MessageBoxFrameForVip:SetClientUserData( 0, this:GetClientID() );
		MessageBoxFrameForVip:SetClientUserData( 1, 1 );
		return;
	end
	DisCheckAllBtn( "EquipFrame_FashionBag", MAX_FASHION_BAG_NUM );
	
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	local Eqiup = MainPlayer:getEquip();
	Eqiup:send_SelectFashionSuit( this:GetClientID() - 1 );
--	SetEquipInfo();
	this:Checked();
end
function EquipFrame_FashionBag_OnEnter()
	local flag = this:GetClientUserData( 0 );
	
	local szText = ""
	if this:GetClientID() == 1 then 
		szText = "��һ��\n���滻����ʱװ\n"
	elseif this:GetClientID() == 2 then 
		szText = "�ڶ���\n���滻����ʱװ\n"
	else 
		szText = "������\n���滻����ʱװ\n"
	end
	if flag == 1 then 
		szText = szText.."����λ��δ����\n��Ҫ��ʱװ��λ��չ�����߿���"
	elseif flag == 2 then 
		szText = szText.."����ʱװ����"
	elseif flag == 3 then
		szText = szText.."���κ�ʱװ"
	end
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
					button = this:GetName(), horizonal = "right" } );
end