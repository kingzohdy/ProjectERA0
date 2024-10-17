local MAX_SKILL_PAGE = 5

-- 符文图标
SkillRuneIcon = {
	"uires\\TuPianLei\\WuPin\\5240000.tga",
	"uires\\TuPianLei\\WuPin\\5240001.tga",
	"uires\\TuPianLei\\WuPin\\5240002.tga",
}

SkillRuneItemAttr = {
	[RESULT_ADD_BASE_ATTR_DEF] = "敏捷",
	[RESULT_ADD_BASE_ATTR_STR] = "力量",
	[RESULT_ADD_BASE_ATTR_INT] = "智力",
	[RESULT_ADD_BASE_ATTR_STA] = "体质",
	[RESULT_ADD_BASE_ATTR_SPR] = "精神",
	[RESULT_ADD_BASE_ATTR_AGILE] = "感知",
}

SkillRuneCombineItemId = {
	[0] = 5251213,	-- 低级符合合成卷轴
	[1] = 5251214,	-- 高级符文合成卷轴
	[2] = 5251215,	-- 高级符文合成卷轴
}

SkillRuneImportantItemId = 5251205	-- 符文之力

SkillRuneItemId = {	-- 符文道具
	[5251207] = {level = 0, color = 1},
	[5251208] = {level = 0, color = 2},
	[5251209] = {level = 0, color = 3},
	
	--[[
	[5251210] = {level = 1, color = 1},
	[5251211] = {level = 1, color = 2},
	[5251212] = {level = 1, color = 3},
	--]]
	
	[5251202] = {level = 1, color = 1},
	[5251203] = {level = 1, color = 2},
	[5251204] = {level = 1, color = 3},
}

SkillRuneRecommend = {
	[1] = "#cFFFFBE攻击类#n",
	[2] = "#cFFFFBE防御类#n",
}


local SkillRuneItemIdByColor = {}
local SkillRuneItemIdByLevel = {}

for id, attr in pairs(SkillRuneItemId) do
	SkillRuneItemIdByColor[attr.color] = SkillRuneItemIdByColor[attr.color] or {}
	SkillRuneItemIdByLevel[attr.level] = SkillRuneItemIdByLevel[attr.level] or {}
	SkillRuneItemIdByColor[attr.color][attr.level] = id
	SkillRuneItemIdByLevel[attr.level][attr.color] = id
end

-- 战士的快捷键技能
role_skill_1 =
	{
		{1200,1100},		-- Ctrl + 鼠标右键 快捷键绑定技能
		{1016},				-- Ctrl + 鼠标左键 快捷键绑定技能
		{1012},				-- Alt + 鼠标左键 快捷键绑定技能
		{7000}				-- Alt + 鼠标右键 快捷键绑定技能
	};

-- 法师的快捷键技能
role_skill_2 =
	{
		{2100,2200},
		{2014},
		{2104,2212},
		{7001}
	};

-- 枪手的快捷键技能
role_skill_3 =
	{
		{3016,3210},
		{3012},
		{3100,3220},
		{7002}
	};

-- 祭司的快捷键技能
role_skill_4 =
	{
		{4100,4200},
		{4018},
		{4004},
		{7003}
	};

local skillList = {}
local skillListIndex = {}
local firstSkill
local selectSkill

local runeOpenSkill
local lockSkillNum = 0;

function InitSkillData()
	skillList = {}
end

function OnSkillInfoDel()
	InitSkillData()
end

function InitSkillFrame()
	firstSkill = nil
	selectSkill = nil
	SkillFrameScrollBar:SetValue(0)
end

function GetSkillOnePage()
	local index = 0
	if selectSkill then
		index = skillListIndex[selectSkill] - SkillFrameScrollBar:GetValue()
	end
	if index > 0 and index <= MAX_SKILL_PAGE then
		return MAX_SKILL_PAGE - 2
	end
	selectSkill = nil
	return MAX_SKILL_PAGE
end

function EnumLearnedSkill()
	return function(control, id)
		if id then
			local nextId = control:getNextLearnedSkill(id)
			if nextId ~= 0 then
				return nextId
			end
		else
			return control:getNextLearnedSkill(0)
		end
	end, SkillMgr
end

--在技能列表里加上夫妻技能
--11403是装备婚戒后获得的，脱下婚戒就没了
--11402 11404是建立夫妻关系后获得的，解除夫妻关系后就没了
function AddLoveSkill()
	local MainPlayer = ActorMgr:getMainPlayer();
	--再检查下是否有夫妻关系，判断11402和11404技能
	if MainPlayer:getLoveColorType() == 2 then
		local s = SkillMgr:getSpellStaticAtt(11402, 1)
		if s and (s.Learn.Career == 0 or s.Learn.Career == race or s.Learn.Career == race % 0x10 or s.Learn.Career % 0x10 == race) then
			table.insert(skillList, 11402)
		end
		s = SkillMgr:getSpellStaticAtt(11404, 1)
		if s and (s.Learn.Career == 0 or s.Learn.Career == race or s.Learn.Career == race % 0x10 or s.Learn.Career % 0x10 == race) then
			table.insert(skillList, 11404)
		end
	else
		return
	end
	--先检查是否有婚戒而且有夫妻关系,判断是不是可以有11403技能
	local equip = MainPlayer:getEquip();
	local ring1 = equip:getItem( 12 );
	local ring2 = equip:getItem( 13 );
	if IsMarryRing( ring1:getItemId() ) or IsMarryRing( ring2:getItemId() ) then
		local s = SkillMgr:getSpellStaticAtt(11403, 1)
		if s and (s.Learn.Career == 0 or s.Learn.Career == race or s.Learn.Career == race % 0x10 or s.Learn.Career % 0x10 == race) then
			table.insert(skillList, 11403)
		end
	end
end

function GetAllSkillList()
	if #skillList == 0 then
		local MainPlayer = ActorMgr:getMainPlayer()
		local race = MainPlayer:getRace()
		local t = {}
		local has = {}
		for _, i in ipairs(GetSkillList(race)) do
			t[i] = true
		end
		for i in EnumLearnedSkill() do
			if SKILL_COMMON[i] then
				t[i] = true
			end
			has[i] = true
		end
		for i in pairs(t) do
			local s = SkillMgr:getSpellStaticAtt(i, 1)
			if s and (s.Learn.Career == 0 or s.Learn.Career == race or s.Learn.Career == race % 0x10 or s.Learn.Career % 0x10 == race) then
				table.insert(skillList, i)
			end
		end
		--添加夫妻技能
		AddLoveSkill();
		table.sort(skillList, function(a, b)
			if has[a] and not has[b] then
				return true
			end
			if not has[a] and has[b] then
				return false
			end
			a = SkillMgr:getSpellStaticAtt(a, 1)
			b = SkillMgr:getSpellStaticAtt(b, 1)
			if a and b then
				if a.Learn.Career ~= b.Learn.Career then
					return a.Learn.Career < b.Learn.Career
				elseif a.Learn.LevelMin ~= b.Learn.LevelMin then
					return a.Learn.LevelMin < b.Learn.LevelMin
				end
				return a.SkillID < b.SkillID
			end
			return a or not b
		end)
		skillListIndex = {}
		for i, v in ipairs(skillList) do
			skillListIndex[v] = i
		end
	end
	
	
	return skillList
end

function EnumSkillRuneAddAttr()
	local id = 0
	return function()
		id = RuneMgr:GetNextRuneAddAttr(id)
		if id == 0 then
			return
		end
		return id
	end
end

function AcceclKeySkillLearn(nSkillId)
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end

	local nRace			= MainPlayer:getRace() % 16;
	local t_skill		= getglobal("role_skill_"..nRace);

	for i=1,table.getn(t_skill) do
		for j=1,table.getn(t_skill[i]) do
			if nSkillId == t_skill[i][j] then
				local nSkillvalue	= GameShortCut:getSPShortCut(i-1);
				if nSkillvalue == 0  then
					GameShortCut:setSPShortCut(i-1,nSkillId);
				end
			end
		end
	end
end

function SkillFrame_OnLoad()
	this:RegisterEvent("GE_SKILL_LIST_UPDATE")
	this:RegisterEvent("GE_CHANGE_RACE")
	this:RegisterEvent("GE_SKILL_COOLD")
	this:RegisterEvent("GE_RUNE_EX_INFO_UPDATE")
	this:RegisterEvent("GE_ENTER_PLAYERLOGIN")
	this:RegisterEvent("GE_LOVE_INFO_CHANGE")
	
	local UIControl
	for i = 1, 3 do
		UIControl = getglobal("SkillFrameExRuneInfo"..i.."RingTex")
		UIControl:SetTexture(SkillRuneIcon[i])
	end
	for i = 1, MAX_SKILL_PAGE do
		local name = "SkillFrameSkill"..i.."Extend"
		if getglobal(name) then
			for j = 1, 3 do
				UIControl = getglobal(name.."Entry"..j.."Icon")
				UIControl:SetTexture(SkillRuneIcon[j])
				UIControl:setMask("uires\\ui\\mask\\Round.tga")
			end
		end
	end
end

function SkillFrame_OnEvent()
	if arg1 == "GE_SKILL_LIST_UPDATE" then
		if SkillFrame:IsShown() then
			ClearSkillFrameUVTex()
			UpdateSkillFrame()
		end
		UpdateShortCutSkill()
	elseif arg1 == "GE_RUNE_EX_INFO_UPDATE" then
		if SkillFrame:IsShown() then
			UpdateSkillExRuneInfo()
		end
	elseif arg1 == "GE_CHANGE_RACE" then
		if SkillFrame:IsShown() then
			UpdateSkillFrame()
		end
	elseif arg1 == "GE_SKILL_COOLD" then
		local Msg = UIMSG:getUIMsg()
		local id = Msg.CoolTime.num
		if SkillFrame:IsShown() then
			local frame
			for i = 1, MAX_SKILL_PAGE do
				frame = getglobal("SkillFrameSkill"..i)
				if frame:GetClientUserData(0) == id then
					local btn = getglobal("SkillFrameSkill"..i.."Item_")
					btn:SetCooldownTimer(Msg.CoolTime.starttime, Msg.CoolTime.endtime, true)
					break
				end
			end
		end
	elseif arg1 == "GE_ENTER_PLAYERLOGIN" then
		local MainPlayer = ActorMgr:getMainPlayer();
		if MainPlayer and MainPlayer:getLv() >= RUNE_OPEN_LV then
			lockSkillNum = countRuneSlot();	
		end
	elseif arg1 == "GE_LOVE_INFO_CHANGE" then
		InitSkillData()
		if SkillFrame:IsShown() then
			ClearSkillFrameUVTex()
			UpdateSkillFrame()
		end
		UpdateShortCutSkill()
	end
