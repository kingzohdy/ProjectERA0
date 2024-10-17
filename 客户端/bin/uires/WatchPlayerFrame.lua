MAX_SUITNUM	= 6;
local bFarWatch = false;	--是否是远距离观察
local bWatchPlayerFrameShowFashion = false;

local t_mainAttrConfig = 
{
-- /* 守护者 */	体质
[CLIENT_CAREER_WARRIOR_SINGLEHAND]	= { ["btn"] = "WPAttrBtn_BaseAttrBtn3", },

-- /* 征服者 */	力量
[CLIENT_CAREER_WARRIOR_DOUBLEHAND]	= { ["btn"] = "WPAttrBtn_BaseAttrBtn1", },

-- /* 聚能者 */	感知
[CLIENT_CAREER_MAGICOR_SINGLEHAND]	= { ["btn"] = "WPAttrBtn_BaseAttrBtn6", },

-- /* 炼魂师 */	智力
[CLIENT_CAREER_MAGICOR_DOUBLEHAND]	= { ["btn"] = "WPAttrBtn_BaseAttrBtn2", },

-- /* 猎杀者 */	力量
[CLIENT_CAREER_GUNER_SINGLEHAND]	= { ["btn"] = "WPAttrBtn_BaseAttrBtn1", },

-- /* 毁灭者 */	敏捷
[CLIENT_CAREER_GUNER_DOUBLEHAND]	= { ["btn"] = "WPAttrBtn_BaseAttrBtn5", },

-- /* 圣佑者 */	精神
[CLIENT_CAREER_CHURCH_SINGLEHAND]	= { ["btn"] = "WPAttrBtn_BaseAttrBtn4", },

-- /* 驭雷师 */	智力
[CLIENT_CAREER_CHURCH_DOUBLEHAND]	= { ["btn"] = "WPAttrBtn_BaseAttrBtn2", },
};


function WatchPlayerFrameOnLoad()
	this:RegisterEvent("GE_WATCHPLAYER_WATCH");
	this:RegisterEvent("GE_WATCHPLAYER_CLOSE");
	this:RegisterEvent("GE_ACTOR_CHANGE");
	this:RegisterEvent("GE_WATCHPLAYER_FARWATCH");
	this:RegisterEvent("GE_MAIN_EQUIP_CHANGE");

	for i=1, 2, 1 do
		for j=1, 8, 1 do
			local bagButton = getglobal( "WatchPlayerFrameEquip"..((i-1)*8+j));
			--bagButton:SetPoint("TOPLEFT", "WatchPlayerFrame", "TOPLEFT", ( 14 + 5*38*(i-1) + (i-1)*22 ), ( (j-1)*43 + 45 ) );
			bagButton:SetClientUserData( 0, DRAG_WEARFORM );
		end
	end
end

--@Desc:用于响应人物属性面板各种事件消息
--@return: Nothing
function WatchPlayerFrameOnEvent()
	if ( arg1 == "GE_WATCHPLAYER_WATCH" ) then
		local frame = getglobal( "WatchPlayerFrame" );
		bFarWatch = false;
		if frame:IsShown() then			
			SetWatchPlayerInfo();
		else
			WatchPlayerFrame:Show();
		end
	end

	if ( arg1 == "GE_WATCHPLAYER_CLOSE" ) then
		WatchPlayerFrame:Hide();
	end

	if ( arg1 == "GE_ACTOR_CHANGE" ) or ( arg1 == "GE_MAIN_EQUIP_CHANGE" ) then
		--[[
		if WatchPlayerFrame:IsShown() then
			if GameWatchPlayer:getWatchPlayerMemId() > 0 then
				GameWatchPlayer:sendWatchRequest( GameWatchPlayer:getWatchPlayerMemId() );
			end
		end
		]]--
	end

	if ( arg1 == "GE_WATCHPLAYER_FARWATCH" ) then
		bFarWatch = true;
		if WatchPlayerFrame:IsShown() then
			SetWatchPlayerInfo();
		else
			WatchPlayerFrame:Show();
		end
	end
end

-- 观察玩家的界面帧刷新 检测玩家是否在视野范围
function WatchPlayerFrameOnUpdate()
	local WatchPlayer = nil;
	if bFarWatch == true then
		WatchPlayer = GameWatchPlayer:getWatchPlayer();
	else		
		WatchPlayer = ActorMgr:FindActor( GameWatchPlayer:getWatchPlayerMemId() );
	end
	if WatchPlayer == nil then
		--WatchPlayerFrame:Hide();
		return;
	end
	--UpdateWPAttr();
	for i=1,EQUIP_POS_END do
		local item = GameWatchPlayer:getItem( i-1 );
		local serverTime = GameClanManager:getServerTime();
		if item:getItemId() ~= 0 and item:getItemValidEndTime() > 0 and item:getValidLeftTime() >= 0 then			
			item:setValidLeftTime( item:getItemValidEndTime() - serverTime );
			if item:getValidLeftTime() < 0 then
				SetWatchPlayerInfo();
			end
		end
	end
	
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer:isOutSide() or GameClanManager:clanGetClanNameByWid( WatchPlayer:getDaoBaClanWid() ) == "" then
		 WatchPlayerFrameDaoBaBtn:Hide()
	else
		 WatchPlayerFrameDaoBaBtn:Show()
	end
end

function GetWatchPlayerSuitArmNum( nSuitId )
	local t_SuitArm = {};
	local nSuitArmNum = 0;
	local nSuitTotalNum = getSuitEquipNum( nSuitId );
	for i = 1, nSuitTotalNum do
		for j = 1, MAXEQUIPPOS do
			local bagButton = getglobal( "WatchPlayerFrameEquip"..j );
			local nWearPos 	= GetWatchFashionSuitPos( bagButton:GetClientID() );
			local Item = GameWatchPlayer:getItem( nWearPos );
			if getSuitSetId( nSuitId, ( i - 1 ) ) == Item:getItemId() and not Item:isValidEnd() then
				t_SuitArm[Item:getItemId()] = 1;
			end
		end
	end
	for _,v in pairs( t_SuitArm ) do
		nSuitArmNum = nSuitArmNum + v;
	end
	return nSuitArmNum;
end

--@desc:玩家信息显示函数
--@return: Nothing
function WatchPlayerFrameOnShow()  
	-- 重置窗口坐标
	ModelAngle = 0;
	WPAttrBtn_AdvanceAttrLabelBtn:Checked();
	WPAttrBtn_SpecialAttrLabelBtn:DisChecked();
	WatchPlayerFrame:SetPoint("topleft", "$parent", "topleft", 55, 163 );
	SetWatchPlayerInfo();

	WPAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont1:SetText("攻");
	WPAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont2:SetText("击");
	WPAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont3:SetText("属");
	WPAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont4:SetText("性");
	WPAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont5:SetText("");
	WPAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont1:SetText("防");
	WPAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont2:SetText("御");
	WPAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont3:SetText("属");
	WPAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont4:SetText("性");
	WPAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont5:SetText("");

	WatchPlayerFrame_ShouSuoBtn:Show();
	WatchPlayerFrame_ZhanKaiBtn:Hide();
	ShowWatchPlayerFashionFrames( false );
	WatchPlayerFrame_FashionBtn:DisChecked();
	WatchPlayerFrame_EquipBtn:Checked();
	WatchPlayerFrameDaoBaBtn:Hide()
	
	UpdateWatchPlayerFrameWearEquipAttrStarButtonInfo();
end

function SetWatchPlayerInfo()
	local WatchPlayer = nil;
	if bFarWatch == true then
		WatchPlayer = GameWatchPlayer:getWatchPlayer();
	else
		WatchPlayer = ActorMgr:FindActor( GameWatchPlayer:getWatchPlayerMemId() );
	end
	if WatchPlayer == nil then
		return;
	end

	for i=1, MAXEQUIPPOS do
		local bagButton = getglobal( "WatchPlayerFrameEquip"..i);
		local UVAnimationTex = getglobal( "WatchPlayerFrameEquip"..i.."UVAnimationTex");
		local ItemTex = getglobal( "WatchPlayerFrameEquip"..i.."IconTexture");
		local ItemBoxTex = getglobal( "WatchPlayerFrameEquip"..i.."BoxTexture");
		local redTexture = getglobal( "WatchPlayerFrameEquip"..i.."RedTexture" );
		local BindTexture = getglobal( "WatchPlayerFrameEquip"..i.."BindTexture" );
		local validTime = getglobal( "WatchPlayerFrameEquip"..i.."ValidTimeTex" );
		local nWearPos 	= GetWatchFashionSuitPos( bagButton:GetClientID() );
		local Item = GameWatchPlayer:getItem( nWearPos );
		if( Item:getItemId() ~= 0 ) then
			local itemdef = Item:getItemDef();
			SetEquipImportantEffectUV( itemdef, UVAnimationTex:GetName(),Item); -- 设置装备特效
			bagButton:SetClientUserData(3,CONTAINER_TYPE_WEAR);

			local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
			-- 如果没找到物品的图标则使用默认图标显示
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			ItemTex:SetTexture( IconPath );

			ItemBoxTex:Show();
			if Item:isValidEnd() then
				validTime:Show()
			else
				validTime:Hide()
			end
			if Item:isItemBind() then
				BindTexture:Show();
			else
				BindTexture:Hide();
			end
		else
			UVAnimationTex:Hide();
			ItemBoxTex:Hide();
			validTime:Hide()
			BindTexture:Hide();
			ItemTex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
		end
	end

	--显示玩家主要消息
	local nLv		= WatchPlayer:getLv();

	--显示面板标题
	local clanName = GameClanManager:clanGetClanNameByWid( WatchPlayer:getClanWID() );
	WatchPlayerFrameTitleFont:SetText( clanName );

	local nCurExp		= WatchPlayer:getExp();
	local nLevelNextExp = WatchPlayer:getLvExp( nLv );

	WatchPlayerFrame_CurExp:SetText( nCurExp );
	WatchPlayerFrame_NextUpLvExp:SetText( nLevelNextExp );

	local ModelViewer = getglobal("WatchPlayerFrame_ModelView");
	ModelViewer:setEntityAvatar(0);
	GameWatchPlayer:setAvatarAttrib();
	local suitFlag = WatchHasLeastOneFashionSuit( GameWatchPlayer:getFashionSuitSelect() + 1 );
	if bWatchPlayerFrameShowFashion and suitFlag then
		setAvatarAttrib( POS_ID_FASHION_HAIR,1 );
	elseif not bWatchPlayerFrameShowFashion or not suitFlag then 
		setAvatarAttrib( POS_ID_FASHION_BODY,0);
		setAvatarAttrib( POS_ID_FASHION_HEAD,0);
		setAvatarAttrib( POS_ID_FASHION_SHOES,0);
		setAvatarAttrib( POS_ID_FASHION_HAND,0);
		setAvatarAttrib( POS_ID_FASHION_LEG,0);
		setAvatarAttrib( POS_ID_FASHION_HAIR,0);
	end
	if not WatchPlayer:isShowHead() then 
		setAvatarAttrib( POS_ID_HAT,0);
		setAvatarAttrib( POS_ID_HAIR, WatchPlayer:getHair() + 1 );
	end
	if not WatchPlayer:isShowFace() then 
		setAvatarAttrib( POS_ID_MASK,0);
	end
	for j=1, CLIENT_MAX_AVATAR_ATTRIB do
		if getAvatarAttrib( j-1 ) ~= 0 then
			if j == 1 then
				ModelViewer:setAvatarAttrib(0, j-1, getAvatarAttrib( j-1 ),getAvatarStarLv( j-1 ), true );
			else
				ModelViewer:setAvatarAttrib(0, j-1, getAvatarAttrib( j-1 ),getAvatarStarLv( j-1 ) );
			end
			--背饰幻化需要处理下显示
			if (j-1) == POS_ID_FASHION_BACK and GameWatchPlayer:getItem(EQUIP_POS_FASHION_BACK):getUnRealBackID() > 0 then
				ModelViewer:setAvatarAttrib(0, j-1, GameWatchPlayer:getItem(EQUIP_POS_FASHION_BACK):getUnRealBackID(),getAvatarStarLv( j-1 ) );
			end
		end
	end
	ModelViewer:setEntityRotate(0,0);
	local leftarmid		= getAvatarAttrib( POS_ID_LEFTHAND );
	local rightarmid	= getAvatarAttrib( POS_ID_RIGHTHAND );

	local t_AvartarControl	= GetAvartarControl();
	local nAnimID = t_AvartarControl:getAvatarAnim( {	["race"] = GetInitRace( WatchPlayer:getRace() ), ["leftHandItem"] = leftarmid, 
														["rightHandItem"] = rightarmid } );
	ModelViewer:playEntityAnim(0, nAnimID, nil );
	--男性小点
	if WatchPlayer:getGender() == 1 then
		ModelViewer:setPetScale(0,0.8,0.8,0.8);--设置缩放
	else
		ModelViewer:setPetScale(0,0.9,0.9,0.9);--设置缩放
	end	
	ModelViewer:setEntityPosition(0,0,20,0);
	WPAttrBtn:Show();
	--跨服战场中显示是否与玩家在同一服的标志
	local mainplayer = ActorMgr:getMainPlayer()
	local worldId = mainplayer:GetWorldID()
	local btnTex = getglobal( "WatchPlayerFrameIsOutSideBtnTexture" )
	if worldId == GameWatchPlayer:getWatchPlayerWorldID() or GameWatchPlayer:getWatchPlayerWorldID() == 0 then
		btnTex:SetTexUV( 422, 172, 18, 24 )
	else
		btnTex:SetTexUV( 441, 172, 18, 24 )
	end
	UpdateWatchPlayerFrameWearEquipAttrStarButtonInfo();
