local battleSummarys = {}
local battleSummaryTotal = 12
local battleSummaryMap = {
	Name					= "name",
	Clan					= "clan",
	Result					= "result",
	Kill					= "hit",
	MaxContinuousKill		= "assist",
	CrystalNum				= "crystal",
	OreNum					= "ore",
	Race					= "race",
}


local sortElement
local sortOrder

local battleSummaryIconMap = {
	--BackgroundTex = "background",
	GroupIcon	  = "group",
}

local battleSummaryRewardMap = {
	{
		RewardFrame1 = 0,
		RewardFrame1Icon = 1,
		RewardFrame1Num = 2,
	},
	{
		RewardFrame2 = 0,
		RewardFrame2Icon = 1,
		RewardFrame2Num = 2,
	},
}

local tips = {
	RankBtn					= "根据积分多少排列名次",
	RoleNameBtn				= "玩家名",
	ClanNameBtn				= "玩家所在公会",
	ActivityMarkBtn			= " 积分越多奖章越多",
	CrystalBtn				= "一个水晶资源增加5个活动积分",
	OreBtn					= "一个矿石资源增加1个活动积分",
	HitTimesBtn				= "本次活动击杀其他玩家次数",
	MaxContinuousKillBtn	= "两次死亡之间击杀其他玩家数量最大值",
	LongestExitTimeBtn		= "两次死亡之间保持生存的时间",
	MedalBtn				= "可在活动商人处购买道具",
}

local sortMap = {
	ExistenceExpFrameFrameTitleName = "name",
	ExistenceExpFrameFrameTitleClan = "clan",
	ExistenceExpFrameFrameTitleRace = "raceId",
	ExistenceExpFrameFrameTitleGroup = "groupResult",
	ExistenceExpFrameFrameTitleDead = "crystal",
	ExistenceExpFrameFrameTitleMaxContinuousKill = "ore",
	ExistenceExpFrameFrameTitleKill = "hit",
	ExistenceExpFrameFrameTitleAssist = "assist",
	ExistenceExpFrameFrameTitleResult = "result",
}

local timeTipTimer
local lastRemainTime, leaveTime = -1

local rankTop = {}
local rankEffect = {
	"9500_9",
	"9501_9",
	"9502_9",
	"9503_9",
	"9504_9",
}

local function IsExistenceExp()
	local id = GameBattle:getCurrentBattleId()
	return id == 1
end

local t_ExistenceExpFrameRemind = {
	["finish"] = { 
	uv = { w = 24, h = 21, x = 576, y = 78,},
	},
	["unFinish"] = { 
	uv = { w = 25, h = 22, x = 604, y = 78, },
	 },	
}

local function setRimindInfo( battleSummarys )
	local mainplayer = ActorMgr:getMainPlayer();
	local mainplayerName = mainplayer:getName();
	local tempBattleSummarys = battleSummarys
	--要按照
	SortBattleSummary( tempBattleSummarys, true, "result" )
	for i = 1, table.getn( tempBattleSummarys ) do
		if tempBattleSummarys[i].name == mainplayerName then
			local kill = tempBattleSummarys[i].hit + tempBattleSummarys[i].assist
			local rank = i;
			local killEnough = true
			local rankEnough = true
			if kill < 25 then
				killEnough = false;
			end
			local  PlayerNum = table.getn( tempBattleSummarys );
			if PlayerNum < 10 then
				if rank ~= 1 and rank == PlayerNum then
					rankEnough = false;
				end
			else
				if PlayerNum >=10 and math.floor(rank * 100 / PlayerNum) > 90 then
					rankEnough = false;
				end
			end
			local szInfo = "完成任一项可获得荣誉、奖章奖励：\n"
			if killEnough then
				szInfo = szInfo.."#c46c800\t1、助攻和击杀总数达到25\n"				
				local uv = t_ExistenceExpFrameRemind["finish"].uv
				ExistenceExpFrameremind1:SetTexUV( uv.x, uv.y, uv.w, uv.h )
			else
				szInfo = szInfo.."#cff6464\t1、助攻和击杀总数达到25\n"				
				local uv = t_ExistenceExpFrameRemind["unFinish"].uv
				ExistenceExpFrameremind1:SetTexUV( uv.x, uv.y, uv.w, uv.h )
			end
			if rankEnough then
				szInfo = szInfo.."#c46c800\t2、战场积分排名达到前90%"
				local uv = t_ExistenceExpFrameRemind["finish"].uv
				ExistenceExpFrameremind2:SetTexUV( uv.x, uv.y, uv.w, uv.h )
			else
				szInfo = szInfo.."#cff6464\t2、战场积分排名达到前90%"	
				local uv = t_ExistenceExpFrameRemind["unFinish"].uv
				ExistenceExpFrameremind2:SetTexUV( uv.x, uv.y, uv.w, uv.h )
			end
			ExistenceExpFrameremind:SetText( szInfo, 255, 255, 190 )
		end
	end
