-- 快捷栏隐藏 过渡动画 变量
local nBeginHide		= 0;
local nDuration			= 1;
local bHideMainFrame	= false;
local bHideTempFrame	= false;
local bShowEnerge		= false;
local nBeginEnerge		= 0;

local bHideXPTex		= false;
local bHideXPBackTex	= false;
local nBeginHideXP		= 0;
local nBeginScale		= 0;
local nNextXPWidth		= 0;

local nCurTmpEnerge		= 0;
local bHideEnergeTex	= false;
local bHideBEnergeTex	= false;
local nBeginHideEnerge	= 0;

local bShowXPSkill		= false;
local nBeginShowXPSkill	= 0;

local bShowExtendSkill	= false;
local nBeginExtendSkill	= 0;

local bHideExtendSkill	= false;
local nHideExtendSkill	= 0;

local bSetExtendSkill	= false;

local bHideTempFairFrame		= false;
local bHideMainFrameByTempFairy = false;

local bShowWebFrame				= false;
local nBeginShowWebFrame		= 0;
local WEBFRAME_LOADINGTIME		= 1;

local MAX_TEMP_SKILL			= 2; 	 
local nBeginClock				= 0;

local nTempBeginMoveTime		= 0;	
bHideTmpFrame					= false;
local bInTmp					= false;

local szNeedChangeFrame			= "";
local nBeginMoveTime			= 0;
local bShowTmpFrame				= false;	 

local nTmpFairyBeginMoveTime	= 0;
local bHideTmpFairyFrame		= false;

local nResultValue				= 0;

local OreTypeNum				= 5;
local OreTypeWatchNum			= 5;

MAX_TMP_SKILL_BTN		= 4;
MAX_TMP_BTN				= 12;

function TempShortCutFrameEnergeGuide_OnShow()
	local MainPlayer = ActorMgr:getMainPlayer()
	local eqiup			= MainPlayer:getEquip();
	local nTmpItemId	= 0;
	if eqiup:getTmpArm() ~= 0 then
		nTmpItemId = eqiup:getTmpArm();
	else
		nTmpItemId = eqiup:getTmpMachin();
	end
	if nTmpItemId == 1460003 then
		--显示能量显示引导泡泡
		TempShortCutFrameEnergeGuide:Show()
		TempShortCutFrameEnergeGuideRich:SetText( "#此处为机械的能量，\n使用Q键上方的数字键“#R1#n”“#R2#n”\n可发动强悍的技能！", 20,30,120 )
	else
		TempShortCutFrameEnergeGuide:Hide()
		TempShortCutFrameEnergeGuideRich:SetText( "", 255,255,190 )
	end
	TempShortCutFrameEnergeGuide:SetClientUserData( 0, os.clock() )
end
function TempShortCutFrameEnergeGuide_OnUpdate()
	if os.clock() - TempShortCutFrameEnergeGuide:GetClientUserData( 0 ) > 10 then
		TempShortCutFrameEnergeGuide:Hide()
	end
end
function TempShortCutFrameEnergeGuide_OnHide()
	TempShortCutFrameEnergeGuide:SetClientUserData( 0, 0 )
end

function TempShortCutFrame_OnLoad()
	this:RegisterEvent("GE_BEGIN_ATTACK_OTHER");
end

function TempShortCutFrame_OnEvent()
	if (arg1 == "GE_BEGIN_ATTACK_OTHER") then
		local tempXinShouBtn = getglobal( "TempShortCutFrameAttackXinShouBtn" );
		if tempXinShouBtn:IsShown() then
			CheckTempShortCutFrameXinShou();
			tempXinShouBtn:Hide();
		end
	end
end

function TempShortCutFrame_OnShow()
	local MainPlayer = ActorMgr:getMainPlayer();
	local Container = MainPlayer:getContainer();

	ExtendSkillRightFrame:SetPoint("right","$parent","right",-1+100,60);   
	if ExtendSkillLeftFrame:IsShown() then	
		ExtendSkillLeftFrame:SetPoint("right","$parent","right",-47+100,60); 
	end

	TempShortCutFrame:SetPoint("bottom","$parent","bottom",0,-1+TempShortCutFrame:GetHeight()); 
	TempShortCutFrame:MoveFrame("top", MOVE_DURATION, TempShortCutFrame:GetWidth(), TempShortCutFrame:GetHeight());

	PhotoFrame:SetPoint( "topleft", "$parent", "topleft", -PhotoFrame:GetWidth(), 0 );
	PhotoFrame:MoveFrame("right", MOVE_DURATION, PhotoFrame:GetWidth(), PhotoFrame:GetHeight());

	SetTmpShortCutBtn( true );

	ShortCutButtonsFrame:SetPoint("bottom","$parent","bottom",0,ShortCutButtonsFrame:GetHeight());
	ShortCutButtonsFrame:MoveFrame("top", MOVE_DURATION, ShortCutButtonsFrame:GetWidth()+3, ShortCutButtonsFrame:GetHeight()-1);
	-- 隐藏指引提示TIPS窗口
	HideTempShortCutFrameGuideEffectTip();
	CheckShortcutEffect( "TempShortCutFrame_OnShow" );
	ClanCollectTmpFrame:Hide();
	ClanTankTmpFrame:Hide();
	NetWorkStatus:SetPoint( "topleft", "ShortCutButtonsFrame", "topleft",52,23);
	local UVATexture = getglobal( "TmpDropButtonUVAnimationTex" )
	UVATexture:Hide();
	
	--隐藏取消使用的提示泡泡
	TempShortCutFrameGuide:Hide()
	--显示能量泡泡提示
	TempShortCutFrameEnergeGuide:Show()
end

function HideTempShortCutFrameGuideEffectTip()
	local t_hideGuideEffect = { "TipsEffectFrame", "PetDialogFrame" }
	for _, szFrame in ipairs( t_hideGuideEffect ) do
		local frame = getglobal( szFrame );
		if ( szExceptFrame == nil and frame:IsShown() ) 
			or ( szExceptFrame ~= nil and szExceptFrame ~= szFrame and frame:IsShown() ) then
			frame:Hide();
		end
	end
end

function TempShortCutFrame_OnHide()
	local t_hideIcons = { "ShortCutButtonsFrameXinShouBtn", "TempShortCutFrameAttackXinShouBtn", "TempShortCutFrameEnergeGuide" };
	util.HideIcons( t_hideIcons );
end

function TempShortCutFrame_OnEndMove()
	CheckShortcutEffect( "TempShortCutFrame_OnEndMove" );
end

local t_AddHpItem = {};
local t_AddHpBuffItem = {};

local t_TmpBtnIndex = 
	{ 
		["LeftShortCut"]		= { clientid = { 5, 6, 7, 8 }  }, 
		["RightShortCut"]		= { clientid = { 11, 12, 13, 14 } }, 
		["ExtendTmpShortCut"]	= { 5, 6, 7, 8, 11, 12, 13, 14 }, 
		["MainTmpShortCut"]		= { 9, 10 }, 
	};

function SetTmpShortCutBtn( bFirstSet )	
	for _, data in ipairs(t_TmpBtnIndex["ExtendTmpShortCut"]) do
		ResetTmpShortCut( data )
	end

	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end

	local nLeftIdx		= 1;
	local nRightIdx		= 1;
	local container		= MainPlayer:getContainer();
	local eqiup			= MainPlayer:getEquip();
	local nMaxNum		= container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES );
	
	t_AddHpItem = {};
	t_AddHpBuffItem = {};
	for i = 1, nMaxNum do
		local item		= container:getItem( CONTAINER_TYPE_SUNDRIES, i-1 );
		local itemDef	= item:getItemDef();
		if itemDef ~= nil then
			for i = 1,MAX_ITEM_RESULT do
				if itemDef.Result[i-1].ResultID == RESULT_HP_ADD and MainPlayer:getLv() >= itemDef.WearPre.RoleLevelMin then
					local bHave = false;
					for i=1,table.getn(t_AddHpItem) do
						if t_AddHpItem[i][1] == itemDef.ItemID then
							bHave = true;
							break;
						end
					end
					if not bHave then
						table.insert( t_AddHpItem,{ itemDef.ItemID,itemDef.WearPre.RoleLevelMin } );
					end
					break;
				elseif itemDef.Result[i-1].ResultID == RESULT_SELF_STATUS and MainPlayer:getLv() >= itemDef.WearPre.RoleLevelMin then					
					local statusDef = getStatusDef( itemDef.Result[i-1].ResultVal2,itemDef.Result[i-1].ResultVal3 );
					if statusDef ~= nil then
						for j=1,MAX_ITEM_RESULT do
							--STATUS_RESULT_HPADD	25
							if statusDef.Result[j-1].ResultID == 25 then
								local bHave = false;
								for i=1,table.getn(t_AddHpBuffItem) do
									if t_AddHpBuffItem[i][1] == itemDef.ItemID then
										bHave = true;
										break;
									end
								end
								if not bHave then
									--table.insert( t_AddHpBuffItem,{itemDef.ItemID,statusDef.Result[j-1].ResultVal2 } )
									table.insert( t_AddHpBuffItem,{itemDef.ItemID,itemDef.WearPre.RoleLevelMin } )
								end
								break;
							end
						end
					end
				end			
			end
		end
	end
	table.sort( t_AddHpItem,CompareItemVal );
	table.sort( t_AddHpBuffItem,CompareItemVal );
	
	if table.getn( t_AddHpBuffItem ) > 0 then
		if bFirstSet or TempMagicShortcut9:GetClientUserData(1) == 0 then
			local nItemID = t_AddHpBuffItem[1][1];
			TempMagicShortcut9:SetClientUserData( 1, nItemID );
			TempMagicShortcut9:SetClientUserData( 2, t_AddHpBuffItem[1][2] );
			local itemDef = getItemDef( nItemID );
			local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
			
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			local nCount = container:getItemCount( nItemID );
			TempMagicShortcut9Icon:SetTexture( IconPath );
			TempMagicShortcut9CollectText:SetText( nCount );
			GameShortCut:setTmpShortCut( nItemID, 0, 8, 0,2 );
			table.remove( t_AddHpBuffItem,1 );
		else
			local id = TempMagicShortcut9:GetClientUserData( 1 );
			for i=1,table.getn( t_AddHpBuffItem ) do
				if t_AddHpBuffItem[i][1] == id then
					table.remove( t_AddHpBuffItem,i );
					break;
				end
			end
		end
	end
	for i=1,table.getn( t_AddHpBuffItem ) do
		if i > 4 then break end
		SetTmpShortCutUI( t_AddHpBuffItem[i],"LeftShortCut",i,t_TmpBtnIndex,container );			
	end
	if table.getn( t_AddHpItem ) > 0 then
		if bFirstSet or TempMagicShortcut10:GetClientUserData( 1 ) == 0 then
			local nItemID = t_AddHpItem[1][1];
			TempMagicShortcut10:SetClientUserData( 1, nItemID );
			TempMagicShortcut10:SetClientUserData( 2, t_AddHpItem[1][2] );
			local itemDef = getItemDef( nItemID );
			local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
			
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end

			local nCount = container:getItemCount( nItemID );
			TempMagicShortcut10Icon:SetTexture( IconPath );
			TempMagicShortcut10CollectText:SetText( nCount );
			GameShortCut:setTmpShortCut( nItemID, 0, 9, 0,2 );
			table.remove( t_AddHpItem,1 );
		else
			local id = TempMagicShortcut10:GetClientUserData( 1 );
			for i=1,table.getn( t_AddHpItem ) do
				if t_AddHpItem[i][1] == id then
					table.remove( t_AddHpItem,i );
					break;
				end
			end
		end
	end

	if container:getItemCount( TempMagicShortcut9:GetClientUserData(1) ) == 0 then
		TempMagicShortcut9:SetClientUserData(1,0);
	end

	if container:getItemCount( TempMagicShortcut10:GetClientUserData(1) ) == 0 then
		TempMagicShortcut10:SetClientUserData(1,0);
	end

	if TempMagicShortcut9:GetClientUserData(1) == 0 then
		ResetTmpShortCut( 9 );
	end
	if TempMagicShortcut10:GetClientUserData(1) == 0 then
		ResetTmpShortCut( 10 );
	end

	for i=1,table.getn( t_AddHpItem ) do
		if i > 4 then break end
		SetTmpShortCutUI( t_AddHpItem[i],"RightShortCut",i,t_TmpBtnIndex,container );
	end
end

function CompareItemVal(lhs,rhs)
	return lhs[2] > rhs[2];
end

function SetTmpShortCutUI( itemData, szType, idx, t_TmpBtn, container )
	local itemID = itemData[1];
	local clientid = t_TmpBtn[szType].clientid[idx];
	local TmpShortcutBtn			= getglobal("TempMagicShortcut"..clientid);
	local TmpShortcutIcon			= getglobal(TmpShortcutBtn:GetName().."Icon");
	local TmpShortcutBox			= getglobal(TmpShortcutBtn:GetName().."BoxTexture");
	local TmpShortcutCollectText	= getglobal(TmpShortcutBtn:GetName().."CollectText");
	if TmpShortcutBtn == nil then
		return;
	end
	TmpShortcutBtn:SetClientUserData( 1, itemID );	
	TmpShortcutBtn:SetClientUserData( 2, itemData[2] );
	local IconPath = GetItemEquipIconPath()..itemID..".tga";
	-- 如果没找到物品的图标则使用默认图标显示
	if not IsInExistence( IconPath ) then
		IconPath = DEFAULT_ITEM_PATH;
	end
	local nCount = container:getItemCount(itemID);
	TmpShortcutIcon:SetTexture( IconPath );
	local szCount = "";
	if nCount > 1 then
		szCount = nCount;
	end
	TmpShortcutCollectText:SetText( szCount );
end

function ResetTmpShortCut( index )
	local TmpShortcutBtn			= getglobal("TempMagicShortcut"..index);
	local TmpShortcutIcon			= getglobal(TmpShortcutBtn:GetName().."Icon");
	local TmpShortcutBox			= getglobal(TmpShortcutBtn:GetName().."BoxTexture");
	local TmpShortcutCollectText	= getglobal(TmpShortcutBtn:GetName().."CollectText");
	
	TmpShortcutBtn:SetCooldownTimer( 0, 0, true );
	TmpShortcutBtn:SetClientUserData( 1, 0 );
	TmpShortcutCollectText:SetText( "" );
	TmpShortcutIcon:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
end

