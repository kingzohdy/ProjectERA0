local scrollDisplayOrder = {}
local scrollDisplayConstant = {time = 0.8}
local scrollDisplayList = {
	"PworldScoreFrameInfoFightInfo",
	"PworldScoreFrameInfoExtraExp",
	"PworldScoreFrameInfoTitleKill",
	"PworldScoreFrameInfoTitleTime",
	"PworldScoreFrameInfoTitleDeath",
	"PworldScoreFrameInfoTitleDifficulty",
	"PworldScoreFrameInfoTitleFriend",
	"PworldScoreFrameInfoTitleGrade",
	"PworldScoreFrameInfoTitleClan",
	"PworldScoreFrameInfoKillNormal",
	"PworldScoreFrameInfoKillNormalNumber",
	"PworldScoreFrameInfoKillNormalScore",
	"PworldScoreFrameInfoKillJingYing",
	"PworldScoreFrameInfoKillJingYingNumber",
	"PworldScoreFrameInfoKillJingYingScore",
	"PworldScoreFrameInfoKillBoss",
	"PworldScoreFrameInfoKillBossNumber",
	"PworldScoreFrameInfoKillBossScore",
	"PworldScoreFrameInfoTimeScore",
	"PworldScoreFrameInfoDeathScore",
	"PworldScoreFrameInfoDifficulty",
	"PworldScoreFrameInfoDifficultyExp",
	"PworldScoreFrameInfoFriendExp",
	"PworldScoreFrameInfoGrade",
	"PworldScoreFrameInfoGradeExp",
	"PworldScoreFrameInfoClanExp",
	"PworldScoreFrameInfoOffExp",
}
local showTime = 0
local scrollDisplaying
local scoreGrowing
local scoreGrowFinal
local stamping
local stampTime = 0.2

local remainingTime = 15
local lastTick = 0

local selectedCards
local selectedVipCards

local aotuSelect
local aotuVipSelect

local selectedUpdateList
local selectedCardCoseToOpenList
local lastOpenCardTime = 0

local selectedUpdateVipList
local selectedVipCardCoseToOpenList
local lastOpenVipCardTime = 0

local difficultyName = {
	[PworldDifficulty.EASY] = "简单",
	[PworldDifficulty.GENERAL] = "普通",
	[PworldDifficulty.HARD] = "困难",
	[PworldDifficulty.EXPERT] = "专家",
}

--测试
local gradeTex = {
	[PworldScore.D] = {
		x = 849, y = 61, w = 113, h = 124,
		path = "uires\\ZhuJieMian\\2.dds",
	},
	[PworldScore.C] = {
		x = 859, y = 186, w = 108, h = 124,
		path = "uires\\ZhuJieMian\\2.dds",
	},
	[PworldScore.B] = {
		x = 750, y = 186, w = 108, h = 124,
		path = "uires\\ZhuJieMian\\2.dds",
	},
	[PworldScore.A] = {
		x = 615, y = 186, w = 134, h = 124,
		path = "uires\\ZhuJieMian\\2.dds",
	},
	[PworldScore.S] = {
		x = 672, y = 311, w = 89, h = 124,
		path = "uires\\ZhuJieMian\\2.dds",
	},
}
--测试

local gradeScoreGrowTime = {
	[PworldScore.D] = 0.5,
	[PworldScore.C] = 1.0,
	[PworldScore.B] = 1.5,
	[PworldScore.A] = 2.0,
	[PworldScore.S] = 2.5,
}

local gradeName = {
	[PworldScore.D] = "D级",
	[PworldScore.C] = "C级",
	[PworldScore.B] = "B级",
	[PworldScore.A] = "A级",
	[PworldScore.S] = "S级",
}


local function getNextSelecteCard(_, index)
	local idx = PworldScore:GetNextSelecteCard(index)
	if idx < 0 then
		return
	end
	return idx
end

local function SelecteCards()
	return getNextSelecteCard, nil, -1
end

local function UpdateScrollDisplay(during)
	local step = math.floor(scrollDisplayConstant.start + during * scrollDisplayConstant.speed + 0.5)
	for _, ui in ipairs(scrollDisplayOrder) do
		local constant = scrollDisplayList[ui]
		local y = constant.y
		if y >= step then
			break
		end
		if not ui:IsShown() then
			ui:Show()
		end
		ui:SetHeight(math.min(step - y, constant.h))
	end
