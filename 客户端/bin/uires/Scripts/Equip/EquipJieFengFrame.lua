local t_EquiJieFengControl = {};
local szMoneyName = "金币"

t_EquiJieFengControl["init"] =
function ( self )
	t_EquiJieFengControl["equipInfo"] = {}
	t_EquiJieFengControl:clearArmInfo();
end

t_EquiJieFengControl["clearArmInfo"] =
function ( self )
	t_EquiJieFengControl:removeUnFengyinItem();
end
t_EquiJieFengControl["canAddUnFengyinItem"] =
function ( self, data )
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local item			= container:getItem( data["list"], data["grid"] );
	local itemDef		= item:getItemDef();

	if not item:isArmFengYin() then
		ShowMidTips( "该物品不需要解封" );
		return false;
	end

	if ConfirmJieFengFrame:IsShown() then
		ShowMidTips( "请先完成确认操作" );
		return false;
	end
	
	return true;
end

t_EquiJieFengControl["setUnFengyinItem"] =
function ( self, data )
	local list			= data["list"];
	local grid			= data["grid"];
	local t_curEquipInfo= self["equipInfo"];
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local item			= container:getItem( list, grid );
	if t_curEquipInfo["list"] ~= NONE_LIST_TYPE then
		self:removeUnFengyinItem();
	end
	
	self["equipInfo"]["list"] = list;
	self["equipInfo"]["grid"] = grid
	LockItem( item:getListType(), item:getGridIdx(), item:getItemId(), "EquipJieFengFrame" );
	--print( "setBaoShi--LockItem--grid = "..item:getGridIdx().."， listType = "..item:getListType().."--202" );
end

t_EquiJieFengControl["removeUnFengyinItem"] =
function ( self, data )
	local t_curEquipInfo= self["equipInfo"];
	local list			= t_curEquipInfo["list"];
	local grid			= t_curEquipInfo["grid"];
	if list == NONE_LIST_TYPE or list == nil then
		return;
	end

	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local item			= container:getItem( list, grid );
	UnLockItem( list, grid, item:getItemId() );
	self["equipInfo"]["list"] = -1;
	self["equipInfo"]["grid"] = -1
end

function StartUnFengYinOp( nNpcId )
	EquipJieFengFrame:Show();
	if not PlayerContainerFrame:IsShown() then
		PlayerContainerFrame:Show();
	end
	PlayerContainerFrame:SetPoint( "topright", "$parent", "topright",-86, 163 );
	EquipJieFengFrame:SetPointAlwaysRecalc("topright", "PlayerContainerFrame", "topleft",0 ,0 );
	--util.testNpcDialogDistance( nNpcId, EquipJieFengFrame );
end

local UN_FENG_YIN_SUNCCES	= 1;
local UN_FENG_YIN_FAIL		= 0;

function ArmUnFengYinResult( nFlag )
	if nFlag == UN_FENG_YIN_SUNCCES then
		ShowMidTips( "解除装备封印成功" );
		
		local t_curEquipInfo= t_EquiJieFengControl["equipInfo"];
		local list			= t_curEquipInfo["list"];
		local grid			= t_curEquipInfo["grid"];
		if list == NONE_LIST_TYPE then
			return;
		end
		
		local mainplayer	= ActorMgr:getMainPlayer();
		local container		= mainplayer:getContainer();
		local item			= container:getItem( list, grid );
		if item:isArmFengYin() then
			return;
		end

		t_EquiJieFengControl:clearArmInfo();
		UpdateEquipJieFengFrame();
	elseif nFlag == UN_FENG_YIN_FAIL then
		ShowMidTips( "解除装备封印失败" );
	end
end

function Save2_EquipJieFengFrame( item )
	if not t_EquiJieFengControl:canAddUnFengyinItem( { grid = item:getGridIdx(), list = item:getListType(), showMsg = true } ) then
		return; 
	end

	local itemDef		= item:getItemDef();
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	
	t_EquiJieFengControl:setUnFengyinItem( { grid = item:getGridIdx(), list = item:getListType(), } );

	UpdateEquipJieFengFrame();
end

function CleaEquipJieFengFrame()
	local MainPlayer = ActorMgr:getMainPlayer();
	local money 
	if szMoneyName == "金币" then
		money = MainPlayer:getGodCoin()
	else
		money = MainPlayer:getBindGodCoin()
	end
	local t_fonts = {	["EquipJieFengFrameNeedMoneyFont"] = { value = "0" },["EquipJieFengFrameCurrMoneyFont"] = { value = money }};
	util.SetFonts( t_fonts );

	local btn = getglobal( "EquipJieFengFrame_EquipItemBtn" );
	btn:SetClientUserData( 0, 0 );
	local t_tgas = { [btn:GetName().."IconTexture"] = { path = "uires\\ui\\mask\\TouMingTongDao.tga" }, };
	util.SetTgas( t_tgas );

	EquipJieFengFrame_JieFengBtn:Disable();
end

