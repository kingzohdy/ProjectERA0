local lastDifficultyBtn
local lastDifficulty
local lastDiffPworldId
--添加一个用以记录的pworldId，暂时只用于迷之塔
local lastPworldId
local npcId = 0
local difficultyMap = {
	[PworldDifficulty.EASY] = {
		suffix = "Easy",
		name = {
			x = 112, y = 636, w = 44, h = 24,
			path = "uires\\ZhuJieMian\\2.dds",
		},
		text = "简    单",
	},
	[PworldDifficulty.GENERAL] = {
		suffix = "General",
		name = {
			x = 157, y = 636, w = 44, h = 24,
			path = "uires\\ZhuJieMian\\2.dds",
		},
		text = "普    通",
	},
	[PworldDifficulty.HARD] = {
		suffix = "Hard",
		name = {
			x = 164, y = 611, w = 44, h = 24,
			path = "uires\\ZhuJieMian\\2.dds",
		},
		text = "困    难",
	},
	[PworldDifficulty.EXPERT] = {
		suffix = "Expert",
		name = {
			x = 786, y = 878, w = 44, h = 24,
			path = "uires\\ZhuJieMian\\2.dds",
		},
		text = "专    家",
	},
}

local difficultyTips = {
	[PworldDifficulty.EASY]		= "简单难度副本，今日可进入次数为：%d",
	[PworldDifficulty.GENERAL]	= "普通难度副本，今日可进入次数为：%d",
	[PworldDifficulty.HARD]		= "困难难度副本，今日可进入次数为：%d",
	[PworldDifficulty.EXPERT]	= "专家难度副本，今日可进入次数为：%d",
	
	default = "活动副本，今日可进入次数为：%d",
}

local difficultyTipsNolimit = {
	[PworldDifficulty.EASY]		= "简单难度副本，不限进入次数",
	[PworldDifficulty.GENERAL]	= "普通难度副本，不限进入次数",
	[PworldDifficulty.HARD]		= "困难难度副本，不限进入次数",
	[PworldDifficulty.EXPERT]	= "专家难度副本，不限进入次数",
	
	default = "活动副本，不限进入次数",
}

-- 掉落列表tips
function PworldDifficultyReward_OnEnter()
	local id = this:GetClientUserData(0)
	if id and id ~= 0 then
		ShowItemTips(LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, getItemDef(id))
	end
end
function PworldDifficultyReward_OnLeave()
	ItemButton_OnLeave()
end

function PworldDifficultyFrameLock_OnEnter()	
	--这里迷之塔特殊处理下,
	if IsMiZhiTaPworld( PworldDifficulty:getDifficultyPworldId( lastDiffPworldId, 0 ) ) then return end
	
	local str
	local difficulty = lastDifficulty or (lastDifficultyBtn and lastDifficultyBtn:GetClientUserData(0))
	if difficulty then
		local data = PworldLimitInfo:getPworldData(PworldDifficulty:getDifficultyPworldId(lastDiffPworldId, difficulty))
		if data.freeTimeTotal == 0 then
			str = difficultyTipsNolimit[difficulty] or ""
		else
			str = string.format(difficultyTips[difficulty] or "", data.freeTimeTotal - data.freeTimeUse)
		end
	else
		local data = PworldLimitInfo:getPworldData(PworldDifficulty:getDifficultyPworldId(lastDiffPworldId, 0))
		if data.freeTimeTotal == 0 then
			str = difficultyTipsNolimit.default or ""
		else
			str = string.format(difficultyTips.default or "", data.freeTimeTotal - data.freeTimeUse)
		end
	end
	
	if str == "" then return end
	
	SetGameTooltips(this:GetName(), str)
end

function PworldDifficultyFrameLock_OnLeave()
	GameTooltipRichText:SetText("",255,255,255)
	GameTooltip:Hide()
end

function PworldDifficultySelect_OnEnter()
	local str
	local difficulty = this:GetClientUserData(0)
	local data = PworldLimitInfo:getPworldData(PworldDifficulty:getDifficultyPworldId(lastDiffPworldId, difficulty))
	if data.freeTimeTotal == 0 then
		str = difficultyTipsNolimit[difficulty] or ""
	else
		str = string.format(difficultyTips[difficulty] or "", data.freeTimeTotal - data.freeTimeUse)
	end
	SetGameTooltips(this:GetName(), str)
end

function PworldDifficultySelect_OnLeave()
	GameTooltipRichText:SetText("",255,255,255)
	GameTooltip:Hide()
