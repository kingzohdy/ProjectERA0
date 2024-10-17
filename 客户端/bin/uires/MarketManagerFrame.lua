
local nCurSalePage = 1 ;	--��ǰ��Ʒҳ��
local nCurSaleTotalPage = 1;	--��ǰ��Ʒ����ҳ��

local ONE_PAGE_CONTAINER_NUM = 16;	--��ʱ����ÿҳ��ʾ����Ʒ����
local nCurContainerPage = 1;	--��ǰ��ʱ������ҳ��
local nCurContainerTotalPage = 1;	--��ǰ��ʱ��������ҳ��

local nCurSelType = 0;	--��ǰѡ�еı�ǩ
local nCurSubSelType = 0;	--��ǰѡ�е�2����ǩ

local nTextExtent = 0;	--��ǰ��ʾ��Ϣ�ĳ���
local nBeginBulletintTime  = 0;
local SCROLL_WIDTH = 860 ;
local nCurDisplayMsgIndex = 0;	--��ǰ��ʾ���������Ϣ����

local t_LabelNameList =	{};		--��ǩ�б�

local t_TicketNameList = {};	-- ��ұ�ǩ�б�
local t_BindGoldNameList = {};	-- ��ȯ��ǩ�б�

local ModelAngle = 0;		--ģ�ͽǶ�

local nCurBuyType	= BUY_TYPE_TICKET;

local MAX_STONE_NUM	= 7;		--ˮ������
local MAX_MAIN_TYPE_NUM = 10
local MAX_SUB_TYPE_NUM = 6
local MT_DAY_GIFT_ITEMID = 5052919;
local SPRING_BLESS_GIFT_ITEMID = 5052119;    --����ף�����
local MAX_AD_PIC_NUM = 5;
-- �̳�Ԥ��
local MARKET_PREVIEW_PET = 1
local MARKET_PREVIEW_RIDE = 2
local MARKET_PREVIEW_EQUIP = 3


local t_SelectPreViewItem = {}
local nCurPreviewItem = 0
local curPreViewType = MARKET_PREVIEW_EQUIP

--��ȡ���tip
local UVATexShowFlag	= true;
local UVATexStartTime	= 0;
--���ͺ�����
local t_FriendName = {};
--�̳ǹ����
local t_AdPath = {};

function GetMarketContact()
	return t_FriendName;
end

function isPackItem( itemData )
	local num = 0
	for i=1,SHOP_ITEM_COUNT_TOLUA do
		if itemData.Items[i-1].ItemID > 0 then
			num = num + 1
		end
	end
	if num > 1 then 
		return true
	else
		return false
	end
end

function MarketContributeFrame_FriendBtn_OnClick()	
	--ȡ�����б�
	t_FriendName = {};
	local nFriendNum = GameFriendManager:getRelationPlayerCount(CLIENT_RELATION_TYPE_FRIEND);
	for i = 1,nFriendNum do
		local friend = GameFriendManager:getFriendInfoByIndex(CLIENT_RELATION_TYPE_FRIEND,i-1);
		table.insert(t_FriendName,friend.m_szName);
	end
	ContactListTipsFrame:SetClientUserData(0,CONTACT_TYPE_MARKET);
	ContactListTipsFrame:Show();
end

--��ȡ��ǩҳ���ϼ���ǩ
function GetCategoryParentNodeInfo( childNodeInfo )
	if childNodeInfo.level == 1 then
		return;
	end
	for i = 1, table.getn( t_LabelNameList ) do
		if t_LabelNameList[i].nSubType== 0 and t_LabelNameList[i].nType == childNodeInfo.nType then
			return t_LabelNameList[i];
		end
	end
end

local function getSubTypeLabelNum( nType )
	local num = 0
	for i = 1, table.getn( t_LabelNameList ) do
		if t_LabelNameList[i].nSubType> 0 and t_LabelNameList[i].nType == nType then
			num = num + 1
		end
	end
	return num
end

--ˢ�±�ǩҳ
function UpdateMarketManagerCategoryList()
	DisCheckAllBtn( "MarketManagerFrame_MainTypeBtn", MAX_MAIN_TYPE_NUM );
	DisCheckAllBtn( "MarketManagerFrame_SubTypeBtn", MAX_SUB_TYPE_NUM )

	local nCurMainShowBtn = 1; -- ��Ϊָ��ť��ָ��
	local nCurSubShowBtn = 1;
	for i=1, table.getn( t_LabelNameList ) do	
		-- ��Ϊ1���ڵ�		
		if t_LabelNameList[i].nSubType == 0 then
			-- ��鰴ťָ���Ƿ�Խ��
			if nCurMainShowBtn <= MAX_MAIN_TYPE_NUM then
				local btn = getglobal( "MarketManagerFrame_MainTypeBtn" .. nCurMainShowBtn );
				btn:SetText( t_LabelNameList[i].name );
				btn:SetClientUserData( 0, t_LabelNameList[i].nType );
				btn:SetClientUserData( 1, t_LabelNameList[i].nSubType);				
				btn:Show();
				if t_LabelNameList[i].nType == nCurSelType then
					btn:Checked()
				end
				nCurMainShowBtn = nCurMainShowBtn + 1;
			end
		-- ��Ϊ2���ڵ�
		elseif t_LabelNameList[i].nType == nCurSelType then
			if getSubTypeLabelNum(nCurSelType) > 1 and MarketManager:getSubTypeItemNum( nCurSelType, t_LabelNameList[i].nSubType, nCurBuyType) > 0 then
				if nCurSubShowBtn <= MAX_SUB_TYPE_NUM then
					local btn = getglobal( "MarketManagerFrame_SubTypeBtn" .. nCurSubShowBtn )
					btn:SetText( t_LabelNameList[i].name );				
					btn:SetClientUserData( 0, t_LabelNameList[i].nType );
					btn:SetClientUserData( 1, t_LabelNameList[i].nSubType);
					btn:Show();
					if t_LabelNameList[i].nSubType == nCurSubSelType then
						btn:Checked()
					end
					nCurSubShowBtn = nCurSubShowBtn + 1;
				end
			end
		end
	end
	-- ��ûʹ�õİ�ť��������
	for i = nCurMainShowBtn,MAX_MAIN_TYPE_NUM do
		local btn	= getglobal( "MarketManagerFrame_MainTypeBtn" .. i  );
		btn:Hide();
	end
	for i = nCurSubShowBtn,MAX_SUB_TYPE_NUM do
		local btn	= getglobal( "MarketManagerFrame_SubTypeBtn" .. i  );
		btn:Hide();
	end
end

function GetItemIconPathByID(itemid)
	if itemid == 0 then
		return DEFAULT_ITEM_PATH
	end
	local itemdef = getItemDef(itemid)
	if not itemdef then
		return DEFAULT_ITEM_PATH
	end
	local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
	return IsInExistence( IconPath ) and IconPath or DEFAULT_ITEM_PATH;
end

--���ñ�ǩҳ����
function MarketManagerFrameSetLabelText()
	--���ñ���
	MarketManagerFrameHeadFont:SetText("�� ��");
	t_TicketNameList = {}
	t_BindGoldNameList = {}
	
	table.insert( t_TicketNameList, { name="ȫ��", nType=0,nSubType=0 } )
	table.insert( t_BindGoldNameList, { name="ȫ��",nType=0,nSubType=0 } )
	local nLabelCount = MarketManager:getLabelCount( BUY_TYPE_TICKET )
	for i = 1,nLabelCount do
		local typeLabel = MarketManager:getTypeLabel( BUY_TYPE_TICKET, i-1 )
		table.insert( t_TicketNameList, { name = typeLabel.labelName, nType = typeLabel.key, nSubType = 0 } )
		for j = 1,typeLabel.subLabelCount do
			local subTypeLabel = typeLabel.subLabel[j - 1]
			if subTypeLabel ~= nil then
				table.insert( t_TicketNameList, { name=subTypeLabel.labelName, nType=typeLabel.key, nSubType=subTypeLabel.key } )
			end
		end
	end
	nLabelCount = MarketManager:getLabelCount( BUY_TYPE_BINDGOLD )
	for i = 1,nLabelCount do
		local typeLabel = MarketManager:getTypeLabel( BUY_TYPE_BINDGOLD, i-1 )
		table.insert( t_BindGoldNameList, { name = typeLabel.labelName, nType = typeLabel.key, nSubType = 0 } )
		for j = 1,typeLabel.subLabelCount do
			local subTypeLabel = typeLabel.subLabel[j - 1]
			if subTypeLabel ~= nil then
				table.insert( t_BindGoldNameList, { name=subTypeLabel.labelName, nType=typeLabel.key, nSubType=subTypeLabel.key } )
			end
		end
	end
end

function MarketManagerFrame_OnLoad()
	this:RegisterEvent("GE_SET_LABEL_TEXT");
	this:RegisterEvent("GE_UPDATE_ITEM_LIST");
	this:RegisterEvent("GE_UPDATE_SHOW_MSG");
	this:RegisterEvent("UI_ACTOR_ATTRIBUTE_CHG");
	this:RegisterEvent("GE_CONTAINER_UPDATE");
	this:RegisterEvent("GE_MARKET_LIMIT_CHG");
	this:RegisterEvent("GE_MAIN_EQUIP_CHANGE");
	this:RegisterEvent("GE_ENTER_PLAYERLOGIN");
	this:setUpdateTime( 30 )
end

function MarketManagerFrame_OnEvent()
	if arg1 == "GE_SET_LABEL_TEXT" then
		MarketManagerFrameSetLabelText();
	elseif arg1 == "GE_UPDATE_ITEM_LIST" then
		if MarketManagerFrame:IsShown() then
			--nCurSelType = 1;
			--nCurSubSelType = 1;
			MarketManager:onSelectType(nCurSelType,nCurSubSelType,nCurBuyType);
			nCurSaleTotalPage = MarketManager:getCurSaleTotalPage(0,0);
			nCurSalePage = 1;
			MarketManagerFrame_ShowMarketItem();
		end
	elseif arg1 == "GE_UPDATE_SHOW_MSG" then
		if MarketManagerFrame:IsShown() then
			MarketManagerFrame_ReSetMsg()
		end
	elseif arg1 == "UI_ACTOR_ATTRIBUTE_CHG" then
		if MarketManagerFrame:IsShown() then
			MarketManagerFrame_UpdateMoney()
		end
		if MarketTicketFrame:IsShown() then
			MarketTickUpdateMoney();
		end
	elseif arg1 == "GE_CONTAINER_UPDATE" then		
	elseif arg1 == "GE_MARKET_LIMIT_CHG" then
		if MarketManagerFrame:IsShown() then
			MarketManager:onSelectType(nCurSelType,nCurSubSelType,nCurBuyType);
			nCurSaleTotalPage = MarketManager:getCurSaleTotalPage(nCurSelType,nCurSubSelType);
			MarketManagerFrame_ShowMarketItem(); 
		end
	elseif arg1 == "GE_MAIN_EQUIP_CHANGE" then
		if FashionPreviewBtn:IsShown() then
			FashionPreviewUpdateModel();
		end
	elseif arg1 == "GE_ENTER_PLAYERLOGIN" then
		t_AdPath = {};
		for i=1, MAX_AD_PIC_NUM do
			local path = "resource\\ad\\" .. i .. ".jpg";
			if GameMgr:CanFindFile( path ) then
				table.insert( t_AdPath, {i,path} );
			end
		end
	end
end

--�����ǩҳ
function MarketManagerFrame_CategoryBtn_OnClick()	
	nCurSelType = this:GetClientUserData( 0 );
	nCurSubSelType = this:GetClientUserData( 1 );
	if nCurSubSelType == 0 then
		nCurSubSelType = 1
	end
	
	UpdateMarketManagerCategoryList();

	MarketManager:onSelectType(nCurSelType,nCurSubSelType,nCurBuyType);
	nCurSaleTotalPage = MarketManager:getCurSaleTotalPage(nCurSelType,nCurSubSelType);
	nCurSalePage = 1;
	--ˢ����Ʒ
	MarketManagerFrame_ShowMarketItem();
end

--��ʾ�������
function MarketManagerFrame_ShowBuyFrame( shopID )
	local itemData = MarketManager:getItemByID( shopID )
	if itemData == nil then
		return;
	end
	
	local nMaxBuyNum = MarketManager:getCanByNum(itemData.ShopID);
	nMaxBuyNum = nMaxBuyNum > 0 and nMaxBuyNum or 1;

	local itemDef = getItemDef( itemData.Items[0].ItemID )
	if itemDef == nil then
		return;
	end

	--���ñ���
	if itemData.Name ~= nil and itemData.Name ~= "" then
		MarketBuyItemFrameName:SetText( itemData.Name )
	else
		MarketBuyItemFrameName:SetText( itemDef.Name )
	end
	local t_NameColor = GetItemNameColor( itemDef );
	MarketBuyItemFrameName:SetTextColor( t_NameColor[1], t_NameColor[2], t_NameColor[3] );
	MarketBuyItemFrameBtnBoxTexture:Show();
	--icon
	local IconPath		
	if itemData.IconID ~= 0 then
		IconPath = GetItemEquipIconPath() .. itemData.IconID .. ".tga";		
	else
		IconPath = GetItemEquipIconPath() .. itemDef.IconID .. ".tga";	
	end
	if not IsInExistence( IconPath ) then
		IconPath = DEFAULT_ITEM_PATH
	end
	MarketBuyItemFrameBtnIconTexture:SetTexture( IconPath );
	SetEquipImportantEffectUV( itemDef, "MarketBuyItemFrameBtnUVAnimationTex" );
	--�۸�
	local costMoney = math.floor( itemData.BuyPrice * ( 100 - itemData.Sale ) / 100 )
	MarketBuyItemFrameMoneyValue:SetText( costMoney);
	local PriceFont = getglobal("MarketBuyItemFrameMoneyValue");
	MarketBuyItemFramePriceTex:SetPoint( "topleft", "MarketBuyItemFrameMoneyValue", "topleft", PriceFont:GetTextExtentWidth( PriceFont:GetText() ) / GetScreenScaleY()+ 5 , 
									-4 );
		
	local nameFont 		= getglobal("MarketBuyItemFrameName");
	local itemNumFont 	= getglobal("MarketBuyItemFrameNameNum");		 
	--count
	if itemData.Items[0].ItemNum > 1 and not isPackItem( itemData ) then
		MarketBuyItemFrameBtnCountFont:SetText( itemData.Items[0].ItemNum );
		MarketBuyItemFrameBtnCountFont:Show();
		itemNumFont:SetText("�� "..itemData.Items[0].ItemNum);
		itemNumFont:SetPoint( "topleft", nameFont:GetName(), "topleft", nameFont:GetTextExtentWidth( nameFont:GetText() ) / GetScreenScaleY() + 3,
								 -1 );
		itemNumFont:Show();
	else
		MarketBuyItemFrameBtnCountFont:Hide()
		itemNumFont:Hide();
	end
	if isPackItem( itemData ) then
		MarketBuyItemFrameDescRich:SetText( "��Ʒ���ܣ�" .. itemData.ShopDesc,255,255,190 )
	else
		MarketBuyItemFrameDescRich:SetText( itemDef.ItemDesc,255,255,190 )
	end
	MarketBuyItemFrameDescRich:ScrollFirst()
	MarketBuyItemFrame:Show()
	MarketBuyItemFrame:SetClientUserData( 1, shopID )
	--��������
	MarketBuyItemFrameBtn:SetClientUserData( 0, costMoney )
	MarketBuyItemFrameBtn:SetClientUserData( 1, shopID )
	MarketBuyItemFrameBtn:SetClientUserData( 2, nMaxBuyNum )
end

--�������
function MarketManagerFrame_BuyBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local lockState = container:getContainerLockState();
	if lockState == CONTAINER_LOCK_STATE_LOCK then
		ShowMidTips("������֤������ȫ��")
		return;
	end
	local parent = getglobal( this:GetParent() )
	local nShopID = parent:GetClientUserData(0);
	if nShopID > 0 then
		local itemData = MarketManager:getItemByID( nShopID )
		MarketBuyItemFrameBtn:SetClientUserData( 3, itemData.BuyType )
		if itemData.Items[0].ItemID == MT_DAY_GIFT_ITEMID then 
			MessageBoxFrame:SetClientUserData(0,nShopID);
			MessageBox( "��ʾ","�Ƿ�ȷ������ÿ�ճɳ�����" );
			MessageBoxFrame:SetClientString("ÿ�ճɳ�����");
		else
			MarketManagerFrame_ShowBuyFrame(nShopID)
		end
		FashionPreviewBuyFrame:Hide();
	end
	