end

function SkillFrame_OnShow()
	if DiathesisFrame:IsShown() then
		local x, y = DiathesisFrame:GetRealLeft(), DiathesisFrame:GetRealTop()
		DiathesisFrame:Hide()
		this:SetPoint("topleft", "$parent", "topleft", x, y);
	else
		this:SetPoint("topleft", "$parent", "topleft", 32, 163)
	end
	SkillFrameTipDesc:SetText("  小提示：你可以到时间城找技能学习员#L#cD97500康拉德@@[306000,1000]#n升级已有技能\n找战场荣誉商人#L#cD97500梅杰@@[302033,1000]#n、正义点数商人#L#cD97500喜多@@[303067,1000]#n购买符文", 255, 254, 189)
	local MainPlayer	= ActorMgr:getMainPlayer()
	local nLevel		= MainPlayer:getLv()	

	UpdateSkillFrame()
	UpdateSkillExRuneInfo()
end

function SkillFrame_OnHide()
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "开启符文槽" then
		MessageBoxFrame:Hide();
	end
	--ClearSkillFrameUVTex()	
end

function ClearSkillFrameUVTex()
	for i = 1, MAX_SKILL_PAGE do
		local UVTex = getglobal("SkillFrameSkill"..i.."Item_UVAnimationTex")
		UVTex:Hide()
	end
end

function ShowSkillRuneXinShouUVTex(index, show)
	local uv = getglobal("SkillFrameSkill"..index.."ItemRuneUVAnimationTex")
	if show then
		uv:SetUVAnimation(80, true)
		uv:Show()
	else
		uv:Hide()
	end
end

function ClearSkillRuneXinShouUVTex()
	for i = 1, MAX_SKILL_PAGE do
		ShowSkillRuneXinShouUVTex(i, false)
	end
end

local isXinShouPopShow = false;

function SetSkillItem(item, id)
	item:SetClientUserData(0, id)
	local name		= item:GetName()
	local level		= SkillMgr:getSpellLv(id)
	local RuneType = SkillMgr:getSpellRuneType(id)
	if not SkillMgr:isSkillLearned(id) then
		level = 0
	end
	local UIControl = getglobal(name.."Item")
	UIControl:SetClientUserData(0, id)
	local GrayLevel = getglobal(name.."ItemGrayLevel")
	UIControl = getglobal(name.."ItemLevel")
	if level == 0 then
		local SkillDef = SkillMgr:getSpellStaticAtt( id, 1 );
		UIControl:SetText("");
		GrayLevel:SetText(string.format( "人物%d级可学", SkillDef.Learn.LevelMin == 0 and 1 or SkillDef.Learn.LevelMin ) );
	else
		UIControl:SetTextColor(255, 254, 189)
		UIControl:SetText(string.format("%d级", level))
		GrayLevel:SetText("");
		local SkillDef = SkillMgr:getSpellStaticAtt( id, level );
		if SkillDef.NextLevelDesc ~= "" then
			local MainPlayer = ActorMgr:getMainPlayer()
			SkillDef = SkillMgr:getSpellStaticAtt( id, level + 1 );
			if SkillDef.SkillID ~= 0 and MainPlayer ~= nil and SkillDef.Learn.LevelMin <= MainPlayer:getLv() then
				UIControl = getglobal(name.."Item_UpTex");
				UIControl:Show();
				UIControl = getglobal(name.."Item_UVAnimationTex");
				UIControl:SetUVAnimation(80, true);
				UIControl:Show();
			end
		end
	end
	local isXinShouParentBtn = false;
	UIControl = getglobal(name.."ItemRune");
	UIControl:SetNormalTextureTemplate( "TransTextureTemplate" );
	UIControl:SetHighlightTextureTemplate( "TransTextureTemplate" );
	UIControl:SetPushedTextureTemplate( "TransTextureTemplate" );
	if RuneMgr:CanLock(id) then
		UIControl = getglobal(name.."ItemRuneDisable")
		UIControl:Hide()
		UIControl = getglobal(name.."ItemRuneLock")
		if SkillMgr:getSpellRuneEnable(id) then
			UIControl:Hide()
			UIControl = getglobal(name.."ItemRuneIcon")
			if RuneType ~= 0 then				
				local ItemId = SkillMgr:getSpellRuneItemId(id, RuneType)
				UIControl:Show()
				UIControl:SetTexture(GetItemIconPathByID(ItemId))
			else
				UIControl:Hide()
				UIControl = getglobal(name.."ItemRune");
				UIControl:SetOverlayRegion(1,1,33,33);
				UIControl:SetNormalTextureTemplate( "RuneBtnNormalTemplate" );
				UIControl:SetHighlightTextureTemplate( "RuneBtnHighlightTemplate" );
				UIControl:SetPushedTextureTemplate( "RuneBtnPushedTemplate" );
			end

			local has = false
			for i = 1, 3 do
				if SkillMgr:getSpellRuneItemId(id, i) ~= 0 then
					has = true
					break
				end
			end
			
			if not isXinShouPopShow and not GameWizard:isDo(GAMEWIZARD_FIRST_GET_RUNE_CONTAINER_BTN) then
				local bFound = FindSomeThingInContainer( RESULT_RUNE );
				if bFound then
					local szText = "左键点击打开符文列表";
					local xinShouBtn = getglobal( name.."ItemRuneXinShouBtn" );
					local rich = getglobal( xinShouBtn:GetName() .. "Rich" );
					rich:SetClientString( szText );
					ReSizeTooltip( rich:GetName(), xinShouBtn:GetName(), 14 );			
					xinShouBtn:Show();
					isXinShouParentBtn = true;
					isXinShouPopShow = true;
				end
			end
		else
			UIControl:Show()
			UIControl = getglobal(name.."ItemRuneIcon")
			UIControl:Hide();
			local MainPlayer = ActorMgr:getMainPlayer();
			local SkillDef = SkillMgr:getSpellStaticAtt( id, 1 );
			if MainPlayer ~= nil and SkillDef ~= nil and MainPlayer:getLv() >= RUNE_OPEN_LV and 
					SkillMgr:isSkillLearned(id) and MainPlayer:getLv() >= SkillDef.Learn.LevelMin then
				ShowSkillRuneXinShouUVTex(item:GetClientID(), true)
				UIControl = getglobal(name.."ItemRuneLock")
				UIControl:Hide();
				UIControl = getglobal(name.."ItemRune");
				UIControl:SetOverlayRegion(0,0,35,35);
				UIControl:SetNormalTextureTemplate( "UnlockNormalTemplate" );
				UIControl:SetHighlightTextureTemplate( "UnlockHighlightTemplate" );
				UIControl:SetPushedTextureTemplate( "UnlockPushedTemplate" );
			end
		end		
	else
		UIControl = getglobal(name.."ItemRuneDisable")
		UIControl:Show()
	end
	local SkillDef	= SkillMgr:getSpellStaticAtt(id, level == 0 and 1 or level)
	if SkillDef ~= nil then
		-- 设置名字
		UIControl = getglobal(name.."ItemName")
		local GrayName = getglobal(name.."ItemGrayName")
		if level == 0 then
			UIControl:SetTextColor(131, 128, 108)
			if RuneType == 0 then
				UIControl:SetText("")
				GrayName:SetText(SkillDef.Name)
			else
				local RuneSkillId = SkillMgr:getSpkillRuneSkillId(id, RuneType)
				local RuneSkillDef = SkillMgr:getSpellStaticAtt(RuneSkillId, level == 0 and 1 or level)
				UIControl:SetText("")
				GrayName:SetText(RuneSkillDef.Name)
			end
		else
			UIControl:SetTextColor(0, 255, 0)
			if RuneType == 0 then
				UIControl:SetText(SkillDef.Name)
				GrayName:SetText("")
			else
				local RuneSkillId = SkillMgr:getSpkillRuneSkillId(id, RuneType)
				local RuneSkillDef = SkillMgr:getSpellStaticAtt(RuneSkillId, level == 0 and 1 or level)
				UIControl:SetText(RuneSkillDef.Name)
				GrayName:SetText("")
			end
		end
		-- 设置技能数据
		local cd = SkillMgr:getCDTime(id)
		UIControl = getglobal(name.."Item_")
		UIControl:SetClientUserData(0, id)
		UIControl:SetCooldownTimer(cd:past() / 1000, cd:total() / 1000, true)
		if SkillMgr:isSkillPassive(id) then
			UIControl:Disable()
		else
			UIControl:Enable()
		end
		-- 设置图标
		local IconPath = GetSkillIconPath()..SkillDef.IconID1..".tga"
		if not IsInExistence(IconPath) then
			IconPath = DEFAULT_ITEM_PATH
		end
		UIControl = getglobal(name.."Item_Icon")
		UIControl:SetTexture(IconPath)
		if level == 0 then
			UIControl:SetGray(true)
		else
			UIControl:SetGray(false)
		end
	end
	UIControl = getglobal(name.."Item")
	if getglobal(name.."Extend") then
		for i = 1, 3 do
			UIControl = getglobal(name.."ExtendEntry"..i)
			UIControl:SetClientUserData(0, id)
			UIControl = getglobal(name.."ExtendEntry"..i.."Skill")
			if i == RuneType then
				UIControl:SetText(string.format("%s  %d级", SkillDef.Name, level == 0 and 1 or level))
			else
				local RuneSkillId = SkillMgr:getSpkillRuneSkillId(id, i)
				local RuneSkillDef = SkillMgr:getSpellStaticAtt(RuneSkillId, level == 0 and 1 or level)
				UIControl:SetText(string.format("%s  %d级", RuneSkillDef.Name, level == 0 and 1 or level))
			end
			local ItemId = SkillMgr:getSpellRuneItemId(id, i)
			if ItemId ~= 0 then
				UIControl = getglobal(name.."ExtendEntry"..i.."RuneIcon")
				UIControl:SetTexture(GetItemIconPathByID(ItemId))
				UIControl:Show()
				UIControl = getglobal(name.."ExtendEntry"..i.."SkillSelect")
				UIControl:Enable()
				UIControl = getglobal(name.."ExtendEntry"..i.."Skill")
				UIControl:Enable()
				UIControl:setInputTransparent(false)
				UIControl = getglobal(name.."ExtendEntry"..i.."Text")
				UIControl:SetText("")
				UIControl:SetTextColor(255, 254, 189)
				UIControl = getglobal(name.."ExtendEntry"..i.."RuneXinShouBtn")
				UIControl:Hide()
				local t = {}
				local itemdef = getItemDef(ItemId)
				for j = 1, MAX_ITEM_RESULT do
					local attr = SkillRuneItemAttr[itemdef.Result[j - 1].ResultID]
					if attr then
						table.insert(t, string.format("+%d %s", itemdef.Result[j - 1].ResultVal2, attr))
					end
				end
				if #t > 1 then
					for j = 1, 2 do
						UIControl = getglobal(name.."ExtendEntry"..i.."Text"..j)
						UIControl:SetText(t[j])
					end
				elseif #t == 1 then
					UIControl = getglobal(name.."ExtendEntry"..i.."Text")
					UIControl:SetText(t[1])
				end
			else
				UIControl = getglobal(name.."ExtendEntry"..i.."RuneIcon")
				UIControl:Hide()				
				UIControl = getglobal(name.."ExtendEntry"..i.."SkillSelect")
				UIControl:Disable()
				UIControl = getglobal(name.."ExtendEntry"..i.."Skill")
				UIControl:Disable()
				UIControl:setInputTransparent(true)
				UIControl = getglobal(name.."ExtendEntry"..i.."Text")
				UIControl:SetText("未激活")
				UIControl:SetTextColor(131, 128, 108)
				UIControl = getglobal(name.."ExtendEntry"..i.."Text1")
				UIControl:SetText("")
				UIControl = getglobal(name.."ExtendEntry"..i.."Text2")
				UIControl:SetText("")
				UIControl = getglobal( name.."ExtendEntry"..i.."RuneXinShouBtn" );
				UIControl:Hide();
				if isXinShouParentBtn and not GameWizard:isDo(GAMEWIZARD_FIRST_GET_RUNE_CONTAINER_BTN) then
					local _, id = FindSomeThingInContainer( RESULT_RUNE );
					local MainPlayer = ActorMgr:getMainPlayer();
					if not MainPlayer then
						return
					end
					local parent = getglobal( name.."ExtendEntry"..i );
					local container = MainPlayer:getContainer();					
					local item = container:getItem(id);
					if item and RuneMgr:RuneItemType(item:getItemId()) == parent:GetClientID() then	
						local t_color = { "#cff0000红色", "#c00c8ff蓝色", "#c00ff00绿色" };
						local szText = "左键拖拽" .. t_color[i] .. "#c554623符文至镶嵌框中，以激活符文技能";
						local xinShouBtn = getglobal( name.."ExtendEntry"..i.."RuneXinShouBtn" );
						local rich = getglobal( xinShouBtn:GetName() .. "Rich" );
						rich:SetClientString( szText );
						ReSizeTooltip( rich:GetName(), xinShouBtn:GetName(), 14 );			
						xinShouBtn:Show();
						isXinShouParentBtn = false;
					end
					local frame = getglobal( name.."Extend" )
					if frame:IsShown() then
						local xinShouBtn = getglobal( name.."ItemRuneXinShouBtn" );
						xinShouBtn:Hide();
					end
				end
			end
		end
	end
