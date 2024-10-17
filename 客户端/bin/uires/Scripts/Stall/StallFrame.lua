function StallPetListBtnFrame_Test_OnShow()
	local tex = getglobal( this:GetName().."PetBtn8PartionTex" );
	tex:Hide();
end

function CallScript_StallChgAdRes( nFlag )
	if nFlag == STALL_CHG_AD_FAILED then
		local t_StallSaleFrameControl = GetStallSaleFrameControl();
		GameStallManager:localChangeAd( t_StallSaleFrameControl["advertiseBeforeModify"] );
	elseif STALL_CHG_AD_SUCCESS == nFlag then
		local selfStallInfo = GameStallManager:getSelfStallItem();	
		StallSaleFrame_StallAdvertiseBtnFont:SetText( selfStallInfo.m_szStallAd );
	end
end

function CallScript_ActorStall( nStallSate, nActorID )
	local actor	= ActorMgr:FindActor( nActorID );
	if nStallSate == CLIENT_STALL_START then
		CreateStallBtn( actor );
	else
		HideStallBtn( actor,true );

		local t_StallBuyFrameBiaoJiStalls = GetStallBuyFrameBiaoJiStalls();
		t_StallBuyFrameBiaoJiStalls:cancelBiaoJi( { ["name"] = actor:getName(), } );
	end
end

-- ֪ͨ���ǰ�̯״̬�仯
function CallScript_UpdateStallState( nStallSate, nActorID )
	-- ����ϴΰ�̯��������
	local t_StallSaleFrameControl = GetStallSaleFrameControl();
	t_StallSaleFrameControl:clearTotalIncome();
	
	local actor	= ActorMgr:FindActor( nActorID );
	if nStallSate == CLIENT_STALL_START then
		local mainplayer		= ActorMgr:getMainPlayer();		
		CreateStallBtn( actor );

		local t_showIcons = { "ContainerFrame_EndStallageBtn" };
		util.ShowIcons( t_showIcons );

		local t_hideIcons = { "ContainerFrame_StallageBtn", };
		util.HideIcons( t_hideIcons );
		
		-- ��ʼ��̯��ʱ���ٻ����еĳ���
		local mainplayer	= ActorMgr:getMainPlayer();
		local GamePet		= mainplayer:getPet();
		GamePet:callBackAllPets();

	elseif nStallSate == CLIENT_STALL_END then
		local t_showIcons = { "ContainerFrame_StallageBtn" };
		util.ShowIcons( t_showIcons );

		local t_hideIcons = { "ContainerFrame_EndStallageBtn", };
		util.HideIcons( t_hideIcons );		
		HideStallBtn( actor,false );
		if StallSaleFrame:IsShown() then
			StallSaleFrame:Hide();
		else
			GameStallManager:SaveLastStallGoodsToStall();
		end

		ClearItemLock( "StallSaleFrame" );
	end

	UpdateStartStallOrEndStallBtn( );
end

function ShangJiaPetToStallSaleFrame( nGrid )
	-- ��鱻ѡ�еĳ����Ƿ��Ѿ���̯λ�ϣ�����̯λ�ϣ����ܼӵ�̯λ��
	if GameStallManager:isInStallIPet( SHITYPE, nGrid ) then
		ShowMidTips( "�ó����Ѿ���̯λ��" );
		return;
	end

	-- ����̯��̯λ�ϳ����Ƿ��Ѿ�����
	if GameStallManager:isStallPetListFull() then
		ShowMidTips( "̯λ�ϳ��������" );
		return;
	end

	if GameExchange:isInExchangePet( SHITYPE, nGrid ) then
		ShowMidTips( "��ѡ�����Ѿ��ڽ�������" );
		return;
	end

	-- ���Ѿ����ڰ�̯״̬�����ܽ�������ӵ�̯λ��
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local petInfo		= petPackage:getPetInfo( nGrid );
	local petWID		= petInfo.WID;
	if petPackage:isPetInFreedom( petWID.nHeight, petWID.nLow ) then
		ShowMidTips( "�ó����Ѿ������������ɽ��иò�����" );
		return;
	end

	local t_StallSellerShangJiaPetFrameControl = GetStallSellerShangJiaPetFrameControl();
	t_StallSellerShangJiaPetFrameControl:setPetInfo( { heightId = petWID.nHeight, lowId = petWID.nLow, id = petInfo.PetID, index = nGrid } );
	StallSellerShangJiaPetFrame:Show();
end

local t_StallSaleFrameControl = {	item = { grid = -1, list = -1, selectIndex = 0, id = 0 }, 
									pet = { heightId = 0, lowId = 0, id = 0, selectIndex = -1, }, totalIncome = 0,
									stallPageType = CLIENT_STALL_TYPE_ITEM, advertiseBeforeModify = "" };

function GetStallSaleFrameControl()
	return t_StallSaleFrameControl;
end

t_StallSaleFrameControl["addTotalIncome"] =
function ( self, data )
	self["totalIncome"] = self["totalIncome"] + data["incrIncome"];
end

t_StallSaleFrameControl["clearTotalIncome"] =
function ( self )
	self["totalIncome"] = 0;
end

t_StallSaleFrameControl["getTotalIncome"] =
function ( self )
	return self["totalIncome"];
end

t_StallSaleFrameControl["setSelectItem"] =
function ( self, data )
	self["item"]["selectIndex"] = data["selectIndex"];
	self["item"]["id"]			= data["id"];
	self["item"]["grid"]		= data["grid"];
	self["item"]["list"]		= data["list"];
end

t_StallSaleFrameControl["clearSelectItem"] =
function ( self )
	self["item"]["selectIndex"] = 0;
	self["item"]["id"]			= 0;
	self["item"]["grid"]		= -1;
	self["item"]["list"]		= -1;
end

t_StallSaleFrameControl["setSelectPet"] =
function ( self, data )
	self["pet"]["selectIndex"]	= data["selectIndex"];
	self["pet"]["heightId"]		= data["heightId"];
	self["pet"]["lowId"]		= data["lowId"];
end

t_StallSaleFrameControl["clearSelectPet"] =
function ( self )
	self["pet"]["selectIndex"]	= 0;
	self["pet"]["heightId"]		= 0;
	self["pet"]["lowId"]		= 0;
end

