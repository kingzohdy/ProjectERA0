---------------------�ؼ��洢��Ϣ--------------------------------------
-- SelfPetInfoButton .. i nInex = 0->�洢�����ڳ�����е�����
-- TradeFrame_SeeIntelligenceBtn, TradeFrame_CallbackBtn, OppositePetInfoButton .. i --->nInex = 0->�洢�����ڳ�����е�����
-- nIndex = 2 ---> �洢����ĳ���״̬ nIndex=3 ---> ��ʾ�����Ƿ񱻷ŵ��˽�������, 0��ʾû��, 1��ʾ���ﱻ�ŵ�����������
--TradeFrame_SelfItem .. i --->nIndex = 0 ->�洢�������� nIndex = 1 -> �洢��Ʒ�ڴ洢�б��е����� -1��ʾ��Ч���� nIndex = 2 --> ��ʾ�ǶԷ�����Ʒ�����Ǽ�������Ʒ��

--@Desc: ���ڼ�¼��ǰ����������ID
local NONEPET				= -1;
local g_nCurHouseCarlIndex	= NONEPET;		-- Ĭ��û��ѡ���κ�����
local g_nCurHouseCarlHightId	= 0;		-- ��Ϊ����ID�ĸ�λֵ
local g_nCurHouseCarlLowId		= 0;		-- ��Ϊ����ID�ĵ�λֵ

--@Desc: ���ڼ�¼��ǰ���ӵ�������ID
local g_nCurFairyIndex		= NONEPET;	-- Ĭ��û��ѡ���κ�����
local g_nCurFairyHightId	= 0;		-- ��Ϊ����ID�ĸ�λֵ
local g_nCurFairyLowId		= 0;		-- ��Ϊ����ID�ĵ�λֵ

--@Desc: ��Ϊ���ڼ�¼��Ʒ����ʱ���õ����������
local MAX_ITEM_FOR_TRADE	= 8; -- ��ʾ����һ�ο��Խ��׵���Ʒ��������
--local g_nCurItemForTrade	= 0; -- ��ʾ��ǰ���н��׵���Ʒ��Ŀ

local g_nCurShowPetFrame = XIANTYPE; -- Ĭ����ʾ������Ϣ

-----------------------------���ｻ����Ŀ����--------------------------
local EXCHG_MAX_PET_NUM = 5;
-----------------------------��Ʒ������Ŀ����--------------------------
local EXCHG_MAX_ITEM_NUM = 8;

--ָ���������������������
local LIST_TYPE_XIAN = 7;
local LIST_TYPE_SHI	 = 8;

local MAX_SKILL_ICON = 10;
local MAX_ARMBUTTON  = 6;
local MAX_LISTBUTTON = 9;
--@Desc: ��������Է������н�����Ʒ ��2����Ľṹ { ID, nIndex }
local t_gPetList	= {};	-- �洢Pet��ID
local t_gItemList	= {};	-- �����洢Item��ID

--���浱ǰ���ڽ���״̬�ĳ���petInTradeNode = { nIndex } nIndexΪ�����ڳ�����е�����λ��
local t_gPetInTrade = {};
local ModelAngle	= 0;		-- ����ģ����ת�Ƕȱ���
local g_nShowTime	= 0;

INITIATIVE_REJECT	= 0; 	-- /* �����ܾ� */
ROLE_LOGOUT_REJECT	= 1; 	-- /* ���� */
CHG_MAP_REJECT		= 2; 	-- /* �е�ͼ */
DIST_REJECT			= 3; 	-- /* ����̫Զ */
TIME_REJECT			= 4; 	-- /* ̫��ʱ��û����Ӧ */


--@Desc: ��Ϊ���ؽ�������ʱ����Ӧ
function TradeFrame_OnLoad()
	-- ע����Ʒ��ť��������Ӧ�Ļص�����
	ItemFirstCallBack( "TradeFrame", Sale2Player );

	this:RegisterEvent("GE_EXCG_INVITE");	-- ��Ϊ�յ����˽��������ʱ���յ�����Ϣ
	this:RegisterEvent("GE_EXCG_REJECT");	-- ��Ϊ�Լ������������ʱ��,�����˾ܾ���ʱ���յ�����Ϣ
	this:RegisterEvent("GE_EXCG_CANCEL");	-- ��Ϊ�Է���"ȡ��"��ť��ʱ��,���յ�����Ϣ
	this:RegisterEvent("GE_EXCG_LOCK");		-- ��Ϊ�Է���"������ť��ʱ��,���յ�����Ϣ"
	this:RegisterEvent("GE_EXCG_DEAL");		-- ��ΪA�����"����"��ť��ʱ��,��һ��B�յ�����Ϣ,B���ܿ���A���������ϵ���ϸ��Ϣ
	this:RegisterEvent("GE_EXCG_START");	-- ��Ϊһ������������,��һ����ͬ���ʱ��,˫�����յ�����Ϣ
	this:RegisterEvent("GE_EXCG_END");		-- ��Ϊ˫������"����"������һ����"ȡ��"��ʱ��,˫�����յ�����Ϣ
	this:RegisterEvent("GE_CANCEL_LOCK");
	this:RegisterEvent("GE_EXCG_BREAK");
	this:RegisterEvent("GE_ACTOR_FIGHT_IN");
	this:RegisterEvent("GE_MESSAGE_DEAD");
	this:RegisterEvent("GE_DEL_PET");
	this:RegisterEvent("GE_CONTAINER_UPDATE");
	local nGridBase = 1000;
	for i = 1, MAX_ITEM_FOR_TRADE do
		local btn = getglobal("TradeFrame_SelfItem"..i .. "ItemIconBtn" );
		btn:SetClientUserData( 0, DRAG_TRADE );	-- ָ����Ʒ��������������
		btn:SetClientUserData( 1, i - 1 );	-- ���ڴ洢��Ʒ�ڽ������ϵ�λ������
		btn:SetClientUserData( 2, 0 );		-- ���ڴ洢��Ʒ�����ڼ������ǶԷ��� 0��ʾ����,1��ʾ�Է�

		btn = getglobal("TradeFrame_OppositeItem"..i .. "ItemIconBtn" );
		btn:SetClientUserData( 0, DRAG_TRADE );
		btn:SetClientUserData( 1, i - 1  );	-- ���ڴ洢��Ʒ�ڽ������ϵ�λ������
		btn:SetClientUserData( 2, 1 );		-- ���ڴ洢��Ʒ�����ڼ������ǶԷ��� 0��ʾ����,1��ʾ�Է�
		btn:SetClientUserData( 3, 0 );		-- ���ڼ�¼�ý������Ƿ�����Ʒ�������� 0��ʾû����Ʒ���ڸý�������, 1��ʾ����Ʒ�Ž�������
	end

	for i = 1, MAX_ITEM_FOR_TRADE do
		-- ��ʾ�װ�����
		local oppoItemBackTex = getglobal( "TradeFrame_OppositeItem" .. i .. "ItemIconBtnBackTexture" );
		oppoItemBackTex:Show();

		-- ��ʾ�װ�����
		local selfItemBackTex = getglobal( "TradeFrame_SelfItem" .. i .. "ItemIconBtnBackTexture" );
		selfItemBackTex:Show();
	end

	SetCheckPetInfo( "MyTotalPetFrame", sendPetForTrade );
