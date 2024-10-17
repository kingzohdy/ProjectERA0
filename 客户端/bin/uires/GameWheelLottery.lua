local normalSpeed = 1000
local acceleration = 1000
local degree = 90
local delay
local retard
local result
local lastTick
local startTick
local currentSpeed
local stop

--设置抽奖次数
function SetGameWheelLottery(i)
	GameWheelLotteryFrameTimes:SetText(i)
	if i == 0 then
		GameWheelLotteryFrameStart:Disable()
	else
		GameWheelLotteryFrameStart:Enable()
	end
end

--设置最终结果
function SetGameWheelLotteryResult(i)
	result = i
	currentSpeed = 0
	stop = nil
	delay = nil
	lastTick = getServerTime()
	startTick = lastTick
	GameWheelLotteryFrameStart:Hide()
	GameWheelLotteryFrameStop:DisChecked()
	GameWheelLotteryFrameStop:Show()
	GameWheelLotteryFrameClose:Disable()
	SetGameWheelLottery(GameWheelLottery:remain())
	StopGameWheelLotteryFramePoint()
end

--设置指针指向角度
function SetGameWheelLotteryFramePoint(angle)
	angle = angle % 360
	angle = 90 - angle
	GameWheelLotteryFramePoint:SetAngle(angle)
end

--开始减速
function StopGameWheelLotteryFramePoint()
	if stop and result then
		if result == -1 then
			GameWheelLottery:reset()
			GameWheelLotteryFrame:Hide()
			return
		end
		local pre = 360 / 16
		local distance = 360 - (pre * result + (math.random(0, 60) + 20) / 100 * pre - (degree % 360))
		distance = distance % 360
		distance = distance + math.floor(currentSpeed * currentSpeed / 200000) * 360
		if distance < 180 then
			distance = distance + 360
		end
		delay = distance * 2 / currentSpeed
		retard = currentSpeed / delay
	end
end

--更新待抽奖物品
function UpdateGameWheelLotteryFrameItems()
	if GameWheelLottery:remain() > 0 then
		local items = GameWheelLottery:getRandomItems(GameWheelLottery:top())
		local item
		for i = 0, 15 do
			item = getglobal("GameWheelLotteryFrameItem"..i)
			SetHundredLevelTownItemIcon(item, items.ItemID[i], items.Num[i])
		end
	end
end

function GameWheelLotteryFrame_OnLoad()
	GameWheelLotteryFrame:setUpdateTime(0)
end

function GameWheelLotteryFrame_OnShow()
	GameWheelLotteryFrameStart:Show()
	GameWheelLotteryFrameStop:Hide()
	GameWheelLotteryFrameGet:Hide()
	GameWheelLotteryFrameClose:Enable()
	lastTick = nil
	degree = math.random() * 360
	SetGameWheelLotteryFramePoint(degree)
	SetGameWheelLottery(GameWheelLottery:remain())
	UpdateGameWheelLotteryFrameItems()
	util.ShowInCenter(this)
end

function GameWheelLotteryFrame_OnStart()
	if not GameWheelLotteryFrameStart:IsShown() then
		return
	end
	result = nil
	GameWheelLottery:start()	
end

function GameWheelLotteryFrame_OnStop()
	if currentSpeed > 0 and not GameWheelLotteryFrameStop:IsChecked() then
		GameWheelLotteryFrameStop:Checked()
		stop = true
		StopGameWheelLotteryFramePoint()
	end
end

function GameWheelLotteryFrame_OnGet()
	GameWheelLotteryFrameGet:Hide()
	GameWheelLotteryFrameStart:Show()
	GameWheelLotteryFrameClose:Enable()
	GameWheelLottery:get()
	UpdateGameWheelLotteryFrameItems()
end

