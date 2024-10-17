local t_AddPetSkillControl = { heightId = 0, lowId = 0, id = 0 };

function GetAddPetSkillControl()
	return t_AddPetSkillControl;
end

t_AddPetSkillControl["init"] = 
function ( self )
	self["heightId"]	= 0;
	self["lowId"]		= 0;
	self["id"]			= 0;
end

t_AddPetSkillControl["setOpPet"] = 
function ( self, data )
	self["heightId"]	= data["heightId"];
	self["lowId"]		= data["lowId"];
	self["id"]			= data["id"];
end

t_AddPetSkillControl["getAddSkillYiBi"] = 
function ( self )
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local petInfo		= petPackage:getPetInfo( t_AddPetSkillControl["heightId"], t_AddPetSkillControl["lowId"] );
	local nAddSkillGrid	= petInfo.PetSkills.SkillGridNum + 1;
	local petSkillGridDef = getPetSkillGridDef( nAddSkillGrid );
	local nNeedMoney	= 0;
	if petSkillGridDef ~= nil then
		nNeedMoney = petSkillGridDef.Money;
	end

	return nNeedMoney;
end

local t_OpenPetSkillConfirmFrameEvents = 
{ 
	["GE_CHANGE_BULL"] = {},
};

t_OpenPetSkillConfirmFrameEvents["GE_CHANGE_BULL"].func = 
function ()	
	if not OpenPetSkillConfirmFrame:IsShown() then
		return;
	end
	
	local mainplayer		= ActorMgr:getMainPlayer();
	local t_fonts = {	
						["OpenPetSkillConfirmFrameCurHaveYinBiValueBtnFont"]	= { value = getPlayerMoney( mainplayer ) }, 
					};
	util.SetFonts( t_fonts );
end

function OpenPetSkillConfirmFrame_OnLoad()
	for event, _ in pairs( t_OpenPetSkillConfirmFrameEvents ) do
		this:RegisterEvent( event );
	end

	t_OpenPetSkillConfirmFrameEvents.__index = function ( tab, key )
									tab[key] = { func = function () end };
									return tab[key];
								end
	setmetatable( t_OpenPetSkillConfirmFrameEvents, t_OpenPetSkillConfirmFrameEvents );
end

function OpenPetSkillConfirmFrame_OnEvent()
	t_OpenPetSkillConfirmFrameEvents[arg1].func();
end

function OpenPetSkillConfirmFrame_OnShow()
	local mainplayer	= ActorMgr:getMainPlayer();
	local t_fonts = {	[this:GetName().."NeedYinBiValueBtnFont"]		= { value = t_AddPetSkillControl:getAddSkillYiBi() }, 
						[this:GetName().."CurHaveYinBiValueBtnFont"]	= {	value = getPlayerMoney( mainplayer ),
											color = ( getPlayerMoney( mainplayer ) < t_AddPetSkillControl:getAddSkillYiBi() ) and
											{ r = 255, g = 0, b = 0 } or { r = 150, g = 150, b = 150 } },
					};
	util.SetFonts( t_fonts );
end

function OpenPetSkillConfirmFrame_OnHide()
	t_AddPetSkillControl:init();
end

function OpenPetSkillConfirmFrame_ConfirmBtn_OnClick()
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	
	if getPlayerMoney( mainplayer ) < t_AddPetSkillControl:getAddSkillYiBi() then
		ShowMidTips( "银币不足" );
		return;
	end

	petPackage:addPetSkillGrid( t_AddPetSkillControl["heightId"], t_AddPetSkillControl["lowId"] );
	OpenPetSkillConfirmFrame:Hide();
end

-- 宠物技能学习
local t_petOperationMutexFrame = { "Pet_XiDianFrame", "Pet_DevelopmentBuildUpFrame" };
function Pet_SkillLearnFrame_OnShow()
	for _, name in ipairs( t_petOperationMutexFrame ) do
		local frame = getglobal( name ); 
		if frame:IsShown() then
			frame:Hide();
			break;
		end
	end

	SetOperationSelUseItem( { id = 0, selItem = nil, listType = 0, gridx = 0 } );
	SetOperationSelSkill( { id = 0, lv = 0, gridx = -1, } );
	
	ClearPet_OperationFrame( this:GetName() );
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local petPackage = mainplayer:getPet();

	local slider = getglobal( this:GetName() .. "_ScrollBar" );
	slider:SetValue( 0 );
	-- 设置滚动条
	SetScrollBar( this:GetName() .. "_ScrollBar", MAX_PET_NUM, petPackage:getPetNum() );
	-- 若没宠物，则清空面板显示的内容
	if petPackage:getPetNum() == 0 then
		return;
	end

	-- 刷新宠物列表
	UpdateOperationPetList( this:GetName() );

	SetPetInfoShow( this:GetName() );

	local conFrame = getglobal( "PlayerContainerFrame" );
	if not conFrame:IsShown() then
		conFrame:Show();
		conFrame:SetPoint( "topleft", this:GetName(), "topright", 0, 0 );
	end

	ChekPet_OperationFrame_XinShou_Guide( this:GetName() );
end


-- 宠物强化
MAX_INCR_ATTR_PERCENT = 10;
local t_arrorOffsetX = { [1] = 70, [2] = 88, [3] = 106, [4] = 124, [5] = 142, [6] = 160, [7] = 178, [8] = 197, [9] = 215, [10] = 233,
						 [11] = 251, [12] = 269 } ;

-- waitSavePetInfo = { ["heightId"] = 0, ["lowId"] = 0, ["incrAttrVal"] = 0, ["level"] = 0 }

local t_petStrengthenControl = {	curStrengthenLevel = 1,	waitSavePetInfoList = {}, curSelLevel = 0,
									curSaveOpPetInfo = { ["heightId"] = 0, ["lowId"] = 0, },
									 -- ["heightId"] = 0, ["lowId"] = 0, ["list"] = {}
									-- listelem: [name] = { lastTime = , isOpaque = , level =  }
									requireResetLevel = { }, AVERAGE_CHG_ALPHA = 0.5 / 1000 };
function GetPetStrengthenControl()
	return t_petStrengthenControl;
end

t_petStrengthenControl["setCurSelLevel"] =
function ( self, data )
	self["curSelLevel"] = data["level"];
end

t_petStrengthenControl["getCurSelLevel"] =
function ( self, data )
	return self["curSelLevel"];
end

t_petStrengthenControl["getAllRequireResetLevel"] =
function ( self, data )
	for _, onePetResetData in ipairs( self["requireResetLevel"] ) do
		if onePetResetData["heightId"] == data["heightId"] and onePetResetData["lowId"] == data["lowId"] then
			--print( "getAllRequireResetLevel--list--170" );
			return onePetResetData["list"];
		end
	end
	
	--print( "getAllRequireResetLevel--{}--175" );
	return {};
end

t_petStrengthenControl["removePetRequireResetLevelInfo"] =
function ( self, data )
	for index, onePetResetData in ipairs( self["requireResetLevel"] ) do
		if onePetResetData["heightId"] == data["heightId"] and onePetResetData["lowId"] == data["lowId"] then
			table.remove( self["requireResetLevel"], index );
		end
	end
end

t_petStrengthenControl["isPetHaveRequireResetLevel"] =
function ( self, data )
	local t_alreadyResetList = self:getAllRequireResetLevel( data );
	return table.getn( t_alreadyResetList ) > 0;
end

t_petStrengthenControl["isInRequireResetLevel"] =
function ( self, data )
	local t_alreadyResetList = self:getAllRequireResetLevel( data );
	for _, oneConfig in ipairs( t_alreadyResetList ) do
		if oneConfig["level"] ==  data["level"] then
			return true;
		end
	end

	return false;
end

t_petStrengthenControl["addRequireResetLevel"] =
function ( self, data )
	local t_alreadyResetList = self:getAllRequireResetLevel( data );
	if table.getn( t_alreadyResetList ) == 0 then
		table.insert( self["requireResetLevel"], { ["heightId"] = data["heightId"], ["lowId"] = data["lowId"], list = {} } );
		t_alreadyResetList = self["requireResetLevel"][table.getn( self["requireResetLevel"] )]["list"];
	end

	table.insert( t_alreadyResetList, { ["lastTime"] = GameMgr:getTickTime(), ["isOpaque"] = false, ["level"] = data["level"] } );
end

t_petStrengthenControl["removeRequireResetLevel"] =
function ( self, data )
	local t_alreadyResetList = self:getAllRequireResetLevel( data );
	if table.getn( t_alreadyResetList ) == 0 then
		return;
	end
	
	for index, oneResetLevelInfo in ipairs( t_alreadyResetList ) do
		if oneResetLevelInfo["level"] == data["level"] then
			table.remove( t_alreadyResetList, index );
			break;
		end
	end

	if table.getn( t_alreadyResetList ) == 0 then
		self:removePetRequireResetLevelInfo( data );
	end
end


t_petStrengthenControl["setCurSaveOpPetInfo"] = 
function ( self, data )
	self["curSaveOpPetInfo"]["heightId"]	= data["heightId"];
	self["curSaveOpPetInfo"]["lowId"]		= data["lowId"];
end

t_petStrengthenControl["getCurSaveOpPetInfo"] = 
function ( self )
	return self["curSaveOpPetInfo"];
end

t_petStrengthenControl["clearCurSaveOpPetInfo"] = 
function ( self, data )
	self["curSaveOpPetInfo"]["heightId"]	= 0;
	self["curSaveOpPetInfo"]["lowId"]		= 0;
end

t_petStrengthenControl["setFirstNotStrengthenLevel"] = 
function ( self, data )
	local petInfo = data["petInfo"];
	--print( "setFirstNotStrengthenLevel--petInfo.NextStrenLvl = "..petInfo.NextStrenLvl.."--257" );
	--[[
	if petInfo.NextStrenLvl == 0 then
		self:setCurStrengthenLevel( { ["level"] = CLIENT_MAX_PET_STRENGTHEN_LEVEL } );
	else
		self:setCurStrengthenLevel( { ["level"] = math.min( CLIENT_MAX_PET_STRENGTHEN_LEVEL, petInfo.NextStrenLvl ) } );
	end
	--]]
	local nextLv = (petInfo.StrengthenLevel + 1 > CLIENT_MAX_PET_STRENGTHEN_LEVEL) and 0 or petInfo.StrengthenLevel + 1;
	self:setCurStrengthenLevel( { ["level"] = math.min( CLIENT_MAX_PET_STRENGTHEN_LEVEL, nextLv ) } );
	
	self:setCurSelLevel( { ["level"] = self:getCurStrengthenLevel() } );
end

t_petStrengthenControl["getCurFirstWaitSavePetInfo"] = 
function ( self, data )
	if table.getn( self["waitSavePetInfoList"] ) == 0 then
		return nil;
	end

	return self["waitSavePetInfoList"][1];
end

t_petStrengthenControl["addCurWaitSavePetInfo"] = 
function ( self, data )
	local t_oneWaitPetInfo = self:getOneWaitSavePetInfo( data );
	if t_oneWaitPetInfo == nil then
		table.insert( self["waitSavePetInfoList"], data );
	else
		t_oneWaitPetInfo["incrAttrVal"] = data["incrAttrVal"];
	end
end

t_petStrengthenControl["removeCurWaitSavePetInfo"] = 
function ( self, data )
	local nDestHeightID = data["heightId"];
	local nDestlowID	= data["lowId"];
	for index, oneStoreData in ipairs ( self["waitSavePetInfoList"] ) do
		if oneStoreData["heightId"] == nDestHeightID and oneStoreData["lowId"] == nDestlowID then
			table.remove( self["waitSavePetInfoList"], index );
			break;
		end
	end
end

t_petStrengthenControl["getOneWaitSavePetInfo"] = 
function ( self, data )
	local nDestHeightID = data["heightId"];
	local nDestlowID	= data["lowId"];
	for index, oneStoreData in ipairs ( self["waitSavePetInfoList"] ) do
		if oneStoreData["heightId"] == nDestHeightID and oneStoreData["lowId"] == nDestlowID then
			return oneStoreData;
		end
	end
end

t_petStrengthenControl["isOneWaitSavePetInfo"] = 
function ( self, data )
	return self:getOneWaitSavePetInfo( data ) ~= nil;
end

t_petStrengthenControl["clearCurWaitSavePetInfo"] = 
function ( self )
	self["waitSavePetInfoList"] = {};
	self["requireResetLevel"]	= {};
	self:clearCurSaveOpPetInfo();
end

t_petStrengthenControl["setCurStrengthenLevel"] = 
function ( self, data )
	self["curStrengthenLevel"] = data["level"];
end

t_petStrengthenControl["getCurStrengthenLevel"] = 
function ( self )
	return self["curStrengthenLevel"];
end

t_petStrengthenControl["getCurStrengthenArrowOFfsetX"] = 
function ( self, data )
	local nLevel = data["level"];
	return t_arrorOffsetX[nLevel];
end

t_petStrengthenControl["getStarTexConfig"] = 
function ( self, data )
	local nIncrVal = data["value"];
	local t_oneStarConfig;

	if nIncrVal == nil then
		t_oneStarConfig = {	path = "uires\\ZhuJieMian\\1.dds", UV = { x = 577, y = 64, width = 13, height = 12 }, };
		return t_oneStarConfig;
	end

	if nIncrVal == MAX_INCR_ATTR_PERCENT then
		t_oneStarConfig = {	path = "uires\\ZhuJieMian\\1.dds", UV = { x = 530, y = 33, width = 13, height = 12 }, };
	else
		t_oneStarConfig = {	path = "uires\\ZhuJieMian\\1.dds", UV = { x = 516, y = 33, width = 13, height = 12 }, };
	end

	return t_oneStarConfig;
end

function ShowStrengSaveFrame()
	if Pet_DevelopmentBuildUpFrameQiangHuaTipsBackBtn:IsShown() then
		UpdatePet_DevelopmentBuildUpFrameQiangHuaTipsBackBtn();
		return;
	end

	local t_showIcons = { "Pet_DevelopmentBuildUpFrameQiangHuaTipsBackBtn", "Pet_DevelopmentBuildUpFrameQiangHuaTipsArrowBtn",
							"Pet_DevelopmentBuildUpFrameSaveBtn", "Pet_DevelopmentBuildUpFrameCancelBtn" };
	util.ShowIcons( t_showIcons );
	local t_petStrengthenControl	= GetPetStrengthenControl();
	--local nCurStrthenLevel			= t_petStrengthenControl:getCurStrengthenLevel();
	local firstWaitSavePetInfo		= t_petStrengthenControl:getCurFirstWaitSavePetInfo();
	local nOffsetX					= t_petStrengthenControl:getCurStrengthenArrowOFfsetX( { level = (firstWaitSavePetInfo["level"] > 0 and firstWaitSavePetInfo["level"] or 1) } );
	local arrowBtn					= getglobal( "Pet_DevelopmentBuildUpFrameQiangHuaTipsArrowBtn" );
	arrowBtn:SetPoint( "topleft", "Pet_DevelopmentBuildUpFrame", "topleft", nOffsetX + 20, 237 );
	
	local t_operationSelPetInfo = GetOperationSelPetInfo();
	t_petStrengthenControl:setCurSaveOpPetInfo( t_operationSelPetInfo );
end

