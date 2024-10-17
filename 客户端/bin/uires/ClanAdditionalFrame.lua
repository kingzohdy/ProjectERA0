MAX_CLAN_LEVEL = 5;				-- 工会所能达到的最高等级

local ADD_TYPE_TECH	= 1	-- 加速科技
local ADD_TYPE_PRODUCE = 2	--加速生产

local t_BuildTypeName = {
							[0] = "科技中心",
							[1] = "采集中心",
							[2] = "工厂",
							[3] = "兽栏",
							[4] = "郊区"
						}
local t_SkillTypeName = {
							[0] = "科技",
							[1] = "采集",
							[2] = "工厂",
							[3] = "兽栏",
							[4] = "郊区"
						}

function IsClanSkillLearned(id, level)
	local ClanAdvInfo = GameClanManager:getAdvanceInfo()
	return ClanAdvInfo.Tech.TechLevel[id] >= level
end

function CanLearnClanSkill(id, level)
	local ClanSkillDef = GameClanManager:getClanTechDef(id, level)
	local ClanAdvInfo = GameClanManager:getAdvanceInfo()
	local ClanViewer = GameClanManager:getClanViewer()
	local cityInfo = GameClanManager:getClanCity():getCityInfo()
	if ClanViewer.m_clanInfo.m_nSkillPoint < ClanSkillDef.NeedTechPointCost then
		return false
	end
	if ClanViewer.m_clanInfo.m_nMoney < ClanSkillDef.NeedMoneyCost then
		return false
	end
	if IsClanSkillLearned(id, level) then
		return false
	end
	if cityInfo.m_buildings[0].Level < ClanSkillDef.Required then
		return false
	end
	if level > 1 and not IsClanSkillLearned(id, level - 1) then
		return false
	end
	local needTech = ClanSkillDef.NeedTech
	local needTechLv = ClanSkillDef.NeedTechLevel
	if needTech ~= 0 and needTechLv ~= 0 and not IsClanSkillLearned(needTech, needTechLv) then
		return false
	end
	return true
end

local function ClanSkillFilter(id, level)
	if ClanLearnSkillFrameOnlyCanLearn:GetCheckState() then
		return CanLearnClanSkill(id, level)
	end
	return true
end

