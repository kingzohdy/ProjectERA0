-- 策划填写的天赋排序表
local t_geniusStandardSortList = 
{
-- [race] = { [id] = { index = , name = },}
[1] = 
{ 
[1000] = { index = 1, name = "Pa" },	[1001] = { index = 2, name = "Ta" },	[1002] = { index = 3, name = "Pon" }, 
[1003] = { index = 5, name = "攻击" },	[1004] = { index = 6, name = "攻击" },	[1005] = { index = 7, name = "攻击" }, 
[1006] = { index = 9, name = "攻击" },	[1007] = { index = 11, name = "攻击" },	[1008] = { index = 13, name = "攻击" },
[1009] = { index = 14, name = "攻击" },	[1010] = { index = 15, name = "攻击" },	[1011] = { index = 16, name = "攻击" },
[1012] = { index = 17, name = "攻击" },	[1013] = { index = 19, name = "攻击" },
},

[2] = 
{
[2000] = { index = 1, name = "攻击" },	[2001] = { index = 2, name = "攻击" },	[2002] = { index = 3, name = "攻击" },
[2003] = { index = 5, name = "攻击" },	[2004] = { index = 6, name = "攻击" },	[2005] = { index = 7, name = "攻击" },
[2006] = { index = 9, name = "攻击" },	[2007] = { index = 11, name = "攻击" },	[2008] = { index = 13, name = "攻击" },
[2009] = { index = 14, name = "攻击" },	[2010] = { index = 15, name = "攻击" },	[2011] = { index = 16, name = "攻击" },
[2012] = { index = 17, name = "攻击" },	[2013] = { index = 19, name = "攻击" },
},

[3] = 
{
[3000] = { index = 1, name = "攻击" },	[3001] = { index = 2, name = "攻击" },	[3002] = { index = 3, name = "攻击" },
[3003] = { index = 5, name = "攻击" },	[3004] = { index = 6, name = "攻击" },	[3005] = { index = 7, name = "攻击" },
[3006] = { index = 9, name = "攻击" },	[3007] = { index = 11, name = "攻击" },	[3008] = { index = 13, name = "攻击" },
[3009] = { index = 14, name = "攻击" },	[3010] = { index = 15, name = "攻击" },	[3011] = { index = 16, name = "攻击" },
[3012] = { index = 17, name = "攻击" },	[3013] = { index = 19, name = "攻击" },
},

[4] = 
{
[4000] = { index = 1, name = "攻击" },	[4001] = { index = 2, name = "攻击" },	[4002] = { index = 3, name = "攻击" },
[4003] = { index = 5, name = "攻击" },	[4004] = { index = 6, name = "攻击" },	[4005] = { index = 7, name = "攻击" },
[4006] = { index = 9, name = "攻击" },	[4007] = { index = 11, name = "攻击" },	[4008] = { index = 13, name = "攻击" },
[4009] = { index = 14, name = "攻击" },	[4010] = { index = 15, name = "攻击" },	[4011] = { index = 16, name = "攻击" },
[4012] = { index = 17, name = "攻击" },	[4013] = { index = 19, name = "攻击" },
},
};

local DiathesisShowType

function DiathesisFrame_Test_OnShow()
	ShowDiathesisFrameXinShouGuide( { text = "测试天赋效果" } );
end

function ShowDiathesisFrameXinShouGuide( data )
	local xinShouBtn		= getglobal( "DiathesisFrameXinShouBtn" );
	local animationTexture	= getglobal( "DiathesisFrameXinShouBlinkGeniusIconBtnUVAnimationTex" );
	animationTexture:SetUVAnimation( 50, true );
	animationTexture:Show();
	xinShouBtn:Show();
	DiathesisFrameXinShouBlinkGeniusIconBtn:Show();

	local rich = getglobal( xinShouBtn:GetName().."Rich" );
	rich:SetClientString( data["text"] );
	local nSelfDefineExtendWidth = 14;
	ReSizeTooltip( rich:GetName(), xinShouBtn:GetName(), nSelfDefineExtendWidth );
