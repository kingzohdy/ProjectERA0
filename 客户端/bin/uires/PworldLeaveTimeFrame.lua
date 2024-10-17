
local nLeaveTime = 0
local nLastTime = 0

local szHour   = "";
local szMinute = "";
local szSecond = "";

function  PworldLeaveTimeFrame_OnShow()
	--LeaveTimeFont:SetScrollNumber(1,60,60);
	--LeaveTimeFont:SetScrollNumberWithUint(1,10000,1);
	--local szText = "S";
	--"K".."L".."M".."N".."O"
	--LeaveTimeTextFont:SetText(szText);
end

--用于其它面板获取副本剩余时间
function GetPworldLeaveTime()
	if MapShower:getRealMapId() >= MIN_COPY_ID then
		if nLeaveTime > 0 then
			return nLeaveTime
		end
	end
	return 0
end

function  PworldLeaveTimeFrame_OnLoad()
	this:RegisterEvent("GE_ENTER_PWORLD");
	this:RegisterEvent("GE_REENTER_PWORLD");
	this:RegisterEvent("GE_LEAVE_PWORLD");
	this:RegisterEvent("GE_STOP_PWORLD_TIME");
end

-- 此为不用显示离开时间的计时面板
local t_notShowPworldLeaveFrame = { CLAN_CITY_MAP_ID };
function  PworldLeaveTimeFrame_OnEvent()
	if arg1 == "GE_ENTER_PWORLD" or arg1 == "GE_REENTER_PWORLD" then
		--尖峰时刻副本特殊处理时间
		if GamePoly:isInPloyPworld_MachineWar() then
			local player = ActorMgr:getMainPlayer();
			if ( player == nil ) then return end
			if  player:getPworldLeaveTime() < 0 then
				return;
			end
			
			local leaveTime = GamePoly:getMachineWarTimeOut()*60 - (getServerTime().sec - player:getPworldStartTime())
		
			local MaxMapControl	= MapShower:GetMaxMapContal();
			local nReferMapId	= MaxMapControl:getMapID();
			local nRealMapId	= MapShower:getRealMapId();
		
			-- 设置地图的纹理
			local nTexID = ( nReferMapId == 0 ) and nRealMapId or nReferMapId;
			for _, mapID in ipairs( t_notShowPworldLeaveFrame ) do
				if mapID == nTexID then
					PworldLeaveTimeFrame:Hide();
					return;
				end
			end
			
			LeaveTimeTextFont:SetText("副本剩余时间")
			LeaveTimeFont:SetText(string.format("%02d:%02d:%02d", math.floor(leaveTime / 3600), math.floor((leaveTime % 3600) / 60), leaveTime % 60))
			PworldLeaveTimeFrame:Show();
		else
			local player = ActorMgr:getMainPlayer();
			if ( player == nil ) then return end
			if  player:getPworldLeaveTime() < 0 then
				return;
			end

			nLeaveTime = player:getPworldLeaveTime();
			nLastTime = os.clock()
		
			local MaxMapControl	= MapShower:GetMaxMapContal();
			local nReferMapId	= MaxMapControl:getMapID();
			local nRealMapId	= MapShower:getRealMapId();
		
			-- 设置地图的纹理
			local nTexID = ( nReferMapId == 0 ) and nRealMapId or nReferMapId;
			for _, mapID in ipairs( t_notShowPworldLeaveFrame ) do
				if mapID == nTexID then
					PworldLeaveTimeFrame:Hide();
					return;
				end
			end
			LeaveTimeTextFont:SetText("副本剩余时间")
			LeaveTimeFont:SetText(string.format("%02d:%02d:%02d", math.floor(nLeaveTime / 3600), math.floor((nLeaveTime % 3600) / 60), nLeaveTime % 60))
			PworldLeaveTimeFrame:Show();
		end
	elseif arg1 == "GE_LEAVE_PWORLD" then		
		PworldLeaveTimeFrame:Hide();
		PworldCountFrame:Hide();
		if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "SYS_POP_WINDOW" then
			MessageBoxFrame:Hide();
		end
		if CountDownFrame:IsShown() then
			CountDownFrame:Hide();
		end
	elseif arg1 == "GE_STOP_PWORLD_TIME" then
		nLeaveTime = 0
	end
