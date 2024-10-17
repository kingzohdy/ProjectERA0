local t_callBackValue = { };

function GetInputFrameData()
	return t_callBackValue;
end

-- 要求传进来的是table,table必须含有nMinNum, nMaxNum和szFrameName字段
function SplitItem( itemData )
	t_callBackValue = itemData;
	InputFrame:Show();
end

-- 物品数量设置窗口
function InputFrame_OnShow()
	SetFocusFrame( "InputFrame_Edit" );
	InputFrame_Edit:Clear();

	InputFrame_HeadFont:SetText("请输入物品数量");

	if t_callBackValue.szFrameName == "PlayerContainerFrame" then -- 若由于交易操作而弹出数量设置窗口
		LockItem(  t_callBackValue.srcList,  t_callBackValue.srcGrid, 0, "PlayerContainerFrame"  ); -- 如果物品添加成功则锁住该物品
	end

	if t_callBackValue.szFrameName == "ShopFrame" then -- 若由于邮寄物品操作而弹出数量设置窗口
		InputFrame_Edit:SetText( tostring( t_callBackValue.nMaxNum ) );
		InputFrame_Edit:SelectAllText();
	elseif t_callBackValue.szFrameName == "SaveOrGetMoney" then -- 若由于背包物品操作而弹出数量设置窗口
		if t_callBackValue.srcList == ATTR_ID_MONEY or 	t_callBackValue.srcList == ATTR_ID_STASHMONEY then
			InputFrame_HeadFont:SetText("请输入".. MONEY_TYPE_MONEY_NAME.."数额");
		end
	else
		InputFrame_Edit:SetText( "1" );
		InputFrame_Edit:SelectAllText();
	end
end

-- 关闭数量设置窗口时执行清空锁定的操作
function InputFrame_OnHide()
	ClearItemLock( "PlayerContainerFrame" );
	InputFrame_Edit:Clear();
	t_callBackValue = {};
	ClearFocus();
end

-- 点击数量输入框的时候设置输入焦点
function InputFrameEditBackButton_OnClick()
	SetFocusFrame( "InputFrame_Edit" );
end

function InfuseExpFrameEditBackButton_OnClick()
	SetFocusFrame( "InfuseExpFrame_Edit" );
end

