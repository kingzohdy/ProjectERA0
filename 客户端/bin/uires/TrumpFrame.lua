local nCurTrumpSelect;
local MAX_FAIRY_LIST;
local MAX_FAIRY_SKILL;
local nBeginRefingTime;
local nRefingLastTime;
local nCurRefingTime;
local nTotalRefingTime;	  		
local ModelAngle = 0;

local t_TrumpIdx = {};
function InitTrumpFrame()
	nCurTrumpSelect		= 1;  
	MAX_FAIRY_LIST		= 8;
	MAX_FAIRY_SKILL		= 3;
	

	nBeginRefingTime	= 0;
	nRefingLastTime		= 0;
	nCurRefingTime		= -1;
	nTotalRefingTime	= 0;
	ModelAngle			= 0;
	
	for i = 1, MAX_FAIRY_SKILL do
		local TrumpSkillBtn = getglobal( "TrumpSkillBtn" .. i );
		TrumpSkillBtn:SetClientUserData( 0, 0 );   
		TrumpSkillBtn:SetClientUserData( 1, 0 );   
		TrumpSkillBtn:SetClientUserData( 2, 0 );
	end

	t_TrumpIdx = {};
end

function TrumpFrame_OnLoad()
	InitTrumpFrame();
	this:RegisterEvent("GE_TRUMP_UPDATE"); 
	this:RegisterEvent("GE_CONTAINER_UPDATE");
	this:RegisterEvent("GE_TRUMP_ADD");
	this:RegisterEvent("GE_ENTER_PWORLD");
	this:RegisterEvent("GE_TRUMP_ATTRUPDATE");
	this:RegisterEvent("GE_PET_STATE_CHANGE");
	this:RegisterEvent("GE_CHANGE_MONEY");
	this:RegisterEvent("UI_ACTOR_LEVEL_CHG");
	this:RegisterEvent("UI_ACTOR_EXP_CHG");
	this:RegisterEvent( "GE_TRUMP_STARUP" );
	for i=1,12 do
		local tex = getglobal("TrumpFrameFairyModelBtnLvTex"..i);
		tex:setRotateTopLeft( true );
		tex:SetAngle( -90+(i-1)*30 );
	end
	
	this:setUpdateTime( 0 );
end

function SetTrumpIdx( GameTrump )
	t_TrumpIdx = {};
	local bFound = false;
	for i=1,GameTrump:getTrumpMaxNum() do
		local RoleTrump = GameTrump:getRoleFairy( i - 1 );
		if RoleTrump.FairyID > 0 then
			table.insert( t_TrumpIdx,i );
			if nCurTrumpSelect == table.getn( t_TrumpIdx ) then
				bFound = true;
			end
		end
	end
	if not bFound then
		UpdateSelectTrump();
	end	
end

function TrumpFrame_OnEvent()
	local MainPlayer	= ActorMgr:getMainPlayer();
	local GameTrump		= MainPlayer:getTrump();
	if arg1 == "GE_TRUMP_ATTRUPDATE" then
		SetTrumpIdx( GameTrump );
		BondingTrumpList();
		if nCurTrumpSelect <= 0 then 
			return;
		end
		local RoleTrump		= GameTrump:getRoleFairy( t_TrumpIdx[nCurTrumpSelect] - 1 );
		local TrumpDef		= GameTrump:getFairyDef( RoleTrump.FairyID, RoleTrump.FairyLevel );
		if GameTrump:isFairyInState( RoleTrump.Stat, FAIRY_STAT_EVOLVE ) then 
			nTotalRefingTime	= TrumpDef.EvolveTime*60;
			nCurRefingTime	= RoleTrump.EvolveLeftTM;
		end		

		SetRefingLastTime();
		if TrumpStrengthFrame:IsShown() then
			TrumpStrengthFrame_OnShow();
		end
	end

	if arg1 == "GE_TRUMP_UPDATE" then
		SetTrumpIdx( GameTrump );
		if TrumpFrame:IsShown() then
			BondingTrumpList();
		end
		SetRefingLastTime();
		if TrumpLevelUpFrame:IsShown() then
			TrumpLevelUpFrame_OnShow();
		end
		if TrumpFrame:IsShown() then
			TrumpBindKeyInData();
		end
	end

	if arg1 == "GE_CONTAINER_UPDATE" then
		if TrumpFrame:IsShown() then
			TrumpBindKeyInData();
		end
		if TrumpStrengthFrame:IsShown() then
			TrumpStrengthFrame_OnShow();
		end
	end

	if arg1 == "GE_TRUMP_ADD" then
		if not TrumpFrame:IsShown() then
			TrumpFrame:Show();
		end
		if TrumpTexListBtn1:IsShown() then
			SetTrumpIdx( GameTrump );
			--显示右边的列表
			for i=1,table.getn( t_TrumpIdx ) do
				local btn = getglobal( "TrumpTexListBtn"..i );
				local tex = getglobal( "TrumpTexListBtn"..i.."Icon");
				--设置图标
				if i == nCurTrumpSelect then
					btn:Checked();
				end		
				local roleTrump = GameTrump:getRoleFairy( t_TrumpIdx[i] - 1 );
				local TrumpDef	= GameTrump:getFairyDef( roleTrump.FairyID, roleTrump.FairyLevel );
				local IconPath = GetItemEquipIconPath()..TrumpDef.ModeID..".tga";
				if not IsInExistence( IconPath ) then
					IconPath = DEFAULT_ITEM_PATH;
				end
				tex:SetTexture( IconPath );
				btn:Show();
			end
		end
	end

	if arg1 == "GE_ENTER_PWORLD" then
		if MapShower:getRealMapId() >= MIN_COPY_ID then
			local MainPlayer = ActorMgr:getMainPlayer();
			local GameTrump = MainPlayer:getTrump();
		--[[
			if not GameTrump:canCampInPworld( MapShower:getRealMapId() ) and GameTrump:isAnyFairyCamp() then
				AddGameSysTip( "本场景禁止使用精灵，精灵暂时收回" );
				SetSystemMessage( "本场景禁止使用精灵，精灵暂时收回" );
			elseif not GameTrump:canCampInPworld( MapShower:getRealMapId() ) then
				AddGameSysTip( "本场景禁止使用精灵" );
				SetSystemMessage( "本场景禁止使用精灵" );
			end
		]]--
		end
	end

	if arg1 == "GE_PET_STATE_CHANGE" then
		if TrumpFrame:IsShown() then
			local MainPlayer = ActorMgr:getMainPlayer();
			local GameTrump = MainPlayer:getTrump();
			local RoleTrump = GameTrump:getRoleFairy( t_TrumpIdx[nCurTrumpSelect] - 1 );
			TrumpBindFightPet( RoleTrump );
		end
	end

	if arg1 == "GE_CHANGE_MONEY" then
		if TrumpLevelUpFrame:IsShown() then
			TrumpLevelUpFrame_OnShow();
		end
		if TrumpStrengthFrame:IsShown() then
			TrumpStrengthFrame_OnShow();
		end
	end

	if arg1 == "UI_ACTOR_LEVEL_CHG" then
		if TrumpLevelUpFrame:IsShown() then
			TrumpLevelUpFrame_OnShow();
		end
	end

	if arg1 == "UI_ACTOR_EXP_CHG" then
		if TrumpLevelUpFrame:IsShown() then
			TrumpLevelUpFrame_OnShow();
		end
	end

	if arg1 == "GE_TRUMP_STARUP" then
		if TrumpStrengthFrame:IsShown() then
			ShowTrumpStrengthSuccessEffect();
		end
	end
end

function TrumpFrame_OnShow()
	local bFound = false;
	local MainPlayer = ActorMgr:getMainPlayer();
	local GameTrump	= MainPlayer:getTrump();
	local nTrumpNum = GameTrump:getTrumpMaxNum();
	for i = 1, nTrumpNum do
		local RoleTrump = GameTrump:getRoleFairy( i - 1 );
		if RoleTrump.FairyID > 0 then
			bFound = true;
		end
	end
	if not bFound then
		this:Hide();
		return;
	end

	-- 重置窗口坐标 
	TrumpFrame:SetPoint("center", "$parent", "center", 0, 0 );
	ModelAngle = 0;
	UpdateSelectTrump();
	BondingTrumpList();

	for i = 1, MAX_FAIRY_LIST do
		local FairyListBtn = getglobal( "TrumpTexListBtn" .. i );
		FairyListBtn:Hide();
	end
	TrumpFrameChangeFariyBtn:ChangeNormalTexture( 380,507,29,21 );
	TrumpFrameChangeFariyBtn:ChangeHighlightTexture( 410,507,29,21 );
	TrumpFrameChangeFariyBtn:ChangePushedTexture( 331,430,29,21 );	
end

function TrumpFrame_OnHide()
	TrumpLevelUpFrame:Hide();
	TrumpStrengthFrame:Hide();
	TrumpKeyInTipsFrame:Hide();
	TrumpFrameEffectFrame:Hide();
end

function UpdateSelectTrump()
	nCurTrumpSelect = -1;
	local MainPlayer	= ActorMgr:getMainPlayer();
	local GameTrump		= MainPlayer:getTrump();
	for i=1,table.getn( t_TrumpIdx ) do
		local RoleTrump = GameTrump:getRoleFairy( t_TrumpIdx[i] - 1 );
		if RoleTrump.FairyID > 0 and GameTrump:isFairyInState( RoleTrump.Stat, FAIRY_STAT_CAMP ) then
			nCurTrumpSelect = i;
			return;
		end
	end

	for i=1,table.getn( t_TrumpIdx ) do
		local RoleTrump = GameTrump:getRoleFairy( t_TrumpIdx[i] - 1 );
		if RoleTrump.FairyID > 0 then
			nCurTrumpSelect = i;
			return;
		end
	end
end

function BondingTrumpList()
	if nCurTrumpSelect <= 0 then
		return
	end
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end
	local GameTrump	= MainPlayer:getTrump();

	-- 显示当前出征精灵
	local bRefining = false;
	local RoleTrump = GameTrump:getRoleFairy( t_TrumpIdx[nCurTrumpSelect] - 1 );
	local TrumpDef	= GameTrump:getFairyDef( RoleTrump.FairyID, RoleTrump.FairyLevel );

	BondingSelectTrump( TrumpDef, RoleTrump );
	if GameTrump:isFairyInState( RoleTrump.Stat, FAIRY_STAT_EVOLVE ) then  
		if nCurRefingTime == -1 and nTotalRefingTime == 0 then 			
			-- 设置倒计时变量
			nBeginRefingTime	= os.clock();
			nCurRefingTime		= RoleTrump.EvolveLeftTM;
			nTotalRefingTime	= TrumpDef.EvolveTime*60;
		end
		bRefining = true;
	end

	SetRefingLastTime();

	if not bRefining then
		nCurRefingTime = -1;
		nRefingLastTime = 0;
		nTotalRefingTime = 0;
		TrumpFrameLastTimeBackTex:SetWidth(0);
	end
