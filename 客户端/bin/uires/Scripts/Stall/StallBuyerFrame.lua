local t_StallBuyerBuyItemFrameControl = { grid = -1 };

function GetStallBuyerBuyItemFrameControl()
	return t_StallBuyerBuyItemFrameControl;
end

t_StallBuyerBuyItemFrameControl["setItemInfo"] = 
function ( self, data )
	self["grid"] = data["grid"]; 
end

t_StallBuyerBuyItemFrameControl["clearItemInfo"] = 
function ( self )
	self["grid"]		= -1;
end

-- 买家购买物品的输入框
local t_StallBuyerBuyItemFrameMutexFrame = { "StallBuyerBuyPetFrame" };
function StallBuyerBuyItemFrame_OnShow()
	for _, name in ipairs( t_StallBuyerBuyItemFrameMutexFrame ) do
		local frame = getglobal( name );
		if frame:IsShown() then
			frame:Hide();
		end
	end
	
	local nGrid			= t_StallBuyerBuyItemFrameControl["grid"];
	local seestalldata	= GameStallManager:getSeeStallItem();
	local item			= seestalldata.m_ItemData[nGrid];
	local itemBtn		= getglobal( this:GetName().."_ItemBtn" );
	itemBtn:SetClientUserData( 0, item:getItemId() );
	local itemDef		= item:getItemDef();	
	local t_tgas		= { [itemBtn:GetName().."IconTexture"] = { path = util.getItemIconPath( { ["itemID"] = itemDef.ItemID } ) }, };
	util.SetTgas( t_tgas );
	
	local nPrice			= item:getStallSellPrice();
	local nDefNum			= 1; 
	local t_itemNameColor	= GetItemNameColor(itemDef);
	local t_fonts = {	[this:GetName().."_ItemNameBtnFont"]			= { value = itemDef.Name, 
																			color = {	r = t_itemNameColor[1], g = t_itemNameColor[2], 
																						b = t_itemNameColor[3] } }, 
						[this:GetName().."_TotalPriceBtnFont"]			= { value = nPrice * nDefNum, },
						[this:GetName().."_ShowPriceBtnFont"]			= { value = nPrice, },
					};
	util.SetFonts( t_fonts );

	local t_edites	= {	
						[this:GetName().."_ShowNumEdit"]	= { value = nDefNum },
						};
	util.SetEdites( t_edites );

	local numEdit = getglobal( this:GetName().."_ShowNumEdit" );
	SetFocusFrame( numEdit:GetName() );
	numEdit:SelectAllText();
	
	local timeBtn = getglobal( this:GetName().."_ShiXiaoTiShiBtn" );
	if item:getItemValidEndTime() > 0 then
		timeBtn:Show();
	else
		timeBtn:Hide();
	end
	this:SetPoint( "topleft", "$parent", "topleft", 74, 363  );
end

function StallBuyerBuyItemFrame_OnUpdate()
	local font		= getglobal( this:GetName().."_TotalPriceBtnFont" );
	local numEdit	= getglobal( this:GetName().."_ShowNumEdit" );
	local nNum		= tonumber( numEdit:GetText() );
	if nNum == nil or nNum == 0 then
		font:SetText( 0 );
		return;
	end

	local nGrid			= t_StallBuyerBuyItemFrameControl["grid"];
	local seestalldata	= GameStallManager:getSeeStallItem();
	local item			= seestalldata.m_ItemData[nGrid];
	local nPrice		= item:getStallSellPrice();
	font:SetText( nPrice * nNum );
end

-- 买家购买物品的输入框减少购买数量
function StallBuyerBuyItemFrame_DelNumBtn_OnClick()
	local numEdit	= getglobal( this:GetParent().."_ShowNumEdit" );
	local nNum		= tonumber( numEdit:GetText() );
	if nNum == nil or nNum <= 1 then
		return;
	end
	
	numEdit:SetText( nNum - 1 );
end

-- 买家购买物品的输入框增加购买数量
function StallBuyerBuyItemFrameMaxCanBuyNum()
	local nGrid			= t_StallBuyerBuyItemFrameControl["grid"];
	local seestalldata	= GameStallManager:getSeeStallItem();
	local item			= seestalldata.m_ItemData[nGrid];
	local nSaleNum		= item:getNum();
	
	local mainplayer	= ActorMgr:getMainPlayer();
	local nMoney		= mainplayer:getMoney();
	local nCanBuyNum	= math.ceil( mainplayer:getMoney() / item:getStallSellPrice() );

	return math.min( nSaleNum, nCanBuyNum );
end

function StallBuyerBuyItemFrame_AddNumBtn_OnClick()
	local numEdit	= getglobal( this:GetParent().."_ShowNumEdit" );
	local nNum		= tonumber( numEdit:GetText() );
	if nNum == nil then
		return;
	end

	local nMaxCanBuyNum	= StallBuyerBuyItemFrameMaxCanBuyNum();

	if nMaxCanBuyNum == 0 then
		ShowMidTips( "金钱不足，无法购买商品" );
		return;
	end

	if nNum >= nMaxCanBuyNum then
		return;
	end

	numEdit:SetText( nNum + 1 );
