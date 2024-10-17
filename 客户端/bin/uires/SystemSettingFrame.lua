local t_curTmpSystemSetting = {};
local NONE_SHADOW_QUALITY	= 0;
local LOW_SHADOW_QUALITY	= 1;
local MEDIUM_SHADOW_QUALITY	= 1024;
local HIGH_SHADOW_QUALITY	= 2048;
local MAX_SOLUTION_OPTION	= 6; -- ���ķֱ���ѡ����Ŀ
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
-- ��ֱͬ��
["SystemSettingFrameVerticalHeadBtn"]		= { desc = "�������Ϸ֡������ʾ��ˢ����ͬ�������Խ����Ϸ��ͼ���޷���ʾ������" },

-- ����ģʽ
["SystemSettingFrameWindowedHeadBtn"]		= { desc = "�Դ���ģʽ������Ϸ" },

-- ȫ��ģʽ
["SystemSettingFrameFullScreenHeadBtn"]		= { desc = "��ѡ��ѡ�����ȫ��ģʽ������Ϸ����ȥ����Ϸ�߿�" },

-- ��Ӱ����
["SystemSettingFrameShadowQualityHeadBtn"]	= { desc = "��ѡ��ѡ���ʹ��������ʾ���ֱ��������Ӱ�ӣ���Ӱ�������ٶȣ����Դ�С���Կ�������رգ��������������Ϸ�����ٶ�" },

-- ˮ������
["SystemSettingFrameOpenWaterQualityHeadBtn"]= { desc = "��ѡ��ѡ�����ˮ���е�Ӱ����Χ�������壬��Ӱ�������ٶȣ����Դ�С���Կ�������رգ��������������Ϸ�����ٶ�\n#G�л�����������Ч" },

-- ��Ļ����
["SystemSettingFrameLightHeadBtn"]			= { desc = "��ѡ��ѡ���������͹�ӰЧ�����ر����ѡ�������������ٶ�" },

-- ��Ļ��Ч
["SystemSettingFrameEffectHeadBtn"]			= { desc = "��ѡ��ѡ���߹�Ч�Ӿ����������Ӱ�������ٶȡ����Դ�С���Կ�������رգ��������������Ϸ�����ٶ�" },

-- ȫ�������
["SystemSettingFrameAtialiaseHeadBtn"]		= { desc = "���ߵĿ���ݿ���ʹ��ɫ�Լ���������ı�Ե���ֵø�ƽ�������ǻή����Ϸ�������ٶȣ����Դ�С���Կ��������ԣ�����ر�" },

-- ��������
["SystemSettingFrameBackgroundMusicHeadBtn"]= { desc = "Ӱ�쳡���ı�������" },

-- ��Ч����
["SystemSettingFrameEffectMusicHeadBtn"]	= { desc = "Ӱ�쳡���л�����Ч������ʹ��ʱ����Ч���¼��������ĸ�����Ч" },
};

function SystemSettingFrameBtn_OnEnter()
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = t_systemSettingBtnDescMap[this:GetName()]["desc"], 
						frame = this:GetParent(), button = this:GetName() } );
end

-- ���ϵͳ�������Ĺرհ�ť��Ӧ
function SystemSettingFrame_CloseButton_OnClick()
	SystemSettingFrame:Hide();
end

-- ��ʾ���ð�ť
function SystemSettingFrame_XianShiSettingButton_OnClick()
	local curShowFrame = getglobal( this:GetParent() );
	GameSettingFrame:SetPoint( "topleft", "UIClient", "topleft", curShowFrame:GetRealLeft(), curShowFrame:GetRealTop() );
	GameSettingFrame:Show();
	curShowFrame:Hide();
end

-- ������ð�ť
function SystemSettingFrame_MouseSettingButton_OnClick()
	local curShowFrame = getglobal( this:GetParent() );
	MouseSettingFrame:SetPoint( "topleft", "UIClient", "topleft", curShowFrame:GetRealLeft(), curShowFrame:GetRealTop() );
	MouseSettingFrame:Show();
	curShowFrame:Hide();
end

-- �������ð�ť
function SystemSettingFrame_KeySettingButton_OnClick()
	local curShowFrame = getglobal(this:GetParent());
	KeySettingFrame:SetPoint( "topleft", "UIClient", "topleft", curShowFrame:GetRealLeft(), curShowFrame:GetRealTop() );
	KeySettingFrame:Show();
	curShowFrame:Hide();
