local t_curTmpSystemSetting = {};
local NONE_SHADOW_QUALITY	= 0;
local LOW_SHADOW_QUALITY	= 1;
local MEDIUM_SHADOW_QUALITY	= 1024;
local HIGH_SHADOW_QUALITY	= 2048;
local MAX_SOLUTION_OPTION	= 6; -- 最多的分辨率选项数目
local MIN_UI_SCALE			= 0.5;

local t_kuanPingSolutions = {
								{ width	= 1366, height = 768  },	{ width = 1280, height = 768  },	{ width = 1280, height = 800  },
								{ width = 1440, height = 900  },	{ width = 1680, height = 1050 },	{ width = 1920, height = 1080 },
								{ width = 1920, height = 1200 },	{ width = 2480, height = 1152 },	{ width = 1280, height = 960  },
								{ width = 1280, height = 1024  },	{ width = 900, height = 600  },		{ width = 1280, height = 720  },
							};

local t_selectSolutions = {};

t_selectSolutions["isSizeExist"] = 
function ( self, data )
	for _, existData in ipairs( self ) do
		if data["width"] == existData["width"] and data["height"] == existData["height"] then
			return true;
		end
	end

	return false;
end

local t_fpsChkBtnConfig = 
{
["SystemSettingFrameFPS15ChkBtn"] = { ["btn"] = "SystemSettingFrameFPS15HeadBtn", ["fps"] = 15 },
["SystemSettingFrameFPS30ChkBtn"] = { ["btn"] = "SystemSettingFrameFPS30HeadBtn", ["fps"] = 30 },
["SystemSettingFrameFPS45ChkBtn"] = { ["btn"] = "SystemSettingFrameFPS45HeadBtn", ["fps"] = 45 },
["SystemSettingFrameFPS60ChkBtn"] = { ["btn"] = "SystemSettingFrameFPS60HeadBtn", ["fps"] = 60 },
};

function GetFpsChkBtnByFpsValue( data )
	local nFpsValue = data["fps"];
	for name, oneConfig in pairs( t_fpsChkBtnConfig ) do
		if oneConfig["fps"] == nFpsValue then
			return name;
		end
	end

	return "SystemSettingFrameFPS15ChkBtn";
end

function GetFpsChkBtnByBtn( data )
	local btnName = data["name"];
	for name, oneConfig in pairs( t_fpsChkBtnConfig ) do
		if oneConfig["btn"] == btnName then
			return name;
		end
	end

	return "SystemSettingFrameFPS15ChkBtn";
end

function GetFpsValueByChkBtn( data )
	local btnName = data["chkBtn"];
	for name, oneConfig in pairs( t_fpsChkBtnConfig ) do
		if name == btnName then
			return oneConfig["fps"];
		end
	end

	return 30;
end

local t_systemSettingBtnDescMap = 
{
-- 垂直同步
["SystemSettingFrameVerticalHeadBtn"]		= { desc = "将你的游戏帧数与显示器刷新率同步。可以解决游戏中图像无法显示的问题" },

-- 窗口模式
["SystemSettingFrameWindowedHeadBtn"]		= { desc = "以窗口模式进行游戏" },

-- 全屏模式
["SystemSettingFrameFullScreenHeadBtn"]		= { desc = "勾选此选项，将以全屏模式运行游戏，并去除游戏边框" },

-- 阴影质量
["SystemSettingFrameShadowQualityHeadBtn"]	= { desc = "勾选此选项，将使地面上显示各种背景物体的影子，会影响运行速度，对显存小的显卡，建议关闭，可以显著提高游戏运行速度" },

-- 水面质量
["SystemSettingFrameOpenWaterQualityHeadBtn"]= { desc = "勾选此选项，将在水面中倒影出周围场景物体，会影响运行速度，对显存小的显卡，建议关闭，可以显著提高游戏运行速度\n#G切换场景才能生效" },

-- 屏幕泛光
["SystemSettingFrameLightHeadBtn"]			= { desc = "勾选此选项，将开启柔和光影效果，关闭这个选项可以提高运行速度" },

-- 屏幕特效
["SystemSettingFrameEffectHeadBtn"]			= { desc = "勾选此选项，提高光效视觉冲击力，会影响运行速度。对显存小的显卡，建议关闭，可以显著提高游戏运行速度" },

-- 全屏抗锯齿
["SystemSettingFrameAtialiaseHeadBtn"]		= { desc = "更高的抗锯齿可以使角色以及背景物体的边缘表现得更平滑，但是会降低游戏的运行速度，对显存小的显卡尤其明显，建议关闭" },

-- 背景音乐
["SystemSettingFrameBackgroundMusicHeadBtn"]= { desc = "影响场景的背景音乐" },

-- 音效设置
["SystemSettingFrameEffectMusicHeadBtn"]	= { desc = "影响场景中环境音效，技能使用时的音效，事件所触发的各种音效" },
};

function SystemSettingFrameBtn_OnEnter()
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = t_systemSettingBtnDescMap[this:GetName()]["desc"], 
						frame = this:GetParent(), button = this:GetName() } );
end

-- 点击系统设置面板的关闭按钮响应
function SystemSettingFrame_CloseButton_OnClick()
	SystemSettingFrame:Hide();
end

-- 显示设置按钮
function SystemSettingFrame_XianShiSettingButton_OnClick()
	local curShowFrame = getglobal( this:GetParent() );
	GameSettingFrame:SetPoint( "topleft", "UIClient", "topleft", curShowFrame:GetRealLeft(), curShowFrame:GetRealTop() );
	GameSettingFrame:Show();
	curShowFrame:Hide();
end

-- 鼠标设置按钮
function SystemSettingFrame_MouseSettingButton_OnClick()
	local curShowFrame = getglobal( this:GetParent() );
	MouseSettingFrame:SetPoint( "topleft", "UIClient", "topleft", curShowFrame:GetRealLeft(), curShowFrame:GetRealTop() );
	MouseSettingFrame:Show();
	curShowFrame:Hide();
end

-- 按键设置按钮
function SystemSettingFrame_KeySettingButton_OnClick()
	local curShowFrame = getglobal(this:GetParent());
	KeySettingFrame:SetPoint( "topleft", "UIClient", "topleft", curShowFrame:GetRealLeft(), curShowFrame:GetRealTop() );
	KeySettingFrame:Show();
	curShowFrame:Hide();
end

-- 点击垂直同步check button时候响应
function SystemSettingFrameVerticalChkBtn_OnClick()
	local chkBtn = getglobal( "SystemSettingFrameVerticalChkBtn" );
	t_curTmpSystemSetting["sync_refresh"] = chkBtn:GetCheckState();
end

function SystemSettingFrameVerticalHeadBtn_OnClick()
	local chkBtn = getglobal( "SystemSettingFrameVerticalChkBtn" );
	chkBtn:setCheckState( not chkBtn:GetCheckState() );
	SystemSettingFrameVerticalChkBtn_OnClick();
end

-- 点击分辨率选择下拉按钮响应
function SystemSettingFrameSolutionDownBtn_OnClick()
	local selectFrame = getglobal( "SystemSettingFrame_SelectBtn" );
	if selectFrame:IsShown() then
		selectFrame:Hide();
	else
		selectFrame:Show();
	end
end