end

function DiathesisFrame_OnLoad()
	this:RegisterEvent( "GE_UPDATE_DIATHESIS" );
	this:RegisterEvent( "GE_GENIUS_CHG" );
	this:RegisterEvent( "GE_DEC_GENIUS_POINT" );
	this:RegisterEvent( "GE_CHANGE_RACE" );
end

function UpdateGeniusFonts()
	local mainplayer	= ActorMgr:getMainPlayer();
	local diathesisMgr	= mainplayer:getDiathesisManager();
	local diathesisInfo = diathesisMgr:getDiathesisInfo();
	local t_fonts = { ["DiathesisFrame_CanUseGeniusValueFont"] = 
					{ value = "剩余天赋点："..diathesisInfo.LearnGeniusPoint }, 
					["DiathesisFrame_NormalGeniusValueFont"] = { value = "已使用天赋点："..diathesisInfo.UsedGeniusPoint }, 
					};
	for name, data in pairs( t_fonts ) do
		local font = getglobal( name );
		font:SetText( data["value"] );
	end
end

function DiathesisFrame_OnEvent()
	if arg1 == "GE_UPDATE_DIATHESIS" then
		UpdateNotLearnDiathesisData()
		if not this:IsShown() then
			return
		end

		UpdateGeniusButtons()
		UpdateGeniusFonts()
	elseif arg1 == "GE_GENIUS_CHG" then
		UpdateGeniusFonts()
	elseif arg1 == "GE_DEC_GENIUS_POINT" then
		if GameWizard:isDo(GAMEWIZARD_FIRST_GETGENIUSOPT) and not GameWizard:isDo(GAMEWIZARD_FIRST_ADD_TIAN_FU) and this:IsShown() then
			local t_hideIcons = { "DiathesisFrameXinShouBtn", "DiathesisFrameShowLeaveGeniusValueBackGroundUVAnimationTex", 
								"DiathesisFrameXinShouBlinkGeniusIconBtn" }
			util.HideIcons( t_hideIcons )
			GameWizard:doStep(GAMEWIZARD_FIRST_ADD_TIAN_FU)
		end
	elseif arg1 == "GE_CHANGE_RACE" then
		local player = ActorMgr:getMainPlayer()
		local msg = UIMSG:getUIMsg()
		if player and msg.Who.id == player:GetID() then
			UpdateNotLearnDiathesisData();
			if not this:IsShown() then
				return
			end

			UpdateGeniusButtons()
			UpdateGeniusFonts()
		end
	end
end

local t_npcInfo = { id = 0 };
function openGeniusUI( npcId )
	t_npcInfo = { id = npcId };
end

function DiathesisFrame_OnHide()	
end

local szDiathesisIconPath = "uires\\TuPianLei\\JiNeng\\TianFu\\";
function getDiathesisPath()
	return szDiathesisIconPath;
end

