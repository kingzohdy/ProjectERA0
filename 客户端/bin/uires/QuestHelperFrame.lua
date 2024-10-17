
function QuestHelperFrame_OnLoad()
	this:RegisterEvent( "GE_QUEST_UPDATECANACCEPT" )
	this:RegisterEvent( "GE_ENTER_MAP" )
	this:RegisterEvent( "GE_PROMOTION_UPDATE" );
	this:RegisterEvent( "GE_PLAYER_LEAVEUP" );
	this:RegisterEvent( "BATTLE_LIMIT_INFO_CHG" );
	this:RegisterEvent( "GE_QUEST_DIALOG_ACCEPT" );
	this:RegisterEvent( "GE_QUEST_CHANGE" )
	this:RegisterEvent( "GE_PROMOTION_DAILY_INFO_CHG" );
	this:RegisterEvent( "GE_ENTER_PLAYERLOGIN" )

	this:RegisterEvent("GE_CLAN_SVR_INFO");
	this:RegisterEvent("GE_CLAN_DISABLEFRAME");
	QuestHelperFrameShowBtnTex:SetUVAnimation( 80,true )
	QuestHelperFrameRich1:setCenterLine( true )
	QuestHelperFrameRich2:setCenterLine( true )
end

function QuestHelperFrame_OnEvent()
	if arg1 == "GE_QUEST_UPDATECANACCEPT" or arg1 == "GE_ENTER_MAP" then
		local mapInfo = MapShower:getMapInfoByID( MapShower:getRealMapId() )
		local nMinLv,nId,nNum = 0,0,0
		for i = 1, Quest:getNumCanAcceptQuest() do			
			local nQuestId = Quest:getCanAcceptQuestId(i-1);
			if Quest:getQuestType( nQuestId ) ~= CLIENT_TASK_TYPE_DAILY_GOAL then
				local desc = Quest:getCanAcceptQuestReqDesc( nQuestId )
				if string.find( desc,"@@", 1, true ) then
					nNum = nNum + 1
					if string.find( mapInfo.m_szMapName, Quest:getQusetScene(nQuestId), 1, true ) then
						nId = nQuestId
						break
					end
					if nMinLv == 0 or nMinLv > Quest:getQuestLvLimit( nQuestId ) then
						nId = nQuestId
						nMinLv = Quest:getQuestLvLimit( nQuestId )
					end
				end
				
			end
		end
		if nNum == 0 then
			QuestHelperFrame:Hide()
		elseif not CameraCoverFrame:IsShown() then			
			QuestHelperFrame:Show()
		end
		local uvTex = getglobal( "QuestHelperFrameEffectUVAnimationTex" );
		if nId > 0 then
			local desc = Quest:getCanAcceptQuestReqDesc(nId)
			local nLeft = string.find( desc, "#L", 1, true )
			local nRight = string.find( desc, "#n", 1, true)
			if nLeft ~= nil and nRight ~= nil then
				desc = string.sub( desc, nLeft, nRight + 1 )
			end
			QuestHelperFrameRich1:SetText( "【" .. Quest:getQusetScene(nId) .. "】" .. desc .. "有事找你", 255, 255, 190 )
			QuestHelperFrameRich1:ScrollFirst()
			if Quest:getNumQusetRun() == 0 then
				uvTex:SetUVAnimation( 50, true );
				uvTex:Show();
			else
				uvTex:Hide();
			end
		else
			QuestHelperFrameRich1:SetText( "",255,255,255 )
			uvTex:Hide();
		end

		if QuestHelpFrameCheckEvent() then
			QuestHelperFrameRich2:SetLinkTextColor( 0, 255, 255 )
			QuestHelperFrameRich2:SetText( "#L正在进行的活动@@1#n", 255, 255, 190 )
		else
			QuestHelperFrameRich2:SetLinkTextColor( 0, 255, 0 )
			QuestHelperFrameRich2:SetText( "#L我还能做什么@@2#n", 255, 255, 190 )
		end

		if Quest:getNumQuestTrace() < 3 then
			ShowQuestHelperData()
		end
	end
	if arg1 == "GE_ENTER_MAP" then		
		if MapShower:getRealMapId() >= MIN_COPY_ID then
			QuestHelperFrame:Hide()
		end
	end
	if arg1 == "GE_PROMOTION_UPDATE" or arg1 == "GE_PLAYER_LEAVEUP" or arg1 == "BATTLE_LIMIT_INFO_CHG" or arg1 == "GE_PROMOTION_DAILY_INFO_CHG" then
		if QuestHelpFrameCheckEvent() then
			QuestHelperFrameRich2:SetLinkTextColor( 0, 255, 255 )
			QuestHelperFrameRich2:SetText( "#L正在进行的活动@@1#n", 255, 255, 190 )
		else
			QuestHelperFrameRich2:SetLinkTextColor( 0, 255, 0 )
			QuestHelperFrameRich2:SetText( "#L我还能做什么@@2#n", 255, 255, 190 )
		end
	end
	if arg1 == "GE_QUEST_CHANGE" then
		if Quest:getNumQuestTrace() < 3 then
			ShowQuestHelperData()
		end
	end
	if arg1 == "GE_ENTER_PLAYERLOGIN" then
		ShowQuestHelperData()
	end
	if arg1 == "GE_PLAYER_LEAVEUP" or 
		arg1 == "GE_CLAN_SVR_INFO" or 
		arg1 == "GE_CLAN_DISABLEFRAME" then
		Quest:updateCanAcceptQuest();
	end
end

function QuestHelperFrameHideBtn_OnClick()
	QuestHelperFrame:SetWidth( 17 )
	QuestHelperFrameTex:Hide()
	QuestHelperFrameRich1:Hide()
	QuestHelperFrameRich2:Hide()
	QuestHelperFrameHideBtn:Hide()
	QuestHelperFrameShowBtn:Show()
	QuestHelperFrameEffect:Hide()
end

function QuestHelperFrameShowBtn_OnClick()
	ShowQuestHelperData()
end

function QuestHelpFrameCheckEvent()
	local MainPlayer = ActorMgr:getMainPlayer()
	if MainPlayer == nil then return false end
	for i = 1,GamePromotion:getAllNum() do
		local nRow =  GamePromotion:getAllRow( i - 1 );
		local nStartInfo = GamePromotion:getCurrPromitionStartInfo( nRow );
		local promotionData = GamePromotion:getPromotionData( nRow )
		if nStartInfo.startFlag == PROMOTION_RUNING and MainPlayer:getLv() >= promotionData.m_nLevel then
			if GamePromotion:GetDynActiveInfo( nRow ) >=0 then
				if GamePromotion:GetDynActiveInfo( nRow ) == 1 then					
					return true
				end
				return false
			end
			local nFinishCount = GamePromotion:getFinishCount( nRow )
			local nRepeatCount = GamePromotion:getRepeatCount( nRow )
			
			if nFinishCount < nRepeatCount then
				return true
			end
		end
	end
	return false
end

function QuestHelpFrameRich2_OnClick()
	if arg2 == "1" then
		PromotionFrame:Show()
	elseif arg2 == "2" then
		TaskLogFrame:Show()
		TaskLogFrame_CanAcceptQuestChk_OnClick()
	end
end

function ShowQuestHelperData()
	QuestHelperFrame:SetWidth( 216 )
	QuestHelperFrameTex:Show()
	QuestHelperFrameRich1:Show()
	QuestHelperFrameRich2:Show()
	QuestHelperFrameHideBtn:Show()
	QuestHelperFrameShowBtn:Hide()
	QuestHelperFrameEffect:Show()
end