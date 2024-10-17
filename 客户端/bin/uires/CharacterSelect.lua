local MAX_HAIR_COLOR	= 4;
local MAX_SELECT_RACE	= 4;

DEL_ERR = 1; --/* role del fail because other reason */
DEL_ERR_CLAN_LEADER = 2; 	--/* 角色是公会会长，不允许被删 */

local MAX_ROLES_NUM_CREATED = 4;
local MAX_BIRTH_BTN = 8;

local selectHairstyle	= 1;	--发型
local selectHairColor	= 1;	--发色
local selectFace		= 1;	--脸型
local selectHead		= 1;	--头像
local nSelectClothes	= 2;	-- 服装

local nSelectRace		= 0;
local nSelectGender		= 0;
MAX_ATTR_BTN			= 6;

local bCreating = false;

g_nNEW_SUCC			= 0; 	--/* role new succ */
g_nNEW_SVRBUSY		= 1; 	--/* role new fail because server busy */
g_nNEW_ROLENUM		= 2; 	--/* role new fail because account have too many roles */
g_nNEW_DUP			= 3; 	--/* role new fail because dup name */
g_nNEW_NAME_CHECK	= 4; 	--/* 角色名不符合规范 */
g_nNEW_ERR			= 5; 	--/* role new fail because other reason */
g_nNEW_NOCREATE 	= 6; 	--	/* 服务器禁止创建角色 */

local nIndex;
local nSelectBirthPlaceId;	--/* 记录玩家选中的出生地的ID */
local ModelAngle = 0;
local CreatModelAngle = 0;

local bDown_Enter			= false; --/* 进入游戏按钮的单击判断 */
local bBeginRecord_Enter	= false;
local nLastTime_Enter		= 0;
local ENTERCLICK_SECOND		= 2;

local nCurrSelectIndex  = 0;
local nSelectBirthIndex = 0;
local MAXROLENUM		= 5;
local bNewRole			= false;	-- 是否新创建角色

--local t_EnterWorldInfo = {};	    -- { 角色索引, 是否新角色, 新角色选择的新手村索引}

local nLastMouseClickX = 0;

local freeActionTime			= 0;			-- 播放休闲动作的时间
local nShowStartTime			= 0;
local nShowSelectFrameFlag		= 0;
local nShowNewRoleStartTime		= 0;
local nCreatNewRoleStartTime	= 0;

local tOtherWorldRole = {}
local bNeedShowOtherWorld = false;
local bOtherFinish = false
local szClickName = ""
local needTurnToCreate = true;

function ClearOtherWorldRoleData()
	SelectFrameOtherRoles:Hide()
	bOtherFinish = false
	bNeedShowOtherWorld = false
	tOtherWorldRole = {}
end

function initSelectFrameData()
	selectHairstyle = 1;	--发型
	selectFace      = 1;	--脸型
	selectHead      = 1;	--头像
	selectHairColor = 1;	--发色
	nSelectClothes	= 2;	-- 服装

	nSelectRace		= 0;
	nSelectGender	= 0;
	ModelAngle = 0;
	CreatModelAngle = 0;

	bDown_Enter			= false; --/* 进入游戏按钮的单击判断 */
	bBeginRecord_Enter	= false;
	nLastTime_Enter		= 0;
	ENTERCLICK_SECOND	= 2;

	nCurrSelectIndex  = 0;
	nSelectBirthIndex = 0;
	CharacterSelectFrame:Hide();
	NudeAvatarLoaded = false;

	nShowStartTime = 0;
	nShowSelectFrameFlag = 0;

	-- 新角色选择
	nShowNewRoleStartTime = 0;
	nShowSelectNewFlag = 0;

	-- 角色创建
	nCreatNewRoleStartTime	= 0;
	bCreatNewRoleFrame		= false;
	bNewRole				= false;
	nSelectBirthPlaceId		= 0;
	szClickName = "";

	needTurnToCreate = true;
end

function CharSelectFrame_OnLoad()
	this:RegisterEvent("GE_UPDATE_ROLELIST");
	this:RegisterEvent("GE_UPDATE_ROLEDEL");
	this:RegisterEvent( "GE_ROLE_DELFAILRESULT" );
	
	initSelectFrameData();
	local tex = getglobal( this:GetName().."BkgTex" );
	tex:SetColor( 0, 0, 0 );
	tex:SetBlendAlpha( 0.6 );

	this:setUpdateTime(0);
end

function SelectFrame_OnUpdate()
	local t_SelectFrameMoveFrameAnimControl = GetSelectFrameMoveFrameAnimControl();
	t_SelectFrameMoveFrameAnimControl:update();
end

function SelectFrame_OnLoad()
	this:setUpdateTime( 0 );

	local backBtn = getglobal( "SelectFrameBackGround" );
	local t_relFrames = { "SelectFrameTopChiLunBtn", "SelectFrameBottomChiLunBtn", "SelectFrame_CreateButton", "SelectFrame_TurnNextPageBtn", 
							"SelectFrame_TurnPrePageBtn", "SelectFrame_PageBtn", };
	for i = 1, MAX_ROLES_NUM_CREATED do
		table.insert( t_relFrames, "CSChangeButton"..i );
	end
	
	for _, name in ipairs( t_relFrames ) do
		local btn = getglobal( name );
		backBtn:AddRelFrame( btn );
	end
end

local t_SelectFrameControl = { ["curPage"] = 1 };
function GetSelectFrameControl()
	return t_SelectFrameControl;
end

t_SelectFrameControl["getCurPage"] =
function ( self )
	return self["curPage"];
end

t_SelectFrameControl["setCurPage"] =
function ( self, data )
	self["curPage"] = data["page"];
end

function SelectFrame_OnShow()
	local t_SelectFrameMoveFrameAnimControl = GetSelectFrameMoveFrameAnimControl();
	t_SelectFrameMoveFrameAnimControl:begin( { ["curAnimframe"] = this:GetName() } );
	Privaete_UpdateCharSelectFrame();
	
	local t_showIcons = {	"CharacterSelectFrameTopZheZhaoTex",		"CharacterSelectFrameBottomZheZhaoTex",				
									"CharacterSelectFrameLeftZheZhaoChiLunTex",	"CharacterSelectFrameRightZheZhaoChiLunTex" };
	util.ShowIcons( t_showIcons );
end

function SelectFrame_OnHide()
	GameBackground:releaseEntity(0);
	bCreating = false;

	local t_SelectFrameMoveFrameAnimControl = GetSelectFrameMoveFrameAnimControl();
	if t_SelectFrameMoveFrameAnimControl:isPlayingAnim() then
		t_SelectFrameMoveFrameAnimControl:init( { ["curAnimframe"] = this:GetName() } );
	end
end

function Privaete_UpdateCreateButtonAndTurnPageBtn()
	local nTotalRoleNum = LoginMgr:getNumRole();
	if nTotalRoleNum >= MAX_ROLES_NUM_CREATED then
		SelectFrame_CreateButton:Hide();
		SelectFrame_TurnPrePageBtn:Show();
		SelectFrame_TurnNextPageBtn:Show();
	else
		SelectFrame_CreateButton:Show();
		SelectFrame_TurnPrePageBtn:Hide();
		SelectFrame_TurnNextPageBtn:Hide();
	end
end

function Privaete_UpdateCharSelectFrame()
	-- 角色列表显示时把之前选择过的角色职业性别索引清空
	nSelectGender	= 0;
	nSelectRace	= 0;
	
	local nLoginSelectRoleIndex = LoginMgr:getSelectRoleIndex();
	if nLoginSelectRoleIndex ~= -1 then
		SelectFrame_UpdateAvatar( { ["roleIndex"] = nLoginSelectRoleIndex, } );
	end

	local t_hideIcons = { "PetUI", };
	util.HideIcons( t_hideIcons );

	Privaete_UpdateCreateButtonAndTurnPageBtn();
	Private_UpdateDeleteAndEnterBtn();
end

function BirthPlaceFrame_OnLoad()
	this:RegisterEvent("GE_SELECTBIRTH_SHOW");
end

local t_busyColor = {	[CLIENT_DIR_EXCELLENCE] = { color = { r = 72, g = 109, b = 9 },		stateDesc = "很好" },		
						[CLIENT_DIR_NORMAL]		= { color = { r = 164, g = 160, b = 46 },	stateDesc = "正常" }, 
						[CLIENT_DIR_BUSY]		= { color = { r = 168, g = 104, b = 57 },	stateDesc = "繁忙" },
						[CLIENT_DIR_FULL]		= { color = { r = 167, g = 50, b = 50 },	stateDesc = "爆满" },
						[CLIENT_DIR_NOT_WORK]	= { color = { r = 72, g = 109, b = 9 },		stateDesc = "维护状态" },
					};

function BirthPlaceFrame_OnEvent()
	if arg1=="GE_SELECTBIRTH_SHOW" then
		local selectbirth = LoginMgr:getSelectBirthLocation();
		
		for i = 1, math.min( MAX_BIRTH_BTN, selectbirth.BirthMapNum ) do
			local MinMapContal	= MapShower:GetMinMapContal();
			local mapname		= MinMapContal:getMapName(selectbirth.MapIDList[i-1]);
			local nBusyType		= selectbirth.Busy[i-1];
			local btn = getglobal("BirthBtn"..i);
			local stateFont		= getglobal( "BirthBtn"..i.."BusyStateFont" );
			--local mapNameFont	= getglobal( "BirthBtn"..i.."MapNameFont" );
			local mapNameFont	= getglobal( "BirthBtn"..i );

			local t_fonts = {	[mapNameFont:GetName()]	= { value = mapname,  },
								[stateFont:GetName()]	= { value = " ("..t_busyColor[nBusyType]["stateDesc"]..") ", 
															color = t_busyColor[nBusyType]["color"] },
							};

			util.SetFonts( t_fonts );
			btn:DisChecked();

			stateFont:SetPoint( "topleft", mapNameFont:GetName(), "topleft", 
									mapNameFont:GetTextExtentWidth( mapNameFont:GetText() ) / GetScreenScaleY() + 32, 8 );
			btn:Show();
		end

		BirthPlaceFrame:Show();
		nSelectBirthPlaceId = 0;
		for i = math.min( MAX_BIRTH_BTN, selectbirth.BirthMapNum ) + 1, MAX_BIRTH_BTN do
			local btn = getglobal("BirthBtn"..i);
			btn:Hide();
		end

		BirthBtn1:Checked();
	end
end

function BirthPlaceFrame_OnShow()
	ENTERCLICK_SECOND = 0;
end

function BirthPlaceFrame_OnHide()
end

function BirthFrame_OnClickBirthBtn()
	for i = 1, MAX_BIRTH_BTN do
		local btn = getglobal("BirthBtn"..i);
		btn:DisChecked();
	end
	this:Checked();
	nSelectBirthPlaceId = this:GetClientID() - 1; 
end

function BirthFrame_OnDoubleClick()
	BirthFrame_OnClickBirthBtn();
	BirthPlaceEnterBtn_OnClick();
end

--登录新手村确定按钮
function BirthPlaceEnterBtn_OnClick()
	BirthPlaceFrame:Hide();	  
	LoginMgr:requestSelectBirthLocation( nSelectBirthPlaceId );
end

--选择取消新手村按钮
function BirthPlaceCanceBtn_OnClick()
	BirthPlaceFrame:Hide();	  
	LoginMgr:requestCancelSelect();
end

-- CORE: Update
function Private_ClearSelectFrameNameList()
	for i = 1, MAX_ROLES_NUM_CREATED do
		local btn = getglobal("CSChangeButton"..i);
		btn:Hide();
	end

	DisCheckAllBtn( "CSChangeButton", MAX_ROLES_NUM_CREATED );
end