end

local function UpdateBattleSummary()
	local v	= ExistenceExpFrame_ScrollBar:GetValue()
	local n = #battleSummarys
	local ui
	for i = 1, battleSummaryTotal do
		local btn = getglobal("ExistenceInfo"..i)
		if btn then
			if i + v > n then
				btn:Hide()
			else
				btn:Show()
				local t = battleSummarys[i + v]
				for suffix, key in pairs(battleSummaryMap) do
					ui = getglobal(string.format("ExistenceInfo%d%s", i, suffix))
					if ui then
						if t[key] then
							ui:Show()
							ui:SetText(t[key])
							ui:SetTextColor(t.color[1], t.color[2], t.color[3])
						else
							ui:Hide()
						end
					end
				end
				for suffix, key in pairs(battleSummaryIconMap) do
					ui = getglobal(string.format("ExistenceInfo%d%s", i, suffix))
					if ui then
						if t[key] then
							ui:Show()
							ui:SetTexture(t[key])
						else
							ui:Hide()
						end
					end
				end
				-- 注释设置奖励
				--local n = 1
				--for m = 1, #battleSummaryRewardMap do
				--	local reward = t[n]
				--	while reward and (reward[1] == 0 or reward[2] == 0) do
				--		n = n + 1
				--		reward = t[n]
				--	end
				--	n = n + 1
				--	for suffix, key in pairs(battleSummaryRewardMap[m]) do
				--		ui = getglobal(string.format("ExistenceInfo%d%s", i, suffix))
				--		if reward then
				--			ui:Show()
				--			if key == 0 then
				--				ui:SetClientUserData(0, reward[1])
				--			elseif key == 1 then
				--				ui:SetTexture(battleRewardIcon[reward[1]])
				--			elseif key == 2 then
				--				ui:SetText("x "..reward[2])
				--				ui:SetTextColor(t.color[1], t.color[2], t.color[3])
				--			end
				--		else
				--			ui:Hide()
				--		end
				--	end
				--end
			end
		end
	end
	local now = getServerTime().sec
	if leaveTime and GameBattle:isRun() then
		local remain = math.ceil(leaveTime - now)
		if remain < 0 then
			remain = 0
		end
		if lastRemainTime ~= remain then
			lastRemainTime = remain
			local hour = math.floor(remain / 3600)
			local minute = math.floor((remain - hour * 3600) / 60)
			local second = remain - hour * 3600 - minute * 60
			ExistenceExpFrame_LeaveTimeBtnFont:SetText(string.format("剩余时间 %02d：%02d：%02d", hour, minute, second))
		end
		ExistenceExpFrameWinFont:SetText("")
		ExistenceExpFramePostscriptText:SetText("",255,255,255)
		ExistenceExpFrameBrandTex:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" )
		ExistenceExpFrameWinBrandInfoFont:SetText("")
	else
		ExistenceExpFrame_LeaveTimeBtnFont:SetText("")
	end
	--战场奖励修改
	setRimindInfo( battleSummarys )
end

