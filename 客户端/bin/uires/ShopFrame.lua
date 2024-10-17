local nCurPageNumber = 1;
local nMaxPageNumber = 5;
local nPageDisNumber = 12;
local nMaxSaveItemNum = 5;
local nCurIndex = 0; --当前操作的物品的所在格子数

local nRepairState;
local nRepairOne	= 10;
local nRepairAll	= 20;
local nRepairEquip	= 30;

local MAX_NUM_TO_BUY = 99;

local EXCHANGEITEMTYPE = 1001;

local t_BuyUseIcon = {
	[ATTR_ID_BATTLE_GLORY] = "uires\\TuPianLei\\WuPin\\5106004.tga",
	[ATTR_ID_AOSHAN_BRAND] = "uires\\TuPianLei\\WuPin\\5052812.tga",
	[ATTR_ID_LIFT_BRAND] = "uires\\TuPianLei\\WuPin\\5052813.tga",
	[ATTR_ID_SCHOOL_VAL] = "uires\\TuPianLei\\WuPin\\5106002.tga",
	[ATTR_ID_ZHENGYI] = "uires\\TuPianLei\\WuPin\\5106001.tga"
}

-- 如果是在商店物品右键点击购买物品
function ShopRightBtnClick( szBtnName )
	-- 若此为右键点击回购按钮，则什么操作也不做
	if string.find( szBtnName, "RecycleItemBtn" ) ~= nil then
		return;
	end
	
	BuyShopItem( { btn = this:GetName() } );
end

function ClearAllShopFrameItem()
	for i = 1, nPageDisNumber do
		local frame			= getglobal("ShopItemFrame"..i);
		local priceIconBtn	= getglobal( "ShopItemFrame" .. i .. "ShopItemPriceIconBtn" );
		local priceIconBtn1 = getglobal( "ShopItemFrame" .. i .. "ShopItemPriceIconBtn1" );
		priceIconBtn:Hide();
		priceIconBtn:SetClientUserData( 0, 0 );
		priceIconBtn1:Hide();
		priceIconBtn1:SetClientUserData( 0, 0 );
		local itemName = getglobal("ShopItemFrame"..i .. "ShopItemName_Font" );
		itemName:SetText( "" );
		local itemPrice = getglobal("ShopItemFrame" .. i .. "ShopItemPrice_Font" );
		itemPrice:SetText( "" );
		local itemBtn = getglobal( "ShopItemFrame" .. i .. "ShopItemBtn" );
		itemBtn:SetClientUserData( 0, 0 );
		itemBtn:SetClientUserData( 1, 0 );
		itemBtn:SetClientUserData( 2, 0 );
		itemBtn:SetClientUserData( 3, 0 );
		local itemPrice1 = getglobal("ShopItemFrame" .. i .. "ShopItemPrice_Font1")
		itemPrice1:SetText("")
		itemBtn:Hide();
		local tex = getglobal( "ShopItemFrame" .. i .. "ShopItemBtn".."IconTexture" ); --物品Icon
		tex:Hide();
		local itemBuyBtn = getglobal( "ShopItemFrame"..i.."ShopItemBuyBtn" ); --物品Icon
		itemBuyBtn:Hide();
		local boxTex	= getglobal( "ShopItemFrame"..i.."ShopItemBtnBoxTexture" );
		boxTex:Hide();

		local IconTex	= getglobal( "ShopItemFrame" .. i .. "ShopItemBtnIconTexture" );
		IconTex:SetGray(false);
	end
end

function ReSetShopSaveItem()
	for i = 1, nMaxSaveItemNum do
		local btn = getglobal("RecycleItemBtn"..i);
		local tex = getglobal("RecycleItemBtn"..i.."IconTexture");
		btn:SetClientUserData( 0, 0 );
		btn:SetClientUserData( 1, 0 );
		btn:SetClientUserData( 2, 0 );
		tex:Hide();
		local boxTex = getglobal("RecycleItemBtn"..i.."BoxTexture");
		boxTex:Hide();
		local num = getglobal("RecycleItemBtn"..i.."CountFont");
		num:SetText( "" );
	end
end

-- 购买某物品是否满足声望要求
function CanSatisfyDesignation( itemDef )
	assert( itemDef ~= nil );
	if itemDef == nil then
		return false;
	end
	
	if itemDef.NeedRespact.ID == 0 then
		return true;
	end
	
	return GameRespact:getRespactValue( itemDef.NeedRespact.ID ) >= itemDef.NeedRespact.Value;
end

function CanSatisfyTech(itemid)
	local clanAdv = GameClanManager:getAdvanceInfo()
	local tech = NpcShop:getRequireTech(itemid)
	for i = 0, tech.num - 1 do
		local id, level = tech.id[i], tech.level[i]
		if clanAdv.Tech.TechLevel[id] < level then
			return false
		end
	end
	return true
end

