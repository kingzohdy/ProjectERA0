local PERFECT_REVIVE_ITEM_ID	= 5050002;
local REVIVE_NORMAL				= 0; 	--/* 普通复活 */
local REVIVE_ACCEPT_SKILL		= 1; 	--/* 接受技能复活 */
local REVIVE_PERFECT			= 2; 	--/* 完美复活 */
local REVIVE_STAND			= 3;	--/* 原地复活 */
local REVIVE_NIXI			= 4;	--/* 逆袭 */


local nLastTime = 0
local nAutoReviveTime = 0
local nStep = 0
local nTotalCount = 300
local reviveRequester
local roleid, skillid

--战勋
local nDaoBaTime = 0;
local szDaoBaClanName ="";
--local szDaoBaKillerName ="";

local t_DeadCloseFrame = 
	{
		"ArmProceStarFrame",
		"GemProcePickFrame",
	}

function ReviveFrame_OnLoad()
        this:RegisterEvent("GE_MESSAGE_DEAD");
        this:RegisterEvent("GE_ACTION_REVIVE");
	this:RegisterEvent("UI_ACTOR_LEVEL_CHG");
end

function ReviveFrame_OnEvent()
	if (arg1 == "GE_MESSAGE_DEAD") then
		local Msg = UIMSG:getUIMsg();
		nLastTime = Msg.DropData.End;
		if Msg.DropData.NiXi > 0 then
			NiXiReviveFrameKillerRich:SetText( "你已被#cff6900【"..GetPlayerName(Msg.DropData.KillerName).."】#n杀死。",255,254,189 );
			NiXiReviveFrame:Show()
		else
			if not HelperReviveFrame:IsShown() then
				if GameBCT:isIn() then
					return
				end
				local YOffSet = 0;
				ReviveFrameKillerRich:SetText( "你已被#cff6900【"..GetPlayerName(Msg.DropData.KillerName).."】#n杀死。",255,254,189 );
				
				nAutoReviveTime = Msg.DropData.AutoReviveTime;
				if Msg.DropData.DieDrop.Money == 0 and Msg.DropData.DieDrop.Exp == 0 then
					YOffSet = YOffSet + 20;
					ReviveFrameMoneyRich:SetText("",255,255,255);
					ReviveFrameDieInfoBtn:SetSize(305,50);
				else
					ReviveFrameMoneyRich:SetText("损失经验：" .. Msg.DropData.DieDrop.Exp, 255,255,190);
					ReviveFrameDieInfoBtn:SetSize(305,70);
				end
				
				local mainplayer = ActorMgr:getMainPlayer();
				local reviveDef = mainplayer:getReviveDef();
				if reviveDef == nil then return end;

				local szText = "";
				ReviveStandBtn:SetClientUserData( 0, Msg.DropData.Revive.ReviveNum );
				ReviveStandTipsBtn:SetClientUserData( 0, Msg.DropData.Revive.ReviveNum );
				ReviveStandBtn:SetClientUserData( 1, Msg.DropData.Revive.DayReviveNum );
				ReviveStandTipsBtn:SetClientUserData( 1, Msg.DropData.Revive.DayReviveNum );
				if reviveDef.StandRevive.MoneyType == ATTR_ID_BULL then
					ReviveStandCostTex:SetTexUV(78,494,20,18);
					ReviveStandCostTex:SetSize(20,18);
					ReviveStandCostTipsBtn:SetClientUserData( 0, 0 );
				elseif reviveDef.StandRevive.MoneyType == ATTR_ID_MONEY then
					ReviveStandCostTex:SetTexUV(117,494,17,18);
					ReviveStandCostTex:SetSize(17,18);
					ReviveStandCostTipsBtn:SetClientUserData( 0, 1 );
				end
				
				if Msg.DropData.Revive.DayReviveNum >= reviveDef.DayReviveTimes then
					if Msg.DropData.Revive.ReviveNum < reviveDef.ReviveTimes then
						ReviveStandCostFont:SetText( reviveDef.StandRevive.MoneyVal );
						ReviveFrame:SetClientUserData( 0, reviveDef.StandRevive.MoneyVal );
					else
						local nCostBull = 0;
						nCostBull = reviveDef.StandRevive.MoneyVal + (Msg.DropData.Revive.ReviveNum - reviveDef.ReviveTimes + 1) * reviveDef.StandIncreaseVal;
						if Msg.DropData.Revive.ReviveNum >= reviveDef.StandIncreaseNum then
							nCostBull = nCostBull * (reviveDef.StandIncreaseVal2 ^ (Msg.DropData.Revive.ReviveNum - reviveDef.StandIncreaseNum + 1));
						end
						ReviveStandCostFont:SetText( nCostBull );
						ReviveFrame:SetClientUserData( 0, nCostBull );
					end
				else
					ReviveStandCostFont:SetText( 0 );
					ReviveFrame:SetClientUserData( 0, 0 );
				end

				RevivePerfectBtn:SetClientUserData( 0, Msg.DropData.Revive.PerfectReviveNum );
				RevivePerfectTipsBtn:SetClientUserData( 0, Msg.DropData.Revive.PerfectReviveNum );
				RevivePerfectBtn:SetClientUserData( 1, Msg.DropData.Revive.DayPerfectReviveNum );
				RevivePerfectTipsBtn:SetClientUserData( 1, Msg.DropData.Revive.DayPerfectReviveNum );
				if reviveDef.PerfectRevive.MoneyType == ATTR_ID_BULL then
					RevivePerfectCostTex:SetTexUV(78,494,20,18);
					RevivePerfectCostTex:SetSize(20,18);
					RevivePerfectCostTipsBtn:SetClientUserData( 0, 0 );
				elseif reviveDef.PerfectRevive.MoneyType == ATTR_ID_MONEY then
					RevivePerfectCostTex:SetTexUV(117,494,17,18);
					RevivePerfectCostTex:SetSize(17,18);
					RevivePerfectCostTipsBtn:SetClientUserData( 0, 1 );
				end

				RevivePerfectCostFont:SetText( Msg.DropData.Revive.PerfectReviveMoney );
				ReviveFrame:SetClientUserData( 1, Msg.DropData.Revive.PerfectReviveMoney );

				if Msg.DropData.DieDrop.Count > 0 then
					szText = szText .. "掉落物品：";
					for i=1,Msg.DropData.DieDrop.Count do
						local itemDef = getItemDef( Msg.DropData.DieDrop.Items[i-1].ItemID );
						if itemDef~= nil then
							local t_NameColor = GetItemNameColor(itemDef);
							szText = szText .. RGBColorToRichColor( t_NameColor ) .. "[" .. itemDef.Name .. "]#n x " .. Msg.DropData.DieDrop.Items[i-1].Num;

							if i ~= Msg.DropData.DieDrop.Count then
								szText = szText .. "\n\t\t\t\t\t";
							end
						end					
					end
				end

				for i=1, Msg.DropData.FengYinArm.Num do
					szText = szText .. "\n被封印物品:";
					local itemDef = getItemDef( Msg.DropData.FengYinArm.Items[i-1] );
					if itemDef~= nil then
						local t_NameColor = GetItemNameColor(itemDef);
						szText = szText .. RGBColorToRichColor( t_NameColor ) .. "[" .. itemDef.Name .. "]";
						break;
					end
				end

				local rich = getglobal( "ReviveFrameDropRich" );
				rich:SetText( szText, 255,255,190 );

				local line = 0;
				if szText ~= "" then
					line = (rich:GetTextLines() == 0) and 1 or rich:GetTextLines();
					rich:SetHeight( line * 16 );
					rich:resizeText( rich:GetWidth() * GetScreenScaleY(), rich:GetHeight() * GetScreenScaleY() );
				else
					rich:SetHeight( 0 );
				end
				if Msg.DropData.DieDrop.Exp > 0 then
					ReviveFrameDropRich:SetPoint( "topleft","ReviveFrameMoneyRich","bottomleft",0,-2 );
				else
					ReviveFrameDropRich:SetPoint( "topleft","ReviveFrameMoneyRich","bottomleft",0,-20 );
				end

				ReviveFrameBkgBtnBlackTips:Hide();
				local mapDef = getMapDef( MapShower:getRealMapId() );
				if GameActorPVP:getAmok( GameActorPVP:getPkValue() ) == 2 and not bitAnd(mapDef.CtrlFlag,128) then
					ReviveFrameBkgBtnBlackTips:SetPoint( "topleft","ReviveFrameDropRich","topleft",0,1 + rich:GetHeight() );
					ReviveFrameBkgBtnBlackTips:Show();
					YOffSet = YOffSet - 15;
				end
				
				local daoBaRich = getglobal( "ReviveFrameDaoBaRich" )
				--获取刀疤消息
				ReviveFrameDaoBaBtn:Hide();
				local daoBaInfo = "";
				if Msg.DropData.DaoBaInfo.ClanID ~= 0 then
					nDaoBaTime = Msg.DropData.DaoBaInfo.End
					szDaoBaClanName = GameClanManager:clanGetClanNameByWid( Msg.DropData.DaoBaInfo.ClanWID )
					daoBaInfo ="被#cff6900【"..szDaoBaClanName.."】#n公会击败留下伤痕";
					ReviveFrameDaoBaBtn:Show();
				end
				
				local daoBaAddLines = 0
				daoBaRich:resizeText( 195*GetScreenScaleY(), 64*GetScreenScaleY() );
				daoBaRich:SetText( daoBaInfo, 255, 255, 190 )
				if daoBaInfo ~= "" then
					daoBaAddLines = (daoBaRich:GetTextLines() == 0) and 1 or daoBaRich:GetTextLines()
					line = line + daoBaAddLines;
					daoBaRich:SetHeight( daoBaAddLines*16 )
					daoBaRich:resizeText( daoBaRich:GetWidth() * GetScreenScaleY(), daoBaRich:GetHeight() * GetScreenScaleY() );
				else
					daoBaRich:SetHeight( 0 )	
				end
				
				daoBaRich:SetPoint( "top", "ReviveFrameDropRich", "bottom", -30, 0 );
				--设置刀疤图标位置
				if ReviveFrameDaoBaBtn:IsShown() then
					ReviveFrameDaoBaBtn:SetPoint( "topright", "ReviveFrameDaoBaRich", "topleft", -3, -2 );
				end
				
				YOffSet = line * 14 - YOffSet;
				ReviveFrame:SetSize(325,224 + YOffSet );
				ReviveBackground:SetSize(325,224 + YOffSet );
				ReviveFrameBkgBtn:SetSize(319,160 + YOffSet );
								
				if nAutoReviveTime == 0 then
					ReviveFrame:Show();
					if MapShower:getRealMapId() == DIANFENG_PWORLD_ID_TOLUA*100 then						
						ReviveFrame:Hide();
					end
				else
					nLastTime = 0;
					nAutoReviveTime = nAutoReviveTime + os.clock();
					CountdownReviveFrame:Show();
				end
				nStep = 0
			end
		end
		for _, szframe in ipairs( t_DeadCloseFrame ) do
			local frame = getglobal( szframe );
			if frame:IsShown() then
				frame:Hide();
			end
		end

		if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "服务器提示" then
			MessageBoxFrame:Hide();
		end
	elseif arg1 == "GE_ACTION_REVIVE" then
		ReviveFrame:Hide()
		CountdownReviveFrame:Hide()
		HelperReviveFrame:Hide()
		NiXiReviveFrame:Hide()
		if reviveRequester then
			hangup(reviveRequester)
			reviveRequester = nil
		end
	elseif arg1 == "UI_ACTOR_LEVEL_CHG" then
		local Msg = UIMSG:getUIMsg();
		local mainplayer = ActorMgr:getMainPlayer();
		local reviveDef = mainplayer:getReviveDef();
		if reviveDef == nil then return end;

		local szText = "";
		if reviveDef.StandRevive.MoneyType == ATTR_ID_BULL then
			ReviveStandCostTex:SetTexUV(78,494,20,18);
			ReviveStandCostTex:SetSize(20,18);
			ReviveStandCostTipsBtn:SetClientUserData( 0, 0 );
		elseif reviveDef.StandRevive.MoneyType == ATTR_ID_MONEY then
			ReviveStandCostTex:SetTexUV(117,494,17,18);
			ReviveStandCostTex:SetSize(17,18);
			ReviveStandCostTipsBtn:SetClientUserData( 0, 1 );
		end

		local nReviveNum = ReviveStandBtn:GetClientUserData( 0 );
		local nDayReviveNum = ReviveStandBtn:GetClientUserData( 1 );
		if nDayReviveNum > reviveDef.DayReviveTimes then
			if nReviveNum < reviveDef.ReviveTimes then
				ReviveStandCostFont:SetText( reviveDef.StandRevive.MoneyVal );
				ReviveFrame:SetClientUserData( 0, reviveDef.StandRevive.MoneyVal );
			else
				local nCostBull = 0;
				nCostBull = reviveDef.StandRevive.MoneyVal + (nReviveNum - reviveDef.ReviveTimes + 1) * reviveDef.StandIncreaseVal;
				if nReviveNum >= reviveDef.StandIncreaseNum then
					nCostBull = nCostBull * (reviveDef.StandIncreaseVal2 ^ (nReviveNum - reviveDef.StandIncreaseNum + 1));
				end
				ReviveStandCostFont:SetText( nCostBull );
				ReviveFrame:SetClientUserData( 0, nCostBull );
			end
		else
			ReviveStandCostFont:SetText( 0 );
			ReviveFrame:SetClientUserData( 0, 0 );
		end

		if reviveDef.PerfectRevive.MoneyType == ATTR_ID_BULL then
			RevivePerfectCostTex:SetTexUV(78,494,20,18);
			RevivePerfectCostTex:SetSize(20,18);
			RevivePerfectCostTipsBtn:SetClientUserData( 0, 0 );
		elseif reviveDef.PerfectRevive.MoneyType == ATTR_ID_MONEY then
			RevivePerfectCostTex:SetTexUV(117,494,17,18);
			RevivePerfectCostTex:SetSize(17,18);
			RevivePerfectCostTipsBtn:SetClientUserData( 0, 1 );
		end

		RevivePerfectCostFont:SetText( Msg.DropData.Revive.PerfectReviveMoney );
		ReviveFrame:SetClientUserData( 1, Msg.DropData.Revive.PerfectReviveMoney );
	end
