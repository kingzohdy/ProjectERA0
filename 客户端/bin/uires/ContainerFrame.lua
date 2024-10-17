local stashCurPage			= CONTAINER_TYPE_STASH;
local curContainerType		= CONTAINER_TYPE_SUNDRIES;
local nAMMGridMax			= 0;
local nSTUFFGridMax			= 24;
nTASKGridMax				= 64;
nSUNDRIESGridMax			= 64;
nAMMGridMax					= 64;
local nStashGridMax			= 100;
local bBeginRecord			= false;
local bDown					= false;
local nLastTime				= 0;
local nMaxNum				= 10000;
local bDown_Stash			= false;
local bBeginRecord_Stash	= false;
local nLastTime_Stash		= 0;
local PACKAGE_ARRANGE_MIN	= 6;
local t_ItemCD				= {};
bHaveOpenContainer	= false;
local bHaveShowLockEffect	= false;

local nContainerPage = 1;
local nContainerMaxPage = 1;

local bSetGuidLastTime = false;
local nGuidLastTime = 0;
bOpenContainerFlag		= false;
local MAX_PACKAGE_NUM		= 6;	--最多可以扩充的背包数量

local MAX_RESOLVE_NUM		= 8;	--可分解最大数目
local MAX_TRADE_NUM		= 16;	--最大货仓数量

local t_ResolveRate	=	{
							[1]		=	"非常低",
							[10]	=	"极低",
							[20]	=	"很低",
							[50]	=	"较低",
							[80]	=	"较高",
							[100]	=	"很高",
						};

-- 获取当前背包的类型
function GetCurContainerType( szParent,szName )
	if szParent == "PlayerContainerFrame" then
		if szName ~= nil and string.find(szName,"BagItem",1,true) then
			return CONTAINER_TYPE_EXTEND;
		end
		return curContainerType;
	elseif szParent == "StashFrame" then
		if szName ~= nil and string.find(szName,"BagItem",1,true) then
			return CONTAINER_TYPE_EXTEND_STASH;
		end
		return CONTAINER_TYPE_STASH;
	end
	return 0;
end

function GetCurContainerPage()
	return nContainerPage
end

function CheckItemResult( itemDef, nResult )
	if itemDef.ItemID == 0 then
		return false;
	end

	for i = 1, MAX_ITEM_RESULT do
		local resultData = itemDef.Result[i-1];
		if resultData.ResultID == nResult then
			return true;
		end
	end

	return false;
end

function InitContainer()
	curContainerType	= CONTAINER_TYPE_SUNDRIES;
	nAMMGridMax			= 0;
	nSTUFFGridMax		= 24;
	nTASKGridMax		= 64;
	nSUNDRIESGridMax	= 64;
	nAMMGridMax			= 64;
	nStashGridMax		= 100;
	bBeginRecord		= false;
	bDown				= false;
	nLastTime			= 0;
	nMaxNum				= 10000;
	bDown_Stash			= false;
	bBeginRecord_Stash	= false;
	nLastTime_Stash		= 0;
	PACKAGE_ARRANGE_MIN	= 6;
	t_ItemCD			= {};
	ReSetContainerBtn();
	SundriesContainer:Checked();

	nContainerPage = 1;
	nContainerMaxPage = 1;
	--bShowContainerMarketFrame = false;
	bHaveOpenContainer = false;
	bHaveShowLockEffect = false;
	bSetGuidLastTime = false;
	--bOpenContainerFlag = false;
	nGuidLastTime = 0;
end

function Get_ItemCDTable()
   return t_ItemCD;
end

-- 获取当前背包的名称
function GetContainerName( nListType )
	local t_ContainerName =
	{
		{ CONTAINER_TYPE_SUNDRIES, "杂物包" },
		{ CONTAINER_TYPE_TASK, "任务包" },
		{ CONTAINER_TYPE_STUFF, "材料包" },
		{ CONTAINER_TYPE_STASH, "仓库" },
	};
	for i = 1, table.getn( t_ContainerName ) do
		if nListType == t_ContainerName[i][1] then
			return t_ContainerName[i][2];
		end
	end
	return "";
end

-- 获取杂物包的最大栏位
function GetContainerGridMax()
	return nSUNDRIESGridMax;
end

function ContainerFrameOnLoad()
	this:RegisterEvent("GE_CONTAINER_UPDATE");
	this:RegisterEvent("UI_ACTOR_ATTRIBUTE_CHG");
	this:RegisterEvent("GE_BOOTY_GET_ONE_ITEM");
	this:RegisterEvent("GE_BOOTY_GET_ALL_ITEM");
	this:RegisterEvent("GE_CONTAINER_ALL_UPDATE");
	this:RegisterEvent("GE_CONTAINER_LOCK_CHANGE");
	--背包变化
	this:RegisterEvent("GE_CONTAINER_CHG");
	--注册仓库面板优先响应的回调函数
	ItemFirstCallBack( "PlayerContainerFrame" , Save2Container );
	PlayerContainerFrameNumFont:setCenterLine( true );

	for i=1, MAX_TRADE_NUM do
		local btn = getglobal( "PlayerContainerFrameTradeItem" .. i );
		btn:SetPoint( "topleft", "PlayerContainerFrameTrade", "topleft", (i-1)%8*44, math.ceil(i/8)*44 - 14 );
		local tex = getglobal( btn:GetName() .. "BackTexture" );
		tex:Show();
	end
end

local GET_TIEM_BY_NONE		= -1;
local GET_TIEM_BY_BOOTY		= 0;
local GET_TIEM_BY_SHOP		= 1;
local GET_TIEM_BY_EXCHANGE	= 2;

local t_getItemSourceControl = { source = GET_TIEM_BY_NONE, grid = -1, listType = 0, blinkGrid = -1 };

function GetItemSourceControl()
	return t_getItemSourceControl;
end

t_getItemSourceControl["setItemSource"] = 
function ( self, data )
	self["source"] = data["source"];
end

t_getItemSourceControl["getItemSource"] = 
function ( self, data )
	return self["source"];
end

t_getItemSourceControl["setItemGrid"] = 
function ( self, data )
	self["grid"] = data["grid"];
end

t_getItemSourceControl["getItemGrid"] = 
function ( self, data )
	return self["grid"];
end

t_getItemSourceControl["setBlinkItemGrid"] = 
function ( self, data )
	self["grid"] = data["grid"];
end

t_getItemSourceControl["getBlinkItemGrid"] = 
function ( self, data )
	return self["blinkGrid"];
end

t_getItemSourceControl["showBootyXinShouGuide"] = 
function ( self )
	if self["source"] ~= GET_TIEM_BY_BOOTY then
		return false;
	end

	self["source"] = GET_TIEM_BY_NONE;

	if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_CONTAINERFRAME_BOOTY_ITEM_BTN ) then
		GameWizard:doStep( GAMEWIZARD_FIRST_BLINK_CONTAINERFRAME_BOOTY_ITEM_BTN );
		GuideEffectShow( "PlayerContainerFrame","","PlayerContainerFrameItem"..(self["grid"]+1), "恭喜您获得自动拾取道具，右键点击使用", "uires\\ZhuJieMian\\1.dds", 284, 602, 67, 67, 67, 67, -12, 12, 34, -52, false );
		self["blinkGrid"] = self["grid"]+1;
		return true;
	end
	
	self["grid"] = -1;
	return false;
end

function ContainerFrameOnEvent()
	if ( arg1 == "GE_CONTAINER_UPDATE" ) then
		-- 背包变化时更新快捷栏的药水栏位
		if TempShortCutFrame:IsShown() then
			SetTmpShortCutBtn( false );
		end
		local frame = getglobal( "PlayerContainerFrame" );
		if frame:IsShown() then
			ContainerFrameOnShow();
		end
		
		local t_ArmProce = 
		{
			"ArmProceStarFrame",
		}
		local MainPlayer	= ActorMgr:getMainPlayer();
		if MainPlayer == nil then
			return;
		end

		local container		= MainPlayer:getContainer();
		local unUsedNum		= container:getUnusedGridNum(CONTAINER_TYPE_SUNDRIES);
		local totalNum		= container:getContainerMaxNum(CONTAINER_TYPE_SUNDRIES);
		PlayerContainerFrameNumFont:SetText( "背包容量（#c0ad700" .. totalNum - unUsedNum .. "#n/" .. totalNum .. "）", 255, 255, 190 );

		for i = 1, table.getn(t_ArmProce) do
			local frame = getglobal(t_ArmProce[i]);
			if frame:IsShown() then
				local srcitem   = GameArmProce:getSrcItem();
				if srcitem:getItemId() ~= 0 then
					srcitem:updateItemData();
				end
			end
		end

		RemoveCheckItem( "PlayerContainerFrame" );
		Quest:showCollectItemInfo();
	elseif arg1 == "UI_ACTOR_ATTRIBUTE_CHG" then
		local frame = getglobal( "PlayerContainerFrame" );
		if frame:IsShown() then
			ChangeMoney();
		end
	elseif arg1 == "GE_BOOTY_GET_ONE_ITEM" or arg1 == "GE_BOOTY_GET_ALL_ITEM" then
		--t_getItemSourceControl:setItemSource( { source = GET_TIEM_BY_BOOTY } );
	elseif ( arg1 == "GE_CONTAINER_ALL_UPDATE" ) then
		if this:IsShown() then
			ShowContainerGuide();
		end
	elseif( arg1 == "GE_CONTAINER_LOCK_CHANGE" ) then
		UpdateContainerLock( "PlayerContainerFrameLockBtn" );
		UpdateContainerLock( "ClanFrameLockBtn" );
	elseif( arg1 == "GE_CONTAINER_CHG" ) then
		if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "背包绑定提示" then
			MessageBoxFrame:Hide();
		end

		local MainPlayer	= ActorMgr:getMainPlayer();
		if MainPlayer == nil then
			return;
		end

		local container		= MainPlayer:getContainer();
		local unUsedNum		= container:getUnusedGridNum(CONTAINER_TYPE_SUNDRIES);
		local totalNum		= container:getContainerMaxNum(CONTAINER_TYPE_SUNDRIES);
		PlayerContainerFrameNumFont:SetText( "背包容量（#c0ad700" .. totalNum - unUsedNum .. "#n/" .. totalNum .. "）", 255, 255, 190 );
	end
end

function ChangeMoney()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	ContainerMomeyFont:SetText( MainPlayer:getMoney() );
	ContainerBullFont:SetText( MainPlayer:getBull() );
	local dailyUsed = MainPlayer:getDailyUsedBull()
	if dailyUsed >= getLevelUP( MainPlayer:getLv() ).DailyUseBull then
		ContainerBullFont:SetTextColor( 210, 40, 50 )
	else
		ContainerBullFont:SetTextColor( 210, 185, 100 )
	end
	--ContainerGoldFont:SetText( MainPlayer:getGodCoin() );
	--PlayerContainer_BondTianYuanFont:SetText( MainPlayer:getBindGodCoin());

	StashContainerMomeyFont:SetText( MainPlayer:getStashMoney());
	StashContainerBullFont:SetText( MainPlayer:getStashBull());
end

function ShowSundriesContainer()
	curContainerType = CONTAINER_TYPE_SUNDRIES;
	PlayerContainerFrameItemBackBkgTex:SetTexture("uires\\ui\\Za\\ZaWuCaiLiaoYeMianDiBan.tga");
	nContainerPage = 1;	
	ReSetContainerBtn();
	this:Checked();
	ContainerFrameOnShow();
	ShowContainerGuide();
	
end

function ShowStuffContainer()
	curContainerType = CONTAINER_TYPE_STUFF;
	PlayerContainerFrameItemBackBkgTex:SetTexture("uires\\ui\\Za\\ZaWuCaiLiaoYeMianDiBan.tga");
	nContainerPage = 1	
	ReSetContainerBtn();
	this:Checked();
	ContainerFrameOnShow();
	ShowContainerGuide();	
end

function ShowTaskContainer()
	curContainerType = CONTAINER_TYPE_TASK;
	PlayerContainerFrameItemBackBkgTex:SetTexture("uires\\ui\\Za\\RenWuYeMianDiTu.tga");
	nContainerPage = 1	
	ReSetContainerBtn();
	TaskContainer:Checked();
	ContainerFrameOnShow();
	ShowContainerGuide();	
end

function ReSetContainerBtn()
	TaskContainer:DisChecked();
	StuffContainer:DisChecked();
	SundriesContainer:DisChecked();
end

function SetItemButtonCheck( szParentFrame, nListType )
	for i=1 ,table.getn(t_ItemButtonCheckIndex) do
		if t_ItemButtonCheckIndex[i][1] == szParentFrame and nListType == curContainerType then
			local button = getglobal( szParentFrame.."Item"..(t_ItemButtonCheckIndex[i][2]+1) );
			button:Checked();
		end
	end
end

function SetItemCDDuration( nItemId, nDuration )
	for i = 1, table.getn( t_ItemCD ) do
		if nItemId == t_ItemCD[i][1] then
			t_ItemCD[i][2] = nDuration;
			return;
		end
	end
	table.insert( t_ItemCD, { nItemId, nDuration } );
end

function SetTextSize( szName, nFontHight )
	local font				= getglobal( szName );
	local fonttext			= font:GetText();
	local fonttextextent	= font:GetTextExtentWidth( fonttext );
	font:SetSize( fonttextextent + 10, nFontHight );
end

local t_containerControl = {};

function GetContainerControl()
	return t_containerControl;
end

t_containerControl["isHavePetSkillLearnItem"] = 
function ( self )
	local mainplayer = ActorMgr:getMainPlayer();
	local container = mainplayer:getContainer();
	local isPetSkillLearnItem = 
	function ( data )
		local itemDef				= data["itemDef"];
		local t_petSkillLearnConfig	= data["config"];
		for _, resultId in ipairs( t_petSkillLearnConfig.itemPre.resultId ) do
			if resultId == itemDef.Result[0].ResultID then
				return true;
			end
		end

		return false;
	end
	
	-- TODO: 策划给个任务ID, 第一次学习宠物技能 and Quest:isQuestRun( 3134 )
	local t_petSkillLearnConfig = GetPetItemFramesConfig()["Pet_SkillLearnFrame"];
	for i = 1, container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES ) do
		local Item = container:getItem( CONTAINER_TYPE_SUNDRIES, (i-1) );
		if Item:getItemId() ~= 0  and isPetSkillLearnItem( { itemDef = Item:getItemDef(), config = t_petSkillLearnConfig } ) then 
			return true;
		end
	end

	return false;
end

t_containerControl["isHavePetDevelopmentBuildUpItem"] = 
function ( self )
	local mainplayer = ActorMgr:getMainPlayer();
	local container = mainplayer:getContainer();

	local isPetDevelopmentBuildUpItem = 
	function ( data )
		local itemDef						= data["itemDef"];
		local t_petDevelopmentBuildUpConfig	= data["config"];
		for _, resultId in ipairs( t_petDevelopmentBuildUpConfig.itemPre.resultId ) do
			if resultId == itemDef.Result[0].ResultID then
				return true;
			end
		end

		return false;
	end

	-- TODO: 策划给个任务ID, 第一次强化宠物 and Quest:isQuestRun( 3134 )
	local t_petDevelopmentBuildUpConfig = GetPetItemFramesConfig()["Pet_DevelopmentBuildUpFrame"];
	for i = 1, container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES ) do
		local Item = container:getItem( CONTAINER_TYPE_SUNDRIES, (i-1) );
		if Item:getItemId() ~= 0  and isPetDevelopmentBuildUpItem( { itemDef = Item:getItemDef(), config = t_petDevelopmentBuildUpConfig } ) then 
			return true;
		end
	end

	return false;
end

t_containerControl["isHaveArmUpItem"] = 
function ( self )
	local mainplayer = ActorMgr:getMainPlayer();
	local container = mainplayer:getContainer();

	
	for i = 1, container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES ) do
		local Item = container:getItem( CONTAINER_TYPE_SUNDRIES, (i-1) );
		if Item:getItemId() ~= 0 then 
			local gemNewDef = GameArmProce:getGemNewDef( Item:getItemId() ); 
			if gemNewDef ~= nil then
				return true;
			end
		end
	end

	return false;
end

t_containerControl["isHaveGemInLayItem"] = 
function ( self )
	local mainplayer = ActorMgr:getMainPlayer();
	local container = mainplayer:getContainer();

	
	for i = 1, container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES ) do
		local Item = container:getItem( CONTAINER_TYPE_SUNDRIES, (i-1) );
		if Item:getItemId() ~= 0 then 
			local gemNewDef = GameArmProce:getGemNewDef( Item:getItemId() ); 
			if gemNewDef ~= nil then
				return true;
			end
		end
	end

	return false;
end

function FindSomeThingInContainer( nResultID )
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	local container = MainPlayer:getContainer();
	if container == nil then return end
	for i = (nContainerPage - 1)*nTASKGridMax + 1, container:getContainerMaxNum( curContainerType ) do
		local Item = container:getItem( curContainerType,(i-1) )
		if Item:getItemId() ~= 0 and Item:getItemDef().Result[0].ResultID == nResultID then
			return true, Item:getItemId();
		end
	end
	return false, 0;
end

function ShowContainerGuide()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	ChangeMoney();
	local container = MainPlayer:getContainer();
	if container == nil then return end
	local bFound = false;
	if bSetGuidLastTime then return end
	-- 
	if not GameWizard:isDo(GAMEWIZARD_FIRST_HAVEWEAPON) and Quest:isQuestFini( 1056 ) then
		for i = (nContainerPage - 1)*nTASKGridMax + 1, container:getContainerMaxNum( curContainerType ) do
			local Item = container:getItem( curContainerType, (i-1) );
			if Item:getItemId() == GetFirstWeaponId( nRace ) then
				--GameWizard:doStep(GAMEWIZARD_FIRST_HAVEWEAPON);
				GuideEffectShow( "PlayerContainerFrame","点击鼠标右键\n即可装备","PlayerContainerFrameItem"..i, "点击鼠标右键\n即可装备", "uires\\ZhuJieMian\\1.dds", 284, 602, 67, 67, 67, 67, -12, 12, 34, -52, false );
				bFound = true;
			end
		end
	end

	if GameWizard:isDo(GAMEWIZARD_FIRST_HAVECLOSE) == false then
		for i = (nContainerPage - 1)*nTASKGridMax + 1, container:getContainerMaxNum( curContainerType ) do
			local Item = container:getItem( curContainerType, (i-1) );
			if Item:getItemId() == GetFirstCloseId( nRace ) then
				GameWizard:doStep(GAMEWIZARD_FIRST_HAVECLOSE);
				GuideEffectShow( "PlayerContainerFrame","点击鼠标右键\n即可装备","PlayerContainerFrameItem"..i, "点击鼠标右键\n即可装备", "uires\\ZhuJieMian\\1.dds", 284, 602, 67, 67, 67, 67, -12, 12, 34, -52, false );
				bFound = true;
			end
		end
	end
	
	-- 第一次获得精灵道具物品	and Quest:isQuestFini( 3111 )
	if not GameWizard:isDo( GAMEWIZARD_FIRST_GETXIAN_ITEM ) then
		for i = (nContainerPage - 1)*nTASKGridMax + 1, container:getContainerMaxNum( curContainerType ) do
			local Item = container:getItem( curContainerType, (i-1) );
			if( Item:getItemId() == 5620000 ) then 
				--GameWizard:doStep( GAMEWIZARD_FIRST_GETXIAN_ITEM );
				GuideEffectShow( "PlayerContainerFrame","","PlayerContainerFrameItem"..i, "鼠标右键点击使用，解除封印获得精灵", "uires\\ZhuJieMian\\1.dds", 284, 602, 67, 67, 67, 67, -12, 12, 34, -52, false );
				bFound = true;
			end
		end
	end
	
	-- 第一次获得骑乘道具物品	and Quest:isQuestFini( 3132 )
	if not GameWizard:isDo( GAMEWIZARD_FIRST_GETRIDE_ITEM )  then
		for i = (nContainerPage - 1)*nTASKGridMax + 1, container:getContainerMaxNum( curContainerType ) do
			local Item		= container:getItem( curContainerType, (i-1) );
			local itemDef	= Item:getItemDef();
			if itemDef ~= nil and itemDef.Result[0].ResultID == RESULT_RIDE then 
				
				--GameWizard:doStep( GAMEWIZARD_FIRST_GETRIDE_ITEM );
				GuideEffectShow( "PlayerContainerFrame","","PlayerContainerFrameItem"..i, "点击鼠标右键可使用骑乘", "uires\\ZhuJieMian\\1.dds", 284, 602, 67, 67, 67, 67, -12, 12, 34, -52, false );
				bFound = true;
			end
		end
	end
	
	-- 第一次获得骑乘强化石		Quest:isQuestRun( 3134 )
	if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_CON_STRENGTHEN_RIDE_ITEM_BTN ) and 
		GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_STRENGTHEN_BTN ) and Quest:isQuestRun( 3134 ) then
		
		local nInitRace	= GetInitRace( MainPlayer:getRace() );
		for i = (nContainerPage - 1)*nTASKGridMax + 1, container:getContainerMaxNum( curContainerType ) do
			local Item = container:getItem( curContainerType, (i-1) );
			if( Item:getItemId() == 5234000 ) then 
				GuideEffectShow( "PlayerContainerFrame","","PlayerContainerFrameItem"..i, "点击鼠标右键可强化骑乘", "uires\\ZhuJieMian\\1.dds", 284, 602, 67, 67, 67, 67, -12, 12, 34, -52, false );
				bFound = true;
			end
		end
	end
	
	-- 第一次获得武器装备		
	local t_firstArms = GuideFirstArm();
	if not GameWizard:isDo( GAMEWIZARD_FIRST_USEWEAPONITEM ) and Quest:isQuestRun( 1056 ) then
		local itemId = t_firstArms[MainPlayer:getRace() % 0x10]["armId"];
		for i = (nContainerPage - 1)*nTASKGridMax + 1, container:getContainerMaxNum( curContainerType ) do
			local Item = container:getItem( curContainerType, (i-1) );
			if( Item:getItemId() == itemId ) then
				GuideEffectShow( "PlayerContainerFrame","","PlayerContainerFrameItem"..i, "点击鼠标右键装备该武器", "uires\\ZhuJieMian\\1.dds", 284, 602, 67, 67, 67, 67, -12, 12, 34, -52, false );
				bFound = true;
				break;
			end
		end
	end
	
	local isPetSkillLearnItem = 
	function ( data )
		local itemDef				= data["itemDef"];
		local t_petSkillLearnConfig	= data["config"];
		for _, resultId in ipairs( t_petSkillLearnConfig.itemPre.resultId ) do
			if resultId == itemDef.Result[0].ResultID then
				return true;
			end
		end

		return false;
	end

	-- TODO: 策划给个任务ID, 第一次学习宠物技能 and Quest:isQuestRun( 3134 )
	if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_CONTAINER_LEARN_PET_SKILL_BOOK_BTN ) and 
		GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_NPCTALK_LEARN_PET_SKILL_OPTION ) then

		local t_petSkillLearnConfig = GetPetItemFramesConfig()["Pet_SkillLearnFrame"];
		for i = (nContainerPage - 1)*nTASKGridMax + 1, container:getContainerMaxNum( curContainerType ) do
			local Item = container:getItem( curContainerType, (i-1) );
			if Item:getItemId() ~= 0  and isPetSkillLearnItem( { itemDef = Item:getItemDef(), config = t_petSkillLearnConfig } ) then 
				GuideEffectShow( "PlayerContainerFrame","","PlayerContainerFrameItem"..i, "右键点击放入技能书", "uires\\ZhuJieMian\\1.dds", 284, 602, 67, 67, 67, 67, -12, 12, 34, -52, false );
				bFound = true;
			end
		end
	end
	

	local isPetDevelopmentBuildUpItem = 
	function ( data )
		local itemDef						= data["itemDef"];
		local t_petDevelopmentBuildUpConfig	= data["config"];
		for _, resultId in ipairs( t_petDevelopmentBuildUpConfig.itemPre.resultId ) do
			if resultId == itemDef.Result[0].ResultID then
				return true;
			end
		end

		return false;
	end

	-- TODO: 策划给个任务ID, 第一次强化宠物 and Quest:isQuestRun( 3134 )
	if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_CONTAINER_STRENGTHEN_ITEM_BTN ) and 
		GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_NPCTALK_STRENGTHEN_PET_OPTION ) then
		local t_petDevelopmentBuildUpConfig = GetPetItemFramesConfig()["Pet_DevelopmentBuildUpFrame"];
		for i = (nContainerPage - 1)*nTASKGridMax + 1, container:getContainerMaxNum( curContainerType ) do
			local Item = container:getItem( curContainerType, (i-1) );
			if Item:getItemId() ~= 0  and isPetDevelopmentBuildUpItem( { itemDef = Item:getItemDef(), config = t_petDevelopmentBuildUpConfig } ) then 
				GuideEffectShow( "PlayerContainerFrame","","PlayerContainerFrameItem"..i, "右键点击放入宠物强化石", "uires\\ZhuJieMian\\1.dds", 284, 602, 67, 67, 67, 67, -12, 12, 34, -52, false );
				bFound = true;
			end
		end
	end
	
	-- 检查自动拾取道具的新手指引 and MainPlayer:getLv() >= 31
	if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_CONTAINERFRAME_BOOTY_ITEM_BTN ) then
		for i = (nContainerPage - 1)*nTASKGridMax + 1, container:getContainerMaxNum( curContainerType ) do
			local Item = container:getItem( curContainerType, (i-1) );
			
			if Item:getItemId() ~= 0  then 
				if CheckAutoPickItem( Item:getItemId(), Item:getGridIdx() ) then
					GuideEffectShow( "PlayerContainerFrame","","PlayerContainerFrameItem"..i, 
									"恭喜您获得自动拾取道具，右键点击使用", "uires\\ZhuJieMian\\1.dds", 
										284, 602, 67, 67, 67, 67, -12, 12, 34, -52, false );
					bFound = true;
					break;
				end
			end
		end
	end

	-- TODO: 策划给个任务ID, 第一次获得宠物经验果 and Quest:isQuestRun( 3134 )	and MainPlayer:getLv() >= 30 
	if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_CONTAINER_ADD_PET_EXP_ITEM_BTN ) then
		for i = (nContainerPage - 1)*nTASKGridMax + 1, container:getContainerMaxNum( curContainerType ) do
			local Item = container:getItem( curContainerType, (i-1) );
			
			if Item:getItemId() ~= 0  and Item:getItemDef().Result[0].ResultID == RESULT_ADD_EXP_PET then 
				GuideEffectShow( "PlayerContainerFrame","","PlayerContainerFrameItem"..i, "恭喜您获得宠物经验果，右键点击使用", "uires\\ZhuJieMian\\1.dds", 284, 602, 67, 67, 67, 67, -12, 12, 34, -52, false );
				bFound = true;
			end
		end
	end

	-- TODO: 策划给个任务ID, 第一次获得宠物蛋 and Quest:isQuestRun( 3134 )
	if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_PET_EGG_CONTAINER_BTN ) then
		for i = (nContainerPage - 1)*nTASKGridMax + 1, container:getContainerMaxNum( curContainerType ) do
			local Item = container:getItem( curContainerType, (i-1) );
			
			if Item:getItemId() ~= 0  and Item:getItemDef().Result[0].ResultID == RESULT_PET_HATCH then 
				GuideEffectShow( "PlayerContainerFrame","","PlayerContainerFrameItem"..i, "恭喜您获得宠物蛋，右键点击使用", "uires\\ZhuJieMian\\1.dds", 284, 602, 67, 67, 67, 67, -12, 12, 34, -52, false );
				bFound = true;
				GameWizard:doStep( GAMEWIZARD_FIRST_BLINK_PET_EGG_CONTAINER_BTN );
			end
		end
	end
	
	--TODO HERE 第一次获取机甲兽
	if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_JIJIASHOU_CONTAINER_BTN ) then
		for i = (nContainerPage - 1)*nTASKGridMax + 1, container:getContainerMaxNum( curContainerType ) do
			local Item = container:getItem( curContainerType, (i-1) );			
			if Item:getItemId() ~= 0 and Item:getListType() == CONTAINER_TYPE_TASK and Item:getItemDef().Result[0].ResultID == RESULT_MACHINE_ITEM then 
				GuideEffectShow( "PlayerContainerFrame","","PlayerContainerFrameItem"..i, "鼠标右键点击乘骑机甲兽", "uires\\ZhuJieMian\\1.dds", 284, 602, 67, 67, 67, 67, -12, 12, 34, -52, false );
				bFound = true;
				break;
			end
		end
	end
	if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_GET_SPAR_CONTAINER_BTN ) then
		for i = (nContainerPage - 1)*nTASKGridMax + 1, container:getContainerMaxNum( curContainerType ) do
			local Item = container:getItem( curContainerType,(i-1) )
			if Item:getItemId() ~= 0 and Item:getItemDef().Result[0].ResultID == RESULT_SPAR then
				GuideEffectShow( "PlayerContainerFrame","","PlayerContainerFrameItem"..i, "恭喜你获得一个能源晶石，右键点击使用", "uires\\ZhuJieMian\\1.dds", 284, 602, 67, 67, 67, 67, -12, 12, 34, -52, false );
				bFound = true;
				break;
			end
		end
	end

	if not GameWizard:isDo( GAMEWIZARD_FIRST_GET_RUNE_CONTAINER_BTN ) and MainPlayer:getLv() >= RUNE_OPEN_LV then
		for i = (nContainerPage - 1)*nTASKGridMax + 1, container:getContainerMaxNum( curContainerType ) do
			local Item = container:getItem( curContainerType,(i-1) )
			if Item:getItemId() ~= 0 and Item:getItemDef().Result[0].ResultID == RESULT_RUNE then
				GuideEffectShow( "PlayerContainerFrame","","PlayerContainerFrameItem"..i, "使用快捷键（K）打开技能面板，鼠标左键拖拽符文至技能列表中，激活符文技能", "uires\\ZhuJieMian\\1.dds", 284, 602, 67, 67, 67, 67, -12, 12, 34, -52, false );
				bFound = true;
				break;
			end
		end
	end

	if GameWizard:isDo( GAMEWIZARD_FIRST_GET_BROKEN_JIJIASHOU_STEP2 ) then
		for i = (nContainerPage - 1)*nTASKGridMax + 1, container:getContainerMaxNum( curContainerType ) do
			local Item = container:getItem( curContainerType, (i-1) );			
			if Item:getItemId() == 5300884 then 
				GuideEffectShow( "PlayerContainerFrame","","PlayerContainerFrameItem"..i, "格罗佛把心爱的驱龙送给你，你可要好好珍惜哟，等你25级时就可以骑乘它来战斗了", "uires\\ZhuJieMian\\1.dds", 284, 602, 67, 67, 67, 67, -12, 12, 34, -52, false );
				bFound = true;
				break;
			end
		end
	end

	--[[
	if t_getItemSourceControl:showBootyXinShouGuide() then
		bFound = true;
	end
	--]]

	--if not bFound or not SundriesContainer:IsChecked() then
	if not bFound then
		-- 关闭新手提示特效
		HideContainerEffectFrame();
	end
