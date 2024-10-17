local nShowDialogTime;
local DIALOG_SHOW_TIME;

local stand_lastposx;
local stand_startclock;
local bShowDialog;
local dialog_startclock;
local DELAY_TIME;
local szCurPos;
local pCampPetActor;

function initPetDialogData()
	nShowDialogTime		= 0;
	DIALOG_SHOW_TIME	= 8;
	stand_lastposx		= 0;
	stand_startclock	= 0;
	bShowDialog			= false;
	dialog_startclock	= 0;
	DELAY_TIME			= 1;
	szCurPos			= "";
	pCampPetActor		= nil;
end

-- 消息事件列表
local t_PetDialogEvent	=	{	["UI_ACTOR_ATTRIBUTE_CHG"] = {}, ["GE_PET_DEAD"] = {}, ["GE_PET_LEVELUP"] = {} }

-- 宠物对话的类型及相对位置
local t_PetDialogPos = {	["PETPHOTO"] = {}, ["PETHEAD"] = {}, ["PETBUTTON"] = {}, ["CHATPOP"] = {}, ["CHATTIP"] = {} }

-- 收到属性变化消息时处理
t_PetDialogEvent.UI_ACTOR_ATTRIBUTE_CHG.func = 
function ()
	if GetPetDialog("PlayerHpLow") == nil then
		return;
	end

	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end
	local nScale = MainPlayer:getHP() * 100/ MainPlayer:getMaxHP();
	local szText = "";
	for _, data in ipairs( GetPetDialog("PlayerHpLow").text ) do
		if nScale <= data.maxhp and nScale >= data.minhp then
			szText = data.text;
			break;
		end
	end
	t_PetDialogPos[GetPetDialog("PlayerHpLow").showtype].func(GetPetDialog("PlayerHpLow").showpos, GetPetDialog("PlayerHpLow").rate, szText);
	GetPetDialog("PlayerHpLow").startclock = os.clock();
end

-- 收到精灵属性变化消息时处理
t_PetDialogEvent.GE_PET_DEAD.func = 
function ()
	-- 宠物死亡 对话
	SetPetDeadDialog();
end

-- 收到精灵等级变化消息时处理
t_PetDialogEvent.GE_PET_LEVELUP.func = 
function ()
	if GetPetDialog("PetLevelUp") ~= nil and GetPetDialog("PetLevelUp")[nLv] ~= nil then
		t_PetDialogPos[ GetPetDialog("PetLevelUp")[nLv].showtype].func( GetPetDialog("PetLevelUp")[nLv].showpos, GetPetDialog("PetLevelUp")[nLv].rate, GetPetDialog("PetLevelUp")[nLv].text );
		GetPetDialog("PetLevelUp").startclock = os.clock();
	end
end

-- 宠物升级对话
function PetLevelUp( nLv )
	if GetPetDialog("PetLv") ~= nil and GetPetDialog("PetLevelUp") == nil then
		return;
	end
	local nRandomIndex = getRand( 1, table.getn( GetPetDialog("PetLevelUp").text ) );
	t_PetDialogPos[ GetPetDialog("PetLevelUp").showtype].func( GetPetDialog("PetLevelUp").showpos, GetPetDialog("PetLevelUp").rate, GetPetDialog("PetLevelUp").text[nRandomIndex] );
	GetPetDialog("PetLevelUp").startclock = os.clock();

	if GetPetDialog("PetLv")[nLv] ~= nil then
		nRandomIndex =getRand( 1, table.getn( GetPetDialog("PetLv")[nLv].text ) );
		t_PetDialogPos[ GetPetDialog("PetLv")[nLv].showtype].func( GetPetDialog("PetLv")[nLv].showpos, GetPetDialog("PetLv")[nLv].rate, GetPetDialog("PetLv")[nLv].text[nRandomIndex] );
		GetPetDialog("PetLv")[nLv].startclock = os.clock();
	end
end

