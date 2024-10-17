local ModeName = {
	[0] = "和平模式",
	"中立模式",
	"自由模式",
	"队伍模式",
	"公会模式",
	"阵营模式",
	"公会宣战模式",
}
local ModeNameTips = {
	[0] = "和平模式：无法攻击任何玩家",
	"中立模式：可自由攻击闪名和红名玩家，无法攻击其他玩家",
	"自由模式：可自由攻击任何玩家",
	"队伍模式：可自由攻击其他玩家，无法攻击同队伍玩家",
	"公会模式：可自由攻击其他玩家，无法攻击本公会玩家",
	"阵营模式：#c00ff00（可自由攻击非己方阵营玩家）#n",
	"公会宣战模式：可攻击处于宣战状态的敌对公会成员、非本公会的闪名、红名、黑名",
}

local points = {
	{x = 0, y = 0},
	{x = -12, y = 17},
	{x = -29, y = 23},
	{x = -48, y = 25},
	{x = -67, y = 26},
	{x = -67, y = 26},
	{x = -86, y = 24},
}

local Max_MODE_NUM = 6

local step = {}
for i = 1, #points do
	table.insert(step, (points[i].x - points[1].x) / (points[#points].x - points[1].x))
end

local function binaryFind(radio)
	local a, b = 1, #step
	while a ~= b do
		local c = math.floor((a + b) / 2)
		if step[c] > radio then
			b = c
		elseif step[c] < radio then
			if a == c then
				return a
			else
				a = c
			end
		else
			return c
		end
	end
	return a
end

local function calculatePoint(radio)
	local s = binaryFind(radio)
	if s < #step then
		local t = points[s]
		local x = (radio - step[s]) * (points[s + 1].x - points[s].x)
		local y = (radio - step[s]) * (points[s + 1].y - points[s].y)
		return t.x + x, t.y + y
	else
		local t = points[#step]
		return t.x, t.y
	end
end

function PKSwitch_OnEnter()
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = ModeNameTips[this:GetClientID()], frame = this:GetParent(),
						button = this:GetName(), horizonal = "right" } );
end

function PKSwitch_OnLeave()
	GameTooltipRichText:SetText("",255,255,255);
	GameTooltip:Hide();
end

local startMoveTime = {}
local totalMoveTime = 0.3

function PKSwitch_OnShow()
	startMoveTime[this] = getServerTime()
	this:setUpdateTime(0)
end

function PKSwitch_OnUpdate()
	local now = getServerTime()
	local id = this:GetClientID() + 1
	local startTime = startMoveTime[this]
	local diff = (now.sec - startTime.sec) + (now.usec - startTime.usec) * 0.001
	if diff > totalMoveTime * id / #points then
		this:setUpdateTime(9999)
		this:SetPoint("topleft", "PKBtn", "topleft", points[id].x, points[id].y)
	else
		this:SetPoint("topleft", "PKBtn", "topleft", calculatePoint(diff / totalMoveTime))
	end
end

function PKBtn_OnClick()
	--PKBtn:Hide()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer:isInPVPMap() then
		ShowMidTips( "跨服不允许切换战斗模式" )
		return
	end
	lastShowBlink = nil
	PKBtnUVAnimationTex:Hide()
	for i = 0, Max_MODE_NUM do
		local btn = getglobal("PKSwitchButton"..i)
		if btn then
			this:SetPoint("topleft", "PKBtn", "topleft", 0, 0)
			btn:Show()
		end
	end
end

function PKSwitch_OnLoad()
	this:SetClientString(ModeName[this:GetClientID()])
end

function PKSwitch_OnClick()
	GameActorPVP:changePKMode(this:GetClientID());
	--ChangePKModeTips(this:GetClientID());

end

function EquipFramePKValue_OnEnter()
	SetGameTooltips(this:GetName(), string.format("杀气值：%d\n在线时间越长,杀气值减少越快", GameActorPVP:getPkValue()/10))
end

function PkModeChanged(mode)
	local btn = getglobal("PKBtn")
	btn:SetClientString(ModeName[mode])
	btn:Show()
	btn:SetPoint("topright", "PhotoFrameMP", "bottomleft", 0, -5)
	if mode ~= 5 then
		btn:Enable();
	end
	local tex
	for i = 0, Max_MODE_NUM do
		tex = getglobal("PKBtn_"..i)
		if tex then
			tex:Hide()
		end
		btn = getglobal("PKSwitchButton"..i)
		if btn then
			btn:Hide()
		end
	end
	tex = getglobal("PKBtn_"..mode)
	if tex then
		tex:Show()
	end
end

function PkValueChanged(value)
	local tex
	for i = 0, 2 do
		tex = getglobal("EquipFramePKValueTex"..i)
		if tex then
			tex:Hide()
		end
		tex = getglobal("SparFramePKValueTex"..i)
		if tex then
			tex:Hide()
		end
	end
	tex = getglobal("EquipFramePKValueTex"..GameActorPVP:getAmok(value))
	if tex then
		tex:Show()
	end
	tex = getglobal("SparFramePKValueTex" ..GameActorPVP:getAmok(value))
	if tex then
		tex:Show()
	end
end

local lastShowBlink

function ForcePkMode(mode)
	if mode == nil then
		PKBtnUVAnimationTex:SetUVAnimation(60, true)
		PKBtnUVAnimationTex:Show()
		lastShowBlink = os.clock()
	end
end

function PKBtn_OnUpdate()
	if not lastShowBlink then
		return
	end
	local now = os.clock()
	if lastShowBlink + 3 < now then
		lastShowBlink = nil
		PKBtnUVAnimationTex:Hide()
	end
end

function ChangePKModeTips(pkMode)
	local btn	=	getglobal("PKSwitchButton"..pkMode);
	ShowMidTips("战斗模式切换为["..btn:GetClientString().."]");
	ClearPKTips();
end