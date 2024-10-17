local tData = {}

local function ClearBaoXiang()
	BaoXiangFrameGetItemBtn:SetClientUserData( 0, 0 )
	BaoXiangFrameGetItemBtn:SetClientUserData( 1, 0 )
	BaoXiangFrameGetItemBtnIconTexture:SetTexture( "ui\\mask\\TouMingTongDao.tga" )
	BaoXiangFrameGetItemNum:SetText( "" )
	--[[
	BaoXiangFrameHideItemBtn:SetClientUserData( 0, 0 )
	BaoXiangFrameHideItemBtnIconTexture:SetTexture( "ui\\mask\\TouMingTongDao.tga" )
	--]]
	BaoXiangFrameHideItemNum:SetText( "" )
	for i = 1, MAX_BAOXIANG_DISPLAY_ITEM_TOLUA do
		local btn = getglobal( "BaoXiangFrameShowItemBtn"..i )
		btn:Hide()
	end
end
--检测是否可以开启下一次宝箱
local function CanOpenNextBaoXiang( baoXiangDef )
	if baoXiangDef == nil then
		return false
	end
	local mainplayer = ActorMgr:getMainPlayer()
	local container = mainplayer:getContainer()
	if baoXiangDef.NeedItemID == 0 then
		return container:getItemCount( tData.BaoXiangItemID ) > 0
	else
		if container:getItemCount( tData.BaoXiangItemID ) > 0 then
			return container:getItemCount( baoXiangDef.NeedItemID ) > 0
		else
			return false
		end
	end
end

local function UpdateBaoXiang()
	local mainplayer = ActorMgr:getMainPlayer()
	local container = mainplayer:getContainer()
	local item = container:getItem( tData.ItemWID )
	if item == nil then
		item = container:getItem( tData.ItemID )
	end
	local itemDef = getItemDef( tData.BaoXiangItemID )
	local baoXiangDef = nil
	if itemDef ~= nil and CheckItemResult( itemDef, RESULT_BAOXIANG ) then
		baoXiangDef = getBaoXiangDef( itemDef.Result[0].ResultVal1 )

		if item ~= nil and item:getItemId() == tData.ItemID then
			local btn = getglobal( "BaoXiangFrameGetItemBtn" )
			local tex = getglobal( btn:GetName() .. "IconTexture" )
			local backTex = getglobal( btn:GetName() .. "BackTexture" )
			backTex:Show()
			local itemdef = item:getItemDef()
			local IconPath = GetItemEquipIconPath().. itemdef.IconID..".tga";
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			SetEquipImportantEffectUV( itemdef, btn:GetName() .. "UVAnimationTex",item )

			tex:SetTexture( IconPath );
			local num = 0
			for i=1,MAX_BAOXIANG_ITEM_TOLUA do
				if baoXiangDef.Items[i-1].ItemID == itemdef.ItemID then
					num = baoXiangDef.Items[i-1].Num
					break
				end
			end
			if num == 0 and baoXiangDef.HideItemID == itemdef.ItemID then
				num = baoXiangDef.HideItemNum
			end
			--如果协议有发来数目，则显示为协议里的数据
			if tData.Num ~= 0 then
				num = tData.Num
			end
			BaoXiangFrameGetItemNum:SetText( "x " ..  num )

			btn:SetClientUserData( 0, item:getListType() )
			btn:SetClientUserData( 1, item:getGridIdx() )
		end
	
		BaoXiangFrameTitle:SetText( itemDef.Name )
		BaoXiangFrameDescFont2:SetText( itemDef.Name .. "中，有几率获得以下稀有东东哦！" )
		--设置有几率获得的物品
		local t_ShowItems = {}		
		for i = 1, MAX_BAOXIANG_DISPLAY_ITEM_TOLUA do
			if baoXiangDef.ShowItems[ i - 1 ].Num > 0 and baoXiangDef.ShowItems[ i - 1 ].ItemID > 0 then
				table.insert( t_ShowItems, { baoXiangDef.ShowItems[ i - 1 ].ItemID, baoXiangDef.ShowItems[ i - 1 ].Num } )
			end 
		end
		local offsetX = (264-44*table.getn( t_ShowItems ))/2
		for i = 1, table.getn( t_ShowItems ) do
			local btn = getglobal( "BaoXiangFrameShowItemBtn"..i )
			btn:Show()			
			local tex = getglobal( btn:GetName() .. "IconTexture" )
			local backTex = getglobal( btn:GetName() .. "BackTexture" )
			local countFont = getglobal( btn:GetName() .. "CountFont" )
			backTex:Show()
			itemDef = getItemDef( t_ShowItems[i][1] )
			countFont:SetText( (t_ShowItems[i][2]>1 and t_ShowItems[i][2] or "") )
			local IconPath = GetItemEquipIconPath().. itemDef.IconID..".tga";
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			tex:SetTexture( IconPath )
			SetEquipImportantEffectUV( itemDef, btn:GetName() .. "UVAnimationTex" )
			--BaoXiangFrameHideItemNum:SetText( "x " .. baoXiangDef.HideItemNum )
			btn:SetClientUserData( 0, t_ShowItems[i][1] )
			btn:SetPoint( "topleft", "BaoXiangFrame", "topleft", 45+offsetX+(i-1)*44, 241 )
		end
	end

	if CanOpenNextBaoXiang( baoXiangDef ) then
		BaoXiangFrameOkBtn:Enable()
	else
		BaoXiangFrameOkBtn:Disable()
	end