end

local function UpdateScoreGrow(during)
	local ui
	if scoreGrowing then
		for suffix, num in pairs(scoreGrowFinal) do
			ui = getglobal("PworldScoreFrameInfo"..suffix)
			ui:SetText(math.floor(num / scoreGrowing * during))
		end
	else
		for suffix, num in pairs(scoreGrowFinal) do
			ui = getglobal("PworldScoreFrameInfo"..suffix)
			ui:SetText(num)
		end
	end
end

local function UpdateStamp(during)
	if not PworldScoreFrameInfoGradeTex:IsShown() then
		PworldScoreFrameInfoGradeTex:Show()
	end
	local info=PworldScore.AwardInfo
	local tex = gradeTex[info.Grade]
	PworldScoreFrameInfoGradeTex:SetWidth(tex.w + 4*tex.w * math.max(0, stampTime - during) / stampTime)
	PworldScoreFrameInfoGradeTex:SetHeight(124 + 496 * math.max(0, stampTime - during) / stampTime)
end

local function EnableSelectCards(enable)
	local ui
	for i = 1, 20 do
		ui = getglobal("PworldScoreFrameAwardCardOpen"..i)
		if enable then
			if ui:IsEnable() then
				break
			end
			ui:Enable()
		else
			if not ui:IsEnable() then
				break
			end
			ui:Disable()
		end
	end
end

local function EnableSelectVipCards(enable)
	local ui
	for i = 1, 10 do
		ui = getglobal("PworldScoreFrameAwardVipCardOpen"..i)
		if enable then
			if ui:IsEnable() then
				break
			end
			ui:Enable()
		else
			if not ui:IsEnable() then
				break
			end
			ui:Disable()
		end
	end
end

local function InitCards()
	--local ui
--	for i = 1, 20 do
--		ui = getglobal("PworldScoreFrameAwardCardOpen"..i)
		--ui:Hide()
		--ui = getglobal("PworldScoreFrameAwardCardCloseToOpen"..i)
		--ui:Hide()
--		ui = getglobal("PworldScoreFrameAwardCardClose"..i)
--		ui:Show()
--	end
	EnableSelectCards(true)
	EnableSelectVipCards(true)
end