end

function TrumpTextrueBtn_OnClick()
	this:DisHighlight();
	if this:Checked() then return end
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil  then return end
	nCurTrumpSelect = this:GetClientID(); 
	BondingTrumpList();
	--HideTrumpListBtn();
	if TrumpLevelUpFrame:IsShown() then
		TrumpLevelUpFrame_OnShow();
	end
	if TrumpStrengthFrame:IsShown() then
		TrumpStrengthFrame_OnShow();
	end
end

local bAlphaAdd = false;
function TrumpFrame_OnUpdate()
	local tex = getglobal( "TrumpFrameFairyModelBtnModelEffectTex" );
	local alpha = tex:GetBlendAlpha();
	if bAlphaAdd then
		alpha = alpha + 0.01;		
		if alpha > 1 then
			alpha = 1;
			bAlphaAdd = false;
		end
		tex:SetBlendAlpha( alpha );
		TrumpFrameKeyInBtnEffectTex:SetBlendAlpha( alpha );
	else
		alpha = alpha - 0.01;		
		if alpha < 0.3 then
			alpha = 0.3
			bAlphaAdd = true;
		end
		tex:SetBlendAlpha( alpha );
		TrumpFrameKeyInBtnEffectTex:SetBlendAlpha( alpha );
	end
end

-- 显示选中的精灵的技能
function BondingTrumpSkillData( TrumpDef, RoleTrump )
	local nSkillIndex = 1;
	for i=1, FAIRY_MAX_SKILL do
		if nSkillIndex > MAX_FAIRY_SKILL then
			break;
		end
		if TrumpDef.FairySkillList[i-1].SkillID ~= 0 then
			local SkillBtn		= getglobal( "TrumpSkillBtn" .. nSkillIndex );

			local SkillDef	= nil;
			if TrumpDef.FairySkillList[i-1].SkillType == SKILL_TYPE then
				SkillDef = SkillMgr:getSpellStaticAtt( TrumpDef.FairySkillList[i-1].SkillID, TrumpDef.FairySkillList[i-1].SkillLevel );
			else
				SkillDef = SkillMgr:getDiathesisDef( TrumpDef.FairySkillList[i-1].SkillID, TrumpDef.FairySkillList[i-1].SkillLevel );
			end
			if SkillDef ~= nil then
				local SkillBtnTex	= getglobal( "TrumpSkillBtn" .. nSkillIndex .. "Icon" );
				SkillBtn:Enable(); 
				SkillBtn:SetClientUserData( 0, TrumpDef.FairySkillList[i-1].SkillID );	
				SkillBtn:SetClientUserData( 1, TrumpDef.FairySkillList[i-1].SkillLevel ); 
				SkillBtn:SetClientUserData( 2, TrumpDef.FairySkillList[i-1].SkillType );
				local SkillIcon = 0;
				local IconPath = "";
				if TrumpDef.FairySkillList[i-1].SkillType == SKILL_TYPE then
					IconPath = GetSkillIconPath()..SkillDef.IconID1..".tga"; 
				elseif TrumpDef.FairySkillList[i-1].SkillType == DIATHESIS_TYPE then
					IconPath = getDiathesisPath()..SkillDef.IconID..".tga"; 
				end				
				-- 如果没找到物品的图标则使用默认图标显示
				if not IsInExistence( IconPath ) then
					IconPath = DEFAULT_ITEM_PATH;
				end
				SkillBtnTex:SetTexture( IconPath );

				nSkillIndex = nSkillIndex + 1;
			end
		end
	end	
	local nStaffIndex	= 1;
	local bCanUse = true;
	
	for i = nSkillIndex, MAX_FAIRY_SKILL do				   
		local SkillBtn		= getglobal( "TrumpSkillBtn" .. i );  	
		local SkillBtnIcon	= getglobal( "TrumpSkillBtn" .. i .. "Icon" );
		SkillBtnIcon:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
		SkillBtn:SetClientUserData( 0, 0 );		   
		SkillBtn:SetClientUserData( 1, 0 );		  
		SkillBtn:SetClientUserData( 2, 0 );	  
		SkillBtn:Disable();
	end
end

function BondTrumpLevelTexture( nLevel )
	for i=1,nLevel do
		local tex = getglobal( "TrumpFrameFairyModelBtnLvTex"..i );
		tex:Show();
	end
	for i=nLevel+1,12 do
		local tex = getglobal( "TrumpFrameFairyModelBtnLvTex"..i );
		tex:Hide();
	end
	if nLevel == 12 then
		TrumpFrameFairyModelBtnModelEffectTex:Show();
	else
		TrumpFrameFairyModelBtnModelEffectTex:Hide();
	end
end

local t_trumpXinShouInfo = 
{ 
-- 召唤
["TrumpFrame_CampaignBtn"] = { size = { width = 104, height = 64 }, 
							offset = { selfPoint = "topleft", relFrame = "TrumpFrame_CampaignBtn", relPoint = "topleft", x = -7, y = -13 }, }, 
}

t_AttrTypeName = {[1] = "风",[2] = "电",[3] = "光",[4] = "暗",[5] = "土"};

local t_TrumpIDStatus = 
{
	[1]		= {"吸收伤害：","%d","变身精灵获得特殊能力“雷电护盾”\n可最多吸收一定量的伤害","ResultVal2"},	--雷神
	[2]		= {"减免伤害：","%d","变身精灵获得特殊能力“苍岩之甲”\n可减免受到的伤害","ResultVal2",math.abs},		--山神
	[1100]	= {"","%d","","ResultVal1"},	--瘟神
	[1200]	= {"闪避等级：","+%d%%","变身精灵获得特殊能力“阴影闪避”\n可提高自身的闪避等级","ResultVal1" },	-- 影子
	[1300]	= {"移动速度：","+%d%%","变身精灵获得特殊能力“高速移动”\n可提高自身的移动速度","ResultVal1"},	-- 风神
	[1400]	= {"恢复生命：","每秒%d点","变身精灵获得特殊能力“自我痊愈”\n可每秒恢复自身生命值","ResultVal1"},	-- 天使
	[3000]	= {"移动速度：","+%d%%","变身精灵获得特殊能力“高速移动”\n可提高自身的移动速度","ResultVal1"},	-- 风神
	[3101]	= {"恢复生命：","每秒%d点","变身精灵获得特殊能力“自我痊愈”\n可每秒恢复自身生命值","ResultVal1"},	-- 天使
	[3201]	= {"","%d","","ResultVal1"}	--瘟神
}