end

-- 点击某个难度选择按钮
function PworldDifficultySelect_OnClick()
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "消耗活力值" then
		MessageBoxFrame:Hide();
	end
	if not lastDifficultyBtn or lastDifficultyBtn:GetName() ~= this:GetName() then
		SelectPworldDifficultyBtn(this)
	end
end

-- 点击进入副本
function PworldDifficultyFrame_OnEnter()
	local difficulty = lastDifficulty or (lastDifficultyBtn and lastDifficultyBtn:GetClientUserData(0) or 0)
	local pworldId = PworldDifficulty:getDifficultyPworldId(lastDiffPworldId, difficulty)
	--迷之塔是没有难度类型的，而且这里pworldId必须给定值，以便服务器判断
	if IsMiZhiTaPworld(PworldDifficulty:getDifficultyPworldId(lastDiffPworldId, 0)) then
		pworldId = lastPworldId ~= nil and lastPworldId or 321
	end
	local PworldDef = getPworldDef( pworldId );
	local mainplayer = ActorMgr:getMainPlayer();
	--是队长												或者  非组队人员,这里当做队长处理
	if ( mainplayer:isInTeam() and mainplayer:isCaptain() ) or not mainplayer:isInTeam() then
		if AutoPworldManager:getPworldState() > 0 then
			PworldDifficultyFrame:Hide();
			PworldDifficulty:enterPworld(npcId, lastDiffPworldId, pworldId);
			return;
		end

		local teamInfo = TeamManager:getTeamInfo();
		if PworldDef.AutoPworldNum > 1 and teamInfo.MemberNum < PworldDef.AutoPworldNum then
			ShowMidTips( "队伍人数不足" .. PworldDef.AutoPworldNum .. "人，无法开启副本" );
			return;
		end

		if PworldDef.EnterUseVig <= 0 then
			PworldDifficultyFrame:Hide();
			PworldDifficulty:enterPworld(npcId, lastDiffPworldId, pworldId);
			return;
		end
		if mainplayer:getVigor() < PworldDef.EnterUseVig then
			ShowMidTips( "您的活力值不足，无法进入该副本" );
		else
			MessageBox( "活力值消耗提示", "进入需要消耗"..PworldDef.EnterUseVig.."点活力值。" )
			MessageBoxFrame:SetClientString( "消耗活力值" );
			MessageBoxFrame:SetClientUserData( 0, npcId );
			MessageBoxFrame:SetClientUserData( 1, lastDiffPworldId );
			MessageBoxFrame:SetClientUserData( 2, pworldId );
		end
	--队员直接发协议给服务器处理
	elseif mainplayer:isInTeam() and not mainplayer:isCaptain() then
		PworldDifficultyFrame:Hide();
		PworldDifficulty:enterPworld(npcId, lastDiffPworldId, pworldId);
	end
end
function EnterDifficultyPworld()
	local NPCId = MessageBoxFrame:GetClientUserData( 0 );
	local DiffPworldId = MessageBoxFrame:GetClientUserData( 1 );
	local PworldId = MessageBoxFrame:GetClientUserData( 2 );
	PworldDifficultyFrame:Hide();
	PworldDifficulty:enterPworld(NPCId, DiffPworldId, PworldId);
end

-- 副本难度选择面板载入
function PworldDifficultyFrame_OnLoad()
	for difficulty, attrib in pairs(difficultyMap) do
		local button = getglobal("PworldDifficultyFrameSelect"..attrib.suffix)
		button:SetClientUserData(0, difficulty)
		button = getglobal("PworldDifficultyFrameSelect"..attrib.suffix.."Btn")
		button:SetClientUserData(0, difficulty)
		button:SetText(attrib.text)
	end
end