end

--@Desc: ���׸��������
function Sale2Player( used_Item, nType, nIndex )
	local itemdef = used_Item:getItemDef();
	-- NoDealֵΪ1��ʱ�򣬱�ʾ�����������֮�佻��

	if math.floor( itemdef.ItemID / 1000 ) == ITEM_PETXIANEGG_ID and TrumpFrame:IsShown() and 
		TrumpFrame:getFrameDrawLevel() > TradeFrame:getFrameDrawLevel() then
		return true;
	end

	local nError = CanExChange( used_Item );
	if nError ~= 0 then
		ShowMidTips( GetExChangeErrorMsg( nError, "exchange" ) );
		return;
	end

	-- ��������Ʒ�Ѵ��ڽ�������,���ܷ��뽻����
	if GameExchange:isSrcItemlistFull( ) then
		ShowMidTips( "�������Ѱ�����Ʒ����������ӽ�����Ʒ��" );
		return;
	end

	-- ���׽���򿪲��ұ������е���Ʒ�����ڽ���״̬
	 local nFirstEmpty = 0;
	 local EXCHG_MAX_ITEM_NUM = 8;
	 local item = nil;
	 for i =1, EXCHG_MAX_ITEM_NUM do
		item = GameExchange:getExchangItem( 0, i - 1 );
		if item:getItemId() == 0 then
			nFirstEmpty = i- 1 ;
			break;
		end
	end

	if GameExchange:isSrcLockExchange() then
		GameExchange:cancelLock();
	end
	
	GameExchange:addExchangeItem( nFirstEmpty, nType, nIndex, used_Item:getNum() );
	LockItem( nType, nIndex, used_Item:getItemId(), "TradeFrame" ); -- �����Ʒ��ӳɹ�����ס����Ʒ
end

-- t_invitePlayers = { { name = ?, startTime = ? } }
local t_invitePlayers = { };

function GetInvitePlayerControl()
	return t_invitePlayers;
end

t_invitePlayers["init"] = 
function ( self )
	for i = 1, table.getn( self ) do
		table.remove( self );
	end
end

t_invitePlayers["rejectByName"] = 
function ( self, destData )
	for index, data in ipairs( self ) do
		if destData["name"] == data["name"] then
			GameExchange:rejectExchange( data["name"], INITIATIVE_REJECT );
			table.remove( self, index );
			break;
		end
	end
end

t_invitePlayers["rejectAll"] = 
function ( self, destData )
	for index, data in ipairs( self ) do
		GameExchange:rejectExchange( data["name"], INITIATIVE_REJECT );
	end

	self:init();

	local frame = getglobal( "RequestTradeFrame" );
	if frame:IsShown() then
		frame:Hide();
	end
end

function GetExchageInvite( szInviteName )
	local frame = getglobal( "RequestTradeFrame" );
	local font	= getglobal( "RequestTradeCharacterName" );
	if  table.getn( t_invitePlayers ) == 0 then
		font:SetText( szInviteName.."�������㽻��!" );
		frame:Show();
	end
	table.insert( t_invitePlayers, { name = szInviteName, startTime = GameMgr:getTickTime() } );
end

-- ��Ϊÿһ֡������õļ����
function UpdateInviteExchange()
	if table.getn( t_invitePlayers ) == 0 then
		return;
	end
	-- ����鵽������λ�ڵ�һ��������ɾ����ͬʱҪ�ر����
	-- ���ȼ���һ��
	local frame = getglobal( "RequestTradeFrame" );
	local font	= getglobal( "RequestTradeCharacterName" );
	if ( GameMgr:getTickTime() - t_invitePlayers[1].startTime ) >= MAX_INVITE_TIME then
		-- �������б��е������˸���������2��
		if table.getn( t_invitePlayers ) > 1 then
			t_invitePlayers[2].startTime = GameMgr:getTickTime();
			font:SetText( t_invitePlayers[2].name.."�������㽻��!" );
		else
			frame:Hide();
		end
	end

	-- �����ͳһɾ�����ֶ��еĴ���
	for index, data in ipairs( t_invitePlayers ) do
		if ( GameMgr:getTickTime() - data.startTime ) >= MAX_INVITE_TIME then
			ShowMidTips( "��ܾ���"..data.name.."��������" );
			GameExchange:rejectExchange( t_invitePlayers[index].name, TIME_REJECT );
			table.remove( t_invitePlayers, index );
		end
	end
end

function RejectAllExchangeInvite( nResufeType )
	for index, data in ipairs( t_invitePlayers ) do
		GameExchange:rejectExchange( data.name, nResufeType );
	end
	-- TODO
	t_invitePlayers:init();
	RequestTradeFrame:Hide();
end

function TradeFrame_OnEvent()
	--[[
	if arg1 == "GE_EXCG_INVITE" then
		-- �ȼ�����뽻�׷��Ƿ����Լ��ĺ�����������������
		local nRelationType = GameFriendManager:getPlayerRelation( GameExchange:getExchangeName() );
		if nRelationType == CLIENT_RELATION_TYPE_XIONG_SHOU or nRelationType == CLIENT_RELATION_TYPE_BLACK  then
			return;
		end

		RequestTradeCharacterName:SetText( GameExchange:getExchangeName() .. "�������㽻��!" );
		RequestTradeFrame:Show();
		g_nShowTime = os.clock();
	end
	--]]
	-- ���ǽ�������״̬
	if arg1 == "GE_MESSAGE_DEAD" then
		if RequestTradeFrame:IsShown() then
			local t_InvitePlayerControl = GetInvitePlayerControl();
			t_InvitePlayerControl:rejectAll();
		elseif TradeFrame:IsShown() then
			GameExchange:cancelExchange( INITIATIVE_REJECT );
		end
	end
	
	if arg1 == "GE_DEL_PET" then
		GameExchange:synchronizeSelfPet();
	end
	

	if arg1 == "GE_EXCG_REJECT" then
		ShowMidTips( "�Է��ܾ����ף�" );
	end

	if arg1 == "GE_EXCG_CANCEL" then
		--ShowMidTips( "�Է�ȡ�����ף�" );
		ClearTradeFrame();
		-- ��ղ鿴���ʵİ�ť
		TradeFrame_SeeIntelligenceBtn:SetClientUserData( 0, NONEPET  );
	end

	if arg1 == "GE_EXCG_START" then

		local btn;
		for i = 1, MAX_ITEM_FOR_TRADE do
			btn = getglobal( "TradeFrame_OppositeItem" .. i .. "ItemDescBtnHighlightBtn" );
			--btn:Disable();

			btn = getglobal( "TradeFrame_SelfItem" .. i .. "ItemDescBtnHighlightBtn" );
			--btn:Disable();
		end

		for i = 1, EXCHG_MAX_PET_NUM do
			btn = getglobal( "OppositePetInfoButton" .. i );
			btn:Disable();

			btn = getglobal( "SelfPetInfoButton" .. i );
			btn:Disable();
		end

		TradeFrame:Show();
	end

	if arg1 == "GE_EXCG_LOCK" then
	end

	if arg1 == "GE_EXCG_DEAL" then
		-- ����Է���ŵ����н�����Ʒ
		--�ȱ���Է����ڽ��׵�Item��ID
	end

	if arg1 == "GE_EXCG_END" then
		ShowMidTips( "���׳ɹ���" );
		ClearTradeFrame();
		TradeFrame_SeeIntelligenceBtn:SetClientUserData( 0, NONEPET  );
	end
	
	if arg1 == "GE_EXCG_BREAK" then
		ClearTradeFrame();
		TradeFrame_SeeIntelligenceBtn:SetClientUserData( 0, NONEPET  );
	end

	if arg1 == "GE_CANCEL_LOCK" then
		CancelLockTrade();
	end

	if arg1 == "GE_CONTAINER_UPDATE" then
		if TradeFrame:IsShown() then
			local mainplayer = ActorMgr:getMainPlayer();
			if mainplayer == nil then return end
			local container = mainplayer:getContainer();
			for i =1, EXCHG_MAX_ITEM_NUM do
				item = GameExchange:getExchangItem( 0, i - 1 );
				if item:getItemId() > 0 then
					local containerItem = container:getItem( item:getListType(), item:getGridIdx() )
					if containerItem:getItemId() ~= item:getItemId() then
						UnLockItem( item:getListType(), item:getGridIdx(), item:getItemId() );--����
						GameExchange:delExchangeItem( i - 1 );
						-- ���Լ�������������״̬��������������
						if GameExchange:isSrcLockExchange() then
							GameExchange:cancelLock();
						end
					end
				end
			end
		end
	end