function FairyShortCutFrame_OnShow()
	
    ExtendSkillRightFrame:SetPoint("right","$parent","right",-1+100,60);   
	if ExtendSkillLeftFrame:IsShown() then	
		ExtendSkillLeftFrame:SetPoint("right","$parent","right",-47+100,60); 
	end

	TempFairyShortCutFrame:SetPoint("bottom","$parent","bottom",0,-1+TempFairyShortCutFrame:GetHeight()); 
	TempFairyShortCutFrame:MoveFrame("top", MOVE_DURATION, TempFairyShortCutFrame:GetWidth(), TempFairyShortCutFrame:GetHeight());

	PhotoFrame:SetPoint( "topleft", "$parent", "topleft", -PhotoFrame:GetWidth(), 0 );
	PhotoFrame:MoveFrame("right", MOVE_DURATION, PhotoFrame:GetWidth(), PhotoFrame:GetHeight());
	
	ShortCutButtonsFrame:SetPoint("bottom","$parent","bottom",0,-1+ShortCutButtonsFrame:GetHeight());
	ShortCutButtonsFrame:MoveFrame("top", MOVE_DURATION, ShortCutButtonsFrame:GetWidth()+3, ShortCutButtonsFrame:GetHeight());

	-- 隐藏指引提示TIPS窗口
	HideFairyShortCutFrameGuideEffectTip(); 
	CheckShortcutEffect( "FairyShortCutFrame_OnShow" );

	if not GameWizard:isDo( GAMEWIZARD_FIST_TRUMP_QUEST_FAIRY_MORPH ) then
		if TempFairyMagicShortcut1:IsShown() then
			GuideEffectShow( "TempFairyShortCutFrame","","TempFairyMagicShortcut1", "施放必杀技能：\n1.鼠标左键点击该技能\n2.使用键盘快捷键“1”", "uires\\ui\\mask\\TouMingTongDao.tga", 1, 1, 1, 1, 50, 60, -9, 5, 11, -51, false );
			local frameUVATexture = getglobal( "TempFairyShortCutFrameEffectFrameUVAnimationTex" )
			frameUVATexture:SetSize(  74,150 )
			frameUVATexture:SetPoint("center", "TempFairyMagicShortcut1", "center", 2, 2);
			frameUVATexture:SetUVAnimation(50,true);	
			frameUVATexture:Show();
		end
		GameWizard:doStep( GAMEWIZARD_FIST_TRUMP_QUEST_FAIRY_MORPH );
	end
	TempShortCutFrame:Hide();
	ClanCollectTmpFrame:Hide();
	ClanTankTmpFrame:Hide();
end

-- 隐藏指引提示TIPS窗口
function HideFairyShortCutFrameGuideEffectTip( szExceptFrame ) 
	-- 在szExceptFrame为nil的时候隐藏所有的tips面板
	-- 在szExceptFrame不为nil的时候将与指定tips面板不同的面板隐藏
	local t_hideGuideEffect = { "TipsEffectFrame", "PetDialogFrame","TempFairyShortCutFrameEffectFrame" }
	for _, szFrame in ipairs( t_hideGuideEffect ) do
		local frame = getglobal( szFrame );
		if ( szExceptFrame == nil and frame:IsShown() ) 
			or ( szExceptFrame ~= nil and szExceptFrame ~= szFrame and frame:IsShown() ) then
			frame:Hide();
		end
	end
end

function TmpShortCutUpdateFrameOnLoad()
	this:RegisterEvent( "GE_TMPITME_GETTMPITEM" );
	this:RegisterEvent( "GE_CONTAINER_UPDATE" );
	this:RegisterEvent( "GE_TMPITME_LOOTTMPMACHIN" );
	this:RegisterEvent( "GE_TMPITME_LOOTTMPITEM" );
	this:RegisterEvent( "GE_TMPITME_TMPMACHIN" );
	this:RegisterEvent( "UI_ACTOR_ATTRIBUTE_CHG" );
	this:RegisterEvent( "GE_TMP_MAX_ENERGY_CHG" );
	this:RegisterEvent( "GE_TMPITME_TMPMACHIN_NOSKILL" );
end

function InitTmpFrameData()
	nBeginHide			= 0;
	nDuration			= 1;
	bHideMainFrame		= false;
	bHideTempFrame		= false;
	bShowEnerge			= false;
	nBeginEnerge		= 0;

	bHideXPTex			= false;
	bHideXPBackTex		= false;
	nBeginHideXP		= 0;
	nBeginScale			= 0;
	nNextXPWidth		= 0;

	nCurTmpEnerge		= 0;
	bHideEnergeTex		= false;
	bHideBEnergeTex		= false;
	nBeginHideEnerge	= 0;

	bShowExtendSkill	= false;
	nBeginExtendSkill	= 0;

	bHideExtendSkill	= false;
	nHideExtendSkill	= 0;

	bShowXPSkill		= false;
	nBeginShowXPSkill	= 0;

	bSetExtendSkill		= false;

	bHideTempFairFrame			= false;
	bHideMainFrameByTempFairy	= false;

	bShowWebFrame				= false;
	nBeginShowWebFrame			= 0;
	WEBFRAME_LOADINGTIME		= 2;

	bHideTmpFrame				= false;
	bInTmp						= false;  
	nTempBeginMoveTime			= 0;	

	szNeedChangeFrame			= "";
	nBeginMoveTime				= 0;
	bShowTmpFrame				= false;   

	nTmpFairyBeginMoveTime		= 0;
	bHideTmpFairyFrame			= false;

	ExtendSkillFrame:SetPoint("bottom","$parent","bottom",-387,-98);
	ShortCutFrame:SetPoint("bottom","$parent","bottom",0,-1); 	 
	ExtendSkillRightFrame:SetPoint("right","$parent","right",-1,60);   
	PhotoFrame:SetPoint( "topleft", "$parent", "topleft", 0, 0 );
	if ExtendSkillLeftFrame:IsShown() then	
		ExtendSkillLeftFrame:SetPoint("right","$parent","right",-47,60); 
	end
	ShortCutButtonsFrame:SetPoint( "bottom","$parent","bottom",0,-1 );
	TempShortCutFrame:Hide();
	TempFairyShortCutFrame:Hide();
	ClanCollectTmpFrame:Hide();
	ClanTankTmpFrame:Hide();
end

function ShowWebFrame()
	--[[
	if webframe:IsShown() then
		webframe:Hide();
	end
	--]]
	bShowWebFrame		= true;
	nBeginShowWebFrame	= os.clock();
end

function ShowExtendSkill()
end

function HideExtendSkill( bHide )
	bHideExtendSkill	= bHide;
	nHideExtendSkill	= os.clock();
end

function HideTempSkillBtn()
	for i = 1, 4 do
		local btn = getglobal("TempMagicShortcut"..i);
		btn:Hide();
		btn:SetClientUserData( 0, 0 );	
		btn:SetClientUserData( 1, 0 );	
	end
end

function TmpShortCutUpdateFrameOnEvent()
	if( arg1 == "GE_TMPITME_LOOTTMPMACHIN" ) or ( arg1 == "GE_TMPITME_LOOTTMPITEM" ) then
		for i = 1, MAX_TMP_SKILL_BTN do
			local btn = getglobal("TempMagicShortcut"..i);
			btn:SetClientUserData( 0, 0 );
			--btn:Hide();
		end

		if SpellTipsFrame:IsShown() then
			SpellTipsFrame:Hide();
		end
		
		PhotoFrame:SetPoint( "topleft", "$parent", "topleft", 0, 0 );
		PhotoFrame:MoveFrame("left", MOVE_DURATION, PhotoFrame:GetWidth(), PhotoFrame:GetHeight());
		TempShortCutFrame:MoveFrame( "bottom", MOVE_DURATION, TempShortCutFrame:GetWidth(), TempShortCutFrame:GetHeight() );	
		ShortCutButtonsFrame:MoveFrame( "bottom", MOVE_DURATION, TempShortCutFrame:GetWidth(), TempShortCutFrame:GetHeight() );
		HideGuideEffectTip();
		nTempBeginMoveTime = os.clock();
		bHideTmpFrame = true;

		nBeginClock = os.clock();
		bInTmp = false;
		if PetDialogFrame:IsShown() then
			PetDialogFrame:Hide();
		end
		if IsTrumpCamp() then
			XPFrame:Show();
		end
	end
	
	if arg1 == "GE_TMPITME_TMPMACHIN_NOSKILL" then
		local player = ActorMgr:getMainPlayer();
		if player == nil then return end;
		if TeamManager:isInTeamFollow() then
			TeamManager:Team_OP_CancelTeamFollow();
		end
	end

	--获得临时道具的消息
	if ( arg1 == "GE_TMPITME_GETTMPITEM" ) or ( arg1 == "GE_TMPITME_TMPMACHIN" ) then
		local player	= ActorMgr:getMainPlayer();
		local eqiup		= player:getEquip();
		if ( arg1 == "GE_TMPITME_GETTMPITEM" ) then
			FirstPickTmpItemByDefID( eqiup:getTmpArm() );
		else
			FirstPickTmpItemByDefID( eqiup:getTmpMachin() );
		end
		
		if player == nil then return end
		HideTempSkillBtn();
		
		--TempMagicShortcut1:SetPoint( "bottomleft", "TempShortCutFrame", "bottomleft", 260, -10 );
		--TempMagicShortcut2:SetPoint( "bottomleft", "TempShortCutFrame", "bottomleft", 320, -10 );
		TempShortCutFrame_IconTex:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );	 
		TempShortCutFrame_IconTex:Hide();
		if eqiup:getTmpArm() ~= 0 then
			local itemdef = getItemDef( eqiup:getTmpArm() );
			local IconPath = string.format("uires\\TuPianLei\\LinShiWuPin\\%d.tga", itemdef.IconID);
			if IsInExistence(IconPath) then
				TempShortCutFrame_IconTex:SetTexture(IconPath);
				TempShortCutFrame_IconTex:Show();
			end
		end
		
		for i = 1, MAX_TEMP_SKILL do
			local nSkillId =SkillMgr:getTmpSkillId(i-1); 
			local Tempbtn = getglobal("TempMagicShortcut"..i);
			if nSkillId ~= 0 then
				local tagSkillInfo		= SkillMgr:getSpellStaticAtt( nSkillId, SkillMgr:getSpellLv(nSkillId) );
				if tagSkillInfo ~= nil  then
					local Temptext= getglobal(Tempbtn:GetName().."Icon");	  
					Tempbtn:SetClientUserData( 0, nSkillId );	
					
					local IconPath = GetSkillIconPath()..tagSkillInfo.IconID1..".tga";
					-- 如果没找到物品的图标则使用默认图标显示
					if not IsInExistence( IconPath ) then
						IconPath = DEFAULT_ITEM_PATH;
					end
					Temptext:SetTexture( IconPath );

					Tempbtn:Show();
					Temptext:SetBlendAlpha( 1.0 );
					GameShortCut:setTmpShortCut( nSkillId, 1, i-1, 0,1 );
				end
			end
		end

		bHideTmpFrame = false; 
		if not bInTmp then
			PhotoFrame:SetPoint( "topleft", "$parent", "topleft", 0, 0 );
			ExtendSkillRightFrame:SetPoint("right","$parent","right",-1,60); 
			
			if not ShortCutFrame:IsShown() then
				ShortCutFrame:Show(); 
				MoveShortcutFrameShow();
			end

			ExtendSkillFrame:MoveFrame("bottom", MOVE_DURATION, ExtendSkillFrame:GetWidth(), 144 );	  
			ShortCutFrame:MoveFrame("bottom", MOVE_DURATION, ShortCutFrame:GetWidth(), ShortCutFrame:GetHeight() );	  
			PhotoFrame:MoveFrame("left", MOVE_DURATION, PhotoFrame:GetWidth(), PhotoFrame:GetHeight());
			ExtendSkillRightFrame:MoveFrame("right",MOVE_DURATION, 100, ExtendSkillRightFrame:GetHeight());
			if ExtendSkillLeftFrame:IsShown() then
				ExtendSkillLeftFrame:SetPoint("right","$parent","right",-47,60); 
				ExtendSkillLeftFrame:MoveFrame("right",MOVE_DURATION, 100, ExtendSkillLeftFrame:GetHeight());
			end			
			ShortCutButtonsFrame:MoveFrame("bottom", MOVE_DURATION, ShortCutButtonsFrame:GetWidth(), ShortCutButtonsFrame:GetHeight() );

			szNeedChangeFrame	= "TempShortCutFrame";
			nBeginMoveTime		= os.clock();
			bShowTmpFrame		= true;
		end

		if XPFrame:IsShown() then
			XPFrame:Hide();
		end
		bInTmp = true;
		
		local tempXinShouBtn = getglobal( "TempShortCutFrameAttackXinShouBtn" );
		tempXinShouBtn:Hide();
		UseTmpItemGuide();
	end

	-- 收到 角色属性变化消息时 判断是否闪屏
	if ( arg1 == "UI_ACTOR_ATTRIBUTE_CHG" ) or ( arg1 == "GE_TMP_MAX_ENERGY_CHG" ) then
		local MainPlayer = ActorMgr:getMainPlayer();
		local nEnerge	 = MainPlayer:getEnerge();
		-- TODO:
		local fScale = 1 - ( nEnerge / MainPlayer:getMaxEnerge() );
		-- TODO:	若为1则是空，若为0则满
		fScale = math.max( fScale, 0 );
		TmpEnerge_Texture:setTextOffsetY( fScale );
		-- 能量为零 提示
		local eqiup	= MainPlayer:getEquip();
		if  eqiup:getTmpMachin() ~= 0 and nEnerge == 0 then
			local UVATexture = getglobal( "TmpDropButtonUVAnimationTex" )
			if not UVATexture:IsShown() then
				UVATexture:SetUVAnimation(50,true);
				UVATexture:Show();
			end
		end
	end
	
	if arg1 == "GE_CONTAINER_UPDATE" then
		local player = ActorMgr:getMainPlayer()
		if player and not player:isInFly() then
			local eqiup = player:getEquip()
			if eqiup:isUsedTmp() then
				local exhaust = true
				local skillNum = 0
				local container = player:getContainer()
				for i = 1, MAX_TEMP_SKILL do
					local SpellId		= SkillMgr:getTmpSkillId(i - 1)
					if SpellId ~= 0 then
						skillNum = skillNum + 1
						local SpellLv		= SkillMgr:getSpellLv(SpellId)
						local SkillDef		= SkillMgr:getSpellStaticAtt(SpellId, SpellLv)
						if SkillDef.AmmID == 0 or SkillDef.AmmUse == 0 or math.floor(container:getItemCount(SkillDef.AmmID) / SkillDef.AmmUse) > 0 then
							exhaust = false
							break
						end
					end
				end
				if skillNum > 0 and exhaust then
					eqiup:looseTmpArm()
				end
			end
		end
	end
end

function UseTmpItemGuide()
	local mainplayer	= ActorMgr:getMainPlayer();
	local eqiup			= mainplayer:getEquip();
	local nTmpItemId	= 0;
	if eqiup:getTmpArm() ~= 0 then
		nTmpItemId = eqiup:getTmpArm();
	else
		nTmpItemId = eqiup:getTmpMachin();
	end

	if nTmpItemId == 1420000 then	-- 火焰刀
		 FirstUseSword( );
	elseif nTmpItemId == 1460001 then	-- 机甲兽
		 FirstUseJiJia( );
	elseif nTmpItemId == 1460002 then	-- 炮台
		 FirstUsePaoTai( );
	elseif nTmpItemId == 1400002 then	-- 手雷
		 FirstUseShouLei( );
	end
