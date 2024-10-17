--@Var1:[ModelAngle]	����ÿ��������ת�ĽǶȱ���
--@Var2:[nCurItemId]	��ǰ������ƷID
--@Var3:[nCurEndtime]	��ǰ������Ʒ������ʱ��
local DEFAULT_ROTATE_ANGLE = 35;
local ModelAngle = DEFAULT_ROTATE_ANGLE;
-- �������������������Ϣ��ť����
local MAX_NUM_RIDE_BTNS		= 12;
-- ǿ��������������ԭ��������
MAX_RIDE_STUFF				= 4;  
local RIDE_PROCE_ITEM_ID = 5234000;
local nUVAnimationTime		= 30;

RIDE_REFUSE_CANCEL = 0; 
RIDE_REFUSE_IGNORE = 1; 

local t_opData = 
{
	["ʹ��"] = { rideID = 0, level = 0, },
	["ѡ��"] = { rideID = 0, level = 0, },
	["����"] = { rideID = 0, level = 0  },
	["����"] = { rideID = 0, level = 0  },
	-- �������ͬ����������ͻ���
	["����������"] = { rideID = 0, level = 0  },
};

-- ����ģ��doModalģʽ�ĵ���ʽ�Ի���
local t_modalFrame =	{	modalFrames		=	{	"RideLevelUpFrame",		"DropRideConfirmFrame",	},


							disabledButtons	=	{	"RideFrame_DropRideBtn",	"RideFrame_RideLevelUpBtn",		"RideFrameRideDownBtn", 
													"RideFrameRideBtn",						
												},
						};

local t_buttonEvent =	{
							["RideLevelUpFrame"]	= {},	["DropRideConfirmFrame"]	= {},	
						};

function RideFrame_CloseBtn_OnClick()
	RideFrame:Hide();
end

function RideFrameButtonsClick()
	-- ģ��doModal,��������Ϸ��嵯���Ķ��������ʾ������£��������һ������ٽ����κβ���
	for _, szFrameName in ipairs( t_modalFrame.modalFrames ) do
		local frame = getglobal( szFrameName );
		if frame:IsShown() then
			ShowMidTips( "���ȹر����ڲ����Ķ�����壬���ܽ��иò���" );
			return;
		end
	end
	t_buttonEvent[this:GetName()]["click"]();
end

-- ������������˵Ķ���	t_inviteRidePlayers = { { id = ?, startTime = 0, name = ? } }
local t_inviteRidePlayers = {};

function InitRideFrame()
	t_inviteRidePlayers = {};
	t_opData = 
		{
			["ʹ��"] = { rideID = 0, level = 0, },
			["ѡ��"] = { rideID = 0, level = 0, },
			["����"] = { rideID = 0, level = 0  },
			["����"] = { rideID = 0, level = 0  },
			["����������"] = { rideID = 0, level = 0  },
		};
end

local t_rideFrameEvents =	
{	
	["GE_RIDE_APPLY"]	= {}, ["GE_RIDE_INVITE"] = {}, ["GE_RIDELIST_CHG"]	= {}, ["GE_ACTOR_FIGHT_IN"] = {},
};

t_rideFrameEvents["GE_RIDE_APPLY"].func = 
function ( )
	local frame = getglobal( "RideFrame" );
	frame:Show();
end

t_rideFrameEvents["GE_ACTOR_FIGHT_IN"].func = 
function ( )
	local mainplayer = ActorMgr:getMainPlayer();
	local rider		= mainplayer:getRider();
	if rider:isPre() then
		--rider:RequireBreakMainPlayerRidePreToServe();
		--rider:rideDown();
	end
end

t_rideFrameEvents["GE_RIDELIST_CHG"].func = 
function ( )
	-- ������б�仯����ˢ��ѡ���������Ϣ
	local mainplayer = ActorMgr:getMainPlayer();
	local rider	= mainplayer:getRider( );
	if t_opData["ѡ��"]["rideID"] ~= 0 then
		-- ������˵ĵȼ���Ϣ
		local rideInfo				= rider:getRideInfoById( t_opData["ѡ��"]["rideID"]  );
		t_opData["ѡ��"]["rideID"]	= rideInfo ~= nil and rideInfo.RideID or 0;
		t_opData["ѡ��"]["level"]	= rideInfo ~= nil and rideInfo.nLevel or 0;
	end

	if t_opData["ѡ��"]["rideID"] == 0 then
		-- ������ѡ��һ���µ�����
		if rider:getRidNum() > 0 then
			local RideInfo = rider:getRideGridInfo( 0 );
			t_opData["ѡ��"] = { rideID = RideInfo.RideID, level = RideInfo.nLevel };
		end
	end

	-- �鿴�Ƿ�����˱�����
	--[[
	if t_opData["����������"]["rideID"] ~= 0 then
		local rideInfo	= rider:getRideInfoById( t_opData["����������"]["rideID"]  );
		-- ��������Ƿ������ͼ��
		for i = 1, MAX_SHORCUT do
			local shortcut = GameShortCut:getShortCut( i - 1 );
			if shortcut.m_nType == SCT_RIDE and shortcut.m_Id == t_opData["����������"]["rideID"] then
				GameShortCut:delShortCut( i - 1 );
			end
		end
		 t_opData["����������"] = { rideID = 0, level = 0, };
	end
	--]]
	-- ˢ������������Ϣ
	UpdateRideFrame();
	--SynchronizeShortcutRideItem();
	
	-- ��������������ʾ�ţ����Ǳ�ѡ������������Ѿ���������ر�����������
	if t_opData["����"]["rideID"] ~= 0 then
		local rideInfo	= rider:getRideInfoById( t_opData["����"]["rideID"]  );
		if rideInfo == nil then
			RideLevelUpFrame:Hide();
		end
	end

	-- ������������Ҳ��ʾ�ţ�ͬʱˢ������������
	if RideLevelUpFrame:IsShown() then
		UpdateRideLevelUpFrame();
	end
end

function isShortcutRideInUse( data )
	local mainplayer = ActorMgr:getMainPlayer();
	local rider	= mainplayer:getRider( );
	for i = 1, rider:getRidNum() do
		local rideInfo		= rider:getRideGridInfo( i - 1 );
		if data["rideID"] == rideInfo.RideID then
			return true;
		end
	end

	return false;
end

function SynchronizeShortcutRideItem()
	for i = 1, MAX_SHORCUT do
		local shortcut = GameShortCut:getShortCut( i - 1 );
		if shortcut.m_nType == SCT_RIDE and not isShortcutRideInUse( { rideID = shortcut.m_Id } ) then
			GameShortCut:delShortCut( i - 1 );
		end
	end
end

function RideFrame_OnLoad()
	t_rideFrameEvents.__index = function ( tab, key )
								tab[key] = { func = function ( ) end };
								return tab[key];
							end
	setmetatable( t_rideFrameEvents, t_rideFrameEvents );

	for event, _ in pairs( t_rideFrameEvents ) do
		this:RegisterEvent( event );
	end

	t_buttonEvent.__index = function ( tab, key )
								tab[key] = {};
								return tab[key];
							end
	setmetatable( t_buttonEvent, t_buttonEvent );

	local t_buttonEventTable =	{
								-- ��������
								["RideFrame_DropRideBtn"]	= RideFrame_RideGiveUp_OnClick,
								-- ��������
								--["RideFrame_RideLevelUpBtn"]= RideFrame_RideLevelUpBtn_OnClick,
								-- ������
								["RideFrameRideDownBtn"]	= RideFrameRideDownBtn_OnClick,	
								-- ������
								["RideFrameRideBtn"]		= RideFrameRideBtn_OnClick,
								-- �ر��������
								--["RideFrame_CloseBtn"]		= RideFrame_CloseBtn_OnClick,
							};

	for name, event in pairs( t_buttonEventTable ) do
		t_buttonEvent[name]["click"] = event;
	end
	
	for i = 1, MAX_NUM_RIDE_BTNS do
		t_buttonEvent["RideFrameButton"..i]["click"] = RideInfoBtn_OnClick;
	end
end

function GetInviteRide( nId )
	local frame = getglobal( "InviteRideFrame" );
	local font	= getglobal( "InviteRideFrameMessageFont" );
	local actor = ActorMgr:FindActor( nId );
	if  table.getn( t_inviteRidePlayers ) == 0 then
		font:SetText( GetPlayerName(actor:getName()).."�����㹲ͬ���" );
		frame:Show();
	end

	-- �ȼ����������Ƿ������ֶ�����
	for _, data in ipairs( t_inviteRidePlayers ) do
		if data["name"] == actor:getName() then
			return;
		end
	end
	table.insert( t_inviteRidePlayers, { id = nId, name = actor:getName(), startTime = GameMgr:getTickTime() } );
end

MAX_INVITE_TIME = 15000;
-- ��Ϊÿһ֡������õļ����
function UpdateInviteRider()
	if table.getn( t_inviteRidePlayers ) == 0 then
		return;
	end
	-- ����鵽������λ�ڵ�һ��������ɾ����ͬʱҪ�ر����
	-- ���ȼ���һ��
	local mainplayer = ActorMgr:getMainPlayer()
	local rider		 = mainplayer:getRider();
	local frame		 = getglobal( "InviteRideFrame" );
	local font		 = getglobal( "InviteRideFrameMessageFont" );
	if ( GameMgr:getTickTime() - t_inviteRidePlayers[1].startTime ) >= MAX_INVITE_TIME then
		-- �������б��е������˸���������2��
		if table.getn( t_inviteRidePlayers ) > 1 then
			t_inviteRidePlayers[2].startTime = GameMgr:getTickTime();
			font:SetText( GetPlayerName(t_inviteRidePlayers[2].name).."�����㹲ͬ���" );
		else
			frame:Hide();
		end
	end

	-- �����ͳһɾ�����ֶ��еĴ���
	for index, data in ipairs( t_inviteRidePlayers ) do
		if ( GameMgr:getTickTime() - data.startTime ) >= MAX_INVITE_TIME then
			ShowMidTips( "�������"..data["name"].."�������" );
			rider:refuseInviteRide( data["id"], RIDE_REFUSE_IGNORE );
			table.remove( t_inviteRidePlayers, index );
		end
	end
end

function RejectAllRideInvite()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then
		t_inviteRidePlayers = {};
		return;
	end

	local rider		 = mainplayer:getRider();
	for index, data in ipairs( t_inviteRidePlayers ) do
		rider:refuseInviteRide( t_inviteRidePlayers[index]["id"], RIDE_REFUSE_IGNORE );
	end
	t_inviteRidePlayers = {};
	InviteRideFrame:Hide();