end

function FormatString( str )
	local reverseStr = string.reverse( str );
	local formatReverseStr = "";

	for i = 1, string.len( reverseStr ) do
		if math.fmod( i, 4 ) == 0 then
			formatReverseStr = formatReverseStr .. string.sub( reverseStr, i, i );
			if i ~= string.len( reverseStr ) then
				formatReverseStr = formatReverseStr .. ',';
			end
		else
			formatReverseStr = formatReverseStr .. string.sub( reverseStr, i, i );
		end
	end

	local formatStr = string.reverse( formatReverseStr );
	return formatStr;
end

function CancelLockTrade()
	-- ���Ҵ�����������״̬,��ȡ����������
	TradeFrameSelfMaskTex:Hide();
	TradeFrameOppositeMaskTex:Hide();
	TradeFrame_TradeBtn:Disable();
	TradeFrame_LockTradeBtn:Enable();
	TradeFrame_SeeIntelligenceBtn:SetClientUserData( 0, NONEPET  );
end

function ClearTradeFrame()
	TradeFrame:Hide();
	InputMoneyFrame:Hide();
	TradeFrameSelfMaskTex:Hide();
	MyTotalPetFrame:Hide();

	RequestTradeFrame:Hide();

	local tex;
	local petNameFont;
	for i = 1, EXCHG_MAX_PET_NUM do
		tex = getglobal( "SelfPetInfoButton" .. i .. "ClickHighLightTex");
		tex:Hide();

		tex = getglobal( "OppositePetInfoButton" .. i .. "ClickHighLightTex");
		tex:Hide();

		petNameFont = getglobal( "SelfPetInfoButton" .. i .. "NameFont");
		petNameFont:SetText( "" );

		petNameFont = getglobal( "OppositePetInfoButton" .. i .. "NameFont");
		petNameFont:SetText( "" );

		petNameFont = getglobal( "SelfPetInfoButton" .. i .. "LoveLevelFont" )
		petNameFont:SetText( "" )

		petNameFont = getglobal( "OppositePetInfoButton" .. i .. "LoveLevelFont" )
		petNameFont:SetText( "" )

		tex = getglobal( "SelfPetInfoButton" .. i .. "LoveLevelTex" )
		tex:Hide()

		tex = getglobal( "OppositePetInfoButton" .. i .. "LoveLevelTex" )
		tex:Hide()

		tex = getglobal( "OppositePetInfoButton" .. i .. "LoveLevelBtn" )
		tex:Hide()
	end
end

--@Desc: ��ս��׳����б���������
function ClearTradeHightLightTex()
	local Selftex;
	local tex;
	for i = 1, EXCHG_MAX_PET_NUM do	-- ����������ԭ���ĸ�����
		Selftex = getglobal( "SelfPetInfoButton" .. i .. "ClickHighLightTex" );
		tex = getglobal( "OppositePetInfoButton" .. i .. "ClickHighLightTex" );
		Selftex:Hide();
		tex:Hide();
	end
end

--@Desc: ��Ϊ��ʾ��������ʱ����Ӧ
function TradeFrame_OnShow()
	for i = 1, MAX_ITEM_FOR_TRADE do
		local btn = getglobal("TradeFrame_OppositeItem"..i.."ItemDescBtnHighlightBtn" );
		btn:DisChecked();
	end

	for i = 1, MAX_ITEM_FOR_TRADE do
		local btn = getglobal("TradeFrame_SelfItem"..i.."ItemDescBtnHighlightBtn" );
		btn:DisChecked();
	end

	--�����������ס�Ļ���������
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	-- �������ϵ�������ʾ��Ϣ���г�ʼ��
	TradeSelfMoneyAmountFont:SetText( FormatString( tostring( GameExchange:getSrcMoney() ) ) );
	TradeSelfMoneyAmountEdit:SetText( tostring( GameExchange:getSrcMoney() ) );

	local mainplayer = ActorMgr:getMainPlayer();

	-- ���ý��׶Է�����
	TradeFrameOppositePlayerFont:SetText( GameExchange:getExchangeName() );
	-- ���ü�������
	TradeFrameSelfPlayerFont:SetText( mainplayer:getName() );
	-- ��ʾ�Լ�ӵ�еĳ�����Ϣ
	MyTotalPetFrame:Show();
	local t_hideIcons = { "MyTotalPetFramePetListTemplate_CloseBtn", "MyTotalPetFramePetListTemplate_RecommendBtn", };
	util.HideIcons( t_hideIcons );

	ClearTradeHightLightTex();
	-- ���ý��׶���Tiltle
	TradeFrame:SetClientString( GameExchange:getExchangeName() );
	TradeFrame_CallbackBtn:SetClientUserData( 0, NONEPET );
	TradeFrame_SeeIntelligenceBtn:SetClientUserData( 0, NONEPET );

	--TradeFrame_CallbackBtn:Enable();
	TradeFrame_LockTradeBtn:Enable();

	PlayerContainerFrame:SetPoint( "topleft", "TradeFrame", "topright", 0, 0 );
	PlayerContainerFrame:Show();

	TradeOppositeMoneyAmountFont:SetText( 0 );
