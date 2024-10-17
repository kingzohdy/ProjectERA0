local t_PetPhotoControl = { xieDaiPos = -1, selectPetIndex = -1 };
MAX_PET_PHOTO_ITEM_BTN = 2;
MAX_PET_SHI_BUFF_NUM = 5;

function initPetActorData()
	t_PetPhotoControl = { xieDaiPos = -1, selectPetIndex = -1 };
end

function GetFightPetXieDaiPos()
	return t_PetPhotoControl.xieDaiPos;
end

function UpdateTargetPetUI( nXieDaiPos )
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local nPetIndex		= -1;
	for i = 1, petPackage:getPetNum() do
		 if petPackage:isPetInState( i - 1, CLIENT_PET_STAT_FIGHT ) then
			nPetIndex = i - 1;
			break;
		 end
	end
	
	t_PetPhotoControl = { xieDaiPos = nXieDaiPos, selectPetIndex = nPetIndex };
	--print( "UpdateTargetPetUI--t_PetPhotoControl[selectPetIndex] = "..t_PetPhotoControl["selectPetIndex"].."--25" );
	--PetUI:Show();
	SetPetCampDialog();
	-- TODO: 控制所有显示的灰态图标
	UpdatePetPhoto();

	CheckFightPetPhotoXinShouGuid();

	CheckAutoRecoverPet();
end

function UpdatePetPhotoWhenGetNewPet()
	if PetUI:IsShown() then
		return;
	end
	
	-- 只有一只宠物，那么一定是第一次获得宠物
	t_PetPhotoControl = { xieDaiPos = -1, selectPetIndex = 0 };
	--print( "UpdatePetPhotoWhenGetNewPet--t_PetPhotoControl[selectPetIndex] = "..t_PetPhotoControl["selectPetIndex"].."--43" );
	PetUI:Show();
end

function UpdatePetPhotoWhenLoginGame()
	if PetUI:IsShown() then
		--print( "UpdatePetPhotoWhenLoginGame--selectPetIndex -- PetUI:IsShown()--50" );
		return;
	end
	
	local mainplayer	= ActorMgr:getMainPlayer();
	-- 判断宠物的状态
	local petPackage	= mainplayer:getPet();
	if petPackage:getPetNum() == 0 then
		--print( "UpdatePetPhotoWhenLoginGame--selectPetIndex -- petPackage:getPetNum() == 0--57" );
		return;
	end

	-- TODO:改成上一次在线选择的宠物
	-- 只有一只宠物，那么一定是第一次获得宠物
	local systemSetting	= GameMgr:getUserSystemConfig();
	
	t_PetPhotoControl["selectPetIndex"] = systemSetting:getGameOptValue( GAME_OPT_LAST_PET_SELECT );
	if t_PetPhotoControl["selectPetIndex"] < 0 then
		t_PetPhotoControl["selectPetIndex"] = 0;
	end 
	--print( "UpdatePetPhotoWhenLoginGame--selectPetIndex = "..t_PetPhotoControl["selectPetIndex"].."--69" );
	PetUI:Show();
end

function ValidateToShowPetPhoto()
	if PetUI:IsShown() then
		return;
	end
	
	local mainplayer	= ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	-- 判断宠物的状态
	local petPackage	= mainplayer:getPet();
	if petPackage:getPetNum() == 0 then
		return;
	end
	PetUI:Show();
end

function UpdatePetPhotoWhenDropPet( data )
	-- 若当前已经有宠物在出征，则丢弃哪只宠物都不用考虑
	if t_PetPhotoControl["xieDaiPos"] >= 0 then
		return;
	end
	
	local mainplayer = ActorMgr:getMainPlayer();
	local petPackage = mainplayer:getPet();
	if petPackage:getPetNum() == 0 then
		PetUI:Hide();
		return;
	end

	if data["oldSelectPetIndex"] == t_PetPhotoControl["selectPetIndex"] then
		t_PetPhotoControl["selectPetIndex"] = data["selectPetIndex"];
		--print( "UpdatePetPhotoWhenDropPet--selectPetIndex = "..t_PetPhotoControl["selectPetIndex"].."--86" );
	end

	-- TODO: 这做一次更新操作
	UpdatePetPhoto();
end

function SaveLastSelectPetInfo()
	local systemSetting	= GameMgr:getUserSystemConfig();
	systemSetting:setGameSaveValue( GAME_OPT_LAST_PET_SELECT, t_PetPhotoControl["selectPetIndex"] );
	systemSetting:saveGameOpt( false );
end

local t_modeDesc =  {	[PET_FIGHT_ACTIVE] = { desc = "被召唤宠物会获得杀怪经验\n宠物会攻击敌人，所有技能生效\n宠物会帮主人护卫，勇气有效", typeDesc = "主动模式",
												nextMode = "被动模式" },  
						[PET_FIGHT_PASSIVE] = { desc = "被召唤宠物会获得杀怪经验\n宠物不会被怪物攻击\n宠物不会攻击敌人，所有技能无效\n宠物不会帮主人护卫，勇气无效",
												typeDesc = "被动模式", nextMode = "主动模式" },
					};

local t_petShiPhotoEvents =	{	["GE_MAINPET_BUF_LIST_UPDATE"]	= {}, ["GE_UPDATE_MAIN_PET_FIGHT_MODE"] = {}, ["GE_MAIN_PET_CALLBACK"]	= {},
								["GE_CONTAINER_UPDATE"]			= {}, ["GE_MAIN_TRUMP_FIGHT_STATE_CHG"] = {}, ["GE_PET_PACKAGE_INIT"]	= {},
								["GE_MAIN_PET_HETI"]			= {},
							};

t_petShiPhotoEvents["GE_MAINPET_BUF_LIST_UPDATE"].func = 
function ()
	if not PetUI:IsShown() then
		return;
	end

	UpdatePetShiBuff();
end

t_petShiPhotoEvents["GE_PET_PACKAGE_INIT"].func = 
function ()
	--print( "t_petShiPhotoEvents--GE_PET_PACKAGE_INIT--132" );
	UpdatePetPhotoWhenLoginGame();
end

--[[
t_petShiPhotoEvents["GE_ENTER_PLAYERLOGIN"].func = 
function ()
	local systemSetting	= GameMgr:getUserSystemConfig();
	t_PetPhotoControl["selectPetIndex"] = systemSetting:getGameOptValue( GAME_OPT_LAST_PET_SELECT );
end
--]]

t_petShiPhotoEvents["GE_MAIN_PET_CALLBACK"].func = 
function ()
	if not PetUI:IsShown() then
		return;
	end
	--print( "t_petShiPhotoEvents--GE_PET_FIGHT_CHG--118" );
	UpdatePetTongLingState();
	UpdatePetShiBuff();
end

t_petShiPhotoEvents["GE_MAIN_TRUMP_FIGHT_STATE_CHG"].func = 
function ()
	t_petShiPhotoEvents["GE_MAIN_PET_CALLBACK"].func();
end

t_petShiPhotoEvents["GE_MAIN_PET_HETI"].func = 
function ()
	t_petShiPhotoEvents["GE_MAIN_PET_CALLBACK"].func();
end

function PrivateUpdatePetTongLingState_NoPetChuZheng()
	local btn	= getglobal( "PetUITongLingBtn" );
	btn:Hide();
end

function PrivateUpdatePetTongLingState_PetChuZheng()
	local btn	= getglobal( "PetUITongLingBtn" );
	local animTex		= getglobal( "PetUITongLingBtnUVAnimationTex" );
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local petInfo		= petPackage:getPetInfo( t_PetPhotoControl["selectPetIndex"] );
	if petInfo == nil then
		btn:Hide();
		return;
	end
	
	if not IsPetTongLing( petInfo ) then
		btn:Hide();
		return;
	end

	if btn:IsShown() then
		return;
	end

	animTex:SetUVAnimation( 80, true);
	animTex:Show();
	btn:Show();
end

local MAX_LIU_GUANG_EFFECT_TIME = 10 * 1000;
local t_PetShiUVAnimationTexControl = { startTime = 0, };

t_PetShiUVAnimationTexControl["start"] = 
function( self )
	self["startTime"] = GameMgr:getTickTime();
end

t_PetShiUVAnimationTexControl["isEnd"] = 
function( self )
	return ( GameMgr:getTickTime() - self["startTime"] ) > MAX_LIU_GUANG_EFFECT_TIME;