end

function SkillFrame_ScrollUpBtn()
	local slider = SkillFrameScrollBar
	if not slider:IsShown() then
		return
	end
	local nValue = slider:GetValue() - slider:GetValueStep()
	if nValue >= slider:GetMinValue() then
		slider:SetValue(nValue)
	end
end

function SkillFrame_ScrollDownBtn()
	local slider	= SkillFrameScrollBar
	if not slider:IsShown() then
		return
	end
	local nValue	= slider:GetValue() + slider:GetValueStep()
	if nValue <= slider:GetMaxValue() then
		slider:SetValue(nValue)
	end
end

function UpdateSkillFrame()
	local slider	= SkillFrameScrollBar
	local value = slider:GetValue()
	local skills = GetAllSkillList()
	local max = #skills
	local tex = getglobal(slider:GetName().."ThumbTexture")
	tex:Show()
	local frame
	if max <= GetSkillOnePage() then
		slider:SetMaxValue(0)
		tex:Hide()
		if value ~= 0 then
			return slider:SetValue(0)
		end
	end
	local maxValue = max - GetSkillOnePage()
	slider:SetMaxValue(maxValue)
	if value > maxValue then
		return slider:SetValue(maxValue)
	end
	if firstSkill then
		local index = skillListIndex[firstSkill]
		if index and index - 1 <= maxValue then
			if index ~= value + 1 then
				return slider:SetValue(index - 1)
			end
		else
			firstSkill = skills[value + 1]
		end
	end
	for i = 1, MAX_SKILL_PAGE do
		frame = getglobal("SkillFrameSkill"..i)
		frame:Show()
		frame = getglobal("SkillFrameSkill"..i.."Item")
		frame:DisChecked()
		frame = getglobal("SkillFrameSkill"..i.."Item_UpTex")
		frame:Hide();
		frame = getglobal("SkillFrameSkill"..i.."ItemRuneXinShouBtn")
		frame:Hide();
		frame = getglobal("SkillFrameSkill"..i.."Extend")
		if frame then
			frame:Hide()
		end
	end
	if selectSkill then
		local index = skillListIndex[selectSkill]
		local diff = index - value
		if diff > MAX_SKILL_PAGE - 2 and diff <= MAX_SKILL_PAGE then
			return slider:SetValue(value + diff + 2 - MAX_SKILL_PAGE)
		end
		if diff > 0 and diff <= MAX_SKILL_PAGE then
			frame = getglobal("SkillFrameSkill"..diff.."Extend")
			frame:Show()
			frame = getglobal("SkillFrameSkill"..diff.."Item")
			--frame:Checked()
			for i = 1, 2 do
				frame = getglobal("SkillFrameSkill"..i + diff)
				frame:Hide()
			end
		end
	end
	ClearSkillFrameUVTex();
	ClearSkillRuneXinShouUVTex()
	HideRuneSkillChgUV();
	local index = value
	local id
	isXinShouPopShow = false;
	for i = 1, MAX_SKILL_PAGE do
		frame = getglobal("SkillFrameSkill"..i)
		if frame:IsShown() then
			index = index + 1
			id = skills[index]
			if not id then
				break
			end
			SetSkillItem(frame, id)
		end
	end
end

function UpdateSkillExRuneInfo()
	local UIControl
	for i = 1, 3 do
		local info = RuneMgr:GetExRuneInfo(i)
		if info then
			local name
			if info.attr ~= 0 then
				local active = (info.num >= info.total)
				UIControl = getglobal("SkillFrameExRuneInfo"..i)
				UIControl:Enable()
				UIControl = getglobal("SkillFrameExRuneInfo"..i.."Name")
				UIControl:SetText(RuneMgr:GetRuneAddAttrName(info.attr))
				if active then
					UIControl:SetTextColor(255, 254, 189)
				else
					UIControl:SetTextColor(131, 128, 108)
				end
				UIControl = getglobal("SkillFrameExRuneInfo"..i.."Progress")
				UIControl:SetText(string.format("%2d/%02d", info.num, info.total))
				if active then
					UIControl:SetTextColor(0, 255, 0)
				else
					UIControl:SetTextColor(131, 128, 108)
				end
			else
				UIControl = getglobal("SkillFrameExRuneInfo"..i)
				UIControl:Disable()
				UIControl = getglobal("SkillFrameExRuneInfo"..i.."Name")
				UIControl:SetText("未激活")
				UIControl:SetTextColor(131, 128, 108)
				UIControl = getglobal("SkillFrameExRuneInfo"..i.."Progress")
				UIControl:SetText(string.format("%d/%d", info.num, info.total))
				UIControl:SetTextColor(131, 128, 108)
			end
			if info.num > 0 then
				UIControl = getglobal("SkillFrameExRuneInfo"..i.."RingTex")
				UIControl:SetGray(false)
				UIControl = getglobal("SkillFrameExRuneInfo"..i.."Ring")
				UIControl:Enable();
				UIControl = getglobal("SkillFrameExRuneInfo"..i.."RingTips")
				UIControl:Hide();
			else
				UIControl = getglobal("SkillFrameExRuneInfo"..i.."RingTex")
				UIControl:SetGray(true)
				UIControl = getglobal("SkillFrameExRuneInfo"..i.."Ring")
				UIControl:Disable();
				UIControl = getglobal("SkillFrameExRuneInfo"..i.."RingTips")
				UIControl:Show();
			end
		end
	end
end

function ExpandSkillRuneInfo(id)
	if not RuneMgr:CanLock(id) then
		return
	end
	if id == selectSkill then
		selectSkill = nil
	else
		selectSkill = id
	end
	--GameWizard:doStep(GAMEWIZARD_FIRST_OPEN_SKILL_RUNE)
	UpdateSkillFrame()
end

function SkillFrame_ScrollBarValueChange()
	local skills = GetAllSkillList()
	local value = this:GetValue()
	local lastValue = arg1
	if value == 0 then
		firstSkill = nil
	else
		firstSkill = skills[value + 1]
	end
	if selectSkill then
		local index = skillListIndex[selectSkill]
		local diff = index - value
		if diff > MAX_SKILL_PAGE - 2 and diff <= MAX_SKILL_PAGE then
			if lastValue < value then
				value = value + diff + 2 - MAX_SKILL_PAGE
			else
				value = value + diff - MAX_SKILL_PAGE - 1
			end
			if value >= 0 and value <= this:GetMaxValue() then
				return this:SetValue(value)
			else
				value = 0
				firstSkill = nil
				selectSkill = nil
			end
		end
	end
	UpdateSkillFrame()
	if lastValue < value and this:GetValue() < value then
		selectSkill = nil
		this:SetValue(value)
	end
