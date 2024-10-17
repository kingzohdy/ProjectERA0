--�״���NPCλ��button�ؼ���Client����˵��
--ClientUserData[0] ->NpcId
--ClientUserData[1] ->�ڳ���X����
--ClientUserData[2] ->�ڳ���Y����
--ClientUserString  ->NPC����

local MAX_NPC_BTN		= 42
local MAX_TEAM_MEMBER	= 6

local LAST_NPC_BTN_USE			= 0
local LAST_MONSTER_BTN_USE		= 0
local LAST_IMPORTANT_BTN_USE	= 0
local LAST_TEAM_MEMBER_BTN_USE	= 0
local LAST_MINIMAP_SUDI_BTN_USE	= 0;
local LAST_ORE_BTN_USE = 0;
local LAST_BIG_ORE_BTN_USE = 0;
local LAST_PWORLD_BOSS_USE = 0;
local mapType	=	1;
local pworldDifficult = {
	"��",
	"��ͨ",
	"����",
}
-- ���֮��
--[[
#L#c42a83d�������@@2843003#n  #L#c42a83dС����@@5050000#n  #L#c42a83d�ݷ�����@@2813004#n  #L#c42a83d�����ۼ�@@2723004#n  #L#c42a83d���Ǵ���@@2813005#n  
#L#c42a83d����֮��@@2733005#n
--]]

local g_ImportantMinimap

local g_MachineWarBossMinimap = {}			--���ʱ����Ӫ����λ��
local g_ClanWarCampMonMinimap = {}						--�������ս��Ŧ

-- ��2����ͼID������:	1.���鷢����mapID����RealMapId��2.ReferMapId��Ϊ0��ʱ��,ȡ��ͼͼƬ��ID��g_nReferMapIdΪ�ؼ�֮����ȡ��ͼͼƬ,
--						��g_nReferMapIdΪ0��ʱ��ȥ��ͼ��Ӧ������g_nRealMapId����ȡ
local g_nReferMapId		= 0;
local g_nRealMapId		= 0;

local t_MinMapNpcInfo	= { };

local nMouseClickPosX = 0;
local nMouseClickPosY = 0;
-- 842
t_specialMapDesc =	{	[2100] = { mapValidNormalizeTextureWidth = 1024, mapValidNormalizeTextureHeight = 842,	leftOffset = 0,	topOffset = 686,
											completeTextureWidth = 2048, completeTextureHeight = 2048, clicpTextureWidth = 1658,
											clicpTextureHeight = 1658 },
						[2500] = { mapValidNormalizeTextureWidth = 1024, mapValidNormalizeTextureHeight = 1024,	leftOffset = 90, topOffset = 335,
											completeTextureWidth = 1600, completeTextureHeight = 1600, clicpTextureWidth = 1024,
											clicpTextureHeight = 1024 },

						[3304] = { mapValidNormalizeTextureWidth = 1024, mapValidNormalizeTextureHeight = 1024,	leftOffset = 221, topOffset = 0,
											completeTextureWidth = 1024, completeTextureHeight = 1024, clicpTextureWidth = 600,
											clicpTextureHeight = 600 },
						
							};
local szLastAreaType = "";

------------------------------��ͼ��Ϣ-------------------------------
local g_nViewWidth			= 140;
local g_nViewHeight			= 140;
local g_nMapTextureWidth	= 1024;
local g_nMapTextureHeight	= 1024;
local g_fZoom				= 0.6;
local g_fMinZoom			= 0.4;
local g_szViewPortName		= "MinimapEnvelopTextureMap";--�ӿ�����

local nOutside = 0;  --������״��Բ��
local nInner   = 1;  --������״��Բ��

local nMouseClickLastTime = 0;
local bOnClick = false;

local nearNpc = {}
local nearNpcByResId = {}
local nearMonster = {}
nearPlayer = {}
local mapStaticNpc = {}
local t_MoveNpc = {}

t_MapNpcSize = { ["Bao"] = 19, ["BaoShi"] = 32, ["Biao"] = 19, ["Cang"] = 19, ["Chong"] = 19, ["Chuan"] = 19, ["DaDi"] = 32,
				 ["DiXin"] = 32, ["Duan"] = 18, ["Fei"] = 19, ["Fu"] = 19, ["FuMo"] = 32, ["HaiTan"] = 32, ["HuanJing"] = 32,
				 ["Hui"] = 19, ["Ji"] = 19, ["JiaNianHua"] = 45, ["Lian"] = 19, ["LianJin"] = 32, ["LongDan"] = 32, ["PanShan"] = 32,
				 ["Qiang"] = 19, ["Shang"] = 19, ["ShangYun"] = 32, ["TaiYang"] = 32, ["TieYi"] = 32, ["Yu"] = 19, ["Za"] = 19, 
				 ["Zhan"] = 19, ["Zhu"] = 19, ["ZhuZhao"] = 32, ["Taiyang"] = 32,	["ShangRen"] = 32 }

function ActorMgrs()
	return ActorMgr.getNextActor, ActorMgr
end

function AddNearNpc(id, resid)
	resid = resid or 0
	if mapStaticNpc[resid] == nil then
		local MinMapContal	= MapShower:GetMinMapContal();
		local npcInfo = MapShower:getNpcInfoById(MinMapContal:getMapID(), resid);
		nearNpc[id] = {npcInfo};
	end
	if nearNpcByResId[resid] then
		nearNpcByResId[resid] = nearNpcByResId[resid] + 1
	else
		nearNpcByResId[resid] = 1
	end
end

function RemoveNearNpc(id,resid)
	if nearNpcByResId[resid] and nearNpcByResId[resid] > 0 then
		nearNpcByResId[resid] = nearNpcByResId[resid] - 1
		if nearNpcByResId[resid] == 0 then
			nearNpcByResId[resid] = nil
		end
		nearNpc[id] = nil
	end
end

function ClearNearNpc()
	nearNpc = {}
	nearNpcByResId = {}
end

function AddNearMonster(id)
	nearMonster[id] = true
end

function RemoveNearMonster(id)
	if nearMonster[id] then
		nearMonster[id] = nil
	end
end

function ClearNearMonster()
	nearMonster = {}
end

function AddNearPlayer(id)
	nearPlayer[id] = true
end

function RemoveNearPlayer(id)
	if nearPlayer[id] then
		nearPlayer[id] = nil
	end
end

function ClearNearPlayer()
	nearPlayer = {}
end

function MinimapFrameButton_OnEnter()
	--local szText = "#001#002#003#004#005#006#007#008#009#010#011#012#013#014#015#016";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text =  this:GetClientString(), frame = this:GetParent(),
						button = this:GetName() } );
end

function MinimapFrameButton_OnLeave()
	GameTooltip:Hide();
end

function MinimapFrame_OnLoad()
	this:RegisterEvent( "GE_ENTER_MAP" );
	this:RegisterEvent( "GE_ENTER_PWORLD" );
	this:RegisterEvent( "GE_REENTER_PWORLD" );
	this:RegisterEvent( "GE_ENTER_PLAYERLOGIN" ); 
	this:RegisterEvent( "GE_MINMAPNPC_LEAVE" );
	this:RegisterEvent( "GE_UPDATE_NPC" );
	this:RegisterEvent( "UI_ACTOR_LEVEL_CHG" );
	local t_buttonInit = 
						{
					--	["NetShowBtn"]		= { content = "����״��������", },
						["WarForcesBtn"]	= { content = "ս������ͼ", },
						["HelpShowBtn"]		= { content = "�ɳ�֮· ( ��ݼ�I )", },	
						["ActivityShowBtn"] = { content = "����� ( ��ݼ�H )", },	
						["MarketBtn"]		= { content = "�̳� ( ��ݼ�V )", },
						["TradeBtn"]		= { content = "��ҽ���( ��ݼ�P )", },	
						["RangeBtn"]		= { content = "���а�", },
						["AreaMapShowBtn"]	= { content = "�����ͼ ( ��ݼ�M )", },
						["NearBtn"]			= { content = "BUG����", },		-- "��Χ����б� ( ��ݼ�T )"
						["TrustBtn"]		= { content = "ί��", },
						["MailBtn"]			= { content = "�ʼ� (��ݼ�Y)", },
						["ZoomInBtn"]		= { content = "�Ŵ�", },
						["ZoomOutBtn"]		= { content = "��С", },
						["PointNorthButton"]= { content = "ָ��", },
					--	["DpsBtn"]			= { content = "�˺�ͳ��" , },
						["AutoTeamBtn"]		= { content = "��������" , },
						["AutoAssistBtn"]         = { content = "�Զ���������ݼ�ctrl + z��",},
						};

	for name, data in pairs( t_buttonInit ) do
		local btn =getglobal( name );
		btn:SetClientString( data.content );
	end
	MiniMapShowAttackPosBtn:setUpdateTime( 0 );
	MiniMapShowVenuePosBtn:setUpdateTime( 0 );
end

-- ���ֳ�����ȫ���͵ı�׼
-- MapLimit_PK = 0, MapLimit_PKMode =  0 ��ȫ
-- MapLimit_PK = 1, MapLimit_PKMode =  0 ����
-- ����ս������

local t_areaType =	{	["��ȫ"] = { color = { r = 135, g = 255, b = 46 },},
						["����"] = { color = { r = 255, g = 252, b = 89 },},
						["ս��"] = { color = { r = 255, g = 0, b = 0 },},
						["����PK"] = { color = { r = 255, g = 0, b = 0 },},
					};

function InitMapStaticNpc()
	mapStaticNpc = {}
	local MinMapContal	= MapShower:GetMinMapContal();
	for i = 1, MapShower:getMapNpcCount( MinMapContal:getMapID() ) do
		local npcInfo		= MapShower:getMapNpcInfo(MinMapContal:getMapID(), i - 1)
		mapStaticNpc[ npcInfo.m_nId ] = npcInfo;
	end
end

function ReSetMapNpc()
	InitMapStaticNpc();
	local MinMapContal	= MapShower:GetMinMapContal();		
	for npcId,_ in pairs( nearNpc ) do
		local actor = ActorMgr:FindActor(npcId);
		if actor == nil then
			nearNpc[npcId] = nil;
		else
			nearNpc[npcId] = { MapShower:getNpcInfoById(MinMapContal:getMapID(), actor:getResId()) }
		end
	end
end

function AddMiniMapMoveNpc( pNpc )
	local npcId = pNpc:GetID();
	t_MoveNpc[npcId] = pNpc;
end