function UpdateShopFrameItem()
	ClearAllShopFrameItem();
	local ShopList		= NpcShop:getShopList();
	local nShopItemNum	= ShopList.m_nShopItemNum;	--出售物品的数量
	local mainplayer 	= ActorMgr:getMainPlayer();

	--计算最大table页面
	nMaxPageNumber = math.ceil( nShopItemNum / nPageDisNumber );

	local nBeginIndex = ( nCurPageNumber - 1 ) * nPageDisNumber;
	for i = 1, nPageDisNumber do
		if i + nBeginIndex > nShopItemNum then
			break;
		end

		local ShopSellItem	= ShopList.m_ShopSellItems[nBeginIndex+i-1];
		local nId			= ShopSellItem.GoodsID;

		local btn = getglobal( "ShopItemFrame" .. i .. "ShopItemBtn" );   -- 物品btn
		btn:SetClientUserData( 0, DRAG_SHOP );
		btn:SetClientUserData( 1, nId );
		btn:SetClientUserData( 2, ShopSellItem.GoodsType );
		btn:SetClientUserData( 3, ShopSellItem.SpecialMoneyType );
		if ShopSellItem.GoodsType == SHOP_ITEM_ITEM_CLIENT then
			local itemDef		= getItemDef( nId );
			local nMoneyType	= itemDef.MoneyType;

			--设置商店物品显示信息
			local tex		= getglobal( "ShopItemFrame" .. i .. "ShopItemBtnIconTexture" ); --物品Icon
			local uvTex		= getglobal( "ShopItemFrame" .. i .. "ShopItemBtnUVAnimationTex");

			local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
			-- 如果没找到物品的图标则使用默认图标显示
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			tex:SetTexture( IconPath );
			SetEquipImportantEffectUV( itemDef, uvTex:GetName() );
			tex:Show();

			--local lockTex	= getglobal( "ShopItemFrame" .. i .. "ShopItemBtnLockTexture" );
			-- ( getItemType( nId ) == ITEM_SLOTTYPE_AMM and 
			if itemDef.WearPre.Career ~= 0 and not IsFitRace( { actor = mainplayer, compareRace = itemDef.WearPre.Career } ) 
					or not CanSatisfyDesignation( itemDef ) or not CanSatisfyTech(itemDef.ItemID) then
				-- 若主角的职业与物品的职业不符合在物品的上面盖红色
				tex:SetGray(true);
			end

			local nameFont  = getglobal( "ShopItemFrame" .. i .. "ShopItemName_Font" );
			nameFont:SetText( itemDef.Name );

			local priceFont = getglobal( "ShopItemFrame" .. i .. "ShopItemPrice_Font" );
			local priceVal	= 0;
			if ShopSellItem.SpecialMoneyType > 0 then	
				if ShopSellItem.SpecialMoneyType == ATTR_ID_MONEY or ShopSellItem.SpecialMoneyType == ATTR_ID_BULL then
					priceVal = math.floor( ShopSellItem.Price * ShopList.RebateCount / 100 );
				else 
					priceVal = ShopSellItem.Price;
				end
				SetPriceTextColor( priceFont:GetName(), ShopSellItem.SpecialMoneyType );
			else
				if itemDef.ClanContri > 0 then
					priceVal = itemDef.ClanContri;
					priceFont:SetTextColor( 232, 226, 72 );
				elseif itemDef.ExchgItemID > 0 then
					priceVal = ShopSellItem.Price;
					priceFont:SetTextColor( 232, 226, 72 );
				elseif nMoneyType == ATTR_ID_MONEY or nMoneyType == ATTR_ID_BULL then
					priceVal = math.floor( ShopSellItem.Price * ShopList.RebateCount / 100 );
					SetPriceTextColor( priceFont:GetName(),nMoneyType);
				else
					priceVal = ShopSellItem.Price;
					priceFont:SetTextColor( 232, 226, 72 );
				end
			end
			priceFont:SetText( priceVal );
			local priceIconBtn	= getglobal( "ShopItemFrame" .. i .. "ShopItemPriceIconBtn" );
			local priceTex		= getglobal( "ShopItemFrame" .. i.. "ShopItemPriceIconBtnTex" );
			local priceIconBtn1 = getglobal( "ShopItemFrame" .. i .. "ShopItemPriceIconBtn1" );
			local priceTex1		= getglobal( "ShopItemFrame" .. i.. "ShopItemPriceIconBtn1Tex" );
			local priceFont1	= getglobal( "ShopItemFrame" .. i .. "ShopItemPrice_Font1" );
			-- 显示购买按钮
			local itemBuyBtn = getglobal("ShopItemFrame"..i.."ShopItemBuyBtn" ); --物品Icon
			itemBuyBtn:Show();
			local boxTex	= getglobal( "ShopItemFrame"..i.."ShopItemBtnBoxTexture" );
			boxTex:Show();
			-- 设置交易货币的图标
			local isMoney = false;
			if ShopSellItem.SpecialMoneyType > 0 then
			
				UpdateShopItemSpecialMoneyType( ShopSellItem.SpecialMoneyType, priceTex, priceIconBtn );
				
				if ShopSellItem.SpecialMoneyType == ATTR_ID_MONEY or ShopSellItem.SpecialMoneyType == ATTR_ID_BULL then
					isMoney = true;
				end
			else
				if itemDef.ExchgItemID > 0 then
					local ExchangeItemDef = getItemDef(itemDef.ExchgItemID);
					priceTex:SetSize(17,17);
					local moneyTexPath	= util.GetExchangMoneyIconPath( { iconID = ExchangeItemDef.IconID } );
					local t_tgas = {	[priceTex:GetName()] = { path = moneyTexPath }, 
									};

					util.SetTgas( t_tgas );
					priceIconBtn:SetClientUserData( 0, itemDef.ExchgItemID );
				-- 说明是消耗公会贡献度的
				elseif itemDef.ClanContri > 0 then
					local szTexPath = "uires\\ZhuJieMian\\1.dds";
					priceTex:SetTexture( szTexPath );
					priceTex:SetTexUV( 577,570,14,18 );
					priceTex:SetSize( 14,18 );
					priceIconBtn:SetClientUserData( 0,-1 );
				elseif nMoneyType == ATTR_ID_SPARTA_GLORY then
					local szTexPath = "uires\\ZhuJieMian\\3.dds";
					priceTex:SetTexture( szTexPath );
					priceTex:SetTexUV( 580,129,16,16 );
					priceTex:SetSize(12,13);
					priceIconBtn:SetClientUserData( 0, ATTR_ID_SPARTA_GLORY );
				else
					local szTexPath = "uires\\ZhuJieMian\\1.dds";
					priceTex:SetTexture( szTexPath );
					if nMoneyType == ATTR_ID_MONEY then
						priceTex:SetTexUV( 117,494,17,18 );
						priceTex:SetSize(12,13);
						priceIconBtn:SetClientUserData( 0, ATTR_ID_MONEY );
					elseif nMoneyType == ATTR_ID_BULL then
						priceTex:SetTexUV(78,494,20,18);
						priceTex:SetSize(12,13);
						priceIconBtn:SetClientUserData( 0, ATTR_ID_BULL );
					end
					isMoney = true;
				end
			end
			
			local posX = math.ceil( (priceFont:GetTextExtentWidth( priceFont:GetText() ) + 3) / GetScreenScaleY() );
			priceIconBtn:SetPoint( "topleft", priceFont:GetName(), "topleft", posX, (isMoney and 2 or -2));
			priceIconBtn:Show();
			
			if ShopSellItem.SpecialMoneyType == 0 then
				local t_ItemData = {[1] = {priceTex,priceIconBtn,priceFont},
									[2] = {priceTex1,priceIconBtn1,priceFont1}
									}					
				UpdateShopItemNonSpecialMoneyType( t_ItemData , itemDef, ShopSellItem.Price );     -- 战场荣誉、战魂徽章、正义点数等
			end
		elseif ShopSellItem.GoodsType == SHOP_ITEM_CLAN_ITEM_CLIENT then
			local clanProductDef = GameClanManager:getProductDefByID( nId );
			if clanProductDef ~= nil then
				local btn = getglobal( "ShopItemFrame" .. i .. "ShopItemBtn" );   -- 物品btn
				local tex		= getglobal( "ShopItemFrame" .. i .. "ShopItemBtnIconTexture" ); --物品Icon
				local nameFont  = getglobal( "ShopItemFrame" .. i .. "ShopItemName_Font" );
				local priceFont = getglobal( "ShopItemFrame" .. i .. "ShopItemPrice_Font" );
				local priceIconBtn	= getglobal( "ShopItemFrame" .. i .. "ShopItemPriceIconBtn" );
				local priceTex		= getglobal( "ShopItemFrame" .. i.. "ShopItemPriceIconBtnTex" );
				local boxTex	= getglobal( "ShopItemFrame"..i.."ShopItemBtnBoxTexture" );
				boxTex:Show();
				
				local itemBuyBtn = getglobal("ShopItemFrame"..i.."ShopItemBuyBtn" ); --物品Icon
				if clanProductDef.Category == 0 then	--道具
					local itemDef = getItemDef( clanProductDef.ID );
					local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";

					if not IsInExistence( IconPath ) then
						IconPath = DEFAULT_ITEM_PATH;
					end
					tex:SetTexture( IconPath );
					nameFont:SetText( itemDef.Name );					
					if itemDef.WearPre.Career ~= 0 and mainplayer:getRace() ~= itemDef.WearPre.Career 
						or not CanSatisfyDesignation( itemDef ) or not CanSatisfyTech(itemDef.ItemID) then
						tex:SetGray(true);
					end
				elseif clanProductDef.Category == 1 then	--机械
					nameFont:SetText( getMonsterName( clanProductDef.ID ) );
					local iconID = GameBCT:getMonsterIcon( clanProductDef.ID );
					local IconPath = GetMonsterIconPath( iconID );
					tex:SetTexture( IconPath );
				end	
				priceFont:SetText( clanProductDef.BuyPrice );
				priceFont:SetTextColor( 232, 226, 72 );
				tex:Show();
				itemBuyBtn:Show();

				local mainplayer = ActorMgr:getMainPlayer();
				local pos = mainplayer:getClanPosition();
				if not (pos == CLAN_POS_VICE_PRESIDENT or pos == CLAN_POS_PRESIDENT) then
					tex:SetGray(true);
				end

				local szTexPath = "uires\\ZhuJieMian\\1.dds";
				if itemDef.MoneyType == ATTR_ID_MONEY then
					priceTex:SetTexUV( 117,494,17,18 );
					priceTex:SetSize(12,13);
					priceIconBtn:SetClientUserData( 0, ATTR_ID_MONEY );
					SetPriceTextColor( priceFont:GetName(),itemDef.MoneyType );
				elseif itemDef.MoneyType == ATTR_ID_BULL then
					priceTex:SetTexUV(78,494,20,18);
					priceTex:SetSize(12,13);
					priceIconBtn:SetClientUserData( 0, ATTR_ID_BULL );
					SetPriceTextColor( priceFont:GetName(),itemDef.MoneyType );
				end
				priceIconBtn:SetPoint( "topleft", priceFont:GetName(), "topleft", priceFont:GetTextExtentWidth( priceFont:GetText() ) + 3 * GetScreenScaleY(), 
										2 );
				priceIconBtn:Show();
			end
		end
		btn:Show();
	end
	UpdatePageNumber();
end

function UpdateShopItemSpecialMoneyType( SpecialMoneyType, priceTex, priceIconBtn )
	local szTexPath = "uires\\ZhuJieMian\\1.dds";
	if SpecialMoneyType == ATTR_ID_MONEY then
		priceTex:SetTexture( szTexPath );
		priceTex:SetTexUV( 117,494,17,18 );
		priceTex:SetSize(12,13);
		priceIconBtn:SetClientUserData( 0, ATTR_ID_MONEY );
	elseif SpecialMoneyType == ATTR_ID_BULL then
		priceTex:SetTexture( szTexPath );
		priceTex:SetTexUV(78,494,20,18);
		priceTex:SetSize(12,13);
		priceIconBtn:SetClientUserData( 0, ATTR_ID_BULL );
	else
		szTexPath = t_BuyUseIcon[SpecialMoneyType];
		if szTexPath == nil then 
			return;
		end
		priceTex:SetTexture( szTexPath );
		priceTex:SetTexRelUV( 1, 1 );
		priceTex:SetSize(13,13);
		priceIconBtn:SetClientUserData( 0, SpecialMoneyType );
	end
end
function UpdateShopItemNonSpecialMoneyType( t_ItemData , itemDef, nPrice )
	local nIndex = 1;
	for idx=1,MAX_OTHER_MONEY_USE_CLIENT do
		if itemDef.MoneyUses[idx-1].Val > 0 then
			if nIndex > 2 then break end
			local moneyTexPath	= t_BuyUseIcon[itemDef.MoneyUses[idx-1].ID];
			local t_tgas = {	[ t_ItemData[nIndex][1]:GetName()] = { path = moneyTexPath }, 
							};
			util.SetTgas( t_tgas );
			t_ItemData[nIndex][1]:SetTexRelUV( 1, 1 );
			t_ItemData[nIndex][1]:SetSize(13,13)
			t_ItemData[nIndex][2]:SetClientUserData( 0, itemDef.MoneyUses[idx-1].ID );
			t_ItemData[nIndex][3]:SetText( itemDef.MoneyUses[idx-1].Val );
			local posX = math.ceil( (t_ItemData[nIndex][3]:GetTextExtentWidth( t_ItemData[nIndex][3]:GetText() ) + 3) / GetScreenScaleY() );
			t_ItemData[nIndex][2]:SetPoint( "topleft", t_ItemData[nIndex][3]:GetName(), "topleft", posX, 0 );
			t_ItemData[nIndex][2]:Show();
			t_ItemData[nIndex][3]:SetTextColor( 232, 226, 72 );
			
			nIndex = nIndex + 1;
		end
	end
	--如果是学院徽章，需要交换下两个售价的位置，只要重新设置下ExchageItemID
	local t_SchoolItemList = { 5052601,5052602,5052603,5052604,5052605,5052606,5052607,5052608,5052609,5052610,5052611,5052612 }
	for i = 1, table.getn( t_SchoolItemList ) do
		if itemDef.ExchgItemID > 0 and itemDef.ItemID == t_SchoolItemList[i] then
			t_ItemData[2][3]:SetTextColor( 232, 226, 72 );
			t_ItemData[2][3]:SetText( nPrice );	
			local ExchangeItemDef = getItemDef(itemDef.ExchgItemID);
			t_ItemData[2][1]:SetSize(14,14);
			local moneyTexPath	= util.GetExchangMoneyIconPath( { iconID = ExchangeItemDef.IconID } );
			local t_tgas = {	[t_ItemData[2][1]:GetName()] = { path = moneyTexPath }, 
							};

			util.SetTgas( t_tgas );

			t_ItemData[2][2]:SetClientUserData( 0, itemDef.ExchgItemID );
			t_ItemData[2][2]:SetPoint( "topleft", t_ItemData[2][3]:GetName(), "topleft", t_ItemData[2][3]:GetTextExtentWidth( t_ItemData[2][3]:GetText() ) + 3 * GetScreenScaleY(), 0 );
			t_ItemData[2][2]:Show()
		end
	end
end

function UpdateShopSaveItem()
	ReSetShopSaveItem();
	local ShopList			= NpcShop:getShopList();
	local nShopSaveItemNum	= ShopList.m_nShopSaveItemNum;	-- 购回物品的种数
	for i = 1, nShopSaveItemNum do
		local shopSaveItem = ShopList.m_ShopSaveItems[i-1].m_SaveItem;
		local itemDef = shopSaveItem:getItemDef();
		local tex		= getglobal( "RecycleItemBtn" .. i .. "IconTexture" );
		local num		= getglobal( "RecycleItemBtn" .. i .. "CountFont" );

		-- 设置商品显示图标
		local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
		-- 如果没找到物品的图标则使用默认图标显示
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH;
		end
		tex:SetTexture( IconPath );

		tex:Show();

		if shopSaveItem:getNum() > 1 then
			num:SetText( shopSaveItem:getNum() );
		end

		local boxTex = getglobal("RecycleItemBtn"..i.."BoxTexture");
		boxTex:Show();

		local btn = getglobal("RecycleItemBtn"..i);
		btn:SetClientUserData( 0,DRAG_SAVEITEM );
		btn:SetClientUserData( 1,itemDef.ItemID );
		btn:SetClientUserData( 2, i );
	end
end

function ShowShopFrameXinShouInfo( data )
	local btn	= getglobal( data["btn"] );
	local rich	= getglobal( data["btn"].."Rich" );
	local animationTexture	= getglobal( data["btn"].."UVAnimationTex" );
	local nSelfDefineExtendWidth = 15;
	rich:SetClientString( data["text"] );
	ReSizeTooltip( rich:GetName(), btn:GetName(), nSelfDefineExtendWidth );
	animationTexture:SetUVAnimation( 50, true );
	btn:Show();
end

function ShopFrame_Test_OnShow()
	ShowShopFrameXinShouInfo( { text = "点击修理损坏的装备", btn = "ShopFrameRepairAllXinShouBtn" } );
end

function CheckShopFrameXinShouGuide()
	--[[
	if not GameWizard:isDo( GAMEWIZARD_FIRST_OPENSHOP ) and Quest:isQuestRun( 1053 ) then
		ShowShopFrameXinShouInfo( { text = "点击购买物品，可完成“紧急任务”", btn = "ShopFrameXinShouBtn" } );
		--GameWizard:doStep( GAMEWIZARD_FIRST_OPENSHOP);
	end
	]]--

	if not GameWizard:isDo( GAMEWIZARD_FIRST_REPAIR ) and Quest:isQuestRun( 1081 ) then
		ShowShopFrameXinShouInfo( { text = "点此修理全身装备，恢复耐久", btn = "ShopFrameRepairAllXinShouBtn" } );
	end

	if GameWizard:isDo( GAMEWIZARD_FIRST_EQUIP_NOT_ENOUGH_DUR ) and 
		not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_REPAIR_ALL_BTN ) then

		ShowShopFrameXinShouInfo( { text = "点击修理损坏的装备", btn = "ShopFrameRepairAllXinShouBtn" } );
	end
