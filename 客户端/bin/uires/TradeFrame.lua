---------------------控件存储信息--------------------------------------
-- SelfPetInfoButton .. i nInex = 0->存储宠物在宠物表中的索引
-- TradeFrame_SeeIntelligenceBtn, TradeFrame_CallbackBtn, OppositePetInfoButton .. i --->nInex = 0->存储宠物在宠物表中的索引
-- nIndex = 2 ---> 存储宠物的出征状态 nIndex=3 ---> 表示宠物是否被放到了交易栏中, 0表示没有, 1表示宠物被放到交易栏中了
--TradeFrame_SelfItem .. i --->nIndex = 0 ->存储容器类型 nIndex = 1 -> 存储物品在存储列表中的索引 -1表示无效索引 nIndex = 2 --> 表示是对方的物品栏还是己方的物品栏

--@Desc: 用于记录当前侍卫的索引ID
local NONEPET				= -1;
local g_nCurHouseCarlIndex	= NONEPET;		-- 默认没有选中任何侍卫
local g_nCurHouseCarlHightId	= 0;		-- 此为宠物ID的高位值
local g_nCurHouseCarlLowId		= 0;		-- 此为宠物ID的低位值

--@Desc: 用于记录当前仙子的索引和ID
local g_nCurFairyIndex		= NONEPET;	-- 默认没有选中任何侍卫
local g_nCurFairyHightId	= 0;		-- 此为宠物ID的高位值
local g_nCurFairyLowId		= 0;		-- 此为宠物ID的低位值

--@Desc: 此为用于记录物品交易时候用到的相关数据
local MAX_ITEM_FOR_TRADE	= 8; -- 表示可以一次可以交易的物品的最多个数
--local g_nCurItemForTrade	= 0; -- 表示当前进行交易的物品数目

local g_nCurShowPetFrame = XIANTYPE; -- 默认显示仙子信息

-----------------------------宠物交易数目上限--------------------------
local EXCHG_MAX_PET_NUM = 5;
-----------------------------物品交易数目上限--------------------------
local EXCHG_MAX_ITEM_NUM = 8;

--指定宠物和仙子所属表类型
local LIST_TYPE_XIAN = 7;
local LIST_TYPE_SHI	 = 8;

local MAX_SKILL_ICON = 10;
local MAX_ARMBUTTON  = 6;
local MAX_LISTBUTTON = 9;
--@Desc: 用来保存对方的所有交易物品 这2个表的结构 { ID, nIndex }
local t_gPetList	= {};	-- 存储Pet的ID
local t_gItemList	= {};	-- 用来存储Item的ID

--保存当前处于交易状态的宠物petInTradeNode = { nIndex } nIndex为宠物在宠物表中的索引位置
local t_gPetInTrade = {};
local ModelAngle	= 0;		-- 宠物模型旋转角度变量
local g_nShowTime	= 0;

INITIATIVE_REJECT	= 0; 	-- /* 主动拒绝 */
ROLE_LOGOUT_REJECT	= 1; 	-- /* 下线 */
CHG_MAP_REJECT		= 2; 	-- /* 切地图 */
DIST_REJECT			= 3; 	-- /* 距离太远 */
TIME_REJECT			= 4; 	-- /* 太长时间没有响应 */