function Private_UpdateSelectFrameNameList()
	Private_ClearSelectFrameNameList();

	local t_SelectFrameControl	= GetSelectFrameControl();
	local nCurPage				= t_SelectFrameControl:getCurPage();

	local nBtnIndex		= 1;
	local MinMapContal	= MapShower:GetMinMapContal();
	for i= ( nCurPage - 1 ) * MAX_ROLES_NUM_CREATED + 1, LoginMgr:getNumRole() do
		if nBtnIndex > MAX_ROLES_NUM_CREATED then
			break;
		end

		local btn = getglobal( "CSChangeButton"..nBtnIndex );
		btn:SetClientUserData( 0, i - 1 );
		btn:Show();

		local role = LoginMgr:getIthRole( i - 1 );
		local szMapName = "新手村";
		if role.map ~= 0 and MinMapContal:getMapName(role.map) ~= nil then
			szMapName = MinMapContal:getMapName( role.map );
		end
		-- "MMMMMMMMMMMMMMM"
		local t_GrayColor = {r=120,g=120,b=120};
		local szdelTime = "";
		local mapFont = getglobal(btn:GetName() .. "CSMap");
		local cancelBtn = getglobal(btn:GetName() .."CancelDelBtn");
		local hairPhotoTex = getglobal( btn:GetName().."hairPhotoTex" );
		local PhotoTex = getglobal( btn:GetName().."PhotoTex" );
		local PhotoHairShadowTex = getglobal( btn:GetName().."PhotoHairShadowTex" );
		local PhotoBodyTex = getglobal( btn:GetName().."PhotoBodyTex" );
		if role.delTime > 0 then
			local nLeftTime = math.ceil( ( DEL_ROLE_PROTECT_TM_TOLUA - (GameClanManager:getServerTime() - role.delTime) )/60 );
			if nLeftTime <= 0 then nLeftTime = 0 end
			szdelTime = "删除倒计时："
			if nLeftTime > 60 then
				szdelTime = szdelTime .. math.floor( nLeftTime/60 ) .. "小时";
			else
				szdelTime = szdelTime .. nLeftTime .. "分钟";
			end
			mapFont:Hide();
			cancelBtn:Show();
			btn:SetGray( true );
			hairPhotoTex:SetGray( true );
			PhotoTex:SetGray( true );
			PhotoHairShadowTex:SetGray( true );
			PhotoBodyTex:SetGray( true );
		else
			mapFont:Show();
			cancelBtn:Hide();
			btn:SetGray( false );
			hairPhotoTex:SetGray( false );
			PhotoTex:SetGray( false );
			PhotoHairShadowTex:SetGray( false );
			PhotoBodyTex:SetGray( false );
		end
		local t_fonts = {	[btn:GetName().."CSName" ]	= { value = role.name, color=( i <= 4 and {r=255,g=255,b=0} ) or t_GrayColor },
							[btn:GetName().."CSLevel"]	= { value = role.level.."级", color=( i <= 4 and {r=255,g=255,b=190} ) or t_GrayColor }, 
							[btn:GetName().."CSRace"]	= { value = GetCareerName(role.race), color=( i <= 4 and {r=255,g=255,b=190} ) or t_GrayColor }, 
							[btn:GetName().."CSMap"]	= { value = ( i <= 4 and szMapName ) or ( szMapName .. " (合服隐藏)" ), color=( i <= 4 and {r=255,g=255,b=190} ) or t_GrayColor }, 
							[btn:GetName().."DelTime"]	= { value = szdelTime , color={r=255,g=0,b=0} },
						};
		util.SetFonts( t_fonts );

		SetPhoto( role.hair, role.face, btn:GetName().."hairPhotoTex", btn:GetName().."PhotoTex", role.gender, role.head,
					btn:GetName().."PhotoHairShadowTex", btn:GetName().."PhotoBodyTex" );
		
		if ( i - 1 ) == LoginMgr:getSelectRoleIndex() then
			btn:Checked();
		end

		nBtnIndex = nBtnIndex + 1;
	end
	
	Privaete_UpdateCreateButtonAndTurnPageBtn();
end

-- 更新当前选中的角色按钮
function Private_UpdateSelectRoleBtn()
	DisCheckAllBtn( "CSChangeButton", MAX_ROLES_NUM_CREATED );
	for i = 1, MAX_ROLES_NUM_CREATED do
		local btn = getglobal( "CSChangeButton"..i );
		local nBtnRoleIndex = btn:GetClientUserData( 0 );
		if nBtnRoleIndex == LoginMgr:getSelectRoleIndex() then
			btn:Checked();
			break;
		end
	end
end

function Private_UpdateDeleteAndEnterBtn()
	SelectFrame_DeleteButton:Enable();
	SelectFrame_EnterGameButton:Enable();
	local nLoginSelectRoleIndex = LoginMgr:getSelectRoleIndex();
	if nLoginSelectRoleIndex >= 0 then
		local roleInfo = LoginMgr:getIthRole( nLoginSelectRoleIndex );
		if roleInfo.delTime > 0 then
			SelectFrame_DeleteButton:Disable();
			SelectFrame_EnterGameButton:Disable();
		end
	end
end

-- 更新当前页数和最大页数
function Private_UpdatePageNum()
	local t_SelectFrameControl	= GetSelectFrameControl();
	local nCurPage				= t_SelectFrameControl:getCurPage();
	local nMaxPage				= math.ceil( LoginMgr:getNumRole() / MAX_ROLES_NUM_CREATED );
	nMaxPage					= math.max( nMaxPage, 1 );
	local t_fonts = {
						["SelectFrame_PageBtnFont"]	= { value = nCurPage.." / "..nMaxPage },
					};
				
	 util.SetFonts( t_fonts );
end
-- CORE: Update
function CharSelectFrame_OnEvent()
	if arg1=="GE_UPDATE_ROLELIST" then
		if getGameState() == CLIENT_STATE_INWORLD then
			ShowChangeRoleFrame:Hide();
			GameMgr:returnLogin();
			GameBackground:playCameraAnim(99102, 1000);
			GameBackground:setViewFlag( VIEW_POINT_SELECT );
			GameBackground:show();
		end
		ClearOtherWorldRoleData()
		local nLoginSelectRoleIndex = LoginMgr:getSelectRoleIndex();
		if szClickName ~= "" then
			for i=1,LoginMgr:getNumRole() do
				local role = LoginMgr:getIthRole( i-1 )
				if role.name == szClickName then
					LoginMgr:selectRole( i-1 )
					nLoginSelectRoleIndex = i-1
					break
				end
			end
		end
		szClickName = ""

		local nCurPage				= math.max( math.floor( ( nLoginSelectRoleIndex ) / MAX_ROLES_NUM_CREATED ), 0 ) + 1;
		local t_SelectFrameControl	= GetSelectFrameControl();
		t_SelectFrameControl:setCurPage( { ["page"] = nCurPage } );
		Private_UpdatePageNum();

		Private_UpdateSelectFrameNameList();
		Privaete_UpdateCreateButtonAndTurnPageBtn();

		if ( LoginMgr:getNumRole() == 0 ) then
			GameBackground:releaseEntity(0); --隐藏模型 
			local tex = getglobal( "SelectFrame_CreateButtonUVAnimationTex" );
			tex:SetUVAnimation(80, true);
			tex:SetPoint("center","SelectFrame_CreateButton","center",5,1)
			tex:Show();
			SelectFrameCreateTip:SetText("\t\t\t该区未创建任何角色\n请点击下方【创建角色】进行创建");
			SelectFrameCreateTip:SetPoint("center","SelectFrameBackGround","center",0,0);
			bNeedShowOtherWorld = true
		else
			local tex = getglobal( "SelectFrame_CreateButtonUVAnimationTex" );
			tex:Hide();
			SelectFrameCreateTip:SetText("");
		end
		Private_UpdateDeleteAndEnterBtn();

		local regionName,worldName = SelectWorldInfo.bigworld,SelectWorldInfo.world
		for i = 1, LoginMgr:getNumBigWorld() do
			local bigWorldInfo = LoginMgr:getBigWorld( i - 1 );
			for j = 1, bigWorldInfo:getNumWorld() do
				local oneWorldInfo = bigWorldInfo:getIthWorld( j - 1 );	
				if oneWorldInfo.worldid  == LoginMgr:getLastSelectWorldId() 
					and oneWorldInfo.RegionID == LoginMgr:getLastSelectRegionId() then
					regionName = bigWorldInfo:getName()
					worldName = oneWorldInfo.name
					break
				end
			end
		end

		local t_fonts = {	["SelectFrameServerName"] = { value = regionName },
						["SelectFrameAreaName"] = { value = worldName },
						};
		util.SetFonts( t_fonts );
		if SelectFrame:IsShown() then
			Privaete_UpdateCharSelectFrame()
		end
	end


	if arg1=="GE_UPDATE_ROLEDEL" then		
		if SelectFrame:IsShown() then
			Privaete_UpdateCharSelectFrame();
		end
	end

	if arg1=="GE_ROLE_DELFAILRESULT" then
		local nRet = LoginMgr:getDelRoleResult();
		if nRet == DEL_ERR then
			Login_MessageBoxOK( { ["text"] =  "删除失败,请稍候重试！", ["opDesc"] = "" } );
	    elseif nRet == DEL_ERR_CLAN_LEADER then
			Login_MessageBoxOK( { ["text"] =  "角色是公会会长，不允许被删！", ["opDesc"] = "" } );
		end
	end
end

function SetAvatarAlpha(alpha, fadetick, exclude_index)
	for i=0, 9 do
		if i~=exclude_index then
			GameBackground:fadeEntity(i+1, alpha, fadetick);
		end
	end

	if exclude_index~=-1 then
		GameBackground:fadeEntity(exclude_index+1, 1.0, 500);

		if exclude_index == 6 then
			GameBackground:fadeEntity(9, 1.0, 500);
		end
		if exclude_index == 7 then
			GameBackground:fadeEntity(10, 1.0, 500);
		end
	end
end

local SelectAvatarTick = -1;
local SelectAvatarIndex = -1;
function SelectAvatar_Update()
	if SelectAvatarTick >= 0 then
		curtick = GameMgr:getTickTime()/1000;
		if curtick >= SelectAvatarTick+0.6 then
			SetAvatarAlpha(0, 1000, SelectAvatarIndex);
			SelectAvatarTick = -1;
		end
	end
end

function DisCheckRaceTex()
	local t_racetex =
		{
			"CSWarriorRaceTexButton",
			"CSChurchRaceTexButton",
			"CSGunnerRaceTexButton",
			"CSMagicianRaceTexButton",
			"CSWarriorMaleButton",
			"CSWarriorFeMaleButton",
			"CSChurchMaleButton",
			"CSChurchFeMaleButton",
			"CSGunnerMaleButton",
			"CSGunnerFeMaleButton",
			"CSMagicianMaleButton",
			"CSMagicianFeMaleButton",
		};
	for i = 1, table.getn(t_racetex) do
		local racebtn = getglobal( t_racetex[i] );
		racebtn:DisChecked();
	end
end

NudeAvatarLoaded = false;
NudeAvatarPos =
{
	{5854, 250, 3654}, -- 男战
	{6401, 252, 3679}, -- 男法
	{6040, 252, 3603}, -- 男枪
	{6228, 251, 3679}, -- 男牧
	{5980, 250, 3560}, -- 女战
	{6424, 252, 3600}, -- 女法
	{6126, 254, 3480}, -- 女枪
	{6245, 254, 3557}, -- 女牧
	{6128, 252, 3514}, -- 女枪箱
	{6254, 248, 3566}, -- 女牧箱
	{5332, 250, 3648},  -- 主角
};

AvatarClothes = {2019902, 2039900, 2029901, 2049900};
AvatarWeapon = {1031602, 1219900, 1109904, 1109901};
AvatarRace = {0, 1, 2, 3};
AvatarGender = {0, 1};
AvatarModelId = {202100,202103,202101,202102,202104,202107,202105,202106,202110,202111};

function IndexFromRaceGender(race, gender)
	return gender*4+race;
end

-- 初始化角色模型
function InitNudeAvatar()
	if NudeAvatarLoaded == false then
		SetNudeAvatar(0, 0);
		SetNudeAvatar(1, 0);
		SetNudeAvatar(2, 0);
		SetNudeAvatar(3, 0);
		SetNudeAvatar(0, 1);
		SetNudeAvatar(1, 1);
		SetNudeAvatar(2, 1);
		SetNudeAvatar(3, 1);
		SetNudeAvatar(4, 1);
		SetNudeAvatar(5, 1);
		NudeAvatarLoaded = true;
		SetAvatarAlpha(0, 0, -1);
	end
end

function SetNudeAvatar(race, gender)
	i = IndexFromRaceGender(race, gender);
	local AnimId			= 10100;
	local name = gender > 0 and "female" or "male";
	name = race > 3 and AvatarModelId[i+1] or name;
	local szModelPath		= "character\\monster\\"..AvatarModelId[i+1].."\\"..name..".omod";
	GameBackground:setPetModel(i+1,AnimId,szModelPath,nil,AvatarModelId[i+1]);
	pos = NudeAvatarPos[i+1];
	GameBackground:setEntityPosition(i+1, pos[1], pos[2], pos[3]);
	local t_Rotate=
	{
		{ CLIENT_CAREER_MAGICOR - 1,	CLIENT_GENDER_MALE - 1,	40		}, -- 男法师角度调整
		--{ CLIENT_CAREER_CHURCH	- 1,	CLIENT_GENDER_MALE - 1,	-31		}, -- 男牧师角度调整
		--{ CLIENT_CAREER_WARRIOR	- 1,	CLIENT_GENDER_MALE - 1,	40		}, -- 男战士角度调整
	}
	for i = 1, table.getn( t_Rotate ) do
		if t_Rotate[i][1] == race then
			GameBackground:setEntityRotate( IndexFromRaceGender( t_Rotate[i][1],t_Rotate[i][2] )+1, t_Rotate[i][3] );--角度调整
		end
	end										   
