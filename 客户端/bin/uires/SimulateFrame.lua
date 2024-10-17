--1为ushort 2为short
local Simulateinfo={
["攻击属性"]=
{
	[1]={ID=ATTR_ID_PHSIC_ATK,		desc="物理攻击",		type=1},
	[2]={ID=ATTR_ID_ICE_ATK,		desc="魔法攻击",		type=1},
	[3]={ID=ATTR_ID_BASE_ATTR_HEAVY,	desc="暴击力",			type=2},
	[4]={ID=ATTR_ID_HEAVY,			desc="暴击率（加成）",		type=1},
	[5]={ID=ATTR_ID_HEAVY_VAL,		desc="暴击伤害加成",		type=2},
	[6]={ID=ATTR_ID_HIT,			desc="命中力",			type=1},
	[7]={ID=ATTR_ID_PHSIC_HURT,		desc="物理伤害加成",		type=1},
	[8]={ID=ATTR_ID_ICE_HURT,		desc="魔法伤害加成",		type=1},
	nums=8,
},
["防御属性"]=
{
	[1]={ID=ATTR_ID_PHSIC_DEF,		desc="物理防御",		type=1},
	[2]={ID=ATTR_ID_ICE_DEF,		desc="魔法防御",		type=1},
	[3]={ID=ATTR_ID_BASE_ATTR_DEFHEAVY,	desc="暴击抵抗",		type=2},
	[4]={ID=ATTR_ID_FLEE,			desc="闪避",			type=1},
	[5]={ID=ATTR_ID_DEF_PHSIC_HURT,		desc="物理伤害抵抗",		type=1},
	[6]={ID=ATTR_ID_DEF_ICE_HURT,		desc="魔法伤害抵抗",		type=1},
	nums=6,
},
["元素攻击力"]=
{
	[1]={ID=ATTR_ID_FIRE_ATK,		desc="风攻击",			type=1},	
	[2]={ID=ATTR_ID_WATER_ATK,		desc="土攻击",			type=1},
	[3]={ID=ATTR_ID_THUNDER_ATK,		desc="雷攻击",			type=1},
	[4]={ID=ATTR_ID_LIGHT_ATK,		desc="光攻击",			type=1},
	[5]={ID=ATTR_ID_NIGHT_ATK,		desc="暗攻击",			type=1},
	[6]={ID=ATTR_ID_HIT_RATE,		desc="命中率",			type=1},
	nums=6,
},
["元素防御力"]=
{
	[1]={ID=ATTR_ID_FIRE_DEF,		desc="风防御",			type=1},
	[2]={ID=ATTR_ID_WATER_DEF,		desc="土防御",			type=1},
	[3]={ID=ATTR_ID_THUNDER_DEF,		desc="雷防御",			type=1},	
	[4]={ID=ATTR_ID_LIGHT_DEF,		desc="光防御",			type=1},
	[5]={ID=ATTR_ID_NIGHT_DEF,		desc="暗防御",			type=1},
	nums=5,
},
};
local AttackID=SimArmourID;
local nSimulatecounts=1;
function SimulateFrame_OnLoad()
	this:RegisterEvent("GE_SIMULATE_ARMOUR");
	this:RegisterEvent("GE_SIMULATE_SECOND");
	this:RegisterEvent("GE_SIMULATE_SHOW");
	this:RegisterEvent("GE_SIMULATE_ERROR");
end