t_PetDialogPos.PETPHOTO.func = 
function ()
	PetDialogFrameGuide:SetPoint( "TOPLEFT", "$parent", "TOPLEFT", 185, 130 );
	PetDialogFrameGuideTex:SetTexture("uires\\ui\\XiaoXiPaoPao\\Shang  2.tga");
	PetDialogFrameGuideTex:SetPoint("BOTTOMLEFT","PetDialogFrameGuide","TOPLFET",50,0);
	if not PetUI:IsShown() then
		PetDialogFrame:Hide();
		PetDialogFrameGuide:Hide();
	end
end

t_PetDialogPos.PETBUTTON.func = 
function ()
	PetDialogFrameGuideTex:SetTexture("uires\\ui\\XiaoXiPaoPao\\Xia.tga");
	PetDialogFrameGuideTex:SetPoint("BOTTOMLEFT","PetDialogFrameGuide","BOTTOMLEFT",5,15);
	local szRef = "PetButton";
	PetDialogFrameGuide:SetPoint("BOTTOMLEFT", szRef, "TOPLEFT", 10, -15);
end

-- TIP类的宠物对话处理
t_PetDialogPos.CHATTIP.func = 
function ( pos, rate, szTalk )
	if not bShowDialog or szTalk == nil or szTalk == "" or rate < getRand(1,100) then
		return;
	end
	if pos == "PETPHOTO" and not isAnyPetCamp() then
		return;
	end
	local systemSetting = GameMgr:getUserSystemConfig();
	if pos == "PETPHOTO" and systemSetting:getGameOptValue( GAME_OPT_SHOW_OTHERPLAYER_TALK_POP ) == 0 then
		return;
	end
	
	szCurPos = pos;

	local frameBtn			= getglobal( "PetDialogFrameGuide");
	local frameBtnRich		= getglobal( "PetDialogFrameGuideFont");

	frameBtnRich:SetClientString( szTalk );
	ReSizeTooltip( frameBtnRich:GetName(), frameBtn:GetName(), 10 );

	t_PetDialogPos[pos].func();
	frameBtn:Show();
	PetDialogFrame:Show();
end

-- 聊天泡泡类的宠物对话处理
t_PetDialogPos.CHATPOP.func = 
function( pos, rate, szTalk )
	local Mainplayer		= ActorMgr:getMainPlayer();
	if Mainplayer == nil or not bShowDialog  or rate < getRand(1,100) or szTalk == "" then
		return;
	end

	local pPetPackage	= Mainplayer:getPet();
	pCampPetActor		= pPetPackage:getPetActor( GetFightPetXieDaiPos() );
	if pCampPetActor == nil or GetFightPetXieDaiPos() < 0 then
		return;
	end
	GameWizard:setNpcTalkByActorId( pCampPetActor:GetID(), szTalk, "ChatPop", false, 0, 0, "PopBackArrowTexture", true,0,0,0,8 );
end

function GetCampPetActorId()
	local Mainplayer	= ActorMgr:getMainPlayer();
	if Mainplayer == nil then
		return 0;
	end
	local pPetPackage	= Mainplayer:getPet();
	pCampPetActor		= pPetPackage:getPetActor( GetFightPetXieDaiPos() );
	if pCampPetActor ~= nil then
		return pCampPetActor:getResId();
	end
	return 0;
end

function GetCurPos()
	return szCurPos;
end

-- 设置宠物出场对话
function SetPetCampDialog()
	if GetPetDialog("PetCamp") == nil then
		return;
	end
	local nRandomIndex = getRand( 1, table.getn( GetPetDialog("PetCamp").text ) );
	t_PetDialogPos[GetPetDialog("PetCamp").showtype].func(GetPetDialog("PetCamp").showpos, GetPetDialog("PetCamp").rate, GetPetDialog("PetCamp").text[nRandomIndex]);
	GetPetDialog("PetCamp").startclock = os.clock();
end

-- 设置宠物死亡对话
function SetPetDeadDialog()
	if GetPetDialog("PetDead") == nil then
		return;
	end
	local nRandomIndex = getRand( 1, table.getn( GetPetDialog("PetDead").text ) );
	t_PetDialogPos[GetPetDialog("PetDead").showtype].func(GetPetDialog("PetDead").showpos, GetPetDialog("PetDead").rate, GetPetDialog("PetDead").text[nRandomIndex]);
	GetPetDialog("PetDead").startclock = os.clock();