end

local function PlaySucessEffect()
	local modelview = getglobal( "BaoXiangFrameModelView" )
	modelview:setPetModel(0, 0, "effect\\entity\\fumo01.ent")
	modelview:setEntityPosition(0, 0, 100, 0)
end

function BaoXiangFrame_OnLoad()
	this:RegisterEvent( "GE_BAOXIANG_OPEN" )
	this:RegisterEvent( "GE_CONTAINER_UPDATE" )
end

function BaoXiangFrame_OnEvent()
	if arg1 == "GE_BAOXIANG_OPEN" then
		local msg = UIMSG:getUIMsg()
		tData.ItemID = msg.BaoXiangAwardInfo.ItemID
		tData.ItemWID = msg.BaoXiangAwardInfo.ItemWID
		tData.BaoXiangItemID = msg.BaoXiangAwardInfo.BaoXiangItemID
		tData.Num = msg.BaoXiangAwardInfo.Num
		BaoXiangFrame:Show()
	elseif arg1 == "GE_CONTAINER_UPDATE" and this:IsShown() then
		UpdateBaoXiang()
	end
end

function BaoXiangFrame_OnShow()
	if PlayerContainerFrame:IsShown() then
		this:SetPoint( "topright","PlayerContainerFrame","topleft", 0, 0 )
	end
	ClearBaoXiang()
	UpdateBaoXiang()
	PlaySucessEffect()
end

function BaoXiangFrameGetItemBtn_OnEnter()
	local mainplayer = ActorMgr:getMainPlayer()
	local container = mainplayer:getContainer()
	local item = container:getItem( this:GetClientUserData(0), this:GetClientUserData(1) )
	ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item );
end

function BaoXiangFrameShowItemBtn_OnEnter()
	ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, getItemDef( this:GetClientUserData(0) ) );
end

function SetCanOpenBaoXiangInfo( itemDef )
	local baoXiangDef = getBaoXiangDef( itemDef.Result[0].ResultVal1 )
	local mainplayer = ActorMgr:getMainPlayer()
	local container = mainplayer:getContainer()
	if baoXiangDef.NeedItemID > 0 then
		local needItemDef = getItemDef( baoXiangDef.NeedItemID )
		return "再开一个" .. itemDef.Name .. "，背包还有:\n".. itemDef.Name .. container:getItemCount(tData.BaoXiangItemID) .."个\n"..needItemDef.Name..container:getItemCount(tData.BaoXiangItemID).."个"
	else
		return "再开一个" .. itemDef.Name .. "，背包还有" .. container:getItemCount(tData.BaoXiangItemID) .."个。"
	end
end

function BaoXiangFrameOkBtn_OnEnter()
	local itemDef = getItemDef( tData.BaoXiangItemID )
	SetGameTooltips(this:GetName(), SetCanOpenBaoXiangInfo( itemDef ) );
end

function BaoXiangFrameOkBtn_OnLeave()
	GameTooltipOnLeave()
end

function BaoXiangFrameOkBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer()
	local container = mainplayer:getContainer()
	local item = container:getItem( tData.BaoXiangItemID )
	if mainplayer:isDead() then
		ShowMidTips("你已经死亡");
		return;
	end
	if item:getItemId() > 0 then
		mainplayer:RequestOpenBaoXiang( item:getListType(), item:getGridIdx() )
	end
end

-----------------------------------------抽卡宝箱-------------------------------------------
local MAX_CARD_BOX = 8;
local MAX_CARD_BOX_EXTEND = 16;
local TOTAL_SELECT_TIME = 4;
local TOTAL_WASH_TIME = 3;
local ROLL_STEP_VALUE = 3;
local CARD_BOX_FRAME_HIGH = 27;
local selIdx = 0;
local t_ResultIdx = {};
local bUpdate = false;
local bTurn = false;
local bWash = false;
local cardData = {};
local selectData = {};
local bShowMsg = true;

function UseCardBoxItem( nGridIdx, nListType )
	if CardBoxFrame:IsShown() or CardBoxExtendFrame:IsShown() then
		return ShowMidTips("正在抽奖中，本次抽奖完毕后才能再次开启抽奖功能！");
	end
	CardBox:requestOpenCardBox( nGridIdx, nListType );
end

function SetCardBoxFrameShowMsg( bShow )
	bShowMsg = not bShow;
end

function CardBoxFrameCloseBtn_OnClick()
	if CardBoxExtendFrame:IsShown() and CardBoxExtendFrameWashBtn:GetText() == "关  闭" then
		CardBox:requestCloseCardBox();
		return;
	end
	if bShowMsg then
		MessageBox( "提示", "关闭抽奖后将失去剩余的抽奖机会，确定关闭？" );
		MessageBoxFrame:SetClientString( "抽卡面板关闭" );
		MessageBoxHideBtn:Show();
	else
		CardBox:requestCloseCardBox();
	end
end

function CloseCardBoxFrame()
	selIdx = 0;
	t_ResultIdx = {};
	bTurn = false;
	bUpdate = false;
	bWash = false;
	if CardBoxFrame:IsShown() then
		CardBoxFrame:Hide();
	elseif CardBoxExtendFrame:IsShown() then
		CardBoxExtendFrame:Hide();
	end
end

local function CleanCardBox( frame, num )
	for i=1, num do
		local btn = getglobal( frame .. i );
		btn:SetClientUserData( 0, 0 );
		local icon = getglobal( btn:GetName() .. "Icon" );
		icon:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
		local font = getglobal( btn:GetName() .. "Font" );
		font:SetText( "" );
		local tex = getglobal( btn:GetName() .. "Box" );
		tex:Hide();
		tex = getglobal( btn:GetName() .. "UVAnimation" );
		tex:Hide();
		tex = getglobal( btn:GetName() .. "Select" );
		tex:Hide();
		tex = getglobal( btn:GetName() .. "HightCenter" );
		tex:Hide();
		tex = getglobal( btn:GetName() .. "HightAnim" );
		tex:Hide();
	end
end

local function SetCardBoxInfo( frame, idx, data )
	local itemDef = getItemDef( data.ItemID );
	if itemDef == nil then return end;
	local btn = getglobal( frame .. "Item" .. idx );
	if btn == nil then return end;
	btn:SetClientUserData( 0, data.ItemID );
	btn:SetPushedTexture( "uires\\ZhuJieMian\\4.dds", "blend" );
	btn:SetHightlightTexture( "uires\\ZhuJieMian\\4.dds", "blend" );
	btn:ChangeHighlightTexture( 130,102,64,64 );
	btn:ChangePushedTexture( 65,102,64,64 );
	btn:Enable();

	local icon = getglobal( btn:GetName() .. "Icon" );
	local IconPath = GetItemEquipIconPath() .. itemDef.IconID .. ".tga";
	-- 如果没找到物品的图标则使用默认图标显示
	if not IsInExistence( IconPath ) then
		IconPath = DEFAULT_ITEM_PATH;
	end
	icon:SetTexture( IconPath );
	SetEquipImportantEffectUV( itemDef, btn:GetName() .. "UVAnimation" );

	local font = getglobal( btn:GetName() .. "Font" );
	font:SetText( data.Num > 1 and data.Num or "" );

	local tex = getglobal( btn:GetName() .. "Bkg" );
	tex:Show();
	tex = getglobal( btn:GetName() .. "Box" );
	tex:Show();
	tex = getglobal( btn:GetName() .. "ModelView" );
	tex:Hide();

	if data.Select > 0 then
		tex = getglobal( btn:GetName() .. "Select" );
		tex:Show();
	end
