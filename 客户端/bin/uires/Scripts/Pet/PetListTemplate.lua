local nCurType		= XIANTYPE;
MAXPETITEM	= 10;
local nCheckPetType	= -1;
local nCheckPetGrid = -1;
local szParentName = "";

local t_callbackfun = {}
--@Desc:注册回调函数
--@Param:继承PetListTemplate的窗口名称
--@Param:点击选择宠物后对应的处理函数 该函数不需要参数
function SetCheckPetInfo( parentname, functionname )
	table.insert( t_callbackfun, {parentname , functionname} );
end


--t_selPetInfo = { [frameName] = { heightId = 0, lowId = 0, id = 0, index = -1 } } 
local t_selPetInfo = { };

function PetListTemplate_OnLoad()
	this:RegisterEvent( "GE_PET_STATE_CHANGE" );
	this:RegisterEvent( "GE_GET_NEW_PET" );
	this:RegisterEvent( "GE_DEL_PET" );
	for i = 1, MAXPETITEM do	
		local btn = getglobal( this:GetName() .. "PetNameButton" .. i );
		btn:SetOverlayRegion( -4, -8, 213, 31 );
	end
end


--@Desc:打开玩家宠物列表模板时响应
function PetListTemplate_OnShow()
	szParentName = this:GetName();
	DisCheckAllBtn( szParentName.."PetNameButton", MAXPETITEM );

	-- 清空上次打开面板时候选中的宠物
	-- TODO: 重新萃取
	local selectBtn = getglobal( szParentName .. "PetList_SelectBtn");
	selectBtn:SetClientUserData( 0, -1 );	
	t_selPetInfo[this:GetName()] = { heightId = 0, lowId = 0, id = 0, index = -1 };
	-- 设置刚开始显示的默认页面为侍卫
	PetShiLabel_Btn_OnClick();
end

--@Desc:将当前选中的宠物设置到摆摊列表中
function PetList_SelectBtn_OnClick()
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local petInfo		= petPackage:getPetInfo( this:GetClientUserData( 0 ) );

	local font;
	for i = 1, MAXPETITEM do
		local szParent = this:GetParent();
		local btn = getglobal( szParent .. "PetNameButton" .. i );
		if btn:IsChecked() then
			font = getglobal( szParent .. "PetNameButton" .. i .. "NameFont" );
			break;
		end
	end

	if  petPackage:isPetInState( this:GetClientUserData( 0 ), CLIENT_PET_STAT_FIGHT ) then
		ShowMidTips( "宠物处于召唤状态,不能上架！" );
		return;
	elseif  petPackage:isPetInState( this:GetClientUserData( 0 ), CLIENT_PET_STAT_SEEFIGHT ) then
		ShowMidTips( "宠物处于见习状态,不能上架！" );
		return;
	elseif petPackage:isPetInState( this:GetClientUserData( 0 ), CLIENT_PET_STAT_DEAD ) then
		ShowMidTips( "宠物处于死亡状态,不能上架！" );
		return;
	elseif petInfo.LoveLevel >= 2 then
		ShowMidTips( "宠物与玩家不离不弃，不能被交易" )
		return
	end

	for i=1, table.getn( t_callbackfun ) do
		if t_callbackfun[i][1] == szParentName then

			if szParentName == "StallSaleFramePetViewList" then
				t_callbackfun[i][2]( SHITYPE, this:GetClientUserData( 0 ), font );
			elseif szParentName == "MyTotalPetFrame" then
				if this:GetClientUserData( 0 ) == NONEPET then
					ShowMidTips( "请先选中一个宠物！" );
					return;
				end

				t_callbackfun[i][2]( SHITYPE, this:GetClientUserData( 0 ), font );
			elseif szParentName == "PetOperationFrame_MyTotalPetFrame" then
				local frame = getglobal( "PetOperationFrame" );
				-- 若当前显示的宠物孵化界面,则点击选择按钮无操作
				if frame:GetClientString( ) == "孵化" or  petPackage:isPetInState( this:GetClientUserData( 0 ), CLIENT_PET_STAT_FIGHT ) then
					if petPackage:isPetInState( this:GetClientUserData( 0 ), CLIENT_PET_STAT_FIGHT ) then
						ShowMidTips( "宠物处于召唤状态,不能进行当前操作！" );
					end
					return;
				end
				t_callbackfun[i][2]( { petInfo.WID.nHeight, petInfo.WID.nLow, petInfo.PetID, nCurType, this:GetClientUserData( 0 ) } );
			end

		end
	end
