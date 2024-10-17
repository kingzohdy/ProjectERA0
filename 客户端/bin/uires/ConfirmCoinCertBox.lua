local Parent

function ConfirmCoinCertBox_OnCoin()
	if not this:GetCheckState() then
		this:setCheckState(true)
	end
	ConfirmCoinCertBoxCert:setCheckState(false)
end

function ConfirmCoinCertBox_OnCert()
	if not this:GetCheckState() then
		this:setCheckState(true)
	end
	ConfirmCoinCertBoxCoin:setCheckState(false)
end

function ConfirmCoinCertBox_OnOK()
	local parent = getglobal(this:GetParent())
	if ConfirmCoinCertBoxCoin:GetCheckState() then
		result(parent, "coin")
	else
		result(parent, "cert")
	end
	parent:Hide()
end

function ConfirmCoinCertBox_OnLoad()
	this:setUpdateTime(0)
end

function ConfirmCoinCertBox_OnShow()
	ConfirmCoinCertBoxCoin:setCheckState(true)
	ConfirmCoinCertBoxCert:setCheckState(false)
end

function ConfirmCoinCertBox_OnHide()
	unblock(this)
end

function ConfirmCoinCertBox_OnUpdate()
	if Parent and not Parent:IsShown() then
		this:Hide()
	end
end

function ConfirmCoinCertBoxBlock(title, content, parent)
	if ConfirmCoinCertBox:IsShown() then
		ConfirmCoinCertBox:Hide()
	end
	if parent then
		ConfirmCoinCertBox:SetPoint("center", parent:GetName(), "center", 0, 0)
	end
	Parent = parent
	ConfirmCoinCertBoxTitle:SetText(title)
	ConfirmCoinCertBoxText:clearHistory()
	ConfirmCoinCertBoxText:resizeRect(215, 50)
	ConfirmCoinCertBoxText:SetSize(215, 50)
	ConfirmCoinCertBoxText:SetText(content, 255, 254, 189)
	local height = ConfirmCoinCertBoxText:GetTotalHeight()
	local width = 1
	for i = 1, ConfirmCoinCertBoxText:GetTextLines() do
		local w = ConfirmCoinCertBoxText:getLineWidth(i - 1)
		if width < w then
			width = w
		end
	end
	ConfirmCoinCertBoxText:resizeRect(width, height)
	ConfirmCoinCertBoxText:SetSize(math.ceil(width / GetScreenScaleY()), math.ceil(height / GetScreenScaleY()))
	ConfirmCoinCertBoxText:SetText(content, 255, 254, 189)
	local width, height = math.max(25 + width, 240), math.max(90 + height, 100)
	ConfirmCoinCertBox:SetSize(width, height)
	ConfirmCoinCertBox:Show()
	return block(ConfirmCoinCertBox)
end