end

local function ResetCoverPoint( frame )
	for i=1, #t_ResultIdx do
		local btn = getglobal( frame .. "Item" .. t_ResultIdx[i] );
		local texAnim = getglobal( btn:GetName() .. "HightAnim" );
		local texAnimPart = getglobal( btn:GetName() .. "HightCenter" );
		texAnim:SetPoint( "center", btn:GetName(), "center", 1, -CARD_BOX_FRAME_HIGH );
		texAnim:Show();
		texAnimPart:SetTexUV( 130, 102, 64, 5 );
		texAnimPart:SetSize( 64, 5 );
		texAnimPart:Show();
	end
	bTurn = true;
end

local function ChangeCardItem( frame )
	local count = 0;
	local step = bTurn and ROLL_STEP_VALUE or -ROLL_STEP_VALUE;
	for i=1, #t_ResultIdx do
		local btn = getglobal( frame .. "Item" .. t_ResultIdx[i] );
		local texAnim = getglobal( btn:GetName() .. "HightAnim" );
		local texAnimPart = getglobal( btn:GetName() .. "HightCenter" );
		local OffsetY = texAnim:GetAnchorOffsetY() + step;
		if math.abs( OffsetY ) < CARD_BOX_FRAME_HIGH then
			texAnim:SetPoint( "center", btn:GetName(), "center", 1, OffsetY );
			local hight = texAnimPart:GetHeight() + step;
			texAnimPart:SetTexUV( 130, 102, 64, hight );
			texAnimPart:SetSize( 64, hight );
		else
			count = count + 1;
		end
	end
	return count;
end

local function BeginCardBoxRoll( frame, idx )
	local btn = getglobal( frame .. "Item" .. idx );
	local tex = getglobal( btn:GetName() .. "Bkg" );
	btn:SetClientUserData( 0, 0 );
	btn:SetPushedTexture( "uires\\ui\\mask\\TouMingTongDao.tga", "blend" );
	btn:SetHightlightTexture( "uires\\ui\\mask\\TouMingTongDao.tga", "blend" );
	btn:Disable();
	tex:Hide();
	local modelview = getglobal( frame .. "Item" .. idx .. "ModelView" );
	modelview:Show();
	modelview:setPetModel(0, 0, "effect\\entity\\fanpai.ent");
	modelview:setPetScale( 0, 1.6, 1.68, 0.1 );
	modelview:setEntityPosition(0, 0, 96, 0);
end

local function ResetFuncBtn( frame )
	local washBtn = getglobal( frame .. "WashBtn" );
	washBtn:Enable();
	local refurbishBtn = getglobal( frame .. "RefurbishBtn" );
	if refurbishBtn ~= nil then
		local count = selectData.Count ~= nil and selectData.Count or 0;
		if cardData.Wash ~= TOTAL_WASH_TIME and count ~= TOTAL_SELECT_TIME then
			refurbishBtn:Enable();
		else
			refurbishBtn:Disable();
		end
	end
end

local function isPlayingAnime()
	return selIdx >0 or bUpdate or bWash;
end