function MinimapFrame_OnEvent()
	if arg1 == "GE_ENTER_PLAYERLOGIN" then
		ClearImportMinimapIcon()
	end
	if arg1 == "GE_ENTER_MAP" or arg1 == "GE_ENTER_PLAYERLOGIN" then
		local MinMapContal	= MapShower:GetMinMapContal();
		local mainplayer = ActorMgr:getMainPlayer();
		if MinMapContal:getMapID() == 1000 and mainplayer:getLv() >= 10 and Quest:isQuestRun( 1077 ) and 
			not GameWizard:isDo(GAMEWIZARD_FIRST_TALK_ZHUCHENG_TRAN ) then
				GameWizard:doStep( GAMEWIZARD_FIRST_TALK_ZHUCHENG_TRAN );
		end

		if BIAO_JI_STALL_MAP ~= MinMapContal:getMapID() then
			local t_StallBuyFrameBiaoJiStalls = GetStallBuyFrameBiaoJiStalls();
			t_StallBuyFrameBiaoJiStalls:clearAllBiaoJi();
		end
		-- ������ڻ�ȡ��ͼ,RealMapID���������𸱱�������ͼ,������������ͼ����һ����ͼ
		g_nReferMapId	= MinMapContal:getMapID();
		g_nRealMapId	= MapShower:getRealMapId();
		local mapDef = getMapDef( g_nReferMapId );
		local szAreaType = "��ȫ";
		if mapDef.MapLimit.PK == 0 and mapDef.MapLimit.PKMode == 0 then
			szAreaType = "��ȫ";
		elseif mapDef.MapLimit.PK == 1 and mapDef.MapLimit.PKMode == 0 and not bitAnd(mapDef.CtrlFlag,128) then
			szAreaType = "����";
		else
			szAreaType = "ս��";
		end
		local t_fonts = { ["MMMapName"] = { value = MinMapContal:getMapName( g_nRealMapId ), color = t_areaType[szAreaType]["color"] }, };
		util.SetFonts( t_fonts );
		CheckSafeAreaTips( szAreaType );
		MinMapContal:setView( g_nViewWidth, g_nViewHeight );
		--g_fZoom	= 0.6;
		MinMapContal:setMapZoom( g_fZoom );
		local nTexID = 0;
		if g_nReferMapId == 0 then
			nTexID = g_nRealMapId;
		else
			nTexID = g_nReferMapId;
		end

		if t_specialMapDesc[nTexID] ~= nil then
			MinMapContal:setMapTextRect( t_specialMapDesc[nTexID].mapValidNormalizeTextureWidth, t_specialMapDesc[nTexID].mapValidNormalizeTextureHeight,
											t_specialMapDesc[nTexID].leftOffset, t_specialMapDesc[nTexID].topOffset,
											t_specialMapDesc[nTexID].completeTextureWidth, t_specialMapDesc[nTexID].completeTextureHeight,
											t_specialMapDesc[nTexID].clicpTextureWidth, t_specialMapDesc[nTexID].clicpTextureHeight,
											g_nViewWidth, g_nViewHeight );

		else
			MinMapContal:setMapTextRect( g_nMapTextureWidth, g_nMapTextureHeight, 0, 0, g_nMapTextureWidth, g_nMapTextureHeight,
											g_nMapTextureWidth, g_nMapTextureHeight, g_nViewWidth, g_nViewHeight  );
		end
		
		if math.floor(nTexID  / 100) == 300 then
			nTexID = 30000
		end
		local tex = "uires\\DiTuLei\\QuYuDiTu\\"..nTexID..".dds";
		MinimapEnvelopTextureMap:SetTexture( tex );
		MinimapEnvelopTextureMap:setMask("uires\\ZhuJieMian\\XiaoDiTu\\XiaoDiTu\\mask2.tga");
		MinMapContal:updateMapInfo();
		InitMapStaticNpc();
		t_MoveNpc = {};
		this:setUpdateTime( 0 );
		
		local t_AutoRoundWorlldControl = GetAutoRoundWorlldControl();
		t_AutoRoundWorlldControl:changeMap();
		
		if arg1 == "GE_ENTER_PLAYERLOGIN" then
			HelpShowBtn:Show();
			AutoAssistBtn:Show();
			ActivityShowBtn:Show();
			AutoTeamBtn:Show();
			BattleOpenBtn:Show();
			if mainplayer:getLv() < 40 then
				BattleOpenBtn:Hide();
				if mainplayer:getLv() < 28 then
					AutoTeamBtn:Hide();
					if mainplayer:getLv() < 15 then
						ActivityShowBtn:Hide();
						if mainplayer:getLv() < 11 then
							AutoAssistBtn:Hide();
							if mainplayer:getLv() < 10 then
								HelpShowBtn:Hide();			
							end
						end
					end
				end
			end
		end
	end
	if arg1 == "GE_UPDATE_NPC" then
		ReSetMapNpc();
	end
	if arg1 == "UI_ACTOR_LEVEL_CHG" then
		GetFirstPKTips();
		HelpShowBtn:Show();
		AutoAssistBtn:Show();
		ActivityShowBtn:Show();
		AutoTeamBtn:Show();
		BattleOpenBtn:Show();
		local mainplayer = ActorMgr:getMainPlayer();		

		if mainplayer:getLv() < 40 then
			BattleOpenBtn:Hide();
			if mainplayer:getLv() == 28 then
				local szText = "�����������ͣ��Ͷ���һ����븱��";
				local xinShouBtn = getglobal( "MiniMapXinShouBtn" );
				local rich	 = getglobal( xinShouBtn:GetName() .."Rich" );
				local nSelfDefineExtendWidth = 14;
				rich:SetClientString( szText );
				xinShouBtn:SetPoint( "topleft", "AutoTeamBtn", "topleft", -148, -17 );
				ReSizeTooltip( rich:GetName(), xinShouBtn:GetName(), nSelfDefineExtendWidth );
				xinShouBtn:Show();
			end
			if mainplayer:getLv() < 28 then
				AutoTeamBtn:Hide();
				if mainplayer:getLv() < 15 then
					ActivityShowBtn:Hide();
					if mainplayer:getLv() == 11 then
						local szText = "�����Զ���������ʼ�Զ�ս��";
						local xinShouBtn = getglobal( "MiniMapXinShouBtn" );
						local rich	 = getglobal( xinShouBtn:GetName() .."Rich" );
						local nSelfDefineExtendWidth = 14;
						rich:SetClientString( szText );
						xinShouBtn:SetPoint( "topleft", "AutoAssistBtn", "topleft", -148, -7 );
						ReSizeTooltip( rich:GetName(), xinShouBtn:GetName(), nSelfDefineExtendWidth );
						xinShouBtn:Show();
					end
					if mainplayer:getLv() < 11 then
						AutoAssistBtn:Hide();
						if mainplayer:getLv() == 10 then
							local szText = "�����ɳ�֮·���鿴��Ϸ����";
							local xinShouBtn = getglobal( "MiniMapXinShouBtn" );
							local rich	 = getglobal( xinShouBtn:GetName() .."Rich" );
							local nSelfDefineExtendWidth = 14;
							rich:SetClientString( szText );
							xinShouBtn:SetPoint( "topleft", "HelpShowBtn", "topleft", -148, -7 );
							ReSizeTooltip( rich:GetName(), xinShouBtn:GetName(), nSelfDefineExtendWidth );
							xinShouBtn:Show();
						end
						if mainplayer:getLv() < 10 then
							HelpShowBtn:Hide();			
						end
					end
				end
			end
		end

		if mainplayer:getLv() == 40 then
			local szText = "����ս������������̼���ս����ս";
			local xinShouBtn = getglobal( "MiniMapXinShouBtn" );
			local rich	 = getglobal( xinShouBtn:GetName() .."Rich" );
			local nSelfDefineExtendWidth = 14;
			rich:SetClientString( szText );
			xinShouBtn:SetPoint( "topleft", "BattleOpenBtn", "topleft", -148, -10 );
			ReSizeTooltip( rich:GetName(), xinShouBtn:GetName(), nSelfDefineExtendWidth );
			xinShouBtn:Show();
		end
		if mainplayer:getLv() == 60 then
			local szText = "����μ�\nĩ����ҫ����";
			local xinShouBtn = getglobal( "MiniMapXinShouBtn" );
			local rich	 = getglobal( xinShouBtn:GetName() .."Rich" );
			local nSelfDefineExtendWidth = 14;
			rich:SetClientString( szText );
			xinShouBtn:SetPoint( "topleft", "BattleOpenBtn", "topleft", -105, -10 );
			ReSizeTooltip( rich:GetName(), xinShouBtn:GetName(), nSelfDefineExtendWidth );
			xinShouBtn:Show();
		end
	end
	
end

local timeTick = 0;
function MiniMapXinShouBtn_OnShow()
	timeTick = 0;
end

function MiniMapXinShouBtn_OnUpdate()
	if timeTick == 0 then
		timeTick = os.clock();
	elseif os.clock() - timeTick > 10 then
		this:Hide();
	end
end

--@Desc:���ʼ�
local nLastTime = 0;
local t_mainBtnHighlightTime = { startTime = 0, getNewMail = false };

function initMinimapData()
	t_mainBtnHighlightTime = { startTime = 0, getNewMail = false };
	MailBtn:DisHighlight();
	MailBtn:SetClientString("�ʼ� (��ݼ�Y)");
	szLastAreaType = "";
	MiniMapDrawLineFriendLine:ClearDrawPointList();
	MiniMapDrawLineEnemyLine:ClearDrawPointList();
	MapBigOres = {}
	tWeatherData = nil
	local uvTex = getglobal("MailBtnUVAnimationTex");
	uvTex:Hide();
end

function ResetMiniMapData()
	t_mainBtnHighlightTime = { startTime = 0, getNewMail = false };
	MailBtn:DisHighlight();
	local key = GetKeyMap("������")
	if key == 0 then
		key = nil
	end
	if key then
		MailBtn:SetClientString( string.format( "�ʼ� (��ݼ�%s)", KeyToString(key) ) );
	else
		MailBtn:SetClientString( "�ʼ�");
	end	
	local uvTex = getglobal("MailBtnUVAnimationTex");
	uvTex:Hide();
end

function MailBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer:isOutSide() then
		ShowMidTips( "���״̬�޷�ʹ���ʼ�ϵͳ" )
		return
	end
	local t_frames = { "MailHeadFrame", "MailSendItemFrame" };
	for _, name in ipairs( t_frames ) do
		local frame = getglobal( name );
		if frame:IsShown() then
			frame:Hide();
			return;
		end
	end

	local defaultShowFrame = getglobal( "MailHeadFrame" );
	defaultShowFrame:Show();
	t_mainBtnHighlightTime["getNewMail"] = false;
	t_mainBtnHighlightTime["startTime"]	 = 0;
	MailBtn:SetClientString("�ʼ� (��ݼ�Y)");
	if MailBtn:IsHighlight() then
		MailBtn:DisHighlight();
	end
	local uvTex = getglobal("MailBtnUVAnimationTex");
	uvTex:Hide();
end

function MailBtn_OnLoad()
	this:RegisterEvent("GE_MAIL_NEWMAIL");
end

function MailBtn_OnEvent()
	if arg1 == "GE_MAIL_NEWMAIL" then
		ShowMidTips( "����һ�����ʼ�" );
		SetSystemMessage( "����һ�����ʼ�" );
		if not MailHeadFrame:IsShown() then
			SetMailBtnHighlight();
			MailBtn:SetClientString("����һ�����ʼ�����ע�����");
		end
	end
end

function MailBtn_OnUpdate()
	if not t_mainBtnHighlightTime["getNewMail"] then
		return;
	end
	
	if  GameMgr:getTickTime() - t_mainBtnHighlightTime["startTime"] < 500 then
		return;
	end
	
	t_mainBtnHighlightTime["startTime"] = GameMgr:getTickTime();
	local btn = getglobal( "MailBtn" );
	if btn:IsHighlight() then
		btn:DisHighlight();
	else
		btn:Highlight();
	end
end

function SetMailBtnHighlight()
	local uvTex = getglobal("MailBtnUVAnimationTex");
	uvTex:SetUVAnimation(80, true)
	uvTex:Show();
	t_mainBtnHighlightTime["getNewMail"]	= true;
	t_mainBtnHighlightTime["startTime"]		= GameMgr:getTickTime();
end

local nTargetTexNum = 1;
function MinimapSelectTargetPosBtn_OnUpdate()
	if nTargetTexNum == 1 then
		SelectTargetPosTexture:SetTexture("uires\\ZhuJieMian\\XiaoDiTu\\XiaoDiTu\\DuiYuanTeXiaoZhenDongHua\\1.tga");
		nTargetTexNum = 2;
	elseif  nTargetTexNum == 2 then
		SelectTargetPosTexture:SetTexture("uires\\ZhuJieMian\\XiaoDiTu\\XiaoDiTu\\DuiYuanTeXiaoZhenDongHua\\2.tga");
		nTargetTexNum = 3;
	elseif nTargetTexNum == 3 then
		SelectTargetPosTexture:SetTexture("uires\\ZhuJieMian\\XiaoDiTu\\XiaoDiTu\\DuiYuanTeXiaoZhenDongHua\\3.tga");
		nTargetTexNum = 4;
	elseif nTargetTexNum == 4 then
		SelectTargetPosTexture:SetTexture("uires\\ZhuJieMian\\XiaoDiTu\\XiaoDiTu\\DuiYuanTeXiaoZhenDongHua\\4.tga");
		nTargetTexNum = 1;
	end