end

-- 输入数量更新
function StallBuyerBuyItemFrame_ShowNumEdit_OnUpdate()
	local nNum		= tonumber( this:GetText() );
	if nNum == nil then
		return;
	end

	local nMaxCanBuyNum	= StallBuyerBuyItemFrameMaxCanBuyNum();
	if nNum > nMaxCanBuyNum then
		this:SetText( nMaxCanBuyNum );
	end
end

-- 确定购买商品
function StallBuyerBuyItemFrame_ConfirmBtn_OnClick()
	local nGrid			= t_StallBuyerBuyItemFrameControl["grid"];
	if nGrid == -1 then
		return;
	end

	local numEdit		= getglobal( this:GetParent().."_ShowNumEdit" );
	local nEditNum		= tonumber( numEdit:GetText() );
	if nEditNum == nil or nEditNum == 0 then
		ShowMidTips( "请输入需要购买的数量" );
		return;
	end

	local seestalldata	= GameStallManager:getSeeStallItem();
	local item		= seestalldata.m_ItemData[nGrid];
	
	local mainplayer	= ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local nCanBuyNum	= math.floor( mainplayer:getMoney() / item:getStallSellPrice() );
	local nMaxCanBuyNum	= StallBuyerBuyItemFrameMaxCanBuyNum();
	if nMaxCanBuyNum == 1 and nCanBuyNum == 0 then
		ShowMidTips( "银币不足，无法购买商品" );
		ShowBuyMoneyFrame();
		return;
	end

	GameStallManager:stall_Buy( CLIENT_STALL_TYPE_ITEM, nGrid, math.min( nEditNum, nMaxCanBuyNum ), seestalldata.m_szRoleName );
	StallBuyerBuyItemFrame:Hide();
end

-- 买家购买物品的输入框关闭
function StallBuyerBuyItemFrame_OnHide()
	t_StallBuyerBuyItemFrameControl:clearItemInfo();
end

-- 买家输入要购买的宠物数据
local t_StallBuyerBuyPetFrameControl = { grid = -1 };

function GetStallBuyerBuyPetFrameControl()
	return t_StallBuyerBuyPetFrameControl;
end

t_StallBuyerBuyPetFrameControl["setPetInfo"] = 
function ( self, data )
	self["grid"] = data["grid"]; 
end

t_StallBuyerBuyPetFrameControl["clearPetInfo"] = 
function ( self )
	self["grid"]		= -1;
end

local t_StallBuyerBuyPetFrameMutexFrame = { "StallBuyerBuyItemFrame" };
function StallBuyerBuyPetFrame_OnShow()
	for _, name in ipairs( t_StallBuyerBuyPetFrameMutexFrame ) do
		local frame = getglobal( name );
		if frame:IsShown() then
			frame:Hide();
		end
	end
	
	local nGrid			= t_StallBuyerBuyPetFrameControl["grid"];
	local seestalldata	= GameStallManager:getSeeStallItem();
	local selectPetData	= seestalldata.m_PetData[nGrid];
	local petInfo		= selectPetData.m_RolePet;
	local petDef		= getPetDef( petInfo.PetID );
	local t_tgas = { [this:GetName().."_PetPhotoBtnIconTexture"] = { path = GetMonsterIconPath( petDef.IconID ) }, };
	util.SetTgas( t_tgas );
	
	local szPetName		= petInfo.Name;
	if szPetName == "" then
		szPetName = petDef.Name;
	end
	
	local t_fonts = {	[this:GetName().."_PetNameBtnFont"]			= { value = szPetName, color = GetPetGeniusColor( petInfo ) }, 
						[this:GetName().."_ShowPriceBtnFont"]		= { value = selectPetData.m_uPrice }, 
					};
	util.SetFonts( t_fonts );
	this:SetPoint( "topleft", "$parent", "topleft", 74, 363  );
end

function StallBuyerBuyPetFrame_OnHide()
	t_StallBuyerBuyPetFrameControl:clearPetInfo();
end

function StallBuyerBuyPetFrame_ConfirmBtn_OnClick()
	local nGrid			= t_StallBuyerBuyPetFrameControl["grid"];
	if nGrid == -1 then
		return;
	end

	local seestalldata	= GameStallManager:getSeeStallItem();
	local selectPetData	= seestalldata.m_PetData[nGrid];
	
	local mainplayer	= ActorMgr:getMainPlayer();
	local nMoney		= mainplayer:getMoney();
	if nMoney < selectPetData.m_uPrice then
		ShowMidTips( "银币不足，无法购买该宠物" );
		ShowBuyMoneyFrame();
		return;
	end

	GameStallManager:stall_Buy( CLIENT_STALL_TYPE_PET, nGrid, 1, seestalldata.m_szRoleName );
	StallBuyerBuyPetFrame:Hide();
end

-- 买主购买摆摊物品主面板
local t_StallBuyFrameControl = {	item	= { grid		= -1,	list = -1, selectIndex = 0, id = 0 }, 
									pet		= { heightId	= 0,	lowId = 0, selectIndex = 0, id = 0 },
									stallPageType = CLIENT_STALL_TYPE_ITEM };

