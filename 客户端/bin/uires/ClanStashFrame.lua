local currentPage = 0
local lastUpdateTime = 0
local modelViewAngle = 0
local selectedIndex
local selectedId
local lockState = {
	[CLAN_GRID_LOCKED_TOLUA] = {
		next = CLAN_GRID_LIMITED_TOLUA,
		text = "锁定状态",
		suffix = "LockCloseBtn",
	},
	[CLAN_GRID_OPEN_TOLUA] = {
		next = CLAN_GRID_LOCKED_TOLUA,
		text = "开放状态",
		suffix = "LockOpenBtn",
	},
	[CLAN_GRID_LIMITED_TOLUA] = {
		next = CLAN_GRID_OPEN_TOLUA,
		text = "限制状态",
		suffix = "LockLimitBtn",
	},
}
local resourceList = {
	5102001,
	5102002,
	5102003,
	5102004,
	5102005,
	5101001,
	5101002,
	5101003,
	5101004,
	5101005,
}

local resourceNum = {}
for _, id in ipairs(resourceList) do
	local t = {}
	resourceNum[id] = t
	t.name = GameClanManager:getProductDefByID(id).Name
end

local function UpdateResourceNum()
	for _, id in ipairs(resourceList) do
		resourceNum[id].amount = GameClanManager:getStorageResourceNum(id)
	end
end

local function UpdateResourceLimit()
	local ClanViewer		= GameClanManager:getClanViewer()
	local ClanInfo			= ClanViewer.m_clanInfo
	local levelUpClanDef	= getClanLevelDef(ClanInfo.m_nClanLevel)
	if not levelUpClanDef then
		return
	end
	local ResourceLimits = levelUpClanDef.ResourceLimit
	for i = 0, MAX_CLAN_RESOURCE_TYPE - 1 do
		local limit = ResourceLimits[i]
		if limit.ResourceID ~= 0 and resourceNum[limit.ResourceID] then
			resourceNum[limit.ResourceID].limit = limit.Limit
		end
	end
end

function GetClanResource()
	local i = 1
	return function()
		if not resourceList[i] then
			return
		end
		local id = resourceList[i]
		i = i + 1
		local t = resourceNum[id]
		return t.name, t.amount or 0, t.limit or 0
	end
end

function GetMainPlayerClanContribute()
	local player = ActorMgr:getMainPlayer()
	if not player then
		return 0
	end
	local member = GameClanManager:getMemberInfo(player:getName())
	if not member then
		return 0
	end
	return member.Contribute
end

local itemList = {}

local function GetClanStashItemPages()
	local num = #itemList
	if num == 0 then
		return 0
	end
	return math.floor((num - 1) / 5)
end