local function OpenPworldScoreCard(i, imm)
	local ui
	--ui = getglobal("PworldScoreFrameAwardCardClose"..i)
	--ui:Hide()
	local card = PworldScore.AwardCards[i - 1]
	local item = card.item
	local ItemDef = item:getItemDef()
	local IconPath = DEFAULT_ITEM_PATH
	if ItemDef then
		IconPath = GetItemEquipIconPath()..ItemDef.IconID..".tga"
	end
	-- 如果没找到物品的图标则使用默认图标显示
	if not IsInExistence(IconPath) then
		IconPath = DEFAULT_ITEM_PATH
	end
	local name = "PworldScoreFrameAwardCardOpen"..i.."Item"
	local icon = getglobal(name.."Icon")
	icon:SetTexture(IconPath)
	icon:SetTexRelUV(1, 1)
	local num = item:getNum()
	local font = getglobal(name.."Font")
	if num > 1 then
		font:Show()
		font:SetText(num)
	else
		font:Hide()
	end
	SetEquipImportantEffectUV(ItemDef, name.."UVAnimationTex")
	local owner = getglobal("PworldScoreFrameAwardCardOpen"..i.."Name")
	owner:SetText(card.name)

	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer ~= nil and card.name == mainplayer:getName() then
		local TeXiao = getglobal("PworldScoreFrameAwardCardOpen"..i.."ItemTexiao")
		TeXiao:Show()
	end

	if imm then
		local cardNum=1
		while cardNum<21 do
			local ShowCardAnim = getglobal("PworldScoreFrameAwardCardOpen"..cardNum.."ItemHightAnim")
			local ShowCardPart = getglobal("PworldScoreFrameAwardCardOpen"..cardNum.."ItemHightCenter")
			--ShowCardAnim:SetPoint("center", "PworldScoreFrameAwardCardOpen"..i.."Item", "center", 0, -23)
			--ShowCardPart:SetTexUV(615,311,56,4)
			--ShowCardPart:SetSize(50,4)
			ShowCardAnim:Hide()
			ShowCardPart:Hide()			
			cardNum=cardNum+1
		end
	else	
		--tex:SetTextOffsetY(10*(os.clock-buttonAnimStartTime) / (50 * 4 * 0.001))
		--local during=os.clock-buttonAnimStartTime		
		--tex:SetTextOffsetY(-10*during)
		--tex:SetHeight(54-during*0.1*54/64)
		--anim:SetUVAnimation(50, false)
		table.insert(selectedCardCoseToOpenList, i)
		
		ui = getglobal("PworldScoreFrameAwardCardOpen"..i)
		timer(function()
			if not PworldScoreFrame:IsShown() then
				return
			end
			local name = ui:GetName()
			local suffix = {
				"Name",
				"Tex",
				"ItemIcon",
				"ItemFont",
				"ItemFrame",
				"ItemUVAnimationTex",
			}
			--anim:Hide()
			ui:Show()
			ui:SetBlendAlpha(1)
			--local now = os.clock()
			--for _, str in ipairs(suffix) do
			--	local ui = getglobal(name..str)
			--	ui:SetBlendAlpha(0)
			--end
			--while PworldScoreFrame:IsShown() do
			--	coroutine.yield(50 * 0.001)
			--	local alpha = (os.clock() - now) / (50 * 4 * 0.001)
			--	if alpha > 1 then
			--	alpha = 1
			--	end
			--	for _, str in ipairs(suffix) do
			--		local ui = getglobal(name..str)
			--		ui:SetBlendAlpha(alpha)
			--	end
			--	if alpha == 1 then
			--		break
			--	end
			--end
		end, 50 * 12 * 0.001)
	end
end

local function UpdatePworldScoreCard(i, finish)
	if finish then
		lastOpenCardTime = os.clock() + 2
		table.insert(selectedUpdateList, i)
	else
		OpenPworldScoreCard(i)
	end
end

local function AutoSelectCard()
	aotuSelect = true
	if not PworldScore:IsSelected() then
		local s
		local n = 0
		for i = 1, 20 do
			local id = i - 1
			if not PworldScore:IsCardSelected(id) then
				n = n + 1
				if math.random() < 1 / n then
					s = id
				end
			end
		end
		EnableSelectCards(false)
		PworldScore:SelectCard(s)
	end
end

function PworldScoreRewardCard_OnClick()
	if not string.find( this:GetName(), "Vip", 1, true ) then
		local parentName = this:GetParent()
		local parentNameReverse=string.reverse(parentName)
		local findNumberStringReverse = string.match(parentNameReverse,"%d+")
		local findNumberString = string.reverse(findNumberStringReverse)
		local id = tonumber(findNumberString)
		if not PworldScore:IsCardSelected(id-1) then
			EnableSelectCards(false)
			PworldScore:SelectCard(id-1)
		end
	else
		if not VipManager:isOwnVipFunc( STAR_RESULT_PWORLD_CARD_TOLUA ) then
			return ShowMidTips("拥有水晶徽章特权才可翻牌");
		end
		local parentName = this:GetParent()
		local parentNameReverse=string.reverse(parentName)
		local findNumberStringReverse = string.match(parentNameReverse,"%d+")
		local findNumberString = string.reverse(findNumberStringReverse)
		local id = tonumber(findNumberString)
		if not PworldScore:IsVipCardSelected(id-1) then
			EnableSelectVipCards(false)
			PworldScore:SelectVipCard(id-1)
		end
	end
end

function PworldScoreRewardCard_OnEnter()
	local parent = getglobal(this:GetParent())
	local id = parent:GetClientID() - 1
	if not string.find( this:GetName(), "Vip", 1, true ) then
		if PworldScore:IsCardSelected(id) then
			local item = PworldScore.AwardCards[id].item
			ShowItemTips(LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item, nil, this:GetName())
		end
	else
		if PworldScore:IsVipCardSelected(id) then
			local item = PworldScore.VipAwardCards[id].item
			ShowItemTips(LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item, nil, this:GetName())
		else
			SetGameTooltips( this:GetName(),  "拥有水晶徽章特权才可翻牌" );
		end
	end