end

function isAutoPickItem( data )
	local itemDef = getItemDef( data["id"] );
	for i = 1, MAX_ITEM_RESULT do
		local oneResult = itemDef.Result[i-1];
		if oneResult.ResultID == RESULT_SELF_STATUS and oneResult.ResultVal2 == 1122 then
			return true;
		end
	end

	return false;
end

function CheckAutoPickItem( nDefId, nGrid )
	if isAutoPickItem( { id = nDefId } ) and not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_CONTAINERFRAME_BOOTY_ITEM_BTN ) then
		return true;
	end

	return false;
end

function GetNewItem( nDefId, nGrid )
	local itemDef = getItemDef( nDefId );
	local mainplayer = ActorMgr:getMainPlayer();
	--  and mainplayer:getLv() >= 32
	if itemDef.Result[0].ResultID == RESULT_ADD_EXP_PET and 
		not GameWizard:isDo( GAMEWIZARD_FIRST_OPEN_CONTAINERFRAME_FIRST_GET_ADD_PET_EXP_ITEM ) then
		local containerFrame = getglobal( "PlayerContainerFrame" );
		GameWizard:doStep( GAMEWIZARD_FIRST_OPEN_CONTAINERFRAME_FIRST_GET_ADD_PET_EXP_ITEM );
		if not containerFrame:IsShown() then
			containerFrame:Show();
		else
			ShowContainerGuide();
		end
	end
	
	--  and mainplayer:getLv() >= 30
	if CheckAutoPickItem( nDefId, nGrid ) then
		local containerFrame = getglobal( "PlayerContainerFrame" );
		if not containerFrame:IsShown() then
			containerFrame:Show();
		else
			ShowContainerGuide();
		end
	end

	
	if itemDef.Result[0].ResultID == RESULT_PET_HATCH and not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_PET_EGG_CONTAINER_BTN ) then
		local containerFrame = getglobal( "PlayerContainerFrame" );
		
		if not containerFrame:IsShown() then
			containerFrame:Show();
		else
			ShowContainerGuide();
		end
	end
	--CheckBootyXinShouGuide( nGrid );
end

function UseNewItem( nDefId, nGrid )
	-- and mainplayer:getLv() >= 30
	if CheckAutoPickItem( nDefId, nGrid )  then
		GameWizard:doStep( GAMEWIZARD_FIRST_BLINK_CONTAINERFRAME_BOOTY_ITEM_BTN );
		HideContainerEffectFrame();
	end
end

function CheckBootyXinShouGuide( nGrid )
	local nGetItemSource = t_getItemSourceControl:getItemSource();
	if nGetItemSource == GET_TIEM_BY_BOOTY and not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_CONTAINERFRAME_BOOTY_ITEM_BTN ) and 
		t_getItemSourceControl:getItemGrid() == -1 then
		t_getItemSourceControl:setItemGrid( { grid = nGrid } );
		if PlayerContainerFrame:IsShown() then
			ShowContainerGuide();
		else
			--print( "显示背包面板" );
			PlayerContainerFrame:Show();
			PlayerContainerFrame:SetPoint( "center", "UIClient", "center", 0, 0 );
		end
	end
end

function GetNewArm( nDefId, nGrid )
	--CheckBootyXinShouGuide( nGrid );
end
 
function GetNewSparNum()
	if not SparButton:IsShown() then
		SparButton:Show();
		SetShortCutHideTexture( 3 )
	end
	local UVATexture = getglobal( "SparButtonUVAnimationTex" );
	UVATexture:SetUVAnimation(50,true);
	UVATexture:Show();

	local szText = "能源中开启了一个新的晶石孔，\n点击查看，\n快捷键（J）";
	local xinShouBtn = getglobal( "ShortCutButtonsFrameXinShouBtn" );
	local rich = getglobal( xinShouBtn:GetName().."Rich" );
	rich:SetClientString( szText );
	local nSelfDefineExtendWidth = 14;
	ReSizeTooltip( rich:GetName(), xinShouBtn:GetName(), nSelfDefineExtendWidth );
	xinShouBtn:SetPoint( "BOTTOMLEFT","SparButton","TOPLEFT",10,-17 )
	local t_CharacterButtonControl = GetCharacterButtonControl();
	t_CharacterButtonControl:startShowLevelUpAnim();
	xinShouBtn:Show();
end

-- 重置窗口坐标
function ResetContainerPoint()
	local t_NotReset = { "StashFrame", "TradeFrame", "StallSaleFrame" };
	local bFound = false;
	for _, szNotRest in ipairs( t_NotReset ) do
		frame = getglobal(szNotRest);
		if frame:IsShown() then
			bFound = true;
		end
	end
	if bOpenContainerFlag then
		PlayerContainerFrame:SetPoint( "topright", "$parent", "topright",-86, 163 );
		bOpenContainerFlag = false;	
	end
	local frame 		= getglobal("PlayerContainerFrame");
	local nScreenWidth	= GetScreenWidth();
	local nScreenHeight	= GetScreenHeight();
	if frame:GetRealTop() < 0
	    or frame:GetRealLeft() < 0
	    or frame:GetRealBottom() > nScreenHeight
	    or frame:GetRealRight() > nScreenWidth then
		PlayerContainerFrame:SetPoint( "topright", "$parent", "topright",-86, 163 );
	end
	--if not bFound then
		--PlayerContainerFrame:SetPoint("topright", "$parent", "topright", -310, 163 );
	--end
end
 
local index = 1;
-- 更新背包面板的摆摊按钮的状态
function UpdateContainerFrame_StallBtnState()
	local mainplayer = ActorMgr:getMainPlayer();
	local btn = getglobal( "ContainerFrame_StallageBtn" );
	local bGray = mainplayer:getLv() < CLIENT_MIN_STALL_LEVEL;
	btn:SetGray( bGray );
end

function ContainerFrame_OnShow()
	if BetterEquipFrame:IsShown() then
		BetterEquipFrame:Hide();
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer()
	--合服后 角色改名
	if string.find( mainplayer:getName(),"*",1,true ) then
		ContainerChangeRoleFrame:SetClientUserData( 0,1 );
		ContainerChangeRoleFrame:Show();
		PlayerContainerFrame:Hide();
		return;
	end

	-- 重置窗口坐标
	ResetContainerPoint();

	curContainerType = CONTAINER_TYPE_SUNDRIES;
	ReSetContainerBtn();
	SundriesContainer:Checked(); 
	nContainerPage = 1;
	ContainerFrameOnShow();
	ShowContainerGuide();
	--UpdateContainerMarketFrame();
	--ContainerRefreshLockFrame();
	UpdateContainerFrame_StallBtnState();

	if mainplayer:isInStall() and not StallSaleFrame:IsShown() then
		StallSaleFrame:Show();
		StallSaleFrame:SetPoint("TOPLEFT", "$parent", "TOPLEFT", 185, 140 );		
	end
	if StallSaleFrame:IsShown() then
		PlayerContainerFrame:SetPoint("TOPLEFT", "$parent", "TOPLEFT",StallSaleFrame:GetWidth() + 185, 140 );
	end
	PlayerContainerFrameLockBtn:SetNormalTexture("uires\\ZhuJieMian\\1.dds","blend");
	PlayerContainerFrameLockBtn:SetHightlightTexture("uires\\ZhuJieMian\\1.dds","blend");
	PlayerContainerFrameLockBtn:SetPushedTexture("uires\\ZhuJieMian\\1.dds","blend");

	PlayerContainerFrameLockBtnEffectTex:Hide();
	if not bHaveOpenContainer then
		UpdateContainerLockEffect( "PlayerContainerFrameLockBtn" );
		bHaveOpenContainer = true;
	end
	ContainerButtonLockAnimationTex:Hide();
	ClanButtonLockAnimationTex:Hide();

	local unUsedNum = container:getUnusedGridNum(CONTAINER_TYPE_SUNDRIES);
	local totalNum = container:getContainerMaxNum(CONTAINER_TYPE_SUNDRIES);

	ContainerFrame_VipShopBtn:SetGray( not VipManager:isOwnVipFunc( STAR_RESULT_SHOP_TOLUA ) );
	ContainerFrame_VipStashBtn:SetGray( not VipManager:isOwnVipFunc( STAR_RESULT_STASH_TOLUA ) );
	ContainerFrame_VipStoreBtn:SetGray( not VipManager:isOwnVipFunc( STAR_RESULT_STORE_TOLUA ) );
end

--[[
local tContainerTips = {
							"你可以用Tab键来选定、切换你面前的敌人",
							"你可以在系统设置→鼠标设置选择控制方式",
							"可以在区域地图[快捷键M ]中输入坐标寻路",
							"当你＞30级，被怪物打死会受到死亡惩罚",
							"战斗中请多加留意自己的血量",
							"生命与魔法过低时可通过食物与药水恢复",
							"地宫与副本中没有出售食物的商人",
							"常用的道具、药水、食物可以放在快捷栏中",
							"技能面板[快捷键K ]中有你学会的所有技能",
							"蓝色！代表此任务可重复完成",
							"任务失败或丢失道具时，请放弃任务重新接",
							"任务数量过多时，请注意任务追踪共有3页",
							"活动助手[快捷键H ]可查看活动的开放条件",
							"点击[ 回购 ]可以找回自己刚刚出售的物品",
							"任务物品通常放在包裹里的[ 任务栏 ]",
							"有些任务物品是可以被交易的",
							"任务物品不可出售，商人也是有原则的",
							"你可以通过Esc键来取消当前选择的目标",
							"用左键点聊天区玩家的名字，可对他私聊",
							"按Enter [ 回车键 ]可激活聊天区域",
						};
]]--

local tLineCovert = {[1]=7,[2]=6,[3]=5,[4]=4,[5]=3,[6]=2,[7]=1,[8]=0};

function ContainerFramesShowTips()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	local container = MainPlayer:getContainer();
	local nLine = math.ceil( container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES )/8 );
	--local nLine = math.ceil( (nMaxNum - (nContainerPage - 1)*nTASKGridMax)/8 );
	if nLine > 8 then
		nLine = 8;
	end
	if curContainerType == CONTAINER_TYPE_TASK then
		nLine = 8;
	end
	--调整界面大小
	PlayerContainerFrame:SetSize( 412,192 + 44*nLine );
	PlayerContainerFrameBackGroundBtn:SetSize( 406,158 + 44*nLine );
	PlayerContainerFrameItemBackBkg:SetSize( 370,20 + 44*nLine );
	PlayerContainerFrameItemBackBkgTex:SetSize( 368,20 + 44*nLine - 3 );
	PlayerContainerFrameItemBackBkgTex:SetTexUV( 0,0,368,20 + 44*nLine - 3 );
--[[
	--TODO HERE
	--local backTex = getglobal("PlayerContainerFrameItemBackBkgBackTex");
	--backTex:SetSize(368,44*nLine + 15 );
	
	local t_Tmp = {};
	for i=1,table.getn( tContainerTips ) do
		table.insert(t_Tmp,tContainerTips[i]);
	end

	local szText = "";
	for i=1,tLineCovert[nLine] do
		if table.getn( t_Tmp ) == 0 then
			break;
		end
		local nIndex = math.random( 1,table.getn( t_Tmp ));
		szText = szText.."#P"..t_Tmp[nIndex].."\n";
		table.remove( t_Tmp,nIndex );
	end
	ContainerFrameTipsRich:SetText(szText,95,50,0);
	ContainerFrameTipsRich:SetPoint("topleft","PlayerContainerFrameItemBackBkgBackTex","bottomleft",10,8+7*( 8-nLine ) );
]]--
end

function ContainerFrameOnShow()		
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	ChangeMoney();
	local container = MainPlayer:getContainer();
	if container == nil then return end
	local nRace = MainPlayer:getRace();
	RemoveCheckItem( "PlayerContainerFrame" );
	PlayerContainerFrameTrade:Hide();

	if curContainerType == CONTAINER_TYPE_SUNDRIES then
		local nMaxNum = container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES );
		nContainerMaxPage = math.floor((nMaxNum - 1)/nSUNDRIESGridMax) + 1;
		if nContainerPage > nContainerMaxPage then
			nContainerMaxPage = nContainerMaxPage;
		end
		nMaxNum = nMaxNum - (nContainerPage - 1)*nTASKGridMax;
		for i=1, 8, 1 do
			for j=1, 8, 1 do
				local bagButton = getglobal( "PlayerContainerFrameItem"..((i-1)*8+j));
				local ItemNum   = getglobal( "PlayerContainerFrameItem"..((i-1)*8+j).."CountFont");
				local ItemBackTex = getglobal( "PlayerContainerFrameItem"..((i-1)*8+j).."BackTexture");
				local ItemBoxTex = getglobal( "PlayerContainerFrameItem"..((i-1)*8+j).."BoxTexture");
				local UVAnimationTex = getglobal( "PlayerContainerFrameItem"..((i-1)*8+j).."UVAnimationTex");
				local ItemTex = getglobal( "PlayerContainerFrameItem"..((i-1)*8+j).."IconTexture");
				local bindTexture = getglobal( "PlayerContainerFrameItem" ..((i-1)*8+j).. "BindTexture" );
				
				bagButton:Hide();
				ItemTex:SetGray(false);				
				ItemBackTex:Show();
				ItemNum:SetText("");
				ItemBoxTex:Hide();
				UVAnimationTex:Hide();
				bagButton:Disable();
				bindTexture:Hide();
				bagButton:SetDisableTexture( "uires\\ui\\mask\\TouMingTongDao.tga", "blend" );
				
				bagButton:SetPoint("TOPLEFT", "PlayerContainerFrameItemBackBkg", "TOPLEFT", (j-1)*44+10, (i-1)*44 + 10 );
				if ((i-1)*8+j) <= nMaxNum then
					bagButton:Show();
					bagButton:SetClientID( ((i-1)*8+j) + (nContainerPage - 1)*nSUNDRIESGridMax );
					bagButton:Enable();
					if IsContainerItemLock( CONTAINER_TYPE_SUNDRIES, ((i-1)*8+j-1) + (nContainerPage - 1)*nSUNDRIESGridMax ) then
						ItemTex:SetGray(true);
					end
					local Item = container:getItem( CONTAINER_TYPE_SUNDRIES, ((i-1)*8+j-1) + (nContainerPage - 1)*nSUNDRIESGridMax );
					if( Item:getItemId() > 0 ) then
						bagButton:SetClientUserData( 2, Item:getNum() );
						local itemdef = Item:getItemDef();
						if itemdef ~= nil then
							local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
							-- 如果没找到物品的图标则使用默认图标显示
							if not IsInExistence( IconPath ) then
								IconPath = DEFAULT_ITEM_PATH;
							end
							--如果是失落图腾物品，要根据激活状态来显示不同的图标
							if CheckItemResult( itemdef, RESULT_LOST_TOTEM ) then
								if Item:getLostTotemState() == 1 then
									IconPath = GetItemEquipIconPath().."505234.tga";
								elseif Item:getLostTotemState() == 2 then
									IconPath = GetItemEquipIconPath().."505235.tga";
								else
									IconPath = GetItemEquipIconPath().."505233.tga";
								end
							end
							ItemTex:SetTexture( IconPath );
							SetEquipImportantEffectUV( itemdef, UVAnimationTex:GetName(),Item);
							ItemBoxTex:Show();
							if( Item:getNum() > 1  ) then
								ItemNum:SetText( Item:getNum() );
							else
								ItemNum:SetText( "" );
							end	   
							bagButton:SetCooldownTimer( 0, 0, true );
							-- 获取CD开始的时间 设置按钮CD
							if Item:getItemType() ~= ITEM_SLOTTYPE_NONE then
								local cd		= container:getItemCD( Item:getItemId() );
								local starttime = container:getItemCDPassTime( Item:getItemId() );
								bagButton:SetCooldownTimer( starttime, starttime+cd, true );
								bagButton:SetCooldownTextureRect( 3, 5, 38, 38 );
							end
						end
						if Item:isItemBind() then
							bindTexture:Show();
						end
					else
						ItemTex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
						ItemNum:SetText( "" );
						UVAnimationTex:Hide();
						ItemBoxTex:Hide();
						bagButton:SetCooldownTimer( 0, 0, true );
					end
				end
			end
		end
		PlayerContainerFrameShowPageButton( true );
	end

	if curContainerType == CONTAINER_TYPE_TASK then
		local nMaxNum = container:getContainerMaxNum( CONTAINER_TYPE_TASK );		
		nContainerMaxPage = math.floor((nMaxNum - 1)/nTASKGridMax) + 1;
		if nContainerPage > nContainerMaxPage then
			nContainerMaxPage = nContainerMaxPage;
		end
		nMaxNum = nMaxNum - (nContainerPage - 1)*nTASKGridMax;
		for i=1, 8, 1 do
			for j=1, 8, 1 do
				local bagButton = getglobal( "PlayerContainerFrameItem"..((i-1)*8+j));
				local ItemNum   = getglobal( "PlayerContainerFrameItem"..((i-1)*8+j).."CountFont");
				local ItemBackTex = getglobal( "PlayerContainerFrameItem"..((i-1)*8+j).."BackTexture");
				local ItemBoxTex = getglobal( "PlayerContainerFrameItem"..((i-1)*8+j).."BoxTexture");
				local UVAnimationTex = getglobal( "PlayerContainerFrameItem"..((i-1)*8+j).."UVAnimationTex");
				local ItemTex = getglobal( "PlayerContainerFrameItem"..((i-1)*8+j).."IconTexture");
				local bindTexture = getglobal( "PlayerContainerFrameItem" ..((i-1)*8+j).. "BindTexture" );
				bindTexture:Hide();
				bagButton:Hide();
				bagButton:SetCooldownTimer( 0, 0, true );
				ItemTex:SetGray(false);
				if IsContainerItemLock( CONTAINER_TYPE_TASK, ((i-1)*8+j)-1 ) then
					ItemTex:SetGray(true);
				end
				bagButton:SetPoint("TOPLEFT", "PlayerContainerFrameItemBackBkg", "TOPLEFT", (j-1)*44+10, (i-1)*44 +10);
				ItemBackTex:Show();
				ItemNum:SetText("");

				ItemBoxTex:Hide();
				UVAnimationTex:Hide();
				bagButton:Disable();
				bagButton:SetDisableTexture( "uires\\ui\\mask\\TouMingTongDao.tga", "blend" );
				
				if ((i-1)*8+j) <= nMaxNum then	
					bagButton:Show();
					bagButton:SetClientID( ((i-1)*8+j) + (nContainerPage - 1)*nTASKGridMax );
					bagButton:Enable();
					local Item = container:getItem( CONTAINER_TYPE_TASK, ((i-1)*8+j-1) + (nContainerPage - 1)*nTASKGridMax );
					if( Item:getItemId()  > 0 ) then	  
						bagButton:SetClientUserData( 2, Item:getNum() );
						local itemdef = Item:getItemDef();
						local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
						-- 如果没找到物品的图标则使用默认图标显示
						if not IsInExistence( IconPath ) then
							IconPath = DEFAULT_ITEM_PATH;
						end
						ItemTex:SetTexture( IconPath );	

						SetEquipImportantEffectUV( itemdef, UVAnimationTex:GetName(),Item);
						ItemBoxTex:Show();
						if( Item:getNum() > 1 ) then
							ItemNum:SetText( Item:getNum() );
							index = index+1;
						else
							ItemNum:SetText( "" );
						end
						bagButton:SetCooldownTimer( 0, 0, true );
						-- 获取CD开始的时间 设置按钮CD
						if Item:getItemType() ~= ITEM_SLOTTYPE_NONE then
							local cd		= container:getItemCD( Item:getItemId() );
							local starttime = container:getItemCDPassTime( Item:getItemId() );
							bagButton:SetCooldownTimer( starttime, starttime+cd, true );
							bagButton:SetCooldownTextureRect( 3, 5, 38, 38 );
						end
						if Item:isItemBind() then
							bindTexture:Show();
						end
						if Item:getItemDef().Result[0].ResultID == RESULT_MACHINE_ITEM and Item:getItemInstVal1() == 0 then
							ItemTex:SetGray( true )
						end
					else
						ItemTex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
						ItemNum:SetText( "" );
						UVAnimationTex:Hide();
						ItemBoxTex:Hide();
					end
				end
			end
		end

		PlayerContainerFrameTrade:Show();
		-- 关闭新手提示特效
		HideContainerEffectFrame();
		PlayerContainerFrameShowPageButton( false );
	end

	if curContainerType == CONTAINER_TYPE_STUFF then
		local nMaxNum = container:getContainerMaxNum( CONTAINER_TYPE_STUFF );		
		nContainerMaxPage = math.floor((nMaxNum - 1)/nTASKGridMax) + 1;
		if nContainerPage > nContainerMaxPage then
			nContainerMaxPage = nContainerMaxPage;
		end
		nMaxNum = nMaxNum - (nContainerPage - 1)*nTASKGridMax;
		for i=1, 8, 1 do
			for j=1, 8, 1 do
				local bagButton = getglobal( "PlayerContainerFrameItem"..((i-1)*8+j));
				local ItemNum   = getglobal( "PlayerContainerFrameItem"..((i-1)*8+j).."CountFont");
				local ItemBackTex = getglobal( "PlayerContainerFrameItem"..((i-1)*8+j).."BackTexture");
				local ItemBoxTex = getglobal( "PlayerContainerFrameItem"..((i-1)*8+j).."BoxTexture");
				local UVAnimationTex = getglobal( "PlayerContainerFrameItem"..((i-1)*8+j).."UVAnimationTex");
				local ItemTex = getglobal( "PlayerContainerFrameItem"..((i-1)*8+j).."IconTexture");
				local bindTexture = getglobal( "PlayerContainerFrameItem"..((i-1)*8+j).. "BindTexture" );
				bagButton:Hide();
				bindTexture:Hide();

				bagButton:SetCooldownTimer( 0, 0, true );
				ItemTex:SetGray(false);
				if IsContainerItemLock( CONTAINER_TYPE_STUFF, ((i-1)*8+j)-1 ) then
					ItemTex:SetGray(true);
				end
				bagButton:SetPoint("TOPLEFT", "PlayerContainerFrameItemBackBkg", "TOPLEFT", (j-1)*44+10, (i-1)*44 +10 );
				ItemBackTex:Show();
				ItemNum:SetText("");

				ItemBoxTex:Hide();
				UVAnimationTex:Hide();
				bagButton:Disable();
				bagButton:SetDisableTexture( "uires\\ui\\mask\\TouMingTongDao.tga", "blend" );

				if ((i-1)*8+j) <= nMaxNum then     
					bagButton:Show();
					bagButton:SetClientID( ((i-1)*8+j) + (nContainerPage - 1)*nTASKGridMax );
					bagButton:Enable();
					local Item = container:getItem( CONTAINER_TYPE_STUFF, ((i-1)*8+j-1) + (nContainerPage - 1)*nTASKGridMax );
					if( Item:getItemId()  > 0 ) then
						bagButton:SetClientUserData( 2, Item:getNum() );
						local itemdef = Item:getItemDef();

						local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
						-- 如果没找到物品的图标则使用默认图标显示
						if not IsInExistence( IconPath ) then
							IconPath = DEFAULT_ITEM_PATH;
						end
						ItemTex:SetTexture( IconPath );	 

						SetEquipImportantEffectUV( itemdef, UVAnimationTex:GetName(),Item);
						ItemBoxTex:Show();
						if( Item:getNum() > 1 ) then
							ItemNum:SetText( Item:getNum() );
						else
							ItemNum:SetText( "" );
						end
						bagButton:SetCooldownTimer( 0, 0, true );
						-- 获取CD开始的时间 设置按钮CD
						if Item:getItemType() ~= ITEM_SLOTTYPE_NONE then
							local cd		= container:getItemCD( Item:getItemId() );
							local starttime = container:getItemCDPassTime( Item:getItemId() );
							bagButton:SetCooldownTimer( starttime, starttime+cd, true );
							bagButton:SetCooldownTextureRect( 3, 5, 38, 38 );
						end
						if Item:isItemBind() then
							bindTexture:Show();
						end
					else
						local ItemTex = getglobal( "PlayerContainerFrameItem"..((i-1)*8+j).."IconTexture");
						ItemTex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
						ItemNum:SetText( "" );
						UVAnimationTex:Hide();
						ItemBoxTex:Hide();
					end
				end
			end
		end		     
		-- 关闭新手提示特效
		HideContainerEffectFrame();
		PlayerContainerFrameShowPageButton( false );
	end

	-- 若主角处于摆摊状态，则显示收摊
	if MainPlayer:isInStall() then
		ContainerFrame_StallageBtn:Hide();
		ContainerFrame_EndStallageBtn:Show();
	else
		ContainerFrame_StallageBtn:Show();
		ContainerFrame_EndStallageBtn:Hide();
	end

	ShowContainerGuide();

	--设置页码
