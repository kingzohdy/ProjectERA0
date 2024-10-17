local VIPBarInfo =[[

#c00ff00	��ӭ�������Ԫ��Ȩ���ɣ��������ܵ�
#c00ff00	ר����������Ϸ��Ȩ��
#cffffbe	1.������Ȩ����ֻ�ȡ��������#EXP#%
#cffffbe	2.������Ȩ����ֵ�����������#FARM#%
#cffffbe	3.�Ƹ���Ȩ��#MONEYDESC#

#cff0000	��ʾ�������������������Ԫ����һ����
			�ܸ�����Ȩ�ӳɣ�ͬһ��������Խ�࣬�ӳ�
			Խ�࣡]]


function VIPBarMessageFrameCloseBtn_onClick()
	util.closeParent(this);
end

function MessageWindowFrameCloseBtn_onClick()
	util.closeParent(this);
end

function MessageWindowFrameRichText_OnClick()
	local _,id,Type = string.match( arg2,"((%d+),(%d+))" );
	if tonumber(Type) == 1 then		
		local itemDef = getItemDef( tonumber(id) );
		ShowItemTips( LUA_ITEMTIPS_TYPE_LINK, LUA_ITEMTIPS_ITEMDEF, itemDef );
	elseif tonumber(Type) == 2 then
		ShowGMWatchPetFrame( tonumber(id) );
	end
end

function ShowGMMessagBox( bullentin )
	if bullentin.Type == 1 then
		MessageWindowFrame:Show();
		MessageWindowFrameText:SetText( bullentin.Text,255,255,190 );
	elseif bullentin.Type == 2 then
		GMMessageTipsFrame:SetClientString( bullentin.Text );
		GMMessageTipsFrame:Show();
	end
end

function GMMessageTipsFrameBtn_OnClick()
	GMWebFrame:Show();
	local szUrl = GMMessageTipsFrame:GetClientString();
	GMWebFrameWeb:OpenSite( szUrl );
	GMMessageTipsFrame:Hide();
end

function GMWebFrame_OnShow()
	GMWebFrameWeb:ShowWindow(true);
end

function GMWebFrame_OnHide()
	GMWebFrameWeb:ShowWindow(false)
end

local ModelAngle = 0;

function ShowGMWatchPetFrame( petId )
	local petDef = getPetDef( petId )
	if petDef == nil then return end
	local modelView = getglobal("GMWatchPetFrameModelBtnModelView");
	local nameFont = getglobal( "GMWatchPetFrameNameFont" );
	local lvValue = getglobal("GMWatchPetFrameLvValue");
	nameFont:SetText( petDef.Name );
	lvValue:SetText( petDef.FightLevel .. "��" )
	local scale = GetNpcDisplayInfo( petDef.MouldID ).size == 0 and 1 or GetNpcDisplayInfo( petDef.MouldID ).size;
	modelView:setPetModel( 0,10100,GetModePathById( petDef.MouldID ) );	
	modelView:setPetScale( 0,scale,scale,scale );
	modelView:setEntityPosition( 0, 0, 40, 0 );
	ModelAngle = 0;
	GMWatchPetFrame:Show();
end

function GMWatchPetFrame_TurnLeftBtn_OnMouseDown()
	local modelView = getglobal("GMWatchPetFrameModelBtnModelView");
	ModelAngle = ModelAngle - 5;
	modelView:setEntityRotate(0,ModelAngle);
end

function GMWatchPetFrame_TurnRightBtn_OnMouseDown()
	local modelView = getglobal("GMWatchPetFrameModelBtnModelView");
	ModelAngle = ModelAngle + 5;
	modelView:setEntityRotate(0,ModelAngle);
end

----------------------------	vip ����	---------------------------------

