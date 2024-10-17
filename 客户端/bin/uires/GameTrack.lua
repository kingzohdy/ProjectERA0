local t_trackRoleControl = { roleQueue = {}, curWatchName = "", lastReqTime = 0, TWO_WATCH_DELTA_TIME = 10, btnName = "", curClickRequireName = "" };
CLICK_REQUIRE		= 0;
TEN_SECOND_REQUIRE	= 1;	
local ZHUI_ZONG_ITEM_ID = 5030014;
function GetTrackRoleControl()
	return t_trackRoleControl;
end

t_trackRoleControl["init"] = 
function ( self )
	self["roleQueue"]			= {};
	self["curWatchName"]		= "";
	self["lastReqTime"]			= 0;
	self["curClickRequireName"]	= "";
	self["btnName"]				= "";
end

t_trackRoleControl["isZhuiZongItem"] = 
function ( self, data )
	local t_zhuiZongItems = { 5030014, };
	for _, id in ipairs( t_zhuiZongItems ) do
		if data["id"] == id then
			return true;
		end
	end

	return false;
end

t_trackRoleControl["getAllTrackRoles"] = 
function ( self )
	return self["roleQueue"];
end

t_trackRoleControl["getAllTrackRolesNum"] =
function ( self )
	local nNum = 0;
	for _, _ in pairs( self["roleQueue"] ) do
		nNum = nNum + 1;
	end

	return nNum;
end

t_trackRoleControl["addTrackRole"] = 
function ( self, data )
	self["roleQueue"][data["name"]] = { endTime = data["endTime"], map = data["map"], x = data["x"], y = data["y"], referMap = data["referMap"],
										lastReqTime = os.clock() };
end

t_trackRoleControl["synchronizeTrackRole"] = 
function ( self )
	local bDeleteOne = false;
	for name, data in pairs( self["roleQueue"] ) do
		local friendInfo = GameFriendManager:getFriendInfoByName( CLIENT_RELATION_TYPE_SU_DI, name );
		if friendInfo == nil then
			self["roleQueue"][name] = nil;
			bDeleteOne = true;
		end
	end

	if bDeleteOne then
		UpdateSuDiTrackTime();
	end
end

t_trackRoleControl["removeTrackRole"] = 
function ( self, data )
	local name = data["name"];
	self["roleQueue"][name] = nil;
end

t_trackRoleControl["beginTrack"] = 
function ( self, data )
	local szRoleName		= data["name"];
	local character			= GameFriendManager:getFriendInfoByName( CLIENT_RELATION_TYPE_SU_DI, szRoleName );
	if character ~= nil and character.m_nIsOnline ~= CLIENT_ONLINE then
		ShowMidTips( "��Ҳ����ߣ�����׷��" );
		return;
	end
	
	self["btnName"]				= data["btnName"];
	self["curClickRequireName"] = szRoleName;
	-- �����Ѿ���׷�ٵ������
	GameFriendManager:track( CLICK_REQUIRE, szRoleName );
end

t_trackRoleControl["clearCurWatchInfo"] =
function ( self, data )
	self["lastReqTime"]		= 0;
	self["curWatchName"]	= "";
	self["endTime"]			= 0;
end

t_trackRoleControl["updateWatchTime"] =
function ( self )
	self["lastReqTime"] = os.clock();
end

t_trackRoleControl["setWatchRole"] =
function ( self, data )
	self["curWatchName"]	= data["name"];
	self["endTime"]			= data["endTime"];
end

t_trackRoleControl["update"] =
function ( self )
	local nCurServeTime = getServeSecondTime();
	--print( "nCurServeTime = "..nCurServeTime.."�� servetime--"..os.date("%c", nCurServeTime ) );
	local bDeleteOne = false;
	for name, data in pairs( self["roleQueue"] ) do
		-- ����ʱ��
		if data["endTime"] <= nCurServeTime then
			if name == self["curWatchName"] then
				GameTrackSuDiFrame:Hide();
			end
			bDeleteOne = true;
			self["roleQueue"][name] = nil;
			ShowMidTips( "�����"..name.."��׷���ѳ�����Ч��" );
		end
	end
	
	--if bDeleteOne then
	UpdateSuDiTrackTime();
	--end
	
	local nCurTime = os.clock();
	for name, data in pairs( self["roleQueue"] ) do
		if nCurTime - data["lastReqTime"] >= self["TWO_WATCH_DELTA_TIME"] then
			GameFriendManager:track( TEN_SECOND_REQUIRE, name );
			data["lastReqTime"] = nCurTime;
		end
	end
