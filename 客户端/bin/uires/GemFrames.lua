GEM_RED		= 0;
GEM_BLUE	= 1;
GEM_GREEN	= 2;
GEM_COLOR	= 3;
-- ��ʯ����
local t_PrepareOpenHoleFrameControl = { holeIdx = -1, equipInfo = { grid = -1, list = -1 } };

local t_openHoleSuccessGaiLv = 
{
[0] = CLIENT_GEM_HOLE1_RANDOM,	[1] = CLIENT_GEM_HOLE2_RANDOM,
[2] = CLIENT_GEM_HOLE3_RANDOM,	[3] = CLIENT_GEM_HOLE4_RANDOM,
};
function GetPrepareOpenHoleFrameControl()
	return t_PrepareOpenHoleFrameControl;
end

t_PrepareOpenHoleFrameControl["clearOpenHoleInfo"] =
function ( self )
	self["holeIdx"]				= -1;
	self["equipInfo"]["grid"]	= -1;
	self["equipInfo"]["list"]	= -1;
end

t_PrepareOpenHoleFrameControl["setOpenHoleInfo"] =
function ( self, data )
	self["holeIdx"]				= data["holeIdx"];
	self["equipInfo"]["grid"]	= data["grid"];
	self["equipInfo"]["list"]	= data["list"];
end

t_PrepareOpenHoleFrameControl["isCurOpenHoleEquip"] =
function ( self, data )
	if self["equipInfo"]["grid"] < 0 then
		return false;
	end

	return self["equipInfo"]["list"] == data["list"] and self["equipInfo"]["grid"] == data["grid"];
end

t_PrepareOpenHoleFrameControl["getOpenHoleEquipItem"] =
function ( self )
	local list			= self["equipInfo"]["list"];
	local grid			= self["equipInfo"]["grid"];
	local mainplayer	= ActorMgr:getMainPlayer();
	if list == CONTAINER_TYPE_SUNDRIES then
		local container	= mainplayer:getContainer();
		equipItem		= container:getItem( list, grid );
	elseif list == CONTAINER_TYPE_WEAR then
		local Eqiup	= mainplayer:getEquip();
		equipItem = Eqiup:getItem( grid );
	end

	return equipItem;
end

t_PrepareOpenHoleFrameControl["startOpenHole"] =
function ( self, data )
	local frame = getglobal( "PrepareOpenHoleFrame" );
	self:setOpenHoleInfo( data );
	if frame:IsShown() then
		UpdatePrepareOpenHoleFrame();
	else
		frame:Show();
	end
end

function UpdatePrepareOpenHoleFrame()
	local mainplayer	= ActorMgr:getMainPlayer();
	local list			= t_PrepareOpenHoleFrameControl["equipInfo"]["list"];
	local grid			= t_PrepareOpenHoleFrameControl["equipInfo"]["grid"];
	local equipItem		= t_PrepareOpenHoleFrameControl:getOpenHoleEquipItem();

	local itemDef = getItemDef( equipItem:getItemId() );
	local t_fonts = {	["PrepareOpenHoleFrameBlackButtonSuccessGaiLVFont"]	= 
						{ value = "�ɹ��ʣ�"..t_openHoleSuccessGaiLv[t_PrepareOpenHoleFrameControl["holeIdx"]].."%" }, 
						["PrepareOpenHoleFrameBullCostBtnFont"]		= { value = itemDef.WearPre.GemOpenHoleMoney }, 
						["PrepareOpenHoleFrameNowHaveBullBtnFont"]	= { value = mainplayer:getMoney() }, 
						["PrepareOpenHoleFrameBlackButtonDescFont"]	= { value = "�Ƿ񻨷����ҿ�����ʯ�ף�" }, 
					};
	util.SetFonts( t_fonts );
end

local t_PrepareOpenHoleFrameEvents = { ["GE_CHANGE_MONEY"] = {}, };

t_PrepareOpenHoleFrameEvents["GE_CHANGE_MONEY"].func = 
function ()
	local mainplayer		= ActorMgr:getMainPlayer();
	local t_fonts = {	
						["PrepareOpenHoleFrameNowHaveBullBtnFont"]	= { value = mainplayer:getMoney() }, 
					};
	util.SetFonts( t_fonts );
end

function PrepareOpenHoleFrame_OnLoad()
	for event, _ in pairs( t_PrepareOpenHoleFrameEvents ) do
		this:RegisterEvent( event );
	end

	t_PrepareOpenHoleFrameEvents.__index = function ( tab, key )
										tab[key] = { func = function () end };
										return tab[key];
									end
	setmetatable( t_PrepareOpenHoleFrameEvents, t_PrepareOpenHoleFrameEvents );

end

function PrepareOpenHoleFrame_OnEvent()
	t_PrepareOpenHoleFrameEvents[arg1].func();
end

function PrepareOpenHoleFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 0, 0 );
	UpdatePrepareOpenHoleFrame();
	PrepareOpenHoleFrameOpenHoleButton:Show();
	PrepareOpenHoleFrameConfirmButton:Hide();
end

function PrepareOpenHoleFrame_OnHide()
	t_PrepareOpenHoleFrameControl:clearOpenHoleInfo();
end

-- ����
function PrepareOpenHoleFrameOpenHoleButton_OnClick()
	local mainplayer		= ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local equiItemlist		= t_PrepareOpenHoleFrameControl["equipInfo"]["list"];
	local equiItemGrid		= t_PrepareOpenHoleFrameControl["equipInfo"]["grid"];
	local equipItem			= t_PrepareOpenHoleFrameControl:getOpenHoleEquipItem();
	

	if mainplayer:getMoney() < tonumber( PrepareOpenHoleFrameBullCostBtnFont:GetText() ) then
		ShowBuyMoneyFrame();
		return;
	end
	GameArmProce:clear();
	GameArmProce:addSrcItem( equipItem );
	GameArmProce:openGemHole( t_PrepareOpenHoleFrameControl["holeIdx"], equiItemlist );
end

-- ��ʯ��Ƕ
local MAX_XIANG_QIAN_KONG_NUM = 4;									-- { color = , list = , grid = }
-- NONE_LIST_TYPE
local t_BaoShiXiangQianFrameControl = { equipInfo = { grid = -1, id = 0, list = -1, lockBtn = "" }, 
			baoShiList = { { color = -1, list = -1, grid = -1 }, { color = -1, list = -1, grid = -1 }, { color = -1, list = -1, grid = -1 },
						{ color = -1, list = -1, grid = -1 }, } };

local t_genColorTex = 
{
[GEM_COLOR_RED]		= { ["back"] = { path = "uires\\ui\\mask\\TouMingTongDao.tga",		UV = { x = 0, y = 0, width = 18, height = 18, } }, 
						["tex"]  = { path = "uires\\ui\\buff  TuBiaoDiBan\\tip  XiangQianKuangDiBan18.tga", UV = { x = 0, y = 0, width = 18, height = 18, } }, 
					  },

[GEM_COLOR_BLUE]	= { ["back"] = { path = "uires\\ui\\mask\\TouMingTongDao.tga",		UV = { x = 18, y = 0, width = 18, height = 18, } }, 
						["tex"]  = { path = "uires\\ui\\buff  TuBiaoDiBan\\tip  XiangQianKuangDiBan18.tga", UV = { x = 18, y = 0, width = 18, height = 18, } }, 
					  },

[GEM_COLOR_GREEN]	= { ["back"] = { path = "uires\\ui\\mask\\TouMingTongDao.tga",		UV = { x = 36, y = 0, width = 18, height = 18, } }, 
						["tex"]  = { path = "uires\\ui\\buff  TuBiaoDiBan\\tip  XiangQianKuangDiBan18.tga", UV = { x = 36, y = 0, width = 18, height = 18, } }, 
					  },

[GEM_COLOR_COLOR]	= { ["back"] = { path = "uires\\ui\\mask\\TouMingTongDao.tga",		UV = { x = 54, y = 0, width = 18, height = 18, } }, 
						["tex"]  = { path = "uires\\ui\\buff  TuBiaoDiBan\\tip  XiangQianKuangDiBan18.tga", UV = { x = 54, y = 0, width = 18, height = 18, } }, 
					  },
					  --[[
[GEM_COLOR_RED]		= { ["back"] = { path = "uires\\ui\\buff  TuBiaoDiBan\\tipXiangQianKuang18.tga",		UV = { x = 0, y = 0, width = 18, height = 18, } }, 
						["tex"]  = { path = "uires\\ui\\buff  TuBiaoDiBan\\tip  XiangQianKuangDiBan18.tga", UV = { x = 0, y = 0, width = 18, height = 18, } }, 
					  },

[GEM_COLOR_BLUE]	= { ["back"] = { path = "uires\\ui\\buff  TuBiaoDiBan\\tipXiangQianKuang18.tga",		UV = { x = 18, y = 0, width = 18, height = 18, } }, 
						["tex"]  = { path = "uires\\ui\\buff  TuBiaoDiBan\\tip  XiangQianKuangDiBan18.tga", UV = { x = 18, y = 0, width = 18, height = 18, } }, 
					  },

[GEM_COLOR_GREEN]	= { ["back"] = { path = "uires\\ui\\buff  TuBiaoDiBan\\tipXiangQianKuang18.tga",		UV = { x = 36, y = 0, width = 18, height = 18, } }, 
						["tex"]  = { path = "uires\\ui\\buff  TuBiaoDiBan\\tip  XiangQianKuangDiBan18.tga", UV = { x = 36, y = 0, width = 18, height = 18, } }, 
					  },

[GEM_COLOR_COLOR]	= { ["back"] = { path = "uires\\ui\\buff  TuBiaoDiBan\\tipXiangQianKuang18.tga",		UV = { x = 54, y = 0, width = 18, height = 18, } }, 
						["tex"]  = { path = "uires\\ui\\buff  TuBiaoDiBan\\tip  XiangQianKuangDiBan18.tga", UV = { x = 54, y = 0, width = 18, height = 18, } }, 
					  },
					  --]]
};

function GetBaoShiXiangQianFrameControl()
	return t_BaoShiXiangQianFrameControl;
end

t_BaoShiXiangQianFrameControl["getBaoShiGridNum"] =
function ( self )
	return table.getn( self["baoShiList"] );
end

t_BaoShiXiangQianFrameControl["getInputBaoShiNum"] =
function ( self )
	local num = 0;

	for i = 1, table.getn( self["baoShiList"] ) do
		local oneBaoShiInfo = self["baoShiList"][i];
		if oneBaoShiInfo["list"] ~= NONE_LIST_TYPE then
			num = num + 1;
		end
	end
	return num;
end
t_BaoShiXiangQianFrameControl["getBaoShiMoneyCostNum"] =
function ( self,nBaseMoney )
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local nValue		= 0;
	for i = 1, table.getn( self["baoShiList"] ) do
		local oneBaoShiInfo = self["baoShiList"][i];
		local list			= oneBaoShiInfo["list"];
		local grid			= oneBaoShiInfo["grid"];
		if oneBaoShiInfo["list"] ~= NONE_LIST_TYPE then
			local oldItem	= container:getItem( list, grid );
			local gemNewDef = GameArmProce:getGemNewDef( oldItem:getItemId() );
			nValue = nValue + nBaseMoney * gemNewDef.InlayUseMoney;
		end
	end
	return nValue;
end
t_BaoShiXiangQianFrameControl["getBaoShiGridInfoByIndex"] =
function ( self, data )
	return self["baoShiList"][data["index"]];
end

-- �Ƿ�Ḳ��

t_BaoShiXiangQianFrameControl["isFuGaiBaoShi"] =
function ( self, data )
	local nIndex			= data["index"];
	local equipItem			= self:getXiangQianEquipItem();
	local nGemId			= equipItem:getHoleGemId( nIndex - 1 );

	return nGemId ~= 0;
end

t_BaoShiXiangQianFrameControl["canAddBaoShi"] =
function ( self, data )
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local item			= container:getItem( data["list"], data["grid"] );
	local itemDef		= item:getItemDef();
	local nIndex		= data["index"];
	
	local gemNewDef = GameArmProce:getGemNewDef( itemDef.ItemID );
	-- �����Ǳ�ʯ���ܷ���
	if not gemNewDef then
		--if bShowMsg then
		ShowMidTips( "����Ʒ���Ǳ�ʯ" );
		--end
		
		return false;
	end

	local equipItem			= self:getXiangQianEquipItem();
	local equipItemDef		= equipItem:getItemDef();
	
	if equipItem:getHoleNum() == 0 then
		ShowMidTips( "��װ��û�п����Ŀ�" );
		return false;
	end

	if nIndex ~=nil and nIndex > equipItem:getHoleNum() then
		ShowMidTips( "�ÿ׻�û�����޷���Ƕ" );
		return false;
	end
	
	local equipItemWearPre = equipItemDef.WearPre;
	--[[
	if ( equipItemWearPre.RoleLevelMin <= 30 and gemNewDef.Level > 1 ) or
		( equipItemWearPre.RoleLevelMin <= 60 and gemNewDef.Level > 2 ) or
		( equipItemWearPre.RoleLevelMin <= 60 and gemNewDef.Level > 3 ) or
		( equipItemWearPre.RoleLevelMin <= 60 and gemNewDef.Level > 4 ) or
		( equipItemWearPre.RoleLevelMin <= 60 and gemNewDef.Level > 5 ) then

			ShowMidTips( "װ���ȼ����㣬������Ƕ" );
			return false;
	end
	--]]

	if nIndex ~=nil then
		local oneBaoShiInfo = self["baoShiList"][nIndex];
		if  oneBaoShiInfo["color"] ~= -1 and oneBaoShiInfo["color"] ~= gemNewDef.Color then
			ShowMidTips( "��ʯ��ÿ���ɫ����Ӧ" );
			return false;
		end
	end

	for i = 1, table.getn( self["baoShiList"] ) do
		local oneBaoShiInfo = self["baoShiList"][i];
		if  oneBaoShiInfo["color"] ~= -1 and oneBaoShiInfo["color"] == gemNewDef.Color then
			return true;
		end
	end
	
	ShowMidTips( "û�ж�Ӧ��ɫ�ı�ʯ��λ" );
	return false;
end