end

-- 设置人物升到特定等级时宠物对话
function SetPlayerLevelUpDialog( nLv )
	if  GetPetDialog("PlayerLv") ~= nil and GetPetDialog("PlayerLv")[nLv] ~= nil then
		t_PetDialogPos[ GetPetDialog("PlayerLv")[nLv].showtype].func( GetPetDialog("PlayerLv")[nLv].showpos, GetPetDialog("PlayerLv")[nLv].rate, GetPetDialog("PlayerLv")[nLv].text );
		GetPetDialog("PlayerLv").startclock = os.clock();
	end
end

-- 设置宠物忠诚度变化时宠物对话
LOW_ZHOGN_CHNEG = 30;
local t_wizardXunFuPet = { highID = 0, lowID = 0 };

function initWizardXunFuPet()
	t_wizardXunFuPet = { highID = 0, lowID = 0 };
end

function PetHPChg( nHP, nPetHeighId, nPetLowId )
	local t_PetAutoRecoverFrameControl = GetPetAutoRecoverFrameControl();
	--t_PetAutoRecoverFrameControl:checkHP( { lowID = nPetLowId, heighID = nPetHeighId, value = nHP } );
	t_PetAutoRecoverFrameControl:CheckWhenHPChange();
end


function PetLoveValChg( nLoveval, nPetHeighId, nPetLowId )
	local t_PetAutoRecoverFrameControl = GetPetAutoRecoverFrameControl();
	t_PetAutoRecoverFrameControl:CheckWhenZhongChengChange();

	local bZhongChentLow = ( nLoveval / 100 <= LOW_ZHOGN_CHNEG );
	if bZhongChentLow and not GameWizard:isDo(GAMEWIZARD_FIRST_BLINK_PET_BTN_WHEN_LOW) and 
		t_wizardXunFuPet["highID"] == 0 and t_wizardXunFuPet["lowID"] == 0 then
		GameWizard:doStep(GAMEWIZARD_FIRST_BLINK_PET_BTN_WHEN_LOW);
		t_wizardXunFuPet = { highID = nPetHeighId, lowID = nPetLowId };
		GuideEffectShow( "", "", "PetButton", "宠物忠诚度太低\n#P快捷键（X）", "uires\\ZhuJieMian\\1.dds", 352, 609, 60, 60, 50, 60, -9, 5, 7, -58, true );
	end

	local petXinShouBtn = getglobal( "PetFrameXinShouBtn" );
	
	if petXinShouBtn:IsShown() and petXinShouBtn:GetClientString() == "PetFrame_TameBtn" and 
		nPetHeighId == t_wizardXunFuPet["highID"] and nPetLowId == t_wizardXunFuPet["lowID"] and
		( nLoveval / 100 >= 50 ) then
		
		local t_hideIcons = { "PetFrameXinShouBtn", "PetFrame_TameBtnUVAnimationTex" };
		util.HideIcons( t_hideIcons )
		GameWizard:doStep(GAMEWIZARD_FIRST_CLICK_PET_ZHONG_CHENG_BTN_WHEN_LOW);

		if not GameWizard:isDo(GAMEWIZARD_FIRST_CLOSE_PET_ZHONG_CHENG_BTN_TIPS) then 
			GameWizard:doStep(GAMEWIZARD_FIRST_CLOSE_PET_ZHONG_CHENG_BTN_TIPS);
		end
		
		initWizardXunFuPet();
	end

	if GetPetDialog("PetLoveval") == nil then
		return;
	end

	for _, data in ipairs( GetPetDialog("PetLoveval").text ) do
		if nLoveval <= data.maxloveval and nLoveval >= data.minloveval then
			local szText = data.text;
			t_PetDialogPos[GetPetDialog("PetLoveval").showtype].func(GetPetDialog("PetLoveval").showpos, GetPetDialog("PetLoveval").rate, szText);
			GetPetDialog("PetLoveval").startclock = os.clock();
			break;
		end
	end