-- �ص�����
function Sale2Stall( used_Item, nType, nIndex )
	local MainPlayer	= ActorMgr:getMainPlayer();
	local container		= MainPlayer:getContainer();
	local item			= container:getItem( nType, nIndex );
	local itemdef		= item:getItemDef();
	-- NoDealֵΪ1��ʱ�򣬱�ʾ�����������֮�佻��

	local nError = CanExChange( item );
	if nError ~= 0 then
		ShowMidTips( GetExChangeErrorMsg( nError, "stall" ) );
		return;
	end

	local mainplayer = ActorMgr:getMainPlayer();
	
	-- ���̯λ�Ƿ��Ѿ���
	if GameStallManager:isStallItemListFull() then
		ShowMidTips( "̯λ�����������޷��ϼܸ������Ʒ" );
		return;
	end

	local desGrid = 0;
	local StallData = GameStallManager:getSelfStallItem();
	for i = 0, GameStallManager:getStallGridNum()-1 do
		local StallItem = StallData.m_ItemData[i];
		if StallItem:getItemId() == 0 then
			desGrid = i;
			break;
		end
	end

	local t_StallSellerShangJiaItemFrameControl = GetStallSellerShangJiaItemFrameControl();
	if StallSellerShangJiaItemFrame:IsShown() then
		ReleaseStallSellerShangJiaItemFrame_LockItem();
	end
	
	t_StallSellerShangJiaItemFrameControl:setItemInfo( { srcList = nType, srcGrid = nIndex, destGrid = desGrid } );
	StallSellerShangJiaItemFrame:Show();
	return;
end

-- ����ǰ�̯��������е��
function StallSaleLeftBtnClick( szBtnName )
	local btn		= getglobal( szBtnName );
	local StallData = GameStallManager:getSelfStallItem();
	local nGrid		= btn:GetClientID();
	local item		= StallData.m_ItemData[nGrid-1];
	if item:getItemId() == 0 then
		return;
	end

	t_StallSaleFrameControl:setSelectItem( { grid = item:getGridIdx(), list = item:getListType(), selectIndex = nGrid, id = item:getItemId() } );
	DisCheckAllBtn( "StallSaleFrameItem", CLIENT_STALL_MAX_ITEM_PAGE );
	btn:Checked();

	UpdateStallSaleFrameSelectItemPrice();
end

-- CORE: ���и��º���
-- 1.
function UpdateStallSaleFrameSelectItemPrice()
	if t_StallSaleFrameControl["stallPageType"] == CLIENT_STALL_TYPE_ITEM then
		Private_UpdateStallSalePriceByItem();
	elseif t_StallSaleFrameControl["stallPageType"] == CLIENT_STALL_TYPE_PET then
		Private_UpdateStallSalePriceByPet();
	end
end

-- 2.
function UpdateStallSaleFrameAllIncome()
	local nTotalIncome	= t_StallSaleFrameControl:getTotalIncome();
	StallSaleFrame_AllIncomeBtnFont:SetText( nTotalIncome );
end

-- 3.
function UpdateStallSaleFrame()
	if t_StallSaleFrameControl["stallPageType"] == CLIENT_STALL_TYPE_ITEM then
		Private_UpdateStallSaleItemFrame();
	elseif t_StallSaleFrameControl["stallPageType"] == CLIENT_STALL_TYPE_PET then
		Private_UpdateStallSalePetFrame();

		if StallSaleFramePetViewList:IsShown() then
			UpdateSelfOwnPetList( "StallSaleFramePetViewList" );
		end
	end
end

-- 4.
function UpdateStallSaleFrameUI()
	-- ����ǰҪ��ʾ���������
	if t_StallSaleFrameControl["stallPageType"] == CLIENT_STALL_TYPE_ITEM then
		Private_ShowStallSaleFrameItemUI();
	-- ����ǰҪ��ʾ���������
	elseif t_StallSaleFrameControl["stallPageType"] == CLIENT_STALL_TYPE_PET then
		Private_ShowStallSaleFramePetUI();
	end
end

-- 5.
function UpdateStallSaleFrameCheckLabel()
	local t_labelBtn = { "StallSaleFrame_ItemLabelBtn", "StallSaleFrame_PetLabelBtn" };
	for _, name in ipairs( t_labelBtn ) do
		local btn = getglobal( name );
		btn:DisChecked();
	end
	local seeStallInfo	= GameStallManager:getSelfStallItem();
	--local btn	=	getglobal("StallBuyFrame_PetLabelBtn");
	if seeStallInfo.m_nPetNum ~=	0 then
		StallSaleFrame_PetLabelBtn:Checked();
	end
	if t_StallSaleFrameControl["stallPageType"]	== CLIENT_STALL_TYPE_ITEM then
		StallSaleFrame_ItemLabelBtn:Checked();
	elseif t_StallSaleFrameControl["stallPageType"]	== CLIENT_STALL_TYPE_PET then
		StallSaleFrame_PetLabelBtn:Checked();
	end
end

-- 6. ���¿�ʼ��̯��ť�ͽ�����̯��ť
function UpdateStartStallOrEndStallBtn()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer:isInStall() then
		StallSaleFrame_EndStallBtn:Show();
		StallSaleFrame_StartStallBtn:Hide();
	else
		StallSaleFrame_EndStallBtn:Hide();
		StallSaleFrame_StartStallBtn:Show();
	end
end

-- CORE: ���и��º���

local t_StallSaleFrameEvents = 
{ 
	["GE_STALL_GRIDNUM_CHG"] = {},	["GE_STALL_SELLER_CHG"] = {},	["GE_SELECT_MAINPLAYER_STALL_MODEL"] = {},	["GE_STOP_STALL_BY_DEAD"] = {},
};

t_StallSaleFrameEvents["GE_STALL_GRIDNUM_CHG"].func = 
function ()
end

t_StallSaleFrameEvents["GE_STALL_SELLER_CHG"].func = 
function ()
	if StallSaleFrame:IsShown() then
		UpdateStallSaleFrame();
	end
end

t_StallSaleFrameEvents["GE_SELECT_MAINPLAYER_STALL_MODEL"].func = 
function ()
	local frame = getglobal( "StallSaleFrame" );
	if not frame:IsShown() then
		frame:Show();
	end
end