end

-- �����ֱͬ��check buttonʱ����Ӧ
function SystemSettingFrameVerticalChkBtn_OnClick()
	local chkBtn = getglobal( "SystemSettingFrameVerticalChkBtn" );
	t_curTmpSystemSetting["sync_refresh"] = chkBtn:GetCheckState();
end

function SystemSettingFrameVerticalHeadBtn_OnClick()
	local chkBtn = getglobal( "SystemSettingFrameVerticalChkBtn" );
	chkBtn:setCheckState( not chkBtn:GetCheckState() );
	SystemSettingFrameVerticalChkBtn_OnClick();
end

-- ����ֱ���ѡ��������ť��Ӧ
function SystemSettingFrameSolutionDownBtn_OnClick()
	local selectFrame = getglobal( "SystemSettingFrame_SelectBtn" );
	if selectFrame:IsShown() then
		selectFrame:Hide();
	else
		selectFrame:Show();
	end
end

-- ������ֱ���������ѡ����
function SystemSettingFrame_LayerBtn_OnClick()
	local font			= getglobal( this:GetName() .. "LayerName" );
	local szSolution	= font:GetText();
	local pivotPos		= string.find( szSolution, "*" );
	local leftBracket	= string.find( szSolution, "��" );
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

-- ����ģʽ��ѡ��ť
function SystemSettingFrameWindowedChkBtn_OnClick()
	local chkBtn = getglobal( "SystemSettingFrameWindowedChkBtn" );
	-- ������ģʽ��ȫ��ģʽһ����ť����ѡ��״̬ʱ��,��ֻ��ͨ����һ����ť����ȡ������
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

-- ȫ��ģʽ��ѡ��ť
function SystemSettingFrameFullScreenChkBtn_OnClick()
	local chkBtn = getglobal( "SystemSettingFrameFullScreenChkBtn" );
	-- ������ģʽ��ȫ��ģʽһ����ť����ѡ��״̬ʱ��,��ֻ��ͨ����һ����ť����ȡ������
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

-- ��Ļ���⹴ѡ��ť
function SystemSettingFrameLightChkBtn_OnClick()
	local chkBtn = getglobal( "SystemSettingFrameLightChkBtn" );
	t_curTmpSystemSetting["bloom"] = chkBtn:GetCheckState();
end

function SystemSettingFrameLightHeadBtn_OnClick()
	local chkBtn = getglobal( "SystemSettingFrameLightChkBtn" );
	chkBtn:setCheckState( not chkBtn:GetCheckState() );
	SystemSettingFrameLightChkBtn_OnClick();
end

-- ��Ļ��Ч��ѡ��ť
function SystemSettingFrameEffectChkBtn_OnClick()
	local chkBtn = getglobal( "SystemSettingFrameEffectChkBtn" );
	t_curTmpSystemSetting["distort"] = chkBtn:GetCheckState();
end

function SystemSettingFrameEffectHeadBtn_OnClick()
	local chkBtn = getglobal( "SystemSettingFrameEffectChkBtn" );
	chkBtn:setCheckState( not chkBtn:GetCheckState() );
	SystemSettingFrameEffectChkBtn_OnClick();
end

-- ����ˮ������ѡ��ť
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

-- �ֱ���ѡ�����ʾ��ʱ�����
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
		-- Ҫ������ʾ�ķֱ��ʱ��벻������ǰ����ķֱ���
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