local function UpdateBattleSummaryData()
	if not GameBattle:isRun() then
		return
	end
	battleSummarys = {}
	local results = { 0, 0, 0, }
	for i = 0, GameBattle:getPlayerNum() - 1 do
		local camp = GameBattle:getPlayerCamp(i)
		results[camp] = results[camp] + GameBattle:getPlayerPolyFraction(i)
	end
	for i = 0, GameBattle:getPlayerNum() - 1 do
		local camp = GameBattle:getPlayerCamp(i)
		local isMain = false
		if GameBattle:getPlayerID(i) == ActorMgr:getMainPlayer():GetID() then
			isMain = true
		end
		table.insert(battleSummarys, {
			name = GetPlayerName( GameBattle:getPlayerName(i) ),
			--race = careerIdxTexture[GameBattle:getPlayerCarrer(i)] or careerIdxTexture[GameBattle:getPlayerCarrer(i) % 16],
			race = GetCareerName( GameBattle:getPlayerCarrer(i) ),
			raceId = GameBattle:getPlayerCarrer(i),
			clan = GameBattle:getPlayerClan(i),
			hit = GameBattle:getPlayerKill(i),
			assist = GameBattle:getPlayerAssist(i),
			dead = GameBattle:getPlayerDieNum(i),
			crystal = GameBattle:getPlayerCrystal(i),
			ore = GameBattle:getPlayerOre(i),
			maxkill = GameBattle:getPlayerMaxContinueKill(i),
			existtime = GameBattle:getPlayerMaxLife(i),
			result = GameBattle:getPlayerPolyFraction(i),
			groupResult = results[GameBattle:getPlayerCamp(i)],
			group = campIdxIcon[GameBattle:getPlayerCamp(i)],
			camp = GameBattle:getPlayerCamp(i),
			idx = i,
			background = isMain and campIdxTexture[-1] or campIdxTexture[camp],
			color = isMain and campIdxTextColor[-1] or campIdxTextColor[camp],
			{
				GameBattle:getPlayerReward(i),
				GameBattle:getPlayerRewardNum(i),
			},
			{
				GameBattle:getPlayerRewardEx(i),
				GameBattle:getPlayerRewardNumEx(i),
			},
		})
	end
	
	if BattleStarted then
		SortBattleSummary( battleSummarys,sortOrder,sortElement )
	end
	
	local n = #battleSummarys
	if n > battleSummaryTotal then
		ExistenceExpFrame_ScrollBar:Show()
		ExistenceExpFrame_ScrollBar:SetMaxValue(n - battleSummaryTotal)
	else
		ExistenceExpFrame_ScrollBar:Hide()
		ExistenceExpFrame_ScrollBar:SetMaxValue(0)
	end
	ExistenceExpFrame_ScrollBar:SetValue(math.min(ExistenceExpFrame_ScrollBar:GetMaxValue(), ExistenceExpFrame_ScrollBar:GetValue()))
	UpdateBattleSummary()
end

function BattleExistenceItemTitle_onClick()
	local name = this:GetName();
	local sort = sortMap[name];
	if sort ~= nil then
		if sortElement == sort then
			sortOrder = not sortOrder
		else
			sortElement = sort
			sortOrder = true
		end
		SortBattleSummary( battleSummarys,sortOrder,sortElement )
		UpdateBattleSummary()
	end
end

function ExistenceExpFrame_OnLoad()
	this:RegisterEvent("GE_ENTER_PLAYERLOGIN")
	this:RegisterEvent("GE_BATTLE_ENTER")
	this:RegisterEvent("GE_BATTLE_LEAVE")
	this:RegisterEvent("GE_BATTLE_END")
	this:RegisterEvent("GE_BATTLE_INFO")
	this:RegisterEvent("GE_BATTLE_OBJ_ENTER")
	ExistenceExpFrame:setUpdateTime(1)
	for i = 1, 12 do
		local btn = getglobal("ExistenceInfo"..i)
		local t_Tex = {
			btn:GetName() .. "NameTex",btn:GetName() .. "ClanTex",btn:GetName() .. "RaceTex",
			btn:GetName() .. "GroupTex",btn:GetName() .. "CrystalTex",btn:GetName() .. "OreTex",
			btn:GetName() .. "KillTex",btn:GetName() .. "MaxContinuousKillTex",btn:GetName() .. "ResultTex"
		};
		for j=1,#t_Tex do
			local tex = getglobal( t_Tex[j] )
			if i%2 == 0 then				
				tex:ChangeTextureTemplate( "WhiteCategoryBkg" )
			else
				tex:ChangeTextureTemplate( "BlackCategoryBkg" )
			end
		end		
	end
	local uv = t_ExistenceExpFrameRemind["unFinish"].uv
	ExistenceExpFrameremind1:SetTexUV( uv.x, uv.y, uv.w, uv.h )
	ExistenceExpFrameremind2:SetTexUV( uv.x, uv.y, uv.w, uv.h )
	ExistenceExpFrameremind:SetText( t_ExistenceExpFrameRemind["unFinish"].szInfo, 255, 255, 190 )