local szLastTrumpModelPath;
function BondingSelectTrump( TrumpDef, RoleTrump )	
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	local GameTrump = MainPlayer:getTrump();
	local MainPlayer = ActorMgr:getMainPlayer();
	local Container = MainPlayer:getContainer();
	if TrumpDef.FairyID == 0 then
		return;
	end
	--获得变身技能
	local SkillDef = nil;
	for i=1, FAIRY_MAX_SKILL do
		if TrumpDef.FairySkillList[i-1].SkillID ~= 0 then
			if TrumpDef.FairySkillList[i-1].SkillType == SKILL_TYPE then
				SkillDef = SkillMgr:getSpellStaticAtt( TrumpDef.FairySkillList[i-1].SkillID, TrumpDef.FairySkillList[i-1].SkillLevel );			
			end
			if SkillDef ~= nil and SkillDef.WushuangFlag > 0 then
				break;
			end
		end
	end
	if SkillDef == nil then return end
	--需要处理没有精灵的情况
	TrumpFrameNameFont:SetText( TrumpDef.Name );
	local t_TrumpModelScale = {	
								[1] =	{0.40,-20,-15},		--雷神
								[2] =	{0.24,20,-2},		--山神
								[1100] = {0.3,-20,0},		--瘟神
								[1200] = {0.60,15,0},		--影子
								[1300] = {0.19,20,0},		--风神
								[1400] = {0.45,25,0},		--天使
								[3000] = {0.19,10,0},		--风神
								[3101] = {0.4,40,0},		--天使
								[3201] = {0.3,-20,0},		--瘟神
							 };
	--精灵模型
	local fairyView = getglobal( "TrumpFrameFairyModelBtnFairyModel" );
	local skillResult = nil
	for i=1,8 do
		if SkillDef.SkillResult[i-1].SkillResultID == RESULT_SELF_STATUS then
			skillResult = SkillDef.SkillResult[i-1]
			break
		end
	end
	if skillResult ~= nil then
		local statusDef = getStatusDef( skillResult.SkillResultVal2, skillResult.SkillResultVal3 );
		if statusDef ~= nil and statusDef.ModeID ~= 0 then
			local modelPath = GetModePathById( statusDef.ModeID )
			if szLastTrumpModelPath ~= modelPath then
				local scale = 1;
				if GetNpcDisplayInfo( statusDef.ModeID ).size > 0 then
					scale = GetNpcDisplayInfo( statusDef.ModeID ).size;
				end
				fairyView:setPetModel( 0, 10100,modelPath );
				if t_TrumpModelScale[ RoleTrump.FairyID ] ~= nil then
					local tScale = t_TrumpModelScale[ RoleTrump.FairyID ];
					fairyView:setPetScale( 0,tScale[1]*scale,tScale[1]*scale,tScale[1]*scale );
					fairyView:setEntityPosition(0,tScale[3],tScale[2],0);
				else
					fairyView:setPetScale( 0,0.4*scale,0.4*scale,0.4*scale );
					fairyView:setEntityPosition(0,0,-40,0);
				end				
				szLastTrumpModelPath = modelPath ;
			end
			fairyView:setEntityRotate(0,ModelAngle);
			fairyView:Show();
		end
	end
	BondTrumpLevelTexture( RoleTrump.StarLvl );
	
	--出征的宠物
	TrumpBindFightPet( RoleTrump );

	TrumpFramePetInfoText:SetText( RoleTrump.StarLvl .."星强化" );
	TrumpFrameFairyInfoText:SetText( RoleTrump.FairyLevel .."级精灵" );
	
	-- 刻印图标
	TrumpBindKeyInData();

	--通灵后性格
	local TraitDef = GameTrump:getPetTraitDef( TrumpDef.TraitID,RoleTrump.StarLvl );
	if TraitDef ~= nil then
		TrumpFrameChartBtnValue:SetText( TraitDef.Name );
		TrumpFrameChartBtn:SetClientString( TraitDef.PetTraitDesc );
	else
		TrumpFrameChartBtnValue:SetText("");
		TrumpFrameChartBtn:SetClientString( "" );
	end

	local FairyStarDef = GameTrump:getFairyStarDef( RoleTrump.FairyID,RoleTrump.StarLvl );
	if FairyStarDef ~= nil then
		TrumpFrameAttrBtnValue:SetText( "+" .. FairyStarDef.PetExpMul .."%" );
		--伤害加成			
		TrumpFrameInstBtnValue:SetText( t_AttrTypeName[FairyStarDef.AttrType] .."伤害" .. "+" .. FairyStarDef.Atk );
	end

	BondingTrumpSkillData( TrumpDef, RoleTrump, "TrumpSkillBtn" );
	if GameTrump:isFairyInState( RoleTrump.Stat, FAIRY_STAT_CAMP ) then
		TrumpFrame_CampaignBtn:Hide();	
		TrumpFrame_InSupportBtn:Show();
	else
		TrumpFrame_CampaignBtn:Show();	
		TrumpFrame_InSupportBtn:Hide();
	end

	--xp 上限
	TrumpFrameMaxXPBtnValue:SetText( TrumpDef.XpUpperLimit );	
	-- 变身时间
	TrumpFrameTimeBtnValue:SetText( SkillDef.SkillResult[0].SkillResultVal4 .."秒" );
	
	--[[
	--获得魂能概率
	TrumpFrameXPRateBtnValue:SetText( TrumpDef.GetXpRate.."%" );
	]]--
	--改成特殊能力
	TrumpFrameXPRateBtnLabel:SetText( t_TrumpIDStatus[TrumpDef.FairyID][1] );	
	TrumpFrameXPRateBtnValue:SetText( string.format( t_TrumpIDStatus[TrumpDef.FairyID][2],GetSepcialVal( RoleTrump,TrumpDef ) )  )
	TrumpFrameAddSpeedBtn:SetGrayEx( true );
	TrumpFrameAddSpeedBtn:ChangeHighlightTexture( 575,933,35,13 );
	TrumpFrameAddSpeedBtn:ChangePushedTexture( 575,933,35,13 );
	for i=1,GameTrump:getTrumpMaxNum() do
		local RoleTrump = GameTrump:getRoleFairy( i - 1 );
		if RoleTrump.FairyID > 0 and GameTrump:isFairyInState( RoleTrump.Stat,FAIRY_STAT_EVOLVE ) then
			TrumpFrameAddSpeedBtn:SetGrayEx( false );
			TrumpFrameAddSpeedBtn:ChangeHighlightTexture( 611,933,35,13 );
			TrumpFrameAddSpeedBtn:ChangePushedTexture( 647,933,35,13 );
			return;
		end
	end

	if not GameWizard:isDo( GAMEWIZARD_FIRST_HAVE_JING_LING ) then
		GuideEffectShow( "TrumpFrame","","TrumpFrame_CampaignBtn", "鼠标左键点击“召唤”按钮，召唤精灵出征：\n1.可使用精灵技能\n2.可管制出征宠物", "uires\\ui\\mask\\TouMingTongDao.tga", 1, 1, 1, 1, 50, 60, -9, 5, 11, -51, false );
		local frameUVATexture = getglobal( "TrumpFrameEffectFrameUVAnimationTex" )
		frameUVATexture:SetSize( 140,120 )
		frameUVATexture:SetPoint("center", "TrumpFrame_CampaignBtn", "center", 5, 5);
		frameUVATexture:SetUVAnimation(50,true);	
		frameUVATexture:Show();
		GameWizard:doStep( GAMEWIZARD_FIRST_HAVE_JING_LING );
	end
	if GameWizard:isDo( GAMEWIZARD_FIST_TRUMP_QUEST_FAIRY_MORPH ) and not GameWizard:isDo( GAMEWIZARD_FIRST_CAMP_TRUMP ) then
		GuideEffectShow( "TrumpFrame","","TrumpFrameLvUpBtn", "鼠标左键点击该按钮，打开精灵升级面板", "uires\\ui\\mask\\TouMingTongDao.tga", 1, 1, 1, 1, 50, 60, -9, 5, 11, -51, false );
		local frameUVATexture = getglobal( "TrumpFrameEffectFrameUVAnimationTex" )
		frameUVATexture:SetSize( 74,74 )
		frameUVATexture:SetPoint("center", "TrumpFrameLvUpBtn", "center", 2, 2);
		frameUVATexture:SetUVAnimation(50,true);	
		frameUVATexture:Show();
		TrumpFrameEffectFrame:SetClientUserData( 0,GAMEWIZARD_FIRST_CAMP_TRUMP )
	end
	if RoleTrump.FairyLevel > 1 and not GameWizard:isDo( GAMEWIZARD_FIRST_LEVEL_UP_TRUMP ) then
		GuideEffectShow( "TrumpFrame","","TrumpFrameStrengthBtn", "鼠标左键点击该按钮，打开精灵强化面板", "uires\\ui\\mask\\TouMingTongDao.tga", 1, 1, 1, 1, 50, 60, -9, 5, 11, -51, false );
		local frameUVATexture = getglobal( "TrumpFrameEffectFrameUVAnimationTex" )
		frameUVATexture:SetSize( 74,74 )
		frameUVATexture:SetPoint("center", "TrumpFrameStrengthBtn", "center", 2, 2);
		frameUVATexture:SetUVAnimation(50,true);	
		frameUVATexture:Show();
		TrumpFrameEffectFrame:SetClientUserData( 0,GAMEWIZARD_FIRST_LEVEL_UP_TRUMP )
	end
end

function GetSepcialVal( RoleTrump,TrumpDef )
	local SkillDef = nil;
	for i=1, FAIRY_MAX_SKILL do
		if TrumpDef.FairySkillList[i-1].SkillID ~= 0 then
			if TrumpDef.FairySkillList[i-1].SkillType == SKILL_TYPE then
				SkillDef = SkillMgr:getSpellStaticAtt( TrumpDef.FairySkillList[i-1].SkillID, TrumpDef.FairySkillList[i-1].SkillLevel );			
			end
			if SkillDef ~= nil and SkillDef.WushuangFlag > 0 then
				break;
			end
		end
	end
	if SkillDef == nil then
		return 0;
	end
	local statusDef = getStatusDef( SkillDef.SkillResult[1].SkillResultVal2,SkillDef.SkillResult[1].SkillResultVal3 );
	if statusDef == nil then
		return 0;
	end
	if t_TrumpIDStatus[TrumpDef.FairyID][5] ~= nil then
		return t_TrumpIDStatus[TrumpDef.FairyID][5]( statusDef.Result[0][t_TrumpIDStatus[TrumpDef.FairyID][4]] );
	else
		return  statusDef.Result[0][t_TrumpIDStatus[TrumpDef.FairyID][4]];
	end
end

-- 显示出征宠物
function TrumpBindFightPet( RoleTrump )	
	TrumpFramePetBtnFont:SetText( "" );
	TrumpFramePetBtnIconTexture:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
	TrumpFramePetBtnBackTex:SetTexture( "uires\\ZhuJieMian\\2.dds" );
	TrumpFramePetBtnBackTex:SetTexUV( 554,585,50,50 );
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	local GameTrump = MainPlayer:getTrump();
	if not GameTrump:isFairyInState( RoleTrump.Stat, FAIRY_STAT_CAMP ) then
		return
	end
	for i = 1, petPackage:getPetNum() do
		if petPackage:isPetInState( i - 1, CLIENT_PET_STAT_FIGHT ) then
			local petInfo = petPackage:getPetInfo( i - 1 );
			local petDef = getPetDef( petInfo.PetID );
			if petDef == nil then
				return;
			end
			TrumpFramePetBtnFont:SetText( petInfo.Name );
			local iconPath = GetMonsterIconPath( petDef.IconID );
			TrumpFramePetBtnIconTexture:SetTexture( iconPath );
			TrumpFramePetBtnBackTex:SetTexture( "uires\\ZhuJieMian\\1.dds" );
			TrumpFramePetBtnBackTex:SetTexUV( 794,562,44,44 );
		end
	end
end

-- 显示刻印图标
function TrumpBindKeyInData()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	local GameTrump = MainPlayer:getTrump();
	local RoleTrump = GameTrump:getRoleFairy( t_TrumpIdx[nCurTrumpSelect] - 1 );
	local TrumpDef = GameTrump:getFairyDef( RoleTrump.FairyID, RoleTrump.FairyLevel );

	local FairyStarDef = GameTrump:getFairyStarDef( RoleTrump.FairyID,RoleTrump.StarLvl );	
	TrumpFrameKeyInBtnEffectTex:Hide();

	--是否可以刻印	
	if TrumpDef.PNum > 0 and TrumpDef.ZeroReelID > 0 then
		if not GameWizard:isDo( GAME_WIZARD_FIRST_TRUMP_KEYIN ) then
			GuideEffectShow( "TrumpFrame","","TrumpFrameKeyInBtn", "鼠标左键点击“刻印”按钮，可制作“元素印记”", "uires\\ui\\mask\\TouMingTongDao.tga", 1, 1, 1, 1, 50, 60, -9, 5, 11, -51, false );
			local frameUVATexture = getglobal( "TrumpFrameEffectFrameUVAnimationTex" )
			frameUVATexture:SetSize( 74,150 )
			frameUVATexture:SetPoint("center", "TrumpFrameKeyInBtn", "center", 2, 2);
			frameUVATexture:SetUVAnimation(50,true);	
			frameUVATexture:Show();
			TrumpFrameEffectFrame:SetClientUserData( 0,GAME_WIZARD_FIRST_TRUMP_KEYIN );
		end
		TrumpFrameKeyInBtnFont:SetText( t_AttrTypeName[FairyStarDef.AttrType] .. "元素刻印" );
		if RoleTrump.IsKeyin == 0 then
			TrumpFrameKeyInBtnEffectTex:Show();
		end
	else
		TrumpFrameKeyInBtnFont:SetText( "" );
	end
end

