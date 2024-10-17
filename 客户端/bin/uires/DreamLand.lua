

-------------------------------------------------BOSS查询-------------------------------------------------

local bossMaps = {
	2600, 2601, 2602, 2603, 2604, 2605, 2606, 2607, 2608, 2609,
}

local bossMapsName = {}

for i = 1, #bossMaps do
	bossMapsName[i] = getMapDef(bossMaps[i]).Name
end

local function UpdateDreamLandBossFrame()
	local i = 1
	local ui
	while true do
		ui = getglobal("DreamLandBossFrameEntry"..i)
		if not ui then
			break
		end
		local suffix = "领主"
		if i == #bossMaps then
			suffix = "君主"
		end
		if DreamLandMgr:IsBossLayerExist(i) then
			ui:SetText(string.format("%s%s       刷新", bossMapsName[i], suffix, i))
		else
			ui:SetText(string.format("%s%s       死亡", bossMapsName[i], suffix, i))
		end
		i = i + 1
	end
end

function DreamLandBossFrame_OnLoad()
	local i = 1
	local ui
	while true do
		ui = getglobal("DreamLandBossFrameEntry"..i.."BlackTex")
		if not ui then
			break
		end
		ui:Hide()
		i = i + 2
	end
	this:RegisterEvent("GE_DREAMLAND_BOSSINFO")
end

function DreamLandBossFrame_OnEvent()
	if arg1 == "GE_DREAMLAND_BOSSINFO" then
		if this:IsShown() then
			UpdateDreamLandBossFrame()
		end
	end
end

function DreamLandBossFrame_OnShow()
	DreamLandSelectLayerFrame:Hide()
	DreamLandTeamFrame:Hide()
	util.ShowInCenter(this)
	UpdateDreamLandBossFrame()
end

function OpenDreamLandBossFrame(npcId)
	DreamLandBossFrame:Show()
	if npcId then
		util.testNpcDialogDistance(npcId, DreamLandBossFrame)
	end
end

-------------------------------------------------地图传送-------------------------------------------------

local selectLayer

local function GetSelectLayer(i)
	if i then
		return (i - 1) * 10 + 1
	end
	return (selectLayer - 1) * 10 + 1
end

local function SelectDreamLandLayer(i)
	local ui
	if selectLayer then
		ui = getglobal("DreamLandSelectLayerFrameEntry"..selectLayer)
		ui:DisChecked()
	end
	selectLayer = i
	if selectLayer then
		ui = getglobal("DreamLandSelectLayerFrameEntry"..selectLayer)
		ui:Checked()
	end
end

local function UpdateDreamLandSelectLayerFrame()
	local i = 1
	local ui
	while true do
		ui = getglobal("DreamLandSelectLayerFrameEntry"..i)
		if not ui then
			break
		end
		if DreamLandMgr:IsLayerSaved(GetSelectLayer(i)) then
			ui:Enable()
		else
			ui:Disable()
			if selectLayer == i then
				SelectDreamLandLayer()
			end
		end
		i = i + 1
	end
end

function DreamLandSelectLayerFrame_OnLoad()
	local i = 1
	local ui
	while true do
		ui = getglobal("DreamLandSelectLayerFrameEntry"..i)
		if not ui then
			break
		end
		ui:SetText(DreamLandMgr:GetLayerName((i - 1) * 10 + 1))
		ui = getglobal("DreamLandSelectLayerFrameEntry"..i.."BlackTex")
		if i % 2 == 1 then
			ui:Hide()
		end
		i = i + 1
	end
	this:RegisterEvent("GE_DREAMLAND_LAYER_SAVE")
	this:RegisterEvent("GE_DREAMLAND_ENTER")
end

function DreamLandSelectLayerFrame_OnEvent()
	if arg1 == "GE_DREAMLAND_LAYER_SAVE" then
		if this:IsShown() then
			UpdateDreamLandSelectLayerFrame()
		end
	elseif arg1 == "GE_DREAMLAND_ENTER" then
		local name = DreamLandMgr:GetCurrentLayerName()
		if name then
			MMMapName:SetText(name)
		end
		local effectName = DreamLandMgr:GetCurrentLayerEffectName();
		if effectName then
			setOverlayRegion( effectName )
		end
	end
end

function DreamLandSelectLayerFrame_OnShow()
	DreamLandBossFrame:Hide()
	DreamLandTeamFrame:Hide()
	util.ShowInCenter(this)
	SelectDreamLandLayer(1)
	UpdateDreamLandSelectLayerFrame()
end

function DreamLandSelectLayerFrame_OnOK()
	if not selectLayer then
		return
	end
	util.closeParent(this)
	DreamLandMgr:SelectLayer(GetSelectLayer())
end

function DreamLandSelectLayerEntry_OnClick()
	SelectDreamLandLayer(this:GetClientID())
end

function OpenDreamLandSelectLayerFrame(npcId)
	DreamLandSelectLayerFrame:Show()
	if npcId then
		util.testNpcDialogDistance(npcId, DreamLandSelectLayerFrame)
	end
end

-------------------------------------------------队伍查询-------------------------------------------------

local selectedTeamIndex
local selectedTeamName
local MAX_ENTRY_NUM = 10;
local t_TeamInfo = {};

local function SelectDreamLandTeam(idx)
	local ui
	for i=1, MAX_ENTRY_NUM do
		ui = getglobal("DreamLandTeamFrameEntry"..i);
		ui:DisChecked()
	end
	selectedTeamIndex = idx
	if selectedTeamIndex then
		ui = getglobal("DreamLandTeamFrameEntry"..selectedTeamIndex)
		ui:Checked()
		ui = getglobal("DreamLandTeamFrameEntry"..selectedTeamIndex.."Name")
		selectedTeamName = ui:GetText()
	else
		selectedTeamName = nil
	end