function UpdateClan( nUIType,nActorId )
	local t_fonts;
	local prefix;
	local ClanViewer		= GameClanManager:getClanViewer();
	local ClanInfo			= ClanViewer.m_clanInfo;
	-- 升级公会
	if nUIType == UI_TYPE_UPGRADE_CLAN then
		ClanLevelUpFrame:Show();
		ClanLevelUpFrame:AddLevelRecursive(1);
		ClanLevelUpFrame:SetClientUserData( 0,ClanInfo.m_nClanLevel + 1 );
		if nActorId ~= nil then
			util.testNpcDialogDistance( nActorId, ClanLevelUpFrame );
		end
	-- 升级公会城市
	elseif nUIType == UI_TYPE_UPGRADE_CLAN_CITY then
		local clanCityInfo		= GameClanManager:getClanCity():getCityInfo();
		local nMinClanLevl		= 0;
		local levelUpCityDef	= getClanCityLevelDef( clanCityInfo.m_nLevel + 1 );
		local curClanDef		= getClanLevelDef( ClanInfo.m_nClanLevel );
		t_fonts = {};
		
		local clanlv = 0;
		while true do
			local levelUpClanDef = getClanLevelDef(clanlv);
			if not levelUpClanDef or levelUpClanDef.LvLimited[CLAN_IDX_CITY_TOLUA] >= clanCityInfo.m_nLevel + 1 then
				break
			end
			clanlv = clanlv + 1
		end
		if clanlv > 0 then
			table.insert(t_fonts, {title = "公会等级", text = tostring(clanlv)});
		end
		local money = levelUpCityDef.Required.Money;
		if money > 0 then
			table.insert(t_fonts, {title = "公会资金", text = tostring(money)});
		end
		
		local stable = levelUpCityDef.Required.StableValue;
		if stable > 0 then
			table.insert(t_fonts, {title = "安定度", text = tostring(stable)});
		end
		local tech = levelUpCityDef.Required.UsedTechPoint;
		if tech > 0 then
			table.insert(t_fonts, {title = "已投入科技点", text = tostring(tech)});
		end
		for i = 1, MAX_CLAN_MAKE_MATERIAL_TOLUA do
			local resource = levelUpCityDef.Required.Material[i - 1];
			local id, num = resource.ID, resource.Num;
			if id ~= 0 and num ~= 0 then
				local name = getItemDef(id).Name;
				table.insert(t_fonts, {title = name, text = tostring(num)});
			end
		end

		-- 若是升级城市
		if clanCityInfo.m_nLevel > 0 then
			-- 判断升级城市是否需要材料
			prefix = "ClanCityRegionLevelUpFrameLine";
			ClanCityRegionLevelUpFrame_CurClanLevelFont:SetText(ClanInfo.m_nClanLevel.."级")
			ClanCityRegionLevelUpFrame_CurClanCityLevelFont:SetText(clanCityInfo.m_nLevel.."级")
			ClanCityRegionLevelUpFrame:SetSize(239, 180 + #t_fonts * 30);
			ClanCityRegionLevelUpFrameSecondBackGroundButton:SetPoint("topleft","ClanCityRegionLevelUpFrameBackGroundButton","topleft", 6, 74);
			ClanCityRegionLevelUpFrameSecondBackGroundButton:SetSize(221, 32 + #t_fonts * 30);
			ClanCityRegionLevelUpFrame:Show();
			if nActorId ~= nil then
				util.testNpcDialogDistance(nActorId, ClanCityRegionLevelUpFrame);
			end
		else
			-- 若是购买城市
			prefix = "ClanBuyCityRegionFrameLine";
			ClanBuyCityRegionFrame:SetSize(239, 225 + #t_fonts * 30);
			ClanBuyCityRegionFrameSecondBackGroundButton:SetPoint("topleft","ClanBuyCityRegionFrameBackGroundButton","topleft", 6, 117);
			ClanBuyCityRegionFrameSecondBackGroundButton:SetSize(221, 32 + #t_fonts * 30);
			ClanBuyCityRegionFrame:Show();
			if nActorId ~= nil then
				util.testNpcDialogDistance( nActorId, ClanBuyCityRegionFrame );
			end
		end
	elseif nUIType == UI_TYPE_UPGRADE_CLAN_CITY_BUILDING then
		-- 升级工会城市中的建筑
		local clanCityBuildingInfo		= GameClanManager:getClanCity():getSelectBuilding();
		local levelUpCityBuildingDef	= getClanCityBuildingLevelDef( clanCityBuildingInfo.m_nType, clanCityBuildingInfo.m_nCurLevel + 1 );
		local clanCityInfo				= GameClanManager:getClanCity():getCityInfo();
		prefix = "ClanBuyBuildingFrameSecondBackGroundButtonLine";
		t_fonts = {};
		local clanlv = 0;
		while true do
			local levelUpClanDef = getClanLevelDef(clanlv);
			if not levelUpClanDef or levelUpClanDef.LvLimited[clanCityBuildingInfo.m_nType] >= clanCityBuildingInfo.m_nCurLevel + 1 then
				break
			end
			clanlv = clanlv + 1
		end
		if clanlv > 0 then
			table.insert(t_fonts, {title = "公会等级", text = tostring(clanlv)});
		end
		local money = levelUpCityBuildingDef.Required.Money;
		if money > 0 then
			table.insert(t_fonts, {title = "公会资金", text = tostring(money)});
		end
		local citylv = levelUpCityBuildingDef.RequiredCityLv;
		if citylv > 0 then
			table.insert(t_fonts, {title = "公会城市等级", text = tostring(citylv)});
		end
		local stable = levelUpCityBuildingDef.Required.StableValue;
		if stable > 0 then
			table.insert(t_fonts, {title = "安定度", text = tostring(stable)});
		end
		local tech = levelUpCityBuildingDef.Required.UsedTechPoint;
		if tech > 0 then
			table.insert(t_fonts, {title = "已投入科技点", text = tostring(tech)});
		end
		for i = 1, MAX_CLAN_MAKE_MATERIAL_TOLUA do
			local resource = levelUpCityBuildingDef.Required.Material[i - 1];
			local id, num = resource.ID, resource.Num;
			if id ~= 0 and num ~= 0 then
				local name = getItemDef(id).Name;
				table.insert(t_fonts, {title = name, text = tostring(num)});
			end
		end
		if clanCityBuildingInfo.m_nCurLevel > 0 then
			-- 若是升级建筑
			ClanBuyBuildingFrame_CurClanLevelFont:SetText(ClanInfo.m_nClanLevel.."级")
			ClanBuyBuildingFrame_CurClanCityLevelFont:SetText(clanCityInfo.m_nLevel.."级")
			ClanBuyBuildingFrameFrameFont:SetText("升级"..t_BuildTypeName[clanCityBuildingInfo.m_nType])
			ClanBuyBuildingFrameSecondBackGroundButtonSecondTitleFont:SetText("满足以下需求即可升级"..t_BuildTypeName[clanCityBuildingInfo.m_nType])
			ClanBuyBuildingFrameSecondBackGroundButtonCostTimeFont:SetText("升级" .. t_BuildTypeName[clanCityBuildingInfo.m_nType] .. "需要花费" .. math.ceil(levelUpCityBuildingDef.TimeCost / 60) .. "分钟")
			ClanBuyBuildingFrameBackGroundButtonCurClanBuildingLevelHeadFont:SetText("当前"..t_BuildTypeName[clanCityBuildingInfo.m_nType].."等级")
			ClanBuyBuildingFrame_CurClanBuildingLevelFont:SetText(clanCityBuildingInfo.m_nCurLevel.."级")
			ClanBuyBuildingFrame_CurClanBuildingLevelBtn:Show();
			ClanBuyBuildingFrame:SetSize(239, 220 + #t_fonts * 30);
			ClanBuyBuildingFrameSecondBackGroundButton:SetPoint("topleft","ClanBuyBuildingFrameBackGroundButton","topleft", 6, 93);
			ClanBuyBuildingFrameSecondBackGroundButton:SetSize(221, 50 + #t_fonts * 30);
			ClanBuyBuildingFrame:Show();
			if nActorId ~= nil then
				util.testNpcDialogDistance( nActorId, ClanBuyBuildingFrame );
			end
		else
			-- 若是购买建筑
			ClanBuyBuildingFrame_CurClanLevelFont:SetText(ClanInfo.m_nClanLevel.."级")
			ClanBuyBuildingFrame_CurClanCityLevelFont:SetText(clanCityInfo.m_nLevel.."级")
			ClanBuyBuildingFrameFrameFont:SetText("建造"..t_BuildTypeName[clanCityBuildingInfo.m_nType])
			ClanBuyBuildingFrameSecondBackGroundButtonSecondTitleFont:SetText("满足以下需求即可建造"..t_BuildTypeName[clanCityBuildingInfo.m_nType])
			ClanBuyBuildingFrameSecondBackGroundButtonCostTimeFont:SetText("建造" .. t_BuildTypeName[clanCityBuildingInfo.m_nType] .. "需要花费" .. math.ceil(levelUpCityBuildingDef.TimeCost / 60) .. "分钟")
			ClanBuyBuildingFrameBackGroundButtonCurClanBuildingLevelHeadFont:SetText("")
			ClanBuyBuildingFrame_CurClanBuildingLevelFont:SetText("")
			ClanBuyBuildingFrame_CurClanBuildingLevelBtn:Hide();
			ClanBuyBuildingFrame:SetSize(239, 200 + #t_fonts * 30);
			ClanBuyBuildingFrameSecondBackGroundButton:SetPoint("topleft","ClanBuyBuildingFrameBackGroundButton","topleft", 6, 73);
			ClanBuyBuildingFrameSecondBackGroundButton:SetSize(221, 50 + #t_fonts * 30);
			ClanBuyBuildingFrame:Show();
			ClanBuyBuildingFrame:SetClientUserData( 0,clanCityBuildingInfo.m_nCurLevel + 1 );
			if nActorId ~= nil then
				util.testNpcDialogDistance( nActorId, ClanBuyBuildingFrame );
			end
		end
	end
	
	if prefix and t_fonts then
		local ui;
		for index, line in ipairs(t_fonts) do
			ui = getglobal(string.format("%s%d", prefix, index));
			ui:Show()
			ui = getglobal(string.format("%s%dTitle", prefix, index));
			ui:SetText(line.title);
			ui = getglobal(string.format("%s%dText", prefix, index));
			ui:SetText(line.text);
		end
		local index = #t_fonts + 1;
		while true do
			ui = getglobal(string.format("%s%d", prefix, index));
			if not ui then
				break;
			end
			ui:Hide();
			index = index + 1;
		end
	end
end

function ClanLevelUpFrame_OnShow()
	local ClanViewer		= GameClanManager:getClanViewer();
	local ClanInfo			= ClanViewer.m_clanInfo;
	local levelUpClanDef	= getClanLevelDef(ClanInfo.m_nClanLevel + 1);
	ClanLevelUpFrame_ClanLevelFont:SetText(levelUpClanDef.Level.."级");
	ClanLevelUpFrame_ClanMemNumFont:SetText(levelUpClanDef.MaxMemberNumLimit);
	ClanLevelUpFrame_ClanMoneyFont:SetText(levelUpClanDef.Required.Money);
	ClanLevelUpFrame_ClanBuildingValueFont:SetText(levelUpClanDef.Required.StableValue);
	this:SetPoint( "center","UIClient","center",0,0 );
end

function ClanLevelUpFrame_OnLoad()
	this:RegisterEvent( "GE_CLAN_SVR_INFO" );
end

function ClanLevelUpFrame_OnEvent()
	if arg1 == "GE_CLAN_SVR_INFO" then
		local ClanViewer		= GameClanManager:getClanViewer();
		local ClanInfo			= ClanViewer.m_clanInfo;
		local clanCityInfo		= GameClanManager:getClanCity():getCityInfo();
		local t_fonts = {};
		if ClanLevelUpFrame:IsShown() then
			if ClanLevelUpFrame:GetClientUserData( 0 ) ~= (ClanInfo.m_nClanLevel + 1) then
				ClanLevelUpFrame:Hide();
			end
		end
		if ClanCityRegionLevelUpFrame:IsShown() then
			if ClanCityRegionLevelUpFrame:GetClientUserData( 0 ) ~= (clanCityInfo.m_nLevel + 1) then
				ClanCityRegionLevelUpFrame:Hide();
			end
		end
		if ClanBuyCityRegionFrame:IsShown() then
			if clanCityInfo.m_nLevel > 0 then
				ClanBuyCityRegionFrame:Hide();
			end
		end
		if ClanBuyBuildingFrame:IsShown() then
			if clanCityInfo.m_nLevel > 0 then
				ClanBuyBuildingFrame:Hide();
			end
		end

		for _, data in ipairs( t_fonts ) do
			local font = getglobal( data.font );		
			font:SetText( data.value );
		end
	end
end

-- 升级工会
function ClanLevelUpFrame_CloseBtn_OnClick()
	ClanLevelUpFrame:Hide();
end

-- 点击升级工会面板中的立即升级按钮响应
function ClanLevelUpFrame_LevelUpBtn_OnClick()
	-- 检查当前操作人是否有权限进行升级公会操作
	local t_positionsCanLevelUp = { CLAN_POS_VICE_PRESIDENT, CLAN_POS_PRESIDENT };
	local mainplayer = ActorMgr:getMainPlayer();
	for _, position in ipairs( t_positionsCanLevelUp ) do
		if position == mainplayer:getClanPosition() then
			local ClanViewer = GameClanManager:getClanViewer();
			if ClanViewer.m_clanInfo.m_nClanLevel >= MAX_CLAN_LEVEL then
				ShowMidTips( "公会等级已达上限，无法再提升" );
				return;
			end
			GameClanManager:clan_Opt_Upgrade();
			local frame = getglobal( this:GetParent() );
			frame:Hide();
			return;
		end
	end
	ClanLevelUpFrame:Hide();
	ShowMidTips( "您现在没有权限进行升级公会的操作，将自动关闭该面板" );
end

-- 工会确认购买城市领地确认面板
local ClanBuyCityRegionFrameSpace = {};

function ClanBuyCityRegionFrame_OnLoad()
end

function ClanBuyCityRegionFrame_OnEvent()
end

function ClanBuyCityRegionFrame_OnShow()
	local frameDesc = "申请城市后可拥有自己公会的专属地图，可从斯蒂芬斯进入。地图中有贡献度商店、资源提交处、资源中心，可建设科技中心、工厂，让自己的公会变的更强大";
	ClanBuyCityRegionFrameDescRich:SetText( frameDesc, 255, 184, 70 );
	this:SetPoint( "center","UIClient","center",0,0 );
end

function ClanBuyCityRegionFrame_BuyBtn_OnClick()
	local city = GameClanManager:getClanCity();
	city:enableClanCity( city:getCityInfo().m_szName );
	
	local frame = getglobal( this:GetParent() );
	frame:Hide();
end

function ClanBuyCityRegionFrame_CancelBtn_OnClick()
end

-- 不需要材料升级城市面板的控件函数
-- 显示升级城市面板的时候响应
function ClanCityRegionLevelUpFrame_OnShow()
	this:SetPoint( "center","UIClient","center",0,0 );
end

function ClanBuyCityRegionFrame_CloseBtn_OnClick()
	local frame = getglobal( this:GetParent() );
	frame:Hide();
end

-- 点击立即升级按钮响应
function ClanCityRegionLevelUpFrame_LevelUpBtn_OnClick()
	local city = GameClanManager:getClanCity();
	city:upgradeClanCity( city:getCityInfo().m_szName );	
	local frame = getglobal( this:GetParent() );
	frame:Hide();
end

-- 点击取消按钮响应
function ClanCityRegionLevelUpFrame_CloseBtn_OnClick()
	local frame = getglobal( this:GetParent() );
	frame:Hide();
end

-- 确认购买建筑的面板相关事件处理函数 
function ClanBuyBuildingFrame_OnShow()
	this:SetPoint( "center","UIClient","center",0,0 );
end

-- 点击确认购买建筑的面板的X关闭按钮响应
function ClanBuyBuildingFrame_CloseBtn_OnClick()
	local frame = getglobal( this:GetParent() );
	frame:Hide();
end

-- 点击确认购买建筑的面板的立即购买按钮响应
function ClanBuyBuildingFrame_BuyBtn_OnClick()
	local city = GameClanManager:getClanCity();
	city:upgradeClanBuilding( city:getSelectBuilding().m_nType );

	
	local frame = getglobal( this:GetParent() );
	frame:Hide();
end


-------------------------------------------		公会生产相关内容	---------------------------------------------------

local MAX_CATAGORY_NUM	= 16;		--标签页的最大个数

local t_LabelNameList = {};
local nCurSelType = 0;			--当前选中的标签
local LabelNames = {}
local LabelTypes = {}
local LabelTypeOpen = {}
local LabelList = {}

function openClanMakingMachineFrame( nActorId )
	ClanProductFrame:Show();
	util.testNpcDialogDistance( nActorId, ClanProductFrame );
end

local firstMenuUV =		{	normal		= {	x = 868, y = 493, width = 156, height = 27 },
							highlight	= {	x = 868, y = 521, width = 156, height = 27 },
							push		= {	x = 417, y = 480, width = 156, height = 27 }
						};

--标签页的2级菜单贴图UV
local secondMenuUV =	{	normal		= {	x = 266, y = 911, width = 166, height = 35 },
							highlight	= {	x = 266, y = 948, width = 166, height = 35 },
							push		= {	x = 266, y = 984, width = 166, height = 35 },
						};

function SetClanProductFrameLabelList()
local newTypeOpen = {}
	LabelTypes = {}
	LabelNames = {}
	local clanAdv = GameClanManager:getAdvanceInfo()
	for i = 0, GameClanManager:getProductNum() - 1 do
		local productDef = GameClanManager:getProductDefByIndex(i)
		local clanViewer = GameClanManager:getClanViewer()

		if productDef then

			if ClanProductFrameProduceListSelectProduceListSelectString:GetText() == "全部" then
			
					local type = productDef.TypeName
					if type ~= "" and productDef.ResourceID == 0 then
						if not LabelNames[type] then
							LabelNames[type] = {}
							table.insert(LabelTypes, type)
							if LabelTypeOpen[type] then
								newTypeOpen[type] = true
							end
						end
						table.insert(LabelNames[type], i)
					end

			elseif ClanProductFrameProduceListSelectProduceListSelectString:GetText() == "可制造" then

				if clanAdv.Tech.TechLevel[productDef.TechRequired[0].TechCategory] >= productDef.TechRequired[0].Level and
				   clanViewer.m_clanInfo.m_nMoney >= productDef.NeedMoneyCost and
				   GameClanManager:getStorageResourceNum(productDef.NeedMaterialCost[0].ID) >= productDef.NeedMaterialCost[0].Num and
				   GameClanManager:getStorageResourceNum(productDef.NeedMaterialCost[1].ID) >= productDef.NeedMaterialCost[1].Num and
				   GameClanManager:getStorageResourceNum(productDef.NeedMaterialCost[2].ID) >= productDef.NeedMaterialCost[2].Num then
					local type = productDef.TypeName
					if type ~= "" and productDef.ResourceID == 0 then
						if not LabelNames[type] then
							LabelNames[type] = {}
							table.insert(LabelTypes, type)
							if LabelTypeOpen[type] then
								newTypeOpen[type] = true
							end
						end
						table.insert(LabelNames[type], i)
					end
				end

			elseif ClanProductFrameProduceListSelectProduceListSelectString:GetText() == "不可制造" then

				if clanAdv.Tech.TechLevel[productDef.TechRequired[0].TechCategory] < productDef.TechRequired[0].Level or
				   clanViewer.m_clanInfo.m_nMoney < productDef.NeedMoneyCost or
				   GameClanManager:getStorageResourceNum(productDef.NeedMaterialCost[0].ID) < productDef.NeedMaterialCost[0].Num or
				   GameClanManager:getStorageResourceNum(productDef.NeedMaterialCost[1].ID) < productDef.NeedMaterialCost[1].Num or
				   GameClanManager:getStorageResourceNum(productDef.NeedMaterialCost[2].ID) < productDef.NeedMaterialCost[2].Num then
					local type = productDef.TypeName
					if type ~= "" and productDef.ResourceID == 0 then
						if not LabelNames[type] then
							LabelNames[type] = {}
							table.insert(LabelTypes, type)
							if LabelTypeOpen[type] then
								newTypeOpen[type] = true
							end
						end
						table.insert(LabelNames[type], i)
					end
				end
			end
		end
	end
	LabelTypeOpen = newTypeOpen
	SortClanProductFrameLabelList()
end

function ClearSelectData()
	for i = 1, MAX_CLAN_MAKE_MATERIAL_TOLUA do
		local ui = getglobal("ClanProductFrameResourceNeedName"..i);
		if ui then
			ui:SetText("");
		end
		ui = getglobal("ClanProductFrameResourceTotalName"..i);
		if ui then
			ui:SetText("");
		end
	end
	ClanProductFrameSkillNeedBkgValue:SetText("");
	ClanProductFrameMoneyNeedBkgValue:SetText("");
	ClanProductFrameItemDescRich:SetText("",255,255,255);
	ClanProductFrameDescRich:SetText("",255,255,255);
	ClanProductFrameLeftTimeBkgValue:SetText("");
	ClanProductFrameMakingDescBkgIconButtonTex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
	ClanProductFrameProgressBkgTex:SetSize(0,13);
	local progressBtn = getglobal("ClanProductFrameProgressBkg");
	progressBtn:SetClientUserData(0,0);
	ClanProductFrameMakingBkgLeftMakeNum:SetText("");
end

function UpdateClanProductFrameSelectData()
	ClearSelectData();

	local productDef = GameClanManager:getProductDefByIndex( nCurSelType );
	local clanAdv = GameClanManager:getAdvanceInfo()
	local clanViewer = GameClanManager:getClanViewer()
	local nMakingNum = tonumber( ClanProductFrameMakeNumEdit:GetText() );

	if productDef then

		local clanTechDef = GameClanManager:getClanTechDef( productDef.TechRequired[0].TechCategory,productDef.TechRequired[0].Level );
		if clanTechDef then
			ClanProductFrameSkillNeedBkgValue:SetText(clanTechDef.Name);
		else
			ClanProductFrameSkillNeedBkgValue:SetText("")
		end
		
		if clanAdv.Tech.TechLevel[productDef.TechRequired[0].TechCategory] >= productDef.TechRequired[0].Level then
			ClanProductFrameSkillNeedBkgValue:SetTextColor(255, 255, 190)
		else
			local progress = GameClanManager:getTechProgress(clanTechDef.Category)
			local upgradeLevel = progress and progress.Value or 0

			if productDef.TechRequired[0].Level == upgradeLevel then
			
			local szText = string.format("%s（学习中)", clanTechDef.Name)
			ClanProductFrameSkillNeedBkgValue:SetText(szText)

			else
			
			local szText = string.format("%s（未学习)", clanTechDef.Name)
			ClanProductFrameSkillNeedBkgValue:SetText(szText)

			end

			ClanProductFrameSkillNeedBkgValue:SetTextColor(255, 0, 0)
		end

		ClanProductFrameMoneyNeedBkgValue:SetText( productDef.NeedMoneyCost * nMakingNum );

		if clanViewer.m_clanInfo.m_nMoney >= productDef.NeedMoneyCost * nMakingNum then
			ClanProductFrameMoneyNeedBkgValue:SetTextColor(255, 255, 190)
		else
			ClanProductFrameMoneyNeedBkgValue:SetTextColor(255, 0, 0)
		end

		ClanProductFrameItemDescRich:SetText( productDef.DescText,255,254,189 );
		local itemDef = getItemDef( productDef.ID );
		if itemDef then
			local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			ClanProductFrameItemIconButtonTex:Show();
			ClanProductFrameItemIconButtonTex:SetTexture( IconPath );
		else
			ClanProductFrameItemIconButtonTex:Hide();
		end
		local nNum = 1;
		for i=1, MAX_CLAN_MAKE_MATERIAL_TOLUA do
			if productDef.NeedMaterialCost[i-1].ID > 0 then
				local itemDef = getItemDef( productDef.NeedMaterialCost[i-1].ID );
				if itemDef ~= nil then
					local Need = getglobal("ClanProductFrameResourceNeedName"..i);
					local Total = getglobal("ClanProductFrameResourceTotalName"..i);
					local must = productDef.NeedMaterialCost[i-1].Num
					local max = GameClanManager:getStorageResourceNum(productDef.NeedMaterialCost[i-1].ID);
					if Need then
						Need:SetText(string.format("%s %d", itemDef.Name, must * nMakingNum));
						if must * nMakingNum <= max then
							Need:SetTextColor(255, 255, 190);
						else
							Need:SetTextColor(255, 0, 0);
						end
					end
					if Total then
						Total:SetText(string.format("%s %d", itemDef.Name, max));
					end
					nNum = nNum + 1;						
				end
			end
		end
		for i=nNum, MAX_CLAN_MAKE_MATERIAL_TOLUA do
			local ui = getglobal("ClanProductFrameResourceNeedName"..i);
			if ui then
				ui:SetText("");
			end
			ui = getglobal("ClanProductFrameResourceTotalName"..i);
			if ui then
				ui:SetText("");
			end
		end
		local makeProgress = GameClanManager:getProductProgressByIdx(0);
		local descRich = getglobal("ClanProductFrameDescRich");
		local iconTex = getglobal("ClanProductFrameMakingDescBkgIconButtonTex");
		local timeFont = getglobal("ClanProductFrameLeftTimeBkgValue");
		local progressTex = getglobal("ClanProductFrameProgressBkgTex");
		local progressBtn = getglobal("ClanProductFrameProgressBkg");
		if makeProgress ~= nil then
			productDef = GameClanManager:getProductDefByID( makeProgress.ID );
			descRich:SetText( "\t\t"..productDef.DescText,255,254,189 );
			if productDef.Category == 0 then
				local itemDef = getItemDef( productDef.ID );
				if itemDef ~= nil then
					local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
					if not IsInExistence( IconPath ) then
						IconPath = DEFAULT_ITEM_PATH;
					end
					iconTex:SetTexture( IconPath );
				end
			elseif productDef.Category == 1 then
				local IconID = getItemDef( productDef.ID ).IconID;
				local IconPath = GetItemEquipIconPath()..IconID..".tga";
				iconTex:SetTexture( IconPath );
			end
			local nServerTime = GameClanManager:getServerTime();
			local clanProductDef = GameClanManager:getProductDefByID( makeProgress.ID )
			local nLeftTime = nServerTime - makeProgress.EndTime;
			local nDelta = clanProductDef.TimeCost ;
			local nLeft = makeProgress.EndTime - nServerTime;
			local Scale =1-nLeft/nDelta;
			if Scale < 0 then
				Scale = 0;
			end
			if Scale > 1 then
				Scale = 1;
			end
			local szText = "";
			if nLeft > 0 then
				if nLeft >= 3600 then
					szText = szText .. math.floor( nLeft / 3600 ) .. "时";
				end
				if nLeft >= 60 then
					szText = szText .. math.floor( math.fmod( nLeft, 3600 ) / 60 ) .. "分";
				end
				-- 有小时的时候，不现在秒
				if makeProgress.EndTime - nServerTime < 3600 then
					szText = szText .. math.fmod( nLeft, 60 ) .. "秒";
				end
			end
			timeFont:SetText( szText );
			progressBtn:SetClientUserData( 0,makeProgress.EndTime );
			progressBtn:SetClientUserData( 1,makeProgress.BeginTime );
			progressTex:SetSize(340*Scale,13);
			ClanProductFrameMakingBkgLeftMakeNum:SetText( makeProgress.Value )
		end
		--ClanProductFrameMakeNumEdit:SetText( 1 );
	end
end


function ClanProductFrameProgressBkg_OnEnter()
	local nEndTime = this:GetClientUserData( 0 );
	local nLeftTime = nEndTime - GameClanManager:getServerTime();
	if nLeftTime > 0 then		
		local szText = "剩余时间：";
		if nLeftTime >= 3600 then
			szText = szText .. math.floor( nLeftTime / 3600 ) .. "时";
		end
		if nLeftTime >= 60 then
			szText = szText .. math.floor( math.fmod( nLeftTime, 3600 ) / 60 ) .. "分";
		end
		-- 有小时的时候，不现在秒
		if nEndTime - GameClanManager:getServerTime() < 3600 then
			szText = szText .. math.fmod( nLeftTime, 60 ) .. "秒";	
		end
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName(), horizonal = "right" } );
	end
end

function ClanProductFrameProgressBkg_OnUpdate()
	local nEndTime = this:GetClientUserData( 0 );
	local nStartTime = this:GetClientUserData( 1 );
	if nEndTime > 0 then
		local nPass =  - this:GetClientUserData( 2 );
		local scale = (GameClanManager:getServerTime() - nStartTime)/(nEndTime - nStartTime);
		if scale > 1 then
			scale = 1;
		end
		local progressTex = getglobal(this:GetName().."Tex");
		progressTex:SetSize(340*scale,13);
		local nLeftTime = nEndTime - GameClanManager:getServerTime();
		if nLeftTime > 0 then		
			local szText = "";
			if nLeftTime >= 3600 then
				szText = szText .. math.floor( nLeftTime / 3600 ) .. "时";
			end
			if nLeftTime >= 60 then
				szText = szText .. math.floor( math.fmod( nLeftTime, 3600 ) / 60 ) .. "分";
			end
			szText = szText .. math.fmod( nLeftTime, 60 ) .. "秒";
			ClanProductFrameLeftTimeBkgValue:SetText( szText );
		end
	end
end

function ClanProductFrame_ScrollBar_OnValueChanged()
	UpdateClanProductFrameCategoryList()
end

function ClanProductFrame_ScrollUpBtn_OnClick()
	local slider = ClanProductFrameScrollBar
	local value = slider:GetValue()
	if value > slider:GetMinValue() then
		slider:SetValue(value - 1)
	end
end

function ClanProductFrame_ScrollDownBtn_OnClick()
	local slider = ClanProductFrameScrollBar
	local value = slider:GetValue()
	if value < slider:GetMaxValue() then
		slider:SetValue(value + 1)
	end
end

function ClanProductFrame_CategoryBtn_OnClick()
	SelectClanProductFrameLabel(this:GetClientID())
end

function ClanProductFrame_OpenListBtn_OnClick()
	local parent = getglobal(this:GetParent())
	ClanProductFrameSelectCategoryBtnBtn(parent:GetClientID())
end

function ClanProductFrameSelectCategoryBtnBtn(i)
	local start = ClanProductFrameScrollBar:GetValue()
	local index = i + start
	local label = LabelList[index]
	LabelTypeOpen[label] = not LabelTypeOpen[label]
	SortClanProductFrameLabelList()
end

function SelectClanProductFrameLabel(i)
	local start = ClanProductFrameScrollBar:GetValue()
	local index = i + start
	local label = LabelList[index]
	if type(label) == type("") then
	else
		if nCurSelType then
			for i = 1, MAX_CATAGORY_NUM do
				local index = i + start
				local label = LabelList[index]
				if type(label) ~= type("") and label == nCurSelType then
					local btn = getglobal("ClanProductFrameCategoryBtn"..i.."HighlightBtn")
					btn:DisChecked()
					break
				end
			end
		end
		nCurSelType = label
		local btn = getglobal("ClanProductFrameCategoryBtn"..i.."HighlightBtn")
		btn:Checked()
		ClanProductFrameMakeNumEdit:SetText(1)
		UpdateClanProductFrameSelectData()
	end
end

function UpdateClanProductFrameCategoryList()
	local clanAdv = GameClanManager:getAdvanceInfo()
	local clanViewer = GameClanManager:getClanViewer()
	
	local start = ClanProductFrameScrollBar:GetValue()
	local n = #LabelList
	for i = 1, MAX_CATAGORY_NUM do
		if i + start > n then
			break
		end

		local btn = getglobal("ClanProductFrameCategoryBtn"..i.."HighlightBtn")
		local font = getglobal("ClanProductFrameCategoryBtn"..i.."HighlightBtnType")
		local openBtn = getglobal("ClanProductFrameCategoryBtn"..i.."HighlightBtnOpenList")
		local label = LabelList[i + start]

		btn:Show()

		if type(label) == type("") then
			btn:SetNormalTextureTemplate("TransTextureTemplate")
			btn:SetHighlightTextureTemplate("TransTextureTemplate")
			btn:SetPushedTextureTemplate("TransTextureTemplate")
			btn:SetDisableTextureTemplate("TransTextureTemplate")
			local itemName = string.format("    %s", label)
			font:SetText(itemName)
			font:SetTextColor(255, 255, 190)
			if LabelTypeOpen[label] then
				openBtn:SetNormalTextureTemplate("$parentCloseListNormalTexTemplate")
				openBtn:SetHighlightTextureTemplate("$parentCloseListHighlightTexTemplate")
				openBtn:SetPushedTextureTemplate("$parentCloseListPushedTexTemplate")		
			else
				openBtn:SetNormalTextureTemplate("$parentOpenListNormalTexTemplate")
				openBtn:SetHighlightTextureTemplate("$parentOpenListHighlightTexTemplate")
				openBtn:SetPushedTextureTemplate("$parentOpenListPushedTexTemplate")
			end
			openBtn:Show();
		else
			openBtn:Hide();
			btn:SetNormalTextureTemplate("TransTextureTemplate")
			btn:SetHighlightTextureTemplate("HighlightTextureTemplate")
			btn:SetPushedTextureTemplate("PushedTextureTemplate")
			btn:SetDisableTextureTemplate("TransTextureTemplate")
			font:SetText(GameClanManager:getProductDefByIndex(label).Name)
			
			local productDef = GameClanManager:getProductDefByIndex(label)

			if clanAdv.Tech.TechLevel[productDef.TechRequired[0].TechCategory] >= productDef.TechRequired[0].Level and
				   clanViewer.m_clanInfo.m_nMoney >= productDef.NeedMoneyCost and
				   GameClanManager:getStorageResourceNum(productDef.NeedMaterialCost[0].ID) >= productDef.NeedMaterialCost[0].Num and
				   GameClanManager:getStorageResourceNum(productDef.NeedMaterialCost[1].ID) >= productDef.NeedMaterialCost[1].Num and
				   GameClanManager:getStorageResourceNum(productDef.NeedMaterialCost[2].ID) >= productDef.NeedMaterialCost[2].Num then

			font:SetTextColor(160, 160, 90)

			else

			font:SetTextColor(255, 0, 0)
			
			end

			if nCurSelType == label then
				btn:Checked()
			else
				btn:DisChecked()
			end
		end
	end
	for i = n - start + 1, MAX_CATAGORY_NUM do
		local btn = getglobal("ClanProductFrameCategoryBtn"..i.."HighlightBtn")
		btn:Hide()
	end
end

function SortClanProductFrameLabelList()
	LabelList = {}
	for _, type in ipairs(LabelTypes) do
		table.insert(LabelList, type)
		if LabelTypeOpen[type] then
			for _, index in ipairs(LabelNames[type]) do
				table.insert(LabelList, index)
			end
		end
	end
	local slider = ClanProductFrameScrollBar
	local n = #LabelList
	local diff = n - MAX_CATAGORY_NUM
	if diff > 0 then
		if not slider:IsShown() then
			slider:Show()
		end
		slider:SetMaxValue(diff)
		if slider:GetValue() > diff then
			slider:SetValue(diff)
		end
	else
		slider:Hide()
		slider:SetValue(0)
	end
	UpdateClanProductFrameCategoryList()
end

--开始制造
function ClanProductFrameStartBtn_OnClick()
	local productDef = GameClanManager:getProductDefByIndex( nCurSelType );
	local nNum = tonumber( ClanProductFrameMakeNumEdit:GetText() );
	if nNum == nil or nNum < 1 then
		ShowMidTips("请输入制造数量");
		return;
	end
	if productDef ~= nil then		
		GameClanManager:clan_Opt_Make(productDef.ID, nNum);
	end
end

--停止制造
function ClanProductFrameStopBtn_OnClick()
	local makeProgress = GameClanManager:getProductProgressByIdx(0);
	if makeProgress == nil then
		ShowMidTips("没有正在制造的物品");
		return;
	end
	GameClanManager:clan_Opt_CancleMake(makeProgress.ID);
end

--加速制造
function ClanProductFrameSpeedBtn_OnClick()
	local makeProgress = GameClanManager:getProductProgressByIdx(0);
	if makeProgress == nil then
		ShowMidTips("没有正在制造的物品");
		return;
	end
	ClanAddSpeedFrame:SetClientUserData( 3,makeProgress.ID );
	ClanAddSpeedFrame:SetClientUserData( 0,ADD_TYPE_PRODUCE );
	ClanAddSpeedFrame:Show();
end

function ClanProductFrame_OnHide()
	if ClanAddSpeedFrame:IsShown() and ClanAddSpeedFrame:GetClientUserData( 0 ) == ADD_TYPE_PRODUCE then
		ClanAddSpeedFrame:Hide();
	end
end

function ClanProductFrame_OnLoad()	
	this:RegisterEvent("GE_CLAN_MAKING_UPDATE");
	this:RegisterEvent("GE_CLAN_STORAGE_UPDATE");
	this:RegisterEvent("GE_CLAN_SVR_INFO");
end

function ClanProductFrame_OnEvent()
	if arg1 == "GE_CLAN_MAKING_UPDATE" or arg1 == "GE_CLAN_STORAGE_UPDATE" then
		if ClanProductFrame:IsShown() then
			SetClanProductFrameLabelList();
			UpdateClanProductFrameSelectData();
		end
	elseif arg1 == "GE_CLAN_SVR_INFO" then
		if ClanProductFrame:IsShown() then
			SetClanProductFrameLabelList();
			UpdateClanProductFrameSelectData();
		end
	end
end

function ClanProductFrame_OnShow()
	util.ShowInCenter(this)
	nCurSelType = nil;
	LabelTypeOpen = {}
	ClanProductFrameMakeNumEdit:SetText(1);
	SetClanProductFrameLabelList();
	ClanProductFrameScrollBar:SetValue(0);

	ClanProductFrameSelectListBoxSelect(1)

	if #LabelList > 0 then
		SelectClanProductFrameLabel(2)
	end
end

function ClanProductFrameProduceListSelectDownBtn_Click()
	local listBox = getglobal("ClanProductFrameSelectListBox")
	if listBox:IsShown() then
		listBox:Hide()
	else
		listBox:Show()
	end
end

function ClanProductFrameProduceSelectListBox_OnShow()
	ClanProductFrameSelectListBoxSelectBtn1LayerName:SetText("全部")
	ClanProductFrameSelectListBoxSelectBtn2LayerName:SetText("可制造")
	ClanProductFrameSelectListBoxSelectBtn3LayerName:SetText("不可制造")
end

function ClanProductFrameProduceSelectListBoxBtn_OnClick()
	local id = this:GetClientID()
	ClanProductFrameSelectListBoxSelect(id)

	local listBox = getglobal("ClanProductFrameSelectListBox")
	listBox:Hide()
end

function ClanProductFrameSelectListBoxSelect(id)
	
	if id == 1 then
		ClanProductFrameProduceListSelectProduceListSelectString:SetText("全部")

	elseif id == 2 then
		ClanProductFrameProduceListSelectProduceListSelectString:SetText("可制造")

	elseif id == 3 then
		ClanProductFrameProduceListSelectProduceListSelectString:SetText("不可制造")

	end
	
	t_LabelNameList = {};
	nCurSelType = 0;
	LabelNames = {}
	LabelTypes = {}
	LabelTypeOpen = {}
	LabelList = {}

	SetClanProductFrameLabelList()
	
	local start = ClanProductFrameScrollBar:GetValue()
	local n = #LabelList
	local firstBtn = false

	for i = 1, MAX_CATAGORY_NUM do
		if i + start > n then
			break
		end

		local label = LabelList[i + start]
		if type(label) == type("") then
			if firstBtn == false then
				firstBtn = true
				LabelTypeOpen[label] = true
			else
				LabelTypeOpen[label] = false
			end
		end
	end

	SortClanProductFrameLabelList()
	UpdateClanProductFrameCategoryList()

	if #LabelList > 0 then
		SelectClanProductFrameLabel(2)
	end

end

function ClanProductFrameItemBtn_OnEnter()
	local productDef = GameClanManager:getProductDefByIndex( nCurSelType );
	local itemDef = getItemDef( productDef.ID );
	ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, itemDef );
end

function ClanProductFrameItemBtn_OnLeave()
	NormalItemTipsFrame:Hide();
	ArmItemTipsFrame:Hide();
	SecondArmItemTipsFrame:Hide();
end

function ClanProductFrameMakingItemBtn_OnEnter()
	local szPath = ClanProductFrameMakingDescBkgIconButtonTex:GetTexture()
	if szPath ~= "uires\\ui\\mask\\TouMingTongDao.tga" then
		local makeProgress = GameClanManager:getProductProgressByIdx(0);
		if makeProgress then
			local itemDef = getItemDef(makeProgress.ID);
			ShowItemTips(LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, itemDef);
		end
	end
end

function ClanProductFrameNumEditOnFocus()
	timer(function()
		ClanProductFrameMakeNumEdit:setSelBegin(0)
		ClanProductFrameMakeNumEdit:setCursorPos(#ClanProductFrameMakeNumEdit:GetText())
	end)
end

function ClanProductFrameNumEditUpdate()
	ClanProductFrameChangeNeed()
end

function ClanProductFrameChangeNeed()

	local nMakingNum = tonumber( ClanProductFrameMakeNumEdit:GetText() );
	local productDef = GameClanManager:getProductDefByIndex( nCurSelType );
	local clanViewer = GameClanManager:getClanViewer();

		for i=1, MAX_CLAN_MAKE_MATERIAL_TOLUA do
			if productDef.NeedMaterialCost[i-1].ID > 0 then
				local itemDef = getItemDef( productDef.NeedMaterialCost[i-1].ID );
				if itemDef ~= nil then
					local Need = getglobal("ClanProductFrameResourceNeedName"..i);
					local must = productDef.NeedMaterialCost[i-1].Num
					local max = GameClanManager:getStorageResourceNum(productDef.NeedMaterialCost[i-1].ID);
					if Need then
						if nMakingNum == nil then
							nMakingNum = 0
						end

						Need:SetText(string.format("%s %d", itemDef.Name, must * nMakingNum));
						if must * nMakingNum <= max then
							Need:SetTextColor(255, 255, 190);
						else
							Need:SetTextColor(255, 0, 0);
						end
					end					
				end
			end
		end

	ClanProductFrameMoneyNeedBkgValue:SetText( productDef.NeedMoneyCost * nMakingNum );

	if productDef.NeedMoneyCost * nMakingNum <= clanViewer.m_clanInfo.m_nMoney then
		ClanProductFrameMoneyNeedBkgValue:SetTextColor(255, 255, 190);
	else
		ClanProductFrameMoneyNeedBkgValue:SetTextColor(255, 0, 0);
	end

end

-------------------		公会科技学习界面	------------------

local MAX_CLAN_SKILL_TYPE = 4;
local MAX_CLAN_SKILL_NUM = 4;
local MAX_LEARNING_NUM = 3;
local nCurClanSkillSelType= 1;
local nClanLearnCurPage = 1;
local nClanLearnMaxPage = 1;

function openClanResearchFrame( nActorId )
	ClanLearnSkillFrame:Show();
	util.testNpcDialogDistance( nActorId, ClanLearnSkillFrame );
end

local t_ClanSkillType = {
							{"蒸汽科技","uires\\TuPianLei\\WuPin\\5108401.tga"},
							{"采集科技","uires\\TuPianLei\\WuPin\\5108400.tga"},
							{"蒸汽机械","uires\\TuPianLei\\WuPin\\5108402.tga"},
							{"城防科技","uires\\TuPianLei\\WuPin\\5108403.tga"},
						}

function InitClanTechTypeBtn(prefix)
	for i=1,MAX_CLAN_SKILL_TYPE do
		local IconTex = getglobal(prefix..i.."IconTexture");
		local nameFont = getglobal(prefix..i.."NameFont");
		nameFont:SetText( t_ClanSkillType[i][1] );
		IconTex:SetTexture( t_ClanSkillType[i][2] );
	end
end

function ClanLearnSkillFrame_OnLoad()
	InitClanTechTypeBtn("ClanLearnSkillFrameTypeBtn");
	for i=1,MAX_CLAN_SKILL_NUM do
		local tex = getglobal( "ClanLearnSkillFrameSkillInfo"..i.."TexBtnTex" );
		tex:setMask("uires\\ui\\mask\\GongHuiYuanXingTuBiaoBiaoShi  mask.tga");
	end
	this:RegisterEvent("GE_CLAN_ADVANCE_UPDATE");
end

function ClanLearnSkillFrame_OnEvent()
	if arg1 == "GE_CLAN_ADVANCE_UPDATE" then
		if ClanLearnSkillFrame:IsShown() then
			local curSelFont = getglobal("ClanLearnSkillFrameTypeBtn"..nCurClanSkillSelType.."NameFont");
			nClanLearnCurPage, nClanLearnMaxPage = ClanLearnSkillFrameUpdateSkill(curSelFont:GetText(), "ClanLearnSkillFrameSkillInfo", "ClanLearnSkillFrameLearnBtn", "ClanLearnSkillFrame_ScrollBar", nClanLearnCurPage, ClanSkillFilter)
			ClanLearnSkillFramePagePageFont:SetText(nClanLearnCurPage.."/"..nClanLearnMaxPage);
		end
	end
end

function BondClanLearnSkillFrameBasicInfo()	
	local ClanViewer = GameClanManager:getClanViewer();
	local clanAdv = GameClanManager:getAdvanceInfo();
	ClanLearnSkillFrameClanTotalSkillTotalSkillValue:SetText( ClanViewer.m_clanInfo.m_nSkillPoint );
	ClanLearnSkillFrameClanTotalSkillTotalMoneyValue:SetText( ClanViewer.m_clanInfo.m_nMoney );
	ClanLearnSkillFrameClanTotalSkillUsedSkillValue:SetText( clanAdv.UsedTechPoint );
	ClanFrameTechUsedTechBtnAllTechFont:SetText( clanAdv.UsedTechPoint );
end

function ClanLearnSkillFrame_OnShow()
	nCurClanSkillSelType = 1;
	GameClanManager:clan_Opt_GetClanInfo();
	ClanLearnSkillFrameDisCheckTypeBtn();
	ClanLearnSkillFrameTypeBtn1:Checked();
	local curSelFont = getglobal("ClanLearnSkillFrameTypeBtn"..nCurClanSkillSelType.."NameFont");
	nClanLearnCurPage, nClanLearnMaxPage = ClanLearnSkillFrameUpdateSkill(curSelFont:GetText(), "ClanLearnSkillFrameSkillInfo", "ClanLearnSkillFrameLearnBtn", "ClanLearnSkillFrame_ScrollBar", nClanLearnCurPage, ClanSkillFilter)
	ClanLearnSkillFramePagePageFont:SetText(nClanLearnCurPage.."/"..nClanLearnMaxPage);
	this:SetPoint("center","$parent","center",0,0);
	local richText = getglobal("ClanLearnSkillFrameRichText");
	richText:SetText("科技点获得：#L#cff0000工会工厂#n内生产道具都可获得科技点" ,120, 235, 50);
	AdditionalFindLearnableTechAndShowHelp()
end

function ClanLearnSkillFrame_OnHide()
	if ClanAddSpeedFrame:IsShown() and ClanAddSpeedFrame:GetClientUserData( 0 ) == ADD_TYPE_TECH then
		ClanAddSpeedFrame:Hide();
	end
end

function ClanLearnSkillFrameUpdateSkill(type, info, learning, scroll, page, filter)
	SetClanLearnSkillFrameScroll(type, scroll, filter);
	local silder = getglobal(scroll)
	if silder:IsShown() then
		BondClanLearnSkillFrameSkilllInfo(type, silder:GetValue() + 1, info, filter);
	else
		BondClanLearnSkillFrameSkilllInfo(type, 1, info, filter);
	end
	BondClanLearnSkillFrameSkillProgress(info);
	nClanLearnCurPage = 1;
	local cur, max = ClanLearnSkillFrameRefreshLearning(learning, page);
	BondClanLearnSkillFrameBasicInfo();
	return cur, max
end

--设置滚动条
function SetClanLearnSkillFrameScroll(type, scroll, filter)
	local ClanAdvInfo = GameClanManager:getAdvanceInfo();
	local nNum = 0;
	for i=1,MAX_CLAN_TECH_CATEGORY_TOLUA-1 do
		local clanSkillDef = GameClanManager:getClanTechDef( i,1 );
		if clanSkillDef~=nil and clanSkillDef.TypeName == type then
			if filter then
				for j = 1, GameClanManager:getMaxSkillLevel( i ) do
					if filter(i, j) then
						nNum = nNum + 1
					end
				end
			else
				nNum = nNum + GameClanManager:getMaxSkillLevel( i );
			end
		end
	end
	SetScrollBar(scroll, MAX_CLAN_SKILL_NUM, nNum);
end

function BondClanLearnSkillFrameSkilllInfo(type, index, prefix, filter)
	local nIndex = 1;
	local nNum = 0;
	local t = {}
	for i = 0, MAX_CLAN_TECH_CATEGORY_TOLUA - 1 do
		local level = 1
		while true do
			local clanSkillDef = GameClanManager:getClanTechDef(i, level)
			if not clanSkillDef then
				break
			end
			if clanSkillDef.TypeName == type then
				if not filter or filter(i, level) then
					local state = 4
					if IsClanSkillLearned(i, level) then
						state = 1
					else
						local progress = GameClanManager:getTechProgress(i)
						local upgradeLevel = progress and progress.Value or 0
						if level == upgradeLevel then
							state = 2
						elseif CanLearnClanSkill(i, level) then
							state = 3
						end
					end
					table.insert(t, {id = i, level = level, state = state})
				end
			end
			level = level + 1
		end
	end
	table.sort(t, function(a, b)
	if a.state == b.state then
			if a.state == 4 then
				local require1 = GameClanManager:getClanTechDef(a.id, a.level).Required   --Elmer
				local require2 = GameClanManager:getClanTechDef(b.id, b.level).Required
				if require1 == require2 then 
					local existNeedTech1 = ( ExistNeedTech( a.id, a.level ) ~= 0 and 1 ) or 0
					local existNeedTech2 = ( ExistNeedTech( b.id, b.level ) ~= 0 and 1 ) or 0
					
					if existNeedTech1 == existNeedTech2 then
						local needTechPoint1 = GameClanManager:getClanTechDef(a.id, a.level).NeedTechPointCost
						local needTechPoint2 = GameClanManager:getClanTechDef(b.id, b.level).NeedTechPointCost
						return needTechPoint1 < needTechPoint2
					else
						return existNeedTech1 < existNeedTech2
					end
				else
					return require1 < require2
				end
			else
				local existNeedTech1 = ( ExistNeedTech( a.id, a.level ) ~= 0 and 1 ) or 0
				local existNeedTech2 = ( ExistNeedTech( b.id, b.level ) ~= 0 and 1 ) or 0
				if existNeedTech1 == existNeedTech2 then
					local needTechPoint1 = GameClanManager:getClanTechDef(a.id, a.level).NeedTechPointCost
					local needTechPoint2 = GameClanManager:getClanTechDef(b.id, b.level).NeedTechPointCost
					return needTechPoint1 < needTechPoint2
				else
					return existNeedTech1 < existNeedTech2
				end
			end
		end
		return a.state < b.state
	end)
	for i = index, #t do
		if nIndex > MAX_CLAN_SKILL_NUM then
			break
		end
		local clanSkillDef = GameClanManager:getClanTechDef(t[i].id, t[i].level)
		local btn = getglobal(prefix..nIndex)
		local skillNameFont = getglobal(prefix..nIndex.."SkillDescLabel")
		local DescRich = getglobal(prefix..nIndex.."DescRich")
		local IconTex = getglobal(prefix..nIndex.."TexBtnTex")
		local progressBtn = getglobal(prefix..nIndex.."ProgressBtn")
		local progressTex = getglobal(prefix..nIndex.."ProgressBtnProgressTex")
		local LearnBtn = getglobal(prefix..nIndex.."LearnBtn")
		local LearnFont = getglobal(prefix..nIndex.."LearnBtnFont")
		local LearnStateFont = getglobal(prefix..nIndex.."LearnState")
		
		DescRich:SetText("\t"..clanSkillDef.DescText, 255, 254, 189)
		skillNameFont:SetText(clanSkillDef.Name.."：")
		local IconPath = GetItemEquipIconPath()..clanSkillDef.IconID..".tga"
		if not IsInExistence(IconPath) then
			IconPath = DEFAULT_ITEM_PATH
		end
		IconTex:SetTexture(IconPath)
		local state = t[i].state
		--LearnBtn:Enable()
		if state == 1 then
			LearnBtn:Hide()
			LearnFont:SetText("已 学 习")
			LearnStateFont:SetTextColor(204, 128, 51)
			LearnStateFont:SetText("已学习")
		elseif state == 2 then
			LearnBtn:Show()
			LearnFont:SetText("正在学习")
			LearnStateFont:SetText("")
		elseif state == 3 then
			LearnBtn:Show()
			LearnFont:SetText("学  习")
			LearnStateFont:SetTextColor(120, 235, 50)
			LearnStateFont:SetText("可学习")
		elseif state == 4 then
			LearnBtn:Show()
			LearnFont:SetText("学  习")
			LearnStateFont:SetTextColor(253, 38, 52)
			LearnStateFont:SetText("条件不足")
		end
		progressTex:SetSize(0, 13)
		progressBtn:SetClientUserData(0, 0)
		btn:SetClientUserData(0, t[i].id)
		btn:SetClientUserData(1, t[i].level)
		btn:Show()
		nIndex = nIndex + 1
	end
	for i = nIndex, MAX_CLAN_SKILL_NUM do
		local btn =	getglobal(prefix..i)
		btn:Hide()
	end
end

function BondClanLearnSkillFrameSkillProgress(info)
	local ClanAdvInfo = GameClanManager:getAdvanceInfo();
	for i=1,MAX_CLAN_SKILL_NUM do
		local btn = getglobal(  info.. i );
		if btn:IsShown() then
			local nCate = btn:GetClientUserData( 0 );
			local level = btn:GetClientUserData( 1 );
			local progress = GameClanManager:getTechProgress( nCate );
			if progress and progress.Value == level then
				local progressBtn = getglobal(info..i.."ProgressBtn");
				local progressTex = getglobal(info..i.."ProgressBtnProgressTex");
				local nServerTime = GameClanManager:getServerTime();
				local clanSkillDef = GameClanManager:getClanTechDef( nCate,progress.Value );
				local nDelta =clanSkillDef.TimeCost;
				local nPass = nServerTime - progress.BeginTime + nDelta - ( progress.EndTime - progress.BeginTime );
				local Scale = nPass/nDelta;
				if Scale < 0 then
					Scale = 0;
				end
				if Scale > 1 then
					Scale = 1;
				end
				progressTex:SetSize(340*Scale,13);
				progressBtn:SetClientUserData( 0,progress.EndTime );
				progressBtn:SetClientUserData( 1,nDelta);
				progressBtn:SetClientUserData( 2,progress.BeginTime);
			end
		end
	end
end

--刷新最右边的学习列表
function ClanLearnSkillFrameRefreshLearning(learning, page)
	local clanAdv = GameClanManager:getAdvanceInfo();
	local nNum = clanAdv.Tech.TechResearchNum;
	local nClanLearnCurPage = page;
	local nClanLearnMaxPage = math.floor((nNum -1)/MAX_LEARNING_NUM) + 1;
	if nClanLearnMaxPage == 0 then
		nClanLearnMaxPage = 1;
	end
	if nClanLearnCurPage > nClanLearnMaxPage then
		nClanLearnCurPage = nClanLearnMaxPage;
	end
	local nIndex = 1;
	local nTotal = 0;
	for i=1,nNum do
		if nIndex > MAX_LEARNING_NUM then
			break;
		end
		local progress = clanAdv.Tech.TechResearchProgress[i-1];
		if progress ~= nil and progress.Category >0 then
			local clanTech = GameClanManager:getClanTechDef( progress.Category,progress.Value );
			if clanTech ~= nil then
				nTotal = nTotal + 1;
				if nTotal > (nClanLearnCurPage-1)*MAX_LEARNING_NUM then
					local btn = getglobal(learning..nIndex);
					local nameFont = getglobal( learning..nIndex.."Name" );
					local TimeFont = getglobal(learning..nIndex.."Time");
					nameFont:SetText( clanTech.Name );
					
					local nServerTime = GameClanManager:getServerTime();
					local nLeft = progress.EndTime - nServerTime;
					local szText = "";
					if nLeft > 0 then
						if nLeft >= 3600 then
							szText = szText .. math.floor( nLeft / 3600 ) .. "时";
						end
						if nLeft >= 60 then
							szText = szText .. math.floor( math.fmod( nLeft, 3600 ) / 60 ) .. "分";
						end
						szText = szText .. math.fmod( nLeft, 60 ) .. "秒";
					end
					TimeFont:SetText(szText);
					btn:SetClientUserData( 0,progress.EndTime );
					btn:SetClientID( i-1 );
					btn:Show();
					nIndex = nIndex + 1;
				end
			end
		end		
	end
	for i=nIndex,MAX_LEARNING_NUM do
		local btn = getglobal(learning..i);
		btn:Hide();
	end
	return nClanLearnCurPage, nClanLearnMaxPage
end

function ClanLearnSkillFrameDisCheckTypeBtn()
	for i=1,MAX_CLAN_SKILL_TYPE do
		local btn = getglobal( "ClanLearnSkillFrameTypeBtn"..i );
		btn:DisChecked();
	end
end

function ClanLearnSkillFrameTypeBtn_OnClick()
	nCurClanSkillSelType = this:GetClientID();
	ClanLearnSkillFrameDisCheckTypeBtn();
	this:Checked();
	local curSelFont = getglobal("ClanLearnSkillFrameTypeBtn"..nCurClanSkillSelType.."NameFont");
	nClanLearnCurPage, nClanLearnMaxPage = ClanLearnSkillFrameUpdateSkill(curSelFont:GetText(), "ClanLearnSkillFrameSkillInfo", "ClanLearnSkillFrameLearnBtn", "ClanLearnSkillFrame_ScrollBar", nClanLearnCurPage, ClanSkillFilter)
	ClanLearnSkillFramePagePageFont:SetText(nClanLearnCurPage.."/"..nClanLearnMaxPage);
	AdditionalFindLearnableTechAndShowHelp()
end

--学习
function ClanLearnSkillFrameLearn_OnClick()
	local ClanAdvInfo = GameClanManager:getAdvanceInfo();
	local parent = getglobal(this:GetParent());
	local nCat = parent:GetClientUserData( 0 );
	local nLevel = parent:GetClientUserData( 1 );
	local CurrentLevel = ClanAdvInfo.Tech.TechLevel[nCat];
	if nLevel <= CurrentLevel then
		return;
	end
	local progress = GameClanManager:getTechProgress(nCat);
	if progress and progress.Value == nLevel then
		return;
	end
	GameClanManager:clan_Opt_Research(nCat, nLevel);

	if not GameWizard:isDo(GAMEWIZARD_FIRST_BLINK_HAVE_CLAN_LEARN_TECH) then
		GameWizard:doStep(GAMEWIZARD_FIRST_BLINK_HAVE_CLAN_LEARN_TECH)

		for i = 1, 4 do
				btn = getglobal("ClanSkillInfo" .. i .. "LearnBtn")
				local animationTexture = getglobal(btn:GetName().."UVAnimationTex")
				animationTexture:Hide()
				btn = getglobal("ClanSkillInfo" .. i .. "LearnBtnHelp")
				btn:Hide()
		end
	end
end

--学习
function AdditionalClanLearnSkillFrameLearn_OnClick()
	local ClanAdvInfo = GameClanManager:getAdvanceInfo();
	local parent = getglobal(this:GetParent());
	local nCat = parent:GetClientUserData( 0 );
	local nLevel = parent:GetClientUserData( 1 );
	local CurrentLevel = ClanAdvInfo.Tech.TechLevel[nCat];
	if nLevel <= CurrentLevel then
		return;
	end
	local progress = GameClanManager:getTechProgress(nCat);
	if progress and progress.Value == nLevel then
		return;
	end
	GameClanManager:clan_Opt_Research(nCat, nLevel);

	if not GameWizard:isDo(GAMEWIZARD_FIRST_BLINK_HAVE_CLAN_LEARN_TECH) then
		GameWizard:doStep(GAMEWIZARD_FIRST_BLINK_HAVE_CLAN_LEARN_TECH)

		for i = 1, 4 do
				btn = getglobal("ClanLearnSkillFrameSkillInfo" .. i .. "LearnBtn")
				local animationTexture = getglobal(btn:GetName().."UVAnimationTex")
				animationTexture:Hide()
				btn = getglobal("ClanLearnSkillFrameSkillInfo" .. i .. "LearnBtnHelp")
				btn:Hide()
		end
	end
end

--加速
function ClanLearnSkillFrameSpeedBtn_OnClick()
	local parent = getglobal( this:GetParent() );
	local nIndex = parent:GetClientID();
	local clanAdv = GameClanManager:getAdvanceInfo();
	ClanAddSpeedFrame:SetClientUserData( 0,ADD_TYPE_TECH );
	ClanAddSpeedFrame:SetClientUserData( 2,clanAdv.Tech.TechResearchProgress[nIndex].Category );
	ClanAddSpeedFrame:Show();
end

--取消
function ClanLearnSkillFrameCancelBtn_OnClick()
	local parent = getglobal( this:GetParent() );
	local nIndex = parent:GetClientID();
	GameClanManager:clan_Opt_CancleResearch( nIndex );
end

function ClanLearnSkillFrameLearnBtn_OnUpdate()
	local TimeFont = getglobal(this:GetName().."Time");
	local nServerTime = GameClanManager:getServerTime();
	local nLeft =this:GetClientUserData( 0 ) - nServerTime;
	local szText = "";
	if nLeft > 0 then
		if nLeft >= 3600 then
			szText = szText .. math.floor( nLeft / 3600 ) .. "时";
		end
		if nLeft >= 60 then
			szText = szText .. math.floor( math.fmod( nLeft, 3600 ) / 60 ) .. "分";
		end
		szText = szText .. math.fmod( nLeft, 60 ) .. "秒";
	end
	TimeFont:SetText(szText);
end

--翻页
function ClanLearnSkillFrameLeftPageBtn_OnClick()
	if nClanLearnCurPage > 1 then
		nClanLearnCurPage = nClanLearnCurPage - 1;
		nClanLearnCurPage, nClanLearnMaxPage = ClanLearnSkillFrameRefreshLearning("ClanLearnSkillFrameLearnBtn", nClanLearnCurPage);
		ClanLearnSkillFramePagePageFont:SetText(nClanLearnCurPage.."/"..nClanLearnMaxPage);
	end
end

function ClanLearnSkillFrameRightPageBtn_OnClick()
	if nClanLearnCurPage < nClanLearnMaxPage then
		nClanLearnCurPage = nClanLearnCurPage + 1;
		nClanLearnCurPage, nClanLearnMaxPage = ClanLearnSkillFrameRefreshLearning("ClanLearnSkillFrameLearnBtn", nClanLearnCurPage);
		ClanLearnSkillFramePagePageFont:SetText(nClanLearnCurPage.."/"..nClanLearnMaxPage);
	end
end

function ClanLearnSkillFrame_ScrollUpBtn_OnClick()
	local slider = getglobal("ClanLearnSkillFrame_ScrollBar");
	if not slider:IsShown() then
		return;
	end

	local nValue = slider:GetValue() - slider:GetValueStep();

	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end
function ClanLearnSkillFrame_ScrollDownBtn_OnClick()
	local slider = getglobal( "ClanLearnSkillFrame_ScrollBar" );
	if not slider:IsShown() then
		return;
	end

	local nMaxValue = slider:GetMaxValue();
	local nValue	= slider:GetValue() + slider:GetValueStep();

	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end
function ClanLearnSkillFrame_ScrollBar_OnValueChanged()
	local slider	= ClanLearnSkillFrame_ScrollBar;
	local curSelFont = getglobal("ClanLearnSkillFrameTypeBtn"..nCurClanSkillSelType.."NameFont");
	BondClanLearnSkillFrameSkilllInfo(curSelFont:GetText(), slider:GetValue() + 1, "ClanLearnSkillFrameSkillInfo", ClanSkillFilter);
	BondClanLearnSkillFrameSkillProgress("ClanLearnSkillFrameSkillInfo");

	AdditionalFindLearnableTechAndShowHelp()
end

function ClanLearnSkillFrameOnlyCanLearn_OnClick()
	local slider	= ClanLearnSkillFrame_ScrollBar;
	local curSelFont = getglobal("ClanLearnSkillFrameTypeBtn"..nCurClanSkillSelType.."NameFont");
	SetClanLearnSkillFrameScroll(curSelFont:GetText(), "ClanLearnSkillFrame_ScrollBar", ClanSkillFilter);
	if slider:IsShown() then
		BondClanLearnSkillFrameSkilllInfo(curSelFont:GetText(), slider:GetValue() + 1, "ClanLearnSkillFrameSkillInfo", ClanSkillFilter);
	else
		BondClanLearnSkillFrameSkilllInfo(curSelFont:GetText(), 1, "ClanLearnSkillFrameSkillInfo", ClanSkillFilter);
	end
	BondClanLearnSkillFrameSkillProgress("ClanLearnSkillFrameSkillInfo");
end

------------------------------      加速        ----------------------------------

function ClanAddSpeedFrameOkBtn_OnClick()
	local nSpeedMin = ClanAddSpeedFrame:GetClientUserData( 1 );
	if nSpeedMin > 0 then
		local nType = ClanAddSpeedFrame:GetClientUserData( 0 );
		local clanMoney = tonumber( ClanAddSpeedFrameCurMoney:GetText() );
		local costMoney = tonumber( ClanAddSpeedFrameNeedCostMoney:GetText() );
		if clanMoney < costMoney then
			ShowMidTips("当前公会资金不足");
			return;
		end
		if nType == ADD_TYPE_TECH then
			local nCat = ClanAddSpeedFrame:GetClientUserData( 2 );
			GameClanManager:clan_Opt_Skill_Speed_Up( nCat,nSpeedMin );
			ClanAddSpeedFrame:Hide();
		elseif nType == ADD_TYPE_PRODUCE then
			local nID = ClanAddSpeedFrame:GetClientUserData( 3 );
			GameClanManager:clan_Opt_Product_Speed_Up(nID,nSpeedMin);
			ClanAddSpeedFrame:Hide();
		end
	else
		ShowMidTips("请输入正确时间");
	end
end

function ClanAddSpeedFrame_OnShow()
	ClanAddSpeedFrameCheckBtn1_OnClick();
	ClanAddSpeedFrameTimeEdit:SetText(0);
	this:SetPoint( "center","UIClient","center",0,0 );
end

function ClanAddSpeedFrameCheckBtn1_OnClick()
	local check = getglobal( "ClanAddSpeedFrameCheckBtn1" );
	check:setCheckState( true );
	nCheckId = check:GetClientID();
	check = getglobal("ClanAddSpeedFrameCheckBtn2");
	check:setCheckState( false );
	ClanAddSpeedFrame_OnUpdate();
end

function ClanAddSpeedFrameCheckBtn2_OnClick()
	local check = getglobal( "ClanAddSpeedFrameCheckBtn2" );
	check:setCheckState( true );
	nCheckId = check:GetClientID();
	check = getglobal("ClanAddSpeedFrameCheckBtn1");
	check:setCheckState( false );
	ClanAddSpeedFrame_OnUpdate();
end

function ClanAddSpeedFrame_OnUpdate()
	local clanAdv = GameClanManager:getAdvanceInfo();
	local nType =ClanAddSpeedFrame:GetClientUserData( 0 );
	local clanView = GameClanManager:getClanViewer();
	ClanAddSpeedFrameCurMoney:SetText( clanView.m_clanInfo.m_nMoney );
	if nType == ADD_TYPE_TECH then
		local nCat = ClanAddSpeedFrame:GetClientUserData( 2 );
		local progressInfo = GameClanManager:getTechProgress( nCat );
		if progressInfo == nil then
			ClanAddSpeedFrame:Hide();
			return;
		end
		local nLeft = math.ceil( (progressInfo.EndTime - GameClanManager:getServerTime())/60);
		if nLeft <= 0 then
			ClanAddSpeedFrame:Hide();
		end
		ClanAddSpeedFrameLeftTime:SetText( nLeft.."分钟" );
		local nSpeedMin = 0;
		if nCheckId == 1 then
			nSpeedMin = nLeft;
		elseif nCheckId == 2 then
			CheckEditNumberInput(ClanAddSpeedFrameTimeEdit);
			local szText = ClanAddSpeedFrameTimeEdit:GetText();
			if szText == "" then
				szText = 0;
				ClanAddSpeedFrameTimeEdit:SetText( 0 );
			end
			nSpeedMin = tonumber(szText);			
			if nSpeedMin > nLeft then
				nLeft = nLeft;
				ClanAddSpeedFrameTimeEdit:SetText( nLeft );
			end
		end
		ClanAddSpeedFrame:SetClientUserData( 1,nSpeedMin );
		local nNeedMoney = 2000*progressInfo.Value^2*nSpeedMin;
		ClanAddSpeedFrameNeedCostMoney:SetText( nNeedMoney );
		ClanAddSpeedFrameSpeedLeftTime:SetText( nLeft - nSpeedMin );
	elseif nType == ADD_TYPE_PRODUCE then
		local productID = ClanAddSpeedFrame:GetClientUserData( 3 );
		local progressInfo = GameClanManager:getProductProgress(productID);
		if progressInfo ~= nil then
			local nLeft = math.ceil( (progressInfo.EndTime - GameClanManager:getServerTime())/60);
			if nLeft <= 0 then
				ClanAddSpeedFrame:Hide();
			end
			ClanAddSpeedFrame:SetClientUserData( 1,nLeft );
			ClanAddSpeedFrameLeftTime:SetText( nLeft.."分钟" );
			local nSpeedMin = 0;
			if nCheckId == 1 then
				nSpeedMin = nLeft;
			elseif nCheckId == 2 then
				CheckEditNumberInput(ClanAddSpeedFrameTimeEdit);
				local szText = ClanAddSpeedFrameTimeEdit:GetText();
				if szText == nil or szText == "" then
					nSpeedMin = 0;
				else
					nSpeedMin = tonumber(szText);
				end
				
				if nSpeedMin > nLeft then
					nLeft = nLeft;
					ClanAddSpeedFrameTimeEdit:SetText( nLeft );
				end
			end
			local clanProductDef = GameClanManager:getProductDefByID( progressInfo.ID );
			if clanProductDef ~= nil then
				local nNeedMoney = 2000*clanProductDef.TechRequired[0].Level^2*nSpeedMin;
				ClanAddSpeedFrameNeedCostMoney:SetText(nNeedMoney);
			end
			ClanAddSpeedFrame:SetClientUserData( 1,nSpeedMin );
			nLeft = nLeft - nSpeedMin;
			if nLeft < 0 then
				nLeft = 0;
			end
			ClanAddSpeedFrameSpeedLeftTime:SetText( nLeft );
		end
	end
end

function ClanSkillInfoTexBtn_OnEnter()
	local parent = getglobal( this:GetParent() );
	local nCat = parent:GetClientUserData( 0 );
	local nLevel = parent:GetClientUserData( 1 );
	ClanSkillTipFrame:SetClientUserData( 0,nCat );
	ClanSkillTipFrame:SetClientUserData( 1,nLevel );
	ClanSkillTipFrame:SetClientString( this:GetName() );
	ClanSkillTipFrame:Show();
end

function ClanSkillInfoTexBtn_OnLeave()
	ClanSkillTipFrame:Hide();
end

function ClanSkillTipFrame_OnShow()
	local nCat = this:GetClientUserData( 0 );
	local nLevel = this:GetClientUserData( 1 );
	local clanSkillDef = GameClanManager:getClanTechDef(nCat, nLevel);
	local nFrameHeight = 70/GetScreenScaleY() ;
	if clanSkillDef ~= nil then
		local IconPath = GetItemEquipIconPath()..clanSkillDef.IconID..".tga";
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH;
		end
		ClanSkillTipFrameTexture:SetTexture( IconPath );
		ClanSkillTipFrameName:SetText( clanSkillDef.Name );
		ClanSkillTipFrameRichText:SetText( clanSkillDef.DescText,221,196,159 );
		--nFrameHeight = nFrameHeight + ClanSkillTipFrameRichText:GetTotalHeight()/GetScreenScaleY() ;		
		if true then
			ClanSkillTipFrameITItemLineTex:Show();
			ClanSkillTipFrameITItemLineTex:SetPoint( "topleft", "ClanSkillTipFrame", "topleft", 0, nFrameHeight );
			nFrameHeight = nFrameHeight + 2;
			ClanSkillTipFrameNextLvichText:SetPoint( "topleft", "ClanSkillTipFrame", "topleft", 18, nFrameHeight );
			local szText = LearnSkillConditionJudge( clanSkillDef );
			local needTech = clanSkillDef.NeedTech;
			local needTechLv = clanSkillDef.NeedTechLevel;
			local needTechs = {};
			if nLevel > 1 then
				table.insert(needTechs, {nCat, nLevel - 1});
			end
			if needTech ~= 0 and needTechLv ~= 0 then
				table.insert(needTechs, {needTech, needTechLv});
			end
			local needTechNames = {}
			local tipFlag = true;
			for _, tech in ipairs(needTechs) do
				local needTechDef = GameClanManager:getClanTechDef(tech[1], tech[2]);
				table.insert(needTechNames, needTechDef.Name);
				if tipFlag then 
					szText = string.format("%s\n\n#cfffc59需要前置科技：#n",szText )
					tipFlag = false;
				end
				if IsClanSkillLearned(tech[1],tech[2]) then
					szText = string.format("%s\n　#cfffc59%s#n", szText, needTechDef.Name );
				else
					szText = string.format("%s\n　#cff0000%s#n", szText, needTechDef.Name );
				end
			end
			ClanSkillTipFrameNextLvichText:SetText( szText,255,255,190 );
			nFrameHeight = 6 + nFrameHeight + ClanSkillTipFrameNextLvichText:GetTotalHeight()/GetScreenScaleY();
		else
			ClanSkillTipFrameITItemLineTex:Hide();
			ClanSkillTipFrameNextLvichText:SetText( "",0,0,0 );
		end
	end	
	ClanSkillTipFrame:SetHeight( nFrameHeight );
	local szBtnName = ClanSkillTipFrame:GetClientString();
	if szBtnName ~= nil then
		local nWidth    = ClanSkillTipFrame:GetWidth();
		local nHeight   = ClanSkillTipFrame:GetHeight();
		local curSelectBtn = getglobal(szBtnName);
		local nRealTop  = curSelectBtn:GetRealTop();
		local nRight    = curSelectBtn:GetRight();

		if ( nRealTop - nHeight ) > 0 then
			if ( nRight + nWidth < GetScreenWidth() ) then
				ClanSkillTipFrame:SetPoint("BottomLeft",szBtnName,"TopRight",0,-10);
			else
				ClanSkillTipFrame:SetPoint("BottomRight",szBtnName,"TopLeft",0,-10);
			end
		else
			if ( nRight + nWidth < GetScreenWidth() ) then
				ClanSkillTipFrame:SetPoint("TopLeft",szBtnName,"BottomRight",0,10);
			else
				ClanSkillTipFrame:SetPoint("TopRight",szBtnName,"BottomLeft",0,10);
			end
		end
	end
end

function ClanSkillTipFrame_OnLoad()
	ClanSkillTipFrameTexture:setMask("uires\\ui\\mask\\GongHuiYuanXingTuBiaoBiaoShi  mask.tga");
end

function ClanLearnSkillFrameTotalSkill_OnTips()
	SetGameTooltips(this:GetName(), "工厂制造可获得科技点")
end
function LearnSkillConditionJudge( clanSkillDef)
	local szText		= string.format("#cfffc59学习条件：#n\n");
	local ClanViewer 	= GameClanManager:getClanViewer();
	local clanAdv 		= GameClanManager:getAdvanceInfo();
	local cityInfo 		= GameClanManager:getClanCity():getCityInfo()
	local skillPoint	= ClanViewer.m_clanInfo.m_nSkillPoint ;
	local clanMoney		= ClanViewer.m_clanInfo.m_nMoney;
	local clanLevel		= cityInfo.m_buildings[0].Level;
	if	clanSkillDef.NeedTechPointCost > skillPoint	then
		szText = string.format("%s#cff0000   需要公会科技点: %d#n\n", szText,clanSkillDef.NeedTechPointCost);
	else
		szText = string.format("%s#cfffc59   需要公会科技点: %d#n\n",szText, clanSkillDef.NeedTechPointCost);
	end
	if	clanSkillDef.NeedMoneyCost > clanMoney	then
		szText = string.format("%s#cff0000   需要公会资金: %d#n\n",szText, clanSkillDef.NeedMoneyCost);
	else
		szText = string.format("%s#cfffc59   需要公会资金: %d#n\n",szText, clanSkillDef.NeedMoneyCost);
	end
	szText = string.format("%s#cfffc59   学习时间: %d 分钟#n\n",szText, clanSkillDef.TimeCost/60 );
	if	clanSkillDef.Required > clanLevel	then
		szText = string.format("%s#cff0000   需要科技中心等级: %d#n",szText, clanSkillDef.Required );
	else
		szText = string.format("%s#cfffc59   需要科技中心等级: %d#n",szText, clanSkillDef.Required );
	end
	return szText;
end
--          新手提示          --

local bAdditionalHaveFindLearnableTech = false

-- 查找可学习的科技并且显示帮助
function AdditionalFindLearnableTechAndShowHelp()
	if not GameWizard:isDo(GAMEWIZARD_FIRST_BLINK_HAVE_CLAN_LEARN_TECH) then
		if not bAdditionalHaveFindLearnableTech then
			for i = 1, 4 do
				local btn = getglobal("ClanLearnSkillFrameSkillInfo" .. i)
				local id, level
				id    = btn:GetClientUserData(0)
				level = btn:GetClientUserData(1)

				if CanLearnClanSkill(id, level) then
					btn = getglobal("ClanLearnSkillFrameSkillInfo" .. i .. "LearnBtn")
					local animationTexture = getglobal(btn:GetName().."UVAnimationTex")
					animationTexture:SetUVAnimation(80, true)
					animationTexture:Show()
					btn = getglobal(btn:GetName() .. "Help")
					btn:Show()
					local rich = getglobal(btn:GetName() .. "Rich")
					rich:SetClientString("点击学习科技")
					ReSizeTooltip(btn:GetName() .. "Rich", btn:GetName(), 14)
					bAdditionalHaveFindLearnableTech = true
					break
				end
			end
		else
			for i = 1, 4 do
				btn = getglobal("ClanLearnSkillFrameSkillInfo" .. i .. "LearnBtn")
				local animationTexture = getglobal(btn:GetName().."UVAnimationTex")
				animationTexture:Hide()
				btn = getglobal("ClanLearnSkillFrameSkillInfo" .. i .. "LearnBtnHelp")
				btn:Hide()
			end

			bAdditionalHaveFindLearnableTech = false
			AdditionalFindLearnableTechAndShowHelp()
		end
	else
		for i = 1, 4 do
				btn = getglobal("ClanLearnSkillFrameSkillInfo" .. i .. "LearnBtn")
				local animationTexture = getglobal(btn:GetName().."UVAnimationTex")
				animationTexture:Hide()
				btn = getglobal("ClanLearnSkillFrameSkillInfo" .. i .. "LearnBtnHelp")
				btn:Hide()
			end
	end
end

function ClanLearnSkillFrameRichText_OnClick()
	ChanButtonOnClick();
	ClanFrameProduct_OnTab();
end
function ExistNeedTech( techId, techLv)
	local needTech   = GameClanManager:getClanTechDef(techId, techLv).NeedTech;
	local needTechLv = GameClanManager:getClanTechDef(techId, techLv).NeedTechLevel;
	local needTechs = {};
	if techLv > 1 then
		table.insert(needTechs, {techId, techLv - 1});
	end
	if needTech ~= 0 and needTechLv ~= 0 then
		table.insert(needTechs, {needTech, needTechLv});
	end
	return #needTechs;
end