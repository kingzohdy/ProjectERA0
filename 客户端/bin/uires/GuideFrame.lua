guide_enum = {      4004,4008,4,8,4016,16,64,128,
					1001,1002,1004,1008,1016,1032,32,1064,1128,3008,3064,
					4032,2002,2004,2008,2064,2128,
					3001,3002,3004,3016,3032,3128,
					4001,4002
				}

guide_isdo = {}

guide_desc = {
					-- 4004 ��һ���ƶ�
					"��ã���ӭ����������磬#cEC6A2B������#n�������Ϳ��ƶ���#cEC6A2B������ס#n�Ļ��������Զ��ƶ�Ӵ��",
					-- 4008 ��һ����ת�ӽ�
			        "��鿴����ľ�����#cEC6A2B��ס����Ҽ��ƶ����#n,�Ϳ�����ת�ӽ��ˡ�",
			        -- 0004 ��һ�ζԻ�
			        "��������ǰ��#cEC6A2BNpc#n�������ǻ������ṩһЩ#cEC6A2B����#n��#cEC6A2B����#n���ƶ���������ǰ��#cEC6A2B������#n���ǣ��Ϳ��Խ��жԻ���",
					-- 0008 ��һ��������ȡ
			        "�ӵ������˰ɣ��ɵ㿪�������#cEC6A2B������־��ť#n�鿴�������顣#cEC6A2B��Ļ�ұ�#n�и�#cEC6A2B����׷��#n����������ʾ�� Npcͷ���ϣ���#cEC6A2B��ɫ��̾��#n�Ļ����Ϳ�������#cEC6A2B��ȡ����#n�ˡ���",
					-- 4016 ��һ���ύ����
			        "�����������һ��Ҫ�ǵ�#cEC6A2B�ύ#nӴ����������Ի��#cEC6A2B���Ľ���#n����������ʾ��#cEC6A2B��ɫ�ʺ�#n�����������ȡ��������δ��ɣ��������ύ��",
					-- 0016 ��һ������
					"��ϲ������2���ˣ�#cEC6A2B�����#n����֮���Զ�������������#cEC6A2B��������#n������������#cEC6A2B10��#n֮ǰϵͳ��������Զ�������",
					-- 0064 ��һ���¸���
				    "��#cEC6A2B�������̹�#n�Ի��ɽ��븱�����ڸ�����ɻ��#cEC6A2B����ľ���#n��#cEC6A2B���õ���Ʒ����#n����Ҫ���Ĳ�Ҫ���볬����ǰ�����ĸ�����",
					-- 0128 ��һ��ɱ��
			        "��#cEC6A2B������#n�������#cEC6A2B�Զ����й���#n�����ֻ��#cEC6A2B����#n����Ļ���#cEC6A2Bֻ��ѡ������",
					-- 1001 ��һ��ʹ�ü���
			        "ѡȡ���������ܶ�Ӧ#cEC6A2B��λ#n������#cEC6A2Bʹ��#n�ˡ����������ѧ���ܣ�����ս����ø������ɡ�",
					-- 1002 ��һ�ο�������
			        "ʹ��#cEC6A2B����#n����#cEC6A2Bһ���ԶԶ��������к�ը#n��վ������#cEC6A2B���ڵ�λ�ô�#n������#cEC6A2B�Զ�ʰȡ#n�ˡ�",
					-- 1004 ��һ��ʹ������
			        "ʰȡ��#cEC6A2B����#n�Զ��󶨵�#cEC6A2B������#n�ϣ�ʹ��#cEC6A2B������#n������ʹ�á����⣬��#cEC6A2B��Ļ���·�#n��˸��#cEC6A2B��ʱ������#n��ѡ������󣬼��ɵ��#cEC6A2B����ͼ��#nʹ������",
					-- 1008 ��һ�ο���ըҩͰ
			        "�����������#cEC6A2B��ҩͰ#n�����ܵ��������#cEC6A2B��ը#n�������������#cEC6A2B��Χɱ�˹���#n��",
					-- 1016 ��һ�νӴ�����
			        "#cEC6A2B����#n�򿪱����󣬿��Կ���#cEC6A2B����ͼ��#n��#cEC6A2B����#n�������װ������ס��һ�����������ɸ����ṩ��ǿ���#cEC6A2B������#n��",
					-- 1032 ��һ������
			        "�ӵ�#cEC6A2B����#n����󣬽�#cEC6A2Bί����Ʒ#n�͵�����Ҫ���#cEC6A2Bָ����#n����������ˡ�",
					-- 0032 ��һ�λ�ý�Ǯ
			        "ͨ��#cEC6A2B����#n�Լ�#cEC6A2Bɱ��#n�����Ի�ý�Ǯ��������Ǯ��Ҫ����#cEC6A2Bά��װ��#n�Լ�#cEC6A2B�����̵���Ʒ#n��",
					-- 1064 ��һ�βɼ�
				    "����#cEC6A2B����׷��#n��Ϣ�ϣ���#cEC6A2B�»���#n�Ĳɼ�������#cEC6A2B������#n������#cEC6A2B�Զ��ƶ�#n����ֲ�����#cEC6A2B���#n�����Ϳɲɼ����ڶ���������#cEC6A2B���ɴ��#n������ôβɼ�ʧ�ܡ�",
					-- 1128 ��һ����Ʒ��
					"�򿪱��������Կ���һ��#cEC6A2B��Ʒ��#n��#cEC6A2B�Ҽ����#n��֮�󣬿��Ի��װ�������Լ�����ľ�ϲ��",
					-- 3008 ��һ�νӴ�ʳ��
					"��#cEC6A2B����#n��#cEC6A2B������#nʳ�����ʳ�á�Ҳ���Խ����϶���#cEC6A2B������#n�ϣ�ͨ�����#cEC6A2Bָ����λ#n����ʹ�á���ʹ�ù����У�����ִ�в���������#cEC6A2B�ж�#n����Ϊ��",
					-- 3064 ��һ��������Ϊ
					"�ӵ�#cEC6A2B����#n�����ˣ��ͶԻ��������ƣ���ֻҪ�ܵ��������������#cEC6A2B����������#n������#cEC6A2B�Ի�#n��������ɡ�",
					-- 4032 ��һ�λ�ȡ������
					"�򿪱�����#cEC6A2B����#n��������飬����ѧ��#cEC6A2B�¼���#n�ˡ�Ҫ֪����ѧ���µ�ս�����ɣ�����Ч�������ս���е����漸�ʡ�",
					-- 2002 ��һ��ѧϰ����
					"ÿ��#cEC6A2B5��#n������Ժͼ���ѧϰԱ�Ի����������ѧϰ�¼��ܺ�������ѧ���ܡ����ܵȼ������󣬿����Ӽ��ܵ����������",
					-- 2004 ��һ�λ��ս��Ʒ
					"�������ϵ�#cEC6A2B����#n��#cEC6A2B����#n����ɴ򿪰�����壬#cEC6A2B����#n���#cEC6A2B��Ʒͼ��#n�����ɽ�����뱳������������ʾ�������#cEC6A2Bȫ��ʰȡ#n����ť����#cEC6A2Bһ����#nʰȡ#cEC6A2B������Ʒ#n��",
					-- 2008 ��һ���ռ�
					"�ӵ�#cEC6A2B�Ѽ�#n�����ˣ����������Ʒ��#cEC6A2B�������#n�ģ����#cEC6A2B�������#n��Ϣ�ϣ���#cEC6A2B�»���#n����Ʒ���֣������ƶ���#cEC6A2B������������#n��#cEC6A2Bɱ��#n���󣬿ɻ�ø���Ʒ��",
					-- 2064 ��һ�ο������浶
					"�����ǰ����������#cEC6A2B���浶#n����ʹ�����ɶԹ������#cEC6A2B�����˺�#n�����ܵ������ڵ�#cEC6A2Bλ��#n��������#cEC6A2B�Զ�ʰȡ#n�ˡ�",
					-- 2128 ��һ��ʰȡ���浶
					"#cEC6A2B���浶����#n���Զ��󶨵����#cEC6A2B���#n�ϣ������ڿ���ͨ��#cEC6A2B������#n������ʹ���ˡ�ͬ�����Ե����ʱ�������ϵ�#cEC6A2B����ͼ��#nʹ������",
					-- 3001 ��һ�λ��ҩ��
					"��#cEC6A2B����#n��#cEC6A2B������#nҩ��������ʹ�á�Ҳ���Խ����϶���#cEC6A2B������#n�ϣ�ͨ�����#cEC6A2Bָ����λ#n����ʹ�á�����Ѫ����ħ����ҩ���в�ͬ��ɫ����Ҫ����ˡ�",
					-- 3002 ��һ�η���
					"#cEC6A2B�������Ա#n�������������񣬷��������㻷�����У���������ޱ��޼ʵ�������",
					-- 3004 ��һ��ˢ��
                    "#cEC6A2Bͼ����#n�����ˣ�����#cEC6A2B����Ի�#n����#cEC6A2B�ٻ�#n�����",
					-- 3016 ��һֻ����
					"���Ѿ�ӵ��#cEC6A2B����#n����������桰#cEC6A2B����#n����ť�ɲ鿴���顣��������ṩ#cEC6A2Bս������#n������ս����#cEC6A2B�������#n�ϣ���#cEC6A2B����ͼ��#n�϶���#cEC6A2B������#n����ʹ�á�",
					-- 3032 ��һ��XP����
					"#cEC6A2B��ɫ״̬��#n�ϣ�#cEC6A2Bŭ����#n��ȼ�ձ�ʾ��ǰŭ��#cEC6A2B����#n������ʹ��XP�����ˡ������Ӧ���ܼ�λ���Ϳ���ʹ��XP���ܡ�",
					-- 3128 ��һ����ʱ����
					"������ȡ#cEC6A2B��ʱ#n����һ��Ҫ��#cEC6A2B�涨ʱ��#n�����Ŀ�꣬����������ʧ���ˣ����������������ȡ��",
					-- 4001 ��һ�ο�����ʱ��е
					"#cEC6A2B������#n������Ϸ�������޴�Ļ�е��ʹ�����ɶԶ������ͬʱ���#cEC6A2B�����˺�#n�����֮��Ϳ��Լ�Ԧ#cEC6A2B��ʱ��е#n�ɡ�",
					-- 4002 ��һ��ʹ����ʱ��е
					"��ʱ��е������#cEC6A2B����#n�˷ֱ��Ӧ��#cEC6A2B����#n�����͡�#cEC6A2BCtrl+����#n����λ���ֱ����ͨ�����#cEC6A2B��λ#n��#cEC6A2B��ʱ������#nʹ�á�"
				};

role_initskill = {
					{	g_nCAREER_WARRIOR,		1018	},
					{	g_nCAREER_MAGICOR,		2012	},
					{	g_nCAREER_GUNER,		3016	},
					{	g_nCAREER_CHURCH,		4002	},
				 };

role_lv5_skill = {1010,1016,1022,2010,2020,2018,4000,4002,4004,3010,3018,3024};
local t_Equip = {{1013000, 2019901, 1},{1103000, 2029901, 2},{1203000, 2039901, 3},{1503000, 2049901, 4}};
local ClanPworldFramShow = false;

g_nFocus_NONE			= 0;
g_nFocus_SPECIALGIFT	= 1;
g_nFocus_CHANGYOUGIFT	= 2;
g_nFocus_KITGIFT		= 3;

local g_nFocus = g_nFocus_NONE;

function CastSpell(id)
end

function GetFirstWeaponId( nRace )
	for i = 1, table.getn( t_Equip ) do
		if nRace == t_Equip[i][3] then
			return t_Equip[i][1];
		end
	end
end

function GetFirstCloseId( nRace )
	for i = 1, table.getn( t_Equip ) do
		if nRace == t_Equip[i][3] then
			return t_Equip[i][2];
		end
	end
end

local stepindex         = 1;
local curpage           = 1;
local countpage         = 0;
local endpage           = 1;
local starttime         = 0;
local disappeartime     = 30;
local guidestate        = 0;
local nGuideColorR      = 255;
local nGuideColorG      = 255;
local nGuideColorB      = 255;
local curStep			= 1;
local setSkillCount     = 0;   

local bRecond					= false;
local nTipsEffectStartTime		= 0;	 
local MAX_EFFECTFRAME_SHOWTIME	= 15;

-- �������߽���
local nCurAwardId = 0;
local nCurAwardTime = 0;
local nAwardLastTime = 0;

local BIRTH_MAP_ID = 2000;

-- ��ɫ��ʼ������
function RoleInit()
	if GameWizard:isDo(GAMEWIZARD_SETSHORTCUT) then 
		return; 
	end

	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then 
		return;
	end

	local nGender	= MainPlayer:getGender();
	local nRace		= MainPlayer:getRace();
	for i = 1, table.getn(role_initskill) do
		if nRace == role_initskill[i][1] and SkillMgr:isSkillLearned( role_initskill[i][2] ) then
			GameShortCut:initNewPlayerShortCut();
			GameWizard:doStep(GAMEWIZARD_SETSHORTCUT);
			break;
		end
	end
end

-- ��һ��ִ��
function PlayIsDo(GAMEWIZARD_FIRST,lv)
	--[[
	local mainPlayer = ActorMgr:getMainPlayer();
    if ( mainPlayer ~= nil and mainPlayer:getLv() > lv )  or (GameWizard:isDo(GAMEWIZARD_CLOSEALL) == true) then
        GameWizard:doStep(GAMEWIZARD_FIRST);
		countpage = countpage + 1;
		guide_isdo[countpage] = GetDesc(GAMEWIZARD_FIRST);
        SetCurPage();
        curStep = GAMEWIZARD_FIRST;
    else
        GuideFrameTalkText:SetText(GetDesc(GAMEWIZARD_FIRST), nGuideColorR, nGuideColorG, nGuideColorB);
		GuideFrameTalkText:SetFontType( FONTSYTLE_NORMAL );
        GameWizard:doStep(GAMEWIZARD_FIRST);
        curStep = GAMEWIZARD_FIRST;
        countpage = countpage + 1;
		guide_isdo[countpage] = GetDesc(GAMEWIZARD_FIRST);

		SetCurPage();
        GuideFrame:Show();
        GuideIcoFrame:Hide();
    end
--]]
end

-- ��һ�����񴥷�ִ��
function SpringPlayIsDo(GAMEWIZARD_FIRST,lv)
--[[
	local mainPlayer = ActorMgr:getMainPlayer();
    if ( mainPlayer ~= nil and mainPlayer:getLv() > lv )  or (GameWizard:isDo(GAMEWIZARD_CLOSEALL) == true) then
        GameWizard:doStep(GAMEWIZARD_FIRST);
		countpage = countpage + 1;
		guide_isdo[countpage] = GetDesc(GAMEWIZARD_FIRST);
        SetCurPage();
    else
        GuideFrameTalkText:SetText(GetDesc(GAMEWIZARD_FIRST), nGuideColorR, nGuideColorG, nGuideColorB);
		GuideFrameTalkText:SetFontType( FONTSYTLE_NORMAL );
        GameWizard:doStep(GAMEWIZARD_FIRST);
		countpage = countpage + 1;
		guide_isdo[countpage] = GetDesc(GAMEWIZARD_FIRST);
		SetCurPage();
        GuideFrame:Show();
        GuideIcoFrame:Hide();
    end
--]]
end

local t_UVAGuideEffectTexFrame = 
	{
	   "GuideAward_BackgroundBtn",
	   "GuideQuestion_BackgroundBtn",
	   "PlayerContainerFrame",
	   "ShopFrame",
	   "SkillFrame_SetRaceSkillBtn"
	};

-- ��ʾ����ָ����Ч��ʾ
FIRST_GET_WEAPON	= 1;
FIRST_GET_RIDE		= 2;

local t_shortcutEffectControl = { blinkBtn = "", startTime = 0, totalTime = 0, texX = 0, texY = 0 };
t_shortcutEffectControl["setBlinkBtn"] = 
function ( self, data )
	self["blinkBtn"]	= data["blinkBtn"];
	self["startTime"]	= data["totalTime"];
	self["texX"]		= data["texX"];
	self["texY"]		= data["texY"];
end

t_shortcutEffectControl["clearShortcutEffect"] = 
function ( self, data )
	self["blinkBtn"]	= "";
	self["startTime"]	= 0;
	self["totalTime"]	= 0;
end

function CheckShortcutEffect( szCaller )
	--CheckCharacterButtonLevelButton();
	if t_shortcutEffectControl["blinkBtn"] == "" then
		return;
	end
	
	--print( "CheckShortcutEffect--294--Caller = "..szCaller.."--blinkBtn = "..t_shortcutEffectControl["blinkBtn"] );
	local frame				= getglobal( "EffectFrame");
	local frameBtn			= getglobal( "EffectFrameGuide");
	local frameBtnTex		= getglobal( "EffectFrameGuideTex");
	local frameBtnRich		= getglobal( "EffectFrameGuideFont");
	local frameUVATexture	= getglobal( "EffectFrameUVAnimationTex");
	local szBlinkBtnName	= t_shortcutEffectControl["blinkBtn"];

	local relativeframe	= getglobal( szBlinkBtnName );
	-- ��������tips
	
	--print( "szBlinkBtnName = "..szBlinkBtnName.."texX = "..t_shortcutEffectControl["texX"].."	texY = "..t_shortcutEffectControl["texY"] );
	frameBtn:SetPointEx("bottomleft", relativeframe:GetName(), "bottomleft", t_shortcutEffectControl["texX"], t_shortcutEffectControl["texY"] );
	
	frameUVATexture:SetPointEx("bottomleft", relativeframe:GetName(), "bottomleft", -17, 15);
	frameUVATexture:SetUVAnimation(50,true);
	frameUVATexture:Show();
	EffectFrame:SetClientString( relativeframe:GetName() );
	frame:SetClientString(relativeframe:GetName());
end

function GuideEffectShow( szParent, szTips, szBlinkBtnName, szText, szPath, x, y, w, h, newWidth, newHight, nPosX, nPosY, nTexX, nTexY, bSetGray,
							nPurpose )
	local frame				= getglobal( szParent.."EffectFrame");
	local frameBtn			= getglobal( szParent.."EffectFrameGuide");
	local frameBtnTex		= getglobal( szParent.."EffectFrameGuideTex");
	local frameBtnRich		= getglobal( szParent.."EffectFrameGuideFont");
	local frameTexture		= getglobal( szParent.."EffectFrame_Texture");
	local frameUVATexture	= getglobal( szParent.."EffectFrameUVAnimationTex");
	local szInitBtnName		= szBlinkBtnName;
	
	local relativeframe	= getglobal( szBlinkBtnName );
	-- ��������tips
	if szText ~= "" then
		frameBtn:Show();
		frameBtnRich:SetClientString( szText );
		
		local offset = 0;
		if szParent == "Tips" then 
			offset = 10;
		end

		ReSizeTooltip( frameBtnRich:GetName(), frameBtn:GetName(), offset );
		frameBtn:SetPoint("bottomleft", relativeframe:GetName(), "bottomleft", nTexX, nTexY);
	end

	if szParent == "" then
		t_shortcutEffectControl:setBlinkBtn( { blinkBtn = szInitBtnName, startTime = os.clock(), totalTime = 15, texX = nTexX, texY = nTexY, } );
		--print( "GuideEffectShow--355--������˸�߼�����" );
		if bSetGray and frameUVATexture ~= nil then
			--print( "GuideEffectShow--355--������˸Ч��" );
			frameUVATexture:SetPoint("bottomleft", relativeframe:GetName(), "bottomleft", -17, 15);
			
			frameUVATexture:SetUVAnimation(50,true);
			frameUVATexture:Show();
			EffectFrame:SetClientString( relativeframe:GetName() );
		end
	end
	
	if szParent == "Tips" then
		nTipsEffectStartTime = os.clock();
	end	
	
	frame:Show();
	frame:SetClientString(relativeframe:GetName());
	
	ShowMidTips( szTips );	
	
	if frameTexture ~= nil then
		frameTexture:Show();
		
		frameTexture:SetSize( newWidth, newHight );
		frameTexture:SetPoint("bottomleft", relativeframe:GetName(), "bottomleft", nPosX, nPosY);
		for i = 1, table.getn( t_UVAGuideEffectTexFrame ) do
			if szParent == t_UVAGuideEffectTexFrame[i] then
				return;
			end
		end

		frameTexture:SetTexture( szPath );
		frameTexture:SetTexUV(  x, y, w, h );	 
	end
end

local t_taskLimitControl = {};

t_taskLimitControl["update"] = 
function ( self )
	local t_limitTaskInfo = self[1073];
	if t_limitTaskInfo == nil then
		return;
	end

	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then
		return;
	end

	local TargetObj  = ActorMgr:FindActor( mainplayer:GetSelectTarget() );

	if TargetObj ~= nil and TargetObj:getResId() == t_limitTaskInfo["monsterResID"] and 
		( TargetObj:getHP() / TargetObj:getMaxHP() ) < t_limitTaskInfo["hpPercent"] then
		
		if not GameWizard:isDo( GAMEWIZARD_FIRST_JIU_ZHI_DENG_LONG_CAO ) and Quest:isQuestRun( 1073 ) then
			for i = 1, MAX_TRACE_NUM * 2 do
				local taskItemBtn = getglobal( "TaskItemBtn"..i );
				local xinShouBtn = getglobal( taskItemBtn:GetName().."GuideBtn" );
				if taskItemBtn:IsShown() and taskItemBtn:GetClientUserData( 1 ) == 1073 and not xinShouBtn:IsShown() then
					local rich = getglobal( taskItemBtn:GetName().."GuideBtnRich" );
					local nSelfDefineExtendWidth = 10;
					local itemDef = getItemDef( taskItemBtn:GetClientUserData( 0 ) );
					local szText ="�����Ŀ��ʹ��";
					rich:SetClientString( szText );
					ReSizeTooltip( rich:GetName(), xinShouBtn:GetName(), nSelfDefineExtendWidth );
					xinShouBtn:Show();
					self["btn"] = xinShouBtn:GetName();
				end
			end
		end
	elseif self["btn"] ~= nil then
		local btn = getglobal( self["btn"] );
		btn:Hide();
		self["btn"] = nil;
	end
end

t_taskLimitControl["setState"] = 
function ( self, data )
	self["click"] = data["click"];
end

t_taskLimitControl["getTaskLimit"] = 
function ( self, data )
	return self[data["taskID"]];
end

t_taskLimitControl["setTaskLimit"] = 
function ( self, data )
	local taskID					= data["taskID"];
	if self[taskID] == nil then
		self[taskID] ={};
	end

	self[taskID]["taskID"]			= data["taskID"];
	self[taskID]["hpPercent"]		= data["hpPercent"] / 100;
	self[taskID]["monsterResID"]	= data["monsterResID"];
end

t_taskLimitControl["clearTaskLimit"] = 
function ( self, data )
	self[data["taskID"]] = nil;
	if self["btn"] ~= nil then
		local btn = getglobal( self["btn"] );
		btn:Hide();
		self["btn"] = nil;
	end
end

function GetTaskLimitControl()
	return t_taskLimitControl;
end

function EffectFrame_OnShow()
	-- �������߽���
	local tex = getglobal(  "EffectFrame_Texture" );
	if EffectFrame:GetClientString() == "" and tex ~= nil then
		util.UIFrameToFadeIn( "EffectFrame_Texture",  0, 1,	300, 100 );
		util.UIFrameToFadeKeep( "EffectFrame_Texture",	1, 0 );
		util.UIFrameToFadeOut( "EffectFrame_Texture",	1, 0,	300, 100 );
	end
	
	-- ����ָ����ʾTIPS����
	HideGuideEffectTip("EffectFrame");
end

function EffectFrame_OnHide()
	t_shortcutEffectControl:clearShortcutEffect();
end

function EffectFrame_OnUpdate()
	local nInterVal = 0.6;
	if EffectFrame:GetClientString() ~= "" then	
		nInterVal = 0.1;
	end
	
	if os.clock() - t_shortcutEffectControl["startTime"] > t_shortcutEffectControl["totalTime"] then
		if bRecond then
			bRecond = false;
			if EffectFrame:GetClientString() ~= "" then	
				EffectFrame_Texture:Hide();
			end 
		end
	else
		EffectFrameUVAnimationTex:Hide();
		EffectFrame:Hide();	  
	end
	local szFrameName = EffectFrame:GetClientString();
	if szFrameName == "TempFairyMagicShortcut1" or szFrameName == "XPBufShortcut1" then
		EffectFrameGuide:SetPointEx("bottomleft", szFrameName, "bottomleft", 7, -63);
	end
end

function XinShouBtn_CloseBtn_OnClick()
	local xinShouBtn = getglobal( this:GetParent() );
	xinShouBtn:Hide();
end

EFFECT_TIPS_FIRST_AUTO_FIND		= 0;	-- ��һ���Զ�Ѱ·
EFFECT_TIPS_FIRST_GET_TASK		= 1;	-- ��һ�ν�ȡ����
EFFECT_TIPS_FIRST_FINISH_TASK	= 2;	-- ��һ���������

local t_showReason = { [EFFECT_TIPS_FIRST_AUTO_FIND] = {desc = "����ȿ����Զ�Ѱ·"} };
function TipsEffectFrame_OnShow()
	-- ����ָ����ʾTIPS����
	HideGuideEffectTip("TipsEffectFrame");
end

local bTipsRecond	= false;
local nTipsLastTime = 0;
function TipsEffectFrame_OnUpdate()
	if os.clock() - nTipsEffectStartTime <= 15 then
		if bTipsRecond then
			TipsEffectFrame_Texture:Hide();
			nTipsLastTime = os.clock();
			bTipsRecond = false;
		end

		if os.clock() - nTipsLastTime > 0.3 then
			TipsEffectFrame_Texture:Show();
			bTipsRecond = true;
		end	  
	else
		TipsEffectFrame:Hide();
	end

	if EffectFrame:IsShown() then
		EffectFrame:Hide();
	end
end

-- �ر�����ָ����Ч��ʾ
function GuideEffectHide( szFrameName )
	local EffectFrame= getglobal( "EffectFrame" );

	if EffectFrame == nil then return end
	
	if not EffectFrame:IsShown() then
		return;
	end
	
	if szFrameName == nil then
		EffectFrame:Hide();
		return;
	end

	if szFrameName == EffectFrame:GetClientString() then
		EffectFrame:Hide();
		local blinkBtn = getglobal( szFrameName );
		blinkBtn:SetGray(false);
	end
end

-- �ر�����tipsָ����Ч��ʾ
function GuideTipsEffectHide( szFrameName )
	local EffectFrame= getglobal( "TipsEffectFrame" );
	if (EffectFrame:IsShown()) then
		if ( szFrameName ~= nil and szFrameName == EffectFrame:GetClientString() )
			or szFrameName == nil then
			EffectFrame:Hide();
		end
	end
end

-- ��һ�ε�¼
function PlayerFirstLogin()

end

-- ��һ���ƶ�
function PlayerFirstMove()
end

-- ��һ����ת�ӽ�
function PlayerFirstChangeView()
	if  GameWizard:isDo(GAMEWIZARD_FIRST_ROTATE) == false then
		GameWizard:doStep(GAMEWIZARD_FIRST_ROTATE);
		curStep = GAMEWIZARD_FIRST_ROTATE;
		--webframe:SetPoint( "center", "$parent", "center", 0,0 );
		--webframe:setLocalFile( "../resource/Helper/html/2.html" );
		--webframe:SetSize(498,409);
		ShowWebFrame();
	end
end

-- ��һ�ζԻ�
function PlayerFirstDialog()
	if GameWizard:isDo(GAMEWIZARD_FIRST_TALK) == false then
		PlayIsDo(GAMEWIZARD_FIRST_TALK,1);
	end
end

-- ��һ��ʹ�ü���
function PlayerFirstUseSkill()
	if GameWizard:isDo(GAMEWIZARD_FIRST_USEDSKILL) == false then
		GameWizard:doStep(GAMEWIZARD_FIRST_USEDSKILL);
		curStep = GAMEWIZARD_FIRST_USEDSKILL;
		--webframe:SetPoint( "center", "$parent", "center", 0,0 );
		--webframe:setLocalFile( "../resource/Helper/html/3.html" );
		--webframe:SetSize(498,409);
		ShowWebFrame();
	end
end

-- ��һ��ʹ������
function PlayerFirstUserGrenade()
	if  GameWizard:isDo(GAMEWIZARD_FIRST_USEDGRENADE) == false then
		--PlayIsDo(GAMEWIZARD_FIRST_USEDGRENADE,2);
	end
end

-- ��һ��ʰȡ���浶
function PlayerFirstGetSword()
    if  GameWizard:isDo(GAMEWIZARD_FIRST_GETSWORD) == false then
        --PlayIsDo(GAMEWIZARD_FIRST_GETSWORD,4);
    end
end

-- ��һ��XP����
function PlayerFirstPowerfull()
    if  GameWizard:isDo(GAMEWIZARD_FIRST_POWERFULL) == false then
        PlayIsDo(GAMEWIZARD_FIRST_POWERFULL,5);
    end
end

-- ��һ��ʹ����ʱ��е
function PlayerFirstUseMachine()
    if  GameWizard:isDo(GAMEWIZARD_FIRST_USEDMACHINE) == false then
        PlayIsDo(GAMEWIZARD_FIRST_USEDMACHINE,5);
    end
end

-- ��һ��ʹ�û��浶
function FirstUseSword()
	-- �����ǵ�һ��ʹ����ʱ���ߣ���ʲôҲ����
	local systemSetting	= GameMgr:getUserSystemConfig();
	if not GameWizard:isDo( GAMEWIZARD_FIRST_USE_SWORD ) and Quest:isQuestRun( 1054 ) then
		local text = "������"..(systemSetting:getGameOpt(GAME_OPT_SWAP_MOUSE) and "���" or "�Ҽ�")
		local code = GetKeyMap("�������1")
		if code == 0 then
			code = nil
		end
		if code then
			text = string.format("%s���ܿ�ݼ� %s ", text, KeyToString(code))
		end
		text = text.."�ɹ���Ŀ��"
		ShowTempShorCutFrameXinShouBtn( { text = text } );
		--GameWizard:doStep( GAMEWIZARD_FIRST_USE_SWORD );
	elseif not GameWizard:isDo( GAMEWIZARD_SECOND_USE_SWORD ) and Quest:isQuestRun(1062 ) then
		local text = "������"..(systemSetting:getGameOpt(GAME_OPT_SWAP_MOUSE) and "���" or "�Ҽ�")
		local code = GetKeyMap("�������1")
		if code == 0 then
			code = nil
		end
		if code then
			text = string.format("%s���ܿ�ݼ� %s ", text, KeyToString(code))
		end
		text = text.."�ɹ���Ŀ��"
		ShowTempShorCutFrameXinShouBtn( { text = text } );
		--GameWizard:doStep( GAMEWIZARD_SECOND_USE_SWORD );
	end
