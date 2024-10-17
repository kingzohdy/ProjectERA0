local MAX_WATCH_BTN_NUM = 4;
local DEFAULT_GRID_NUM = 8;
local MAX_GRID_NUM = 40;
local MAX_STORE_LIST = 10;
local MAX_STORE_PAGE = 50;
local goldRate = 0;
local selfStoreId = 0;
local curStoreId = 0;
local curGridIdx = 0;
local gridCount = DEFAULT_GRID_NUM;
local curPageNum = 0;
local pageCount = 0;
local storeNum = 0;		--��¼������Ŀ
local t_WatchBtn = {};
local t_WatchList = {};
local t_RankTex = { { 586, 112 }, { 564, 129 }, { 468, 142 } };
local szSearchKeyWords = ""		--���ڱ�����������Ĺؼ��ʣ���ƥ���Ӧ���������Ʒ
local t_SearchResults = {};		--���ڱ�����������ĵ���ID
local t_HistoryItemPrice = {};	--���ڱ�����̵���ʷ�ۼ�

function SetGoldRate( rate )
	goldRate = rate;
	if OnlineStoreFrame:IsShown() and curStoreId == selfStoreId then
		local base = math.ceil( goldRate / 20 );
		local cost = GetCeilFormC( base * ( 0.2 * (gridCount - DEFAULT_GRID_NUM) )^2 + base );
		OnlineStoreFrameRateFont:SetText( "���޷ѣ�" .. cost );
	end
	if MarketTicketFrame:IsShown() then 
		UpdateMarketFrameRate();
	end
end

function GetGoldRate()
	return goldRate;
end

function IsInSelfStore()
	return curStoreId == selfStoreId, curStoreId;
end

local function getRankIdx( num )
	if num == 1 then
		return 1, "#cff9600\t  ����λ����ߵĻʹڵ��̣�#n";
	elseif num < 5 then
		return 2, "#B\t���������ڻʹڵ���ʯ���̣�#n";
	else
		return 3, "#G\t������������ʯ���������̣�#n";
	end
end

local function hideAllWatchBtn()
	for i=1, MAX_WATCH_BTN_NUM do
		local btn = getglobal( "OnlineStoreFrameWatchBtn" .. i );
		btn:SetClientUserData( 0, 0 );
		btn:Hide();
	end
end

local function updateWatchBtn( total, value )
	hideAllWatchBtn();
	local count = 1;
	for i=value, total do
		if count > MAX_WATCH_BTN_NUM then
			break;
		end
		local btn = getglobal( "OnlineStoreFrameWatchBtn" .. count );
		for j=1, #t_WatchBtn do
			if i == t_WatchBtn[j][1] then
				btn:SetClientUserData( 0, t_WatchBtn[j][2] );
				btn:Show();
				break;
			end
		end
		count = count + 1;
	end
end

local function UpdateStoreSliderRange()
	if not OnlineStoreFrame:IsShown() then
		return;
	end
	local rich	 = getglobal( "OnlineStoreFrameAdRich" );
	SetScrollBar( "OnlineStoreFrameScrollBar", rich:GetAccurateViewLines(), rich:GetTextLines() );

	local slider	 = getglobal( "OnlineStoreFrameScrollBar" );
	local nValue	 = slider:GetValue();
	local nMaxValue	 = slider:GetMaxValue()
	if nMaxValue > 1 then
		if nValue >= nMaxValue - 2 then
			slider:SetValue( nMaxValue );
		end
	end
	util.UpdateRich( rich:GetName(), slider:GetValue() );
	updateWatchBtn( rich:GetTextLines(), slider:GetValue() );
end

local function ClearGridItem( name )
	local btn = getglobal( name );
	local ItemNum   = getglobal( name.."CountFont");
	local ItemBoxTex = getglobal( name.."BoxTexture");
	local UVAnimationTex = getglobal( name.."UVAnimationTex");
	local ItemTex = getglobal( name.."IconTexture");
	local TargetUV = getglobal( name.."TargetUV" )
	btn:SetClientUserData( 0, 0 );
	btn:DisChecked();
	ItemNum:SetText( "" );
	ItemBoxTex:Hide();
	UVAnimationTex:Hide();
	ItemTex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
	TargetUV:Hide()
	
end

local function ResetAllGrid( num )
	curGridIdx = 0;
	for i=1, MAX_GRID_NUM do
		local btn = getglobal( "OnlineStoreFrameItem" .. i );
		ClearGridItem( btn:GetName() );
		if i > DEFAULT_GRID_NUM then
			if i <= num then
				btn:SetNormalTexture( "uires\\ui\\mask\\TouMingTongDao.tga", "blend" );
				btn:SetPushedTexture( "uires\\ZhuJieMian\\1.dds", "blend" );
				btn:SetHightlightTexture( "uires\\ZhuJieMian\\1.dds", "blend" );
				btn:ChangeHighlightTexture( 794,697,44,44 );
				btn:ChangePushedTexture( 794,607,44,44 );
				btn:Show();
			else
				btn:Hide();
			end
		end
	end
end

local function DisCheckAllGridItem()
	for i=1, MAX_GRID_NUM do
		local btn = getglobal( "OnlineStoreFrameItem" .. i );
		btn:DisChecked();
	end
end

local function SetClosedStore()
	local mainplayer = ActorMgr:getMainPlayer();
	OnlineStoreFrameOwnerRich:SetText( mainplayer ~= nil and mainplayer:getName() or "", 255, 255, 190 );
	OnlineStoreFrameOwnerRich:SetClientUserData( 0, 0 );
	OnlineStoreFrameNameFont:SetText( "������" );
	OnlineStoreFrameNameEdit:Hide()
	OnlineStoreFrameNameShowBtn:Show();
	OnlineStoreFrameBackBtn:Show();
	OnlineStoreFrameBuyBtn:Hide();
	OnlineStoreFrameAddBtn:Show();
	OnlineStoreFrameDecBtn:Show();
	OnlineStoreMoneyOptFrame:Hide();
	OnlineStoreFrameOpenBtn:Show();
	OnlineStoreFrameRate:Show();
	OnlineStoreFrameOpenBtn:SetText( "��  ��" );
	OnlineStoreFrameRateFont:SetText( "���޷ѣ�" .. math.ceil( goldRate / 20 ) );
	OnlineStoreFramePriceFont:SetText(0);
	OnlineStoreFrameFundDescBtn:SetText( "�ʲ�" );
	OnlineStoreFrameFundDescBtn:Checked();
	OnlineStoreFrameFundFont:SetText(0);
	OnlineStoreFrameFundFont:SetTextColor( 120, 120, 100 );
	OnlineStoreFrameSaleHistoryBtnRich:Clear();
	OnlineStoreFrameSaleHistoryBtnTime:Hide();
	OnlineStoreSaleHistoryBtnScrollBar:Hide();
	OnlineStoreItemOptFrame:Hide();
	gridCount = DEFAULT_GRID_NUM;
	curStoreId = 0;
	selfStoreId = 0;
	ResetAllGrid( DEFAULT_GRID_NUM );
end

local function AddHistoryItemPrice( itemID, price )
	--����б�����
	for i = 1, table.getn( t_HistoryItemPrice ) do
		if t_HistoryItemPrice[i].ItemID == itemID then
			t_HistoryItemPrice[i].Price = price
			return
		end
	end
	--û�У�������б�
	local itemPrice = { ItemID = itemID, Price = price }
	table.insert( t_HistoryItemPrice, itemPrice )
end

local function GetHistoryItemPrice( itemID )
	--����б�����
	for i = 1, table.getn( t_HistoryItemPrice ) do
		if t_HistoryItemPrice[i].ItemID == itemID then
			return t_HistoryItemPrice[i].Price
		end
	end
	--û��
	return 0
end