local function RefreshClanStashFrame()
	local player = ActorMgr:getMainPlayer()
	local clanPos = CLAN_POS_NOT_MEMBER
	if player then
		clanPos = player:getClanPosition()
	end
	local ui
	do
		local n = 1
		for name, amount, limit in GetClanResource() do
			ui = getglobal("ClanStashFrameResourceName"..n)
			ui:SetText(name)
			ui = getglobal("ClanStashFrameResourceNum"..n)
			if limit == 0 then
				ui:SetText(string.format("： %d/∞", amount))
			else
				ui:SetText(string.format("： %d/%d", amount, limit))
			end
			n = n + 1
		end
	end
	for i = 1, 5 do
		local n = currentPage * 5 + i
		-- 取消选择
		ui = getglobal(string.format("ClanStashFrameList%dSelectedTex", i))
		ui:Hide()
		if n <= #itemList then
			if itemList[n] == selectedIndex then
				ui = getglobal(string.format("ClanStashFrameList%dSelectedTex", i))
				ui:Show()
			end
			local item = GameClanManager:getStorageItemInfo(itemList[n])
			local product = GameClanManager:getProductDefByID(item.ResourceID)
			ui = getglobal(string.format("ClanStashFrameList%dName", i))
			if not ui:IsShown() then
				ui:Show()
			end
			ui:SetText(product.Name)
			ui = getglobal(string.format("ClanStashFrameList%dMoneyIcon", i))
			if not ui:IsShown() then
				ui:Show()
			end
			ui = getglobal(string.format("ClanStashFrameList%dNum", i))
			if not ui:IsShown() then
				ui:Show()
			end
			ui:SetText(product.ContributPrice)
			
			ui = getglobal(string.format("ClanStashFrameList%dBuy", i))
			if item.State == CLAN_GRID_LOCKED_TOLUA then
				if clanPos == CLAN_POS_PRESIDENT then
					ui:Enable()
				else
					ui:Disable()
				end
			elseif item.State == CLAN_GRID_LIMITED_TOLUA then
				if clanPos ~= CLAN_POS_NORMAL and clanPos ~= CLAN_POS_NOT_MEMBER then
					ui:Enable()
				else
					ui:Disable()
				end
			else
				ui:Enable()
			end
			
			ui = getglobal(string.format("ClanStashFrameList%dItem", i))
			ui:Enable()
			ui:SetClientUserData(0, item.ResourceID)
			local IconPath = DEFAULT_ITEM_PATH
			local ItemDef = getItemDef(item.ResourceID)
			if ItemDef then
				IconPath = GetItemEquipIconPath()..ItemDef.IconID..".tga"
			end
			-- 如果没找到物品的图标则使用默认图标显示
			if not IsInExistence(IconPath) then
				IconPath = DEFAULT_ITEM_PATH
			end
			ui = getglobal(string.format("ClanStashFrameList%dItemIcon", i))
			if not ui:IsShown() then
				ui:Show()
			end
			ui:SetTexture(IconPath)
			ui:SetTexRelUV(1, 1)
			ui = getglobal(string.format("ClanStashFrameList%dItemCount", i))
			if not ui:IsShown() then
				ui:Show()
			end
			ui:SetText(item.Num)
			SetEquipImportantEffectUV(ItemDef, string.format("ClanStashFrameList%dItemUVAnimationTex", i))
		else
			ui = getglobal(string.format("ClanStashFrameList%dName", i))
			ui:Hide()
			ui = getglobal(string.format("ClanStashFrameList%dMoneyIcon", i))
			ui:Hide()
			ui = getglobal(string.format("ClanStashFrameList%dNum", i))
			ui:Hide()
			ui = getglobal(string.format("ClanStashFrameList%dBuy", i))
			ui:Disable()
			
			ui = getglobal(string.format("ClanStashFrameList%dItem", i))
			ui:Disable()
			ui:SetClientUserData(0, 0)
			ui = getglobal(string.format("ClanStashFrameList%dItemIcon", i))
			ui:Hide()
			ui = getglobal(string.format("ClanStashFrameList%dItemCount", i))
			ui:Hide()
			ui = getglobal(string.format("ClanStashFrameList%dItemUVAnimationTex", i))
			ui:Hide()
		end
	end
	local pages = GetClanStashItemPages()
	if currentPage == 0 then
		ClanStashFramePagePrev:Disable()
		ClanStashFramePageFirst:Disable()
	else
		ClanStashFramePagePrev:Enable()
		ClanStashFramePageFirst:Enable()
	end
	if currentPage == pages then
		ClanStashFramePageNext:Disable()
		ClanStashFramePageLast:Disable()
	else
		ClanStashFramePageNext:Enable()
		ClanStashFramePageLast:Enable()
	end
	ClanStashFramePageText:SetText(string.format("第%d/%d页", currentPage + 1, pages + 1))
end