end

function WatchPlayerFrameOnHide()
	if NormalItemTipsFrame:IsShown() then
		if IsInFrameAbs( "NormalItemTipsFrame", "PlayerContainerFrame" ) then
			NormalItemTipsFrame:Hide();
		end
	end
	--bFarWatch = false;
end

function WPActiveButton_OnEnter()
	--SetGameTooltips( this:GetName(), "精力："..GetWPAttr( ATTR_ID_VIM ).."/"..GetWPAttr( ATTR_ID_VIMMAX ) );
end

function WPActiveButton_OnLeave()
	GameTooltip:Hide();
end

function WatchPlayerFrame_PetBtn_OnClick()
	RideFrame:Hide();
	WatchPlayerFrame:Hide();
	EquipRealX	= WatchPlayerFrame:GetRealLeft();
	EquipRealY	= WatchPlayerFrame:GetRealTop();
	PetFrame:Show();
end

--@desc:点击左转按钮时向左转动预览模型
--@return: Nothing
function WatchPlayerFrame_TurnLeftBtn_OnMouseDown()
	local ModelViewer = getglobal("WatchPlayerFrame_ModelView");
	ModelAngle = ModelAngle - 5;
	ModelViewer:setEntityRotate(0,ModelAngle);
end

--@desc:点击左右转按钮时向右转动预览模型
--@return: Nothing
function WatchPlayerFrame_TurnRightBtn_OnMouseDown()
	local ModelViewer = getglobal("WatchPlayerFrame_ModelView");
	ModelAngle = ModelAngle + 5;
	ModelViewer:setEntityRotate(0,ModelAngle);
end

function WPAttrBtn_CloseBtn_OnClick()
	WPAttrBtn:Hide();
end

function GetWPAttr( nIndex )
	if nIndex == 0 then
		return "";
	end

	local WatchPlayer = nil;
	if bFarWatch == true then
		WatchPlayer = GameWatchPlayer:getWatchPlayer();
	else
		WatchPlayer = ActorMgr:FindActor( GameWatchPlayer:getWatchPlayerMemId() );
	end
	if WatchPlayer == nil then
		return 0;
	end
	local nValue = WatchPlayer:getShortAttriValule( nIndex );
	if nIndex == ATTR_ID_DAMAGE then
		nValue = math.ceil(nValue/100);
	end
	return nValue;
end

function WPAttrBtn_OnShow()
	WPAttrBtn_AdvanceAttrLabelBtn:Checked();
	WPAttrBtn_SpecialAttrLabelBtn:DisChecked();
	UpdateWPAttr();
	if not bWatchPlayerFrameShowFashion then
		WatchPlayerFrame_ShouSuoBtn:Show();
	else
		WatchPlayerFrame_ShouSuoBtn:Hide();
	end
	WatchPlayerFrame_ZhanKaiBtn:Hide();
end

function WPAttrBtn_OnHide()
	if not bWatchPlayerFrameShowFashion then
		WatchPlayerFrame_ZhanKaiBtn:Show();
	else
		WatchPlayerFrame_ZhanKaiBtn:Hide();
	end
	WatchPlayerFrame_ShouSuoBtn:Hide();
	WatchPlayerFrame_ZhanKaiBtn:Show();
end

function WPAttrBtn_AdvanceAttrLabelBtn_OnClick()
	WPAttrBtn_AdvanceAttrLabelBtn:Checked();
	WPAttrBtn_SpecialAttrLabelBtn:DisChecked();
	
	Private_ShowWatchAdvanceAttr();
	UpdateWatchNormalAttackAttr();
	UpdateWatchNormalDefendAttr();

	WPAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont1:SetText("攻");
	WPAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont2:SetText("击");
	WPAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont3:SetText("属");
	WPAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont4:SetText("性");
	WPAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont5:SetText("");
	WPAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont1:SetText("防");
	WPAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont2:SetText("御");
	WPAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont3:SetText("属");
	WPAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont4:SetText("性");
	WPAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont5:SetText("");
end

function WPAttrBtn_SpecialAttrLabelBtn_OnClick()
	WPAttrBtn_AdvanceAttrLabelBtn:DisChecked();
	WPAttrBtn_SpecialAttrLabelBtn:Checked();
	
	Private_ShowWatchYuanSuAttr();

	UpdateWatchSpecialAttackAttr();
	UpdateWatchSpecialDefendAttr();
	UpdateWatchFuZhuAttr();	
end

-- 更新基础属性
function ClearWatchBaseAttr()
	for i = 1, MAX_BASE_ATTR_NUM do
		local btn = getglobal( "WPAttrBtn_BaseAttrBtn"..i );
		btn:Hide();
	end
end

function UpdateWatchBaseAttr()
	ClearWatchBaseAttr();
	local WatchPlayer = nil;
	if bFarWatch == true then
		WatchPlayer = GameWatchPlayer:getWatchPlayer();
	else
		WatchPlayer = ActorMgr:FindActor( GameWatchPlayer:getWatchPlayerMemId() );
	end

	local initRace		= GetInitRace( WatchPlayer:getRace() );
	
	local t_attrList	= t_RoleAttrInfo["基础属性"][initRace];
	local maxAttrNum	= t_attrList["maxElemNum"];
	for i = 1, maxAttrNum do
		local oneElem		= t_attrList[i];
		local funcGetBase	= oneElem["getBaseAttrFunc"] ~= nil and oneElem["getBaseAttrFunc"]	or GetRoleBaseAttr;
		local funcGetCur	= oneElem["getCurAttrFunc"] ~= nil	and oneElem["getCurAttrFunc"]	or GetRoleAttr;

		local nCurValue		= funcGetCur( oneElem["attrID"],	WatchPlayer );
		local nBaseValue	= funcGetBase( oneElem["attrID"],	WatchPlayer );

		local t_color	= { r = 255, g = 254, b = 189 };
		if nCurValue > nBaseValue then
			t_color	= { r = 15, g = 255, b = 0 };
		elseif nCurValue < nBaseValue then
			t_color	= { r = 255, g = 0, b = 0 };
		end

		if oneElem["maxAttrID"] ~= nil then
			nCurMaxValue = funcGetCur( oneElem["maxAttrID"],	WatchPlayer );
			nBaseMaxValue = funcGetBase( oneElem["maxAttrID"],	WatchPlayer );
			if nCurMaxValue < nBaseMaxValue then
				t_color	= { r = 255, g = 0, b = 0 };
			end
			nCurValue = nCurValue .. "-" .. nCurMaxValue
		end

		local btn = getglobal( "WPAttrBtn_BaseAttrBtn"..i );
		local funcGetTips = oneElem["getAttrTipsDescFunc"];
		if funcGetTips ~= nil then
			btn:SetClientString( funcGetTips( oneElem["attrID"], WatchPlayer ) );
		else
			btn:SetClientString( oneElem["desc"] );
		end

		local funcGetDesc = oneElem["getAttrDescFunc"];
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneElem["name"] }, 
							[btn:GetName().."ValueFont"]	= 
							{	value = funcGetDesc ~= nil and funcGetDesc( oneElem["attrID"], WatchPlayer ) or nCurValue,	
								color = t_color, 
							},
						};
		util.SetFonts( t_fonts );
		btn:Show();
	end

	local mainAttrFlagBtn		= getglobal( "WPAttrBtnMainAttrFlagButton" );
	local t_changeCareerControl = GetChangeCareerControl();
	local nRace					= WatchPlayer:getRace();
	if t_changeCareerControl:isChangeCareer( { ["race"] = nRace } ) then
		local szAttrBtn = t_mainAttrConfig[nRace]["btn"];
		mainAttrFlagBtn:SetPoint( "left", szAttrBtn, "left", 35, 0 );
		mainAttrFlagBtn:Show();
	else
		mainAttrFlagBtn:Hide();
	end
end

-- 更新普通攻击属性
function ClearWatchNormalAttackAttr()
	for i = 1, MAX_NORMAL_ATTACK_ATTR_NUM do
		local btn = getglobal( "WPAttrBtn_AttackAttrBtn"..i );
		btn:Hide();
	end
end

function UpdateWatchNormalAttackAttr()
	ClearWatchNormalAttackAttr();
	
	local WatchPlayer = nil;
	if bFarWatch == true then
		WatchPlayer = GameWatchPlayer:getWatchPlayer();
	else
		WatchPlayer = ActorMgr:FindActor( GameWatchPlayer:getWatchPlayerMemId() );
	end

	local initRace		= GetInitRace( WatchPlayer:getRace() );
	local t_attrList	= t_RoleAttrInfo["攻击属性"][initRace];
	local maxAttrNum	= t_attrList["maxElemNum"];

	for i = 1, maxAttrNum do
		local oneElem		= t_attrList[i];
		local funcGetBase	= oneElem["getBaseAttrFunc"] ~= nil and oneElem["getBaseAttrFunc"]	or GetRoleBaseAttr;
		local funcGetCur	= oneElem["getCurAttrFunc"] ~= nil	and oneElem["getCurAttrFunc"]	or GetRoleAttr;

		local nCurValue		= funcGetCur( oneElem["attrID"],	WatchPlayer );
		local nBaseValue	= funcGetBase( oneElem["attrID"],	WatchPlayer );

		local t_color	= { r = 255, g = 254, b = 189 };
		if nCurValue > nBaseValue then
			t_color	= { r = 15, g = 255, b = 0 };
		elseif nCurValue < nBaseValue then
			t_color	= { r = 255, g = 0, b = 0 };
		end

		if oneElem["maxAttrID"] ~= nil then
			nCurMaxValue = funcGetCur( oneElem["maxAttrID"],	WatchPlayer );
			nBaseMaxValue = funcGetBase( oneElem["maxAttrID"],	WatchPlayer );
			if nCurMaxValue < nBaseMaxValue then
				t_color	= { r = 255, g = 0, b = 0 };
			end
			nCurValue = nCurValue .. "-" .. nCurMaxValue
		end

		local btn = getglobal( "WPAttrBtn_AttackAttrBtn"..i );

		local funcGetTips = oneElem["getAttrTipsDescFunc"];
		if funcGetTips ~= nil then
			btn:SetClientString( funcGetTips( oneElem["attrID"], WatchPlayer ) );
		else
			btn:SetClientString( oneElem["desc"] );
		end

		local funcGetDesc = oneElem["getAttrDescFunc"];
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneElem["name"] }, 
							[btn:GetName().."ValueFont"]	= 
							{	value = funcGetDesc ~= nil and funcGetDesc( oneElem["attrID"], WatchPlayer ) or nCurValue,	
								color = t_color, 
							},
						};
		util.SetFonts( t_fonts );
		btn:Show();
	end