-- @Desc: 此为点击数量设置窗口的确定按钮的时候响应
function InputFrame_ConfirmBtn_OnClick()
	local szText = InputFrame_Edit:GetText();
	local itemNum = 0 ;
	if szText == "" then
		itemNum = 0;
	else
		itemNum = tonumber( szText );
	end

	if itemNum == 0 then
		InputFrame:Hide();
		InputFrame_Edit:Clear();
		return;
	end

	if itemNum < t_callBackValue.nMinNum then
		itemNum = t_callBackValue.nMinNum;
	elseif itemNum > t_callBackValue.nMaxNum then
		itemNum = t_callBackValue.nMaxNum;
	end

	local MainPlayer = ActorMgr:getMainPlayer();
	local container = MainPlayer:getContainer();

	if t_callBackValue.szFrameName == "TradeFrame" then -- 若由于交易操作而弹出数量设置窗口
		GameExchange:addExchangeItem( t_callBackValue.desGrid, t_callBackValue.srcList, t_callBackValue.srcGrid, itemNum );
	end

	if t_callBackValue.szFrameName == "MailSendItemFrame" then -- 若由于邮寄物品操作而弹出数量设置窗口
		local senddata		= GameMailManager:getSendData();
		local mailitem		= senddata.m_Items[t_callBackValue.desGrid];
		if mailitem:getItemId() > 0 then
			UnLockItem( mailitem:getListType(), mailitem:getGridIdx(), mailitem:getItemId() );	-- 解锁
			GameMailManager:delSendItem( t_callBackValue.desGrid );	-- 放回背包后移除邮件的待发物品中的当前物品
		end
		srcitem = container:getItem(  t_callBackValue.srcList,  t_callBackValue.srcGrid );
		if srcitem:getItemId() > 0 then
			GameMailManager:addSendItem( srcitem , ( t_callBackValue.desGrid ), itemNum );
			LockItem(  t_callBackValue.srcList,  t_callBackValue.srcGrid, srcitem:getItemId(), "MailSendItemFrame"  ); -- 如果物品添加成功则锁住该物品
			-- 物品放上去后刷新待发物品列表
			MailSendItemUpdate();
		end
	end

	if t_callBackValue.szFrameName == "ShopFrame" then -- 若由于出售物品操作而弹出数量设置窗口
		local item = container:getItem( t_callBackValue.srcList, t_callBackValue.srcGrid );
		if item:getItemId() == 0 then return end
		local itemDef = item:getItemDef();

		local idx = CheckItemNeedImportantConfirm( item );
		if idx > 0 then
			ImportantItemConfirmFrameTitle:SetText("道具出售确认");
			ShowImportantItemConfirmFrame( "出售", item, idx, true );
			SetItemWarnFrame("ShopFrame");
			ImportantItemConfirmFrame:SetClientUserData( 0, t_callBackValue.srcList );
			ImportantItemConfirmFrame:SetClientUserData( 1, t_callBackValue.srcGrid );
			ImportantItemConfirmFrame:SetClientUserData( 2, itemNum );
			LockItem( item:getListType(), item:getGridIdx(), item:getItemId(), "ImportantItemConfirmFrame" );
			InputFrame:Hide();
			return;
		else
			if item:getItemType() == ITEM_SLOTTYPE_AMM and itemdef.Important > 2 then
				ImportantItemConfirmFrameTitle:SetText("道具出售确认");
				ShowImportantItemConfirmFrame( "出售", item, idx, false );
				SetItemWarnFrame("ShopFrame");
				ImportantItemConfirmFrame:SetClientUserData( 0, t_callBackValue.srcList );
				ImportantItemConfirmFrame:SetClientUserData( 1, t_callBackValue.srcGrid );
				ImportantItemConfirmFrame:SetClientUserData( 2, itemNum );
				LockItem( item:getListType(), item:getGridIdx(), item:getItemId(), "ImportantItemConfirmFrame" );
				InputFrame:Hide();
				return;
			end
		end
		-- 若购买的物品是蓝装以上，则要弹出确认面板
		if needConfirmSale( itemDef ) then
			SellToShopConfirmFrame:Show();
			SellToShopConfirmFrame:SetClientUserData( 0, t_callBackValue.srcList );
			SellToShopConfirmFrame:SetClientUserData( 1, t_callBackValue.srcGrid );
			SellToShopConfirmFrame:SetClientUserData( 2, itemNum );
		else
			NpcShop:sellItem( t_callBackValue.srcList, t_callBackValue.srcGrid, itemNum ); -- 对于低品质商品直接出售不弹出二次确认窗口
			playUISound( "sounds\\UI\\shoumai.wav" );
		end
	end

	if t_callBackValue.szFrameName == "PlayerContainerFrame" or t_callBackValue.szFrameName == "StashFrame" then -- 若由于背包物品操作而弹出数量设置窗口
		local item = container:getItem( t_callBackValue.srcList, t_callBackValue.srcGrid );
		if itemNum == item:getNum() then
			return ShowMidTips( "已是最大数量，无需拆分" );
		end
		if item:getItemId() == 0  then return end
		container:send_MoveItem( t_callBackValue.srcList, t_callBackValue.srcList, t_callBackValue.srcGrid, itemNum, t_callBackValue.desGrid );
	end

	if t_callBackValue.szFrameName == "SaveOrGetMoney" then -- 若由于背包物品操作而弹出数量设置窗口
		container:send_MoveMoney( t_callBackValue.srcList, t_callBackValue.srcGrid, itemNum);
	end

	SetFocusFrame( "UIClient" );
	t_callBackValue = {};
	InputFrame:Hide();
	InputFrame_Edit:Clear();
end

function InputFrame_Edit_OnUpdate()
	local szText = InputFrame_Edit:GetText();
	if szText == "" then
		return;
	end

	local itemNum = tonumber( szText );
	if itemNum == nil then
		ItemSplitInputFrame_Edit:SetText( 0 );
		return;
	end

	if itemNum < t_callBackValue.nMinNum then
		InputFrame_Edit:Clear();
		InputFrame_Edit:SetText( tostring( t_callBackValue.nMinNum ) )
	elseif itemNum > t_callBackValue.nMaxNum then
		InputFrame_Edit:Clear();
		InputFrame_Edit:SetText( tostring( t_callBackValue.nMaxNum - 1 ) )
	end

	CheckEditNumberInput( ItemSplitInputFrame_Edit );
end

-- @Desc: 此为点击数量设置窗口的取消按钮的时候响应
function InputFrame_CancelBtn_OnClick()
	InputFrame:Hide();
end