local function SetGridItem( item, price )
	local idx = item.GridIdx + 1;
	local btn = getglobal( "OnlineStoreFrameItem" .. idx );
	local ItemNum   = getglobal( btn:GetName().."CountFont");
	local ItemBoxTex = getglobal( btn:GetName().."BoxTexture");
	local ItemTex = getglobal( btn:GetName().."IconTexture");
	local TargetUV = getglobal( btn:GetName().."TargetUV" )
	btn:SetClientUserData( 0, idx );
	ItemNum:SetText( item.Num );
	ItemBoxTex:Show();

	local itemDef = getItemDef( item.DefID );
	local IconPath = GetItemEquipIconPath() .. itemDef.IconID .. ".tga";
	-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
	if not IsInExistence( IconPath ) then
		IconPath = DEFAULT_ITEM_PATH;
	end
	ItemTex:SetTexture( IconPath );
	SetEquipImportantEffectUV( itemDef, btn:GetName() .. "UVAnimationTex" );

	if btn:IsChecked() then
		OnlineStoreFramePriceFont:SetText( price );
	end
	
	--��鵱ǰ��Ʒ�����ƣ��Ƿ��йؼ���
	if table.getn(t_SearchResults) > 0  then
		if string.find( itemDef.Name, szSearchKeyWords ) and szSearchKeyWords ~= "" then
			TargetUV:Show()
			TargetUV:SetUVAnimation( 48, true )	
		else
			TargetUV:Hide()
		end
	else
		TargetUV:Hide()
	end
	
	--������Լ�������Ʒ�������б�
	if selfStoreId ~= 0 and curStoreId == selfStoreId then
		AddHistoryItemPrice( itemDef.ItemID, price )
	end
end

local function ClearStoreList()
	for i=1, MAX_STORE_LIST do
		local btn = getglobal( "OnlineStoreFrameList" .. i );
		local tex = getglobal( "OnlineStoreFrameList" .. i .. "RankTex" );
		local font = getglobal( "OnlineStoreFrameList" .. i .. "Font" );
		local SearchTex = getglobal( "OnlineStoreFrameList" .. i .. "SearchTex" );
		btn:SetClientUserData( 0, 0 );
		btn:SetClientString("");
		btn:DisChecked();
		tex:Hide();
		font:SetText( "" );
		SearchTex:Hide()
	end
end

local function SetStoreNameColor( id, color, name )
	local t_Color = { orange = { 255, 150, 0 }, green = { 0, 230, 0 }, blue = { 60, 255,255 } };
	if name ~= "" then
		local font = getglobal( name );
		font:SetTextColor( t_Color[color][1], t_Color[color][2], t_Color[color][3] );
	else
		for i=1, MAX_STORE_LIST do
			local btn = getglobal( "OnlineStoreFrameList" .. i );
			if btn:GetClientUserData(0) == id then
				btn:Checked();
				local font = getglobal( "OnlineStoreFrameList" .. i .. "Font" );
				font:SetTextColor( t_Color[color][1], t_Color[color][2], t_Color[color][3] );
			else
				btn:DisChecked();
			end
		end
	end
end

local function CheckCanOpenStoreFrame()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	
	if mainplayer:isDead() then
		ShowMidTips("���Ѿ��������޷�ʹ�õ���");
		return false;
	end

	if mainplayer:isInStall() then
		ShowMidTips("���ڰ�̯�У��޷�ʹ�õ���");
		return false;
	end

	if MapShower:getRealMapId() >= MIN_COPY_ID then
		ShowMidTips( "��ǰ���ڸ����У��޷�ʹ�õ���" );
		return false;
	end

	if mainplayer:isInPK() then
		ShowMidTips("�����д��У��޷�ʹ�õ���");
		return false;
	end

	if mainplayer:isInFight() then
		ShowMidTips( "����ս���У��޷�ʹ�õ���" );
		return false;
	end

	if GameExchange:isInExchange() then
		ShowMidTips("���ڽ����У��޷�ʹ�õ���");
		return false;
	end

	if mainplayer:isInFly() then
		ShowMidTips("��ǰ״̬�£��޷�ʹ�õ���");
		return false;
	end

	return true;
end

local function CheckHaveGrid()
	for i=1, gridCount do
		local btn = getglobal( "OnlineStoreFrameItem" .. i );
		if btn:GetClientUserData(0) == 0 then
			return true;
		end
	end
	ShowMidTips( "�����Ѳ������ܰڷ���Ʒ" );
	return false;
end

function UpdateStoreList( listData )
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	curPageNum = listData.Page + 1;
	pageCount = listData.PageCount;
	storeNum = listData.Num
	OnlineStoreFramePageTotal:SetText( "/" .. ( pageCount > 0 and pageCount or 1 ) );
	OnlineStoreFramePageEdit:SetText( curPageNum );
	ClearStoreList();

	for i=1, listData.Num do
		local btn = getglobal( "OnlineStoreFrameList" .. i );
		local tex = getglobal( "OnlineStoreFrameList" .. i .. "RankTex" );
		local font = getglobal( "OnlineStoreFrameList" .. i .. "Font" );
		local SearchTex = getglobal( "OnlineStoreFrameList" .. i .. "SearchTex" );
		for j = 1, table.getn( t_SearchResults ) do
			if listData.PageOne[i-1].ID == t_SearchResults[j] then
				SearchTex:Show()
			end
		end
		local title = "\t ";
		if curPageNum > 1 then
			tex:Hide();
			title = (listData.Page * 10 + i) .. ".";
		else
			local idx = getRankIdx( i );
			tex:SetTexture("uires\\ZhuJieMian\\3.dds")
			tex:SetTexUV( t_RankTex[idx][1], t_RankTex[idx][2], 15, 16 );
			tex:Show();
		end
		local id = listData.PageOne[i-1].ID;
		btn:SetClientUserData( 0, id );
		btn:SetClientString( listData.PageOne[i-1].RoleName );
		font:SetText( title .. listData.PageOne[i-1].StoreName );

		if mainplayer:getName() == listData.PageOne[i-1].RoleName then
			SetStoreNameColor( id, "green", font:GetName() )
		elseif t_WatchList[id] ~= nil then
			SetStoreNameColor( id, "orange", font:GetName() )
		else
			font:SetTextColor( 255, 255, 190 );
		end

		if id == curStoreId then
			btn:Checked();
		end
	end
end

function SetSelfStoreName( nSelfStoreID, szNewName )
	for i=1, MAX_STORE_LIST do
		local btn = getglobal( "OnlineStoreFrameList" .. i );
		local font = getglobal( "OnlineStoreFrameList" .. i .. "Font" );
		if btn:GetClientUserData( 0 ) == nSelfStoreID then
			local szOldName = font:GetText();
			if curPageNum > 1 then
				szNewName = string.sub( szOldName, 1,string.find( szOldName,".",1,true ) ) .. szNewName
			else
				szNewName = "\t " .. szNewName
			end
			font:SetText( szNewName )
		end
	end
end