function UpdateCardInfo()
	local cardDef = CardBox:getCardBoxDef( cardData.ID );
	if cardDef == nil then return end;
	local itemDef = getItemDef( cardData.ID );
	
	local frame = cardData.CardNum == MAX_CARD_BOX and "CardBoxFrame" or "CardBoxExtendFrame";
	local font = getglobal( frame .. "Title" );
	font:SetText( itemDef.Name );
	local rich = getglobal( frame .. "Rich" );
	if rich ~= nil then
		rich:SetText( string.format( "剩余%d次抽奖机会", TOTAL_SELECT_TIME - cardData.SelectCount ), 0, 230, 0 );
	end
	local washBtn = getglobal( frame .. "WashBtn" );
	if ( cardData.Wash == 0 and cardData.SelectCount == 0 ) or cardData.SelectCount == TOTAL_SELECT_TIME then
		washBtn:SetText( "开始抽奖" );
		washBtn:Enable();
	end

	t_ResultIdx = {};
	for i=1, cardData.CardNum do
		local idx = cardData.CardInfo[i].GridIdx + 1;
		if cardData.CardInfo[i].Select == 0 then
			table.insert( t_ResultIdx, idx );
		end
	end

	local refurbishBtn = getglobal( frame .. "RefurbishBtn" );
	if refurbishBtn ~= nil then
		local num = TOTAL_WASH_TIME - cardData.Wash;
		refurbishBtn:SetText( string.format( "免费更新奖励（%d）", num ) );
		--判断是否需要播放刷新特效
		if refurbishBtn:GetClientUserData(0) > num then
			ResetCoverPoint( frame );
			refurbishBtn:SetClientUserData( 0, num );
			bUpdate = true;
		else
			refurbishBtn:SetClientUserData( 0, num );
			bUpdate = false;
		end
	end

	if ( not bUpdate and cardData.SelectCount == 0 ) or cardData.SelectCount == TOTAL_SELECT_TIME then
		CleanCardBox( frame .. "Item", cardData.CardNum );
		for i=1, cardData.CardNum do
			local idx = cardData.CardInfo[i].GridIdx + 1;
			SetCardBoxInfo( frame, idx, cardData.CardInfo[i] );
		end
		CardBoxFrameRefurbishBtn:Enable();
	end

	if cardData.CardNum == MAX_CARD_BOX then
		if not CardBoxFrame:IsShown() then
			CardBoxFrame:Show();
			CardBoxExtendFrame:Hide();
		end
	else
		if not CardBoxExtendFrame:IsShown() then
			CardBoxExtendFrame:Show();
			CardBoxFrame:Hide();
		end
	end
end

function UpdateSelectCardResult()
	local cardDef = CardBox:getCardBoxDef( selectData.ID );
	if cardDef == nil then return end;

	local frame = selectData.CardNum == MAX_CARD_BOX and "CardBoxFrame" or "CardBoxExtendFrame";
	local rich = getglobal( frame .. "Rich" );
	if rich ~= nil then
		if selectData.Count ~= TOTAL_SELECT_TIME then
			rich:SetText( string.format( "剩余%d次抽奖机会", TOTAL_SELECT_TIME - selectData.Count ), 0, 230, 0 );
		else
			rich:SetText( "获得一次免费抽奖机会", 255, 255, 0 );
		end
	end

	local washBtn = getglobal( frame .. "WashBtn" );
	if selectData.Count < TOTAL_SELECT_TIME then
		washBtn:SetText( string.format( "花费%d金再抽一次", cardDef.GodGoin[selectData.Count] ) );
		washBtn:SetClientUserData( 0, cardDef.GodGoin[selectData.Count] );
	else
		washBtn:SetText( selectData.Count > TOTAL_SELECT_TIME and "关  闭" or "免费抽奖" );
		washBtn:SetClientUserData( 0, 0 );
	end
	
	t_ResultIdx = {};
	for i=1, selectData.CardNum do
		if selectData.CardInfo[i].Select == 0 then
			table.insert( t_ResultIdx, selectData.CardInfo[i].GridIdx + 1 );
		end
	end

	local idx = selectData.SelectInfo.GridIdx + 1;
	--BeginCardBoxRoll( frame, idx );
	selIdx = idx;
end

function CardBoxFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 0, 0 );
	bWash = false;
	selIdx = 0;
end

function CardBoxFrame_OnHide()
	cardData = {};
	selectData = {};
	CardBoxFrameRefurbishBtn:SetClientUserData(0, 0);
	CardBoxFrameRefurbishBtn:Enable();
end

function CardBoxFrame_OnLoad()
	this:setUpdateTime( 0 );
	this:RegisterEvent( "GE_CARDBOX_INFO" );
	this:RegisterEvent( "GE_CARDBOX_SELECT" );
	this:RegisterEvent( "GE_ENTER_PLAYERLOGIN" );
	CardBoxFrameRich:setCenterLine( true );
end