t_BaoShiXiangQianFrameControl["isAnyBaoShiBind"] =
function ( self )
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	for i = 1, table.getn( self["baoShiList"] ) do
		local oneBaoShiInfo = self["baoShiList"][i];
		local list			= oneBaoShiInfo["list"];
		local grid			= oneBaoShiInfo["grid"];
		-- ����λ���ж���
		if list ~= NONE_LIST_TYPE then
			local gemItem = container:getItem( list, grid );
			if gemItem:isItemBind() then
				return true;
			end
		end
	end

	return false;
end

t_BaoShiXiangQianFrameControl["setBaoShi"] =
function ( self, data )
	local nIndex		= data["index"];
	--print( "setBaoShi--nIndex = "..nIndex );
	local list			= data["list"];
	local grid			= data["grid"];
	local oneBaoShiInfo = self["baoShiList"][nIndex];
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local item			= container:getItem( list, grid );
	if oneBaoShiInfo["list"] ~= NONE_LIST_TYPE then
		self:removeBaoShi( { index = nIndex } );
	end
	
	self["baoShiList"][nIndex]["list"] = list;
	self["baoShiList"][nIndex]["grid"] = grid;
	LockItem( item:getListType(), item:getGridIdx(), item:getItemId(), "BaoShiXiangQianFrame" );
	--print( "setBaoShi--LockItem--grid = "..item:getGridIdx().."�� listType = "..item:getListType().."--202" );
end

t_BaoShiXiangQianFrameControl["addBaoShi"] =
function ( self, data )
	local list			= data["list"];
	local grid			= data["grid"];
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local item			= container:getItem( list, grid );
	local gemNewDef		= GameArmProce:getGemNewDef( item:getItemId() );
	for i = 1, table.getn( self["baoShiList"] ) do
		local oneBaoShiInfo = self["baoShiList"][i];
		local btn		= getglobal( "BaoShiXiangQianFrame_BaoShiGridBtn"..i );
		
		if oneBaoShiInfo["color"] == gemNewDef.Color and oneBaoShiInfo["list"] == NONE_LIST_TYPE and btn:GetClientUserData( 0 ) == 0 then
				self["baoShiList"][i]["list"] = list;
				self["baoShiList"][i]["grid"] = grid;
				--print( "setBaoShi--LockItem--grid = "..item:getGridIdx().."�� listType = "..item:getListType().."--221" );
				LockItem( item:getListType(), item:getGridIdx(), item:getItemId(), "BaoShiXiangQianFrame" );
				return;
		end
	end

	ShowMidTips( "��û�п��еĶ�Ӧ��ʯ��λ" );
end

t_BaoShiXiangQianFrameControl["removeBaoShi"] =
function ( self, data )
	local nIndex		= data["index"];
	local oneBaoShi		= self["baoShiList"][nIndex];
	local list			= oneBaoShi["list"];
	local grid			= oneBaoShi["grid"];
	if list == -1 then
		return;
	end

	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local item			= container:getItem( list, grid );
	UnLockItem( list, grid, item:getItemId() );
	self["baoShiList"][nIndex]["list"] = -1;
	self["baoShiList"][nIndex]["grid"] = -1;
	local getGemUVAnim = getglobal( "BaoShiXiangQianFrame_BaoShiGridBtn"..nIndex.."GetGemUVAnimationTex" );	
	getGemUVAnim:Hide();
end

t_BaoShiXiangQianFrameControl["getXiangQianEquipItem"] =
function ( self )
	local list			= self["equipInfo"]["list"];
	local grid			= self["equipInfo"]["grid"];
	local mainplayer	= ActorMgr:getMainPlayer();
	if list == CONTAINER_TYPE_SUNDRIES then
		local container	= mainplayer:getContainer();
		equipItem		= container:getItem( list, grid );
	elseif list == CONTAINER_TYPE_WEAR then
		local Eqiup	= mainplayer:getEquip();
		equipItem = Eqiup:getItem( grid );
	end

	return equipItem;
end

t_BaoShiXiangQianFrameControl["setEquipInfo"] =
function ( self, data )
	local list = data["list"];
	local grid = data["grid"];
	--print( "setEquipInfo--list = "..list.."�� grid = "..grid );
	if self["equipInfo"]["grid"] == grid and self["equipInfo"]["list"] == list then	
		return;
	end
	
	self["equipInfo"]["grid"]	= grid;
	self["equipInfo"]["list"]	= list;
	local equipItem				= self:getXiangQianEquipItem();
	self["equipInfo"]["id"]		= equipItem:getItemId();

	for i = 1, equipItem:getHoleNum() do
		local nColor = equipItem:getHoleGemColor( i - 1 );
		self["baoShiList"][i]["color"] = nColor;
	end

	self["equipInfo"]["lockBtn"] = data["lockBtn"];
end

t_BaoShiXiangQianFrameControl["clearXiangQianInfo"] =
function ( self )	
	if self["equipInfo"]["list"] == CONTAINER_TYPE_WEAR then		
		local ItemTex	= getglobal( self["equipInfo"]["lockBtn"].."IconTexture" );
		ItemTex:SetGray( false );
	end

	self["equipInfo"]["list"]	= -1;
	self["equipInfo"]["grid"]	= -1;
	self["equipInfo"]["id"]		= 0;
	self["equipInfo"]["lockBtn"]= "";
	self:clearBaoShiList();
	GameArmProce:clear();
end

t_BaoShiXiangQianFrameControl["clearBaoShiList"] =
function ( self )
	for i = 1, table.getn( self["baoShiList"] ) do
		self["baoShiList"][i]["color"]	= -1;
		self["baoShiList"][i]["list"]	= NONE_LIST_TYPE;
		self["baoShiList"][i]["grid"]	= -1;
	end
		
	ClearItemLock( "BaoShiXiangQianFrame" );
end

t_BaoShiXiangQianFrameControl["updateBaoShiList"] =
function ( self )
	local equipItem	= self:getXiangQianEquipItem();

	for i = 1, equipItem:getHoleNum() do
		local oneBaoShiInfo			= self["baoShiList"][i];
		local list					= oneBaoShiInfo["list"];
		local grid					= oneBaoShiInfo["grid"];
		local bufferGridColor		= oneBaoShiInfo["color"];
		local equipItemGridColor	= equipItem:getHoleGemColor( i - 1 );
		if bufferGridColor == -1 and equipItemGridColor >= 0 then
			self["baoShiList"][i]["color"] = equipItemGridColor;
		end
	end
end

t_BaoShiXiangQianFrameControl["unlockAllBaoShi"] =
function ( self )
	-- �����ԭ�����еı�ʯ
	for i = 1, self:getBaoShiGridNum() do
		local oneBaoShiInfo = self:getBaoShiGridInfoByIndex( { index = i } );
		local list			= oneBaoShiInfo["list"];
		local grid			= oneBaoShiInfo["grid"];
		if list ~= NONE_LIST_TYPE then
			local oldItem	= container:getItem( list, grid );
			UnLockItem( list, grid, oldItem:getItemId() );
		end
	end
end

t_BaoShiXiangQianFrameControl["startBaoShiXiangQian"] = 
function ( self, data )
	local frame	= getglobal( "BaoShiXiangQianFrame" );
	frame:Hide();	
	if self["equipInfo"]["grid"] == data["grid"] and self["equipInfo"]["list"] == data["list"] then
		frame:Active();
		return;
	end

	for i = 1, MAX_XIANG_QIAN_KONG_NUM do
		local getGemUVAnim = getglobal( "BaoShiXiangQianFrame_BaoShiGridBtn"..i.."GetGemUVAnimationTex" );	
		getGemUVAnim:SetUVAnimation( 100, true );
		getGemUVAnim:Show();
	end
	
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local list			= data["list"];
	local grid			= data["grid"];
	if frame:IsShown() then
		if t_PrepareOpenHoleFrameControl:isCurOpenHoleEquip( self["equipInfo"] ) then
			PrepareOpenHoleFrame:Hide();
		end

		t_BaoShiXiangQianFrameControl:unlockAllBaoShi();
		self:clearXiangQianInfo( );
		self:setEquipInfo( data );
		UpdateBaoShiXiangQianFrame();
		frame:Active();
	else
		self:setEquipInfo( data );
		frame:Show();
	end
	
	PlayerContainerResolveFrame:Hide();
	PlayerContainerSparEpurateFrame:Hide();

	local item;
	if list == CONTAINER_TYPE_WEAR and EquipFrame:IsShown() then
		local ItemTex	= getglobal( data["lockBtn"].."IconTexture" );
		ItemTex:SetGray( true );
		local Equip = mainplayer:getEquip();
		item = Equip:getItem( grid );
	else
		item = container:getItem( list, grid );
		LockItem( item:getListType(), item:getGridIdx(), item:getItemId(), "BaoShiXiangQianFrame" );
	end
	--���ｫװ����������ʾ����������������������Ч�쳣����
	if item:getItemId() ~= nil then
		local itemDef = item:getItemDef();
		SetEquipImportantEffectUV( itemDef, "BaoShiXiangQianFrameEquipShowBtnUVAnimationTex", item );
	end
end

t_BaoShiXiangQianFrameControl["synchronizeBaoShiXiangQianList"] = 
function ( self )
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	for i = 1, table.getn( self["baoShiList"] ) do
		local list = self["baoShiList"][i]["list"];
		local grid = self["baoShiList"][i]["grid"];
		if list ~= -1 then
			local item = container:getItem( list, grid );
			if item:getItemId() == 0 then
				self["baoShiList"][i]["list"] = -1;
				self["baoShiList"][i]["grid"] = -1;
			end
		end
	end
end

function ClearBaoShiXiangQianFrameBaoShiList()
	for i = 1, MAX_XIANG_QIAN_KONG_NUM do
		local btn = getglobal( "BaoShiXiangQianFrame_BaoShiGridBtn"..i );
		btn:SetClientUserData( 0, 0 );
		
	--	local t_hideIcons = { btn:GetName().."BoxTexture", btn:GetName().."UVAnimationTex", btn:GetName().."WanMeiUVAnimationTex", };
		local t_hideIcons = { btn:GetName().."BoxTexture", btn:GetName().."BindTexture",};
		util.HideIcons( t_hideIcons );
		local t_textures = 
		{
			[btn:GetName().."IconTexture"] = {	path = "uires\\ZhuJieMian\\1.dds", UV = { x = 749, y = 517, width = 44, height = 44 },
												},
		};
		util.SetTextures( t_textures );
	end
end

local t_BaoShiGridColorTexList = 
{
[GEM_RED]	={ path = [[uires\ui\BaoShi\tip XiangQianKuanDiBan 42.tga]],	UV = { x = 0,   y = 0, width = 42, height = 42 }, },
[GEM_BLUE]	={ path = [[uires\ui\BaoShi\tip XiangQianKuanDiBan 42.tga]],	UV = { x = 42,  y = 0, width = 42, height = 42 }, },
[GEM_GREEN]	={ path = [[uires\ui\BaoShi\tip XiangQianKuanDiBan 42.tga]],	UV = { x = 84,  y = 0, width = 42, height = 42 }, },
[GEM_COLOR]	={ path = [[uires\ui\BaoShi\tip XiangQianKuanDiBan 42.tga]],	UV = { x = 168, y = 0, width = 42, height = 42 }, },
};

function UpdateBaoShiXiangQianFrameBaoShiList()
	ClearBaoShiXiangQianFrameBaoShiList();
	local mainplayer		= ActorMgr:getMainPlayer();
	local equipItem			= t_BaoShiXiangQianFrameControl:getXiangQianEquipItem();
	local container			= mainplayer:getContainer();
	
	for i = 1, table.getn( t_BaoShiXiangQianFrameControl["baoShiList"] ) do
		local oneBaoShiInfo = t_BaoShiXiangQianFrameControl["baoShiList"][i];
		local list			= oneBaoShiInfo["list"];
		local grid			= oneBaoShiInfo["grid"];
		local color			= oneBaoShiInfo["color"];
		local btn			= getglobal( "BaoShiXiangQianFrame_BaoShiGridBtn"..i );
		local boxTex		= getglobal( btn:GetName().."BoxTexture" );
		local getGemUVAnim = getglobal( btn:GetName().."GetGemUVAnimationTex" );	
		getGemUVAnim:Hide();
		-- ����λ���ж���
		if list ~= NONE_LIST_TYPE then
			local item			= container:getItem( list, grid );
			local itemDef		= item:getItemDef();
			btn:SetClientUserData( 0, itemDef.ItemID );
			local szPath		= GetItemEquipIconPath()..itemDef.IconID..".tga";
			if not IsInExistence( szPath ) then
				szPath = DEFAULT_ITEM_PATH;
			end
			
			getGemUVAnim:Show();
			
			local t_tgas = { [btn:GetName().."IconTexture"] = { path = szPath }, };
			util.SetTgas( t_tgas );			
			local bindTexture = getglobal( btn:GetName() .."BindTexture" );
			if item:isItemBind() then
				bindTexture:Show();
			else
				bindTexture:Hide();
			end
		elseif color ~= -1 then -- �����Ѿ���������û����
			boxTex:Hide();
			--print( "i = "..i.."�� color = "..color.."list"..list );
			local t_textures = 
			{
				[btn:GetName().."IconTexture"] = {	path = t_BaoShiGridColorTexList[color]["path"], UV = t_BaoShiGridColorTexList[color]["UV"],
													},
			};
			util.SetTextures( t_textures );
			local bindTexture = getglobal( btn:GetName() .."BindTexture" );
			bindTexture:Hide();
		--û�п��������ع���
		elseif color == -1 then
			HideBaoShiTools( i );
		end
	end

	for i = 1, equipItem:getHoleNum() do
		local btn		= getglobal( "BaoShiXiangQianFrame_BaoShiGridBtn"..i );
		local boxTex	= getglobal( btn:GetName().."BoxTexture" );
		boxTex:Hide();
		HideBaoShiTools( i );
		if btn:GetClientUserData( 0 ) == 0 then
			local nGemId	= equipItem:getHoleGemId( i - 1 );
			if nGemId ~= 0 then
				local itemDef		= getItemDef( nGemId );
				btn:SetClientUserData( 0, itemDef.ItemID );
				local szPath		= GetItemEquipIconPath()..itemDef.IconID..".tga";
				if not IsInExistence( szPath ) then
					szPath = DEFAULT_ITEM_PATH;
				end
				
				boxTex:Show();
				
				local getGemUVAnim = getglobal( btn:GetName().."GetGemUVAnimationTex" );	
				getGemUVAnim:Hide();
			
				ShowBaoShiTools( i );
				SetBaoshiToolsGray( i, false );
				local gemNewDef = GameArmProce:getGemNewDef( itemDef.ItemID );
				local setChargeGray = false;
				--if gemNewDef.Level <= GEM_RESOLVE_LEVEL_MIN or IsGemTopLevel( gemNewDef.Level, equipItem:getHoleGemStar( i - 1 ) ) then
				if gemNewDef.Level < GEM_CHARGE_LEVEL_MIN then
					setChargeGray = true;
				end
				local chargeBtn = getglobal("BaoShiXiangQianFrame_BaoShiChargeBtn"..i);
				chargeBtn:SetGray( setChargeGray );
				if setChargeGray then
					chargeBtn:Disable();
				else
					chargeBtn:Enable();
				end

				local t_tgas = { [btn:GetName().."IconTexture"] = { path = szPath }, };
				util.SetTgas( t_tgas );
				local bindTexture = getglobal( btn:GetName() .."BindTexture" );
				bindTexture:Hide();
			else
				ShowBaoShiTools( i );
				SetBaoshiToolsGray( i, true );
			end
		else
			ShowBaoShiTools( i );
			SetBaoshiToolsGray( i, true );
		end
	end