function CanStudyGenius( id, lv )
	local diathesisiDef = getDiathesisDef( id, lv );
	-- 若已经达到天赋的上限
	if diathesisiDef == nil then
		ShowMidTips( "天赋已达到最高级别，无法继续升级该天赋" );
		return false;
	end

	local mainplayer	= ActorMgr:getMainPlayer();
	local diathesisMgr	= mainplayer:getDiathesisManager();
	local diathesisInfo	= diathesisMgr:getDiathesisInfo();
	-- 若等级不够，天赋点不够，已经用过的天赋点不足
	if diathesisiDef.Learn.LevelMin > mainplayer:getLv() then
		ShowMidTips( "需要达到"..diathesisiDef.Learn.LevelMin.."级才能学习" );
		return false;
	end
	
	if diathesisiDef.PreUsedGeniusPoint > diathesisInfo.UsedGeniusPoint then
		ShowMidTips( "己投入天赋点未达到需求" );
		return false;
	end

	if diathesisiDef.UseGeniusPoint > diathesisInfo.LearnGeniusPoint then
		ShowMidTips( "可使用天赋点不足" );
		return false;
	end
	
	local preDiathesisDef;
	if diathesisiDef.Learn.PreDiathesisID ~= 0 then
		preDiathesisDef = getDiathesisDef( diathesisiDef.Learn.PreDiathesisID,  diathesisiDef.Learn.PreDiathesisLevel );
	end
	
	if preDiathesisDef == nil then
		return true;
	end

	for i = 1, diathesisInfo.DiathesisNum do
		local diathesisData = diathesisInfo.DiathesisList[i-1];
		if diathesisData.DiathesisID == diathesisiDef.Learn.PreDiathesisID then
			if diathesisData.DiathesisLevel < diathesisiDef.Learn.PreDiathesisLevel then
				ShowMidTips( preDiathesisDef.Name.."天赋至少要"..diathesisiDef.Learn.PreDiathesisLevel.."级，"..
					( lv == 1 and "才能学习" or "才能升级" )..diathesisiDef.Name );
				return false;
			end

			return true;
		end
	end
	
	ShowMidTips( "需要先学习"..preDiathesisDef.Name.."，"..( lv == 1 and "才能学习" or "才能升级" )..diathesisiDef.Name );
	return false;
end

function DiathesisButton_OnDoubleClick()
	local id = this:GetClientUserData( 0 );
	local lv = this:GetClientUserData( 1 ) + 1;
	if not CanStudyGenius( id, lv ) then
		return;
	end
	
	if lv == 1 then
		GameSkillLearn:geniusLearn( 0, id, lv );
	else
		GameSkillLearn:geniusLvUp( 0, id, lv );
	end
end

-- TODO: 显示素质的tips内容
function DiathesisButton_OnEnter()
	local id = this:GetClientUserData( 0 );
	local lv = this:GetClientUserData( 1 );
	SpellTipsFrame:SetClientUserData( 0, id );
	SpellTipsFrame:SetClientUserData( 1, lv );	
	SpellTipsFrame:SetClientUserData( 2, DIATHESIS_TYPE );
	SpellTipsFrame:SetClientString( this:GetName() );
	SpellTipsFrame:Show();
end