function UpdateStoreInfo( nType )
	local storeData = StoreMgr:getStoreData();
	if storeData.StoreID == 0 then
		return;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local color = "orange"

	curStoreId = storeData.StoreID;
	local bShow = false;
	if nType > 0 then
		selfStoreId = storeData.StoreID;
		bShow = true;
		OnlineStoreFrameBuyBtn:Hide();
		OnlineStoreFrameOpenBtn:SetText( "�ʲ�����" );
		OnlineStoreFrameFundDescBtn:SetText( "�ʲ�" );
		OnlineStoreFrameFundDescBtn:DisChecked();

		gridCount = storeData.StoreInfo.MaxNum;
		color = "green";
		local base = math.ceil( goldRate / 20 );
		local cost = GetCeilFormC( base * ( 0.2 * (gridCount - DEFAULT_GRID_NUM) )^2 + base );
		OnlineStoreFrameRateFont:SetText( "���޷ѣ�" .. cost );
		OnlineStoreFrameFundFont:SetText( storeData.StoreInfo.Money );
		local idx = storeData.StoreInfo.Money < 0 and 2 or 1;
		local t_Color = { { 120, 120, 100 }, { 230, 0, 0 } };
		OnlineStoreFrameFundFont:SetTextColor( t_Color[idx][1], t_Color[idx][2], t_Color[idx][3] );
		OnlineStoreFrameOwnerRich:SetText( storeData.Name, 255, 255, 190 );
		OnlineStoreFrameOwnerRich:SetClientUserData( 0, 0 );

		local font = getglobal( "OnlineStoreMoneyOptFrameTime" );
		font:SetText( os.date("����ʱ�䣺%Y��%m��%d�� - %H:%M", storeData.StoreInfo.OpenTime) );
		font = getglobal( "OnlineStoreMoneyOptFrameTotal" );
		font:SetText( "�����ܶ" .. GameWizard:uLongLongToString(storeData.StoreInfo.Exchg) );
		OnlineStoreFrameNameFont:SetText( "������" );
		OnlineStoreFrameNameShowBtn:Show();
		OnlineStoreFrameNameShowBtnFont:SetText( storeData.StoreInfo.StoreName );
		SetSelfStoreName( storeData.StoreID, storeData.StoreInfo.StoreName )
	else
		t_WatchList[curStoreId] = 1;
		OnlineStoreFrameBuyBtn:Show();
		OnlineStoreFrameFundDescBtn:SetText( "����" );
		OnlineStoreFrameFundDescBtn:Checked();
		OnlineStoreFrameFundFont:SetText( mainplayer:getMoney() );
		OnlineStoreFrameFundFont:SetTextColor( 120, 120, 100 );
		OnlineStoreFrameOwnerRich:SetText( "#L" .. storeData.Name .. "@@player#n", 255, 255, 190 );
		OnlineStoreFrameOwnerRich:SetClientUserData( 0, 1 );
		OnlineStoreFrameNameFont:SetText( "������" .. storeData.StoreInfo.StoreName );
		OnlineStoreFrameNameShowBtn:Hide();
	end

	local t_OptBtn = { "OpenBtn", "Rate", "AddBtn", "DecBtn", "BackBtn" }
	for i=1, #t_OptBtn do
		local btn = getglobal( "OnlineStoreFrame" .. t_OptBtn[i] );
		if bShow then
			btn:Show();
		else
			btn:Hide();
		end
	end
	
	SetStoreNameColor( curStoreId, color, "" );
	OnlineStoreFrameNameEdit:Hide();
	OnlineStoreMoneyOptFrame:Hide();
	OnlineStoreItemOptFrame:Hide();

	ResetAllGrid( storeData.StoreInfo.MaxNum );
	for i=1, storeData.StoreInfo.Num do
		local gridData = storeData.StoreInfo.Grid[i-1];
		local item;
		if gridData.Grid.Type > 0 then
			item = gridData.Grid.GridData.RoleArm;
		else
			item = gridData.Grid.GridData.RoleItem
		end
		SetGridItem( item, gridData.Price );
	end

	OnlineStoreFrameSaleHistoryBtnTime:Show();
	local font = getglobal( "OnlineStoreFrameSaleHistoryBtnTimeFont" );
	font:SetText( os.date("%Y��%m��%d��\n\t\t  %H:%M", storeData.StoreInfo.OpenTime) );
	local rich = getglobal( "OnlineStoreFrameSaleHistoryBtnRich" );
	rich:Clear();
	for i=1, storeData.StoreInfo.RecordNum do
		local itemDef = getItemDef( storeData.StoreInfo.Record[i-1].ItemID );
		local record = getBonusTime(storeData.StoreInfo.Record[i-1].Time) .. "\n#L#cfffebd" .. storeData.StoreInfo.Record[i-1].Who .. "@@player#n#c938259��"
					.. storeData.StoreInfo.Record[i-1].Price .. "��"..MONEY_TYPE_MONEY_NAME.."\n������#n#L#c" .. getItemColor(itemDef.Important,itemDef.SuitSetID)
					.. itemDef.Name .. "@@item" .. itemDef.ItemID .."#n*" .. storeData.StoreInfo.Record[i-1].Num .. "��\n\n";
		rich:AddText( record, 255, 255, 190 );
	end
	local slider = getglobal( "OnlineStoreSaleHistoryBtnScrollBar" );
	SetScrollBar( slider:GetName(), rich:GetViewLines(), rich:GetTextLines() );
	slider:SetValue( slider:GetMaxValue() );

	OnlineStoreFramePriceFont:SetText(0);
end

function OpenStoreResult( storeId, result, name )
	if OnlineStoreMessageBoxFrame:IsShown() then
		OnlineStoreMessageBoxFrame:Hide();
	end
	if result == 0 then
		selfStoreId = storeId;
		if curStoreId ~= 0 then
			return
		end
		StoreMgr:requestStorePage( curPageNum - 1 );
		curStoreId = storeId;
		OnlineStoreFrameNameFont:SetText( "������" );
		OnlineStoreFrameNameShowBtn:Show();
		OnlineStoreFrameNameShowBtnFont:SetText( name );
		OnlineStoreFrameNameEdit:Hide();
		OnlineStoreFrameNameEdit:SetText( "" );
		OnlineStoreFrameFundDescBtn:DisChecked();
		OnlineStoreFrameOpenBtn:SetText( "�ʲ�����" );
	end
end

function CloseStoreResult( storeId )
	--�����ǰֻ��һ�����̵�ʱ��ֱ������б�
	if storeNum <= 1 then
		ClearStoreList()
	else
		StoreMgr:requestStorePage( curPageNum - 1 );
	end
	if curStoreId == storeId then
		curStoreId = 0;
		SetClosedStore();
	end
end

function UpdateStoreGrid( storeId, gridMax )
	if storeId == curStoreId then
		local btn;
		if gridCount > gridMax then
			btn = getglobal( "OnlineStoreFrameItem" .. gridCount );
			btn:Hide();
			ClearGridItem( btn:GetName() );
		else
			btn = getglobal( "OnlineStoreFrameItem" .. gridMax );
			btn:SetNormalTexture( "uires\\ui\\mask\\TouMingTongDao.tga", "blend" );
			btn:SetPushedTexture( "uires\\ZhuJieMian\\1.dds", "blend" );
			btn:SetHightlightTexture( "uires\\ZhuJieMian\\1.dds", "blend" );
			btn:ChangeHighlightTexture( 794,697,44,44 );
			btn:ChangePushedTexture( 794,607,44,44 );
			btn:Show();
		end
	end
	gridCount = gridMax;
	local base = math.ceil( goldRate / 20 );
	local cost = GetCeilFormC( base * ( 0.2 * (gridCount - DEFAULT_GRID_NUM) )^2 + base );
	OnlineStoreFrameRateFont:SetText( "���޷ѣ�" .. cost );
end

function UpdateStoreMoney( storeId, money )
	if storeId == curStoreId then
		OnlineStoreFrameFundFont:SetText( money );
		local idx = money < 0 and 2 or 1;
		local t_Color = { { 120, 120, 100 }, { 230, 0, 0 } };
		OnlineStoreFrameFundFont:SetTextColor( t_Color[idx][1], t_Color[idx][2], t_Color[idx][3] );

		if OnlineStoreMoneyOptFrame:IsShown() then
			StoreMgr:requestStorePos( storeId );
		end
	end
end

function UpdateStorePos( storeId, pos, exChg )
	if storeId == selfStoreId then
		OnlineStoreMoneyOptFramePos:SetText( pos+1 );
		local font = getglobal( "OnlineStoreMoneyOptFrameTotal" );
		font:SetText( "�����ܶ" .. exChg );
		local btn = getglobal( "OnlineStoreMoneyOptFrameTotalMoney" );
		btn:SetPoint( "topleft", "OnlineStoreMoneyOptFrame", "topleft", (16 + font:GetTextExtentWidth(font:GetText()))/ GetScreenScaleY(), 128 );
	end
end

function SetSelectStoreItem( szBtnName )
	DisCheckAllGridItem();
	local btn = getglobal( szBtnName );
	btn:Checked();
	curGridIdx = btn:GetClientUserData(0);
end