t_StallSaleFrameEvents["GE_STOP_STALL_BY_DEAD"].func = 
function ()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer:isInStall() then
		GameStallManager:stall_End();
		StallSaleFrame_SaleHistoryBtnRich:Clear();
		if StallSaleFrame:IsShown() then
			StallSaleFrame:Hide();
		end
	end
end

function StallSaleFrame_OnLoad()	
	for i = 1, CLIENT_STALL_MAX_ITEM_PAGE do
		local salebtn	= getglobal( "StallSaleFrameItem"..i.."BackTexture" );
		salebtn:Show();
	end

	local t_hideIcons = { this:GetName().."Pet8PartionTex", };
	util.HideIcons( t_hideIcons );

	--ע���̵����������Ӧ�Ļص�����
	ItemFirstCallBack( "StallSaleFrame" , Sale2Stall );

	for event, _ in pairs( t_StallSaleFrameEvents ) do
		this:RegisterEvent( event );
	end

	t_StallSaleFrameEvents.__index = function ( tab, key )
									tab[key] = { func = function () end };
									return tab[key];
								end
	setmetatable( t_StallSaleFrameEvents, t_StallSaleFrameEvents );
end

function StallSaleFrame_OnEvent()
	t_StallSaleFrameEvents[arg1].func();
end

local szDefaultAd = "�߹�·����Ҫ������������ʺ���ĺö���";
--local szDefaultAd = "������";
-- �򿪰�̯�������
function Private_AutoLockLastStallItem()
	local selfStallInfo = GameStallManager:getSelfLastStallItem();	
	for i = 1, CLIENT_STALL_MAX_ITEM_PAGE do
		local item		= selfStallInfo.m_ItemData[i-1];
		local nItemId	= item:getItemId();
		if nItemId ~= 0 then
			
			if IsItemLock( item:getListType(), item:getGridIdx(), nItemId ) then
				GameStallManager:delLastStallItem( i - 1 );
			else
				LockItem( item:getListType(), item:getGridIdx(), item:getItemId(), "StallSaleFrame" );
			end
		end
	end
end

function StallSaleFrame_OnShow()
	-- Ĭ����ʾ��Ʒҳ��
	t_StallSaleFrameControl["stallPageType"]	= CLIENT_STALL_TYPE_ITEM;
	UpdateStallSaleFrameCheckLabel();
	UpdateStallSaleFrameSelectItemPrice();
	UpdateStallSaleFrameAllIncome();
	UpdateStartStallOrEndStallBtn();

	local mainplayer		= ActorMgr:getMainPlayer();
	local szDefStallName	= mainplayer:getName().."��̯λ";
	
	if not mainplayer:isInStall() then
		GameStallManager:ValidateLastStallGoods();
		Private_AutoLockLastStallItem();
		GameStallManager:PutLastStallGoodsToStall();

		local selfLastStallInfo = GameStallManager:getSelfLastStallItem();
		
		if selfLastStallInfo.m_szStallName ~= "" then
			GameStallManager:stall_ChangeName( selfLastStallInfo.m_szStallName );
		else
			GameStallManager:stall_ChangeName( szDefStallName );
		end

		if selfLastStallInfo.m_szStallAd ~= "" then
			GameStallManager:localChangeAd( selfLastStallInfo.m_szStallAd );
		else
			GameStallManager:localChangeAd( szDefaultAd );
		end
	end
	
	local selfStallInfo = GameStallManager:getSelfStallItem();	
	
	local t_fonts = {	["StallSaleFrame_StallNameBtnFont"]			= { value = selfStallInfo.m_szStallName..
																		( mainplayer:isInStall() and "" or "������༭̯λ���ƣ�" ) }, 
						["StallSaleFrame_StallAdvertiseBtnFont"]	= { value = selfStallInfo.m_szStallAd }, 
					};
	util.SetFonts( t_fonts );

	local t_edites = {	["StallSaleFrame_StallNameEdit"]		= { value = selfStallInfo.m_szStallName },
						["StallSaleFrame_StallAdvertiseEdit"]	= { value = selfStallInfo.m_szStallAd },
					};
	util.SetEdites( t_edites );

	local t_hideIcons = {	"StallSaleFrame_StallNameEdit", "StallSaleFramePetViewList", "StallSaleFrame_StallAdvertiseEdit" };
	util.HideIcons( t_hideIcons );

	local t_showIcons = { "StallSaleFrame_StallNameBtn", "StallSaleFrame_StallAdvertiseBtn" };
	util.ShowIcons( t_showIcons );

	UpdateStallSaleFrame();
	UpdateStallSaleFrameUI();

	--this:SetPoint( "center", "$parent", "center", 0, 0  );
end

-- �ر����Ұ�̯���
function StallSaleFrame_OnHide()
	local mainplayer = ActorMgr:getMainPlayer();
	if not mainplayer:isInStall() then
		ClearItemLock( "StallSaleFrame" );
		GameStallManager:SaveLastStallGoodsToStall();
		GameStallManager:resetStallInfo();
	end
	
	-- ��ԭ����check�Ŀؼ�״̬��ȡ��
	DisCheckAllBtn( "StallSaleFramePet", CLIENT_STALL_MAX_PET );
	DisCheckAllBtn( "StallSaleFramePetViewListPetBtn", CLIENT_STALL_MAX_PET );

	local t_hideIcons = { "StallSellerShangJiaItemFrame", "StallSellerShangJiaPetFrame", "StallSaleFramePetViewList" };
	util.HideIcons( t_hideIcons );
	
	t_StallSaleFrameControl:clearSelectPet();
	t_StallSaleFrameControl:clearSelectItem();
	bOpenContainerFlag = true;
end

-- ���Ұ�̯�����߱�ǩ��ť
function StallSaleFrame_ItemLabelBtn_OnClick()
	if t_StallSaleFrameControl["stallPageType"] ==  CLIENT_STALL_TYPE_ITEM then
		return;
	end

	t_StallSaleFrameControl["stallPageType"]	= CLIENT_STALL_TYPE_ITEM;
	UpdateStallSaleFrameUI();
	UpdateStallSaleFrameCheckLabel();
	
	if not PlayerContainerFrame:IsShown() then
		StallSaleFrame:SetPoint("TOPLEFT", "$parent", "TOPLEFT", 185, 140 );
		PlayerContainerFrame:Show();
		PlayerContainerFrame:SetPoint("TOPLEFT", "$parent", "TOPLEFT",StallSaleFrame:GetWidth() + 185, 140 );
	end

	UpdateStallSaleFrame();