end

function UpdateMinimapNpcInfo( MinMapContal )
	local MapInfo = MinMapContal:updateMapInfo()
	local btnIndex = 1
	local btn
	-- ����NPC�����ʾ���
	local startX = MapInfo.nMapPosStartX * MapInfo.fZoom
	local startY = MapInfo.nMapPosStartY * MapInfo.fZoom

	for npcId,t_Info in pairs(nearNpc) do
		local npcInfo = t_Info[0];
		btn = getglobal("MinimapNpcPosBtn" .. btnIndex)
		if not btn then
			break
		end
		local npc = ActorMgr:FindActor(npcId);
		if npc then
			local npcResId = npc:getResId()
			local x = npc:getPlayerX() / 10
			local y = npc:getPlayerZ() / 10
			local npcTexCoord		= MinMapContal:getPointCoordOnTex( x, y )
			local nX = math.floor(math.floor(npcTexCoord.first * MapInfo.fZoom) - startX)
			local nY = math.floor(math.floor(npcTexCoord.second * MapInfo.fZoom) - startY)
			--local npcInfo = MapShower:getNpcInfoById( MinMapContal:getMapID(), npcResId )

			if GetDistance( nX, nY ) == nInner and (not npcInfo or npcInfo.m_bShow) then
				local btnTaskCommitTex	= getglobal( "MinimapNpcPosBtn"..btnIndex.."TaskNpcTex" )
				local btnNpcTypeTex		= getglobal( "MinimapNpcPosBtn"..btnIndex.."NpcTypeTex" )
				local nNPCstate			= getNpcQuestShow( npcResId )
				if npcInfo and npcInfo.m_szMapDesc and npcInfo.m_szMapDesc ~= "" and IsInExistence( "uires\\TuPianLei\\NPC\\" .. npcInfo.m_szMapDesc .. ".tga" ) then
					btnNpcTypeTex:SetTextue( "uires\\TuPianLei\\NPC\\" .. npcInfo.m_szMapDesc .. ".tga" );
					btnNpcTypeTex:Show()
					if t_MapNpcSize ~= nil then
						btnNpcTypeTex:SetSize( t_MapNpcSize[npcInfo.m_szMapDesc],19 )
					else
						btnNpcTypeTex:SetSize( 32,19 )
					end
				else
					btnNpcTypeTex:Hide()
				end
				-- �����״�����NPC������ʾ������ͼ��
				if ( nNPCstate == CLIENT_NORMAL_TASK_STAT_CAN_ACCEPT ) then
					btnTaskCommitTex:SetTexUV( 508,45,17,22 )
					btnTaskCommitTex:SetSize( 17,22 )
					btnTaskCommitTex:Show()
				elseif ( nNPCstate == CLIENT_TASK_STAT_FINI ) then
					btnTaskCommitTex:SetTexUV( 472,45,17,22 )
					btnTaskCommitTex:SetSize( 17,22 )
					btnTaskCommitTex:Show()
				elseif ( nNPCstate == CLIENT_TASK_STAT_FAIL ) then
					btnTaskCommitTex:SetTexUV( 490,45,17,22 )
					btnTaskCommitTex:SetSize( 17,22 )
					btnTaskCommitTex:Show()
				elseif ( nNPCstate == CLIENT_RECYCLE_TASK_STAT_CAN_ACCEPT ) then
					btnTaskCommitTex:SetTexUV( 526,45,17,22 )
					btnTaskCommitTex:SetSize( 17,22 )
					btnTaskCommitTex:Show()
				elseif not btnNpcTypeTex:IsShown() then
					btnTaskCommitTex:SetTexUV( 390,51,17,16 )
					btnTaskCommitTex:SetSize( 15,15 )
					btnTaskCommitTex:Show()
				else
					btnTaskCommitTex:Hide()
				end

				btn:SetClientString(getNpcDesc(npcResId).." "..GetNpcName(npcResId))
				btn:SetClientUserData( 0, npcResId )
				btn:SetClientUserData( 1, x )
				btn:SetClientUserData( 2, y )
				btn:SetPoint( "center", "MinimapEnvelopTextureMap", "topleft", nX, nY )
				-- ��ʾ���״ﵱǰ��ʾ��Χ��NPC
				btn:Show()
				btnIndex = btnIndex + 1
			end
		end
	end
	for _, npcInfo in pairs(mapStaticNpc) do
		btn = getglobal("MinimapNpcPosBtn" .. btnIndex)
		if not btn then
			break
		end

		if nearNpcByResId[npcInfo.m_nId] then
			local npcTexCoord		= MinMapContal:getPointCoordOnTex( npcInfo.m_nRealX,npcInfo.m_nRealY )
			local nX = math.floor(math.floor(npcTexCoord.first * MapInfo.fZoom) - startX)
			local nY = math.floor(math.floor(npcTexCoord.second * MapInfo.fZoom) - startY)
			--local npcInfo = MapShower:getNpcInfoById( MinMapContal:getMapID(), v.resid )
			if GetDistance( nX, nY ) == nInner and npcInfo.m_bShow then
				local btnTaskCommitTex	= getglobal( "MinimapNpcPosBtn"..btnIndex.."TaskNpcTex" )
				local btnNpcTypeTex		= getglobal( "MinimapNpcPosBtn"..btnIndex.."NpcTypeTex" )
				local nNPCstate			= getNpcQuestShow( npcInfo.m_nId )
				if npcInfo and npcInfo.m_szMapDesc and npcInfo.m_szMapDesc ~= "" and IsInExistence( "uires\\TuPianLei\\NPC\\" .. npcInfo.m_szMapDesc .. ".tga" ) then
					btnNpcTypeTex:SetTexture( "uires\\TuPianLei\\NPC\\" .. npcInfo.m_szMapDesc .. ".tga" );
					btnNpcTypeTex:Show()					
					if t_MapNpcSize[npcInfo.m_szMapDesc] ~= nil then
						btnNpcTypeTex:SetSize( t_MapNpcSize[npcInfo.m_szMapDesc],19 )
					else
						btnNpcTypeTex:SetSize( 32,19 )
					end
				else
					btnNpcTypeTex:Hide()
				end
				-- �����״�����NPC������ʾ������ͼ��
				if ( nNPCstate == CLIENT_NORMAL_TASK_STAT_CAN_ACCEPT ) then
					btnTaskCommitTex:SetTexUV( 508,45,17,22 )
					btnTaskCommitTex:SetSize( 17,22 )
					btnTaskCommitTex:Show()
				elseif ( nNPCstate == CLIENT_TASK_STAT_FINI ) then
					btnTaskCommitTex:SetTexUV( 472,45,17,22 )
					btnTaskCommitTex:SetSize( 17,22 )
					btnTaskCommitTex:Show()
				elseif ( nNPCstate == CLIENT_TASK_STAT_FAIL ) then
					btnTaskCommitTex:SetTexUV( 490,45,17,22 )
					btnTaskCommitTex:SetSize( 17,22 )
					btnTaskCommitTex:Show()
				elseif ( nNPCstate == CLIENT_RECYCLE_TASK_STAT_CAN_ACCEPT ) then
					btnTaskCommitTex:SetTexUV( 526,45,17,22 )
					btnTaskCommitTex:SetSize( 17,22 )
					btnTaskCommitTex:Show()
				elseif not btnNpcTypeTex:IsShown() then
					btnTaskCommitTex:SetTexUV( 390,51,17,16 )
					btnTaskCommitTex:SetSize( 15,15 )
					btnTaskCommitTex:Show()
				else
					btnTaskCommitTex:Hide()
				end

				btn:SetClientString(getNpcDesc(npcInfo.m_nId).." "..GetNpcName(npcInfo.m_nId))
				btn:SetClientUserData( 0, npcInfo.m_nId )
				btn:SetClientUserData( 1, npcInfo.m_nRealX )
				btn:SetClientUserData( 2, npcInfo.m_nRealY )
				btn:SetPoint( "center", "MinimapEnvelopTextureMap", "topleft", nX, nY )
				-- ��ʾ���״ﵱǰ��ʾ��Χ��NPC
				btn:Show()
				btnIndex = btnIndex + 1
			end
		end
	end
	
	for _, pNpc in pairs(t_MoveNpc) do
		local x = pNpc:getPlayerX() / 10
		local y = pNpc:getPlayerZ() / 10
		local npcTexCoord = MinMapContal:getPointCoordOnTex( x, y )
		local nX = math.floor(math.floor(npcTexCoord.first * MapInfo.fZoom) - startX)
		local nY = math.floor(math.floor(npcTexCoord.second * MapInfo.fZoom) - startY)

		if GetDistance( nX, nY ) == nInner then
			local btnTaskCommitTex	= getglobal( "MinimapNpcPosBtn"..btnIndex.."TaskNpcTex" )
			local btnNpcTypeTex	= getglobal( "MinimapNpcPosBtn"..btnIndex.."NpcTypeTex" )
			btnNpcTypeTex:Hide()
			btnTaskCommitTex:SetTexUV( 390,51,17,16 )
			btnTaskCommitTex:SetSize( 15,15 )
			btnTaskCommitTex:Show()

			btn:SetClientString(pNpc:getName())
			btn:SetClientUserData( 0, 0 )
			btn:SetClientUserData( 1, x )
			btn:SetClientUserData( 2, y )
			btn:SetPoint( "center", "MinimapEnvelopTextureMap", "topleft", nX, nY )
			btn:Show();
			btnIndex = btnIndex + 1;
		end
	end

	for i = btnIndex, LAST_NPC_BTN_USE do
		btn = getglobal("MinimapNpcPosBtn" .. i)
		if btn then
			btn:Hide()
		end
	end
	LAST_NPC_BTN_USE = btnIndex - 1
end

local t_monsterCanShow =  
						{	[0] = { desc = "΢" }, [1] = { desc = "��" }, [2] = { desc = "��" }, [3] = { desc = "��" }, [4] = { desc = "Ӳ" },
							[5] = { desc = "ϡ" }, [6] = { desc = "��" }, [7] = { desc = "BOSS" }, [8] = { desc = "���Ĺ���" },
						};
function CanShowMonster( nSubType, showCollection )
	for subType, _ in pairs( showCollection ) do
		if subType == nSubType then
			return true;
		end
	end

	return false;
end