function CheckMainPlayerVipBar()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local netBarInfo = mainplayer:getNetBarInfo();
	if netBarInfo.DynValueNum > 0 then
		VIPBarMessageFrame:Show();
		local addExp = netBarInfo.DynValue[1] + netBarInfo.DynValue[3];
		local addFarm = netBarInfo.DynValue[2] + netBarInfo.DynValue[4];
		local addMoney = "";
		--[[
		for i=1,GameWizard:GetVipNetBarGiftNum() do
			local BarGift = GameWizard:GetVipNetBarGift( i - 1 );
			local szTimeDesc = "";
			if BarGift.GiftCond.ContOnlineTM >= 60 then
				szTimeDesc = math.ceil(BarGift.GiftCond.ContOnlineTM/60) .. "Сʱ";
			else
				szTimeDesc = BarGift.GiftCond.ContOnlineTM  .. "����";
			end
			addMoney = addMoney .. "����" .. szTimeDesc .."���" .. BarGift.Money .. GetMoneyDesc(BarGift.MoneyType) .."\n\t\t";
		end
		]]--
		addMoney = "ÿ����������60���ӿ��Ի��\n\t\t15��ȯ��ÿ����������120���ӿ��Ի��\n\t\t35��ȯ����;���ߺ����¼��㡣";
		local t_ReplaceInfo = {
								["#EXP#"] = addExp,
								["#FARM#"] = addFarm,
								["#MONEYDESC#"] = addMoney,
								}
		local szText = VIPBarInfo;
		for name, szRep in pairs(t_ReplaceInfo) do
			local v = string.find(szText,name);
			if v ~= 0 then
				szText = string.gsub(szText,name,szRep);
			end
		end
		local infoRichtext =getglobal("VIPBarMessageFrameInfo");
		infoRichtext:SetText( szText, 111, 82, 32 );
	end
end

-------------------------		ý�����		----------------------------------------

local MAX_NUM_PER_LINE = 8
local MAX_GET_ALL_GIFT = 64
local MAX_SELECT_ONE_GIFT = 64
local MAX_NEXT_SHOW_GIFT = 128
function ClearMediaGiftData()
	for i=1,MAX_GET_ALL_GIFT do
		local btn = getglobal( "GMGiftsFrameAllGift" .. i )
		if btn ~= nil then
			btn:Hide();
			btn:DisChecked();
			btn:SetClientUserData( 0,0 )
		end
	end
	for i=1,MAX_GET_ALL_GIFT do
		local btn = getglobal( "GMGiftsFrameSelctGift" .. i )
		if btn ~= nil then
			btn:Hide();
			btn:DisChecked();
			btn:SetClientUserData( 0,0 )
		end
	end
	for i=1,MAX_GET_ALL_GIFT do
		local btn = getglobal( "GMGiftsFrameNextGift" .. i )
		if btn ~= nil then
			btn:Hide();
			btn:DisChecked();
			btn:SetClientUserData( 0,0 )
		end
	end
	GMGiftsFrameCaptionFont:SetText("")
	GMGiftsFrameAllRich:SetText("",0,0,0)
	GMGiftsFrameAllRich:resizeRect( 356,100 )
	GMGiftsFrameSelectRich:SetText("",0,0,0)
	GMGiftsFrameSelectRich:resizeRect( 356,100 )
	GMGiftsFrameNextRich:SetText("",0,0,0)
	GMGiftsFrameNextRich:resizeRect( 356,100 )
	GMGiftsFrameDescRich:resizeRect( 356,100 )
	SetScrollBar( "GMGiftsFrame_ScrollBar", 0, 0 );
	-- ѡ�����
	nCurGiftSelectIdx = 0;
end