end

--@Desc: ��Ϊ�رս�������ʱ����Ӧ
function TradeFrame_OnHide()
	-- ��ձ���������Ʒ�б�
	for i = 1, EXCHG_MAX_ITEM_NUM do
		local item = GameExchange:getExchangItem( 0, i - 1 );
		if item:getItemId() ~= 0 then
			UnLockItem( item:getListType(), item:getGridIdx(), item:getItemId() )
		end
	end
	bOpenContainerFlag = true;
	MyTotalPetFrame:Hide();
	TradeFrameSelfMaskTex:Hide();
	TradeFrameOppositeMaskTex:Hide();
	TradeOppositeMoneyAmountFont:SetText( "" );
	TradeOppositeMoneyAmountFont:SetText( "" );
	TradeFrameSelfPlayerFont:SetText( "" );
	TradeSelfMoneyAmountFont:SetText( "" );
	TradeFrameOppositePlayerFont:SetText( "" );

	-- ��ս����������ʾ������
	
	for i = 1, EXCHG_MAX_ITEM_NUM do
		local itemIconTex		= getglobal( "TradeFrame_SelfItem" .. i .. "ItemIconBtnIconTexture" );
		local selfItemDescBtn	= getglobal( "TradeFrame_SelfItem" .. i .. "ItemDescBtnHighlightBtn" );
		local itemDesc			= getglobal( "TradeFrame_SelfItem" .. i .. "ItemDescBtnHighlightBtnDescFont" );

		itemDesc:SetText( "" );
		--selfItemDescBtn:Disable();
		itemIconTex:Hide();

		local itemIcon			= getglobal( "TradeFrame_OppositeItem" .. i .. "ItemIconBtn" );
		local itemIconTex		= getglobal( "TradeFrame_OppositeItem" .. i .. "ItemIconBtnIconTexture" );
		local itemDesc			= getglobal( "TradeFrame_OppositeItem" .. i .. "ItemDescBtnHighlightBtnDescFont" );
		local oppoItemDescBtn	= getglobal( "TradeFrame_OppositeItem" .. i .. "ItemDescBtnHighlightBtn" );
		itemIconTex:Hide( );
		itemDesc:SetText( "" );
		--itemIcon:Disable();
		--oppoItemDescBtn:Disable();
	end

	for i = 1, EXCHG_MAX_PET_NUM do
		local petButton = getglobal( "SelfPetInfoButton" .. i );
		local petName	= getglobal( "SelfPetInfoButton" .. i .. "NameFont" );
		local clickTex	= getglobal( "SelfPetInfoButton" .. i .. "ClickHighLightTex" );
		local enterTex	= getglobal( "SelfPetInfoButton" .. i .. "EnterHighLightTex" );
		local lovelevelfont = getglobal( "SelfPetInfoButton" .. i .. "LoveLevelFont" )
		local loveleveltex = getglobal( "SelfPetInfoButton" .. i .. "LoveLevelTex" )
		local lovelevelbtn = getglobal( "SelfPetInfoButton" .. i .. "LoveLevelBtn" )
		petButton:SetClientUserData( 0, NONEPET );
		petButton:Disable();
		petName:SetText( "" );
		lovelevelfont:SetText( "" )
		clickTex:Hide();
		enterTex:Hide();
		loveleveltex:Hide()
		lovelevelbtn:Hide()

		local petButton = getglobal( "OppositePetInfoButton" .. i );
		local petName	= getglobal( "OppositePetInfoButton" .. i .. "NameFont" );
		local clickTex	=  getglobal( "OppositePetInfoButton" .. i .. "ClickHighLightTex" );
		local enterTex	=  getglobal( "OppositePetInfoButton" .. i .. "EnterHighLightTex" );
		lovelevelfont	= getglobal( "OppositePetInfoButton" .. i .. "LoveLevelFont" )
		loveleveltex	= getglobal( "OppositePetInfoButton" .. i .. "LoveLevelTex" )
		lovelevelbtn = getglobal( "OppositePetInfoButton" .. i .. "LoveLevelBtn" )

		petButton:SetClientUserData( 0, NONEPET );
		petName:SetText( "" );
		lovelevelfont:SetText( "" )
		petButton:Disable();
		clickTex:Hide();
		enterTex:Hide();
		loveleveltex:Hide()
		lovelevelbtn:Hide()
	end
end

