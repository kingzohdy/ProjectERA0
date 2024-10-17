guide_enum = {      4004,4008,4,8,4016,16,64,128,
					1001,1002,1004,1008,1016,1032,32,1064,1128,3008,3064,
					4032,2002,2004,2008,2064,2128,
					3001,3002,3004,3016,3032,3128,
					4001,4002
				}

guide_isdo = {}

guide_desc = {
					-- 4004 第一次移动
					"你好，欢迎进入这个世界，#cEC6A2B鼠标左键#n点击地面就可移动，#cEC6A2B持续按住#n的话，可以自动移动哟。",
					-- 4008 第一次旋转视角
			        "想查看更多的景物吗？#cEC6A2B按住鼠标右键移动鼠标#n,就可以旋转视角了。",
			        -- 0004 第一次对话
			        "看见你面前的#cEC6A2BNpc#n了吗？它们会向你提供一些#cEC6A2B功能#n和#cEC6A2B任务#n，移动到它们面前，#cEC6A2B左键点击#n它们，就可以进行对话。",
					-- 0008 第一次任务领取
			        "接到任务了吧？可点开主界面的#cEC6A2B任务日志按钮#n查看任务详情。#cEC6A2B屏幕右边#n有该#cEC6A2B任务追踪#n。（友情提示： Npc头顶上，有#cEC6A2B黄色感叹号#n的话，就可以向它#cEC6A2B领取任务#n了。）",
					-- 4016 第一次提交任务
			        "完成任务了吗？一定要记得#cEC6A2B提交#n哟，这样你可以获得#cEC6A2B丰厚的奖励#n。（友情提示：#cEC6A2B白色问号#n代表从这里领取的任务尚未完成，还不能提交）",
					-- 0016 第一次升级
					"恭喜你升到2级了，#cEC6A2B经验槽#n满了之后自动升级，升级后#cEC6A2B人物属性#n会有提升。在#cEC6A2B10级#n之前系统都会帮你自动升级。",
					-- 0064 第一次下副本
				    "跟#cEC6A2B竞技场教官#n对话可进入副本，在副本里可获得#cEC6A2B更多的经验#n和#cEC6A2B更好的物品奖励#n，但要留心不要进入超出当前能力的副本。",
					-- 0128 第一次杀怪
			        "用#cEC6A2B左鼠点击#n怪物，即可#cEC6A2B自动进行攻击#n，如果只用#cEC6A2B右鼠#n点击的话，#cEC6A2B只能选中它。",
					-- 1001 第一次使用技能
			        "选取怪物，点击技能对应#cEC6A2B键位#n，就能#cEC6A2B使用#n了。灵活运用所学技能，会让战斗变得更加轻松。",
					-- 1002 第一次看到手雷
			        "使用#cEC6A2B手雷#n可以#cEC6A2B一次性对多个怪物进行轰炸#n，站到手雷#cEC6A2B所在的位置处#n，即可#cEC6A2B自动拾取#n了。",
					-- 1004 第一次使用手雷
			        "拾取后，#cEC6A2B手雷#n自动绑定到#cEC6A2B鼠标左键#n上，使用#cEC6A2B左鼠点怪#n，即可使用。另外，在#cEC6A2B屏幕右下方#n闪烁的#cEC6A2B临时技能栏#n，选定怪物后，即可点击#cEC6A2B技能图标#n使用它。",
					-- 1008 第一次看到炸药桶
			        "看到场景里的#cEC6A2B火药桶#n了吗，受到攻击后会#cEC6A2B爆炸#n。你可以利用它#cEC6A2B大范围杀伤怪物#n。",
					-- 1016 第一次接触武器
			        "#cEC6A2B左鼠#n打开背包后，可以看到#cEC6A2B武器图标#n，#cEC6A2B右鼠#n点击即可装备。记住，一件好武器，可给你提供更强大的#cEC6A2B攻击力#n。",
					-- 1032 第一次送信
			        "接到#cEC6A2B送信#n任务后，将#cEC6A2B委托物品#n送到任务要求的#cEC6A2B指定人#n处即可完成了。",
					-- 0032 第一次获得金钱
			        "通过#cEC6A2B任务#n以及#cEC6A2B杀怪#n，可以获得金钱奖励；金钱主要用于#cEC6A2B维修装备#n以及#cEC6A2B购买商店物品#n。",
					-- 1064 第一次采集
				    "看到#cEC6A2B任务追踪#n信息上，带#cEC6A2B下划线#n的采集物了吗？#cEC6A2B左键点击#n，即可#cEC6A2B自动移动#n到其分布区域。#cEC6A2B左键#n点它就可采集，在动作进行中#cEC6A2B不可打断#n，否则该次采集失败。",
					-- 1128 第一个礼品袋
					"打开背包，可以看到一个#cEC6A2B礼品袋#n，#cEC6A2B右键点击#n打开之后，可以获得装备奖励以及意外的惊喜。",
					-- 3008 第一次接触食物
					"打开#cEC6A2B背包#n，#cEC6A2B右鼠点击#n食物，即可食用。也可以将它拖动到#cEC6A2B动作条#n上，通过点击#cEC6A2B指定键位#n快速使用。在使用过程中，若你执行操作，即会#cEC6A2B中断#n该行为。",
					-- 3064 第一次特殊行为
					"接到#cEC6A2B开炮#n任务了？和对话任务类似，你只要跑到该类任务需求的#cEC6A2B交互对象上#n，与它#cEC6A2B对话#n，即可完成。",
					-- 4032 第一次获取技能书
					"打开背包，#cEC6A2B右鼠#n点击技能书，就能学会#cEC6A2B新技能#n了。要知道，学会新的战斗技巧，能有效提高你在战斗中的生存几率。",
					-- 2002 第一次学习技能
					"每隔#cEC6A2B5级#n，你可以和技能学习员对话，它会教你学习新技能和升级已学技能。技能等级提升后，可增加技能的相关能力。",
					-- 2004 第一次获得战利品
					"看到地上的#cEC6A2B包裹#n吗？#cEC6A2B左鼠#n点击可打开包裹面板，#cEC6A2B左鼠#n点击#cEC6A2B物品图标#n，即可将其放入背包。（友情提示，点击“#cEC6A2B全部拾取#n”按钮，可#cEC6A2B一次性#n拾取#cEC6A2B所有物品#n）",
					-- 2008 第一次收集
					"接到#cEC6A2B搜集#n任务了？这个任务物品是#cEC6A2B怪物掉落#n的，点击#cEC6A2B任务跟踪#n信息上，带#cEC6A2B下划线#n的物品名字，即可移动到#cEC6A2B怪物所在区域#n，#cEC6A2B杀死#n它后，可获得该物品。",
					-- 2064 第一次看到火焰刀
					"看到那把闪闪发光的#cEC6A2B火焰刀#n了吗？使用它可对怪物造成#cEC6A2B极大伤害#n。快跑到它所在的#cEC6A2B位置#n处，即可#cEC6A2B自动拾取#n了。",
					-- 2128 第一次拾取火焰刀
					"#cEC6A2B火焰刀技能#n已自动绑定到你的#cEC6A2B左键#n上，你现在可以通过#cEC6A2B左键点怪#n，即可使用了。同样可以点击临时技能栏上的#cEC6A2B技能图标#n使用它。",
					-- 3001 第一次获得药剂
					"打开#cEC6A2B背包#n，#cEC6A2B右鼠点击#n药剂，即可使用。也可以将它拖动到#cEC6A2B动作条#n上，通过点击#cEC6A2B指定键位#n快速使用。补充血量和魔法的药剂有不同颜色，不要搞混了。",
					-- 3002 第一次飞行
					"#cEC6A2B点击飞行员#n，启动飞行任务，飞龙将带你环岛飞行，感受天空无边无际的魅力。",
					-- 3004 第一次刷怪
                    "#cEC6A2B图腾柱#n出现了，与它#cEC6A2B点击对话#n可以#cEC6A2B召唤#n出怪物。",
					-- 3016 第一只精灵
					"你已经拥有#cEC6A2B精灵#n，点击主界面“#cEC6A2B宠物#n”按钮可查看详情。精灵可以提供#cEC6A2B战斗技能#n帮助你战斗，#cEC6A2B宠物面板#n上，将#cEC6A2B技能图标#n拖动到#cEC6A2B动作条#n进行使用。",
					-- 3032 第一次XP涨满
					"#cEC6A2B角色状态栏#n上，#cEC6A2B怒气槽#n的燃烧表示当前怒气#cEC6A2B已满#n，可以使用XP技能了。点击相应技能键位，就可以使用XP技能。",
					-- 3128 第一次限时任务
					"你已领取#cEC6A2B限时#n任务，一定要在#cEC6A2B规定时间#n内完成目标，否则该任务就失败了，必须放弃后重新领取。",
					-- 4001 第一次看到临时机械
					"#cEC6A2B机甲兽#n就是游戏中威力巨大的机械，使用它可对多个怪物同时造成#cEC6A2B极大伤害#n。点击之后就可以驾驭#cEC6A2B临时机械#n吧。",
					-- 4002 第一次使用临时机械
					"临时机械的两个#cEC6A2B技能#n了分别对应“#cEC6A2B左鼠#n”、和“#cEC6A2BCtrl+左鼠#n”键位，分别可以通过点击#cEC6A2B键位#n和#cEC6A2B临时技能栏#n使用。"
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

-- 新手在线奖励
local nCurAwardId = 0;
local nCurAwardTime = 0;
local nAwardLastTime = 0;

local BIRTH_MAP_ID = 2000;

-- 角色初始化配置
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

-- 第一次执行
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

-- 第一次任务触发执行
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

-- 显示新手指引特效显示
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
	-- 设置文字tips
	
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
	-- 设置文字tips
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
		--print( "GuideEffectShow--355--设置闪烁逻辑数据" );
		if bSetGray and frameUVATexture ~= nil then
			--print( "GuideEffectShow--355--设置闪烁效果" );
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
					local szText ="点击对目标使用";
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
	-- 背景花边渐隐
	local tex = getglobal(  "EffectFrame_Texture" );
	if EffectFrame:GetClientString() == "" and tex ~= nil then
		util.UIFrameToFadeIn( "EffectFrame_Texture",  0, 1,	300, 100 );
		util.UIFrameToFadeKeep( "EffectFrame_Texture",	1, 0 );
		util.UIFrameToFadeOut( "EffectFrame_Texture",	1, 0,	300, 100 );
	end
	
	-- 隐藏指引提示TIPS窗口
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

EFFECT_TIPS_FIRST_AUTO_FIND		= 0;	-- 第一次自动寻路
EFFECT_TIPS_FIRST_GET_TASK		= 1;	-- 第一次接取任务
EFFECT_TIPS_FIRST_FINISH_TASK	= 2;	-- 第一次完成任务

local t_showReason = { [EFFECT_TIPS_FIRST_AUTO_FIND] = {desc = "点击既可以自动寻路"} };
function TipsEffectFrame_OnShow()
	-- 隐藏指引提示TIPS窗口
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

-- 关闭新手指引特效显示
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

-- 关闭新手tips指引特效显示
function GuideTipsEffectHide( szFrameName )
	local EffectFrame= getglobal( "TipsEffectFrame" );
	if (EffectFrame:IsShown()) then
		if ( szFrameName ~= nil and szFrameName == EffectFrame:GetClientString() )
			or szFrameName == nil then
			EffectFrame:Hide();
		end
	end
end

-- 第一次登录
function PlayerFirstLogin()

end

-- 第一次移动
function PlayerFirstMove()
end

-- 第一次旋转视角
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

-- 第一次对话
function PlayerFirstDialog()
	if GameWizard:isDo(GAMEWIZARD_FIRST_TALK) == false then
		PlayIsDo(GAMEWIZARD_FIRST_TALK,1);
	end
end

-- 第一次使用技能
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

-- 第一次使用手雷
function PlayerFirstUserGrenade()
	if  GameWizard:isDo(GAMEWIZARD_FIRST_USEDGRENADE) == false then
		--PlayIsDo(GAMEWIZARD_FIRST_USEDGRENADE,2);
	end
end

-- 第一次拾取火焰刀
function PlayerFirstGetSword()
    if  GameWizard:isDo(GAMEWIZARD_FIRST_GETSWORD) == false then
        --PlayIsDo(GAMEWIZARD_FIRST_GETSWORD,4);
    end
end

-- 第一次XP涨满
function PlayerFirstPowerfull()
    if  GameWizard:isDo(GAMEWIZARD_FIRST_POWERFULL) == false then
        PlayIsDo(GAMEWIZARD_FIRST_POWERFULL,5);
    end
end

-- 第一次使用临时机械
function PlayerFirstUseMachine()
    if  GameWizard:isDo(GAMEWIZARD_FIRST_USEDMACHINE) == false then
        PlayIsDo(GAMEWIZARD_FIRST_USEDMACHINE,5);
    end
end

-- 第一次使用火焰刀
function FirstUseSword()
	-- 若不是第一次使用临时道具，则什么也不做
	local systemSetting	= GameMgr:getUserSystemConfig();
	if not GameWizard:isDo( GAMEWIZARD_FIRST_USE_SWORD ) and Quest:isQuestRun( 1054 ) then
		local text = "点击鼠标"..(systemSetting:getGameOpt(GAME_OPT_SWAP_MOUSE) and "左键" or "右键")
		local code = GetKeyMap("主快捷栏1")
		if code == 0 then
			code = nil
		end
		if code then
			text = string.format("%s或技能快捷键 %s ", text, KeyToString(code))
		end
		text = text.."可攻击目标"
		ShowTempShorCutFrameXinShouBtn( { text = text } );
		--GameWizard:doStep( GAMEWIZARD_FIRST_USE_SWORD );
	elseif not GameWizard:isDo( GAMEWIZARD_SECOND_USE_SWORD ) and Quest:isQuestRun(1062 ) then
		local text = "点击鼠标"..(systemSetting:getGameOpt(GAME_OPT_SWAP_MOUSE) and "左键" or "右键")
		local code = GetKeyMap("主快捷栏1")
		if code == 0 then
			code = nil
		end
		if code then
			text = string.format("%s或技能快捷键 %s ", text, KeyToString(code))
		end
		text = text.."可攻击目标"
		ShowTempShorCutFrameXinShouBtn( { text = text } );
		--GameWizard:doStep( GAMEWIZARD_SECOND_USE_SWORD );
	end
end


-- 第一次使用临时机械
function FirstUseJiJia()
	-- 若不是第一次使用临时机械，则什么也不做
	local systemSetting	= GameMgr:getUserSystemConfig();
	if not GameWizard:isDo( GAMEWIZARD_FIRST_USE_TMPMACHINE ) then
		local text = "点击鼠标"..(systemSetting:getGameOpt(GAME_OPT_SWAP_MOUSE) and "左键" or "右键")
		local t = {}
		local code = GetKeyMap("主快捷栏1")
		if code == 0 then
			code = nil
		end
		if code then
			table.insert(t, KeyToString(code))
		end
		code = GetKeyMap("主快捷栏2")
		if code == 0 then
			code = nil
		end
		if code then
			table.insert(t, KeyToString(code))
		end
		if #t > 0 then
			text = string.format("%s或技能快捷键 %s ", text, table.concat(t, ", "))
		end
		text = text.."可攻击目标"
		ShowTempShorCutFrameXinShouBtn( { text = text } );
		--GameWizard:doStep( GAMEWIZARD_FIRST_USE_TMPMACHINE );
	end
end

-- 第一次使用炮台
function FirstUsePaoTai()
	-- 若不是第一次使用炮台，则什么也不做
	local systemSetting	= GameMgr:getUserSystemConfig();
	if not GameWizard:isDo( GAMEWIZARD_FIRST_USE_TPAOTAI ) then
		local text = "点击鼠标"..(systemSetting:getGameOpt(GAME_OPT_SWAP_MOUSE) and "左键" or "右键")
		local t = {}
		local code = GetKeyMap("主快捷栏1")
		if code == 0 then
			code = nil
		end
		if code then
			table.insert(t, KeyToString(code))
		end
		code = GetKeyMap("主快捷栏2")
		if code == 0 then
			code = nil
		end
		if code then
			table.insert(t, KeyToString(code))
		end
		if #t > 0 then
			text = string.format("%s或技能快捷键 %s ", text, table.concat(t, ", "))
		end
		text = text.."可攻击目标"
		ShowTempShorCutFrameXinShouBtn( { text = text } );
		--GameWizard:doStep( GAMEWIZARD_FIRST_USE_TPAOTAI );
	end
end

-- 第一次使用手雷
function FirstUseShouLei()
	-- 若不是第一次使用手雷，则什么也不做
	local systemSetting	= GameMgr:getUserSystemConfig();
	if not GameWizard:isDo( GAMEWIZARD_FIRST_USE_TSHOULEI ) then
		local text = "点击鼠标"..(systemSetting:getGameOpt(GAME_OPT_SWAP_MOUSE) and "左键" or "右键")
		local code = GetKeyMap("主快捷栏1")
		if code == 0 then
			code = nil
		end
		if code then
			text = string.format("%s或技能快捷键 %s ", text, KeyToString(code))
		end
		text = text.."可攻击目标"
		ShowTempShorCutFrameXinShouBtn( { text = text } );
		--GameWizard:doStep( GAMEWIZARD_FIRST_USE_TSHOULEI );
	end
end

-- 第一次点击完成任务按钮
function FirstClickFinishTask( questId )
	-- 若不是第一次点击完成任务按钮，则什么也不做
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
		szText = "点击可完成任务";
	else
		szText = "点击可完成任务并领取奖励";
	end
	
	ShowNpcDialogEffectFrame( { text = szText, relFrame = "TaskAwardFrame", button = "TaskAwardFrame_GetBtn" } );

	if questId == 1051 then
		GameWizard:doStep( GAMEWIZARD_FIRST_FINISHQUEST_FROMUI );
	end
end

-- 第一次点击接受任务按钮
function FirstClickAcceptTask( questId )
	-- 若不是第一次点击接受任务按钮，则什么也不做
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
	
	local szText = "点击可领取任务";
		
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
	-- 若为嘟嘟鸟	and Quest:isQuestRun( 1052 )
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
-- 任务获得触发
function SpringByGetQuest( questId, nRecycleID )	
	-- 自动组队检查
	CheckCloseSuccessCreatTaskAutoTeamFrame( questId );
	local t_NpcAddTalk =
	{
		-- 安东尼 -- 危机四伏
		[1050] = { resId = 304001, content = "点击领取任务",  popBackRich = "ChatPop", showOnce = true, headOffsetX = 12, 
					headOffsetY = -100, arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
					wizardOp = GAMEWIZARD_FIRST_GETQUEST_FROMUI, questId = 1050,  npcState = CLIENT_NORMAL_TASK_STAT_CAN_ACCEPT
				},

		-- 罗纳度 -- 新兵签到
		[1051] = { resId = 306011, content = "点击领取任务",  popBackRich = "ChatPop", showOnce = true, headOffsetX = 12, 
					headOffsetY = -100, arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
					wizardOp = GAMEWIZARD_FIRST_GETQUEST_FROMUI, questId = 1051,  npcState = CLIENT_NORMAL_TASK_STAT_CAN_ACCEPT
				},
		
		-- 艾西 -- 紧急任务
		[1053] = { resId = 303003, content = "点击可购买物品",  popBackRich = "ChatPop", showOnce = true, headOffsetX = 12, 
					headOffsetY = -50, arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
					wizardOp = GAMEWIZARD_FIRST_OPENSHOP, questId = 1053, npcState = CLIENT_NORMAL_TASK_STAT_CAN_ACCEPT
				},	
		--[[
		-- 姬玛 -- 无奈的战斗
		[1062] = { resId = 302016, content = "点击可以进入飞羽峰",  popBackRich = "ChatPop", showOnce = true, headOffsetX = 12, 
					headOffsetY = -50, arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
					wizardOp = GAMEWIZARD_FIRST_TRAN_TO_SKY_COPY, questId = 1062, npcState = CLIENT_NORMAL_TASK_STAT_CAN_ACCEPT
				},
		--]]
		--[[
		-- 西曼 -- 壮志凌云
		[1060] = { resId = 316000, content = "点击可飞行",  popBackRich = "ChatPop", showOnce = true, headOffsetX = 12, 
					headOffsetY = -50, arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
					wizardOp = GAMEWIZARD_FIRST_FLY_QUEST, questId = 1060, npcState = CLIENT_NORMAL_TASK_STAT_CAN_ACCEPT
				},
		--]]
	};
	
	local addTalkData = t_NpcAddTalk[questId];
	if addTalkData ~= nil then
		SetNpcQuestPop( addTalkData );
	end
	
	-- 在获得	救援部队	的时候	
	if questId == 1049 and not GameWizard:isDo(GAMEWIZARD_FIRST_GETMISSION ) and not GameMgr:isInPlayCamerAnim() then
		EndFirstEnterGameAnim();
	end

	-- 获取该任务的时候更新该任务
	-- 第一次任务领取
	if questId == 1050 then
		--LUALOGINFO("questId == 1000");
		if not GameWizard:isDo(GAMEWIZARD_FIRST_GETMISSION ) then
			GameWizard:hideNpcTalkByResId( 304001 );
			SetTaskLogFrameGuide( questId, nRecycleID );
			GameWizard:doStep( GAMEWIZARD_FIRST_GETMISSION );
		end

		-- 新手指引tips 第一次看到任务超链接
		if  GameWizard:isDo(GAMEWIZARD_FIRST_OPENTASK) == false and Quest:getNumCanAcceptQuest() > 0 then
			--TaskLogFrame_CanAcceptQuestChkGuide:Show();
			--TaskLogFrame_CanAcceptQuestChkGuideRichFont:SetText( "点击打开\n可接任务列表",	86, 79, 54);
			GameWizard:doStep(GAMEWIZARD_FIRST_OPENTASK);
		else
			if TaskLogFrame_CanAcceptQuestChkGuide:IsShown() then
				TaskLogFrame_CanAcceptQuestChkGuide:Hide();
			end
		end
	end
	
	-- 精灵炼化任务
	if questId == 3112 and not GameWizard:isDo(GAMEWIZARD_FIRST_BLINK_LIAN_HUA_LABEL) then
		if TrumpFrame:IsShown() then
			ShowJinglIngMainFrameXinShou();
		else
			TrumpFrame:Show();
		end
	end
	
	-- 新手指引tips 第一次看到任务超链接
	if  not GameWizard:isDo(GAMEWIZARD_FIRST_SAWTASKLINK) then
		local lastId = GetLastShowTipsTask();
		if lastId ~= questId then
			SetNeedShowTips( true, questId );
		end
	end
	
	-- 提交获取骑乘的任务，第二场比赛
	if questId == 3134 and not GameWizard:isDo(GAMEWIZARD_FIRST_BLINK_STRENGTHEN_BTN) then
		RideFrame:Show();
	end

	-- 第一次下副本
	if questId == 1004 and GameWizard:isDo(GAMEWIZARD_FIRST_GOCOPY) then
		SpringPlayIsDo(GAMEWIZARD_FIRST_GOCOPY,2);
	end
	
	-- [questId] = { [resId] = {}, [resId] = {}, }
	local mainplayer		= ActorMgr:getMainPlayer();
	local nSkillID			= t_firstGetSkill[GetInitRace( mainplayer:getRace() )]["skillID"];
	local SkillDef			= SkillMgr:getSpellStaticAtt( nSkillID,1 );
	local szMuZhuangPaoPao	= "请对我使用【"..SkillDef.Name .."】技能";
	local systemSetting		= GameMgr:getUserSystemConfig();
	local t_taskAddActorPop = 
	{
	-- 获得第一次采集任务
	-- taskId  
	[1055] =	{ [293003] =	{	resId = 293003, 
									content = "鼠标左键点击我可采集",  
									popBackRich = "ChatPop", showOnce = true,  headOffsetX = 10, headOffsetY = 0,
									arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
									wizardOp = GAMEWIZARD_FIRST_COLLECT,
								},
				},
	[1059] =	{ [293002] =	{	resId = 293002, 
									content = "鼠标左键点击我可采集",  
									popBackRich = "ChatPop", showOnce = true,  headOffsetX = 10, headOffsetY = 0,
									arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
									wizardOp = GAMEWIZARD_FIRST_COLLECT,
								},
				},
	[1070] =	{ [293000] =	{	resId = 293000, 
									content = "鼠标左键点击我可采集",  
									popBackRich = "ChatPop", showOnce = true,  headOffsetX = 10, headOffsetY = 0,
									arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
									wizardOp = GAMEWIZARD_FIRST_COLLECT,
								},
				},
	[1078] =	{ [293001] =	{	resId = 293001, 
									content = "鼠标左键点击我可采集",  
									popBackRich = "ChatPop", showOnce = true,  headOffsetX = 10, headOffsetY = 0,
									arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
									wizardOp = GAMEWIZARD_FIRST_COLLECT,
								},
				},

	-- 第一救治飞龙
	[1064] = {	[351000] = {	resId = 351000, 
								-- 我很难受，快来救我，
								content = "请右键点击我，对我使用药品",  
								popBackRich = "ChatPop", showOnce = true,  headOffsetX = 0, headOffsetY = 0,
								arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
								wizardOp = GAMEWIZARD_FIRST_CURE_NPC, 
							},
			},
	
	-- 第一战斗
	[1052] = {	[241000] = {	resId = 241000, 
								content = ( systemSetting:getGameOpt( GAME_OPT_SWAP_MOUSE ) and "左键" or "右键" ) .."点击怪物攻击",   
								popBackRich = "ChatPop", showOnce = true,  headOffsetX = 0, headOffsetY = 0,
								arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
								wizardOp = GAMEWIZARD_FIRST_FIGHT, 
							},
			},
	[1058] = {	[241002] = {	resId = 241002, 
								content = ( systemSetting:getGameOpt( GAME_OPT_SWAP_MOUSE ) and "左键" or "右键" ) .."点击怪物攻击",   
								popBackRich = "ChatPop", showOnce = true,  headOffsetX = 0, headOffsetY = 0,
								arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
								wizardOp = GAMEWIZARD_FIRST_FIGHT, 
							},
			},
	[1065] = {	[231021] = {	resId = 231021, 
								content = ( systemSetting:getGameOpt( GAME_OPT_SWAP_MOUSE ) and "左键" or "右键" ) .."点击怪物攻击",   
								popBackRich = "ChatPop", showOnce = true,  headOffsetX = 0, headOffsetY = 0,
								arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
								wizardOp = GAMEWIZARD_FIRST_FIGHT, 
							},
			},
	[1069] = {	[235000] = {	resId = 235000, 
								content = ( systemSetting:getGameOpt( GAME_OPT_SWAP_MOUSE ) and "左键" or "右键" ) .."点击怪物攻击",   
								popBackRich = "ChatPop", showOnce = true,  headOffsetX = 0, headOffsetY = 0,
								arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
								wizardOp = GAMEWIZARD_FIRST_FIGHT, 
							},
			},
	[1073] = {	[250001] = {	resId = 250001, 
								content = ( systemSetting:getGameOpt( GAME_OPT_SWAP_MOUSE ) and "左键" or "右键" ) .."点击怪物攻击",   
								popBackRich = "ChatPop", showOnce = true,  headOffsetX = 0, headOffsetY = 0,
								arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
								wizardOp = GAMEWIZARD_FIRST_FIGHT, 
							},
			},
	[1080] = {	[200008] = {	resId = 200008, 
								content = ( systemSetting:getGameOpt( GAME_OPT_SWAP_MOUSE ) and "左键" or "右键" ) .."点击怪物攻击",   
								popBackRich = "ChatPop", showOnce = true,  headOffsetX = 0, headOffsetY = 0,
								arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
								wizardOp = GAMEWIZARD_FIRST_FIGHT, 
							},
			},
	
	-- 实力的证明
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

	-- 第一次飞行
	if questId == 4001 then
		if not GameWizard:isDo(GAMEWIZARD_FIRST_FLY) then
			SpringPlayIsDo(GAMEWIZARD_FIRST_FLY,4);
		end
	end

	-- 第一次特殊行为
	if questId == 4002 then
		if not GameWizard:isDo(GAMEWIZARD_FIRST_SPECIALMISSION) then
			SpringPlayIsDo(GAMEWIZARD_FIRST_SPECIALMISSION,4);
		end
	end

--[[
	-- 第一次接触武器
	if questId == 1056 and not GameWizard:isDo(GAMEWIZARD_FIRST_GETWEAPON) then
		--GameWizard:doStep(GAMEWIZARD_FIRST_GETWEAPON);
		ShowMidTips("请点击背包按钮");
		GuideEffectShow( "", "点击打开背包界面", "ContainerButton", "你获得一件装备，点击打开背包查收\n#P快捷键（B）", "uires\\ZhuJieMian\\1.dds", 352, 609, 60, 60, 50, 60, -9, 5, 7, -58, true, FIRST_GET_WEAPON );
	end
--]]
	
	local t_taskAddNpcPop = 
	{

	-- 第一次传送到时间城莉莎
	[1077] = {	resId = 303021, 
					content = "    点击可以传送到时间城", 
					popBackRich = "ChatPop", showOnce = true, headOffsetX = 12, headOffsetY = -50,
					arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0,
					wizardOp = { GAMEWIZARD_FIRST_TALK_ZHUCHENG_TRAN, } ,
				},
	};
	local t_chgNpcShowEvent = t_taskAddNpcPop[questId];
	-- 第一次传送到时间城
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

-- 任务完成触发
function SpringByFinishQuest( questId )
	-- [questId] = { npcResId = ? }
	-- 删除的泡泡内容
	local t_finishQuestNpcDelMap = 
	{
	-- 危机四伏	-- 安东尼
	[1050] = { npcResId = 304001 },
	-- 新兵签到	-- 罗纳度
	--[1051] = { npcResId = 306011 },
	-- 被污染的核心	-- 飞龙炽焰
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
	-- 增加的泡泡内容
	local t_NpcAddTalk =
	{
		-- 罗纳度 -- 危机四伏
		[1050] = { resId = 306011, content = "点击完成任务",  popBackRich = "ChatPop", showOnce = true, headOffsetX = 12, 
					headOffsetY = -100, arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
					wizardOp = GAMEWIZARD_FIRST_FINISHQUEST_FROMUI, questId = 1050, npcState = CLIENT_TASK_STAT_FINI,
				},
		
		-- 罗纳度 -- 新兵签到
		[1051] = { resId = 306011, content = "点击完成任务",  popBackRich = "ChatPop", showOnce = true, headOffsetX = 12, 
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
	-- 第一次战斗
	-- taskId  
	[1052] = { 241000, },
	[1058] = { 241002, },
	[1065] = { 231021, },
	[1069] = { 235000, },
	[1073] = { 250001, },
	[1080] = { 200008, },

	-- 获得第一次采集任务
	-- taskId  
	[1055] = { 293003, },
	[1059] = { 293002, },
	[1070] = { 293000, },
	[1078] = { 293001, },

	-- 实力的证明
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
		-- 同时清除
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

-- 任务提交触发
function SpringByQuestCommit( questId )
	SetFadeTaskNameShow( questId );
	local t_commitQuestNpcMap = 
	{
	-- 救援部队	-- 安东尼
	[1049] = { npcResId = 304001 },
	-- 危机四伏	-- 罗纳度
	[1050] = { npcResId = 306011 },
	-- 新兵签到	-- 罗纳度
	[1051] = { npcResId = 306011 },
	-- 紧急任务	-- 艾西
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

	-- 第一次提交任务
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

	-- 完成 实力的证明  的任务
	if questId == 1061 then
		GameWizard:delGroupActorsByResId( 350040 );
		GameWizard:doStep(GAMEWIZARD_FIRST_USE_NEW_SKILL);
	end

	-- 第一次提交任务
	if not GameWizard:isDo(GAMEWIZARD_FIRST_SAWTASKLINK) and TaskTraceFrameTaskTraceRich1Guide:IsShown() then
		TaskTraceFrameTaskTraceRich1Guide:Hide();
		if questId == 1081 then
			GameWizard:doStep(GAMEWIZARD_FIRST_SAWTASKLINK);
		end
	end

	-- 提交获取骑乘的任务，第一场比赛
	if questId == 3132 and not GameWizard:isDo(GAMEWIZARD_FIRST_BLINK_SHORTCUT_RIDE_BTN) then
		local containerFrame = getglobal( "PlayerContainerFrame" );
		if containerFrame:IsShown() then
			ShowContainerGuide();
		else
			containerFrame:Show();
		end
	end

	-- 提交获取精灵的任务，第一次接触精灵
	if questId == 3111 and not GameWizard:isDo(GAMEWIZARD_FIRST_GETXIAN_ITEM) then
		ShowMidTips("获得了一只精灵");
		PlayerContainerFrame:Show();
	end

	-- 第一次接触食物
	if questId == 1006 then
		if GameWizard:isDo(GAMEWIZARD_FIRST_GETFOOD) == false then
            SpringPlayIsDo(GAMEWIZARD_FIRST_GETFOOD,3);
		end
	end
	-- 第一次获得技能书
	if questId == 1007 then
		if GameWizard:isDo(GAMEWIZARD_FIRST_GETSKILLBOOK) == false then
			SpringPlayIsDo(GAMEWIZARD_FIRST_GETSKILLBOOK,4);
		end
	end

	-- 第一次礼品袋
	if questId == 4000 then
		if  GameWizard:isDo(GAMEWIZARD_FIRST_PRESENT) == false then
			PlayIsDo(GAMEWIZARD_FIRST_PRESENT,2)
		end
	end

	-- 第一只精灵
	if questId == 1003 then

	end
end

-- 任务放弃
function SpringByQuestDrop( questId )
	local t_dropQuestNpcMap = 
	{
	-- 危机四伏	-- 罗纳度
	[1050]	= { npcResId = 306011 },
	-- 新兵签到	-- 罗纳度
	[1051]	= { npcResId = 306011 },
	-- 紧急任务	-- 艾西
	[1053]	= { npcResId = 303003 },
	-- 壮志凌云	-- 西曼
	--[1060]	= { npcResId = 316000 },
	-- 无奈的战斗	-- 姬玛
	--[1062]	= { npcResId = 302016 },
	-- 被污染的核心	-- 飞龙炽焰
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
	-- 第一次战斗
	-- taskId  
	[1052] = { 241000, },
	[1058] = { 241002, },
	[1065] = { 231021, },
	[1069] = { 235000, },
	[1073] = { 250001, },
	[1080] = { 200008, },
	-- 获得第一次采集任务
	-- taskId  
	[1055] = { 293003, },
	[1059] = { 293002, },
	[1070] = { 293000, },
	[1078] = { 293001, },
	-- 实力的证明
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
			-- 莉莎
			--{ 303021, GAMEWIZARD_FIRST_TALK_ZHUCHENG_TRAN },
			-- 副本传送门
			{ 333003, GAMEWIZARD_FIRST_TALK_COPY_TRAN_DOOR },
			-- 飞龙
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
					local szText ="左键点击使用";
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
	-- 姬玛  进入天空岛副本
	[302016] =	{	preCondition = GAMEWIZARD_FIRST_TRAN_TO_SKY_COPY, 
					postCondition = GAMEWIZARD_FIRST_TALK_TO_SKY_COPY_NPC 
				},
				
	
	-- 西曼  第一次飞行任务
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
	-- 需要任务限制的
	local t_talkNpcWithTask = 
	{ 
	-- 飞龙  
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
			local szText ="左键点击使用";
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

-- 升级触发
function SpringByLevelUp( Lv )
	--[[
	-- 第一次升级
	if Lv == 5 then
		if GameWizard:isDo(GAMEWIZARD_FIRST_LEVELUP) == false then
			GameWizard:doStep(GAMEWIZARD_FIRST_LEVELUP);
			ShowMidTips("请点击技能按钮");
			
			GuideEffectShow( "", "点击打开技能界面", "SkillButton", "点击打开技能界面\n#P快捷键（K）", "uires\\ZhuJieMian\\1.dds", 352, 609, 60, 60, 50, 60, -9, 5, 7, -58, true );
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
--公会情报攻防
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
--公会活动-守护公会枢纽
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
	--这里添加判断是否进入公会冶炼岛
	if bIsClanPwrldBegin and GameClanManager:IsInClanPworldIsland() then
		ClanPworldIslandShowInfoFrame:Show();
	else
		ClanPworldIslandShowInfoFrame:Hide();		
	end
	--判断是否在公会守护枢纽副本
	if GameClanManager:IsInClanMachinePworld() or IsInTestMapShowClanPworldTime() then
		ClanMachinePworldTimeShowFrame:Show();
	else
		ClanMachinePworldTimeShowFrame:Hide();
		setIterationStarted(false);
	end
	--这里添加判断是否进入公会据点战副本
	if GameClan_BaseWar:IsInBaseWarPworld() then
		ClanStrongWarResFrame:Show();
	else
		--清除下方据点漂移图标
		local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
		local nIdx = t_centerXinShouBtnControl:getIndexByType( CENTER_CLAN_STRONG_WAR );
		if nIdx > 0 then
			t_centerXinShouBtnControl:eraseByIndex( nIdx );
			t_centerXinShouBtnControl:updateCenterBtns();
		end
		ClanStrongWarResFrame:Hide();
	end
	--判断是否在攻城战地图中
	if MapShower:getRealMapId() ~= WorldCityMapID then
		--清除下方时间城争霸战漂移图标
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

-- 对象进入视野触发
function SpringByObjectEnter( objType, objId, actorId, dis )
	-- [npcResId] = {}
	local t_NpcAddTalk =
	{	
		-- 罗纳度
		[306011] = {	resId = 306011, 
						content ={ [CLIENT_TASK_STAT_FINI] = "点击完成任务", [CLIENT_NORMAL_TASK_STAT_CAN_ACCEPT] = "点击领取任务", },  
						popBackRich = "ChatPop", showOnce = true, headOffsetX = 12, headOffsetY = -100,
						arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
						wizardOp = { GAMEWIZARD_FIRST_FINISHQUEST_FROMUI, GAMEWIZARD_FIRST_GETQUEST_FROMUI },
						questId = 1050, 
				},
	
		-- 艾西
		[303003] = {	resId = 303003, 
						content = { [CLIENT_TASK_STAT_FINI] = "点击可购买物品", [CLIENT_NORMAL_TASK_STAT_CAN_ACCEPT] = "点击可购买物品", },  
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
		-- 副本传送门	GAMEWIZARD_FIRST_SEE_COPY_TRAN_DOOR
		[333003] = {	resId = 333003, 
						content = "点击这里可以快速传送出副本", 
						popBackRich = "ChatPop", showOnce = true, headOffsetX = 12, headOffsetY = 0,
						arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0,
						wizardOp = { GAMEWIZARD_FIRST_SEE_COPY_TRAN_DOOR }, preConditionCheck = function () return true; end
					},

		-- 技能学习员	GAMEWIZARD_FIRST_SEE_STUDY_SKILL_NPC
		[306000] = {	resId = 306000, 
						content = "点击这里学习技能", 
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
		-- 莉莎
		[303021] = {	resId = 303021, 
						content = "    点击可以传送到时间城", 
						popBackRich = "ChatPop", showOnce = true, headOffsetX = 12, headOffsetY = -50,
						arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0,
						wizardOp = { GAMEWIZARD_FIRST_TALK_ZHUCHENG_TRAN },
						preConditionCheck = function ()
												local mainplayer = ActorMgr:getMainPlayer();
												return mainplayer:getLv() >= 10;
											end
					},
	};
	-- 传送到时间城的NPC
	local addNpcWithQuestTalkData = t_NpcWithQuestAddTalk[objId];
	if objId == 303021 and not GameWizard:isDo(GAMEWIZARD_FIRST_TALK_ZHUCHENG_TRAN ) and 
			addNpcWithQuestTalkData.preConditionCheck() and Quest:isQuestRun( 1077 ) then
			SetNpcWithoutQuestPopByState( t_NpcWithQuestAddTalk[objId] );
	end
	
	-- 第一次见到安东尼
	-- 在获得	救援部队	的时候	
	if objId == 304001 and not GameWizard:isDo(GAMEWIZARD_FIRST_GETMISSION ) and not GameMgr:isInPlayCamerAnim() then
		EndFirstEnterGameAnim();
	end

	-- 第一次见到时间城传送大使
	if objId == 333003 and not GameWizard:isDo(GAMEWIZARD_FIRST_SEE_COPY_TRAN_DOOR) then
		GameWizard:doStep(GAMEWIZARD_FIRST_SEE_COPY_TRAN_DOOR);
	end

	local systemSetting	= GameMgr:getUserSystemConfig();
	
	local pickKey			= GetKeyMap("拾取附近物品")
	if pickKey == 0 then
		pickKey = nil
	end
	if pickKey then
		pickKey = KeyToString(pickKey)
	end
	local mainplayer		= ActorMgr:getMainPlayer();
	local nSkillID			= t_firstGetSkill[GetInitRace( mainplayer:getRace() )]["skillID"];
	local SkillDef			= SkillMgr:getSpellStaticAtt( nSkillID,1 );
	local szMuZhuangPaoPao	= "请对我使用【"..SkillDef.Name .."】技能";
	local t_monsterAddTalk	= 
	{
		-- 机甲兽
		--[1460001] = {	resId = 1460001, 
		--				content = "左键点击乘骑，可以获得威力巨大的技能",  
		--				popBackRich = "ChatPop", showOnce = true,  headOffsetX = 10, headOffsetY = -70,
		--				arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
		--				wizardOp = GAMEWIZARD_FIRST_SAW_JIJIASHOU, questId = 0, 
		--			},

		-- 炮台
		--[1460002] = {	resId = 1460002, 
		--				content = "左键点击炮台，可以获得威力巨大的技能",  
		--				popBackRich = "ChatPop", showOnce = true,  headOffsetX = 10, headOffsetY = -20,
		--				arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
		--				wizardOp = GAMEWIZARD_FIRST_SAWPAOTAI, questId = 0, 
		--			},
		
		-- 手雷
		--[1400002] = {	resId = 1400002, 
		--				content = "左键点击拾取手雷，可以获得威力巨大的技能",  
		--				popBackRich = "ChatPop", showOnce = true,  headOffsetX = 10, headOffsetY = -20,
		--				arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
		--				wizardOp = GAMEWIZARD_FIRST_SAW_SHOULEI, questId = 0, 
		--			},

		-- 嘟嘟鸟
		[241000] = {	resId = 241000, 
						content = ( systemSetting:getGameOpt( GAME_OPT_SWAP_MOUSE ) and "左键" or "右键" ) .."点击怪物攻击",  
						popBackRich = "ChatPop", showOnce = true,  headOffsetX = 10, headOffsetY = -20,
						arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
						wizardOp = GAMEWIZARD_FIRST_FIGHT,
					},
		[241002] = {	resId = 241002, 
						content = ( systemSetting:getGameOpt( GAME_OPT_SWAP_MOUSE ) and "左键" or "右键" ) .."点击怪物攻击",   
						popBackRich = "ChatPop", showOnce = true,  headOffsetX = 10, headOffsetY = -20,
						arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
						wizardOp = GAMEWIZARD_FIRST_FIGHT, 
					},
		[231021] = {	resId = 231021, 
						content = ( systemSetting:getGameOpt( GAME_OPT_SWAP_MOUSE ) and "左键" or "右键" ) .."点击怪物攻击",   
						popBackRich = "ChatPop", showOnce = true,  headOffsetX = 10, headOffsetY = -20,
						arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
						wizardOp = GAMEWIZARD_FIRST_FIGHT, 
					},
		[235000] = {	resId = 235000, 
						content = ( systemSetting:getGameOpt( GAME_OPT_SWAP_MOUSE ) and "左键" or "右键" ) .."点击怪物攻击",   
						popBackRich = "ChatPop", showOnce = true,  headOffsetX = 10, headOffsetY = -20,
						arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
						wizardOp = GAMEWIZARD_FIRST_FIGHT, 
					},
		[250001] = {	resId = 250001, 
						content = ( systemSetting:getGameOpt( GAME_OPT_SWAP_MOUSE ) and "左键" or "右键" ) .."点击怪物攻击",   
						popBackRich = "ChatPop", showOnce = true,  headOffsetX = 10, headOffsetY = -20,
						arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
						wizardOp = GAMEWIZARD_FIRST_FIGHT, 
					},
		[200008] = {	resId = 200008, 
						content = ( systemSetting:getGameOpt( GAME_OPT_SWAP_MOUSE ) and "左键" or "右键" ) .."点击怪物攻击",   
						popBackRich = "ChatPop", showOnce = true,  headOffsetX = 10, headOffsetY = -20,
						arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
						wizardOp = GAMEWIZARD_FIRST_FIGHT, 
					},
		
		-- 地上可拾取包裹
		--[9900000] = {	resId = 9900000, 
		--				content = pickKey and string.format("左键点击拾取，或使用快捷键(%s)快速拾取", pickKey) or "左键点击拾取",
		--				popBackRich = "ChatPop", showOnce = true,  headOffsetX = 10, headOffsetY = -60,
		--				arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
		--				wizardOp = GAMEWIZARD_FIRST_GETBOOTY,
		--			},

		-- 第一次采集任务的晶石
		[293003] = {	resId = 293003, 
						content = "鼠标左键点击我可采集",  
						popBackRich = "ChatPop", showOnce = true,  headOffsetX = 10, headOffsetY = 0,
						arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
						wizardOp = GAMEWIZARD_FIRST_COLLECT,
					},
		[293002] =	{	resId = 293002, 
						content = "鼠标左键点击我可采集",  
						popBackRich = "ChatPop", showOnce = true,  headOffsetX = 10, headOffsetY = 0,
						arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
						wizardOp = GAMEWIZARD_FIRST_COLLECT,
					},
		[293000] =	{	resId = 293000, 
						content = "鼠标左键点击我可采集",  
						popBackRich = "ChatPop", showOnce = true,  headOffsetX = 10, headOffsetY = 0,
						arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
						wizardOp = GAMEWIZARD_FIRST_COLLECT,
					},
		[293001] =	{	resId = 293001, 
						content = "鼠标左键点击我可采集",  
						popBackRich = "ChatPop", showOnce = true,  headOffsetX = 10, headOffsetY = 0,
						arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
						wizardOp = GAMEWIZARD_FIRST_COLLECT,
					},

		-- 第一救治飞龙
		[351000] = {	resId = 351000, 
						content = "我很难受，快来救我，请左键点击我，对我使用药品",  
						popBackRich = "ChatPop", showOnce = true,  headOffsetX = 10, headOffsetY = 0,
						arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
						wizardOp = GAMEWIZARD_FIRST_CURE_NPC,
					},
		
		-- 木桩
		[350040] = {	resId = 350040, 
						content = szMuZhuangPaoPao,  
						popBackRich = "ChatPop", showOnce = true,  headOffsetX = 10, headOffsetY = 50/GetScreenScaleY(),
						arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
						wizardOp = GAMEWIZARD_FIRST_USE_NEW_SKILL,
					},
	};


	--在播放开场动画的时候，不进行处理
	if GameMgr:isInPlayCamerAnim() then
		return;
	end

	-- 在接到第一次战斗的相关人物后第一次见到嘟嘟鸟
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
	
	-- 第一次使用新技能完成任务
	if objId == 350040 and ( not GameWizard:isDo(GAMEWIZARD_FIRST_USE_NEW_SKILL) ) and Quest:isQuestRun( 1061 ) then
		SetMonsterQuestPopByState( { config = t_monsterAddTalk[objId], ActorId = actorId } );
	end

	-- 第一次治疗飞龙
	if objId == 351000 and not GameWizard:isDo(GAMEWIZARD_FIRST_CURE_NPC) and Quest:isQuestRun( 1064 ) then
		SetMonsterQuestPopByState( { config = t_monsterAddTalk[objId], ActorId = actorId } );
	end

	-- 掉落包裹
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

	-- 第一次采集任务的晶石
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

	-- 火焰刀进入视野	火焰刀  1054 第一次接触
	local t_showTmpXinShouControl = GetShowTmpXinShouControl();
	local typeId = math.floor( objId/1000 )
	if typeId == 1420 then		
		local t_config =  t_monsterAddTalk[objId];
		local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();

		if not GameWizard:isDo( GAMEWIZARD_FIRST_SAWSWORD ) and Quest:isQuestRun( 1054 ) then
			ShowMidTips( "竞技场中央出现一把火焰刀" );
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
			--ShowMidTips( "天空岛中央出现一把火焰刀" );
			GameWizard:doStep(GAMEWIZARD_SECOND_SAWSWORD);

			if not GameWizard:isDo( GAMEWIZARD_SECOND_POP_SWORD_XINSHOU_HELP_BTN ) then
				t_showTmpXinShouControl:addTmpActor( { memId = actorId, resId = objId, wizardOp = GAMEWIZARD_SECOND_POP_SWORD_XINSHOU_HELP_BTN,
														text = "天空岛中央出现一把火焰刀", xinShouType = XINSHOU_HUO_YAN_DAO2_TYPE } );
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
														text = "天空岛中央出现一把火焰刀", xinShouType = XINSHOU_HUO_YAN_DAO2_TYPE } );
			end
			]]--

			TmpItemPickTipsFrame:SetClientUserData( 0,actorId )
			TmpItemPickTipsFrame:Show();
		end
	end

	-- 炮台进入视野		炮台    3322 盘山道-炮击	and Quest:isQuestRun( 3322 )
	-- TODO: 这些临时道具分别和哪些任务绑定	
	if objId == 1460002 and ( not GameWizard:isDo(GAMEWIZARD_FIRST_SAWPAOTAI) ) then
		--SetMonsterQuestPopByState( { config = t_monsterAddTalk[objId], ActorId = actorId } );
		ShowMidTips( "盘山道中出现一架"..getItemDef( objId ).Name );
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

	-- 手雷进入视野
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
	-- 机甲兽进入视野	机甲兽  1075 排山倒海
	if objId == 1460001 and not GameWizard:isDo(GAMEWIZARD_FIRST_SAW_JIJIASHOU) and Quest:isQuestRun( 1075 ) then
		--SetMonsterQuestPopByState( { config = t_monsterAddTalk[objId], ActorId = actorId, line = 1118 } );
		ShowMidTips( "天空回廊中出现一只机甲兽" );
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

	-- 若掉落的是包裹
	-- 掉落包裹
	local actor		= ActorMgr:FindActor( actorId );
	local szBtnName = actor:getName()..actor:GetID().."的UI名字";
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

	local szStallBtnName = actor:getName()..actor:GetID().."的摊位的头顶UI";
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

	local szStallBtnName = actor:getName()..actor:GetID().."的摊位的头顶UI";
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
	local btn	= FindButton(actor:getName()..id.."的UI名字")
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

	-- 掉落包裹
	if actor:getType() == GAT_ITEM and ( not GameWizard:isDo(GAMEWIZARD_FIRST_GETBOOTY) ) and actor:canPickUp() then
		local itemDef = getItemDef(actor:getResId() );
		if itemDef.ItemTmp.Type == 0 then
			local pickKey			= GetKeyMap("拾取附近物品")
			if pickKey == 0 then
				pickKey = nil
			end
			if pickKey then
				pickKey = KeyToString(pickKey)
			end
			SetMonsterQuestPopByState( { config = {	resId = 9900000, 
														content = pickKey and string.format("左键点击拾取，或使用快捷键(%s)快速拾取", pickKey) or "左键点击拾取",
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
		GuideEffectShow( "", "点击打开天赋界面", "DiathesisButton", "你获得了一个天赋点请点击设置", "uires\\ZhuJieMian\\1.dds", 352, 609, 60, 60, 50, 60, -9, 5, 7, -58, true );
	end
end

function PickTmpItem( nDefId, nActorId )
	if nDefId == 1420000 then	-- 火焰刀
		 FirstPickSword( nActorId );
	elseif nDefId == 1460001 then	-- 机甲兽
		 FirstPickJiJia( nActorId );
	elseif nDefId == 1460002 then	-- 炮台
		 FirstPickPaoTai( nActorId );
	elseif nDefId == 1400002 then	-- 手雷
		 FirstPicShouLei( nActorId );
	end
end

function FirstPickTmpItemByDefID( nDefId )
	local typeId = math.floor( nDefId/1000 )
	if typeId == 1420 then	-- 火焰刀
		 FirstPickSwordByDefID( nDefId );
	elseif nDefId == 1460001 then	-- 机甲兽
		 FirstPickJiJiaByDefID( nDefId );
	elseif nDefId == 1460002 then	-- 炮台
		 FirstPickPaoTaiByDefID( nDefId );
	elseif nDefId == 1400002 then	-- 手雷
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

-- 学习技能触发
function SpringByStudySkill( skillId )
end

-- 进入场景触发
function SpringByEnterMap( mapId )
	-- 第一次限时任务
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

-- 使用物品
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

-- 点击新手指引tips
function XinShouGuideTipsCloseBtn_OnClick()
	local frame = getglobal( this:GetParent() );
	frame:Hide();
end

local t_guideEvents = {		-- 切换目标对象
							--["GE_TARGET_CHANGED"]	= {},
							-- 拾取地上非临时物品
							["GE_BOOTY_GETITEM"]	= {},
							["GE_BOOTY_GET_ALL_ITEM"]	= {},
							-- 升级
							["UI_ACTOR_LEVEL_CHG"]	= {},
							-- 离开副本
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
	-- 若为可以拾取的临时包裹
	if not GameWizard:isDo( GAMEWIZARD_FIRST_GETBOOTY ) then
		GameWizard:doStep( GAMEWIZARD_FIRST_GETBOOTY );
		-- TODO: 其实可以去掉了
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
							content = "    点击可以传送到时间城", 
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

--玩家第一次进入游戏
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
	GuideFrameTalkText:SetText("    当命运的齿轮把世界推向毁灭，当末日的阴影笼罩整个地球，你肩负着人类生死存亡的使命，穿越浩瀚星海，来到这片熟悉而神秘的大陆。\n    踏上未知的旅程吧，满怀希望与憧憬的旅人。当历史的尘埃散去，你将成为众人传颂的英雄！\n\n", 93,56,24 );
	GuideFrameTalkText:AddText( "“鼠标左键”点击地面可以移动，按住“鼠标右键”旋转视角。", 1,129,249 );

	local xinShouBtn = getglobal( this:GetName() .. "XinShouBtn" );
	local rich	 = getglobal( xinShouBtn:GetName() .. "Rich" );
	local nSelfDefineExtendWidth = 10;
	rich:SetClientString( "点击按钮继续游戏" );
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

-- 读取对应的描述信息
function GetDesc(nstep)
    for i = 1, table.getn(guide_enum)  do
        if guide_enum[i] == nstep then
            return guide_desc[i];
        end
    end
    return ""
end


-- 设置已执行过操作
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

-- 设置当前页
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


-- 设置分页
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

-- 新手指引小图标
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
		"数百年前，一个自称为文明议会的势力迅速崛起，大肆掠夺远古遗物，",
		"他们为达到目的不择手段，又掌握着某种神秘的力量，可以召唤远古的恐怖生物，",
		"在他们的控制下死亡军团毁灭了无数的文明，我们被迫离开自己的家乡，",
		"在那个黑暗的年代，无数的种族在血海浩劫中灭绝！ ",
		"拥有土元素和风元素的种族也投靠在黑暗势力之下……虽然是被迫的……",
		"在水元素庇护下的精灵与掌控火元素的迈雅人组成了联军，顽强的抵抗，",
		"终于为世界迎来了一片喘息之机，然而，“天行者”的到来却打破了这短暂的和平。",
		"由于天行者先进的科技，文明议会甚至调动了地底的恶魔，将落日故乡变成一片火海，",
		"为了生存，迈雅人利用神源融合仅残的两大元素，开启了被禁忌的魔法，将大陆分隔在不同的结界内，",
		"作为一名#Race#，你的职责就是击败那些已经现身或是仍潜伏在暗自的敌人，你必须面对挑战，带领同胞走向一个光明的未来……",
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
	-- 判断是否显示飞龙对白
	if bShowTalk then
		AutoFlyMonsterTalk();
	end

	if os.difftime ( os.clock(), nAwardLastTime) >= 1 then
		nAwardLastTime = os.clock(); --重新计时
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
		AwardLeaveTimeFont:SetText("请等待 "..nMin..":"..nSecond);
		GuideAward_BackgroundBtn:Hide();
	end
end


local t_firstArm = { [1] = { desc = "战士", armId = 1019901 }, [2] = { desc = "法师", armId = 1109901 }, 
					[3] = { desc = "枪手", armId = 1209901 }, [4] = { desc = "祭司", armId = 1509901 }, };

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
	SetGameTooltips(this:GetName(), "鼠标点击可获得帮助信息" );
end

function GuideAward_BackgroundBtn_OnEnter()
	GameTooltipRichText:SetWidth(150);
	SetGameTooltips(this:GetName(), "2012为你量身定制一些奖励鼠标左键点击领取" );
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
		nAwardLastTime	= os.clock(); --重新计时
		local nMin		= os.date( "%M", nAwardTime );
		local nSecond	= os.date( "%S", nAwardTime );
		AwardLeaveTimeFont:SetText("请等待 "..nMin..":"..nSecond);
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
--------------------------------------------新手礼包------------------------------------------------------
--////////////////////////////////////////////////////////////////////////////////////////////////////////

local t_LabelNameList =	{};		--标签名表
local szTitleColor		= "#c431D09";
local szContentColor	= "#c6f6149";

local MAX_LV_GIFT			= 32; 	--/* 最多等你奖励次数 */
local MAX_ONEDAY_GIFT		= 8; 	--/* 每日奖励最大次数 */
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
--设置标签页数据
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

-- nAwardState: 0 不可领取 1 可领取 2 已领取 
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
				table.insert(t_LabelNameList,{name="连续在线福利",bCheck=true, nType=CLIENT_GIFT_TYPE_CONTI,nSubType=0,nDataIndex=0,szTable="",nAwardState=0 });
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
					table.insert(t_LabelNameList,{name="连续在线福利",bCheck=true, nType=CLIENT_GIFT_TYPE_CONTI_GROUP,nSubType=0,nDataIndex=0,szTable="",nAwardState=0 });
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
	--特殊福利
	for i = 1, nNum do
		local lvgift = GameWizard:getLVGiftDef( i - 1 );
		local nRet = -1;
		if not bSet then
			table.insert(t_LabelNameList,{name="等级福利",bCheck = true,nType = CLIENT_GIFT_TYPE_LV, nSubType = 0, nDataIndex = 0, szTable = "", nAwardState = nRet});
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
	-- 特权网吧礼包
	if GameWizard:IsVipNetBarUser() then
		local nNum = GameWizard:GetVipNetBarGiftNum();
		if nNum > 0 then
			table.insert( t_LabelNameList,{name="特权网吧礼包",bCheck = true,nType = CLIENT_GIFT_TYPE_ONEDAY, nSubType = 0, nDataIndex = 0, szTable = "VipNetBarGift", nAwardState = -1} )
		end
		for i=1,nNum do
			local BarGift = GameWizard:GetVipNetBarGift( i-1 );
			local nRet = CanGetAward( i-1, 0, 0, "VipNetBarGift" )
			table.insert(t_LabelNameList,{ name=BarGift.Name, bCheck=true, nType=CLIENT_GIFT_TYPE_ONEDAY, nSubType=nSubType, nDataIndex = i, szTable = "VipNetBarGift", nAwardState = nRet, lv = 0 } );
			nSubType = nSubType + 1;
		end
	end	
end

--充值礼包 重新处理
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
		table.insert( t_LabelNameList,{name="充值礼包",bCheck = true,nType=CLIENT_GIFT_TYPE_PROPGIFT, nSubType = 0, nDataIndex = 0, szTable = "", nAwardState = -1} );
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
				--礼包名
				table.insert( t_LabelNameList,{name=propGift.PropsDef[propGift.DefCount-i].Name, bCheck=true, nType=CLIENT_GIFT_TYPE_PROPGIFT, nSubType=nSubType, nDataIndex = propGift.DefCount - i + 1, szTable = "充值礼包", nAwardState = nRet} )
				nSubType = nSubType + 1;
			end
		end
	else
		table.insert( t_LabelNameList,{name="充值礼包",bCheck=true, nType=CLIENT_GIFT_TYPE_PROPGIFT,nSubType=0,nDataIndex=0,szTable="",nAwardState=0 } )
		GuideGiftFramePropGiftBtn:Hide();
	end
end

function BindCDKeyGiftData()
	table.insert(t_LabelNameList,{name="活动礼包",bCheck=true, nType=CLIENT_GIFT_TYPE_CDKEY,nSubType=0,nDataIndex=0,szTable="",nAwardState=0 });
	table.insert(t_LabelNameList,{name="公测特权金卡领取",bCheck=true, nType=CLIENT_GIFT_TYPE_CDKEY,nSubType=1,nDataIndex=1,szTable="CodeGift",nAwardState=0 });
	table.insert(t_LabelNameList,{name="畅玩礼包领取",bCheck=true, nType=CLIENT_GIFT_TYPE_CDKEY,nSubType=2,nDataIndex=2,szTable="CodeGift",nAwardState=0 });
	table.insert(t_LabelNameList,{name="道具礼包领取",bCheck=true, nType=CLIENT_GIFT_TYPE_CDKEY,nSubType=3,nDataIndex=3,szTable="CodeGift",nAwardState=0 });
end

function BindPlayGiftData()
	table.insert(t_LabelNameList,{name="畅玩礼包领取",bCheck=true, nType=CLIENT_GIFT_TYPE_CDKEY,nSubType=0,nDataIndex=0,szTable="",nAwardState=0 });
end

function BindKitData()
	table.insert(t_LabelNameList,{name="道具礼包领取",bCheck=true, nType=CLIENT_GIFT_TYPE_CDKEY,nSubType=0,nDataIndex=0,szTable="",nAwardState=0 });
end

function BindAccGiftData()
	table.insert( t_LabelNameList,{name="运营礼包",bCheck=true, nType=CLIENT_GIFT_TYPE_ACCGIFT,nSubType=0,nDataIndex=0,szTable="",nAwardState=0 } )
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
	table.insert(t_LabelNameList,{name="签到奖励",bCheck=true, nType=CLIENT_GIFT_TYPE_CONT_COVER,nSubType=0,nDataIndex=0,szTable="",nAwardState=0 });
	table.insert(t_LabelNameList,{name="连续签到奖励",bCheck=true, nType=CLIENT_GIFT_TYPE_CONT_COVER,nSubType=1,nDataIndex=1,szTable="CheckIn",nAwardState=0 });
	table.insert(t_LabelNameList,{name="每日成长基金",bCheck=true, nType=CLIENT_GIFT_TYPE_CONT_COVER,nSubType=2,nDataIndex=2,szTable="CheckIn",nAwardState=0 });
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

--获取标签页的上级标签
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

--刷新标签页
function UpdateGiftCategoryList( nStartContentIndex )
	local nCurShowBtn			= 1; -- 此为指向按钮的指针
	local t_selHilightBtnInfo	= { haveSel = false, initCurSelIndex = nCurSelIndex };
	local nCurHilightBtnIndex	= 0;
	local nSubLabelNum = 1;
	local bFound = false;
	for nContentIndex = nStartContentIndex, table.getn( t_LabelNameList ) do
		-- 检查按钮指针是否越界
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
		-- 若为1级节点
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
		-- 若为2级节点
		else
			-- 先查看其父节点是否为展开状态
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

			-- 若其父节点为展开状态, 则显示当前的2级节点内容,否则跳过当前2级节点
			if firstLevelNode.bCheck then
				local t_AwardState = 
					{
						[-1]					= { text = "",			r = 52,  g = 139, b = 116	},
						[CAN_NOT_GET_GIFT]		= { text = "",			r = 52,  g = 139, b = 116	},
						[CAN_GET_GIFT]			= { text = "可领取",	r = 10,  g = 215, b = 0		},
						[ALREADY_GET_GIFT]		= { text = "已领取",	r = 255, g = 60,  b = 0		},
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

	-- 将没使用的按钮隐藏起来
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
-- 要选取的礼品按钮
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
	
	local szGiftLimit	= szTitleColor.."领取条件：#n\n"..szContentColor;
	local szGiftContent = "#n"..szTitleColor.."礼包内容：#n"..szContentColor;
	local szGiftDesc	= szTitleColor.."奖励描述：#n\n"..szContentColor;
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
		
		szGiftLimit = szGiftLimit .. "每天在线时长累积达到"
		if onedaygift.GiftCond.OnlineTM >= 60 then
			szGiftLimit = szGiftLimit .. math.ceil(onedaygift.GiftCond.OnlineTM/60) .. "小时".."可领取#n";
		else
			szGiftLimit = szGiftLimit .. onedaygift.GiftCond.OnlineTM .. "分钟".."可领取#n";
		end

		--[[
		if onedaygift.GiftType == CLIENT_GIFT_TYPE_LV and CanGetAward( onedaygift.GiftCond.LVMin, 0, onedaygift.GiftID, "OneDayGift" ) == 1 then
			bShowGiftClock	= true;
			nLeavingTM		= onedaygift.GiftCond.OnlineTM;
		elseif onedaygift.GiftType == CLIENT_GIFT_TYPE_LV and CanGetAward( onedaygift.GiftCond.LVMin, 0, onedaygift.GiftID, "OneDayGift" ) == 2 then
			szGiftLimit = szGiftLimit.."已领取#n";
		else
			if onedaygift.GiftCond.LVMin == 0 then
				szGiftLimit = szGiftLimit.."可领取#n";
			else
				szGiftLimit = szGiftLimit.."等级满足"..onedaygift.GiftCond.LVMin.."可领取#n";
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
			szGiftLimit = szGiftLimit.."等级满足"..lvgift.LV.."可领取#n";
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
			szGiftLimit = szGiftLimit .."连续".. giftDef.Day .. "天累积在线时长都达到";
		end
		if giftDef.Hour >= 60 then
			szGiftLimit = szGiftLimit .. math.ceil(giftDef.Hour/60) .. "小时".."可领取#n";
		else
			szGiftLimit = szGiftLimit .. giftDef.Hour .. "分钟".."可领取#n";
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
				szGiftLimit = szGiftLimit .."连续".. ContiGroupData.Day .. "天登录游戏可领取"
			else
				szGiftLimit = szGiftLimit .."连续".. ContiGroupData.Day .. "天累积在线时长都达到" .. giftGroupDef.Time .. "分钟".."可领取#n";			
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
		GuideGiftFrameBackBtnAwardNameFont:SetText( "充值礼包" );
	elseif szCurTableName == "VipNetBarGift" then
		local index = 0;
		local szContent = "";
		local BarGift = GameWizard:GetVipNetBarGift( nCurSelIndex - 1 );
		if BarGift.GiftCond.ContOnlineTM >= 60 then
			szGiftLimit = szGiftLimit .. "连续在线" .. math.ceil(BarGift.GiftCond.ContOnlineTM/60) .. "小时".."可领取#n";
		else
			szGiftLimit = szGiftLimit .."连续在线" .. BarGift.GiftCond.ContOnlineTM  .. "分钟" .. "可领取#n";
		end
		
		szContent = "获得" .. GetMoneyDesc(BarGift.MoneyType) .. BarGift.Money;
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
	-- 隐藏以前的界面
	for i = 1, 8 do
		local Btn = getglobal("GuideGiftFrame_ItemBtn"..i);
		local itemName = getglobal("GuideGiftFrame_ItemBtn"..i.."AwardItemFont");
		Btn:Hide();
		itemName:Hide();
	end

	GuideGiftFrameBackBtnAwardNameFont:SetText( "公测特权金卡领取" );
	GuideGiftFrame_GetAwardContentRich:SetText( szTitleColor.."输入公测特权金卡号：#n",255,255,255);
	local szGiftDesc	= szTitleColor.."说明：#n\n"..szContentColor;
	local str = 
[[
1、在上框中输入公测特权金卡号，点击领取，礼包将通过邮件发送到您的邮箱里，请注意查收。
2、每个角色第一次打开公测特权金卡时可获得所有金卡中的道具，请谨慎选择领取的角色。
3、当您再次拥有公测特权金卡来此领取时，打开公测特权金卡仅可获得金卡中的白金卡。
4、收集3张黄金礼券可在游戏内与NPC【考菲】兑换1个“黄金礼包”
5、收集4张白金卡可在游戏内与NPC【考菲】兑换1个“白金福袋” ]];

	szGiftDesc = szGiftDesc .. str;
	
	GuideGiftFrame_GetAwardDescRich:SetText( szGiftDesc,255,255,255);
	--输入框
	GuideGiftFrame_CDKeyEdit:Show();
	GuideGiftFrame_GetAwardBtn:SetPoint( "topleft","GuideGiftFrameBackBtn","topright",-105,66 );
	GuideGiftFrame_GetAwardBtn:Enable();
end

function BondingPlayGiftDesc()
	-- 隐藏以前的界面
	for i = 1, 8 do
		local Btn = getglobal("GuideGiftFrame_ItemBtn"..i);
		local itemName = getglobal("GuideGiftFrame_ItemBtn"..i.."AwardItemFont");
		Btn:Hide();
		itemName:Hide();
	end

	GuideGiftFrameBackBtnAwardNameFont:SetText( "畅玩礼包领取" );
	GuideGiftFrame_GetAwardContentRich:SetText( szTitleColor.."输入畅玩礼包卡号：#n",255,255,255);
	local szGiftDesc	= szTitleColor.."说明：#n\n"..szContentColor;
	local str = 
[[
1、在上框中输入畅玩礼包卡号，点击领取，礼包将通过邮件发送到您的邮箱里，请注意查收。
2、每个账号只有一个角色可以领取畅玩礼包，请谨慎选择领取的角色。 ]];

	szGiftDesc = szGiftDesc .. str ;

	GuideGiftFrame_GetAwardDescRich:SetText( szGiftDesc,255,255,255);
	--输入框
	GuideGiftFrame_CDKeyEdit:Show();	
	GuideGiftFrame_GetAwardBtn:SetPoint( "topleft","GuideGiftFrameBackBtn","topright",-105,66 );
	GuideGiftFrame_GetAwardBtn:Enable();
end

function BondingKitDesc()
	-- 隐藏以前的界面
	for i = 1, 8 do
		local Btn = getglobal("GuideGiftFrame_ItemBtn"..i);
		local itemName = getglobal("GuideGiftFrame_ItemBtn"..i.."AwardItemFont");
		Btn:Hide();
		itemName:Hide();
	end

	GuideGiftFrameBackBtnAwardNameFont:SetText( "道具礼包领取" );
	GuideGiftFrame_GetAwardContentRich:SetText( szTitleColor.."输入道具礼包卡号：#n",255,255,255);
	local szGiftDesc	= szTitleColor.."说明：#n\n"..szContentColor;
	local str = 
[[
1、在上框中输入道具礼包卡号，点击领取，礼包将通过邮件发送到您的邮箱里，请注意查收。
2、道具礼包卡号在幸运魔方中有机会获得，数量有限。 ]];

	szGiftDesc = szGiftDesc .. str ;

	GuideGiftFrame_GetAwardDescRich:SetText( szGiftDesc,255,255,255);
	--输入框
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
	GuideGiftFramePropGiftBtnNum:SetText( "活动期间累计充值金币：" .. (cashNow ~= nil and GameWizard:uLongLongToString( cashNow ) or 0) );
	local beginDate = propGiftInfo.PropsDef[nCurSelIndex-1].BeginDate;
	local endDate = propGiftInfo.PropsDef[nCurSelIndex-1].EndDate;
	local szDate = string.format( "活动时间：#cff1e00%d年%d月%d日-%d年%d月%d日",string.sub(beginDate,1,4),string.sub(beginDate,5,6),string.sub(beginDate,7,8),
					string.sub(endDate,1,4), string.sub(endDate,5,6), string.sub(endDate,7,8));
	GuideGiftFramePropGiftBtnDescRich1:SetText( szDate .. "#c6e6450（剩余时间：#cff1e00" .. GetLimitDate(beginDate,endDate) .. "#c6e6450）", 70, 30, 10 );
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
			btn:SetText("领取奖励");
			local IconPath = GetItemEquipIconPath() .. propGiftInfo.PropsDef[i-1].IconID .. ".tga";
			-- 如果没找到物品的图标则使用默认图标显示
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			icon:SetTexture( IconPath );
			name:SetTextColor( 255, 50, 0 );
			name:SetText( string.format( "%d元充值礼包", math.floor(GameWizard:uLongLongToString( propGiftInfo.PropsDef[i-1].CashUp ) /10) ) );

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
				btn:SetText("已领取")
				btn:SetClientUserData( 0, 0 );
			else
				icon:SetGray( false );
				bkg:SetTexture( "uires\\ui\\Za\\LiBao\\Lv.tga" );
				btn:DisChecked();
				btn:SetClientUserData( 0, i );
				uvTex:SetUVAnimation(80, true);
				uvTex:Show();
			end
			
			local szText = t_descColor[state][1] .. "累计充值金币数：" .. t_descColor[state][2] .. GameWizard:uLongLongToString( propGiftInfo.PropsDef[i-1].CashUp ) .."金币\n" .. t_descColor[state][1] .."礼包内容：" .. t_descColor[state][2];
			for j=1, PROPS_GIFT_ITEM_NUM_TOLUA do
				if propGiftInfo.PropsDef[i-1].PropsGiftItem[j-1].ItemID ~= 0 then
					local itemdef = getItemDef( propGiftInfo.PropsDef[i-1].PropsGiftItem[j-1].ItemID )
					szText = szText .. "、" .. itemdef.Name .. "*" .. propGiftInfo.PropsDef[i-1].PropsGiftItem[j-1].ItemNum;
				end
			end
			rich:SetText( string.gsub( szText, "、", "", 1 ), 70, 30, 10 );
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
	-- 如果没找到物品的图标则使用默认图标显示
	if not IsInExistence( IconPath ) then
		IconPath = DEFAULT_ITEM_PATH;
	end
	itemTex:SetTexture( IconPath );
	itemTex:SetTexRelUV( 1, 1 );
	itemCountFont:SetPoint( "bottomleft", Btn:GetName(), "bottomleft", 0, 10 );
	
	SetEquipImportantEffectUV( itemdef, itemUVTex:GetName());
	itemUVTex:SetTexRelUV( 1, 1 );

	local szItemName = itemdef.Name;
	szItemName = szItemName.."\n数量："..nItemCount;
	
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
			GuideGiftFrameBackBtnLeaveTimeFont:SetText( "领取倒计时："..nHour..":"..nMin..":"..nSecond );
		else
			GuideGiftFrameBackBtnLeaveTimeFont:SetText( "可领取奖励" );
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
-- 签到礼包
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
				ShowMidTips( "请输入公测特权金卡号" );
			elseif g_nFocus == g_nFocus_CHANGYOUGIFT then
				ShowMidTips( "请输入畅玩礼包卡号" );
			elseif g_nFocus == g_nFocus_KITGIFT then
				ShowMidTips( "请输入道具礼包卡号" );
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
		AddGameMiddleTips2("死亡状态不可领取礼包");
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
				MessageBox("","该账号只能领取一次，您确定要领取到当前角色吗");
				MessageBoxFrame:SetClientString("领取福利");
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
			ShowMidTips( "请选中正确的礼包" );
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
								[CDKEY_FAIL_NO_TOLUA] = "礼包领取成功，请注意查收邮件",
								[CDKEY_FAIL_VAILD_TOLUA] = "您输入的公测特权金卡号错误",
								[CDKEY_FAIL_USE_TOLUA] = "您输入的公测特权金卡号已被使用",
								[CDKEY_FAIL_OVERDUE_TOLUA] = "您输入的公测特权金卡号已过期",
								[CDKEY_FAIL_DUMP_TOLUA] = "重复领取相同类型的礼包",
								[CDKEY_FAIL_SYS_TOLUA] = "系统错误",
								[CDKEY_FAIL_MANY_TOLUA] = "多个角色同时在使用该卡号",
								[CDKEY_FAIL_LEVEL_LOW_TOLUA] = "当前角色等级过低",
								[CDKEY_FAIL_LEVEL_HIGH_TOLUA] = "当前角色等级过高"
							}

local tPlayGiftResponeText = {
								[CDKEY_FAIL_NO_TOLUA] = "礼包领取成功，请注意查收邮件",
								[CDKEY_FAIL_VAILD_TOLUA] = "您输入的畅玩礼包卡号错误",
								[CDKEY_FAIL_USE_TOLUA] = "您输入的畅玩礼包卡号已被使用",
								[CDKEY_FAIL_OVERDUE_TOLUA] = "您输入的畅玩礼包卡号已过期",
								[CDKEY_FAIL_DUMP_TOLUA] = "重复领取相同类型的礼包",
								[CDKEY_FAIL_SYS_TOLUA] = "系统错误",
								[CDKEY_FAIL_MANY_TOLUA] = "多个角色同时在使用该卡号",
								[CDKEY_FAIL_LEVEL_LOW_TOLUA] = "当前角色等级过低",
								[CDKEY_FAIL_LEVEL_HIGH_TOLUA] = "当前角色等级过高"
							}

local tKitResponeText = {
								[CDKEY_FAIL_NO_TOLUA] = "礼包领取成功，请注意查收邮件",
								[CDKEY_FAIL_VAILD_TOLUA] = "您输入的道具礼包卡号错误",
								[CDKEY_FAIL_USE_TOLUA] = "您输入的道具礼包卡号已被使用",
								[CDKEY_FAIL_OVERDUE_TOLUA] = "您输入的道具礼包卡号已过期",
								[CDKEY_FAIL_DUMP_TOLUA] = "重复领取相同类型的礼包",
								[CDKEY_FAIL_SYS_TOLUA] = "系统错误",
								[CDKEY_FAIL_MANY_TOLUA] = "多个角色同时在使用该卡号",
								[CDKEY_FAIL_LEVEL_LOW_TOLUA] = "当前角色等级过低",
								[CDKEY_FAIL_LEVEL_HIGH_TOLUA] = "当前角色等级过高"
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
-- TYPE定义
CENTER_XINSHOU_BTN_ITEM		= 0;
CENTER_XINSHOU_BTN_LEVEL	= 1;
CENTER_XINSHOW_BTN_MSG		= 2;
CENTER_CLAN_PWORLD			= 3;
CENTER_CLAN_OPEN_TOUCH		= 4;
CENTER_CLAN_SPY				= 5;
CENTER_CLAN_MACHINE_PWORLD	= 6;
INTELLIGENCE_RACE_JOIN		= 7;
--新增活动引导type
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

GUCHENG_BATTLE_ACTIVITY		= 22;    -- 勇闯失落之域活动

TOP_CAREER_INVITE_ACTIVITY		= 23;		--巅峰之战报名
TOP_CAREER_TRANS_ACTIVITY		= 24;		--巅峰之战传送

TANK_WAR_ACTIVITY				= 25;		--坦克大战

CENTER_CLAN_SPAN_STRONG_WAR		= 26;		--跨服据点战

-- 最多的新手按钮数量
MAX_CENTER_BTS = 5;

local t_centerXinShouBtnControl = 
{

STANDARD_MOVE_SPEED = 180, 
--STANDARD_MOVE_TIME = 10,

-- 设置撞击点
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

-- 删除某种类型的按钮
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

-- 获得当前要显示按钮的数量
t_centerXinShouBtnControl["GetXinShouDataNum"] = 
function ( self )
	return table.getn( self["showXinShouBtnVector"] );
end

-- 获得索引为index上的逻辑数据
t_centerXinShouBtnControl["GetXinShouDataByIndex"] = 
function ( self, targetIndex )
	--assert( targetIndex <= table.getn( self["showXinShouBtnVector"] ) );
	if targetIndex > table.getn( self["showXinShouBtnVector"] ) then
		return;
	end

	return self["showXinShouBtnVector"][targetIndex];
end

-- 显示额外会移动的按钮
t_centerXinShouBtnControl["ShowExtraBtn"] = 
function ( self, data )
	self:AddNewCenterBtn( data );
	local extraBtn = getglobal( "ExtraXinShouCenterGuideButton" );
	extraBtn:SetClientUserData( 0, table.getn( self["showXinShouBtnVector"] ) ); -- 保存逻辑索引]
	
	if data["Type"] == CENTER_CLAN_PWORLD then
		--GameWizard:zeroSetp();
		if not GameWizard:isDo( GAMEWIZARD_FIRST_GET_CLAN_PWORLD ) then	
			GameWizard:doStep(GAMEWIZARD_FIRST_GET_CLAN_PWORLD);
			ShowClanPworldGuideTipsPOP = true;
			XinshouYinDaoGuiShow( "ExtraXinShouCenterGuideButton", "  点击打开面板\n  公会活动助手\n  ——驱逐入侵");
		end
	elseif data["Type"] == CENTER_CLAN_BANNER_GLORY then
		--GameWizard:zeroSetp();
		if not GameWizard:isDo( GAMEWIZARD_FIRST_GET_CLAN__BANNER_GLORY ) then	
			GameWizard:doStep(GAMEWIZARD_FIRST_GET_CLAN__BANNER_GLORY);
			ShowClanPworldGuideTipsPOP = true;
			XinshouYinDaoGuiShow( "ExtraXinShouCenterGuideButton", "  点击打开面板\n  公会活动助手\n  ——战旗荣耀");
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
		XinshouYinDaoGuiShow( "ExtraXinShouCenterGuideButton", "  点击打开面板\n  公会活动助手\n  ——公会商运");
	end
end

-- 增加一个新的中间按钮逻辑
t_centerXinShouBtnControl["AddNewCenterBtn"] = 
function ( self, data )
	table.insert( self["showXinShouBtnVector"], data );
end

-- 清除所有的中间按钮
t_centerXinShouBtnControl["clear"] = 
function ( self )
	for i = 1, table.getn( self["showXinShouBtnVector"] ) do
		table.remove( self["showXinShouBtnVector"] );
	end
end

-- 根据ID来清除按钮
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

-- 根据逻辑索引来清除按钮
t_centerXinShouBtnControl["eraseByIndex"] = 
function ( self, targetIndex )
	table.remove( self["showXinShouBtnVector"], targetIndex );
end
--根据MsgBtn漂移泡泡的索引来清除按钮，此处写给策划脚本删除固定id的泡泡
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
--清除公会驱逐tips
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

-- 更新所有的中间按钮
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
		btn:SetClientUserData( 0, i ); -- 保存逻辑索引
		if not btn:IsShown() then
			btn:Show();
		end
		if ShowClanPworldGuideTipsPOP and data["Type"] == CENTER_CLAN_PWORLD then
			XinshouYinDaoGuiShow( btn:GetName(), "  点击打开面板\n  公会活动助手\n  ——驱逐入侵");
		elseif ShowClanPworldGuideTipsPOP and data["Type"] == CLAN_DART_DELIVERY then
			XinshouYinDaoGuiShow( btn:GetName(), "  点击打开面板\n  公会活动助手\n  ——公会商运");
		end
		nBtnIndex = nBtnIndex + 1;
	end

	for i = nBtnIndex, MAX_CENTER_BTS do
		local btn = getglobal( "XinShouCenterGuideButton"..i );
		btn:Hide();
	end
end

-- 逻辑更新
t_centerXinShouBtnControl["update"] = 
function ( self )
	local extraBtn = getglobal( "ExtraXinShouCenterGuideButton" );
	if not extraBtn:IsShown() then
		return;
	end
	
	-- lastShowBtn是最后一个没显示的按钮
	local lastShowBtn = getglobal( "XinShouCenterGuideButton5" );
	--local firstShowBtn;
	for i = 1, MAX_CENTER_BTS do
		local btn = getglobal( "XinShouCenterGuideButton"..i );
		if not btn:IsShown() then
			lastShowBtn = btn;
			break;
		end
	end
	
	-- 只要跟最后一个没显示的按钮碰撞立刻全量更新
	-- 先检测
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
	-- 其次才能更新描点位置，这要做一个越界检测，即2个碰到一起，要分开
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

-- 出现在中央的新手提示
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
	-- TODO: 以后根据需求再调整，目前先硬绑定
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
				return ShowMidTips("请先关闭待确认的提示框");
			end
			if getResolveState() then
				return ShowMidTips( "分解中，无法执行操作" );
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
-- 点击后对应frame处理
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
			szText = "点击该按键可以打开关于使用#cff0000临时武器-火焰刀#n的指引介绍。"
		else
			szText = "点击该按钮可以打开关于使用"..itemDef.Name.."的指引介绍";
		end
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == CENTER_XINSHOU_BTN_LEVEL then
		local szText = "点击该按钮可以打开升级提示";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == CENTER_CLAN_PWORLD then
		local szText = "点击打开“公会活动助手”\n——驱逐入侵";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == CENTER_CLAN_OPEN_TOUCH then
		local szText = "点击打开“公会活动助手”\n---公会宝藏";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == CENTER_CLAN_SPY then
		local szText = "点击打开“公会活动助手”\n---情报攻防战";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == CENTER_CLAN_MACHINE_PWORLD then
		local szText = "点击打开“公会活动助手”\n---守护公会枢纽";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == INTELLIGENCE_RACE_JOIN then
		local szText = "点击打开“智力竞赛活动助手”\n—全服智力竞赛";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == WHETHER_REAL_BOSS then
		local szText = "点击打开“活动助手”\n—魔影重重";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == COWBOY_BUSY_FINALS then
		local szText = "点击打开“活动助手”\n—牛仔很忙-决赛";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == HAPPY_CARNIVAL_ACTIVITY then
		local szText = "点击打开“活动助手”\n—欢乐嘉年华";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == TANK_WAR_ACTIVITY then
		local szText = "点击打开“活动助手”\n—轰炸大战";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == HEAVEN_PIE_ACTIVITY then
		local szText = "点击打开“活动助手”\n—天降横福";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == DARK_COMING_ACTIVITY then
		local szText = "点击打开“活动助手”\n—黑暗来袭";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == LIGHT_SHADOW_FIGHT then
		local szText = "点击打开“活动助手”\n—光影之战";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == CENTER_CLAN_STRONG_WAR then
		local szText = "点击打开“据点战”\n---战场信息";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == CENTER_WORLD_CITY_WAR then
		local szText = "点击打开“时间城争霸战”\n---战场信息";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == CENTER_XINSHOU_SPAR_EQURATE then
		local szText = "点击该按钮可以打开晶石提炼面板";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == CENTER_CLAN_BANNER_GLORY then
		local szText = "点击打开“活动助手”\n—战旗荣耀";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == MACHINE_WAR_ACTIVITY then
		local szText = "点击打开“活动助手”\n—尖峰时刻";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == LIFE_IS_MOVEMENT_ACTIVITY then
		local szText = "点击打开“活动助手”\n—生命在于运动";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == SPARTA_PWORLD_ACTIVITY then
		local szText = "点击打开“活动助手”\n—末世荣耀殿堂";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == CLAN_DART_DELIVERY then
		local szText = "点击打开“活动助手”\n—公会商运";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == GUCHENG_BATTLE_ACTIVITY then
		local szText = "点击打开“活动助手”\n—勇闯失落之域";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == TOP_CAREER_INVITE_ACTIVITY then
		local szText = "点击打开“活动助手”\n—末世巅峰-报名";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == TOP_CAREER_TRANS_ACTIVITY then
		local szText = "点击打开“活动助手”\n—末世巅峰-传送";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
	elseif t_xinShouData["Type"] == CENTER_CLAN_SPAN_STRONG_WAR then
		local szText = "点击打开“跨服据点战”\n---战场信息";
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

------------------------------充值礼包---------------------------------

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
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "充值礼包" then
		return;
	end
	local idx = this:GetClientUserData( 0 );
	if idx ~= 0 then
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer == nil then return end;
		if mainplayer:isDead() then
			AddGameMiddleTips2("死亡状态不可领取礼包");
			return;
		end
		MessageBox("","该账号只能领取一次，您确定要领取到当前角色吗");
		MessageBoxFrame:SetClientString("充值礼包");
		MessageBoxFrame:SetClientUserData( 0,idx );
	end
end

function GetLimitDate( beginDate, endDate )
	beginDate = string2time( beginDate .. "000000");
	endDate = string2time( endDate .. "235959");
	local serverTime = getServerTime().sec;
	if serverTime < beginDate then
		return "未开始";
	elseif serverTime > endDate then
		return "已结束";
	else
		local day = math.floor( (endDate - serverTime) / 86400 ) + 1;
		return day .. "天";
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

----------------新建角色进入后选择视角模式界面--------------------------------------
function SelectCameraModeGuideFrame_OnShow()
	SelectCameraModeGuideFrameInfo:SetText( "进入游戏后可通过#n#c4eccdbEsc-游戏设置-使用3D解锁功能#n进行更改", 255,255,190 )
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