end

function SkillFrame_OnMouseWheel()
	local old = this
	this = SkillFrameScrollBar
	ScrollBar_OnMouseWheel()
	this = old
end

function SkillItem_OnClick()
	local parent = getglobal(this:GetParent())
	local id = parent:GetClientUserData(0)
	if not RuneMgr:CanLock(id) then
		ShowMidTips("该技能无符文技能")
	elseif SkillMgr:getSpellRuneEnable(id) then
		ExpandSkillRuneInfo(id)
	else
		ShowMidTips("符文还未解锁")
	end
end

function SkillInfo_OnEnter()
	local id = this:GetClientUserData(0)
	if id ~= 0 then
		SpellTipsFrame:SetClientUserData(0, SkillMgr:getRuneSkillId(id))
		SpellTipsFrame:SetClientUserData(1, SkillMgr:getSpellLv(id))
		SpellTipsFrame:SetClientUserData(2, SKILL_TYPE)
		SpellTipsFrame:SetClientString(this:GetName())
		SpellTipsFrame:Show()
	end
	util.HightParent(this)
end

function SkillInfo_OnLeave()
	SpellTipsFrame:Hide()
	util.HightParent(this, false)
end

local skillInfoDragIndex = {}
local indexDragSkillInfo = {}
local skillInfoDragIndexFree = 1

function IndexSkillInfoTemplate(index)
	return indexDragSkillInfo[index]
end

function DragSkillIcon(skillId, name, id, tex)
	if UIIsInDragState() then
		UIEndDrag()
		UIMSG:getUIGV().DragFrom = DRAG_NONE
		return false
	end
	if not string.find(name, tostring(id), 1, true) then
		return false
	end
	local template = string.gsub(name, string.format("^(.*)%d", id), "%1{id}")
	local index = skillInfoDragIndex[template]
	if not index then
		index = skillInfoDragIndexFree
		skillInfoDragIndexFree = skillInfoDragIndexFree + 1
		skillInfoDragIndex[template] = index
		indexDragSkillInfo[index] = template
	end
	
	UIMSG:getUIGV().DragFrom = DRAG_SKILL
	UIBeginDrag(tex, skillId, id, index)

	-- 处于拖拽状态 同时 右部技能栏未显示 则显示右部技能栏
	if UIIsInDragState() and not ExtendSkillLeftFrame:IsShown() then
		ShowExtendSkill()
	end
	
	return true
end

function SkillInfo_Drag()
	local skillId = this:GetClientUserData(0)
	if not SkillMgr:isSkillLearned(skillId) then
		ShowMidTips("尚未学会该技能")
		return false
	end
	
	local name = this:GetName()
	-- 关闭新手指引特效显示
	local tex = getglobal(name.."UVAnimationTex")
	tex:Hide()
	
	tex = getglobal(name.."Icon")
	local parent = getglobal(this:GetParent())
	parent = getglobal(parent:GetParent())
	DragSkillIcon(skillId, name, parent:GetClientID(), tex:GetTexture())
end

function SkillInfo_OnClick()
	local skillId		= this:GetClientUserData(0)
	if not SkillMgr:isSkillLearned(skillId) then
		return
	end
	
	local name = this:GetName()
	-- 关闭新手指引特效显示
	local tex = getglobal(name.."UVAnimationTex")
	tex:Hide()

	if UIIsInDragState() then
		UIEndDrag()
		UIMSG:getUIGV().DragFrom = DRAG_NONE
		return
	end

	local MainPlayer = ActorMgr:getMainPlayer()
	if MainPlayer then
		GameShortCut:useSkill(skillId)
		SkillMgr:unUseSpell()
	end
end

function SkillRune_OnEnter()
	util.HightParent(this)
	local name = this:GetName()
--	local highlight = getglobal(name.."Highlight")
--	if highlight then
--		highlight:Show()
--	end
	local parent = getglobal(this:GetParent())
	local id = parent:GetClientUserData(0)
	if not RuneMgr:CanLock(id) then
		SetGameTooltips(name, "该技能无符文技能")
	elseif SkillMgr:getSpellRuneEnable(id) then
		SetGameTooltips(name, "点击可展开符文技能")
	else
		SetGameTooltips(name, "符文还未解锁")
	end
end

function SkillRune_OnLeave()
	util.HightParent(this, false)
--	local highlight = getglobal(this:GetName().."Highlight")
--	if highlight then
--		highlight:Hide()
--	end
	ItemButton_OnLeave()
	GameTooltipRichText:SetText("", 255, 255, 255)
	GameTooltip:Hide()
end

local resultBtn = "";
function RuneResult(sType, id, result)
	if result then
		util.PlaySound("succeed")
		if sType == "insert" and resultBtn ~= "" then
			if not GameWizard:isDo( GAMEWIZARD_FIRST_GET_RUNE_CONTAINER_BTN ) then
				GameWizard:doStep( GAMEWIZARD_FIRST_GET_RUNE_CONTAINER_BTN );
			end			
		end
	end
end

function RuneUnLock( id, nNeedBull )
	local result = RuneMgr:Unlock(id)
	if result == RuneMgr.UNLOCK_SUCCEED then
	elseif result == RuneMgr.UNLOCK_LEVELLIMIT then
		util.PlaySound("fail")
		ShowMidTips("您需要达到40级后，才可执行该操作！")
	elseif result == RuneMgr.UNLOCK_DEAD then
		ShowMidTips("死亡状态下无法执行该操作")
	end
end

function SkillRune_OnClick()
	if CostMoneyConfirmFrame:IsShown() then
		return;
	end
	local parent = getglobal(this:GetParent())
	local id = parent:GetClientUserData(0)
	if not RuneMgr:CanLock(id) then
		return
	end
	if RuneMgr:IsRuneEnable(id) then
		return ExpandSkillRuneInfo(id)
	else
		if not SkillMgr:isSkillLearned(id) then
			util.PlaySound("fail")
			return ShowMidTips("你没有学会该技能！")
		end
		local MainPlayer = ActorMgr:getMainPlayer()
		if MainPlayer == nil then return end;
		if MainPlayer:getLv() < 40 then
			return ShowMidTips("40级时可激活符文技能")
		end
		if MainPlayer:isDead() then
			return ShowMidTips("死亡状态下无法执行该操作")
		end		
		local SkillDef = SkillMgr:getSpellStaticAtt( id, 1 );
		if SkillDef ~= nil then
			local nNeedBull = math.ceil( math.ceil( ( SkillDef.Learn.LevelMin / 10 ) ^ 3 * 115 ) / 1000 ) * 1000;
			MessageBox( "提示", "消耗" .. nNeedBull .. "绑银，开启符文技能，是否开启？" );
			MessageBoxFrame:SetClientString( "开启符文槽" );
			MessageBoxFrame:SetClientUserData( 0, id );
			MessageBoxFrame:SetClientUserData( 1, nNeedBull );
		end
	end
end

SKILL_FLAG_SPE_RUNE = 1 	--/* 该技能的符文需要特定相对应的符文 */
function SkillRuneExtendEntry_OnEnter()
	util.HightParent(this)
	local name = this:GetName()
	local highlight = getglobal(name.."Highlight")
	if highlight then
		highlight:Show()
	end
	local parent = getglobal(this:GetParent())
	local id = parent:GetClientUserData(0)
	if SkillMgr:getSpellRuneEnable(id) then
		local nType = parent:GetClientID()
		local t_color = { "#cff0000红色", "#c00c8ff蓝色", "#c00ff00绿色" };
		local itemid = SkillMgr:getSpellRuneItemId(id, nType)
		local level	= SkillMgr:getSpellLv(id)
		local SkillDef = SkillMgr:getSpellStaticAtt( id, level )
		if bitAnd( SkillDef.CtrlFlag, SKILL_FLAG_SPE_RUNE ) then			
			if itemid == 0 then
				local t_color = { "#cff0000猩红符文·", "#c00c8ff海洋符文·", "#c00ff00翡翠符文·" };
				SetGameTooltips(name, "左键拖拽" .. t_color[nType]..SkillDef.Name.. "#cffffbe，可以激活该符文技能")
			else
				SetGameTooltips( name, "符文技能已激活，点击“#c00c8ff使用技能#cffffbe”按钮可切换使用该技能" );
			end	
		else
			if itemid == 0 then
				SetGameTooltips(name, "左键拖拽" .. t_color[nType] .. "#cffffbe符文，可以激活该符文技能")			
			else
				SetGameTooltips( name, "符文技能已激活，点击“#c00c8ff使用技能#cffffbe”按钮可切换使用该技能" );
			end
		end	
	end
end

function SkillRuneExtendEntry_OnLeave()
	util.HightParent(this, false)
	local highlight = getglobal(this:GetName().."Highlight")
	if highlight then
		highlight:Hide()
	end
	ItemButton_OnLeave()
	GameTooltipRichText:SetText("", 255, 255, 255)
	GameTooltip:Hide()
end