-- 副本难度选择面板显示
function PworldDifficultyFrame_OnShow()
	if PworldDifficultyTipsFrame:IsShown() then
		PworldDifficultyTipsFrame:Hide()
	end
	util.ShowInCenter(this)
	PworldDifficultyFrameName:SetText(PworldDifficulty:getDiffPworldName(lastDiffPworldId))
	PworldDifficultyFrameText:SetText(PworldDifficulty:getDiffPworldDesc(lastDiffPworldId))
	for difficulty, attrib in pairs(difficultyMap) do
		local button = getglobal("PworldDifficultyFrameSelect"..attrib.suffix)
		local id = PworldDifficulty:getDifficultyPworldId(lastDiffPworldId, difficulty)
		if id == 0 then
			button:Hide()
		else
			button:Show()
			button = getglobal("PworldDifficultyFrameSelect"..attrib.suffix.."Btn")
			if PworldDifficulty:isPworldLimit(lastDiffPworldId, difficulty) then
				button:Disable()
				button:setInputTransparent(true)
			else
				button:Enable()
				button:setInputTransparent(false)
			end
		end
	end
	if lastDifficultyBtn and not lastDifficultyBtn:IsEnable() then
		local t = {}
		for difficulty in pairs(difficultyMap) do
			table.insert(t, difficulty)
		end
		table.sort(t)
		for _, difficulty in ipairs(t) do
			local id = PworldDifficulty:getDifficultyPworldId(lastDiffPworldId, difficulty)
			if id ~= 0 then
				if not PworldDifficulty:isPworldLimit(lastDiffPworldId, difficulty) then
					local button = getglobal("PworldDifficultyFrameSelect"..difficultyMap[difficulty].suffix)
					SelectPworldDifficultyBtn(button)
					break
				end
			end
		end
	end
end
function PworldDifficultyFrame_OnHide()
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "消耗活力值" then
		MessageBoxFrame:Hide();
	end
end

-- 部分队员条件不符提示继续进入副本
function PworldDifficultyTipsFrame_OnContinue()
	PworldDifficultyTipsFrame:Hide()
	local difficulty = lastDifficulty or (lastDifficultyBtn and lastDifficultyBtn:GetClientUserData(0) or 0)
	local pworldId = PworldDifficulty:getDifficultyPworldId(lastDiffPworldId, difficulty)
	PworldDifficulty:forceEnterPworld(npcId, lastDiffPworldId, pworldId)
end

-- 部分队员条件不符提示面板显示
function PworldDifficultyTipsFrame_OnShow()
	if PworldDifficultyFrame:IsShown() then
		PworldDifficultyFrame:Hide()
	end
	if PworldPortalFrame:IsShown() then
		PworldPortalFrame:Hide();
	end

	util.ShowInCenter(this)
	local total = PworldDifficulty.tipNum
	for i = 1, 6 do
		local name = "PworldDifficultyTipsFrameLine"..i
		local left = getglobal(name.."Left")
		local right = getglobal(name.."Right")
		if i > total then
			left:Hide()
			right:Hide()
		else
			left:Show()
			right:Show()
			local t = PworldDifficulty.tips[i - 1]
			left:SetText(t.RoleName)
			right:SetText(t.Msg)
		end
	end
end

function SelectPworldDifficulty(difficulty)
	local name = difficultyMap[difficulty].name
	PworldDifficultyFrameDifficulty:SetTexture(name.path)
	PworldDifficultyFrameDifficulty:SetTexUV(name.x, name.y, name.w, name.h)
	UpdatePworldDifficulty(difficulty)
end


function SelectPworldDifficultyBtn(this)
	if lastDifficultyBtn then
		lastDifficultyBtn:DisChecked()
	end
	lastDifficultyBtn = this
	if this then
		this:Checked()
		SelectPworldDifficulty(this:GetClientUserData(0))
	end
end

function ShowPworldDifficultyType(optional, difficulty)
	if optional then
		PworldDifficultyFrameDifficultyDyn:Hide()
		PworldDifficultyFrameDifficulty:Show()
		PworldDifficultyFrameLock:Hide()
		PworldDifficultyFrameSelect:Show()
	else
		PworldDifficultyFrameSelect:Hide()
		PworldDifficultyFrameLock:Show()
		if difficulty then
			PworldDifficultyFrameDifficultyDyn:Hide()
			PworldDifficultyFrameDifficulty:Show()
			PworldDifficultyFrameLockTextDyn:Hide()
			PworldDifficultyFrameLockText:Show()
			PworldDifficultyFrameLockText:SetText(difficultyMap[difficulty].text)
		else
			PworldDifficultyFrameDifficulty:Hide()
			PworldDifficultyFrameDifficultyDyn:Show()
			PworldDifficultyFrameLockText:Hide()
			PworldDifficultyFrameLockTextDyn:Show()
		end
	end

	if PworldPortalFrame:IsShown() then
		PworldPortalFrame:Hide();
	end
end