end

-- 更新普通防御属性
function ClearWatchNormalDefendAttr()
	for i = 1, MAX_NORMAL_DEFEND_ATTR_NUM do	
		local btn = getglobal( "WPAttrBtn_DefendAttrBtn"..i );
		btn:Hide();
	end
end

function UpdateWatchNormalDefendAttr()
	ClearWatchNormalDefendAttr();
	
	local WatchPlayer = nil;
	if bFarWatch == true then
		WatchPlayer = GameWatchPlayer:getWatchPlayer();
	else
		WatchPlayer = ActorMgr:FindActor( GameWatchPlayer:getWatchPlayerMemId() );
	end

	local initRace		= GetInitRace( WatchPlayer:getRace() );
	
	local t_attrList	= t_RoleAttrInfo["防御属性"][initRace];
	local maxAttrNum	= t_attrList["maxElemNum"];

	for i = 1, maxAttrNum do
		local oneElem		= t_attrList[i];
		local funcGetBase	= oneElem["getBaseAttrFunc"] ~= nil and oneElem["getBaseAttrFunc"]	or GetRoleBaseAttr;
		local funcGetCur	= oneElem["getCurAttrFunc"] ~= nil	and oneElem["getCurAttrFunc"]	or GetRoleAttr;

		local nCurValue		= funcGetCur( oneElem["attrID"],	WatchPlayer );
		local nBaseValue	= funcGetBase( oneElem["attrID"],	WatchPlayer );

		local t_color	= { r = 255, g = 254, b = 189 };
		if nCurValue > nBaseValue then
			t_color	= { r = 15, g = 255, b = 0 };
		elseif nCurValue < nBaseValue then
			t_color	= { r = 255, g = 0, b = 0 };
		end

		if oneElem["maxAttrID"] ~= nil then
			nCurMaxValue = funcGetCur( oneElem["maxAttrID"],	WatchPlayer );
			nBaseMaxValue = funcGetBase( oneElem["maxAttrID"],	WatchPlayer );
			if nCurMaxValue < nBaseMaxValue then
				t_color	= { r = 255, g = 0, b = 0 };
			end
			nCurValue = nCurValue .. "-" .. nCurMaxValue
		end

		local btn			= getglobal( "WPAttrBtn_DefendAttrBtn"..i );
		local funcGetTips	= oneElem["getAttrTipsDescFunc"];

		if funcGetTips ~= nil then
			btn:SetClientString( funcGetTips( oneElem["attrID"], WatchPlayer ) );
		else
			btn:SetClientString( oneElem["desc"] );
		end

		local funcGetDesc	= oneElem["getAttrDescFunc"];
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneElem["name"] }, 
							[btn:GetName().."ValueFont"]	= 
							{	value = funcGetDesc ~= nil and funcGetDesc( oneElem["attrID"], WatchPlayer ) or nCurValue,	
								color = t_color, 
							},
						};
		util.SetFonts( t_fonts );
		btn:Show();
	end
end

-- 更新特殊攻击属性
function ClearWatchYuanSuAttackAttr()
	for i = 1, MAX_YUAN_SU_ATTACK_ATTR_NUM do
		local btn = getglobal( "WPAttrBtn_YuanSuAttackAttrBtn"..i );
		btn:Hide();
	end
end

function UpdateWatchSpecialAttackAttr()
	ClearWatchYuanSuAttackAttr();
	
	local WatchPlayer = nil;
	if bFarWatch == true then
		WatchPlayer = GameWatchPlayer:getWatchPlayer();
	else
		WatchPlayer = ActorMgr:FindActor( GameWatchPlayer:getWatchPlayerMemId() );
	end

	local initRace		= GetInitRace( WatchPlayer:getRace() );
	
	local t_attrList	= t_RoleAttrInfo["元素攻击力"][initRace];
	local maxAttrNum	= t_attrList["maxElemNum"];

	for i = 1, maxAttrNum do
		local oneElem		= t_attrList[i];
		local funcGetBase	= oneElem["getBaseAttrFunc"] ~= nil and oneElem["getBaseAttrFunc"]	or GetRoleBaseAttr;
		local funcGetCur	= oneElem["getCurAttrFunc"] ~= nil	and oneElem["getCurAttrFunc"]	or GetRoleAttr;

		local nCurValue		= funcGetCur( oneElem["attrID"],	WatchPlayer );
		local nBaseValue	= funcGetBase( oneElem["attrID"],	WatchPlayer );

		local t_color	= { r = 255, g = 254, b = 189 };
		if nCurValue > nBaseValue then
			t_color	= { r = 15, g = 255, b = 0 };
		elseif nCurValue < nBaseValue then
			t_color	= { r = 255, g = 0, b = 0 };
		end

		if oneElem["maxAttrID"] ~= nil then
			nCurMaxValue = funcGetCur( oneElem["maxAttrID"],	WatchPlayer );
			nBaseMaxValue = funcGetBase( oneElem["maxAttrID"],	WatchPlayer );
			if nCurMaxValue < nBaseMaxValue then
				t_color	= { r = 255, g = 0, b = 0 };
			end
			nCurValue = nCurValue .. "-" .. nCurMaxValue
		end

		local btn = getglobal( "WPAttrBtn_YuanSuAttackAttrBtn"..i );
		btn:SetClientString( oneElem["desc"] );
		local funcGetDesc = oneElem["getAttrDescFunc"];
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneElem["name"] }, 
							[btn:GetName().."ValueFont"]	= 
							{	value = funcGetDesc ~= nil and funcGetDesc( oneElem["attrID"], WatchPlayer ) or nCurValue,	
								color = t_color, 
							},
						};
		util.SetFonts( t_fonts );
		btn:Show();
	end
end

-- 更新特殊防御属性
function ClearWatchYuanSuDefendAttr()
	for i = 1, MAX_YUAN_SU_DEFEND_ATTR_NUM do
		local btn = getglobal( "WPAttrBtn_YuanSuDefendAttrBtn"..i );
		btn:Hide();
	end
end

function UpdateWatchSpecialDefendAttr()
	ClearWatchYuanSuDefendAttr();
	
	local WatchPlayer = nil;
	if bFarWatch == true then
		WatchPlayer = GameWatchPlayer:getWatchPlayer();
	else
		WatchPlayer = ActorMgr:FindActor( GameWatchPlayer:getWatchPlayerMemId() );
	end

	local initRace		= GetInitRace( WatchPlayer:getRace() );
	
	local t_attrList	= t_RoleAttrInfo["元素防御力"][initRace];
	local maxAttrNum	= t_attrList["maxElemNum"];

	for i = 1, maxAttrNum do
		local oneElem		= t_attrList[i];
		local funcGetBase	= oneElem["getBaseAttrFunc"] ~= nil and oneElem["getBaseAttrFunc"]	or GetRoleBaseAttr;
		local funcGetCur	= oneElem["getCurAttrFunc"] ~= nil	and oneElem["getCurAttrFunc"]	or GetRoleAttr;

		local nCurValue		= funcGetCur( oneElem["attrID"],	WatchPlayer );
		local nBaseValue	= funcGetBase( oneElem["attrID"],	WatchPlayer );

		local t_color	= { r = 255, g = 254, b = 189 };
		if nCurValue > nBaseValue then
			t_color	= { r = 15, g = 255, b = 0 };
		elseif nCurValue < nBaseValue then
			t_color	= { r = 255, g = 0, b = 0 };
		end

		if oneElem["maxAttrID"] ~= nil then
			nCurMaxValue = funcGetCur( oneElem["maxAttrID"],	WatchPlayer );
			nBaseMaxValue = funcGetBase( oneElem["maxAttrID"],	WatchPlayer );
			if nCurMaxValue < nBaseMaxValue then
				t_color	= { r = 255, g = 0, b = 0 };
			end
			nCurValue = nCurValue .. "-" .. nCurMaxValue
		end

		local btn			= getglobal( "WPAttrBtn_YuanSuDefendAttrBtn"..i );
		btn:SetClientString( oneElem["desc"] );
		local funcGetDesc	= oneElem["getAttrDescFunc"];
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneElem["name"] }, 
							[btn:GetName().."ValueFont"]	= 
							{	value = funcGetDesc ~= nil and funcGetDesc( oneElem["attrID"], WatchPlayer ) or nCurValue,	
								color = t_color, 
							},
						};
		util.SetFonts( t_fonts );
		btn:Show();
	end
end

function ClearWatchFuZhuAttr()
	for i = 1, MAX_FU_ZHU_ATTR_NUM do
		local btn = getglobal( "WPAttrBtn_FuZhuAttrBtn"..i );
		btn:Hide();
	end
end

-- 更新套装属性
function ClearWatchSuitAttr()
	for i = 1, MAX_SUIT_ATTR_NUM do
		local btn = getglobal( "WPAttrBtn_SuitAttrBtn"..i );
		btn:Hide();
	end
	
	local t_hideIcons = { "WPAttrBtnSuitBackGroundButton", };
	util.HideIcons( t_hideIcons );
end

function UpdateWatchFuZhuAttr()
	ClearWatchFuZhuAttr();
	
	local WatchPlayer = nil;
	if bFarWatch == true then
		WatchPlayer = GameWatchPlayer:getWatchPlayer();
	else
		WatchPlayer = ActorMgr:FindActor( GameWatchPlayer:getWatchPlayerMemId() );
	end

	local initRace		= GetInitRace( WatchPlayer:getRace() );
	
	local t_attrList	= t_RoleAttrInfo["辅助属性"][initRace];
	local maxAttrNum	= t_attrList["maxElemNum"];

	for i = 1, maxAttrNum do
		local oneElem		= t_attrList[i];
		local funcGetBase	= oneElem["getBaseAttrFunc"] ~= nil and oneElem["getBaseAttrFunc"]	or GetRoleBaseAttr;
		local funcGetCur	= oneElem["getCurAttrFunc"] ~= nil	and oneElem["getCurAttrFunc"]	or GetRoleAttr;

		local nCurValue		= funcGetCur( oneElem["attrID"],	WatchPlayer );
		local nBaseValue	= funcGetBase( oneElem["attrID"],	WatchPlayer );

		local t_color	= { r = 255, g = 254, b = 189 };
		if nCurValue > nBaseValue then
			t_color	= { r = 15, g = 255, b = 0 };
		elseif nCurValue < nBaseValue then
			t_color	= { r = 255, g = 0, b = 0 };
		end

		if oneElem["maxAttrID"] ~= nil then
			nCurMaxValue = funcGetCur( oneElem["maxAttrID"],	WatchPlayer );
			nBaseMaxValue = funcGetBase( oneElem["maxAttrID"],	WatchPlayer );
			if nCurMaxValue < nBaseMaxValue then
				t_color	= { r = 255, g = 0, b = 0 };
			end
			nCurValue = nCurValue .. "-" .. nCurMaxValue
		end

		local btn = getglobal( "WPAttrBtn_FuZhuAttrBtn"..i );

		local funcGetTips = oneElem["getAttrTipsDescFunc"];
		if funcGetTips ~= nil then
			btn:SetClientString( funcGetTips( oneElem["attrID"], WatchPlayer ) );
		else
			btn:SetClientString( oneElem["desc"] );
		end

		local funcGetDesc	= oneElem["getAttrDescFunc"];
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneElem["name"] }, 
							[btn:GetName().."ValueFont"]	= 
							{	value = funcGetDesc ~= nil and funcGetDesc( oneElem["attrID"], WatchPlayer ) or nCurValue,	
								color = t_color, 
							},
						};
		util.SetFonts( t_fonts );
		btn:Show();
	end