local szdaithesisFilePath = "uires\\TuPianLei\\JiNeng\\JiNeng\\";
-- test
--[[
local t_geniusDatas = 
{
	szdaithesisFilePath.."1001.tga", szdaithesisFilePath.."1001.tga", szdaithesisFilePath.."1001.tga", szdaithesisFilePath.."1001.tga",
	szdaithesisFilePath.."1003.tga", szdaithesisFilePath.."1003.tga", szdaithesisFilePath.."1003.tga", szdaithesisFilePath.."1003.tga",
	szdaithesisFilePath.."1006.tga", szdaithesisFilePath.."1006.tga", szdaithesisFilePath.."1006.tga", szdaithesisFilePath.."1006.tga",
	szdaithesisFilePath.."1010.tga", szdaithesisFilePath.."1010.tga", szdaithesisFilePath.."1010.tga", szdaithesisFilePath.."1010.tga",
	szdaithesisFilePath.."1012.tga", szdaithesisFilePath.."1012.tga", szdaithesisFilePath.."1012.tga", szdaithesisFilePath.."1012.tga",
	szdaithesisFilePath.."1014.tga", szdaithesisFilePath.."1014.tga", szdaithesisFilePath.."1014.tga", szdaithesisFilePath.."1014.tga",
	szdaithesisFilePath.."1016.tga", szdaithesisFilePath.."1016.tga", szdaithesisFilePath.."1016.tga", szdaithesisFilePath.."1016.tga",
	szdaithesisFilePath.."1018.tga", szdaithesisFilePath.."1018.tga", szdaithesisFilePath.."1018.tga", szdaithesisFilePath.."1018.tga", 
	szdaithesisFilePath.."1020.tga", szdaithesisFilePath.."1020.tga", szdaithesisFilePath.."1020.tga", szdaithesisFilePath.."1020.tga",
	szdaithesisFilePath.."1022.tga", szdaithesisFilePath.."1022.tga", szdaithesisFilePath.."1022.tga", szdaithesisFilePath.."1022.tga",
	szdaithesisFilePath.."1024.tga", szdaithesisFilePath.."1024.tga", szdaithesisFilePath.."1024.tga", szdaithesisFilePath.."1024.tga",
	szdaithesisFilePath.."1026.tga", szdaithesisFilePath.."1026.tga", szdaithesisFilePath.."1026.tga", szdaithesisFilePath.."1026.tga",
	szdaithesisFilePath.."1028.tga", szdaithesisFilePath.."1028.tga", szdaithesisFilePath.."1028.tga", szdaithesisFilePath.."1028.tga",
	szdaithesisFilePath.."1030.tga", szdaithesisFilePath.."1030.tga", szdaithesisFilePath.."1030.tga", szdaithesisFilePath.."1030.tga",
	szdaithesisFilePath.."1032.tga", szdaithesisFilePath.."1032.tga", szdaithesisFilePath.."1032.tga", szdaithesisFilePath.."1032.tga",
	szdaithesisFilePath.."1034.tga", szdaithesisFilePath.."1034.tga", szdaithesisFilePath.."1034.tga", szdaithesisFilePath.."1034.tga",
	szdaithesisFilePath.."2010.tga", szdaithesisFilePath.."2010.tga", szdaithesisFilePath.."2010.tga", szdaithesisFilePath.."2010.tga",
	szdaithesisFilePath.."2012.tga", szdaithesisFilePath.."2012.tga", szdaithesisFilePath.."2012.tga", szdaithesisFilePath.."2012.tga",
	szdaithesisFilePath.."2014.tga", szdaithesisFilePath.."2014.tga", szdaithesisFilePath.."2014.tga", szdaithesisFilePath.."2014.tga",
	szdaithesisFilePath.."2016.tga", szdaithesisFilePath.."2016.tga", szdaithesisFilePath.."2016.tga", szdaithesisFilePath.."2016.tga",
	szdaithesisFilePath.."2018.tga", szdaithesisFilePath.."2018.tga", szdaithesisFilePath.."2018.tga", szdaithesisFilePath.."2018.tga",
	szdaithesisFilePath.."2020.tga", szdaithesisFilePath.."2020.tga", szdaithesisFilePath.."2020.tga", szdaithesisFilePath.."2020.tga",
};
--]]

local t_adjustData =	{	buttonSize = 52, spaceAboveFirstBtn = 19, 
							verticalSpaceBetweenBtn = 32, spaceUnderLastBtn = 24, 
							-- 当ScrollFrame往上翻内容		当ScrollFrame往下翻内容，第一行button上面的剩余空间
							firstBtnYWhenScrollDown = 32,	firstBtnYWhenScrollUp = 11, maxBtnNum = 24,
							-- 当前第一行按钮显示的内容在t_daithesisDatas表中的行数
							curStartLine = 1, iconButtonSize = 44, numPerLine = 4
						};

-- format: [nIndex] = { { id = , level = }, }
-- TODO
local t_geniusDatas = {};

function UpdateNotLearnDiathesisData()
	t_geniusDatas			= {};

	local mainplayer	= ActorMgr:getMainPlayer();
	local nRace			= mainplayer:getRace();
	local diathesisMgr	= mainplayer:getDiathesisManager();
	for i = 1, diathesisMgr:getAllGeniusNum() do
		local geniusDef = diathesisMgr:getDiathesis( i - 1 );

		-- 若与主角职业不符合，既没学过，也还没显示出来，则显示这个天赋内容
		if (nRace == geniusDef.Learn.Career or nRace % 16 == geniusDef.Learn.Career) then
			t_geniusDatas[geniusDef.DiathesisID] = diathesisMgr:getLearnedGeniusLevel( geniusDef.DiathesisID );
		end
	end
