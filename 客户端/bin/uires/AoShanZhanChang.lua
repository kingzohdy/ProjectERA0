local battleSummarys = {}
local battleSummaryTotal = 12
local battleSummaryMap = {
	Name = "name",
	Clan = "clan",
	Result = "result",
	Kill = "kill",
	Assist = "assist",
	Dead = "dead",
	--MaxContinuousKill = "maxkill",
	Race = "race"
}

local battleSummaryIconMap = {
	--BackgroundTex = "background",
	GroupIcon = "group",
}

	
local sortMap = {
	BattleSummaryItemTitleName = "name",
	BattleSummaryItemTitleClan = "clan",
	BattleSummaryItemTitleRace = "raceId",
	BattleSummaryItemTitleGroup = "groupResult",
	BattleSummaryItemTitleResult = "kill",
	BattleSummaryItemTitleKill = "dead",
	BattleSummaryItemTitleAssist = "assist",
	BattleSummaryItemTitleDead = "result",
	BattleSummaryItemTitleMaxContinuousKill = "maxkill",
}

local sortElement
local sortOrder

local boss1
local boss2

local boss1res = { 282036, 282071 }
local boss2res = { 282035, 282070 }

local _battleImportantIcon = {
	350046,
	350047,
	350048,
	350049,
	350050,
}

local battleCampImportantActorColor = {
	{255, 0, 0},
	{0, 0, 255},
}

local battleImportantIcon = {}

for _, v in ipairs(_battleImportantIcon) do
	battleImportantIcon[v] = true
end

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
	}
}

local nKill = 0
local nDead = 0
local nAssist = 0
local nMaxContinue = 0
local nReward1 = {}
local nReward2 = {}

local campName = {
	"绿洲",
	"沙漠",
}

local function IsAoShan()
	local id = GameBattle:getCurrentBattleId()
	return id == 2
end