end

function ShopFrame_OnShow()
	local t_hideIcons = { "ShopFrameXinShouBtn", "ShopFrameRepairAllXinShouBtn" };
	util.HideIcons( t_hideIcons );
	
	if not PlayerContainerFrame:IsShown() then
		PlayerContainerFrame:Show();
	end
	this:SetPoint("topright", "$parent", "topright", -538, 163 );
	PlayerContainerFrame:SetPoint("topright", "$parent", "topright", -120, 163);

	for i = 1, nMaxSaveItemNum do
		local backTex	= getglobal( "RecycleItemBtn" .. i .. "BackTexture" );
		backTex:Show();
	end

	nCurPageNumber = 1;
	UpdateShopSaveItem();
	UpdateShopFrameItem();
	UpdateShopFrameRebate();

	-- 关闭新手指引特效显示
	CheckShopFrameXinShouGuide();
end

--AttributeFrame
function ShopFrame_OnLoad()
	this:RegisterEvent("GE_NPC_SHOPLIST");
	this:RegisterEvent("GE_NPC_SHOPBUYLIST");

	--注册商店面板优先响应的回调函数
	ItemFirstCallBack( "ShopFrame" , Sale2Shop );

	--for i = 1, nPageDisNumber do
		--local lockTex = getglobal( "ShopItemFrame" .. i .. "ShopItemBtnLockTexture");
		--lockTex:SetColor( 255, 68, 45 );
		--lockTex:SetBlendAlpha( 0.5 );
	--end
end

local t_confirmSaleItem=	{ 
								useTarget = { USE_TARGET_PET, },
								subType = { ITEM_PET_HELP, ITEM_PETSHIEGG_ID, ITEM_PETXIANEGG_ID },
								resultID = { RESULT_FAIRY_BORN, },
							};
function needConfirmSale( itemDef )
	for _, useTarget in ipairs( t_confirmSaleItem.useTarget ) do
		if itemDef.UseTarget == useTarget then
			return true;
		end
	end
	
	for _, subType in ipairs( t_confirmSaleItem.subType ) do
		if math.modf( itemDef.ItemID / 1000 ) == subType then
			return true;
		end
	end

	for _, resultID in ipairs( t_confirmSaleItem.resultID ) do
		for i = 1, MAX_ITEM_RESULT do
			if itemDef.Result[i-1].ResultID == resultID then
				return true;
			end
		end
	end
	
	-- 蓝装以上物品要售出
	if itemDef.Important >= 2 then
		return true;
	end

	return false;
end

-- 出售给商店
function Sale2Shop( used_Item, nType, nIndex )
	if GameExchange:isSrcLockExchange() or GameExchange:isInExchangeItem( SHITYPE, nIndex ) then
		return;
	end

	-- NoNpcSell值为1的时候，表示不能卖给NPC
	local itemdef	= used_Item:getItemDef();
	if itemdef.NoNpcSell == 1 then
		ShowMidTips("该物品不可出售！");
		return;
	end

	-- 坐骑不处理
	for i=1,MAX_ITEM_RESULT do
		if itemdef.Result[i-1].ResultID == RESULT_RIDE then
			return true
		end
	end

	if ImportantItemConfirmFrame:IsShown() and ImportantItemConfirmFrameTitle:GetText() == "道具出售确认" then
		return;
	end
	
	if used_Item:getNum() == 1 then
		if InputFrame:IsShown() then
			local t_callBackValue = GetInputFrameData();
			if t_callBackValue["szFrameName"] == "ShopFrame" then
				InputFrame:Hide();
			end
		end

		local idx = CheckItemNeedImportantConfirm( used_Item );
		if idx > 0 then
			ImportantItemConfirmFrameTitle:SetText("道具出售确认");
			ShowImportantItemConfirmFrame( "出售", used_Item, idx, true );
			SetItemWarnFrame("ShopFrame");
			ImportantItemConfirmFrame:SetClientUserData( 0, nType );
			ImportantItemConfirmFrame:SetClientUserData( 1, nIndex );
			ImportantItemConfirmFrame:SetClientUserData( 2, 1 );
			LockItem( used_Item:getListType(), used_Item:getGridIdx(), used_Item:getItemId(), "ImportantItemConfirmFrame" );
			return;
		else
			if used_Item:getItemType() == ITEM_SLOTTYPE_AMM and itemdef.Important > 2 then
				ImportantItemConfirmFrameTitle:SetText("道具出售确认");
				ShowImportantItemConfirmFrame( "出售", used_Item, idx, false );
				SetItemWarnFrame("ShopFrame");
				ImportantItemConfirmFrame:SetClientUserData( 0, nType );
				ImportantItemConfirmFrame:SetClientUserData( 1, nIndex );
				ImportantItemConfirmFrame:SetClientUserData( 2, 1 );
				LockItem( used_Item:getListType(), used_Item:getGridIdx(), used_Item:getItemId(), "ImportantItemConfirmFrame" );
				return;
			end
		end
		
		-- 若物品的品质高要弹出二次确认窗口
		if needConfirmSale( itemdef ) then
			SellToShopConfirmFrame:SetClientUserData( 0, nType );
			SellToShopConfirmFrame:SetClientUserData( 1, nIndex );
			SellToShopConfirmFrame:SetClientUserData( 2, 1 );
			SellToShopConfirmFrame:Show();
		else
			NpcShop:sellItem( nType, nIndex, 1 );
			playUISound( "sounds\\UI\\shoumai.wav" );
		end
		return;
	end

	-- 若被拖曳的物品数量大于1,则做拆分处理
	if used_Item:getNum() > 1 then
		SplitItem( {
					szFrameName = "ShopFrame", nMinNum = 0, nMaxNum = used_Item:getNum(),
					desGrid = 0, srcList = nType, srcGrid = nIndex
					} );
	end
end

--事件消息
function ShopFrame_OnEvent()
	if arg1 == "GE_NPC_SHOPLIST" then
		this:Show();
		local ShopList = NpcShop:getShopList();
		if ShopList.m_nNpcId > 0 then
			util.testNpcDialogDistance( ShopList.m_nNpcId,ShopFrame )
		end
	elseif arg1 == "GE_NPC_SHOPBUYLIST" then
		UpdateShopSaveItem();
	end
end

function ShopFrame_OnHide()
	NpcShop:resetConditions()
	if GetCurrentCursor() == "repair" then
		ChangeCursorState("normal");
	end
	
	local t_hideIcons = { "ShopFrameXinShouBtn", "ShopFrameRepairAllXinShouBtn" };
	util.HideIcons( t_hideIcons );

	local t_relativeChildFrame = { "ShopSellFrame", "ShopRepairFrame", "SellToShopConfirmFrame", "PlayerContainerFrame" };

	for i = 1, table.getn( t_relativeChildFrame ) do
		local frame = getglobal( t_relativeChildFrame[i] );
		if frame:IsShown() then
			frame:Hide();
		end
	end
end

--关闭商店界面按钮
function CFCloseBtn_OnClick()
	ShopFrame:Hide();
end

function ShopItemPriceIconBtn_OnEnter()
	local szText = "";
	if this:GetClientUserData( 0 ) == ATTR_ID_MONEY then
		--szText = ""..MONEY_TYPE_MONEY_NAME.."，此为玩家与玩家之间交易所使用的货币！";
		szText = ""..MONEY_TYPE_MONEY_NAME.."";
	elseif this:GetClientUserData( 0 ) == ATTR_ID_BULL then
		--szText = ""..MONEY_TYPE_BULL_NAME.."，此为玩家与NPC之间交易所使用的货币！";
		szText = "购买物品时优先消耗绑银，绑银不足将消耗银币";
	elseif this:GetClientUserData( 0 ) == ATTR_ID_BATTLE_GLORY then
		szText = "战场荣誉";
	elseif this:GetClientUserData( 0 ) == ATTR_ID_AOSHAN_BRAND then
		szText = "战魂徽章";
	elseif this:GetClientUserData( 0 ) == ATTR_ID_LIFT_BRAND then
		szText = "生存奖章";
	elseif this:GetClientUserData( 0 ) == ATTR_ID_SPARTA_GLORY then
		szText = "殿堂荣誉"
	elseif this:GetClientUserData( 0 ) == ATTR_ID_SCHOOL_VAL then
		szText = "传承点数"
	elseif this:GetClientUserData( 0 ) == ATTR_ID_ZHENGYI then
		szText = "正义点数"
	elseif this:GetClientUserData( 0 ) == -1 then
		szText = "公会贡献度";
	elseif this:GetClientUserData( 0 ) > 0 then
		local itemdef = getItemDef( this:GetClientUserData( 0 ) );
		if itemdef ~= nil then
			szText = "需要使用"..itemdef.Name.."兑换！";
		end
	end
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function ShopItemPriceIconBtn_OnLeave()
	GameTooltip:Hide();
end