end

function PetShiUVAnimationTex_OnShow()
	t_PetShiUVAnimationTexControl:start();
end

function PetShiUVAnimationTex_OnUpdate()
	if t_PetShiUVAnimationTexControl:isEnd() then
		this:Hide();
	end
end

function UpdatePetTongLingState()
	if t_PetPhotoControl.xieDaiPos >= 0 then
		PrivateUpdatePetTongLingState_PetChuZheng();
	else
		PrivateUpdatePetTongLingState_NoPetChuZheng();
	end
end

t_petShiPhotoEvents["GE_UPDATE_MAIN_PET_FIGHT_MODE"].func = 
function ()
	SetPKBtnTex();
		
	if not isPointInFrame( "TargetPetPKBtn" ) or not GameTooltip:IsShown() then
		return;
	end

	local mainplayer = ActorMgr:getMainPlayer();
	local petPackage = mainplayer:getPet();
	local pet		 = petPackage:getPetActor( t_PetPhotoControl.xieDaiPos );
	
	local szText = "#R" .. t_modeDesc[pet:getPetFightMode()]["typeDesc"].."：" .. "\n#G"..t_modeDesc[pet:getPetFightMode()]["desc"].."\n#Y点击可切换为"..
					t_modeDesc[pet:getPetFightMode()]["nextMode"];
	

	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = "PetUI",
						button = "TargetPetPKBtn", horizonal = "right" } );
end

t_petShiPhotoEvents["GE_MAIN_PET_CALLBACK"].func = 
function ()
	local mainplayer = ActorMgr:getMainPlayer();
	local petPackage = mainplayer:getPet();
	local pet		 = petPackage:getPetActor( t_PetPhotoControl.xieDaiPos );
	if pet == nil or pet:isDead() then
		t_PetPhotoControl["xieDaiPos"] = -1;
	end
	
	local petInfo		= petPackage:getPetInfo( t_PetPhotoControl["selectPetIndex"] );
	local xinShouBtn	= getglobal( "PetUIXinShouBtn" );
	if xinShouBtn:IsShown() and xinShouBtn:GetClientString() == "TargetPetHP" then
		GameWizard:undoStep(GAMEWIZARD_FIRST_PET_HP_LOW);
		local t_hideIcons = { "PetUIXinShouBtn", "TargetPetHPUVAnimationTex" };
		util.HideIcons( t_hideIcons );
	end
	
	UpdatePetPhoto();
end

t_petShiPhotoEvents["GE_CONTAINER_UPDATE"].func = 
function ()
	local MainPlayer	= ActorMgr:getMainPlayer();
	local container		= MainPlayer:getContainer();

	for i = 1, MAX_PET_PHOTO_ITEM_BTN do
		local btn			= getglobal( "PetUIItemBtn"..i );
		local nItemID		= btn:GetClientUserData( 0 );
		local nItemCount	= container:getItemCount( nItemID );

		if nItemCount == 0 then
			local t_tgas = { [btn:GetName().."IconTexture"] = { path = "uires\\ui\\mask\\TouMingTongDao.tga" }, };
			util.SetTgas( t_tgas );
			btn:SetClientUserData( 0, 0 );
		end
	end
end

local BUFF_CLIENT		= 0;
local DIATHESIS_CLIENT	= 1;
local BUFF_GUAN_ZHI_CLIENT= 2;

function AddPetShiBuffer( data )
	local buffBtn	= getglobal( data["btn"] );
	local status	= data["buff"];
	buffBtn:SetClientUserData( 0, data["id"] );
	buffBtn:SetClientUserData( 1, data["level"] );
	buffBtn:SetClientUserData( 2, BUFF_CLIENT );
	buffBtn:SetClientUserData( 3, data["targetId"] );
	
	local statusDef		= getStatusDef( data["id"], data["level"] );
	local buffBackTex	= getglobal( buffBtn:GetName() .. "Ground" );
	buffBackTex:SetTexture( t_buffBackPath[statusDef.Type][statusDef.SubType] );

	local buffIcon = getglobal( buffBtn:GetName() .. "Icon" );
	buffIcon:SetTexture( GetBuffIconPath( statusDef.IconID ) );

	buffBtn:Show();
end

function AddPetShiGuanZhiBuffer( data )
	local buffBtn	= getglobal( data["btn"] );
	if buffBtn == nil then
		return;
	end
	local status	= data["buff"];
	buffBtn:SetClientUserData( 0, data["id"] );
	buffBtn:SetClientUserData( 1, data["level"] );
	buffBtn:SetClientUserData( 2, BUFF_GUAN_ZHI_CLIENT );
	buffBtn:SetClientUserData( 3, data["targetId"] );
	
	local statusDef		= getStatusDef( data["id"], data["level"] );
	local buffBackTex	= getglobal( buffBtn:GetName() .. "Ground" );
	buffBackTex:SetTexture( t_buffBackPath[statusDef.Type][statusDef.SubType] );

	local buffIcon = getglobal( buffBtn:GetName() .. "Icon" );
	buffIcon:SetTexture( GetBuffIconPath( statusDef.IconID ) );

	buffBtn:Show();
end

-- clientUserData - 2 = 1表示素质，为0表示buff
function AddPetShiJiaBuffer( data )
	-- TODO:  跟美术对路径
	local buffBtn	= getglobal( data["btn"] );
	buffBtn:SetClientUserData( 0, data["id"] );
	buffBtn:SetClientUserData( 1, data["level"] );
	buffBtn:SetClientUserData( 2, DIATHESIS_CLIENT );
	buffBtn:SetClientUserData( 3, data["targetId"] );
	
	local buffBackTex	= getglobal( buffBtn:GetName() .. "Ground" );
	buffBackTex:SetTexture( t_buffBackPath[BUFF_TYPE_GOOD][0] );	

	local diathesisDef = SkillMgr:getDiathesisDef( data["id"], data["level"] );
	local buffIcon	= getglobal( buffBtn:GetName() .. "Icon" );
	buffIcon:SetTexture( GetPetSkillIconPath( diathesisDef.IconID ) );	
	
	buffBtn:Show();
end

function ShowPetBuffTips( data )
	local nID		= data["id"];
	local nLv		= data["level"];
	local nOwnerId	= data["ownerId"];
	
	local mainplayer	= ActorMgr:getMainPlayer();	
	local petPackage	= mainplayer:getPet();
	local pet			= petPackage:getPetActor( t_PetPhotoControl.xieDaiPos );
	local BufStatus		= pet:getBufStatus();
	local buffData		= BufStatus:getBufStatus( nID, nLv );
	
	ShowBuffTips( { statusId = nID, level = nLv, totalTime = buffData:getLife(), startTime = 0, 
							buffButtonName = this:GetParent(), ownerId = nOwnerId, button = data["button"],
							parent = data["parent"], overlay = buffData:getOverlay() } );
end

function ShowPetDiathesisTips( data )
	local nID		= data["id"];
	local nLv		= data["level"];
	local nOwnerId	= data["ownerId"];
	local diathesisDef = SkillMgr:getDiathesisDef( nID, nLv );
	
	local szText = "#cff6e1c"..diathesisDef.Name.."#n\n".."#cffffff"..diathesisDef.DiathesisDesc.."#n";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = data["parent"],
						button = data["button"] } );

	SetGameTooltipRelFrame( { data["button"], data["parent"] } );
end

function ShowPetGuanZhiTips( data )
	local nID		= data["id"];
	local nLv		= data["level"];
	local nOwnerId	= data["ownerId"];
	local statusDef	= getStatusDef( nID, nLv );
	
	local szText = "#cff6e1c"..statusDef.Name.."#n\n".."#cffffff"..statusDef.StatusDesc.."#n";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = data["parent"],
						button = data["button"] } );

	SetGameTooltipRelFrame( { data["button"], data["parent"] } );
end

function PetUIPhotoBuffButton_OnEnter()
	local nBuffType = this:GetClientUserData( 2 );
	local data = { id = this:GetClientUserData( 0 ), level = this:GetClientUserData( 1 ), 
					ownerId = this:GetClientUserData( 3 ), button = this:GetName(), parent = this:GetParent() };
	if nBuffType == BUFF_CLIENT then
		ShowPetBuffTips( data );
	elseif nBuffType == BUFF_GUAN_ZHI_CLIENT then
		ShowPetGuanZhiTips( data );
	elseif nBuffType == DIATHESIS_CLIENT then
		ShowPetDiathesisTips( data );
	end