function UpdatePworldDifficulty(difficulty)
	PworldDifficultyFrameDifficultyDesc:SetText(PworldDifficulty:getDifficultyDesc(lastDiffPworldId, difficulty))
	local DifficultyPworldId = PworldDifficulty:getDifficultyPworldId(lastDiffPworldId, difficulty);
	local PworldDef = getPworldDef( DifficultyPworldId );
	if PworldDef.EnterUseVig ~= 0 then
		PworldDifficultyFrameDifficultyCostInfo:Show();
		PworldDifficultyFrameDifficultyCostInfo:SetText( "（消耗"..PworldDef.EnterUseVig.."活力值）" )
	else
		PworldDifficultyFrameDifficultyCostInfo:Hide();
	end
	local mapid = PworldDifficulty:getPworldReferMapId(DifficultyPworldId)
	local path = string.format("uires\\DiTuLei\\QuYuDiTu\\%d.dds", mapid)
	if not IsInExistence(path) then
		PworldDifficultyFrameMap:Hide()
	else
		PworldDifficultyFrameMap:Show()
		PworldDifficultyFrameMap:SetTexture(path)
	end
	local button
	for i = 1, 10 do
		button = getglobal("PworldDifficultyFrameItem"..i)
		local id = PworldDifficulty:getDifficultyReward(lastDiffPworldId, difficulty, i - 1)
		if id == 0 then
			button:Hide()
		else
			button:Show()
			button:SetClientUserData(0, id)
			local name = button:GetName()
			local icon = getglobal(name.."Icon")
			if icon then
				local ItemDef = getItemDef(id)
				local IconPath = DEFAULT_ITEM_PATH
				if ItemDef then
					IconPath = GetItemEquipIconPath()..ItemDef.IconID..".tga"
				end
				-- 如果没找到物品的图标则使用默认图标显示
				if not IsInExistence(IconPath) then
					IconPath = DEFAULT_ITEM_PATH
				end
				icon:SetTexture(IconPath)
				icon:SetTexRelUV(1, 1)
				SetEquipImportantEffectUV(ItemDef, name.."UVAnimationTex")
			end
		end
	end

	button = getglobal( "PworldDifficultyFrameLeftTitle3" );
	if PworldDef.AutoPworldNum > 1 then
		button:SetText("进入人数：队伍人数不能少于" .. PworldDef.AutoPworldNum .. "人" );
	else
		button:SetText("");
	end
end

function OpenPworldDifficultyFrame(npc, index, difficulty, pworldId)
	npcId = npc
	lastDiffPworldId = index
	lastDifficulty = difficulty
	lastPworldId = pworldId
	--迷之塔特殊处理下
	if difficulty and not IsMiZhiTaPworld( PworldDifficulty:getDifficultyPworldId( index, 0 ) ) then
		SelectPworldDifficulty(difficulty)
	else
		local button = getglobal("PworldDifficultyFrameSelect"..difficultyMap[PworldDifficulty.EASY].suffix.."Btn")
		SelectPworldDifficultyBtn(button)
	end
	--这里迷之塔特殊处理下
	if IsMiZhiTaPworld( PworldDifficulty:getDifficultyPworldId( index, 0 ) ) then
		ShowPworldDifficultyType()
	else
		ShowPworldDifficultyType(not difficulty, difficulty)
	end
	PworldDifficultyFrameDifficultyDynLevel:Hide()
	PworldDifficultyFrameDifficultyDesc:SetTextColor( 255, 255, 190 );
	PworldDifficultyFrameLeftTitle2:SetText("推荐级别：");
	PworldDifficultyFrame:Show()
	util.testNpcDialogDistance(npc, PworldDifficultyFrame)
end

function OpenPworldDifficultyDynFrame(npc, index, level)
	npcId = npc
	lastDiffPworldId = index
	lastDifficulty = nil
	SelectPworldDifficultyBtn()
	UpdatePworldDifficulty(0)
	ShowPworldDifficultyType()
	PworldDifficultyFrameDifficultyDynLevel:Show()
	PworldDifficultyFrameDifficultyDynLevel:SetText((string.gsub(PworldDifficultyFrameDifficultyDynLevel:GetText(), "%d+%s*级", function(s)
		return string.gsub(s, "%d+", level)
	end)))
	PworldDifficultyFrameDifficultyDesc:SetTextColor( 0, 255, 0 );
	PworldDifficultyFrameLeftTitle2:SetText("队伍经验：");
	PworldDifficultyFrameLeftTitle3:Hide();
	PworldDifficultyFrame:Show()
	util.testNpcDialogDistance(npc, PworldDifficultyFrame)
end

function OpenPworldDifficultyTipsFrame(index, difficulty)
	lastDiffPworldId = index
	lastDifficulty = difficulty
	PworldDifficultyTipsFrame:Show()
end