end

function ClearBaoShiXiangQianFrameBaoShiLanList()
	for i = 1, MAX_XIANG_QIAN_KONG_NUM do
		local btn = getglobal( "BaoShiXiangQianFrame_BaoShiLanBtn"..i );
		btn:Hide();
		local btnMaxTex = getglobal( btn:GetName().."MaxTexture" );
		btnMaxTex:Hide();
	end
end

local t_colorDesc = 
{	[0] = { desc = "��ɫ���", color = { r = GetColor()[COLOR_RED][1], g = GetColor()[COLOR_RED][2], b = GetColor()[COLOR_RED][3] }, }, 
	[1] = { desc = "��ɫ���", color = { r = GetColor()[COLOR_BULE][1], g = GetColor()[COLOR_BULE][2], b = GetColor()[COLOR_BULE][3] }, }, 
	[2] = { desc = "��ɫ���", color = { r = GetColor()[COLOR_GREEN][1], g = GetColor()[COLOR_GREEN][2], b = GetColor()[COLOR_GREEN][3] }, }, 
	[3] = { desc = "��ʲ��", color = { r = GetColor()[COLOR_ITEMGEMINLAY][1], g = GetColor()[COLOR_ITEMGEMINLAY][2], 
											b = GetColor()[COLOR_ITEMGEMINLAY][3] }, }, 
};

function UpdateBaoShiXiangQianFrameBaoShiLanList()
	ClearBaoShiXiangQianFrameBaoShiLanList();

	local equipItem	= t_BaoShiXiangQianFrameControl:getXiangQianEquipItem();
	
	for i = 1, equipItem:getHoleNum() do
		local btn = getglobal( "BaoShiXiangQianFrame_BaoShiLanBtn"..i );
		--print( "i = "..i.."�� btnName = "..btn:GetName() );
		
		local nGemId		= equipItem:getHoleGemId( i - 1 );
		local nColor		= equipItem:getHoleGemColor( i - 1 );
		local nGemMul		= ( 1 + equipItem:getHoleGemMul( i - 1 )/100 );
		-- TODO: �������ֱ������
		local nAttrResultId = equipItem:getHoleGemAttrIdx( i - 1 );
		local nResultIndex	= equipItem:getHoleGemResultIndex( i - 1 );
		local nStar			= equipItem:getHoleGemStar( i - 1 );

		local t_textures = { [btn:GetName().."BackTexture"]		= { path = t_genColorTex[nColor]["back"]["path"], 
																	UV = t_genColorTex[nColor]["back"]["UV"], 
																	}, 
							};
		util.SetTextures( t_textures )

		if nGemId ~= 0 then
			local itemDef		= getItemDef( nGemId );
			btn:SetClientUserData( 0, itemDef.ItemID );
			local szPath		= GetItemEquipIconPath()..itemDef.IconID..".tga";
			if not IsInExistence( szPath ) then
				szPath = DEFAULT_ITEM_PATH;
			end
			
			local t_tgas = { [btn:GetName().."IconTexture"]		= { path = szPath }, };
			util.SetTgas( t_tgas );
			
			local t_color;
			local t_NameColor = GetColor()[COLOR_WHITE];
			local gemNewDef = GameArmProce:getGemNewDef( itemDef.ItemID );
			-- ���Ƶ�ǰ��ʯ�ȼ��� ��Ӧ�ĳ��ܵȼ����ܲ����ڣ�
			local topStar = IsGemTopLevel( gemNewDef.Level,nStar );
			if topStar < nStar then 
				nStar = topStar;
			end
			
			local nRealIndex = getGemHoleAttrIndex( gemNewDef, nAttrResultId, gemNewDef.Level, nStar,nResultIndex,nGemMul);
			if nRealIndex == 9 then
				t_color = GetColor()[COLOR_PURPLE];
			elseif nRealIndex >=4 then
				t_color = GetColor()[COLOR_BULE];
			elseif nRealIndex < 4 then
				t_color = GetColor()[COLOR_GREEN];
			end
			local t_fonts = {	[btn:GetName().."DescFont"]		= { value = GetItemResultByIndex( nGemId, nStar,nAttrResultId,nResultIndex,nGemMul ), 
																	color = { r = t_color[1], g = t_color[2], b = t_color[3] }, 
																	}, 

								[btn:GetName().."NameFont"]		= { value = itemDef.Name, color = { r = t_NameColor[1], g = t_NameColor[2], b = t_NameColor[3] },
																	},
							};
			util.SetFonts( t_fonts );
			
			local nMin,nMax = getResultInfo( gemNewDef, nAttrResultId, gemNewDef.Level, nStar );
			local gemAttrStarDef = GameArmProce:getGemAttrStarDef( gemNewDef.Attr[nAttrResultId].AttrID, gemNewDef.Level, nStar );
			local attrValue = math.floor( gemAttrStarDef.Attr[nResultIndex].AttrID * nGemMul );
			local MaxTexture = getglobal( btn:GetName().."MaxTexture" );
			if attrValue >= nMax then
				MaxTexture:Show();
			else
				MaxTexture:Hide();
			end
		else
			local t_color = GetColor()[COLOR_ITEMGEMINLAY];
			local t_fonts = {	
							[btn:GetName().."NameFont"]		= { value = t_colorDesc[nColor]["desc"], 
																color = { r = t_color[1], g = t_color[2], b = t_color[3] } }, 
							[btn:GetName().."DescFont"]		= { value = "δ��Ƕ", 
																color = { r = t_color[1], g = t_color[2], b = t_color[3] }, }, 
							};
			util.SetFonts( t_fonts );

			local t_textures = { [btn:GetName().."IconTexture"]		= { path	= t_genColorTex[nColor]["tex"]["path"], 
																		UV		= t_genColorTex[nColor]["tex"]["UV"], }, 
																		};
			util.SetTextures( t_textures )			
		end

		btn:Show();
	end
end
	
function getGemHoleAttrIndex( gemNewDef, nResultID, level, star ,nResultIndex,nGemMul )
	local nMin				= 0;
	local nMax				= 0;
	local MAX_GEM_ATTR		= 10;
	local MAX_GEM_VAL		= 10;
	local nRealIndex 		= 1;
	local nAttrValue		= 0;
	for i = 1, MAX_GEM_ATTR do
		if gemNewDef.Attr[i-1].AttrID == gemNewDef.Attr[nResultID].AttrID then
			local gemAttrStarDef = GameArmProce:getGemAttrStarDef( gemNewDef.Attr[i-1].AttrID, level, star );
			if gemAttrStarDef == nil then
				break;
			end
			nMax = gemAttrStarDef.Attr[0].AttrID;
			nMin = gemAttrStarDef.Attr[0].AttrID;
			for j = 1,MAX_GEM_VAL do
				if gemAttrStarDef.Attr[j-1].AttrID > nMax then
					nMax = gemAttrStarDef.Attr[j-1].AttrID;
				end
				if gemAttrStarDef.Attr[j-1].AttrID < nMin then
					nMin = gemAttrStarDef.Attr[j-1].AttrID;
				end
			end
			nAttrValue = gemAttrStarDef.Attr[nResultIndex].AttrID * nGemMul;
			break;
		end
	end
	if nGemMul == 1 then 
		nRealIndex = nResultIndex;
	else
		for i = 1, MAX_GEM_ATTR do
			if gemNewDef.Attr[i-1].AttrID == gemNewDef.Attr[nResultID].AttrID then
				local gemAttrStarDef = GameArmProce:getGemAttrStarDef( gemNewDef.Attr[i-1].AttrID, level, star );
				if gemAttrStarDef == nil then
					break;
				end
				if nAttrValue >= nMax then 
					return MAX_GEM_ATTR - 1;
				elseif nAttrValue <= nMin then 
					return 0 ;
				end
				for j = 1,MAX_GEM_VAL do
					if gemAttrStarDef.Attr[j-1].AttrID > nAttrValue then
						nRealIndex = j - 2;
						break;
					end
				end
			end
		end		
	end
	return nRealIndex;
end

function BaoShiXiangQianFrame_onUpdate()
	t_BaoShiXiangQianFrameControl:updateBaoShiList();
	UpdateBaoShiXiangQianFrameBaoShiList();
	UpdateBaoShiXiangQianFrameBaoShiLanList();
	ShowBaoShiXiangQianFrameGuide();
end

function UpdateBaoShiXiangQianFrame()
	t_BaoShiXiangQianFrameControl:updateBaoShiList();
	UpdateBaoShiXiangQianFrameBaoShiList();
	UpdateBaoShiXiangQianFrameBaoShiLanList();
	ShowBaoShiXiangQianFrameGuide();
	
	local mainplayer		= ActorMgr:getMainPlayer();
	local equipItem			= t_BaoShiXiangQianFrameControl:getXiangQianEquipItem();
	local itemDef			= equipItem:getItemDef( );
	local t_NameColor		= GetItemNameColor( itemDef );
	local t_fonts = {	["BaoShiXiangQianFrameEquipNameBtnFont"]	= { value = itemDef.Name, 
																		color = { r = t_NameColor[1], g = t_NameColor[2], b = t_NameColor[3] },
																		}, 
						["BaoShiXiangQianFrameBullCostBtnFont"]		= 
						{ value = t_BaoShiXiangQianFrameControl:getBaoShiMoneyCostNum( itemDef.WearPre.GemInlayMoney ) }, 
						["BaoShiXiangQianFrameBullNowHaveBtnFont"]	= { value = mainplayer:getMoney() }, 
					};
	util.SetFonts( t_fonts );
	
	local ShowBtn = getglobal("BaoShiXiangQianFrameEquipShowBtn")
	ShowBtn:SetClientUserData( 0, itemDef.ItemID )	
	local itemIConTex = getglobal("BaoShiXiangQianFrameEquipShowBtnIconTexture")
	local szPath = GetItemEquipIconPath()..itemDef.IconID..".tga"	
	if not IsInExistence( szPath ) then
		szPath = DEFAULT_ITEM_PATH;
	end
	itemIConTex:SetTexture( szPath );
end

function GemOpenHole( nFlag )
	--local t_BaoShiXiangQianFrameControl = GetBaoShiXiangQianFrameControl();
	--t_BaoShiXiangQianFrameControl:updateBaoShiList();
	if nFlag == CLIENT_GEM_SUCCESS then
		--PrepareOpenHoleFrameOpenHoleButton:Hide();
		--PrepareOpenHoleFrameConfirmButton:Show();
		PrepareOpenHoleFrame:Hide();
	elseif nFlag == CLIENT_GEM_FAIL then
		local t_fonts = {	["PrepareOpenHoleFrameBlackButtonDescFont"]	= { value = "���ź������ʧ���ˣ�Ҫ���������" }, 
						};
		util.SetFonts( t_fonts );
	end
	UpdateBaoShiXiangQianFrame();
end