end

-- ���Ұ�̯�������ǩ��ť
function StallSaleFrame_PetLabelBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then 
		return 
	end
	local petPackage    =  mainplayer:getPet();
	if petPackage:getPetNum() == 0 then
		ShowMidTips( "�޷�����������ڻ�û�г��" );
		return;
	end
	--[[
	if t_StallSaleFrameControl["stallPageType"] ==  CLIENT_STALL_TYPE_PET then
		return;
	end
	--]]

	t_StallSaleFrameControl["stallPageType"]	= CLIENT_STALL_TYPE_PET;
	
	UpdateStallSaleFrameUI();
	UpdateStallSaleFrameCheckLabel();
	UpdateStallSaleFrame();
end

function StallSaleFrame_CloseBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	if not mainplayer:isInStall() then
		StallSaleFrame:Hide();
		return;
	end
	
	local confirmFrame = getglobal( "StallSeller_CloseStallFrmaeConfirmFrame" );
	if confirmFrame:IsShown() then
		return;
	end

	confirmFrame:Show();
end

-- ���̯λ���ְ�ť
function StallSaleFrame_StallNameBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer:isInStall() then
		ShowMidTips( "��̯״̬���ɱ༭̯λ����" );
		return;
	end

	this:Hide();

	local nameEidt = getglobal( "StallSaleFrame_StallNameEdit" );
	nameEidt:Show();

	local selfStallInfo = GameStallManager:getSelfStallItem();	
	nameEidt:SetText( selfStallInfo.m_szStallName );
	SetFocusFrame( nameEidt:GetName() );
	nameEidt:SelectAllText();
end

-- ̯λ���������ʧȥ����
function StallNameNormalShowEdit_OnFocusLost()
	local nameEidt		= getglobal( "StallSaleFrame_StallNameEdit" );
	local szInputText	= nameEidt:GetText();
	local formatText	= eraseSpace( szInputText );
	if formatText ~= "" then
		GameStallManager:stall_ChangeName( formatText );
	end
	
	local selfStallInfo = GameStallManager:getSelfStallItem();	
	StallSaleFrame_StallNameBtnFont:SetText( selfStallInfo.m_szStallName );
	StallSaleFrame_StallNameBtn:Show();
	this:Hide();
end

-- ���̯λ��水ť
function StallSaleFrame_StallAdvertiseBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	--[[
	if mainplayer:isInStall() then
		ShowMidTips( "��̯״̬���ɱ༭̯λ���" );
		return;
	end
	--]]
	local selfStallInfo = GameStallManager:getSelfStallItem();	
	this:Hide();
	local edit = getglobal( "StallSaleFrame_StallAdvertiseEdit" );
	edit:Show();
	edit:SetText( selfStallInfo.m_szStallAd );
	SetFocusFrame( edit:GetName() );
	edit:SelectAllText();
end

-- ̯λ��������ʧȥ����
function StallSaleFrame_StallAdvertiseEdit_OnFocusLost()
	-- localChangeAd
	local edit			= getglobal( "StallSaleFrame_StallAdvertiseEdit" );
	local szInputText	= edit:GetText();
	local formatText	= eraseSpace( szInputText );
	local mainplayer = ActorMgr:getMainPlayer();
	--[[
	if formatText ~= "" then
		if mainplayer:isInStall() then
			Private_InStall_StallAdvertiseEdit_OnFocusLost( formatText );
		else
			Private_OutStall_StallAdvertiseEdit_OnFocusLost( formatText );
		end		
	else
		LostAdvertiseEdit();
	end	
	--]]

	if mainplayer:isInStall() then
		Private_InStall_StallAdvertiseEdit_OnFocusLost( formatText );
	else
		Private_OutStall_StallAdvertiseEdit_OnFocusLost( formatText );
	end
end

function LostAdvertiseEdit()
	local selfStallInfo = GameStallManager:getSelfStallItem();	
	StallSaleFrame_StallAdvertiseBtnFont:SetText( selfStallInfo.m_szStallAd );
	StallSaleFrame_StallAdvertiseBtn:Show();
	StallSaleFrame_StallAdvertiseEdit:Hide();
end

-- ���ڰ�̯״̬�޸Ĺ��
function Private_InStall_StallAdvertiseEdit_OnFocusLost( szAdvertise )
	local selfStallInfo = GameStallManager:getSelfStallItem();	
	t_StallSaleFrameControl["advertiseBeforeModify"] = selfStallInfo.m_szStallAd;
	LostAdvertiseEdit();

	GameStallManager:stall_ChangeAd( szAdvertise );
end

-- �����ڰ�̯״̬�޸Ĺ��
function Private_OutStall_StallAdvertiseEdit_OnFocusLost( szAdvertise )
	GameStallManager:localChangeAd( szAdvertise );
	LostAdvertiseEdit();
end

--�鿴̯λ�Ľ��׼�¼
function StallSaleFrame_TradeRecordBtn_OnClick()
	local historyBtn = getglobal( "StallSaleFrame_SaleHistoryBtn" );
	if historyBtn:IsShown() then
		historyBtn:Hide();
	else
		historyBtn:Show();
	end
end

function CallScript_StallUpSuccRes( nStallType, nStallGrid )
	UpdateStallSaleFrame();
	local selfStallInfo = GameStallManager:getSelfStallItem();
	if nStallType == CLIENT_STALL_TYPE_ITEM then
		local item = selfStallInfo.m_ItemData[nStallGrid];
		LockItem( item:getListType(), item:getGridIdx(), item:getItemId(), "StallSaleFrame" );
	end
end