end

function RideFrame_OnEvent()	
	--if this:IsShown() then
	t_rideFrameEvents[arg1].func();
	--end
end

function RideFrame_OnHide()
	for _, szFrameName in ipairs( t_modalFrame.modalFrames ) do
		local frame = getglobal( szFrameName );
		frame:Hide();
	end

	local btn = getglobal( "RideFrameXinShouBtn" );
	btn:Hide();

	ModelAngle = DEFAULT_ROTATE_ANGLE;
end

function GetNewRider( nId, nLv )
	t_opData["ѡ��"] = { rideID = nId, level =nLv };
	if RideFrame:IsShown() then
		-- ˢ������б�
		UpdateRideFrame();
		return;
	end
	
	if not GameWizard:isDo(GAMEWIZARD_FIRST_BLINK_SHORTCUT_RIDE_BTN) then
		GuideEffectShow( "", "�����������", "RideShortcutButton", "����һ����ˣ�����鿴\n#P��ݼ���N��", "uires\\ZhuJieMian\\1.dds", 352, 609, 60, 60, 50, 60, -9, 5, 7, -58, true, FIRST_GET_RIDE );
		GameWizard:doStep(GAMEWIZARD_FIRST_BLINK_SHORTCUT_RIDE_BTN);
	else
		RideFrame:Show();
	end
	
end

function ShowRideGuide()
	-- ��һ�λ�ȡ���
	local btn = getglobal( "RideFrameXinShouBtn");
	local animationTexture	= getglobal( "RideFrameXinShouBtnUVAnimationTex");
	-- and Quest:isQuestFini( 3132 ) 
	-- �ر�ʹ�ñ������ߺ�
	if not GameWizard:isDo( GAMEWIZARD_FIRST_GETRIDE_ITEM ) then
		local effectFrame = getglobal( "PlayerContainerFrameEffectFrame" );
		effectFrame:Hide();
		GameWizard:doStep( GAMEWIZARD_FIRST_GETRIDE_ITEM );
	end

	if not GameWizard:isDo( GAMEWIZARD_FIRST_HAVERIDE ) then
		ShowRideXinShouBtn( { text = "���������������", btn = "RideFrameRideBtn" } );
	end
	
	-- ��һ��ǿ�����
	if GameWizard:isDo( GAMEWIZARD_FIRST_HAVERIDE ) and not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_STRENGTHEN_BTN ) and 
		Quest:isQuestRun( 3134 ) then
		--GameWizard:doStep( GAMEWIZARD_FIRST_BLINK_STRENGTHEN_BTN );
		ShowRideXinShouBtn( { text = "�������ǿ������", btn = "RideFrame_RideLevelUpBtn" } );
	end
end

function ShowRideXinShouBtn( data )
	local btn				= getglobal( "RideFrameXinShouBtn");
	local rich				= getglobal( "RideFrameXinShouBtnRich");
	local animationTexture	= getglobal( "RideFrameXinShouBtnUVAnimationTex");
	
	btn:SetPoint( "topleft", data["btn"], "topleft", -9, -5 );

	local nSelfDefineExtendWidth = 10;
	rich:SetClientString( data["text"] );
	ReSizeTooltip( rich:GetName(), btn:GetName(), nSelfDefineExtendWidth );
	
	animationTexture:SetUVAnimation( 50, true );
	btn:SetClientString( data["btn"] ) ;
	btn:Show();
end

--@Desc:���������ʾʱ����
--@Return:Nothing
function RideFrame_OnShow()	
	-- ���ô�������
	RideFrame:SetPoint("topleft", "$parent", "topleft", 32, 163 );
	-- �ж�װ����ǩ����Ƿ���ʾ
	-- ����ָ��
	local mainplayer = ActorMgr:getMainPlayer()
	local rider		 = mainplayer:getRider();
	ShowRideGuide();
	
	-- ��ʼ��Ĭ�ϱ�ѡ�������itemͼ��
	-- 
	if rider:getRidNum() == 0 then
		t_opData["ѡ��"] = { rideID = 0, level = 0 };
	elseif t_opData["ѡ��"]["rideID"] == 0 then
		-- ���ϴ�ûѡ���κ�һֻ��ˣ���Ĭ��ѡ��һ��
		local RideInfo = rider:getRideGridInfo( 0 );
		t_opData["ѡ��"] = { rideID = RideInfo.RideID, level =  RideInfo.nLevel };
	end
	
	-- ˢ������б�
	UpdateRideFrame();
end

function ClearRideListWhenOffline()
	for i = 1, MAX_NUM_RIDE_BTNS do
		local btn	= getglobal( "RideFrameButton"..i );
		local tex	= getglobal( "RideFrameButton"..i.."BackGround_UV_Texture" );
		--local RideInfo_Btn_Tex	= getglobal( btn:GetName().."Tex" );
		-- ������λ������û���κ�ҩ��ʾ����ˣ�����Ϊ����ʾ״̬
		tex:Hide();
		--RideInfo_Btn_Tex:Hide();
		btn:DisChecked();
		btn:SetNormalTexture( "uires\\ui\\mask\\TouMingTongDao.tga", "blend" );
		btn:SetPushedTexture( "uires\\ZhuJieMian\\1.dds", "blend" );
		btn:SetHightlightTexture( "uires\\ZhuJieMian\\1.dds", "blend" );
		btn:ChangeHighlightTexture( 794,697,44,44 );
		btn:ChangePushedTexture( 794,607,44,44 );
		btn:SetClientUserData( 0 , 0 );
		--btn:Disable();
	end

	-- ������״̬
	for i = 1, MAX_NUM_RIDE_BTNS do
		--local tex	= getglobal( "RideFrameButton"..i.."Tex" );
		--tex:SetTexture( "uires\\ZhuJieMian\\1.dds" );
		--tex:Show();
		--tex:SetTexUV(749, 427, 44,44 ); 
		local btn	= getglobal( "RideFrameButton"..i );
		btn:SetNormalTexture( "uires\\ZhuJieMian\\3.dds", "blend" );
		btn:SetPushedTexture( "uires\\ZhuJieMian\\3.dds", "blend" );
		btn:SetHightlightTexture( "uires\\ZhuJieMian\\3.dds", "blend" );
		btn:ChangeNormalTexture( 306,772,44,44 );
		btn:ChangeHighlightTexture( 351,817,44,44 );
		btn:ChangePushedTexture( 306,817,44,44 );
		btn:SetClientUserData( 0 , 1 );
	end

	local t_disableBtns = { "RideFrame_TurnLeftBtn", "RideFrame_TurnRightBtn" };

	for _, name in ipairs( t_disableBtns ) do
		local btn	= getglobal( name );
		btn:Disable();
	end

	local t_hideFrames = { "RideFrame_ModelView" };
	for _, name in ipairs( t_hideFrames ) do
		local frame	= getglobal( name );
		frame:Hide();
	end

	local t_fonts = { "RideFrameRideNameText" };
	for _, name in ipairs( t_fonts ) do
		local font	= getglobal( name );
		font:SetText( "" );
	end
end

-- ������б��ÿ�
function ClearRideList()
	local mainplayer = ActorMgr:getMainPlayer()
	local rider	= mainplayer:getRider();
	if rider:getRidNum() == 0 then
		RideFrame:Hide();
		return;
	end

	for i = 1, MAX_NUM_RIDE_BTNS do
		local btn	= getglobal( "RideFrameButton"..i );
		local tex	= getglobal( "RideFrameButton"..i.."BackGround_UV_Texture" );
		local RideInfo_Btn_Tex	= getglobal( btn:GetName().."Tex" );
		local validTime = getglobal( btn:GetName().."ValidTimeTex" )
		-- ������λ������û���κ�ҩ��ʾ����ˣ�����Ϊ����ʾ״̬
		tex:Hide();
		RideInfo_Btn_Tex:Hide();
		btn:DisChecked();
		btn:SetNormalTexture( "uires\\ui\\mask\\TouMingTongDao.tga", "blend" );
		btn:SetPushedTexture( "uires\\ZhuJieMian\\1.dds", "blend" );
		btn:SetHightlightTexture( "uires\\ZhuJieMian\\1.dds", "blend" );
		btn:ChangeHighlightTexture( 794,697,44,44 );
		btn:ChangePushedTexture( 794,607,44,44 );
		btn:SetClientUserData( 0 , 0 );
		validTime:Hide();
	end

	-- ������״̬	
	for i = rider:getMaxRideNum() + 1, MAX_NUM_RIDE_BTNS do
		--local tex	= getglobal( "RideFrameButton"..i .. "Tex" );
		--tex:SetTexture( "uires\\ZhuJieMian\\1.dds" );
		--tex:SetTexUV( 749, 427, 44,44 ); 
		--tex:Show();
		local btn	= getglobal( "RideFrameButton"..i );
		btn:SetNormalTexture( "uires\\ZhuJieMian\\3.dds", "blend" );
		btn:SetPushedTexture( "uires\\ZhuJieMian\\3.dds", "blend" );
		btn:SetHightlightTexture( "uires\\ZhuJieMian\\3.dds", "blend" );
		btn:ChangeNormalTexture( 306,772,44,44 );
		btn:ChangeHighlightTexture( 351,817,44,44 );
		btn:ChangePushedTexture( 306,817,44,44 );
		btn:SetClientUserData( 0 , 1 );
	end

	local t_disableBtns = { "RideFrame_TurnLeftBtn", "RideFrame_TurnRightBtn" };

	for _, name in ipairs( t_disableBtns ) do
		local btn	= getglobal( name );
		btn:Disable();
	end

	local t_hideFrames = { "RideFrame_ModelView" };
	for _, name in ipairs( t_hideFrames ) do
		local frame	= getglobal( name );
		frame:Hide();
	end

	local t_fonts = { "RideFrameRideNameText" };
	for _, name in ipairs( t_fonts ) do
		local font	= getglobal( name );
		font:SetText( "" );
	end
end

local szBuffPath = "uires\\TuPianLei\\JiNeng\\Buff\\";
function GetRideBuffIcon( nIconID )
	local IconPath = szBuffPath..nIconID..".tga";
	return IsInExistence( IconPath ) and IconPath or DEFAULT_ITEM_PATH;
end

