NONE_LIST_TYPE			= -1;
CANGBAOTUPOS_NUM		= 8;
MAX_CANG_BAO_BTN		= 10

local MAP_TEX_WIDTH		= 258;
local MAP_TEX_HEIGHT	= 258;
local fZoomScale		= MAP_TEX_WIDTH / 1024;

local t_GameCBTFrameControl = { listType = NONE_LIST_TYPE, gridx = 0, id = 0 };
function GetGameCBTFrameControl()
	return t_GameCBTFrameControl;
end

t_GameCBTFrameControl["canStartCangBao"] = 
function ( self, data )
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local item			= container:getItem( data["listType"], data["gridx"] );
	if container:isItemInCD( item:getItemId() ) then
		return false;
	end

	local gameCBT		= mainplayer:getCBTInfo();
	if not gameCBT:isCangBaoTuItem( item:getItemId() ) then
		return false;
	end

	local vipNum = VipManager:isOwnVipFunc( STAR_RESULT_CANGBAOTU_TOLUA ) and VipManager:getCangBaoTuVipNum() or 0;

	if gameCBT:getCanUseTimes() + vipNum <= 0 then
		ShowMidTips( "���ղر�ͼʹ�ô���������" );
		return false;
	end

	return true;
end

t_GameCBTFrameControl["startCangBao"] = 
function ( self, data )
	local frame			= getglobal( "GameCBTFrame" );
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	
	if frame:IsShown() then
		local item	= container:getItem( self["listType"], self["gridx"] );
		UnLockItem( item:getListType(), item:getGridIdx(), item:getItemId() );
		self:setCBTInfo( { ["listType"] = data["listType"], ["gridx"] = data["gridx"], ["id"] = item:getItemId() } );
		UpdateGameCBTFrame();
	else
		local item				= container:getItem( data["listType"], data["gridx"] );
		local gameCBT			= mainplayer:getCBTInfo();
		local nItemID			= item:getItemId();
		local oneCangBaoTuInfo	= gameCBT:getCangBaoTuMapInfoByItemID( nItemID );
		if oneCangBaoTuInfo.MapID == 0 then
			ShowMidTips( "�ȼ�����" );
			return;
		end
		
		self:setCBTInfo(  { ["listType"] = data["listType"], ["gridx"] = data["gridx"], ["id"] = nItemID } );
		frame:Show();
	end
	
	local item	= container:getItem( data["listType"], data["gridx"] );
	-- �����Ʒ��ӳɹ�����ס����Ʒ
	LockItem( data["listType"], data["gridx"], item:getItemId(), "GameCBTFrame" );
end

t_GameCBTFrameControl["endCangBao"] = 
function ( self, data )
	self:setCBTInfo( { listType = NONE_LIST_TYPE, gridx = 0, id = 0 } );
	ClearItemLock( "GameCBTFrame" );
end

t_GameCBTFrameControl["setCBTInfo"] = 
function ( self, data )
	self["listType"]	= data["listType"];
	self["gridx"]		= data["gridx"];
	self["id"]			= data["id"];
end

function ClearCangBaoPos()
	for i = 1, MAX_CANG_BAO_BTN do
		local btn = getglobal( "GameCBTFrameBaoZangPosBtn"..i );
		btn:Hide();
	end
end

