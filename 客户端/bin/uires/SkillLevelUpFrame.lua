local nSkillupindex		 = 1;
local nMaxSkillBtn       = 4;
local MAX_MODEL_NUM		 = 4;
local SkillLearnState    = 0;
local SetTime            = 0;
local disappeartime      = 4;
local MAX_SKILL_TABLE	 =	10;
local newLearnSkills = {}

function InsertLearnSkill(id, lv)
	for _,info in ipairs(newLearnSkills) do
		if info[1] == id and info[2] == lv then
			return
		end
	end
	table.insert(newLearnSkills, {id, lv})
	SetTime = os.clock()
	UpdateSkillLevelUpFrame();
	
end

function RemoveLearnSkill(id)
	for i = 1, #newLearnSkills do
		if newLearnSkills[i][1] == id then
			table.remove(newLearnSkills, i)
			UpdateSkillLevelUpFrame()
			break
		end
	end
end

function UpdateSkillLevelUpFrame()
	if #newLearnSkills == 0 then
		if SkillLevelUpFrame:IsShown() then
			SkillLevelUpFrame:Hide()
		end
	else
		if not SkillLevelUpFrame:IsShown() then
			SkillLevelUpFrame:Show()
		end
		for i = 1 , nMaxSkillBtn do
			local skillIcon_btn	= getglobal("SkillLevelUpButton"..i.."SkillIcon")
			local skillBtn		= getglobal("SkillLevelUpButton"..i);
			local skillId		= newLearnSkills[i] and newLearnSkills[i][1]
			local level		= newLearnSkills[i] and newLearnSkills[i][2]
			
			if skillId then
				local skillDef = SkillMgr:getSpellStaticAtt(skillId, level )
				local IconPath = GetSkillIconPath()..skillDef.IconID1..".tga"
				-- 如果没找到物品的图标则使用默认图标显示
				if not IsInExistence(IconPath) then
					IconPath = DEFAULT_ITEM_PATH
				end
				skillIcon_btn:Show()
				skillIcon_btn:SetTexture(IconPath);
			else
				skillIcon_btn:Hide()
			end
			skillBtn:SetClientUserData(0, skillId or 0)
			skillBtn:SetClientUserData(1, level or 0)
		end
	end
end


function SkillLevelUpFrame_OnShow()
	SetTime = os.clock();
	local mainplayer = ActorMgr:getMainPlayer();
	local nLv = mainplayer:getLv();
	-- TODO: 与策划绑定技能ID
	if not GameWizard:isDo(GAMEWIZARD_FIRST_LEVELUP)  then
		GameWizard:doStep(GAMEWIZARD_FIRST_LEVELUP);
		ShowMidTips("请点击技能按钮");

		local btn		= getglobal( this:GetName().."XinShouBtn");
		local animationTexture	= getglobal( this:GetName().."XinShouBtnUVAnimationTex");
		animationTexture:SetUVAnimation( 50, true );
		btn:Show();
	end
end

function SkillLevelUpFrame_OnLoad()
	for i=1,nMaxSkillBtn do
		local btn = getglobal("SkillLevelUpButton"..i);
		btn:SetClientUserData(0, 0);
	end
end

function SkillLevelUpFrame_OnUpdate()
	if os.difftime (os.clock(), SetTime) >= disappeartime then
		SkillLevelUpFrame:Hide()
	end
	
end

--@Desc:鼠标移入技能按钮时执行
--@Return:Nothing
function SkillLevelUpButton_OnEnter()
	local skillId   = this:GetClientUserData(0);
	local level		= this:GetClientUserData(1);
	local szName    = this:GetName();

	if skillId <= 0 or skillId == nil or szName == nil or szName == "" then return end

	SpellTipsFrame:SetClientUserData( 0, SkillMgr:getRuneSkillId(skillId) );
	SpellTipsFrame:SetClientUserData( 1, level )
	SpellTipsFrame:SetClientUserData( 2, SKILL_TYPE )
	SpellTipsFrame:SetClientString(szName);
	SpellTipsFrame:Show();
end

--@Desc:技能按钮被拖动时执行
--@Return:Nothing
function SkillLevelUpButton_Drag()
	local skillId = this:GetClientUserData(0)
	if skillId == 0 then
		return
	end
	
	local btn = getglobal( "SkillLevelUpFrameXinShouBtn")
	if btn:IsShown() and this:GetClientID() == 1 then
		btn:Hide()
	end
	
	local name = this:GetName()
	local IconTex = getglobal(name.."SkillIcon")
	if DragSkillIcon(skillId, name, this:GetClientID(), IconTex:GetTexture()) then
		RemoveLearnSkill(skillId)
	end
end