-- 鼠标点击分辨率下拉框选择条
function SystemSettingFrame_LayerBtn_OnClick()
	local font			= getglobal( this:GetName() .. "LayerName" );
	local szSolution	= font:GetText();
	local pivotPos		= string.find( szSolution, "*" );
	local leftBracket	= string.find( szSolution, "（" );
	local width			= tonumber( string.sub( szSolution, 1, pivotPos - 1 ) );
	local height		= tonumber( string.sub( szSolution, pivotPos + 1, leftBracket ~= nil and ( leftBracket - 1 ) or -1 ) );
	local nMode			= this:GetClientID();
	local systemSetting	= GameMgr:getUserSystemConfig();

	t_curTmpSystemSetting["width"]	= systemSetting:getScreenModeWidth( t_curTmpSystemSetting.fullscreen,	width, height );
	t_curTmpSystemSetting["height"] = systemSetting:getScreenModeHeight( t_curTmpSystemSetting.fullscreen,	width, height );
	
	SystemSettingFrameSolutionSelect_OneLayerName:SetText( t_curTmpSystemSetting.width .. "*" .. t_curTmpSystemSetting.height );
	SystemSettingFrame_SelectBtn:Hide();
end


function UpdateSolutionFunction()
	local t_solutionControlEnableBtns	= { "SystemSettingFrameSolutionHeadBtn", "SystemSettingFrameSolutionSelect_OneLayerBtn", };
	local t_solutionControlShowBtns		= { "SystemSettingFrameSolutionDownBtn" };
	if SystemSettingFrameFullScreenChkBtn:GetCheckState() then
		for _, name in ipairs( t_solutionControlEnableBtns ) do
			local btn = getglobal( name );
			btn:Disable()
		end

		for _, name in ipairs( t_solutionControlShowBtns ) do
			local btn = getglobal( name );
			btn:Hide()
		end
	else
		for _, name in ipairs( t_solutionControlEnableBtns ) do
			local btn = getglobal( name );
			btn:Enable()
		end

		for _, name in ipairs( t_solutionControlShowBtns ) do
			local btn = getglobal( name );
			btn:Show()
		end
	end
end

-- 窗口模式点选按钮
function SystemSettingFrameWindowedChkBtn_OnClick()
	local chkBtn = getglobal( "SystemSettingFrameWindowedChkBtn" );
	-- 当窗口模式或全屏模式一个按钮处于选中状态时候,则只能通过另一个按钮进行取消操作
	SystemSettingFrameFullScreenChkBtn:setCheckState( false );
	SystemSettingFrameFullScreenChkBtn:Enable();
	chkBtn:Disable();

	t_curTmpSystemSetting.fullscreen = false;
	UpdateSolutionFunction();
end

function SystemSettingFrameWindowedHeadBtn_OnClick()
	local chkBtn = getglobal( "SystemSettingFrameWindowedChkBtn" );
	if not chkBtn:IsEnable() then
		return;
	end

	chkBtn:setCheckState( not chkBtn:GetCheckState() );
	SystemSettingFrameWindowedChkBtn_OnClick();
end

-- 全屏模式点选按钮
function SystemSettingFrameFullScreenChkBtn_OnClick()
	local chkBtn = getglobal( "SystemSettingFrameFullScreenChkBtn" );
	-- 当窗口模式或全屏模式一个按钮处于选中状态时候,则只能通过另一个按钮进行取消操作
	SystemSettingFrameWindowedChkBtn:setCheckState( false );
	SystemSettingFrameWindowedChkBtn:Enable();
	chkBtn:Disable();

	local font			= getglobal( "SystemSettingFrameSolutionSelect_OneLayerName" );
	local szSolution	= font:GetText();
	local pivotPos		= string.find( szSolution, "*" );
	local width			= tonumber( string.sub( szSolution, 1, pivotPos - 1 ) );
	local height		= tonumber( string.sub( szSolution, pivotPos + 1 ) );

	local systemSetting	= GameMgr:getUserSystemConfig();
	t_curTmpSystemSetting.fullscreen	= true;
	--t_curTmpSystemSetting["width"]		= systemSetting:getScreenModeWidth( t_curTmpSystemSetting.fullscreen,	width, height );
	--t_curTmpSystemSetting["height"]		= systemSetting:getScreenModeHeight( t_curTmpSystemSetting.fullscreen,	width, height );

	t_curTmpSystemSetting["width"]		= systemSetting:getCurScreenWidth();
	t_curTmpSystemSetting["height"]		= systemSetting:getCurScreenHeight();
	SystemSettingFrameSolutionSelect_OneLayerName:SetText( t_curTmpSystemSetting.width .. "*" .. t_curTmpSystemSetting.height );
	UpdateSolutionFunction();
end

function SystemSettingFrameFullScreenHeadBtn_OnClick()
	local chkBtn = getglobal( "SystemSettingFrameFullScreenChkBtn" );
	if not chkBtn:IsEnable() then
		return;
	end

	chkBtn:setCheckState( not chkBtn:GetCheckState() );
	SystemSettingFrameFullScreenChkBtn_OnClick();
end

-- 屏幕泛光勾选按钮
function SystemSettingFrameLightChkBtn_OnClick()
	local chkBtn = getglobal( "SystemSettingFrameLightChkBtn" );
	t_curTmpSystemSetting["bloom"] = chkBtn:GetCheckState();
end

function SystemSettingFrameLightHeadBtn_OnClick()
	local chkBtn = getglobal( "SystemSettingFrameLightChkBtn" );
	chkBtn:setCheckState( not chkBtn:GetCheckState() );
	SystemSettingFrameLightChkBtn_OnClick();
end

-- 屏幕特效勾选按钮
function SystemSettingFrameEffectChkBtn_OnClick()
	local chkBtn = getglobal( "SystemSettingFrameEffectChkBtn" );
	t_curTmpSystemSetting["distort"] = chkBtn:GetCheckState();
end

function SystemSettingFrameEffectHeadBtn_OnClick()
	local chkBtn = getglobal( "SystemSettingFrameEffectChkBtn" );
	chkBtn:setCheckState( not chkBtn:GetCheckState() );
	SystemSettingFrameEffectChkBtn_OnClick();
end

-- 开启水面质量选择按钮
function SystemSettingFrameOpenWaterQualityChkBtn_OnClick()
	local chkBtn = getglobal( "SystemSettingFrameOpenWaterQualityChkBtn" );
	t_curTmpSystemSetting["waterreflect"] = chkBtn:GetCheckState();
end

function SystemSettingFrameOpenWaterQualityHeadBtn_OnClick()
	local chkBtn = getglobal( "SystemSettingFrameOpenWaterQualityChkBtn" );
	chkBtn:setCheckState( not chkBtn:GetCheckState() );
	SystemSettingFrameOpenWaterQualityChkBtn_OnClick();
end

function isKuanPing( nWidth, nHeight )
	for _, data in ipairs( t_kuanPingSolutions ) do
		if data.width == nWidth and data.height == nHeight then
			return true;
		end
	end

	return false;
end

