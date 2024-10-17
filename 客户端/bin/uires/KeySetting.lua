local keys
local MAX_ENTRY = 18


function KeySettingFrame_ShiPinSettingButton_OnClick()
	local curShowFrame = getglobal(this:GetParent())
	SystemSettingFrame:SetPoint("topleft", "UIClient", "topleft", curShowFrame:GetRealLeft(), curShowFrame:GetRealTop())
	SystemSettingFrame:Show()
	curShowFrame:Hide()
end

function KeySettingFrame_XianShiSettingButton_OnClick()
	local curShowFrame = getglobal(this:GetParent())
	GameSettingFrame:SetPoint("topleft", "UIClient", "topleft", curShowFrame:GetRealLeft(), curShowFrame:GetRealTop())
	GameSettingFrame:Show()
	curShowFrame:Hide()
end

function KeySettingFrame_MouseSettingButton_OnClick()
	local curShowFrame = getglobal(this:GetParent())
	MouseSettingFrame:SetPoint("topleft", "UIClient", "topleft", curShowFrame:GetRealLeft(), curShowFrame:GetRealTop())
	MouseSettingFrame:Show()
	curShowFrame:Hide()
end

function KeySettingFrame_OnLoad()
	KeySettingFrame_KeySettingButton:Checked()
end

function KeySettingFrame_OnHide()
	EndTrySetKeyMap()
end

function KeySettingFrame_OnShow()
	InitKeySettings()
	KeySettingFrameDesc:SetText("")
	local value = KeySettingFrameScrollBar:GetValue()
	if value ~= 0 then
		KeySettingFrameScrollBar:SetValue(0)
	else
		UpdateKeySettingFrame()
	end
end

function KeySettingFrame_ScrollUpBtn()
	local slider = KeySettingFrameScrollBar
	if not slider:IsShown() then
		return
	end
	local nValue = slider:GetValue() - slider:GetValueStep()
	if nValue >= slider:GetMinValue() then
		slider:SetValue(nValue)
	end
end

function KeySettingFrame_ScrollDownBtn()
	local slider	= KeySettingFrameScrollBar
	if not slider:IsShown() then
		return
	end
	local nValue	= slider:GetValue() + slider:GetValueStep()
	if nValue <= slider:GetMaxValue() then
		slider:SetValue(nValue)
	end
end

function KeySettingFrame_OnMouseWheel()
	local selected = TrySetKeyMap()
	if selected then
		TrySetKeyMap(selected)
	else
		local old = this
		this = KeySettingFrameScrollBar
		ScrollBar_OnMouseWheel()
		this = old
	end
end

function KeySettingFrame_ScrollBarValueChange()
	UpdateKeySettingFrame()
end