function ShowMediaGiftFrame( nListType,GridIndex,nGiftId )
	ClearMediaGiftData();
	GMGiftsFrameDescRich:SetText("",0,0,0)

	local scrollFrame = getglobal( "GMGiftsFrameScrollFrame" );
	scrollFrame:SetValueStep( 5 )
	scrollFrame:SetVerticalScroll( 0 );

	--��ʾ���
	local MediaGift = GetMediaGiftDef( nGiftId );
	GMGiftsFrameCaptionFont:SetText( MediaGift.GiftDesc )
	-- ȫ����ȡ�������
	local nHeight = 50
	GMGiftsFrameAllRich:SetPoint( "topleft","GMGiftsFrame","topleft",21,nHeight );
	GMGiftsFrameAllRich:SetText( MediaGift.SelectAllGift.GiftDesc,255,255,190 );	
	GMGiftsFrameAllRich:SetHeight( GMGiftsFrameAllRich:GetTotalHeight() / GetScreenScaleY() )
	nHeight = nHeight + GMGiftsFrameAllRich:GetHeight();
	local nIndex = 1;
	for i=1,MEDIAGIFT_OPTIONS_ITEM_MAX_TOLUA do
		if nIndex > MAX_GET_ALL_GIFT then
			break
		end
		if MediaGift.SelectAllGift.GiftItems[i-1].ItemID > 0 then
			local ItemDef = getItemDef( MediaGift.SelectAllGift.GiftItems[i-1].ItemID )
			local btn = getglobal( "GMGiftsFrameAllGift" .. nIndex );
			if btn == nil then
				btn = CreateButton( "GMGiftsFrameAllGift" .. nIndex, "GMGiftTemplate", scrollFrame );
			end
			btn:SetPoint( "topleft", "GMGiftsFrame", "topleft", 16 + ((nIndex-1)%MAX_NUM_PER_LINE)*45, nHeight + math.floor((nIndex-1)/MAX_NUM_PER_LINE)*45 )
			btn:SetClientUserData( 0,ItemDef.ItemID )
			btn:SetClientID( nIndex )
			btn:Show();
			local IconTex = getglobal( btn:GetName() .. "IconTexture" )			
			local IconPath = GetItemEquipIconPath()..ItemDef.IconID..".tga";
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			IconTex:SetTexture( IconPath );
			SetEquipImportantEffectUV( ItemDef, btn:GetName() .. "UVAnimationTex" );
			local CountFont = getglobal( btn:GetName() .. "CountFont" )
			if MediaGift.SelectAllGift.GiftItems[i-1].ItemNum > 1 then
				CountFont:SetText( MediaGift.SelectAllGift.GiftItems[i-1].ItemNum )
			else
				CountFont:SetText("")
			end
			nIndex = nIndex + 1;
		end
	end
	nHeight = nHeight + math.floor((nIndex-1)/MAX_NUM_PER_LINE)*45;
	if nIndex > 1 then
		nHeight = nHeight + 5 + (((nIndex-1)%MAX_NUM_PER_LINE ~=0) and 45 or 0 )
	end

	-- ��ѡ���������
	nIndex = 1;
	GMGiftsFrameSelectRich:SetPoint( "topleft","GMGiftsFrame","topleft",21,nHeight );
	GMGiftsFrameSelectRich:SetText( MediaGift.SelectOneGift.GiftDesc,255,255,190 );
	GMGiftsFrameSelectRich:SetHeight( GMGiftsFrameSelectRich:GetTotalHeight() / GetScreenScaleY() )
	nHeight = nHeight + GMGiftsFrameSelectRich:GetHeight();
	for i=1,MEDIAGIFT_OPTIONS_ITEM_MAX_TOLUA do
		if nIndex > MAX_SELECT_ONE_GIFT then
			break
		end
		if MediaGift.SelectOneGift.GiftItems[i-1].ItemID > 0 then
			local ItemDef = getItemDef( MediaGift.SelectOneGift.GiftItems[i-1].ItemID )
			local btn = getglobal( "GMGiftsFrameSelctGift" .. nIndex );
			if btn == nil then
				btn = CreateButton( "GMGiftsFrameSelctGift" .. nIndex, "GMGiftTemplate", scrollFrame );
			end
			btn:SetPoint("topleft", "GMGiftsFrame", "topleft", 16 + ((nIndex-1)%MAX_NUM_PER_LINE)*45, nHeight + math.floor((nIndex-1)/MAX_NUM_PER_LINE)*45 )
			btn:SetClientUserData( 0,ItemDef.ItemID )
			btn:SetClientID( nIndex )
			btn:Show();
			local IconTex = getglobal( btn:GetName() .. "IconTexture" )			
			local IconPath = GetItemEquipIconPath()..ItemDef.IconID..".tga";
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			IconTex:SetTexture( IconPath );
			SetEquipImportantEffectUV( ItemDef, btn:GetName() .. "UVAnimationTex" );
			local CountFont = getglobal( btn:GetName() .. "CountFont" )
			if MediaGift.SelectOneGift.GiftItems[i-1].ItemNum > 1 then
				CountFont:SetText( MediaGift.SelectOneGift.GiftItems[i-1].ItemNum )
			else
				CountFont:SetText("")
			end
			nIndex = nIndex + 1;
		end
	end
	nHeight = nHeight + math.floor((nIndex-1)/MAX_NUM_PER_LINE)*45;
	if nIndex > 1 then
		nHeight = nHeight + 5 + (((nIndex-1)%MAX_NUM_PER_LINE ~=0) and 45 or 0 )
	end

	-- �¸��������
	nIndex = 1;
	GMGiftsFrameNextRich:SetPoint( "topleft","GMGiftsFrame","topleft",21,nHeight );
	GMGiftsFrameNextRich:SetText( MediaGift.PreViewGift.GiftDesc,255,255,190 );
	GMGiftsFrameNextRich:SetHeight( GMGiftsFrameNextRich:GetTotalHeight() / GetScreenScaleY() )
	nHeight = nHeight + GMGiftsFrameNextRich:GetHeight();
	local nextMediaGift = GetMediaGiftDef( MediaGift.PreViewGift.MediaGiftID );
	if nextMediaGift ~= nil then		
		for i=1,MEDIAGIFT_OPTIONS_ITEM_MAX_TOLUA do
			if nIndex > MAX_NEXT_SHOW_GIFT then
				break
			end
			if nextMediaGift.SelectAllGift.GiftItems[i-1].ItemID > 0 then
				local ItemDef = getItemDef( nextMediaGift.SelectAllGift.GiftItems[i-1].ItemID )
				local btn = getglobal( "GMGiftsFrameNextGift" .. nIndex );
				if btn == nil then
					btn = CreateButton( "GMGiftsFrameNextGift" .. nIndex, "GMGiftTemplate", scrollFrame );					
				end
				btn:SetPoint("topleft", "GMGiftsFrame", "topleft", 16 + ((nIndex-1)%MAX_NUM_PER_LINE)*45, nHeight + math.floor((nIndex-1)/MAX_NUM_PER_LINE)*45 )
				btn:SetClientUserData( 0,ItemDef.ItemID )
				btn:SetClientID( nIndex )
				btn:Show();
				local IconTex = getglobal( btn:GetName() .. "IconTexture" )			
				local IconPath = GetItemEquipIconPath()..ItemDef.IconID..".tga";
				if not IsInExistence( IconPath ) then
					IconPath = DEFAULT_ITEM_PATH;
				end
				IconTex:SetTexture( IconPath );
				SetEquipImportantEffectUV( ItemDef, btn:GetName() .. "UVAnimationTex" );
				local CountFont = getglobal( btn:GetName() .. "CountFont" )
				if nextMediaGift.SelectAllGift.GiftItems[i-1].ItemNum > 1 then
					CountFont:SetText( nextMediaGift.SelectAllGift.GiftItems[i-1].ItemNum )
				else
					CountFont:SetText("")
				end
				nIndex = nIndex + 1;
			end
		end
		for i=1,MEDIAGIFT_OPTIONS_ITEM_MAX_TOLUA do
			if nIndex > MAX_NEXT_SHOW_GIFT then
				break
			end
			if nextMediaGift.SelectOneGift.GiftItems[i-1].ItemID > 0 then
				local ItemDef = getItemDef( nextMediaGift.SelectOneGift.GiftItems[i-1].ItemID )
				local btn = getglobal( "GMGiftsFrameNextGift" .. nIndex );
				if btn == nil then
					btn = CreateButton( "GMGiftsFrameNextGift" .. nIndex, "GMGiftTemplate", scrollFrame );
				end
				btn:SetPoint("topleft", "GMGiftsFrame", "topleft", 16 + ((nIndex-1)%MAX_NUM_PER_LINE)*45, nHeight + math.floor((nIndex-1)/MAX_NUM_PER_LINE)*45 )
				btn:SetClientUserData( 0,ItemDef.ItemID )
				btn:SetClientID( nIndex )
				btn:Show();
				local IconTex = getglobal( btn:GetName() .. "IconTexture" )			
				local IconPath = GetItemEquipIconPath()..ItemDef.IconID..".tga";
				if not IsInExistence( IconPath ) then
					IconPath = DEFAULT_ITEM_PATH;
				end
				IconTex:SetTexture( IconPath );
				SetEquipImportantEffectUV( ItemDef, btn:GetName() .. "UVAnimationTex" );
				local CountFont = getglobal( btn:GetName() .. "CountFont" )
				if nextMediaGift.SelectOneGift.GiftItems[i-1].ItemNum > 1 then
					CountFont:SetText( nextMediaGift.SelectOneGift.GiftItems[i-1].ItemNum )
				else
					CountFont:SetText("")
				end
				nIndex = nIndex + 1;
			end
		end
	end

	scrollFrame:ResetChildAnchor();
	SetScrollBar( "GMGiftsFrame_ScrollBar", 0, scrollFrame:GetVerticalScrollRange() );
	-- ���������
	if scrollFrame:GetVerticalScrollRange() > 0 then
		GMGiftsFrameScrollBackGroundTex:Show();
		GMGiftsFrameScrollBackGroundTexTop:Show();
		GMGiftsFrameScrollBackGroundTexBottom:Show();

		GMGiftsFrameGiftsBack:SetSize( 370+21,363 )
		GMGiftsFrame:SetSize( 390+21,439 )
	else
		GMGiftsFrameScrollBackGroundTex:Hide();
		GMGiftsFrameScrollBackGroundTexTop:Hide();
		GMGiftsFrameScrollBackGroundTexBottom:Hide();

		GMGiftsFrameGiftsBack:SetSize( 370,363 )
		GMGiftsFrame:SetSize( 390,439 )
	end

	if GMGiftsFrame:GetClientUserData( 0 ) ~= 0 then
		UnLockItem( GMGiftsFrame:GetClientUserData(0),GMGiftsFrame:GetClientUserData(1),0 )
	end
	GMGiftsFrame:SetClientUserData( 0,nListType )
	GMGiftsFrame:SetClientUserData( 1,GridIndex )
	LockItem( nListType, GridIndex, 0 );
	GMGiftsFrame:Show();