end

function SetModelMaskWinPos()
	local scale = GetScreenScaleY();
	local t_MastPos = 
				{
					{ ["race"] = CLIENT_CAREER_WARRIOR-1,	["gender"] = CLIENT_GENDER_MALE-1,		["button"]	= "CSWarriorMaleButton_Select",		["offsetx"] = -70, ["offsety"] = -15, ["w"] = 121, ["h"] = 380 },
					{ ["race"] = CLIENT_CAREER_WARRIOR-1,	["gender"] = CLIENT_GENDER_FEMALE-1,	["button"]	= "CSWarriorFeMaleButton_Select",	["offsetx"] = -70, ["offsety"] = -15, ["w"] = 110, ["h"] = 430},
					{ ["race"] = CLIENT_CAREER_MAGICOR-1,	["gender"] = CLIENT_GENDER_MALE-1,		["button"]	= "CSMagicianMaleButton_Select",	["offsetx"] = -100, ["offsety"] = -15, ["w"] = 190, ["h"] = 320 },
					{ ["race"] = CLIENT_CAREER_MAGICOR-1,	["gender"] = CLIENT_GENDER_FEMALE-1,	["button"]	= "CSMagicianFeMaleButton_Select",	["offsetx"] = -50, ["offsety"] = -15, ["w"] = 150, ["h"] = 380 },
					{ ["race"] = CLIENT_CAREER_GUNER-1,	["gender"] = CLIENT_GENDER_MALE-1,		["button"]	= "CSGunnerMaleButton_Select",		["offsetx"] = -100, ["offsety"] = -15, ["w"] = 125, ["h"] = 255 },
					{ ["race"] = CLIENT_CAREER_GUNER-1,	["gender"] = CLIENT_GENDER_FEMALE-1,	["button"]	= "CSGunnerFeMaleButton_Select",	["offsetx"] = -30, ["offsety"] = 50, ["w"] = 75, ["h"] = 235 },
					{ ["race"] = CLIENT_CAREER_CHURCH-1,	["gender"] = CLIENT_GENDER_MALE-1,		["button"]	= "CSChurchMaleButton_Select",		["offsetx"] = -50, ["offsety"] = -15, ["w"] = 90, ["h"] = 340 },
					{ ["race"] = CLIENT_CAREER_CHURCH-1,	["gender"] = CLIENT_GENDER_FEMALE-1,	["button"]	= "CSChurchFeMaleButton_Select",	["offsetx"] = -50, ["offsety"] = -15, ["w"] = 145, ["h"] = 405 },
				};

	for i=1, table.getn( t_MastPos ) do
		local btn		= getglobal( t_MastPos[i].button );
		local winpos	= GameBackground:getEntityWinPos( IndexFromRaceGender( t_MastPos[i].race, t_MastPos[i].gender ) + 1 );
		btn:SetPoint("TOPLEFT","$parent", "TOPLEFT", winpos.m_nPosX/scale + t_MastPos[i].offsetx, winpos.m_nPosY/scale + t_MastPos[i].offsety);
	end
end

function SelectFrame_UpdateAvatar( data )
	local nRoleIndex = data["roleIndex"];
	SelectFrame_EnterGameButton:Enable();
	LoginMgr:setAvatarAttrib( nRoleIndex );	

	for j = 1, CLIENT_MAX_AVATAR_ATTRIB do
		local nGridAttribID =getAvatarAttrib( j-1 );
		if nGridAttribID ~= 0 then
			local nAvartarGridStarLv = getAvatarStarLv( j - 1 );
			if j == 1 then
				GameBackground:setAvatarAttrib(0, j-1, nGridAttribID, nAvartarGridStarLv, true );
			else
				GameBackground:setAvatarAttrib(0, j-1, nGridAttribID, nAvartarGridStarLv );
			end
		end
	end
	
	local loginRoleInfo		= LoginMgr:getIthRole( LoginMgr:getSelectRoleIndex() );
	--[[
	local nShould			= getAvatarAttrib( POS_ID_SHOULD );	
	GameBackground:setAvatarAttrib( 0, POS_ID_SHOULD, nShould );
	]]--
	GameBackground:updateAttrib( 0 );

	local pos	= NudeAvatarPos[11];
	GameBackground:setEntityAvatar( 0 );
	ModelAngle	= 0;
	GameBackground:setEntityRotate(0,0);
	GameBackground:setEntityScale(0, 1.2, 1.2, 1.2);
	GameBackground:setEntityPosition( 0, pos[1], pos[2], pos[3] );

	local leftarmid		= getAvatarAttrib( POS_ID_LEFTHAND );
	local rightarmid	= getAvatarAttrib( POS_ID_RIGHTHAND );

	local t_AvartarControl	= GetAvartarControl();
	--print( "leftarmid = "..leftarmid.."， rightarmid = "..rightarmid.."--488" );
	local nAnimID = t_AvartarControl:getAvatarAnim( {	["race"] = GetInitRace( loginRoleInfo.race ), 
														["leftHandItem"] = leftarmid, ["rightHandItem"] = rightarmid } );
	GameBackground:playEntityAnim( 0, nAnimID, nil );
	freeActionTime = GameMgr:getTickTime() / 1000;
end

function GetFreePosState( armid )
	if armid ~= 0 then
		local arm_sub_type  = math.floor(armid/10000);
		local arm_race_type = math.floor(armid/100000);
		if arm_race_type == ARM_GUN then
			return -1;
		end
		for i = 1, table.getn( t_posestate ) do
			if arm_race_type == t_posestate[i][1] and arm_sub_type == t_posestate[i][2] then
				if t_posestate[i][3]  > POSE_END then
					return 10103;
				else
					return (tonumber(t_posestate[i][3])*1000)+100+tonumber(t_posestate[i][5]);
				end
			end
		end
	end
	return -1;
end

function GetPoseState( armid )
	if armid ~= 0 then
		local arm_sub_type  = math.floor(armid/10000);
		local arm_race_type = math.floor(armid/100000);
		for i = 1, table.getn( t_posestate ) do
			if arm_race_type == t_posestate[i][1] and arm_sub_type == t_posestate[i][2] then
				if t_posestate[i][3]  > POSE_END then
					return 10108; -- 各职业的基本备战动作 后三位108
				else
					return (tonumber(t_posestate[i][3])*1000)+100+tonumber(t_posestate[i][4]); -- 各职业的穿装备的备战动作 后三位108
				end
			end
		end
	end

	return 10108;
end

function ShowNewAvatar( nRace, nGender )
	--local t_fashionid  = { 3010000,3020000,3030000,3040000 };
	local t_lefthandid = { 1019900,1109900,1209900,1509900 };
	local pos = NudeAvatarPos[11];
	--print( "ShowNewAvatar--537" );
	--清空avatar属性
	GameBackground:setAvatarAttrib(0, 0, 0, 0, true );
	GameBackground:setEntityAvatar(0);
	GameBackground:setAvatarAttrib(0, 0, nRace, 0, true );
	GameBackground:setAvatarAttrib(0, 1, (nGender-1) );
	--GameBackground:setAvatarAttrib(0, POS_ID_FASHION_BODY, t_fashionid[nRace] );
	GameBackground:setAvatarAttrib(0, POS_ID_LEFTHAND, t_lefthandid[nRace] );
	GameBackground:updateAttrib( 0 );
	
	local t_AvartarControl = GetAvartarControl();
	local nAnimID = t_AvartarControl:getAvatarAnim( {	["race"] = GetInitRace( nRace ), 
														["leftHandItem"] = t_lefthandid[nRace], ["rightHandItem"] = 0 } );
	GameBackground:playEntityAnim( 0, nAnimID, nil );
	--GameBackground:playEntityAnim(0, GetPoseState(t_lefthandid[nRace]), nil ); -- 各职业的基本备战动作 后三位108
	SetAvatarAlpha(0, 0, -1);
	GameBackground:setViewFlag( VIEW_POINT_SELECT );
	GameBackground:playCameraAnim(99114, 0);
	GameBackground:playCameraAnim(99102, 1000);
	GameBackground:setEntityRotate(0,0);
	GameBackground:setEntityScale(0, 1.2, 1.2, 1.2);
	GameBackground:setEntityPosition( 0, pos[1], pos[2], pos[3] );
	freeActionTime = GameMgr:getTickTime()/1000;
end

function ShowAvatar11( nRace, nGender )
	if nRace==0 or nGender==0 then return end;
	ar = AvatarRace[nRace];
	ag = AvatarGender[nGender];
	index = IndexFromRaceGender(ar,ag);
	GameBackground:playCameraAnim(99105+index, 1000);
	GameBackground:playEntityAnim(index+1, 10180, AvatarModelId[index+1] );
	SetAvatarAlpha(1.0, 200, -1);
	SelectAvatarIndex = index;
	SelectAvatarTick = GameMgr:getTickTime()/1000;
	ModelAngle = 0;

	CSSelectRaceDescFrameRich:SetText( "#P"..t_racedesc[nRace][1],255,255,190);
	CSSelectRaceDescFrameRich:AddText( t_racedesc[nRace][2],255,255,190);
	CSSelectRaceDescFrameRich:AddText( t_racedesc[nRace][3],0,255,255);
	CSSelectRaceDescFrameRich:AddText( t_racedesc[nRace][4],0,255,255);
	CSCreateFrameBackGroundRaceName:SetText( GetCareerName(nRace).."  ("..GetGender(nGender)..")");
	local t_RaceAttr =	{
					[g_nCAREER_WARRIOR] =	{ 
									[1] = { attr = "力量",  value = 6, maxvalue = 8 }, 
									[2] = { attr = "智力",  value = 1, maxvalue = 8 },
									[3] = { attr = "体质",  value = 5, maxvalue = 8 },
									[4] = { attr = "精神",  value = 3, maxvalue = 8 },
									[5] = { attr = "敏捷",  value = 4, maxvalue = 8 },
									[6] = { attr = "感知",  value = 6, maxvalue = 8 },
								},
					[g_nCAREER_MAGICOR] =	{ 
									[1] = { attr = "力量",  value = 1, maxvalue = 8 }, 
									[2] = { attr = "智力",  value = 8, maxvalue = 8 },
									[3] = { attr = "体质",  value = 4, maxvalue = 8 },
									[4] = { attr = "精神",  value = 4, maxvalue = 8 },
									[5] = { attr = "敏捷",  value = 3, maxvalue = 8 },
									[6] = { attr = "感知",  value = 6, maxvalue = 8 },
								},
					[g_nCAREER_GUNER] =	{ 
									[1] = { attr = "力量",  value = 7, maxvalue = 8 }, 
									[2] = { attr = "智力",  value = 5, maxvalue = 8 },
									[3] = { attr = "体质",  value = 4, maxvalue = 8 },
									[4] = { attr = "精神",  value = 3, maxvalue = 8 },
									[5] = { attr = "敏捷",  value = 4, maxvalue = 8 },
									[6] = { attr = "感知",  value = 6, maxvalue = 8 },
								},
					[g_nCAREER_CHURCH] =	{ 
									[1] = { attr = "力量",  value = 1, maxvalue = 8 }, 
									[2] = { attr = "智力",  value = 6, maxvalue = 8 },
									[3] = { attr = "体质",  value = 4, maxvalue = 8 },
									[4] = { attr = "精神",  value = 4, maxvalue = 8 },
									[5] = { attr = "敏捷",  value = 3, maxvalue = 8 },
									[6] = { attr = "感知",  value = 6, maxvalue = 8 },
								},
				}
	for i = 1, MAX_ATTR_BTN do
		local btn		= getglobal("RaceAttrButton"..i);
		local font		= getglobal(btn:GetName().."NameFont");
		local valuefont = getglobal(btn:GetName().."ValueFont");
		local valuetex	= getglobal(btn:GetName().."value");
	
		btn:SetClientString( t_RaceAttr[nRace][i].attr );
		font:SetText( t_RaceAttr[nRace][i].attr );
		valuefont:SetText( t_RaceAttr[nRace][i].value );
	
		valuetex:SetSize( 136*(t_RaceAttr[nRace][i].value/ t_RaceAttr[nRace][i].maxvalue), 9 );
	end	
