local t_RespactNodeState;	-- { { 节点名, 节点状态 } }	  节点状态( 展开true,收缩 false)
local MAX_RESPACT_NUM;
local MAX_RESPACT_VALUE;

-- 初始化声望奖章数据
function InitRespactFrame()
	t_RespactNodeState = {};
	GameRespact:clearRespactEntry();
	MAX_RESPACT_NUM = 9;
	MAX_RESPACT_VALUE = 72000;
end

function RespactFrame_OnLoad()
	InitRespactFrame();
	for i = 1, MAX_RESPACT_NUM do
		local nameBtn = getglobal( "RespactFrame_NameBtn"..i );
		nameBtn:SetClientUserData(0,0);
	end
	
	this:RegisterEvent("GE_PESPACT_UPDATE");
	this:RegisterEvent("UI_ACTOR_ATTRIBUTE_CHG");
	this:RegisterEvent("GE_CHANGE_ZHENGYI");
	this:RegisterEvent("GE_CHANGE_CHUANCHENG");
	local rich = getglobal( this:GetName() .. "DescRich" )
	rich:SetLinkTextColor( 0, 255, 0 )
	rich:SetText( "荣誉商人:#L巴菲[时间城]@@[303036,1000]#n 高级荣誉商人:#L梅杰[时间城]@@[302033,1000]#n", 255, 255, 190 )
end

function RespactFrame_OnEvent()	
	if arg1 == "GE_PESPACT_UPDATE" then	
		RespactFrame_OnShow();
	elseif arg1 == "UI_ACTOR_ATTRIBUTE_CHG" and RespactFrame:IsShown() then
		UpdateRespactGlory();
		UpdateSpartaGlory();
	elseif arg1 == "GE_CHANGE_ZHENGYI" then 
		UpdateRespactZhengYi();
	elseif arg1 == "GE_CHANGE_CHUANCHENG" then
		UpdateRespactChuanCheng();
	end
end

function RespactFrame_OnShow()
	local nAllRespactNum	= GameRespact:getRespactPartyNum();
	local nPhaseNum			= GameRespact:getRespactPhaseNum();
	local nCategoryNum		= GameRespact:getRespactCategoryNum();
	if table.getn(t_RespactNodeState) == 0 then
		for i = 1, nCategoryNum do
			local nCategoryId = GameRespact:getRespactCategoryId( i - 1 );
			table.insert( t_RespactNodeState, { nCategoryId, true, n } );
		end
	end
	
	-- 设置最大的声望值
	MAX_RESPACT_VALUE = GameRespact:getRespactPhaseDef( nPhaseNum - 1 ).NeedRespact - GameRespact:getRespactPhaseDef( nPhaseNum - 2 ).NeedRespact;

	-- 设置列表滚动条
	if ( nAllRespactNum + nCategoryNum ) > MAX_RESPACT_NUM then
		RespactFrame_ScrollBar:SetMaxValue( ( nAllRespactNum + nCategoryNum ) - MAX_RESPACT_NUM );
		RespactFrame_ScrollBar:SetValueStep(1);
		RespactFrame_ScrollBar:Show();
		RespactFrame_ScrollUpBtn:Show();
		RespactFrame_ScrollDownBtn:Show();
		RespactFrame_SliderBack:Show();
	else												  		
		RespactFrame_ScrollBar:Hide();
		RespactFrame_ScrollUpBtn:Hide();
		RespactFrame_ScrollDownBtn:Hide();
		RespactFrame_SliderBack:Hide();
	end

	BondingRespact(1);
	UpdateRespactGlory();
	UpdateSpartaGlory();
end

function GetPosIndex( nSliderValue )
	local t_PosIndex = {};
	local nCount = 1;
	for i = 1, table.getn( t_RespactNodeState ) do
		if nCount == nSliderValue then
			t_PosIndex = { i, 1, true };
			return t_PosIndex;
		end

		nCount = nCount + 1;
		if t_RespactNodeState[i][2] then
			local nPartyNum = GameRespact:getRespactPartyNum( t_RespactNodeState[i][1] );
			for j = 1, nPartyNum do
				if nCount == nSliderValue then
					t_PosIndex = { i, j, false };
					return t_PosIndex;
				end
				nCount = nCount + 1;
			end
		end
	end
	return { 1, 1, true };
end