end

function PetUIPhotoBuffButton_OnLeave()
	GameTooltip:Hide();
end

function ClearPetShiBuff()
	for i = 1, MAX_PET_SHI_BUFF_NUM do
		DelCollBuff( "PetUIBuff" .. i );	
	end
end

function UpdatePetShiBuff()
	ClearPetShiBuff();
	local mainplayer	= ActorMgr:getMainPlayer();	
	local petPackage	= mainplayer:getPet();
	local pet			= petPackage:getPetActor( t_PetPhotoControl["xieDaiPos"] );
	local BufStatus		= pet:getBufStatus();

	local nBufBtnCount = 1;
	for i = 1, MAX_GOOD_STATUS do
		if nBufBtnCount > MAX_PET_SHI_BUFF_NUM then
			break;
		end

		-- 刷新目标头像的buf
		local Buf = BufStatus:getGoodStatus( i - 1 );
		if Buf:isShow() then
			AddPetShiBuffer( { btn = "PetUIBuff"..nBufBtnCount, id = Buf:getBufId(), level = Buf:getBufLv(), targetId = 0 } );
			nBufBtnCount = nBufBtnCount + 1;		
		end
	end

	-- 管制BUFF
	local petInfo			= petPackage:getPetInfo( t_PetPhotoControl["selectPetIndex"] );
	local guanZhiTrumpID	= GetPetTongLingTrumpID( petInfo, mainplayer );
	if guanZhiTrumpID ~= 0 then
		local t_oneTrumpGuanZhiConfig = t_petGuanZhiBuff[guanZhiTrumpID];
		if t_oneTrumpGuanZhiConfig ~= nil then
			AddPetShiGuanZhiBuffer( { btn = "PetUIBuff"..nBufBtnCount, id = t_oneTrumpGuanZhiConfig["StatusID"], 
								level = t_oneTrumpGuanZhiConfig["level"], targetId = 0 } );
			nBufBtnCount = nBufBtnCount + 1;
		end
	end

	for i=1, MAX_NOLIMIT_STATUS do
		if nBufBtnCount > MAX_PET_SHI_BUFF_NUM then
			break;
		end

		-- 刷洗目标头像的buf
		local Buf = BufStatus:getNolimitStatus( i - 1 );
		if Buf:isShow() then
			AddPetShiBuffer( { btn = "PetUIBuff"..nBufBtnCount, id = Buf:getBufId(), level = Buf:getBufLv(), targetId = 0 } );
			nBufBtnCount = nBufBtnCount + 1;		
		end
	end

	for i=1, MAX_BAD_STATUS do
		if nBufBtnCount > MAX_PET_SHI_BUFF_NUM then
			break;
		end

		-- 刷洗目标头像的buf
		local Buf = BufStatus:getBadStatus( i - 1 );
		if Buf:isShow() then
			AddPetShiBuffer( { btn = "PetUIBuff"..nBufBtnCount, id = Buf:getBufId(), level = Buf:getBufLv(), targetId = 0 } );
			nBufBtnCount = nBufBtnCount + 1;		
		end
	end

	-- 再查看是否有假buff
	local petDiathesis = pet:getPetDiathesis();
	for i = 1, petDiathesis.m_nNum do
		if nBufBtnCount > MAX_PET_SHI_BUFF_NUM then
			break;
		end
		
		local nId = petDiathesis.m_diathesisData[i-1].m_nID;
		local nLv = petDiathesis.m_diathesisData[i-1].m_nLevel;
		local diathesisDef = SkillMgr:getDiathesisDef( nId, nLv );
		if diathesisDef ~= nil and diathesisDef.IconPosition == 2 then
			AddPetShiJiaBuffer( { btn = "PetUIBuff"..nBufBtnCount, id = nId, level = nLv, targetId = 0 } );
			nBufBtnCount = nBufBtnCount + 1;	
		end
	end
end

-- 放宠物物品
function PetPhotoItemBtnEnterFunc( szBtnName )
	local btn		= getglobal( szBtnName );
	local nItemID	= btn:GetClientUserData( 0 );
	local MainPlayer= ActorMgr:getMainPlayer();
	local container	= MainPlayer:getContainer();
	local item		= container:getItem( nItemID );
	if item:getItemId() ~= 0 then
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item, PetPhotoItemBtnEnterFunc, szBtnName );
	end
end

function PetPhotoItemBtn_OnEnter()
	local nItemID = this:GetClientUserData( 0 );
	if nItemID == 0 then
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text ="可以放入宠物药水", 
							frame = this:GetParent(), button = this:GetName(), vertical = "bottom", horizonal = "right" } );
	else
		--PetPhotoItemBtnEnterFunc( this:GetName() );
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text ="左键点击使用", 
							frame = this:GetParent(), button = this:GetName(), vertical = "bottom", horizonal = "right" } );
	end
end

function PetPhotoItemBtn_OnLeave()
	local t_hideIcons = { "GameTooltip", "NormalItemTipsFrame", "ArmItemTipsFrame", "SecondArmItemTipsFrame", "SpellTipsFrame" };
	util.HideIcons( t_hideIcons );
end

function PetPhotoItemBtn_OnClick()
	local nItemID = this:GetClientUserData( 0 );
	if nItemID == 0 then
		return;
	end
	if arg1 == "LeftButton" then
		local MainPlayer= ActorMgr:getMainPlayer();
		local container	= MainPlayer:getContainer();
		local item		= container:getItem( nItemID );
		if t_PetPhotoControl["xieDaiPos"] < 0 then
			ShowMidTips( "当前没有宠物被召唤" );
			return;
		end

		container:usedItem( item:getListType(), item:getGridIdx(), 0, t_PetPhotoControl["selectPetIndex"] );
	elseif arg1 == "RightButton" then
		local t_tgas = { [this:GetName().."IconTexture"] = { path = "uires\\ui\\mask\\TouMingTongDao.tga" }, };
		util.SetTgas( t_tgas );
		this:SetClientUserData( 0, 0 );
	end
end

function PetPhotoItemBtn_BeginDrag()
end

function CanReceivePetPhotoItem( dragfrom, dragdata1, dragdata2, dragdata3 )
	local srcList		= dragfrom;					-- 源容器类型
	local srcGrid		= dragdata1;				-- 从源容器哪个格子过来
	-- 如果目标来自背包
	local t_acceptListType	= { CONTAINER_TYPE_SUNDRIES, CONTAINER_TYPE_TASK, CONTAINER_TYPE_STUFF };
	local bAcceptList		= false;
	for _, listType in ipairs( t_acceptListType ) do
		if listType == srcList then
			bAcceptList = true;
			break;
		end
	end

	if not bAcceptList then
		return false;
	end
	
	local MainPlayer	= ActorMgr:getMainPlayer();
	local container		= MainPlayer:getContainer();
	--print( "srcList = "..srcList.."， srcGrid = "..srcGrid );
	local item			= container:getItem( srcList, srcGrid );
	local itemDef		= item:getItemDef();
	
	if itemDef.UseTarget ~= USE_TARGET_PET then
		ShowMidTips( "不是宠物药水，不能放入" );
		return false;
	end
	
	local t_cannotUse= 
	{ 
			resultID	=	
			{	
				RESULT_PET_WASH_GENIUS ,		-- 宠物洗点道具
				RESULT_PET_ADD_ATTR ,			-- 增强宠物属性道具
				RESULT_PET_STRENGTHEN_STUFF ,	-- 宠物强化道具
				RESULT_PET_STRENGTHEN_SAFE ,	-- 宠物强化道具
				RESULT_PET_SKILL_BOOK ,			-- 宠物学习技能书
				RESULT_ADD_SKILL_GRID ,			-- 宠物增加技能格子的道具
				RESULT_PET_WASH_SKILL_GRID ,	-- 宠物洗技能格子道具
			},
	};

	-- 判定道具是否能够右键使用
	for _, resultID in ipairs( t_cannotUse.resultID ) do
		for i = 1, MAX_ITEM_RESULT do
			if itemDef.Result[i-1].ResultID == resultID then
				ShowMidTips( "不是宠物药水，不能放入" );
				return false;
			end
		end
	end

	return true;
end

