local selectedButton

local mapLineInfo = {}

local mapBusyText = {
	"流畅",
	"拥挤",
	"爆满",
}

local mapBusyColor = {
	{0, 255, 0},
	{255, 125, 0},
	{255, 0, 0},
	
	default = {255, 255, 255}
}

local miniMapButtonTex = {
	{
		normal		= {935, 417, 33, 24},
		highlight	= {935, 442, 33, 24},
		push		= {935, 467, 33, 24},
	},

	{
		normal		= {935, 417, 33, 24},
		highlight	= {935, 442, 33, 24},
		push		= {935, 467, 33, 24},
	},

	{
		normal		= {935, 417, 33, 24},
		highlight	= {935, 442, 33, 24},
		push		= {935, 467, 33, 24},
	},
}

local SetMapServerSelectButtonSelect
local clearLastSelectMapServerSelectButton
function clearLastSelectMapServerSelectButton()
	if selectedButton then
		SetMapServerSelectButtonSelect(selectedButton, false)
	end
end
function SetMapServerSelectButtonSelect(button, select)
	local name = button:GetName()
	local selected = getglobal(name.."Select")
	local font1 = getglobal(name.."Text1")
	local font2 = getglobal(name.."Text2")
	if select then
		clearLastSelectMapServerSelectButton()
		selected:Show()
		local color = mapBusyColor.default
		font1:SetTextColor(color[1], color[2], color[3])
		font2:SetTextColor(color[1], color[2], color[3])
		selectedButton = button
	else
		selected:Hide()
		local color = mapBusyColor[mapLineInfo[button:GetClientID()][2]] or mapBusyColor.default
		font1:SetTextColor(color[1], color[2], color[3])
		font2:SetTextColor(color[1], color[2], color[3])
		if selectedButton == button then
			selectedButton = nil
		end
	end
end

local function MapServerSelectFrameUpdate()
	for i = 1, #mapLineInfo do
		local btn = getglobal("MapServerSelectFrameClause"..i)
		if btn then
			btn:Show()
			local font1 = getglobal("MapServerSelectFrameClause"..i.."Text1")
			local font2 = getglobal("MapServerSelectFrameClause"..i.."Text2")
			local t = mapLineInfo[i]
			if not(selectedButton and selectedButton:GetClientID() == i) then
				local color = mapBusyColor[t[2]] or mapBusyColor.default
				font1:SetTextColor(color[1], color[2], color[3])
				font2:SetTextColor(color[1], color[2], color[3])
			end
			font1:SetText(getMapName(t[1]))
			font2:SetText(mapBusyText[t[2]] or "")
		end
	end
	for i = #mapLineInfo + 1, 7 do
		local btn = getglobal("MapServerSelectFrameClause"..i)
		if btn then
			btn:Hide()
		end
	end
end

local function CanSelectMapServer()
	if GameExchange:isInExchange() then
		ShowMidTips("你正在进行交易，无法切换线路")
		return false
	end
	if ActorMgr:getMainPlayer():isInFight() then
		ShowMidTips("战斗状态下无法切换线路")
		return false
	end
	if IntonateBar:IsShown() then
		ShowMidTips("你正在做其他事，无法切换线路")
		return false
	end
	if ActorMgr:getMainPlayer():isInFly() then
		ShowMidTips("飞行状态下无法切换线路")
		return false
	end
	return true
end

local function RequestMapServerSelect()
	if selectedButton then
		MapLineSelecter:requestChangeMapLine(mapLineInfo[selectedButton:GetClientID()][1])
	end
end

local function AcceptMapServerSelectInfo()
	mapLineInfo = {}
	local curMapId = MapLineSelecter:getCurMapId()
	for i = 0, MapLineSelecter:getMapLineNum() - 1 do
		local mapId = MapLineSelecter:getMapLineId(i)
		if mapId == curMapId then
			local t = miniMapButtonTex[MapLineSelecter:getMapLineState(i)]
			MapServerSelectOpenBtn:ChangeNormalTexture(t.normal[1], t.normal[2], t.normal[3], t.normal[4])
			MapServerSelectOpenBtn:ChangeHighlightTexture(t.highlight[1], t.highlight[2], t.highlight[3], t.highlight[4])
			MapServerSelectOpenBtn:ChangePushedTexture(t.push[1], t.push[2], t.push[3], t.push[4])
		else
			table.insert(mapLineInfo, {mapId, MapLineSelecter:getMapLineState(i)})
		end
	end
end

local function AskMapServerSelectInfo()
	MapLineSelecter:requestMapLineInfo()
end

function MapServerSelectButton_OnEnter()
	local name = this:GetName()
	local highlight = getglobal(name.."Highlight")
	highlight:Show()
end

function MapServerSelectButton_OnLeave()
	local name = this:GetName()
	local highlight = getglobal(name.."Highlight")
	highlight:Hide()
end

function MapServerSelectButton_OnClick()
	SetMapServerSelectButtonSelect(this, true)
end

function MapServerSelectButton_OnDBClick()
	SetMapServerSelectButtonSelect(this, true)
	MapServerSelectFrame_OnSelect()
end

function MapServerSelectFrame_OnSelect()
	if not CanSelectMapServer() then
		return
	end
	if selectedButton then
		MapServerSelectFrame:Hide()
		RequestMapServerSelect()
	end
end

function MapServerSelectFrame_OnLoad()
	this:RegisterEvent("GE_MAP_LINE_INFO")
end

function MapServerSelectFrame_OnEvent()
	if arg1 == "GE_MAP_LINE_INFO" then
		AcceptMapServerSelectInfo()
		if MapLineSelecter:getMapLineNum() == 0 or LoginMgr:isCloseLine() then
			MapServerSelectFrame:Hide()
			MapServerSelectOpenBtn:Hide()
		else
			MapServerSelectOpenBtn:Show()
		end
		if MapServerSelectFrame:IsShown() then
			MapServerSelectFrameUpdate()
		end
	end
end



function MapServerSelectFrame_OnShow()
	util.ShowInCenter(this)
	clearLastSelectMapServerSelectButton()
	MapServerSelectFrameUpdate()
end


function MapServerSelectOpenBtn_OnClick()
	if MapServerSelectFrame:IsShown() then
		return
	end
	if CanSelectMapServer() then
		MapServerSelectFrame:Show()
	end
end

function MapServerSelectOpenBtn_OnEnter()
	SetGameTooltips(this:GetName(), "切换地图线路")
end

function MapServerSelectOpenBtn_OnLeave()
	GameTooltipRichText:SetText("", 255, 255, 255)
	GameTooltip:Hide()
end

local lastAskTime = 0

function MapServerSelectOpenBtn_OnUpdate()
	local now = os.clock()
	if now - lastAskTime >= 15 then
		lastAskTime = now
		AskMapServerSelectInfo()
	end
end