local t_inLayMsg = 
{
[CLIENT_GEM_FAIL]		= "��ʯ��Ƕʧ��",
[CLIENT_GEM_SUCCESS]	= "��ʯ��Ƕ�ɹ�",
[CLIENT_GEM_PERFECT]	= "��ʯ������Ƕ",
};
function GemInLay( nFlag, nIndex )
	BaoShiXiangQianFrame_XiangQianBtn:SetGray( false );
	BaoShiXiangQianFrame_XiangQianBtn:Enable();
	if not BaoShiXiangQianFrame:IsShown() then
		return;
	end	
	if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_XIANGQIANBAOSHIFRAME_XIANGQIAN_BTN ) and
		GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_XIANGQIANBAOSHIFRAME_BAO_SHI_STUFF_BTN ) then
		GameWizard:doStep( GAMEWIZARD_FIRST_BLINK_XIANGQIANBAOSHIFRAME_XIANGQIAN_BTN );
	end
	t_BaoShiXiangQianFrameControl:clearBaoShiList();
	--UpdateBaoShiXiangQianFrame();
	-- TODO HERE �����ﵽline 750 ����ʱ���� �滻��UpdateBaoShiXiangQianFrame,���������ȥ������uv ��Ч�Ĵ���,
	t_BaoShiXiangQianFrameControl:updateBaoShiList();
	for i = 1, MAX_XIANG_QIAN_KONG_NUM do
		local btn = getglobal( "BaoShiXiangQianFrame_BaoShiGridBtn"..i );
		btn:SetClientUserData( 0, 0 );
		local t_textures = 
		{
			[btn:GetName().."IconTexture"] = {	path = "uires\\ZhuJieMian\\1.dds", UV = { x = 749, y = 517, width = 44, height = 44 },
												},
		};
		util.SetTextures( t_textures );
	end

	local mainplayer		= ActorMgr:getMainPlayer();
	local equipItem			= t_BaoShiXiangQianFrameControl:getXiangQianEquipItem();
	local container			= mainplayer:getContainer();

	for i = 1, table.getn( t_BaoShiXiangQianFrameControl["baoShiList"] ) do
		local oneBaoShiInfo = t_BaoShiXiangQianFrameControl["baoShiList"][i];
		local list			= oneBaoShiInfo["list"];
		local grid			= oneBaoShiInfo["grid"];
		local color			= oneBaoShiInfo["color"];
		local btn			= getglobal( "BaoShiXiangQianFrame_BaoShiGridBtn"..i );
		local boxTex		= getglobal( btn:GetName().."BoxTexture" );
		-- ����λ���ж���
		if list ~= NONE_LIST_TYPE then
			local item			= container:getItem( list, grid );
			local itemDef		= item:getItemDef();
			btn:SetClientUserData( 0, itemDef.ItemID );
			local szPath		= GetItemEquipIconPath()..itemDef.IconID..".tga";
			if not IsInExistence( szPath ) then
				szPath = DEFAULT_ITEM_PATH;
			end
			
			local t_tgas = { [btn:GetName().."IconTexture"] = { path = szPath }, };
			util.SetTgas( t_tgas );
		elseif color ~= -1 then -- �����Ѿ���������û����	
			boxTex:Show();
			local t_textures = 
			{
				[btn:GetName().."IconTexture"] = {	path = t_BaoShiGridColorTexList[color]["path"], UV = t_BaoShiGridColorTexList[color]["UV"],
													},
			};
			util.SetTextures( t_textures );
		end
	end

	for i = 1, equipItem:getHoleNum() do
		local btn		= getglobal( "BaoShiXiangQianFrame_BaoShiGridBtn"..i );
		local boxTex	= getglobal( btn:GetName().."BoxTexture" );
		local bindTexture = getglobal( btn:GetName() .. "BindTexture" );
		boxTex:Hide();
		bindTexture:Hide();
		HideBaoShiTools( i );
		if btn:GetClientUserData( 0 ) == 0 then
			local nGemId	= equipItem:getHoleGemId( i - 1 );
			if nGemId ~= 0 then
				local itemDef		= getItemDef( nGemId );
				btn:SetClientUserData( 0, itemDef.ItemID );
				local szPath		= GetItemEquipIconPath()..itemDef.IconID..".tga";
				if not IsInExistence( szPath ) then
					szPath = DEFAULT_ITEM_PATH;
				end				
				boxTex:Show();
				
				ShowBaoShiTools( i );
				SetBaoshiToolsGray( i, false );
				local gemNewDef = GameArmProce:getGemNewDef( itemDef.ItemID );
				local setChargeGray = false;
				--if gemNewDef.Level <= GEM_RESOLVE_LEVEL_MIN or IsGemTopLevel( gemNewDef.Level, equipItem:getHoleGemStar( i - 1 ) ) then
				if gemNewDef.Level < GEM_CHARGE_LEVEL_MIN then
					setChargeGray = true;
				end
				local chargeBtn = getglobal("BaoShiXiangQianFrame_BaoShiChargeBtn"..i);
				chargeBtn:SetGray( setChargeGray );
				if setChargeGray then
					chargeBtn:Disable();
				else
					chargeBtn:Enable();
				end

				local t_tgas = { [btn:GetName().."IconTexture"] = { path = szPath }, };
				util.SetTgas( t_tgas );
			else
				ShowBaoShiTools( i );
				SetBaoshiToolsGray( i, true );
			end
		else
			ShowBaoShiTools( i );
			SetBaoshiToolsGray( i, true );
		end
	end

	UpdateBaoShiXiangQianFrameBaoShiLanList();
	
	local itemDef			= equipItem:getItemDef( );
	local t_NameColor		= GetItemNameColor( itemDef );
	local t_fonts = {	["BaoShiXiangQianFrameEquipNameBtnFont"]	= { value = itemDef.Name, 
																		color = { r = t_NameColor[1], g = t_NameColor[2], b = t_NameColor[3] },
																		}, 
						["BaoShiXiangQianFrameBullCostBtnFont"]		= 
						{ value = t_BaoShiXiangQianFrameControl:getBaoShiMoneyCostNum( itemDef.WearPre.GemInlayMoney )}, 
						["BaoShiXiangQianFrameBullNowHaveBtnFont"]	= { value = getPlayerMoney( mainplayer, ATTR_ID_MONEY ) }, 
					};
	util.SetFonts( t_fonts );

	if nFlag == CLIENT_GEM_FAIL then
		ShowMidTips( t_inLayMsg[nFlag] );
	else
		--print( "nFlag = "..nFlag.."--616 nIndex = "..nIndex );
		local nBtnIndex = nIndex;
		local animationTexture = getglobal( "BaoShiXiangQianFrame_BaoShiGridBtn"..nBtnIndex.."UVAnimationTex" );
		animationTexture:SetUVAnimation( 100, false );
		animationTexture:Show();
		if nFlag == CLIENT_GEM_PERFECT then
			--print( "nFlag = "..nFlag.."--622" );
			local animationTexture = getglobal( "BaoShiXiangQianFrame_BaoShiGridBtn"..nBtnIndex.."WanMeiUVAnimationTex" );
			animationTexture:SetUVAnimation( 100, false );
			animationTexture:Show();
		end
	end
end

-- 
local t_BaoShiXiangQianFrameEvents = { ["GE_CONTAINER_UPDATE"] = {}, ["GE_CONTAINER_ITEM_UPDATE"] = {}, ["GE_CHANGE_BULL"] = {}, };

t_BaoShiXiangQianFrameEvents["GE_CONTAINER_UPDATE"].func = 
function ()
	local equipItem			= t_BaoShiXiangQianFrameControl:getXiangQianEquipItem();
	if equipItem == nil then
		return;
	end
	
	local list = t_BaoShiXiangQianFrameControl["equipInfo"]["list"];
	if list == CONTAINER_TYPE_WEAR and equipItem:getItemId() ~= t_BaoShiXiangQianFrameControl["equipInfo"]["id"] then
		ShowMidTips( "��Ҫ��Ƕװ���Ѿ���������������" );
		BaoShiXiangQianFrame:Hide();
	end
end

t_BaoShiXiangQianFrameEvents["GE_CHANGE_BULL"].func = 
function ()
	local mainplayer		= ActorMgr:getMainPlayer();
	local t_fonts = {	
						["BaoShiXiangQianFrameBullNowHaveBtnFont"]	= { value = getPlayerMoney( mainplayer,ATTR_ID_MONEY ) }, 
					};
	util.SetFonts( t_fonts );
end

function BaoShiXiangQianFrame_OnLoad()
	for event, _ in pairs( t_BaoShiXiangQianFrameEvents ) do
		this:RegisterEvent( event );
	end

	t_BaoShiXiangQianFrameEvents.__index = function ( tab, key )
									tab[key] = { func = function () end };
									return tab[key];
								end
	setmetatable( t_BaoShiXiangQianFrameEvents, t_BaoShiXiangQianFrameEvents );
end

function BaoShiXiangQianFrame_OnEvent()
	t_BaoShiXiangQianFrameEvents[arg1].func();
end

function BaoShiXiangQianFrame_Test_OnShow()
	local animationTexture = getglobal( "BaoShiXiangQianFrame_BaoShiGridBtn1UVAnimationTex" );
	animationTexture:SetUVAnimation( 50, false );
	animationTexture:Show();
	
	local animationTexture = getglobal( "BaoShiXiangQianFrame_BaoShiGridBtn1WanMeiUVAnimationTex" );
	animationTexture:SetUVAnimation( 50, false );
	animationTexture:Show();

	BaoShiXiangQianFrame_XiangQianBtn:SetGray( false );
	BaoShiXiangQianFrame_XiangQianBtn:Enable();
end

function BaoShiXiangQianFrame_OnShow()
	this:SetPoint( "center","UIClient", "center", -320, 0 );
	UpdateBaoShiXiangQianFrame();
end

function ShowBaoShiXiangQianFrameGuide()
	HideBaoShiXiangQianFrameGuide();
	for i = 1, table.getn( t_BaoShiXiangQianFrameControl["baoShiList"] ) do
		local oneBaoShiInfo = t_BaoShiXiangQianFrameControl["baoShiList"][i];
		local list			= oneBaoShiInfo["list"];
		local grid			= oneBaoShiInfo["grid"];
		local color			= oneBaoShiInfo["color"];
		if color == -1 then
			local gridBtn			= getglobal( "BaoShiXiangQianFrame_BaoShiGridBtn"..i );
			local animationTexture	= getglobal( gridBtn:GetName().."XinShouUVAnimationTex");
			animationTexture:SetUVAnimation( 200, true );
		end
	end	
end

function HideBaoShiXiangQianFrameGuide()
	for i = 1, MAX_XIANG_QIAN_KONG_NUM do
		local animationTexture = getglobal( "BaoShiXiangQianFrame_BaoShiGridBtn"..i.."XinShouUVAnimationTex" );
		animationTexture:Hide();
	end

	local t_hideIcons = { "BaoShiXiangQianFrameXinShouBtn" };
	util.HideIcons( t_hideIcons );
end

function BaoShiXiangQianFrame_OnHide()
	GemProceResetFrame:Hide();
	GemProceChargeFrame:Hide();
	GemProcePickFrame:Hide();
	local t_hideIcons = { "PrepareOpenHoleFrame", "GemProcePickFrame", "ConfirmXiangQianNotBindEquipFrame","GemProceChargeFrame","GemProceResetFrame", };
	util.HideIcons( t_hideIcons );
	t_BaoShiXiangQianFrameControl:clearXiangQianInfo();
end

function BaoShiXiangQianOnEnter( szBtnName )
	local btn			= getglobal( szBtnName );
	local mainplayer	= ActorMgr:getMainPlayer();
	if mainplayer == nil then
		return;
	end
	local t_hideIcons = { "NormalItemTipsFrame", "ArmItemTipsFrame", "SecondArmItemTipsFrame", };
	util.HideIcons( t_hideIcons );	
	
	local equipItem = t_BaoShiXiangQianFrameControl:getXiangQianEquipItem();
	local holeIdx = btn:GetClientID();
	local szGemData = equipItem:getHoleGemStar( holeIdx - 1 )..","..equipItem:getHoleGemPower( holeIdx - 1 );

	local container		= mainplayer:getContainer();
	local id			= btn:GetClientUserData( 0 );
	local item			= container:getItem( id );
	if equipItem:getHoleGemId( holeIdx - 1 ) <= 0 then
		if item:getItemId() == 0 then return end
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item, BaoShiXiangQianOnEnter, szBtnName, nil, nil, szGemData);
	else
		--���ﴦ���滻��û����Ƕ�ı�ʯtips
		if item:getItemId() > 0 then
			if equipItem:getHoleGemId( holeIdx - 1 ) ~= id or equipItem:getHoleGemPower( holeIdx - 1 ) ~= item:getGemPower() or equipItem:getHoleGemStar( holeIdx - 1 ) ~= item:getGemStart() then
				if IsItemLock( item:getListType(), item:getGridIdx(), id ) then
					ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item, BaoShiXiangQianOnEnter, szBtnName, nil, nil, szGemData);
					return;
				end
			end
		end
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, getItemDef( equipItem:getHoleGemId( holeIdx - 1 ) ), nil, nil, nil, nil, szGemData );
	end
end

function BaoShiXiangQianFrame_BaoShiGridBtn_OnEnter()
	local id = this:GetClientUserData( 0 );
	if id == 0 then
		local nBaoShiIndex	= this:GetClientID();
		local oneBaoShiInfo = t_BaoShiXiangQianFrameControl["baoShiList"][nBaoShiIndex];
		
		local color			= oneBaoShiInfo["color"];
		if color ~= -1 then
			return;
		end
		local szText = "���������ʯ��";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
							button = this:GetName() } );
		return;
	end

	BaoShiXiangQianOnEnter( this:GetName() );
end

function BaoShiXiangQianFrame_BaoShiGridBtn_OnLeave()
	local t_hideIcons = { "NormalItemTipsFrame", "ArmItemTipsFrame", "SecondArmItemTipsFrame", "GameTooltip", "SpellTipsFrame" };
	util.HideIcons( t_hideIcons );
end

function Save2BaoBaoShiXiangQianFrame( item, nIndex )
	-- ���Ѿ�������ʯ�����������ͨ���Ҽ�׷�ӱ�ʯ
	if not t_BaoShiXiangQianFrameControl:canAddBaoShi( { grid = item:getGridIdx(), list = item:getListType(), index = nIndex } ) then
		return; 
	end

	local itemDef		= item:getItemDef();
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	
	if nIndex ~= nil then
		if t_BaoShiXiangQianFrameControl:isFuGaiBaoShi( { index = nIndex } ) then
			local t_ReplaceBaoShiConfirmFramControl = GetReplaceBaoShiConfirmFramControl();
			t_ReplaceBaoShiConfirmFramControl:startConfirmReplaceData( { baoShiGrid = nIndex, grid = item:getGridIdx(), list = item:getListType() } );
			return;
		end
		t_BaoShiXiangQianFrameControl:setBaoShi( { grid = item:getGridIdx(), list = item:getListType(), index = nIndex } );
	else
		t_BaoShiXiangQianFrameControl:addBaoShi( { grid = item:getGridIdx(), list = item:getListType() } );
	end

	UpdateBaoShiXiangQianFrame();
end

function BaoShiXiangQianFrame_BaoShiGridBtn_ReceiveDrag()
	local srcList = UIMSG:getUIGV().DragFrom;
	if ConfirmXiangQianNotBindEquipFrame:IsShown() then
		ShowMidTips( "���������Ƕȷ�ϲ���" );
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	end

	if srcList ~= CONTAINER_TYPE_SUNDRIES and srcList ~= CONTAINER_TYPE_STUFF then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	end

	local srcGrid		= UIGetDragUserData1();
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local item			= container:getItem( srcList, srcGrid );
	--if t_BaoShiHeChengControl:canAddBaoShi( { grid = item:getGridIdx(), list = item:getListType(), showMsg = true } ) then
	Save2BaoBaoShiXiangQianFrame( item, this:GetClientID() );
	UIEndDrag();
	UIMSG:getUIGV().DragFrom = DRAG_NONE;
end