function PetPhotoItemBtn_ReceiveDrag()
	local srcList	= UIMSG:getUIGV().DragFrom;					-- 源容器类型
	local srcGrid	= UIGetDragUserData1();				-- 从源容器哪个格子过来

	if not CanReceivePetPhotoItem( srcList, srcGrid, UIGetDragUserData2(), UIGetDragUserData3() ) then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	end
	
	local MainPlayer	= ActorMgr:getMainPlayer();
	local container		= MainPlayer:getContainer();
	local item			= container:getItem( srcList, srcGrid );
	local itemDef		= item:getItemDef();
	local szPath		= GetItemEquipIconPath()..itemDef.IconID..".tga";
	if not IsInExistence( szPath ) then
		szPath = DEFAULT_ITEM_PATH;
	end
	--print( "PetPhotoItemBtn_ReceiveDrag--szPath"..szPath );
	local t_tgas = { [this:GetName().."IconTexture"] = { path = szPath }, };
	util.SetTgas( t_tgas );
	local IconTex = getglobal( this:GetName().."IconTexture" );
	
	this:SetClientUserData( 0, itemDef.ItemID );
	UIEndDrag();
	UIMSG:getUIGV().DragFrom = DRAG_NONE;
	
	if isPointInFrame( this:GetName() ) then
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text ="左键点击使用", 
							frame = this:GetParent(), button = this:GetName(), vertical = "bottom", horizonal = "right" } );
	end
end

function PetShi_OnLoad()
	for event, _ in pairs( t_petShiPhotoEvents ) do
		this:RegisterEvent( event );
	end

	t_petShiPhotoEvents.__index = function ( tab, key )
								tab[key] = { func = function () end };
								return tab[key];
							end

	setmetatable( t_petShiPhotoEvents, t_petShiPhotoEvents );
end

function PetShi_OnEvent()
	t_petShiPhotoEvents[arg1].func();
end


function SetPKBtnTex()
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local nPetIndex		= getFightPetIndex();
	--local pet		= petPackage:getPetActor( t_PetPhotoControl.xieDaiPos );	
	local petInfo		= petPackage:getPetInfo( nPetIndex );
	local chkBtn		= getglobal( "TargetPetPKBtn" );
	if petInfo.Passive == PET_FIGHT_PASSIVE then		
		chkBtn:SetNormalTextureTemplate( "PkBtnPassiveNormalTemplate" );
		chkBtn:SetHighlightTextureTemplate( "PkBtnPassiveHighlightTemplate" );
		chkBtn:SetPushedTextureTemplate( "PkBtnPassivePushedTemplate" );		
	elseif petInfo.Passive == PET_FIGHT_ACTIVE then
		chkBtn:SetNormalTextureTemplate( "PkBtnActiveNormalTemplate" );
		chkBtn:SetHighlightTextureTemplate( "PkBtnActiveHighlightTemplate" );
		chkBtn:SetPushedTextureTemplate( "PkBtnActivePushedTemplate" );
	end
end

function SetListPKBtnTex( data )
	local chkBtn		= data["Btn"];
	local slider		= getglobal( "PetPhotoListFrame_ScrollBar" );
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local nPetIndex		= slider:GetValue() + chkBtn:GetClientID() - 1;
	local petInfo		= petPackage:getPetInfo( nPetIndex );
	if petInfo.Passive == PET_FIGHT_PASSIVE then
		chkBtn:SetNormalTextureTemplate( "PkBtnPassiveNormalTemplate" );
		chkBtn:SetHighlightTextureTemplate( "PkBtnPassiveHighlightTemplate" );
		chkBtn:SetPushedTextureTemplate( "PkBtnPassivePushedTemplate" );		
	elseif petInfo.Passive == PET_FIGHT_ACTIVE then
		chkBtn:SetNormalTextureTemplate( "PkBtnActiveNormalTemplate" );
		chkBtn:SetHighlightTextureTemplate( "PkBtnActiveHighlightTemplate" );
		chkBtn:SetPushedTextureTemplate( "PkBtnActivePushedTemplate" );
	end
	if nPetIndex == getFightPetIndex() then
		SetPKBtnTex();
	end
end

local t_petPhotoXinShouBtnPosInfo = {	["TargetPetPKBtn"] = { selfPoint = "TopLeft", relFrame = "TargetPetPKBtn", relPoint = "bottomleft", 
																offsetX = -15, offsetY = 20, },
							
										["TargetPetHP"] = { selfPoint = "TopLeft", relFrame = "TargetPetHP", relPoint = "bottomleft", 
																offsetX = 10, offsetY = 20, },
									};

function CheckFightPetPhotoXinShouGuid()
	-- 若第一次获得宠物
	local xinShouBtn = getglobal( "PetFrameXinShouBtn" );
	if not GameWizard:isDo(GAMEWIZARD_FIRST_PET_FIGHT) then
		GameWizard:doStep(GAMEWIZARD_FIRST_PET_FIGHT);
		--ShowPetPhotoXinShouBtn( { text = "点击这里，可切换宠物战斗模式", btn = "TargetPetPKBtn" } );
	end
end

function CheckFightPetPhotoHPXinShouGuid()
	-- 若第一次获得宠物
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local pet			= petPackage:getPetActor( t_PetPhotoControl.xieDaiPos );
	local bHpLow		= ( ( pet:getHP() / pet:getMaxHP() ) <= 0.5 );
	local xinShouBtn	= getglobal( "PetFrameXinShouBtn" );
	if bHpLow and not GameWizard:isDo(GAMEWIZARD_FIRST_PET_HP_LOW) then
		local xinShouBtn	= getglobal( "PetUIXinShouBtn" );
		if xinShouBtn:IsShown() and xinShouBtn:GetClientString() == "TargetPetHPn" then
			return;
		end

		GameWizard:doStep(GAMEWIZARD_FIRST_PET_HP_LOW);
		ShowPetPhotoXinShouBtn( { text = "宠物生命值太低了，需要使用宠物药水", btn = "TargetPetHP" } );
	end
end

function ShowPetPhotoXinShouBtn( data )
	local rich			= getglobal( "PetUIXinShouBtnRich" );
	local xinShouBtn	= getglobal( "PetUIXinShouBtn" );
	local nSelfDefineExtendWidth = 14;
	rich:SetClientString( data["text"] );
	ReSizeTooltip( rich:GetName(), xinShouBtn:GetName(), nSelfDefineExtendWidth );
	local posInfo = t_petPhotoXinShouBtnPosInfo[data["btn"]];
	xinShouBtn:SetPoint( posInfo["selfPoint"], posInfo["relFrame"], posInfo["relPoint"], posInfo["offsetX"], posInfo["offsetY"] );
	xinShouBtn:SetClientString( data["btn"] );
	
	local animationTexture = getglobal( data["btn"].."UVAnimationTex" );
	animationTexture:SetUVAnimation( 50, true );
	animationTexture:Show();
	
	xinShouBtn:Show();
end

local nChkSelPetIndex = -1;
function UpdatePetPhotoHavePetChuZheng()
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local pet		= petPackage:getPetActor( t_PetPhotoControl.xieDaiPos );
	PetPhoto:SetTexture( GetMonsterIconPath( pet:getPetIconId() ) );
	SetPetShiPhoto();
	UpdatePetShiBuff();
	TargetPetPKBtn:SetGray( false );
	TargetPetPKBtn:Enable();
	SetPKBtnTex();
end

function UpdatePetPhotoNoPetChuZheng()
	TargetPetPKBtn:SetGray( true );
	TargetPetPKBtn:Disable();	
	ClearPetShiBuff();

	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local petInfo		= petPackage:getPetInfo( t_PetPhotoControl["selectPetIndex"] );
	if petInfo == nil then
		return;
	end
	local petDef		= getPetDef( petInfo.PetID );
	PetPhoto:SetTexture( GetMonsterIconPath( petDef.IconID ) );
	SetPetShiPhoto();
end

