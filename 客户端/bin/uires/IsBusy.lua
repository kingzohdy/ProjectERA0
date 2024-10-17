local BusyExpItem 	= 0;
local BusyFiniType 	= 0;
local BusyDefID 	= 0;
local BUSY_FINI_ITEM_ID = 5052899;  --我很忙免做券ID

local	BUSY_TYPE_TASK = 1;				--	任务 
local   BUSY_TYPE_TASK_RECYCLE = 2; 	-- 	环组 
local  	BUSY_TYPE_PWORLD = 3; 			-- 	副本 
local   BUSY_TYPE_HAITAN = 4;			--	海滩

function IsBusyFrame_OnShow()
	this:SetPoint("center","$parent","center",0,0);
	local frame		= getglobal("IsBusyFrame");
		BusyDefID	= frame:GetClientUserData(0);
	local useNum	= BusyMgr:GetUsedNum() + 1;
	local nameFont	= getglobal("IsBusyFrameFont");
	local busyDef	= BusyMgr:getBusyDef(BusyDefID);
	if busyDef == nil then return end
	
	nameFont:SetText("直接完成【#B"..busyDef.Name.."#n】",255,255,190);
	--默认选择金币
	IsBusyFrame_NeedMoneyChk:setCheckState( false );
	IsBusyFrame_NeedItemChk:setCheckState( true );
	BusyFiniType = 1;
	BusyExpItem  = 0;

	--经验果设置
	HandleExpItemCheckBtn( busyDef.Type );
	--活动是否可以使用免做券
	--CheckNeedItemCheckBtn( busyDef, useNum);
	--检查付费模式
	CheckBusyFramePay( useNum ,BusyDefID );
	--tips第一条设置
	SetFirstTip( busyDef );
	
	local numFont = getglobal("IsBusyFrame_TipsBtnFont");
	numFont:SetText("今天第"..useNum.."次");
end 
function IsBusyFrame_OnHide()
	BusyExpItem 	= 0;
	BusyFiniType 	= 0;
end
function SetFirstTip( BusyDef )
	local font = getglobal("IsBusyFrameTipsFont1");
	
	if BusyDef.Type == BUSY_TYPE_PWORLD then 
		font:SetText("完成后获得该活动及其日常任务的经验/绑银/声望奖励。");
	elseif BusyDef.Type == BUSY_TYPE_HAITAN then
		font:SetText("完成后获得该活动的经验奖励。");
	else 
		font:SetText("完成后获得该活动对应任务的所有奖励。");		
	end
end
function HandleExpItemCheckBtn( BusyType )
	local expChk1 = getglobal("IsBusyFrame_ItemBtn1");
	local expChk2 = getglobal("IsBusyFrame_ItemBtn2");
	expChk1:Enable();
	expChk2:Enable();
	if  BusyExpItem == 0  then  
		expChk1:setCheckState( false );		
		expChk2:setCheckState( false );
	end
	if BusyType == 3 then 
		expChk1:ChangeNormalTexture( 105, 300, 21, 21 );
		expChk2:ChangeNormalTexture( 105, 300, 21, 21 );
		if BusyExpItem == 1 then 
			expChk1:setCheckState( true );		
			expChk2:setCheckState( false );
		elseif  BusyExpItem == 2 then 
			expChk1:setCheckState( false );		
			expChk2:setCheckState( true );
		end
	else 
		expChk1:Disable();
		expChk2:Disable();
		expChk1:ChangeNormalTexture( 149, 300, 21, 21 );
		expChk2:ChangeNormalTexture( 149, 300, 21, 21 );
	end
end 

function CheckNeedItemCheckBtn( busyDef,nNum )
	local Btn1 = getglobal("IsBusyFrame_NeedMoneyChk");
	local Btn2 = getglobal("IsBusyFrame_NeedItemChk");

	if busyDef.CostItemNum > 0 then 
		Btn2:Enable();
		Btn2:ChangeNormalTexture( 105, 300, 21, 21 );
	else 
		Btn2:Disable();
		Btn2:ChangeNormalTexture( 149, 300, 21, 21 );
	end 