end

-- 设置玩家装备耐久度变化时宠物对话
function SetPlayerArmDurDialog( nState )
	local nBreak   = 1;
	local nUnUse   = 2;
	if GetPetDialog("PlayerArmDur") ~= nil then 
		if nState == nBreak then
			local nRandomIndex = getRand( 1, table.getn( GetPetDialog("PlayerArmDur").arm_break.text ) );
			local szText = GetPetDialog("PlayerArmDur").arm_break.text[nRandomIndex];
			t_PetDialogPos[GetPetDialog("PlayerArmDur").arm_break.showtype].func(GetPetDialog("PlayerArmDur").arm_break.showpos, GetPetDialog("PlayerArmDur").arm_break.rate, szText);
			GetPetDialog("PlayerArmDur").startclock = os.clock();
		elseif nState == nUnUse then 
			local nRandomIndex = getRand( 1, table.getn( GetPetDialog("PlayerArmDur").arm_unuse.text ) );
			local szText = GetPetDialog("PlayerArmDur").arm_unuse.text[nRandomIndex];
			t_PetDialogPos[GetPetDialog("PlayerArmDur").arm_unuse.showtype].func(GetPetDialog("PlayerArmDur").arm_unuse.showpos, GetPetDialog("PlayerArmDur").arm_unuse.rate, szText); 
			GetPetDialog("PlayerArmDur").startclock = os.clock();
		end
	end
end

-- 设置活动开始时宠物对话
function SetPromotionDialog( nId )
	if GetPetDialog("Promotion") ~= nil and GetPetDialog("Promotion")[nId] ~= nil then
		t_PetDialogPos[ GetPetDialog("Promotion")[nId].showtype].func( GetPetDialog("Promotion")[nId].showpos, GetPetDialog("Promotion")[nId].rate, GetPetDialog("Promotion")[nId].text );
		GetPetDialog("Promotion").startclock = os.clock();
	end
end

-- 设置进入地图时宠物对话
function SetEnterMapDialog( nMapId )
	if GetPetDialog("EnterMap")~= nil and GetPetDialog("EnterMap")[nMapId] ~= nil then
		local nRandomIndex = getRand( 1, table.getn( GetPetDialog("EnterMap")[nMapId].text ) );
		local szText = GetPetDialog("EnterMap")[nMapId].text[nRandomIndex];
		t_PetDialogPos[ GetPetDialog("EnterMap")[nMapId].showtype].func( GetPetDialog("EnterMap")[nMapId].showpos, GetPetDialog("EnterMap")[nMapId].rate, szText );
		GetPetDialog("EnterMap").startclock = os.clock();
	end 
end

function PetDialogFrame_OnShow()
	nShowDialogTime = os.clock();
	
	-- 隐藏指引提示TIPS窗口
	HideGuideEffectTip("PetDialogFrame");
end

function PetDialogFrame_OnLoad()
	for event, _ in pairs( t_PetDialogEvent ) do
		this:RegisterEvent( event );
	end
	initPetDialogData();
end

function PetDialogFrame_OnEvent()
	t_PetDialogEvent[arg1].func();
end

function PetDialogFrame_OnUpdate()
	if ( os.clock() - nShowDialogTime ) > DIALOG_SHOW_TIME then
		PetDialogFrame:Hide();
	end
end

function SetPetDiaologClock()
	dialog_startclock = os.clock();
end

function CanShowTaskMidMsg()
	return bShowDialog;
end