function UpdatePetPhoto()
	local t_grayTex = { "TargetPetHPTex", "PetPhoto", "TargetPetEXPTex" };
	if t_PetPhotoControl.xieDaiPos >= 0 then
		local t_showIcons = { "TargetPetHPTex", "TargetPetEXPTex" };
		util.ShowIcons( t_showIcons );
		local t_hideIcons = { "TargetPetChuZhengBtn" };
		util.HideIcons( t_hideIcons );
		
		UpdatePetPhotoHavePetChuZheng();
		local mainplayer	= ActorMgr:getMainPlayer();
		local petPackage	= mainplayer:getPet();
		local isPetHeti		= petPackage:getPetHeti() ~= -1
		if isPetHeti then
			TargetPetZhaoHuiBtn:Hide()
			TargetPetNoHeTiBtn:Show()
		else
			TargetPetNoHeTiBtn:Hide()
			TargetPetZhaoHuiBtn:Show()
		end
		for _, name in ipairs( t_grayTex ) do
			local tex = getglobal( name );
			tex:SetGray( isPetHeti );
		end
	else
		local t_hideIcons = { "TargetPetZhaoHuiBtn", "TargetPetNoHeTiBtn", "TargetPetHPTex", "TargetPetEXPTex" };
		util.HideIcons( t_hideIcons );
		
		local t_showIcons = { "TargetPetChuZhengBtn" };
		util.ShowIcons( t_showIcons );
		
		if nChkSelPetIndex > -1 then
			local mainplayer	= ActorMgr:getMainPlayer();
			local petPackage	= mainplayer:getPet();
			local nPetIndex		= getFightPetIndex();
			local petInfo		= petPackage:getPetInfo( nChkSelPetIndex );
			local chkBtn		= getglobal( "TargetPetPKBtn" );
			if petInfo.Passive == PET_FIGHT_PASSIVE then		
				chkBtn:SetNormalTextureTemplate( "PkBtnPassiveNormalTemplate" );
				chkBtn:SetHighlightTextureTemplate( "PkBtnPassiveHighlightTemplate" );
				chkBtn:SetPushedTextureTemplate( "PkBtnPassivePushedTemplate" );		
			elseif petInfo.Passive == PET_FIGHT_ACTIVE then
				chkBtn:SetNormalTextureTemplate( "PkBtnActiveNormalTemplate" );
				chkBtn:SetHighlightTextureTemplate( "PkBtnActiveHighlightTemplate" );
				chkBtn:SetPushedTextureTemplate( "PkBtnActivePushedTemplate" );
			end
		end
		UpdatePetPhotoNoPetChuZheng();
		
		for _, name in ipairs( t_grayTex ) do
			local tex = getglobal( name );
			tex:SetGray( true );
		end
	end

	UpdatePetTongLingState();
end

function PetShi_OnShow()
	UpdatePetPhoto();
	
	for i = 1, MAX_PET_PHOTO_ITEM_BTN do
		local btn = getglobal( this:GetName().."ItemBtn"..i );
		btn:SetClientUserData( 0, 0 );

		local t_tgas = { [btn:GetName().."IconTexture"] = { path = "uires\\ui\\mask\\TouMingTongDao.tga" }, };
		util.SetTgas( t_tgas );
	end
end

function PetShi_OnUpdate()
	SetPetShiPhoto();	
end

function PetShi_OnHide()
	local t_hideIcons = { "TargetPetHPUVAnimationTex", "TargetPetPKBtnUVAnimationTex", "PetPhotoListFrame" };
	util.HideIcons( t_hideIcons );
	if GetCurPos() == "PETPHOTO" and PetDialogFrame:IsShown() then
		PetDialogFrame:Hide();
	end
	
	t_PetPhotoControl = { xieDaiPos = -1, selectPetIndex = -1 };
	--print( "PetShi_OnHide--selectPetIndex = "..t_PetPhotoControl["selectPetIndex"].."--726" );
end

local MAX_PET_PK_OP = 2;
local t_mode =  { ["主动模式"] = { mode = PET_FIGHT_ACTIVE },  ["被动模式"] = { mode = PET_FIGHT_PASSIVE }, };
function PetUI_PKRightOptionFrame_OnShow()
	for i = 1, MAX_PET_PK_OP do
		local tex = getglobal( "PetUI_PKRightBtn"..i.."Hook" );
		tex:Hide();
	end
	
	local mainplayer = ActorMgr:getMainPlayer();
	local petPackage = mainplayer:getPet();
	local pet		 = petPackage:getPetActor( t_PetPhotoControl.xieDaiPos );

	local t_rightOptions = { "主动模式", "被动模式" };
	for i = 1, table.getn( t_rightOptions ) do
		local font = getglobal( "PetUI_PKRightBtn"..i.."Font" );
		font:SetText( t_rightOptions[i] );
		font:SetTextColor( 145, 125, 55 );
		if pet:getPetFightMode() == t_mode[font:GetText()]["mode"] then
			local tex = getglobal( "PetUI_PKRightBtn"..i.."Hook" );
			tex:Show();
		end
	end
	util.AdjustTooltipsScreenPos( { tips = this:GetName() } );
end

function PetUIPhotoBtn_OnClick()
	if arg1 == "LeftButton" then
		SetTargetPetPhotoByClickPetPhoto();
		return;
	end

	if PetPhotoListFrame:IsShown() or arg1 ~= "RightButton" then
		return;
	end
	
	PetPhotoListFrame:Show();
	PetPhotoListFrame:SetPoint("TopLeft", this:GetName(),"bottomright",0,1);
--	util.AdjustBigtipsScreenPos( { tipsFrame = "PetPhotoListFrame", button = this:GetName() } );
end

function PetUIPhotoBtn_OnEnter()
	if PetPhotoListFrame:IsShown() then
		return;
	end

	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "右键点击打开宠物列表", frame = this:GetParent(),
						button = this:GetName(), horizonal = "right", vertical = "bottom" } );
end

function PetUIPhotoBtn_OnLeave()
	GameTooltip:Hide();
end

-- 打开宠物列表
function PetPhotoOpenListBtn_OnClick()
	local szText = "";
	if PetPhotoListFrame:IsShown() then
		PetPhotoListFrame:Hide();
		szText = "点击打开宠物列表";
	else
		PetPhotoListFrame:Show();
		szText = "点击关闭宠物列表";
	end
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName(), horizonal = "right" } );
end

function PetPhotoOpenListBtn_OnEnter()
	local szText = "";
	if PetPhotoListFrame:IsShown() then
		szText = "点击关闭宠物列表";
	else
		szText = "点击打开宠物列表";
	end

	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName(), horizonal = "right" } );
end

function PetPhotoOpenListBtn_OnLeave()
	GameTooltip:Hide();
end

-- 出征
function TargetPetChuZhengBtn_OnClick()
	if not CanCallPet( { petIndex = t_PetPhotoControl["selectPetIndex"] } ) then
		return;
	end

	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local petInfo		= petPackage:getPetInfo( t_PetPhotoControl["selectPetIndex"] );
	
	local t_PetRongHeProcessFrameControl = GetPetRongHeProcessFrameControl();
	if t_PetRongHeProcessFrameControl:isAnyPetRongHe() then
		ShowMidTips( "正在进行宠物融合，不能进行召唤" );
		return;
	end
	if petPackage:isExtraClanJiYangPet( petInfo ) then 
		ShowMidTips( "宠物正在寄养中，请前往公会城市取消寄养再进行该操作！" );
		return;
	end
	petPackage:petPre( petInfo.WID.nHeight, petInfo.WID.nLow );
end

function TargetPetChuZhengBtn_OnEnter()
	local szText = "点击召唤宠物";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName(), horizonal = "left" } );
end

function TargetPetChuZhengBtn_OnLeave()
	GameTooltip:Hide();
end

-- 解除合体
function TargetPetNoHeTiBtn_OnClick()
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local petInfo		= petPackage:getPetInfo( t_PetPhotoControl["selectPetIndex"] );
	
	local t_PetRongHeProcessFrameControl = GetPetRongHeProcessFrameControl();
	if t_PetRongHeProcessFrameControl:isAnyPetRongHe() then
		--ShowMidTips( "正在进行宠物融合，不能召回" );
		--return;
	end

	petPackage:petFreeHeti( petInfo.WID.nHeight, petInfo.WID.nLow );
end

function TargetPetNoHeTiBtn_OnEnter()
	local szText = "点击解除宠物合体";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName(), horizonal = "left" } );
end

function TargetPetNoHeTiBtn_OnLeave()
	GameTooltip:Hide();
end

-- 召回
function TargetPetZhaoHuiBtn_OnClick()
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local petInfo		= petPackage:getPetInfo( t_PetPhotoControl["selectPetIndex"] );
	
	local t_PetRongHeProcessFrameControl = GetPetRongHeProcessFrameControl();
	if t_PetRongHeProcessFrameControl:isAnyPetRongHe() then
		ShowMidTips( "正在进行宠物融合，不能召回" );
		return;
	end

	petPackage:petBackUp( petInfo.WID.nHeight, petInfo.WID.nLow );