-- 分辨率选项框显示的时候调用
function SystemSettingFrame_SelectBtn_OnShow()
	for i = 1, MAX_SOLUTION_OPTION do
		local btn = getglobal( "SystemSettingFrame_SoluOpBtn" .. i );
		btn:Hide();
	end
	
	local systemSetting		= GameMgr:getUserSystemConfig();
	local nCurDeskWidth		= systemSetting:getCurScreenWidth();
	local nCurDeskHeight	= systemSetting:getCurScreenHeight();
	local nButtonIndex		= 1;
	local bFullScreenSizeSet = false;

	for i = 1, table.getn( t_selectSolutions ) do
		-- 要求所显示的分辨率必须不超过当前桌面的分辨率
		if nButtonIndex == MAX_SOLUTION_OPTION then
			break;
		end

		local btn	= getglobal( "SystemSettingFrame_SoluOpBtn" .. nButtonIndex );
		local font	= getglobal( "SystemSettingFrame_SoluOpBtn" .. nButtonIndex .. "LayerName" );
		if t_selectSolutions[i].width == nCurDeskWidth and t_selectSolutions[i].height == nCurDeskHeight then
			bFullScreenSizeSet = true;
		end

		local bCanSet = SetSolution( font, {	width = t_selectSolutions[i].width, height = t_selectSolutions[i].height, maxWidth = nCurDeskWidth,
												maxHeight = nCurDeskHeight } );
		
		if bCanSet then
			btn:Show();
			nButtonIndex = nButtonIndex + 1;
		end
	end
	
	if not bFullScreenSizeSet then
		local btn	= getglobal( "SystemSettingFrame_SoluOpBtn" .. nButtonIndex );
		local font	= getglobal( "SystemSettingFrame_SoluOpBtn" .. nButtonIndex .. "LayerName" );
		SetSolution( font, {	width		= nCurDeskWidth,	height		= nCurDeskHeight, 
								maxWidth	= nCurDeskWidth,	maxHeight	= nCurDeskHeight } );
		btn:Show();
		nButtonIndex = nButtonIndex + 1;
	end	
	SystemSettingFrame_SelectBtn:SetHeight( (nButtonIndex-1) * 22 );
end

-- 阴影质量选项框
local t_shadowQuality = {	{ desc = "低", value = NONE_SHADOW_QUALITY		}, { desc = "中", value = LOW_SHADOW_QUALITY }, 
							{ desc = "高", value = HIGH_SHADOW_QUALITY	}, 
						};

function GetShadowQualityValue( data )
	local szDesc = data["desc"];
	for i = 1, table.getn( t_shadowQuality ) do
		if t_shadowQuality[i].desc == szDesc then
			return t_shadowQuality[i].value;
		end
	end

	return NONE_SHADOW_QUALITY;
end

function GetShadowQualityDesc( value )
	for i = 1, table.getn( t_shadowQuality ) do
		if t_shadowQuality[i].value == value then
			return t_shadowQuality[i].desc;
		end
	end

	return "无";
end

function SystemSettingFrameShadowQualityBtn_OnClick()
	local selectFrame = getglobal( "SystemSettingFrame_SelectShadowQualityBtn" );
	if selectFrame:IsShown() then
		selectFrame:Hide();
	else
		selectFrame:Show();
	end
end

function SystemSettingFrame_SelectShadowQualityBtn_OnShow()
	for i = 1, table.getn( t_shadowQuality ) do
		local font = getglobal( "SystemSettingFrame_ShadowQualityOptBtn"..i.."Font" );
		font:SetText( t_shadowQuality[i].desc );
	end
end

function SystemSettingFrame_ShadowQualityOptBtn_OnClick()
	local font = getglobal( this:GetName().."Font" );
	t_curTmpSystemSetting["shadowmap"] = GetShadowQualityValue( { desc = font:GetText() } );
	SystemSettingFrameShadowQualitySelect_OneLayerName:SetText( font:GetText() );
	SystemSettingFrame_SelectShadowQualityBtn:Hide();
end

local t_antialiaseQuality = { { desc = "无", value = 0 }, { desc = "低", value = 1 }, 
							{ desc = "中", value = 2 }, { desc = "高", value = 3 } };

function GetAtialiaseQuality( nValue )
	for i = 1, table.getn( t_antialiaseQuality ) do
		if t_antialiaseQuality[i].value == nValue then
			return t_antialiaseQuality[i].desc;
		end
	end

	return "无";
end

function SystemSettingFrame_SelectAtialiaseBtn_OnShow()
	for i = 1, table.getn( t_antialiaseQuality ) do
		local font = getglobal( "SystemSettingFrame_AtialiaseOpBtn"..i.."Font" );
		font:SetText( t_antialiaseQuality[i].desc );
	end
end

function SystemSettingFrame_AtialiaseOpBtn_OnClick()
	local font = getglobal( this:GetName().."Font" );
	local szText = font:GetText();
	for i = 1, table.getn( t_antialiaseQuality ) do
		if t_antialiaseQuality[i].desc == szText then
			t_curTmpSystemSetting["multisample"] = t_antialiaseQuality[i].value;
			break;
		end
	end

	SystemSettingFrameAtialiaseSelect_OneLayerName:SetText( szText );
	SystemSettingFrame_SelectAtialiaseBtn:Hide();
end

function SetSolution( font, data )
	if data.width > data.maxWidth or data.height > data.maxHeight then
		return false;
	end

	if isKuanPing( data.width, data.height ) then
		font:SetText( data.width .. "*" .. data.height .. "（宽屏）" );
	else
		font:SetText( data.width .. "*" .. data.height  );
	end

	return true;
end

-- 开启阴影质量按钮
function SystemSettingFrameShadowQualityChkBtn_OnClick()
	local chkBtn = getglobal( "SystemSettingFrameShadowQualityChkBtn" );
	t_curTmpSystemSetting["shadowmap"] = chkBtn:GetCheckState() and HIGH_SHADOW_QUALITY or LOW_SHADOW_QUALITY;
end

function SystemSettingFrameShadowQualityHeadBtn_OnClick()
	local chkBtn = getglobal( "SystemSettingFrameShadowQualityChkBtn" );
	chkBtn:setCheckState( not chkBtn:GetCheckState() );

	SystemSettingFrameShadowQualityChkBtn_OnClick();
end

-- 选择阴影质量按钮
function SystemSettingFrameAtialiaseDownBtn_OnClick()
	local selectFrame = getglobal( "SystemSettingFrame_SelectAtialiaseBtn" );
	if selectFrame:IsShown() then
		selectFrame:Hide();
	else
		selectFrame:Show();
	end
end

function SystemSettingFrameBackgroundMusic_ScrollBar_OnValueChanged()
	local curUserConfig = GameMgr:getUserSystemConfig();
	if curUserConfig.m_bMute then
		return;
	end
	GameMgr:setGlobalMusicVolume( this:GetValue() / this:GetMaxValue() ); 
end

function SystemSettingFrameEffectMusic_ScrollBa_OnValueChanged()
	local curUserConfig = GameMgr:getUserSystemConfig();
	if curUserConfig.m_bMute then
		return;
	end

	GameMgr:setGlobalSoundVolume( this:GetValue() / this:GetMaxValue() ); 
end

-- 全部静音
function SystemSettingFrameForbitAllNoiseChkBtn_OnClick()
end

-- 最低配置
function SystemSettingFrame_LowestConfigButton_OnClick()
	local userConfig			= GameMgr:getLatestUserSystemConfig();
	--userConfig.m_bFullScreen	= false;
	userConfig.m_bDistort		= false;
	userConfig.m_bBloom			= false;
	userConfig.m_bWaterReflect	= false;
	userConfig.m_bSyncRefresh	= true;
	--userConfig.m_nWinWidth			= t_curTmpSystemSetting["width"]		or userConfig.m_nWinWidth;
	--userConfig.m_nWinHeight			= t_curTmpSystemSetting["height"]		or userConfig.m_nWinHeight;
	userConfig.m_nShadowmapSize			= NONE_SHADOW_QUALITY;
	userConfig.m_nMultiSampleLevel		= 0;
	userConfig.m_nFPS					= 30;
	GameMgr:ApplyNewUserConfig( userConfig );
	UpdateSystemSettingFrame();
	SetGameVolume();
end

