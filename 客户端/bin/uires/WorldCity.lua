local CityBuildInfosUI = {
	"WorldCityBuildFrameName", "WorldCityBuildFrameScrollPost", "WorldCityBuildFrameModelView",
}
--[[
local StartX = 544 / 1516
local StartY = 405 / 1516
]]
local StartX = 544 / 1516
local StartY = 405 / 1516

local ExpandButtonTime = 0.1
local ProgressTime = 1.5
local BuildInfos = {}
local ExpandButtonInfo
local Constructs = {}
local Progress = {}
local lastUpdateTime = 0
local modelViewAngle = 0

local function BuildIds()
	local id = WorldCityManager:getFirstCityBuildId()
	return function()
		if id == 0 then
			return
		end
		local old = id
		id = WorldCityManager:getNextCityBuildId(id)
		return old
	end
end

local function BuildConstructs(info, state)
	local id = WorldCityManager:getCityBuildFirstConstruct(info, state)
	return function()
		if id == 0 then
			return
		end
		local old = id
		id = WorldCityManager:getCityBuildNextConstruct(info, id, state)
		return old
	end
end

local function BuildConstructResources(id)
	local resource = 0
	return function()
		resource = WorldCityManager:GetCityBuildConstructNextResource(id, resource)
		if resource == 0 then
			return
		end
		return resource, WorldCityManager:GetCityBuildConstructResourceCost(id, resource)
	end
end

local function GetBuildConstructResourceString(id)
	local t = {}
	for id, num in BuildConstructResources(id) do
		table.insert(t, string.format("%d%s", num, GameClanManager:getProductDefByID(id).Name))
	end
	return table.concat(t, "、")
end

local function SetCityBuildButton(btn, icon)
	local prefix = "uires\\DiTuLei\\MinimapIcon\\"
	btn:SetNormalTexture(string.format("%s%d.tga", prefix, icon), "alphablend")
	btn:SetHightlightTexture(string.format("%s%d_h.tga", prefix, icon), "alphablend")
	btn:SetPushedTexture(string.format("%s%d_p.tga", prefix, icon), "alphablend")
end

local function SetWorldCityBuildInfo(info)
	if info then
		for _, name in ipairs(CityBuildInfosUI) do
			local ui = getglobal(name)
			if not ui:IsShown() then
				ui:Show()
			end
		end
		local id = WorldCityManager:getCityBuildResID(info)
		WorldCityBuildFrameName:SetText(getMonsterName(id))
		local display = GetNpcDisplayInfo(id)
		local scale = display.size
		if scale == 0 then
			scale = 1
		end
		if not WorldCityBuildFrameModelView:IsShown() then
			WorldCityBuildFrameModelView:Show()
		end
		WorldCityBuildFrameModelView:setPetModel(0, 10100, GetModePathById(id), GetSocketPathById(id))
		WorldCityBuildFrameModelView:setPetScale(0, scale, scale, scale)
		WorldCityBuildFrameModelView:setEntityPosition(0, 0, 60, 0)
		WorldCityBuildFrameModelView:setEntityRotate(0, modelViewAngle)
		lastUpdateTime = os.clock()
		local rich = WorldCityBuildFrameScrollPost
		local text = WorldCityManager:getCityProductBuildDesc(id)
		text = string.gsub(text, "$HP", info.HP)
		text = string.gsub(text, "$MP", info.Energy)
		rich:SetText(text, 255, 255, 190)
		local height = rich:GetTotalHeight() / GetScreenScaleY()
		rich:resizeRichHeight(height)
		rich:SetHeight(height)
		rich:SetText(text, 255, 255, 190)
		WorldCityBuildFrameScroll:ResetChildAnchor()
		SetScrollBar("WorldCityBuildFrameScrollBar", 0, WorldCityBuildFrameScroll:GetVerticalScrollRange())
		WorldCityBuildFrameScrollBar:SetValue(0)
	else
		for _, name in ipairs(CityBuildInfosUI) do
			local ui = getglobal(name)
			ui:Hide()
		end
		WorldCityBuildFrameScrollBar:Hide()
		WorldCityBuildFrameModelView:Hide()
	end
end