end

function TargetPetZhaoHuiBtn_OnEnter()
	local szText = "点击召回宠物";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName(), horizonal = "left" } );
end

function TargetPetZhaoHuiBtn_OnLeave()
	GameTooltip:Hide();
end

function TargetPetPKBtn_OnClick()
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local nPetIndex		= getFightPetIndex();
	--local pet		= petPackage:getPetActor( t_PetPhotoControl.xieDaiPos );	
	local petInfo		= petPackage:getPetInfo( nPetIndex );
	--GameWizard:undoStep(GAMEWIZARD_FIRST_CHANGE_PET_MODE);
	if IsMessageBoxBlock(this) then
		return;
	end
	if not GameWizard:isDo(GAMEWIZARD_FIRST_CHANGE_PET_MODE) and petInfo.Passive == PET_FIGHT_ACTIVE then
		GameWizard:doStep(GAMEWIZARD_FIRST_CHANGE_PET_MODE);
		if MessageBoxBlock( "提示", "被动模式下，宠物无法攻击，技能无效，不会被怪物攻击。确定切换为被动模式？", this ) then
			petPackage:setPetFightMode( PET_FIGHT_PASSIVE, petInfo.WID.nHeight, petInfo.WID.nLow );
		end
	elseif GameWizard:isDo(GAMEWIZARD_FIRST_CHANGE_PET_MODE) then
		if petInfo.Passive == PET_FIGHT_ACTIVE then
			petPackage:setPetFightMode( PET_FIGHT_PASSIVE, petInfo.WID.nHeight, petInfo.WID.nLow );
		elseif petInfo.Passive == PET_FIGHT_PASSIVE then
			petPackage:setPetFightMode( PET_FIGHT_ACTIVE, petInfo.WID.nHeight, petInfo.WID.nLow );
		end
	else 
		if petInfo.Passive == PET_FIGHT_ACTIVE then
			petPackage:setPetFightMode( PET_FIGHT_PASSIVE, petInfo.WID.nHeight, petInfo.WID.nLow );
		elseif petInfo.Passive == PET_FIGHT_PASSIVE then
			petPackage:setPetFightMode( PET_FIGHT_ACTIVE, petInfo.WID.nHeight, petInfo.WID.nLow );
		end
	end

	SetPKBtnTex();
	PetPhotoListFrame:Hide();

	local xinShouBtn	= getglobal( "PetUIXinShouBtn" );
	if xinShouBtn:IsShown() and xinShouBtn:GetClientString() == this:GetName() then
		xinShouBtn:Hide();
		TargetPetPKBtnUVAnimationTex:Hide();
	end
end

function PetHP_OnClick()
	if arg1 == "LeftButton" then
		SetTargetPetPhotoByClickPetPhoto();
		return;
	end
end

-- @Desc:移入侍卫目标头像血条
function PetHP_OnEnter()
	if t_PetPhotoControl.xieDaiPos < 0 then
		return;
	end
	local szText = "生命值："..this:GetClientUserData(0).."/"..this:GetClientUserData(1);
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function PetHP_OnLeave()
	GameTooltip:Hide();
end

function TargetPetEXP_OnClick()
	if arg1 == "LeftButton" then
		SetTargetPetPhotoByClickPetPhoto();
		return;
	end
end

function TargetPetEXP_OnEnter()
	if t_PetPhotoControl.xieDaiPos < 0 then
		return;
	end

	local szText = "经验值："..this:GetClientUserData(0).."/"..this:GetClientUserData(1);
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function TargetPetEXP_OnLeave()
	GameTooltip:Hide();
end


function TargetPetPKBtn_OnEnter()
	local mainplayer = ActorMgr:getMainPlayer();
	local petPackage = mainplayer:getPet();
	local pet	 = petPackage:getPetActor( t_PetPhotoControl.xieDaiPos );
	local btn = getglobal( "TargetPetPKBtn" );
	btn:SetSize( 17, 17 );
	btn:SetOverlayRegion( -2, -2, 17, 17 );
	local szText = "#R" .. t_modeDesc[pet:getPetFightMode()]["typeDesc"].."：" .. "\n#G"..t_modeDesc[pet:getPetFightMode()]["desc"].."\n#Y点击可切换为"..
					t_modeDesc[pet:getPetFightMode()]["nextMode"];
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName(), horizonal = "right" } );	
	--TargetPetPKBtnUVAnimationTex:Show();
	--TargetPetPKBtnUVAnimationTex:SetUVAnimation( 50, true );
end

function TargetPetPKBtn_OnLeave()
	local btn = getglobal( "TargetPetPKBtn" );
	btn:SetSize( 13, 13 );
	btn:SetOverlayRegion( 0, 0, 13, 13 );
	--TargetPetPKBtnUVAnimationTex:Hide();
end

function PetShi_Test_OnShow()
	local animationTexture	= getglobal( "TargetPetHPUVAnimationTex" );
	animationTexture:SetUVAnimation( 50, true );
	animationTexture	= getglobal( "TargetPetPKBtnUVAnimationTex" );
	animationTexture:SetUVAnimation( 50, true );

	ShowPetPhotoXinShouBtn( { text = "测试显示效果", btn = "TargetPetPKBtn" } );
end

function PetShiXinShouBtn_OnHide()
	local animationTexture = getglobal( "TargetPetPKBtnUVAnimationTex" );
	animationTexture:Hide();

	local animationTexture = getglobal( "TargetPetHPUVAnimationTex" );
	animationTexture:Hide();
end

function SetTargetPetPhotoByClickPetPhoto()
	if t_PetPhotoControl.xieDaiPos < 0 then
		return;
	end

	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	if petPackage:getPetHeti() ~= -1 then
		return;
	end
	local PetActor		= petPackage:getPetActor( t_PetPhotoControl.xieDaiPos );
	if PetActor == nil then
		return;
	end

	mainplayer:SetSelectTarget( PetActor:GetID() );
end

-- @Desc:侍卫头像单击
function PetShiOnClick()
	if t_PetPhotoControl.xieDaiPos < 0 then
		return;
	end

	if arg1 == "RightButton" then
		PetUI_RightOptionFrame:Show();
	else
		local mainplayer	= ActorMgr:getMainPlayer();
		local petPackage	= mainplayer:getPet();
		if petPackage:getPetHeti() ~= -1 then
			return;
		end
		local PetActor		= petPackage:getPetActor( t_PetPhotoControl.xieDaiPos );
		if PetActor == nil then
			return;
		end
		mainplayer:SetSelectTarget( PetActor:GetID() );
	end
end

function SetPetShiPhotoHavePetChuZheng()
	-- 首先检测宠物
	local mainplayer	= ActorMgr:getMainPlayer();	
	local petPackage	= mainplayer:getPet();
	local pet			= petPackage:getPetActor( t_PetPhotoControl.xieDaiPos );
	if pet == nil or pet:isDead() then
		--this:Hide();
		return;
	end

	TargetPetLevel:SetText( pet:getLv() );
	TargetPetName:SetText( pet:getName() );
	TargetPetHP:SetClientUserData( 0, pet:getHP() );
	TargetPetHP:SetClientUserData( 1, pet:getMaxHP() );
	local scale = pet:getHP() / pet:getMaxHP();
	scale = scale > 1 and 1 or scale;
	local hpTex = getglobal( "TargetPetHPTex" );
	hpTex:SetSize( scale * 90, 6 );

	local xinShouBtn	= getglobal( "PetUIXinShouBtn" );
	if xinShouBtn:IsShown() and xinShouBtn:GetClientString() == "TargetPetHP" and scale > 0.5 then
		xinShouBtn:Hide();
		TargetPetHPUVAnimationTex:Hide();
	end
	
	local petInfo = petPackage:getPetInfo( pet:getRoleWId() );
	local petLevelDef = getPetLevelDef( petInfo.Level );
	TargetPetEXP:SetClientUserData( 0, petInfo.Exp );
	TargetPetEXP:SetClientUserData( 1, petLevelDef.LevelUpExp );
	scale = petInfo.Exp / petLevelDef.LevelUpExp;
	scale = scale > 1 and 1 or scale;
	local expTex = getglobal( "TargetPetEXPTex" );
	expTex:SetSize( scale * 90, 6 );

	CheckFightPetPhotoHPXinShouGuid();
