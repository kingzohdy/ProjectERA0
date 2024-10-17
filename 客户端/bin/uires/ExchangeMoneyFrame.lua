local nNpcID		= 0;
local nCost			= 0;
local nChangeNum	= 0;
local CHANGE_RATE	= 1;		-- 1:1 兑换比例
local CHANGE_COST_RATE = 0.001		-- 0.1%	兑换手续费率


function ExchangeMoneyFrame_OnLoad()
	--this:SetUpdateTime(0);
	this:RegisterEvent("UI_ACTOR_ATTRIBUTE_CHG");	
end

function ExchangeMoneyFrame_OnEvent()
	if arg1 == "UI_ACTOR_ATTRIBUTE_CHG" then
		if ExchangeMoneyFrame:IsShown() then
			local mainPlayer = ActorMgr:getMainPlayer();
			if mainPlayer == nil then return end

			ExchangeMoneyFrameBackBtnMyMoney:SetText( mainPlayer:getMoney() );
		end
	end
end

function ExchangeMoneyFrameOKBtnOnClick()
	if not ExchangeMoneyFrame:IsShown() then
		return;
	end
	if nChangeNum == 0 then
		ShowMidTips("请输入兑换数量");
		SetFocusFrame("ExchangeMoneyFrameNumIput");
		return;
	end
	ExchangeMoneyFrame:Hide();
	ExchangeMoneyConfirmFrame:Show();
end

function ExchangeMoneyFrame_OnShow()
	
	ExchangeMoneyFrameDescText:SetText("欢迎来到联邦交易所，银行职员巴拉贡竭诚为您服务！",40,31,3);
	local mainPlayer = ActorMgr:getMainPlayer();
	if mainPlayer == nil then return end

	ExchangeMoneyFrameBackBtnMyMoney:SetText(mainPlayer:getMoney());
	ExchangeMoneyFrameBackBtnTodayPrice:SetText( CHANGE_RATE );
	ExchangeMoneyFrameNumIput:SetText( 1 );
	ExchangeMoneyFrameBackBtnExchangeDesc1:SetText("");
	ExchangeMoneyFrameBackBtnExchangeDesc2:SetText("");
	SetFocusFrame("ExchangeMoneyFrameNumIput");
	
end

function openChangeCoin(npcId)
	nNpcID = npcId;
	if not ExchangeMoneyFrame:IsShown() then
		ExchangeMoneyFrame:Show();
		util.testNpcDialogDistance(npcId, ExchangeMoneyFrame)
	end
end

function ExchangeMoneyConfirmFrameOKBtnOnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container	= mainplayer:getContainer();
	container:send_ChangeCoin( 1 , nChangeNum , nNpcID )		--1 是绑银( MONEY_TYPE_MONEY_NAME)换银币，2是银币换银券( MONEY_TYPE_MONEY_NAME)
	nCost = 0;
	nNpcID = 0;
	nChangeNum = 0;
	ExchangeMoneyConfirmFrame:Hide();
end

function ExchangeMoneyFrame_OnUpdate()
	--检查输入的数量，然后计算当前总价和手续费，
	local mainPlayer = ActorMgr:getMainPlayer();
	if mainPlayer == nil then return end

	local szNum = ExchangeMoneyFrameNumIput:GetText();
	local nNum = 0;
	if szNum == nil or szNum == "" then
		nChangeNum = 0;
		return;
	end
	nNum = tonumber(szNum);

	if nNum > 1000000 then		--最多1次换100W
		nNum = 1000000;		
		ExchangeMoneyFrameNumIput:SetText(nChangeNum);
	end
	local nhaveMoney = mainPlayer:getMoney();
	if nNum > nhaveMoney then
		nNum = nhaveMoney;		
		ExchangeMoneyFrameNumIput:SetText(nChangeNum);
	end
	nChangeNum = nNum;
	if nNum <= 0 then
		ExchangeMoneyFrameBackBtnExchangeDesc1:SetText("");
		ExchangeMoneyFrameBackBtnExchangeDesc2:SetText("");
		return
	end
	nCost = math.floor( nNum * CHANGE_RATE * CHANGE_COST_RATE );
	if nCost <=0 then
		nCost = 1;
	end
	ExchangeMoneyFrameBackBtnExchangeDesc1:SetText(nChangeNum .. MONEY_TYPE_MONEY_NAME.."可以兑换：" .. nChangeNum * CHANGE_RATE .. " "..MONEY_TYPE_BULL_NAME.."");
	ExchangeMoneyFrameBackBtnExchangeDesc2:SetText( "兑换需要支付税金：" .. nCost .. " "..MONEY_TYPE_BULL_NAME.."" );	

end

function ExchangeMoneyConfirmFrame_OnShow()
	local nWidth = 0;
	ExchangeMoneyConfirmFrameExchangeNum:SetText( nChangeNum );

	ExchangeMoneyConfirmFrameExchangePrice:SetText( CHANGE_RATE );
	nWidth = ExchangeMoneyConfirmFrameExchangePrice:GetTextExtentWidth( CHANGE_RATE );
	ExchangeMoneyConfirmFrameExchangePriceTail:SetPoint( "topleft","ExchangeMoneyConfirmFrameExchangePrice","topleft",nWidth + 10 ,0 );

	ExchangeMoneyConfirmFrameExchangeTotal:SetText( nChangeNum * CHANGE_RATE );
	nWidth = ExchangeMoneyConfirmFrameExchangeTotal:GetTextExtentWidth( nChangeNum * CHANGE_RATE );
	ExchangeMoneyConfirmFrameExchangeTotalTail:SetPoint( "topleft","ExchangeMoneyConfirmFrameExchangeTotal","topleft",nWidth + 10,0 )

	ExchangeMoneyConfirmFrameExchangeCost:SetText( nCost );
	nWidth = ExchangeMoneyConfirmFrameExchangeCost:GetTextExtentWidth( nCost );
	ExchangeMoneyConfirmFrameExchangeCostTail:SetPoint( "topleft","ExchangeMoneyConfirmFrameExchangeCost","topleft",nWidth + 10,0 );
	util.testNpcDialogDistance(nNpcID, ExchangeMoneyConfirmFrame);
end

function NumIput_OnEscapePressed()
	ExchangeMoneyFrame:Hide();
end

function EcchangeMonetFrame_OnHide()
	ClearFocus();
end