function BondingRespact( nSliderValue )
	local nIndex		= 1;
	local nChildIndex	= 1;

	for i = 1, MAX_RESPACT_NUM do
		local NodeBtn	= getglobal( "RespactFrame_NodeBtn"..i );
		local ChildBtn	= getglobal( "RespactFrame_NameBtn"..i );  	

		NodeBtn:Hide();
		ChildBtn:Hide();
	end

	if table.getn(t_RespactNodeState) == 0 then
		return;
	end

	local t_Pos = GetPosIndex( nSliderValue );

	local bShowNode = t_Pos[3];
	for i = t_Pos[1], table.getn( t_RespactNodeState ) do
		if nIndex <= MAX_RESPACT_NUM then
			if bShowNode then
				local NodeBtn = getglobal( "RespactFrame_NodeBtn"..nIndex );
				local NodeChk = getglobal( "RespactFrame_NodeBtn"..nIndex.."Chk" );
				local NodeNameFont = getglobal( "RespactFrame_NodeBtn"..nIndex.."Name" );

				NodeBtn:Show();
				NodeChk:setCheckState( t_RespactNodeState[i][2] );
				NodeNameFont:SetText( GameRespact:getRespactCategoryName(t_RespactNodeState[i][1]) );

				nIndex = nIndex + 1;
			else
				bShowNode = true;
			end

			if t_RespactNodeState[i][2] then
				nIndex = SetRespactValue( nIndex, t_Pos[2], t_RespactNodeState[i][1] );
				t_Pos[2] = 1;
			end
		end
	end
end

function SetRespactValue( nIndex, nPartyIndex, nCategoryId )
	local nPartyNum = GameRespact:getRespactPartyNum( nCategoryId );

	for j = nPartyIndex, nPartyNum do
		if nIndex <= MAX_RESPACT_NUM then
			local ChildBtn			= getglobal( "RespactFrame_NameBtn"..nIndex );
			local ChildNameFont		= getglobal( "RespactFrame_NameBtn"..nIndex.."Name" );
			local ChildValueFont	= getglobal( "RespactFrame_NameBtn"..nIndex.."ValueFont" );   
			local ChildValueName	= getglobal( "RespactFrame_NameBtn"..nIndex.."ValueName" );
			local ChildProgress		= getglobal( "RespactFrame_NameBtn"..nIndex.."BarTex" )
			ChildBtn:Show();

			local RespactPartyDef   = GameRespact:getRespactPartyDef( nCategoryId, ( j - 1 ) );
			if RespactPartyDef == nil then
				return;
			end			
			
			if GameRespact:isHaveRespact( RespactPartyDef.ID ) then
				local t_Phase = GetRespactParseDesc( GameRespact:getRespactValue(RespactPartyDef.ID) );
				if t_Phase ~= nil then		
					local nCurRespactValue = (GameRespact:getRespactValue(RespactPartyDef.ID)-t_Phase[3]);
					if nCurRespactValue > MAX_RESPACT_VALUE	then
						nCurRespactValue = 	MAX_RESPACT_VALUE;
					end
					ChildValueName:SetText( nCurRespactValue .."/"..(t_Phase[2]-t_Phase[3]).."  " );
					ChildValueFont:SetText( " "..t_Phase[1] );
					local rate = nCurRespactValue/(t_Phase[2]-t_Phase[3]);
					if rate > 1 then rate = 1 end
					ChildProgress:SetSize( rate*145,13 );
				end
			else	   
				local PhaseDef = GameRespact:getRespactPhaseDef(0);
				local NextPhaseDef = GameRespact:getRespactPhaseDef(1);
				ChildValueName:SetText( 0 .."/"..NextPhaseDef.NeedRespact.."  " );
				ChildValueFont:SetText( " "..PhaseDef.PhaseText );
				ChildProgress:SetSize( 0,13 );
			end	

			ChildNameFont:SetText( "   "..RespactPartyDef.PartyName );	
			ChildBtn:SetClientUserData( 0, RespactPartyDef.ID );
			nIndex = nIndex + 1;
		end
	end

	return nIndex;
end

function RespactLoadingBtn_OnEnter()
	local BackBtn = getglobal( "RespactFrame_NameBtn"..this:GetClientID().."_BackBtn" );
	BackBtn:Highlight();
end

function RespactLoadingBtn_OnLeave()
	local BackBtn = getglobal( "RespactFrame_NameBtn"..this:GetClientID().."_BackBtn" );
	BackBtn:DisHighlight();
end