function SimulateFrame_OnEvent()
	if  arg1 == "GE_SIMULATE_ARMOUR" then
		if SimulateFrame_ArmourAdvanceAttrLabelBtn:IsChecked() then
				UpdateSimulateFrameAttrLabelBtn(1,1,SimArmourID);
		else
				UpdateSimulateFrameAttrLabelBtn(1,2,SimArmourID);
		end
		if AttackID == SimArmourID then
			SimulateFrameEditAttackmodulus:SetText(SimulateMgr:getAtkLevelMul(SimArmourID));
			SimulateFrameEditRecoverymodulus:SetText(SimulateMgr:getDefLevelMul(SimArmourID));
		end
	elseif arg1 == "GE_SIMULATE_SECOND" then
		if SimulateFrame_SecondAdvanceAttrLabelBtn:IsChecked() then
				UpdateSimulateFrameAttrLabelBtn(2,1,SimSecondID);
		else
				UpdateSimulateFrameAttrLabelBtn(2,2,SimSecondID);
		end
		if AttackID == SimSecondID then
			SimulateFrameEditAttackmodulus:SetText(SimulateMgr:getAtkLevelMul(SimSecondID));
			SimulateFrameEditRecoverymodulus:SetText(SimulateMgr:getDefLevelMul(SimSecondID));
		end
	elseif arg1 == "GE_SIMULATE_SHOW" then
		SimulateFrame_OnShowResult();
	elseif arg1 == "GE_SIMULATE_ERROR" then
		ShowMidTips("服务器未返回上次计算结果，请返回后再发送!");
	end
end

function SimulateFrame_OnShowResult()
	local rich=getglobal("SimulateFrameresultdescribeRichText");
	local result=SimulateMgr:getResultshow();
	rich:AddText("第#cFF7D00"..nSimulatecounts.."#n次测试，攻击次数".."#cFF7D00"..result.nAtkCounts.."#n次\n",255,255,190);
	rich:AddText("命中".."#cFF7D00"..result.nShootCounts.."#n，闪避#cFF7D00"..result.nducks.."#n次\n",255,255,190);
	rich:AddText("共造成伤害".."#cFF7D00"..result.nTotalhurts.."#n，平均伤害#cFF7D00"..result.nAveragehurts.."#n\n",255,255,190);
	rich:AddText("暴击次数".."#cFF7D00"..result.nDoubleHits.."#n，暴击最大伤害#cFF7D00"..result.nDoubleMaxHurts.."#n\n",255,255,190);
	rich:AddText("暴击最小伤害#cFF7D00"..result.nDoubleMinHurts.."#n,暴击平均伤害#cFF7D00"..result.nDoubleAvrHurts.."#n\n",255,255,190);
	rich:AddText("普通攻击次数#cFF7D00"..result.nAtkCounts-result.nDoubleHits.."#n,普通伤害最大伤害".."#cFF7D00"..result.nNormalMaxHurts.."#n\n",255,255,190);
	rich:AddText("普通伤害最小伤害#cFF7D00"..result.nNormalMinHurts.."#n,普通伤害平均伤害#cFF7D00"..result.nNormalAvrHurts.."#n\n",255,255,190);
	rich:AddText("==============================================================",255,255,190);
	rich:ScrollEnd();
	nSimulatecounts=nSimulatecounts+1;
end

function SimulateFrameRichDownBtn_OnClick()
	local rich=getglobal("SimulateFrameresultdescribeRichText");
	--for i=1,5 do
	rich:ScrollDown();
	--end
end

function SimulateFrameRichUpBtn_OnClick()
	local rich=getglobal("SimulateFrameresultdescribeRichText");
	--for i=1,5 do
	rich:ScrollUp();
	--end
end

function SimulateFrame_OnShow()
	Armour_AdvanceAttrLabelBtn_OnClick();
	Second_AdvanceAttrLabelBtn_OnClick();
	local mainplayer = ActorMgr:getMainPlayer();
	local str = mainplayer:GetID().."";
	SimulateMgr:setArmourId(str);
	SimulateFrameEditArmourID:SetText(str);
	SimulateMgr:requestWatchInfo(str);
end

function SimulateFrameDownBtn_OnClick()
	if SimulateFrameListSelect:IsShown() then
		SimulateFrameListSelect:Hide();
	else
		SimulateFrameListSelect:Show();
	end
end