end

function ExistenceExpFrame_OnEvent()
	if arg1 == "GE_ENTER_PLAYERLOGIN" then
		leaveTime = nil
		ExistenceExp_ResourceDisplay:Hide()
		ExistenceExpTotalResultFrame:Hide()
		if timeTipTimer then
			hangup(timeTipTimer)
			timeTipTimer = nil
		end
	elseif arg1 == "GE_BATTLE_ENTER" then
		if IsExistenceExp() then
			rankTop = {}
			BattleSummaryFrame:Hide()
			ExistenceExpFrame_LeaveTimeBtn:Show()
			ExistenceExpFrame_ExitActivityBtn:Show()
			ExistenceExpFrame:Hide()
			ExistenceExpFrame_ScrollBar:SetValue(0)
			ExistenceExp_ResourceSet(0, 0)
			ExistenceExp_ResourceDisplay:Show()
			ExistenceExpTotalResultFrame:Show()
			SetTeamPhotoShowShow(false)
			local player = ActorMgr:getMainPlayer()
			if player then
				leaveTime = player:getPworldLeaveTime()
				timeTipTimer = timer(CountTimeTipsTimer, leaveTime - 10)
				leaveTime = leaveTime + getServerTime().sec
			end
		end
	elseif arg1 == "GE_BATTLE_LEAVE" then
		if IsExistenceExp() then
			leaveTime = nil
			SetTeamPhotoShowShow(true)
			ExistenceExpFrame_LeaveTimeBtn:Hide()
			ExistenceExpFrame_ExitActivityBtn:Hide()
			ExistenceExp_ResourceDisplay:Hide()
			ExistenceExpTotalResultFrame:Hide()
			if timeTipTimer then
				hangup(timeTipTimer)
				timeTipTimer = nil
			end
			for k, v in ipairs(rankTop) do
				local actor = ActorMgr:FindActor(v[1])
				if actor then
					actor:stopMotion(rankEffect[k])
				else
					actor = ActorMgr:findActorByName(v[2])
					if actor then
						actor:stopMotion(rankEffect[k])
					end
				end
			end
		end
	elseif arg1 == "GE_BATTLE_END" then
		if IsExistenceExp() then
			leaveTime = nil
			local nIdx = GameBattle:getPlayerIdx(ActorMgr:getMainPlayer():GetID())			
			local rankGlory = GameBattle:getPlayerRankGlory(nIdx)
			local numGlory = GameBattle:getPlayerNumGlory(nIdx)
			local nExp = GameBattle:getPlayerExp(nIdx)
			local nFristWinExp = GameBattle:getPlayerFristWinExp(nIdx)
			local num = GameBattle:getPlayerFiniNum(nIdx)
			local winGlory
			local winText
			if GameBattle:isWinGame() then
				ExistenceExpFrameWinFont:SetText("胜利！")
				winGlory = GameBattle:getPlayerWinGlory(nIdx)
				winText = "胜利：+"				
			else
				ExistenceExpFrameWinFont:SetText("失败！")
				winGlory = GameBattle:getPlayerFailGlory(nIdx)
				winText = "失败：+"
			end
			
			local szText = "";
			if (winGlory + rankGlory + numGlory) > 0 then
				szText = "#cffffbe获得荣誉：" .. (winGlory + rankGlory + numGlory)  .. "#n（#c96965a";
				if winGlory > 0 then
					szText = szText .. winText .. winGlory .. "；";
				end
				szText = szText .. " 排名第" .. nIdx + 1 .. "名：+" .. rankGlory .. "；第" .. num .."场出战：" .. numGlory .. "）#n\n"
			else
				szText = szText .. "\n"
			end
			szText = szText .. "#cffffbe获得经验：" .. ( nExp + nFristWinExp ) .."#n"
			if nFristWinExp > 0 then
				szText = szText .. "（#c96965a首场胜利经验" .. nFristWinExp .. "）#n"
			end
			szText = szText .. "\n";
			ExistenceExpFramePostscriptText:SetText(szText,255,255,255)
			local brandNum = GameBattle:getPlayerRewardNum( nIdx )
			local brandFiniNum = GameBattle:getPlayerFiniNum( nIdx )
			local brandTotalFiniNum = GameBattle:getTotalCanFiniNum()
			local brandInfoFont = getglobal( "ExistenceExpFrameWinBrandInfoFont" )
			brandInfoFont:SetText( "获得奖章："..brandNum.."/3".."      (每天最多可以获得3个奖章）" )
			local brand = GameBattle:getPlayerReward( nIdx )
			local brandTex = getglobal( "ExistenceExpFrameBrandTex" )
			brandTex:SetTexture(battleRewardIcon[ATTR_ID_AOSHAN_BRAND])
			if brandNum > 0 then
				brandInfoFont:Show()
				brandTex:Show()
			else
				brandInfoFont:Hide()
				brandTex:Hide()
			end
		end
	elseif arg1 == "GE_BATTLE_INFO" then
		if IsExistenceExp() then
			local m, n
			local id = ActorMgr:getMainPlayer():GetID()
			for i = 0, GameBattle:getPlayerNum() - 1 do
				if GameBattle:getPlayerID(i) == id then
					m = GameBattle:getPlayerCrystal(i)
					n = GameBattle:getPlayerOre(i)
					break
				end
			end
			if m and n then
				ExistenceExp_ResourceSet(m, n)
			end
			local old = {}
			for k, v in ipairs(rankTop) do
				old[v[1]] = k
				rankTop[k] = nil
			end
			if BattleStarted then
				local t = {}
				for i = 0, GameBattle:getPlayerNum() - 1 do
					table.insert(t, {
						id = GameBattle:getPlayerID(i),
						result = GameBattle:getPlayerPolyFraction(i),
						name = GameBattle:getPlayerName(i),
					})
				end
				-- 按积分面板上玩家按阵营排列，阵营顺序按总积分高低从上到下排列，阵营中按个人积分从大到小排列
				table.sort(t, function(a, b)
					if a.camp == b.camp then
						return a.result > b.result
					end
					return a.camp < b.camp
				end)				
				for k, v in ipairs(t) do
					table.insert(rankTop, {v.id, v.name})
					if #rankTop == 5 then
						break
					end
				end
			else
				for i = 0, GameBattle:getPlayerNum() - 1 do
					table.insert(rankTop, {GameBattle:getPlayerID(i), GameBattle:getPlayerName(i)})
					if #rankTop == 5 then
						break
					end
				end
			end
			for k, v in ipairs(rankTop) do
				if k == old[v[1]] then
					old[v[1]] = nil
				else
					local actor = ActorMgr:FindActor(v[1])
					if actor then
						actor:playMotion(rankEffect[k])
					end
				end
			end
			for k, v in pairs(old) do
				local actor = ActorMgr:FindActor(k)
				if actor then
					actor:stopMotion(rankEffect[v])
				end
			end
		end
	elseif arg1 == "GE_BATTLE_OBJ_ENTER" then
		if IsExistenceExp() then
			local msg = UIMSG:getUIMsg()
			if msg.Who.type == GAT_PLAYER then
				local id = msg.Who.id
				for k, v in ipairs(rankTop) do
					if v[1] == id then
						local actor = ActorMgr:FindActor(id)
						if actor then
							actor:playMotion(rankEffect[k])
						end
						break
					end
				end
			end
		end
	end