end

function TempShortCutFrame_OnUpdate()
	if bHideTmpFrame and ( os.clock() - nTempBeginMoveTime ) >= MOVE_DURATION then
		for i = 1, MAX_TMP_SKILL_BTN do
			local btn = getglobal("TempMagicShortcut"..i);
			btn:SetCooldownTimer(0, 0, true);
		end
		TempShortCutFrame:Hide();
		ShortCutFrame:Show(); 
		MoveShortcutFrameShow();
		ExtendSkillFrame:Show();
		bHideTmpFrame = false; 
		nTempBeginMoveTime = os.clock();
		HideTempShortCutFrameGuideEffectTip();
		CheckShortcutEffect( "TempShortCutFrame_OnUpdate" );
	end
end

function ShortCutFrame_OnUpdate()
	-- 每一帧 判断是否需要闪屏警告
	SetLowHPTexture( 0.3, 5 );
	-- 设置XP水晶球贴图
	if bShowTmpFrame then
		if szNeedChangeFrame ~= ""  and (os.clock() - nBeginMoveTime) >= MOVE_DURATION then
			local changeFrame = getglobal( szNeedChangeFrame );
			changeFrame:Show();
			ShortCutFrame:Hide();
			ExtendSkillFrame:Hide();
			bShowTmpFrame = false;
			HideShutcutFrameGuideEffectTip();
		end
	end
end

function ShortCutFrame_OnEndMove()
	CheckShortcutEffect( "ShortCutFrame_OnEndMove" );
end

function FairyShortCutFrame_OnUpdate()
	-- 开始倒计XP槽
	if GetStartXpUse() then
		local scale = ( os.clock() - GetStartXpTime() )/GetXPDuration();
		if scale < 0 then
			scale = 0;
		end
		if scale > 1 then
			scale = 1;
		end
		if ( os.clock() -  GetStartXpTime() ) < GetXPDuration() then
			TempFairyEnerge_Texture:Show();
			TempFairyEnerge_Texture:SetTexUV( 104, 813,(1-scale)*327, 69 );
			TempFairyEnerge_Texture:SetSize( (1-scale)*327,69 );
		else
			SetStartXpUse( false, 0, 0 );
		end
	end

	if bHideTmpFairyFrame and ( os.clock() - nTmpFairyBeginMoveTime ) >= MOVE_DURATION then	
	    TempFairyShortCutFrame:Hide();
		ShortCutFrame:Show();
		MoveShortcutFrameShow();
		ExtendSkillFrame:Show();
		bHideTmpFairyFrame = false; 
		nTmpFairyBeginMoveTime = os.clock();
		HideFairyShortCutFrameGuideEffectTip(); 
		CheckShortcutEffect( "FairyShortCutFrame_OnUpdate" );
	end
end

function TempFairyShortCutFrame_OnEndMove()
	CheckShortcutEffect( "TempFairyShortCutFrame_OnEndMove" );
end

function TmpShortCutUpdateFrame_OnUpdate()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil or getGameState() ~= STATE_INWORLD then
		return;
	end
	 
	if TempShortCutFrame:IsShown() then
		local fScale = 1-(MainPlayer:getEnerge()/MainPlayer:getMaxEnerge());
		-- TODO:	若为1则是空，若为0则满
		fScale = math.max( fScale, 0 );
		TmpEnerge_Texture:setTextOffsetY( fScale );
		local eqiup			= MainPlayer:getEquip();
		local nTmpItemId	= 0;
		if eqiup:getTmpArm() ~= 0 then
			nTmpItemId = eqiup:getTmpArm();
		else
			nTmpItemId = eqiup:getTmpMachin();
		end
		if nTmpItemId == 1460003 and fScale == 1 then
			--显示下车提示
			TempShortCutFrameGuide:Show()
			TempShortCutFrameGuideRich:SetText( "能量用光了，\n请点击“取消使用”\n丢弃此机械",20,30,120 )
		else
			TempShortCutFrameGuide:Hide()
			TempShortCutFrameGuideRich:SetText( "",255,255,190 )
		end
	end

	-- 设置特效显示XP技能
	if bShowXPSkill then
		if os.clock() - nBeginShowXPSkill < 0.5 then
			XPFrameYiGeUVAnimationTex:Show();
		else
			XPFrameYiGeUVAnimationTex:Hide();
			bShowXPSkill = false;
			nBeginShowXPSkill = 0;
		end
	end
end

function SetHideXPTex( bShow, nBeginTime, nCurScale, nWidth )
	bHideXPTex		= bShow;
	nBeginHideXP	= nBeginTime;
	nBeginScale		= nCurScale;
	nNextXPWidth	= nWidth;
end

function SetTempFairyFrameShow()
	bHideMainFrameByTempFairy		= true;
	bHideTempFairFrame	= false;
	nBeginHide = os.clock();
end

function SetTempFairyFrameHide()
	bHideMainFrameByTempFairy		= false;
	bHideTempFairFrame	= true;
	nBeginHide = os.clock();
end

function TempShortcutOnUpdate()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end

	local FontText = getglobal(this:GetName().."CollectText");
	local IconTex = getglobal(this:GetName().."Icon");
	local SpellId		= this:GetClientUserData(0);
	if SpellId ~= 0 then
		local SkillAttr		= SkillMgr:getSkillDyAttrib( SpellId );
		if SkillAttr == nil then
			return;
		end
		local SpellLv		= SkillMgr:getSpellLv( SpellId );
		local CollectUse	= SkillMgr:getSpellCollectNumUse( SpellId, SpellLv );
		local SkillDef = SkillMgr:getSpellStaticAtt(SpellId, SpellLv)
		if CollectUse > 0 then
			local CollectNum   = SkillMgr:getSpellCollectNumCur( SpellId );
			FontText:SetText( CollectNum );
		else
			if SkillDef.AmmID ~= 0 and SkillDef.AmmUse ~= 0 then
				local container = MainPlayer:getContainer();
				FontText:SetText(math.floor(container:getItemCount(SkillDef.AmmID) / SkillDef.AmmUse));
			else
				FontText:SetText( "" );
			end
		end
		if SkillMgr:isTmpSkill(SpellId) and SkillDef.Learn.LevelMin > MainPlayer:getLv() then
			IconTex:SetGray(true);
		else
			IconTex:SetGray(false);
		end
	elseif this:GetClientUserData(1) ~= 0 then
		local Container = MainPlayer:getContainer();
		local nCount = Container:getItemCount(this:GetClientUserData(1));
		IconTex:SetGray(false);
		FontText:SetText( "" );
		if IsItemLock( 0, 0, this:GetClientUserData(1) ) or nCount == 0 then
			IconTex:SetGray(true);
		end

		if nCount > 1 then
			FontText:SetText( nCount );
		end
	elseif this:GetClientUserData(1) == 0 then
		FontText:SetText( "" );
	end
end

function TempSkill_OnUpdate()
	
end

function CheckTempShortCutFrameXinShou()
	local mainplayer = ActorMgr:getMainPlayer();
	local eqiup = mainplayer:getEquip();
	if not GameWizard:isDo( GAMEWIZARD_FIRST_USE_SWORD ) and Quest:isQuestRun( 1054 ) and eqiup:getTmpArm() == 1420000 then
		GameWizard:doStep( GAMEWIZARD_FIRST_USE_SWORD );
	end

	if not GameWizard:isDo( GAMEWIZARD_SECOND_USE_SWORD ) and Quest:isQuestRun( 1062 ) and eqiup:getTmpArm() == 1420000 then
		GameWizard:doStep( GAMEWIZARD_SECOND_USE_SWORD );
	end

	if not GameWizard:isDo( GAMEWIZARD_FIRST_USE_TMPMACHINE ) and eqiup:getTmpArm() == 1460001 then
		GameWizard:doStep( GAMEWIZARD_FIRST_USE_TMPMACHINE );
	end

	if not GameWizard:isDo( GAMEWIZARD_FIRST_USE_TPAOTAI ) and eqiup:getTmpArm() == 1460002 then
		GameWizard:doStep( GAMEWIZARD_FIRST_USE_TPAOTAI );
	end

	if not GameWizard:isDo( GAMEWIZARD_FIRST_USE_TSHOULEI ) and eqiup:getTmpArm() == 1400002 then
		GameWizard:doStep( GAMEWIZARD_FIRST_USE_TSHOULEI );
	end
end

function TempShortCutFrameXinShouBtn_CloseBtn_OnClick()
	local xinShouBtn = getglobal( this:GetParent() );
	xinShouBtn:Hide();
end

function ShowTempShorCutFrameXinShouBtn( data )
	local tempXinShouBtn = getglobal( "TempShortCutFrameAttackXinShouBtn" );
	local rich = getglobal( tempXinShouBtn:GetName().."Rich" );
	rich:SetClientString( data["text"] );

	local nSelfDefineExtendWidth = 14;
	ReSizeTooltip( rich:GetName(), tempXinShouBtn:GetName(), nSelfDefineExtendWidth );
	tempXinShouBtn:Show();
end

function TempShortcutOnClick()
	local tempXinShouBtn = getglobal( "TempShortCutFrameAttackXinShouBtn" );
	if this:GetClientID() == 1 and tempXinShouBtn:IsShown() then
		CheckTempShortCutFrameXinShou();
		tempXinShouBtn:Hide();
	end
	
	GameShortCut:usedShortCut( this:GetClientID() - 1 );
end

function TempFairyShortcutClick()
	GameShortCut:usedShortCut( this:GetClientID() - 1 );
	if this:GetName() == "TempFairyMagicShortcut1" and TempFairyShortCutFrameEffectFrame:IsShown() then
		TempFairyShortCutFrameEffectFrame:Hide();
	end
end

function TmpHPShortCut_OnShow()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	for i=5,8 do
		local btn = getglobal( "TempMagicShortcut" .. i );
		if btn:GetClientUserData( 1 ) > 0 then
			local cd = container:getItemCD( btn:GetClientUserData( 1 ) );
			local starttime = container:getItemCDPassTime( btn:GetClientUserData( 1 ) );
			btn:SetCooldownTimer( starttime, starttime+cd, true );
			btn:SetCooldownTextureRect( 3, 5, 38, 38 );
		end
	end
end

function TmpMPShortCut_OnShow()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	for i=11,14 do
		local btn = getglobal( "TempMagicShortcut" .. i );
		if btn:GetClientUserData( 1 ) > 0 then
			local cd = container:getItemCD( btn:GetClientUserData( 1 ) );
			local starttime = container:getItemCDPassTime( btn:GetClientUserData( 1 ) );
			btn:SetCooldownTimer( starttime, starttime+cd, true );
			btn:SetCooldownTextureRect( 3, 5, 38, 38 );
		end
	end
end

function TempShortcutOnLoad()
	local t_TmpShortCut = 
			{
				[1]		= { backtex="uires\\ZhuJieMian\\1.dds", u = 749, v = 472, w = 44, h = 44, x = 0, y = 0, size = 42 };
				[2]		= { backtex="uires\\ZhuJieMian\\1.dds", u = 749, v = 472, w = 44, h = 44, x = 0, y = 0, size = 42 };
				[3]		= { backtex="uires\\ZhuJieMian\\1.dds", u = 749, v = 472, w = 44, h = 44, x = 0, y = 0, size = 42 };
				[4]		= { backtex="uires\\ZhuJieMian\\1.dds", u = 749, v = 472, w = 44, h = 44, x = 0, y = 0, size = 42 };
				[5]		= { backtex="uires\\ui\\Za\\XuePingDiBan.tga", u = 0, v = 0, w = 35, h = 35, x = 4, y = 4, size = 35 };
				[6]		= { backtex="uires\\ui\\Za\\XuePingDiBan.tga", u = 0, v = 0, w = 35, h = 35, x = 4, y = 4, size = 35 };
				[7]		= { backtex="uires\\ui\\Za\\XuePingDiBan.tga", u = 0, v = 0, w = 35, h = 35, x = 4, y = 4, size = 35 };
				[8]		= { backtex="uires\\ui\\Za\\XuePingDiBan.tga", u = 0, v = 0, w = 35, h = 35, x = 4, y = 4, size = 35 };
				[9]		= { backtex="uires\\ui\\Za\\XuePingDiBan.tga", u = 0, v = 0, w = 35, h = 35, x = 4, y = 4, size = 35 };
				[10]	= { backtex="uires\\ui\\Za\\XuePingDiBan.tga", u = 0, v = 0, w = 35, h = 35, x = 4, y = 4, size = 35 };
				[11]	= { backtex="uires\\ui\\Za\\XuePingDiBan.tga", u = 0, v = 0, w = 35, h = 35, x = 4, y = 4, size = 35 };
				[12]	= { backtex="uires\\ui\\Za\\XuePingDiBan.tga", u = 0, v = 0, w = 35, h = 35, x = 4, y = 4, size = 35  };
				[13]	= { backtex="uires\\ui\\Za\\XuePingDiBan.tga", u = 0, v = 0, w = 35, h = 35, x = 4, y = 4, size = 35  };
				[14]	= { backtex="uires\\ui\\Za\\XuePingDiBan.tga", u = 0, v = 0, w = 35, h = 35, x = 4, y = 4, size = 35  };
			};

	local Btn			= getglobal(this:GetName());
	local BackTex		= getglobal(this:GetName().."Back");
	local CollectText	= getglobal(this:GetName().."CollectText");
	if t_TmpShortCut[this:GetClientID()] ~= nil then
		BackTex:SetTexture( t_TmpShortCut[this:GetClientID()].backtex );
		BackTex:SetTexUV( t_TmpShortCut[this:GetClientID()].u,t_TmpShortCut[this:GetClientID()].v,t_TmpShortCut[this:GetClientID()].w,t_TmpShortCut[this:GetClientID()].h );
		BackTex:SetSize(t_TmpShortCut[this:GetClientID()].size,t_TmpShortCut[this:GetClientID()].size);
		BackTex:SetPoint( "TopLeft", this:GetName(), "TopLeft", t_TmpShortCut[this:GetClientID()].x, t_TmpShortCut[this:GetClientID()].y );
		CollectText:SetText("");
		Btn:SetClientUserData(0,0);
		Btn:SetClientUserData(1,0);
	end
	
	this:RegisterEvent("GE_SKILL_COOLD");
	this:RegisterEvent("GE_SHORTLIST_CHANGE");
	this:RegisterEvent("GE_ITEMUSED_PASSCD");
end

