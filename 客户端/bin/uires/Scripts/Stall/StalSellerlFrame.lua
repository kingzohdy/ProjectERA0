-- 摆摊自身拥有的宠物列表
local t_StallSelfOwnPetListBtnEvents = 
{ 
	["GE_PET_STATE_CHANGE"] = {},	["GE_PET_INFO_CHG"] = {},	["GE_DEL_PET"] = {},
};

t_StallSelfOwnPetListBtnEvents["GE_PET_STATE_CHANGE"].func = 
function ( name )	
	local btn = getglobal( name );
	if btn:IsShown() then
		UpdateSelfOwnPetList( name );
	end
end

t_StallSelfOwnPetListBtnEvents["GE_PET_INFO_CHG"].func = 
function ( name )
	t_StallSelfOwnPetListBtnEvents["GE_PET_STATE_CHANGE"].func( name );
end

t_StallSelfOwnPetListBtnEvents["GE_DEL_PET"].func = 
function ( name )
	t_StallSelfOwnPetListBtnEvents["GE_PET_STATE_CHANGE"].func( name );
end

local t_StallSelfOwnPetListBtnControl = { grid = -1 };

-- 更新自身拥有的宠物列表
function ClearSelfOwnPetList( szFrameName )	
	for i = 1, CLIENT_STALL_MAX_PET do
		local btn		= getglobal( szFrameName.."PetBtn"..i );
		local font		= getglobal( btn:GetName().."NameFont" );
		local stateTex	= getglobal( btn:GetName().."StateTex" )
		local lovelevelbtn = getglobal( btn:GetName() .. "LoveLevelBtn" )
		local loveleveltex	= getglobal( btn:GetName() .. "LoveLevelTex" )
		btn:DisChecked();
		btn:Disable();
		font:SetText( "" );
		stateTex:Hide();
		lovelevelbtn:Hide()
		loveleveltex:Hide()
	end
end

function UpdateSelfOwnPetList( szFrameName )
	ClearSelfOwnPetList( szFrameName );

	local nBtnIndex		= 1;
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	-- 先验证原来选择的宠物是否还有效
	if t_StallSelfOwnPetListBtnControl["grid"] >= 0 then
		local preSelectPetInfo	= petPackage:getPetInfo( t_StallSelfOwnPetListBtnControl["grid"] );
		if preSelectPetInfo == nil then
			t_StallSelfOwnPetListBtnControl["grid"] = -1;
		end
	end
	
	if t_StallSelfOwnPetListBtnControl["grid"] == -1 and petPackage:getPetNum() > 0 then
		t_StallSelfOwnPetListBtnControl["grid"] = 0;
	end

	local nSelectGrid	= t_StallSelfOwnPetListBtnControl["grid"];

	for i = 1, petPackage:getPetNum() do
		local nGrid			= i - 1;
		local PetInfo		= petPackage:getPetInfo( i - 1 );
		if PetInfo.PetID ~= 0 then
			local PetDefInfo	= getPetDef( PetInfo.PetID );
			local szPetName		= PetInfo.Name;
			if szPetName == "" then
				szPetName = PetDefInfo.Name;
			end

			local petbtn		= getglobal( szFrameName .. "PetBtn" .. nBtnIndex );
			local petnamefont	= getglobal( petbtn:GetName().."NameFont" );
			local stateTex		= getglobal( petbtn:GetName().."StateTex" )
			local lovelevelbtn	= getglobal( petbtn:GetName() .. "LoveLevelBtn" )
			local loveleveltex	= getglobal( petbtn:GetName() .. "LoveLevelTex" )

			petbtn:Enable();
			petbtn:SetClientUserData( 1, nGrid );	 -- 宠物的栏位
			petnamefont:SetText( szPetName );
			-- 当宠物已经在交易栏或者摆摊面板上的时候，将宠物名字的颜色调为灰色
			if GameStallManager:isInStallIPet( SHITYPE, nGrid ) then
				petnamefont:SetTextColor( 131, 128, 108 );
			else
				petnamefont:SetTextColor( 180, 180, 90 );
			end

			if petPackage:isPetInState( nGrid, CLIENT_PET_STAT_FIGHT ) then
				stateTex:Show();
			else
				stateTex:Hide();
			end

			if nSelectGrid == nGrid then
				petbtn:Checked();
			end

			lovelevelbtn:SetClientUserData( 0, PetInfo.LoveLevel )
			lovelevelbtn:Show()
			local uv = t_petLoveLevelTextures[PetInfo.LoveLevel].UV
			loveleveltex:SetTexUV( uv.x, uv.y, uv.width, uv.height )
			loveleveltex:Show()

			nBtnIndex = nBtnIndex + 1;
		end
	end