function GetRideItemIcon( nIconID )
	local IconPath = GetItemEquipIconPath()..nIconID..".tga";
	return IsInExistence( IconPath ) and IconPath or DEFAULT_ITEM_PATH;
end

-- ���������Ϣ��ť
function SetRideInfoBtn( RideInfo, btn )
	local RideInfo_Btn_Tex	= getglobal( btn:GetName().."Tex" );
	local validTime = getglobal( btn:GetName().."ValidTimeTex" )
	-- ������λ������û���κ�ҩ��ʾ����ˣ�����Ϊ����ʾ״̬	
	if RideInfo.RideID == 0 then
		RideInfo_Btn_Tex:Hide();
		return;
	end

	local RideInfo_BackGround_UV= getglobal( btn:GetName().."BackGround_UV_Texture" );
	local RideDef	= getRideDef( RideInfo.RideID, RideInfo.nLevel );
	--btn:Enable();
	local IconPath	= GetItemEquipIconPath()..RideDef.IconID..".tga";
	RideInfo_Btn_Tex:SetTexture( IsInExistence( IconPath ) and IconPath or DEFAULT_ITEM_PATH );
	--local redTex = getglobal( btn:GetName() .."RedTex" );
	if RideInfo:isValidEnd() then
		validTime:Show();
	else
		validTime:Hide();
	end
	RideInfo_Btn_Tex:SetTexRelUV( 1, 1 );
	RideInfo_Btn_Tex:Show();
	-- �鿴��ǰ��ʾ�������Ϣ��ť�Ƿ��Ǳ�ѡ���鿴����ˣ�������checkס��
	if t_opData["ѡ��"]["rideID"] == RideInfo.RideID then
		btn:Checked();
		RideFrameRideNameText:SetText( RideDef.RideName );
	end
	
	local mainplayer	= ActorMgr:getMainPlayer()
	local rider			= mainplayer:getRider();
	local curUsingRider	= rider:getRideItem();
	-- ����ǰ���õ������Ϣ��ť��Ϊ���ڱ������ˣ�����ʾ��Ч
	if curUsingRider.RideID == RideInfo.RideID then
		RideInfo_BackGround_UV:SetUVAnimation( nUVAnimationTime, true );
		RideInfo_BackGround_UV:Show();
	end
end 

--@Desc:���������������
--@Return:Nothing
function UpdateRideFrame()
	ClearRideList();

	local mainplayer	= ActorMgr:getMainPlayer()
	local rider			= mainplayer:getRider();
	local curUsingRider	= rider:getRideItem();
	
	-- �����Ǽ�û�д������״̬��Ҳû�д��ڷ���״̬������ʾ��˰�ť�������¼ݰ�ť
	if curUsingRider.RideID == 0 then
		RideFrameRideBtn:Show();
		RideFrameRideDownBtn:Hide();
	else
		RideFrameRideBtn:Hide();
		RideFrameRideDownBtn:Show();
	end
	
	-- �Ƿ��б�ѡ���鿴����ˣ����У�����ʾ���ģ��
	if t_opData["ѡ��"]["rideID"] ~= 0 then
		SetRideModerView( t_opData["ѡ��"]["rideID"], t_opData["ѡ��"]["level"] );
	end	
	
	for i = 1, math.min( MAX_NUM_RIDE_BTNS, rider:getRidNum() ) do
		local rideInfoBtn	= getglobal( "RideFrameButton"..i );
		local rideInfo		= rider:getRideGridInfo( i - 1 );
		SetRideInfoBtn( rideInfo, rideInfoBtn );
	end
end

--@Desc:������뼼�ܰ�ťʱִ��
--@Return:Nothing
function RideInfoBtn_OnEnter()
	local szName	= this:GetName();
	local mainplayer = ActorMgr:getMainPlayer();
	local rider		= mainplayer:getRider();
	local RideInfo	= rider:getRideGridInfo( this:GetClientID() - 1 );
	if RideInfo.RideID <= 0  then		
		if this:GetClientUserData( 0 ) == 1 then
			ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "����λ�ѱ���������ʹ��������λ��������", 
								frame = this:GetParent(), button = this:GetName(), horizonal = "right" } );
		end
		return ;
	end
	
	ShowRideTips( { id = RideInfo.RideID, lv = RideInfo.nLevel, relFrame = "RideFrame", button = this:GetName(), rideInfo = RideInfo } );
end

--@Desc:����ƿ����ܰ�ťʱִ��
--@Return:Nothing
function RideInfoBtn_OnLeave()
	GameTooltipOnLeave();
	RideTipsFrame:Hide();	
end

--@Desc:���ܰ�ť���϶�ʱִ��
--@Return:Nothing
function RideInfoBtn_Drag()
	UIMSG:getUIGV().DragFrom = DRAG_RIDE;
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then
		return;
	end
	local rider		= mainplayer:getRider();
	local RideInfo	= rider:getRideGridInfo( this:GetClientID() - 1 );

	if RideInfo.RideID == 0  then
		return ;
	end

	local IconTex = getglobal( this:GetName().."Tex" );
	UIBeginDrag( IconTex:GetTexture() , RideInfo.RideID, RideInfo.Endtime, RideInfo.nLevel );
end

function RideInfoBtn_OnReceiveDrag()
	if UIMSG:getUIGV().DragFrom ~= DRAG_RIDE_MOVE then return end	
	if UIGetDragUserData3() == 1 then
		RideMoveLevelFrameSrcRide:SetClientUserData( 0, 0 );
		RideMoveLevelFrameSrcRide:SetClientUserData( 1, 0 );
		RideMoveLevelFrameSrcRideIconTexture:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	elseif UIGetDragUserData3() == 2 then
		RideMoveLevelFrameDesRide:SetClientUserData( 0, 0 );
		RideMoveLevelFrameDesRide:SetClientUserData( 1, 0 );
		RideMoveLevelFrameDesRideIconTexture:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	end
end

--@Desc:�Ҽ����ʱ����
--@Used:��������Ԥ��ģ��
--@Return:Nothing
function RideInfoBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();

	local rider		= mainplayer:getRider();
	local RideInfo	= rider:getRideGridInfo( this:GetClientID() - 1 );
	-- ����ǰ����û�����
	if RideInfo.RideID == 0 then
		if this:GetClientUserData( 0 ) == 1 then
			MessageBoxForVip( "��ʾ", "����������λ��Ҫ����һ��������λ�ƣ�ȷ����Ҫ��������λ��" );
			MessageBoxFrameForVipAcceptButton:SetText("ȷ������");
			MessageBoxFrameForVip:SetClientUserData( 0, this:GetClientID() );
			MessageBoxFrameForVip:SetClientUserData( 1, 2 );
		end
		return;
	end

	local curUsingRide = rider:getRideItem();
	-- ����ǰѡ�е���������ڱ�ʹ�õ���ˣ�����ʾ�¼�
	if curUsingRide.RideID == RideInfo.RideID then
		RideFrameRideBtn:Hide();
		RideFrameRideDownBtn:Show();
	else -- ������ʾ�ϼ�
		RideFrameRideBtn:Show();
		RideFrameRideDownBtn:Hide();
	end

	DisCheckAllBtn( "RideFrameButton", MAX_NUM_RIDE_BTNS );
	t_opData["ѡ��"] = { rideID = RideInfo.RideID, level = RideInfo.nLevel };
	local RideDef = getRideDef( RideInfo.RideID, RideInfo.nLevel );
	RideFrameRideNameText:SetText( RideDef.RideName );
	SetRideModerView( RideInfo.RideID, RideInfo.nLevel );
	this:Checked();
	
	if RideLevelUpFrame:IsShown() then
		if t_opData["ѡ��"]["rideID"] == t_opData["����"]["rideID"] then
			return;
		end
		
		local canLevelUp = false;
		for i = 1, MAX_RIDE_STUFF do
			local stuffInfo = RideDef.LevelUpStuff[i-1];
			if stuffInfo.ItemID ~= 0 then
				canLevelUp = true;
				break;
			end
		end

		if not canLevelUp then
			ShowMidTips( "�������޷�����ǿ��" );
			RideLevelUpFrame:Hide();
			return;
		end

		t_opData["����"] = t_opData["ѡ��"];
		local curShowFrame = getglobal( "RideFrame" );
		if curShowFrame:GetRealRight() >= DEFAULT_UI_WIDTH then
			RideLevelUpFrame:SetPoint( "topright", "UIClient", "topleft", curShowFrame:GetRealLeft() - 3, curShowFrame:GetRealTop() );
		else
			RideLevelUpFrame:SetPoint( "topleft", "UIClient", "topleft", curShowFrame:GetRealLeft() + curShowFrame:GetWidth() + 3, curShowFrame:GetRealTop() );
		end

		RideLevelUpFrame:Show();
	end
end

--@Desc:������˰�ťʱ����
--@Return:Nothing
 PWORLD_NO_RIDE = 4;
 t_cannotRideStatus = { 1009, 1003, 1050, 1018, 4525 };
 function CanRide()
	-- ���ж��������ڵ�ͼ�Ƿ��ܹ����
	local nRealMapID = MapShower:getRealMapId();
	if nRealMapID >= MIN_COPY_ID then
		local pworldDef = getPworldDef( nRealMapID / 100 );
		if bitAnd( pworldDef.CtrlFlag, PWORLD_NO_RIDE ) then
			ShowMidTips( "�˵�ͼ�������" );
			return false;
		end
	end
	
	local mainplayer	= ActorMgr:getMainPlayer();
	local BufStatus		= mainplayer:getBufStatus();
	local rider			= mainplayer:getRider();
	-- ���ж������Ƿ���ĳЩ������˵�״̬
	for _, id in ipairs( t_cannotRideStatus ) do
		if BufStatus:checkBuf( id ) then
			ShowMidTips( "����"..getStatusDef( id, 1 ).Name.."״̬�����ܽ�����˲���" );
			return false;
		end
	end

	-- ��������״̬���������
	if mainplayer:isDead() then
		ShowMidTips( "��������״̬�����ܽ�����˲���" );
		return false;
	end

	-- ���ڰ�̯״̬���������
	if not rider:isOnHorse() and mainplayer:isInStall() then
		ShowMidTips( "��̯״̬���޷����" );
		return false;
	end

	-- ���ڱ���״̬���������
	if not rider:isOnHorse() and mainplayer:isInMorph() then
		ShowMidTips( "���ڱ���״̬�����ܽ�����˲���" );
		return false;
	end
	
	-- ����ս��״̬���������
	if not rider:isOnHorse() and mainplayer:isInFight() then
		ShowMidTips( "����ս��״̬�����ܽ�����˲���" );
		return false;
	end
	
	-- ���ڷ���״̬���������
	if rider:isOnHorse() and mainplayer:isInFly() then
		ShowMidTips( "���ڷ���״̬���������" );
		return false;
	end
	
	-- ���Ƕ�����˵Ŀ��˲������
	if ( rider:isOnHorse() and not rider:isOwnRide() ) then
		ShowMidTips( "���ڱ��˵������ϣ����ܽ�����˲���" );
		return false;
	end

	return true;
 end