end

-- 检查某个天赋还未学习的等级
function InitGeniusData()
	t_adjustData.curStartLine = 1;
	t_geniusDatas = {};
	t_alreadyShowGeniusID = { };
end

local t_geniusBackTex = 
{ 
	[g_nCAREER_WARRIOR] = { path = [[uires\TuPianLei\TianFuShuDiTu\ZhanShi.jpg]] },
	[g_nCAREER_MAGICOR] = { path = [[uires\TuPianLei\TianFuShuDiTu\FaShi.jpg]] },
	[g_nCAREER_GUNER]	= { path = [[uires\TuPianLei\TianFuShuDiTu\QiangShou.jpg]] },
	[g_nCAREER_CHURCH]	= { path = [[uires\TuPianLei\TianFuShuDiTu\JiSi.jpg]] },
	
	[g_nCAREER_WARRIOR_SINGEL] = { path = [[uires\TuPianLei\TianFuShuDiTu\Zhan1.jpg]] },
	[g_nCAREER_WARRIOR_DOUBLE] = { path = [[uires\TuPianLei\TianFuShuDiTu\Zhan2.jpg]] },
	[g_nCAREER_MAGICOR_SINGEL] = { path = [[uires\TuPianLei\TianFuShuDiTu\Fa1.jpg]] },
	[g_nCAREER_MAGICOR_DOUBLE] = { path = [[uires\TuPianLei\TianFuShuDiTu\Fa2.jpg]] },
	[g_nCAREER_GUNER_SINGEL] = { path = [[uires\TuPianLei\TianFuShuDiTu\Qiang1.jpg]] },
	[g_nCAREER_GUNER_DOUBLE] = { path = [[uires\TuPianLei\TianFuShuDiTu\Qiang2.jpg]] },
	[g_nCAREER_CHURCH_SINGEL] = { path = [[uires\TuPianLei\TianFuShuDiTu\Ji1.jpg]] },
	[g_nCAREER_CHURCH_DOUBLE] = { path = [[uires\TuPianLei\TianFuShuDiTu\Ji2.jpg]] },
};

-- 更新界面的时候，我们只需判断第一行的按钮是否完全不能显示了，若是，则重新刷新界面
-- 否则由ScrollFrame来控制显示效果

-- 天赋面板右边的3个标签
local t_skillLabel = {
	[g_nCAREER_WARRIOR]			= "uires\\TuPianLei\\WuPin\\5300519.tga",
	[g_nCAREER_MAGICOR]			= "uires\\TuPianLei\\WuPin\\5300519.tga",
	[g_nCAREER_GUNER]			= "uires\\TuPianLei\\WuPin\\5300519.tga",
	[g_nCAREER_CHURCH]			= "uires\\TuPianLei\\WuPin\\5300519.tga",
    
	[g_nCAREER_WARRIOR_SINGEL]	= "uires\\TuPianLei\\WuPin\\1019904.tga",
	[g_nCAREER_MAGICOR_SINGEL]	= "uires\\TuPianLei\\JiNeng\\JiNeng\\1003.tga",
	[g_nCAREER_GUNER_SINGEL]	= "uires\\TuPianLei\\WuPin\\1219900.tga",
	[g_nCAREER_CHURCH_SINGEL]	= "uires\\TuPianLei\\JiNeng\\JiNeng\\4030.tga",
    
	[g_nCAREER_WARRIOR_DOUBLE]	= "uires\\TuPianLei\\WuPin\\1009901.tga",
	[g_nCAREER_MAGICOR_DOUBLE]	= "uires\\TuPianLei\\JiNeng\\JiNeng\\2010.tga",
	[g_nCAREER_GUNER_DOUBLE]	= "uires\\TuPianLei\\WuPin\\1259901.tga",
	[g_nCAREER_CHURCH_DOUBLE]	= "uires\\TuPianLei\\JiNeng\\JiNeng\\4002.tga",
};