local function ShowClanStashFrameSelectItem()
	for i = 1, 5 do
		local ui = getglobal(string.format("ClanStashFrameList%dSelectedTex", i))
		if selectedIndex and itemList[currentPage * 5 + i] == selectedIndex then
			if not ui:IsShown() then
				ui:Show()
			end
		else
			ui:Hide()
		end
	end
	if selectedIndex then
		local item = GameClanManager:getStorageItemInfo(selectedIndex)
		local product = GameClanManager:getProductDefByID(item.ResourceID)
		if not ClanStashFrameModelName:IsShown() then
			ClanStashFrameModelName:Show()
		end
		ClanStashFrameModelName:SetText(product.Name)
		if not ClanStashFrameDesc:IsShown() then
			ClanStashFrameDesc:Show()
		end
		ClanStashFrameDesc:SetText(product.RecDescText, 255, 255, 190)
		
		local path = GetModePathById(item.ResourceID)
		local info = GetNpcDisplayInfo(item.ResourceID)
		local scale = info.size
		local height = info.height
		if scale == 0 then
			scale = 1
		end
		if not ClanStashFrameModelView:IsShown() then
			ClanStashFrameModelView:Show()
		end
		ClanStashFrameModelView:setPetModel(0, 10100, path)
		ClanStashFrameModelView:setPetScale(0, 0.3 * scale, 0.3 * scale, 0.3 * scale)
		ClanStashFrameModelView:setEntityPosition(0, 0, 0.3 * (height / 2 + 100), 0)
		ClanStashFrameModelView:setEntityRotate(0, modelViewAngle)
		lastUpdateTime = os.clock()
		
		local lockbtn
		local highlight
		for s, t in pairs(lockState) do
			lockbtn = getglobal("ClanStashFrame"..t.suffix)
			if lockbtn:IsHighlight() then
				highlight = true
			end
			lockbtn:Hide()
		end
		for s, t in pairs(lockState) do
			lockbtn = getglobal("ClanStashFrame"..t.suffix)
			if s == item.State then
				lockbtn:Show()
				if highlight then
					SetGameTooltips(lockbtn:GetName(), string.format("当前：%s", t.text))
				end
			end
		end
	else
		ClanStashFrameModelName:Hide()
		ClanStashFrameDesc:Hide()
		ClanStashFrameModelView:setPetModel(0, 0, "")
		ClanStashFrameModelView:Hide()
		
		local lockbtn
		for _, t in pairs(lockState) do
			lockbtn = getglobal("ClanStashFrame"..t.suffix)
			lockbtn:Hide()
		end
	end
end

local function UpdateClanStashFrameData()
	itemList = {}
	selectedIndex = nil
	local orders = {}
	for i = 0, GameClanManager:getStorageItemNum() - 1 do
		local item = GameClanManager:getStorageItemInfo(i)
		local id = item.ResourceID
		if id ~= 0 and item.Num ~= 0 then
			table.insert(itemList, i)
			orders[i] = item.Pos
			if id == selectedId then
				selectedIndex = i
			end
		end
	end
	table.sort(itemList, function(a, b)
		return orders[a] < orders[b]
	end)
	if ClanStashFrame:IsShown() then
		RefreshClanStashFrame()
		ShowClanStashFrameSelectItem()
	end
end

local function SetClanStashFramePage(i)
	currentPage = i
	RefreshClanStashFrame()
end

local function SelectClanStashFrameItem(i)
	local oldIndex = selectedIndex
	if i then
		selectedIndex = itemList[currentPage * 5 + i]
		if selectedIndex then
			selectedId = GameClanManager:getStorageItemInfo(selectedIndex).ResourceID
		else
			selectedId = nil
		end
	else
		selectedIndex = nil
		selectedId = nil
	end
	if oldIndex ~= selectedIndex then
		if ClanStashExchangeFrame:IsShown() then
			ClanStashExchangeFrame:Hide()
		end
		ShowClanStashFrameSelectItem()
	end
end

local function UpdateMainPlayerClanContribute()
	local num = GetMainPlayerClanContribute()
	ClanStashFrameMoney:SetText(num)
end

function ClanStashFrame_OnLoad()
	this:RegisterEvent("GE_CLAN_STORAGE_UPDATE")
	this:RegisterEvent("GE_CLAN_SVR_INFO")
	this:RegisterEvent("GE_CLAN_CONTRIBUTE_CHANGE")
	this:setUpdateTime(0)
	local btn
	for i = 1, 5 do
		btn = getglobal(string.format("ClanStashFrameList%dItem", i))
		btn:SetClientID(i)
		btn = getglobal(string.format("ClanStashFrameList%dBuy", i))
		btn:SetClientID(i)
	end