--	PlayerContainerFramePagePageFont:SetText( "第"..nContainerPage.."/"..nContainerMaxPage.."页" );
	PlayerContainerFramePagePageFont:SetText( nContainerPage.."/"..nContainerMaxPage );
	PlayerContainerFrameSetPackage();

	ContainerFrameOnUpdate();
	ContainerFramesShowTips();
end

function StashFrame_OnHide()
	if MessageBoxFrame:IsShown() then
		if MessageBoxFrame:GetClientString() == "背包绑定提示" and ( MessageBoxFrame:GetClientUserData(0) == CONTAINER_TYPE_STASH or MessageBoxFrame:GetClientUserData(2) == CONTAINER_TYPE_STASH ) then
			MessageBoxFrame:Hide();
		end
	end
	bOpenContainerFlag = true;
end

function ContainerFrameOnHide()
	if MessageBoxFrame:IsShown() then
		if MessageBoxFrame:GetClientString() == "背包绑定提示" and ( MessageBoxFrame:GetClientUserData(0) == CONTAINER_TYPE_SUNDRIES or MessageBoxFrame:GetClientUserData(2) == CONTAINER_TYPE_SUNDRIES ) then
			MessageBoxFrame:Hide();
		end
	end
	local resolveFrame	=	getglobal( "PlayerContainerResolveFrame");
	resolveFrame:Hide();
	PlayerContainerSparEpurateFrame:Hide();
	HideContainerEffectFrame();
	AutoHideNomalTips( "PlayerContainerFrame" );
	MoneyGuideFrame:Hide();
	if GemProceChargeFrame:IsShown() then
		GemProceChargeFrame:Hide();
	end
	if GameWizard:isDo( GAMEWIZARD_FIRST_GET_BROKEN_JIJIASHOU_STEP2 ) then
		GameWizard:undoStep( GAMEWIZARD_FIRST_GET_BROKEN_JIJIASHOU_STEP2 );
	end
	
end

function StashFrameOnHide()
	AutoHideNomalTips( "StashFrame" );
end

function HideContainerEffectFrame()
	if PlayerContainerFrameEffectFrame:IsShown() then
		PlayerContainerFrameEffectFrame:Hide();
	end
end

function StashFrameOnLoad()
	this:RegisterEvent("GE_CONTAINER_END");
	this:RegisterEvent("GE_CONTAINER_UPDATE");
	--注册仓库面板优先响应的回调函数
	ItemFirstCallBack( "StashFrame" , Save2Stash );
end

function StashFrameOnEvent()
	if ( arg1 == "GE_CONTAINER_UPDATE" ) then
		local frame = getglobal( "StashFrame" );
		if frame:IsShown() then
			StashFrameOnShow();
		end
	end

	if ( arg1 == "GE_CONTAINER_END" ) then
		StashFrame:Hide();
	end
end

function StashFrameOnShow()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	local container = MainPlayer:getContainer();
	if container == nil then return end
	
	RemoveCheckItem( "StashFrame" );
	if stashCurPage == CONTAINER_TYPE_STASH then
		local nMaxNum = container:getContainerMaxNum( CONTAINER_TYPE_STASH );
		local nInvalidNum = ContainerGetInvalidNum( curContainerType );
		local nInvalidIndex = nMaxNum - nInvalidNum;
		for i=1, 10, 1 do
			for j=1, 10, 1 do
				local bagButton = getglobal( "StashFrameItem"..((i-1)*10+j));
				local ItemNum   = getglobal( "StashFrameItem"..((i-1)*10+j).."CountFont");
				local ItemBackTex = getglobal( "StashFrameItem"..((i-1)*10+j).."BackTexture");
				local ItemBoxTex = getglobal( "StashFrameItem"..((i-1)*10+j).."BoxTexture");
				local UVAnimationTex = getglobal( "StashFrameItem"..((i-1)*10+j).."UVAnimationTex");
				local ItemTex = getglobal( "StashFrameItem"..((i-1)*10+j).."IconTexture");
				local bindTexture = getglobal( "StashFrameItem"..((i-1)*10+j).."BindTexture" );
				
				bagButton:SetPoint("TOPLEFT", "StashFrame", "TOPLEFT", (j-1)*44+19, (i-1)*44+52 );
				bagButton:Show();
				ItemBackTex:Show();
				bindTexture:Hide();

				if ((i-1)*10+j) <= nMaxNum then
					local Item = container:getItem( CONTAINER_TYPE_STASH, ((i-1)*10+j-1) );
					if( Item:getItemId() > 0 ) then	
						bagButton:SetClientUserData( 2, Item:getNum() );
						local itemdef = Item:getItemDef();
						local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
						-- 如果没找到物品的图标则使用默认图标显示
						if not IsInExistence( IconPath ) then
							IconPath = DEFAULT_ITEM_PATH;
						end
						--如果是失落图腾物品，要根据激活状态来显示不同的图标
						if CheckItemResult( itemdef, RESULT_LOST_TOTEM ) then
							if Item:getLostTotemState() == 1 then
								IconPath = GetItemEquipIconPath().."505234.tga";
							elseif Item:getLostTotemState() == 2 then
								IconPath = GetItemEquipIconPath().."505235.tga";
							else
								IconPath = GetItemEquipIconPath().."505233.tga";
							end
						end
						ItemTex:SetTexture( IconPath );		
						SetEquipImportantEffectUV( itemdef, UVAnimationTex:GetName(),Item);
						ItemBoxTex:Show();
						if( Item:getNum() > 1 ) then
							ItemNum:SetText( Item:getNum() );
						else
							ItemNum:SetText( "" );
						end

						-- 获取CD开始的时间 设置按钮CD
						for i = 1, table.getn( t_ItemCD ) do
							if Item:getItemType() ~= ITEM_SLOTTYPE_NONE and t_ItemCD[i][1] == Item:getItemId() then
								local starttime = container:getItemCDPassTime( Item:getItemId() );
								bagButton:SetCooldownTimer( starttime, t_ItemCD[i][2], true );
								bagButton:SetCooldownTextureRect( 3, 5, 38, 38 ); 
							else
								bagButton:SetCooldownTimer( 0, 0, true );
							end
						end
						if Item:isItemBind() then
							bindTexture:Show();
						end
					else
						ItemTex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
						ItemNum:SetText( "" );
						UVAnimationTex:Hide();
						ItemBoxTex:Hide();
					end
				end
			end
		end

		local nMaxNum = container:getContainerMaxNum( CONTAINER_TYPE_STASH );
		for i = nMaxNum + 1, nStashGridMax do
			local bagButton = getglobal( "StashFrameItem"..i);
			local ItemNum   = getglobal( "StashFrameItem"..i.."CountFont");
			ItemNum:SetText("");
			bagButton:Hide();
		end
	end

	StashContainerMomeyFont:SetText( MainPlayer:getStashMoney());
	StashContainerBullFont:SetText( MainPlayer:getStashBull());

	for i=1 ,table.getn(t_ItemButtonCheckIndex) do
		if t_ItemButtonCheckIndex[i][1] == "StashFrame" then
			local button = getglobal( "StashFrameItem"..(t_ItemButtonCheckIndex[i][2]+1) );
			button:Checked();
		end
	end	
	StashContainerFrameSetPackage();
	StashFrame_OnUpdate();

	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	local container = MainPlayer:getContainer();
	local nLine = math.ceil( container:getContainerMaxNum( CONTAINER_TYPE_STASH )/10 );
	
	--调整界面大小
	StashFrame:SetSize( 476,192 + 44*nLine );
	--StashFrame_ChildBackGround:SetSize( 470,158 + 44*nLine );
	StashFrame_ItemBkgTexture:SetSize( 460,28 + 44*nLine );
end

function StashContainerFrame_Close()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	local container = MainPlayer:getContainer();
	if container == nil then return end

	local panel = getglobal( "StashFrame" );
	panel:Hide();
	container:send_CloseStash();
end

-- 以下面板打开时背包不可整理
local t_CanNotTidy =
	{
		"TradeFrame",
		"InputFrame",
		"MailSendItemFrame",
	};

--背包物品整理
function ContainerFrame_TidyBtn_OnClick()
	--if bDown  then
	--	ShowMidTips( "操作受限,背包物品5秒只能整理一次");
	--	return;
	--end

	for i = 1, table.getn( t_CanNotTidy ) do
		local frame = getglobal( t_CanNotTidy[i] );
		if frame:IsShown() then
			ShowMidTips( "当前状态,无法整理背包" );
			return;
		end
	end

	if IsContainerHasItemLock( curContainerType ) then
		ShowMidTips( "背包中有物品被锁定，无法整理背包" );
		return;
	end

	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer:isInStall() then
		ShowMidTips( "摆摊状态中无法整理背包" );
		return;
	end
	
	local container = MainPlayer:getContainer();

	if container:isUseItemPre() then
		ShowMidTips("使用物品中，无法整理背包");
		return;
	end
	
	if ItemSplitInputFrame:IsShown() then
		ItemSplitInputFrame:Hide();
	end

	if curContainerType == CONTAINER_TYPE_SUNDRIES then
		container:send_PackageArrange(CONTAINER_TYPE_SUNDRIES);
		local t_getItemSourceControl = GetItemSourceControl();
		if t_getItemSourceControl:getBlinkItemGrid() ~= -1 then
			HideContainerEffectFrame();
			t_getItemSourceControl:setBlinkItemGrid( { grid = -1 } );
		end
	elseif curContainerType == CONTAINER_TYPE_STUFF then
		container:send_PackageArrange(CONTAINER_TYPE_STUFF);
	elseif curContainerType == CONTAINER_TYPE_TASK then
		container:send_PackageArrange(CONTAINER_TYPE_TASK);
	end

--[[
	ShowMidTips( "背包物品已整理");
--]]
	RemoveCheckItem( "PlayerContainerFrame" );
	bDown = true;
	bBeginRecord = true;
end

function ContainerFrameOnUpdate()	
	local player		= ActorMgr:getMainPlayer();
	local playerlv		= player:getLv();
	local containerResolveFrame	= getglobal( "PlayerContainerResolveFrame" )
	--等级限制
	--if playerlv < CONTAINER_RESOLVE_LEVEL_LIMIT_MIN or containerResolveFrame:IsShown() then
	if playerlv < CONTAINER_RESOLVE_LEVEL_LIMIT_MIN then
		local btn = getglobal("PlayerContainerFrameResolveBtn")
		btn:Disable();
		local touMingBtn = getglobal("PlayerContainerFrameResolveTouMingBtn")
		touMingBtn:Show();
	else
		local btn = getglobal("PlayerContainerFrameResolveBtn")
		btn:Enable();
		local touMingBtn = getglobal("PlayerContainerFrameResolveTouMingBtn")
		touMingBtn:Hide();
	end
	if bDown then
		if bBeginRecord then
			nLastTime = os.clock();
			bBeginRecord = false;
		end
		if ( os.clock() - nLastTime ) > PACKAGE_ARRANGE_MIN then
			ContainerFrame_TidyBtn:Enable();
			ContainerFrame_TidyBtn:DisHighlight();
			bDown = false;
		end
	end
	
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	local container = MainPlayer:getContainer();
	if container == nil then return end

	local nMaxNum = container:getContainerMaxNum( curContainerType );
	local nInvalidNum = ContainerGetInvalidNum( curContainerType );
	local nInvalidIndex = nMaxNum - nInvalidNum;
	nMaxNum = nMaxNum - (nContainerPage - 1)*nTASKGridMax;
	nInvalidIndex = nInvalidIndex - (nContainerPage - 1)*nTASKGridMax;
	for i=1,8 do
		for j=1,8 do
			if ((i-1)*8+j) <= nMaxNum then				
				local redTex = getglobal("PlayerContainerFrameItem"..((i-1)*8+j).."RedTexture")
				local iconTex = getglobal( "PlayerContainerFrameItem"..((i-1)*8+j).."IconTexture" )
				local validTime = getglobal( "PlayerContainerFrameItem"..((i-1)*8+j).."ValidTimeTex" )
				if ((i-1)*8+j) > nInvalidIndex then
					redTex:Show();
				else
					redTex:Hide();
				end
				local Item = container:getItem( curContainerType, ((i-1)*8+j-1) + (nContainerPage - 1)*nSUNDRIESGridMax );				
				if Item:getItemId() > 0 and Item:isValidEnd() then
					validTime:Show();
				else
					validTime:Hide();
				end
			end
		end
	end
	PlayerContainerFrameSetPackage();
end

local TALK_DIS = 250;
function  StashFrame_OnUpdate()
	
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	local container = MainPlayer:getContainer();
	if container == nil then return end
	local nMaxNum = container:getContainerMaxNum( CONTAINER_TYPE_STASH );
	local nInvalidNum = ContainerGetInvalidNum( CONTAINER_TYPE_STASH );
	local nInvalidIndex = nMaxNum - nInvalidNum;
	for i=1,10 do
		for j=1,10 do
			if ((i-1)*10+j) <= nMaxNum then				
				local redTex = getglobal( "StashFrameItem"..((i-1)*10+j).."RedTexture");
				local iconTex = getglobal( "StashFrameItem"..((i-1)*10+j).."IconTexture" );
				local validTime = getglobal( "StashFrameItem"..((i-1)*10+j).."ValidTimeTex" );
				if ((i-1)*10+j) > nInvalidIndex then
					redTex:Show();
				else
					redTex:Hide();
				end
				local Item = container:getItem( CONTAINER_TYPE_STASH, ((i-1)*10+j-1) + (nContainerPage - 1)*nTASKGridMax );
				if Item:getItemId() > 0 and Item:isValidEnd() then
					validTime:Show();
				else
					validTime:Hide();
				end
			end
		end
	end
	StashContainerFrameSetPackage();
end

--@Desc:【摆摊】
local t_ContainerFrameStallBtnEvents = 
{ 
	["UI_ACTOR_LEVEL_CHG"] = {},
};

t_ContainerFrameStallBtnEvents["UI_ACTOR_LEVEL_CHG"].func = 
function()
	UpdateContainerFrame_StallBtnState();
	SetTmpShortCutBtn( false );
end

function ContainerFrame_StallageBtn_OnLoad()
	for event, _ in pairs( t_ContainerFrameStallBtnEvents ) do
		this:RegisterEvent( event );
	end

	t_ContainerFrameStallBtnEvents.__index = function ( tab, key )
									tab[key] = { func = function () end };
									return tab[key];
								end
	setmetatable( t_ContainerFrameStallBtnEvents, t_ContainerFrameStallBtnEvents );
end

function ContainerFrame_StallageBtn_OnEvent()
	t_ContainerFrameStallBtnEvents[arg1].func();
end

function ContainerFrame_StallageBtn_OnClick()
	if TradeFrame:IsShown() then
		ShowMidTips( "正处于交易状态，无法进行摆摊操作" );
		return;
	end

	if OnlineStoreFrame:IsShown() then
		return ShowMidTips( "查看店铺中，无法进行摆摊操作" );
	end

	-- 若卖家的店铺已经是显示状态,则什么也不做
	if StallSaleFrame:IsShown() then
		return;
	end

	--若处于组队跟随状态则不能摆摊
	if TeamFollowHintFrame:IsShown() then
		ShowMidTips( "正处于组队跟随状态，无法进行摆摊操作" );
		return;
	end

	if not GameStallManager:canStartStall() then
		return;
	end

	if StashFrame:IsShown() then
		ShowMidTips( "正在进行银行操作，无法进行摆摊操作" );
		return;
	end

	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then 
		return 
	end

	local container = mainplayer:getContainer();
	local lockState = container:getContainerLockState();
	if lockState == CONTAINER_LOCK_STATE_LOCK then
		ShowMidTips("请先验证背包安全锁");
		return;
	end
	if not GameStallManager:isInStallArea() then
		ShowMidTips("时间城外无法摆摊");
		return;
	end

	-- 清空上次摆摊的总收入
	local t_StallSaleFrameControl = GetStallSaleFrameControl();
	t_StallSaleFrameControl:clearTotalIncome();
	--清空上次摆摊的交易记录	
	StallSaleFrame_SaleHistoryBtnRich:Clear();
	StallSaleFrame:Show();
	
	StallSaleFrame:SetPoint("TOPLEFT", "$parent", "TOPLEFT", 185, 140 );
	PlayerContainerFrame:SetPoint("TOPLEFT", "$parent", "TOPLEFT",StallSaleFrame:GetWidth() + 185, 140 );
end

--@Desc:【拆分】
function ContainerFrame_SplitBtn_OnClick()
	if InputFrame:IsShown() then
		ShowMidTips( "背包中有物品被拆分，无法再次拆分" );
		return;
	end
	SplitCurFrameItem( "PlayerContainerFrame",  GetCurContainerType( "PlayerContainerFrame" ),nSUNDRIESGridMax );
end

--@Desc:拆分当前背包类型下的物品
function SplitCurFrameItem( szFrameName, szFrameType,maxNum )
	local MainPlayer = ActorMgr:getMainPlayer();
	local container = MainPlayer:getContainer();
	local bFound = false;
	for i = 1, maxNum do
		local checkBtn = getglobal( szFrameName .. "Item" .. i );
		if checkBtn:IsChecked() then
			bFound = true;
			if IsContainerItemLock( szFrameType, i-1 ) then
				ShowMidTips( "该物品已被锁定，无法拆分" );
				return;
			end
			local redTex = getglobal("PlayerContainerFrameItem"..i.."RedTexture");
			if redTex:IsShown() then
				ShowMidTips("选中物品栏已失效，无法拆分");
				return;
			end

			local item = container:getItem( szFrameType, checkBtn:GetClientID()-1 );
			local nonegrid = container:getNoneGridIndex( szFrameType );
			if item:getNum() > 1 then
				SplitItem( {
							szFrameName = szFrameName, nMinNum = 0, nMaxNum = item:getNum(),
							desGrid = nonegrid, srcList = szFrameType, srcGrid = checkBtn:GetClientID()-1
							} );
			else
				return;
			end
		end
	end

	if not bFound then
		return ShowMidTips("请先选中需要拆分的物品");
	end
end

--@Desc:【整理】
function  StashFrame_TidyBtn_OnClick()
	--if bDown  then
	--	ShowMidTips( "操作受限,仓库物品5秒只能整理一次" );
	--	return;
	--end

	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	local container = MainPlayer:getContainer();
	if container == nil then return end

	if ItemSplitInputFrame:IsShown() then
		ItemSplitInputFrame:Hide();
	end

	container:send_PackageArrange(CONTAINER_TYPE_STASH);

--[[
	ShowMidTips( "仓库物品已整理");
--]]
	bDown = true;
	bBeginRecord = true;
	RemoveCheckItem( "PlayerContainerFrame" );
end

--拆分
function StashFrame_SplitBtn_OnClick()
	if InputFrame:IsShown() then
		ShowMidTips( "背包中有物品被拆分，无法再次拆分" );
		return;
	end
	SplitCurFrameItem( "StashFrame",  GetCurContainerType( "StashFrame" ),nStashGridMax );

end

-- 取款
function StashContainerFrame_GetMoneyBtn_OnClick()
	ContainerSaveGetMoneyFrameTitle:SetText("取 款");
	ContainerSaveGetMoneyFrame:SetClientUserData(0,1);
	ContainerSaveGetMoneyFrame:Show();
end

--@Desc:仓库管理面板银币( MONEY_TYPE_MONEY_NAME)的【存】按钮
function StashContainerFrame_SaveMoneyBtn_OnClick()
	local MainPlayer = ActorMgr:getMainPlayer();
	local levelupDef = getLevelUP( MainPlayer:getLv() );
	if MainPlayer:getStashMoney() == levelupDef.MaxStashMoney then
		ShowMidTips("存不了更多的银币了");
		return
	end
	ContainerSaveGetMoneyFrameTitle:SetText("存 款");
	ContainerSaveGetMoneyFrame:SetClientUserData(0,2);
	ContainerSaveGetMoneyFrame:Show();	
end

function PlayerContainerFrameEffectFrame_OnUpdate()
	if not bSetGuidLastTime then
		nGuidLastTime = os.clock();
		bSetGuidLastTime = true;
	end
	if os.clock() - nGuidLastTime > 4 and bSetGuidLastTime then
		PlayerContainerFrameEffectFrameGuide:Hide();
		PlayerContainerFrameEffectFrame_Texture:Hide();
	end
end
function PlayerContainerFrameEffectFrame_OnHide()
	if bSetGuidLastTime then
		bSetGuidLastTime = false;
	end
end

function CanSaveToLeavePile( container, nContainerType, nItemId, nItemNum,itemBind )
	local nMaxNum	= container:getContainerMaxNum( nContainerType );
	local nCount	= 0;
	for i = 1, nMaxNum do
		local Item = container:getItem( nContainerType, i-1 );
		if Item:getItemId() == 0 then
			return true;
		end

		if( Item:getItemId() > 0 ) and Item:getItemId() == nItemId and Item:isItemBind() == itemBind then	
			local itemDef = Item:getItemDef();
		    nCount = nCount + itemDef.MaxPile - Item:getNum(); 
		end
	end
	
	if nCount >= nItemNum then
		return true;
	end

	return false;
end

function Save2Stash( item, nSrcList, nSrcGridIndex )
	local Mainplayer	= ActorMgr:getMainPlayer();
	if Mainplayer == nil then
		return;
	end

	local container		= Mainplayer:getContainer();
	if container == nil then
		return;
	end

	if not PlayerContainerFrame:IsShown() then
		return;
	end

	if item:getListType() == CONTAINER_TYPE_TASK then
		ShowMidTips("任务物品，不可存入仓库");
		return;
	end
	
	if CheckItemResult( item:getItemDef(), RESULT_LOST_TOTEM ) and item:getLostTotemState() == 1 then
		ShowMidTips( "失落图腾处于激活状态，不可存入仓库" )
		return
	end

	if item:getItemId() ~= 0 then
		local nItemNum		= item:getNum();
		local nMoveItemNum	= nItemNum;	  -- 需要移到背包的数量
		local nLastIndex	= 0; 
		local nFiniMoveNum  = 0;		  -- 已经移到背包的数量
		if not CanSaveToLeavePile( container, CONTAINER_TYPE_STASH, item:getItemId(), nItemNum, item:isItemBind() ) then
			ShowMidTips("仓库已满，不可存入");
			return;
		end

		while nItemNum > 0 do
			local itemId = item:getItemId();
			if CheckItemResult( item:getItemDef(), RESULT_GEM ) then
				if item:getItemInstVal1() > 0 or item:getItemInstVal2() > 0 then
					itemId = 0;
				end
			end
			local nonegrid = container:getNoneGridIndex( CONTAINER_TYPE_STASH, itemId, nLastIndex, item:isItemBind() );
			local ItemDef = item:getItemDef();
			if nonegrid < 0 then
				break;
			end
			local ContainerItem = container:getItem( CONTAINER_TYPE_STASH, nonegrid);
			if ContainerItem:getItemId() ~= 0 then
				nItemNum	 = ItemDef.MaxPile - ContainerItem:getNum();
				if item:getNum() - nFiniMoveNum < nItemNum then
					nMoveItemNum = item:getNum() - nFiniMoveNum
				else 
					nMoveItemNum = nItemNum;
				end
				nFiniMoveNum = nFiniMoveNum + nItemNum;	
			else
				nItemNum = 0;
				nMoveItemNum = item:getNum() - nFiniMoveNum;
			end

			if nMoveItemNum == 0 then
				nItemNum = 0;
				break;
			end
			container:send_MoveItem( nSrcList, CONTAINER_TYPE_STASH, nSrcGridIndex, nMoveItemNum, nonegrid );		
			nLastIndex = nonegrid + 1;
		end
	end
end

function GetItemContainerType( nItemId )
	local Mainplayer	= ActorMgr:getMainPlayer();
	local container		= Mainplayer:getContainer();
	if container:canDragIn( CONTAINER_TYPE_SUNDRIES, nItemId) then
		return CONTAINER_TYPE_SUNDRIES;
	elseif container:canDragIn( CONTAINER_TYPE_TASK, nItemId) then
		return CONTAINER_TYPE_TASK;
	elseif container:canDragIn( CONTAINER_TYPE_STUFF, nItemId) then
		return CONTAINER_TYPE_STUFF; 
	end

	return -1;
end

function Save2Container( item, nSrcList, nSrcGridIndex )
	local Mainplayer	= ActorMgr:getMainPlayer();
	if Mainplayer == nil then
		return;
	end

	local container		= Mainplayer:getContainer();
	if container == nil then
		return;
	end

	local ContainerType = GetItemContainerType( item:getItemId() );
	if item:getItemId() ~= 0 and ContainerType ~= -1 then
		local nItemNum		= item:getNum();
		local nMoveItemNum	= nItemNum;	  -- 需要移到背包的数量
		local nLastIndex	= 0; 
		local nFiniMoveNum  = 0;		  -- 已经移到背包的数量	  
	
		if not CanSaveToLeavePile( container, GetItemContainerType( item:getItemId() ), item:getItemId(), nItemNum, item:isItemBind() ) then
			ShowMidTips("背包已满,不可存入!");
			return;
		end
		while nItemNum > 0 do
			local itemId = item:getItemId();
			if CheckItemResult( item:getItemDef(), RESULT_GEM ) then
				if item:getItemInstVal1() > 0 or item:getItemInstVal2() > 0 then
					itemId = 0;
				end
			end
			local nonegrid = container:getNoneGridIndex(GetItemContainerType( item:getItemId() ), itemId, nLastIndex, item:isItemBind() );
			local ItemDef = item:getItemDef();
			if nonegrid < 0 then
				break;
			end
			local ContainerItem = container:getItem( ContainerType, nonegrid);
			if ContainerItem:getItemId() ~= 0 then
				nItemNum	 = ItemDef.MaxPile - ContainerItem:getNum();
				if item:getNum() - nFiniMoveNum < nItemNum then
					nMoveItemNum = item:getNum() - nFiniMoveNum
				else 
					nMoveItemNum = nItemNum;
				end
				nFiniMoveNum = nFiniMoveNum + nItemNum;	
			else
				nItemNum = 0;
				nMoveItemNum = item:getNum() - nFiniMoveNum;
			end

			if nMoveItemNum == 0 then
				nItemNum = 0;
				break;
			end

			container:send_MoveItem( nSrcList, ContainerType, nSrcGridIndex, nMoveItemNum, nonegrid );	
			nLastIndex = nonegrid + 1;
		end
	end
end