function BaoShiXiangQianFrame_BaoShiGridBtn_OnClick()
	local nIndex		= this:GetClientID();
	local oneBaoShi		= t_BaoShiXiangQianFrameControl["baoShiList"][nIndex];
	local list			= oneBaoShi["list"];
	local grid			= oneBaoShi["grid"];
	if arg1 == "RightButton" then
		if list == -1 then
			return;
		end

		local id = this:GetClientUserData( 0 );
		if id == 0 then
			return;
		end

		if ConfirmXiangQianNotBindEquipFrame:IsShown() then
			ShowMidTips( "���������Ƕȷ�ϲ���" );
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return;
		end
		
		t_BaoShiXiangQianFrameControl:removeBaoShi( { index = nIndex } );
		UpdateBaoShiXiangQianFrame()
	elseif arg1 == "LeftButton" then
		local equipItem	= t_BaoShiXiangQianFrameControl:getXiangQianEquipItem();
		if equipItem:getHoleNum() == 0 and nIndex > 1 then
			--ShowMidTips( "���ȿ���ǰ��Ŀ�" );
			t_PrepareOpenHoleFrameControl:startOpenHole( {	holeIdx = equipItem:getHoleNum(), 
															grid = t_BaoShiXiangQianFrameControl["equipInfo"]["grid"],
															list = t_BaoShiXiangQianFrameControl["equipInfo"]["list"], } );
			return;
		end
		-- TODO
		--	����һ���ף�������2
		--print( "equipItem:getHoleNum() = "..equipItem:getHoleNum().."��nIndex = "..nIndex );
		if equipItem:getHoleNum() > 0 and equipItem:getHoleNum() + 1 < nIndex then
			--ShowMidTips( "���ȿ���ǰ��Ŀ�" );
			t_PrepareOpenHoleFrameControl:startOpenHole( {	holeIdx = equipItem:getHoleNum(), 
															grid	= t_BaoShiXiangQianFrameControl["equipInfo"]["grid"],
															list = t_BaoShiXiangQianFrameControl["equipInfo"]["list"], } );
			return;
		end

		local color = oneBaoShi["color"];
		if color == -1 then
			t_PrepareOpenHoleFrameControl:startOpenHole( {	holeIdx = nIndex - 1, grid = t_BaoShiXiangQianFrameControl["equipInfo"]["grid"],
															list = t_BaoShiXiangQianFrameControl["equipInfo"]["list"], } );
			return;
		end
--[[
		if this:GetClientUserData( 0 ) > 0 and equipItem:getHoleGemId( nIndex - 1 ) > 0 then
			GemProcePickFrame:SetClientUserData( 0,this:GetClientUserData(0) )
			GemProcePickFrame:SetClientUserData( 1,nIndex );
			GemProcePickFrame:SetClientUserData( 2,t_BaoShiXiangQianFrameControl["equipInfo"]["grid"] );
			GemProcePickFrame:SetClientUserData( 3,t_BaoShiXiangQianFrameControl["equipInfo"]["list"] );
			GemProcePickFrame:Show();
		end
	--]]
	end
end

-- ��Ƕ��ť
function BaoShiXiangQianFrame_XiangQianBtn_OnClick()
	local mainplayer		= ActorMgr:getMainPlayer();
	local equiItemList		= t_BaoShiXiangQianFrameControl["equipInfo"]["list"];
	local equipItem			= t_BaoShiXiangQianFrameControl:getXiangQianEquipItem();
	
	if equipItem:getHoleNum() == 0 then
		ShowMidTips( "û�п�����Ƕ�Ŀף�������װ���Ͽ���" );
		return;
	end

	if t_BaoShiXiangQianFrameControl:getInputBaoShiNum() == 0 then
		ShowMidTips( "���ȷ���Ҫ��Ƕ�ı�ʯ" );
		return;
	end

	local equipItemDef	= equipItem:getItemDef( );
	local nNeedBull		= t_BaoShiXiangQianFrameControl:getBaoShiMoneyCostNum( equipItemDef.WearPre.GemInlayMoney );
	if mainplayer:getMoney() < nNeedBull then
		ShowMidTips( "���Ҳ��㣬��Ƕʧ��" );
		ShowBuyMoneyFrame();
		return;
	end
	
	if t_BaoShiXiangQianFrameControl:isAnyBaoShiBind() and not equipItem:isItemBind() then
		ConfirmXiangQianNotBindEquipFrame:Show();
	else
		Public_BaoShiXiangQianFrame_ActXiangQian();
	end
end

function Public_BaoShiXiangQianFrame_ActXiangQian()
	local mainplayer		= ActorMgr:getMainPlayer();
	local equiItemList		= t_BaoShiXiangQianFrameControl["equipInfo"]["list"];
	local equipItem			= t_BaoShiXiangQianFrameControl:getXiangQianEquipItem();
	local container			= mainplayer:getContainer();
	GameArmProce:clear();
	GameArmProce:addSrcItem( equipItem, STUFF_EQUIP );

	for i = 1, table.getn( t_BaoShiXiangQianFrameControl["baoShiList"] ) do
		local oneBaoShiInfo = t_BaoShiXiangQianFrameControl["baoShiList"][i];
		local list			= oneBaoShiInfo["list"];
		local grid			= oneBaoShiInfo["grid"];
		-- ����λ���ж���
		if list ~= NONE_LIST_TYPE then
			local item = container:getItem( list, grid );
			if container:getContainerLockState() ~= CONTAINER_LOCK_STATE_LOCK then
				--BaoShiXiangQianFrame_XiangQianBtn:SetGray( true );
				--BaoShiXiangQianFrame_XiangQianBtn:Disable();
			else
				ShowMidTips("�����������޷���Ƕ");
				return;
			end
			GameArmProce:gemInlay( i, item, equiItemList );
		end
	end
end

--  ȷ��������Ƕ
local t_ReplaceBaoShiConfirmFramControl = { baoShiGrid = 0, grid = -1, list = -1 };

function GetReplaceBaoShiConfirmFramControl()
	return t_ReplaceBaoShiConfirmFramControl;
end

t_ReplaceBaoShiConfirmFramControl["startConfirmReplaceData"] = 
function ( self, data )
	self:setConfirmReplaceData( { baoShiGrid = data["baoShiGrid"], grid = data["grid"], list = data["list"] } );
	local nGrid					= data["baoShiGrid"];
	local equipItem				= t_BaoShiXiangQianFrameControl:getXiangQianEquipItem();
	local nReplacedGemId		= equipItem:getHoleGemId( nGrid - 1 );
	local replacedGemNewDef		= GameArmProce:getGemNewDef( nReplacedGemId );
	local replacedItemDef		= getItemDef( nReplacedGemId );
	local nReplaceAttrResultId	= equipItem:getHoleGemAttrIdx( nGrid - 1 );
	local nReplacedResultIndex	= equipItem:getHoleGemResultIndex( nGrid - 1 );
	local nStar					= equipItem:getHoleGemStar( nGrid - 1 );
	local nGemMul		= ( 1 + equipItem:getHoleGemMul( nGrid - 1 )/100 );
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local item			= container:getItem( data["list"], data["grid"] );
	local itemDef		= item:getItemDef();
	local gemNewDef		= GameArmProce:getGemNewDef( itemDef.ItemID ); 
	local frame = getglobal( "ReplaceBaoShi_ConfirmFrame" );
	local szResultText	= "";
	for i = 1, item:getHoleNum() do
	end
	
	local t_riches = 
	{	
		["ReplaceBaoShi_ConfirmFrameDescRich"] = 
		{	value = "�ò����Ὣ".."#cFC99E4"..replacedGemNewDef.Level.."��"..replacedItemDef.Name.."  "..
					GetItemResultByIndex( nReplacedGemId, nStar, nReplaceAttrResultId, nReplacedResultIndex,nGemMul ).."#n�滻����ȷ���滻��Ƕ��",
			color = { r = 255, g = 229, b = 189 },  
		}, 
	
	};

	util.SetRiches( t_riches );
	frame:Show();
end

t_ReplaceBaoShiConfirmFramControl["setConfirmReplaceData"] = 
function ( self, data )
	self["baoShiGrid"]	= data["baoShiGrid"];
	self["grid"]		= data["grid"];
	self["list"]		= data["list"];
end

t_ReplaceBaoShiConfirmFramControl["clearConfirmReplaceData"] = 
function ( self )
	self["baoShiGrid"]	= 0;
	self["grid"]		= -1;
	self["list"]		= -1;
end

function ReplaceBaoShi_ConfirmFrame_ConfirmBtn_OnClick()
	t_BaoShiXiangQianFrameControl:setBaoShi( {	grid = t_ReplaceBaoShiConfirmFramControl["grid"], 
												list = t_ReplaceBaoShiConfirmFramControl["list"], 
												index = t_ReplaceBaoShiConfirmFramControl["baoShiGrid"] } );
	UpdateBaoShiXiangQianFrame();
	ReplaceBaoShi_ConfirmFrame:Hide();
end

function ReplaceBaoShi_ConfirmFrame_OnHide()
	t_ReplaceBaoShiConfirmFramControl:clearConfirmReplaceData()
end

---------------------------------------------------------------------------------------------------------------------------------------------------------------

-- ��ʯ�ϳ�
local t_BaoShiHeChengControl = { heChengFuID = 0, baoShiLV = -1, list = NONE_LIST_TYPE, grid = -1, isFinalHeChengFu = -1,
								baoShiList = { { list = NONE_LIST_TYPE, grid = -1, }, { list = NONE_LIST_TYPE, grid = -1, }, 
												{ list = NONE_LIST_TYPE, grid = -1, }, { list = NONE_LIST_TYPE, grid = -1, }, 
												{ list = NONE_LIST_TYPE, grid = -1, }, }, 

								successBaoShi = { id = 0, }, NPC_RES_ID = 294027, MAP_ID = 1000, MAX_DIST = 1000, MAX_BAO_SHI_NUM = 5, };

function GetBaoShiHeChengControl()
	return t_BaoShiHeChengControl;
end

t_BaoShiHeChengControl["isBaoShiFull"] =
function ( self )
	return self:getBaoShiNum() == self["MAX_BAO_SHI_NUM"];
end

t_BaoShiHeChengControl["getBaoShiNum"] =
function ( self )
	local nNum = 0;
	for i = 1, table.getn( self["baoShiList"] ) do
		local oneBaoShiInfo = self["baoShiList"][i];
		if oneBaoShiInfo["list"] ~= -1 then
			nNum = nNum + 1;
		end
	end

	return nNum;
end

t_BaoShiHeChengControl["update"] = 
function ( self )
--[[
	local heChengNpcDefInfo = MapShower:getNpcInfoById( self["MAP_ID"], self["NPC_RES_ID"] );
	local mainplayer		= ActorMgr:getMainPlayer();
	local nCurPosX			= mainplayer:getPlayerX() / 10;
	local nCurPosY			= mainplayer:getPlayerZ() / 10;
	local mainPosPoint		= { x = nCurPosX, y = nCurPosY };
	local npcPosPoint		= { x = 11700, y = 36300 };
	local nDist				= util.CalPointsDistance( mainPosPoint, npcPosPoint );
	
	if nDist > ( self["MAX_DIST"] ) ^ 2 then
		ShowMidTips( "����̫Զ���޷����кϳɱ�ʯ����" );
		BaoShiHeChengFrame:Hide();
	end
--]]
end

t_BaoShiHeChengControl["canAddBaoShi"] =
function ( self, data )
	-- ����ʱ���Ѿ������ϳ�ֻ����ʾ���ɹ�Ч���򣬲��ܷ��뱦ʯ
	if self["list"] == NONE_LIST_TYPE then
		return false;
	end

	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local item			= container:getItem( data["list"], data["grid"] );
	local itemDef		= item:getItemDef();

	local nSrcTotalNum		= item:getNum();
	local nDestHaveNum		= self:getOneSameGridBaoShiNum( data );
	if nDestHaveNum == nSrcTotalNum then
		return;
	end
	
	local gemNewDef = GameArmProce:getGemNewDef( itemDef.ItemID );
	local bShowMsg = data["showMsg"];
	-- �����Ǳ�ʯ���ܷ���
	if not gemNewDef then
		--if bShowMsg then
		ShowMidTips( "����Ʒ���Ǳ�ʯ" );
		--end
		
		return false;
	end
	
	if self["isFinalHeChengFu"] == 1 and self:getBaoShiNum() == 0 then
		self["baoShiLV"] = gemNewDef.Level
	end

	if gemNewDef.Level ~= self["baoShiLV"] then
		if self["isFinalHeChengFu"] == 1 then
			ShowMidTips( "���Ѿ�����ı�ʯ�ȼ���ͬ���������ͬ�ȼ���ʯ" );
			return false
		end
		--if bShowMsg then
		ShowMidTips( "�ñ�ʯ�ȼ��뱦ʯ�������ȼ�����" );
		--end
		
		return false;
	end

	return true;
end

t_BaoShiHeChengControl["setBaoShi"] =
function ( self, data )
	local nIndex		= data["index"];
	--print( "setBaoShi--nIndex = "..nIndex );
	local list			= data["list"];
	local grid			= data["grid"];
	local oneBaoShiInfo = self["baoShiList"][nIndex];
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local item			= container:getItem( list, grid );
	if oneBaoShiInfo["list"] ~= NONE_LIST_TYPE then
		self:removeBaoShi( { index = nIndex } );
	end
	
	self["baoShiList"][nIndex]["list"] = list;
	self["baoShiList"][nIndex]["grid"] = grid;
	LockItem( item:getListType(), item:getGridIdx(), item:getItemId(), "BaoShiHeChengFrame" );
end

t_BaoShiHeChengControl["setMultiBaoShi"] =
function ( self, data )
	self:setBaoShi( data );
	self:addMultiBaoShi( { ["notShowMsg"] = true, ["list"] = data["list"], ["grid"] = data["grid"] } );
end

t_BaoShiHeChengControl["getOneSameGridBaoShiNum"] =
function ( self, data )
	local nNum = 0;
	if data["list"] == -1 or data["grid"] == -1 then
		return nNum;
	end

	for i = 1, table.getn( self["baoShiList"] ) do
		local oneBaoShiInfo = self["baoShiList"][i];
		if oneBaoShiInfo["list"] == data["list"] and oneBaoShiInfo["grid"] == data["grid"] then
			nNum = nNum + 1;
		end
	end

	return nNum;
end