end

function CheckBusyFramePay( nNum ,busyID )
	local monFont		= getglobal("IsBusyFrameMoneyFont");
	local itmFont		= getglobal("IsBusyFrameItemFont");
	local busyUseDef	= BusyMgr:getBusyUsedDef(nNum);
	if busyUseDef == nil then return end
	
	local busyDef		= BusyMgr:getBusyDef(busyID);
	if busyDef == nil then return end
	local TotalMoney;
	local TotalItem = busyDef.CostItemNum + busyUseDef.ItemNum;
	if BusyExpItem == 1 then 
		TotalMoney = busyDef.CostMoney.MoneyNum + busyDef.CostMoney15 + busyUseDef.MoneyVal ;
	elseif BusyExpItem == 2 then 
		TotalMoney = busyDef.CostMoney.MoneyNum + busyDef.CostMoney20 + busyUseDef.MoneyVal ;
	else 
		TotalMoney = busyDef.CostMoney.MoneyNum + busyUseDef.MoneyVal ;	
	end

	monFont:SetText(TotalMoney); 
	itmFont:SetText(TotalItem);
	
--	CheckBusyFrameTip();
end

function CheckBusyFrameTip()
	local tipFont 		= getglobal("IsBusyFrameUseNumFont");
	local useNum		= BusyMgr:GetUsedNum() + 1;
	local busyUseDef	= BusyMgr:getBusyUsedDef(useNum);
	if busyUseDef == nil then return end
	
	if BusyFiniType == 0 then
		tipFont:SetText("4.同天多次使用费用增加；第"..useNum.."次，增加"..busyUseDef.ItemNum.."金币")
	else 
		tipFont:SetText("4.同天多次使用费用增加；第"..useNum.."次，增加"..busyUseDef.ItemNum.."个替身机器人")
	end 
end

function BusyFrame_ItemChkOneClick()
	local ExpItem1  = IsBusyFrame_ItemBtn1:GetCheckState();
	local ExpItem2	= IsBusyFrame_ItemBtn2:GetCheckState();
	if ExpItem1 then 
		BusyExpItem = 1;
	else 
		BusyExpItem = 0;
	end
	if ExpItem1 and ExpItem2 then 
		IsBusyFrame_ItemBtn2:setCheckState(false);
	end
	BusyFrameUseMoney( BusyExpItem );
end

function BusyFrame_ItemChkTwoClick()
	local ExpItem1  = IsBusyFrame_ItemBtn1:GetCheckState();
	local ExpItem2	= IsBusyFrame_ItemBtn2:GetCheckState();
	if ExpItem2 then 
		BusyExpItem = 2;
	else 
		BusyExpItem = 0;
	end
	if ExpItem1 and ExpItem2 then 
		IsBusyFrame_ItemBtn1:setCheckState(false);
	end
	BusyFrameUseMoney( BusyExpItem );
end
--使用经验果 必须金币
function BusyFrameUseMoney( BusyExpItem )
	IsBusyFrame_NeedItemChk:setCheckState( false );
	IsBusyFrame_NeedMoneyChk:setCheckState( true );
	BusyFiniType = 0;
	local useNum	= BusyMgr:GetUsedNum() + 1;
	CheckBusyFramePay( useNum ,BusyDefID );
end

function BusyFrame_NeedMoneyClick()
	local itemChk = getglobal("IsBusyFrame_NeedItemChk");
	this:setCheckState( true );
	itemChk:setCheckState( false );
	BusyFiniType = 0;
--	CheckBusyFrameTip();
	local busyDef	= BusyMgr:getBusyDef(BusyDefID);
	HandleExpItemCheckBtn(  busyDef.Type );
	
	local useNum	= BusyMgr:GetUsedNum() + 1;
	CheckBusyFramePay( useNum ,BusyDefID );
end