function UpdateMinimapMonsterInfo( MinMapContal )
	local MapInfo = MinMapContal:updateMapInfo()
	local btnIndex = 1
	local btn
	if MapShower:getRealMapId() >= MIN_COPY_ID then
		if GameActorPVP:isForcePkMode() and GameActorPVP:getPkMode() ~= 0 then
			for playerId in pairs(nearPlayer) do
				local player = ActorMgr:FindActor(playerId)
				if player and not player:isDead() and not g_ImportantMinimap[playerId] then
					if player:isCamp(2) then
						local x = player:getPlayerX() / 10
						local y = player:getPlayerZ() / 10
						local playerTexCoord = MinMapContal:getPointCoordOnTex( x, y )
						local nX = ( playerTexCoord.first	- MapInfo.nMapPosStartX ) * MapInfo.fZoom
						local nY = ( playerTexCoord.second	- MapInfo.nMapPosStartY ) * MapInfo.fZoom
						
						-- ��ʾ���״ﵱǰ��ʾ��Χ�����
						if GetDistance( nX, nY ) == nInner then
							btn = FindButton( "MinimapMonsterBtn"..btnIndex );
							if not btn then
								btn = CreateButton( "MinimapMonsterBtn"..btnIndex, "MinimapMonsterBtnTemplate", MinimapCluster );
							end
							--�����¿��ս�����������
							local playerName = GetPlayerName(player:getName())
							if player:getServerId() ~= -1 then
								playerName = "["..player:getActorWorldName().."]"..playerName
							end
							btn:SetClientString( playerName )
							btn:SetClientUserData( 0, playerId )
							btn:SetClientUserData( 1, x )
							btn:SetClientUserData( 2, y )
							btn:SetPoint( "center", "MinimapEnvelopTextureMap", "topleft", nX, nY )
							btn:Show()
							btn = getglobal(btn:GetName().."Tex")
							if GameClan_BaseWar:IsInBaseWarPworld() then
								btn:SetTexUV(189, 494, 17, 16)
							else
								btn:SetTexUV(171, 494, 17, 16)
							end
							btnIndex = btnIndex + 1
						end
					end
				end
			end
		else
			for monsterId in pairs(nearMonster) do
				local monster = ActorMgr:FindActor(monsterId)
				if monster and not monster:isDead() and not g_ImportantMinimap[monsterId] then
					local x = monster:getPlayerX() / 10
					local y = monster:getPlayerZ() / 10
					local monsterTexCoord = MinMapContal:getPointCoordOnTex( x, y )
					local nX = ( monsterTexCoord.first	- MapInfo.nMapPosStartX ) * MapInfo.fZoom
					local nY = ( monsterTexCoord.second	- MapInfo.nMapPosStartY ) * MapInfo.fZoom
						
					-- ��ʾ���״ﵱǰ��ʾ��Χ�Ĺ���
					if GetDistance( nX, nY ) == nInner and CanShowMonster( getMonsterSubtype( monster:getResId() ), t_monsterCanShow ) then
						btn = FindButton( "MinimapMonsterBtn"..btnIndex );
						if not btn then
							btn = CreateButton( "MinimapMonsterBtn"..btnIndex, "MinimapMonsterBtnTemplate", MinimapCluster );
						end
						btn:SetClientString( monster:getName() )
						btn:SetClientUserData( 0, monsterId )
						btn:SetClientUserData( 1, x )
						btn:SetClientUserData( 2, y )
						btn:SetPoint( "center", "MinimapEnvelopTextureMap", "topleft", nX, nY )
						btn:Show()
						btn = getglobal(btn:GetName().."Tex")
						btn:SetTexUV(189, 494, 17, 16)
						btnIndex = btnIndex + 1
					end
				end
			end
		end
	end
	
	for i = btnIndex, LAST_MONSTER_BTN_USE do
		btn = getglobal("MinimapMonsterBtn" .. i)
		if btn then
			btn:Hide()
		end
	end
	LAST_MONSTER_BTN_USE = btnIndex - 1
end

function UpdateMinimapImportantInfo(MinMapContal)
	local actor
	local index = 1
	local MapInfo = MinMapContal:updateMapInfo()
	for id, path in pairs(g_ImportantMinimap) do
		actor = ActorMgr:FindActor(id)
		if actor and not actor:isDead() then
			local x = actor:getPlayerX() / 10
			local y = actor:getPlayerZ() / 10
			local monsterTexCoord = MinMapContal:getPointCoordOnTex( x, y )
			local nX = ( monsterTexCoord.first	- MapInfo.nMapPosStartX ) * MapInfo.fZoom
			local nY = ( monsterTexCoord.second	- MapInfo.nMapPosStartY ) * MapInfo.fZoom
				
			-- ��ʾ���״ﵱǰ��ʾ��Χ�Ĺ���
			if GetDistance( nX, nY ) == nInner then
				local btn = FindButton("MinimapImportantBtn"..index);
				if not btn then
					btn = CreateButton( "MinimapImportantBtn"..index, "MinimapImportantBtnTemplate", MinimapCluster );
				end
				btn:SetClientString(actor:getName())
				btn:SetClientUserData(0, id)
				btn:SetClientUserData(1, x)
				btn:SetClientUserData(2, y)
				btn:SetPoint("center", "MinimapEnvelopTextureMap", "topleft", nX, nY)
				btn:Show()
				btn = getglobal("MinimapImportantBtn"..index.."Tex")
				if type(path) == type("") then
					btn:SetTexture(path)
				else
					btn:SetTexture(path.path)
					btn:SetTexUV(path.x, path.y, path.w, path.h)
				end
				index = index + 1
			end
		end
	end
	for i = index, LAST_IMPORTANT_BTN_USE do
		btn = getglobal("MinimapImportantBtn" .. i)
		if btn then
			btn:Hide()
		end
	end
	LAST_IMPORTANT_BTN_USE = index - 1
end


function UpdateMinimapMachineWarBossInfo(MinMapContal)
	local index = 1
	local MapInfo = MinMapContal:updateMapInfo()
	for _, args in pairs(g_MachineWarBossMinimap) do
		local MaxMapControl	= MapShower:GetMaxMapContal();
		if MaxMapControl:getMapID() == args.map then
			local x = args.x/10
			local y = args.y/10
			local monsterTexCoord = MinMapContal:getPointCoordOnTex( x, y )
			local nX = ( monsterTexCoord.first	- MapInfo.nMapPosStartX ) * MapInfo.fZoom
			local nY = ( monsterTexCoord.second	- MapInfo.nMapPosStartY ) * MapInfo.fZoom
			
			if GetDistance( nX, nY ) == nInner then
				local btn = FindButton("MinimapPworldBossBtnTemplate"..index);
				if not btn then
					btn = CreateButton( "MinimapPworldBossBtnTemplate"..index, "MinimapPworldBossBtnTemplate", MinimapCluster );
				end
				btn:SetClientString( args.name )
				btn:SetClientUserData(1, x)
				btn:SetClientUserData(2, y)
				btn:SetPoint("center", "MinimapEnvelopTextureMap", "topleft", nX, nY)
				btn:Show()
				btn = getglobal("MinimapPworldBossBtnTemplate"..index.."Tex")
				local path = args.path
				if type(path) == type("") then
					btn:SetTexture(path)
				else
					btn:SetTexture(path.path)
					btn:SetTexUV(path.x, path.y, path.w, path.h)
				end
				index = index + 1
			end
		end
	end
	for i = index, LAST_PWORLD_BOSS_USE do
		btn = getglobal("MinimapPworldBossBtnTemplate" .. i)
		if btn then
			btn:Hide()
		end
	end
	LAST_PWORLD_BOSS_USE = index - 1
end

function UpdateMinimapClanWarCampMonInfo(MinMapContal)
	local index = 1
	local MapInfo = MinMapContal:updateMapInfo()
	for _, args in pairs(g_ClanWarCampMonMinimap) do
		local MaxMapControl	= MapShower:GetMaxMapContal();
		if MaxMapControl:getMapID() == args.map then
			local x = args.x/10
			local y = args.y/10
			local monsterTexCoord = MinMapContal:getPointCoordOnTex( x, y )
			local nX = ( monsterTexCoord.first	- MapInfo.nMapPosStartX ) * MapInfo.fZoom
			local nY = ( monsterTexCoord.second	- MapInfo.nMapPosStartY ) * MapInfo.fZoom
			
			if GetDistance( nX, nY ) == nInner then
				local btn = FindButton("MinimapPworldBossBtnTemplate"..index);
				if not btn then
					btn = CreateButton( "MinimapPworldBossBtnTemplate"..index, "MinimapPworldBossBtnTemplate", MinimapCluster );
				end
				btn:SetClientString( args.name )
				btn:SetClientUserData(1, x)
				btn:SetClientUserData(2, y)
				btn:SetPoint("center", "MinimapEnvelopTextureMap", "topleft", nX, nY)
				btn:Show()
				btn = getglobal("MinimapPworldBossBtnTemplate"..index.."Tex")
				local path = args.path
				if type(path) == type("") then
					btn:SetTexture(path)
				else
					btn:SetTexture(path.path)
					btn:SetTexUV(path.x, path.y, path.w, path.h)
				end
				index = index + 1
			end
		end
	end
	for i = index, LAST_PWORLD_BOSS_USE do
		btn = getglobal("MinimapPworldBossBtnTemplate" .. i)
		if btn then
			btn:Hide()
		end
	end
	LAST_PWORLD_BOSS_USE = index - 1
end

function ClearAllTeamButton()
	local btn;
	for i = 1, MAX_TEAM_MEMBER do
		btn = getglobal( "MinimapTeamMemPos" .. i );
		SetButtonInfo( { szName = btn:GetName(), ClientString = nil, ClientUserData0 = 0,
							ClientUserData1 = -1, ClientUserData2 = -1 } );
		btn:Hide();

		btn = getglobal( "MinimapTeamMemDir" .. i );
		SetButtonInfo( { szName = btn:GetName(), ClientString = nil, ClientUserData0 = 0,
							ClientUserData1 = -1, ClientUserData2 = -1 } );
		btn:Hide();
	end

	btn = getglobal( "MinimapCaptainPosBtn" );
	SetButtonInfo( { szName = "MinimapCaptainPosBtn", ClientString = "", ClientUserData0 = 0,
							ClientUserData1 = -1, ClientUserData2 = -1 } );
	btn:Hide();

	btn = getglobal( "MinimapCaptainDirBtn" );
	SetButtonInfo( { szName = "MinimapCaptainDirBtn", ClientString = "", ClientUserData0 = 0,
							ClientUserData1 = -1, ClientUserData2 = -1 } );
	btn:Hide();
end

function UpdateMinimapStallBiaoJiButtonOnMap( MinMapContal )
	for i = 1, MAX_STALL_BIAO_JI_NUM do
		local btn = getglobal( "Minimap_StallBiaoJiBtn"..i );
		btn:Hide();
	end
	
	local t_StallBuyFrameBiaoJiStalls = GetStallBuyFrameBiaoJiStalls();
	local MapInfo	= MinMapContal:updateMapInfo();
	local startX	= MapInfo.nMapPosStartX * MapInfo.fZoom;
	local startY	= MapInfo.nMapPosStartY * MapInfo.fZoom;
	local nBtnIndex	= 1;

	for name, roleData in pairs( t_StallBuyFrameBiaoJiStalls["trackList"] ) do
		local btn			= FindButton( "Minimap_StallBiaoJiBtn"..nBtnIndex );
		local suDiTexCoord	= MinMapContal:getPointCoordOnTex( roleData["x"], roleData["y"] );
		local nX			= math.floor(math.floor(suDiTexCoord.first * MapInfo.fZoom) - startX);
		local nY			= math.floor(math.floor(suDiTexCoord.second * MapInfo.fZoom) - startY);
		if GetDistance( nX, nY ) == nInner then
			btn:SetClientString( name.."��̯λ" )
			btn:SetClientUserData( 0, 0 )
			btn:SetClientUserData( 1, roleData["x"] )
			btn:SetClientUserData( 2, roleData["y"] )
			btn:SetPoint( "center", "MinimapEnvelopTextureMap", "topleft", nX, nY )
			-- ��ʾ���״ﵱǰ��ʾ��Χ��NPC
			btn:Show()
			nBtnIndex = nBtnIndex + 1
		end
	end