end

function HelperReviveFrame_OnLoad()
	this:RegisterEvent("GE_SKILL_REVIVE");
	this:RegisterEvent( "GE_UPDATE_ROLELIST" )
end

function HelperReviveFrame_OnEvent()
	if (arg1 == "GE_SKILL_REVIVE") then
		ReviveFrame:Hide();
		CountdownReviveFrame:Hide();
		NiXiReviveFrame:Hide()
		HelperReviveFrame:Show();
		local Msg = UIMSG:getUIMsg();
		roleid, skillid = Msg.ReviveInfo.m_nPlayerId, Msg.ReviveInfo.m_nSkillId
		HelperReviveText:SetText("玩家"..GetPlayerName(Msg.ReviveInfo.m_szRoleName).."正在复活你，是否立即复活？");
		local text = "";
		if Msg.ReviveInfo.DropExp > 0 then
			text = text .. string.format( "获得掉落经验%d(%.2f%%) ",Msg.ReviveInfo.ReviveExp,Msg.ReviveInfo.ReviveExp/Msg.ReviveInfo.DropExp*100 );
		end
		HelperReviveGetText:SetText( text )
	elseif arg1 == "GE_UPDATE_ROLELIST" then
		ReviveCheckGoldPerfectBtn:setCheckState( false )
	end