function UpdateGameCBTFrame()
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local item			= container:getItem( t_GameCBTFrameControl["listType"], t_GameCBTFrameControl["gridx"] );
	local gameCBT		= mainplayer:getCBTInfo();
	local nItemID		= item:getItemId();
	if nItemID == 0 then
		return;
	end
	
	local itemDef			= getItemDef( nItemID );
	local oneCangBaoTuInfo	= gameCBT:getCangBaoTuMapInfoByItemID( nItemID );

	-- TODO: �Ƿ�Ҫ�Ӹ���ȡ����ID�Ĳ���

	local mapDef			= getMapDef( oneCangBaoTuInfo.MapID );
	local nMapTexID			= mapDef.MapID;
	if mapDef.ReferMapID ~= 0 then
		nMapTexID = mapDef.ReferMapID;
	end
	
	local t_textures = 
	{
		["GameCBTFrameMapBtnTex"] = {	path = "uires\\DiTuLei\\CangBaoTuDiTu\\"..nMapTexID..".tga",
									},

	};
	util.SetTgas( t_textures );

	local t_riches = 
	{ ["GameCBTFrameCanUseTimesDescRich"] = 
		{ 
		value = "    ����ʣ��Ѱ��������#G"..(gameCBT:getCanUseTimes()>0 and gameCBT:getCanUseTimes() or 0).."��#n", 
		color = { r = 255, g = 254, b = 189 } 
		}, 
		["GameCBTFrameVipUseTimesDescRich"] = 
		{
		value = "    ��������Ȩʣ��Ѱ��������#G"..(gameCBT:getCanUseTimes()>0 and VipManager:getCangBaoTuVipNum() or (gameCBT:getCanUseTimes()+VipManager:getCangBaoTuVipNum())).. "��#n", 
		color = { r = 255, g = 254, b = 189 } 
		},
	};
	util.SetRiches( t_riches );
	local mapDef		= getMapDef( oneCangBaoTuInfo.MapID );
	local nTexID		= mapDef.ReferMapID ~= 0 and mapDef.ReferMapID or oneCangBaoTuInfo.MapID
	local cangBaoTuDef	= getCangBaoTuDefDef( nTexID, itemDef.WearPre.RoleLevelMin, nItemID );
	local mapSize		= MapShower:getMapSize( nTexID );
	ClearCangBaoPos();

	local worldPosInfo	= oneCangBaoTuInfo.Pos;
	local btn = getglobal( "GameCBTFrameBaoZangPosBtn1" );
	
	local texOffset	= getPointCoordOnTex( worldPosInfo.X, worldPosInfo.Y, mapSize.first, mapSize.second, nTexID );
	btn:SetPoint( "center", "GameCBTFrameMapBtnTex", "topleft", texOffset.x * fZoomScale, texOffset.y * fZoomScale );
	btn:SetClientUserData( 0, nTexID );
	btn:SetClientUserData( 1, worldPosInfo.X );
	btn:SetClientUserData( 2, worldPosInfo.Y );
	btn:Show();
	local tex = getglobal( btn:GetName() .. "UVAnimationTex" );
	tex:StartAlphaAmin( 1 );
		
	-- �����������ù���
	if gameCBT:isRequestReset() or container:isItemInCD( nItemID ) then
		GameCBTFrame_ResetBtn:Disable();
	else
		GameCBTFrame_ResetBtn:Enable();
	end

	UpdateWaJueBtnState();
end

local t_GameCBTFrameEvents =	{	["GE_CONTAINER_UPDATE"]	= {}, ["GE_CANGBAO_UPDATE"] = {}, ["GE_SUCCESS_GET_WA_JUE"] = {},
								};

t_GameCBTFrameEvents["GE_CONTAINER_UPDATE"].func = 
function ()
	if not GameCBTFrame:IsShown() then
		return;
	end
	
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local useItem		= container:getItem( t_GameCBTFrameControl["listType"], t_GameCBTFrameControl["gridx"] );
	if useItem:getNum() > 0 then
		UpdateGameCBTFrame();
		return;
	end
	ClearItemLock( "GameCBTFrame" );
end

t_GameCBTFrameEvents["GE_CANGBAO_UPDATE"].func = 
function ()
	if not GameCBTFrame:IsShown() then
		return;
	end

	UpdateGameCBTFrame();
end

t_GameCBTFrameEvents["GE_SUCCESS_GET_WA_JUE"].func = 
function ()
	if GameCBTFrame:IsShown() then
		GameCBTFrame:Hide();
	end
end