end

function PetListTemplate_OnEvent()
	if arg1 == "GE_PET_STATE_CHANGE" or arg1 == "GE_GET_NEW_PET" or arg1 == "GE_DEL_PET" then
		if this:IsShown() then
			updatePetList();
		end
	end
end

local function PetPackageUpdate()
	local frame = getglobal( szParentName );
	if frame:IsShown() then
		updatePetList();
	end
end

function PetListTemplate_OnHide()
	local shiLabelBtn = getglobal( this:GetName() .. "PetShiLabel_Btn" );
	if shiLabelBtn:IsChecked() then
		shiLabelBtn:DisChecked();
	end

	--[[local xianLabelBtn = getglobal( this:GetName() .. "PetXianLabel_Btn" );
	if xianLabelBtn:IsChecked() then
		xianLabelBtn:DisChecked();
	end
	--]]
	local selectBtn = getglobal( this:GetName() .. "PetList_SelectBtn");
	selectBtn:SetClientUserData( 0, -1 );
end

function updatePetList()
	if szParentName == "" then
		return;
	end
	local PetShiLabel_Btn	= getglobal( szParentName.."PetShiLabel_Btn" );
	SetPetListByType( SHITYPE );
end

function ShowSelectPet( petInfo, szRelFrame )
	SetSeeZiZhiPetInfo( petInfo );
	ShowPetInfo( petInfo, "SeePetZiZhiFrame", true );
	SeePetZiZhiFrame:SetClientString( szRelFrame );
	SeePetZiZhiFrame:Show();
end

--@Desc:查看宠物资质按钮
function PetList_SeeIntelligenceBtn_OnClick()
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage		= mainplayer:getPet();
	for i=1, MAXPETITEM do
		local btn = getglobal( szParentName.."PetNameButton".. i );
		if btn:IsChecked() then
			local PetInfo = petPackage:getPetInfo( btn:GetClientUserData(1) );

			if string.find( szParentName, "StallSaleFramePetViewList" ) ~= nil then
				ShowSelectPet( PetInfo, "StallSaleFrame" );
			elseif string.find( szParentName, "MyTotalPetFrame" ) ~= nil then
				ShowSelectPet( PetInfo, "TradeFrame" );
			end
			
			return;
		end
	end
	ShowMidTips( "请先选中要查看的宠物！" );
end

function ClearPetListButton()
	for i = 1, MAXPETITEM do
		local btn		= getglobal( szParentName .. "PetNameButton" .. i );
		local font		= getglobal( szParentName.."PetNameButton"..i.."NameFont" );
		local stateTex	= getglobal( szParentName .. "PetNameButton" .. i .. "StateTex" )
		local loveLevelfont = getglobal( szParentName .. "PetNameButton" .. i .. "LoveLevelFont" )
		local loveLevelTex	= getglobal( szParentName .. "PetNameButton" .. i .. "LoveLevelTex" )
		local loveLevelBtn = getglobal( szParentName .. "PetNameButton" .. i .. "LoveLevelBtn" )
		btn:DisChecked();
		btn:Disable();
		font:SetText( "" );
		stateTex:Hide();
		loveLevelfont:SetText( "" )
		loveLevelTex:Hide()
		loveLevelBtn:Hide()
	end
end