function CardBoxFrame_OnEvent()
	if arg1 == "GE_CARDBOX_INFO" then
		local msg = UIMSG:getUIMsg()
		cardData.ID		= msg.CardBoxInfo.ID;
		cardData.CostCount	= msg.CardBoxInfo.CostCount;
		cardData.SelectCount	= msg.CardBoxInfo.SelectCount;
		cardData.Wash		= msg.CardBoxInfo.Wash;
		cardData.CardNum	= msg.CardBoxInfo.CardNum;
		cardData.CardInfo	= {};
		for i=1, msg.CardBoxInfo.CardNum do
			table.insert( cardData.CardInfo, 
			{ Select  = msg.CardBoxInfo.CardInfo[i-1].Select, 
			  GridIdx = msg.CardBoxInfo.CardInfo[i-1].GridIdx, 
			  ItemID  = msg.CardBoxInfo.CardInfo[i-1].ItemID,
			  Num	  = msg.CardBoxInfo.CardInfo[i-1].Num } );
		end
		UpdateCardInfo();
	elseif arg1 == "GE_CARDBOX_SELECT" then
		local msg = UIMSG:getUIMsg()
		selectData.ID		= msg.CardBoxSelectInfo.ID;
		selectData.Count	= msg.CardBoxSelectInfo.Count;
		selectData.CardNum	= msg.CardBoxSelectInfo.CardNum;
		selectData.SelectInfo	= { Select  = msg.CardBoxSelectInfo.SelectInfo.Select, 
					    GridIdx = msg.CardBoxSelectInfo.SelectInfo.GridIdx, 
					    ItemID  = msg.CardBoxSelectInfo.SelectInfo.ItemID, 
					    Num	    = msg.CardBoxSelectInfo.SelectInfo.Num };
		selectData.CardInfo	= {};
		for i=1, msg.CardBoxSelectInfo.CardNum do
			table.insert( selectData.CardInfo, 
			{ Select  = msg.CardBoxSelectInfo.CardInfo[i-1].Select, 
			  GridIdx = msg.CardBoxSelectInfo.CardInfo[i-1].GridIdx, 
			  ItemID  = msg.CardBoxSelectInfo.CardInfo[i-1].ItemID,
			  Num	  = msg.CardBoxSelectInfo.CardInfo[i-1].Num } );
		end
		UpdateSelectCardResult();
	elseif arg1 == "GE_ENTER_PLAYERLOGIN" then
		bShowMsg = true;
	end
end

function CardBoxExtendFrame_OnLoad()
	this:setUpdateTime( 0 );
end

function CardBoxFrame_OnUpdate()
	if selIdx > 0 then
		local btn = getglobal( this:GetName() .. "Item" .. selIdx );
		--local modelview = getglobal( btn:GetName() .. "ModelView" );
		--if not modelview:isPlayingAnim(0) then
			if btn:GetClientUserData(0) == 0 then
				--优先显示选择的物品
				SetCardBoxInfo( this:GetName(), selIdx, selectData.SelectInfo );
				return;
			else
				local btn = getglobal( this:GetName() .. "Item" .. t_ResultIdx[1] );
				if btn:GetClientUserData(1) == 0 then
					--播放翻牌特效
					for i=1, #t_ResultIdx do
						BeginCardBoxRoll( this:GetName(), t_ResultIdx[i] );
					end
					btn:SetClientUserData(1, 1)
				elseif btn:GetClientUserData(1) == 1 then
					modelview = getglobal( this:GetName() .. "Item" .. t_ResultIdx[1] .. "ModelView" );
					if not modelview:isPlayingAnim(0) then
						for i=1, selectData.CardNum do
							SetCardBoxInfo( this:GetName(), selectData.CardInfo[i].GridIdx + 1, selectData.CardInfo[i] );
						end
						btn:SetClientUserData(1, 0);
						ResetFuncBtn( this:GetName() );
						selIdx = 0;
					end
				end
			end
		--end
	elseif bUpdate then
		local count = ChangeCardItem( this:GetName() );
		if count == #t_ResultIdx then
			if bTurn then
				for i=1, cardData.CardNum do
					if cardData.CardInfo[i].Select == 0 then
						SetCardBoxInfo( this:GetName(), cardData.CardInfo[i].GridIdx + 1, cardData.CardInfo[i] );
					end
				end
				bTurn = false;
			else
				for i=1, #t_ResultIdx do
					local btn = getglobal( this:GetName() .. "Item" .. t_ResultIdx[i] );
					local texAnim = getglobal( btn:GetName() .. "HightAnim" );
					local texAnimPart = getglobal( btn:GetName() .. "HightCenter" );
					texAnim:Hide();
					texAnimPart:Hide();
				end
				ResetFuncBtn( this:GetName() );
				bUpdate = false;
			end
		end
	elseif bWash then
		local count = 0;
		for i=1, #t_ResultIdx do
			local btn = getglobal( this:GetName() .. "Item" .. t_ResultIdx[i] );
			local tex = getglobal( btn:GetName() .. "Bkg" );
			local modelview = getglobal( btn:GetName() .. "ModelView" );
			if not modelview:isPlayingAnim(0) then
				if not tex:IsShown() then
					btn:SetPushedTexture( "uires\\ZhuJieMian\\4.dds", "blend" );
					btn:SetHightlightTexture( "uires\\ZhuJieMian\\4.dds", "blend" );
					btn:ChangeHighlightTexture( 130,102,64,64 );
					btn:ChangePushedTexture( 65,102,64,64 );
					btn:Enable();
					tex:Show();
					modelview:Hide();
				else
					count = count + 1;
				end
			end
		end

		if count == #t_ResultIdx then
			local rich = getglobal( this:GetName() .. "Rich" );
			if rich ~= nil then
				rich:SetText( "点击翻牌抽奖", 255, 255, 0 );
			end
			bWash = false;
		end
	end