--是否出在当前出征精灵的变身状态
function IsCampTrumpInMorph()
	local mainplayer = ActorMgr:getMainPlayer();
	local GameTrump = mainplayer:getTrump();
	local RoleTrump = GameTrump:getCampRoleFairy();
	if RoleTrump == nil then
		return;
	end
	local TrumpDef = GameTrump:getFairyDef( RoleTrump.FairyID,RoleTrump.FairyLevel );
	local SkillDef = nil;
	for i=1, FAIRY_MAX_SKILL do
		if TrumpDef.FairySkillList[i-1].SkillID ~= 0 then
			if TrumpDef.FairySkillList[i-1].SkillType == SKILL_TYPE then
				SkillDef = SkillMgr:getSpellStaticAtt( TrumpDef.FairySkillList[i-1].SkillID, TrumpDef.FairySkillList[i-1].SkillLevel );			
			end
			if SkillDef ~= nil and SkillDef.WushuangFlag > 0 then
				break;
			end
		end
	end
	if SkillDef == nil then
		return;
	end
	if SkillDef.SkillResult[0].SkillResultID == RESULT_SELF_STATUS then
		local statusDef = getStatusDef( SkillDef.SkillResult[0].SkillResultVal2,SkillDef.SkillResult[0].SkillResultVal3 );
		if statusDef ~= nil and statusDef.ModeID == mainplayer:GetMorphResID() then
			return true;
		else
			return;
		end
	end
end

function TrumpCampaignBtn_OnClick() 
	-- 再判断当前是否处于吟唱状态
	if TempFairyShortCutFrame:IsShown() and IsCampTrumpInMorph() then
		ShowMidTips("已有精灵出征，请先召回");
		return;
	end
	
	local MainPlayer = ActorMgr:getMainPlayer();
	if GetTrumpSelectGridIdx() < 0 then 
		return;
	end
	local GameTrump = MainPlayer:getTrump();

	-- 判断是否能在副本出征精灵
	if not GameTrump:canCampInPworld( MapShower:getRealMapId() ) then
		ShowMidTips("此副本不允许召唤精灵");
		return;
	end

	if GameTrump:isPre() then
		ShowMidTips("精灵正在出征中")
		return;
	end

	SkillMgr:unUseSpell();

	local RoleTrump = GameTrump:getRoleFairy( GetTrumpSelectGridIdx() );
	GameTrump:trumpPre( RoleTrump.WID.nHeight, RoleTrump.WID.nLow, GetTrumpSelectGridIdx() );
	if TrumpFrameEffectFrame:IsShown() then
		TrumpFrameEffectFrame:Hide();
	end
end

function TrumpInSupportBtn_OnClick()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil or GetTrumpSelectGridIdx() < 0 then 
		return;
	end
	--[[
	if MainPlayer:isDead() then
		ShowMidTips( "死亡状态，无法召回精灵" );
		return;
	end
	if MainPlayer:isInFight() then
		ShowMidTips( "战斗状态，无法召回精灵" );
		return;
	end
	]]--
	if TempFairyShortCutFrame:IsShown() and IsCampTrumpInMorph() then
		ShowMidTips("当前处于精灵变身状态，不可收回");
		return;
	end

	local GameTrump = MainPlayer:getTrump();
	local RoleTrump = GameTrump:getRoleFairy( GetTrumpSelectGridIdx() );
	GameTrump:trumpBack( RoleTrump.WID.nHeight, RoleTrump.WID.nLow, GetTrumpSelectGridIdx() );
end

function GetTrumpSelectGridIdx()
	if table.getn( t_TrumpIdx ) > 0 and nCurTrumpSelect > 0 then
		return t_TrumpIdx[nCurTrumpSelect] - 1;
	end
	return -1;
end

function TrumpCampaign( nTime )
	IntonateBar_text:SetText("正在召唤精灵出征");
	IntonateBar:SetClientUserData( 0, TRUMP_TYPE_PRE );
	IntonateBar:SetClientUserData( 2, nTime );
	IntonateBar:Show();
end

function EndTrumpPre()
	if IntonateBar:GetClientUserData( 0 ) == TRUMP_TYPE_PRE then
		if IntonateBar:IsShown() then
			IntonateBar:Hide();
		end
	end
end

function TrumpSkillBtn_OnEnter()
	local skillId   = this:GetClientUserData(0);
	local skilllv	= this:GetClientUserData(1);  
	local skilltype	= this:GetClientUserData(2);
	local szName    = this:GetName();
	
	if skillId <= 0 or skillId == nil or szName == nil or szName == "" then return end
	
	SpellTipsFrame:SetClientUserData( 0, SkillMgr:getRuneSkillId(skillId) );	 
	SpellTipsFrame:SetClientUserData( 1, skilllv );	 
	SpellTipsFrame:SetClientUserData( 2, skilltype ); 
	SpellTipsFrame:SetClientUserData( 3, TRUMP_SKILL_TYPE );
	SpellTipsFrame:SetClientString(szName);
	SpellTipsFrame:Show();
end	
 
function TrumpSkillBtn_OnLeave()
	SpellTipsFrame:Hide();
end

function TrumpRefiningClockFrame_OnUpdate()
   SetRefingLastTime();
end

function SetRefingLastTime()
	if nCurRefingTime >=0 and os.difftime ( os.clock(), nRefingLastTime) >= 1 then
		nRefingLastTime = os.clock(); --重新计时
		nCurRefingTime = nCurRefingTime - 1;
		local MainPlayer = ActorMgr:getMainPlayer();
		local GameTrump = MainPlayer:getTrump();
		local RoleTrump = GameTrump:getRoleFairy( t_TrumpIdx[nCurTrumpSelect] - 1 );
		if RoleTrump ~= nil then
			RoleTrump.EvolveLeftTM = RoleTrump.EvolveLeftTM - 1;
		end
	end
	if nCurRefingTime == 0 then
		nCurRefingTime = -1;
		nRefingLastTime = 0;
		nTotalRefingTime = 0;
		TrumpFrameLastTimeBackTex:SetWidth(0);
		
	elseif nCurRefingTime > 0 and nCurRefingTime <= nTotalRefingTime then
		local scale = nCurRefingTime/nTotalRefingTime;
		if scale <= 0 then
			scale = 0;
		end
		if scale > 1 then
			scale = 1;
		end
		TrumpFrameLastTimeBackTex:SetWidth( scale*316 );
	end
end

function TrumpLevelUp( nFairyLv, nGridIdx )
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then 
		return;
	end
	local GameTrump = MainPlayer:getTrump();
	local RoleTrump = GameTrump:getRoleFairy( nGridIdx );
	if GameTrump:isFairyInState( RoleTrump.Stat, FAIRY_STAT_CAMP ) then
		SetFairySkill();
		SetXPSkillShow();
		UpdateMainPlayerBuff( MainPlayer:GetID(),RoleTrump.FairyID,RoleTrump.FairyLevel );
	end
	UpdateShortCutSkill();
	nCurRefingTime	= 1;	
end

local nLastPreTime = 0;
function TrumpFramePreFairyBtn_OnClick()
	if table.getn( t_TrumpIdx ) == 0 then
		return
	end
	if os.clock() - nLastPreTime > 0.3 then
		nLastPreTime = os.clock();
		if nCurTrumpSelect > 1 then
			nCurTrumpSelect = nCurTrumpSelect - 1;
		else
			nCurTrumpSelect = table.getn( t_TrumpIdx );
		end
		BondingTrumpList();		
		if TrumpLevelUpFrame:IsShown() then
			TrumpLevelUpFrame_OnShow();
		end
		if TrumpStrengthFrame:IsShown() then
			TrumpStrengthFrame_OnShow();
		end
	end
end

local nLastNextTime = 0;
function TrumpFrameNextFariyBtn_OnClick()
	if table.getn( t_TrumpIdx ) == 0 then
		return
	end
	if os.clock() - nLastNextTime > 0.3 then
		nLastNextTime = os.clock();
		if nCurTrumpSelect < table.getn( t_TrumpIdx ) then
			nCurTrumpSelect = nCurTrumpSelect + 1;
		else
			nCurTrumpSelect = 1;
		end
		BondingTrumpList();
		if TrumpLevelUpFrame:IsShown() then
			TrumpLevelUpFrame_OnShow();
		end
		if TrumpStrengthFrame:IsShown() then
			TrumpStrengthFrame_OnShow();
		end
	end
end

function HideTrumpListBtn()
	for i=1,MAX_FAIRY_LIST do
		local btn = getglobal("TrumpTexListBtn"..i );
		btn:DisChecked();
		btn:Hide();
	end
end

function TrumpFrameChangeFariyBtn_OnClick()
	if TrumpTexListBtn1:IsShown() then
		HideTrumpListBtn();
		this:ChangeNormalTexture( 380,507,29,21 );
		this:ChangeHighlightTexture( 410,507,29,21 );
		this:ChangePushedTexture( 331,430,29,21 );
	else
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer == nil then return end
		local gameTrump = mainplayer:getTrump();
		
		--显示右边的列表
		for i=1,table.getn( t_TrumpIdx ) do
			local btn = getglobal( "TrumpTexListBtn"..i );
			local tex = getglobal( "TrumpTexListBtn"..i.."Icon");
			--设置图标
			if i == nCurTrumpSelect then
				btn:Checked();
			end		
			local roleTrump = gameTrump:getRoleFairy( t_TrumpIdx[i] - 1 );
			local TrumpDef	= gameTrump:getFairyDef( roleTrump.FairyID, roleTrump.FairyLevel );
			local IconPath = GetItemEquipIconPath()..TrumpDef.ModeID..".tga";
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			tex:SetTexture( IconPath );
			btn:Show();
		end
		this:ChangeNormalTexture( 290,507,29,21 );
		this:ChangeHighlightTexture( 320,507,29,21 );
		this:ChangePushedTexture( 350,507,29,21 );
	end	
end

function TrumpFrameChangeFariyBtn_OnEnter()
	SetGameTooltips( this:GetName(),"展开/收起精灵列表" );
end

function TrumpFrameKeyInBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local GameTrump = mainplayer:getTrump();	
	local RoleTrump = GameTrump:getRoleFairy( t_TrumpIdx[nCurTrumpSelect] - 1 );
	local TrumpDef	= GameTrump:getFairyDef( RoleTrump.FairyID, RoleTrump.FairyLevel );
	if RoleTrump == nil or TrumpDef == nil then
		return;
	end
	if TrumpFrameEffectFrame:IsShown() and TrumpFrameEffectFrame:GetClientUserData(0) == GAME_WIZARD_FIRST_TRUMP_KEYIN then
		TrumpFrameEffectFrame:Hide();
		TrumpFrameEffectFrame:SetClientUserData( 0,0 )
		GameWizard:doStep( GAME_WIZARD_FIRST_TRUMP_KEYIN );
	end
	if TrumpDef.ZeroReelID == 0 or TrumpDef.PNum == 0 then
		ShowMidTips("精灵等级过低无法刻印，请先把精灵升级至7级");
		return;
	end
	if RoleTrump.IsKeyin == 1 then
		ShowMidTips(TrumpDef.Name .. "今天已刻印过一次，请明天再来刻印");
		return;
	end
	if not GameTrump:isFairyInState( RoleTrump.Stat, FAIRY_STAT_CAMP ) then
		ShowMidTips( "该精灵未出征，无法刻印" )
		return
	end
	local container = mainplayer:getContainer();
	if container:getItemCount( ZEROREELID_TOLUA ) < TrumpDef.ZeroReelID then
		local itemDef = getItemDef( ZEROREELID_TOLUA );
		if itemDef ~= nil then
			ShowMidTips( itemDef.Name .."不足，无法刻印");
		end
		return;
	end
	GameTrump:requestOpKeyIn( t_TrumpIdx[nCurTrumpSelect] - 1 );
end

local nLastMouseClickX = 0;
function TrumpFrameModelBtn_OnMouseDown()
if arg1 == "LeftButton" then
		nLastMouseClickX = GetCursorPosX();
	end
end

function TrumpFrameModelBtn_OnDragStart()
	local modelview = getglobal( "TrumpFrameFairyModelBtnFairyModel" );
	if not modelview:IsShown() then
		return;
	end	
	if this:GetPushedState() then
		local nCurMouseClickX = GetCursorPosX();
		ModelAngle = ModelAngle - (nCurMouseClickX - nLastMouseClickX) ;
		modelview:setEntityRotate(0, ModelAngle);
		nLastMouseClickX = nCurMouseClickX;
	end
end

function TrumpFrameLastTime_OnEnter()
	if nCurRefingTime > 0 and nCurRefingTime <= nTotalRefingTime then
		local szText = "";
		local nHour		= math.floor( nCurRefingTime / 3600 );
		local nMin		=  math.ceil( (nCurRefingTime-nHour*3600)/60 );
		if nHour == 0 and nMin == 0 then
			return;
		elseif nHour > 0 then
			szText = "精灵升级剩余时间："..nHour.."小时"
			if nMin > 0 then
				szText = szText ..nMin.."分钟";
			end			
		else
			szText ="精灵升级剩余时间："..nMin.."分钟";
		end
		SetGameTooltips( this:GetName(),szText );
	end
end

--打开强化面板
function TrumpFrameStrengthBtn_OnClick()
	if TrumpFrameEffectFrame:IsShown() and TrumpFrameEffectFrame:GetClientUserData(0) == GAMEWIZARD_FIRST_LEVEL_UP_TRUMP then
		if not GameWizard:isDo( GAMEWIZARD_FIRST_LEVEL_UP_TRUMP ) then
			GameWizard:doStep( GAMEWIZARD_FIRST_LEVEL_UP_TRUMP );
		end
		TrumpFrameEffectFrame:Hide();
	end
	if TrumpStrengthFrame:IsShown() then
		TrumpStrengthFrame:Hide();
	else
		TrumpStrengthFrame:Show();
		TrumpStrengthFrame:SetPoint( "topleft","UIClient","topleft",TrumpFrame:GetRealLeft()-TrumpStrengthFrame:GetWidth(),TrumpFrame:GetRealBottom()-TrumpStrengthFrame:GetHeight() )
	end
end

--打开升级面板
function TrumpFrameLvUpBtn_OnClick()
	if TrumpFrameEffectFrame:IsShown() and TrumpFrameEffectFrame:GetClientUserData(0) == GAMEWIZARD_FIRST_CAMP_TRUMP then
		if not GameWizard:isDo( GAMEWIZARD_FIRST_CAMP_TRUMP ) then
			GameWizard:doStep( GAMEWIZARD_FIRST_CAMP_TRUMP );
		end
		TrumpFrameEffectFrame:Hide();
	end
	if TrumpLevelUpFrame:IsShown() then
		TrumpLevelUpFrame:Hide();
	else
		
		TrumpLevelUpFrame:Show();
		TrumpLevelUpFrame:SetPoint( "topleft","UIClient","topleft",TrumpFrame:GetRealRight(),TrumpFrame:GetRealBottom()-TrumpLevelUpFrame:GetHeight() );
	end
end

function TrumpFrameAddSpeedBtn_OnClick()
--[[
	local mainplayer = ActorMgr:getMainPlayer();
	local container = mainplayer:getContainer();
	if container:getItemCount( DEC_EVOLVE_TIME_ITEMID_TOLUA ) > 0  then
		MessageBox("炼化加速","是否使用加速齿轮缩短炼化时间");
		MessageBoxFrame:SetClientUserData( 0,t_TrumpIdx[nCurTrumpSelect] - 1 );
	else
		ShowMidTips("没有加速齿轮");
	end
]]--
	ShowMidTips("暂无该功能");
end

--------------------------			升级精灵		--------------------------

function TrumpLevelUpFrame_OnShow()
	if nCurTrumpSelect <= 0 then
		return
	end
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end
	TrumpLevelUpFrameEffectFrame:Hide();
	TrumpLevelUpFrameLvUpBtn:Enable();
	local GameTrump	= MainPlayer:getTrump();
	local RoleTrump = GameTrump:getRoleFairy( t_TrumpIdx[nCurTrumpSelect] - 1 );
	local TrumpDef	= GameTrump:getFairyDef( RoleTrump.FairyID, RoleTrump.FairyLevel );
	local nextLvTrumpDef = GameTrump:getFairyDef( RoleTrump.FairyID,RoleTrump.FairyLevel + 1 );
	--说明是最高等级了
	if nextLvTrumpDef == nil then
		--ShowMidTips("精灵等级已到达最高级");
		--TrumpLevelUpFrame:Hide();		
		--return;
		nextLvTrumpDef = TrumpDef;
		TrumpDef =  GameTrump:getFairyDef( RoleTrump.FairyID, RoleTrump.FairyLevel - 1 );
	end
	if GameTrump:isFairyInState( RoleTrump.Stat, FAIRY_STAT_EVOLVE ) then
		local color = GetColor()[COLOR_WEAK_YELLOW];
		TrumpLevelUpFrameNeedLvBtnValue:SetText( "..升级中" );
		TrumpLevelUpFrameNeedLvBtnValue:SetTextColor( color[1],color[2],color[3] );
		TrumpLevelUpFrameNeedExpBtnValue:SetText( "..升级中" );
		TrumpLevelUpFrameNeedExpBtnValue:SetTextColor( color[1],color[2],color[3] );
		TrumpLevelUpFrameNeedMoneyBtnValue:SetText( "..升级中" );
		TrumpLevelUpFrameNeedMoneyBtnValue:SetTextColor( color[1],color[2],color[3] );
		TrumpLevelUpFrameNeedTimeBtnValue:SetText( "..升级中" );
		TrumpLevelUpFrameNeedTimeBtnValue:SetTextColor( color[1],color[2],color[3] );
	else
		TrumpLevelUpFrameNeedLvBtnValue:SetText( TrumpDef.EvolveLevelLimit .. "级" );
		local textColor = MainPlayer:getLv() >= TrumpDef.EvolveLevelLimit and GetColor()[COLOR_WEAK_YELLOW] or GetColor()[COLOR_RED];
		TrumpLevelUpFrameNeedLvBtnValue:SetTextColor( textColor[1],textColor[2],textColor[3] );
		TrumpLevelUpFrameNeedExpBtnValue:SetText( TrumpDef.UseExp );
		textColor = MainPlayer:getExp() >= TrumpDef.UseExp and GetColor()[COLOR_WEAK_YELLOW] or GetColor()[COLOR_RED];
		TrumpLevelUpFrameNeedExpBtnValue:SetTextColor( textColor[1],textColor[2],textColor[3] );
		TrumpLevelUpFrameNeedMoneyBtnValue:SetText( TrumpDef.UseMoney );
		textColor = getPlayerMoney( MainPlayer ) >= TrumpDef.UseMoney and GetColor()[COLOR_WEAK_YELLOW] or GetColor()[COLOR_RED];
		TrumpLevelUpFrameNeedMoneyBtnValue:SetTextColor( textColor[1],textColor[2],textColor[3] );
		local szText = "";
		local nHour		= math.floor( TrumpDef.EvolveTime / 60 );
		local nMin		=  math.ceil( (TrumpDef.EvolveTime-nHour*60) );
		if nHour == 0 and nMin == 0 then
			return;
		elseif nHour > 0 then
			szText = nHour.."小时";
			if nMin > 0 then
				szText = szText .. nMin.."分钟"
			end
		else
			szText =nMin.."分钟";
		end
		TrumpLevelUpFrameNeedTimeBtnValue:SetText( szText );
		textColor = GetColor()[COLOR_WEAK_YELLOW];
		TrumpLevelUpFrameNeedTimeBtnValue:SetTextColor( textColor[1],textColor[2],textColor[3] );
	end
	

	TrumpLevelUpFrameTrumpLvBtnValue:SetText( nextLvTrumpDef.Level .. "级" );
	TrumpLevelUpFrameMaxXpBtnValue:SetText( nextLvTrumpDef.XpUpperLimit );
	-- 变身时间
	for i=1, FAIRY_MAX_SKILL do
		if nextLvTrumpDef.FairySkillList[i-1].SkillID ~= 0 then
			local NextSkillDef	= nil;
			if nextLvTrumpDef.FairySkillList[i-1].SkillType == SKILL_TYPE then
				NextSkillDef = SkillMgr:getSpellStaticAtt( nextLvTrumpDef.FairySkillList[i-1].SkillID, nextLvTrumpDef.FairySkillList[i-1].SkillLevel );				
			end
			if NextSkillDef ~= nil and NextSkillDef.WushuangFlag > 0 then
				TrumpLevelUpFrameTimeBtnValue:SetText( NextSkillDef.SkillResult[0].SkillResultVal4 .."秒" );
			end
		end
	end
	--[[
	TrumpLevelUpFrameXpRateBtnValue:SetText( nextLvTrumpDef.GetXpRate .."%几率" );
	]]--
	TrumpLevelUpFrameXpRateBtnLabel:SetText( t_TrumpIDStatus[nextLvTrumpDef.FairyID][1] );	
	TrumpLevelUpFrameXpRateBtnValue:SetText( string.format( t_TrumpIDStatus[nextLvTrumpDef.FairyID][2],GetSepcialVal( RoleTrump,nextLvTrumpDef ) )  )

	BondingTrumpLevelUpSkillData( TrumpDef,nextLvTrumpDef, RoleTrump );
	if GameTrump:isFairyInState( RoleTrump.Stat, FAIRY_STAT_EVOLVE ) then
		TrumpLevelUpFrameLvUpBtn:Disable();
	end

	if GameWizard:isDo( GAMEWIZARD_FIRST_CAMP_TRUMP ) and not GameWizard:isDo( GAMEWIZARD_FIRST_TRUMP_LEVEL_UP ) then
		GuideEffectShow( "TrumpLevelUpFrame","","TrumpLevelUpFrameLvUpBtn", "鼠标左键点击“升级”按钮，开始升级精灵", "uires\\ui\\mask\\TouMingTongDao.tga", 1, 1, 1, 1, 50, 60, -9, 5, 11, -51, false );
		local frameUVATexture = getglobal( "TrumpLevelUpFrameEffectFrameUVAnimationTex" )
		frameUVATexture:SetSize( 140,120 )
		frameUVATexture:SetPoint("center", "TrumpLevelUpFrameLvUpBtn", "center", 5, 5);
		frameUVATexture:SetUVAnimation(50,true);	
		frameUVATexture:Show();
		TrumpFrameEffectFrame:SetClientUserData( 0,GAMEWIZARD_FIRST_CAMP_TRUMP )
	end