function OpenStashFrame( nActorId )

	if TradeFrame:IsShown() then
		ShowMidTips("处于交易状态不能进行仓储操作");
		return;
	end

	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end

	if mainplayer:isInStall() then
		ShowMidTips("处于摆摊状态不能进行仓储操作");
		return;
	end

	local frame = getglobal( "StashFrame" );
	frame:Show();
	if not PlayerContainerFrame:IsShown() then
		PlayerContainerFrame:Show();
		if nActorId ~= nil then
			util.testNpcDialogDistance(nActorId, PlayerContainerFrame)
		end
	end
	frame:SetPoint("TOPLEFT", "$parent", "TOPLEFT", 78, 140 );
	PlayerContainerFrame:SetPoint("TOPLEFT", "StashFrame", "TOPRIGHT", 0, 0 );
	if nActorId ~= nil then
		util.testNpcDialogDistance(nActorId, StashFrame)
	end
end

function ContainerFrame_OnEnter()
	SetGameTooltips(this:GetName(), "到达20级以上的玩家，可以在时间城交易广场摆摊" );
end

--右键装备背包
function ContainerChecAddkBagItem( used_Item )
	local itemDef = used_Item:getItemDef();
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	if used_Item:isValidEnd() then
		return false;
	end
	if used_Item:getListType() == CONTAINER_TYPE_EXTEND or used_Item:getListType() == CONTAINER_TYPE_EXTEND_STASH then
		return true;
	end
	if used_Item:getListType() ~= CONTAINER_TYPE_SUNDRIES then
		return false;
	end

	if StallSaleFrame:IsShown() then	
		return false;
	end

	local nMaxNum = container:getContainerMaxNum( CONTAINER_TYPE_EXTEND );
	local stuffNum = container:getStuffNum( CONTAINER_TYPE_EXTEND );
	local bProcessed = false;
	for i=1,MAX_ITEM_RESULT do
		if itemDef.Result[i-1].ResultID == RESULT_ADD_SUNDPKG_GRID then
			bProcessed = nil;
			for j=1,(nMaxNum-stuffNum) do
				local Item = container:getItem( CONTAINER_TYPE_EXTEND, j-1 );
				if Item:getItemId() == 0 then
					container:send_MoveItem( used_Item:getListType(), CONTAINER_TYPE_EXTEND, used_Item:getGridIdx(), used_Item:getNum(), j-1 );
					bProcessed = true;
					break;
				end
			end
			if StashFrame:IsShown() and not bProcessed then
				local nMaxNum = container:getContainerMaxNum( CONTAINER_TYPE_EXTEND_STASH );
				for j=1,nMaxNum do
					local Item = container:getItem( CONTAINER_TYPE_EXTEND_STASH,j-1 );
					if Item:getItemId() == 0 then
						container:send_MoveItem( used_Item:getListType(), CONTAINER_TYPE_EXTEND_STASH, used_Item:getGridIdx(), used_Item:getNum(), j-1 );
						bProcessed = true;
						break;
					end
				end
			end			
		elseif itemDef.Result[i-1].ResultID == RESULT_ADD_STUFFPKG_GRID then
			bProcessed = nil;
			if stuffNum > 0 then
				local Item = container:getItem( CONTAINER_TYPE_EXTEND, nMaxNum-1 );
				if Item:getItemId() == 0 then
					container:send_MoveItem( used_Item:getListType(), CONTAINER_TYPE_EXTEND, used_Item:getGridIdx(), used_Item:getNum(),nMaxNum-1 );
					bProcessed = true;
				end
			end			
		end
	end
	return bProcessed;
	    --RESULT_ADD_SUNDPKG_GRID = 256, 	/* 增加杂物背包格子  val1表示加多少  */
		--RESULT_ADD_STUFFPKG_GRID = 258, 	/* 增加材料背包格子  val1表示加多少  */
end

--按钮Tips 提示
function ContainerFrame_SplitBtn_OnEnter()
	SetGameTooltips( this:GetName(),"把多个叠放在一起的物品拆成多份" );
end
function StashFrame_SplitBtn_OnEnter()
	SetGameTooltips( this:GetName(),"把多个叠放在一起的物品拆成多份" );
end
function ContainerFrame_TidyBtn_OnEnter()
	SetGameTooltips( this:GetName(),"自动整理物品\n同种类物品会自动叠加" );
end

local t_LockTips = {
					[CONTAINER_LOCK_STATE_NO]  = [[

为了您的账号安全，请设置安全锁密码（仓库管理员处可取消安全锁）

#G设置安全锁步骤：
    1、点击安全锁按键
    2、设定安全锁密码
    3、点击确定#n

#Y安全锁图标变成绿色，即生效#n

]],
					[CONTAINER_LOCK_STATE_LOCK] = [[
#cffffbe为了您的账号安全，请经常更换安全锁密码

#G安全锁开锁步骤：
    1、点击背包内安全锁按键
    2、输入安全锁密码
    3、点击确定#n

#Y您的账号正在受到安全锁的保护：
    背包：无法将物品丢弃
    仓库：无法从仓库中取出物品
	公会：你在公会中的个人权限操作不可使用，包括“退出公会”等#n
	（仓库管理员处可取消安全锁）]],
					[CONTAINER_LOCK_STATE_OPEN] = [[

#Y您已经设置了安全锁密码，请您牢记

#G每次重新登录后，自动启动安全锁#n
（仓库管理员处可取消安全锁）
锁定安全锁步骤：
    1、点击安全锁按键锁上安全锁

]]
				}
function ContainerFrameLockBtn_OnEnter()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local lockState = container:getContainerLockState();
	SetGameTooltips( this:GetName(),t_LockTips[lockState] );	
end

---背包锁相关
local t_LockUV = { 
					[CONTAINER_LOCK_STATE_NO]	= { ["normal"]={818,103,30,20},["high"]={818,124,30,20},["push"]={818,145,30,20} },	--灰色
					[CONTAINER_LOCK_STATE_LOCK] = { ["normal"]={752,103,30,20},["high"]={752,124,30,20},["push"]={752,145,30,20} },	--红色
					[CONTAINER_LOCK_STATE_OPEN] = { ["normal"]={785,103,30,20},["high"]={785,124,30,20},["push"]={785,145,30,20} },	--绿色
				}

local t_ShortcutLockTexture = {
									[CONTAINER_LOCK_STATE_NO] = "uires\\ui\\TeXiao\\FangDaoJiZhi-Hui.tga",
									[CONTAINER_LOCK_STATE_LOCK] = "uires\\ui\\TeXiao\\FangDaoJiZhi-Hong.tga",
									[CONTAINER_LOCK_STATE_OPEN] = "uires\\ui\\TeXiao\\FangDaoJiZhi-Hong.tga"
								}
function UpdateContainerLock( szBtnName )
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local lockState = container:getContainerLockState();
	local btn = getglobal( szBtnName );
	btn:ChangeNormalTexture( t_LockUV[lockState]["normal"][1],t_LockUV[lockState]["normal"][2],t_LockUV[lockState]["normal"][3],t_LockUV[lockState]["normal"][4] );
	btn:ChangeHighlightTexture( t_LockUV[lockState]["high"][1],t_LockUV[lockState]["high"][2],t_LockUV[lockState]["high"][3],t_LockUV[lockState]["high"][4] );
	btn:ChangePushedTexture( t_LockUV[lockState]["push"][1],t_LockUV[lockState]["push"][2],t_LockUV[lockState]["push"][3],t_LockUV[lockState]["push"][4] );
	if not bHaveShowLockEffect and lockState ~= CONTAINER_LOCK_STATE_OPEN then
		ContainerButtonLockAnimationTex:SetTexture(t_ShortcutLockTexture[lockState]);
		ContainerButtonLockAnimationTex:SetUVAnimation( 80,true );
		ClanButtonLockAnimationTex:SetTexture(t_ShortcutLockTexture[lockState]);
		ClanButtonLockAnimationTex:SetUVAnimation( 80,true );
		bHaveShowLockEffect = true;
	end
	local EffectTex = getglobal( szBtnName .. "EffectTex" )
	if EffectTex ~= nil and EffectTex:IsShown() then
		UpdateContainerLockEffect( szBtnName );
	end
end

function PlayerContainerFrameLockBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	--[[
	if mainplayer:getLv() < 20 then
		ShowMidTips("该功能20级后才能使用");
		return
	end
	]]--
	local container = mainplayer:getContainer();
	local lockState = container:getContainerLockState();

	if lockState == CONTAINER_LOCK_STATE_NO then
		--显示设置密码框
		ContainerSetLockFrame:Show();
	elseif lockState == CONTAINER_LOCK_STATE_LOCK then
		ContainerOpenLockFrame:Show();
	elseif lockState == CONTAINER_LOCK_STATE_OPEN then
		container:requestLockContainer();
	end
	local effectTex = getglobal( this:GetName() .. "EffectTex" )
	if effectTex ~= nil and effectTex:IsShown() then
		effectTex:Hide();
		this:SetNormalTexture("uires\\ZhuJieMian\\1.dds","blend");
		this:SetHightlightTexture("uires\\ZhuJieMian\\1.dds","blend");
		this:SetPushedTexture("uires\\ZhuJieMian\\1.dds","blend");
	end
end

function ContainerSetLockFrame_OnShow()
	local edit = getglobal( "ContainerSetLockFramePwdEditbox" );
	edit:SetText("");
	edit = getglobal("ContainerSetLockFrameComfirmPwdEditbox");
	edit:SetText("");
	SetFocusFrame("ContainerSetLockFramePwdEditbox");
	this:SetPoint("center","$parent","center",0,0);
end

function ContainerSetLockFrame_OnHide()
	ClearFocus();
end

function ContainerSetLockFrameAcceptBtnOnClick()
	local szPwd = ContainerSetLockFramePwdEditbox:GetPassWord();
	local szComfirmPwd = ContainerSetLockFrameComfirmPwdEditbox:GetPassWord();
	if szPwd == "" then
		AddGameMiddleTips2("请输入密码");
		return;
	end
	if szPwd ~= szComfirmPwd then
		AddGameMiddleTips2("两次输入的密码不匹配");
		return;
	end
	if string.len( szPwd ) ~= 6 then
		AddGameMiddleTips2("请输入六位密码，密码由数字和英文字母组成");
		return;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	container:requestSetContainerLock( szPwd );
	ContainerSetLockFrame:Hide();
	ClearFocus();
end

function ContainerOpenLockFrame_OnShow()
	ContainerOpenLockFramePwdEditbox:SetText("");
	SetFocusFrame("ContainerOpenLockFramePwdEditbox");
	--this:SetPoint("center","$parent","center",0,0);
end

function ContainerOpenLockFrame_OnHide()
		ClearFocus();
end

function ContainerOpenLockFrameAcceptBtnOnClick()
	local szPwd = ContainerOpenLockFramePwdEditbox:GetPassWord();
	if szPwd == "" then
		AddGameMiddleTips2("请输入密码");
		return;
	end
	if string.len( szPwd ) ~= 6 then
		AddGameMiddleTips2("请输入六位密码，密码由数字和英文字母组成");
		return;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	container:requestCheckContainerPsd( szPwd );
	ContainerOpenLockFrame:Hide();
	ClearFocus();
end

function PlayerContainerFrameLeftPageBtn_OnClick()
	if nContainerPage > 1 then
		nContainerPage = nContainerPage - 1;
		ContainerFrameOnShow();
		HideContainerEffectFrame();
		ShowContainerGuide();
	end
end

function PlayerContainerFrameRightPageBtn_OnClick()
	if nContainerPage < nContainerMaxPage  then
		nContainerPage = nContainerPage + 1;
		ContainerFrameOnShow();
		HideContainerEffectFrame();
		ShowContainerGuide();
	end
end

function PlayerContainerFrameToLastPageBtn_OnClick()
	if nContainerPage < nContainerMaxPage then
		nContainerPage = nContainerMaxPage;
		ContainerFrameOnShow();
		HideContainerEffectFrame();
		ShowContainerGuide();
	end
end

function PlayerContainerFrameToFirstPageBtn_OnClick()
	if nContainerPage > 1 then
		nContainerPage = 1;
		ContainerFrameOnShow();
		HideContainerEffectFrame();
		ShowContainerGuide();
	end
end

function PlayerContainerFrameShowPageButton( bShow )
	if bShow then
		PlayerContainerFrameLeftPageBtn:Show();
		PlayerContainerFrameRightPageBtn:Show();
	--	PlayerContainerFrameToLastPageBtn:Show();
	--	PlayerContainerFrameToFirstPageBtn:Show();
	else
		PlayerContainerFrameLeftPageBtn:Hide();
		PlayerContainerFrameRightPageBtn:Hide();
	--	PlayerContainerFrameToLastPageBtn:Hide();
	--	PlayerContainerFrameToFirstPageBtn:Hide();
	end
end

function PlayerContainerFrameSetPackage()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	local container = MainPlayer:getContainer();
	if container == nil then return end
	local nMaxNum = container:getContainerMaxNum( CONTAINER_TYPE_EXTEND );
	local stuffNum = container:getStuffNum( CONTAINER_TYPE_EXTEND );
	nMaxNum = nMaxNum - stuffNum;
	for i=1,MAX_PACKAGE_NUM do		
		local bagButton = getglobal( "PlayerContainerFrameBagItem"..i);
		local ItemNum   = getglobal( "PlayerContainerFrameBagItem"..i.."CountFont");
		local ItemBackTex = getglobal( "PlayerContainerFrameBagItem"..i.."BackTexture");
		local ItemBoxTex = getglobal( "PlayerContainerFrameBagItem"..i.."BoxTexture");
		local UVAnimationTex = getglobal( "PlayerContainerFrameBagItem"..i.."UVAnimationTex");
		local ItemTex = getglobal( "PlayerContainerFrameBagItem"..i.."IconTexture");
		local BagBackTex = getglobal("PlayerContainerFrameBagItem"..i.."BagBackTexture");
		local validTime = getglobal( "PlayerContainerFrameBagItem"..i.."ValidTimeTex" );
		if i == 7 then
			BagBackTex:SetTexUV( 935,474,35,35 );
		end
		BagBackTex:Show();
		bagButton:SetNormalTexture( "uires\\ZhuJieMian\\3.dds", "blend" );
		bagButton:SetPushedTexture( "uires\\ZhuJieMian\\3.dds", "blend" );
		bagButton:SetHightlightTexture( "uires\\ZhuJieMian\\3.dds", "blend" );
		bagButton:ChangeNormalTexture( 127,772,44,44 );
		bagButton:ChangeHighlightTexture( 171,772,44,44 );
		bagButton:ChangePushedTexture( 126,817,44,44 );
		bagButton:SetClientID(i);
		ItemBackTex:Show();
		bagButton:DisChecked();
		ItemBoxTex:Hide();
		validTime:Hide();
		UVAnimationTex:Hide();
		if i <= nMaxNum then 
			BagBackTex:Hide();
			bagButton:SetNormalTexture( "uires\\ui\\mask\\TouMingTongDao.tga", "blend" );
			bagButton:SetPushedTexture( "uires\\ZhuJieMian\\1.dds", "blend" );
			bagButton:SetHightlightTexture( "uires\\ZhuJieMian\\1.dds", "blend" );
			bagButton:ChangeHighlightTexture( 794,697,44,44 );
			bagButton:ChangePushedTexture( 794,607,44,44 );
			--bagButton:SetClientID( i );
			local Item = container:getItem( CONTAINER_TYPE_EXTEND, i-1 );
			if( Item:getItemId() > 0 ) then
				bagButton:SetClientUserData( 2, Item:getNum() );
				local itemdef = Item:getItemDef();
				local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
				-- 如果没找到物品的图标则使用默认图标显示
				if not IsInExistence( IconPath ) then
					IconPath = DEFAULT_ITEM_PATH;
				end
				ItemTex:SetTexture( IconPath );			
				
				SetEquipImportantEffectUV( itemdef, UVAnimationTex:GetName(),Item);
				ItemBoxTex:Show();
				if Item:isValidEnd() then
					validTime:Show();
				else
					validTime:Hide();
				end
			else
				ItemTex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
			end
		else
			ItemTex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
		end
	end
	--[[
	if stuffNum > 0 then
		local bagButton = getglobal( "PlayerContainerFrameBagItem7");
		local ItemTex = getglobal( "PlayerContainerFrameBagItem7IconTexture");
		local UVAnimationTex = getglobal( "PlayerContainerFrameBagItem7UVAnimationTex");
		local ItemBoxTex = getglobal( "PlayerContainerFrameBagItem7BoxTexture");
		local BagBackTex = getglobal("PlayerContainerFrameBagItem7BagBackTexture");
		BagBackTex:Hide();
		bagButton:Enable();
		bagButton:SetClientID( container:getContainerMaxNum( CONTAINER_TYPE_EXTEND ) );
		--最后一个栏位是材料扩充栏
		local Item = container:getItem( CONTAINER_TYPE_EXTEND, nMaxNum );
		if( Item:getItemId() > 0 ) then
			bagButton:SetClientUserData( 2, Item:getNum() );
			local itemdef = Item:getItemDef();
			local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
			-- 如果没找到物品的图标则使用默认图标显示
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			ItemTex:SetTexture( IconPath );			
			SetEquipImportantEffectUV( itemdef, UVAnimationTex:GetName(),Item);
			ItemBoxTex:Show();
		else
			ItemTex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
			UVAnimationTex:Hide();
			ItemBoxTex:Hide();
		end
	end
	]]--
end

function StashContainerFrameSetPackage()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	local container = MainPlayer:getContainer();
	if container == nil then return end
	
	for i=1,4 do		
		local bagButton = getglobal( "StashFrameBagItem"..i);
		local ItemNum   = getglobal( "StashFrameBagItem"..i.."CountFont");
		local ItemBackTex = getglobal( "StashFrameBagItem"..i.."BackTexture");
		local ItemBoxTex = getglobal( "StashFrameBagItem"..i.."BoxTexture");
		local UVAnimationTex = getglobal( "StashFrameBagItem"..i.."UVAnimationTex");
		local ItemTex = getglobal( "StashFrameBagItem"..i.."IconTexture");
		local BagBackTex = getglobal("StashFrameBagItem"..i.."BagBackTexture");
		--local redTex = getglobal( "StashFrameBagItem"..i.."RedTexture" );
		local validTime = getglobal( "StashFrameBagItem"..i.."ValidTimeTex" );
		BagBackTex:Show();
		--bagButton:Disable();
		bagButton:SetNormalTexture( "uires\\ZhuJieMian\\3.dds", "blend" );
		bagButton:SetPushedTexture( "uires\\ZhuJieMian\\3.dds", "blend" );
		bagButton:SetHightlightTexture( "uires\\ZhuJieMian\\3.dds", "blend" );
		bagButton:ChangeNormalTexture( 127,772,44,44 );
		bagButton:ChangeHighlightTexture( 171,772,44,44 );
		bagButton:ChangePushedTexture( 126,817,44,44 );
		bagButton:SetClientID(i);

		bagButton:DisChecked();
		ItemBackTex:Show();
		ItemBoxTex:Hide();
		--redTex:Show();
		--redTex:SetTexUV( 749,652,44,44 );
		validTime:Hide();
		UVAnimationTex:Hide();

		local nMaxNum = container:getContainerMaxNum( CONTAINER_TYPE_EXTEND_STASH );
		if i <= nMaxNum then 
			BagBackTex:Hide();
			--bagButton:Enable();
			bagButton:SetNormalTexture( "uires\\ui\\mask\\TouMingTongDao.tga", "blend" );
			bagButton:SetPushedTexture( "uires\\ZhuJieMian\\1.dds", "blend" );
			bagButton:SetHightlightTexture( "uires\\ZhuJieMian\\1.dds", "blend" );
			bagButton:ChangeHighlightTexture( 794,697,44,44 );
			bagButton:ChangePushedTexture( 794,607,44,44 );
			local Item = container:getItem( CONTAINER_TYPE_EXTEND_STASH, i-1 );
			if( Item:getItemId() > 0 ) then				
				bagButton:SetClientUserData( 2, Item:getNum() );
				local itemdef = Item:getItemDef();
				local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
				-- 如果没找到物品的图标则使用默认图标显示
				if not IsInExistence( IconPath ) then
					IconPath = DEFAULT_ITEM_PATH;
				end
				ItemTex:SetTexture( IconPath );				
				SetEquipImportantEffectUV( itemdef, UVAnimationTex:GetName(),Item);
				ItemBoxTex:Show();
				if Item:isValidEnd() then
					validTime:Show();
				else
					validTime:Hide();
				end
			else
				ItemTex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
			end
			--redTex:Hide();
		else
			ItemTex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
		end
	end
end

function ContainerSaveGetMoneyFrame_OnUpdate()
	local nType = this:GetClientUserData( 0 );
	local MainPlayer = ActorMgr:getMainPlayer();
	local maxBull = 0;
	local maxMoney = 0;
	if nType == 1 then
		--maxBull = MainPlayer:getStashBull();
		maxMoney = MainPlayer:getStashMoney()
	elseif nType == 2 then
		--maxBull = MainPlayer:getBull( );
		maxMoney = MainPlayer:getMoney();
	end
	--[[
	local nBull = tonumber(  ContainerSaveGetMoneyFrameGetBullEditbox:GetText() );	
	if nBull == nil or nBull < 0 then
		ContainerSaveGetMoneyFrameGetBullEditbox:SetText( 0 );
	elseif nBull > maxBull then
		ContainerSaveGetMoneyFrameGetBullEditbox:SetText( maxBull );
	end--]]

	local nMoney = tonumber( ContainerSaveGetMoneyFrameGetMoneyEditbox:GetText() );
	if nMoney == nil or nMoney < 0 then
		ContainerSaveGetMoneyFrameGetMoneyEditbox:SetText( 0 );
	else
		if nType == 2 then
			local levelupDef = getLevelUP( MainPlayer:getLv() );
			if levelupDef ~= nil then
				local leftNum = levelupDef.MaxStashMoney - MainPlayer:getStashMoney();
				if nMoney > leftNum or nMoney > maxMoney then
					if leftNum < maxMoney then
						ContainerSaveGetMoneyFrameGetMoneyEditbox:SetText( leftNum > 0 and leftNum or 0 );
					else
						ContainerSaveGetMoneyFrameGetMoneyEditbox:SetText( maxMoney );
					end
				end
			end
		else
			if nMoney > maxMoney then
				ContainerSaveGetMoneyFrameGetMoneyEditbox:SetText( maxMoney );
			end
		end
	end

	CheckEditNumberInput( ContainerSaveGetMoneyFrameGetBullEditbox );
	CheckEditNumberInput( ContainerSaveGetMoneyFrameGetMoneyEditbox );
end

function ContainerSaveGetMoneyFrame_OnShow()
	ContainerSaveGetMoneyFrameGetMoneyEditbox:SetText("");
	ContainerSaveGetMoneyFrameGetBullEditbox:SetText("");
	SetFocusFrame("ContainerSaveGetMoneyFrameGetMoneyEditbox");
	this:SetPoint("center","$parent","center",0,0);
end

function ContainerSaveGetMoneyFrameAcceptBtnOnClick()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	local container = MainPlayer:getContainer();
	if container == nil then return end

	local nType = ContainerSaveGetMoneyFrame:GetClientUserData( 0 );
	local nBull = tonumber(ContainerSaveGetMoneyFrameGetBullEditbox:GetText());
	local nMoney = tonumber(ContainerSaveGetMoneyFrameGetMoneyEditbox:GetText());
	if (nBull == nil or nBull == 0) and (nMoney == nil or nMoney == 0) then
		ShowMidTips("请输入金额");
		return;
	end
	if nType == 1 then
		if nBull ~= nil and nBull ~= 0 then
			--container:send_MoveMoney( ATTR_ID_STASHBULL,ATTR_ID_BULL,nBull );
		end
		if nMoney ~= nil and nMoney ~= 0 then
			container:send_MoveMoney( ATTR_ID_STASHMONEY,ATTR_ID_MONEY,nMoney );
		end
	elseif nType == 2 then
		if nBull ~= nil and nBull ~= 0 then
			--container:send_MoveMoney( ATTR_ID_BULL,ATTR_ID_STASHBULL,nBull );
		end
		if nMoney ~= nil and nMoney ~= 0 then			
			container:send_MoveMoney( ATTR_ID_MONEY,ATTR_ID_STASHMONEY,nMoney );
		end
	end
	ContainerSaveGetMoneyFrame:Hide();
	ClearFocus();
end

function ContainerGetInvalidNum( nListType )
	local nNum = 0;
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	local container = MainPlayer:getContainer();
	if container == nil then return end

	if nListType == CONTAINER_TYPE_SUNDRIES then
		local nMaxNum = container:getContainerMaxNum( CONTAINER_TYPE_EXTEND );
		local nStuffNum = container:getStuffNum( CONTAINER_TYPE_EXTEND );
		nMaxNum = nMaxNum - nStuffNum;
		for i=1,nMaxNum do
			local item = container:getItem( CONTAINER_TYPE_EXTEND,i-1 );
			if item:getItemId() > 0 then
				local nValidEndTime = item:getItemValidEndTime();
				if nValidEndTime ~= 0 and nValidEndTime < GameClanManager:getServerTime() then
					local itemDef = item:getItemDef();
					for i=1,MAX_ITEM_RESULT do
						if itemDef.Result[i-1].ResultID == RESULT_ADD_SUNDPKG_GRID then
							nNum = nNum + itemDef.Result[i-1].ResultVal1;
						end
					end
				end
			end
		end
	elseif nListType == CONTAINER_TYPE_STUFF then
		local nMaxNum = container:getContainerMaxNum( CONTAINER_TYPE_EXTEND );
		local nStuffNum = container:getStuffNum( CONTAINER_TYPE_EXTEND );
		if nStuffNum > 0 then
			local item = container:getItem( CONTAINER_TYPE_EXTEND,nMaxNum-1 );
			if item:getItemId() > 0 then
				local nValidEndTime = item:getItemValidEndTime();
				if nValidEndTime ~= 0 and nValidEndTime < GameClanManager:getServerTime() then
					local itemDef = item:getItemDef();
					for i=1,MAX_ITEM_RESULT do
						if itemDef.Result[i-1].ResultID == RESULT_ADD_STUFFPKG_GRID then
							nNum = nNum + itemDef.Result[i-1].ResultVal1;
						end
					end
				end
			end
		end
	elseif nListType == CONTAINER_TYPE_STASH then
		local nMaxNum = container:getContainerMaxNum( CONTAINER_TYPE_EXTEND_STASH );
		for i=1,nMaxNum do
			local item = container:getItem( CONTAINER_TYPE_EXTEND_STASH,i-1 );
			if item:getItemId() > 0 then
				local nValidEndTime = item:getItemValidEndTime();
				if nValidEndTime ~= 0 and nValidEndTime < GameClanManager:getServerTime() then
					local itemDef = item:getItemDef();
					for i=1,MAX_ITEM_RESULT do
						if itemDef.Result[i-1].ResultID == RESULT_ADD_SUNDPKG_GRID then
							nNum = nNum + itemDef.Result[i-1].ResultVal1;
						end
					end
				end
			end
		end
	end
	return nNum;
end

function ContainerRefreshLockFrame()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local lockState = container:getContainerLockState();
	if lockState == CONTAINER_LOCK_STATE_LOCK then
		ContainerOpenLockFrame:Show();
	end
end

local t_ContainerLockBtnEffectTexture = {
											[CONTAINER_LOCK_STATE_NO] = "uires\\ui\\TeXiao\\BeiBaoAnQuanSuo-Hui.tga",
											[CONTAINER_LOCK_STATE_OPEN] = "uires\\ui\\TeXiao\\BeiBaoAnQuanSuo-Lv.tga",
											[CONTAINER_LOCK_STATE_LOCK] = "uires\\ui\\TeXiao\\BeiBaoAnQuanSuo-Hong.tga"
										}