--购买物品按钮
function ShopItemBuyBtn_OnClick()
	if arg1 ~= "LeftButton" then
		return;
	end

	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "公会商店贡献度消费" then
		MessageBoxFrame:Hide();
	end

	if CostMoneyConfirmFrame:IsShown() then
		return;
	end

	BuyShopItem( { btn = this:GetName() } );
end

function getMoneyNumByType( nType )
	local nHave = 0;
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return nHave; end
	
	if nType == ATTR_ID_BATTLE_GLORY then
		nHave = mainplayer:getBattleGlory()
	elseif nType == ATTR_ID_AOSHAN_BRAND then
		nHave = mainplayer:getAoShanBrand()
	elseif nType == ATTR_ID_LIFT_BRAND then
		nHave = mainplayer:getLiftBrand()
	elseif nType == ATTR_ID_SCHOOL_VAL then
		nHave = mainplayer:getSchoolVal()
	elseif nType == ATTR_ID_ZHENGYI then
		nHave = mainplayer:getZhengYiPoint();
	end
	return nHave;
end

function BuyShopItem( data )
	local argBtn		= getglobal( data["btn"] );   
	local btn		= getglobal( "ShopItemFrame" .. argBtn:GetClientID() .. "ShopItemBtn" );   --物品btn
	local priceFont		= getglobal( "ShopItemFrame" .. argBtn:GetClientID() .. "ShopItemPrice_Font" );
	local ShopList			= NpcShop:getShopList();
	local nClientId		= btn:GetClientID();
	nCurIndex = ( nCurPageNumber - 1 ) * nPageDisNumber + nClientId - 1;

	local mainplayer = ActorMgr:getMainPlayer();

	local GoodsType = btn:GetClientUserData( 2 );
	if GoodsType == SHOP_ITEM_CLAN_ITEM_CLIENT then
		--购买公会物品的话，只对权限进行判断
		local pos = mainplayer:getClanPosition();
		if pos == CLAN_POS_VICE_PRESIDENT or pos == CLAN_POS_PRESIDENT then
			local clanProductDef = GameClanManager:getProductDefByID( btn:GetClientUserData( 1 ) );
			if clanProductDef ~= nil then
				if clanProductDef.Category == 0 then	--道具
					local itemDef = getItemDef( clanProductDef.ID );
					if itemDef.MaxPile > 1 then
						ShopSellFrame:SetClientUserData( 0, itemDef.ItemID );
						ShopSellFrame:SetClientUserData( 1, GoodsType );
						ShopSellFrame:Show();
						return;
					end
				end
				NpcShop:buyClanItem( nCurIndex, 1 );
			end			
		else
			ShowMidTips("你没有权限进行此操作");
		end
	elseif GoodsType == SHOP_ITEM_ITEM_CLIENT then
		local itemDef		= getItemDef( btn:GetClientUserData( 1 ) );
		local nMoneyType	= btn:GetClientUserData(3);
		local nPrice		= nMoneyType > 0 and tonumber( priceFont:GetText() ) or itemDef.BuyPrice;

		local container		= mainplayer:getContainer();		
		-- 判断声望是否足够
		if not CanSatisfyDesignation( itemDef ) then
			local respactPartDef = GameRespact:getRespactPartyDef( itemDef.NeedRespact.ID );
			ShowMidTips( "购买本物品需要"..respactPartDef.PartyName.."声望达到"..GetRespactParseDesc( itemDef.NeedRespact.Value )[1] );
			return;
		end
		
		if not CanSatisfyTech(itemDef.ItemID) then
			local tech = NpcShop:getRequireTech(itemDef.ItemID)
			local t = {}
			for i = 0, tech.num - 1 do
				local id, level = tech.id[i], tech.level[i]
				local clanTech = GameClanManager:getClanTechDef(id, level)
				local name = clanTech and clanTech.Name or ""
				table.insert(t, string.format("%s%d级", name, level))
			end
			ShowMidTips("购买本物品需要科技  "..table.concat(t, "，"))
			return;
		end
		local maxNumToBuy = 0;
		--如果是学院徽章
		local t_SchoolItemList = { 5052601,5052602,5052603,5052604,5052605,5052606,5052607,5052608,5052609,5052610,5052611,5052612 }
		for i = 1, table.getn( t_SchoolItemList ) do
			if itemDef.ItemID == t_SchoolItemList[i] then
				if itemDef.ExchgItemID > 0 then
					local nExchangeItemNum	= container:getItemCount( itemDef.ExchgItemID )
					maxNumToBuy				= ( itemDef.BuyPrice == 0 and MAX_NUM_TO_BUY or math.floor( nExchangeItemNum / itemDef.BuyPrice ) )
					if maxNumToBuy <= 0 then
						local ExchangeItemDef  = getItemDef( itemDef.ExchgItemID )
						ShowMidTips( "购买物品所需" .. ExchangeItemDef.Name .. "不够" )
						return						
					end
				end
			end
		end
		
		if nMoneyType > 0 then
			if nMoneyType == ATTR_ID_BULL or nMoneyType == ATTR_ID_MONEY then 
				maxNumToBuy = itemDef.BuyPrice == 0 and MAX_NUM_TO_BUY or math.floor( getPlayerMoney( mainplayer, nMoneyType ) / (nPrice * ShopList.RebateCount/100) ) 
				if nMoneyType == ATTR_ID_BULL and checkNeedWarnCost( math.floor(nPrice * ShopList.RebateCount/100) ) then
					CostMoneyConfirmFrame:SetClientString("购买物品");
					CostMoneyConfirmFrame:SetClientUserData( 0, nCurIndex );
					SetWarnFrame( "ShopFrame" );
					CostMoneyConfirmFrame:Show();
					return;
				end
			else 
				maxNumToBuy = math.floor( getMoneyNumByType( nMoneyType ) / nPrice );
			end
		else
			if itemDef.ExchgItemID > 0 then
				local nExchangeItemNum	= container:getItemCount( itemDef.ExchgItemID );
				maxNumToBuy				= ( itemDef.BuyPrice == 0 and MAX_NUM_TO_BUY or math.floor( nExchangeItemNum / itemDef.BuyPrice ) );
			elseif itemDef.ClanContri > 0 then
				if mainplayer:isInClan() then
					local clanMember = GameClanManager:getMemberInfo( mainplayer:getName());
					if clanMember ~= nil then
						maxNumToBuy = math.floor( clanMember.Contribute/itemDef.ClanContri );
					end
				end
			elseif itemDef.MoneyType == ATTR_ID_SPARTA_GLORY then
				maxNumToBuy = math.floor( mainplayer:getSpartaGlory()/itemDef.BuyPrice )
			elseif itemDef.MoneyType == ATTR_ID_ZHENGYI then
				maxNumToBuy = math.floor( mainplayer:getZhengYiPoint()/itemDef.BuyPrice )
			else
				maxNumToBuy = itemDef.BuyPrice == 0 and MAX_NUM_TO_BUY or math.floor( getPlayerMoney( mainplayer, itemDef.MoneyType ) / (itemDef.BuyPrice * ShopList.RebateCount/100) ) 
				if itemDef.MoneyType == ATTR_ID_BULL and checkNeedWarnCost( math.floor(itemDef.BuyPrice * ShopList.RebateCount/100) ) then
					CostMoneyConfirmFrame:SetClientString("购买物品");
					CostMoneyConfirmFrame:SetClientUserData( 0, nCurIndex );
					SetWarnFrame( "ShopFrame" );
					CostMoneyConfirmFrame:Show();
					return;
				end
			end
			local t_MoneyName = { [ATTR_ID_BATTLE_GLORY] = "战场荣誉",
								  [ATTR_ID_AOSHAN_BRAND] = "战魂徽章",
								  [ATTR_ID_LIFT_BRAND] = "生存奖章",
								  [ATTR_ID_SCHOOL_VAL] = "传承点数",
								  [ATTR_ID_ZHENGYI] = "正义点数"  }
			for i=1,MAX_OTHER_MONEY_USE_CLIENT do
				if itemDef.MoneyUses[i-1].Val > 0 then
					local nHave = getMoneyNumByType( itemDef.MoneyUses[i-1].ID );
					
					local nCanBuy = math.floor( nHave/itemDef.MoneyUses[i-1].Val )
					if nCanBuy == 0 then
						ShowMidTips( "购买物品所需" .. ( t_MoneyName[itemDef.MoneyUses[i-1].ID] ) .. "不够" );
						return
					end
					maxNumToBuy = ( i == 1) and nCanBuy or math.min(maxNumToBuy,nCanBuy)
				end
			end
		end
		
		-- 隐藏新手指引按钮
		if nClientId == 3 then
			ShopFrameXinShouBtn:Hide();
		end

		local t_szText =	{	[false]	=	{ [ATTR_ID_MONEY] = ""..MONEY_TYPE_MONEY_NAME.."",		[ATTR_ID_BULL] = ""..MONEY_TYPE_BULL_NAME.."",		[ATTR_ID_SPARTA_GLORY] = "殿堂荣誉"		},
								[true] =	{ [ATTR_ID_MONEY] = "兑换道具",	[ATTR_ID_BULL] = "兑换道具" }
							};
		if maxNumToBuy == 0 then
			if nMoneyType > 0 then
				if nMoneyType == ATTR_ID_MONEY then
					return ShowBuyMoneyFrame();
				else
					return ShowMidTips( "购买物品所需" .. getBuyItemMoneyTypeName(nMoneyType) .. "不够" );
				end
			else 
				if itemDef.ClanContri > 0 then
					return ShowMidTips( "购买物品所需公会贡献度不够" );
				else
					return ShowMidTips( "购买物品所需" .. ( t_szText[itemDef.ExchgItemID > 0][itemDef.MoneyType] ) .. "不够" );
				end
			end
		else
			if nMoneyType == 0 then
				-- 花费超过100贡献度提示
				if itemDef.ClanContri >= 100 then
					MessageBox("","花费#G".. itemDef.ClanContri .."#n贡献度购买#G" ..itemDef.Name .."#n，是否购买？");
					MessageBoxFrame:SetClientString("公会商店贡献度消费");
					MessageBoxFrame:SetClientUserData( 0, nCurIndex );
					MessageBoxFrame:SetClientUserData( 1, 1 );
					return;
				end

				if itemDef.ExchgItemID > 0 and getItemType( itemDef.ItemID ) == ITEM_SLOTTYPE_AMM and 
					not IsFitRace( { actor = mainplayer, compareRace = itemDef.WearPre.Career } ) then
					
					if not CheckContainerGridEnough( { id = itemDef.ItemID, buyNum = 1 } ) then
						-- TODO: 服务器频道
						AddGameMiddleTips2( "背包空间不足，放不下要购买的物品" );
						return;
					end

					ShopExchangeConfirmFrame:SetClientUserData( 0, itemDef.ItemID );
					ShopExchangeConfirmFrame:SetClientUserData( 1, 1 );
					ShopExchangeConfirmFrame:Show();
					return;
				end
			end
		end
		if itemDef.MaxPile > 1 then
			ShopSellFrame:SetClientUserData( 0, btn:GetClientUserData( 1 ) );
			ShopSellFrame:SetClientUserData( 1, GoodsType );
			ShopSellFrame:SetClientUserData( 2, nMoneyType );
			ShopSellFrame:SetClientUserData( 3, nPrice );
			ShopSellFrame:Show();
			return;
		elseif CheckGoodsMoneyAndPrice( nMoneyType,nPrice,nCurIndex,itemDef ) then 
			return
		end

		-- 对地图判断
		if MapShower:getRealMapId()== WorldCityMapID then
			MessageBox("","花费".. math.floor((itemDef.BuyPrice * ShopList.RebateCount/100)) .."银币购买" ..itemDef.Name .."，是否购买？");
			MessageBoxFrame:SetClientString("公会购买")
			MessageBoxFrame:SetClientUserData( 0,nCurIndex );
			return;
		end

		NpcShop:buyItem( nCurIndex, 1 );
	end