function BusyFrame_NeedItemClick()
	local moneyChk = getglobal("IsBusyFrame_NeedMoneyChk");
	this:setCheckState( true );
	moneyChk:setCheckState( false );
	BusyFiniType = 1;
--	CheckBusyFrameTip();
	BusyExpItem = 0;
	--使用免做券后 不允许使用"经验果"
	HandleExpItemCheckBtn( 1 );
end

function BusyFrame_ConfirmOnClick()
	if NeedingConditionInspect() then 
		BusyMgr:IsBusyTaskCommit( BusyDefID,BusyExpItem,BusyFiniType );
		PromotionFrameBusyBtn_OnClick();
	end
	IsBusyFrame:Hide();  
end

function NeedingConditionInspect()
	--金币、免做券判断
	local monFont		= getglobal("IsBusyFrameMoneyFont");
	local itmFont		= getglobal("IsBusyFrameItemFont");
	local monNum 		= tonumber( monFont:GetText() );
    local needItNum 	= tonumber( itmFont:GetText() );
    
	local mainplayer	= ActorMgr:getMainPlayer();
	if mainplayer == nil then return false; end
	local goldNum = mainplayer:getGodCoin();
	
	local container	= mainplayer:getContainer();
	if container == nil then return false; end
	local itmNum = container:getItemCount( BUSY_FINI_ITEM_ID );
	
	local busyDef	= BusyMgr:getBusyDef(BusyDefID);
	if busyDef == nil then 
		return false;
	end	
	if HaveDoneBusyPromotion() then 
		return false;
	end
	
	if BusyFiniType == 0 and goldNum < monNum then
		ExchangePointFrame:SetClientUserData(0,1);
		MarketManager:requestQueryTianyuan();
		return false ;
	elseif  BusyFiniType == 1 and itmNum < needItNum then
		MessageBoxForVip( "提示", "您的替身机器人数量不足，是否使用" .. monNum .. "金币支付" );
		MessageBoxFrameForVip:SetClientUserData(0, BusyDefID);
		MessageBoxFrameForVip:SetClientUserData(1, BusyExpItem);
		MessageBoxFrameForVip:SetClientUserData(2, monNum);
		MessageBoxFrameForVipAcceptButton:SetText("金币支付");
		return false ;
	end
    
	if busyDef.AwardItem.ItemID > 0 and container:getUnusedGridNum( CONTAINER_TYPE_SUNDRIES ) == 0 then
		ShowMidTips( "背包无法容纳更多物品，请整理背包" );
		return false;
	end
	
	if CalculateLevelExpFull() then 
		return false;
	end
	local lvFeng = GetFengYinData();
	if lvFeng > 0 and mainplayer:getLv() > lvFeng then
		ShowMidTips( "当前等级大于封印等级，无法使用" );
		return false;
	end
	return true;
end

function HaveDoneBusyPromotion()
	local busyDef		= BusyMgr:getBusyDef(BusyDefID);
	if busyDef == nil then return true; end
	local busyType		= busyDef.Type;

	local beachTime		= BusyMgr:GetBeachTime();
	local state;
	if busyType == BUSY_TYPE_HAITAN then 
		if  beachTime > 0 then  
			ShowMidTips( "只有未进行的活动可以直接完成" );
			return true;
		end
	elseif busyType == BUSY_TYPE_TASK then 
		state = Quest:getQuestRunState( busyDef.TaskID );
		if state > CLIENT_TASK_STAT_NO then 
			ShowMidTips( "任务正在进行中不能直接完成" );
			return true;
		end
	elseif busyType == BUSY_TYPE_TASK_RECYCLE then
		local recyId	= busyDef.TaskRecycleID;
		local recycle 	= Quest:getQuestRunByRecycle( recyId ) ;
		if recycle ~= nil then 
			ShowMidTips( "任务正在进行中不能直接完成" );
			return true;
		end
		local finiTimes = Quest:getTaskRecycleIdx( recyId );
		if finiTimes > 1 then 
			ShowMidTips( "环组任务正在进行中不能直接完成" );
			return true;
		end
	end
	return false;
