local EnterQueueTime = 0
local inviteTime = 0
local curPowrldID = 0
function ShiKongEnterApply( nPworldID )
	curPowrldID = nPworldID
	EnterQueueTime = os.clock()
	ShiKongInfoFrame:Show()
	ShiKongInfoFrame:SetClientUserData( 0, 1 )
	ShiKongInfoFrameButton:SetText( "ȡ��" )
	local pworldDef = getPworldDef( nPworldID )
	local szText = string.format( "[%s]���Ŷ�0��", pworldDef.Name )
	ShiKongInfoFrameText:SetText( szText, 255,255,190 )
end
function ShiKongInviteEnter( nPworldID )
	inviteTime = os.clock()
	curPowrldID = nPworldID
	ShiKongInfoFrame:Hide()
	ShiKongQueueTimeFrameText:setCenterLine(true)
	ShiKongQueueTimeFrame:Show()
	local pworldDef = getPworldDef( nPworldID )
	local szText = string.format( "[%s]�ѿ���������Ҫ��90s�ڽ��룡", pworldDef.Name )
	ShiKongQueueTimeFrameText:SetText( szText, 255,255,255 )
end
function ExitShiKongQueue( nPworldID, nType )
	curPowrldID = nPworldID
	if nType == SHIKONG_APPLY_QUEUE_TOLUA then
		ShiKongInfoFrame:Hide()
	elseif nType == SHIKONG_WAIT_QUEUE_TOLUA then
		ShiKongInfoFrame:Hide()
		ShiKongInfoFrame:SetClientUserData( 0, 2 )
		local pworldDef = getPworldDef( nPworldID )
		local szText = string.format( "[%s]���뵹��ʱ:90s", pworldDef.Name )
		ShiKongInfoFrameText:SetText( szText, 255,255,190 )
		ShiKongInfoFrameButton:SetText( "����" )
		ShiKongQueueTimeFrame:Hide()
	end
end
--
function ShiKongInfoFrameButton_OnClick()
	ShiKongInfoFrame:Hide()
	if ShiKongInfoFrame:GetClientUserData(0) == 1 then
		GameShiKong:ExitApplyQueue( curPowrldID )
	elseif ShiKongInfoFrame:GetClientUserData(0) == 2 then
		GameShiKong:EnterPworld( curPowrldID )
	end
end
function ShiKongInfoFrame_OnUpdate()
	if ShiKongInfoFrame:GetClientUserData(0) == 1 then
		local pastTime = os.clock() - EnterQueueTime
		local pworldDef = getPworldDef( curPowrldID )
		local szMinute = math.floor( pastTime/60 ) == 0 and "" or math.floor( pastTime/60 ).."��"
		local szText = string.format( "[%s]���Ŷ�ʱ��%s%d��", pworldDef.Name, szMinute, math.floor( pastTime%60 ) )
		ShiKongInfoFrameText:SetText( szText, 255,255,190 )
	elseif ShiKongInfoFrame:GetClientUserData(0) == 2 then
		local pastTime = os.clock() - inviteTime
		if INVITE_ENTER_SHIKONG_PWORLD_TIME_TOLUA - pastTime <= 0 then
			ShiKongInfoFrame:Hide()
		end
		local pworldDef = getPworldDef( curPowrldID )
		local szText = string.format( "[%s]���뵹��ʱ:\n%ds", pworldDef.Name, math.floor(INVITE_ENTER_SHIKONG_PWORLD_TIME_TOLUA - pastTime) )
		ShiKongInfoFrameText:SetText( szText, 255,255,190 )
	end
end
--
function ShiKongQueueTimeFrameEnterButton_OnClick()
	ShiKongQueueTimeFrame:Hide()
	GameShiKong:EnterPworld( curPowrldID )
end
function ShiKongQueueTimeFrameWaitButton_OnClick()
	ShiKongInfoFrame:Show()
	ShiKongInfoFrame:SetClientUserData( 0, 2 )
	local pworldDef = getPworldDef( curPowrldID )
	local szText = string.format( "[%s]���뵹��ʱ:90s", pworldDef.Name )
	ShiKongInfoFrameText:SetText( szText, 255,255,190 )
	ShiKongInfoFrameButton:SetText( "����" )
	ShiKongQueueTimeFrame:Hide()
end
function ShiKongQueueTimeFrame_OnUpdate()
	local pastTime = os.clock() - inviteTime
	if INVITE_ENTER_SHIKONG_PWORLD_TIME_TOLUA - pastTime <= 0 then
		ShiKongQueueTimeFrame:Hide()
	end
	local pworldDef = getPworldDef( curPowrldID )
	local szText = string.format( "[%s]�ѿ���������Ҫ��%ds�ڽ��룡", pworldDef.Name, math.floor(INVITE_ENTER_SHIKONG_PWORLD_TIME_TOLUA - pastTime) )
	ShiKongQueueTimeFrameText:SetText( szText, 255,255,255 )
end