function TempShortcutOnEvent()
	if arg1 == "GE_SKILL_COOLD" then
		local Msg = UIMSG:getUIMsg();
		if Msg.CoolTime.num == this:GetClientUserData(0) then
			this:SetCooldownTimer( Msg.CoolTime.starttime, Msg.CoolTime.endtime, true );
			this:SetCooldownTextureRect( 3, 3, 34, 34 );
		end
	end

	if( arg1 == "GE_ITEMUSED_PASSCD" ) then
		local MainPlayer = ActorMgr:getMainPlayer();
		if MainPlayer == nil then return end
		local container = MainPlayer:getContainer();
		if container == nil then return end
		if this:GetClientID() > MAX_TMP_SKILL_BTN and this:GetClientUserData(1) > 0 then
			this:SetCooldownTimer( 0, 0, true );
			local cd		= container:getItemCD( this:GetClientUserData(1) );
			local starttime = container:getItemCDPassTime( this:GetClientUserData(1) );
			this:SetCooldownTimer( starttime, starttime+cd, true );
			this:SetCooldownTextureRect( 3, 3, 34, 34 );
		end
	end

	if arg1 == "GE_SHORTLIST_CHANGE" then
		local Msg = UIMSG:getUIMsg();
		if Msg.ShortCut.m_nNum <= MAX_TMP_SKILL_BTN or Msg.ShortCut.m_nNum > MAX_TMP_BTN or not TempShortCutFrame:IsShown() then
			return;
		end

		local TmpShortcutBtn		= getglobal("TempMagicShortcut"..Msg.ShortCut.m_nNum);
		local TmpShortcutIcon		= getglobal(TmpShortcutBtn:GetName().."Icon");
		local TmpShortcutBox		= getglobal(TmpShortcutBtn:GetName().."BoxTexture");
		local FontText				= getglobal(TmpShortcutBtn:GetName().."CollectText");
		if( Msg.ShortCut.m_nType  == SCT_ITEM ) then
			local mainplayer = ActorMgr:getMainPlayer();
			local container	 = mainplayer:getContainer();
			TmpShortcutBtn:SetClientUserData( 1, Msg.ShortCut.m_Id );
			local itemDef = getItemDef( Msg.ShortCut.m_Id );
			local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
			-- 如果没找到物品的图标则使用默认图标显示
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			TmpShortcutIcon:SetTexture( IconPath );
			if container:getItemCount( Msg.ShortCut.m_Id ) > 1 then
				FontText:SetText( container:getItemCount( Msg.ShortCut.m_Id ) );
			end
		else
			FontText:SetText( "" );
			TmpShortcutBtn:SetClientUserData( 1, 0 );
			TmpShortcutBtn:SetClientUserData( 0, 0 );
			TmpShortcutIcon:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
		end
	end
end

function TempFairyShortcutOnLoad()
	local FontText = getglobal(this:GetName().."Text");
	if( this:GetClientID() == 1 ) then
		FontText:SetText( "1" );
	elseif( this:GetClientID() == 2 ) then
		FontText:SetText( "2" );
	end

	this:RegisterEvent("GE_SKILL_COOLD");
end

function TempFairyShortcutOnEvent()
	if arg1 == "GE_SKILL_COOLD" then
		local Msg = UIMSG:getUIMsg();
		if Msg.CoolTime.num == this:GetClientUserData(0) then
			this:SetCooldownTimer( Msg.CoolTime.starttime, Msg.CoolTime.endtime, true );
			this:SetCooldownTextureRect( 3, 3, 38, 38 );
		end
	end
end

function TempShortcutOnEnter()
	if this:GetClientID() > MAX_TMP_SKILL_BTN and this:GetClientUserData(1) ~= 0 then
		local itemDef	= getItemDef( this:GetClientUserData(1) );
		if  itemDef == nil then
			return;
		end

		local MainPlayer = ActorMgr:getMainPlayer();
		if MainPlayer ~= nil then
			local container = MainPlayer:getContainer();
			local nMaxNum = container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES );
			for i = 1, nMaxNum do
				local ContainerItem = container:getItem( CONTAINER_TYPE_SUNDRIES, i-1 );
				if itemDef.ItemID == ContainerItem:getItemId() then
					ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, ContainerItem );
					return;
				end
			end
		end
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, itemDef );
	end

	local skillId   = this:GetClientUserData(0);
	if skillId ~= 0 then
		SpellTipsFrame:SetClientUserData( 0, SkillMgr:getRuneSkillId(skillId) );
		SpellTipsFrame:SetClientString(this:GetName());
		if string.find( this:GetName(),"TmpFrame",1,true ) then
			local skilllv	= this:GetClientUserData(1);
			SpellTipsFrame:SetClientUserData( 1, skilllv );
			SpellTipsFrame:SetClientUserData( 2, SKILL_TYPE );
			SpellTipsFrame:SetClientUserData( 3, TMP_TANK_SKILL_TYPE );
		end
		SpellTipsFrame:Show();
	end
end

function TempShortcutOnLeave()
	SpellTipsFrame:Hide();
	NormalItemTipsFrame:Hide();
	ArmItemTipsFrame:Hide();
	SecondArmItemTipsFrame:Hide();
end

function CanDragInTempShortcut( item )
	if item ~= nil then	
		local itemDef = item:getItemDef();
		for i = 1,MAX_ITEM_RESULT do
			if itemDef.Result[i-1].ResultID == RESULT_HP_ADD 
			or itemDef.Result[i-1].ResultID == RESULT_MP_ADD then
				return true;
			end
		end
	end
	return false;
end

MONSTER_RIDE = 4;
function DropTempPropButton_OnClick()
	local player = ActorMgr:getMainPlayer();
	if player == nil then return end
	if player:isInFly() then
		ShowMidTips(" 当前状态不能丢弃");
		return;
	end
	
	local eqiup = player:getEquip();

	--非副本地图,机械时，需要2次确认
	if eqiup:getTmpMachin() > 0 then
		local nCurMapID = MapShower:getRealMapId();
		if nCurMapID < MIN_COPY_ID then
			local MaxMapControl	= MapShower:GetMaxMapContal();
			local nRace = MaxMapControl:getNpcRace( eqiup:getTmpMachin() );
			if nRace ~= MONSTER_RIDE then
				local szName = GetNpcName( eqiup:getTmpMachin() );
				if szName ~= "" then
					MessageBox("","点击“确定”按钮将取消使用【" .. szName .. "】，您的技能栏也会恢复为自己的职业技能。")
				else
					MessageBox("","点击“确定”按钮将取消使用此机械，您的技能栏也会恢复为自己的职业技能。")
				end
				MessageBoxFrame:SetClientString( "丢弃机械" );
				return;
			end
		end
	end
	eqiup:looseTmpArm();
	local btn1 = getglobal("TempMagicShortcut1");
	local btn2 = getglobal("TempMagicShortcut2");
	btn1:SetClientUserData( 0, 0 );
	btn2:SetClientUserData( 0, 0 );
end

function DropTempPropButton_OnEnter()
	local mainplayer = ActorMgr:getMainPlayer();
	local eqiup = mainplayer:getEquip();
	if not eqiup:isUsedTmp() then
		return;
	end
	
	local nItemId = ( eqiup:getTmpArm() ~= 0 ) and ( eqiup:getTmpArm() ) or ( eqiup:getTmpMachin() );
	local code = GetKeyMap("取消临时道具");
	code = code and KeyToShort(code) or ""
	local szText;
	if code ~= "" then
		szText = string.format("取消使用%s(%s)", getItemDef( nItemId ).Name, code);
	else
		szText = string.format("取消使用%s", getItemDef( nItemId ).Name);
	end
	-- "cursor"
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text =  szText, frame = "TempShortCutFrame",
						button = this:GetName(), horizonal = "right" } );
end

function TmpShowHpShortCutBtn_OnClick()
	if arg1 == "LeftButton" then
		if not TmpHPShortCut:IsShown() then
			TmpHPShortCut:Show();
		else
			TmpHPShortCut:Hide();
		end
	end
end

function TmpShowMpShortCutBtn_OnClick()
	if arg1 == "LeftButton" then
		if not TmpMPShortCut:IsShown() then
			TmpMPShortCut:Show();
		else
			TmpMPShortCut:Hide();
		end
	end
end
local MAX_PET_SKILL	 = 1;
--[parm] 秒数

local t_ModelIconId = { [292000] = 5620004,	-- 风神
						[292002] = 5620001,	-- 山神
						[231003] = 5620000, -- 雷神
						[292003] = 5620006, -- 天使
						[292004] = 5620003,	-- 瘟疫
						[292005] = 5620005, -- 影子
						[292001] = 292001, -- 树妖
					  }
function BeginXP(second,iconid)
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end
	
	if XPFrame:IsShown() then
		XPFrame:Hide();
	end
	if SpellTipsFrame:IsShown() then
		if IsInFrameAbs( "SpellTipsFrame", "XPFrame" ) then
			SpellTipsFrame:Hide();
		end
	end

	SetStartXpUse( true, second, os.clock() );
	if PetDialogFrame:IsShown() then
		PetDialogFrame:Hide();
	end

	local nCount = 1;
	for i = 1, MAX_PET_SKILL do
		local nSkillId =GameShortCut:getTmpShortCutId(i-1);
		local nSkillLv =GameShortCut:getTmpShortCutLv(i-1);
		if nSkillId > 0 then
			local skill_attr	= SkillMgr:getSkillDyAttrib( nSkillId );
			if skill_attr ~= nil then
				local btn = getglobal("TempFairyMagicShortcut"..nCount);
				local text= getglobal("TempFairyMagicShortcut"..nCount.."Icon");
				local tagSkillInfo		= SkillMgr:getSpellStaticAtt( nSkillId, nSkillLv );
				if tagSkillInfo == nil then
					return;
				end

				btn:SetClientUserData( 0, nSkillId );

				local IconPath = GetSkillIconPath()..tagSkillInfo.IconID1..".tga";
				-- 如果没找到物品的图标则使用默认图标显示
				if not IsInExistence( IconPath ) then
					IconPath = DEFAULT_ITEM_PATH;
				end
				text:SetTexture( IconPath );
				btn:Show();
				nCount = nCount + 1;
			end
		end
	end

	for i = nCount, 2 do
		local btn = getglobal("TempFairyMagicShortcut"..i);
		btn:Hide();
	end

	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end

	local GameTrump = MainPlayer:getTrump();
	local nTrumpNum	= GameTrump:getTrumpMaxNum();
	local nCount = 1;
	
	if not ShortCutFrame:IsShown() and not LoadingFrame:IsShown() then
		ShortCutFrame:Show(); 
		MoveShortcutFrameShow();
	end

	PhotoFrame:SetPoint( "topleft", "$parent", "topleft", 0, 0 );
	ExtendSkillRightFrame:SetPoint("right","$parent","right",-1,60); 

	ExtendSkillFrame:MoveFrame("bottom", MOVE_DURATION, ExtendSkillFrame:GetWidth(), 144 );	  
	ShortCutFrame:MoveFrame("bottom", MOVE_DURATION, ShortCutFrame:GetWidth(), ShortCutFrame:GetHeight() );	  
	PhotoFrame:MoveFrame("left", MOVE_DURATION, PhotoFrame:GetWidth(), PhotoFrame:GetHeight());
	ExtendSkillRightFrame:MoveFrame("right",MOVE_DURATION, 100, ExtendSkillRightFrame:GetHeight());
	if ExtendSkillLeftFrame:IsShown() then
		ExtendSkillLeftFrame:SetPoint("right","$parent","right",-47,60); 
		ExtendSkillLeftFrame:MoveFrame("right",MOVE_DURATION, 100, ExtendSkillLeftFrame:GetHeight());
	end
	ShortCutButtonsFrame:MoveFrame("bottom", MOVE_DURATION, ShortCutButtonsFrame:GetWidth(), ShortCutButtonsFrame:GetHeight());

	szNeedChangeFrame	= "TempFairyShortCutFrame";
	nBeginMoveTime		= os.clock();
	bShowTmpFrame		= true;
	bHideTmpFrame		= false;
	bHideTmpFairyFrame	= false;
	TempShortCutFrame:Hide();	
	
	local RoleTrump = GameTrump:getCampRoleFairy();
	if RoleTrump ~= nil and RoleTrump.FairyID > 0 then
		local SkillDef = nil;
		local TrumpDef	= GameTrump:getFairyDef( RoleTrump.FairyID, RoleTrump.FairyLevel );
		for j=1, FAIRY_MAX_SKILL do
			if TrumpDef.FairySkillList[j-1].SkillID ~= 0 then
				if TrumpDef.FairySkillList[j-1].SkillType == SKILL_TYPE then
					SkillDef = SkillMgr:getSpellStaticAtt( TrumpDef.FairySkillList[j-1].SkillID, TrumpDef.FairySkillList[j-1].SkillLevel );			
				end
				if SkillDef ~= nil and SkillDef.WushuangFlag > 0 and SkillDef.SkillResult[0].SkillResultID == RESULT_SELF_STATUS then
					break;
				end
			end
		end
		if SkillDef ~= nil then
			local statusDef = getStatusDef( SkillDef.SkillResult[0].SkillResultVal2,SkillDef.SkillResult[0].SkillResultVal3 );
			if statusDef ~= nil and statusDef.ModeID == iconid then
				local IconPath = "uires\\TuPianLei\\TouXiang\\JingLingTouXiang\\"..TrumpDef.ModeID..".tga";
				-- 如果没找到物品的图标则使用默认图标显示
				if not IsInExistence( IconPath ) then
					IconPath = GetTrumpIconPath().."000000.tga";
				end
				TempFairyPetTex_Texture:SetTexture( IconPath );
				return;
			end
		end
	end
	
	local IconPath = GetMonsterIconPath( GameBCT:getMonsterIcon( iconid ) )
	if not IsInExistence( IconPath ) then
		IconPath = "uires\\TuPianLei\\TouXiang\\JingLingTouXiang\\"..t_ModelIconId[iconid]..".tga";
	end
	-- 如果没找到物品的图标则使用默认图标显示
	if not IsInExistence( IconPath ) then
		IconPath = GetTrumpIconPath().."000000.tga";
	end
	TempFairyPetTex_Texture:SetTexture( IconPath );	
end

function EndXP()
	SkillMgr:clearIntentSpell();
	PhotoFrame:SetPoint( "topleft", "$parent", "topleft", 0, 0 );
	PhotoFrame:MoveFrame("left", MOVE_DURATION, PhotoFrame:GetWidth(), PhotoFrame:GetHeight());
	TempFairyShortCutFrame:MoveFrame("bottom", MOVE_DURATION, TempFairyShortCutFrame:GetWidth(), TempFairyShortCutFrame:GetHeight() );
	ShortCutButtonsFrame:MoveFrame("bottom", MOVE_DURATION, ShortCutButtonsFrame:GetWidth(), ShortCutButtonsFrame:GetHeight());

	bHideTmpFairyFrame		= true;
	nTmpFairyBeginMoveTime	= os.clock();

	SetXPSkillShow();
	if IsTrumpCamp() then
		XPFrame:Show();
	end
end

function SetXPSkillEffective( bShow, nBeginTime)
	XPFrameYiGeUVAnimationTex:SetUVAnimation(20, false);
	bShowXPSkill		= bShow;
	nBeginShowXPSkill	= nBeginTime;
end