-- 最高配置
function SystemSettingFrame_HighestConfigButton_OnClick()
	local userConfig			= GameMgr:getLatestUserSystemConfig();
	--userConfig.m_bFullScreen	= false;
	userConfig.m_bDistort		= true;
	userConfig.m_bBloom			= true;
	userConfig.m_bWaterReflect	= true;
	--userConfig.m_bSyncRefresh	= false;
	--userConfig.m_nWinWidth			= t_curTmpSystemSetting["width"]		or userConfig.m_nWinWidth;
	--userConfig.m_nWinHeight			= t_curTmpSystemSetting["height"]		or userConfig.m_nWinHeight;
	userConfig.m_nShadowmapSize			= HIGH_SHADOW_QUALITY;
	userConfig.m_nMultiSampleLevel		= 0;
	userConfig.m_nFPS					= 60;
	GameMgr:ApplyNewUserConfig( userConfig );
	UpdateSystemSettingFrame();
	SetGameVolume();
end

-- 点击默认设置按钮响应
function SystemSettingFrameDefaultSettingBtn_OnClick()	
	GameMgr:ApplyDefaultUserConfig();
	t_curTmpSystemSetting = {};
	UpdateSystemSettingFrame();
	--SystemSettingFrame:Hide();
	
	--GameMgr:startCheckPerformace();
end

-- 点击保存按钮响应
function SystemSettingFrameSaveBtn_OnClick()
	local userConfig = GameMgr:getLatestUserSystemConfig();
	if t_curTmpSystemSetting.fullscreen ~= nil then
		userConfig.m_bFullScreen = t_curTmpSystemSetting.fullscreen;
	end

	if t_curTmpSystemSetting.distort ~= nil then
		userConfig.m_bDistort = t_curTmpSystemSetting.distort;
	end

	if t_curTmpSystemSetting.bloom ~= nil then
		userConfig.m_bBloom = t_curTmpSystemSetting.bloom;
	end

	if t_curTmpSystemSetting.waterreflect ~= nil then
		userConfig.m_bWaterReflect = t_curTmpSystemSetting.waterreflect;
	end

	if t_curTmpSystemSetting.sync_refresh ~= nil then
		userConfig.m_bSyncRefresh = t_curTmpSystemSetting.sync_refresh;
	end

	userConfig.m_nWinWidth			= t_curTmpSystemSetting["width"]		or userConfig.m_nWinWidth;
	userConfig.m_nWinHeight			= t_curTmpSystemSetting["height"]		or userConfig.m_nWinHeight;
	userConfig.m_nShadowmapSize		= t_curTmpSystemSetting["shadowmap"]	or userConfig.m_nShadowmapSize;
	userConfig.m_nMultiSampleLevel	= t_curTmpSystemSetting["multisample"]	or userConfig.m_nMultiSampleLevel;
	-- 音效
	local slider = getglobal( "SystemSettingFrameEffectMusic_ScrollBar" );
	userConfig.m_fGlobalSoundVolume	= slider:GetValue() / slider:GetMaxValue();
	-- 环境音乐
	slider = getglobal( "SystemSettingFrameBackgroundMusic_ScrollBar" );
	userConfig.m_fGlobalMusicVolume	= slider:GetValue() / slider:GetMaxValue();
	userConfig.m_bMute = SystemSettingFrameForbitAllNoiseChkBtn:GetCheckState();
	
	slider = getglobal( "SystemSettingFrameJieMianSuoFangLv_ScrollBar" );
	userConfig.m_fUIScale	= slider:GetValue() / slider:GetMaxValue() * MIN_UI_SCALE + MIN_UI_SCALE;

	slider = getglobal( "SystemSettingFrameViewSize_ScrollBar" );
	userConfig.m_fViewSizeScale = slider:GetValue() / slider:GetMaxValue() + 0.5;
	
	userConfig.m_nFPS = GetFpsValueByChkBtn( { ["chkBtn"] = GetSelectFPSChkBtn() } );
	GameMgr:ApplyNewUserConfig( userConfig );
	t_curTmpSystemSetting = {};
	UpdateSystemSettingFrame();

	SetGameVolume();
end

function GetSelectFPSChkBtn()
	for name, oneConfig in pairs( t_fpsChkBtnConfig ) do
		local chkBtn = getglobal( name );
		if chkBtn:GetCheckState() then
			return name;
		end
	end

	return "SystemSettingFrameFPS30ChkBtn";
end

-- 点击FPS按钮
function SystemSettingFrameFPSChkBtn_OnClick()
	for name, oneConfig in pairs( t_fpsChkBtnConfig ) do
		local chkBtn = getglobal( name );
		chkBtn:setCheckState( false );
	end

	this:setCheckState( true );
end

function SystemSettingFrameFPSBtn_OnClick()
	for name, oneConfig in pairs( t_fpsChkBtnConfig ) do
		local chkBtn = getglobal( name );
		chkBtn:setCheckState( false );
	end
	
	local selectFpsChkBtn = getglobal( GetFpsChkBtnByBtn( { ["name"] = this:GetName() } ) );
	selectFpsChkBtn:setCheckState( true );
end

local t_hideCheckButtons =	{	-- 垂直同步											
							["SystemSettingFrameVerticalChkBtn"]			= false,
								-- 窗口模式
							["SystemSettingFrameWindowedChkBtn"]			= false,
							-- 全屏模式											
							["SystemSettingFrameFullScreenChkBtn"]			= false,	
							-- 水面质量											
							["SystemSettingFrameOpenWaterQualityChkBtn"]	= false,	
							-- 屏幕泛光
							["SystemSettingFrameLightChkBtn"]				= false,
							-- 屏幕特效											
							["SystemSettingFrameEffectChkBtn"]				= false,		
							};
function SystemSettingFrame_OnHide()
	for name, state in pairs( t_hideCheckButtons ) do
		local btn = getglobal( name );
		btn:setCheckState( state );
	end

	SetGameVolume();
end

function SetGameVolume()
	local curUserConfig = GameMgr:getUserSystemConfig();
	if curUserConfig.m_bMute then
		GameMgr:setGlobalSoundVolume( 0 ); 
		GameMgr:setGlobalMusicVolume( 0 ); 
	else
		GameMgr:setGlobalSoundVolume( curUserConfig.m_fGlobalSoundVolume ); 
		GameMgr:setGlobalMusicVolume( curUserConfig.m_fGlobalMusicVolume ); 
	end
end