end

function ClanStashFrame_OnEvent()
	if arg1 == "GE_CLAN_STORAGE_UPDATE" then
		UpdateResourceNum()
		UpdateClanStashFrameData()
	elseif arg1 == "GE_CLAN_CONTRIBUTE_CHANGE" then
		if this:IsShown() then
			UpdateMainPlayerClanContribute()
		end
	elseif arg1 == "GE_CLAN_SVR_INFO" then
		UpdateResourceLimit()
		if ClanStashFrame:IsShown() then
			RefreshClanStashFrame()
		end
	end
end

function ClanStashFrame_OnShow()
	util.ShowInCenter(this)
	UpdateMainPlayerClanContribute()
	SetClanStashFramePage(0)
	selectedIndex = -1
	SelectClanStashFrameItem()
end

function ClanStashFrame_OnHide()
	if ClanStashExchangeFrame:IsShown() then
		ClanStashExchangeFrame:Hide()
	end
end

function ClanStashFrame_OnUpdate()
	if ClanStashFrameModelView:IsShown() then
		local now = os.clock()
		local diff = now - lastUpdateTime
		lastUpdateTime = now
		if diff < 0 then
			diff = 0
		end
		modelViewAngle = (modelViewAngle + diff * 90) % 360
		ClanStashFrameModelView:setEntityRotate(0, modelViewAngle)
	end
end

function ClanStashFrameFirst_OnClick()
	if currentPage ~= 0 then
		SetClanStashFramePage(0)
	end
end

function ClanStashFrameLast_OnClick()
	local pages = GetClanStashItemPages()
	if currentPage ~= pages then
		SetClanStashFramePage(pages)
	end
end

function ClanStashFramePrev_OnClick()
	if currentPage > 0 then
		SetClanStashFramePage(currentPage - 1)
	end
end

function ClanStashFrameNext_OnClick()
	if currentPage < GetClanStashItemPages() then
		SetClanStashFramePage(currentPage + 1)
	end
end

function ClanStashFrameLockBtn_OnClick()
	if selectedIndex then
		local item = GameClanManager:getStorageItemInfo(selectedIndex)
		if item and item.ResourceID ~= 0 then
			GameClanManager:lockSwitchStorage(selectedIndex, lockState[item.State].next)
		end
	end
end

function ClanStashFrameLockBtn_OnEnter()
	if selectedIndex then
		local item = GameClanManager:getStorageItemInfo(selectedIndex)
		if item and item.ResourceID ~= 0 then
			SetGameTooltips(this:GetName(), string.format("当前：%s", lockState[item.State].text))
		end
	end
end

function ClanStashFrameOnMoneyTips()
	local btn = getglobal( this:GetParent() .. "MoneyIcon" );
	if btn:IsShown() then
		SetGameTooltips(this:GetName(), "公会贡献度")
	end
end

function ClanStashFrameItemButton_OnEnter()	
	local id = this:GetClientUserData(0)
	if id ~= 0 then
		ShowItemTips(LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, getItemDef(id));
	end
end

function ClanStashFrameOnItemClick()
	SelectClanStashFrameItem(this:GetClientID())
end

function ClanStashFrameUseBtn_OnClick()
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "公会仓库贡献度消费" then
		MessageBoxFrame:Hide();
	end

	local id = this:GetClientID()
	SelectClanStashFrameItem(id)
	ClanStashExchangeFrame:Show()
	local result = block(ClanStashExchangeFrame)
	if result then
		GameClanManager:exchangeStorageItem(selectedIndex, result)
	end
end