function HideStrengSaveFrame()
	if not Pet_DevelopmentBuildUpFrameQiangHuaTipsBackBtn:IsShown() then
		return;
	end

	local t_hideIcons = { "Pet_DevelopmentBuildUpFrameQiangHuaTipsBackBtn", "Pet_DevelopmentBuildUpFrameQiangHuaTipsArrowBtn",
							"Pet_DevelopmentBuildUpFrameSaveBtn", "Pet_DevelopmentBuildUpFrameCancelBtn",
							"Pet_DevelopmentBuildUpFrame_ContinueQiangHuaBtn" };
	util.HideIcons( t_hideIcons );

	local t_showIcons = { "Pet_DevelopmentBuildUpFrame_QiangHuaBtn", };
	util.ShowIcons( t_showIcons );

	t_petStrengthenControl:clearCurSaveOpPetInfo();
end

function ValidateStrengSaveFrame()
	if not Pet_DevelopmentBuildUpFrameQiangHuaTipsBackBtn:IsShown() then
		return;
	end

	local t_petStrengthenControl	= GetPetStrengthenControl();
	local t_CurSaveOpPetInfo		= t_petStrengthenControl:getCurSaveOpPetInfo();
	local mainplayer				= ActorMgr:getMainPlayer();
	local petPackage				= mainplayer:getPet();
	local t_operationSelPetInfo		= GetOperationSelPetInfo();
	if t_operationSelPetInfo["heightId"] == t_CurSaveOpPetInfo["heightId"] and 
		t_operationSelPetInfo["lowId"] == t_CurSaveOpPetInfo["lowId"] then

		return;
	end

	HideStrengSaveFrame();
end

function Change_Pet_DevelopmentBuildUpFrameFrameStrengBtnState()
	local firstWaitSavePetInfo = t_petStrengthenControl:getCurFirstWaitSavePetInfo();
	local t_btns ={	"Pet_DevelopmentBuildUpFrame_QiangHuaBtn",
					"Pet_DevelopmentBuildUpFrame_ContinueQiangHuaBtn"
					};
	local initBtn		= getglobal( "Pet_DevelopmentBuildUpFrame_QiangHuaBtn" );
	local continueBtn	= getglobal( "Pet_DevelopmentBuildUpFrame_ContinueQiangHuaBtn" );
	
	local mainplayer				= ActorMgr:getMainPlayer();
	local petPackage				= mainplayer:getPet();
	local t_operationSelPetInfo		= GetOperationSelPetInfo();
	local curSelPetInfo				= petPackage:getPetInfo( t_operationSelPetInfo["heightId"], t_operationSelPetInfo["lowId"] );
	local nCurSelectLevel			= t_petStrengthenControl:getCurSelLevel();
	if firstWaitSavePetInfo == nil then
		continueBtn:Hide();
		initBtn:Show();
		util.EnableBtns( t_btns );
		return;
	end

	
	local petInfo = petPackage:getPetInfo( firstWaitSavePetInfo["heightId"], firstWaitSavePetInfo["lowId"] );
	if petInfo == nil then
		t_petStrengthenControl:removeCurWaitSavePetInfo( { ["heightId"] = firstWaitSavePetInfo["heightId"], ["lowId"] = firstWaitSavePetInfo["lowId"] } );
		continueBtn:Hide();
		initBtn:Show();
		util.EnableBtns( t_btns );
		return;
	end
	
	--print( "Change_Pet_DevelopmentBuildUpFrameFrameStrengBtnState--nCurSelectLevel = "..nCurSelectLevel.."， petInfo.NextStrenLvl = "
	--		..petInfo.NextStrenLvl );
	if t_operationSelPetInfo["heightId"] == firstWaitSavePetInfo["heightId"] and 
		t_operationSelPetInfo["lowId"] == firstWaitSavePetInfo["lowId"] and 
		nCurSelectLevel == firstWaitSavePetInfo["level"] then
		--nCurSelectLevel == petInfo.NextStrenLvl then
		continueBtn:Show();
		initBtn:Hide();
		util.EnableBtns( t_btns );
	else
		continueBtn:Hide();
		initBtn:Show();
		util.DisableBtns( t_btns );
	end
end

-- 显示继续强化按钮
function ShowContinueStrthenBtn()
	local continueBtn = getglobal( "Pet_DevelopmentBuildUpFrame_ContinueQiangHuaBtn" );
	if continueBtn:IsShown() then
		return;
	end

	local initBtn = getglobal( "Pet_DevelopmentBuildUpFrame_QiangHuaBtn" );
	initBtn:Hide();
	continueBtn:Show();
end

-- 显示强化按钮
function ShowInitStrthenBtn()
	local initBtn = getglobal( "Pet_DevelopmentBuildUpFrame_QiangHuaBtn" );
	if initBtn:IsShown() then
		return;
	end

	local continueBtn = getglobal( "Pet_DevelopmentBuildUpFrame_ContinueQiangHuaBtn" );
	continueBtn:Hide();
	initBtn:Show();
end

-- 更新宠物强化面板的强化星星
function ClearPet_DevelopmentBuildUpFrameStars()
	for i = 1, CLIENT_MAX_PET_STRENGTHEN_LEVEL do
		local btn = getglobal( "Pet_DevelopmentBuildUpFrame_QiangHuaSmallStarBtn"..i );
		btn:Hide();

		btn = getglobal( "Pet_DevelopmentBuildUpFrame_QiangHuaBigStarBtn"..i );
		btn:Hide();
	end
end

function UpdatePet_DevelopmentBuildUpFrameStars()
	ClearPet_DevelopmentBuildUpFrameStars();

	local t_operationSelPetInfo = GetOperationSelPetInfo();
	local mainplayer			= ActorMgr:getMainPlayer();
	local petPackage			= mainplayer:getPet();
	local petInfo				= petPackage:getPetInfo( t_operationSelPetInfo["heightId"], t_operationSelPetInfo["lowId"] );
	if petInfo == nil then
		return;
	end
	
	local t_petStrengthenControl	= GetPetStrengthenControl();
	local nCurSelectLevel			= t_petStrengthenControl:getCurSelLevel();
	local nWaitSaveLevel			= 0;
	local petWID					= petInfo.WID;
	local firstWaitSavePetInfo		= t_petStrengthenControl:getCurFirstWaitSavePetInfo();
	if firstWaitSavePetInfo ~= nil and firstWaitSavePetInfo["heightId"] == petWID.nHeight and
		firstWaitSavePetInfo["lowId"] == petWID.nLow then
			nWaitSaveLevel = firstWaitSavePetInfo["level"]; 
	end

	for i = 1, math.min( petInfo.StrengthenLevel + 1, MAX_STRENGTH_LEVEL ) do
		if nCurSelectLevel == i or nWaitSaveLevel == i then
			local btn = getglobal( "Pet_DevelopmentBuildUpFrame_QiangHuaBigStarBtn"..i );
			btn:Show();
		else
			local btn				= getglobal( "Pet_DevelopmentBuildUpFrame_QiangHuaSmallStarBtn"..i );
			local tex				= getglobal( btn:GetName().."Tex" );
			local nIncrAttr			= petPackage:getStrthenVal( petInfo, i - 1 );
			local t_oneTexConfig	= t_petStrengthenControl:getStarTexConfig( { value = nIncrAttr } );
			tex:SetTexture( t_oneTexConfig["path"] );
			local UV				= t_oneTexConfig["UV"];
			tex:SetTexUV( UV["x"], UV["y"], UV["width"], UV["height"] );
			btn:Show();
			btn:Enable();
			if i == petInfo.StrengthenLevel + 1 then
				t_oneTexConfig	= t_petStrengthenControl:getStarTexConfig( { value = nil } );
				tex:SetTexture( t_oneTexConfig["path"] );
				UV		= t_oneTexConfig["UV"];
				tex:SetTexUV( UV["x"], UV["y"], UV["width"], UV["height"] );
			end
		end
	end
		

	--[[for i = petInfo.StrengthenLevel + 1, math.min( petInfo.StrengthenLevel + 1, MAX_STRENGTH_LEVEL ) do
		if i == petInfo.NextStrenLvl + 1 then
			local btn = getglobal( "Pet_DevelopmentBuildUpFrame_QiangHuaBigStarBtn"..i );
			btn:Show();
		else
			local btn				= getglobal( "Pet_DevelopmentBuildUpFrame_QiangHuaSmallStarBtn"..i );
			local tex				= getglobal( btn:GetName().."Tex" );
			local t_oneTexConfig	= t_petStrengthenControl:getStarTexConfig( { value = nil } );
			tex:SetTexture( t_oneTexConfig["path"] );
			local UV				= t_oneTexConfig["UV"];
			tex:SetTexUV( UV["x"], UV["y"], UV["width"], UV["height"] );
			btn:Show();
			btn:Disable();
		end
	end--]]

	for i = petInfo.StrengthenLevel + 2, MAX_STRENGTH_LEVEL do
		local btn				= getglobal( "Pet_DevelopmentBuildUpFrame_QiangHuaSmallStarBtn"..i );
		local tex				= getglobal( btn:GetName().."Tex" );
		local t_oneTexConfig	= t_petStrengthenControl:getStarTexConfig( { value = nil } );
		tex:SetTexture( t_oneTexConfig["path"] );
		local UV				= t_oneTexConfig["UV"];
		tex:SetTexUV( UV["x"], UV["y"], UV["width"], UV["height"] );
		btn:Show();
		btn:Disable();
	end
end

-- 点击小星星
function Pet_DevelopmentBuildUpFrame_QiangHuaSmallStarBtn_OnClick()
	if PetStrengthenConfirmSavePerfectFrame:IsShown() then
		ShowMidTips( "请先确认是否保存完美强化" );
		return;
	end
	
	local t_petStrengthenControl= GetPetStrengthenControl();
	local mainplayer			= ActorMgr:getMainPlayer();
	local petPackage			= mainplayer:getPet();
	local t_operationSelPetInfo = GetOperationSelPetInfo();
	local petInfo				= petPackage:getPetInfo( t_operationSelPetInfo["heightId"], t_operationSelPetInfo["lowId"] );
	local nCurStrengthenedLevel	= petInfo.StrengthenLevel;
	local nSelStrengthenLevel	= this:GetClientID();
	
	local nCurSelectLevel = t_petStrengthenControl:getCurSelLevel();
	if nSelStrengthenLevel == nCurSelectLevel then
		return;
	end

	if nSelStrengthenLevel <= ( nCurStrengthenedLevel + 1 ) then
		if Pet_DevelopmentBuildUpFrameQiangHuaTipsBackBtn:IsShown() then
			ShowMidTips( "请先保存当前强化结果" );
			return;
		end
		t_petStrengthenControl:setCurSelLevel( { ["level"] = nSelStrengthenLevel } );
		t_petStrengthenControl:setCurStrengthenLevel( { ["level"] = nSelStrengthenLevel } );
		Validate_Pet_DevelopmentBuildUpFrame_StrengthenItem();
		UpdateUseCaiLiaoNum();
		UpdatePet_DevelopmentBuildUpFrameStars();
		Change_Pet_DevelopmentBuildUpFrameFrameStrengBtnState();
		--[[
		if t_petStrengthenControl:isInRequireResetLevel( {	["heightId"] = t_operationSelPetInfo["heightId"], 
															["lowId"] = t_operationSelPetInfo["lowId"],
															["level"] = nSelStrengthenLevel } 
														) then

			t_petStrengthenControl:setCurSelLevel( { ["level"] = nSelStrengthenLevel } );
			UpdatePet_DevelopmentBuildUpFrameStars();
			Change_Pet_DevelopmentBuildUpFrameFrameStrengBtnState();
		else
			if Pet_DevelopmentBuildUpFrameQiangHuaTipsBackBtn:IsShown() then
				ShowMidTips( "请先保存当前强化结果" );
				return;
			end

			if petInfo.NextStrenLvl ~= 0 and petInfo.NextStrenLvl < petInfo.StrengthenLevel + 1 then
				ShowMidTips( "请先强化已激活的星星" );
				return;
			end

			if t_petStrengthenControl:isPetHaveRequireResetLevel( t_operationSelPetInfo ) then
				ShowMidTips( "请先强化已激活的星星" );
				return;
			end

			local container	= mainplayer:getContainer();
			if container:getItemCount( CLIENT_RESTREN_ITEMID ) == 0 then
				local itemDef = getItemDef( CLIENT_RESTREN_ITEMID );
				ShowMidTips( "缺少" .. itemDef.Name .. "，无法抹除强化星级" );
				return;				
			end

			local t_PetStrengthenConfirmStarBackFrameControl = GetPetStrengthenConfirmStarBackFrameControl();
			t_PetStrengthenConfirmStarBackFrameControl:setCurStrthenLevel( { ["curStrthenLevel"] = nSelStrengthenLevel, 
																				["heightId"] = t_operationSelPetInfo["heightId"],
																				["lowId"] = t_operationSelPetInfo["lowId"], } );
			PetStrengthenConfirmStarBackFrame:Show();
		end
		--]]
	end
end

function Pet_DevelopmentBuildUpFrame_QiangHuaSmallStarBtn_OnEnter()
	local t_operationSelPetInfo = GetOperationSelPetInfo();
	local mainplayer			= ActorMgr:getMainPlayer();
	local petPackage			= mainplayer:getPet();
	local petInfo				= petPackage:getPetInfo( t_operationSelPetInfo["heightId"], t_operationSelPetInfo["lowId"] );
	if petInfo == nil then
		return;
	end
	
	local nQueryLevel		= this:GetClientID();
	local nIncrAttr			= petPackage:getStrthenVal( petInfo, nQueryLevel - 1 );
	if nIncrAttr == 0 then
		return;
	end

	local szText = "";
	if nIncrAttr == MAX_INCR_ATTR_PERCENT then
		szText = "宠物完美强化，所有属性增加"..nIncrAttr.."%";
	else
		szText = "宠物普通强化，所有属性增加"..nIncrAttr.."%";
	end

	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = "cursor", vertical = "top",horizonal="right", relBtn = { this:GetName(), } } );
end

-- 点击大星星
function Pet_DevelopmentBuildUpFrame_QiangHuaBigStarBtn_OnShow()
	local animTex = getglobal( this:GetName().."UVAnimationTex" );
	animTex:SetUVAnimation( 30, true );
	animTex:Show();
end

function Pet_DevelopmentBuildUpFrame_QiangHuaBigStarBtn_OnClick()
	if PetStrengthenConfirmSavePerfectFrame:IsShown() then
		ShowMidTips( "请先确认是否保存完美强化" );
		return;
	end

	local t_petStrengthenControl= GetPetStrengthenControl();
	local mainplayer			= ActorMgr:getMainPlayer();
	local petPackage			= mainplayer:getPet();
	local t_operationSelPetInfo = GetOperationSelPetInfo();
	local petInfo				= petPackage:getPetInfo( t_operationSelPetInfo["heightId"], t_operationSelPetInfo["lowId"] );
	local nCurStrengthenedLevel	= petInfo.StrengthenLevel;
	local nSelStrengthenLevel	= this:GetClientID();
	local nCurSelectLevel		= t_petStrengthenControl:getCurSelLevel();

	if nCurSelectLevel == nSelStrengthenLevel then
		return;
	end

	t_petStrengthenControl:setCurSelLevel( { ["level"] = nSelStrengthenLevel } );
	Change_Pet_DevelopmentBuildUpFrameFrameStrengBtnState();
end