function RideFrameRideBtn_OnClick()
	-- ��������ָ����Ч
	local xinShouBtn = getglobal( "RideFrameXinShouBtn");
	if xinShouBtn:IsShown() and xinShouBtn:GetClientString() == this:GetName() then
		GameWizard:doStep( GAMEWIZARD_FIRST_HAVERIDE );
		xinShouBtn:Hide();
	end

	--RideFrameEffectFrame:Hide();
	-- ���жϵ�ǰ�Ƿ�������״̬
	-- ��û���κα�ѡ����ˣ������κβ���
	if t_opData["ѡ��"]["rideID"] == 0 then
		return;
	end

	-- ��������������������ˣ�����ʹ��
	--[[
	if t_opData["ѡ��"]["rideID"] == t_opData["����"]["rideID"] then
		ShowMidTips( "�������������������ʹ��" );
		return;
	end
	--]]

	-- ����������ڱ�����������ʹ��
	if t_opData["ѡ��"]["rideID"] == t_opData["����"]["rideID"] then
		ShowMidTips( "��������ڱ�����������ʹ��" );
		return;
	end
	
	-- ������ʹ���У���������������
	-- �ȼ���Ƿ������ڱ���˵�����
	local mainplayer	= ActorMgr:getMainPlayer();
	local rider			= mainplayer:getRider();
	local curUsingRide	= rider:getRideItem();
	if curUsingRide.RideID ~= 0 and curUsingRide.RideID == t_opData["ѡ��"]["rideID"] then
		return;
	end

	if not CanRide() then
		return;
	end
	-- TODO: ��Ҫ�ĳɼ�ʹ������ҲҪ�Զ���ʼ��һ������
	if rider:isPre() then
		return;
	end
	
	if curUsingRide.RideID ~= 0 then
		rider:requestRideDown();
	end

	local rider = mainplayer:getRider();
	rider:MainPlayerRidePre( t_opData["ѡ��"]["rideID"], t_opData["ѡ��"]["level"] );
end

function RideFrameRideDownBtn_OnClick()
	-- TODO: �Ƿ�Ҫѡ���˲����¼�
	local mainplayer = ActorMgr:getMainPlayer();
	local rider = mainplayer:getRider();
	local curUsingRider	= rider:getRideItem();
	if curUsingRider.RideID == 0 then
		return;
	end

	rider:requestRideDown();
end

function RideFrame_RideLevelUpBtn_OnClick()
	if t_opData["ѡ��"]["rideID"] == 0 then
		return;
	end
	
	local xinShouBtn = getglobal( "RideFrameXinShouBtn");
	if xinShouBtn:IsShown() and xinShouBtn:GetClientString() == this:GetName() then
		xinShouBtn:Hide();
	end

	-- ����������ڱ�����������ʹ��
	if t_opData["ѡ��"]["rideID"] == t_opData["����"]["rideID"] then
		ShowMidTips( "������ֹ�Ը���˵Ķ������������ܽ�������" );
		return;
	end
	
	local mainplayer	= ActorMgr:getMainPlayer();
	local rider		= mainplayer:getRider();
	local curUsingRide	= rider:getRideItem();
	--[[
	if t_opData["ѡ��"]["rideID"] == curUsingRide.RideID then
		ShowMidTips( "��ֹͣ�Ը���˵�ʹ�ã����ܽ�������" );
		return;
	end
	--]]
	if t_opData["ѡ��"]["rideID"] == t_opData["����"]["rideID"] then
		return;
	end

	local nRidID		= t_opData["ѡ��"]["rideID"];
	local rideInfo		= rider:getRideInfoById( nRidID );
	local nRideLevel	= rideInfo.nLevel;
	local rideDef		= getRideDef( nRidID, nRideLevel );
	
	local canLevelUp = false;
	for i = 1, MAX_RIDE_STUFF do
		local stuffInfo = rideDef.LevelUpStuff[i-1];
		if stuffInfo.ItemID ~= 0 then
			canLevelUp = true;
			break;
		end
	end

	if not canLevelUp then
		ShowMidTips( "�������޷�����ǿ��" );
		return;
	end
	
	t_opData["����"] = t_opData["ѡ��"];
	local curShowFrame = getglobal( "RideFrame" );
	if curShowFrame:GetRealRight() >= DEFAULT_UI_WIDTH then
		RideLevelUpFrame:SetPoint( "topright", "UIClient", "topleft", curShowFrame:GetRealLeft() - 3, curShowFrame:GetRealTop() );
	else
		RideLevelUpFrame:SetPoint( "topleft", "UIClient", "topleft", curShowFrame:GetRealLeft() + curShowFrame:GetWidth() + 3, curShowFrame:GetRealTop() );
	end
	
	RideLevelUpFrame:Show();
end

function RideFrame_RideGiveUp_OnClick()
	if t_opData["ѡ��"]["rideID"] == 0 then
		return;
	end

	-- ��������������������ˣ�����ʹ��
	if t_opData["ѡ��"]["rideID"] == t_opData["����"]["rideID"] then
		ShowMidTips( "������������������ܶ���" );
		return;
	end

	t_opData["����"]	= t_opData["ѡ��"];
	local mainplayer	= ActorMgr:getMainPlayer();
	local rider			= mainplayer:getRider();
	local container = mainplayer:getContainer();
	local lockState = container:getContainerLockState();
	if lockState == CONTAINER_LOCK_STATE_LOCK then
		ShowMidTips("������֤������ȫ��");
		return;
	end
	-- �������� ��RideName
	local szText		= "ȷ��Ҫ����������";
	local nRidID		= t_opData["����"]["rideID"];
	local rideInfo		= rider:getRideInfoById( nRidID );
	if rideInfo ~= nil then 
		local RideDef = getRideDef( rideInfo.RideID, rideInfo.nLevel );
		if RideDef ~= nil then 
			szText = "ȷ��Ҫ�������#G"..RideDef.RideName.."#n����"
		end
	end
		
	local curUsingRide	= rider:getRideItem();
	local longFont		= getglobal( "DropRideConfirmFrameLongMessageFont" );
	local shortFont		= getglobal( "DropRideConfirmFrameShortMessageRich" );
	if curUsingRide.RideID == t_opData["����"]["rideID"] then
		longFont:SetText( "��Ҫ�����ĵ�ǰ��������ʹ���У�������\n������ǿ�������ȷ��Ҫ������" );
		longFont:SetTextColor( 255, 229, 189 );
		shortFont:SetText( "",255,229,189 );
	else 
		shortFont:setCenterLine(true);
		shortFont:SetText( szText,255,229,189 );
		longFont:SetText("��ע�⣺ȷ�϶������������ʧȥ�����\n    �ѽ��е�����Ҳ��ȫ����Ч��");
		longFont:SetTextColor( 202, 54,  33 );
	end

	DropRideConfirmFrame:Show();
end

function DropRideConfirmFrame_OnHide()
	t_opData["����"] = { rideID = 0, level = 0, };
end

-- ȷ����������
function DropRideConfirmFrame_ConfirmBtn_OnClick()
	local mainplayer	= ActorMgr:getMainPlayer();
	local rider			= mainplayer:getRider();
	local curUsingRide	= rider:getRideItem();
	-- ���ж�Ҫ����������Ƿ����ڱ������ţ����ǣ����¼ݣ��ٶ���
	if curUsingRide.RideID == t_opData["����"]["rideID"] then
		rider:requestRideDown();
		ShowMidTips( "��ǰ����ʹ�õ����ﱻ����������ǿ��������" );
	end
	
	rider:dropRide( t_opData["����"]["rideID"] );
	t_opData["����������"] = { rideID = t_opData["����"]["rideID"], level = t_opData["����"]["level"], };
	t_opData["ѡ��"] = { rideID = 0, level = 0, };
	UpdateRideFrame();

	DropRideConfirmFrame:Hide();
end

-- ȡ����������
function DropRideConfirmFrame_CancelBtn_OnClick()
	DropRideConfirmFrame:Hide();
end

--@Desc:���տͻ��˷��͵�֪ͨ��ʼ��������
--@Return:Nothing
function StartRide( nTime )
	IntonateBar_text:SetText("�����ٻ�����");
	IntonateBar:SetClientUserData( 0, RIDE_TYPE );
	IntonateBar:SetClientUserData( 2, nTime );
	IntonateBar:Show();
end

--@Desc:��������ģ����ʾ
--@Parm:[in] nItemId  �������ƷID
--@Return:Nothing
function SetRideModerView( nRideId, nLevel )
	local mainplayer = ActorMgr:getMainPlayer()
	
	if nRideId == 0 then
		return;
	end
	
	local rideDef = getRideDef( nRideId, nLevel );
	local t_modelViewers = { ["RideFrame_ModelView"] = { path = GetModePathById( rideDef.ModelID ), animId = 10100, 
							 scale = GetNpcDisplayInfo( rideDef.ModelID ).size == 0 and 1 or GetNpcDisplayInfo( rideDef.ModelID ).size,
							 angle = DEFAULT_ROTATE_ANGLE } };
	for name, data in pairs( t_modelViewers ) do
		local modelViewer = getglobal( name );
		if modelViewer ~= nil then
			modelViewer:setPetModel( 0, data.animId, data.path );
			modelViewer:setPetScale( 0, data.scale, data.scale, data.scale );
			--modelViewer:setEntityPosition( 0, 0, 100, 0 );
			modelViewer:setEntityPosition( 0, 0, 40, 0 );
			modelViewer:setEntityRotate( 0, data.angle );
			modelViewer:Show();
		end
	end

	local t_enableButtons = { "RideFrame_TurnLeftBtn", "RideFrame_TurnRightBtn" };
	for _, name in ipairs( t_enableButtons ) do
		local btn = getglobal( name );
		btn:Enable();
	end
end