local fScale = MAP_TEX_WIDTH / 1024;
t_specialCangBaoMapDesc =	
{	[2100] = { mapValidNormalizeTextureWidth = 1024 * fScale, mapValidNormalizeTextureHeight = 842 * fScale,	
				leftOffset = 0,	topOffset = 686,
				completeTextureWidth = 2048, completeTextureHeight = 2048, 
				clicpTextureWidth = 1658,	clicpTextureHeight = 1658 
			},

[2500] = { mapValidNormalizeTextureWidth = 1024 * fScale, mapValidNormalizeTextureHeight = 1024 * fScale,	
			leftOffset = 90, topOffset = 335,
			completeTextureWidth = 1600, completeTextureHeight = 1600, 
			clicpTextureWidth = 1024,	clicpTextureHeight = 1024 
		},
};

function calScreenPosFromWorldPos( nRealX, nRealY, nMapWidth, nMapHeight, nMapID )
	local nMapTextLeft			= t_specialCangBaoMapDesc[nMapID] ~= nil and t_specialCangBaoMapDesc[nMapID].leftOffset or 0;
	local nMapTextTop			= t_specialCangBaoMapDesc[nMapID] ~= nil and t_specialCangBaoMapDesc[nMapID].topOffset or 0;

	local nMapWorldTextWidth	= t_specialCangBaoMapDesc[nMapID] ~= nil and t_specialCangBaoMapDesc[nMapID].completeTextureWidth 
									or MAP_TEX_WIDTH;
	local nMapWorldTextHeight	= t_specialCangBaoMapDesc[nMapID] ~= nil and t_specialCangBaoMapDesc[nMapID].completeTextureHeight 
									or MAP_TEX_HEIGHT;

	local nMapClicpTextWidth	=  t_specialCangBaoMapDesc[nMapID] ~= nil and t_specialCangBaoMapDesc[nMapID].clicpTextureWidth 
									or MAP_TEX_WIDTH;
	local nMapClicpTextHeight	= t_specialCangBaoMapDesc[nMapID] ~= nil and t_specialCangBaoMapDesc[nMapID].clicpTextureHeight 
									or MAP_TEX_HEIGHT;

	local nPlayerOffsetXOnClicpTexture = nRealX * nMapWorldTextWidth / nMapWidth - nMapTextLeft;
	local nPlayerOffsetYOnClicpTexture = ( nMapHeight - nRealY ) * nMapWorldTextHeight / nMapHeight - nMapTextTop;

	local nPlayerOffsetXOnNormalizeTexture = nPlayerOffsetXOnClicpTexture * MAP_TEX_WIDTH / nMapClicpTextWidth;
	local nPlayerOffsetYOnNormalizeTexture = nPlayerOffsetYOnClicpTexture * MAP_TEX_HEIGHT / nMapClicpTextHeight;
	
	return { x = nPlayerOffsetXOnNormalizeTexture, y = nPlayerOffsetYOnNormalizeTexture };
end

function GameCBTFrame_OnLoad()
	t_GameCBTFrameEvents.__index = 
	function ( tab, key )
		tab[key] = { func = function () end };
		return tab[key];
	end

	setmetatable( t_GameCBTFrameEvents, t_GameCBTFrameEvents );

	for event, _ in pairs( t_GameCBTFrameEvents ) do
		this:RegisterEvent( event );
	end
	this:setUpdateTime( 1 );
end

function GameCBTFrame_OnEvent()
	if not this:IsShown() then
		return;
	end

	t_GameCBTFrameEvents[arg1].func();
end

function GameCBTFrame_OnShow()
	--[[
	local t_riches = 
	{ 
		[this:GetName().."TitleDescRich"] = 
		{ 
		value = "#G�ҵ�ͼʾ��λ���ھ򱦲�#n", 
		color = { r = 255, g = 254, b = 189  } 
		}, 
		[this:GetName().."WarnDescRich"] = 
		{ 
		value = "����������λ��", 
		color = { r = 255, g = 254, b = 189  } 
		}, 

	  	[this:GetName().."AdviseDescRich"] = 
		{ 
		value = "    ���ѣ����òر�ͼ��������һ��Ѱ������\n    ���飺�����пɸ��ÿ˷����裬������", 
		color = { r = 255, g = 254, b = 189 } 
		},
	};
	util.SetRiches( t_riches );
	--]]
	UpdateGameCBTFrame();