end
-- 检查商品价格限度
function CheckGoodsMoneyAndPrice( moneyType, price, itemIndex,itemDef )
	local tipRich 	= getglobal("ShopPriceWarnFrameTipRich")
	local warnRich	= getglobal("ShopPriceWarnFrameConfirmRich")
	local szText	= ""
	local xPoint	= 290
	ShopPriceWarnFrame:SetClientUserData( 0 , itemIndex)
	if moneyType == ATTR_ID_BULL or ( moneyType == 0 and itemDef.MoneyType == ATTR_ID_BULL ) and price >= 20000 then 
		szText = "绑银：购买此道具需要花费"..price
		tipRich:SetText(szText,255,252,254)
		ShopPriceWarnFrameTex:SetTexUV( 78,494,20,18 )
		xPoint = math.ceil( tipRich:GetTextExtentWidth(szText) / GetScreenScaleY() / 2 ) + 180
		ShopPriceWarnFrameTex:SetPoint( "topleft", "ShopPriceWarnFrame", "topleft", xPoint, 50 );
		ShopPriceWarnFrameConfirmRich:SetText("",255,0,0)
		ShopPriceWarnFrame:Show();
		return true	
	elseif moneyType == ATTR_ID_MONEY or ( moneyType == 0 and itemDef.MoneyType == ATTR_ID_MONEY ) and price >= 10000 then
		szText = "银币：购买此道具需要花费"..price
		tipRich:SetText(szText,255,252,254)
		ShopPriceWarnFrameTex:SetTexUV( 117,494,17,18 )
		xPoint = math.ceil( tipRich:GetTextExtentWidth(szText) / GetScreenScaleY() / 2 ) + 180
		ShopPriceWarnFrameTex:SetPoint( "topleft", "ShopPriceWarnFrame", "topleft", xPoint, 50 );
		ShopPriceWarnFrameConfirmRich:SetText("银币是重要流通货币",255,0,0)
		ShopPriceWarnFrame:Show();
		return true
	else 
		ShopPriceWarnFrame:SetClientUserData( 0 , -1 )
		if ShopPriceWarnFrame:IsShown() then 
			ShopPriceWarnFrame:Hide()
		end
		return false;
	end
end

function CheckContainerGridEnough( data )
	local itemDef = getItemDef( data["id"] );
	local nBuyNum = data["buyNum"];

	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	return container:getUnusedGridNum( CONTAINER_TYPE_SUNDRIES ) >= math.ceil( nBuyNum / itemDef.MaxPile );
end

--修复所有
function CFRepairAllBtn_OnClick()
	if arg1 ~= "LeftButton" then
		return;
	end
	if CostMoneyConfirmFrame:IsShown() then
		return;
	end

	local repairXinShouBtn = getglobal( "ShopFrameRepairAllXinShouBtn" );
	if repairXinShouBtn:IsShown() and not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_REPAIR_ALL_BTN ) then
		GameWizard:doStep( GAMEWIZARD_FIRST_BLINK_REPAIR_ALL_BTN );
	end
	if repairXinShouBtn:IsShown() and not GameWizard:isDo( GAMEWIZARD_FIRST_REPAIR ) then
		GameWizard:doStep( GAMEWIZARD_FIRST_REPAIR );
	end
	repairXinShouBtn:Hide();
	ChangeCursorState("normal");

    nRepairState = nRepairAll;
	if NpcShop:getRepairAllPrice() == 0 then
		NpcShop:repairAllItem();
		return;
	end

	SRF_MinNumber_Font:SetText( "本次修理需花费" .. NpcShop:getRepairAllPrice() .. ""..MONEY_TYPE_BULL_NAME.."" );
	ShopRepairFrame:Show();
end

--修复单件
function CFRepairOneBtn_OnClick()
	if arg1 ~= "LeftButton" then
		return;
	end
	if CostMoneyConfirmFrame:IsShown() then
		return;
	end

	nRepairState = nRepairOne;
	ChangeCursorState("repair");

	if ShopRepairFrame:IsShown() then ShopRepairFrame:Hide() end
end

--左键点击背包上的物品时(修理)
local nCurSelectRepairItemListType;
local nCurSelectRepairItemIndex;
local nCurSelectRepairItemID;
function RepairOneItem( nlistType, nIndex )
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	local item = nil;

	--如果是在背包点击
	if nlistType == CONTAINER_TYPE_SUNDRIES then
		local container = MainPlayer:getContainer();
		item = container:getItem( nlistType, nIndex );
	--如果是在主角面板点击
	elseif nlistType == CONTAINER_TYPE_WEAR then
		local Eqiup = MainPlayer:getEquip();
		if Eqiup == nil then return end
		item = Eqiup:getItem( nIndex );
	end

	if item:getItemType() ~= ITEM_SLOTTYPE_AMM or item:getItemId() == 0 then return end
	--缓存相关参数
	
	if not item:canRepair() then
		NpcShop:repairOneItem( nlistType, nIndex, item:getItemId() );
		--ShowMidTips("该物品无法修理");
		return;
	end
	

	local itemDef					= item:getItemDef();
	-- 若修理的是装备，但是它的最大耐久度为0，则不能修理该装备
	
	if nlistType == CONTAINER_TYPE_WEAR and itemDef.ArmDur.MaxDur == 0 then
		NpcShop:repairOneItem( nlistType, nIndex, itemDef.ItemID );
		return;
	end
	
	nCurSelectRepairItemListType	= nlistType;
	nCurSelectRepairItemIndex		= nIndex;
	nCurSelectRepairItemID			= itemDef.ItemID;
    nRepairState					= nRepairOne;
	local nRepairPrice				= NpcShop:getRepairOnePrice( item );
	-- 只有物品处于损坏状态才会弹出确认修理框
	if nRepairPrice > 0 then
		SRF_MinNumber_Font:SetText( "本次修理需花费" .. nRepairPrice .. ""..MONEY_TYPE_BULL_NAME.."" );
		ShopRepairFrame:Show();
	end
end

--右键点击背包上的物品时(出售)
function SellItem( nlistType, nIndex)
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	local container = MainPlayer:getContainer();

	local item = container:getItem( nlistType, nIndex );
	if item:getItemType() == ITEM_SLOTTYPE_NONE or item:getItemId() == 0 then return end

	SellToShopConfirmFrame:Show();
	SellToShopConfirmFrame:SetClientUserData( 0, nlistType );
	SellToShopConfirmFrame:SetClientUserData( 1, nIndex );
	SellToShopConfirmFrame:SetClientUserData( 2, 1 );
	--NpcShop:sellItem( nlistType, nIndex, 1 );
end

--左键点击商店上的回购物品时(回购)
function BuySaveItem(nIndex)
	NormalItemTipsFrame:Hide();
	NpcShop:buySaveItem(nIndex);
end

--上一个界面
function CFPreviousBtn_OnClick()
	if arg1 ~= "LeftButton" or nCurPageNumber <= 1 then
		return;
	end
	-- 关闭新手指引特效显示
	if nCurPageNumber > 1 then
		nCurPageNumber = nCurPageNumber - 1;
	end
	UpdateShopFrameItem();

	ShopFrameXinShouBtn:Hide();
end

--下一个界面
function CFNextBtn_OnClick()
	if arg1 ~= "LeftButton" or nCurPageNumber >= nMaxPageNumber then
		return;
	end	
	-- 关闭新手指引特效显示
	if nCurPageNumber < nMaxPageNumber then
		nCurPageNumber = nCurPageNumber + 1;
	end
	UpdateShopFrameItem();
	ShopFrameXinShouBtn:Hide();
end

function UpdatePageNumber()
	CFPageNumber_Font:SetText( nCurPageNumber .. "/" .. nMaxPageNumber );
end

-- 购买的物品数量最大化
function SSF_MaxBtn_OnClick()
	local maxNumToBuy = 0;
	local itemDef = getItemDef( ShopSellFrame:GetClientUserData( 0 ) );
	local nMoneyType = ShopSellFrame:GetClientUserData( 2 );
	local nPrice = ShopSellFrame:GetClientUserData( 3 );
	local mainplayer = ActorMgr:getMainPlayer();
	local ShopList = NpcShop:getShopList();
	if nMoneyType > 0 then
		maxNumToBuy = nPrice == 0 and MAX_NUM_TO_BUY or math.floor( getPlayerMoney( mainplayer, nMoneyType ) / (nPrice * ShopList.RebateCount/100) )
	else
		maxNumToBuy = itemDef.BuyPrice == 0 and MAX_NUM_TO_BUY or math.floor( getPlayerMoney( mainplayer, itemDef.MoneyType ) / (itemDef.BuyPrice * ShopList.RebateCount/100) )
	end
	SSF_SellNumberEdit:SetText( math.min( maxNumToBuy, MAX_NUM_TO_BUY ) );
end

-- 购买的物品数量最小化
function SSF_MinBtn_OnClick()
	local maxNumToBuy = 0;
	local itemDef = getItemDef( ShopSellFrame:GetClientUserData( 0 ) );
	local nMoneyType = ShopSellFrame:GetClientUserData( 2 );
	local nPrice = ShopSellFrame:GetClientUserData( 3 );
	local mainplayer = ActorMgr:getMainPlayer();
	local ShopList = NpcShop:getShopList();
	if nMoneyType > 0 then
		maxNumToBuy = nPrice == 0 and MAX_NUM_TO_BUY or math.floor( getPlayerMoney( mainplayer, nMoneyType ) / (nPrice * ShopList.RebateCount/100) )
	else
		maxNumToBuy = itemDef.BuyPrice == 0 and MAX_NUM_TO_BUY or math.floor( getPlayerMoney( mainplayer, itemDef.MoneyType ) / (itemDef.BuyPrice * ShopList.RebateCount/100) )
	end
	SSF_SellNumberEdit:SetText( math.min( maxNumToBuy, 1 ) );