function UpdateFrame_OnUpdate()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil 
	or getGameState() ~= STATE_INWORLD 
	or GameMgr:isInPlayCamerAnim() 
	or LoadingFrame:IsShown() then
		if PetDialogFrame:IsShown() then
			PetDialogFrame:Hide();
		end
		return;
	end
	if ( os.clock() - dialog_startclock ) > DELAY_TIME then
		bShowDialog = true;
	end
	
	-- 玩家不移动时设置宠物对话
	local pos = MainPlayer:getPlayerPostion();
	if stand_lastposx ~= pos.m_nPosX then
		stand_startclock = os.clock();
		stand_lastposx = pos.m_nPosX;
	end

	if GetPetDialog("PlayerStand") ~= nil and ( os.clock() - stand_startclock ) >= GetPetDialog("PlayerStand").duration then
		local petPackage = MainPlayer:getPet();
		if petPackage:getPetNum() == 0 then
			return;
		end
		stand_lastposx = 0;
		t_PetDialogPos[ GetPetDialog("PlayerStand").showtype].func( GetPetDialog("PlayerStand").showpos, GetPetDialog("PlayerStand").rate, GetPetDialog("PlayerStand").text[getRand(1,table.getn(GetPetDialog("PlayerStand").text))] );
		GetPetDialog("PlayerStand").startclock = os.clock();
	end

	-- 宠物未出征时设置宠物对话
	if GetPetDialog("PetNotCamp") ~= nil and not PetUI:IsShown() then
		local petPackage = MainPlayer:getPet();
		if petPackage:getPetNum() == 0 then
			return;
		end
		local nPetIndex = getRand( 1, petPackage:getPetNum() );
		local petInfo	= petPackage:getPetInfo( nPetIndex-1 );	
		local petDef	= petPackage:getPetDef(petInfo.PetID);
		local szPetName = petDef.Name;
		-- 查看宠物的忠诚度是否达到出征的最低要求
		if petInfo.LoveVal < PET_FIGHT_LOVEVAL then
			return;
		end
		if petInfo.Name ~= "" then
			szPetName = petInfo.Name;
		end

		local szTalk	= GetPetDialog("PetNotCamp").text[getRand(1,table.getn(GetPetDialog("PetNotCamp").text))];
		if szTalk == nil or szTalk == "" then
			return;
		end
		if ( string.find(szTalk, "#petname#", 1, true) ) then
			local nPos			= string.find(szTalk, "#petname#");
			local nLength		= string.len( szTalk );
			local szPreTalk		= string.sub( szTalk, 1, nPos-1 );
			local szLastTalk	= string.sub( szTalk, nPos+9, nLength );
			szTalk = szPreTalk..szPetName..szLastTalk;
		end
		
		t_PetDialogPos[ GetPetDialog("PetNotCamp").showtype].func( GetPetDialog("PetNotCamp").showpos, GetPetDialog("PetNotCamp").rate, szTalk );
		GetPetDialog("PetNotCamp").startclock = os.clock();
	end 

	if GetPetDialog("InMap") ~= nil then
		local MinMapContal	= MapShower:GetMinMapContal();
		local nCurMapId	= MinMapContal:getMapID();
		if GetPetDialog("InMap")[nCurMapId] ~= nil then
			local szTalk	= GetPetDialog("InMap")[nCurMapId].text[getRand(1,table.getn(GetPetDialog("InMap")[nCurMapId].text))];
			t_PetDialogPos[ GetPetDialog("InMap")[nCurMapId].showtype].func( GetPetDialog("InMap")[nCurMapId].showpos, GetPetDialog("InMap")[nCurMapId].rate, szTalk );
			GetPetDialog("InMap").startclock = os.clock();
		end
	end
end

------------------------------------------------------------宠物天赋书-使用二次确认框--------------------------------------------------
local szPetGiftBooUseRemind = "使用天赋书学习天赋后将#R替换掉宠物当前天赋#n，确定需要学习天赋？"
local PetGiftBookUseConfirmFrame_OnShowTime = 0
function UsePetGiftBook( use_item )
	ClearItemLock("PetGiftBookUseConfirmFrame")
	-- 检查自己是否有出征宠物	
	local mainplayer = ActorMgr:getMainPlayer()
	local myFightPet	= GetFightPet( mainplayer );
	if myFightPet == nil then
		ShowMidTips( "请先召唤一只宠物，再使用宠物天赋书" );
		return;
	end
	
	PetGiftBookUseConfirmFrameText:SetText( szPetGiftBooUseRemind, 255,255,190 )
	PetGiftBookUseConfirmFrame_OnShowTime = os.clock()
	PetGiftBookUseConfirmFrame:Show()
	PetGiftBookUseConfirmFrame:SetClientUserData( 0, use_item:getGridIdx() )
	LockItem( use_item:getListType(), use_item:getGridIdx(), use_item:getItemId(), "PetGiftBookUseConfirmFrame" );