end

function StallSelfOwnPetListBtn_OnLoad()
	-- 隐藏最后一个按钮的分割线
	local t_hideIcons = { this:GetName().."PetBtn8PartionTex", };
	util.HideIcons( t_hideIcons );

	for event, _ in pairs( t_StallSelfOwnPetListBtnEvents ) do
		this:RegisterEvent( event );
	end

	t_StallSelfOwnPetListBtnEvents.__index = function ( tab, key )
									tab[key] = { func = function () end };
									return tab[key];
								end
	setmetatable( t_StallSelfOwnPetListBtnEvents, t_StallSelfOwnPetListBtnEvents );
end

function StallSelfOwnPetListBtn_OnEvent()
	t_StallSelfOwnPetListBtnEvents[arg1].func( this:GetName() );
end

-- 卖家摆摊自身用宠物列表显示
function StallSelfOwnPetListBtn_OnShow()
	UpdateSelfOwnPetList( this:GetName() );
end

function StallSelfOwnPetListBtn_OnHide()
	t_StallSelfOwnPetListBtnControl["grid"] = -1;
end

-- 点击卖家摆摊自身用宠物按钮
function StallOwnPetButton_OnClick()
	local nGrid = this:GetClientUserData( 1 );
	t_StallSelfOwnPetListBtnControl["grid"] = nGrid;

	DisCheckAllBtn( this:GetParent().."PetBtn", CLIENT_STALL_MAX_PET );
	this:Checked();
end

-- 双击卖家摆摊自身用宠物按钮
function StallOwnPetButton_OnDoubleClick()
	local nGrid			= this:GetClientID( )-1;
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local PetInfo		= petPackage:getPetInfo( nGrid );
	ShowSelectPet( PetInfo, "StallSaleFrame" );

	t_StallSelfOwnPetListBtnControl["grid"] = nGrid;

	DisCheckAllBtn( this:GetParent().."PetBtn", CLIENT_STALL_MAX_PET );
	this:Checked();
end

-- 点击查看资质
function StallPetListBtn_ChaKanBtn_OnClick()
	local nGrid			= t_StallSelfOwnPetListBtnControl["grid"];
	if nGrid < 0 then
		return;
	end

	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local PetInfo		= petPackage:getPetInfo( nGrid );
	ShowSelectPet( PetInfo, "StallSaleFrame" );
end

-- 点击选择
function StallPetListBtn_SelectBtn_OnClick()
	local nGrid			= t_StallSelfOwnPetListBtnControl["grid"];
	if nGrid < 0 then
		return;
	end

	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local petInfo		= petPackage:getPetInfo( nGrid );
	if  petPackage:isPetInState( nGrid, CLIENT_PET_STAT_FIGHT ) then
		ShowMidTips( "宠物处于召唤状态,不能上架！" );
		return;
	elseif petPackage:isPetInState( nGrid, CLIENT_PET_STAT_DEAD ) then
		ShowMidTips( "宠物处于死亡状态,不能上架！" );
		return;
	elseif petInfo.LoveLevel >= 2 then
		ShowMidTips( "宠物与玩家不离不弃，不能上架" )
		return
	end

	ShangJiaPetToStallSaleFrame( nGrid );
end