function DiathesisFrame_OnShow()
	if SkillFrame:IsShown() then
		local x, y = SkillFrame:GetRealLeft(), SkillFrame:GetRealTop()
		SkillFrame:Hide()
		this:SetPoint("topleft", "$parent", "topleft", x, y);
	else
		this:SetPoint("topleft", "$parent", "topleft", 55, 163)
	end
	local mainplayer = ActorMgr:getMainPlayer();

	-- 设置标签按钮
	local t_lableBtns = { "DiathesisFrame_SetRaceSkillBtn","DiathesisFrame_SetGeniusBtn" };
	for _, name in ipairs( t_lableBtns ) do
		local btn = getglobal( name );
		btn:Enable();
	end

	local t_disableBtns = { "DiathesisFrame_SetGeniusBtn", };
	for _, name in ipairs( t_disableBtns ) do
		local btn = getglobal( name );
		btn:Disable();
	end
	
	DiathesisFrame_CommonSkillLabel_OnClick()
	
	-- 设置剩余天赋点数
	UpdateGeniusFonts();

	UpdateGeniusButtons();
			
	local nRace = mainplayer:getRace();
	
	DiathesisFrameCommonSkillLabelIcon:SetTexture(t_skillLabel[nRace % 0x10]);
	DiathesisFrameCommonSkillLabelIcon:SetTexRelUV( 1, 1 );
	
	if nRace > 0x10 then
		DiathesisFrameSpecialSkillLabel:Show();
		DiathesisFrameSpecialSkillLabelIcon:SetTexture(t_skillLabel[nRace]);
		DiathesisFrameCommonSkillLabelIcon:SetTexRelUV( 1, 1 );
	else
		DiathesisFrameSpecialSkillLabel:Hide()
	end
	
	local diathesisMgr	= mainplayer:getDiathesisManager();
	local diathesisInfo = diathesisMgr:getDiathesisInfo();
	if GameWizard:isDo(GAMEWIZARD_FIRST_GETGENIUSOPT) and not GameWizard:isDo(GAMEWIZARD_FIRST_ADD_TIAN_FU) and
		diathesisInfo.LearnGeniusPoint > 0 then
		ShowDiathesisFrameXinShouGuide( { text = "双击彩色图标学习天赋" } );
	else
		local t_hideIcons = { "DiathesisFrameXinShouBtn", "DiathesisFrameShowLeaveGeniusValueBackGroundUVAnimationTex",
								"DiathesisFrameXinShouBlinkGeniusIconBtn"
							};
		util.HideIcons( t_hideIcons );
	end
end

-- 设置按钮的显示内容
local szGeniusIconPath = "uires\\TuPianLei\\JiNeng\\TianFu\\";
-- 获取天赋图标路径
function GetGeniusPath( nIconId )
	local szPath = szGeniusIconPath..nIconId..".tga" ;
	return IsInExistence( szPath ) and szPath or DEFAULT_ITEM_PATH;
end

local t_boxTexture = 
{
	gray	= { path = [[uires\ui\Za\TianFuHuiChengLv  40 40.tga]], UV = { x = 0,	y = 0, width = 40, height = 40 }, color = { r = 120, g = 120, b = 120 } },
	orange	= { path = [[uires\ui\Za\TianFuHuiChengLv  40 40.tga]], UV = { x = 40,	y = 0, width = 40, height = 40 }, color = { r = 255, g = 135, b = 37 } },
	green	= { path = [[uires\ui\Za\TianFuHuiChengLv  40 40.tga]], UV = { x = 80,	y = 0, width = 40, height = 40 }, color = { r = 121, g = 255, b = 64 } },
};