end

function BondingTrumpLevelUpSkillData( TrumpDef,nextLvTrumpDef, RoleTrump )
	local nSkillIndex = 1;
	for i=1, FAIRY_MAX_SKILL do
		if nSkillIndex > MAX_FAIRY_SKILL then
			break;
		end
		if nextLvTrumpDef.FairySkillList[i-1].SkillID ~= 0 then
			local SkillBtn		= getglobal( "TrumpLevelUpSkillBtn" .. nSkillIndex );
			if TrumpDef.FairySkillList[i-1].SkillID == 0 or nextLvTrumpDef.FairySkillList[i-1].SkillLevel > TrumpDef.FairySkillList[i-1].SkillLevel then
				local SkillDef	= nil;
				if nextLvTrumpDef.FairySkillList[i-1].SkillType == SKILL_TYPE then
					SkillDef = SkillMgr:getSpellStaticAtt( nextLvTrumpDef.FairySkillList[i-1].SkillID, nextLvTrumpDef.FairySkillList[i-1].SkillLevel );
				else
					SkillDef = SkillMgr:getDiathesisDef( nextLvTrumpDef.FairySkillList[i-1].SkillID, nextLvTrumpDef.FairySkillList[i-1].SkillLevel );
				end
				if SkillDef ~= nil then
					local SkillBtnTex	= getglobal( "TrumpLevelUpSkillBtn" .. nSkillIndex .. "Icon" );
					local SkillBtnUpTex = getglobal( "TrumpLevelUpSkillBtn" .. nSkillIndex .. "UpTex" );
					SkillBtn:Enable(); 
					SkillBtn:SetClientUserData( 0, nextLvTrumpDef.FairySkillList[i-1].SkillID );	
					SkillBtn:SetClientUserData( 1, nextLvTrumpDef.FairySkillList[i-1].SkillLevel ); 
					SkillBtn:SetClientUserData( 2, nextLvTrumpDef.FairySkillList[i-1].SkillType );
					local SkillIcon = 0;
					local IconPath = "";
					if nextLvTrumpDef.FairySkillList[i-1].SkillType == SKILL_TYPE then
						IconPath = GetSkillIconPath()..SkillDef.IconID1..".tga"; 
					elseif nextLvTrumpDef.FairySkillList[i-1].SkillType == DIATHESIS_TYPE then
						IconPath = getDiathesisPath()..SkillDef.IconID..".tga"; 
					end				
					-- 如果没找到物品的图标则使用默认图标显示
					if not IsInExistence( IconPath ) then
						IconPath = DEFAULT_ITEM_PATH;
					end
					SkillBtnTex:SetTexture( IconPath );
					SkillBtnUpTex:Show();

					nSkillIndex = nSkillIndex + 1;
				end
			end
		end
	end	
	local nStaffIndex	= 1;
	local bCanUse = true;
	
	for i = nSkillIndex, MAX_FAIRY_SKILL do				   
		local SkillBtn		= getglobal( "TrumpLevelUpSkillBtn" .. i );  	
		local SkillBtnIcon	= getglobal( "TrumpLevelUpSkillBtn" .. i .. "Icon" );
		local SkillBtnUpTex = getglobal( "TrumpLevelUpSkillBtn" .. i .. "UpTex" );
		SkillBtnIcon:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
		SkillBtn:SetClientUserData( 0, 0 );		   
		SkillBtn:SetClientUserData( 1, 0 );		  
		SkillBtn:SetClientUserData( 2, 0 );	  
		SkillBtn:Disable();
		SkillBtnUpTex:Hide();
	end
end

function TrumpLevelUpFrameLvUpBtn_OnClick()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil or GetTrumpSelectGridIdx() < 0 then 
		return;
	end
	if TrumpLevelUpFrameEffectFrame:IsShown() then
		TrumpLevelUpFrameEffectFrame:Hide();
		if not GameWizard:isDo( GAMEWIZARD_FIRST_TRUMP_LEVEL_UP ) then
			GameWizard:doStep( GAMEWIZARD_FIRST_TRUMP_LEVEL_UP )
		end
	end
	local GameTrump = MainPlayer:getTrump();
	local RoleTrump = GameTrump:getRoleFairy( GetTrumpSelectGridIdx() );
	local TrumpDef	= GameTrump:getFairyDef( RoleTrump.FairyID, RoleTrump.FairyLevel );

	if RoleTrump.FairyLevel == 12 then
		ShowMidTips("该精灵已达到最高等级");
		return;
	end
	for i=1,GameTrump:getTrumpNum() do
		local otheRoleTrump = GameTrump:getRoleFairy( i - 1 );
		if GameTrump:isFairyInState( RoleTrump.Stat, FAIRY_STAT_EVOLVE ) then
			ShowMidTips( "已有精灵正在升级中" );
			return;
		end
	end
	if MainPlayer:isInMorph() and IsCampTrumpInMorph() then
		ShowMidTips("当前处于变身状态，无法升级");
		return;
	end
	
	if MainPlayer:getExp() < TrumpDef.UseExp  then
		ShowMidTips("人物经验不足，无法进行升级");
		return;
	end	

	if getPlayerMoney( MainPlayer ) < TrumpDef.UseMoney  then
		ShowMidTips("携带银币不足，无法进行升级");
		return;
	end
	if MainPlayer:getLv() <  TrumpDef.EvolveLevelLimit then
		ShowMidTips("人物等级不足，无法进行升级");
		return;
	end

	GameTrump:trumpEvolve( RoleTrump.WID.nHeight, RoleTrump.WID.nLow, GetTrumpSelectGridIdx() );
end

--------------------------			强化精灵		--------------------------

function TrumpStrengthFrame_OnShow()	
	if nCurTrumpSelect <= 0 then
		return
	end
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end
	TrumpStrengthFrameEffectFrame:Hide();
	local GameTrump	= MainPlayer:getTrump();
	local RoleTrump = GameTrump:getRoleFairy( t_TrumpIdx[nCurTrumpSelect] - 1 );
	local TrumpDef	= GameTrump:getFairyDef( RoleTrump.FairyID, RoleTrump.FairyLevel );

	local fairyStarDef = GameTrump:getFairyStarDef( RoleTrump.FairyID,RoleTrump.StarLvl );
	local nextFairyStarDef = GameTrump:getFairyStarDef( RoleTrump.FairyID,RoleTrump.StarLvl + 1 );
	local petTraitDef = GameTrump:getPetTraitDef( TrumpDef.TraitID,RoleTrump.StarLvl + 1 );
	-- 设置星级图标
	for i=1,RoleTrump.StarLvl do
		local tex = getglobal( "TrumpStrengthFrameStarTex" .. i );
		tex:ChangeTextureTemplate( "BlueLittleStarTemplate" );
	end
	for i=RoleTrump.StarLvl+1,12 do
		local tex = getglobal( "TrumpStrengthFrameStarTex" .. i );
		tex:ChangeTextureTemplate( "GraySmallXingXingTemplate" );
	end
	
	if nextFairyStarDef == nil then
		--ShowMidTips("该精灵已达到强化最高星级");
		--TrumpStrengthFrame:Hide();
		--return;
		nextFairyStarDef = fairyStarDef;
		fairyStarDef = GameTrump:getFairyStarDef( RoleTrump.FairyID,RoleTrump.StarLvl - 1 );
		petTraitDef = GameTrump:getPetTraitDef( TrumpDef.TraitID,RoleTrump.StarLvl );
	end

	--需要 精灵等级
	local needFairyLevel = GameTrump:getStrengthFairyLevel( RoleTrump.FairyID,RoleTrump.StarLvl + 1 );
	TrumpStrengthFrameNeedLvBtnValue:SetText( needFairyLevel .. "级" );
	if RoleTrump.FairyLevel < needFairyLevel then
		TrumpStrengthFrameNeedLvBtnValue:SetTextColor( 255,0,0 );
	else
		TrumpStrengthFrameNeedLvBtnValue:SetTextColor( 160,160,90 );
	end
	local itemDef = getItemDef( fairyStarDef.StarUpStuff.MainStuffID );
	if itemDef == nil then
		TrumpStrengthFrame:Hide();
		return;
	end
	TrumpStrengthFrameNeedStuffBtnValue:SetText( itemDef.Name .. "X" .. fairyStarDef.StarUpStuff.MainStuffNum );
	--对数量的判断
	local container = MainPlayer:getContainer();
	if container:getItemCount( itemDef.ItemID ) < fairyStarDef.StarUpStuff.MainStuffNum then
		TrumpStrengthFrameNeedStuffBtnValue:SetTextColor( 255,0,0 );
	else
		TrumpStrengthFrameNeedStuffBtnValue:SetTextColor( 160,160,90 );
	end
	-- 图标
	local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
	-- 如果没找到物品的图标则使用默认图标显示
	if not IsInExistence( IconPath ) then
		IconPath = DEFAULT_ITEM_PATH;
	end
	TrumpStrengthFrameNeedItemBtnIconTexture:SetTexture( IconPath );
	TrumpStrengthFrameNeedItemBtnBoxTexture:Show();
	SetEquipImportantEffectUV( itemDef, "TrumpStrengthFrameNeedItemBtnUVAnimationTex" );
	TrumpStrengthFrameNeedItemBtn:SetClientUserData( 0,itemDef.ItemID );
	TrumpStrengthFrameNeedItemBtnCountFont:SetText( fairyStarDef.StarUpStuff.MainStuffNum.. "/" .. container:getItemCount( itemDef.ItemID ) );

	TrumpStrengthFrameNeedMoneyBtnValue:SetText( fairyStarDef.StarUpStuff.Money );
	--对钱的判断
	if getPlayerMoney( MainPlayer ) < fairyStarDef.StarUpStuff.Money then
		TrumpStrengthFrameNeedMoneyBtnValue:SetTextColor( 255,0,0 );
	else
		TrumpStrengthFrameNeedMoneyBtnValue:SetTextColor( 160,160,90 );
	end

	TrumpStrengthFrameStrengthLvBtnValue:SetText( nextFairyStarDef.Star .. "星");
	TrumpStrengthFramePetChartBtnValue:SetText( petTraitDef.Name );
	TrumpStrengthFramePetChartBtn:SetClientString( petTraitDef.PetTraitDesc );
	TrumpStrengthFramePetExpBtnValue:SetText( "获取+" .. nextFairyStarDef.PetExpMul .."%" );
	TrumpStrengthFramePetDamageBtnValue:SetText( t_AttrTypeName[nextFairyStarDef.AttrType] .. "元素伤害+" .. nextFairyStarDef.Atk );

	if GameWizard:isDo( GAMEWIZARD_FIRST_LEVEL_UP_TRUMP ) and not GameWizard:isDo( GAME_WIZARD_FIRST_TRUMP_STAR_UP ) then
		GuideEffectShow( "TrumpStrengthFrame","","TrumpStrengthFrameStrengthBtn", "鼠标左键点击“强化”按钮，进行精灵强化", "uires\\ui\\mask\\TouMingTongDao.tga", 1, 1, 1, 1, 50, 60, -9, 5, 11, -51, false );
		local frameUVATexture = getglobal( "TrumpStrengthFrameEffectFrameUVAnimationTex" )
		frameUVATexture:SetSize( 140,120 )
		frameUVATexture:SetPoint("center", "TrumpStrengthFrameStrengthBtn", "center", 5, 5);
		frameUVATexture:SetUVAnimation(50,true);	
		frameUVATexture:Show();
		TrumpFrameEffectFrame:SetClientUserData( 0,GAMEWIZARD_FIRST_CAMP_TRUMP )
	end