--@Desc: 此为加载交易栏的时候响应
function TradeFrame_OnLoad()
	-- 注册物品按钮的优先响应的回调函数
	ItemFirstCallBack( "TradeFrame", Sale2Player );

	this:RegisterEvent("GE_EXCG_INVITE");	-- 此为收到他人交易邀请的时候收到该消息
	this:RegisterEvent("GE_EXCG_REJECT");	-- 此为自己发起交易请求的时候,被他人拒绝的时候收到该消息
	this:RegisterEvent("GE_EXCG_CANCEL");	-- 此为对方按"取消"按钮的时候,我收到的消息
	this:RegisterEvent("GE_EXCG_LOCK");		-- 此为对方点"锁定按钮的时候,我收到的消息"
	this:RegisterEvent("GE_EXCG_DEAL");		-- 此为A方点击"交易"按钮的时候,另一方B收到的消息,B就能看到A方交易栏上的详细信息
	this:RegisterEvent("GE_EXCG_START");	-- 此为一方发起交易邀请,另一方点同意的时候,双方都收到的消息
	this:RegisterEvent("GE_EXCG_END");		-- 此为双方都点"交易"或者有一方点"取消"的时候,双方都收到的消息
	this:RegisterEvent("GE_CANCEL_LOCK");
	this:RegisterEvent("GE_EXCG_BREAK");
	this:RegisterEvent("GE_ACTOR_FIGHT_IN");
	this:RegisterEvent("GE_MESSAGE_DEAD");
	this:RegisterEvent("GE_DEL_PET");
	this:RegisterEvent("GE_CONTAINER_UPDATE");
	local nGridBase = 1000;
	for i = 1, MAX_ITEM_FOR_TRADE do
		local btn = getglobal("TradeFrame_SelfItem"..i .. "ItemIconBtn" );
		btn:SetClientUserData( 0, DRAG_TRADE );	-- 指出物品所在容器的类型
		btn:SetClientUserData( 1, i - 1 );	-- 用于存储物品在交易栏上的位置索引
		btn:SetClientUserData( 2, 0 );		-- 用于存储物品是属于己方还是对方的 0表示己方,1表示对方

		btn = getglobal("TradeFrame_OppositeItem"..i .. "ItemIconBtn" );
		btn:SetClientUserData( 0, DRAG_TRADE );
		btn:SetClientUserData( 1, i - 1  );	-- 用于存储物品在交易栏上的位置索引
		btn:SetClientUserData( 2, 1 );		-- 用于存储物品是属于己方还是对方的 0表示己方,1表示对方
		btn:SetClientUserData( 3, 0 );		-- 用于记录该交易栏是否有物品放在上面 0表示没有物品放在该交易栏上, 1表示有物品放交易栏上
	end

	for i = 1, MAX_ITEM_FOR_TRADE do
		-- 显示底版纹理
		local oppoItemBackTex = getglobal( "TradeFrame_OppositeItem" .. i .. "ItemIconBtnBackTexture" );
		oppoItemBackTex:Show();

		-- 显示底版纹理
		local selfItemBackTex = getglobal( "TradeFrame_SelfItem" .. i .. "ItemIconBtnBackTexture" );
		selfItemBackTex:Show();
	end

	SetCheckPetInfo( "MyTotalPetFrame", sendPetForTrade );
end

--@Desc: 交易给其他玩家
function Sale2Player( used_Item, nType, nIndex )
	local itemdef = used_Item:getItemDef();
	-- NoDeal值为1的时候，表示不能用于玩家之间交易

	if math.floor( itemdef.ItemID / 1000 ) == ITEM_PETXIANEGG_ID and TrumpFrame:IsShown() and 
		TrumpFrame:getFrameDrawLevel() > TradeFrame:getFrameDrawLevel() then
		return true;
	end

	local nError = CanExChange( used_Item );
	if nError ~= 0 then
		ShowMidTips( GetExChangeErrorMsg( nError, "exchange" ) );
		return;
	end

	-- 若交易物品已处于交易栏上,则不能放入交易栏
	if GameExchange:isSrcItemlistFull( ) then
		ShowMidTips( "交易栏已摆满物品，不能再添加交易物品！" );
		return;
	end

	-- 交易界面打开并且背包栏中的物品不处于交易状态
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
	LockItem( nType, nIndex, used_Item:getItemId(), "TradeFrame" ); -- 如果物品添加成功则锁住该物品
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
		font:SetText( szInviteName.."申请与你交易!" );
		frame:Show();
	end
	table.insert( t_invitePlayers, { name = szInviteName, startTime = GameMgr:getTickTime() } );
end