function UpdateContainerLockEffect( szBtnName )
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local container = mainplayer:getContainer();
	local lockState = container:getContainerLockState();
	local btn = getglobal( szBtnName );
	local tex = getglobal( szBtnName .. "EffectTex" );
	tex:SetTexture( t_ContainerLockBtnEffectTexture[lockState] );
	tex:SetUVAnimation(80, true);
	btn:SetNormalTexture("uires\\ui\\mask\\TouMingTongDao.tga","blend");
	btn:SetHightlightTexture("uires\\ui\\mask\\TouMingTongDao.tga","blend");
	btn:SetPushedTexture("uires\\ui\\mask\\TouMingTongDao.tga","blend");
end

function ContainerModifyPwd( npcId )
	if not ContainerChangeLockFrame:IsShown() then
		ContainerChangeLockFrame:Show();
		util.testNpcDialogDistance( npcId,ContainerChangeLockFrame );
	end
end

function ContainerCancelPwd( npcId )
	if not ContainerCancelLockFrame:IsShown() then
		ContainerCancelLockFrame:Show();
		util.testNpcDialogDistance( npcId,ContainerCancelLockFrame );
	end
end

function ContainerCancelLockFrame_OnShow()
	local mainplayer = ActorMgr:getMainPlayer();
	local container = mainplayer:getContainer();
	if container:getContainerLockState() == CONTAINER_LOCK_STATE_NO then
		AddGameMiddleTips2("您还未设置安全锁");
		this:Hide();
	end
	local edit = getglobal( "ContainerCancelLockFramePwdEditbox" );
	edit:SetText("");
	edit = getglobal("ContainerCancelLockFrameComfirmPwdEditbox");
	edit:SetText("");
	SetFocusFrame("ContainerCancelLockFramePwdEditbox");
	this:SetPoint("center","$parent","center",0,0);
end

function ContainerCancelLockFrame_OnHide()
	ClearFocus();
end

function ContainerCancelLockFrameComfirmPwdEditbox_OnTabPressed()
	SetFocusFrame("ContainerCancelLockFramePwdEditbox");
	ContainerCancelLockFramePwdEditbox:SelectAllText();
end

function ContainerCancelLockFramePwdEditbox_OnTabPressed()
	SetFocusFrame("ContainerCancelLockFrameComfirmPwdEditbox");
	ContainerCancelLockFrameComfirmPwdEditbox:SelectAllText();
end

function ContainerCancelLockFrameAcceptBtnOnClick()
	local szPwd = ContainerCancelLockFramePwdEditbox:GetPassWord();
	local szComfirmPwd = ContainerCancelLockFrameComfirmPwdEditbox:GetPassWord();
	if szPwd == "" then
		AddGameMiddleTips2("请输入密码");
		return;
	end
	if szPwd ~= szComfirmPwd then
		AddGameMiddleTips2("两次输入的密码不匹配");
		return;
	end
	if string.len( szPwd ) ~= 6 then
		AddGameMiddleTips2("请输入六位密码，密码由数字和英文字母组成");
		return;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	container:requestCancelContainerLock( szPwd );
	ContainerCancelLockFrame:Hide();
end

function ContainerChangeLockFramePwdEditbox_OnTabPressed()
	SetFocusFrame("ContainerChangeLockFrameComfirmPwdEditbox");
	ContainerChangeLockFrameComfirmPwdEditbox:SelectAllText();
end

function ContainerChangeLockFrameComfirmPwdEditbox_OnTabPressed()
	SetFocusFrame("ContainerChangeLockFrameNewPwdEditbox");
	ContainerChangeLockFrameNewPwdEditbox:SelectAllText();
end

function ContainerChangeLockFrameNewEditbox_OnTabPressed()
	SetFocusFrame("ContainerChangeLockFramePwdEditbox");
	ContainerChangeLockFramePwdEditbox:SelectAllText();
end

function ContainerChangeLockFrameAcceptBtnOnClick()
	local szPwd = ContainerChangeLockFramePwdEditbox:GetPassWord();
	local szComfirmPwd = ContainerChangeLockFrameComfirmPwdEditbox:GetPassWord();
	local szNewPwd = ContainerChangeLockFrameNewPwdEditbox:GetPassWord();
	if szPwd == "" or szComfirmPwd == "" or szNewPwd == "" then
		AddGameMiddleTips2("请输入密码");
		return;
	end	
	if string.len( szNewPwd ) ~= 6 then
		AddGameMiddleTips2("请输入六位密码，密码由数字和英文字母组成");
		return;
	end
	if szNewPwd ~= szComfirmPwd then
		AddGameMiddleTips2("两次输入的密码不匹配");
		return;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	container:requestChangeContainerPsd( szPwd,szNewPwd );
	ContainerChangeLockFrame:Hide();
end

function ContainerChangeLockFrame_OnShow()
	local mainplayer = ActorMgr:getMainPlayer();
	local container = mainplayer:getContainer();
	if container:getContainerLockState() == CONTAINER_LOCK_STATE_NO then
		AddGameMiddleTips2("您还未设置安全锁");
		this:Hide();
	end
	local edit = getglobal( "ContainerChangeLockFramePwdEditbox" );
	edit:SetText("");
	edit = getglobal("ContainerChangeLockFrameComfirmPwdEditbox");
	edit:SetText("");
	edit = getglobal( "ContainerChangeLockFrameNewPwdEditbox" );
	edit:SetText("");
	SetFocusFrame("ContainerChangeLockFramePwdEditbox");
	this:SetPoint("center","$parent","center",0,0);
end

function ContainerChangeLockFrame_OnHide()
	ClearFocus();
end

function ContainerFrame_OnClick()
	if ContainerBuyFrame:IsShown() then
		ContainerBuyFrame:AddLevelRecursive();
	end
end

-------------------------------------------背包商城-------------------------------------------
local CONTAINER_MARKET_NUM = 8;
local MAX_SHOP_LIST_NUM = 10;
local buyItemIdx = 0;
local t_Items = {};
local t_Fashion = {};

local function DisCheckAllBuyItem()
	for i=1, MAX_SHOP_LIST_NUM do
		local btn = getglobal( "ContainerBuyFrameItem" .. i );
		btn:DisChecked();
	end
end

function ContainerFrameMarketBtn_OnClick()
	if not ContainerBuyFrame:IsShown() then
		ContainerBuyFrame:Show();
	else
		ContainerBuyFrame:Hide();
	end
end

function ContainerBuyFrame_OnLoad()
	for i=1, MAX_SHOP_LIST_NUM do
		local tex = getglobal( "ContainerBuyFrameItem" .. i .. "IconTexture" );
		if i < 3 then
			local IconPath = "uires\\TuPianLei\\WuPin\\" .. (i < 2 and 5050004 or 9900010) .. ".tga";
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH
			end
			tex:SetTexture( IconPath );
			tex = getglobal( "ContainerBuyFrameItem" .. i .. "UVAnimationTex" );
			tex:SetTexture( "uires\\ui\\TeXiao\\ZhuangBei  14.tga" )
			tex:Show();
		end
	end
end