function SimulateFrameArmour_clear()
	for i=1,8 do
		local font=getglobal("Armour_AttackAttrBtn"..i.."NameFont");
		font:SetText("");
		font=getglobal("Armour_AttackAttrBtn"..i.."_NickNameEdit");
		font:SetText("");
	end
	for i=1,6 do
		local font=getglobal("Armour_DefendAttrBtn"..i.."NameFont");
		font:SetText("");
		font=getglobal("Armour_DefendAttrBtn"..i.."_NickNameEdit");
		font:SetText("");
	end
end

function SimulateFrameSecond_clear()
	for i=1,8 do
		local font=getglobal("Second_AttackAttrBtn"..i.."NameFont");
		font:SetText("");
		font=getglobal("Second_AttackAttrBtn"..i.."_NickNameEdit");
		font:SetText("");
	end
	for i=1,6 do
		local font=getglobal("Second_DefendAttrBtn"..i.."NameFont");
		font:SetText("");
		font=getglobal("Second_DefendAttrBtn"..i.."_NickNameEdit");
		font:SetText("");
	end
end

function UpdateSimulateFrameAttrLabelBtn(nselectid1,nselectid2,SimulaterId)
	--nselectid1标示是甲方还是乙方1为甲2为乙，nselectid2标示要更新的是特殊属性还是高级属性
	local str1,str2,sstr1,sstr2;
	if nselectid1 == 1 then
		str1 = "Armour_AttackAttrBtn";
		str2 = "Armour_DefendAttrBtn";
	 else
		str1 = "Second_AttackAttrBtn";
		str2 = "Second_DefendAttrBtn";
	 end
	 if  nselectid2 == 1 then
		sstr1 = "攻击属性";
		sstr2 = "防御属性";
	 else
		sstr1 = "元素攻击力";
		sstr2 = "元素防御力";
	end
	for i=1,Simulateinfo[sstr1]["nums"] do
		local font=getglobal(str1..i.."NameFont");
		font:SetText(Simulateinfo[sstr1][i]["desc"]);
		font=getglobal(str1..i.."_NickNameEdit");
		if Simulateinfo[sstr1][i]["type"] == 1 then
			font:SetText(SimulateMgr:getshortValue(SimulaterId,Simulateinfo[sstr1][i]["ID"]));
		else
			font:SetText(SimulateMgr:getunshortValue(SimulaterId,Simulateinfo[sstr1][i]["ID"]));
		end
	end
	for i=1,Simulateinfo[sstr2]["nums"] do
		local font=getglobal(str2..i.."NameFont");
		font:SetText(Simulateinfo[sstr2][i]["desc"]);
		font=getglobal(str2..i.."_NickNameEdit");
		if Simulateinfo[sstr2][i]["type"] == 1 then
			font:SetText(SimulateMgr:getshortValue(SimulaterId,Simulateinfo[sstr2][i]["ID"]));
		else
			font:SetText(SimulateMgr:getunshortValue(SimulaterId,Simulateinfo[sstr2][i]["ID"]));
		end
	end
end

function SimulateFrameArmour_getAttr(nselectid1,nselectid2)
	--获取的时候因为
	local str1,str2,sstr1,sstr2,SimulaterId;
	if nselectid1 == 1 then
		str1 = "Armour_AttackAttrBtn";
		str2 = "Armour_DefendAttrBtn";
		SimulaterId = SimArmourID;
	 else
		str1 = "Second_AttackAttrBtn";
		str2 = "Second_DefendAttrBtn";
		SimulaterId = SimSecondID;
	 end
	 if  nselectid2 == 1 then
		sstr1 = "攻击属性";
		sstr2 = "防御属性";
	 else
		sstr1 = "元素攻击力";
		sstr2 = "元素防御力";
	end
	for i=1,Simulateinfo[sstr1]["nums"] do
		local edit=getglobal(str1..i.."_NickNameEdit");
		SimulateMgr:setAttr(SimulaterId,Simulateinfo[sstr1][i]["ID"],edit:GetText());
	end
	for i=1,Simulateinfo[sstr2]["nums"] do
		local edit=getglobal(str2..i.."_NickNameEdit");
		SimulateMgr:setAttr(SimulaterId,Simulateinfo[sstr2][i]["ID"],edit:GetText());
	end