end

function ExistenceExpFrame_ScrollUpBtn_OnClick()
	ExistenceExpFrame_ScrollBar:SetValue(ExistenceExpFrame_ScrollBar:GetValue() - ExistenceExpFrame_ScrollBar:GetValueStep())
end

function ExistenceExpFrame_ScrollDownBtn_OnClick()
	ExistenceExpFrame_ScrollBar:SetValue(ExistenceExpFrame_ScrollBar:GetValue() + ExistenceExpFrame_ScrollBar:GetValueStep())
end

function ExistenceExpFrame_ScrollBar_OnValueChanged()
	UpdateBattleSummary()
end

function ExistenceExpFrame_OnShow()	
	sortElement = "result"
	sortOrder = true
	lastRemainTime = -1
	UpdateBattleSummaryData()
end

function ExistenceExpFrame_OnUpdate()
	if BattleStarted then
		UpdateBattleSummaryData()
	end
end

function ExistenceExpFrame_OnHide()
	BattlePlayerInfoRightMenuFrame:Hide()
end

-- 退出活动
function ExistenceExpFrame_ExitActivity_OnClick()
	ExistenceExpFrame:Hide()
	if BattleStarted then
		BattleLeaveConfirmFrame:Show()
	else
		GameBattle:LeavePworld()
	end