t_BaoShiHeChengControl["addMultiBaoShi"] =
function ( self, data )
	if self:isBaoShiFull() then
		local notShowWarning = data["notShowMsg"];
		if notShowWarning then
			return;
		end

		ShowMidTips( "���ںϳɵı�ʯ�Ѵﵽ�������������ȡ��һ��" );
		return;
	end

	local mainplayer		= ActorMgr:getMainPlayer();
	local container			= mainplayer:getContainer();
	local item				= container:getItem( data["list"], data["grid"] );
	local nSrcTotalNum		= item:getNum();
	local nDestHaveNum		= self:getOneSameGridBaoShiNum( data );
	if nDestHaveNum == nSrcTotalNum then
		return;
	end
	
	local nEmptyBaoShiGridNum = self["MAX_BAO_SHI_NUM"] - self:getBaoShiNum();
	for i = 1, math.min( nSrcTotalNum - nDestHaveNum, nEmptyBaoShiGridNum ) do
		self:addBaoShi( data );
	end
end

t_BaoShiHeChengControl["addBaoShi"] =
function ( self, data )
	if self:isBaoShiFull() then
		ShowMidTips( "���ںϳɵı�ʯ�Ѵﵽ�������������ȡ��һ��" );
		return;
	end
	
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	for i = 1, table.getn( self["baoShiList"] ) do
		local oneBaoShiInfo = self["baoShiList"][i];
		if oneBaoShiInfo["list"] == -1 then
			local item = container:getItem( data["list"], data["grid"] );
			self["baoShiList"][i]["list"] = data["list"];
			self["baoShiList"][i]["grid"] = data["grid"];
			LockItem( item:getListType(), item:getGridIdx(), item:getItemId(), "BaoShiHeChengFrame" );
			--��Ҫ���ռ���ʯ�ϳɣ��ȼ�Ҫ���ݵ�һ������ı�ʯ�ȼ�����ʾ
			ChangeTitleAndGodCoinCost();
			--��Ҫ���ռ���ʯ�ϳɣ�����������������Ҫ�ı�����ʾ
			ChangeShowWhenVIPGodCoinCost();
			--�ռ���ʯ�ϳɷ�,�ı���ʾtitle
			ChangeTitleWhenFinalHeCheng()
			return;
		end
	end
end

t_BaoShiHeChengControl["setSuccessBaoShiInfo"] =
function ( self, data )
	self["successBaoShi"] = data;
end

t_BaoShiHeChengControl["removeBaoShi"] =
function ( self, data )
	local index			= data["index"];
	local oneBaoShi		= self["baoShiList"][index];
	local list			= oneBaoShi["list"];
	local grid			= oneBaoShi["grid"];
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local item			= container:getItem( list, grid );
	
	self["baoShiList"][index]["list"] = -1;
	self["baoShiList"][index]["grid"] = -1;
	
	local nDestHaveNum	= self:getOneSameGridBaoShiNum( { ["list"] = list, ["grid"] = grid } );
	if nDestHaveNum == 0 then
		UnLockItem( list, grid, item:getItemId() );
	end
	--��Ҫ���ռ���ʯ�ϳɣ��ȼ�Ҫ���ݵ�һ������ı�ʯ�ȼ�����ʾ
	ChangeTitleAndGodCoinCost();
	--�߼��������
	ChangeShowWhenVIPGodCoinCost()
	--�ռ���ʯ�ϳɷ�,�ı���ʾtitle
	ChangeTitleWhenFinalHeCheng()
end

t_BaoShiHeChengControl["isHeChengFu"] =
function ( self, data )
	-- ���жϴ���������Ʒ�Ƿ��Ǳ�ʯ�ϳɷ�
	--print( "id = "..data["id"] );
	local itemDef = getItemDef( data["id"] );

	for i = 1, MAX_ITEM_RESULT do
		local oneItemResult = itemDef.Result[i-1];
		if oneItemResult.ResultID == RESULT_GEM_LEVEL_UP_STUFF then
			return true;
		end
	end
	--print( "isHeChengFu--���Ǻϳɷ�id = "..data["id"] );
	return false;
end

t_BaoShiHeChengControl["canStartHeChengGem"] =
function ( self, data )
	-- ���жϴ���������Ʒ�Ƿ��Ǳ�ʯ�ϳɷ�
	if not self:isHeChengFu( data ) then
		return false;
	end
--[[
	-- ���ж�������ָ��NPC�ľ���	
	--local heChengNpcDefInfo = MapShower:getNpcInfoById( self["MAP_ID"], self["NPC_RES_ID"] );
	local mainplayer		= ActorMgr:getMainPlayer();
	local nCurPosX			= mainplayer:getPlayerX() / 10;
	local nCurPosY			= mainplayer:getPlayerZ() / 10;
	local mainPosPoint		= { x = nCurPosX, y = nCurPosY };
	--local npcPosPoint		= { x = heChengNpcDefInfo.m_nRealX, y = heChengNpcDefInfo.m_nRealY };
	local npcPosPoint		= { x = 11700, y = 36300 };
	local nDist				= util.CalPointsDistance( mainPosPoint, npcPosPoint );
	
	if nDist > ( self["MAX_DIST"] ) ^ 2 then
		--local nCoordX = math.ceil( heChengNpcDefInfo.m_nRealX / 100 );		
		--local nCoordY = math.ceil( heChengNpcDefInfo.m_nRealY / 100 );	
		local nCoordX = math.ceil( npcPosPoint["x"] / 100 );
		local nCoordY = math.ceil( npcPosPoint["y"] / 100 );	
		--ShowMidTips( "��������"..getMapName( self["MAP_ID"] ).."λ�ڣ� "..nCoordX.."�� "..nCoordY.."���ı�ʯ�������Բ��ܽ��б�ʯ�ϳ�" );
		ShowMidTips( "��������ʱ��ǹ���֮�ҵı�ʯ�������Ա߲��ܽ��б�ʯ�ϳ�" );
		return false;
	end
--]]

	return true;
end

t_BaoShiHeChengControl["startHeChengGem"] =
function ( self, data )
	local frame			= getglobal( "BaoShiHeChengFrame" );
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local item			= container:getItem( data["list"], data["grid"] );
	local itemDef		= item:getItemDef();
	
	if frame:IsShown() then
		local oldItem	= container:getItem( self["list"], self["grid"] );
		UnLockItem( oldItem:getListType(), oldItem:getGridIdx(), oldItem:getItemId() );
		self:clearHeChengInfo();

		for i = 1, MAX_ITEM_RESULT do
			local oneItemResult = itemDef.Result[i-1];
			if oneItemResult.ResultID == RESULT_GEM_LEVEL_UP_STUFF then
				self:setHeChengFuInfo( { list = data["list"], grid = data["grid"], baoShiLV = oneItemResult.ResultVal1, isFinalHeChengFu = oneItemResult.ResultVal2 } );
				break;
			end
		end

		UpdateBaoShiHeChengFrame();

	else
		for i = 1, MAX_ITEM_RESULT do
			local oneItemResult = itemDef.Result[i-1];
			if oneItemResult.ResultID == RESULT_GEM_LEVEL_UP_STUFF then
				self:setHeChengFuInfo( { list = data["list"], grid = data["grid"], baoShiLV = oneItemResult.ResultVal1, isFinalHeChengFu = oneItemResult.ResultVal2 } );
				break;
			end
		end
		frame:Show();
	end

	frame:Show();	
	-- �����Ʒ��ӳɹ�����ס����Ʒ
	LockItem( data["list"], data["grid"], item:getItemId(), "BaoShiHeChengFrame" );
end

t_BaoShiHeChengControl["endHengChengBaoShi"] = 
function ( self, data )
	self:clearHeChengInfo();
	ClearItemLock( "BaoShiHeChengFrame" );
end

t_BaoShiHeChengControl["clearHeChengBaoShiList"] =
function ( self )
	for i = 1, table.getn( self["baoShiList"] ) do
		self["baoShiList"][i]["list"] = -1;
		self["baoShiList"][i]["grid"] = -1;
	end
end

t_BaoShiHeChengControl["clearHeChengInfo"] =
function ( self )
	self["heChengFuID"]		= 0;
	self["baoShiLV"]		= -1;
	self["list"]			= NONE_LIST_TYPE;
	self["grid"]			= -1;
	self["successBaoShi"]	= { id = 0, };
	self:clearHeChengBaoShiList();
	GameArmProce:clear();
end

t_BaoShiHeChengControl["setHeChengFuInfo"] =
function ( self, data )
	self["baoShiLV"]	= data["baoShiLV"];
	self["list"]		= data["list"];
	self["grid"]		= data["grid"];
	self["isFinalHeChengFu"] = data["isFinalHeChengFu"];
end

function TryNextHeCheng( item )
	local mainplayer = ActorMgr:getMainPlayer()
	local container = mainplayer:getContainer()
	local itemDef = item:getItemDef()
	local gemNewDef = GameArmProce:getGemNewDef( itemDef.ItemID );
	if not gemNewDef then
		--���Ǳ�ʯ,���Ǻϳɷ�
		for i = 1, MAX_ITEM_RESULT do
			local oneItemResult = itemDef.Result[i-1];
			if oneItemResult.ResultID == RESULT_GEM_LEVEL_UP_STUFF then
				--ʹ�úϳɷ�
				t_BaoShiHeChengControl:startHeChengGem( { list = item:getListType(), grid = item:getGridIdx() } );
			end
		end
	else
		--����ǿɳ��ܵı�ʯ
		--���ұ����Ƿ��ж�Ӧ�ĺϳɷ�
		local heChengFuID = 0;
		if gemNewDef.Level == 1 then
			heChengFuID = 5219400
		elseif gemNewDef.Level == 2 then
			heChengFuID = 5219401
		elseif gemNewDef.Level < 7 then
			heChengFuID = 5219400 + gemNewDef.Level - 1;
		else
			return
		end
		local heChengFuItem = container:getItem( heChengFuID )
		if heChengFuItem ~= nil then		
			--ʹ�úϳɷ��������뱦ʯ
			t_BaoShiHeChengControl:startHeChengGem( { list = heChengFuItem:getListType(), grid = heChengFuItem:getGridIdx() } );
			if item:getNum() > 1 then
				t_BaoShiHeChengControl:addMultiBaoShi( { grid = item:getGridIdx(), list = item:getListType() } );
			else
				t_BaoShiHeChengControl:addBaoShi( { grid = item:getGridIdx(), list = item:getListType() } );
			end
		end
	end
end

function Save2BaoShiHeChengFrame( item, nIndex )
	if MessageBoxFrame:IsShown() and MessageTitle:GetText() == "��ʯ�ϳɰ���ʾ" then
		return;
	end
	--�����ʱ��һ�κϳɽ�������岢δ�ر�
	local nSuccessItemID = t_BaoShiHeChengControl["successBaoShi"]["id"];
	local baoShiLV = t_BaoShiHeChengControl["baoShiLV"]
	if BaoShiHeChengFrame:IsShown() and baoShiLV == -1 then
		TryNextHeCheng( item )
		
		UpdateBaoShiHeChengFrame();
		return
	end
	-- ���Ѿ�������ʯ�����������ͨ���Ҽ�׷�ӱ�ʯ
	if not t_BaoShiHeChengControl:canAddBaoShi( { grid = item:getGridIdx(), list = item:getListType(), showMsg = true } ) then
		return; 
	end
	--[[
	if t_BaoShiHeChengControl:isBaoShiFull() then
		ShowMidTips( "���ںϳɵı�ʯ�Ѵﵽ�������������ȡ��һ��" );
		return; 
	end
	--]]

	local itemDef		= item:getItemDef();
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	
	
	if nIndex ~= nil then		
		if item:getNum() > 1 then
		-- TODO
			t_BaoShiHeChengControl:setMultiBaoShi( { grid = item:getGridIdx(), list = item:getListType(), index = nIndex } );
		else
			t_BaoShiHeChengControl:setBaoShi( { grid = item:getGridIdx(), list = item:getListType(), index = nIndex } );
		end		
	else
		if item:getNum() > 1 then
		-- TODO
			t_BaoShiHeChengControl:addMultiBaoShi( { grid = item:getGridIdx(), list = item:getListType() } );
		else
			t_BaoShiHeChengControl:addBaoShi( { grid = item:getGridIdx(), list = item:getListType() } );
		end		
	end
	UpdateBaoShiHeChengFrame();
end

local MAX_HE_CHENG_BAO_SHI_NUM = 5;

local t_BaoShiHeChengFrameEffect = 
--effect\\entity\\fumo01.ent
{	[CLIENT_GEM_FAIL]		= { path = [[effect\entity\bshc02.ent]] }, 
	[CLIENT_GEM_SUCCESS]	= { path = [[effect\entity\bshc01.ent]] },
};
function GemLevelUp( nFlag, nSuccessItemID, nSuccessItemListType, nSuccessItemGirdIdx )
	-- TODO: ���Ƿ�Ҫ���
	t_BaoShiHeChengControl:endHengChengBaoShi();
	if nFlag == CLIENT_GEM_FAIL then	
		t_BaoShiHeChengControl:setSuccessBaoShiInfo( { id = 0, list = 0, grid = 0, } );
		if not BaoShiHeChengFrame_GodCoinBtn_CheckBtn:GetCheckState() then 
			UpdateHeChengFrame_BaoShiList();
		end
		ShowMidTips("��ʯ�ϳ�δ�ɹ������ٽ�����");
	else
		t_BaoShiHeChengControl:setSuccessBaoShiInfo( { id = nSuccessItemID, list = nSuccessItemListType, grid = nSuccessItemGirdIdx, } );
		UpdateHeChengFrame_BaoShiList();
	end

	--UpdateBaoShiHeChengFrame();
	UpdateBaoShiHeChengBtn();
	local modelView = getglobal( "BaoShiHeChengFrame_ResultBtnModelView" );
	modelView:setPetModel( 0, 0, t_BaoShiHeChengFrameEffect[nFlag]["path"] );
	modelView:setEntityPosition(0,0,100,0);
	modelView:Show();

	BaoShiHeChengFrame_BigCloseBtn:Show();
	BaoShiHeChengFrame_HeChengBtn:Hide();
end

function BaoShiHeChengFrame_OnLoad()
	--BaoShiHeChengFrame_EffectBackBtnIconTexture:SetGray( true );