function GetStallBuyFrameControl()
	return t_StallBuyFrameControl;
end

t_StallBuyFrameControl["setSelectItem"] =
function ( self, data )
	self["item"]["selectIndex"] = data["selectIndex"];
	self["item"]["id"]			= data["id"];
	self["item"]["grid"]		= data["grid"];
	self["item"]["list"]		= data["list"];
end

t_StallBuyFrameControl["clearSelectItem"] =
function ( self )
	self["item"]["selectIndex"] = 0;
	self["item"]["id"]			= 0;
	self["item"]["grid"]		= -1;
	self["item"]["list"]		= -1;
end

t_StallBuyFrameControl["setSelectPet"] =
function ( self, data )
	self["pet"]["selectIndex"]	= data["selectIndex"];
	self["pet"]["heightId"]		= data["heightId"];
	self["pet"]["lowId"]		= data["lowId"];
end

t_StallBuyFrameControl["clearSelectPet"] =
function ( self )
	self["pet"]["selectIndex"]	= 0;
	self["pet"]["heightId"]		= 0;
	self["pet"]["lowId"]		= 0;
end

-- 如果是买家左键点击售卖的商品面板
function StallBuyLeftBtnClick( szBtnName )
	local btn			= getglobal( szBtnName );
	local seeStallInfo	= GameStallManager:getSeeStallItem();
	local nGrid			= btn:GetClientID();
	local item			= seeStallInfo.m_ItemData[nGrid-1];
	if item:getItemId() == 0 then
		return;
	end

	t_StallBuyFrameControl:setSelectItem( { grid = item:getGridIdx(), list = item:getListType(), selectIndex = nGrid, id = item:getItemId() } );
	DisCheckAllBtn( "StallBuyFrameItem", CLIENT_STALL_MAX_ITEM_PAGE );
	btn:Checked();

	-- 是否是使用shitf产生物品物品超链接
	GameStallManager:getItemLink( 1, nGrid - 1 );

	UpdateStallBuyFrameSelectItemPrice();
end

-- 如果是买家右键点击售卖的商品面板
function StallBuyRightBtnClick( szBtnName )
	local btn			= getglobal( szBtnName );
	local nGrid			= btn:GetClientID()-1;
	local seeStallInfo	= GameStallManager:getSeeStallItem();
	local item			= seeStallInfo.m_ItemData[nGrid];

	if item:getItemId() == 0 then
		return;
	end
	
	local t_StallBuyerBuyItemFrameControl = GetStallBuyerBuyItemFrameControl();
	t_StallBuyerBuyItemFrameControl:setItemInfo( { ["grid"] = nGrid, } );
	StallBuyerBuyItemFrame:Show();
end

-- CORE: 所有更新函数
-- 1.
function UpdateStallBuyFrameSelectItemPrice()
	if t_StallBuyFrameControl["stallPageType"] == CLIENT_STALL_TYPE_ITEM then
		Private_UpdateStallBuyPriceByItem();
	elseif t_StallBuyFrameControl["stallPageType"] == CLIENT_STALL_TYPE_PET then
		Private_UpdateStallBuyPriceByPet();
	end
end

-- 2.
function UpdateStallBuyFrame()
	if t_StallBuyFrameControl["stallPageType"] == CLIENT_STALL_TYPE_ITEM then
		Private_UpdateStallBuyItemFrame();
	elseif t_StallBuyFrameControl["stallPageType"] == CLIENT_STALL_TYPE_PET then
		Private_UpdateStallBuyPetFrame();
	end
end

-- 3. 
function UpdateStallBuyFrameUI()
	-- 若当前要显示道具子面板
	if t_StallBuyFrameControl["stallPageType"] == CLIENT_STALL_TYPE_ITEM then
		Private_ShowStallBuyFrameItemUI();
	-- 若当前要显示宠物子面板
	elseif t_StallBuyFrameControl["stallPageType"] == CLIENT_STALL_TYPE_PET then
		Private_ShowStallBuyFramePetUI();
	end
end

-- 4.
function UpdateStallBuyFrameCheckLabel()
	local t_labelBtn = { "StallBuyFrame_ItemLabelBtn", "StallBuyFrame_PetLabelBtn" };
	for _, name in ipairs( t_labelBtn ) do
		local btn = getglobal( name );
		btn:DisChecked();
	end
	local seeStallInfo	= GameStallManager:getSeeStallItem();
	--local btn	=	getglobal("StallBuyFrame_PetLabelBtn");
	if seeStallInfo.m_nPetNum ~=	0 then
		StallBuyFrame_PetLabelBtn:Checked();
	end
	if t_StallBuyFrameControl["stallPageType"]	== CLIENT_STALL_TYPE_ITEM then
		StallBuyFrame_ItemLabelBtn:Checked();
	elseif t_StallBuyFrameControl["stallPageType"]	== CLIENT_STALL_TYPE_PET then
		StallBuyFrame_PetLabelBtn:Checked();
	end
end

-- CORE: 所有更新函数