end

function CardBoxItem_OnClick()
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "抽卡面板关闭" then
		return;
	end
	if isPlayingAnime() then return end
	if this:GetClientUserData( 0 ) > 0 then
		local tex = getglobal( this:GetName() .. "Select" );
		if tex:IsShown() then
			return ShowMidTips( "该奖励物品已经被抽取过，无法选择" );
		end
		local btn = getglobal( this:GetParent() .. "WashBtn" );
		if btn:GetText() == "开始抽奖" then
			return ShowMidTips( "请先洗牌再点击图标抽奖" );
		else
			return;
		end
	end
	local idx = this:GetClientID();
	for i=1, #t_ResultIdx do
		if idx == t_ResultIdx[i] then
			CardBox:requestSelectCard( idx - 1 );
			break;
		end
	end
end

function CardBoxItem_OnEnter()
	local itemId = this:GetClientUserData(0);
	if itemId ~= 0 then
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, getItemDef( itemId ) );
	end
end

function CardBoxFrameRefurbishBtn_OnClick()
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "抽卡面板关闭" then
		return;
	end
	if isPlayingAnime() then return end
	this:Disable();
	CardBox:requestWashCard();
	local washBtn = getglobal( this:GetParent() .. "WashBtn" );
	washBtn:Disable();
end

function CardBoxFrameWashBtn_OnClick()
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "抽卡面板关闭" then
		return;
	end
	if isPlayingAnime() then return end
	if this:GetText() == "关  闭" then
		CardBox:requestCloseCardBox();
		return;
	elseif this:GetText() ~= "开始抽奖" then
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer == nil then return end;
		if mainplayer:getGodCoin() < this:GetClientUserData(0) then
			ExchangePointFrame:SetClientUserData(0,1);
			MarketManager:requestQueryTianyuan();
			return;
		end
		CardBox:requestNextCard();
		if this:GetText() == "免费抽奖" then
			return;			
		end
	end
	CardBoxFrameRefurbishBtn:Disable();
	this:Disable();

	for i=1, #t_ResultIdx do
		local btn = getglobal( this:GetParent() .. "Item" .. t_ResultIdx[i] );
		local icon = getglobal( btn:GetName() .. "Icon" );
		icon:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
		local font = getglobal( btn:GetName() .. "Font" );
		font:SetText( "" );
		local tex = getglobal( btn:GetName() .. "Box" );
		tex:Hide();
		tex = getglobal( btn:GetName() .. "UVAnimation" );
		tex:Hide();
		tex = getglobal( btn:GetName() .. "Select" );
		tex:Hide();
		BeginCardBoxRoll( this:GetParent(), t_ResultIdx[i] );
	end
	bWash = true;
end