--@Desc:������ת��ťʱģ������ת��
--@Return:Nothing
function RideFrame_TurnLeftBtn_OnMouseDown()
	local ModelViewer = getglobal("RideFrame_ModelView");
	ModelAngle = ModelAngle - 5;
	ModelViewer:setEntityRotate( 0,ModelAngle );
end

--@Desc:������ת��ťʱģ������ת��
--@Return:Nothing
function RideFrame_TurnRightBtn_OnMouseDown()
	local ModelViewer = getglobal("RideFrame_ModelView");
	ModelAngle = ModelAngle + 5;
	ModelViewer:setEntityRotate(0,ModelAngle);
end

local bRecond	= false;
local nLastTime = 0;
function RideFrameEffectFrame_OnUpdate()
	if bRecond then
		RideFrameEffectFrame_Texture:Hide();
		nLastTime = os.clock();
		bRecond = false;
	end

	if os.clock() - nLastTime > 0.1 then
		RideFrameEffectFrame_Texture:Show();
		bRecond = true;
	end
end

-- ���빲ͬ����
function InviteRideFrame_OnShow()
	--[[
	local mainplayer	= ActorMgr:getMainPlayer()
	local rider			= mainplayer:getRider();
	local inviteRole	= rider:getInviteRoleInfo()
	local font			= getglobal( "InviteRideFrameMessageFont" );
	font:SetText( inviteRole.m_szRoleName.."�����㹲ͬ���" );
	--]]
end

function IsRideInviteValid()
	local actor = ActorMgr:FindActor( t_inviteRidePlayers[1].id );
	local mainplayer = ActorMgr:getMainPlayer();
	local rider		 = mainplayer:getRider();
	if actor == nil then
		ShowMidTips( "����̫Զ��������Ч" );
		rider:refuseInviteRide( t_inviteRidePlayers[1]["id"], RIDE_REFUSE_IGNORE );
		return false;
	end

	local dist = util.CalActorsDistance( actor, mainplayer );
	if dist > 5 ^ 2 then
		ShowMidTips( "����̫Զ��������Ч" );
		rider:refuseInviteRide( t_inviteRidePlayers[1]["id"], RIDE_REFUSE_IGNORE );
		return false;
	end

	return true;
end

function CanAcceptMultiRide()
	-- ���ж��������ڵ�ͼ�Ƿ��ܹ����
	local nRealMapID = MapShower:getRealMapId();
	if nRealMapID >= MIN_COPY_ID then
		local pworldDef = getPworldDef( nRealMapID / 100 );
		if bitAnd( pworldDef.CtrlFlag, PWORLD_NO_RIDE ) then
			ShowMidTips( "�˵�ͼ�������" );
			return false;
		end
	end
	
	local mainplayer	= ActorMgr:getMainPlayer();
	local BufStatus		= mainplayer:getBufStatus();
	-- ���ж������Ƿ���ĳЩ������˵�״̬
	for _, id in ipairs( t_cannotRideStatus ) do
		if BufStatus:checkBuf( id ) then
			ShowMidTips( "����"..getStatusDef( id, 1 ).Name.."״̬�����ܽ�����˲���" );
			return false;
		end
	end

	-- ��������״̬���������
	if mainplayer:isDead() then
		ShowMidTips( "��������״̬�����ܽ�����˲���" );
		return false;
	end

	-- ���ڰ�̯״̬���������
	if mainplayer:isInStall() then
		ShowMidTips( "��̯״̬���޷����" );
		return false;
	end
	
	-- ���ڱ���״̬���������
	if mainplayer:isOnMonsterMachine() then
		ShowMidTips( "���ڻ�е��ʻ״̬�����ܽ�����˲���" );
		return false;
	end

	-- ���ڱ���״̬���������
	if mainplayer:isInMorph() then
		ShowMidTips( "���ڱ���״̬�����ܽ�����˲���" );
		return false;
	end
	
	-- ��������״̬���������
	if mainplayer:isInFight() then
		ShowMidTips( "����ս��״̬�����ܽ�����˲���" );
		return false;
	end
	
	-- ��������״̬���������
	if mainplayer:isInFly() then
		ShowMidTips( "���ڷ���״̬�����ܽ�����˲���" );
		return false;
	end
	-- �ж��Ƿ��ڱ��˵�״̬
	if mainplayer:isInCarry() then
		ShowMidTips("���ڱ���״̬�����ܽ�����˲���");
		return false;
	end

	return true;
end

function getLastSelectRide()
	return t_opData["ѡ��"]["rideID"], t_opData["ѡ��"]["level"];
end

function InviteRideFrame_AcceptBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	local rider	= mainplayer:getRider();
	if IsRideInviteValid() and CanAcceptMultiRide() then
		mainplayer:breakPreAction();
		rider:acceptInviteRide( t_inviteRidePlayers[1]["id"] );
	end
	
	table.remove( t_inviteRidePlayers, 1 );
	-- ��������˶����Ѿ�û�����������ߵĻ������������
	if table.getn( t_inviteRidePlayers ) == 0 then
		InviteRideFrame:Hide();
		return;
	end
	
	local frame = getglobal( "InviteRideFrame" );
	local font	= getglobal( "InviteRideFrameMessageFont" );
	-- ���������������ߣ��򵯳����µ�������
	t_inviteRidePlayers[1].startTime = GameMgr:getTickTime();
	font:SetText( GetPlayerName(t_inviteRidePlayers[1].name).."�����㹲ͬ���" );
end

function InviteRideFrame_RefuseBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer()
	local frame = getglobal( "InviteRideFrame" );
	local rider	= mainplayer:getRider();
	local actor = ActorMgr:FindActor( t_inviteRidePlayers[1].id );

	--if IsRideInviteValid() then
	rider:refuseInviteRide( t_inviteRidePlayers[1]["id"], RIDE_REFUSE_CANCEL );
	--end
	
	table.remove( t_inviteRidePlayers, 1 );
	-- ��������˶����Ѿ�û�����������ߵĻ������������
	if table.getn( t_inviteRidePlayers ) == 0 then
		InviteRideFrame:Hide();
		return;
	end
	
	local font	= getglobal( "InviteRideFrameMessageFont" );
	-- ���������������ߣ��򵯳����µ�������
	t_inviteRidePlayers[1].startTime = GameMgr:getTickTime();
	font:SetText( GetPlayerName(t_inviteRidePlayers[1].name).."�����㹲ͬ���" );
end

t_pinZhiDesc =	{
							[0] = "��ͨ����",
							[1] = "��������",
							[2] = "��������",
							[3] = "ʷʫ����",
						};

t_rideColor = {
		[0] = { 254, 249, 234 },
		[1] = { 66,  168, 61 },
		[2] = { 61,  109, 168 },
		[3] = { 151, 45, 249 },
		};

local nLevel = 0;
-- ����ǿ�����
function UpdateRideLevelUpFrame()
	local SPEED_UNIT	= 1;
	local nRidID		= t_opData["����"]["rideID"];
	local mainplayer	= ActorMgr:getMainPlayer();
	local rider			= mainplayer:getRider( );
	local rideInfo	= rider:getRideInfoById( nRidID );
	local nRideLevel	= rideInfo.nLevel;
	local rideDef		= getRideDef( nRidID, nRideLevel );
	local ridePinZhiDef = rider:getRidePinZhiDef( rideDef.PinZhi );
	
	local levelStuffDef;
	local t_curRideLevelUpItem	= GetCurRideLevelUpItem();
	if t_curRideLevelUpItem["list"] ~= -1 then
		local mainplayer	= ActorMgr:getMainPlayer();
		local container		= mainplayer:getContainer();
		local item		= container:getItem( t_curRideLevelUpItem["list"], t_curRideLevelUpItem["grid"] );
		levelStuffDef		= item:getItemDef();
	end
	
	local nIncrSpeedScale	= math.ceil( rideDef.RideSpd * 100 / CLIENT_BASE_MSPD );
	local nIntegralPart		= math.modf( nIncrSpeedScale / SPEED_UNIT );
	nIncrSpeedScale			= SPEED_UNIT * nIntegralPart;

	nLevel = ( nLevel == 0 and rideDef.Level or nLevel );

	local rate = rideDef.LevelUpRate/100;
	local szRate = "";
	if rate < 1 then
		szRate = "�ܵ�";
	elseif rate <= 10 then
		szRate = "��";
	elseif rate <= 20 then
		szRate = "�ϵ�";
	elseif rate <= 50 then
		szRate = "��";
	elseif rate <= 80 then
		szRate = "�ܸ�";
	else
		szRate = "�ǳ���";
	end

	local t_rideFonts = {	["RideLevelUpFrame_RideNameFont"]			= { value = rideDef.RideName },
							["RideLevelUpFrame_RideCurLevelFont"]		= { value = rideDef.Level .. "/".. ridePinZhiDef.MaxLevel },
							["RideLevelUpFrame_IncreaseSpeedFont"]		= { value = nIncrSpeedScale .. "%" },
							["RideLevelUpFrame_CurRidePinZhiFont"]		= { value = ridePinZhiDef.PinZhiName },
							["RideLevelUpFrame_LevelUpNeedBaoShiNumFont"]	= { value = ( levelStuffDef ~= nil and ( "��1" ) or "" ) },
							["RideLevelUpFrame_LevelUpSuccessJiLvFont"]	= { value = szRate },
							["RideLevelUpFrame_LevelUpCostBtnValueFont"] = { value = rideDef.MoneyVal },	-- ��ʱû�д���moneytype,Ĭ��Ϊ����
						};
	RideLevelUpFrame_CurRidePinZhiFont:SetTextColor( t_rideColor[rideDef.PinZhi][1], t_rideColor[rideDef.PinZhi][2], t_rideColor[rideDef.PinZhi][3] );
	
	for name, data in pairs( t_rideFonts ) do
		local font = getglobal( name );
		font:SetText( data.value );
	end

	local tex = getglobal( "RideLevelUpFrame_LevelUpNeedBaoShiBtnIcon" );	
	tex:SetSize(36,36);
	local countFont = getglobal("RideLevelUpFrame_LevelUpNeedBaoShiBtnCountFont");
	if levelStuffDef ~= nil then
		local szPath = GetItemEquipIconPath()..levelStuffDef.IconID..".tga";
		tex:SetTexture(  IsInExistence( szPath ) and szPath or DEFAULT_ITEM_PATH );
		tex:SetTexRelUV( 1, 1 );		
		local container	 = mainplayer:getContainer();
		local haveNum = container:getItemCount( levelStuffDef.ItemID );
		if haveNum > 999 then
			haveNum = math.floor( haveNum/1000 ) .."K";
		end
		countFont:SetText( haveNum );
	else
		tex:SetTexture( [[uires\\ZhuJieMian\\3.dds]] );
		tex:SetTexUV( 421, 197, 44, 44 );
		tex:SetSize(40,40);
		countFont:SetText("");
	end

	if rideDef.Level > nLevel then
		nLevel = rideDef.Level;
		local modelview = RideLevelUpFrame_LevelUpNeedBaoShiBtnModelView;
		modelview:setPetModel(0, 0, "effect\\entity\\fumo01.ent");
		modelview:setEntityPosition(0, 0, 100, 0);
		if not modelview:IsShown() then
			modelview:Show()
		end
	end

	UpdateShortCutRide();