end

local function countdown()
	local  diff = nLastTime - GameClanManager:getServerTime();
	if diff <= 0 then
		ReviveFrame:Hide();
		HelperReviveFrame:Hide();
		NiXiReviveFrame:Hide()
		local MainPlayer = ActorMgr:getMainPlayer();
		if MainPlayer then
			MainPlayer:revivePlayer(REVIVE_NORMAL, 0, 0);
		end
	else
		if diff <= 60 then
			ReviveFrameTimeRich:SetText("#c00dcdc".. diff .."秒#n后你的灵魂将被释放回复活点。",160,160,90);
		else
			ReviveFrameTimeRich:SetText("#c00dcdc".. math.ceil( diff/60 ) .."分钟#n后你的灵魂将被释放回复活点。",160,160,90);
		end
	end
end

function ReviveFrame_OnUpdate()
	countdown();
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end;

	if getPlayerMoney( MainPlayer ) < this:GetClientUserData( 0 ) then
		ReviveStandBtn:Disable();
		ReviveStandTipsBtn:Show();
		ReviveStandCostFont:SetTextColor( 255, 0, 0 );
	else
		ReviveStandBtn:Enable();
		ReviveStandTipsBtn:Hide();
		ReviveStandCostFont:SetTextColor( 160, 160, 90 );
	end
	
	if MainPlayer:getMoney() < this:GetClientUserData( 1 ) then
		RevivePerfectBtn:Disable();
		RevivePerfectTipsBtn:Show();
		RevivePerfectCostFont:SetTextColor( 255, 0, 0 );
	else
		RevivePerfectBtn:Enable();
		RevivePerfectTipsBtn:Hide();
		RevivePerfectCostFont:SetTextColor( 160, 160, 90 );
	end