function UpdateSystemSettingFrame()
	local curUserConfig = GameMgr:getUserSystemConfig();
	local t_initCheckButtons =	{	
									-- 垂直同步											
									["SystemSettingFrameVerticalChkBtn"]			= curUserConfig.m_bSyncRefresh,	
									-- 窗口模式
									["SystemSettingFrameWindowedChkBtn"]			= not curUserConfig.m_bFullScreen,
									-- 全屏模式														
									["SystemSettingFrameFullScreenChkBtn"]			= curUserConfig.m_bFullScreen,	
									-- 水面质量												
									["SystemSettingFrameOpenWaterQualityChkBtn"]	= curUserConfig.m_bWaterReflect,	
									-- 屏幕泛光
									["SystemSettingFrameLightChkBtn"]				= curUserConfig.m_bBloom ,
									-- 屏幕特效															
									["SystemSettingFrameEffectChkBtn"]				= curUserConfig.m_bDistort,	
									-- 静音															
									["SystemSettingFrameForbitAllNoiseChkBtn"]		= curUserConfig.m_bMute,	
									-- 实时阴影
									--["SystemSettingFrameShadowQualityChkBtn"]		= ( curUserConfig.m_nShadowmapSize == HIGH_SHADOW_QUALITY )
								};

	for name, state in pairs( t_initCheckButtons ) do
		local btn = getglobal( name );
		btn:setCheckState( state );
	end

	local t_fonts = {	["SystemSettingFrameAtialiaseSelect_OneLayerName"]		= { value = GetAtialiaseQuality( curUserConfig.m_nMultiSampleLevel ) },
						["SystemSettingFrameShadowQualitySelect_OneLayerName"]	= { value = GetShadowQualityDesc( curUserConfig.m_nShadowmapSize ) },
		
					};
	for name, data in pairs( t_fonts ) do
		local font = getglobal( name );
		font:SetText( data.value );
	end

	local t_disableButtons =	{	"SystemSettingFrameFullScreenChkBtn",			"SystemSettingFrameWindowedChkBtn",	
								};
	for _, name in ipairs( t_disableButtons ) do
		local btn = getglobal( name );
		if btn:GetCheckState() then
			btn:Disable();
		else
			btn:Enable();
		end
	end

	
	t_curTmpSystemSetting = {	["fullscreen"]		= curUserConfig.m_bFullScreen,		["bloom"]			= curUserConfig.m_bBloom,
								["distort"]			= curUserConfig.m_bDistort,			["waterreflect"]	= curUserConfig.m_bWaterReflect, 
								["sync_refresh"]	= curUserConfig.m_bSyncRefresh,		["multisample"]		= curUserConfig.m_nMultiSampleLevel,
								["width"]			= curUserConfig.m_nWinWidth,		["height"]			= curUserConfig.m_nWinHeight, 
								["shadowmap"]		= curUserConfig.m_nShadowmapSize,
							};
		
	local szSolution	= SystemSettingFrameSolutionSelect_OneLayerName:SetText( 
							tostring( curUserConfig.m_nWinWidth ) .. "*" .. tostring( curUserConfig.m_nWinHeight ) );

	local slider = getglobal( "SystemSettingFrameBackgroundMusic_ScrollBar" );
	slider:SetMaxValue( 100 );
	slider:SetValue( slider:GetMaxValue() * curUserConfig.m_fGlobalMusicVolume );
	slider:SetMinValue( 0 );
	slider:SetValueStep( 1 );

	slider = getglobal( "SystemSettingFrameEffectMusic_ScrollBar" );
	slider:SetMaxValue( 100 );
	slider:SetValue( slider:GetMaxValue() * curUserConfig.m_fGlobalSoundVolume );
	slider:SetMinValue( 0 );
	slider:SetValueStep( 1 );

	slider = getglobal( "SystemSettingFrameJieMianSuoFangLv_ScrollBar" );
	slider:SetMaxValue( 100 );
	slider:SetValue( slider:GetMaxValue() * ( ( curUserConfig.m_fUIScale - MIN_UI_SCALE ) / MIN_UI_SCALE ) );
	slider:SetMinValue( 0 );
	slider:SetValueStep( 1 );

	slider = getglobal( "SystemSettingFrameViewSize_ScrollBar" );
	slider:SetMaxValue( 100 );
	slider:SetValue( slider:GetMaxValue() * ( (curUserConfig.m_fViewSizeScale - 0.5 ) ) );
	slider:SetMinValue( 0 );
	slider:SetValueStep( 1 );

	UpdateSolutionFunction();

	for name, oneConfig in pairs( t_fpsChkBtnConfig ) do
		local chkBtn = getglobal( name );
		chkBtn:setCheckState( false );
	end

	local selectFpsChkBtn = getglobal( GetFpsChkBtnByFpsValue( { ["fps"] = curUserConfig.m_nFPS } ) );
	selectFpsChkBtn:setCheckState( true );
end

function SystemSettingFrame_OnShow()
	UpdateSystemSettingFrame();

	local t_btns = { this:GetName().."_ShiPinSettingButton", this:GetName().."_XianShiSettingButton", this:GetName().."_MouseSettingButton", };
	util.DischeckBtns( t_btns );
	
	t_btns = { this:GetName().."_ShiPinSettingButton", };
	util.CheckBtns( t_btns );
end

local t_SystemSettingFramControl = { startTime = 0, MAX_UPDATE_INTERVAL = 5 };

function GetSystemSettingFramControl()
	return t_SystemSettingFramControl;
end

t_SystemSettingFramControl["init"] =
function ( self )
	self["startTime"] = 0;
end

t_SystemSettingFramControl["update"] =
function ( self )
	if getGameState() ~= STATE_INWORLD or GameMgr:isInPlayCamerAnim() then 
		return 
	end

	local fCurTime = os.clock();
	if fCurTime - self["startTime"] < self["MAX_UPDATE_INTERVAL"] then
		return;
	end
	
	self["startTime"] = fCurTime;
	-- 若超过10s,则开始自动切换分辨率
	local t_selectCardOption	= self:getNewScreenSize();
	local userConfig			= GameMgr:getLatestUserSystemConfig();
	userConfig.m_nWinWidth		= t_selectCardOption["width"];
	userConfig.m_nWinHeight		= t_selectCardOption["height"];
	GameMgr:ApplyNewUserConfig( userConfig );
end

t_SystemSettingFramControl["getNewScreenSize"] = 
function ( self )
	-- 分辨率变化策略是优先查找还没使用的分辨率
	local userConfig		= GameMgr:getLatestUserSystemConfig();
	local nCurWidth			= userConfig.m_nWinWidth;
	local nCurHeight		= userConfig.m_nWinHeight;
	-- 若当前分辨率是最后一个分辨率，则
	local lastCardOption	= t_selectSolutions[table.getn( t_selectSolutions )];
	if lastCardOption["width"] == nCurWidth and lastCardOption["height"] == nCurHeight then
		return { ["width"] = t_selectSolutions[1]["width"], ["height"] = t_selectSolutions[1]["height"] };
	end
	
	local nCurIndex = 1;
	for i = 1, table.getn( t_selectSolutions ) do
		local oneCardOption = t_selectSolutions[i];
		if oneCardOption["width"] == nCurWidth and oneCardOption["height"] == nCurHeight then
			nCurIndex = i;
			break;
		end
	end

	local nSelectIndex = nCurIndex + 1;

	return { ["width"] = t_selectSolutions[nSelectIndex]["width"], ["height"] = t_selectSolutions[nSelectIndex]["height"] };
end

function SystemSettingFrame_OnLoad()
	local systemSetting	= GameMgr:getUserSystemConfig()
	
	-- 先获取最新的
	for i = 1, systemSetting:getNumScreenMode()do
		local nWidth	= systemSetting:getScreenModeWidth( i - 1 );
		local nHeight	= systemSetting:getScreenModeHeight( i - 1 );
		if not t_selectSolutions:isSizeExist( { width = nWidth, height = nHeight } ) then
			table.insert( t_selectSolutions, { width = nWidth, height = nHeight } );
		end
	end
end

-- 游戏设置