end
function GetWatchFashionSuitPos( nAmmPos )
	local nSelectSuit 	= GameWatchPlayer:getFashionSuitSelect();
	--[[
	local watchActor	= GameWatchPlayer:getWatchPlayer();
	if watchActor:isShowFashion() then 
		return nAmmPos;
	end
	--]]
	if nAmmPos == EQUIP_POS_FASHION_BODY then 
		if nSelectSuit == 0 then 
			nWearPos = EQUIP_POS_FASHION_BODY;
		elseif nSelectSuit == 1 then 
			nWearPos = EQUIP_POS_FASHION_BODY2 ;
		else 
			nWearPos = EQUIP_POS_FASHION_BODY3 ;
		end
	elseif nAmmPos >= EQUIP_POS_FASHION_HEAD then
		nWearPos = nAmmPos + nSelectSuit * 7; 
	else 
		nWearPos = nAmmPos;
	end
	return nWearPos;
end

function UpdateWatchSuitAttr()
	ClearWatchSuitAttr();
	
	local t_SuitList = {};
	local isHaveSuitFunc = 
	function ( itemDef )
		for _, data in ipairs( t_SuitList ) do
			if itemDef.SuitSetID == data["suitID"] then
				return true;
			end
		end
		return false;
	end
	
	for i = 1, MAXEQUIPPOS do
		
		local bagButton	= getglobal( "WatchPlayerFrameEquip"..i);
		local nWearPos 	= GetWatchFashionSuitPos( bagButton:GetClientID() );
		local Item	= GameWatchPlayer:getItem( nWearPos );
		local itemDef	= Item:getItemDef();
		if itemDef ~= nil and itemDef.SuitSetID > 0 then
			if not isHaveSuitFunc( itemDef ) then
				local SuitSet		= getSuitSet( itemDef.SuitSetID );
				local t_SuitColor	= GetItemNameColor(itemDef);
				local t_oneSuitElem = 
				{	name	= SuitSet.Name,
					value	= GetWatchPlayerSuitArmNum( itemDef.SuitSetID ) .. "/"..getSuitEquipNum( itemDef.SuitSetID ),
					color	= { r = t_SuitColor[1], g = t_SuitColor[2], b = t_SuitColor[3] },
					suitID	= itemDef.SuitSetID,
				};
				table.insert( t_SuitList, t_oneSuitElem );
			end
		end
	end
	
	-- 先设置装备按钮数据
	for i = 1, table.getn( t_SuitList ) do
		local oneSuitData	= t_SuitList[i];
		local btn			= getglobal( "WPAttrBtn_SuitAttrBtn"..i );
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneSuitData["name"],	color = oneSuitData["color"],  }, 
							[btn:GetName().."ValueFont"]	= {	value = oneSuitData["value"],	color = oneSuitData["color"], },
						};
		util.SetFonts( t_fonts );
		btn:Show();
	end
	
	local ONE_SUIT_BTN_HEIGHT			= 16;
	local EXTEND_FRAME_SIZE_LIMIT_NUM	= 2;
	local nSuitNum	= table.getn( t_SuitList );
	if nSuitNum > 0 then
		WPAttrBtnSuitBackGroundButton:Show();
		WPAttrBtnSuitBackGroundButton:SetHeight( math.max( EXTEND_FRAME_SIZE_LIMIT_NUM, table.getn( t_SuitList ) ) * ONE_SUIT_BTN_HEIGHT );
	end
	
	if nSuitNum > EXTEND_FRAME_SIZE_LIMIT_NUM then
		local size = 456 - 32 - MAX_BOTTOM_LEAVE_SPACE +  nSuitNum * 16;
		WPAttrBtn:SetHeight( size );
	else
		WPAttrBtn:SetHeight( 456 );
	end
	
end

function Private_ShowWatchAdvanceAttr()
	ClearWatchYuanSuDefendAttr();
	ClearWatchFuZhuAttr();
	ClearWatchYuanSuAttackAttr();

	local t_hideIcons = { "WPAttrBtnSecondBig_YuanSuAttackAttrBackGroundButton", "WPAttrBtnSecondBig_YuanSuDefendAttrBackGroundButton",	
							"WPAttrBtnSecondBig_FuZhuAttrBackGroundButton" };
	util.HideIcons( t_hideIcons );

	local t_showIcons = { "WPAttrBtnSecondBig_FirstChildAttrBackGroundButton", "WPAttrBtnSecondBig_SecondChildAttrBackGroundButton" };
	util.ShowIcons( t_showIcons );
end

function Private_ShowWatchYuanSuAttr()
	ClearWatchNormalDefendAttr();
	ClearWatchNormalAttackAttr();

	local t_showIcons = { "WPAttrBtnSecondBig_YuanSuAttackAttrBackGroundButton", "WPAttrBtnSecondBig_YuanSuDefendAttrBackGroundButton",	
							"WPAttrBtnSecondBig_FuZhuAttrBackGroundButton" };
	util.ShowIcons( t_showIcons );

	local t_hideIcons = { "WPAttrBtnSecondBig_FirstChildAttrBackGroundButton", "WPAttrBtnSecondBig_SecondChildAttrBackGroundButton" };
	util.HideIcons( t_hideIcons );
end

function WPAttrBtn_NodeBtn_OnEnter()
	local szText = this:GetClientString();
	if szText == "" then
		return;
	end
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = "cursor", vertical = "top" , color = {255,150,0} } );
end

function WPAttrBtn_NodeBtn_OnLeave()
	GameTooltip:Hide();
end

function UpdateWPAttr()
	local WatchPlayer = nil;
	if bFarWatch == true then
		WatchPlayer = GameWatchPlayer:getWatchPlayer();
	else
		WatchPlayer = ActorMgr:FindActor( GameWatchPlayer:getWatchPlayerMemId() );
	end
	if WatchPlayer == nil then return end

	--显示玩家主要消息
	local nLv			= WatchPlayer:getLv();
	local nCurExp		=	WatchPlayer:getExp();
	local MaxHp			=	WatchPlayer:getShortAttriValule( ATTR_ID_HPMAX );
	local MaxMp			=	WatchPlayer:getShortAttriValule( ATTR_ID_MPMAX );
	local nLevelNextExp =	WatchPlayer:getLvExp( nLv );
	local levelUpDef = getLevelUP( WatchPlayer:getLv() );

	local t_fonts = 
	{ 
		["WPAttrNameFont"]			= { value = GetPlayerName(WatchPlayer:getName()) }, 
		["WPAttrLvFont"]			= { value = "等级："..WatchPlayer:getLv().."级" }, 
		["WPAttrRaceFont"]			= { value = "职业："..GetCareerName( WatchPlayer:getRace() ) }, 
		["WPAttrGenuisFont"]			= { value =  nLv >= 60 and ("天赋值：" .. GameWatchPlayer:getWatchPlayerGeniusPoint() ) or "" , color = (GameWatchPlayer:getWatchPlayerGeniusPoint() >= levelUpDef.FzMod) and {r=255,g=150,b=0} or {r=255,g=0,b=0} },
		["WatchPlayerFrame_CurExp"]		= { value = nCurExp }, 
		["WatchPlayerFrame_NextUpLvExp"]= { value = nLevelNextExp }, 
	};
	util.SetFonts( t_fonts );
	
	-- 更新HP和MP条
	local MAX_HP_LENGTH = 152;
	--显示面板标题
	-- 设置HP
	local maxHp		= WatchPlayer:getMaxHP();
	local hp		= WatchPlayer:getHP();
	hp				= math.max( math.min( hp, maxHp ), 0 );
	local fHpScale	= math.max( math.min( hp / maxHp, 1 ), 0 );
	local hpBtn		= getglobal( "WPAttrBtnFirstBaseLifeAttrButton" );
	local hpTex		= getglobal(  hpBtn:GetName().."Tex" );
	hpTex:SetSize( fHpScale * MAX_HP_LENGTH, 11 );

	-- 设置MP
	local maxMp		= WatchPlayer:getMaxMP();
	local mp		= WatchPlayer:getMP();
	mp				= math.max( math.min( mp, maxMp ), 0 );
	local fMpScale	= math.max( math.min( mp / maxMp, 1 ), 0 );
	local mpBtn		= getglobal( "WPAttrBtnFirstBaseMagicAttrButton" );
	local mpTex		= getglobal(  mpBtn:GetName().."Tex" );
	mpTex:SetSize( fMpScale * MAX_HP_LENGTH, 11 );

	local t_fonts = {	[hpBtn:GetName().."PercentFont"] = { value = hp.."/"..maxHp }, 
						[mpBtn:GetName().."PercentFont"] = { value = mp.."/"..maxMp },
					};
	util.SetFonts( t_fonts );

	UpdateWatchBaseAttr();
	-- 若选择的高级属性	IsEnable
	if WPAttrBtn_AdvanceAttrLabelBtn:IsChecked() then
		Private_ShowWatchAdvanceAttr();
		UpdateWatchNormalAttackAttr();
		UpdateWatchNormalDefendAttr();
	else
		UpdateWatchSpecialAttackAttr();
		UpdateWatchSpecialDefendAttr();
		UpdateWatchFuZhuAttr();

		Private_ShowWatchYuanSuAttr();
	end

	UpdateWatchSuitAttr();
end

function ShowWatchPlayerFashionFrames( bShow )
	if bShow then
		for i=1,7 do
			local tex = getglobal( "WatchPlayerFrameLeftEquipbackground" .. i );
			tex:Hide();
			tex = getglobal( "WatchPlayerFrameRightEquipbackground" .. i );
			tex:Hide();
		end
		for i=1,16 do
			local btn = getglobal( "WatchPlayerFrameEquip" .. i );
			btn:Hide();
		end
		-- 经验
		WatchPlayerFrameExpBkg:Hide();
		WatchPlayerFrameExpBkg1:Hide();
		WatchPlayerFrame_CurExpDesc:Hide();
		WatchPlayerFrame_NextUpLvExpDesc:Hide();
		WatchPlayerFrame_CurExp:Hide();
		WatchPlayerFrame_NextUpLvExp:Hide();

		WatchPlayerFrameFashionBkg:Show();
		for i=1,9 do
			local tex = getglobal( "WatchPlayerFrameFashionItemBkg" ..i );
			tex:Show();
		end
		WatchPlayerFrameEquip8:Show();
		WatchPlayerFrameEquip17:Show();
		WatchPlayerFrameEquip18:Show();
		WatchPlayerFrameEquip19:Show();
		WatchPlayerFrameEquip20:Show();
		WatchPlayerFrameEquip21:Show();
		WatchPlayerFrameEquip22:Show();
		WatchPlayerFrame_ZhanKaiBtn:Hide();
		WatchPlayerFrame_ShouSuoBtn:Hide();
	else
		for i=1,7 do
			local tex = getglobal( "WatchPlayerFrameLeftEquipbackground" .. i );
			tex:Show();
			tex = getglobal( "WatchPlayerFrameRightEquipbackground" .. i );
			tex:Show();
		end
		for i=1,16 do
			local btn = getglobal( "WatchPlayerFrameEquip" .. i );
			btn:Show();
		end
		-- 经验
		WatchPlayerFrameExpBkg:Show();
		WatchPlayerFrameExpBkg1:Show();
		WatchPlayerFrame_CurExpDesc:Show();
		WatchPlayerFrame_NextUpLvExpDesc:Show();
		WatchPlayerFrame_CurExp:Show();
		WatchPlayerFrame_NextUpLvExp:Show();

		WatchPlayerFrameFashionBkg:Hide();
		for i=1,9 do
			local tex = getglobal( "WatchPlayerFrameFashionItemBkg" ..i );
			tex:Hide();
		end
		WatchPlayerFrameEquip8:Hide();
		WatchPlayerFrameEquip17:Hide();
		WatchPlayerFrameEquip18:Hide();
		WatchPlayerFrameEquip19:Hide();
		WatchPlayerFrameEquip20:Hide();
		WatchPlayerFrameEquip21:Hide();
		WatchPlayerFrameEquip22:Hide();
		if WPAttrBtn:IsShown() then
			WatchPlayerFrame_ShouSuoBtn:Show();
			WatchPlayerFrame_ZhanKaiBtn:Hide();
		else
			WatchPlayerFrame_ShouSuoBtn:Hide();
			WatchPlayerFrame_ZhanKaiBtn:Show();
		end	
	end
	bWatchPlayerFrameShowFashion = bShow;
	SetWatchPlayerInfo();
	WatchPlayerFrameEquip7:Hide();