function SetPetListByType( nPetType )
	local nCount = 1;
	local nMaxPet = 0;
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();

	ClearPetListButton();
	for i=1, petPackage:getPetNum() do
		local nGrid			= i-1;
		local PetInfo		= petPackage:getPetInfo( i-1 );
		if PetInfo.PetID ~= 0 then
			local PetDefInfo	= getPetDef( PetInfo.PetID );
			local szPetName		= PetInfo.Name;
			if szPetName == "" then
				szPetName = PetDefInfo.Name;
			end

			local petbtn		= getglobal( szParentName .. "PetNameButton" .. nCount );
			local petnamefont	= getglobal( szParentName .. "PetNameButton" .. nCount .. "NameFont" );
			local stateTex		= getglobal( szParentName .. "PetNameButton" .. nCount .. "StateTex" )
			local loveLevelfont = getglobal( szParentName .. "PetNameButton" .. nCount .. "LoveLevelFont" )
			local loveLevelTex	= getglobal( szParentName .. "PetNameButton" .. nCount .. "LoveLevelTex" )
			local loveLevelBtn	= getglobal( szParentName .. "PetNameButton" .. nCount .. "LoveLevelBtn" )
			petbtn:Enable();
			petbtn:SetClientUserData( 0, nPetType ); -- 宠物的类型
			petbtn:SetClientUserData( 1, nGrid );	 -- 宠物的栏位
			petbtn:Show();
			petnamefont:SetText( szPetName );
			loveLevelfont:SetText( GetPetLoveLevelDesc( PetInfo.LoveLevel ) )
			loveLevelBtn:SetClientUserData( 0, PetInfo.LoveLevel )
			loveLevelBtn:Show()
			-- 当宠物已经在交易栏或者摆摊面板上的时候，将宠物名字的颜色调为灰色
			if ( szParentName == "MyTotalPetFrame" and TradeFrame:IsShown() and
					GameExchange:isInExchangePet( SHITYPE, nGrid ) ) or
				( szParentName == "StallSaleFramePetViewList" and StallSaleFrame:IsShown() and
					GameStallManager:isInStallIPet( SHITYPE, nGrid ) ) then
				petnamefont:SetTextColor( 131, 128, 108 );
			else
				petnamefont:SetTextColor( 255, 254, 189 );
			end

			if petPackage:isPetInState( nGrid, CLIENT_PET_STAT_FIGHT ) then
				stateTex:SetTexture( [[uires\ui\Za\01.tga]] );
				stateTex:Show();
			elseif petPackage:isPetInState( nGrid, CLIENT_PET_STAT_SEEFIGHT )  then
				stateTex:SetTexture( [[uires\ui\Za\02.tga]] );
				stateTex:Show();
			else
				stateTex:Hide();
			end
			local uv = t_petLoveLevelTextures[PetInfo.LoveLevel].UV
			loveLevelTex:SetTexUV( uv.x, uv.y, uv.width, uv.height )
			loveLevelTex:Show()

			nCount = nCount + 1;
		end
	end
end

--@Desc:获取摆摊待售宠物的栏位
function GetStallPetGridIndex()
	local allSelfItem	= GameStallManager:getSelfStallItem();
	local allSelfPet	= allSelfItem.m_PetData;
	for i = 1, CLIENT_STALL_MAX_PET do
		local onePetInfo = allSelfPet[i-1];
		if onePetInfo.m_nDefID == 0 then
			return i - 1;
		end
	end

	return CLIENT_STALL_MAX_PET-1;
end

--@Desc:单击宠物名字时响应选中状态
function PetNameButton_OnClick()
	local font = getglobal( this:GetName() .. "NameFont" );
	assert( font ~= nil, "获取空控件!!" );
	if font:GetText() == "" then
		return;
	end
	DisCheckAllBtn( szParentName.."PetNameButton", MAXPETITEM );

	this:Checked();

	local selectBtn		= getglobal( szParentName .. "PetList_SelectBtn");
	selectBtn:SetClientUserData( 0, this:GetClientUserData( 1 ) );
end

function ResetMyTotalPetFrameFont( petInfo, petDefInfo, szParentName )
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	for i = 1, MAXPETITEM do
		local petNamebtn	= getglobal( szParentName .. "PetNameButton" .. i );
		local font			= getglobal( szParentName .. "PetNameButton" .. i .. "NameFont" );
		local nGridIdx		= petNamebtn:GetClientUserData( 1 );
		local listPetInfo	= petPackage:getPetInfo( i - 1 );
		if listPetInfo.WID.nHeight == petInfo.WID.nHeight and listPetInfo.WID.nLow == petInfo.WID.nLow  then
			font:SetTextColor( 255, 254, 189 );
			break;
		end
	end
end