local t_BattleSummaryFrameRemind = {
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
			local kill = tempBattleSummarys[i].kill + tempBattleSummarys[i].assist
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
				local uv = t_BattleSummaryFrameRemind["finish"].uv
				BattleSummaryFrameremind1:SetTexUV( uv.x, uv.y, uv.w, uv.h )
			else
				szInfo = szInfo.."#cff6464\t1、助攻和击杀总数达到25\n"				
				local uv = t_BattleSummaryFrameRemind["unFinish"].uv
				BattleSummaryFrameremind1:SetTexUV( uv.x, uv.y, uv.w, uv.h )
			end
			if rankEnough then
				szInfo = szInfo.."#c46c800\t2、战场积分排名达到前90%"
				local uv = t_BattleSummaryFrameRemind["finish"].uv
				BattleSummaryFrameremind2:SetTexUV( uv.x, uv.y, uv.w, uv.h )
			else
				szInfo = szInfo.."#cff6464\t2、战场积分排名达到前90%"
				local uv = t_BattleSummaryFrameRemind["unFinish"].uv
				BattleSummaryFrameremind2:SetTexUV( uv.x, uv.y, uv.w, uv.h )
			end
			BattleSummaryFrameremind:SetText( szInfo, 255, 255, 190 )
		end
	end
end

local function UpdateBattleSummary()
	local v	= BattleSummaryFrameScrollBar:GetValue()
	local n = #battleSummarys
	local ui
	for i = 1, battleSummaryTotal do
		local btn = getglobal("BattleSummaryItem"..i)
		if btn then
			if i + v > n then
				btn:Hide()
			else
				btn:Show()
				local t = battleSummarys[i + v]
				for suffix, key in pairs(battleSummaryMap) do
					ui = getglobal(string.format("BattleSummaryItem%d%s", i, suffix))
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
					ui = getglobal(string.format("BattleSummaryItem%d%s", i, suffix))
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
				--		ui = getglobal(string.format("BattleSummaryItem%d%s", i, suffix))
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

	if BattleStarted then
		BattleSummaryFrameProgressTimeText:SetText("")
		BattleSummaryFrameWinFont:SetText("")
		BattleSummaryPostscriptText:SetText("",255,255,255)
		BattleSummaryFrameWinBrandInfoFont:SetText("")
		BattleSummaryFrameBrandTex:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" )
		local player = ActorMgr:getMainPlayer()
		if player then
			local diff = getServerTime().sec - player:getPworldStartTime()
			BattleSummaryFrameProgressTimeText:SetText(string.format("持续时间 %02d:%02d:%02d", math.floor(diff / 3600), math.floor((diff % 3600) / 60), diff % 60))
		end
	
	end
	--战场奖励修改
	setRimindInfo( battleSummarys )
end

local function UpdateBattleSummaryData()
	if GameBattle:isRun() then
		battleSummarys = {}
		local results = { 0, 0, }
		for i = 0, GameBattle:getPlayerNum() - 1 do
			local camp = GameBattle:getPlayerCamp(i)
			results[camp] = results[camp] + GameBattle:getPlayerPolyFraction(i)
		end
		for i = 0, GameBattle:getPlayerNum() - 1 do
			local kills = GameBattle:getPlayerKill(i)
			local deads = GameBattle:getPlayerDieNum(i)
			local assists = GameBattle:getPlayerAssist(i)
			local maxcontinues = GameBattle:getPlayerMaxContinueKill(i)
			local m1 = GameBattle:getPlayerReward(i)
			local m2 = GameBattle:getPlayerRewardEx(i)
			local n1 = GameBattle:getPlayerRewardNum(i)
			local n2 = GameBattle:getPlayerRewardNumEx(i)
			local camps = GameBattle:getPlayerCamp(i)
			local isMain = false
			if GameBattle:getPlayerID(i) == ActorMgr:getMainPlayer():GetID() then
				nKill = kills
				nDead = deads
				nAssist = assists
				nMaxContinue = maxcontinues
				nReward1 = {m1, n1}
				nReward2 = {m2, n2}
				isMain = true
			end
			table.insert(battleSummarys, {
				name = GetPlayerName( GameBattle:getPlayerName(i) ),
				--race = careerIdxTexture[GameBattle:getPlayerCarrer(i)] or careerIdxTexture[GameBattle:getPlayerCarrer(i) % 16],
				race = GetCareerName( GameBattle:getPlayerCarrer(i) ),
				raceId = GameBattle:getPlayerCarrer(i),
				clan = GameBattle:getPlayerClan(i),
				result = GameBattle:getPlayerPolyFraction(i),
				groupResult = results[GameBattle:getPlayerCamp(i)],
				kill = kills,
				assist = assists,
				dead = deads,
				maxkill = maxcontinues,
				group = campIdxIcon[camps],
				camp = camps,
				idx = i,
				background = isMain and campIdxTexture[-1] or campIdxTexture[camps],
				color = isMain and campIdxTextColor[-1] or campIdxTextColor[camps],
				{
					m1, n1
				},
				{
					m2, n2
				},
			})
		end
	end

	SortBattleSummary( battleSummarys,sortOrder,sortElement )

	local n = #battleSummarys
	if n > battleSummaryTotal then
		BattleSummaryFrameScrollBar:Show()
		BattleSummaryFrameScrollBar:SetMaxValue(n - battleSummaryTotal)
	else
		BattleSummaryFrameScrollBar:Hide()
		BattleSummaryFrameScrollBar:SetMaxValue(0)
	end
	BattleSummaryFrameScrollBar:SetValue(math.min(BattleSummaryFrameScrollBar:GetMaxValue(), BattleSummaryFrameScrollBar:GetValue()))
	UpdateBattleSummary()
end

function AoShanBattleSummaryFrame_OnLoad()
	this:RegisterEvent("GE_BATTLE_ENTER")
	this:RegisterEvent("GE_BATTLE_LEAVE")
	this:RegisterEvent("GE_BATTLE_END")
	this:RegisterEvent("GE_BATTLE_OBJ_ENTER")
	this:RegisterEvent("GE_BATTLE_OBJ_LEAVE")
	--BattleSummaryItemTitleReward:Disable()
	BattleSummaryFrame:setUpdateTime(1)

	for i = 1, 12 do
		local btn = getglobal("BattleSummaryItem"..i)
		local t_Tex = {
			btn:GetName() .. "NameTex",btn:GetName() .. "ClanTex",btn:GetName() .. "RaceTex",
			btn:GetName() .. "GroupTex",btn:GetName() .. "KillTex",btn:GetName() .. "DeadTex",
			btn:GetName() .. "AssistTex",btn:GetName() .. "ResultTex",
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
end


function BattleSummaryItemTitle_OnClick()
	local name = this:GetName()
	local sort = sortMap[name]
	if sort then
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

function AoShanBattleSummaryFrame_OnEvent()
	if arg1 == "GE_BATTLE_ENTER" then
		if IsAoShan() then
			BattleSummaryFrame:Hide()
			BattleSummaryFrameScrollBar:SetValue(0)
			boss1 = nil
			boss2 = nil
			AoShanBattleBossFrame:Show()
			BattleSummaryFrameLeave:Show()
			BattleSummaryFrameExtraExp:Hide()
		end
	elseif arg1 == "GE_BATTLE_LEAVE" then
		if IsAoShan() then
			ClearImportMinimapIcon()
			AoShanBattleBossFrame:Hide()
			BattleSummaryFrameLeave:Hide()
		end
	elseif arg1 == "GE_BATTLE_END" then
		if IsAoShan() then
			BattleStarted = false
			local nIdx = GameBattle:getPlayerIdx(ActorMgr:getMainPlayer():GetID())			
			local rankGlory = GameBattle:getPlayerRankGlory(nIdx)
			local numGlory = GameBattle:getPlayerNumGlory(nIdx)
			local nExp = GameBattle:getPlayerExp(nIdx)
			local nFristWinExp = GameBattle:getPlayerFristWinExp(nIdx)
			local num = GameBattle:getPlayerFiniNum(nIdx)
			local winGlory
			local winText
			if GameBattle:isWinGame() then
				BattleSummaryFrameWinFont:SetText("胜利！")
				winGlory = GameBattle:getPlayerWinGlory(nIdx)
				winText = "胜利：+"				
			else
				BattleSummaryFrameWinFont:SetText("失败！")
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
				szText = szText .. " \n"
			end
			szText = szText .. "#cffffbe获得经验：" .. ( nExp + nFristWinExp ) .."#n"
			if nFristWinExp > 0 then
				szText = szText .. "（#c96965a首场胜利经验" .. nFristWinExp .. "）#n"
			end
			szText = szText .. "\n";
			BattleSummaryPostscriptText:SetText(szText,255,255,255)
			local brandNum = GameBattle:getPlayerRewardNum( nIdx )
			local brandFiniNum = GameBattle:getPlayerFiniNum( nIdx )
			local brandTotalFiniNum = GameBattle:getTotalCanFiniNum()
			local brandInfoFont = getglobal( "BattleSummaryFrameWinBrandInfoFont" )
			--[[
			if brandFiniNum > brandTotalFiniNum then
				brandInfoFont:SetText( "已完成徽章奖励" )
				local brandTex = getglobal( "BattleSummaryFrameBrandTex" )
				brandTex:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" )
			else
				brandInfoFont:SetText( "获得奖章："..brandNum.."      (完成战场:"..brandFiniNum.."/"..brandTotalFiniNum..")" )			
				local brand = GameBattle:getPlayerReward( nIdx )
				local brandTex = getglobal( "BattleSummaryFrameBrandTex" )
				if brandNum <= 0 then
					brandTex:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" )
				else
					brandTex:SetTexture(battleRewardIcon[brand])
				end				
			end
			]]--
			brandInfoFont:SetText( "获得奖章："..brandNum.."/3".."      (每天最多可以获得3个奖章）" )
			local brand = GameBattle:getPlayerReward( nIdx )
			local brandTex = getglobal( "BattleSummaryFrameBrandTex" )
			brandTex:SetTexture(battleRewardIcon[ATTR_ID_AOSHAN_BRAND])
			if brandNum > 0 then
				brandInfoFont:Show()
				brandTex:Show()
			else
				brandInfoFont:Hide()
				brandTex:Hide()
			end
		end
	elseif arg1 == "GE_BATTLE_OBJ_ENTER" then
		if IsAoShan() then
			local msg = UIMSG:getUIMsg()
			if msg.Who.type == GAT_MONSTER then
				local id = msg.Who.id
				local actor = ActorMgr:FindActor(id)
				if actor then
					local resid = actor:getResId()
					if battleImportantIcon[resid] then
						local str
						local camp = actor:getCampIdx()
						local color = battleCampImportantActorColor[camp]
						if color then
							actor:setTeamColor(color[1], color[2], color[3])
						end
						if camp == 0 then
							str = tostring(actor:getResId())
						else
							str = string.format("%d_%d", actor:getResId(), camp)
						end
						str = string.format("uires\\DiTuLei\\MinimapIcon\\%s.tga", str)
						if IsInExistence(str) then
							AddImportMinimapIcon(id, str)
							local MaxMapControl	= MapShower:GetMaxMapContal()
							AddImportWordMapIcon(id, MaxMapControl:getMapID(), str, actor:getPlayerX(), actor:getPlayerZ())
						end
					end
					if resid == boss1res[1] or resid == boss1res[2] then
						boss1 = id
					elseif resid == boss2res[1] or resid == boss2res[2] then
						boss2 = id
					end
				end
			end
		end
	elseif arg1 == "GE_BATTLE_OBJ_LEAVE" then
		if IsAoShan() then
			local msg = UIMSG:getUIMsg()
			local id = msg.Who.id
			RemoveImportMinimapIcon(id)
			RemoveImportWordMapIcon(id)
			if id == boss1 then
				boss1 = nil
			elseif id == boss2 then
				boss2 = nil
			end
		end
	end
end

function AoShanBattleSummaryFrame_ScrollUpBtn()
	local slider = BattleSummaryFrameScrollBar
	if not slider:IsShown() then
		return
	end
	local nValue = slider:GetValue() - slider:GetValueStep()
	if nValue >= slider:GetMinValue() then
		slider:SetValue(nValue)
	end
end

function AoShanBattleSummaryFrame_ScrollDownBtn()
	local slider	= BattleSummaryFrameScrollBar
	if not slider:IsShown() then
		return
	end
	local nValue	= slider:GetValue() + slider:GetValueStep()
	if nValue <= slider:GetMaxValue() then
		slider:SetValue(nValue)
	end
end

function AoShanBattleSummaryFrame_ScrollBarValueChange()
	UpdateBattleSummary()
end

function AoShanBattleSummaryFrame_OnShow()
	sortElement = "result"
	sortOrder = true
	UpdateBattleSummaryData()
end

function AoShanBattleSummaryFrame_OnUpdate()
	if BattleStarted then
		UpdateBattleSummaryData()
	end
end

function AoShanBattleSummaryFrame_OnHide()
	util.ShowInCenter(this)
	BattlePlayerInfoRightMenuFrame:Hide()
end

function AoShanBattleSummaryFrameLeave_OnClick()
	BattleSummaryFrame:Hide()
	if BattleStarted then
		BattleLeaveConfirmFrame:Show()
	else
		GameBattle:LeavePworld()
	end
end

function AoShanBattleSummaryPostscriptText_OnClick()
	--[[
	if arg3 == "LeftButton" or arg3 == "RightButton" then
		local id = tonumber(arg2)
		if id then
			ShowItemTips(LUA_ITEMTIPS_TYPE_LINK, LUA_ITEMTIPS_ITEMDEF, getItemDef(id))
		end
	end
	]]--
end

function AoShanBattleBossFrame_OnLoad()
	BattleSummaryFrame:setUpdateTime(0.3)
end

function AoShanBattleBossFrame_OnEvent()
	
end

function AoShanBattleBossFrame_OnShow()
	if not AoShanBattleBossFrameHP1:IsShown() then
		AoShanBattleBossFrameHP1:Show()
	end
	AoShanBattleBossFrameHP1:SetTexUV(698, 670, 62, 9)
	AoShanBattleBossFrameHP1:SetSize(62, 9)
	AoShanBattleBossFrameHP1:SetPoint("left", "AoShanBattleBossFrame", "left", 36, 0)
	if not AoShanBattleBossFrameHP2:IsShown() then
		AoShanBattleBossFrameHP2:Show()
	end
	AoShanBattleBossFrameHP2:SetTexUV(698, 680, 62, 9)
	AoShanBattleBossFrameHP2:SetSize(62, 9)
	AoShanBattleBossFrameHP2:SetPoint("right", "AoShanBattleBossFrame", "right", -36, 0)
end

function AoShanBattleBossFrame_OnUpdate()
	if boss1 then
		local actor = ActorMgr:FindActor(boss1)
		if actor then
			if actor:isDead() then
				if AoShanBattleBossFrameHP1:IsShown() then
					AoShanBattleBossFrameHP1:Hide()
				end
			else
				if not AoShanBattleBossFrameHP1:IsShown() then
					AoShanBattleBossFrameHP1:Show()
				end
				local scale = actor:getHP() / actor:getMaxHP()
				local width = math.floor(scale * 62 + 0.5)
				AoShanBattleBossFrameHP1:SetTexUV(698, 670, width, 9)
				AoShanBattleBossFrameHP1:SetSize(width, 9)
				AoShanBattleBossFrameHP1:SetPoint("left", "AoShanBattleBossFrame", "left", 36, 0)
			end
		end
	end
	if boss2 then
		local actor = ActorMgr:FindActor(boss2)
		if actor then
			if actor:isDead() then
				if AoShanBattleBossFrameHP2:IsShown() then
					AoShanBattleBossFrameHP2:Hide()
				end
			else
				if not AoShanBattleBossFrameHP2:IsShown() then
					AoShanBattleBossFrameHP2:Show()
				end
				local scale = actor:getHP() / actor:getMaxHP()
				local width = math.floor(scale * 62 + 0.5)
				AoShanBattleBossFrameHP2:SetTexUV(698, 680, width, 9)
				AoShanBattleBossFrameHP2:SetSize(width, 9)
				AoShanBattleBossFrameHP2:SetPoint("right", "AoShanBattleBossFrame", "right", -(36 + 62 - width), 0)
			end
		end
	end
	local index = GameBattle:getPlayerIdx( ActorMgr:getMainPlayer():GetID() );
	AoShanBattleBossFramekillNum:SetText( GameBattle:getPlayerKill( index ) )
	AoShanBattleBossFrameAssistNum:SetText( GameBattle:getPlayerAssist( index ) )
	AoShanBattleBossFrameRank:SetText( index + 1 )
end

function AoShanBattleBossFrame_OnHPEnter()
	local id = this:GetClientID()
	if id == 1 then
		if boss1 then
			local actor = ActorMgr:FindActor(boss1)
			if actor then
				SetGameTooltips(this:GetName(), string.format("%d", actor:getHP(), actor:getMaxHP()))
			end
		end
	elseif id == 2 then
		if boss2 then
			local actor = ActorMgr:FindActor(boss2)
			if actor then
				SetGameTooltips(this:GetName(), string.format("%d", actor:getHP(), actor:getMaxHP()))
			end
		end
	end
end

--------------------------------------------------绿洲战场排行榜右键界面--------------------------------------
function BattleSummaryItemTemplate_OnClick()
	if arg1 == "RightButton" then
		if not GameBattle:isRun() then return end
		local name= this:GetName()	
		local n = string.sub( name, 18, string.len( name ) )
		local index = BattleSummaryFrameScrollBar:GetValue() + n
		local playerName = GameBattle:getPlayerName( index - 1 )
		BattlePlayerInfoRightMenuFrame:Show()
		BattlePlayerInfoRightMenuFrame:SetClientString( playerName )
		BattlePlayerInfoRightMenuFrame:SetPoint( "topleft", "$parent", "topleft", GetCursorPosX(), GetCursorPosY() )
	elseif arg1 == "LeftButton" then
		BattlePlayerInfoRightMenuFrame:Hide()
	end
end

--右键菜单列表
local t_BattlePlayerInfoRightMenu
--设置右键菜单
function SetBattleRightMenu()
	t_BattlePlayerInfoRightMenu = {
		[1] = { text = "观察玩家", func = function () 
									BattlePlayerInfo_WatchBtn_OnClick(); 
								end },
	}
end
function BattlePlayerInfoRightMenuFrame_OnShow()
	BattlePlayerInfoRightMenuFrame:SetClientString( "" )
	SetBattleRightMenu();
	local MenuNum = table.getn( t_BattlePlayerInfoRightMenu )
	for i = 1, MenuNum do
		local btnName = getglobal( "BattlePlayerInfoRightMenu"..i.."Name" )
		btnName:SetText( t_BattlePlayerInfoRightMenu[i].text )
	end
	BattlePlayerInfoRightMenuFrame:SetSize( 90, 16*MenuNum+10 )
end
function BattlePlayerInfoRightMenuFrame_OnUpdate()
	if not GameBattle:isRun() then
		this:Hide()
	end
end
function BattlePlayerInfoListMenu_OnClick()
	local index = this:GetClientID()	
	t_BattlePlayerInfoRightMenu[index].func()
end
function BattlePlayerInfo_WatchBtn_OnClick()
	local name = BattlePlayerInfoRightMenuFrame:GetClientString()
	if name ~= "" then
		GameFriendManager:RequestWatchFriend( name )
	end
	BattlePlayerInfoRightMenuFrame:Hide()
end