end


-- ��һ��ʹ����ʱ��е
function FirstUseJiJia()
	-- �����ǵ�һ��ʹ����ʱ��е����ʲôҲ����
	local systemSetting	= GameMgr:getUserSystemConfig();
	if not GameWizard:isDo( GAMEWIZARD_FIRST_USE_TMPMACHINE ) then
		local text = "������"..(systemSetting:getGameOpt(GAME_OPT_SWAP_MOUSE) and "���" or "�Ҽ�")
		local t = {}
		local code = GetKeyMap("�������1")
		if code == 0 then
			code = nil
		end
		if code then
			table.insert(t, KeyToString(code))
		end
		code = GetKeyMap("�������2")
		if code == 0 then
			code = nil
		end
		if code then
			table.insert(t, KeyToString(code))
		end
		if #t > 0 then
			text = string.format("%s���ܿ�ݼ� %s ", text, table.concat(t, ", "))
		end
		text = text.."�ɹ���Ŀ��"
		ShowTempShorCutFrameXinShouBtn( { text = text } );
		--GameWizard:doStep( GAMEWIZARD_FIRST_USE_TMPMACHINE );
	end
end

-- ��һ��ʹ����̨
function FirstUsePaoTai()
	-- �����ǵ�һ��ʹ����̨����ʲôҲ����
	local systemSetting	= GameMgr:getUserSystemConfig();
	if not GameWizard:isDo( GAMEWIZARD_FIRST_USE_TPAOTAI ) then
		local text = "������"..(systemSetting:getGameOpt(GAME_OPT_SWAP_MOUSE) and "���" or "�Ҽ�")
		local t = {}
		local code = GetKeyMap("�������1")
		if code == 0 then
			code = nil
		end
		if code then
			table.insert(t, KeyToString(code))
		end
		code = GetKeyMap("�������2")
		if code == 0 then
			code = nil
		end
		if code then
			table.insert(t, KeyToString(code))
		end
		if #t > 0 then
			text = string.format("%s���ܿ�ݼ� %s ", text, table.concat(t, ", "))
		end
		text = text.."�ɹ���Ŀ��"
		ShowTempShorCutFrameXinShouBtn( { text = text } );
		--GameWizard:doStep( GAMEWIZARD_FIRST_USE_TPAOTAI );
	end
end

-- ��һ��ʹ������
function FirstUseShouLei()
	-- �����ǵ�һ��ʹ�����ף���ʲôҲ����
	local systemSetting	= GameMgr:getUserSystemConfig();
	if not GameWizard:isDo( GAMEWIZARD_FIRST_USE_TSHOULEI ) then
		local text = "������"..(systemSetting:getGameOpt(GAME_OPT_SWAP_MOUSE) and "���" or "�Ҽ�")
		local code = GetKeyMap("�������1")
		if code == 0 then
			code = nil
		end
		if code then
			text = string.format("%s���ܿ�ݼ� %s ", text, KeyToString(code))
		end
		text = text.."�ɹ���Ŀ��"
		ShowTempShorCutFrameXinShouBtn( { text = text } );
		--GameWizard:doStep( GAMEWIZARD_FIRST_USE_TSHOULEI );
	end
end

-- ��һ�ε���������ť
function FirstClickFinishTask( questId )
	-- �����ǵ�һ�ε���������ť����ʲôҲ����
	if GameWizard:isDo( GAMEWIZARD_FIRST_FINISHQUEST_FROMUI ) then
		return;
	end
	
	local checkEffectFunc = 
	function ( questId )
		local t_limitQuestIDs = { 1049, 1050, 1051 };
		for _, limitId in ipairs( t_limitQuestIDs ) do
			if limitId == questId then
				return true;
			end
		end

		return false;
	end

	if not checkEffectFunc( questId ) then
		return;
	end

	local szText = "";
	if 1049 == questId then
		szText = "������������";
	else
		szText = "��������������ȡ����";
	end
	
	ShowNpcDialogEffectFrame( { text = szText, relFrame = "TaskAwardFrame", button = "TaskAwardFrame_GetBtn" } );

	if questId == 1051 then
		GameWizard:doStep( GAMEWIZARD_FIRST_FINISHQUEST_FROMUI );
	end
end

-- ��һ�ε����������ť
function FirstClickAcceptTask( questId )
	-- �����ǵ�һ�ε����������ť����ʲôҲ����
	if GameWizard:isDo( GAMEWIZARD_FIRST_GETQUEST_FROMUI ) then
		return;
	end
	
	local checkEffectFunc = 
	function ( questId )
		local t_limitQuestIDs = { 1049, 1050, 1051, 1052, 1053, 1054, 1055,
					  1056, 1057, 1058, 1059, 1060, 1062, 1064,
					  1065, 1066, 1068, 1069, 1070, 1071, 1072,
					  1073, 1074, 1075, 1076, 1077, 1078, 1079,
					  1080, 1081 };
		for _, limitId in ipairs( t_limitQuestIDs ) do
			if limitId == questId then
				return true;
			end
		end

		return false;
	end

	if not checkEffectFunc( questId ) then
		return;
	end
	
	local szText = "�������ȡ����";
		
	ShowNpcDialogEffectFrame( { text = szText, relFrame = "TaskAcceptFrame", button = "TaskAcceptFrame_AcceptBtn" } );
	
	if questId == 1081 then
		GameWizard:doStep( GAMEWIZARD_FIRST_GETQUEST_FROMUI );
	end
end

function SetNpcQuestPop( data )
	local npcId				= ActorMgr:getActorIdByResId( data["resId"] );
	local npc				= ActorMgr:FindActor( npcId );
	if npc ~= nil then
		GameWizard:setNpcTalkByActorId( npc:GetID(), data["content"], data["popBackRich"], data["showOnce"], data["headOffsetX"], data["headOffsetY"],								data["arrowTexture"], data["showArrowTex"], data["texOffsetX"], data["texOffsetY"], data["wizardOp"] );
	end
end

function KillMonster( nMonsterResId )
	-- ��Ϊ����	and Quest:isQuestRun( 1052 )
	if not GameWizard:isDo( GAMEWIZARD_FIRST_FIGHT ) then
		if nMonsterResId == 241000 and Quest:getQuestRunState( 1052 ) == CLIENT_TASK_STAT_FINI then
			GameWizard:delGroupActorsByResId( nMonsterResId );
		elseif nMonsterResId == 241002 and Quest:getQuestRunState( 1058 ) == CLIENT_TASK_STAT_FINI then
			GameWizard:delGroupActorsByResId( nMonsterResId );
		elseif nMonsterResId == 231021 and Quest:getQuestRunState( 1065 ) == CLIENT_TASK_STAT_FINI then
			GameWizard:delGroupActorsByResId( nMonsterResId );
		elseif nMonsterResId == 235000 and Quest:getQuestRunState( 1069 ) == CLIENT_TASK_STAT_FINI then
			GameWizard:delGroupActorsByResId( nMonsterResId );
		elseif nMonsterResId == 250001 and Quest:getQuestRunState( 1073 ) == CLIENT_TASK_STAT_FINI then
			GameWizard:delGroupActorsByResId( nMonsterResId );
		elseif nMonsterResId == 200008 and Quest:getQuestRunState( 1080 ) == CLIENT_TASK_STAT_FINI then
			GameWizard:delGroupActorsByResId( nMonsterResId );
			GameWizard:doStep(GAMEWIZARD_FIRST_FIGHT);
		end
	end
end

local t_firstGetSkill = {	[CLIENT_CAREER_WARRIOR]		= { skillID = 1016 }, 
							[CLIENT_CAREER_MAGICOR]		= { skillID = 2014 }, 
							[CLIENT_CAREER_GUNER]		= { skillID = 3011 }, 
							[CLIENT_CAREER_CHURCH]		= { skillID = 4002 },
							};
-- �����ô���
function SpringByGetQuest( questId, nRecycleID )	
	-- �Զ���Ӽ��
	CheckCloseSuccessCreatTaskAutoTeamFrame( questId );
	local t_NpcAddTalk =
	{
		-- ������ -- Σ���ķ�
		[1050] = { resId = 304001, content = "�����ȡ����",  popBackRich = "ChatPop", showOnce = true, headOffsetX = 12, 
					headOffsetY = -100, arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
					wizardOp = GAMEWIZARD_FIRST_GETQUEST_FROMUI, questId = 1050,  npcState = CLIENT_NORMAL_TASK_STAT_CAN_ACCEPT
				},

		-- ���ɶ� -- �±�ǩ��
		[1051] = { resId = 306011, content = "�����ȡ����",  popBackRich = "ChatPop", showOnce = true, headOffsetX = 12, 
					headOffsetY = -100, arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
					wizardOp = GAMEWIZARD_FIRST_GETQUEST_FROMUI, questId = 1051,  npcState = CLIENT_NORMAL_TASK_STAT_CAN_ACCEPT
				},
		
		-- ���� -- ��������
		[1053] = { resId = 303003, content = "����ɹ�����Ʒ",  popBackRich = "ChatPop", showOnce = true, headOffsetX = 12, 
					headOffsetY = -50, arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
					wizardOp = GAMEWIZARD_FIRST_OPENSHOP, questId = 1053, npcState = CLIENT_NORMAL_TASK_STAT_CAN_ACCEPT
				},	
		--[[
		-- ���� -- ���ε�ս��
		[1062] = { resId = 302016, content = "������Խ�������",  popBackRich = "ChatPop", showOnce = true, headOffsetX = 12, 
					headOffsetY = -50, arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
					wizardOp = GAMEWIZARD_FIRST_TRAN_TO_SKY_COPY, questId = 1062, npcState = CLIENT_NORMAL_TASK_STAT_CAN_ACCEPT
				},
		--]]
		--[[
		-- ���� -- ׳־����
		[1060] = { resId = 316000, content = "����ɷ���",  popBackRich = "ChatPop", showOnce = true, headOffsetX = 12, 
					headOffsetY = -50, arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
					wizardOp = GAMEWIZARD_FIRST_FLY_QUEST, questId = 1060, npcState = CLIENT_NORMAL_TASK_STAT_CAN_ACCEPT
				},
		--]]
	};
	
	local addTalkData = t_NpcAddTalk[questId];
	if addTalkData ~= nil then
		SetNpcQuestPop( addTalkData );
	end
	
	-- �ڻ��	��Ԯ����	��ʱ��	
	if questId == 1049 and not GameWizard:isDo(GAMEWIZARD_FIRST_GETMISSION ) and not GameMgr:isInPlayCamerAnim() then
		EndFirstEnterGameAnim();
	end

	-- ��ȡ�������ʱ����¸�����
	-- ��һ��������ȡ
	if questId == 1050 then
		--LUALOGINFO("questId == 1000");
		if not GameWizard:isDo(GAMEWIZARD_FIRST_GETMISSION ) then
			GameWizard:hideNpcTalkByResId( 304001 );
			SetTaskLogFrameGuide( questId, nRecycleID );
			GameWizard:doStep( GAMEWIZARD_FIRST_GETMISSION );
		end

		-- ����ָ��tips ��һ�ο�����������
		if  GameWizard:isDo(GAMEWIZARD_FIRST_OPENTASK) == false and Quest:getNumCanAcceptQuest() > 0 then
			--TaskLogFrame_CanAcceptQuestChkGuide:Show();
			--TaskLogFrame_CanAcceptQuestChkGuideRichFont:SetText( "�����\n�ɽ������б�",	86, 79, 54);
			GameWizard:doStep(GAMEWIZARD_FIRST_OPENTASK);
		else
			if TaskLogFrame_CanAcceptQuestChkGuide:IsShown() then
				TaskLogFrame_CanAcceptQuestChkGuide:Hide();
			end
		end
	end
	
	-- ������������
	if questId == 3112 and not GameWizard:isDo(GAMEWIZARD_FIRST_BLINK_LIAN_HUA_LABEL) then
		if TrumpFrame:IsShown() then
			ShowJinglIngMainFrameXinShou();
		else
			TrumpFrame:Show();
		end
	end
	
	-- ����ָ��tips ��һ�ο�����������
	if  not GameWizard:isDo(GAMEWIZARD_FIRST_SAWTASKLINK) then
		local lastId = GetLastShowTipsTask();
		if lastId ~= questId then
			SetNeedShowTips( true, questId );
		end
	end
	
	-- �ύ��ȡ��˵����񣬵ڶ�������
	if questId == 3134 and not GameWizard:isDo(GAMEWIZARD_FIRST_BLINK_STRENGTHEN_BTN) then
		RideFrame:Show();
	end

	-- ��һ���¸���
	if questId == 1004 and GameWizard:isDo(GAMEWIZARD_FIRST_GOCOPY) then
		SpringPlayIsDo(GAMEWIZARD_FIRST_GOCOPY,2);
	end
	
	-- [questId] = { [resId] = {}, [resId] = {}, }
	local mainplayer		= ActorMgr:getMainPlayer();
	local nSkillID			= t_firstGetSkill[GetInitRace( mainplayer:getRace() )]["skillID"];
	local SkillDef			= SkillMgr:getSpellStaticAtt( nSkillID,1 );
	local szMuZhuangPaoPao	= "�����ʹ�á�"..SkillDef.Name .."������";
	local systemSetting		= GameMgr:getUserSystemConfig();
	local t_taskAddActorPop = 
	{
	-- ��õ�һ�βɼ�����
	-- taskId  
	[1055] =	{ [293003] =	{	resId = 293003, 
									content = "����������ҿɲɼ�",  
									popBackRich = "ChatPop", showOnce = true,  headOffsetX = 10, headOffsetY = 0,
									arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
									wizardOp = GAMEWIZARD_FIRST_COLLECT,
								},
				},
	[1059] =	{ [293002] =	{	resId = 293002, 
									content = "����������ҿɲɼ�",  
									popBackRich = "ChatPop", showOnce = true,  headOffsetX = 10, headOffsetY = 0,
									arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
									wizardOp = GAMEWIZARD_FIRST_COLLECT,
								},
				},
	[1070] =	{ [293000] =	{	resId = 293000, 
									content = "����������ҿɲɼ�",  
									popBackRich = "ChatPop", showOnce = true,  headOffsetX = 10, headOffsetY = 0,
									arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
									wizardOp = GAMEWIZARD_FIRST_COLLECT,
								},
				},
	[1078] =	{ [293001] =	{	resId = 293001, 
									content = "����������ҿɲɼ�",  
									popBackRich = "ChatPop", showOnce = true,  headOffsetX = 10, headOffsetY = 0,
									arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
									wizardOp = GAMEWIZARD_FIRST_COLLECT,
								},
				},

	-- ��һ���η���
	[1064] = {	[351000] = {	resId = 351000, 
								-- �Һ����ܣ��������ң�
								content = "���Ҽ�����ң�����ʹ��ҩƷ",  
								popBackRich = "ChatPop", showOnce = true,  headOffsetX = 0, headOffsetY = 0,
								arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
								wizardOp = GAMEWIZARD_FIRST_CURE_NPC, 
							},
			},
	
	-- ��һս��
	[1052] = {	[241000] = {	resId = 241000, 
								content = ( systemSetting:getGameOpt( GAME_OPT_SWAP_MOUSE ) and "���" or "�Ҽ�" ) .."������﹥��",   
								popBackRich = "ChatPop", showOnce = true,  headOffsetX = 0, headOffsetY = 0,
								arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
								wizardOp = GAMEWIZARD_FIRST_FIGHT, 
							},
			},
	[1058] = {	[241002] = {	resId = 241002, 
								content = ( systemSetting:getGameOpt( GAME_OPT_SWAP_MOUSE ) and "���" or "�Ҽ�" ) .."������﹥��",   
								popBackRich = "ChatPop", showOnce = true,  headOffsetX = 0, headOffsetY = 0,
								arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
								wizardOp = GAMEWIZARD_FIRST_FIGHT, 
							},
			},
	[1065] = {	[231021] = {	resId = 231021, 
								content = ( systemSetting:getGameOpt( GAME_OPT_SWAP_MOUSE ) and "���" or "�Ҽ�" ) .."������﹥��",   
								popBackRich = "ChatPop", showOnce = true,  headOffsetX = 0, headOffsetY = 0,
								arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
								wizardOp = GAMEWIZARD_FIRST_FIGHT, 
							},
			},
	[1069] = {	[235000] = {	resId = 235000, 
								content = ( systemSetting:getGameOpt( GAME_OPT_SWAP_MOUSE ) and "���" or "�Ҽ�" ) .."������﹥��",   
								popBackRich = "ChatPop", showOnce = true,  headOffsetX = 0, headOffsetY = 0,
								arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
								wizardOp = GAMEWIZARD_FIRST_FIGHT, 
							},
			},
	[1073] = {	[250001] = {	resId = 250001, 
								content = ( systemSetting:getGameOpt( GAME_OPT_SWAP_MOUSE ) and "���" or "�Ҽ�" ) .."������﹥��",   
								popBackRich = "ChatPop", showOnce = true,  headOffsetX = 0, headOffsetY = 0,
								arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
								wizardOp = GAMEWIZARD_FIRST_FIGHT, 
							},
			},
	[1080] = {	[200008] = {	resId = 200008, 
								content = ( systemSetting:getGameOpt( GAME_OPT_SWAP_MOUSE ) and "���" or "�Ҽ�" ) .."������﹥��",   
								popBackRich = "ChatPop", showOnce = true,  headOffsetX = 0, headOffsetY = 0,
								arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
								wizardOp = GAMEWIZARD_FIRST_FIGHT, 
							},
			},
	
	-- ʵ����֤��
	[1061] = {	[350040] = {	resId = 350040, 
								content = szMuZhuangPaoPao,   
								popBackRich = "ChatPop", showOnce = true,  headOffsetX = 0, headOffsetY = 50/GetScreenScaleY(),
								arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
								wizardOp = GAMEWIZARD_FIRST_USE_NEW_SKILL, 
							},
			},
	};

	local t_chgActorShowEvent = t_taskAddActorPop[questId];
	if t_chgActorShowEvent ~= nil then
		for nResId, data in pairs( t_chgActorShowEvent ) do
			SetMonsterQuestPopByResId( { config = data, resId = nResId } );
		end
	end

	-- ��һ�η���
	if questId == 4001 then
		if not GameWizard:isDo(GAMEWIZARD_FIRST_FLY) then
			SpringPlayIsDo(GAMEWIZARD_FIRST_FLY,4);
		end
	end

	-- ��һ��������Ϊ
	if questId == 4002 then
		if not GameWizard:isDo(GAMEWIZARD_FIRST_SPECIALMISSION) then
			SpringPlayIsDo(GAMEWIZARD_FIRST_SPECIALMISSION,4);
		end
	end

--[[
	-- ��һ�νӴ�����
	if questId == 1056 and not GameWizard:isDo(GAMEWIZARD_FIRST_GETWEAPON) then
		--GameWizard:doStep(GAMEWIZARD_FIRST_GETWEAPON);
		ShowMidTips("����������ť");
		GuideEffectShow( "", "����򿪱�������", "ContainerButton", "����һ��װ��������򿪱�������\n#P��ݼ���B��", "uires\\ZhuJieMian\\1.dds", 352, 609, 60, 60, 50, 60, -9, 5, 7, -58, true, FIRST_GET_WEAPON );
	end
--]]
	
	local t_taskAddNpcPop = 
	{

	-- ��һ�δ��͵�ʱ�����ɯ
	[1077] = {	resId = 303021, 
					content = "    ������Դ��͵�ʱ���", 
					popBackRich = "ChatPop", showOnce = true, headOffsetX = 12, headOffsetY = -50,
					arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0,
					wizardOp = { GAMEWIZARD_FIRST_TALK_ZHUCHENG_TRAN, } ,
				},
	};
	local t_chgNpcShowEvent = t_taskAddNpcPop[questId];
	-- ��һ�δ��͵�ʱ���
	local mainplayer = ActorMgr:getMainPlayer();
	if questId == 1077 and not GameWizard:isDo(GAMEWIZARD_FIRST_TALK_ZHUCHENG_TRAN) and  mainplayer:getLv() >= 10 then
		SetNpcWithoutQuestPopByState( t_chgNpcShowEvent );
	end
end

function SetMonsterQuestPopByResId( data )
	local config = data["config"];
	GameWizard:setGroupActorsByResId( data["resId"], config["content"], config["popBackRich"], config["showOnce"], 
										config["headOffsetX"], config["headOffsetY"], config["arrowTexture"], config["showArrowTex"], config["texOffsetX"], config["texOffsetY"], config["wizardOp"] );
end

-- ������ɴ���
function SpringByFinishQuest( questId )
	-- [questId] = { npcResId = ? }
	-- ɾ������������
	local t_finishQuestNpcDelMap = 
	{
	-- Σ���ķ�	-- ������
	[1050] = { npcResId = 304001 },
	-- �±�ǩ��	-- ���ɶ�
	--[1051] = { npcResId = 306011 },
	-- ����Ⱦ�ĺ���	-- ��������
	[1064] = { npcResId = 351000 },
	};

	local npcData = t_finishQuestNpcDelMap[questId];
	if npcData ~= nil then
		local npcId	= ActorMgr:getActorIdByResId( npcData["npcResId"] );
		if npcId ~= 0 then
			GameWizard:delNpcTalkByActorId( npcId );
		end
	end

	t_taskLimitControl:clearTaskLimit( { taskID = questId } );
	
	if not GameWizard:isDo( GAMEWIZARD_FIRST_JIU_ZHI_DENG_LONG_CAO ) and questId == 1073 then
		GameWizard:doStep( GAMEWIZARD_FIRST_JIU_ZHI_DENG_LONG_CAO );
	end
	-- ���ӵ���������
	local t_NpcAddTalk =
	{
		-- ���ɶ� -- Σ���ķ�
		[1050] = { resId = 306011, content = "����������",  popBackRich = "ChatPop", showOnce = true, headOffsetX = 12, 
					headOffsetY = -100, arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
					wizardOp = GAMEWIZARD_FIRST_FINISHQUEST_FROMUI, questId = 1050, npcState = CLIENT_TASK_STAT_FINI,
				},
		
		-- ���ɶ� -- �±�ǩ��
		[1051] = { resId = 306011, content = "����������",  popBackRich = "ChatPop", showOnce = true, headOffsetX = 12, 
					headOffsetY = -100, arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
					wizardOp = GAMEWIZARD_FIRST_FINISHQUEST_FROMUI, questId = 1051, npcState = CLIENT_TASK_STAT_FINI,
				},
		
	};

	local addTalkData = t_NpcAddTalk[questId];
	if addTalkData ~= nil then
		SetNpcQuestPop( addTalkData );
	end

	local t_taskDelActorPop = 
	{
	-- ��һ��ս��
	-- taskId  
	[1052] = { 241000, },
	[1058] = { 241002, },
	[1065] = { 231021, },
	[1069] = { 235000, },
	[1073] = { 250001, },
	[1080] = { 200008, },

	-- ��õ�һ�βɼ�����
	-- taskId  
	[1055] = { 293003, },
	[1059] = { 293002, },
	[1070] = { 293000, },
	[1078] = { 293001, },

	-- ʵ����֤��
	-- taskId  
	[1061] = { 350040, },
	};
	
	local t_chgActorShowEvent = t_taskDelActorPop[questId];
	if t_chgActorShowEvent ~= nil then
		for _, resId in ipairs( t_chgActorShowEvent ) do
			GameWizard:delGroupActorsByResId( resId );
		end
	end

	if questId == 1056 and not GameWizard:isDo( GAMEWIZARD_FIRST_USEWEAPONITEM ) then
		-- ͬʱ���
		local containerEffectFrame = getglobal( "PlayerContainerFrameEffectFrame" );
		containerEffectFrame:Hide();
		GameWizard:doStep( GAMEWIZARD_FIRST_GETWEAPON );
		GameWizard:doStep( GAMEWIZARD_FIRST_USEWEAPONITEM );
		EquipFrame:Show();
	end

	if questId == 1064 and not GameWizard:isDo( GAMEWIZARD_FIRST_CURE_NPC ) then
		GameWizard:doStep( GAMEWIZARD_FIRST_CURE_NPC );
	end
	
	if questId == 1066 and not GameWizard:isDo( GAMEWIZARD_FIRST_QUEST_IF_YOU_CAN ) then
		GameWizard:doStep( GAMEWIZARD_FIRST_QUEST_IF_YOU_CAN )
	end
	
	if questId == 1072 and not GameWizard:isDo( GAMEWIZARD_FIRST_QUEST_SAVE_OTHERS ) then
		GameWizard:doStep( GAMEWIZARD_FIRST_QUEST_SAVE_OTHERS )
	end
end

-- �����ύ����
function SpringByQuestCommit( questId )
	SetFadeTaskNameShow( questId );
	local t_commitQuestNpcMap = 
	{
	-- ��Ԯ����	-- ������
	[1049] = { npcResId = 304001 },
	-- Σ���ķ�	-- ���ɶ�
	[1050] = { npcResId = 306011 },
	-- �±�ǩ��	-- ���ɶ�
	[1051] = { npcResId = 306011 },
	-- ��������	-- ����
	[1053]	= { npcResId = 303003 },
	};
	
	t_taskLimitControl:clearTaskLimit( { taskID = questId } );
	local npcData = t_commitQuestNpcMap[questId];
	if npcData ~= nil then
		local npcId	= ActorMgr:getActorIdByResId( npcData["npcResId"] );
		if npcId ~= 0 then
			GameWizard:delNpcTalkByActorId( npcId );
		end
	end

	-- ��һ���ύ����
	if questId == 1049 then
		if not GameWizard:isDo(GAMEWIZARD_FIRST_COMMMISSION) then
			SpringPlayIsDo(GAMEWIZARD_FIRST_COMMMISSION,1);
		end

		if TaskTraceFrameTaskTraceRich1Guide:IsShown() then
			TaskTraceFrameTaskTraceRich1Guide:Hide();
		end
	end

	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	ChangeMoney();
	local container = MainPlayer:getContainer();

	-- ��� ʵ����֤��  ������
	if questId == 1061 then
		GameWizard:delGroupActorsByResId( 350040 );
		GameWizard:doStep(GAMEWIZARD_FIRST_USE_NEW_SKILL);
	end

	-- ��һ���ύ����
	if not GameWizard:isDo(GAMEWIZARD_FIRST_SAWTASKLINK) and TaskTraceFrameTaskTraceRich1Guide:IsShown() then
		TaskTraceFrameTaskTraceRich1Guide:Hide();
		if questId == 1081 then
			GameWizard:doStep(GAMEWIZARD_FIRST_SAWTASKLINK);
		end
	end

	-- �ύ��ȡ��˵����񣬵�һ������
	if questId == 3132 and not GameWizard:isDo(GAMEWIZARD_FIRST_BLINK_SHORTCUT_RIDE_BTN) then
		local containerFrame = getglobal( "PlayerContainerFrame" );
		if containerFrame:IsShown() then
			ShowContainerGuide();
		else
			containerFrame:Show();
		end
	end

	-- �ύ��ȡ��������񣬵�һ�νӴ�����
	if questId == 3111 and not GameWizard:isDo(GAMEWIZARD_FIRST_GETXIAN_ITEM) then
		ShowMidTips("�����һֻ����");
		PlayerContainerFrame:Show();
	end

	-- ��һ�νӴ�ʳ��
	if questId == 1006 then
		if GameWizard:isDo(GAMEWIZARD_FIRST_GETFOOD) == false then
            SpringPlayIsDo(GAMEWIZARD_FIRST_GETFOOD,3);
		end
	end
	-- ��һ�λ�ü�����
	if questId == 1007 then
		if GameWizard:isDo(GAMEWIZARD_FIRST_GETSKILLBOOK) == false then
			SpringPlayIsDo(GAMEWIZARD_FIRST_GETSKILLBOOK,4);
		end
	end

	-- ��һ����Ʒ��
	if questId == 4000 then
		if  GameWizard:isDo(GAMEWIZARD_FIRST_PRESENT) == false then
			PlayIsDo(GAMEWIZARD_FIRST_PRESENT,2)
		end
	end

	-- ��һֻ����
	if questId == 1003 then

	end
end

-- �������
function SpringByQuestDrop( questId )
	local t_dropQuestNpcMap = 
	{
	-- Σ���ķ�	-- ���ɶ�
	[1050]	= { npcResId = 306011 },
	-- �±�ǩ��	-- ���ɶ�
	[1051]	= { npcResId = 306011 },
	-- ��������	-- ����
	[1053]	= { npcResId = 303003 },
	-- ׳־����	-- ����
	--[1060]	= { npcResId = 316000 },
	-- ���ε�ս��	-- ����
	--[1062]	= { npcResId = 302016 },
	-- ����Ⱦ�ĺ���	-- ��������
	[1064]	= { npcResId = 351000 },
	};
	
	t_taskLimitControl:clearTaskLimit( { taskID = questId } );
	local npcData = t_dropQuestNpcMap[questId];
	if npcData ~= nil then
		local npcId	= ActorMgr:getActorIdByResId( npcData["npcResId"] );
		if npcId ~= 0 then
			GameWizard:delNpcTalkByActorId( npcId );
		end
	end
	
	local t_taskDelActorPop = 
	{
	-- ��һ��ս��
	-- taskId  
	[1052] = { 241000, },
	[1058] = { 241002, },
	[1065] = { 231021, },
	[1069] = { 235000, },
	[1073] = { 250001, },
	[1080] = { 200008, },
	-- ��õ�һ�βɼ�����
	-- taskId  
	[1055] = { 293003, },
	[1059] = { 293002, },
	[1070] = { 293000, },
	[1078] = { 293001, },
	-- ʵ����֤��
	-- taskId  
	[1061] = { 350040, },
	};
	local t_chgActorShowEvent = t_taskDelActorPop[questId];
	if t_chgActorShowEvent ~= nil then
		for _, resId in ipairs( t_chgActorShowEvent ) do
			GameWizard:delGroupActorsByResId( resId );
		end
	end