end

--确定购买
function SSF_OKBtn_OnClick()
	local nItemID	= ShopSellFrame:GetClientUserData( 0 );
	local nMoneyType = ShopSellFrame:GetClientUserData( 2 );
	local nPrice = ShopSellFrame:GetClientUserData( 3 );
	local sellFrame	= getglobal( "ShopSellFrame" );
	if nItemID == 0 and not sellFrame:IsShown() then
		return;
	end
	
	local nNumber		= tonumber( SSF_SellNumberEdit:GetText() );
	local itemDef		= getItemDef( nItemID );

	if nNumber == 0 or nNumber == nil then
		ShowMidTips( "请重新输入要购买的数量" );
		return;
	end

	ShopSellFrame:Hide();
	SSF_SellNumberEdit:Clear();
	ClearFocus();

	if nMoneyType > 0 then
		if nMoneyType == ATTR_ID_BULL then
			local ShopList = NpcShop:getShopList();
			local money = math.floor(nPrice * nNumber * ShopList.RebateCount / 100);
			if checkNeedWarnCost( money ) then
				CostMoneyConfirmFrame:SetClientString("批量购买");
				CostMoneyConfirmFrame:SetClientUserData( 0, nCurIndex );
				CostMoneyConfirmFrame:SetClientUserData( 1, nNumber );
				SetWarnFrame( "ShopFrame" );
				CostMoneyConfirmFrame:Show();
				return;
			end
		end
	else
		-- 若为兑换道具才能购买的商品，则弹出一个二级确认面板
		if itemDef.ExchgItemID > 0 and getItemType( itemDef.ItemID ) == ITEM_SLOTTYPE_AMM and 
				not IsFitRace( { actor = mainplayer, compareRace = itemDef.WearPre.Career } ) then

			if not CheckContainerGridEnough( { id = itemDef.ItemID, buyNum = nNumber } ) then
				-- TODO: 服务器频道
				AddGameMiddleTips2( "背包空间不足，放不下要购买的物品" );
				return;
			end

			ShopExchangeConfirmFrame:SetClientUserData( 0, itemDef.ItemID );
			ShopExchangeConfirmFrame:SetClientUserData( 1, nNumber );
			ShopExchangeConfirmFrame:Show();
			return;
		-- 花费超过100贡献度提示
		elseif itemDef.ClanContri * nNumber >= 100 then
				MessageBox("","花费#G".. itemDef.ClanContri * nNumber .."#n贡献度购买#G" ..itemDef.Name .. "#B" .. nNumber .. "#n个，是否购买？");
				MessageBoxFrame:SetClientString("公会商店贡献度消费");
				MessageBoxFrame:SetClientUserData( 0, nCurIndex );
				MessageBoxFrame:SetClientUserData( 1, nNumber );
				return;
		elseif itemDef.MoneyType == ATTR_ID_BULL and itemDef.ExchgItemID == 0 and itemDef.ClanContri == 0 then
			local ShopList = NpcShop:getShopList();
			local money = math.floor(itemDef.BuyPrice * nNumber * ShopList.RebateCount / 100);
			if checkNeedWarnCost( money ) then
				CostMoneyConfirmFrame:SetClientString("批量购买");
				CostMoneyConfirmFrame:SetClientUserData( 0, nCurIndex );
				CostMoneyConfirmFrame:SetClientUserData( 1, nNumber );
				SetWarnFrame( "ShopFrame" );
				CostMoneyConfirmFrame:Show();
				return;
			end
		end
	end
	NpcShop:buyItem( nCurIndex, nNumber );
end

--取消购买
function SSF_CancelBtn_OnClick()
	ShopSellFrame:Hide();
	SSF_SellNumberEdit:Clear();
end

function UpdateShopSellFrameCost()
	local frame		= getglobal( "ShopSellFrame" );
	local nItemID	= frame:GetClientUserData( 0 );
	local nMoneyType = frame:GetClientUserData( 2 );
	local nPrice = ShopSellFrame:GetClientUserData( 3 );
	if nItemID == 0 and not frame:IsShown() then
		return;
	end

	local maxNumToBuy	= 0;
	local itemDef		= getItemDef( nItemID );
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local edit			= getglobal( "SSF_SellNumberEdit" );
	local nMaxCost		= 0;
	local ShopList = NpcShop:getShopList();
	if nMoneyType > 0 then
		maxNumToBuy = getBuyItemMaxValue( nMoneyType,nPrice );
		if nMoneyType == ATTR_ID_MONEY then 
			nMaxCost = getPlayerMoney( mainplayer, nMoneyType );
		else 
			nMaxCost = getPlayerMoney( mainplayer );
		end
	else
		if itemDef.ExchgItemID > 0 then
			local ExchangeItemDef	= getItemDef( itemDef.ExchgItemID );
			nMaxCost				= container:getItemCount( ExchangeItemDef.ItemID );
			maxNumToBuy				=  math.floor( nMaxCost / itemDef.BuyPrice );
		elseif itemDef.ClanContri > 0 then
			nMaxCost	= GetMainPlayerClanContribute();
			maxNumToBuy =  ( itemDef.ClanContri == 0 and MAX_NUM_TO_BUY or math.floor( nMaxCost / itemDef.ClanContri ) );
		elseif itemDef.MoneyType == ATTR_ID_MONEY then
			nMaxCost	= getPlayerMoney( mainplayer, ATTR_ID_MONEY );
			maxNumToBuy = ( itemDef.BuyPrice == 0 and MAX_NUM_TO_BUY or math.floor( nMaxCost / (itemDef.BuyPrice * ShopList.RebateCount/100) ) );
		elseif itemDef.MoneyType == ATTR_ID_BULL then
			nMaxCost	= getPlayerMoney( mainplayer );
			maxNumToBuy =  ( itemDef.BuyPrice == 0 and MAX_NUM_TO_BUY or math.floor( nMaxCost / (itemDef.BuyPrice * ShopList.RebateCount/100) ) );
		elseif itemDef.MoneyType == ATTR_ID_SPARTA_GLORY then
			nMaxCost	= mainplayer:getSpartaGlory()
			maxNumToBuy = math.floor( nMaxCost / itemDef.BuyPrice )
		end
	end

	local moneyUseType = 0;
	if nMoneyType == 0 then 
		local nIndex = 0;
		for idx=1,MAX_OTHER_MONEY_USE_CLIENT do
			if itemDef.MoneyUses[idx-1].Val > 0 then
				if idx > 2 then break end
				nIndex = nIndex + 1;
			end	
		end
		if nIndex == 2 then
			local maxNumToBuy1 = getBuyItemMaxValue( itemDef.MoneyUses[0].ID,itemDef.MoneyUses[0].Val );
			local maxNumToBuy2 = getBuyItemMaxValue( itemDef.MoneyUses[1].ID,itemDef.MoneyUses[1].Val );
			maxNumToBuy = maxNumToBuy1 > maxNumToBuy2 and maxNumToBuy2 or maxNumToBuy1;
			moneyUseType = maxNumToBuy1 > maxNumToBuy2 and 2 or 1;
		elseif nIndex == 1 then
			maxNumToBuy = getBuyItemMaxValue( itemDef.MoneyUses[0].ID,itemDef.MoneyUses[0].Val );
			moneyUseType = 1;
		end
	end
	
	-- 若什么也买不起，则自动关闭这个面板
	if maxNumToBuy == 0 then
		local name = "货币"
		if nMoneyType > 0 then
			name = getBuyItemMoneyTypeName( nMoneyType );
		else
			if itemDef.ExchgItemID ~= 0 then
				name = getItemDef( itemDef.ExchgItemID ).Name
			elseif itemDef.ClanContri > 0 then
				name = MONEY_TYPE_CONTRI_NAME
			elseif itemDef.MoneyType == ATTR_ID_MONEY then
				name = MONEY_TYPE_MONEY_NAME
			elseif itemDef.MoneyType == ATTR_ID_BULL then
				name = MONEY_TYPE_BULL_NAME
			elseif itemDef.MoneyType == ATTR_ID_SPARTA_GLORY then
				name = "殿堂荣誉"
			end
			if moneyUseType > 0 then
				name = getBuyItemMoneyTypeName( itemDef.MoneyUses[moneyUseType-1].ID );
			end
		end
		ShowMidTips( name.."不足，中断购买操作" );
		frame:Hide();
		return;
	end
	
	CheckEditNumberInput( edit );

	local nCurNum	= tonumber( edit:GetText() );
	if nCurNum == nil then 
		local t_fonts = { 	["ShopSellFrameCurCostFont"] = { value = 0 }, 
							["ShopSellFrameCurHaveFont"] = { value = nMaxCost }, };
		util.SetFonts( t_fonts );
		return; 
	end
	
	local nCurCost = math.min( math.max( nCurNum, 1 ), maxNumToBuy );
	-- 设置现消耗的，身上总共有的
	local value = 0
	if nMoneyType > 0 then
		if nMoneyType == ATTR_ID_BULL or nMoneyType == ATTR_ID_MONEY then
			value = math.floor( nPrice * ShopList.RebateCount/100 );
		end
	else
		if itemDef.ExchgItemID > 0 then
			value = itemDef.BuyPrice
		elseif itemDef.ClanContri > 0 then
			value = itemDef.ClanContri
		elseif itemDef.MoneyType == ATTR_ID_MONEY or itemDef.MoneyType == ATTR_ID_BULL then
			value = math.floor( itemDef.BuyPrice * ShopList.RebateCount/100 )
		end
	end
	local t_fonts = {	["ShopSellFrameCurCostFont"] = { value = nCurCost * value }, 
						["ShopSellFrameCurHaveFont"] = { value = nMaxCost }, 
					};
	util.SetFonts( t_fonts );
	
	local curCostFont = getglobal("ShopSellFrameCurCostFont");
	local curHaveFont = getglobal("ShopSellFrameCurHaveFont");
	ShopSellFrameCostTex:SetPoint( "topleft", curCostFont:GetName(), "topleft", curCostFont:GetTextExtentWidth( curCostFont:GetText() ) / GetScreenScaleY()+ 10 , 
									-4 );
	ShopSellFrameHaveTex:SetPoint( "topleft", curHaveFont:GetName(), "topleft", curHaveFont:GetTextExtentWidth( curHaveFont:GetText() )/ GetScreenScaleY() + 10 , 
									-4 );
	-- 若editbox中的数值没超出极限范围，则无需重新设置其底层数据
	-- 否则会出现不能选中所有字的情况
	if math.max( nCurNum, 1 ) <= maxNumToBuy then
		return;
	end
	
	SSF_SellNumberEdit:SetText( nCurCost );