-- �����Ʒ�¼ܰ�ť
-- ��̯״̬�¼�
function StallDownRes( nStallType, nStallGrid, nList, nGrid, nId )
	UpdateStallSaleFrame();
	local selfStallInfo = GameStallManager:getSelfStallItem();
	if nStallType == CLIENT_STALL_TYPE_ITEM then
		UnLockItem( nList, nGrid, nId );
		RemoveCheckItem( "StallSaleFrame" );

		local t_selectItemData	= t_StallSaleFrameControl["item"];
		local nSelectIndex		= t_selectItemData["selectIndex"];
		if nSelectIndex == 0 then
			return;
		end
		
		if nStallGrid ~= ( nSelectIndex - 1 ) then
			return;
		end
		
		t_StallSaleFrameControl:clearSelectItem();
	elseif nStallType == CLIENT_STALL_TYPE_PET then
		local t_selectPetData	= t_StallSaleFrameControl["pet"];
		local nSelectIndex		= t_selectPetData["selectIndex"];
		if nSelectIndex == 0 then
			return;
		end
		
		if nStallGrid ~= ( nSelectIndex - 1 ) then
			return;
		end

		t_StallSaleFrameControl:clearSelectPet();
	end
end

function Private_XiaJiaInStall()
	local selfStallInfo = GameStallManager:getSelfStallItem();
	if t_StallSaleFrameControl["stallPageType"] == CLIENT_STALL_TYPE_ITEM then
		local t_selectItemData	= t_StallSaleFrameControl["item"];
		local nSelectIndex		= t_selectItemData["selectIndex"];
		if nSelectIndex == 0 then
			ShowMidTips( "����ѡ����Ҫ�¼ܵ���Ʒ" );
			return;
		end

		GameStallManager:delItemInStall( nSelectIndex - 1 );
	elseif t_StallSaleFrameControl["stallPageType"] == CLIENT_STALL_TYPE_PET then
		local t_selectPetData	= t_StallSaleFrameControl["pet"];
		local nSelectIndex		= t_selectPetData["selectIndex"];
		if nSelectIndex == 0 then
			ShowMidTips( "����ѡ����Ҫ�¼ܵ���Ʒ" );
			return;
		end
		
		GameStallManager:delPetInStall( nSelectIndex - 1 );
	end
end

function Private_XiaJiaNotInStall()
	local selfStallInfo = GameStallManager:getSelfStallItem();
	if t_StallSaleFrameControl["stallPageType"] == CLIENT_STALL_TYPE_ITEM then
		local t_selectItemData	= t_StallSaleFrameControl["item"];
		local nSelectIndex		= t_selectItemData["selectIndex"];
		if nSelectIndex == 0 then
			ShowMidTips( "����ѡ����Ҫ�¼ܵ���Ʒ" );
			return;
		end

		local item = selfStallInfo.m_ItemData[nSelectIndex-1];
		UnLockItem( item:getListType(), item:getGridIdx(), item:getItemId() );--����
		GameStallManager:delStallItem( nSelectIndex - 1 );
		-- TODO: DEL
		RemoveCheckItem( "StallSaleFrame" ); -- �Ƴ���Ʒѡ��״̬
		t_StallSaleFrameControl:clearSelectItem();

	elseif t_StallSaleFrameControl["stallPageType"] == CLIENT_STALL_TYPE_PET then
		local t_selectPetData	= t_StallSaleFrameControl["pet"];
		local nSelectIndex		= t_selectPetData["selectIndex"];
		if nSelectIndex == 0 then
			ShowMidTips( "����ѡ����Ҫ�¼ܵ���Ʒ" );
			return;
		end
		
		GameStallManager:delStallPet( nSelectIndex - 1 );
		-- ͬʱ�������б��еĳ���������ɫ�����������ɫ
		if StallSaleFramePetViewList:IsShown() then
			UpdateSelfOwnPetList( "StallSaleFramePetViewList" );
		end
		t_StallSaleFrameControl:clearSelectPet();
	end

	UpdateStallSaleFrame();
end

function StallSaleFrame_XiaJiaItemBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer:isInStall() then
		Private_XiaJiaInStall()
	else
		Private_XiaJiaNotInStall();
	end
end

-- ��ʼ��̯
local t_StallSaleFrameMutexFrame = { "StallSellerShangJiaItemFrame", "StallSellerShangJiaPetFrame" };
function StallSaleFrame_StartStallBtn_OnClick()
	local selfStallInfo = GameStallManager:getSelfStallItem();
	local mainplayer = ActorMgr:getMainPlayer();
	mainplayer:breakBTypePre();
	if selfStallInfo.m_szStallName == "" then
		ShowMidTips( "̯λ���ֲ���Ϊ��" );
		return;
	end
	
	if GameStallManager:isNoneGoodsOnStall() then
		ShowMidTips( "û����Ʒ�ϼܣ����ܰ�̯" );
		return;
	end

	-- �����������ڽ���״̬�����ܽ��а�̯
	if GameExchange:isInExchange() then
		ShowMidTips( "�����ڽ���״̬���޷���̯" );
		return;
	end

	if OnlineStoreFrame:IsShown() then
		return ShowMidTips( "�鿴�����У��޷����а�̯����" );
	end
	
	--��������Ӹ���״̬���ܰ�̯
	if TeamFollowHintFrame:IsShown() then
		ShowMidTips( "��������Ӹ���״̬���޷����а�̯����" );
		return;
	end

	-- �����Ǵ�����ͨ����״̬���ϸ���
	if FollowHintFrame:IsShown() then
		GameFollowManager:cancelWeiXing();
	end
	
	-- ����Ʒ�̼ҵĲ�����������ʾ�����ܽ��а�̯����
	for _, name in ipairs( t_StallSaleFrameMutexFrame ) do
		local mutexFrame = getglobal( name );
		if mutexFrame:IsShown() then
			ShowMidTips( "���ȹرյ�ǰ�򿪵���Ʒ�ϼܵĲ�����" );
			return;
		end
	end

	GameStallManager:stall_Start( selfStallInfo.m_szStallName );
end

-- ������̯
function StallSaleFrame_EndStallBtn_OnClick()
	EndStallConfirmFrame:Show();
end

-- ��������б�ť
function StallSaleFrame_OwnPetListBtn_OnClick()
	local btn = getglobal( "StallSaleFramePetViewList" );
	if btn:IsShown() then
		btn:Hide();
	else
		btn:Show();
	end
end

-- ˢ�°�̯���������ʾ�İ�ť
-- ��ʾ���Ұ�̯���ĵ��������
function Private_ShowStallSaleFrameItemUI()
	for i = 1, CLIENT_STALL_MAX_ITEM_PAGE do
		local itemframe = getglobal( "StallSaleFrameItem"..i );
		itemframe:Show();
	end

	for i = 1, CLIENT_STALL_MAX_PET do
		local petframe = getglobal( "StallSaleFramePet"..i );
		petframe:Hide();
	end

	local t_hideIcons = { "StallSaleFrame_OwnPetListBtn", "StallSaleFramePetViewList" };
	util.HideIcons( t_hideIcons );