end

function springByTalk2Npc( nNpcResId )
	local t_TalkNpc =
		{
			--{ 303003, GAMEWIZARD_FIRST_SEEMEDICASALER },
			{ 302025, GAMEWIZARD_FIRST_SEETEACHER	  },
			{ 306000, GAMEWIZARD_FIRST_SEETEACHER2	  },
			{ 303006, GAMEWIZARD_FIRST_SEECLANMANAGER },
			{ 303007, GAMEWIZARD_FIRST_SEEACTIMANAGER },
			{ 302001, GAMEWIZARD_FIRST_SEERIDESALER   },
			{ 304006, GAMEWIZARD_FIRST_SEEPETSALER	  },
			{ 310000, GAMEWIZARD_FIRST_SEEWEAPONSALER },
			{ 317000, GAMEWIZARD_FIRST_SEEDEFANDSALER },
			{ 303004, GAMEWIZARD_FIRST_SEETRANSPORTER },
			-- ��ɯ
			--{ 303021, GAMEWIZARD_FIRST_TALK_ZHUCHENG_TRAN },
			-- ����������
			{ 333003, GAMEWIZARD_FIRST_TALK_COPY_TRAN_DOOR },
			-- ����
			--{ 351000, GAMEWIZARD_FIRST_CURE_NPC },
		};
	for i = 1, table.getn( t_TalkNpc ) do
		if t_TalkNpc[i][1] == nNpcResId then
			if not GameWizard:isDo( t_TalkNpc[i][2] ) then
				GameWizard:doStep( t_TalkNpc[i][2] );
				GameWizard:hideNpcTalkByResId( t_TalkNpc[i][1] );
			end
		end
	end
	
	local mainplayer = ActorMgr:getMainPlayer();
	if nNpcResId == 303021 and mainplayer:getLv() >= 10 and Quest:isQuestRun( 1077 ) and 
		not GameWizard:isDo(GAMEWIZARD_FIRST_TALK_ZHUCHENG_TRAN ) then
		--GameWizard:doStep( GAMEWIZARD_FIRST_TALK_ZHUCHENG_TRAN );
		GameWizard:hideNpcTalkByResId( nNpcResId );
	end

	if nNpcResId == 306000 and not GameWizard:isDo( GAMEWIZARD_FIRST_SEE_STUDY_SKILL_NPC ) then
		GameWizard:hideNpcTalkByResId( nNpcResId );
	end

	if nNpcResId == 351000 then
		--GameWizard:doStep( GAMEWIZARD_FIRST_CURE_NPC );
		GameWizard:hideNpcTalkByResId( nNpcResId );

		if not GameWizard:isDo( GAMEWIZARD_FIRST_CURE_NPC ) and Quest:isQuestRun( 1064 ) then
			for i = 1, MAX_TRACE_NUM * 2 do
				local taskItemBtn = getglobal( "TaskItemBtn"..i );
				if taskItemBtn:IsShown() and taskItemBtn:GetClientUserData( 1 ) == 1064 then
					local xinShouBtn = getglobal( taskItemBtn:GetName().."GuideBtn" );
					local rich = getglobal( taskItemBtn:GetName().."GuideBtnRich" );
					local nSelfDefineExtendWidth = 10;
					local itemDef = getItemDef( taskItemBtn:GetClientUserData( 0 ) );
					local szText ="������ʹ��";
					rich:SetClientString( szText );
					ReSizeTooltip( rich:GetName(), xinShouBtn:GetName(), nSelfDefineExtendWidth );
					xinShouBtn:Show();
				end
			end
		end
	end
	
	local t_talkNpcWithPreCon = 
	{ 
	--[[
	-- ����  ������յ�����
	[302016] =	{	preCondition = GAMEWIZARD_FIRST_TRAN_TO_SKY_COPY, 
					postCondition = GAMEWIZARD_FIRST_TALK_TO_SKY_COPY_NPC 
				},
				
	
	-- ����  ��һ�η�������
	[316000] =	{	preCondition = GAMEWIZARD_FIRST_FLY_QUEST, 
					postCondition = GAMEWIZARD_FIRST_TALK_TO_FLY_QUEST_NPC 
				},
	--]]
	};
	for resId, data in pairs( t_talkNpcWithPreCon ) do
		if nNpcResId == resId then
			if GameWizard:isDo( data["preCondition"] ) and not GameWizard:isDo( data["postCondition"] ) then
				GameWizard:doStep( data["postCondition"] );
				GameWizard:hideNpcTalkByResId( resId );
			end
			break;
		end
	end
	--[[
	-- ��Ҫ�������Ƶ�
	local t_talkNpcWithTask = 
	{ 
	-- ����  
	[351000] =	{	taskID = 1064, preCondition = GAMEWIZARD_FIRST_CURE_NPC, postCondition = GAMEWIZARD_FIRST_CURE_NPC_AFTER_TALK,
postControl = 
function ()
	for i = 1, MAX_TRACE_NUM * 2 do
		local taskItemBtn = getglobal( "TaskItemBtn"..i );
		if taskItemBtn:IsShown() and taskItemBtn:GetClientUserData( 1 ) == 1064 then
			local xinShouBtn = getglobal( taskItemBtn:GetName().."GuideBtn" );
			local rich = getglobal( taskItemBtn:GetName().."GuideBtnRich" );
			local nSelfDefineExtendWidth = 10;
			local itemDef = getItemDef( taskItemBtn:GetClientUserData( 0 ) );
			local szText ="������ʹ��";
			rich:SetClientString( szText );
			ReSizeTooltip( rich:GetName(), xinShouBtn:GetName(), nSelfDefineExtendWidth );
			xinShouBtn:Show();
		end
	end
end
				},
	};
	
	for resId, data in pairs( t_talkNpcWithTask ) do
		if nNpcResId == resId and GameWizard:isDo( data["preCondition"] ) and not GameWizard:isDo( data["postCondition"] ) and
			Quest:isQuestRun( data["taskID"] ) then

			GameWizard:doStep( data["postCondition"] );
			GameWizard:hideNpcTalkByResId( resId );
			data.postControl();
		end
	end
	--]]
end

-- ��������
function SpringByLevelUp( Lv )
	--[[
	-- ��һ������
	if Lv == 5 then
		if GameWizard:isDo(GAMEWIZARD_FIRST_LEVELUP) == false then
			GameWizard:doStep(GAMEWIZARD_FIRST_LEVELUP);
			ShowMidTips("�������ܰ�ť");
			
			GuideEffectShow( "", "����򿪼��ܽ���", "SkillButton", "����򿪼��ܽ���\n#P��ݼ���K��", "uires\\ZhuJieMian\\1.dds", 352, 609, 60, 60, 50, 60, -9, 5, 7, -58, true );
		end
	end
	--]]
	SetPlayerLevelUpDialog( Lv );
	setLevelUpTips( Lv );
	SetGrowUp( Lv );
end

local bIsClanPwrldBegin = false;
function ClanPworldGameBegin( clanContri, clanMaxVal, clanMoney, clanVal, val, endTime, playerMoney, playerExp, showBoss )
	local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
	if t_centerXinShouBtnControl:getIndexByType( CENTER_CLAN_PWORLD ) == 0 then
		setClanPworldTips();
	end
	setClanPworldTipsFrameInfo( clanContri, clanMaxVal, clanMoney, clanVal, val, endTime, playerMoney, playerExp, showBoss );
	bIsClanPwrldBegin = true;
end
function ClanPworldGameEnd()
	local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
	local nIdx = t_centerXinShouBtnControl:getIndexByType( CENTER_CLAN_PWORLD );
	if nIdx > 0 then
		t_centerXinShouBtnControl:eraseByIndex( nIdx );
		t_centerXinShouBtnControl:updateCenterBtns();
	end
	local clanPworldTipFrame = getglobal( "ClanPworldTipFrame" );
	clanPworldTipFrame:Hide();
	bIsClanPwrldBegin = false;
end
--�����鱨����
local bIsClanSPYBegin = false;
function ClanSPYGameBegin()
	local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
	if t_centerXinShouBtnControl:getIndexByType( CENTER_CLAN_SPY ) == 0 then
		setClanSPYTips();
	end
	bIsClanSPYBegin = true;
end
function ClanSPYGameEnd()
	local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
	local nIdx = t_centerXinShouBtnControl:getIndexByType( CENTER_CLAN_SPY );
	if nIdx > 0 then
		t_centerXinShouBtnControl:eraseByIndex( nIdx );
		t_centerXinShouBtnControl:updateCenterBtns();
	end
	ClanIntellingenceFrame:Hide();
	if ChoseClanCityFrame:IsShown() or ChoseCityPosFrame:IsShown() then
		ChoseClanCityFrame:Hide();
		ChoseCityPosFrame:Hide();
	end
	bIsClanPwrldBegin = false;
end
function IsClanPworldBegin()
	return bIsClanPwrldBegin;
end
--����-�ػ�������Ŧ
function ClanMachinePworldBegin()
	local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
	if t_centerXinShouBtnControl:getIndexByType( CENTER_CLAN_MACHINE_PWORLD ) == 0 then
		SetClanMachinePworld();
	end
end
function ClanMachinePworldEnd()
	local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
	local nIdx = t_centerXinShouBtnControl:getIndexByType( CENTER_CLAN_MACHINE_PWORLD );
	if nIdx > 0 then
		t_centerXinShouBtnControl:eraseByIndex( nIdx );
		t_centerXinShouBtnControl:updateCenterBtns();
	end
	ClanMachinePworldFrame:Hide();
end

-- [memId] = { resId = ?, wizardOp = ? }, 
local TEMP_MAX_DIST = 1000;
local t_showTmpXinShouControl = {  };

function GetShowTmpXinShouControl()
	return t_showTmpXinShouControl;
end

t_showTmpXinShouControl["addTmpActor"] = 
function ( self, data )
	--print( "memId = "..data["memId"] );	
	table.insert( self, { resId = data["resId"], wizardOp = data["wizardOp"], memId = data["memId"], text = data["text"],
							xinShouType = data["xinShouType"] } );
end

t_showTmpXinShouControl["deleteTmpActor"] = 
function ( self, data )
end

function IsInTestMapShowClanPworldTime()
	local MainPlayer = ActorMgr:getMainPlayer()
	if MainPlayer ~= nil then
		if MainPlayer:getMapRealId() ==21900 then
			return true
		elseif MainPlayer:getMapRealId() == 25000 and isIterationStarted() then
			return true
		end
	end
	return false
end

t_showTmpXinShouControl["update"] = 
function ( self, data )
	local mainplayer = ActorMgr:getMainPlayer();
	local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
	for index, data in ipairs( self ) do
		--print( "memId = "..data["memId"] );
		local actor = ActorMgr:FindActor( data["memId"] );
		if actor ~= nil and util.CalActorsDistance( mainplayer, actor ) <= ( ( TEMP_MAX_DIST / 100 ) ^ 2 ) and not mainplayer:isInFly() then
			t_centerXinShouBtnControl:ShowExtraBtn( { itemDefId = data["resId"], Type = CENTER_XINSHOU_BTN_ITEM, 
														frame = "PworldSceneFrame", realMapID = MapShower:getRealMapId(),
														xinShouType = data["xinShouType"] } );
			GameWizard:doStep( data["wizardOp"] );
			local szMidText = data["text"];
			if szMidText ~= nil then
				ShowMidTips( szMidText );
			end
			
			table.remove( self, index );
		elseif actor == nil then
			table.remove( self, index );
		end
	end
	--��������ж��Ƿ���빫��ұ����
	if bIsClanPwrldBegin and GameClanManager:IsInClanPworldIsland() then
		ClanPworldIslandShowInfoFrame:Show();
	else
		ClanPworldIslandShowInfoFrame:Hide();		
	end
	--�ж��Ƿ��ڹ����ػ���Ŧ����
	if GameClanManager:IsInClanMachinePworld() or IsInTestMapShowClanPworldTime() then
		ClanMachinePworldTimeShowFrame:Show();
	else
		ClanMachinePworldTimeShowFrame:Hide();
		setIterationStarted(false);
	end
	--��������ж��Ƿ���빫��ݵ�ս����
	if GameClan_BaseWar:IsInBaseWarPworld() then
		ClanStrongWarResFrame:Show();
	else
		--����·��ݵ�Ư��ͼ��
		local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
		local nIdx = t_centerXinShouBtnControl:getIndexByType( CENTER_CLAN_STRONG_WAR );
		if nIdx > 0 then
			t_centerXinShouBtnControl:eraseByIndex( nIdx );
			t_centerXinShouBtnControl:updateCenterBtns();
		end
		ClanStrongWarResFrame:Hide();
	end
	--�ж��Ƿ��ڹ���ս��ͼ��
	if MapShower:getRealMapId() ~= WorldCityMapID then
		--����·�ʱ�������սƯ��ͼ��
		local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
		local nIdx = t_centerXinShouBtnControl:getIndexByType( CENTER_WORLD_CITY_WAR );
		if nIdx > 0 then
			t_centerXinShouBtnControl:eraseByIndex( nIdx );
			t_centerXinShouBtnControl:updateCenterBtns();
		end
	end
end
function SetClanPworldBegin( begin )
	bIsClanPwrldBegin = begin;
end

-- ���������Ұ����
function SpringByObjectEnter( objType, objId, actorId, dis )
	-- [npcResId] = {}
	local t_NpcAddTalk =
	{	
		-- ���ɶ�
		[306011] = {	resId = 306011, 
						content ={ [CLIENT_TASK_STAT_FINI] = "����������", [CLIENT_NORMAL_TASK_STAT_CAN_ACCEPT] = "�����ȡ����", },  
						popBackRich = "ChatPop", showOnce = true, headOffsetX = 12, headOffsetY = -100,
						arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
						wizardOp = { GAMEWIZARD_FIRST_FINISHQUEST_FROMUI, GAMEWIZARD_FIRST_GETQUEST_FROMUI },
						questId = 1050, 
				},
	
		-- ����
		[303003] = {	resId = 303003, 
						content = { [CLIENT_TASK_STAT_FINI] = "����ɹ�����Ʒ", [CLIENT_NORMAL_TASK_STAT_CAN_ACCEPT] = "����ɹ�����Ʒ", },  
						popBackRich = "ChatPop", showOnce = true, headOffsetX = 12, headOffsetY = -50,
						arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
						wizardOp = { GAMEWIZARD_FIRST_OPENSHOP }, questId = 1053, 
					},
	};
	
	local addNpcTalkData = t_NpcAddTalk[objId];
	if addNpcTalkData ~= nil then
		SetNpcQuestPopByState( addNpcTalkData );
	end
	
	
	local t_NpcWithoutQuestAddTalk =
	{
		-- ����������	GAMEWIZARD_FIRST_SEE_COPY_TRAN_DOOR
		[333003] = {	resId = 333003, 
						content = "���������Կ��ٴ��ͳ�����", 
						popBackRich = "ChatPop", showOnce = true, headOffsetX = 12, headOffsetY = 0,
						arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0,
						wizardOp = { GAMEWIZARD_FIRST_SEE_COPY_TRAN_DOOR }, preConditionCheck = function () return true; end
					},

		-- ����ѧϰԱ	GAMEWIZARD_FIRST_SEE_STUDY_SKILL_NPC
		[306000] = {	resId = 306000, 
						content = "�������ѧϰ����", 
						popBackRich = "ChatPop", showOnce = true, headOffsetX = 12, headOffsetY = -50,
						arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0,
						wizardOp = { GAMEWIZARD_FIRST_SEE_STUDY_SKILL_NPC }, preConditionCheck = function () return true; end
					}
	};
	
	local addNpcWithoutQuestTalkData = t_NpcWithoutQuestAddTalk[objId];
	if addNpcWithoutQuestTalkData ~= nil and addNpcWithoutQuestTalkData.preConditionCheck() then
		SetNpcWithoutQuestPopByState( addNpcWithoutQuestTalkData );
	end
	
	local t_NpcWithQuestAddTalk =
	{
		-- ��ɯ
		[303021] = {	resId = 303021, 
						content = "    ������Դ��͵�ʱ���", 
						popBackRich = "ChatPop", showOnce = true, headOffsetX = 12, headOffsetY = -50,
						arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0,
						wizardOp = { GAMEWIZARD_FIRST_TALK_ZHUCHENG_TRAN },
						preConditionCheck = function ()
												local mainplayer = ActorMgr:getMainPlayer();
												return mainplayer:getLv() >= 10;
											end
					},
	};
	-- ���͵�ʱ��ǵ�NPC
	local addNpcWithQuestTalkData = t_NpcWithQuestAddTalk[objId];
	if objId == 303021 and not GameWizard:isDo(GAMEWIZARD_FIRST_TALK_ZHUCHENG_TRAN ) and 
			addNpcWithQuestTalkData.preConditionCheck() and Quest:isQuestRun( 1077 ) then
			SetNpcWithoutQuestPopByState( t_NpcWithQuestAddTalk[objId] );
	end
	
	-- ��һ�μ���������
	-- �ڻ��	��Ԯ����	��ʱ��	
	if objId == 304001 and not GameWizard:isDo(GAMEWIZARD_FIRST_GETMISSION ) and not GameMgr:isInPlayCamerAnim() then
		EndFirstEnterGameAnim();
	end

	-- ��һ�μ���ʱ��Ǵ��ʹ�ʹ
	if objId == 333003 and not GameWizard:isDo(GAMEWIZARD_FIRST_SEE_COPY_TRAN_DOOR) then
		GameWizard:doStep(GAMEWIZARD_FIRST_SEE_COPY_TRAN_DOOR);
	end

	local systemSetting	= GameMgr:getUserSystemConfig();
	
	local pickKey			= GetKeyMap("ʰȡ������Ʒ")
	if pickKey == 0 then
		pickKey = nil
	end
	if pickKey then
		pickKey = KeyToString(pickKey)
	end
	local mainplayer		= ActorMgr:getMainPlayer();
	local nSkillID			= t_firstGetSkill[GetInitRace( mainplayer:getRace() )]["skillID"];
	local SkillDef			= SkillMgr:getSpellStaticAtt( nSkillID,1 );
	local szMuZhuangPaoPao	= "�����ʹ�á�"..SkillDef.Name .."������";
	local t_monsterAddTalk	= 
	{
		-- ������
		--[1460001] = {	resId = 1460001, 
		--				content = "������������Ի�������޴�ļ���",  
		--				popBackRich = "ChatPop", showOnce = true,  headOffsetX = 10, headOffsetY = -70,
		--				arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
		--				wizardOp = GAMEWIZARD_FIRST_SAW_JIJIASHOU, questId = 0, 
		--			},

		-- ��̨
		--[1460002] = {	resId = 1460002, 
		--				content = "��������̨�����Ի�������޴�ļ���",  
		--				popBackRich = "ChatPop", showOnce = true,  headOffsetX = 10, headOffsetY = -20,
		--				arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
		--				wizardOp = GAMEWIZARD_FIRST_SAWPAOTAI, questId = 0, 
		--			},
		
		-- ����
		--[1400002] = {	resId = 1400002, 
		--				content = "������ʰȡ���ף����Ի�������޴�ļ���",  
		--				popBackRich = "ChatPop", showOnce = true,  headOffsetX = 10, headOffsetY = -20,
		--				arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
		--				wizardOp = GAMEWIZARD_FIRST_SAW_SHOULEI, questId = 0, 
		--			},

		-- ����
		[241000] = {	resId = 241000, 
						content = ( systemSetting:getGameOpt( GAME_OPT_SWAP_MOUSE ) and "���" or "�Ҽ�" ) .."������﹥��",  
						popBackRich = "ChatPop", showOnce = true,  headOffsetX = 10, headOffsetY = -20,
						arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
						wizardOp = GAMEWIZARD_FIRST_FIGHT,
					},
		[241002] = {	resId = 241002, 
						content = ( systemSetting:getGameOpt( GAME_OPT_SWAP_MOUSE ) and "���" or "�Ҽ�" ) .."������﹥��",   
						popBackRich = "ChatPop", showOnce = true,  headOffsetX = 10, headOffsetY = -20,
						arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
						wizardOp = GAMEWIZARD_FIRST_FIGHT, 
					},
		[231021] = {	resId = 231021, 
						content = ( systemSetting:getGameOpt( GAME_OPT_SWAP_MOUSE ) and "���" or "�Ҽ�" ) .."������﹥��",   
						popBackRich = "ChatPop", showOnce = true,  headOffsetX = 10, headOffsetY = -20,
						arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
						wizardOp = GAMEWIZARD_FIRST_FIGHT, 
					},
		[235000] = {	resId = 235000, 
						content = ( systemSetting:getGameOpt( GAME_OPT_SWAP_MOUSE ) and "���" or "�Ҽ�" ) .."������﹥��",   
						popBackRich = "ChatPop", showOnce = true,  headOffsetX = 10, headOffsetY = -20,
						arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
						wizardOp = GAMEWIZARD_FIRST_FIGHT, 
					},
		[250001] = {	resId = 250001, 
						content = ( systemSetting:getGameOpt( GAME_OPT_SWAP_MOUSE ) and "���" or "�Ҽ�" ) .."������﹥��",   
						popBackRich = "ChatPop", showOnce = true,  headOffsetX = 10, headOffsetY = -20,
						arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
						wizardOp = GAMEWIZARD_FIRST_FIGHT, 
					},
		[200008] = {	resId = 200008, 
						content = ( systemSetting:getGameOpt( GAME_OPT_SWAP_MOUSE ) and "���" or "�Ҽ�" ) .."������﹥��",   
						popBackRich = "ChatPop", showOnce = true,  headOffsetX = 10, headOffsetY = -20,
						arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
						wizardOp = GAMEWIZARD_FIRST_FIGHT, 
					},
		
		-- ���Ͽ�ʰȡ����
		--[9900000] = {	resId = 9900000, 
		--				content = pickKey and string.format("������ʰȡ����ʹ�ÿ�ݼ�(%s)����ʰȡ", pickKey) or "������ʰȡ",
		--				popBackRich = "ChatPop", showOnce = true,  headOffsetX = 10, headOffsetY = -60,
		--				arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
		--				wizardOp = GAMEWIZARD_FIRST_GETBOOTY,
		--			},

		-- ��һ�βɼ�����ľ�ʯ
		[293003] = {	resId = 293003, 
						content = "����������ҿɲɼ�",  
						popBackRich = "ChatPop", showOnce = true,  headOffsetX = 10, headOffsetY = 0,
						arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
						wizardOp = GAMEWIZARD_FIRST_COLLECT,
					},
		[293002] =	{	resId = 293002, 
						content = "����������ҿɲɼ�",  
						popBackRich = "ChatPop", showOnce = true,  headOffsetX = 10, headOffsetY = 0,
						arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
						wizardOp = GAMEWIZARD_FIRST_COLLECT,
					},
		[293000] =	{	resId = 293000, 
						content = "����������ҿɲɼ�",  
						popBackRich = "ChatPop", showOnce = true,  headOffsetX = 10, headOffsetY = 0,
						arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
						wizardOp = GAMEWIZARD_FIRST_COLLECT,
					},
		[293001] =	{	resId = 293001, 
						content = "����������ҿɲɼ�",  
						popBackRich = "ChatPop", showOnce = true,  headOffsetX = 10, headOffsetY = 0,
						arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
						wizardOp = GAMEWIZARD_FIRST_COLLECT,
					},

		-- ��һ���η���
		[351000] = {	resId = 351000, 
						content = "�Һ����ܣ��������ң����������ң�����ʹ��ҩƷ",  
						popBackRich = "ChatPop", showOnce = true,  headOffsetX = 10, headOffsetY = 0,
						arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
						wizardOp = GAMEWIZARD_FIRST_CURE_NPC,
					},
		
		-- ľ׮
		[350040] = {	resId = 350040, 
						content = szMuZhuangPaoPao,  
						popBackRich = "ChatPop", showOnce = true,  headOffsetX = 10, headOffsetY = 50/GetScreenScaleY(),
						arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
						wizardOp = GAMEWIZARD_FIRST_USE_NEW_SKILL,
					},
	};


	--�ڲ��ſ���������ʱ�򣬲����д���
	if GameMgr:isInPlayCamerAnim() then
		return;
	end

	-- �ڽӵ���һ��ս�������������һ�μ�������
	if not GameWizard:isDo(GAMEWIZARD_FIRST_FIGHT) then
		if objId == 241000 and Quest:getQuestRunState( 1052 ) == CLIENT_NORMAL_TASK_STAT_CAN_ACCEPT then
			SetMonsterQuestPopByState( { config = t_monsterAddTalk[objId], ActorId = actorId } );
		elseif objId == 241002 and Quest:getQuestRunState( 1058 ) == CLIENT_NORMAL_TASK_STAT_CAN_ACCEPT then
			SetMonsterQuestPopByState( { config = t_monsterAddTalk[objId], ActorId = actorId } );
		elseif objId == 231021 and Quest:getQuestRunState( 1065 ) == CLIENT_NORMAL_TASK_STAT_CAN_ACCEPT then
			SetMonsterQuestPopByState( { config = t_monsterAddTalk[objId], ActorId = actorId } );
		elseif objId == 235000 and Quest:getQuestRunState( 1069 ) == CLIENT_NORMAL_TASK_STAT_CAN_ACCEPT then
			SetMonsterQuestPopByState( { config = t_monsterAddTalk[objId], ActorId = actorId } );
		elseif objId == 250001 and Quest:getQuestRunState( 1073 ) == CLIENT_NORMAL_TASK_STAT_CAN_ACCEPT then
			SetMonsterQuestPopByState( { config = t_monsterAddTalk[objId], ActorId = actorId } );
		elseif objId == 200008 and Quest:getQuestRunState( 1080 ) == CLIENT_NORMAL_TASK_STAT_CAN_ACCEPT then
			SetMonsterQuestPopByState( { config = t_monsterAddTalk[objId], ActorId = actorId } );
		end
	end
	
	-- ��һ��ʹ���¼����������
	if objId == 350040 and ( not GameWizard:isDo(GAMEWIZARD_FIRST_USE_NEW_SKILL) ) and Quest:isQuestRun( 1061 ) then
		SetMonsterQuestPopByState( { config = t_monsterAddTalk[objId], ActorId = actorId } );
	end

	-- ��һ�����Ʒ���
	if objId == 351000 and not GameWizard:isDo(GAMEWIZARD_FIRST_CURE_NPC) and Quest:isQuestRun( 1064 ) then
		SetMonsterQuestPopByState( { config = t_monsterAddTalk[objId], ActorId = actorId } );
	end

	-- �������
	--local actor = ActorMgr:FindActor( actorId );
	--if actor:getType() == GAT_ITEM and ( not GameWizard:isDo(GAMEWIZARD_FIRST_GETBOOTY) ) then
	--	local itemDef = getItemDef(actor:getResId() );
	--	if itemDef.ItemTmp.Type == 0 then
	--		SetMonsterQuestPopByState( { config = t_monsterAddTalk[9900000], ActorId = actorId } );
	--	end
	--end
	if objType == GAT_ITEM then
		local itemDef = getItemDef( objId )
		if itemDef.ItemTmp.Type ~= 0 then
			ShowTmpItemPickButton( actorId )
		end
	end

	-- ��һ�βɼ�����ľ�ʯ
	if not GameWizard:isDo( GAMEWIZARD_FIRST_COLLECT ) then
		if objId == 293003 and Quest:getQuestRunState( 1055 ) == CLIENT_NORMAL_TASK_STAT_CAN_ACCEPT then
			SetMonsterQuestPopByState( { config = t_monsterAddTalk[objId], ActorId = actorId, line = 1087 } );
		elseif objId == 293002 and Quest:getQuestRunState( 1059 ) == CLIENT_NORMAL_TASK_STAT_CAN_ACCEPT then
			SetMonsterQuestPopByState( { config = t_monsterAddTalk[objId], ActorId = actorId, line = 1087 } );
		elseif objId == 293000 and Quest:getQuestRunState( 1070 ) == CLIENT_NORMAL_TASK_STAT_CAN_ACCEPT then
			SetMonsterQuestPopByState( { config = t_monsterAddTalk[objId], ActorId = actorId, line = 1087 } );
		elseif objId == 293001 and Quest:getQuestRunState( 1078 ) == CLIENT_NORMAL_TASK_STAT_CAN_ACCEPT then
			SetMonsterQuestPopByState( { config = t_monsterAddTalk[objId], ActorId = actorId, line = 1087 } );
		end
	end

	-- ���浶������Ұ	���浶  1054 ��һ�νӴ�
	local t_showTmpXinShouControl = GetShowTmpXinShouControl();
	local typeId = math.floor( objId/1000 )
	if typeId == 1420 then		
		local t_config =  t_monsterAddTalk[objId];
		local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();

		if not GameWizard:isDo( GAMEWIZARD_FIRST_SAWSWORD ) and Quest:isQuestRun( 1054 ) then
			ShowMidTips( "�������������һ�ѻ��浶" );
			--[[
			if t_config ~= nil then
				t_config["wizardOp"] = GAMEWIZARD_FIRST_SAWSWORD;
				SetMonsterQuestPopByState( { config = t_config, ActorId = actorId } );
			end
			GameWizard:doStep(GAMEWIZARD_FIRST_SAWSWORD);
			
			if not GameWizard:isDo( GAMEWIZARD_FIRST_POP_SWORD_XINSHOU_HELP_BTN ) then
				t_showTmpXinShouControl:addTmpActor( { memId = actorId, resId = objId, wizardOp = GAMEWIZARD_FIRST_POP_SWORD_XINSHOU_HELP_BTN,
														xinShouType = XINSHOU_HUO_YAN_DAO1_TYPE } );
			end
			]]--

			TmpItemPickTipsFrame:SetClientUserData( 0,actorId )
			TmpItemPickTipsFrame:Show();
			
		elseif not GameWizard:isDo( GAMEWIZARD_FIRST_PICKSWORD ) and Quest:isQuestRun( 1054 ) then
			--[[
			if t_config ~= nil then
				t_config["wizardOp"] = GAMEWIZARD_FIRST_PICKSWORD;
				SetMonsterQuestPopByState( { config = t_config, ActorId = actorId } );
			end
			if not GameWizard:isDo( GAMEWIZARD_FIRST_POP_SWORD_XINSHOU_HELP_BTN ) then
				t_showTmpXinShouControl:addTmpActor( { memId = actorId, resId = objId, wizardOp = GAMEWIZARD_FIRST_POP_SWORD_XINSHOU_HELP_BTN,
														xinShouType = XINSHOU_HUO_YAN_DAO1_TYPE } );
			end
			]]--
			TmpItemPickTipsFrame:SetClientUserData( 0,actorId )
			TmpItemPickTipsFrame:Show();

		elseif not GameWizard:isDo( GAMEWIZARD_SECOND_SAWSWORD ) and Quest:isQuestRun( 1062 )  then
			--[[
			if t_config ~= nil then
				t_config["wizardOp"] = GAMEWIZARD_SECOND_SAWSWORD;
				SetMonsterQuestPopByState( { config = t_config, ActorId = actorId } );
			end
			--ShowMidTips( "��յ��������һ�ѻ��浶" );
			GameWizard:doStep(GAMEWIZARD_SECOND_SAWSWORD);

			if not GameWizard:isDo( GAMEWIZARD_SECOND_POP_SWORD_XINSHOU_HELP_BTN ) then
				t_showTmpXinShouControl:addTmpActor( { memId = actorId, resId = objId, wizardOp = GAMEWIZARD_SECOND_POP_SWORD_XINSHOU_HELP_BTN,
														text = "��յ��������һ�ѻ��浶", xinShouType = XINSHOU_HUO_YAN_DAO2_TYPE } );
			end
			]]--

			TmpItemPickTipsFrame:SetClientUserData( 0,actorId )
			TmpItemPickTipsFrame:Show();

		elseif not GameWizard:isDo( GAMEWIZARD_SECOND_PICKSWORD ) and Quest:isQuestRun( 1062 ) then
			--[[
			if t_config ~= nil then
				t_config["wizardOp"] = GAMEWIZARD_SECOND_PICKSWORD;
				SetMonsterQuestPopByState( { config = t_config, ActorId = actorId } );
			end

			if not GameWizard:isDo( GAMEWIZARD_SECOND_POP_SWORD_XINSHOU_HELP_BTN ) then
				t_showTmpXinShouControl:addTmpActor( { memId = actorId, resId = objId, wizardOp = GAMEWIZARD_SECOND_POP_SWORD_XINSHOU_HELP_BTN,
														text = "��յ��������һ�ѻ��浶", xinShouType = XINSHOU_HUO_YAN_DAO2_TYPE } );
			end
			]]--

			TmpItemPickTipsFrame:SetClientUserData( 0,actorId )
			TmpItemPickTipsFrame:Show();
		end
	end

	-- ��̨������Ұ		��̨    3322 ��ɽ��-�ڻ�	and Quest:isQuestRun( 3322 )
	-- TODO: ��Щ��ʱ���߷ֱ����Щ�����	
	if objId == 1460002 and ( not GameWizard:isDo(GAMEWIZARD_FIRST_SAWPAOTAI) ) then
		--SetMonsterQuestPopByState( { config = t_monsterAddTalk[objId], ActorId = actorId } );
		ShowMidTips( "��ɽ���г���һ��"..getItemDef( objId ).Name );
		GameWizard:doStep(GAMEWIZARD_FIRST_SAWPAOTAI);
		local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
		
		if not GameWizard:isDo( GAMEWIZARD_FIRST_POP_PAOTAI_XINSHOU_HELP_BTN ) then
			t_showTmpXinShouControl:addTmpActor( { memId = actorId, resId = objId, wizardOp = GAMEWIZARD_FIRST_POP_PAOTAI_XINSHOU_HELP_BTN,
													xinShouType = XINSHOU_DA_PAO_TYPE } );
		end
	elseif objId == 1460002 and not GameWizard:isDo( GAMEWIZARD_FIRST_PICKPAOTAI ) then
		local t_config = t_monsterAddTalk[objId];
		if t_config ~= nil then
			t_config["wizardOp"] = GAMEWIZARD_FIRST_PICKPAOTAI;
			--SetMonsterQuestPopByState( { config = t_config, ActorId = actorId } );
			if not GameWizard:isDo( GAMEWIZARD_FIRST_POP_PAOTAI_XINSHOU_HELP_BTN ) then
				t_showTmpXinShouControl:addTmpActor( { memId = actorId, resId = objId, wizardOp = GAMEWIZARD_FIRST_POP_PAOTAI_XINSHOU_HELP_BTN,
														xinShouType = XINSHOU_DA_PAO_TYPE } );
			end
		end
	end

	-- ���׽�����Ұ
	if objId == 1400002 and not GameWizard:isDo(GAMEWIZARD_FIRST_SAW_SHOULEI) then
		--SetMonsterQuestPopByState( { config = t_monsterAddTalk[objId], ActorId = actorId } );
		GameWizard:doStep(GAMEWIZARD_FIRST_SAW_SHOULEI);
		local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
		
		if not GameWizard:isDo( GAMEWIZARD_FIRST_POP_SHOULEI_XINSHOU_HELP_BTN ) then
			t_showTmpXinShouControl:addTmpActor( { memId = actorId, resId = objId, wizardOp = GAMEWIZARD_FIRST_POP_SHOULEI_XINSHOU_HELP_BTN,
													xinShouType = XINSHOU_SHOU_LEI_TYPE } );
		end
	elseif objId == 1400002 and not GameWizard:isDo( GAMEWIZARD_FIRST_PICKSHOULEI ) then
		local t_config = t_monsterAddTalk[objId];
		t_config["wizardOp"] = GAMEWIZARD_FIRST_PICKSHOULEI;
		--SetMonsterQuestPopByState( { config = t_config, ActorId = actorId } );
		if not GameWizard:isDo( GAMEWIZARD_FIRST_POP_SHOULEI_XINSHOU_HELP_BTN ) then
			t_showTmpXinShouControl:addTmpActor( { memId = actorId, resId = objId, wizardOp = GAMEWIZARD_FIRST_POP_SHOULEI_XINSHOU_HELP_BTN,
													xinShouType = XINSHOU_SHOU_LEI_TYPE } );
		end
	end
	
	--[[
	-- �����޽�����Ұ	������  1075 ��ɽ����
	if objId == 1460001 and not GameWizard:isDo(GAMEWIZARD_FIRST_SAW_JIJIASHOU) and Quest:isQuestRun( 1075 ) then
		--SetMonsterQuestPopByState( { config = t_monsterAddTalk[objId], ActorId = actorId, line = 1118 } );
		ShowMidTips( "��ջ����г���һֻ������" );
		GameWizard:doStep(GAMEWIZARD_FIRST_SAW_JIJIASHOU);
		local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
		
		if not GameWizard:isDo( GAMEWIZARD_FIRST_POP_JIJIA_XINSHOU_HELP_BTN ) then
			t_showTmpXinShouControl:addTmpActor( { memId = actorId, resId = objId, wizardOp = GAMEWIZARD_FIRST_POP_JIJIA_XINSHOU_HELP_BTN,
													xinShouType = XINSHOU_JI_JIA_TYPE } );
		end
	elseif objId == 1460001 and not GameWizard:isDo(GAMEWIZARD_FIRST_PICKMACHINE) and Quest:isQuestRun( 1075 ) then
		local t_config = t_monsterAddTalk[objId];
		t_config["wizardOp"] = GAMEWIZARD_FIRST_PICKMACHINE;
		--SetMonsterQuestPopByState( { config = t_config, ActorId = actorId } );
		if not GameWizard:isDo( GAMEWIZARD_FIRST_POP_JIJIA_XINSHOU_HELP_BTN ) then
			t_showTmpXinShouControl:addTmpActor( { memId = actorId, resId = objId, wizardOp = GAMEWIZARD_FIRST_POP_JIJIA_XINSHOU_HELP_BTN,
													xinShouType = XINSHOU_JI_JIA_TYPE } );
		end
	end
	]]--

	-- ��������ǰ���
	-- �������
	local actor		= ActorMgr:FindActor( actorId );
	local szBtnName = actor:getName()..actor:GetID().."��UI����";
	if actor:needAddButtonName() and not isAlreadyExistFrame( szBtnName ) then
		CreateButton( szBtnName, "DropPackageHeadButtonTemplate", nil, true );
		local font	= FindFontString( szBtnName.."NameFont" );
		local rollTex = FindTexture( szBtnName .. "RollSign" );
		local btn	= FindButton( szBtnName );
		local color = tonumber(getItemColor(0), 16);
		local item	= getItemDef(actor:getRealId());
		if item then
			color = tonumber(getItemColor(item.Important,item.SuitSetID), 16);
		end
		font:SetTextColor(math.floor(color / 65536), math.floor((color % 65536) / 256), color % 256);
		local text;
		if actor:getItemNum() < 2 then
			text = actor:getName();
		else
			text = string.format("%sx%d", actor:getName(), actor:getItemNum())
		end
		local nArmStar = actor:getArmStar();
		if nArmStar > 0 then
			text = text.." +"..nArmStar;
		end
		font:SetText( text );
		btn:SetClientUserData( 0, actor:GetID() );
		if actor:canPickUp() then
			btn:Enable();
			EnableDropPackageHeadButton(btn, true);
		else
			btn:Disable();
			EnableDropPackageHeadButton(btn, false);
		end
		if actor:isRollItem() then
			local offset = ( btn:GetWidth() - font:GetTextExtentWidth(font:GetText()) ) / 2
			rollTex:SetPoint( "topright",szBtnName,"topleft",offset,0 )
			rollTex:Show();
		else
			rollTex:Hide();
		end
	end
	
	CreateStallBtn( actor );

	local t_StallBuyFrameBiaoJiStalls = GetStallBuyFrameBiaoJiStalls();
	if t_StallBuyFrameBiaoJiStalls:isInBiaoJi( { name = actor:getName() } ) then
		actor:playMotion( "9622_16" );
	end