-- @Desc: ��Ϊ���ϸ��½���
function TradeFrame_OnUpdate( )
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then
		return;
	end
	local GamePet	= mainplayer:getPet();
	-- ����ˢ�½�����������ߵ���Ϣ
	
	-- ��ˢ����Ʒ����Ϣ

	for i = 1, EXCHG_MAX_ITEM_NUM do
		local itemIcon			= getglobal( "TradeFrame_SelfItem" .. i .. "ItemIconBtn" );
		local itemIconTex		= getglobal( "TradeFrame_SelfItem" .. i .. "ItemIconBtnIconTexture" );
		local selfItemDescBtn	= getglobal( "TradeFrame_SelfItem" .. i .. "ItemDescBtnHighlightBtn" );
		local itemDesc			= getglobal( "TradeFrame_SelfItem" .. i .. "ItemDescBtnHighlightBtnDescFont" );

		local item				= GameExchange:getExchangItem( 0, i - 1 );
		local itemDef			= item:getItemDef();
		-- �ж��Ƿ�����ƷҪ��ʾ,��û��,����ͼ�겻��ʾ
		if	item:getItemId() == 0 then
			itemDesc:SetText( "" );
			--selfItemDescBtn:Disable();
			itemIconTex:Hide();
		else
			local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
			itemIconTex:SetTexture( IsInExistence( IconPath ) and IconPath or DEFAULT_ITEM_PATH );

			itemIconTex:Show();
			local szItemName = getItemDef( item:getItemId() ).Name;
			local t_NameColor = GetItemNameColor( getItemDef(item:getItemId()) );
			itemDesc:SetTextColor( t_NameColor[1], t_NameColor[2], t_NameColor[3] );
			if item:getNum() > 0 then
				itemDesc:SetText( szItemName.." �� ".. item:getNum() );
			else
				itemDesc:SetText( szItemName );
			end
			--selfItemDescBtn:Enable();
			--itemIcon:Enable();
		end
	end

	--��ʾ�ҵ�ǰ���׵Ľ�Ǯ��Ŀ
	TradeSelfMoneyAmountFont:SetText( FormatString( tostring( GameExchange:getSrcMoney() ) ) );

	--��ʾ�������������
	local nCurShowIcon = 1; -- ��Ϊ����ȷ��������������������м��ǿհ׵����
	for i = 1, EXCHG_MAX_PET_NUM do
		local petButton = getglobal( "SelfPetInfoButton" .. nCurShowIcon );
		local petName	= getglobal( "SelfPetInfoButton" .. nCurShowIcon .. "NameFont" );
		local lovelevelfont = getglobal( "SelfPetInfoButton" .. nCurShowIcon .. "LoveLevelFont" )
		local loveleveltex = getglobal( "SelfPetInfoButton" .. nCurShowIcon .. "LoveLevelTex" )
		local lovelevelbtn = getglobal( "SelfPetInfoButton" .. nCurShowIcon .. "LoveLevelBtn" )
		local pet		= GameExchange:getExchangePetInfo( 0, i - 1 );
		local szPetName = "";


		-- ���г���,����ʾ���������
		if pet~= nil and pet.PetID ~= 0 then
			-- ���ڳ���Ҫ�ر�����,ָ������λ�ڳ�����е�λ��
			local PetDefInfo	= getPetDef( pet.PetID );
			szPetName = pet.Name;
			if szPetName == "" then
				szPetName = PetDefInfo.Name;
			end

			petButton:SetClientUserData( 0, i - 1 );
			petButton:Enable();

			petName:SetText( szPetName );
			petName:SetTextColor( 0, 250, 0 );

			lovelevelfont:SetText( GetPetLoveLevelDesc( pet.LoveLevel ) )
			local uv = t_petLoveLevelTextures[pet.LoveLevel].UV
			loveleveltex:SetTexUV( uv.x, uv.y, uv.width, uv.height )
			loveleveltex:Show()
			lovelevelbtn:SetClientUserData( 0, pet.LoveLevel )
			lovelevelbtn:Show()

			nCurShowIcon = nCurShowIcon + 1;
		end
	end
	--Ȼ������û��ʾ�������ֵĿؼ�
	for i = nCurShowIcon, EXCHG_MAX_PET_NUM do
		local petButton = getglobal( "SelfPetInfoButton" .. i );
		local petName	= getglobal( "SelfPetInfoButton" .. i .. "NameFont" );
		local clickTex	= getglobal( "SelfPetInfoButton" .. i .. "ClickHighLightTex" );
		local enterTex	= getglobal( "SelfPetInfoButton" .. i .. "EnterHighLightTex" );
		local lovelevelfont = getglobal( "SelfPetInfoButton" .. i .. "LoveLevelFont" )
		local loveleveltex = getglobal( "SelfPetInfoButton" .. i .. "LoveLevelTex" )
		local lovelevelbtn = getglobal( "SelfPetInfoButton" .. i .. "LoveLevelBtn" )
		petButton:SetClientUserData( 0, NONEPET );
		petButton:Disable();
		petName:SetText( "" );
		lovelevelfont:SetText( "" )
		clickTex:Hide();
		enterTex:Hide();
		loveleveltex:Hide()
		lovelevelbtn:Hide()
	end

	if GameExchange:isSrcLockExchange() then
		TradeFrameSelfMaskTex:Show();
		TradeFrame_LockTradeBtn:Disable();
	else
		TradeFrameSelfMaskTex:Hide();
		TradeFrame_LockTradeBtn:Enable();
	end

	--���Է��Ƿ���������,��û����,����ʾ�Է��Ľ�����Ϣ
	if not GameExchange:isDesLockExchange() then
		TradeFrameOppositeMaskTex:Hide();
		TradeOppositeMoneyAmountFont:Hide();
		for i = 1, EXCHG_MAX_ITEM_NUM do
			local itemIcon		= getglobal( "TradeFrame_OppositeItem" .. i .. "ItemIconBtn" );
			local itemIconTex	= getglobal( "TradeFrame_OppositeItem" .. i .. "ItemIconBtnIconTexture" );
			local itemDesc		= getglobal( "TradeFrame_OppositeItem" .. i .. "ItemDescBtnHighlightBtnDescFont" );
			local oppoItemDescBtn	= getglobal( "TradeFrame_OppositeItem" .. i .. "ItemDescBtnHighlightBtn" );
			itemIconTex:Hide( );
			itemDesc:SetText( "" );
			--itemIcon:Disable();
			--oppoItemDescBtn:Disable();
		end

		for i = 1, EXCHG_MAX_PET_NUM do
			local petButton = getglobal( "OppositePetInfoButton" .. i );
			local petName	= getglobal( "OppositePetInfoButton" .. i .. "NameFont" );
			local clickTex	=  getglobal( "OppositePetInfoButton" .. i .. "ClickHighLightTex" );
			local enterTex	=  getglobal( "OppositePetInfoButton" .. i .. "EnterHighLightTex" );
			local lovelevelfont = getglobal( "OppositePetInfoButton" .. i .. "LoveLevelFont" )
			local loveleveltex = getglobal( "OppositePetInfoButton" .. i .. "LoveLevelTex" )
			local lovelevelbtn = getglobal( "OppositePetInfoButton" .. i .. "LoveLevelBtn" )
			petButton:SetClientUserData( 0, NONEPET );
			petName:SetText( "" );
			lovelevelfont:SetText( "" )
			petButton:Disable();
			clickTex:Hide();
			enterTex:Hide();
			loveleveltex:Hide()
			lovelevelbtn:Hide()
		end

		-- �鿴���ʰ�ťҲ������
		TradeFrame_SeeIntelligenceBtn:Disable();
		TradeFrame_TradeBtn:Disable(); -- ���װ�ťĬ���ǲ����õ�
		return;
	end

	-- �����׷����Ҷ��������������,����˫���İ�ť������ʹ��
	if GameExchange:isSrcLockExchange() and not TradeFrame_TradeBtn:IsEnable() and not GameExchange:isConfirmExchage() then
		TradeFrame_TradeBtn:Enable();
		TradeFrame_TradeBtn:DisHighlight();				
	end

	if ( not GameExchange:isSrcLockExchange() and TradeFrame_TradeBtn:IsEnable() ) or GameExchange:isConfirmExchage() then	
		TradeFrame_TradeBtn:Disable(); -- ���װ�ťĬ���ǲ����õ�
	end

	TradeFrameOppositeMaskTex:Show();
	--���Է���������,����ʾ�Է��Ľ�����Ϣ
	--��ˢ�¶Է���Ʒ����Ϣ
	for i = 1, EXCHG_MAX_ITEM_NUM do
		local itemIcon			= getglobal( "TradeFrame_OppositeItem" .. i .. "ItemIconBtn" );
		local itemIconTex		= getglobal( "TradeFrame_OppositeItem" .. i .. "ItemIconBtnIconTexture" );
		local oppoItemDescBtn	= getglobal( "TradeFrame_OppositeItem" .. i .. "ItemDescBtnHighlightBtn" );
		local itemDesc			= getglobal( "TradeFrame_OppositeItem" .. i .. "ItemDescBtnHighlightBtnDescFont" );

		local item				= GameExchange:getExchangItem( 1, i - 1 );
		local itemDef			= item:getItemDef();
		-- �ж��Ƿ�����ƷҪ��ʾ,��û��,����ͼ�겻��ʾ
		if	item:getItemId() == 0 then
			itemDesc:SetText( "" );
			--itemIcon:Disable();
			--oppoItemDescBtn:Disable();
			itemIconTex:Hide();
		else
			
			local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
			-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			itemIconTex:SetTexture( IconPath );

			itemIconTex:Show();
			local szItemName = getItemDef( item:getItemId() ).Name;
			local t_NameColor = GetItemNameColor( getItemDef(item:getItemId()) );
			itemDesc:SetTextColor( t_NameColor[1], t_NameColor[2], t_NameColor[3] );
			if item:getNum() > 0 then
				itemDesc:SetText( szItemName .. " �� " .. item:getNum() );
			else
				itemDesc:SetText( szItemName );
			end
			--oppoItemDescBtn:Enable();
			--itemIcon:Enable();
			--itemIcon:SetClientUserData( 1, i - 1 );
		end
		itemIcon:SetClientUserData( 1, i - 1 );
	end
	local actor	= ActorMgr:findActorByName( GameExchange:getExchangeName() );
	if actor == nil then return end

	GamePet	= actor:getPet();
	--��ʾ��ǰ���׷��Ľ�Ǯ��Ŀ
	TradeOppositeMoneyAmountFont:SetText( FormatString( tostring( GameExchange:getDesMoney() ) ) );
	TradeOppositeMoneyAmountFont:Show();

	--��ʾ�Է����������
	nCurShowIcon = 1; -- ��Ϊ����ȷ��������������������м��ǿհ׵����
	for i = 1, EXCHG_MAX_PET_NUM do
		local petButton = getglobal( "OppositePetInfoButton" .. nCurShowIcon );
		local petName	= getglobal( "OppositePetInfoButton" .. nCurShowIcon .. "NameFont" );
		local lovelevelfont = getglobal( "OppositePetInfoButton" .. nCurShowIcon .. "LoveLevelFont" )
		local loveleveltex = getglobal( "OppositePetInfoButton" .. nCurShowIcon .. "LoveLevelTex" )
		local lovelevelbtn = getglobal( "OppositePetInfoButton" .. nCurShowIcon .. "LoveLevelBtn" )
		local pet		= GameExchange:getExchangePetInfo( 1, i - 1 );
		local szPetName = "";

		-- ���г���,����ʾ���������
		if pet.PetID ~= 0 then
			local PetDefInfo = getPetDef( pet.PetID );
			szPetName = pet.Name;

			if szPetName == "" then
				szPetName = PetDefInfo.Name;
			end

			-- ���ý���������еĳ����б���Ϣ
			petButton:SetClientUserData( 0, i - 1 );
			petButton:Enable();

			-- ���ڳ���Ҫ�ر�����,ָ������λ�ڳ�����е�λ��
			petName:SetText( szPetName );

			local MAX_LOVE_LEVEL = 2
			local level = pet.LoveLevel + 1
			if level > MAX_LOVE_LEVEL then
				level = MAX_LOVE_LEVEL
			end
			lovelevelfont:SetText( GetPetLoveLevelDesc( level ) )
			local uv = t_petLoveLevelTextures[level].UV
			loveleveltex:SetTexUV( uv.x, uv.y, uv.width, uv.height )
			loveleveltex:Show()
			lovelevelbtn:SetClientUserData( 0, level )
			lovelevelbtn:Show()

			-- ����������彻���б�
			nCurShowIcon = nCurShowIcon + 1;
		end
	end

	--Ȼ������û��ʾ�������ֵĿؼ�
	for i = nCurShowIcon, EXCHG_MAX_PET_NUM do
		local petButton = getglobal( "OppositePetInfoButton" .. i );
		local petName	= getglobal( "OppositePetInfoButton" .. i .. "NameFont" );
		local clickTex	= getglobal( "OppositePetInfoButton" .. i .. "ClickHighLightTex" );
		local enterTex	= getglobal( "OppositePetInfoButton" .. i .. "EnterHighLightTex" );
		local lovelevelfont = getglobal( "OppositePetInfoButton" .. i .. "LoveLevelFont" )
		local loveleveltex = getglobal( "OppositePetInfoButton" .. i .. "LoveLevelTex" )
		local lovelevelbtn = getglobal( "OppositePetInfoButton" .. i .. "LoveLevelBtn" )
		petButton:SetClientUserData( 0, NONEPET );
		petButton:Disable();
		petName:SetText( "" );
		lovelevelfont:SetText( "" )
		clickTex:Hide();
		enterTex:Hide();
		loveleveltex:Hide()
		lovelevelbtn:Hide()
	end

	-- ʹ�鿴���ʵİ�ťҪ����
	TradeFrame_SeeIntelligenceBtn:Enable();