-- 用选定的物品更新单价
function Private_UpdateStallBuyPriceByItem()
	local nSelectIndex	= t_StallBuyFrameControl["item"]["selectIndex"];
	local priceFont		= getglobal( "StallBuyFrame_ShowPriceBtnFont" );
	if nSelectIndex == 0 then
		priceFont:SetText( 0 );
		return;
	end

	local seeStallInfo	= GameStallManager:getSeeStallItem();
	local item			= seeStallInfo.m_ItemData[nSelectIndex-1];
	local nPrice		= item:getStallSellPrice();
	priceFont:SetText( nPrice );
end

-- 用选定的宠物更新单价
function Private_UpdateStallBuyPriceByPet()
	local nSelectIndex	= t_StallBuyFrameControl["pet"]["selectIndex"];
	local priceFont		= getglobal( "StallBuyFrame_ShowPriceBtnFont" );
	if nSelectIndex == 0 then
		priceFont:SetText( 0 );
		return;
	end

	local seeStallInfo	= GameStallManager:getSeeStallItem();
	local stallPetData	= seeStallInfo.m_PetData[nSelectIndex-1];
	priceFont:SetText( stallPetData.m_uPrice );
end

-- 刷新买家摆摊面板的道具子面板
function Private_UpdateStallBuyItemFrame()
	DisCheckAllBtn( "StallBuyFrameItem", CLIENT_STALL_MAX_ITEM_PAGE );
	local seeStallInfo		= GameStallManager:getSeeStallItem();
	local t_selectItemInfo	= t_StallBuyFrameControl["item"];
	local nSelectItemIndex	= t_selectItemInfo["selectIndex"];
	for i = 1, CLIENT_STALL_MAX_ITEM_PAGE do
		local item		= seeStallInfo.m_ItemData[i-1];
		local stallbtn	= getglobal( "StallBuyFrameItem"..i );
		stallbtn:SetClientUserData( 0, DRAG_STALLBUY );
		stallbtn:SetClientUserData( 1, i - 1 );
		local ItemTex			= getglobal( stallbtn:GetName().."IconTexture" );
		local CountFont			= getglobal( stallbtn:GetName().."CountFont" );
		local UVAnimationTex	= getglobal( stallbtn:GetName().."UVAnimationTex" );
		local nItemId			= item:getItemId();
		if nItemId ~= 0 then
			local itemdef	= item:getItemDef();
			local IconPath	= GetItemEquipIconPath()..itemdef.IconID..".tga";
			-- 如果没找到物品的图标则使用默认图标显示
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			ItemTex:SetTexture( IconPath );
			SetEquipImportantEffectUV( itemdef, UVAnimationTex:GetName());
			if item:getNum() > 1 then
				CountFont:SetText( item:getNum() );
			else
				CountFont:SetText( "" );
			end
			stallbtn:SetClientUserData( 2, item:getNum() );
			-- check选定的按钮
			if i == nSelectItemIndex then
				stallbtn:Checked();
			end
		else
			ItemTex:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
			CountFont:SetText( "" );
			UVAnimationTex:Hide();
		end
	end
	
	
	if t_selectItemInfo["selectIndex"] > 0 then
		local item		= seeStallInfo.m_ItemData[nSelectItemIndex-1];
		if item:getItemId() == 0 then
			t_StallBuyFrameControl:clearSelectItem();
		end
	end
	
	-- 更新价格框
	UpdateStallBuyFrameSelectItemPrice();
end