end

function HideStallBtn( actor, bColor )

	local szStallBtnName = actor:getName()..actor:GetID().."��̯λ��ͷ��UI";
	local btn	= FindButton( szStallBtnName );
	local font	= FindFontString( szStallBtnName.."Font" );
	
	if btn == nil then
		return;
	end
	if bColor then 
		font:SetTextColor( 255,255,190 );
	end
	btn:Hide();
end

function CreateStallBtn( actor )

	local szStallBtnName = actor:getName()..actor:GetID().."��̯λ��ͷ��UI";
	if not ( actor:getType() == GAT_PLAYER or actor:getType() == GAT_MAINPLAYER ) then
		return;
	end

	if not actor:isInStall() then
		return;
	end

	if isAlreadyExistFrame( szStallBtnName ) then
		local btn	= FindButton( szStallBtnName );
		local font	= FindFontString( szStallBtnName.."Font" );	
		font:SetText( actor:getStallName()  );
		btn:Show();
		return;
	end

	CreateButton( szStallBtnName, "StallActorHeadButtonTemplate", nil, true );
	local font	= FindFontString( szStallBtnName.."Font" );
	local tex	= FindTexture( szStallBtnName.."Tex" );	
	local btn	= FindButton( szStallBtnName );
	font:SetText( actor:getStallName() );
	tex:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" )
	btn:SetClientUserData( 0, actor:GetID() );
	btn:Show();
end

function OnItemChangeOwner(id)
	local actor	= ActorMgr:FindActor(id)
	local btn	= FindButton(actor:getName()..id.."��UI����")
	if not btn then
		return
	end
	local rollTex = FindTexture( btn:GetName() .. "RollSign" )
	if not btn then
		return
	end
	local rollTex = FindTexture( btn:GetName() .. "RollSign" );

	if actor:canPickUp() then
		btn:Enable()
		EnableDropPackageHeadButton(btn, true)
	else
		btn:Disable()
		EnableDropPackageHeadButton(btn, false)
	end
	if actor:isRollItem() then
		rollTex:Show();
	else
		rollTex:Hide();
	end

	-- �������
	if actor:getType() == GAT_ITEM and ( not GameWizard:isDo(GAMEWIZARD_FIRST_GETBOOTY) ) and actor:canPickUp() then
		local itemDef = getItemDef(actor:getResId() );
		if itemDef.ItemTmp.Type == 0 then
			local pickKey			= GetKeyMap("ʰȡ������Ʒ")
			if pickKey == 0 then
				pickKey = nil
			end
			if pickKey then
				pickKey = KeyToString(pickKey)
			end
			SetMonsterQuestPopByState( { config = {	resId = 9900000, 
														content = pickKey and string.format("������ʰȡ����ʹ�ÿ�ݼ�(%s)����ʰȡ", pickKey) or "������ʰȡ",
														popBackRich = "ChatPop", showOnce = true,  headOffsetX = 10, headOffsetY = -60,
														arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
														wizardOp = GAMEWIZARD_FIRST_GETBOOTY,
													}, ActorId = id } );
		end
	end
end

function SetNpcQuestPopByState( data )
	local npcId				= ActorMgr:getActorIdByResId( data["resId"] );
	local nQuestState		= getNpcQuestShow( data["resId"] );
	
	if npcId ~= 0 then
		local szText = data["content"][nQuestState];
		if szText == nil then
			return;
		end
		
		for _, wizardOp in ipairs( data["wizardOp"] ) do
			GameWizard:setNpcTalkByActorId( npcId, szText, data["popBackRich"], data["showOnce"], data["headOffsetX"],
									data["headOffsetY"], data["arrowTexture"], data["showArrowTex"], data["texOffsetX"], data["texOffsetY"], 
									wizardOp );
		end
	end
end

function FirstGetGenus()
	if GameWizard:isDo(GAMEWIZARD_FIRST_GETGENIUSOPT) == false then
		GameWizard:doStep(GAMEWIZARD_FIRST_GETGENIUSOPT);
		GuideEffectShow( "", "������츳����", "DiathesisButton", "������һ���츳����������", "uires\\ZhuJieMian\\1.dds", 352, 609, 60, 60, 50, 60, -9, 5, 7, -58, true );
	end
end

function PickTmpItem( nDefId, nActorId )
	if nDefId == 1420000 then	-- ���浶
		 FirstPickSword( nActorId );
	elseif nDefId == 1460001 then	-- ������
		 FirstPickJiJia( nActorId );
	elseif nDefId == 1460002 then	-- ��̨
		 FirstPickPaoTai( nActorId );
	elseif nDefId == 1400002 then	-- ����
		 FirstPicShouLei( nActorId );
	end
end

function FirstPickTmpItemByDefID( nDefId )
	local typeId = math.floor( nDefId/1000 )
	if typeId == 1420 then	-- ���浶
		 FirstPickSwordByDefID( nDefId );
	elseif nDefId == 1460001 then	-- ������
		 FirstPickJiJiaByDefID( nDefId );
	elseif nDefId == 1460002 then	-- ��̨
		 FirstPickPaoTaiByDefID( nDefId );
	elseif nDefId == 1400002 then	-- ����
		 FirstPickPaoTaiByDefID( nDefId );
	end
end

function FirstPickSwordByDefID( nDefId )
	if not GameWizard:isDo(GAMEWIZARD_FIRST_PICKSWORD) and Quest:isQuestRun( 1054 ) then
		GameWizard:doStep(GAMEWIZARD_FIRST_PICKSWORD);
		GameWizard:delGroupActorsByResId( nDefId );
	elseif not GameWizard:isDo(GAMEWIZARD_SECOND_PICKSWORD) and Quest:isQuestRun( 1062 ) then
		GameWizard:doStep(GAMEWIZARD_SECOND_PICKSWORD);
		GameWizard:delGroupActorsByResId( nDefId );
	end
end

function FirstPickJiJiaByDefID( nDefId )
	if not GameWizard:isDo(GAMEWIZARD_FIRST_PICKMACHINE) then
		GameWizard:doStep(GAMEWIZARD_FIRST_PICKMACHINE);
		GameWizard:delGroupActorsByResId( nDefId );
	end
end

function FirstPickPaoTaiByDefID( nDefId )
	if not GameWizard:isDo(GAMEWIZARD_FIRST_PICKPAOTAI) then
		GameWizard:doStep(GAMEWIZARD_FIRST_PICKPAOTAI);
		GameWizard:delGroupActorsByResId( nDefId );
	end
end

function FirstPickPaoTaiByDefIDFirstPickPaoTaiByDefID( nDefId )
	if not GameWizard:isDo(GAMEWIZARD_FIRST_PICKSHOULEI) then
		GameWizard:doStep(GAMEWIZARD_FIRST_PICKSHOULEI);
		GameWizard:delGroupActorsByResId( nDefId );
	end
end

function FirstPickPaoTai( id )
	if not GameWizard:isDo(GAMEWIZARD_FIRST_PICKPAOTAI) then
		GameWizard:doStep(GAMEWIZARD_FIRST_PICKPAOTAI);
		GameWizard:delActorTalkByActorId( id );
	end
end

function FirstPicShouLei( id )
	if not GameWizard:isDo(GAMEWIZARD_FIRST_PICKSHOULEI) then
		GameWizard:doStep(GAMEWIZARD_FIRST_PICKSHOULEI);
		GameWizard:delActorTalkByActorId( id );
	end
end

function FirstPickSword( id )
	if not GameWizard:isDo(GAMEWIZARD_FIRST_PICKSWORD) and Quest:isQuestRun( 1054 ) then
		GameWizard:doStep(GAMEWIZARD_FIRST_PICKSWORD);
		GameWizard:delActorTalkByActorId( id );
	elseif not GameWizard:isDo(GAMEWIZARD_SECOND_PICKSWORD) and Quest:isQuestRun( 1062 ) then
		GameWizard:doStep(GAMEWIZARD_SECOND_PICKSWORD);
		GameWizard:delActorTalkByActorId( id );
	end
end

function FirstPickJiJia( id )
	if not GameWizard:isDo(GAMEWIZARD_FIRST_PICKMACHINE) then
		GameWizard:doStep(GAMEWIZARD_FIRST_PICKMACHINE);
		GameWizard:delActorTalkByActorId( id );
	end
end

function SetNpcWithoutQuestPopByState( data )
	local npcId	= ActorMgr:getActorIdByResId( data["resId"] );
	if npcId ~= 0 then
		for _, wizardOp in ipairs( data["wizardOp"] ) do
			GameWizard:setNpcTalkByActorId( npcId, data["content"], data["popBackRich"], data["showOnce"], data["headOffsetX"],
									data["headOffsetY"], data["arrowTexture"], data["showArrowTex"], data["texOffsetX"], data["texOffsetY"], 
									wizardOp );
		end
	end
end

function SetMonsterQuestPopByState( data )
	local config = data["config"];
	
	GameWizard:setActorTalkByActorId( data["ActorId"], config["content"], config["popBackRich"], config["showOnce"], 
										config["headOffsetX"], config["headOffsetY"], config["arrowTexture"], config["showArrowTex"], config["texOffsetX"], config["texOffsetY"], config["wizardOp"] );
end

-- ѧϰ���ܴ���
function SpringByStudySkill( skillId )
end

-- ���볡������
function SpringByEnterMap( mapId )
	-- ��һ����ʱ����
	if mapId == 50200 then
		if GameWizard:isDo(GAMEWIZARD_FIRST_LIMITMISSION) == false then
			SpringPlayIsDo(GAMEWIZARD_FIRST_LIMITMISSION,5);
		end
	end

	if mapId == 1000 then
		if GameWizard:isDo(GAMEWIZARD_FIRST_LOGINTIMECITY) == true then
			GameWizard:undoStep(GAMEWIZARD_FIRST_LOGINTIMECITY);
			local mainplayer = ActorMgr:getMainPlayer();
			if mainplayer == nil then return end
			addEndPoint(mainplayer:GetID(),103);
		end
	end

	SetEnterMapDialog( mapId );
end

-- ʹ����Ʒ
function SpringUseContainerItem( itemid,listType,gridIdx )
	local itemDef = getItemDef( itemid )
	if itemDef.Result[0].ResultID == RESULT_MACHINE_ITEM and listType == CONTAINER_TYPE_TASK then
		if not GameWizard:isDo(GAMEWIZARD_FIRST_BLINK_JIJIASHOU_CONTAINER_BTN) then
			GameWizard:doStep(GAMEWIZARD_FIRST_BLINK_JIJIASHOU_CONTAINER_BTN);
			ShowContainerGuide();
		end
	end
end

function EffectFrame_OnLoad()
	this:RegisterEvent("GE_GET_GENIUS_POINT");
	this:setUpdateTime( 0 );
end

function EffectFrame_OnEvent()
	if (arg1 == "GE_GET_GENIUS_POINT") then
		FirstGetGenus();
	end
end

-- �������ָ��tips
function XinShouGuideTipsCloseBtn_OnClick()
	local frame = getglobal( this:GetParent() );
	frame:Hide();
end

local t_guideEvents = {		-- �л�Ŀ�����
							--["GE_TARGET_CHANGED"]	= {},
							-- ʰȡ���Ϸ���ʱ��Ʒ
							["GE_BOOTY_GETITEM"]	= {},
							["GE_BOOTY_GET_ALL_ITEM"]	= {},
							-- ����
							["UI_ACTOR_LEVEL_CHG"]	= {},
							-- �뿪����
							["GE_LEAVE_PWORLD"]	= {},
							};

--[[
t_guideEvents["GE_TARGET_CHANGED"].func = 
function ()
	local mainplayer	= ActorMgr:getMainPlayer();
	local nTargetId		= mainplayer:GetSelectTarget();
	if nTargetId == 0 then
		return;
	end

	local TargetObj	= ActorMgr:FindActor( nTargetId );
end
--]]

function EndWizardCollect( actorId )
end

t_guideEvents["GE_BOOTY_GETITEM"].func = 
function ()
	-- ��Ϊ����ʰȡ����ʱ����
	if not GameWizard:isDo( GAMEWIZARD_FIRST_GETBOOTY ) then
		GameWizard:doStep( GAMEWIZARD_FIRST_GETBOOTY );
		-- TODO: ��ʵ����ȥ����
		GameWizard:delAllDropItemPop( );
	end
end

t_guideEvents["GE_BOOTY_GET_ALL_ITEM"].func = 
function ()
	--t_guideEvents["GE_BOOTY_GETITEM"].func();
end


t_guideEvents["UI_ACTOR_LEVEL_CHG"].func =
function ()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer:getLv() >= 10 and Quest:isQuestRun( 1077 ) then
		local config = {	resId = 303021, 
							content = "    ������Դ��͵�ʱ���", 
							popBackRich = "ChatPop", showOnce = true, headOffsetX = 12, headOffsetY = -50,
							arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0,
							wizardOp = GAMEWIZARD_FIRST_TALK_ZHUCHENG_TRAN,
						};
		GameWizard:setGroupActorsByResId( config["resId"], config["content"], config["popBackRich"], config["showOnce"], 
										config["headOffsetX"], config["headOffsetY"], config["arrowTexture"], config["showArrowTex"], config["texOffsetX"], config["texOffsetY"], config["wizardOp"] );
	end
end

t_guideEvents["GE_LEAVE_PWORLD"].func = 
function ()
	local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
	t_centerXinShouBtnControl:ClearOneTypetXinShouData( { Type = CENTER_XINSHOU_BTN_ITEM } );
	t_centerXinShouBtnControl:updateCenterBtns();
	
	local t_hideIcons = { "PworldSceneFrame", };
	util.HideIcons( t_hideIcons );
end

function GuideFrame_OnLoad()
	for event, _ in pairs( t_guideEvents ) do
		this:RegisterEvent( event );
	end

	t_guideEvents.__index = function ( tab, key )
								tab[key] = { func = function () end };
								return tab[key];
							end
	setmetatable( t_guideEvents, t_guideEvents );
end

function GuideFrame_OnEvent()
	t_guideEvents[arg1].func();
end
 
function PlayerFirstEnterWorld()	 
	--webframe:SetPoint( "center", "$parent", "center", 0,0 );
	--webframe:setLocalFile( "../resource/Helper/html/1.html" );
	--webframe:SetSize(498,409);

	PlayerFirstMove();
end

--��ҵ�һ�ν�����Ϸ
function PlayerFirstBorn()
	if not GameWizard:isDo(GAMEWIZARD_FIRST_UI) then
		GameWizard:doStep(GAMEWIZARD_FIRST_UI);
		--GuideFrame:Show();
		SelectCameraModeGuideFrame:Show()
	end


	if GameWizard:isDo(GAMEWIZARD_FIRST_FLASHHELP) == false then
		--GuideQuestion_BackgroundBtn:Show();
	end

end

function GuideFrame_OnShow()
	-- #c34B511\		5D3818	 52,181,17 );
	GuideFrameTalkText:SetText("    �����˵ĳ��ְ�����������𣬵�ĩ�յ���Ӱ��������������縺����������������ʹ������Խ����Ǻ���������Ƭ��Ϥ�����صĴ�½��\n    ̤��δ֪���ó̰ɣ�����ϣ�����㽵����ˡ�����ʷ�ĳ���ɢȥ���㽫��Ϊ���˴��̵�Ӣ�ۣ�\n\n", 93,56,24 );
	GuideFrameTalkText:AddText( "���������������������ƶ�����ס������Ҽ�����ת�ӽǡ�", 1,129,249 );

	local xinShouBtn = getglobal( this:GetName() .. "XinShouBtn" );
	local rich	 = getglobal( xinShouBtn:GetName() .. "Rich" );
	local nSelfDefineExtendWidth = 10;
	rich:SetClientString( "�����ť������Ϸ" );
	ReSizeTooltip( rich:GetName(), xinShouBtn:GetName(), nSelfDefineExtendWidth );
	
	local animationTexture = getglobal( xinShouBtn:GetName().."UVAnimationTex" );
	animationTexture:SetUVAnimation( 50, true );
	animationTexture:Show();
	xinShouBtn:Show();
end

function GuideFrame_OnUpdate()
	local MaxMapControl = MapShower:GetMaxMapContal();
	if MaxMapControl:getMapID() ~= BIRTH_MAP_ID then
		this:Hide();
	end
end

function GuideFrame_OnHide()
end

function GuideConfirmBtn_OnClick()
	GuideFrameCloseBtn_OnClick();
end

-- ��ȡ��Ӧ��������Ϣ
function GetDesc(nstep)
    for i = 1, table.getn(guide_enum)  do
        if guide_enum[i] == nstep then
            return guide_desc[i];
        end
    end
    return ""
end


-- ������ִ�й�����
function SetIsDo()
	endpage = 0;
	for i=1, table.getn(guide_enum) do
		 if GameWizard:isDo(guide_enum[i]) then
			 endpage = endpage + 1;
			 curpage = endpage;
			 guide_isdo[endpage] = GetDesc(guide_enum[i]);
		 end
	end
	countpage = curpage;
	if curpage <= table.getn(guide_enum) then
		--GuideIcoFrame:Show();
	end
end

-- ���õ�ǰҳ
function SetCurPage()
	endpage = 0;
    for i=1, table.getn(guide_isdo) do
		endpage = endpage + 1;
		curpage = endpage;
		GuideFrameTalkText:SetText(guide_isdo[i], nGuideColorR, nGuideColorG, nGuideColorB);
		GuideFrameTalkText:SetFontType( FONTSYTLE_NORMAL );
		GuideFramePageText:SetText(endpage.."/"..endpage);
	end
end


-- ���÷�ҳ
function SetPage(curpage)
	GuideFramePageText:SetText(curpage.."/"..endpage);
	GuideFrameTalkText:SetText(guide_isdo[curpage], nGuideColorR, nGuideColorG, nGuideColorB);
	GuideFrameTalkText:SetFontType( FONTSYTLE_NORMAL );
end

function GuideFirstBtn_OnClick()
	curpage = 1;
	SetPage(curpage);
end

function GuideLeftBtn_OnClick()
	if curpage ~= 1 then
		curpage = curpage -1;
		SetPage(curpage);
	end
end

function GuideRigthBtn_OnClick()
	if curpage < endpage then
		curpage = curpage +1;
		SetPage(curpage);
	end
end

function GuideLastBtn_OnClick()
	curpage = endpage;
	SetPage(curpage);
end

function GuideFrameCloseBtn_OnClick()
    GuideFrame:Hide();
    GuideFrameXinShouBtn:Hide();

	--MapShower:GetMaxMapContal():linkMoveto(2000, 14839, 21938 );
	NpcDialog:talkByResId( 304001 );
	--ShowPhotomFrameXinShou();
end

function GuideChkBtn_OnClick()
	if  GameWizard:isDo(GAMEWIZARD_CLOSEALL) == false then
		--GuideFrame:Hide();
		--GuideIcoFrame:Show();
		--GameWizard:doStep(GAMEWIZARD_CLOSEALL);
	end
end

-- ����ָ��Сͼ��
function GuideIcoFrameOnClick()
	--SetCurPage();
    --GuideFrame:Show();
end

local xpstart =0;
function XPEffectFrame_OnShow()
	XPEffectFrame_ModelView:addBackground( "effect\\\\entity\\\\xpranshao.ent" );
	xpstart = os.clock();
end

function XPEffectFrame_OnUpdate()

end

-- FlyScript
local bShowTalk			= false;
local nTalkLastTime		= 0;
local nTalkStartTime	= 0;
local nActorId			= 0;