end

-- @Desc: ��Ϊ����ˢ�����������
function RequestTradeFrame_OnUpdate()
	--[[if os.clock() - g_nShowTime > 15 then
		this:Hide();
		RequestTradeCharacterName:SetText( "" );
	end
	--]]
end

--@Desc: ��Ϊ����رհ�ť��ʱ����Ӧ
function TradeFrame_Close()
	GameExchange:cancelExchange( INITIATIVE_REJECT );
	ClearTradeFrame();
end

--@Desc: ��Ϊ����Լ��Ľ�Ǯ��ť��ʱ����Ӧ
function TradeFrame_SelfMoneyBtn_OnClick()
	-- �����Ѿ���������,�����������Ǯ
	-- ���߽�Ǯ������Ѿ�����ʾ״̬,����Ӧ����
	if GameExchange:isSrcLockExchange() or InputMoneyFrame:IsShown() then
		return;
	end

	InputMoneyFrame:Show();
end

--@Desc: ��Ϊ���������ť��ʱ����Ӧ
function TradeFrame_LockTradeBtn_OnClick()
	GameExchange:lockExchange();
	InputMoneyFrame:Hide();
	TradeFrameSelfMaskTex:Show(); -- ��ʾ����������
	-- ʹ�������װ�ť������
	this:Disable();
end

--@Desc: ��Ϊ���ȡ����ť��ʱ����Ӧ
function TradeFrame_CancelBtn_OnClick()
	GameExchange:cancelExchange( INITIATIVE_REJECT );
	ClearTradeFrame();
end

--@Desc: ��Ϊ������װ�ť��ʱ����Ӧ
function TradeFrame_TradeBtn_OnClick()
	GameExchange:dealExchange();
	this:Disable();
end

--@Desc: ��Ϊ����ջذ�ť��ʱ����Ӧ
function TradeFrame_CallBackBtn_OnClick()
	if this:GetClientUserData( 0 ) == NONEPET then
		return;
	end

	-- ��ս��׳����б����
	for i = 1, EXCHG_MAX_PET_NUM do	-- ����������ԭ���ĸ�����
		local Selftex = getglobal( "SelfPetInfoButton" .. i .. "ClickHighLightTex" );
		Selftex:Hide();
	end

	-- ���Ҵ�����������״̬����ȡ����������״̬
	if GameExchange:isSrcLockExchange() then
		GameExchange:cancelLock();
	end

	local nContentIndex = this:GetClientUserData( 0 );
	local petInfo = GameExchange:getExchangePetInfo( 0, nContentIndex );
	ResetMyTotalPetFrameFont( petInfo, getPetDef( petInfo.PetID ), "MyTotalPetFrame" );
	GameExchange:delExchangePet( this:GetClientUserData( 0 ) ); -- ���ݳ����ڳ������ݱ��е�������ɾ������
	-- Ȼ��������ˢ�¼����������ϵĳ�����Ϣ
	TradeFrame_CallbackBtn:SetClientUserData( 0, NONEPET ); -- ����ջذ�ť��Ӧ�ĳ���
end

function TradeFrameSelfMoneyShowBtn_OnClick()
	SetFocusFrame( "TradeSelfMoneyAmountEdit" );
	TradeSelfMoneyAmountEdit:SelectAllText();
	-- �л���ʾ���ҵĿؼ�
	this:Hide();
	TradeSelfMoneyAmountEdit:Show();
	TradeSelfMoneyAmountEdit:SelectAllText();
end

-- @Desc: ��Ϊ�����ҿ���������������ʱ��ȷ����������ݲ�������������ϵ���������
function TradeSelfMoneyAmountEdit_OnUpdate()
	if this:GetText() == "" then
		return;
	end

	local mainplayer = ActorMgr:getMainPlayer();
	CheckEditNumberInput( this );
	CheckMaxPriceNum( this, math.min( CLIENT_MAX_MONEY, mainplayer:getMoney() ) )
end

function TradeSelfMoneyAmountEdit_OnFocusLost()
	local nMoney =  tonumber( this:GetText() );
	if nMoney == nil then
		GameExchange:addExchangeMoney( 0 );
		if GameExchange:isSrcLockExchange() then
			GameExchange:cancelLock();
		end
		return;
	end

	-- �����ǵ�ǰ������������״̬����ȡ����������
	if GameExchange:isSrcLockExchange()then
		GameExchange:cancelLock();
	end
	
	local mainplayer = ActorMgr:getMainPlayer();
	nMoney = math.min( nMoney, CLIENT_MAX_MONEY, mainplayer:getMoney() ); 
	GameExchange:addExchangeMoney( nMoney );
	-- �л����������������ʾ�Ŀؼ�
	this:Hide();
	TradeFrameSelfMoneyShowBtn:Show();
end

--@Desc: ��Ϊ����鿴���ʵİ�ť��ʱ����Ӧ
function TradeFrame_SeeIntelligenceBtn_OnClick()
	-- ��ûѡ���κγ���,���ܲ鿴����
	local SetPetBtn = getglobal( "TradeFrame_SeeIntelligenceBtn" );
	if SetPetBtn:GetClientUserData( 0 ) == NONEPET  then
		ShowMidTips( "����ѡ��Ҫ�鿴�ĳ��" );
		return;
	end
	local PetIntelligenceBtn = getglobal(  "TradeFrame_SeeIntelligenceBtn" );
	local PetInfo = GameExchange:getExchangePetInfo( 1, PetIntelligenceBtn:GetClientUserData( 0 ) );
	if PetInfo.PetID ~= 0 then
		ShowSelectPet( PetInfo, "TradeFrame" );
	end
end

--@Desc: ��Ϊ����������еĶԷ��������ְ�ť��ʱ����Ӧ
function TradeOppositePetInfoButton_OnClick()
	-- ���Է�û��������,���ܵ���Է��ĳ�������
	-- ���ߵ�ǰ�ؼ���û�洢�κγ�����Ϣ
	if not GameExchange:isDesLockExchange() or this:GetClientUserData( 0 ) == NONEPET then
		return;
	end

	TradeFrame_SeeIntelligenceBtn:SetClientUserData( 0, this:GetClientUserData( 0 ) );

	local tex;
	for i = 1,EXCHG_MAX_PET_NUM do	-- ����������ԭ���ĸ�����
		tex = getglobal( "OppositePetInfoButton" .. i .. "ClickHighLightTex" );
		tex:Hide();
	end

	tex = getglobal( this:GetName() .. "ClickHighLightTex" );
	tex:Show();
end

--@Desc: ��Ϊ�����뽻�����еĶԷ��������ְ�ť��ʱ����Ӧ
function TradeOppositePetInfoButton_OnEnter()
	if false == GameExchange:isDesLockExchange() or this:GetClientUserData( 0 ) == NONEPET then
		return;
	end

	local tex;
	for i = 1, EXCHG_MAX_PET_NUM do	-- ����������ԭ���ĸ�����
		tex = getglobal( "OppositePetInfoButton" .. i .. "EnterHighLightTex" );
		tex:Hide();
	end

	tex = getglobal( this:GetName() .. "EnterHighLightTex" );
	tex:Show();
end

--@Desc: ��Ϊ�����뽻�����еĶԷ��������ְ�ť��ʱ����Ӧ
function TradeOppositePetInfoButton_OnLeave()
	local tex = getglobal( this:GetName() .. "EnterHighLightTex" );
	tex:Hide();
end

--@Desc: ��Ϊ��������������������ְ�ť��ʱ����Ӧ
function TradeSelfPetInfoButton_OnClick()
	-- ���ؼ���û�г�����߿ؼ��ѱ���������ͨ�����������������
	TradeFrame_CallbackBtn:SetClientUserData( 0, this:GetClientUserData( 0 ) ); -- �洢�����ڳ�����е�����λ��
	local tex;
	for i = 1, EXCHG_MAX_PET_NUM do	-- ����������ԭ���ĸ�����
		tex = getglobal( "SelfPetInfoButton" .. i .. "ClickHighLightTex" );
		tex:Hide();
	end

	tex = getglobal( this:GetName() .. "ClickHighLightTex" );
	tex:Show();
end

function TradeSelfPetInfoButton_OnEnter()
	-- ���ؼ���û�г�����߿ؼ��ѱ���������ͨ�����������������
	if this:GetClientUserData( 0 ) == NONEPET or true == GameExchange:isSrcLockExchange()  then
		return;
	end

	local tex;
	for i = 1, EXCHG_MAX_PET_NUM do	-- ����������ԭ���ĸ�����
		tex = getglobal( "SelfPetInfoButton" .. i .. "EnterHighLightTex" );
		tex:Hide();
	end

	tex = getglobal( this:GetName() .. "EnterHighLightTex" );
	tex:Show();
end

function TradeSelfPetInfoButton_OnLeave()
	local tex = getglobal( this:GetName() .. "EnterHighLightTex" );
	tex:Hide();
end

function TradeFrameItemDescButton_OnClick()
	if arg1 == "LeftButton" then
		local parentButton = this:GetParent();
		local szPreName;
		if string.find( this:GetName(), "TradeFrame_OppositeItem" ) ~= nil then
			szPreName = "TradeFrame_OppositeItem";
		elseif string.find( this:GetName(), "TradeFrame_SelfItem" ) ~= nil then
			szPreName = "TradeFrame_SelfItem";
		end

		for i = 1, MAX_ITEM_FOR_TRADE do
			local btn = getglobal( szPreName .. i .. "ItemDescBtnHighlightBtn" );
			btn:DisChecked();
		end

		this:Checked();
	elseif arg1 == "RightButton" then
		local parentBtn		= getglobal( this:GetParent() );
		local szItemBtnName = parentBtn:GetParent().."ItemIconBtn";
		TradeRightBtnClick( szItemBtnName );
	end