end

function  PworldLeaveTimeFrame_OnUpdate()
	--尖峰时刻副本特殊处理
	if GamePoly:isInPloyPworld_MachineWar() then
		local MainPlayer = ActorMgr:getMainPlayer()
		leaveTime = GamePoly:getMachineWarTimeOut()*60 - ( getServerTime().sec - MainPlayer:getPworldStartTime() )
		if leaveTime < 0 then
			leaveTime = 0
		end
		local wholeTime = math.ceil(leaveTime)
		LeaveTimeFont:SetText(string.format("%02d:%02d:%02d", math.floor(wholeTime / 3600), math.floor((wholeTime % 3600) / 60), wholeTime % 60))
	else
		if nLeaveTime > 0 then
			local now = os.clock()
			local leaveTime = nLeaveTime - (now - nLastTime)
			nLastTime = now
			if leaveTime < 0 then
				leaveTime = 0
			end
			local wholeTime = math.ceil(leaveTime)
			if wholeTime ~= math.ceil(nLeaveTime) then
				LeaveTimeFont:SetText(string.format("%02d:%02d:%02d", math.floor(wholeTime / 3600), math.floor((wholeTime % 3600) / 60), wholeTime % 60))
			end
			nLeaveTime = leaveTime
		end
	end
end

function ShowMapEndTime( nTime )
	if MapShower:getRealMapId() < MIN_COPY_ID then
		if nTime > 0 then
			nLeaveTime = nTime
			nLastTime = os.clock()
			LeaveTimeTextFont:SetText("剩余时间")
			LeaveTimeFont:SetText(string.format("%02d:%02d:%02d", math.floor(nLeaveTime / 3600), math.floor((nLeaveTime % 3600) / 60), nLeaveTime % 60))
			PworldLeaveTimeFrame:Show();
		else
			PworldLeaveTimeFrame:Hide();
		end
	end
end

local ntime,szMsg,startTime
function SetCountDownInfo( time,msg )
	if time == 0 then
		CountDownFrame:Hide()
		return
	end
	if MapShower:getRealMapId() < MIN_COPY_ID then
		CountDownFrame:Hide()
		return
	end
	ntime,szMsg,startTime = time,msg,GameMgr:getTickTime()
	local nTime = ntime
	local szText = szMsg
	if nTime > 3600 then
		szText = math.floor( nTime/3600 ) .. "小时"
		nTime = nTime%3600
	end
	if nTime > 60 then
		szText = szText .. math.floor( nTime/60 ) .. "分"
		nTime = nTime%60
	end
	if nTime > 0 then
		szText = szText .. nTime .."秒"
	end
	CountDownFrameFont:SetText( szText )
	CountDownFrame:Show()
end

function CountDownFrame_OnUpdate()
	 local nTime = ntime - math.floor( ( GameMgr:getTickTime() - startTime ) / 1000 )
	 if nTime <= 0 then
		this:Hide()
	else
		local szText = szMsg
		if nTime > 3600 then
			szText = math.floor( nTime/3600 ) .. "小时"
			nTime = nTime%3600
		end
		if nTime > 60 then
			szText = szText .. math.floor( nTime/60 ) .. "分"
			nTime = nTime%60
		end
		if nTime > 0 then
			szText = szText .. nTime .."秒"
		end
		CountDownFrameFont:SetText( szText )
	end
end

function SetPworldCountNum( num )
	if MapShower:getRealMapId() >= MIN_COPY_ID then
		PworldCountFrameFont:SetText( "分数：" .. num );
		if not PworldCountFrame:IsShown() then
			PworldCountFrame:Show();
		end
	end
end