-- 确认关闭摆摊面板
local t_StallSeller_CloseStallFrmaeConfirmFrame_MutexFrames = { "EndStallConfirmFrame" };
function StallSeller_CloseStallFrmaeConfirmFrame_OnShow()
	for _, name in ipairs( t_StallSeller_CloseStallFrmaeConfirmFrame_MutexFrames ) do
		local frame = getglobal( name );
		if frame:IsShown() then
			frame:Hide();
		end
	end
	this:SetPoint( "center", "StallSaleFrame", "center", 0, 0 );
end

-- 确认关闭摆摊面板
function StallSeller_CloseStallFrmaeConfirmFrame_Accept_OnClick()
	local t_hideIcons = { "StallSaleFrame", "StallSeller_CloseStallFrmaeConfirmFrame" };
	util.HideIcons( t_hideIcons );
end


-- 卖家上架物品标价面板
local t_StallSellerShangJiaItemFrameControl = { srcGrid = -1, srcList = -1, destGrid = -1 };
function GetStallSellerShangJiaItemFrameControl()
	return t_StallSellerShangJiaItemFrameControl;
end

t_StallSellerShangJiaItemFrameControl["clearItemInfo"] = 
function ( self )
	self["srcGrid"]		= -1;
	self["srcList"]		= -1;
	self["destGrid"]	= -1;
end

t_StallSellerShangJiaItemFrameControl["setItemInfo"] = 
function ( self, data )
	self["srcGrid"]		= data["srcGrid"];
	self["srcList"]		= data["srcList"];
	self["destGrid"]	= data["destGrid"];
end

-- 卖家上架物品标价面板显示
local t_ShangJiaItemFrameMutexFrame = { "StallSellerShangJiaPetFrame" };
function StallSellerShangJiaItemFrame_OnShow()
	for _, name in ipairs( t_ShangJiaItemFrameMutexFrame ) do
		local frame = getglobal( name );
		if frame:IsShown() then
			frame:Hide();
		end
	end

	local srcList		= t_StallSellerShangJiaItemFrameControl["srcList"];
	local srcGrid		= t_StallSellerShangJiaItemFrameControl["srcGrid"];
	local nStallGrid	= t_StallSellerShangJiaItemFrameControl["destGrid"];
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local item			= container:getItem( srcList, srcGrid );
	-- 如果物品添加成功则锁住该物品
	LockItem( srcList, srcGrid, item:getItemId(), "StallSaleFrame" ); 

	local itemBtn		= getglobal( this:GetName().."_ItemBtn" );
	itemBtn:SetClientUserData( 0, item:getItemId() );
	local itemDef	= item:getItemDef();	
	local t_tgas = { [itemBtn:GetName().."IconTexture"] = { path = util.getItemIconPath( { ["itemID"] = itemDef.ItemID } ) }, };
	util.SetTgas( t_tgas );
	
	local nDefNum	= item:getNum();
	local nDefPrice	= math.max( 1, item:getSellPrice() );
	local t_edites	= {	[this:GetName().."_ShowPriceEdit"]	= { value = nDefPrice }, 
						[this:GetName().."_ShowNumEdit"]	= { value = nDefNum },
						};
	util.SetEdites( t_edites );
	
	local t_itemNameColor = GetItemNameColor(itemDef);
	local t_fonts = {	[this:GetName().."_ItemNameBtnFont"]			= { value = itemDef.Name, 
																			color = {	r = t_itemNameColor[1], g = t_itemNameColor[2], 
																						b = t_itemNameColor[3] } }, 
						[this:GetName().."_TotalPriceBtnFont"]			= { value = nDefPrice * nDefNum, },
					};
	util.SetFonts( t_fonts );

	local priceEdit = getglobal( this:GetName().."_ShowPriceEdit" );
	SetFocusFrame( this:GetName().."_ShowPriceEdit" );
	priceEdit:SelectAllText();
	
	local timeBtn = getglobal( this:GetName().."_ShiXiaoTiShiBtn" );
	if item:getItemValidEndTime() > 0 then
		timeBtn:Show();
	else
		timeBtn:Hide();
	end

	this:SetPoint( "center", "$parent", "center", 0, 0  );