end
function getBuyItemMaxValue( nMoneyType,nPrice )
	local nMaxCost = 0;
	local maxNumToBuy = 0;
	local mainplayer	= ActorMgr:getMainPlayer();
	local ShopList 		= NpcShop:getShopList();
	if nPrice <= 0 then 
		return MAX_NUM_TO_BUY ;
	end
	if nMoneyType == ATTR_ID_BULL or nMoneyType == ATTR_ID_MONEY then 
		nMaxCost	= getPlayerMoney( mainplayer, nMoneyType );
		maxNumToBuy = math.floor( nMaxCost / (nPrice * ShopList.RebateCount/100) );
	else 
		nMaxCost = getMoneyNumByType( nMoneyType );
		maxNumToBuy = math.floor( nMaxCost / nPrice );
	end
	return maxNumToBuy;
end

function getBuyItemMoneyTypeName( nMoneyType )
	local moneyName = "所需道具";
	if nMoneyType == ATTR_ID_MONEY then
		moneyName = MONEY_TYPE_MONEY_NAME
	elseif nMoneyType == ATTR_ID_BULL then
		moneyName = MONEY_TYPE_BULL_NAME
	elseif nMoneyType == ATTR_ID_ZHENGYI then
		moneyName = "正义点数"
	elseif nMoneyType == ATTR_ID_LIFT_BRAND then
		moneyName = "生存徽章"
	elseif nMoneyType == ATTR_ID_BATTLE_GLORY then
		moneyName = "战场荣誉"
	elseif nMoneyType == ATTR_ID_AOSHAN_BRAND then
		moneyName = "战魂徽章"
	elseif nMoneyType == ATTR_ID_SCHOOL_VAL then
		moneyName = "传承点数"
	end
	return moneyName;
end
function ShopSellFrame_Test_OnShow()
	local animationTexture = getglobal( "SSF_OKBtnUVAnimationTex" );
	animationTexture:SetUVAnimation( 50, true );
	animationTexture:Show();
end

function CheckShopSellFrameXinShouGuide()
	local animationTexture = getglobal( "SSF_OKBtnUVAnimationTex" );
	if not GameWizard:isDo( GAMEWIZARD_FIRST_OPEN_BUY_CONFIRM ) then
		animationTexture:SetUVAnimation( 50, true );
		animationTexture:Show();
		GameWizard:doStep( GAMEWIZARD_FIRST_OPEN_BUY_CONFIRM);
	else
		animationTexture:Hide();
	end
end

function ShopSellFrame_OnShow()
	local t_hideIcons = { "SSF_OKBtnUVAnimationTex", };
	util.HideIcons( t_hideIcons );
	SetFocusFrame( "SSF_SellNumberEdit" );
	SSF_SellNumberEdit:SetText( "1" );
	SSF_SellNumberEdit:SelectAllText();
	-- 设置图标
	local frame	= getglobal( "ShopSellFrame" );
	local nItemID	= frame:GetClientUserData( 0 );
	if nItemID == 0 then
		return;
	end

	local maxNumToBuy	= 0;
	local itemDef		= getItemDef( nItemID );
	local nMoneyType	= frame:GetClientUserData( 2 );
	local nPrice		= frame:GetClientUserData( 3 );
	if nMoneyType == ATTR_ID_MONEY or nMoneyType == ATTR_ID_BULL or nMoneyType == ATTR_ID_ZHENGYI then
		local t_config = 
		{	size = ( ( nMoneyType == ATTR_ID_MONEY ) and ( { width = 17, height = 18 } ) or ( { width = 18, height = 18 } ) ), 

			path = "uires\\ZhuJieMian\\1.dds", 

			UV = ( ( nMoneyType == ATTR_ID_MONEY ) and ( { x = 117, y = 494, width = 17, height = 18 } ) or 
															( { x = 78, y = 494, width = 20, height = 18 } ) ) 
		};

		local t_textures = 
		{
			["ShopSellFrameCostTex"] = t_config,
			["ShopSellFrameHaveTex"] = t_config,
		};

		for name, data in pairs( t_textures ) do
			local tex = getglobal( name );
			tex:SetTexture( data["path"] );

			local UV = data["UV"];
			tex:SetTexUV( UV["x"], UV["y"], UV["width"], UV["height"] );

			local size = data["size"];
			tex:SetSize( size["width"], size["height"] );
		end
	else
		if itemDef.ExchgItemID > 0 then
			local ExchangeItemDef	= getItemDef( itemDef.ExchgItemID );	
			local moneyTexPath		= util.GetExchangMoneyIconPath( { iconID =ExchangeItemDef.IconID } );
			local t_tgas = {	["ShopSellFrameCostTex"] = { path = moneyTexPath }, 
								["ShopSellFrameHaveTex"] = { path = moneyTexPath }, };
			util.SetTgas( t_tgas );
		elseif itemDef.ClanContri > 0 then
			ShopSellFrameCostTex:SetTexture("uires\\ZhuJieMian\\1.dds");
			ShopSellFrameCostTex:SetTexUV(577,570,14,18);
			ShopSellFrameHaveTex:SetTexture("uires\\ZhuJieMian\\1.dds");
			ShopSellFrameHaveTex:SetTexUV(577,570,14,18);
			--priceTex:SetSize( 14,18 );
		else
			local t_config = 
			{	size = ( ( itemDef.MoneyType == ATTR_ID_MONEY ) and ( { width = 17, height = 18 } ) or ( { width = 18, height = 18 } ) ), 

				path = "uires\\ZhuJieMian\\1.dds", 

				UV = ( ( itemDef.MoneyType == ATTR_ID_MONEY ) and ( { x = 117, y = 494, width = 17, height = 18 } ) or 
																( { x = 78, y = 494, width = 20, height = 18 } ) ) 
			};

			local t_textures = 
			{
				["ShopSellFrameCostTex"] = t_config,
				["ShopSellFrameHaveTex"] = t_config,
			};

			for name, data in pairs( t_textures ) do
				local tex = getglobal( name );
				tex:SetTexture( data["path"] );

				local UV = data["UV"];
				tex:SetTexUV( UV["x"], UV["y"], UV["width"], UV["height"] );

				local size = data["size"];
				tex:SetSize( size["width"], size["height"] );
			end
		end
	end

	local TipsTxt = getglobal("ShopSellFrame_TipFont");
	local TipsTex = getglobal("ShopSellFrame_TipTexture")
	if nMoneyType == ATTR_ID_MONEY or nMoneyType == ATTR_ID_BULL then
		TipsTex:Show();
		TipsTxt:SetText("购买物品时优先消耗绑银，绑银不足将消耗银币");
	else
		if itemDef.ExchgItemID > 0 or itemDef.ClanContri > 0 or itemDef.MoneyType == ATTR_ID_SPARTA_GLORY then
			TipsTex:Hide();
			TipsTxt:SetText("");
		elseif itemDef.MoneyType == ATTR_ID_MONEY or itemDef.MoneyType == ATTR_ID_BULL then
			TipsTex:Show();
			TipsTxt:SetText("购买物品时优先消耗绑银，绑银不足将消耗银币");
		end

		local nIndex = 1;
		for idx=1,MAX_OTHER_MONEY_USE_CLIENT do
			if itemDef.MoneyUses[idx-1].Val > 0 then
				if nIndex > 2 then break end
				TipsTex:Hide();
				TipsTxt:SetText("");
				nIndex = nIndex + 1;
			end
		end
	end
	
	UpdateShopSellFrameCost();
	CheckShopSellFrameXinShouGuide();
end

function ShopSellFrame_OnHide()
	ShopSellFrame:SetClientUserData( 0, 0 );
	ShopSellFrame:SetClientUserData( 1, 0 );
	ShopSellFrame:SetClientUserData( 2, 0 );
	ShopSellFrame:SetClientUserData( 3, 0 );
	local t_hideIcons = { "SSF_OKBtnUVAnimationTex", };
	util.HideIcons( t_hideIcons );
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "公会购买" then
		MessageBoxFrame:Hide();
	end
end

function ShopSellFrame_EditBackBtn_OnClick()
	SetFocusFrame( "SSF_SellNumberEdit" );
end

function SSF_SellNumberEdit_onfocuslost()
	local nNumber = SSF_SellNumberEdit:GetText();
	if nNumber == "" then
		nNumber = "0";
	end
	SSF_SellNumberEdit:SetText(nNumber);
end

-- 增加购买数据
function SSF_AddBtn_OnClick()
	local maxNumToBuy	= 0;
	local itemDef		= getItemDef( ShopSellFrame:GetClientUserData( 0 ) );
	local nMoneyType	= ShopSellFrame:GetClientUserData( 2 );
	local nPrice		= ShopSellFrame:GetClientUserData( 3 );
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local ShopList = NpcShop:getShopList();
	if nMoneyType > 0 then	
		maxNumToBuy = getBuyItemMaxValue( nMoneyType,nPrice );
	else
		if itemDef.ExchgItemID > 0 then
			local ExchangeItemDef	= getItemDef( itemDef.ExchgItemID );
			local nMaxCost			= container:getItemCount( ExchangeItemDef.ItemID );
			maxNumToBuy				=  math.floor( nMaxCost / itemDef.BuyPrice );
		elseif itemDef.ClanContri > 0 then
			if mainplayer:isInClan() then
				local clanMember = GameClanManager:getMemberInfo( mainplayer:getName());
				if clanMember ~= nil then
					maxNumToBuy = math.floor( clanMember.Contribute/itemDef.ClanContri );
				end
			end
		else
			maxNumToBuy = itemDef.BuyPrice == 0 and MAX_NUM_TO_BUY or math.floor( getPlayerMoney( mainplayer, itemDef.MoneyType ) / (itemDef.BuyPrice * ShopList.RebateCount/100) )
		end
	end

	assert( SSF_SellNumberEdit:GetText() ~= "", "输入数量的值不能为空字符串!!" );
	if SSF_SellNumberEdit:GetText() == "" then
		SSF_SellNumberEdit:SetText( "0" );
	end

	local nCurNum = tonumber( SSF_SellNumberEdit:GetText() );
	SSF_SellNumberEdit:SetText( math.min( nCurNum + 1, maxNumToBuy ) )
end