end
--RaceAttrButton1value
function RaceAttrButton_OnEnter()
	local t_Tip =	{ 
						["力量"] = "1点力量增加5点物理攻击", -- 这改成智力
						["体质"] = "1点体质增加25点生命值\n提升非战斗中，生命值每3秒回复效果",
						["精神"] = "1点精神增加25点魔法值\n提升非战斗中，魔法值每3秒回复效果",
						["敏捷"] = "1点敏捷增加5点物理防御",
						["闪躲"] = "1点闪躲增加10点闪避",
						["感知"] = "1点感知增加5点魔法防御",
						["智力"] = "1点智力增加5点魔法攻击",
					};
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = t_Tip[this:GetClientString()], frame = this:GetParent(),
						button = this:GetName() } );
end

function CSCreateFrame_OnLoad()
	this:setUpdateTime( 0 );

	local backBtn = getglobal( this:GetName().."BackGround" );
	local t_relFrames = {	this:GetName().."RaceDescBtn", 
							this:GetName().."RoleNameHeadBtn", "CSCreateNameEditBackBtn", "CSCreateName", 
							this:GetName().."HairStyleHeadBtn", "CSCreateHairStyleEditBackBtn", "CSHairstyleReduceButton", "CSHairstyleAddButton",
							this:GetName().."HairColorHeadBtn", "CSCreateHairColorEditBackBtn", "CSHairColorReduceButton", "CSHairColorAddButton",
							this:GetName().."FaceStyleHeadBtn", "CSCreateFaceEditBackBtn", "CSFaceReduceButton", "CSFaceAddButton", 
							this:GetName().."PhotoBtn", };
							--this:GetName().."ClothesStyleHeadBtn", this:GetName().."ClothesStyleBackBtn", "CSCreateClothesReduceButton",
							--"CSCreateClothesAddButton", this:GetName().."PhotoBtn", };
	
	for _, name in ipairs( t_relFrames ) do
		local btn = getglobal( name );
		backBtn:AddRelFrame( btn );
	end
end

function CSCreateFrame_OnShow()
	local t_SelectFrameMoveFrameAnimControl = GetSelectFrameMoveFrameAnimControl();
	t_SelectFrameMoveFrameAnimControl:begin( { ["curAnimframe"] = this:GetName() } );
end

function CSCreateFrame_OnUpdate()
	local t_SelectFrameMoveFrameAnimControl = GetSelectFrameMoveFrameAnimControl();
	t_SelectFrameMoveFrameAnimControl:update();
end

function CSCreateFrame_OnHide()
	CSCreateName:Clear();
	ClearFocus();
	GameBackground:releaseEntity(0);
	CSCreateName:enableIME( false ); 

	local nLoginSelectRoleIndex = LoginMgr:getSelectRoleIndex();
	if nLoginSelectRoleIndex >= 0 then
		SelectFrame_UpdateAvatar( { ["roleIndex"] = nLoginSelectRoleIndex, } );
	end
	bCreatNewRoleFrame = false;
	nShowSelectNewFlag = 0;
	GameBackground:ChangeAnimModelConfig( CLIENT_GENDER_NONE, CLIENT_GENDER_NONE );
	--print( "CSCreateFrame_OnHide--699" );
	local t_SelectFrameMoveFrameAnimControl = GetSelectFrameMoveFrameAnimControl();
	if t_SelectFrameMoveFrameAnimControl:isPlayingAnim() then
		t_SelectFrameMoveFrameAnimControl:init( { ["curAnimframe"] = this:GetName() } );
	end
	Private_UpdateDeleteAndEnterBtn();
end

function CSCreateFrame_OnMouseWheel()
	--GameMgr:PullInOutCamear( arg1 );
end

-- <!--选人角色-->--------------------------------------------------------------------------------------------------------------------------------
-- 角色选择

-- 新角色选择
local nShowSelectNewFlag = 0;

-- 角色创建
local bCreatNewRoleFrame = false;
-- 背景音乐
local nMusicTickTime = 0;

function CharacterSelectFrame_OnShow()
	if CSFashionFrame:IsShown() then
		CSFashionFrame:Hide();
	end
	CharacterSelectFrameBkgTex:Hide();
	szClickName = ""
	nSelectGender	= 0;
	nSelectRace	= 0;
	nMusicTickTime = GameMgr:getTickTime()/1000;

	if LoginMgr:getNumRole() ~= 0 then
		if not LoginMgr:isBackFromInWorld() then
			nShowStartTime = GameMgr:getTickTime()/1000;
		end
		nShowSelectFrameFlag = 1;
		onGetRoleList();
				
		local nLoginSelectRoleIndex = LoginMgr:getSelectRoleIndex();
		if nLoginSelectRoleIndex ~= -1 then
			SelectFrame_UpdateAvatar( { ["roleIndex"] = nLoginSelectRoleIndex, } );
		end
	else
		-- 显示角色选择界面
		LoginBackFrame:Hide();
		nShowSelectFrameFlag = 0;
		InitNudeAvatar();
		ShowRoleEntity();
		nShowNewRoleStartTime = GameMgr:getTickTime()/1000;
		nShowSelectNewFlag = 3;

		CSSelectNextStepButton:Hide();
		CSSelectPerStepButton:SetPoint( "Bottom", "CSSelectRoleBackGroundFrame", "Bottom", 0, -24 );
		GameBackground:setViewFlag( VIEW_POINT_CREATE );
		GameBackground:playCameraAnim(99103, 0);
		GameBackground:playCameraAnim(99104, 1000);
		needTurnToCreate = false;
		PrebuildMobileCheckFrame:Hide();
	end
end

function CharacterSelectFrame_OnUpdate()
	local t_chgSizeIcons = 
	{
	[this:GetName().."TopZheZhaoTex"]	=	{	width	= GetScreenWidth() / GetScreenScaleY(),	
												height	= 77 
											},

	[this:GetName().."BottomZheZhaoTex"]	=	{	width	= GetScreenWidth() / GetScreenScaleY(),	
													height	= 92 
												}
	};	
	util.SetIconsSize( t_chgSizeIcons );

	SelectAvatar_Update();

	-- 延迟显示UI
	local times = GameMgr:getTickTime()/1000 - nShowStartTime;	
	if nShowSelectFrameFlag > 0 then 
		--从游戏中返回处理不一样
		local tick = nShowSelectFrameFlag > 1 and 5 or 2;
		if times > tick then
			local frame = getglobal( "SelectFrame" );
			if not frame:IsShown() then
				frame:Show();
			else
				Privaete_UpdateCharSelectFrame();
			end

			LoginBackFrame:Hide();
			nShowSelectFrameFlag = 0;
		else
			local alpha = ( 3 - times )/3;

			local t_chgAlphaTex = { "LoginBackFrameBackgroundTex" };
			for _, name in ipairs( t_chgAlphaTex ) do
				local tex = getglobal( name );
				tex:SetBlendAlpha(alpha);
			end

			local t_chgAlphaFont = { "LoginBackFrameBackgroundFont1", "LoginBackFrameBackgroundFont2" };
			for _, name in ipairs( t_chgAlphaFont ) do
				local font = getglobal( name );
				font:SetBlendAlpha(alpha);
			end
		end
	end

	if NeedPlayBGM() then
		local times = GameMgr:getTickTime()/1000 - nMusicTickTime;
		--延迟一秒播放背景音乐
		if GameBackground:isShown() and times > 1 then
			GameBackground:playBGM( 0 );
			SetPlayBGMflag( false );
		end
	end

	if nShowSelectNewFlag > 0 then
		local newroletimes = GameMgr:getTickTime()/1000 - nShowNewRoleStartTime;
		if nShowSelectNewFlag < 2 and newroletimes > 2 then
			nShowSelectNewFlag = 2;
			ShowRoleEntity();
		elseif nShowSelectNewFlag == 3 and newroletimes > 1.6 then
			local t_showIcons = {	"CharacterSelectFrameTopZheZhaoTex",		"CharacterSelectFrameBottomZheZhaoTex",				
									"CharacterSelectFrameLeftZheZhaoChiLunTex",	"CharacterSelectFrameRightZheZhaoChiLunTex" };
			util.ShowIcons( t_showIcons );
			CSSelectRoleFrame:Show();
			CSSelectRoleBackGroundFrame:Show();
			nShowSelectNewFlag = 0;
		elseif newroletimes > 4 then
			CSSelectRoleFrame:Show();
			CSSelectRoleBackGroundFrame:Show();
			nShowSelectNewFlag = 0;
		end
	end

	local creatroletimes = GameMgr:getTickTime()/1000 - nCreatNewRoleStartTime;
	if creatroletimes > 4 and bCreatNewRoleFrame then
		CSCreateFrame:Show();
		bCreatNewRoleFrame = false;
		nShowSelectNewFlag = 0;
	end
	RoleAction_Update();

	-- 刷新删除时间
	local nCurPage = t_SelectFrameControl:getCurPage();
	local nBtnIndex = 1;
	for i= ( nCurPage - 1 ) * MAX_ROLES_NUM_CREATED + 1, LoginMgr:getNumRole() do
		local font = getglobal( "CSChangeButton"..nBtnIndex.."DelTime" );
		nBtnIndex = nBtnIndex + 1;
		local role = LoginMgr:getIthRole( i - 1 );
		if role.delTime > 0 then
			local nLeftTime = math.ceil( ( DEL_ROLE_PROTECT_TM_TOLUA - (GameClanManager:getServerTime() - role.delTime) )/60 );
			if nLeftTime <= 0 then nLeftTime = 0 end
			szdelTime = "删除倒计时："
			if nLeftTime > 60 then
				szdelTime = szdelTime .. math.floor( nLeftTime/60 ) .. "小时";
			else
				szdelTime = szdelTime .. nLeftTime .."分钟";
			end
			font:SetText( szdelTime );
		end		
	end

	if not bOtherFinish and GameGetAccountAllRole:IsFinish() then
		bOtherFinish = true
		if not bNeedShowOtherWorld then
			-- 比较last,
			local last,lastRegion,lastWorld = 0
			local num = GameGetAccountAllRole:GetWorldRoleInfoNum()
			for i=1,num do
				local info = GameGetAccountAllRole:getWorldRoleInfoByIdx( i-1 )
				if info.roleInfo.Succ == 1 and info.roleInfo.Num > 0 then
					for j=1,info.roleInfo.Num do
						local role = info.roleInfo.RoleInfo[j-1]
						if role.LastLogout > last then
							last = role.LastLogout
							lastRegion = info.roleInfo.RegionID
							lastWorld = role.WorldID
						end
					end
				end
			end
			if last > 0 and not( lastRegion == LoginMgr:getLastSelectRegionId() and lastWorld == LoginMgr:getLastSelectWorldId() ) then
				bNeedShowOtherWorld = true
			end
		end
		if bNeedShowOtherWorld then
			bNeedShowOtherWorld = false
			SelectFrameOtherRoles:Show()
		end
	end
end

function RoleAction_Update()
	if GameMgr:getTickTime()/1000 - freeActionTime > 30 then
		freeActionTime = GameMgr:getTickTime()/1000;
		if SelectFrame:IsShown() or CSCreateFrame:IsShown() then
			local leftarmid = GameBackground:getAvatarAttrib( 0,POS_ID_LEFTHAND );
			local rightarmid = GameBackground:getAvatarAttrib( 0,POS_ID_RIGHTHAND );
			if leftarmid ~= 0 then
				if GetFreePosState( leftarmid ) > 0 then
					GameBackground:playEntityAnim(0, GetFreePosState( leftarmid ), nil );
				end
			elseif rightarmid ~= 0 then
				if GetFreePosState( rightarmid ) > 0 then
					GameBackground:playEntityAnim(0, GetFreePosState( rightarmid ), nil );
				end
			else
				GameBackground:playEntityAnim(0, GetFreePosState( 0 ), nil );
			end			
		end
	end
end

--选择角色
function SelectFrame_RoleBtn_OnClick()
	ClearBirthFrame();	-- 切换角色时隐藏出生地选择面板
	local nPreSelectRoleIndex	= LoginMgr:getSelectRoleIndex();
	local nCurSelectRoleIndex	= this:GetClientUserData( 0 );
	LoginMgr:selectRole( nCurSelectRoleIndex );

	if nCurSelectRoleIndex ~= nPreSelectRoleIndex then
		SelectFrame_UpdateAvatar( { ["roleIndex"] = nCurSelectRoleIndex, } );
	end

	DisCheckAllBtn( "CSChangeButton", MAX_ROLES_NUM_CREATED );
	this:Checked();
	Private_UpdateDeleteAndEnterBtn();
end

-- 双击角色名字
function SelectFrame_RoleBtn_OnDoubleClick()
	SelectFrame_RoleBtn_OnClick();
	SelectFrame_EnterGameButton_OnClick();
end