function StoreGridItemOnEnter( szBtnName )
	local btn = getglobal( szBtnName );
	local idx = btn:GetClientUserData(0) - 1;
	if idx < 0 then return end;

	local item = StoreMgr:getStoreItem( idx );
	if item ~= nil then
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item, nil, btn:GetName(), nil, nil, nil, idx );
	end
end

function Save2OnlineStoreItem( item, nSrcList, nSrcGridIndex )
	local nError = CanExChange( item );
	if nError ~= 0 then
		return ShowMidTips( GetExChangeErrorMsg( nError, "stall" ) );
	end
	local bSelf, id = IsInSelfStore();
	if bSelf then
		if id > 0 then
			if CheckHaveGrid() then
				ShowOnlineStoreItemOptFrame( nSrcGridIndex, "��  ��" );
			end
		else
			return ShowMidTips( "�������ܶԵ��̵���Ʒ���в���" );
		end
	else
		return ShowMidTips( "ֻ�ܶ��Լ��ĵ��̽����ϼܲ���" );
	end
end

function OnlineStoreFrameNameShowBtn_OnClick()
	this:Hide();
	local szText = OnlineStoreFrameNameShowBtnFont:GetText();
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer ~= nil then
		szText = szText ~= "������༭�������ƣ�" and szText or ( mainplayer:getName() .. "�ĵ���" );
	end
	OnlineStoreFrameNameEdit:SetText( szText );
	OnlineStoreFrameNameEdit:Show();
	SetFocusFrame( "OnlineStoreFrameNameEdit" );
	OnlineStoreFrameNameEdit:SelectAllText();
end

function OnlineStoreFrameNameShowBtn_OnEnter()
	SetGameTooltips( this:GetName(),"����ɱ༭��������" );
end

function OnlineStoreFrameNameEdit_OnFocusLost()
	this:Hide();
	local szName = this:GetText()
	if szName == "" then return end
	if curStoreId == selfStoreId and selfStoreId ~= 0 then
		StoreMgr:requestChgStoreName( szName )
	end
	OnlineStoreFrameNameShowBtnFont:SetText( this:GetText() );
	OnlineStoreFrameNameShowBtn:Show();
end

function OnlineStoreFrameNameEdit_OnEnterPressed()
end

function OnlineStoreFrame_OnLoad()
	this:RegisterEvent( "GE_ENTER_PLAYERLOGIN" );
	this:RegisterEvent( "GE_CHANGE_MONEY" );
	this:RegisterEvent( "GE_STORE_OPENUI" );
	this:RegisterEvent( "GE_STORE_PAGE" );
	this:RegisterEvent( "GE_STORE_ITEMCHG" );
	this:RegisterEvent( "GE_ACTOR_FIGHT_IN" );
	this:RegisterEvent( "GE_MESSAGE_DEAD" );
	this:RegisterEvent( "GE_ENTER_PWORLD" );
	ItemFirstCallBack( "OnlineStoreFrame" , Save2OnlineStoreItem );

	for i=1, MAX_GRID_NUM do
		local btn = getglobal( "OnlineStoreFrameItem" .. i );
		btn:SetPoint( "topleft", "OnlineStoreFrame", "topleft", 198 + (i-1)%8*47, 123 + math.ceil(i/8)*47 );
		local tex = getglobal( btn:GetName() .. "BackTexture" );
		tex:Show();
	end

	for i=1, MAX_STORE_LIST do
		local btn = getglobal( "OnlineStoreFrameList" .. i );
		btn:SetPoint( "topleft", "OnlineStoreFrame", "topleft", 11, 132 + i*25 );
		if i%2 ~= 0 then
			local tex = getglobal( btn:GetName() .. "BlackTex" );
			tex:Hide();
		end
	end
	OnlineStoreFramePageEdit:setUpdateTime(0.5);
end

--�յ���Ϣʱ�ڵ��̺����ʾ��ǣ�ͬʱ������ǰ���̳�����Ʒ����ʾĿ����Ʒ
function ChangeSearchStoreResult()
	--���̱�Ǵ���
	for i = 1, MAX_STORE_LIST do
		local btn = getglobal( "OnlineStoreFrameList" .. i );
		local SearchTex = getglobal( "OnlineStoreFrameList" .. i .. "SearchTex" );
		for j = 1, table.getn( t_SearchResults ) do
			if btn:GetClientUserData(0) == t_SearchResults[j] then
				SearchTex:Show()
			end
		end
	end
	--������Ʒ����
	for i = 1, MAX_GRID_NUM do
		local btn = getglobal( "OnlineStoreFrameItem"..i );
		if btn:IsShown() then
			local TargetUV = getglobal( btn:GetName().."TargetUV" )
			local idx = btn:GetClientUserData(0) - 1;
			if idx < 0 then return end;

			local item = StoreMgr:getStoreItem( idx );		
			if item ~= nil then
				--��鵱ǰ��Ʒ�����ƣ��Ƿ��йؼ���
				if table.getn(t_SearchResults) > 0  then
					if string.find( item:getItemDef().Name, szSearchKeyWords ) and szSearchKeyWords ~= "" then
						TargetUV:Show()
						TargetUV:SetUVAnimation( 48, true )	
					else
						TargetUV:Hide()
					end
				else
					TargetUV:Hide()
				end
			end
		end
	end
end

function OnlineStoreFrame_OnEvent()
	if arg1 == "GE_ENTER_PLAYERLOGIN" then
		local rich = getglobal( "OnlineStoreFrameAdRich" );
		rich:Clear();
		SetScrollBar( "OnlineStoreFrameScrollBar", 1, 0 );
	elseif arg1 == "GE_CHANGE_MONEY" then
		if this:IsShown() and curStoreId ~= selfStoreId then
			local mainplayer = ActorMgr:getMainPlayer();
			if mainplayer ~= nil then
				OnlineStoreFrameFundFont:SetText( mainplayer:getMoney() );
			end
		end
	elseif arg1 == "GE_STORE_OPENUI" then
		if not CheckCanOpenStoreFrame() then
			return;
		end
		if this:IsShown() then
			this:Hide();
		end

		local msg = UIMSG:getUIMsg();
		if msg.StoreData.OpenUI.Store.StoreID > 0 then
			UpdateStoreInfo(1);
		else
			SetClosedStore();
		end
		local listData = msg.StoreData.OpenUI.StorePageSvr;
		UpdateStoreList( listData );
		
		this:Show();
		if msg.StoreData.OpenUI.NpcID > 0 then
			util.testNpcDialogDistance( msg.StoreData.OpenUI.NpcID, OnlineStoreFrame );
		end
	elseif arg1 == "GE_STORE_PAGE" then
		local msg = UIMSG:getUIMsg();
		local listData = msg.StoreData.Page;
		UpdateStoreList( listData );
	elseif arg1 == "GE_STORE_ITEMCHG" then
		if not this:IsShown() then
			return;
		end

		local msg = UIMSG:getUIMsg();
		if msg.StoreData.ItemChg.ID ~= curStoreId then
			return;
		end
		local gridData = msg.StoreData.ItemChg.StoreGrid;
		local item;
		if gridData.Grid.Type > 0 then
			item = gridData.Grid.GridData.RoleArm;
		else
			item = gridData.Grid.GridData.RoleItem
		end
		if item.Num > 0 then
			SetGridItem( item, gridData.Price );
			OnlineStoreFramePriceFont:SetText( gridData.Price );
		else
			local btn = getglobal( "OnlineStoreFrameItem" .. item.GridIdx + 1 );
			ClearGridItem( btn:GetName() );
			OnlineStoreFramePriceFont:SetText( 0 );
		end
	elseif arg1 == "GE_ACTOR_FIGHT_IN" or arg1 == "GE_MESSAGE_DEAD"
		or arg1 == "GE_ENTER_PWORLD" then
		if this:IsShown() then
			this:Hide();
		end
	end
end

