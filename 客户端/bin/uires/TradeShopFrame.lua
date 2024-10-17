local MAX_TRADE_NUM = 16;
local MAX_LINE_NUM = 4;
local curItemNum = 0;
local curShowPage = 1;
local curMoney = 0;
local targetMoney = 0;
local t_FontColor = { { 190, 160, 90 }, { 100, 100, 100 } };
local t_paoShangData = {};

local function SetItemBtn( name, itemId, price, num )
	local btn	= getglobal( name );
	local ItemTex	= getglobal( name .. "IconTexture" );
	local BackTex	= getglobal( name .. "BackTexture" );
	local ItemBoxTex= getglobal( name .. "BoxTexture" );
	local ItemNum	= getglobal( name .. "CountFont");
	btn:SetClientUserData( 0, itemId );
	btn:SetClientUserData( 1, price );

	local itemDef	= getItemDef( itemId );
	local IconPath	= GetItemEquipIconPath() .. itemDef.IconID .. ".tga";
	-- 如果没找到物品的图标则使用默认图标显示
	if not IsInExistence( IconPath ) then
		IconPath = DEFAULT_ITEM_PATH;
	end
	ItemTex:SetTexture( IconPath );
	ItemTex:SetGray( num == 0 );
	BackTex:Show();
	ItemBoxTex:Show();
	ItemNum:SetText( num );
	SetEquipImportantEffectUV( itemDef, btn:GetName() .. "UVAnimationTex" );
end

local function SetTradeLineInfo( name, itemId, num, price, base )
	local btn	= getglobal( name );
	local priceFont = getglobal( name .. "Price" );
	local baseFont	= getglobal( name .. "Base" );
	local itemBtn	= getglobal( name .. "Item" );
	local optBtn	= getglobal( name .. "Opt" );
	
	if base then
		baseFont:SetText( base );
	elseif baseFont then
		baseFont:SetText( "" );
	end

	if itemId ~= nil and num ~= nil then
		priceFont:SetText( price );
		local idx = num > 0 and 1 or 2;
		itemBtn:Show();
		SetItemBtn( name .. "Item", itemId, price, num );
		optBtn:Show();

		if num > 0 and string.find( name, "Left" ) then
			local canBuy = math.floor( curMoney / price );
			idx = canBuy > 0 and 1 or 2;
		end
		if idx < 2 then
			optBtn:Enable();
		else
			optBtn:Disable();
		end

		priceFont:SetTextColor( t_FontColor[idx][1], t_FontColor[idx][2], t_FontColor[idx][3] );
		if base then
			baseFont:SetTextColor( t_FontColor[idx][1], t_FontColor[idx][2], t_FontColor[idx][3] );
		end
	else
		priceFont:SetText( "" );
		itemBtn:Hide();
		optBtn:Hide();
	end
end

local function updatePackageTradeItem()
	local count = 1;
	local name  = "";
	for i=1, #t_paoShangData do
		if count > MAX_TRADE_NUM then
			break;
		end
		if t_paoShangData[i].num > 1 then
			for j=1, t_paoShangData[i].num do
				name = "PlayerContainerFrameTradeItem" .. count;
				SetPackageTradeItem( name, t_paoShangData[i].itemId );
				count = count + 1;
			end
		else
			name = "PlayerContainerFrameTradeItem" .. count;
			SetPackageTradeItem( name, t_paoShangData[i].itemId );
			count = count + 1;
		end
	end
	curItemNum = count - 1;

	for i=count, MAX_TRADE_NUM do
		name = "PlayerContainerFrameTradeItem" .. i;
		SetPackageTradeItem( name );
	end
end

local function updateRightPage()
	for i=1, MAX_LINE_NUM do
		local name = "TradeShopFrameRightLine" .. i;
		local idx = ( curShowPage - 1 ) * MAX_LINE_NUM + i;
		if idx > #t_paoShangData then
			SetTradeLineInfo( name );
		else
			local base = math.floor( t_paoShangData[idx].base / t_paoShangData[idx].num );
			SetTradeLineInfo( name, t_paoShangData[idx].itemId, t_paoShangData[idx].num, t_paoShangData[idx].price, base );
		end
	end
	
	if curItemNum > 0 then
		TradeShopFrameSellAll:Enable();
	else
		TradeShopFrameSellAll:Disable();
	end
end

local function checkShopOptBtnState()
	for i=1, MAX_LINE_NUM do
		local font = getglobal( "TradeShopFrameLeftLine" .. i .. "Price" );
		local btn = getglobal( "TradeShopFrameLeftLine" .. i .. "Opt" );
		if curMoney < tonumber( font:GetText() ) then
			btn:Disable();
		else
			btn:Enable();
		end
	end
end

function TradeShopFrame_OnLoad()
	this:RegisterEvent("GE_PAOSHANG_INFO");
end