end

function BaoShiHeChengFrame_OnEvent()
end

function BaoShiHeChengFrame_OnShow()
	if not PlayerContainerFrame:IsShown() then
		PlayerContainerFrame:Show()
	end
	PlayerContainerFrame:SetPoint("topleft", "UIClient", "topleft", 453, 163)
	this:SetPoint( "right", "PlayerContainerFrame", "left", 0, 0 );
	UpdateBaoShiHeChengFrame();
	
	BaoShiHeChengFrame_BigCloseBtn:Hide();
	BaoShiHeChengFrame_HeChengBtn:Show();
	-- ���շ������	
	ChangeTitleAndGodCoinCost()
	--�߼��������
	ChangeShowWhenVIPGodCoinCost()
	--�ռ���ʯ�ϳɷ�
	ChangeTitleWhenFinalHeCheng()
end

function ClearHeChengFrame_SuccessProbabilityList()
	for i = 1, MAX_HE_CHENG_BAO_SHI_NUM do
		local btn = getglobal( "BaoShiHeChengFrame_SuccessProbabilityLanBtn"..i );
		--btn:Hide();
		btn:Disable();
		
		local t_hideIcons = { btn:GetName().."NameFont", btn:GetName().."ValueFont", btn:GetName().."BackTexture", btn:GetName().."IconTexture" };
		util.HideIcons( t_hideIcons );
	end
	BaoShiHeChengFrame_GodCoinBtn_CheckBtn:Enable()
	BaoShiHeChengFrame_GodCoinBtnFont:SetTextColor(255,255,0)
end
function BaoShiHeChengFrame_GodCoinBtn_OnEnter()
	SetGameTooltips(this:GetName(), "���ѽ�ҹ����պ󣬺ϳ�ʧ�ܺ���ʧ����ϳɵı�ʯ\n��ʯ�����ʴﵽ100%������ѡ��ѡ��")
end
function ChangeTitleAndGodCoinCost()
	local t_BaoShiTitleDes = {"һ��","����","����","�ļ�","�弶","����","�߼�","�˼�",}
	local t_BaoShiGodCoinCost = { 	
									[1] = ONE_GEM_HECHENG_GODCOIN_TOLUA,	
									[2] = TWO_GEM_HECHENG_GODCOIN_TOLUA,	
									[3] = THREE_GEM_HECHENG_GODCOIN_TOLUA,	
									[4] = FOUR_GEM_HECHENG_GODCOIN_TOLUA,	
									[5] = FIVE_GEM_HECHENG_GODCOIN_TOLUA,
									[6] = SIX_GEM_HECHENG_GODCOIN_TOLUA,	
									[7] = SEVEN_GEM_HECHENG_GODCOIN_TOLUA,
									[8] = EIGHT_GEM_HECHENG_GODCOIN_TOLUA,
								 }
									
	local baoShiLV = t_BaoShiHeChengControl["baoShiLV"];
	local titleFont 	= getglobal("BaoShiHeChengFrameTitleFont")
	local godCoinFont 	= getglobal("BaoShiHeChengFrame_GodCoinBtnFont");
	local godCoinTex	= getglobal("BaoShiHeChengFrame_GodCoinBtnTex")
	if baoShiLV < 1 then 
		titleFont:SetText("��ʯ�ϳ�")
		godCoinFont:SetText("�����գ�0")
	else 
		titleFont:SetText(t_BaoShiTitleDes[baoShiLV].."��ʯ�ϳ�")
		godCoinFont:SetText("�����գ�"..t_BaoShiGodCoinCost[baoShiLV])
	end
	local xPoint = godCoinFont:GetTextExtentWidth( godCoinFont:GetText() ) / GetScreenScaleY() + 1
	godCoinTex:SetPoint( "topleft", "BaoShiHeChengFrame_GodCoinBtnFont", "topleft",xPoint  , -4 );
	BaoShiHeChengFrame_GodCoinBtn_CheckBtn:setCheckState(false)
end

function UpdateHeChengFrame_SuccessProbabilityList()
	ClearHeChengFrame_SuccessProbabilityList();
	-- [color][GemType]
--	local t_gaiLvList		= { [0] = {}, [1] = {}, [2] = {}, [3] = {}, };
	local t_baoShiList		= t_BaoShiHeChengControl["baoShiList"];
	local mainplayer		= ActorMgr:getMainPlayer();
	local container			= mainplayer:getContainer();
	local oneAddProbability = 20;
	for i = 1, table.getn( t_baoShiList ) do
		local oneBaoShi = t_baoShiList[i];
		local list		= oneBaoShi["list"];
		local grid		= oneBaoShi["grid"];
		if list ~= -1 then
			local item = container:getItem( list, grid );
			local gemNewDef = GameArmProce:getGemNewDef( item:getItemId() ); 
--			if t_gaiLvList[gemNewDef.GemColor][gemNewDef.GemType] == nil then
--				t_gaiLvList[gemNewDef.GemColor][gemNewDef.GemType]		= {};
--				t_gaiLvList[gemNewDef.GemColor][gemNewDef.GemType]["num"] = 1;
--			else	
--				t_gaiLvList[gemNewDef.GemColor][gemNewDef.GemType]["num"] = t_gaiLvList[gemDef.GemColor][gemDef.GemType]["num"] + 1;
--			end
		end
	end
	
	local isHaveId = 
	function ( list, id )
		for _, elem in ipairs( list ) do
			if id  == elem["ID"] then
				return true;
			end
		end

		return false;
	end

	local t_infoList = {};
	for i = 1, table.getn( t_baoShiList ) do
		local oneBaoShi		= t_baoShiList[i];
		local list			= oneBaoShi["list"];
		local grid			= oneBaoShi["grid"];
		if list ~= -1 then
			local item	= container:getItem( list, grid );
			local id	= item:getItemId();
			local oneInfo = { ["ID"] = id, ["num"] = 1, };
			if not isHaveId( t_infoList, id ) then
				table.insert( t_infoList, oneInfo );
			else
				for i = 1, table.getn( t_infoList ) do
					if t_infoList[i]["ID"] == id then
						t_infoList[i]["num"] = t_infoList[i]["num"] + 1;
					end
				end
			end
		end
	end


	local nBtnIndex = 1;
	local nTotalRate =0;
	for i = 1, table.getn( t_infoList ) do
		local id		= t_infoList[i]["ID"];
		local itemDef	= getItemDef( id );
		local gemNewDef	= GameArmProce:getGemNewDef( id ); 
		local btn		= getglobal( "BaoShiHeChengFrame_SuccessProbabilityLanBtn"..nBtnIndex );
		local szPath	= GetItemEquipIconPath()..itemDef.IconID..".tga";
		if not IsInExistence( szPath ) then
			szPath = DEFAULT_ITEM_PATH;
		end

		local t_tgas = { [btn:GetName().."IconTexture"] = { path = szPath }, };
		util.SetTgas( t_tgas );
		
		local nextGemNewDef	= GameArmProce:getGemNewDef( gemNewDef.UpID );
		local nextGemItemDef= getItemDef( nextGemNewDef.ItemID );
		nTotalRate = nTotalRate + gemNewDef.UpRate / 100 * t_infoList[i]["num"]
		local szText		= nextGemNewDef ~= nil and nextGemItemDef.Name or "��ʯ�ȼ��ѵ���ߣ��޷������ϳ�";
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = szText }, 
							[btn:GetName().."ValueFont"]	= 
							{ value = ( gemNewDef.UpRate / 100 * t_infoList[i]["num"] ).."%" },
						--	{ value = (20*t_infoList[i]["num"]).."%" },
						};
		util.SetFonts( t_fonts );
		btn:Enable();
		--btn:Show();
		local t_showIcons = { btn:GetName().."NameFont", btn:GetName().."ValueFont", btn:GetName().."BackTexture", btn:GetName().."IconTexture" };
		util.ShowIcons( t_showIcons );
		nBtnIndex = nBtnIndex + 1
	end
	
	if table.getn( t_infoList ) > 0 then 
		BaoShiHeChengFrame_GodCoinBtn:Show();
	else 
		BaoShiHeChengFrame_GodCoinBtn:Hide();
	end
	nTotalRate = nTotalRate > 100 and 100 or nTotalRate
	BaoShiHeChengFrameSuccessProbabilityHeadBtnFont:SetText("��ʯ�����ʣ�"..nTotalRate.."%��")
	if nTotalRate >= 100 then 
		BaoShiHeChengFrame_GodCoinBtn_CheckBtn:setCheckState(false)
		BaoShiHeChengFrame_GodCoinBtn_CheckBtn:Disable()
		BaoShiHeChengFrame_GodCoinBtnFont:SetTextColor(110,110,110)
	end
end

function ClearHeChengFrame_BaoShiList()
	for i = 1, ( MAX_HE_CHENG_BAO_SHI_NUM ) do
		local btn = getglobal( "BaoShiHeChengFrame_BaoShiGridBtn"..i );
		btn:SetClientUserData( 0, 0 );
		btn:SetClientUserData( 1, 0 );
		btn:SetClientUserData( 2, 0 );
		local t_tgas = { [btn:GetName().."IconTexture"] = { path = "uires\\ui\\mask\\TouMingTongDao.tga" }, };
		util.SetTgas( t_tgas );
		SetEquipImportantEffectUV( nil, btn:GetName().."UVAnimationTex" );
	end

	local btn = getglobal( "BaoShiHeChengFrame_CenterGridBtn" );
	btn:SetClientUserData( 0, 0 );
	btn:SetClientUserData( 1, 0 );
	btn:SetClientUserData( 2, 0 );
	local t_tgas = { [btn:GetName().."IconTexture"] = { path = "uires\\ui\\mask\\TouMingTongDao.tga" },  };
	util.SetTgas( t_tgas );
	SetEquipImportantEffectUV( nil, btn:GetName().."UVAnimationTex" );
	BaoShiHeChengFrame_GodCoinBtn_CheckBtn:setCheckState(false)
end

function UpdateHeChengFrame_BaoShiList()
	ClearHeChengFrame_BaoShiList();
	local t_baoShiList = t_BaoShiHeChengControl["baoShiList"];
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	
	for i = 1, table.getn( t_baoShiList ) do
		local oneBaoShi = t_baoShiList[i];
		local list = oneBaoShi["list"];
		local grid = oneBaoShi["grid"];
		if list ~= - 1 then
			local item			= container:getItem( list, grid );
			local itemDef		= item:getItemDef();
			local btn			= getglobal( "BaoShiHeChengFrame_BaoShiGridBtn"..i );
			btn:SetClientUserData( 0, itemDef.ItemID );
			btn:SetClientUserData( 1, list );
			btn:SetClientUserData( 2, grid );

			local szPath		= GetItemEquipIconPath()..itemDef.IconID..".tga";
			if not IsInExistence( szPath ) then
				szPath = DEFAULT_ITEM_PATH;
			end
	
			SetEquipImportantEffectUV( itemDef, btn:GetName().."UVAnimationTex" );
			local t_tgas = { [btn:GetName().."IconTexture"] = { path = szPath }, };
			util.SetTgas( t_tgas );
		end
	end

	local nSuccessItemID = t_BaoShiHeChengControl["successBaoShi"]["id"];
	if nSuccessItemID ~= 0 then
		local successItemDef	= getItemDef( nSuccessItemID );
		local szPath			= GetItemEquipIconPath()..successItemDef.IconID..".tga";
		if not IsInExistence( szPath ) then
			szPath = DEFAULT_ITEM_PATH;
		end
		
		local btn = getglobal( "BaoShiHeChengFrame_CenterGridBtn" );
		btn:SetClientUserData( 0, nSuccessItemID );
		btn:SetClientUserData( 1, t_BaoShiHeChengControl["successBaoShi"]["list"] );
		btn:SetClientUserData( 2, t_BaoShiHeChengControl["successBaoShi"]["grid"] );
		SetEquipImportantEffectUV( successItemDef, "BaoShiHeChengFrame_CenterGridBtnUVAnimationTex" );
		local t_tgas = { ["BaoShiHeChengFrame_CenterGridBtnIconTexture"] = { path = szPath }, };
		util.SetTgas( t_tgas );
	end
end

function UpdateBaoShiHeChengBtn()
	-- ���ñ�ʯ
	local heChengBtn	= getglobal( "BaoShiHeChengFrame_HeChengBtn" );
	if t_BaoShiHeChengControl:getBaoShiNum() < 2 then
		heChengBtn:Disable();
	else
		heChengBtn:Enable();
	end
end

function UpdateBaoShiHeChengFrame()
	UpdateHeChengFrame_SuccessProbabilityList();
	UpdateHeChengFrame_BaoShiList();
	UpdateBaoShiHeChengBtn();
end

function BaoShiHeChengFrame_OnHide()
	if MessageBoxFrame:IsShown() and MessageTitle:GetText() == "��ʯ�ϳɰ���ʾ" then
		MessageBoxFrame:Hide();
	end
	t_BaoShiHeChengControl:endHengChengBaoShi();
	local modelView = getglobal( "BaoShiHeChengFrame_ResultBtnModelView" );
	modelView:Hide();
end

function BaoShiHeChengFrame_OnUpdate()
	t_BaoShiHeChengControl:update();
	--[[
	if t_BaoShiHeChengControl["list"] == -1 then
		local modelView = getglobal( "BaoShiHeChengFrame_ResultBtnModelView" );
		if not modelView:isPlayingAnim( 0 ) then
			this:Hide();
		end
	end
	--]]
end

function BaoShiHeChengFrame_HeChengBtn_Test_OnClick()
	local modelView = getglobal( "BaoShiHeChengFrame_ResultBtnModelView" );
	modelView:setPetModel( 0, 0, t_BaoShiHeChengFrameEffect[0]["path"] );
	--modelView:setPetModel( 0, 0, "effect\\entity\\fumo01.ent" );
	
	modelView:setEntityPosition(0,0,100,0);
	modelView:Show();
end

function BaoShiHeChengFrame_Test_OnShow()
	local modelView = getglobal( "BaoShiHeChengFrame_ResultBtnModelView" );
	modelView:setPetModel( 0, 0, t_BaoShiHeChengFrameEffect[0]["path"] );
	--modelView:setPetModel( 0, 0, "effect\\entity\\fumo01.ent" );
	
	modelView:setEntityPosition(0,0,100,0);
	modelView:Show();