end

t_trackRoleControl["updateCurWatchRole"] =
function ( self )
	if self["curWatchName"] == "" then
		return;
	end
	
	local nCurTime = os.clock();
	if nCurTime - self["lastReqTime"] >= self["TWO_WATCH_DELTA_TIME"] then
		GameFriendManager:track( TEN_SECOND_REQUIRE, self["curWatchName"] );
		t_trackRoleControl:updateWatchTime();
	end
end

function initTrackInfo()
	t_trackRoleControl:init();
end

local t_GameTrackSuDiFrameEvents = 
{ 
	["GE_GET_TRACKPLAYER_INFO"] = {},	["GE_FRIEND_GETLIST"] = {},
};

t_GameTrackSuDiFrameEvents["GE_GET_TRACKPLAYER_INFO"].func = 
function ()	
	local uiMsg			= UIMSG:getUIMsg();
	local trackRoleInfo	= uiMsg.GPRSData;
	local nRealMapID	= TeamManager:getMapDefID( trackRoleInfo.Map );
	local mapDef		= getMapDef( nRealMapID );
	t_trackRoleControl:addTrackRole( {	name = trackRoleInfo.RoleName, endTime = trackRoleInfo.GPRSEnd, 
										map = nRealMapID, referMap = mapDef.ReferMapID,
										x = trackRoleInfo.X, y = trackRoleInfo.Y 
									} 
									);
	
	-- ���б�����������֣����������ʾ������������Ϊ��ǰ�۲��������
	if trackRoleInfo.RoleName == t_trackRoleControl["curClickRequireName"] then
		t_trackRoleControl:setWatchRole( { name = trackRoleInfo.RoleName, endTime = trackRoleInfo.GPRSEnd } );
		t_trackRoleControl:updateWatchTime();
		if GameTrackSuDiFrame:IsShown() then
			UpdateGameTrackSuDiFrame();
		else
			GameTrackSuDiFrame:Show();
		end
		t_trackRoleControl["curClickRequireName"] = "";
	-- �����Ѿ���ʾ�Ź۲���壬����µ�ǰ���ڹ۲�����
	elseif t_trackRoleControl["curWatchName"] == trackRoleInfo.RoleName then
		t_trackRoleControl:updateWatchTime();
		if GameTrackSuDiFrame:IsShown() then
			UpdateGameTrackSuDiFrame();
		else
			GameTrackSuDiFrame:Show();
		end
	end
	
	UpdateSuDiTrackTime();
end

function KillPlayer( id )
	local actor		= ActorMgr:FindActor( id );
	local killName	= actor:getName();
	local bDeleteOne = false;
	for name, data in pairs( t_trackRoleControl["roleQueue"] ) do
		if name == killName then
			local roleInfo = GameFriendManager:getFriendInfoByName( CLIENT_RELATION_TYPE_SU_DI, name );
			if roleInfo ~= nil then
				--GameFriendManager:delRelation( CLIENT_RELATION_TYPE_SU_DI, roleInfo.m_RoleID.nHeight, roleInfo.m_RoleID.nLow );
				t_trackRoleControl["roleQueue"][name] = nil;
				bDeleteOne = true;
			end
		end
	end

	if killName == t_trackRoleControl["curWatchName"] then
		t_trackRoleControl:clearCurWatchInfo();
		GameTrackSuDiFrame:Hide();
	end
	
	if bDeleteOne then
		UpdateSuDiTrackTime();
	end
end