function TradeShopFrame_OnEvent()
	if arg1 == "GE_PAOSHANG_INFO" then
		local msg = UIMSG:getUIMsg();
		--更新背包货仓
		curMoney = msg.PaoShangData.YinPiao;
		targetMoney = msg.PaoShangData.JiaoPiao;
		t_paoShangData = {};
		for i=1, msg.PaoShangData.RightNum do
			table.insert( t_paoShangData, { 
						itemId	= msg.PaoShangData.PaoShangRightUi[i-1].RightItemID, 
						price	= msg.PaoShangData.PaoShangRightUi[i-1].UserSellPrice,
						num	= msg.PaoShangData.PaoShangRightUi[i-1].HaveItemNum,
						base	= msg.PaoShangData.PaoShangRightUi[i-1].CostPrice,
						} );
		end
		updatePackageTradeItem();

		if msg.PaoShangData.LeftNum > 0 then
			if not this:IsShown() then
				curShowPage = 1;
			end
			--更新npc店铺
			for i=1, MAX_LINE_NUM do
				local name = "TradeShopFrameLeftLine" .. i;
				if i > msg.PaoShangData.LeftNum then
					SetTradeLineInfo( name );
				else
					local itemId	= msg.PaoShangData.PaoShangLeftUi[i-1].LeftItemID;
					local num	= msg.PaoShangData.PaoShangLeftUi[i-1].BuyRemainNum;
					local price	= msg.PaoShangData.PaoShangLeftUi[i-1].UserBuyPrice;
					SetTradeLineInfo( name, itemId, num, price );
				end
			end

			--更新玩家货仓
			if curShowPage > 1 and curShowPage > math.ceil( msg.PaoShangData.RightNum / MAX_LINE_NUM ) then
				curShowPage = curShowPage - 1;
			end
			updateRightPage();

			TradeShopFrameFinishFont:SetText( "交票金额：" .. targetMoney );
			TradeShopFrameCurFont:SetText( "银票余额：" .. curMoney );
			TradeShopFrameBlockFont:SetText( "：" .. curItemNum .. "/" .. MAX_TRADE_NUM );

			if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "卖出全部货物" then
				MessageBoxFrame:Hide();
			end
			this:Show();
			util.testNpcDialogDistance( msg.PaoShangData.iNpcPaoShangID, TradeShopFrame );
		else
			if not this:IsShown() then
				return;
			end
			if targetMoney == 0 then
				this:Hide();
			else
				TradeShopFrameCurFont:SetText( "银票余额：" .. curMoney );
				checkShopOptBtnState();
			end
		end
	end
end

function TradeShopFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 0, 0 );
end

function TradeShopFrame_OnHide()
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "卖出全部货物" then
		MessageBoxFrame:Hide();
	end
end

function TradeShopFrame_OnUpdate()
	if GetFocusFrameName() ~= nil and string.find( GetFocusFrameName(), this:GetName() )
		and string.find( GetFocusFrameName(), "NumEdit" ) then
		local edit = getglobal( GetFocusFrameName() );
		local total = edit:GetClientUserData(0);
		local num = tonumber( edit:GetText() );
		if total >= 0 and num ~= nil then
			CheckEditNumberInput( edit );
			local price = edit:GetClientUserData(1);
			if string.find( GetFocusFrameName(), "Left" ) then
				num = math.min( math.floor( curMoney / price ), num );
				num = math.min( MAX_TRADE_NUM - curItemNum, num );
			end
			num = math.min( num, total );
			edit:SetText( num );
			local font = getglobal( string.sub( GetFocusFrameName(), 1, string.len( GetFocusFrameName() ) - 7 ) .. "Total" );
			font:SetText( num * price );
		end
	end
end

function TradeShopFrameStorageBtn_OnClick()
	if not PlayerContainerFrame:IsShown() then
		PlayerContainerFrame:Show();
	end
	ShowTaskContainer();
end

local function checkTradeQuestState()
	for i = 1, Quest:getNumQusetRun() do
		local nQuestId = Quest:getQuestRunId(i-1);
		if Quest:getQuestType(nQuestId) == CLIENT_TASK_TYPE_PAOSHANG then
			return Quest:getQuestRunState(nQuestId) ~= CLIENT_TASK_STAT_FAIL;
		end
	end
	return false;
end

function TradeShopFrameOptBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	
	if this:GetText() == "全部卖出" then
		if #t_paoShangData > 0 then
			MessageBox( "提示", "确定要出售货仓中的所有货物吗？" );
			MessageBoxFrame:SetClientString( "卖出全部货物" );
			MessageBoxFrame:SetClientUserData( 0, t_paoShangData[1].itemId );
			MessageBoxFrame:SetClientUserData( 1, t_paoShangData[1].price );
			MessageBoxFrame:SetClientUserData( 2, t_paoShangData[1].num );
			return;
		else
			return ShowMidTips( "当前没有可卖出的货物" );
		end
	else
		if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "卖出全部货物" then
			return;
		end
		local btn	= getglobal( this:GetParent() .. "Item" );
		local itemId	= btn:GetClientUserData(0);
		local price	= btn:GetClientUserData(1);
		if this:GetClientID() > 0 then
			mainplayer:requestPaoShangOpt( this:GetClientID(), itemId, price, 1 );
		else
			if not checkTradeQuestState() then
				return AddGameMiddleTips2( "很遗憾，您的【商会贸易】任务失败了，无法购买贸易商品" );
			end

			if curItemNum < MAX_TRADE_NUM then
				mainplayer:requestPaoShangOpt( this:GetClientID(), itemId, price, 1 );
			else
				return AddGameMiddleTips2( "你的货仓已经装不下更多东西了，可将贸易商品出售给贸易商人以清空货仓" );
			end
		end
	end
end

function TradeShopFramePrePage_OnClick()
	curShowPage = curShowPage > 1 and curShowPage - 1 or 1;
	updateRightPage();
end

function TradeShopFrameNextPage_OnClick()
	local totalPage = math.ceil( #t_paoShangData / MAX_LINE_NUM );
	curShowPage = curShowPage < totalPage and curShowPage + 1 or curShowPage;
	updateRightPage();
end
