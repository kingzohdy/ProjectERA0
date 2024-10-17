
function ShowConvoDistanceInfo( text )
	local mainplayer = ActorMgr:getMainPlayer()
	local convoyInfo = mainplayer:getConvoyInfo();
	local runState = Quest:getQuestRunState( convoyInfo.m_nQuestId )
	if ( convoyInfo.m_nMonsterID ~= 0 and mainplayer:getMonsterMachine().m_playerInfo.m_nMonsterMemID == convoyInfo.m_nMonsterID )
		or runState == CLIENT_TASK_STAT_NO or runState == CLIENT_TASK_STAT_FINI then
		return
	end
	ConvoyDistanceFrameRich:SetText( text, 255, 254, 190 )
	ConvoyDistanceFrame:Show();
end

function ConvoyDistanceFrameRich_OnClick()
	if arg3 == "LeftButton" then
		local mainplayer = ActorMgr:getMainPlayer()
		if mainplayer then
			local convoyInfo = mainplayer:getConvoyInfo();
			if convoyInfo.m_nMonsterMapId > 0 then
				MapShower:GetMaxMapContal():linkMoveto( convoyInfo.m_nMonsterMapId,convoyInfo.m_nMonsterPosX,convoyInfo.m_nMosnterPoxY );	
			end
		end
	end
end

function ConvoyDistanceFrameRich_OnUpdate()
	local mainplayer = ActorMgr:getMainPlayer()
	local convoyInfo = mainplayer:getConvoyInfo();
	local runState = Quest:getQuestRunState( convoyInfo.m_nQuestId )
	if not mainplayer or( convoyInfo.m_nMonsterID ~= 0 and mainplayer:getMonsterMachine().m_playerInfo.m_nMonsterMemID == convoyInfo.m_nMonsterID )
		or not convoyInfo.m_bHaveConvoy or runState == CLIENT_TASK_STAT_NO or runState == CLIENT_TASK_STAT_FINI or
		runState == CLIENT_TASK_STAT_FAIL then
		util.closeParent( this )	
	end
end