end

function TrumpStrengthFrameStrengthBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end

	if TrumpStrengthFrameEffectFrame:IsShown() then
		TrumpStrengthFrameEffectFrame:Hide();
		if not GameWizard:isDo( GAME_WIZARD_FIRST_TRUMP_STAR_UP ) then
			GameWizard:doStep( GAME_WIZARD_FIRST_TRUMP_STAR_UP )
		end
	end

	local GameTrump = mainplayer:getTrump();
	local RoleTrump = GameTrump:getRoleFairy( t_TrumpIdx[nCurTrumpSelect] - 1 );
	local FairyStarDef = GameTrump:getFairyStarDef( RoleTrump.FairyID,RoleTrump.StarLvl );
	local nextFairyStarDef = GameTrump:getFairyStarDef( RoleTrump.FairyID,RoleTrump.StarLvl + 1 );
	local container = mainplayer:getContainer();
	if nextFairyStarDef == nil then
		ShowMidTips("该精灵已达到强化最高星级");
		return;
	end
	if TradeFrame:IsShown() then
		ShowMidTips("交易状态中，无法进行强化");
		return;
	end
	if mainplayer:isInStall() then
		ShowMidTips("摆摊状态中，无法进行强化");
		return;
	end

	if GameTrump:getStrengthFairyLevel( RoleTrump.FairyID,RoleTrump.StarLvl ) > RoleTrump.FairyLevel then
		ShowMidTips("精灵等级不足，无法进行强化");
		return;
	end
	
	local nNum = container:getItemCount( FairyStarDef.StarUpStuff.MainStuffID );
	if nNum < FairyStarDef.StarUpStuff.MainStuffNum then
		local itemDef = getItemDef( FairyStarDef.StarUpStuff.MainStuffID );
		ShowMidTips( itemDef.Name .. "不足，无法进行强化" );
		return;
	end

	if getPlayerMoney( mainplayer ) < FairyStarDef.StarUpStuff.Money  then
		ShowMidTips("携带的银币不足，无法进行强化");
		return; 
	end
	
	GameTrump:requestOpStarUp( t_TrumpIdx[nCurTrumpSelect] - 1,FairyStarDef.StarUpStuff.MainStuffID,FairyStarDef.StarUpStuff.MainStuffNum,FairyStarDef.StarUpStuff.Money );
end

-- 强化成功 显示特效
function ShowTrumpStrengthSuccessEffect()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local GameTrump = mainplayer:getTrump();
	local RoleTrump = GameTrump:getRoleFairy( t_TrumpIdx[nCurTrumpSelect] - 1 );
	local starlv = RoleTrump.StarLvl + 1;
	if starlv <= 12 then
		local tex = getglobal( "TrumpStrengthFrameStarTex" .. starlv );
		TrumpStrengthFrameEffectTex:SetPoint( "topleft",tex:GetName(),"topleft",-18,-24 );
		TrumpStrengthFrameEffectTex:SetUVAnimation( 50,false );
	end
end

--------------------------			TIPS 相关		--------------------------

function TrumpFrameAddSpeedBtn_OnEnter()
	SetGameTooltips( this:GetName(),"点击打开精灵升级加速面板" );
end

function TrumpFrameKeyInBtn_OnEnter()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local GameTrump = mainplayer:getTrump();
	local RoleTrump		= GameTrump:getRoleFairy( t_TrumpIdx[nCurTrumpSelect] - 1 );
	local TrumpDef		= GameTrump:getFairyDef( RoleTrump.FairyID, RoleTrump.FairyLevel );
	local nTipsHeight = 30;
	if TrumpDef.ZeroReelID and TrumpDef.PNum == 0 then
		TrumpKeyInTipsFrameUseFont:Hide();
		TrumpKeyInTipsFrameLvFont:Hide();
		TrumpKeyInTipsFrameXPUseFont:Hide();
		TrumpKeyInTipsFrameItemUseFont:Hide();
		TrumpKeyInTipsFrameNextLvFont:Hide();
		TrumpKeyInTipsFrameNextDescTitle:Hide();
		TrumpKeyInTipsFrameNextDescFont:Hide();
		TrumpKeyInTipsFrameLineTex1:SetPoint( "topleft","TrumpKeyInTipsFrame","topleft",2,nTipsHeight );
		nTipsHeight = nTipsHeight + 8;
		TrumpKeyInTipsFrameRich:resizeText( 170,100 );
		TrumpKeyInTipsFrameRich:SetText( "\t激活精灵的能力，制造出元素属性印记，使用这些印记可为装备添加上元素属性。\n#c00ff00每个精灵每天只能刻印1次\n精灵等级越高，刻印制造的印记属性越高#n",221,196,159 );
		TrumpKeyInTipsFrameRich:SetPoint( "topleft","TrumpKeyInTipsFrame","topleft",10,nTipsHeight );
		nTipsHeight = nTipsHeight + TrumpKeyInTipsFrameRich:GetTotalHeight()+2;
		TrumpKeyInTipsFrameLineTex2:SetPoint( "topleft","TrumpKeyInTipsFrame","topleft",2,nTipsHeight );
		nTipsHeight = nTipsHeight + 8;
		TrumpKeyInTipsFrameNextFariyLvFont:SetText( "精灵达到7级才能进行刻印" );
		TrumpKeyInTipsFrameNextFariyLvFont:SetPoint( "topleft","TrumpKeyInTipsFrame","topleft",15,nTipsHeight );
		TrumpKeyInTipsFrameNextFariyLvFont:SetTextColor( 255,0,0 );
		nTipsHeight = nTipsHeight + 20;
	else
		TrumpKeyInTipsFrameUseFont:Show();
		TrumpKeyInTipsFrameUseFont:SetPoint( "topleft","TrumpKeyInTipsFrame","topleft",20,nTipsHeight )
		nTipsHeight = nTipsHeight + 15;
		TrumpKeyInTipsFrameLvFont:Show();
		TrumpKeyInTipsFrameLvFont:SetText( "刻印等级：" .. (RoleTrump.FairyLevel - 6) .."级" );
		TrumpKeyInTipsFrameLvFont:SetPoint( "topleft","TrumpKeyInTipsFrame","topleft",20,nTipsHeight );
		nTipsHeight = nTipsHeight + 15;
		TrumpKeyInTipsFrameXPUseFont:Show();
		TrumpKeyInTipsFrameXPUseFont:SetText("刻印消耗：" .. TrumpDef.KeyinUseXp .."魂能值");
		local textColor = mainplayer:getXP() >= TrumpDef.KeyinUseXp and GetColor()[COLOR_WHITE] or GetColor()[COLOR_RED];
		TrumpKeyInTipsFrameXPUseFont:SetTextColor( textColor[1],textColor[2],textColor[3] );
		TrumpKeyInTipsFrameXPUseFont:SetPoint( "topleft","TrumpKeyInTipsFrame","topleft",20,nTipsHeight );
		nTipsHeight = nTipsHeight + 15;
		TrumpKeyInTipsFrameItemUseFont:Show();
		local itemDef = getItemDef( ZEROREELID_TOLUA );
		TrumpKeyInTipsFrameItemUseFont:SetText( "消耗材料：" ..itemDef.Name .. TrumpDef.ZeroReelID .. "个" );
		local container = mainplayer:getContainer();
		local textColor = container:getItemCount( ZEROREELID_TOLUA ) >= TrumpDef.ZeroReelID and GetColor()[COLOR_WHITE] or GetColor()[COLOR_RED];
		TrumpKeyInTipsFrameItemUseFont:SetTextColor( textColor[1],textColor[2],textColor[3] );
		TrumpKeyInTipsFrameItemUseFont:SetPoint( "topleft","TrumpKeyInTipsFrame","topleft",20,nTipsHeight );
		nTipsHeight = nTipsHeight + 15;
		TrumpKeyInTipsFrameLineTex1:SetPoint( "topleft","TrumpKeyInTipsFrame","topleft",2,nTipsHeight );
		nTipsHeight = nTipsHeight + 8;
		TrumpKeyInTipsFrameRich:resizeText( 170,100 );
		TrumpKeyInTipsFrameRich:SetText( "\t激活精灵的能力，制造出元素属性印记，使用这些印记可为装备添加上元素属性。\n#c00ff00每个精灵每天只能刻印1次\n精灵等级越高，刻印制造的印记属性越高#n",221,196,159 );
		TrumpKeyInTipsFrameRich:SetPoint( "topleft","TrumpKeyInTipsFrame","topleft",10,nTipsHeight );
		nTipsHeight = nTipsHeight + TrumpKeyInTipsFrameRich:GetTotalHeight() + 2;
		--TODO HERE
		if RoleTrump.FairyLevel ~= 12 then
			TrumpKeyInTipsFrameLineTex2:Show();
			TrumpKeyInTipsFrameLineTex2:SetPoint( "topleft","TrumpKeyInTipsFrame","topleft",2,nTipsHeight );
			nTipsHeight = nTipsHeight + 8;
			TrumpKeyInTipsFrameNextLvFont:Show();
			TrumpKeyInTipsFrameNextLvFont:SetPoint( "topleft","TrumpKeyInTipsFrame","topleft",20,nTipsHeight );
			nTipsHeight = nTipsHeight + 15;
			TrumpKeyInTipsFrameNextFariyLvFont:SetText( "需要精灵等级：" .. RoleTrump.FairyLevel + 1 .. "级" );
			TrumpKeyInTipsFrameNextFariyLvFont:SetTextColor( 0,255,0 );
			TrumpKeyInTipsFrameNextFariyLvFont:SetPoint( "topleft","TrumpKeyInTipsFrame","topleft",20,nTipsHeight );
			nTipsHeight = nTipsHeight + 15;
			TrumpKeyInTipsFrameNextDescTitle:Show();
			TrumpKeyInTipsFrameNextDescTitle:SetPoint( "topleft","TrumpKeyInTipsFrame","topleft",20,nTipsHeight );
			nTipsHeight = nTipsHeight + 15;
			TrumpKeyInTipsFrameNextDescFont:Show();
			TrumpKeyInTipsFrameNextDescFont:SetPoint( "topleft","TrumpKeyInTipsFrame","topleft",15,nTipsHeight );
			nTipsHeight = nTipsHeight + 20;
		else
			TrumpKeyInTipsFrameLineTex2:Hide();
			TrumpKeyInTipsFrameNextLvFont:Hide();
			TrumpKeyInTipsFrameNextFariyLvFont:SetText("");
			TrumpKeyInTipsFrameNextDescTitle:Hide();
			TrumpKeyInTipsFrameNextDescFont:Hide();
		end
	end
	TrumpKeyInTipsFrame:SetSize( 200,nTipsHeight );
	
	local nWidth    = TrumpKeyInTipsFrame:GetWidth();
	local nHeight   = TrumpKeyInTipsFrame:GetHeight();

	local nRealTop  = this:GetRealTop();
	local nRight    = this:GetRight();

	if ( nRealTop - nHeight ) > 0 then
		if ( nRight + nWidth < GetScreenWidth() ) then
			TrumpKeyInTipsFrame:SetPoint("BottomLeft",this:GetName(),"TopRight",0,-10);
		else
			TrumpKeyInTipsFrame:SetPoint("BottomRight",this:GetName(),"TopLeft",0,-10);
		end
	else
		if ( nRight + nWidth < GetScreenWidth() ) then
			TrumpKeyInTipsFrame:SetPoint("TopLeft",this:GetName(),"BottomRight",0,10);
		else
			TrumpKeyInTipsFrame:SetPoint("TopRight",this:GetName(),"BottomLeft",0,10);
		end
	end
	TrumpKeyInTipsFrame:Show();