end

function UpdateShortCutRide()
	local mainplayer	= ActorMgr:getMainPlayer();
	local rider			= mainplayer:getRider( );
	local rideInfo		= rider:getRideInfoById( t_opData["����"]["rideID"]  );
	for i = 1, MAX_SHORCUT do
		local shortcut = GameShortCut:getShortCut( i - 1 );
		if shortcut.m_nType == SCT_RIDE and shortcut.m_Id == rideInfo.RideID then
			GameShortCut:setShortCut( SCT_RIDE, rideInfo.RideID, i - 1, rideInfo.Endtime, rideInfo.nLevel );
			break;
		end
	end
end

local t_rideLevelUpFrameEvents =	
{	
	["GE_RIDELIST_CHG"]	= {}, ["GE_CONTAINER_UPDATE"] = {},
};

t_rideLevelUpFrameEvents["GE_RIDELIST_CHG"].func = 
function ( name )
	UpdateRideLevelUpFrame();
end

t_rideLevelUpFrameEvents["GE_CONTAINER_UPDATE"].func = 
function ( name )
	-- �鿴�Ƿ�Ҫ��������ʹ�õ���Ʒ
	local t_curRideLevelUpItem	= GetCurRideLevelUpItem();
	if t_curRideLevelUpItem["grid"] == -1 then
		return;
	end
	
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();

	local haveNum		= container:getItemCount( RIDE_PROCE_ITEM_ID );

	local useItem		= container:getItem( t_curRideLevelUpItem["list"], t_curRideLevelUpItem["grid"] );
	--���Ŀǰ�������Ʒ��������
	if useItem:getNum() > 0 then
		ClearRideLevelUpItem();
		Save2RideLevelUpFrame( useItem );
		return;
	--���Ŀǰ�������Ʒ����û�У�������Ĭ�ϵ�Ѹ��ʯ
	elseif useItem:getNum() == 0 and haveNum > 0 then
		ClearRideLevelUpItem();
		for i = 1, container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES ) do
			local item = container:getItem( CONTAINER_TYPE_SUNDRIES, i - 1 );
			if item:getItemId() == RIDE_PROCE_ITEM_ID then
				Save2RideLevelUpFrame( item );
				break;
			end
		end		
	end
	
	if haveNum > 0 then
		local countFont = getglobal("RideLevelUpFrame_LevelUpNeedBaoShiBtnCountFont");
		if haveNum > 999 then
			haveNum = math.floor( haveNum/1000 ) .."K";
		end
		countFont:SetText( haveNum );
		return;
	end

	ClearRideLevelUpItem();
end

function RideLevelUpFrame_OnLoad()
	t_rideLevelUpFrameEvents.__index = function ( tab, key )
								tab[key] = { func = function ( name ) end };
								return tab[key];
							end
	setmetatable( t_rideLevelUpFrameEvents, t_rideLevelUpFrameEvents );

	for event, _ in pairs( t_rideLevelUpFrameEvents ) do
		this:RegisterEvent( event );
	end
end

function RideLevelUpFrame_OnEvent()
	if this:IsShown() then
		t_rideLevelUpFrameEvents[arg1].func();
	end
end

local t_curRideLevelUpItem = { grid = -1, list = -1 };
function GetCurRideLevelUpItem()
	return t_curRideLevelUpItem;
end

function SetCurRideLevelUpItem( data )
	t_curRideLevelUpItem = data;
end

function CheckRideLevelUpFrameXinShou()
	-- ��һ��ǿ�����
	if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_STRENGTHEN_BTN ) then
		GameWizard:doStep( GAMEWIZARD_FIRST_BLINK_STRENGTHEN_BTN );
	end

	if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_CON_STRENGTHEN_RIDE_ITEM_BTN ) and GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_STRENGTHEN_BTN ) and 
			Quest:isQuestRun( 3134 ) then
		if not PlayerContainerFrame:IsShown() then
			PlayerContainerFrame:Show();
		else
			ShowContainerGuide();
		end
	end
	
	if GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_CON_STRENGTHEN_RIDE_ITEM_BTN ) and 
		not GameWizard:isDo( GAMEWIZARD_FIRST_PUT_STRENGTHEN_RIDE_TO_FRAME ) and		
			Quest:isQuestRun( 3134 ) then

		ShowRideLevelUpFrameXinShouBtn( { text = "�����������", btn = "RideLevelUpFrameXinShouBtn" } );
	end
end

function ShowRideLevelUpFrameXinShouBtn( data )
	local xinShouBtn	= getglobal( data["btn"] );
	local rich			= getglobal( xinShouBtn:GetName().."Rich" );
	local nSelfDefineExtendWidth = 10;
	rich:SetClientString( data["text"]);
	ReSizeTooltip( rich:GetName(), xinShouBtn:GetName(), nSelfDefineExtendWidth );
	
	local animationTexture = getglobal( xinShouBtn:GetName().."UVAnimationTex" );
	animationTexture:SetUVAnimation( 50, true );
	animationTexture:Show();
	xinShouBtn:Show();
end

function RideLevelUpFrame_OnShow()
	local mainplayer = ActorMgr:getMainPlayer();
	local container	= mainplayer:getContainer();
	if t_curRideLevelUpItem["grid"] ~= -1 then		
		local item = container:getItem( t_curRideLevelUpItem["list"], t_curRideLevelUpItem["grid"] );
		UnLockItem( t_curRideLevelUpItem["list"], t_curRideLevelUpItem["grid"], item:getItemId() );
	end

	CheckRideLevelUpFrameXinShou();
	if not PlayerContainerFrame:IsShown() then
		PlayerContainerFrame:Show();

		local curShowFrame = getglobal( "RideFrame" );
		if curShowFrame:GetRealRight() >= DEFAULT_UI_WIDTH then
			PlayerContainerFrame:SetPoint( "topright", "UIClient", "topleft", 
											curShowFrame:GetRealLeft() - 3 - this:GetWidth() - 3, 
											curShowFrame:GetRealTop() );
		else
			PlayerContainerFrame:SetPoint( "topleft", "UIClient", "topleft", 
											curShowFrame:GetRealLeft() + curShowFrame:GetWidth() + 3 + this:GetWidth() + 3, 
											curShowFrame:GetRealTop() );
		end

	end

	t_curRideLevelUpItem = { grid = -1, list = -1 };
	RideLevelUpFrame_LevelUpNeedBaoShiBtnModelView:Hide();
	nLevel = 0;

	local haveNum		= container:getItemCount( RIDE_PROCE_ITEM_ID );

	if haveNum > 0 then
		for i = 1, container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES ) do
			local item = container:getItem( CONTAINER_TYPE_SUNDRIES, i - 1 );
			if item:getItemId() == RIDE_PROCE_ITEM_ID then
				Save2RideLevelUpFrame( item );
				break;
			end
		end
	else
		UpdateRideLevelUpFrame();
	end
end

function RideLevelUpFrame_OnHide()
	local btn = getglobal( "RideLevelUpFrameXinShouBtn" );
	btn:Hide();
	if t_curRideLevelUpItem["grid"] ~= -1 then
		local mainplayer	= ActorMgr:getMainPlayer();
		local container		= mainplayer:getContainer();
		local item			= container:getItem( t_curRideLevelUpItem["list"], t_curRideLevelUpItem["grid"] );
		UnLockItem( t_curRideLevelUpItem["list"], t_curRideLevelUpItem["grid"], item:getItemId() );
	end

	SetCurRideLevelUpItem( { grid = -1, list = -1 } );
	t_opData["����"] = { rideID = 0, level = 0, };
	nLevel = 0;
	bOpenContainerFlag = true;
end

function RideLevelUpFrame_StrengthenBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	if t_opData["����"]["rideID"] == 0 then
		return;
	end

	local btn = getglobal( "RideLevelUpFrameXinShouBtn" );
	if btn:IsShown() and GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_STRENGTHEN_BTN ) and 
			not GameWizard:isDo( GAMEWIZARD_FIRST_PUT_STRENGTHEN_RIDE_TO_FRAME ) and		
			Quest:isQuestRun( 3134 ) then

			GameWizard:doStep( GAMEWIZARD_FIRST_PUT_STRENGTHEN_RIDE_TO_FRAME );
	end
	btn:Hide();
	
	local rider	= mainplayer:getRider();
	local rideInfo = rider:getRideInfoById( t_opData["����"]["rideID"] );
	local rideDef = getRideDef( rideInfo.RideID, rideInfo.nLevel );
	
	if mainplayer:getMoney() < rideDef.MoneyVal then
		ShowBuyMoneyFrame();
		return;
	end

	local ridePinZhiDef = rider:getRidePinZhiDef( rideDef.PinZhi );
	if rideInfo.nLevel >= ridePinZhiDef.MaxLevel then
		ShowMidTips( "�ѴﵽƷ�����ޣ��޷���������" );
		return
	end
	if t_curRideLevelUpItem["list"] == -1 then
		MessageBoxForItem("��ʾ",getItemDef(RIDE_PROCE_ITEM_ID).Name);
		MessageBoxForItemFrame:SetClientUserData(0,RIDE_PROCE_ITEM_ID);
		return;
	end
	rider:levelUpRide( t_opData["����"]["rideID"], t_curRideLevelUpItem["list"], t_curRideLevelUpItem["grid"] );
end

function RideLevelUpFrame_CancelBtn_OnClick()
	RideLevelUpFrame:Hide();
end