end

function SetPetShiPhotoNoPetChuZheng()
	local mainplayer	= ActorMgr:getMainPlayer();	
	local petPackage	= mainplayer:getPet();
	
	local petInfo		= petPackage:getPetInfo( t_PetPhotoControl["selectPetIndex"] );
	if petInfo == nil then
		t_PetPhotoControl["selectPetIndex"] = 0
		petInfo = petPackage:getPetInfo( t_PetPhotoControl["selectPetIndex"] );
	end
	if petInfo == nil then
		t_PetPhotoControl["selectPetIndex"] = -1
		PetUI:Hide()
		return
	end
	
	TargetPetLevel:SetText( petInfo.Level );
	TargetPetName:SetText( petInfo.Name );
	TargetPetHP:SetClientUserData( 0,	petInfo.HP );
	TargetPetHP:SetClientUserData( 1,	petInfo.MaxHP );
	local scale = math.min( 1, petInfo.HP / petInfo.MaxHP );
	
	local hpTex = getglobal( "TargetPetHPTex" );
	hpTex:SetSize( scale * 90, 6 );

	local petLevelDef = getPetLevelDef( petInfo.Level );
	TargetPetEXP:SetClientUserData( 0, petInfo.Exp );
	TargetPetEXP:SetClientUserData( 1, petLevelDef.LevelUpExp );
	scale = math.min( 1, petInfo.Exp / petLevelDef.LevelUpExp );
	local expTex = getglobal( "TargetPetEXPTex" );
	expTex:SetSize( scale * 90, 6 );
end

function SetPetShiPhoto()
	if t_PetPhotoControl.xieDaiPos >= 0 then
		SetPetShiPhotoHavePetChuZheng();
	else
		SetPetShiPhotoNoPetChuZheng();
	end
end

-- 宠物头像宠物列表
MAX_PET_PHOTO_LIST_LEN = 4;

function ClearPetPhotoList()
	for i = 1, MAX_PET_PHOTO_LIST_LEN do
		local btn = getglobal( "PetPhotoListBtn"..i );
		btn:Disable();
		btn:Hide();
		local font = getglobal( btn:GetName().."Font" );
		font:SetText( "" );

		local pkBtn = getglobal( "PetPhotoListPkBtn"..i );
		--pkBtn:setCheckState( false );
		pkBtn:Hide();

		local chkBtn = getglobal( "PetPhotoListChkBtn"..i );
		chkBtn:setCheckState( false );
		chkBtn:Hide();
	end

	local t_hideIcons = { "PetPhotoListFrame_SliderBack", "PetPhotoListFrame_ScrollUpBtn", "PetPhotoListFrame_ScrollDownBtn", };
	util.HideIcons( t_hideIcons );
end

function UpdatePetPhotoList( nStartIndex )
	ClearPetPhotoList();
	local mainplayer = ActorMgr:getMainPlayer();
	local petPackage = mainplayer:getPet();
	local nIconIndex = 1;
	for i = nStartIndex, petPackage:getPetNum() do
		if nIconIndex > MAX_PET_PHOTO_LIST_LEN then
			break;
		end

		local petInfo = petPackage:getPetInfo( i - 1 );
		local btn = getglobal( "PetPhotoListBtn"..nIconIndex );
		btn:Enable();
		btn:Show();
		if isPointInFrame( btn:GetName() ) then
			ShowPetPhotoListChkBtnTips( { Btn = btn } );
		end
		

		local font = getglobal( btn:GetName().."Font" );
		font:SetText( petInfo.Name );

		local pkBtn = getglobal( "PetPhotoListPkBtn"..nIconIndex );
		SetListPKBtnTex({ Btn = pkBtn });
		if isPointInFrame( pkBtn:GetName() ) then
			ShowPetPhotoListPkBtnTips( { Btn = pkBtn } );
		end
		pkBtn:Show();

		local chkBtn = getglobal( "PetPhotoListChkBtn"..nIconIndex );
		if t_PetPhotoControl["selectPetIndex"] == ( i - 1 ) then
			chkBtn:setCheckState( true );
		end

		if isPointInFrame( chkBtn:GetName() ) then
			ShowPetPhotoListChkBtnTips( { Btn = chkBtn } );
		end

		chkBtn:Show();
		-- 被选定宠物要打钩
		nIconIndex = nIconIndex + 1;
	end

	if petPackage:getPetNum() > MAX_PET_PHOTO_LIST_LEN then
		local t_showIcons = { "PetPhotoListFrame_SliderBack", "PetPhotoListFrame_ScrollUpBtn", "PetPhotoListFrame_ScrollDownBtn", };
		util.ShowIcons( t_showIcons );
	end
	local nNum = math.min( MAX_PET_PHOTO_LIST_LEN, petPackage:getPetNum() );
	PetPhotoListFrame:SetHeight( nNum * 25 + ( nNum - 1 ) * 1 + 3 );
end

function ShowPetPhotoListChkBtnTips( data )
	local btn		= data["Btn"];
	local slider		= getglobal( "PetPhotoListFrame_ScrollBar" );
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local nPetIndex		= slider:GetValue() + btn:GetClientID() - 1;
	local petInfo		= petPackage:getPetInfo( nPetIndex );
	if petPackage:isPetInState( nPetIndex, CLIENT_PET_STAT_FIGHT ) then
		GameTooltip:Hide();
		return;
	end

	local szText = "点击召唤该宠物";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = btn:GetParent(),
						button = btn:GetName(), horizonal = "right" } );
end

function PetPhotoListChkBtn_OnEnter()
	ShowPetPhotoListChkBtnTips( { Btn = this } );	
end

function PetPhotoListChkBtn_OnLeave()
	GameTooltip:Hide();
end

function ShowPetPhotoListPkBtnTips( data )
	local btn		= data["Btn"];
	local slider		= getglobal( "PetPhotoListFrame_ScrollBar" );
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local nPetIndex		= slider:GetValue() + btn:GetClientID() - 1;
	local petInfo		= petPackage:getPetInfo( nPetIndex );

	local szText = "#R" .. t_modeDesc[petInfo.Passive]["typeDesc"].."：" .. "\n#G"..t_modeDesc[petInfo.Passive]["desc"].."\n#Y点击可切换为"..
					t_modeDesc[petInfo.Passive]["nextMode"];
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName(), horizonal = "right" } );
end

function PetPhotoListPkBtn_OnEnter()
	ShowPetPhotoListPkBtnTips( { Btn = this } );
end

function PetPhotoListPkBtn_OnClick()
	local slider		= getglobal( this:GetParent().."_ScrollBar" );
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local nPetIndex		= slider:GetValue() + this:GetClientID() - 1;
	local chkBtn		= getglobal( "PetPhotoListPkBtn"..this:GetClientID() ); 
	local petInfo		= petPackage:getPetInfo( nPetIndex );
	if IsMessageBoxBlock(this) then
		return;
	end
	if not GameWizard:isDo(GAMEWIZARD_FIRST_CHANGE_PET_MODE) and petInfo.Passive == PET_FIGHT_ACTIVE then
		GameWizard:doStep(GAMEWIZARD_FIRST_CHANGE_PET_MODE);
		if MessageBoxBlock( "提示", "被动模式下，宠物无法攻击，技能无效，不会被怪物攻击。确定切换为被动模式？", this) then
			petPackage:setPetFightMode( PET_FIGHT_PASSIVE, petInfo.WID.nHeight, petInfo.WID.nLow );
		end
	elseif GameWizard:isDo(GAMEWIZARD_FIRST_CHANGE_PET_MODE) then
		if petInfo.Passive == PET_FIGHT_ACTIVE then
			petPackage:setPetFightMode( PET_FIGHT_PASSIVE, petInfo.WID.nHeight, petInfo.WID.nLow );
		elseif petInfo.Passive == PET_FIGHT_PASSIVE then
			petPackage:setPetFightMode( PET_FIGHT_ACTIVE, petInfo.WID.nHeight, petInfo.WID.nLow );
		end
	else
		if petInfo.Passive == PET_FIGHT_ACTIVE then
			petPackage:setPetFightMode( PET_FIGHT_PASSIVE, petInfo.WID.nHeight, petInfo.WID.nLow );
		elseif petInfo.Passive == PET_FIGHT_PASSIVE then
			petPackage:setPetFightMode( PET_FIGHT_ACTIVE, petInfo.WID.nHeight, petInfo.WID.nLow );
		end
	end

	SetListPKBtnTex( { Btn = chkBtn } );