--@Desc:双击宠物名字时响应查看资质状态
function PetNameButton_OnDoubleClick()
	local font = getglobal( this:GetName() .. "NameFont" );
	assert( font ~= nil, "获取空控件!!" );
	if font:GetText() == "" then
		return;
	end

	local selectBtn		= getglobal( szParentName .. "PetList_SelectBtn");
	selectBtn:SetClientUserData( 0, this:GetClientUserData( 1 ) );

	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local petInfo		= petPackage:getPetInfo( selectBtn:GetClientUserData( 0 ) );
	-- 若为摆摊面板,则双击时查看宠物资质
	if szParentName == "StallSaleFramePetViewList" then
		PetList_SeeIntelligenceBtn_OnClick();
	-- 若为交易面板,双击为将宠物上架到交易栏中
	elseif szParentName == "MyTotalPetFrame" then
		-- 查看宠物当前的状态
		if  petPackage:isPetInState( selectBtn:GetClientUserData( 0 ), CLIENT_PET_STAT_FIGHT )  then
			ShowMidTips( "宠物处于召唤状态,不能上架！" );
			return;
		elseif petPackage:isPetInState( selectBtn:GetClientUserData( 0 ), CLIENT_PET_STAT_SEEFIGHT ) then
			ShowMidTips( "宠物处于见习状态,不能上架！" );
			return;
		elseif petPackage:isPetInState( selectBtn:GetClientUserData( 0 ), CLIENT_PET_STAT_DEAD ) then
			ShowMidTips( "宠物处于死亡状态,不能上架！" );
			return;
		elseif petInfo.LoveLevel >= 2 then
			ShowMidTips( "宠物与玩家不离不弃，不能被交易" )
			return
		end
		sendPetForTrade( SHITYPE, selectBtn:GetClientUserData( 0 ), font );
	elseif szParentName == "PetOperationFrame_MyTotalPetFrame" then
		local frame = getglobal( "PetOperationFrame" );

		PetList_SeeIntelligenceBtn_OnClick();
		SetOperationCurSelectPetInfo( { petInfo.WID.nHeight, petInfo.WID.nLow, petInfo.PetID, nCurType, selectBtn:GetClientUserData( 0 ) } );
	end
end

--@Desc:设置仙子列表
--[[
function PetXianLabel_Btn_OnClick()
	local PetXianBtn = getglobal( szParentName.."PetXianLabel_Btn" );
	PetXianBtn:Checked();

	local PetShiLabel_Btn = getglobal( szParentName.."PetShiLabel_Btn" );
	PetShiLabel_Btn:DisChecked();
	nCurType = XIANTYPE;
	updatePetList();

	DisCheckAllBtn( szParentName .. "PetNameButton", MAXPETITEM );

	local selectBtn = getglobal( szParentName .. "PetList_SelectBtn" );
	selectBtn:SetClientUserData( 0, -1 );
end
--]]
--@Desc:设置侍卫列表
function PetShiLabel_Btn_OnClick()
	local PetShiBtn = getglobal( szParentName .. "PetShiLabel_Btn" );
	PetShiBtn:Checked();

	--local PetXianLabel_Btn = getglobal( szParentName .. "PetXianLabel_Btn" );
	--PetXianLabel_Btn:DisChecked();
	nCurType = SHITYPE;
	updatePetList();

	DisCheckAllBtn( szParentName .. "PetNameButton", MAXPETITEM );

	local selectBtn = getglobal( szParentName .. "PetList_SelectBtn");
	selectBtn:SetClientUserData( 0, -1 );
end

function StallPetItemLoveLevelBtn_OnEnter()
	local level = this:GetClientUserData( 0 )
	ShowGameTooltip( { rich = "WieGameTooltipRichText", tipsFrame = "WideGameTooltip", text = tLoveLevelDesc[level], frame = this:GetParent(),
						button = this:GetName() } );
	local parent = getglobal( this:GetParent() )
	parent:Highlight()
end

function StallPetItemLoveLevelBtn_OnLeave()
	local parent = getglobal( this:GetParent() )
	if not isPointInFrame( parent:GetName() ) then
		parent:DisHighlight()
	end
	WideGameTooltip:Hide()
end