--@Desc:函数SkillLearn被调用时触发
--@Param: [in]nSkillId	指定要显示在面板上的技能ID
--@Param: [in]nSkillLv	指定要显示在面板上的技能等级
--@Return:Nothing
local t_NoNeedShowSkill = {1010, 1018, 2010, 2012, 3010, 3016, 4000, 4002, 7005, 7000, 7001, 7002, 7003, 6010};

local t_NeedShowSkill = { 10395, 10396,10811,10812,10813,10814,10815,10816,10817,10818,11403 };
function SkillLearnClientData(nSkillId, nSkillLv)
	if SkillMgr:isSkillLearned(nSkillId) then
		return
	end
	
	if SkillMgr:isSkillPassive(nSkillId, nSkillLv) then
		return
	end
	
	needShowSkillTips() --判断是否要显示技能按钮提示
	UpdateSkillAutoAssist( nSkillId );

	for i = 1, #t_NoNeedShowSkill do
		if t_NoNeedShowSkill[i] == nSkillId then
			return
		end
	end
	local nSkillIBtnId	= 0
	local MainPlayer	= ActorMgr:getMainPlayer()
	local nRace			= MainPlayer:getRace()
	for _, i in ipairs(GetSkillList(nRace)) do
		if nSkillId == i then
			InsertLearnSkill(nSkillId, nSkillLv)
			break;
		end
	end
	for _,i in ipairs( t_NeedShowSkill ) do
		if nSkillId == i then
			InsertLearnSkill( nSkillId, nSkillLv )
			break
		end
	end
end

function SkillLevelUpFrame_OnHide()
	newLearnSkills = {}
	local btn = getglobal( this:GetName().."XinShouBtn");
	btn:Hide();
end

function setShortCutNewSkillEffect(id,level)
	if SkillMgr:isSkillPassive(id, level) then
		return
	end
	for i = 1, #t_NoNeedShowSkill do
		if t_NoNeedShowSkill[i] == id then
			return
		end
	end
	if not ShortCutFrame:IsShown() then
		return ;
	end
	local index			= LookForBlankShortCut();
	local MainPlayer	= ActorMgr:getMainPlayer()
	local nRace			= MainPlayer:getRace();
	for _, i in ipairs(GetSkillList(nRace)) do
		if id == i then
			GameShortCut:setShortCut( 1, id, index-1, 0 );		
			local btnIndex = FindSkillLevelUpBtn( id );
			if btnIndex > 0 then
				local effectToFrame = getglobal("NewSkillEffectToFrame"..btnIndex);
				effectToFrame:Show();
				effectToFrame:SetPoint( "center", "SkillLevelUpButton"..btnIndex , "center", 0, 0 );
				effectToFrame:SetClientString( "MagicShortcut"..index);
			end
		end
	end
end

function FindSkillLevelUpBtn( id )
	for i = 1, nMaxSkillBtn do
		local btn = getglobal("SkillLevelUpButton"..i);
		if btn:GetClientUserData(0) == id then
			return i;
		end
	end
	return 0;
end

function LookForBlankShortCut()
	local nShortCutIndex = 0;
	local flag = true;
	for i=1,10 do
		local shortcutBtn = getglobal("MagicShortcut"..i);
		if shortcutBtn:GetClientUserData(0) == 0 then
			nShortCutIndex = i;
			flag = false;
			break;
		end
	end
	if flag then
		for i=13,22 do
			local shortcutBtn = getglobal("MagicShortcut"..i);
			if shortcutBtn:GetClientUserData(0) == 0 then
				nShortCutIndex = i;
				break;
			end
		end
	end
	if	nShortCutIndex == 0	then
		nShortCutIndex = GameShortCut:getNewSkillShortcutIndex();
		if nShortCutIndex == 10 then
			GameShortCut:setNewSkillShortcutIndex(13);
		else
			GameShortCut:setNewSkillShortcutIndex(nShortCutIndex + 1);
		end
	end
	return nShortCutIndex;
end

function NewSkillEffectToFrame_onShow()
	this:SetClientUserData( 0, os.clock() );
	local framModel	=	getglobal(this:GetName().."FairyModel");
	framModel:setPetModel( 0,0,"effect\\entity\\huodezhuangbei.ent" )
	framModel:setPetScale(0,5,5,5);
	framModel:setEntityPosition(0,0,100,0);
end

function NewSkillEffectToFrame_onUpdate()
	local lastFrameTime = math.floor( os.clock() - this:GetClientUserData(0) );
	if lastFrameTime > 4 then
		this:Hide();
		return;
	end
	--转换特效
	if  lastFrameTime >= 1 then
		local DesShortcut = getglobal( this:GetClientString() );
		this:SetPoint( "center", DesShortcut:GetName(), "center", 0, 0 );
	end	
end