-- 此为每一帧都会调用的检查器
function UpdateInviteExchange()
	if table.getn( t_invitePlayers ) == 0 then
		return;
	end
	-- 若检查到的名字位于第一个，则在删除的同时要关闭面板
	-- 优先检查第一个
	local frame = getglobal( "RequestTradeFrame" );
	local font	= getglobal( "RequestTradeCharacterName" );
	if ( GameMgr:getTickTime() - t_invitePlayers[1].startTime ) >= MAX_INVITE_TIME then
		-- 若名字列表中的邀请人个数不少于2个
		if table.getn( t_invitePlayers ) > 1 then
			t_invitePlayers[2].startTime = GameMgr:getTickTime();
			font:SetText( t_invitePlayers[2].name.."申请与你交易!" );
		else
			frame:Hide();
		end
	end

	-- 这进行统一删除名字队列的处理
	for index, data in ipairs( t_invitePlayers ) do
		if ( GameMgr:getTickTime() - data.startTime ) >= MAX_INVITE_TIME then
			ShowMidTips( "你拒绝了"..data.name.."交易申请" );
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
		-- 先检测申请交易方是否在自己的黑名单或凶手名单中
		local nRelationType = GameFriendManager:getPlayerRelation( GameExchange:getExchangeName() );
		if nRelationType == CLIENT_RELATION_TYPE_XIONG_SHOU or nRelationType == CLIENT_RELATION_TYPE_BLACK  then
			return;
		end

		RequestTradeCharacterName:SetText( GameExchange:getExchangeName() .. "申请与你交易!" );
		RequestTradeFrame:Show();
		g_nShowTime = os.clock();
	end
	--]]
	-- 主角进入死亡状态
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
		ShowMidTips( "对方拒绝交易！" );
	end

	if arg1 == "GE_EXCG_CANCEL" then
		--ShowMidTips( "对方取消交易！" );
		ClearTradeFrame();
		-- 清空查看素质的按钮
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
		-- 保存对方存放的所有交易物品
		--先保存对方用于交易的Item的ID
	end

	if arg1 == "GE_EXCG_END" then
		ShowMidTips( "交易成功！" );
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
						UnLockItem( item:getListType(), item:getGridIdx(), item:getItemId() );--解锁
						GameExchange:delExchangeItem( i - 1 );
						-- 若自己处于锁定交易状态，则解除锁定交易
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
	-- 若我处于锁定交易状态,则取消锁定交易
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

--@Desc: 清空交易宠物列表栏高亮条
function ClearTradeHightLightTex()
	local Selftex;
	local tex;
	for i = 1, EXCHG_MAX_PET_NUM do	-- 先隐藏所有原来的高亮条
		Selftex = getglobal( "SelfPetInfoButton" .. i .. "ClickHighLightTex" );
		tex = getglobal( "OppositePetInfoButton" .. i .. "ClickHighLightTex" );
		Selftex:Hide();
		tex:Hide();
	end
end

--@Desc: 此为显示交易栏的时候响应
function TradeFrame_OnShow()
	for i = 1, MAX_ITEM_FOR_TRADE do
		local btn = getglobal("TradeFrame_OppositeItem"..i.."ItemDescBtnHighlightBtn" );
		btn:DisChecked();
	end

	for i = 1, MAX_ITEM_FOR_TRADE do
		local btn = getglobal("TradeFrame_SelfItem"..i.."ItemDescBtnHighlightBtn" );
		btn:DisChecked();
	end

	--如果背包是锁住的话，不交易
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	-- 将界面上的银币显示信息进行初始化
	TradeSelfMoneyAmountFont:SetText( FormatString( tostring( GameExchange:getSrcMoney() ) ) );
	TradeSelfMoneyAmountEdit:SetText( tostring( GameExchange:getSrcMoney() ) );

	local mainplayer = ActorMgr:getMainPlayer();

	-- 设置交易对方名字
	TradeFrameOppositePlayerFont:SetText( GameExchange:getExchangeName() );
	-- 设置己方名字
	TradeFrameSelfPlayerFont:SetText( mainplayer:getName() );
	-- 显示自己拥有的宠物信息
	MyTotalPetFrame:Show();
	local t_hideIcons = { "MyTotalPetFramePetListTemplate_CloseBtn", "MyTotalPetFramePetListTemplate_RecommendBtn", };
	util.HideIcons( t_hideIcons );

	ClearTradeHightLightTex();
	-- 设置交易对象Tiltle
	TradeFrame:SetClientString( GameExchange:getExchangeName() );
	TradeFrame_CallbackBtn:SetClientUserData( 0, NONEPET );
	TradeFrame_SeeIntelligenceBtn:SetClientUserData( 0, NONEPET );

	--TradeFrame_CallbackBtn:Enable();
	TradeFrame_LockTradeBtn:Enable();

	PlayerContainerFrame:SetPoint( "topleft", "TradeFrame", "topright", 0, 0 );
	PlayerContainerFrame:Show();

	TradeOppositeMoneyAmountFont:SetText( 0 );