end

function WatchPlayerFrameEquipBtn_OnClick()
	if not bWatchPlayerFrameShowFashion then
		return
	end
	ShowWatchPlayerFashionFrames( false );
	WatchPlayerFrame_FashionBtn:DisChecked();
	WatchPlayerFrame_EquipBtn:Checked();

end

function WatchPlayerFrameFashionBtn_OnClick()
	if bWatchPlayerFrameShowFashion then
		return
	end
	ShowWatchPlayerFashionFrames( true );
	WatchPlayerFrame_FashionBtn:Checked();
	WatchPlayerFrame_EquipBtn:DisChecked();
end

function WatchPlayerFrameNengYuanBtn_OnClick()
	WatchSparFrame:Show()
	WatchSparFrame:SetPoint("topleft", "UIClient", "topleft",WatchPlayerFrame:GetRealLeft(), WatchPlayerFrame:GetRealTop());
	WatchPlayerFrame:Hide()
end

------------	观察能源界面	-------------

local t_mainSparAttrConfig = 
{
-- /* 守护者 */	体质
[CLIENT_CAREER_WARRIOR_SINGLEHAND]	= { ["btn"] = "WatchSparFrameRoleAttrBtn_BaseAttrBtn3", },

-- /* 征服者 */	力量
[CLIENT_CAREER_WARRIOR_DOUBLEHAND]	= { ["btn"] = "WatchSparFrameRoleAttrBtn_BaseAttrBtn1", },

-- /* 聚能者 */	感知
[CLIENT_CAREER_MAGICOR_SINGLEHAND]	= { ["btn"] = "WatchSparFrameRoleAttrBtn_BaseAttrBtn6", },

-- /* 炼魂师 */	智力
[CLIENT_CAREER_MAGICOR_DOUBLEHAND]	= { ["btn"] = "WatchSparFrameRoleAttrBtn_BaseAttrBtn2", },

-- /* 猎杀者 */	力量
[CLIENT_CAREER_GUNER_SINGLEHAND]	= { ["btn"] = "WatchSparFrameRoleAttrBtn_BaseAttrBtn1", },

-- /* 毁灭者 */	敏捷
[CLIENT_CAREER_GUNER_DOUBLEHAND]	= { ["btn"] = "WatchSparFrameRoleAttrBtn_BaseAttrBtn5", },

-- /* 圣佑者 */	精神
[CLIENT_CAREER_CHURCH_SINGLEHAND]	= { ["btn"] = "WatchSparFrameRoleAttrBtn_BaseAttrBtn4", },

-- /* 驭雷师 */	智力
[CLIENT_CAREER_CHURCH_DOUBLEHAND]	= { ["btn"] = "WatchSparFrameRoleAttrBtn_BaseAttrBtn2", },
};

function WatchSparFrame_OnLoad()
	
	local szText = [[●将晶石镶嵌至晶石孔中可为角色增加属性
●已镶嵌的晶石可#c0ad700shift+右键#cffffbe点击打开吸收界面
●拖拽已镶嵌晶石进背包可取下晶石
]]
	WatchSparFrameDescRich:SetText( szText, 255, 255, 190 )
end

function WatchSparFrame_OnShow()
	UpdateWatchSparFrameInfo();
	WatchSparFrameRoleAttrBtn:Show();
end

function WatchSparFrameItemBtn_OnEnter()
	if this:GetClientUserData( 0 ) > 0 then
		local item = GameWatchPlayer:getWatchSparItem( this:GetClientUserData( 0 ) - 1 )
		if item:getItemId() > 0 then
			ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item );		
		end
	else
		local lockTex = getglobal( this:GetName() .. "LockTexture" )
		if lockTex ~= nil and not lockTex:IsShown() then
			SetGameTooltips(this:GetName(),"晶石孔：可镶嵌晶石")
		end
	end
end

function WatchSparFrameItemBtn_OnClick()
	if arg1 == "RightButton" then
		return;
	end
end

function UpdateWatchSparFrameInfo()
	if bFarWatch == true then
		WatchPlayer = GameWatchPlayer:getWatchPlayer();
	else		
		WatchPlayer = ActorMgr:FindActor( GameWatchPlayer:getWatchPlayerMemId() );
	end
	if WatchPlayer == nil then
		return;
	end

	local modelTex = tSparModelTex[WatchPlayer:getGender()];
	WatchSparFramePlayerTex:SetTexture( modelTex[1] )
	WatchSparFramePlayerTex:SetSize( modelTex[2], modelTex[3] )
	WatchSparFramePlayerTex:SetPoint("topleft", "WatchSparFrame", "topleft", modelTex[4], modelTex[5] )
	WatchSparFrameNameFont:SetText( GetPlayerName(WatchPlayer:getName()) )
	WatchSparFrameLvFont:SetText( "等级："..WatchPlayer:getLv().."级" )
	WatchSparFrameRaceFont:SetText( "职业："..GetCareerName( WatchPlayer:getRace() ) )
	
	local spar = GameWatchPlayer:getWatchSparPkg()

	local nIndex = 1
	for i=1,spar.MaxNum do
		local btn = getglobal( "WatchSparFrameSparItemBtn" .. i )
		local tex = getglobal( "WatchSparFrameSparTex" .. i )
		local iconTexture = getglobal( btn:GetName() .. "IconTexture" )
		local lockTexture  = getglobal( btn:GetName() .. "LockTexture" )
		local bindTexture = getglobal( btn:GetName() .. "BindTexture" )
		local openFont = getglobal( btn:GetName() .. "OpenFont" )
		iconTexture:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" )
		btn:SetClientUserData( 0,0 )
		bindTexture:Hide()
		lockTexture:Hide()
		tex:Hide();
		openFont:SetText( "" )
		nIndex = nIndex + 1
	end
	for i=1,spar.Num do		
		-- 显示
		local item = GameWatchPlayer:getWatchSparItem(i-1 )
		if item:getItemId() > 0 then
			local btn = getglobal( "WatchSparFrameSparItemBtn" .. item:getGridIdx() + 1 )
			local tex = getglobal( "WatchSparFrameSparTex" .. item:getGridIdx() + 1 )
			local iconTexture = getglobal( btn:GetName() .. "IconTexture" )
			local lockTexture  = getglobal( btn:GetName() .. "LockTexture" )
			local bindTexture = getglobal( btn:GetName() .. "BindTexture" )
			local openFont = getglobal( btn:GetName() .. "OpenFont" )
			btn:SetClientUserData( 0,i )
			local itemDef = item:getItemDef()
			local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end	
			iconTexture:SetTexture( IconPath )
			if item:isItemBind() then
				bindTexture:Show()
			else
				bindTexture:Hide()
			end
			lockTexture:Hide()
			tex:Show()
		end
	end
	for i=nIndex,MAX_SPAR_PKG_TOLUA do
		local btn = getglobal( "WatchSparFrameSparItemBtn" .. i )
		local tex = getglobal( "WatchSparFrameSparTex" .. i )
		local iconTexture = getglobal( btn:GetName() .. "IconTexture" )
		local lockTexture  = getglobal( btn:GetName() .. "LockTexture" )
		local bindTexture = getglobal( btn:GetName() .. "BindTexture" )
		local openFont = getglobal( btn:GetName() .. "OpenFont" )
		btn:SetClientUserData( 0,0 )
		lockTexture:Show()
		local openLv = GameArmProce:getSparOpenLv( i-1 )
		openFont:SetText( openLv .. "级开启" )
		tex:Hide()
		iconTexture:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" )
		bindTexture:Hide()		
	end	
end

function UpdateWatchSparFrameRoleAttrBtn()
	if bFarWatch == true then
		WatchPlayer = GameWatchPlayer:getWatchPlayer();
	else		
		WatchPlayer = ActorMgr:FindActor( GameWatchPlayer:getWatchPlayerMemId() );
	end
	if WatchPlayer == nil then
		--WatchPlayerFrame:Hide();
		return;
	end
	local nCurExp		= WatchPlayer:getExp();
	local nLevelNextExp = WatchPlayer:getLvExp( WatchPlayer:getLv() );
	
	-- 更新HP和MP条
	local MAX_HP_LENGTH = 152;
	--显示面板标题
	-- 设置HP
	local maxHp		= WatchPlayer:getMaxHP();
	local hp		= WatchPlayer:getHP();
	hp				= math.max( math.min( hp, maxHp ), 0 );
	local fHpScale	= math.max( math.min( hp / maxHp, 1 ), 0 );
	local hpBtn		= getglobal( "WatchSparFrameRoleAttrBtnFirstBaseLifeAttrButton" );
	local hpTex		= getglobal(  hpBtn:GetName().."Tex" );
	hpTex:SetSize(  fHpScale * MAX_HP_LENGTH, 11 );
	
	-- 设置MP
	local maxMp		= WatchPlayer:getMaxMP();
	local mp		= WatchPlayer:getMP();
	mp				= math.max( math.min( mp, maxMp ), 0 );
	local fMpScale	= math.max( math.min( mp / maxMp, 1 ), 0 );
	local mpBtn		= getglobal( "WatchSparFrameRoleAttrBtnFirstBaseMagicAttrButton" );
	local mpTex		= getglobal(  mpBtn:GetName().."Tex" );
	mpTex:SetSize( fMpScale * MAX_HP_LENGTH, 11 );

	local t_fonts = {	[hpBtn:GetName().."PercentFont"] = { value = hp.."/"..maxHp }, 
						[mpBtn:GetName().."PercentFont"] = { value = mp.."/"..maxMp },
					};
	util.SetFonts( t_fonts );

	UpdateWatchSparBaseAttr();
	-- 若选择的高级属性	IsEnable
	if WatchSparFrameRoleAttrBtn_AdvanceAttrLabelBtn:IsChecked() then
		UpdateWatchSparNormalAttackAttr();
		UpdateWatchSparNormalDefendAttr();
		Private_ShowWatchSparAdvanceAttr();
	else
		UpdateWatchSparSpecialAttackAttr();
		UpdateWatchSparSpecialDefendAttr();
		UpdateWatchSparFuZhuAttr();
		Private_ShowWatchSparYuanSuAttr();
	end
	UpdateWatchSparSuitAttr();
end

-- 更新基础属性
function ClearWatchSparBaseAttr()
	for i = 1, MAX_BASE_ATTR_NUM do
		local btn = getglobal( "WatchSparFrameRoleAttrBtn_BaseAttrBtn"..i );
		btn:Hide();
	end
end