function AdjustGeniusBtnColor( id, lv )
	local diathesisiDef = getDiathesisDef( id, lv );
	-- 若已经达到天赋的上限
	if diathesisiDef == nil then
		return t_boxTexture["orange"];
	end
	
	-- 若已经学过一点，则无需判断当前是否可以学习，都置为灰色状态
	if lv > 1 then
		return t_boxTexture["green"];
	end

	-- 现只需检验1点都没学过的天赋
	local mainplayer	= ActorMgr:getMainPlayer();
	local diathesisMgr	= mainplayer:getDiathesisManager();
	local diathesisInfo	= diathesisMgr:getDiathesisInfo();
	-- 若等级不够，天赋点不够，已经用过的天赋点不足
	if diathesisiDef.Learn.LevelMin > mainplayer:getLv() then
		return t_boxTexture["gray"];
	end
	
	-- 若已经使用的天赋点不满足学习1级天赋的要求，
	if diathesisiDef.PreUsedGeniusPoint > diathesisInfo.UsedGeniusPoint then
		return t_boxTexture["gray"];
	end
	
	-- 若前置天赋点不足
	if diathesisiDef.UseGeniusPoint > diathesisInfo.LearnGeniusPoint then
		return t_boxTexture["gray"];
	end
	
	local preDiathesisDef;
	if diathesisiDef.Learn.PreDiathesisID ~= 0 then
		preDiathesisDef = getDiathesisDef( diathesisiDef.Learn.PreDiathesisID,  diathesisiDef.Learn.PreDiathesisLevel );
	end
	
	if preDiathesisDef == nil then
		return t_boxTexture["green"];
	end

	for i = 1, diathesisInfo.DiathesisNum do
		local diathesisData = diathesisInfo.DiathesisList[i-1];
		if diathesisData.DiathesisID == diathesisiDef.Learn.PreDiathesisID then
			-- 若已经学过的前置天赋等级小于学习该天赋所需的前置天赋的最小等级，则不能学习该技能
			if diathesisData.DiathesisLevel < diathesisiDef.Learn.PreDiathesisLevel then
				
				return t_boxTexture["gray"];
			end
			
			return t_boxTexture["green"];
		end
	end
	
	return t_boxTexture["gray"];
end