end

function PetPhotoListChkBtn_OnClick()
	local slider		= getglobal( this:GetParent().."_ScrollBar" );
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local nPetIndex		= slider:GetValue() + this:GetClientID() - 1;
	local chkBtn		= getglobal( "PetPhotoListChkBtn"..this:GetClientID() );
	local petInfo		= petPackage:getPetInfo( nPetIndex );
	if petPackage:isPetInState( nPetIndex, CLIENT_PET_STAT_FIGHT ) then
		ShowMidTips( "该宠物已经处于召唤状态" );
		chkBtn:setCheckState( true );
		return;
	end

	if not CanCallPet( { petIndex = nPetIndex } ) then
		if chkBtn:GetCheckState() and t_PetPhotoControl["selectPetIndex"] ~= nPetIndex then
			chkBtn:setCheckState( false );
		end
		
		return;
	end
	
	local t_PetRongHeProcessFrameControl = GetPetRongHeProcessFrameControl();
	if t_PetRongHeProcessFrameControl:isAnyPetRongHe() then
		ShowMidTips( "正在进行宠物融合，不能进行召唤" );
		return;
	end

	petPackage:petPre( petInfo.WID.nHeight, petInfo.WID.nLow );
	nChkSelPetIndex = nPetIndex;
	PetPhotoListFrame:Hide();
	
	t_PetPhotoControl = { xieDaiPos = -1, selectPetIndex = nPetIndex};
	--print( "PetPhotoListChkBtn_OnClick--selectPetIndex = "..t_PetPhotoControl["selectPetIndex"].."--1177" );
	UpdatePetPhoto();
end

function PetPhotoListBtn_OnClick()
end

local t_PetPhotoListFrameEvents =	{ ["GE_DEL_PET"] = {},	["GE_GET_NEW_PET"] = {},
									};

t_PetPhotoListFrameEvents["GE_DEL_PET"].func = 
function ()
	Private_UpdateSelectPetByDelPet();
	--print( "t_PetPhotoListFrameEvents--GE_DEL_PET--selectPetIndex = "..t_PetPhotoControl["selectPetIndex"].."--1236" );
	if not PetPhotoListFrame:IsShown() then
		return;
	end

	local slider = getglobal( "PetPhotoListFrame_ScrollBar" );	
	UpdatePetPhotoList( slider:GetValue() + 1 );
end

function Private_UpdateSelectPetByDelPet()
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local petInfo		= petPackage:getPetInfo( t_PetPhotoControl["selectPetIndex"] );
	if petInfo ~= nil then
		return;
	end

	local nPetIndex	= 0;
	for i = 1, petPackage:getPetNum() do
		 if petPackage:isPetInState( i - 1, CLIENT_PET_STAT_FIGHT ) then
			nPetIndex = i - 1;
			break;
		 end
	end
	

	t_PetPhotoControl["selectPetIndex"] = nPetIndex;
end

t_PetPhotoListFrameEvents["GE_GET_NEW_PET"].func = 
function ()
	t_PetPhotoListFrameEvents["GE_DEL_PET"].func();
end

function PetPhotoListFrame_OnLoad()
	for event, _ in pairs( t_PetPhotoListFrameEvents ) do
		this:RegisterEvent( event );
	end

	t_PetPhotoListFrameEvents.__index = function ( tab, key )
									tab[key] = { func = function () end };
									return tab[key];
								end
	setmetatable( t_PetPhotoListFrameEvents, t_PetPhotoListFrameEvents );
end

function PetPhotoListFrame_OnEvent()
	t_PetPhotoListFrameEvents[arg1].func();
end

function PetPhotoListFrame_OnShow()
	local slider = getglobal( this:GetName().."_ScrollBar" );	
	slider:SetValue( 0 );
	
	local mainplayer = ActorMgr:getMainPlayer();
	local petPackage = mainplayer:getPet();
	local nIconIndex = 1;
	SetScrollBar( slider:GetName(), MAX_PET_PHOTO_LIST_LEN, petPackage:getPetNum() );

	UpdatePetPhotoList( slider:GetValue() + 1 );
end

function PetPhotoListFrame_ScrollUpBtn_OnClick()
	local slider = getglobal( this:GetParent().."_ScrollBar" );
	if not slider:IsShown() then
		return;
	end

	local nValue = slider:GetValue() - slider:GetValueStep();
	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end

function PetPhotoListFrame_ScrollDownBtn_OnClick()
	local slider = getglobal( this:GetParent().."_ScrollBar" );
	if not slider:IsShown() then
		return;
	end

	local nValue	= slider:GetValue() + slider:GetValueStep();
	local nMaxValue = slider:GetMaxValue();
	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

function PetPhotoListFrame_ScrollBar_OnValueChanged()
	local slider	= getglobal( this:GetParent().."_ScrollBar" );
	local nValue	= slider:GetValue();
	local nMaxValue = slider:GetMaxValue();

	if nValue <= nMaxValue then
		UpdatePetPhotoList( slider:GetValue() + 1 );
	end
end

local t_petPhotoRightEvent = 
{
	["召回"]	= {},
	["驯服"]	= {},
};

t_petPhotoRightEvent["召回"].func =
function ()
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local petInfo;
	for i = 1, petPackage:getPetNum() do
		 petInfo = petPackage:getPetInfo( i - 1 );
		 if petPackage:isPetInState( i - 1, CLIENT_PET_STAT_FIGHT ) then
			petPackage:petBackUp( petInfo.WID.nHeight, petInfo.WID.nLow );
			return;
		 end
	end

	assert( false, "没有及时关闭宠物头像面板" );
end

t_petPhotoRightEvent["驯服"].func = 
function ()
	local nItemGrid = GetXunFuItemGirdx();
	if nItemGrid == -1 then
		ShowMidTips("缺少驯兽糖果或驯兽鞭子，无法驯服宠物来增加忠诚度");
		return;
	end
	
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local petPackage	= mainplayer:getPet();
	local nPetIndex = -1;
	for i = 1, petPackage:getPetNum() do
		local  petInfo = petPackage:getPetInfo( i - 1 );
		 if petPackage:isPetInState( i - 1, CLIENT_PET_STAT_FIGHT ) then
			container:usedItem( CONTAINER_TYPE_SUNDRIES, nItemGrid, 0, i - 1 );
			return;
		 end
	end
end

function PetUI_RightOptionFrame_OnLoad()
	local t_rightContent = { "召回", "驯服" };
	for index, content in ipairs( t_rightContent ) do
		local font = getglobal( "PetUI_RightBtn"..index.."Font" );
		font:SetText( content );
	end
end

-- 宠物头像右键选项
function PetUI_RightOptionFrame_OnShow()
	util.AdjustTooltipsScreenPos( { tips = this:GetName() } );
end

function PetUI_RightBtn_OnClick()
	local font = getglobal( this:GetName() .. "Font" );
	t_petPhotoRightEvent[font:GetText()].func();
	PetUI_RightOptionFrame:Hide();
end

function PetUI_PKRightBtn_OnClick()
	PetUI_PKRightOptionFrame:Hide();
	local font = getglobal( this:GetName() .. "Font" );
	local mainplayer = ActorMgr:getMainPlayer();
	local petPackage = mainplayer:getPet();
	local pet	 = petPackage:getPetActor( t_PetPhotoControl["xieDaiPos"] );
	local petInfo	 = petPackage:getPetInfo( t_PetPhotoControl["xieDaiPos"] );

	local nSelMode	 = t_mode[font:GetText()]["mode"];
	if nSelMode == petInfo.Passive then
		return;
	end

	PetPhotoListFrame:Hide();
	petPackage:setPetFightMode( nSelMode, petInfo.WID.nHeight, petInfo.WID.nLow );	

	--[[
	if nSelMode == pet:getPetFightMode() then
		return;
	end

	petPackage:setPetFightMode( nSelMode );
	--]]
end

function PetUI_RightBtn_OnEnter()
	local font = getglobal( this:GetName() .. "Font" );
	font:SetTextColor( 255, 254, 189 );
end

function PetUI_RightBtn_OnLeave()
	local font = getglobal( this:GetName() .. "Font" );
	font:SetTextColor( 145, 125, 55 );
end