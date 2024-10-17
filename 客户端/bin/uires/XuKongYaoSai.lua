local battleSummarys = {}
local battleSummaryTotal = 12
local battleSummaryMap = {
	Name					= "name",
	Clan					= "clan",
	Result					= "result",
	Kill					= "hit",
	AssistKill				= "assist",
	Energy					= "energy",
	Race					= "race",
}

local battleSummaryIconMap = {
	GroupIcon = "group",
}

local sortMap = {
	XuKongBattleFrameFrameTitleName = "name",
	XuKongBattleFrameFrameTitleClan = "clan",
	XuKongBattleFrameFrameTitleRace = "raceId",
	XuKongBattleFrameFrameTitleGroup = "groupResult",
	XuKongBattleFrameFrameTitleEnergy = "energy",
	XuKongBattleFrameFrameTitleKill = "hit",
	XuKongBattleFrameFrameTitleAssist = "assist",
	XuKongBattleFrameFrameTitleResult = "result",
}

local sortElement
local sortOrder

local boss
local bossRes = 307026

local lastRemainTime, leaveTime = -1
local timeTipTimer

local function IsInXuKong()
	return GameBattle:getCurrentBattleId() == 3;
end

local t_XuKongBattleFrameRemind = {
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
	--Ҫ����
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
			local szInfo = "�����һ��ɻ�����������½�����\n"
			if killEnough then
				szInfo = szInfo.."#c46c800\t1�������ͻ�ɱ�����ﵽ25\n"				
				local uv = t_XuKongBattleFrameRemind["finish"].uv
				XuKongBattleFrameFrameRemind1:SetTexUV( uv.x, uv.y, uv.w, uv.h )
			else
				szInfo = szInfo.."#cff6464\t1�������ͻ�ɱ�����ﵽ25\n"				
				local uv = t_XuKongBattleFrameRemind["unFinish"].uv
				XuKongBattleFrameFrameRemind1:SetTexUV( uv.x, uv.y, uv.w, uv.h )
			end
			if rankEnough then
				szInfo = szInfo.."#c46c800\t2��ս�����������ﵽǰ90%"
				local uv = t_XuKongBattleFrameRemind["finish"].uv
				XuKongBattleFrameFrameRemind2:SetTexUV( uv.x, uv.y, uv.w, uv.h )
			else
				szInfo = szInfo.."#cff6464\t2��ս�����������ﵽǰ90%"	
				local uv = t_XuKongBattleFrameRemind["unFinish"].uv
				XuKongBattleFrameFrameRemind2:SetTexUV( uv.x, uv.y, uv.w, uv.h )
			end
			XuKongBattleFrameFrameRemind:SetText( szInfo, 255, 255, 190 )
		end
	end
end

local function UpdateBattleSummary()
	local v	= XuKongBattleFrame_ScrollBar:GetValue()
	local n = #battleSummarys
	local ui
	for i = 1, battleSummaryTotal do
		local btn = getglobal("XuKongInfo"..i)
		if btn then
			if i + v > n then
				btn:Hide()
			else
				btn:Show()
				local t = battleSummarys[i + v]
				for suffix, key in pairs(battleSummaryMap) do
					ui = getglobal(string.format("XuKongInfo%d%s", i, suffix))
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
					ui = getglobal(string.format("XuKongInfo%d%s", i, suffix))
					if ui then
						if t[key] then
							ui:Show()
							ui:SetTexture(t[key])
						else
							ui:Hide()
						end
					end
				end
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
			XuKongBattleFrame_LeaveTimeBtnFont:SetText(string.format("ʣ��ʱ�� %02d��%02d��%02d", hour, minute, second))
		end
		XuKongBattleFrameFrameWinFont:SetText("")
		XuKongBattleFramePostscriptText:SetText("",255,255,255)
		XuKongBattleFrameBrandTex:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" )
		XuKongBattleFrameFrameWinBrandInfoFont:SetText("")
	else
		XuKongBattleFrame_LeaveTimeBtnFont:SetText("")
	end
	--ս�������޸�
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
			race = GetCareerName( GameBattle:getPlayerCarrer(i) ),
			raceId = GameBattle:getPlayerCarrer(i),
			clan = GameBattle:getPlayerClan(i),
			hit = GameBattle:getPlayerKill(i),
			assist = GameBattle:getPlayerAssist(i),
			dead = GameBattle:getPlayerDieNum(i),
			energy = GameBattle:getPlayerEnergy(i),
			maxkill = GameBattle:getPlayerMaxContinueKill(i),
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
		XuKongBattleFrame_ScrollBar:Show()
		XuKongBattleFrame_ScrollBar:SetMaxValue(n - battleSummaryTotal)
	else
		XuKongBattleFrame_ScrollBar:Hide()
		XuKongBattleFrame_ScrollBar:SetMaxValue(0)
	end
	XuKongBattleFrame_ScrollBar:SetValue(math.min(XuKongBattleFrame_ScrollBar:GetMaxValue(), XuKongBattleFrame_ScrollBar:GetValue()))
	UpdateBattleSummary()
end

function XuKongBattleItemTemplate_OnClick()
	if arg1 == "RightButton" then
		if not GameBattle:isRun() then return end
		local n = this:GetClientID();
		local index = XuKongBattleFrame_ScrollBar:GetValue() + n
		local playerName = GameBattle:getPlayerName( index - 1 )
		BattlePlayerInfoRightMenuFrame:Show()
		BattlePlayerInfoRightMenuFrame:SetClientString( playerName )
		BattlePlayerInfoRightMenuFrame:SetPoint( "topleft", "$parent", "topleft", GetCursorPosX(), GetCursorPosY() )
	elseif arg1 == "LeftButton" then
		BattlePlayerInfoRightMenuFrame:Hide()
	end
end

function XuKongBattleItemTitle_onClick()
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

function XuKongBattleFrame_OnLoad()
	this:RegisterEvent("GE_ENTER_PLAYERLOGIN")
	this:RegisterEvent("GE_BATTLE_ENTER")
	this:RegisterEvent("GE_BATTLE_LEAVE")
	this:RegisterEvent("GE_BATTLE_END")
	XuKongBattleFrame:setUpdateTime(1)
	for i = 1, battleSummaryTotal do
		local btn = getglobal("XuKongInfo"..i)
		local t_Tex = {
			btn:GetName() .. "NameTex",btn:GetName() .. "ClanTex",btn:GetName() .. "RaceTex",
			btn:GetName() .. "GroupTex",btn:GetName() .. "EnergyTex",
			btn:GetName() .. "KillTex",btn:GetName() .. "AssistKillTex",btn:GetName() .. "ResultTex"
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
	local uv = t_XuKongBattleFrameRemind["unFinish"].uv
	XuKongBattleFrameFrameRemind1:SetTexUV( uv.x, uv.y, uv.w, uv.h )
	XuKongBattleFrameFrameRemind2:SetTexUV( uv.x, uv.y, uv.w, uv.h )
	XuKongBattleFrameFrameRemind:SetText( t_XuKongBattleFrameRemind["unFinish"].szInfo, 255, 255, 190 )
end

function XuKongBattleFrame_OnEvent()
	if arg1 == "GE_ENTER_PLAYERLOGIN" then
		leaveTime = nil
		if timeTipTimer then
			hangup(timeTipTimer)
			timeTipTimer = nil
		end
	elseif arg1 == "GE_BATTLE_ENTER" then
		if IsInXuKong() then
			BattleSummaryFrame:Hide()
			XuKongBalanceFrame:Show()
			XuKongBattleFrame_LeaveTimeBtn:Show()
			XuKongBattleFrame_ExitActivityBtn:Show()
			XuKongBattleFrame:Hide()
			XuKongBattleFrame_ScrollBar:SetValue(0)
			SetTeamPhotoShowShow(false)
			local player = ActorMgr:getMainPlayer()
			if player then
				leaveTime = player:getPworldLeaveTime()
				timeTipTimer = timer(CountTimeTipsTimer, leaveTime - 10)
				leaveTime = leaveTime + getServerTime().sec
			end
		end
	elseif arg1 == "GE_BATTLE_LEAVE" then
		if IsInXuKong() then
			leaveTime = nil
			XuKongBalanceFrame:Hide()
			SetTeamPhotoShowShow(true)
			XuKongBattleFrame_LeaveTimeBtn:Hide()
			XuKongBattleFrame_ExitActivityBtn:Hide()
			if timeTipTimer then
				hangup(timeTipTimer)
				timeTipTimer = nil
			end
		end
	elseif arg1 == "GE_BATTLE_END" then
		if IsInXuKong() then
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
				XuKongBattleFrameFrameWinFont:SetText("ʤ����")
				winGlory = GameBattle:getPlayerWinGlory(nIdx)
				winText = "ʤ����+"				
			else
				XuKongBattleFrameFrameWinFont:SetText("ʧ�ܣ�")
				winGlory = GameBattle:getPlayerFailGlory(nIdx)
				winText = "ʧ�ܣ�+"
			end
			
			local szText = "";
			if (winGlory + rankGlory + numGlory) > 0 then
				szText = "#cffffbe���������" .. (winGlory + rankGlory + numGlory)  .. "#n��#c96965a";
				if winGlory > 0 then
					szText = szText .. winText .. winGlory .. "��";
				end
				szText = szText .. " ������" .. nIdx + 1 .. "����+" .. rankGlory .. "����" .. num .."����ս��" .. numGlory .. "��#n\n"
			else
				szText = szText .. "\n"
			end
			szText = szText .. "#cffffbe��þ��飺" .. ( nExp + nFristWinExp ) .."#n"
			if nFristWinExp > 0 then
				szText = szText .. "��#c96965a�׳�ʤ������" .. nFristWinExp .. "��#n"
			end
			szText = szText .. "\n";
			XuKongBattleFramePostscriptText:SetText(szText,255,255,255)
			local brandNum = GameBattle:getPlayerRewardNum( nIdx )
			local brandFiniNum = GameBattle:getPlayerFiniNum( nIdx )
			local brandTotalFiniNum = GameBattle:getTotalCanFiniNum()
			local brandInfoFont = getglobal( "XuKongBattleFrameFrameWinBrandInfoFont" )
			brandInfoFont:SetText( "��ý��£�"..brandNum.."/3".."      (ÿ�������Ի��3�����£�" )
			local brand = GameBattle:getPlayerReward( nIdx )
			local brandTex = getglobal( "XuKongBattleFrameBrandTex" )
			brandTex:SetTexture(battleRewardIcon[ATTR_ID_AOSHAN_BRAND])
			if brandNum > 0 then
				brandInfoFont:Show()
				brandTex:Show()
			else
				brandInfoFont:Hide()
				brandTex:Hide()
			end
		end
	end
end

function XuKongBattleFrame_OnShow()	
	sortElement = "result"
	sortOrder = true
	lastRemainTime = -1
	UpdateBattleSummaryData()
end

function XuKongBattleFrame_OnUpdate()
	if BattleStarted then
		UpdateBattleSummaryData()
	end
end

function XuKongBattleFrame_OnHide()
	BattlePlayerInfoRightMenuFrame:Hide()
end

function XuKongBattleFrame_ScrollUpBtn_OnClick()
	XuKongBattleFrame_ScrollBar:SetValue(XuKongBattleFrame_ScrollBar:GetValue() - XuKongBattleFrame_ScrollBar:GetValueStep())
end

function XuKongBattleFrame_ScrollDownBtn_OnClick()
	XuKongBattleFrame_ScrollBar:SetValue(XuKongBattleFrame_ScrollBar:GetValue() + XuKongBattleFrame_ScrollBar:GetValueStep())
end

function XuKongBattleFrame_ScrollBar_OnValueChanged()
	UpdateBattleSummary()
end

function XuKongBattleFrame_ExitActivity_OnClick()
	XuKongBattleFrame:Hide()
	if BattleStarted then
		BattleLeaveConfirmFrame:Show()
	else
		GameBattle:LeavePworld()
	end
end


-------------------------------------�ȷ����--------------------------------
local TOTAL_POINT = 25
local leftPos = 0,0;
local totalDist = 0;
--20,178
function XuKongBalanceFramePosBtn_OnEnter()
	local name = this:GetName()
	if name == "XuKongBalanceFramePointBtn" then
		SetGameTooltips(name, "������Ԥ�Ƶ����λ��")
	elseif name == "XuKongBalanceFrameCurBtn" then
		SetGameTooltips(name, "��������ǰ��λ��")
	end
end

function UpdateXuKongEnergy( val1, val2 )
	if val1 ~= 0 or val2 ~= 0 or XuKongBalanceFrame:IsShown() then
		local scale = 0.5 + ( val1 - val2 ) / TOTAL_POINT / 2;
		scale = math.max( scale, 0 );
		scale = math.min( scale, 1 );
		XuKongBalanceFrameRedProcessTex:SetSize( 173*scale, 9 );
		XuKongBalanceFramePointBtn:SetPoint( "topleft", "XuKongBalanceFrame", "topleft", 20 + 158*scale, 5 );
	end
end

function XuKongBalanceFrame_OnShow()
	XuKongBalanceFrameRedProcessTex:SetSize( 173*0.5, 9 );
	XuKongBalanceFramePointBtn:SetPoint( "top", "XuKongBalanceFrame", "top", 0, 5 );
	XuKongBalanceFrameCurBtn:SetPoint( "top", "XuKongBalanceFrame", "top", 0, 5 );
end

function XuKongBalanceFrame_OnLoad()
	this:setUpdateTime(0.5);
	local pos1Def = GameBattle:getXuKongPosDef( 1, TOTAL_POINT );
	local pos2Def = GameBattle:getXuKongPosDef( 2, TOTAL_POINT );
	if pos1Def ~= nil and pos2Def ~= nil then
		leftPos = math.min( pos1Def.Pos.X, pos2Def.Pos.X );
		local rightPos = math.max( pos1Def.Pos.X, pos2Def.Pos.X );
		totalDist = rightPos - leftPos;
	end
end

function XuKongBalanceFrame_OnUpdate()
	local npcId = ActorMgr:getActorIdByResId( XUKONG_NPC_ID_TOLUA );
	if npcId ~= 0 then
		local pNpc = ActorMgr:FindActor( npcId );
		if pNpc ~= nil then
			local posX = pNpc:getPlayerX() / 10;
			local scale = ( posX - leftPos ) / totalDist;
			scale = math.max( scale, 0 );
			scale = math.min( scale, 1 );
			XuKongBalanceFrameCurBtn:SetPoint( "topleft", "XuKongBalanceFrame", "topleft", 20 + 158*scale, 5 );
			AddMiniMapMoveNpc( pNpc );
			WorldMapSetMoveNpcPosBtn( pNpc );
		end
	end
end

-------------------------------���а��Ҽ�--------------------------------

function BattleSummaryItemTemplate_OnClick()
	if arg1 == "RightButton" then
		if not GameBattle:isRun() then return end
		local name= this:GetName()	
		local n = string.sub( name, 18, string.len( name ) )
		local index = XuKongBattleFrame_ScrollBar:GetValue() + n
		local playerName = GameBattle:getPlayerName( index - 1 )
		BattlePlayerInfoRightMenuFrame:Show()
		BattlePlayerInfoRightMenuFrame:SetClientString( playerName )
		BattlePlayerInfoRightMenuFrame:SetPoint( "topleft", "$parent", "topleft", GetCursorPosX(), GetCursorPosY() )
	elseif arg1 == "LeftButton" then
		BattlePlayerInfoRightMenuFrame:Hide()
	end
end

--------------------------�´�ʧ��֮������------------------------------------
local t_GuChengBattleInfo = {
					[1] = [[
#c461e0a����ƣ��´�ʧ��֮��
#c461e0a���ͼ��ʧ��֮��
#c461e0a����ȼ�����70��]],
					[2] = [[
					
					
#c461e0a������㡾#c0a6400������#n����������ȡ�������飡
#c461e0a��Խ�߲�����������������ȡ�ľ���Խ��

#c461e0a����������Ա��]],
					[3] = [[ʧ��֮����߲㣺
��������������˹�����פ���ŵ��߲�
���ɱ����˹��������񱬴������ռ�������]],
					[4] = [[
#c0000a4������ϣ�
  �봳����Ⱥͬ�У�����󽵵�·;����
						]],
					 			}
local nGuChengTime = 0;	
local nGuChengAward = 0;
local MAX_GUCHENG_AWARD_NUM = 6;		 			
-- �´�ʧ��֮��
function GuChengActiveBegin( nEndTime,nAward )
	nGuChengTime = nEndTime;
	nGuChengAward = nAward;
	if nEndTime <= 0 then 
		CloseSpecialActivityGuid( GUCHENG_BATTLE_ACTIVITY_ID );
		if GuChengBattleFrame:IsShown() then 
			GuChengBattleFrame:Hide();
		end
		return;
	end
	SetSpecialActivityGuid( GUCHENG_BATTLE_ACTIVITY_ID );
end

function GuChengActiveEnd()
	CloseSpecialActivityGuid( GUCHENG_BATTLE_ACTIVITY_ID );
	if GuChengBattleFrame:IsShown() then 
		GuChengBattleFrame:Hide();
	end
end

local function showGuChengAwardItem()
	local promotionData = GamePromotion:getPromotionData( GUCHENG_BATTLE_ACTIVITY_ID );
	if promotionData == nil then 
		return;
	end
	local lvFeng = GetFengYinData();
	if lvFeng <= 0 and GameMgr:GetMaxLevel() > 0 then 
		lvFeng = GameMgr:GetMaxLevel();
	end
	local szText = "#c461e0a���㽱��";
	if lvFeng >= 79 then 
		szText = szText.."\n\n\n\n#c461e0a�ռ�����\n\n\n\n#c00a400 �������˾���ʰȡ���ռ���������";
	end
	GuChengBattleFrameSecondRichGift:SetText( szText, 255, 255, 255 );
	for i = 1,MAX_GUCHENG_AWARD_NUM do 
		local itemId 	= promotionData.m_RewardItem[i-1];
		local itemdef 	= getItemDef( itemId );
		local itemBtn 	= getglobal( "GuChengBattleFrameAwardItem"..i );
		local IconTexture = getglobal( "GuChengBattleFrameAwardItem"..i.."IconTexture" )
		local uvTexture = getglobal( "GuChengBattleFrameAwardItem"..i.."UVAnimationTex" )
		if lvFeng < 79 and i > 2 then
			itemBtn:Hide(); 
		elseif itemdef == nil then
			itemBtn:SetClientUserData( 0 ,0 );
			itemBtn:Hide();
			uvTexture:Hide();
		else
			local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
			-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			itemBtn:Show();
			IconTexture:SetTexture(IconPath);
			itemBtn:SetClientUserData( 0,itemId );
			SetEquipImportantEffectUV( itemdef, uvTexture:GetName() )
		end
	end	
end
function GuChengBattleFrame_OnLoad()
	this:setUpdateTime(1);
	GuChengBattleFrameLeftRichInfo:SetText( t_GuChengBattleInfo[1], 255, 255, 255 );	
end

function GuChengBattleFrame_OnEvent()

end

local function getAwardStateDes( nLayer )
	if nLayer < 1 then 
		return;
	end
	local szText = "��δ��ȡ��"
	local nValue = 2 ^ ( nLayer - 1 );
	if bitAnd( nGuChengAward, nValue ) then 
		szText = "��#c0a6400����ȡ#n��"
	end	
	return szText;
end

function GuChengBattleFrame_OnShow()
	local curTime 	= getServerTime().sec;
	local nTme		= nGuChengTime - curTime; 
	local richFont	= getglobal("GuChengBattleFrameEndTimeFont");
	
	if nTme > 0 then 
		local timeFont = SetDartEndTimeText( nTme );
		richFont:SetText( "#c461e0aʣ��ʱ�䣺#n#c0a6400 "..timeFont.."#n",46,15,10 );
	else 	
		richFont:SetText( "#c461e0aʣ��ʱ�䣺#n#c0a6400 00:00#n",46,15,10 );
	end
	local lvFeng = GetFengYinData();
	if lvFeng <= 0 and GameMgr:GetMaxLevel() > 0 then 
		lvFeng = GameMgr:GetMaxLevel();
	end
	local szText = t_GuChengBattleInfo[2].."\n\t\t��һ�㡪��#L������˾��@@[304141,3211]#n"..getAwardStateDes(1).."\n\t\t�ڶ��㡪��#L�������೤@@[304142,3212]#n"..getAwardStateDes(2).."\n\t\t���Ĳ㡪��#L�������ӳ�@@[304143,3214]#n"..getAwardStateDes(4);	
	
	if lvFeng < 74 then 
		szText = szText.."\n\n"..t_GuChengBattleInfo[4]
	elseif lvFeng < 79 then 
		szText = szText.."\n\t\t#L����������@@[304144,3215]#n"..getAwardStateDes(5).."\n\n"..t_GuChengBattleInfo[4]
	else 
		szText = szText.."\n\t\t����㡪��#L����������@@[304144,3215]#n"..getAwardStateDes(5).."\n\t\t���߲㡪��#L�������ų�@@[304145,3217]#n"..getAwardStateDes(7).."\n\n"..t_GuChengBattleInfo[3].."\n\n"..t_GuChengBattleInfo[4]
	end
	GuChengBattleFrameRightRichInfo:SetLinkTextColor( 217,106,8 );
	GuChengBattleFrameRightRichInfo:SetText( szText, 46, 15, 10 );
	showGuChengAwardItem();
end

function GuChengBattleFrame_OnUpdate()	
	local curTime 	= getServerTime().sec;
	local nTme		= nGuChengTime - curTime; 
	local richFont	= getglobal("GuChengBattleFrameEndTimeFont");
	if nTme > 0 then 
		local timeFont = SetDartEndTimeText( nTme );
		richFont:SetText( "#c461e0aʣ��ʱ�䣺#n#c0a6400 "..timeFont.."#n",46,15,10 );
	else 	
		richFont:SetText( "#c461e0aʣ��ʱ�䣺#n#c0a6400 00:00#n",46,15,10 );
	end
end
-- ���ϲμ�
function GuChengBattleFrameOkBtn_OnClick()
	if CanNotTransportOtherMap() then 
		return;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer:getLv() < 70 then 
		ShowMidTips("��������Ҫ�ټ��ﵽ70�������ߣ�����δ�ﵽ�ȼ��޷��μ�");
		return;
	end
	GamePromotion:RequestTransportToNPC( GUCHENG_BATTLE_ACTIVITY_ID );
	if GuChengBattleFrame:IsShown() then 
		GuChengBattleFrame:Hide();
	end
end

function GuChengBattleFrameAwardItemBtn_OnEnter()
	local itemId = this:GetClientUserData( 0 );
	if itemId == 0 then return end
	local itemdef = getItemDef( itemId );
	if itemdef == nil then return end
	ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, itemdef )
end