end

function TradeFrameItemDescButton_ReceiveDrag()
	local btn			= getglobal( this:GetName() );
	local parentBtn		= getglobal( btn:GetParent() );
	local szItemBtnName = parentBtn:GetParent().."ItemIconBtn";
	if string.find( this:GetName(), "TradeFrame_OppositeItem" ) ~= nil then
		local topParentBtn = getglobal( parentBtn:GetParent() );
		szItemBtnName = "TradeFrame_SelfItem"..topParentBtn:GetClientID().."ItemIconBtn";
	end
		
	ReceiveTrade( szItemBtnName, UIMSG:getUIGV().DragFrom, UIGetDragUserData1(), UIGetDragUserData2(), UIGetDragUserData3() );
end

-------------------------���뽻�׿�---------------------------

function CheckAcceptExchange( nIndex )
	local mainplayer = ActorMgr:getMainPlayer( );
	if mainplayer == nil then return end;
	if OnlineStoreFrame:IsShown() then
		ShowMidTips( "�鿴�����У��޷����н���" );
		return false;
	end
	local actor = ActorMgr:findActorByName( t_invitePlayers[nIndex].name );
	if actor == nil then
		ShowMidTips( "Ŀ�곬�����룬����ʧ��" );
		GameExchange:rejectExchange( t_invitePlayers[nIndex].name, DIST_REJECT );
		return false;
	end

	local dist = util.CalActorsDistance( actor, mainplayer );
	if dist > 10 ^ 2 then
		ShowMidTips( "Ŀ�곬�����룬����ʧ��" );
		GameExchange:rejectExchange( t_invitePlayers[1].name, DIST_REJECT );
		return false;
	end

	return true;
end

--@Desc: ��Ϊ������뽻�׿��е�ͬ�ⰴť��ʱ����Ӧ
function RequestTradeFrame_AcceptBtn_OnClick()
	local frame = getglobal( "RequestTradeFrame" );
	if CheckAcceptExchange( 1 ) then
		GameExchange:acceptExchange( t_invitePlayers[1].name );
	end
	
	table.remove( t_invitePlayers, 1 );
	if table.getn( t_invitePlayers ) == 0 then
		frame:Hide();
		return;
	end
	
	local font	= getglobal( "RequestTradeCharacterName" );
	-- ���������������ߣ��򵯳����µ�������
	t_invitePlayers[1].startTime = GameMgr:getTickTime();
	font:SetText( t_invitePlayers[1].name.."�������㽻��!" );
end

function CheckRejectReason( nIndex )
	local mainplayer = ActorMgr:getMainPlayer( );
	local actor = ActorMgr:findActorByName( t_invitePlayers[nIndex].name );
	if actor == nil then
		--ShowMidTips( "��������Ѿ�������Ұ��Χ�ڣ�������Ч" );
		GameExchange:rejectExchange( t_invitePlayers[nIndex].name, DIST_REJECT );
		return false;
	end

	local dist = util.CalActorsDistance( actor, mainplayer );
	if dist > 10 ^ 2 then
		--ShowMidTips( "��������������̫Զ��������Ч" );
		GameExchange:rejectExchange( t_invitePlayers[1].name, DIST_REJECT );
		return false;
	end

	return true;
end

--@Desc: ��Ϊ������뽻�׿��еľܾ���ť��ʱ����Ӧ
function RequestTradeFrame_RefuseBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer()
	local frame = getglobal( "RequestTradeFrame" );

	if CheckRejectReason( 1 ) then
		GameExchange:rejectExchange( t_invitePlayers[1].name, INITIATIVE_REJECT );
	end
	
	table.remove( t_invitePlayers, 1 );
	-- �����뽻�׶����Ѿ�û�����������ߵĻ�����رս������
	if table.getn( t_invitePlayers ) == 0 then
		frame:Hide();
		return;
	end
	
	local font	= getglobal( "RequestTradeCharacterName" );
	-- ���������������ߣ��򵯳����µ�������
	t_invitePlayers[1].startTime = GameMgr:getTickTime();
	font:SetText( t_invitePlayers[1].name .. "�������㽻��!" );
end

-----------------------����ȫ��������Ϣ��---------------------

function sendPetForTrade( nPetType, nGrid, font )
	if GameExchange:isInExchangePet( SHITYPE, nGrid ) then
		ShowMidTips( "��ѡ�����Ѿ��ڽ�������" );
		return;
	end

	if GameExchange:isSrcPetlistFull() then
		ShowMidTips( "�������ϳ����Ѿ�����,������ӳ��ﵽ������" );
		return;
	end

	if GameStallManager:isInStallIPet( SHITYPE, nGrid ) then
		ShowMidTips( "�ó����Ѿ���̯λ��" );
		return;
	end

	local nFirst = -1;
	for i = 1, EXCHG_MAX_PET_NUM do
		local pet = GameExchange:getExchangePetInfo( 0, i - 1 );
		if pet == nil then
			nFirst = i - 1;
			break;
		end
	end

	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local petInfo		= petPackage:getPetInfo( nGrid );
	local petWID		= petInfo.WID;
	
	if petPackage:isPetInFreedom( petWID.nHeight, petWID.nLow ) then
		ShowMidTips( "�ó����Ѿ������������ɽ��иò�����" );
		return;
	end
	
		-- ���ҵ�ǰ���ڽ�������״̬,��ȡ������״̬
	if GameExchange:isSrcLockExchange() then
		GameExchange:cancelLock();
	end

	font:SetTextColor( 131, 128, 108 );

	GameExchange:addExchangePet( nFirst, SHITYPE, nGrid, petWID.nHeight, petWID.nLow );
end

function TradeFrameTradeFeeBtn_OnEnter()
	SetGameTooltips( this:GetName(),"�ɹ����׺�ϵͳ����ȡ2.0%�Ľ���˰��" );
end

function TradeOppositePetInfoButtonLoveLevelBtn_OnEnter()
	local level = this:GetClientUserData( 0 )
	ShowGameTooltip( { rich = "WideGameTooltipRichText", tipsFrame = "WideGameTooltip", text = tLoveLevelDesc[level], frame = this:GetParent(),
						button = this:GetName() } );
	local tex = getglobal( this:GetParent() .. "EnterHighLightTex" );
	tex:Show();
end

function TradeOppositePetInfoButtonLoveLevelBtn_OnLeave()
	local parent = getglobal( this:GetParent() )
	if not isPointInFrame( parent:GetName() ) then
		local tex = getglobal( this:GetParent() .. "EnterHighLightTex" );
		tex:Hide();	
	end
	WideGameTooltip:Hide()
end