end

function HelperReviveFrame_OnUpdate()
	
end

function HelperReviveBtnOnClick()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	MainPlayer:revivePlayer( REVIVE_ACCEPT_SKILL, roleid, skillid );
	HelperReviveFrame:Hide();
	ReviveFrame:Hide();
end

function HelperReviveCancelBtnOnClick()
	HelperReviveFrame:Hide();
	if nAutoReviveTime == 0 then
		ReviveFrame:Show();
	else
		CountdownReviveFrame:Show();
	end
end

function ReviveReleaseBtnOnClick()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	MainPlayer:revivePlayer(REVIVE_NORMAL,0,0);
	ReviveFrame:Hide();
end

--新增的金币完美复活功能
function RevivePerfectBtnOnClick()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end

	local flag =  ReviveCheckGoldPerfectBtn:GetCheckState() and 1 or 0

	MainPlayer:revivePlayer(REVIVE_PERFECT,0,0,flag);
end

function ReviveGoldPerfectCostTexOnEnter()
	SetGameTooltips( this:GetName(), "金币" )
end

function ReviveCheckGoldPerfectBtn_OnUpdate()
	if this:GetCheckState() then
		ReviveGoldPerfectCostTipsBtn:Show()
		ReviveCheckGoldPerfectBtnCostFont:Show()
		ReviveCheckGoldPerfectBtnCostTex:Show()
		ReviveCheckGoldPerfectBtnBackTex:Show()
		if not ReviveCheckGoldPerfectBtnUVAnimationTex:IsShown() then
			ReviveCheckGoldPerfectBtnUVAnimationTex:SetUVAnimation( 80, true )
			ReviveCheckGoldPerfectBtnUVAnimationTex:Show()
		end
	else
		ReviveGoldPerfectCostTipsBtn:Hide()
		ReviveCheckGoldPerfectBtnCostFont:Hide()
		ReviveCheckGoldPerfectBtnCostTex:Hide()
		ReviveCheckGoldPerfectBtnBackTex:Hide()
		ReviveCheckGoldPerfectBtnUVAnimationTex:Hide()
	end