end

-- ˢ�����Ұ�̯���ĵ��������
function Private_UpdateStallSaleItemFrame()
	DisCheckAllBtn( "StallSaleFrameItem", CLIENT_STALL_MAX_ITEM_PAGE );
	local allSelfItem		= GameStallManager:getSelfStallItem();
	local t_selectItemInfo	= t_StallSaleFrameControl["item"];
	local nSelectItemIndex	= t_selectItemInfo["selectIndex"];
	for i = 1, CLIENT_STALL_MAX_ITEM_PAGE do
		local item		= allSelfItem.m_ItemData[i-1];
		local stallbtn	= getglobal( "StallSaleFrameItem"..i );
		stallbtn:SetClientUserData( 0, DRAG_STALL );
		stallbtn:SetClientUserData( 1, i - 1 );
		local ItemTex			= getglobal( stallbtn:GetName().."IconTexture" );
		local CountFont			= getglobal( stallbtn:GetName().."CountFont" );
		local UVAnimationTex	= getglobal( stallbtn:GetName().."UVAnimationTex" );
		local nItemId			= item:getItemId();
		if nItemId ~= 0 then
			local itemdef	= item:getItemDef();
			local IconPath	= GetItemEquipIconPath()..itemdef.IconID..".tga";
			-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
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
			-- checkѡ���İ�ť
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
		local item		= allSelfItem.m_ItemData[nSelectItemIndex-1];
		if item:getItemId() == 0 then
			t_StallSaleFrameControl:clearSelectItem();
		end
	end
	
	-- ���¼۸��
	UpdateStallSaleFrameSelectItemPrice();
	-- ���������
	UpdateStallSaleFrameAllIncome();
end