local t_FlyMonsterTalk =
	{
		"������ǰ��һ���Գ�Ϊ������������Ѹ�����𣬴����Ӷ�Զ�����",
		"����Ϊ�ﵽĿ�Ĳ����ֶΣ���������ĳ�����ص������������ٻ�Զ�ŵĿֲ����",
		"�����ǵĿ������������Ż��������������������Ǳ����뿪�Լ��ļ��磬",
		"���Ǹ��ڰ��������������������Ѫ���ƽ�������� ",
		"ӵ����Ԫ�غͷ�Ԫ�ص�����ҲͶ���ںڰ�����֮�¡�����Ȼ�Ǳ��ȵġ���",
		"��ˮԪ�رӻ��µľ������ƿػ�Ԫ�ص��������������������ǿ�ĵֿ���",
		"����Ϊ����ӭ����һƬ��Ϣ֮����Ȼ�����������ߡ��ĵ���ȴ����������ݵĺ�ƽ��",
		"�����������Ƚ��ĿƼ�������������������˵ص׵Ķ�ħ�������չ�����һƬ�𺣣�",
		"Ϊ�����棬������������Դ�ںϽ��е�����Ԫ�أ������˱����ɵ�ħ��������½�ָ��ڲ�ͬ�Ľ���ڣ�",
		"��Ϊһ��#Race#�����ְ����ǻ�����Щ�Ѿ����������Ǳ���ڰ��Եĵ��ˣ�����������ս������ͬ������һ��������δ������",
	};

function SetOnHorseId( nId, indexId )
	local MainPlayer = ActorMgr:getMainPlayer();
	if indexId ~= 1 or MainPlayer == nil then
		return;
	end
	if nId == MainPlayer:GetID() then
		bShowTalk	= true;
		nActorId	= nId;
		nTalkStartTime	= os.clock();
	end
end

function AutoFlyMonsterTalk()
	if ( os.clock() - nTalkStartTime ) > 68 then
		return;
	end

	local t_timeInterval = { 3, 10, 17, 24, 31, 38, 45, 52, 59, 66 };
	if  ( os.clock() - nTalkStartTime ) > t_timeInterval[math.ceil( ( os.clock() - nTalkStartTime ) / 7.3 )] - 0.1 then
		GameWizard:setFlyMonsterTalk( nActorId, ReplaceQuestDesc(t_FlyMonsterTalk[math.ceil( ( os.clock() - nTalkStartTime ) / 7.3 )] ), "ChatPop","PopBackArrowTexture" );
	end
end

function GuideUpdateFrame_OnUpdate()
	if getGameState() ~= STATE_INWORLD then return end 
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end
	-- �ж��Ƿ���ʾ�����԰�
	if bShowTalk then
		AutoFlyMonsterTalk();
	end

	if os.difftime ( os.clock(), nAwardLastTime) >= 1 then
		nAwardLastTime = os.clock(); --���¼�ʱ
		nCurAwardTime = nCurAwardTime - 1;
	end

	if nCurAwardTime == 0 then
		--GuideAward_BackgroundBtn:Show();
		nCurAwardTime = -1;
		nAwardLastTime = 0;
		AwardLeaveTime:Hide();
	elseif nCurAwardTime > 0 then 
		local nMin		= os.date( "%M", nCurAwardTime );
		local nSecond	= os.date( "%S", nCurAwardTime );
		AwardLeaveTimeFont:SetText("��ȴ� "..nMin..":"..nSecond);
		GuideAward_BackgroundBtn:Hide();
	end
end


local t_firstArm = { [1] = { desc = "սʿ", armId = 1019901 }, [2] = { desc = "��ʦ", armId = 1109901 }, 
					[3] = { desc = "ǹ��", armId = 1209901 }, [4] = { desc = "��˾", armId = 1509901 }, };

function GuideFirstArm()
	return t_firstArm;
end

function GuideCameraAnimFrame_OnLoad()
	GuideCameraAnimTop_Background:SetColor( 255, 68, 45 );
	GuideCameraAnimBottom_Background:SetColor( 255, 68, 45 );
end

function GuideQuestion_BackgroundBtn_OnShow()
	GuideEffectShow( "GuideQuestion_BackgroundBtn","","GuideQuestion_BackgroundBtn", "", "uires\\ZhuJieMian\\1.dds", 284, 602, 67, 67, 67, 67, -12, 16, 34, -52, false );
end

function GuideAward_BackgroundBtn_OnShow()
	GuideEffectShow( "GuideAward_BackgroundBtn","","GuideAward_BackgroundBtn", "", "uires\\ZhuJieMian\\1.dds", 284, 602, 67, 67, 67, 67,  -15, 16, 34, -52, false );
end

function GuideQuestion_BackgroundBtn_OnEnter()
	GameTooltipRichText:SetWidth(150);
	SetGameTooltips(this:GetName(), "������ɻ�ð�����Ϣ" );
end

function GuideAward_BackgroundBtn_OnEnter()
	GameTooltipRichText:SetWidth(150);
	SetGameTooltips(this:GetName(), "2012Ϊ��������һЩ���������������ȡ" );
end

function GuideQuestion_BackgroundBtn_OnClick()
	if  GameWizard:isDo(GAMEWIZARD_FIRST_MOVE) == false then
		GameWizard:doStep(GAMEWIZARD_FIRST_MOVE);
		curStep = GAMEWIZARD_FIRST_MOVE;
		--webframe:SetPoint( "center", "$parent", "center", 0,0 );
		--webframe:setLocalFile( "../resource/Helper/html/1.html" );
		--webframe:SetSize(498,409);
		ShowWebFrame();
	end
	GameWizard:doStep(GAMEWIZARD_FIRST_FLASHHELP);
	GuideQuestion_BackgroundBtn:Hide();
end

function GetAwardInfo( nAwardTime, nAwardId )
	if getGameState() ~= STATE_INWORLD then return end
	nCurAwardId = nAwardId;
	if nAwardTime == 0 and nAwardId ~= 0 then
		--GuideAward_BackgroundBtn:Show();
	elseif nAwardTime > 0 and nAwardId ~= 0 then
		nCurAwardTime	= nAwardTime;
		nAwardLastTime	= os.clock(); --���¼�ʱ
		local nMin		= os.date( "%M", nAwardTime );
		local nSecond	= os.date( "%S", nAwardTime );
		AwardLeaveTimeFont:SetText("��ȴ� "..nMin..":"..nSecond);
		AwardLeaveTime:Show();
	end
end

function GuideAward_BackgroundBtn_OnClick()
	if( arg1 == "LeftButton" ) then
		GameWizard:sendAwardRequest( nCurAwardId );
		PlayerContainerFrame:Show();
		GuideAward_BackgroundBtn:Hide();
	end
end

local bGuideRecond	= false;
local nGuideLastTime = 0;
function GuideQuestion_BackgroundBtnEffectFrame_OnUpdate()
	
end

local bAwardRecond	= false;
local nAwardLastTime = 0;
function GuideAward_BackgroundBtnEffectFrame_OnUpdate()
	
end

function GuideAwardFrame_OnShow()
	GuideQuestion_BackgroundBtn:Hide();
	GuideAward_BackgroundBtn:Hide();
	AwardLeaveTime:Hide();
end

function GuideAwardFrame_OnLoad()
	GuideQuestion_BackgroundBtnEffectFrame_Texture:SetUVAnimation(80, true); 
	GuideAward_BackgroundBtnEffectFrame_Texture:SetUVAnimation(50, true);
	PlayerContainerFrameEffectFrame_Texture:SetUVAnimation(50, true);
end

--////////////////////////////////////////////////////////////////////////////////////////////////////////
--------------------------------------------�������------------------------------------------------------
--////////////////////////////////////////////////////////////////////////////////////////////////////////

local t_LabelNameList =	{};		--��ǩ����
local szTitleColor		= "#c431D09";
local szContentColor	= "#c6f6149";

local MAX_LV_GIFT			= 32; 	--/* �����㽱������ */
local MAX_ONEDAY_GIFT		= 8; 	--/* ÿ�ս��������� */
local MAX_CATEGORY_NUM		= 20;
local CAREER_MAX			= 4;
local ONEDAY_GIFT_ITEM_MAX	= 8;
local bShowGiftClock		= false;
local nLeavingTM			= 0;
local szCurTableName		= "";
local nCurSelIndex			= 0;
local nCurSelType			= 0;
local nCurSelSubType		= 0;  
CAN_NOT_GET_GIFT	= 0;
CAN_GET_GIFT		= 1;
ALREADY_GET_GIFT	= 2;
--���ñ�ǩҳ����
function BondingGiftLabelData()
	t_LabelNameList = {};
	GuideGiftFrame_:Show()
	GuideGiftFramePropGiftBtn:Hide()
	GuideGiftFrameCheckInGiftBtn:Hide();
	if GuideGiftFrame_CycleBtn:IsChecked() then
		BondingOneDayGiftData();
		szCurTableName = "OneDayGift"
	elseif GuideGiftFrame_GrowBtn:IsChecked() then
		BondingLVGiftData();
		szCurTableName = "LVGift"
	elseif GuideGiftFrame_ChargeBtn:IsChecked() then
		BindPropGiftData();
		szCurTableName = ""
	elseif GuideGiftFrame_CDKeyBtn:IsChecked() then
		BindCDKeyGiftData();
		szCurTableName = "CodeGift"
	elseif GuideGiftFrame_AccGiftBtn:IsChecked() then
		BindAccGiftData();
		szCurTableName = "AccGift"
	elseif GuideGiftFrame_PlayGiftBtn:IsChecked() then
		BindPlayGiftData();
		szCurTableName = ""
	elseif GuideGiftFrame_KitBtn:IsChecked() then
		BindKitData();
		szCurTableName= ""
	elseif GuideGiftFrame_CheckInBtn:IsChecked() then
		BindCheckInGiftData();
		szCurTableName= "CheckIn";		
	end
	if GuideGiftFrame:IsShown() then
		UpdateGiftCategoryList( 1 );
	end
end

function InitGiftData()
	bShowGiftClock		= false;
	nLeavingTM			= 0;
	szCurTableName		= "";
	nCurSelIndex		= 0;
	nCurSelType			= 0;
	nCurSelSubType		= 0;
	GuideGiftFrameDisCheckAllBtn();
	GameWizard:InitGiftData();
end

-- nAwardState: 0 ������ȡ 1 ����ȡ 2 ����ȡ 
function BondingOneDayGiftData()
	local nNum = GameWizard:getOneDayGiftNum();
	local nSubType = 1;
	local bFound = false;
	for i = 1, nNum do
		local onedaygift	= GameWizard:getOneDayGiftDef( i - 1 );
		local nRet			= -1;
		local nOnLineTm		= 0;
		for _, data in ipairs(t_LabelNameList) do
			if data.nType == onedaygift.GiftType then
				bFound = true;
			end
		end
		if not bFound then
			table.insert(t_LabelNameList,{ name=onedaygift.TypeName, bCheck = true, nType = onedaygift.GiftType, nSubType = 0, nDataIndex = 0, szTable = "", nAwardState = 0, id = onedaygift.GiftID });
		end
	
		if onedaygift.GiftCond.OnlineTM ~= 0 then
			nOnLineTm = onedaygift.GiftCond.OnlineTM;
		end

		nRet = CanGetAward( onedaygift.GiftCond.LVMin, nOnLineTm, onedaygift.GiftID, "OneDayGift" );
		table.insert(t_LabelNameList,{ name=onedaygift.Name, bCheck=true, nType=onedaygift.GiftType, nSubType=nSubType, nDataIndex = i, szTable = "OneDayGift", nAwardState = nRet, id = onedaygift.GiftID } );
		nSubType = nSubType + 1;
	end
	local ContOnlineInfo = GameWizard:getContOnlineInfo();
	local bHave = false;
	for i=1,ContOnlineInfo.Num do
		local info = ContOnlineInfo.ContOnlineInfo[i-1];
		local giftDef = GameWizard:getContOnlineGiftDef( info.GiftID );
		if giftDef ~= nil then
			if not bHave then
				table.insert(t_LabelNameList,{name="�������߸���",bCheck=true, nType=CLIENT_GIFT_TYPE_CONTI,nSubType=0,nDataIndex=0,szTable="",nAwardState=0 });
				bHave = true;
			end
			table.insert( t_LabelNameList,{name=giftDef.Name,bCheck=true,nType=CLIENT_GIFT_TYPE_CONTI,nSubType=nSubType,nDataIndex=info.GiftID,szTable="ContOnlineGiftDef",nAwardState=info.IsGift } );			
			nSubType = nSubType + 1;
		end
	end
	local nNum = GameWizard:getContiNum();
	for i=1,nNum do
		local ContiGroupData = GameWizard:getContiData( i-1 );
		if ContiGroupData ~= nil then
			local giftGroupDef = GameWizard:getOnlineGiftGroupDef( ContiGroupData.ID );
			local giftOne = GameWizard:getGiftInfo( ContiGroupData.ID,ContiGroupData.Day );
			if giftGroupDef ~= nil and giftOne ~= nil then
				if not bHave then
					table.insert(t_LabelNameList,{name="�������߸���",bCheck=true, nType=CLIENT_GIFT_TYPE_CONTI_GROUP,nSubType=0,nDataIndex=0,szTable="",nAwardState=0 });
					bHave = true;
				end
				table.insert( t_LabelNameList,{name=giftGroupDef.Name,bCheck=true,nType=CLIENT_GIFT_TYPE_CONTI_GROUP,nSubType=nSubType,nDataIndex=i,szTable="OnlineGiftGroupDef",nAwardState=CAN_GET_GIFT } );			
				nSubType = nSubType + 1;
			end
		end
	end
end

function GetLabelNameList()
	return t_LabelNameList;
end

function CanGetLVGiftDataNum()
	local nCanGetNum = 0;
	for _, data in ipairs( t_LabelNameList ) do
		if data["szTable"] == "LVGift" and data["nAwardState"] == CAN_GET_GIFT then
			nCanGetNum = nCanGetNum + 1;
		end
		if data["szTable"] == "AccGift" and data["nAwardState"] == CAN_GET_GIFT then
			nCanGetNum = nCanGetNum + 1;
		end
	end
	return nCanGetNum;
end

function BondingLVGiftData()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return
	end
	
	local nNum = GameWizard:getLVGiftNum();
	local nSubType = 1;
	local bSet = false;
	--���⸣��
	for i = 1, nNum do
		local lvgift = GameWizard:getLVGiftDef( i - 1 );
		local nRet = -1;
		if not bSet then
			table.insert(t_LabelNameList,{name="�ȼ�����",bCheck = true,nType = CLIENT_GIFT_TYPE_LV, nSubType = 0, nDataIndex = 0, szTable = "", nAwardState = nRet});
			bSet = true;
		end
		nRet = CanGetAward( lvgift.LV, 0, 0, "LVGift" );
		if lvgift.LV == 0 then
			if GameWizard:CanShowPreCreateGift() then
				if GameWizard:CanGetPreCreateGift() then
					nRet = CAN_GET_GIFT;
				else
					nRet = ALREADY_GET_GIFT;
				end
			--	table.insert(t_LabelNameList,{ name=lvgift.Name, bCheck=true, nType=CLIENT_GIFT_TYPE_LV, nSubType=nSubType, nDataIndex = i, szTable = "LVGift", nAwardState = nRet, lv = lvgift.LV } );
			end
		else
			table.insert(t_LabelNameList,{ name=lvgift.Name, bCheck=true, nType=CLIENT_GIFT_TYPE_LV, nSubType=nSubType, nDataIndex = i, szTable = "LVGift", nAwardState = nRet, lv = lvgift.LV } );
		end
		nSubType = nSubType + 1;
	end
	-- ��Ȩ�������
	if GameWizard:IsVipNetBarUser() then
		local nNum = GameWizard:GetVipNetBarGiftNum();
		if nNum > 0 then
			table.insert( t_LabelNameList,{name="��Ȩ�������",bCheck = true,nType = CLIENT_GIFT_TYPE_ONEDAY, nSubType = 0, nDataIndex = 0, szTable = "VipNetBarGift", nAwardState = -1} )
		end
		for i=1,nNum do
			local BarGift = GameWizard:GetVipNetBarGift( i-1 );
			local nRet = CanGetAward( i-1, 0, 0, "VipNetBarGift" )
			table.insert(t_LabelNameList,{ name=BarGift.Name, bCheck=true, nType=CLIENT_GIFT_TYPE_ONEDAY, nSubType=nSubType, nDataIndex = i, szTable = "VipNetBarGift", nAwardState = nRet, lv = 0 } );
			nSubType = nSubType + 1;
		end
	end	
end

--��ֵ��� ���´���
function BindPropGiftData()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return
	end
	GuideGiftFrame_:Hide()
	GuideGiftFramePropGiftBtn:Show()
	GuideGiftFrameBackBtnAwardNameFont:SetText("");

	local propGift = GameWizard:getPropGiftInfo();
	local nSubType = 1;
	if propGift.DefCount > 0 then
		GuideGiftFramePropGiftBtnOkBtn:Show();
		table.insert( t_LabelNameList,{name="��ֵ���",bCheck = true,nType=CLIENT_GIFT_TYPE_PROPGIFT, nSubType = 0, nDataIndex = 0, szTable = "", nAwardState = -1} );
		for i=1,propGift.DefCount do
			if propGift.PropsDef[propGift.DefCount-i].DesHead ~= "" and propGift.PropsDef[propGift.DefCount-i].DesTail ~= "" then
				local nRet = -1;
				local result = CanGetPropGift( propGift.PropsDef[propGift.DefCount-i].GroupID );
				if result == 0 then
					nRet = CAN_NOT_GET_GIFT;
				elseif result == 1 then
					nRet = ALREADY_GET_GIFT;
				elseif result == 99 then
					nRet = CAN_GET_GIFT;
				end
				--�����
				table.insert( t_LabelNameList,{name=propGift.PropsDef[propGift.DefCount-i].Name, bCheck=true, nType=CLIENT_GIFT_TYPE_PROPGIFT, nSubType=nSubType, nDataIndex = propGift.DefCount - i + 1, szTable = "��ֵ���", nAwardState = nRet} )
				nSubType = nSubType + 1;
			end
		end
	else
		table.insert( t_LabelNameList,{name="��ֵ���",bCheck=true, nType=CLIENT_GIFT_TYPE_PROPGIFT,nSubType=0,nDataIndex=0,szTable="",nAwardState=0 } )
		GuideGiftFramePropGiftBtn:Hide();
	end
end

function BindCDKeyGiftData()
	table.insert(t_LabelNameList,{name="����",bCheck=true, nType=CLIENT_GIFT_TYPE_CDKEY,nSubType=0,nDataIndex=0,szTable="",nAwardState=0 });
	table.insert(t_LabelNameList,{name="������Ȩ����ȡ",bCheck=true, nType=CLIENT_GIFT_TYPE_CDKEY,nSubType=1,nDataIndex=1,szTable="CodeGift",nAwardState=0 });
	table.insert(t_LabelNameList,{name="���������ȡ",bCheck=true, nType=CLIENT_GIFT_TYPE_CDKEY,nSubType=2,nDataIndex=2,szTable="CodeGift",nAwardState=0 });
	table.insert(t_LabelNameList,{name="���������ȡ",bCheck=true, nType=CLIENT_GIFT_TYPE_CDKEY,nSubType=3,nDataIndex=3,szTable="CodeGift",nAwardState=0 });
end

function BindPlayGiftData()
	table.insert(t_LabelNameList,{name="���������ȡ",bCheck=true, nType=CLIENT_GIFT_TYPE_CDKEY,nSubType=0,nDataIndex=0,szTable="",nAwardState=0 });
end

function BindKitData()
	table.insert(t_LabelNameList,{name="���������ȡ",bCheck=true, nType=CLIENT_GIFT_TYPE_CDKEY,nSubType=0,nDataIndex=0,szTable="",nAwardState=0 });
end

function BindAccGiftData()
	table.insert( t_LabelNameList,{name="��Ӫ���",bCheck=true, nType=CLIENT_GIFT_TYPE_ACCGIFT,nSubType=0,nDataIndex=0,szTable="",nAwardState=0 } )
	local accGiftInfo = GameWizard:getAccGiftInfo();
	local nSubType = 1;
	for i=1,accGiftInfo.Count do
		local nRet = -1;
		if accGiftInfo.AccGifts[i-1].Flag == ACCGIFT_CAN_GET_TOLUA then
			nRet = CAN_GET_GIFT;
			table.insert(t_LabelNameList,{ name=accGiftInfo.AccGifts[i-1].Name, bCheck=true, nType=CLIENT_GIFT_TYPE_ACCGIFT, nSubType=nSubType, nDataIndex = accGiftInfo.AccGifts[i-1].ID, szTable = "AccGift", nAwardState = nRet } );
			nSubType = nSubType + 1;
		end
	end
end
function BindCheckInGiftData()
	table.insert(t_LabelNameList,{name="ǩ������",bCheck=true, nType=CLIENT_GIFT_TYPE_CONT_COVER,nSubType=0,nDataIndex=0,szTable="",nAwardState=0 });
	table.insert(t_LabelNameList,{name="����ǩ������",bCheck=true, nType=CLIENT_GIFT_TYPE_CONT_COVER,nSubType=1,nDataIndex=1,szTable="CheckIn",nAwardState=0 });
	table.insert(t_LabelNameList,{name="ÿ�ճɳ�����",bCheck=true, nType=CLIENT_GIFT_TYPE_CONT_COVER,nSubType=2,nDataIndex=2,szTable="CheckIn",nAwardState=0 });
--	SetCheckInGiftInfo();
end
function CanGetAward( nlv, nTime, nGiftId, szTable )
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end

	local playerlv = MainPlayer:getLv();
	if playerlv < nlv then
		return CAN_NOT_GET_GIFT;
	end

	local rolegiftinfo = GameWizard:getRoleGiftInfo();
	if szTable == "LVGift" then
		for i = 1, MAX_LV_GIFT do
			--print("playerlv"..playerlv.."nlv"..nlv.."rolegiftinfo.GiftLvList[i-1]"..rolegiftinfo.GiftLvList[i-1].Lv);
			if nlv == rolegiftinfo.GiftLvList[i-1].Lv then
				return ALREADY_GET_GIFT;
			end
		end
		return CAN_GET_GIFT;
	elseif szTable == "OneDayGift" then
		if nTime ~= 0 and GameWizard:getLeavingTime(nTime*60) > 0 then
			return CAN_NOT_GET_GIFT; 
		end
		for i = 1, rolegiftinfo.OneDayNum do
			if nGiftId == rolegiftinfo.GiftInfoOneDay[i-1].GiftID and GameWizard:isToday( rolegiftinfo.GiftInfoOneDay[i-1].GiftTM ) then
				return ALREADY_GET_GIFT;
			end
		end
		return CAN_GET_GIFT;
	elseif szTable == "VipNetBarGift" then
		if GameWizard:IsVipNetBarUser() then			
			local roleGiftInfo = GameWizard:getRoleGiftInfo();
			local barGift = GameWizard:GetVipNetBarGift( nlv );			
			for i=1,roleGiftInfo.OneDayNum do
				if roleGiftInfo.GiftInfoOneDay[i-1].GiftID == barGift.GiftID then
					return ALREADY_GET_GIFT
				end
			end
			local onTime = LoginMgr:getTodayOnTime();
			if onTime >= barGift.GiftCond.ContOnlineTM * 60 then
				return CAN_GET_GIFT;
			else
				return CAN_NOT_GET_GIFT;
			end
		end
	end
end

--��ȡ��ǩҳ���ϼ���ǩ
function GetGiftCategoryParentNodeInfo( childNodeInfo )
	if childNodeInfo.level == 1 then
		return;
	end
	for i = 1, table.getn( t_LabelNameList ) do
		if t_LabelNameList[i].nSubType== 0 and t_LabelNameList[i].nType == childNodeInfo.nType then
			return t_LabelNameList[i];
		end
	end
end

--ˢ�±�ǩҳ
function UpdateGiftCategoryList( nStartContentIndex )
	local nCurShowBtn			= 1; -- ��Ϊָ��ť��ָ��
	local t_selHilightBtnInfo	= { haveSel = false, initCurSelIndex = nCurSelIndex };
	local nCurHilightBtnIndex	= 0;
	local nSubLabelNum = 1;
	local bFound = false;
	for nContentIndex = nStartContentIndex, table.getn( t_LabelNameList ) do
		-- ��鰴ťָ���Ƿ�Խ��
		if nCurShowBtn > MAX_CATEGORY_NUM then
			break;
		end
		local btn				= getglobal( "GuideGiftFrame_CategoryBtn" .. nCurShowBtn .. "HighlightBtn" );
		local font				= getglobal( "GuideGiftFrame_CategoryBtn" .. nCurShowBtn .. "HighlightBtnType" );  
		local subTypeFont		= getglobal( "GuideGiftFrame_CategoryBtn" .. nCurShowBtn .. "HighlightBtnSubType" );
		local awardStateFont	= getglobal( "GuideGiftFrame_CategoryBtn" .. nCurShowBtn .. "HighlightBtnAwardState" );
		
		btn:SetClientUserData( 0, 0 );
		btn:SetClientUserData( 1, 0 );
		btn:SetClientUserData( 2, 0 );
		btn:SetClientString( "" );
		btn:DisChecked();
		-- ��Ϊ1���ڵ�
		if t_LabelNameList[nContentIndex].nSubType == 0 then
			font:SetText( t_LabelNameList[nContentIndex].name );
			subTypeFont:SetText("");
			awardStateFont:SetText( "" );
			btn:SetClientUserData( 0, t_LabelNameList[nContentIndex].nType );
			btn:SetClientUserData( 1, t_LabelNameList[nContentIndex].nSubType);
			btn:SetClientUserData( 2, t_LabelNameList[nContentIndex].nDataIndex);
			btn:SetClientString( t_LabelNameList[nContentIndex].szTable );
			
			btn:SetNormalTextureTemplate( "GongYongAnNiuA1NormalTemplate" );
			btn:SetHighlightTextureTemplate( "GongYongAnNiuA1HighTemplate" );
			btn:SetPushedTextureTemplate( "GongYongAnNiuA1PushedTemplate" );
			btn:SetDisableTextureTemplate( "GongYongAnNiuA1DisabledTemplate" );
			font:SetTextColor( 255, 254, 189 );
			btn:SetOverlayRegion( 0, 0, 160, 26 );
			btn:Show();
			nCurShowBtn = nCurShowBtn + 1;
		-- ��Ϊ2���ڵ�
		else
			-- �Ȳ鿴�丸�ڵ��Ƿ�Ϊչ��״̬
			local firstLevelNode = GetGiftCategoryParentNodeInfo( t_LabelNameList[nContentIndex] );
			if nCurSelType == 0 and nCurSelSubType == 0 and nCurSelIndex == 0 and
				t_LabelNameList[nContentIndex].nAwardState == CAN_GET_GIFT then
				nCurSelType		= t_LabelNameList[nContentIndex].nType;
				nCurSelSubType	= t_LabelNameList[nContentIndex].nSubType;
				nCurSelIndex	= t_LabelNameList[nContentIndex].nDataIndex; 
				szCurTableName	= t_LabelNameList[nContentIndex].szTable; 
				
				DisCheckAllHighLightBtn( "GuideGiftFrame_CategoryBtn", MAX_CATEGORY_NUM );
				btn:DisHighlight();
				btn:Checked();
			elseif nCurSelType == t_LabelNameList[nContentIndex].nType 
				and nCurSelSubType == t_LabelNameList[nContentIndex].nSubType 
				and nCurSelIndex == t_LabelNameList[nContentIndex].nDataIndex then

				szCurTableName	= t_LabelNameList[nContentIndex].szTable; 
				DisCheckAllHighLightBtn( "GuideGiftFrame_CategoryBtn", MAX_CATEGORY_NUM );
				btn:DisHighlight();
				btn:Checked();
			end

			-- ���丸�ڵ�Ϊչ��״̬, ����ʾ��ǰ��2���ڵ�����,����������ǰ2���ڵ�
			if firstLevelNode.bCheck then
				local t_AwardState = 
					{
						[-1]					= { text = "",			r = 52,  g = 139, b = 116	},
						[CAN_NOT_GET_GIFT]		= { text = "",			r = 52,  g = 139, b = 116	},
						[CAN_GET_GIFT]			= { text = "����ȡ",	r = 10,  g = 215, b = 0		},
						[ALREADY_GET_GIFT]		= { text = "����ȡ",	r = 255, g = 60,  b = 0		},
					};
				subTypeFont:SetText( t_LabelNameList[nContentIndex].name ); 
				local awarddata = t_AwardState[t_LabelNameList[nContentIndex].nAwardState];
				awardStateFont:SetText( awarddata.text );
				awardStateFont:SetTextColor(awarddata.r, awarddata.g, awarddata.b );
				font:SetText("");
				btn:SetClientUserData( 0, t_LabelNameList[nContentIndex].nType );
				btn:SetClientUserData( 1, t_LabelNameList[nContentIndex].nSubType );
				btn:SetClientUserData( 2, t_LabelNameList[nContentIndex].nDataIndex );
				btn:SetClientString( t_LabelNameList[nContentIndex].szTable );
				
				if math.mod( nSubLabelNum,2 ) == 0 then
					btn:SetNormalTextureTemplate( "WhiteCategoryBkg" );
				else
					btn:SetNormalTextureTemplate( "BlackCategoryBkg" );
				end
				btn:SetHighlightTextureTemplate( "HighlightTextureTemplate" );
				btn:SetPushedTextureTemplate( "PushedTextureTemplate" );
				btn:SetDisableTextureTemplate( "TransTextureTemplate" );
				btn:SetOverlayRegion( 1, 2, 155, 20 );
				btn:Show();
				nCurShowBtn = nCurShowBtn + 1;
				nSubLabelNum = nSubLabelNum + 1;
			end
			if nCurSelType == t_LabelNameList[nContentIndex].nType and t_LabelNameList[nContentIndex].nDataIndex == nCurSelIndex then				
				bFound = true
			end
		end
	end
	if ( nCurSelType == 0 and nCurSelSubType == 0 and  nCurSelIndex == 0 ) or not bFound then
		local nCurSelShowBtn = 1;
		for nContentIndex = nStartContentIndex, table.getn( t_LabelNameList ) do
			if t_LabelNameList[nContentIndex].nSubType ~= 0 then
				local btn		= getglobal( "GuideGiftFrame_CategoryBtn" .. nCurSelShowBtn .. "HighlightBtn" );
				nCurSelType		= t_LabelNameList[nContentIndex].nType;
				nCurSelSubType	= t_LabelNameList[nContentIndex].nSubType;  
				nCurSelIndex	= t_LabelNameList[nContentIndex].nDataIndex; 
				szCurTableName	= t_LabelNameList[nContentIndex].szTable; 
				
				DisCheckAllHighLightBtn( "GuideGiftFrame_CategoryBtn", MAX_CATEGORY_NUM );
				btn:DisHighlight();
				btn:Checked();
				break;
			end
			nCurSelShowBtn = nCurSelShowBtn + 1;
		end
	end
	if nCurShowBtn == 1 then
		nCurSelIndex = 0;
	end

	-- ��ûʹ�õİ�ť��������
	for i = nCurShowBtn, MAX_CATEGORY_NUM do
		local btn	= getglobal( "GuideGiftFrame_CategoryBtn" .. i .. "HighlightBtn" );
		btn:Hide();
		local font	= getglobal( "GuideGiftFrame_CategoryBtn" .. i .. "HighlightBtnType" );
		font:SetText( "" );
	end
	GuideGiftFrame_GetAwardBtn:Show();
	if GuideGiftFrame_CDKeyBtn:IsChecked() then
	--	BondingGifDesc();
		BondingChosenCDKeyDes();
	--	BondingCDKeyDesc();
	elseif GuideGiftFrame_ChargeBtn:IsChecked() then
		BondingPropGiftDesc();
	elseif GuideGiftFrame_PlayGiftBtn:IsChecked() then
		BondingPlayGiftDesc();
	elseif GuideGiftFrame_KitBtn:IsChecked() then
		BondingKitDesc();
	elseif GuideGiftFrame_CheckInBtn:IsChecked() then
		BondingGifCheckInBtnDesc();
	else
		BondingGifDesc();
	end