function UpdateGeniusButtons()
	local mainplayer	= ActorMgr:getMainPlayer()
	local nRace			= mainplayer:getRace()
	if not DiathesisShowType then
		nRace = nRace % 0x10
	end
	-- 设置纹理
	local t_tgas = { ["DiathesisFrameThirdBackGroundTexture"] = { path = t_geniusBackTex[nRace]["path"] }, }
	for name, data in pairs( t_tgas ) do
		local tex = getglobal( name )
		tex:SetTexture( data["path"] )
		tex:SetTexRelUV( 1, 1 )
	end
	-- 再设置按钮的显示内容
	-- 这边要根据是否自动创建按钮来决定是否要重新计算所有按钮相对于ScrollFrame的描点数据
	local t = DiathesisRaceTable[nRace]
	local btn
	for i = 1, 20 do
		if t[i] then
			local id = t[i]
			local lv = t_geniusDatas[id]
			local boxData = AdjustGeniusBtnColor(id, lv + 1)
			local t_textures = {
				["DiathesisFrameScrollFrameIconBtn"..i.."ColorBoxTex"] = { path = boxData["path"], UV = boxData["UV"] },
			}
			util.SetTextures(t_textures)
			
			local tex = getglobal( "DiathesisFrameScrollFrameIconBtn"..i.."Icon" );
			if boxData == t_boxTexture["gray"] then
				tex:SetGray( true )
			else
				tex:SetGray( false )
			end
			btn = getglobal("DiathesisFrameScrollFrameIconBtn"..i)
			btn:Show()
			btn:SetClientUserData( 0, id)
			btn:SetClientUserData( 1, lv)
			local geniusDef = getDiathesisDef(id, 1)
			local t_tgas = { ["DiathesisFrameScrollFrameIconBtn"..i.."Icon"] = { path = GetGeniusPath( geniusDef.IconID ) }, }
			util.SetTgas(t_tgas)
			btn = getglobal("DiathesisFrameScrollFrameGeniusValueBtn"..i)
			btn:Show()
			btn:SetClientUserData( 0, id)
			btn:SetClientUserData( 1, lv)
			
			local font = getglobal(btn:GetName().."Font")
			font:SetText(lv)
			font:SetTextColor(boxData["color"]["r"], boxData["color"]["g"], boxData["color"]["b"])
			
			btn = getglobal("DiathesisFrameScrollFrameNeedTreeArrow"..i)
			if geniusDef.Learn.PreDiathesisID == 0 then
				btn:Hide()
			else
				local upIndex = i - 4
				for j = upIndex, 1, -4 do
					upIndex = j
					if t[j] then
						break
					end
				end
				upIndex = (i - upIndex) / 4
				upIndex = upIndex * 32 + (upIndex - 1) * 44
				btn:Show()
				btn:SetHeight(upIndex)
				btn = getglobal(btn:GetName().."Body")
				btn:SetHeight(upIndex - 30)
			end
		else
			btn = getglobal( "DiathesisFrameScrollFrameIconBtn"..i )
			btn:Hide()
			btn = getglobal( "DiathesisFrameScrollFrameGeniusValueBtn"..i )
			btn:Hide()
			btn = getglobal("DiathesisFrameScrollFrameNeedTreeArrow"..i)
			btn:Hide()
		end
	end
end

-- 点击确定按钮
function DiathesisFrame_ConfirmBtn_OnClick()
end

local t_RaceskillType = {
	[g_nCAREER_WARRIOR]			= "常规类技能",
	[g_nCAREER_MAGICOR]			= "常规类技能",
	[g_nCAREER_GUNER]			= "常规类技能",
	[g_nCAREER_CHURCH]			= "常规类技能",
    
	[g_nCAREER_WARRIOR_SINGEL]	= "刀技能",
	[g_nCAREER_MAGICOR_SINGEL]	= "冰技能",
	[g_nCAREER_GUNER_SINGEL]	= "手枪技能",
	[g_nCAREER_CHURCH_SINGEL]	= "圣技能",
    
	[g_nCAREER_WARRIOR_DOUBLE]	= "剑技能",
	[g_nCAREER_MAGICOR_DOUBLE]	= "火技能",
	[g_nCAREER_GUNER_DOUBLE]	= "机枪技能",
	[g_nCAREER_CHURCH_DOUBLE]	= "雷技能",
}

function DiathesisFrame_CommonSkillLabel_OnEnter()
	local MainPlayer	= ActorMgr:getMainPlayer()
	if not MainPlayer then
		return
	end
	local nRace			= MainPlayer:getRace()
	SetGameTooltips(this:GetName(), t_RaceskillType[nRace % 0x10])
end

function DiathesisFrame_SpecialSkillLabel_OnEnter()
	local MainPlayer = ActorMgr:getMainPlayer()
	if not MainPlayer then
		return
	end
	local nRace			= MainPlayer:getRace()
	SetGameTooltips(this:GetName(), t_RaceskillType[nRace])
end

function DiathesisFrame_CommonSkillLabel_OnClick()
	DiathesisShowType = nil
	DiathesisFrameCommonSkillLabel:Checked()
	DiathesisFrameSpecialSkillLabel:DisChecked()
	UpdateGeniusButtons()
end

function DiathesisFrame_SpecialSkillLabel_OnClick()
	DiathesisShowType = true
	DiathesisFrameCommonSkillLabel:DisChecked()
	DiathesisFrameSpecialSkillLabel:Checked()
	UpdateGeniusButtons()
end