end
function UpdateMinimapTeamInfo( MinMapContal )
	-- �����ԭ��button�е���Ϣ,���ǲ�����button
	ClearAllTeamButton();
	-- ��ʾ��Ա��button
	-- ��ȡ������Ϣ
	local MapInfo			= MinMapContal:updateMapInfo();
	local teamInfo			= TeamManager:getTeamInfo();
	local mainplayer		= ActorMgr:getMainPlayer();
	local nRadius			= MinimapEnvelopTextureMap:GetHeight() / 2; --�뾶
	local mainPlayerTeamInfo = TeamManager:getTeamMemberInfo( mainplayer:getName() );
	if mainPlayerTeamInfo == nil then return end
	-- ����ֵ
	local nPosTeamCount = 1; -- ���ڱ�ʾ��ǰ�Ѿ�ʹ�õ�������ʾ�����Աλ�õ�button����
	local nDirTeamCount	= 1; -- ���ڱ�ʾ��ǰ�Ѿ�ʹ�õ�������ʾ�����Ա�����״�뾶��Χ�ĵ�button����
	for i = 1, teamInfo.MemberNum do
		local memberInfo = teamInfo.TeamMember[i-1];
		-- �ȴ�����ʾ�ӳ���ť,�ӳ���Ϊ1.������2.�ӳ����ߣ�3.�ӳ���������ͬһ�ŵ�ͼ������²���С�״�������ʾ�ӳ�ͼ��
		if not CompareRoleName( mainplayer:getName(), memberInfo.MemberName ) and memberInfo.OffLineTime == 0 and
			mainPlayerTeamInfo.MapID == memberInfo.MapID then
			if memberInfo.Position == 1 then
				local captainInfo = MinMapContal:updateMapCaptainInfo();
				if captainInfo.m_nPlayerX > 0 and captainInfo.m_nPlayerY > 0 then
					SetButtonInfo( { szName = "MinimapCaptainPosBtn", ClientString = captainInfo.m_szName,
								ClientUserData0 = captainInfo.m_nMapId,
								ClientUserData1 = captainInfo.m_nRealX, captainInfo = captainInfo.m_nRealY } );

					SetButtonInfo( { szName = "MinimapCaptainDirBtn", ClientString = captainInfo.m_szName,
								ClientUserData0 = captainInfo.m_nMapId,
								ClientUserData1 = captainInfo.m_nRealX, captainInfo = captainInfo.m_nRealY } );

					if GetDistance( captainInfo.m_nPlayerX, captainInfo.m_nPlayerY ) == nInner then
						MinimapCaptainPosBtn:SetPoint( "center", "MinimapEnvelopTextureMap", "topleft",
														captainInfo.m_nPlayerX, captainInfo.m_nPlayerY );
						MinimapCaptainPosBtn:Show();
					else
						local pos = MinMapContal:getPosition( nRadius, nRadius, captainInfo.m_nPlayerX, captainInfo.m_nPlayerY, nRadius - 5 );
						MinimapCaptainDirBtn:SetPoint( "center", "MinimapEnvelopTextureMap", "topleft", pos.nX , pos.nY );
						MinimapCaptainDirBtn:Show();
					end
				end
			else -- ��Ϊ�Ƕӳ�����
				local notCaptainInfo = MinMapContal:updateMapTeamMemberInfo( i - 1 );
				if notCaptainInfo.m_nPlayerX > 0 and notCaptainInfo.m_nPlayerY > 0 then
					-- ����Ա��ͬһ�ŵ�ͼ����,�����ߣ�����ʾ���Ӧ��button���״���
					if GetDistance( notCaptainInfo.m_nPlayerX, notCaptainInfo.m_nPlayerY ) == nInner then
						local btn = getglobal( "MinimapTeamMemPos" .. nPosTeamCount );
						SetButtonInfo( { szName = btn:GetName(), ClientString = notCaptainInfo.m_szName,
											ClientUserData0 = notCaptainInfo.m_nMapId,
											ClientUserData1 = notCaptainInfo.m_nRealX, ClientUserData2 = notCaptainInfo.m_nRealY } );

						btn:SetPoint( "center", "MinimapEnvelopTextureMap", "topleft", notCaptainInfo.m_nPlayerX, notCaptainInfo.m_nPlayerY );
						btn:Show();
						-- ����Ҫ��ʾ�Ĳ������ӳ����ڵĶ�Ա��Ŀ
						nPosTeamCount = nPosTeamCount + 1;

					else
						local btnDir = getglobal( "MinimapTeamMemDir" .. nDirTeamCount );
						SetButtonInfo( { szName = btnDir:GetName(), ClientString = notCaptainInfo.m_szName,
											ClientUserData0 = notCaptainInfo.m_nMapId,
											ClientUserData1 = notCaptainInfo.m_nRealX, ClientUserData2 = notCaptainInfo.m_nRealY } );

						local pos = MinMapContal:getPosition( nRadius, nRadius, notCaptainInfo.m_nPlayerX,
																notCaptainInfo.m_nPlayerY, nRadius - 5 );
						btnDir:SetPoint( "center", "MinimapEnvelopTextureMap", "topleft", pos.nX , pos.nY );
						btnDir:Show();
						-- ����Ҫ��ʾ�Ĳ������ӳ����ڵĶ�Ա��Ŀ
						nDirTeamCount = nDirTeamCount + 1;
					end
				end
			end
		end
	end
end

function UpdateMinimapSuDi( MinMapContal )
	local t_trackRoleControl	= GetTrackRoleControl();
	local t_allRoles			= t_trackRoleControl:getAllTrackRoles();
	local nTrackNum				= t_trackRoleControl:getAllTrackRolesNum();
	for i = LAST_MINIMAP_SUDI_BTN_USE + 1, nTrackNum do
		CreateButton( "MinimapSuDiBtn"..i, "MinimapSuDiBtnTemplate", MinimapFrame );
	end

	if nTrackNum > LAST_MINIMAP_SUDI_BTN_USE then
		LAST_MINIMAP_SUDI_BTN_USE = nTrackNum;
	end

	for i = 1, LAST_MINIMAP_SUDI_BTN_USE do
		local btn = FindButton( "MinimapSuDiBtn"..i );
		btn:Hide();
	end
	
	local MapInfo	= MinMapContal:updateMapInfo();
	local startX	= MapInfo.nMapPosStartX * MapInfo.fZoom;
	local startY	= MapInfo.nMapPosStartY * MapInfo.fZoom;
	local nBtnIndex	= 1;
	for name, roleData in pairs( t_allRoles ) do
		local btn			= FindButton( "MinimapSuDiBtn"..nBtnIndex );
		local suDiTexCoord	= MinMapContal:getPointCoordOnTex( roleData["x"], roleData["y"] );
		local nX			= math.floor(math.floor(suDiTexCoord.first * MapInfo.fZoom) - startX);
		local nY			= math.floor(math.floor(suDiTexCoord.second * MapInfo.fZoom) - startY);
		if GetDistance( nX, nY ) == nInner then
			btn:SetClientString( name )
			btn:SetClientUserData( 0, 0 )
			btn:SetClientUserData( 1, roleData["x"] )
			btn:SetClientUserData( 2, roleData["y"] )
			btn:SetPoint( "center", "MinimapEnvelopTextureMap", "topleft", nX, nY )
			-- ��ʾ���״ﵱǰ��ʾ��Χ��NPC
			btn:Show()
			nBtnIndex = nBtnIndex + 1
		end
	end

end

-- ����С��ͼ�ϵ�Ŀ��ѡ�е�
function UpdateMinimapTargetInfo( MinMapContal )
	--Ŀ���
	local targetInfo = MinMapContal:updateMinMapTargetInfo();
	if targetInfo.m_nPlayerX == 0 and targetInfo.m_nPlayerY == 0 then
		MinimapSelectTargetPosBtn:Hide();
	else
		if GetDistance( targetInfo.m_nPlayerX, targetInfo.m_nPlayerY ) == nInner then
			MinimapSelectTargetPosBtn:SetPoint( "center", "MinimapEnvelopTextureMap", "topleft", targetInfo.m_nPlayerX,
												targetInfo.m_nPlayerY );
			MinimapSelectTargetPosBtn:Show();
		else
			MinimapSelectTargetPosBtn:Hide();
		end
	end
end

-- ����С��ͼ�ϵ��������Ч
function UpdateMinimapClickInfo( MinMapContal )
	local mainplayer	= ActorMgr:getMainPlayer();
	local MapInfo		= MinMapContal:updateMapInfo();
	if not mainplayer:getAutoFind() then
		MinimapMouseClickPosBtn:Hide();
		bOnClick = false;
	end

	if bOnClick then
		local nClickX = ( nMouseClickPosX - MapInfo.nMapPosStartX ) * MapInfo.fZoom;
		local nClickY = ( nMouseClickPosY - MapInfo.nMapPosStartY ) * MapInfo.fZoom;

		if GetDistance(nClickX, nClickY) == nInner  then
			MinimapMouseClickPosBtn:SetPoint( "Center", "MinimapEnvelopTextureMap", "TopLeft", nClickX, nClickY );
			MinimapMouseClickPosBtn:Show();
		else
			MinimapMouseClickPosBtn:Hide();
		end
	end
end

function MinimapOnUpdate()
	-- �����״��Ϸ���ʾ������λ����Ϣ
	local mainplayer		= ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local playerPosition	= mainplayer:getPlayerPostion();
	--CoordInfoFont:SetText( "�� "..playerPosition.m_nPosX..", "..playerPosition.m_nPosZ.." ��" );
	CoordInfoFont:SetText( math.ceil( mainplayer:getPlayerX() / 1000 )..", "..math.ceil( mainplayer:getPlayerZ() / 1000 ) );
	
	local MinMapContal	= MapShower:GetMinMapContal();
	local MapInfo		= MinMapContal:updateMapInfo();
	local mapTexInfo	= MinMapContal:getMapTexInfo();
	MinimapEnvelopTextureMap:SetTexUV(	MapInfo.nMapPosStartX,	MapInfo.nMapPosStartY,
										MapInfo.nMapPosWidth,	MapInfo.nMapPosHeight );
	UpdateMapOres();
	-- ����Լ���
	local button = getglobal( "MinimapMainShowButton" );
	button:SetPoint( "center", "MinimapEnvelopTextureMap", "topleft", MapInfo.nPlayerX , MapInfo.nPlayerY );
	button:SetAngle( MapInfo.fAngle );

	-- ����
	local buttonCam = getglobal( "MinimapMainShowCamButton" );
	buttonCam:SetPoint( "center", "MinimapEnvelopTextureMap", "center", 0, 0 );
	buttonCam:SetAngle( MapInfo.fCamAngle );
	-- ����NPC����ʾ��Ϣ
	UpdateMinimapNpcInfo( MinMapContal );

	-- ���¶����Ա����ʾ��Ϣ
	UpdateMinimapTeamInfo( MinMapContal );

	-- ���¹������ʾ��Ϣ
	UpdateMinimapMonsterInfo( MinMapContal );
	
	-- ������ҪĿ�����ʾ��Ϣ
	UpdateMinimapImportantInfo(MinMapContal);
	
	--���⸱�����ʱ��Boss��ʾ��Ϣ
	UpdateMinimapMachineWarBossInfo(MinMapContal)
	
	--�������ս����Ŧ��ʾ
	UpdateMinimapClanWarCampMonInfo(MinMapContal)

	-- ����Ŀ������ʾ��Ϣ
	--UpdateMinimapTargetInfo( MinMapContal );

	--�������
	UpdateMinimapClickInfo( MinMapContal );

	UpdateMinimapSuDi( MinMapContal );
	UpdateMinimapStallBiaoJiButtonOnMap( MinMapContal );

	UpdateMiniClanBattleInfo( MinMapContal )

	UpdateMiniMapClanMapPos();
	UpdateMinimapOreInfo(MinMapContal);
	
	--�ж��Ƿ���ʾս������ͼ
	if GameClan_BaseWar:hasStrongOpen() and mainplayer:getLv() >= 30 then
		WarForcesBtn:Show();
	else
		WarForcesBtn:Hide();
		WarForcesFrame:Hide();
	end
end

function UpdateMiniClanBattleInfo( MinMapContal )
	MiniMapDrawLineFriendLine:ClearDrawPointList();
	MiniMapDrawLineEnemyLine:ClearDrawPointList();
	if MapShower:getRealMapId() == WorldCityMapID or GameClan_BaseWar:IsInBaseWarPworld() then	
		MinMapContal:drawClanMemberPoint("MinimapEnvelopTextureMap","MiniMapDrawLineFriendLine","MiniMapDrawLineEnemyLine");
	end
end