end

function StallSellerShangJiaItemFrame_OnUpdate()
	local font			= getglobal( this:GetName().."_TotalPriceBtnFont" );
	local numEdit		= getglobal( this:GetName().."_ShowNumEdit" );
	local nNum			= tonumber( numEdit:GetText() );
	local nTotalPrice	= 0;
	if nNum == nil or nNum == 0 then
		font:SetText( 0 );
		return;
	end

	local priceEdit	= getglobal( this:GetName().."_ShowPriceEdit" );
	local nPrice	= tonumber( priceEdit:GetText() );
	if nPrice == nil or nPrice == 0 then
		font:SetText( 0 );
		return;
	end

	font:SetText( nPrice * nNum );
end

-- 卖家上架物品标价面板隐藏
function ReleaseStallSellerShangJiaItemFrame_LockItem()
	local srcList		= t_StallSellerShangJiaItemFrameControl["srcList"];
	local srcGrid		= t_StallSellerShangJiaItemFrameControl["srcGrid"];
	if srcList == -1 or srcGrid == -1 then
		return;
	end

	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local item			= container:getItem( srcList, srcGrid );
	UnLockItem( srcList, srcGrid, item:getItemId() );
end

function StallSellerShangJiaItemFrame_OnHide()
	ReleaseStallSellerShangJiaItemFrame_LockItem();
	t_StallSellerShangJiaItemFrameControl:clearItemInfo();
end

-- 鼠标移到物品按钮
function StallSellerShangJiaItemOnEnter( szBtnName )
	local btn			= getglobal( szBtnName );
	local mainplayer	= ActorMgr:getMainPlayer();
	if mainplayer == nil then
		return;
	end
	local t_hideIcons = { "NormalItemTipsFrame", "ArmItemTipsFrame", "SecondArmItemTipsFrame", };
	util.HideIcons( t_hideIcons );

	local container		= mainplayer:getContainer();
	local id			= btn:GetClientUserData( 0 );
	local item			= container:getItem( id );
	if item:getItemId() <= 0 then
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, getItemDef( id ) );
	else
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item, StallSellerShangJiaItemOnEnter, szBtnName);
	end
end

function StallSellerShangJiaItemFrame_ItemBtn_OnEnter()
	local id = this:GetClientUserData( 0 );
	if id == 0 then
		return;
	end

	StallSellerShangJiaItemOnEnter( this:GetName() );
end

function StallSellerShangJiaItemFrame_ItemBtn_OnLeave()
	local t_hideIcons = { "NormalItemTipsFrame", "ArmItemTipsFrame", "SecondArmItemTipsFrame", "GameTooltip", "SpellTipsFrame" };
	util.HideIcons( t_hideIcons );
end

-- 更新数量输入框
function StallSellerShangJiaItemFrame_ShowNumEdit_OnUpdate()
	local nEditNum		= tonumber( this:GetText() );
	if nEditNum == nil then
		return;
	end

	local srcList		= t_StallSellerShangJiaItemFrameControl["srcList"];
	local srcGrid		= t_StallSellerShangJiaItemFrameControl["srcGrid"];
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local item			= container:getItem( srcList, srcGrid );
	
	local nMaxNum		= item:getNum();
	if nEditNum > nMaxNum then
		this:SetText( nMaxNum )
	end
end

-- 点击卖家上架物品标价面板的减少数量按钮
function StallSellerShangJiaItemFrame_DelNumBtn_OnClick()
	local numEdit		= getglobal( this:GetParent().."_ShowNumEdit" );
	local nEditNum		= tonumber( numEdit:GetText() );
	if nEditNum == nil or nEditNum <= 1 then
		return;
	end

	numEdit:SetText( nEditNum - 1 )
end