-- ��Ӱ����ѡ���
local t_shadowQuality = {	{ desc = "��", value = NONE_SHADOW_QUALITY		}, { desc = "��", value = LOW_SHADOW_QUALITY }, 
							{ desc = "��", value = HIGH_SHADOW_QUALITY	}, 
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

	return "��";
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

local t_antialiaseQuality = { { desc = "��", value = 0 }, { desc = "��", value = 1 }, 
							{ desc = "��", value = 2 }, { desc = "��", value = 3 } };

function GetAtialiaseQuality( nValue )
	for i = 1, table.getn( t_antialiaseQuality ) do
		if t_antialiaseQuality[i].value == nValue then
			return t_antialiaseQuality[i].desc;
		end
	end

	return "��";
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
		font:SetText( data.width .. "*" .. data.height .. "��������" );
	else
		font:SetText( data.width .. "*" .. data.height  );
	end

	return true;
end

-- ������Ӱ������ť
function SystemSettingFrameShadowQualityChkBtn_OnClick()
	local chkBtn = getglobal( "SystemSettingFrameShadowQualityChkBtn" );
	t_curTmpSystemSetting["shadowmap"] = chkBtn:GetCheckState() and HIGH_SHADOW_QUALITY or LOW_SHADOW_QUALITY;
end

function SystemSettingFrameShadowQualityHeadBtn_OnClick()
	local chkBtn = getglobal( "SystemSettingFrameShadowQualityChkBtn" );
	chkBtn:setCheckState( not chkBtn:GetCheckState() );

	SystemSettingFrameShadowQualityChkBtn_OnClick();
end

-- ѡ����Ӱ������ť
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

-- ȫ������
function SystemSettingFrameForbitAllNoiseChkBtn_OnClick()
end

-- �������
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

-- �������
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

-- ���Ĭ�����ð�ť��Ӧ
function SystemSettingFrameDefaultSettingBtn_OnClick()	
	GameMgr:ApplyDefaultUserConfig();
	t_curTmpSystemSetting = {};
	UpdateSystemSettingFrame();
	--SystemSettingFrame:Hide();
	
	--GameMgr:startCheckPerformace();
end

-- ������水ť��Ӧ
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
	-- ��Ч
	local slider = getglobal( "SystemSettingFrameEffectMusic_ScrollBar" );
	userConfig.m_fGlobalSoundVolume	= slider:GetValue() / slider:GetMaxValue();
	-- ��������
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

-- ���FPS��ť
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

local t_hideCheckButtons =	{	-- ��ֱͬ��											
							["SystemSettingFrameVerticalChkBtn"]			= false,
								-- ����ģʽ
							["SystemSettingFrameWindowedChkBtn"]			= false,
							-- ȫ��ģʽ											
							["SystemSettingFrameFullScreenChkBtn"]			= false,	
							-- ˮ������											
							["SystemSettingFrameOpenWaterQualityChkBtn"]	= false,	
							-- ��Ļ����
							["SystemSettingFrameLightChkBtn"]				= false,
							-- ��Ļ��Ч											
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
									-- ��ֱͬ��											
									["SystemSettingFrameVerticalChkBtn"]			= curUserConfig.m_bSyncRefresh,	
									-- ����ģʽ
									["SystemSettingFrameWindowedChkBtn"]			= not curUserConfig.m_bFullScreen,
									-- ȫ��ģʽ														
									["SystemSettingFrameFullScreenChkBtn"]			= curUserConfig.m_bFullScreen,	
									-- ˮ������												
									["SystemSettingFrameOpenWaterQualityChkBtn"]	= curUserConfig.m_bWaterReflect,	
									-- ��Ļ����
									["SystemSettingFrameLightChkBtn"]				= curUserConfig.m_bBloom ,
									-- ��Ļ��Ч															
									["SystemSettingFrameEffectChkBtn"]				= curUserConfig.m_bDistort,	
									-- ����															
									["SystemSettingFrameForbitAllNoiseChkBtn"]		= curUserConfig.m_bMute,	
									-- ʵʱ��Ӱ
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
	-- ������10s,��ʼ�Զ��л��ֱ���
	local t_selectCardOption	= self:getNewScreenSize();
	local userConfig			= GameMgr:getLatestUserSystemConfig();
	userConfig.m_nWinWidth		= t_selectCardOption["width"];
	userConfig.m_nWinHeight		= t_selectCardOption["height"];
	GameMgr:ApplyNewUserConfig( userConfig );
end

t_SystemSettingFramControl["getNewScreenSize"] = 
function ( self )
	-- �ֱ��ʱ仯���������Ȳ��һ�ûʹ�õķֱ���
	local userConfig		= GameMgr:getLatestUserSystemConfig();
	local nCurWidth			= userConfig.m_nWinWidth;
	local nCurHeight		= userConfig.m_nWinHeight;
	-- ����ǰ�ֱ��������һ���ֱ��ʣ���
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
	
	-- �Ȼ�ȡ���µ�
	for i = 1, systemSetting:getNumScreenMode()do
		local nWidth	= systemSetting:getScreenModeWidth( i - 1 );
		local nHeight	= systemSetting:getScreenModeHeight( i - 1 );
		if not t_selectSolutions:isSizeExist( { width = nWidth, height = nHeight } ) then
			table.insert( t_selectSolutions, { width = nWidth, height = nHeight } );
		end
	end
end

-- ��Ϸ����

local t_buttonMap = 
{
	-- ��ʾ�������/��������
	["GameSettingFrameShowPlayerNameChkBtn"]	= { index = GAME_OPT_SHOW_ALLPLAYER_AND_PET_NAME + 1, },
	-- ��ʾ������Ұ������ǵĳƺ���
	["GameSettingFrameShowTeamNameChkBtn"]		= { index = GAME_OPT_SHOW_ALLPLAYER_TEAM_DESIGNATION_NAME + 1, },
	-- ��ʾ�ٻ�����
	--["GameSettingFrameShowPetNameChkBtn"]		= { index = GAME_OPT_SHOW_MAINPET_NAME + 1, },
	-- ��ʾ������
	["GameSettingFrameShowMonsterNameChkBtn"]	= { index = GAME_OPT_SHOW_MONSTER_NAME + 1, },
	-- ��ʾ����Ѫ��
	["GameSettingFrameShowMonsterHPChkBtn"]		= { index = GAME_OPT_SHOW_MONSTER_HP + 1, },
	-- ��ʾ�������Ѫ��
	["GameSettingFrameShowOtherPlayerHPChkBtn"] = { index = GAME_OPT_SHOW_OTHERPLAYER_HP + 1, },
	-- ��ʾ�����ٻ���Ѫ��
	--["GameSettingFrameShowOtherPetHPChkBtn"]	= { index = GAME_OPT_SHOW_OTHERPLAYER_PET_HP + 1, },
	-- ��ʾ����/��������ֵ
	["GameSettingFrameShowMeHPChkBtn"]			= { index = GAME_OPT_SHOW_MAINPLAYER_AND_TEAM_HP + 1, },
	-- ����������
	["GameSettingFrameLockShortCutChkBtn"]		= { index = GAME_OPT_LOCK_SHORTCUT + 1, },
	-- ��ֹѡ�г���
	["GameSettingFrameForbitSelectPetChkBtn"]	= { index = GAME_OPT_FORBIT_SELECT_PET + 1, },
	--�����Զ��ͷ�
	["GameSettingFrameAutoSkillReleaseChkBtn"]      = { index = GAME_OPT_AUTO_SKILL_RELEASE + 1, reverse = true },
	-- ��ʾ�������
	["GameSettingFrameShowPlayerTalkChkBtn"]	= { index = GAME_OPT_SHOW_OTHERPLAYER_TALK_POP + 1, },
	-- ��ʾ�˺�Ʈ��
	["GameSettingFrameShowDamageChkBtn"]		= { index = GAME_OPT_SHOW_MAINPLAYER_DAMAGE_SHOW + 1, },
	-- ��ʾװ���Ƚ�
	["GameSettingFrameShowCompareEquipChkBtn"]	= { index = GAME_OPT_SHOW_EQUIP_SHOW + 1, },
	-- ��ʾ�ܾ���������
	["GameSettingFrameRefuseFriendReqChkBtn"]	= { index = GAME_OPT_REFUSE_ADD_FRIEND_REQ + 1, },
	-- �ܾ���������
	["GameSettingFrameRefuseClanInviteChkBtn"]	= { index = GAME_OPT_REFUSE_CLAN_INVITE + 1, },
	-- �ܾ���������
	["GameSettingFrameRefuseTradeInviteChkBtn"] = { index = GAME_OPT_REFUSE_TRADE_REQ + 1, },
	-- �ܾ��������
	["GameSettingFrameRefuseRideInviteChkBtn"]	= { index = GAME_OPT_REFUSE_RIDE_INVITE + 1, },
	-- �ܾ��������
	["GameSettingFrameRefuseTeamInviteChkBtn"]	= { index = GAME_OPT_REFUSE_TEAM_INVITE + 1, },
	-- �ܾ��鿴װ��
	["GameSettingFrameRefuseSeeEquipChkBtn"]	= { index = GAME_OPT_REFUSE_SEE_EQUIP + 1, },
	-- �ܾ��д�����
	["GameSettingFrameRefusePKInviteChkBtn"]	= { index = GAME_OPT_REFUSE_REFUSE_PK + 1, },
	-- ��ʾ���˹�������
	["GameSettingFrameShowSelfClanNameChkBtn"]	= { index = GAME_OPT_SHOW_MAIN_CLAN_NAME + 1, },
	-- ��ʾ�����˹�������
	["GameSettingFrameShowOtherPlayerClanNameChkBtn"]	= { index = GAME_OPT_SHOW_OTHERPLAYER_CLAN_NAME + 1, },
	-- ��ʾ��������Ч
	["GameSettingFramePerfectStarChkBtn"]		= { index = GAME_OPT_SHOW_PERFECT_STAR_EFFECT + 1, reverse = true },
	-- ��ʾ��Ȩ����
	["GameSettingFrameStarStoneChkBtn"]		= { index = GAME_OPT_SHOW_STAR_STONE + 1, reverse = true },
	--ʹ��3D��������
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
			-- ��Ϊ��һ�ε�¼��Ϸ�������Ϸһ��Ĭ������
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
-- ��ʾ�������
["GameSettingFrameShowPlayerNameHeadBtn"]		= { chkBtn = "GameSettingFrameShowPlayerNameChkBtn", desc = "��ʾ/����������Ҽ����������" },
-- ��ʾ��ҳƺ���
["GameSettingFrameShowTeamNameHeadBtn"]			= { chkBtn = "GameSettingFrameShowTeamNameChkBtn", desc = "��ʾ/����������ҵĳƺ�" },
-- ��ʾ������
["GameSettingFrameShowMonsterNameHeadBtn"]		= { chkBtn = "GameSettingFrameShowMonsterNameChkBtn", desc = "��ʾ/���ع��������" },
-- ��ʾ����Ѫ��
["GameSettingFrameShowMonsterHPHeadBtn"]		= { chkBtn = "GameSettingFrameShowMonsterHPChkBtn", desc = "��ʾ/���ع��������ֵ" },
-- ��ʾ�������Ѫ��
["GameSettingFrameShowOtherPlayeHPHeadBtn"]		= { chkBtn = "GameSettingFrameShowOtherPlayerHPChkBtn", desc = "��ʾ/����������ҵ�����ֵ" },
-- ����������
["GameSettingFrameLockShortCutHeadBtn"]			= { chkBtn = "GameSettingFrameLockShortCutChkBtn", desc = "����/���������λ����ק" },
-- ��ֹѡ�г���
["GameSettingFrameForbitSelectPetHeadBtn"]		= { chkBtn = "GameSettingFrameForbitSelectPetChkBtn", desc = "��ֹ/����ѡ����ҵĳ���" },
-- �����Զ��ͷ�
["GameSettingFrameAutoSkillReleaseHeadBtn"]             = { chkBtn = "GameSettingFrameAutoSkillReleaseChkBtn", desc = "ʩ�ż���ʱ���Զ�ѡȡ��Χ�ж�Ŀ��"},
-- ��ʾ����/��������ֵ
["GameSettingFrameShowMeHPHeadBtn"]				= { chkBtn = "GameSettingFrameShowMeHPChkBtn", desc = "��ʾ/�����Լ��Ͷ��ѵ�����ֵ" },
-- ��ʾ�������	
["GameSettingFrameShowPlayerTalkHeadBtn"]		= { chkBtn = "GameSettingFrameShowPlayerTalkChkBtn", desc = "��ʾ/����������Ҽ�����ķ�������" },
-- ��ʾ�˺�Ʈ��
["GameSettingFrameShowDamageHeadBtn"]			= { chkBtn = "GameSettingFrameShowDamageChkBtn", desc = "��ʾ/���ع�����ɵ��˺�����" },
-- ��ʾװ���Ƚ�
["GameSettingFrameShowCompareEquipgeHeadBtn"]	= { chkBtn = "GameSettingFrameShowCompareEquipChkBtn", desc = "��ʾ/����װ���Աȴ���" },
-- ��ʾ�ܾ���������
["GameSettingFrameRefuseFriendReqHeadBtn"]		= { chkBtn = "GameSettingFrameRefuseFriendReqChkBtn", 
													desc = "���ú�������Ҷ����ܼ���Ϊ���ѣ�Ϊ�˶ཻ�������ĺ����ѣ���ѡ" },
-- ��ʾ�ܾ���������
["GameSettingFrameRefuseClanInviteHeadBtn"]		= { chkBtn = "GameSettingFrameRefuseClanInviteChkBtn", 
													desc = "���ú󽫲����յ�������ҵļ��빫�����룬Ҳ����������ѵ����룬��ѡ" },
-- �ܾ���������
["GameSettingFrameRefuseTradeInviteHeadBtn"]	= { chkBtn = "GameSettingFrameRefuseTradeInviteChkBtn", 
													desc = "���ú󽫲����յ�������ҵĽ�������Ҳ����������ѵ����ͣ���ѡ" },
-- �ܾ��������
["GameSettingFrameRefuseRideInviteHeadBtn"]		= { chkBtn = "GameSettingFrameRefuseRideInviteChkBtn", 
													desc = "���ú󽫲����յ�������ҵ�������룬��ѡ" },
-- ��ʾ�ܾ��������
["GameSettingFrameRefuseTeamInviteHeadBtn"]		= { chkBtn = "GameSettingFrameRefuseTeamInviteChkBtn",
													desc = "���ú󽫲����յ�������ҵ�������룬һ�����ս���ܹ����ܸ������Ϸ��Ȥ����ѡ" },
-- ��ʾ�ܾ��鿴װ��
["GameSettingFrameRefuseSeeEquipHeadBtn"]		= { chkBtn = "GameSettingFrameRefuseSeeEquipChkBtn", 
													desc = "���ú������˲��ܲ鿴�����ϵ�װ����Ϣ" },
-- ��ʾ�ܾ��д�����
["GameSettingFrameRefusePKInviteHeadBtn"]		= { chkBtn = "GameSettingFrameRefusePKInviteChkBtn", 
													desc = "���ú󽫲����յ������˵��д�����" },

-- ��ʾ���˹�������
["GameSettingFrameSelfClanNameHeadBtn"]			= { chkBtn = "GameSettingFrameShowSelfClanNameChkBtn", 
													desc = "��ʾ/�����Լ����ڹ��������" },

-- ��ʾ������ҹ�������
["GameSettingFrameOtherPlayerClanNameHeadBtn"]	= { chkBtn = "GameSettingFrameShowOtherPlayerClanNameChkBtn", 
													desc = "��ʾ/�����������������" },
-- ��ʾ��������Ч
["GameSettingFramePerfectStarHeadBtn"]			= { chkBtn = "GameSettingFramePerfectStarChkBtn", 
													desc = "��ʾ/������������Ч" },
-- ��ʾ��Ȩ����
["GameSettingFrameStarStoneHeadBtn"]			= { chkBtn = "GameSettingFrameStarStoneChkBtn", 
													desc = "��ʾ/������ʾ��Ȩ����" },
--ʹ��3D��������
["GameSettingFrameLookUpDownHeadBtn"]			= { chkBtn = "GameSettingFrameLookUpDownChkBtn", 
													desc = "ʹ��/����3D��������" },
};

-- ��Ƶ���ð�ť
function GameSettingFrame_ShiPinSettingButton_OnClick()
	local curShowFrame = getglobal( this:GetParent() );
	SystemSettingFrame:SetPoint( "topleft", "UIClient", "topleft", curShowFrame:GetRealLeft(), curShowFrame:GetRealTop() );
	SystemSettingFrame:Show();
	curShowFrame:Hide();
end

--������ð�ť
function GameSettingFrame_MouseSettingButton_OnClick()
	local curShowFrame = getglobal( this:GetParent() );
	MouseSettingFrame:SetPoint( "topleft", "UIClient", "topleft", curShowFrame:GetRealLeft(), curShowFrame:GetRealTop() );
	MouseSettingFrame:Show();
	curShowFrame:Hide();
end

--��ť���ð�ť
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

-- Ĭ������
function GameSettingFrameDefaultSettingBtn_OnClick()
	local systemSetting	= GameMgr:getUserSystemConfig();
	systemSetting:setDefaultGameOpt();
	UpdateGameSettingFrame();
end

-- ȡ��
function GameSettingFrameCancelBtn_CloseButton_OnClick()
	GameSettingFrame:Hide();
end

-- ����
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

-- ��ʾ�������
function GameSettingFrameShowPlayerNameChkBtn_OnClick()
end

-- ��ʾ��ҳƺ�/����
function GameSettingFrameShowTeamNameChkBtn_OnClick()
end

-- ��ʾ�ٻ�����
function GameSettingFrameShowPetNameChkBtn_OnClick()
end

-- ��ʾ��������
function GameSettingFrameShowMonsterNameChkBtn_OnClick()
end

-- ��ʾ����Ѫ��
function GameSettingFrameShowMonsterHPChkBtn_OnClick()
end

-- ��ʾ�������Ѫ��
function GameSettingFrameShowOtherPlayerHPChkBtn_OnClick()
end

-- ��ʾ�����ٻ���Ѫ��
function GameSettingFrameShowOtherPetHPChkBtn_OnClick()
end

-- ��ʾ���˼��ٻ���Ѫ��
function GameSettingFrameShowMeHPChkBtn_OnClick()
end

-- ��ʾ�������
function GameSettingFrameShowPlayerTalkChkBtn_OnClick()
end

-- ��ʾ�˺�Ʈ��
function GameSettingFrameShowDamageChkBtn_OnClick()
end

-- ��ʾװ���Ƚ�
function GameSettingFrameShowCompareEquipChkBtn_OnClick()
end

-- �ܾ���������
function GameSettingFrameRefuseFriendReqChkBtn_OnClick()
end

-- �ܾ���������
function GameSettingFrameRefuseClanInviteChkBtn_OnClick()
end

-- �ܾ���������
function GameSettingFrameRefuseTradeInviteChkBtn_OnClick()
end

-- �ܾ��������
function GameSettingFrameRefuseRideInviteChkBtn_OnClick()
end

-- �ܾ��������
function GameSettingFrameRefuseTeamInviteChkBtn_OnClick()
end

-- �ܾ��鿴װ��
function GameSettingFrameRefuseSeeEquipChkBtn_OnClick()
end

-- �ܾ��д�����
function GameSettingFrameRefusePKInviteChkBtn_OnClick()
end


-- ����������
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
		
		["MouseSettingFrameLeftSelectChkBtn"]	= { state = not bLeftHabit },	-- ��Ϊ���ģʽ( bLeftHabit = true )��Ϊfalse��
																				-- ��Ϊ�Ҽ�ģʽ( bLeftHabit = false )��Ϊtrue

		["MouseSettingFrameRightSelectChkBtn"]	= { state = bLeftHabit },		-- ��Ϊ���ģʽ( bLeftHabit = true )��Ϊtrue��
																				-- ��Ϊ�Ҽ�ģʽ( bLeftHabit = false )��Ϊfalse

		["MouseSettingFrameLeftAttackChkBtn"]	= { state = bLeftHabit },		-- ��Ϊ���ģʽ( bLeftHabit = true )��Ϊtrue��
																				-- ��Ϊ�Ҽ�ģʽ( bLeftHabit = false )��Ϊfalse

		["MouseSettingFrameRightAttackChkBtn"]	= { state = not bLeftHabit },	-- ��Ϊ���ģʽ( bLeftHabit = true )��Ϊfalse��
																				-- ��Ϊ�Ҽ�ģʽ( bLeftHabit = false )��Ϊtrue
	};

	util.SetChkState( t_chkBtns );
end

-- ��Ƶ���ð�ť
function MouseSettingFrame_ShiPinSettingButton_OnClick()
	local curShowFrame = getglobal( this:GetParent() );
	SystemSettingFrame:SetPoint( "topleft", "UIClient", "topleft", curShowFrame:GetRealLeft(), curShowFrame:GetRealTop() );
	SystemSettingFrame:Show();
	curShowFrame:Hide();
end

-- ��ʾ���ð�ť
function MouseSettingFrame_XianShiSettingButton_OnClick()
	local curShowFrame = getglobal( this:GetParent() );
	GameSettingFrame:SetPoint( "topleft", "UIClient", "topleft", curShowFrame:GetRealLeft(), curShowFrame:GetRealTop() );
	GameSettingFrame:Show();
	curShowFrame:Hide();
end

--��ť���ð�ť
function MouseSettingFrame_KeySettingButton_OnClick()
	local curShowFrame = getglobal(this:GetParent());
	KeySettingFrame:SetPoint( "topleft", "UIClient", "topleft", curShowFrame:GetRealLeft(), curShowFrame:GetRealTop() );
	KeySettingFrame:Show();
	curShowFrame:Hide();
end

-- ����1��ѡ��
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

-- ����2��ѡ��
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

-- Ĭ������
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

-- ��������
function MouseSettingFrameSaveBtn_OnClick()
	local leftHabitChkBtn	= getglobal( this:GetParent().."Type1ChkBtn" );
	local bLeftHabit		= leftHabitChkBtn:GetCheckState( );
	local systemSetting	= GameMgr:getUserSystemConfig();
	systemSetting:setGameOpt( GAME_OPT_SWAP_MOUSE, bLeftHabit );
	systemSetting:saveGameOpt();
	--MouseSettingFrame:Hide();
end