--@Desc: �ж�Ŀ����Ƿ����״�Բ��
function GetDistance( x,y )
	local nRadiusX	=  MinimapEnvelopTextureMap:GetHeight() / 2;  --�״�뾶X
	local nRadiusY	=  MinimapEnvelopTextureMap:GetHeight() / 2; --�״�뾶Y
	local fX		= math.abs( nRadiusX - x );
	local fY		= math.abs( nRadiusY - y );
	local fLong		= math.sqrt( fX * fX + fY * fY );
	local nWidth	= MinimapEnvelopTextureMap:GetHeight();

	if fLong < nWidth/2 - 5 then
		return nInner;
	else
		return nOutside;
	end
end

function MinimapOnMouseDown()
	--TODO: ���⴦�������ͼ
	if arg1 == "LeftButton" then
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer == nil then return end

		local x = GetCursorPosX();
		local y = GetCursorPosY();

		local nx = MinimapEnvelopTextureMap:GetRealLeft();
		local ny = MinimapEnvelopTextureMap:GetRealTop();

		local realx = x - nx ;
		local realy = y - ny ;

		local MinMapContal	= MapShower:GetMinMapContal();
		local MapInfo		= MinMapContal:updateMapInfo();

		if GetDistance( realx, realy ) == nInner then
			local sceneCoord = MinMapContal:getMouseCurPos( realx, realy );
			MapShower:GetMaxMapContal():linkMoveto( MinMapContal:getMapID(),  sceneCoord.first, sceneCoord.second );

			MinMapContal:setView( g_nViewWidth, g_nViewHeight );

			nMouseClickPosX = MapInfo.nMapPosStartX + ( x - nx ) / MapInfo.fZoom;
			nMouseClickPosY = MapInfo.nMapPosStartY + ( y - ny ) / MapInfo.fZoom;

			nMouseClickLastTime = os.clock();
			bOnClick = true;
		end
	end
end

local nClickTexNum = 1
function MinimapMouseClickPosBtn_OnUpdate()
	if os.difftime ( os.clock(), nMouseClickLastTime) > 5 then
		bOnClick = false;
		this:Hide();
	else
		if nClickTexNum == 1 then
			MouseClickPosTexture:SetTexture("uires\\ZhuJieMian\\XiaoDiTu\\XiaoDiTu\\XiaoDiTuShuBiaoDongHua\\XiaoDiTuShuBiao.tga");
			nClickTexNum = 2;
		elseif  nClickTexNum == 2 then
			MouseClickPosTexture:SetTexture("uires\\ZhuJieMian\\XiaoDiTu\\XiaoDiTu\\XiaoDiTuShuBiaoDongHua\\XiaoDiTuShuBiao1.tga");
			nClickTexNum = 1;
		end
	end
end

function MinMapWorldMapShowButtonClick()
	local Frame= getglobal( "WorldMapFrame" );
	if (Frame:IsShown()) then
		Frame:Hide();
	else
		Frame:Show();
	end
end

function MinimapNpcPosBtn_OnEnter()
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = this:GetClientString(), frame = this:GetParent(),
						button = this:GetName() } );
end

function MinimapNpcPosBtn_OnLeave()
	GameTooltip:Hide();
end

function MinimapSelectTargetPosBtn_OnLoad()
	--this:RegisterEvent("GE_TARGET_CHANGED");
end

function MinimapSelectTargetPosBtn_OnEvent()
	--[[if arg1 == "GE_TARGET_CHANGED" then
		local MinMapContal	= MapShower:GetMinMapContal();
		local MapInfo		= MinMapContal:updateMapInfo();
		local targetInfo = MinMapContal:updateMinMapTargetInfo();
		if targetInfo.m_nPlayerX ~= 0 and targetInfo.m_nPlayerY ~= 0 then
			local nRealTargetX = 0;
			local nRealTargetY = 0;
			local mainplayer = ActorMgr:getMainPlayer();
			if mainplayer == nil then return end

			local TargetObj	= ActorMgr:FindActor( mainplayer:GetSelectTarget() );
			nRealTargetX	= TargetObj:getPlayerPostion().m_nPosX * 1000 / 10;
			nRealTargetY	= TargetObj:getPlayerPostion().m_nPosZ * 1000 / 10;

			local targetTexCoord = MinMapContal:getPointCoordOnTex( nRealTargetX, nRealTargetY );
			local nX = ( targetTexCoord.first	- MapInfo.nMapPosStartX ) * MapInfo.fZoom;
			local nY = ( targetTexCoord.second	- MapInfo.nMapPosStartY ) * MapInfo.fZoom;
			if GetDistance( nX, nY ) == nInner then
				MinimapSelectTargetPosBtn:SetPoint( "center", "MinimapEnvelopTextureMap", "topleft", nX, nY );
				MinimapSelectTargetPosBtn:Show();
			else
				MinimapSelectTargetPosBtn:Hide();
			end
		end
	end
	--]]
end

-- ���ֳ�����ȫ���͵ı�׼
-- MapLimit_PK = 0, MapLimit_PKMode =  0 ��ȫ
-- MapLimit_PK = 1, MapLimit_PKMode =  0 ����
-- ����ս������

-- @Desc: ��Ϊ��꿿����ʾ�������İ�ť��ʱ����Ӧ
function SceneNameBtn_OnEnter()
	local szText = "#c87ff2e��ɫ������ȫ�����޷�PK#n\n#cfffc59��ɫ�����������򣬿�����PK����pk�ͷ�#n\n#cff0000��ɫ����ս�����򣬿�����PK����PK�ͷ�#n\n#cff0000(��������ڣ�������PK�ͷ����ط���PK�ͷ�)#n";
	ShowGameTooltip( { rich = "WorldMapGameRegionTooltipRichText", tipsFrame = "WorldMapGameRegionTooltip", text = szText, frame = "MinimapFrame",
						button = this:GetName() } );
end

--@Desc: ��Ϊ������簴ť��ʱ����Ӧ
function NetShowBtn_OnClick()
	--ChangeCareerFrame:Show();
	--[[
	FadeSceneNameFrame:Show();
	FadeTaskNameFrame:Show();
	ArmDurSignFrame:Show();
	ShowArmDurSignXinShouBtn( { text = "װ���;öȲ��㣬��ȥ�̵�����",  } );
	--]]
end

function PrintPhotoInfo( )
	local mainplayer	= ActorMgr:getMainPlayer();
	local nHairID		= mainplayer:getHair();
	local nFaceID		= mainplayer:getFace();
	local nGender		= mainplayer:getGender();
	local nHairColor	= mainplayer:getHead();
	local szHairIconName = "hairPhoto";
	local szPhotoIconName = "Photo";

	local t_tgas =	{
						[szPhotoIconName] = 
						{	[CLIENT_GENDER_MALE]	= { func = function ()
													print( szPhotoIconName.." = uires\\TuPianLei\\TouXiang\\JueSeTouXiang\\552000" .. ( nFaceID ) .. ".tga" );
													end  
											},
													
							[CLIENT_GENDER_FEMALE] = {	func = function ()
													print( szPhotoIconName.." = uires\\TuPianLei\\TouXiang\\JueSeTouXiang\\552100" .. ( nFaceID ) .. ".tga" );
													end  
											},
						},

						[szHairIconName] = 
						{	[CLIENT_GENDER_MALE]	= { func = function () 
														print( szHairIconName.." = uires\\TuPianLei\\TouXiang\\JueSeTouXiang\\male\\hair_" .. ( nHairID + 1 ).."_"..(nHairColor )..".tga"  );
													end
										},
								
							[CLIENT_GENDER_FEMALE] = { 
											func = function () 
														print( szHairIconName.." = uires\\TuPianLei\\TouXiang\\JueSeTouXiang\\female\\hair_".. ( nHairID + 1 ).."_"..(nHairColor )..".tga"  );
													end
											},
						},
					};
	for name, data in pairs( t_tgas ) do
		local func = data[nGender].func;
		func();
	end
end

function PointNorthButton_OnClick()
	local MinMapContal	= MapShower:GetMinMapContal();
	MinMapContal:faceToNorth();
	--StopAutoRoundWorlld();
end


--@Desc: ��Ϊ���������ť��ʱ����Ӧ
function HelpShowBtn_OnClick()
	ShorcutOpen();
	--[[
	if webframe:IsShown() then
		webframe:Hide();
	elseif string.find( webframe:getLocalFile(), "../resource/Helper/html/s", 1, true) then
		webframe:SetPoint( "center", "$parent", "center", 0,0 );
		webframe:setLocalFile( "../resource/Helper/html/1.html" );
		webframe:SetSize(498,409);
		ShowWebFrame();
	else
		webframe:Show();
	end
	--]]
end

--@Desc: ��Ϊ������ť��ʱ����Ӧ
function ActivityShowBtn_nClick()
	ActivityShowBtnUVAnimationTex:Hide();
	local Frame= getglobal( "PromotionFrame" );
	if (Frame:IsShown()) then
		Frame:Hide();
	else
		Frame:Show();
	end
end

function MarketBtn_nClick()
	local Frame = getglobal("MarketManagerFrame");
	if(Frame:IsShown()) then
		Frame:Hide();
	else
		Frame:Show();
	end
end
--�����ҽ��װ�ť 
function TradeBtn_OnClick()
	local Frame = getglobal("MarketTicketFrame");
	if(Frame:IsShown()) then
		Frame:Hide();
	else
		Frame:Show();
	end
end

----@Desc: ��Ϊ��������ͼ��ť��ʱ����Ӧ
function AreaMapShow_OnClick()
    if( WorldMapFrame:IsShown() ) then
		WorldMapFrame:Hide();
    else
		WorldMapFrame:Show();
    end
end
function WarForcesBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer:isOutSide() then
		ShowMidTips( "���״̬�޷����оݵ����" )
		return
	end
	if WarForcesFrame:IsShown() then
		WarForcesFrame:Hide();
		return;
	else
		GameClan_BaseWar:RequestAllStrongInfo();
		WarForcesFrame:Show();
		WarForcesFrame:SetPoint( "center", "UIClient", "center", 0, 0 );
	end
end

----@Desc: ��Ϊ����Զ�������ť��ʱ����Ӧ
function AutoAssistBtn_OnClick()
	for i = 1, 10 do
		local AutoAssistBtn = getglobal("AutoAssistFrame_SkillBtn"..i);
		if  t_btnAutoAssistSkillToResult[i]["SkillId"] ~= 0  then
			local skillCDTime	= SkillMgr:getCDTime( t_btnAutoAssistSkillToResult[i]["SkillId"] );
			local nCurtime		= GameMgr:getTickTime();
			AutoAssistBtn:SetCooldownTimer(skillCDTime:past() / 1000, skillCDTime:total() / 1000, true);
		else
			AutoAssistBtn:SetCooldownTimer(0, 0, true);
		end
	end
	FirstUseAutoAssist();
	AutoAssist_FoodCheck();
	local Frame = getglobal("AutoAssistFrame");
	if( AutoAssistFrame:IsShown() ) then
		AutoAssistFrame:Hide();
	else
		AutoAssistFrame:SetPoint( "center","UIClient","center",0,0 );
		AutoAssistFrame:Show();
	end
end

--@Desc: ��Ϊ����̳ǰ�ť��ʱ����Ӧ
function ActivityShowBt_OnClick()
end

--@Desc: ��Ϊ������а�ť��ʱ����Ӧ
function RangeBtn_OnClick()
	if( SortLineFrame:IsShown() ) then
		SortLineFrame:Hide();
    else
		SortLineFrame:Show();
    end
end

--���а�tips ��
function MinimapFrameButton_OnUpdate()
	if nLastAcceptTime > 0 and os.clock() - nLastAcceptTime > 1 then
		nLastAcceptTime = 0;
		ShowRankTips();
	end
end

