local showTime
local totalTime

local function UpdateBlockPrepareSlip()
	local now = os.clock()
	local during = now - showTime
	if during < 0 then
		during = 0
		showTime = now
	end
	if during >= totalTime then
		result(BlockPrepareSlip, true)
		BlockPrepareSlip:Hide()
	else
		local scale = during / totalTime
		BlockPrepareSlipTex:SetTexUV(81 + (1 - scale) * 202, 620, scale * 202, 32)
		BlockPrepareSlipTex:SetSize(scale * 202, 32)
	end
end

function BlockPrepareSlip_OnLoad()
	this:setUpdateTime(0)
end

function BlockPrepareSlip_OnShow()
	showTime = os.clock()
	UpdateBlockPrepareSlip()
end

function BlockPrepareSlip_OnHide()
	unblock(BlockPrepareSlip)
end

function BlockPrepareSlip_OnUpdate()
	UpdateBlockPrepareSlip()
end

function StartBlockPrepareSlip(time)
	totalTime = time
	BlockPrepareSlip:Show()
end