end

--@Desc: 此为关闭交易栏的时候响应
function TradeFrame_OnHide()
	-- 清空被锁定的物品列表
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

	-- 清空交易面板上显示的内容
	
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

-- @Desc: 此为不断更新界面
function TradeFrame_OnUpdate( )
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then
		return;
	end
	local GamePet	= mainplayer:getPet();
	-- 首先刷新交易栏上我这边的信息
	
	-- 先刷新物品栏信息

	for i = 1, EXCHG_MAX_ITEM_NUM do
		local itemIcon			= getglobal( "TradeFrame_SelfItem" .. i .. "ItemIconBtn" );
		local itemIconTex		= getglobal( "TradeFrame_SelfItem" .. i .. "ItemIconBtnIconTexture" );
		local selfItemDescBtn	= getglobal( "TradeFrame_SelfItem" .. i .. "ItemDescBtnHighlightBtn" );
		local itemDesc			= getglobal( "TradeFrame_SelfItem" .. i .. "ItemDescBtnHighlightBtnDescFont" );

		local item				= GameExchange:getExchangItem( 0, i - 1 );
		local itemDef			= item:getItemDef();
		-- 判断是否有物品要显示,若没有,则让图标不显示
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
				itemDesc:SetText( szItemName.." × ".. item:getNum() );
			else
				itemDesc:SetText( szItemName );
			end
			--selfItemDescBtn:Enable();
			--itemIcon:Enable();
		end
	end

	--显示我当前交易的金钱数目
	TradeSelfMoneyAmountFont:SetText( FormatString( tostring( GameExchange:getSrcMoney() ) ) );

	--显示己方宠物的名字
	local nCurShowIcon = 1; -- 此为用于确保宠物名字栏不会出现中间是空白的情况
	for i = 1, EXCHG_MAX_PET_NUM do
		local petButton = getglobal( "SelfPetInfoButton" .. nCurShowIcon );
		local petName	= getglobal( "SelfPetInfoButton" .. nCurShowIcon .. "NameFont" );
		local lovelevelfont = getglobal( "SelfPetInfoButton" .. nCurShowIcon .. "LoveLevelFont" )
		local loveleveltex = getglobal( "SelfPetInfoButton" .. nCurShowIcon .. "LoveLevelTex" )
		local lovelevelbtn = getglobal( "SelfPetInfoButton" .. nCurShowIcon .. "LoveLevelBtn" )
		local pet		= GameExchange:getExchangePetInfo( 0, i - 1 );
		local szPetName = "";


		-- 若有宠物,则显示宠物的名字
		if pet~= nil and pet.PetID ~= 0 then
			-- 对于宠物要特别设置,指明宠物位于宠物表中的位置
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
	--然后隐藏没显示宠物名字的控件
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

	--检测对方是否锁定交易,若没锁定,则不显示对方的交易信息
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

		-- 查看资质按钮也不能用
		TradeFrame_SeeIntelligenceBtn:Disable();
		TradeFrame_TradeBtn:Disable(); -- 交易按钮默认是不可用的
		return;
	end

	-- 若交易方和我都点击了锁定交易,则让双方的按钮都可以使用
	if GameExchange:isSrcLockExchange() and not TradeFrame_TradeBtn:IsEnable() and not GameExchange:isConfirmExchage() then
		TradeFrame_TradeBtn:Enable();
		TradeFrame_TradeBtn:DisHighlight();				
	end

	if ( not GameExchange:isSrcLockExchange() and TradeFrame_TradeBtn:IsEnable() ) or GameExchange:isConfirmExchage() then	
		TradeFrame_TradeBtn:Disable(); -- 交易按钮默认是不可用的
	end

	TradeFrameOppositeMaskTex:Show();
	--若对方锁定交易,则显示对方的交易信息
	--先刷新对方物品栏信息
	for i = 1, EXCHG_MAX_ITEM_NUM do
		local itemIcon			= getglobal( "TradeFrame_OppositeItem" .. i .. "ItemIconBtn" );
		local itemIconTex		= getglobal( "TradeFrame_OppositeItem" .. i .. "ItemIconBtnIconTexture" );
		local oppoItemDescBtn	= getglobal( "TradeFrame_OppositeItem" .. i .. "ItemDescBtnHighlightBtn" );
		local itemDesc			= getglobal( "TradeFrame_OppositeItem" .. i .. "ItemDescBtnHighlightBtnDescFont" );

		local item				= GameExchange:getExchangItem( 1, i - 1 );
		local itemDef			= item:getItemDef();
		-- 判断是否有物品要显示,若没有,则让图标不显示
		if	item:getItemId() == 0 then
			itemDesc:SetText( "" );
			--itemIcon:Disable();
			--oppoItemDescBtn:Disable();
			itemIconTex:Hide();
		else
			
			local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
			-- 如果没找到物品的图标则使用默认图标显示
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			itemIconTex:SetTexture( IconPath );

			itemIconTex:Show();
			local szItemName = getItemDef( item:getItemId() ).Name;
			local t_NameColor = GetItemNameColor( getItemDef(item:getItemId()) );
			itemDesc:SetTextColor( t_NameColor[1], t_NameColor[2], t_NameColor[3] );
			if item:getNum() > 0 then
				itemDesc:SetText( szItemName .. " × " .. item:getNum() );
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
	--显示当前交易方的金钱数目
	TradeOppositeMoneyAmountFont:SetText( FormatString( tostring( GameExchange:getDesMoney() ) ) );
	TradeOppositeMoneyAmountFont:Show();

	--显示对方宠物的名字
	nCurShowIcon = 1; -- 此为用于确保宠物名字栏不会出现中间是空白的情况
	for i = 1, EXCHG_MAX_PET_NUM do
		local petButton = getglobal( "OppositePetInfoButton" .. nCurShowIcon );
		local petName	= getglobal( "OppositePetInfoButton" .. nCurShowIcon .. "NameFont" );
		local lovelevelfont = getglobal( "OppositePetInfoButton" .. nCurShowIcon .. "LoveLevelFont" )
		local loveleveltex = getglobal( "OppositePetInfoButton" .. nCurShowIcon .. "LoveLevelTex" )
		local lovelevelbtn = getglobal( "OppositePetInfoButton" .. nCurShowIcon .. "LoveLevelBtn" )
		local pet		= GameExchange:getExchangePetInfo( 1, i - 1 );
		local szPetName = "";

		-- 若有宠物,则显示宠物的名字
		if pet.PetID ~= 0 then
			local PetDefInfo = getPetDef( pet.PetID );
			szPetName = pet.Name;

			if szPetName == "" then
				szPetName = PetDefInfo.Name;
			end

			-- 设置交易主面板中的宠物列表信息
			petButton:SetClientUserData( 0, i - 1 );
			petButton:Enable();

			-- 对于宠物要特别设置,指明宠物位于宠物表中的位置
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

			-- 设置属性面板交易列表
			nCurShowIcon = nCurShowIcon + 1;
		end
	end

	--然后隐藏没显示宠物名字的控件
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

	-- 使查看资质的按钮要能用
	TradeFrame_SeeIntelligenceBtn:Enable();