local function GetContainerShopItem()
	t_Items = {};
	for i=1,MarketManager:getShopItemNum() do
		local shopDef = MarketManager:getShopItemByIndex( i-1 );
		if shopDef ~= nil and shopDef.BuyType == BUY_TYPE_TICKET and bitAnd( shopDef.BuyUIType,SHOPPING_BUY_UI_PACKAGE_TOLUA ) then
			table.insert(t_Items,shopDef.ShopID);
		end
	end

	while #t_Items > CONTAINER_MARKET_NUM do
		table.remove( t_Items, math.random( 1, #t_Items ) );
	end
end

local function CheckFashionTimeEnd()
	t_Fashion = {};
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local Eqiup = mainplayer:getEquip();
	local t_Pos = { EQUIP_POS_FASHION_BODY, EQUIP_POS_FASHION_HEAD, EQUIP_POS_FASHION_BACK, EQUIP_POS_FASHION_SHOES, 
		EQUIP_POS_FASHION_HAND, EQUIP_POS_FASHION_LEG, EQUIP_POS_FASHION_HAIR };
	local tmp = {};
	for i=1, #t_Pos do
		local nWearPos = GetFashionSuitPos( t_Pos[i] );
		if Eqiup:getItem(nWearPos):isValidEnd() then
			table.insert( tmp, { t_Pos[i], Eqiup:getItem(nWearPos):getItemId() } );
		end
	end
	if #tmp > 0 then
		for i=1, #tmp do
			local shopId = MarketManager:getShopIDByID(BUY_TYPE_TICKET, tmp[i][2])
			if shopId > 0 then
				table.insert( t_Fashion, { tmp[i][1], tmp[i][2], shopId } );
			end
		end
		
		return #t_Fashion > 0;
	end
	return false;
end

function ContainerBuyFrame_OnShow()
	local tex = getglobal( "ContainerBuyFrameItem2UVAnimationTex" );
	local bEnd = CheckFashionTimeEnd();
	ContainerBuyFrameItem2:SetClientUserData( 1, bEnd and 1 or 0 );
	if bEnd then
		tex:SetTexture( "uires\\ui\\TeXiao\\ZhuangBei  3.tga" );
		tex:SetUVAnimation( 50, true );
	else
		tex:StopUVAnim();
		tex:SetTexture( "uires\\ui\\TeXiao\\ZhuangBei  14.tga" );
		tex:Show();
	end

	GetContainerShopItem();
	for i=1, #t_Items do
		if i + 2 > MAX_SHOP_LIST_NUM then
			break;
		end
		local nShopID = t_Items[i];
		local itemData = MarketManager:getItemByID( nShopID );
		local itemDef = getItemDef( itemData.Items[0].ItemID );
		
		if itemDef.ItemID > 0 then
			local btn = getglobal( "ContainerBuyFrameItem" .. i+2 );
			btn:SetClientUserData( 1, nShopID );
			local tex = getglobal( "ContainerBuyFrameItem" .. i+2 .. "IconTexture" );
			local IconPath
			if itemData.IconID ~= 0 then
				IconPath = GetItemEquipIconPath() .. itemData.IconID .. ".tga";
			else
				IconPath = GetItemEquipIconPath() .. itemDef.IconID .. ".tga";
			end
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH
			end
			tex:SetTexture( IconPath );
			SetEquipImportantEffectUV( itemDef, "ContainerBuyFrameItem" .. i+2 .. "UVAnimationTex" );
			local font = getglobal( "ContainerBuyFrameItem" .. i+2 .. "CountFont" );
			font:SetText( itemData.Items[0].ItemNum > 1 and itemData.Items[0].ItemNum or "" );
		end
	end

	for i = #t_Items + 1, CONTAINER_MARKET_NUM do
		if i + 2 > MAX_SHOP_LIST_NUM then
			break;
		end
		local btn = getglobal( "ContainerBuyFrameItem" .. i+2 );
		btn:SetClientUserData( 1, 0 );
		local tex = getglobal( "ContainerBuyFrameItem" .. i+2 .. "IconTexture" );
		tex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
		tex = getglobal( "ContainerBuyFrameItem" .. i+2 .. "UVAnimationTex" );
		tex:Hide();
		local font = getglobal( "ContainerBuyFrameItem" .. i+2 .. "CountFont" );
		font:SetText("");
	end

	PlayerContainerFrameMarketBtnUVAnimationTex:Hide();
end

function ContainerBuyFrame_OnHide()
	PlayerContainerFrameMarketBtnUVAnimationTex:SetUVAnimation( 50, true );
	PlayerContainerFrameMarketBtnUVAnimationTex:Show();
	DisCheckAllBuyItem();
	buyItemIdx = 0;
end

function ContainerBuyFrameItemOnClick( btn )
	DisCheckAllBuyItem();
	btn:Checked();
	buyItemIdx = btn:GetClientID();
end

function ContainerBuyFrameItemOnEnter( btn )
	if btn:GetClientID() == 1 then
		SetGameTooltips( btn:GetName(), "选中此项点击购买将打开VIP界面" );
	elseif btn:GetClientID() == 2 then
		SetGameTooltips( btn:GetName(), btn:GetClientUserData(1) > 0 and "存在已到期时装，选中后点击购买可快捷购买已到期时装" or "选中后点击购买将打开商城时装界面" );
	else
		if btn:GetClientUserData(1) > 0 then
			MarketManagerItemOnEnter( btn );
		end
	end
end

function ContainerBuyFrameBuyBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local lockState = container:getContainerLockState();
	if lockState == CONTAINER_LOCK_STATE_LOCK then
		return ShowMidTips("请先验证背包安全锁");
	end

	if buyItemIdx == 0 then
		ShowMidTips("请先点击需要购买的物品");
	elseif buyItemIdx == 1 then
		if not VipFrame:IsShown() then
			VipFrame:Show();
		end
	elseif buyItemIdx == 2 then
		if not MarketManagerFrame:IsShown() then
			MarketManagerFrame:Show();
		end
		MarketShowTargetPage( 3, 1, BUY_TYPE_TICKET );
		if #t_Fashion > 0 then
			for i=1, #t_Fashion do
				SetFashionPreview( t_Fashion[i][1], t_Fashion[i][2], t_Fashion[i][3] );
			end
			FashionPreviewBtnBuyAllBtnUVAnimationTex:SetUVAnimation( 50, true );
			FashionPreviewBtnBuyAllBtnUVAnimationTex:Show();
		end
		FashionPreviewUpdateModel();
	else
		local btn = getglobal( "ContainerBuyFrameItem" .. buyItemIdx );
		local nShopID = btn:GetClientUserData(1);
		if nShopID > 0 then
			local itemData = MarketManager:getItemByID( nShopID )
			MarketBuyItemFrameBtn:SetClientUserData( 3, itemData.BuyType )
			MarketManagerFrame_ShowBuyFrame(nShopID)
			UpdateMarketBuyItemFrameMoneySymbol( BUY_TYPE_TICKET );
			FashionPreviewBuyFrame:Hide();
		else
			ShowMidTips("请先点击需要购买的物品");
		end
	end
end

--[[
function ContainerMarketFrame_OnLoad()
	this:setUpdateTime(0);
	this:RegisterEvent("GE_UPDATE_ITEM_LIST");
	this:RegisterEvent("UI_ACTOR_ATTRIBUTE_CHG");
	this:RegisterEvent("GE_MARKET_LIMIT_CHG");
end

function ContainerMarketFrame_OnEvent()
	if arg1 == "GE_UPDATE_ITEM_LIST" or arg1 == "GE_MARKET_LIMIT_CHG" then		
		if ContainerMarketFrame:IsShown() then	
			ContainerMarketFrameSetPage();
			ContainerMarketFrameUpdateItem();
		end
	elseif arg1 == "UI_ACTOR_ATTRIBUTE_CHG" then
		if ContainerMarketFrame:IsShown() then
			ContainerMarketFrame_UpdateMoney();
		end
	end
end

function ContainerMarketFrameSetPage()
	t_Items = {};
	for i=1,MarketManager:getShopItemNum() do
		local shopDef = MarketManager:getShopItemByIndex( i-1 );
		if shopDef ~= nil and bitAnd( shopDef.BuyUIType,SHOPPING_BUY_UI_PACKAGE_TOLUA ) then
			table.insert(t_Items,shopDef.ShopID);
		end
	end
	nContainerMarketMaxPage = math.floor((table.getn(t_Items) - 1) /CONTAINER_MARKET_ONE_PAGE_NUM) + 1;
	if nContainerMarketMaxPage < 1 then
		nContainerMarketMaxPage = 1;
	end
	if nContainerMarketCurPage > nContainerMarketMaxPage then
		nContainerMarketMaxPage = nContainerMarketMaxPage;
	end
	if nContainerMarketCurPage < 1 then
		nContainerMarketCurPage = 1;
	end
end

function ContainerMarketFrame_UpdateMoney()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	ContainerMarketFrameDianquanBkgValueFont:SetText(MainPlayer:getGodCoin());
	ContainerMarketFrameJinbiBkgValueFont:SetText(MainPlayer:getBindGodCoin());
end


function ContainerMarketFrameUpdateItem()
	local nStartIndex =( nContainerMarketCurPage - 1 )*CONTAINER_MARKET_ONE_PAGE_NUM + 1;
	local nIndex = 1;
	for i=nStartIndex,table.getn( t_Items ) do
		if nIndex > CONTAINER_MARKET_ONE_PAGE_NUM then
			break;
		end
		local itemData = MarketManager:getItemByID( t_Items[i] );
		if itemData ~= nil then	
			local itemDef = getItemDef( itemData.ItemID );
			if itemDef ~= nil then
				local itemBtn = getglobal("ContainerMarketFrameItem"..nIndex);
				local item = getglobal("ContainerMarketFrameItem"..nIndex.."Btn");
				local itemBtnBoxTex = getglobal("ContainerMarketFrameItem"..nIndex.."BtnBoxTexture");	
				local itemBtnItemTex = getglobal("ContainerMarketFrameItem"..nIndex.."BtnIconTexture");
				local itemCountFont = getglobal("ContainerMarketFrameItem"..nIndex.."BtnCountFont");
				local itemNameFont = getglobal("ContainerMarketFrameItem"..nIndex.."Name")
				local itemMoneyValueFont = getglobal("ContainerMarketFrameItem"..nIndex.."MoneyValue");
				local itemMoneyTex = getglobal("ContainerMarketFrameItem"..nIndex.."MoneyTex");
				local itemUVAnimationTex = getglobal("ContainerMarketFrameItem"..nIndex.."BtnAnimationTex");
				local firstLineLabel = getglobal("ContainerMarketFrameItem"..nIndex.."MoneyValueName");				
				itemBtnBoxTex:Show();
				itemUVAnimationTex:Hide();
				itemBtn:SetClientUserData(0,itemData.ShopID);
				
				item:SetClientUserData(1,itemDef.ItemID);
				item:SetClientUserData(2,itemData.ShopID);
				local limitType = MarketManager:getMarketLimitType( itemData.ShopID );
				if MarketManager:isFlagNumLimit( limitType ) then
					itemUVAnimationTex:SetTexUV( 701,644,37,37 );
					itemUVAnimationTex:SetSize(37,37);
					itemUVAnimationTex:Show();
				elseif MarketManager:isFlagTimeLimit( limitType ) then
					itemUVAnimationTex:SetTexUV( 663,645,37,37 );
					itemUVAnimationTex:SetSize(37,37);
					itemUVAnimationTex:Show();
				end
				if itemData.Hot == 1 then
					itemUVAnimationTex:SetTexUV( 663,683,51,34 );
					itemUVAnimationTex:SetSize(51,34);
					itemUVAnimationTex:Show();
				end
				if itemData.New == 1 then
					itemUVAnimationTex:SetTexUV( 663,718,36,27 );
					itemUVAnimationTex:SetSize(36,27);
					itemUVAnimationTex:Show();
				end
				if itemData.ChuXiao == 1 then
					--显示促销
					itemUVAnimationTex:SetTexUV( 715,682,33,36 );
					itemUVAnimationTex:SetSize(33,36);
					itemUVAnimationTex:Show();
				end
				itemMoneyValueFont:SetText( MarketManager:getDiscountPirce(itemData.ShopID) );
				--icon
				if itemData.IconID ~= 0 then			
					itemBtnItemTex:SetTexture(GetItemIconPath(itemData.IconID));
				else
					itemBtnItemTex:SetTexture(GetItemIconPath(itemDef.IconID))
				end
				--物品名
				if itemData.Name ~= nil and itemData.Name ~= "" then
					itemNameFont:SetText(itemData.Name);
				else
					itemNameFont:SetText(itemDef.Name );
				end
				local t_NameColor = GetItemNameColor(itemDef);
				itemNameFont:SetTextColor( t_NameColor[1], t_NameColor[2], t_NameColor[3] );

				--根据 BuyType 改变银币图标
				itemMoneyTex:ChangeNormalTexture(t_TianyuanTypeUV[itemData.BuyType ].x,t_TianyuanTypeUV[itemData.BuyType ].y,t_TianyuanTypeUV[itemData.BuyType ].width,t_TianyuanTypeUV[itemData.BuyType].height);				
				itemMoneyTex:SetClientUserData(0,itemData.BuyType);
				--count
				if itemData.ItemNum > 0 and itemData.ItemNum ~= 1 then
					itemCountFont:SetText(itemData.ItemNum);
					itemCountFont:Show();
				else
					itemCountFont:Hide();
				end				
				itemBtn:Show();
				nIndex = nIndex + 1;
			end
		end
	end
	for i=nIndex,CONTAINER_MARKET_ONE_PAGE_NUM do
		local btn = getglobal("ContainerMarketFrameItem"..i);
		btn:Hide();
	end
	ContainerMarketFramePagePageFont:SetText("第"..nContainerMarketCurPage.."/"..nContainerMarketMaxPage.."页")
end

function ContainerMarketFrame_OnUpdate()
	this:SetPoint( "topright","$parent","topleft",PlayerContainerFrame:GetRealLeft()+4,PlayerContainerFrame:GetRealTop() );
end

function ContainerMarketFrame_OnShow()	
	ContainerMarketFrame_UpdateMoney();
	ContainerMarketFrameSetPage();
	ContainerMarketFrameUpdateItem();
end

function ContainerMarketFrameLeftPageBtn_OnClick()
	if nContainerMarketCurPage > 1 then
		nContainerMarketCurPage = nContainerMarketCurPage - 1;
		ContainerMarketFrameUpdateItem();
	end
end

function ContainerMarketFrameRightPageBtn_OnClick()
	if nContainerMarketCurPage < nContainerMarketMaxPage then
		nContainerMarketCurPage = nContainerMarketCurPage + 1;
		ContainerMarketFrameUpdateItem();
	end
end

function ContainerMarketFrameToLastPageBtn_OnClick()
	if nContainerMarketCurPage < nContainerMarketMaxPage then
		nContainerMarketCurPage = nContainerMarketMaxPage;
		ContainerMarketFrameUpdateItem();
	end
end

function ContainerMarketFrameToFirstPageBtn_OnClick()
	if nContainerMarketCurPage > 1 then
		nContainerMarketCurPage = 1;
		ContainerMarketFrameUpdateItem();
	end
end
--]]
function ContainerSetLockFrameComfirmPwdEditbox_OnTabPressed()
	SetFocusFrame("ContainerSetLockFrameComfirmPwdEditbox");
	ContainerSetLockFrameComfirmPwdEditbox:SelectAllText();
end

function ContainerSetLockFramePwdEditbox_OnTabPressed()
	SetFocusFrame("ContainerSetLockFramePwdEditbox");
	ContainerSetLockFramePwdEditbox:SelectAllText();
end

function GetMoneyEditbox_OnTabPressed()
	SetFocusFrame("ContainerSaveGetMoneyFrameGetBullEditbox");
	ContainerSaveGetMoneyFrameGetBullEditbox:SelectAllText();
end

function GetBullEditbox_OnTabPressed()
	SetFocusFrame("ContainerSaveGetMoneyFrameGetMoneyEditbox");
	ContainerSaveGetMoneyFrameGetMoneyEditbox:SelectAllText();
end

function OpenStashGridFrame_OnLoad()
	this:RegisterEvent( "UI_ACTOR_ATTRIBUTE_CHG" );
end

function OpenStashGridFrame_OnEvent()
	if arg1 == "UI_ACTOR_ATTRIBUTE_CHG" and OpenStashGridFrame:IsShown() then
		local mainplayer		= ActorMgr:getMainPlayer();
		OpenStashGridFrameCurHaveYinBiValueBtnFont:SetText( mainplayer:getMoney() );
	end
end

local t_ExStashMoney = { 10000,50000,150000,300000,300000,300000 };

function OpenStashGridFrame_OnShow()
	local mainplayer	= ActorMgr:getMainPlayer();
	local container = mainplayer:getContainer();
	local nMaxNum = container:getContainerMaxNum( CONTAINER_TYPE_EXTEND_STASH );	
	local nNeedMoney = t_ExStashMoney[ nMaxNum + 1 ];
	OpenStashGridFrame:SetClientUserData( 0,nNeedMoney );
	OpenStashGridFrameNeedYinBiValueBtnFont:SetText( nNeedMoney );
	OpenStashGridFrameCurHaveYinBiValueBtnFont:SetText( mainplayer:getMoney() );	
	if getPlayerMoney( mainplayer ) < nNeedMoney then
		OpenStashGridFrameCurHaveYinBiValueBtnFont:SetTextColor( 255,0,0 );
	else
		OpenStashGridFrameCurHaveYinBiValueBtnFont:SetTextColor( 150, 150, 150 );
	end
end

function OpenStashGridFrame_ConfirmBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	local nNeedMoney = OpenStashGridFrame:GetClientUserData(0);
	if getPlayerMoney( mainplayer ) < nNeedMoney then
		ShowMidTips( "银币不足" );
		return;
	end	
	if mainplayer == nil then return end;
	local container = mainplayer:getContainer();
	container:requestAddStashGrid();
	OpenStashGridFrame:Hide();
end

-------------------------------		合服后改名		--------------------------------------

function ContainerChangeRoleFrame_OnLoad()
	ContainerChangeRoleFrameRichText:resizeRect( 210,40 );
	ContainerChangeRoleFrameRichText:SetText("亲爱的玩家，由于合服后该角色名称重复，需要为您的角色重新命名！",255,125,0);
end

function ContainerChangeRoleFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 0, 0 );
	if this:GetClientUserData( 0 ) == 1 then
		ContainerChangeRoleFrameRichText:SetText("亲爱的玩家，由于合服后该角色名称重复，需要为您的角色重新命名！",255,125,0);
		ContainerChangeRoleFrameEditFont:SetText("请在输入框中输入角色新名字");
		ContainerChangeRoleFrame_NameEdit:setMaxChar( 15 );
	else
		ContainerChangeRoleFrameRichText:SetText("亲爱的玩家，由于合服后该公会名称重复，需要为您的公会重新命名！",255,125,0);
		ContainerChangeRoleFrameEditFont:SetText("请在输入框中输入公会新名字");
		ContainerChangeRoleFrame_NameEdit:setMaxChar( 14 );
	end
	ContainerChangeRoleFrame_NameEdit:Clear();
end

function ContainerChangeRoleFrame_OnHide()
	ClearFocus()
end

function ContainerChangeRoleFrameConfirmBtn_OnClick()
	local szText = ContainerChangeRoleFrame_NameEdit:GetText();
	if szText == "" then
		ShowMidTips( "请输入名字" );
		return;
	end	
	local mainplayer = ActorMgr:getMainPlayer();
	if ContainerChangeRoleFrame:GetClientUserData( 0 ) == 1 then
		mainplayer:ChangeRoleName( szText );
	else
		mainplayer:ChangeClanName( szText );
	end
	ContainerChangeRoleFrame:Hide();
end

function ContainerOpenLockFrameForgetPwdButton_OnClick()
	GameMgr:openWebSite( "http://passport.wanyouyl.com/editLockPwd.php" );
end

-----------------------		客户端分解相关		-----------------------------------
local RESOLVE_MAX			=	8;
local RESOLVE_MAX_PRODUCTION	=	2;
local CUR_RESOLVE_TYPE = "Equip"

local nResolveEquipCurIndex		=	1;
local nResolveGemCurIndex		=	1;
IsEquipResolved					=	false;
local IsGemResolved				=	false;
local ContainerResolveBagCurEquips	=	0;
local ContainerResolveBagCurGems	=	0;
local t_GemGridNum = { 0, 0, 0, 0, 0, 0, 0, 0,}

-- 装备分解 从背包分解拖出
function ContainerEquipResolveDrag( szBtnName )
	if IsEquipResolved then
		ShowMidTips( "分解中，无法执行操作" );
		return;
	end
	if not string.find( szBtnName, "Item" ) then
		return;
	end
	local btn			=	getglobal( szBtnName );
	local MainPlayer	=	ActorMgr:getMainPlayer();
	local container		=	MainPlayer:getContainer();
	local desList		=	btn:GetClientUserData( 1 )
	local desGrid		=	btn:GetClientUserData( 2 )
	nResolveEquipCurIndex	=	btn:GetClientID()
	local Item = container:getItem( desList, desGrid );
	if Item:getItemId() ~= 0 then
		UIMSG:getUIGV().DragFrom = DRAG_CONTAINERRESOLVE;
		local IconTex = getglobal(btn:GetName().."IconTexture");
		UIBeginDrag(IconTex:GetTexture(), desGrid, nResolveEquipCurIndex, Item:getItemId());
	end
end
-- 宝石分解 从背包分解拖出
function containerGemResolveDrag( szBtnName )
	if IsGemResolved then
		ShowMidTips( "分解中，无法执行操作" );
		return;
	end
	if not string.find( szBtnName, "Item" ) then
		return;
	end
	local btn			=	getglobal( szBtnName );
	local MainPlayer	=	ActorMgr:getMainPlayer();
	local container		=	MainPlayer:getContainer();
	local desList		=	btn:GetClientUserData( 1 )
	local desGrid		=	btn:GetClientUserData( 2 )
	nResolveGemCurIndex	=	btn:GetClientID()
	local Item = container:getItem( desList, desGrid );
	if Item:getItemId() ~= 0 then
		UIMSG:getUIGV().DragFrom = DRAG_CONTAINERRESOLVE;
		local IconTex = getglobal(btn:GetName().."IconTexture");
		UIBeginDrag(IconTex:GetTexture(), desGrid, nResolveGemCurIndex, Item:getItemId());
	end
end
--从背包分解拖出
function ContainerResolveDrag( szBtnName )
	if CUR_RESOLVE_TYPE =="Equip" then
		ContainerEquipResolveDrag( szBtnName );
	else
		containerGemResolveDrag( szBtnName );
	end
end

--装备分解 从分解栏拖到背包
function ContainerRemoveEquipResolveItem( nIdx )
	local btn = getglobal( "PlayerContainerResolveFrameItem" .. nIdx );
	local IconTex = getglobal(btn:GetName().."IconTexture");	
	local texture =	getglobal( btn:GetName().."IconTexture" )
	texture:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" )		
	ContainerResolveBagCurEquips = ContainerResolveBagCurEquips - 1;
	local UVAnimationTex = getglobal( "PlayerContainerResolveFrameItem"..nIdx.."UVAnimationTex" )
	UVAnimationTex:Hide()
	local desList =	btn:GetClientUserData( 1 )
	local desGrid =	btn:GetClientUserData( 2 )	
	UnLockItem( desList, desGrid );	
	btn:SetClientUserData( 1, 0 )
	btn:SetClientUserData( 2, 0 )
end
--宝石分解 从分解栏拖到背包
function ContainerRemoveGemResolveItem( nIdx )
	local btn = getglobal( "PlayerContainerResolveFrameGemItem" .. nIdx );
	local IconTex = getglobal(btn:GetName().."IconTexture");	
	local texture =	getglobal( btn:GetName().."IconTexture" )
	texture:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" )	
	local CountFont = getglobal( btn:GetName().."CountFont" );
	CountFont:SetText("");	
	ContainerResolveBagCurGems = ContainerResolveBagCurGems - 1;
	local UVAnimationTex = getglobal( "PlayerContainerResolveFrameGemItem"..nIdx.."UVAnimationTex" )
	UVAnimationTex:Hide()
	local desList =	btn:GetClientUserData( 1 )
	local desGrid =	btn:GetClientUserData( 2 )	
	UnLockItem( desList, desGrid );	
	btn:SetClientUserData( 1, 0 )
	btn:SetClientUserData( 2, 0 )
	t_GemGridNum[nIdx] = 0;
end
-- 从分解栏拖到背包
function ContainerRemoveResolveItem( nIdx )
	if CUR_RESOLVE_TYPE == "Equip" then
		ContainerRemoveEquipResolveItem( nIdx );
	else
		ContainerRemoveGemResolveItem( nIdx );
	end
end

--装备分解 如果目标容器是背包分解
function ReceiveContainerEquipResolve( szBtnName, dragfrom, dragdata1, dragdata2, dragdata3 )
	if ContainerResolveBagCurEquips == RESOLVE_MAX then
		ShowMidTips( "分解背包已满，无法执行操作" )
		return;
	end
	if IsEquipResolved then
		ShowMidTips( "分解中，无法执行操作" );
		return;
	end
	if not string.find( this:GetName(), "Item" ) then
		return;
	end
	local btn			=	getglobal( szBtnName );
	local MainPlayer	=	ActorMgr:getMainPlayer();
	local container		=	MainPlayer:getContainer();
	--如果拖拽来自背包分解自身
	if dragfrom == DRAG_CONTAINERRESOLVE then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	elseif dragfrom == CONTAINER_TYPE_SUNDRIES then
		local srcList		=	dragfrom;
		local srcGrid		=	dragdata1;		
		local item = container:getItem( srcList, srcGrid );
		local itemDef	=	item:getItemDef();
		if item:getItemType() ~= ITEM_SLOTTYPE_AMM or itemDef.NoResolve == 1 or itemDef.Important == 0 then
			ShowMidTips( "请放入可分解的装备" );
			return;
		end	
		if MainPlayer:getLv() + 20 < itemDef.WearPre.RoleLevelMin then
			ShowMidTips( "不可分解高于自身等级20级的装备" );
			return;
		end
		nResolveEquipCurIndex = btn:GetClientID();
		local BtnIconTexture	=	getglobal( btn:GetName().."IconTexture" );
		if not string.find( BtnIconTexture:GetTexture(), "TouMingTongDao.tga" ) then
			SetResolveCurEquipIndex();
		end
		btn = getglobal( "PlayerContainerResolveFrameItem"..nResolveEquipCurIndex);
		local desBtn = getglobal( "PlayerContainerFrameItem"..(srcGrid+1) );
		LockCurItem( item, desBtn:GetParent() );
		ContainerResolveBagCurEquips = ContainerResolveBagCurEquips + 1;
		btn:SetClientUserData( 1, srcList );
		btn:SetClientUserData( 2, srcGrid );
		local texture	=	getglobal( btn:GetName().."IconTexture" );
		SetEquipImportantEffectUV( itemDef, btn:GetName().."UVAnimationTex" )
		local texturePath	=	GetItemEquipIconPath()..itemDef.IconID..".tga";
		if not IsInExistence( texturePath ) then
			texturePath = DEFAULT_ITEM_PATH;
		end
		texture:SetTexture( texturePath );
		texture:Show()	
				
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	else
		ShowMidTips( "必须是背包中的装备才能被分解" );
		return;
	end
end	
--宝石分解 如果目标容器是背包分解
function ReceiveContainerGemResolve( szBtnName, dragfrom, dragdata1, dragdata2, dragdata3 )
	if ContainerResolveBagCurGems == RESOLVE_MAX then
		ShowMidTips( "分解背包已满，无法执行操作" )
		return;
	end
	if IsGemResolved then
		ShowMidTips( "分解中，无法执行操作" );
		return;
	end
	if not string.find( this:GetName(), "Item" ) then
		return;
	end
	local btn			=	getglobal( szBtnName );
	local MainPlayer	=	ActorMgr:getMainPlayer();
	local container		=	MainPlayer:getContainer();
	--如果拖拽来自背包分解自身
	if dragfrom == DRAG_CONTAINERRESOLVE then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	elseif dragfrom == CONTAINER_TYPE_SUNDRIES then
		local srcList		=	dragfrom;
		local srcGrid		=	dragdata1;		
		local item = container:getItem( srcList, srcGrid );
		local itemDef	=	item:getItemDef();
		local gemNewDef	=	GameArmProce:getGemNewDef( itemDef.ItemID );
		if not gemNewDef then
			ShowMidTips("请放入用于装备镶嵌的宝石");
			return;
		end
		if gemNewDef.Level > 4 then
			ShowMidTips("该宝石等级不符");
			return;
		end
		nResolveGemCurIndex = btn:GetClientID();
		local BtnIconTexture	=	getglobal( btn:GetName().."IconTexture" );
		if not string.find( BtnIconTexture:GetTexture(), "TouMingTongDao.tga" ) then
			SetResolveCurGemIndex();
		end
		btn = getglobal( "PlayerContainerResolveFrameGemItem"..nResolveGemCurIndex );
		local btnIndex = srcGrid % nSUNDRIESGridMax
		local desBtn = getglobal( "PlayerContainerFrameItem"..(btnIndex+1) );
		LockCurItem( item, desBtn:GetParent() );
		ContainerResolveBagCurGems = ContainerResolveBagCurGems + 1;
		btn:SetClientUserData( 1, srcList );
		btn:SetClientUserData( 2, srcGrid );
		local texture	=	getglobal( btn:GetName().."IconTexture" );
		SetEquipImportantEffectUV( itemDef, btn:GetName().."UVAnimationTex" )
		local texturePath	=	GetItemEquipIconPath()..itemDef.IconID..".tga";
		if not IsInExistence( texturePath ) then
			texturePath = DEFAULT_ITEM_PATH;
		end
		texture:SetTexture( texturePath );
		texture:Show();
		local CountFont = getglobal( btn:GetName().."CountFont" );
		if item:getNum() > 1 then
			CountFont:SetText( item:getNum() );
		end
		t_GemGridNum[nResolveGemCurIndex] = item:getNum();
				
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	else
		ShowMidTips( "必须是背包中的宝石才能被切割" );
		return;
	end
end	
--如果目标容器是背包分解
function ReceiveContainerResolve( szBtnName, dragfrom, dragdata1, dragdata2, dragdata3 )
	if CUR_RESOLVE_TYPE == "Equip" then
		ReceiveContainerEquipResolve( szBtnName, dragfrom, dragdata1, dragdata2, dragdata3 );
	else
		ReceiveContainerGemResolve( szBtnName, dragfrom, dragdata1, dragdata2, dragdata3 );
	end
end


--装备分解 设置背包当前空位置索引
function SetResolveCurEquipIndex()
	for i = 1, RESOLVE_MAX do
		local curtexture	=	getglobal( "PlayerContainerResolveFrameItem"..i.."IconTexture" );
		local curtextureName	=	curtexture:GetTexture();
		if  string.find( curtextureName, "TouMingTongDao.tga")  then	
			nResolveEquipCurIndex	= i;
			break;
		end
	end
end
--宝石分解 设置背包当前空位置索引
function SetResolveCurGemIndex()
	for i = 1, RESOLVE_MAX do
		local curtexture	=	getglobal( "PlayerContainerResolveFrameGemItem"..i.."IconTexture" );
		local curtextureName	=	curtexture:GetTexture();
		if  string.find( curtextureName, "TouMingTongDao.tga")  then	
			nResolveGemCurIndex	= i;
			break;
		end
	end
end
--设置背包当前空位置索引
function SetResolveCurIndex()
	if CUR_RESOLVE_TYPE == "Equip" then
		SetResolveCurEquipIndex();
	else
		SetResolveCurGemIndex();
	end
end

function PlayerContainerResolve_onShow()
	GemProceChargeFrame:Hide();
	BaoShiXiangQianFrame:Hide();
	ContainerResolveBagCurGems = 0;
	ContainerResolveBagCurEquips = 0;
--	this:SetPoint( "topright", "PlayerContainerFrame", "topleft", 0, 0 );
	if PlayerContainerSparEpurateFrame:IsShown() then
		local posX = PlayerContainerSparEpurateFrame:GetRealLeft() ;
		local posY = PlayerContainerSparEpurateFrame:GetRealTop();
		PlayerContainerResolveFrame:SetPoint( "topleft", "UIClient", "topleft", posX , posY );
		PlayerContainerSparEpurateFrame:Hide();
	else
		if PlayerContainerFrame:GetRealLeft() > 0 then
			local posX = PlayerContainerFrame:GetRealLeft() ;
			local posY = PlayerContainerFrame:GetRealTop();
			PlayerContainerResolveFrame:SetPoint( "topright", "UIClient", "topleft", posX , posY );
		else
			local posX = PlayerContainerFrame:GetRealRight() ;
			local posY = PlayerContainerFrame:GetRealTop();
			PlayerContainerResolveFrame:SetPoint( "topleft", "UIClient", "topleft", posX , posY );
		end
	end

	for i = 1,RESOLVE_MAX do
		local curSelectBtnBoxTex	=	getglobal( "PlayerContainerResolveFrameItem"..i.."BoxTexture" );
		curSelectBtnBoxTex:Hide();
		curSelectBtnBoxTex	=	getglobal( "PlayerContainerResolveFrameGemItem"..i.."BoxTexture" );
		curSelectBtnBoxTex:Hide();
	end

	if CUR_RESOLVE_TYPE == "Equip" then
		HideGems();
		PlayerContainerResolveFrameGemResolveBtn:DisChecked();
		PlayerContainerResolveFrameEquipResolveBtn:Checked();
		PlayerContainerResolveFrameSparEpurateBtn:DisChecked();
	else
		HideEquips();
		PlayerContainerResolveFrameGemResolveBtn:Checked();
		PlayerContainerResolveFrameEquipResolveBtn:DisChecked();
		PlayerContainerResolveFrameSparEpurateBtn:DisChecked();
	end
end

function PlayerContainerResolve_onLoad()
	SetProduction();
	ItemFirstCallBack( "PlayerContainerResolveFrame", AddItemToContainerResolve );
	for i = 1,RESOLVE_MAX do
		local equipBtn = getglobal("PlayerContainerResolveFrameItem"..i);
		equipBtn:SetClientUserData( 0, DRAG_CONTAINERRESOLVE);
		local gemBtn = getglobal("PlayerContainerResolveFrameGemItem"..i);
		gemBtn:SetClientUserData( 0, DRAG_CONTAINERRESOLVE );
	end
end

--装备分解 在背包右击对应处理
function AddItemToContainerEquipResolve( item, nListType,nGridIndex )
	if ContainerResolveBagCurEquips ==8 then
		ShowMidTips( "分解背包已满，无法执行操作" )
		return;
	end
	if IsEquipResolved then
		ShowMidTips( "分解中，无法执行操作" );
		return;
	end	
	local itemDef	=	item:getItemDef();
	-- 晶石不处理
	if itemDef.Result[0].ResultID == RESULT_SPAR then
		return true
	end
	if item:getItemType() ~= ITEM_SLOTTYPE_AMM or itemDef.NoResolve == 1 or itemDef.Important == 0 then
		ShowMidTips( "请放入可分解的装备" );
		return;
	end
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer:getLv() + 20 < itemDef.WearPre.RoleLevelMin then
		ShowMidTips( "不可分解高于自身等级20级的装备" );
		return;
	end
	--[[ 装备未鉴定
	if item:isArmIdentity() then 
		ShowMidTips("此装备还未鉴定，不能进行此操作");
		return;
	end
	--]]
	AddItemToResolveEquipFrame( item, nListType,nGridIndex )
end
function AddItemToResolveEquipFrame( item, nListType,nGridIndex )
	local itemDef = item:getItemDef();
	if item:getItemId() ~= nil then
		SetResolveCurEquipIndex()
		if not IsContainerItemLock( nListType, nGridIndex ) then
			LockCurItem( item, "PlayerContainerFrame" );
		end
		ContainerResolveBagCurEquips = ContainerResolveBagCurEquips + 1;
		local btn =	getglobal( "PlayerContainerResolveFrameItem"..nResolveEquipCurIndex) ;
		btn:SetClientUserData( 1, nListType);
		btn:SetClientUserData( 2, nGridIndex);
		local texture	=	getglobal( btn:GetName().."IconTexture" );
		SetEquipImportantEffectUV( itemDef, btn:GetName().."UVAnimationTex" )
		local texturePath	=	GetItemEquipIconPath()..itemDef.IconID..".tga";
		if not IsInExistence( texturePath ) then
			texturePath = DEFAULT_ITEM_PATH;
		end
		texture:SetTexture( texturePath );
		texture:Show()	
	end
end
--宝石分解 在背包右击对应处理
function AddItemToContainerGemResolve( item, nListType,nGridIndex )
	if ContainerResolveBagCurGems ==8 then
		ShowMidTips( "分解背包已满，无法执行操作" )
		return;
	end
	if IsGemResolved then
		ShowMidTips( "分解中，无法执行操作" );
		return;
	end	
	local itemDef	=	item:getItemDef();
	local gemNewDef	=	GameArmProce:getGemNewDef( itemDef.ItemID );
	if not gemNewDef then
		ShowMidTips("请放入用于装备镶嵌的宝石");
		return;
	end
	if gemNewDef.Level > 4 then
		ShowMidTips("该宝石等级不符");
		return;
	end
	if item:getItemId() ~= nil then
		SetResolveCurGemIndex();
		if not IsContainerItemLock( nListType, nGridIndex ) then
			LockCurItem( item, "PlayerContainerFrame" );
		end
		ContainerResolveBagCurGems = ContainerResolveBagCurGems + 1;
		local btn =	getglobal( "PlayerContainerResolveFrameGemItem"..nResolveGemCurIndex) ;
		btn:SetClientUserData( 1, nListType);
		btn:SetClientUserData( 2, nGridIndex);
		local texture	=	getglobal( btn:GetName().."IconTexture" );
		SetEquipImportantEffectUV( itemDef, btn:GetName().."UVAnimationTex" )
		local texturePath	=	GetItemEquipIconPath()..itemDef.IconID..".tga";
		if not IsInExistence( texturePath ) then
			texturePath = DEFAULT_ITEM_PATH;
		end
		texture:SetTexture( texturePath );
		texture:Show();
		local CountFont = getglobal( btn:GetName().."CountFont" );
		if item:getNum() > 1 then
			CountFont:SetText( item:getNum() );
		end
		t_GemGridNum[nResolveGemCurIndex] = item:getNum();
	end
end
--在背包右击对应处理
function AddItemToContainerResolve( item, nListType,nGridIndex )
	if CUR_RESOLVE_TYPE == "Equip" then
		return AddItemToContainerEquipResolve( item, nListType,nGridIndex );
	else
		return AddItemToContainerGemResolve( item, nListType,nGridIndex );
	end
end

--装备分解 在分解背包右击
function ContainerEquipResolveRightBtnClick( szBtnName )
	if IsEquipResolved then
		ShowMidTips( "分解中，无法执行操作" );
		return;
	end
	local btn			=	getglobal( szBtnName );
	local MainPlayer	=	ActorMgr:getMainPlayer();
	local container		=	MainPlayer:getContainer();
	local desList		=	btn:GetClientUserData( 1 );
	local desGrid		=	btn:GetClientUserData( 2 );
	
	local Item = container:getItem( desList, desGrid );
	if Item:getItemId() ~= 0 then				
		if IsContainerItemLock(  desList, desGrid ) then
			UnLockItem( desList, desGrid )
		end
		ContainerResolveBagCurEquips = ContainerResolveBagCurEquips - 1;
		local IconTex = getglobal(btn:GetName().."IconTexture");		
		local texture		=	getglobal( btn:GetName().."IconTexture" )
		texture:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" )
		local UVAnimationTex = getglobal( "PlayerContainerResolveFrameItem"..nResolveEquipCurIndex.."UVAnimationTex" )
		UVAnimationTex:Hide()
		btn:SetClientUserData( 1, 0 )
		btn:SetClientUserData( 2, 0 );
		ItemButton_OnLeave();
	end
end
--宝石分解 在分解背包右击
function ContainerGemResolveRightBtnClick( szBtnName )
	if IsGemResolved then
		ShowMidTips( "分解中，无法执行操作" );
		return;
	end
	local btn			=	getglobal( szBtnName );
	local MainPlayer	=	ActorMgr:getMainPlayer();
	local container		=	MainPlayer:getContainer();
	local desList		=	btn:GetClientUserData( 1 )
	local desGrid		=	btn:GetClientUserData( 2 )
	
	local Item = container:getItem( desList, desGrid );
	if Item:getItemId() ~= 0 then				
		if IsContainerItemLock(  desList, desGrid ) then
			UnLockItem( desList, desGrid )
		end
		ContainerResolveBagCurGems = ContainerResolveBagCurGems - 1;
		local IconTex = getglobal(btn:GetName().."IconTexture");		
		local texture		=	getglobal( btn:GetName().."IconTexture" )
		texture:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" )
		local UVAnimationTex = getglobal( "PlayerContainerResolveFrameGemItem"..nResolveGemCurIndex.."UVAnimationTex" )
		UVAnimationTex:Hide();
		local CountFont = getglobal( btn:GetName().."CountFont" );
		CountFont:SetText("");
		t_GemGridNum[nResolveGemCurIndex] = 0;
		btn:SetClientUserData( 1, 0 );
		btn:SetClientUserData( 2, 0 );
		ItemButton_OnLeave();
	end
end
--在分解背包右击
function ContainerResolveRightBtnClick( szBtnName )
	if CUR_RESOLVE_TYPE == "Equip" then
		ContainerEquipResolveRightBtnClick( szBtnName );
	else
		ContainerGemResolveRightBtnClick( szBtnName );
	end
end

function SetResolveEquipIndex()
	for i = 1, RESOLVE_MAX do
		local curtexture	=	getglobal( "PlayerContainerResolveFrameItem"..i.."IconTexture" );
		local curtextureName	=	curtexture:GetTexture();
		if not string.find( curtextureName, "TouMingTongDao.tga")  then	
			nResolveEquipCurIndex	= i;
			break;
		end
	end
end
function SetResolveGemIndex()
	for i = 1, RESOLVE_MAX do
		local curtexture	=	getglobal( "PlayerContainerResolveFrameGemItem"..i.."IconTexture" );
		local curtextureName	=	curtexture:GetTexture();
		if not string.find( curtextureName, "TouMingTongDao.tga")  then	
			nResolveGemCurIndex	= i;
			break;
		end
	end
end

--装备分解 分解按键响应
function PlayerContainerResolveBegin_onClick_Equip()
	if IsEquipResolved then 
		ShowMidTips("正在进行装备分解")
		return 
	end
	nResolveEquipCurIndex	=	1;
	if ContainerResolveBagCurEquips == 0 then
		ShowMidTips("背包为空，无法分解");
		IsEquipResolved = false;
		return;
	else
		SetResolveEquipIndex();
	end
	local btn	=	getglobal( "PlayerContainerResolveFrameItem"..nResolveEquipCurIndex) 
	local desList		=	btn:GetClientUserData( 1 );
	local desGrid		=	btn:GetClientUserData( 2 );	
	local MainPlayer	=	ActorMgr:getMainPlayer();
	local container		=	MainPlayer:getContainer();
	local Item 			=	container:getItem( desList, desGrid );
	local itemDef	=	Item:getItemDef();
	local armFenJieDef	=	GameArmProce:getArmFenJieDef( (itemDef.WearPre.RoleLevelMin / 10 ), itemDef.Important );
	if getPlayerMoney( MainPlayer ) < armFenJieDef.UseMoney then
		ShowMidTips("绑银不足，无法分解");
		return;
	end
	
	local idx = CheckItemNeedImportantConfirm( Item );
	if idx > 0 then
		ImportantItemConfirmFrameTitle:SetText("装备分解确认");
		ShowImportantItemConfirmFrame( "分解", Item, idx, true );
		SetItemWarnFrame("PlayerContainerResolveFrame");
		ImportantItemConfirmFrame:SetClientUserData( 0, desList );
		ImportantItemConfirmFrame:SetClientUserData( 1, desGrid );
		ImportantItemConfirmFrame:SetClientUserData( 2, 0 );
		ImportantItemConfirmFrame:SetClientUserData( 3, armFenJieDef.UseMoney );
		return;
	else
		if itemDef.Important > 2 then
			ImportantItemConfirmFrameTitle:SetText("装备分解确认");
			ShowImportantItemConfirmFrame( "分解", Item, idx, false );
			SetItemWarnFrame("PlayerContainerResolveFrame");
			ImportantItemConfirmFrame:SetClientUserData( 0, desList );
			ImportantItemConfirmFrame:SetClientUserData( 1, desGrid );
			ImportantItemConfirmFrame:SetClientUserData( 2, 0 );
			ImportantItemConfirmFrame:SetClientUserData( 3, armFenJieDef.UseMoney );
			return;
		end
	end

	if checkNeedWarnCost( armFenJieDef.UseMoney ) then
		CostMoneyConfirmFrame:SetClientString("装备分解");
		CostMoneyConfirmFrame:SetClientUserData( 0, desList );
		CostMoneyConfirmFrame:SetClientUserData( 1, desGrid );
		CostMoneyConfirmFrame:SetClientUserData( 2, 0 );
		SetWarnFrame( "PlayerContainerResolveFrame" );
		CostMoneyConfirmFrame:Show();
		return;
	end

	GameArmProce:requestContainerResolvePre( Item:getItemWId(), desList, desGrid, 0 );
end
--宝石分解 分解按键响应
function PlayerContainerResolveBegin_onClick_Gem()
	if IsGemResolved then 
		ShowMidTips("正在进行宝石切割");
		return 
	end
	nResolveGemCurIndex	=	1;
	if ContainerResolveBagCurGems == 0 then
		ShowMidTips("请放入想要分解的宝石");
		IsGemResolved = false;
		return;
	else
		SetResolveGemIndex();
	end
	local btn	=	getglobal( "PlayerContainerResolveFrameGemItem"..nResolveGemCurIndex) 
	local desList		=	btn:GetClientUserData( 1 );
	local desGrid		=	btn:GetClientUserData( 2 );	
	local MainPlayer	=	ActorMgr:getMainPlayer();
	local container		=	MainPlayer:getContainer();
	local Item 			=	container:getItem( desList, desGrid );
	local itemDef	=	Item:getItemDef();
	local gemNewDef = GameArmProce:getGemNewDef( itemDef.ItemID );
	if getPlayerMoney( MainPlayer ) < gemNewDef.UseMoney then
		ShowMidTips("银币不足，无法分解");
		return;
	end
	local idx = CheckItemNeedImportantConfirm( Item );
	if idx > 0 then
		ImportantItemConfirmFrameTitle:SetText("宝石分解确认");
		ShowImportantItemConfirmFrame( "分解", Item, idx, false );
		SetItemWarnFrame("PlayerContainerResolveFrame");
		ImportantItemConfirmFrame:SetClientUserData( 0, desList );
		ImportantItemConfirmFrame:SetClientUserData( 1, desGrid );
		ImportantItemConfirmFrame:SetClientUserData( 2, 1 );
		return;
	end
	GameArmProce:requestContainerResolvePre( Item:getItemWId(), desList, desGrid, 1 );
end
--分解按键响应
function PlayerContainerResolveBegin_onClick()
	if CUR_RESOLVE_TYPE == "Equip" then
		if CostMoneyConfirmFrame:IsShown() then
			return;
		end
		PlayerContainerResolveBegin_onClick_Equip();
	else
		PlayerContainerResolveBegin_onClick_Gem();
	end
end

function StartContainerResolve( nTime )
	if CUR_RESOLVE_TYPE == "Equip" then
		IsEquipResolved	=	true;
		IntonateBar_text:SetText( "正在进行装备分解" );
	else
		IsGemResolved = true;
		IntonateBar_text:SetText( "正在进行宝石分解" );
	end	
	IntonateBar:SetClientUserData( 0, RESOLVE_TYPE );
	IntonateBar:SetClientUserData( 2, nTime );
	IntonateBar:Show()
end

function PlayerContainerResolveBreak_onClick()	
	local containerResolveFrame	=	getglobal( "PlayerContainerResolveFrame" );
	containerResolveFrame:Hide();
end

function PlayerContainerResolveFrameBreakPre()
	if CUR_RESOLVE_TYPE == "Equip" then
		IsEquipResolved = false;
	else
		IsGemResolved = false;
	end
	local containerResolveFrame	=	getglobal( "PlayerContainerResolveFrame" );
	containerResolveFrame:Hide();
end

function ContainerResolveBtn_onClick()
	local resolveBtn	= getglobal( "PlayerContainerFrameResolveBtn" )
	--[[
	if not resolveBtn:IsEnable() then
		return;
	end
	--]]
	local resolveFrame	=	getglobal( "PlayerContainerResolveFrame" )
	if resolveFrame:IsShown() then
		resolveFrame:Hide()
	else
		nResolveEquipCurIndex	=	1
		nResolveGemCurIndex		=	1
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer:getLv() < CONTAINER_RESOLVE_LEVEL_LIMIT_MIN then return end
		resolveFrame:Show()
	end
end

function BreakContainerResolve()
	if CUR_RESOLVE_TYPE == "Equip" then
		IsEquipResolved = false;
	else
		IsGemResolved = false;
	end
	IntonateBar:Hide();
end

function ContainerResolveTouMingBtn_onEnter()
	local frame	=	getglobal( "PlayerContainerResolveFrame" )
	if frame:IsShown() then
		return;
	end
	local btn	=	getglobal( "PlayerContainerFrameResolveBtn" );
	if not btn:IsEnable() then
		SetGameTooltips( "PlayerContainerFrameResolveBtn", "角色达到"..CONTAINER_RESOLVE_LEVEL_LIMIT_MIN.."级自动开启分解功能" );
	end	
end

function PlayerContainerResolve_onHide()
	PlayerContainerResolveFrameDipartBtn:SetGray( false );
	PlayerContainerResolveFrameDipartBtn:Enable();
	if IsEquipResolved or IsGemResolved then
		IsEquipResolved = false;
		IsGemResolved = false;
		if IntonateBar:IsShown() then
			IntonateBar:Hide();
		end
	end
	for i = 1, RESOLVE_MAX do
		local btn	=	getglobal( "PlayerContainerResolveFrameItem"..i );
		local btnTexture	=	getglobal( "PlayerContainerResolveFrameItem"..i.."IconTexture" );
		if not string.find( btnTexture:GetTexture(), "TouMingTongDao.tga") then			
			local MainPlayer	=	ActorMgr:getMainPlayer();
			local container		=	MainPlayer:getContainer();
			local desList		=	btn:GetClientUserData( 1 );
			local desGrid		=	btn:GetClientUserData( 2 );
	
			UnLockItem( desList, desGrid );
			
			local IconTex = getglobal(btn:GetName().."IconTexture");		
			local texture		=	getglobal( btn:GetName().."IconTexture" );
			texture:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
			local UVAnimationTex = getglobal( btn:GetName().."UVAnimationTex" )
			UVAnimationTex:Hide()
			local boxTexture = getglobal( btn:GetName().."BoxTexture" );
			boxTexture:Hide();
			btn:SetClientUserData( 1, 0 )
			btn:SetClientUserData( 2, 0 )
		end
		local gemBtn	=	getglobal( "PlayerContainerResolveFrameGemItem"..i );
		local gemBtnTexture	=	getglobal( "PlayerContainerResolveFrameGemItem"..i.."IconTexture" );
		if not string.find( gemBtnTexture:GetTexture(), "TouMingTongDao.tga") then			
			local MainPlayer	=	ActorMgr:getMainPlayer();
			local container		=	MainPlayer:getContainer();
			local desList		=	gemBtn:GetClientUserData( 1 );
			local desGrid		=	gemBtn:GetClientUserData( 2 );
	
			UnLockItem( desList, desGrid );
			
			local IconTex = getglobal(gemBtn:GetName().."IconTexture");		
			local texture		=	getglobal( gemBtn:GetName().."IconTexture" );
			texture:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
			local UVAnimationTex = getglobal( gemBtn:GetName().."UVAnimationTex" )
			UVAnimationTex:Hide()
			local boxTexture = getglobal( gemBtn:GetName().."BoxTexture" );
			boxTexture:Hide();
			local CountFont = getglobal( gemBtn:GetName().."CountFont" );
			CountFont:SetText("");
			gemBtn:SetClientUserData( 1, 0 )
			gemBtn:SetClientUserData( 2, 0 )
			t_GemGridNum[i] = 0;
		end
	end
	ContainerResolveBagCurGems = 0;
	ContainerResolveBagCurEquips = 0;
	SetProduction();
	GameArmProce:requestBreakContainerResolvePre();
	CUR_RESOLVE_TYPE = "Equip";
end

function HideGems()
	PlayerContainerResolveFrameInfoFont:SetText( "装备" );
	PlayerContainerResolveFrameInfoFont:SetTextColor( 151, 45, 249 );
	for i = 1, RESOLVE_MAX do
		local gemBtn = getglobal( "PlayerContainerResolveFrameGemItem"..i );
		gemBtn:Hide();
		local equipBtn = getglobal( "PlayerContainerResolveFrameItem"..i );
		equipBtn:Show();
	end
end
function HideEquips()
	PlayerContainerResolveFrameInfoFont:SetText( "宝石" );
	PlayerContainerResolveFrameInfoFont:SetTextColor( 244, 188, 36 );
	for i = 1, RESOLVE_MAX do
		local equipBtn = getglobal( "PlayerContainerResolveFrameItem"..i );
		equipBtn:Hide();
		local gemBtn = getglobal( "PlayerContainerResolveFrameGemItem"..i );
		gemBtn:Show();
	end
end
function PlayerContainerResolve_onUpdate()
	if IsGemResolved or IsEquipResolved then
		PlayerContainerResolveFrameDipartBtn:SetGray( true );
		PlayerContainerResolveFrameDipartBtn:Disable();
	else
		PlayerContainerResolveFrameDipartBtn:SetGray( false );
		PlayerContainerResolveFrameDipartBtn:Enable();
	end
	if CUR_RESOLVE_TYPE == "Equip" then
		HideGems();
		for i = 1,RESOLVE_MAX do
			local curSelectBtnBoxTex	=	getglobal( "PlayerContainerResolveFrameItem"..i.."BoxTexture" );
			local btnTexture			=	getglobal( "PlayerContainerResolveFrameItem"..i.."IconTexture" );
			if i ~= nResolveEquipCurIndex or string.find( btnTexture:GetTexture(), "TouMingTongDao.tga") then
				curSelectBtnBoxTex:Hide();
			else
				curSelectBtnBoxTex:Show();
			end
		end
	else
		HideEquips();
		for i = 1,RESOLVE_MAX do
			local curSelectBtnBoxTex	=	getglobal( "PlayerContainerResolveFrameGemItem"..i.."BoxTexture" );
			local btnTexture			=	getglobal( "PlayerContainerResolveFrameGemItem"..i.."IconTexture" );
			if i ~= nResolveGemCurIndex or string.find( btnTexture:GetTexture(), "TouMingTongDao.tga") then
				curSelectBtnBoxTex:Hide();
			else
				curSelectBtnBoxTex:Show();
			end
		end
	end
	SetProduction();
end
function ResolveBtn_onEnter()
	if not string.find( this:GetName(), "Item" ) then
		local itemId 		=	this:GetClientUserData( 3 )
		if itemId == 0 then
			return;
		end
		local ItemDef			=	getItemDef( itemId );
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, ItemDef );
	else
		local MainPlayer	=	ActorMgr:getMainPlayer();
		local container		=	MainPlayer:getContainer();
		local nType	= this:GetClientUserData( 1 )
		local nGrid = this:GetClientUserData( 2 )
		local Item 	= container:getItem( nType, nGrid );
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, Item, nil, this:GetName() );
	end