function ClanCollectTmpFrame_OnShow()
	SwitchDropMonsterMachineBtn(ClanCollectTmpFrameDownBtn, true)
	ClanCollectMachineResourceFrame:Show()
	local MainPlayer = ActorMgr:getMainPlayer();

	ExtendSkillRightFrame:SetPoint("right","$parent","right",-1+100,60);   
	if ExtendSkillLeftFrame:IsShown() then	
		ExtendSkillLeftFrame:SetPoint("right","$parent","right",-47+100,60); 
	end

	ClanCollectTmpFrame:SetPoint("bottom","$parent","bottom",0,-1+ClanCollectTmpFrame:GetHeight()); 
	ClanCollectTmpFrame:MoveFrame("top", MOVE_DURATION, ClanCollectTmpFrame:GetWidth(), ClanCollectTmpFrame:GetHeight());

	PhotoFrame:SetPoint( "topleft", "$parent", "topleft", -PhotoFrame:GetWidth(), 0 );
	PhotoFrame:MoveFrame("right", MOVE_DURATION, PhotoFrame:GetWidth(), PhotoFrame:GetHeight());

	ShortCutButtonsFrame:SetPoint("bottom","$parent","bottom",0,ShortCutButtonsFrame:GetHeight());
	ShortCutButtonsFrame:MoveFrame("top", MOVE_DURATION, ShortCutButtonsFrame:GetWidth()+3, ShortCutButtonsFrame:GetHeight());
	CheckShortcutEffect( "ClanCollectTmpFrame_OnShow" );
	TempShortCutFrame:Hide();
	ClanTankTmpFrame:Hide();
	TmpMachineBuffFrame:Show();
	NetWorkStatus:SetPoint( "topleft", "ShortCutButtonsFrame", "topleft",52,23);
	--elmer
	local UVATexture = getglobal( "ClanCollectTmpFrameDownBtnUVAnimationTex" )
	UVATexture:Hide();
end

-------------------------------------------------------------------------------------

function ClanCollectTmpFrameOnLoad()
	this:RegisterEvent( "GE_MONSTER_ATTR_CHANGE" );
	this:RegisterEvent( "GE_MONSTER_MACHINE_LEAVE_PREPARE" );
	for i=1,6 do
		local btn = getglobal( "ClanCollectTmpFrameCollectShortCut"..i );
		local FontText = getglobal("ClanCollectTmpFrameCollectShortCut"..i.."Text");
		FontText:SetText( btn:GetClientID() );
	end
	ClanCollectTmpFrameRepairExpand:SetClientUserData(0, 311)
	ClanCollectTmpFrameEnergyExpand:SetClientUserData(0, 283)
	ClanCollectTmpFrameFoodExpand:SetClientUserData(0, 48)
	ClanCollectTmpFrameFoodExpand:SetClientUserData(2, 1020)
	ClanCollectTmpFrameHPExpand:SetClientUserData(0, 46)
	ClanCollectTmpFrameRepairExpand:SetClientString("ClanCollectTmpFrameCollectShortCut5")
	ClanCollectTmpFrameEnergyExpand:SetClientString("ClanCollectTmpFrameCollectShortCut6")
	ClanCollectTmpFrameFoodExpand:SetClientString("ClanCollectTmpFrameCollectShortCut7")
	ClanCollectTmpFrameHPExpand:SetClientString("ClanCollectTmpFrameCollectShortCut8")
	ClanCollectTmpFrameRepairExpandBtn:SetClientString("ClanCollectTmpFrameRepairExpand")
	ClanCollectTmpFrameEnergyExpandBtn:SetClientString("ClanCollectTmpFrameEnergyExpand")
	ClanCollectTmpFrameFoodExpandBtn:SetClientString("ClanCollectTmpFrameFoodExpand")
	ClanCollectTmpFrameHPExpandBtn:SetClientString("ClanCollectTmpFrameHPExpand")
end

function ClanCollectTmpFrameOnEvent()
	--临时采集车
	if arg1 == "GE_MONSTER_ATTR_CHANGE" then
		if ClanCollectTmpFrame:IsShown() then
			RefreshAttriInfo();
		elseif ClanTankTmpFrame:IsShown() then
			RefreshTankAttriInfo();
		end
	elseif arg1 == "GE_MONSTER_MACHINE_LEAVE_PREPARE" then
		
	end
end

function RefreshAttriInfo()
	local player = ActorMgr:getMainPlayer();
	local monsterMachine = player:getMonsterMachine();
	local monster = ActorMgr:FindActor( monsterMachine.m_playerInfo.m_nMonsterMemID );
	if monster == nil then return end
	monsterMachine = monsterMachine:getPlayerMonsterMachine();
	if monsterMachine == nil then return end
	local nEnergy = monsterMachine:getEnergy();
	local nMaxEnergy = monsterMachine:getMaxEnergy();
	local energyScale = nEnergy/nMaxEnergy;
	local hp = monster:getHP();
	local maxHp = monster:getMaxHP();
	local hpScale = hp/maxHp;
	if energyScale > 1 then
		energyScale = 1;
	end
	if energyScale < 0 then
		energyScale = 0;
	end
	ClanCollectTmpFrameNPTex:SetTexUV( 745,692+(60-60*energyScale),16,60*energyScale );
	ClanCollectTmpFrameNPTex:SetSize( 16,60*energyScale );
	if hpScale > 1 then
		hpScale = 1;
	end
	if hpScale < 0 then
		hpScale = 0;
	end
	ClanCollectTmpFrameHPTex:SetTexUV( 728,692+( 60-60*hpScale ),16,60*hpScale );
	ClanCollectTmpFrameHPTex:SetSize( 16,60*hpScale );
end

function SwitchDropMonsterMachineBtn(button, reset)
	if reset then
		button:SetNormalTextureTemplate("GongYongAnNiuA1NormalTemplate")
		button:SetHighlightTextureTemplate("GongYongAnNiuA1HighTemplate")
		button:SetPushedTextureTemplate("GongYongAnNiuA1PushedTemplate")
		button:SetDisableTextureTemplate("GongYongAnNiuA1DisabledTemplate")
		button:SetText("下车")
	else
		button:SetNormalTextureTemplate("GongYongAnNiuA3NormalTemplate")
		button:SetHighlightTextureTemplate("GongYongAnNiuA3HighTemplate")
		button:SetPushedTextureTemplate("GongYongAnNiuA3PushedTemplate")
		button:SetDisableTextureTemplate("GongYongAnNiuA3DisabledTemplate")
		button:SetText("取消")
	end
end

function DropMonsterMachine()
	local this = this
	local player = ActorMgr:getMainPlayer()
	if not player then
		return
	end
	local monsterMachine = player:getMonsterMachine()
	local monsterMachineInfo = monsterMachine:getMonMachineDef()
	monsterMachine = monsterMachine:getPlayerMonsterMachine()
	if not monsterMachine then
		return
	end
	if monsterMachineInfo.LeaveLimit > 0 then
		ShowMidTips( "此活动不能下机械" )
		return
	end
	local id = monsterMachine.m_monsterInfo.m_nMonsterMemID
	local _, key = util.IsBlockPrepare()
	if key == "DropMonsterMachine" then
		util.BreakBlockPrepare("DropMonsterMachine")
		player:breakPrepareDownMonsterMachine(id)
	else
		if monsterMachineInfo and monsterMachineInfo.CollectType ~= 0 then
			local hasOre = 0
			for i = 1, 5 do
				local ore = monsterMachine:getOre(i)
				hasOre = hasOre + ore
			end
			if hasOre ~= 0 then
				if not MessageBoxBlock("下车确认", "#P下车将导致车上资源损耗，剩余资源也将掉落在地上，是否继续？") then
					return
				end
			end
		end
		local time = monsterMachine:getLeavePrepareTime()
		if time == 0 then
			player:requestDownMonsterMachine(id)
		else
			SwitchDropMonsterMachineBtn(this, false)
			player:prepareDownMonsterMachine(id)
			if util.WaitForBlockPrepare(time, "DropMonsterMachine", "准备下车") then
				player:requestDownMonsterMachine(id)
			end
			SwitchDropMonsterMachineBtn(this, true)
		end
	end
	local UVATexture1 = getglobal( "ClanCollectTmpFrameDownBtnUVAnimationTex" )
	if UVATexture1:IsShown() then 
		UVATexture:Hide();
	end
	local UVATexture = getglobal( "ClanTankTmpFrameDownBtnUVAnimationTex" )
	if UVATexture:IsShown() then
		UVATexture:Hide();
	end
end

function ClanCollectTmpFrameResourceBtnOnClick()
	local frame = ClanCollectMachineResourceFrame
	if frame:IsShown() then
		frame:Hide()
		ClanCollectTmpFrameResourceBtnTexture:Show();
	else
		frame:Show()
		ClanCollectTmpFrameResourceBtnTexture:Hide();
	end
end

function ClanCollectTmpFrameHP_OnEnter()
	local player = ActorMgr:getMainPlayer();
	local monsterMachine = player:getMonsterMachine();
	local monster = ActorMgr:FindActor( monsterMachine.m_playerInfo.m_nMonsterMemID );
	if monster == nil then return end
	local hp = monster:getHP();
	local maxHp = monster:getMaxHP();
	if hp > maxHp then hp = maxHp end
	local szText = "#G生命值："..hp.."/"..maxHp.."#n\n可前往时间城“#R格罗佛#n”\n处或#R公会城市#n内购买机械\n能量或机械扳手"
	if MapShower:getRealMapId() >= MIN_COPY_ID then
		local pworldDef = getPworldDef( MapShower:getRealMapId()/100 )
		if pworldDef.PolyType == PWORLD_POLY_TANK_TOLUA then
			szText = "#G生命值："..hp.."/"..maxHp.."#n\n可前往硝烟战地“#R瓦力#n”\n处内购买机械能量或机械扳手"
		end
	end
	SetGameTooltips( this:GetName(),szText );
end

function ClanCollectTmpFrameNP_OnEnter()
	local player = ActorMgr:getMainPlayer();
	local monsterMachine = player:getMonsterMachine();
	monsterMachine = monsterMachine:getPlayerMonsterMachine();
	if monsterMachine == nil then return end
	local np = monsterMachine:getEnergy();
	local maxNp = monsterMachine:getMaxEnergy();
	local szText = "#G能量："..np.."/"..maxNp.."#n\n可前往时间城“#R格罗佛#n”\n处或#R公会城市#n内购买机械\n能量或机械扳手"
	if MapShower:getRealMapId() >= MIN_COPY_ID then
		local pworldDef = getPworldDef( MapShower:getRealMapId()/100 )
		if pworldDef.PolyType == PWORLD_POLY_TANK_TOLUA then
			szText = "#G能量："..np.."/"..maxNp.."#n\n可前往硝烟战地“#R瓦力#n”\n处内购买机械能量或机械扳手"
		end
	end
	SetGameTooltips( this:GetName(),szText );
end

function ClanCollectTmpFrame_OnUpdate()
	if bHideTmpFrame and ( os.clock() - nTempBeginMoveTime ) >= MOVE_DURATION then	
		ClanCollectTmpFrame:Hide();
		ShortCutFrame:Show(); 
		MoveShortcutFrameShow();
		ExtendSkillFrame:Show();
		bHideTmpFrame = false; 
		nTempBeginMoveTime = os.clock();
		HideTempShortCutFrameGuideEffectTip();
		CheckShortcutEffect( "ClanCollectTmpFrame_OnUpdate" );
	end
	local player = ActorMgr:getMainPlayer();
	local monsterMachine = player:getMonsterMachine();
	monsterMachine = monsterMachine:getPlayerMonsterMachine();
	if monsterMachine == nil then return end
	local np = monsterMachine:getEnergy();
	if np == 0 then
		local UVATexture = getglobal( "ClanCollectTmpFrameDownBtnUVAnimationTex" )
		if not UVATexture:IsShown() then
			UVATexture:SetUVAnimation(50,true);
			UVATexture:Show();
		end
	end
end

function OnBindCollectMachine()
	local player	= ActorMgr:getMainPlayer();
	if player == nil then return end
	HideCollectTempSkillBtn();
	for i = 1, 2 do
		local nSkillId =SkillMgr:getTmpSkillId(i-1);
		local nSkillLv = SkillMgr:getTmpSkillLv(i-1);
		local Tempbtn = getglobal("ClanCollectTmpFrameCollectShortCut"..i);
		if nSkillId ~= 0 then
			local tagSkillInfo = SkillMgr:getSpellStaticAtt( nSkillId, 1 );
			if tagSkillInfo ~= nil  then				
				local Temptext= getglobal(Tempbtn:GetName().."Icon");
				Tempbtn:SetClientUserData( 0, nSkillId );
				Tempbtn:SetClientUserData( 1, nSkillLv );
				local IconPath = GetSkillIconPath()..tagSkillInfo.IconID1..".tga";
				if not IsInExistence( IconPath ) then
					IconPath = DEFAULT_ITEM_PATH;
				end
				Temptext:SetTexture( IconPath );
				Tempbtn:Show();
				Temptext:SetBlendAlpha( 1.0 );
				GameShortCut:setTmpShortCut( nSkillId,nSkillLv , i-1, 0,1 );
			end
		end
	end
	
	bHideTmpFrame = false; 
	if not bInTmp then
		PhotoFrame:SetPoint( "topleft", "$parent", "topleft", 0, 0 );
		ExtendSkillRightFrame:SetPoint("right","$parent","right",-1,60); 
		
		if not ShortCutFrame:IsShown() then
			ShortCutFrame:Show(); 
			MoveShortcutFrameShow();
		end

		ExtendSkillFrame:MoveFrame("bottom", MOVE_DURATION, ExtendSkillFrame:GetWidth(), 144 );	  
		ShortCutFrame:MoveFrame("bottom", MOVE_DURATION, ShortCutFrame:GetWidth(), ShortCutFrame:GetHeight() );	  
		PhotoFrame:MoveFrame("left", MOVE_DURATION, PhotoFrame:GetWidth(), PhotoFrame:GetHeight());
		ExtendSkillRightFrame:MoveFrame("right",MOVE_DURATION, 100, ExtendSkillRightFrame:GetHeight());
		if ExtendSkillLeftFrame:IsShown() then
			ExtendSkillLeftFrame:SetPoint("right","$parent","right",-47,60); 
			ExtendSkillLeftFrame:MoveFrame("right",MOVE_DURATION, 100, ExtendSkillLeftFrame:GetHeight());
		end			
		ShortCutButtonsFrame:MoveFrame("bottom", MOVE_DURATION, ShortCutButtonsFrame:GetWidth(), ShortCutButtonsFrame:GetHeight() );

		szNeedChangeFrame	= "ClanCollectTmpFrame";
		nBeginMoveTime		= os.clock();
		bShowTmpFrame		= true;
	end

	if XPFrame:IsShown() then
		XPFrame:Hide();
	end
	bInTmp = true;
	RefreshAttriInfo();
end

function HideCollectTempSkillBtn()
	for i = 1, 4 do
		local btn = getglobal("ClanCollectTmpFrameCollectShortCut"..i);
		btn:Hide();
		btn:SetClientUserData( 0, 0 );	
		btn:SetClientUserData( 1, 0 );	
	end
end