local t_buttonMap = 
{
	-- 显示所有玩家/宠物名字
	["GameSettingFrameShowPlayerNameChkBtn"]	= { index = GAME_OPT_SHOW_ALLPLAYER_AND_PET_NAME + 1, },
	-- 显示所有玩家包括主角的称号名
	["GameSettingFrameShowTeamNameChkBtn"]		= { index = GAME_OPT_SHOW_ALLPLAYER_TEAM_DESIGNATION_NAME + 1, },
	-- 显示召唤兽名
	--["GameSettingFrameShowPetNameChkBtn"]		= { index = GAME_OPT_SHOW_MAINPET_NAME + 1, },
	-- 显示怪物名
	["GameSettingFrameShowMonsterNameChkBtn"]	= { index = GAME_OPT_SHOW_MONSTER_NAME + 1, },
	-- 显示怪物血量
	["GameSettingFrameShowMonsterHPChkBtn"]		= { index = GAME_OPT_SHOW_MONSTER_HP + 1, },
	-- 显示其他玩家血量
	["GameSettingFrameShowOtherPlayerHPChkBtn"] = { index = GAME_OPT_SHOW_OTHERPLAYER_HP + 1, },
	-- 显示其他召唤兽血量
	--["GameSettingFrameShowOtherPetHPChkBtn"]	= { index = GAME_OPT_SHOW_OTHERPLAYER_PET_HP + 1, },
	-- 显示本人/队友生命值
	["GameSettingFrameShowMeHPChkBtn"]			= { index = GAME_OPT_SHOW_MAINPLAYER_AND_TEAM_HP + 1, },
	-- 锁定技能栏
	["GameSettingFrameLockShortCutChkBtn"]		= { index = GAME_OPT_LOCK_SHORTCUT + 1, },
	-- 禁止选中宠物
	["GameSettingFrameForbitSelectPetChkBtn"]	= { index = GAME_OPT_FORBIT_SELECT_PET + 1, },
	--技能自动释放
	["GameSettingFrameAutoSkillReleaseChkBtn"]      = { index = GAME_OPT_AUTO_SKILL_RELEASE + 1, reverse = true },
	-- 显示玩家气泡
	["GameSettingFrameShowPlayerTalkChkBtn"]	= { index = GAME_OPT_SHOW_OTHERPLAYER_TALK_POP + 1, },
	-- 显示伤害飘字
	["GameSettingFrameShowDamageChkBtn"]		= { index = GAME_OPT_SHOW_MAINPLAYER_DAMAGE_SHOW + 1, },
	-- 显示装备比较
	["GameSettingFrameShowCompareEquipChkBtn"]	= { index = GAME_OPT_SHOW_EQUIP_SHOW + 1, },
	-- 显示拒绝好友申请
	["GameSettingFrameRefuseFriendReqChkBtn"]	= { index = GAME_OPT_REFUSE_ADD_FRIEND_REQ + 1, },
	-- 拒绝公会邀请
	["GameSettingFrameRefuseClanInviteChkBtn"]	= { index = GAME_OPT_REFUSE_CLAN_INVITE + 1, },
	-- 拒绝交易申请
	["GameSettingFrameRefuseTradeInviteChkBtn"] = { index = GAME_OPT_REFUSE_TRADE_REQ + 1, },
	-- 拒绝骑乘邀请
	["GameSettingFrameRefuseRideInviteChkBtn"]	= { index = GAME_OPT_REFUSE_RIDE_INVITE + 1, },
	-- 拒绝组队邀请
	["GameSettingFrameRefuseTeamInviteChkBtn"]	= { index = GAME_OPT_REFUSE_TEAM_INVITE + 1, },
	-- 拒绝查看装备
	["GameSettingFrameRefuseSeeEquipChkBtn"]	= { index = GAME_OPT_REFUSE_SEE_EQUIP + 1, },
	-- 拒绝切磋邀请
	["GameSettingFrameRefusePKInviteChkBtn"]	= { index = GAME_OPT_REFUSE_REFUSE_PK + 1, },
	-- 显示本人公会名字
	["GameSettingFrameShowSelfClanNameChkBtn"]	= { index = GAME_OPT_SHOW_MAIN_CLAN_NAME + 1, },
	-- 显示其他人公会名字
	["GameSettingFrameShowOtherPlayerClanNameChkBtn"]	= { index = GAME_OPT_SHOW_OTHERPLAYER_CLAN_NAME + 1, },
	-- 显示完美星特效
	["GameSettingFramePerfectStarChkBtn"]		= { index = GAME_OPT_SHOW_PERFECT_STAR_EFFECT + 1, reverse = true },
	-- 显示特权徽章
	["GameSettingFrameStarStoneChkBtn"]		= { index = GAME_OPT_SHOW_STAR_STONE + 1, reverse = true },
	--使用3D解锁功能
	["GameSettingFrameLookUpDownChkBtn"]	= { index = GAME_OPT_CAMERA_3D+1, reverse = false },
};

function GameSettingFrame_OnLoad()
	this:RegisterEvent( "GE_UPDATE_GAME_SETTING" );
	this:RegisterEvent( "GE_LOGIN_GAME_SETTING" );
end

function GameSettingFrame_OnEvent()
	if arg1 == "GE_UPDATE_GAME_SETTING" then
		UpdateGameSettingFrame();
	end

	if arg1 == "GE_LOGIN_GAME_SETTING" then
		if not GameWizard:isDo( GAMEWIZARD_FIRST_SETGAMEOPT ) then 
			-- 若为第一次登录游戏，则给游戏一个默认设置
			local systemSetting	= GameMgr:getUserSystemConfig();
			systemSetting:setDefaultGameOpt();
			GameWizard:doStep( GAMEWIZARD_FIRST_SETGAMEOPT );
		end
	end
end

function GameSettingFrame_OnShow()
	UpdateGameSettingFrame();

	local t_btns = { this:GetName().."_ShiPinSettingButton", this:GetName().."_XianShiSettingButton", this:GetName().."_MouseSettingButton", };
	util.DischeckBtns( t_btns );
	
	t_btns = { this:GetName().."_XianShiSettingButton", };
	util.CheckBtns( t_btns );
end

function UpdateGameSettingFrame()
	local systemSetting	= GameMgr:getUserSystemConfig();
	for name, data in pairs( t_buttonMap ) do
		local chkBtn = getglobal( name );
		local result = systemSetting:getGameOpt( data["index"] - 1 );
		if data["reverse"] then
			result = not result
		end
		chkBtn:setCheckState( result );
	end
end