--@Desc: ��Ϊ�����Χ��ť��ʱ����Ӧ
function NearBtn_OnClick()
	--[[
	local t_teamFrames = { "TeamMemberFrame", "TeamApplyListFrame", "TeamAroundPlayerFrame", "TeamQiYueFrame" };
	for _, name in ipairs( t_teamFrames ) do
		local frame = getglobal( name );
		if frame:IsShown() then
			frame:Hide();
			return;
		end
	end

	local frame = getglobal( "TeamAroundPlayerFrame" );
	frame:SetPoint( "center", "$parent", "center", 0, 0 );
	frame:Show();

	if ReportBugFrame:IsShown() then
		ReportBugFrame:Hide();
	else
		ReportBugFrame:Show();
	end
	--]]
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local url = string.format( "http://service.wanyouyl.com/question/addGameQuestion.php?f=game&r1=%s&s=%s&u=%s&r2=%s", GameMgr:GetUtf8Uri(SelectWorldInfo.bigworld),
									GameMgr:GetUtf8Uri(SelectWorldInfo.world) , GameMgr:GetUtf8Uri(LoginMgr:getUserName()),GameMgr:GetUtf8Uri(GetPlayerName(mainplayer:getName())) )
	GameMgr:openWebSite( url );
	--http://service.wanyouyl.com/question/addGameQuestion.php?f=game&r1=����&s=������&u=�˺�&r2=��ɫ��
end

local t_AutoTeamBtnEvents = { ["GE_LEAVE_PWORLD"] = {},	["GE_AUTO_TEAM_SYNC"] = {}, ["GE_ENTER_PWORLD"] = {},
								["GE_AUTO_TEAM_SYNC_SUCC"] = {}, 
							};

t_AutoTeamBtnEvents["GE_LEAVE_PWORLD"].func = 
function ()
	--[[
	local nAutoTeamID = AutoTeamManager:getAutoTeamID();
	-- and isAnyTeamMemberInPworld()
	--print( "GE_LEAVE_PWORLD--nAutoTeamID = "..nAutoTeamID );
	
	if nAutoTeamID > 0 then
		BeginPlayAutoTeamBtnEffect();
		-- ��������Ͳ������Զ������
		if SearchTeamFrame:IsShown() then
			SearchTeamFrame:Hide();
		end

		-- �뿪�������ֻ�ܴ��Զ��������
		print( "GE_LEAVE_PWORLD--SuccessCreatActiveAutoTeamFrame--1044" );
		SetCurSuoXiaoFrameInfo( { name = "SuccessCreatActiveAutoTeamFrame" } );
	end
	--]]
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil or mainplayer:getLv() < 30 then
		return;
	end
	AutoPworldManager:requestPortalFrame( 1 );
end

t_AutoTeamBtnEvents["GE_ENTER_PWORLD"].func = 
function ()
	--[[
	local frame = getglobal( "SuccessCreatActiveAutoTeamFrame" );
	if frame:IsShown() then
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer:isCaptain() then
			SetCurSuoXiaoFrameInfo( { name = "SearchTeamFrame" } );
		end
		frame:Hide();
	end
	--]]
	EndPlayAutoTeamBtnEffect();
end

t_AutoTeamBtnEvents["GE_AUTO_TEAM_SYNC"].func = 
function ()
	--[[
	local nAutoTeamID = AutoTeamManager:getAutoTeamID();
	local t_curSuoXiaoFrame	= GetCurSuoXiaoFrameInfo();
	-- ����������
	-- �������Զ����
	if nAutoTeamID > 0 then
		local nCurMapID		= MapShower:getRealMapId();
		-- ��Ŀǰ��С�������Ѱ�����������ڸ�������
		if t_curSuoXiaoFrame["name"] == "SearchTeamFrame" and nCurMapID < MIN_COPY_ID then
			--print( "GE_AUTO_TEAM_SYNC--SuccessCreatActiveAutoTeamFrame--1064" );
			SetCurSuoXiaoFrameInfo( { name = "SuccessCreatActiveAutoTeamFrame" } );
			BeginPlayAutoTeamBtnEffect();
			-- �����ڸ����ڣ��Ҳ���SearchTeamFrame��壬������������С���
		elseif t_curSuoXiaoFrame["name"] ~= "SearchTeamFrame" and nCurMapID >= MIN_COPY_ID then
			SetCurSuoXiaoFrameInfo( { name = "SearchTeamFrame" } );
			EndPlayAutoTeamBtnEffect();
		end
		return;
	end
	
	-- ��Ч���Զ���Ӹ�����ȡ��
	EndPlayAutoTeamBtnEffect();
	
	SetCurSuoXiaoFrameInfo( { name = "SearchTeamFrame" } );

	if SuccessCreatActiveAutoTeamFrame:IsShown() then
		SuccessCreatActiveAutoTeamFrame:Hide();
	end
	if CreatingAutoTeamFrame:IsShown() then
		CreatingAutoTeamFrame:Hide()
	end
	--]]
end

t_AutoTeamBtnEvents["GE_AUTO_TEAM_SYNC_SUCC"].func = 
function ()	
	t_AutoTeamBtnEvents["GE_AUTO_TEAM_SYNC"].func();
end

function AutoTeamBtn_OnLoad()
	for event, _ in pairs( t_AutoTeamBtnEvents ) do
		this:RegisterEvent( event );
	end

	t_AutoTeamBtnEvents.__index = function ( tab, key )
									tab[key] = { func = function () end };
									return tab[key];
								end
	setmetatable( t_AutoTeamBtnEvents, t_AutoTeamBtnEvents );
end

function AutoTeamBtn_OnEvent()
	t_AutoTeamBtnEvents[arg1].func();
end

function AutoTeamBtn_OnClick()
	if PworldPortalFrame:IsShown() then
		PworldPortalFrame:Hide();
		return;
	end
	if PworldPortalConfirmFrame:IsShown() then
		PworldPortalConfirmFrame:Hide();
		return;
	end
	if PworldPortalPrepareFrame:IsShown() then
		PworldPortalPrepareFrame:Hide();
		return;
	end
	if MiZhiTaGetReadyFrame:IsShown() then
		MiZhiTaGetReadyFrame:Hide()
		return;
	end

	local num = AutoPworldManager:getMemberNum()
	
	if IsMiZhiTaPworld( AutoPworldManager:getPworldState() ) and not IsMiZhiTaPworldTimeOut() then
		if num > 0 then
			ShowMiZhiTaReadyFrame( AutoPworldManager:getPworldState() )
		end
		return
	end
	if num > 0 then
		PworldPortalPrepareFrame:Show()
		return
	end

	local nCurMapID	= MapShower:getRealMapId();
	if nCurMapID >= MIN_COPY_ID then
		ShowMidTips( "�ڸ�������ܽ��и�������" );
		return;
	end
	if PworldDifficultyFrame:IsShown() then
		PworldDifficultyFrame:Hide();
	end

	AutoPworldManager:requestPortalFrame( 0 );
end


function BeginPlayAutoTeamBtnEffect()
	if AutoTeamBtnModelView:IsShown() then
		return;
	end
	--util.PlayUIEffect( { modelView = "AutoTeamBtnModelView", effect = [[effect\entity\zudui02.ent]], scale = 1 } );
	local modelview = AutoTeamBtnModelView;
	modelview:setPetModel(0, 0, "effect\\entity\\zudui02.ent");
	modelview:setEntityPosition(0, -1, 102, 0);
	modelview:Show();
	--print( "BeginPlayAutoTeamBtnEffect" );
end

function EndPlayAutoTeamBtnEffect()
	if AutoTeamBtnModelView:IsShown() then
		AutoTeamBtnModelView:Hide();
		--print( "EndPlayAutoTeamBtnEffect" );
	end
end

function TrustBtn_OnClick()
	--��ʱ�����ε�
	ShowMidTips("�ù�����ʱδ����");
	--[[
	local mainplayer = ActorMgr:getMainPlayer();
	local nLevel   = mainplayer:getLv();
	if tonumber( nLevel ) < 30 then
		AddGameMiddleTips( GetMidTipsList()[MID_TRUST_NOTENOUGHLV] );
		return;
	end
	if TrustFrame:IsShown() then
		TrustFrame:Hide();
	else
		TrustFrame:Show();
	end
	]]--
end

function DpsBtn_OnClick()
	if DamageCountFrame:IsShown() then
		DamageCountFrame:Hide();
	else
		DamageCountFrame:Show();
	end
end

local nCurZoomType = 0;
--�״�������С
function ZoomOutBtn_OnClick()
	local largeBtn = getglobal( "ZoomInBtn" );
	if g_fZoom > g_fMinZoom then
		g_fZoom = g_fZoom - 0.1;
		MapShower:GetMinMapContal():setMapZoom( g_fZoom );

		if g_fZoom <= g_fMinZoom then
			this:Disable();
			GameTooltip:Hide();
		end

		if not largeBtn:IsEnable() then
			largeBtn:Enable();
		end
	end
end

--�״�����Ŵ�
function ZoomInBtn_OnClick()
	local smallBtn = getglobal( "ZoomOutBtn" );
	if g_fZoom < 1 then
		g_fZoom = g_fZoom + 0.1;
		MapShower:GetMinMapContal():setMapZoom( g_fZoom );
		-- ���Ѿ���������disable
		if g_fZoom >= 1 then
			this:Disable();
			GameTooltip:Hide();
		end

		if not smallBtn:IsEnable() then
			smallBtn:Enable();
		end
	end
end

function MinimapShow()
end

function  MinimapTeamMemPosBtn_OnEnter()
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = this:GetClientString(), frame = this:GetParent(),
						button = this:GetName() } );
end

function  MinimapTeamMemPosBtn_OnLeave()
       GameTooltip:Hide();
end

function  MinimapCaptainPosBtn_OnEnter()
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = this:GetClientString(), frame = this:GetParent(),
						button = this:GetName() } );
end

function MinimapCaptainPosBtn_OnLeave()
	GameTooltip:Hide();
end

function MinimapTeamMemDirBtn_OnEnter()
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = this:GetClientString(), frame = this:GetParent(),
						button = this:GetName() } );
end

function MinimapTeamMemDirBtn_OnLeave()
	GameTooltip:Hide();
end

function MinimapCaptainDirBtn_OnEnter()
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = this:GetClientString(), frame = this:GetParent(),
						button = this:GetName() } );
end

function MinimapCaptainDirBtn_OnLeave()
	GameTooltip:Hide();
end

function ShowMinimapOpenPworldXinShouBtn(which, text)
	if which then
		local animationTexture = getglobal(which:GetName().."UVAnimationTex")
		animationTexture:SetUVAnimation(80, true)
		animationTexture:Show()
	end
	MinimapOpenPworldXinShouBtn:Show()
	local rich = getglobal("MinimapOpenPworldXinShouBtnRich")
	rich:SetClientString(text or "")
	ReSizeTooltip("MinimapOpenPworldXinShouBtnRich", "MinimapOpenPworldXinShouBtn", 14)
end

function HideMinimapOpenPworldXinShouBtn(which)
	if which then
		local animationTexture = getglobal(which:GetName().."UVAnimationTex")
		animationTexture:Hide()
	end
	MinimapOpenPworldXinShouBtn:Hide()
end

function ChangeSafeArea( nType )
	local szAreaType = "��ȫ";
	mapType	=	nType;	
	SetRongYanItemUseMapType( true );
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer and mainplayer:isInPVPMap() then
		nType = 4
	end
	if nType == 0 then
		local mapDef = getMapDef( g_nReferMapId );
		if mapDef.MapLimit.PK == 0 and mapDef.MapLimit.PKMode == 0 then
			szAreaType = "��ȫ";

		elseif mapDef.MapLimit.PK == 1 and mapDef.MapLimit.PKMode == 0 and not bitAnd(mapDef.CtrlFlag,128) then
			szAreaType = "����";
			SetRongYanItemUseMapType( false );
		else
			szAreaType = "ս��";
		end	
	elseif bitAnd(nType,4) then
		szAreaType = "����PK"
	end
	local t_fonts = { ["MMMapName"] = { color = t_areaType[szAreaType]["color"] }, };
	util.SetFontsColor( t_fonts );
	CheckSafeAreaTips( szAreaType );
end