end
function BondingChosenCDKeyDes()
	if szCurTableName == "CodeGift" then 
		if nCurSelIndex == 1 then 
			BondingCDKeyDesc();
			g_nFocus = g_nFocus_SPECIALGIFT;
		elseif nCurSelIndex == 2 then
			BondingPlayGiftDesc();
			g_nFocus = g_nFocus_CHANGYOUGIFT;
		elseif nCurSelIndex == 3 then
			BondingKitDesc();
			g_nFocus = g_nFocus_KITGIFT;
		end
	end
end
-- Ҫѡȡ����Ʒ��ť
function GuideGiftFrame_CategoryBtn_OnClick()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return
	end
	bShowGiftClock	= false;
	GuideGiftFrameBackBtnLeaveTimeFont:SetText("");
	if this:GetClientUserData( 1 ) ~= 0 and this:GetClientUserData( 2 ) ~= 0 and this:GetClientString() ~= "" then
		nCurSelType		= this:GetClientUserData( 0 );
		nCurSelSubType	= this:GetClientUserData( 1 );  
		nCurSelIndex	= this:GetClientUserData( 2 ); 
		szCurTableName	= this:GetClientString(); 
	end

	local nCurSelectType = 0;
	for i = 1, table.getn( t_LabelNameList ) do
		if t_LabelNameList[i].nType == this:GetClientUserData( 0 ) 
			and t_LabelNameList[i].nSubType == this:GetClientUserData( 1 ) 
			and t_LabelNameList[i].nDataIndex == this:GetClientUserData( 2 ) 
			and t_LabelNameList[i].szTable == this:GetClientString() then
			t_LabelNameList[i].bCheck = not t_LabelNameList[i].bCheck;	
		end
	end
	UpdateGiftCategoryList( 1 );
end
function BondingGifCheckInBtnDesc()
	if not GuideGiftFrame_CheckInBtn:IsChecked() then 
		return;
	end
	if szCurTableName == "CheckIn" then 
		if nCurSelIndex == 1 then 
			SetCheckInGiftInfo();
		elseif nCurSelIndex == 2 then
			SetAwardMTGiftDetailInfo();
		end
	end
end
function BondingGifDesc()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return
	end

	for i = 1, 8 do
		local Btn = getglobal("GuideGiftFrame_ItemBtn"..i);
		local itemName = getglobal("GuideGiftFrame_ItemBtn"..i.."AwardItemFont");
		Btn:Hide();
		itemName:Hide();
	end

	GuideGiftFrame_GetAwardBtn:SetPoint("topRight","GuideGiftFrameBackBtn","topRight",-65,28);
	GuideGiftFrame_CDKeyEdit:Hide();


	GuideGiftFrame_GetAwardContentRich:SetText("",0,0,0);
	GuideGiftFrame_GetAwardDescRich:SetText("",0,0,0);
	GuideGiftFrameBackBtnAwardNameFont:SetText("");
	GuideGiftFrame_GetAwardBtn:Disable();
	
	local szGiftLimit	= szTitleColor.."��ȡ������#n\n"..szContentColor;
	local szGiftContent = "#n"..szTitleColor.."������ݣ�#n"..szContentColor;
	local szGiftDesc	= szTitleColor.."����������#n\n"..szContentColor;
	local nRace			= MainPlayer:getRace();
	if szCurTableName == "AccGift" then
		GuideGiftFrame_GetAwardBtn:Hide()
	end
	if nCurSelIndex == 0 then
		return;
	end
	if szCurTableName == "OneDayGift" then
		local index = 0;
		local szContent = "";
		local onedaygift = GameWizard:getOneDayGiftDef( nCurSelIndex - 1 );
		
		szGiftLimit = szGiftLimit .. "ÿ������ʱ���ۻ��ﵽ"
		if onedaygift.GiftCond.OnlineTM >= 60 then
			szGiftLimit = szGiftLimit .. math.ceil(onedaygift.GiftCond.OnlineTM/60) .. "Сʱ".."����ȡ#n";
		else
			szGiftLimit = szGiftLimit .. onedaygift.GiftCond.OnlineTM .. "����".."����ȡ#n";
		end

		--[[
		if onedaygift.GiftType == CLIENT_GIFT_TYPE_LV and CanGetAward( onedaygift.GiftCond.LVMin, 0, onedaygift.GiftID, "OneDayGift" ) == 1 then
			bShowGiftClock	= true;
			nLeavingTM		= onedaygift.GiftCond.OnlineTM;
		elseif onedaygift.GiftType == CLIENT_GIFT_TYPE_LV and CanGetAward( onedaygift.GiftCond.LVMin, 0, onedaygift.GiftID, "OneDayGift" ) == 2 then
			szGiftLimit = szGiftLimit.."����ȡ#n";
		else
			if onedaygift.GiftCond.LVMin == 0 then
				szGiftLimit = szGiftLimit.."����ȡ#n";
			else
				szGiftLimit = szGiftLimit.."�ȼ�����"..onedaygift.GiftCond.LVMin.."����ȡ#n";
			end
		end
		]]--
		for i = 1, CAREER_MAX do
			if onedaygift.GiftItems[i-1].Career ~= 0 and onedaygift.GiftItems[i-1].Career == nRace then
				index = i-1;
			end
		end
		local itemIndex = 1;
		for i = 1, ONEDAY_GIFT_ITEM_MAX do
			if onedaygift.GiftItems[index].GiftItems[i-1].ItemID ~= 0 then
				local itemDef = getItemDef( onedaygift.GiftItems[index].GiftItems[i-1].ItemID );
				local nItemCount = onedaygift.GiftItems[index].GiftItems[i-1].ItemNum;
				if itemDef ~= nil then
					BondingItemInfo( itemDef, itemIndex, nItemCount );
					itemIndex = itemIndex + 1;
				end
			end
		end
		szGiftContent =  "\n\n"..szGiftContent..szContent;
		szGiftDesc = szGiftDesc..onedaygift.GiftDesc;
		GuideGiftFrameBackBtnAwardNameFont:SetText( onedaygift.Name );
	elseif szCurTableName =="LVGift" then
		local index = 0;
		local szContent = "";
		local lvgift = GameWizard:getLVGiftDef( nCurSelIndex - 1 );
		if lvgift.LV > 0 then
			szGiftLimit = szGiftLimit.."�ȼ�����"..lvgift.LV.."����ȡ#n";
		end
		for i = 1, CAREER_MAX do
			if lvgift.GiftItems[i-1].Career ~= 0 and lvgift.GiftItems[i-1].Career == nRace then
				index = i-1;
			end
		end
		local itemIndex = 1;
		for i = 1, ONEDAY_GIFT_ITEM_MAX do
			if lvgift.GiftItems[index].GiftItems[i-1].ItemID ~= 0 then
				local itemDef = getItemDef( lvgift.GiftItems[index].GiftItems[i-1].ItemID );
				local nItemCount = lvgift.GiftItems[index].GiftItems[i-1].ItemNum;
				if itemDef ~= nil then
					BondingItemInfo( itemDef, itemIndex, nItemCount );
					itemIndex = itemIndex + 1;
				end
			end
		end
		szGiftContent =  "\n\n"..szGiftContent..szContent;
		szGiftDesc = szGiftDesc..lvgift.GiftDesc;
		GuideGiftFrameBackBtnAwardNameFont:SetText( lvgift.Name );
	elseif szCurTableName == "ContOnlineGiftDef" then
		local index = 0;
		local szContent = "";
		local giftDef = GameWizard:getContOnlineGiftDef( nCurSelIndex );
		if giftDef.Day > 0 then
			szGiftLimit = szGiftLimit .."����".. giftDef.Day .. "���ۻ�����ʱ�����ﵽ";
		end
		if giftDef.Hour >= 60 then
			szGiftLimit = szGiftLimit .. math.ceil(giftDef.Hour/60) .. "Сʱ".."����ȡ#n";
		else
			szGiftLimit = szGiftLimit .. giftDef.Hour .. "����".."����ȡ#n";
		end
		
		for i = 1, CAREER_MAX do
			if giftDef.GiftItems[i-1].Career == 0 or giftDef.GiftItems[i-1].Career == nRace then
				index = i-1;
				break;
			end
		end
		local itemIndex = 1;
		for i = 1, ONEDAY_GIFT_ITEM_MAX do
			if giftDef.GiftItems[index].GiftItems[i-1].ItemID ~= 0 then
				local itemDef = getItemDef( giftDef.GiftItems[index].GiftItems[i-1].ItemID );
				local nItemCount = giftDef.GiftItems[index].GiftItems[i-1].ItemNum;
				if itemDef ~= nil then
					BondingItemInfo( itemDef, itemIndex, nItemCount );
					itemIndex = itemIndex + 1;
				end
			end
		end
		szGiftContent =  "\n\n"..szGiftContent..szContent;
		szGiftDesc = szGiftDesc..giftDef.GiftDesc;
		GuideGiftFrameBackBtnAwardNameFont:SetText( giftDef.Name );
	elseif szCurTableName == "OnlineGiftGroupDef" then
		local szContent = "";
		local ContiGroupData = GameWizard:getContiData(nCurSelIndex - 1 );
		if ContiGroupData ~= nil then
			local giftGroupDef = GameWizard:getOnlineGiftGroupDef( ContiGroupData.ID );
			local giftOne = GameWizard:getGiftInfo( ContiGroupData.ID,ContiGroupData.Day );
			if giftGroupDef.Time == 0 then
				szGiftLimit = szGiftLimit .."����".. ContiGroupData.Day .. "���¼��Ϸ����ȡ"
			else
				szGiftLimit = szGiftLimit .."����".. ContiGroupData.Day .. "���ۻ�����ʱ�����ﵽ" .. giftGroupDef.Time .. "����".."����ȡ#n";			
			end

			local itemIndex = 1;
			if giftOne.ItemID1 > 0 then
				local itemDef = getItemDef( giftOne.ItemID1 );
				local nItemCount = giftOne.ItemNum1;
				if itemDef ~= nil then
					BondingItemInfo( itemDef, itemIndex, nItemCount );
					itemIndex = itemIndex + 1;
				end
			end
			if giftOne.ItemID2 > 0 then
				local itemDef = getItemDef( giftOne.ItemID2 );
				local nItemCount = giftOne.ItemNum2;
				if itemDef ~= nil then
					BondingItemInfo( itemDef, itemIndex, nItemCount );
					itemIndex = itemIndex + 1;
				end
			end
			if giftOne.ItemID3 > 0 then
				local itemDef = getItemDef( giftOne.ItemID3 );
				local nItemCount = giftOne.ItemNum3;
				if itemDef ~= nil then
					BondingItemInfo( itemDef, itemIndex, nItemCount );
					itemIndex = itemIndex + 1;
				end
			end
			szGiftContent =  "\n\n"..szGiftContent..szContent;
			szGiftDesc = szGiftDesc..giftOne.GiftDesc;
			GuideGiftFrameBackBtnAwardNameFont:SetText( giftGroupDef.Name );
		end
	elseif szCurTableName == "AccGift" then
		local accGiftInfo = GameWizard:getAccGiftInfo();
		local accGift = nil
		for i=1,accGiftInfo.Count do
			if accGiftInfo.AccGifts[i-1].ID == nCurSelIndex then
				accGift = accGiftInfo.AccGifts[i-1]
				break
			end
		end
		if accGift == nil then 
			return
		end
		szGiftLimit = "";
		szGiftDesc = szGiftDesc..accGift.GiftDesc;

		local itemIndex = 1;
		for i = 1, MAX_ACC_GIFT_LIST_TOLUA do
			if accGift.GiftItems[i-1].ItemID ~= 0 then
				local itemDef = getItemDef( accGift.GiftItems[i-1].ItemID );
				local nItemCount = accGift.GiftItems[i-1].ItemNum;
				if itemDef ~= nil then
					BondingItemInfo( itemDef, itemIndex, nItemCount );
					itemIndex = itemIndex + 1;
				end
			end
		end
		GuideGiftFrame_GetAwardBtn:Show()
		GuideGiftFrameBackBtnAwardNameFont:SetText( accGift.Name );
	elseif szCurTableName == "PropGift" then
		local propGiftInfo = GameWizard:getPropGiftInfo();
		if nCurSelIndex -1 < propGiftInfo.Count then
			szGiftLimit = "";
			szGiftDesc = szGiftDesc .. propGiftInfo.PropGifts[nCurSelIndex-1].PropsGiftDesc;
		end
		local itemIndex = 1;
		for i = 1, PROPS_GIFT_ITEM_NUM_TOLUA do
			if propGiftInfo.PropGifts[nCurSelIndex - 1].PropsGiftItem[i-1].ItemID ~= 0 then
				local itemDef = getItemDef( propGiftInfo.PropGifts[nCurSelIndex - 1].PropsGiftItem[i-1].ItemID );
				local nItemCount = propGiftInfo.PropGifts[nCurSelIndex - 1].PropsGiftItem[i-1].ItemNum;
				if itemDef ~= nil then
					BondingItemInfo( itemDef, itemIndex, nItemCount );
					itemIndex = itemIndex + 1;
				end
			end
		end
		GuideGiftFrameBackBtnAwardNameFont:SetText( "��ֵ���" );
	elseif szCurTableName == "VipNetBarGift" then
		local index = 0;
		local szContent = "";
		local BarGift = GameWizard:GetVipNetBarGift( nCurSelIndex - 1 );
		if BarGift.GiftCond.ContOnlineTM >= 60 then
			szGiftLimit = szGiftLimit .. "��������" .. math.ceil(BarGift.GiftCond.ContOnlineTM/60) .. "Сʱ".."����ȡ#n";
		else
			szGiftLimit = szGiftLimit .."��������" .. BarGift.GiftCond.ContOnlineTM  .. "����" .. "����ȡ#n";
		end
		
		szContent = "���" .. GetMoneyDesc(BarGift.MoneyType) .. BarGift.Money;
		szGiftContent =  "\n\n"..szGiftContent..szContent;
		szGiftDesc = szGiftDesc..BarGift.GiftDesc;
		GuideGiftFrameBackBtnAwardNameFont:SetText( BarGift.Name );
	end
	
	local bAward = false;
	for i = 1, table.getn( t_LabelNameList ) do
		if t_LabelNameList[i].nType == nCurSelType 
			and t_LabelNameList[i].nSubType == nCurSelSubType 
			and t_LabelNameList[i].nDataIndex == nCurSelIndex 
			and t_LabelNameList[i].szTable == szCurTableName 
			and t_LabelNameList[i].nAwardState == CAN_GET_GIFT then
			bAward = true;
			t_LabelNameList[i].bCheck = not t_LabelNameList[i].bCheck;	
		elseif nCurSelSubType == 0 then
			t_LabelNameList[i].bCheck = false;
		end
	end

	if bAward then
		GuideGiftFrame_GetAwardBtn:Enable();
	else
		GuideGiftFrame_GetAwardBtn:Disable();
	end
	
	GuideGiftFrame_GetAwardContentRich:SetText( szGiftLimit..szGiftContent,255,255,255);
	GuideGiftFrame_GetAwardDescRich:SetText( szGiftDesc,255,255,255);
end

function BondingCDKeyDesc()
	-- ������ǰ�Ľ���
	for i = 1, 8 do
		local Btn = getglobal("GuideGiftFrame_ItemBtn"..i);
		local itemName = getglobal("GuideGiftFrame_ItemBtn"..i.."AwardItemFont");
		Btn:Hide();
		itemName:Hide();
	end

	GuideGiftFrameBackBtnAwardNameFont:SetText( "������Ȩ����ȡ" );
	GuideGiftFrame_GetAwardContentRich:SetText( szTitleColor.."���빫����Ȩ�𿨺ţ�#n",255,255,255);
	local szGiftDesc	= szTitleColor.."˵����#n\n"..szContentColor;
	local str = 
[[
1�����Ͽ������빫����Ȩ�𿨺ţ������ȡ�������ͨ���ʼ����͵������������ע����ա�
2��ÿ����ɫ��һ�δ򿪹�����Ȩ��ʱ�ɻ�����н��еĵ��ߣ������ѡ����ȡ�Ľ�ɫ��
3�������ٴ�ӵ�й�����Ȩ��������ȡʱ���򿪹�����Ȩ�𿨽��ɻ�ý��еİ׽𿨡�
4���ռ�3�Żƽ���ȯ������Ϸ����NPC�����ơ��һ�1�����ƽ������
5���ռ�4�Ű׽𿨿�����Ϸ����NPC�����ơ��һ�1�����׽𸣴��� ]];

	szGiftDesc = szGiftDesc .. str;
	
	GuideGiftFrame_GetAwardDescRich:SetText( szGiftDesc,255,255,255);
	--�����
	GuideGiftFrame_CDKeyEdit:Show();
	GuideGiftFrame_GetAwardBtn:SetPoint( "topleft","GuideGiftFrameBackBtn","topright",-105,66 );
	GuideGiftFrame_GetAwardBtn:Enable();
end

function BondingPlayGiftDesc()
	-- ������ǰ�Ľ���
	for i = 1, 8 do
		local Btn = getglobal("GuideGiftFrame_ItemBtn"..i);
		local itemName = getglobal("GuideGiftFrame_ItemBtn"..i.."AwardItemFont");
		Btn:Hide();
		itemName:Hide();
	end

	GuideGiftFrameBackBtnAwardNameFont:SetText( "���������ȡ" );
	GuideGiftFrame_GetAwardContentRich:SetText( szTitleColor.."���볩��������ţ�#n",255,255,255);
	local szGiftDesc	= szTitleColor.."˵����#n\n"..szContentColor;
	local str = 
[[
1�����Ͽ������볩��������ţ������ȡ�������ͨ���ʼ����͵������������ע����ա�
2��ÿ���˺�ֻ��һ����ɫ������ȡ��������������ѡ����ȡ�Ľ�ɫ�� ]];

	szGiftDesc = szGiftDesc .. str ;

	GuideGiftFrame_GetAwardDescRich:SetText( szGiftDesc,255,255,255);
	--�����
	GuideGiftFrame_CDKeyEdit:Show();	
	GuideGiftFrame_GetAwardBtn:SetPoint( "topleft","GuideGiftFrameBackBtn","topright",-105,66 );
	GuideGiftFrame_GetAwardBtn:Enable();
end

function BondingKitDesc()
	-- ������ǰ�Ľ���
	for i = 1, 8 do
		local Btn = getglobal("GuideGiftFrame_ItemBtn"..i);
		local itemName = getglobal("GuideGiftFrame_ItemBtn"..i.."AwardItemFont");
		Btn:Hide();
		itemName:Hide();
	end

	GuideGiftFrameBackBtnAwardNameFont:SetText( "���������ȡ" );
	GuideGiftFrame_GetAwardContentRich:SetText( szTitleColor.."�������������ţ�#n",255,255,255);
	local szGiftDesc	= szTitleColor.."˵����#n\n"..szContentColor;
	local str = 
[[
1�����Ͽ����������������ţ������ȡ�������ͨ���ʼ����͵������������ע����ա�
2�������������������ħ�����л����ã��������ޡ� ]];

	szGiftDesc = szGiftDesc .. str ;

	GuideGiftFrame_GetAwardDescRich:SetText( szGiftDesc,255,255,255);
	--�����
	GuideGiftFrame_CDKeyEdit:Show();	
	GuideGiftFrame_GetAwardBtn:SetPoint( "topleft","GuideGiftFrameBackBtn","topright",-105,66 );
	GuideGiftFrame_GetAwardBtn:Enable();
end

local MAX_GIFT_LIST = 4;
local t_descColor = { 
			[0] = { "#c461e0a", "#cff3200" }, 
			[1] = { "#c464646", "#c6e6450" }, 
			};
function BondingPropGiftDesc()
	local propGiftInfo = GameWizard:getPropGiftInfo();
	local groupID = propGiftInfo.PropsDef[nCurSelIndex-1].GroupID;
	local cashNow = nil;
	local index = 0;
	for i=1, propGiftInfo.Count do
		if propGiftInfo.PropsGift[i-1].GroupID == groupID then
			cashNow = propGiftInfo.PropsGift[i-1].CashNow;
			index = i - 1;
			break;
		end
	end

	GuideGiftFramePropGiftBtnName:SetText( propGiftInfo.PropsDef[nCurSelIndex-1].Title );
	GuideGiftFramePropGiftBtnNum:SetText( "��ڼ��ۼƳ�ֵ��ң�" .. (cashNow ~= nil and GameWizard:uLongLongToString( cashNow ) or 0) );
	local beginDate = propGiftInfo.PropsDef[nCurSelIndex-1].BeginDate;
	local endDate = propGiftInfo.PropsDef[nCurSelIndex-1].EndDate;
	local szDate = string.format( "�ʱ�䣺#cff1e00%d��%d��%d��-%d��%d��%d��",string.sub(beginDate,1,4),string.sub(beginDate,5,6),string.sub(beginDate,7,8),
					string.sub(endDate,1,4), string.sub(endDate,5,6), string.sub(endDate,7,8));
	GuideGiftFramePropGiftBtnDescRich1:SetText( szDate .. "#c6e6450��ʣ��ʱ�䣺#cff1e00" .. GetLimitDate(beginDate,endDate) .. "#c6e6450��", 70, 30, 10 );
	GuideGiftFramePropGiftBtnDescRich2:SetText( propGiftInfo.PropsDef[nCurSelIndex-1].DesHead, 70, 30, 10 );
	GuideGiftFramePropGiftBtnDescRich3:SetText( propGiftInfo.PropsDef[nCurSelIndex-1].DesTail, 70, 30, 10 );
	GuideGiftFramePropGiftBtn:SetClientUserData( 0, index );

	local count = 0;
	for i=1, propGiftInfo.DefCount do
		if propGiftInfo.PropsDef[i-1].GroupID == groupID then
			count = count + 1;
		end
		if cashNow == nil or GameWizard:compareValue( cashNow, propGiftInfo.PropsDef[i-1].CashUp ) < 0 then
			break;
		end
	end
	
	local slider 	= getglobal( "GuideGiftFramePropGiftBtnScrollBar" );
	local nNum		= count - MAX_GIFT_LIST;
	if nNum > 0  then
		slider:SetMaxValue( nNum );
		slider:SetValue( nNum );
		slider:Show();
	else
		slider:SetMaxValue( 1 );
		slider:Hide();
	end
	slider:SetMinValue( 0 );
	slider:SetValueStep( 1 );
	if slider:GetValue() > slider:GetMaxValue() then
		slider:SetValue( slider:GetMaxValue() );
	end
--	SetScrollBar( slider:GetName(), MAX_GIFT_LIST, count );
	UpdatePropGiftList( slider:GetValue() );
end

function UpdatePropGiftList( value )
	local num = 1;
	local propGiftInfo = GameWizard:getPropGiftInfo();
	local groupID = propGiftInfo.PropsDef[nCurSelIndex-1].GroupID;
	local index = GuideGiftFramePropGiftBtn:GetClientUserData(0);
	local cashNow = propGiftInfo.PropsGift[index].CashNow;
	local count = 0;
	for i=1, propGiftInfo.DefCount do
		if num > MAX_GIFT_LIST then
			break;
		end

		if propGiftInfo.PropsDef[i-1].GroupID == groupID then
			count = count + 1;
		end

		if count > value then
			local bkg = getglobal( "GuideGiftFramePropGiftBtnGift" .. num .. "Bkg" );
			local box = getglobal( "GuideGiftFramePropGiftBtnGift" .. num .. "IconBox" );
			local btn = getglobal( "GuideGiftFramePropGiftBtnGift" .. num .. "GetAwardBtn" );
			local icon = getglobal( "GuideGiftFramePropGiftBtnGift" .. num .. "IconTex" );
			local name = getglobal( "GuideGiftFramePropGiftBtnGift" .. num .. "Name" );
			local rich = getglobal( "GuideGiftFramePropGiftBtnGift" .. num .. "Rich" );
			local uvTex = getglobal( "GuideGiftFramePropGiftBtnGift" .. num .. "GetAwardBtnUVAnimationTex" );

			box:Show();
			btn:Show();
			uvTex:Hide();
			btn:Enable();
			btn:SetText("��ȡ����");
			local IconPath = GetItemEquipIconPath() .. propGiftInfo.PropsDef[i-1].IconID .. ".tga";
			-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			icon:SetTexture( IconPath );
			name:SetTextColor( 255, 50, 0 );
			name:SetText( string.format( "%dԪ��ֵ���", math.floor(GameWizard:uLongLongToString( propGiftInfo.PropsDef[i-1].CashUp ) /10) ) );

			local state = 0;
			local bEnd = false;
			if cashNow == nil or GameWizard:compareValue( cashNow, propGiftInfo.PropsDef[i-1].CashUp ) < 0 then
				icon:SetGray( false );
				bkg:SetTexture( "uires\\ui\\Za\\LiBao\\Lan.tga" );
				btn:Checked();
				btn:SetClientUserData( 0, 0 );
				bEnd = true;
			elseif CheckGiftGetState( index, propGiftInfo.PropsDef[i-1].CashUp ) then
				state = 1;
				icon:SetGray( true );
				name:SetTextColor( 120, 80, 80 );
				bkg:SetTexture( "uires\\ui\\Za\\LiBao\\Hui.tga" );
				btn:Disable();
				btn:SetText("����ȡ")
				btn:SetClientUserData( 0, 0 );
			else
				icon:SetGray( false );
				bkg:SetTexture( "uires\\ui\\Za\\LiBao\\Lv.tga" );
				btn:DisChecked();
				btn:SetClientUserData( 0, i );
				uvTex:SetUVAnimation(80, true);
				uvTex:Show();
			end
			
			local szText = t_descColor[state][1] .. "�ۼƳ�ֵ�������" .. t_descColor[state][2] .. GameWizard:uLongLongToString( propGiftInfo.PropsDef[i-1].CashUp ) .."���\n" .. t_descColor[state][1] .."������ݣ�" .. t_descColor[state][2];
			for j=1, PROPS_GIFT_ITEM_NUM_TOLUA do
				if propGiftInfo.PropsDef[i-1].PropsGiftItem[j-1].ItemID ~= 0 then
					local itemdef = getItemDef( propGiftInfo.PropsDef[i-1].PropsGiftItem[j-1].ItemID )
					szText = szText .. "��" .. itemdef.Name .. "*" .. propGiftInfo.PropsDef[i-1].PropsGiftItem[j-1].ItemNum;
				end
			end
			rich:SetText( string.gsub( szText, "��", "", 1 ), 70, 30, 10 );
			num = num + 1;
			if bEnd then break end;
		end
	end
	ClearPropGiftFrame( num );
end

function ClearPropGiftFrame( num )
	for i=num, MAX_GIFT_LIST do
		local bkg = getglobal( "GuideGiftFramePropGiftBtnGift" .. i .. "Bkg" );
		local box = getglobal( "GuideGiftFramePropGiftBtnGift" .. i .. "IconBox" );
		local btn = getglobal( "GuideGiftFramePropGiftBtnGift" .. i .. "GetAwardBtn" );
		local icon = getglobal( "GuideGiftFramePropGiftBtnGift" .. i .. "IconTex" );
		local name = getglobal( "GuideGiftFramePropGiftBtnGift" .. i .. "Name" );
		local rich = getglobal( "GuideGiftFramePropGiftBtnGift" .. i .. "Rich" );
		local uvTex = getglobal( "GuideGiftFramePropGiftBtnGift" .. i .. "GetAwardBtnUVAnimationTex" );

		bkg:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
		box:Hide();
		btn:Hide();
		icon:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
		name:SetText( "" );
		rich:SetText( "", 70, 30, 10 );
		uvTex:Hide();
	end
end

function GuideGiftFramePropGiftScrollUpBtn_OnClick()
	local slider = getglobal( "GuideGiftFramePropGiftBtnScrollBar" );
	if not slider:IsShown() then return end;
	local nValue = slider:GetValue() - slider:GetValueStep()
	if nValue >= slider:GetMinValue() then
		slider:SetValue(nValue)
	end
end

function GuideGiftFramePropGiftScrollDownBtn_OnClick()
	local slider = getglobal( "GuideGiftFramePropGiftBtnScrollBar" );
	if not slider:IsShown() then return end;
	local nValue	= slider:GetValue() + slider:GetValueStep()
	if nValue <= slider:GetMaxValue() then
		slider:SetValue(nValue)
	end
end