-- 点击保存按钮
function Pet_DevelopmentBuildUpFrameSaveBtn_OnClick()
	local mainplayer			= ActorMgr:getMainPlayer();
	local petPackage			= mainplayer:getPet();
	local t_operationSelPetInfo = GetOperationSelPetInfo();
	local t_waitSavePetInfo		= t_petStrengthenControl:getOneWaitSavePetInfo( t_operationSelPetInfo );
	local firstWaitSavePetInfo	= t_petStrengthenControl:getCurFirstWaitSavePetInfo();
	if t_waitSavePetInfo ~= nil and firstWaitSavePetInfo ~= nil then
		petPackage:petSaveStrengthen( t_waitSavePetInfo["heightId"], t_waitSavePetInfo["lowId"], t_waitSavePetInfo["incrAttrVal"], firstWaitSavePetInfo["level"] );
		t_petStrengthenControl:removeCurWaitSavePetInfo( t_operationSelPetInfo );
	end

	HideStrengSaveFrame();	
	
	if IsMessageBoxBlock(Pet_DevelopmentBuildUpFrame) then
		MessageBoxBlockEnd(Pet_DevelopmentBuildUpFrame)
	end
end

function Pet_DevelopmentBuildUpFrameSaveBtn_OnEnter()
	local szText = "保存后，强化的属性才会被加成到宠物身上";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = "Pet_DevelopmentBuildUpFrameQiangHuaTipsBackBtn",
						button = this:GetName(), vertical = "bottom" } );
end

-- 点击取消按钮
function Pet_DevelopmentBuildUpFrameCancelBtn_OnClick()
	HideStrengSaveFrame();
	
	local t_operationSelPetInfo = GetOperationSelPetInfo();
	local firstWaitSavePetInfo	= t_petStrengthenControl:getCurFirstWaitSavePetInfo();
	
	t_petStrengthenControl:removeRequireResetLevel( { ["heightId"] = t_operationSelPetInfo["heightId"], ["lowId"] = t_operationSelPetInfo["lowId"], 
														["level"] = firstWaitSavePetInfo["level"] });
	
	t_petStrengthenControl:removeCurWaitSavePetInfo( t_operationSelPetInfo );
	local tex = getglobal( "Pet_DevelopmentBuildUpFrame_QiangHuaSmallStarBtn"..firstWaitSavePetInfo["level"].."Tex" );
	tex:SetBlendAlpha( 1 );
	UpdatePet_DevelopmentBuildUpFrameStars();
	
	if IsMessageBoxBlock(Pet_DevelopmentBuildUpFrame) then
		MessageBoxBlockEnd(Pet_DevelopmentBuildUpFrame)
	end
end

function Pet_DevelopmentBuildUpFrameCancelBtn_OnEnter()
	local szText = "取消保存则您此次操作将不会改变宠物星级";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, 
						frame = "Pet_DevelopmentBuildUpFrameQiangHuaTipsBackBtn",
						button = this:GetName(), vertical = "bottom" } );
end

-- 属性提升tips
function Pet_DevelopmentBuildUpFrameQiangHuaTipsBackBtn_OnShow()
	Pet_DevelopmentBuildUpFrameQiangHuaTipsBtn:Hide()
	UpdatePet_DevelopmentBuildUpFrameQiangHuaTipsBackBtn();
end

function Pet_DevelopmentBuildUpFrameQiangHuaTipsBackBtn_OnHide()
	Pet_DevelopmentBuildUpFrameQiangHuaTipsBtn:Show()
end

function UpdatePet_DevelopmentBuildUpFrameQiangHuaTipsBackBtn()
	local t_petStrengthenControl	= GetPetStrengthenControl();
	local mainplayer			= ActorMgr:getMainPlayer();
	local petPackage			= mainplayer:getPet();
	local t_operationSelPetInfo = GetOperationSelPetInfo();
	local t_waitSavePetInfo		= t_petStrengthenControl:getOneWaitSavePetInfo( t_operationSelPetInfo );
	
	local nIncrAttrVal			= t_waitSavePetInfo["incrAttrVal"];
	local szText;

	local t_Color;
	if nIncrAttrVal == MAX_INCR_ATTR_PERCENT then
		szText = "完美强化成功，所有属性提高";
		t_Color = { r = 255, g = 0, b = 255 };
	else
		szText = "普通强化成功，所有属性提高";
		t_Color = { r = 255, g = 255, b = 190 };
	end

	local t_fonts = {	
						["Pet_DevelopmentBuildUpFrameQiangHuaTipsBackBtnFont"]	= { value = szText..nIncrAttrVal.."%", color = t_Color }, 
					};
	util.SetFonts( t_fonts );
end

-- 确认保存完美强化星级
function PetStrengthenConfirmSavePerfectFrame_OnShow()
	this:SetPoint( "center", "Pet_DevelopmentBuildUpFrame", "center", 0, 0 );
end

-- 宠物强化面板update
function Pet_DevelopmentBuildUpFrame_OnUpdate()
	--Pet_OperationFrame_OnUpdate();

	local nCurTime				= GameMgr:getTickTime();
	local t_operationSelPetInfo = GetOperationSelPetInfo();
	local t_alreadyResetList	= t_petStrengthenControl:getAllRequireResetLevel( t_operationSelPetInfo );
	
	--GameExchange:test( "Pet_DevelopmentBuildUpFrame_OnUpdate--table.getn( t_alreadyResetList ) = "..table.getn( t_alreadyResetList ).."--711" );
	--[[
	for _, oneConfig in ipairs( t_alreadyResetList ) do
		local tex			= getglobal( "Pet_DevelopmentBuildUpFrame_QiangHuaSmallStarBtn"..oneConfig["level"].."Tex" );
		local fLastAlpha	= tex:GetBlendAlpha();
		local fAlpha		= 0;
		local fChgAlpha		= ( nCurTime - oneConfig["lastTime"] ) * t_petStrengthenControl["AVERAGE_CHG_ALPHA"];
		if oneConfig["isOpaque"] then
			fAlpha = fLastAlpha + fChgAlpha;
		else
			fAlpha = fLastAlpha - fChgAlpha;
		end

		if fAlpha >= 1 then
			fAlpha = 1;
			oneConfig["isOpaque"] = false;
		elseif fAlpha <= 0 then
			fAlpha = 0;
			oneConfig["isOpaque"] = true;
		end
		
		oneConfig["lastTime"] = nCurTime;
		tex:SetBlendAlpha( fAlpha );
	end
	--]]
end

-- 宠物星级回退确认
local t_PetStrengthenConfirmStarBackFrameControl = { curStrthenLevel = 0 };

function GetPetStrengthenConfirmStarBackFrameControl()
	return t_PetStrengthenConfirmStarBackFrameControl;
end

t_PetStrengthenConfirmStarBackFrameControl["setCurStrthenLevel"] = 
function ( self, data )
	self["curStrthenLevel"] = data["curStrthenLevel"];
	self["heightId"]		= data["heightId"];
	self["lowId"]			= data["lowId"];
end

t_PetStrengthenConfirmStarBackFrameControl["getCurStrthenLevel"] = 
function ( self )
	return self["curStrthenLevel"];
end

t_PetStrengthenConfirmStarBackFrameControl["clearCurStrthenLevel"] = 
function ( self )
	self["curStrthenLevel"] = 0;
	self["heightId"]		= 0;
	self["lowId"]			= 0;
end

function PetStrengthenConfirmStarBackFrame_OnShow()
	local itemDef = getItemDef( CLIENT_RESTREN_ITEMID );
	if itemDef == nil then
		return;
	end

	this:SetPoint( "center", "UIClient", "center", 0, 0 );

	local t_fonts = {	
					[this:GetName().."BlackButtonItemNeedFont"]		= { value = "（需要消耗一个"..itemDef.Name.."）" }, 
					};
	util.SetFonts( t_fonts );
end

function PetStrengthenConfirmStarBackFrame_OnHide()
	t_PetStrengthenConfirmStarBackFrameControl:clearCurStrthenLevel();
end

function PetStrengthenConfirmStarBackFrame_OnClick()
	local mainplayer			= ActorMgr:getMainPlayer();
	local petPackage			= mainplayer:getPet();
	local nCurStrthenLevel		= t_PetStrengthenConfirmStarBackFrameControl:getCurStrthenLevel();
	local t_operationSelPetInfo = GetOperationSelPetInfo();
	local container				= mainplayer:getContainer();
	if container:getItemCount( CLIENT_RESTREN_ITEMID ) > 0 then
		petPackage:petResetStrengthen( t_PetStrengthenConfirmStarBackFrameControl["heightId"], t_PetStrengthenConfirmStarBackFrameControl["lowId"], 
										nCurStrthenLevel );
	else
		local itemDef = getItemDef( CLIENT_RESTREN_ITEMID );
		ShowMidTips( "缺少" .. itemDef.Name .. "，无法抹除强化星级" );
	end
	
	PetStrengthenConfirmStarBackFrame:Hide();
end

function GetPetResetStrthen( nHeightId, nLowId, nNewCanStrthenLevel )
	t_petStrengthenControl:addRequireResetLevel( { ["heightId"] = nHeightId, ["lowId"] = nLowId, ["level"] = nNewCanStrthenLevel });
	local t_operationSelPetInfo = GetOperationSelPetInfo();
	if t_operationSelPetInfo["heightId"] ~= nHeightId or t_operationSelPetInfo["lowId"] ~= nLowId then
		return;
	end

	t_petStrengthenControl:setCurSelLevel( { ["level"] = nNewCanStrthenLevel } );
	t_petStrengthenControl:setCurStrengthenLevel( { ["level"] = nNewCanStrthenLevel } );
	UpdatePet_DevelopmentBuildUpFrameStars();
end

function GetPetSuccessStrthen( nHeightId, nLowId, nOldResetLevel )
	if not Pet_DevelopmentBuildUpFrame:IsShown() then
		return;
	end

	t_petStrengthenControl:removeRequireResetLevel( { ["heightId"] = nHeightId, ["lowId"] = nLowId, ["level"] = nOldResetLevel });
	local t_operationSelPetInfo = GetOperationSelPetInfo();
	if t_operationSelPetInfo["heightId"] == nHeightId and t_operationSelPetInfo["lowId"] == nLowId and nOldResetLevel ~= 0 then
		local tex = getglobal( "Pet_DevelopmentBuildUpFrame_QiangHuaSmallStarBtn"..nOldResetLevel.."Tex" );
		tex:SetBlendAlpha( 1 );
	end

	local nCurSelectLevel = t_petStrengthenControl:getCurSelLevel();
	if nCurSelectLevel == nOldResetLevel then
		local mainplayer			= ActorMgr:getMainPlayer();
		local petPackage			= mainplayer:getPet();
		local petInfo				= petPackage:getPetInfo( t_operationSelPetInfo["heightId"], t_operationSelPetInfo["lowId"] );
		t_petStrengthenControl:setCurSelLevel( { ["level"] = petInfo.StrengthenLevel + 1 }  );
	end

	UpdatePet_DevelopmentBuildUpFrameStars();
	Change_Pet_DevelopmentBuildUpFrameFrameStrengBtnState();
end

--------------------------------------------宠物技能--------------------------------------------

local PetListOnePage = 5

function PetSystemCostItem_OnEnter()
	local id = this:GetClientUserData(0)
	local num = this:GetClientUserData(1)
	if id ~= 0 then
		local itemdef = getItemDef(id)
		if num > 0 then
			ShowItemTips(LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, itemdef)
		else
			SetGameTooltips(this:GetName(), string.format("请放入%s", itemdef.Name))
		end
	end
end

function PetSystemCostItem_OnLeave()
	ItemButton_OnLeave()
	GameTooltipRichText:SetText("", 255, 255, 255)
	GameTooltip:Hide()
end

function PetSkillList_OnEnter()
	ShowPetSkillTips(getglobal(this:GetParent()), petInfo, {
		SkillID = this:GetClientUserData(0),
		SkillLevel = this:GetClientUserData(1),
		GridType = this:GetClientUserData(2),
		}, this:GetName());
end

function PetSkillList_OnLeave()
	local t_showTips = {PetSkillTipsFrame, PetSkillGridTipsFrame}
	for _, frame in ipairs( t_showTips ) do
		frame:Hide()
	end
end

-- 重置 --

local PetSkillResetConfirm = {}
local ResetPetSkillStatus = {}
local SelectedPetOnPetSkillResetFrame

function InitPetSkillResetFrameData()
	PetSkillResetConfirm = {}
	ResetPetSkillStatus = {}
end

local function UpdatePetSkillResetButton()
	PetSkillResetFrameReset:Enable()
	if SelectedPetOnPetSkillResetFrame then
		local player = ActorMgr:getMainPlayer()
		local petMgr = player:getPet()
		local info = petMgr:getPetInfo(bytesint(SelectedPetOnPetSkillResetFrame))
		if not info or info.PetSkills.SkillGridNum <= 0 then
			PetSkillResetFrameReset:Disable()
			return
		end
	end
	local has = {}
	for id in pairs(ResetPetSkillStatus) do
		table.insert(has, id)
	end
	if #has > 0 then
		PetSkillResetFrameReset:Disable()
	end
end

local function UpdatePetSkillOnSkillResetFrame(info)
	if info then
		local player = ActorMgr:getMainPlayer()
		local petMgr = player:getPet()
		local locks = {}
		for i = 0, petMgr:getPetResetSkillLockNum(info) - 1 do
			locks[petMgr:getPetResetSkillLockIdx(info, i) + 1] = true
		end
		local state = petMgr:isPetResetSkillLockUseCoin(info)
		local skills	= info.PetSkills.SkillGrid
		local ui
		for i = 1, MAX_PET_SKILLS do
			if locks[i] then
				ui = getglobal("PetSkillResetFrameSkillList"..i.."Lock")
				if not ui:IsShown() then
					ui:Show()
				end
				ui = getglobal("PetSkillResetFrameSkillList"..i.."UnLock")
				ui:Hide()
				if state then
					ui = getglobal("PetSkillResetFrameSkillList"..i.."LockCert")
					ui:Hide()
					ui = getglobal("PetSkillResetFrameSkillList"..i.."LockCoin")
					if not ui:IsShown() then
						ui:Show()
					end
				else
					ui = getglobal("PetSkillResetFrameSkillList"..i.."LockCoin")
					ui:Hide()
					ui = getglobal("PetSkillResetFrameSkillList"..i.."LockCert")
					if not ui:IsShown() then
						ui:Show()
					end
				end
			else
				ui = getglobal("PetSkillResetFrameSkillList"..i.."Lock")
				ui:Hide()
				ui = getglobal("PetSkillResetFrameSkillList"..i.."UnLock")
				if not ui:IsShown() then
					ui:Show()
				end
				ui = getglobal("PetSkillResetFrameSkillList"..i.."LockCoin")
				ui:Hide()
				ui = getglobal("PetSkillResetFrameSkillList"..i.."LockCert")
				ui:Hide()
			end
			if i <= info.PetSkills.SkillGridNum then
				local id, level = skills[i - 1].SkillID, skills[i - 1].SkillLevel
				local diathesisDef = SkillMgr:getDiathesisDef(id, level)
				ui = getglobal("PetSkillResetFrameSkillList"..i.."Icon")
				if diathesisDef then
					local IconPath = getPetSkillPath()..diathesisDef.IconID..".tga"
					if not IsInExistence(IconPath) then
						IconPath = DEFAULT_ITEM_PATH
					end
					ui:SetTexture(IconPath)
				else
					ui:SetTexture("uires\\TuPianLei\\ChongWuJiNengLeiXing\\2.tga")
				end
				ui = getglobal("PetSkillResetFrameSkillList"..i.."Level")
				if level == 0 then
					ui:SetText("")
				else
					ui:SetText("lv"..level)
				end
				ui = getglobal("PetSkillResetFrameSkillList"..i)
				ui:SetClientUserData(0, id)
				ui:SetClientUserData(1, level)
				ui:SetClientUserData(2, skills[i - 1].GridType)
				ui:SetClientUserData(2, PET_SKILL_SELF_BUFF)
				ui:Enable()
			else
				ui = getglobal("PetSkillResetFrameSkillList"..i.."Icon")
				ui:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga")
				ui = getglobal("PetSkillResetFrameSkillList"..i.."Level")
				ui:SetText("")
				ui = getglobal("PetSkillResetFrameSkillList"..i)
				ui:SetClientUserData(0, 0)
				ui:SetClientUserData(1, 0)
				ui:SetClientUserData(2, PET_SKILL_DISABLE)
				ui:Disable()
				ui = getglobal("PetSkillResetFrameSkillList"..i.."UnLock")
				ui:Hide()
			end
		end
	else
		for i = 1, MAX_PET_SKILLS do
			ui = getglobal("PetSkillResetFrameSkillList"..i.."Icon")
			ui:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga")
			ui = getglobal("PetSkillResetFrameSkillList"..i.."Level")
			ui:SetText("")
			ui = getglobal("PetSkillResetFrameSkillList"..i)
			ui:SetClientUserData(0, 0)
			ui:SetClientUserData(1, 0)
			ui:SetClientUserData(2, PET_SKILL_DISABLE)
			ui:Disable()
			ui = getglobal("PetSkillResetFrameSkillList"..i.."Lock")
			ui:Hide()
			ui = getglobal("PetSkillResetFrameSkillList"..i.."UnLock")
			ui:Hide()
			ui = getglobal("PetSkillResetFrameSkillList"..i.."LockCoin")
			ui:Hide()
			ui = getglobal("PetSkillResetFrameSkillList"..i.."LockCert")
			ui:Hide()
		end
	end
	UpdatePetSkillResetButton()