--创建角色按钮
function SelectFrame_CreateButton_OnClick()
	local isRoleNumFull = LoginMgr:isRoleNumFull();
	if isRoleNumFull then
		Login_MessageBoxOK( { ["text"] =  "创建角色失败，角色数量已满！", ["opDesc"] = "", ["lock"] = true } );
		return;
	end

	math.randomseed(os.time(x));

	-- 显示角色选择界面
	CSCreateFrame:Hide();
	SelectFrame:Hide();
	nShowSelectFrameFlag = 0;

	nShowNewRoleStartTime = GameMgr:getTickTime()/1000;
	nShowSelectNewFlag = 1;

	ClearBirthFrame();
	DisCheckRaceTex();
	InitNudeAvatar();
	GameBackground:setViewFlag( VIEW_POINT_CREATE );
	GameBackground:playCameraAnim(99113, 0);
	GameBackground:playCameraAnim(99104, 1000);
	--SetAvatarAlpha(1.0, 0, -1);
	--ShowRoleEntity();

	CSSelectNextStepButton:Hide();
	CSSelectPerStepButton:SetPoint( "Bottom", "CSSelectRoleBackGroundFrame", "Bottom", 0, -24 );
	needTurnToCreate = false;
	PrebuildMobileCheckFrame:Hide();
end

function ClearBirthFrame()
	if BirthPlaceFrame:IsShown() then
		BirthPlaceCanceBtn_OnClick();
	end
end

--删除角色按钮
function SelectFrame_DeleteButton_OnClick()
	ClearBirthFrame();
    local nTotalRoleNum = LoginMgr:getNumRole();
	if (nTotalRoleNum == 0) then
	  Login_MessageBoxOK( { ["text"] =  "目前没有角色可删除!", ["opDesc"] = "", ["lock"] = true } );
	  return;
	end

	local nSelectRoleIndex = LoginMgr:getSelectRoleIndex();
	if nSelectRoleIndex < 0 or nSelectRoleIndex >= nTotalRoleNum then
		Login_MessageBoxOK( { ["text"] =  "请选择要删除的角色！", ["opDesc"] = "", ["lock"] = true } );
	else
		Login_MessageBox( { ["text"] = "你确定要删除该角色？", ["opDesc"] = "确定删除角色", ["lock"] = true } );
	end
end

--进入游戏
function SelectFrame_EnterGameButton_OnClick()
    local nTotalRoleNum = LoginMgr:getNumRole();
	if (nTotalRoleNum == 0) then
		Login_MessageBoxOK( { ["text"] =  "目前没有角色,请创建角色后再登录！", ["opDesc"] = "" } );
		return;
	end

	local nSelectRoleIndex = LoginMgr:getSelectRoleIndex();
	if nSelectRoleIndex >= 4 then
		Login_MessageBoxOK( { ["text"] =  "不能使用合服隐藏的角色登录！", ["opDesc"] = "" } );
		return;
	end
	if nSelectRoleIndex < 0 or nSelectRoleIndex >= nTotalRoleNum then
		Login_MessageBoxOK( { ["text"] =  "请选择要登录的角色！", ["opDesc"] = "" } );
	elseif BirthPlaceFrame:IsShown() then
		LoginMgr:requestSelectBirthLocation( nSelectBirthPlaceId );
		BirthPlaceFrame:Hide();
	else
		LoginMgr:requestEnterWorld();
	end
end

function SelectFrame_EnterGameButton_OnLoad()
	this:RegisterEvent("GE_ROLE_LOGIN");
end

function SelectFrame_EnterGameButton_OnEvent()
	Login_MessageBoxOK( { ["text"] =  "服务器繁忙，请稍后登录！", ["opDesc"] = "" } );
end

--选择服务器
function SelectFrame_SelectServerButton_OnClick()
	ClearBirthFrame();
	LoginMgr:breakNetConnect();
	CharacterSelectFrame:Hide();
	SelectFrame:Hide();
	nShowSelectFrameFlag = 0;	
	DirSimpleServerFrame:Show();
	PrebuildMobileCheckFrame:Hide();
	DirFrame_OnClickRefurbishBtn()
	GameBackground:releaseEntity(0); --隐藏模型
	initSelectFrameData();
	GameBackground:playCameraAnim(99100, 1000);
	GameBackground:setViewFlag( VIEW_POINT_LOGIN );
end

--返回
function ReturnToLoginFrameByEsc()
	if CharacterSelectFrame:IsShown() then
		LoginMgr:breakNetConnect();
		CharacterSelectFrame:Hide();
		CSSelectRoleBackGroundFrame:Hide();
		CSSelectRoleFrame:Hide();
		SelectFrame:Hide();
		nShowSelectNewFlag = 0;

		LoginFrame:Show();
		LoginBackFrame:Show();
		BirthPlaceFrame:Hide();
		PrebuildMobileCheckFrame:Hide();
		GameBackground:playCameraAnim(99100, 1000);
		GameBackground:setViewFlag( VIEW_POINT_LOGIN );
		needTurnToCreate = true;
	end
end

function SelectFrame_ReturnButton_OnClick()
	if CharacterSelectFrame:IsShown() then
		LoginMgr:breakNetConnect();
		CharacterSelectFrame:Hide();
		CSSelectRoleBackGroundFrame:Hide();
		CSSelectRoleFrame:Hide();
		SelectFrame:Hide();
		nShowSelectNewFlag = 0;

		LoginFrame:Show();
		LoginBackFrame:Show();
		BirthPlaceFrame:Hide();
		PrebuildMobileCheckFrame:Hide();
		GameBackground:playCameraAnim(99100, 1000);
		GameBackground:setViewFlag( VIEW_POINT_LOGIN );
		SetFocusFrame("LoginNameEdit");
	end
end

--显示各职业角色模型
function ShowRoleEntity()
	for i = 0, 9 do
		GameBackground:fadeEntity(i+1, 1.0, 500);
	end
end

--左转
function SelectFrame_TurnLeftButton_OnMouseDown()
	local nSelectRole = LoginMgr:getSelectRoleIndex();
	if nSelectRole < 0 then
		return;
	end
	ClearBirthFrame();
	ModelAngle = ModelAngle - 5;
	GameBackground:setEntityRotate(0, ModelAngle);
end

--右转
function SelectFrame_TurnRightButton_OnMouseDown()
	local nSelectRole = LoginMgr:getSelectRoleIndex();
	if nSelectRole < 0 then
		return;
	end
	ClearBirthFrame();
	ModelAngle = ModelAngle + 5;
	GameBackground:setEntityRotate(0, ModelAngle);
end

function Private_TurnToRolePage( data )
	local nDestPage				= data["page"];
	local t_SelectFrameControl	= GetSelectFrameControl();
	t_SelectFrameControl:setCurPage( { ["page"] = nDestPage } );
	Private_UpdatePageNum();

	Private_UpdateSelectFrameNameList();
	Privaete_UpdateCreateButtonAndTurnPageBtn();
end

-- 翻到第1页
function SelectFrame_TurnFirstPageBtn_OnClick()
	local t_SelectFrameControl	= GetSelectFrameControl();
	local nCurPage				= t_SelectFrameControl:getCurPage();
	if nCurPage == 1 then
		return;
	end
	
	Private_TurnToRolePage( { ["page"] = 1 } );
end

-- 向左翻页
function SelectFrame_TurnPrePageBtn_OnClick()
	local t_SelectFrameControl	= GetSelectFrameControl();
	local nCurPage				= t_SelectFrameControl:getCurPage();
	if nCurPage == 1 then
		return;
	end
	
	nCurPage = nCurPage - 1;
	Private_TurnToRolePage( { ["page"] = nCurPage } );
end

-- 向右翻页
function SelectFrame_TurnNextPageBtn_OnClick()
	local t_SelectFrameControl	= GetSelectFrameControl();
	local nCurPage				= t_SelectFrameControl:getCurPage();
	local nMaxPage				= math.ceil( LoginMgr:getNumRole() / MAX_ROLES_NUM_CREATED );
	nMaxPage					= math.max( nMaxPage, 1 );
	if nCurPage >= nMaxPage then
		return;
	end

	nCurPage = nCurPage + 1;
	Private_TurnToRolePage( { ["page"] = nCurPage } );
end

-- 翻到最后一页
function SelectFrame_TurnLastPageBtn_OnClick()
	local t_SelectFrameControl	= GetSelectFrameControl();
	local nMaxPage				= math.ceil( LoginMgr:getNumRole() / MAX_ROLES_NUM_CREATED );
	nMaxPage					= math.max( nMaxPage, 1 );
	local nCurPage				= t_SelectFrameControl:getCurPage();
	if nCurPage >= nMaxPage then
		return;
	end

	Private_TurnToRolePage( { ["page"] = nMaxPage } );
end

--获取角色列表
function onGetRoleList()
	if LoginMgr:isBackFromInWorld() == false then
		GameBackground:playCameraAnim(99101, 0);
		GameBackground:playCameraAnim(99102, 1000);
		GameBackground:setViewFlag( VIEW_POINT_SELECT );
	end
end

-- <!--角色选择背景-->--------------------------------------------------------------------------------------------------------------------------------

-- 职业描述面板显示
function CSSelectRaceDescFrame_OnLoad()
	local backBtn = getglobal( "CSSelectRaceDescFrameBackGround" );
	local t_relFrames = { "CSSelectRaceDescFrameTopChiLunBtn", "CSSelectRaceDescFrameBottomChiLunBtn", "CSSelectRaceDescFrameRich" };
	for i = 1, MAX_ATTR_BTN do
		table.insert( t_relFrames, "RaceAttrButton"..i );
	end

	for _, name in ipairs( t_relFrames ) do
		local btn = getglobal( name );
		backBtn:AddRelFrame( btn );
	end

	this:setUpdateTime( 0 );
end

function CSSelectRaceDescFrame_OnShow()
	local t_SelectFrameMoveFrameAnimControl = GetSelectFrameMoveFrameAnimControl();
	t_SelectFrameMoveFrameAnimControl:begin( { ["curAnimframe"] = this:GetName() } );
end

function CSSelectRaceDescFrame_OnUpdate()
	local t_SelectFrameMoveFrameAnimControl = GetSelectFrameMoveFrameAnimControl();
t_SelectFrameMoveFrameAnimControl:update();
end

function CSSelectRaceDescFrame_OnHide()
	local t_SelectFrameMoveFrameAnimControl = GetSelectFrameMoveFrameAnimControl();
	if t_SelectFrameMoveFrameAnimControl:isPlayingAnim() then
		t_SelectFrameMoveFrameAnimControl:init( { ["curAnimframe"] = this:GetName() } );
	end
end

-- 下一步
function CSSelectNextStepButtonOnClick()
	if CSSelectRoleFrame:IsShown() then
		ShowSelectRole();
		return;
	end
	
	SelectFrame:Hide();						-- 隐藏登录角色选择界面
	ShowNewAvatar( nSelectRace, nSelectGender );
	nShowSelectFrameFlag = 0;
	CSSelectRoleFrame:Hide();				-- 隐藏新角色选择界面
	CSSelectRaceDescFrame:Hide();			-- 显示角色描述界面
	nShowSelectNewFlag = 0;
	CSSelectRoleBackGroundFrame:Hide();		-- 隐藏角色设置背景界面
	--temp_CSRandomButton();
	selectHairstyle = 1;	--发型
	selectHairColor = 1;	--发色
	selectFace      = 1;	--脸型
	selectHead      = 1;	--头像
	nSelectClothes	= 2;

	CSCreateFaceEditBackBtnFont:SetText("脸型"..selectFace);
	CSCreateHairStyleEditBackBtnFont:SetText("发型"..selectHairstyle);
	CSCreateHairColorEditBackBtnFont:SetText("发色"..selectHairColor);
	--CSCreateFrameClothesStyleBackBtnFont:SetText("着装"..nSelectClothes);
	selectHeadPhoto();						-- 显示角色头像

	-- 角色创建
	nCreatNewRoleStartTime = GameMgr:getTickTime()/1000;	-- 显示角色设置界面
	bCreatNewRoleFrame = true;

	CSCreateName:enableIME(true);
	SetFocusFrame("CSCreateName");
	
	GameBackground:stopEntityAnim(SelectAvatarIndex + 1);
	GameBackground:playEntityAnim(SelectAvatarIndex + 1,10100,nil);
	GameBackground:ChangeAnimModelConfig( nSelectGender, nSelectRace );
	--print( "CSSelectNextStepButtonOnClick--1103" );
end