end
--��ʾ���ͽ���
function MarketManagerFrame_ContributeFrame( nShopID )
	--ֱ�ӹ�������
	local itemData = MarketManager:getItemByID(nShopID);
	if itemData == nil then
		return;
	end
	local itemDef = getItemDef( itemData.Items[0].ItemID );
	if itemDef == nil then
		return;
	end
	--���ñ���
	if itemData.Name ~= nil and itemData.Name ~= "" then
		MarketContributeFrameName:SetText(itemData.Name);
	else
		MarketContributeFrameName:SetText(itemDef.Name);
	end
	local t_NameColor = GetItemNameColor( itemDef );
	MarketContributeFrameName:SetTextColor( t_NameColor[1], t_NameColor[2], t_NameColor[3] );
	local nMaxBuyNum = MarketManager:getCanByNum(itemData.ShopID);
	nMaxBuyNum = nMaxBuyNum > 0 and nMaxBuyNum or 1;
	MarketBuyItemFrameBtnBoxTexture:Show();
	--icon
	local IconPath
	if itemData.IconID ~= 0 then
		IconPath = GetItemEquipIconPath() .. itemData.IconID .. ".tga";		
	else
		IconPath = GetItemEquipIconPath() .. itemDef.IconID .. ".tga";	
	end
	if not IsInExistence( IconPath ) then
		IconPath = DEFAULT_ITEM_PATH;
	end
	MarketContributeFrameBtnIconTexture:SetTexture( IconPath );
	SetEquipImportantEffectUV( itemDef, "MarketContributeFrameBtnUVAnimationTex" );
	--�۸� 
	local costMoney = math.floor( itemData.BuyPrice * ( 100 - itemData.Sale ) / 100 )
	MarketContributeFrameMoneyValue:SetText(costMoney);
	local PriceFont = getglobal("MarketContributeFrameMoneyValue");
	MarketContributeFramePriceTex:SetPoint( "topleft", "MarketContributeFrameMoneyValue", "topleft", PriceFont:GetTextExtentWidth( PriceFont:GetText() ) / GetScreenScaleY()+ 5 , 
									-4 );
									
	--count
	local nameFont 		= getglobal("MarketContributeFrameName");
	local itemNumFont 	= getglobal("MarketContributeFrameNameNum");		 

	if itemData.Items[0].ItemNum > 1 and not isPackItem( itemData ) then
		MarketContributeFrameBtnCountFont:SetText( itemData.Items[0].ItemNum );
		MarketContributeFrameBtnCountFont:Show();

		itemNumFont:SetText("�� "..itemData.Items[0].ItemNum);
		itemNumFont:SetPoint( "topleft", nameFont:GetName(), "topleft", nameFont:GetTextExtentWidth( nameFont:GetText() ) / GetScreenScaleY() + 3,
								 -1 );
		itemNumFont:Show();
	else 
		MarketContributeFrameBtnCountFont:Hide();
		itemNumFont:Hide();
	end

	if isPackItem( itemData ) then
		MarketContributeFrameDescRich:SetText( "��Ʒ���ܣ�" .. itemData.ShopDesc,255,255,190 )
	else
		MarketContributeFrameDescRich:SetText( itemDef.ItemDesc,255,255,190 )
	end
	MarketContributeFrameDescRich:ScrollFirst()

	MarketContributeFrame:Show();
	MarketContributeFrame:SetClientUserData( 1, nShopID )
	MarketContributeFrameBtn:SetClientUserData( 0, costMoney )
	MarketContributeFrameBtn:SetClientUserData( 1, nShopID )
	MarketContributeFrameBtn:SetClientUserData( 2, nMaxBuyNum )
end

--������Ͱ�ť
function MarketManagerFrame_ContributeBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local lockState = container:getContainerLockState();
	if lockState == CONTAINER_LOCK_STATE_LOCK then
		ShowMidTips("������֤������ȫ��");
		return;
	end
	local parent = getglobal( this:GetParent() );
	local nShopID = parent:GetClientUserData(0);
	if nShopID > 0 then
		local itemData = MarketManager:getItemByID( nShopID )
		if itemData.Items[0].ItemID == MT_DAY_GIFT_ITEMID then 
			ShowMidTips("�õ����޷�����");
			return;
		end
		MarketManagerFrame_ContributeFrame( nShopID );
		local itemData = MarketManager:getItemByID( nShopID )
		MarketContributeFrameBtn:SetClientUserData( 3, itemData.BuyType )
		FashionPreviewBuyFrame:Hide();
	end
end

--�ر�
function MarketManagerFrame_Close()
	MarketManagerFrame:Hide();
end

--��ҳ
function MarketManagerSaleLeftPageBtn_OnClick()
	if nCurSalePage == 1 then
		return;
	end
	nCurSalePage = nCurSalePage - 1;
	MarketManagerFrame_ShowMarketItem();	
end

function MarketManagerSaleRightPageBtn_OnClick()
	if nCurSalePage == nCurSaleTotalPage then
		return;
	end
	nCurSalePage = nCurSalePage + 1;
	MarketManagerFrame_ShowMarketItem();
end

function MarketManagerSaleToLastPageBtn_OnClick()
	nCurSalePage = nCurSaleTotalPage;
	MarketManagerFrame_ShowMarketItem();
end

function MarketManagerSaleToFirstPageBtn_OnClick()
	nCurSalePage = 1;
	MarketManagerFrame_ShowMarketItem();
end

local t_ShopFlagTexUV = {
							[SHOP_ITEM_FLAG_DISCOUNT_TOLUA] = { 718, 687, 30, 30 },		-- ����
							[SHOP_ITEM_FLAG_HOT_TOLUA]	= { 663, 683, 51, 34 },	-- ����
							[SHOP_ITEM_FLAG_LIMIT_TOLUA] = { 701, 644, 37, 37 },	-- ����
							[SHOP_ITEM_FLAG_NEW_TOLUA] = { 663, 718, 36, 27 },	-- ��Ʒ
							[SHOP_ITEM_FLAG_PANIC_TOLUA] = { 663, 645, 30, 30 },	-- ����
						}
function MarketManagerFrame_ShowMarketItem()
	--����������Ʒ
	for i = 1,ONE_PAGE_NUM do
		local itemBtn = getglobal("MarketManagerFrame_SaleItem"..i);	
		itemBtn:Hide();
	end
	--��ʾҳ��
	MarketManagerFrame_SaleItemPagePageFont:SetText("��"..nCurSalePage.."/"..nCurSaleTotalPage.."ҳ");
	--��ʾ�Դ���ʾ
	local viewFont = getglobal( "MarketManagerFrameViewFont" );
	if nCurSelType == 2 then
		viewFont:SetText("���������������ɲ鿴");
	elseif nCurSelType == 3 then
		viewFont:SetText("������ʱװͼ����Դ�");
	elseif nCurSelType == 9 or nCurSelType == 0 then
		viewFont:SetText("������ʱװ�����������ɲ鿴");
	else
		viewFont:SetText("");
	end

	local nStartIndex = (nCurSalePage - 1) * ONE_PAGE_NUM;
	for nIndex = 1,ONE_PAGE_NUM do
		local itemData = MarketManager:getMarketItem(nIndex + nStartIndex , nCurSelType , nCurSubSelType);
		if itemData ~= nil then
			local item				= getglobal( "MarketManagerFrame_SaleItem"..nIndex )
			local itemNumFont		= getglobal( item:GetName() .. "NameNum");	
			local itemBtn			= getglobal( item:GetName() .. "Btn" )
			local itemBtnItemTex	= getglobal( item:GetName() .. "BtnIconTexture")
			local itemCountFont		= getglobal( item:GetName() .. "BtnCountFont")
			local itemNameFont		= getglobal( item:GetName() .. "Name")
			local redLineTex		= getglobal( item:GetName() .."DiscountRedLineTex")
			local line1Font			= getglobal( item:GetName() .. "Line1Font" )
			local line2Font			= getglobal( item:GetName() .. "Line2Font" )
			local line3Font			= getglobal( item:GetName() .. "Line3Font" )
			local line4Font			= getglobal( item:GetName() .. "Line4Font" )
			local line1Head			= getglobal( item:GetName() .. "Line1Head" )
			local line2Head			= getglobal( item:GetName() .. "Line2Head" )
			local line3Head			= getglobal( item:GetName() .. "Line3Head" )
			local line4Head			= getglobal( item:GetName() .. "Line4Head" )
			local moneyBtn1			= getglobal( item:GetName() .. "MoneyBtn1" )
			local moneyBtn2			= getglobal( item:GetName() .. "MoneyBtn2" )
			local FlagTex			= getglobal( item:GetName() .. "BtnFlagTex" )
			local buyBtn;
			local ContributeBtn;
			local buyBtn1			= getglobal( item:GetName() .. "BuyBtn" );
			local ContributeBtn1	= getglobal( item:GetName() .. "ContributeBtn" )
			local buyBtn2			= getglobal( item:GetName() .. "BuyBtnTwo" );
			local ContributeBtn2	= getglobal( item:GetName() .. "ContributeBtnTwo" )
			if nCurBuyType == BUY_TYPE_TICKET then
				buyBtn				= getglobal( item:GetName() .. "BuyBtn" );
				ContributeBtn		= getglobal( item:GetName() .. "ContributeBtn" )
				buyBtn1:Show();
				ContributeBtn1:Show();
				buyBtn2:Hide();
				ContributeBtn2:Hide();
			else 
				buyBtn				= getglobal( item:GetName() .. "BuyBtnTwo" );
				ContributeBtn		= getglobal( item:GetName() .. "ContributeBtnTwo" )
				buyBtn2:Show();
				ContributeBtn2:Show();
				buyBtn1:Hide();
				ContributeBtn1:Hide();
			end
			
			itemBtnItemTex:SetGray( false )
			line1Font:SetTextColor( 255, 255, 190 )
			line2Font:SetTextColor( 0, 255, 0 )
			line3Font:SetTextColor( 255, 130, 0 )
			buyBtn:Enable()
			if nCurBuyType == BUY_TYPE_TICKET then
				if itemData.Items[0].ItemID == MT_DAY_GIFT_ITEMID or  itemData.Items[0].ItemID == SPRING_BLESS_GIFT_ITEMID then
					ContributeBtn:Disable();
				else
					ContributeBtn:Enable();
				end
			else
				ContributeBtn:Disable()
			end
			-- ÿ�ճɳ����� ���⴦��
			if itemData.Items[0].ItemID == MT_DAY_GIFT_ITEMID and getCurMTRewardState() then
				buyBtn:Disable();
			else
				buyBtn:Enable();
			end
			local itemDef = getItemDef( itemData.Items[0].ItemID )
			if itemDef ~= nil then
				itemBtn:SetClientUserData( 0, itemData.ShopID )
				item:SetClientUserData( 0, itemData.ShopID )
				--icon
				local IconPath		
				if itemData.IconID ~= 0 then
					IconPath = GetItemEquipIconPath() .. itemData.IconID .. ".tga";		
				else
					IconPath = GetItemEquipIconPath() .. itemDef.IconID .. ".tga";	
				end
				if not IsInExistence( IconPath ) then
					IconPath = DEFAULT_ITEM_PATH;
				end
				itemBtnItemTex:SetTexture( IconPath )
				SetEquipImportantEffectUV( itemDef, item:GetName() .. "BtnUVAnimationTex" );
				
				--��Ʒ��
				if itemData.Name ~= "" then
					itemNameFont:SetText(itemData.Name);
				else
					itemNameFont:SetText(itemDef.Name );
				end

				local t_NameColor = GetItemNameColor( itemDef );
				itemNameFont:SetTextColor( t_NameColor[1], t_NameColor[2], t_NameColor[3] );

				-- �۸�				
				line2Font:SetText( math.floor( itemData.BuyPrice * ( 100 - itemData.Sale ) / 100 ) )
				if itemData.Sale > 0 then
					redLineTex:Show();
					line1Font:SetText( itemData.BuyPrice )
					line1Head:Show()
					moneyBtn1:Show()
				else
					redLineTex:Hide();
					line1Font:SetText( "" )
					line1Head:Hide()
					moneyBtn1:Hide()
				end
				
				-- ��������				
				if itemData.BuyType == BUY_TYPE_TICKET then
					moneyBtn1:ChangeNormalTexture( 99, 494, 17, 18 )
					moneyBtn1:SetClientUserData( 0,1 )
					moneyBtn1:SetSize( 16, 14 )
					moneyBtn2:ChangeNormalTexture( 99, 494, 17, 18 )
					moneyBtn2:SetClientUserData( 0,1 )
					moneyBtn2:SetSize( 16, 14 )
				else
					moneyBtn1:ChangeNormalTexture( 57, 494, 20, 18 )
					moneyBtn1:SetClientUserData( 0,2 )
					moneyBtn1:SetSize( 15, 16 )
					moneyBtn2:ChangeNormalTexture( 57, 494, 20, 18 )
					moneyBtn2:SetClientUserData( 0,2 )
					moneyBtn2:SetSize( 15, 16 )
				end

				-- ��ʱ������
				local limit = MarketManager:getShopLimit( itemData.ShopID )
				if limit ~= nil then
					line3Font:SetText( limit.LimitSellTotalNum - limit.LimitSelledNum .. "��" )
					line3Head:SetText("������")
					line3Head:Show()
					line4Font:SetText( MarketManager:getNextRefreshTime( limit ) )
					line4Font:SetPoint( "topleft", item:GetName(), "topleft", 97, 66 )
					line4Head:Show()
					if limit.LimitSellTotalNum == limit.LimitSelledNum then
						itemBtnItemTex:SetGray( true )
						line1Font:SetTextColor( 160, 160, 160 )
						line2Font:SetTextColor( 160, 160, 160 )
						line3Font:SetTextColor( 160, 160, 160 )
						buyBtn:Disable()
						ContributeBtn:Disable()
					end
				elseif itemData.HideDate ~= "" then
					-- �¼�ʱ��
					line3Head:SetText("�¼�ʱ�䣺")
					line3Head:Show()
					line4Font:SetText( os.date( "%m-%d %H:%M",string2time( itemData.HideDate ) ) )
					line4Font:SetPoint( "topleft", item:GetName(), "topleft", 62, 66 )
				else
					line3Font:SetText( "" )
					line3Head:Hide()
					line4Font:SetText( "" )
					line4Head:Hide()
				end
				
				-- ����
				if itemData.Items[0].ItemNum > 1 and not isPackItem( itemData ) then
					itemCountFont:SetText( itemData.Items[0].ItemNum );
					itemCountFont:Show();
					itemNumFont:SetText("�� "..itemData.Items[0].ItemNum);
					itemNumFont:SetPoint( "topleft", itemNameFont:GetName(), "top", itemNameFont:GetTextExtentWidth( itemNameFont:GetText() )/ 2 / GetScreenScaleY() + 3,
											 -1 );
					itemNumFont:Show();
				else
					itemCountFont:Hide();
					itemNumFont:Hide();
				end

				--[[
				if getItemType( itemDef.ItemID ) == ITEM_SLOTTYPE_AMM or CheckItemResult( itemDef, RESULT_PET_HATCH ) or CheckItemResult( itemDef, RESULT_RIDE ) then
					viewBtn:Show()
				else
					viewBtn:Hide()
				end
				--]]
				
				-- ���
				if itemData.Flag ~= SHOP_ITEM_FLAG_NO_TOLUA then
					if t_ShopFlagTexUV[ itemData.Flag ] ~= nil then
						local uv = t_ShopFlagTexUV[ itemData.Flag ]
						FlagTex:SetTexUV( uv[1], uv[2], uv[3], uv[4] )
						FlagTex:SetSize( uv[3], uv[4] )
						FlagTex:Show()
					end
				else
					FlagTex:Hide()
				end
				item:Show();
			end
		end
	end
end

--���½�һ���
function MarketManagerFrame_UpdateMoney()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	MarketManagerFrameMoneyFont1:SetText( "��ң�" .. MainPlayer:getGodCoin() );
	MarketManagerFrameMoneyFont2:SetText( "��ȯ��" .. MainPlayer:getBindGodCoin() );
end

--���� ��ʾ�ص�
function MarketManagerFrame_OnShow()
	nCurSelType = 9;
	nCurSubSelType = 1;
	--��ʾ��Ʒ
	MarketManagerFrame_TicketBtn_OnClick();
	--MarketManagerFrame_BindGoldBtn_OnClick();
	--���ý�ң�����
	MarketManagerFrame_UpdateMoney();	
	this:SetPoint( "center", "$parent", "center", 0, 0 );
	MarketManager:requestOpenMarket();
	MarketManagerFrame_ReSetMsg();
	
	MarketManager:requestQueryTianyuan();
	ExchangePointFrame:SetClientUserData(0,0);

	UVATexShowFlag = true;
end