-- ˢ�����Ұ�̯���ĳ��������
function Private_UpdateStallSalePetFrame()
	DisCheckAllBtn( "StallSaleFramePet", CLIENT_STALL_MAX_PET );
	local allSelfItem		= GameStallManager:getSelfStallItem();
	local nCurLine			= 1;
	local t_selPetInfo		= t_StallSaleFrameControl["pet"];
	local nSelectPetIndex	= t_selPetInfo["selectIndex"];
	for i = 1, CLIENT_STALL_MAX_PET do
		local oneSelfPetData	= allSelfItem.m_PetData[i-1];
		local petInfo			= oneSelfPetData.m_RolePet;
		if petInfo.PetID ~= 0 then
			local petBtn		= getglobal( "StallSaleFramePet"..nCurLine );
			local szPetName		= petInfo.Name;
			local PetDefInfo	= getPetDef( petInfo.PetID );
			if szPetName == "" then
				szPetName = PetDefInfo.Name;
			end
			-- color = GetPetGeniusColor( petInfo )
			local t_fonts = {	[petBtn:GetName().."Name"]				= { value = szPetName }, 
								[petBtn:GetName().."MoneyCountFont"]	= {	value = oneSelfPetData.m_uPrice, },
								[petBtn:GetName().."Level"]				= {	value = petInfo.Level.."��", },
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
			
			local uv = t_petLoveLevelTextures[petInfo.LoveLevel].UV
			loveleveltex:SetTexUV( uv.x, uv.y, uv.width, uv.height )
			lovelevelbtn:SetClientUserData( 0,petInfo.LoveLevel )
			lovelevelbtn:Show()

			petBtn:SetClientUserData( 0, oneSelfPetData.m_nStallGridIdx );
			petBtn:SetClientUserData( 1, oneSelfPetData.m_nListType );
			nCurLine = nCurLine + 1;
		end
	end

	if nSelectPetIndex > 0  then
		local oneSelfPetData	= allSelfItem.m_PetData[nSelectPetIndex-1];
		local petInfo			= oneSelfPetData.m_RolePet;
		if petInfo.PetID == 0 then
			t_StallSaleFrameControl:clearSelectPet();
		end
	end

	for i = nCurLine, CLIENT_STALL_MAX_PET do
		local petBtn		= getglobal( "StallSaleFramePet"..i );
		
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
	-- ���¼۸��
	UpdateStallSaleFrameSelectItemPrice();
	-- ���������
	UpdateStallSaleFrameAllIncome();
end

-- ��ʾ���Ұ�̯���ĳ��������
function Private_ShowStallSaleFramePetUI()
	for i = 1, CLIENT_STALL_MAX_ITEM_PAGE do
		local itemframe = getglobal( "StallSaleFrameItem"..i );
		itemframe:Hide();
	end

	for i = 1, CLIENT_STALL_MAX_PET do
		local petframe = getglobal( "StallSaleFramePet"..i );
		petframe:Show();
	end

	local t_showIcons = { "StallSaleFrame_OwnPetListBtn", "StallSaleFramePetViewList" };
	util.ShowIcons( t_showIcons );
end

-- ��ѡ������Ʒ���µ���
function Private_UpdateStallSalePriceByItem()
	local nSelectIndex	= t_StallSaleFrameControl["item"]["selectIndex"];
	local priceFont		= getglobal( "StallSaleFrame_ShowPriceBtnFont" );
	if nSelectIndex <= 0 then
		priceFont:SetText( 0 );
		return;
	end

	local allSelfItem	= GameStallManager:getSelfStallItem();
	local item			= allSelfItem.m_ItemData[nSelectIndex-1];
	local nPrice		= item:getStallSellPrice();
	priceFont:SetText( nPrice );
end

-- ��ѡ���ĳ�����µ���
function Private_UpdateStallSalePriceByPet()
	local nSelectIndex	= t_StallSaleFrameControl["pet"]["selectIndex"];
	local priceFont		= getglobal( "StallSaleFrame_ShowPriceBtnFont" );
	if nSelectIndex <= 0 then
		priceFont:SetText( 0 );
		return;
	end

	local allSelfItem	= GameStallManager:getSelfStallItem();
	local stallPetData	= allSelfItem.m_PetData[nSelectIndex-1];
	priceFont:SetText( stallPetData.m_uPrice );
end


function StallSaleFrame_SaleHistoryBtn_Test_OnShow()
	local rich = getglobal( "StallSaleFrame_SaleHistoryBtnRich" );
	local szText = "�߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö����߹�·����Ҫ������������ʺ���ĺö���";
	rich:SetText( szText, 255, 255, 255 );
end

function StallSaleFrame_SaleHistoryBtn_OnShow()
	local rich = getglobal( "StallSaleFrame_SaleHistoryBtnRich" );
	SetScrollBar( "StallSaleFrame_SaleHistoryBtn_ScrollBar", rich:GetViewLines(), rich:GetTextLines() );
end

function StallSaleFrame_StallNameBtn_Test_OnClick()
	this:Hide();
	local font = getglobal( this:GetName().."Font" );
	StallSaleFrame_StallNameEdit:Show();
	StallSaleFrame_StallNameEdit:SetText( font:GetText() );
	SetFocusFrame( "StallSaleFrame_StallNameEdit" );
	StallSaleFrame_StallNameEdit:SelectAllText();
end

function StallSaleFrame_StallAdvertiseBtn_Test_OnClick()
	local font = getglobal( this:GetName().."Font" );
	this:Hide();
	local edit = getglobal( "StallSaleFrame_StallAdvertiseEdit" );
	edit:Show();
	edit:SetText( font:GetText() );
	SetFocusFrame( "StallSaleFrame_StallAdvertiseEdit" );
	edit:SelectAllText();
end

--@Desc:����ɹ���ˢ��̯λ��Ϣͬʱ���潻�׼�¼
function CallScript_UpdateStallInfo( nStallType, nGridIdx, nChgItemNum, szRoleName, saledItemIdx )
	local StallData		= GameStallManager:getSelfStallItem();
	local szRecord;
	local nCurTime	= os.date("%c", os.time());
	local t_StallSaleFrameControl = GetStallSaleFrameControl();
	if nStallType == CLIENT_STALL_TYPE_ITEM then
		local item			= StallData.m_ItemData[nGridIdx];
		local itemdef		= item:getItemDef();
		local stallprice	= item:getStallSellPrice();
		local nLeaveNum		= StallData.m_ItemData[nGridIdx]:getNum() - nChgItemNum;
		if itemdef == nil then
			return;
		end
		
		szRecord = nCurTime.."\n#L#cfffebd" .. szRoleName .. "@@player#n#c938259��"
					.. stallprice .. "��"..MONEY_TYPE_MONEY_NAME.."\n������#n#L#c" .. getItemColor(itemdef.Important,itemdef.SuitSetID) .. item:getDisplayName() .. "@@" .. saledItemIdx .. "#n*" .. nChgItemNum .. "��\n\n";
		t_StallSaleFrameControl:addTotalIncome( { ["incrIncome"] = ( stallprice * nChgItemNum ) } );
		-- ���½���������Ʒ����		
		local nBtnIndex = nGridIdx + 1;
		local CountFont	= getglobal( "StallSaleFrameItem".. (nGridIdx+1) .."CountFont" );
		CountFont:SetText( nLeaveNum );

		-- ��ĳ����Ʒ�������ˣ����䱳�������Ӧ����λ����
		if item:getNum() - nChgItemNum == 0 then
			UnLockItem( item:getListType(), item:getGridIdx(), item:getItemId() );	-- ����
		end

	elseif nStallType == CLIENT_STALL_TYPE_PET then
		local mainplayer	= ActorMgr:getMainPlayer();
		local GamePet		= mainplayer:getPet();
		local PetInfo		= StallData.m_PetData[nGridIdx].m_RolePet;
		local PetDefInfo	= getPetDef( PetInfo.PetID );
		local stallprice	= StallData.m_PetData[nGridIdx].m_uPrice;
		local szPetName		= PetInfo.Name;

		if szPetName == "" then
			szPetName = PetDefInfo.Name;
		end

		szRecord = nCurTime.."\n#L#cfffebd" .. szRoleName .. "@@player#n	#c938259��"
					.. stallprice .. "��"..MONEY_TYPE_MONEY_NAME.."\n������#n#cffffff" .. szPetName .. "#n\n\n";
		t_StallSaleFrameControl:addTotalIncome( { ["incrIncome"] = ( stallprice * nChgItemNum ) } );
	end
	
	local rich = getglobal( "StallSaleFrame_SaleHistoryBtnRich" );
	rich:AddText( szRecord, 147, 130, 89 );

	local slider = getglobal( "StallSaleFrame_SaleHistoryBtn_ScrollBar" );
	SetScrollBar( slider:GetName(), rich:GetViewLines(), rich:GetTextLines() );
	slider:SetValue( slider:GetMaxValue() );
	util.UpdateRich( rich:GetName(), slider:GetValue() );
	StallSaleFrame_AllIncomeBtnFont:SetText( t_StallSaleFrameControl:getTotalIncome() );
end

-- ����Ƶ��۸�ť
function StallSaleFrame_ShowPriceBtn_OnEnter()
	local szText = "��Ʒ����";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

-- ����Ƶ����밴ť
function StallSaleFrame_AllIncomeBtn_OnEnter()
	local szText = "������Ʒ������";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

-- ���ڰ�̯״̬��ʱ��Ҫ�ı����ǵ�̯λ������ʾ
function StallSaleFrame_EndStallBtn_OnShow()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	StallSaleFrame_StallNameBtnFont:SetText( mainplayer:getStallName() );
end

function StallSaleFrame_SaleHistoryBtnRich_OnClick()
	if arg3 == "LeftButton" and arg2 == "player" then
		SendPrivateMessage( arg1 );
	elseif arg3 == "LeftButton" and string.find( arg2, "item", 1, true ) then
		local itemId = string.gsub( arg2, "item", "" );
		local itemDef = getItemDef( itemId );
		ShowItemTips(LUA_ITEMTIPS_TYPE_LINK, LUA_ITEMTIPS_ITEMDEF, itemDef );
	elseif arg3 == "LeftButton" and tonumber( arg2 ) ~= nil then
		local item = GameStallManager:getSaledItem(tonumber( arg2 ));
		if item and item:getItemId() > 0 then
			ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item );
		end		
	end
end

function StallSaleFrame_SaleHistoryBtnRich_OnLeave()
	NormalItemTipsFrame:Hide();
	ArmItemTipsFrame:Hide();
	SecondArmItemTipsFrame:Hide();