local t_optBtnEventsMap = 
{
-- 显示玩家名字
["GameSettingFrameShowPlayerNameHeadBtn"]		= { chkBtn = "GameSettingFrameShowPlayerNameChkBtn", desc = "显示/隐藏所有玩家及宠物的名字" },
-- 显示玩家称号名
["GameSettingFrameShowTeamNameHeadBtn"]			= { chkBtn = "GameSettingFrameShowTeamNameChkBtn", desc = "显示/隐藏所有玩家的称号" },
-- 显示怪物名
["GameSettingFrameShowMonsterNameHeadBtn"]		= { chkBtn = "GameSettingFrameShowMonsterNameChkBtn", desc = "显示/隐藏怪物的名字" },
-- 显示怪物血量
["GameSettingFrameShowMonsterHPHeadBtn"]		= { chkBtn = "GameSettingFrameShowMonsterHPChkBtn", desc = "显示/隐藏怪物的生命值" },
-- 显示其他玩家血量
["GameSettingFrameShowOtherPlayeHPHeadBtn"]		= { chkBtn = "GameSettingFrameShowOtherPlayerHPChkBtn", desc = "显示/隐藏其他玩家的生命值" },
-- 锁定技能栏
["GameSettingFrameLockShortCutHeadBtn"]			= { chkBtn = "GameSettingFrameLockShortCutChkBtn", desc = "锁定/解锁快捷栏位的拖拽" },
-- 禁止选中宠物
["GameSettingFrameForbitSelectPetHeadBtn"]		= { chkBtn = "GameSettingFrameForbitSelectPetChkBtn", desc = "禁止/允许选中玩家的宠物" },
-- 技能自动释放
["GameSettingFrameAutoSkillReleaseHeadBtn"]             = { chkBtn = "GameSettingFrameAutoSkillReleaseChkBtn", desc = "施放技能时，自动选取周围敌对目标"},
-- 显示本人/队友生命值
["GameSettingFrameShowMeHPHeadBtn"]				= { chkBtn = "GameSettingFrameShowMeHPChkBtn", desc = "显示/隐藏自己和队友的生命值" },
-- 显示玩家气泡	
["GameSettingFrameShowPlayerTalkHeadBtn"]		= { chkBtn = "GameSettingFrameShowPlayerTalkChkBtn", desc = "显示/隐藏所有玩家及宠物的发言气泡" },
-- 显示伤害飘字
["GameSettingFrameShowDamageHeadBtn"]			= { chkBtn = "GameSettingFrameShowDamageChkBtn", desc = "显示/隐藏攻击造成的伤害数字" },
-- 显示装备比较
["GameSettingFrameShowCompareEquipgeHeadBtn"]	= { chkBtn = "GameSettingFrameShowCompareEquipChkBtn", desc = "显示/隐藏装备对比窗口" },
-- 显示拒绝好友申请
["GameSettingFrameRefuseFriendReqHeadBtn"]		= { chkBtn = "GameSettingFrameRefuseFriendReqChkBtn", 
													desc = "设置后所有玩家都不能加你为好友，为了多交几个真心好朋友，慎选" },
-- 显示拒绝公会邀请
["GameSettingFrameRefuseClanInviteHeadBtn"]		= { chkBtn = "GameSettingFrameRefuseClanInviteChkBtn", 
													desc = "设置后将不再收到其他玩家的加入公会邀请，也许你会错过好友的邀请，慎选" },
-- 拒绝交易申请
["GameSettingFrameRefuseTradeInviteHeadBtn"]	= { chkBtn = "GameSettingFrameRefuseTradeInviteChkBtn", 
													desc = "设置后将不再收到其他玩家的交易请求，也许你会错过好友的赠送，慎选" },
-- 拒绝骑乘邀请
["GameSettingFrameRefuseRideInviteHeadBtn"]		= { chkBtn = "GameSettingFrameRefuseRideInviteChkBtn", 
													desc = "设置后将不再收到其他玩家的骑乘邀请，慎选" },
-- 显示拒绝组队邀请
["GameSettingFrameRefuseTeamInviteHeadBtn"]		= { chkBtn = "GameSettingFrameRefuseTeamInviteChkBtn",
													desc = "设置后将不再收到其他玩家的组队邀请，一起组队战斗能够享受更多的游戏乐趣，慎选" },
-- 显示拒绝查看装备
["GameSettingFrameRefuseSeeEquipHeadBtn"]		= { chkBtn = "GameSettingFrameRefuseSeeEquipChkBtn", 
													desc = "设置后将其他人不能查看你身上的装备信息" },
-- 显示拒绝切磋邀请
["GameSettingFrameRefusePKInviteHeadBtn"]		= { chkBtn = "GameSettingFrameRefusePKInviteChkBtn", 
													desc = "设置后将不再收到其他人的切磋邀请" },

-- 显示本人公会名字
["GameSettingFrameSelfClanNameHeadBtn"]			= { chkBtn = "GameSettingFrameShowSelfClanNameChkBtn", 
													desc = "显示/隐藏自己所在公会的名称" },

-- 显示其他玩家公会名字
["GameSettingFrameOtherPlayerClanNameHeadBtn"]	= { chkBtn = "GameSettingFrameShowOtherPlayerClanNameChkBtn", 
													desc = "显示/隐藏其它公会的名称" },
-- 显示完美星特效
["GameSettingFramePerfectStarHeadBtn"]			= { chkBtn = "GameSettingFramePerfectStarChkBtn", 
													desc = "显示/隐藏完美星特效" },
-- 显示特权徽章
["GameSettingFrameStarStoneHeadBtn"]			= { chkBtn = "GameSettingFrameStarStoneChkBtn", 
													desc = "显示/隐藏显示特权徽章" },
--使用3D解锁功能
["GameSettingFrameLookUpDownHeadBtn"]			= { chkBtn = "GameSettingFrameLookUpDownChkBtn", 
													desc = "使用/禁用3D解锁功能" },
};

-- 视频设置按钮
function GameSettingFrame_ShiPinSettingButton_OnClick()
	local curShowFrame = getglobal( this:GetParent() );
	SystemSettingFrame:SetPoint( "topleft", "UIClient", "topleft", curShowFrame:GetRealLeft(), curShowFrame:GetRealTop() );
	SystemSettingFrame:Show();
	curShowFrame:Hide();
end

--鼠标设置按钮
function GameSettingFrame_MouseSettingButton_OnClick()
	local curShowFrame = getglobal( this:GetParent() );
	MouseSettingFrame:SetPoint( "topleft", "UIClient", "topleft", curShowFrame:GetRealLeft(), curShowFrame:GetRealTop() );
	MouseSettingFrame:Show();
	curShowFrame:Hide();
end

--按钮设置按钮
function GameSettingFrame_KeySettingButton_OnClick()
	local curShowFrame = getglobal(this:GetParent());
	KeySettingFrame:SetPoint( "topleft", "UIClient", "topleft", curShowFrame:GetRealLeft(), curShowFrame:GetRealTop() );
	KeySettingFrame:Show();
	curShowFrame:Hide();
end

function GameSettingFrameOptButton_OnEnter()
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = t_optBtnEventsMap[this:GetName()]["desc"], 
						frame = this:GetParent(), button = this:GetName() } );
end

function GameSettingFrameOptButton_OnClick()
	local chkBtn = getglobal( t_optBtnEventsMap[this:GetName()]["chkBtn"] );
	chkBtn:setCheckState( not chkBtn:GetCheckState() );
end

-- 默认设置
function GameSettingFrameDefaultSettingBtn_OnClick()
	local systemSetting	= GameMgr:getUserSystemConfig();
	systemSetting:setDefaultGameOpt();
	UpdateGameSettingFrame();
end

-- 取消
function GameSettingFrameCancelBtn_CloseButton_OnClick()
	GameSettingFrame:Hide();
end

-- 保存
function GameSettingFrameSaveBtn_OnClick()
	local systemSetting	= GameMgr:getUserSystemConfig();
	for name, data in pairs( t_buttonMap ) do
		local chkBtn = getglobal( name );
		local result = chkBtn:GetCheckState();
		if data["reverse"] then
			result = not result
		end
		systemSetting:setGameOpt( data["index"] - 1, result );
	end

	systemSetting:saveGameOpt();
	UpdateGameSettingFrame();
	ActorMgr:SetPerfectStarNineActor();
end

-- 显示玩家名字
function GameSettingFrameShowPlayerNameChkBtn_OnClick()
end

-- 显示玩家称号/队名
function GameSettingFrameShowTeamNameChkBtn_OnClick()
end

-- 显示召唤兽名
function GameSettingFrameShowPetNameChkBtn_OnClick()
end

-- 显示怪物名称
function GameSettingFrameShowMonsterNameChkBtn_OnClick()
end

-- 显示怪物血量
function GameSettingFrameShowMonsterHPChkBtn_OnClick()
end

-- 显示其他玩家血量
function GameSettingFrameShowOtherPlayerHPChkBtn_OnClick()
end

-- 显示其他召唤兽血量
function GameSettingFrameShowOtherPetHPChkBtn_OnClick()
end

-- 显示本人及召唤兽血量
function GameSettingFrameShowMeHPChkBtn_OnClick()
end

-- 显示玩家气泡
function GameSettingFrameShowPlayerTalkChkBtn_OnClick()
end

-- 显示伤害飘字
function GameSettingFrameShowDamageChkBtn_OnClick()
end