function ShowSelectRole()
	if nSelectRace == 0 or nSelectGender == 0 then
		math.random(os.time(x));
		nSelectRace = math.random(MAX_SELECT_RACE);
		nSelectGender = math.random(CLIENT_GENDER_MAX);
	end
	ShowAvatar11( nSelectRace, nSelectGender );
	--CharacterSelectFrameBkgTex:Show();
	CSCreateFrame:Hide();				-- 显示角色设置界面
	SelectFrame:Hide();					-- 隐藏登录角色选择界
	nShowSelectFrameFlag = 0;
	CSSelectRoleFrame:Hide();			-- 隐藏新角色选择界面
	CSSelectRaceDescFrame:Show();		-- 显示角色描述界面
	nShowSelectNewFlag = 0;
	CSSelectRoleBackGroundFrame:Show();	-- 隐藏角色设置背景界面
	
	-- 显示确定按钮
	CSSelectNextStepButton:Show();
	CSSelectNextStepButton:SetPoint( "Bottom", "CSSelectRoleBackGroundFrame", "Bottom", -90, -24 );
	
	-- 显示返回按钮
	CSSelectPerStepButton:SetPoint( "Bottom", "CSSelectRoleBackGroundFrame", "Bottom", 90, -24 );
end

-- 上一步 (新角色选择)
function CSSelectPerStepButtonOnClick()
	if CSSelectRoleFrame:IsShown() then
		nShowStartTime = GameMgr:getTickTime()/1000 - 2;		-- 隐藏登录角色选择界面
		nShowSelectFrameFlag = 2;				-- 隐藏登录角色选择界面

		CSSelectRoleFrame:Hide();				-- 隐藏新角色选择界面
		CSSelectRoleBackGroundFrame:Hide();		-- 隐藏角色设置背景界面
		nShowSelectNewFlag = 0;
		CSSelectRaceDescFrame:Hide();
		SetAvatarAlpha(0, 0, -1);
		GameBackground:setViewFlag( VIEW_POINT_SELECT );
		GameBackground:playCameraAnim(99114, 0);
		GameBackground:playCameraAnim(99102, 1000);
	else
		--ShowRoleEntity();
		CSSelectRoleFrame:Show();
		CSSelectRoleBackGroundFrame:Show();
		CSSelectRaceDescFrame:Hide();
		SelectAvatarTick = -1;
		SetAvatarAlpha(1.0,200,-1);
		GameBackground:playCameraAnim(99104, 1000); -- 播放返回新角色选择界面
	end
	--CharacterSelectFrameBkgTex:Hide();
	nSelectRace = 0;
	nSelectGender = 0;
	CSSelectNextStepButton:Hide();
	CSSelectPerStepButton:SetPoint( "Bottom", "CSSelectRoleBackGroundFrame", "Bottom", 0, -24 );
end

function CSSelectRoleFrame_OnUpdate()
	SetModelMaskWinPos();
end

--男战士
function CSWarriorMaleButton_OnClick()
	DisCheckRaceTex();
	CSWarriorRaceTexButton:Checked();
	this:Checked();
	nSelectRace = CLIENT_CAREER_WARRIOR;
	nSelectGender = CLIENT_GENDER_MALE;
	ShowSelectRole();
end

--女战士
function CSWarriorFeMaleButton_OnClick()
	DisCheckRaceTex();
	CSWarriorRaceTexButton:Checked();
	this:Checked();
	nSelectRace = CLIENT_CAREER_WARRIOR;
	nSelectGender = CLIENT_GENDER_FEMALE;
	ShowSelectRole();	   
end

--男牧师
function CSChurchMaleButton_OnClick()
	DisCheckRaceTex();
	CSChurchRaceTexButton:Checked();
	this:Checked();
	nSelectRace = CLIENT_CAREER_CHURCH;
	nSelectGender = CLIENT_GENDER_MALE;
	ShowSelectRole();		
end

--女牧师
function CSChurchFeMaleButton_OnClick()
	DisCheckRaceTex();
	CSChurchRaceTexButton:Checked();
	this:Checked();
	nSelectRace = CLIENT_CAREER_CHURCH;
	nSelectGender = CLIENT_GENDER_FEMALE;
	ShowSelectRole();	 
end

--男枪手
function CSGunnerMaleButton_OnClick()
	DisCheckRaceTex();
	CSGunnerRaceTexButton:Checked();
	this:Checked();
	nSelectRace = CLIENT_CAREER_GUNER;
	nSelectGender = CLIENT_GENDER_MALE;
	ShowSelectRole();
end

--女枪手
function CSGunnerFeMaleButton_OnClick()
	DisCheckRaceTex();
	CSGunnerRaceTexButton:Checked();
	this:Checked();
	nSelectRace = CLIENT_CAREER_GUNER;
	nSelectGender = CLIENT_GENDER_FEMALE;
	ShowSelectRole();
end

--男法师
function CSMagicianMaleButton_OnClick()
	DisCheckRaceTex();
	CSMagicianRaceTexButton:Checked();
	this:Checked();
	nSelectRace = CLIENT_CAREER_MAGICOR;
	nSelectGender = CLIENT_GENDER_MALE;
	ShowSelectRole();
end

--女法师
function CSMagicianFeMaleButton_OnClick()
	DisCheckRaceTex();
	CSMagicianRaceTexButton:Checked();
	this:Checked();
	nSelectRace = CLIENT_CAREER_MAGICOR;
	nSelectGender = CLIENT_GENDER_FEMALE;
	ShowSelectRole();
end

function CSSelectButton_OnEnter()
	GameBackground:setModelBorder(-1);
	local szButtonName = this:GetName();
	local ar,ag = 0,0;
	if szButtonName == "CSWarriorMaleButton_Select" then
		ar = AvatarRace[CLIENT_CAREER_WARRIOR];
		ag = AvatarGender[CLIENT_GENDER_MALE];
	elseif szButtonName == "CSWarriorFeMaleButton_Select" then
		ar = AvatarRace[CLIENT_CAREER_WARRIOR];
		ag = AvatarGender[CLIENT_GENDER_FEMALE];
	elseif szButtonName == "CSChurchMaleButton_Select" then
		ar = AvatarRace[CLIENT_CAREER_CHURCH];
		ag = AvatarGender[CLIENT_GENDER_MALE];
	elseif szButtonName == "CSChurchFeMaleButton_Select" then
		ar = AvatarRace[CLIENT_CAREER_CHURCH];
		ag = AvatarGender[CLIENT_GENDER_FEMALE];
	elseif szButtonName == "CSGunnerMaleButton_Select" then
		ar = AvatarRace[CLIENT_CAREER_GUNER];
		ag = AvatarGender[CLIENT_GENDER_MALE];
	elseif szButtonName == "CSGunnerFeMaleButton_Select" then
		ar = AvatarRace[CLIENT_CAREER_GUNER];
		ag = AvatarGender[CLIENT_GENDER_FEMALE];
	elseif szButtonName == "CSMagicianMaleButton_Select" then 
		ar = AvatarRace[CLIENT_CAREER_MAGICOR];
		ag = AvatarGender[CLIENT_GENDER_MALE];
	elseif szButtonName == "CSMagicianFeMaleButton_Select" then
		ar = AvatarRace[CLIENT_CAREER_MAGICOR];
		ag = AvatarGender[CLIENT_GENDER_FEMALE];
	end
	i = IndexFromRaceGender(ar,ag);
	GameBackground:setModelBorder(i+1);
end

function CSSelectButton_OnLeave()
	GameBackground:setModelBorder(-1);
end

-- <!--职业描述-->--------------------------------------------------------------------------------------------------------------------------------
t_racedesc =
	{
		{ "战士", 
		
		"    他们是天生的勇者，凭借勇气与不屈的意志支撑到战斗的最后一刻。作为传统战斗技巧的继承者，战士认真的对待着每一场战斗。他们勇敢无惧，将信念凝聚成利剑。他们无坚不摧，用身躯炼铸成铁壁。战士走过的路没有退缩的脚印，因为身后是必须要去保护的战友。", 
		
		"武器类型：刀、对剑、盾",
		
		"职业风格：近战物理攻击、拉怪抗击能力\n\n#cFFFFBE角色达到59级，可转职职业：\n<征服者>利用牵制与瞬间爆发技能的组合，一念间扭转战局。\n<守护者>冲锋在敌阵中，运用娴熟的范围攻击技巧与敌人周旋厮杀。#n" 
		}, -- 战士


		{ "法师", 
		
		"    他们是恐怖的破坏者，催动着元素魔法毁灭一切。作为打破常理的战斗逻辑，法师改变了人类陈旧的认知。他们是元素的掌控者，顷刻间凝聚毁灭。他们是聪慧的策划者，弹指间扭转局势。他们用智慧征服一切，用魔法摧毁一切，几乎没人能逃脱他们的吟唱。", 
		
		"武器类型：魔杖、法器",
		
		"职业风格：远程魔法伤害、群体控制能力\n\n#cFFFFBE角色达到59级，可转职职业：\n<炼魂师> 拥有着瞬间爆发的强大破坏魔法，让敌人在一瞬间陷入死亡边缘。\n<聚能者> 催动着具备减速效果的大范围魔法，他们能轻易的给予敌人有力打击。#n" 
		}, -- 法师


		{ "枪手", 
		
		"    他们是冷酷的狙杀者，每一次扣动板机都是精密的部署。作为重要的牵制者，枪手兼备冷静的思考与果断的判断，把敌人压制在远处。他们是天生的狙击手，弹无虚发。他们是无情的杀戮手，狂热不羁。他们把敌人玩弄于股掌之中，不留一丝喘息，然后逐个击破。", 
		
		"武器类型：手枪、机枪",
		
		"职业风格：远程物理伤害、蓄气技能\n\n#cFFFFBE角色达到59级，可转职职业：\n<猎杀者>枪法与地雷完美的演绎，利用着各种地雷牵制着敌人行动逐个击破。\n<毁灭者>名副其实的炮台，熟练的利用机枪与肩炮对敌人发起猛烈的轰炸。#n" 
		}, -- 枪手


		{ "祭司", 
		
		"    他们是神的传颂者，贯彻崇高的信仰在战场上保护着他们的同伴。作为队伍的心脏，祭司只有一个念头，坚守至生命的最后一秒，阻止任何牺牲。他们如晨曦的黎明，为世界带来曙光。他们如雷电的审判，为罪人带来噩耗。相信祭司，他们能走多远，同伴就能走多远。", 
		
		"武器类型：圣杖、圣器",
		
		"职业风格：治疗能力、辅助能力、远程魔法伤害\n\n#cFFFFBE角色达到59级，可转职职业：\n<圣佑者>拥有最强大的治疗与祝福辅助技能，保护着前线的队友们。\n<驭雷师>驾驭雷电攻守兼备，作为神的制裁者，以诅咒折磨着世间的罪恶。#n" 
		}  -- 祭司
	};

-- <!--创建角色框架-->--------------------------------------------------------------------------------------------------------------------------------
function CSGenderAcceptButton_OnLoad()
	this:RegisterEvent("GE_UPDATE_ROLENEW");
end

-- 创建角色
function CSGenderAcceptButton_OnEvent()
	if arg1 ~= "GE_UPDATE_ROLENEW" then
		return;
	end

	local nRoleNewResult = LoginMgr:getRoleNewResult();
	if nRoleNewResult == g_nNEW_SUCC then
		CSCreateFrame:Hide();
		bCreatNewRoleFrame = false;
		local frame = getglobal( "SelectFrame" );
		if not frame:IsShown() then
			frame:Show();
		else
			Privaete_UpdateCharSelectFrame();
		end
		
		GameBackground:playCameraAnim(99102, 1000);
	elseif nRoleNewResult == g_nNEW_SVRBUSY then
		   Login_MessageBoxOK( { ["text"] =  "无法创建角色，请选择其他服务器", ["opDesc"] = "", ["lock"] = true } );
	elseif nRoleNewResult == g_nNEW_ROLENUM then
		    Login_MessageBoxOK( { ["text"] =  "创建角色失败，角色数量已满！", ["opDesc"] = "", ["lock"] = true } );
	elseif nRoleNewResult == g_nNEW_DUP then
		   Login_MessageBoxOK( { ["text"] =  "创建角色失败，该角色名已被使用！", ["opDesc"] = "", ["lock"] = true } );
	elseif nRoleNewResult == g_nNEW_NAME_CHECK then
		   Login_MessageBoxOK( { ["text"] =  "创建角色失败，角色名不合法！", ["opDesc"] = "", ["lock"] = true } );
	elseif nRoleNewResult == g_nNEW_ERR then
		   Login_MessageBoxOK( { ["text"] =  "创建角色失败！", ["opDesc"] = "", ["lock"] = true } );
	elseif nRoleNewResult == g_nNEW_NOCREATE then
		   Login_MessageBoxOK( { ["text"] =  "本服目前禁止创建角色，请到推荐服务器体验游戏！", ["opDesc"] = "", ["lock"] = true } );
	end 
	bCreating = false;
end