function UpdateKeySettingFrame()
	local selected = TrySetKeyMap()
	if selected then
		TrySetKeyMap(selected)
		KeySettingFrameReset:Enable()
	else
		KeySettingFrameReset:Disable()
	end
	local max = #keys
	local value = KeySettingFrameScrollBar:GetValue()
	local maxValue = max < MAX_ENTRY and 0 or (max - MAX_ENTRY)
	if maxValue == 0 then
		KeySettingFrameScrollBar:Hide()
	else
		KeySettingFrameScrollBar:Show()
	end
	KeySettingFrameScrollBar:SetMaxValue(maxValue)
	if value > maxValue then
		KeySettingFrameScrollBar:SetValue(maxValue)
	else
		local ui
		for i = 1, MAX_ENTRY do
			ui = getglobal("KeySettingFrameSpace"..i)
			if (i + value) % 2 == 0 then
				ui:Show()
			else
				ui:Hide()
			end
			ui = getglobal("KeySettingFrameEntry"..i)
			if i + value > max then
				ui:Hide()
			else
				ui:Show()
				local entry = keys[i + value]
				if type(entry) == "table" then--快捷键
					ui = getglobal("KeySettingFrameEntry"..i.."TypeTex")
					ui:Hide()
					ui = getglobal("KeySettingFrameEntry"..i.."Type")
					ui:Hide()
					ui = getglobal("KeySettingFrameEntry"..i.."Name")
					ui:Show()
					ui:SetText(entry[1])
					ui = getglobal("KeySettingFrameEntry"..i.."DefaultKey")
					ui:Show()
					ui:SetText(entry[2] or "")
					ui = getglobal("KeySettingFrameEntry"..i.."NowKey")
					ui:Show()
					if selected == entry[1] then
						ui:Checked()
					else
						ui:DisChecked()
					end
					ui = getglobal("KeySettingFrameEntry"..i.."NowKeyText")
					local nowKey = GetKeyMap(entry[1])
					if nowKey == 0 then
						nowKey = nil
					end
					nowKey = KeyToString(nowKey)
					if nowKey then
						ui:SetText(nowKey)
						ui:SetTextColor(255, 255, 190)
					else
						ui:SetText("未设置")
						ui:SetTextColor(150, 150, 90)
					end
				else--分类
					ui = getglobal("KeySettingFrameEntry"..i.."Name")
					ui:Hide()
					ui = getglobal("KeySettingFrameEntry"..i.."DefaultKey")
					ui:Hide()
					ui = getglobal("KeySettingFrameEntry"..i.."NowKey")
					ui:Hide()
					ui = getglobal("KeySettingFrameEntry"..i.."TypeTex")
					ui:Show()
					ui = getglobal("KeySettingFrameEntry"..i.."Type")
					ui:Show()
					ui:SetText(entry)
				end
			end
		end
	end
end

function OnAccelOverlayKeySettingFrame(name)
	KeySettingFrameDesc:SetText(string.format("[%s]功能未设置", name))
	KeySettingFrameDesc:SetTextColor(255, 0, 0)
end

function KeySettingFrameSave_OnClick()
	MessageBox("提示","是否确定修改并保存当前按键设置？")
	MessageBoxFrame:SetClientString("按键设置")
end

function KeySettingFrameResetAll_OnClick()
	MessageBox("提示","是否确定把所有按键恢复成默认设置？")
	MessageBoxFrame:SetClientString("恢复键位设置")
end

function KeySettingFrameReset_OnClick()
	local selected = TrySetKeyMap()
	if selected then
		StartTrySetKeyMap()
		ResetKeyMap(selected)
		this:Disable()
	end
end

function KeySettingFrameOnSave()
	EndTrySetKeyMap(true)
end

function KeySettingFrameOnResetAll()
	StartTrySetKeyMap()
	ResetKeyMap()
	EndTrySetKeyMap(true)
end

function InitKeySettings()
	keys = {}
	for type in KeyMapTypes() do
		table.insert(keys, type)
		for name in KeyMapFunctions(type) do
			local _, default = GetKeyMap(name)
			table.insert(keys, {name, KeyToString(default)})
		end
	end
end

function KeySettingEntryNowKey_OnClick()
	local value = KeySettingFrameScrollBar:GetValue()
	local id = getglobal(this:GetParent()):GetClientID()
	local name = keys[value + id][1]
	local old = TrySetKeyMap()
	if old ~= name then
		TrySetKeyMap(name)
		this:Checked()
		for i, v in ipairs(keys) do
			if type(v) == "table" and v[1] == old then
				local ui = getglobal("KeySettingFrameEntry"..(i - value).."NowKey")
				ui:DisChecked()
				break
			end
		end
		KeySettingFrameReset:Enable()
		KeySettingFrameDesc:SetText(string.format("点击键盘按键，以设置[%s]的快捷键", name))
		KeySettingFrameDesc:SetTextColor(150, 150, 90)
	else
		this:DisChecked()
		KeySettingFrameReset:Disable()
		KeySettingFrameDesc:SetText("")
	end
end