-- 刷新买家摆摊面板的宠物子面板
function Private_UpdateStallBuyPetFrame()
	DisCheckAllBtn( "StallBuyFramePet", CLIENT_STALL_MAX_PET );
	local seeStallInfo		= GameStallManager:getSeeStallItem();
	local nCurLine			= 1;
	local t_selPetInfo		= t_StallBuyFrameControl["pet"];
	local nSelectPetIndex	= t_selPetInfo["selectIndex"];
	for i = 1, CLIENT_STALL_MAX_PET do
		local oneSeePetData		= seeStallInfo.m_PetData[i-1];
		local petInfo			= oneSeePetData.m_RolePet;
		if petInfo.PetID ~= 0 then
			local petBtn		= getglobal( "StallBuyFramePet"..nCurLine );
			local szPetName		= petInfo.Name;
			local PetDefInfo	= getPetDef( petInfo.PetID );
			if szPetName == "" then
				szPetName = PetDefInfo.Name;
			end
			-- color = GetPetGeniusColor( petInfo )
			local t_fonts = {	[petBtn:GetName().."Name"]				= { value = szPetName }, 
								[petBtn:GetName().."MoneyCountFont"]	= {	value = oneSeePetData.m_uPrice, },
								[petBtn:GetName().."Level"]				= {	value = petInfo.Level.."级", },
								[petBtn:GetName().."Type"]				= {	value = GetPetGeniusDesc( petInfo ), 
																			 },
							};
			util.SetFonts( t_fonts );
			
			local moneyTex		= getglobal( petBtn:GetName().."MoneyIconTex" );
			moneyTex:Show();
			petBtn:Enable();
			
			local petWID = petInfo.WID;
			if petWID.nHeight == t_selPetInfo["heightId"] and petWID.nLow == t_selPetInfo["lowId"] then
				petBtn:Checked();
			end
			

			local lovelevelbtn = getglobal( petBtn:GetName() .. "LoveLevelBtn" )
			local loveleveltex = getglobal( petBtn:GetName() .. "LoveLevelBtnTex" )

			local lovelevel = petInfo.LoveLevel+1
			local MAX_LOVE_LEVEL = 2
			if lovelevel > MAX_LOVE_LEVEL then
				lovelevel = MAX_LOVE_LEVEL
			end

			lovelevelbtn:SetClientUserData( 0, lovelevel )
			lovelevelbtn:Show()

			local uv = t_petLoveLevelTextures[lovelevel].UV
			loveleveltex:SetTexUV( uv.x, uv.y, uv.width, uv.height )

			petBtn:SetClientUserData( 0, oneSeePetData.m_nStallGridIdx );
			petBtn:SetClientUserData( 1, oneSeePetData.m_nListType );
			nCurLine = nCurLine + 1;
		end
	end

	if nSelectPetIndex > 0  then
		local oneSeePetData	= seeStallInfo.m_PetData[nSelectPetIndex-1];
		local petInfo			= oneSeePetData.m_RolePet;
		if petInfo.PetID == 0 then
			t_StallBuyFrameControl:clearSelectPet();
		end
	end

	for i = nCurLine, CLIENT_STALL_MAX_PET do
		local petBtn		= getglobal( "StallBuyFramePet"..i );
		
		local moneyTex		= getglobal( petBtn:GetName().."MoneyIconTex" );
		moneyTex:Hide();

		local t_fonts = {	[petBtn:GetName().."Name"]				= { value = "" }, 
							[petBtn:GetName().."MoneyCountFont"]	= {	value = "", },
							[petBtn:GetName().."Level"]				= {	value = "", },
							[petBtn:GetName().."Type"]				= {	value = "", },
							};
		util.SetFonts( t_fonts );
		
		local lovelevelbtn = getglobal( petBtn:GetName() .. "LoveLevelBtn" )
		lovelevelbtn:Hide()

		petBtn:SetClientUserData( 0, -1 );
		petBtn:SetClientUserData( 1, -1 );
		petBtn:Disable();
	end
end

-- 刷新摆摊面板上面显示的按钮
-- 显示买家摆摊面板的道具子面板
function Private_ShowStallBuyFrameItemUI()
	for i = 1, CLIENT_STALL_MAX_ITEM_PAGE do
		local itemframe = getglobal( "StallBuyFrameItem"..i );
		itemframe:Show();
	end

	for i = 1, CLIENT_STALL_MAX_PET do
		local petframe = getglobal( "StallBuyFramePet"..i );
		petframe:Hide();
	end

	local t_hideIcons = { "StallBuyFrame_SeePetInfoBtn"  };
	util.HideIcons( t_hideIcons );

	local t_showIcons = { "StallBuyFrame_ShowPriceBackBtn", "StallBuyFrame_ShowPriceHeadBtn", "StallBuyFrame_ShowPriceBtn", "StallBuyFrame_BuyBtn" };
	util.ShowIcons( t_showIcons );

	StallBuyFrame_BuyBtn:SetPoint( "bottom", "StallBuyFrame", "bottom", 0, -4 );
end

-- 显示买家摆摊面板的宠物子面板
function Private_ShowStallBuyFramePetUI()
	for i = 1, CLIENT_STALL_MAX_ITEM_PAGE do
		local itemframe = getglobal( "StallBuyFrameItem"..i );
		itemframe:Hide();
	end

	for i = 1, CLIENT_STALL_MAX_PET do
		local petframe = getglobal( "StallBuyFramePet"..i );
		petframe:Show();
	end

	local t_hideIcons = { "StallBuyFrame_ShowPriceBackBtn", "StallBuyFrame_ShowPriceHeadBtn", "StallBuyFrame_ShowPriceBtn"  };
	util.HideIcons( t_hideIcons );

	local t_showIcons = { "StallBuyFrame_SeePetInfoBtn", "StallBuyFrame_BuyBtn" };
	util.ShowIcons( t_showIcons );

	StallBuyFrame_BuyBtn:SetPoint( "bottomright", "StallBuyFrame", "bottomright", -29, -4 );
	StallBuyFrame_SeePetInfoBtn:SetPoint( "bottomleft", "StallBuyFrame", "bottomleft", 29, -4 );
end

-- 购买摊位物品面板
local t_StallBuyFrameEvents = 
{ 
	["GE_STALL_SEE"] = {},	["GE_STALL_BUYER_CHG"] = {},	["GE_STALL_BUY_END"] = {},	["GE_MAIN_START_PK"] = {},	["GE_MAIN_DEAD"] = {},
	["GE_ACTOR_FIGHT_IN"] = {},
};

t_StallBuyFrameEvents["GE_STALL_BUYER_CHG"].func = 
function ()
	UpdateStallBuyFrame();
end