--选择取消
function CSGenderCancelButtonOnClick()
	CSCreateFrame:Hide();
	CSCreateName:Clear(); -- 清空角色名输入框
	SelectFrame:Hide();
	CSCreateFrame:Hide();

	--CSSelectRoleFrame:Show();
	--CSSelectRoleBackGroundFrame:Show();
	bCreatNewRoleFrame = false;

	nShowNewRoleStartTime = GameMgr:getTickTime()/1000;
	nShowSelectNewFlag = 1;

	CSSelectRaceDescFrame:Hide();
	GameBackground:setViewFlag( VIEW_POINT_CREATE );
	GameBackground:playCameraAnim(99113, 0);
	GameBackground:playCameraAnim(99104, 1000); -- 播放返回新角色选择界面
	--ShowRoleEntity();

	CSSelectNextStepButton:Hide();
	CSSelectPerStepButton:SetPoint( "Bottom", "CSSelectRoleBackGroundFrame", "Bottom", 0, -24 );
end

--退出游戏
function CSQuitGameButtonOnClick()
	LoginMgr:breakNetConnect();
	GameMgr:QuitGame();
end

function CSCreatRoleTurnLeftButtonOnMouseDown()
	CreatModelAngle = CreatModelAngle - 5 ;
	GameBackground:setEntityRotate(0, CreatModelAngle);
end

function CSCreatRoleTurnRightButtonOnMouseDown()
	CreatModelAngle = CreatModelAngle + 5 ;
	GameBackground:setEntityRotate(0, CreatModelAngle);
end

-- 放大按钮
function CSCreatRoleFangDaButton_OnClick()
	GameBackground:beginPull( PULL_IN );
end

function CSCreatRoleFangDaButton_OnEnter()
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "拉近看角色模型", frame = this:GetParent(),
						button = this:GetName() } );
end

function CSCreatRoleFangDaButton_OnLeave()
	GameTooltip:Hide();
end

-- 缩小按钮
function CSCreatRoleSuoXiaoButton_OnClick()
	GameBackground:beginPull( PULL_OUT );
end

function CSCreatRoleSuoXiaoButton_OnEnter()
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "拉远看角色模型", frame = this:GetParent(),
						button = this:GetName() } );
end

function CSCreatRoleSuoXiaoButton_OnLeave()
	GameTooltip:Hide();
end

--创建角色确定按钮
function CSGenderAcceptButtonOnClick()
	if bCreating then return end
	ClearBirthFrame();
	local name = CSCreateName:GetText();
	local nNameLength = string.len(name);
	local isRoleNumFull = LoginMgr:isRoleNumFull();
	if name == "" or name == nil then
		Login_MessageBoxOK( { ["text"] = "角色名不能为空", ["opDesc"] = "", ["lock"] = true } );
	elseif isRoleNumFull then
		Login_MessageBoxOK( { ["text"] = "创建角色失败，角色数量已满！", ["opDesc"] = "", ["lock"] = true } );
	elseif nNameLength > 15 then
		Login_MessageBoxOK( { ["text"] = "创建角色失败，角色名字过长！", ["opDesc"] = "", ["lock"] = true } );
	elseif nSelectRace == 0 then
		Login_MessageBoxOK( { ["text"] = "请选择角色职业", ["opDesc"] = "", ["lock"] = true } );
	else
		-- TODO: 这还没加
		LoginMgr:requestCreateRole(name, nSelectGender, nSelectRace, (selectFace-1),(selectHairColor-1),( selectHairstyle-1 ), 
									( nSelectClothes - 1 ) );
		bCreating = true;
		CSSelectRaceDescFrame:Hide();
		if SelectAvatarIndex>= 0 then
			GameBackground:playEntityAnim( SelectAvatarIndex + 1,10100,nil );
			SelectAvatarIndex = -1;
		end
	end
end

-- 拉近镜头
--发型加
function temp_HairstyleAddClick()
	if selectHairstyle >= t_sex[nSelectGender]["maxHair"] then
		selectHairstyle = 1;
	else
		selectHairstyle = selectHairstyle + 1;
	end
	
	-- TODO: 修改点
	GameBackground:setAvatarAttrib(0, POS_ID_HAIR, selectHairstyle );-- (selectHairstyle-1)
	GameBackground:updateAttrib( 0 );
	CSCreateHairStyleEditBackBtnFont:SetText("发型"..selectHairstyle);
	selectHeadPhoto();

	--GameBackground:beginPull( PULL_IN );
end

--发型减
function temp_HairstyleReduceClick()
	if selectHairstyle <= 1 then
	  selectHairstyle = t_sex[nSelectGender]["maxHair"];
	else
	  selectHairstyle = selectHairstyle - 1;
	end
	CSCreateHairStyleEditBackBtnFont:SetText("发型"..selectHairstyle);

	-- TODO: 修改点
	GameBackground:setAvatarAttrib(0, POS_ID_HAIR, selectHairstyle );-- (selectHairstyle-1)
	GameBackground:updateAttrib( 0 );
	selectHeadPhoto();

	--GameBackground:beginPull( PULL_IN );
end

--发色加
function temp_HairColorAddClick()
	if selectHairColor >= MAX_HAIR_COLOR then
		selectHairColor = 1;
	else
		selectHairColor = selectHairColor + 1;
	end

	CSCreateHairColorEditBackBtnFont:SetText("发色"..selectHairColor);
	selectHeadPhoto();

	--GameBackground:beginPull( PULL_IN );
end

--发色减
function temp_HairColorReduceClick()
	if selectHairColor <= 1 then
	  selectHairColor = MAX_HAIR_COLOR;
	else
	  selectHairColor = selectHairColor - 1;
	end
	CSCreateHairColorEditBackBtnFont:SetText("发色"..selectHairColor);
	selectHeadPhoto();

	--GameBackground:beginPull( PULL_IN );
end

--脸型加
function temp_FaceAddClick()
	if selectFace >= t_sex[nSelectGender]["maxFace"] then
	  selectFace = 1;
	else
	  selectFace = selectFace + 1;
	end
	CSCreateFaceEditBackBtnFont:SetText("脸型"..selectFace);
	GameBackground:setAvatarAttrib(0, POS_ID_FACE, selectFace-1 );
	GameBackground:updateAttrib( 0 );
	selectHeadPhoto();
   --GameBackground:beginPull( PULL_IN );
end

--脸型减
function temp_FaceReduceClick()
	if selectFace <= 1 then
	   selectFace = t_sex[nSelectGender]["maxFace"];
	else
	   selectFace = selectFace - 1;
	end
	CSCreateFaceEditBackBtnFont:SetText("脸型"..selectFace);
	GameBackground:setAvatarAttrib(0, POS_ID_FACE, selectFace-1 );
	GameBackground:updateAttrib( 0 );
	selectHeadPhoto();
	--GameBackground:beginPull( PULL_IN );
end
					-- 	2049900
local t_clothes = { [CLIENT_CAREER_WARRIOR]	= { [1] = { id = 2019900 }, [2] = { id = 2019920 }, },	
					[CLIENT_CAREER_MAGICOR]	= { [1] = { id = 2049900 }, [2] = { id = 2029920 }, },
					[CLIENT_CAREER_GUNER]		= { [1] = { id = 2049900 }, [2] = { id = 2039920 }, },	
					[CLIENT_CAREER_CHURCH]		= { [1] = { id = 2049900 }, [2] = { id = 2049920 }, }, 
				};

local t_myIndexMapToDBIndex = { [1] = 1, [2] = 4 };
-- 着装减少
function CSCreateClothesReduceButton_OnClick()	
	if nSelectClothes <= 1 then
	   nSelectClothes = t_sex[nSelectGender]["maxClothes"];
	else
	   nSelectClothes = nSelectClothes - 1;
	end
	
	UpdateCSCreateRoleClothes();
	selectHeadPhoto();

	GameBackground:beginPull( PULL_OUT );
end

function UpdateCSCreateRoleClothes()
	--[[
	local tagBirthItemDef = LoginMgr:getBirthItemDef( nSelectRace, nSelectGender );
	local nDBIndex = t_myIndexMapToDBIndex[nSelectClothes];
	local eqipClothes = tagBirthItemDef.Items[nDBIndex];
	CSCreateFrameClothesStyleBackBtnFont:SetText("着装"..nSelectClothes);

	if eqipClothes.ItemID ~= 0 then
		-- POS_ID_CLOSES
		GameBackground:setAvatarAttrib(0, POS_ID_CLOSES, eqipClothes.ItemID );
		GameBackground:updateAttrib( 0 );
	end
	--]]
	local tClothes = { 
				[CLIENT_CAREER_WARRIOR] = { 3050018, 3450018, 3750018, 3850018 },
				[CLIENT_CAREER_MAGICOR] = { 3050019, 3450019, 3750019, 3850019 },
				[CLIENT_CAREER_GUNER]	= { 3050020, 3450020, 3750020, 3850020 },
				[CLIENT_CAREER_CHURCH]	= { 3050021, 3450021, 3750021, 3850021 },
				};
	GameBackground:setAvatarAttrib( 0, POS_ID_FASHION_BODY, tClothes[nSelectRace][1] );
	GameBackground:setAvatarAttrib( 0, POS_ID_FASHION_LEG, tClothes[nSelectRace][2] );
	GameBackground:setAvatarAttrib( 0, POS_ID_FASHION_SHOES, tClothes[nSelectRace][3] );
	GameBackground:setAvatarAttrib( 0, POS_ID_FASHION_HAND, tClothes[nSelectRace][4] );
	GameBackground:updateAttrib( 0 );
end

-- 着装加
function CSCreateClothesAddButton_OnClick()
	if nSelectClothes >= t_sex[nSelectGender]["maxClothes"] then
	  nSelectClothes = 1;
	else
	  nSelectClothes = nSelectClothes + 1;
	end

	--print( "CSCreateClothesAddButton_OnClick--nSelectClothes = "..nSelectClothes.."--1429" );

	CSCreateFrameClothesStyleBackBtnFont:SetText("着装"..nSelectClothes);
	UpdateCSCreateRoleClothes();
	selectHeadPhoto();

	GameBackground:beginPull( PULL_OUT );
end

--随机外观按钮
function temp_CSRandomButton()
	math.random(os.time(x));
	selectHairstyle = math.random( t_sex[nSelectGender]["maxHair"] );
	selectFace      = math.random( t_sex[nSelectGender]["maxFace"] );
	selectHead      = math.random( t_sex[nSelectGender]["maxHead"] );
	nSelectClothes	= math.random( t_sex[nSelectGender]["maxClothes"] );
	selectHairColor	= math.random( MAX_HAIR_COLOR );
	CSCreateFaceEditBackBtnFont:SetText("脸型"..selectFace);
	CSCreateHairStyleEditBackBtnFont:SetText("发型"..selectHairstyle);
	CSCreateHairColorEditBackBtnFont:SetText("发色"..selectHairColor);
	--CSCreateFrameClothesStyleBackBtnFont:SetText("着装"..nSelectClothes);
	selectHeadPhoto();
end

function selectHeadPhoto()
	SetPhoto( selectHairstyle - 1, selectFace-1, "CreateHeadHairTexture", "CreateHeadTexture", nSelectGender, selectHairColor-1,
				"CreateHeadHairShadowTex", "CreateHeadBodyTex" );

	-- TODO: 修改点
	GameBackground:setAvatarAttrib( 0, POS_ID_HAIR,		selectHairstyle ); -- (selectHairstyle-1)
	GameBackground:setAvatarAttrib( 0, POS_ID_FACE,		selectFace - 1 );
	GameBackground:setAvatarAttrib( 0, POS_ID_HEAD,		selectHairColor - 1 );
	GameBackground:updateAttrib( 0 )
	UpdateCSCreateRoleClothes();
end


function SelectFrame_OnMouseDown()
	if arg1 == "LeftButton" then
		nLastMouseClickX = GetCursorPosX();
	end
end

function SelectFrame_OnMouseMove()
	local nSelectRole = LoginMgr:getSelectRoleIndex();
	if nSelectRole < 0 then
		return;
	end
	if this:GetPushedState() then
		local nCurMouseClickX = GetCursorPosX();
		ModelAngle = ModelAngle - (nCurMouseClickX - nLastMouseClickX) ;
		GameBackground:setEntityRotate(0, ModelAngle);
		nLastMouseClickX = nCurMouseClickX;
	end
end

function ReturnToSelect()
	GameMgr:returnLogin();
	GameBackground:playCameraAnim(99100, 1000);
	GameBackground:setViewFlag( VIEW_POINT_LOGIN );
	GameBackground:show();
	LoginMgr:breakNetConnect();
	CharacterSelectFrame:Hide();
	CSSelectRaceDescFrame:Hide();
	CSSelectRoleBackGroundFrame:Hide();
	CSSelectRoleFrame:Hide();
	LoadingFrame:Hide();
	SelectFrame:Hide();
	CSCreateFrame:Hide();
	nShowSelectNewFlag = 0;
	
	LoginFrame:Show();
	LoginBackFrame:Show();
	PasswdCardFrameCloseBtn_OnClick()
	PrebuildMobileCheckFrame:Hide();