end

function GameCBTFrame_OnHide()
	t_GameCBTFrameControl:endCangBao();
end

function GameCBTFrame_OnUpdate()
	local mainplayer	= ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local btn = getglobal( "GameCBTFrameBaoZangPosBtn1" );
	local tex = getglobal( "GameCBTFrame_WaJueBtnUVAnimationTex" );
	local mapId = btn:GetClientUserData(0);
	local posX = btn:GetClientUserData(1);
	local posY = btn:GetClientUserData(2);
	local multiMapFind = MapShower:getMultiMapFindControl();
	if MapShower:getRealMapId() == mapId and mainplayer:getPlayerX()/10 == posX and mainplayer:getPlayerZ()/10 == posY then
		GameCBTFrameStateDesc:SetText("�ѵ���ر��ص㣬����ھ��ҵ�����");
		if not tex:IsShown() then
			tex:SetUVAnimation( 50, true );
			tex:Show();
		end
	else
		GameCBTFrameStateDesc:SetText("���ͼʾ�ı��ر�ǣ�ǰ���ر��ص�");
		tex:Hide();
	end
end

-- �ھ�ť
function UpdateWaJueBtnState()
	local nItemID = t_GameCBTFrameControl["id"];
	if nItemID == 0 then
		GameCBTFrame_WaJueBtn:ClearPushState();
		GameCBTFrame_WaJueBtn:Disable();		
		GameTooltip:Hide();
		return;
	end

	-- ����Ʒ����CD,Ҳdisable
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	
	if GameCBTFrame_WaJueBtn:IsEnable() and container:isItemInCD( nItemID ) then
		GameCBTFrame_WaJueBtn:ClearPushState();
		GameCBTFrame_WaJueBtn:DisHighlight();
		GameCBTFrame_ResetBtn:ClearPushState();
		GameCBTFrame_ResetBtn:DisHighlight();
		GameCBTFrame_WaJueBtn:Disable();
		GameCBTFrame_ResetBtn:Disable();		
		GameTooltip:Hide();
		return;
	end
	
	if not GameCBTFrame_WaJueBtn:IsEnable() and not container:isItemInCD( nItemID ) then
		GameCBTFrame_WaJueBtn:Enable();
		local gameCBT = mainplayer:getCBTInfo();
		if not gameCBT:isRequestReset() then
			GameCBTFrame_ResetBtn:Enable();
		end
		
	end
end

function GameCBTFrame_WaJueBtn_OnUpdate()
	UpdateWaJueBtnState();
end

function GameCBTFrame_WaJueBtn_OnEnter()
	local szText = "����ر��ص㣬������ھ������\n#G�ɹ��ھ�����һ��Ѱ������#n";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function GameCBTFrame_WaJueBtn_OnLeave()
	GameTooltip:Hide();
end

function GameCBTFrame_TipsBtn_OnEnter()
	local szText = "���С��ͼ�϶˵�ָ����ť��#GN#n��\n�ɻָ����ϱ������ӽǣ�����Ѱ��";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end 

function GameCBTFrame_WaJueBtn_OnClick()
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local item			= container:getItem( t_GameCBTFrameControl["listType"], t_GameCBTFrameControl["gridx"] );
	if item:getItemId() == 0 then
		GameCBTFrame:Hide();
		return;
	end
	container:usedItem( t_GameCBTFrameControl["listType"], t_GameCBTFrameControl["gridx"] );
end

-- ���ð�ť
function GameCBTFrame_ResetBtn_OnEnter()
	local szText = "��������òر�ͼ\n#Gÿ����������һ��Ѱ������#n";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function GameCBTFrame_ResetBtn_OnLeave()
	GameTooltip:Hide();
end