end

function Armour_AdvanceAttrLabelBtn_OnClick()
	if not SimulateFrame_ArmourAdvanceAttrLabelBtn:IsChecked() then
		SimulateFrameArmour_getAttr(1,2);
		SimulateFrameArmour_clear();
		SimulateFrame_ArmourAdvanceAttrLabelBtn:Checked();
		SimulateFrame_ArmourSpecialAttrLabelBtn:DisChecked();
		UpdateSimulateFrameAttrLabelBtn(1,1,SimArmourID);
		SimulateFrameArmourBig_FirstChildAttrBackGroundButtonFont1:SetText("攻");
		SimulateFrameArmourBig_FirstChildAttrBackGroundButtonFont2:SetText("击");
		SimulateFrameArmourBig_FirstChildAttrBackGroundButtonFont3:SetText("属");
		SimulateFrameArmourBig_FirstChildAttrBackGroundButtonFont4:SetText("性");
		SimulateFrameArmourBig_FirstChildAttrBackGroundButtonFont5:SetText("");
		SimulateFrameArmourBig_SecondChildAttrBackGroundButtonFont1:SetText("防");
		SimulateFrameArmourBig_SecondChildAttrBackGroundButtonFont2:SetText("御");
		SimulateFrameArmourBig_SecondChildAttrBackGroundButtonFont3:SetText("属");
		SimulateFrameArmourBig_SecondChildAttrBackGroundButtonFont4:SetText("性");
		SimulateFrameArmourBig_SecondChildAttrBackGroundButtonFont5:SetText("");
	end
end

function Armour_SpecialAttrLabelBtn_OnClick()
	if not SimulateFrame_ArmourSpecialAttrLabelBtn:IsChecked() then
		SimulateFrameArmour_getAttr(1,1);
		SimulateFrameArmour_clear();
		SimulateFrame_ArmourAdvanceAttrLabelBtn:DisChecked();
		SimulateFrame_ArmourSpecialAttrLabelBtn:Checked();
		UpdateSimulateFrameAttrLabelBtn(1,2,SimArmourID);
		SimulateFrameArmourBig_FirstChildAttrBackGroundButtonFont1:SetText("元");
		SimulateFrameArmourBig_FirstChildAttrBackGroundButtonFont2:SetText("素");
		SimulateFrameArmourBig_FirstChildAttrBackGroundButtonFont3:SetText("攻");
		SimulateFrameArmourBig_FirstChildAttrBackGroundButtonFont4:SetText("击");
		SimulateFrameArmourBig_FirstChildAttrBackGroundButtonFont5:SetText("力");
		SimulateFrameArmourBig_SecondChildAttrBackGroundButtonFont1:SetText("元");
		SimulateFrameArmourBig_SecondChildAttrBackGroundButtonFont2:SetText("素");
		SimulateFrameArmourBig_SecondChildAttrBackGroundButtonFont3:SetText("防");
		SimulateFrameArmourBig_SecondChildAttrBackGroundButtonFont4:SetText("御");
		SimulateFrameArmourBig_SecondChildAttrBackGroundButtonFont5:SetText("力");
	end
end