function OnlineStoreFrame_OnShow()
	if PlayerContainerFrame:IsShown() then
		PlayerContainerFrame:SetPoint( "topright", "$parent", "topright",-16, 163 );
	else
		PlayerContainerFrame:Show();
		PlayerContainerFrame:SetPoint( "topright", "$parent", "topright",-16, 163 );
	end
	this:SetPoint( "topright", "PlayerContainerFrame", "topleft", 0, 0);
	--this:SetPoint( "center", "UIClient", "center", 0, 0 );
	local rich	 = getglobal( "OnlineStoreFrameAdRich" );
	local slider	 = getglobal("OnlineStoreFrameScrollBar");
	slider:SetValue( slider:GetMaxValue() );
	updateWatchBtn( rich:GetTextLines(), slider:GetMaxValue() );
end

function OnlineStoreFrame_OnHide()
	OnlineStoreFrameBackBtn:Show();
	OnlineStoreFrameBuyBtn:Hide();
	OnlineStoreFrameRate:Show();
	OnlineStoreFrameSaleHistoryBtn:Hide();
	OnlineStoreFrameNameShowBtnFont:SetText("������༭�������ƣ�");
	OnlineStoreFrameNameEdit:SetText( "" );
	t_WatchList = {};
	OnlineStoreMoneyOptFrame:Hide();
	OnlineStoreItemOptFrame:Hide();
	--��ձ�����������ĵ����б��������ؼ���
	ClearSearchResult()
	szSearchKeyWords = ""

	if OnlineStoreMessageBoxFrame:IsShown() then
		OnlineStoreMessageBoxFrame:Hide();
	end
end

function OnlineStoreFrameOwnerRich_OnEnter()
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "������˽�ģ��Ҽ�����鿴", frame = this:GetParent(),
				button = "freedom", conflictFrame = this:GetName(), yAdjust = -16, ["verticalReferencePoint"] = "vertical", ["vertical"] = "top" } );
end

function OnlineStoreFrameOwnerRich_OnClick()
	if arg1 == nil or arg1 == "LeftButton" or arg1 == "RightButton" then return end;
	if arg3 == "LeftButton" then
		SendPrivateMessage( arg1 );
	elseif arg3 == "RightButton" then
		ShowStorePlayerRightFrame( arg1 );
	end
end

function OnlineStoreFrameWatchBtn_OnClick()
	local id = this:GetClientUserData(0);
	if id > 0 then
		StoreMgr:requestStoreView( id );
	end
end

function OnlineStoreFrameWatchBtn_OnEnter()
	SetGameTooltips( this:GetName(), "�����ֱ�Ӳ鿴�õ���" );
end

function OnlineStoreFrameHomeBtn_OnClick()
	if selfStoreId > 0 then
		StoreMgr:requestStoreView( selfStoreId );
	else
		SetClosedStore();
	end
end

function OnlineStoreFrameHomeBtn_OnEnter()
	SetGameTooltips( this:GetName(), "����ص��Լ��ĵ���" );
end

function OnlineStoreFrameAdBtn_OnClick()
	if selfStoreId > 0 then
		ChatManager:setStoreLink( selfStoreId );
	else
		return ShowMidTips( "�㵱ǰû�е��̣��޷���������" );
	end
end

function OnlineStoreFrameAdBtn_OnEnter()
	SetGameTooltips( this:GetName(), "������ڹ��Ƶ������" );
end

--������е��������
function ClearSearchResult()
	--�Ƚ�֮ǰ�����ĵ��̵ı��ͼ�����أ�ͬʱ�����ǰ������ʾ��Ŀ����Ʒ
	for i = 1, MAX_STORE_LIST do
		local btn = getglobal( "OnlineStoreFrameList" .. i );
		local SearchTex = getglobal( "OnlineStoreFrameList" .. i .. "SearchTex" );
		for j = 1, table.getn( t_SearchResults ) do
			if btn:GetClientUserData(0) == t_SearchResults[j] then
				SearchTex:Hide()
			end
		end
	end
	--��ճ�����Ʒ�ı��
	for i = 1, MAX_GRID_NUM do
		local btn = getglobal( "OnlineStoreFrameItem"..i );
		local TargetUV = getglobal( btn:GetName().."TargetUV" )
		TargetUV:Hide()
	end
	--���ſ�������б��Խ����µ��������
	t_SearchResults = {}
end

--����������б������һ������ID
function AddSearchResult( ID )
	table.insert( t_SearchResults, ID )
end

--���ؽ�ɫʱ����յ������
function InitStoreByChangePlayer()
	ClearSearchResult()
	szSearchKeyWords = ""
	t_HistoryItemPrice = {}
end

function OnlineStoreFrameSearchBtn_OnClick()
	local szInfo = OnlineStoreFrameSearchEdit:GetText()
	if szInfo == "" then
		ShowMidTips( "��������Ҫ��������Ʒ����" )
		return
	end
	StoreMgr:requestSearch( szInfo )
	szSearchKeyWords = szInfo;
end
function OnlineStoreFrameSearchBtn_OnEnter()
	SetGameTooltips( this:GetName(), "���������������ڵ���Ʒ\n#G��������Ʒ�����е�ĳ���ֽ�������#n" )
end

function OnlineStoreFrameAddBtn_OnClick()
	if selfStoreId > 0 then
		if gridCount >= MAX_GRID_NUM then
			return ShowMidTips( "�������޷����������Ļ���" );
		end
		local gridDef = StoreMgr:getStoreGridDef( gridCount );
		if gridDef == nil then
			return;
		end
		local base = math.ceil( goldRate / 20 );
		local newCost = GetCeilFormC( base * ( 0.2 * (gridCount - DEFAULT_GRID_NUM + 1) )^2 + base );
		local nowCost = string.gsub( OnlineStoreFrameRateFont:GetText(), "[^%d]", "" );

		OnlineStoreMessageBoxFrameTitle:SetText( "����ȷ��" );
		OnlineStoreMessageBoxFrameRich:SetText( "#cff7d00�������޷�" .. newCost - nowCost .."����#n\n�����" .. gridCount + 1 .. "�������Ҫʹ��#c00e600" .. gridDef.UseItemNum .."#n����" .. getItemDef( gridDef.UseItemID ).Name 
				.."��\n#c00e600ע������ĵ��̿����ڵ�ǰ��������ʹ��\#n\n�Ƿ�ȷ�����䣿", 255, 255, 190 )
		OnlineStoreMessageBoxFrame:SetClientUserData( 0, gridCount );
		OnlineStoreMessageBoxFrame:SetClientUserData( 1, gridDef.UseItemID );
		OnlineStoreMessageBoxFrame:SetClientUserData( 2, gridDef.UseItemNum );
		OnlineStoreMessageBoxFrame:Show();
	else
		return ShowMidTips( "�㵱ǰû�е��̣��޷��������" );
	end
end

function OnlineStoreFrameAddBtn_OnEnter()
	SetGameTooltips( this:GetName(),"���������1����̻���" );
end

function OnlineStoreFrameDecBtn_OnClick()
	if selfStoreId > 0 then
		if gridCount <= DEFAULT_GRID_NUM then
			return ShowMidTips( "��������Ļ����޷����" );
		else
			local btn = getglobal( "OnlineStoreFrameItem" .. gridCount );
			if btn:GetClientUserData(0) > 0 then
				return ShowMidTips( "��" .. gridCount .. "������Ѱڷ���Ʒ����ʱ�޷���������ȰѸ���Ʒ�¼�" );
			end
		end
		local base = math.ceil( goldRate / 20 );
		local newCost = GetCeilFormC( base * ( 0.2 * (gridCount - DEFAULT_GRID_NUM - 1) )^2 + base );
		local nowCost = string.gsub( OnlineStoreFrameRateFont:GetText(), "[^%d]", "" );
		OnlineStoreMessageBoxFrameTitle:SetText( "���ȷ��" );
		OnlineStoreMessageBoxFrameRich:SetText( "#cff7d00�������޷�" .. nowCost - newCost .. "����#n\n��" .. gridCount .. "����ܽ��������\n�Ƿ�ȷ�ϲ����", 255, 255, 190 );
		OnlineStoreMessageBoxFrame:SetClientUserData( 0, gridCount - 1 );
		OnlineStoreMessageBoxFrame:Show();
	else
		return ShowMidTips( "�㵱ǰû�е��̣��޷��������" );
	end