end
function ResolveBtn_onLeave()
	NormalItemTipsFrame:Hide();
end
--装备分解 未进行分解时，鼠标在分解框内点击
function ResolveBtn_onClick_Equip()
	if IsEquipResolved then
		return;
	end
	if not string.find( this:GetName(), "Item" ) then
		return
	end
	local selectid	=	this:GetClientID();
	if selectid > 0 and selectid <9 then
		nResolveEquipCurIndex = selectid;		
	end
	if( arg1 == "RightButton" ) then
		ContainerEquipResolveRightBtnClick( this:GetName() );
		return;
	end
	local curSelectBtnTexture	=	getglobal( "PlayerContainerResolveFrameItem"..nResolveEquipCurIndex.."BoxTexture" );
	curSelectBtnTexture:Hide();
end
--宝石分解 未进行分解时，鼠标在分解框内点击
function ResolveBtn_onClick_Gem()
	if IsGemResolved then
		return;
	end
	if not string.find( this:GetName(), "Item" ) then
		return
	end
	local selectid	=	this:GetClientID();
	if selectid > 0 and selectid <9 then
		nResolveGemCurIndex = selectid;		
	end
	if( arg1 == "RightButton" ) then
		ContainerResolveRightBtnClick( this:GetName() );
		return;
	end
	local curSelectBtnTexture	=	getglobal( "PlayerContainerResolveFrameItem"..nResolveGemCurIndex.."BoxTexture" );
	curSelectBtnTexture:Hide();
end
--未进行分解时，鼠标在分解框内点击
function ResolveBtn_onClick()
	if CUR_RESOLVE_TYPE == "Equip" then
		ResolveBtn_onClick_Equip();
	else
		ResolveBtn_onClick_Gem();
	end
end

--装备分解 下一个分解
function callNextEquipResolve()
	ContainerResolveBagCurEquips = ContainerResolveBagCurEquips - 1;
	local texture		=	getglobal( "PlayerContainerResolveFrameItem"..nResolveEquipCurIndex.."IconTexture" )
	texture:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" )
	local UVAnimationTex = getglobal( "PlayerContainerResolveFrameItem"..nResolveEquipCurIndex.."UVAnimationTex" )
	UVAnimationTex:Hide()
	local btn	=	getglobal( "PlayerContainerResolveFrameItem"..nResolveEquipCurIndex)		
	if IsContainerItemLock(  btn:GetClientUserData( 1 ), btn:GetClientUserData( 2 ) ) then
		UnLockItem( btn:GetClientUserData( 1 ), btn:GetClientUserData( 2 ) )
	end
	btn:SetClientUserData( 1, 0 )
	btn:SetClientUserData( 2, 0 )
	if not IsEquipResolved then
		return;
	end
	if ContainerResolveBagCurEquips == 0 then
		ShowMidTips( "分解已完成");
		IsEquipResolved	=	false;
		SetResolveCurEquipIndex();
		GameArmProce:stopContainerResolve();
		return;
	else
		SetResolveEquipIndex();
	end
	local btn	=	getglobal( "PlayerContainerResolveFrameItem"..nResolveEquipCurIndex) 
	local desList		=	btn:GetClientUserData( 1 );
	local desGrid		=	btn:GetClientUserData( 2 );	
	local MainPlayer	=	ActorMgr:getMainPlayer();
	local container		=	MainPlayer:getContainer();
	local Item 			=	container:getItem( desList, desGrid );
	if Item:getItemType() ~= ITEM_SLOTTYPE_AMM then
		return
	end
	local itemDef		=	Item:getItemDef()
	local armFenJieDef	=	GameArmProce:getArmFenJieDef( (itemDef.WearPre.RoleLevelMin / 10 ), itemDef.Important );
	if getPlayerMoney( MainPlayer ) < armFenJieDef.UseMoney then
		ShowMidTips("绑银不足，无法分解");
		return;
	end

	local idx = CheckItemNeedImportantConfirm( Item );
	if idx > 0 then
		ImportantItemConfirmFrameTitle:SetText("装备分解确认");
		ShowImportantItemConfirmFrame( "分解", Item, idx, true );
		SetItemWarnFrame("PlayerContainerResolveFrame");
		ImportantItemConfirmFrame:SetClientUserData( 0, desList );
		ImportantItemConfirmFrame:SetClientUserData( 1, desGrid );
		ImportantItemConfirmFrame:SetClientUserData( 2, 0 );
		ImportantItemConfirmFrame:SetClientUserData( 3, armFenJieDef.UseMoney );
		return;
	else
		if itemDef.Important > 2 then
			ImportantItemConfirmFrameTitle:SetText("装备分解确认");
			ShowImportantItemConfirmFrame( "分解", Item, idx, false );
			SetItemWarnFrame("PlayerContainerResolveFrame");
			ImportantItemConfirmFrame:SetClientUserData( 0, desList );
			ImportantItemConfirmFrame:SetClientUserData( 1, desGrid );
			ImportantItemConfirmFrame:SetClientUserData( 2, 0 );
			ImportantItemConfirmFrame:SetClientUserData( 3, armFenJieDef.UseMoney );
			return;
		end
	end

	if checkNeedWarnCost( armFenJieDef.UseMoney ) then
		CostMoneyConfirmFrame:SetClientString("装备分解");
		CostMoneyConfirmFrame:SetClientUserData( 0, desList );
		CostMoneyConfirmFrame:SetClientUserData( 1, desGrid );
		CostMoneyConfirmFrame:SetClientUserData( 2, 1 );
		SetWarnFrame( "PlayerContainerResolveFrame" );
		CostMoneyConfirmFrame:Show();
		return;
	end
	GameArmProce:requestContainerResolvePre( Item:getItemWId(), desList, desGrid, 0 );
end
--宝石分解 下一个分解
function callNextGemResolve()
	ContainerResolveBagCurGems = ContainerResolveBagCurGems - 1;
	local texture		=	getglobal( "PlayerContainerResolveFrameGemItem"..nResolveGemCurIndex.."IconTexture" )
	texture:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" )
	local UVAnimationTex = getglobal( "PlayerContainerResolveFrameGemItem"..nResolveGemCurIndex.."UVAnimationTex" )
	UVAnimationTex:Hide();
	local CountFont = getglobal( "PlayerContainerResolveFrameGemItem"..nResolveGemCurIndex.."CountFont" );
	CountFont:SetText("");
	t_GemGridNum[nResolveGemCurIndex] = 0;
	local btn	=	getglobal( "PlayerContainerResolveFrameGemItem"..nResolveGemCurIndex)		
	if IsContainerItemLock(  btn:GetClientUserData( 1 ), btn:GetClientUserData( 2 ) ) then
		UnLockItem( btn:GetClientUserData( 1 ), btn:GetClientUserData( 2 ) )
	end
	btn:SetClientUserData( 1, 0 )
	btn:SetClientUserData( 2, 0 )
	if not IsGemResolved then
		return;
	end
	if ContainerResolveBagCurGems == 0 then
		ShowMidTips( "分解已完成");
		IsGemResolved	=	false;
		SetResolveCurGemIndex();
		GameArmProce:stopContainerResolve();
		return;
	else
		SetResolveGemIndex();
	end
	local btn	=	getglobal( "PlayerContainerResolveFrameGemItem"..nResolveGemCurIndex) 
	local desList		=	btn:GetClientUserData( 1 );
	local desGrid		=	btn:GetClientUserData( 2 );	
	local MainPlayer	=	ActorMgr:getMainPlayer();
	local container		=	MainPlayer:getContainer();
	local Item 			=	container:getItem( desList, desGrid );
	local itemDef		=	Item:getItemDef()
	local gemNewDef 	=	GameArmProce:getGemNewDef( itemDef.ItemID );
	if getPlayerMoney( MainPlayer ) < gemNewDef.UseMoney then
		ShowMidTips("银币不足，无法分解");
		if IsEquipResolved or IsGemResolved then
			IsEquipResolved = false;
			IsGemResolved = false;
			if IntonateBar:IsShown() then
				IntonateBar:Hide();
			end
		end
		GameArmProce:requestBreakContainerResolvePre();
		return;
	end
	local idx = CheckItemNeedImportantConfirm( Item );
	if idx > 0 then
		ImportantItemConfirmFrameTitle:SetText("宝石分解确认");
		ShowImportantItemConfirmFrame( "分解", Item, idx, false );
		SetItemWarnFrame("PlayerContainerResolveFrame");
		ImportantItemConfirmFrame:SetClientUserData( 0, desList );
		ImportantItemConfirmFrame:SetClientUserData( 1, desGrid );
		ImportantItemConfirmFrame:SetClientUserData( 2, 1 );
		return;
	end
	GameArmProce:requestContainerResolvePre( Item:getItemWId(), desList, desGrid, 1 )	
end
--下一个分解
function callNextResolve()
	if CUR_RESOLVE_TYPE == "Equip" then
		callNextEquipResolve();
	else
		callNextGemResolve();
	end
end

function CancelFenjieByComfirm()
	IsEquipResolved = false;
	IsGemResolved = false;
	GameArmProce:stopContainerResolve();
end

function OnFinishFenjie( nSucc )
	if nSucc == 0 then
		ShowMidTips("分解失败");
		IsEquipResolved = false;
		IsGemResolved = false;
		GameArmProce:stopContainerResolve();
	else
		callNextResolve();
	end
end
function ClearProduction()	
	for i = 1,RESOLVE_MAX_PRODUCTION do
		local production		=	getglobal( "PlayerContainerResolveFrameProduction"..i )
		production:SetClientUserData( 3, 0 )
		local productionName	=	getglobal( "PlayerContainerResolveFrameProductionName"..i );
		productionName:SetText( "" );
		local productionRate	=	getglobal( "PlayerContainerResolveFrameProductionRate"..i );
		productionRate:SetText( "" );
		local productionTex	=	getglobal( "PlayerContainerResolveFrameProduction"..i.."IconTexture" );
		productionTex:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
		local UVAnimationTex = getglobal( "PlayerContainerResolveFrameProduction"..i.."UVAnimationTex" )
		UVAnimationTex:Hide()
		local CountFont = getglobal( "PlayerContainerResolveFrameProduction"..i.."CountFont" )
		CountFont:SetText("")
		local BackTexture = getglobal( "PlayerContainerResolveFrameProduction"..i.."BackTexture")
		BackTexture:Hide()
	end
	local WarningTex	=	getglobal("PlayerContainerResolveFrameWarningTex");
	WarningTex:Hide();
	local WarningInfoFont = getglobal("PlayerContainerResolveFrameWarningInfoFont");
	WarningInfoFont:SetText("");
	local WarningMoneyTex = getglobal("PlayerContainerResolveFrameWarningMoneyTex");
	WarningMoneyTex:Hide();
end

--装备分解 设置产品
function SetEquipProduction()
	if nResolveEquipCurIndex == 0 then return end
	local btnTexture = getglobal( "PlayerContainerResolveFrameItem"..nResolveEquipCurIndex.."IconTexture" )
	if string.find( btnTexture:GetTexture(), "TouMingTongDao.tga") then
		ClearProduction();
		return;
	end
	local btn			=	getglobal( "PlayerContainerResolveFrameItem"..nResolveEquipCurIndex ) ;
	local desList		=	btn:GetClientUserData( 1 );
	local desGrid		=	btn:GetClientUserData( 2 );
	
	local MainPlayer	=	ActorMgr:getMainPlayer();
	local container		=	MainPlayer:getContainer();
	local Item 			=	container:getItem( desList, desGrid );
	if Item:getItemId() == 0 or Item:getItemType() ~= ITEM_SLOTTYPE_AMM then
		return;
	end
	local itemDef		=	Item:getItemDef();
	local armFenJieDef	=	GameArmProce:getArmFenJieDef( (itemDef.WearPre.RoleLevelMin / 10 ), itemDef.Important );
	local WarningTex	=	getglobal("PlayerContainerResolveFrameWarningTex");
	WarningTex:Show();
	local WarningInfoFont = getglobal("PlayerContainerResolveFrameWarningInfoFont");
	WarningInfoFont:SetText("装备分解需要花费"..armFenJieDef.UseMoney);
	local WarningMoneyTex = getglobal("PlayerContainerResolveFrameWarningMoneyTex");
	local OffSetX = WarningInfoFont:GetTextExtentWidth( WarningInfoFont:GetText() )/ GetScreenScaleY() - 1 ;
	WarningMoneyTex:SetPoint( "topleft", "PlayerContainerResolveFrameWarningInfoFont", "topleft", OffSetX, -3 );
	WarningMoneyTex:ChangeTextureTemplate( "BullTypeTextureTemplate" )
	WarningMoneyTex:Show();
	for i = 1, RESOLVE_MAX_PRODUCTION do
		if armFenJieDef.GetItem[i-1].Num == 0 then
			break;
		end
		if Item:isArmIdentity() and i > 1 then
			break;
		end
		local production	=	getglobal( "PlayerContainerResolveFrameProduction"..i )
		production:SetClientUserData( 3, armFenJieDef.GetItem[i-1].ItemID )
		local productionItem	=	container:getItem( armFenJieDef.GetItem[i-1].ItemID )
		local productionItemDef	=	getItemDef( armFenJieDef.GetItem[i-1].ItemID )
		SetEquipImportantEffectUV( productionItemDef, "PlayerContainerResolveFrameProduction"..i.."UVAnimationTex" )
		local productionName	=	getglobal( "PlayerContainerResolveFrameProductionName"..i );
		productionName:SetText( productionItemDef.Name );
		local rate	= setContainerResolveRate( armFenJieDef.GetItem[i-1].Rate );	
		local productionRate	=	getglobal( "PlayerContainerResolveFrameProductionRate"..i );
		productionRate:SetText( "概率：".. rate  );
		local productionTex	=	getglobal( "PlayerContainerResolveFrameProduction"..i.."IconTexture" );
		productionTex:SetTexture( GetItemEquipIconPath()..productionItemDef.IconID..".tga" );
		local CountFont = getglobal( "PlayerContainerResolveFrameProduction"..i.."CountFont" )
		CountFont:SetText( armFenJieDef.GetItem[i-1].Num )
		local BackTexture = getglobal( "PlayerContainerResolveFrameProduction"..i.."BackTexture")
		BackTexture:Show()
	end
end
--宝石分解 设置产品
function SetGemProduction()
	if nResolveGemCurIndex == 0 then return end
	local btnTexture = getglobal( "PlayerContainerResolveFrameGemItem"..nResolveGemCurIndex.."IconTexture" )
	if string.find( btnTexture:GetTexture(), "TouMingTongDao.tga") then
		ClearProduction();
		return;
	end
	local btn			=	getglobal( "PlayerContainerResolveFrameGemItem"..nResolveGemCurIndex ) ;
	local desList		=	btn:GetClientUserData( 1 );
	local desGrid		=	btn:GetClientUserData( 2 );
	
	local MainPlayer	=	ActorMgr:getMainPlayer();
	local container		=	MainPlayer:getContainer();
	local Item 			=	container:getItem( desList, desGrid );
	if Item:getItemId() == 0 then
		return;
	end
	local gemNewDef		=	GameArmProce:getGemNewDef( Item:getItemId() )
	local gemFenJieDef	=	gemNewDef.GemFengJie;
	local WarningTex	=	getglobal("PlayerContainerResolveFrameWarningTex");
	WarningTex:Show();
	local WarningInfoFont = getglobal("PlayerContainerResolveFrameWarningInfoFont");
	local needMoneyNum = gemNewDef.UseMoney*t_GemGridNum[nResolveGemCurIndex];
	WarningInfoFont:SetText("宝石分解需要花费"..needMoneyNum);
	local WarningMoneyTex = getglobal("PlayerContainerResolveFrameWarningMoneyTex");
	local OffSetX = WarningInfoFont:GetTextExtentWidth( WarningInfoFont:GetText() )/ GetScreenScaleY() - 1  ;
	WarningMoneyTex:SetPoint( "topleft", "PlayerContainerResolveFrameWarningInfoFont", "topleft", OffSetX, -3 );
	WarningMoneyTex:ChangeTextureTemplate( "MoneyTypeTextureTemplate" )
	WarningMoneyTex:Show();
	for i = 1, RESOLVE_MAX_PRODUCTION do
		if gemFenJieDef[i-1].ID == 0 then return end
		local production	=	getglobal( "PlayerContainerResolveFrameProduction"..i )
		production:SetClientUserData( 3, gemFenJieDef[i-1].ID );
		local productionItem	=	container:getItem( gemFenJieDef[i-1].ID );
		local productionItemDef	=	getItemDef( gemFenJieDef[i-1].ID );
		SetEquipImportantEffectUV( productionItemDef, "PlayerContainerResolveFrameProduction"..i.."UVAnimationTex" )
		local productionName	=	getglobal( "PlayerContainerResolveFrameProductionName"..i );
		productionName:SetText( productionItemDef.Name );
		local rate	= gemFenJieDef[i-1].Min*t_GemGridNum[nResolveGemCurIndex].."~"..gemFenJieDef[i-1].Max*t_GemGridNum[nResolveGemCurIndex];	
		local productionRate	=	getglobal( "PlayerContainerResolveFrameProductionRate"..i );
		productionRate:SetText( "数量：".. rate  );
		local productionTex	=	getglobal( "PlayerContainerResolveFrameProduction"..i.."IconTexture" );
		productionTex:SetTexture( GetItemEquipIconPath()..productionItemDef.IconID..".tga" );
		local BackTexture = getglobal( "PlayerContainerResolveFrameProduction"..i.."BackTexture")
		BackTexture:Show()
	end
end
function SetProduction()
	ClearProduction()
	if CUR_RESOLVE_TYPE == "Equip" then
		SetEquipProduction();
	else
		SetGemProduction();
	end
end
function setContainerResolveRate( Rate )
	local rate;
	if Rate >= 0 then
		rate = t_ResolveRate[1];
	end
	if Rate >= 1 then
		rate = t_ResolveRate[10];
	end
	if Rate > 10 then
		rate = t_ResolveRate[20];
	end
	if Rate > 20 then
		rate = t_ResolveRate[50];
	end
	if Rate > 50 then
		rate = t_ResolveRate[80];
	end
	if Rate > 80 then
		rate = t_ResolveRate[100];
	end
	return rate;
end
function PlayerContainerResolveChooseTypeBtn_onClick()
	if IsEquipResolved or IsGemResolved then return end
	if ImportantItemConfirmFrame:IsShown() and string.find( ImportantItemConfirmFrameTitle:GetText(), "分解确认", 1, true ) then
		return;
	end

	local equipResolveBtn = getglobal("PlayerContainerResolveFrameEquipResolveBtn");
	local gemResolveBtn = getglobal("PlayerContainerResolveFrameGemResolveBtn");
	local sparEpurateBtn = getglobal("PlayerContainerResolveFrameSparEpurateBtn");
	local name = this:GetName();
	if string.find( name, "Equip") then
		CUR_RESOLVE_TYPE ="Equip";
		equipResolveBtn:Checked();
		gemResolveBtn:DisChecked();
		sparEpurateBtn:DisChecked();
	else
		if CostMoneyConfirmFrame:IsShown() then
			CostMoneyConfirmFrame:Hide();
		end
		if string.find( name, "Gem") then
			CUR_RESOLVE_TYPE = "Gem";
			equipResolveBtn:DisChecked();
			gemResolveBtn:Checked();
			sparEpurateBtn:DisChecked();
		else
			if GeniusMgr:getAllGeniusValue() < TILIAN_CAN_USE_LV then
				return ShowMidTips( string.format("需要总天赋值达到%d点才能使用晶石提炼", TILIAN_CAN_USE_LV) );
			end
			PlayerContainerSparEpurateFrame:Show();
		end
	end