function GuideGiftFramePropGiftScrollBar_OnValueChanged()
	if not GuideGiftFrame_ChargeBtn:IsChecked() then
		return;
	end
	local slider = getglobal("GuideGiftFramePropGiftBtnScrollBar");
	local nValue = slider:GetValue();
	local nMaxValue = slider:GetMaxValue();

	if nValue <= nMaxValue then
		UpdatePropGiftList( nValue );
	end
end

function BondingItemInfo( itemdef, itemIndex, nItemCount )
	local Btn = getglobal("GuideGiftFrame_ItemBtn"..itemIndex);
	local itemBtn = getglobal("GuideGiftFrame_ItemBtn"..itemIndex.."AwardItem");
	local itemTex = getglobal("GuideGiftFrame_ItemBtn"..itemIndex.."AwardItemIconTexture");
	local itemCountFont = getglobal("GuideGiftFrame_ItemBtn"..itemIndex.."AwardItemCountFont");
	local itemName = getglobal("GuideGiftFrame_ItemBtn"..itemIndex.."AwardItemFont");
	local itemNameTex = getglobal("GuideGiftFrame_ItemBtn"..itemIndex.."AwardItemTex");
	local itemBackTex = getglobal("GuideGiftFrame_ItemBtn"..itemIndex.."AwardItemBoxTexture");	
	local itemUVTex = getglobal("GuideGiftFrame_ItemBtn"..itemIndex.."AwardItemUVAnimationTex");
	itemBackTex:SetTexUV(794, 562, 44, 44);
	itemCountFont:SetText("");
	itemBackTex:Show();
	itemNameTex:Show();
	itemName:Show();

	if itemIndex%2 == 0 then
		Btn:SetPoint("TOPLEFT","GuideGiftFrame_GetAwardContentRich","BOTTOMLEFT", 160,	(math.ceil(itemIndex/2)-1)*50);
		itemName:SetPoint("TOPLEFT","GuideGiftFrame_GetAwardContentRich","BOTTOMLEFT", 210, 5+(math.ceil(itemIndex/2)-1)*50);
		GuideGiftFrame_GetAwardDescRich:SetPoint("TOPLEFT","GuideGiftFrame_GetAwardContentRich","BOTTOMLEFT", 0, 50+(math.ceil(itemIndex/2)-1)*50);
	else
		Btn:SetPoint("TOPLEFT","GuideGiftFrame_GetAwardContentRich","BOTTOMLEFT", 0,	(math.ceil(itemIndex/2)-1)*50);
		itemName:SetPoint("TOPLEFT","GuideGiftFrame_GetAwardContentRich","BOTTOMLEFT", 50, 5+(math.ceil(itemIndex/2)-1)*50);
		GuideGiftFrame_GetAwardDescRich:SetPoint("TOPLEFT","GuideGiftFrame_GetAwardContentRich","BOTTOMLEFT", 0, 50+(math.ceil(itemIndex/2)-1)*50);
	end

	local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
	-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
	if not IsInExistence( IconPath ) then
		IconPath = DEFAULT_ITEM_PATH;
	end
	itemTex:SetTexture( IconPath );
	itemTex:SetTexRelUV( 1, 1 );
	itemCountFont:SetPoint( "bottomleft", Btn:GetName(), "bottomleft", 0, 10 );
	
	SetEquipImportantEffectUV( itemdef, itemUVTex:GetName());
	itemUVTex:SetTexRelUV( 1, 1 );

	local szItemName = itemdef.Name;
	szItemName = szItemName.."\n������"..nItemCount;
	
	itemName:SetText( szItemName, 57, 48, 34 );
	Btn:Show();
	itemBtn:SetClientUserData( 1, itemdef.ItemID );
end

function CanGetOneDayGift()	
	for i = 1, GameWizard:getOneDayGiftNum() do
		local onedaygift	= GameWizard:getOneDayGiftDef( i - 1 );
		local nOnLineTm		= onedaygift.GiftCond.OnlineTM;
		local nRet			= CanGetAward( onedaygift.GiftCond.LVMin, nOnLineTm, onedaygift.GiftID, "OneDayGift" );	
		if nRet == CAN_GET_GIFT then
			return true;
		end
	end
	local ContOnlineInfo = GameWizard:getContOnlineInfo();
	for i=1,ContOnlineInfo.Num do
		if i + 1 >= 5 then
			break;
		end
		local info = ContOnlineInfo.ContOnlineInfo[i-1];
		if info.IsGift == CAN_GET_GIFT then
			return true;
		end
	end
	return ( GameWizard:getContiNum() > 0 )
end

function CanGetAccGift()
	local accGiftInfo = GameWizard:getAccGiftInfo();
	if accGiftInfo.Count > 0 then
		for i=1,accGiftInfo.Count do
			if (i - 1) >= MAX_ACC_GIFT_LIST_TOLUA then
				break;
			end			
			if accGiftInfo.AccGifts[i-1].Flag == ACCGIFT_CAN_GET_TOLUA then
				return true;
			end			
		end
	end
end

function CanGetLVGift()
	for i = 1, GameWizard:getLVGiftNum() do
		local lvgift	= GameWizard:getLVGiftDef( i - 1 );
		local nRet		= CanGetAward( lvgift.LV, 0, 0, "LVGift" );
		
		if nRet == CAN_GET_GIFT then
			return true;
		end
	end
	for i=1,GameWizard:GetVipNetBarGiftNum() do
		local nRet = CanGetAward( i-1, 0, 0, "VipNetBarGift" );
		if nRet == CAN_GET_GIFT then
			return true;
		end
	end
	return false;
end

function CanGetPropGift( groupID )
	local propGiftInfo = GameWizard:getPropGiftInfo();
	if groupID ~= nil then
		if propGiftInfo.Count == 0 then
			return 0;
		end

		local money = 0;
		local idx = 0;
		for i=1,propGiftInfo.Count do
			if propGiftInfo.PropsGift[i-1].GroupID == groupID then
				money = propGiftInfo.PropsGift[i-1].CashNow;
				idx = i;
				break;
			end
		end
		if money == 0 or idx == 0 then
			return 0;
		end

		local total = 0;
		local num = 0;
		for i=1,propGiftInfo.DefCount do
			if propGiftInfo.PropsDef[i-1].GroupID == groupID then
				total = total + 1;
				if GameWizard:compareValue( money, propGiftInfo.PropsDef[i-1].CashUp ) >= 0 then
					num = num + 1;
				end
			end
		end
		if total == 0 or num == 0 then
			return 0;
		end

		if total == propGiftInfo.PropsGift[idx-1].GiftGetNum then
			return 1;
		elseif num - propGiftInfo.PropsGift[idx-1].GiftGetNum > 0 then
			return 99;
		end
		return 0;
	else
		for i=1,propGiftInfo.Count do
			local sum = 0;
			local money = propGiftInfo.PropsGift[i-1].CashNow;
			for j=1,propGiftInfo.DefCount do
				if propGiftInfo.PropsDef[j-1].GroupID == propGiftInfo.PropsGift[i-1].GroupID and 
					GameWizard:compareValue( money, propGiftInfo.PropsDef[j-1].CashUp ) >= 0 then
					sum = sum + 1;
				end
			end
			
			 if sum - propGiftInfo.PropsGift[i-1].GiftGetNum > 0 then
				return true;
			 end
		end
		return false;
	end
end

function GuideGiftFrame_OnShow()
	this:SetPoint("center","$parent","center",0,0);
	ResetSelectData();
	GuideGiftFrameDisCheckAllBtn();
	if GiftCenterGuideAwardMTButton:IsShown() then
		nCurSelType	 	= CLIENT_GIFT_TYPE_CONT_COVER;
		nCurSelSubType 	= 2;
		nCurSelIndex 	= 2;
		GuideGiftFrame_CheckInBtn:Checked();
	elseif CanGetAccGift() then
		GuideGiftFrame_AccGiftBtn:Checked();
		GameWizard:RequestReloadAccgift();
	elseif CanGetLVGift() then
		GuideGiftFrame_GrowBtn:Checked();
	elseif CanGetPropGift() then
		GuideGiftFrame_ChargeBtn:Checked();
	elseif GiftCenterGuideCheckInButton:IsShown() then 
		GuideGiftFrame_CheckInBtn:Checked();
	else
		GuideGiftFrame_GrowBtn:Checked();
	end
	BondingGiftLabelData();
	GuideGiftFrame_GetAwardBtn:DisHighlight();
end

function GuideGiftFrame_OnLoad()
	this:RegisterEvent("GE_GIFT_UPDATE");
end

function GuideGiftFrame_OnEvent()
	if arg1 == "GE_GIFT_UPDATE" then
		BondingGiftLabelData();
		if CanGetLVGiftDataNum() > 0 then
			--ResetSelectData();
			UpdateGiftCategoryList( 1 );
		end
	end
end

local nLastRefreshTime = 0;
function GuideGiftFrame_OnUpdate()
	if GuideGiftFrame_CycleBtn:IsChecked() and bShowGiftClock then
		local nTM = GameWizard:getLeavingTime( nLeavingTM*60 );
		if nTM > 0 then
			local nHour			= string.format( "%02d", nTM / 3600 );
			local nMin			= os.date( "%M", nTM );
			local nSecond		= os.date( "%S", nTM );
			GuideGiftFrameBackBtnLeaveTimeFont:SetText( "��ȡ����ʱ��"..nHour..":"..nMin..":"..nSecond );
		else
			GuideGiftFrameBackBtnLeaveTimeFont:SetText( "����ȡ����" );
			bShowGiftClock = false;
		end
	elseif GuideGiftFrame_GrowBtn:IsChecked() then
		GuideGiftFrameBackBtnLeaveTimeFont:SetText( "" );
	end
	--[[
	if GuideGiftFrame_CycleBtn:IsChecked() and os.clock() - nLastRefreshTime > 30 then
		BondingGiftLabelData();
		UpdateGiftCategoryList( 1 );
	end
	]]--
end

function GuideGiftFrameDisCheckAllBtn()
	GuideGiftFrame_ChargeBtn:DisChecked();
	GuideGiftFrame_CycleBtn:DisChecked();
	GuideGiftFrame_GrowBtn:DisChecked();
	GuideGiftFrame_CDKeyBtn:DisChecked();
	GuideGiftFrame_AccGiftBtn:DisChecked();
	GuideGiftFrame_PlayGiftBtn:DisChecked();
	GuideGiftFrame_KitBtn:DisChecked();
	GuideGiftFrame_CheckInBtn:DisChecked();
end

function GuideGiftFrame_GrowBtn_OnClick()
	if GuideGiftFrame_GrowBtn:IsChecked() then
		return;
	end
	GuideGiftFrameDisCheckAllBtn();
	GuideGiftFrame_GrowBtn:Checked();
	ResetSelectData();
	BondingGiftLabelData();
end

function GuideGiftFrame_CycleBtn_OnClick()
	if GuideGiftFrame_CycleBtn:IsChecked() then
		return;
	end
	GuideGiftFrameDisCheckAllBtn();
	GuideGiftFrame_CycleBtn:Checked();
	ResetSelectData();
	BondingGiftLabelData();
end

function GuideGiftFrame_Charge_OnClick()
	if GuideGiftFrame_ChargeBtn:IsChecked() then
		return;
	end
	GuideGiftFrameDisCheckAllBtn();
	GuideGiftFrame_ChargeBtn:Checked();
	ResetSelectData();
	BondingGiftLabelData();
end

function GuideGiftFrame_CDKey_OnClick()
	if GuideGiftFrame_CDKeyBtn:IsChecked() then
		return
	end
	g_nFocus = g_nFocus_SPECIALGIFT;
	GuideGiftFrameDisCheckAllBtn();
	GuideGiftFrame_CDKeyBtn:Checked();
	GuideGiftFrame_CDKeyEdit:SetText( "" );
	ResetSelectData();
	BondingGiftLabelData();
end

function GuideGiftFrame_PlayGift_OnClick()
	if GuideGiftFrame_PlayGiftBtn:IsChecked() then
		return
	end
	g_nFocus = g_nFocus_CHANGYOUGIFT;
	GuideGiftFrameDisCheckAllBtn();
	GuideGiftFrame_PlayGiftBtn:Checked();
	GuideGiftFrame_CDKeyEdit:SetText( "" );
	ResetSelectData();
	BondingGiftLabelData();
end

function GuideGiftFrame_Kit_OnClick()
	if GuideGiftFrame_KitBtn:IsChecked() then
		return
	end
	g_nFocus = g_nFocus_KITGIFT;
	GuideGiftFrameDisCheckAllBtn();
	GuideGiftFrame_KitBtn:Checked();
	GuideGiftFrame_CDKeyEdit:SetText( "" );
	ResetSelectData();
	BondingGiftLabelData();
end

function GuideGiftFrame_AccGift_OnClick()
	if GuideGiftFrame_AccGiftBtn:IsChecked() then
		return
	end
	GuideGiftFrameDisCheckAllBtn();
	GameWizard:RequestReloadAccgift();
	GuideGiftFrame_AccGiftBtn:Checked();
	ResetSelectData();
	BondingGiftLabelData();
end
-- ǩ�����
function GuideGiftFrame_CheckInBtn_OnClick()
	if GuideGiftFrame_CheckInBtn:IsChecked() then
		return
	end
	GuideGiftFrameDisCheckAllBtn();
	GuideGiftFrame_CheckInBtn:Checked();
	ResetSelectData();
	BondingGiftLabelData();
end
function ResetSelectData()
	nCurSelType		= 0;
	nCurSelSubType	= 0;  
	nCurSelIndex	= 0; 
	szCurTableName	= "";  
	bShowGiftClock	= false;
	nLeavingTM		= 0;
end

function GuideGiftFrame_GetAwardBtn_OnClick()
	if GuideGiftFrame_CDKeyEdit:IsShown() then
		local szText = GuideGiftFrame_CDKeyEdit:GetText();
		if szText == "" then
			if g_nFocus == g_nFocus_SPECIALGIFT then
				ShowMidTips( "�����빫����Ȩ�𿨺�" );
			elseif g_nFocus == g_nFocus_CHANGYOUGIFT then
				ShowMidTips( "�����볩���������" );
			elseif g_nFocus == g_nFocus_KITGIFT then
				ShowMidTips( "����������������" );
			end
			return;
		end
		GameWizard:sendCDKeyGiftRequest( szText );
		--GameWizard:sendPlayGiftRequest( szText );
		return;
	end

	local nLv		= 0;
	local nGiftId	= 0;
	local nType		= 0;
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return
	end
	if MainPlayer:isDead() then
		AddGameMiddleTips2("����״̬������ȡ���");
		return;
	end

	if szCurTableName == "OneDayGift" then
		local onedaygift = GameWizard:getOneDayGiftDef( nCurSelIndex - 1 );
		nGiftId = onedaygift.GiftID;
		nType = CLIENT_GIFT_TYPE_ONEDAY;
	elseif szCurTableName =="LVGift" then
		local lvgift = GameWizard:getLVGiftDef( nCurSelIndex - 1 );
		nLv = lvgift.LV;
		nType =CLIENT_GIFT_TYPE_LV;
	elseif szCurTableName == "ContOnlineGiftDef" then
		nType = CLIENT_GIFT_TYPE_CONTI;
		nGiftId = nCurSelIndex;
	elseif szCurTableName == "OnlineGiftGroupDef" then
		nType = CLIENT_GIFT_TYPE_CONTI_GROUP;
		local ContiGroupData = GameWizard:getContiData(nCurSelIndex - 1 );
		if ContiGroupData ~= nil then
			nLv = ContiGroupData.ID;
			nGiftId = ContiGroupData.Day;
		end
	elseif szCurTableName == "AccGift" then		
		local accGiftInfo = GameWizard:getAccGiftInfo();		
		local curSelAccInfo = nil
		for i=1,accGiftInfo.Count do
			if accGiftInfo.AccGifts[i-1].ID == nCurSelIndex then
				curSelAccInfo = accGiftInfo.AccGifts[i-1]
				break
			end
		end
		if curSelAccInfo ~= nil then
			if curSelAccInfo.RoleID == 0 then
				MessageBox("","���˺�ֻ����ȡһ�Σ���ȷ��Ҫ��ȡ����ǰ��ɫ��");
				MessageBoxFrame:SetClientString("��ȡ����");
				MessageBoxFrame:SetClientUserData( 0,curSelAccInfo.ID );
			else
				GameWizard:sendGiftRequest( 0,curSelAccInfo.ID,CLIENT_GIFT_TYPE_ACCGIFT );
			end
		end				
		return;
	elseif szCurTableName == "PropGift" then		
		local propGiftInfo = GameWizard:getPropGiftInfo();
		if nCurSelIndex - 1 < propGiftInfo.Count then
			nType = CLIENT_GIFT_TYPE_PROPGIFT;
			nGiftId = propGiftInfo.PropGifts[nCurSelIndex - 1].ID;
		else
			ShowMidTips( "��ѡ����ȷ�����" );
			return;
		end
	elseif szCurTableName == "VipNetBarGift" then
		local BarGift = GameWizard:GetVipNetBarGift(nCurSelIndex-1);
		nType = CLIENT_GIFT_TYPE_ONEDAY;
		nLv = 0;
		nGiftId = BarGift.GiftID;
	end
	
	if nType ~= 0 then
		GameWizard:sendGiftRequest( nLv, nGiftId, nType );
	end
end

local tCDKeyResponeText = {
								[CDKEY_FAIL_NO_TOLUA] = "�����ȡ�ɹ�����ע������ʼ�",
								[CDKEY_FAIL_VAILD_TOLUA] = "������Ĺ�����Ȩ�𿨺Ŵ���",
								[CDKEY_FAIL_USE_TOLUA] = "������Ĺ�����Ȩ�𿨺��ѱ�ʹ��",
								[CDKEY_FAIL_OVERDUE_TOLUA] = "������Ĺ�����Ȩ�𿨺��ѹ���",
								[CDKEY_FAIL_DUMP_TOLUA] = "�ظ���ȡ��ͬ���͵����",
								[CDKEY_FAIL_SYS_TOLUA] = "ϵͳ����",
								[CDKEY_FAIL_MANY_TOLUA] = "�����ɫͬʱ��ʹ�øÿ���",
								[CDKEY_FAIL_LEVEL_LOW_TOLUA] = "��ǰ��ɫ�ȼ�����",
								[CDKEY_FAIL_LEVEL_HIGH_TOLUA] = "��ǰ��ɫ�ȼ�����"
							}

local tPlayGiftResponeText = {
								[CDKEY_FAIL_NO_TOLUA] = "�����ȡ�ɹ�����ע������ʼ�",
								[CDKEY_FAIL_VAILD_TOLUA] = "������ĳ���������Ŵ���",
								[CDKEY_FAIL_USE_TOLUA] = "������ĳ�����������ѱ�ʹ��",
								[CDKEY_FAIL_OVERDUE_TOLUA] = "������ĳ�����������ѹ���",
								[CDKEY_FAIL_DUMP_TOLUA] = "�ظ���ȡ��ͬ���͵����",
								[CDKEY_FAIL_SYS_TOLUA] = "ϵͳ����",
								[CDKEY_FAIL_MANY_TOLUA] = "�����ɫͬʱ��ʹ�øÿ���",
								[CDKEY_FAIL_LEVEL_LOW_TOLUA] = "��ǰ��ɫ�ȼ�����",
								[CDKEY_FAIL_LEVEL_HIGH_TOLUA] = "��ǰ��ɫ�ȼ�����"
							}

local tKitResponeText = {
								[CDKEY_FAIL_NO_TOLUA] = "�����ȡ�ɹ�����ע������ʼ�",
								[CDKEY_FAIL_VAILD_TOLUA] = "������ĵ���������Ŵ���",
								[CDKEY_FAIL_USE_TOLUA] = "������ĵ�����������ѱ�ʹ��",
								[CDKEY_FAIL_OVERDUE_TOLUA] = "������ĵ�����������ѹ���",
								[CDKEY_FAIL_DUMP_TOLUA] = "�ظ���ȡ��ͬ���͵����",
								[CDKEY_FAIL_SYS_TOLUA] = "ϵͳ����",
								[CDKEY_FAIL_MANY_TOLUA] = "�����ɫͬʱ��ʹ�øÿ���",
								[CDKEY_FAIL_LEVEL_LOW_TOLUA] = "��ǰ��ɫ�ȼ�����",
								[CDKEY_FAIL_LEVEL_HIGH_TOLUA] = "��ǰ��ɫ�ȼ�����"
							}
function GetCDKeyResponse( nSucc )
	if g_nFocus == g_nFocus_SPECIALGIFT then
		MessageBoxOK( "",tCDKeyResponeText[nSucc] );
	elseif g_nFocus == g_nFocus_CHANGYOUGIFT then
		MessageBoxOK( "",tPlayGiftResponeText[nSucc] );
	elseif g_nFocus == g_nFocus_KITGIFT then
		MessageBoxOK( "",tKitResponeText[nSucc] );
	end
	if nSucc == CDKEY_FAIL_NO_TOLUA then		
		GuideGiftFrame_CDKeyEdit:SetText( "" );
	end
end

local ShowClanPworldGuideTipsPOP = false;
function XinshouYinDaoGuiShow( szLinkBtnName, szRichText )
	local linkBtnGuid = getglobal( szLinkBtnName.."Guide" );
	local linkBtnRich = getglobal( szLinkBtnName.."Rich" );
	local CloseBtn = getglobal( szLinkBtnName.."GuideCloseBtn" )
	linkBtnRich:SetText( szRichText, 86, 79, 54 );
	linkBtnRich:Show();
	linkBtnGuid:Show();	
	CloseBtn:Show();
end
function HideXinshouYinDaoGui( szLinkBtnName )
	local GuideBtn = getglobal( szLinkBtnName.."Guide" );
	local RichBtn = getglobal( szLinkBtnName.."Rich" );
	local CloseBtn = getglobal( szLinkBtnName.."GuideCloseBtn" )
	GuideBtn:Hide();
	RichBtn:Hide();
	CloseBtn:Hide();
	ShowClanPworldGuideTipsPOP = false;
end
function XinShouCenterGuideButtonGuideCloseBtn_onClick()
	HideXinshouYinDaoGui( this:GetParent() );
end

-- [index]
-- TYPE����
CENTER_XINSHOU_BTN_ITEM		= 0;
CENTER_XINSHOU_BTN_LEVEL	= 1;
CENTER_XINSHOW_BTN_MSG		= 2;
CENTER_CLAN_PWORLD			= 3;
CENTER_CLAN_OPEN_TOUCH		= 4;
CENTER_CLAN_SPY				= 5;
CENTER_CLAN_MACHINE_PWORLD	= 6;
INTELLIGENCE_RACE_JOIN		= 7;
--���������type
WHETHER_REAL_BOSS		= 8;
COWBOY_BUSY_FINALS		= 9;
HEAVEN_PIE_ACTIVITY		= 10;
HAPPY_CARNIVAL_ACTIVITY		= 11;

CENTER_CLAN_STRONG_WAR		= 12;
CENTER_WORLD_CITY_WAR		= 13;
CENTER_XINSHOU_SPAR_EQURATE	= 14;
CENTER_CLAN_BANNER_GLORY	= 15;

DARK_COMING_ACTIVITY		= 16;
LIGHT_SHADOW_FIGHT		= 17;
MACHINE_WAR_ACTIVITY	= 18;
SPARTA_PWORLD_ACTIVITY	= 19;
CLAN_DART_DELIVERY		= 20;
LIFE_IS_MOVEMENT_ACTIVITY	= 21;

GUCHENG_BATTLE_ACTIVITY		= 22;    -- �´�ʧ��֮��

TOP_CAREER_INVITE_ACTIVITY		= 23;		--�۷�֮ս����
TOP_CAREER_TRANS_ACTIVITY		= 24;		--�۷�֮ս����

TANK_WAR_ACTIVITY				= 25;		--̹�˴�ս

CENTER_CLAN_SPAN_STRONG_WAR		= 26;		--����ݵ�ս

-- �������ְ�ť����
MAX_CENTER_BTS = 5;

local t_centerXinShouBtnControl = 
{

STANDARD_MOVE_SPEED = 180, 
--STANDARD_MOVE_TIME = 10,

-- ����ײ����
-- ENTER_BTN_TOPLEFT_POS
[6]	= { anchor = { selfPoint = "topleft", relPoint = "topleft", reframe = "XinShouCenterGuideFrame", xOffset = 559,	yOffset = 0 } },

-- HIT_FIRST_BTN_POS
[1] = { anchor = { selfPoint = "topleft", relPoint = "topleft", reframe = "XinShouCenterGuideFrame", xOffset = 46,	yOffset = 0 } },
-- HIT_SECOND_BTN_POS
[2] = { anchor = { selfPoint = "topleft", relPoint = "topleft", reframe = "XinShouCenterGuideFrame", xOffset = 96,	yOffset = 0 } },
--  HIT_THIRD_BTN_POS
[3] = { anchor = { selfPoint = "topleft", relPoint = "topleft", reframe = "XinShouCenterGuideFrame", xOffset = 146,	yOffset = 0 } },
-- HIT_FORTH_BTN_POS
[4] = { anchor = { selfPoint = "topleft", relPoint = "topleft", reframe = "XinShouCenterGuideFrame", xOffset = 196,	yOffset = 0 } },
--  HIT_FIFTH_BTN_POS
[5] = { anchor = { selfPoint = "topleft", relPoint = "topleft", reframe = "XinShouCenterGuideFrame", xOffset = 246,	yOffset = 0 } },

showXinShouBtnVector = {},
extraBtnMoveInfo = { lastTime = 0 },

};

-- data = { itemDefId = , Type =

-- ɾ��ĳ�����͵İ�ť
t_centerXinShouBtnControl["ClearOneTypetXinShouData"] = 
function ( self, destData )
	local t_tmpBuffer = {};
	for _, data in ipairs( self["showXinShouBtnVector"] ) do
		if destData["Type"] ~= data["Type"] then
			table.insert( t_tmpBuffer, data );
		end
	end
	
	for i = 1, table.getn( self["showXinShouBtnVector"] ) do
		table.remove( self["showXinShouBtnVector"] );
	end
	
	for i = 1, table.getn( t_tmpBuffer ) do
		table.insert( self["showXinShouBtnVector"], t_tmpBuffer[i] );
	end
end

-- ��õ�ǰҪ��ʾ��ť������
t_centerXinShouBtnControl["GetXinShouDataNum"] = 
function ( self )
	return table.getn( self["showXinShouBtnVector"] );
end

-- �������Ϊindex�ϵ��߼�����
t_centerXinShouBtnControl["GetXinShouDataByIndex"] = 
function ( self, targetIndex )
	--assert( targetIndex <= table.getn( self["showXinShouBtnVector"] ) );
	if targetIndex > table.getn( self["showXinShouBtnVector"] ) then
		return;
	end

	return self["showXinShouBtnVector"][targetIndex];
end

-- ��ʾ������ƶ��İ�ť
t_centerXinShouBtnControl["ShowExtraBtn"] = 
function ( self, data )
	self:AddNewCenterBtn( data );
	local extraBtn = getglobal( "ExtraXinShouCenterGuideButton" );
	extraBtn:SetClientUserData( 0, table.getn( self["showXinShouBtnVector"] ) ); -- �����߼�����]
	
	if data["Type"] == CENTER_CLAN_PWORLD then
		--GameWizard:zeroSetp();
		if not GameWizard:isDo( GAMEWIZARD_FIRST_GET_CLAN_PWORLD ) then	
			GameWizard:doStep(GAMEWIZARD_FIRST_GET_CLAN_PWORLD);
			ShowClanPworldGuideTipsPOP = true;
			XinshouYinDaoGuiShow( "ExtraXinShouCenterGuideButton", "  ��������\n  ��������\n  ������������");
		end
	elseif data["Type"] == CENTER_CLAN_BANNER_GLORY then
		--GameWizard:zeroSetp();
		if not GameWizard:isDo( GAMEWIZARD_FIRST_GET_CLAN__BANNER_GLORY ) then	
			GameWizard:doStep(GAMEWIZARD_FIRST_GET_CLAN__BANNER_GLORY);
			ShowClanPworldGuideTipsPOP = true;
			XinshouYinDaoGuiShow( "ExtraXinShouCenterGuideButton", "  ��������\n  ��������\n  ����ս����ҫ");
		end
	elseif data["Type"] == CLAN_DART_DELIVERY then 
		--GameWizard:zeroSetp();
		FirstShowClanDartDeliveryCenterBtn();
	end
	extraBtn:Show();
end

function FirstShowClanDartDeliveryCenterBtn()
	if not GameWizard:isDo( GAMEWIZARD_FIRST_CLAN_DART_DELIVERY ) then	
		GameWizard:doStep(GAMEWIZARD_FIRST_CLAN_DART_DELIVERY);
		ShowClanPworldGuideTipsPOP = true;
		XinshouYinDaoGuiShow( "ExtraXinShouCenterGuideButton", "  ��������\n  ��������\n  ������������");
	end
end

-- ����һ���µ��м䰴ť�߼�
t_centerXinShouBtnControl["AddNewCenterBtn"] = 
function ( self, data )
	table.insert( self["showXinShouBtnVector"], data );
end

-- ������е��м䰴ť
t_centerXinShouBtnControl["clear"] = 
function ( self )
	for i = 1, table.getn( self["showXinShouBtnVector"] ) do
		table.remove( self["showXinShouBtnVector"] );
	end
end

-- ����ID�������ť
t_centerXinShouBtnControl["eraseByDefId"] = 
function ( self, targetData )
	for index, data in ipairs( self["showXinShouBtnVector"] ) do
		if targetData["Type"] == CENTER_XINSHOU_BTN_ITEM and targetData["Type"] == data["Type"] and
			targetData["itemDefId"] == data["itemDefId"] then

			table.remove( self["showXinShouBtnVector"], index );
			break;
		end
	end
end

-- �����߼������������ť
t_centerXinShouBtnControl["eraseByIndex"] = 
function ( self, targetIndex )
	table.remove( self["showXinShouBtnVector"], targetIndex );
end
--����MsgBtnƯ�����ݵ������������ť���˴�д���߻��ű�ɾ���̶�id������
t_centerXinShouBtnControl["eraseMsgBtnByIndex"] = 
function ( self, targetData )
	for index, data in ipairs( self["showXinShouBtnVector"] ) do
		if targetData["index"] == data["index"] then
			table.remove( self["showXinShouBtnVector"], index );
			break;
		end
	end