function UpdateEquipJieFengFrame()
	CleaEquipJieFengFrame();

	local t_curEquipInfo= t_EquiJieFengControl["equipInfo"];
	local list			= t_curEquipInfo["list"];
	local grid			= t_curEquipInfo["grid"];
	if list == NONE_LIST_TYPE or list == nil then
		return;
	end

	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local item			= container:getItem( list, grid );
	local itemDef		= item:getItemDef();

	local btn = getglobal( "EquipJieFengFrame_EquipItemBtn" );
	btn:SetClientUserData( 0, itemDef.ItemID );
	

	local t_tgas = { [btn:GetName().."IconTexture"] = { path = util.getItemIconPath( { ["itemID"] = itemDef.ItemID } ) }, };
	util.SetTgas( t_tgas );

	local t_fonts = {	["EquipJieFengFrameNeedMoneyFont"] = { value = item:getUnFengYinArmMoney() }, };
	util.SetFonts( t_fonts );

	EquipJieFengFrame_JieFengBtn:Enable();
end

function ChangeCostMoneyType( nType )
	if nType == 1 then
		szMoneyName = "金券"
		EquipJieFengFrameMoneyBtn1:ChangeNormalTexture( 57,494,20,18 )
		EquipJieFengFrameMoneyBtn2:ChangeNormalTexture( 57,494,20,18 )
		EquipJieFengFrameMoneyBtn1:SetSize( 20,18 )
		EquipJieFengFrameMoneyBtn2:SetSize( 20,18 )
		EquipJieFengFrameMoneyBtn1:SetClientUserData( 0,2 )
		EquipJieFengFrameMoneyBtn2:SetClientUserData( 0,2 )
	else
		szMoneyName = "金币"
		EquipJieFengFrameMoneyBtn1:ChangeNormalTexture( 99,494,17,18 )
		EquipJieFengFrameMoneyBtn2:ChangeNormalTexture( 99,494,17,18 )
		EquipJieFengFrameMoneyBtn1:SetSize( 17,18 )
		EquipJieFengFrameMoneyBtn2:SetSize( 17,18 )
		EquipJieFengFrameMoneyBtn1:SetClientUserData( 0,1 )
		EquipJieFengFrameMoneyBtn2:SetClientUserData( 0,1 )
	end
	EquipJieFengFrameNeedMoneyTitle:SetText( "需要" .. szMoneyName )
	EquipJieFengFrameCurrMoneyTitle:SetText( "现有" .. szMoneyName )

	EquipJieFengFrameDescRich:SetText( "●\t绑定装备掉落后，会进入封印状态，封印的装备将不会带来任何属性加成。\n",170,160,120 );
	EquipJieFengFrameDescRich:AddText( "●\t解封需要消耗一定的" .. szMoneyName ,170,160,120 );
	EquipJieFengFrameDescRich:AddText( "●\t红、黑名状态更容易使装备进入封印状态，需谨慎。",170,160,120 );
end

function EquipJieFengFrame_OnLoad()
	EquipJieFengFrameDescRich:resizeRect( 250,90 );
	EquipJieFengFrameDescRich:SetText( "●\t绑定装备掉落后，会进入封印状态，封印的装备将不会带来任何属性加成。\n",170,160,120 );
	EquipJieFengFrameDescRich:AddText( "●\t解封需要消耗一定的金币。",170,160,120 );
	EquipJieFengFrameDescRich:AddText( "●\t红、黑名状态更容易使装备进入封印状态，需谨慎。",170,160,120 );
	this:RegisterEvent( "UI_ACTOR_ATTRIBUTE_CHG" );
	t_EquiJieFengControl:init()
end

function EquipJieFengFrame_OnEvent()
	if arg1 == "UI_ACTOR_ATTRIBUTE_CHG" then
		if EquipJieFengFrame:IsShown() then
			local mainplayer = ActorMgr:getMainPlayer();
			local money 
			if szMoneyName == "金币" then
				money = mainplayer:getGodCoin()
			else
				money = mainplayer:getBindGodCoin()
			end
			EquipJieFengFrameCurrMoneyFont:SetText( money );
		end
	end
end

function EquipJieFengFrame_OnShow()
	UpdateEquipJieFengFrame();
	if not PlayerContainerFrame:IsShown() then
		PlayerContainerFrame:Show();
	end
	this:SetPoint( "topright", "PlayerContainerFrame", "topleft", 0 , 0 );
	if not PlayerContainerFrame:IsShown() then
		PlayerContainerFrame:Show();
	end	
end

function EquipJieFengFrame_OnHide()
	t_EquiJieFengControl:clearArmInfo();
	t_EquiJieFengControl:init();
	
	local t_hideIcons = { "ConfirmJieFengFrame", };
	util.HideIcons( t_hideIcons );
end

function EquipJieFengFrame_EquipItemBtn_OnEnter()
	local id = this:GetClientUserData( 0 );
	if id == 0 then
		return;
	end
	
	EquipJieFengOnEnter( this:GetName() );
end

function EquipJieFengFrame_EquipItemBtn_OnLeave()
	local t_hideIcons = { "NormalItemTipsFrame", "ArmItemTipsFrame", "SecondArmItemTipsFrame", "GameTooltip", "SpellTipsFrame" };
	util.HideIcons( t_hideIcons );
end