-- 显示装备比较
function GameSettingFrameShowCompareEquipChkBtn_OnClick()
end

-- 拒绝好友申请
function GameSettingFrameRefuseFriendReqChkBtn_OnClick()
end

-- 拒绝公会邀请
function GameSettingFrameRefuseClanInviteChkBtn_OnClick()
end

-- 拒绝交易申请
function GameSettingFrameRefuseTradeInviteChkBtn_OnClick()
end

-- 拒绝骑乘邀请
function GameSettingFrameRefuseRideInviteChkBtn_OnClick()
end

-- 拒绝组队邀请
function GameSettingFrameRefuseTeamInviteChkBtn_OnClick()
end

-- 拒绝查看装备
function GameSettingFrameRefuseSeeEquipChkBtn_OnClick()
end

-- 拒绝切磋邀请
function GameSettingFrameRefusePKInviteChkBtn_OnClick()
end


-- 设置鼠标面板
function MouseSettingFrame_OnShow()
	local t_btns = { this:GetName().."_ShiPinSettingButton", this:GetName().."_XianShiSettingButton", this:GetName().."_MouseSettingButton", };
	util.DischeckBtns( t_btns );
	
	t_btns = { this:GetName().."_MouseSettingButton", };
	util.CheckBtns( t_btns );

	local systemSetting	= GameMgr:getUserSystemConfig();
	
	local bLeftHabit = systemSetting:getGameOpt( GAME_OPT_SWAP_MOUSE );
	local t_chkBtns =	
	{	[this:GetName().."Type1ChkBtn"]		= { state = bLeftHabit }, 
		[this:GetName().."Type2ChkBtn"]		= { state = not bLeftHabit },
	};
	util.SetChkState( t_chkBtns );

	UpdateMouseSettingFrame();
	
	t_btns = {	this:GetName().."LeftMoveChkBtn",	this:GetName().."RightMoveChkBtn",	this:GetName().."LeftSelectChkBtn", 
				this:GetName().."RightSelectChkBtn",	this:GetName().."LeftAttackChkBtn",	this:GetName().."RightAttackChkBtn", };
	util.DisableBtns( t_btns );
end

function UpdateMouseSettingFrame()
	local leftHabitChkBtn	= getglobal( "MouseSettingFrameType1ChkBtn" );
	local bLeftHabit		= leftHabitChkBtn:GetCheckState( );

	local t_chkBtns =	
	{	["MouseSettingFrameLeftMoveChkBtn"]		= { state = true }, 
		["MouseSettingFrameRightMoveChkBtn"]	= { state = false },
		
		["MouseSettingFrameLeftSelectChkBtn"]	= { state = not bLeftHabit },	-- 若为左键模式( bLeftHabit = true )则为false，
																				-- 若为右键模式( bLeftHabit = false )则为true

		["MouseSettingFrameRightSelectChkBtn"]	= { state = bLeftHabit },		-- 若为左键模式( bLeftHabit = true )则为true，
																				-- 若为右键模式( bLeftHabit = false )则为false

		["MouseSettingFrameLeftAttackChkBtn"]	= { state = bLeftHabit },		-- 若为左键模式( bLeftHabit = true )则为true，
																				-- 若为右键模式( bLeftHabit = false )则为false

		["MouseSettingFrameRightAttackChkBtn"]	= { state = not bLeftHabit },	-- 若为左键模式( bLeftHabit = true )则为false，
																				-- 若为右键模式( bLeftHabit = false )则为true
	};

	util.SetChkState( t_chkBtns );
end

-- 视频设置按钮
function MouseSettingFrame_ShiPinSettingButton_OnClick()
	local curShowFrame = getglobal( this:GetParent() );
	SystemSettingFrame:SetPoint( "topleft", "UIClient", "topleft", curShowFrame:GetRealLeft(), curShowFrame:GetRealTop() );
	SystemSettingFrame:Show();
	curShowFrame:Hide();
end

-- 显示设置按钮
function MouseSettingFrame_XianShiSettingButton_OnClick()
	local curShowFrame = getglobal( this:GetParent() );
	GameSettingFrame:SetPoint( "topleft", "UIClient", "topleft", curShowFrame:GetRealLeft(), curShowFrame:GetRealTop() );
	GameSettingFrame:Show();
	curShowFrame:Hide();
end

--按钮设置按钮
function MouseSettingFrame_KeySettingButton_OnClick()
	local curShowFrame = getglobal(this:GetParent());
	KeySettingFrame:SetPoint( "topleft", "UIClient", "topleft", curShowFrame:GetRealLeft(), curShowFrame:GetRealTop() );
	KeySettingFrame:Show();
	curShowFrame:Hide();
end

-- 类型1勾选框
function MouseSettingFrameType1ChkBtn_OnClick()
	local type1ChkBtn = getglobal( this:GetParent().."Type1ChkBtn" );
	local type2ChkBtn = getglobal( this:GetParent().."Type2ChkBtn" );
	type2ChkBtn:setCheckState( not type1ChkBtn:GetCheckState() );
	UpdateMouseSettingFrame();
end

function MouseSettingFrameType1Btn_OnClick()
	local type1ChkBtn = getglobal( this:GetParent().."Type1ChkBtn" );
	local type2ChkBtn = getglobal( this:GetParent().."Type2ChkBtn" );
	type1ChkBtn:setCheckState( not type1ChkBtn:GetCheckState() );
	type2ChkBtn:setCheckState( not type1ChkBtn:GetCheckState() );
	UpdateMouseSettingFrame();
end

-- 类型2勾选框
function MouseSettingFrameType2ChkBtn_OnClick()
	local type1ChkBtn = getglobal( this:GetParent().."Type1ChkBtn" );
	local type2ChkBtn = getglobal( this:GetParent().."Type2ChkBtn" );
	type1ChkBtn:setCheckState( not type2ChkBtn:GetCheckState() );
	UpdateMouseSettingFrame();
end

function MouseSettingFrameType2Btn_OnClick()
	local type1ChkBtn = getglobal( this:GetParent().."Type1ChkBtn" );
	local type2ChkBtn = getglobal( this:GetParent().."Type2ChkBtn" );
	type2ChkBtn:setCheckState( not type2ChkBtn:GetCheckState() );
	type1ChkBtn:setCheckState( not type2ChkBtn:GetCheckState() );
	UpdateMouseSettingFrame();
end

-- 默认设置
function MouseSettingFrameDefaultSettingBtn_OnClick()
	local systemSetting	= GameMgr:getUserSystemConfig();
	systemSetting:setGameOpt( GAME_OPT_SWAP_MOUSE, true );

	local bLeftHabit = systemSetting:getGameOpt( GAME_OPT_SWAP_MOUSE );
	local t_chkBtns =	
	{	[this:GetParent().."Type1ChkBtn"]	= { state = bLeftHabit }, 
		[this:GetParent().."Type2ChkBtn"]	= { state = not bLeftHabit },
	};
	util.SetChkState( t_chkBtns );

	UpdateMouseSettingFrame();

	systemSetting:saveGameOpt();
end

-- 保存设置
function MouseSettingFrameSaveBtn_OnClick()
	local leftHabitChkBtn	= getglobal( this:GetParent().."Type1ChkBtn" );
	local bLeftHabit		= leftHabitChkBtn:GetCheckState( );
	local systemSetting	= GameMgr:getUserSystemConfig();
	systemSetting:setGameOpt( GAME_OPT_SWAP_MOUSE, bLeftHabit );
	systemSetting:saveGameOpt();
	--MouseSettingFrame:Hide();
end