function UpdateWatchSparBaseAttr()
	ClearWatchSparBaseAttr();
	local WatchPlayer = nil;
	if bFarWatch == true then
		WatchPlayer = GameWatchPlayer:getWatchPlayer();
	else
		WatchPlayer = ActorMgr:FindActor( GameWatchPlayer:getWatchPlayerMemId() );
	end

	local initRace		= GetInitRace( WatchPlayer:getRace() );
	
	local t_attrList	= t_RoleAttrInfo["基础属性"][initRace];
	local maxAttrNum	= t_attrList["maxElemNum"];
	for i = 1, maxAttrNum do
		local oneElem		= t_attrList[i];
		local funcGetBase	= oneElem["getBaseAttrFunc"] ~= nil and oneElem["getBaseAttrFunc"]	or GetRoleBaseAttr;
		local funcGetCur	= oneElem["getCurAttrFunc"] ~= nil	and oneElem["getCurAttrFunc"]	or GetRoleAttr;

		local nCurValue		= funcGetCur( oneElem["attrID"],	WatchPlayer );
		local nBaseValue	= funcGetBase( oneElem["attrID"],	WatchPlayer );

		local t_color	= { r = 255, g = 254, b = 189 };
		if nCurValue > nBaseValue then
			t_color	= { r = 15, g = 255, b = 0 };
		elseif nCurValue < nBaseValue then
			t_color	= { r = 255, g = 0, b = 0 };
		end

		if oneElem["maxAttrID"] ~= nil then
			nCurMaxValue = funcGetCur( oneElem["maxAttrID"],	WatchPlayer );
			nBaseMaxValue = funcGetBase( oneElem["maxAttrID"],	WatchPlayer );
			if nCurMaxValue < nBaseMaxValue then
				t_color	= { r = 255, g = 0, b = 0 };
			end
			nCurValue = nCurValue .. "-" .. nCurMaxValue
		end

		local btn = getglobal( "WatchSparFrameRoleAttrBtn_BaseAttrBtn"..i );
		local funcGetTips = oneElem["getAttrTipsDescFunc"];
		if funcGetTips ~= nil then
			btn:SetClientString( funcGetTips( oneElem["attrID"], WatchPlayer ) );
		else
			btn:SetClientString( oneElem["desc"] );
		end

		local funcGetDesc = oneElem["getAttrDescFunc"];
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneElem["name"] }, 
							[btn:GetName().."ValueFont"]	= 
							{	value = funcGetDesc ~= nil and funcGetDesc( oneElem["attrID"], WatchPlayer ) or nCurValue,	
								color = t_color, 
							},
						};
		util.SetFonts( t_fonts );
		btn:Show();
	end

	local mainAttrFlagBtn		= getglobal( "WatchSparFrameRoleAttrBtnMainAttrFlagButton" );
	local t_changeCareerControl = GetChangeCareerControl();
	local nRace					= WatchPlayer:getRace();
	if t_changeCareerControl:isChangeCareer( { ["race"] = nRace } ) then
		local szAttrBtn = t_mainSparAttrConfig[nRace]["btn"];
		mainAttrFlagBtn:SetPoint( "left", szAttrBtn, "left", 35, 0 );
		mainAttrFlagBtn:Show();
	else
		mainAttrFlagBtn:Hide();
	end
end

-- 更新普通攻击属性
function ClearWatchSparNormalAttackAttr()
	for i = 1, MAX_NORMAL_ATTACK_ATTR_NUM do
		local btn = getglobal( "WatchSparFrameRoleAttrBtn_AttackAttrBtn"..i );
		btn:Hide();
	end
end

function UpdateWatchSparNormalAttackAttr()
	ClearWatchSparNormalAttackAttr();
	
	local WatchPlayer = nil;
	if bFarWatch == true then
		WatchPlayer = GameWatchPlayer:getWatchPlayer();
	else
		WatchPlayer = ActorMgr:FindActor( GameWatchPlayer:getWatchPlayerMemId() );
	end

	local initRace		= GetInitRace( WatchPlayer:getRace() );
	local t_attrList	= t_RoleAttrInfo["攻击属性"][initRace];
	local maxAttrNum	= t_attrList["maxElemNum"];

	for i = 1, maxAttrNum do
		local oneElem		= t_attrList[i];
		local funcGetBase	= oneElem["getBaseAttrFunc"] ~= nil and oneElem["getBaseAttrFunc"]	or GetRoleBaseAttr;
		local funcGetCur	= oneElem["getCurAttrFunc"] ~= nil	and oneElem["getCurAttrFunc"]	or GetRoleAttr;

		local nCurValue		= funcGetCur( oneElem["attrID"],	WatchPlayer );
		local nBaseValue	= funcGetBase( oneElem["attrID"],	WatchPlayer );

		local t_color	= { r = 255, g = 254, b = 189 };
		if nCurValue > nBaseValue then
			t_color	= { r = 15, g = 255, b = 0 };
		elseif nCurValue < nBaseValue then
			t_color	= { r = 255, g = 0, b = 0 };
		end

		if oneElem["maxAttrID"] ~= nil then
			nCurMaxValue = funcGetCur( oneElem["maxAttrID"],	WatchPlayer );
			nBaseMaxValue = funcGetBase( oneElem["maxAttrID"],	WatchPlayer );
			if nCurMaxValue < nBaseMaxValue then
				t_color	= { r = 255, g = 0, b = 0 };
			end
			nCurValue = nCurValue .. "-" .. nCurMaxValue
		end

		local btn = getglobal( "WatchSparFrameRoleAttrBtn_AttackAttrBtn"..i );

		local funcGetTips = oneElem["getAttrTipsDescFunc"];
		if funcGetTips ~= nil then
			btn:SetClientString( funcGetTips( oneElem["attrID"], WatchPlayer ) );
		else
			btn:SetClientString( oneElem["desc"] );
		end

		local funcGetDesc = oneElem["getAttrDescFunc"];
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneElem["name"] }, 
							[btn:GetName().."ValueFont"]	= 
							{	value = funcGetDesc ~= nil and funcGetDesc( oneElem["attrID"], WatchPlayer ) or nCurValue,	
								color = t_color, 
							},
						};
		util.SetFonts( t_fonts );
		btn:Show();
	end
end

-- 更新普通防御属性
function ClearWatchSparNormalDefendAttr()
	for i = 1, MAX_NORMAL_DEFEND_ATTR_NUM do	
		local btn = getglobal( "WatchSparFrameRoleAttrBtn_DefendAttrBtn"..i );
		btn:Hide();
	end
end

function UpdateWatchSparNormalDefendAttr()
	ClearWatchSparNormalDefendAttr();
	
	local WatchPlayer = nil;
	if bFarWatch == true then
		WatchPlayer = GameWatchPlayer:getWatchPlayer();
	else
		WatchPlayer = ActorMgr:FindActor( GameWatchPlayer:getWatchPlayerMemId() );
	end

	local initRace		= GetInitRace( WatchPlayer:getRace() );
	
	local t_attrList	= t_RoleAttrInfo["防御属性"][initRace];
	local maxAttrNum	= t_attrList["maxElemNum"];

	for i = 1, maxAttrNum do
		local oneElem		= t_attrList[i];
		local funcGetBase	= oneElem["getBaseAttrFunc"] ~= nil and oneElem["getBaseAttrFunc"]	or GetRoleBaseAttr;
		local funcGetCur	= oneElem["getCurAttrFunc"] ~= nil	and oneElem["getCurAttrFunc"]	or GetRoleAttr;

		local nCurValue		= funcGetCur( oneElem["attrID"],	WatchPlayer );
		local nBaseValue	= funcGetBase( oneElem["attrID"],	WatchPlayer );

		local t_color	= { r = 255, g = 254, b = 189 };
		if nCurValue > nBaseValue then
			t_color	= { r = 15, g = 255, b = 0 };
		elseif nCurValue < nBaseValue then
			t_color	= { r = 255, g = 0, b = 0 };
		end

		if oneElem["maxAttrID"] ~= nil then
			nCurMaxValue = funcGetCur( oneElem["maxAttrID"],	WatchPlayer );
			nBaseMaxValue = funcGetBase( oneElem["maxAttrID"],	WatchPlayer );
			if nCurMaxValue < nBaseMaxValue then
				t_color	= { r = 255, g = 0, b = 0 };
			end
			nCurValue = nCurValue .. "-" .. nCurMaxValue
		end

		local btn			= getglobal( "WatchSparFrameRoleAttrBtn_DefendAttrBtn"..i );
		local funcGetTips	= oneElem["getAttrTipsDescFunc"];

		if funcGetTips ~= nil then
			btn:SetClientString( funcGetTips( oneElem["attrID"], WatchPlayer ) );
		else
			btn:SetClientString( oneElem["desc"] );
		end

		local funcGetDesc	= oneElem["getAttrDescFunc"];
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneElem["name"] }, 
							[btn:GetName().."ValueFont"]	= 
							{	value = funcGetDesc ~= nil and funcGetDesc( oneElem["attrID"], WatchPlayer ) or nCurValue,	
								color = t_color, 
							},
						};
		util.SetFonts( t_fonts );
		btn:Show();
	end
end

-- 更新特殊攻击属性
function ClearWatchSparYuanSuAttackAttr()
	for i = 1, MAX_YUAN_SU_ATTACK_ATTR_NUM do
		local btn = getglobal( "WatchSparFrameRoleAttrBtn_YuanSuAttackAttrBtn"..i );
		btn:Hide();
	end
end

function UpdateWatchSparSpecialAttackAttr()
	ClearWatchSparYuanSuAttackAttr();
	
	local WatchPlayer = nil;
	if bFarWatch == true then
		WatchPlayer = GameWatchPlayer:getWatchPlayer();
	else
		WatchPlayer = ActorMgr:FindActor( GameWatchPlayer:getWatchPlayerMemId() );
	end

	local initRace		= GetInitRace( WatchPlayer:getRace() );
	
	local t_attrList	= t_RoleAttrInfo["元素攻击力"][initRace];
	local maxAttrNum	= t_attrList["maxElemNum"];

	for i = 1, maxAttrNum do
		local oneElem		= t_attrList[i];
		local funcGetBase	= oneElem["getBaseAttrFunc"] ~= nil and oneElem["getBaseAttrFunc"]	or GetRoleBaseAttr;
		local funcGetCur	= oneElem["getCurAttrFunc"] ~= nil	and oneElem["getCurAttrFunc"]	or GetRoleAttr;

		local nCurValue		= funcGetCur( oneElem["attrID"],	WatchPlayer );
		local nBaseValue	= funcGetBase( oneElem["attrID"],	WatchPlayer );

		local t_color	= { r = 255, g = 254, b = 189 };
		if nCurValue > nBaseValue then
			t_color	= { r = 15, g = 255, b = 0 };
		elseif nCurValue < nBaseValue then
			t_color	= { r = 255, g = 0, b = 0 };
		end

		if oneElem["maxAttrID"] ~= nil then
			nCurMaxValue = funcGetCur( oneElem["maxAttrID"],	WatchPlayer );
			nBaseMaxValue = funcGetBase( oneElem["maxAttrID"],	WatchPlayer );
			if nCurMaxValue < nBaseMaxValue then
				t_color	= { r = 255, g = 0, b = 0 };
			end
			nCurValue = nCurValue .. "-" .. nCurMaxValue
		end

		local btn = getglobal( "WatchSparFrameRoleAttrBtn_YuanSuAttackAttrBtn"..i );
		btn:SetClientString( oneElem["desc"] );
		local funcGetDesc = oneElem["getAttrDescFunc"];
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneElem["name"] }, 
							[btn:GetName().."ValueFont"]	= 
							{	value = funcGetDesc ~= nil and funcGetDesc( oneElem["attrID"], WatchPlayer ) or nCurValue,	
								color = t_color, 
							},
						};
		util.SetFonts( t_fonts );
		btn:Show();
	end
end

-- 更新特殊防御属性
function ClearWatchSparYuanSuDefendAttr()
	for i = 1, MAX_YUAN_SU_DEFEND_ATTR_NUM do
		local btn = getglobal( "WatchSparFrameRoleAttrBtn_YuanSuDefendAttrBtn"..i );
		btn:Hide();
	end
end