end

-- �齱�������
function ShowLtrGiftFrame( nListType,GridIndex,nItemId,nGiftId )
	ClearMediaGiftData();
	local LtrGiftDef = GetLtrGiftDef( nGiftId )
	if LtrGiftDef == nil then return end
	local nCount = ( GameWizard:getLtrGiftCount( nItemId ) % LtrGiftDef.MaxTime );
	if nCount >= LtrGiftDef.MaxTime then return end
	while( nCount >= LtrGiftDef.RangeTime ) do
		LtrGiftDef = GetLtrGiftDef( LtrGiftDef.NextLtrGift.LtrID )
		if LtrGiftDef == nil then return end
	end

	local scrollFrame = getglobal( "GMGiftsFrameScrollFrame" );
	scrollFrame:SetValueStep( 5 )
	scrollFrame:SetVerticalScroll( 0 );

	local nHeight = 50;	
	
	local szDesc = LtrGiftDef.GiftDesc;
	local t_ReplaceInfo = { ["%%num"] = nCount, ["%%total"] = LtrGiftDef.MaxTime,	}

	for name, szRep in pairs(t_ReplaceInfo) do
		local v = string.find(szDesc,name);
		if v ~= 0 then
			szDesc = string.gsub(szDesc,name,szRep);
		end
	end
	GMGiftsFrameDescRich:SetPoint( "topleft","GMGiftsFrame","topleft",21,nHeight );
	GMGiftsFrameDescRich:resizeRect( 356, 100 )
	GMGiftsFrameDescRich:SetText(szDesc,255,255,190)
	GMGiftsFrameDescRich:SetHeight( GMGiftsFrameDescRich:GetTotalHeight() / GetScreenScaleY() + 1 )
	nHeight = nHeight + GMGiftsFrameDescRich:GetHeight();

	local nIndex = 1;
	GMGiftsFrameAllRich:SetPoint( "topleft","GMGiftsFrame","topleft",21,nHeight );
	GMGiftsFrameAllRich:resizeRect( 356, 100 )
	GMGiftsFrameAllRich:SetText( LtrGiftDef.LtrGiftItems.GiftDesc,255,255,190 );
	GMGiftsFrameAllRich:SetHeight( GMGiftsFrameAllRich:GetTotalHeight() / GetScreenScaleY() + 1 )
	nHeight = nHeight + GMGiftsFrameAllRich:GetHeight();
	
	for i=1,MEDIAGIFT_OPTIONS_ITEM_MAX_TOLUA do
		if nIndex > MAX_GET_ALL_GIFT then
			break
		end
		if LtrGiftDef.LtrGiftItems.GiftItems[i-1].ItemID > 0 then
			local ItemDef = getItemDef( LtrGiftDef.LtrGiftItems.GiftItems[i-1].ItemID )
			local btn = getglobal( "GMGiftsFrameAllGift" .. nIndex );
			if btn == nil then
				btn = CreateButton( "GMGiftsFrameAllGift" .. nIndex, "GMGiftTemplate", scrollFrame );
			end
			btn:SetPoint("topleft", "GMGiftsFrame", "topleft", 16 + ((nIndex-1)%MAX_NUM_PER_LINE)*45, nHeight + math.floor((nIndex-1)/MAX_NUM_PER_LINE)*45 )
			btn:SetClientUserData( 0,ItemDef.ItemID )
			btn:SetClientID( nIndex )
			btn:Show();
			local IconTex = getglobal( btn:GetName() .. "IconTexture" )			
			local IconPath = GetItemEquipIconPath()..ItemDef.IconID..".tga";
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			IconTex:SetTexture( IconPath );
			SetEquipImportantEffectUV( ItemDef, btn:GetName() .. "UVAnimationTex" );
			local CountFont = getglobal( btn:GetName() .. "CountFont" )
			if LtrGiftDef.LtrGiftItems.GiftItems[i-1].ItemNum > 1 then
				CountFont:SetText( LtrGiftDef.LtrGiftItems.GiftItems[i-1].ItemNum )
			else
				CountFont:SetText("")
			end
			nIndex = nIndex + 1;
		end
	end
	nHeight = nHeight + math.floor((nIndex-1)/MAX_NUM_PER_LINE)*45;
	if nIndex > 1 then
		nHeight = nHeight + 5 + (((nIndex-1)%MAX_NUM_PER_LINE ~=0) and 45 or 0 )
	end
	-- ��ʾ��һ������Ľ�������	
	NextLtrGiftDef = GetLtrGiftDef( LtrGiftDef.NextLtrGift.LtrID )
	if NextLtrGiftDef ~= nil then
		nIndex = 1;
		GMGiftsFrameNextRich:SetPoint( "topleft","GMGiftsFrame","topleft",21,nHeight );
		GMGiftsFrameNextRich:resizeRect( 356, 100 )
		GMGiftsFrameNextRich:SetText( LtrGiftDef.NextLtrGift.Desc,255,255,190 )
		GMGiftsFrameNextRich:SetHeight( GMGiftsFrameNextRich:GetTotalHeight() / GetScreenScaleY() + 1 )
		nHeight = nHeight + GMGiftsFrameNextRich:GetHeight();
		if LtrGiftDef.NextLtrGift.Desc ~= "" then
			nHeight = nHeight + GMGiftsFrameNextRich:GetHeight();
		end
		for i=1,MEDIAGIFT_OPTIONS_ITEM_MAX_TOLUA do
			if nIndex > MAX_GET_ALL_GIFT then
				break
			end
			if NextLtrGiftDef.LtrGiftItems.GiftItems[i-1].ItemID > 0 then
				local ItemDef = getItemDef( NextLtrGiftDef.LtrGiftItems.GiftItems[i-1].ItemID )
				local btn = getglobal( "GMGiftsFrameNextGift" .. nIndex );
				if btn == nil then
					btn = CreateButton( "GMGiftsFrameNextGift" .. nIndex, "GMGiftTemplate", scrollFrame );
				end
				btn:SetPoint("topleft", "GMGiftsFrame", "topleft", 16 + ((nIndex-1)%MAX_NUM_PER_LINE)*45, nHeight + math.floor((nIndex-1)/MAX_NUM_PER_LINE)*45 )
				btn:SetClientUserData( 0,ItemDef.ItemID )
				btn:SetClientID( nIndex )
				btn:Show();
				local IconTex = getglobal( btn:GetName() .. "IconTexture" )
				local IconPath = GetItemEquipIconPath()..ItemDef.IconID..".tga";
				if not IsInExistence( IconPath ) then
					IconPath = DEFAULT_ITEM_PATH;
				end
				IconTex:SetTexture( IconPath );
				SetEquipImportantEffectUV( ItemDef, btn:GetName() .. "UVAnimationTex" );
				local CountFont = getglobal( btn:GetName() .. "CountFont" )
				if NextLtrGiftDef.LtrGiftItems.GiftItems[i-1].ItemNum > 1 then
					CountFont:SetText( NextLtrGiftDef.LtrGiftItems.GiftItems[i-1].ItemNum )
				else
					CountFont:SetText("")
				end
				nIndex = nIndex + 1;
			end
		end
	end

	scrollFrame:ResetChildAnchor();
	SetScrollBar( "GMGiftsFrame_ScrollBar", 0, scrollFrame:GetVerticalScrollRange() );

	-- ���������
	if scrollFrame:GetVerticalScrollRange() > 0 then
		GMGiftsFrameScrollBackGroundTex:Show();
		GMGiftsFrameScrollBackGroundTexTop:Show();
		GMGiftsFrameScrollBackGroundTexBottom:Show();
		GMGiftsFrameScrollUpBtn:Show();
		GMGiftsFrameScrollDownBtn:Show();
		GMGiftsFrameGiftsBack:SetSize( 370+21,363 )
		GMGiftsFrame:SetSize( 390+21,439 )
	else
		GMGiftsFrameScrollBackGroundTex:Hide();
		GMGiftsFrameScrollBackGroundTexTop:Hide();
		GMGiftsFrameScrollBackGroundTexBottom:Hide();
		GMGiftsFrameScrollUpBtn:Hide();
		GMGiftsFrameScrollDownBtn:Hide();
		GMGiftsFrameGiftsBack:SetSize( 370,363 )
		GMGiftsFrame:SetSize( 390,439 )
	end

	if GMGiftsFrame:GetClientUserData( 0 ) ~= 0 then
		UnLockItem( GMGiftsFrame:GetClientUserData(0),GMGiftsFrame:GetClientUserData(1),0 )
	end
	GMGiftsFrame:SetClientUserData( 0,nListType )
	GMGiftsFrame:SetClientUserData( 1,GridIndex )
	LockItem( nListType, GridIndex, 0 );
	GMGiftsFrame:Show();