end

function Stall_ScrollUpBtn_OnClick()
	local slider = getglobal( "StallSaleFrame_SaleHistoryBtn_ScrollBar" );
	if not slider:IsShown() then
		return;
	end

	local nMaxValue		= slider:GetMaxValue();
	local nValue		= slider:GetValue() - slider:GetValueStep();

	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end

function Stall_ScrollDownBtn_OnClick()
	local slider = getglobal( "StallSaleFrame_SaleHistoryBtn_ScrollBar" );
	if not slider:IsShown() then
		return;
	end

	local nMaxValue		= slider:GetMaxValue();
	local nValue		= slider:GetValue() + slider:GetValueStep();

	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

function Stall_ScrollBar_OnValueChanged()
	local slider		= getglobal( "StallSaleFrame_SaleHistoryBtn_ScrollBar" );
	local nValue		= slider:GetValue();
	local nMaxValue		= slider:GetMaxValue();

	if nValue <= nMaxValue then
		util.UpdateRich( "StallSaleFrame_SaleHistoryBtnRich", nValue );
	end
end
-------------------------------------------------------------------------��̯������---------------------------------------------------------------------------------------------------
function StallBuyFrame_Test_OnShow()
	local rich = getglobal( this:GetName().."_StallOwnerNameRich" );
	local szText = "#L̯������̯������̯������̯������̯������#n";
	rich:SetText( szText, 255, 255, 255 );
end

-- ˫�����۳����б�����Ӧ
function Private_StallSaleFramePetItem_OnDoubleClic( petBtn )
	local nPetStallGrid		= petBtn:GetClientUserData( 0 ) + 1;
	local selfStallInfo		= GameStallManager:getSelfStallItem();
	local oneSelfPetData	= selfStallInfo.m_PetData[nPetStallGrid-1];
	local petInfo			= oneSelfPetData.m_RolePet;
	local petWID			= petInfo.WID;
	ShowSelectPet( petInfo, "StallSaleFrame" );

	t_StallSaleFrameControl:setSelectPet( { selectIndex = nPetStallGrid, heightId = petWID.nHeight, lowId = petWID.nLow } );

	DisCheckAllBtn( "StallSaleFramePet", CLIENT_STALL_MAX_PET );
	petBtn:Checked();
	UpdateStallSaleFrameSelectItemPrice();
end

function StallPetItem_OnDoubleClick()
	if this:GetParent() == "StallSaleFrame" then
		Private_StallSaleFramePetItem_OnDoubleClic( this );
	elseif this:GetParent() == "StallBuyFrame" then
		Private_StallBuyerBuyItemFramePetItem_OnDoubleClick( this );
	end
end

function StallPetItem_Test_OnClick()
	this:Checked();
end

--@Desc:������۳����б�����Ӧ
-- ������Ұ�̯���ĳ����б�ť
function Private_StallSaleFramePetItem_OnClick( petBtn )
	local nPetStallGrid		= petBtn:GetClientUserData( 0 ) + 1;
	local selfStallInfo		= GameStallManager:getSelfStallItem();
	local oneSelfPetData	= selfStallInfo.m_PetData[nPetStallGrid-1];
	local petInfo			= oneSelfPetData.m_RolePet;
	local petWID			= petInfo.WID;
	t_StallSaleFrameControl:setSelectPet( { selectIndex = nPetStallGrid, heightId = petWID.nHeight, lowId = petWID.nLow } );

	DisCheckAllBtn( "StallSaleFramePet", CLIENT_STALL_MAX_PET );
	petBtn:Checked();
	UpdateStallSaleFrameSelectItemPrice();
end

function StallPetItem_OnClick()
	if this:GetParent() == "StallSaleFrame" then
		Private_StallSaleFramePetItem_OnClick( this );
	elseif this:GetParent() == "StallBuyFrame" then
		Private_StallBuyerBuyItemFramePetItem_OnClick( this );
	end
end

function SellerNameBtn_OnClick()
	-- ֻ����Ҳ��ܿ���������
	if string.find( this:GetName(), "StallBuyFrame" )== nil then
		return;
	end

	local seestallitem = GameStallManager:getSeeStallItem();

	if arg1 == "RightButton" then
		local mainplayer = ActorMgr:getMainPlayer()
		if mainplayer == nil then return end

		ShowCharacterRightFrame( seestallitem.m_szRoleName );
	elseif arg1 == "LeftButton" then
		SendPrivateMessage( seestallitem.m_szRoleName );
	end
end

-- ȷ�Ͻ�����̯
function EndStallConfirmFrame_AcceptBtn_OnClick()
	GameStallManager:stall_End();
	StallSaleFrame_SaleHistoryBtnRich:Clear();
	EndStallConfirmFrame:Hide();
end

-- ȡ��ȷ�Ͻ�����̯
function EndStallConfirmFrame_RefuseBtn_OnClick()
	EndStallConfirmFrame:Hide();
end

-- ȷ�Ͻ�����̯
local t_EndStallConfirmFrame_MutexFrames = { "StallSeller_CloseStallFrmaeConfirmFrame" };

function EndStallConfirmFrame_OnShow()
	for _, name in ipairs( t_EndStallConfirmFrame_MutexFrames ) do
		local frame = getglobal( name );
		if frame:IsShown() then
			frame:Hide();
		end
	end

	this:SetPoint( "center", "StallSaleFrame", "center", 0, 0 );
end


function EndStallConfirmFrame_OnHide()
end

function StallSaleFrameFeeBtn_OnEnter()
	SetGameTooltips( this:GetName(),"̯λ�����۳�����Ʒ����ȡ5%���" );
end

function StallPetItemLoveLevelBtn_OnEnter()
	local level = this:GetClientUserData( 0 )
	ShowGameTooltip( { rich = "WideGameTooltipRichText", tipsFrame = "WideGameTooltip", text = tLoveLevelDesc[level], frame = this:GetParent(),
						button = this:GetName() } );
	local parent = getglobal( this:GetParent() )
	parent:Highlight()
end

function StallPetItemLoveLevelBtn_OnLeave()
	local parent = getglobal( this:GetParent() )
	if not isPointInFrame( parent:GetName() ) then
		parent:DisHighlight()
	end
	WideGameTooltip:Hide()
end
