local Parent
local Extra

function BlockMessageBox_OnEnter()
	local parent = getglobal(this:GetParent())
	result(parent, true)
	parent:Hide()
end

function BlockMessageBox_OnLoad()
	this:setUpdateTime(0)
end

function BlockMessageBox_OnShow()
	
end

function BlockMessageBox_OnHide()
	unblock(this)
end

function BlockMessageBox_OnUpdate()
	if Parent and not Parent:IsShown() then
		this:Hide()
	end
end

function SetBlockMessage(frame, title, content)
	local name = frame:GetName()
	local ui
	ui = getglobal(name.."Title")
	ui:SetText(title)
	ui = getglobal(name.."Text")
	ui:clearHistory()
	ui:resizeRect(320, 20)
	ui:SetSize(320, 20)
	ui:SetText(content, 215, 190, 132)
	local height = ui:GetTotalHeight()
	local width = 1
	for i = 1, ui:GetTextLines() do
		local w = ui:getLineWidth(i - 1)
		if width < w then
			width = w
		end
	end
	ui:resizeRect(width, height)
	ui:SetSize(math.ceil(width / GetScreenScaleY()), math.ceil(height / GetScreenScaleY()))
	ui:SetText(content, 215, 190, 132)
	local width, height = math.max(36 + width, 320), math.max(90 + height, 120)
	ui = getglobal(name.."Line")
	ui:SetWidth(width - 6)
	frame:SetSize(width, height)
end

function MessageBoxBlock(title, content, parent, ...)
	if BlockMessageBox:IsShown() then
		BlockMessageBox:Hide()
	end
	Parent = parent
	Extra = {...}
	SetBlockMessage(BlockMessageBox, title, content)
	BlockMessageBox:Show()
	local relfram = UIClient
	if parent then
		--relfram = parent
	end
	BlockMessageBox:SetPoint("center", relfram:GetName(), "center", 0, 0)
	return block(BlockMessageBox)
end

function MessageBoxBlockEnd(parent)
	if parent and parent ~= Parent then
		return
	end
	if BlockMessageBox:IsShown() then
		BlockMessageBox:Hide()
	end
end

function IsMessageBoxBlock(frame)
	if not BlockMessageBox:IsShown() then
		return false
	end
	if not frame then
		return true, unpack(Extra)
	end
	local parent = Parent
	while true do
		if not parent then
			break
		end
		if parent == frame then
			return true, unpack(Extra)
		end
		parent = parent:GetParent()
		if parent then
			parent = getglobal(parent)
		end
	end
	return false
end