end

function ReviveStandBtnOnClick()
	if CostMoneyConfirmFrame:IsShown() then
		return;
	end
	if checkNeedWarnCost( ReviveFrame:GetClientUserData( 0 ) ) then
		CostMoneyConfirmFrame:SetClientString("原地复活");
		CostMoneyConfirmFrame:SetClientUserData( 0, REVIVE_STAND );
		SetWarnFrame( "ReviveFrame" );
		CostMoneyConfirmFrame:Show();
		return;
	end
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	MainPlayer:revivePlayer(REVIVE_STAND,0,0);
end

function ReviveDaoBaBtnOnEnter()
	local clanName = szDaoBaClanName;
--	local name = szDaoBaKillerName;
	local timer = nDaoBaTime - getServerTime().sec;
	if timer <= 0 then
		return ;
	end
	local nHour = math.floor( timer/3600 )
	local nMinute =  math.floor( ( timer - nHour*3600 ) / 60 )
	--local nSecond = math.ceil( timer - nHour*3600 - nMinute*60 )
	local ClockTime = SetClockNum( nHour )..":"..SetClockNum( nMinute )
	local szShowInfo = "#c30ffff\t\t\t伤痕将见证荣誉的时刻！\n\n#cffffbe战斗中当双方均为据点公会的成员，\n伤痕让战败者时刻铭记着重拾荣誉！\n\n#cffffbe被#cff0000"..clanName.."#cffffbe公会的成员击败\n所能获得的公会贡献度将受到削减\n\n#c00ff00愈合时间:"..ClockTime.."\n\n#cff7404击杀对方公会成员，可提前愈合"
	SetGameTooltips( this:GetName(),  szShowInfo)
end