function Second_AdvanceAttrLabelBtn_OnClick()
	if not SimulateFrame_SecondAdvanceAttrLabelBtn:IsChecked() then
		SimulateFrameArmour_getAttr(2,2);
		SimulateFrameSecond_clear();
		SimulateFrame_SecondAdvanceAttrLabelBtn:Checked();
		SimulateFrame_SecondSpecialAttrLabelBtn:DisChecked();
		UpdateSimulateFrameAttrLabelBtn(2,1,SimSecondID);
		SimulateFrameSecondBig_FirstChildAttrBackGroundButtonFont1:SetText("攻");
		SimulateFrameSecondBig_FirstChildAttrBackGroundButtonFont2:SetText("击");
		SimulateFrameSecondBig_FirstChildAttrBackGroundButtonFont3:SetText("属");
		SimulateFrameSecondBig_FirstChildAttrBackGroundButtonFont4:SetText("性");
		SimulateFrameSecondBig_FirstChildAttrBackGroundButtonFont5:SetText("");
		SimulateFrameSecondBig_SecondChildAttrBackGroundButtonFont1:SetText("防");
		SimulateFrameSecondBig_SecondChildAttrBackGroundButtonFont2:SetText("御");
		SimulateFrameSecondBig_SecondChildAttrBackGroundButtonFont3:SetText("属");
		SimulateFrameSecondBig_SecondChildAttrBackGroundButtonFont4:SetText("性");
		SimulateFrameSecondBig_SecondChildAttrBackGroundButtonFont5:SetText("");
	end
end

function Second_SpecialAttrLabelBtn_OnClick()
	if not SimulateFrame_SecondSpecialAttrLabelBtn:IsChecked() then
		SimulateFrameArmour_getAttr(2,1);
		SimulateFrameSecond_clear();
		SimulateFrame_SecondAdvanceAttrLabelBtn:DisChecked();
		SimulateFrame_SecondSpecialAttrLabelBtn:Checked();
		UpdateSimulateFrameAttrLabelBtn(2,2,SimSecondID);
		SimulateFrameSecondBig_FirstChildAttrBackGroundButtonFont1:SetText("元");
		SimulateFrameSecondBig_FirstChildAttrBackGroundButtonFont2:SetText("素");
		SimulateFrameSecondBig_FirstChildAttrBackGroundButtonFont3:SetText("攻");
		SimulateFrameSecondBig_FirstChildAttrBackGroundButtonFont4:SetText("击");
		SimulateFrameSecondBig_FirstChildAttrBackGroundButtonFont5:SetText("力");
		SimulateFrameSecondBig_SecondChildAttrBackGroundButtonFont1:SetText("元");
		SimulateFrameSecondBig_SecondChildAttrBackGroundButtonFont2:SetText("素");
		SimulateFrameSecondBig_SecondChildAttrBackGroundButtonFont3:SetText("防");
		SimulateFrameSecondBig_SecondChildAttrBackGroundButtonFont4:SetText("御");
		SimulateFrameSecondBig_SecondChildAttrBackGroundButtonFont5:SetText("力");
	end
end

function ArmourReadBtn_OnClick()
	if SimulateFrameEditArmourID:GetText() == "" then
		local mainplayer = ActorMgr:getMainPlayer();
		local str = mainplayer:GetSelectTarget().."";
		SimulateFrameEditArmourID:SetText(str);
	end
	SimulateMgr:setArmourId(SimulateFrameEditArmourID:GetText());
	SimulateMgr:requestWatchInfo(SimulateFrameEditArmourID:GetText());
end

function SecondReadBtn_OnClick()
	if SimulateFrameEditSecondID:GetText() == "" then

		local mainplayer = ActorMgr:getMainPlayer();
		local str = mainplayer:GetSelectTarget().."";
		SimulateFrameEditSecondID:SetText(str);
	end
	SimulateMgr:setSecondId(SimulateFrameEditSecondID:GetText());
	SimulateMgr:requestWatchInfo(SimulateFrameEditSecondID:GetText());
end

function SimulateFrameListSelect1btn_OnClick()
	AttackID=SimArmourID;
	SimulateFrameSelectbtnFont:SetText("甲");
	SimulateFrameListSelect:Hide();
	SimulateFrameEditAttackmodulus:SetText(SimulateMgr:getAtkLevelMul(SimArmourID));
	SimulateFrameEditRecoverymodulus:SetText(SimulateMgr:getDefLevelMul(SimArmourID));