function CheckSafeAreaTips( szAreaType )
	if g_nRealMapId < 10000 and szAreaType ~= szLastAreaType then
		local MapOwner = GameClan_BaseWar:getCurMapStrongOWnerName( g_nRealMapId );
		local addOwnerInfo = "";
		if MapOwner == "" or MapOwner == nil then
			addOwnerInfo = "";
		else
			addOwnerInfo = "��"..MapOwner.."������أ�";
		end
		if szAreamType == "����PK" then
			AddGameMiddleTips2( "����"..szAreaType.."����PK���ƽ��".. addOwnerInfo );
		else
			AddGameMiddleTips2( "����"..szAreaType.."����"..addOwnerInfo );
		end		
		GetFirstPKTips();
	end
	szLastAreaType = szAreaType;
end

function GetFirstPKTips()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then 
		return ;
	end	
	local MinMapControl	= MapShower:GetMinMapContal();
	local g_MapId	= MinMapControl:getMapID();
	local mapsDef 	= getMapDef( g_MapId );
	
	if mainplayer:getLv() >= 20 
	     and mapType == 0 
	     and mapsDef.MapLimit.PK == 1 
	     and mapsDef.MapLimit.PKMode == 0  
	     and not GameWizard:isDo(GAMEWIZARD_FIRST_GET_PK_TIPS) then
		local xinShouBtn	= getglobal( "PhotoFramePKXinShouBtn" );
		local rich		= getglobal( xinShouBtn:GetName().."Rich" );
		local szText = "��������ģʽ�����֮�������PK������ڴ˴�ѡ��PKģʽ";
		local nSelfDefineExtendWidth = 10;
		rich:SetClientString( szText );
		ReSizeTooltip( rich:GetName(), xinShouBtn:GetName(), nSelfDefineExtendWidth );
		xinShouBtn:Show();
		local closeBtn = getglobal( xinShouBtn:GetName().."_CloseBtn" );
		closeBtn:SetPointEx( "topright", "PhotoFramePKXinShouBtnGuideBtn", "topright", -2, 0 );
		--��һ�δ�PK��ʾ��
		GameWizard:doStep( GAMEWIZARD_FIRST_GET_PK_TIPS );
	end
end
function ClearPKTips()
	local xinShouBtn = getglobal( "PhotoFramePKXinShouBtn" );
	if xinShouBtn:IsShown() then 
		xinShouBtn:Hide();
	end
end
function AddImportMinimapIcon(id, path)
	g_ImportantMinimap[id] = path
end

function RemoveImportMinimapIcon(id)
	g_ImportantMinimap[id] = nil
end

function ClearImportMinimapIcon()
	g_ImportantMinimap = {}
end

function AddMachineWarBossMinimap( name, mapid, path, x, y )
	local args = {}
	args.name = name
	args.map = mapid
	args.path = path
	args.x = x
	args.y = y
	table.insert( g_MachineWarBossMinimap, args )
end
function ClearMachineWarBossMinimap()
	g_MachineWarBossMinimap = {}
end

function AddClanWarCampMonMinimap(  monsterId, name, mapid, path, x, y )
	--������У��洢�µ���Ϣ
	for i = 1, table.getn( g_ClanWarCampMonMinimap ) do
		if g_ClanWarCampMonMinimap[i].monsterId == monsterId then
			g_ClanWarCampMonMinimap[i].name = name
			g_ClanWarCampMonMinimap[i].map = mapid
			g_ClanWarCampMonMinimap[i].path = path
			g_ClanWarCampMonMinimap[i].x = x
			g_ClanWarCampMonMinimap[i].y = y		
			return
		end
	end
	local args = {}
	args.monsterId = monsterId
	args.name = name
	args.map = mapid
	args.path = path
	args.x = x
	args.y = y
	table.insert( g_ClanWarCampMonMinimap, args )
end
function ClearClanWarCampMonMinimap()
	g_ClanWarCampMonMinimap = {}
end

local nFadeTime = 0.6;
local nStartShowAttackTime = 0;

function MiniMapShowAttackPosBtn_OnUpdate()
	local clanMapInfo = GameClanManager:getClanMapPos();
	local MinMapContal	= MapShower:GetMinMapContal();
	local MapInfo = MinMapContal:updateMapInfo();
	
	if clanMapInfo.AtkInfo.MapID > 0 and MapShower:getRealMapId() == clanMapInfo.AtkInfo.MapID then
		local serverTime = getServerTime().sec;
		local pos = MinMapContal:getPointCoordOnTex( clanMapInfo.AtkInfo.Pos.X, clanMapInfo.AtkInfo.Pos.Y );
		local nX = ( pos.first	- MapInfo.nMapPosStartX ) * MapInfo.fZoom ;
		local nY = ( pos.second	- MapInfo.nMapPosStartY ) * MapInfo.fZoom ;
		if GetDistance( nX, nY ) == nInner then
			MiniMapShowAttackPosBtn:SetPoint( "center", "MinimapEnvelopTextureMap", "topleft", nX, nY );
		else
			MiniMapShowAttackPosBtn:Hide();
			return;
		end

		if clanMapInfo.AtkInfo.EndTime < serverTime then
			local alpha = MiniMapShowAttackPosBtnTex:GetBlendAlpha();
			alpha = alpha - 0.01;
			if alpha <= 0 then
				MiniMapShowAttackPosBtn:Hide();
				return;
			end
			MiniMapShowAttackPosBtnTex:SetBlendAlpha( alpha );
			MiniMapShowAttackPosBtnEffectTex:SetBlendAlpha( alpha );
			
			local dt = os.clock() - nStartShowAttackTime;
			local size = 0.5 + 0.5*(dt%nFadeTime)/nFadeTime;

			MiniMapShowAttackPosBtnEffectTex:SetSize( 64*size,64*size );
		else
			if not MiniMapShowAttackPosBtn:IsShown() then
				nStartShowAttackTime = os.clock();
			end
			MiniMapShowAttackPosBtn:Show();
			MiniMapShowAttackPosBtnTex:SetBlendAlpha( 1 );
			MiniMapShowAttackPosBtnEffectTex:SetBlendAlpha( 1 );

			local dt = os.clock() - nStartShowAttackTime;
			local size = 0.5 + 0.5*(dt%nFadeTime)/nFadeTime;

			MiniMapShowAttackPosBtnEffectTex:SetSize( 64*size,64*size );
		end
	else
		MiniMapShowAttackPosBtn:Hide();
	end
end

local nStartShowVenueTime = 0;

function MiniMapShowVenuePosBtn_OnUpdate()
	local clanMapInfo = GameClanManager:getClanMapPos();
	local MinMapContal	= MapShower:GetMinMapContal();
	local MapInfo = MinMapContal:updateMapInfo();

	if clanMapInfo.VenueInfo.MapID > 0 and MapShower:getRealMapId() == clanMapInfo.VenueInfo.MapID then
		local pos = MinMapContal:getPointCoordOnTex( clanMapInfo.VenueInfo.Pos.X, clanMapInfo.VenueInfo.Pos.Y );
		local nX = ( pos.first	- MapInfo.nMapPosStartX ) * MapInfo.fZoom;
		local nY = ( pos.second	- MapInfo.nMapPosStartY ) * MapInfo.fZoom;

		if GetDistance( nX, nY ) == nInner then
			MiniMapShowVenuePosBtn:SetPoint( "center", "MinimapEnvelopTextureMap", "topleft", nX, nY );
		else
			MiniMapShowVenuePosBtn:Hide();
			return;
		end
		local serverTime = getServerTime().sec;
		if clanMapInfo.VenueInfo.EndTime < serverTime then
			local alpha = MiniMapShowVenuePosBtnTex:GetBlendAlpha();			
			alpha = alpha - 0.02;
			if alpha <= 0 then
				MiniMapShowVenuePosBtn:Hide();
				return;
			end
			MiniMapShowVenuePosBtnTex:SetBlendAlpha( alpha );
			MiniMapShowVenuePosBtnEffectTex:SetBlendAlpha( alpha );

			local dt = os.clock() - nStartShowVenueTime;
			local size = 0.5 + 0.5*(dt%nFadeTime)/nFadeTime;

			MiniMapShowVenuePosBtnEffectTex:SetSize( 64*size,64*size );
		else
			if not MiniMapShowVenuePosBtn:IsShown() then
				nStartShowVenueTime = os.clock();
			end
			MiniMapShowVenuePosBtn:Show();
			MiniMapShowVenuePosBtnTex:SetBlendAlpha( 1 );
			MiniMapShowVenuePosBtnEffectTex:SetBlendAlpha( 1 );
			
			local dt = os.clock() - nStartShowVenueTime;
			local size = 0.5 + 0.5*(dt%nFadeTime)/nFadeTime;

			MiniMapShowVenuePosBtnEffectTex:SetSize( 64*size,64*size );
		end
	else
		MiniMapShowVenuePosBtn:Hide();
	end
end

function UpdateMiniMapClanMapPos()
	MiniMapShowAttackPosBtn_OnUpdate();
	MiniMapShowVenuePosBtn_OnUpdate();
end

function UpdateMinimapOreInfo(MinMapContal)
	local MapInfo = MinMapContal:updateMapInfo()
	local startX = MapInfo.nMapPosStartX * MapInfo.fZoom
	local startY = MapInfo.nMapPosStartY * MapInfo.fZoom
	local index = 1
	for _, ore in ipairs(MapOres) do
		if ore.show and ActorMgr:FindActor(ore.handle) then
			local x, y = ore.x, ore.y
			local npcTexCoord		= MinMapContal:getPointCoordOnTex(x, y)
			local nX = math.floor(math.floor(npcTexCoord.first * MapInfo.fZoom) - startX)
			local nY = math.floor(math.floor(npcTexCoord.second * MapInfo.fZoom) - startY)

			if GetDistance( nX, nY ) == nInner then
				local btn = FindButton( "MinimapOreBtn"..index );
				if not btn then
					btn = CreateButton("MinimapOreBtn"..index, "MinimapOreBtnTemplate", MinimapCluster);
				end
				btn:SetClientString(ore.name)
				btn:SetClientUserData(0, ore.id)
				btn:SetClientUserData(1, x)
				btn:SetClientUserData(2, y)
				btn:SetPoint("center", "MinimapEnvelopTextureMap", "topleft", nX, nY)
				btn:Show()
				index = index + 1
			end
		end
	end
	for i = index, LAST_ORE_BTN_USE do
		local btn = getglobal("MinimapOreBtn" .. i)
		if btn then
			btn:Hide()
		end
	end
	LAST_ORE_BTN_USE = index - 1
	
	local player = ActorMgr:getMainPlayer()
	local map = player:getMapRealId()
	index = 1
	for _, ore in ipairs(MapBigOres) do
		if map == ore.map then
			local x, y = ore.x, ore.y
			local npcTexCoord		= MinMapContal:getPointCoordOnTex(x, y)
			local nX = math.floor(math.floor(npcTexCoord.first * MapInfo.fZoom) - startX)
			local nY = math.floor(math.floor(npcTexCoord.second * MapInfo.fZoom) - startY)

			if GetDistance( nX, nY ) == nInner then
				local btn = FindButton( "MinimapBigOreBtn"..index );
				if not btn then
					btn = CreateButton("MinimapBigOreBtn"..index, "MinimapBigOreBtnTemplate", MinimapCluster);
				end
				btn:SetClientString(ore.name)
				btn:SetClientUserData(0, ore.id)
				btn:SetClientUserData(1, x)
				btn:SetClientUserData(2, y)
				btn:SetPoint("center", "MinimapEnvelopTextureMap", "topleft", nX, nY)
				btn:Show()
				index = index + 1
			end
		end
	end
	for i = index, LAST_BIG_ORE_BTN_USE do
		btn = getglobal("MinimapBigOreBtn" .. i)
		if btn then
			btn:Hide()
		end
	end
	LAST_BIG_ORE_BTN_USE = index - 1
end