-- 点击卖家上架物品标价面板的增加数量按钮
function StallSellerShangJiaItemFrame_AddNumBtn_OnClick()
	local numEdit = getglobal( this:GetParent().."_ShowNumEdit" );
	if numEdit:GetText() == "" then
		numEdit:SetText( 1 );
		return;
	end

	local srcList		= t_StallSellerShangJiaItemFrameControl["srcList"];
	local srcGrid		= t_StallSellerShangJiaItemFrameControl["srcGrid"];
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local item			= container:getItem( srcList, srcGrid );
	
	local nEditNum		= tonumber( numEdit:GetText() );
	local nMaxNum		= item:getNum();
	if nEditNum < nMaxNum then
		numEdit:SetText( nEditNum + 1 )
	end
end

-- 点击卖家上架物品标价面板的确定按钮
function StallSellerShangJiaItemFrame_ConfirmBtn_OnClick()
	local numEdit	= getglobal( this:GetParent().."_ShowNumEdit" );
	local nNum		= tonumber( numEdit:GetText() );
	if nNum == nil or nNum == 0 then
		ShowMidTips( "请重新输入数量" );
		return;
	end

	local priceEdit	= getglobal( this:GetParent().."_ShowPriceEdit" );
	local nPrice	= tonumber( priceEdit:GetText() );
	if nPrice == nil or nPrice == 0 then
		ShowMidTips( "出售单价不能空缺" );
		return;
	end
	nPrice = math.min( nPrice, GetMaxStallSellPrice() );
	local frame			= getglobal( this:GetParent() );

	local srcList		= t_StallSellerShangJiaItemFrameControl["srcList"];
	local srcGrid		= t_StallSellerShangJiaItemFrameControl["srcGrid"];
	if srcList == -1 or srcGrid == -1 then
		return;
	end

	local nStallGrid	= t_StallSellerShangJiaItemFrameControl["destGrid"];
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local item			= container:getItem( srcList, srcGrid );
	nNum = math.min( nNum, item:getNum() );
	
	if mainplayer:isInStall() then
		GameStallManager:addStallItemInStall( nStallGrid, srcList, srcGrid, nNum, nPrice );
		frame:Hide();
	else	
		GameStallManager:addStallItem( nStallGrid, srcList, srcGrid, nNum, nPrice );
		frame:Hide();
		LockItem( srcList, srcGrid, item:getItemId(), "StallSaleFrame" );
		UpdateStallSaleFrame();
	end
end

-- 卖家上架宠物标价面板
local t_StallSellerShangJiaPetFrameControl = { heightId = 0, lowId = 0, id = 0, index = -1 };
function GetStallSellerShangJiaPetFrameControl()
	return t_StallSellerShangJiaPetFrameControl;
end

t_StallSellerShangJiaPetFrameControl["clearPetInfo"] = 
function ( self )
	self["heightId"]	= 0;
	self["lowId"]		= 0;
	self["id"]			= 0;
	self["index"]		= 0;
end

t_StallSellerShangJiaPetFrameControl["setPetInfo"] = 
function ( self, data )
	self["heightId"]	= data["heightId"];
	self["lowId"]		= data["lowId"];
	self["id"]			= data["id"];
	self["index"]		= data["index"];
end

-- 显示卖家标价宠物框
local t_ShangJiaPetFrameMutexFrame = { "StallSellerShangJiaItemFrame" };
function StallSellerShangJiaPetFrame_OnShow()
	for _, name in ipairs( t_ShangJiaPetFrameMutexFrame ) do
		local frame = getglobal( name );
		if frame:IsShown() then
			frame:Hide();
		end
	end
	
	local petDef = getPetDef( t_StallSellerShangJiaPetFrameControl["id"] );
	local t_tgas = { [this:GetName().."_PetPhotoBtnIconTexture"] = { path = GetMonsterIconPath( petDef.IconID ) }, };
	util.SetTgas( t_tgas );
	
	local t_edites = { ["StallSellerShangJiaPetFrame_ShowPriceEdit"] = { value = "" }, };
	util.SetEdites( t_edites );
	
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local petInfo		= petPackage:getPetInfo( t_StallSellerShangJiaPetFrameControl["heightId"], t_StallSellerShangJiaPetFrameControl["lowId"] );
	local szPetName		= petInfo.Name;
	local petDef		= getPetDef( petInfo.PetID );
	if szPetName == "" then
		szPetName = petDef.Name;
	end

	local t_fonts = {	[this:GetName().."_PetNameBtnFont"]			= { value = szPetName, color = GetPetGeniusColor( petInfo ) }, 
					};
	util.SetFonts( t_fonts );
	SetFocusFrame( "StallSellerShangJiaPetFrame_ShowPriceEdit" );

	this:SetPoint( "center", "$parent", "center", 0, 0  );