function SkillRuneExtendEntry_ReceiveDrag()
	if (UIMSG:getUIGV().DragFrom == CONTAINER_TYPE_SUNDRIES) then
		local MainPlayer = ActorMgr:getMainPlayer()
		if not MainPlayer then
			return
		end
		local parent = getglobal(this:GetParent())
		local id = parent:GetClientUserData(0)
		if not RuneMgr:CanLock(id) then
			return
		end
		local container = MainPlayer:getContainer()
		local item	= container:getItem(UIMSG:getUIGV().DragFrom, UIGetDragUserData1())
		if not item then
			return
		end
		if not RuneMgr:IsRuneItem(item:getItemId()) then
			util.PlaySound("fail")
			return ShowMidTips("非符文物品不能镶嵌")
		end
		local type = parent:GetClientID()
		local level	= SkillMgr:getSpellLv(id)
		local SkillDef = SkillMgr:getSpellStaticAtt( id, level )
		local _,_,itemForSKillID = getItemResultValues(item:getItemDef(), RESULT_RUNE )
		if bitAnd( SkillDef.CtrlFlag, SKILL_FLAG_SPE_RUNE ) and  itemForSKillID ~= id  then
			util.PlaySound("fail")
			local t_color = { "#cff0000猩红符文·", "#c00c8ff海洋符文·", "#c00ff00翡翠符文·" };
			return ShowMidTips("该符文技能只能通过镶嵌"..t_color[type]..SkillDef.Name.."#n激活")
		end
		if itemForSKillID ~= 0 and itemForSKillID ~= id then
			local colorType = RuneMgr:RuneItemType(item:getItemId())
			local t_color = { "#cff0000", "#c00c8ff", "#c00ff00" };
			--先获取直接关联技能
			local itemForSkilldef = SkillMgr:getSpellStaticAtt( itemForSKillID, 1 )
			--再通过颜色获取实际需要镶嵌到的技能id
			local referID = 0
			if colorType == 1 then
				referID = itemForSkilldef.RedRuneTransferedID
			elseif colorType == 2 then
				referID = itemForSkilldef.BlueRuneTransferedID
			elseif colorType == 3 then
				referID = itemForSkilldef.YellowRuneTransferedID
			end
			local referSkillDef = SkillMgr:getSpellStaticAtt( referID, 1 )
			return ShowMidTips(item:getItemDef().Name.."#n 只能镶嵌到 "..t_color[colorType]..referSkillDef.Name.."#n 技能上")
		end
		if RuneMgr:RuneItemType(item:getItemId()) ~= type then
			util.PlaySound("fail")
			if bitAnd( SkillDef.CtrlFlag, SKILL_FLAG_SPE_RUNE )  then
				local t_color = { "#cff0000猩红符文·", "#c00c8ff海洋符文·", "#c00ff00翡翠符文·" };
				return ShowMidTips("该符文技能只能通过镶嵌"..t_color[type]..SkillDef.Name.."#n激活")
			else
				return ShowMidTips("不能镶嵌该类符文物品")
			end
		end
		UIEndDrag()
		UIMSG:getUIGV().DragFrom = DRAG_NONE
		if not SkillMgr:getSpellRuneEnable(id) then
			return ShowMidTips("符文还未解锁")
		end
		if MainPlayer:isDead() then
			return ShowMidTips("死亡状态下无法执行该操作")
		end
		if MainPlayer:isInFight() then
			return ShowMidTips("战斗状态下无法执行该操作")
		end
		if RuneMgr:RuneItemId(id, type) == 0 then
			MessageBox("确认","是否镶嵌符文？")
		else
			MessageBox("确认","是否替换当前符文？")
		end		

		MessageBoxFrame:SetClientString("符文镶嵌");
		function RuneInsert()
			RuneMgr:Insert(id, item)
			resultBtn = parent;
		end
	end
end

function SkillRuneExtendEntrySkill_OnEnter()
	local parent = getglobal(this:GetParent())
	local id = parent:GetClientUserData(0)
	local color = parent:GetClientID()
	if color ~= SkillMgr:getSpellRuneType(id) then
		id = SkillMgr:getSpkillRuneSkillId(id, color)
	end
	if id ~= 0 then
		SpellTipsFrame:SetClientUserData(0, id)
		local attr = SkillMgr:getSkillDyAttrib( parent:GetClientUserData(0) )
		SpellTipsFrame:SetClientUserData(1, attr.SkillLevel)
		SpellTipsFrame:SetClientUserData(2, SKILL_TYPE)
		SpellTipsFrame:SetClientUserData(3, RUNE_SKILL_TYPE)
		SpellTipsFrame:SetClientString(this:GetName())
		SpellTipsFrame:Show()
	end
end

function SkillRuneExtendEntrySkill_OnLeave()
	SpellTipsFrame:Hide()
end

function SkillRuneExtendEntrySkill_OnClick()
	local parent = getglobal(this:GetParent())
	local id = parent:GetClientUserData(0)
	local color = parent:GetClientID()
	if color == SkillMgr:getSpellRuneType(id) then
		RuneMgr:SelectRuneSkill(id, 0)
	else
		RuneMgr:SelectRuneSkill(id, color)
	end	
end

local MAX_LIU_GUANG_EFFECT_TIME = 0.9;
local t_RuneUVAnimationTexControl = { startTime = 0, };
local isStart = false;

t_RuneUVAnimationTexControl["start"] = 
function( self )
	self["startTime"] = os.clock();
end

t_RuneUVAnimationTexControl["isEnd"] = 
function( self )
	return ( os.clock() - self["startTime"] ) > MAX_LIU_GUANG_EFFECT_TIME;
end

function RuneChgUVAnimationTex_OnUpdate()
	if isStart and t_RuneUVAnimationTexControl:isEnd() then		
		HideRuneSkillChgUV();
	end
end

function ShowRuneSkillChgUV( nSkillID, nColor )	
	HideRuneSkillChgUV();
	for i = 1, MAX_SKILL_PAGE do
		local btn = getglobal("SkillFrameSkill"..i.."Item");
		if btn:GetClientUserData(0) == nSkillID then
			local uvTex = getglobal( btn:GetName() .. "UVAnimationTex" .. nColor );
			uvTex:SetUVAnimation( 75, true );
			uvTex:Show();
			t_RuneUVAnimationTexControl:start();
			isStart = true;
		end		
	end
	CheckSkillForAutoAssist( nSkillID );
end

function HideRuneSkillChgUV()
	for i = 1, MAX_SKILL_PAGE do
		local btn = getglobal("SkillFrameSkill"..i.."Item");
		for j=0,3 do
			local uvTex = getglobal( btn:GetName() .. "UVAnimationTex" .. j );
			uvTex:SetUVAnimation( 75, false );
			uvTex:Hide();
		end
	end
	isStart = false;
end

function RuneErase()
	
end

local function ResetSkillExRuneInfo(color)
	if SkillRuneResetFrame:GetClientUserData(0) ~= color or not SkillRuneResetFrame:IsShown() then
		SkillRuneResetFrame:SetClientUserData(0, color)
		InitSkillRuneResetFrame(color)
	end
	SkillRuneResetFrame:Show()
end

local function ShowSkillExRuneInfoTips(color)
	local info = RuneMgr:GetExRuneInfo(color)
	if info and info.attr ~= 0 then
		SetGameTooltips(this:GetName(), RuneMgr:GetRuneAddAttrDesc(info.attr))
	end
end

function SkillFrameExRuneInfoReset_OnEnter()
	local parent = getglobal(this:GetParent())
	local color = parent:GetClientID()
	local info = RuneMgr:GetExRuneInfo(color)
	if info then
		if info.attr ~= 0 then
			SetGameTooltips(this:GetName(), "点击重置该符文之力属性")
		else
			SetGameTooltips(this:GetName(), string.format("镶嵌%d个%s可激活额外的\n#c0ad700符文之力#cffffbe属性", info.total, getItemDef(SkillRuneItemIdByLevel[1][color]).Name))
		end
	end
end

function SkillFrameExRuneInfoReset_OnLeave()
	GameTooltipRichText:SetText("", 255, 255, 255)
	GameTooltip:Hide()
end

function SkillFrameExRuneInfoReset_OnClick()
	--[[
	local parent = getglobal(this:GetParent())
	local color = parent:GetClientID();
	local info = RuneMgr:GetExRuneInfo(color)
	if info.num ~= 0 then
		ResetSkillExRuneInfo(color);	
	end
	--]]
end

function SkillFrameExRuneInfo_OnEnter()
	local parent = getglobal(this:GetParent())
	ShowSkillExRuneInfoTips(parent:GetClientID())
end

function SkillFrameExRuneInfo_OnLeave()
	GameTooltipRichText:SetText("", 255, 255, 255)
	GameTooltip:Hide()
end

function SkillRuneExtendEntrySkillSelect_OnEnter()
	SetGameTooltips(this:GetName(), "点击可切换至该技能")
end

function SkillRuneExtendEntrySkillSelect_OnLeave()
	GameTooltipRichText:SetText("", 255, 255, 255)
	GameTooltip:Hide()
end

function canSkillUp()
	local MainPlayer = ActorMgr:getMainPlayer();
	if not MainPlayer then
		return false;
	end

	local skillList = GetAllSkillList();
	for i=1, #skillList do
		local id = skillList[i];
		local lv = SkillMgr:getSpellLv(id);
		if id ~= 0 and lv ~= 0 then
			local SkillDef = SkillMgr:getSpellStaticAtt( id, lv );
			if SkillDef.NextLevelDesc ~= "" then
				SkillDef = SkillMgr:getSpellStaticAtt( id, lv + 1 );
				if SkillDef.SkillID ~= 0 and SkillDef.Learn.LevelMin <= MainPlayer:getLv() then
					return true;
				end
			end
		end
	end

	return false;
end

function countRuneSlot()
	local skillList = GetAllSkillList();
	local num = 0;
	for i=1, #skillList do
		local id = skillList[i];
		if id ~= 0 and RuneMgr:CanLock(id) and SkillMgr:isSkillLearned(id) and 
				not SkillMgr:getSpellRuneEnable(id) then
			num = num + 1;
		end
	end
	return num;
end

local oldNum = 0;
function needShowSkillTips()
	if lockSkillNum ~= 0 then
		oldNum = lockSkillNum;
		lockSkillNum = 0;
	end

	local bShow = false;
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil or MainPlayer:getLv() < 10 then
		return;
	end

	if MainPlayer:getLv() >= RUNE_OPEN_LV then
		local num = countRuneSlot();

		if oldNum < num then
			local szText		= "当前有新的符文槽可以解锁,\n点击查看，\n快捷键（K）";
			local xinShouBtn	= getglobal( "SkillButtonXinShouBtn" );
			local rich			= getglobal( xinShouBtn:GetName().."Rich" );
			rich:SetClientString( szText );
			local nSelfDefineExtendWidth = 14;
			ReSizeTooltip( rich:GetName(), xinShouBtn:GetName(), nSelfDefineExtendWidth );
			xinShouBtn:Show();
			SkillButtonUVAnimationTex:SetUVAnimation(50, true);
			local t_CharacterButtonControl = GetCharacterButtonControl();
			t_CharacterButtonControl:startShowLevelUpAnim();
			bShow = true;
		end
		oldNum = num;
	end

	if not bShow and canSkillUp() then
		SkillButtonUVAnimationTex:SetUVAnimation(50, true);
		local t_CharacterButtonControl = GetCharacterButtonControl();
		t_CharacterButtonControl:startShowLevelUpAnim();
	end