end

-- @Desc: 此为不断刷新请求交易面板
function RequestTradeFrame_OnUpdate()
	--[[if os.clock() - g_nShowTime > 15 then
		this:Hide();
		RequestTradeCharacterName:SetText( "" );
	end
	--]]
end

--@Desc: 此为点击关闭按钮的时候响应
function TradeFrame_Close()
	GameExchange:cancelExchange( INITIATIVE_REJECT );
	ClearTradeFrame();
end

--@Desc: 此为点击自己的金钱按钮的时候响应
function TradeFrame_SelfMoneyBtn_OnClick()
	-- 若我已经锁定交易,则不能再输入金钱
	-- 或者金钱输入框已经是显示状态,则不响应操作
	if GameExchange:isSrcLockExchange() or InputMoneyFrame:IsShown() then
		return;
	end

	InputMoneyFrame:Show();
end

--@Desc: 此为点击锁定按钮的时候响应
function TradeFrame_LockTradeBtn_OnClick()
	GameExchange:lockExchange();
	InputMoneyFrame:Hide();
	TradeFrameSelfMaskTex:Show(); -- 显示己方的遮罩
	-- 使锁定交易按钮不可用
	this:Disable();
end

--@Desc: 此为点击取消按钮的时候响应
function TradeFrame_CancelBtn_OnClick()
	GameExchange:cancelExchange( INITIATIVE_REJECT );
	ClearTradeFrame();