end

function OnlineStoreFrameDecBtn_OnEnter()
	SetGameTooltips( this:GetName(),"����ɲ��1����̻���" );
end

function OnlineStoreFrameRecordBtn_OnClick()
	local historyBtn = getglobal( "OnlineStoreFrameSaleHistoryBtn" );
	if historyBtn:IsShown() then
		historyBtn:Hide();
	else
		historyBtn:Show();
	end
end

function OnlineStoreFrameRocordBtn_OnEnter()
	SetGameTooltips( this:GetName(),"����ɲ鿴���׼�¼" );
end

function OnlineStoreFrameScrollUpBtn_OnClick()
	local slider = getglobal( "OnlineStoreFrameScrollBar" );
	if not slider:IsShown() then
		return;
	end

	local nValue = slider:GetValue() - slider:GetValueStep();
	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end

function OnlineStoreFrameScrollDownBtn_OnClick()
	local slider	= getglobal( "OnlineStoreFrameScrollBar" );
	if not slider:IsShown() then
		return;
	end

	local nMaxValue = slider:GetMaxValue();
	local nValue	= slider:GetValue() + slider:GetValueStep();
	
	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

function OnlineStoreFrameScrollBar_OnValueChanged()
	local slider	 = getglobal( "OnlineStoreFrameScrollBar" );
	local nValue	 = slider:GetValue();
	local nMaxValue	 = slider:GetMaxValue();
	local rich	 = getglobal( "OnlineStoreFrameAdRich" );
	if nValue <= nMaxValue then
		util.UpdateRich( rich:GetName(), nValue );
		updateWatchBtn( rich:GetTextLines(), nValue + 1 );
	end
end

function UpdateOnlineStoreAd( szText, storeId )
	local rich = getglobal( "OnlineStoreFrameAdRich" );
	table.insert( t_WatchBtn, { rich:GetTextLines() + 1, storeId } );
	rich:AddText( szText, 217, 219, 60 );
	UpdateStoreSliderRange();
end

function OnlineStoreSaleHistoryScrollUpBtn_OnClick()
	local slider = getglobal( "OnlineStoreSaleHistoryBtnScrollBar" );
	if not slider:IsShown() then
		return;
	end

	local nValue = slider:GetValue() - slider:GetValueStep();
	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end

function OnlineStoreSaleHistoryScrollDownBtn_OnClick()
	local slider	= getglobal( "OnlineStoreSaleHistoryBtnScrollBar" );
	if not slider:IsShown() then
		return;
	end

	local nMaxValue = slider:GetMaxValue();
	local nValue	= slider:GetValue() + slider:GetValueStep();
	
	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

function OnlineStoreSaleHistoryScrollBar_OnValueChanged()
	local slider	 = getglobal( "OnlineStoreSaleHistoryBtnScrollBar" );
	local nValue	 = slider:GetValue();
	local nMaxValue	 = slider:GetMaxValue();
	if nValue <= nMaxValue then
		util.UpdateRich( "OnlineStoreFrameSaleHistoryBtnRich", nValue );
	end
end

function OnlineStoreFramePriceTipBtn_OnEnter()
	SetGameTooltips( this:GetName(), "��Ʒ�ĵ���" );
end

function OnlineStoreFrameFundDescBtn_OnEnter()
	if this:GetText() == "�ʲ�" then
		SetGameTooltips( this:GetName(), "����ɽ����ʲ�����" );
	end
end

function OnlineStoreFrameFundTipBtn_OnEnter()
	if OnlineStoreFrameFundDescBtn:GetText() == "�ʲ�" then
		local szText = "#c00dcdc���̵��ʲ���\n1. ����ʱ��ȡ�ʲ���ά�ֵ��̵�����\n2. ������Ʒ�����룬���Զ������ʲ�\n3. �յ�ʱ�ʲ��Զ����뱳��\n#c00e6004. ���ʲ�����48Сʱ�󣬽��Զ��յ�";
		SetGameTooltips( this:GetName(), szText );
	else
		SetGameTooltips( this:GetName(), "��ǰ����Я��������" );
	end
end

function OnlineStoreFrameFundDescBtn_OnClick()
	if this:GetText() ~= "�ʲ�" then
		return;
	elseif selfStoreId == 0 then
		return ShowMidTips( "������������̴�ȡ�ʲ�" );
	end
	if not OnlineStoreMoneyOptFrame:IsShown() then
		OnlineStoreMoneyOptFrame:Show();
	else
		OnlineStoreMoneyOptFrame:Hide();
	end
end

function OnlineStoreFrameListTopBtn_OnEnter()
	local szText = "#cff9600�ʹڵ��̵�����λ����ߣ�\n#B��ʯ���̽����ڻʹڵ��̣�\n#G�������̽�������ʯ���̣�"
	SetGameTooltips( this:GetName(), szText );
end

function OnlineStoreFrameMoneyOpt_OnShow()
	StoreMgr:requestStorePos( selfStoreId );
	local edit = getglobal( this:GetName() .. "Edit" );
	edit:SetText( "" );
	SetFocusFrame( edit:GetName() );

	local font = getglobal( this:GetName() .. "Grid" );
	font:SetText( "��������" .. gridCount );
	font = getglobal( this:GetName() .. "Cost" );
	local base = math.ceil( goldRate / 20 );
	local cost = GetCeilFormC( base * ( 0.2 * (gridCount - DEFAULT_GRID_NUM) )^2 + base );
	font:SetText( "���޷ѣ�" .. cost );
	local btn = getglobal( this:GetName() .. "CostMoney" );
	btn:SetPoint( "topleft", this:GetName(), "topleft", (16 + font:GetTextExtentWidth(font:GetText()))/ GetScreenScaleY(), 76 );
	font = getglobal( this:GetName() .. "Total" );
	btn = getglobal( this:GetName() .. "TotalMoney" );
	btn:SetPoint( "topleft", this:GetName(), "topleft", (16 + font:GetTextExtentWidth(font:GetText()))/ GetScreenScaleY(), 128 );
	this:SetPoint( "center", "UIClient", "center", 0, 0 );
end

function OnlineStoreFrameMoneyOpt_OnHide()
	ClearFocus();
end

function OnlineStoreFrameBackBtn_OnClick()
	if selfStoreId == 0 then
		return ShowMidTips( "�������ܶԵ��̵���Ʒ���в���" );
	end
	if curGridIdx > 0 then
		local idx = curGridIdx - 1;
		local item = StoreMgr:getStoreItem( idx );
		if item ~= nil then
			StoreMgr:requestStoreDownItem( idx, item:getItemWId() )
		end
	else
		ShowMidTips( "����ѡ����Ҫ�¼ܵ���Ʒ" );
	end
end

function OnlineStoreFrameOpenBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	if this:GetText() == "��  ��" then
		local money = math.ceil( goldRate / 20 );
		local name = OnlineStoreFrameNameEdit:GetText();
		if name == "" then
			OnlineStoreFrameNameEdit:SetText( mainplayer:getName() .. "�ĵ���" );
			name = OnlineStoreFrameNameEdit:GetText();
		end
		OnlineStoreMessageBoxFrameTitle:SetText( "����ȷ��" );
		OnlineStoreMessageBoxFrameRich:SetText( "����ÿСʱ���ʲ�����#c00e600" .. money .. "#n���ҵ����޷�\n�Ƿ�ȷ�����ꣿ\n#c00e600ע��������޷��ٸ��ĵ���", 255, 255, 190 );
		OnlineStoreMessageBoxFrame:SetClientString( name );
		OnlineStoreMessageBoxFrame:SetClientUserData( 0, money );
		OnlineStoreMessageBoxFrame:Show();
	else
		if not OnlineStoreMoneyOptFrame:IsShown() then
			OnlineStoreMoneyOptFrame:Show();
		else
			OnlineStoreMoneyOptFrame:Hide();
		end
	end