end

local function UpdatePetSkillResetMoney()
	if SelectedPetOnPetSkillResetFrame then
		local player = ActorMgr:getMainPlayer()
		local petMgr = player:getPet()
		local info = petMgr:getPetInfo(bytesint(SelectedPetOnPetSkillResetFrame))
		if info then
			local num = petMgr:getPetResetSkillLockNum(info)
			if num == 0 then
				local money = petMgr:getPetResetSkillCostMoney()
				money = tostring(money)
				PetSkillResetFrameCostMoneyTitle:SetText("需要消耗")
				local width = PetSkillResetFrameCostMoney:GetTextExtentWidth(money)
				PetSkillResetFrameCostMoney:SetWidth(width)
				PetSkillResetFrameCostMoney:SetText(money)
				PetSkillResetFrameCostMoneyIcon:SetTexture("uires\\ZhuJieMian\\1.dds")
				PetSkillResetFrameCostMoneyIcon:SetTexUV(78, 494, 20, 18)
				PetSkillResetFrameCostMoneyIcon:SetSize(20, 18)
			else
				local usecoin = petMgr:isPetResetSkillLockUseCoin(info)
				local money = petMgr:getPetResetSkillCostMoney(num, usecoin)
				PetSkillResetFrameCostMoneyTitle:SetText(string.format("花费%s：", usecoin and MONEY_TYPE_BIND_GODCOIN_NAME or MONEY_TYPE_GODCOIN_NAME))
				local width = PetSkillResetFrameCostMoney:GetTextExtentWidth(money)
				PetSkillResetFrameCostMoney:SetWidth(width)
				PetSkillResetFrameCostMoney:SetText(money)
				if usecoin then
					PetSkillResetFrameCostMoneyIcon:SetTexture("uires\\ZhuJieMian\\1.dds")
					PetSkillResetFrameCostMoneyIcon:SetTexUV(57, 494, 20, 18)
					PetSkillResetFrameCostMoneyIcon:SetSize(20, 18)
				else
					PetSkillResetFrameCostMoneyIcon:SetTexture("uires\\ZhuJieMian\\1.dds")
					PetSkillResetFrameCostMoneyIcon:SetTexUV(99, 494, 17, 18)
					PetSkillResetFrameCostMoneyIcon:SetSize(17, 18)
				end
			end
		end
	end
end

local function UpdatePetListPageOnPetSkillResetFrame()
	local value = PetSkillResetFrameScrollBar:GetValue()
	local player = ActorMgr:getMainPlayer()
	local petMgr = player:getPet()
	local btn
	for i = 1, PetListOnePage do
		local info = petMgr:getPetInfo(value + i - 1)
		btn = getglobal("PetSkillResetFramePetListName"..i)
		if info then
			local id = intbytes(info.WID.nHeight, info.WID.nLow)
			btn:SetText(info.Name)
			if id == SelectedPetOnPetSkillResetFrame then
				btn:Checked()
			else
				btn:DisChecked()
			end
		else
			btn:SetText("")
			btn:DisChecked()
		end
	end
end

local function UpdatePetsOnPetSkillResetFrame()
	local player = ActorMgr:getMainPlayer()
	local petMgr = player:getPet()
	local num = petMgr:getPetNum()
	local step = num - PetListOnePage
	local slider = PetSkillResetFrameScrollBar
	if step > 0 then
		slider:Show()
		if slider:GetValue() > step then
			slider:SetValue(step)
		end
		slider:SetMaxValue(step)
	else
		slider:Hide()
		slider:SetValue(0)
	end
	PetSkillResetFramePetListTitle:SetText(string.format("宠物列表 %d/%d", num, petMgr:getPetMaxNum()))
	local id
	for i = 0, num - 1 do
		local info = petMgr:getPetInfo(i)
		if info then
			id = intbytes(info.WID.nHeight, info.WID.nLow)
			if id == SelectedPetOnPetSkillResetFrame then
				break
			end
			id = nil
		end
	end
	SelectedPetOnPetSkillResetFrame = id
	UpdatePetListPageOnPetSkillResetFrame()
	if SelectedPetOnPetSkillResetFrame then
		local player = ActorMgr:getMainPlayer()
		local petMgr = player:getPet()
		local info = petMgr:getPetInfo(bytesint(SelectedPetOnPetSkillResetFrame))
		UpdatePetSkillOnSkillResetFrame(info)
		UpdatePetSkillResetMoney()
	else
		SelectPetOnPetSkillResetFrame(1)
	end
end

local function UpdatePetSkillResetItem()
	local MainPlayer = ActorMgr:getMainPlayer()
	local container = MainPlayer:getContainer()
	local num = container:getItemCount(PET_SKILL_RESET_ITEM)
	if num > 0 then
		PetSkillResetFrameCostItemIcon:SetTexture(GetItemIconPathByID(PET_SKILL_RESET_ITEM))
		PetSkillResetFrameCostItemCount:Show()
	else
		PetSkillResetFrameCostItemIcon:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga")
		PetSkillResetFrameCostItemCount:Hide()
	end
	PetSkillResetFrameCostItem:SetClientUserData(1, num)
	if 1 > num then
		PetSkillResetFrameCostItemCount:SetTextColor(255, 0, 0)
	else
		PetSkillResetFrameCostItemCount:SetTextColor(255, 255, 190)
	end
	PetSkillResetFrameCostItemCount:SetText(string.format("%s/%s", num, 1))
	for i = 0, container:getContainerMaxNum(CONTAINER_TYPE_SUNDRIES) - 1 do
		local item = container:getItem(CONTAINER_TYPE_SUNDRIES, i)
		if item:getItemId() == PET_SKILL_RESET_ITEM then
			LockItem(CONTAINER_TYPE_SUNDRIES, i, PET_SKILL_RESET_ITEM, "PetSkillResetFrame")
		end
	end
end

function SelectPetOnPetSkillResetFrame(i)
	if i then
		local player = ActorMgr:getMainPlayer()
		local petMgr = player:getPet()
		local info = petMgr:getPetInfo(i - 1)
		local id = intbytes(info.WID.nHeight, info.WID.nLow)
		if id ~= SelectedPetOnPetSkillResetFrame then
			SelectedPetOnPetSkillResetFrame = id
			PetSkillResetFrameName:SetText(info.Name)
			local PetDef = getPetDef(info.PetID)
			PetSkillResetFramePhoto:SetTexture(GetMonsterIconPath(PetDef.IconID))
			UpdatePetSkillOnSkillResetFrame(info)
			UpdatePetListPageOnPetSkillResetFrame()
			UpdatePetSkillResetMoney()
		end
	else
		if SelectedPetOnPetSkillResetFrame then
			SelectedPetOnPetSkillResetFrame = nil
			PetSkillResetFrameName:SetText("")
			PetSkillResetFramePhoto:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga")
			UpdatePetSkillOnSkillResetFrame()
			UpdatePetListPageOnPetSkillResetFrame()
		end
	end
	local bool, func = IsMessageBoxBlock(PetSkillResetFrame)
	if bool then
		if not func or not func() then
			MessageBoxBlockEnd(PetSkillResetFrame)
		end
	end
end

function PetSkillResetFrame_OnLoad()
	this:RegisterEvent("GE_CONTAINER_UPDATE")
	this:RegisterEvent("GE_CHANGE_MONEY")
	this:RegisterEvent("GE_PET_INFO_CHG")
	this:RegisterEvent("GE_DEL_PET")
	this:RegisterEvent("GE_PET_ATTR_CHG")
	PetSkillResetFrameCostItem:SetClientUserData(0, PET_SKILL_RESET_ITEM)
	PetSkillResetFrameCostItemName:SetText(getItemDef(PET_SKILL_RESET_ITEM).Name)
	ItemFirstCallBack(this:GetName(), function(item)
		if item:getItemId() ~= PET_SKILL_RESET_ITEM then
			ShowMidTips("该道具不能用于重置宠物技能")
		end
	end)
end

function PetSkillResetFrame_OnEvent()
	if arg1 == "GE_CONTAINER_UPDATE" then
		if this:IsShown() then
			UpdatePetSkillResetItem()
		end
	elseif arg1 == "GE_CHANGE_MONEY" then
		if this:IsShown() then
			UpdatePetSkillResetMoney()
		end
	elseif arg1 == "GE_PET_INFO_CHG" or arg1 == "GE_DEL_PET" or arg1 == "GE_PET_ATTR_CHG" then
		if this:IsShown() then
			local player = ActorMgr:getMainPlayer()
			local petMgr = player:getPet()
			if petMgr:getPetNum() == 0 then
				ShowMidTips( "身上没宠物，不能进行该操作，自动关闭面板" );
				this:Hide()
				PetFrame:Hide();
			else
				UpdatePetsOnPetSkillResetFrame()
			end
		end
	end
end

function PetSkillResetFrame_OnShow()
	PetSkillResetConfirm = {}
	SelectPetOnPetSkillResetFrame()
	PetSkillResetFrameScrollBar:SetValue(0)
	UpdatePetsOnPetSkillResetFrame()
	UpdatePetSkillResetItem()
	if not PetFrame:IsShown() then
		PetFrame:Show()
	end
	local player = ActorMgr:getMainPlayer()
	local petMgr = player:getPet()
	if petMgr:getPetNum() > 0 then
		SelectPetOnPetSkillResetFrame(1)
		PetListButtonSelect(0)
	end
	PetFrame:SetPoint("topright", "UIClient", "topleft", 428, 163)
	this:SetPoint("topleft", "UIClient", "topleft", 428, 163)
	local modelview = getglobal(this:GetName().."ModelView")
	modelview:Hide()
end

function PetSkillResetFrame_OnHide()
	ClearItemLock("PetSkillResetFrame")
end

function PetSkillResetFrame_ScrollBarValueChange()
	UpdatePetListPageOnPetSkillResetFrame()
end

function PetSkillResetFrame_ScrollUpBtn()
	local slider = PetSkillResetFrameScrollBar
	if not slider:IsShown() then
		return
	end
	local nValue = slider:GetValue() - slider:GetValueStep()
	if nValue >= slider:GetMinValue() then
		slider:SetValue(nValue)
	end
end

function PetSkillResetFrame_ScrollDownBtn()
	local slider = PetSkillResetFrameScrollBar
	if not slider:IsShown() then
		return
	end
	local nValue	= slider:GetValue() + slider:GetValueStep()
	if nValue <= slider:GetMaxValue() then
		slider:SetValue(nValue)
	end
end

local function LockPetSkillConfirm(id, idx)
	local player = ActorMgr:getMainPlayer()
	local petMgr = player:getPet()
	local info = petMgr:getPetInfo(bytesint(id))
	local result
	if petMgr:getPetResetSkillLockNum(info) == 0 then
		PetSkillResetConfirm[id] = true
		result = ConfirmCoinCertBoxBlock("宠物技能格锁定", "锁定技能格子，每次重置宠物技能时将消耗金币或者金券。\n请选择消耗类型：", PetSkillResetFrame)
	else
		result = petMgr:isPetResetSkillLockUseCoin(info) and "coin" or "cert"
	end
	if result then
		local high, low = bytesint(id)
		petMgr:LockPetSkill(high, low, idx - 1, result == "coin")
	end
end

local function ResetPetSkillConfirm(id)
	ClearItemLock("PetSkillResetFrame")
	local lock
	if IsItemLock(0, 0, PET_SKILL_RESET_ITEM) then
		lock = true
	end
	local player = ActorMgr:getMainPlayer()
	local container = player:getContainer()
	local bFound = false;
	for i = 0, container:getContainerMaxNum(CONTAINER_TYPE_SUNDRIES) - 1 do
		local item = container:getItem(CONTAINER_TYPE_SUNDRIES, i)
		if item:getItemId() == PET_SKILL_RESET_ITEM then
			LockItem(CONTAINER_TYPE_SUNDRIES, i, PET_SKILL_RESET_ITEM, "PetSkillResetFrame")
			bFound = true;
		end
	end
	if lock then
		return ShowMidTips(string.format("%s已被锁定，无法重置宠物技能", getItemDef(PET_SKILL_RESET_ITEM).Name))
	end
	if not bFound then
		MessageBoxForItem("提示",getItemDef(PET_SKILL_RESET_ITEM).Name);
		MessageBoxForItemFrame:SetClientUserData(0,PET_SKILL_RESET_ITEM);
		return;
	end

	local petMgr = player:getPet()
	local info = petMgr:getPetInfo(bytesint(id))
	local usecoin = petMgr:isPetResetSkillLockUseCoin(info)
	local num = petMgr:getPetResetSkillLockNum(info)
	if num > 0 and not PetSkillResetConfirm[id] then
		local skills = {}
		local PetSkill = info.PetSkills.SkillGrid
		for i = 0, num - 1 do
			local idx = petMgr:getPetResetSkillLockIdx(info, i)
			local diathesisDef = SkillMgr:getDiathesisDef(PetSkill[idx].SkillID, PetSkill[idx].SkillLevel)
			table.insert(skills, diathesisDef.Name)
		end
		local money = petMgr:getPetResetSkillCostMoney(num, usecoin)
		if not MessageBoxBlock("宠物技能重置确认", string.format("#cFFFFBE您已经锁定了%s的%s技能，重置时将消耗%d%s，确认重置吗？#n", info.Name, table.concat(skills, "、"), money, usecoin and MONEY_TYPE_BIND_GODCOIN_NAME or MONEY_TYPE_GODCOIN_NAME), PetSkillResetFrame, function() return SelectedPetOnPetSkillResetFrame == id end) then
			return
		end
		PetSkillResetConfirm[id] = true
	end

	if num == 0 then
		local money = petMgr:getPetResetSkillCostMoney();
		if getPlayerMoney( player ) < money then
			ShowMidTips( "绑银不足，无法重置" );
			return;
		end

		if checkNeedWarnCost( money ) then
			PetSkillResetFrameReset:SetClientString( id );
			CostMoneyConfirmFrame:SetClientString("宠物技能重置");
			SetWarnFrame( "PetSkillResetFrame" );
			CostMoneyConfirmFrame:Show();
			return;
		end
	end

	ResetPetSkillStatus[id] = true
	UpdatePetSkillResetButton()
	petMgr:ResetPetSkill(bytesint(id))