function unBindCollectMachine()
	HideCollectTempSkillBtn();
	if SpellTipsFrame:IsShown() then
		SpellTipsFrame:Hide();
	end
	
	PhotoFrame:SetPoint( "topleft", "$parent", "topleft", 0, 0 );
	PhotoFrame:MoveFrame("left", MOVE_DURATION, PhotoFrame:GetWidth(), PhotoFrame:GetHeight());
	ClanCollectTmpFrame:MoveFrame( "bottom", MOVE_DURATION, TempShortCutFrame:GetWidth(), TempShortCutFrame:GetHeight() );	
	ShortCutButtonsFrame:MoveFrame( "bottom", MOVE_DURATION, TempShortCutFrame:GetWidth(), TempShortCutFrame:GetHeight() );

	nTempBeginMoveTime = os.clock();
	bHideTmpFrame = true;

	nBeginClock = os.clock();
	bInTmp = false;
	if PetDialogFrame:IsShown() then
		PetDialogFrame:Hide();
	end
	if IsTrumpCamp() then
		XPFrame:Show();
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local machine = mainplayer:getMonsterMachine();
	machine:cancelSkillSelectData();
	TmpMachineBuffFrame:Hide();
end

------------------------------------------------------------------------------------------
local nCurMonsterID = 0;	--对应的机械id
local nCurSelectIdx = 0;	--选中的座位

function ClanTankTmpFrame_OnLoad()
	this:RegisterEvent("GE_CLAN_BINDMACHINE");
	this:RegisterEvent("GE_CLAN_UNBINDMACHINE");
	this:RegisterEvent("GE_TANK_SIT_CHANGE");
	this:RegisterEvent("GE_PASSENGER_CHANGE");
	for i=1,8 do
		local btn = getglobal( "ClanTankTmpFrameCollectShortCut"..i );
		local FontText = getglobal("ClanTankTmpFrameCollectShortCut"..i.."Text");
		FontText:SetText( btn:GetClientID() );
	end
	ClanTankTmpFrameRepairExpand:SetClientUserData(0, 311)
	ClanTankTmpFrameEnergyExpand:SetClientUserData(0, 283)
	ClanTankTmpFrameFoodExpand:SetClientUserData(0, 48)
	ClanTankTmpFrameFoodExpand:SetClientUserData(2, 1020)
	ClanTankTmpFrameHPExpand:SetClientUserData(0, 46)
	ClanTankTmpFrameRepairExpand:SetClientString("ClanTankTmpFrameCollectShortCut5")
	ClanTankTmpFrameEnergyExpand:SetClientString("ClanTankTmpFrameCollectShortCut6")
	ClanTankTmpFrameFoodExpand:SetClientString("ClanTankTmpFrameCollectShortCut7")
	ClanTankTmpFrameHPExpand:SetClientString("ClanTankTmpFrameCollectShortCut8")
	ClanTankTmpFrameRepairExpandBtn:SetClientString("ClanTankTmpFrameRepairExpand")
	ClanTankTmpFrameEnergyExpandBtn:SetClientString("ClanTankTmpFrameEnergyExpand")
	ClanTankTmpFrameFoodExpandBtn:SetClientString("ClanTankTmpFrameFoodExpand")
	ClanTankTmpFrameHPExpandBtn:SetClientString("ClanTankTmpFrameHPExpand")
end

function ClanTankTmpFrame_OnEvent()
	if arg1 == "GE_CLAN_BINDMACHINE" then
		local player = ActorMgr:getMainPlayer();		
		if player == nil then return end
		local monsterMachine = player:getMonsterMachine();
		local monsterMachineInfo = monsterMachine:getMonMachineDef();
		if monsterMachineInfo == nil then return end
		if monsterMachineInfo.CollectType ~= 0 then
			OnBindCollectMachine();
		else
			OnBindTankMachine(monsterMachine.m_playerInfo.m_nMonsterMemID);
		end
	end
	if arg1 == "GE_CLAN_UNBINDMACHINE" then
		local player = ActorMgr:getMainPlayer();
		if player == nil then return end
		local monsterMachine = player:getMonsterMachine();
		local monsterMachineInfo = monsterMachine:getMonMachineDef();
		if monsterMachineInfo == nil then return end
		if monsterMachineInfo.CollectType ~= 0 then
			unBindCollectMachine();
		else
			UnBindTankMachine();
		end
		util.BreakBlockPrepare("DropMonsterMachine")
	end
	if arg1 == "GE_TANK_SIT_CHANGE" then
		HideTankTempSkillBtn();
		for i = 1, 2 do
			local nSkillId =SkillMgr:getTmpSkillId(i-1);
			local nSkillLv = SkillMgr:getTmpSkillLv(i-1);
			local Tempbtn = getglobal("ClanTankTmpFrameCollectShortCut"..i);
			if nSkillId ~= 0 then   
				local tagSkillInfo = SkillMgr:getSpellStaticAtt( nSkillId, 1 );
				if tagSkillInfo ~= nil  then					
					local Temptext= getglobal(Tempbtn:GetName().."Icon");
					Tempbtn:SetClientUserData( 0, nSkillId );
					Tempbtn:SetClientUserData( 1, nSkillLv );
					local IconPath = GetSkillIconPath()..tagSkillInfo.IconID1..".tga";
					if not IsInExistence( IconPath ) then
						IconPath = DEFAULT_ITEM_PATH;
					end
					Temptext:SetTexture( IconPath );
					Tempbtn:Show();
					Temptext:SetBlendAlpha( 1.0 );
					GameShortCut:setTmpShortCut( nSkillId, nSkillLv, i-1, 0,1 );
				end				
			end
		end
		ClanTankTmpFrameUpdateSitFont();
	end
	if arg1 == "GE_PASSENGER_CHANGE" then
		if SelectSitFrame:IsShown() then
			SelectSitFrame_OnShow();
		end
	end
end

function OnPassengerInfo( monsterID )
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	nCurMonsterID = monsterID;	
	if not mainplayer:isOnMonsterMachine() then
		SelectSitFrame:Show();
	end	
end

function UnBindTankMachine()
	HideTankTempSkillBtn();
	if SpellTipsFrame:IsShown() then
		SpellTipsFrame:Hide();
	end
	PhotoFrame:SetPoint( "topleft", "$parent", "topleft", 0, 0 );
	PhotoFrame:MoveFrame("left", MOVE_DURATION, PhotoFrame:GetWidth(), PhotoFrame:GetHeight());
	ClanTankTmpFrame:MoveFrame( "bottom", MOVE_DURATION, TempShortCutFrame:GetWidth(), TempShortCutFrame:GetHeight() );	
	ShortCutButtonsFrame:MoveFrame( "bottom", MOVE_DURATION, TempShortCutFrame:GetWidth(), TempShortCutFrame:GetHeight() );

	nTempBeginMoveTime = os.clock();
	bHideTmpFrame = true;

	nBeginClock = os.clock();
	bInTmp = false;
	if PetDialogFrame:IsShown() then
		PetDialogFrame:Hide();
	end
	if IsTrumpCamp() then
		XPFrame:Show();
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local machine = mainplayer:getMonsterMachine();
	machine:cancelSkillSelectData();
	TmpMachineBuffFrame:Hide();
end

function ClanTankTmpFrameUpdateSitFont()
	ClanTankTmpFrameCurrSitFont:SetText("");
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local monMachine = mainplayer:getMonsterMachine();
	ClanTankTmpFrameCurrSitFont:SetText( monMachine.m_playerInfo.m_nSitIdx + 1 );
end

function HideTankTempSkillBtn()
	for i = 1, 4 do
		local btn = getglobal("ClanTankTmpFrameCollectShortCut"..i);
		btn:Hide();
		btn:SetClientUserData( 0, 0 );	
		btn:SetClientUserData( 1, 0 );	
	end

end

function OnBindTankMachine( id )
	local Machine = ActorMgr:FindActor(id)
	local nPassengerNum = 1
	if Machine then
		local monsterMachine = Machine:getMonsterMachine()
		local monsterMachineInfo = monsterMachine:getMonMachineDef(monsterMachine.m_monsterInfo.m_nMonsterDefID)
		nPassengerNum = monsterMachineInfo.PassengerNum		
	end
	if nPassengerNum == 1 then
		--替换按钮的背景图，隐藏座位按钮，隐藏位置背景和文字
		ClanTankTmpFrameButtonBkg:SetSize( 108,30 );
		ClanTankTmpFrameButtonBkg:SetTexUV( 0,697,108,30 );
		ClanTankTmpFrameSitBtn:Hide();
		ClanTankTmpFrameCurrSitBkg:Hide();
		ClanTankTmpFrameCurrSitFont:Hide();
	else
		ClanTankTmpFrameButtonBkg:SetSize( 140,30 );
		ClanTankTmpFrameButtonBkg:SetTexUV( 808,824,140,30 );
		ClanTankTmpFrameSitBtn:Show();
		ClanTankTmpFrameCurrSitBkg:Show();
		ClanTankTmpFrameCurrSitFont:Show();
		ClanTankTmpFrameUpdateSitFont();		
	end

	local player	= ActorMgr:getMainPlayer();
	if player == nil then return end
	if player:isInTeam() and ( TeamManager:isInTeamFollow() or isAnyTeamMemberInFollow() ) then
		TeamManager:Team_OP_CancelTeamFollow();
	end
	HideTankTempSkillBtn();
	for i = 1,4 do
		local nSkillId = SkillMgr:getTmpSkillId(i-1);
		local nSkillLv = SkillMgr:getTmpSkillLv(i-1);
		local Tempbtn = getglobal("ClanTankTmpFrameCollectShortCut"..i);
		if nSkillId ~= 0 then
			local tagSkillInfo = SkillMgr:getSpellStaticAtt( nSkillId, nSkillLv );
			if tagSkillInfo ~= nil  then				
				local Temptext= getglobal(Tempbtn:GetName().."Icon");
				Tempbtn:SetClientUserData( 0, nSkillId );
				Tempbtn:SetClientUserData( 1, nSkillLv );
				local IconPath = GetSkillIconPath()..tagSkillInfo.IconID1..".tga";
				-- 如果没找到物品的图标则使用默认图标显示
				if not IsInExistence( IconPath ) then
					IconPath = DEFAULT_ITEM_PATH;
				end
				Temptext:SetTexture( IconPath );
				Tempbtn:Show();
				Temptext:SetBlendAlpha( 1.0 );
				GameShortCut:setTmpShortCut( nSkillId, nSkillLv, i-1, 0,1 );
			end
		end
	end
	
	bHideTmpFrame = false; 
	if not bInTmp then
		PhotoFrame:SetPoint( "topleft", "$parent", "topleft", 0, 0 );
		ExtendSkillRightFrame:SetPoint("right","$parent","right",-1,60); 
		
		if not ShortCutFrame:IsShown() then
			ShortCutFrame:Show(); 
			MoveShortcutFrameShow();
		end

		ExtendSkillFrame:MoveFrame("bottom", MOVE_DURATION, ExtendSkillFrame:GetWidth(), 144 );	  
		ShortCutFrame:MoveFrame("bottom", MOVE_DURATION, ShortCutFrame:GetWidth(), ShortCutFrame:GetHeight() );	  
		PhotoFrame:MoveFrame("left", MOVE_DURATION, PhotoFrame:GetWidth(), PhotoFrame:GetHeight());
		ExtendSkillRightFrame:MoveFrame("right",MOVE_DURATION, 100, ExtendSkillRightFrame:GetHeight());
		if ExtendSkillLeftFrame:IsShown() then
			ExtendSkillLeftFrame:SetPoint("right","$parent","right",-47,60); 
			ExtendSkillLeftFrame:MoveFrame("right",MOVE_DURATION, 100, ExtendSkillLeftFrame:GetHeight());
		end			
		ShortCutButtonsFrame:MoveFrame("bottom", MOVE_DURATION, ShortCutButtonsFrame:GetWidth(), ShortCutButtonsFrame:GetHeight() );

		szNeedChangeFrame	= "ClanTankTmpFrame";
		nBeginMoveTime		= os.clock();
		bShowTmpFrame		= true;
	end

	if XPFrame:IsShown() then
		XPFrame:Hide();
	end
	bInTmp = true;
	RefreshTankAttriInfo();
end

function RefreshTankAttriInfo()
	local player = ActorMgr:getMainPlayer();
	local monsterMachine = player:getMonsterMachine();
	local monster = ActorMgr:FindActor( monsterMachine.m_playerInfo.m_nMonsterMemID );
	if monster == nil then return end
	monsterMachine = monsterMachine:getPlayerMonsterMachine();
	if monsterMachine == nil then return end
	local nEnergy = monsterMachine:getEnergy();
	local nMaxEnergy = monsterMachine:getMaxEnergy();
	local energyScale = nEnergy/nMaxEnergy;
	local hp = monster:getHP();
	local maxHp = monster:getMaxHP();
	local hpScale = hp/maxHp;
	if energyScale > 1 then
		energyScale = 1;
	end
	if energyScale < 0 then
		energyScale = 0;
	end
	ClanTankTmpFrameNPTex:SetTexUV( 745,692+(60-60*energyScale),16,60*energyScale );
	ClanTankTmpFrameNPTex:SetSize( 16,60*energyScale );
	
	if hpScale > 1 then
		hpScale = 1;
	end
	if hpScale < 0 then
		hpScale = 0;
	end
	ClanTankTmpFrameHPTex:SetTexUV( 728,692+( 60-60*hpScale ),16,60*hpScale );
	ClanTankTmpFrameHPTex:SetSize( 16,60*hpScale );
	if nEnergy == 0 then
		local UVATexture = getglobal( "ClanTankTmpFrameDownBtnUVAnimationTex" )
		if not UVATexture:IsShown() then
			UVATexture:SetUVAnimation(50,true);
			UVATexture:Show();
		end
	end
end

function ClanTankTmpFrame_OnShow()
	SwitchDropMonsterMachineBtn(ClanTankTmpFrameDownBtn, true)
	local MainPlayer = ActorMgr:getMainPlayer();
	ExtendSkillRightFrame:SetPoint("right","$parent","right",-1+100,60);   
	if ExtendSkillLeftFrame:IsShown() then	
		ExtendSkillLeftFrame:SetPoint("right","$parent","right",-47+100,60); 
	end
	ClanTankTmpFrame:SetPoint("bottom","$parent","bottom",0,-1+ClanTankTmpFrame:GetHeight()); 
	ClanTankTmpFrame:MoveFrame("top", MOVE_DURATION, ClanTankTmpFrame:GetWidth(), ClanTankTmpFrame:GetHeight());

	PhotoFrame:SetPoint( "topleft", "$parent", "topleft", -PhotoFrame:GetWidth(), 0 );
	PhotoFrame:MoveFrame("right", MOVE_DURATION, PhotoFrame:GetWidth(), PhotoFrame:GetHeight());

	ShortCutButtonsFrame:SetPoint("bottom","$parent","bottom",0,ShortCutButtonsFrame:GetHeight());
	ShortCutButtonsFrame:MoveFrame("top", MOVE_DURATION, ShortCutButtonsFrame:GetWidth()+3, ShortCutButtonsFrame:GetHeight());
	CheckShortcutEffect( "ClanTankTmpFrame_OnShow" );
	TmpMachineBuffFrame:Show();
	TempShortCutFrame:Hide();
	ClanCollectTmpFrame:Hide();
	NetWorkStatus:SetPoint( "topleft", "ShortCutButtonsFrame", "topleft",52,23);
	--elmer
	local UVATexture = getglobal( "ClanTankTmpFrameDownBtnUVAnimationTex" )
	UVATexture:Hide();