local function UpdateBuildInfos()
	local MinMapContal = MapShower:GetMinMapContal()
	for i = 1, #BuildInfos do
		local ui = getglobal("WorldCityBuildFrameEntry"..i)
		ui:Hide()
	end
	BuildInfos = {}
	for id in BuildIds() do
		local info = WorldCityManager:getCityBuildInfo(id)
		if WorldCityManager:CityBuildCanDisplay(info) then
			table.insert(BuildInfos, info)
			local Coord = MinMapContal:getPointCoordOnTex(WorldCityManager:getCityBuildPosX(id), WorldCityManager:getCityBuildPosY(id))
			local ui = getglobal("WorldCityBuildFrameEntry"..#BuildInfos)
			ui:Show()
			SetCityBuildButton(ui, WorldCityManager:getCityBuildMapIconId(id))
			ui:SetPoint("center", "WorldCityBuildFrameMap", "topleft", math.floor((Coord.first / NORMALIZE_TEX_WIDTH - StartX) * 1516 + 0.5),
				math.floor((Coord.second / NORMALIZE_TEX_HEIGHT - StartY) * 1516 + 0.5))
			
			ui = getglobal("WorldCityBuildFrameEntry"..#BuildInfos.."_")
			ui:Hide()
			ui = getglobal("WorldCityBuildFrameEntry"..#BuildInfos.."Progress")
			ui:Hide()
		end
	end
	SetWorldCityBuildInfo()
end

local function ExpandWorldCityBuildEntry(btn, state)
	ExpandButtonInfo = {}
	ExpandButtonInfo.start = os.clock()
	ExpandButtonInfo.prefix = btn:GetName().."_"
	ExpandButtonInfo.menus = {}
	local info = BuildInfos[btn:GetClientID()]
	local ui
	if state then
		ui = getglobal(ExpandButtonInfo.prefix.."Upgrade")
		if WorldCityManager:CityBuildUpgradeId(info) ~= 0 then
			table.insert(ExpandButtonInfo.menus, ui)
		else
			ui:Hide()
		end
		ui = getglobal(ExpandButtonInfo.prefix.."Repair")
		if WorldCityManager:CityBuildCanRepair(info) then
			table.insert(ExpandButtonInfo.menus, ui)
		else
			ui:Hide()
		end
		ui = getglobal(ExpandButtonInfo.prefix.."Supply")
		if WorldCityManager:CityBuildCanSupply(info) then
			table.insert(ExpandButtonInfo.menus, ui)
		else
			ui:Hide()
		end
		ui = getglobal(ExpandButtonInfo.prefix.."Remove")
		if WorldCityManager:CityBuildCanRemove(info) then
			table.insert(ExpandButtonInfo.menus, ui)
		else
			ui:Hide()
		end
	else
		local t_FuncBtn = { "Upgrade", "Repair", "Supply", "Remove" };
		for i=1, #t_FuncBtn do
			ui = getglobal(ExpandButtonInfo.prefix .. t_FuncBtn[i])
			ui:Hide()
		end
	end
	Constructs = {}
	for id in BuildConstructs(info, state) do
		table.insert(Constructs, id)
		ui = getglobal(ExpandButtonInfo.prefix.."Construct"..#Constructs)
		table.insert(ExpandButtonInfo.menus, ui)
		SetCityBuildButton(ui, WorldCityManager:getCityProductBuildIconId(id))
	end

	for i = #Constructs + 1, 8 do
		ui = getglobal(ExpandButtonInfo.prefix.."Construct"..i)
		ui:Hide()
	end
	for _, ui in ipairs(ExpandButtonInfo.menus) do
		ui:Show()
		ui:SetPoint("center", ExpandButtonInfo.prefix, "center", 0, 0)
	end
	local tex = getglobal(ExpandButtonInfo.prefix.."Frame")
	tex:SetSize(0, 0)
end

local function UpdateExpandWorldCityBuildEntry()
	if not ExpandButtonInfo then
		return
	end
	local during = os.clock() - ExpandButtonInfo.start
	if during > ExpandButtonTime or during < 0 then
		during = ExpandButtonTime
	end
	local total = #ExpandButtonInfo.menus
	local pi = math.pi
	for i, ui in ipairs(ExpandButtonInfo.menus) do
		local angle = i / total * pi * 2 - pi / 2
		ui:SetPoint("center", ExpandButtonInfo.prefix, "center", math.floor((during / ExpandButtonTime) * 37.5 * math.cos(angle) + 0.5), math.floor((during / ExpandButtonTime) * 37.5 * math.sin(angle) + 0.5))
	end
	local tex = getglobal(ExpandButtonInfo.prefix.."Frame")
	tex:SetSize((during / ExpandButtonTime) * 75, (during / ExpandButtonTime) * 75)
	if during == ExpandButtonTime then
		ExpandButtonInfo = nil
	end
end

local function DelayProgress(btn, func)
	local ui = getglobal(btn:GetName().."Progress")
	ui:SetSize(0, 7)
	ui:Show()
	Progress[ui] = {start = os.clock(), action = func}
end

local function UpdateDelayProgress()
	for ui, pro in pairs(Progress) do
		local during = os.clock() - pro.start
		if during > ProgressTime or during < 0 then
			during = ProgressTime
		end
		if not ui:IsShown() then
			Progress[ui] = nil
		elseif during == ProgressTime then
			ui:Hide()
			pro.action()
			Progress[ui] = nil
		else
			ui:SetSize(50 * during / ProgressTime, 7)
		end
	end
end


function WorldCityBuildFrame_OnLoad()
	this:RegisterEvent("GE_CITYBUILD_UPDATE")
	this:setUpdateTime(0)
end

function WorldCityBuildFrame_OnEvent()
	if arg1 == "GE_CITYBUILD_UPDATE" then
		if this:IsShown() then
			UpdateBuildInfos()
		end
	end
end

function WorldCityBuildFrame_OnShow()
	util.ShowInCenter(this)
	UpdateBuildInfos()
end

function WorldCityBuildFrame_OnUpdate()
	UpdateExpandWorldCityBuildEntry()
	UpdateDelayProgress()
	if WorldCityBuildFrameModelView:IsShown() then
		local now = os.clock()
		local diff = now - lastUpdateTime
		lastUpdateTime = now
		if diff < 0 then
			diff = 0
		end
		modelViewAngle = (modelViewAngle + diff * 90) % 360
		WorldCityBuildFrameModelView:setEntityRotate(0, modelViewAngle)
	end
end

function WorldCityBuildEntry_OnEnter()
	
end

function WorldCityBuildEntry_OnClick()
	local info = BuildInfos[this:GetClientID()]
	SetWorldCityBuildInfo(info)
	local ui = getglobal(this:GetName().."_")
	ui:Show()
	SetFocusFrame(ui)
	ui:SetPoint("center", this:GetName(), "center", 0, 0)
	ExpandWorldCityBuildEntry(this, true)
end

function WorldCityBuildEntry_OnFocusLost()
	this:Hide()
end

function WorldCityBuildEntry_OnUpdate()
	if this:GetName() ~= GetFocusFrameName() then
		this:Hide()
	end
end

function WorldCityBuildEntry_Upgrade_OnEnter()
	local parent = getglobal(this:GetParent())
	parent = getglobal(parent:GetParent())
	local info = BuildInfos[parent:GetClientID()]
	local id = WorldCityManager:CityBuildUpgradeId(info)
	ShowGameTooltip({rich = "WorldMapGameTooltipRichText", tipsFrame = "WorldMapGameTooltip", text = WorldCityManager:getCityProductBuildTips(id),
		frame = "WorldCityBuildFrame", button = this:GetName()})
end

function WorldCityBuildEntry_Upgrade_OnClick()
	local parent = getglobal(this:GetParent())
	parent = getglobal(parent:GetParent())
	local info = BuildInfos[parent:GetClientID()]
	local id = WorldCityManager:getCityBuildResID(info)
	local name = getMonsterName(id)
	local high = WorldCityManager:CityBuildUpgradeId(info)
	local money = WorldCityManager:GetCityBuildConstructCost(high)
	local cost = GetBuildConstructResourceString(high)
	if money ~= 0 then
		cost = string.format("%d公会资金、%s", money, cost)
	end
	if MessageBoxBlock("升级确认", string.format("#P升级%s将消耗%s，是否继续？", name, cost), WorldCityBuildFrame) then
		DelayProgress(parent, function()
			WorldCityManager:UpgradeCityBuild(info.DefID)
		end)
	end
end

function WorldCityBuildEntry_Repair_OnEnter()
	SetGameTooltips(this:GetName(), "修理")
end

function WorldCityBuildEntry_Repair_OnClick()
	local parent = getglobal(this:GetParent())
	parent = getglobal(parent:GetParent())
	local info = BuildInfos[parent:GetClientID()]
	if info.HP == info.MaxHP then
		return ShowMidTips("该建筑完好，无需修理")
	end
	local id = WorldCityManager:getCityBuildResID(info)
	local name = getMonsterName(id)
	local money = WorldCityManager:GetCityBuildRepairCost(id) - info.HP*100000*WorldCityManager:GetCityBuildRepairCost(id)/(info.MaxHP*100000 )
	if MessageBoxBlock("修理确认", string.format("#P修理%s将消耗%s，是否继续？", name, string.format("%d公会资金", money)), WorldCityBuildFrame) then
		DelayProgress(parent, function()
			WorldCityManager:RepairCityBuild(info.DefID)
		end)
	end
end

function WorldCityBuildEntry_Supply_OnEnter()
	SetGameTooltips(this:GetName(), "充能")
end

function WorldCityBuildEntry_Supply_OnClick()
	local parent = getglobal(this:GetParent())
	parent = getglobal(parent:GetParent())
	local info = BuildInfos[parent:GetClientID()]
	if info.Energy == info.MaxEnergy then
		return ShowMidTips("该建筑能量已满，无需充能")
	end
	local id = WorldCityManager:getCityBuildResID(info)
	local name = getMonsterName(id)
	local money = math.floor(WorldCityManager:GetCityBuildSupplyCost(id) * (1 - info.Energy / info.MaxEnergy))
	if MessageBoxBlock("充能确认", string.format("#P充能%s将消耗%s，是否继续？", name, string.format("%d公会资金", money)), WorldCityBuildFrame) then
		DelayProgress(parent, function()
			WorldCityManager:SupplyCityBuild(info.DefID)
		end)
	end
end

function WorldCityBuildEntry_Remove_OnEnter()
	SetGameTooltips(this:GetName(), "建设")
end

function WorldCityBuildEntry_Remove_OnClick()
	GameTooltip:Hide()
	local parent = getglobal(this:GetParent())
	parent = getglobal(parent:GetParent())
	local info = BuildInfos[parent:GetClientID()]
	SetWorldCityBuildInfo(info)
	local ui = getglobal(parent:GetName().."_")
	ui:Show()
	SetFocusFrame(ui)
	ui:SetPoint("center", parent:GetName(), "center", 0, 0)
	ExpandWorldCityBuildEntry(parent, false)
	--[[
	local id = WorldCityManager:getCityBuildResID(info)
	local name = getMonsterName(id)
	if MessageBoxBlock("拆除确认", string.format("#P即将拆除%s，是否继续？", name), WorldCityBuildFrame) then
		DelayProgress(parent, function()
			WorldCityManager:RemoveCityBuild(info.DefID)
		end)
	end
	--]]
end

function WorldCityBuildEntry_Construct_OnEnter()
	local id = Constructs[this:GetClientID()]
	ShowGameTooltip({rich = "WorldMapGameTooltipRichText", tipsFrame = "WorldMapGameTooltip", text = WorldCityManager:getCityProductBuildTips(id),
		frame = "WorldCityBuildFrame", button = this:GetName()})
end

function WorldCityBuildEntry_Construct_OnClick()
	local parent = getglobal(this:GetParent())
	parent = getglobal(parent:GetParent())
	local info = BuildInfos[parent:GetClientID()]
	local id = Constructs[this:GetClientID()]
	local name = getMonsterName(id)
	local money = WorldCityManager:GetCityBuildConstructCost(id)
	local cost = GetBuildConstructResourceString(id)
	if money ~= 0 then
		cost = string.format("%d公会资金、%s", money, cost)
	end
	if MessageBoxBlock("建造确认", string.format("#P建造%s将消耗%s，是否继续？", name, cost), WorldCityBuildFrame) then
		DelayProgress(parent, function()
			WorldCityManager:ConstructCityBuild(info.DefID, id)
		end)
	end
end

function WorldCityBuildFrameScrollUpBtn_OnClick()
	local silder = WorldCityBuildFrameScrollBar
	local value = silder:GetValue() - silder:GetValueStep()
	if value >= 0 then
		silder:SetValue(value)
	end
end

function WorldCityBuildFrameScrollDownBtn_OnClick()
	local silder = WorldCityBuildFrameScrollBar
	local nMaxValue = silder:GetMaxValue()
	local value = silder:GetValue() + silder:GetValueStep()
	if value <= nMaxValue then
		silder:SetValue(value)
	end
end

function WorldCityBuildFrameScrollBar_OnValueChanged()
	local silder = WorldCityBuildFrameScrollBar
	WorldCityBuildFrameScroll:IncreaseVerticalScroll(silder:GetValue() - silder:GetLastValue())
end

function OpenWorldCityBuildFrame(id)
	WorldCityBuildFrame:Show()
	util.testNpcDialogDistance(id, WorldCityBuildFrame)
end

-----------------------------------主界面------------------------------------------
local FinishCaptureTime
local CaptureRemain
local CaptureBuilds = {}
local CaptureBuildIndex = {}
local lastTick
local totalTick
local blinkPeriod = 0.5

local function SetWorldCityFightCaptureBuildOwner(i, name)
	local id = CaptureBuilds[i].id
	local nShow = CaptureBuilds[i].show
	local info = WorldCityManager:getCityBuildInfo(id)
	if not info then
		return
	end
	local icon = getMonsterIcon(WorldCityManager:getCityBuildResID(info))
	local path
	if name ~= "" and name == CaptureBuilds[i].name then
		path = string.format("uires\\DiTuLei\\MinimapIcon\\%d.tga", icon)
	else
		path = string.format("uires\\DiTuLei\\MinimapIcon\\%d_g.tga", icon)
	end
	if nShow > 0 then
		local tex
		tex = getglobal("WorldCityCaptureFrameBuild"..i)
		tex:SetTexture(path)
		tex = getglobal("WorldCityCaptureFrameBuildBlink"..i)
		tex:SetTexture(string.format("uires\\DiTuLei\\MinimapIcon\\%d_m.tga", icon))
	end
	local resid = WorldCityManager:getCityBuildResID(info)
	AddImportWordMapVirtualIcon("worldcity"..i, WorldCityMapID, path, getMonsterName(resid),
		WorldCityManager:getCityBuildPosX(id), WorldCityManager:getCityBuildPosY(id))
end

function ResetWorldCityFightCaptureBuildOwner()
	local name = GameClanManager:getClanViewer().m_clanInfo.m_szClanName
	for i in ipairs(CaptureBuilds) do
		SetWorldCityFightCaptureBuildOwner(i, name)
	end
end

function SetWorldCityFightCaptureBuild(id, name,nShow)
	local i = CaptureBuildIndex[id]
	if i then
		CaptureBuilds[i].name = name
		CaptureBuilds[i].show = nShow
	else
		table.insert(CaptureBuilds, {id = id, name = name, show = nShow})
		i = #CaptureBuilds
		CaptureBuildIndex[id] = i
	end
	local font = getglobal("WorldCityCaptureFrameOwner"..i)
	if nShow > 0 then		
		font:SetText(name)
	else
		font:SetText("")
	end
	SetWorldCityFightCaptureBuildOwner(i, GameClanManager:getClanViewer().m_clanInfo.m_szClanName)
end

function ResetWorldCityFightCaptureBuild()
	for id, idx in pairs(CaptureBuildIndex) do
		RemoveImportWordMapVirtualIcon("worldcity"..idx)
	end
	CaptureBuilds = {}
	CaptureBuildIndex = {}
end

function SetWorldCityFightCaptureTime(time, name)
	if time == 0 then
		WorldCityCaptureFrameName:SetText("")
		WorldCityCaptureFrameTime:SetText("")
		FinishCaptureTime = nil
	else
		local t = getServerTime()
		local now = t.sec + t.usec * 0.001
		FinishCaptureTime = time
		CaptureRemain = math.ceil(time - now)
		WorldCityCaptureFrameName:SetText(name)
		WorldCityCaptureFrameTime:SetText(string.format("将于%d秒后占领控制中心", CaptureRemain))
	end
end

function WorldCityCaptureFrame_OnLoad()
	this:setUpdateTime(0.03)
	this:RegisterEvent("GE_ENTER_MAP")
	this:RegisterEvent("GE_CLAN_SVR_INFO")
	this:RegisterEvent("GE_CITYBUILD_WARNING")
end

function WorldCityCaptureFrame_OnEvent()
	if arg1 == "GE_ENTER_MAP" then
		if MapShower:getRealMapId() == WorldCityMapID then
			if not WorldCityCaptureFrame:IsShown() then
				WorldCityCaptureFrame:Show()
			end
		else
			WorldCityCaptureFrame:Hide()
		end
		for id, idx in pairs(CaptureBuildIndex) do
			RemoveImportWordMapVirtualIcon("worldcity"..idx)
		end
	elseif arg1 == "GE_CLAN_SVR_INFO" then
		ResetWorldCityFightCaptureBuildOwner()
	elseif arg1 == "GE_CITYBUILD_WARNING" then
		for id, idx in pairs(CaptureBuildIndex) do
			local info = WorldCityManager:getCityBuildInfo(id)
			if info then
				CaptureBuilds[idx].time = info.WarnEndTime
			end
		end
	end
end

function WorldCityCaptureFrame_OnShow()
	lastTick = os.clock()
	totalTick = 0
	for i = 1, 3 do
		local ui = getglobal("WorldCityCaptureFrameBuildBlink"..i)
		ui:Hide()
	end
end

function WorldCityCaptureFrame_OnUpdate()
	local now = os.clock()
	local during = now - lastTick
	lastTick = now
	if during < 0 then
		during = 0
	end
	totalTick = totalTick + during
	local alpha = math.abs(math.cos((totalTick % blinkPeriod) / blinkPeriod * math.pi))
	local t = getServerTime()
	local name = GameClanManager:getClanViewer().m_clanInfo.m_szClanName
	for id, idx in pairs(CaptureBuildIndex) do
		local ui = getglobal("WorldCityCaptureFrameBuildBlink"..idx)
		local time = CaptureBuilds[idx].time
		local owner = CaptureBuilds[idx].name
		if time and CaptureBuilds[idx].show > 0 then
			if time > t.sec and name ~= "" and name == owner then
				if not ui:IsShown() then
					ui:Show()
				end
				ui:SetBlendAlpha(alpha)
			else
				CaptureBuilds[idx].time = nil
				ui:Hide()
			end
		end
	end
	if FinishCaptureTime then
		local remain = math.ceil(FinishCaptureTime - t.sec + t.usec * 0.001)
		if remain ~= CaptureRemain then
			CaptureRemain = remain
			if CaptureRemain <= 0 then
				WorldCityCaptureFrameName:SetText("")
				WorldCityCaptureFrameTime:SetText("")
				FinishCaptureTime = nil
			else
				WorldCityCaptureFrameTime:SetText(string.format("将于%d秒后占领控制中心", CaptureRemain))
			end
		end
	end
end

-----------------------------------世界地图标记------------------------------------------

function WorldCityEnterView(id)
	local actor = ActorMgr:FindActor(id)
	if not actor then
		return
	end
	local info = WorldCityManager:getCityBuildInfo(WorldCityManager:GetCityBuildDefID(id))
	if not info then
		return
	end
	local resid = actor:getResId()
	local IconID = WorldCityManager:getCityProductMapIconId(resid)
	if IconID == 0 then
		IconID = getMonsterIcon(resid)
	end
	local name = GameClanManager:getClanViewer().m_clanInfo.m_szClanName
	local path
	if name ~= "" and name == info.ClanName then
		path = string.format("uires\\DiTuLei\\MinimapIcon\\%d.tga", IconID)
	else
		path = string.format("uires\\DiTuLei\\MinimapIcon\\%d_g.tga", IconID)
		if not IsInExistence(path) then
			path = string.format("uires\\DiTuLei\\MinimapIcon\\%d.tga", IconID)
		end
	end
	if IsInExistence(path) then
		AddImportMinimapIcon(id, path)
	end
end

function WorldCityLeaveView(id)
	RemoveImportMinimapIcon(id)
end
-------------------------------时间城争霸战连杀面板---------------
local t_NumberTexUv = {
	[1] = { path = "uires\\ShuZiXiangGuanZiYuan\\new\\JiShaShu.tga",	X="0",		Y="0",		W="70",		H="80", },
	[2] = { path = "uires\\ShuZiXiangGuanZiYuan\\new\\JiShaShu.tga",	X="70",		Y="0",		W="70",		H="80", },
	[3] = { path = "uires\\ShuZiXiangGuanZiYuan\\new\\JiShaShu.tga",	X="140",	Y="0",		W="70",		H="80", },
	[4] = { path = "uires\\ShuZiXiangGuanZiYuan\\new\\JiShaShu.tga",	X="210",	Y="0",		W="70",		H="80", },
	[5] = { path = "uires\\ShuZiXiangGuanZiYuan\\new\\JiShaShu.tga",	X="280",	Y="0",		W="70",		H="80", },
	[6] = { path = "uires\\ShuZiXiangGuanZiYuan\\new\\JiShaShu.tga",	X="0",		Y="80",		W="70",		H="80", },
	[7] = { path = "uires\\ShuZiXiangGuanZiYuan\\new\\JiShaShu.tga",	X="70",		Y="80",		W="70",		H="80", },
	[8] = { path = "uires\\ShuZiXiangGuanZiYuan\\new\\JiShaShu.tga",	X="140",	Y="80",		W="70",		H="80", },
	[9] = { path = "uires\\ShuZiXiangGuanZiYuan\\new\\JiShaShu.tga",	X="210",	Y="80",		W="70",		H="80", },
	[0]	= { path = "uires\\ShuZiXiangGuanZiYuan\\new\\JiShaShu.tga",	X="280",	Y="80",		W="70",		H="80", },
	["连杀"] = { path = "uires\\ShuZiXiangGuanZiYuan\\new\\JiShaShu.tga", X="0",	Y="160",	W="260",	H="130", },
	["双杀"] = { path = "uires\\ShuZiXiangGuanZiYuan\\new\\JiShaShu.tga", X="260",	Y="160",	W="230",	H="130", },
}
local alpha_time = 0
function WorldCityWarContineKillFrame_OnLoad()
	this:setUpdateTime( 0.1 )
	WorldCityWarContineKillFrameKillNum1:SetBlendAlpha( 0 )
	WorldCityWarContineKillFrameKillNum2:SetBlendAlpha( 0 )
	WorldCityWarContineKillFrameKillNum3:SetBlendAlpha( 0 )
	WorldCityWarContineKillFrameKillTex:SetBlendAlpha( 0 )
	WorldCityWarContineKillFrameKillBack1:SetBlendAlpha(0)	
	WorldCityWarContineKillFrameKillBack2:SetBlendAlpha(0)	
end
function WorldCityWarContineKillFrame_OnHide()
	this:SetClientUserData( 0, 0 )
	WorldCityWarContineKillFrameKillNum1:SetBlendAlpha( 0 )
	WorldCityWarContineKillFrameKillNum2:SetBlendAlpha( 0 )
	WorldCityWarContineKillFrameKillNum3:SetBlendAlpha( 0 )
	WorldCityWarContineKillFrameKillTex:SetBlendAlpha( 0 )
	WorldCityWarContineKillFrameKillBack1:SetBlendAlpha(0)	
	WorldCityWarContineKillFrameKillBack2:SetBlendAlpha(0)	
end
function SetContineKillTexForWorldCityWar(ContineKill)
	if math.floor(ContineKill/100) ~= 0 then
		WorldCityWarContineKillFrameKillBack1:Show()	
		WorldCityWarContineKillFrameKillBack2:Hide()	
		local KillNumOne = math.floor(ContineKill/100)
		WorldCityWarContineKillFrameKillNum1:SetTexture( t_NumberTexUv[KillNumOne].path );
		WorldCityWarContineKillFrameKillNum1:SetTexUV( t_NumberTexUv[KillNumOne].X, t_NumberTexUv[KillNumOne].Y, t_NumberTexUv[KillNumOne].W, t_NumberTexUv[KillNumOne].H );
		local KillNumTwo = math.floor(ContineKill%100/10)
		WorldCityWarContineKillFrameKillNum2:SetTexture( t_NumberTexUv[KillNumTwo].path );
		WorldCityWarContineKillFrameKillNum2:SetTexUV( t_NumberTexUv[KillNumTwo].X, t_NumberTexUv[KillNumTwo].Y, t_NumberTexUv[KillNumTwo].W, t_NumberTexUv[KillNumTwo].H );
		local KillNumThree = ContineKill%100%10
		WorldCityWarContineKillFrameKillNum3:SetTexture( t_NumberTexUv[KillNumThree].path );
		WorldCityWarContineKillFrameKillNum3:SetTexUV( t_NumberTexUv[KillNumThree].X, t_NumberTexUv[KillNumThree].Y, t_NumberTexUv[KillNumThree].W, t_NumberTexUv[KillNumThree].H );
		WorldCityWarContineKillFrameKillTex:SetTexUV( t_NumberTexUv["连杀"].X, t_NumberTexUv["连杀"].Y, t_NumberTexUv["连杀"].W, t_NumberTexUv["连杀"].H );
	elseif math.floor(ContineKill%100/10) ~= 0 then
		WorldCityWarContineKillFrameKillBack1:Hide()	
		WorldCityWarContineKillFrameKillBack2:Show()	
		WorldCityWarContineKillFrameKillNum1:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
		local KillNumTwo = math.floor(ContineKill%100/10)
		WorldCityWarContineKillFrameKillNum2:SetTexture( t_NumberTexUv[KillNumTwo].path );
		WorldCityWarContineKillFrameKillNum2:SetTexUV( t_NumberTexUv[KillNumTwo].X, t_NumberTexUv[KillNumTwo].Y, t_NumberTexUv[KillNumTwo].W, t_NumberTexUv[KillNumTwo].H );
		local KillNumThree = ContineKill%100%10
		WorldCityWarContineKillFrameKillNum3:SetTexture( t_NumberTexUv[KillNumThree].path );
		WorldCityWarContineKillFrameKillNum3:SetTexUV( t_NumberTexUv[KillNumThree].X, t_NumberTexUv[KillNumThree].Y, t_NumberTexUv[KillNumThree].W, t_NumberTexUv[KillNumThree].H );
		WorldCityWarContineKillFrameKillTex:SetTexUV( t_NumberTexUv["连杀"].X, t_NumberTexUv["连杀"].Y, t_NumberTexUv["连杀"].W, t_NumberTexUv["连杀"].H );
	elseif ContineKill%100%10 ~= 0 then
		WorldCityWarContineKillFrameKillBack1:Hide()	
		WorldCityWarContineKillFrameKillBack2:Hide()	
		WorldCityWarContineKillFrameKillNum1:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
		WorldCityWarContineKillFrameKillNum2:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
		local KillNumThree = ContineKill%100%10
		if KillNumThree == 2 then
			WorldCityWarContineKillFrameKillNum3:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
			WorldCityWarContineKillFrameKillTex:SetTexUV( t_NumberTexUv["双杀"].X, t_NumberTexUv["双杀"].Y, t_NumberTexUv["双杀"].W, t_NumberTexUv["双杀"].H );
		else
			WorldCityWarContineKillFrameKillNum3:SetTexture( t_NumberTexUv[KillNumThree].path );
			WorldCityWarContineKillFrameKillNum3:SetTexUV( t_NumberTexUv[KillNumThree].X, t_NumberTexUv[KillNumThree].Y, t_NumberTexUv[KillNumThree].W, t_NumberTexUv[KillNumThree].H );
			WorldCityWarContineKillFrameKillTex:SetTexUV( t_NumberTexUv["连杀"].X, t_NumberTexUv["连杀"].Y, t_NumberTexUv["连杀"].W, t_NumberTexUv["连杀"].H );
		end
	end
end
function WorldCityWarContineKillFrame_OnUpdate()
	local oldContineKill = this:GetClientUserData(0)
	local newContineKill = WorldCityManager:GetWorldCitySelfContineKill()
	this:SetClientUserData(0,newContineKill);
	if newContineKill ~= oldContineKill and newContineKill >= 2 then
		--设置连杀数目图片
		SetContineKillTexForWorldCityWar( newContineKill )
		alpha_time = os.clock()
		WorldCityWarContineKillFrameKillNum1:SetBlendAlpha( 1 )
		WorldCityWarContineKillFrameKillNum2:SetBlendAlpha( 1 )
		WorldCityWarContineKillFrameKillNum3:SetBlendAlpha( 1 )
		WorldCityWarContineKillFrameKillTex:SetBlendAlpha( 1 )
		WorldCityWarContineKillFrameKillBack1:SetBlendAlpha( 1 )
		WorldCityWarContineKillFrameKillBack2:SetBlendAlpha( 1 )
		WorldCityWarContineKillFrame:SetSize( 280*1.2, 90*1.25 )
		return
	end
	if alpha_time > 0 and os.clock() - alpha_time <= 5 then
		WorldCityWarContineKillFrame:SetSize( 280, 90 )
		local alpha = 1 - 0.2*( os.clock() - alpha_time )
		WorldCityWarContineKillFrameKillNum1:SetBlendAlpha( alpha )
		WorldCityWarContineKillFrameKillNum2:SetBlendAlpha( alpha )
		WorldCityWarContineKillFrameKillNum3:SetBlendAlpha( alpha )
		WorldCityWarContineKillFrameKillTex:SetBlendAlpha( alpha )
		WorldCityWarContineKillFrameKillBack1:SetBlendAlpha( alpha )
		WorldCityWarContineKillFrameKillBack2:SetBlendAlpha( alpha )
	else
		WorldCityWarContineKillFrameKillNum1:SetBlendAlpha(0)
		WorldCityWarContineKillFrameKillNum2:SetBlendAlpha(0)
		WorldCityWarContineKillFrameKillNum3:SetBlendAlpha(0)
		WorldCityWarContineKillFrameKillTex:SetBlendAlpha(0)
		WorldCityWarContineKillFrameKillBack1:SetBlendAlpha( 0 )
		WorldCityWarContineKillFrameKillBack2:SetBlendAlpha( 0 )
	end
end
--------------------------------时间城争霸战漂移图标------------------------
local worldCityWarEnd = false;
function WorldCityWarBegin()
	local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
	if t_centerXinShouBtnControl:getIndexByType( CENTER_WORLD_CITY_WAR ) == 0 then
		t_centerXinShouBtnControl:ShowExtraBtn( { itemDefId = 0, Type = CENTER_WORLD_CITY_WAR, frame = "WorldCityWarInfoFrame", 
												path ="uires\\ui\\Za\\ShiJianChengZhenBaZhan.tga", index = 0 } );

		WorldCityWarInfoFrameWinInfoRank:SetText("")
		WorldCityWarInfoFrameWinInfoGetMoney:SetText("")
		WorldCityWarInfoFrameWinInfoGetFont:SetText("")
		WorldCityWarInfoFrameWinInfoGetExp:SetText("")
		WorldCityWarInfoFrameWinInfoGetGift:SetText("")
		
		worldCityWarEnd = false;
	end	
	WorldCityWarInfoFrameEnterTime:Show();
	WorldCityWarContineKillFrame:Show();
end
function WorldCityWarEnd()
	worldCityWarEnd = true;
	local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
	local nIdx = t_centerXinShouBtnControl:getIndexByType( CENTER_WORLD_CITY_WAR );
	if nIdx > 0 then
		t_centerXinShouBtnControl:eraseByIndex( nIdx );
		t_centerXinShouBtnControl:updateCenterBtns();
	end
	WorldCityWarContineKillFrame:Hide();
end
--------------------------------时间城争霸战排行榜--------------------------
local t_WorldCityWarInfo = {
	[1] = [[#ca0a05a满足任意条件可获得战争奖励:
#cff0000	1.进入战争场景超过25分钟
#cff0000	2.公会总积分超过100分
#ca0a05a注：以上奖励，仅据点占领成员可获得]],
	[2] = [[#ca0a05a战争奖励：战争结束时玩家自动获得
#cffffbe1.排名第1公会：每人获得2222354银币
#cffffbe2.排名第2公会：每人获得2254534银币
#cffffbe3.排名第3公会：每人获得2254534银币
#cffffbe4.排名第4公会：每人获得2254534银币
#cffffbe5.排名第5公会：每人获得2254534银币
#cffffbe6.排名第6公会：每人获得2254534银币]],
}
local WORLD_CITY_WAR_MAX_CLAN = 6
local t_WorldCityWarClanInfo = {}
local t_WorldCityWarInfoFrameRemindTexUV = {
	["finish"] = { 
	uv = { w = 24, h = 21, x = 576, y = 78,},
	},
	["unFinish"] = { 
	uv = { w = 25, h = 22, x = 604, y = 78, },
	 },	
}
function WorldCityWarInfoFrame_OnLoad()
	WorldCityWarInfoFrameRemindInfoRemind1:SetText(t_WorldCityWarInfo[1],255,255,255)
	WorldCityWarInfoFrameRemindInfoRemind2:SetText(t_WorldCityWarInfo[2],255,255,255)
	WorldCityWarInfoFrameWinInfoRank:SetText("")
	WorldCityWarInfoFrameWinInfoGetMoney:SetText("")	
	WorldCityWarInfoFrameWinInfoGetFont:SetText("")
	WorldCityWarInfoFrameWinInfoGetExp:SetText("")
	WorldCityWarInfoFrameWinInfoGetGift:SetText("")
end
function GetWorldCityClanInfoData()
	local clanNum = WorldCityManager:GetWorldCityClanNum()
	t_WorldCityWarClanInfo = {}
	for i = 1, clanNum do
		local oneClanInfo = {
			rank = i,
			clanName = WorldCityManager:GetWorldCityClanName( i - 1 ),
			reviveNum = WorldCityManager:GetWorldCityClanGetReviveNum( i - 1 ),
			controlNum = WorldCityManager:GetWorldCityClanGetControlNum( i - 1 ),
			buffNum = WorldCityManager:GetWorldCityClanBuffNum( i - 1 ),
			doorNum = WorldCityManager:GetWorldCityClanDoorNum( i - 1 ),
			defNum = WorldCityManager:GetWorldCityClanDefNum( i - 1 ),
			total = WorldCityManager:GetWorldCityClanTotal( i - 1 ),
			time = WorldCityManager:GetWorldCityClanTime( i - 1 ),
		}
		table.insert( t_WorldCityWarClanInfo, oneClanInfo )
	end
end
--添加了Time，以此作为排序的第二标准
function SortWorldCityClanInfo()
	table.sort( t_WorldCityWarClanInfo, function( a, b )
		if a["total"] == b["total"] then
			return  a["time"] < b["time"]
		else
			return a["total"] > b["total"]
		end
	end)
	--更改排名
	for i = 1, table.getn( t_WorldCityWarClanInfo ) do
		t_WorldCityWarClanInfo[i]["rank"] = i;
	end
end
function UpdateWorldCityClanInfo()
	--清除之前显示的
	for i = 1, WORLD_CITY_WAR_MAX_CLAN do
		local btn = getglobal( "WorldCityWarInfoFrameClanInfoClanInfo"..i )
		local rankFont = getglobal( btn:GetName().."Rank" )
		rankFont:SetText( "" )
		local clanFont = getglobal( btn:GetName().."Clan" )
		clanFont:SetText( "" )
		local reviveFont = getglobal( btn:GetName().."ReviveBuild" )
		reviveFont:SetText( "" )
		local controlFont = getglobal( btn:GetName().."ControlBuild" )
		controlFont:SetText( "" )
		local buffFont = getglobal( btn:GetName().."BuffBuild" )
		buffFont:SetText( "" )
		local doorFont = getglobal( btn:GetName().."DoorBuild" )
		doorFont:SetText( "" )
		local defFont = getglobal( btn:GetName().."DefBuild" )
		defFont:SetText( "" )
		local totalFont = getglobal( btn:GetName().."Total" )
		totalFont:SetText( "" )
	end
	for i = 1, table.getn( t_WorldCityWarClanInfo ) do
		local color;
		--判断此条公会信息是否为自己公会
		local mainplayer = ActorMgr:getMainPlayer()
		if mainplayer:getClanName() == t_WorldCityWarClanInfo[i]["clanName"] then
			color = { 0, 255, 0, }
		else
			color = { 190, 190, 90, }
		end
		local btn = getglobal( "WorldCityWarInfoFrameClanInfoClanInfo"..i )
		local rankFont = getglobal( btn:GetName().."Rank" )
		rankFont:SetText( t_WorldCityWarClanInfo[i]["rank"] )
		rankFont:SetTextColor( color[1], color[2], color[3] )
		local clanFont = getglobal( btn:GetName().."Clan" )
		clanFont:SetText( t_WorldCityWarClanInfo[i]["clanName"] )
		clanFont:SetTextColor( color[1], color[2], color[3] )
		local reviveFont = getglobal( btn:GetName().."ReviveBuild" )
		reviveFont:SetText( t_WorldCityWarClanInfo[i]["reviveNum"] )
		reviveFont:SetTextColor( color[1], color[2], color[3] )
		local controlFont = getglobal( btn:GetName().."ControlBuild" )
		controlFont:SetText( t_WorldCityWarClanInfo[i]["controlNum"] )
		controlFont:SetTextColor( color[1], color[2], color[3] )
		local buffFont = getglobal( btn:GetName().."BuffBuild" )
		buffFont:SetText( t_WorldCityWarClanInfo[i]["buffNum"] )
		buffFont:SetTextColor( color[1], color[2], color[3] )
		local doorFont = getglobal( btn:GetName().."DoorBuild" )
		doorFont:SetText( t_WorldCityWarClanInfo[i]["doorNum"] )
		doorFont:SetTextColor( color[1], color[2], color[3] )
		local defFont = getglobal( btn:GetName().."DefBuild" )
		defFont:SetText( t_WorldCityWarClanInfo[i]["defNum"] )
		defFont:SetTextColor( color[1], color[2], color[3] )
		local totalFont = getglobal( btn:GetName().."Total" )
		totalFont:SetText( t_WorldCityWarClanInfo[i]["total"] )
		totalFont:SetTextColor( color[1], color[2], color[3] )
	end
	--显示奖励
	local szRankAward = "#ca0a05a战争奖励：战争结束时玩家自动获得"
	for i = 1, WORLD_CITY_WAR_MAX_CLAN do
		local moneyType = GetMoneyDesc(WorldCityManager:GetWorldCityClanAwardMoneyType( i ))
		local money = WorldCityManager:GetWorldCityClanAwardMoney( i )
		local szMoney = (money==0) and "" or "、"..money..moneyType
		local awardItemDef = getItemDef(WorldCityManager:GetWorldCityClanAwardItemID( i ))
		local awardItem = ( WorldCityManager:GetWorldCityClanAwardItemID( i ) == 0 ) and "" or "、\n"..awardItemDef.Name
		szRankAward = szRankAward.."\n#cffffbe"..i..".排名第"..i.."公会：每人获得大量经验"..szMoney..awardItem
	end
	WorldCityWarInfoFrameRemindInfoRemind2:SetText( szRankAward, 255, 255, 255 )
	
	local passTime = WorldCityManager:GetWorldCityClanEnterTime()
	WorldCityWarInfoFrameEnterTime:SetText( "进入战场时间："..math.floor(passTime/60).."分钟" )
	local selfClanTotal = WorldCityManager:GetWorldCitySelClanTotal()
	local szRemindNeed = "#ca0a05a满足任意条件可获得战争奖励:"
	if math.floor(passTime/60) <= 25 then
		szRemindNeed = szRemindNeed.."\n#cff0000	1.进入战争场景超过25分钟"
		local uv = t_WorldCityWarInfoFrameRemindTexUV["unFinish"].uv
		WorldCityWarInfoFrameRemindInfoNeed1:SetTexUV( uv.x, uv.y, uv.w, uv.h )
	else
		szRemindNeed = szRemindNeed.."\n#c00ff00	1.进入战争场景超过25分钟"
		local uv = t_WorldCityWarInfoFrameRemindTexUV["finish"].uv
		WorldCityWarInfoFrameRemindInfoNeed1:SetTexUV( uv.x, uv.y, uv.w, uv.h )
	end
	if selfClanTotal <= 100 then
		szRemindNeed = szRemindNeed.."\n#cff0000	2.公会总积分超过100分"
		local uv = t_WorldCityWarInfoFrameRemindTexUV["unFinish"].uv
		WorldCityWarInfoFrameRemindInfoNeed2:SetTexUV( uv.x, uv.y, uv.w, uv.h )
	else
		szRemindNeed = szRemindNeed.."\n#c00ff00	2.公会总积分超过100分"
		local uv = t_WorldCityWarInfoFrameRemindTexUV["finish"].uv
		WorldCityWarInfoFrameRemindInfoNeed2:SetTexUV( uv.x, uv.y, uv.w, uv.h )
	end
	szRemindNeed = szRemindNeed.."\n#ca0a05a注：以上奖励，仅据点占领成员可获得"
	WorldCityWarInfoFrameRemindInfoRemind1:SetText( szRemindNeed, 255, 255, 255 )
end
local t_NumberToChineseNum = { "一", "二", "三", "四", "五", "六", }
function SetWorldCityWarInfoFrameWinInfo()
	local rank = WorldCityManager:GetWorldCitySelfClanRank()
	--非据点占领公会的处理
	if rank == 0 then
		WorldCityWarInfoFrameWinInfoRank:SetText("")
		WorldCityWarInfoFrameWinInfoGetMoney:SetText("")
		WorldCityWarInfoFrameWinInfoGetFont:SetText("")
		WorldCityWarInfoFrameWinInfoGetExp:SetText("")
		WorldCityWarInfoFrameWinInfoGetGift:SetText("")
	else
		local moneyType = GetMoneyDesc(WorldCityManager:GetWorldCitySelfClanMoneyType())
		local money = WorldCityManager:GetWorldCitySelfClanMoney()
		WorldCityWarInfoFrameWinInfoRank:SetText("第"..t_NumberToChineseNum[rank].."名")
		local passTime = WorldCityManager:GetWorldCityClanEnterTime()
		local selfClanTotal = WorldCityManager:GetWorldCitySelClanTotal()
		--没有满足条件的清空显示的金钱，并给与提示
		if math.floor(passTime/60) <= 25 and selfClanTotal <= 100 then
			WorldCityWarInfoFrameWinInfoGetMoney:SetTextColor( 255, 0, 0 )
			WorldCityWarInfoFrameWinInfoGetMoney:SetText("未满足获得战争奖励条件")
			WorldCityWarInfoFrameWinInfoGetFont:SetText("")
			WorldCityWarInfoFrameWinInfoGetExp:SetText("")
			WorldCityWarInfoFrameWinInfoGetGift:SetText("")
		else
			WorldCityWarInfoFrameWinInfoGetMoney:SetTextColor( 255, 200, 0 )
			WorldCityWarInfoFrameWinInfoGetMoney:SetText("获得"..money..moneyType)
			if money == 0 then
				WorldCityWarInfoFrameWinInfoGetMoney:SetText("")
			end
			WorldCityWarInfoFrameWinInfoGetFont:SetText("已获得以下奖励")
			local exp = WorldCityManager:GetWorldCityClanAwardExp( rank )
			local mainplayer = ActorMgr:getMainPlayer()
			local levelupDef = getLevelUP( mainplayer:getLv() )
			local getExp = math.floor(exp*( levelupDef.ContributeExp/10000.0 ))
			WorldCityWarInfoFrameWinInfoGetExp:SetText("经验:"..getExp)
			local awardItemDef = getItemDef(WorldCityManager:GetWorldCityClanAwardItemID( rank ))
			if WorldCityManager:GetWorldCityClanAwardItemID( rank ) == 0 then
				WorldCityWarInfoFrameWinInfoGetGift:SetText( "" )
			else
				WorldCityWarInfoFrameWinInfoGetGift:SetText( awardItemDef.Name.."(邮件发送)" )
			end
		end
	end	
	WorldCityWarInfoFrameEnterTime:Hide();
	--获取数据
	GetWorldCityClanInfoData();
	--最后一次消息，服务器排过序，客户端不排序
	--显示
	UpdateWorldCityClanInfo();
	if not WorldCityWarInfoFrame:IsShown() then	
		WorldCityWarInfoFrame:Show();
	end
end
function WorldCityWarInfoFrame_OnShow()	
	this:SetPoint( "center", "UIClient", "center", 0, 0 )
	if worldCityWarEnd then return end
	--获取数据
	GetWorldCityClanInfoData();
	--排序
	SortWorldCityClanInfo();
	--显示
	UpdateWorldCityClanInfo();
end
function WorldCityWarInfoFrame_OnUpdate()
	if worldCityWarEnd then return end
	--获取数据
	GetWorldCityClanInfoData();
	--排序
	SortWorldCityClanInfo();
	--显示
	UpdateWorldCityClanInfo();
end