end

function SetPetSkillResetState( id )
	ResetPetSkillStatus[id] = true;
	UpdatePetSkillResetButton();
end

function PetSkillResetSkillListUnLock_OnEnter()
	SetGameTooltips(this:GetName(), "点击锁定技能，锁定后该技能将不被重置")
end

function PetSkillResetSkillListLock_OnClick()
	if IsMessageBoxBlock(PetSkillResetFrame) then
		return
	end
	local parent = getglobal(this:GetParent())
	local idx = parent:GetClientID()
	coroutine.wrap(LockPetSkillConfirm)(SelectedPetOnPetSkillResetFrame, idx)
end

function PetSkillResetSkillListLockCoin_OnEnter()
	SetGameTooltips(this:GetName(), "点击解锁技能，解锁后可对该技能进行重置")
end

function PetSkillResetSkillListLockCert_OnEnter()
	SetGameTooltips(this:GetName(), "点击解锁技能，解锁后可对该技能进行重置")
end

function PetSkillResetSkillListUnLock_OnClick()
	if IsMessageBoxBlock(PetSkillResetFrame) then
		return
	end
	local parent = getglobal(this:GetParent())
	local idx = parent:GetClientID()
	local high, low = bytesint(SelectedPetOnPetSkillResetFrame)
	local player = ActorMgr:getMainPlayer()
	local petMgr = player:getPet()
	petMgr:UnLockPetSkill(high, low, idx - 1)
end

function PetSkillResetFrame_OnReset()
	if CostMoneyConfirmFrame:IsShown() then
		return;
	end
	if IsMessageBoxBlock(PetSkillResetFrame) then
		return
	end
	coroutine.wrap(ResetPetSkillConfirm)(SelectedPetOnPetSkillResetFrame)
end

function PetSkillResetPetListName_OnClick()
	if IsMessageBoxBlock(PetSkillResetFrame) then
		return
	end

	local i = this:GetClientID() + PetSkillResetFrameScrollBar:GetValue()
	local player = ActorMgr:getMainPlayer()
	local petMgr = player:getPet()
	local info = petMgr:getPetInfo(i - 1)
	if info then
		SelectPetOnPetSkillResetFrame(i)
		if PetFrame:IsShown() then
			PetListButtonSelect(i - 1)
		end
	end
end

function PetSkillOnReset(high, low)
	if not PetSkillResetFrame:IsShown() then
		return
	end
	ResetPetSkillStatus = {}
	if high and low then
		local id = intbytes(high, low)
		
		local modelview = PetSkillResetFrameModelView
		modelview:setPetModel(0, 0, "effect\\entity\\fumo01.ent")
		modelview:setEntityPosition(0, 0, 100, 0)
		if not modelview:IsShown() then
			modelview:Show()
		end
	end
	UpdatePetSkillResetButton()
end

-- 升级 --

local SelectedPetSkillIdx
local SelectedPetOnPetSkillLevelUpFrame

local function UpdatePetSkillOnSkillLevelUpFrame(info)
	if info then
		local player = ActorMgr:getMainPlayer()
		local petMgr = player:getPet()
		local skills = info.PetSkills.SkillGrid
		local ui
		for i = 1, MAX_PET_SKILLS do
			if i <= info.PetSkills.SkillGridNum then
				local id, level = skills[i - 1].SkillID, skills[i - 1].SkillLevel
				local diathesisDef = SkillMgr:getDiathesisDef(id, level)
				ui = getglobal("PetSkillLevelUpFrameSkillList"..i.."Icon")
				if diathesisDef then
					local IconPath = getPetSkillPath()..diathesisDef.IconID..".tga"
					if not IsInExistence(IconPath) then
						IconPath = DEFAULT_ITEM_PATH
					end
					ui:SetTexture(IconPath)
				else
					ui:SetTexture("uires\\TuPianLei\\ChongWuJiNengLeiXing\\2.tga")
				end
				ui = getglobal("PetSkillLevelUpFrameSkillList"..i.."Level")
				if level == 0 then
					ui:SetText("")
				else
					ui:SetText("lv"..level)
				end
				ui = getglobal("PetSkillLevelUpFrameSkillList"..i)
				ui:SetClientUserData(0, id)
				ui:SetClientUserData(1, level)
				ui:SetClientUserData(2, skills[i - 1].GridType)
				ui:Enable()
			else
				ui = getglobal("PetSkillLevelUpFrameSkillList"..i.."Icon")
				ui:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga")
				ui = getglobal("PetSkillLevelUpFrameSkillList"..i.."Level")
				ui:SetText("")
				ui = getglobal("PetSkillLevelUpFrameSkillList"..i)
				ui:SetClientUserData(0, 0)
				ui:SetClientUserData(1, 0)
				ui:SetClientUserData(2, PET_SKILL_DISABLE)
				ui:Disable()
			end
		end
	else
		for i = 1, MAX_PET_SKILLS do
			ui = getglobal("PetSkillLevelUpFrameSkillList"..i.."Icon")
			ui:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga")
			ui = getglobal("PetSkillLevelUpFrameSkillList"..i.."Level")
			ui:SetText("")
			ui = getglobal("PetSkillLevelUpFrameSkillList"..i)
			ui:SetClientUserData(0, 0)
			ui:SetClientUserData(1, 0)
			ui:SetClientUserData(2, PET_SKILL_DISABLE)
			ui:Disable()
		end
	end
	if info and info.PetSkills.SkillGridNum > 0 then
		PetSkillLevelUpFrameLevelUp:Enable()
	else
		PetSkillLevelUpFrameLevelUp:Disable()
	end
end

local function UpdatePetSkillLevelUpMoney()
	if SelectedPetOnPetSkillLevelUpFrame then
		if SelectedPetSkillIdx then
			local ui = getglobal("PetSkillLevelUpFrameSkillList"..SelectedPetSkillIdx)
			local id = ui:GetClientUserData(0)
			local level = ui:GetClientUserData(1)
			local player = ActorMgr:getMainPlayer()
			local petMgr = player:getPet()
			if (id == 0 and level == 0) or not petMgr:isPetSkillCanLevelUp(id, level) then
				PetSkillLevelUpFrameCostMoney:SetText("")
			else
				local money = ( level + 1 ) * 1000;
				money = tostring(money)
				local width = PetSkillLevelUpFrameCostMoney:GetTextExtentWidth(money)
				PetSkillLevelUpFrameCostMoney:SetWidth(width)
				PetSkillLevelUpFrameCostMoney:SetText(money)
				PetSkillLevelUpFrameCostMoneyIcon:SetTexture("uires\\ZhuJieMian\\1.dds")
				PetSkillLevelUpFrameCostMoneyIcon:SetTexUV(78, 494, 20, 18)
				PetSkillLevelUpFrameCostMoneyIcon:SetSize(20, 18)
			end
		else
			PetSkillLevelUpFrameCostMoney:SetText("")
		end
	end
end

local function UpdatePetSkillLevelUpItem()
	local player = ActorMgr:getMainPlayer()
	local container = player:getContainer()
	local petMgr = player:getPet()
	local num = container:getItemCount(PET_SKILL_LEVELUP_ITEM)
	if num > 0 then
		PetSkillLevelUpFrameCostItemIcon:SetTexture(GetItemIconPathByID(PET_SKILL_LEVELUP_ITEM))
		PetSkillLevelUpFrameCostItemCount:Show()
	else
		PetSkillLevelUpFrameCostItemIcon:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga")
		PetSkillLevelUpFrameCostItemCount:Hide()
	end
	PetSkillLevelUpFrameCostItem:SetClientUserData(1, num)
	if SelectedPetOnPetSkillLevelUpFrame and SelectedPetSkillIdx then
		local ui = getglobal("PetSkillLevelUpFrameSkillList"..SelectedPetSkillIdx)
		local id = ui:GetClientUserData(0)
		local level = ui:GetClientUserData(1)
		if petMgr:isPetSkillCanLevelUp(id, level) then
			local cost = petMgr:getPetSkillLevelUpCostItemCount(id, level)
			if cost > num then
				PetSkillLevelUpFrameCostItemCount:SetTextColor(255, 0, 0)
			else
				PetSkillLevelUpFrameCostItemCount:SetTextColor(255, 255, 190)
			end
			PetSkillLevelUpFrameCostItemCount:SetText(string.format("%s/%s", num, cost))
		else
			PetSkillLevelUpFrameCostItemCount:SetText("")
		end
	else
		PetSkillLevelUpFrameCostItemCount:SetText("")
	end
	for i = 0, container:getContainerMaxNum(CONTAINER_TYPE_SUNDRIES) - 1 do
		local item = container:getItem(CONTAINER_TYPE_SUNDRIES, i)
		if item:getItemId() == PET_SKILL_LEVELUP_ITEM then
			LockItem(CONTAINER_TYPE_SUNDRIES, i, PET_SKILL_LEVELUP_ITEM, "PetSkillLevelUpFrame")
		end
	end
end

local function UpdatePetListPageOnPetSkillLevelUpFrame()
	local value = PetSkillLevelUpFrameScrollBar:GetValue()
	local player = ActorMgr:getMainPlayer()
	local petMgr = player:getPet()
	local btn
	for i = 1, PetListOnePage do
		local info = petMgr:getPetInfo(value + i - 1)
		btn = getglobal("PetSkillLevelUpFramePetListName"..i)
		if info then
			local id = intbytes(info.WID.nHeight, info.WID.nLow)
			btn:SetText(info.Name)
			if id == SelectedPetOnPetSkillLevelUpFrame then
				btn:Checked()
			else
				btn:DisChecked()
			end
		else
			btn:SetText("")
			btn:DisChecked()
		end
	end
end

local function UpdatePetsOnPetSkillLevelUpFrame()
	local player = ActorMgr:getMainPlayer()
	local petMgr = player:getPet()
	local num = petMgr:getPetNum()
	local step = num - PetListOnePage
	local slider = PetSkillLevelUpFrameScrollBar
	if step > 0 then
		slider:Show()
		if slider:GetValue() > step then
			slider:SetValue(step)
		end
		slider:SetMaxValue(step)
	else
		slider:Hide()
		slider:SetValue(0)
	end
	PetSkillLevelUpFramePetListTitle:SetText(string.format("宠物列表 %d/%d", num, petMgr:getPetMaxNum()))
	local id
	for i = 0, num - 1 do
		local info = petMgr:getPetInfo(i)
		if info then
			id = intbytes(info.WID.nHeight, info.WID.nLow)
			if id == SelectedPetOnPetSkillLevelUpFrame then
				break
			end
			id = nil
		end
	end
	SelectedPetOnPetSkillLevelUpFrame = id
	UpdatePetListPageOnPetSkillLevelUpFrame()
	if SelectedPetOnPetSkillLevelUpFrame then
		local player = ActorMgr:getMainPlayer()
		local petMgr = player:getPet()
		local info = petMgr:getPetInfo(bytesint(SelectedPetOnPetSkillLevelUpFrame))
		UpdatePetSkillOnSkillLevelUpFrame(info)
		UpdatePetSkillLevelUpMoney()
		UpdatePetSkillLevelUpItem()
	else
		SelectPetOnPetSkillLevelUpFrame(1)
	end
end

local function SelectPetSkillOnPetSkillLevelUpFrame(i)
	local ui
	if SelectedPetSkillIdx then
		ui = getglobal("PetSkillLevelUpFrameSkillList"..SelectedPetSkillIdx)
		ui:DisChecked()
	end
	SelectedPetSkillIdx = i
	if SelectedPetSkillIdx then
		ui = getglobal("PetSkillLevelUpFrameSkillList"..SelectedPetSkillIdx)
		ui:Checked()
	end
	PetSkillLevelUpFrameLevelUp:Enable()
	if SelectedPetSkillIdx then
		local ui = getglobal("PetSkillLevelUpFrameSkillList"..SelectedPetSkillIdx)
		local id = ui:GetClientUserData(0)
		local level = ui:GetClientUserData(1)
		if id == 0 or level == 0 then
			PetSkillLevelUpFrameLevelUp:Disable()
		end
	end
	UpdatePetSkillLevelUpMoney()
	UpdatePetSkillLevelUpItem()
end

function SelectPetOnPetSkillLevelUpFrame(i)
	SelectPetSkillOnPetSkillLevelUpFrame()
	if i then
		local player = ActorMgr:getMainPlayer()
		local petMgr = player:getPet()
		local info = petMgr:getPetInfo(i - 1)
		local id = intbytes(info.WID.nHeight, info.WID.nLow)
		if id ~= SelectedPetOnPetSkillLevelUpFrame then
			SelectedPetOnPetSkillLevelUpFrame = id
			PetSkillLevelUpFrameName:SetText(info.Name)
			local PetDef = getPetDef(info.PetID)
			PetSkillLevelUpFramePhoto:SetTexture(GetMonsterIconPath(PetDef.IconID))
			UpdatePetSkillOnSkillLevelUpFrame(info)
			UpdatePetListPageOnPetSkillLevelUpFrame()
			UpdatePetSkillLevelUpMoney()
			UpdatePetSkillLevelUpItem()
		end
	else
		if SelectedPetOnPetSkillLevelUpFrame then
			SelectedPetOnPetSkillLevelUpFrame = nil
			PetSkillLevelUpFrameName:SetText("")
			PetSkillLevelUpFramePhoto:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga")
			UpdatePetSkillOnSkillLevelUpFrame()
			UpdatePetListPageOnPetSkillLevelUpFrame()
		end
	end
end

function PetSkillLevelUpFrame_OnLoad()
	this:RegisterEvent("GE_CONTAINER_UPDATE")
	this:RegisterEvent("GE_CHANGE_MONEY")
	this:RegisterEvent("GE_PET_INFO_CHG")
	this:RegisterEvent("GE_DEL_PET")
	this:RegisterEvent("GE_PET_ATTR_CHG")
	PetSkillLevelUpFrameCostItem:SetClientUserData(0, PET_SKILL_LEVELUP_ITEM)
	PetSkillLevelUpFrameCostItemName:SetText(getItemDef(PET_SKILL_LEVELUP_ITEM).Name)
	ItemFirstCallBack(this:GetName(), function(item)
		if item:getItemId() ~= PET_SKILL_LEVELUP_ITEM then
			ShowMidTips("该道具不能用于升级宠物技能")
		end
	end)
end

function PetSkillLevelUpFrame_OnEvent()
	if arg1 == "GE_CONTAINER_UPDATE" then
		if this:IsShown() then
			UpdatePetSkillLevelUpItem()
		end
	elseif arg1 == "GE_CHANGE_MONEY" then
		if this:IsShown() then
			UpdatePetSkillLevelUpMoney()
		end
	elseif arg1 == "GE_PET_INFO_CHG" or arg1 == "GE_DEL_PET" or arg1 == "GE_PET_ATTR_CHG" then
		if this:IsShown() then
			local player = ActorMgr:getMainPlayer()
			local petMgr = player:getPet()
			if petMgr:getPetNum() == 0 then
				ShowMidTips( "身上没宠物，不能进行该操作，自动关闭面板" );
				this:Hide()
				PetFrame:Hide();
			else
				UpdatePetsOnPetSkillLevelUpFrame()
			end
		end
	end
end