function UpdateWatchSparSpecialDefendAttr()
	ClearWatchSparYuanSuDefendAttr();
	
	local WatchPlayer = nil;
	if bFarWatch == true then
		WatchPlayer = GameWatchPlayer:getWatchPlayer();
	else
		WatchPlayer = ActorMgr:FindActor( GameWatchPlayer:getWatchPlayerMemId() );
	end

	local initRace		= GetInitRace( WatchPlayer:getRace() );
	
	local t_attrList	= t_RoleAttrInfo["元素防御力"][initRace];
	local maxAttrNum	= t_attrList["maxElemNum"];

	for i = 1, maxAttrNum do
		local oneElem		= t_attrList[i];
		local funcGetBase	= oneElem["getBaseAttrFunc"] ~= nil and oneElem["getBaseAttrFunc"]	or GetRoleBaseAttr;
		local funcGetCur	= oneElem["getCurAttrFunc"] ~= nil	and oneElem["getCurAttrFunc"]	or GetRoleAttr;

		local nCurValue		= funcGetCur( oneElem["attrID"],	WatchPlayer );
		local nBaseValue	= funcGetBase( oneElem["attrID"],	WatchPlayer );

		local t_color	= { r = 255, g = 254, b = 189 };
		if nCurValue > nBaseValue then
			t_color	= { r = 15, g = 255, b = 0 };
		elseif nCurValue < nBaseValue then
			t_color	= { r = 255, g = 0, b = 0 };
		end

		if oneElem["maxAttrID"] ~= nil then
			nCurMaxValue = funcGetCur( oneElem["maxAttrID"],	WatchPlayer );
			nBaseMaxValue = funcGetBase( oneElem["maxAttrID"],	WatchPlayer );
			if nCurMaxValue < nBaseMaxValue then
				t_color	= { r = 255, g = 0, b = 0 };
			end
			nCurValue = nCurValue .. "-" .. nCurMaxValue
		end

		local btn			= getglobal( "WatchSparFrameRoleAttrBtn_YuanSuDefendAttrBtn"..i );
		btn:SetClientString( oneElem["desc"] );
		local funcGetDesc	= oneElem["getAttrDescFunc"];
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneElem["name"] }, 
							[btn:GetName().."ValueFont"]	= 
							{	value = funcGetDesc ~= nil and funcGetDesc( oneElem["attrID"], WatchPlayer ) or nCurValue,	
								color = t_color, 
							},
						};
		util.SetFonts( t_fonts );
		btn:Show();
	end
end

function ClearWatchSparFuZhuAttr()
	for i = 1, MAX_FU_ZHU_ATTR_NUM do
		local btn = getglobal( "WatchSparFrameRoleAttrBtn_FuZhuAttrBtn"..i );
		btn:Hide();
	end
end

-- 更新套装属性
function ClearWatchSparSuitAttr()
	for i = 1, MAX_SUIT_ATTR_NUM do
		local btn = getglobal( "WatchSparFrameRoleAttrBtn_SuitAttrBtn"..i );
		btn:Hide();
	end
	
	local t_hideIcons = { "WatchSparFrameRoleAttrBtnSuitBackGroundButton", };
	util.HideIcons( t_hideIcons );
end

function UpdateWatchSparFuZhuAttr()
	ClearWatchSparFuZhuAttr();
	
	local WatchPlayer = nil;
	if bFarWatch == true then
		WatchPlayer = GameWatchPlayer:getWatchPlayer();
	else
		WatchPlayer = ActorMgr:FindActor( GameWatchPlayer:getWatchPlayerMemId() );
	end

	local initRace		= GetInitRace( WatchPlayer:getRace() );
	
	local t_attrList	= t_RoleAttrInfo["辅助属性"][initRace];
	local maxAttrNum	= t_attrList["maxElemNum"];

	for i = 1, maxAttrNum do
		local oneElem		= t_attrList[i];
		local funcGetBase	= oneElem["getBaseAttrFunc"] ~= nil and oneElem["getBaseAttrFunc"]	or GetRoleBaseAttr;
		local funcGetCur	= oneElem["getCurAttrFunc"] ~= nil	and oneElem["getCurAttrFunc"]	or GetRoleAttr;

		local nCurValue		= funcGetCur( oneElem["attrID"],	WatchPlayer );
		local nBaseValue	= funcGetBase( oneElem["attrID"],	WatchPlayer );

		local t_color	= { r = 255, g = 254, b = 189 };
		if nCurValue > nBaseValue then
			t_color	= { r = 15, g = 255, b = 0 };
		elseif nCurValue < nBaseValue then
			t_color	= { r = 255, g = 0, b = 0 };
		end

		if oneElem["maxAttrID"] ~= nil then
			nCurMaxValue = funcGetCur( oneElem["maxAttrID"],	WatchPlayer );
			nBaseMaxValue = funcGetBase( oneElem["maxAttrID"],	WatchPlayer );
			if nCurMaxValue < nBaseMaxValue then
				t_color	= { r = 255, g = 0, b = 0 };
			end
			nCurValue = nCurValue .. "-" .. nCurMaxValue
		end

		local btn = getglobal( "WatchSparFrameRoleAttrBtn_FuZhuAttrBtn"..i );

		local funcGetTips = oneElem["getAttrTipsDescFunc"];
		if funcGetTips ~= nil then
			btn:SetClientString( funcGetTips( oneElem["attrID"], WatchPlayer ) );
		else
			btn:SetClientString( oneElem["desc"] );
		end

		local funcGetDesc	= oneElem["getAttrDescFunc"];
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneElem["name"] }, 
							[btn:GetName().."ValueFont"]	= 
							{	value = funcGetDesc ~= nil and funcGetDesc( oneElem["attrID"], WatchPlayer ) or nCurValue,	
								color = t_color, 
							},
						};
		util.SetFonts( t_fonts );
		btn:Show();
	end
end

function UpdateWatchSparSuitAttr()
	ClearWatchSparSuitAttr();
	
	local t_SuitList = {};
	local isHaveSuitFunc = 
	function ( itemDef )
		for _, data in ipairs( t_SuitList ) do
			if itemDef.SuitSetID == data["suitID"] then
				return true;
			end
		end

		return false;
	end
	
	for i = 1, MAXEQUIPPOS do
		local bagButton	= getglobal( "WatchPlayerFrameEquip"..i);
		local nWearPos 	= GetWatchFashionSuitPos( bagButton:GetClientID() );
		local Item		= GameWatchPlayer:getItem( nWearPos );
		local itemDef	= Item:getItemDef();
		if itemDef ~= nil and itemDef.SuitSetID > 0 then
			if not isHaveSuitFunc( itemDef ) then
				local SuitSet		= getSuitSet( itemDef.SuitSetID );
				local t_SuitColor	= GetItemNameColor(itemDef);

				local t_oneSuitElem = 
				{	name	= SuitSet.Name,
					value	= GetWatchPlayerSuitArmNum( itemDef.SuitSetID ) .. "/"..getSuitEquipNum( itemDef.SuitSetID ),
					color	= { r = t_SuitColor[1], g = t_SuitColor[2], b = t_SuitColor[3] },
					suitID	= itemDef.SuitSetID,
				};
				table.insert( t_SuitList, t_oneSuitElem );
			end
		end
	end
	
	-- 先设置装备按钮数据
	for i = 1, table.getn( t_SuitList ) do
		local oneSuitData	= t_SuitList[i];
		local btn			= getglobal( "WatchSparFrameRoleAttrBtn_SuitAttrBtn"..i );
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneSuitData["name"],	color = oneSuitData["color"],  }, 
							[btn:GetName().."ValueFont"]	= {	value = oneSuitData["value"],	color = oneSuitData["color"], },
						};
		util.SetFonts( t_fonts );
		btn:Show();
	end
	
	local ONE_SUIT_BTN_HEIGHT			= 16;
	local EXTEND_FRAME_SIZE_LIMIT_NUM	= 2;
	local nSuitNum	= table.getn( t_SuitList );
	if nSuitNum > 0 then
		WatchSparFrameRoleAttrBtnSuitBackGroundButton:Show();
		WatchSparFrameRoleAttrBtnSuitBackGroundButton:SetHeight( math.max( EXTEND_FRAME_SIZE_LIMIT_NUM, table.getn( t_SuitList ) ) * ONE_SUIT_BTN_HEIGHT );
	end
	
	if nSuitNum > EXTEND_FRAME_SIZE_LIMIT_NUM then
		local size = 456 - 32 - MAX_BOTTOM_LEAVE_SPACE +  nSuitNum * 16;
		WatchSparFrameRoleAttrBtn:SetHeight( size );
	else
		WatchSparFrameRoleAttrBtn:SetHeight( 456 );
	end	
end

function Private_ShowWatchSparAdvanceAttr()
	ClearWatchSparYuanSuDefendAttr();
	ClearWatchSparFuZhuAttr();
	ClearWatchSparYuanSuAttackAttr();

	local t_hideIcons = { "WatchSparFrameRoleAttrBtnSecondBig_YuanSuAttackAttrBackGroundButton", "WatchSparFrameRoleAttrBtnSecondBig_YuanSuDefendAttrBackGroundButton",	
							"WatchSparFrameRoleAttrBtnSecondBig_FuZhuAttrBackGroundButton" };
	util.HideIcons( t_hideIcons );

	local t_showIcons = { "WatchSparFrameRoleAttrBtnSecondBig_FirstChildAttrBackGroundButton", "WatchSparFrameRoleAttrBtnSecondBig_SecondChildAttrBackGroundButton" };
	util.ShowIcons( t_showIcons );
end

function Private_ShowWatchSparYuanSuAttr()
	ClearWatchSparNormalDefendAttr();
	ClearWatchSparNormalAttackAttr();

	local t_showIcons = { "WatchSparFrameRoleAttrBtnSecondBig_YuanSuAttackAttrBackGroundButton", "WatchSparFrameRoleAttrBtnSecondBig_YuanSuDefendAttrBackGroundButton",	
							"WatchSparFrameRoleAttrBtnSecondBig_FuZhuAttrBackGroundButton" };
	util.ShowIcons( t_showIcons );

	local t_hideIcons = { "WatchSparFrameRoleAttrBtnSecondBig_FirstChildAttrBackGroundButton", "WatchSparFrameRoleAttrBtnSecondBig_SecondChildAttrBackGroundButton" };
	util.HideIcons( t_hideIcons );
end

function WatchSparFrameRoleAttrBtn_OnShow()
	WatchSparFrameRoleAttrBtn_AdvanceAttrLabelBtn:Checked();
	WatchSparFrameRoleAttrBtn_SpecialAttrLabelBtn:DisChecked();
	UpdateWatchSparFrameRoleAttrBtn();
end

function WatchSparFrameRoleAttrBtn_SpecialAttrLabelBtn_OnClick()
	WatchSparFrameRoleAttrBtn_AdvanceAttrLabelBtn:DisChecked();
	WatchSparFrameRoleAttrBtn_SpecialAttrLabelBtn:Checked();
	
	Private_ShowWatchSparYuanSuAttr();

	UpdateWatchSparSpecialAttackAttr();
	UpdateWatchSparSpecialDefendAttr();
	UpdateWatchSparFuZhuAttr();	

end

function WatchSparFrameRoleAttrBtn_AdvanceAttrLabelBtn_OnClick()
	WatchSparFrameRoleAttrBtn_AdvanceAttrLabelBtn:Checked();
	WatchSparFrameRoleAttrBtn_SpecialAttrLabelBtn:DisChecked();
	
	Private_ShowWatchSparAdvanceAttr();
	UpdateWatchSparNormalAttackAttr();
	UpdateWatchSparNormalDefendAttr();

	WatchSparFrameRoleAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont1:SetText("攻");
	WatchSparFrameRoleAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont2:SetText("击");
	WatchSparFrameRoleAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont3:SetText("属");
	WatchSparFrameRoleAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont4:SetText("性");
	WatchSparFrameRoleAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont5:SetText("");
	WatchSparFrameRoleAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont1:SetText("防");
	WatchSparFrameRoleAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont2:SetText("御");
	WatchSparFrameRoleAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont3:SetText("属");
	WatchSparFrameRoleAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont4:SetText("性");
	WatchSparFrameRoleAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont5:SetText("");

end

function WatchSparFrameEquipBtn_OnClick()
	WatchPlayerFrame:Show()
	WatchPlayerFrame:SetPoint("topleft", "UIClient", "topleft",WatchSparFrame:GetRealLeft(), WatchSparFrame:GetRealTop());
	WatchSparFrame:Hide()
end

function WatchSparFrameFashionBtn_OnClick()
	WatchPlayerFrame:Show()
	WatchPlayerFrame:SetPoint("topleft", "UIClient", "topleft",WatchSparFrame:GetRealLeft(), WatchSparFrame:GetRealTop());
	WatchSparFrame:Hide()
	ShowWatchPlayerFashionFrames( true );
	WatchPlayerFrame_FashionBtn:Checked();
	WatchPlayerFrame_EquipBtn:DisChecked();