function EquipJieFengFrame_EquipItemBtn_ReceiveDrag()
	local srcList = UIMSG:getUIGV().DragFrom;
	if srcList ~= CONTAINER_TYPE_SUNDRIES and srcList ~= CONTAINER_TYPE_STUFF then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	end

	local srcGrid		= UIGetDragUserData1();
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local item			= container:getItem( srcList, srcGrid );
	Save2_EquipJieFengFrame( item );
	UIEndDrag();
	UIMSG:getUIGV().DragFrom = DRAG_NONE;
end

function EquipJieFengFrame_EquipItemBtn_OnClick()
	if arg1 ~= "RightButton" then
		return;
	end
	
	local id = this:GetClientUserData( 0 );
	if id == 0 then
		return;
	end

	t_EquiJieFengControl:removeUnFengyinItem();
	UpdateEquipJieFengFrame();
end

function EquipJieFengOnEnter( szBtnName )
	local t_curEquipInfo 	= t_EquiJieFengControl["equipInfo"];
	local nGrid 			= t_curEquipInfo["grid"]
	local nType				= t_curEquipInfo["list"]
	if not t_EquiJieFengControl:canAddUnFengyinItem( { grid = nGrid, list = nType, showMsg = true } ) then
		return; 
	end
	
	local btn			= getglobal( szBtnName );
	local mainplayer	= ActorMgr:getMainPlayer();
	if mainplayer == nil then
		return;
	end
	local t_hideIcons = { "NormalItemTipsFrame", "ArmItemTipsFrame", "SecondArmItemTipsFrame", };
	util.HideIcons( t_hideIcons );

	local container		= mainplayer:getContainer();
	local id			= btn:GetClientUserData( 0 );
	local item			= container:getItem( nType,nGrid);
	if item:getItemId() <= 0 then
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, getItemDef( id ) );
	else
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item, EquipJieFengOnEnter, szBtnName);
	end
end

-- 解封按钮
function EquipJieFengFrame_JieFengBtn_OnClick()
	if szMoneyName == "金币" then
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer == nil or mainplayer:getGodCoin() < tonumber( EquipJieFengFrameNeedMoneyFont:GetText() ) then
			ExchangePointFrame:SetClientUserData(0,1);
			MarketManager:requestQueryTianyuan();
			return;
		end
	end
	local t_curEquipInfo				= t_EquiJieFengControl["equipInfo"];
	local t_ConfirmJieFengFrameControl	= GetConfirmJieFengFrameControl();
	t_ConfirmJieFengFrameControl:setUnFengyinItem( { grid = t_curEquipInfo["grid"], list = t_curEquipInfo["list"], } );
	GameArmProce:requireFengYinPlayerInfo( t_curEquipInfo["list"], t_curEquipInfo["grid"] );
end

local t_ConfirmJieFengFrameControl = { equipInfo = { list = -1, grid = -1 }, playInfo = { name = "" } };

function GetConfirmJieFengFrameControl()
	return t_ConfirmJieFengFrameControl;
end

t_ConfirmJieFengFrameControl["init"] =
function ( self, data )
	self["playInfo"]	= { name = "" };
	self["equipInfo"]	= { list = -1, grid = -1 };
end

t_ConfirmJieFengFrameControl["setUnFengyinItem"] =
function ( self, data )
	local list	= data["list"];
	local grid	= data["grid"];
	
	self["equipInfo"]["list"] = list;
	self["equipInfo"]["grid"] = grid
end

t_ConfirmJieFengFrameControl["setFengyinPlayerInfo"] = 
function ( self, data )
	self["playInfo"]["name"] = data["name"];
end

function GetFengYinEquiPlayerInfo( szRoleName )
	t_ConfirmJieFengFrameControl:setFengyinPlayerInfo( { ["name"] = szRoleName } );
	ConfirmJieFengFrame:Show();
end

function UpdateConfirmJieFengFrame()
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local t_curEquipInfo= t_ConfirmJieFengFrameControl["equipInfo"];
	local list			= t_curEquipInfo["list"];
	local grid			= t_curEquipInfo["grid"];
	local item			= container:getItem( list, grid );
	local itemDef		= item:getItemDef();
	
	local t_riches = 
		{ ["ConfirmJieFengFrame_DescRich"] = { 
			value = "该装备被"..t_ConfirmJieFengFrameControl["playInfo"]["name"].."击败封印，解封该装备需要消耗"..szMoneyName..
					item:getUnFengYinArmMoney().."点。是否进行解封？", 
			color = { r = 255, g = 184, b = 0 } }, 
		};
	util.SetRiches( t_riches );
end

function ConfirmJieFengFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", -0, 0 );
	UpdateConfirmJieFengFrame();
end

function ConfirmJieFengFrame_ConfirmButton_OnClick()
	local t_curEquipInfo= t_ConfirmJieFengFrameControl["equipInfo"];
	GameArmProce:requireUnFengYin( t_curEquipInfo["list"], t_curEquipInfo["grid"] );
	ConfirmJieFengFrame:Hide();
end

function ConfirmJieFengFrame_OnHide()
	t_ConfirmJieFengFrameControl:init();
end