function GetRespactParseDesc( nValue )
	local t_Parse = {};							   
	local nNum = GameRespact:getRespactPhaseNum();
	for i = 1, nNum do
		local RespactPhaseDef = GameRespact:getRespactPhaseDef( i - 1 );
		local MaxRespactPhaseDef = GameRespact:getRespactPhaseDef( nNum - 1 );
		local NextRespactPhaseDef = nil;
		if i < nNum then
			NextRespactPhaseDef = GameRespact:getRespactPhaseDef( i );
		end

		if nValue >= MaxRespactPhaseDef.NeedRespact	then
			t_Parse = { MaxRespactPhaseDef.PhaseText, MAX_RESPACT_VALUE, RespactPhaseDef.NeedRespact };
			return t_Parse;
		end

		if nValue == RespactPhaseDef.NeedRespact and NextRespactPhaseDef ~= nil then
			t_Parse = { RespactPhaseDef.PhaseText, NextRespactPhaseDef.NeedRespact, RespactPhaseDef.NeedRespact };
			return t_Parse; 
		end

		if NextRespactPhaseDef ~= nil then
			if nValue > RespactPhaseDef.NeedRespact and nValue < NextRespactPhaseDef.NeedRespact  then
				t_Parse = { RespactPhaseDef.PhaseText, NextRespactPhaseDef.NeedRespact, RespactPhaseDef.NeedRespact };
				return t_Parse;
			end
		end
	end
	return nil;
end

function RespactFrame_OnHide()
end

-- 点击声望节点时响应
function RespactChk_OnClick()
	local szParentName = this:GetParent();
	if szParentName == "RespactFrame" then
		szParentName = this:GetName();
	end
								
	local NodeNameFont	= getglobal( szParentName.."Name" );
	local NodeChk		= getglobal( szParentName.."Chk" );
	local szNodeName	= NodeNameFont:GetText();
	local nCurIndex		= 0;
	local nTotalNum = GameRespact:getRespactCategoryNum();	   
	local nValue = 0;
	local slider = getglobal("RespactFrame_ScrollBar"); 
	for i = 1, table.getn( t_RespactNodeState ) do
	   if szNodeName == GameRespact:getRespactCategoryName(t_RespactNodeState[i][1]) then
			local nState = t_RespactNodeState[i][2];
			if nState then
				nState = false;
			else
				nState = true;
			end
			t_RespactNodeState[i][2] = nState;
			if nTotalNum - MAX_RESPACT_NUM > 0 then
				nValue = nTotalNum - MAX_RESPACT_NUM; 
			end
			slider:SetValue( nValue );
	   end
	   if t_RespactNodeState[i][2] then
			nTotalNum = nTotalNum + GameRespact:getRespactPartyNum( t_RespactNodeState[i][1] );
		end
	end

	local nAllRespactNum = GameRespact:getRespactPartyNum();
	if nTotalNum - MAX_RESPACT_NUM > 0 then
		slider:SetMaxValue( nTotalNum - MAX_RESPACT_NUM );
	end
	BondingRespact(nValue+1);
end

-- 点击声望阵营进度条时响应
function RespactLoadingBtn_OnClick()
end

function RespactFrame_ScrollBar_OnValueChanged()
	local slider = getglobal("RespactFrame_ScrollBar");
	local nValue = slider:GetValue();
	local nMaxValue = slider:GetMaxValue();

	if nValue <= nMaxValue then
		BondingRespact( nValue + 1 );
	end
end

function RespactFrame_ScrollDownBtn_OnClick()
	local slider = getglobal("RespactFrame_ScrollBar");
	local nValue = slider:GetValue() + slider:GetValueStep();
	local nMaxValue = slider:GetMaxValue();
	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

function RespactFrame_ScrollUpBtn_OnClick()
	local slider = getglobal("RespactFrame_ScrollBar");
	local nValue = slider:GetValue() - slider:GetValueStep();
	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end
function UpdateRespactGlory()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	RespactFrameValueFont1:SetText( mainplayer:getBattleGlory() );
	RespactFrameValueFont2:SetText( mainplayer:getAoShanBrand() );
	RespactFrameValueFont3:SetText( mainplayer:getLiftBrand() )
	RespactFrameChuanChengBtnValueFont:SetText( mainplayer:getSchoolVal() );
end
--计算殿堂的胜率
function GetSpartaVRate( nVictory, nDefeat )
	local totalSpartaNum = nVictory + nDefeat
	if totalSpartaNum == 0 then
		return "0%"
	else
		return math.floor( nVictory/totalSpartaNum*100 ).."%"
	end
end