function GameWheelLotteryFrame_OnUpdate()
	if lastTick then
		if delay then
			local now = getServerTime()
			local during = (now.sec - lastTick.sec) + (now.usec - lastTick.usec) * 0.001
			lastTick = now
			delay = delay - during
			local speed
			if delay > 0 then
				speed = currentSpeed - retard * during
			else
				during = during + delay
				speed = 0
				lastTick = nil
				delay = nil
				GameWheelLotteryFrameStop:Hide()
				GameWheelLotteryFrameGet:Show()
			end
			degree = degree - (currentSpeed + speed) / 2 * during
			currentSpeed = speed
		else
			local now = getServerTime()
			local during = (now.sec - lastTick.sec) + (now.usec - lastTick.usec) * 0.001
			lastTick = now
			local speed = currentSpeed + acceleration * during
			if speed > normalSpeed then
				local t1 = (normalSpeed - currentSpeed) / acceleration
				local t2 = during - t1
				degree = degree - ((currentSpeed + normalSpeed) / 2 * t1 + normalSpeed * t2)
				currentSpeed = normalSpeed
			else
				degree = degree - (currentSpeed + speed) / 2 * during
				currentSpeed = speed
			end
			local diff = (lastTick.sec - startTick.sec) + (lastTick.usec - startTick.usec) * 0.001
			if diff >= 10 then
				GameWheelLotteryFrame_OnStop()--10秒后停止
			end
		end
		SetGameWheelLotteryFramePoint(degree)
	end
end

------------------------------------------------ 商城vip抽奖 ------------------------------------------------

local vipDegree = 90
local vipDelay
local vipRetard
local vipResult
local vipLastTick
local vipStartTick
local vipCurrentSpeed
local vipStop
local vipSelect

function VipWheelLotteryFrameCloseBtn_OnClick()
	if VipWheelLotteryFrameClose:IsShown() and VipWheelLotteryFrameClose:IsEnable() then
		VipWheelLotteryFrame:Hide();
	end
end

--设置抽奖次数
function SetVipWheelLottery(i)
	if VipWheelLotteryFrame:IsShown() then
		VipWheelLotteryFrameTimes:SetText("剩余抽奖次数：" .. i)
		if i == 0 then
			VipWheelLotteryFrameStart:Disable()
		else
			VipWheelLotteryFrameStart:Enable()
		end
	end
end
--设置最终结果
function SetVipWheelLotteryResult(i)
	vipResult = i
	vipCurrentSpeed = 0
	vipStop = nil
	vipDelay = nil
	vipLastTick = getServerTime()
	vipStartTick = vipLastTick
	VipWheelLotteryFrameStart:Hide()
	VipWheelLotteryFrameStop:DisChecked()
	VipWheelLotteryFrameStop:Show()
	VipWheelLotteryFrameClose:Disable()
	StopVipWheelLotteryFramePoint()
	SetVipWheelLottery( GameWheelLottery:getVipTimes() );
end

--设置指针指向角度
function SetVipWheelLotteryFramePoint(angle)
	angle = angle % 360
	angle = 90 - angle
	VipWheelLotteryFramePoint:SetAngle(angle)
end

--开始减速
function StopVipWheelLotteryFramePoint()
	if vipStop and vipResult then
		if vipResult == -1 then
			GameWheelLottery:reset()
			GameWheelLotteryFrame:Hide()
			return
		end
		local pre = 360 / 16
		local distance = 360 - (pre * vipResult + (math.random(0, 60) + 20) / 100 * pre - (vipDegree % 360))
		distance = distance % 360
		distance = distance + math.floor(vipCurrentSpeed * vipCurrentSpeed / 200000) * 360
		if distance < 180 then
			distance = distance + 360
		end
		vipDelay = distance * 2 / vipCurrentSpeed
		vipRetard = vipCurrentSpeed / vipDelay
	end
end

--更新待抽奖物品
function UpdateVipWheelLotteryFrameItems()
	local items = GameWheelLottery:getRandomItems(GameWheelLottery:getVipIdx( vipSelect ))
	local item
	for i = 0, 15 do
		item = getglobal("VipWheelLotteryFrameItem"..i)
		SetHundredLevelTownItemIcon(item, items.ItemID[i], items.Num[i])
	end