end

--@Desc: 此为点击交易按钮的时候响应
function TradeFrame_TradeBtn_OnClick()
	GameExchange:dealExchange();
	this:Disable();
end

--@Desc: 此为点击收回按钮的时候响应
function TradeFrame_CallBackBtn_OnClick()
	if this:GetClientUserData( 0 ) == NONEPET then
		return;
	end

	-- 清空交易宠物列表高亮
	for i = 1, EXCHG_MAX_PET_NUM do	-- 先隐藏所有原来的高亮条
		local Selftex = getglobal( "SelfPetInfoButton" .. i .. "ClickHighLightTex" );
		Selftex:Hide();
	end

	-- 若我处于锁定交易状态，则取消锁定交易状态
	if GameExchange:isSrcLockExchange() then
		GameExchange:cancelLock();
	end

	local nContentIndex = this:GetClientUserData( 0 );
	local petInfo = GameExchange:getExchangePetInfo( 0, nContentIndex );
	ResetMyTotalPetFrameFont( petInfo, getPetDef( petInfo.PetID ), "MyTotalPetFrame" );
	GameExchange:delExchangePet( this:GetClientUserData( 0 ) ); -- 根据宠物在宠物数据表中的索引来删除宠物
	-- 然后再重新刷新己方宠物栏上的宠物信息
	TradeFrame_CallbackBtn:SetClientUserData( 0, NONEPET ); -- 清空收回按钮对应的宠物
end

function TradeFrameSelfMoneyShowBtn_OnClick()
	SetFocusFrame( "TradeSelfMoneyAmountEdit" );
	TradeSelfMoneyAmountEdit:SelectAllText();
	-- 切换显示银币的控件
	this:Hide();
	TradeSelfMoneyAmountEdit:Show();
	TradeSelfMoneyAmountEdit:SelectAllText();
end

-- @Desc: 此为往银币框输入银币数量的时候确保输入的数据不会大于主角身上的银币数量
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

	-- 若主角当前处于锁定交易状态，则取消锁定交易
	if GameExchange:isSrcLockExchange()then
		GameExchange:cancelLock();
	end
	
	local mainplayer = ActorMgr:getMainPlayer();
	nMoney = math.min( nMoney, CLIENT_MAX_MONEY, mainplayer:getMoney() ); 
	GameExchange:addExchangeMoney( nMoney );
	-- 切换交易面板上银币显示的控件
	this:Hide();
	TradeFrameSelfMoneyShowBtn:Show();