end

function OnlineStoreFrameOpenBtn_OnEnter()
	if this:GetText() == "��  ��" then
		SetGameTooltips( this:GetName(), "����ɽ��п���" );
	else
		SetGameTooltips( this:GetName(), "����ɽ����ʲ�����" );
	end
end

function OnlineStoreFrameOptClose_OnEnter()
	SetGameTooltips( this:GetName(), "����ɽ����յ�" );
end

function OnlineStoreFrameOptClose_OnClick()
	if selfStoreId > 0 then
		OnlineStoreMessageBoxFrameTitle:SetText( "�յ�ȷ��" );
		OnlineStoreMessageBoxFrameRich:SetText( "�յ�󽫷����������еľ�Ӫ��ģ\n#ce600001.������������\n2.������������λ#n\n�Ƿ�ȷ���յ�", 255, 255, 190 );
		OnlineStoreMessageBoxFrame:SetClientUserData( 0, selfStoreId );
		OnlineStoreMessageBoxFrame:Show();
		OnlineStoreMessageBoxFrame:AddLevelRecursive();
	end
end

function OnlineStoreFrameBuyBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local container = mainplayer:getContainer();
	local lockState = container:getContainerLockState();
	if lockState == CONTAINER_LOCK_STATE_LOCK then
		return AddGameMiddleTips2("������֤��ȫ������");
	end
	if curGridIdx > 0 then
		ShowOnlineStoreItemOptFrame( curGridIdx - 1, "����ȷ��", StoreMgr:getStoreItemPrice( curGridIdx - 1 ) );
	else
		return ShowMidTips( "����ѡ����Ҫ�������Ʒ" );
	end
end

function OnlineStoreFrameStoreList_OnClick()
	local id = this:GetClientUserData(0);
	if id > 0 then
		StoreMgr:requestStoreView( id );
	end
end

function OnlineStoreFrameStoreList_OnEnter()
	if this:GetClientUserData(0) ~= 0 and curPageNum == 1 and this:GetClientID() == 1 then
		local _,desc = getRankIdx( this:GetClientID() );
		local szText = "\n1. ��������Խ�࣬��������Խ��\n2. ����ʱ��Խ������������Խ��\n3. �����ܶ�Խ�࣬��������Խ��";
		desc = desc .. szText;
		SetGameTooltips( this:GetName(), desc );
	end
end

function OnlineStoreFramePrePage_OnClick()
	local page = tonumber( OnlineStoreFramePageEdit:GetText() );
	if page == nil or page - 1 < 1 then
		page = 1;
	else
		page = page - 1;
	end
	OnlineStoreFramePageEdit:SetText( page );
	StoreMgr:requestStorePage( page - 1 );
end

function OnlineStoreFrameNextPage_OnClick()
	local total = pageCount > 0 and pageCount or 1;
	local page = tonumber( OnlineStoreFramePageEdit:GetText() );
	if page == nil then
		page = 1;
	elseif page + 1 > total then
		page = total;
	else
		page = page + 1;
	end
	OnlineStoreFramePageEdit:SetText( page );
	StoreMgr:requestStorePage( page - 1 );
end

function OnlineStoreFramePageEnterBtn_OnClick()
	local total = pageCount > 0 and pageCount or 1;
	local page = tonumber( OnlineStoreFramePageEdit:GetText() );
	if page == nil or page < 1 then
		page = 1;
	elseif page > total then
		page = total;
	end
	StoreMgr:requestStorePage( page - 1 );
end

function OnlineStoreFramePageEnterBtn_OnEnter()
	SetGameTooltips( this:GetName(), "�����ת�������ҳ��" );
end

function OnlineStoreFramePageEdit_OnFocusLost()
	local total = pageCount > 0 and pageCount or 1;
	local page = tonumber( this:GetText() );
	if page == nil or page < 1 then
		this:SetText( 1 );
	elseif page > total then
		this:SetText( total );
	end
end

function OnlineStoreFrameSaleHistoryBtn_OnShow()
end

function OnlineStoreFrameKeepTipBtn_OnEnter()
	local szText = "#c00dcdc��Ʒ�ı��ܷѣ�\n�۳���Ʒ����е�5%����Ϊ���ܷ�#n";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
						button = this:GetName(), horizonal = "right" } );
end

function OnlineStoreFrameRateTipBtn_OnEnter()
	local szText = "#c00dcdc���̵����޷ѣ�\n����ÿСʱ���ʲ����������޷�\n���̵Ļ���Խ�࣬�������޷�Խ��\n#c00e600�����޷����г�����Ӱ�측����#n";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
						button = this:GetName(), horizonal = "right" } );
end

function OnlineStoreFrameMoneyOpt_OnClick()
	local flag = this:GetText() == "��  ��" and 1 or 0;
	local num = tonumber( OnlineStoreMoneyOptFrameEdit:GetText() );
	if num == nil or num <= 0 then
		return ShowMidTips( "����ȷ������Ҫ" .. (flag > 0 and "����" or "ȡ��") .. "�ʲ������Ҷ��" );
	end
	if flag > 0 then
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer == nil or mainplayer:getMoney() < num then
			if mainplayer:getMoney() > 0 then
				num = mainplayer:getMoney();
			else
				return ShowMidTips( "����Я�������Ҳ��㣬�޷�����ö�ȵ�����" );
			end
		end
	else
		local fund = tonumber( OnlineStoreFrameFundFont:GetText() );
		if num > fund then
			if fund > 0 then
				num = fund;
			else
				return ShowMidTips( "�ʲ���ʣ�����Ҳ��㣬�޷�ȡ���ö�ȵ��ʲ�" );
			end
		end
	end
	StoreMgr:requestStoreMoneyOpt( flag, num );
	HideUIFrame(this:GetParent());
end

function ShowOnlineStoreItemOptFrame( idx, flag, price )
	local frame = getglobal( "OnlineStoreItemOptFrame" );
	OnlineStoreItemOptFrameTitle:SetText( flag );
	
	local item;
	local focusBtn = "ShowNumEdit";
	if flag == "��  ��" then
		local id = frame:GetClientUserData(2);
		if id ~= 0 then
			UnLockItem( CONTAINER_TYPE_SUNDRIES, frame:GetClientUserData(0), id );
		end
		focusBtn = "ShowPriceEdit";
		local mainplayer = ActorMgr:getMainPlayer();
		local container	= mainplayer:getContainer();
		item = container:getItem( CONTAINER_TYPE_SUNDRIES, idx );
		if item ~= nil then
			LockItem( CONTAINER_TYPE_SUNDRIES, idx, frame );
			OnlineStoreItemOptFrameShowPriceEdit:enableEdit( true );
		else
			return;
		end
	elseif flag == "����ȷ��" then
		item = StoreMgr:getStoreItem( idx );
		if item == nil or price == 0 then
			return;
		end
		OnlineStoreItemOptFrameShowPriceEdit:enableEdit( false );
	end
	
	if price == nil then
		price = math.max( 1, item:getSellPrice() );
		local historyPrice = GetHistoryItemPrice( item:getItemId() )
		if historyPrice > 0 then
			price = historyPrice
		end
	end
	local num = item:getNum();
	frame:SetClientUserData( 0, idx );
	frame:SetClientUserData( 1, num );
	frame:SetClientUserData( 2, item:getItemId() );
	frame:SetClientString( flag );
	local itemDef = item:getItemDef();
	local t_tgas = { [frame:GetName().."ItemBtnIconTexture"] = { path = util.getItemIconPath( { ["itemID"] = itemDef.ItemID } ) }, };
	util.SetTgas( t_tgas );

	local t_edites	= {	[frame:GetName().."ShowPriceEdit"]	= { value = price }, 
				[frame:GetName().."ShowNumEdit"]		= { value = flag == "��  ��" and num or 1 },
						};
	util.SetEdites( t_edites );

	local t_itemNameColor = GetItemNameColor(itemDef);
	local t_fonts = {	[frame:GetName().."ItemNameBtnFont"]	= { value = itemDef.Name, color = { r = t_itemNameColor[1], g = t_itemNameColor[2], b = t_itemNameColor[3] } }, 
				[frame:GetName().."TotalPriceBtnFont"]	= { value = price * num, },
				[frame:GetName().."Title"]		= { value = optType, },
				};
	util.SetFonts( t_fonts );

	local edit = getglobal( frame:GetName()..focusBtn );
	SetFocusFrame( edit:GetName() );
	edit:SelectAllText();
	
	local timeBtn = getglobal( frame:GetName().."ShiXiaoTiShiBtn" );
	if item:getItemValidEndTime() > 0 then
		timeBtn:Show();
	else
		timeBtn:Hide();
	end

	if not frame:IsShown() then
		frame:Show();
	end