end
--[[
--�����������tips
t_centerXinShouBtnControl["eraseClanPworldTip"] =
function ( self )
	table.remove( self["showXinShouBtnVector"], ClanPworldGameTipsIndex );
	ClanPworldGameTipsIndex = 0;
end
--]]

t_centerXinShouBtnControl["getIndexByType"] = 
function ( self, nType )
	for idx, data in ipairs( self["showXinShouBtnVector"] ) do
		if nType == data["Type"] then
			return idx
		end
	end
	return 0;
end

local szXinShouItemIconPath = "uires\\TuPianLei\\PiaoYiTiShiTuBiao\\";
function GetXinShouMoveBtn( nIconID )
	local IconPath = szXinShouItemIconPath..nIconID..".tga";
	return IsInExistence( IconPath ) and IconPath or DEFAULT_ITEM_PATH;
end

-- �������е��м䰴ť
t_centerXinShouBtnControl["updateCenterBtns"] = 
function ( self )
	local nBtnIndex = 1;
	local nLogicBeginIndex = table.getn( self["showXinShouBtnVector"] ) - MAX_CENTER_BTS + 1;
	nLogicBeginIndex = ( nLogicBeginIndex < 1 ) and 1 or nLogicBeginIndex;
	for i = nLogicBeginIndex, table.getn( self["showXinShouBtnVector"] ) do
		local btn			= getglobal( "XinShouCenterGuideButton"..nBtnIndex );
		local data			= self["showXinShouBtnVector"][i];
		
		local szIconPath	= DEFAULT_ITEM_PATH;
		if data["Type"] == CENTER_XINSHOU_BTN_ITEM then	-- CENTER_XINSHOU_BTN_ITEM
			szIconPath = GetXinShouMoveBtn( getItemDef( data["itemDefId"] ).IconID );
		else
			szIconPath = data["path"];
		end

		local t_tgas = { [btn:GetName().."IconTexture"] = { path = szIconPath }, };
		util.SetTgas( t_tgas );
		btn:SetClientUserData( 0, i ); -- �����߼�����
		if not btn:IsShown() then
			btn:Show();
		end
		if ShowClanPworldGuideTipsPOP and data["Type"] == CENTER_CLAN_PWORLD then
			XinshouYinDaoGuiShow( btn:GetName(), "  ��������\n  ��������\n  ������������");
		elseif ShowClanPworldGuideTipsPOP and data["Type"] == CLAN_DART_DELIVERY then
			XinshouYinDaoGuiShow( btn:GetName(), "  ��������\n  ��������\n  ������������");
		end
		nBtnIndex = nBtnIndex + 1;
	end

	for i = nBtnIndex, MAX_CENTER_BTS do
		local btn = getglobal( "XinShouCenterGuideButton"..i );
		btn:Hide();
	end
end

-- �߼�����
t_centerXinShouBtnControl["update"] = 
function ( self )
	local extraBtn = getglobal( "ExtraXinShouCenterGuideButton" );
	if not extraBtn:IsShown() then
		return;
	end
	
	-- lastShowBtn�����һ��û��ʾ�İ�ť
	local lastShowBtn = getglobal( "XinShouCenterGuideButton5" );
	--local firstShowBtn;
	for i = 1, MAX_CENTER_BTS do
		local btn = getglobal( "XinShouCenterGuideButton"..i );
		if not btn:IsShown() then
			lastShowBtn = btn;
			break;
		end
	end
	
	-- ֻҪ�����һ��û��ʾ�İ�ť��ײ����ȫ������
	-- �ȼ��
	local fCurAchorOffsetX	= extraBtn:GetAnchorOffsetX();
	local anchorData		= self[extraBtn:GetClientID()]["anchor"];

	if isTwoFrameXConflict( lastShowBtn, extraBtn ) then
		extraBtn:Hide();
		self:updateCenterBtns();
		return;
	end
	
	local nCurTime		= GameMgr:getTickTime();
	local fDeltaTime	= ( nCurTime - t_centerXinShouBtnControl["extraBtnMoveInfo"]["lastTime"] ) / 1000;
	local xOffsetChg	= ( ( nCurTime - t_centerXinShouBtnControl["extraBtnMoveInfo"]["lastTime"] ) / 1000 ) * self["STANDARD_MOVE_SPEED"];
	-- ��β��ܸ������λ�ã���Ҫ��һ��Խ���⣬��2������һ��Ҫ�ֿ�
	local fAdustOffsetY = 0;
	--[[
	if GetScreenHeight() > DEFAULT_UI_HEIGHT then
		fAdustOffsetY = ( GetScreenHeight() - DEFAULT_UI_HEIGHT ) / 2;
	end
	--]]
	extraBtn:SetPoint( anchorData["selfPoint"], anchorData["reframe"], anchorData["relPoint"], 
						anchorData["xOffset"] - xOffsetChg, anchorData["yOffset"] + fAdustOffsetY );
	
	--if fCurAchorOffsetX <= anchorData["xOffset"] then
	if isTwoFrameXConflict( lastShowBtn, extraBtn ) then
		extraBtn:Hide();
		self:updateCenterBtns();
		return;
	end
end

function GetCenterXinShouBtnControl()
	return t_centerXinShouBtnControl;
end

function XinShouCenterGuideButton_OnHide()
	local nClientID = this:GetClientID();
	if nClientID == 6 then		
		local anchorData = t_centerXinShouBtnControl[nClientID]["anchor"];
		this:SetPoint( anchorData["selfPoint"], anchorData["reframe"], anchorData["relPoint"], 
						anchorData["xOffset"],  anchorData["yOffset"] );
	end
end

-- �����������������ʾ
function XinShouCenterGuideButton_OnShow()
	local animationTexture = getglobal( this:GetName().."UVAnimationTex" );
	animationTexture:SetUVAnimation( 50, true );
	animationTexture:Show();
	
	local nClientID = this:GetClientID();
	if nClientID == 6 then	
		local nLogicIndex	= this:GetClientUserData( 0 );
		local t_xinShouData = t_centerXinShouBtnControl:GetXinShouDataByIndex( nLogicIndex );
		local szIconPath	= DEFAULT_ITEM_PATH;
		if t_xinShouData["Type"] == CENTER_XINSHOU_BTN_ITEM then	-- CENTER_XINSHOU_BTN_ITEM
			szIconPath = GetXinShouMoveBtn( getItemDef( t_xinShouData["itemDefId"] ).IconID );
		else
			szIconPath = t_xinShouData["path"];
		end

		local t_tgas = { [this:GetName().."IconTexture"] = { path = szIconPath }, };
		util.SetTgas( t_tgas );

		local anchorData = t_centerXinShouBtnControl[nClientID]["anchor"];
		local fAdustOffsetY = 0;
		if GetScreenHeight() > DEFAULT_UI_HEIGHT then
			fAdustOffsetY = ( GetScreenHeight() - DEFAULT_UI_HEIGHT ) / 2;
		end

		this:SetPoint( anchorData["selfPoint"], anchorData["reframe"], anchorData["relPoint"], 
						anchorData["xOffset"],  anchorData["yOffset"] + fAdustOffsetY );
		t_centerXinShouBtnControl["extraBtnMoveInfo"] = { lastTime = GameMgr:getTickTime() };
	end
end

local t_desc = { [CENTER_XINSHOU_BTN_ITEM] = "CENTER_XINSHOU_BTN_ITEM", [CENTER_XINSHOU_BTN_LEVEL] = "CENTER_XINSHOU_BTN_LEVEL" };
function XinShouCenterGuideButton_OnClick()
	local nLogicIndex = this:GetClientUserData( 0 );
	-- TODO: �Ժ���������ٵ�����Ŀǰ��Ӳ��
	local data = t_centerXinShouBtnControl:GetXinShouDataByIndex( nLogicIndex );
	local frame = getglobal( data["frame"] );
	if data["Type"] == CENTER_XINSHOU_BTN_ITEM then
		frame:SetClientUserData( 0, data["realMapID"] );
		frame:SetClientUserData( 1, data["xinShouType"] );
		frame:Show();
	elseif data["Type"] == CENTER_XINSHOU_BTN_LEVEL then
		ShowLevelUpTipFrame( data["level"] );
	elseif data["Type"] == CENTER_XINSHOW_BTN_MSG then
		ShowLevelUpMsgTip( data["index"] );
	elseif data["Type"] == CENTER_CLAN_PWORLD then
		HideXinshouYinDaoGui( this:GetName() );
		if ClanPworldFramShow then
			ClanPworldTipFrame:Hide();
		else
			ClanPworldTipFrame:Show();
		end
		return
	elseif data["Type"] == CENTER_CLAN_OPEN_TOUCH then
		if ClanOpenTouchTipFrame:IsShown() then
			ClanOpenTouchTipFrame:Hide();
		else
			ClanOpenTouchTipFrame:Show();
		end
		local TipsUVAnimTex = getglobal( this:GetName() .. "UVAnimationTex" )
		if TipsUVAnimTex ~= nil then
			TipsUVAnimTex:Hide();
		end
		return;
	elseif data["Type"] == CENTER_CLAN_SPY then
		if ClanIntellingenceFrame:IsShown() then
			ClanIntellingenceFrame:Hide();
		else
			ClanIntellingenceFrame:Show();
		end
		local TipsUVAnimTex = getglobal( this:GetName() .. "UVAnimationTex" )
		if TipsUVAnimTex ~= nil then
			TipsUVAnimTex:Hide();
		end
		return;
	elseif data["Type"] == CENTER_CLAN_MACHINE_PWORLD then
		if ClanMachinePworldFrame:IsShown() then
			ClanMachinePworldFrame:Hide();
		else
			ClanMachinePworldFrame:Show();
		end
		local TipsUVAnimTex = getglobal( this:GetName() .. "UVAnimationTex" )
		if TipsUVAnimTex ~= nil then
			TipsUVAnimTex:Hide();
		end
		return;
	elseif data["Type"] == INTELLIGENCE_RACE_JOIN then
		if IntelligenceRaceJoinFrame:IsShown() then
			IntelligenceRaceJoinFrame:Hide();
		else
			IntelligenceRaceJoinFrame:Show();
		end
		local TipsUVAnimTex = getglobal( this:GetName() .. "UVAnimationTex" )
		if TipsUVAnimTex ~= nil then
			TipsUVAnimTex:Hide();
		end
		return;
	elseif data["Type"] == HAPPY_CARNIVAL_ACTIVITY then
		if HappyCarnivalGuidFrame:IsShown() then
			HappyCarnivalGuidFrame:Hide();
		else
			HappyCarnivalGuidFrame:SetClientUserData(0, data["nActiveID"]);
			HappyCarnivalGuidFrame:Show();
		end
		local TipsUVAnimTex = getglobal( this:GetName() .. "UVAnimationTex" )
		if TipsUVAnimTex ~= nil then
			TipsUVAnimTex:Hide();
		end
		return;
	elseif data["Type"] == LIGHT_SHADOW_FIGHT then
		if HappyCarnivalGuidFrame:IsShown() then
			HappyCarnivalGuidFrame:Hide();
		else
			HappyCarnivalGuidFrame:SetClientUserData(0, data["nActiveID"]);
			HappyCarnivalGuidFrame:Show();
		end
		local TipsUVAnimTex = getglobal( this:GetName() .. "UVAnimationTex" )
		if TipsUVAnimTex ~= nil then
			TipsUVAnimTex:Hide();
		end
		return;
	elseif data["Type"] == CENTER_CLAN_STRONG_WAR then
		if StrongWarInfoFrame:IsShown() then
			StrongWarInfoFrame:Hide();
		else
			StrongWarInfoFrame:Show();
		end
		local TipsUVAnimTex = getglobal( this:GetName() .. "UVAnimationTex" )
		if TipsUVAnimTex ~= nil then
			TipsUVAnimTex:Hide();
		end
		return;
	elseif data["Type"] == CENTER_WORLD_CITY_WAR then
		if WorldCityWarInfoFrame:IsShown() then
			WorldCityWarInfoFrame:Hide();
		else
			WorldCityWarInfoFrame:Show();
		end
		local TipsUVAnimTex = getglobal( this:GetName() .. "UVAnimationTex" )
		if TipsUVAnimTex ~= nil then
			TipsUVAnimTex:Hide();
		end
		return;
	elseif data["Type"] == CENTER_CLAN_BANNER_GLORY then
		if ClanBannerGloryFrame:IsShown() then
			ClanBannerGloryFrame:Hide()
		else
			ClanBannerGloryFrame:Show()
		end
		local TipsUVAnimTex = getglobal( this:GetName() .. "UVAnimationTex" )
		if TipsUVAnimTex ~= nil then
			TipsUVAnimTex:Hide();
		end
		return;
	elseif data["Type"] >=  WHETHER_REAL_BOSS and data["Type"] <= HEAVEN_PIE_ACTIVITY then
		SpecialActiveGuidBtn_OnClick( data["nActiveID"] );
	elseif data["Type"] == TANK_WAR_ACTIVITY then
		if HappyCarnivalGuidFrame:IsShown() then
			HappyCarnivalGuidFrame:Hide();
		else
			HappyCarnivalGuidFrame:SetClientUserData(0, data["nActiveID"]);
			HappyCarnivalGuidFrame:Show();
		end
		local TipsUVAnimTex = getglobal( this:GetName() .. "UVAnimationTex" )
		if TipsUVAnimTex ~= nil then
			TipsUVAnimTex:Hide();
		end
		return;
	elseif data["Type"] ==  DARK_COMING_ACTIVITY then
		if HappyCarnivalGuidFrame:IsShown() then
			HappyCarnivalGuidFrame:Hide();
		else
			HappyCarnivalGuidFrame:SetClientUserData(0, data["nActiveID"]);
			HappyCarnivalGuidFrame:Show();
		end
		local TipsUVAnimTex = getglobal( this:GetName() .. "UVAnimationTex" )
		if TipsUVAnimTex ~= nil then
			TipsUVAnimTex:Hide();
		end
		return;
	elseif data["Type"] == CENTER_XINSHOU_SPAR_EQURATE then
		if frame:IsShown() then
			if CostMoneyConfirmFrame:IsShown() then
				return ShowMidTips("���ȹرմ�ȷ�ϵ���ʾ��");
			end
			if getResolveState() then
				return ShowMidTips( "�ֽ��У��޷�ִ�в���" );
			end
		end
		GameWizard:doStep( GAMEWIZARD_FIRST_SPAR_EQURATE );
		PlayerContainerSparEpurateFrame:Show();
	elseif data["Type"] == MACHINE_WAR_ACTIVITY or data["Type"] == LIFE_IS_MOVEMENT_ACTIVITY then
		if HappyCarnivalGuidFrame:IsShown() then
			HappyCarnivalGuidFrame:Hide();
		else
			HappyCarnivalGuidFrame:SetClientUserData(0, data["nActiveID"]);
			HappyCarnivalGuidFrame:Show();
		end
		local TipsUVAnimTex = getglobal( this:GetName() .. "UVAnimationTex" )
		if TipsUVAnimTex ~= nil then
			TipsUVAnimTex:Hide();
		end
		return;
	elseif data["Type"] == SPARTA_PWORLD_ACTIVITY then
		if SpartaPworldGuidFrame:IsShown() then
			SpartaPworldGuidFrame:Hide();
		else
			SpartaPworldGuidFrame:SetClientUserData(0, data["nActiveID"]);
			SpartaPworldGuidFrame:Show();
		end
		local TipsUVAnimTex = getglobal( this:GetName() .. "UVAnimationTex" )
		if TipsUVAnimTex ~= nil then
			TipsUVAnimTex:Hide();
		end
		return;
	elseif data["Type"] == TOP_CAREER_INVITE_ACTIVITY or data["Type"] == TOP_CAREER_INVITE_ACTIVITY then
		if TopCareerInviteJoinFrame:IsShown() then
			TopCareerInviteJoinFrame:Hide();
		else
			TopCareerInviteJoinFrame:Show();
		end
		local TipsUVAnimTex = getglobal( this:GetName() .. "UVAnimationTex" )
		if TipsUVAnimTex ~= nil then
			TipsUVAnimTex:Hide();
		end
		return;
	elseif data["Type"] == TOP_CAREER_TRANS_ACTIVITY or data["Type"] == TOP_CAREER_TRANS_ACTIVITY then
		if TopCareerTransFrame:IsShown() then
			TopCareerTransFrame:Hide();
		else
			TopCareerTransFrame:Show();
		end
		local TipsUVAnimTex = getglobal( this:GetName() .. "UVAnimationTex" )
		if TipsUVAnimTex ~= nil then
			TipsUVAnimTex:Hide();
		end
		return;
	elseif data["Type"] == CLAN_DART_DELIVERY then
		UpdateCenterFrameShowState( "ClanDartDeliveryFrame", this ,data["nActiveID"] );
		return;
	elseif data["Type"] == GUCHENG_BATTLE_ACTIVITY then
		UpdateCenterFrameShowState( "GuChengBattleFrame", this ,data["nActiveID"] );
		return;
	elseif data["Type"] == CENTER_CLAN_SPAN_STRONG_WAR then
		if SpanStrongWarInfoFrame:IsShown() then
			SpanStrongWarInfoFrame:Hide();
		else
			SpanStrongWarInfoFrame:Show();
		end
		local TipsUVAnimTex = getglobal( this:GetName() .. "UVAnimationTex" )
		if TipsUVAnimTex ~= nil then
			TipsUVAnimTex:Hide();
		end
		return;
	end
	this:Hide();
	t_centerXinShouBtnControl:eraseByIndex( nLogicIndex );
	t_centerXinShouBtnControl:updateCenterBtns();
end
-- ������Ӧframe����
function UpdateCenterFrameShowState( nFrameName, nBtn ,nActiveID )
	local frame = getglobal( nFrameName );
	if frame:IsShown() then
		frame:Hide();
	else
		frame:SetClientUserData(0, nActiveID);
		frame:Show();
	end
	local TipsUVAnimTex = getglobal( nBtn:GetName() .. "UVAnimationTex" )
	if TipsUVAnimTex ~= nil and TipsUVAnimTex:IsShown() then
		TipsUVAnimTex:Hide();
	end
end

function SetClanPworldFrameShow( Show )
	ClanPworldFramShow = Show;
	local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
	local nIdx = t_centerXinShouBtnControl:getIndexByType( CENTER_CLAN_PWORLD )
	if Show == true and nIdx ~= nil then
		local TipsUVAnimTex = getglobal( "XinShouCenterGuideButton" .. nIdx.."UVAnimationTex" )
		if TipsUVAnimTex ~= nil then
			TipsUVAnimTex:Hide();
		end
	end
end

function XinShouCenterGuideButton_OnEnter()
	local nLogicIndex	= this:GetClientUserData( 0 );
	local t_xinShouData = t_centerXinShouBtnControl:GetXinShouDataByIndex( nLogicIndex );
	if t_xinShouData["Type"] == CENTER_XINSHOU_BTN_ITEM then
		local itemDef	= getItemDef( t_xinShouData["itemDefId"] );
		local szText;
		if math.floor( t_xinShouData["itemDefId"]/1000 ) == 1420 then
			szText = "����ð������Դ򿪹���ʹ��#cff0000��ʱ����-���浶#n��ָ�����ܡ�"
		else
			szText = "����ð�ť���Դ򿪹���ʹ��"..itemDef.Name.."��ָ������";
		end
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == CENTER_XINSHOU_BTN_LEVEL then
		local szText = "����ð�ť���Դ�������ʾ";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == CENTER_CLAN_PWORLD then
		local szText = "����򿪡��������֡�\n������������";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == CENTER_CLAN_OPEN_TOUCH then
		local szText = "����򿪡��������֡�\n---���ᱦ��";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == CENTER_CLAN_SPY then
		local szText = "����򿪡��������֡�\n---�鱨����ս";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == CENTER_CLAN_MACHINE_PWORLD then
		local szText = "����򿪡��������֡�\n---�ػ�������Ŧ";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == INTELLIGENCE_RACE_JOIN then
		local szText = "����򿪡�������������֡�\n��ȫ����������";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == WHETHER_REAL_BOSS then
		local szText = "����򿪡�����֡�\n��ħӰ����";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == COWBOY_BUSY_FINALS then
		local szText = "����򿪡�����֡�\n��ţ�к�æ-����";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == HAPPY_CARNIVAL_ACTIVITY then
		local szText = "����򿪡�����֡�\n�����ּ��껪";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == TANK_WAR_ACTIVITY then
		local szText = "����򿪡�����֡�\n����ը��ս";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == HEAVEN_PIE_ACTIVITY then
		local szText = "����򿪡�����֡�\n���콵�ḣ";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == DARK_COMING_ACTIVITY then
		local szText = "����򿪡�����֡�\n���ڰ���Ϯ";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == LIGHT_SHADOW_FIGHT then
		local szText = "����򿪡�����֡�\n����Ӱ֮ս";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == CENTER_CLAN_STRONG_WAR then
		local szText = "����򿪡��ݵ�ս��\n---ս����Ϣ";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == CENTER_WORLD_CITY_WAR then
		local szText = "����򿪡�ʱ�������ս��\n---ս����Ϣ";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == CENTER_XINSHOU_SPAR_EQURATE then
		local szText = "����ð�ť���Դ򿪾�ʯ�������";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == CENTER_CLAN_BANNER_GLORY then
		local szText = "����򿪡�����֡�\n��ս����ҫ";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == MACHINE_WAR_ACTIVITY then
		local szText = "����򿪡�����֡�\n�����ʱ��";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == LIFE_IS_MOVEMENT_ACTIVITY then
		local szText = "����򿪡�����֡�\n�����������˶�";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == SPARTA_PWORLD_ACTIVITY then
		local szText = "����򿪡�����֡�\n��ĩ����ҫ����";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == CLAN_DART_DELIVERY then
		local szText = "����򿪡�����֡�\n����������";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == GUCHENG_BATTLE_ACTIVITY then
		local szText = "����򿪡�����֡�\n���´�ʧ��֮��";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == TOP_CAREER_INVITE_ACTIVITY then
		local szText = "����򿪡�����֡�\n��ĩ���۷�-����";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == TOP_CAREER_TRANS_ACTIVITY then
		local szText = "����򿪡�����֡�\n��ĩ���۷�-����";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == CENTER_CLAN_SPAN_STRONG_WAR then
		local szText = "����򿪡�����ݵ�ս��\n---ս����Ϣ";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	end
end

function XinShouCenterGuideButton_OnLeave()
	GameTooltip:Hide();
end

function InitGuideData()
	t_centerXinShouBtnControl:clear();
	local clanPworldTipFrame = getglobal( "ClanPworldTipFrame" );
	clanPworldTipFrame:Hide();
end

function GuideFrame_ItemButton_OnEnter()
	local itemid = this:GetClientUserData( 0 );
	ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, getItemDef( itemid ) );
end

function GuideFrameGetAccGiftNum( itemid )
	local accGiftInfo = GameWizard:getAccGiftInfo();
	for i=1,accGiftInfo.Count do
		for j=1, MAX_ACC_GIFT_LIST_TOLUA do
			if accGiftInfo.AccGifts[i-1].GiftItems[j-1].ItemID == itemid then				
				return accGiftInfo.AccGifts[i-1].GiftItems[j-1].ItemNum;
			end
		end
	end
	return 0;
end

function TmpItemPickTipsFrame_OnShow()
	local actorid = this:GetClientUserData( 0 )
	GameWizard:UpdateDirEntity( actorid );
	this:setUpdateTime( 0 )
end

function TmpItemPickTipsFrame_OnHide()
	this:SetClientUserData( 0, 0 )
	GameWizard:HideDirEntity();
end

function TmpItemPickTipsFrame_OnUpdate()
	local actorid = this:GetClientUserData( 0 )
	local actor = ActorMgr:FindActor( actorid )
	if actor == nil or util.CalActorsDistance(actor, ActorMgr:getMainPlayer()) < 20 then
		this:Hide()
		return
	end
	GameWizard:UpdateDirEntity( actorid );
end

local MAX_TMPITEM_PICK_BUTTON = 10;
function ShowTmpItemPickButton( actorid )
	local actor = ActorMgr:FindActor( actorid )
	if actor == nil or actor:isPickItem() then
		return
	end
	local curBtn = nil;
	for i=1,MAX_TMPITEM_PICK_BUTTON do
		local btn = getglobal( "TmpItemPickTemplateButton" .. i );
		if btn ~= nil and not btn:IsShown() then
			curBtn = btn;
			break;
		elseif btn == nil then
			curBtn = CreateButton( "TmpItemPickTemplateButton" .. i, "TmpItemPickTemplateButton", UIClient )
			curBtn:setUpdateTime( 0 )
			break;
		elseif btn:GetClientUserData( 0 ) == actorid then
			curBtn = btn;
			break;
		end
	end
	if curBtn ~= nil then
		curBtn:SetClientUserData( 0,actorid );
		curBtn:Show();
	end
end

function TmpItemPickTemplateButton_OnShow()
	local tex = getglobal( this:GetName() .. "BkgTex" );
	tex:SetUVAnimation( 100, true );
	local actorid = this:GetClientUserData( 0 );
	local pos = GameWizard:getPlayTopWindowPos( actorid );
	local nScale = GetScreenScaleY();
	pos.x = pos.x - 43;
	pos.y = pos.y - 120 * nScale;
	updateHeadBindingFrame( this:GetName(), pos.x, pos.y, 86*nScale, 65*nScale )
end

function TmpItemPickTemplateButton_OnHide()
	this:SetClientUserData( 0, 0 );
end

function TmpItemPickTemplateButton_OnUpdate()
	local actorid = this:GetClientUserData( 0 )
	local actor = ActorMgr:FindActor( actorid )
	if actor == nil then
		this:Hide()
		return
	end
	local nScale = GetScreenScaleY() < GetScreenScaleX() and GetScreenScaleY() or GetScreenScaleX();
	local pos = GameWizard:getPlayTopWindowPos( actorid );
	pos.x = pos.x - 43 * nScale;
	pos.y = pos.y - 120 * nScale;
	updateHeadBindingFrame( this:GetName(), pos.x, pos.y, 86*nScale, 65*nScale )
end

------------------------------��ֵ���---------------------------------

function GuideGiftFramePropGiftOkBtn_OnClick()
	GameMgr:openWebSite( "http://pay.ljy0.com" );
end

function CheckGiftGetState( index, cash )
	local propGiftInfo = GameWizard:getPropGiftInfo();
	for i=1, propGiftInfo.PropsGift[index].GiftGetNum do
		if GameWizard:compareValue( propGiftInfo.PropsGift[index].GiftGetInfo[i-1].CashUp, cash ) == 0 then
			return true;
		end
	end
	return false;
end

function GuideGiftFramePropGiftGetAwardBtn_OnClick()
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "��ֵ���" then
		return;
	end
	local idx = this:GetClientUserData( 0 );
	if idx ~= 0 then
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer == nil then return end;
		if mainplayer:isDead() then
			AddGameMiddleTips2("����״̬������ȡ���");
			return;
		end
		MessageBox("","���˺�ֻ����ȡһ�Σ���ȷ��Ҫ��ȡ����ǰ��ɫ��");
		MessageBoxFrame:SetClientString("��ֵ���");
		MessageBoxFrame:SetClientUserData( 0,idx );
	end
end

function GetLimitDate( beginDate, endDate )
	beginDate = string2time( beginDate .. "000000");
	endDate = string2time( endDate .. "235959");
	local serverTime = getServerTime().sec;
	if serverTime < beginDate then
		return "δ��ʼ";
	elseif serverTime > endDate then
		return "�ѽ���";
	else
		local day = math.floor( (endDate - serverTime) / 86400 ) + 1;
		return day .. "��";
	end
end

function GiftFrameCheckInButton_OnClick()
	if nCurSelIndex == 1 then 
		GiftCenterGuideCheckInButton_OnClick();
	elseif nCurSelIndex == 2 then 
		GetAwardMTGift();
	end
end
function UpdateGiftCenterGuideAwardMT() 
	local Frame= getglobal( "GuideGiftFrame" );
	if Frame:IsShown() then
		Frame:Hide();
	else
		Frame:Show();
		nCurSelType	 = CLIENT_GIFT_TYPE_CONT_COVER;
		nCurSelSubType 	= 2;
		nCurSelIndex 	= 2;
		GuideGiftFrame_CheckInBtn_OnClick();
	end
	local uvTexture = getglobal("GiftCenterGuideAwardMTButtonUVAnimationTex");
	uvTexture:Hide();
end

----------------�½���ɫ�����ѡ���ӽ�ģʽ����--------------------------------------
function SelectCameraModeGuideFrame_OnShow()
	SelectCameraModeGuideFrameInfo:SetText( "������Ϸ���ͨ��#n#c4eccdbEsc-��Ϸ����-ʹ��3D��������#n���и���", 255,255,190 )
	SelectCameraModeGuideFrameMode1:Checked()
	SelectCameraModeGuideFrameMode2:DisChecked()
end
function SelectCameraModeGuideFrameMode1_OnClick()
	SelectCameraModeGuideFrameMode1:Checked()
	SelectCameraModeGuideFrameMode2:DisChecked()
end
function SelectCameraModeGuideFrameMode2_OnClick()
	SelectCameraModeGuideFrameMode2:Checked()
	SelectCameraModeGuideFrameMode1:DisChecked()
end
function SelectCameraModeGuideFrameOKBtn_OnClick()
	local systemSetting	= GameMgr:getUserSystemConfig();
	local chkBtn = getglobal( "GameSettingFrameLookUpDownChkBtn" );
	if SelectCameraModeGuideFrameMode1:IsChecked() then
		chkBtn:setCheckState( false );
		systemSetting:setGameOpt( GAME_OPT_CAMERA_3D, false );
	else
		chkBtn:setCheckState( true );
		systemSetting:setGameOpt( GAME_OPT_CAMERA_3D, true );
	end
	systemSetting:saveGameOpt();
	SelectCameraModeGuideFrame:Hide()
	NpcDialog:talkByResId( 304001 );
end
