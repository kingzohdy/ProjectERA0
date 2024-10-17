--1Ϊushort 2Ϊshort
local Simulateinfo={
["��������"]=
{
	[1]={ID=ATTR_ID_PHSIC_ATK,		desc="������",		type=1},
	[2]={ID=ATTR_ID_ICE_ATK,		desc="ħ������",		type=1},
	[3]={ID=ATTR_ID_BASE_ATTR_HEAVY,	desc="������",			type=2},
	[4]={ID=ATTR_ID_HEAVY,			desc="�����ʣ��ӳɣ�",		type=1},
	[5]={ID=ATTR_ID_HEAVY_VAL,		desc="�����˺��ӳ�",		type=2},
	[6]={ID=ATTR_ID_HIT,			desc="������",			type=1},
	[7]={ID=ATTR_ID_PHSIC_HURT,		desc="�����˺��ӳ�",		type=1},
	[8]={ID=ATTR_ID_ICE_HURT,		desc="ħ���˺��ӳ�",		type=1},
	nums=8,
},
["��������"]=
{
	[1]={ID=ATTR_ID_PHSIC_DEF,		desc="�������",		type=1},
	[2]={ID=ATTR_ID_ICE_DEF,		desc="ħ������",		type=1},
	[3]={ID=ATTR_ID_BASE_ATTR_DEFHEAVY,	desc="�����ֿ�",		type=2},
	[4]={ID=ATTR_ID_FLEE,			desc="����",			type=1},
	[5]={ID=ATTR_ID_DEF_PHSIC_HURT,		desc="�����˺��ֿ�",		type=1},
	[6]={ID=ATTR_ID_DEF_ICE_HURT,		desc="ħ���˺��ֿ�",		type=1},
	nums=6,
},
["Ԫ�ع�����"]=
{
	[1]={ID=ATTR_ID_FIRE_ATK,		desc="�繥��",			type=1},	
	[2]={ID=ATTR_ID_WATER_ATK,		desc="������",			type=1},
	[3]={ID=ATTR_ID_THUNDER_ATK,		desc="�׹���",			type=1},
	[4]={ID=ATTR_ID_LIGHT_ATK,		desc="�⹥��",			type=1},
	[5]={ID=ATTR_ID_NIGHT_ATK,		desc="������",			type=1},
	[6]={ID=ATTR_ID_HIT_RATE,		desc="������",			type=1},
	nums=6,
},
["Ԫ�ط�����"]=
{
	[1]={ID=ATTR_ID_FIRE_DEF,		desc="�����",			type=1},
	[2]={ID=ATTR_ID_WATER_DEF,		desc="������",			type=1},
	[3]={ID=ATTR_ID_THUNDER_DEF,		desc="�׷���",			type=1},	
	[4]={ID=ATTR_ID_LIGHT_DEF,		desc="�����",			type=1},
	[5]={ID=ATTR_ID_NIGHT_DEF,		desc="������",			type=1},
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
		ShowMidTips("������δ�����ϴμ��������뷵�غ��ٷ���!");
	end
end

function SimulateFrame_OnShowResult()
	local rich=getglobal("SimulateFrameresultdescribeRichText");
	local result=SimulateMgr:getResultshow();
	rich:AddText("��#cFF7D00"..nSimulatecounts.."#n�β��ԣ���������".."#cFF7D00"..result.nAtkCounts.."#n��\n",255,255,190);
	rich:AddText("����".."#cFF7D00"..result.nShootCounts.."#n������#cFF7D00"..result.nducks.."#n��\n",255,255,190);
	rich:AddText("������˺�".."#cFF7D00"..result.nTotalhurts.."#n��ƽ���˺�#cFF7D00"..result.nAveragehurts.."#n\n",255,255,190);
	rich:AddText("��������".."#cFF7D00"..result.nDoubleHits.."#n����������˺�#cFF7D00"..result.nDoubleMaxHurts.."#n\n",255,255,190);
	rich:AddText("������С�˺�#cFF7D00"..result.nDoubleMinHurts.."#n,����ƽ���˺�#cFF7D00"..result.nDoubleAvrHurts.."#n\n",255,255,190);
	rich:AddText("��ͨ��������#cFF7D00"..result.nAtkCounts-result.nDoubleHits.."#n,��ͨ�˺�����˺�".."#cFF7D00"..result.nNormalMaxHurts.."#n\n",255,255,190);
	rich:AddText("��ͨ�˺���С�˺�#cFF7D00"..result.nNormalMinHurts.."#n,��ͨ�˺�ƽ���˺�#cFF7D00"..result.nNormalAvrHurts.."#n\n",255,255,190);
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
	--nselectid1��ʾ�Ǽ׷������ҷ�1Ϊ��2Ϊ�ң�nselectid2��ʾҪ���µ����������Ի��Ǹ߼�����
	local str1,str2,sstr1,sstr2;
	if nselectid1 == 1 then
		str1 = "Armour_AttackAttrBtn";
		str2 = "Armour_DefendAttrBtn";
	 else
		str1 = "Second_AttackAttrBtn";
		str2 = "Second_DefendAttrBtn";
	 end
	 if  nselectid2 == 1 then
		sstr1 = "��������";
		sstr2 = "��������";
	 else
		sstr1 = "Ԫ�ع�����";
		sstr2 = "Ԫ�ط�����";
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
	--��ȡ��ʱ����Ϊ
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
		sstr1 = "��������";
		sstr2 = "��������";
	 else
		sstr1 = "Ԫ�ع�����";
		sstr2 = "Ԫ�ط�����";
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
		SimulateFrameArmourBig_FirstChildAttrBackGroundButtonFont1:SetText("��");
		SimulateFrameArmourBig_FirstChildAttrBackGroundButtonFont2:SetText("��");
		SimulateFrameArmourBig_FirstChildAttrBackGroundButtonFont3:SetText("��");
		SimulateFrameArmourBig_FirstChildAttrBackGroundButtonFont4:SetText("��");
		SimulateFrameArmourBig_FirstChildAttrBackGroundButtonFont5:SetText("");
		SimulateFrameArmourBig_SecondChildAttrBackGroundButtonFont1:SetText("��");
		SimulateFrameArmourBig_SecondChildAttrBackGroundButtonFont2:SetText("��");
		SimulateFrameArmourBig_SecondChildAttrBackGroundButtonFont3:SetText("��");
		SimulateFrameArmourBig_SecondChildAttrBackGroundButtonFont4:SetText("��");
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
		SimulateFrameArmourBig_FirstChildAttrBackGroundButtonFont1:SetText("Ԫ");
		SimulateFrameArmourBig_FirstChildAttrBackGroundButtonFont2:SetText("��");
		SimulateFrameArmourBig_FirstChildAttrBackGroundButtonFont3:SetText("��");
		SimulateFrameArmourBig_FirstChildAttrBackGroundButtonFont4:SetText("��");
		SimulateFrameArmourBig_FirstChildAttrBackGroundButtonFont5:SetText("��");
		SimulateFrameArmourBig_SecondChildAttrBackGroundButtonFont1:SetText("Ԫ");
		SimulateFrameArmourBig_SecondChildAttrBackGroundButtonFont2:SetText("��");
		SimulateFrameArmourBig_SecondChildAttrBackGroundButtonFont3:SetText("��");
		SimulateFrameArmourBig_SecondChildAttrBackGroundButtonFont4:SetText("��");
		SimulateFrameArmourBig_SecondChildAttrBackGroundButtonFont5:SetText("��");
	end
end

function Second_AdvanceAttrLabelBtn_OnClick()
	if not SimulateFrame_SecondAdvanceAttrLabelBtn:IsChecked() then
		SimulateFrameArmour_getAttr(2,2);
		SimulateFrameSecond_clear();
		SimulateFrame_SecondAdvanceAttrLabelBtn:Checked();
		SimulateFrame_SecondSpecialAttrLabelBtn:DisChecked();
		UpdateSimulateFrameAttrLabelBtn(2,1,SimSecondID);
		SimulateFrameSecondBig_FirstChildAttrBackGroundButtonFont1:SetText("��");
		SimulateFrameSecondBig_FirstChildAttrBackGroundButtonFont2:SetText("��");
		SimulateFrameSecondBig_FirstChildAttrBackGroundButtonFont3:SetText("��");
		SimulateFrameSecondBig_FirstChildAttrBackGroundButtonFont4:SetText("��");
		SimulateFrameSecondBig_FirstChildAttrBackGroundButtonFont5:SetText("");
		SimulateFrameSecondBig_SecondChildAttrBackGroundButtonFont1:SetText("��");
		SimulateFrameSecondBig_SecondChildAttrBackGroundButtonFont2:SetText("��");
		SimulateFrameSecondBig_SecondChildAttrBackGroundButtonFont3:SetText("��");
		SimulateFrameSecondBig_SecondChildAttrBackGroundButtonFont4:SetText("��");
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
		SimulateFrameSecondBig_FirstChildAttrBackGroundButtonFont1:SetText("Ԫ");
		SimulateFrameSecondBig_FirstChildAttrBackGroundButtonFont2:SetText("��");
		SimulateFrameSecondBig_FirstChildAttrBackGroundButtonFont3:SetText("��");
		SimulateFrameSecondBig_FirstChildAttrBackGroundButtonFont4:SetText("��");
		SimulateFrameSecondBig_FirstChildAttrBackGroundButtonFont5:SetText("��");
		SimulateFrameSecondBig_SecondChildAttrBackGroundButtonFont1:SetText("Ԫ");
		SimulateFrameSecondBig_SecondChildAttrBackGroundButtonFont2:SetText("��");
		SimulateFrameSecondBig_SecondChildAttrBackGroundButtonFont3:SetText("��");
		SimulateFrameSecondBig_SecondChildAttrBackGroundButtonFont4:SetText("��");
		SimulateFrameSecondBig_SecondChildAttrBackGroundButtonFont5:SetText("��");
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
	SimulateFrameSelectbtnFont:SetText("��");
	SimulateFrameListSelect:Hide();
	SimulateFrameEditAttackmodulus:SetText(SimulateMgr:getAtkLevelMul(SimArmourID));
	SimulateFrameEditRecoverymodulus:SetText(SimulateMgr:getDefLevelMul(SimArmourID));
end

function SimulateFrameListSelect2btn_OnClick()
	AttackID=SimSecondID;
	SimulateFrameSelectbtnFont:SetText("��");
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
		ShowMidTips("��ѡ��δ��!");
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
	SimulateFrameSelectbtnFont:SetText("��");
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