end

function ClanTankTmpFrame_OnUpdate()
	if bHideTmpFrame and ( os.clock() - nTempBeginMoveTime ) >= MOVE_DURATION then	
		ClanTankTmpFrame:Hide();
		ShortCutFrame:Show(); 
		MoveShortcutFrameShow();
		ExtendSkillFrame:Show();
		bHideTmpFrame = false; 
		nTempBeginMoveTime = os.clock();
		HideTempShortCutFrameGuideEffectTip();
		CheckShortcutEffect( "ClanTankTmpFrame_OnUpdate" );
		if SelectSitFrame:IsShown() then
			SelectSitFrame:Hide();
		end
		nCurMonsterID = 0;
	end
	RefreshTankAttriInfo();
end

function ClanTankTmpFrameSitBtnOnClick()
	if SelectSitFrame:IsShown() then
		SelectSitFrame:Hide();
	else
		local player = ActorMgr:getMainPlayer();
		local monsterMachine = player:getMonsterMachine();
		nCurMonsterID = monsterMachine.m_playerInfo.m_nMonsterMemID;
		SelectSitFrame:Show();
	end
end

function SelectSitFrameClearSelect()
	for i=1,6 do
		local btn = getglobal( "SelectSitFrameSitBtn"..i );
		local hairPhoto = getglobal( "SelectSitFrameSitBtn"..i.."HairPhoto" );
		local photo = getglobal( "SelectSitFrameSitBtn"..i.."Photo" );
		local hairShadow = getglobal( "SelectSitFrameSitBtn"..i.."HairShadowTex" );
		local bodyTex = getglobal( "SelectSitFrameSitBtn"..i.."BodyTex" );
		local nameFont = getglobal( "SelectSitFrameSitBtn"..i.."NameFont" );

		hairPhoto:Hide();
		photo:Hide();
		hairShadow:Hide();
		bodyTex:Hide();
		nameFont:SetText( i .."号位" );

		-- 设置按钮状态
		btn:ChangeNormalTexture( 112, 480, 51, 51 );
		btn:ChangeHighlightTexture( 112, 532, 51, 51 );
		btn:ChangePushedTexture( 112, 584, 51, 51 );
		btn:Disable();
		btn:DisChecked();
	end
	nCurSelectIdx = -1;
	SelectSitFrameSitBtn1DriveFont:SetText( "驾驶" );
end

function SelectSitFrame_OnShow()
	SelectSitFrameClearSelect();
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local monster = ActorMgr:FindActor( nCurMonsterID );
	if monster == nil then return end;
	local monMachine = monster:getMonsterMachine();
	local passengerInfo = monMachine:getMachinePassgerInfo();
	for i=1,passengerInfo.MaxPassenger do
		local btn = getglobal( "SelectSitFrameSitBtn"..i );
		local hairPhoto = getglobal( "SelectSitFrameSitBtn"..i.."HairPhoto" );
		local photo = getglobal( "SelectSitFrameSitBtn"..i.."Photo" );
		local hairShadow = getglobal( "SelectSitFrameSitBtn"..i.."HairShadowTex" );
		local bodyTex = getglobal( "SelectSitFrameSitBtn"..i.."BodyTex" );
		if passengerInfo.Passengers[i-1].PassengerID ~= 0 then			
			SetPhoto( passengerInfo.Passengers[i-1].Hair,passengerInfo.Passengers[i-1].Face,"SelectSitFrameSitBtn"..i.."HairPhoto","SelectSitFrameSitBtn"..i.."Photo",
					passengerInfo.Passengers[i-1].Gender,passengerInfo.Passengers[i-1].Head,"SelectSitFrameSitBtn"..i.."HairShadowTex","SelectSitFrameSitBtn"..i.."BodyTex" );

			hairPhoto:Show();
			photo:Show();
			hairShadow:Show();
			bodyTex:Show();
			btn:ChangeNormalTexture( 549,676,51,51 );
			btn:ChangeHighlightTexture( 636, 676, 51, 51 );
			btn:ChangePushedTexture( 636, 676, 51, 51 );
			if i == 1 then
				SelectSitFrameSitBtn1DriveFont:SetText("");
			end
		end
		btn:Enable();
	end
	if monMachine:canChangeSide() then
		SelectSitFrameOKBtn:Show();
		SelectSitFrameCancelBtn:Show();
		SelectSitFrameFont:Hide();
	else
		SelectSitFrameOKBtn:Hide();
		SelectSitFrameCancelBtn:Hide();
		SelectSitFrameFont:Show();
	end
	SelectSitTipsFrame:Hide();
end

function SelectSitFrameSitBtn_OnClick()
	local monster = ActorMgr:FindActor( nCurMonsterID );
	if monster == nil then return end;
	local monMachine = monster:getMonsterMachine();
	local passengerInfo = monMachine:getMachinePassgerInfo();
	if arg1 == "LeftButton" then		
		if monMachine:canChangeSide() then			
			if passengerInfo.Passengers[ this:GetClientID() - 1 ].PassengerID == 0 then
				nCurSelectIdx = this:GetClientID() - 1;
				DisCheckAllBtn( "SelectSitFrameSitBtn",6 );
				this:Checked();
			end
		end
	elseif arg1 == "RightButton" then
		if not monMachine:canChangeSide() and monMachine:isMainPlayerDriver() then
			--local selectid = passengerInfo.Passengers[ this:GetClientID() - 1 ].PassengerID;
			local passenger = passengerInfo.Passengers[ this:GetClientID() - 1 ];
			if passenger.PassengerID ~= 0 then
				MessageBox( "请离机械","是否确认请离" .. passenger.RoleName .. "乘客" );
				MessageBoxFrame:SetClientUserData( 0,passenger.PassengerID );
			end
			
			if selectid ~= 0 then
				--local mainplayer = ActorMgr:getMainPlayer();
				--mainplayer:requestMachineKickPlayer( selectid );
			end
		end
	end	
end

function SelectSitFrameSitBtn_OnEnter()
	local monster = ActorMgr:FindActor( nCurMonsterID );
	if monster == nil then return end;
	local monMachine = monster:getMonsterMachine();
	local passengerInfo = monMachine:getMachinePassgerInfo();	
	local passenger = passengerInfo.Passengers[ this:GetClientID() - 1 ]
	if passenger.PassengerID ~= 0 then
		SelectSitTipsFrameName:SetText( passenger.RoleName );
		SelectSitTipsFrameLevel:SetText( "等级：" .. passenger.Level );
		SelectSitTipsFrameRace:SetText( "职业：" .. t_raceList[ passenger.Career ] );
		if passenger.ClanName ~= "" then
			SelectSitTipsFrameClanName:SetText( "公会："..passenger.ClanName );
		else
			SelectSitTipsFrameClanName:SetText( "" );
		end
		SetPhoto( passenger.Hair, passenger.Face, "SelectSitTipsFrameHairPhoto", "SelectSitTipsFramePhoto", passenger.Gender, passenger.Head,
				"SelectSitTipsFrameHairShadowTex", "SelectSitTipsFrameBodyTex" );

		SelectSitTipsFrame:SetPoint("topleft","UIClient","topleft",GetCursorPosX(),GetCursorPosY());
		SelectSitTipsFrame:Show();
	end	
end

function SelectSitFrameOKBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	mainplayer:requestRideMonsterMachine( nCurMonsterID,nCurSelectIdx );
	SelectSitFrame:Hide();
end

function SelectSitFrame_OnHide()
	nCurSelectIdx = 0;
	SelectSitTipsFrame:Hide();
end

function SelectSitFrame_OnUpdate()
	local monster = ActorMgr:FindActor( nCurMonsterID );
	if monster == nil then
		this:Hide();
	end

	if util.CalActorsDistance(monster, ActorMgr:getMainPlayer()) > (NpcDialog:getMaxNpcTalkDist( monster ) / 100) ^ 2 then
		this:Hide();
	end
end

function TempShortCutFrameEnergeTips()
	local MainPlayer = ActorMgr:getMainPlayer()
	if MainPlayer then	
		local energy 	= MainPlayer:getEnerge();
		local eqiup		= MainPlayer:getEquip();
		local szText;
		if  eqiup:getTmpMachin() ~= 0 then
			szText = "#G能量："..energy.."#n\n可前往时间城“#R格罗佛#n”\n处或#R公会城市#n内购买机械\n能量或机械扳手"
			if eqiup:getTmpMachin() == 1460003 then
				szText = "#G能量："..energy.."#n\n此处为机械的能量，\n使用Q键上方的数字键“#G1#n”“#G2#n”\n可发动强悍的技能！"
			end
			SetGameTooltips(this:GetName(),szText )
		end
	end
end

--------------------------------	怪物机械buff显示	---------------------------------
local MAX_MACHINE_BUF_COUNT = 10;	--先弄10个

function TmpMachineBuffFrame_OnLoad()
	this:RegisterEvent( "GE_MONSTER_MACHINE_BUFF_UPDATE" );
end

function TmpMachineBuffFrame_OnEvent()
	if arg1 == "GE_MONSTER_MACHINE_BUFF_UPDATE" and TmpMachineBuffFrame:IsShown() then
		TmpMachineRefreshBuff();
	end
end

function TmpMachineBuffFrame_OnShow()
	TmpMachineBuffFrame:SetPoint( "bottom","UIClient","bottom",-20,1-ClanTankTmpFrame:GetHeight() )
	TmpMachineRefreshBuff();
end



function TmpMachineRefreshBuff()
	for i = 1, MAX_MACHINE_BUF_COUNT do
		DelCollBuff( "TmpMachineBuffFrameBuff" .. i );	
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if not mainplayer:isOnMonsterMachine() then return end
	local monsterMachine = mainplayer:getMonsterMachine();
	local player = ActorMgr:FindActor( monsterMachine.m_playerInfo.m_nMonsterMemID );
	if player == nil then
		return 
	end;
	
	local BufStatus	= player:getBufStatus();

	local nBuffBtnCount = 1;
	for i=1, MAX_GOOD_STATUS do
		if nBuffBtnCount > MAX_MACHINE_BUF_COUNT then
			break;
		end

		local Buf		= BufStatus:getGoodStatus( i - 1 );
		local BufIconId	= Buf:getIconId();
		local BufText	= "uires\\TuPianLei\\JiNeng\\Buff\\" .. BufIconId..".tga";
		local btn		= getglobal( "TmpMachineBuffFrameBuff" .. nBuffBtnCount );
		if Buf:isShow() and Buf:getTotalLife() > 0 then
			local bAddResult = AddCoolBuffer( "TmpMachineBuffFrameBuff" .. nBuffBtnCount, GetBuffIconPath( BufIconId ) ,Buf:getLife(), Buf:getTotalLife(), 
									Buf:getBufId(), Buf:getBufLv(), Buf:getBufId() == CLIENT_DIE_REMIT_BUFFID and ( Buf:getOverlay() ) or ( Buf:getOverlay() + 1 ), 
									player:GetID(), Buf );
			if bAddResult then
				nBuffBtnCount = nBuffBtnCount + 1;
			end			
		end
	end

	for i=1, MAX_NOLIMIT_STATUS do
		if nBuffBtnCount > MAX_MACHINE_BUF_COUNT then
			break;
		end
		local Buf		= BufStatus:getNolimitStatus(i-1);
		local BufIconId	= Buf:getIconId();
		local BufText	= "uires\\TuPianLei\\JiNeng\\Buff\\"..BufIconId..".tga";
		local btn		= getglobal( "TmpMachineBuffFrameBuff" .. nBuffBtnCount );
		if Buf:isShow() and Buf:getTotalLife() > 0 then
			local bAddResult = AddCoolBuffer( "TmpMachineBuffFrameBuff" .. nBuffBtnCount, GetBuffIconPath( BufIconId ), Buf:getLife(), Buf:getTotalLife(), 
							Buf:getBufId(), Buf:getBufLv(), Buf:getBufId() == CLIENT_DIE_REMIT_BUFFID and ( Buf:getOverlay() ) or ( Buf:getOverlay() + 1 ), 
							player:GetID(), Buf );
			if bAddResult then
				nBuffBtnCount = nBuffBtnCount + 1;
			end		
		end
	end

	for i=1, MAX_BAD_STATUS do
		if nBuffBtnCount > MAX_MACHINE_BUF_COUNT then
			break;
		end
		local Buf		= BufStatus:getBadStatus(i-1);
		local BufIconId	= Buf:getIconId();
		local BufText	= "uires\\TuPianLei\\JiNeng\\Buff\\"..BufIconId..".tga";
		local btn		= getglobal( "TmpMachineBuffFrameBuff" .. nBuffBtnCount );
		if Buf:isShow() and Buf:getTotalLife() > 0 then
			local bAddResult = AddCoolBuffer( "TmpMachineBuffFrameBuff" .. nBuffBtnCount, GetBuffIconPath( BufIconId ),Buf:getLife(), Buf:getTotalLife(), 
												Buf:getBufId(), Buf:getBufLv(), 
												Buf:getBufId() == CLIENT_DIE_REMIT_BUFFID and ( Buf:getOverlay() ) or ( Buf:getOverlay() + 1 ),
												player:GetID(), Buf );
			if bAddResult then
				nBuffBtnCount = nBuffBtnCount + 1;
			end		
		end
	end
end

local function UpdateClanCollectMachineResourceFrame()
	if not ClanCollectMachineResourceFrame:IsShown() then
		return
	end
	local player = ActorMgr:getMainPlayer()
	local monsterMachine = player:getMonsterMachine()
	monsterMachine = monsterMachine:getPlayerMonsterMachine()
	if not monsterMachine then
		return
	end
	local ui
	local maxOre = monsterMachine:getMaxOre()
	local hasOre = 0
	for i = 1, OreTypeNum do
		local ore = monsterMachine:getOre(i)
		hasOre = hasOre + ore
		ui = getglobal("ClanCollectMachineResourceFrameItem"..i.."Text")
		ui:SetText(ore)
		ui = getglobal("ClanCollectMachineResourceFrameItem"..i.."Slip")
		ui:SetSize(math.floor(120 * (ore / maxOre) + 0.5), ui:GetHeight())
	end
	ClanCollectMachineResourceFrameTotalText:SetText(string.format("%d/%d", hasOre, maxOre))
	ClanCollectMachineResourceFrameTotalSlip:SetSize(math.floor(120 * (hasOre / maxOre) + 0.5),
			ClanCollectMachineResourceFrameTotalSlip:GetHeight())
end