end

function GMGiftsFrame_OnShow()
	this:SetPoint("center","$parent","center",0,0)
end

function GMGiftsFrame_OnHide()
	if GMGiftsFrame:GetClientUserData( 0 ) ~= 0 then
		UnLockItem( GMGiftsFrame:GetClientUserData(0),GMGiftsFrame:GetClientUserData(1),0 )
	end
end

function GMGiftsFrameItemBtn_OnEnter()
	local itemId =this:GetClientUserData( 0 );
	local itemDef = getItemDef( itemId );
	ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, itemDef );
end

function GMGiftsFrameItemBtn_OnClick()
	if string.find( this:GetName(),"SelctGift",1,true ) then
		for i=1,MAX_GET_ALL_GIFT do
			local btn = getglobal( "GMGiftsFrameSelctGift" .. i )
			if btn ~= nil then
				btn:DisChecked();
			end
		end
		nCurGiftSelectIdx = this:GetClientID();
		this:Checked();
	end
end

function GMGiftsFrameOpenBtn_onClick()
	if GMGiftsFrameSelctGift1 ~= nil and GMGiftsFrameSelctGift1:IsShown() and nCurGiftSelectIdx == 0 then
		ShowMidTips("����ѡ��һ����Ʒ")
		return
	end
	local mainplayer = ActorMgr:getMainPlayer();
	local container = mainplayer:getContainer();
	local nList = GMGiftsFrame:GetClientUserData(0);
	local nGrid = GMGiftsFrame:GetClientUserData(1);
	container:send_UsedExpendableItem( nList,nGrid,0,nCurGiftSelectIdx );
	GMGiftsFrame:Hide();