end

--@Desc: 此为点击查看资质的按钮的时候响应
function TradeFrame_SeeIntelligenceBtn_OnClick()
	-- 若没选定任何宠物,则不能查看资质
	local SetPetBtn = getglobal( "TradeFrame_SeeIntelligenceBtn" );
	if SetPetBtn:GetClientUserData( 0 ) == NONEPET  then
		ShowMidTips( "请先选中要查看的宠物！" );
		return;
	end
	local PetIntelligenceBtn = getglobal(  "TradeFrame_SeeIntelligenceBtn" );
	local PetInfo = GameExchange:getExchangePetInfo( 1, PetIntelligenceBtn:GetClientUserData( 0 ) );
	if PetInfo.PetID ~= 0 then
		ShowSelectPet( PetInfo, "TradeFrame" );
	end
end

--@Desc: 此为点击交易栏中的对方宠物名字按钮的时候响应
function TradeOppositePetInfoButton_OnClick()
	-- 若对方没锁定交易,则不能点击对方的宠物名字
	-- 或者当前控件中没存储任何宠物信息
	if not GameExchange:isDesLockExchange() or this:GetClientUserData( 0 ) == NONEPET then
		return;
	end

	TradeFrame_SeeIntelligenceBtn:SetClientUserData( 0, this:GetClientUserData( 0 ) );

	local tex;
	for i = 1,EXCHG_MAX_PET_NUM do	-- 先隐藏所有原来的高亮条
		tex = getglobal( "OppositePetInfoButton" .. i .. "ClickHighLightTex" );
		tex:Hide();
	end

	tex = getglobal( this:GetName() .. "ClickHighLightTex" );
	tex:Show();
end

--@Desc: 此为鼠标进入交易栏中的对方宠物名字按钮的时候响应
function TradeOppositePetInfoButton_OnEnter()
	if false == GameExchange:isDesLockExchange() or this:GetClientUserData( 0 ) == NONEPET then
		return;
	end

	local tex;
	for i = 1, EXCHG_MAX_PET_NUM do	-- 先隐藏所有原来的高亮条
		tex = getglobal( "OppositePetInfoButton" .. i .. "EnterHighLightTex" );
		tex:Hide();
	end

	tex = getglobal( this:GetName() .. "EnterHighLightTex" );
	tex:Show();
end

--@Desc: 此为鼠标进入交易栏中的对方宠物名字按钮的时候响应
function TradeOppositePetInfoButton_OnLeave()
	local tex = getglobal( this:GetName() .. "EnterHighLightTex" );
	tex:Hide();
end

--@Desc: 此为点击交易栏己方宠物名字按钮的时候响应
function TradeSelfPetInfoButton_OnClick()
	-- 若控件中没有宠物或者控件已被锁定则不能通过点击来锁定高亮条
	TradeFrame_CallbackBtn:SetClientUserData( 0, this:GetClientUserData( 0 ) ); -- 存储宠物在宠物表中的索引位置
	local tex;
	for i = 1, EXCHG_MAX_PET_NUM do	-- 先隐藏所有原来的高亮条
		tex = getglobal( "SelfPetInfoButton" .. i .. "ClickHighLightTex" );
		tex:Hide();
	end

	tex = getglobal( this:GetName() .. "ClickHighLightTex" );
	tex:Show();
end

function TradeSelfPetInfoButton_OnEnter()
	-- 若控件中没有宠物或者控件已被锁定则不能通过点击来锁定高亮条
	if this:GetClientUserData( 0 ) == NONEPET or true == GameExchange:isSrcLockExchange()  then
		return;
	end

	local tex;
	for i = 1, EXCHG_MAX_PET_NUM do	-- 先隐藏所有原来的高亮条
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

-------------------------申请交易框---------------------------