local function AdjustClanCollectMachineResourceFrame()
	local player = ActorMgr:getMainPlayer()
	local monsterMachine = player:getMonsterMachine()
	monsterMachine = monsterMachine:getPlayerMonsterMachine()
	if not monsterMachine then
		return
	end
	local t = {}
	for i = 1, 5 do
		local id = monsterMachine:getOreId(i)
		if id ~= 0 then
			local itemdef = getItemDef(id)
			if itemdef then
				table.insert(t, itemdef.Name)
			end
		end
	end
	local n = #t
	if n > 0 then
		local height = ClanCollectMachineResourceFrameTex:GetHeight()
		local h1 = height / OreTypeNum * n
		local h2 = ClanCollectMachineResourceFrame:GetHeight() - height + h1;
		ClanCollectMachineResourceFrame:SetHeight(h2)
		ClanCollectMachineResourceFrameTex:SetHeight(h1)
		for i = 1, n do
			local item = getglobal("ClanCollectMachineResourceFrameItem"..i)
			if not item:IsShown() then
				item:Show()
			end
			local text = getglobal("ClanCollectMachineResourceFrameItem"..i.."Title")
			text:SetText(t[i])
		end
		for i = n + 1, OreTypeNum do
			local item = getglobal("ClanCollectMachineResourceFrameItem"..i)
			item:Hide()
		end
		OreTypeNum = n;
		local szText = "#G采集后请到#L资源提取员@@[302123,1000]#n#G处提取#n\n#B掉线后一部分资源直接获取到背包#n";
		ClanCollectMachineResourceFrameRichInfo:SetText( szText, 66,  168, 61 );
		local richHight = h2 - 40;
		ClanCollectMachineResourceFrameRichInfo:SetPoint( "topleft" ,"ClanCollectMachineResourceFrame" ,"topleft" ,10 , richHight );
	else
		ClanCollectMachineResourceFrame:Hide()
	end
	ClanCollectMachineResourceFrame:SetPoint("topleft", "UIClient", "topleft", 339, 518)
end

function ClanCollectMachineResourceFrame_OnLoad()
	this:RegisterEvent("GE_MONSTER_ATTR_CHANGE")
end

function ClanCollectMachineResourceFrame_OnEvent()
	if arg1 == "GE_MONSTER_ATTR_CHANGE" then
		UpdateClanCollectMachineResourceFrame()
		UpdateWatchMachineResourceFrame();
	end
end

function ClanCollectMachineResourceFrame_OnShow()
	AdjustClanCollectMachineResourceFrame()
	UpdateClanCollectMachineResourceFrame()
	ClanCollectTmpFrameResourceBtnTexture:Hide();
end
function ClanCollectMachineResourceFrame_OnHide()
	ClanCollectTmpFrameResourceBtnTexture:Show();
end
local t_linkMoveMapID = {
				{ nCurMapId = 2000, linkMoveMapID = 1000,},
				{ nCurMapId = 2100, linkMoveMapID = 1000,},
				{ nCurMapId = 2202, linkMoveMapID = 1000,},
				{ nCurMapId = 3300, linkMoveMapID = 2300,},
				{ nCurMapId = 3302, linkMoveMapID = 2300,},
				{ nCurMapId = 3304, linkMoveMapID = 3404,},
				{ nCurMapId = 3400, linkMoveMapID = 2400,},
				{ nCurMapId = 3500, linkMoveMapID = 2500,},
				{ nCurMapId = 3200, linkMoveMapID = 2201,},
				{ nCurMapId = 3110, linkMoveMapID = 3100,},
				{ nCurMapId = 3510, linkMoveMapID = 2500,},
				{ nCurMapId = 3210, linkMoveMapID = 2201,},
			}
function ClanCollectMachineResourceFrameRich_OnClick()
	local mapId;
	local MaxMapControl	= MapShower:GetMaxMapContal();
	if MaxMapControl:getMapID() == 0 then
		mapId = MapShower:getRealMapId();
	else
		mapId = MaxMapControl:getMapID();
	end
--[[
	if ( arg3 == "LeftButton" ) then
		local strLinkItemHref	= arg2;
		local bLinkNpc, nNpcId, nTargetMapId = GetNpcPos( strLinkItemHref );
		if bLinkNpc == true then
			local flag	= true;
			for i = 1, #t_linkMoveMapID do
				if  t_linkMoveMapID[i]["nCurMapId"] == mapId then
					MapShower:GetMaxMapContal():linkMoveto( t_linkMoveMapID[i]["linkMoveMapID"], nNpcId );
					flag = false;
					break;
				end
			end
			if flag then
				MapShower:GetMaxMapContal():linkMoveto( mapId, nNpcId );
			end			
			return;
		end
	end
--]]	
	local flag	= true;
	for i = 1, #t_linkMoveMapID do
		if  t_linkMoveMapID[i]["nCurMapId"] == mapId then
			MapShower:GetMaxMapContal():linkMoveto( t_linkMoveMapID[i]["linkMoveMapID"], 302123 );
			flag = false;
			break;
		end
	end
	if flag then
		MapShower:GetMaxMapContal():linkMoveto( mapId, 302123 );
	end
end
local TypeText = {
	[5101001] = "F",
	[5101002] = "G",
	[5101003] = "H",
	[5101004] = "I",
	[5101005] = "K",
	[5101009] = "J",
	[5102001] = "A",
	[5102002] = "B",
	[5102003] = "C",
	[5102004] = "D",
	[5102005] = "E",	
}

local lastTextTagTime = 0

function OnMonsterMachineAttribChg(type, value)
	if value < 0 then
		return
	end
	if type == 0 then
		AddFlyFont(20, MACHINE_ENERGY, string.format("%s%d", value > 0 and "+" or "-", math.abs(value)))
	else
		local now = os.clock()
		if now < lastTextTagTime then
			lastTextTagTime = now
		end
		if lastTextTagTime + 1 > now then
			return
		end
		lastTextTagTime = now
		local player = ActorMgr:getMainPlayer()
		local monsterMachine = player:getMonsterMachine()
		monsterMachine = monsterMachine:getPlayerMonsterMachine()
		if not monsterMachine then
			return
		end
		local id = monsterMachine:getOreId(type)
		if type == 1 then
			AddFlyFont(21, MACHINE_RESOURCE, string.format("%s%d%s", value > 0 and "+" or "-", math.abs(value), TypeText[id]))
		elseif type == 2 then
			AddFlyFont(21, MACHINE_RESOURCE, string.format("%s%d%s", value > 0 and "+" or "-", math.abs(value), TypeText[id]))
		elseif type == 3 then
			AddFlyFont(21, MACHINE_RESOURCE, string.format("%s%d%s", value > 0 and "+" or "-", math.abs(value), TypeText[id]))
		elseif type == 4 then
			AddFlyFont(21, MACHINE_RESOURCE, string.format("%s%d%s", value > 0 and "+" or "-", math.abs(value), TypeText[id]))
		elseif type == 5 then
			AddFlyFont(21, MACHINE_RESOURCE, string.format("%s%d%s", value > 0 and "+" or "-", math.abs(value), TypeText[id]))
		end
	end
end

local function AdjustWatchMachineResourceFrame()
	local id = this:GetClientUserData( 0 );
	local player = ActorMgr:FindActor( id );
	local monsterMachine = player:getMonsterMachine()
	if not monsterMachine then return end
	WatchMachineResourceFrame:SetPoint("topleft","UIClient","topleft",542,518);
	local t = {}
	for i = 1, 5 do
		local id = monsterMachine:getOreId(i)
		if id ~= 0 then
			local itemdef = getItemDef(id)
			if itemdef then
				table.insert(t, itemdef.Name)
			end
		end
	end
	local n = #t
	if n > 0 then
		local height = WatchMachineResourceFrameTex:GetHeight()
		local h1 = height / OreTypeWatchNum * n
		local h2 = WatchMachineResourceFrame:GetHeight() - height + h1
		WatchMachineResourceFrame:SetHeight(h2)
		WatchMachineResourceFrameTex:SetHeight(h1)
		for i = 1, n do
			local item = getglobal("WatchMachineResourceFrameItem"..i)
			if not item:IsShown() then
				item:Show()
			end
			local text = getglobal("WatchMachineResourceFrameItem"..i.."Title")
			text:SetText(t[i])
		end
		for i = n + 1, OreTypeWatchNum do
			local item = getglobal("WatchMachineResourceFrameItem"..i)
			item:Hide()
		end
		OreTypeWatchNum = n
	else
		WatchMachineResourceFrame:Hide()
	end
end

function UpdateWatchMachineResourceFrame()
	if not WatchMachineResourceFrame:IsShown() then
		return
	end
	local id = WatchMachineResourceFrame:GetClientUserData( 0 );
	local player = ActorMgr:FindActor( id );
	if player == nil then
		WatchMachineResourceFrame:Hide();
		return 
	end;
	local monsterMachine = player:getMonsterMachine()	
	if not monsterMachine then return end
	local ui
	local maxOre = monsterMachine:getMaxOre()
	local hasOre = 0
	for i = 1, OreTypeWatchNum do
		local ore = monsterMachine:getOre(i)
		hasOre = hasOre + ore
		ui = getglobal("WatchMachineResourceFrameItem"..i.."Text")
		ui:SetText(ore)
		ui = getglobal("WatchMachineResourceFrameItem"..i.."Slip")
		ui:SetSize(math.floor(120 * (ore / maxOre) + 0.5), ui:GetHeight())
	end
	WatchMachineResourceFrameTotalText:SetText(string.format("%d/%d", hasOre, maxOre))
	WatchMachineResourceFrameTotalSlip:SetSize(math.floor(120 * (hasOre / maxOre) + 0.5),
			WatchMachineResourceFrameTotalSlip:GetHeight())
end

function WatchMachineResourceFrame_OnShow()
	AdjustWatchMachineResourceFrame();
	UpdateWatchMachineResourceFrame();
end

function ReciveMachineInvite( name,id )
	MessageBox("邀请乘骑",GetPlayerName(name) .. "邀请您共同乘骑作战");
	MessageBoxFrame:SetClientUserData(0,id);
end

local MAX_EXPAND_ITEM_NUM = 4

local function SetClanShortcutExpandItem(parent, id)
	local player = ActorMgr:getMainPlayer()
	if not player then
		return
	end
	local name = parent:GetClientString()
	local frame = getglobal(name)
	if not frame then
		return
	end
	local IconPath
	local lv
	if id == 0 then
		IconPath = "uires\\ui\\mask\\TouMingTongDao.tga"
		lv = 0
	else
		local itemdef = getItemDef(id)
		lv = itemdef.WearPre.RoleLevelMin
		IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga"
		if not IsInExistence(IconPath) then
			IconPath = DEFAULT_ITEM_PATH
		end
	end
	frame:SetClientUserData(1, id)
	frame:SetClientUserData(2, lv)
	local icon = getglobal(name.."Icon")
	icon:SetTexture(IconPath)
	local text = getglobal(name.."Text")
	local container = player:getContainer()
	GameShortCut:setTmpShortCut(id, 0, frame:GetClientID() - 1, 0, 2)
end

local function UpdateClanShortcutItemExpand(frame)
	local result = frame:GetClientUserData(0)
	local player = ActorMgr:getMainPlayer()
	if not player then
		return
	end
	local container = player:getContainer()
	local level = player:getLv()
	local items = {}
	for i = 1, container:getContainerMaxNum(CONTAINER_TYPE_SUNDRIES) do
		local item		= container:getItem(CONTAINER_TYPE_SUNDRIES, i - 1)
		local itemdef	= item:getItemDef()
		if itemdef and level >= itemdef.WearPre.RoleLevelMin then
			for i = 0, MAX_ITEM_RESULT - 1 do
				if itemdef.Result[i].ResultID == result then
					local v1 = frame:GetClientUserData(1)
					local v2 = frame:GetClientUserData(2)
					local v3 = frame:GetClientUserData(3)
					v1 = (v1 == 0 or v1 == itemdef.Result[i].ResultVal1)
					v2 = (v2 == 0 or v2 == itemdef.Result[i].ResultVal2)
					v3 = (v3 == 0 or v3 == itemdef.Result[i].ResultVal3)
					if v1 and v2 and v3 then
						table.insert(items, {
							id = itemdef.ItemID, level = itemdef.WearPre.RoleLevelMin,
						})
					end
					break
				end
			end
		end
	end
	table.sort(items, function(a, b)
		return a.level < b.level
	end)
	if #items > MAX_EXPAND_ITEM_NUM then
		local t = {}
		for i = 1 + #items - MAX_EXPAND_ITEM_NUM, #items do
			table.insert(t, items[i])
		end
		items = t
	end
	local prefix = frame:GetName()
	for i = 1, MAX_EXPAND_ITEM_NUM do
		local name = string.format("%sItem%d", prefix, i)
		local btn = getglobal(name)
		if i > #items then
			btn:SetClientUserData(0, 0)
			local tex = getglobal(name.."Icon")
			tex:Hide()
		else
			local id = items[i].id
			btn:SetClientUserData(0, id)
			local itemdef = getItemDef(id)
			local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga"
			if not IsInExistence(IconPath) then
				IconPath = DEFAULT_ITEM_PATH
			end
			local tex = getglobal(name.."Icon")
			tex:Show()
			tex:SetTexture(IconPath)
		end
	end
end

function ClanShortcutItemExpand_OnClick()
	local name = this:GetClientString()
	local frame = getglobal(name)
	if not frame then
		return
	end
	if frame:IsShown() then
		frame:Hide()
	else
		frame:Show()
	end
end

function ClanShortcutItemExpand_OnLoad()
	this:RegisterEvent("GE_CONTAINER_UPDATE")
	this:RegisterEvent("GE_CLAN_BINDMACHINE")
end

function ClanShortcutItemExpand_OnEvent()
	if arg1 == "GE_CONTAINER_UPDATE" then
		if ClanCollectTmpFrame:IsShown() or ClanTankTmpFrame:IsShown() then
			UpdateClanShortcutItemExpand(this)
		end
	elseif arg1 == "GE_CLAN_BINDMACHINE" then
		UpdateClanShortcutItemExpand(this)
		SetClanShortcutExpandItem(this, 0)
		local prefix = this:GetName()
		for i = 4, 1, -1 do
			local name = string.format("%sItem%d", prefix, i)
			local btn = getglobal(name)
			if btn then
				local id = btn:GetClientUserData(0)
				if id ~= 0 then
					SetClanShortcutExpandItem(this, id)
					break
				end
			end
		end
	end
end

function ClanShortcutItemExpandItem_OnClick()
	local id = this:GetClientUserData(0)
	if id == 0 then
		return
	end
	local parent = this:GetParent()
	parent = getglobal(parent)
	parent:Hide()
	SetClanShortcutExpandItem(parent, id)
end

function ClanShortcutItemExpandItem_OnEnter()
	local id = this:GetClientUserData(0)
	if id == 0 then
		return
	end
	ShowItemTips(LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, getItemDef(id))
end

function ClanShortcutItemExpandItem_OnLeave()
	NormalItemTipsFrame:Hide()
end