end

function ContainerFrame_Tex_OnEnter()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then return end
	local levelupDef = getLevelUP( mainplayer:getLv() )
	if levelupDef ~= nil then
		local text = string.format("当前人物等级：%d\n可携带银币上限：%d\n超出上限的银币在下线时会自动转换为绑银",mainplayer:getLv(),levelupDef.MaxPackageMoney);
		SetGameTooltips( this:GetName(), text )
	end
end

function ContainerFrame_Bull_Tex_OnEnter()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then return end
	local levelupDef = getLevelUP( mainplayer:getLv() )
	if levelupDef ~= nil then
		local leftNum = levelupDef.DailyUseBull - mainplayer:getDailyUsedBull()
		local szLeftNum
		if leftNum <= 0 then
			szLeftNum = "#cd22832" .. leftNum .. "#n"
		else
			szLeftNum = tostring( leftNum )
		end
		local text = string.format("当前人物等级：%d\n可使用的绑银：%s\n绑银使用上限：%d\n不能消耗绑银时会自动消耗银币",mainplayer:getLv(),szLeftNum,levelupDef.DailyUseBull);
		SetGameTooltips( this:GetName(), text )
	end
end

function StashFrame_Money_Tex_OnEnter()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then return end
	local levelupDef = getLevelUP( mainplayer:getLv() )
	if levelupDef ~= nil then
		local text = string.format("当前人物等级：%d\n仓库存储银币上限：%s",mainplayer:getLv(),levelupDef.MaxStashMoney);
		SetGameTooltips( this:GetName(), text )
	end
end

-------------------------------------------晶石提炼-----------------------------------------------
local t_Tilian = { 5250400, 5250402, 5250404, };--提炼器ID，以后根据需要添加
local lastTick = 0;

local function getCanUseItem( impLv )
	local idx = 1;
	for i=1, #t_Tilian do
		local itemDef = getItemDef( t_Tilian[i] );
		if itemDef.Result[0].ResultVal4 >= impLv then
			break;
		end
		idx = idx + 1;
	end
	return t_Tilian[idx];
end

function getResolveState()
	if IsEquipResolved or IsGemResolved then
		return true;
	end
	return false;
end

function PlayerContainerSparEpurate_onLoad()
	local szText = [[提炼能源晶石说明：
#ca09b82●提炼晶石需要用绿色以上品质的装备
#c0ad700●提炼不同品质的装备需不同的提炼器
#cffd200●晶石提炼成功后提炼装备将会被摧毁]];
	PlayerContainerSparEpurateFrameDescRich:SetText( szText, 255, 255, 190 );
	ItemFirstCallBack( "PlayerContainerSparEpurateFrame" , Save2SparEpurateFrame );
end

function PlayerContainerSparEpurate_onUpdate()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end;
	PlayerContainerSparEpurateFrameMoneyFont:SetText( MainPlayer:getMoney() );
	local money = tonumber( PlayerContainerSparEpurateFrameCostFont:GetText() );
	if MainPlayer:getMoney() < money then
		PlayerContainerSparEpurateFrameCostFont:SetTextColor( 255, 0, 0 );
		PlayerContainerSparEpurateFrameOkBtn:Disable();
		return;
	else
		PlayerContainerSparEpurateFrameCostFont:SetTextColor( 160, 160, 90 );
		PlayerContainerSparEpurateFrameOkBtn:Enable();
	end

	local btn = getglobal( "PlayerContainerSparEpurateFrameArmItem" );
	if btn:GetClientUserData( 3 ) == 0 then
		PlayerContainerSparEpurateFrameOkBtn:Disable();
		return;
	end

	btn = getglobal( "PlayerContainerSparEpurateFrameNeedItem" );
	if btn:GetClientUserData( 3 ) == 0 then
		PlayerContainerSparEpurateFrameOkBtn:Disable();
		return;
	end
end

function PlayerContainerSparEpurate_onShow()
	if not PlayerContainerResolveFrame:IsShown() then
		if not PlayerContainerFrame:IsShown() then
			PlayerContainerFrame:Show();
		end

		if PlayerContainerFrame:GetRealLeft() > 0 then
			local posX = PlayerContainerFrame:GetRealLeft() ;
			local posY = PlayerContainerFrame:GetRealTop();
			PlayerContainerSparEpurateFrame:SetPoint( "topright", "UIClient", "topleft", posX , posY );
		else
			local posX = PlayerContainerFrame:GetRealRight() ;
			local posY = PlayerContainerFrame:GetRealTop();
			PlayerContainerSparEpurateFrame:SetPoint( "topleft", "UIClient", "topleft", posX , posY );
		end
	else
		local posX = PlayerContainerResolveFrame:GetRealLeft();
		local posY = PlayerContainerResolveFrame:GetRealTop();
		PlayerContainerSparEpurateFrame:SetPoint( "topleft", "UIClient", "topleft", posX , posY );
		PlayerContainerResolveFrame:Hide();
	end
	PlayerContainerSparEpurateFrameGemResolveBtn:DisChecked();
	PlayerContainerSparEpurateFrameEquipResolveBtn:DisChecked();
	PlayerContainerSparEpurateFrameSparEpurateBtn:Checked();
	
	PlayerContainerSparEpurateFrameCostFont:SetText( 0 );
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer~= nil then
		PlayerContainerSparEpurateFrameMoneyFont:SetText( mainplayer:getMoney() );
	end
	PlayerContainerSparEpurateFrameOkBtn:Disable();
	PlayerContainerSparEpurateFrameSuccessFont:Hide();
end

function PlayerContainerSparEpurate_onHide()
	NormalItemTipsFrame:Hide();
	GameTooltip:Hide();
	ClearArmItem();
	ClearNeedItem();
	ClearSparItem();
	lastTick = 0;
end

function PlayerContainerSparEpurateTypeBtn_onClick()
	local equipResolveBtn = getglobal("PlayerContainerResolveFrameEquipResolveBtn");
	local gemResolveBtn = getglobal("PlayerContainerResolveFrameGemResolveBtn");
	local sparEpurateBtn = getglobal("PlayerContainerResolveFrameSparEpurateBtn");
	local name = this:GetName();
	if string.find( name, "Equip") then
		CUR_RESOLVE_TYPE ="Equip";
		equipResolveBtn:Checked();
		gemResolveBtn:DisChecked();
		sparEpurateBtn:DisChecked();
	elseif string.find( name, "Gem") then
		CUR_RESOLVE_TYPE = "Gem";
		equipResolveBtn:DisChecked();
		gemResolveBtn:Checked();
		sparEpurateBtn:DisChecked();
	else
		return;
	end
	PlayerContainerResolveFrame:Show();
end

function PlayerContainerSparEpurateOkBtn_onClick()
	if os.clock() - lastTick < 0.5 then
		return;
	end

	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local container	= mainplayer:getContainer();

	local btn = getglobal( "PlayerContainerSparEpurateFrameArmItem" );
	local armList = btn:GetClientUserData( 0 );
	local armGrid = btn:GetClientUserData( 1 );
	local armItem = container:getItem( armList, armGrid );
	btn = getglobal( "PlayerContainerSparEpurateFrameNeedItem" );
	local needList = btn:GetClientUserData( 0 );
	local needGrid = btn:GetClientUserData( 1 );
	local needItem = container:getItem( needList, needGrid );

	lastTick = os.clock();
	GameArmProce:requestSparEpurate( armItem:getItemWId(), armList, armGrid, needItem:getItemWId(), needList, needGrid );
end

function FindEpurateItem()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local container	= mainplayer:getContainer();
	local armImp = PlayerContainerSparEpurateFrameArmItem:GetClientUserData( 3 );
	local t_NeedItem = {};

	for i = 1, container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES ) do
		local item = container:getItem( CONTAINER_TYPE_SUNDRIES, i - 1 );
		local itemDef = item:getItemDef();
		if item:getItemId() ~= 0 and itemDef.Result[0].ResultID == RESULT_TILIAN and itemDef.Result[0].ResultVal4 >= armImp then
			table.insert( t_NeedItem, { lv = itemDef.Result[0].ResultVal4, rate = itemDef.Result[0].ResultVal1,
				list = item:getListType(), grid = item:getGridIdx(), id = itemDef.ItemID } );
		end
	end

	local btn = getglobal( "PlayerContainerSparEpurateFrameNeedItem" );
	if btn:GetClientUserData( 3 ) > 0 then
		UnLockItem( btn:GetClientUserData( 0 ), btn:GetClientUserData( 1 ) );
	end

	if #t_NeedItem > 0 then
		table.sort( t_NeedItem, function ( lhs, rhs )
			if lhs.lv == rhs.lv then
				return lhs.rate > rhs.rate;
			else
				return lhs.lv < rhs.lv;
			end
		end);

		btn:SetClientUserData( 0, t_NeedItem[1].list );
		btn:SetClientUserData( 1, t_NeedItem[1].grid );
		btn:SetClientUserData( 2, t_NeedItem[1].id );
		LockItem( t_NeedItem[1].list, t_NeedItem[1].grid, t_NeedItem[1].id );

		local itemDef = getItemDef( t_NeedItem[1].id );
		btn:SetClientUserData( 3, itemDef.Result[0].ResultVal4 );

		IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga"
		-- 如果没找到物品的图标则使用默认图标显示
		if not IsInExistence(IconPath) then
			IconPath = DEFAULT_ITEM_PATH
		end
		local icon = getglobal( btn:GetName() .. "IconTexture" );
		icon:SetTexture( IconPath );
		icon:SetGray( false );
		SetEquipImportantEffectUV( itemDef, btn:GetName() .."UVAnimationTex")
		local font = getglobal( btn:GetName() .. "CountFont" );
		font:SetText( container:getItemCount( t_NeedItem[1].id ) );
		font = getglobal( "PlayerContainerSparEpurateFrameCostFont" );
		font:SetText( itemDef.Result[0].ResultVal3 );
	else
		local itemID = getCanUseItem( armImp );
		btn:SetClientUserData( 0, 0 );
		btn:SetClientUserData( 1, 0 );
		btn:SetClientUserData( 2, itemID );
		btn:SetClientUserData( 3, 0 );

		local itemDef = getItemDef( itemID );
		IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga"
		-- 如果没找到物品的图标则使用默认图标显示
		if not IsInExistence(IconPath) then
			IconPath = DEFAULT_ITEM_PATH
		end
		local icon = getglobal( btn:GetName() .. "IconTexture" );
		icon:SetTexture( IconPath );
		icon:SetGray( true );
		SetEquipImportantEffectUV( itemDef, btn:GetName() .."UVAnimationTex")
		local font = getglobal( btn:GetName() .. "CountFont" );
		font:SetText( "" );
		font = getglobal( "PlayerContainerSparEpurateFrameCostFont" );
		font:SetText( itemDef.Result[0].ResultVal3 );
	end
end

function ClearArmItem()
	local btn = getglobal( "PlayerContainerSparEpurateFrameArmItem" );
	UnLockItem( btn:GetClientUserData( 0 ), btn:GetClientUserData( 1 ) );
	btn:SetClientUserData( 0, 0 );
	btn:SetClientUserData( 1, 0 );
	btn:SetClientUserData( 2, 0 );
	btn:SetClientUserData( 3, 0 );
	local icon = getglobal( btn:GetName() .. "IconTexture" );
	icon:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
	local uv = getglobal( btn:GetName() .."UVAnimationTex" );
	uv:Hide();
end

function ClearNeedItem()
	local btn = getglobal( "PlayerContainerSparEpurateFrameNeedItem" );
	UnLockItem( btn:GetClientUserData( 0 ), btn:GetClientUserData( 1 ) );
	btn:SetClientUserData( 0, 0 );
	btn:SetClientUserData( 1, 0 );
	btn:SetClientUserData( 2, 0 );
	btn:SetClientUserData( 3, 0 );
	local icon = getglobal( btn:GetName() .. "IconTexture" );
	icon:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
	local uv = getglobal( btn:GetName() .."UVAnimationTex" );
	uv:Hide();
	local font = getglobal( btn:GetName() .. "CountFont" );
	font:SetText( "" );
	font = getglobal( "PlayerContainerSparEpurateFrameCostFont" );
	font:SetText( 0 );
end

function ClearSparItem()
	PlayerContainerSparEpurateFrameSuccessFont:Hide();
	local btn = getglobal( "PlayerContainerSparEpurateFrameSparItem" );
	btn:SetClientUserData( 0, 0 );
	btn:SetClientUserData( 1, 0 );
	btn:SetClientUserData( 2, 0 );
	local icon = getglobal( btn:GetName() .. "IconTexture" );
	icon:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
	local uv = getglobal( btn:GetName() .."UVAnimationTex" );
	uv:Hide();
end

function SetArmItem( item )
	local itemDef = item:getItemDef();
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end;

	if item:getItemType() ~= ITEM_SLOTTYPE_AMM or itemDef.NoResolve == 1 or itemDef.Important == 0 then
		return ShowMidTips( "请放入可提炼的装备" );
	end

	if MainPlayer:getLv() + 20 < itemDef.WearPre.RoleLevelMin then
		ShowMidTips( "不可分解高于自身等级20级的装备" );
		return;
	end
	-- 装备未鉴定
	if item:isArmIdentity() then 
		ShowMidTips("此装备还未鉴定，不能进行此操作");
		return;
	end
	local btn = getglobal( "PlayerContainerSparEpurateFrameNeedItem" );
	local hadItem = false;
	if btn:GetClientUserData( 3 ) > 0 then
		if itemDef.Important > btn:GetClientUserData( 3 ) then
			return ShowMidTips( "装备品质与提炼器品质不符" );
		end
		hadItem = true;
	end

	ClearSparItem();

	btn = getglobal( "PlayerContainerSparEpurateFrameArmItem" );
	if btn:GetClientUserData( 3 ) > 0 then
		UnLockItem( btn:GetClientUserData( 0 ), btn:GetClientUserData( 1 ) );
	end
	btn:SetClientUserData( 0, item:getListType() );
	btn:SetClientUserData( 1, item:getGridIdx() );
	btn:SetClientUserData( 2, itemDef.ItemID );
	btn:SetClientUserData( 3, itemDef.Important );
	LockItem( item:getListType(), item:getGridIdx(), itemDef.ItemID );

	IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga"
	-- 如果没找到物品的图标则使用默认图标显示
	if not IsInExistence(IconPath) then
		IconPath = DEFAULT_ITEM_PATH
	end
	icon = getglobal( btn:GetName() .. "IconTexture" );
	icon:SetTexture( IconPath );
	SetEquipImportantEffectUV( itemDef, btn:GetName() .."UVAnimationTex");
	
	if not hadItem then
		FindEpurateItem();
	end
end

function SetNeedItem( item )
	local itemDef = item:getItemDef();
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end;
	local container	= MainPlayer:getContainer();

	if not CheckItemResult( itemDef, RESULT_TILIAN ) then
		return ShowMidTips( "请放入相应的提炼器" );
	end

	local btn = getglobal( "PlayerContainerSparEpurateFrameArmItem" );
	if btn:GetClientUserData( 3 ) > 0 then
		if itemDef.Result[0].ResultVal4 < btn:GetClientUserData( 3 ) then
			return ShowMidTips( "装备品质与提炼器品质不符" );
		end
	end

	ClearSparItem();

	btn = getglobal( "PlayerContainerSparEpurateFrameNeedItem" );
	if btn:GetClientUserData( 3 ) > 0 then
		UnLockItem( btn:GetClientUserData( 0 ), btn:GetClientUserData( 1 ) );
	end
	btn:SetClientUserData( 0, item:getListType() );
	btn:SetClientUserData( 1, item:getGridIdx() );
	btn:SetClientUserData( 2, itemDef.ItemID );
	btn:SetClientUserData( 3, itemDef.Result[0].ResultVal4 );
	LockItem( item:getListType(), item:getGridIdx(), itemDef.ItemID );

	IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga"
	-- 如果没找到物品的图标则使用默认图标显示
	if not IsInExistence(IconPath) then
		IconPath = DEFAULT_ITEM_PATH
	end
	local icon = getglobal( btn:GetName() .. "IconTexture" );
	icon:SetTexture( IconPath );
	icon:SetGray( false );
	SetEquipImportantEffectUV( itemDef, btn:GetName() .."UVAnimationTex")
	local font = getglobal( btn:GetName() .. "CountFont" );
	font:SetText( container:getItemCount( itemDef.ItemID ) );
	font = getglobal( "PlayerContainerSparEpurateFrameCostFont" );
	font:SetText( itemDef.Result[0].ResultVal3 );
end

function Save2SparEpurateFrame( item, nListType,nGridIndex )
	if GeniusMgr:getAllGeniusValue() < TILIAN_CAN_USE_LV then
		return ShowMidTips( string.format("需要总天赋值达到%d点才能使用晶石提炼", TILIAN_CAN_USE_LV) );
	end

	if item:getItemId() == 0 then return end;
	if item:getItemType() ~= ITEM_SLOTTYPE_AMM then
		SetNeedItem( item );
	else
		SetArmItem( item );
	end
end

function EpurateItem_onEnter()
	local nType = this:GetClientUserData( 0 );
	local nGrid = this:GetClientUserData( 1 );
	local id = this:GetClientUserData( 2 );
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end;
	local container	= MainPlayer:getContainer();
	local Item = container:getItem( nType, nGrid );
	if Item:getItemId() <= 0 then
		if string.find( this:GetName(), "ArmItem", 1, true ) then
			SetGameTooltips( this:GetName(), "请放入要提炼的装备" );
		elseif string.find( this:GetName(), "NeedItem", 1, true  ) then
			if id > 0 then
				local itemdef = getItemDef( id );
				ShowItemTips(LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, itemdef)
			else
				SetGameTooltips( this:GetName(), "请放入对应装备等级的提炼器" );
			end
		elseif string.find( this:GetName(), "SparItem", 1, true  ) then
			SetGameTooltips( this:GetName(), "提炼产生的晶石" );
		end
	else
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, Item, nil, this:GetName() );
	end
end

function EpurateItem_onLeave()
	NormalItemTipsFrame:Hide();
	GameTooltip:Hide();
end

function EpurateItem_onClick()
	if arg1 == "RightButton" then
		if this:GetClientID() == 1 then
			ClearArmItem();
		elseif this:GetClientID() == 2 then
			ClearNeedItem();
		else
			ClearSparItem();
		end
	end
end

function SetEpurateResult( nResult, nGrid )
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end;
	local container	= MainPlayer:getContainer();

	--道具消耗特效
	local modelview = getglobal( "PlayerContainerSparEpurateFrameNeedItemModelView" );
	modelview:setPetModel(0, 0, "effect\\entity\\baoshixiaohao.ent");
	modelview:setPetScale( 0, 0.4, 0.4, 0.4 );
	modelview:setEntityPosition(0, 0, 100, 0);
	if not modelview:IsShown() then
		modelview:Show()
	end

	if nResult > 0 then
		modelview = getglobal( "PlayerContainerSparEpurateFrameSparItemModelView" );
		modelview:setPetModel(0, 0, "effect\\entity\\fumo01.ent");
		modelview:setEntityPosition(0, 0, 100, 0);
		if not modelview:IsShown() then
			modelview:Show()
		end
		PlayerContainerSparEpurateFrameSuccessFont:Show();
		PlayerContainerSparEpurateFrameOkBtn:Disable();

		ClearArmItem();
		ClearNeedItem();

		local btn = getglobal( "PlayerContainerSparEpurateFrameSparItem" );
		btn:SetClientUserData( 0, CONTAINER_TYPE_SUNDRIES );
		btn:SetClientUserData( 1, nGrid );
		local Item = container:getItem( CONTAINER_TYPE_SUNDRIES, nGrid );
		local itemDef = Item:getItemDef();
		btn:SetClientUserData( 2, itemDef.ItemID );
		IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga"
		-- 如果没找到物品的图标则使用默认图标显示
		if not IsInExistence(IconPath) then
			IconPath = DEFAULT_ITEM_PATH
		end
		local icon = getglobal( btn:GetName() .. "IconTexture" );
		icon:SetTexture( IconPath );
		SetEquipImportantEffectUV( itemDef, btn:GetName() .."UVAnimationTex");
	else
		local btn = getglobal( "PlayerContainerSparEpurateFrameSparItem" );
		if btn:GetClientUserData( 2 ) > 0 then
			ClearSparItem();
		end

		btn = getglobal( "PlayerContainerSparEpurateFrameNeedItem" );
		if btn:GetClientUserData( 3 ) > 0 then
			local item = container:getItem( btn:GetClientUserData(0), btn:GetClientUserData(1) );
			local itemId = btn:GetClientUserData( 2 );
			local num = container:getItemCount( itemId );
			local font = getglobal( btn:GetName() .. "CountFont" );
			if item:getItemId() > 0 then
				font:SetText( num );
			else
				if container:getItemCount( itemId ) > 0 then
					for i = 1, container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES ) do
						item = container:getItem( CONTAINER_TYPE_SUNDRIES, i - 1 );
						if item:getItemId() == itemId then
							SetNeedItem( item );
							break;
						end
					end
					font:SetText( num );
				else
					FindEpurateItem();
				end
			end
		end
	end
end

function PlayerContainerResolveCannotEpurateBtn_onClick()
	return ShowMidTips( string.format("需要总天赋值达到%d点才能使用晶石提炼", TILIAN_CAN_USE_LV) );
end

--从提炼界面拖到背包
function ContainerSparEpurateDrag( szBtnName )
	local btn		=	getglobal( szBtnName );
	if btn:GetClientUserData( 3 ) <= 0 or btn:GetClientID() == 3 then
		return;
	end

	local MainPlayer	=	ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end;

	local container		=	MainPlayer:getContainer();
	local desList		=	btn:GetClientUserData( 0 );
	local desGrid		=	btn:GetClientUserData( 1 );
	local Item = container:getItem( desList, desGrid );
	if Item:getItemId() ~= 0 then
		UIMSG:getUIGV().DragFrom = DRAG_SPAR_EPURATE;
		local IconTex = getglobal(btn:GetName().."IconTexture");
		UIBeginDrag(IconTex:GetTexture(), desGrid, btn:GetClientID(), Item:getItemId() );
	end
end

--从提炼界面拖到背包接收
function ContainerRemoveSparEpurateItem( nIdx )
	if nIdx == 1 then
		ClearArmItem();
	elseif nIdx == 2 then
		ClearNeedItem();
	end
end

--从其他界面拖到提炼界面
function ReceiveContainerSparEpurate( szBtnName, dragfrom, dragdata1, dragdata2, dragdata3 )
	if GeniusMgr:getAllGeniusValue() < TILIAN_CAN_USE_LV then
		return ShowMidTips( string.format("需要总天赋值达到%d点才能使用晶石提炼", TILIAN_CAN_USE_LV) );
	end

	if this:GetClientID() == 3 then
		return;
	end

	local btn		=	getglobal( szBtnName );
	local MainPlayer	=	ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end;
	local container		=	MainPlayer:getContainer();
	--如果拖拽来自背包分解自身
	if dragfrom == DRAG_SPAR_EPURATE then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	elseif dragfrom == CONTAINER_TYPE_SUNDRIES then
		local srcList	=	dragfrom;
		local srcGrid	=	dragdata1;
		local item = container:getItem( srcList, srcGrid );
		if item:getItemId() == 0 then return end;
		if this:GetClientID() == 1 then
			SetArmItem( item );
		elseif this:GetClientID() == 2 then
			SetNeedItem( item );
		end
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	else
		ShowMidTips( "必须是背包中的物品才能进行提炼" );
		return;
	end
end

function ContainerFrame_VipShopBtn_OnClick()
	if not VipManager:isOwnVipFunc( STAR_RESULT_SHOP_TOLUA ) then
		return ShowMidTips( "没有【银星徽章】无法使用随身商店，[快捷键O]可查看徽章特权" );
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	if mainplayer:isDead() then
		return ShowMidTips( "你已经死亡" );
	end
	VipManager:openVipShop();
end

function ContainerFrame_VipShopBtn_OnEnter()
	SetGameTooltips( this:GetName(),  "银星徽章特权，远程使用NPC商店" );
end

function ContainerFrame_VipStashBtn_OnClick()
	if not VipManager:isOwnVipFunc( STAR_RESULT_STASH_TOLUA ) then
		return ShowMidTips( "没有【水晶徽章】无法使用随身仓库，[快捷键O]可查看徽章特权" );
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	if mainplayer:isDead() then
		return ShowMidTips( "你已经死亡" );
	end
	OpenStashFrame();
end

function ContainerFrame_VipStashBtn_OnEnter()
	SetGameTooltips( this:GetName(),  "水晶徽章特权，远程使用仓库服务" );
end

function ContainerFrame_VipStoreBtn_OnClick()
	if not VipManager:isOwnVipFunc( STAR_RESULT_STORE_TOLUA ) then
		return ShowMidTips( "没有【银星徽章】无法使用随身店铺，[快捷键O]可查看徽章特权" );
	end
	OpenVipStore();
end

function ContainerFrame_VipStoreBtn_OnEnter()
	SetGameTooltips( this:GetName(),  "银星徽章特权，远程使用店铺服务" );
end

---------------------------------------------银币获取引导------------------------------------------------

function ContainerFrame_MoneyBtn_OnClick()
	if not MoneyGuideFrame:IsShown() then
		MoneyGuideFrame:Show();
	else
		MoneyGuideFrame:Hide();
	end
end

function ContainerFrame_MoneyBtn_OnEnter()
	SetGameTooltips( this:GetName(), "教你轻松致富！" );
end

function MoneyGuideItem_OnEnter()
	local itemId = this:GetClientUserData( 0 );
	if itemId ~= 0 then
		local szGemData = "0,0";
		ShowItemTips(LUA_ITEMTIPS_TYPE_NORMAL,LUA_ITEMTIPS_ITEMDEF,getItemDef(itemId),nil, nil, nil, nil, szGemData);
	end
end

function MoneyGuideFrame_OnShow()
	MoneyGuideFrameItem1:SetClientUserData( 0, 5062000 );
	MoneyGuideFrameItem2:SetClientUserData( 0, 5060030 );
	this:SetPoint("topright","PlayerContainerFrame","topleft", 0, 0);
end

function SetPackageTradeItem( name, itemId )
	local btn = getglobal( name );
	local ItemTex = getglobal( name .. "IconTexture" );
	local ItemNum = getglobal( name .. "CountFont" );
	local ItemBoxTex = getglobal( name .. "BoxTexture" );
	local UVAnimationTex = getglobal( name .."UVAnimationTex" );
	if itemId then
		btn:SetClientUserData( 0, itemId );
		local itemDef = getItemDef( itemId );
		local IconPath = GetItemEquipIconPath() .. itemDef.IconID .. ".tga";
		-- 如果没找到物品的图标则使用默认图标显示
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH;
		end
		ItemTex:SetTexture( IconPath );
		ItemBoxTex:Show();
		SetEquipImportantEffectUV( itemDef, name .. "UVAnimationTex" );
	else
		btn:SetClientUserData( 0, 0 );
		ItemTex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
		UVAnimationTex:Hide();
		ItemBoxTex:Hide();
	end
	ItemNum:SetText( "" );
end