end

function GMGiftsFrameScrollFrameOnScrollRangeChanged()
	-- ����������Ϊ
	local scrollFrame	= getglobal( "GMGiftsFrameScrollFrame" );
	SetScrollBar( "GMGiftsFrame_ScrollBar",  0, scrollFrame:GetVerticalScrollRange() );
	if scrollFrame:GetVerticalScrollRange() > 0 then
		GMGiftsFrameScrollBackGroundTex:Show();
		GMGiftsFrameScrollBackGroundTexTop:Show();
		GMGiftsFrameScrollBackGroundTexBottom:Show();
		GMGiftsFrameScrollUpBtn:Show();
		GMGiftsFrameScrollDownBtn:Show();

		GMGiftsFrameGiftsBack:SetSize( 370+21,363 )
		GMGiftsFrame:SetSize( 390+21,439 )
	else
		GMGiftsFrameScrollBackGroundTex:Hide();
		GMGiftsFrameScrollBackGroundTexTop:Hide();
		GMGiftsFrameScrollBackGroundTexBottom:Hide();
		GMGiftsFrameScrollUpBtn:Hide();
		GMGiftsFrameScrollDownBtn:Hide();

		GMGiftsFrameGiftsBack:SetSize( 370,363 )
		GMGiftsFrame:SetSize( 390,439 )
	end
end

function GMGiftsFrameScrollUpBtnOnClick()
	local slider = getglobal("GMGiftsFrame_ScrollBar");
	local nValue = slider:GetValue() - slider:GetValueStep();
	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end

function GMGiftsFrameScrollDownBtnOnClick()
	local slider = getglobal("GMGiftsFrame_ScrollBar");
	local nValue = slider:GetValue() + slider:GetValueStep();
	local nMaxValue = slider:GetMaxValue();
	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

function GMGiftsFrame_ScrollBar_OnValueChanged()
	local slider	 = getglobal( "GMGiftsFrame_ScrollBar" );
	local nValue	 = slider:GetValue();
	local nMaxValue	 = slider:GetMaxValue();
	local nLastValue = slider:GetLastValue();	
	local nStep		 = slider:GetValueStep();
	
	if nValue <= nMaxValue then
		local scrollFrame	= getglobal( "GMGiftsFrameScrollFrame" );
		scrollFrame:IncreaseVerticalScroll( nValue - nLastValue );
	end
end