end

function OnlineStoreItemOptFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 0, 0 );
end

function OnlineStoreItemOptFrame_OnHide()
	if this:GetClientString() == "��  ��" then
		local idx = this:GetClientUserData(0);
		local id = this:GetClientUserData(2);
		UnLockItem( CONTAINER_TYPE_SUNDRIES, idx, id );
	end
	this:SetClientUserData( 0, 0 );
	this:SetClientUserData( 1, 0 );
	this:SetClientUserData( 2, 0 );
	this:SetClientString( "" );
end

function OnlineStoreItemOptFrame_OnUpdate()
	local font	= getglobal( this:GetName().."TotalPriceBtnFont" );
	local numEdit	= getglobal( this:GetName().."ShowNumEdit" );
	local nNum	= tonumber( numEdit:GetText() );
	local nTotalPrice = 0;
	if nNum == nil or nNum == 0 then
		font:SetText( 0 );
		return;
	end

	local priceEdit	= getglobal( this:GetName().."ShowPriceEdit" );
	local nPrice	= tonumber( priceEdit:GetText() );
	if nPrice == nil or nPrice == 0 then
		font:SetText( 0 );
		return;
	end

	font:SetText( nPrice * nNum );
end

function OnlineStoreItemOptFrameItemBtn_OnEnter()
	local frame = getglobal( this:GetParent() );
	local idx = frame:GetClientUserData(0);
	local optType = frame:GetClientString();
	local item;
	if optType == "��  ��" then
		local mainplayer = ActorMgr:getMainPlayer();
		local container	= mainplayer:getContainer();
		item = container:getItem( CONTAINER_TYPE_SUNDRIES, idx );
	elseif optType == "����ȷ��" then
		item = StoreMgr:getStoreItem( idx );
	end

	if item ~= nil then
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item, nil, this:GetName(), nil, nil, nil, (optType == "����ȷ��" and idx or nil) );
	end
end

function OnlineStoreItemOptFrameShowPriceEdit_OnUpdate()
	CheckEditNumberInput( this );
end

function OnlineStoreItemOptFrameShowPriceEdit_OnTabPressed()
	local edit = getglobal( this:GetParent().."ShowNumEdit" );
	SetFocusFrame( edit:GetName() );
	edit:SelectAllText();
end

function OnlineStoreItemOptFrameDelNumBtn_OnClick()
	local edit = getglobal( this:GetParent().."ShowNumEdit" );
	local num = tonumber( edit:GetText() );
	if num == nil or num < 1 then
		edit:SetText( 1 );
	elseif num > 1 then
		edit:SetText( num - 1 );
	end
end

function OnlineStoreItemOptFrameAddNumBtn_OnClick()
	local total = OnlineStoreItemOptFrame:GetClientUserData(1);
	local edit = getglobal( this:GetParent().."ShowNumEdit" );
	if total > 0 then
		local num = tonumber( edit:GetText() );
		if num == nil or num < 1 then
			edit:SetText( 1 );
		elseif num < total then
			edit:SetText( num + 1 );
		end
	else
		edit:SetText( 1 );
	end
end

function OnlineStoreItemOptFrameShowNumEdit_OnUpdate()
	local total = OnlineStoreItemOptFrame:GetClientUserData(1);
	if total > 0 then
		local num = tonumber( this:GetText() );
		if num == nil or num < 1 then
			this:SetText( 1 );
		elseif num > total then
			this:SetText( total );
		end
	else
		this:SetText( 1 );
	end
end

function OnlineStoreItemOptFrameShowNumEdit_OnTabPressed()
	local edit = getglobal( this:GetParent().."ShowPriceEdit" );
	SetFocusFrame( edit:GetName() );
	edit:SelectAllText();
end

function OnlineStoreItemOptFrameConfirmBtn_OnClick()
	local frame = getglobal( this:GetParent() );
	local optType = frame:GetClientString();
	local idx = frame:GetClientUserData( 0 );
	local item;
	local num = tonumber( OnlineStoreItemOptFrameShowNumEdit:GetText());
	if num == nil or num <= 0 then
		return ShowMidTips( "��������ȷ������" );
	end
	local price = tonumber( OnlineStoreItemOptFrameShowPriceEdit:GetText());
	if price == nil or price <= 0 then
		if optType == "��  ��" then
			return ShowMidTips( "��������ȷ�ļ۸�" );
		else
			HideUIFrame(this:GetParent());
			return ShowMidTips( "��Ʒ���ݴ����޷����й������" );
		end
	end

	if optType == "��  ��" then
		local mainplayer = ActorMgr:getMainPlayer();
		local container	= mainplayer:getContainer();
		item = container:getItem( CONTAINER_TYPE_SUNDRIES, idx );
		if item ~= nil then
			StoreMgr:requestStoreUpItem( idx, price, num );
		end
	elseif optType == "����ȷ��" then
		item = StoreMgr:getStoreItem( idx );
		StoreMgr:requestStoreBuyItem( curStoreId, idx, price, num, item:getItemWId() );
	end
	
	HideUIFrame(this:GetParent());
end

function OnlineStoreMessageBoxFrame_OnLoad()
	OnlineStoreMessageBoxFrameRich:setCenterLine( true );
end

function OnlineStoreMessageBoxFrameAcceptButton_OnClick()
	local title = OnlineStoreMessageBoxFrameTitle:GetText();
	local parent = getglobal( this:GetParent() );
	if title == "����ȷ��" then
		local idx = parent:GetClientUserData(0);
		local itemId = parent:GetClientUserData(1);
		local num = parent:GetClientUserData(2);
		local MainPlayer = ActorMgr:getMainPlayer();
		if MainPlayer == nil then return end
		local container	= MainPlayer:getContainer();
		if container:getItemCount( itemId ) >= num then
			StoreMgr:requestStoreAddGrid( idx );
		else
			--SetMessageBoxForItem( itemId );
			ShowMidTips( getItemDef(itemId).Name .. "���㣬�޷���������" );
		end
	elseif title == "����ȷ��" then
		local money = parent:GetClientUserData( 0 );
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer == nil or money > mainplayer:getMoney() then 
			ShowBuyMoneyFrame();
		else
			StoreMgr:requestStoreOpen( parent:GetClientString() );
		end
	elseif title == "�յ�ȷ��" then
		local storeId = parent:GetClientUserData( 0 );
		StoreMgr:requestStoreClose( storeId );
	elseif title  == "���ȷ��" then
		local idx = parent:GetClientUserData(0);
		StoreMgr:requestStoreDecGrid( idx );
	end
	parent:Hide();
end

function OpenVipStore()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	if mainplayer:getLv() < 20 then
		return ShowMidTips( "��Ҫ�ȼ��ﵽ20������ʹ�õ���" );
	elseif mainplayer:isDead() then
		return ShowMidTips( "���Ѿ��������޷�ʹ�õ���" );
	elseif mainplayer:isOutSide() then
		return ShowMidTips( "���ڿ��״̬���޷�ʹ�õ���" );
	end

	if CheckCanOpenStoreFrame() then
		StoreMgr:requestVipStore();
	end
end