function Save2RideLevelUpFrame( item )
	local itemdef		= item:getItemDef();
	local isLevelUpItem = false;

	local nRidID		= t_opData["����"]["rideID"];
	local nRideLevel	= t_opData["����"]["level"];
	local rideDef		= getRideDef( nRidID, nRideLevel );
	
	for i = 1, MAX_RIDE_STUFF do
		local stuffInfo = rideDef.LevelUpStuff[i-1];
		if stuffInfo.ItemID == itemdef.ItemID then
			isLevelUpItem = true;
			break;
		end
	end

	if not isLevelUpItem then
		ShowMidTips( "����������������ߣ�Ѹ��ʯ" );
		return;
	end
	
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	-- ���Ѿ�����Ʒ������
	if t_curRideLevelUpItem["grid"] ~= -1 then
		local oldItem	= container:getItem( t_curRideLevelUpItem["list"], t_curRideLevelUpItem["grid"] );
		UnLockItem( t_curRideLevelUpItem["list"], t_curRideLevelUpItem["grid"], oldItem:getItemId() );
		t_curRideLevelUpItem = { grid = -1, list = -1 };
	end

	t_curRideLevelUpItem = { grid = item:getGridIdx(), list = item:getListType() };
	LockItem( item:getListType(), item:getGridIdx(), item:getItemId(), "RideLevelUpFrame" );

	UIEndDrag();
	UIMSG:getUIGV().DragFrom = DRAG_NONE;

	UpdateRideLevelUpFrame();

	-- �Ƿ��ǵ�һ�β���ǿ�����
	-- �ȼ�鱳��
	if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_CON_STRENGTHEN_RIDE_ITEM_BTN ) and 
		GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_STRENGTHEN_BTN ) and
		Quest:isQuestRun( 3134 ) then

		local effectFrame = getglobal( "PlayerContainerFrameEffectFrame" );
		effectFrame:Hide();
		GameWizard:doStep( GAMEWIZARD_FIRST_BLINK_CON_STRENGTHEN_RIDE_ITEM_BTN );
	end

	if GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_STRENGTHEN_BTN ) and not GameWizard:isDo( GAMEWIZARD_FIRST_PUT_STRENGTHEN_RIDE_TO_FRAME ) and		
			Quest:isQuestRun( 3134 ) then

		ShowRideLevelUpFrameXinShouBtn( { text = "�����������", btn = "RideLevelUpFrameXinShouBtn" } );
	end
end

function RideLevelUpFrame_LevelUpNeedBaoShiBtn_ReceiveDrag()
	local srcList = UIMSG:getUIGV().DragFrom;
	if srcList ~= CONTAINER_TYPE_SUNDRIES and srcList ~= CONTAINER_TYPE_STUFF then
		return;
	end

	local srcGrid		= UIGetDragUserData1();
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local item			= container:getItem( srcList, srcGrid );
	Save2RideLevelUpFrame( item );
end

function ClearRideLevelUpItem()
	local mainplayer		= ActorMgr:getMainPlayer();
	local container			= mainplayer:getContainer();
	local item				= container:getItem( t_curRideLevelUpItem["list"], t_curRideLevelUpItem["grid"] );
	UnLockItem( t_curRideLevelUpItem["list"], t_curRideLevelUpItem["grid"], item:getItemId() );

	t_curRideLevelUpItem = { grid = -1, list = -1 };

	UpdateRideLevelUpFrame()
end

function RideLevelUpFrame_LevelUpNeedBaoShiBtn_OnClick()
	if arg1 ~= "RightButton" then
		return;
	end
	
	if UIIsInDragState() then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	end

	if t_curRideLevelUpItem["grid"] == - 1 then
		return;
	end
	
	ClearRideLevelUpItem();
end

function RideLevelUpFrame_LevelUpNeedBaoShiBtn_OnEnter()	
	if t_curRideLevelUpItem["list"] == -1 then
		local nRidID		= t_opData["����"]["rideID"];
		local nRideLevel	= t_opData["����"]["level"];
		local rideDef		= getRideDef( nRidID, nRideLevel );
		local t_levelUpItem	= {};
		for i = 1, MAX_RIDE_STUFF do
			local stuffInfo = rideDef.LevelUpStuff[i-1];
			if stuffInfo.ItemID ~= 0 then
				table.insert( t_levelUpItem, stuffInfo.ItemID );
			end
		end
		
		if table.getn( t_levelUpItem ) == 0 then
			return;
		end

		local szText = "�����Ѹ��ʯ";
		
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = "RideLevelUpFrame",
							button = this:GetName() } );
	else
		local mainplayer	= ActorMgr:getMainPlayer();
		local container		= mainplayer:getContainer();
		local item			= container:getItem( t_curRideLevelUpItem["list"], t_curRideLevelUpItem["grid"] );
		local itemDef		= item:getItemDef();
			if itemDef == nil then
			return;
		end 
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, itemDef );
	end	
end

function RideLevelUpFrame_LevelUpNeedBaoShiBtn_OnLeave()
	local t_tipsFrames = { "NormalItemTipsFrame", "ArmItemTipsFrame", "SecondArmItemTipsFrame", "SpellTipsFrame", "GameTooltip" };
	for _, name in ipairs( t_tipsFrames ) do
		local frame = getglobal( name );
		frame:Hide();
	end
end

function RideLevelUpFrameLevelUpSuccessJiLvBtn_OnEnter()
	SetGameTooltips( this:GetName(),"�ɹ�����������ȼ����������仯" );
end

function RideCurLevelBtn_OnEnter()
	local mainplayer = ActorMgr:getMainPlayer();
	local rider	= mainplayer:getRider( );
	local rideInfo	= rider:getRideInfoById( t_opData["����"]["rideID"]  );
	local nRideLevel	= rideInfo.nLevel;
	local rideDef = getRideDef( rideInfo.RideID, rideInfo.nLevel );
	local ridePinZhiDef = rider:getRidePinZhiDef( rideDef.PinZhi );
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "���ﵱǰΪ" .. nRideLevel .. "������߿�������"..ridePinZhiDef.MaxLevel.."��", frame = "RideLevelUpFrame",
							button = this:GetName() } );
end

function IncreaseSpeedBtn_OnEnter()
	local SPEED_UNIT	= 1;
	local nRidID		= t_opData["����"]["rideID"];
	local mainplayer	= ActorMgr:getMainPlayer();
	local rider			= mainplayer:getRider( );
	local rideInfo	= rider:getRideInfoById( nRidID );
	local nRideLevel	= rideInfo.nLevel;
	local rideDef		= getRideDef( nRidID, nRideLevel );
	
	local nIncrSpeedScale	= math.ceil( rideDef.RideSpd * 100 / CLIENT_BASE_MSPD );
	local nIntegralPart		= math.modf( nIncrSpeedScale / SPEED_UNIT );
	nIncrSpeedScale			= SPEED_UNIT * nIntegralPart;

	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "��˺���������" .. nIncrSpeedScale .. "%���ƶ��ٶ�", frame = "RideLevelUpFrame",
							button = this:GetName() } );
end

function CurRidePinZhiBtn_OnEnter()
	local szText = "#cffffff��ͨ������������#n\n#c3fc117���ã�����ͨϡ��#n\n#c338dfd���㣺�����ø�ϡ��#n\n#c8e5cd8ʷʫ���������ϡ��#n"
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(), 
						button = this:GetName() } );