end


function TrumpCampaignBtn_OnEnter()
	SetGameTooltips( this:GetName(),"召唤出强大的精灵守护你" );
end

function TrumpInSupportBtn_OnEnter()
	SetGameTooltips( this:GetName(),"召回出征的精灵" );
end

function TrumpFrameFairySkill_OnMouseMove()
	if isPointInFrame( this:GetName() .."TextBkg" ) then
		SetGameTooltips( this:GetName() .."TextBkg","精灵已学技能" );
	end
end

function TrumpFramePetInfo_OnMouseMove()
	if isPointInFrame( this:GetName() .."TextBkg" ) then
		SetGameTooltips( this:GetName() .."TextBkg","强化星级越高，精灵对出征宠物影响越大" );
	end
end

function TrumpFrameFairyInfo_OnMouseMove()
	if isPointInFrame( this:GetName() .."TextBkg" ) then
		SetGameTooltips( this:GetName() .."TextBkg","精灵等级越高，精灵越强" );
	end
end

function TrumpFramePetBtn_OnEnter()
	SetGameTooltips( this:GetName(),"当前出征宠物\n只有已召唤的精灵，才能增强出征宠物\n点击打开宠物面板" );
end

function TrumpFrameStrengthBtn_OnEnter()
	SetGameTooltips( this:GetName(),"点击进行精灵强化\n强化精灵可提高对出征宠物的影响" );
end

function TrumpFrameChartBtn_OnEnter()
	SetGameTooltips( this:GetName(),"提高当前出征宠物的属性\n" .. this:GetClientString() );
end

function TrumpFrameAttrBtn_OnEnter()
	SetGameTooltips( this:GetName(),"提高当前出征宠物杀怪获取的经验" );
end

function TrumpFrameInstBtn_OnEnter()
	SetGameTooltips( this:GetName(),"当前出征宠物额外增加的元素伤害" );
end

function TrumpFrameLvUpBtn_OnEnter()
	SetGameTooltips( this:GetName(),"点击进行精灵升级" );
end

function TrumpFrameMaxXPBtn_OnEnter()
	SetGameTooltips( this:GetName(),"精灵当前的魂能值\n使用精灵技能需要的消耗" );
end

function TrumpFrameTimeBtn_OnEnter()
	SetGameTooltips( this:GetName(),"精灵当前技能的变身时间" );
end

function TrumpFrameXPRateBtn_OnEnter()
	local MainPlayer	= ActorMgr:getMainPlayer();
	local GameTrump		= MainPlayer:getTrump();
	local RoleTrump = GameTrump:getRoleFairy( t_TrumpIdx[nCurTrumpSelect] - 1 );
	SetGameTooltips( this:GetName(),t_TrumpIDStatus[RoleTrump.FairyID][3] );
end

function TrumpLevelUpFrameNeedLvBtn_OnEnter()
	SetGameTooltips( this:GetName(),"精灵升级需要人物达到的等级" );
end

function TrumpLevelUpFrameNeedExpBtn_OnEnter()
	SetGameTooltips( this:GetName(),"精灵升级需要扣除的人物经验" );
end

function TrumpLevelUpFrameNeedMoneyBtn_OnEnter()
	SetGameTooltips( this:GetName(),"精灵升级需要花费的银币" );
end

function TrumpLevelUpFrameNeedTimeBtn_OnEnter()
	SetGameTooltips( this:GetName(),"精灵升级需要消耗的时间" );
end

function TrumpLevelUpFrameTrumpLvBtn_OnEnter()
	SetGameTooltips( this:GetName(),"精灵升级后的等级" );
end

function TrumpLevelUpFrameMaxXpBtn_OnEnter()
	SetGameTooltips( this:GetName(),"精灵升级后的魂能值" );
end

function TrumpLevelUpFrameTimeBtn_OnEnter()
	SetGameTooltips( this:GetName(),"精灵升级后技能的变身时间" );
end

function TrumpLevelUpFrameXpRateBtn_OnEnter()
	local MainPlayer	= ActorMgr:getMainPlayer();
	local GameTrump		= MainPlayer:getTrump();
	local RoleTrump = GameTrump:getRoleFairy( t_TrumpIdx[nCurTrumpSelect] - 1 );
	local nextLvTrumpDef = GameTrump:getFairyDef( RoleTrump.FairyID,RoleTrump.FairyLevel + 1 );
	if nextLvTrumpDef == nil then
		nextLvTrumpDef = GameTrump:getFairyDef( RoleTrump.FairyID,RoleTrump.FairyLevel );
	end
	SetGameTooltips( this:GetName(),t_TrumpIDStatus[nextLvTrumpDef.FairyID][3] );	
	--SetGameTooltips( this:GetName(),"精灵升级后获得魂能的几率" );
end

function TrumpLevelUpFrameLvUpBtn_OnEnter()
	SetGameTooltips( this:GetName(),"点击开始升级精灵" );
end

function TrumpStrengthFrameNeedLvBtn_OnEnter()
	SetGameTooltips( this:GetName(),"强化需要达到的精灵等级" );
end

function TrumpStrengthFrameNeedStuffBtn_OnEnter()
	SetGameTooltips( this:GetName(),"精灵强化需要消耗的材料" );
end

function TrumpStrengthFrameNeedMoneyBtn_OnEnter()
	SetGameTooltips( this:GetName(),"强化需要花费的银币" );
end

function TrumpStrengthFrameStrengthLvBtn_OnEnter()
	SetGameTooltips( this:GetName(),"精灵强化后的强化星级" );
end

function TrumpStrengthFramePetChartBtn_OnEnter()
	SetGameTooltips( this:GetName(),"精灵强化后提高出征宠物的属性\n" .. this:GetClientString() );
end

function TrumpStrengthFramePetExpBtn_OnEnter()
	SetGameTooltips( this:GetName(),"精灵强化后提高出征宠物杀怪获取的经验" );
end

function TrumpStrengthFramePetDamageBtn_OnEnter()
	SetGameTooltips( this:GetName(),"强化后出征宠物额外增加的元素伤害" );
end

function TrumpStrengthFrameDescBtn_OnEnter()
	SetGameTooltips( this:GetName(),"强化说明：\n1. 强化星级越高，精灵对出征宠物的影响越大\n2. 强化失败有一定几率回退强化星级" );
end

function TrumpStrengthFrameStrengthBtn_OnEnter()
	SetGameTooltips( this:GetName(),"点击强化精灵，提高对出征宠物的影响" );
end

function TrumpStrengthFrameNeedItemBtn_OnEnter()
	local itemDef = getItemDef( this:GetClientUserData( 0 ) );
	if itemDef ~= nil then
		ShowItemTips(LUA_ITEMTIPS_TYPE_NORMAL,LUA_ITEMTIPS_ITEMDEF,itemDef);
	end
end

function TrumpFramePreFairyBtn_OnEnter()
	SetGameTooltips( this:GetName(),"切换上一个精灵" );
end

function TrumpFrameNextFariyBtn_OnEnter()
	SetGameTooltips( this:GetName(),"切换下一个精灵" );
end