function CheckAcceptExchange( nIndex )
	local mainplayer = ActorMgr:getMainPlayer( );
	if mainplayer == nil then return end;
	if OnlineStoreFrame:IsShown() then
		ShowMidTips( "查看店铺中，无法进行交易" );
		return false;
	end
	local actor = ActorMgr:findActorByName( t_invitePlayers[nIndex].name );
	if actor == nil then
		ShowMidTips( "目标超出距离，交易失败" );
		GameExchange:rejectExchange( t_invitePlayers[nIndex].name, DIST_REJECT );
		return false;
	end

	local dist = util.CalActorsDistance( actor, mainplayer );
	if dist > 10 ^ 2 then
		ShowMidTips( "目标超出距离，交易失败" );
		GameExchange:rejectExchange( t_invitePlayers[1].name, DIST_REJECT );
		return false;
	end

	return true;
end

--@Desc: 此为点击申请交易框中的同意按钮的时候响应
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
	-- 若还有其他邀请者，则弹出最新的邀请者
	t_invitePlayers[1].startTime = GameMgr:getTickTime();
	font:SetText( t_invitePlayers[1].name.."申请与你交易!" );
end

function CheckRejectReason( nIndex )
	local mainplayer = ActorMgr:getMainPlayer( );
	local actor = ActorMgr:findActorByName( t_invitePlayers[nIndex].name );
	if actor == nil then
		--ShowMidTips( "申请玩家已经不在视野范围内，申请无效" );
		GameExchange:rejectExchange( t_invitePlayers[nIndex].name, DIST_REJECT );
		return false;
	end

	local dist = util.CalActorsDistance( actor, mainplayer );
	if dist > 10 ^ 2 then
		--ShowMidTips( "申请玩家与你距离太远，申请无效" );
		GameExchange:rejectExchange( t_invitePlayers[1].name, DIST_REJECT );
		return false;
	end

	return true;
end

--@Desc: 此为点击申请交易框中的拒绝按钮的时候响应
function RequestTradeFrame_RefuseBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer()
	local frame = getglobal( "RequestTradeFrame" );

	if CheckRejectReason( 1 ) then
		GameExchange:rejectExchange( t_invitePlayers[1].name, INITIATIVE_REJECT );
	end
	
	table.remove( t_invitePlayers, 1 );
	-- 若申请交易队列已经没有其他申请者的话，则关闭交易面板
	if table.getn( t_invitePlayers ) == 0 then
		frame:Hide();
		return;
	end
	
	local font	= getglobal( "RequestTradeCharacterName" );
	-- 若还有其他邀请者，则弹出最新的邀请者
	t_invitePlayers[1].startTime = GameMgr:getTickTime();
	font:SetText( t_invitePlayers[1].name .. "申请与你交易!" );
end

-----------------------己方全部宠物信息栏---------------------

function sendPetForTrade( nPetType, nGrid, font )
	if GameExchange:isInExchangePet( SHITYPE, nGrid ) then
		ShowMidTips( "所选宠物已经在交易栏上" );
		return;
	end

	if GameExchange:isSrcPetlistFull() then
		ShowMidTips( "交易栏上宠物已经摆满,不能添加宠物到交易栏" );
		return;
	end

	if GameStallManager:isInStallIPet( SHITYPE, nGrid ) then
		ShowMidTips( "该宠物已经在摊位上" );
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
		ShowMidTips( "该宠物已经被您放生不可进行该操作！" );
		return;
	end
	
		-- 若我当前处于交易锁定状态,则取消锁定状态
	if GameExchange:isSrcLockExchange() then
		GameExchange:cancelLock();
	end

	font:SetTextColor( 131, 128, 108 );

	GameExchange:addExchangePet( nFirst, SHITYPE, nGrid, petWID.nHeight, petWID.nLow );
end

function TradeFrameTradeFeeBtn_OnEnter()
	SetGameTooltips( this:GetName(),"成功交易后，系统会收取2.0%的交易税金" );
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