end

-- 设置主界面显示水晶和矿石的数量
function ExistenceExp_ResourceSet(crystal, ore)
	local m = tonumber(ExistenceExp_ResourceDisplayCrystalFont:GetText())
	local n = tonumber(ExistenceExp_ResourceDisplayOreFont:GetText())
	ExistenceExp_ResourceDisplayCrystalFont:SetText(crystal)
	ExistenceExp_ResourceDisplayOreFont:SetText(ore)
	if ExistenceExp_ResourceDisplay:IsShown() then
		if m then
			m = crystal - m
			if m ~= 0 then
				AddFlyFont(18, CRYSTAL_FONT, string.format("%s%dA", m > 0 and "+" or "-", math.abs(m)))
			end
		end
		if n then
			n = ore - n
			if n ~= 0 then
				AddFlyFont(19, ORE_FONT, string.format("%s%dA", n > 0 and "+" or "-", math.abs(n)))
			end
		end
	end
end

-- 统计表头显示tips
function ExistenceExpTitle_OnEnter()
	local name = this:GetName()
	local _, _, title = string.find(name, "^ExistenceExpFrame(.*)")
	if title and tips[title] then
		SetGameTooltips(name, tips[title])
	end
end

function ExistenceExpTitle_OnLeave()
	GameTooltipRichText:SetText("", 255, 255, 255)
	GameTooltip:Hide()
end

function ExistenceExpResource_OnEnter()
	local name = this:GetName()
	if name == "ExistenceExp_ResourceDisplayCrystalBack" then
		SetGameTooltips(name, "收集到的水晶")
	elseif name == "ExistenceExp_ResourceDisplayOreBack" then
		SetGameTooltips(name, "收集到的矿石")
	end
end

function ExistenceExpResource_OnLeave()
	GameTooltipRichText:SetText("",255,255,255)
	GameTooltip:Hide()
end

function CountTimeTipsTimer()
	for i = 10, 1, -1 do
		AddGameMiddleTips(string.format("本场活动将在%d秒后结束", i))
		coroutine.yield(1)
	end
end

function ExistenceExpTotalResultFrame_OnShow()
	this:setUpdateTime(0)
	for i=1,3 do
		local tex = getglobal("ExistenceExpTotalResultFrameProgressTex" .. i)
		local font = getglobal("ExistenceExpTotalResultFrameResultFont" .. i)
		tex:SetSize(0,9);
		font:SetText("");
	end
end

function ExistenceExpTotalResultFrame_OnUpdate()
	if not GameBattle:isRun() then
		return
	end
	local tCampReult = {0,0,0}
	for i = 0, GameBattle:getPlayerNum() - 1 do

		tCampReult[GameBattle:getPlayerCamp(i)] = tCampReult[GameBattle:getPlayerCamp(i)] + GameBattle:getPlayerPolyFraction(i)
	end
	local totalResult = 0;
	for i=1,#tCampReult do
		local font = getglobal("ExistenceExpTotalResultFrameResultFont" .. i)
		font:SetText(tCampReult[i])
		totalResult = totalResult + tCampReult[i]
	end
	if totalResult ~= 0 then
		for i=1,#tCampReult do
			local tex = getglobal("ExistenceExpTotalResultFrameProgressTex" .. i)
			tex:SetSize( 101*tCampReult[i]/totalResult,9 )
		end
	end
	local index = GameBattle:getPlayerIdx( ActorMgr:getMainPlayer():GetID() );
	ExistenceExpTotalResultFrameKillInfo:SetText( "击杀："..GameBattle:getPlayerKill( index ).."	排名："..(index+1))
end

--------------------------------------------------狂沙战场排行榜右键界面--------------------------------------
function BattleExistenceItemTemplate_OnClick()
	if arg1 == "RightButton" then
		if not GameBattle:isRun() then return end
		local n = this:GetClientID()
		local index = ExistenceExpFrame_ScrollBar:GetValue() + n
		local playerName = GameBattle:getPlayerName( index - 1 )
		BattlePlayerInfoRightMenuFrame:Show()
		BattlePlayerInfoRightMenuFrame:SetClientString( playerName )
		BattlePlayerInfoRightMenuFrame:SetPoint( "topleft", "$parent", "topleft", GetCursorPosX(), GetCursorPosY() )
	end
end