end

function PworldScoreRewardCard_OnLeave()
	ItemButton_OnLeave();
	GameTooltipOnLeave();
end

function PworldScoreFrame_OnLoad()
	this:RegisterEvent("GE_LEAVE_PWORLD")
	PworldScoreFrameTex:SetColor(0, 0, 0)
	PworldScoreFrameTex:SetBlendAlpha(40)
	PworldScoreFrame:setUpdateTime(0)
	local t = {}
	for _, name in ipairs(scrollDisplayList) do
		local ui = getglobal(name)
		t[ui] = {
			x = ui:GetAnchorOffsetX(), y = ui:GetAnchorOffsetY(), w = ui:GetWidth(), h = ui:GetHeight()
		}
		table.insert(scrollDisplayOrder, ui)
	end
	scrollDisplayList = t
	table.sort(scrollDisplayOrder, function(a, b)
		return scrollDisplayList[a].y < scrollDisplayList[b].y
	end)
	scrollDisplayConstant.start = scrollDisplayList[scrollDisplayOrder[1]].y
	scrollDisplayConstant.speed = (scrollDisplayList[scrollDisplayOrder[#scrollDisplayOrder]].y + scrollDisplayList[scrollDisplayOrder[#scrollDisplayOrder]].h - scrollDisplayList[scrollDisplayOrder[1]].y) / scrollDisplayConstant.time
end

function PworldScoreFrame_OnEvent()
	if arg1 == "GE_LEAVE_PWORLD" then
		if PworldScoreFrame:IsShown() then
			PworldScoreFrame:Hide()
		end
	end
end

function PworldScoreFrame_OnShow()
	--FrameMgr:EnableAccelerator(false)
	--PworldScoreFrameAwardClose:Hide()
	PworldScoreFrameAwardMsg:Hide()
	PworldScoreFrameAwardMsgTip:Hide()
	InitPworldScoreFrame()
end

function PworldScoreFrame_OnEsc()
	if not PworldScoreFrameAwardTimer:IsShown() then
		PworldScoreFrame:Hide()
	end
end

function PworldScoreFrame_OnUpdate()
	local now = os.clock()
	local during = now - showTime
	local lastduring = lastTick
	lastTick = during
	if scrollDisplaying then
		if during > scrollDisplayConstant.time then
			scrollDisplaying = nil
		end
		UpdateScrollDisplay(during)
	end
	if scoreGrowing then
		if during > scoreGrowing then
			scoreGrowing = nil
			stamping = during
		end
		UpdateScoreGrow(during)
	end
	if stamping then
		local during = during - stamping
		if during > stampTime then
			stamping = nil
		end
		UpdateStamp(during)
	end
	if remainingTime >= lastduring and PworldScoreFrameAwardTimer:IsShown() then
		local tick = math.floor(during)
		if math.floor(lastduring) ~= tick then
			local remain = remainingTime - tick
			if remain > 0 then
				PworldScoreFrameAwardTimer:SetText(string.format("剩余选择时间：%2d秒", remain))
			else
				PworldScoreFrameAwardTimer:Hide()
				AutoSelectCard()
				if VipManager:isOwnVipFunc( STAR_RESULT_PWORLD_CARD_TOLUA ) then
					AutoSelectVipCard();
				end
			end
		end
	end
	if selectedUpdateList and #selectedUpdateList > 0 and lastOpenCardTime <= now then
		for _, i in ipairs(selectedUpdateList) do
			OpenPworldScoreCard(i, true)
		end
		selectedUpdateList = {}
	end
	if selectedCardCoseToOpenList and #selectedCardCoseToOpenList>0 then
		for _, i in ipairs(selectedCardCoseToOpenList) do
			PworldScoreRewardCardCloseToOpen_OnUpdate(i)
		end
	end
	--vip
	if selectedUpdateVipList and #selectedUpdateVipList > 0 and lastOpenVipCardTime <= now then
		for _, i in ipairs(selectedUpdateVipList) do
			OpenPworldScoreVipCard(i, true)
		end
		selectedUpdateVipList = {}
	end
	if selectedVipCardCoseToOpenList and #selectedVipCardCoseToOpenList>0 then
		for _, i in ipairs(selectedVipCardCoseToOpenList) do
			PworldScoreRewardVipCardCloseToOpen_OnUpdate(i)
		end
	end
end


function InitPworldScoreFrame()
	showTime = os.clock()
	lastTick = 0
	selectedCards = {}
	selectedUpdateList = {}
	selectedCardCoseToOpenList={}
	selectedVipCards = {}
	selectedUpdateVipList = {}
	selectedVipCardCoseToOpenList={}
	aotuSelect = nil
	aotuVipSelect = nil
	for _, ui in ipairs(scrollDisplayOrder) do
		ui:Hide()
	end
	PworldScoreFrameInfoGradeTex:Hide()
	
	local info = PworldScore.AwardInfo
	local hasDifficulty = false
	for i = 1, PworldDifficulty.MAXNUM - 1 do
		if PworldDifficulty:getDifficultyPworldId(info.DiffPworldID, i) ~= 0 then
			hasDifficulty = true
			break
		end
	end
	scoreGrowing = gradeScoreGrowTime[info.Grade]
	scrollDisplaying = true
	scoreGrowFinal = {Total = info.Total, Money = info.Money, Exp = info.Exp}
	
	local tex = gradeTex[info.Grade]
	PworldScoreFrameInfoGradeTex:SetTexture(tex.path)
	PworldScoreFrameInfoGradeTex:SetTexUV(tex.x, tex.y, tex.w, tex.h)
	PworldScoreFrameInfoGradeTex:SetSize(tex.w,tex.h)
	
	local cardNum=1
	while cardNum<21 do
		local cardIcon = getglobal("PworldScoreFrameAwardCardOpen"..cardNum.."ItemIcon")
		cardIcon:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga")
		local cardFont = getglobal("PworldScoreFrameAwardCardOpen"..cardNum.."ItemFont")
		cardFont:SetText("")
		local cardUVAnimationTex = getglobal("PworldScoreFrameAwardCardOpen"..cardNum.."ItemUVAnimationTex")
		cardUVAnimationTex:Hide()
		local cardName = getglobal("PworldScoreFrameAwardCardOpen"..cardNum.."Name")
		cardName:SetText("")
		local cardAnim = getglobal("PworldScoreFrameAwardCardOpen"..cardNum.."ItemHightAnim")
		cardAnim:Show()
		local cardPart =	getglobal("PworldScoreFrameAwardCardOpen"..cardNum.."ItemHightCenter")	
		cardAnim:SetPoint("center", "PworldScoreFrameAwardCardOpen"..cardNum.."Item", "center", 0, 23)
		cardPart:Show()
		cardPart:SetTexUV(615,311,56,56)
		cardPart:SetSize(50,50)
		local TeXiao = getglobal("PworldScoreFrameAwardCardOpen"..cardNum.."ItemTexiao")
		TeXiao:Hide()
		cardNum=cardNum+1
	end

	for i=1, 10 do
		local cardIcon = getglobal("PworldScoreFrameAwardVipCardOpen"..i.."ItemIcon")
		cardIcon:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga")
		local cardFont = getglobal("PworldScoreFrameAwardVipCardOpen"..i.."ItemFont")
		cardFont:SetText("")
		local cardUVAnimationTex = getglobal("PworldScoreFrameAwardVipCardOpen"..i.."ItemUVAnimationTex")
		cardUVAnimationTex:Hide()
		local cardName = getglobal("PworldScoreFrameAwardVipCardOpen"..i.."Name")
		cardName:SetText("")
		local cardAnim = getglobal("PworldScoreFrameAwardVipCardOpen"..i.."ItemHightAnim")
		cardAnim:Show()
		local cardPart =	getglobal("PworldScoreFrameAwardVipCardOpen"..i.."ItemHightCenter")	
		cardAnim:SetPoint("center", "PworldScoreFrameAwardVipCardOpen"..i.."Item", "center", 0, 23)
		cardPart:Show()
		cardPart:SetTexUV(838,49,56,56)
		cardPart:SetSize(50,50)
		local TeXiao = getglobal("PworldScoreFrameAwardVipCardOpen"..i.."ItemTexiao")
		TeXiao:Hide()
	end

	PworldScoreFrameInfoTotal:SetText(0)
	PworldScoreFrameInfoMoney:SetText(0)
	PworldScoreFrameInfoExp:SetText(0)
	--测试	奖励物品动态实现
	PworldScoreFrameAwardTitle:SetText(string.format("奖励物品：（%s组合）",
		gradeName[info.Grade]))
	if hasDifficulty then
		local difficulty = PworldDifficulty:getDifficulty(info.DiffPworldID, info.PworldID)
		PworldScoreFrameTitle:SetText(string.format("%s(%s) 通关！",
			PworldDifficulty:getDiffPworldName(info.DiffPworldID), difficultyName[difficulty]))
		PworldScoreFrameInfoDifficulty:SetText(difficultyName[difficulty])
	else
		PworldScoreFrameTitle:SetText(string.format("%s 通关！",
			PworldDifficulty:getDiffPworldName(info.DiffPworldID)))
		PworldScoreFrameInfoDifficulty:SetText("动态")
	end
	PworldScoreFrameInfoKillNormalNumber:SetText(string.format(" ×%d",info.NormalNum))
	PworldScoreFrameInfoKillNormalScore:SetText(string.format("%d分",info.Normal))
	PworldScoreFrameInfoKillJingYingNumber:SetText(string.format(" ×%d",info.JingNum))
	PworldScoreFrameInfoKillJingYingScore:SetText(string.format("%d分",info.JingYing))
	PworldScoreFrameInfoKillBossNumber:SetText(string.format(" ×%d",info.BossNum))
	PworldScoreFrameInfoKillBossScore:SetText(string.format("%d分",info.Boss))
	
	if info.OffExp == 0 then
		PworldScoreFrameInfoOffExp:SetText("")
	else
		PworldScoreFrameInfoOffExp:SetText(string.format("缺失活动补偿经验： \n+%d",info.OffExp))
	end
	if info.TimeCost >= 3600 then
		PworldScoreFrameInfoTimeText:SetText(string.format("%d小时%d分%d秒",
			math.floor(info.TimeCost / 3600), math.floor((info.TimeCost % 3600) / 60), info.TimeCost % 60))
	elseif info.TimeCost >= 60 then
		PworldScoreFrameInfoTimeText:SetText(string.format("%d分%d秒",
			math.floor(info.TimeCost / 60), info.TimeCost % 60))
	else
		PworldScoreFrameInfoTimeText:SetText(string.format("%d秒",
			info.TimeCost))
	end
	PworldScoreFrameInfoTimeScore:SetText(string.format("%d分", info.Time))
	PworldScoreFrameInfoDeathNumber:SetText(string.format(" ×%d", info.DeathNum))
	if info.Death >0 then
		PworldScoreFrameInfoDeathScore:SetText(string.format("- %d分", info.Death))
	else	
		PworldScoreFrameInfoDeathScore:SetText(string.format("  %d分", info.Death))		
	end
	PworldScoreFrameInfoDifficultyExp:SetText(string.format("+ %d经验", info.DiffExp))
	PworldScoreFrameInfoFriendExp:SetText(string.format("+ %d经验", info.FriendExp))
	PworldScoreFrameInfoGrade:SetText(gradeName[info.Grade])
	PworldScoreFrameInfoGradeExp:SetText(string.format("+ %d经验", info.GradeExp))
	PworldScoreFrameInfoClanExp:SetText(string.format("+ %d经验", info.ClanExp))
	PworldScoreFrameAwardTimer:Show()
	PworldScoreFrameAwardTimer:SetText(string.format("剩余选择时间：%d秒", remainingTime))
	InitCards()
end

function OpenPworldScoreFrame()
	PworldScoreFrame:Show()
end

function UpdatePworldScoreCards(finish)
	if not PworldScoreFrame:IsShown() then
		return
	end
	local newcards = {}
	for i in SelecteCards() do
		if not selectedCards[i] then
			selectedCards[i] = true
			table.insert(newcards, i)			
		end
	end
	for _, i in ipairs(newcards) do
		UpdatePworldScoreCard(i + 1, finish)
	end
end

function UpdatePworldScoreCardMsg(msg)
	if not PworldScoreFrameAwardMsg:IsShown() then
		PworldScoreFrameAwardMsg:Show()
	end
	if not PworldScoreFrameAwardMsgTip:IsShown() then
		PworldScoreFrameAwardMsgTip:Show()
	end
	PworldScoreFrameAwardMsg:SetText(msg)
end

function SelectPworldScoreCardResult(result)
	if result then
		if PworldScore:IsSelected() then
			--[[local i = PworldScore:GetSelected()
			if i >= 0 then
				local TeXiao = getglobal("PworldScoreFrameAwardCardOpen"..(i + 1).."ItemTexiao")
				TeXiao:Show()
			end--]]
			EnableSelectCards(false)
			if VipManager:isOwnVipFunc( STAR_RESULT_PWORLD_CARD_TOLUA ) and PworldScore:GetVipSelected() < 0 then
				return;
			end
			--PworldScoreFrameAwardClose:Show()
			PworldScoreFrameAwardTimer:Hide()
		end
	else
		if not PworldScore:IsSelected() then
			EnableSelectCards(true)
		end
		if aotuSelect then
			AutoSelectCard()
		end
	end
end
function PworldScoreRewardCardCloseToOpen_OnUpdate(i)--这里有些问题，按帧更新来表现动画，会导致低帧时动画速度很慢
	local texAnim = getglobal("PworldScoreFrameAwardCardOpen"..i.."ItemHightAnim")
	local texAnimPart =	getglobal("PworldScoreFrameAwardCardOpen"..i.."ItemHightCenter")
	texAnim:Show()
	texAnimPart:Show()
	local newOffsetY = texAnim:GetAnchorOffsetY() - 1
	if	newOffsetY + 23 > 0	then
		texAnim:SetPoint("center", "PworldScoreFrameAwardCardOpen"..i.."Item", "center", 0, newOffsetY)
		local hight = 50 - (23 - newOffsetY)
		texAnimPart:SetTexUV(615,311,56,hight*28/25)
		texAnimPart:SetSize(50,hight)
	end
	
	if	newOffsetY + 22 < 0 then
		texAnim:Hide()
		texAnimPart:Hide()
		--table.remove(selectedCardCoseToOpenList,i)
		--这里remove错了索引，由于正在迭代中，最好也别进行remove
	end
end

-----------------------------vip部分-------------------------------------

function getNextVipSelecteCard(_, index)
	local idx = PworldScore:GetNextVipSelectCard(index)
	if idx < 0 then
		return
	end
	return idx
end

function SelecteVipCards()
	return getNextVipSelecteCard, nil, -1
end

function OpenPworldScoreVipCard(i, imm)
	local ui
	local card = PworldScore.VipAwardCards[i - 1]
	local item = card.item
	local ItemDef = item:getItemDef()
	local IconPath = DEFAULT_ITEM_PATH
	if ItemDef then
		IconPath = GetItemEquipIconPath()..ItemDef.IconID..".tga"
	end
	-- 如果没找到物品的图标则使用默认图标显示
	if not IsInExistence(IconPath) then
		IconPath = DEFAULT_ITEM_PATH
	end
	local name = "PworldScoreFrameAwardVipCardOpen"..i.."Item"
	local icon = getglobal(name.."Icon")
	icon:SetTexture(IconPath)
	icon:SetTexRelUV(1, 1)
	local num = item:getNum()
	local font = getglobal(name.."Font")
	if num > 1 then
		font:Show()
		font:SetText(num)
	else
		font:Hide()
	end
	SetEquipImportantEffectUV(ItemDef, name.."UVAnimationTex")
	local owner = getglobal("PworldScoreFrameAwardVipCardOpen"..i.."Name")
	owner:SetText(card.name)

	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer ~= nil and card.name == mainplayer:getName() then
		local TeXiao = getglobal("PworldScoreFrameAwardVipCardOpen"..i.."ItemTexiao")
		TeXiao:Show()
	end

	if imm then
		local cardNum=1
		while cardNum<11 do
			local ShowCardAnim = getglobal("PworldScoreFrameAwardVipCardOpen"..cardNum.."ItemHightAnim")
			local ShowCardPart = getglobal("PworldScoreFrameAwardVipCardOpen"..cardNum.."ItemHightCenter")
			ShowCardAnim:Hide()
			ShowCardPart:Hide()			
			cardNum=cardNum+1
		end
	else	
		table.insert(selectedVipCardCoseToOpenList, i)
		
		ui = getglobal("PworldScoreFrameAwardVipCardOpen"..i)
		timer(function()
			if not PworldScoreFrame:IsShown() then
				return
			end
			local name = ui:GetName()
			local suffix = {
				"Name",
				"Tex",
				"ItemIcon",
				"ItemFont",
				"ItemFrame",
				"ItemUVAnimationTex",
			}
			ui:Show()
			ui:SetBlendAlpha(1)
		end, 50 * 12 * 0.001)
	end
end

function UpdatePworldScoreVipCard(i, finish)
	if finish then
		lastOpenVipCardTime = os.clock() + 2
		table.insert(selectedUpdateVipList, i)
	else
		OpenPworldScoreVipCard(i)
	end
end

function AutoSelectVipCard()
	aotuVipSelect = true
	if not PworldScore:IsVipSelected() then
		local s
		local n = 0
		for i = 1, 10 do
			local id = i - 1
			if not PworldScore:IsVipCardSelected(id) then
				n = n + 1
				if math.random() < 1 / n then
					s = id
				end
			end
		end
		EnableSelectVipCards(false)
		PworldScore:SelectVipCard(s)
	end
end

function PworldScoreRewardVipCardCloseToOpen_OnUpdate(i)
	local texAnim = getglobal("PworldScoreFrameAwardVipCardOpen"..i.."ItemHightAnim")
	local texAnimPart =	getglobal("PworldScoreFrameAwardVipCardOpen"..i.."ItemHightCenter")
	texAnim:Show()
	texAnimPart:Show()
	local newOffsetY = texAnim:GetAnchorOffsetY() - 1
	if	newOffsetY + 23 > 0	then
		texAnim:SetPoint("center", "PworldScoreFrameAwardVipCardOpen"..i.."Item", "center", 0, newOffsetY)
		local hight = 50 - (23 - newOffsetY)
		texAnimPart:SetTexUV(838,49,56,hight*28/25)
		texAnimPart:SetSize(50,hight)
	end
	
	if	newOffsetY + 22 < 0 then
		texAnim:Hide()
		texAnimPart:Hide()
	end
end

function PworldScoreFrame_OnHide()
	if not PworldScore:IsSelected() then
		AutoSelectCard()
	end

	if VipManager:isOwnVipFunc( STAR_RESULT_PWORLD_CARD_TOLUA ) and not PworldScore:IsVipSelected() then
		AutoSelectVipCard();
	end
end

function UpdatePworldScoreVipCards(finish)
	if not PworldScoreFrame:IsShown() then
		return
	end
	local newcards = {}
	for i in SelecteVipCards() do
		if not selectedVipCards[i] then
			selectedVipCards[i] = true
			table.insert(newcards, i)			
		end
	end
	for _, i in ipairs(newcards) do
		UpdatePworldScoreVipCard(i + 1, finish)
	end
end

function SelectPworldScoreVipCardResult(result)
	if result then
		if PworldScore:IsVipSelected() then
			--[[local i = PworldScore:GetVipSelected()
			if i >= 0 then
				local TeXiao = getglobal("PworldScoreFrameAwardVipCardOpen"..(i + 1).."ItemTexiao")
				TeXiao:Show()
			end--]]
			EnableSelectVipCards(false)
			if PworldScore:GetSelected() < 0 then
				return;
			end
			--PworldScoreFrameAwardClose:Show()
			PworldScoreFrameAwardTimer:Hide()
		end
	else
		if not PworldScore:IsVipSelected() then
			EnableSelectVipCards(true)
		end
		if aotuVipSelect then
			AutoSelectVipCard()
		end
	end
end