function OffLine( szOffRoleName )
	local bDeleteOne = false;
	for name, data in pairs( t_trackRoleControl["roleQueue"] ) do
		if name == szOffRoleName then
			local roleInfo = GameFriendManager:getFriendInfoByName( CLIENT_RELATION_TYPE_SU_DI, name );
			if roleInfo ~= nil then
				t_trackRoleControl["roleQueue"][name] = nil;
				bDeleteOne = true;
			end
		end
	end

	if szOffRoleName == t_trackRoleControl["curWatchName"] then
		t_trackRoleControl:clearCurWatchInfo();
		GameTrackSuDiFrame:Hide();
		ShowMidTips( "���"..szOffRoleName.."���ߣ�ֹͣ׷��" );
	end

	if bDeleteOne then
		UpdateSuDiTrackTime();
	end
end

t_GameTrackSuDiFrameEvents["GE_FRIEND_GETLIST"].func = 
function ()
	t_trackRoleControl:synchronizeTrackRole();

	-- �����ڹ۲�����Ѿ����޵��б�ɾ�������ܼ����۲�����
	if t_trackRoleControl["curWatchName"] == "" then
		return;
	end

	-- TODO: �ڼӸ�����ʱ�޺����
	local suDiInfo = GameFriendManager:getFriendInfoByName( CLIENT_RELATION_TYPE_SU_DI, t_trackRoleControl["curWatchName"] );
	if suDiInfo == nil then
		t_trackRoleControl:clearCurWatchInfo();
		GameTrackSuDiFrame:Hide();
	end
end

function GameTrackSuDiFrame_OnLoad()
	for event, _ in pairs( t_GameTrackSuDiFrameEvents ) do
		this:RegisterEvent( event );
	end

	t_GameTrackSuDiFrameEvents.__index = function ( tab, key )
									tab[key] = { func = function () end };
									return tab[key];
								end
	setmetatable( t_GameTrackSuDiFrameEvents, t_GameTrackSuDiFrameEvents );
end

function GameTrackSuDiFrame_OnEvent()
	t_GameTrackSuDiFrameEvents[arg1].func();
end

function UpdateGameTrackSuDiFrame()
	local uiMsg			= UIMSG:getUIMsg();
	local trackRoleInfo	= uiMsg.GPRSData;
	local t_fonts = 
	{	["GameTrackSuDiFrameNameBtnFont"]		= { value = trackRoleInfo.RoleName },
		["GameTrackSuDiFrameLevelBtnFont"]		= { value = trackRoleInfo.Level.."��" },-- ֵ��0
		["GameTrackSuDiFrameClanBtnFont"]		= { value = trackRoleInfo.ClanName == "" and "δ���빫��" or trackRoleInfo.ClanName },
		["GameTrackSuDiFrameSceneBtnFont"]		= { value = MapShower:GetMinMapContal():getMapName( TeamManager:getMapDefID( trackRoleInfo.Map ) ), },
		["GameTrackSuDiFrameCoordBtnFont"]		= { value = math.ceil( trackRoleInfo.X / 100 ).."��"..math.ceil( trackRoleInfo.Y / 100 ) },
		["GameTrackSuDiFrameIsInTeamBtnFont"]	= { value = trackRoleInfo.Team == 0 and "��" or "��" },
		["GameTrackSuDiFrameCareerBtnFont"]		= { value = t_allCareerDesc[trackRoleInfo.Career]["desc"] },
	};
	util.SetFonts( t_fonts );
	
	SetPhoto( trackRoleInfo.Hair, trackRoleInfo.Face, "GameTrackSuDiFramePhotoBtnHairTex",
				"GameTrackSuDiFramePhotoBtnFaceTex", trackRoleInfo.Gender, trackRoleInfo.Head,
				"GameTrackSuDiFramePhotoBtnHairShadowTex", "GameTrackSuDiFramePhotoBtnBodyTex" );
end

function GameTrackSuDiFrame_OnShow()
	UpdateGameTrackSuDiFrame();
	util.AdjustBigtipsScreenPos( { tipsFrame = this:GetName(), button = t_trackRoleControl["btnName"] } );
end

function GameTrackSuDiFrame_OnHide()
	t_trackRoleControl:clearCurWatchInfo();
end

function GameTrackSuDiFrame_OnUpdate()
	t_trackRoleControl:updateCurWatchRole();
end