end
-------------------------------����ȼ�ת��---------------------
local szText = [[#cffffbe ת��˵����
#ca09b82 ��ת������ĵȼ�ת��Ŀ�����
#ca09b82 ��ת����ת������ȼ���Ϊ1��Ŀ����
#ca09b82 ��ĵȼ���Ϊԭ��ת������ĵȼ���]]
local szWarnig =[[#cffffbe 		ת������δ���й���������Ŀ������
#cffffbe �ȼ��Ѵﵽ����]]
function OpenRideMoveLevelFrame( npcId )
	RideFrame:Show();
	RideMoveLevelFrame:SetPoint( "topleft", "RideFrame", "topright", 0, 0 );
	RideMoveLevelFrame:Show();
	util.testNpcDialogDistance( npcId, RideMoveLevelFrame );
end
function RideMoveLevelFrame_OnLoad()
	RideMoveLevelFrameSrcRideBackTexture:Show();
	RideMoveLevelFrameDesRideBackTexture:Show();
	RideMoveLevelFrameDescRich:SetText( szText, 255,255,255 )
	RideMoveLevelFrameWarning:SetText( szWarnig, 255,255,255 )
end
function RideMoveLevelFrame_OnShow()
	RideMoveLevelFrameModelView:Hide();
	RideMoveLevelFrameOKBtn:Disable();
	RideMoveLevelFrameWarning:Hide();
	RideMoveLevelFrameWaringTex:Hide();
	RideMoveLevelFrameNeedMoney:SetText( "0" )
	RideMoveLevelFrameDesRide:SetClientUserData( 0, 0 );
	RideMoveLevelFrameDesRide:SetClientUserData( 1, 0 );
	RideMoveLevelFrameDesRideIconTexture:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
	RideMoveLevelFrameSrcRide:SetClientUserData( 0, 0 );
	RideMoveLevelFrameSrcRide:SetClientUserData( 1, 0 );
	RideMoveLevelFrameSrcRideIconTexture:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
end
function RideMoveLevelFrame_OnHide()
	RideMoveLevelFrameDesRide:SetClientUserData( 0, 0 );
	RideMoveLevelFrameDesRide:SetClientUserData( 1, 0 );
	RideMoveLevelFrameDesRideIconTexture:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
	RideMoveLevelFrameSrcRide:SetClientUserData( 0, 0 );
	RideMoveLevelFrameSrcRide:SetClientUserData( 1, 0 );
	RideMoveLevelFrameSrcRideIconTexture:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
	RideMoveLevelFrameModelView:Hide();
	RideMoveLevelFrameWarning:Hide();
	RideMoveLevelFrameWaringTex:Hide();
	if RideMoveMessageFrame:IsShown() then
		RideMoveMessageFrame:Hide();
	end
end

function RideMoveLevelFrame_OnUpdate()
	RideMoveLevelFrameWarning:Hide();
	RideMoveLevelFrameWaringTex:Hide();
	RideMoveLevelFrameOKBtn:Disable();
	RideMoveLevelFrameNeedMoney:SetText( "0" )
	RideMoveLevelFrameNeedMoney:SetTextColor( 255,255,190 )
	local mainplayer	= ActorMgr:getMainPlayer();
	local rider			= mainplayer:getRider();
	if RideMoveLevelFrameSrcRide:GetClientUserData(0) ~= 0 and rider:getRideInfoById( RideMoveLevelFrameSrcRide:GetClientUserData(0) ) == nil then
		RideMoveLevelFrame:Hide();
		return;
	end
	if RideMoveLevelFrameDesRide:GetClientUserData(0) ~= 0 and rider:getRideInfoById( RideMoveLevelFrameDesRide:GetClientUserData(0) ) == nil then
		RideMoveLevelFrame:Hide();
		return;
	end
	if rider:getRideInfoById( RideMoveLevelFrameSrcRide:GetClientUserData(0) ) ~= nil then
		local rideInfo = rider:getRideInfoById( RideMoveLevelFrameSrcRide:GetClientUserData(0) );
		local rideDef = getRideDef( rideInfo.RideID, rideInfo.nLevel );
		if rideDef.MoveLevelMoneyType == ATTR_ID_MONEY then
			local hasMoney = mainplayer:getMoney();
			if hasMoney < rideDef.MoveLevelMoneyVal then
				RideMoveLevelFrameNeedMoney:SetTextColor( 255,0,0 )
			else
				RideMoveLevelFrameNeedMoney:SetTextColor( 255,255,190 )
			end
			RideMoveLevelFrameNeedMoney:SetText( rideDef.MoveLevelMoneyVal )
			RideMoveLevelFrameMoneyTex:SetTexUV( 117, 494, 17, 18 );
		elseif rideDef.MoveLevelMoneyType == ATTR_ID_BULL then
			local hasBull = mainplayer:getBull();
			if hasBull < rideDef.MoveLevelMoneyVal then
				RideMoveLevelFrameNeedMoney:SetTextColor( 255,0,0 )
			else
				RideMoveLevelFrameNeedMoney:SetTextColor( 255,255,190 )
				
			end
			RideMoveLevelFrameNeedMoney:SetText( rideDef.MoveLevelMoneyVal )
			RideMoveLevelFrameMoneyTex:SetTexUV( 78, 494, 17, 18 );
		end
	end
	local hasRide = false
	if rider:getRideInfoById(RideMoveLevelFrameDesRide:GetClientUserData(0)) ~=nil and rider:getRideInfoById( RideMoveLevelFrameSrcRide:GetClientUserData(0) ) ~= nil then
		hasRide = true;
	end
	local moneyEnough = true;
	if hasRide then
		local rideInfo = rider:getRideInfoById( RideMoveLevelFrameSrcRide:GetClientUserData(0) );
		local rideDef = getRideDef( rideInfo.RideID, rideInfo.nLevel );
		if rideDef.MoveLevelMoneyType == ATTR_ID_MONEY then
			local hasMoney = mainplayer:getMoney();
			if hasMoney >= rideDef.MoveLevelMoneyVal then
				moneyEnough = true;
			end
		elseif rideDef.MoveLevelMoneyType == ATTR_ID_BULL then
			local hasBull = mainplayer:getBull();
			if hasBull >= rideDef.MoveLevelMoneyVal then
				moneyEnough = true;
			end
		end
	end
	if hasRide and moneyEnough then
		RideMoveLevelFrameOKBtn:Enable();
	end
	local srcRideInfo = rider:getRideInfoById( RideMoveLevelFrameSrcRide:GetClientUserData(0) );
	local srcRideDef = getRideDef( srcRideInfo.RideID, srcRideInfo.nLevel )
	local DesRideInfo = rider:getRideInfoById( RideMoveLevelFrameDesRide:GetClientUserData(0) );
	local desRideDef = getRideDef( DesRideInfo.RideID, DesRideInfo.nLevel )
	local desRidePinZhiDef = rider:getRidePinZhiDef( desRideDef.PinZhi )
	if srcRideInfo.nLevel == 1 or desRidePinZhiDef.MaxLevel <= DesRideInfo.nLevel then
		RideMoveLevelFrameOKBtn:Disable();
		RideMoveLevelFrameWarning:Show();
		RideMoveLevelFrameWaringTex:Show();
	end
end

function RideMoveReceiveDrag( szBtnName, dragfrom, dragdata1, dragdata2, dragdata3 )
	if RideMoveMessageFrame:IsShown() then return end
	local mainplayer	= ActorMgr:getMainPlayer();
	local rider			= mainplayer:getRider( );
	local rideInfo = rider:getRideInfoById( dragdata1 );
	if rideInfo == nil then
		ShowMidTips( "���������" );
		return;
	end
	local rideDef = getRideDef( rideInfo.RideID, rideInfo.nLevel );
	if rideDef == nil then return end
	if szBtnName == "RideMoveLevelFrameSrcRide" then
		if dragdata1 == RideMoveLevelFrameDesRide:GetClientUserData( 0 )then
			RideMoveLevelFrameDesRide:SetClientUserData( 0, 0 );
			RideMoveLevelFrameDesRide:SetClientUserData( 1, 0 );
			RideMoveLevelFrameDesRideIconTexture:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
		end
	elseif szBtnName == "RideMoveLevelFrameDesRide" then
		if dragdata1 == RideMoveLevelFrameSrcRide:GetClientUserData( 0 )then
			RideMoveLevelFrameSrcRide:SetClientUserData( 0, 0 );
			RideMoveLevelFrameSrcRide:SetClientUserData( 1, 0 );
			RideMoveLevelFrameSrcRideIconTexture:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
		end
	end
	local btn = getglobal( szBtnName );
	btn:SetClientUserData( 0, rideInfo.RideID );
	btn:SetClientUserData( 1, rideInfo.nLevel );
	local btnTex = getglobal( szBtnName.."IconTexture" );
	local IconPath	= GetItemEquipIconPath()..rideDef.IconID..".tga";
	btnTex:SetTexture( IsInExistence( IconPath ) and IconPath or DEFAULT_ITEM_PATH );
	UIEndDrag();
	UIMSG:getUIGV().DragFrom = DRAG_NONE;
end

function RideMoveLevelFrameRide_OnEnter()
	local mainplayer = ActorMgr:getMainPlayer();
	local rider		= mainplayer:getRider();
	local RideInfo	= rider:getRideInfoById( this:GetClientUserData(0) );
	if RideInfo == nil then
		if this:GetName() == "RideMoveLevelFrameSrcRide" then
			SetGameTooltips( this:GetName(), "�����Ҫת���ȼ�������" )
			return
		else
			SetGameTooltips( this:GetName(), "�����Ҫ�õ��ȼ�������" )
			return
		end
	end
	ShowRideTips( { id = RideInfo.RideID, lv = RideInfo.nLevel,
					relFrame = "RideMoveLevelFrame", button = this:GetName(), rideInfo = RideInfo, } )
end
function RideMoveLevelFrameRide_OnBeginDrag()
	UIMSG:getUIGV().DragFrom = DRAG_RIDE_MOVE;
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then
		return;
	end
	local rider		= mainplayer:getRider();
	local RideInfo	= rider:getRideInfoById( this:GetClientUserData(0));

	if RideInfo.RideID == 0  then
		return ;
	end

	local IconTex = getglobal( this:GetName().."IconTexture" );
	UIBeginDrag( IconTex:GetTexture() , RideInfo.RideID, RideInfo.nLevel, this:GetClientID() );
end

function RideMoveLevelFrameRide_OnClick()
	if arg1 == "RightButton" then
		if RideMoveMessageFrame:IsShown() then return end
		local btn = getglobal( this:GetName() );
		btn:SetClientUserData( 0, 0 );
		btn:SetClientUserData( 1, 0 );
		local btnTex = getglobal( this:GetName().."IconTexture" );
		btnTex:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
	end
end
function RideMoveLevelFrameOkBtn_OnClick()
	if RideMoveMessageFrame:IsShown() then return end
	local mainplayer = ActorMgr:getMainPlayer();
	local rider = mainplayer:getRider();
	local srcRideInfo = rider:getRideInfoById( RideMoveLevelFrameSrcRide:GetClientUserData(0) );
	local srcRideDef = getRideDef( srcRideInfo.RideID, srcRideInfo.nLevel )
	if srcRideDef.MoveLevelMoneyType == ATTR_ID_MONEY then
		if mainplayer:getMoney() < srcRideDef.MoveLevelMoneyVal then
			ShowBuyMoneyFrame()
			return;
		end
	end

	local DesRideInfo = rider:getRideInfoById( RideMoveLevelFrameDesRide:GetClientUserData(0) );
	local desRideDef = getRideDef( DesRideInfo.RideID, DesRideInfo.nLevel )
	local desRidePinZhiDef = rider:getRidePinZhiDef( desRideDef.PinZhi )
	if srcRideInfo.nLevel > desRidePinZhiDef.MaxLevel then		
		local szInfo = "#cff9600"..srcRideDef.RideName.."#cffffbe�ĵ�ǰ�ȼ�������#cff9600"..desRideDef.RideName.."#cffffbe�ĵȼ����ޣ�����ת�ƻ���ʧ���ֵȼ���ȷ��������"
		RideMoveMessageFrameDescRich:SetText( szInfo, 255,255,255 );
		RideMoveMessageFrame:Show();
		return;
	end
	rider:rideMoveLevel( RideMoveLevelFrameSrcRide:GetClientUserData(0), RideMoveLevelFrameDesRide:GetClientUserData(0) );
	RideMoveLevelFrameModelView:Show();
	RideMoveLevelFrameModelView:setPetModel(0,0,"effect\\entity\\pinzhizhuanyi.ent");
	RideMoveLevelFrameModelView:setEntityPosition(0,0,55,0);
end

function RideMoveMessageFrame_OnLoad()
	local szInfo = "srcRide�ĵ�ǰ�ȼ�������desRide�ĵȼ����ޣ�����ת�ƻ���ʧ���ֵȼ���ȷ��������"
	RideMoveMessageFrameDescRich:SetText( szInfo, 255,255,255 );
end
function RideMoveMessageFrameOkBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	local rider = mainplayer:getRider();
	rider:rideMoveLevel( RideMoveLevelFrameSrcRide:GetClientUserData(0), RideMoveLevelFrameDesRide:GetClientUserData(0) );
	RideMoveLevelFrameModelView:Show();
	RideMoveLevelFrameModelView:setPetModel(0,0,"effect\\entity\\pinzhizhuanyi.ent");
	RideMoveLevelFrameModelView:setEntityPosition(0,0,55,0);
	RideMoveMessageFrame:Hide();
end
function RideMoveMessageFrameCancleBtn_OnClick()
	RideMoveMessageFrame:Hide();
end