function PetSkillLevelUpFrame_OnShow()
	SelectPetOnPetSkillLevelUpFrame()
	PetSkillLevelUpFrameScrollBar:SetValue(0)
	UpdatePetsOnPetSkillLevelUpFrame()
	if not PetFrame:IsShown() then
		PetFrame:Show()
	end
	local player = ActorMgr:getMainPlayer()
	local petMgr = player:getPet()
	if petMgr:getPetNum() > 0 then
		SelectPetOnPetSkillLevelUpFrame(1)
		PetListButtonSelect(0)
	end
	PetFrame:SetPoint("topright", "UIClient", "topleft", 428, 163)
	this:SetPoint("topleft", "UIClient", "topleft", 428, 163)
	local modelview = getglobal(this:GetName().."ModelView")
	modelview:Hide()
end

function PetSkillLevelUpFrame_OnHide()
	ClearItemLock("PetSkillLevelUpFrame")
end

function PetSkillLevelUpFrame_ScrollBarValueChange()
	UpdatePetListPageOnPetSkillLevelUpFrame()
end

function PetSkillLevelUpFrame_ScrollUpBtn()
	local slider = PetSkillLevelUpFrameScrollBar
	if not slider:IsShown() then
		return
	end
	local nValue = slider:GetValue() - slider:GetValueStep()
	if nValue >= slider:GetMinValue() then
		slider:SetValue(nValue)
	end
end

function PetSkillLevelUpFrame_ScrollDownBtn()
	local slider = PetSkillLevelUpFrameScrollBar
	if not slider:IsShown() then
		return
	end
	local nValue	= slider:GetValue() + slider:GetValueStep()
	if nValue <= slider:GetMaxValue() then
		slider:SetValue(nValue)
	end
end

local function PetSkillLevelUpConfirm(id)
	local player = ActorMgr:getMainPlayer()
	local petMgr = player:getPet()
	local idx = SelectedPetSkillIdx
	if not idx then
		return ShowMidTips("请先点击选择需要升级的技能")
	end
	ClearItemLock("PetSkillLevelUpFrame")
	local lock
	if IsItemLock(0, 0, PET_SKILL_LEVELUP_ITEM) then
		lock = true
	end
	local player = ActorMgr:getMainPlayer()
	local container = player:getContainer()
	local bFound = false;
	for i = 0, container:getContainerMaxNum(CONTAINER_TYPE_SUNDRIES) - 1 do
		local item = container:getItem(CONTAINER_TYPE_SUNDRIES, i)
		if item:getItemId() == PET_SKILL_LEVELUP_ITEM then
			LockItem(CONTAINER_TYPE_SUNDRIES, i, PET_SKILL_LEVELUP_ITEM, "PetSkillLevelUpFrame")
			bFound = true;
		end
	end
	if lock then
		return ShowMidTips(string.format("%s已被锁定，无法升级宠物技能", getItemDef(PET_SKILL_LEVELUP_ITEM).Name))
	end

	if not bFound then
		MessageBoxForItem("提示",getItemDef(PET_SKILL_LEVELUP_ITEM).Name);
		MessageBoxForItemFrame:SetClientUserData(0,PET_SKILL_LEVELUP_ITEM);
		return;
	end
	local high, low = bytesint(id)
	
	local petInfo = petMgr:getPetInfo( high,low );
	if petMgr:isExtraClanJiYangPet( petInfo ) then 
		ShowMidTips( "宠物正在寄养中，请前往公会城市取消寄养再进行该操作！" );
		return;
	end
	local money = tonumber( PetSkillLevelUpFrameCostMoney:GetText() );
	if money ~= nil and money > 0 then
		if getPlayerMoney( player ) < money then
			ShowMidTips( "绑银不足，无法升级" );
			return;
		end

		if checkNeedWarnCost( money ) then
			CostMoneyConfirmFrame:SetClientString("宠物技能升级");
			CostMoneyConfirmFrame:SetClientUserData( 0, high );
			CostMoneyConfirmFrame:SetClientUserData( 1, low );
			CostMoneyConfirmFrame:SetClientUserData( 2, idx - 1 );
			SetWarnFrame( "PetSkillLevelUpFrame" );
			CostMoneyConfirmFrame:Show();
			return;
		end
	end

	petMgr:LevelUpPetSkill(high, low, idx - 1)
end

function PetSkillLevelUpFrame_OnLevelUp()
	if CostMoneyConfirmFrame:IsShown() then
		return;
	end
	coroutine.wrap(PetSkillLevelUpConfirm)(SelectedPetOnPetSkillLevelUpFrame)
end

function PetSkillLevelUpSkillList_OnClick()
	SelectPetSkillOnPetSkillLevelUpFrame(this:GetClientID())
end

function PetSkillLevelUpPetListName_OnClick()
	local i = this:GetClientID() + PetSkillLevelUpFrameScrollBar:GetValue()
	local player = ActorMgr:getMainPlayer()
	local petMgr = player:getPet()
	local info = petMgr:getPetInfo(i - 1)
	if info then
		SelectPetOnPetSkillLevelUpFrame(i)
		if PetFrame:IsShown() then
			PetListButtonSelect(i - 1)
		end
	end
end

-- 幻化 --

local PetGrowResetLockInfo
local SelectedPetOnPetGrowResetFrame
local ResetPetStatus = {}
local ResetConfirmGrow = {}

local PetGrowResetLockMap = {
	--["PetGrowResetFrameAllGrowHook"]	= PET_RESET_LOCK_GENIUS,
	["PetGrowResetFrameAllAttackHook"]	= PET_RESET_LOCK_ATK_GENIUS,
	["PetGrowResetFrameAllHPHook"]		= PET_RESET_LOCK_HP_GENIUS,
	["PetGrowResetFrameAllDefendHook"]	= PET_RESET_LOCK_PHSIC_DEF_GENIUS,
	["PetGrowResetFrameAllResistHook"]	= PET_RESET_LOCK_ICE_DEF_GENIUS,
	["PetGrowResetFrameAllExactHook"]	= PET_RESET_LOCK_HITRATE_GENIUS,
	["PetGrowResetFrameAllDodgeHook"]	= PET_RESET_LOCK_FLEE_GENIUS,
}

local AttrType = {
		Attack = "#c00dcdc攻击#n", HP = "#c00dcdc生命#n", Defend = "#c00dcdc物防#n", Resist = "#c00dcdc魔防#n", Exact = "#c00dcdc命中#n", Dodge = "#c00dcdc闪避#n"
}

local AttrOrder = {
	"资质", "攻击", "生命", "物防", "魔防", "命中", "闪避"
}

do
	local t = AttrOrder
	AttrOrder = {}
	for index, attr in ipairs(t) do
		AttrOrder[attr] = index
	end
end

local AttrHook = {
	Attack	= "PetGrowResetFrameAllAttackHook",
	HP		= "PetGrowResetFrameAllHPHook",
	Defend	= "PetGrowResetFrameAllDefendHook",
	Resist	= "PetGrowResetFrameAllResistHook",
	Exact	= "PetGrowResetFrameAllExactHook",
	Dodge	= "PetGrowResetFrameAllDodgeHook",
}

local LockType, LockNum

local function UpdatePetGrowResetMoney()
	local player = ActorMgr:getMainPlayer()
	local petMgr = player:getPet()
	local high, low = bytesint(SelectedPetOnPetGrowResetFrame)
	if LockType then
		local usecoin = (LockType == "coin")
		PetGrowResetFrameCostMoney:SetText(petMgr:getPetGrowResetCostMoney(high, low, LockNum, usecoin))
		PetGrowResetFrameCostMoneyTitle:SetText(string.format("花费%s：", usecoin and MONEY_TYPE_BIND_GODCOIN_NAME or MONEY_TYPE_GODCOIN_NAME))
		if usecoin then
			PetGrowResetFrameCostMoneyIcon:SetTexture("uires\\ZhuJieMian\\1.dds")
			PetGrowResetFrameCostMoneyIcon:SetTexUV(57, 494, 20, 18)
			PetGrowResetFrameCostMoneyIcon:SetSize(20, 18)
		else
			PetGrowResetFrameCostMoneyIcon:SetTexture("uires\\ZhuJieMian\\1.dds")
			PetGrowResetFrameCostMoneyIcon:SetTexUV(99, 494, 17, 18)
			PetGrowResetFrameCostMoneyIcon:SetSize(17, 18)
		end
	else
		PetGrowResetFrameCostMoney:SetText(petMgr:getPetGrowResetCostMoney(high, low))
		PetGrowResetFrameCostMoneyTitle:SetText("需要消耗")
		PetGrowResetFrameCostMoneyIcon:SetTexture("uires\\ZhuJieMian\\1.dds")
		PetGrowResetFrameCostMoneyIcon:SetTexUV(78, 494, 20, 18)
		PetGrowResetFrameCostMoneyIcon:SetSize(20, 18)
	end
end

local function UpdatePetGrowResetButton()
	if PetGrowResetFrameConfirm:IsShown() then
		PetGrowResetFrameReset:Hide();
		PetGrowResetFrameSaveReset:Show();
		PetGrowResetFrameCancelReset:Show();
	else
		PetGrowResetFrameReset:Show();
		PetGrowResetFrameSaveReset:Hide();
		PetGrowResetFrameCancelReset:Hide();
	end
	if not SelectedPetOnPetGrowResetFrame then
		PetGrowResetFrameReset:Disable()
	else
		PetGrowResetFrameReset:Enable()
		local player = ActorMgr:getMainPlayer()
		local petMgr = player:getPet()
		for id, status in pairs(ResetPetStatus) do
			if status == "request" then
				PetGrowResetFrameReset:Disable()
				break
			elseif status == "confirm" then
				local info = petMgr:getPetInfo(bytesint(id))
				if info and id ~= SelectedPetOnPetGrowResetFrame then
					PetGrowResetFrameReset:Disable()
					break
				end
			end
		end
	end
end

local function SetPetGrowResetMoney(locktype, num)
	LockType, LockNum = locktype, num
	UpdatePetGrowResetMoney()
end

-- 还原保存的锁状态
local function UpdatePetGrowLockInfo()
	if not SelectedPetOnPetGrowResetFrame then
		return
	end
	local t = PetGrowResetLockInfo[SelectedPetOnPetGrowResetFrame]
	if t then
		local selects = {}
		local noselects = {}
		for name in pairs(PetGrowResetLockMap) do
			local btn = getglobal(name)
			if t[name] then
				btn:setCheckState(true)
				table.insert(selects, btn)
				if not btn:IsShown() then
					btn:Show()
				end
			else
				btn:setCheckState(false)
				table.insert(noselects, btn)
				if PetGrowResetFrameConfirm:IsShown() then
					btn:Hide()
				elseif not btn:IsShown() then
					btn:Show()
				end
			end
			btn:SetClientString("")
		end
		if #noselects == 1 then
			noselects[1]:SetClientString("只能锁定5项属性")
		end
		PetGrowResetFrameAllUnLock:Hide()
		if t.coin then
			PetGrowResetFrameAllLockCert:Hide()
			if not PetGrowResetFrameAllLockCoin:IsShown() then
				PetGrowResetFrameAllLockCoin:Show()
			end
		else
			PetGrowResetFrameAllLockCoin:Hide()
			if not PetGrowResetFrameAllLockCert:IsShown() then
				PetGrowResetFrameAllLockCert:Show()
			end
		end
		if PetGrowResetFrameConfirm:IsShown() then
			PetGrowResetFrameAllLockCoin:Hide()
			PetGrowResetFrameAllLockCert:Hide()
		end
		SetPetGrowResetMoney(t.coin and "coin" or "cert", #selects)
	else
		for name in pairs(PetGrowResetLockMap) do
			local btn = getglobal(name)
			btn:Hide()
			btn:setCheckState(false)
		end
		PetGrowResetFrameAllLockCoin:Hide()
		PetGrowResetFrameAllLockCert:Hide()
		if not PetGrowResetFrameAllUnLock:IsShown() then
			PetGrowResetFrameAllUnLock:Show()
		end
		if PetGrowResetFrameConfirm:IsShown() then
			PetGrowResetFrameAllUnLock:Hide()
		end
		SetPetGrowResetMoney()
	end
	PetGrowResetFrameAllGrowHook:Hide()
end

local function UpdatePetGrowResetItem()
	local MainPlayer = ActorMgr:getMainPlayer()
	local container = MainPlayer:getContainer()
	local num = container:getItemCount(PET_GROW_RESET_ITEM)
	if num > 0 then
		PetGrowResetFrameCostItemIcon:SetTexture(GetItemIconPathByID(PET_GROW_RESET_ITEM))
		PetGrowResetFrameCostItemCount:Show()
	else
		PetGrowResetFrameCostItemIcon:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga")
		PetGrowResetFrameCostItemCount:Hide()
	end
	PetGrowResetFrameCostItem:SetClientUserData(1, num)
	for i = 0, container:getContainerMaxNum(CONTAINER_TYPE_SUNDRIES) - 1 do
		local item = container:getItem(CONTAINER_TYPE_SUNDRIES, i)
		if item:getItemId() == PET_GROW_RESET_ITEM then
			LockItem(CONTAINER_TYPE_SUNDRIES, i, PET_GROW_RESET_ITEM, "PetGrowResetFrame")
		end
	end
end

local function UpdatePetListPageOnPetGrowResetFrame()
	local value = PetGrowResetFrameScrollBar:GetValue()
	local player = ActorMgr:getMainPlayer()
	local petMgr = player:getPet()
	local btn
	for i = 1, PetListOnePage do
		local info = petMgr:getPetInfo(value + i - 1)
		btn = getglobal("PetGrowResetFramePetListName"..i)
		if info then
			local id = intbytes(info.WID.nHeight, info.WID.nLow)
			btn:SetText(info.Name)
			if id == SelectedPetOnPetGrowResetFrame then
				btn:Checked()
			else
				btn:DisChecked()
			end
		else
			btn:SetText("")
			btn:DisChecked()
		end
	end
end

local function SetPetResetGrowInfoRate(prefix, grade, value, range, blinkRange)
	local ratesuffix = {
		["Attack"] = "Attack", ["HP"] = "HP", ["Defend"] = "Defend", ["Resist"] = "Resist", ["Exact"] = "Exact", ["Dodge"] = "Dodge"
	}
	local ui
	for rate, suffix in pairs(ratesuffix) do
		local current = value[rate]
		local min = range.Min[rate]
		local max = range.Max[rate]
		ui = getglobal(prefix..suffix.."Rate")
		ui:SetText(current)
		local tips = ui;
		tips:SetClientUserData( 0, 0 );
		ui = getglobal(prefix..suffix.."RateTex")
		ui:Show()
		if current - min > 0 then
			if not ui:IsShown() then
				ui:Show()
			end
			local scale = (current - min) / (max - min)
			if scale >= 1 then
				ui:ChangeTextureTemplate("PurpleBarTexTemplate")
			else
				ui:ChangeTextureTemplate("WaterBlueBarTexTemplate")
			end
			ui:SetWidth(math.floor(66 * scale + 0.5))
		else
			ui:Hide()
			tips:SetClientUserData( 0, 0 );
		end
		if blinkRange then
			local blink = blinkRange.Min[rate]
			ui = getglobal(prefix..suffix.."RateEffect")
			if current > blink and grade ~= CLIENT_PET_GENIUS_S then
				ui:SetUVAnimation(160,true)
				tips:SetClientUserData( 0, 1 );
			else
				ui:Hide()
			end
		else
			ui:Hide()
		end
	end
	local genius = t_geniusDesc[grade]
	local color = genius.color
	ui = getglobal(prefix.."GrowValue")
	ui:SetText(genius.value)
	ui:SetTextColor(color.r, color.g, color.b)
end

function PetAttrRate_OnEnter()
	if this:GetClientUserData( 0 ) > 0 then
		local szText = "这条属性成长已达到#G晋级条件#n，当#G全部成长属性#n都达到#G晋级条件#n时幻化将\n#G100%提升#n宠物资质";
		local name = this:GetName();
		if string.find( name, "Old" ) ~= nil then
			ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
							button = this:GetName() } );
		else
			ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
							button = this:GetName(), horizonal = "right" } );
		end
	end