t_StallBuyFrameEvents["GE_STALL_SEE"].func = 
function ()
	if TradeFrame:IsShown() then
		ShowMidTips( "正处于交易状态，无法查看别人的摊位信息！" );
		return;
	end

	if not GameStallManager:canSeeStall() then
		return;
	end

	StallBuyFrame:Show();
	local seeStallInfo	= GameStallManager:getSeeStallItem();
	--local btn	=	getglobal("StallBuyFrame_PetLabelBtn");
	if seeStallInfo.m_nPetNum ~=	0 then
		StallBuyFrame_PetLabelBtn:Checked();
	else
		StallBuyFrame_PetLabelBtn:DisChecked();
	end

	local actor = ActorMgr:findActorByName( seeStallInfo.m_szRoleName );
	ChangeSeenStallNameColor( actor:GetID() );
end

t_StallBuyFrameEvents["GE_STALL_BUY_END"].func = 
function ()
	StallBuyFrame:Hide();
end

t_StallBuyFrameEvents["GE_MAIN_START_PK"].func = 
function ()
	if StallBuyFrame:IsShown() then
		ShowMidTips( "进入切磋状态，不能查看他人摊位" );
		StallBuyFrame:Hide();
	end
end

t_StallBuyFrameEvents["GE_MAIN_DEAD"].func = 
function ()
	if StallBuyFrame:IsShown() then
		ShowMidTips( "进入死亡状态，不能查看他人摊位" );
		StallBuyFrame:Hide();
	end
end

t_StallBuyFrameEvents["GE_ACTOR_FIGHT_IN"].func = 
function ()
	if StallBuyFrame:IsShown() then
		ShowMidTips( "进入战斗状态，不能查看他人摊位" );
		StallBuyFrame:Hide();
	end
end

function StallBuyFrame_OnLoad()	
	for i = 1, CLIENT_STALL_MAX_ITEM_PAGE do
		local buybtn	= getglobal( "StallBuyFrameItem"..i.."BackTexture" );
		buybtn:Show();
	end

	local t_hideIcons = { this:GetName().."Pet8PartionTex", };
	util.HideIcons( t_hideIcons );

	for event, _ in pairs( t_StallBuyFrameEvents ) do
		this:RegisterEvent( event );
	end

	t_StallBuyFrameEvents.__index = function ( tab, key )
									tab[key] = { func = function () end };
									return tab[key];
								end
	setmetatable( t_StallBuyFrameEvents, t_StallBuyFrameEvents );
end

function StallBuyFrame_OnEvent()
	t_StallBuyFrameEvents[arg1].func();
end

function Private_UpdateStallBuyFrameSelectPageType_OnShow()
	local seestallitem = GameStallManager:getSeeStallItem();
	if seestallitem.m_nItemNum == 0 then	
		t_StallBuyFrameControl["stallPageType"]	= CLIENT_STALL_TYPE_PET;
	elseif seestallitem.m_nPetNum == 0 then
		t_StallBuyFrameControl["stallPageType"]	= CLIENT_STALL_TYPE_ITEM;
	else
		t_StallBuyFrameControl["stallPageType"]	= CLIENT_STALL_TYPE_ITEM;
	end
end

function StallBuyFrame_OnShow()
	Private_UpdateStallBuyFrameSelectPageType_OnShow();
	UpdateStallBuyFrameCheckLabel();
	UpdateStallBuyFrameSelectItemPrice();
	UpdateBiaoJiAnNiu();

	local seestallitem = GameStallManager:getSeeStallItem();
	StallBuyFrame_StallOwnerNameRich:SetText( "#L" .. seestallitem.m_szRoleName .. "#n", 174, 221, 159 );
	StallBuyFrame_StallOwnerNameRich:SetLinkTextColor( 174, 221, 159 );
	
	UpdateStallBuyFrameUI();
	UpdateStallBuyFrame();

	-- 设置买方面板上的摊位名字
	local t_fonts = {	["StallBuyFrame_StallNameBtnFont"]	= { value = seestallitem.m_szStallName },
					};
	util.SetFonts( t_fonts );
	
--	this:SetPoint( "center", "$parent", "center", 0, 0  );
	StallBuyFrame:SetPoint( "topleft", "$parent", "topleft", 32, 163  );
end

function StallBuyFrame_OnHide()
	DisCheckAllBtn( "StallBuyFramePet", CLIENT_STALL_MAX_PET );
	GameStallManager:resetSeeStallInfo();

	local t_hideIcons = { "StallBuyerBuyItemFrame", "StallBuyerBuyPetFrame", };
	util.HideIcons( t_hideIcons );

	t_StallBuyFrameControl:clearSelectPet();
	t_StallBuyFrameControl:clearSelectItem();
	
	ClearStallNameBack();
end

-- 点击摊主名字超链接
function SellerNameRichText_OnClick()
	-- 只有买家才能看到超链接
	if string.find( this:GetName(), "StallBuyFrame" )== nil then
		return;
	end
	local seestallitem = GameStallManager:getSeeStallItem();

	if arg3 == "RightButton" then
		local mainplayer = ActorMgr:getMainPlayer()
		if mainplayer == nil then return end
		ShowCharacterRightFrame( seestallitem.m_szRoleName );
	elseif arg3 == "LeftButton" then
		SendPrivateMessage( seestallitem.m_szRoleName );
	end
end