end

function SelectRoleOnKickOff()
	bCreatNewRoleFrame		= false;
	nShowSelectNewFlag		= 0;
	if CSCreateFrame:IsShown() then	
		CSCreateFrame:Hide();
	end
	CSSelectRaceDescFrame:Hide();
	CSSelectRoleFrame:Hide();
	CSSelectRoleBackGroundFrame:Hide();
end

function SelectFrameRoleBtnCancelDel_OnClick()
	local btn = getglobal( this:GetParent() );
	LoginMgr:requestUnDelRole( btn:GetClientUserData( 0 ) );
end

----------------------------------------------------------------------------------
local MAX_OTHER_ROLE_NUM = 8
function SelectFrame_OtherRolesButton_OnClick()
	if SelectFrameOtherRoles:IsShown() then
		SelectFrameOtherRoles:Hide()
	elseif GameGetAccountAllRole:IsFinish() then
		InitOtherWorldRoleData()
		if #tOtherWorldRole > 0 then
			SelectFrameOtherRoles:Show()
		else
			Login_MessageBoxOK( { ["text"] = "其他服务器中没角色", ["opDesc"] = "", ["lock"] = true } );
		end
	else
		bNeedShowOtherWorld = true
	end
end

-- 完成
function InitOtherWorldRoleData()
	tOtherWorldRole = {}
	local num = GameGetAccountAllRole:GetWorldRoleInfoNum()
	local nLastRegionId,nLastWorldId = LoginMgr:getLastSelectRegionId(),LoginMgr:getLastSelectWorldId()
	for i=1,num do
		local info = GameGetAccountAllRole:getWorldRoleInfoByIdx( i-1 )
		if info.roleInfo.Succ == 1 and info.roleInfo.Num > 0 then
			for j=1,info.roleInfo.Num do
				local role = info.roleInfo.RoleInfo[j-1]
				-- 当前服不显示
				if not ( info.roleInfo.RegionID == nLastRegionId and role.WorldID == nLastWorldId ) then
					table.insert( tOtherWorldRole, { role.RoleName, info.roleInfo.RegionID, role.WorldID } )
				end
			end
		end
	end
	table.sort( tOtherWorldRole,	function (lhs,rhs)
										if lhs[2] == rhs[2] then
											return lhs[3] < rhs[3]
										end
										return lhs[2] < rhs[2]
									end)
	SetScrollBar( "SelectFrameOtherRolesScrollBar", MAX_OTHER_ROLE_NUM, #tOtherWorldRole );
	SelectFrameOtherRolesScrollBar:SetValue( 0 )
end

local function UpdateSelectFrameOtherRoles( nStartIdx )
	local nIdx = 1
	for i=1,#tOtherWorldRole do
		if i >= nStartIdx then
			if nIdx > MAX_OTHER_ROLE_NUM then
				break
			end
			local btn = getglobal( "SelectFrameOtherRolesInfo" .. nIdx )
			local t_fonts = {	[btn:GetName() .. "World"] = { value = LoginMgr:getWorldName(tOtherWorldRole[i][2],tOtherWorldRole[i][3]) },
								[btn:GetName() .. "Name"] = { value = tOtherWorldRole[i][1] },
							}
			util.SetFonts( t_fonts )
			btn:SetClientUserData( 0, tOtherWorldRole[i][2] )
			btn:SetClientUserData( 1, tOtherWorldRole[i][3] )
			btn:Show()
			nIdx = nIdx + 1
		end
	end
	for i=nIdx, MAX_OTHER_ROLE_NUM do
		local btn = getglobal( "SelectFrameOtherRolesInfo" .. i )
		btn:Hide()
	end
end

function SelectFrameOtherRoles_OnShow()
	InitOtherWorldRoleData()
	if #tOtherWorldRole == 0 then
		this:Hide()
		return
	end
	UpdateSelectFrameOtherRoles( SelectFrameOtherRolesScrollBar:GetValue() + 1 )
end

function SelectFrameOtherRoleInfoScrollUpBtn_OnClick()
	local slider	= getglobal( "SelectFrameOtherRolesScrollBar" );
	if not slider:IsShown() then
		return;
	end
	local nMaxValue = slider:GetMaxValue();
	local nValue = slider:GetValue() - slider:GetValueStep();
	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end

function SelectFrameOtherRoleInfoScrollDownBtn_OnClick()
	local slider	 = getglobal( "SelectFrameOtherRolesScrollBar" );
	local nMaxValue	 = slider:GetMaxValue();
	local nValue = slider:GetValue() + slider:GetValueStep();
	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

function SelectFrameOtherRoleInfo_OnValueChanged()
	local slider = SelectFrameOtherRolesScrollBar:GetValue()
	UpdateSelectFrameOtherRoles( SelectFrameOtherRolesScrollBar:GetValue() + 1 )
end

local lastChgTime = 0
function SelectFrameOtherRoleInfo_OnDoubleClick()
	if GameMgr:getTickTime() - lastChgTime > 6*1000 then
		lastChgTime = GameMgr:getTickTime()
		local font = getglobal( this:GetName() .. "Name" )
		szClickName = font:GetText()
		local regionId = this:GetClientUserData(0)
		local worldId = this:GetClientUserData(1);
		
		if LoginMgr:QuickLoginServer( regionId, worldId ) == false then
			-- 登陆失败，返回到登陆界面
			ReturnToSelect()
			Login_MessageBoxOK( { ["text"] = "登录失败！", ["opDesc"] = "", ["lock"] = true } );
		end
		PrebuildMobileCheckFrame:Hide();
	else
		Login_MessageBoxOK( { ["text"] = "不能频繁切换！", ["opDesc"] = "", ["lock"] = true } );
	end
end

function checkNeedTurnToCharacterPage()
	if needTurnToCreate and LoginMgr:getNumRole() == 0 then
		SelectFrame_CreateButton_OnClick();
	end
end

local prevHairstyle 	= 1;
local prevFace 		= 1;
local prevHairColor 	= 1;
local isModified	= false;

function CSFashionFrame_OnShow()
	local mainplayer = ActorMgr:getMainPlayer();
	local nameFont = getglobal(this:GetName().."RoleNameBtn".."NameFont");
	nameFont:SetText(mainplayer:getName());

	selectHairstyle = mainplayer:getHair() + 1; --发型
	selectFace      = mainplayer:getFace() + 1; --脸型
	selectHead      = 1;	--头像
	selectHairColor = mainplayer:getHead() + 1; --发色
	nSelectGender	= mainplayer:getGender();

	prevHairstyle 	= selectHairstyle;
	prevFace 	= selectFace;
	prevHairColor 	= selectHairColor;
	CSFashionHairStyleEditBackBtnFont:SetText("发型" .. selectHairstyle);
	CSFashionHairColorEditBackBtnFont:SetText("发色" .. selectHairColor);
	CSFashionFaceEditBackBtnFont:SetText("脸型" .. selectFace);

	SetPhoto( selectHairstyle - 1, selectFace-1, "FashionHeadHairTexture", "FashionHeadTexture", nSelectGender, selectHairColor-1,
				"FashionHeadHairShadowTex", "FashionHeadBodyTex" );
	--selectFashionHeadPhoto();
end

function CSFashionFrame_OnHide()
	if not isModified then
		selectHairstyle, selectFace, selectHairColor = prevHairstyle, prevFace, prevHairColor;
		selectFashionHeadPhoto();
	end
	GameMgr:resetPlayAngle();
end

function CSFashionFrame_OnEsc()
	MessageBox("造型更改取消", "尚未修改造型，是否取消退出?");
	MessageBoxFrame:SetClientString( "造型更改取消" );
	return true;
end

function CSFashionFrame_OnUpdate()

end

function selectFashionHeadPhoto()
	local mainplayer = ActorMgr:getMainPlayer();
	if not mainplayer then
		return;
	end
	
	SetPhoto( selectHairstyle - 1, selectFace-1, "FashionHeadHairTexture", "FashionHeadTexture", nSelectGender, selectHairColor-1,
				"FashionHeadHairShadowTex", "FashionHeadBodyTex" );

	-- TODO: 修改点
	mainplayer:setHair(selectHairstyle - 1);
	mainplayer:setFace(selectFace - 1);
	mainplayer:setHead(selectHairColor - 1);
end

function Fashion_HairstyleReduceClick()
	if selectHairstyle <= 1 then
	  selectHairstyle = t_sex[nSelectGender]["maxHair"];
	else
	  selectHairstyle = selectHairstyle - 1;
	end
	CSFashionHairStyleEditBackBtnFont:SetText("发型"..selectHairstyle);

	selectFashionHeadPhoto();
end

function Fashion_HairstyleAddClick()
	if selectHairstyle >= t_sex[nSelectGender]["maxHair"] then
		selectHairstyle = 1;
	else
		selectHairstyle = selectHairstyle + 1;
	end
	
	CSFashionHairStyleEditBackBtnFont:SetText("发型"..selectHairstyle);
	selectFashionHeadPhoto();
end

function Fashion_HairColorReduceClick()
	if selectHairColor <= 1 then
	  selectHairColor = MAX_HAIR_COLOR;
	else
	  selectHairColor = selectHairColor - 1;
	end
	CSFashionHairColorEditBackBtnFont:SetText("发色"..selectHairColor);
	selectFashionHeadPhoto();
end

function Fashion_HairColorAddClick()
	if selectHairColor >= MAX_HAIR_COLOR then
		selectHairColor = 1;
	else
		selectHairColor = selectHairColor + 1;
	end

	CSFashionHairColorEditBackBtnFont:SetText("发色"..selectHairColor);
	selectFashionHeadPhoto();
end

function Fashion_FaceReduceClick()
	if selectFace <= 1 then
	  selectFace = t_sex[nSelectGender]["maxFace"];
	else
	  selectFace = selectFace - 1;
	end
	CSFashionFaceEditBackBtnFont:SetText("脸型"..selectFace);
	selectFashionHeadPhoto();
end

function Fashion_FaceAddClick()
	if selectFace >= t_sex[nSelectGender]["maxFace"] then
	   selectFace = 1;
	else
	   selectFace = selectFace + 1;
	end
	CSFashionFaceEditBackBtnFont:SetText("脸型"..selectFace);
	selectFashionHeadPhoto();
end

function CSFashionResetButtonOnClick()
	selectHairstyle = prevHairstyle;
	selectFace 	= prevFace;
	selectHairColor = prevHairColor;
	
	CSFashionHairStyleEditBackBtnFont:SetText("发型" .. selectHairstyle);
	CSFashionHairColorEditBackBtnFont:SetText("发色" .. selectHairColor);
	CSFashionFaceEditBackBtnFont:SetText("脸型" .. selectFace);
	
	selectFashionHeadPhoto();
end

function CSFashionCancelButtonOnClick()
	MessageBox("造型更改取消", "尚未修改造型，是否取消退出?");
	MessageBoxFrame:SetClientString( "造型更改取消" );
end

function getFashion()
	return selectHairColor - 1, selectFace - 1, selectHairstyle - 1;
end

local lhs, lsf, lhc = selectHairstyle, selectFace, selectHairColor;

function CSFashionChangeButtonOnClick()
	MessageBox("造型更改确认", "是否确认当前的修改?");
	MessageBoxFrame:SetClientString( "造型更改确认" );
	lhs, lsf, lhc = selectHairstyle, selectFace, selectHairColor;
end

function ShowFashionChangeResult(res)
	isModified = true;
	CSFashionFrame:Hide();
	if res == 1 then
		MessageBoxOK("造型更改结果", "更改造型成功");
		selectHairstyle, selectFace, selectHairColor = lhs, lsf, lhc;
	else
		MessageBoxOK("造型更改结果", "更改造型失败");
		selectHairstyle, selectFace, selectHairColor = prevHairstyle, prevFace, prevHairColor;
	end
	selectFashionHeadPhoto();
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer then
		SetPhoto( mainplayer:getHair(), mainplayer:getFace(), "hairPhoto", "Photo", mainplayer:getGender() , mainplayer:getHead(),
				"PhotoHairShadowTex", "PhotoBodyTex" );
		UpdateModelView();
	end
end

function returnToChangeFashion()
	local mainplayer = ActorMgr:getMainPlayer();
	if not mainplayer then
		return;
	end
	local mapid = mainplayer:getMapId();
	if mapid == 30200 or mapid == 4404 then
		ShowMidTips("该地图不允许变更造型");
		return;
	end

	if not CSFashionFrame:IsShown() then
		GameMgr:setFashionChangeAngle(-5000.0);
		CSFashionFrame:Show();
	end
end