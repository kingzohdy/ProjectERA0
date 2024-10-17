local t_callBackValue = { };

function GetInputFrameData()
	return t_callBackValue;
end

-- Ҫ�󴫽�������table,table���뺬��nMinNum, nMaxNum��szFrameName�ֶ�
function SplitItem( itemData )
	t_callBackValue = itemData;
	InputFrame:Show();
end

-- ��Ʒ�������ô���
function InputFrame_OnShow()
	SetFocusFrame( "InputFrame_Edit" );
	InputFrame_Edit:Clear();

	InputFrame_HeadFont:SetText("��������Ʒ����");

	if t_callBackValue.szFrameName == "PlayerContainerFrame" then -- �����ڽ��ײ����������������ô���
		LockItem(  t_callBackValue.srcList,  t_callBackValue.srcGrid, 0, "PlayerContainerFrame"  ); -- �����Ʒ��ӳɹ�����ס����Ʒ
	end

	if t_callBackValue.szFrameName == "ShopFrame" then -- �������ʼ���Ʒ�����������������ô���
		InputFrame_Edit:SetText( tostring( t_callBackValue.nMaxNum ) );
		InputFrame_Edit:SelectAllText();
	elseif t_callBackValue.szFrameName == "SaveOrGetMoney" then -- �����ڱ�����Ʒ�����������������ô���
		if t_callBackValue.srcList == ATTR_ID_MONEY or 	t_callBackValue.srcList == ATTR_ID_STASHMONEY then
			InputFrame_HeadFont:SetText("������".. MONEY_TYPE_MONEY_NAME.."����");
		end
	else
		InputFrame_Edit:SetText( "1" );
		InputFrame_Edit:SelectAllText();
	end
end

-- �ر��������ô���ʱִ����������Ĳ���
function InputFrame_OnHide()
	ClearItemLock( "PlayerContainerFrame" );
	InputFrame_Edit:Clear();
	t_callBackValue = {};
	ClearFocus();
end

-- �������������ʱ���������뽹��
function InputFrameEditBackButton_OnClick()
	SetFocusFrame( "InputFrame_Edit" );
end

function InfuseExpFrameEditBackButton_OnClick()
	SetFocusFrame( "InfuseExpFrame_Edit" );
end

-- @Desc: ��Ϊ����������ô��ڵ�ȷ����ť��ʱ����Ӧ
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

	if t_callBackValue.szFrameName == "TradeFrame" then -- �����ڽ��ײ����������������ô���
		GameExchange:addExchangeItem( t_callBackValue.desGrid, t_callBackValue.srcList, t_callBackValue.srcGrid, itemNum );
	end

	if t_callBackValue.szFrameName == "MailSendItemFrame" then -- �������ʼ���Ʒ�����������������ô���
		local senddata		= GameMailManager:getSendData();
		local mailitem		= senddata.m_Items[t_callBackValue.desGrid];
		if mailitem:getItemId() > 0 then
			UnLockItem( mailitem:getListType(), mailitem:getGridIdx(), mailitem:getItemId() );	-- ����
			GameMailManager:delSendItem( t_callBackValue.desGrid );	-- �Żر������Ƴ��ʼ��Ĵ�����Ʒ�еĵ�ǰ��Ʒ
		end
		srcitem = container:getItem(  t_callBackValue.srcList,  t_callBackValue.srcGrid );
		if srcitem:getItemId() > 0 then
			GameMailManager:addSendItem( srcitem , ( t_callBackValue.desGrid ), itemNum );
			LockItem(  t_callBackValue.srcList,  t_callBackValue.srcGrid, srcitem:getItemId(), "MailSendItemFrame"  ); -- �����Ʒ��ӳɹ�����ס����Ʒ
			-- ��Ʒ����ȥ��ˢ�´�����Ʒ�б�
			MailSendItemUpdate();
		end
	end

	if t_callBackValue.szFrameName == "ShopFrame" then -- �����ڳ�����Ʒ�����������������ô���
		local item = container:getItem( t_callBackValue.srcList, t_callBackValue.srcGrid );
		if item:getItemId() == 0 then return end
		local itemDef = item:getItemDef();

		local idx = CheckItemNeedImportantConfirm( item );
		if idx > 0 then
			ImportantItemConfirmFrameTitle:SetText("���߳���ȷ��");
			ShowImportantItemConfirmFrame( "����", item, idx, true );
			SetItemWarnFrame("ShopFrame");
			ImportantItemConfirmFrame:SetClientUserData( 0, t_callBackValue.srcList );
			ImportantItemConfirmFrame:SetClientUserData( 1, t_callBackValue.srcGrid );
			ImportantItemConfirmFrame:SetClientUserData( 2, itemNum );
			LockItem( item:getListType(), item:getGridIdx(), item:getItemId(), "ImportantItemConfirmFrame" );
			InputFrame:Hide();
			return;
		else
			if item:getItemType() == ITEM_SLOTTYPE_AMM and itemdef.Important > 2 then
				ImportantItemConfirmFrameTitle:SetText("���߳���ȷ��");
				ShowImportantItemConfirmFrame( "����", item, idx, false );
				SetItemWarnFrame("ShopFrame");
				ImportantItemConfirmFrame:SetClientUserData( 0, t_callBackValue.srcList );
				ImportantItemConfirmFrame:SetClientUserData( 1, t_callBackValue.srcGrid );
				ImportantItemConfirmFrame:SetClientUserData( 2, itemNum );
				LockItem( item:getListType(), item:getGridIdx(), item:getItemId(), "ImportantItemConfirmFrame" );
				InputFrame:Hide();
				return;
			end
		end
		-- ���������Ʒ����װ���ϣ���Ҫ����ȷ�����
		if needConfirmSale( itemDef ) then
			SellToShopConfirmFrame:Show();
			SellToShopConfirmFrame:SetClientUserData( 0, t_callBackValue.srcList );
			SellToShopConfirmFrame:SetClientUserData( 1, t_callBackValue.srcGrid );
			SellToShopConfirmFrame:SetClientUserData( 2, itemNum );
		else
			NpcShop:sellItem( t_callBackValue.srcList, t_callBackValue.srcGrid, itemNum ); -- ���ڵ�Ʒ����Ʒֱ�ӳ��۲���������ȷ�ϴ���
			playUISound( "sounds\\UI\\shoumai.wav" );
		end
	end

	if t_callBackValue.szFrameName == "PlayerContainerFrame" or t_callBackValue.szFrameName == "StashFrame" then -- �����ڱ�����Ʒ�����������������ô���
		local item = container:getItem( t_callBackValue.srcList, t_callBackValue.srcGrid );
		if itemNum == item:getNum() then
			return ShowMidTips( "�������������������" );
		end
		if item:getItemId() == 0  then return end
		container:send_MoveItem( t_callBackValue.srcList, t_callBackValue.srcList, t_callBackValue.srcGrid, itemNum, t_callBackValue.desGrid );
	end

	if t_callBackValue.szFrameName == "SaveOrGetMoney" then -- �����ڱ�����Ʒ�����������������ô���
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

-- @Desc: ��Ϊ����������ô��ڵ�ȡ����ť��ʱ����Ӧ
function InputFrame_CancelBtn_OnClick()
	InputFrame:Hide();
end