end

--------------------------------------------属性重置--------------------------------------------------------
local ResetTime
local ResetEffectDuring = 0.5

local function UpdateSkillRuneResetItem()
	local MainPlayer	=	ActorMgr:getMainPlayer()
	local container		=	MainPlayer:getContainer()
	SkillRuneResetFrameReset:Enable()
	if getPlayerMoney( MainPlayer ) < RESET_RUNE_COST_MONEY then
		SkillRuneResetFrameReset:Disable()
		SkillRuneResetFrameMoney:SetTextColor(255, 0, 0)
	else
		SkillRuneResetFrameMoney:SetTextColor(255, 254, 189)
	end
	local haslock = IsItemLock(0, 0, SkillRuneImportantItemId)
	for _, nId in ipairs(SkillRuneItemIdByLevel[2]) do
		if haslock then
			break
		end
		haslock = IsItemLock(0, 0, nId)
	end
	if haslock then
		SkillRuneResetFrameReset:Disable()
		return
	end
	local color = SkillRuneResetFrame:GetClientUserData(0)
	local info = RuneMgr:GetExRuneInfo(color)
	if info and info.num >= info.total then
		if container:getItemCount(SkillRuneImportantItemId) == 0 then
			SkillRuneResetFrameReset:Disable()
			SkillRuneResetFrameItem0Icon:Hide()
			SkillRuneResetFrameItem0:SetClientUserData(0, 0)
		else
			SkillRuneResetFrameItem0Icon:Show()
			SkillRuneResetFrameItem0Icon:SetTexture(GetItemIconPathByID(SkillRuneImportantItemId))
			SkillRuneResetFrameItem0:SetClientUserData(0, SkillRuneImportantItemId)
		end
		for i = 1, 3 do
			local btn = getglobal("SkillRuneResetFrameItem"..i)
			local ui = getglobal(string.format("SkillRuneResetFrameItem%dIcon", i))
			local id = SkillRuneItemIdByLevel[2][i]
			if container:getItemCount(id) == 0 then
				SkillRuneResetFrameReset:Disable()
				ui:Hide()
				btn:SetClientUserData(0, 0)
			else
				ui:Show()
				ui:SetTexture(GetItemIconPathByID(id))
				btn:SetClientUserData(0, id)
			end
		end
	else
		for i = 0, 3 do
			local btn = getglobal("SkillRuneResetFrameItem"..i)
			local ui = getglobal(string.format("SkillRuneResetFrameItem%dIcon", i))
			ui:Hide()
			btn:SetClientUserData(0, 0)
		end
		SkillRuneResetFrameReset:Disable()
	end
	for i = 0, container:getContainerMaxNum(CONTAINER_TYPE_SUNDRIES) do
		local item = container:getItem(CONTAINER_TYPE_SUNDRIES, i)
		local id = item:getItemId()
		local conform = (id == SkillRuneImportantItemId)
		for _, nId in ipairs(SkillRuneItemIdByLevel[2]) do
			if conform then
				break
			end
			conform = (id == nId)
		end
		if conform then
			LockItem(CONTAINER_TYPE_SUNDRIES, i, id, "SkillRuneResetFrame")
		end
	end
end

local function UpdateSkillRuneResetTipDesc()
	local text = {}
	table.insert(text, "#cFFFFBE推荐属性如下#n")
	local types = {}
	local attrs = {}
	for id in EnumSkillRuneAddAttr() do
		local r = RuneMgr:GetRuneAddAttrRecommend(id)
		if SkillRuneRecommend[r] then
			if not attrs[r] then
				attrs[r] = {}
				table.insert(types, r)
			end
			table.insert(attrs[r], id)
		end
	end
	table.sort(types)
	for _, r in ipairs(types) do
		table.insert(text, SkillRuneRecommend[r])
		for _, attr in ipairs(attrs[r]) do
			table.insert(text, string.format("\t%s\t%s", RuneMgr:GetRuneAddAttrName(attr), RuneMgr:GetRuneAddAttrDesc(attr)))
		end
	end
	SkillRuneResetFrameTipDesc:SetText(table.concat(text, "\n"), 160, 160, 90)
end

function SkillRuneResetFrame_OnLoad()
	this:RegisterEvent("GE_SKILL_RUNE_EX_INFO_RESET")
	this:RegisterEvent("GE_CONTAINER_UPDATE")
	this:RegisterEvent("GE_CHANGE_MONEY")
	SkillRuneResetFrameRuneIcon:setMask("uires\\ui\\mask\\Round.tga")
	SkillRuneResetFrameMoney:SetText(RESET_RUNE_COST_MONEY)
	UpdateSkillRuneResetTipDesc()
	this:setUpdateTime(0)
end

function SkillRuneResetFrame_OnEvent()
	if arg1 == "GE_SKILL_RUNE_EX_INFO_RESET" then
		if SkillRuneResetFrame:IsShown() then
			local msg = UIMSG:getUIMsg()
			local color = SkillRuneResetFrame:GetClientUserData(0)
			if msg.RuneResetData.RuneType == color then
				local info = RuneMgr:GetExRuneInfo(color)
				if info then
					local oldattr = info.attr
					local newattr = msg.RuneResetData.RuneExID
					local name = string.format("原有属性：[%s]%s\n#cC0FFC0重置属性：[%s]%s#n",
						RuneMgr:GetRuneAddAttrName(oldattr), RuneMgr:GetRuneAddAttrDesc(oldattr),
						RuneMgr:GetRuneAddAttrName(newattr), RuneMgr:GetRuneAddAttrDesc(newattr))
					SkillRuneResetFrameConfirmText:SetText(name, 255, 255, 190)
					local height = SkillRuneResetFrameConfirmText:GetTotalHeight() / GetScreenScaleY()
					SkillRuneResetFrameConfirmText:resizeRichHeight(height)
					SkillRuneResetFrameConfirmText:SetHeight(height)
					SkillRuneResetFrameConfirmText:SetText(name, 255, 255, 190)
					SkillRuneResetFrameConfirm:SetSize(279, 30 + height)
					SkillRuneResetFrameConfirm:Show()
				end
			end
		end
	elseif arg1 == "GE_CONTAINER_UPDATE" or arg1 == "GE_CHANGE_MONEY" then
		if SkillRuneResetFrame:IsShown() then
			UpdateSkillRuneResetItem()
		end
	end
end

function InitSkillRuneResetFrame(color)
	SkillRuneResetFrameRuneIcon:SetTexture(SkillRuneIcon[color])
	SkillRuneResetFrameConfirm:Hide()
	SkillRuneResetFrameShadeEffect:Hide()
	SkillRuneResetFrameReset:DisChecked()
	ResetTime = nil
	UpdateSkillRuneResetItem()
end

function SkillRuneResetFrame_OnShow()
	if not PlayerContainerFrame:IsShown() then
		PlayerContainerFrame:Show()
	end
	local color = SkillRuneResetFrame:GetClientUserData(0)
	local info = RuneMgr:GetExRuneInfo(color)
	if info.num == info.total and not GameWizard:isDo( GAMEWIZARD_FIRST_OPEN_RUNE_RESET ) then
		local t_RuneName = { "符文之力", "猩红符文", "海洋符文", "翡翠符文" };
		for i=0, 3 do
			local szText = "请放入" .. t_RuneName[i + 1];
			local xinShouBtn = getglobal( "SkillRuneResetFrameXinShouBtn" .. i );
			local rich = getglobal( xinShouBtn:GetName() .. "Rich" );
			rich:SetClientString( szText );
			ReSizeTooltip( rich:GetName(), xinShouBtn:GetName(), 14 );
			xinShouBtn:Show();
		end
	end
	PlayerContainerFrame:SetPoint("topleft", "UIClient", "topleft", 453, 163)
	this:SetPoint("topright", "UIClient", "topleft", 453, 163)
	SkillRuneResetFrame_OnUpdate()
end

function HideSkillResetXinShouBtn()
	local btn = getglobal( this:GetParent() );
	btn:Hide();
end

function SkillRuneResetFrame_OnHide()
	ClearItemLock("SkillRuneResetFrame")
	if not GameWizard:isDo( GAMEWIZARD_FIRST_OPEN_RUNE_RESET ) then
		GameWizard:doStep( GAMEWIZARD_FIRST_OPEN_RUNE_RESET );
		for i=0, 3 do
			local xinShouBtn = getglobal( "SkillRuneResetFrameXinShouBtn" .. i );
			xinShouBtn:Hide();
		end
	end
end

function SkillRuneResetFrame_OnUpdate()
	local player = ActorMgr:getMainPlayer()
	if not player or player:isDead() then
		this:Hide()
		return
	end
	if not ResetTime then
		return
	end
	local during = os.clock() - ResetTime
	if during >= ResetEffectDuring then
		SkillRuneResetFrameShadeEffect:Hide()
		SkillRuneResetFrameReset:DisChecked()
		ResetTime = nil
		util.PlaySound("succeed")
		RuneMgr:ResetExInfoNormal(SkillRuneResetFrame:GetClientUserData(0))
	else
		SkillRuneResetFrameShadeEffect:SetBlendAlpha(1- during / ResetEffectDuring)
	end
end

function SkillRuneResetFrame_OnReset()
	if SkillRuneResetFrameReset:IsChecked() then
		return
	end
	ResetTime = os.clock()
	SkillRuneResetFrameReset:Checked()
	SkillRuneResetFrameShadeEffect:Show()
	SkillRuneResetFrameShadeEffect:SetBlendAlpha(1)
end

function SkillRuneResetFrameConfirm_OnSave()
	SkillRuneResetFrameConfirm:Hide()
	RuneMgr:ResetExInfoSave(SkillRuneResetFrame:GetClientUserData(0))
end

function SkillRuneResetFrameConfirm_OnCancel()
	SkillRuneResetFrameConfirm:Hide()
end