local function UpdateMustClanStashExchangeMoney(must)
	if must then
		if not ClanStashExchangeFrameMust:IsShown() then
			ClanStashExchangeFrameMust:Show()
		end
		ClanStashExchangeFrameMust:SetText(must)
		local mustWidth = ClanStashExchangeFrameMust:GetTextExtentWidth(must)
		local mustMaxWidth = ClanStashExchangeFrameMust:GetWidth()
		if mustWidth > mustMaxWidth then
			mustWidth = mustMaxWidth
		end
		if not ClanStashExchangeFrameMustMoneyIcon:IsShown() then
			ClanStashExchangeFrameMustMoneyIcon:Show()
		end
		ClanStashExchangeFrameMustMoneyIcon:SetPoint("topleft", "ClanStashExchangeFrame", "topleft", mustWidth + 3 + 113, 67)
		local max = GetMainPlayerClanContribute()
		if max and max > 0 and must <= max then
			ClanStashExchangeFrameOKBtn:Enable()
		else
			ClanStashExchangeFrameOKBtn:Disable()
		end
	else
		ClanStashExchangeFrameMust:Hide()
		ClanStashExchangeFrameMustMoneyIcon:Hide()
		ClanStashExchangeFrameOKBtn:Disable()
	end
end

local function UpdateMaxClanStashExchangeMoney()
	local max = GetMainPlayerClanContribute()
	ClanStashExchangeFrameMax:SetText(max)
	local maxWidth = ClanStashExchangeFrameMax:GetTextExtentWidth(max)
	local maxMaxWidth = ClanStashExchangeFrameMax:GetWidth()
	if maxWidth > maxMaxWidth then
		maxWidth = maxMaxWidth
	end
	ClanStashExchangeFrameMaxMoneyIcon:SetPoint("topleft", "ClanStashExchangeFrame", "topleft", maxWidth + 3 + 229, 67)
end

local function ChangeClanStashExchangeNum()
	local num = tonumber(ClanStashExchangeFrameNum:GetText())
	if num then
		local product = GameClanManager:getProductDefByID(selectedId)
		local cost = product.ContributPrice
		local max = GameClanManager:getStorageItemInfo(selectedIndex).Num
		if num > max then
			num = max
			ClanStashExchangeFrameNum:SetText(num)
		end
		UpdateMustClanStashExchangeMoney(num * cost)
	else
		UpdateMustClanStashExchangeMoney()
	end
end

function ClanStashExchangeFrame_OnLoad()
	this:RegisterEvent("GE_CLAN_CONTRIBUTE_CHANGE")
end

function ClanStashExchangeFrame_OnEvent()
	if arg1 == "GE_CLAN_CONTRIBUTE_CHANGE" then
		if this:IsShown() then
			UpdateMaxClanStashExchangeMoney()
		end
	end
end

function ClanStashExchangeFrame_OnShow()
	util.ShowInCenter(this)
	SetFocusFrame(ClanStashExchangeFrameNum)
	ClanStashExchangeFrameNum:SetText(1)
	ChangeClanStashExchangeNum()
	UpdateMaxClanStashExchangeMoney()
end

function ClanStashExchangeFrame_OnHide()
	unblock(ClanStashExchangeFrame)
end

function ClanStashExchangeFrame_OnEnter()
	local product = GameClanManager:getProductDefByID(selectedId)
	local cost = product.ContributPrice
	local num = tonumber(ClanStashExchangeFrameNum:GetText());
	if num * cost >= 100 then
		MessageBox("","花费#G".. num * cost .."#n贡献度购买#G" ..product.Name .."#B" .. num .. "#n个，是否购买？");
		MessageBoxFrame:SetClientString("公会仓库贡献度消费");
		MessageBoxFrame:SetClientUserData( 0, num );
		return;
	end

	result(ClanStashExchangeFrame, tonumber(ClanStashExchangeFrameNum:GetText()))
	util.closeParent(this)
end

function ClanStashExchangeFrame_OnInc()
	local num = tonumber(ClanStashExchangeFrameNum:GetText()) or 0
	ClanStashExchangeFrameNum:SetText(math.min(num + 1, 9999))
end

function ClanStashExchangeFrame_OnDes()
	local num = tonumber(ClanStashExchangeFrameNum:GetText()) or 0
	ClanStashExchangeFrameNum:SetText(math.max(num - 1, 1))
end

function ClanStashExchangeFrame_OnChange()
	ChangeClanStashExchangeNum()
end

function OpenClanStorageFrame(actorId)
	ClanStashFrame:Show()
	util.testNpcDialogDistance(actorId, ClanStashFrame)
end