end

-- 隐藏卖家标价宠物框
function StallSellerShangJiaPetFrame_OnHide()
	t_StallSellerShangJiaPetFrameControl:clearPetInfo();
end

-- 点击确认按钮
function StallSellerShangJiaPetFrame_ConfirmBtn_OnClick()
	local priceEdit = getglobal( "StallSellerShangJiaPetFrame_ShowPriceEdit" );
	local nPrice	= tonumber( priceEdit:GetText() );
	if nPrice == nil or nPrice == 0 then
		ShowMidTips( "出售单价不能空缺" );
		return;
	end

	nPrice = math.min( nPrice, GetMaxStallSellPrice() );
	local frame			= getglobal( "StallSellerShangJiaPetFrame" );
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local nHeightID		= t_StallSellerShangJiaPetFrameControl["heightId"];
	local nLowID		= t_StallSellerShangJiaPetFrameControl["lowId"];
	if nHeightID == 0 and nLowID == 0 then
		return;
	end

	local petInfo		= petPackage:getPetInfo( nHeightID, t_StallSellerShangJiaPetFrameControl["lowId"] );
	if petInfo == nil then
		ShowMidTips( "宠物已被放生，这次操作无效" );
		frame:Hide();
		return;
	end

	local nStallGrid = GetStallPetGridIndex();
	if mainplayer:isInStall() then	
		GameStallManager:addStallPetInStall( nStallGrid, SHITYPE, t_StallSellerShangJiaPetFrameControl["index"], nPrice );
	else
		GameStallManager:addStallPet( nStallGrid, SHITYPE, t_StallSellerShangJiaPetFrameControl["index"], nPrice );
		UpdateStallSaleFrame();
	end
	frame:Hide();
end

function GetMaxStallSellPrice()
	return CLIENT_MAX_MONEY;
end

-- 价格输入框
function StallSellerShangJiaFrame_ShowPriceEdit_OnUpdate()
	if this:GetText() == "" then
		return;
	end

	CheckEditNumberInput( this );
	CheckMaxPriceNum( this, GetMaxStallSellPrice() )
end

function StallSellerShangJiaItemFrame_ShowPriceEdit_OnTabPressed()
	local focusEdit = getglobal( this:GetParent().."_ShowNumEdit" );
	SetFocusFrame( focusEdit:GetName() );
	focusEdit:SelectAllText();
end

function StallSellerShangJiaItemFrame_ShowNumEdit_OnTabPressed()
	local focusEdit = getglobal( this:GetParent().."_ShowPriceEdit" );
	SetFocusFrame( focusEdit:GetName() );
	focusEdit:SelectAllText();
end

function StallOwnPetButtonLoveLevelBtn_OnEnter()
	local level = this:GetClientUserData( 0 )
	ShowGameTooltip( { rich = "WideGameTooltipRichText", tipsFrame = "WideGameTooltip", text = tLoveLevelDesc[level], frame = this:GetParent(),
						button = this:GetName() } );
	local parent = getglobal( this:GetParent() )
	parent:Highlight()
end

function StallOwnPetButtonLoveLevelBtn_OnLeave()
	local parent = getglobal( this:GetParent() )
	if not isPointInFrame( parent:GetName() ) then
		parent:DisHighlight()
	end
	WideGameTooltip:Hide()
end