function SkillRuneResetFrameItem_OnEnter()
	local id = this:GetClientUserData(0)
	if id == 0 then
		id = this:GetClientID()
		if id == 0 then
			id = SkillRuneImportantItemId
		else
			id = SkillRuneItemIdByLevel[2][id]
		end
		SetGameTooltips(this:GetName(), string.format("请放入%s", getItemDef(id).Name))
	else
		ShowItemTips(LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, getItemDef(id))
	end
end

function SkillRuneResetFrameItem_OnLeave()
	ItemButton_OnLeave()
end

--------------------------------------------符文合成--------------------------------------------------------
local CombineId
local CombineItem

local function UpdateSkillRuneCombineTipDesc()
	SkillRuneCombineFrameTipDesc:SetText("#cFFFFBE符文合成#n\n    ●5个红色符文有几率合成1个猩红符文\n    ●5个蓝色符文有几率合成1个海洋符文#n\n    ●5个绿色符文有几率合成1个翡翠符文\n#cFFFFBE说明\n#ca0a05a    符文合成失败后，会保留2-4个低级符文",
		160, 160, 90)
end

local function UpdateSkillRuneCombineItem(item)
	local MainPlayer	=	ActorMgr:getMainPlayer()
	local container		=	MainPlayer:getContainer()
	local id 
	if item == nil then
		id = 0;
	else
		id = item:getItemId();
		--比较是否与目前栏位的符文是同ID，不是则清除原ID的符文，放入此ID的符文
		for i  = 1, 5 do
			local btn = getglobal("SkillRuneCombineFrameItem"..i)
			local btnItem = container:getItem( btn:GetClientUserData(0), btn:GetClientUserData(1) )
			if not(btnItem == nil or btnItem:getItemId() == 0) then
				if not ( btnItem:getItemId() == item:getItemId() ) then
					RemoveSkillRuneCombineItem( i );
				end
			end
		end
	end
	ClearItemLock("SkillRuneCombineFrame")
	SkillRuneCombineFrameCombine:Enable()
	local num
	if not id or id == 0 then
		num = 0
	else
		num = item:getNum();
	end
	local hasItemNum = 0;
	for i  = 1, 5 do
		local btn = getglobal("SkillRuneCombineFrameItem"..i)
		local btnItem = container:getItem( btn:GetClientUserData(0), btn:GetClientUserData(1) )
		if not(btnItem == nil or btnItem:getItemId() == 0) then
			hasItemNum = hasItemNum + 1;
			LockItem( btnItem:getListType(), btnItem:getGridIdx(), btnItem:getItemId(), "SkillRuneCombineFrame" );
		end
	end
	for i = hasItemNum + 1, (num + hasItemNum >= 5 and 5 or num + hasItemNum ) do
		--寻找一个空位置来放置
		for j = 1, 5 do
			local btn = getglobal("SkillRuneCombineFrameItem"..j)
			if btn:GetClientUserData(0) == 0 and btn:GetClientUserData(1) == 0 then
				local ui = getglobal(string.format("SkillRuneCombineFrameItem%dIcon", j))
				ui:Show()
				ui:SetTexture(GetItemIconPathByID(id))
				btn:SetClientUserData(0, item:getListType());
				btn:SetClientUserData(1, item:getGridIdx());
				hasItemNum = hasItemNum + 1;
				LockItem( item:getListType(), item:getGridIdx(), item:getItemId(), "SkillRuneCombineFrame" );
				break;
			end
		end
	end
	if hasItemNum < 5 then
		SkillRuneCombineFrameCombine:Disable()
		for i = 1, 5 do
			local btn = getglobal("SkillRuneCombineFrameItem"..i)
			if btn:GetClientUserData(0) == 0 and btn:GetClientUserData(1) == 0 then
				local ui = getglobal(string.format("SkillRuneCombineFrameItem%dIcon", i))
				ui:Hide()
			end
		end
	end
	local mItem = container:getItem( SkillRuneCombineFrame:GetClientUserData(0), SkillRuneCombineFrame:GetClientUserData(1) )
	if not( mItem:getItemId() == 0 or mItem == nil )then
		LockItem( mItem:getListType(), mItem:getGridIdx(), mItem:getItemId(), "SkillRuneCombineFrame" );
	end
	if hasItemNum < 5 then
		SkillRuneCombineFrameCombine:Disable()
	end
	local srcItemID = 0;
	for i = 1, 5 do
		local btn = getglobal("SkillRuneCombineFrameItem"..i)
		local btnItem = container:getItem( btn:GetClientUserData(0), btn:GetClientUserData(1) )
		if not(btnItem == nil or btnItem:getItemId() == 0) then
			srcItemID = btnItem:getItemId();
		end
	end
	if srcItemID ~= 0 then
		SkillRuneCombineFrameItem0Icon:Show()
		local result = SkillRuneItemIdByLevel[SkillRuneItemId[srcItemID].level + 1][SkillRuneItemId[srcItemID].color]
		SkillRuneCombineFrameItem0Icon:SetTexture(GetItemIconPathByID(result))
		SkillRuneCombineFrameItem0Icon:SetClientUserData(0, result)
		SkillRuneCombineFrameItem0Icon:SetClientUserData(1, result)
	elseif  srcItemID == 0 and not SkillRuneCombineFrameClose:IsShown() then
		SkillRuneCombineFrameItem0Icon:Hide()
		SkillRuneCombineFrameItem0Icon:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
		SkillRuneCombineFrameItem0Icon:SetClientUserData( 0, 0 );
	end
	if SkillRuneCombineFrameItem0Icon:GetClientUserData(0) == 0 and not SkillRuneCombineFrameClose:IsShown() then
		SkillRuneCombineFrameItem0Icon:Hide();
		SkillRuneCombineFrameItem0Icon:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
		SkillRuneCombineFrameItem0Icon:SetClientUserData( 0, 0 );
	end
	if SkillRuneCombineFrameClose:IsShown() and srcItemID == 0 then
		SkillRuneCombineFrameItem0Icon:Show()
		local oldResult = SkillRuneCombineFrameItem0Icon:GetClientUserData(1)
		SkillRuneCombineFrameItem0Icon:SetTexture(GetItemIconPathByID(oldResult))
		SkillRuneCombineFrameItem0Icon:SetClientUserData(0, oldResult)
	end
	--这里处理失败后的显示
	for j = 1, 5 do
		local btn = getglobal("SkillRuneCombineFrameItem"..j)
		if btn:GetClientUserData(2) ~= 0 and SkillRuneCombineFrameClose:IsShown() then
			local ui = getglobal(string.format("SkillRuneCombineFrameItem%dIcon", j))
			ui:Show()
			ui:SetTexture(GetItemIconPathByID(btn:GetClientUserData(2)));
		end
	end	
end

local function ClearSkillRuneCombineItem()
	CombineLevel = nil
	for i = 1, 5 do
		local btn = getglobal("SkillRuneCombineFrameItem"..i)
		local ui = getglobal(string.format("SkillRuneCombineFrameItem%dIcon", i))
		ui:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga")
		btn:SetClientUserData(0, 0)
		btn:SetClientUserData(1, 0);
		btn:SetClientUserData(2, 0);
	end
end

function SkillRuneCombineFrame_OnLoad()
	this:RegisterEvent("GE_CONTAINER_UPDATE")
	this:setUpdateTime(0)
	UpdateSkillRuneCombineTipDesc()
end

function SkillRuneCombineFrame_OnShow()
	if not PlayerContainerFrame:IsShown() then
		PlayerContainerFrame:Show()
	end
	PlayerContainerFrame:SetPoint("topleft", "UIClient", "topleft", 453, 163)
	this:SetPoint("topright", "UIClient", "topleft", 453, 163)
	SkillRuneCombineFrameModelView:Hide()
	if CombineLevel then
		if CombineLevel == 0 then
			SkillRuneCombineFrameTitle:SetText("低级符文合成")
		elseif CombineLevel == 1 then
			SkillRuneCombineFrameTitle:SetText("高级符文合成")
		else
			SkillRuneCombineFrameTitle:SetText("符文合成")
		end
	else
		SkillRuneCombineFrameTitle:SetText("符文合成")
	end
	ClearSkillRuneCombineItem();
	UpdateSkillRuneCombineItem()
	SkillRuneCombineFrame_OnUpdate()
	SkillRuneCombineFrameItem0Icon:SetGray( true );
	SkillRuneCombineFrameItem0Icon:Hide();
	SkillRuneCombineFrameItem0Icon:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
	SkillRuneCombineFrameItem0Icon:SetClientUserData( 0, 0 );
	SkillRuneCombineFrameItem0Icon:SetClientUserData(1, 0)
	SkillRuneCombineFrameCombine:DisChecked();
	SkillRuneCombineFrameClose:Hide();
end

function SkillRuneCombineFrame_OnHide()
	if MessageBoxFrame:IsShown() and MessageTitle:GetText() == "符文合成绑定提示" then
		MessageBoxFrame:Hide();
	end
	ClearSkillRuneCombineItem()
	ClearItemLock("SkillRuneCombineFrame")
	SkillRuneCombineFrame:SetClientUserData( 0, 0 );
	SkillRuneCombineFrame:SetClientUserData( 1, 0 );
	SkillRuneCombineFrameItem0Icon:SetClientUserData(0, 0)
	SkillRuneCombineFrameItem0Icon:SetClientUserData(1, 0)
end

function SkillRuneCombineFrame_OnEvent()
	if arg1 == "GE_CONTAINER_UPDATE" then
		if SkillRuneCombineFrame:IsShown() then
			UpdateSkillRuneCombineItem()
		end
	end
end

function SkillRuneCombineFrame_OnUpdate()
	local player = ActorMgr:getMainPlayer()
	if not player or player:isDead() then
		this:Hide()
		return
	end
end

function SkillRuneCombineFrame_OnClose()
	SkillRuneCombineFrame:Hide();
end