function UpdateSpartaGlory()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then return end
	local nVictory = mainplayer:getSpartaVictory()
	local nDefeat = mainplayer:getSpartaDefeat()
	RespactFrameSpartaVRate:SetText( "胜率："..GetSpartaVRate( nVictory, nDefeat ) )
	RespactFrameSpartaRunAwayTimes:SetText( "逃跑：".. mainplayer:getSpartaRunAway() .."次" )
	RespactFrameSpartaHonor:SetText( "殿堂荣誉：".. mainplayer:getSpartaGlory() )
	local honorTexOffsetX = RespactFrameSpartaHonor:GetTextExtentWidth( RespactFrameSpartaHonor:GetText() ) / GetScreenScaleY() + 3
	RespactFrameSpartaHonorTex:SetPoint( "left", "RespactFrameSpartaHonor", "left", honorTexOffsetX, -3 )
	local nFightVal = mainplayer:getSpartaFightVal()
	RespactFrameSpartaFightVal:SetText( string.format( "末世功勋：%d(%s)", nFightVal, ActorMgr:GetSpartaLevelName( nFightVal ) ) )	
end

function RespactFrameGloryTexBtn1_OnEnter()
	SetGameTooltips( this:GetName(), "参加PVP战场可以获得" )
end

function RespactFrameGloryTexBtn2_OnEnter()
	local mainplayer = ActorMgr:getMainPlayer()
	local szText = "战魂徽章："..mainplayer:getAoShanBrand().."/"..MAX_AOSHAN_BRAND_TOLUA.."\n参加绿洲风暴或狂沙角斗场可以获得"
	SetGameTooltips( this:GetName(), szText )
end

function RespactFrameGloryTexBtn3_OnEnter()
	local mainplayer = ActorMgr:getMainPlayer()
	local szText = "生存奖章："..mainplayer:getLiftBrand().."/"..MAX_LIFT_BRAND_TOLUA.."\n参加日常活动可以获得"
	SetGameTooltips( this:GetName(), szText )
end

------------------------------ 传承点部分 ---------------------------

local nLastChuanCheng = 0;				-- 改变前的传承点
local ChuanChengPoint = 0;  			-- 每日获得传承点
function UpdateChuanChengPerDay( nPoint )
	ChuanChengPoint = nPoint;
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	nLastChuanCheng = mainplayer:getSchoolVal();
end

-- 更新传承点
function UpdateRespactChuanCheng()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local nCount = mainplayer:getSchoolVal();
	local nAddChuanCheng =  ( ( nCount - nLastChuanCheng ) > 0 ) and nCount - nLastChuanCheng or 0
	ChuanChengPoint = ChuanChengPoint + nAddChuanCheng;
	nLastChuanCheng = mainplayer:getSchoolVal();
end
function RespactFrameChuanChengBtn_OnEnter()
	local szText = "今日获取传承点数 "..ChuanChengPoint.."/500\n可以通过师生关系获得传承点数"
	SetGameTooltips( this:GetName(),szText );
end

function RespactFrameSpartaName_OnEnter()
	SetGameTooltips( this:GetName(), "周日19：30-23：00参与末世荣耀殿堂可额外获得末日荣耀奖励" )
end

function RespactFrameSpartaHonor_OnEnter()
	SetGameTooltips( this:GetName(), "周日19：30-23：00参与末世荣耀殿堂可获得殿堂荣誉" )
end

------------------------------------ 正义点部分 -----------------------------
local nZhengYiCount 		= 0;
local nZhengYiCountLimit 	= 0;
local nLastZhengYi			= 0;

function UpdateZhengYiPoint( nCount, nCountLimit )
	nZhengYiCount = nCount;
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local nCount = mainplayer:getZhengYiPoint();
	nZhengYiCountLimit	= nCountLimit;
	local font = getglobal("RespactFrameZhengYiBtnValueFont");
	font:SetText( nCount );
	nLastZhengYi = nCount;
end

function UpdateRespactZhengYi()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local nCount = mainplayer:getZhengYiPoint();
	local font = getglobal("RespactFrameZhengYiBtnValueFont");
	font:SetText( nCount );
	local nAddZhengYi =  ( ( nCount - nLastZhengYi ) > 0 ) and nCount - nLastZhengYi or 0
	nZhengYiCount = nZhengYiCount + nAddZhengYi;
	nLastZhengYi = nCount;
end

function RespactFrameZhengYiBtn_OnEnter()
	local szText = "今日获取正义点数 "..nZhengYiCount.."/"..nZhengYiCountLimit .. "\n参加副本和活动时可以获得"
	SetGameTooltips( this:GetName(),szText );
end