end

function VipWheelLotteryFrame_OnLoad()
	VipWheelLotteryFrame:setUpdateTime(0)
end

function VipWheelLotteryFrame_OnShow()
	VipWheelLotteryFrameStart:Show()
	VipWheelLotteryFrameStop:Hide()
	VipWheelLotteryFrameGet:Hide()
	VipWheelLotteryFrameClose:Enable()
	vipLastTick = nil
	vipDegree = math.random() * 360
	SetVipWheelLotteryFramePoint(vipDegree)	
	SetVipWheelLottery( GameWheelLottery:getVipTimes() );
	for i=1,3 do
		local check = getglobal( "VipWheelLotteryFrameCheckBtn" .. i );
		check:setCheckState( false );
	end
	vipSelect = 1;
	VipWheelLotteryFrameCheckBtn1:setCheckState( true );
	UpdateVipWheelLotteryFrameItems()
	util.ShowInCenter(this)
end

function VipWheelLotteryFrame_OnStart()
	if not VipWheelLotteryFrameStart:IsShown() then
		return
	end	
	vipResult = nil
	GameWheelLottery:vipStart( vipSelect )
end

function VipWheelLotteryFrame_OnStop()
	if vipCurrentSpeed > 0 then
		VipWheelLotteryFrameStop:Checked()
		vipStop = true
		StopVipWheelLotteryFramePoint()
	end
end

function VipWheelLotteryFrame_OnGet()
	VipWheelLotteryFrameGet:Hide()
	VipWheelLotteryFrameStart:Show()
	VipWheelLotteryFrameClose:Enable()
	GameWheelLottery:vipGet( vipSelect )
	UpdateVipWheelLotteryFrameItems()
end

function VipWheelLotteryFrame_OnUpdate()
	if vipLastTick then
		if vipDelay then
			local now = getServerTime()
			local during = (now.sec - vipLastTick.sec) + (now.usec - vipLastTick.usec) * 0.001
			vipLastTick = now
			vipDelay = vipDelay - during
			local speed
			if vipDelay > 0 then
				speed = vipCurrentSpeed - vipRetard * during
			else
				during = during + vipDelay
				speed = 0
				vipLastTick = nil
				vipDelay = nil
				VipWheelLotteryFrameStop:Hide()
				VipWheelLotteryFrameGet:Show()
			end
			vipDegree = vipDegree - (vipCurrentSpeed + speed) / 2 * during
			vipCurrentSpeed = speed
		else
			local now = getServerTime()
			local during = (now.sec - vipLastTick.sec) + (now.usec - vipLastTick.usec) * 0.001
			vipLastTick = now
			local speed = vipCurrentSpeed + acceleration * during
			if speed > normalSpeed then
				local t1 = (normalSpeed - vipCurrentSpeed) / acceleration
				local t2 = during - t1
				vipDegree = vipDegree - ((vipCurrentSpeed + normalSpeed) / 2 * t1 + normalSpeed * t2)
				vipCurrentSpeed = normalSpeed
			else
				vipDegree = vipDegree - (vipCurrentSpeed + speed) / 2 * during
				vipCurrentSpeed = speed
			end
			local diff = (vipLastTick.sec - vipStartTick.sec) + (vipLastTick.usec - vipStartTick.usec) * 0.001
			if diff >= 10 then
				VipWheelLotteryFrame_OnStop()--10秒后停止
			end
		end
		SetVipWheelLotteryFramePoint(vipDegree)
	end
end

function VipWheelLotteryFrameCheckBtn_OnClick()
	if not (VipWheelLotteryFrameStart:IsShown() and VipWheelLotteryFrameStart:IsEnable()) then
		this:setCheckState( false );
		return;
	end
	for i=1,3 do
		local check = getglobal("VipWheelLotteryFrameCheckBtn"..i);
		check:setCheckState( false );
	end
	this:setCheckState( true );
	vipSelect = this:GetClientID();
	UpdateVipWheelLotteryFrameItems()
end