function SkillRuneCombineFrame_OnCombine()
	local hasItemBind = false;
	--合成符
	local mainplayer = ActorMgr:getMainPlayer();
	local container = mainplayer:getContainer();
	local heChengItem = container:getItem( SkillRuneCombineFrame:GetClientUserData(0), SkillRuneCombineFrame:GetClientUserData(1) )
	if heChengItem:isItemBind() then
		hasItemBind = true;
	end
	--合成材料列表
	local t_Item = {};
	local id;
	for i = 1, 5 do
		local btn = getglobal("SkillRuneCombineFrameItem"..i)
		local btnItem = container:getItem( btn:GetClientUserData(0), btn:GetClientUserData(1) )
		if not(btnItem == nil or btnItem:getItemId() == 0) then
			table.insert( t_Item, btnItem );
			id = btnItem:getItemId();
			if btnItem:isItemBind() then
				hasItemBind = true;
			end
		end
	end
	if hasItemBind then
		MessageBox( "符文合成绑定提示", "使用绑定的符文或合成卷轴会使合成的符文绑定，确定继续合成？" );
		return;
	end
	if SkillRuneCombineFrameCombine:IsChecked() then
		return
	end
	SkillRuneCombineFrameCombine:Checked()
	RuneMgr:CombineRuneItem( heChengItem, t_Item[1], t_Item[2], t_Item[3], t_Item[4], t_Item[5] );
end

function SkillRuneCombineResult( flag, returnNum )
	SkillRuneCombineFrameCombine:Disable()
	SkillRuneCombineFrameClose:Show();
	SkillRuneCombineFrameItem0Icon:Show()
	if not SkillRuneCombineFrameModelView:IsShown() then
		SkillRuneCombineFrameModelView:Show()
	end
	local mainplayer = ActorMgr:getMainPlayer();
	local container = mainplayer:getContainer();
	if flag == 1 then
		SkillRuneCombineFrameItem0Icon:SetGray( false );
		SkillRuneCombineFrameModelView:setPetModel(0, 0, [[effect\entity\bshc01.ent]])
		SkillRuneCombineFrameModelView:setEntityPosition(0, 0, 100, 0)
		ShowMidTips( "恭喜您，符文合成成功" );
		ClearSkillRuneCombineItem();
	else
		SkillRuneCombineFrameItem0Icon:SetGray( true );
		SkillRuneCombineFrameModelView:setPetModel(0, 0, [[effect\entity\bshc02.ent]])
		SkillRuneCombineFrameModelView:setEntityPosition(0, 0, 100, 0)
		SkillRuneCombineFrameItem0Icon:Hide();
		SkillRuneCombineFrameItem0Icon:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
		SkillRuneCombineFrameItem0Icon:SetClientUserData( 0, 0 );
		ShowMidTips( "很遗憾，符文合成失败" );
		--失败后根据服务器返回数据，随机设置在目前的格子里
		local srcItemID = 0;
		for i = 1, 5 do
			local btn = getglobal("SkillRuneCombineFrameItem"..i)
			local btnItem = container:getItem( btn:GetClientUserData(0), btn:GetClientUserData(1) )
			if not(btnItem == nil or btnItem:getItemId() == 0) then
				srcItemID = btnItem:getItemId();
			end
		end
		ClearSkillRuneCombineItem();
		for i = 1, (5 - returnNum) do
			local findOneIndex = false;
			while not findOneIndex do
				local index = math.random( 1, 5 );
				local btn = getglobal("SkillRuneCombineFrameItem"..index)
				if btn:GetClientUserData(2) == 0 then
					 findOneIndex = true
					 btn:SetClientUserData( 2 , srcItemID );
					local ui = getglobal(string.format("SkillRuneCombineFrameItem%dIcon", index))
					ui:Show()
					ui:SetTexture(GetItemIconPathByID(btn:GetClientUserData(2)));
				end
			end
		end
	end
	UpdateSkillRuneCombineItem()
	SkillRuneCombineFrameCombine:DisChecked()
end

function HeChengBindOk()
	--合成符
	local mainplayer = ActorMgr:getMainPlayer();
	local container = mainplayer:getContainer();
	local heChengItem = container:getItem( SkillRuneCombineFrame:GetClientUserData(0), SkillRuneCombineFrame:GetClientUserData(1) )
	if heChengItem:isItemBind() then
		hasItemBind = true;
	end
	--合成材料列表
	local t_Item = {};
	local id;
	for i = 1, 5 do
		local btn = getglobal("SkillRuneCombineFrameItem"..i)
		local btnItem = container:getItem( btn:GetClientUserData(0), btn:GetClientUserData(1) )
		if not(btnItem == nil or btnItem:getItemId() == 0) then
			table.insert( t_Item, btnItem );
			id = btnItem:getItemId();
			if btnItem:isItemBind() then
				hasItemBind = true;
			end
		end
	end
	if SkillRuneCombineFrameCombine:IsChecked() then
		return
	end
	SkillRuneCombineFrameCombine:Checked()
	UpdateSkillRuneCombineItem()
	RuneMgr:CombineRuneItem( heChengItem, t_Item[1], t_Item[2], t_Item[3], t_Item[4], t_Item[5] );
end

function SkillRuneCombineFrameItem_OnEnter()
	local mainplayer = ActorMgr:getMainPlayer();
	local container = mainplayer:getContainer();
	local item = container:getItem( this:GetClientUserData( 0 ), this:GetClientUserData( 1 ) );
	local id = item:getItemId();
	if this:GetClientID() == 0 then
		local icon = getglobal( this:GetName().."Icon" )
		id = icon:GetClientUserData(0);
	end
	if id == 0 and not SkillRuneCombineFrameClose:IsShown() then
		if this:GetClientID() ~= 0 and not SkillRuneCombineFrameClose:IsShown() then
			SetGameTooltips( this:GetName(), "请放入小型符文" );
		end
		return
	end
	if SkillRuneCombineFrameClose:IsShown() and this:GetClientID() ~= 0 then
		id = this:GetClientUserData( 2 );
		if id == 0 then return end
	end
	ShowItemTips(LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, getItemDef(id))
end

function SkillRuneCombineFrameItem_OnLeave()
	ItemButton_OnLeave()
end

function RemoveSkillRuneCombineItem( index )
	if SkillRuneCombineFrameClose:IsShown() then return end
	local Btn = getglobal( "SkillRuneCombineFrameItem"..index )
	local listType = Btn:GetClientUserData(0);
	local gridIdx = Btn:GetClientUserData(1);
	--确定来自相同位置的符文，将其清除
	for i = 1, 5 do
		local btn = getglobal("SkillRuneCombineFrameItem"..i)
		if btn:GetClientUserData( 0 ) == listType and btn:GetClientUserData( 1 ) == gridIdx then
			print( btn:GetClientUserData( 0 ), btn:GetClientUserData( 1 ) )
			local ui = getglobal(string.format("SkillRuneCombineFrameItem%dIcon", i))
			ui:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga")
			btn:SetClientUserData(0, 0)
			btn:SetClientUserData(1, 0);
		end			
	end
	UpdateSkillRuneCombineItem();
end

function SkillRuneCombineFrameItem_OnClick()
	if MessageBoxFrame:IsShown() and MessageTitle:GetText() == "符文合成绑定提示" then
		return;
	end
	if arg1 == "RightButton" then
		local listType = this:GetClientUserData(0);
		local gridIdx = this:GetClientUserData(1);
		--确定来自相同位置的符文，将其清除
		for i = 1, 5 do
			local btn = getglobal("SkillRuneCombineFrameItem"..i)
			if btn:GetClientUserData( 0 ) == listType and btn:GetClientUserData( 1 ) == gridIdx then
				local ui = getglobal(string.format("SkillRuneCombineFrameItem%dIcon", i))
				ui:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga")
				btn:SetClientUserData(0, 0)
				btn:SetClientUserData(1, 0);
			end			
		end
		UpdateSkillRuneCombineItem();
	end
end

function SkillRuneCombineFrameItem_Drag()
	local mainplayer = ActorMgr:getMainPlayer();
	local container = mainplayer:getContainer();
	local item = container:getItem( this:GetClientUserData(0), this:GetClientUserData(1) );
	if item == nil or item:getItemId() == 0 then return end
	UIMSG:getUIGV().DragFrom = DRAG_SKILL_RUNE_COMBINE;
	local IconTex = getglobal(this:GetName().."Icon");
	UIBeginDrag(IconTex:GetTexture(), this:GetClientID(), item:getNum(), item:getItemId());
end

function SkillRuneCombineFrameItem_ReceiveDrag()
	if SkillRuneCombineFrame:GetClientUserData(2) == 1 then return end
	local srcList = UIMSG:getUIGV().DragFrom;
	local srcGrid = UIGetDragUserData1();
	local mainplayer = ActorMgr:getMainPlayer();
	local container = mainplayer:getContainer();
	local item = container:getItem( srcList, srcGrid );
	UpdateSkillRuneCombineItem(item);
	UIEndDrag();
	UIMSG:getUIGV().DragFrom = DRAG_NONE;
end

function SetSkillRuneCombineItem(item)
	if SkillRuneCombineFrameClose:IsShown() then return end
	if not item then
		return
	end
	local id = item:getItemId()
	if not SkillRuneItemId[id] then
		return ShowMidTips("只能放置符文道具")
	end
	local level = SkillRuneItemId[id].level
	--[[
	if CombineLevel and level ~= CombineLevel then
		return ShowMidTips("不能合成该类符文")
	end
	--]]
	if level > 0 then
		return ShowMidTips("该符文已合成到最高级")
	end
	UpdateSkillRuneCombineItem(item)
end

function OpenSkillRuneCombineFrame(item)
	CombineLevel = nil
	if item then
		local id = item:getItemId()
		for l, i in pairs(SkillRuneCombineItemId) do
			if i == id then
				CombineLevel = l
				break
			end
		end
	end
	LockItem( item:getListType(), item:getGridIdx(), item:getItemId(), "SkillRuneCombineFrame" );
	SkillRuneCombineFrame:SetClientUserData( 0, item:getListType() );
	SkillRuneCombineFrame:SetClientUserData( 1, item:getGridIdx() );
	SkillRuneCombineFrame:Show()
end

function IsSkillRuneCombineItem(id)
	for _, i in pairs(SkillRuneCombineItemId) do
		if i == id then
			return true
		end
	end
	return false
end

--------------------------------------------飘字--------------------------------------------------------

function SkillRuneDoEx(attr)
	AddFlyFont(22, RUNE_EX_DO, RuneMgr:GetRuneAddAttrName(attr), 0xFFFF73)
end