end

local function GetAllTeamInfo()
	t_TeamInfo={};
	for i=1, DreamLandMgr:GetTeamNum() do
		local info = DreamLandMgr:GetTeamInfo(i-1);
		table.insert( t_TeamInfo,info );
	end
end

local function UpdateDreamLandTeamFrame( nSliderValue )
	local nListIndex = 1
	local name
	local ui
	local selected
	for i = nSliderValue, DreamLandMgr:GetTeamNum() do
		if nListIndex > MAX_ENTRY_NUM then
			break;
		end

		local info = t_TeamInfo[i];
		name = "DreamLandTeamFrameEntry"..nListIndex;
		ui = getglobal(name)
		ui:Enable()

		local fonts = {Name = info.Name, Level = info.Level, Count = info.Count, Layer = info.Layer}
		for suffix, value in pairs(fonts) do
			ui = getglobal(name..suffix);
			ui:SetText(value);
		end

		if selectedTeamName ~= "" and info.Name == selectedTeamName then
			selected = nListIndex;
		end
		nListIndex = nListIndex + 1;
	end

	for i = nListIndex, MAX_ENTRY_NUM do
		name = "DreamLandTeamFrameEntry"..i;
		ui = getglobal(name);
		ui:Disable();
		local fonts = {"Name", "Level", "Count", "Layer"};
		for _, suffix in ipairs(fonts) do
			ui = getglobal(name..suffix);
			ui:SetText("");
		end
	end

	if selected then
		SelectDreamLandTeam(selected)
	elseif selectedTeamIndex then
		ui = getglobal("DreamLandTeamFrameEntry"..selectedTeamIndex);
		ui:DisChecked()
	end
end

function DreamLandTeamFrame_OnLoad()
	local i = 1
	local ui
	while true do
		ui = getglobal("DreamLandTeamFrameEntry"..i.."BlackTex")
		if not ui then
			break
		end
		ui:Hide()
		i = i + 2
	end
end

function DreamLandTeamFrame_OnShow()
	DreamLandBossFrame:Hide()
	DreamLandSelectLayerFrame:Hide()
	util.ShowInCenter(this)
	GetAllTeamInfo();
	SelectDreamLandTeam()
	UpdateDreamLandTeamFrame(1);
	selectedTeamIndex = nil;
	selectedTeamName = nil;

	if DreamLandMgr:GetTeamNum() > MAX_ENTRY_NUM then
		DreamLandTeamFrame_ScrollBar:SetMaxValue( DreamLandMgr:GetTeamNum() - MAX_ENTRY_NUM );
		DreamLandTeamFrame_ScrollBar:SetValue(0);
		DreamLandTeamFrame_ScrollBar:Show();
		DreamLandTeamFrame_ScrollUpBtn:Show();
		DreamLandTeamFrame_ScrollDownBtn:Show();
	else
		DreamLandTeamFrame_ScrollBar:Hide();
		DreamLandTeamFrame_ScrollUpBtn:Hide();
		DreamLandTeamFrame_ScrollDownBtn:Hide();
	end
end

function DreamLandTeamFrame_OnJoin()
	if not selectedTeamName or selectedTeamName == "" then
		return
	end
	TeamManager:Team_OP_Add(selectedTeamName)
end

function DreamLandTeamEntry_OnEnter()
	local name = this:GetName()
	local tex = getglobal(name.."Highlight")
	tex:Show()
	local text
	local fonts = {"Name", "Level", "Count", "Layer"}
	for _, suffix in ipairs(fonts) do
		text = getglobal(name..suffix)
		text:SetTextColor(255, 255, 190)
	end
end

function DreamLandTeamEntry_OnLeave()
	local name = this:GetName()
	local tex = getglobal(name.."Highlight")
	tex:Hide()
	local text
	local fonts = {"Name", "Level", "Count", "Layer"}
	for _, suffix in ipairs(fonts) do
		text = getglobal(name..suffix)
		text:SetTextColor(130, 130, 130)
	end
end

function DreamLandTeamEntry_OnClick()
	SelectDreamLandTeam(this:GetClientID())
end

function OpenDreamLandTeamFrame(npcId)	
	if not DreamLandTeamFrame:IsShown() and npcId then
		DreamLandTeamFrame:Show()
		util.testNpcDialogDistance(npcId, DreamLandTeamFrame)
	else
		DreamLandTeamFrame:Show();
	end
end

function DreamLandTeamFrame_ScrollBar_OnValueChanged()
	local slider = getglobal("DreamLandTeamFrame_ScrollBar");
	local nValue = slider:GetValue();
	local nMaxValue = slider:GetMaxValue();

	if nValue <= nMaxValue then
		UpdateDreamLandTeamFrame( nValue + 1 );
	end
end

function DreamLandTeamFrame_ScrollDownBtn_OnClick()
	local slider = getglobal("DreamLandTeamFrame_ScrollBar");
	local nValue = slider:GetValue() + slider:GetValueStep();
	local nMaxValue = slider:GetMaxValue();
	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

function DreamLandTeamFrame_ScrollUpBtn_OnClick()	
	local slider = getglobal("DreamLandTeamFrame_ScrollBar");
	local nValue = slider:GetValue() - slider:GetValueStep();
	if nValue >= 0 then
		slider:SetValue( nValue );
	end	
end