end

function CalculateLevelExpFull()
	local mainplayer	= ActorMgr:getMainPlayer();
	local levelup	 	= getLevelUP( mainplayer:getLv() )
	local lvRate 		= GetLevelExpCaculateRate();
	
	local busyDef	= BusyMgr:getBusyDef(BusyDefID);
	if busyDef == nil then 
		return false;
	end	

	local busyExp = busyDef.Exp;
	if BusyExpItem == 0 then 
		busyExp = ( busyExp + levelup.ContributeExp / 10000 * busyDef.ExpMul ) * lvRate;
	elseif BusyExpItem == 1 then 
		busyExp = ( busyExp + levelup.ContributeExp / 10000 * ( busyDef.ExpMul + busyDef.ExpMul15 ) ) * lvRate ;	
	else 
		busyExp = ( busyExp + levelup.ContributeExp / 10000 * ( busyDef.ExpMul + busyDef.ExpMul20 ) ) * lvRate ;
	end

	local nCurExp			= mainplayer:getExp() + busyExp ;
	local nLevelNextExp		= levelup ~= nil and levelup.LevelNextExp or 0
	if busyExp == 0 then 
		return false;
	elseif levelup.ExpLeiji > 0 and nCurExp > levelup.ExpLeiji * nLevelNextExp then
 		AddGameMiddleTips2( "经验已达到上限，请升级后再尝试" );
 		return true;
    end 
    return false;
end
function GetLevelExpCaculateRate()
	local lvFeng = GetFengYinData();
	local lvRate = 100;
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer ~= nil then
		-- 被封印了
		if MainPlayer:getLv() == lvFeng then
			local fengyinDef = getLevelFeng( lvFeng )
			if fengyinDef == nil then
				lvRate = 100;
			else
				local levelup = getLevelUP( MainPlayer:getLv() )
				local bfound  = false;
				if levelup ~= nil then
					local nCount = math.ceil( MainPlayer:getExp()/levelup.LevelNextExp )
					if nCount == 0 then
						nCount = 1
					end
					for i=1,MAX_LEV_FENG_EXPDEC_TOLUA do
						if nCount > 0 and fengyinDef.ExpRateDec[i-1].ExpCount == nCount then
							lvRate = fengyinDef.ExpRateDec[i-1].ExpRate;
							bfound  = true;
							break;
						end
					end
				end
				if not bfound then
					lvRate = 100;
				end
			end
		else
			local topLv = GetTopOneLv();
			if MainPlayer:getLv() > 40 and MainPlayer:getLv() ~= 59 and MainPlayer:getLv() < topLv then
				local levelExpDef = MainPlayer:getLevelExpDef( topLv );
				local lv = (topLv - MainPlayer:getLv()) > 80 and 80 or (topLv - MainPlayer:getLv());
				local num = 100;
				if levelExpDef ~= nil then
					num = num + levelExpDef.ExpMul[lv - 1];
				end
				lvRate = num;
			else
				lvRate = 100;
			end
		end
	end
	return lvRate / 100;
end

function BusyFrame_CancelOnClick()
	local frame = getglobal("IsBusyFrame");
	frame:Hide();
end

function OpenIsBusyFrame( nId, npcId )
	local frame = getglobal( "IsBusyFrame" );
	frame:SetClientUserData(0 ,nId)
	frame:Show();
	util.testNpcDialogDistance( npcId, IsBusyFrame)
end

function HalfExpBtnOnEnter()
	local szText = "副本类活动可以选择花费金币增加获得的经验";
	SetGameTooltips(this:GetName(),szText)
end

function IsBusyFrame_TipsBtn_OnEnter()
	local szText = "1.只有未完成的活动才可以使用“我很忙”。\n2.多次使用“我很忙”，费用增加。";
	SetGameTooltips(this:GetName(),szText);
end