end

local function UpdatePetInfoOnPetGrowResetFrame()
	local player = ActorMgr:getMainPlayer()
	local petMgr = player:getPet()
	local info = petMgr:getPetInfo(bytesint(SelectedPetOnPetGrowResetFrame))
	PetGrowResetFrameName:SetText(info.Name)
	local PetDef = getPetDef(info.PetID)
	PetGrowResetFramePhoto:SetTexture(GetMonsterIconPath(PetDef.IconID))
	SetPetResetGrowInfoRate("PetGrowResetFrameAllOld", info.PetGenius, petMgr:getPetGrowValue(info), petMgr:getPetGrowResetRange(info), petMgr:getPetBlinkMinRange(info.PetGenius,info.PetID, info.Imp) )
	local genius = t_geniusDesc[info.PetGenius]
	local color = genius.color
	PetGrowResetFrameGrowGrade:SetText(genius.value)
	PetGrowResetFrameGrowGrade:SetTextColor(color.r, color.g, color.b)
	UpdatePetGrowResetMoney()
end

local function UpdatePetsOnPetGrowResetFrame()
	local player = ActorMgr:getMainPlayer()
	local petMgr = player:getPet()
	local num = petMgr:getPetNum()
	local step = num - PetListOnePage
	local slider = PetGrowResetFrameScrollBar
	if step > 0 then
		slider:Show()
		if slider:GetValue() > step then
			slider:SetValue(step)
		end
		slider:SetMaxValue(step)
	else
		slider:Hide()
		slider:SetValue(0)
	end
	PetGrowResetFramePetListTitle:SetText(string.format("宠物列表 %d/%d", num, petMgr:getPetMaxNum()))
	local id
	for i = 0, num - 1 do
		local info = petMgr:getPetInfo(i)
		if info then
			id = intbytes(info.WID.nHeight, info.WID.nLow)
			if id == SelectedPetOnPetGrowResetFrame then
				break
			end
			id = nil
		end
	end
	SelectedPetOnPetGrowResetFrame = id
	UpdatePetListPageOnPetGrowResetFrame()
	if SelectedPetOnPetGrowResetFrame then
		local player = ActorMgr:getMainPlayer()
		local petMgr = player:getPet()
		local info = petMgr:getPetInfo(bytesint(SelectedPetOnPetGrowResetFrame))
		UpdatePetInfoOnPetGrowResetFrame()
	else
		SelectPetOnPetGrowResetFrame(1)
	end
end

local function ResetPetGrowConfirm(id)
	ClearItemLock("PetGrowResetFrame")
	local player = ActorMgr:getMainPlayer()
	do
		local lock
		if IsItemLock(0, 0, PET_GROW_RESET_ITEM) then
			lock = true
		end
		local container = player:getContainer()
		for i = 0, container:getContainerMaxNum(CONTAINER_TYPE_SUNDRIES) - 1 do
			local item = container:getItem(CONTAINER_TYPE_SUNDRIES, i)
			if item:getItemId() == PET_GROW_RESET_ITEM then
				LockItem(CONTAINER_TYPE_SUNDRIES, i, PET_GROW_RESET_ITEM, "PetGrowResetFrame")
			end
		end
		if lock then
			return ShowMidTips(string.format("%s已被锁定，无法进行宠物幻化", getItemDef(PET_GROW_RESET_ITEM).Name))
		end
	end
	local lock = 0
	local t = PetGrowResetLockInfo[id]
	if t then
		for name, value in pairs(PetGrowResetLockMap) do
			if t[name] then
				lock = lock + value
			end
		end
		if lock == 0 then
			return ShowMidTips("请选择需要锁定的属性，被锁定的属性在幻化中不会被改变")
		end
	end
	local container = player:getContainer()
	if container:getItemCount(PET_GROW_RESET_ITEM) <= 0 then
		MessageBoxForItem("提示",getItemDef(PET_GROW_RESET_ITEM).Name);
		MessageBoxForItemFrame:SetClientUserData(0,PET_GROW_RESET_ITEM);
		return;
	end
	local petMgr = player:getPet()
	local high, low = bytesint(id)
	if t then
		local num = 0
		for name in pairs(PetGrowResetLockMap) do
			if t[name] then
				num = num + 1
			end
		end
		local money = petMgr:getPetGrowResetCostMoney(high, low, num, t.coin)
		if t.coin then
			if player:getBindGodCoin() < money then
				return ShowMidTips(string.format("%s不足，无法进行宠物幻化，请解锁后再尝试", MONEY_TYPE_BIND_GODCOIN_NAME))
			end
		else
			if player:getGodCoin() < money then
				return ShowMidTips(string.format("%s不足，无法进行宠物幻化，请解锁后再尝试", MONEY_TYPE_GODCOIN_NAME))
			end
		end
	else
		local money = petMgr:getPetGrowResetCostMoney(high, low)
		if getPlayerMoney( player ) < money then
			return ShowMidTips(string.format("%s不足，无法进行宠物幻化", MONEY_TYPE_BULL_NAME))
		end
	end
	local info = petMgr:getPetInfo(high, low)
	local value = petMgr:getPetGrowValue(info)
	local limit = petMgr:getPetGrowResetLimit(info)
	local upper = limit.Max
	local nolock = {}
	for attr, text in pairs(AttrType) do
		if value[attr] > upper[attr] then
			if not (PetGrowResetLockInfo[id] and PetGrowResetLockInfo[id][AttrHook[attr]]) then
				table.insert(nolock, text)
			end
		end
	end
	if #nolock > 0 then
		--[[
		table.sort(nolock, function(a, b)
			return AttrOrder[a] < AttrOrder[b]
		end)
		]]--
		local genius = t_geniusDesc[info.PetGenius]
		local color = genius.color
		color = string.format("%02x%02x%02x", color.r, color.g, color.b)
		if not MessageBoxBlock("幻化确认", string.format("%s的%s成长已高于#c%s%s#n资质上限，建议您对其锁定后再幻化！\n#cFFFFBE是否立即进行宠物幻化？#n", info.Name, table.concat(nolock, "、"), color, genius.value), PetGrowResetFrame, function() return SelectedPetOnPetGrowResetFrame == id end) then
			return
		end
	end
	if t then
		petMgr:ResetPetGrow(high, low, lock, t.coin)
	else
		local money = petMgr:getPetGrowResetCostMoney(high, low);
		if checkNeedWarnCost( money ) then
			PetGrowResetFrameReset:SetClientString( id );
			CostMoneyConfirmFrame:SetClientString("宠物幻化");
			SetWarnFrame( "PetGrowResetFrame" );
			CostMoneyConfirmFrame:Show();
			return;
		end
		petMgr:ResetPetGrow(high, low)
	end
	ResetPetStatus = {}
	ResetPetStatus[id] = "request"
	UpdatePetGrowResetButton()
end

function SetPetGrowResetState( id )
	ResetPetStatus = {};
	ResetPetStatus[id] = "request";
	UpdatePetGrowResetButton();
end

function SelectPetOnPetGrowResetFrame(i)
	if i then
		local player = ActorMgr:getMainPlayer()
		local petMgr = player:getPet()
		local info = petMgr:getPetInfo(i - 1)
		local id = intbytes(info.WID.nHeight, info.WID.nLow)
		if id ~= SelectedPetOnPetGrowResetFrame then
			SelectedPetOnPetGrowResetFrame = id
			UpdatePetGrowLockInfo()
			UpdatePetInfoOnPetGrowResetFrame()
		end
	else
		if SelectedPetOnPetGrowResetFrame then
			SelectedPetOnPetGrowResetFrame = nil
			PetGrowResetFrameName:SetText("")
			PetGrowResetFramePhoto:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga")
			UpdatePetsOnPetGrowResetFrame()
		end
	end
	UpdatePetListPageOnPetGrowResetFrame()
	UpdatePetGrowResetButton()
	if SelectedPetOnPetGrowResetFrame and ResetPetStatus[SelectedPetOnPetGrowResetFrame] == "confirm" then
		PetGrowResetFrameConfirm:Show()
	elseif PetGrowResetFrameConfirm:IsShown() then
		PetGrowResetFrameConfirm:Hide()
	end
	local bool, func = IsMessageBoxBlock(PetGrowResetFrame)
	if bool then
		if not func or not func() then
			MessageBoxBlockEnd(PetGrowResetFrame)
		end
	end
end

function PetGrowResetFrame_OnLoad()
	this:RegisterEvent("GE_CONTAINER_UPDATE")
	this:RegisterEvent("GE_CHANGE_MONEY")
	this:RegisterEvent("GE_PET_INFO_CHG")
	this:RegisterEvent("GE_DEL_PET")
	this:RegisterEvent("GE_PET_ATTR_CHG")
	PetGrowResetFrameCostItem:SetClientUserData(0, PET_GROW_RESET_ITEM)
	PetGrowResetFrameCostItemName:SetText(getItemDef(PET_GROW_RESET_ITEM).Name)
	ItemFirstCallBack(this:GetName(), function(item)
		if item:getItemId() ~= PET_GROW_RESET_ITEM then
			ShowMidTips("该道具不能用于宠物幻化")
		end
	end)
	PetGrowResetFrameSaveResetRich:setCenterLine( true );
	PetGrowResetFrameSaveResetRich:SetText( "#c00ffff保存#n+继续幻化", 255, 255, 190 );
	PetGrowResetFrameCancelResetRich:setCenterLine( true );
	PetGrowResetFrameCancelResetRich:SetText( "#cffc800取消#n+继续幻化", 255, 255, 190 );
end

function PetGrowResetFrame_OnEvent()
	if arg1 == "GE_CONTAINER_UPDATE" then
		if this:IsShown() then
			UpdatePetGrowResetItem()
		end
	elseif arg1 == "GE_CHANGE_MONEY" then
		if this:IsShown() then
			UpdatePetGrowResetMoney()
		end
	elseif arg1 == "GE_PET_INFO_CHG" or arg1 == "GE_DEL_PET" or arg1 == "GE_PET_ATTR_CHG" then
		if this:IsShown() then
			local player = ActorMgr:getMainPlayer()
			local petMgr = player:getPet()
			if petMgr:getPetNum() == 0 then
				ShowMidTips( "身上没宠物，不能进行该操作，自动关闭面板" );
				this:Hide()
				PetFrame:Hide();
			else
				UpdatePetsOnPetGrowResetFrame()
				UpdatePetGrowResetButton()
			end
		end
	end
end

function PetGrowResetFrame_OnShow()
	PetGrowResetLockInfo = {}
	ResetPetStatus = {}
	SelectPetOnPetGrowResetFrame()
	PetGrowResetFrameScrollBar:SetValue(0)
	UpdatePetsOnPetGrowResetFrame()
	UpdatePetGrowResetItem()
	if not PetFrame:IsShown() then
		PetFrame:Show()
	end
	local player = ActorMgr:getMainPlayer()
	local petMgr = player:getPet()
	if petMgr:getPetNum() > 0 then
		SelectPetOnPetGrowResetFrame(1)
		PetListButtonSelect(0)
	end
	UpdatePetGrowResetButton()
	PetFrame:SetPoint("topright", "UIClient", "topleft", 428, 163)
	this:SetPoint("topleft", "UIClient", "topleft", 428, 163)
	local modelview = getglobal(this:GetName().."ModelView")
	modelview:Hide()
end

function PetGrowResetFrame_OnHide()
	ClearItemLock("PetGrowResetFrame")
	PetGrowResetFrameConfirm:Hide()
end

function PetGrowResetFrame_ScrollBarValueChange()
	UpdatePetListPageOnPetGrowResetFrame()
end

function PetGrowResetFrame_ScrollUpBtn()
	local slider = PetGrowResetFrameScrollBar
	if not slider:IsShown() then
		return
	end
	local nValue = slider:GetValue() - slider:GetValueStep()
	if nValue >= slider:GetMinValue() then
		slider:SetValue(nValue)
	end
end

function PetGrowResetFrame_ScrollDownBtn()
	local slider = PetGrowResetFrameScrollBar
	if not slider:IsShown() then
		return
	end
	local nValue	= slider:GetValue() + slider:GetValueStep()
	if nValue <= slider:GetMaxValue() then
		slider:SetValue(nValue)
	end
end

function PetGrowResetFrame_OnReset()
	if CostMoneyConfirmFrame:IsShown() then
		return;
	end
	
	if string.find( this:GetName(), "Save", 1, true ) then
		local player = ActorMgr:getMainPlayer()
		local petMgr = player:getPet()
		petMgr:ResetPetGrowSave(bytesint(SelectedPetOnPetGrowResetFrame))
		if IsMessageBoxBlock(PetGrowResetFrame) then
			MessageBoxBlockEnd(PetGrowResetFrame)
		end
	elseif string.find( this:GetName(), "Cancel", 1, true ) then
		if IsMessageBoxBlock(PetGrowResetFrame) then
			MessageBoxBlockEnd(PetGrowResetFrame)
		end
	else
		if IsMessageBoxBlock(PetGrowResetFrame) then
			return
		end
	end
	coroutine.wrap(ResetPetGrowConfirm)(SelectedPetOnPetGrowResetFrame)
end

function PetGrowResetPetListName_OnClick()
	if IsMessageBoxBlock(PetGrowResetFrame) then
		return
	end
	local i = this:GetClientID() + PetGrowResetFrameScrollBar:GetValue()
	local player = ActorMgr:getMainPlayer()
	local petMgr = player:getPet()
	local info = petMgr:getPetInfo(i - 1)
	if info then
		SelectPetOnPetGrowResetFrame(i)
		if PetFrame:IsShown() then
			PetListButtonSelect(i - 1)
		end
	end
end

local function LockPetGrowConfirm(id)
	local result = ConfirmCoinCertBoxBlock("宠物幻化锁定", "锁定属性，每次幻化时将消耗金币或者金券。选择后在希望锁定的属性前打勾以锁定该属性。\n请选择消耗类型：", PetGrowResetFrame)
	if result then
		PetGrowResetLockInfo[id] = {}
		--PetGrowResetLockInfo[id]["PetGrowResetFrameAllGrowHook"] = true
		PetGrowResetLockInfo[id].coin = (result == "coin")
		UpdatePetGrowLockInfo()
	end
end

function PetGrowResetLock_OnClick()
	coroutine.wrap(LockPetGrowConfirm)(SelectedPetOnPetGrowResetFrame)
end

function PetGrowResetUnLock_OnClick()
	if SelectedPetOnPetGrowResetFrame then
		PetGrowResetLockInfo[SelectedPetOnPetGrowResetFrame] = nil
	end
	UpdatePetGrowLockInfo()
end