end

function SimulateFrameListSelect2btn_OnClick()
	AttackID=SimSecondID;
	SimulateFrameSelectbtnFont:SetText("乙");
	SimulateFrameListSelect:Hide();
	SimulateFrameEditAttackmodulus:SetText(SimulateMgr:getAtkLevelMul(SimSecondID));
	SimulateFrameEditRecoverymodulus:SetText(SimulateMgr:getDefLevelMul(SimSecondID));
end

function	SimulateFrameStartBtn_OnClick()
	if SimulateFrameCanSend() then
		if SimulateFrame_ArmourAdvanceAttrLabelBtn:IsChecked() then
			SimulateFrameArmour_getAttr(1,1);
		else
			SimulateFrameArmour_getAttr(1,2);
		end
		if SimulateFrame_SecondAdvanceAttrLabelBtn:IsChecked() then
			SimulateFrameArmour_getAttr(2,1);
		else
			SimulateFrameArmour_getAttr(2,2);
		end
		SimulateMgr:setAtkerId(AttackID);
		SimulateMgr:setSkillId(SimulateFrameEditSkillID:GetText());
		SimulateMgr:setSkillLevel(SimulateFrameEditLevel:GetText());
		SimulateMgr:setAtkCounts(SimulateFrameEditCounts:GetText());
		SimulateMgr:setAtkLevelMul(AttackID,SimulateFrameEditAttackmodulus:GetText());
		SimulateMgr:setDefLevelMul(AttackID,SimulateFrameEditRecoverymodulus:GetText());
		SimulateMgr:requestSimulatefight();
	else
		ShowMidTips("有选项未填!");
	end
end

function SimulateFrameEdibox_OnTap()
	if this:GetName() == "SimulateFrameEditArmourID" then
		SetFocusFrame("SimulateFrameEditSecondID");
	elseif this:GetName() == "SimulateFrameEditSecondID" then
		SetFocusFrame("SimulateFrameEditAttackmodulus");
	elseif this:GetName() == "SimulateFrameEditAttackmodulus" then
		SetFocusFrame("SimulateFrameEditRecoverymodulus");
	elseif this:GetName() == "SimulateFrameEditRecoverymodulus" then
		SetFocusFrame("SimulateFrameEditSkillID");
	elseif this:GetName() == "SimulateFrameEditSkillID" then
		SetFocusFrame("SimulateFrameEditLevel");
	elseif this:GetName() == "SimulateFrameEditLevel" then
		SetFocusFrame("SimulateFrameEditCounts");
	end
end

function SimulateFrameReset()
	SimulateMgr:Reset();
	SimulateFrameArmour_clear();
	SimulateFrameSecond_clear();
	SimulateFrameEditArmourID:Clear();
	SimulateFrameEditSecondID:Clear();
	SimulateFrameEditAttackmodulus:Clear();
	SimulateFrameEditRecoverymodulus:Clear();
	SimulateFrameEditSkillID:Clear();
	SimulateFrameEditLevel:Clear();
	SimulateFrameEditCounts:Clear();
	nSimulatecounts=1;
	AttackID=SimArmourID;
	SimulateFrameSelectbtnFont:SetText("甲");
	SimulateFrameresultdescribeRichText:Clear();
end

function SimulateFrameCanSend()
	if SimulateFrameEditArmourID:GetText() == "" or SimulateFrameEditSecondID:GetText() == "" or 
	   SimulateFrameEditAttackmodulus:GetText() == "" or SimulateFrameEditRecoverymodulus:GetText() == "" or
	  SimulateFrameEditSkillID:GetText() == "" or  SimulateFrameEditLevel:GetText() == "" or
	   SimulateFrameEditLevel:GetText() == ""	then
	  return false;
	  end
	return true;
end