end

-- �ϳɰ�ť
function BaoShiHeChengFrame_HeChengBtn_OnClick()
	local t_baoShiList	= t_BaoShiHeChengControl["baoShiList"];
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	if container:getContainerLockState() == CONTAINER_LOCK_STATE_LOCK then
		ShowMidTips( "�����������޷��ϳ�" )
		return
	end
	if container:getUnusedGridNum(CONTAINER_TYPE_SUNDRIES) == 0 then
		ShowMidTips("�����ռ䲻�㣬����������");
		return
	end
	GameArmProce:clear();
	local IsGemOrHeChengFuBind = false;
	for i = 1, table.getn( t_baoShiList ) do
		local oneBaoShi		= t_baoShiList[i];
		local list			= oneBaoShi["list"];
		local grid			= oneBaoShi["grid"];
		if list ~= -1 then
			local item	= container:getItem( list, grid );
			if item:isItemBind() then
				IsGemOrHeChengFuBind = true;
			end
			GameArmProce:addStuff( item, i - 1, STUFF_LEVELGEM );
		end
	end
	
	local heChengFuItem = container:getItem( t_BaoShiHeChengControl["list"], t_BaoShiHeChengControl["grid"] );
	if heChengFuItem:isItemBind() then
		IsGemOrHeChengFuBind = true;
	end
	local nType = 0;
	local checkBtn = getglobal("BaoShiHeChengFrame_GodCoinBtn_CheckBtn")
	if checkBtn:GetCheckState() then 
		nType = 1
	elseif BaoShiHeChengFrame_VIPGodCoinBtn_CheckBtn:GetCheckState() then
		nType = 2
	end
	if IsGemOrHeChengFuBind  then
		MessageBox( "��ʯ�ϳɰ���ʾ", "ʹ�ð󶨵ı�ʯ����������ʹ�ϳɵı�ʯ�󶨣�ȷ�������ϳɣ�" );
		MessageBoxFrame:SetClientUserData( 0, t_BaoShiHeChengControl["list"] );
		MessageBoxFrame:SetClientUserData( 1, t_BaoShiHeChengControl["grid"] );
		MessageBoxFrame:SetClientUserData( 2, nType );
		return;
	end
	GameArmProce:gemLevelUp( heChengFuItem, nType);
end

-- �����������뱳��
function BaoShiHeChengOnEnter( szBtnName )
	local btn			= getglobal( szBtnName );
	local mainplayer	= ActorMgr:getMainPlayer();
	if mainplayer == nil then
		return;
	end
	local container		= mainplayer:getContainer();
	local id			= btn:GetClientUserData( 0 );
	local list			= btn:GetClientUserData( 1 );
	local grid			= btn:GetClientUserData( 2 );
	local item			= container:getItem( list, grid );
	if item:getItemId() <= 0 then
		local t_hideIcons = { "NormalItemTipsFrame", "ArmItemTipsFrame", "SecondArmItemTipsFrame", };
		util.HideIcons( t_hideIcons );
		return;
	end
	
	ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item, BaoShiHeChengOnEnter, szBtnName);
end

function BaoShiHeChengFrame_HeChengTiShiButton_OnEnter()
	local szText = "1�� ��ʯ�ϳɻ�ѵͽױ�ʯ�ϳɸ߽ױ�ʯ\n2�� ֻ�ܷ����뱦ʯ��ͬ�ȼ��ı�ʯ\n3�� ����Ҫ���ű�ʯ���ܿ�ʼ�ϳ�\n4�� ������ʯ�������������μӺϳɵı�ʯ�й� ";
	SetGameTooltips(this:GetName(), szText );
end

function BaoShiHeChengFrame_BaoShiGridBtn_OnEnter()
	local id = this:GetClientUserData( 0 );
	if id == 0 then
		return;
	end

	BaoShiHeChengOnEnter( this:GetName() );
end

function BaoShiHeChengFrame_BaoShiGridBtn_OnLeave()
	local t_hideIcons = { "NormalItemTipsFrame", "ArmItemTipsFrame", "SecondArmItemTipsFrame", "GameTooltip", "SpellTipsFrame" };
	util.HideIcons( t_hideIcons );
end

function BaoShiHeChengFrame_BaoShiGridBtn_Drag()
end

function BaoShiHeChengFrame_BaoShiGridBtn_ReceiveDrag()
	if MessageBoxFrame:IsShown() and MessageTitle:GetText() == "��ʯ�ϳɰ���ʾ" then
		return;
	end
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
	--if t_BaoShiHeChengControl:canAddBaoShi( { grid = item:getGridIdx(), list = item:getListType(), showMsg = true } ) then
	Save2BaoShiHeChengFrame( item, this:GetClientID() );
	UIEndDrag();
	UIMSG:getUIGV().DragFrom = DRAG_NONE;
	--end
end

function BaoShiHeChengFrame_BaoShiGridBtn_OnClick()
	if MessageBoxFrame:IsShown() and MessageTitle:GetText() == "��ʯ�ϳɰ���ʾ" then
		return;
	end
	if arg1 ~= "RightButton" or this:GetClientID() == 6 then
		return;
	end
	
	local id = this:GetClientUserData( 0 );
	if id == 0 then
		return;
	end

	t_BaoShiHeChengControl:removeBaoShi( { index = this:GetClientID() } );
	UpdateBaoShiHeChengFrame();
end
--�߼����գ����Ը߼����յı�ʯ��ĿǰΪ5��6��7��8����ʯ
function BaoShiHeChengFrame_VIPGodCoinBtn_OnEnter()	
	SetGameTooltips(this:GetName().."Tex", "���ѽ�ҹ���߼����պ�ϳ�100%�ɹ�\n����5�ű�ʯʱ�����й���߼�����")
end

function ChangeShowWhenVIPGodCoinCost()
	local t_BaoShiTitleDes = {"һ��","����","����","�ļ�","�弶","����","�߼�","�˼�",}
	local t_BaoShiVIPGodCoinCost = { 	
									[1] = 0,	
									[2] = 0,	
									[3] = 0,	
									[4] = 0,	
									[5] = FIVE_GEM_HECHENG_GODCOIN_GJ_TOLUA,
									[6] = SIX_GEM_HECHENG_GODCOIN_GJ_TOLUA,
									[7] = SEVEN_GEM_HECHENG_GODCOIN_GJ_TOLUA,
									[8] = EIGHT_GEM_HECHENG_GODCOIN_GJ_TOLUA,
								 }
									
	local baoShiLV = t_BaoShiHeChengControl["baoShiLV"];
	local titleFont 	= getglobal("BaoShiHeChengFrameTitleFont")
	local godCoinFont 	= getglobal("BaoShiHeChengFrame_VIPGodCoinBtnFont");
	local godCoinTex	= getglobal("BaoShiHeChengFrame_VIPGodCoinBtnTex")
	--��ֵ�ı�ʾ���Ը߼�����
	if t_BaoShiVIPGodCoinCost[baoShiLV] > 0 and t_BaoShiHeChengControl:getBaoShiNum() == 5 then
		godCoinFont:SetText("�߼����գ�"..t_BaoShiVIPGodCoinCost[baoShiLV].."      (100%�ɹ�)")
		BaoShiHeChengFrame_VIPGodCoinBtn:Show()
	else
		BaoShiHeChengFrame_VIPGodCoinBtn:Hide()
	end
	local xPoint = (godCoinFont:GetTextExtentWidth( "�߼����գ�"..t_BaoShiVIPGodCoinCost[baoShiLV] ) ) / GetScreenScaleY() + 1
	godCoinTex:SetPoint( "topleft", "BaoShiHeChengFrame_VIPGodCoinBtnFont", "topleft",xPoint  , -4 );
	BaoShiHeChengFrame_VIPGodCoinBtn_CheckBtn:setCheckState(false)
end
--�ռ���ʯ�ϳɷ�
function ChangeTitleWhenFinalHeCheng()
	if t_BaoShiHeChengControl["isFinalHeChengFu"] == 1 then
		local titleFont 	= getglobal("BaoShiHeChengFrameTitleFont")
		titleFont:SetText( "�ռ���ʯ�ϳ�" )
	end
end
function BaoShiHeChengGodCostSelect()
	--��ͨ���պ͸߼�����ͬʱֻ��ѡһ��
	if this:GetName() == "BaoShiHeChengFrame_GodCoinBtn_CheckBtn" then
		if BaoShiHeChengFrame_GodCoinBtn_CheckBtn:GetCheckState() then
			BaoShiHeChengFrame_VIPGodCoinBtn_CheckBtn:setCheckState(false)
		end
	else
		if BaoShiHeChengFrame_VIPGodCoinBtn_CheckBtn:GetCheckState() then
			BaoShiHeChengFrame_GodCoinBtn_CheckBtn:setCheckState(false)
		end
	end
end
----------------------------------------------------------------------------------------------------------------------------------------------------------------

function BaoShiXiangQianFrameResetBtn_onClick()
	if GemProceResetFrame:IsShown() and this:IsChecked() then
		GemProceResetFrame:Hide();
		return;
	end	
	GemProceResetFrame:Show();
	GemProceChargeFrame:Hide();
	GemProcePickFrame:Hide();
	local nIndex = this:GetClientID();
	local equipItem	= t_BaoShiXiangQianFrameControl:getXiangQianEquipItem();
	local itemId = equipItem:getHoleGemId( nIndex - 1 );
	local list = equipItem:getListType();
	local grid = equipItem:getGridIdx();
	if itemId > 0 then
		setResetItemInfo( itemId, nIndex, list, grid );
	end
	for i = 1, MAX_XIANG_QIAN_KONG_NUM do
		local resetBtn = getglobal( "BaoShiXiangQianFrame_BaoShiResetBtn"..i );
		resetBtn:DisChecked();
		if i == nIndex then
			resetBtn:Checked();
		end
	end
end
function BaoShiXiangQianFrameResetBtn_onEnter()
	SetGameTooltips(this:GetName(), "��ʯ����" );
end
function BaoShiXiangQianFrameResetBtn_onLeave()
	GameTooltip:Hide();
end
function BaoShiXiangQianFrameChargeBtn_onClick()
	local nIndex = this:GetClientID();
	local equipItem = t_BaoShiXiangQianFrameControl:getXiangQianEquipItem();
	local itemId = equipItem:getHoleGemId( nIndex - 1 );
	local EquipListType = equipItem:getListType();
	local EquipGridIndex = equipItem:getGridIdx();
	if itemId > 0 then
		GemProceChargeFrame:Hide();
		setChargeInfo( itemId, nIndex,EquipListType, EquipGridIndex );
	end
	GemProceChargeFrame:Show();
	GemProceResetFrame:Hide();
	GemProcePickFrame:Hide();
end
function BaoShiXiangQianFrameChargeBtn_onEnter()
	SetGameTooltips(this:GetName(), "��ʯ����" );
end
function BaoShiXiangQianFrameChargeBtn_onLeave()
	GameTooltip:Hide();
end
function BaoShiXiangQianFramePickBtn_onClick()	
	local nIndex = this:GetClientID();
	local equipItem	= t_BaoShiXiangQianFrameControl:getXiangQianEquipItem();
	if equipItem:getHoleGemId( nIndex - 1 ) > 0 then
		GemProcePickFrame:SetClientUserData( 0,this:GetClientUserData(0) )
		GemProcePickFrame:SetClientUserData( 1,nIndex );
		GemProcePickFrame:SetClientUserData( 2,t_BaoShiXiangQianFrameControl["equipInfo"]["grid"] );
		GemProcePickFrame:SetClientUserData( 3,t_BaoShiXiangQianFrameControl["equipInfo"]["list"] );
		GemProcePickFrame:Show();
		GemProceChargeFrame:Hide();
		GemProceResetFrame:Hide();
	end
end
function BaoShiXiangQianFramePickBtn_onEnter()
	SetGameTooltips(this:GetName(), "ժȡ��ʯ" );
end
function BaoShiXiangQianFramePickBtn_onLeave()
	GameTooltip:Hide();
end

function BaoShiXiangQianFrameEquipShowBtn_onEnter()
	local ItemID = this:GetClientUserData( 0 );
	local ItemDef = getItemDef( ItemID );
	local equipItem	= t_BaoShiXiangQianFrameControl:getXiangQianEquipItem();
	ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, equipItem, nil, this:GetName() );
end
function BaoShiXiangQianFrameEquipShowBtn_onLeave()
	NormalItemTipsFrame:Hide();
end

function SetBaoshiToolsGray( nIndex, isGray )
	local resetBtn = getglobal("BaoShiXiangQianFrame_BaoShiResetBtn"..nIndex);
	local chargeBtn = getglobal("BaoShiXiangQianFrame_BaoShiChargeBtn"..nIndex);
	local pickBtn = getglobal("BaoShiXiangQianFrame_BaoShiPickBtn"..nIndex);
	resetBtn:SetGrayEx( isGray );
	chargeBtn:SetGrayEx( isGray );
	pickBtn:SetGrayEx( isGray );
	if isGray == true then
		resetBtn:Disable();
		chargeBtn:Disable();
		pickBtn:Disable();
	elseif isGray == false then
		resetBtn:Enable();
		chargeBtn:Enable();
		pickBtn:Enable();
	end
end

function ShowBaoShiTools( nIndex)
	local resetBtn = getglobal("BaoShiXiangQianFrame_BaoShiResetBtn"..nIndex);
	local chargeBtn = getglobal("BaoShiXiangQianFrame_BaoShiChargeBtn"..nIndex);
	local pickBtn = getglobal("BaoShiXiangQianFrame_BaoShiPickBtn"..nIndex);
	resetBtn:Show();
	chargeBtn:Show();
	pickBtn:Show();
end
function HideBaoShiTools( nIndex)
	local resetBtn = getglobal("BaoShiXiangQianFrame_BaoShiResetBtn"..nIndex);
	local chargeBtn = getglobal("BaoShiXiangQianFrame_BaoShiChargeBtn"..nIndex);
	local pickBtn = getglobal("BaoShiXiangQianFrame_BaoShiPickBtn"..nIndex);
	resetBtn:Hide();
	chargeBtn:Hide();
	pickBtn:Hide();
end