-- 买家摆摊面板道具标签按钮
function StallBuyFrame_ItemLabelBtn_OnClick()
	if t_StallBuyFrameControl["stallPageType"] ==  CLIENT_STALL_TYPE_ITEM then
		return;
	end
	t_StallBuyFrameControl["stallPageType"]	= CLIENT_STALL_TYPE_ITEM;

	UpdateStallBuyFrameUI();
	UpdateStallBuyFrameCheckLabel();
	UpdateStallBuyFrame();
end

-- 买家摆摊面板宠物标签按钮
function StallBuyFrame_PetLabelBtn_OnClick()
	if t_StallBuyFrameControl["stallPageType"] ==  CLIENT_STALL_TYPE_PET then
		return;
	end
	local seeStallInfo		= GameStallManager:getSeeStallItem();
	local btn	=	getglobal("StallBuyFrame_PetLabelBtn");
	if seeStallInfo.m_nPetNum ==	0 then
		ShowMidTips( "该摊位没有出售宠物" );
		return;
	end

	t_StallBuyFrameControl["stallPageType"]	= CLIENT_STALL_TYPE_PET;
	
	UpdateStallBuyFrameUI();
	UpdateStallBuyFrameCheckLabel();
	UpdateStallBuyFrame();
end

-- 购买摆摊物品
-- 购买道具
function Private_StallBuyFrame_BuyItem()
	local nSelectIndex	= t_StallBuyFrameControl["item"]["selectIndex"];
	if nSelectIndex == 0 then
		return;
	end
	
	local t_StallBuyerBuyItemFrameControl = GetStallBuyerBuyItemFrameControl();
	t_StallBuyerBuyItemFrameControl:setItemInfo( { ["grid"] = nSelectIndex - 1, } );
	StallBuyerBuyItemFrame:Show();
end

-- 购买宠物
function Private_StallBuyFrame_BuyPet()
	local nSelectIndex	= t_StallBuyFrameControl["pet"]["selectIndex"];
	if nSelectIndex == 0 then
		return;
	end
	
	local mainplayer	= ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local petPackage	= mainplayer:getPet();

	if petPackage:isPetFull( ) then
		ShowMidTips( "没有空余栏位存放该宠物，购买失败" );
		return;
	end

	t_StallBuyerBuyPetFrameControl:setPetInfo( { ["grid"] = nSelectIndex - 1, } );
	StallBuyerBuyPetFrame:Show();
end

function UpdateBiaoJiAnNiu()
	local t_StallBuyFrameBiaoJiStalls = GetStallBuyFrameBiaoJiStalls();
	local seeStallInfo		= GameStallManager:getSeeStallItem();
	local roleName			= seeStallInfo.m_szRoleName;
	if t_StallBuyFrameBiaoJiStalls:isInBiaoJi( { ["name"] = roleName } ) then
		StallBuyFrame_CancelBiaoJiBtn:Show();
		StallBuyFrame_BiaoJiBtn:Hide();
	else
		StallBuyFrame_CancelBiaoJiBtn:Hide();
		StallBuyFrame_BiaoJiBtn:Show()
	end
end

-- [roleName] = { x = , y = , startTime = 0 },
MAX_STALL_BIAO_JI_NUM	= 20;
BIAO_JI_STALL_MAP		= 1000;
local t_StallBuyFrameBiaoJiStalls = { trackList = {} };
function GetStallBuyFrameBiaoJiStalls()
	return t_StallBuyFrameBiaoJiStalls;
end

t_StallBuyFrameBiaoJiStalls["clearAllBiaoJi"] = 
function ( self )
	for name, _ in pairs( self["trackList"] ) do
		self["trackList"][name] = nil;
	end
end

t_StallBuyFrameBiaoJiStalls["getEarliestBiaoJiRoleName"] = 
function ( self )
	local t_earliestRole = { startTime = 0, name = "" };
	for name, data in pairs( self["trackList"] ) do
		t_earliestRole["name"]		= name;
		t_earliestRole["startTime"]	= data["startTime"];
		break;
	end

	for name, data in pairs( self["trackList"] ) do
		if data["startTime"] < t_earliestRole["startTime"] then
			t_earliestRole["name"]		= name;
			t_earliestRole["startTime"]	= data["startTime"];
		end
	end

	if t_earliestRole["name"] == "" then
		return;
	end

	return t_earliestRole["name"];
end

t_StallBuyFrameBiaoJiStalls["getBiaoJiNum"] = 
function ( self )
	local nCount = 0;
	for _, _ in pairs( self["trackList"] ) do
		nCount = nCount + 1;
	end

	return nCount;
end

t_StallBuyFrameBiaoJiStalls["isInBiaoJi"] = 
function ( self, data )
	local name = data["name"];
	return self["trackList"][name] ~= nil;
end

t_StallBuyFrameBiaoJiStalls["addBiaoJi"] = 
function ( self, data )
	local nCurBiaoJiCount = self:getBiaoJiNum();
	if nCurBiaoJiCount == MAX_STALL_BIAO_JI_NUM then
		local earlistRoleName = self:getEarliestBiaoJiRoleName();
		self:cancelBiaoJi( { ["name"] = earlistRoleName } );
	end
	
	local name = data["name"];
	self["trackList"][name] = { x = data["x"], y = data["y"], startTime = GameMgr:getTickTime(), ["mapID"] = data["mapID"] };