function MarketManagerFrame_ReSetMsg()
	math.randomseed( GameMgr:getTickTime() );
	local path,index;
	if #t_AdPath > 0 then
		index = math.random(1, #t_AdPath);
		path = t_AdPath[index][2];
		MarketManagerFrameAdBtn:SetClientUserData( 0, t_AdPath[index][1] );
	end
	if not path or not GameMgr:CanFindFile( path ) then
		path = "uires\\TuPianLei\\ShiYiJian.tga";
	end
	MarketManagerFrameAdBtnBkg:SetTexture( path );
	if not MarketManagerFrameAdBtn:IsShown() then
		MarketManagerFrameAdBtn:Show();
	end
	--[[
	local szText = MarketManager:getShowMsg();
	MarketManagerFrameMsgRich:SetText( "#P����#n\n" .. szText, 255, 255, 190 )

	local rich = getglobal( "MarketManagerFrameMsgRich" );
	local slider  = getglobal( "MarketManagerFrameScrollBar" );  
	local nViewLine	= rich:GetViewLines();
	local nAllLine	= rich:GetTextLines();
	rich:SetDispPos(rich:GetStartDispPos());
	slider:SetValue(0);	
	if nAllLine - nViewLine > 0 then
		local nMaxValue = nAllLine - nViewLine; 		
		slider:SetMaxValue( nMaxValue );
		slider:SetMinValue(0);
		slider:SetValueStep(1);
		slider:Show();
		MarketManagerFrameScrollDownBtn:Show();
		MarketManagerFrameScrollUpBtn:Show();
	else
		slider:Hide();
		MarketManagerFrameScrollDownBtn:Hide()
		MarketManagerFrameScrollUpBtn:Hide()
	end
	--]]
end

function MarketManagerFrameAdBtn_OnClick()
	local index = this:GetClientUserData(0);
	local path = "resource\\ad\\" .. index .. ".ini";
	MarketManager:openAdLink( path );
end

function MarketManagerFrame_OnHide()
	MarketBuyItemFrame:Hide();
	MarketContributeFrame:Hide();
	MarketPreviewFrame:Hide();
	FashionPreviewBtn:Hide();
	AutoHideNomalTips( "MarketManagerFrame" );
end


function MarketManagerFrame_OnUpdate()
	if MarketManager:CheckShowDate() then
		MarketManager:onSelectType( nCurSelType, nCurSubSelType, nCurBuyType )
		nCurSaleTotalPage = MarketManager:getCurSaleTotalPage(0,0);
		nCurSalePage = 1;
		MarketManagerFrame_ShowMarketItem();
	end
end


function MarketBuyItemFrame_OnShow()
	MarketBuyItemFrameEdit:SetText(1);
	MarketBuyItemFramePrice:SetText( "�����ܼ�" .. 0 );
	SetFocusFrame( "MarketBuyItemFrameEdit" )
	UpdateMarketBuyItemFrameMoneySymbol();
end

function MarketBuyItemFrame_OnHide()
	ClearFocus();
end

function UpdateMarketBuyItemFrameMoneySymbol( nType )
	local priceTex 		= getglobal("MarketBuyItemFramePriceTex");
	local allMoneyTex	= getglobal("MarketBuyItemFrameAllGoldTex");
	if ( nType ~= nil and nType == BUY_TYPE_TICKET ) or nCurBuyType == BUY_TYPE_TICKET then
		priceTex:SetTexUV( 99, 494, 17, 18 ); 	
		allMoneyTex:SetTexUV( 99, 494, 17,18 );
	else 
		priceTex:SetTexUV( 57, 494, 20,18 ); 
		allMoneyTex:SetTexUV( 57, 494, 20,18 );
	end 
end

function MarketBuyItemFrameEditConfirmBtnOnClick()
	MarketBuyItemFrameConfirmBtnOnClick();
end

--ȷ��
function MarketBuyItemFrameConfirmBtnOnClick()
	if not MarketBuyItemFrame:IsShown() then
		return
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local nBuyID = MarketBuyItemFrame:GetClientUserData(1);
	local szText = MarketBuyItemFrameEdit:GetText();
	local itemNum = 0 ;
	local buyType = MarketBuyItemFrameBtn:GetClientUserData(3);
	if szText == "" then
		ShowMidTips("����������");
		return;
	end
	itemNum = tonumber( szText );
	if nBuyID > 0 and itemNum > 0 then
		local itemData = MarketManager:getItemByID( nBuyID );
		local nMaxBuyNum = MarketManager:getCanByNum(itemData.ShopID);
		if nMaxBuyNum == 0 then
			if itemData.BuyType == 1 then
				ExchangePointFrame:SetClientUserData(0,1);
				MarketManager:requestQueryTianyuan();
			else
				ShowMidTips("û���㹻��"..MONEY_TYPE_BIND_GODCOIN_NAME);
			end
			MarketBuyItemFrame:Hide();
			ClearFocus();
			return;
		end
		local itemDef = getItemDef( itemData.Items[0].ItemID );
		if itemDef == nil then
			return;
		end
		--[[
		if itemDef.WearPos1 == 8 and not IsFitRace( { actor = mainplayer, compareRace = itemDef.WearPre.Career } ) then
			MessageBox("��ʾ","ְҵ�������Ƿ�ȷ��Ҫ����");
			MessageBoxFrame:SetClientString("����ʱװ");
			MessageBoxFrame:SetClientUserData(1,nBuyID);
			MessageBoxFrame:SetClientUserData(2,itemNum);
			MessageBoxFrame:SetClientUserData(3,buyType);
			MarketBuyItemFrame:Hide();
			return;
		end
		--]]
		MarketManager:requestBuy(nBuyID,itemNum,0,"","",buyType);
	else
		ShowMidTips("��������ȷ������");
		return;
	end
	MarketBuyItemFrame:Hide();
	ClearFocus();
end

--ȡ��
function MarketBuyItemFrameCancleBtnOnClick()
	MarketBuyItemFrame:Hide();
end

--���� -1
function MarketBuyItemFrame_ScrollDownBtn_OnClick()
	local szText = MarketBuyItemFrameEdit:GetText();
	local itemNum = 0;
	if szText == "" then
		itemNum = 0;
	else	
		itemNum = tonumber( szText );
	end
	if itemNum == nil or itemNum == 0 then
		MarketBuyItemFrameEdit:SetText( 0 );
		return;
	end
	MarketBuyItemFrameEdit:SetText(itemNum - 1);
end

--���� +1
function MarketBuyItemFrame_ScrollUpBtn_OnClick()
	local szText = MarketBuyItemFrameEdit:GetText();
	local itemNum = 0;
	if szText == "" then
		itemNum = 0;
	else
		itemNum = tonumber( szText );
	end
	if itemNum == nil then
		MarketBuyItemFrameEdit:SetText( 0 );
		return;
	end
	MarketBuyItemFrameEdit:SetText(itemNum + 1);
end

--�������
function MarketBuyItemFrame_ScrollTopBtn_OnClick()
	local nCanBuyNum = MarketBuyItemFrameBtn:GetClientUserData(2);
	if nCanBuyNum ~= nil then
		MarketBuyItemFrameEdit:SetText( nCanBuyNum );
	else
		MarketBuyItemFrameEdit:SetText( 0 );
	end
end

--��ʾ���ͽ���
function MarketContributeFrame_OnShow()
	MarketContributeFrameEdit:SetText(1);
	MarketContributeFramePrice:SetText( "�����ܼ�" .. 0);
	SetFocusFrame("MarketContributeFrameEdit");
	UpdateMarketContributeFrameMoneySymbol();
end

function UpdateMarketContributeFrameMoneySymbol()
	local priceTex 		= getglobal("MarketContributeFramePriceTex");
	local allMoneyTex	= getglobal("MarketContributeFrameAllGoldTex");
	if nCurBuyType == BUY_TYPE_TICKET then
		priceTex:SetTexUV( 99, 494, 17, 18 ); 	
		allMoneyTex:SetTexUV( 99, 494, 17,18 );
	else 
		priceTex:SetTexUV( 57, 494, 20,18 ); 
		allMoneyTex:SetTexUV( 57, 494, 20,18 );
	end 
end

--ȷ��
function MarketContributeFrameConfirmBtnOnClick()	
	if not MarketContributeFrame:IsShown() then
		return;
	end
	local szText = MarketContributeFrameEdit:GetText();
	if szText == "" or szText == "0" then
		ShowMidTips("����������");
		return;
	end
	local itemNum = tonumber( szText );
	MarketContributeFrame:Hide();
	ClearFocus();
	local nBuyID = MarketContributeFrame:GetClientUserData(1);
	local itemData = MarketManager:getItemByID( nBuyID );
	local nMaxBuyNum = MarketManager:getCanByNum(itemData.ShopID);
	if nMaxBuyNum == 0 then
		if itemData.BuyType == 1 then
			ExchangePointFrame:SetClientUserData(0,1);
			MarketManager:requestQueryTianyuan();
		else
			ShowMidTips("û���㹻��"..MONEY_TYPE_BIND_GODCOIN_NAME.."");
		end
		return;
	end
	MarketContributeConfirmFrame:Show();	
end

function GetFriendList()
	t_FriendName = {};
	local nFriendNum = GameFriendManager:getRelationPlayerCount(CLIENT_RELATION_TYPE_FRIEND);
	for i = 1,nFriendNum do
		local friend = GameFriendManager:getFriendInfoByIndex(CLIENT_RELATION_TYPE_FRIEND,i-1);
		table.insert(t_FriendName,friend.m_szName);
	end
end

local MAX_FRIEND_NUM = 7;
function UpdateSendFriendList( nIndex )
	local name = "MarketContributeConfirmFrameFriendNameBtn";
	local num = 1;
	for i=nIndex, #t_FriendName do
		if num > MAX_FRIEND_NUM then
			break;
		end

		local font = getglobal( name .. num .. "Font" );
		font:SetText( t_FriendName[i] );
		num = num + 1;
	end

	for i=num, MAX_FRIEND_NUM do
		local font = getglobal( name .. i .. "Font" );
		font:SetText( "" );
	end
end

function MarketContributeConfirmFrame_ScrollBar_OnValueChanged()
	local slider = getglobal("MarketContributeConfirmFrame_ScrollBar");
	local nValue = slider:GetValue();
	local nMaxValue = slider:GetMaxValue();

	if nValue <= nMaxValue then
		UpdateSendFriendList( nValue + 1 );
	end
end

function MarketContributeConfirmFrame_ScrollDownBtn_OnClick()
	local slider = getglobal("MarketContributeConfirmFrame_ScrollBar");
	if not slider:IsShown() then
		return;
	end

	local nValue = slider:GetValue() + slider:GetValueStep();
	local nMaxValue = slider:GetMaxValue();
	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

function MarketContributeConfirmFrame_ScrollUpBtn_OnClick()	
	local slider = getglobal("MarketContributeConfirmFrame_ScrollBar");
	if not slider:IsShown() then
		return;
	end

	local nValue = slider:GetValue() - slider:GetValueStep();
	if nValue >= 0 then
		slider:SetValue( nValue );
	end	
end

function MarketContributeConfirmFrame_OnShow()
	MarketContributeConfirmFrameNameEdit:SetText("");
	MarketContributeConfirmFrameMessageEdit:SetText("");
	SetFocusFrame("MarketContributeConfirmFrameNameEdit");
	
	for i=1, MAX_FRIEND_NUM do
		local btn = getglobal( "MarketContributeConfirmFrameFriendNameBtn" .. i );
		if i%2 ~= 0 then
			btn:SetNormalTextureTemplate( "BlackCategoryBkg" );
		end
	end

	GetFriendList();
	local slider = getglobal("MarketContributeConfirmFrame_ScrollBar");
	SetScrollBar( slider:GetName(), MAX_FRIEND_NUM, #t_FriendName );
	slider:SetValue( 0 );
	UpdateSendFriendList( 1 );
end

function MarketContributeConfirmFrame_OnHide()
	if ContactListTipsFrame:IsShown() then
		ContactListTipsFrame:Hide();
	end
	ClearFocus();
end

function MarketContributeConfirmFrameSureBtn_OnClick()

	local szText = MarketContributeFrameEdit:GetText();
	local itemNum = tonumber( szText );
	local szSendName = MarketContributeConfirmFrameNameEdit:GetText();
	local buyType = MarketContributeFrameBtn:GetClientUserData(3);
	if szSendName == nil or szSendName == "" then
		ShowMidTips("���������ͺ�����");
		return;
	end
	local szDesc = MarketContributeConfirmFrameMessageEdit:GetText();
	local nBuyID = MarketContributeFrame:GetClientUserData(1);
	if nBuyID > 0 and itemNum > 0 then
		MarketManager:requestBuy(nBuyID,itemNum,1,szSendName,szDesc,buyType);
	end
	ClearFocus();
	MarketContributeConfirmFrame:Hide();
end

--ȡ��
function MarketContributeFrameCancleBtnOnClick()
	MarketContributeFrame:Hide();
end

function MarketContributeConfirmFrameFriendNameBtn_OnClick()
	local font = getglobal( "MarketContributeConfirmFrameFriendNameBtn" .. this:GetClientID() .. "Font" );
	MarketContributeConfirmFrameNameEdit:SetText( font:GetText() );
end

--���� -1
function MarketContributeFrame_ScrollDownBtn_OnClick()
	local szText = MarketContributeFrameEdit:GetText();
	if szText == "" then
		return;
	end
	local itemNum = tonumber( szText );
	if itemNum == nil then
		MarketContributeFrameEdit:SetText( 0 );
		return;
	end
	MarketContributeFrameEdit:SetText(itemNum - 1);
end

--���� +1
function MarketContributeFrame_ScrollUpBtn_OnClick()
	local szText = MarketContributeFrameEdit:GetText();
	if szText == "" then
		return;
	end
	local itemNum = tonumber( szText );
	if itemNum == nil then
		MarketContributeFrameEdit:SetText( 0 );
		return;
	end
	MarketContributeFrameEdit:SetText(itemNum + 1);
end

--�������
function MarketContributeFrame_ScrollTopBtn_OnClick()
	local nCanBuyNum = MarketContributeFrameBtn:GetClientUserData(2);
	if nCanBuyNum ~= nil then
		MarketContributeFrameEdit:SetText( nCanBuyNum );
	else
		MarketContributeFrameEdit:SetText( 0 );
	end
end

--���ͽ���ˢ��
function MarketContributeFrameEditOnUpdate()
	local szText = MarketContributeFrameEdit:GetText();
	if szText == "" then
		MarketContributeFramePrice:SetText( "�����ܼۣ�" .. 0 );
		return;
	end
	local itemNum = tonumber( szText );
	if itemNum == nil or itemNum < 0 then
		MarketContributeFrameEdit:SetText( 0 );
		MarketContributeFramePrice:SetText( "�����ܼۣ�" .. 0 );
		return;
	end
	local nPrice = MarketContributeFrameBtn:GetClientUserData(0);
	local nCanBuyNum = MarketContributeFrameBtn:GetClientUserData(2);
	if itemNum > nCanBuyNum then
		itemNum = nCanBuyNum;
		MarketContributeFrameEdit:SetText( nCanBuyNum )
	end
	CheckEditNumberInput(MarketContributeFrameEdit);
	local nTotalPrice = nPrice*itemNum;
	MarketContributeFramePrice:SetText( "�����ܼۣ�" .. nTotalPrice);

	local AllCostFont = getglobal("MarketContributeFramePrice");
	MarketContributeFrameAllGoldTex:SetPoint( "topleft", "MarketContributeFramePrice", "topleft", AllCostFont:GetTextExtentWidth( AllCostFont:GetText() ) / GetScreenScaleY()+ 5 , 
									-4 );
end

--�������ˢ��
function MarketBuyItemFrameEditOnUpdate()
	local szText = MarketBuyItemFrameEdit:GetText();
	if szText == "" then
		MarketBuyItemFramePrice:SetText( "�����ܼۣ�" .. 0 );
		return;
	end
	local itemNum = tonumber( szText );
	if itemNum == nil or itemNum < 0 then
		MarketBuyItemFrameEdit:SetText( 0 );
		MarketBuyItemFramePrice:SetText( "�����ܼۣ�" .. 0 );
		return;
	end
	local nPrice = MarketBuyItemFrameBtn:GetClientUserData( 0 )
	local nCanBuyNum = MarketBuyItemFrameBtn:GetClientUserData( 2 )
	
	if itemNum > nCanBuyNum then
		itemNum = nCanBuyNum;
		MarketBuyItemFrameEdit:SetText( nCanBuyNum )
	end
	CheckEditNumberInput(MarketBuyItemFrameEdit);
	local nTotalPrice = nPrice*itemNum;
	MarketBuyItemFramePrice:SetText( "�����ܼۣ�" .. nTotalPrice );
	
	local AllCostFont = getglobal("MarketBuyItemFramePrice");
	MarketBuyItemFrameAllGoldTex:SetPoint( "topleft", "MarketBuyItemFramePrice", "topleft", AllCostFont:GetTextExtentWidth( AllCostFont:GetText() ) / GetScreenScaleY()+ 5 , 
									-4 );
end

--��ֵ��ť
function MarketManagerFrame_FillBtn_OnClick()
	GameMgr:openWebSite( "http://pay.ljy0.com" );
end

--���һ��
function MarketManagerFrame_GetMoneyDesBtn_OnClick()
	ShowMidTips("�ù�����ʱδ����");
end

function MarketManagerItemOnEnter( btn )
	local shopid = string.find( btn:GetName(), "SaleItem" ) and btn:GetClientUserData( 0 ) or btn:GetClientUserData( 1 );
	if shopid == 0 and t_WearPosInfo[btn:GetClientID()] ~= nil then
		SetGameTooltips(btn:GetName(), t_WearPosInfo[btn:GetClientID()] );
		return;
	end
	local itemData = MarketManager:getItemByID( shopid )
	if itemData ~= nil then
		local num = 0
		for i=1,SHOP_ITEM_COUNT_TOLUA do
			if itemData.Items[i-1].ItemID > 0 then
				num = num + 1
			end
		end
		local itemDef = getItemDef( itemData.Items[0].ItemID )
		if num > 1 then
			local IconPath = GetItemEquipIconPath() .. itemData.IconID .. ".tga";
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH
			end
			local nFrameHeight = 90
			MarketItemTipsFrameTexture:SetTexture( IconPath )
			if itemData.Name ~= "" then
				MarketItemTipsFrameName:SetText(itemData.Name);
			else
				MarketItemTipsFrameName:SetText(itemDef.Name );
			end
			local t_NameColor = GetItemNameColor( itemDef );
			MarketItemTipsFrameName:SetTextColor( t_NameColor[1], t_NameColor[2], t_NameColor[3] );
			MarketItemTipsFrameRich:SetText( itemData.ShopDesc,255,255,190 )
			nFrameHeight = nFrameHeight + MarketItemTipsFrameRich:GetTotalHeight()/GetScreenScaleY();
			MarketItemTipsFrame:SetHeight( nFrameHeight )
			MarketItemTipsFrame:Show();

			local nWidth    = MarketItemTipsFrame:GetWidth();
			local nHeight   = MarketItemTipsFrame:GetHeight();

			local nRealTop  = btn:GetRealTop();
			local nRight    = btn:GetRight();
			if ( nRealTop - nHeight ) > 0 then
				if ( nRight + nWidth < GetScreenWidth() ) then
					MarketItemTipsFrame:SetPoint( "BottomLeft", btn:GetName(), "TopRight", 0, -10 );
				else
					MarketItemTipsFrame:SetPoint( "BottomRight", btn:GetName(), "TopLeft", 0, -10 );
				end
			else
				if ( nRight + nWidth < GetScreenWidth() ) then
					MarketItemTipsFrame:SetPoint( "TopLeft", btn:GetName(), "BottomRight", 0, 10 );
				else
					MarketItemTipsFrame:SetPoint( "TopRight", btn:GetName(), "BottomLeft", 0, 10 );
				end
			end
		else
			local szGemData = "0,0";
			ShowItemTips(LUA_ITEMTIPS_TYPE_NORMAL,LUA_ITEMTIPS_ITEMDEF,itemDef,nil, nil, nil, nil, szGemData);
		end
	end
end

function MarketManagerItemOnClick( btn )
	local shopid = btn:GetClientUserData( 0 )
	local itemData = MarketManager:getItemByID( shopid )
	if itemData == nil then return end
	local num = 0
	for i=1,SHOP_ITEM_COUNT_TOLUA do
		if itemData.Items[i-1].ItemID > 0 then
			num = num + 1
		end
	end
	local itemDef = getItemDef( itemData.Items[0].ItemID );
	if num == 1 and itemDef ~= nil then
		if getItemType( itemDef.ItemID ) == ITEM_SLOTTYPE_AMM then
			if FashionPreviewBuyFrame:IsShown() then
				FashionPreviewBuyFrame:Hide();
			end
			-- װ��
			local mainplayer = ActorMgr:getMainPlayer()
			if itemDef.WearPre.Gender > 0 and itemDef.WearPre.Gender ~= mainplayer:getGender() then
				ShowMidTips("�Ա�ͬ���޷��Դ�")
				return
			end
			t_SelectPreViewItem[ itemDef.WearPos1 ] = { itemDef.ItemID, shopid };
			curPreViewType = MARKET_PREVIEW_EQUIP
			FashionPreviewUpdateModel()
		elseif CheckItemResult( itemDef, RESULT_PET_HATCH ) then
			-- ����
			nCurPreviewItem = itemDef.ItemID
			curPreViewType = MARKET_PREVIEW_PET
			MarketManagerPreviewUpdateModel()
		elseif CheckItemResult( itemDef, RESULT_RIDE ) then
			-- ����
			nCurPreviewItem = itemDef.ItemID
			curPreViewType = MARKET_PREVIEW_RIDE
			MarketManagerPreviewUpdateModel()
		end
	end
end

function MarketManagerFrameSaleItemViewBtn_OnClick()
	local parent = getglobal( this:GetParent() )
	local shopid = parent:GetClientUserData( 0 )
	local itemData = MarketManager:getItemByID( shopid )
	if itemData == nil then return end
	local num = 0
	for i=1,SHOP_ITEM_COUNT_TOLUA do
		if itemData.Items[i-1].ItemID > 0 then
			num = num + 1
		end
	end
	if num == 1 then
		local itemDef = getItemDef( itemData.Items[0].ItemID )
		if getItemType( itemDef.ItemID ) == ITEM_SLOTTYPE_AMM then
			-- װ��
			local mainplayer = ActorMgr:getMainPlayer()
			if itemDef.WearPre.Gender > 0 and itemDef.WearPre.Gender ~= mainplayer:getGender() then
				ShowMidTips("�Ա�ͬ���޷��Դ�")
				return
			end
			t_SelectPreViewItem[ itemDef.WearPos1 ] = itemDef.ItemID
			curPreViewType = MARKET_PREVIEW_EQUIP
			MarketManagerPreviewUpdateModel()
		elseif CheckItemResult( itemDef, RESULT_PET_HATCH ) then
			-- ����
			nCurPreviewItem = itemDef.ItemID
			curPreViewType = MARKET_PREVIEW_PET
			MarketManagerPreviewUpdateModel()
		elseif CheckItemResult( itemDef, RESULT_RIDE ) then
			-- ����
			nCurPreviewItem = itemDef.ItemID
			curPreViewType = MARKET_PREVIEW_RIDE
			MarketManagerPreviewUpdateModel()
		end
	end
end

--��ȡ���
local nLastExchangeClick = 0;
function MarketManagerFrametExchangePointBtn_OnClick()
	if not ExchangePointFrame:IsShown() then
		if os.clock() - nLastExchangeClick > 1.5 then
			MarketManager:requestQueryTianyuan();
			nLastExchangeClick = os.clock();
			
			ExchangePointFrameNumEdit:SetText( 0 );
			SetFocusFrame("ExchangePointFrameNumEdit");
		--	ExchangePointFrameNumEdit:SelectAllText();
			ExchangePointFrame:SetClientUserData(0,0);
			ExchangePointFrame:Show();
		end
	else
		ExchangePointFrame:Hide();
	end
	local UVATexture = getglobal( "MarketManagerFrameExchangePointBtnUVAnimationTex" );
	if UVATexture:IsShown() then
		UVATexture:Hide();
	end
end

function MarketManagerFrametExchangePointBtn_OnUpdate()
	
	--�鿴����ȡ�������
	local UVATexture = getglobal( "MarketManagerFrameExchangePointBtnUVAnimationTex" );
	local dianquan	 = MarketManager:getCurDianquan();

	if dianquan > 0 and UVATexShowFlag then
		UVATexture:SetUVAnimation(50,true);
		UVATexture:Show();
		UVATexStartTime = os.clock();
		MarketManagerFrameMoneyFont3:SetText("����ȡ��ң�"..dianquan);
		UVATexShowFlag = false;
	end
	if os.clock() - UVATexStartTime > 5 then
		UVATexture:Hide();
	end
end
function MarketManagerFrameShowItem( nIndex,nBuyType )
	if not MarketManagerFrame:IsShown() then
		MarketManagerFrame:Show();
	end
	nCurBuyType = nBuyType;
	
	local nPage = math.floor((nIndex)/ONE_PAGE_NUM) + 1;
	nCurSaleTotalPage = MarketManager:getCurSaleTotalPage(nCurSelType,nCurSubSelType);
	if nPage <= nCurSaleTotalPage then
		nCurSalePage = nPage;
		MarketManagerFrame_ShowMarketItem();
	end
end

--����
function MarketFrameSearchEditBtn_OnClicked()
	local str = MarketManagerFrameSearchEdit:GetText();
	MarketManager:searchItems( str, nCurBuyType );
	nCurSaleTotalPage = MarketManager:getCurSaleTotalPage(0,0);
	nCurSalePage = 1;
	MarketManagerFrame_ShowMarketItem( );
end

function MarketManagerFrame_TicketBtn_OnClick()
	nCurBuyType = BUY_TYPE_TICKET;
	t_LabelNameList = t_TicketNameList
	nCurSelType = 9
	nCurSubSelType = 1
	
	local backTexture = getglobal("MarketManagerFrameBkgTex3");
	backTexture:SetTexture( "uires\\TuPianLei\\ShangCheng\\JinBiShangCheng.tga" )
	local uvTex		= getglobal("MarketManagerFrameCurTicketBtnAnimationTex");
	local uvTex2	= getglobal("MarketManagerFrameCurBindGodBtnAnimationTex");
	uvTex:SetUVAnimation(80,true);
	uvTex:Show();
	uvTex2:Hide();

	UpdateMarketManagerCategoryList();
	MarketManager:onSelectType( nCurSelType, nCurSubSelType, nCurBuyType )
	nCurSaleTotalPage = MarketManager:getCurSaleTotalPage(0,0);
	nCurSalePage = 1;
	MarketManagerFrame_ShowMarketItem();
	if MarketManagerFrame_BindGoldBtn:IsChecked() then
		FashionPreviewBtn:Hide();
	end
	MarketManagerFrame_TicketBtn:Checked()
	MarketManagerFrame_BindGoldBtn:DisChecked()
end

function MarketManagerFrame_BindGoldBtn_OnClick()
	nCurBuyType = BUY_TYPE_BINDGOLD;
	t_LabelNameList = t_BindGoldNameList
	nCurSelType = 9
	nCurSubSelType = 1
	
	local backTexture = getglobal("MarketManagerFrameBkgTex3");
	backTexture:SetTexture( "uires\\TuPianLei\\ShangCheng\\JinQuanShangCheng.tga" )
	local uvTex		= getglobal("MarketManagerFrameCurTicketBtnAnimationTex");
	local uvTex2	= getglobal("MarketManagerFrameCurBindGodBtnAnimationTex");
	uvTex2:SetUVAnimation(80,true);
	uvTex2:Show();
	uvTex:Hide();

	UpdateMarketManagerCategoryList();
	MarketManager:onSelectType( nCurSelType, nCurSubSelType, nCurBuyType )
	nCurSaleTotalPage = MarketManager:getCurSaleTotalPage(0,0);
	nCurSalePage = 1;
	MarketManagerFrame_ShowMarketItem();
	if MarketManagerFrame_TicketBtn:IsChecked() then
		FashionPreviewBtn:Hide();
	end
	MarketManagerFrame_TicketBtn:DisChecked()
	MarketManagerFrame_BindGoldBtn:Checked()
end

function MarketManagerFrameScrollUpBtnOnClick()
	local slider = getglobal("MarketManagerFrameScrollBar");
	local nValue = slider:GetValue() - slider:GetValueStep();
	local nMaxValue = slider:GetMaxValue();
	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end

function MarketManagerFrameScrollDownBtnOnClick()
	local slider = getglobal("MarketManagerFrameScrollBar");
	local nValue = slider:GetValue() + slider:GetValueStep();
	local nMaxValue = slider:GetMaxValue();
	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

function MarketManagerFrameOnValueChanged()
	local slider = getglobal("MarketManagerFrameScrollBar");
	local rich	= getglobal( "MarketManagerFrameMsgRich" );
	rich:SetDispPos( rich:GetStartDispPos() );
	for i = 1, slider:GetValue() do
		rich:ScrollDown();
	end
end

function MarketManagerFrameDescRich_OnClick()
	if arg2 ~= nil and arg2 ~= "" then
		GameMgr:openWebSite( arg2 );
	end
end

function MarketShowTargetPage( mainType, subType, buyType )
	nCurSelType = mainType;
	nCurSubSelType = subType;
	
	UpdateMarketManagerCategoryList();

	MarketManager:onSelectType(mainType,subType,buyType);
	nCurSaleTotalPage = MarketManager:getCurSaleTotalPage(mainType,subType);
	nCurSalePage = 1;
	--ˢ����Ʒ
	MarketManagerFrame_ShowMarketItem();
end

-------------------		�̳�Ԥ��		-----------------------

local nLastMouseClickX
function MarketManagerPreviewUpdateModel()
	local modelView = getglobal( "MarketPreviewFrameModelView" )
	if curPreViewType == MARKET_PREVIEW_PET then
		-- pet
		local itemDef = getItemDef( nCurPreviewItem )
		local petid = 0
		for i=1,MAX_ITEM_RESULT do
			if itemDef.Result[i-1].ResultID == RESULT_PET_HATCH then
				petid = itemDef.Result[i-1].ResultVal1
				break
			end
		end
		if petid == 0 then return end
		local petDef = getPetDef( petid )
		if petDef == nil then return end
		local  scale = GetNpcDisplayInfo( petDef.MouldID ).size == 0 and 1 or GetNpcDisplayInfo( petDef.MouldID ).size
		modelView:setPetModel( 0, 10100, GetModePathById( petDef.MouldID ) );
		modelView:setPetScale( 0, scale, scale, scale );
		modelView:setEntityPosition( 0, 0, 50, 0 );
	elseif curPreViewType == MARKET_PREVIEW_RIDE then
		-- ride
		local itemDef = getItemDef( nCurPreviewItem )
		local rideid,ridelv = 0,0
		for i=1,MAX_ITEM_RESULT do
			if itemDef.Result[i-1].ResultID == RESULT_RIDE then
				rideid = itemDef.Result[i-1].ResultVal2
				ridelv = itemDef.Result[i-1].ResultVal3
				break
			end
		end
		if rideid == 0 or ridelv == 0 then return end
		local rideDef = getRideDef( rideid, ridelv )
		if rideDef == nil then return end
		local  scale = GetNpcDisplayInfo( rideDef.ModelID ).size == 0 and 1 or GetNpcDisplayInfo( rideDef.ModelID ).size
		modelView:setPetModel( 0, 10100, GetModePathById( rideDef.ModelID ) );
		modelView:setPetScale( 0, scale, scale, scale );
		modelView:setEntityPosition( 0, 0, 50, 0 );
	end

	if not MarketPreviewFrame:IsShown() then
		MarketPreviewFrame:Show();
		MarketPreviewFrame:SetPoint("center","UIClient","center",0,0)
	end
end

function MarketPreviewFrameResetBtn_OnClick()
	MarketPreviewFrame:Hide();
end

function MarketPreviewFrame_OnHide()
	curPreViewType = MARKET_PREVIEW_EQUIP
	nCurPreviewItem = 0
	ModelAngle = 0;
	nLastMouseClickX = 0;
end

function MarketPreviewFrame_OnMouseDown()
	if arg1 == "LeftButton" then
		nLastMouseClickX = GetCursorPosX();
	end
end

function MarketPreviewFrame_OnMouseMove()
	if this:GetPushedState() then
		local nCurMouseClickX = GetCursorPosX();
		ModelAngle = ModelAngle - (nCurMouseClickX - nLastMouseClickX) ;
		MarketPreviewFrameModelView:setEntityRotate(0, ModelAngle);
		nLastMouseClickX = nCurMouseClickX;
	end
end

--------------------------------ʱװԤ������------------------------------
local nModelAngel = 0;
local nLastFashionMouseClickX = 0;
local MAX_FASHION_SUIT = 7;
local t_BuyList = {}

function setWearItem()
	local name = "FashionPreviewBtnWear";
	for i=1,MAX_FASHION_SUIT do
		local btn = getglobal( name .. i );
		local iconTex = getglobal( name .. i .. "IconTexture" );
		local UVAnimationTex = getglobal( name .. i .. "UVAnimationTex" );

		local pos = btn:GetClientID();
		local itemDef;
		if t_SelectPreViewItem[pos] ~= nil then
			btn:SetClientUserData( 0, t_SelectPreViewItem[pos][2] );
			itemDef = getItemDef( t_SelectPreViewItem[pos][1] );
		end

		if itemDef ~= nil then
			local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			iconTex:SetTexture( IconPath );
			SetEquipImportantEffectUV( itemDef, name .. i .. "UVAnimationTex" );
		else
			btn:SetClientUserData( 0, 0 );
			iconTex:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
			UVAnimationTex:Hide();
		end
	end
end

function FashionPreviewUpdateModel()
	local modelView = getglobal( "FashionPreviewBtnModelView" )
	if curPreViewType == MARKET_PREVIEW_EQUIP then
		-- item
		setWearItem();
		-- avatar
		local MainPlayer = ActorMgr:getMainPlayer();
		if MainPlayer == nil then return end
		local Eqiup = MainPlayer:getEquip();
		local nRace = MainPlayer:getRace();
		local nGender = MainPlayer:getGender();
		Eqiup:setAvatarAttrib( nRace, nGender )
		setAvatarAttrib( POS_ID_FACE,MainPlayer:getFace())
		setAvatarAttrib( POS_ID_HEAD,MainPlayer:getHead())
		local headId = 0;
		if t_SelectPreViewItem[EQUIP_POS_FASHION_HEAD] ~= nil then
			headId = t_SelectPreViewItem[EQUIP_POS_FASHION_HEAD][1]
			if getItemDisplayInfoHairStyle( headId ) <= 1 then
				setAvatarAttrib( POS_ID_HAIR,0 );
			end
		end

		for pos,item in pairs( t_SelectPreViewItem ) do
			setAvatarAttrib( convertEquipPos2PosId(pos),item[1] )
		end
		if headId == 0 and Eqiup:getAvatarId( POS_ID_FASHION_HEAD ) ~= 0 then
			g_ConfigFashionFair();
		end
		
		for j=1, CLIENT_MAX_AVATAR_ATTRIB do
			if Eqiup:getAvatarId( j-1 ) ~= 0 then
				if j == 1 then
					modelView:setAvatarAttrib(0, j-1, Eqiup:getAvatarId( j-1 ),Eqiup:getAvatarStarLv( j-1 ), true );
				else
					modelView:setAvatarAttrib(0, j-1, Eqiup:getAvatarId( j-1 ),Eqiup:getAvatarStarLv( j-1 ) );
				end
			end
		end
		modelView:setEntityAvatar( 0 );

		local leftarmid		= getAvatarAttrib( POS_ID_LEFTHAND );
		local rightarmid	= getAvatarAttrib( POS_ID_RIGHTHAND );	
		
		local t_AvartarControl	= GetAvartarControl();
		local nAnimID = t_AvartarControl:getAvatarAnim( {	["race"] = GetInitRace( nRace ), ["leftHandItem"] = leftarmid, 
															["rightHandItem"] = rightarmid } );
		modelView:playEntityAnim(0, nAnimID, nil )
		modelView:setPetScale( 0, 0.70, 0.70, 0.70 )
		modelView:setEntityPosition( 0, 0, 30, 0 )

		if not FashionPreviewBtn:IsShown() then
			FashionPreviewBtn:Show();
		end
	end
end

function FashionViewOnClick( btn )
	if btn:GetClientUserData( 0 ) ~= 0 and btn:GetClientID() ~= 0 then
		t_SelectPreViewItem[btn:GetClientID()] = nil;
		FashionPreviewUpdateModel();
		if FashionPreviewBuyFrame:IsShown() then
			FashionPreviewBuyFrame:AddLevelRecursive(1);
			UpdateFashionPreviewBuyFrame();
		end
	end
end

function FashionPreviewBuyAllBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local container	= mainplayer:getContainer();
	local lockState = container:getContainerLockState();
	if lockState == CONTAINER_LOCK_STATE_LOCK then
		ShowMidTips("������֤������ȫ��");
		return;
	end
	local count = 0;
	local money = 0;
	for _,item in pairs( t_SelectPreViewItem ) do
		count = count + 1;
		local itemData = MarketManager:getItemByID( item[2] );
		if itemData ~= nil then
			money = money + math.floor( itemData.BuyPrice * ( 100 - itemData.Sale ) / 100 );
		end
	end
	if count == 0 then
		ShowMidTips("��ǰû����Ҫ�����ʱװ");
		return;
	end
	if GetMaxCanBuyNum( money ) == 0 then
		ShowMidTips("��ǰ���ϵ�" .. (nCurBuyType == BUY_TYPE_TICKET and MONEY_TYPE_GODCOIN_NAME or MONEY_TYPE_BIND_GODCOIN_NAME) .. "���㣬�޷������Դ���ʱװ");
		return;
	end
	FashionPreviewBuyFrameEdit:SetText(1);
	UpdateFashionPreviewBuyFrame();
	MarketBuyItemFrame:Hide();
	MarketContributeFrame:Hide();
	FashionPreviewBtnBuyAllBtnUVAnimationTex:Hide();
end

function FashionPreviewResetBtn_OnClick()
	FashionPreviewBtn:Hide();
end

function FashionPreviewBtn_OnMouseDown()
	if arg1 == "LeftButton" then
		nLastFashionMouseClickX = GetCursorPosX();
	end
end

function FashionPreviewBtn_OnMouseMove()
	if this:GetPushedState() then
		local nCurMouseClickX = GetCursorPosX();
		nModelAngel = nModelAngel - (nCurMouseClickX - nLastFashionMouseClickX) ;
		FashionPreviewBtnModelView:setEntityRotate(0, nModelAngel);
		nLastFashionMouseClickX = nCurMouseClickX;
	end
end

function FashionPreviewBtn_OnHide()
	curPreViewType = MARKET_PREVIEW_EQUIP;
	t_SelectPreViewItem = {};
	nModelAngel = 0;
	nLastFashionMouseClickX = 0;
	FashionPreviewBuyFrame:Hide();
	FashionPreviewBtnBuyAllBtnUVAnimationTex:Hide();
end

function UpdateFashionPreviewBuyFrame()
	local costMoney = 0;
	local szText = "#cf4bc24ʱװ��#n\n";
	local count = 0;
	for _,item in pairs( t_SelectPreViewItem ) do
		local itemData = MarketManager:getItemByID( item[2] );
		if itemData ~= nil then
			costMoney = costMoney + math.floor( itemData.BuyPrice * ( 100 - itemData.Sale ) / 100 );
		end
		local itemDef = getItemDef( item[1] );
		if itemDef ~= nil then
			local t_NameColor = GetItemNameColor( itemDef );
			szText = szText ..  RGBColorToRichColor( t_NameColor ) .. itemDef.Name .. "#n\n";
		end
		count = count + 1;
	end

	if count == 0 then
		FashionPreviewBuyFrame:Hide();
		return;
	end

	local PriceFont = getglobal("FashionPreviewBuyFrameMoneyValue");
	PriceFont:SetText( costMoney );
	FashionPreviewBuyFramePriceTex:SetPoint( "topleft", "FashionPreviewBuyFrameMoneyValue", "topleft", PriceFont:GetTextExtentWidth( PriceFont:GetText() ) / GetScreenScaleY()+ 5 , 
									-2 );
	local rich = getglobal( "FashionPreviewBuyFrameDescRich" );
	rich:SetText( szText, 255, 255, 190 );
	if rich:GetTextLines() > 5 then
		FashionPreviewBuyFrame:SetHeight( 170 + rich:GetTextLines() * 16 );
		FashionPreviewBuyFrameCenterBG:SetHeight( rich:GetTextLines() * 16 + 8 );
	else
		FashionPreviewBuyFrame:SetHeight( 248 );
		FashionPreviewBuyFrameCenterBG:SetHeight( 88 );
	end

	local nMaxBuyNum = GetMaxCanBuyNum( costMoney );

	FashionPreviewBuyFrameBtn:SetClientUserData( 0, costMoney )
	FashionPreviewBuyFrameBtn:SetClientUserData( 1, nMaxBuyNum )
	FashionPreviewBuyFrameBtn:SetClientUserData( 2, count )

	if not FashionPreviewBuyFrame:IsShown() then
		FashionPreviewBuyFrame:Show();
	else
		FashionPreviewBuyFrame:AddLevelRecursive(1);
	end
end

function GetMaxCanBuyNum( costMoney )
	local nMaxBuyNum = 0;
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer ~= nil and costMoney > 0 then
		if nCurBuyType == BUY_TYPE_TICKET then
			nMaxBuyNum = math.floor( mainplayer:getGodCoin() / costMoney );
		else
			nMaxBuyNum = math.floor( mainplayer:getBindGodCoin() / costMoney );
		end
	end
	return nMaxBuyNum > 99 and 99 or nMaxBuyNum;
end

function UpdateFashionPreviewBuyFrameMoneySymbol()
	local priceTex 		= getglobal("FashionPreviewBuyFramePriceTex");
	local allMoneyTex	= getglobal("FashionPreviewBuyFrameAllGoldTex");
	if nCurBuyType == BUY_TYPE_TICKET then
		priceTex:SetTexUV( 99, 494, 17, 18 ); 	
		allMoneyTex:SetTexUV( 99, 494, 17,18 );
	else 
		priceTex:SetTexUV( 57, 494, 20,18 ); 
		allMoneyTex:SetTexUV( 57, 494, 20,18 );
	end 
end

function FashionPreviewBuyFrame_OnShow()
	SetFocusFrame( "FashionPreviewBuyFrameEdit" );
	UpdateFashionPreviewBuyFrameMoneySymbol();
end

function FashionPreviewBuyFrame_OnHide()
end

function FashionPreviewBuyFrame_OnLoad()
	FashionPreviewBuyFrameName:SetText("�Դ�ʱװ");
	FashionPreviewBuyFrameBtnBoxTexture:Show();
	FashionPreviewBuyFrameBtnIconTexture:SetTexture( "uires\\TuPianLei\\WuPin\\ShiChuangShiZhuang.tga" );
	FashionPreviewBuyFrameBtnUVAnimationTex:SetTexture(  "uires\\ui\\TeXiao\\ZhuangBei  13.tga" );
	FashionPreviewBuyFrameBtnUVAnimationTex:Show();
end

function FashionPreviewBuyFrameEdit_OnUpdate()
	local szText = FashionPreviewBuyFrameEdit:GetText();
	if szText == "" then
		FashionPreviewBuyFramePrice:SetText( "�����ܼۣ�" .. 0 );
		return;
	end
	local itemNum = tonumber( szText );
	if itemNum == nil or itemNum < 0 then
		FashionPreviewBuyFrameEdit:SetText( 0 );
		FashionPreviewBuyFramePrice:SetText( "�����ܼۣ�" .. 0 );
		return;
	end
	local nPrice = FashionPreviewBuyFrameBtn:GetClientUserData( 0 )
	local nCanBuyNum = FashionPreviewBuyFrameBtn:GetClientUserData( 1 )
	
	if itemNum > nCanBuyNum then
		itemNum = nCanBuyNum;
		FashionPreviewBuyFrameEdit:SetText( nCanBuyNum )
	end
	CheckEditNumberInput(FashionPreviewBuyFrameEdit);
	local nTotalPrice = nPrice*itemNum;
	FashionPreviewBuyFramePrice:SetText( "�����ܼۣ�" .. nTotalPrice );
	
	local AllCostFont = getglobal("FashionPreviewBuyFramePrice");
	FashionPreviewBuyFrameAllGoldTex:SetPoint( "topleft", "FashionPreviewBuyFramePrice", "topleft", AllCostFont:GetTextExtentWidth( AllCostFont:GetText() ) / GetScreenScaleY()+ 5 , 
									-2 );
end

function FashionPreviewBuyFrameEditConfirmBtn_OnClick()
	FashionPreviewBuyFrameConfirmBtn_OnClick();
end

function FashionPreviewBuyFrameConfirmBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local container	= mainplayer:getContainer();
	local lockState = container:getContainerLockState();
	if lockState == CONTAINER_LOCK_STATE_LOCK then
		ShowMidTips("������֤������ȫ��");
		return;
	end
	local nCanBuyNum = FashionPreviewBuyFrameBtn:GetClientUserData( 1 );
	if nCanBuyNum == 0 then
		ShowMidTips("��ǰ���ϵ�" .. (nCurBuyType == BUY_TYPE_TICKET and MONEY_TYPE_GODCOIN_NAME or MONEY_TYPE_BIND_GODCOIN_NAME) .. "���㣬�޷������Դ���ʱװ");
		return;
	else
		local num = tonumber( FashionPreviewBuyFrameEdit:GetText() );
		if num == nil or num == 0 then
			ShowMidTips("��������Ҫ���������");
			return;
		elseif num > nCanBuyNum then
			ShowMidTips("��ǰ���ϵ�" .. (nCurBuyType == BUY_TYPE_TICKET and MONEY_TYPE_GODCOIN_NAME or MONEY_TYPE_BIND_GODCOIN_NAME) .. "���㣬�޷������Դ���ʱװ");
			return;
		end

		t_BuyList = {};
		for _,item in pairs ( t_SelectPreViewItem ) do
			table.insert( t_BuyList, { item[2], num, nCurBuyType } );
		end
		MarketManager:SetBuyPreviewState( true );
		FashionPreviewBuyFrame:Hide();
	end
end

function BuyAllPreViewItem()
	if #t_BuyList ~= 0 then
		MarketManager:requestBuy( t_BuyList[1][1], t_BuyList[1][2], 0, "", "", t_BuyList[1][3] );
		table.remove( t_BuyList, 1 );
	else
		MarketManager:SetBuyPreviewState( false );
	end
end

--���� -1
function FashionPreviewBuyFrame_ScrollDownBtn_OnClick()
	local szText = FashionPreviewBuyFrameEdit:GetText();
	local itemNum = 0;
	if szText == "" then
		itemNum = 0;
	else	
		itemNum = tonumber( szText );
	end
	if itemNum == nil or itemNum == 0 then
		FashionPreviewBuyFrameEdit:SetText( 0 );
		return;
	end
	FashionPreviewBuyFrameEdit:SetText(itemNum - 1);
end

--���� +1
function FashionPreviewBuyFrame_ScrollUpBtn_OnClick()
	local szText = FashionPreviewBuyFrameEdit:GetText();
	local itemNum = 0;
	if szText == "" then
		itemNum = 0;
	else
		itemNum = tonumber( szText );
	end
	if itemNum == nil then
		FashionPreviewBuyFrameEdit:SetText( 0 );
		return;
	end
	FashionPreviewBuyFrameEdit:SetText(itemNum + 1);
end

--�������
function FashionPreviewBuyFrame_ScrollTopBtn_OnClick()
	local nCanBuyNum = FashionPreviewBuyFrameBtn:GetClientUserData( 1 );
	if nCanBuyNum ~= nil then
		FashionPreviewBuyFrameEdit:SetText( nCanBuyNum );
	else
		FashionPreviewBuyFrameEdit:SetText( 0 );
	end
end

function SetFashionPreview( pos, itemId, shopId )
	if t_SelectPreViewItem[pos] == nil then
		t_SelectPreViewItem[pos] = { itemId, shopId };
	end
end

------------------------��ҽ������---------------------------------

local nCurPageNum		= 1;	--��ǰ��ʾ��ҳ��
local nMaxPageNum		= 10;	--���ҳ��
local tTime = {[1] = 1,[2] = 2,[3] = 5,[4] = 12,[5] = 24};
local nCurGoldPageNum	= 1;    --��ǰ��ҳ�����ʾ��ҳ��
local nCurSilverPageNum	= 1;    --��ǰ���ҳ�����ʾ��ҳ��

local nTradeListCurSel = 0;	-- �����б��� ��ǰѡ�е���

-- ������ˢ��
function MarketBuyTicketFrame_OnUpdate()
	local listbtn = getglobal( "TicketListInfo"..nTradeListCurSel );
	if listbtn ~= nil then
		local nHeigh = listbtn:GetClientUserData(0);
		local nLow = listbtn:GetClientUserData(1);
		local nPrice = listbtn:GetClientUserData(2);
		local nTotalNum = listbtn:GetClientUserData(3);
		local szNum = TicketBuyNumEditBox:GetText();

		if szNum == "" then
			return;
		end
		local nNum = tonumber( szNum );
		if nNum == nil or nNum < 0 then
			MarketBuyTicketFrameMainBackBtnPrice:SetText(0);
			TicketBuyNumEditBox:SetText(0);
			return;
		end

		if nNum > nTotalNum then
			nNum = nTotalNum;
			TicketBuyNumEditBox:SetText(nNum);
		end
		local MainPlayer = ActorMgr:getMainPlayer();
		if MainPlayer == nil then return end

		local nMoneyCanBuyNum = math.ceil(MainPlayer:getMoney()/nPrice);
		if nNum > nMoneyCanBuyNum then
			nNum = nMoneyCanBuyNum;
			TicketBuyNumEditBox:SetText(nNum);
		end
		local nTotalPrice = nNum * nPrice;
		MarketBuyTicketFrameMainBackBtnPrice:SetText(nTotalPrice);
		CheckEditNumberInput(TicketBuyNumEditBox);
	end
end

--�� ��������
function MarketSaleTicketFrame_ScrollUpBtnOnClick()
	local szNum = TicketSaleNumEditBox:GetText();
	local nNum = 0;
	if szNum == nil or szNum == "" then
		nNum = 0;
	else
		nNum = tonumber(szNum);
	end
	TicketSaleNumEditBox:SetText(nNum + 1)
end

--�� ��������
function MarketSaleTicketFrame_ScrollDownBtnOnClick()
	local szNum = TicketSaleNumEditBox:GetText();
	local nNum = 0;
	if szNum == nil or szNum == "" then
		nNum = 0;
	else
		nNum = tonumber(szNum);
	end
	if nNum == 0 then
		TicketSaleNumEditBox:SetText(0);
		return
	end
	TicketSaleNumEditBox:SetText(nNum - 1);
end

--���� ��������
function MarketBuyTicketFrame_ScrollUpBtnOnClick()
	local szNum = TicketBuyNumEditBox:GetText();
	local nNum = 0;
	if szNum == nil or szNum == "" then
		nNum = 0;
	else
		nNum = tonumber(szNum);
	end
	TicketBuyNumEditBox:SetText(nNum + 1)
end 

--���� ��������
function MarketBuyTicketFrame_ScrollDownBtnOnClick()
	local szNum = TicketBuyNumEditBox:GetText();
	local nNum = 0;
	if szNum == nil or szNum == "" then
		nNum = 0;
	else
		nNum = tonumber(szNum);
	end
	if nNum == 0 then
		TicketBuyNumEditBox:SetText(0);
		return
	end
	TicketBuyNumEditBox:SetText(nNum - 1);
end

--��ʼ�ĺ�����

function MarketManagerFrame_TradeBtn_OnClick()

	if MarketTicketFrame:IsShown() then
		MarketTicketFrame:Hide();
	else
		MarketTicketFrame:Show();
		MarketTicketBuyBtn_OnClick();
	end 
end

function MarketTickUpdateMoney()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	MarketTicketFrameMoneyInfoBackBtnMoneyValue:SetText(MainPlayer:getMoney());
	MarketTicketFrameMoneyInfoBackBtnTicketValue:SetText(MainPlayer:getGodCoin());
end

local TradeType = 0;
local MAX_TRADE_INFO_NUM = 14;
local SellFlag = false;
local BuyFlag = false;
local t_tradeListInfo = {
					{ nCurPage = 1, nTotalPage = 1,},
					{ nCurPage = 1, nTotalPage = 1,},
					{ nCurPage = 1, nTotalPage = 1,},
				};
local t_tradeInfoSort = {
					{ SortType = 1, SortFlag1 = -1, SortFlag2 = -1, SortCategery = 2,SortNum = 0,},
					{ SortType = 2, SortFlag1 = -1, SortFlag2 = 1, SortCategery = 2,SortNum = 0,},
				};

function MarketTicketFrameButtonLayout()
	local szText;
	if TradeType == 1 or TradeType == 2 then
		MarketTicketFrameGoldListBtn:Show();
		MarketTicketFrameTicketTradeBtn:Hide();
		MarketTicketFrameConfirmBtn:SetText("�� ��");
		MarketTicketFrameConfirmBtn:Show(); 
		szText = "#G˫���ɽ��й�����������۵����׳ɹ���Ľ��׽�ͨ��#n#L�ʼ�#n#G�ķ�ʽ���͸���#n"
		MarketTicketFrameHeadRichFont:SetText( szText, 66,  168, 61 );
		MarketTicketFrameHeadRichFont:Show();
	elseif TradeType == 3 then
		MarketTicketFrameGoldListBtn:Show();
		MarketTicketFrameTicketTradeBtn:Hide();
		MarketTicketFrameConfirmBtn:SetText("�� ��");
		MarketTicketFrameConfirmBtn:Show(); 
		szText = "#G˫���ɽ��г��������������ɹ�����ע�����#n"
		MarketTicketFrameHeadRichFont:SetText( szText, 66,  168, 61 );
		MarketTicketFrameHeadRichFont:Show();
	elseif TradeType == 4 then
		MarketTicketFrameGoldListBtn:Hide();
		MarketTicketFrameTicketTradeBtn:Show();
		MarketTicketFrameConfirmBtn:Hide(); 
		MarketTicketFrameHeadRichFont:Hide(); 
		
	end
	GoldListBtnSaleOwnerBtn:Disable();
	GoldListBtnSaleLeaveTimeBtn:Disable();
end
 
 function MarketTicketFrameHeadRichFont_OnClick()
 	if TradeType == 1 or TradeType == 2 then
 		MailBtn_OnClick();
 	end	
 end
 	
 function UpdateMarketFrameRate()
 	if TradeType == 2 then 
 		MarketTicketFrameRateFont:SetText( "����ƽ���ۣ�"..GetGoldRate() );
 	end
 end
 
--��ҳ����б�ť
function MarketTicketSaleBtn_OnClick()	
	if ButtonClickForbidden() then	return  end 
	nTradeListCurSel = 0;
	DisCheckAllMarketLable();
	MarketTicketSaleBtn:Checked();
	if  TradeType ~= 1 then
		TradeType = 1;
		SellFlag = true;
		InitMarketTicketSaleInfo();
		MarketTicketFrameRateFont:SetText("");
	end
	
	SellFlag = false;
	if t_tradeInfoSort[TradeType]["SortNum"] == 0 then
		GoldListBtnSaleAccountNumBtnFont:SetText("ȫ��");
	end
	
	GoldListBtnSaleAccountNumBtnTex:Show();
	MarketTicketFrameButtonLayout();
end

--���ҳ����б�ť
function MarketTicketBuyBtn_OnClick()
	if ButtonClickForbidden() then	return  end 
	nTradeListCurSel = 0;
	DisCheckAllMarketLable();
	MarketTicketBuyBtn:Checked();
	if TradeType ~= 2 then
		TradeType = 2;
		BuyFlag = true;
		InitMarketTicketSaleInfo();
		MarketTicketFrameRateFont:SetText( "����ƽ���ۣ�"..GetGoldRate() );
	end
	BuyFlag = false;
	if t_tradeInfoSort[TradeType]["SortNum"] == 0 then
		GoldListBtnSaleAccountNumBtnFont:SetText("ȫ��");
	end
	
	GoldListBtnSaleAccountNumBtnTex:Show();
	MarketTicketFrameButtonLayout();
end	

--��ҽ��װ�ť
function MarketTicketTradeBtn_OnClick()
	if ButtonClickForbidden() then	return  end 
	nTradeListCurSel = 0;
	DisCheckAllMarketLable();
	MarketTicketTradeBtn:Checked();
	TradeType = 4;
	MarketTicketFrameButtonLayout();
	
	MarketTicketFrameRateFont:SetText("");
end

--�ҵĹҵ���ť
function MarketOrderSelectBtn_OnClick()	
	if ButtonClickForbidden() then 	return	end 
	nTradeListCurSel = 0;

	DisCheckAllMarketLable();
	MarketOrderSelectBtn:Checked();
	TradeType = 3;
	GoldListBtnSaleAccountNumBtnFont:SetText("��������");
	GoldListBtnSaleAccountNumBtnTex:Hide();
	MarketTicketFrameButtonLayout();
	
	ShowGoldTradeListInfo();
	GoldListBtnSalePriceBtnSortTex:Hide();
	GoldListBtnSaleUnitPriceBtnSortTex:Hide();
	
	MarketTicketFrameRateFont:SetText("");
end

function DisCheckAllMarketLable()
	MarketTicketSaleBtn:DisChecked();
	MarketTicketBuyBtn:DisChecked();
	MarketTicketTradeBtn:DisChecked();
	MarketOrderSelectBtn:DisChecked();
	for i = 1,MAX_TRADE_INFO_NUM do
		local btn = getglobal("TicketListInfo"..i);
		btn:DisChecked();
	end
end

function ShowGoldTradeListInfo()
	ClearListBtnInfo();
	for i = 1,MAX_TRADE_INFO_NUM do
		local OneTradeInfo =  MarketManager:getOneTradeInfoList( t_tradeListInfo[TradeType]["nCurPage"] - 1, i - 1, TradeType );
		if OneTradeInfo == nil then
			break;
		end
		local btn = getglobal("TicketListInfo"..i);
		BondingTradeListFrame( OneTradeInfo, btn ) ;
		btn:Show();
	end
end
function BondingTradeListFrame( OneTradeInfo, button )
	local nCurtime		= getServerTime().sec;
	local leaveTime		= math.ceil( ( OneTradeInfo.TradeRecordData.CountedDownTime - nCurtime ) / 3600 );
	if leaveTime < 1 then
		leaveTime = 1;
	end
	local nOwnerName	= OneTradeInfo.TradeRecordData.OwnerName;
	local mainPlayer	= ActorMgr:getMainPlayer();
	if nOwnerName == "" then
		nOwnerName = "����";
	end
	if TradeType == 3 then
		nOwnerName = "";
	end
	local unitPrice = ( OneTradeInfo.UnitPrice + 0.005 ) - ( OneTradeInfo.UnitPrice + 0.005 )%0.01;
	local t_GoldTradeListFonts = {
									[button:GetName() .. "NumberFont"]		= { value = OneTradeInfo.TradeRecordData.Gold },
									[button:GetName() .. "OwnerFont"]		= { value = nOwnerName },	
									[button:GetName() .. "PriceFont"]		= { value = OneTradeInfo.TradeRecordData.Money },	
									[button:GetName() .. "LeaveTimeFont"]	= { value = leaveTime.."Сʱ" },
									[button:GetName() .. "UnitPriceFont2"]	= { value = unitPrice.."������1��" },

								}
	local t_SilverTradeListFonts = {
									[button:GetName() .. "NumberFont"]		= { value = OneTradeInfo.TradeRecordData.Money },
									[button:GetName() .. "OwnerFont"]		= { value = nOwnerName },	
									[button:GetName() .. "PriceFont"]		= { value = OneTradeInfo.TradeRecordData.Gold },	
									[button:GetName() .. "LeaveTimeFont"]	= { value = leaveTime.."Сʱ" },
									[button:GetName() .. "UnitPriceFont2"]	= { value = "1����"..unitPrice.."��" },
								}
	
	local numberTex 	= getglobal( button:GetName().."NumberTex" );
	local priceTex 		= getglobal( button:GetName().."PriceTex" );

--	local unitPriceFont = getglobal( button:GetName().."UnitPriceFont2");
	
	if OneTradeInfo.TradeRecordData.TradeType == 0 then    
		for name, data in pairs( t_GoldTradeListFonts ) do
			local font = getglobal( name );
			font:SetText( data.value );
		end 
		numberTex:SetTexUV(99, 494, 17, 18);
		priceTex:SetTexUV(117, 494, 17, 18);
	else
		for name, data in pairs( t_SilverTradeListFonts ) do
			local font = getglobal( name );
			font:SetText( data.value );
		end 
		numberTex:SetTexUV(117, 494, 17, 18);
		priceTex:SetTexUV(99, 494, 17, 18);
	end
	button:SetClientUserData(0,OneTradeInfo.TradeRecordData.TradeID );
	button:SetClientUserData(1,OneTradeInfo.TradeRecordData.TradeType );
end

function ClearListBtnInfo()
	for i = 1,MAX_TRADE_INFO_NUM do
		local btn = getglobal( "TicketListInfo"..i );
		btn:Hide();
	end
	for i = 1, 2 do
		t_tradeListInfo[i]["nTotalPage"] = MarketManager:getCurTradeTotalPage( i );
	end
	local nPageFont = getglobal("TicketListLeftPageFont");
	nPageFont:SetText("��"..t_tradeListInfo[TradeType]["nCurPage"].."/"..t_tradeListInfo[TradeType]["nTotalPage"].."ҳ");
end

--��������ť
function MarketTicketFrameAccountNumBtn_OnClick()
	if TradeType == 3 then return end
	if ButtonClickForbidden() then return end
	if TicketFrameTradeNumSelectBtn:IsShown() then
		TicketFrameTradeNumSelectBtn:Hide();
	else
		TicketFrameTradeNumSelectBtn:Show();
	end
end

function TicketFrameTradeNumSelectBtn_OnShow()
	TicketFrameTradeNumSelectBtn1Font:SetText("ȫ��");
	TicketFrameTradeNumSelectBtn1:SetClientUserData( 0,0 );
	if TradeType == 1 then
		TicketFrameTradeNumSelectBtn2Font:SetText("10��");
		TicketFrameTradeNumSelectBtn2:SetClientUserData( 0,10 );
		TicketFrameTradeNumSelectBtn3Font:SetText("50��");
		TicketFrameTradeNumSelectBtn3:SetClientUserData( 0,50 );
		TicketFrameTradeNumSelectBtn4Font:SetText("100��");
		TicketFrameTradeNumSelectBtn4:SetClientUserData( 0,100 );
		TicketFrameTradeNumSelectBtn5Font:SetText("500��");
		TicketFrameTradeNumSelectBtn5:SetClientUserData( 0,500 );
		TicketFrameTradeNumSelectBtn6Font:SetText("1000��");
		TicketFrameTradeNumSelectBtn6:SetClientUserData( 0,1000 );
	elseif TradeType == 2 then
		TicketFrameTradeNumSelectBtn2Font:SetText("1000��");
		TicketFrameTradeNumSelectBtn2:SetClientUserData( 0,1000 );
		TicketFrameTradeNumSelectBtn3Font:SetText("5000��");
		TicketFrameTradeNumSelectBtn3:SetClientUserData( 0,5000 );
		TicketFrameTradeNumSelectBtn4Font:SetText("10000��");
		TicketFrameTradeNumSelectBtn4:SetClientUserData( 0,10000 );
		TicketFrameTradeNumSelectBtn5Font:SetText("50000��");
		TicketFrameTradeNumSelectBtn5:SetClientUserData( 0,50000 );
		TicketFrameTradeNumSelectBtn6Font:SetText("100000��");
		TicketFrameTradeNumSelectBtn6:SetClientUserData( 0,100000 );
	end
end
function TicketFrameTradeNumSelectBtn_OnClick()
	TicketFrameTradeNumSelectBtn:Hide();
	if t_tradeInfoSort[TradeType]["SortNum"] ~= this:GetClientUserData(0) then
		t_tradeInfoSort[TradeType]["SortNum"] = this:GetClientUserData(0)
		t_tradeListInfo[TradeType]["nCurPage"] = 1;
	end
	if t_tradeInfoSort[TradeType]["SortNum"] == 0 then
		GoldListBtnSaleAccountNumBtnFont:SetText("ȫ��");
	else
		GoldListBtnSaleAccountNumBtnFont:SetText("������"..t_tradeInfoSort[TradeType]["SortNum"].."��");
	end
	if t_tradeInfoSort[TradeType]["SortCategery"] == 1 then
		SellFlag = true;
		MarketTicketFrameSalePriceBtn_OnClick();
	elseif t_tradeInfoSort[TradeType]["SortCategery"] == 2 then
		SellFlag = true;
		MarketTicketFrameSaleUnitPriceBtn_OnClick();
	end
	SellFlag = false;
end

-- ��Ҫ���ж�������
function SortTradeList( tradeNum )
	if tradeNum == 0  and TradeType == 1 then
		--��ҳ��� SortType = 1
		MarketManager:sortTradeRecord( 1 , -1 ,TradeType );
	elseif tradeNum == 0  and TradeType == 2 then
		--���ҳ��� SortType = 2
		MarketManager:sortTradeRecord( 2 , -1 ,TradeType );
	end
end

function MarketTicketFrameSalePriceBtn_OnClick()
	if TradeType > 2 then
		return;
	end
	if ButtonClickForbidden() then	return  end 
	--�������
	local TradeNum = t_tradeInfoSort[TradeType]["SortNum"];
	MarketManager:setTradeRecordByNum( TradeType, TradeNum );
	--��������
	if not ( SellFlag or BuyFlag ) then
		t_tradeInfoSort[TradeType]["SortFlag1"] = ( t_tradeInfoSort[TradeType]["SortFlag1"] == 1) and -1 or 1;
	end
	local sortFlag = t_tradeInfoSort[TradeType]["SortFlag1"];
	if TradeType == 1 then
		t_tradeInfoSort[TradeType]["SortType"] = 2;
	else
		t_tradeInfoSort[TradeType]["SortType"] = 1;
	end
	MarketManager:sortTradeRecord( t_tradeInfoSort[TradeType]["SortType"], sortFlag ,TradeType );
--	SortTradeList( TradeNum );
	--��ʾ���
	ShowGoldTradeListInfo();
	
	t_tradeInfoSort[TradeType]["SortCategery"] = 1;
	
	local sortTex = getglobal( "GoldListBtnSalePriceBtnSortTex");

	GoldListBtnSalePriceBtnSortTex:Show();
	GoldListBtnSaleUnitPriceBtnSortTex:Hide();
	ChangeRecordSortTexture(  sortTex,sortFlag );
end

function MarketTicketFrameSaleUnitPriceBtn_OnClick()
	if TradeType > 2 then
		return;
	end
	if ButtonClickForbidden() then	return  end 
	local TradeNum = t_tradeInfoSort[TradeType]["SortNum"];
	MarketManager:setTradeRecordByNum( TradeType, TradeNum );

	t_tradeInfoSort[TradeType]["SortType"] = 3;
	if not ( SellFlag or BuyFlag ) then
		t_tradeInfoSort[TradeType]["SortFlag2"] = ( t_tradeInfoSort[TradeType]["SortFlag2"] == 1) and -1 or 1;
	end

	MarketManager:sortTradeRecord( t_tradeInfoSort[TradeType]["SortType"] ,t_tradeInfoSort[TradeType]["SortFlag2"],TradeType );
--	SortTradeList( TradeNum );
	ShowGoldTradeListInfo();
	t_tradeInfoSort[TradeType]["SortCategery"] = 2;

	local sortTex 	= getglobal("GoldListBtnSaleUnitPriceBtnSortTex");
	local sortFlag	= t_tradeInfoSort[TradeType]["SortFlag2"];

	GoldListBtnSalePriceBtnSortTex:Hide();
	GoldListBtnSaleUnitPriceBtnSortTex:Show();
	ChangeRecordSortTexture(  sortTex,sortFlag );
end

function ChangeRecordSortTexture( sortTex,sortFlag )
	if TradeType == 1 and t_tradeInfoSort[TradeType]["SortCategery"] == 2 then 
		if sortFlag == 1 then
			sortTex:SetAngle(0);
		else
			sortTex:SetAngle(180);
		end
	else 
		if sortFlag == -1 then
			sortTex:SetAngle(0);
		else
			sortTex:SetAngle(180);
		end
	end 
end

function SortRecordWithCategery()
	if t_tradeInfoSort[TradeType]["SortCategery"] == 1 then
		MarketTicketFrameSalePriceBtn_OnClick();
	elseif t_tradeInfoSort[TradeType]["SortCategery"] == 2 then
		MarketTicketFrameSaleUnitPriceBtn_OnClick();
	end
end
--������ҳ
function TradeListLeftPageBtn_OnClick()
	if ButtonClickForbidden() then	return  end
	local nCurPage = t_tradeListInfo[TradeType]["nCurPage"];
	t_tradeListInfo[TradeType]["nCurPage"] = nCurPage - 1 > 1 and nCurPage - 1 or 1;
	ShowGoldTradeListInfo();
end

function TradeListRightPageBtn_OnClick()
	if ButtonClickForbidden() then	return  end
	local nCurPage = t_tradeListInfo[TradeType]["nCurPage"];
	t_tradeListInfo[TradeType]["nCurPage"] = nCurPage + 1 < t_tradeListInfo[TradeType]["nTotalPage"] 
												and nCurPage + 1 
												or t_tradeListInfo[TradeType]["nTotalPage"];
	ShowGoldTradeListInfo();
end

function TradeListToLastPageBtn_OnClick()
	if ButtonClickForbidden() then	return  end
	t_tradeListInfo[TradeType]["nCurPage"] = t_tradeListInfo[TradeType]["nTotalPage"];
	ShowGoldTradeListInfo();
end

function TradeListToFirstPageBtn_OnClick()
	if ButtonClickForbidden() then	return  end
	t_tradeListInfo[TradeType]["nCurPage"] = 1;
	ShowGoldTradeListInfo();
end
--����б�
function TicketListInfoOnClick()
	if ButtonClickForbidden() then	return  end 
	nTradeListCurSel = this:GetClientID();
	for i=1, MAX_TRADE_INFO_NUM do
		local listbtn = getglobal( "TicketListInfo"..i );
		listbtn:DisChecked();
	end
	this:Checked();
	MarketBuyTicketFrame:Hide();
	MarketSaleTicketFrame:Hide();
end
--˫���б�
function TicketListInfoOnDoubleClick()
	if ButtonClickForbidden() then	return  end  
	nTradeListCurSel = this:GetClientID();
	for i=1, MAX_TRADE_INFO_NUM do
		local listbtn = getglobal( "TicketListInfo"..i );
		listbtn:DisChecked();
	end
	this:Checked();
	TicketListBuyConfirm_OnClick();
end
--����ť
function TicketListBuyConfirm_OnClick()
	if TradeType == 1 then
		TradeListBuyGoldConfirm();
	elseif TradeType == 2 then
		TicketListBuySilverConfirm();
	elseif TradeType == 3 then
		TicketListCancleOrderConfirm();
	end
end
--���ذ�ť
function MarketTicketFrame_CancleBtn()
	MarketTicketFrame:Hide();
	MarketManagerFrame:Show();
end
-- ��ҳ��۽��׵�
function TradeListBuyGoldConfirm()	
	if ButtonClickForbidden() then	return  end  
	local mainplayer= ActorMgr:getMainPlayer();
	local container = mainplayer:getContainer();
	local lockState = container:getContainerLockState();
	if lockState == CONTAINER_LOCK_STATE_LOCK then
		ShowMidTips("������֤������ȫ��");
		return;
	end
	local listbtn = getglobal( "TicketListInfo"..nTradeListCurSel );
	if listbtn == nil or not listbtn:IsShown() then
		ShowMidTips("��ѡ������");
		return;
	end
	if listbtn ~= nil then
		MarketBuyTicketFrame:Show();
	end	
end
--�س����������
function MarketBuyTicketFrameConfirm_OnClick()
	if MarketBuyTicketFrame_ConfirmBtn:IsEnable() then
		MarketFrameGoldTradeConfirm_OnClick();
	end
	if MarketBuyTicketFrame:IsShown() then
		MarketBuyTicketFrame:Hide();
	end
end

function MarketFrameGoldTradeCancle_OnClick()
	MarketBuyTicketFrame:Hide();
	SellFlag = true;
	SetTradeListPageInfo( 1 )
	SortRecordWithCategery();
	SellFlag = false;
end

--��ҽ��׵�ȷ��
function MarketFrameGoldTradeConfirm_OnClick()
	if not MarketBuyTicketFrame:IsShown() then
		return;
	end
	
	local priceFont	= getglobal( "TicketListInfo"..nTradeListCurSel.."PriceFont" );
	local nPrice	= tonumber( priceFont:GetText() );
	local tradeFee = math.ceil( ( nPrice * 5 / 100 ) < 1 and 1 or nPrice * 5 / 100 );
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil or mainplayer:getMoney() < ( nPrice + tradeFee ) then
		ShowBuyMoneyFrame();
		return;
	end

	local listbtn = getglobal( "TicketListInfo"..nTradeListCurSel );
	if listbtn ~= nil then
		local nTradeID = listbtn:GetClientUserData(0);
		MarketManager:requestTrade( 0,nTradeID );
		MarketBuyTicketFrame:Hide();
		--ClearFocus();
	end
end
function MarketSaleTicketFrameConfirm_OnClick()
 	if  MarketSaleTicketFrame_ConfirmBtn:IsEnable() then
 		MarketFrameSilverTradeConfirm_OnClick();
 	end
 	if MarketSaleTicketFrame:IsShown() then
		MarketSaleTicketFrame:Hide();
	end
end

function MarketFrameSilverTradeCancle_OnClick()
	MarketSaleTicketFrame:Hide();
	BuyFlag = true;
	SetTradeListPageInfo( 2 )
	SortRecordWithCategery();
	BuyFlag = false;
end

--���ҽ��׵�ȷ��
function MarketFrameSilverTradeConfirm_OnClick()
	if not MarketSaleTicketFrame:IsShown() then
		return;
	end
	local priceFont	= getglobal( "TicketListInfo"..nTradeListCurSel.."PriceFont" );
	local mainplayer= ActorMgr:getMainPlayer();
	if mainplayer == nil or mainplayer:getGodCoin() < tonumber( priceFont:GetText() ) then
		ExchangePointFrame:SetClientUserData(0,1);
		MarketManager:requestQueryTianyuan();
		return;
	end

	local listbtn = getglobal( "TicketListInfo"..nTradeListCurSel );
	if listbtn ~= nil then
		local nTradeID = listbtn:GetClientUserData(0);
		MarketManager:requestTrade(1,nTradeID);
		MarketSaleTicketFrame:Hide();
		--ClearFocus();
	end
end

function TicketListCancleOrderConfirm()
	if ButtonClickForbidden() then	return  end  
	local listbtn = getglobal( "TicketListInfo"..nTradeListCurSel );
	if listbtn == nill or not listbtn:IsShown() then
		ShowMidTips("��ѡ������");
		return ;
	end
	MessageBox("��������","�������������ʽ�ֱ�Ӵ�������У�ȷ��������");
end

function TickListCancleOrderConfirm()
	local listbtn = getglobal("TicketListInfo"..nTradeListCurSel);
	if listbtn ~= nil then
		local nTradeID	= listbtn:GetClientUserData(0);
		local nType		= listbtn:GetClientUserData(1);
		local isExist	= MarketManager:checkMyTradeRecord( nTradeID );
		if isExist then
			MarketManager:destroyTradeRecord(nTradeID,nType);
		else
			ShowMidTips("����ʧ�ܣ��������׳ɹ��򵽴ｻ��ʱ��");
			SetTradeListPageInfo( 3 );
			ShowGoldTradeListInfo();
		end
	end
end

function MarketBuyTicketFrame_OnShow()
	local numFont	= getglobal( "TicketListInfo"..nTradeListCurSel.."NumberFont" );
	local priceFont	= getglobal( "TicketListInfo"..nTradeListCurSel.."PriceFont" );
	MarketBuyTicketFrameMainBackBtnNumberFont:SetText( numFont:GetText() );
	MarketBuyTicketFrameMainBackBtnPriceFont:SetText( priceFont:GetText() );
	local nPrice		= tonumber( priceFont:GetText() );
	local mainplayer= ActorMgr:getMainPlayer();

	if mainplayer:getMoney() < nPrice then
		MarketBuyTicketFrameMainBackBtnPriceFont:SetTextColor(255,0,0);
		
	else
		MarketBuyTicketFrameMainBackBtnPriceFont:SetTextColor(160,160,90);
	end
	local tradeFee = math.ceil( ( nPrice * 5 / 100 ) < 1 and 1 or nPrice * 5 / 100 );

	MarketBuyTicketFrameMainBackBtnTipsDesc1:SetText("���׺���ȡ5%�������ѣ�"..tradeFee.."����");
	if mainplayer:getMoney() < (nPrice + tradeFee )then
		MarketBuyTicketFrameMainBackBtnTipsDesc1:SetTextColor(255,0,0);
		MarketBuyTicketFrameMainBackBtnTipsDesc2:SetTextColor(255,0,0);
		--MarketBuyTicketFrame_ConfirmBtn:Disable();
	else
		MarketBuyTicketFrameMainBackBtnTipsDesc1:SetTextColor(161,143,96);
		MarketBuyTicketFrameMainBackBtnTipsDesc2:SetTextColor(161,143,96);
		--MarketBuyTicketFrame_ConfirmBtn:Enable();
	end
end

function MarketBuySilverFrame_OnShow()
	local listbtn	= getglobal( "TicketListInfo"..nTradeListCurSel );
	local numFont	= getglobal( "TicketListInfo"..nTradeListCurSel.."NumberFont" );
	local priceFont	= getglobal( "TicketListInfo"..nTradeListCurSel.."PriceFont" );
	MarketSaleTicketFrameMainBackBtnNumberFont:SetText( numFont:GetText() );
	MarketSaleTicketFrameMainBackBtnPriceFont:SetText( priceFont:GetText() );
	local nPrice		= tonumber( priceFont:GetText() );
	local mainplayer= ActorMgr:getMainPlayer();
	if mainplayer:getGodCoin() < nPrice then
		MarketSaleTicketFrameMainBackBtnPriceFont:SetTextColor( 255,0,0 );
		--MarketSaleTicketFrame_ConfirmBtn:Disable();
	else
		MarketSaleTicketFrameMainBackBtnPriceFont:SetTextColor(160,160,90);
		--MarketSaleTicketFrame_ConfirmBtn:Enable();
	end
	local nNum		= tonumber( numFont:GetText() )
	local tradeFee	=  math.ceil( ( nNum * 5 / 100 ) < 1 and 1 or (nNum * 5 / 100) ) ;
	
	MarketSaleTicketFrameMainBackBtnTipsDesc1:SetText("���׺���ȡ5%�������ѣ�"..tradeFee.."��");
	
end

-- ���ҳ��۽��׵�
function TicketListBuySilverConfirm()
	if ButtonClickForbidden() then	return  end 
	local mainplayer	= ActorMgr:getMainPlayer();
	local container = mainplayer:getContainer();
	local lockState = container:getContainerLockState();
	if lockState == CONTAINER_LOCK_STATE_LOCK then
		ShowMidTips("������֤������ȫ��");
		return;
	end
	local listbtn = getglobal( "TicketListInfo"..nTradeListCurSel );
	if listbtn == nill or not listbtn:IsShown() then
		ShowMidTips("��ѡ������");
		return;
	end
	if listbtn ~= nil then
		local MainPlayer = ActorMgr:getMainPlayer();
		MarketSaleTicketFrame:Show();
		--SetFocusFrame("TicketSaleNumEditBox");
	end
end

function MarketTicketFrameTicketTradeBtn_OnShow()
	local GoldNum			= tonumber( TradeGoldSaleNumChooseBtnFont:GetText() );
	local SilverNum			= tonumber( TradeSilverSaleNumChooseBtnFont:GetText() );
	local minGoldPrice		= MarketManager:getUnitPriceMin( 0 )  * GoldNum + 0.005;
	local sellPrice			= minGoldPrice - minGoldPrice % 0.01;
	local minSilverPrice;
	
	TicketTradeBtnGoldTipFont:SetText("���ʱ�ۣ�"..sellPrice.."����");
	if MarketManager:getUnitPriceMin( 1 ) == 0 then 
		minSilverPrice = 0;
	else 
		minSilverPrice	= SilverNum / MarketManager:getUnitPriceMin( 1 ) + 0.005;  --�������� ������λС��
	end
	local buyPrice = minSilverPrice - minSilverPrice % 0.01;
	TicketTradeBtnSilverTipFont:SetText("���ʱ�ۣ�"..buyPrice.."���");
	if minGoldUnitPrice == 0 then
		TradeFrameGoldSaleTipBtn:Hide();
	else
		TradeFrameGoldSaleTipBtn:Show();
	end
	if minSilverUnitPrice == 0 then
		TradeFrameSilverSaleTipBtn:Hide();
	else
		TradeFrameSilverSaleTipBtn:Show();
	end

end

function MarketTicketFrameTicketTradeBtn_OnUpdate()
	--��ҳ�����ϸ����
	local goldPriceFont = TradeTicketBuyPriceEditBox:GetText();
	MarketTicketFrameTicketTradeBtnTicketNumFont3:SetText(TradeGoldSaleNumChooseBtnFont:GetText());
	MarketTicketFrameTicketTradeBtnTicketPriceFont3:SetText( goldPriceFont );
	--��ҳ��������� 
	local goldPrice	= goldPriceFont == "" and 0 or tonumber( goldPriceFont );	
	local GoldFee = math.floor( ( goldPrice * 3 / 100 ) < 1 and 1 or goldPrice * 3 / 100 );
	if goldPriceFont == "" or goldPriceFont == "0" then
		GoldFee = 0;
	end
	MarketTicketFrameTicketTradeBtnTicketFeeFont2:SetText( GoldFee );

	--���ҳ�����ϸ����
	local silverNumFont = TradeSilverSaleNumChooseBtnFont:GetText();
	MarketTicketFrameTicketTradeBtnSilverNumFont3:SetText( silverNumFont );
	MarketTicketFrameTicketTradeBtnSilverPriceFont3:SetText(TradeSilverBuyPriceEditBox:GetText());
	local silverNum	= tonumber( silverNumFont );
	local SilverFee = math.floor( ( silverNum * 3 / 100 ) < 1 and 1 or silverNum * 3 / 100 );
	MarketTicketFrameTicketTradeBtnSilverFeeFont2:SetText( SilverFee );
	
	CheckEditNumberInput( TradeTicketBuyPriceEditBox );
	CheckEditNumberInput( TradeSilverBuyPriceEditBox );
	
	MarketTicketFrameTicketTradeBtn_OnShow();
end

function MarketTicketFrame_OnShow()
	this:SetPoint("center","UIClient","center",0,0);
	for i = 1,3 do
		t_tradeListInfo[i]["nCurPage"] = 1;
	end
	t_tradeListInfo[3]["nTotalPage"] = MarketManager:getCurTradeTotalPage( 3 );
	InitMarketTicketFrame();
	
	TradeType = 0;
	MarketTicketSaleBtn_OnClick();
	TradeTicketBuyPriceEditBox:SetText("0");
	if ExchangePointFrame:IsShown() then
		SetFocusFrame( "ExchangePointFrameNumEdit" );
	end
end
function InitMarketTicketFrame()
	if MarketManagerFrame:IsShown() then
		MarketManagerFrame:Hide();
	end
	--���׵���ʼ��
	t_tradeInfoSort[1]["SortCategery"]	= 2;
	t_tradeInfoSort[2]["SortCategery"]	= 2;
	t_tradeInfoSort[1]["SortFlag2"] 	= -1;
	t_tradeInfoSort[2]["SortFlag2"] 	= 1;
	--�Ƿ�����ѡ��
	local GoldChkBtn 	= getglobal("MarketTicketFrameTicketTradeBtnGoldChkBtn");
	local SilverChkBtn 	= getglobal("MarketTicketFrameTicketTradeBtnSilverChkBtn");
	GoldChkBtn:setCheckState( true );
	SilverChkBtn:setCheckState( true );
	--�������������ʾ
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	MarketTicketFrameMoneyInfoBackBtnMoneyValue:SetText(MainPlayer:getMoney());
	MarketTicketFrameMoneyInfoBackBtnTicketValue:SetText(MainPlayer:getGodCoin());
end
function MarketTicketFrame_OnHide()
	if MarketBuyTicketFrame:IsShown() or MarketSaleTicketFrame:IsShown() then 
		MarketBuyTicketFrame:Hide();
		MarketSaleTicketFrame:Hide();
	end 
	if  MessageBoxFrame:IsShown() 
			and (MessageTitle:GetText() == "��������"
				or MessageBoxFrame:GetClientString() == "��ҳ���ȷ��" 
				or MessageBoxFrame:GetClientString() ==  "���ҳ���ȷ��" ) then
		MessageBoxFrame:Hide();
	end
end
--����������ʾ ������ťͳһ���ɵ��
function ButtonClickForbidden()
	if MarketBuyTicketFrame:IsShown() or MarketSaleTicketFrame:IsShown() then 
		return true;
	end 
	if  MessageBoxFrame:IsShown() and MessageTitle:GetText() == "��������"then
		return true;
	end
	if MessageBoxFrame:IsShown() and ( MessageBoxFrame:GetClientString() == "��ҳ���ȷ��" or MessageBoxFrame:GetClientString() ==  "���ҳ���ȷ��" ) then
		return true;
	end
	return false;
end

function InitMarketTicketSaleInfo()
	t_tradeInfoSort[1]["SortNum"] = 0;
	t_tradeInfoSort[2]["SortNum"] = 0;
	SortRecordWithCategery();
end
function MarketTicketFrame_OnLoad()
	this:RegisterEvent("GE_UPDATE_TRADE_LIST");
	this:RegisterEvent("GE_UPDATE_TRADE_NAME");
end
function MarketTicketFrame_OnEvent()
	if arg1 == "GE_UPDATE_TRADE_LIST" then
		if ButtonClickForbidden() then return end
		if TradeType == 0 then return end
		for i = 1, 3 do
			SetTradeListPageInfo( i );
		end
		--������ҳ��۽ӵ��µ� ��������
		if TradeType == 1 or TradeType == 2 then
			if t_tradeInfoSort[TradeType]["SortCategery"] == 1 then
				MarketManager:sortTradeRecord( t_tradeInfoSort[TradeType]["SortType"] ,t_tradeInfoSort[TradeType]["SortFlag1"],TradeType );
			elseif t_tradeInfoSort[TradeType]["SortCategery"] == 2 then
				MarketManager:sortTradeRecord( t_tradeInfoSort[TradeType]["SortType"] ,t_tradeInfoSort[TradeType]["SortFlag2"],TradeType );
			end
		--	SortTradeList( t_tradeInfoSort[TradeType]["SortNum"] );
		end
		if MarketTicketFrameGoldListBtn:IsShown() then
			ShowGoldTradeListInfo();
		end	
	end
	if arg1 == "GE_UPDATE_TRADE_NAME" then 
		if MarketTicketFrameGoldListBtn:IsShown() then
			local TradeNum = t_tradeInfoSort[TradeType]["SortNum"];
			MarketManager:setTradeRecordByNum( TradeType, TradeNum );
			ShowGoldTradeListInfo();
		end
	end
end

function SetTradeListPageInfo( nType )
	if nType < 1 or nType > 3 then return ;end
	if nType < 3 then
		MarketManager:setTradeRecordByNum( nType, t_tradeInfoSort[nType]["SortNum"] );
	end
	t_tradeListInfo[nType]["nTotalPage"] = MarketManager:getCurTradeTotalPage( nType );
	local nCurPage = t_tradeListInfo[nType]["nCurPage"];
	t_tradeListInfo[nType]["nCurPage"] = nCurPage > t_tradeListInfo[nType]["nTotalPage"] 
											and t_tradeListInfo[nType]["nTotalPage"] 
											or nCurPage;
end

--[��ҽ��ײ���] ���������ť
function TradeGoldScrollDownBtnOnClick()
	if TicketFrameGoldNumSelectBtn:IsShown() then
		TicketFrameGoldNumSelectBtn:Hide();
	else
		TicketFrameGoldNumSelectBtn:Show();
	end
end
function TicketFrameGoldNumSelectBtn_OnShow()
	TicketFrameGoldNumSelectBtn1Font:SetText("10");
	TicketFrameGoldNumSelectBtn2Font:SetText("50");
	TicketFrameGoldNumSelectBtn3Font:SetText("100");
	TicketFrameGoldNumSelectBtn4Font:SetText("500");
	TicketFrameGoldNumSelectBtn5Font:SetText("1000");
end
function TicketFrameGoldNumSelectBtn_OnClick()
	local numFont = getglobal(this:GetName().."Font");
	TradeGoldSaleNumChooseBtnFont:SetText( numFont:GetText() );
	TicketFrameGoldNumSelectBtn:Hide();
end
--[��ҽ��ײ���] ����������ť
function TradeSilverScrollDownBtnOnClick()
	if TicketFrameSilverNumSelectBtn:IsShown() then
		TicketFrameSilverNumSelectBtn:Hide();
	else
		TicketFrameSilverNumSelectBtn:Show();
	end
end
function TicketFrameSilverNumSelectBtn_OnShow()
	TicketFrameSilverNumSelectBtn1Font:SetText("1000");
	TicketFrameSilverNumSelectBtn2Font:SetText("5000");
	TicketFrameSilverNumSelectBtn3Font:SetText("10000");
	TicketFrameSilverNumSelectBtn4Font:SetText("50000");
	TicketFrameSilverNumSelectBtn5Font:SetText("100000");
end
function TicketFrameSilverNumSelectBtn_OnClick()
	local numFont = getglobal(this:GetName().."Font");
	TradeSilverSaleNumChooseBtnFont:SetText( numFont:GetText() );
	TicketFrameSilverNumSelectBtn:Hide();
end
function TicketFrame_GoldSFeeBtn_OnEnter()
	SetGameTooltips(this:GetName(), "�����ѣ�3%" );
end
function MarketTradeBtnGoldNonymousBtn_OnClick()
	local chkBtn = getglobal("MarketTicketFrameTicketTradeBtnGoldChkBtn");
	chkBtn:setCheckState( not chkBtn:GetCheckState() );
end
function MarketTradeBtnSilverNonymousBtn_OnClick()
	local chkBtn = getglobal("MarketTicketFrameTicketTradeBtnSilverChkBtn");
	chkBtn:setCheckState( not chkBtn:GetCheckState() );
end
--ȷ�����۽��
function MarketTradeBtnSellGoldConfirmBtn_OnClick()
	if ButtonClickForbidden() then	return  end 
	local goldNumFont	= getglobal("MarketTicketFrameTicketTradeBtnTicketFeeFont2")
	local GoldFee		= tonumber( goldNumFont:GetText() );
	local goldNum		= tonumber( TradeGoldSaleNumChooseBtnFont:GetText() );
	local MainPlayer 	= ActorMgr:getMainPlayer();
	if MainPlayer:getMoney() < GoldFee then
		ShowMidTips("�������Ҳ�����֧�����۵�������");
		ShowBuyMoneyFrame();
		if MessageBoxFrame:IsShown() and  MessageBoxFrame:GetClientString() == "��ҳ���ȷ��"  then
			MessageBoxFrame:Hide();
		end
		return;
	end
	if MainPlayer:getGodCoin() < goldNum then
		ExchangePointFrame:SetClientUserData(0,1);
		MarketManager:requestQueryTianyuan();
		if MessageBoxFrame:IsShown() and  MessageBoxFrame:GetClientString() == "��ҳ���ȷ��"  then
			MessageBoxFrame:Hide();
		end
		return;
	end
	if TradeTicketBuyPriceEditBox:GetText() == "0" or TradeTicketBuyPriceEditBox:GetText() == "" then
		ShowMidTips("�������ҳ��ۼ۸�");
		if MessageBoxFrame:IsShown() and  MessageBoxFrame:GetClientString() == "��ҳ���ȷ��"  then
			MessageBoxFrame:Hide();
		end
		return;
	end
	--MessageBox("��ʾ","��ȷ����Ҫ�����ý�ҽ��׵���");
	MessageBox("��ʾ","�����ύ�ɹ��������Ѳ��˻�����ȷ����Ҫ���ۣ�");
	MessageBoxFrame:SetClientString( "��ҳ���ȷ��" );
end
function MarketTradeSellGoldOrderSummit()
	local goldNumFont	= getglobal("MarketTicketFrameTicketTradeBtnTicketFeeFont2")
	local GoldFee		= tonumber( goldNumFont:GetText() );
	local MainPlayer 	= ActorMgr:getMainPlayer();
	if MainPlayer:getMoney() < GoldFee then
		ShowMidTips("�������Ҳ��㣬�޷�����")
		ShowBuyMoneyFrame();
		return;
	end
	if TradeTicketBuyPriceEditBox:GetText() == "0" or TradeTicketBuyPriceEditBox:GetText() == "" then
		ShowMidTips("�������ҳ��ۼ۸�");
		return;
	end
	local nMoney	= tonumber( TradeTicketBuyPriceEditBox:GetText() );
	local nGold		= tonumber( TradeGoldSaleNumChooseBtnFont:GetText() );
	local chkBtn 	= getglobal("MarketTicketFrameTicketTradeBtnGoldChkBtn");
	if MainPlayer:getGodCoin() < nGold then
		ExchangePointFrame:SetClientUserData(0,1);
		MarketManager:requestQueryTianyuan();
		return;
	end
	if chkBtn:GetCheckState() then
		MarketManager:requestSendTradeRecord( 0, nMoney, nGold, 24, 0);
	else
		MarketManager:requestSendTradeRecord( 0, nMoney, nGold, 24, 1);
	end
end
--ȷ����������
function MarketTradeBtnSellSilverConfirmBtn_OnClick()
	if ButtonClickForbidden() then	return  end 
	local silverFeeFont	= getglobal("MarketTicketFrameTicketTradeBtnSilverFeeFont2")
	local silverFee		= tonumber( silverFeeFont:GetText() );
	local nMoney		= tonumber( TradeSilverSaleNumChooseBtnFont:GetText() );
	local MainPlayer	= ActorMgr:getMainPlayer();
	if MainPlayer:getMoney() < ( silverFee + nMoney ) then
		ShowMidTips("�������Ҳ�����֧�����۵�������")
		ShowBuyMoneyFrame();
		if MessageBoxFrame:IsShown() and  MessageBoxFrame:GetClientString() == "���ҳ���ȷ��"  then
			MessageBoxFrame:Hide();
		end
		return;
	end
	if TradeSilverBuyPriceEditBox:GetText() == "0" or TradeSilverBuyPriceEditBox:GetText() == "" then
		ShowMidTips("���������ҳ��ۼ۸�");
		if MessageBoxFrame:IsShown() and  MessageBoxFrame:GetClientString() == "���ҳ���ȷ��"  then
			MessageBoxFrame:Hide();
		end
		return;
	end
	MessageBox("��ʾ","�����ύ�ɹ��������Ѳ��˻�����ȷ����Ҫ���ۣ�");
	MessageBoxFrame:SetClientString( "���ҳ���ȷ��" );
end
function MarketTradeSellSilverOrderSummit()
	local silverFeeFont	= getglobal("MarketTicketFrameTicketTradeBtnSilverFeeFont2")
	local silverFee		= tonumber( silverFeeFont:GetText() );
	local nMoney		= tonumber( TradeSilverSaleNumChooseBtnFont:GetText() );
	local MainPlayer	= ActorMgr:getMainPlayer();
	if MainPlayer:getMoney() < ( silverFee + nMoney ) then
		ShowMidTips("�������Ҳ��㣬�޷�����");
		ShowBuyMoneyFrame();
		return;
	end
	if TradeSilverBuyPriceEditBox:GetText() == "0" or TradeSilverBuyPriceEditBox:GetText() == "" then
		ShowMidTips("���������ҳ��ۼ۸�");
		return;
	end
	local nGold		= tonumber( TradeSilverBuyPriceEditBox:GetText() );
	local chkBtn 	= getglobal("MarketTicketFrameTicketTradeBtnSilverChkBtn");
	if chkBtn:GetCheckState() then
		MarketManager:requestSendTradeRecord( 1, nMoney, nGold, 24, 0);
	else
		MarketManager:requestSendTradeRecord( 1, nMoney, nGold, 24, 1);
	end
end

--elmer �������û�õ�

function TicketBuyTimeSelectBtn_OnShow()
	for i = 1,5 do 
		local btn = getglobal("TicketFrameBuyTimeSelectBtn"..i);
		local Font = getglobal("TicketFrameBuyTimeSelectBtn"..i.."LayerName");
		btn:Show();
		Font:Show();
	end
end

function TicketBuyTimeSelectBtn_OnHide()
	for i = 1,5 do 
		local btn = getglobal("TicketFrameBuyTimeSelectBtn"..i);
		local Font = getglobal("TicketFrameBuyTimeSelectBtn"..i.."LayerName");
		btn:Hide();
		Font:Hide();
	end
end

--�������ʱ�������˵�
function TicketBuyTimeSelectBtn_OnClick()
	ShowBuyTimeSelectOpt(false);
	local nIndex = this:GetClientID();
	MarketTicketFrameBuyOrderListBackBtnOrderTimeMoneyValue:SetText(tTime[nIndex]);
end

function TicketSaleTimeSelectBtn_OnShow()
	for i = 1,5 do 
		local btn = getglobal("TicketFrameSaleTimeSelectBtn"..i);
		local Font = getglobal("TicketFrameSaleTimeSelectBtn"..i.."LayerName");
		btn:Show();
		Font:Show();
	end
end

function TicketSaleTimeSelectBtn_OnHide()
	for i = 1,5 do 
		local btn = getglobal("TicketFrameSaleTimeSelectBtn"..i);
		local Font = getglobal("TicketFrameSaleTimeSelectBtn"..i.."LayerName");
		btn:Hide();
		Font:Hide();
	end
end

--��ʾ�����س���ʱ�䰴ť
function ShowSaleTimeSelectOpt(bShow)
	if bShow then
		TicketFrameSaleTimeSelectBtn:Show();
	else
		TicketFrameSaleTimeSelectBtn:Hide();
	end
end

--�������ʱ�䰴ť
function TicketSaleTimeBtn_OnClick()
	ShowSaleTimeSelectOpt(not TicketFrameSaleTimeSelectBtn1:IsShown() );
end

--�������ʱ�������˵�
function TicketSaleTimeSelectBtn_OnClick()
	ShowSaleTimeSelectOpt(false);
	local nIndex = this:GetClientID();
	MarketTicketFrameSaleOrderListBackBtnOrderTimeMoneyValue:SetText(tTime[nIndex]);
end

function MoneyTex_OnEnter()
	local szTipsDes = "";
	if this:GetClientUserData(0) == 1 then
		szTipsDes = MONEY_TYPE_GODCOIN_NAME.."";
	elseif this:GetClientUserData(0) == 2 then 
		szTipsDes = MONEY_TYPE_BIND_GODCOIN_NAME.."";
	elseif this:GetClientUserData(0) == 3 then 
		szTipsDes = MONEY_TYPE_MONEY_NAME.."";
	elseif this:GetClientUserData(0) == 4 then
		szTipsDes = MONEY_TYPE_BULL_NAME.."";
	end

	if szTipsDes == "" then
		return;
	end
	
	SetGameTooltips(this:GetName(), szTipsDes );
end

function MarketManagerFrame_CumulateBtn_OnClick()
	ShowMidTips("�ù�����ʱδ����");
end

--------------------------------------�һ���ȯ-------------------------------------------------

function ExchangePointFrame_OnLoad()
	this:RegisterEvent("UI_MARKET_TIANYUAN_RESPONSE");
	this:RegisterEvent("UI_ACTOR_ATTRIBUTE_CHG");
	this:setUpdateTime( 0 );
end

function ExchangePointFrame_OnEvent()
	if arg1 == "UI_MARKET_TIANYUAN_RESPONSE" then
		--ExchangePointFrame:Show();
		if ExchangePointFrame:GetClientUserData(0) == 1 then
			SwitchGoldFrame();
		elseif ExchangePointFrame:GetClientUserData(0) == 2 then
			local num = ExchangePointFrame:GetClientUserData(1);
			if num > 0 then
				if MarketManager:getCurDianquan() > 0 then
					MarketManager:requestExchangePoint( num );
				else
					MessageBoxForVip( "��ʾ", "������ȡ�Ľ�Ҳ��㣬�Ƿ���ٳ�ֵ" )
					MessageBoxFrameForVipAcceptButton:SetText("���ٳ�ֵ");
				end
			end
			ExchangePointFrame:SetClientUserData(1,0);
			MarketManagerFrameMoneyFont3:SetText("����ȡ��ң�"..MarketManager:getCurDianquan());
		else
			ExchangePointFrame_OnShow();
		end
	end
	if arg1 == "UI_ACTOR_ATTRIBUTE_CHG" then
		if ExchangePointFrame:IsShown() then
			local mainplayer = ActorMgr:getMainPlayer();
			local szNum = ExchangePointFrameNumEdit:GetText();
			if szNum == "" then
				ExchangePointFrameAfterGodCoinFont:SetText( mainplayer:getGodCoin() );
			else
				ExchangePointFrameAfterGodCoinFont:SetText( tonumber( szNum ) );
			end
		end
	end
end

function ExchangePointFrame_OnShow()
	this:SetPoint("center","UIClient","center",0,0);
	local curDianQuan = MarketManager:getCurDianquan();
	ExchangePointFrameBeforePointFont:SetText( curDianQuan );

	local szText 	= ExchangePointFrameNumEdit:GetText();
	local nNum 		= tonumber( szText ~= "" and szText or "0" );	

	ExchangePointFrameAfterGodCoinFont:SetText( nNum );
	ExchangePointFrameAfterPointFont:SetText( curDianQuan - nNum );
	MarketManagerFrameMoneyFont3:SetText("����ȡ��ң�"..curDianQuan);
	if this:IsShown() then
		SetFocusFrame( "ExchangePointFrameNumEdit" );
	end
end

function ExchangePointFrameNumEdit_OnUpdate()
	CheckEditNumberInput( ExchangePointFrameNumEdit );
	local szText = ExchangePointFrameNumEdit:GetText();	
	local mainplayer = ActorMgr:getMainPlayer();
	if szText == "" then
		ExchangePointFrameNumEdit:SetText( 0 );
		ExchangePointFrameBeforePointFont:SetText( MarketManager:getCurDianquan() );
		ExchangePointFrameAfterGodCoinFont:SetText( 0 );
		ExchangePointFrameAfterPointFont:SetText( MarketManager:getCurDianquan() );
		return;
	end	
	local nNum = tonumber( szText );
	if nNum > MarketManager:getCurDianquan() then
		ExchangePointFrameNumEdit:SetText( MarketManager:getCurDianquan() );
		nNum = MarketManager:getCurDianquan();
	end
	if nNum < 0 then
		ExchangePointFrameNumEdit:SetText( 0 );
		nNum = 0;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	ExchangePointFrameAfterGodCoinFont:SetText( nNum );
	ExchangePointFrameAfterPointFont:SetText( MarketManager:getCurDianquan() - nNum );
end

function ExchangePointFrameOkBtn_OnClick()
	local szNum = ExchangePointFrameNumEdit:GetText();
	local nNum = tonumber( szNum );
	if nNum ~= "" and nNum > 0 then
		ExchangePointFrame:SetClientUserData(0,2);
		ExchangePointFrame:SetClientUserData(1,nNum);
		MarketManager:requestQueryTianyuan();
		util.closeParent(this);
	else
		ShowMidTips("��������ȷ�Ķһ�����");
	end
end

function ExchangePointFrameAllBtn_OnClick()
	local szNum	= ExchangePointFrameBeforePointFont:GetText();
	local nNum	= tonumber( szNum );
	if nNum > 0 then
		ExchangePointFrame:SetClientUserData(0,2);
		ExchangePointFrame:SetClientUserData(1,nNum);
		MarketManager:requestQueryTianyuan();
		util.closeParent(this);
	else
		MessageBoxForVip( "��ʾ", "������ȡ�Ľ�Ҳ��㣬�Ƿ���ٳ�ֵ" )
		MessageBoxFrameForVipAcceptButton:SetText("���ٳ�ֵ");
		util.closeParent(this);
	end
end


----------------------------------------���ٹ�����Ʒ--------------------------------------
local t_RecastItem = { 5200030, 5200031, 5200032, 5200033 };
local t_WashItem = { 5200040, 5200041, 5200042, 5200043 };
local RIDE_PROCE_ITEM_ID = 5234000;

function FastBuyItem_OnClick()
	local name = this:GetName();
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	
	local shopId = 0;
	if string.find( name, "ArmProceStarFrame", 1, true ) then
		shopId = MarketManager:getShopIDByID(BUY_TYPE_TICKET, ARM_STAR_ITEM_TOLUA);
	elseif string.find( name, "ArmRecastFrame", 1, true ) then
		local parent = getglobal( this:GetParent() );
		local itemId = parent:GetClientUserData( 2 );
		if itemId > 0 then
			shopId = MarketManager:getShopIDByID(BUY_TYPE_TICKET, itemId);
		end
		
		if shopId == 0 then
			local armListType = ArmRecastFrameArmItemBtn:GetClientUserData( 0 );
			local armGridIndex = ArmRecastFrameArmItemBtn:GetClientUserData( 1 );
			local container = mainplayer:getContainer();
			local item = container:getItem( armListType, armGridIndex );
			if item:getItemId() > 0 then
				local lv = item:getItemDef().WearPre.RoleLevelMin;
				for i=1, #t_RecastItem do
					local itemDef = getItemDef( t_RecastItem[i] );
					local val = itemDef.Result[0].ResultVal1;
					if val >= lv then
						shopId = MarketManager:getShopIDByID(BUY_TYPE_TICKET, itemDef.ItemID);
						if shopId > 0 then
							break;
						end
					end
				end
			else
				if not MarketManagerFrame:IsShown() then
					MarketManagerFrame:Show();
				end
				MarketShowTargetPage( 1, 2, BUY_TYPE_TICKET );
				return;
			end
		end

		if shopId == 0 then
			if not MarketManagerFrame:IsShown() then
				MarketManagerFrame:Show();
			end
			MarketShowTargetPage( 1, 2, BUY_TYPE_TICKET );
			return;
		end
	elseif string.find( name, "WashFrame", 1, true ) then
		local parent = getglobal( this:GetParent() );
		local itemId = parent:GetClientUserData( 2 );
		if itemId > 0 then
			shopId = MarketManager:getShopIDByID(BUY_TYPE_TICKET, itemId);
		end

		if shopId == 0 then
			local btn = getglobal( parent:GetParent() .. "ArmItemBtn" );
			local armListType = btn:GetClientUserData( 0 );
			local armGridIndex = btn:GetClientUserData( 1 );
			local container = mainplayer:getContainer();
			local item = container:getItem( armListType, armGridIndex );
			if item:getItemId() > 0 then
				local lv = item:getItemDef().WearPre.RoleLevelMin;
				for i=1, #t_WashItem do
					local itemDef = getItemDef( t_WashItem[i] );
					local val = itemDef.Result[0].ResultVal1;
					if val >= lv then
						shopId = MarketManager:getShopIDByID(BUY_TYPE_TICKET, itemDef.ItemID);
						if shopId > 0 then
							break;
						end
					end
				end
			else
				if not MarketManagerFrame:IsShown() then
					MarketManagerFrame:Show();
				end
				MarketShowTargetPage( 1, 2, BUY_TYPE_TICKET );
				return;
			end
		end

		if shopId == 0 then
			if not MarketManagerFrame:IsShown() then
				MarketManagerFrame:Show();
			end
			MarketShowTargetPage( 1, 2, BUY_TYPE_TICKET );
			return;
		end
	elseif string.find( name, "DevelopmentBuildUp", 1, true ) then
		local itemId = GetPetStrongUpItem();
		shopId = MarketManager:getShopIDByID(BUY_TYPE_TICKET, itemId);
	elseif string.find( name, "PetGrowResetFrame", 1, true ) then
		shopId = MarketManager:getShopIDByID(BUY_TYPE_TICKET, PET_GROW_RESET_ITEM);
	elseif string.find( name, "PetSkillLevelUpFrame", 1, true ) then
		shopId = MarketManager:getShopIDByID(BUY_TYPE_TICKET, PET_SKILL_LEVELUP_ITEM);
	elseif string.find( name, "PetSkillResetFrame", 1, true ) then
		shopId = MarketManager:getShopIDByID(BUY_TYPE_TICKET, PET_SKILL_RESET_ITEM);
	elseif string.find( name, "GemProcePickFrame", 1, true ) then
		shopId = MarketManager:getShopIDByID(BUY_TYPE_TICKET, ARM_GEM_PICK_ITEM_TOLUA);
	elseif string.find( name, "RideLevelUpFrame", 1, true ) then
		shopId = MarketManager:getShopIDByID(BUY_TYPE_TICKET, RIDE_PROCE_ITEM_ID);
	elseif string.find( name, "Pet_UnBindFrame", 1, true ) then
		shopId = MarketManager:getShopIDByID(BUY_TYPE_TICKET, PET_UNBIND_ITEM_ID_TOLUA )
	end

	if shopId > 0 then
		MarketBuyItemFrameBtn:SetClientUserData( 3, BUY_TYPE_TICKET );
		MarketManagerFrame_ShowBuyFrame(shopId);
		UpdateMarketBuyItemFrameMoneySymbol( BUY_TYPE_TICKET );
		FashionPreviewBuyFrame:Hide();
	else
		ShowMidTips("�̳���û�ж�Ӧ����Ʒ");
	end
end

function GetShopIDByArm( armId, tableName )
	local shopId = 0;
	if armId <= 0 then
		return shopId;
	end
	
	local lv = getItemDef(armId).WearPre.RoleLevelMin;
	local tItem = {};
	if tableName == "recast" then
		tItem = t_RecastItem;
	elseif tableName == "wash" then
		tItem = t_WashItem;
	end
	for i=1, #tItem do
		local itemDef = getItemDef( tItem[i] );
		local val = itemDef.Result[0].ResultVal1;
		if val >= lv then
			shopId = MarketManager:getShopIDByID(BUY_TYPE_TICKET, itemDef.ItemID);
			if shopId > 0 then
				break;
			end
		end
	end
	return shopId;
end