function GameCBTFrame_ResetBtn_OnClick()
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local item			= container:getItem( t_GameCBTFrameControl["listType"], t_GameCBTFrameControl["gridx"] );
	local nItemID = item:getItemId();
	assert( nItemID ~= 0  );
	if nItemID == 0 then
		return;
	end

	local gameCBT = mainplayer:getCBTInfo();
	gameCBT:send_reset( nItemID );

	gameCBT:setRequestReset( true );
	this:ClearPushState();
	this:DisHighlight();
	this:Disable();
end

function GameCBTFrame_PosBtn_OnEnter()
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "���ǰ����������λ��", frame = this:GetParent(),
						button = this:GetName() } );
	local tex = getglobal( this:GetName() .. "UVAnimationTex" );
	if tex:IsPlayAlphaAnim() then
		tex:Hide();
	end
end

function GameCBTFrame_PosBtn_OnLeave()
	GameTooltip:Hide();
	local tex = getglobal( this:GetName() .. "UVAnimationTex" );
	if not tex:IsPlayAlphaAnim() then
		tex:StartAlphaAmin( 1 );
	end
	tex:Show();
end

function GameCBTFrame_PosBtn_OnClick()
	MapShower:GetMaxMapContal():linkMoveto( this:GetClientUserData(0), this:GetClientUserData(1), this:GetClientUserData(2) );
end

function GameCBTFrame_GuideBtn_OnEnter()
	local t_Desc = { "�����ͼ�ĺ���ǰ���ر��ص�", "����ر��ص����ھ������", "����·��ġ��ھ򡱰�ť���ھ򱦲�" };
	SetGameTooltips( this:GetName(),  t_Desc[this:GetClientID()] );
end

---------------		�򿪵�ͼ����	------------------------

local t_ItemDesc = 
{
	[5300603] = "\t���������ȥ��ɳǰ�ڸ�����Ѱ��һ����������Ѿ��������������㹻���ˣ��Ϳ��������£������ķ����ҵ���һ������\n\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t��˹��",
	[5300604] = "\t��ůȪ�������һֱ�������ﰢķŵ���޵����Ϸ��������и��ܴ�ĵ����Լ�һ�Ŵ�����˸ߵ�ʯͷ����ϸ����ǿ�ʯͷ��\n\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t��˹��",
}

function OpenItemMap( itemid, mapid, x, y )
	if not OpenItemMapFrame:IsShown() then
		OpenItemMapFrame:Show();
	end
	OpenItemMapFrame:SetClientUserData( 0,itemid );

	local mapDef		= getMapDef( mapid );
	local nTexID		= mapDef.ReferMapID ~= 0 and mapDef.ReferMapID or mapid;
	local mapSize		= MapShower:getMapSize( nTexID );

	local nMapTexID			= mapDef.MapID;
	if mapDef.ReferMapID ~= 0 then
		nMapTexID = mapDef.ReferMapID;
	end
	
	local t_textures = 
	{
		["OpenItemMapFrameMapBtnTex"] = {	path = "uires\\DiTuLei\\CangBaoTuDiTu\\"..nMapTexID..".tga",
									},

	};
	util.SetTgas( t_textures );

	local screenPosInfo	= calScreenPosFromWorldPos( x, y, mapSize.first, mapSize.second, nTexID );
	local btn = getglobal( "OpenItemMapFrameBaoZangPosBtn" );
	btn:SetPoint( "center", "OpenItemMapFrameMapBtnTex", "topleft", screenPosInfo.x, screenPosInfo.y );
	btn:SetClientUserData( 0, mapid )
	btn:SetClientUserData( 1, x )
	btn:SetClientUserData( 2, y )
	btn:Show();
	if t_ItemDesc[itemid] ~= nil then
		OpenItemMapFrameDescRich:SetText(t_ItemDesc[itemid],255,255,190);
	else
		OpenItemMapFrameDescRich:SetText("",255,255,190);
	end
	
end

function OpenItemMapFrame_WaJueBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local itemid = OpenItemMapFrame:GetClientUserData( 0 );
	local item = container:getItem( itemid );
	if item:getItemId() > 0 then
		container:usedItem( item:getListType(), item:getGridIdx(), 0, 0 );
	end
	--OpenItemMapFrame:Hide();
end