end

local szTipsText = "";
local szNextTips = "";
local MIN_ARM_STAR_NUM = 3
local MID_ARM_STAR_NUM = 6
local MAX_ARM_STAR_NUM = 9
local MAX_ARM_HIDE_ATTR_RESULT = 15;
local MAX_ARM_PERFECT_NUM = 12;
--1：主手，2：头盔，3：衣，5：副手，6：面，7：肩，12/13：戒指，15：足，17：项链，20/21：护符
local t_perfectEuqipIdx={ 1, 2, 3, 5, 6, 7, 12, 13, 15, 17, 20, 21, };

function UpdateWatchPlayerFrameWearEquipAttrStarButtonInfo()
	local perfectStarTex = getglobal( "WatchPlayerFrameWearEquipAttrStarButtonTex" );
	local EquipStarNum = 12;
	for i = 1, MAX_ARM_PERFECT_NUM do
		local Item	= GameWatchPlayer:getItem(t_perfectEuqipIdx[i]);
		if not Item:isArmFengYin() then
			local perfectStarNum = Item:getArmPefectStarNum();
			if perfectStarNum < EquipStarNum then
				EquipStarNum = perfectStarNum;
			end 
		end
	end
	szTipsText = "#c0096ff			完美强化隐藏属性";
	szNextTips = "#c787878			下一阶段套装："
	local lastInfo = "";
	local bGray = false;
	if EquipStarNum < MIN_ARM_STAR_NUM then
		szTipsText = szTipsText.."\n#c787878	12件完美3星装备（未激活）  \n";
		EquipStarNum = 3;
		bGray = true;
		lastInfo = "\n\n	待激活效果";
		perfectStarTex:SetTexUV( 1002, 147, 22, 20 );
		perfectStarTex = getglobal( "WatchSparFrameWearEquipAttrStarButtonTex" );
		perfectStarTex:SetTexUV( 1002, 147, 22, 20 );
	elseif EquipStarNum == MAX_ARM_STAR_NUM then
		szTipsText = szTipsText.."\n#cffff00	12件完美9星装备#cff9600(已激活)\t#cffff00\n";
		lastInfo = "\n\n#cb464ff	已激活最高阶效果";
		perfectStarTex:SetTexUV( 979, 126, 22, 20 );
		perfectStarTex = getglobal( "WatchSparFrameWearEquipAttrStarButtonTex" );
		perfectStarTex:SetTexUV( 979, 126, 22, 20 );
	else
		szTipsText = szTipsText.."\n#cffff00\t 12件完美" .. EquipStarNum .. "星装备#cff9600(已激活)\t#cffff00\n";
		lastInfo = "\n\n#cff9600 下一阶套装：12件完美" .. EquipStarNum + 1 .. "星装备";
		perfectStarTex:SetTexUV( 1002, 126, 22, 20 );	
		perfectStarTex = getglobal( "WatchSparFrameWearEquipAttrStarButtonTex" );
		perfectStarTex:SetTexUV( 1002, 126, 22, 20 );
	end
	local WatchPlayer = nil;
	if bFarWatch == true then
		WatchPlayer = GameWatchPlayer:getWatchPlayer();
	else
		WatchPlayer = ActorMgr:FindActor( GameWatchPlayer:getWatchPlayerMemId() );
	end
	if WatchPlayer == nil then
		return;
	end
	local armHideAttrDef = getArmHideAttrDef( WatchPlayer:getRace(), EquipStarNum );
	for i = 1, MAX_ARM_HIDE_ATTR_RESULT do
		if armHideAttrDef.Result[i-1].ResultID ~= 0 then
			local resultConfig = GetOneItemResultConfig( { ["resultID"] = armHideAttrDef.Result[i-1].ResultID } )
			if resultConfig ~= nil then
				local val = ( armHideAttrDef.Result[i-1].ResultID == RESULT_POFANG_LEVEL or armHideAttrDef.Result[i-1].ResultID == RESULT_DIKANG_LEVEL ) and math.floor(armHideAttrDef.Result[i-1].ResultVal1 / 10 * math.floor(WatchPlayer:getLv() / 10) ^ 2 / 3) or armHideAttrDef.Result[i-1].ResultVal1;
				local desc = ( armHideAttrDef.Result[i-1].ResultID == RESULT_POFANG_LEVEL or armHideAttrDef.Result[i-1].ResultID == RESULT_DIKANG_LEVEL ) and resultConfig[2] .. string.format( "(Lv%d)", math.floor(WatchPlayer:getLv() / 10)*10 ) or resultConfig[2];
				szTipsText = szTipsText .. "\n		" .. ( ( not bGray and i > 9 ) and "#c5af0ff" or "" ) .. desc .. ":+" ..val .. (IsPercentResultID(armHideAttrDef.Result[i-1].ResultID) and "%" or "")
			end
		end
	end
	
	if not bGray and EquipStarNum < MAX_ARM_STAR_NUM then
		EquipStarNum = EquipStarNum + 1;
		szNextTips = szNextTips .. "\n\t 12件完美" .. EquipStarNum .. "星装备(未激活)  \n";
		armHideAttrDef = getArmHideAttrDef( WatchPlayer:getRace(), EquipStarNum );
		for i = 1, MAX_ARM_HIDE_ATTR_RESULT do
		if armHideAttrDef.Result[i-1].ResultID ~= 0 then
				local resultConfig = GetOneItemResultConfig( { ["resultID"] = armHideAttrDef.Result[i-1].ResultID } )
				if resultConfig ~= nil then
					local val = ( armHideAttrDef.Result[i-1].ResultID == RESULT_POFANG_LEVEL or armHideAttrDef.Result[i-1].ResultID == RESULT_DIKANG_LEVEL ) and math.floor(armHideAttrDef.Result[i-1].ResultVal1 / 10 * math.floor(WatchPlayer:getLv() / 10) ^ 2 / 3) or armHideAttrDef.Result[i-1].ResultVal1;
					local desc = ( armHideAttrDef.Result[i-1].ResultID == RESULT_POFANG_LEVEL or armHideAttrDef.Result[i-1].ResultID == RESULT_DIKANG_LEVEL ) and resultConfig[2] .. string.format( "(Lv%d)", math.floor(WatchPlayer:getLv() / 10)*10 ) or resultConfig[2];
					szNextTips = szNextTips .. "\n		" .. desc .. ":+" ..val .. (IsPercentResultID(armHideAttrDef.Result[i-1].ResultID) and "%" or "")
				end
			end
		end
		WatchPlayerFrameWearEquipAttrStarButton:SetClientUserData( 0, 1 );
		WatchSparFrameWearEquipAttrStarButton:SetClientUserData( 0, 1 );
	else
		szTipsText = szTipsText..lastInfo;
		WatchPlayerFrameWearEquipAttrStarButton:SetClientUserData( 0, 0 );
		WatchSparFrameWearEquipAttrStarButton:SetClientUserData( 0, 0 );
	end
end

function WatchPlayerFrameIsOutSideBtn_OnClick()
end

function WatchPlayerFrameIsOutSideBtn_OnEnter()
	local WatchPlayer = nil;
	if bFarWatch == true then
		WatchPlayer = GameWatchPlayer:getWatchPlayer();
	else
		WatchPlayer = ActorMgr:FindActor( GameWatchPlayer:getWatchPlayerMemId() );
	end
	if WatchPlayer == nil then
		return;
	end
	
	local mainplayer = ActorMgr:getMainPlayer()
	local worldId = mainplayer:GetWorldID()
	local btnTex = getglobal( this:GetName().."Texture" )
	if worldId == GameWatchPlayer:getWatchPlayerWorldID() or GameWatchPlayer:getWatchPlayerWorldID() == 0 then
		SetGameTooltips( this:GetName(), "他与您在同一个服务器" )
		btnTex:SetTexUV( 422, 172, 18, 24 )		
	else
		local worldName = GameWatchPlayer:getWatchPlayerWorldName();
		SetGameTooltips( this:GetName(), "来自["..worldName.."]的玩家" )
		btnTex:SetTexUV( 441, 172, 18, 24 )		
	end
	
end

function WatchPlayerFrameWearEquipAttrStarButton_onEnter()
	if this:GetClientUserData(0) > 0 then
		ShowPerfectStarTip( szTipsText, szNextTips, this:GetName() );
	else
		ShowGameTooltip( { rich = "WideGameTooltipRichText", tipsFrame = "WideGameTooltip", text = szTipsText, frame = this:GetParent(),
							button =  this:GetName() } );
	end
end

function WatchPlayerFrameDaoBaBtn_OnEnter()
	local WatchPlayer = nil;
	if bFarWatch == true then
		WatchPlayer = GameWatchPlayer:getWatchPlayer();
	else
		WatchPlayer = ActorMgr:FindActor( GameWatchPlayer:getWatchPlayerMemId() );
	end
	if WatchPlayer == nil then
		return;
	end
	local clanName = GameClanManager:clanGetClanNameByWid( WatchPlayer:getDaoBaClanWid() )
	--local name = WatchPlayer:getDaoBaName()
	
	if WatchPlayer:getDaoBaEnd() == 0 then
		return;
	end
	local timer = WatchPlayer:getDaoBaEnd() - getServerTime().sec;
	if timer <= 0 then
		return ;
	end
	local nHour = math.floor( timer/3600 )
	local nMinute =  math.floor( ( timer - nHour*3600 ) / 60 )
	--local nSecond = math.ceil( timer - nHour*3600 - nMinute*60 )
	local ClockTime = SetClockNum( nHour )..":"..SetClockNum( nMinute )
	
	local szShowInfo = "#c30ffff\t\t\t伤痕将见证荣誉的时刻！\n\n#cffffbe战斗中当双方均为据点公会的成员，\n伤痕让战败者时刻铭记着重拾荣誉！\n\n#cffffbe被#cff0000"..clanName.."#cffffbe公会的成员击败\n所能获得的公会贡献度将受到削减\n\n#c00ff00愈合时间:"..ClockTime.."\n\n#cff7404击杀对方公会成员，可提前愈合"
	SetGameTooltips( this:GetName(),  szShowInfo)
end                                                                                                                                                       

function WPAttrGenuis_OnEnter()
	if WPAttrGenuisFont:GetText() == "" then
		return;
	end

	local WatchPlayer = GameWatchPlayer:getWatchPlayer();
	if WatchPlayer == nil then return end;

	local geniusCareerDef = GeniusMgr:getGeniusCareerDef( WatchPlayer:getRace() )
	local minLevel = getMaxTianFuLv();
	if geniusCareerDef ~= nil then
		for i=1,MAX_GENIUS_SUBTYPE_TOLUA do
			local attrId = geniusCareerDef.Types[0].AttrIDs[i-1]
			if attrId > 0 then
				local lv = GameWatchPlayer:getAttrLv(0,attrId);
				minLevel = minLevel > lv and lv or minLevel;
			end
		end
	end

	UpdateTianFuLockTips( 1, minLevel, this:GetName(), WatchPlayer );
	TianFuLockTipsFrame:Show();
end

local MAX_FASHION_EQUIP_NUM = 6  --时装数量
--依次 时装位|胸、时装位|头、时装位|发、时装位|背、时装位|鞋、时装位|手、时装位|腿 
local t_fashionPos = {
					[1] = { 8, 22, 23,  25, 26, 27 },
					[2] = { 28, 29, 30,  32, 33, 34 },
					[3] = { 35, 36, 37,  39, 40, 41 },
					}
-- 该时装包至少有一件 时装
function WatchHasLeastOneFashionSuit( nIndex )
	for i=1, MAX_FASHION_EQUIP_NUM do 
		local Item	= GameWatchPlayer:getItem( t_fashionPos[nIndex][i] );
		if Item:getItemId() ~= 0 and not Item:isValidEnd() then 
			return true;	
		end
	end
	return false;
end