-- 减少购买数据
function SSF_ReduceBtn_OnClick()
	local maxNumToBuy	= 0;
	local itemDef		= getItemDef( ShopSellFrame:GetClientUserData( 0 ) );
	local nMoneyType	= ShopSellFrame:GetClientUserData( 2 );
	local nPrice		= ShopSellFrame:GetClientUserData( 3 );
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local ShopList = NpcShop:getShopList();
	if nMoneyType > 0 then
		maxNumToBuy = getBuyItemMaxValue( nMoneyType,nPrice );
	else
		if itemDef.ExchgItemID > 0 then
			local ExchangeItemDef	= getItemDef( itemDef.ExchgItemID );
			local nMaxCost			= container:getItemCount( ExchangeItemDef.ItemID );
			maxNumToBuy				=  math.floor( nMaxCost / itemDef.BuyPrice );
		elseif itemDef.ClanContri > 0 then
			if mainplayer:isInClan() then
				local clanMember = GameClanManager:getMemberInfo( mainplayer:getName());
				if clanMember ~= nil then
					maxNumToBuy = math.floor( clanMember.Contribute/itemDef.ClanContri );
				end
			end
		else
			maxNumToBuy = itemDef.BuyPrice == 0 and MAX_NUM_TO_BUY or math.floor( getPlayerMoney( mainplayer, itemDef.MoneyType ) / (itemDef.BuyPrice * ShopList.RebateCount/100) )
		end
	end
	if maxNumToBuy == 0 then
		SSF_SellNumberEdit:SetText( 0 );
	end

	local nNumber = tonumber( SSF_SellNumberEdit:GetText() );
	if nNumber == nil then
		return;
	end

	SSF_SellNumberEdit:SetText( math.max( nNumber - 1, 1 ) );
end

function SSF_SellNumberEdit_OnUpdate()
	UpdateShopSellFrameCost();
end

-- 确定修理二次确认面板
function ShowShopRepairFrameXinShouInfo( data )
	local btn	= getglobal( data["btn"] );
	local rich	= getglobal( data["btn"].."Rich" );
	local animationTexture	= getglobal( data["btn"].."UVAnimationTex" );
	local nSelfDefineExtendWidth = 10;
	rich:SetClientString( data["text"] );
	ReSizeTooltip( rich:GetName(), btn:GetName(), nSelfDefineExtendWidth );
	animationTexture:SetUVAnimation( 50, true );
	btn:Show();
end

function CheckShopRepairFrameXinShou()
	if GameWizard:isDo(GAMEWIZARD_FIRST_BLINK_REPAIR_ALL_BTN) and  
			not GameWizard:isDo(GAMEWIZARD_FIRST_BLINK_CONFIRM_REPAIR_BTN) then

		local animationTexture	= getglobal( "SRF_OKBtnUVAnimationTex" );
		animationTexture:SetUVAnimation( 50, true );
		animationTexture:Show();
	end
end

function ShopRepairFrame_OnShow()
	local t_hideIcons = { "SRF_OKBtnUVAnimationTex", };
	util.HideIcons( t_hideIcons );
	CheckShopRepairFrameXinShou();
end

function ShopRepairFrame_OnHide()
	local animationTexture	= getglobal( "SRF_OKBtnUVAnimationTex" );
	animationTexture:Hide();
end

--确定修理
function SRF_OKBtn_OnClick()
	local _,_,money = string.find( SRF_MinNumber_Font:GetText(), "(%d+)" );
	local mainplayer = ActorMgr:getMainPlayer();
	if getPlayerMoney( mainplayer ) < tonumber( money ) then
		ShowMidTips( "绑银不足，无法修理" )
		return
	end
	if checkNeedWarnCost( tonumber( money ) ) then
		CostMoneyConfirmFrame:SetClientString("物品修理");
		if nRepairState == nRepairAll then
			CostMoneyConfirmFrame:SetClientUserData( 0, 0 );
			CostMoneyConfirmFrame:SetClientUserData( 1, 0 );
			CostMoneyConfirmFrame:SetClientUserData( 2, 0 );
		elseif nRepairState == nRepairOne then
			CostMoneyConfirmFrame:SetClientUserData( 0, nCurSelectRepairItemListType );
			CostMoneyConfirmFrame:SetClientUserData( 1, nCurSelectRepairItemIndex );
			CostMoneyConfirmFrame:SetClientUserData( 2, nCurSelectRepairItemID );
		end
		SetWarnFrame( "ShopRepairFrame" );
		CostMoneyConfirmFrame:Show();
		return;
	end

	if nRepairState == nRepairAll then --全部修理
		NpcShop:repairAllItem();
	elseif nRepairState == nRepairOne then --单件修理
		NpcShop:repairOneItem( nCurSelectRepairItemListType, nCurSelectRepairItemIndex, nCurSelectRepairItemID );
	end

	ShopRepairFrame:Hide();

	local animationTexture	= getglobal( "SRF_OKBtnUVAnimationTex" );
	if animationTexture:IsShown() then
		GameWizard:doStep( GAMEWIZARD_FIRST_BLINK_CONFIRM_REPAIR_BTN );
	end
	
	animationTexture:Hide();
end

--取消修理
function SRF_CancelBtn_OnClick()
	ShopRepairFrame:Hide();
end

function ShopExchangeConfirmFrame_OnShow()
end

function ShopExchangeConfirmFrame_OKBtn_OnClick()
	local nItemID	= ShopExchangeConfirmFrame:GetClientUserData( 0 );
	local nBuyNum	= ShopExchangeConfirmFrame:GetClientUserData( 1 );
	NpcShop:buyItem( nCurIndex, nBuyNum );
	ShopExchangeConfirmFrame:Hide();
end

function ShopExchangeConfirmFrame_CancelBtn_OnClick()
	ShopExchangeConfirmFrame:Hide();
	ShopExchangeConfirmFrame:SetClientUserData( 0, 0  );
	ShopExchangeConfirmFrame:SetClientUserData( 1, 0 )
end

function SBC_OKBtn_OnClick()
	local frame		= getglobal( this:GetParent() );
	local srcList	= frame:GetClientUserData( 0 );
	local srcGrid	= frame:GetClientUserData( 1 );
	local itemNum	= frame:GetClientUserData( 2 );
	NpcShop:sellItem( srcList, srcGrid, itemNum );
	playUISound( "sounds\\UI\\shoumai.wav" );
	frame:Hide();
	frame:SetClientUserData( 0, 0 );
	frame:SetClientUserData( 1, 0 );
	frame:SetClientUserData( 2, 0 );
end

function SBC_CancelBtn_OnClick()
	local frame		= getglobal( this:GetParent() );
	frame:Hide();
	frame:SetClientUserData( 0, 0 );
	frame:SetClientUserData( 1, 0 );
	frame:SetClientUserData( 2, 0 );
end

--确定丢掉
function SDF_OKBtn_OnClick()

end

--取消丢掉
function SDF_CancelBtn_OnClick()

end

function ShopSelectRepairFrame_EquipBtn_OnClick()
	ShopSelectRepairFrame:Hide();
end

function ShopSelectRepairFrame_AllBtn_OnClick()
	ShopSelectRepairFrame:Hide();
end

-- 折扣
local t_RebateTypeName = { [REBATE_TYPE_RED_NAME_TOLUA] = "红名、黑名价格",
							[REBATE_TYPE_CITY_OWNER_1_TOLUA] = "占领时间城折扣",
							[REBATE_TYPE_CITY_OWNER_2_TOLUA] = "占领时间城折扣",
							[REBATE_TYPE_CITY_OWNER_3_TOLUA] = "占领时间城折扣",
						}
function UpdateShopFrameRebate()
	local ShopList = NpcShop:getShopList();
	local szText	= "";
	if ShopList.RebateTypeNum == 0 then
		szText = "商店当前无折扣";
		ShopFrameRebateBtnFont:SetText( szText );
		ShopFrameRebateBtnFont:SetTextColor( 232, 226, 72 );
	elseif ShopList.RebateTypeNum == 1 and ShopList.RebateTypes[0] == REBATE_TYPE_RED_NAME_TOLUA then
		szText = "商店当前价格为" .. ShopList.RebateCount .."%( 红名、黑名价格 )";
		ShopFrameRebateBtnFont:SetText( szText );
		ShopFrameRebateBtnFont:SetTextColor( 255, 119, 184 );
	else
		szText = "商店当前价格为" .. ShopList.RebateCount .."%";
		if ShopList.RebateTypeNum > 0 then
			szText = szText .."(";
		end
		for i=1,ShopList.RebateTypeNum do
			szText = szText .. " " .. t_RebateTypeName[ ShopList.RebateTypes[i-1] ]
		end
		if ShopList.RebateTypeNum > 0 then
			szText = szText .." )";
		end
		ShopFrameRebateBtnFont:SetText( szText );
		ShopFrameRebateBtnFont:SetTextColor( 0,  220, 0 );
	end
	local width = ShopFrameRebateBtnFont:GetTextExtentWidth( szText );
	ShopFrameRebateBtn:SetSize( width,16 ) ;
end

function SetPriceTextColor( fontName , nMoneyType ) 
	local priceFont = getglobal( fontName );
	local ShopList	= NpcShop:getShopList();
	if not ( nMoneyType == ATTR_ID_MONEY or nMoneyType == ATTR_ID_BULL ) then 
		priceFont:SetTextColor( 232, 226, 72 );
		return;
	end
	if ShopList.RebateTypeNum == 0 then
		priceFont:SetTextColor( 232, 226, 72 );
	elseif ShopList.RebateTypeNum == 1 and ShopList.RebateTypes[0] == REBATE_TYPE_RED_NAME_TOLUA then
		priceFont:SetTextColor( 255, 119, 184 );
	else
		priceFont:SetTextColor( 0,  220, 0 );
	end
end

function ShopFrameRebateBtn_OnEnter()
	SetGameTooltips( this:GetName(), "红名、黑名价格和占领时间城折扣只对绑银、银币生效" );
end

function SetTaskAwardTexture( texName, nType )
	local tex = getglobal(texName);
	if tex == nil or t_BuyUseIcon[nType] == nil then 
		return;
	end
	tex:SetTexture( t_BuyUseIcon[nType]);
	tex:SetTexRelUV( 1, 1 );
	tex:SetSize(17,17);
end
--------------购买价格(银币绑银)超限二级提示------------------------
function ShopPriceWarnFrameOKBtnOnClick()
	local nCurIndex = ShopPriceWarnFrame:GetClientUserData(0)
	if nCurIndex == -1 then 
		ShopPriceWarnFrame:Hide();
		return
	end
	NpcShop:buyItem( nCurIndex, 1 );
	ShopPriceWarnFrame:Hide()
end
function ShopPriceWarnFrameCancelBtnOnClick()
	ShopPriceWarnFrame:Hide();
end

function ShopPriceWarnFrameOnLoad()
	ShopPriceWarnFrameTipRich:setCenterLine(true)
	ShopPriceWarnFrameConfirmRich:setCenterLine(true)	
end
------------------------------------------------