end

t_StallBuyFrameBiaoJiStalls["cancelBiaoJi"] = 
function ( self, data )
	local name = data["name"];
	self["trackList"][name] = nil;

	local actor	= ActorMgr:findActorByName( name );
	if actor ~= nil then
		actor:stopMotion( "9622_16" );
	end
end

-- 点击标记摊位按钮
function StallBuyFrame_BiaoJiBtn_OnClick()
	local seeStallInfo		= GameStallManager:getSeeStallItem();
	local roleName			= seeStallInfo.m_szRoleName;
	local actor				= ActorMgr:findActorByName( roleName )
	t_StallBuyFrameBiaoJiStalls:addBiaoJi( { name = roleName, x = actor:getPlayerX() / 10, y = actor:getPlayerZ() / 10,
												mapID = MapShower:getRealMapId() } );
	UpdateBiaoJiAnNiu();
	actor:playMotion( "9622_16" );
end

-- 点击取消标记按钮
function StallBuyFrame_CancelBiaoJiBtn_OnClick()
	local seeStallInfo		= GameStallManager:getSeeStallItem();
	local roleName			= seeStallInfo.m_szRoleName;
	local actor				= ActorMgr:findActorByName( roleName )
	t_StallBuyFrameBiaoJiStalls:cancelBiaoJi( { name = roleName, } );
	UpdateBiaoJiAnNiu();
end

function CallScript_ActorEndStall( name )
	local t_StallBuyFrameBiaoJiStalls = GetStallBuyFrameBiaoJiStalls();
	t_StallBuyFrameBiaoJiStalls:cancelBiaoJi( { ["name"] = name, } );
end

-- 点击购买按钮
function StallBuyFrame_BuyBtn_OnClick()
	-- 若没有选中任何物品,则不做任何操作
	if t_StallBuyFrameControl["stallPageType"] == CLIENT_STALL_TYPE_ITEM then
		Private_StallBuyFrame_BuyItem();
	elseif t_StallBuyFrameControl["stallPageType"] == CLIENT_STALL_TYPE_PET then
		Private_StallBuyFrame_BuyPet();
	end
end

-- 点击买家摆摊面板的宠物列表按钮
function Private_StallBuyerBuyItemFramePetItem_OnClick( petBtn )
	local nPetStallGrid		= petBtn:GetClientUserData( 0 ) + 1;
	local seeStallInfo		= GameStallManager:getSeeStallItem();
	local oneSeePetData		= seeStallInfo.m_PetData[nPetStallGrid-1];
	local petInfo			= oneSeePetData.m_RolePet;
	local petWID			= petInfo.WID;
	t_StallBuyFrameControl:setSelectPet( { selectIndex = nPetStallGrid, heightId = petWID.nHeight, lowId = petWID.nLow } );

	DisCheckAllBtn( "StallBuyFramePet", CLIENT_STALL_MAX_PET );
	petBtn:Checked();
	UpdateStallBuyFrameSelectItemPrice();
end

-- 双击买家摆摊面板的宠物列表按钮
function Private_StallBuyerBuyItemFramePetItem_OnDoubleClick( petBtn )
	local nPetStallGrid		= petBtn:GetClientUserData( 0 ) + 1;
	local seeStallInfo		= GameStallManager:getSeeStallItem();
	local oneSeePetData		= seeStallInfo.m_PetData[nPetStallGrid-1];
	local petInfo			= oneSeePetData.m_RolePet;
	local petWID			= petInfo.WID;
	ShowSelectPet( petInfo, "StallBuyFrame" );

	t_StallBuyFrameControl:setSelectPet( { selectIndex = nPetStallGrid, heightId = petWID.nHeight, lowId = petWID.nLow } );

	DisCheckAllBtn( "StallBuyFramePet", CLIENT_STALL_MAX_PET );
	petBtn:Checked();
	UpdateStallBuyFrameSelectItemPrice();
end

-- 查看宠物资料
function StallBuyFrame_SeePetInfoBtn_OnClick()
	local nSelectIndex	= t_StallBuyFrameControl["pet"]["selectIndex"];
	if nSelectIndex <= 0 then
		ShowMidTips( "请选中要查看的宠物" );
		return;
	end

	local seestalldata	= GameStallManager:getSeeStallItem();
	local mainplayer	= ActorMgr:getMainPlayer();
	local nMoney		= mainplayer:getMoney();
	local petInfo		= seestalldata.m_PetData[nSelectIndex-1].m_RolePet;
	ShowSelectPet( petInfo, this:GetParent() );
end

function StallBuyFrame_SeePetInfoBtn_OnEnter()
	local szText = "双击宠物按钮可以快速查看宠物资料";
	
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, 
						frame =	this:GetParent(), button = this:GetName() } );
end

function StallBuyerBuyItemFrame_ItemBtn_OnEnter()
	local nGrid			= t_StallBuyerBuyItemFrameControl["grid"];
	local seestalldata	= GameStallManager:getSeeStallItem();
	local item			= seestalldata.m_ItemData[nGrid];
	if item:getItemId() > 0 then
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item );
	end
end