function PetGrowResetConfirm_OnShow()
	PetGrowResetFrameTopLine:Hide()
	PetGrowResetFrameGrowGradeTitle:Hide()
	PetGrowResetFrameGrowGrade:Hide()
	PetGrowResetFrameMaskAll:Show()
	UpdatePetGrowResetButton()
	UpdatePetGrowLockInfo()
	
	local grade = ResetConfirmGrow.genius
	local value = ResetConfirmGrow.value
	local player = ActorMgr:getMainPlayer()
	local petMgr = player:getPet()
	local info = petMgr:getPetInfo(bytesint(SelectedPetOnPetGrowResetFrame))
	SetPetResetGrowInfoRate("PetGrowResetFrameAllNew", grade, value, petMgr:getPetGrowResetRange(info), petMgr:getPetBlinkMinRange(grade,info.PetID,info.Imp))
	local current = petMgr:getPetGrowValue(info)
	local ratesuffix = {
		["Attack"] = "Attack", ["HP"] = "HP", ["Defend"] = "Defend", ["Resist"] = "Resist", ["Exact"] = "Exact", ["Dodge"] = "Dodge"
	}
	for rate, suffix in pairs(ratesuffix) do
		local up = getglobal(string.format("PetGrowResetFrameConfirm%sUp", suffix))
		local down = getglobal(string.format("PetGrowResetFrameConfirm%sDown", suffix))
		local equal = getglobal(string.format("PetGrowResetFrameConfirm%sEqual", suffix))
		local c = current[rate]
		local v = value[rate]
		if c < v then
			down:Hide()
			equal:Hide()
			if not up:IsShown() then
				up:Show()
			end
		elseif c > v then
			up:Hide()
			equal:Hide()
			if not down:IsShown() then
				down:Show()
			end
		else
			up:Hide()
			down:Hide()
			if not equal:IsShown() then
				equal:Show()
			end
		end
	end
	local up = PetGrowResetFrameConfirmGrowUp
	local down = PetGrowResetFrameConfirmGrowDown
	local equal = PetGrowResetFrameConfirmGrowEqual
	if info.PetGenius < grade then
		down:Hide()
		equal:Hide()
		if not up:IsShown() then
			up:Show()
		end
	elseif info.PetGenius > grade then
		up:Hide()
		equal:Hide()
		if not down:IsShown() then
			down:Show()
		end
	else
		up:Hide()
		down:Hide()
		if not equal:IsShown() then
			equal:Show()
		end
	end
end

function PetGrowResetConfirm_OnHide()
	PetGrowResetFrameTopLine:Show()
	PetGrowResetFrameGrowGradeTitle:Show()
	PetGrowResetFrameGrowGrade:Show()
	PetGrowResetFrameMaskAll:Hide()
	local ratesuffix = {
		"Attack", "HP", "Defend", "Resist", "Exact", "Dodge"
	}
	local textsuffix = {
		"Grow"
	}
	local ui
	for _, suffix in ipairs(ratesuffix) do
		ui = getglobal("PetGrowResetFrameAllNew"..suffix.."Rate")
		ui:SetText("")
		ui:SetClientUserData( 0, 0 );
		ui = getglobal("PetGrowResetFrameAllNew"..suffix.."RateTex")
		ui:Hide()
		ui = getglobal("PetGrowResetFrameAllNew"..suffix.."RateEffect")
		ui:Hide()
	end
	for _, suffix in ipairs(textsuffix) do
		ui = getglobal("PetGrowResetFrameAllNew"..suffix.."Value")
		ui:SetText("")
	end
	if SelectedPetOnPetGrowResetFrame and ResetPetStatus[SelectedPetOnPetGrowResetFrame] then
		ResetPetStatus[SelectedPetOnPetGrowResetFrame] = nil
	end
	UpdatePetGrowResetButton()
	UpdatePetGrowLockInfo()
end

function PetGrowResetFrameConfirm_OnSave()
	util.closeParent(this)
	local player = ActorMgr:getMainPlayer()
	local petMgr = player:getPet()
	petMgr:ResetPetGrowSave(bytesint(SelectedPetOnPetGrowResetFrame))
	if IsMessageBoxBlock(PetGrowResetFrame) then
		MessageBoxBlockEnd(PetGrowResetFrame)
	end
end

function PetGrowResetFrameConfirm_OnCancel()
	util.closeParent(this)
	if IsMessageBoxBlock(PetGrowResetFrame) then
		MessageBoxBlockEnd(PetGrowResetFrame)
	end
end

function PetGrowResetUnLock_OnEnter()
	SetGameTooltips(this:GetName(), "点击锁定宠物资质成长，被锁定的属性成长在幻化时不会变化")
end

function PetGrowResetLockCert_OnEnter()
	SetGameTooltips(this:GetName(), "点击解锁宠物资质成长，解锁后宠物所有属性成长在幻化时都会变化")
end

function PetGrowResetLockCoin_OnEnter()
	SetGameTooltips(this:GetName(), "点击解锁宠物资质成长，解锁后宠物所有属性成长在幻化时都会变化")
end

function PetGrowResetFrame_OnTips()
	local szText = [[
宠物幻化说明：
1.  宠物幻化会改变宠物资质
2.  宠物幻化会改变宠物的#G各项成长#n
3.  可以锁定最多#G五项#n成长属性
4.  使用#G幻化果实#n进行宠物幻化
5.  当所有的#G成长条变亮#n时会100%#G提升资质#n]];
	ShowGameTooltip( { rich = "WideGameTooltipRichText", tipsFrame = "WideGameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function PetGrowResetItem_OnLock()
	if not SelectedPetOnPetGrowResetFrame then
		return this:setCheckState(false)
	end
	local str = this:GetClientString()
	if str ~= "" then
		ShowMidTips(str)
		return this:setCheckState(false)
	end
	local name = this:GetName()
	if this:GetCheckState() then
		PetGrowResetLockInfo[SelectedPetOnPetGrowResetFrame][name] = true
	else
		PetGrowResetLockInfo[SelectedPetOnPetGrowResetFrame][name] = nil
	end
	UpdatePetGrowLockInfo()
end

local function PetGrowOnResetWrap(high, low, grade, value)
	local id = intbytes(high, low)
	if id ~= SelectedPetOnPetGrowResetFrame then
		return
	end
	
	local modelview = PetGrowResetFrameModelView
	modelview:setPetModel(0, 0, "effect\\entity\\fumo01.ent")
	modelview:setEntityPosition(0, 0, 100, 0)
	if not modelview:IsShown() then
		modelview:Show()
	end
	
	ResetConfirmGrow.genius = grade
	ResetConfirmGrow.value = {}
	for attr in pairs(AttrType) do
		ResetConfirmGrow.value[attr] = value[attr]
	end
	ResetPetStatus = {}
	ResetPetStatus[id] = "confirm"
	local player = ActorMgr:getMainPlayer()
	local petMgr = player:getPet()
	local info = petMgr:getPetInfo(high, low)
	PetGrowResetFrameConfirm:Show()
	local range = petMgr:getPetGrowResetRange(info)
	local max = range.Max
	local limits = {}
	if grade > info.PetGenius then
		if not (PetGrowResetLockInfo[id] and PetGrowResetLockInfo[id]["PetGrowResetFrameAllGrowHook"]) then
			--table.insert(limits, "资质")
		end
	end
	for attr, text in pairs(AttrType) do
		if value[attr] == max[attr] then
			if not (PetGrowResetLockInfo[id] and PetGrowResetLockInfo[id][AttrHook[attr]]) then
				table.insert(limits, text)
			end
		end
	end
	if #limits > 0 or grade > info.PetGenius then
		--[[
		table.sort(limits, function(a, b)
			return AttrOrder[a] < AttrOrder[b]
		end)
		]]--
		local text = {}
		if grade > info.PetGenius then
			local genius = t_geniusDesc[grade]
			local color = genius.color
			color = string.format("%02x%02x%02x", color.r, color.g, color.b)
			table.insert(text, string.format("已经晋升为#c%s%s#cFFFFBE", color, genius.value))
		end
		if #limits > 0 then
			table.insert(text, string.format("%s成长已满", table.concat(limits, "、")))
		end
		if MessageBoxBlock("幻化确认", string.format("#cFFFFBE您的%s%s，是否保存？", info.Name, table.concat(text, "，")), PetGrowResetFrameConfirm, function() return SelectedPetOnPetGrowResetFrame == id end) then
			petMgr:ResetPetGrowSave(high, low)
			PetGrowResetFrameConfirm:Hide()
		end
	end
end

function PetGrowOnReset(...)
	if not PetGrowResetFrame:IsShown() then
		return
	end
	local args = {...}
	if #args == 0 then
		ResetPetStatus = {}
		if PetGrowResetFrameConfirm:IsShown() then
			PetGrowResetFrameConfirm:Hide()
		end
		UpdatePetGrowResetButton()
	else
		coroutine.wrap(PetGrowOnResetWrap)(args[1], args[2], args[3], args[4])
	end
end

-------------------------------宠物培养指引界面-----------------------------------

function PetProceFrameTemplateCloseBtn_OnClick()
	util.closeParent(this);
end

function PetProceFrame_OnLoad()
	this:RegisterEvent("GE_DEL_PET");
	local szText1 = [[		欢迎使用宠物培养系统，请点击下方标签进行功能选择。
		想宠物拥有#cf57300更高的属性#n吗？快来#cf57300强化#n吧！
		想要#cf57300完美资质#n的宠物吗？快来#cf57300幻化#n吧！
		想拥有与你绝配的宠物技能吗？想让宠物技能提供更高的属性吗？快使用技能重置吧！]];
	local szText2 = [[获得宠物方式：
1. 野外捕捉
2. 彩虹岛
3. 活动奖励
4. 宝箱抽奖]];
	PetProceFrameRich1:SetText( szText1, 160, 155, 130 );
	PetProceFrameRich2:SetText( szText2, 255, 255, 190 );
end

function PetProceFrame_OnEvent()
	if arg1 == "GE_DEL_PET" then
		if PetProceFrame:IsShown() then
			local mainplayer = ActorMgr:getMainPlayer();
			local petPackage = mainplayer:getPet();
			if petPackage:getPetNum() == 0 then
				ShowMidTips( "身上没宠物，不能进行该操作，自动关闭面板" );
				PetProceFrame:Hide();
				PetFrame:Hide();
			end
		end
	end
end

local t_petFrames = { "PetProceFrame", "PetGrowResetFrame", "Pet_DevelopmentBuildUpFrame", "PetSkillLevelUpFrame", "PetSkillResetFrame","PetStepUpFrame" };

-- 宠物培养指引
function OpenPetProceFrame( nNpcID )
	local mainplayer = ActorMgr:getMainPlayer();
	local petPackage = mainplayer:getPet();
	if petPackage:getPetNum() == 0 then
		ShowMidTips( "身上没有宠物，不能进行该操作" );
		return;
	end

	for i=1, #t_petFrames do
		local frame = getglobal( t_petFrames[i] );
		if frame:IsShown() then
			frame:Hide();
		end
	end
	PetProceFrame:SetClientUserData( 0,nNpcID );
	PetProceFrame:Show();
	PetProceFrame:SetPoint("topleft", "UIClient", "topleft", 428, 163);
	PetFrame:Show();
	PetFrame:SetPoint("topright", "UIClient", "topleft", 428, 163);
	--util.testNpcDialogDistance( nNpcID, PetProceFrame );
end

function PetProceFrame_OnShow()
	DisCheckAllSideBtn( this:GetName() );
end

function DisCheckAllSideBtn( name )
	local t_btn = { name .. "GrowResetBtn", name .. "StarBtn", name .. "SkillUpBtn", name .. "SkillResetBtn" };
	for i=1, #t_btn do
		local btn = getglobal( t_btn[i] );
		btn:DisChecked();
	end
end

function PetProceFrameGrowResetBtn_OnClick()
	if this:IsChecked() then
		return
	end
	local frame = getglobal( this:GetParent() );
	local npcid = PetProceFrame:GetClientUserData( 0 );
	frame:Hide();
	PetGrowResetFrame:Show()
	PetGrowResetFrame:SetClientUserData( 0,npcid );
	DisCheckAllSideBtn( "PetProceFrame" );
	PetGrowResetFrameGrowResetBtn:Checked();
	--util.testNpcDialogDistance( npcid, PetGrowResetFrame );
end

function PetProceFrameStarBtn_OnClick()
	if this:IsChecked() then
		return
	end
	local frame = getglobal( this:GetParent() );
	local npcid = PetProceFrame:GetClientUserData( 0 );
	frame:Hide();
	Pet_DevelopmentBuildUpFrame:Show()
	Pet_DevelopmentBuildUpFrame:SetClientUserData( 0,npcid );
	DisCheckAllSideBtn( "PetProceFrame" );
	Pet_DevelopmentBuildUpFrameStarBtn:Checked();
	--util.testNpcDialogDistance( npcid, Pet_DevelopmentBuildUpFrame );
end

function PetProceFrameSkillUpBtn_OnClick()
	if this:IsChecked() then
		return
	end
	local frame = getglobal( this:GetParent() );
	local npcid = PetProceFrame:GetClientUserData( 0 );
	frame:Hide();
	PetSkillLevelUpFrame:Show()
	PetSkillLevelUpFrame:SetClientUserData( 0,npcid );
	DisCheckAllSideBtn( "PetProceFrame" );
	PetSkillLevelUpFrameSkillUpBtn:Checked();
	--util.testNpcDialogDistance( npcid, PetSkillLevelUpFrame );
end

function PetProceFrameSkillResetBtn_OnClick()
	if this:IsChecked() then
		return
	end
	local frame = getglobal( this:GetParent() );
	local npcid = PetProceFrame:GetClientUserData( 0 );
	frame:Hide();
	PetSkillResetFrame:Show()
	PetSkillResetFrame:SetClientUserData( 0,npcid );
	DisCheckAllSideBtn( "PetProceFrame" );
	PetSkillResetFrameSkillResetBtn:Checked();
	--util.testNpcDialogDistance( npcid, PetSkillResetFrame );
end

function PetProceFrameBtn1_OnClick()
	PetProceFrameStarBtn_OnClick();
end

function PetProceFrameBtn2_OnClick()
	PetProceFrameGrowResetBtn_OnClick();
end

function PetProceFrameBtn3_OnClick()
	PetProceFrameSkillResetBtn_OnClick();
end

function PetProceFrameBtn4_OnClick()
	PetProceFrameSkillUpBtn_OnClick();
end

function PetProceFrameBtn1_OnEnter()
	local szText = [[宠物强化：
		消耗“#cf57300基因果实#n”来为宠物强化加星
		宠物星级越高，增加的属性越多
		当宠物拥有12颗#cf57300完美星级#n，#cf57300全部属性提高120%#n]];
	ShowGameTooltip( { rich = "WideGameTooltipRichText", tipsFrame = "WideGameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName(), horizonal = "right" } );
end

function PetProceFrameBtn2_OnEnter()
	local szText = [[宠物幻化：
		消耗“#cf57300幻化果实#n”来提升宠物资质
		宠物资质分为：#cffffff普通#n、#c3fc117良好#n、#c338dfd优秀#n、#c8e5cd8杰出#n、#cf57300完美#n
		宠物资质越高，基础属性越高
		强化后获得的宠物属性加成越高]];
	ShowGameTooltip( { rich = "WideGameTooltipRichText", tipsFrame = "WideGameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName(), horizonal = "right" } );
end

function PetProceFrameBtn3_OnEnter()
	local szText = [[技能重置：
		宠物在升级中会自然领悟一些技能
		部分技能会在宠物出战时为你增加属性
		若你对宠物自然领悟的技能不满意，可以“重置”
		消耗“#cf57300遗忘果实#n”让宠物重新领悟技能]];
	ShowGameTooltip( { rich = "WideGameTooltipRichText", tipsFrame = "WideGameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function PetProceFrameBtn4_OnEnter()
	local szText = [[技能升级：
		消耗#cf57300训练果实#n为宠物已领悟的技能提升等级
		宠物技能等级越高，为你提供的收益就越大]];
	ShowGameTooltip( { rich = "WideGameTooltipRichText", tipsFrame = "WideGameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end