function RevivePerfectBtnOnEnter()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end;
	local reviveDef = MainPlayer:getReviveDef();
	if reviveDef == nil then return end;

	local szText;
	if this:GetClientUserData( 1 ) >= reviveDef.DayPerfectReviveTimes then
		szText = "完美复活原地回满生命值和魔法值；\n";
		if this:GetClientUserData( 0 ) < reviveDef.PerfectIncreaseNum or reviveDef.IncreaseVal == 0 then
			szText = szText .. "已经连续【完美复活】了#c00dcdc" .. this:GetClientUserData( 0 ) .. "#n次。";
		else
			szText = szText .. "已经连续【完美复活】了#c00dcdc" .. this:GetClientUserData( 0 ) .. "#n次，#c00dcdc继续复活将要支付高额费用。"
		end
	else
		szText = "完美复活原地回满生命值和魔法值；\n每天可获得#c00dcdc" .. reviveDef.DayPerfectReviveTimes .. "#n次免费【完美复活】机\n会，已使用#c00dcdc" .. this:GetClientUserData( 1 ) .."#n次。";
	end
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function ReviveStandBtnOnEnter()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end;
	local reviveDef = MainPlayer:getReviveDef();
	if reviveDef == nil then return end;

	local szText;
	if this:GetClientUserData( 1 ) >= reviveDef.DayReviveTimes then
		szText= "复活时恢复#c00dcdc" .. reviveDef.HPMPRate .. "%#n的生命和魔法值，绑银不足时将会使用银币；\n";
		if this:GetClientUserData( 0 ) < reviveDef.StandIncreaseNum or reviveDef.StandIncreaseVal == 0 then
			szText = szText .. "已经连续【原地复活】了#c00dcdc" .. this:GetClientUserData( 0 ) .. "#n次。";
		else
			szText = szText .. "已经连续【原地复活】了#c00dcdc" .. this:GetClientUserData( 0 ) .. "#n次，#c00dcdc继续复活将要支付高额费用。"
		end
	else
		szText = "复活时恢复#c00dcdc" .. reviveDef.HPMPRate .. "%#n的生命和魔法值；\n每天可获得#c00dcdc" .. reviveDef.DayReviveTimes .. "#n次免费【原地复活】\n机会，已使用#c00dcdc" .. this:GetClientUserData( 1 ) .."#n次。"
	end

	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function ReviveReleaseBtnOnEnter()
	local szText = "回复活点复活，且连续原地复活和连续完美复活的次数也将#c00dcdc重置为零#n。";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

local function repeatRevive()
	while true do
		local MainPlayer = ActorMgr:getMainPlayer()
		if MainPlayer then
			MainPlayer:revivePlayer(REVIVE_NORMAL, 0, 0)
		end
		coroutine.yield(1)
	end
end

function CountdownReviveFrameOnUpdate()
	local left = nAutoReviveTime - os.clock()
	if left <= 0 then
		nAutoReviveTime = 0
		CountdownReviveFrame:Hide()
		if nLastTime > GameClanManager:getServerTime() then
			ReviveFrame:Show()
		else
			if not reviveRequester then
				reviveRequester = timer(repeatRevive)
			end
		end
	elseif left <= 60 then
		CountdownReviveFrameText:SetText(string.format("%2d秒", left))
	else
		CountdownReviveFrameText:SetText(string.format("%2d分", math.ceil(left / 60)))
	end
end

local t_moneyTips = {
			[0] = "优先消耗绑银，绑银\n不足时将消耗银币",
			[1] = "银币",
			};

function ReviveMoneyTexOnEnter()
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = t_moneyTips[this:GetClientUserData(0)], frame = this:GetParent(),
						button = this:GetName() } );
end

function NiXiReviveFrame_OnLoad()
	NiXiReviveFrameDescRich:SetText( "复活后将有 #cffc85a[逆袭！！！]#n 效果，本次原地复活免费",160, 160, 90 )
end

function NiXiReviveFrame_OnUpdate()
	local  diff = nLastTime - getServerTime().sec
	if diff <= 0 then
		ReviveFrame:Hide()
		HelperReviveFrame:Hide()
		NiXiReviveFrame:Hide()
		local MainPlayer = ActorMgr:getMainPlayer()
		if MainPlayer then
			MainPlayer:revivePlayer(REVIVE_NORMAL, 0, 0);
		end
	else
		if diff <= 60 then
			NiXiReviveFrameTimeRich:SetText("#c00dcdc".. diff .."秒#n后你的灵魂将被释放回复活点。",160,160,90);
		else
			NiXiReviveFrameTimeRich:SetText("#c00dcdc".. math.ceil( diff/60 ) .."分钟#n后你的灵魂将被释放回复活点。",160,160,90);
		end
	end
end

function NiXiRevieFrameNiXiBtn_OnClick()
	local MainPlayer = ActorMgr:getMainPlayer()
	if MainPlayer == nil then return end
	MainPlayer:revivePlayer(REVIVE_NIXI,0,0)
	util.closeParent( this )
end