end
function PetGiftBookUseConfirmFrame_AcceptBtnOnClick()
	local grid = PetGiftBookUseConfirmFrame:GetClientUserData(0)
	local mainplayer 	= ActorMgr:getMainPlayer();
	local GamePet	= mainplayer:getPet();
	GamePet:petGiftBookUse( grid )
end
function PetGiftBookUseConfirmFrame_OnUpdate()
	local t = os.clock()
	local left = 30 - t + PetGiftBookUseConfirmFrame_OnShowTime
	if left <= 0 then
		this:Hide()
	end
end

function PetGiftBookUseConfirmFrame_OnHide()
	ClearItemLock("PetGiftBookUseConfirmFrame")
end

function PetGeniusLearnResult( nSucc )	
	PetGiftBookUseConfirmFrame:Hide()
	if nSucc == 1 then
		if PetFrame:IsShown() then
			UpdatePetFrame()
		else
			PetFrame:Show()
		end
		--播放特效
		local modelView = getglobal( "PetFrameGeniusModelViewTex" )
		modelView:setPetModel(0,0,"effect\\entity\\fumo01.ent");
		modelView:setPetScale(0,1,1,1);
		modelView:setEntityPosition(0,0,100,0);
	end
end
--------------------------------------------宠物幻化果实--------------------------------------
function UsePetGeniusUpItem( used_Item )
	-- 检查自己是否有出征宠物	
	local mainplayer = ActorMgr:getMainPlayer()
	--玩家死亡不可使用资质果实
	if mainplayer:isDead() then
		ShowMidTips( "死亡状态下不能使用此道具" )
		return
	end
	local petPackage		= mainplayer:getPet();
	local myFightPet		= GetFightPet( mainplayer );
	if myFightPet == nil then
		ShowMidTips( "请先召唤一只宠物，再使用宠物资质果实" );
		return;
	end
	local petInfo = petPackage:getPetInfo( getFightPetIndex() )
	
	local itemDef = used_Item:getItemDef()
	if itemDef.ItemID == 0 then
		return;
	end

	for i = 1, MAX_ITEM_RESULT do
		local resultData = itemDef.Result[i-1];
		if resultData.ResultID == RESULT_PET_GENIUS_UP then
			if resultData.ResultVal1 == petInfo.PetGenius then
				local nextGeniusDesc = t_geniusDesc[resultData.ResultVal1+1].value
				MessageBoxFrame:SetClientUserData( 0, used_Item:getListType() );
				MessageBoxFrame:SetClientUserData( 1, used_Item:getGridIdx() );
				MessageBoxFrame:SetClientUserData( 2, itemDef.UseTarget == USE_TARGET_PET and getFightPetIndex() or 0);
				MessageBox("宠物资质重置","确定需要将#R"..myFightPet:getName().."#n的资质提升至#R"..nextGeniusDesc.."#n？");
				LockItem( used_Item:getListType(), used_Item:getGridIdx(), used_Item:getItemId(), "MessageBoxFrame" );
				return;
			else
				local GeniusDesc = t_geniusDesc[resultData.ResultVal1].value
				local nextGeniusDesc = t_geniusDesc[resultData.ResultVal1+1].value
				ShowMidTips( "只有资质"..GeniusDesc.."的宠物才能使用"..nextGeniusDesc.."资质果实!" );
				return;
			end
		end
	end
end
--只要受到此消息就播特效，失败不会发的
function PetGeniusUpResult()
	if PetFrame:IsShown() then
		UpdatePetFrame()
	else
		PetFrame:Show()
	end
	--播放特效
	local modelView = getglobal( "PetFrameModelBtnModelView" )
	modelView:setPetModel(1,0,"effect\\entity\\fumo01.ent");
	modelView:setPetScale(1,1,1,1);
	modelView:setEntityPosition(1,0,100,0);
end
