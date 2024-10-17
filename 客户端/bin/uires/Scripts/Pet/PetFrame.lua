-- 新宠物系统
PET_WASH_GENIUS_MONEY		= 100;
PET_ADD_SKILL_GRID_MONEY	= 100;
PET_WASH_SKILL_GRID_MONEY	= 100;
-- 宠物技能书
RESULT_PET_SKILL_BOOK		= 89;
--洗技能格道具
RESULT_PET_WASH_SKILL_GRID	= 193;
-- 宠物升级技能
RESULT_PET_SKILL_UP_STUFF	= 211;
-- 洗资质道具
RESULT_PET_WASH_GENIUS		= 192;
-- 宠物强化成长符
RESULT_PET_STRENGTHEN_SAFE	= 194;
-- 宠物强化成长材料
RESULT_PET_STRENGTHEN_STUFF	= 200;
-- 宠物增强属性
RESULT_PET_ADD_ATTR			= 195;
-- 宠物忠诚度上限
MAX_PET_LOVE_VAL			= 10000;
-- 宠物出战忠诚度要求
PET_FIGHT_LOVEVAL			= 3000;
MAX_PET_NUM = 5;
-- 最多融合次数
MAX_PET_RONG_HE_NUM			= 3;
-- 最大成长强化等级
MAX_STRENGTH_LEVEL			= 12;
-- 合体最低等级
PEIT_HETI_MIN_LEVEL			= 60;
local ModelAngle = 0;
-- 宠物增强面板

-- 宠物主面板
-- @param: 传入宠物实体的动态信息
-- @action: 显示宠物的详细信息
PET_GENIUS_D = 0; 	--/* 普通资质 */
PET_GENIUS_C = 1; 	--/* 良好资质 */
PET_GENIUS_B = 2; 	--/* 优秀资质 */
PET_GENIUS_A = 3; 	--/* 杰出资质 */
PET_GENIUS_S = 4; 	--/* 完美资质 */
-- 宠物最多技能格位
MAX_LOCK_SKILL_GRID	= 1;
MAX_PET_SKILL_GRID	= 5;
MAX_PET_PACKAGE		= 8;
local MAX_PET_ONE_PAGE = 6;
local t_mainSelPetInfo = { heightId = 0, lowId = 0, id = 0, index = -1, getNew = false };
local requestStrengthen
--宠物技能解绑消耗
local t_PetUnBindNeed	= {
		[CLIENT_PET_GENIUS_D] = CLIENT_PET_GENIUS_D_UNBIND_MONEY,
		[CLIENT_PET_GENIUS_C] = CLIENT_PET_GENIUS_C_UNBIND_MONEY,
		[CLIENT_PET_GENIUS_B] = CLIENT_PET_GENIUS_B_UNBIND_MONEY,
		[CLIENT_PET_GENIUS_A] = CLIENT_PET_GENIUS_A_UNBIND_MONEY,
		[CLIENT_PET_GENIUS_S] = CLIENT_PET_GENIUS_S_UNBIND_MONEY,
	}

function SetMainSelPetInfo( data )
	t_mainSelPetInfo = data;
end

function GetMainSelPetInfo()
	return t_mainSelPetInfo;
end

function initPetFrameData()
	t_mainSelPetInfo = { heightId = 0, lowId = 0, id = 0, index = -1 };
	SetSeeZiZhiPetInfo( nil );
end

-- 全局辅助函数
-- 获取出征的宠物的索引
function getFightPetIndex()
	local nFightPetIndex	= -1;
	local mainplayer		= ActorMgr:getMainPlayer();
	local petPackage		= mainplayer:getPet();
	for i = 1, petPackage:getPetNum() do
		if petPackage:isPetInState( i - 1, CLIENT_PET_STAT_FIGHT ) then
			nFightPetIndex = i - 1;
			break;
		end
	end

	return nFightPetIndex;
end

function GetFightPet( actor )
	local petPackage	= actor:getPet();
	for i = 1, CLIENT_PET_MAX_FIGHT_SHI do
		local petActor = petPackage:getFightPet( i - 1 );
		if petActor ~= nil then
			return petActor;
		end
	end

	return nil;
end

function isAnyPetCamp()
	return getFightPetIndex() ~= -1;
end

t_geniusDesc =	{
								[CLIENT_PET_GENIUS_D] = { value = "普通", color = { r = 255,	g = 255,	b = 255	},	desc="普通品质", 
															hexiColor = "ffffff", money = CLIENT_PET_GENIUS_D_WASH_MONEY, }, 
								[CLIENT_PET_GENIUS_C] = { value = "良好", color = { r = 63,	g = 193,	b = 23	},	desc="良好品质", 
															hexiColor = "3fc117", money = CLIENT_PET_GENIUS_C_WASH_MONEY,	},
								[CLIENT_PET_GENIUS_B] = { value = "优秀", color = { r = 51,	g = 141,	b = 253	},	desc="优秀品质", 
													hexiColor = "338dfd", money = CLIENT_PET_GENIUS_B_WASH_MONEY, },
								[CLIENT_PET_GENIUS_A] = { value = "杰出", color = { r = 142,	g = 92,		b = 216	},	desc="杰出品质", 
															hexiColor = "8e5cd8", money = CLIENT_PET_GENIUS_A_WASH_MONEY, },
								[CLIENT_PET_GENIUS_S] = { value = "完美", color = { r = 245,	g = 115,	b = 0	},	desc="完美品质", 
													hexiColor = "f57300", money = CLIENT_PET_GENIUS_S_WASH_MONEY,	},
						};

-- 
function GetPetGeniusValueDesc( petInfo )
	local t_geniusInfo = t_geniusDesc[petInfo.PetGenius];
	if t_geniusInfo == nil then
		return "普通";
	end

	return t_geniusInfo["value"];
end

function GetPetGeniusDesc( petInfo )
	local t_geniusInfo = t_geniusDesc[petInfo.PetGenius];
	if t_geniusInfo == nil then
		return "普通品质";
	end

	return t_geniusInfo["desc"];
end

function GetPetGeniusColor( petInfo )
	local t_geniusInfo = t_geniusDesc[petInfo.PetGenius];
	if t_geniusInfo == nil then
		return { r = 255,	g = 255,	b = 255	};
	end

	return t_geniusInfo["color"];
end

t_petDaiShu = 
{ 
-- 普通
[CLIENT_PET_TYPE_NORMAL] =	{ 
		[1] = { path = "uires\\ZhuJieMian\\1.dds", UV = { x = 377, y = 756, width = 22, height = 16 }, }, 
		[2] = { path = "uires\\ZhuJieMian\\1.dds", UV = { x = 400, y = 756, width = 22, height = 16 }, }, 
		},

-- 变异
[CLIENT_PET_TYPE_ABERRANCE] =	{ 
		[1] = { path = "uires\\ZhuJieMian\\1.dds", UV = { x = 423, y = 756, width = 22, height = 16 }, }, 
		[2] = { path = "uires\\ZhuJieMian\\1.dds", UV = { x = 446, y = 756, width = 22, height = 16 }, }, 
		},
};
-- 宠物类别						-- 277
local t_petTypeTextures	=	{	[0] = { desc = "血宠", path = "uires\\ZhuJieMian\\1.dds", UV = { x = 258, y = 494, width = 18, height = 18 } }, 
								[1] = { desc = "防宠", path = "uires\\ZhuJieMian\\1.dds", UV = { x = 296, y = 494, width = 18, height = 18 } },
								[2] = { desc = "攻宠", path = "uires\\ZhuJieMian\\1.dds", UV = { x = 277, y = 494, width = 18, height = 18 } },
							};
t_petLoveLevelTextures = {
									[0] = { UV = { x = 952, y = 627, width = 20, height = 17 } },
									[1] = { UV = { x = 952, y = 645, width = 20, height = 17 } },
									[2] = { UV = { x = 973, y = 645, width = 20, height = 17 } },
								};
--[[
local t_texturesDef =	{
							-- 宠物性别
							["PetFramePetGenderBtnTex"]	=	{	[GENDER_MAN]	=	{	
																					path = "uires\\ui\\MianBan  12.dds", 
																					UV = { x = 659, y = 517, width = 24, height = 27 } 
																				},
															[GENDER_WOMAN]	=	{	
																					path = "uires\\ui\\MianBan  12.dds", 
																					UV = { x = 634, y = 517, width = 24, height = 27 } 
																				},
														},
							["WatchPetFramePetGenderBtnTex"]	=	{	[GENDER_MAN]	=	{	
																					path = "uires\\ui\\MianBan  12.dds", 
																					UV = { x = 659, y = 517, width = 24, height = 27 } 
																				},
															[GENDER_WOMAN]	=	{	
																					path = "uires\\ui\\MianBan  12.dds", 
																					UV = { x = 634, y = 517, width = 24, height = 27 } 
																				},
														},
							["SeePetZiZhiFramePetGenderBtnTex"]	=	{	[GENDER_MAN]	=	{	
																					path = "uires\\ui\\MianBan  12.dds", 
																					UV = { x = 659, y = 517, width = 24, height = 27 } 
																				},
															[GENDER_WOMAN]	=	{	
																					path = "uires\\ui\\MianBan  12.dds", 
																					UV = { x = 634, y = 517, width = 24, height = 27 } 
																				},
														},

							-- 宠物法魔类型
							["PetFramePetAtkTypeBtnTex"]	=	{	
															-- true PhsicAtk ~= 0
															[true]	=	{ 
																			path = "uires\\ui\\MianBan  3.dds", 
																			UV = { x = 333, y = 968, width = 25, height = 25 } 
																		},
															-- false not IceAtk == 0
															[false]	=	{ 
																			path = "uires\\ui\\MianBan  3.dds", 
																			UV = { x = 359, y = 968, width = 25, height = 25 } 
																		},
														},
							["WatchPetFramePetAtkTypeBtnTex"]	=	{	
															-- true PhsicAtk ~= 0
															[true]	=	{ 
																			path = "uires\\ui\\MianBan  3.dds", 
																			UV = { x = 333, y = 968, width = 25, height = 25 } 
																		},
															-- false not IceAtk == 0
															[false]	=	{ 
																			path = "uires\\ui\\MianBan  3.dds", 
																			UV = { x = 359, y = 968, width = 25, height = 25 } 
																		},
														},
							["SeePetZiZhiFramePetAtkTypeBtnTex"]	=	{	
															-- true PhsicAtk ~= 0
															[true]	=	{ 
																			path = "uires\\ui\\MianBan  3.dds", 
																			UV = { x = 333, y = 968, width = 25, height = 25 } 
																		},
															-- false not IceAtk == 0
															[false]	=	{ 
																			path = "uires\\ui\\MianBan  3.dds", 
																			UV = { x = 359, y = 968, width = 25, height = 25 } 
																		},
														},
						};
]]--
PET_SKILL_DISABLE		= 0;	-- /* 未开启 */
PET_SKILL_ATK			= 1;	-- /* 攻击类技能 */
PET_SKILL_OWNER_BUFF	= 2;	-- /* 佑主类技能 */
PET_SKILL_SELF_BUFF		= 3;	-- /* 增强类技能 */

PET_TYPE_NORMAL		= 0; 	-- /* 普通宠物 */
PET_TYPE_ABERRANCE	= 1; 	-- /* 异型宠物 */
local t_skillGridTga = 
						{
						[PET_SKILL_ATK]			= { path = [[uires\TuPianLei\ChongWuJiNengLeiXing\1.tga]], typeDesc = "攻击型" },
						[PET_SKILL_OWNER_BUFF]	= { path = [[uires\TuPianLei\ChongWuJiNengLeiXing\3.tga]], typeDesc = "佑主型" },
						[PET_SKILL_SELF_BUFF]	= { path = [[uires\TuPianLei\ChongWuJiNengLeiXing\2.tga]], typeDesc = "增强型" },
						};

-- doModal
-- 用于模拟doModal模式的弹出式对话框
local t_modalFrame =	{	modalFrames		=	{	"DropPetConfirmFrame", "OpenPetSkillConfirmFrame" },


							disabledButtons	=	{	"PetFrame_PrePageBtn",	"PetFrame_NextPageBtn",		"PetFrame_FightBtn", 
													"PetFrame_BackUpBtn",	"PetFrame_TameBtn",			"PetFrame_FreedomBtn", 
													"PetFrame_CloseBtn"
												},
						};

-- 点击关闭宠物面板的时候关闭
function PetFrame_CloseBtn_OnClick()	
	local frame =getglobal( this:GetParent() );
	local t_clickEvent =	{
							["PetFrame"]		= { func = function ()
															for _, name in ipairs( t_modalFrame.modalFrames ) do
																local modalFrame = getglobal( name );
																if modalFrame:IsShown() then
																	return;
																end
															end
															frame:Hide();
															end
													},
							["WatchPetFrame"]		= { func = function ()
															frame:Hide();
															end
													},
							["SeePetZiZhiFrame"]		= { func = function ()
															frame:Hide();
															end
													},
							};
	
	t_clickEvent[this:GetParent()].func();
	
end

SKILL_ATTR_NONE		= 0;	-- 无技能
SKILL_ATTR_ICE		= 1;	-- 法攻 
SKILL_ATTR_FIRE		= 2;	-- 火攻
SKILL_ATTR_THUNDER	= 3;	--  
SKILL_ATTR_LIGHT	= 4;	-- 
SKILL_ATTR_NIGHT	= 5;	-- 
SKILL_ATTR_PHSIC	= 6;	-- 物攻	

PET_MAX_FIGHT_SHI	= 1;	-- 同时最多出征的宠物数量
-- 宠物攻击类型
local t_attackTypeDesc = { [0] = { desc = "物    攻"}, [1] = { desc = "物    攻"}, [2] = { desc = "法    攻"}, [3] = { desc = "物    攻"}, };

local t_buttonEvent =	{
							["PetFrame_PrePageBtn"]	= {},	["PetFrame_NextPageBtn"]	= {},	["PetFrame_FightBtn"]	= {},  
							["PetFrame_BackUpBtn"]	= {},	["PetFrame_TameBtn"]		= {},	["PetFrame_FreedomBtn"]	= {},	
							["PetFrame_CloseBtn"]	= {},
						};


function PetFrame_Btn_OnLeave()
	GameTooltip:Hide()
	WideGameTooltip:Hide()
end

-- 出征按钮
function PetFrame_FightBtn_OnEnter()
	--[[
	if t_mainSelPetInfo["id"] == 0 then
		return;
	end
	--]]
	
	local szText = "召唤宠物与你一起并肩作战";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName(), vertical = "bottom" } );
end

-- 驯服按钮
function PetFrame_TameBtn_OnEnter()
	--[[
	if t_mainSelPetInfo["id"] == 0 then
		return;
	end
	--]]
	local szText = "使用“驯兽糖果”增加宠物忠诚度";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName(), vertical = "bottom" } );
end

-- 召回按钮
function PetFrame_BackUpBtn_OnEnter()
	--[[
	if t_mainSelPetInfo["id"] == 0 then
		return;
	end
	--]]
	local szText = "可使用该按钮召回宠物";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName(), vertical = "bottom" } );
end

-- 放生按钮
function PetFrame_FreedomBtn_OnEnter()
	--[[
	if t_mainSelPetInfo["id"] == 0 then
		return;
	end
	--]]

	local szText = "放生宠物您有机会获得灵源或者因子的一种";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName(), vertical = "bottom" } );
end

function PetFrameXinShouBtn_OnHide()
	local t_hideIcons = { "PetFrame_TameBtnUVAnimationTex", "PetFrame_FightBtnUVAnimationTex" };
	util.HideIcons( t_hideIcons );
end

--  宠物代数按钮
function PetFrame_DaiShuBtn_OnEnter()
	local petInfo = GetShowTipsPetInfo( this:GetParent() );
	if not petInfo then
		return;
	end

	local petDef		= getPetDef( petInfo.PetID );
	local szText;
	--[[if petDef.Type == PET_TYPE_ABERRANCE then
		szText = ( petInfo.Hierarchy == 2 ) and "二代变异宠物，不能融合" or "一代变异宠物，可以融合";
	else
		szText = ( petInfo.Hierarchy == 2 ) and "二代宠物，不能融合" or "一代宠物，可以融合";
	end]]
--[[
	local mainplayer		= ActorMgr:getMainPlayer();
	local petPackage		= mainplayer:getPet();
	
	if petPackage:isExtraBattlePet( petInfo ) then 
		szText = (petDef.Type == PET_TYPE_ABERRANCE) and "变异战宠" or "一代战宠"
	else 
		szText = (petDef.Type == PET_TYPE_ABERRANCE) and "变异宠物" or "一代宠物"
	end
--]]
	szText = "初始成长："..petDef.Important.."\n当前成长："..petInfo.Imp.."\n\n"
	local ImpUpDef = getPetImpUpDef( petInfo.Imp );
	if ImpUpDef == nil then 
		szText = szText.."当前已达到最高成长等级"
	else 
		szText = szText.."当潜力达到100%后可前往时间城，\n在莉莉的帮助下进化为"..( petInfo.Imp + 1 );
	end
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function PetFrame_TipAreaBtn_OnEnter()
	local szText = "战宠合体规则\n1.玩家使用战宠合体后,将获得该宠物的40%属性加成\n2.玩家使用战宠合体后,将会获得其他未合体战宠的25%属性加成";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function PetFrameBattlePetBtn_OnEnter()
	local szText = "战宠:\n\t\t普通宠物等级达到60，品质达到优秀,可在时间城拉奥多处,进阶为更加强大的战宠\n\t\t战宠合体,主人将获得该战宠的40%属性,并额外获得其余每只战宠25%的属性";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end
-- 宠物等级按钮
function PetFrame_PetLvValueBtn_OnEnter()
	local petInfo = GetShowTipsPetInfo( this:GetParent() );
	if not petInfo then
		return;
	end

	local szText = "    当您的宠物等级超过您时，它获得的经验将逐渐减少，当它的等级超过您10级时，将无法被召唤！";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

local t_petTypeTips =	{
						[0] = { desc = "血宠", tips = "该宠物升级将获得更多的生命成长以及少量的双防御和闪避成长" },
						[1] = { desc = "防宠", tips = "该宠物升级将获得更多双系防御成长以及少量的生命成长和闪避成长" },
						[2] = { desc = "攻宠", tips = "该宠物升级将获得更多双系攻击成长以及少量的命中成长" },
						--[3] = { desc = "敏宠", tips = "该宠物升级将获得更多的命中、敏捷" },
						};
-- 宠物攻击类型 物理 魔法
function PetFrame_PetAtkTypeBtn_OnEnter()
	local nPetID = GetShowTipsPetID( this:GetParent() );
	if nPetID == 0 then
		return;
	end
		
	local petDef	= getPetDef( nPetID );
	local szText	= t_petTypeTips[petDef.PetAttrType].tips;
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function PetFrameLoveLevelBtn_OnClick()
	if this:GetParent() == "PetFrame" then
		local petInfo = GetShowTipsPetInfo( this:GetParent() )
		local MAX_LOVE_LEVEL = 2
		if petInfo.LoveLevel < MAX_LOVE_LEVEL then
			if GameStallManager:isInStallIPet( SHITYPE, t_mainSelPetInfo.heightId, t_mainSelPetInfo.lowId ) then
				ShowMidTips( "宠物上架中，不能进行爱心操作" );
				return;
			end
			if GameExchange:isInExchangePetByWID( t_mainSelPetInfo.heightId, t_mainSelPetInfo.lowId ) then
				ShowMidTips( "宠物正在交易中，不能进行爱心操作" );
				return;
			end

			PetAddLoveLevelMessageFrame:SetClientUserData( 0, petInfo.WID.nHeight )
			PetAddLoveLevelMessageFrame:SetClientUserData( 1, petInfo.WID.nLow )
			PetAddLoveLevelMessageFrame:Show()
		end
	end
end

tLoveLevelDesc = {			[0] = [[宠物与您#cff1414相见恨晚#n（宠物可交易二次）
#G点击心形按钮可使您的宠物晋级为不离不弃状态#n

#cf57300当宠物与您处于不离不弃状态时:
宠物所有基础属性增加10%
宠物忠诚度上限提升20%
宠物不能被再次交易#n]],
							[1] = [[宠物与您#cff64ff相识相知#n（宠物可交易一次）
#G点击心形按钮可使您的宠物晋级为不离不弃状态#n

#cf57300当宠物与您处于不离不弃状态时:
宠物所有基础属性增加10%
宠物忠诚度上限提升20%
宠物不能被再次交易#n]],
							[2] = [[宠物与您#cffe132不离不弃#n：
宠物所有基础属性增加10%
宠物忠诚度上限提升20%
宠物不能被再次交易#n]],
					}
function PetFrameLoveLevelBtn_OnEnter()
	local petInfo = GetShowTipsPetInfo( this:GetParent() )
	ShowGameTooltip( { rich = "WideGameTooltipRichText", tipsFrame = "WideGameTooltip", text = tLoveLevelDesc[petInfo.LoveLevel], frame = this:GetParent(),
						button = this:GetName() } );
end

-- 宠物资质按钮	类别	普通 良好 优秀 杰出 完美 
local t_geniusTips =	{
								--[PET_GENIUS_D] = 
								-- 普通 -- 一般
								"#cffffff普通：宠物获得正常属性成长#n\n", 
								--[PET_GENIUS_C] = 
								-- 良好 -- 优秀
								"#c3fc117良好：比普通资质宠物获得更多成长#n\n", 
								--[PET_GENIUS_B] = 
								-- 优秀 -- 精良
								"#c338dfd优秀：比良好资质宠物获得更多成长#n\n",
								--[PET_GENIUS_A] = 
								-- 杰出 -- 史诗
								"#c8e5cd8杰出：比优秀资质宠物获得更多成长#n\n", 
								--[PET_GENIUS_S] = 
								-- 完美 -- 逆天
								"#cf57300完美：比杰出资质宠物获得更多成长#n\n"
						};

function PetFrame_PetGeniusValuBtn_OnEnter()
	local petInfo = GetShowTipsPetInfo( this:GetParent() );
	if not petInfo then
		return;
	end
	
	local szText = "#cffffff普通：宠物获得正常属性成长#n\n#c3fc117良好：比普通资质宠物获得更多成长#n\n#c338dfd优秀：比良好资质宠物获得更多成长#n\n#c8e5cd8杰出：比优秀资质宠物获得更多成长#n\n#cf57300完美：比杰出资质宠物获得更多成长#n"
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(), 
						button = this:GetName() } );
end

-- TODO: 宠物性格按钮
local t_PetCharacteDesc = { ["认真"] = { tips = "认真：提高命中和闪避5%" },		["鲁莽"] = { tips = "鲁莽：提高暴击率3%" }, 
							["热血"] = { tips = "热血：提高物攻和法攻5%" },		["胆小"] = { tips = "胆小：提高物防和法防5%" }, 
							["冷静"] = { tips = "冷静：提高生命上限10%" },		["英勇"] = { tips = "英勇：提高替身率5%" }, 
							["孤僻"] = { tips = "孤僻：降低替身率5%" },			["坦率"] = { tips = "坦率：提高命中、物攻、物防、暴击率、生命值上限5% ( 仅二代宠可获得 )" }, 
							["独立"] = { tips = "独立：提高闪避、法攻、法防、暴击率、生命值上限5% ( 仅二代宠可获得 )" },  
							};

function IsPetTongLing( petInfo )
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	
	if not petPackage:isPetInState( petInfo.WID, CLIENT_PET_STAT_FIGHT ) then
		return false;
	end

	local GameTrump = mainplayer:getTrump(); 
	local campFairy	= GameTrump:getCampRoleFairy();
	if campFairy == nil then
		return false;
	end
	
	local TrumpDef = GameTrump:getFairyDef( campFairy.FairyID, campFairy.FairyLevel );
	local TraitDef = GameTrump:getPetTraitDef( TrumpDef.TraitID,campFairy.StarLvl );

	if TraitDef == nil then
		return false;
	end

	return true;
end

-- 宠物总成长按钮
function PetFrame_PetGrowthBtn_OnEnter()
	local petInfo = GetShowTipsPetInfo( this:GetParent() );
	if not petInfo then
		return;
	end
	
	local szText = "宠物成长总值，是衡量宠物总体成长能力的标准";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

-- 宠物战斗力显示按钮
function PetFrame_FightValueBtn_OnEnter()
	local szText = "宠物当前战斗力，该数值受到宠物当前能力影响";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function PetFrame_QiangHuaStarBackBtn_OnEnter()
	local szText = "宠物强化直接增强宠物战斗属性，每次强化提升5%";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function PetFrame_DevelopmentBuildUpFrame_QiangHuaSmallStarBtn_OnEnter()
	local petInfo = GetShowTipsPetInfo( this:GetParent() );
	if not petInfo then
		return;
	end
	
	local mainplayer		= ActorMgr:getMainPlayer();
	local petPackage		= mainplayer:getPet();
	local nQueryLevel		= this:GetClientID();
	local nIncrAttr			= petPackage:getStrthenVal( petInfo, nQueryLevel - 1 );
	if nIncrAttr == 0 then
		return;
	end

	local szText = "";
	if nIncrAttr == MAX_INCR_ATTR_PERCENT then
		szText = "宠物完美强化，所有属性增加"..nIncrAttr.."%";
	else
		szText = "宠物普通强化，所有属性增加"..nIncrAttr.."%";
	end

	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = "cursor", vertical = "top", horizonal="right" } );
end

-- 宠物召唤等级显示按钮
function PetFrame_CallLvValueBtn_OnEnter()
	local nPetID = GetShowTipsPetID( this:GetParent() );
	if nPetID == 0 then
		return;
	end
	
	local petDef = getPetDef( nPetID );
	-- ..petDef.Name
	local szText = "需角色等级达到"..petDef.FightLevel.."级时才能召唤";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

-- 宠物成长强化显示按钮
function PetFrame_GrowthStrengValueBtn_OnEnter()
	--[[
	local petInfo = GetShowTipsPetInfo( this:GetParent() );
	if not petInfo then
		return;
	end
	--]]
	local szText = "宠物强化等级，影响宠物各项成长";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

-- 宠物HP条按钮
function PetFrame_HPBtn_OnEnter()
	--[[
	local petInfo = GetShowTipsPetInfo( this:GetParent() );
	if not petInfo then
		return;
	end
	--]]
	local szText = "生命值为0时，宠物将会死亡\n使用宠物药水可增加被召唤宠物的生命\n宠物强化后可提升该属性";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

-- 宠物EXP条按钮
function PetFrame_EXPBtn_OnEnter()
	--[[
	local petInfo = GetShowTipsPetInfo( this:GetParent() );
	if not petInfo then
		return;
	end
	--]]
	local szText = "宠物经验达到升级上限时，宠物将自动升级";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

-- 宠物攻击成长按钮
function PetFrame_AttackGrowthValueBtn_OnEnter()
	local szText = "影响宠物升级后获得的攻击力，此属性值越大宠物升级后增加的攻击力越多";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

-- 宠物生命成长按钮
function PetFrame_LifeGrowthValueBtn_OnEnter()
	--[[
	local petInfo = GetShowTipsPetInfo( this:GetParent() );
	if not petInfo then
		return;
	end
	--]]
	local szText = "影响宠物升级后获得的生命值，此属性值越大宠物升级后增加的生命值越多";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

-- 宠物物防成长按钮
function PetFrame_PhysicDefGrowthValueBtn_OnEnter()
	local szText = "影响宠物升级后获得的物理防御值，此属性值越大宠物升级后增加的物理防御值越高";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

-- 宠物魔防成长按钮
function PetFrame_IceDefGrowthValueBtn_OnEnter()
	local szText = "影响宠物升级后获得的魔法防御值，此属性值越大宠物升级后增加的魔法防御值越高";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

-- 宠物命中成长按钮
function PetFrame_HitGrowthValueBtn_OnEnter()
	--[[
	local petInfo = GetShowTipsPetInfo( this:GetParent() );
	if not petInfo then
		return;
	end
	--]]
	local szText = "影响宠物升级后获得的命中加成，此属性值越大宠物升级后命中率越高";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

-- 宠物闪避成长按钮
function PetFrame_FleeGrowthValueBtn_OnEnter()
	--[[
	local petInfo = GetShowTipsPetInfo( this:GetParent() );
	if not petInfo then
		return;
	end
	--]]
	local szText = "影响宠物升级后获得的闪避加成，此属性值越大宠物升级后闪避率越高";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

local t_attackTypeTips = {	[SKILL_ATTR_PHSIC]	= { desc = "物 攻", tips = "宠物物理攻击，通过宠物增强可直接增强该属性，最多5次" },											[SKILL_ATTR_ICE]	= { desc = "法 攻", tips = "宠物魔法攻击，通过宠物增强可直接增强该属性，最多5次" } };
-- 宠物物攻，法攻按钮
function PetFrame_PhyAtkValueBtn_OnEnter()
	local mainplayer = ActorMgr:getMainPlayer();
	local petPackage = mainplayer:getPet();
	if petPackage:getPetNum() == 0 then
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = t_attackTypeTips[SKILL_ATTR_PHSIC].tips, 
							frame = this:GetParent(), button = this:GetName() } );
		return;
	end

	local petInfo = GetShowTipsPetInfo( this:GetParent() );
	if not petInfo then
		return;
	end
	
	local petDef		= getPetDef( petInfo.PetID );
	local petSkillDef	= SkillMgr:getSpellStaticAtt( petDef.NormalSkill, 1 );
	local szText		= t_attackTypeTips[petSkillDef.MainAttr].tips;
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function PetFrame_TotalAttackValueBtn_OnEnter()
	local szText		= "宠物攻击，通过宠物强化可提升该属性，最多强化12次"
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function PetFrame_MaxLifeValueBtn_OnEnter()
	local szText		= "宠物生命值，通过宠物强化可直接增强该属性，最多12次"
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

-- 宠物法防按钮
function PetFrame_IceDefValueBtn_OnEnter()
	--[[
	local petInfo = GetShowTipsPetInfo( this:GetParent() );
	if not petInfo then
		return;
	end
	--]]
	local szText = "宠物法术防御，通过宠物强化可直接增强该属性，最多12次";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

-- 宠物物防按钮
function PetFrame_PhyDefValueBtn_OnEnter()
	--[[
	local petInfo = GetShowTipsPetInfo( this:GetParent() );
	if not petInfo then
		return;
	end
	--]]
	local szText = "宠物物理防御，通过宠物强化可直接增强该属性，最多12次";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

-- 宠物命中按钮
function PetFrame_HitValueBtn_OnEnter()
	--[[
	local petInfo = GetShowTipsPetInfo( this:GetParent() );
	if not petInfo then
		return;
	end
	--]]
	local szText = "宠物命中，通过宠物强化可直接增强该属性，最多12次";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

-- 宠物闪避按钮
function PetFrame_FleeValueBtn_OnEnter()
	--[[
	local petInfo = GetShowTipsPetInfo( this:GetParent() );
	if not petInfo then
		return;
	end
	--]]
	local szText = "宠物闪避，通过宠物强化可直接增强该属性，最多12次";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

-- 宠物暴击率按钮
function PetFrame_HeaveyAttackValueBtn_OnEnter()
	local petInfo = GetShowTipsPetInfo( this:GetParent() );
	if not petInfo then
		return;
	end
	local petImpUp = getPetImpUpDef( petInfo.Imp );
	local szText = ""
	if petImpUp == nil then 
		szText = "宠物当前已达到最高成长等级"
	else 
		szText = "宠物当前潜力值："..petInfo.ImpExp.."\n升级所需潜力值："..petImpUp.ExpUp;
	end
	
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

-- 宠物替身率按钮
function PetFrame_TiShenValueBtn_OnEnter()
	local petInfo = GetShowTipsPetInfo( this:GetParent() );
	if not petInfo then
		return;
	end
	local hurt 	= petInfo.FightAttr.YongQi/(30*petInfo.FightAttr.YongQi+200)*100 ;
	local point = hurt - hurt % 0.01;
	local rate 	= math.ceil( petInfo.FightAttr.YongQi/(5*petInfo.FightAttr.YongQi+200)*100 ) 
	local szText = "召唤：主人被怪物攻击，宠物有" ..rate .. "%几率\n发动护卫，减免当次伤害" .. petInfo.FightAttr.DecHurtMul .."%\n\n合体：\n使主人对其他玩家的最终伤害增加"..point.."%\n使其他玩家对主人的最终伤害减少"..point.."%\n（对方合体宠物勇气值越低，效果越好；\n未合体时勇气值为0）"
	ShowGameTooltip( { rich = "WideGameTooltipRichText", tipsFrame = "WideGameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function GetShowTipsPetID( szFrameName )
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();

	if szFrameName == "PetFrame" then
		if t_mainSelPetInfo["id"] == 0 then
			return 0;
		else
			local petInfo = petPackage:getPetInfo( t_mainSelPetInfo["index"] );
			return petInfo.PetID;
		end
	end

	if szFrameName == "WatchPetFrame" then
		local petInfo = petPackage:getWatchPetInfo();
		return petInfo.PetID;
	end

	if szFrameName == "SeePetZiZhiFrame" then
		return GetSeeZiZhiPetID();
	end
end

function GetShowTipsPetInfo( szFrameName )
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();

	if szFrameName == "PetFrame" then
		if t_mainSelPetInfo["id"] == 0 then
			return;
		else
			return petPackage:getPetInfo( t_mainSelPetInfo["index"] );
		end
	end

	if szFrameName == "WatchPetFrame" then
		return petPackage:getWatchPetInfo();
	end

	if szFrameName == "SeePetZiZhiFrame" then
		return GetSeeZiZhiPetInfo();
	end
end

-- 宠物忠诚度按钮
function PetFrame_ZhongChenValueBtn_OnEnter()
	--[[
	local petInfo = GetShowTipsPetInfo( this:GetParent() );
	if not petInfo then
		return;
	end
	--]]
	-- 分宠物查看资质面板，观察面板和宠物主面板来分类讨论
	local szText = "已被召唤的宠物忠诚度会随时间消耗，当宠物的忠诚度少于30%时，将不能被召唤";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

-- 更新技能栏位显示
function UpdateMainPetFrameSkillGrids( szFrameName, petInfo )
	-- 设置宠物已有的技能
	local skillGrids	= petInfo.PetSkills.SkillGrid;
	local nLockedNum	= MAX_LOCK_SKILL_GRID - petInfo.PetSkills.AddSkillGridNum;
	for i = 1, petInfo.PetSkills.SkillGridNum do
		local diathesisDef	= SkillMgr:getDiathesisDef( skillGrids[i-1].SkillID, skillGrids[i-1].SkillLevel );
		local btn			= getglobal( szFrameName.."_SkillButton"..i );
		local tex			= getglobal( btn:GetName().."Icon" );
		local font			= getglobal( btn:GetName().."Level" );
		if diathesisDef ~= nil then
			local IconPath = getPetSkillPath()..diathesisDef.IconID..".tga";
			tex:SetTexture( ( IsInExistence( IconPath ) and IconPath or DEFAULT_ITEM_PATH ) );
			font:SetText("lv"..diathesisDef.Level)
		else
			--tex:SetTexture( t_skillGridTga[skillGrids[i-1].GridType].path );
			tex:SetTexture( t_skillGridTga[PET_SKILL_SELF_BUFF].path );
			font:SetText("")
		end

		tex:SetTexRelUV( 1, 1 );
		btn:SetClientUserData( 0, skillGrids[i-1].SkillID );
		btn:SetClientUserData( 1, skillGrids[i-1].SkillLevel );
		--btn:SetClientUserData( 2, skillGrids[i-1].GridType );
		btn:SetClientUserData( 2, PET_SKILL_SELF_BUFF );
	end

	for i = petInfo.PetSkills.SkillGridNum + 1, MAX_PET_SKILL_GRID do
		local btn		= getglobal( szFrameName.."_SkillButton"..i );
		local tex		= getglobal( btn:GetName().."Icon" );
		local font		= getglobal( btn:GetName().."Level" );
		tex:SetTexture( "uires\\ZhuJieMian\\1.dds" );
		tex:SetTexUV( 749, 517, 44,44 ); 
		font:SetText("")
		btn:SetClientUserData( 2, PET_SKILL_DISABLE );
	end

	local geniusSkillInfo = petInfo.NativeSkill;
	local btn			= getglobal( szFrameName.."GeniusBtn" );
	if geniusSkillInfo.SkillID ~= 0 then
		local diathesisiDef = getDiathesisDef( geniusSkillInfo.SkillID, geniusSkillInfo.SkillLevel );
		local IconPath = getPetSkillPath()..diathesisiDef.IconID..".tga";
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH;
		end
		local t_tgas = { [btn:GetName().."IconTex"] = { path = IconPath }, }
		util.SetTgas( t_tgas );

		btn:SetClientUserData( 0, geniusSkillInfo.SkillID );
		btn:SetClientUserData( 1, geniusSkillInfo.SkillLevel );
		--btn:SetClientUserData( 2, geniusSkillInfo.GridType );
		btn:SetClientUserData( 2, PET_SKILL_SELF_BUFF );
		--btn:Enable();
	else
		local t_textures = { [btn:GetName().."IconTex"] = 
							{ path = "uires\\ZhuJieMian\\2.dds", UV = { x = 182, y = 513, width = 44, height = 44 }, }, };
		util.SetTextures( t_textures );
		btn:SetClientUserData( 0, 0 );
		btn:SetClientUserData( 1, 0 );
		btn:SetClientUserData( 2, PET_SKILL_DISABLE );
		--btn:Disable();
	end
end

function GetPetTraitName( TraitID )
	local petTraitDef = getPetTraitDef( TraitID )
	if petTraitDef == nil then
		return "";
	end

	return petTraitDef.Name;
end

function GetPetTongLingTrumpID( petInfo, actor )
	if petInfo == nil then
		return 0;
	end

	local petPackage	= actor:getPet();
	
	if not petPackage:isPetInState( petInfo.WID, CLIENT_PET_STAT_FIGHT ) then
		return 0;
	end

	local GameTrump = actor:getTrump(); 
	local campFairy	= GameTrump:getCampRoleFairy();
	if campFairy == nil then
		return 0;
	end
	
	local TrumpDef = GameTrump:getFairyDef( campFairy.FairyID, campFairy.FairyLevel );
	local TraitDef = GameTrump:getPetTraitDef( TrumpDef.TraitID,campFairy.StarLvl );

	if TraitDef == nil then
		return 0;
	end

	return campFairy.FairyID;
end

function GetChuZhengPetTongLingTrumpID( pet )
	local GameTrump = actor:getTrump(); 
	local campFairy	= GameTrump:getCampRoleFairy();
	if campFairy == nil then
		return 0;
	end
	
	local TrumpDef = GameTrump:getFairyDef( campFairy.FairyID, campFairy.FairyLevel );
	local TraitDef = GameTrump:getPetTraitDef( TrumpDef.TraitID,campFairy.StarLvl );

	if TraitDef == nil then
		return 0;
	end

	return campFairy.FairyID;
end

function GetChuZhengPetTongLingTrumpID( pet )
	local nOwnerID	= pet:getOwnerId();
	local petOwner	= ActorMgr:FindActor( nOwnerID );
	if petOwner == nil then
		return 0;
	end

	local GameTrump = petOwner:getTrump(); 
	local campFairy	= GameTrump:getCampRoleFairy();
	if campFairy == nil then
		return 0;
	end
	
	local TrumpDef = GameTrump:getFairyDef( campFairy.FairyID, campFairy.FairyLevel );
	local TraitDef = GameTrump:getPetTraitDef( TrumpDef.TraitID,campFairy.StarLvl );

	if TraitDef == nil then
		return 0;
	end

	return campFairy.FairyID;
end

-- 获取宠物的战斗力

function GetPetFightValue( petInfo )
	-- 根据宠物类别来计算宠物的战斗力 
	--[[
	宠物战斗力新公式=（物理攻击*15+hp+物理防御*2.5+魔法防御*2.5+命中等级*4+闪避等级*5）/10
	
	//血量
	iHP = pstPetLevelDef->HP*(pstRolePet->HPGenius*(100+pstRolePet->StrengthenLevel*5)/100.0)/1000.0;
	//物理攻击
	iPhsicAtk = pstPetLevelDef->PhsicAtk*(pstRolePet->PhsicAtkGenius*(100+pstRolePet->StrengthenLevel*5)/100.0)/1000.0;
	//魔法防御
	iIceDef = pstPetLevelDef->IceDef*(pstRolePet->IceDefGenius*(100+pstRolePet->StrengthenLevel*5)/100.0)/1000.0;
	//物理防御
	iDef = pstPetLevelDef->PhsicDef*(pstRolePet->DefGenius*(100+pstRolePet->StrengthenLevel*5)/100.0)/1000.0;
	//闪避
	iFlee = pstPetLevelDef->Flee*(0.8+pstRolePet->FleeGenius*(100+pstRolePet->StrengthenLevel*5)/100.0/5000.0);
	//命中 
	iHitRate = pstPetLevelDef->HitRate*(0.8+pstRolePet->HitRateGenius*

	((iPhsicAtk*15) + iHP + (iIceDef*2.5) + (iDef*2.5) + (iHitRate*4) + (iFlee*5))/10;

	--]]

	local mainplayer			= ActorMgr:getMainPlayer();
	local petPackage			= mainplayer:getPet();
	local petDef				= getPetDef( petInfo.PetID );
	local petLevelDef			= getPetLevelDef( petInfo.Level );
	local strthenCofficiency	= 0;
	for i = 1, petInfo.StrengthenLevel do
		strthenCofficiency = strthenCofficiency + petPackage:getStrthenVal( petInfo, i - 1 );
	end
	
	-- iHP = pstPetLevelDef->HP*(pstRolePet->HPGenius*(100+pstRolePet->StrengthenLevel)/100.0)/1000.0;
	local nHp			= petLevelDef.HP * ( petInfo.HPGenius * ( 100 + strthenCofficiency ) / 100 ) / 1000;
	nHp = math.floor( nHp); 

	-- iPhsicAtk = pstPetLevelDef->PhsicAtk*(pstRolePet->PhsicAtkGenius*(100+pstRolePet->StrengthenLevel)/100.0)/1000.0;
	local nPhysicAttack	= petLevelDef.PhsicAtk * ( petInfo.PhsicAtkGenius * ( 100 + strthenCofficiency ) / 100 ) / 1000;
	nPhysicAttack = math.floor( nPhysicAttack);

	-- iIceDef = pstPetLevelDef->IceDef*(pstRolePet->IceDefGenius*(100+pstRolePet->StrengthenLevel)/100.0)/1000.0;
	local nIce			= petLevelDef.IceDef * ( petInfo.IceDefGenius * ( 100 + strthenCofficiency ) / 100 ) / 1000;
	nIce = math.floor( nIce); 

	-- iDef = pstPetLevelDef->PhsicDef*(pstRolePet->DefGenius*(100+pstRolePet->StrengthenLevel)/100.0)/1000.0;
	local nPhy			= petLevelDef.PhsicDef * ( petInfo.DefGenius * ( 100 + strthenCofficiency ) / 100 ) / 1000;
	nPhy = math.floor( nPhy); 

	-- iFlee = pstPetLevelDef->Flee*(0.8+pstRolePet->FleeGenius*(100+pstRolePet->StrengthenLevel)/100.0/5000.0);
	local nFlee			= petLevelDef.Flee * ( 0.8 + petInfo.FleeGenius * ( 100 + strthenCofficiency ) / 100 / 5000 );
	nFlee = math.floor( nFlee); 

	-- iHitRate = pstPetLevelDef->HitRate*(0.8+pstRolePet->HitRateGenius*
	local nHitRate		= petLevelDef.HitRate * ( 0.8 + petInfo.HitRateGenius * ( 100 + strthenCofficiency ) / 100 / 5000 );
	nHitRate = math.floor( nHitRate); 

	return math.floor( ( nPhysicAttack * 15 + nHp + nIce * 2.5 + nPhy * 2.5 + nHitRate * 4 + nFlee * 5 ) / 10 );
end

function GetAttackGeniusValue( petInfo )
	local nAttackGenius	= petInfo.PhsicAtkGenius;
	if nAttackGenius == 0 then
		nAttackGenius = petInfo.IceAtkGenius;
	end

	return nAttackGenius;
end

function ClearPetFrameStars( szFrameName )
	for i = 1, CLIENT_MAX_PET_STRENGTHEN_LEVEL do
		local btn = getglobal( szFrameName.."_QiangHuaSmallStarBtn"..i );
		btn:Hide();
	end
end

function UpdatePetFrameStars( petInfo, szFrameName )
	ClearPetFrameStars( szFrameName );
	
	local mainplayer			= ActorMgr:getMainPlayer();
	local petPackage			= mainplayer:getPet();
	if petInfo == nil then
		return;
	end

	local t_petStrengthenControl = GetPetStrengthenControl();
	for i = 1, petInfo.StrengthenLevel do
		local btn				= getglobal( szFrameName.."_QiangHuaSmallStarBtn"..i );
		local tex				= getglobal( btn:GetName().."Tex" );
		local nIncrAttr			= petPackage:getStrthenVal( petInfo, i - 1 );
		local t_oneTexConfig	= t_petStrengthenControl:getStarTexConfig( { value = nIncrAttr } );
		tex:SetTexture( t_oneTexConfig["path"] );
		local UV				= t_oneTexConfig["UV"];
		tex:SetTexUV( UV["x"], UV["y"], UV["width"], UV["height"] );
		btn:Show();
	end

	for i = petInfo.StrengthenLevel + 1, MAX_STRENGTH_LEVEL do
		local btn				= getglobal( szFrameName.."_QiangHuaSmallStarBtn"..i );
		local tex				= getglobal( btn:GetName().."Tex" );
		local t_oneTexConfig	= t_petStrengthenControl:getStarTexConfig( { value = nil } );
		tex:SetTexture( t_oneTexConfig["path"] );
		local UV				= t_oneTexConfig["UV"];
		tex:SetTexUV( UV["x"], UV["y"], UV["width"], UV["height"] );
		btn:Show();
	end
end

function UpdatePetFrameFairyBtn()
	local player	= ActorMgr:getMainPlayer()
	local trump		= player:getTrump()
	local fairy		= trump:getCampRoleFairy()
	if fairy then
		local TrumpDef = trump:getFairyDef(fairy.FairyID, fairy.FairyLevel)
		local IconPath = GetItemEquipIconPath()..TrumpDef.ModeID..".tga"
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH;
		end
		PetFrame_FairyBtnIcon:SetTexture(IconPath)
	else
		PetFrame_FairyBtnIcon:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga")
	end
end

function ShowPetInfo( petInfo, szFrameName, bResetModelView, bNotResetName )
	local mainplayer	= ActorMgr:getMainPlayer();

	local petPackage	= mainplayer:getPet();
	local petDef		= getPetDef( petInfo.PetID );
	if petDef == nil then
		return;
	end

	local fScale				= 0.8;	
	local szStrengthenLevelDesc = "";
	local szNickName			= petDef.Name;
	local szStrengthenLevelDesc = petInfo.StrengthenLevel == 0 and "" or ( " ( +"..( petInfo.StrengthenLevel * 2 ) .. "% )" );
	local fExtraStrengthen = petInfo.StrengthenLevel * 5 / 100;
	local szStrengthenAttrDesc	= petInfo.StrengthenLevel == 0 and "" or ( " ( +"..( petInfo.StrengthenLevel * 1 ) .. "% )" );
	local petLevelDef = getPetLevelDef( petInfo.Level );
	local petSkillDef = SkillMgr:getSpellStaticAtt( petDef.NormalSkill, 1 );
	local t_attackTypeValue =	{ [SKILL_ATTR_PHSIC] = { value = petInfo.FightAttr.PhsicAtk }, 
								[SKILL_ATTR_ICE] = { value = petInfo.FightAttr.IceAtk } 
								};
	-- 宠物总成长次数
	local nTotalGrowthValue	= petInfo.PhsicAtkGenius + petInfo.IceAtkGenius + petInfo.HPGenius +																				petInfo.DefGenius + petInfo.HitRateGenius + petInfo.FleeGenius + petInfo.IceDefGenius;
	nTotalGrowthValue = math.floor( nTotalGrowthValue * ( fScale + fExtraStrengthen ) );

	local nAttackGenius	= petInfo.PhsicAtkGenius;
	if nAttackGenius == 0 then
		nAttackGenius = petInfo.IceAtkGenius;
	end
	local fFightScale = ( 1 + 0.05 ) ^ ( petInfo.Level - 1 );
	-- 根据宠物类别来计算宠物的战斗力 
	local t_petTypeFightValue = {
								-- 血宠
								[0] = { value = ( nTotalGrowthValue + ( petInfo.HPGenius + petInfo.DefGenius ) / 2 ) * fFightScale }, 
								-- 防宠
								[1] = { value = ( nTotalGrowthValue + petInfo.DefGenius ) * fFightScale }, 
								-- 攻宠
								[2] = { value = ( nTotalGrowthValue + nAttackGenius ) * fFightScale }, 
								};
	
	-- 潜力值
	local ImpUpDef		= getPetImpUpDef( petInfo.Imp );
	local ImpPercent 	= 0
	local percentVal 	= 0;
	if ImpUpDef ~= nil then 
		percentVal = petInfo.ImpExp / ImpUpDef.ExpUp * 10000
	end
	if percentVal < 1 then 
		ImpPercent = math.ceil( percentVal )	
	else 
		ImpPercent = math.floor( percentVal )
	end
	local t_fonts = {
						-- 宠物名字
						[szFrameName.."PetNameBtnFont"]			=	{ value = petDef.Name },
						-- 宠物等级
						[szFrameName.."PetLvValueBtnFont"]		=	{ value = petInfo.Level.."级" },
						-- 宠物资质级别
						[szFrameName.."PetGeniusValuBtnFont"]	=	{ value = t_geniusDesc[petInfo.PetGenius]["value"], 
																		color = t_geniusDesc[petInfo.PetGenius]["color"] },
						-- 宠物HP
						[szFrameName.."HPBtnHPValueFont"]			=	{ value = petInfo.HP .. "/" .. petInfo.MaxHP },
						-- 宠物EXP
						[szFrameName.."EXPBtnExpValueFont"]			=	{ value = petInfo.Exp .. "/" .. petLevelDef.LevelUpExp },
						
						-- TODO: 成长值
						-- 宠物攻击成长值		petInfo.PhsicAtkGenius
						[szFrameName.."AttackGrowthValueBtnFont"]	=	{ value = nAttackGenius },
						-- 宠物生命成长值
						[szFrameName.."LifeGrowthValueBtnFont"]		=	{ value = petInfo.HPGenius },
				
						-- 宠物物防成长值
						[szFrameName.."PhysicDefGrowthValueBtnFont"]=	{ value = petInfo.DefGenius },

						-- 宠物魔防成长值
						[szFrameName.."IceDefGrowthValueBtnFont"]	=	{ value = petInfo.IceDefGenius },

						-- 宠物命中成长值
						[szFrameName.."HitGrowthValueBtnFont"]		=	{ value = petInfo.HitRateGenius },

						-- 宠物闪避成长值
						[szFrameName.."FleeGrowthValueBtnFont"]		=	{ value = petInfo.FleeGenius },
						-- TODO: 成长值
						
						-- 宠物攻击值
						[szFrameName.."TotalAttackValueBtnFont"]	= {
						value = petSkillDef ~= nil and ( t_attackTypeValue[SKILL_ATTR_PHSIC].value ) or "" },-- petSkillDef.MainAttr
						-- 生命值 
						[szFrameName.."MaxLifeValueBtnFont"]	= { value = petInfo.MaxHP },
						-- 宠物魔法防御值 
						[szFrameName.."IceDefValueBtnFont"]			=	
						{ value = math.ceil( petInfo.FightAttr.IceDef ) },
						-- 宠物物理防御值
						[szFrameName.."PhyDefValueBtnFont"]				=	{ value = math.ceil( petInfo.FightAttr.PhsicDef ) },
						-- 宠物命中等级
						[szFrameName.."HitValueBtnFont"]				=	
						{ value = math.ceil( petInfo.FightAttr.HitRate ) },
						-- 宠物闪避等级
						[szFrameName.."FleeValueBtnFont"]				=	
						{ value =  ( petInfo.FightAttr.Flee ) },
						
						-- 宠物潜力值
						[szFrameName.."HeaveyAttackValueBtnFont"]		=	{ value = ( ImpPercent / 100 .. "%" ) },
						-- 宠物替身值
						[szFrameName.."TiShenValueBtnFont"]				=	{ value = petInfo.FightAttr.YongQi },
						-- 宠物忠诚度值	value = math.floor( petInfo.LoveVal / 100 )
						[szFrameName.."ZhongChenValueBtnFont"]			=	{ value = math.floor( petInfo.LoveVal / 100 ).."%" },
						-- 宠物战斗力
						[szFrameName.."FightValueBtnFont"]				=	{ value = GetPetFightValue( petInfo ) },
						-- 宠物召唤等级
						[szFrameName.."CallLvValueBtnFont"]				=	{ value = petDef.FightLevel },
						-- 昵称名字
						[szFrameName.."_NickNameBtnFont"]				=	{ value = petInfo.Name ~= "" and petInfo.Name or petDef.Name }
					};
	for name, data in pairs( t_fonts ) do
		local font = getglobal( name );
		if font ~= nil then
			font:SetText( data.value );
		end

		if font ~= nil and data["color"] ~= nil then
			font:SetTextColor( data["color"]["r"], data["color"]["g"], data["color"]["b"] );
		end
	end
	-- 战宠标记
	local tex 		= getglobal("PetFrame_HetiBtnIcon")
	local level 	= SkillMgr:getSpellLv(PET_HETI_SKILL_ID)
	local SkillDef	= SkillMgr:getSpellStaticAtt(PET_HETI_SKILL_ID, level == 0 and 1 or level);
	local IconPath  = DEFAULT_ITEM_PATH;
	local battleBtn = getglobal( szFrameName.."BattlePetBtn" );
	local flagTex 	= getglobal( szFrameName.."BackGroundButtonTex" )
	local flagBtn	= getglobal( szFrameName.."TipAreaBtn" )
	local petName 	= getglobal( szFrameName.."PetNameBtnFont" );
	if petPackage:isExtraBattlePet(petInfo) then 
		battleBtn:SetPoint( "topleft", petName:GetName(), "topleft", petName:GetTextExtentWidth( petName:GetText() ) + 3 * GetScreenScaleY(), 
										0 );
		battleBtn:Show();
		flagTex:Show();
		flagBtn:Show();
		if szFrameName == "PetFrame" then
			IconPath = GetSkillIconPath()..SkillDef.IconID2..".tga"
		end
	else 
		if szFrameName == "PetFrame" then
			IconPath = GetSkillIconPath()..SkillDef.IconID1..".tga"
		end
		battleBtn:Hide();
		flagTex:Hide();
		flagBtn:Hide();
	end
	if not IsInExistence(IconPath) then
		IconPath = DEFAULT_ITEM_PATH
	end
	tex:SetTexture(IconPath)
	
	UpdatePetFrameStars( petInfo, szFrameName );


	local t_textures =	{ 
							[szFrameName.."PetAtkTypeBtnTex"]	= t_petTypeTextures[petDef.PetAttrType],
							[szFrameName.."DaiShuBtnTex"]		= t_petDaiShu[petDef.Type][petInfo.Hierarchy],
							[szFrameName.."LoveLevelBtnTex"]	= t_petLoveLevelTextures[petInfo.LoveLevel],
						};
	for name, data in pairs( t_textures ) do
		local tex = getglobal( name );
		if tex ~= nil then
			if data.path ~= nil then
				tex:SetTexture( data.path );
			end
			tex:SetTexUV( data.UV.x, data.UV.y, data.UV.width, data.UV.height );
			tex:Show();
		end
	end

	UpdateMainPetFrameSkillGrids( szFrameName, petInfo );
	
	local t_ModelPos = { [214] = -20,[1214] = -20 }

	if bResetModelView then
		local t_modelViewers = { [szFrameName.."ModelBtnModelView"] = { path = GetModePathById( petDef.MouldID ), animId = 10100, 
							 scale = GetNpcDisplayInfo( petDef.MouldID ).size == 0 and 1 or GetNpcDisplayInfo( petDef.MouldID ).size } };
		for name, data in pairs( t_modelViewers ) do
			local modelViewer = getglobal( name );
			if modelViewer ~= nil then
				modelViewer:setPetModel( 0, data.animId, data.path );
				modelViewer:setPetScale( 0, data.scale, data.scale, data.scale );
				
				if t_ModelPos[petInfo.PetID] ~= nil then
					modelViewer:setEntityPosition( 0, 0, t_ModelPos[petInfo.PetID], 0 );
				else
					modelViewer:setEntityPosition( 0, 0, 40, 0 );
				end
				modelViewer:Show();
			end
		end
	end
	
	if not  bNotResetName then
		local t_editBoxes = 
						{
						[szFrameName.."_NickNameEdit"]	=	{ value = petInfo.Name ~= "" and petInfo.Name or petDef.Name }
						};
		for name, data in pairs( t_editBoxes ) do
			local edit = getglobal( name );
			if edit ~= nil then
				edit:SetText( data.value );
			end
		end
	end
	
	local range = petPackage:getPetGrowResetRange(petInfo)
	local min, max = range.Min, range.Max
	local t_regionButtons = {
		[szFrameName.."HPBtn"]					= { scale = math.min( 1, petInfo.HP / petInfo.MaxHP ) },

		[szFrameName.."EXPBtn"]					= { scale = math.min( 1, petInfo.Exp / petLevelDef.LevelUpExp )  },

		[szFrameName.."AttackGrowthValueBtn"]	= {
			scale = math.max( 0, math.min( 1, ( nAttackGenius - min.Attack ) / (max.Attack - min.Attack) ) ),
			limit = "PurpleBarTexTemplate", normal = "WaterBlueBarTexTemplate",
		},

		[szFrameName.."LifeGrowthValueBtn"]		= {
			scale =  math.max( 0, math.min( 1, ( petInfo.HPGenius - min.HP ) / (max.HP - min.HP) ) ),
			limit = "PurpleBarTexTemplate", normal = "WaterBlueBarTexTemplate",
		},

		[szFrameName.."PhysicDefGrowthValueBtn"]	= {
			scale = math.max( 0, math.min( 1, ( petInfo.DefGenius - min.Defend ) / (max.Defend - min.Defend) ) ),
			limit = "PurpleBarTexTemplate", normal = "WaterBlueBarTexTemplate",
		},

		[szFrameName.."IceDefGrowthValueBtn"]	= {
			scale = math.max( 0, math.min( 1, ( petInfo.IceDefGenius - min.Resist ) / (max.Resist - min.Resist) ) ),
			limit = "PurpleBarTexTemplate", normal = "WaterBlueBarTexTemplate",
		},

		[szFrameName.."HitGrowthValueBtn"]	= {
			scale = math.max( 0, math.min( 1, ( petInfo.HitRateGenius - min.Exact ) / (max.Exact - min.Exact) ) ),
			limit = "PurpleBarTexTemplate", normal = "WaterBlueBarTexTemplate",
		},

		[szFrameName.."FleeGrowthValueBtn"]	= {
			scale = math.max( 0, math.min( 1, ( petInfo.FleeGenius - min.Dodge ) / (max.Dodge - min.Dodge) ) ),
			limit = "PurpleBarTexTemplate", normal = "WaterBlueBarTexTemplate",
		},
	};
	for name, data in pairs ( t_regionButtons ) do
		local btn = getglobal( name );
		if btn ~= nil then
			if data.scale == 1 then
				if data.limit then
					btn:SetNormalTextureTemplate(data.limit)
				end
			else
				if data.normal then
					btn:SetNormalTextureTemplate(data.normal)
				end
			end
			btn:SetNormalRegionWidth( data.scale );
			btn:Show();
		end
	end
	
	if szFrameName ~= "PetFrame" then
		return
	end

	local t_changeShowButtons = 
								{
								[true] = { func = function ()
														PetFrame_FightBtn:Hide();
														PetFrame_BackUpBtn:Show();
														PetFrame_FreedomBtn:Disable();
													end
											},
								[false] = { func = function ()
														PetFrame_FightBtn:Show();
														PetFrame_BackUpBtn:Hide();
														PetFrame_FreedomBtn:Enable();
													end
											},
								};	
	t_changeShowButtons[ petPackage:isPetInState( t_mainSelPetInfo["index"], CLIENT_PET_STAT_FIGHT )].func();
	
	if PetRongHeProcessFrame:IsShown() then	
		UpdateCallBackBtn();
	end
	
	local MainPlayer = ActorMgr:getMainPlayer()
	if petInfo.Level >= PEIT_HETI_MIN_LEVEL and MainPlayer:getLv() >= PEIT_HETI_MIN_LEVEL then
		PetFrame_HetiBtnIcon:SetGray(false)
		if not GameWizard:isDo(GAMEWIZARD_FIRST_USE_PET_HETI) then
			PetFrame_HetiBtnUVAnimationTex:SetUVAnimation(80, true)
			PetFrame_HetiBtnUVAnimationTex:Show()
		end
	else
		PetFrame_HetiBtnIcon:SetGray(true)
		PetFrame_HetiBtnUVAnimationTex:Hide()
	end

	if PetAddLoveLevelMessageFrame:IsShown() then
		if PetAddLoveLevelMessageFrame:GetClientUserData( 0 ) ~= petInfo.WID.nHeight 
			and PetAddLoveLevelMessageFrame:GetClientUserData( 1 ) ~= petInfo.WID.nLow then
			PetAddLoveLevelMessageFrame:Hide()
		end
	end
end

--[[
-- 属性变化消息
PET_CHG_NAME		= 0;
PET_CHG_EXP			= 1;
PET_CHG_HP			= 2; 
PET_CHG_MAXHP		= 3; 
PET_CHG_LEVEL		= 4; 
PET_CHG_LOVE_VAL	= 5; 
PET_CHG_BREEDNUM	= 6; 
PET_CHG_STAT		= 7; 

local t_attrUpdate = 
					{
					[PET_CHG_NAME]	= {},
					};
function PetUIUpdateAttr( nAttrID )
end
--]]

function PetActorLevelUp( nPetId, nPetLv )
	--local petActor = ActorMgr:FindActor( nPetId );
	local MainPlayer	= ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end

	if ( nPetLv - MainPlayer:getLv() ) >= 10 then
		ShowMidTips( "宠物等级过高，被召唤时不能获得经验值" );
	end

	PetLevelUp(nPetLv);
end

local t_petFrameEvents =	{	["GE_PET_UPDATE"]			= {}, ["GE_DEL_PET"]		= {}, ["GE_ACTOR_FIGHT_IN"] = {},
								["GE_PET_INFO_CHG"]			= {}, ["GE_PET_ATTR_CHG"]	= {}, ["GE_PET_FIGHT_CHG"]	= {},
								["GE_PET_STATE_CHANGE"]		= {}, ["GE_PET_GRID_CHG"]	= {}, ["GE_PET_EXP_CHANGE"] = {},
								["GE_TRUMP_ATTRUPDATE"]		= {},
							};

t_petFrameEvents["GE_PET_STATE_CHANGE"].func = 
function ()
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local petInfo;
	for i = 1, petPackage:getPetNum() do
		 petInfo = petPackage:getPetInfo( i - 1 );
		 if petPackage:isPetInState( i - 1, CLIENT_PET_STAT_FIGHT ) and 
			( petInfo.Level - mainplayer:getLv() == PET_LEVEL_LIMIT ) then
			ShowMidTips( "宠物等级过高，被召唤时不能获得经验值" );
			return;
		 end
	end
end

t_petFrameEvents["GE_PET_EXP_CHANGE"].func = 
function ()
	if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_CONTAINER_ADD_PET_EXP_ITEM_BTN ) then
		local effectFrame = getglobal( "PlayerContainerFrameEffectFrame" );
		effectFrame:Hide();
		GameWizard:doStep( GAMEWIZARD_FIRST_BLINK_CONTAINER_ADD_PET_EXP_ITEM_BTN );
	end
end

t_petFrameEvents["GE_PET_UPDATE"].func = 
function ()
	UpdatePeFrame_PetList();
end

t_petFrameEvents["GE_DEL_PET"].func = 
function ()
	ClearPetFrame();
end

t_petFrameEvents["GE_ACTOR_FIGHT_IN"].func = 
function ()
	--[[
	local mainplayer = ActorMgr:getMainPlayer();
	local petPackage = mainplayer:getPet();
	if petPackage:isPre() then
		petPackage:petPreBreak( t_mainSelPetInfo["heightId"], t_mainSelPetInfo["lowId"] );
	end
	--]]
end

t_petFrameEvents["GE_PET_GRID_CHG"].func = 
function ()
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local t_fonts = {
						["PetFrameListBackGroundNumTitleFont"] = { value = "宠物列表" .. petPackage:getPetNum().."/"..petPackage:getPetMaxNum() },
					};
	for name, data in pairs( t_fonts ) do
		local font = getglobal( name );
		font:SetText( data.value );
	end
	ShowMidTips( "扩充宠物栏位成功" );
end

t_petFrameEvents["GE_PET_INFO_CHG"].func = 
function ()
	if t_mainSelPetInfo.index ~= -1 then
		local mainplayer = ActorMgr:getMainPlayer();
		local petPackage = mainplayer:getPet();
		if petPackage:getPetNum() ~= 0 then
			UpdatePeFrame_PetList();
			local petInfo = petPackage:getPetInfo( t_mainSelPetInfo.index );
			ShowPetInfo( petInfo, "PetFrame", false, true );
		end
	end
end

t_petFrameEvents["GE_TRUMP_ATTRUPDATE"].func = 
function ()
	if PetFrame:IsShown() then
		--UpdatePetFrameFairyBtn()
	end
end

function PetChgName( nHeightId, nLowId )
	if t_mainSelPetInfo.index == -1 then
		return;
	end
	
	local mainplayer = ActorMgr:getMainPlayer();
	local petPackage = mainplayer:getPet();
	if petPackage:getPetNum() == 0 then
		return;
	end

	local petInfo = petPackage:getPetInfo( t_mainSelPetInfo.index );
	if petInfo.WID.nHeight ~= nHeightId or petInfo.WID.nLow ~= nLowId then
		return;
	end

	local t_editBoxes = 
						{
							["PetFrame_NickNameEdit"]	=	{ value = petInfo.Name ~= "" and petInfo.Name or petDef.Name }
						};
	for name, data in pairs( t_editBoxes ) do
		local edit = getglobal( name );
		edit:SetText( data.value );
	end
end

t_petFrameEvents["GE_PET_ATTR_CHG"].func = 
function ()
	t_petFrameEvents["GE_PET_INFO_CHG"].func();
end

t_petFrameEvents["GE_PET_FIGHT_CHG"].func = 
function ()
	t_petFrameEvents["GE_PET_INFO_CHG"].func();
end

function PetFrame_Test_OnShow()
	local animationTexture = getglobal( "PetFrame_TameBtnUVAnimationTex" );
	animationTexture:SetUVAnimation( 50, true );
	animationTexture:Show();
	local animationTexture = getglobal( "PetFrame_FightBtnUVAnimationTex" );
	animationTexture:SetUVAnimation( 50, true );
	animationTexture:Show();

	local animationTexture = getglobal( "PetFrameAutoWeiYangBtnUVAnimationTex" );
	animationTexture:SetUVAnimation( 50, true );
	animationTexture:Show();

	PetFrame_CatchPetDesc:SetLinkTextColor( 0, 255, 0 );
	PetFrame_CatchPetDesc:SetText( "宠物捕捉：彩虹岛  通过#L莉莉[时间城]#n传送", 255, 255, 190 );
	PetFrame_TrainPetDesc:SetLinkTextColor( 0, 255, 0 );
	PetFrame_TrainPetDesc:SetText( "宠物训练师：#L拉奥多[时间城]#n", 255, 255, 190 );
end

function PetFrame_OnLoad()
	t_petFrameEvents.__index = function ( tab, key )
			tab[key] = { func = function () end };
			return tab[key];
	end
	setmetatable( t_petFrameEvents, t_petFrameEvents );

	for event, _ in pairs( t_petFrameEvents ) do
		this:RegisterEvent( event );
	end

	for i = 1, MAX_PET_ONE_PAGE do
		local btn = getglobal( "PetFrame_ListButton" ..i );
		btn:SetOverlayRegion( 1, -5, 106, 20 );
	end
	t_buttonEvent.__index = function ( tab, key )
								tab[key] = {};
								return tab[key];
							end
	setmetatable( t_buttonEvent, t_buttonEvent );

	local t_buttonEventTable =	{
								--  向上翻一页按钮
								["PetFrame_PrePageBtn"]		= PrePageBtn_OnClick,
								--  向下翻一页按钮
								["PetFrame_NextPageBtn"]	= NextPageBtn_OnClick,
								-- 出征按钮
								["PetFrame_FightBtn"]		= PetFrameFightBtn_OnClick,	
								-- 召回按钮
								["PetFrame_BackUpBtn"]		= PetFrameBackUpBtn_OnClick,
								-- 驯服按钮
								["PetFrame_TameBtn"]		= PetFrameTameBtn_OnClick,
								-- 放生按钮
								["PetFrame_FreedomBtn"]		= PetFrameFreedomBtn_OnClick,
								-- 关闭按钮
								["PetFrame_CloseBtn"]		= PetFrame_CloseBtn_OnClick,
							};

	for name, event in pairs( t_buttonEventTable ) do
		t_buttonEvent[name]["click"] = event;
	end

	for i = 1, MAX_PET_ONE_PAGE do
		t_buttonEvent["PetFrame_ListButton"..i]["click"] = PetListButton_OnClick;
	end

	PetFrame_ArmButton1IconTexture:SetTexUV( 220, 711, 44, 44 );
	PetFrame_ArmButton2IconTexture:SetTexUV( 175, 711, 44, 44 );
	PetFrame_ArmButton3IconTexture:SetTexUV( 265, 711, 44, 44 );
end

function PetFrame_OnEvent()
	t_petFrameEvents[arg1].func();
end


-- doModal处理函数
function PetFrameButtonsClick()
	local petXinShouBtn = getglobal( "PetFrameXinShouBtn" );
	if petXinShouBtn:IsShown() and petXinShouBtn:GetClientString() == this:GetName() and this:GetName() ~= "PetFrame_TameBtn" then
		local t_hideIcons = { this:GetName().."UVAnimationTex", "PetFrameXinShouBtn" };
		util.HideIcons( t_hideIcons );
	end

	-- 模拟doModal,在由于游戏面板弹出的二级面板显示的情况下，不允许对一级面板再进行任何操作
	for _, szFrameName in ipairs( t_modalFrame.modalFrames ) do
		local frame = getglobal( szFrameName );
		if frame:IsShown() then
			return;
		end
	end
	t_buttonEvent[this:GetName()]["click"]();
end

local t_petNpc =	{ 
						["拉奥多"]	= { x = 28700, y = 26118, mapId = 1000, npcId = 304006 }, 
						["莉莉"]	= { x = 29100, y = 27000, mapId = 1000, npcId = 317005 }, 
					};
-- 宠物训练师名字
function PetFrame_TrainPetDesc_OnClick()
	if ( arg3 == "LeftButton" ) then
		--MapShower:GetMaxMapContal():linkMoveto( t_petNpc["mapId"], t_petNpc["x"], t_petNpc["y"] );	
		local t_destNpcInfo = t_petNpc["拉奥多"];
		MapShower:GetMaxMapContal():linkMoveto( t_destNpcInfo["mapId"], t_destNpcInfo["npcId"] );	
	end
end

-- 捕获宠物
function PetFrame_CatchPetDesc_OnClick()
	if ( arg3 == "LeftButton" ) then
		--MapShower:GetMaxMapContal():linkMoveto( t_petNpc["mapId"], t_petNpc["x"], t_petNpc["y"] );	
		local t_destNpcInfo = t_petNpc["莉莉"];
		MapShower:GetMaxMapContal():linkMoveto( t_destNpcInfo["mapId"], t_destNpcInfo["npcId"] );	
	end
end

function PetFrame_XinShouBtn_CloseBtn_OnClick()
	local petXinShouBtn = getglobal( "PetFrameXinShouBtn" );
	if petXinShouBtn:GetClientString() == "PetFrame_TameBtn" and not GameWizard:isDo(GAMEWIZARD_FIRST_CLOSE_PET_ZHONG_CHENG_BTN_TIPS) then
		GameWizard:doStep( GAMEWIZARD_FIRST_CLOSE_PET_ZHONG_CHENG_BTN_TIPS );
	end

	local xinShouBtn = getglobal( this:GetParent() );
	xinShouBtn:Hide();
end

function GetNewPet( nHeightId, nLowId )
	local mainplayer	= ActorMgr:getMainPlayer();
	-- 判断宠物的状态
	local petPackage	= mainplayer:getPet();
	local petInfo		= petPackage:getPetInfo( nHeightId, nLowId );
	t_mainSelPetInfo = { heightId = petInfo.WID.nHeight, lowId = petInfo.WID.nLow, id = petInfo.PetID, 
							index = petPackage:getPetIndex( petInfo ), getNew = true };
	UpdatePetPhotoWhenGetNewPet();
	if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_PET_EGG_CONTAINER_BTN ) then
		GameWizard:doStep( GAMEWIZARD_FIRST_BLINK_PET_EGG_CONTAINER_BTN );
	end

	if not GameWizard:isDo(GAMEWIZARD_FIRST_GET_PET_BLINK_PET_SHORTCUT_BTN) then
		GuideEffectShow( "", "点击打开宠物界面", "PetButton", "你获得一只宠物，点击打开宠物界面可以查看\n快捷键（X）", "uires\\ZhuJieMian\\1.dds", 352, 609, 60, 60, 50, 60, -9, 5, 7, -58, true, FIRST_GET_WEAPON );
		GameWizard:doStep( GAMEWIZARD_FIRST_GET_PET_BLINK_PET_SHORTCUT_BTN );
		return;
	end

	if PetFrame:IsShown() then
		UpdatePetFrame();	
		return;
	end

	PetFrame:Show();
end

function UpdatePetFrame()	
	local mainplayer = ActorMgr:getMainPlayer();
	local petPackage = mainplayer:getPet();
	local pageFont	= getglobal( "PetFrameListBackGroundPageFont" ); 
	ClearPetInfo( "PetFrame" );
	-- 若有宠物，默认选中第一个
	local nChkIndex = 0;
	-- TODO: 加个参数判定是否是由于特殊需求而导致的强制默认显示
	if not t_mainSelPetInfo["getNew"] then
		-- 优先显示处于出征的宠物
		for i = 1, petPackage:getPetNum() do
			if petPackage:isPetInState( i - 1, CLIENT_PET_STAT_FIGHT ) then
				nChkIndex = i;
				break;
			end
		end
	end
	
	-- 然后再显示上次被选中的宠物
	if nChkIndex == 0 and t_mainSelPetInfo["id"] ~= 0 then
		nChkIndex = t_mainSelPetInfo["index"] + 1;
	end
	
	if Pet_DevelopmentBuildUpFrame:IsShown() or PetStepUpFrame:IsShown() then
		local t_operationSelPetInfo = GetOperationSelPetInfo();
		t_mainSelPetInfo	= t_operationSelPetInfo;
		nChkIndex		= t_mainSelPetInfo["index"] + 1;
	end
	-- 最后默认选中第一只宠物
	if nChkIndex == 0 then
		--nChkIndex = 1;
		-- TODO
		local systemSetting		= GameMgr:getUserSystemConfig();
		local nLastSelectPetIndex	= systemSetting:getGameOptValue( GAME_OPT_LAST_PET_FIGHT ) - 1;
		if nLastSelectPetIndex < 0 or nLastSelectPetIndex >= petPackage:getPetNum() then
			nLastSelectPetIndex = 0;
		end
		nChkIndex = nLastSelectPetIndex + 1;
	end

	local nPetIndex		= nChkIndex - 1;
	local petInfo		= petPackage:getPetInfo( nPetIndex );	
	t_mainSelPetInfo	= { heightId = petInfo.WID.nHeight, lowId = petInfo.WID.nLow, id = petInfo.PetID, index = nPetIndex };
	pageFont:SetText( math.ceil( nChkIndex / MAX_PET_ONE_PAGE ) );
	UpdatePeFrame_PetList();
	
	ShowPetInfo( petInfo, "PetFrame", true );

	-- 显示宠物名字按钮
	local btn = getglobal( "PetFrame_NickNameBtn" );
	btn:Show();
	-- 隐藏宠物名字编辑框
	local edit = getglobal( "PetFrame_NickNameEdit" );
	edit:Hide();
end

function ClearPetFrameWithoutPets()
	t_mainSelPetInfo = { heightId = 0, lowId = 0, id = 0, index = -1 };
	ClearPetInfo( "PetFrame" );
	ClearPetList();

	local mainplayer = ActorMgr:getMainPlayer();
	local petPackage = mainplayer:getPet();
	local t_fonts = {
						["PetFrameListBackGroundNumTitleFont"]	= { value = "宠物列表" .. petPackage:getPetNum().."/"..petPackage:getPetMaxNum() },
						["PetFrameListBackGroundPageFont"]		= { value = 0 },
						["PetFrameHPBtnHPValueFont"]			= { value = "" },
						["PetFrameEXPBtnExpValueFont"]			= { value = "" },
					};
				
	 util.SetFonts( t_fonts );
	
	local t_regionButtons = 
	{	["PetFrameHPBtn"]					= { scale = 0 },	
		["PetFrameEXPBtn"]					= { scale = 0 }, 
		["PetFrameAttackGrowthValueBtn"]	= { scale = 0 },
		["PetFrameLifeGrowthValueBtn"]		= { scale = 0 },
		["PetFramePhysicDefGrowthValueBtn"]	= { scale = 0 },
		["PetFrameIceDefGrowthValueBtn"]	= { scale = 0 },
		["PetFrameHitGrowthValueBtn"]		= { scale = 0 },
		["PetFrameFleeGrowthValueBtn"]		= { scale = 0 },
	};
	for name, data in pairs ( t_regionButtons ) do
		local btn = getglobal( name );
		if btn ~= nil then
			btn:SetNormalRegionWidth( data.scale );
			btn:Show();
		end
	end

end

local t_xinShouBtnPosInfo = {	["PetFrame_FightBtn"] = { selfPoint = "BOTTOMLEFT", relFrame = "PetFrame_FightBtn", relPoint = "TOPLEFT", 
															offsetX = 15, offsetY = -17, },
							
								["PetFrame_TameBtn"] = { selfPoint = "BOTTOMLEFT", relFrame = "PetFrame_TameBtn", relPoint = "TOPLEFT", 
															offsetX = 15, offsetY = -17, },
							};
function CheckPetXinShouGuid()
	-- 若第一次获得宠物
	local xinShouBtn = getglobal( "PetFrameXinShouBtn" );
	if not GameWizard:isDo(GAMEWIZARD_FIRST_GETPETACTOR) then
		--xinShouBtn:SetPoint( "BOTTOMLEFT", "PetFrame_FightBtn", "TOPLEFT", 30, -50 );
		GameWizard:doStep(GAMEWIZARD_FIRST_GETPETACTOR);
		ShowPetFrameXinShouBtn( { text = "点击这里，就可以召唤出宠物和您并肩作战", btn = "PetFrame_FightBtn" } );
		--将宠物自动吃药的设置打开
		for i = 0, 2 do
			GameUserConfig:setOnePetConfig( i, true, 0, 0 );
		end
	end
	
	local mainplayer = ActorMgr:getMainPlayer();
	local petPackage = mainplayer:getPet();
	local bPetZhongChentLow =false;
	for i = 1, petPackage:getPetNum() do
		local pet = petPackage:getPetInfo( i - 1 );
		if pet.LoveVal / 100 <= LOW_ZHOGN_CHNEG then
			bPetZhongChentLow = true;
			break;
		end
	end

	if bPetZhongChentLow and GameWizard:isDo(GAMEWIZARD_FIRST_BLINK_PET_BTN_WHEN_LOW) and 
			not GameWizard:isDo(GAMEWIZARD_FIRST_CLICK_PET_ZHONG_CHENG_BTN_WHEN_LOW) then
		--xinShouBtn:SetPoint( "BOTTOMLEFT", "PetFrame_TameBtn", "TOPLEFT", 30, -50 );
		--GameWizard:doStep(GAMEWIZARD_FIRST_CLICK_PET_ZHONG_CHENG_BTN_WHEN_LOW);
		ShowPetFrameXinShouBtn( { text = "可点击这里回复宠物忠诚度（需要消耗驯兽糖果）", btn = "PetFrame_TameBtn" } );
	end
	
	if not GameWizard:isDo(GAMEWIZARD_FIRST_BLINK_AUTO_WEI_YANG_BTN) and petPackage:getPetNum() > 0 then
		local animationTexture = getglobal( "PetFrameAutoWeiYangBtnUVAnimationTex" );
		animationTexture:SetUVAnimation( 50, true );
		animationTexture:Show();
	end	
end

function CheckRollBackPetXinShouBtn()
	if not GameWizard:isDo(GAMEWIZARD_FIRST_CLICK_PET_ZHONG_CHENG_BTN_WHEN_LOW) and 
		GameWizard:isDo(GAMEWIZARD_FIRST_CLOSE_PET_ZHONG_CHENG_BTN_TIPS) then

		GameWizard:undoStep(GAMEWIZARD_FIRST_CLOSE_PET_ZHONG_CHENG_BTN_TIPS);
	end
end

function ShowPetFrameXinShouBtn( data )
	local petXinShouBtn = getglobal( "PetFrameXinShouBtn" );
	local rich = getglobal( "PetFrameXinShouBtnRich" );
	rich:SetClientString( data["text"] );

	local nSelfDefineExtendWidth = 18;
	ReSizeTooltip( rich:GetName(), petXinShouBtn:GetName(), nSelfDefineExtendWidth );
	petXinShouBtn:SetClientString( data["btn"] );
	local posInfo = t_xinShouBtnPosInfo[data["btn"]];
	petXinShouBtn:SetPoint( posInfo["selfPoint"], posInfo["relFrame"], posInfo["relPoint"], posInfo["offsetX"], posInfo["offsetY"] );

	local animationTexture = getglobal( data["btn"].."UVAnimationTex" );
	animationTexture:SetUVAnimation( 50, true );
	animationTexture:Show();

	local closeBtn = getglobal( petXinShouBtn:GetName().."_CloseBtn" );
	if data["btn"] == "PetFrame_TameBtn" then
		if not GameWizard:isDo(GAMEWIZARD_FIRST_CLOSE_PET_ZHONG_CHENG_BTN_TIPS) then
			petXinShouBtn:Show();
			closeBtn:Show();
		else
			petXinShouBtn:Hide();
		end
		
	else
		petXinShouBtn:Show();
		closeBtn:Hide();
	end
end

function PetFrame_OnShow()
	-- 重置窗口坐标		  
	PetFrame:SetPoint("topleft", "$parent", "topleft", 32, 163 );

	local t_fonts = { ["PetFrameTitleFont"] = { value = "宠物" } };
	for name, data in pairs( t_fonts ) do 
		local font = getglobal( name );
		font:SetText( data.value );
	end

	local mainplayer = ActorMgr:getMainPlayer();
	local petPackage = mainplayer:getPet();

	-- t_mainSelPetInfo = { heightId = 0, lowId = 0, id = 0, index = -1 };

	PetFrame_CatchPetDesc:SetLinkTextColor( 0, 255, 0 );
	PetFrame_CatchPetDesc:SetText( "宠物捕捉：彩虹岛  通过#L莉莉[时间城]#n传送", 255, 255, 190 );
	PetFrame_TrainPetDesc:SetLinkTextColor( 0, 255, 0 );
	PetFrame_TrainPetDesc:SetText( "宠物训练师：#L拉奥多[时间城]#n", 255, 255, 190 );

	local pageFont	= getglobal( "PetFrameListBackGroundPageFont" ); 
	
	-- 默认选中第一个宠物
	if petPackage:getPetNum() == 0 then
		ClearPetFrameWithoutPets();
		--PetFrame:Hide();
		--ShowMidTips("你还未拥有宠物！");
		return;
	end
	
	UpdatePetFrame();
	--UpdatePetFrameFairyBtn();	
	CheckPetXinShouGuid();
end

function UpdateCallBackBtn()
	local nMainSelectPetHeightID	= t_mainSelPetInfo["heightId"];
	local nMainSelectPetLowID		= t_mainSelPetInfo["lowId"];
	if nMainSelectPetHeightID == 0 and nMainSelectPetLowID == 0 then
		return;
	end

	local mainplayer		= ActorMgr:getMainPlayer();
	local petPackage		= mainplayer:getPet();
	local selfRongHePetWID	= petPackage:getSelfRongHePetWID();

	if selfRongHePetWID.nHeight == 0 and selfRongHePetWID.nLow == 0 then
		if not PetFrame_BackUpBtn:IsEnable() then
			Private_Enable_CallBackBtn();
		end
	else
		if selfRongHePetWID.nHeight == nMainSelectPetHeightID and selfRongHePetWID.nLow == nMainSelectPetLowID then
			Private_Disable_CallBackBtn();
		end
	end
end

function Private_Enable_CallBackBtn()
	PetFrame_BackUpBtn:Enable();
end

function Private_Disable_CallBackBtn()
	PetFrame_BackUpBtn:Disable();
end

function PetFrame_OnHide()
	--t_mainSelPetInfo = { heightId = 0, lowId = 0, id = 0, index = -1 };
	local t_hideIcons = { "PetFrame_TameBtnUVAnimationTex", "PetFrame_FightBtnUVAnimationTex", "PetFrameXinShouBtn", "PetAddLoveLevelMessageFrame" };
	util.HideIcons( t_hideIcons );
	ModelAngle = 0;
end

function PetFrame_OnUpdate()
	if t_mainSelPetInfo["id"] == 0 then
		return;
	end

	local mainplayer		= ActorMgr:getMainPlayer();
	local petPackage		= mainplayer:getPet();
	if not petPackage:isPetInState( t_mainSelPetInfo.index, CLIENT_PET_STAT_FIGHT ) then
		return;
	end

	local petActor = petPackage:getPetActor( GetFightPetXieDaiPos() );	
	local petInfo	= petPackage:getPetInfo( t_mainSelPetInfo["index"] );
	local t_fonts = { [this:GetName().."HPBtnHPValueFont"]	=	{ value = petActor:getHP() .. "/" .. petActor:getMaxHP() }, };				
	for name, data in pairs( t_fonts ) do
		local font = getglobal( name );
		font:SetText( data.value );
	end

	local t_regionButtons = 
	{
	[this:GetName().."HPBtn"] = { scale = petActor:getHP() / petActor:getMaxHP() },
	};
	for name, data in pairs ( t_regionButtons ) do
		local btn = getglobal( name );
		btn:SetNormalRegionWidth( data.scale );
		btn:Show();
	end
end

-- 清空宠物查看信息
function ClearPetInfo( szFrameName )
	PetFrame_BackUpBtn:Hide();
	PetFrame_FightBtn:Show();
	--t_mainSelPetInfo = { heightId = 0, lowId = 0, id = 0, index = -1 };
	local t_fonts = {
						-- 宠物名字
						szFrameName.."PetNameBtnFont",
						-- 宠物等级
						szFrameName.."PetLvValueBtnFont",
						-- 宠物天赋级别
						szFrameName.."PetGeniusValuBtnFont",
						-- 宠物HP
						szFrameName.."HPBtnHPValueFont",
						-- 宠物EXP
						szFrameName.."EXPBtnExpValueFont",		
						-- 宠物攻击成长值
						szFrameName.."AttackGrowthValueBtnFont",	
						-- 宠物生命成长值
						szFrameName.."LifeGrowthValueBtnFont",	
						-- 宠物物防成长值
						szFrameName.."PhysicDefGrowthValueBtnFont",
						-- 宠物魔防成长值
						szFrameName.."IceDefGrowthValueBtnFont",
						-- 宠物命中成长值
						szFrameName.."HitGrowthValueBtnFont",	
						-- 宠物闪避成长值
						szFrameName.."FleeGrowthValueBtnFont",
						-- 宠物攻击值
						szFrameName.."TotalAttackValueBtnFont",	
						-- 生命值
						szFrameName.."MaxLifeValueBtnFont",
						-- 宠物魔法防御值
						szFrameName.."IceDefValueBtnFont",	
						-- 宠物生命值
						szFrameName.."MaxLifeValueBtnFont",	
						-- 宠物物理防御值
						szFrameName.."PhyDefValueBtnFont",	
						-- 宠物命中值
						szFrameName.."HitValueBtnFont",	
						-- 宠物闪避值
						szFrameName.."FleeValueBtnFont",	
						-- 宠物暴击率值
						szFrameName.."HeaveyAttackValueBtnFont",	
						-- 宠物替身值
						szFrameName.."TiShenValueBtnFont",	
						-- 宠物忠诚度值
						szFrameName.."ZhongChenValueBtnFont",
						-- 宠物战斗力
						szFrameName.."FightValueBtnFont",
						-- 宠物召唤等级
						szFrameName.."CallLvValueBtnFont",	
						-- 宠物昵称
						szFrameName.."_NickNameBtnFont"	
					};
	for _, name in ipairs( t_fonts ) do
		local font = getglobal( name );
		font:SetText( "" );
	end

	local t_textures =	{ 
							szFrameName.."PetAtkTypeBtnTex", szFrameName.."DaiShuBtnTex"	
						};
	for _, name in ipairs( t_textures ) do
		local tex = getglobal( name );
		tex:Hide( );
	end

	for i = 1, MAX_PET_SKILL_GRID do
		local btn = getglobal( szFrameName.."_SkillButton"..i );
		local tex = getglobal( btn:GetName().."Icon" );
		local font = getglobal( btn:GetName().."Level" );
		tex:SetTexture( "uires\\ZhuJieMian\\1.dds" );
		tex:SetTexUV( 749, 517, 44,44 ); 
		font:SetText("")
		btn:SetClientUserData( 2, PET_SKILL_DISABLE );
	end
	
	local t_modelViewers = { szFrameName.."ModelBtnModelView" };
	for _, name in ipairs( t_modelViewers ) do
		local modelViewer = getglobal( name );
		modelViewer:Hide();
	end

	
	local t_editBoxes = { szFrameName.."_NickNameEdit" };
	for _, name in ipairs( t_editBoxes ) do
		local edit = getglobal( name );
		edit:Clear();
	end

	local t_hideIcons = { szFrameName.."HPBtn", szFrameName.."EXPBtn", szFrameName.."FleeGrowthValueBtn", szFrameName.."HitGrowthValueBtn",
						szFrameName.."IceDefGrowthValueBtn", szFrameName.."PhysicDefGrowthValueBtn", szFrameName.."LifeGrowthValueBtn",
						szFrameName.."AttackGrowthValueBtn" };
	for _, name in ipairs( t_hideIcons ) do
		local icon = getglobal( name );
		icon:Hide();
	end

	
	local btn			= getglobal( szFrameName.."GeniusBtn" );
	btn:SetClientUserData( 2, PET_SKILL_DISABLE );
	--btn:Disable();
	local geniuseTex	= getglobal( btn:GetName().."IconTex" );
	geniuseTex:SetTexture( "uires\\ZhuJieMian\\2.dds" );
	geniuseTex:SetTexUV( 182, 513, 44, 44 );
end

function PetListButtonSelect(index)
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local petInfo		= petPackage:getPetInfo( index );  
	DisCheckAllBtn( "PetFrame_ListButton", MAX_PET_ONE_PAGE );
	local pageFont		= getglobal( "PetFrameListBackGroundPageFont" ); 
	local nPage			= tonumber( pageFont:GetText() );
	local i = index + 1 - ( nPage - 1 ) * MAX_PET_ONE_PAGE;
	if i > 0 and i <= MAX_PET_ONE_PAGE then
		local btn = getglobal("PetFrame_ListButton"..i);
		if btn then
			btn:Checked();
		end
	end

	t_mainSelPetInfo	= { heightId = petInfo.WID.nHeight, lowId = petInfo.WID.nLow, id = petInfo.PetID, index = index };
	ShowPetInfo( petInfo, "PetFrame", true );
end

function PetListButton_OnEnter()
	SetGameTooltips(this:GetName(), "Shift+左键点击可发送宠物链接至聊天框")
end

function PetListButton_OnClick()
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local pageFont		= getglobal( "PetFrameListBackGroundPageFont" ); 
	local nPage		= tonumber( pageFont:GetText() )
	local nPetIndex		= ( nPage - 1 ) * MAX_PET_ONE_PAGE + this:GetClientID() - 1;
	local petInfo		= petPackage:getPetInfo( nPetIndex );  
	-- 按住shift
	if isKeyPressed(160) or isKeyPressed(161) then
		ChatManager:setPetLink( petInfo.WID.nLow,petInfo.WID.nHeight,petInfo.Name,mainplayer:getName() );
		return;
	end
	if IsMessageBoxBlock(PetSkillResetFrame) or IsMessageBoxBlock(PetGrowResetFrame) or IsMessageBoxBlock(Pet_DevelopmentBuildUpFrame) then
		return
	end
	PetListButtonSelect(nPetIndex);
	if PetSkillResetFrame:IsShown() then
		SelectPetOnPetSkillResetFrame(nPetIndex + 1);
	end
	if PetSkillLevelUpFrame:IsShown() then
		SelectPetOnPetSkillLevelUpFrame(nPetIndex + 1);
	end
	if PetGrowResetFrame:IsShown() then
		SelectPetOnPetGrowResetFrame(nPetIndex + 1);
	end
	
	if Pet_DevelopmentBuildUpFrame:IsShown() then
		PetFrame_SelectPet("Pet_DevelopmentBuildUpFrame", nPetIndex)
	end
	if PetStepUpFrame:IsShown() then 
		PetFrame_SelectPet("PetStepUpFrame", nPetIndex)
	end
	if PetImpUpFrame:IsShown() then 
		PetFrame_SelectPet("PetImpUpFrame", nPetIndex)
	end
	if PetUnBindFrame:IsShown() then 
		PetFrame_SelectPet("PetUnBindFrame", nPetIndex)
	end
end

-- 宠物主面板宠物列表的上翻页
function PrePageBtn_OnClick()
	local pageFont	= getglobal( "PetFrameListBackGroundPageFont" ); 
	local nPage		= tonumber( pageFont:GetText() );
	
	if nPage <= 1 then
		return;
	end
	pageFont:SetText( nPage - 1 );
	UpdatePeFrame_PetList();
end

-- 宠物主面板宠物列表的下翻页
function NextPageBtn_OnClick()
	local pageFont	= getglobal( "PetFrameListBackGroundPageFont" ); 
	local nPage		= tonumber( pageFont:GetText() );
	local mainplayer = ActorMgr:getMainPlayer();
	local petPackage = mainplayer:getPet();
	local nMaxPage	= math.ceil( petPackage:getPetNum() / MAX_PET_ONE_PAGE );
	if nPage >= nMaxPage then
		return;
	end
	pageFont:SetText( nPage + 1 );
	UpdatePeFrame_PetList();
end

--@desc:点击左转按钮时向左转动预览模型
--@return: Nothing
function Pet_TurnLeftBtn_OnMouseDown()
	local ModelViewer = getglobal( this:GetParent().."ModelBtnModelView");
	if not ModelViewer:IsShown() then
		return;
	end
	
	ModelAngle = ModelAngle - 5;
	ModelViewer:setEntityRotate(0,ModelAngle);
end

--@desc:点击左右转按钮时向右转动预览模型
--@return: Nothing
function Pet_TurnRightBtn_OnMouseDown()	  
	local ModelViewer = getglobal( this:GetParent().."ModelBtnModelView");
	if not ModelViewer:IsShown() then
		return;
	end
	
	ModelAngle = ModelAngle + 5;
	ModelViewer:setEntityRotate(0,ModelAngle);
end

function ClearPetList()
	for i = 1, MAX_PET_ONE_PAGE do
		local btn			= getglobal( "PetFrame_ListButton"..i );
		local icon			= getglobal( "PetFrame_ListButton"..i.."FightIcon" );
		local battleIcon	= getglobal( "PetFrame_ListButton"..i.."BattleIcon" );
		btn:SetText( "" );
		btn:SetClientUserData( 0, 0 );
		btn:DisChecked();
		btn:Disable();
		icon:Hide();
		battleIcon:Hide();
	end
end

function UpdatePeFrame_PetList()
	ClearPetList();
	local pageFont		= getglobal( "PetFrameListBackGroundPageFont" ); 
	local nPage			= tonumber( pageFont:GetText() );
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local nButtonIndex	= 1;
	local nPetNum		= 0;

	for i = 1, petPackage:getPetNum() do
		if nButtonIndex > MAX_PET_ONE_PAGE then
			break;
		end

		local btn			= getglobal( "PetFrame_ListButton"..nButtonIndex );
		local icon			= getglobal( "PetFrame_ListButton"..nButtonIndex.."FightIcon" );
		local battleIcon	= getglobal( "PetFrame_ListButton"..nButtonIndex.."BattleIcon" );
		local pet	= petPackage:getPetInfo( i - 1 );
		if pet ~=nil and pet.PetID ~= 0 then
			nPetNum = nPetNum + 1;
			if nPetNum > ( nPage - 1 ) * MAX_PET_ONE_PAGE and nButtonIndex <= MAX_PET_ONE_PAGE then
				--font:SetTextColor( color.r, color.g, color.b );
				if petPackage:isPetInState( i - 1, CLIENT_PET_STAT_FIGHT ) then
					if not icon:IsShown() then
						icon:Show();
					end
				else
					icon:Hide();
				end

				if petPackage:isExtraClanJiYangPet( pet ) then 
					battleIcon:SetTexture("uires\\ZhuJieMian\\3.dds")
					battleIcon:SetTexUV( 395, 792, 15, 15 )
					battleIcon:Show();
				elseif petPackage:isExtraBattlePet( pet ) then
					battleIcon:SetTexture("uires\\ZhuJieMian\\1.dds")
					battleIcon:SetTexUV( 994, 285, 15, 15 )
					battleIcon:Show();
				else 
					battleIcon:Hide();
				end
				local petName 		= pet.Name
				local nNameLength 	= string.len( petName );
				if nNameLength > 10 then
					if IsDBCSHead( petName, 10) then 
						btn:SetText( string.sub( petName ,1 ,8 ).."..." );
					else
						btn:SetText( string.sub( petName ,1 ,9 ).."..." );
					end
				else
					btn:SetText( petName );
				end
		--		btn:SetText( pet.Name );
				btn:SetClientUserData( 0, i );
				btn:Enable();
				nButtonIndex = nButtonIndex + 1;
				-- 若当前设置的宠物即为被选定的宠物
				if pet.WID.nHeight == t_mainSelPetInfo["heightId"] and pet.WID.nLow == t_mainSelPetInfo["lowId"] then
					btn:Checked();
				end
			end
		end
	end

	local t_fonts = {
						["PetFrameListBackGroundNumTitleFont"] = { value = "宠物列表" .. petPackage:getPetNum().."/"..petPackage:getPetMaxNum() },
					};
	for name, data in pairs( t_fonts ) do
		local font = getglobal( name );
		font:SetText( data.value );
	end

	local t_showFightBtn = 
							{
							[true] =	{ func = function ()
													PetFrame_BackUpBtn:Show();
													PetFrame_FightBtn:Hide();
												end
										},
							[false] = { func = function ()
												PetFrame_BackUpBtn:Hide();
												PetFrame_FightBtn:Show();
											end
										}
							};
	-- 设定出征和召回按钮的显示情况
	if t_mainSelPetInfo["id"] ~= 0 then
		t_showFightBtn[petPackage:isPetInState( t_mainSelPetInfo.index, CLIENT_PET_STAT_FIGHT )].func();
	end
end

function PetFrameAutoWeiYangBtn_Click()
	if PetAutoRecoverFrame:IsShown() then
		PetAutoRecoverFrame:Hide();
	else
		PetAutoRecoverFrame:Show();
	end
	
	local mainplayer = ActorMgr:getMainPlayer();
	local petPackage = mainplayer:getPet();
	if not GameWizard:isDo(GAMEWIZARD_FIRST_BLINK_AUTO_WEI_YANG_BTN) and petPackage:getPetNum() > 0 then
		local animationTexture = getglobal( "PetFrameAutoWeiYangBtnUVAnimationTex" );
		animationTexture:Hide();
		GameWizard:doStep( GAMEWIZARD_FIRST_BLINK_AUTO_WEI_YANG_BTN );
	end
end

PET_LEVEL_LIMIT		= 10;	-- /* 等级差限制 */
PWORLD_NO_PET_FIGHT = 8;	-- 宠物在副本中不能出征的字段
-- 宠物出征吟唱
function CanCallPet( data )
	local nPetIndex		= data["petIndex"];
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local petInfo		= petPackage:getPetInfo( nPetIndex );
	if petPackage:isPre() then
		ShowMidTips( "宠物正在召唤中，无法进行新的召唤" );
		return;
	end

	if mainplayer:isDead() then
		ShowMidTips( "主角处于死亡状态，不能召唤宠物" );
		return;
	end

	if mainplayer:isInFly() then
		ShowMidTips( "处于飞行状态，不能召唤宠物" );
		return;
	end
	
	if petInfo == nil then
		return
	end

	-- 先判断主角所在地图是否能够骑乘
	local nRealMapID = MapShower:getRealMapId();
	if nRealMapID >= MIN_COPY_ID then
		local pworldDef = getPworldDef( nRealMapID / 100 );
		if bitAnd( pworldDef.CtrlFlag, PWORLD_NO_PET_FIGHT ) then
			ShowMidTips( "该副本不能召唤宠物" );
			return;
		end
	end
	
	local petDef	= getPetDef( petInfo.PetID );
	-- 判断宠物是否在摊位上
	if StallSaleFrame:IsShown() and GameStallManager:isInStallIPet( SHITYPE, nPetIndex ) then
		ShowMidTips( "该宠物已经处于摆摊交易栏上，不能进行召唤" );
		return;
	end

	-- 判断宠物是否在交易栏上
	if TradeFrame:IsShown() and GameExchange:isInExchangePet( SHITYPE, nPetIndex ) then
		ShowMidTips( "该宠物已经处于交易栏上，不能进行召唤" );
		return;
	end

	-- 若人物低于召唤等级，则不能召唤宠物
	if mainplayer:getLv() < petDef.FightLevel then
		ShowMidTips( "召唤该宠物需要角色等级达到" .. petDef.FightLevel );
		return;
	end
	-- 若宠物等级比主角高10级，则宠物不能出战
	if petInfo.Level - mainplayer:getLv() > PET_LEVEL_LIMIT then
		ShowMidTips( "宠物等级比你高10级，不能进行召唤" );
		return;
	end

	-- 查看宠物的忠诚度是否达到出征的最低要求
	if petInfo.LoveVal < PET_FIGHT_LOVEVAL then
		ShowMidTips( "宠物忠诚度太低，不能被召唤，请使用驯兽糖果或者驯兽鞭子恢复" );
		return;
	end

	return true;
end

function PetFrameFightBtn_OnClick()
	-- 若处于吟唱状态，则不能点击出征
	local mainplayer = ActorMgr:getMainPlayer();
	local petPackage = mainplayer:getPet();
	if petPackage:isPre() then
		ShowMidTips( "宠物正在召唤中，无法进行新的召唤" );
		return;
	end

	-- 再判断当前是否处于吟唱状态
	--[[
	if not CheckIntonateBar( "召唤宠物出征" ) then
		return;
	end
	--]]

	if t_mainSelPetInfo.id == 0 then
		return;
	end
	
	if mainplayer:isDead() then
		ShowMidTips( "主角处于死亡状态，不能召唤宠物" );
		return;
	end

	if mainplayer:isInFly() then
		ShowMidTips( "处于飞行状态，不能召唤宠物" );
		return;
	end

	if GameExchange:isInExchangePetByWID( t_mainSelPetInfo.heightId, t_mainSelPetInfo.lowId ) then
		ShowMidTips( "宠物正在交易中，无法召唤" );
		return;
	end

	-- 先判断主角所在地图是否能够骑乘
	local nRealMapID = MapShower:getRealMapId();
	if nRealMapID >= MIN_COPY_ID then
		local pworldDef = getPworldDef( nRealMapID / 100 );
		if bitAnd( pworldDef.CtrlFlag, PWORLD_NO_PET_FIGHT ) then
			ShowMidTips( "该副本不能召唤宠物" );
			return;
		end
	end
	
	local petInfo	= petPackage:getPetInfo( t_mainSelPetInfo["index"] );
	local petDef	= getPetDef( petInfo.PetID );
	-- 判断宠物是否在摊位上
	if StallSaleFrame:IsShown() and GameStallManager:isInStallIPet( SHITYPE, t_mainSelPetInfo["index"] ) then
		ShowMidTips( "该宠物上架中，无法进行召唤" );
		return;
	end

	-- 判断宠物是否在交易栏上
	if TradeFrame:IsShown() and GameExchange:isInExchangePet( SHITYPE, t_mainSelPetInfo["index"] ) then
		ShowMidTips( "该宠物已经处于交易栏上，不能进行召唤" );
		return;
	end

	-- 若人物低于召唤等级，则不能召唤宠物
	if mainplayer:getLv() < petDef.FightLevel then
		ShowMidTips( "召唤该宠物需要角色等级达到" .. petDef.FightLevel );
		return;
	end
	
	-- 若宠物等级比主角高10级，则宠物不能出战
	if petInfo.Level - mainplayer:getLv() > PET_LEVEL_LIMIT then
		ShowMidTips( "宠物等级比你高10级，不能进行召唤" );
		return;
	end
	if petPackage:isExtraClanJiYangPet( petInfo ) then 
		ShowMidTips( "宠物正在寄养中，请前往公会城市取消寄养再进行该操作！" );
		return;
	end
	-- 查看宠物的忠诚度是否达到出征的最低要求
	if petInfo.LoveVal < PET_FIGHT_LOVEVAL then
		ShowMidTips( "宠物忠诚度太低，不能被召唤，请使用驯兽糖果或者驯兽鞭子恢复" );
		return;
	end

	-- 检查出征的宠物是否正在融合
	local t_PetRongHeProcessFrameControl = GetPetRongHeProcessFrameControl();
	if t_PetRongHeProcessFrameControl:isAnyPetRongHe() then
		ShowMidTips( "正在进行宠物融合，不能进行召唤" );
		return;
	end

	-- 先召唤会原来所有已经处于出征状态的宠物
	petPackage:petPre( t_mainSelPetInfo.heightId, t_mainSelPetInfo.lowId );
end

-- 宠物召回
function PetFrameBackUpBtn_OnClick()
	if t_mainSelPetInfo.id == 0 then
		return;
	end

	if PetRongHeProcessFrame:IsShown() then
		ShowMidTips( "正在进行融合，不能召回宠物" );
		return;
	end

	if GetCampPetActorId() ~= 0 then
		GameWizard:hideNpcTalkByResId( GetCampPetActorId() );
	end
	local mainplayer = ActorMgr:getMainPlayer();
	local petPackage = mainplayer:getPet();
	petPackage:petBackUp( t_mainSelPetInfo.heightId, t_mainSelPetInfo.lowId );
end

function GetXunFuItemGirdx()
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local nGridIdx		= -1;

	for i = 1, container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES ) do
		local Item		= container:getItem( CONTAINER_TYPE_SUNDRIES, i - 1 );
		local itemDef	= Item:getItemDef();
		if Item:getItemId() > 0 then
			assert( itemDef ~= nil );
			for j = 1,MAX_ITEM_RESULT do
				if itemDef.UseTarget == USE_TARGET_PET and	itemDef.Result[j-1].ResultID ==  RESULT_PET_ADD_LOVEVAL then
						nItemLevl = itemDef.Result[j-1].ResultVal2;
						nGridIdx = i-1;
				end
			end
		end
	end
	
	return nGridIdx;
end

-- 宠物驯服
function PetFrameTameBtn_OnClick()
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet( );
	if petPackage:isPre() then
		ShowMidTips( "宠物正在召唤中，无法驯服" );
		return;
	end

	if t_mainSelPetInfo.id == 0 then
		return;
	end

	local petInfo		= petPackage:getPetInfo( t_mainSelPetInfo["index"] );
	local nCurLoveVal	= math.floor( petInfo.LoveVal / 100 );
	--[[
	if nCurLoveVal == MAX_PET_LOVE_VAL / 100 then
		ShowMidTips( "宠物忠诚度已达到上限，不能增加宠物的忠诚度" );
		return;
	end
	--]]

	if petPackage:isPetInFreedom( t_mainSelPetInfo["heightId"], t_mainSelPetInfo["lowId"] ) then
		ShowMidTips( "该宠物已经被您放生，不可进行该操作！" );
		return;
	end

	local container		= mainplayer:getContainer();
	local nMaxNum		= container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES );
	local nGridIdx		= -1;

	for i = 1, nMaxNum do
		local Item = container:getItem( CONTAINER_TYPE_SUNDRIES, i - 1 );
		local itemDef = Item:getItemDef();
		if Item:getItemId() > 0 then
			if itemDef.UseTarget == USE_TARGET_PET and itemDef.Result[0].ResultID ==  RESULT_PET_ADD_LOVEVAL then
				nGridIdx = i - 1;
			end
		end
	end
	if nGridIdx == -1 then
		ShowMidTips("缺少驯兽糖果或驯兽鞭子，无法驯服宠物来增加忠诚度");
		return;
	end
	
	container:usedItem( CONTAINER_TYPE_SUNDRIES, nGridIdx, 0, t_mainSelPetInfo["index"] );
end

-- 宠物放生
function PetFrameFreedomBtn_OnClick()
	local mainplayer	= ActorMgr:getMainPlayer();
	local container = mainplayer:getContainer();
	local lockState = container:getContainerLockState();
	if lockState == CONTAINER_LOCK_STATE_LOCK then
		ShowMidTips("请先验证背包安全锁");
		return;
	end
	local petPackage	= mainplayer:getPet( );
	if petPackage:isPre() then
		ShowMidTips( "宠物正在召唤中，无法放生" );
		return;
	end

	if GameExchange:isInExchangePetByWID( t_mainSelPetInfo.heightId, t_mainSelPetInfo.lowId ) then
		ShowMidTips( "宠物正在交易中，不能放生" );
		return;
	end

	if t_mainSelPetInfo.id == 0 then
		return;
	end

	if petPackage:isPetInFreedom( t_mainSelPetInfo["heightId"], t_mainSelPetInfo["lowId"] ) then
		ShowMidTips( "该宠物已经被您放生，不可进行该操作！" );
		return;
	end

	if GameStallManager:isInStallIPet( SHITYPE, t_mainSelPetInfo.heightId, t_mainSelPetInfo.lowId ) then
		ShowMidTips( "宠物上架中，不能放生" );
		return;
	end
	local petInfo		= petPackage:getPetInfo( t_mainSelPetInfo["index"] );
	if petInfo.PetGenius > 2 then
		return AddGameMiddleTips2( "杰出、完美资质的宠物无法放生！" );
	end
	SetDropPetInfo( t_mainSelPetInfo );	
	DropPetConfirmFrame:Show();	
end

--@Desc:接收客户端发送的通知开始宠物出征吟唱
--@Return:Nothing
function PetCampaign( nTime )
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then
		return;
	end

	IntonateBar_text:SetText("正在召唤宠物");
	IntonateBar:SetClientUserData( 0, PET_FAIRY_TYPE );
	IntonateBar:SetClientUserData( 2, nTime );
	IntonateBar:Show();
end

function Pet_HousecarlArmButton_OnEnter()
	local szText = "功能暂未开放，敬请期待";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function Pet_HousecarlArmButton_OnLeave()
end

function Pet_HousecarlArmButton_Drag()
end

function Pet_HousecarlArmButton_ReceiveDrag()
end

function PetFrameFairyBtn_OnEnter()
	local player = ActorMgr:getMainPlayer()
	local petMgr = player:getPet()
	local trump = player:getTrump() 
	local fairy = trump:getCampRoleFairy()
	
	if fairy and petMgr:isPetInState(t_mainSelPetInfo.index, CLIENT_PET_STAT_FIGHT) then
		local TrumpDef = trump:getFairyDef(fairy.FairyID, fairy.FairyLevel)
		local FairyStarDef = trump:getFairyStarDef(fairy.FairyID, fairy.StarLvl)
		local TraitDef = trump:getPetTraitDef(TrumpDef.TraitID, fairy.StarLvl)
		
		local t = {}
		table.insert(t, string.format("宠物%s伤害+%d", t_AttrTypeName[FairyStarDef.AttrType], FairyStarDef.Atk))
		table.insert(t, string.format("宠物%s", TraitDef.PetTraitDesc))
		table.insert(t, string.format("宠物获取经验+%s%%", FairyStarDef.PetExpMul))
		SetGameTooltips(this:GetName(), string.format("当前宠物已被%s管制\n%s\n点击打开精灵面板", TrumpDef.Name, table.concat(t, "\n")))
	else
		SetGameTooltips(this:GetName(), "当前已召唤的精灵\n只有出征宠物，才能接受精灵的管制\n点击打开精灵面板")
	end
end

function PetFrameFairyBtn_OnLeave()
	GameTooltipOnLeave()
end

function PetFrameFairyBtn_OnClick()
	XianButtonOnClick()
end

function PetFrameHetiBtn_OnLoad()
	local tex = getglobal(this:GetName().."Icon")
	local level = SkillMgr:getSpellLv(PET_HETI_SKILL_ID)
	local SkillDef	= SkillMgr:getSpellStaticAtt(PET_HETI_SKILL_ID, level == 0 and 1 or level)
	local IconPath = GetSkillIconPath()..SkillDef.IconID1..".tga"
	if not IsInExistence(IconPath) then
		IconPath = DEFAULT_ITEM_PATH
	end
	tex:SetTexture(IconPath)
end

function PetFrameHetiBtn_OnEnter()
	SetGameTooltips(this:GetName(), [===[
#cff6e1c合体条件#n
1、宠物必须召唤后方可合体
2、宠物忠诚度必须大于80%
3、宠物和人物等级同时大于60级
#cff6e1c合体状态#n
1、合体后每分钟消耗一点忠诚度
2、忠诚度小于50%将结束合体状态
3、根据宠物综合属性增加人物属性
4、勇气值影响人物对其他玩家的最
终伤害加成
#c00ff00宠物合体图标可以左键拖拽至快捷栏#n]===])
end

function PetFrameHetiBtn_OnLeave()
	GameTooltipOnLeave()
end

function PetFrameHetiBtn_OnDrag()
	if UIIsInDragState() then
		UIEndDrag()
		UIMSG:getUIGV().DragFrom = DRAG_NONE
		return
	end
	
	local name = this:GetName()
	local tex = getglobal(name.."Icon")
	if tex:IsGray() then
		return
	end
	
	GameWizard:doStep(GAMEWIZARD_FIRST_USE_PET_HETI)
	local uv = getglobal(name.."UVAnimationTex")
	uv:Hide()
	
	UIMSG:getUIGV().DragFrom = DRAG_SKILL
	UIBeginDrag(tex:GetTexture(), PET_HETI_SKILL_ID, 0, 0)
	-- 处于拖拽状态 同时 右部技能栏未显示 则显示右部技能栏
	if UIIsInDragState() and not ExtendSkillLeftFrame:IsShown() then
		ShowExtendSkill()
	end
end

function PetFrameHetiBtn_OnClick()
	if UIIsInDragState() then
		UIEndDrag()
		UIMSG:getUIGV().DragFrom = DRAG_NONE
		return
	end
	
	local name = this:GetName()
	local tex = getglobal(name.."Icon")
	if tex:IsGray() then
		return
	end
	
	GameWizard:doStep(GAMEWIZARD_FIRST_USE_PET_HETI)
	local uv = getglobal(name.."UVAnimationTex")
	uv:Hide()

	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local petPackage = mainplayer:getPet();
	local petInfo	 = petPackage:getPetInfo( t_mainSelPetInfo.index );
	if not petPackage:isPetInState( petInfo.WID, CLIENT_PET_STAT_FIGHT ) then
		return AddGameMiddleTips2("召唤的宠物才能合体，请先召唤该宠物");
	end

	GameShortCut:useSkill(PET_HETI_SKILL_ID)
	SkillMgr:unUseSpell()
end

function PetFrame_NickNameBtn_OnClick()
	this:Hide();
	local edit = getglobal( this:GetParent().."_NickNameEdit" );
	SetFocusFrame( edit:GetName() );
	edit:SelectAllText();
	edit:Show();
end

function PetFrame_NickNameEdit_OnFocusLost()
	local edit = getglobal( this:GetParent().."_NickNameEdit" );
	edit:Hide();
	local btn = getglobal( this:GetParent().."_NickNameBtn" );
	btn:Show();
end

function PetSetNickNameBtn_OnClick()
	if t_mainSelPetInfo.id == 0 then
		return;
	end
	
	local mainplayer = ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	if petPackage:isPetInFreedom( t_mainSelPetInfo["heightId"], t_mainSelPetInfo["lowId"] ) then
		ShowMidTips( "该宠物已经被您放生不可进行该操作！" );
		return;
	end
	
	local edit = getglobal( this:GetParent().."_NickNameEdit" );
	edit:Hide();
	local btn = getglobal( this:GetParent().."_NickNameBtn" );
	btn:Show();

	
	local petDef = petPackage:getPetDef( t_mainSelPetInfo.id );
	if petDef == nil then
		return;
	end

	local szPetName = petDef.Name;
	if t_mainSelPetInfo.Name ~= "" then
		szPetName = t_mainSelPetInfo.Name;
	end
	
	local newName = edit:GetText();
	if newName == "" or szPetName == newName then
		return;
	end
	
	if PetFrame_ListButton1Font:GetTextExtentWidth(newName) > 75 then
		--return ShowMidTips("宠物名字太长！");
	end

	petPackage:petChangeName( t_mainSelPetInfo.heightId, t_mainSelPetInfo.lowId, PetFrame_NickNameEdit:GetText() );

	local font = getglobal( this:GetParent().."_NickNameBtnFont" );
	font:SetText( edit:GetText() );
end

function ShowSkillGridTips( nGridType )
end

function getPetSkillPath()
	return "uires\\TuPianLei\\JiNeng\\ChongWuJiNeng\\";
end

local oldHeight = 36
function ShowPetSkillTips( parentFrame, petInfo, skillGridInfo, szRelButtonName, isGenius )
	local diathesisDef	= SkillMgr:getDiathesisDef( skillGridInfo.SkillID, skillGridInfo.SkillLevel );
	local t_fonts	= {};
	local t_tgas	= {};
	local t_riches	= {};

	-- 若格子有技能，则显示技能tips
	if diathesisDef ~= nil then
		local IconPath = getPetSkillPath()..diathesisDef.IconID..".tga";
		t_fonts = 
						{
						["PetSkillTipsFrameNameFont"]		= { value = diathesisDef.Name },-- ..diathesisDef.Level.."级"
						--["PetSkillTipsFrameSkillTypeFont"]	= { value = "类型："..t_skillGridTga[skillGridInfo.GridType].typeDesc },
						};
		
		t_tgas = 
						{
						["PetSkillTipsFrameTex"] = { path = ( IsInExistence( IconPath ) and IconPath or DEFAULT_ITEM_PATH ) },
						};
		
		t_riches = 
					{
					["PetSkillTipsFrameDescRich"] = { text = isGenius and diathesisDef.DiathesisDesc.."\n#G宠物召唤状态下激活该效果" or 
																diathesisDef.DiathesisDesc, 
													color = { r = 255, g = 255, b = 255 } },
					};
	-- 否则，显示格子tips
	else
		t_fonts = 
						{
						["PetSkillGridTipsFrameNameFont"]		= { value = "宠物技能栏位" },
						--["PetSkillGridTipsFrameGridTypeFont"]	= { value = "类型："..t_skillGridTga[skillGridInfo.GridType].typeDesc },
						["PetSkillGridTipsFrameGridTypeFont"]	= { value = "可以学习新技能" },
						};
		
		t_tgas = 
						{
						-- path = t_skillGridTga[skillGridInfo.GridType].path
						["PetSkillGridTipsFrameTex"] = { path = t_skillGridTga[PET_SKILL_SELF_BUFF].path },
						};
		local t_skillGridTga = 
						{
						[PET_SKILL_ATK]			= "该技能格为攻击类技能格，可学习攻击类技能，找时间城宠物训练师拉奥多消耗相应类型技能书可学习",
						[PET_SKILL_OWNER_BUFF]	= "该技能格为佑主类技能格，可学习佑主类技能，找时间城宠物训练师拉奥多消耗相应类型技能书可学习",
						[PET_SKILL_SELF_BUFF]	= "该技能格为增强类技能格，可学习增强类技能，找时间城宠物训练师拉奥多消耗相应类型技能书可学习",
						};
		t_riches = 
					{
					["PetSkillGridTipsFrameDescRich"] = { text = "找时间城宠物训练师拉奥多消耗相应类型技能书可学习技能", color = { r = 255, g = 255, b = 255 } },
					};
		
		if parentFrame:GetName() ~= "PetFrame" then
			return;
		end

		--[[local szText = "右键点击学习技能";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
							button = this:GetName(), vertical = "bottom" } );]]
		return;
	end

	for name, data in pairs( t_fonts ) do
		local font = getglobal( name );
		font:SetText( data.value );
	end

	for name, data in pairs( t_riches ) do
		local rich = getglobal(name);
		rich:SetText(data.text, data.color.r, data.color.g, data.color.b);
--		local oldHeight = rich:GetRealHeight();
		local height = rich:GetTotalHeight();
		local width = rich:GetRealWidth();
		rich:resizeRect(width, height);
		rich:SetSize(math.ceil(width / GetScreenScaleY()), math.ceil(height / GetScreenScaleY()));
		rich:SetText(data.text, data.color.r, data.color.g, data.color.b);
		local parent = getglobal(rich:GetParent())
		rich:SetPoint("TopLeft","PetSkillTipsFrameTex","bottomleft",0, 18 * GetScreenScaleY());
		if parent then
			parent:SetHeight(parent:GetHeight() + height - oldHeight );
		end
		oldHeight	= height;
	end

	for name, data in pairs( t_tgas ) do
		local tex = getglobal( name );
		tex:SetTexture( data.path );
		tex:SetTexRelUV( 1, 1 );
	end
	
	local t_showTips = { [true] = { frame = "PetSkillTipsFrame", }, [false] = { frame = "PetSkillGridTipsFrame", } };
	local tips = getglobal( t_showTips[diathesisDef ~= nil].frame );
	tips:SetClientString( parentFrame:GetName() );
	tips:Show();
	util.AdjustBigtipsScreenPos( { tipsFrame = tips:GetName(), button = szRelButtonName } );
end

function PetSkillBtn_OnClick()
	if this:GetParent() ~= "PetFrame" then
		return;
	end
	
	if t_mainSelPetInfo["id"] == 0 then
		return;
	end
	do return end
	
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local nSkillType = this:GetClientUserData( 2 );
	if nSkillType == PET_SKILL_DISABLE and arg1 == "LeftButton" then
		if petPackage:isPetInFreedom( t_mainSelPetInfo["heightId"], t_mainSelPetInfo["lowId"] ) then
			ShowMidTips( "该宠物已经被您放生不可进行该操作！" );
			return;
		end

		local t_AddPetSkillControl = GetAddPetSkillControl();
		t_AddPetSkillControl:setOpPet( t_mainSelPetInfo );
		OpenPetSkillConfirmFrame:Show();
	elseif arg1 == "RightButton" then
		
		if petPackage:isPetInFreedom( t_mainSelPetInfo["heightId"], t_mainSelPetInfo["lowId"] ) then
			ShowMidTips( "该宠物已经被您放生不可进行该操作！" );
			return;
		end

		SetOperationSelPetInfo( t_mainSelPetInfo );
		if Pet_SkillLearnFrame:IsShown() then
			--Pet_SkillLearnFrame:Hide();
			local t_selUseItem = GetOperationSelUseItem();
			--[[
			if t_selUseItem["id"] ~= 0 then
				ClearItemLock( "Pet_SkillLearnFrame" );
			end
			--]]
			if t_selUseItem["id"] ~= 0 then
				ClearPet_OperationItem();
			end
		end
		Pet_SkillLearnFrame:Show();
	end
end

function PetGeniusButton_OnEnter()
	local nSkillType = this:GetClientUserData( 2 );
	if nSkillType == PET_SKILL_DISABLE then
		--local szText = "未激活\n激活方式：将一代宠物融合成二代时有100%几率获得\n特别提示：一代变异宠物有50%几率在孵化时激活天赋";
		local szText = "未激活\n激活方式：变异宠物有50%概率在孵化时获得天赋";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
							button = this:GetName(), vertical = "bottom" } );
		return;
	end
	ShowPetSkillTips( getglobal( this:GetParent() ), petInfo, 
						{ SkillID = this:GetClientUserData( 0 ), SkillLevel = this:GetClientUserData( 1 ),
							GridType = this:GetClientUserData( 2 ), }, this:GetName(), true );
end

function PetSkillBtn_OnEnter()
	local nSkillType = this:GetClientUserData( 2 );
	if nSkillType == PET_SKILL_DISABLE then
		if this:GetParent() ~= "PetFrame" then
			return;
		end
		local levels = {20, 30, 40, 60, 70}

		local szText = string.format("宠物达到%d级时将随机领悟一个技能", levels[this:GetClientID()])
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
							button = this:GetName(), vertical = "bottom" } );
		return;
	end
	ShowPetSkillTips( getglobal( this:GetParent() ), petInfo, 
						{ SkillID = this:GetClientUserData( 0 ), SkillLevel = this:GetClientUserData( 1 ),
							GridType = this:GetClientUserData( 2 ), }, this:GetName() );
end

function PetSkillBtn_OnLeave()
	local t_showTips = { "PetSkillTipsFrame","PetSkillGridTipsFrame", "GameTooltip" };
	for _, name in ipairs( t_showTips ) do
		local frame = getglobal( name );
		frame:Hide();
	end
end

function OpenWatchPetFrame()
	WatchPetFrame:Show();
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local petInfo		= petPackage:getWatchPetInfo();
	ShowPetInfo( petInfo, "WatchPetFrame", true );
end

-- 此为丢弃宠物的时候刷新宠物
function ClearPetFrame()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local petPackage = mainplayer:getPet();
	
	local nOldSelectPetIndex = t_mainSelPetInfo["index"];
	t_mainSelPetInfo = { heightId = 0, lowId = 0, id = 0, index = -1 };

	-- 默认选中第一个宠物
	if petPackage:getPetNum() == 0 then
		ClearPetFrameWithoutPets();
		UpdatePetPhotoWhenDropPet( { selectPetIndex = - 1, oldSelectPetIndex = nOldSelectPetIndex } );
		--PetFrame:Hide();
		--ShowMidTips("身上已没有宠物，自动关闭宠物面板");
		return;
	end

	local pageFont	= getglobal( "PetFrameListBackGroundPageFont" ); 
	local nPage		= tonumber( pageFont:GetText() );
	local petInfo;
	local nPetIndex	= -1;
	-- 若当前页宠物列表还有宠物，则显示当前页第一个宠物
	if petPackage:getPetNum() > ( nPage - 1 ) * MAX_PET_ONE_PAGE then
		nPetIndex = ( nPage - 1 ) * MAX_PET_ONE_PAGE;
		petInfo = petPackage:getPetInfo( nPetIndex );	
	else
		pageFont:SetText( nPage - 1 );
		nPetIndex = ( nPage - 2 ) * MAX_PET_ONE_PAGE;
		petInfo = petPackage:getPetInfo( nPetIndex );	
	end

	-- 当宠物被丢弃后总是默认刷新最后次显示的那页的第一个宠物
	local btn	= getglobal( "PetFrame_ListButton1" );
	btn:Checked();
	t_mainSelPetInfo = { heightId = petInfo.WID.nHeight, lowId = petInfo.WID.nLow, id = petInfo.PetID, index = nPetIndex };
	
	if PetFrame:IsShown() then
		UpdatePeFrame_PetList();
		ShowPetInfo( petInfo, "PetFrame", true );
	end
	
	UpdatePetPhotoWhenDropPet( { selectPetIndex = nPetIndex, oldSelectPetIndex = nOldSelectPetIndex } );
end

UI_TYPE_PET_WASH_GENIUS		= 27;	-- /* 宠物幻化 */
UI_TYPE_PET_STRENGTHEN		= 28; 	-- /* 宠物强化 */
UI_TYPE_PET_ADD_ATTR		= 29;	-- /* 宠物增强 */
UI_TYPE_PET_SKILL_LEARN		= 30;	-- /* 宠物学习技能 */
UI_TYPE_PET_SKILL_UP		= 31;	-- /* 宠物升级技能 */
UI_TYPE_PET_ADD_SKILL_GRID	= 32;	-- /* 宠物技能栏扩充 */
UI_TYPE_PET_WASH_SKILL_GRID = 33;	-- /* 宠物技能栏改造 */
UI_TYPE_PET_SKILL_RESET		= 65;	-- /* 宠物技能重置 */

local t_openFrames = 
					{
					[UI_TYPE_PET_WASH_GENIUS]		= { frame = "PetGrowResetFrame",				npcId = 0 },	
					[UI_TYPE_PET_STRENGTHEN]		= { frame = "Pet_DevelopmentBuildUpFrame",	npcId = 0 },
					[UI_TYPE_PET_SKILL_UP]			= { frame = "PetSkillLevelUpFrame",			npcId = 0 },
					[UI_TYPE_PET_SKILL_RESET]		= { frame = "PetSkillResetFrame",			npcId = 0 },
					--[UI_TYPE_PET_ADD_ATTR]			= { frame = "Pet_ZengQiangFrame",			npcId = 0 },	
					--[UI_TYPE_PET_SKILL_LEARN]		= { frame = "Pet_SkillLearnFrame",			npcId = 0 },
					--[UI_TYPE_PET_ADD_SKILL_GRID]	= { frame = "Pet_SkillGridExtendFrame",		npcId = 0 },
					--[UI_TYPE_PET_WASH_SKILL_GRID]	= { frame = "Pet_SkillGridRebuildFrame",	npcId = 0 }
					};
OP_DIST = 400;
-- 用于打开宠物操作界面
local t_petOperationMutexFrame = { "Pet_SkillLearnFrame", };
function openPetOperationUI( nUIType, nNpcId )
	local mainplayer = ActorMgr:getMainPlayer();
	local petPackage = mainplayer:getPet();
	if petPackage:getPetNum() == 0 then
		ShowMidTips( "身上没有宠物，不能进行该操作" );
		return;
	end
	
	for _, name in ipairs( t_petOperationMutexFrame ) do
		local frame = getglobal( name ); 
		if frame:IsShown() then
			frame:Hide();
			break;
		end
	end

	for _, data in pairs( t_openFrames ) do
		local frame = getglobal( data.frame ); 
		if frame:IsShown() then
			frame:Hide();
			break;
		end
	end

	local destFrame = getglobal( t_openFrames[nUIType].frame );
	t_openFrames[nUIType].npcId = nNpcId;
	destFrame:Show();
end

function Pet_OperationFrame_OnUpdate()
	local npc;
	for _, data in pairs( t_openFrames ) do
		if data.frame == this:GetName() then
			if data.npcId ~= 0 then
				npc  = ActorMgr:FindActor( data.npcId );
				break;
			end
			
		end
	end

	if npc == nil then
		this:Hide();
		return;
	end

	local nDist = util.CalActorsDistance( npc, ActorMgr:getMainPlayer() );
	if nDist > ( NpcDialog:getMaxNpcTalkDist( npc ) / 100 ) ^ 2 then
		this:Hide();
	end
end

local t_singletonFrames = 
{
	-- 幻化面板
	["Pet_XiDianFrame"]				= 
										{
											fonts = {	["Pet_XiDianFrame_PetNameFont"] = {}, 
														["Pet_XiDianFrame_GeniusFont"] = {}, 
														["Pet_XiDianFrame_CostBullFont"] = {},
														["Pet_XiDianFrame_PetListBackGroundHeadFont"] = {}, 
													},
											--riches = { ["Pet_XiDianFrame_PetNameRich"] = {}, },
											
											textures =	{ ["Pet_XiDianFrame_PhotoTex"] = { defaultOp = function () Pet_XiDianFrame_PhotoTex:Hide() end												 }, 
														["Pet_XiDianFrame_IconBtnIcon"] = { defaultOp = 
														function () 
															local tex = getglobal( "Pet_XiDianFrame_IconBtnIcon" ); 
															tex:SetTexture( "uires\\ZhuJieMian\\1.dds" );
															tex:SetTexUV( 749, 472, 44, 44 );
														end	},
														},
											buttons =	{ 
															--["Pet_XiDianFrame_IconBtn"] = {} 
														},
											checkButtons = {},
										},
	-- 增强面板
	["Pet_ZengQiangFrame"]			= 
										{
											fonts = { ["Pet_ZengQiangFrame_PetNameFont"] = {}, ["Pet_ZengQiangFrame_PetListBackGroundHeadFont"] = {}, },
											textures =	{ ["Pet_ZengQiangFrame_PhotoTex"] = { defaultOp = function () Pet_ZengQiangFrame_PhotoTex:Hide() end												 }, 
														["Pet_ZengQiangFrame_IconBtnIcon"] = { defaultOp = 
														function () 
															local tex = getglobal( "Pet_ZengQiangFrame_IconBtnIcon" ); 
															tex:SetTexture( "uires\\ZhuJieMian\\1.dds" );
															tex:SetTexUV( 749, 472, 44, 44 );
														end	},
														},
											buttons =	{ 
															--["Pet_ZengQiangFrame_IconBtn"] = {} 
														},
											checkButtons = {},
										},
	-- 成长强化面板
	["Pet_DevelopmentBuildUpFrame"] = 
										{
											fonts = {	["Pet_DevelopmentBuildUpFrame_PetNameFont"]					= {},
														["Pet_DevelopmentBuildUpFrame_CostBullShowFont"]			= {},
														["Pet_DevelopmentBuildUpFrame_PetListBackGroundHeadFont"]	= {},
														["Pet_DevelopmentBuildUpFrame_CostCaiLiaoBtnFont"]			= {},
														--["Pet_DevelopmentBuildUpFrame_ChangeCaiLiaoBtnFont"]		= { value = 0, },
													},
											textures =	{ ["Pet_DevelopmentBuildUpFrame_PhotoTex"] =	{ defaultOp = function ()	Pet_DevelopmentBuildUpFrame_PhotoTex:Hide() end	}, 
														["Pet_DevelopmentBuildUpFrame_IconBtnIcon"] = { defaultOp = 
														function () 
															local tex = getglobal( "Pet_DevelopmentBuildUpFrame_IconBtnIcon" ); 
															tex:SetTexture( "uires\\ZhuJieMian\\1.dds" );
															tex:SetTexUV( 749, 472, 44, 44 );
														end	},
														},
											buttons =	{ 
															--["Pet_DevelopmentBuildUpFrame_IconBtn"] = {} 
														},
											checkButtons =	{
															},
											
											showIcons = { ["Pet_DevelopmentBuildUpFrame_ChangeCaiLiaoBtnFont"] = {}, },
										},
	-- 技能学习面板
	["Pet_SkillLearnFrame"]			= 
										{
											fonts = {	["Pet_SkillLearnFrame_PetNameFont"] = {}, 
														["Pet_SkillLearnFrame_CostBullFont"] = {}, 
														["Pet_SkillLearnFrame_PetListBackGroundHeadFont"] = {}, 
													},
											textures =	{ ["Pet_SkillLearnFrame_PhotoTex"] = { defaultOp = 
															function () Pet_SkillLearnFrame_PhotoTex:Hide() end	 }, 
														["Pet_SkillLearnFrame_IconBtnIcon"] = { defaultOp = 
														function () 
															local tex = getglobal( "Pet_SkillLearnFrame_IconBtnIcon" ); 
															tex:SetTexture( "uires\\ZhuJieMian\\1.dds" );
															tex:SetTexUV( 749, 472, 44, 44 );
														end	},
														},
											buttons =	{ 
															--["Pet_SkillLearnFrame_IconBtn"] = {} 
														},
											checkButtons =	{},
										},
	-- 技能升级面板
	["Pet_SkillLevelUpFrame"]		= 
										{
											fonts = {	["Pet_SkillLevelUpFrame_PetNameFont"] = {}, 
														["Pet_SkillLevelUpFrame_PetListBackGroundHeadFont"] = {},
														["Pet_SkillLevelUpFrame_CostBullFont"] = {}, },
											textures =	{ ["Pet_SkillLevelUpFrame_PhotoTex"] = { defaultOp = 
															function () Pet_SkillLevelUpFrame_PhotoTex:Hide() end  }, 
														["Pet_SkillLevelUpFrame_IconBtnIcon"] = { defaultOp = 
														function () 
															local tex = getglobal( "Pet_SkillLevelUpFrame_IconBtnIcon" ); 
															tex:SetTexture( "uires\\ZhuJieMian\\1.dds" );
															tex:SetTexUV( 749, 472, 44, 44 );
														end	},
														},
											buttons =	{ 
															--["Pet_SkillLevelUpFrame_IconBtn"] = {} 
														},
											checkButtons =	{},
										},
	-- 技能格扩充面板
	["Pet_SkillGridExtendFrame"]	= 
										{
											fonts = {	["Pet_SkillGridExtendFrame_PetNameFont"] = {}, 
														["Pet_SkillGridExtendFrame_CostBullFont"] = {}, 
														["Pet_SkillGridExtendFrame_PetListBackGroundHeadFont"] = {}, 
													},
											textures =	{ ["Pet_SkillGridExtendFrame_PhotoTex"] = { defaultOp = 
															function () Pet_SkillGridExtendFrame_PhotoTex:Hide() end	}, 
														["Pet_SkillGridExtendFrame_IconBtnIcon"] = { defaultOp = 
														function () 
															local tex = getglobal( "Pet_SkillGridExtendFrame_IconBtnIcon" ); 
															tex:SetTexture( "uires\\ZhuJieMian\\1.dds" );
															tex:SetTexUV( 749, 472, 44, 44 );
														end	},
														},
											buttons =	{ 
															--["Pet_SkillGridExtendFrame_IconBtn"] = {} 
														},
											checkButtons =	{},
										},
	-- 技能格改造面板
	["Pet_SkillGridRebuildFrame"]	= 
										{
											fonts = {	["Pet_SkillGridRebuildFrame_PetNameFont"] = {}, 
														["Pet_SkillGridRebuildFrame_CostBullFont"] = {}, 
														["Pet_SkillGridRebuildFrame_PetListBackGroundHeadFont"] = {}, 
													},
											textures =	{ ["Pet_SkillGridRebuildFrame_PhotoTex"] = { defaultOp = 
															function () Pet_SkillGridRebuildFrame_PhotoTex:Hide() end												 }, 
														["Pet_SkillGridRebuildFrame_IconBtnIcon"] = { defaultOp = 
														function () 
															local tex = getglobal( "Pet_SkillGridRebuildFrame_IconBtnIcon" ); 
															tex:SetTexture( "uires\\ZhuJieMian\\1.dds" );
															tex:SetTexUV( 749, 472, 44, 44 );
														end	},
														},
											buttons =	{ 
															--["Pet_SkillGridRebuildFrame_IconBtn"] = {} 
														},
											checkButtons =	{},
										},
	-- 宠物进阶面板
	["PetStepUpFrame"] = 
										{
											fonts = {	["PetStepUpFrame_PetNameFont"]					= {},
														["PetStepUpFrame_CostBullShowFont"]				= {},
														["PetStepUpFrame_PetListBackGroundHeadFont"]	= {},
														["PetStepUpFrame_CostCaiLiaoBtnFont"]			= {},
													},
											textures ={ 	["PetStepUpFrame_PhotoTex"] =	{ defaultOp = 
															function ()		
																PetStepUpFrame_PhotoTex:Hide() 
															end								}, 
															["PetStepUpFrame_IconBtnIcon"] = { defaultOp = 
															function () 
																local tex = getglobal( "PetStepUpFrame_IconBtnIcon" ); 
																tex:SetTexture( "uires\\ZhuJieMian\\1.dds" );
																tex:SetTexUV( 749, 472, 44, 44 );
															end								},
														},
											buttons =	{},
											checkButtons =	{},
										},
	-- TODO: 暂时不做 -- 繁殖面板
	--["Pet_CloneFrame"] = {},
};

-- 被选中进行操作的宠物
local t_operationSelPetInfo = { heightId = 0, lowId = 0, id = 0, index = -1 };
function SetOperationSelPetInfo( data )
	t_operationSelPetInfo["heightId"]	= data["heightId"];
	t_operationSelPetInfo["lowId"]		= data["lowId"];
	t_operationSelPetInfo["id"]			= data["id"];
	t_operationSelPetInfo["index"]		= data["index"];
end

function GetOperationSelPetInfo( )
	return t_operationSelPetInfo;
end

-- 被选中的使用道具
local t_selUseItem	= { id = 0, selItem = nil, listType = 0, gridx = 0, num = 0 };
-- 被选中的技能栏位
local t_selSkill	= { id = 0, lv = 0, gridx = -1, };

function SetOperationSelUseItem( data )
	t_selUseItem["id"]			= data["id"];
	t_selUseItem["selItem"]		= data["selItem"];
	t_selUseItem["listType"]	= data["listType"];
	t_selUseItem["gridx"]		= data["gridx"];
	t_selUseItem["num"]			= data["num"];
end

function GetOperationSelUseItem( )
	return t_selUseItem;
end

function SetOperationSelSkill( data )
	t_selSkill["id"]	= data["id"];
	t_selSkill["lv"]	= data["lv"];
	t_selSkill["gridx"]	= data["gridx"];
end

function GetOperationSelSkill( )
	return t_selSkill;
end

function initPetOperationFrameData()
	t_operationSelPetInfo = { heightId = 0, lowId = 0, id = 0, index = -1 };
	t_selSkill		= { id = 0, lv = 0, gridx = -1, };
	t_selUseItem	= { id = 0, selItem = nil, listType = 0, gridx = 0, num = 0 };
end

local t_PetOperationFrameXinShouGuideHuiGun = { ["Pet_SkillLearnFrame"] = {}, ["Pet_DevelopmentBuildUpFrame"] = {}, };
t_PetOperationFrameXinShouGuideHuiGun["Pet_SkillLearnFrame"].func = 
function ()
	if GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_PETSKILLFRAME_LEARN_SKILL_BTN ) then
		return;
	end
	
	if GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_NPCTALK_LEARN_PET_SKILL_OPTION ) then
		GameWizard:undoStep( GAMEWIZARD_FIRST_BLINK_NPCTALK_LEARN_PET_SKILL_OPTION );
	end
	
	if GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_CONTAINER_LEARN_PET_SKILL_BOOK_BTN ) then
		GameWizard:undoStep( GAMEWIZARD_FIRST_BLINK_CONTAINER_LEARN_PET_SKILL_BOOK_BTN );
	end

	if GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_CONTAINER_LEARN_PET_SKILL_BOOK_BTN ) then
		GameWizard:undoStep( GAMEWIZARD_FIRST_BLINK_CONTAINER_LEARN_PET_SKILL_BOOK_BTN );
	end
end

t_PetOperationFrameXinShouGuideHuiGun["Pet_DevelopmentBuildUpFrame"].func = 
function ()
	if GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_PESTRENGTHENFRAME_STRENGTHEN_BTN ) then
		return;
	end
end

local t_PetOperationFrameHideControl = { ["Pet_SkillLearnFrame"] = {}, ["Pet_DevelopmentBuildUpFrame"] = {}, ["Pet_SkillGridRebuildFrame"] = {}, };
t_PetOperationFrameHideControl["Pet_SkillLearnFrame"].func = 
function ()
	for i = 1, MAX_PET_SKILL_GRID do
		local animationTexture = getglobal( "Pet_SkillLearnFrame_SkillButton"..i.."UVAnimationTex" );
		animationTexture:Hide();
	end
	
	local t_hideIcons = { "Pet_SkillLearnFrame_StudyBtnUVAnimationTex", "Pet_SkillLearnFrameXinShouBtn" };
	util.HideIcons( t_hideIcons );
end

t_PetOperationFrameHideControl["Pet_DevelopmentBuildUpFrame"].func = 
function ()
	local t_hideIcons = { "Pet_DevelopmentBuildUpFrameXinShouBtn", "Pet_DevelopmentBuildUpFrame_QiangHuaBtnUVAnimationTex",
							"PetStrengthenConfirmStarBackFrame", };
	util.HideIcons( t_hideIcons );

	local t_petStrengthenControl = GetPetStrengthenControl();
	t_petStrengthenControl:clearCurWaitSavePetInfo();
	HideStrengSaveFrame();

	for i = 1, MAX_STRENGTH_LEVEL do
		local tex = getglobal( "Pet_DevelopmentBuildUpFrame_QiangHuaSmallStarBtn"..i.."Tex" );
		tex:SetBlendAlpha( 1 );
	end
end

t_PetOperationFrameHideControl["Pet_SkillGridRebuildFrame"].func = 
function ()
	local t_hideIcons = { "ResetExistSkillConfirmFrame", };
	util.HideIcons( t_hideIcons );
end

function Pet_OperationFrame_OnHide()
	-- 清空面板打开的对应NPC信息
	for key, _ in pairs( t_openFrames ) do
		t_openFrames[key]["npcId"] = 0;
		-- 若已有道具被选定，则替换掉原来的道具
		if t_selUseItem["id"] ~= 0 then
			ClearItemLock( this:GetName() );
		end
	end

	local modelview = getglobal( this:GetName().."_IconBtnModelView" );
	if modelview ~= nil then
		modelview:Hide();
	end

	local t_huiGunXinShouGuide = t_PetOperationFrameXinShouGuideHuiGun[this:GetName()];
	if t_huiGunXinShouGuide ~= nil then
		t_huiGunXinShouGuide.func();
	end
	
	local t_petOperationFrameHide = t_PetOperationFrameHideControl[this:GetName()];
	if t_petOperationFrameHide ~= nil then
		t_petOperationFrameHide.func();
	end
end

-- 通用操作函数
-- 宠物操作面板的向上滚动按钮
function Pet_OperationFrame_ScrollUpBtn_OnClick()
	local slider = getglobal( this:GetParent() .. "_ScrollBar" );
	if not slider:IsShown() then
		return;
	end

	local nValue = slider:GetValue() - slider:GetValueStep();
	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end

PET_SVR_ACT_WASH_GENIUS		= 0; 
PET_SVR_ACT_ADD_SKILLGRID	= 1;
PET_SVR_ACT_SKILL_LEARN		= 2; 
PET_SVR_ACT_SKILL_UP		= 3;
PET_SVR_ACT_WASH_GRID		= 4; 

local t_opRelativeFrame = 
{ 
	[PET_SVR_ACT_WASH_GENIUS]	= { frame = "PetGrowResetFrame", }, 
	[PET_SVR_ACT_ADD_SKILLGRID] = { frame = "Pet_SkillGridExtendFrame", }, 
	[PET_SVR_ACT_SKILL_LEARN]	= { frame = "PetSkillResetFrame", }, 
	[PET_SVR_ACT_SKILL_UP]		= { frame = "PetSkillLevelUpFrame", }, 
	[PET_SVR_ACT_WASH_GRID]		= { frame = "Pet_SkillGridRebuildFrame", }, 
};

function SuccessPetOperation( nOpType )
	local name = t_opRelativeFrame[nOpType]["frame"];
	if name == nil then
		return;
	end

	local frame = getglobal( name );
	if frame == nil then
		return;
	end

	if not frame:IsShown() then
		return;
	end

	local modelview = getglobal( frame:GetName().."ModelView" );
	modelview:setPetModel(0,0,"effect\\entity\\fumo01.ent");
	--effect\\entity\\cwsj_01.ent
	modelview:setEntityPosition(0,0,100,0);
	if not modelview:IsShown() then
		modelview:Show();
	end
end

local function PetStrengthenWrap(nPetHeighID, nPetLowID, nIncrAttrVal, nStrthenLevel)
	requestStrengthen = nil;
	if nIncrAttrVal == 0 then
		return
	end
	ShowMidTips( "宠物强化成功，请根据需要保存属性" );
	local modelview = getglobal( "Pet_DevelopmentBuildUpFrame_IconBtnModelView" );
	modelview:setPetModel(0,0,"effect\\entity\\fumo01.ent");
	modelview:setEntityPosition(0,0,100,0);
	if not modelview:IsShown() then
		modelview:Show();
	end
	
	
	if t_operationSelPetInfo["heightId"] ~= nPetHeighID or t_operationSelPetInfo["lowId"] ~= nPetLowID then
		return;
	end
	
	local t_petStrengthenControl = GetPetStrengthenControl();
	t_petStrengthenControl:addCurWaitSavePetInfo( { ["incrAttrVal"] = nIncrAttrVal, ["heightId"] = nPetHeighID, ["lowId"] = nPetLowID,
													["level"] = nStrthenLevel } );
	ShowStrengSaveFrame();
	t_petStrengthenControl:setCurStrengthenLevel( { ["level"] = math.min( CLIENT_MAX_PET_STRENGTHEN_LEVEL, nStrthenLevel ) } );
	
	t_petStrengthenControl:setCurSelLevel( { ["level"] = t_petStrengthenControl:getCurStrengthenLevel() } );
	if nIncrAttrVal < MAX_INCR_ATTR_PERCENT then
		local player = ActorMgr:getMainPlayer()
		local petMgr = player:getPet()
		local info = petMgr:getPetInfo(nPetHeighID, nPetLowID)
		local nIncrAttr = petMgr:getStrthenVal( info, nStrthenLevel - 1 )
		if nIncrAttr < nIncrAttrVal then
			ShowMidTips( "当前获得属性比原属性高，已自动为您保存" )		
			Pet_DevelopmentBuildUpFrameSaveBtn_OnClick()
		end
	end
	ShowContinueStrthenBtn();
	UpdatePet_DevelopmentBuildUpFrameStars();
	if nIncrAttrVal == MAX_INCR_ATTR_PERCENT then
		local player = ActorMgr:getMainPlayer()
		local petMgr = player:getPet()
		local info = petMgr:getPetInfo(nPetHeighID, nPetLowID)
		if MessageBoxBlock("强化确认", string.format("#cFFFFBE您的%s已经完成一次完美强化，是否保存？", info.Name), Pet_DevelopmentBuildUpFrame, function() print(t_operationSelPetInfo["heightId"], t_operationSelPetInfo["lowId"], nPetHeighID, nPetLowID) return t_operationSelPetInfo["heightId"] == nPetHeighID and t_operationSelPetInfo["lowId"] == nPetLowID end) then
			Pet_DevelopmentBuildUpFrameSaveBtn_OnClick()
		end
	end
end

function PetStrengthenRes( nPetHeighID, nPetLowID, nIncrAttrVal, nStrthenLevel )
	coroutine.wrap(PetStrengthenWrap)(nPetHeighID, nPetLowID, nIncrAttrVal, nStrthenLevel)
end

-- 宠物操作面板的向下滚动按钮
function Pet_OperationFrame_ScrollDownBtn_OnClick()
	local slider	= getglobal( this:GetParent() .. "_ScrollBar" );
	if not slider:IsShown() then
		return;
	end

	local nMaxValue = slider:GetMaxValue();
	local nValue	= slider:GetValue() + slider:GetValueStep();

	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

-- 宠物操作面板的滑动条滚动的时候响应
function Pet_OperationFrame_ScrollBar_OnValueChanged()
	local slider	 = getglobal( this:GetParent() .. "_ScrollBar" );
	local nValue	 = slider:GetValue();
	local nMaxValue	 = slider:GetMaxValue();

	if nValue <= nMaxValue then
		UpdateOperationPetList( this:GetParent() )
		if this:GetParent() == "PetStepUpFrame" then 
			UpdatePetBattleFlagIcon( "PetStepUpFrame" );
		elseif  this:GetParent() == "PetImpUpFrame" then 
			UpdatePetBattleFlagIcon( "PetImpUpFrame" );
		end
	end	
end

-- 操作面板共用的清空界面信息的方法
local t_extraOp = { ["Pet_DevelopmentBuildUpFrame"] = {} };
t_extraOp["Pet_DevelopmentBuildUpFrame"].func = 
function ()
	for i = 1, CLIENT_MAX_PET_STRENGTHEN_LEVEL do
		local btn = getglobal( "Pet_DevelopmentBuildUpFrame_QiangHuaSmallStarBtn"..i );
		btn:Hide();

		btn = getglobal( "Pet_DevelopmentBuildUpFrame_QiangHuaBigStarBtn"..i );
		btn:Hide();
	end

	HideStrengSaveFrame();

	local t_showIcons = { "Pet_DevelopmentBuildUpFrame_QiangHuaBtn", };
	util.ShowIcons( t_showIcons );
end

function ClearPet_OperationFrame( szFrameName )
	local t_icons = t_singletonFrames[szFrameName];
	if t_icons == nil then 
		return
	end
	for name, data in pairs( t_icons.fonts ) do
		local font = getglobal( name );
		font:SetText( data["value"] or "" );
	end

	for _, data in pairs( t_icons.textures ) do
		data.defaultOp();
	end

	for name, _ in pairs( t_icons.buttons ) do
		local btn = getglobal( name );
		btn:DisChecked();
		btn:Disable();
	end

	for name, _ in pairs( t_icons.checkButtons ) do
		local chkBtn = getglobal( name );
		chkBtn:setCheckState( false );
		--chkBtn:Disable();
	end

	local t_edites = t_icons["edits"];
	if t_edites ~= nil then
		for name, _ in pairs( t_edites ) do
			local edit = getglobal( name );
			edit:SetText( "" );
		end
	end

	local t_showIcons = t_icons["showIcons"];
	if t_showIcons ~= nil then
		for name, _ in pairs( t_showIcons ) do
			local icon = getglobal( name );
			icon:Show( );
		end
	end

	local t_hideIcons = t_icons["hideIcons"];
	if t_hideIcons ~= nil then
		for name, _ in pairs( t_hideIcons ) do
			local icon = getglobal( name );
			icon:Hide( );
		end
	end

	local t_riches = t_icons["riches"];
	if t_riches ~= nil then
		for name, _ in pairs( t_riches ) do
			local rich = getglobal( name );
			rich:Clear( );
		end
	end

	local extraConfig = t_extraOp[szFrameName];
	if extraConfig == nil then
		return;
	end

	local processFunc = extraConfig.func;
	if processFunc == nil then
		return;
	end

	processFunc();
end

-- 根据操作类型选择对应的item result值
-- 此为能将道具放在对应操作面板的前置条件 
local t_itemFrames = 
					{	
						-- 幻化 
						["Pet_XiDianFrame"]				= { itemPre = { resultId = { RESULT_PET_WASH_GENIUS }, },	petPre = {} },
						-- 增强属性
						
						--[[["Pet_ZengQiangFrame"]			= { itemPre = { resultId = { RESULT_PET_ADD_ATTR, }, },		
															petPre = {} },
						--]]
						-- 强化成长
						["Pet_DevelopmentBuildUpFrame"]	= { itemPre = { resultId = { RESULT_PET_STRENGTHEN_STUFF }, },petPre = {} },
						
						-- 宠物学习技能 备注：升级技能根据素质表查soneid1来找到对应的itemId
						["Pet_SkillLearnFrame"]			= { itemPre = { resultId = { RESULT_PET_SKILL_BOOK }, },	petPre = {} },
						
						-- 宠物进阶
						["PetStepUpFrame"]				= { itemPre = { itemId = { PET_STEP_UP_USE_ID_TOLUA }, } ,	petPre = {} },
						
						-- 宠物升级技能
						--[[["Pet_SkillLevelUpFrame"]			= { itemPre = { resultId = { RESULT_PET_SKILL_UP_STUFF }, }, petPre = { }, },
						--]]
						-- 宠物技能格扩充
						--[[["Pet_SkillGridExtendFrame"]	= { itemPre = { resultId = { RESULT_ADD_SKILL_GRID }, },	petPre = {} },
						--]]
						-- 宠物技能格改造,优先消耗绑定道具，然后是非绑定道具
						--[[["Pet_SkillGridRebuildFrame"]	= { itemPre = { resultId = { RESULT_PET_WASH_SKILL_GRID }, },petPre = {} },--]]
						
						-- 宠物解绑
						["PetUnBindFrame"]				= { itemPre = { itemId = { PET_UNBIND_ITEM_ID_TOLUA }, } ,	petPre = {} },
					};

function GetPetItemFramesConfig()
	return t_itemFrames;
end

-- 宠物进阶面板道具检查
t_itemFrames["PetStepUpFrame"].check = 
function ( itemDef )
	for _, resultId in ipairs( t_itemFrames["PetStepUpFrame"].itemPre.itemId ) do
		if resultId == itemDef.ItemID then
			return true;
		end
	end

	ShowMidTips( "该道具不能用于进阶操作,请换个道具" );
	return false;
end
-- 宠物进阶面板道具检查
t_itemFrames["PetUnBindFrame"].check = 
function ( itemDef )
	for _, resultId in ipairs( t_itemFrames["PetUnBindFrame"].itemPre.itemId ) do
		if resultId == itemDef.ItemID then
			return true;
		end
	end

	ShowMidTips( "该道具不能用于解绑操作,请换个道具" );
	return false;
end

-- 幻化面板接收道具的检查
t_itemFrames["Pet_XiDianFrame"].check = 
function ( itemDef )
	for _, resultId in ipairs( t_itemFrames["Pet_XiDianFrame"].itemPre.resultId ) do
		if resultId == itemDef.Result[0].ResultID then
			return true;
		end
	end

	ShowMidTips( "该道具不能用于幻化操作,请换个道具" );
	return false;
end
--[[
-- 宠物增强面板接收道具的检查
t_itemFrames["Pet_ZengQiangFrame"].check = 
function ( itemDef )
	for _, resultId in ipairs( t_itemFrames["Pet_ZengQiangFrame"].itemPre.resultId ) do
		if resultId == itemDef.Result[0].ResultID then
			return true;
		end
	end

	ShowMidTips( "该道具不能用于增强宠物属性,请换个道具" );
	return false;
end
--]]

-- 宠物强化面板接收道具的检查
NORMAL_QIANG_HUA_SHI		= 0;
SHEN_SHENG_QIANG_HUA_SHI	= 1;
t_itemFrames["Pet_DevelopmentBuildUpFrame"].check = 
function ( itemDef, petInfo )
	local mainplayer			= ActorMgr:getMainPlayer();
	local petPackage			= mainplayer:getPet();
	local t_petStrengthenControl= GetPetStrengthenControl();
	local nDestStrenthenLevel	= t_petStrengthenControl:getCurStrengthenLevel();
	nDestStrenthenLevel = (nDestStrenthenLevel > 0) and nDestStrenthenLevel or 1;
	if nDestStrenthenLevel == 0 then
		ShowMidTips( "没有选定要强化的星级" );
		return;
	end

	if GamePet:isPetStrenthenItem( nDestStrenthenLevel, itemDef.ItemID ) then
		return true;
	end

	local petStrengthenDef	= getPetStrengthenDef( nDestStrenthenLevel );
	if petStrengthenDef == nil then
		ShowMidTips( "宠物已经达到最高强化等级，无法强化" );
		return;
	end

	local nDestItemID =  petStrengthenDef.UseItemID;
	
	local bIsStrengthenItem		= false;
	local nItemStrengthenLevel	= 0;
	for i = 1, MAX_STRENGTH_LEVEL do
		if GamePet:isPetStrenthenItem( i, itemDef.ItemID ) then
			bIsStrengthenItem		= true;
			nItemStrengthenLevel	= i;
			break;
		end
	end
	
	local destItemDef;
	if nDestItemID ~= 0  then
		destItemDef = getItemDef( nDestItemID );
	end

	if destItemDef == nil then
		ShowMidTips( "该道具无法强化宠物，请换个" );
	else
		if bIsStrengthenItem and nItemStrengthenLevel < nDestStrenthenLevel then
			ShowMidTips( "强化当前宠物需要更高级的强化材料"..destItemDef.Name );
		elseif bIsStrengthenItem and nItemStrengthenLevel > nDestStrenthenLevel then
			ShowMidTips( "强化当前宠物需要更低级的强化材料"..destItemDef.Name );
		else
			ShowMidTips( "强化当前宠物需要强化材料"..destItemDef.Name );
		end
	end
	
	return false;
end

function GetPetStrongUpItem()
	local mainplayer			= ActorMgr:getMainPlayer();
	local petPackage			= mainplayer:getPet();
	local t_petStrengthenControl= GetPetStrengthenControl();
	local nDestStrenthenLevel	= t_petStrengthenControl:getCurStrengthenLevel();

	local petStrengthenDef	= getPetStrengthenDef( nDestStrenthenLevel );
	if petStrengthenDef == nil then
		petStrengthenDef = getPetStrengthenDef( 1 );
	end

	return petStrengthenDef.UseItemID;
end

-- 宠物学习技能 备注：升级技能根据素质表查soneid1来找到对应的itemId
t_itemFrames["Pet_SkillLearnFrame"].check = 
function ( itemDef )
	for _, resultId in ipairs( t_itemFrames["Pet_SkillLearnFrame"].itemPre.resultId ) do
		if resultId == itemDef.Result[0].ResultID then
			local mainplayer		= ActorMgr:getMainPlayer();
			local petPackage		= mainplayer:getPet();
			local petInfo			= petPackage:getPetInfo( t_operationSelPetInfo["index"] );
			local itemDiathesisDef	= SkillMgr:getDiathesisDef( itemDef.Result[0].ResultVal1, 1 );
			--[[
			for i = 1, petInfo.PetSkills.SkillGridNum do
				local diathInfo = petInfo.PetSkills.SkillGrid[i-1];
				--local diathDef	= SkillMgr:getDiathesisDef( diathInfo.SkillID, diathInfo.SkillLevel );
				-- diathDef == nil and 
				if itemDiathesisDef.PetDiathesisType == diathInfo.GridType then
					return true;
				end
			end

			ShowMidTips( "选定宠物没有对应类型技能格子" );
			return false
			--]]

			return true;
		end
	end

	ShowMidTips( "该道具不能学习技能,请换个道具" );
	return false;
end

--[[
-- 宠物升级技能
t_itemFrames["Pet_SkillLevelUpFrame"].check = 
function ( itemDef )
	for _, resultId in ipairs( t_itemFrames["Pet_SkillLevelUpFrame"].itemPre.resultId ) do
		if resultId == itemDef.Result[0].ResultID then
			return true;
		end
	end
	
	ShowMidTips( "该道具不能用于升级技能,请换个道具" );
	return false;
end

-- 宠物技能格扩充
t_itemFrames["Pet_SkillGridExtendFrame"].check = 
function ( itemDef )
	for _, resultId in ipairs( t_itemFrames["Pet_SkillGridExtendFrame"].itemPre.resultId ) do
		if resultId == itemDef.Result[0].ResultID then
			return true;
		end
	end

	ShowMidTips( "该道具不能用于扩充技能格,请换个道具" );
	return false;
end

-- 宠物技能格改造,优先消耗绑定道具，然后是非绑定道具
t_itemFrames["Pet_SkillGridRebuildFrame"].check = 
function ( itemDef )
	if ResetExistSkillConfirmFrame:IsShown() then
		ShowMidTips( "请先完成确认操作" );
		return false;
	end

	for _, resultId in ipairs( t_itemFrames["Pet_SkillGridRebuildFrame"].itemPre.resultId ) do
		if resultId == itemDef.Result[0].ResultID then
			return true;
		end
	end

	ShowMidTips( "该道具不能用于改造技能格,请换个道具" );
	return false;
end
--]]

-- 当宠物操作面板的道具变化的时候，相关的变化
t_itemFrames["Pet_DevelopmentBuildUpFrame"].changeItem = 
function ( )
	local t_fonts;
	-- or t_selUseItem["id"] == 0
	if t_operationSelPetInfo["id"] == 0 then
		 t_fonts = 
					{
					["Pet_DevelopmentBuildUpFrame_CostBullShowFont"]			= { value = "" },
					["Pet_DevelopmentBuildUpFrame_CostCaiLiaoBtnFont"]			= { value = "" },
					["Pet_DevelopmentBuildUpFrame_ChangeCaiLiaoBtnFont"]		= { value = "" },
					};
		util.SetFonts( t_fonts );
		return;
	end
	
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	if t_selUseItem["id"] ~= 0 then
		local item			= container:getItem( t_selUseItem["listType"], t_selUseItem["gridx"] );
		t_selUseItem["num"] = math.min( item:getNum(), 1 );
	end
	

	-- 判断宠物的状态
	local petPackage	= mainplayer:getPet();
	local petInfo		= petPackage:getPetInfo( t_operationSelPetInfo["index"] );
	
	local t_petStrengthenControl	= GetPetStrengthenControl();
	local nCurStrengthenLevel		= t_petStrengthenControl:getCurStrengthenLevel();
	nCurStrengthenLevel = (nCurStrengthenLevel > 0) and nCurStrengthenLevel or 1;
	local petStrengthenDef			= getPetStrengthenDef( nCurStrengthenLevel);
	local nPetStrengthenMoney		= getPetStrengthenCost( nCurStrengthenLevel);
	
	local t_fonts = 
	{
		["Pet_DevelopmentBuildUpFrame_CostBullShowFont"]	= { value = nPetStrengthenMoney },

		
	};
	util.SetFonts( t_fonts );
	
	UpdateUseCaiLiaoNum();
	-- 是否是第一次操作强化宠物
	-- 先检查背包	Quest:isQuestRun( 3134 )
	-- 使用了宠物强化后要将背包对应的特效隐藏
	-- TODO: 与策划确认是否要绑定任务ID 
	if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_CONTAINER_STRENGTHEN_ITEM_BTN ) and 
		GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_NPCTALK_STRENGTHEN_PET_OPTION ) then
		
		local effectFrame = getglobal( "PlayerContainerFrameEffectFrame" );
		effectFrame:Hide();
		GameWizard:doStep( GAMEWIZARD_FIRST_BLINK_CONTAINER_STRENGTHEN_ITEM_BTN );
		ShowPetDevelopmentBuildUpFrameXinShouGuide();
	end
end

-- 学习面板的道具发生变化的时候
t_itemFrames["Pet_SkillLearnFrame"].changeItem = 
function ( )
	local diathesisDef;
	if t_selUseItem["id"] ~= 0 then
		local bookItemDef	= getItemDef( t_selUseItem.id );
		diathesisDef = SkillMgr:getDiathesisDef( bookItemDef.Result[0].ResultVal1, 1 );
	end
	
	local t_fonts = { ["Pet_SkillLearnFrame_CostBullFont"] = { value = diathesisDef ~= nil and diathesisDef.Learn.MoneyVal or "" } };
	for name, data in pairs( t_fonts ) do
		local font = getglobal( name );
		font:SetText( data.value );
	end
	
	
	-- 是否是第一次操作学习宠物技能
	-- 先检查背包	Quest:isQuestRun( 3134 )
	-- 使用了宠物技能学习书后要将背包对应的特效隐藏
	-- TODO: 与策划确认是否要绑定任务ID 
	if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_CONTAINER_LEARN_PET_SKILL_BOOK_BTN ) and 
		GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_NPCTALK_LEARN_PET_SKILL_OPTION ) then
		local effectFrame = getglobal( "PlayerContainerFrameEffectFrame" );
		effectFrame:Hide();
		GameWizard:doStep( GAMEWIZARD_FIRST_BLINK_CONTAINER_LEARN_PET_SKILL_BOOK_BTN );
		ShowPetSkillFrameXinShouGuide();
	end
end
-- 当宠物操作面板的道具变化的时候，相关的变化
t_itemFrames["PetStepUpFrame"].changeItem = 
function ( )
	local t_fonts;
	if t_operationSelPetInfo["id"] == 0 then
		 t_fonts = {
					["PetStepUpFrame_CostBullShowFont"]			= { value = "" },
					["PetStepUpFrame_CostCaiLiaoBtnFont"]		= { value = "" },
					};
		util.SetFonts( t_fonts );
		return;
	end
	
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	if t_selUseItem["id"] ~= 0 then
		local item			= container:getItem( t_selUseItem["listType"], t_selUseItem["gridx"] );
		t_selUseItem["num"] = math.min( item:getNum(), 1 );
	end
	local useDef 		= GameArmProce:getUseDef( USE_ID_PET_STEP_UP_TOLUA );
	local nNeedMoney 	= ( useDef == nil ) and 0 or useDef.UseMoney;
	t_fonts = 
	{
		["PetStepUpFrame_CostBullShowFont"]	= { value = nNeedMoney },	
	};
	util.SetFonts( t_fonts );
	UpdatePetStepUpItemNum();
end
-- 当宠物操作面板的道具变化的时候，相关的变化
t_itemFrames["PetUnBindFrame"].changeItem = 
function ( )
	local t_fonts;
	if t_operationSelPetInfo["id"] == 0 then
		 t_fonts = {
					["PetUnBindFrame_CostBullShowFont"]			= { value = "" },
					["PetUnBindFrame_CostCaiLiaoBtnFont"]		= { value = "" },
					};
		util.SetFonts( t_fonts );
		return;
	end
	
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	if t_selUseItem["id"] ~= 0 then
		local item			= container:getItem( t_selUseItem["listType"], t_selUseItem["gridx"] );
		t_selUseItem["num"] = math.min( item:getNum(), 1 );
	end
	local petPackage			= mainplayer:getPet();
	local petInfo				= petPackage:getPetInfo( t_operationSelPetInfo["heightId"], t_operationSelPetInfo["lowId"] );

	local nNeedMoney 	= 0;
	if t_PetUnBindNeed[petInfo.PetGenius] ~= nil then
		nNeedMoney = t_PetUnBindNeed[petInfo.PetGenius];
	end
	t_fonts = 
	{
		["PetUnBindFrame_CostBullShowFont"]	= { value = nNeedMoney },	
	};
	util.SetFonts( t_fonts );
	UpdatePetUnBindItemNum();
end

-- 清空道具图标
function ClearPet_OperationItem()
	local szFrameName;
	for name, _ in pairs( t_itemFrames ) do
		local frame = getglobal( name );
		if frame~= nil and frame:IsShown() then
			szFrameName = name;
			break;
		end
	end
	
	ClearItemLock( szFrameName );
	t_singletonFrames[szFrameName].textures[szFrameName .. "_IconBtnIcon"].defaultOp();
	t_selUseItem = { id = 0, selItem = nil, listType = 0, gridx = 0, num = 0 };
	-- 道具更换后改变道具对应的显示内容
	if t_itemFrames[szFrameName].changeItem ~= nil then
		t_itemFrames[szFrameName].changeItem();
	end
end

-- 拖曳道具图标
function Pet_OperationFrame_ItemButton_Drag()
	-- 若没道具则不能进行拖曳操作
	if t_selUseItem["id"] == 0 then
		return;
	end
	
	local dragDta		= UIMSG:getUIGV();
	dragDta.DragFrom	= DRAG_PETOPERATION;
	local tex = getglobal( this:GetName() .. "Icon" );
	UIBeginDrag( tex:GetTexture(), t_selUseItem["id"], 0, 0 );
end

function Save2Pet_OperationFrame( item, nSrcList, nSrcGridIndex, szFrameName )
	local itemdef = item:getItemDef();

	-- 若该物品不是给宠物用的，则也不接收，给个中央提示
	if itemdef.UseTarget ~= USE_TARGET_PET then
		if szFrameName == "Pet_DevelopmentBuildUpFrame" then
			ShowMidTips( "该物品不能进行宠物强化操作，请换个道具" );
		elseif szFrameName == "PetStepUpFrame" then 
			ShowMidTips( "该物品不能进行宠物进阶操作，请换个道具" );
		elseif szFrameName == "PetImpUpFrame" then 
			ShowMidTips( "该物品不能进行宠物进化操作，请换个道具" );
		elseif szFrameName == "PetUnBindFrame" then
			ShowMidTips( "该物品不能进行宠物解绑操作，请换个道具" );
		else
			ShowMidTips( "该物品宠物不能使用，请换个道具" );
		end
		
		return;
	end

	if t_operationSelPetInfo["id"] == 0 then
		ShowMidTips( "没有选定宠物，不能放物品" );
		return;
	end
	if szFrameName == "PetImpUpFrame" then 
		ChangePetImpUpItem( itemdef.ItemID )
		return;
	end
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local petInfo		= petPackage:getPetInfo( t_operationSelPetInfo["index"] );
	if not t_itemFrames[szFrameName].check( itemdef, petInfo ) then
		return;
	end

	-- 若已有道具被选定，则替换掉原来的道具
	if t_selUseItem["id"] ~= 0 then
		ClearPet_OperationItem();
	end

	-- 设置图标
	local tex = getglobal( szFrameName .. "_IconBtnIcon" );
	local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
	tex:SetTexture( ( IsInExistence( IconPath ) and IconPath or DEFAULT_ITEM_PATH ) );

	tex:SetTexRelUV( 1, 1 );
	t_selUseItem = { selItem = item, id = item:getItemId(), listType = item:getListType(), gridx = item:getGridIdx() };

	LockItem( item:getListType(), item:getGridIdx(), item:getItemId(), szFrameName );

	-- 道具更换后改变道具对应的显示内容
	if t_itemFrames[szFrameName].changeItem ~= nil then
		t_itemFrames[szFrameName].changeItem();
	end
end

-- 接收道具图标
function Pet_OperationFrame_ItemButton_ReceiveDrag()
	if t_operationSelPetInfo["id"] == 0 then
		ShowMidTips( "没有选定宠物，不能放物品" );
		return;
	end

	-- 是否只能从背包拖过来
	local nSrcListType	= UIMSG:getUIGV().DragFrom;	
	local nSrcGrid		= UIGetDragUserData1();
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local item			= container:getItem( nSrcListType, nSrcGrid );
	
	Save2Pet_OperationFrame( item, nSrcListType, nSrcGrid, this:GetParent() );
	UIEndDrag();
	UIMSG:getUIGV().DragFrom = DRAG_NONE;
end

-- 鼠标移入道具图标
function Pet_OperationFrame_ItemButton_OnEnter()
	if t_selUseItem["id"] == 0 then
		SetGameTooltips( this:GetName(), "请放入基因果实" );
		return;
	end

	ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, getItemDef( t_selUseItem["id"] ) );
end

-- 鼠标离开道具图标
function Pet_OperationFrame_ItemButton_OnLeave()
	NormalItemTipsFrame:Hide();
end

local t_changePetItemPreCondition = { ["Pet_SkillGridRebuildFrame"] = {}, };
t_changePetItemPreCondition["Pet_SkillGridRebuildFrame"].func =
function ( data )
	if ResetExistSkillConfirmFrame:IsShown() then
		ShowMidTips( "请先完成确认操作" );
		return false;
	end

	return true;
end

function Pet_OperationFrame_ItemButton_OnClick()
	if arg1 ~= "RightButton" then
		return;
	end
	
	local changePetItemPreCondition = t_changePetItemPreCondition[this:GetParent()];
	if changePetItemPreCondition ~= nil then
		if not changePetItemPreCondition.func() then
			return;
		end
	end

	ClearPet_OperationItem();
end

-- 点击输入数量按钮
function Pet_DevelopmentBuildUpFrame_ChangeCaiLiaoBtn_OnClick()
	if t_selUseItem["id"] == 0 then
		return;
	end
	
	local font = getglobal( "Pet_DevelopmentBuildUpFrame_ChangeCaiLiaoBtnFont" );
	font:Hide();
end

function Pet_DevelopmentBuildUpFrame_ChangeCaiLiaoBtn_OnEnter()
	--[[
	local szText = "点击输入宠物强化道具数量，最大数量为10";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
					button = this:GetName(), vertical = "bottom" } );--]]
end

function UpdateUseCaiLiaoNum()
	if t_selUseItem["id"] == 0 then
		local t_fonts = 
			{
				-- item:getNum()
				["Pet_DevelopmentBuildUpFrame_CostCaiLiaoBtnFont"]			= {	value = "" },
			};
		util.SetFonts( t_fonts );
		return;
	end
	
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local item			= container:getItem( t_selUseItem["listType"], t_selUseItem["gridx"] );
	local nTotalItemNum = container:getItemTotalNum( item:getItemId() );

	local petPackage				= mainplayer:getPet();
	local petInfo					= petPackage:getPetInfo( t_operationSelPetInfo["index"] );
	local t_petStrengthenControl	= GetPetStrengthenControl();
	local nCurStrengthenLevel		= t_petStrengthenControl:getCurStrengthenLevel();
	nCurStrengthenLevel = (nCurStrengthenLevel > 0) and nCurStrengthenLevel or 1;
	local petStrengthenDef			= getPetStrengthenDef( nCurStrengthenLevel);

	local t_fonts = 
			{
				-- item:getNum()
				["Pet_DevelopmentBuildUpFrame_CostCaiLiaoBtnFont"]			= {	value = 
					( petStrengthenDef == nil ) and "" or nTotalItemNum.."/"..petStrengthenDef.UseItemNum },
			};
	util.SetFonts( t_fonts );
end

-- 强化道具数量减少按钮
function Pet_DevelopmentBuildUpFrame_LeftDecreaseBtn_OnClick()
	if t_selUseItem["id"] == 0 then
		return;
	end

	if t_selUseItem["num"] <= 1 then
		return;
	end

	t_selUseItem["num"] = t_selUseItem["num"] - 1;
	UpdateUseCaiLiaoNum();
end

-- 强化道具数量增加按钮
function Pet_DevelopmentBuildUpFrame_RightIncreaseBtn_OnClick()
	if t_selUseItem["id"] == 0 then
		return;
	end
	
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local item			= container:getItem( t_selUseItem["listType"], t_selUseItem["gridx"] );

	local petPackage		= mainplayer:getPet();
	local petInfo			= petPackage:getPetInfo( t_operationSelPetInfo["index"] );
	local t_petStrengthenControl	= GetPetStrengthenControl();
	local nCurStrengthenLevel		= t_petStrengthenControl:getCurStrengthenLevel();
	local petStrengthenDef	= getPetStrengthenDef( nCurStrengthenLevel);
	local nTotalItemNum		= container:getItemTotalNum( item:getItemId() );
	-- item:getNum()
	local nMaxNum			= nTotalItemNum;
	if petStrengthenDef ~= nil then
		nMaxNum			= math.min( nTotalItemNum, petStrengthenDef.UseItemNum );
	end

	if t_selUseItem["num"] >= nMaxNum then
		return;
	end

	t_selUseItem["num"] = t_selUseItem["num"] + 1;
	UpdateUseCaiLiaoNum();
end

-- 清空宠物列表
function ClearOperationPetList( szFrameName )
	for i = 1, MAX_PET_NUM do
		local btn		= getglobal( szFrameName.."_PetNameBtn"..i );
		btn:SetText( "" );
		btn:DisChecked();
		btn:Disable();
	end
end

-- 刷新宠物列表
function UpdateOperationPetList( szFrameName )
	ClearOperationPetList( szFrameName );
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local slider		= getglobal( szFrameName.."_ScrollBar" );
	local nStartIndex	= 0;
	if slider:IsShown() then
		nStartIndex = slider:GetValue();
	else
		nStartIndex = 0;
	end

	local nButtonIndex	= 1;
	-- 选定原来的被选定的宠物列表按钮
	for i = nStartIndex + 1, petPackage:getPetNum() do
		if nButtonIndex > MAX_PET_NUM then
			break;
		end

		local btn		= getglobal( szFrameName.."_PetNameBtn"..nButtonIndex );
		local petInfo	= petPackage:getPetInfo( i - 1 );	 
		local petDef	= getPetDef( petInfo.PetID );
		btn:SetText( petInfo.Name ~= "" and petInfo.Name or petDef.Name );
		nButtonIndex = nButtonIndex + 1;
		btn:Enable();
		-- 检查当前显示的宠物是否是被选定的宠物
		if t_operationSelPetInfo["heightId"] == petInfo.WID.nHeight and t_operationSelPetInfo["lowId"] == petInfo.WID.nLow then
			btn:Checked();
		end
	end
	
	-- 检查选定的宠物是否还在
	for i = 1, petPackage:getPetNum() do
		local petInfo = petPackage:getPetInfo( i - 1 );	
		if t_operationSelPetInfo["heightId"] == petInfo.WID.nHeight and t_operationSelPetInfo["lowId"] == petInfo.WID.nLow then
			t_operationSelPetInfo["index"] = i - 1;
			return;
		end
	end
	-- 执行到这说明原来被选定的宠物已经不在,清空选定的宠物信息
	t_operationSelPetInfo = { heightId = 0, lowId = 0, id = 0, index = -1 };
	if petPackage:getPetNum() == 0 then
		return;
	end

	-- 若主角背包里面有宠物，则选定当前面板的第一只宠物
	local petInfo	= petPackage:getPetInfo( nStartIndex );	
	local petDef	= getPetDef( petInfo.PetID );
	t_operationSelPetInfo = { heightId = petInfo.WID.nHeight, lowId = petInfo.WID.nLow, id = petInfo.PetID, index = nStartIndex };
	local btn		= getglobal( szFrameName.."_PetNameBtn1" );
	btn:Checked();

	if szFrameName == "Pet_DevelopmentBuildUpFrame" then
		local t_petStrengthenControl = GetPetStrengthenControl();
		t_petStrengthenControl:setFirstNotStrengthenLevel( { ["petInfo"] = petInfo } );
	end
end

-- 点击宠物操作面板的宠物列表名字按钮
local t_changePetPreCondition = { ["Pet_SkillGridRebuildFrame"] = {}, };
t_changePetPreCondition["Pet_SkillGridRebuildFrame"].func =
function ()
	if ResetExistSkillConfirmFrame:IsShown() then
		ShowMidTips( "请先完成确认操作" );
		return false;
	end

	return true;
end

local t_changePetPostCondition = { ["Pet_DevelopmentBuildUpFrame"] = {}, ["PetStepUpFrame"] = {}, };
t_changePetPostCondition["Pet_DevelopmentBuildUpFrame"].func =
function ()
	HideStrengSaveFrame();
	ShowInitStrthenBtn();
	
	for i = 1, CLIENT_MAX_PET_STRENGTHEN_LEVEL do
		local tex = getglobal( "Pet_DevelopmentBuildUpFrame_QiangHuaSmallStarBtn"..i.."Tex" );
		tex:SetBlendAlpha( 1 );
	end

	local t_petStrengthenControl= GetPetStrengthenControl();
	local mainplayer			= ActorMgr:getMainPlayer();
	local petPackage			= mainplayer:getPet();
	local petInfo				= petPackage:getPetInfo( t_operationSelPetInfo["heightId"], t_operationSelPetInfo["lowId"] );
	t_petStrengthenControl:setFirstNotStrengthenLevel( { ["petInfo"] = petInfo } );
	if t_petStrengthenControl:isOneWaitSavePetInfo( t_operationSelPetInfo ) then
		ShowStrengSaveFrame();
	end
	
	t_mainSelPetInfo = t_operationSelPetInfo;
	if PetFrame:IsShown() then
		UpdatePetFrame();
	end
end
t_changePetPostCondition["PetStepUpFrame"].func =
function ()
	local mainplayer			= ActorMgr:getMainPlayer();
	local petPackage			= mainplayer:getPet();
	local petInfo				= petPackage:getPetInfo( t_operationSelPetInfo["heightId"], t_operationSelPetInfo["lowId"] );

	t_mainSelPetInfo = t_operationSelPetInfo;
	if PetFrame:IsShown() then
		UpdatePetFrame();
	end
end
function PetFrame_SelectPet(parent, idx)
	DisCheckAllBtn( parent.."_PetNameBtn", MAX_PET_NUM );
	local slider		= getglobal( parent .. "_ScrollBar" );
	local nStartIndex	= 0;
	if slider:IsShown() then
		nStartIndex	= slider:GetValue();
	end
	local id = idx - nStartIndex + 1
	if id > 0 and id <= MAX_PET_NUM then
		local btn = getglobal(parent.."_PetNameBtn"..id)
		btn:Checked()
	end

	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local petInfo		= petPackage:getPetInfo( idx );  
	t_operationSelPetInfo = { heightId = petInfo.WID.nHeight, lowId = petInfo.WID.nLow, id = petInfo.PetID, index = idx };
	
	local changePetPostCondition = t_changePetPostCondition[parent];
	if changePetPostCondition ~= nil then
		changePetPostCondition.func();
	end
	SetOperationSelSkill( { id = 0, lv = 0, gridx = -1, } );
	SetPetInfoShow( parent );
end

function Pet_Frame_PetNameBtn_OnClick()
	local changePetPreCondition = t_changePetPreCondition[this:GetParent()];
	if changePetPreCondition ~= nil then
		if not changePetPreCondition.func() then
			return;
		end
	end

	if this:IsChecked() then
		return;
	end
	local parent = getglobal(this:GetParent())
	if parent and IsMessageBoxBlock(parent) then
		return;
	end
	
	local slider		= getglobal( this:GetParent() .. "_ScrollBar" );
	local nStartIndex	= 0;
	if slider:IsShown() then
		nStartIndex	= slider:GetValue();
	end
	PetFrame_SelectPet(this:GetParent(), this:GetClientID() - 1 + nStartIndex)
end

function DefaultShow( szFrameName )
	ClearPet_OperationFrame( szFrameName );
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local petPackage = mainplayer:getPet();

	t_operationSelPetInfo = { heightId = 0, lowId = 0, id = 0, index = -1 };
	local slider = getglobal( szFrameName .. "_ScrollBar" );
	slider:SetValue( 0 );
	-- 设置滚动条
	SetScrollBar( szFrameName .. "_ScrollBar", MAX_PET_NUM, petPackage:getPetNum() );
	-- 若没宠物，则清空面板显示的内容
	if petPackage:getPetNum() == 0 then
		return;
	end

	-- 刷新宠物列表
	UpdateOperationPetList( szFrameName );
	-- 若有宠物，默认选中第一个
	-- TODO: 根据clientID选取宠物可能会出错，绑定index
	local nStartIndex	= slider:GetValue();
	local btn	= getglobal( szFrameName.."_PetNameBtn1" );
	btn:Checked();
	local nPetIndex = btn:GetClientID() - 1 + nStartIndex;
	local petInfo	= petPackage:getPetInfo( nPetIndex );	
	local petDef	= getPetDef( petInfo.PetID );
	t_operationSelPetInfo = { heightId = petInfo.WID.nHeight, lowId = petInfo.WID.nLow, id = petInfo.PetID, index = nPetIndex };

	local frame = getglobal( szFrameName );
	frame:SetPoint( "center", "$parent", "center", -70, 0  );
end


-- 更新技能栏位显示
function UpdatePetOperationSkillGrids( szFrameName, petInfo )
	-- 设置宠物已有的技能
	DisCheckAllBtn( szFrameName.."_SkillButton", MAX_PET_SKILL_GRID );
	local skillGrids	= petInfo.PetSkills.SkillGrid;
	local nLockedNum	= MAX_LOCK_SKILL_GRID - petInfo.PetSkills.AddSkillGridNum;
	for i = 1, petInfo.PetSkills.SkillGridNum do
		local diathesisDef	= SkillMgr:getDiathesisDef( skillGrids[i-1].SkillID, skillGrids[i-1].SkillLevel );
		local btn			= getglobal( szFrameName.."_SkillButton"..i );
		local tex			= getglobal( btn:GetName().."Icon" );
		local font			= getglobal( btn:GetName().."Level" );
		if diathesisDef ~= nil then
			local IconPath = getPetSkillPath()..diathesisDef.IconID..".tga";
			tex:SetTexture( ( IsInExistence( IconPath ) and IconPath or DEFAULT_ITEM_PATH ) );
			font:SetText("lv"..diathesisDef.Level)
		else
			--tex:SetTexture( t_skillGridTga[skillGrids[i-1].GridType].path );
			tex:SetTexture( t_skillGridTga[PET_SKILL_SELF_BUFF].path );
			font:SetText("")
		end
		tex:SetTexRelUV( 1, 1 );
		btn:SetClientUserData( 0, skillGrids[i-1].SkillID );
		btn:SetClientUserData( 1, skillGrids[i-1].SkillLevel );
		btn:SetClientUserData( 2, skillGrids[i-1].GridType );
		btn:SetClientUserData( 3, PET_SKILL_SELF_BUFF );
		btn:Enable();

		if t_selSkill["gridx"] == ( i - 1 ) then
			--btn:Checked();
		end
	end

	for i = petInfo.PetSkills.SkillGridNum + 1, MAX_PET_SKILL_GRID do
		local btn		= getglobal( szFrameName.."_SkillButton"..i );
		local tex		= getglobal( btn:GetName().."Icon" );
		local font		= getglobal( btn:GetName().."Level" );
		tex:SetTexture( "uires\\ZhuJieMian\\1.dds" );
		tex:SetTexUV( 749, 517, 44,44 ); 
		font:SetText("")
		btn:DisChecked();
		btn:SetClientUserData( 3, PET_SKILL_DISABLE );
		btn:Disable();
	end

	-- 默认选中第一个格子
	--local btn = getglobal( szFrameName.."_SkillButton1" );
	--btn:Checked();
	--t_selSkill	= { id = 0, lv = 0, gridx = 0, };
end


-- 此为改变选定的宠物时候改变显示的宠物信息内容
local t_petInfoShow = 
						{
							["Pet_XiDianFrame"]		= { },	["Pet_ZengQiangFrame"]	= { },		["Pet_DevelopmentBuildUpFrame"] = { },
							["Pet_SkillLearnFrame"] = { },	["Pet_SkillLevelUpFrame"]	= { },	["Pet_SkillGridExtendFrame"]	= { },
							["Pet_SkillGridRebuildFrame"] = { },["PetStepUpFrame"] = { },["PetImpUpFrame"] = {},["PetUnBindFrame"] = {},
						};
-- 幻化面板
t_petInfoShow["Pet_XiDianFrame"].func = 
function ( petInfo )
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local petDef		= getPetDef( petInfo.PetID );
	local t_fonts = 
		{
			["Pet_XiDianFrame_PetNameFont"] = { value = ( petInfo.Name ~= "" and petInfo.Name or petDef.Name ) },

			-- color = t_geniusDesc[petInfo.PetGenius]["color"] },
			["Pet_XiDianFrame_GeniusFont"] = {	value = "  "..t_geniusDesc[petInfo.PetGenius]["value"], 
												color = t_geniusDesc[petInfo.PetGenius]["color"]  },
															
			["Pet_XiDianFrame_PetListBackGroundHeadFont"] = { value = "宠物列表"..petPackage:getPetNum().."/".. petPackage:getPetMaxNum() },
			["Pet_XiDianFrame_CostBullFont"] = { value = t_geniusDesc[petInfo.PetGenius]["money"] },
		};
	util.SetFonts( t_fonts );
	
	local nameFont = getglobal( "Pet_XiDianFrame_PetNameFont" );
	-- * GetScreenScaleY()
	local nOffsetX = nameFont:GetTextExtentWidth( nameFont:GetText() ) / GetScreenScaleY() + 5;
	local t_tgas = 
			{
				["Pet_XiDianFrame_PhotoTex"] = { path = GetMonsterIconPath( petDef.IconID ) },
			};
	for name, data in pairs( t_tgas ) do
		local tex = getglobal( name );
		tex:SetTexture( data.path );
		tex:SetTexRelUV( 1, 1 );
		tex:Show();
	end
end

-- 增强面板
t_petInfoShow["Pet_ZengQiangFrame"].func =
function ( petInfo )
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local petDef		= getPetDef( petInfo.PetID );
	local t_fonts = 
		{
			["Pet_ZengQiangFrame_PetNameFont"] = { value = ( petInfo.Name ~= "" and petInfo.Name or petDef.Name )..( petInfo.StrengthenLevel > 0 
															and " ( +"..petInfo.StrengthenLevel.." )" or "" )  },
			["Pet_ZengQiangFrame_PetListBackGroundHeadFont"] = { value = "宠物列表"..petPackage:getPetNum().."/".. petPackage:getPetMaxNum() },
		};
	
	for name, data in pairs( t_fonts ) do
		local font = getglobal( name );
		font:SetText( data.value );
	end

	local t_tgas = 
			{
				["Pet_ZengQiangFrame_PhotoTex"] = { path = GetMonsterIconPath( petDef.IconID ) },
			};
	for name, data in pairs( t_tgas ) do
		local tex = getglobal( name );
		tex:SetTexture( data.path );
		tex:SetTexRelUV( 1, 1 );
		tex:Show();
	end
end

-- 强化面板
t_petInfoShow["Pet_DevelopmentBuildUpFrame"].func =
function ( petInfo ) 
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local petDef		= getPetDef( petInfo.PetID );
	local petInfo		= petPackage:getPetInfo( t_operationSelPetInfo["index"] );
	
	local t_petStrengthenControl	= GetPetStrengthenControl();
	local nCurStrengthenLevel		= t_petStrengthenControl:getCurStrengthenLevel();
	nCurStrengthenLevel = (nCurStrengthenLevel > 0) and nCurStrengthenLevel or 1;
	local petStrengthenDef			= getPetStrengthenDef( nCurStrengthenLevel );
	local isMaxStrengthenLevel		= isMaxStrengthenLevel( nCurStrengthenLevel );
	local nPetStrengthenMoney		= getPetStrengthenCost( nCurStrengthenLevel );
	
	local container		= mainplayer:getContainer();
	local item;
	if t_selUseItem["id"] ~= 0 then
		item = container:getItem( t_selUseItem["listType"], t_selUseItem["gridx"] );
	end

	local t_fonts = 
		{	
			["Pet_DevelopmentBuildUpFrame_PetNameFont"] = { value = ( petInfo.Name ~= "" and petInfo.Name or petDef.Name )  },
			["Pet_DevelopmentBuildUpFrame_PetListBackGroundHeadFont"] = { value = "宠物列表"..petPackage:getPetNum().."/".. 
																					petPackage:getPetMaxNum() },

			["Pet_DevelopmentBuildUpFrame_CostBullShowFont"] = 
			{ value = nPetStrengthenMoney },

			["Pet_DevelopmentBuildUpFrame_CostCaiLiaoBtnFont"] = 
			{ value = ( ( t_selUseItem["id"] == 0 or petStrengthenDef == nil ) and "" or 
						( container:getItemTotalNum( item:getItemId() ) .."/"..petStrengthenDef.UseItemNum  ) ) },
		};
	
	util.SetFonts( t_fonts );

	local t_tgas = 
			{
				["Pet_DevelopmentBuildUpFrame_PhotoTex"] = { path = GetMonsterIconPath( petDef.IconID ) },
			};
	for name, data in pairs( t_tgas ) do
		local tex = getglobal( name );
		tex:SetTexture( data.path );
		tex:SetTexRelUV( 1, 1 );
		tex:Show();
	end
	
	local t_textures = 
	{
		["PetAutoRecoverFrame_ItemGridBtn1BoxTexture"] = {	path = "uires\\ZhuJieMian\\1.dds", UV = { x = 244, y = 756, width = 33, height = 33 },
											},

		["PetAutoRecoverFrame_ItemGridBtn2BoxTexture"] = {	path = "uires\\ZhuJieMian\\1.dds", UV = { x = 244, y = 756, width = 33, height = 33 },
											},
			
		["PetAutoRecoverFrame_ItemGridBtn3BoxTexture"] = {	path = "uires\\ZhuJieMian\\1.dds", UV = { x = 279, y = 756, width = 33, height = 33 },
											},
	};
	util.SetTextures( t_textures );
	
	UpdatePet_DevelopmentBuildUpFrameStars();
	ShowPetDevelopmentBuildUpFrameXinShouGuide();
	ValidateStrengSaveFrame();
	Change_Pet_DevelopmentBuildUpFrameFrameStrengBtnState();
	Validate_Pet_DevelopmentBuildUpFrame_StrengthenItem();
	local bool, func = IsMessageBoxBlock(Pet_DevelopmentBuildUpFrame)
	if bool then
		if not func or not func() then
			MessageBoxBlockEnd(Pet_DevelopmentBuildUpFrame)
		end
	end
end

-- 技能学习
t_petInfoShow["Pet_SkillLearnFrame"].func =
function ( petInfo ) 
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local petDef		= getPetDef( petInfo.PetID );
	local diathesisiDef;
	if t_selUseItem["id"] ~= 0 then
		local bookItemDef	= getItemDef( t_selUseItem.id );
		diathesisiDef	= SkillMgr:getDiathesisDef( bookItemDef.Result[0].ResultVal1, 1 );
	end
	local t_fonts = 
		{
			["Pet_SkillLearnFrame_PetNameFont"] = { value = ( petInfo.Name ~= "" and petInfo.Name or petDef.Name ) },
			["Pet_SkillLearnFrame_PetListBackGroundHeadFont"] = { value = "宠物列表"..petPackage:getPetNum().."/".. petPackage:getPetMaxNum() },
			["Pet_SkillLearnFrame_CostBullFont"] = { value = diathesisiDef ~= nil and diathesisiDef.Learn.MoneyVal or "" },
		};
	
	for name, data in pairs( t_fonts ) do
		local font = getglobal( name );
		font:SetText( data.value );
	end

	local t_tgas = 
		{
			["Pet_SkillLearnFrame_PhotoTex"] = { path = GetMonsterIconPath( petDef.IconID ) },
		};

	for name, data in pairs( t_tgas ) do
		local tex = getglobal( name );
		tex:SetTexture( data.path );
		tex:SetTexRelUV( 1, 1 );
		tex:Show();
	end
	
	UpdatePetOperationSkillGrids( "Pet_SkillLearnFrame", petInfo );
	ShowPetSkillFrameXinShouGuide();
end

-- 宠物进阶
t_petInfoShow["PetStepUpFrame"].func =
function ( petInfo ) 
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local petDef		= getPetDef( petInfo.PetID );
	local container		= mainplayer:getContainer();
	local item;
	if t_selUseItem["id"] ~= 0 then
		item = container:getItem( t_selUseItem["listType"], t_selUseItem["gridx"] );
	end
	local useDef 		= GameArmProce:getUseDef( USE_ID_PET_STEP_UP_TOLUA );
	local nNeedMoney 	= ( useDef == nil ) and 0 or useDef.UseMoney;
	local t_fonts = 
		{
			["PetStepUpFrame_PetNameFont"] = { value = ( petInfo.Name ~= "" and petInfo.Name or petDef.Name ) },
			["PetStepUpFrame_PetListBackGroundHeadFont"] = { value = "宠物列表"..petPackage:getPetNum().."/".. petPackage:getPetMaxNum() },
			["PetStepUpFrame_CostBullShowFont"] = { value = nNeedMoney },

			["PetStepUpFrame_CostCaiLiaoBtnFont"] = 
			{ value = ( t_selUseItem["id"] == 0 and "" or ( container:getItemTotalNum( item:getItemId() ) .."/"..( petPackage:getBattlePetNum() + 1 ) * 10  ) ) },
		};
	
	for name, data in pairs( t_fonts ) do
		local font = getglobal( name );
		font:SetText( data.value );
	end
	local t_tgas = 
		{
			["PetStepUpFrame_PhotoTex"] = { path = GetMonsterIconPath( petDef.IconID ) },
		};

	for name, data in pairs( t_tgas ) do
		local tex = getglobal( name );
		tex:SetTexture( data.path );
		tex:SetTexRelUV( 1, 1 );
		tex:Show();
	end
end
-- 宠物解绑
t_petInfoShow["PetUnBindFrame"].func =
function ( petInfo ) 
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local petDef		= getPetDef( petInfo.PetID );
	local container		= mainplayer:getContainer();
	local item;
	if t_selUseItem["id"] ~= 0 then
		item = container:getItem( t_selUseItem["listType"], t_selUseItem["gridx"] );
	end
	local petInfo		= petPackage:getPetInfo( t_operationSelPetInfo["heightId"], t_operationSelPetInfo["lowId"] );
	local nNeedMoney 	= 0;
	if t_PetUnBindNeed[petInfo.PetGenius] ~= nil then
		nNeedMoney = t_PetUnBindNeed[petInfo.PetGenius];
	end
	local t_fonts = 
		{
			["PetUnBindFrame_PetNameFont"] = { value = ( petInfo.Name ~= "" and petInfo.Name or petDef.Name ) },
			["PetUnBindFrame_PetListBackGroundHeadFont"] = { value = "宠物列表"..petPackage:getPetNum().."/".. petPackage:getPetMaxNum() },
			["PetUnBindFrame_CostBullShowFont"] = { value = nNeedMoney },

			["PetUnBindFrame_CostCaiLiaoBtnFont"] = 
			{ value = ( t_selUseItem["id"] == 0 and "" or ( container:getItemTotalNum( item:getItemId() ) .."/"..( petPackage:getBattlePetNum() + 1 ) * 10  ) ) },
		};
	
	for name, data in pairs( t_fonts ) do
		local font = getglobal( name );
		font:SetText( data.value );
	end
	local t_tgas = 
		{
			["PetUnBindFrame_PhotoTex"] = { path = GetMonsterIconPath( petDef.IconID ) },
		};

	for name, data in pairs( t_tgas ) do
		local tex = getglobal( name );
		tex:SetTexture( data.path );
		tex:SetTexRelUV( 1, 1 );
		tex:Show();
	end
end
-- 宠物成长进化
t_petInfoShow["PetImpUpFrame"].func =
function ( petInfo )
	UpdatePetImpUpFrame( 1 );  -- 参数查看原函数
end
local t_petLearnSkillXinShouBtnPosInfo = {	["Pet_SkillLearnFrame_StudyBtn"] = 
											{ selfPoint = "bottomleft", relFrame = "Pet_SkillLearnFrame_StudyBtn", relPoint = "TopLeft", 
														offsetX = 8, offsetY = -20, 
											},
							
										["Pet_SkillLearnFrame_SkillButton"] = 
										{ selfPoint = "bottomleft", relFrame = "Pet_SkillLearnFrame_SkillButton1", relPoint = "TopLeft", 
											offsetX = 8, offsetY = -20, 
										},
									};

-- Pet_SkillLearnFrame_SkillButton1		Pet_SkillLearnFrame_StudyBtn
function Pet_SkillLearnFrame_Test_OnShow()
	--AdjustPetSkillFrameXinShouBtn( { btn = "Pet_SkillLearnFrame_SkillButton1", text = "测试宠物技能学习按钮" } );

	AdjustPetSkillFrameXinShouBtn( { btn = "Pet_SkillLearnFrame_StudyBtn", text = "点击让宠物学习技能" } );
end

function AdjustPetSkillFrameXinShouBtn( data )
	local rich			= getglobal( "Pet_SkillLearnFrameXinShouBtnRich" );
	local xinShouBtn	= getglobal( "Pet_SkillLearnFrameXinShouBtn" );
	local nSelfDefineExtendWidth = 14;
	rich:SetClientString( data["text"] );
	ReSizeTooltip( rich:GetName(), xinShouBtn:GetName(), nSelfDefineExtendWidth );
	local posInfo;
	for name, _ in pairs( t_petLearnSkillXinShouBtnPosInfo ) do
		if string.find( data["btn"], name, 1, true) then
			posInfo = t_petLearnSkillXinShouBtnPosInfo[name];
			break;
		end
	end
	assert( posInfo ~= nil );
	
	xinShouBtn:SetPoint( posInfo["selfPoint"], data["btn"], posInfo["relPoint"], posInfo["offsetX"], posInfo["offsetY"] );
	xinShouBtn:SetClientString( data["btn"] );
	
	local animationTexture = getglobal( data["btn"].."UVAnimationTex" );
	animationTexture:SetUVAnimation( 50, true );
	animationTexture:Show();
	
	xinShouBtn:Show();
end

function ShowPetSkillFrameXinShouGuide()
	-- TODO: 这儿需要策划给个宠物技能学习的任务ID	and Quest:isQuestRun( 1062 )
	local mainplayer			= ActorMgr:getMainPlayer();
	local petPackage			= mainplayer:getPet();
	local t_containerControl	= GetContainerControl();
	if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_NPCTALK_LEARN_PET_SKILL_OPTION ) and mainplayer:getLv() >= 30 and 
		t_containerControl:isHavePetSkillLearnItem() and petPackage:getPetNum() > 0 then
		GameWizard:doStep( GAMEWIZARD_FIRST_BLINK_NPCTALK_LEARN_PET_SKILL_OPTION );
	end
	
	local mainplayer	= ActorMgr:getMainPlayer();
	-- 判断宠物的状态
	local petPackage	= mainplayer:getPet();
	local petInfo		= petPackage:getPetInfo( t_operationSelPetInfo["index"] );
	-- 显示技能格子新手指引，需要有个前提，闪烁的格子必须与放入的技能书匹配，若没找到匹配的则使用第一个默认技能格子
	--[[
	if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_PETSKILLFRAME_SKILL_GRID_BTN ) and 
		GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_CONTAINER_LEARN_PET_SKILL_BOOK_BTN ) and t_selUseItem["id"] ~= 0 then

		local bookItemDef			= getItemDef( t_selUseItem.id );
		local diathesisDef			= SkillMgr:getDiathesisDef( bookItemDef.Result[0].ResultVal1, 1 );
		local selectSkillGridBtn	= getglobal( "Pet_SkillLearnFrame_SkillButton1" );
		for i = 1, MAX_PET_SKILL_GRID do
			local diathesisInfo	= petInfo.PetSkills.SkillGrid[i-1];

			if diathesisDef.PetDiathesisType == diathesisInfo.GridType then
				selectSkillGridBtn = getglobal( "Pet_SkillLearnFrame_SkillButton"..i );
				break;
			end
		end
		AdjustPetSkillFrameXinShouBtn( { btn = selectSkillGridBtn:GetName(), text = "点击宠物学习的技能格" } );
	end
	--]]

	-- 显示学习技能按钮的新手指引
	if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_PETSKILLFRAME_LEARN_SKILL_BTN ) and 
			GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_CONTAINER_LEARN_PET_SKILL_BOOK_BTN ) then
		AdjustPetSkillFrameXinShouBtn( { btn = "Pet_SkillLearnFrame_StudyBtn", text = "点击让宠物学习技能" } );
	end
end

function ShowPetDevelopmentBuildUpFrameXinShouGuide()
	-- 宠物强化新手指引
	-- TODO: 这儿需要策划给个宠物技能学习的任务ID	and Quest:isQuestRun( 1062 )
	local mainplayer			= ActorMgr:getMainPlayer();
	local petPackage			= mainplayer:getPet();
	local t_containerControl	= GetContainerControl();
	if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_NPCTALK_STRENGTHEN_PET_OPTION ) and mainplayer:getLv() >= 30 and 
			t_containerControl:isHavePetDevelopmentBuildUpItem() and petPackage:getPetNum() > 0 then
		GameWizard:doStep( GAMEWIZARD_FIRST_BLINK_NPCTALK_STRENGTHEN_PET_OPTION );
	end

	-- 显示宠物强化按钮的新手指引
	if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_PESTRENGTHENFRAME_STRENGTHEN_BTN ) and 
			GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_CONTAINER_STRENGTHEN_ITEM_BTN ) then
		AdjustPetDevelopmentBuildUpFrameXinShouBtn( { btn = "Pet_DevelopmentBuildUpFrame_QiangHuaBtn", text = "点击进行宠物强化" } );
	end
end

local t_petDevelopmentBuildUpXinShouBtnPosInfo = {	["Pet_DevelopmentBuildUpFrame_QiangHuaBtn"] = 
											{ selfPoint = "bottomleft", relFrame = "Pet_DevelopmentBuildUpFrame_QiangHuaBtn", relPoint = "TopLeft", 
														offsetX = 8, offsetY = -20, 
											},
												};

function Pet_DevelopmentBuildUpFrame_Test_OnShow()
	AdjustPetDevelopmentBuildUpFrameXinShouBtn( { btn = "Pet_DevelopmentBuildUpFrame_QiangHuaBtn", text = "测试宠物强化按钮" } );
end

function AdjustPetDevelopmentBuildUpFrameXinShouBtn( data )
	local rich			= getglobal( "Pet_DevelopmentBuildUpFrameXinShouBtnRich" );
	local xinShouBtn	= getglobal( "Pet_DevelopmentBuildUpFrameXinShouBtn" );
	local nSelfDefineExtendWidth = 14;
	rich:SetClientString( data["text"] );
	ReSizeTooltip( rich:GetName(), xinShouBtn:GetName(), nSelfDefineExtendWidth );
	local posInfo = t_petDevelopmentBuildUpXinShouBtnPosInfo[data["btn"]];
	
	xinShouBtn:SetPoint( posInfo["selfPoint"], posInfo["relFrame"], data["btn"], posInfo["offsetX"], posInfo["offsetY"] );
	xinShouBtn:SetClientString( data["btn"] );
	
	local animationTexture = getglobal( data["btn"].."UVAnimationTex" );
	animationTexture:SetUVAnimation( 50, true );
	animationTexture:Show();
	
	xinShouBtn:Show();
end

function UpdatePet_SkillLevelUpFrameCostBull()
	local szLevelUpSkillText = "";

	-- 若有宠物被选定，则显示相应的信息
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local petInfo		= petPackage:getPetInfo( t_operationSelPetInfo["index"] );

	if t_selSkill["gridx"] ~= -1 then
		local diathesisInfo	= petInfo.PetSkills.SkillGrid[t_selSkill["gridx"]];
		diathesisiDef	= SkillMgr:getDiathesisDef( diathesisInfo.SkillID, diathesisInfo.SkillLevel );
		if diathesisiDef == nil then
			szLevelUpSkillText = "";
		else
			diathesisiDef		= SkillMgr:getDiathesisDef( diathesisInfo.SkillID, diathesisInfo.SkillLevel + 1 );
			szLevelUpSkillText	= diathesisiDef ~= nil and diathesisiDef.Learn.MoneyVal or "0(已经达到最高级)" ;
		end		
	else
		szLevelUpSkillText = "";
	end

	local t_fonts = 
		{
			["Pet_SkillLevelUpFrame_CostBullFont"] = { value = szLevelUpSkillText },
		};
	
	for name, data in pairs( t_fonts ) do
		local font = getglobal( name );
		font:SetText( data.value );
	end
end

-- 技能升级
t_petInfoShow["Pet_SkillLevelUpFrame"].func =
function ( petInfo ) 
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local petDef		= getPetDef( petInfo.PetID );
	local szLevelUpSkillText = "";
	local diathesisiDef;

	if t_selSkill["gridx"] ~= -1 then
		local diathesisInfo	= petInfo.PetSkills.SkillGrid[t_selSkill["gridx"]];
		diathesisiDef	= SkillMgr:getDiathesisDef( diathesisInfo.SkillID, diathesisInfo.SkillLevel );
		if diathesisiDef == nil then
			szLevelUpSkillText = "";
		else
			diathesisiDef	= SkillMgr:getDiathesisDef( diathesisInfo.SkillID, diathesisInfo.SkillLevel + 1 );
			szLevelUpSkillText = diathesisiDef ~= nil and diathesisiDef.Learn.MoneyVal or "0(已经达到最高级)" ;
		end	
	else
		szLevelUpSkillText = "";
	end

	local t_fonts = 
		{
			["Pet_SkillLevelUpFrame_PetNameFont"] = { value = ( petInfo.Name ~= "" and petInfo.Name or petDef.Name )..( petInfo.StrengthenLevel > 0 
																and " ( +"..petInfo.StrengthenLevel.." )" or "" ) },
			["Pet_SkillLevelUpFrame_PetListBackGroundHeadFont"] = { value = "宠物列表"..petPackage:getPetNum().."/".. petPackage:getPetMaxNum() },
			["Pet_SkillLevelUpFrame_CostBullFont"] = { value = szLevelUpSkillText },
		};
	
	for name, data in pairs( t_fonts ) do
		local font = getglobal( name );
		font:SetText( data.value );
	end

	local t_tgas = 
		{
			["Pet_SkillLevelUpFrame_PhotoTex"] = { path = GetMonsterIconPath( petDef.IconID ) },
		};

	for name, data in pairs( t_tgas ) do
		local tex = getglobal( name );
		tex:SetTexture( data.path );
		tex:SetTexRelUV( 1, 1 );
		tex:Show();
	end

	UpdatePetOperationSkillGrids( "Pet_SkillLevelUpFrame", petInfo );
end

-- 宠物技能格扩充
t_petInfoShow["Pet_SkillGridExtendFrame"].func =
function ( petInfo ) 
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local petDef		= getPetDef( petInfo.PetID );
	local t_fonts = 
		{
			["Pet_SkillGridExtendFrame_PetNameFont"] = { value = ( petInfo.Name ~= "" and petInfo.Name or petDef.Name )..
																	( petInfo.StrengthenLevel > 0 and " ( +"..petInfo.StrengthenLevel.." )" or "" ) },
			["Pet_SkillGridExtendFrame_PetListBackGroundHeadFont"] = { value = "宠物列表"..petPackage:getPetNum().."/".. petPackage:getPetMaxNum() },
			["Pet_SkillGridExtendFrame_CostBullFont"] = { value = PET_ADD_SKILL_GRID_MONEY },
		};
	
	for name, data in pairs( t_fonts ) do
		local font = getglobal( name );
		font:SetText( data.value );
	end

	local t_tgas = 
	{
		["Pet_SkillGridExtendFrame_PhotoTex"] = { path = GetMonsterIconPath( petDef.IconID ) },
	};

	for name, data in pairs( t_tgas ) do
		local tex = getglobal( name );
		tex:SetTexture( data.path );
		tex:SetTexRelUV( 1, 1 );
		tex:Show();
	end

	UpdatePetOperationSkillGrids( "Pet_SkillGridExtendFrame", petInfo );
end

-- 宠物技能格改造
t_petInfoShow["Pet_SkillGridRebuildFrame"].func =
function ( petInfo )
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local petDef = getPetDef( petInfo.PetID );
	local t_fonts = 
		{
			["Pet_SkillGridRebuildFrame_PetNameFont"] = { value = ( petInfo.Name ~= "" and petInfo.Name or petDef.Name )..
																	( petInfo.StrengthenLevel > 0 and " ( +"..petInfo.StrengthenLevel.." )" or "" ) },
			["Pet_SkillGridRebuildFrame_PetListBackGroundHeadFont"] = { value = "宠物列表"..petPackage:getPetNum().."/".. petPackage:getPetMaxNum() },
			["Pet_SkillGridRebuildFrame_CostBullFont"] = { value = PET_WASH_SKILL_GRID_MONEY },
		};
	
	for name, data in pairs( t_fonts ) do
		local font = getglobal( name );
		font:SetText( data.value );
	end

	local t_tgas = 
	{
		["Pet_SkillGridRebuildFrame_PhotoTex"] = { path = GetMonsterIconPath( petDef.IconID ) },
	};

	for name, data in pairs( t_tgas ) do
		local tex = getglobal( name );
		tex:SetTexture( data.path );
		tex:SetTexRelUV( 1, 1 );
		tex:Show();
	end

	UpdatePetOperationSkillGrids( "Pet_SkillGridRebuildFrame", petInfo );
end

-- 此为记录鼠标移到查看说明图标时候要显示的内容
local t_seeIntroContent = 
						{
						["Pet_XiDianFrame"] = { desc = "宠物幻化说明：\n1.	宠物幻化后等级回到一级，已学会技能消失，已强化等级消失、已增强的能力消失。\n2.宠物幻化后，成长值、资质和性格都会重新随机产生。\n3.使用弱效重生石，宠物幻化后，资质随机。\n4.使用强效重生石，宠物幻化后，最低资质为优秀。\n5.使用越高等级的重生石，宠物获得的成长值有机会更高。" },
						};

-- 此为仅仅根据选定的宠物改变显示的宠物信息内容
function SetPetInfoShow( szFrameName )
	if t_operationSelPetInfo["id"] == 0 then
		ClearPet_OperationFrame( szFrameName );
		return;
	end
	-- 若有宠物被选定，则显示相应的信息
	local idx = t_operationSelPetInfo["index"];
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local petInfo		= petPackage:getPetInfo(idx);	
	t_operationSelPetInfo = { heightId = petInfo.WID.nHeight, lowId = petInfo.WID.nLow, id = petInfo.PetID, index = idx };
	t_petInfoShow[szFrameName].func( petInfo );
end

local t_operationTips = {
						-- 幻化面板
						["Pet_XiDianFrame"] = { text = "宠物幻化说明：\n1.	宠物幻化后等级回到一级，学会的技能和宠物强化等级不会改变\n2.	宠物幻化后资质会改变，但是不会降低\n3.使用幻化果实进行宠物幻化" },
						-- 增强面板
						["Pet_ZengQiangFrame"] = { text = "宠物增强说明：\n1.每次增强需要消耗一个宠物增强材料\n2.每个属性最多能增强五次" },
						-- 宠物成长强化
						["Pet_DevelopmentBuildUpFrame"] = { text = "宠物强化说明：\n1.宠物幻化后等级回到一级，学会的技能和宠物强化等级不会改变\n2.宠物幻化后资质会改变，但是不会降低\n3.使用幻化石进行宠物幻化" },
						-- 宠物技能学习
						["Pet_SkillLearnFrame"] = { text = "宠物技能学习说明：\n1.学习宠物技能前，需右键点击宠物技能栏位开启技能格\n2.通过宠物技能书学习宠物技能\3.宠物可同时拥有5个技能，后面学习的技能有几率顶替前面所学技能" },
						-- 宠物技能升级
						["Pet_SkillLevelUpFrame"] = { text = "宠物技能升级说明：\n1.技能等级最高为九级\n2.每次升级需要消耗一个宠物技能升级材料\n3.低级技能石：可升级低级技能；高级技能石：可升级高级技能" },
						-- 宠物技能格扩充
						["Pet_SkillGridExtendFrame"] = { text = "宠物技能升级说明：\n1.扩充技能格需要消耗：领悟之石\n2.扩充技能格无需选择技能格" },
						-- 宠物技能格改造
						["Pet_SkillGridRebuildFrame"] = { text = "宠物技能格改造说明：\n1.技能格类型改造需要消耗：遗忘之石\n2.技能格改造后，该技能格类型将随机产生\n3.同类技能格上限为3个\n4.改造已有技能的技能格时，该技能将被遗忘" },
						};
-- 此为显示宠物操作面板的查看说明的tips
-- 此为改变道具变化时候改变响应的显示
function PetOperationSeeIntroIconBtn_OnEnter()
	--[[
	for i = 1, 7 do
		local font = getglobal( "PetOperateSeeIntroTipsFrameFont"..i );
		font:Hide();
	end

	local tipFrame = getglobal( "PetOperateSeeIntroTipsFrame" );
	local t_tips = t_operationTips[this:GetParent()];
	local t_fonts = {};
	for i = 1, table.getn( t_tips ) do
		local font = getglobal( "PetOperateSeeIntroTipsFrameFont"..i );
		font:SetText( t_tips[i] );
		font:SetWidth( GetFontTextWidth( font ) );
		font:Show();
		table.insert( t_fonts, font:GetName() );
	end
	
	tipFrame:SetWidth( util.GetMaxFontWidth( t_fonts ) + 20 );
	tipFrame:SetHeight( 10 + 20 * table.getn( t_tips )  );
	tipFrame:SetClientString( this:GetParent() );
	tipFrame:Show();
	util.AdjustTooltipsScreenPos( tipFrame:GetName() );
	--]]

	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = t_operationTips[this:GetParent()]["text"], 
						frame = this:GetParent(), button = this:GetName() } );
end

function PetOperateSeeIntroTipsFrame_OnUpdate()
	local frame = getglobal( this:GetClientString() );
	if not frame:IsShown() then
		this:Hide();
	end
end

function PetOperateSeeIntroTipsFrame_OnHide()
	this:SetClientString( "" );
end

local t_containerUpdate = { ["Pet_DevelopmentBuildUpFrame"] = {} };
t_containerUpdate["Pet_DevelopmentBuildUpFrame"].func = 
function ()
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local useItem		= container:getItem( t_selUseItem["listType"], t_selUseItem["gridx"] );
	local szFrameName	= "Pet_DevelopmentBuildUpFrame";
	
	if useItem:getNum() > 0 then
		local nTotalNum = container:getItemTotalNum( useItem:getItemId() );
		if nTotalNum < t_selUseItem["num"] then
			t_selUseItem["num"] = nTotalNum;
		end

		UpdateUseCaiLiaoNum();
		return;
	end

	local nOldNum = t_selUseItem["num"];
	ClearItemLock( szFrameName );
	t_singletonFrames[szFrameName].textures[szFrameName .. "_IconBtnIcon"].defaultOp();
	t_selUseItem = { id = 0, selItem = nil, listType = 0, gridx = 0, num = 0 };
	-- 道具更换后改变道具对应的显示内容
	if t_itemFrames[szFrameName].changeItem ~= nil then
		t_itemFrames[szFrameName].changeItem();
	end

	-- 再检查是否有相应的强化道具
	-- 若物品减少且少于原来选定的数量，则刷新变化数量
	local petPackage			= mainplayer:getPet();
	local petInfo				= petPackage:getPetInfo( t_operationSelPetInfo["index"] );
	local t_petStrengthenControl= GetPetStrengthenControl();
	local nCurStrengthenLevel	= t_petStrengthenControl:getCurStrengthenLevel();
	nCurStrengthenLevel = (nCurStrengthenLevel > 0) and nCurStrengthenLevel or 1;
	local petStrengthenDef		= getPetStrengthenDef( nCurStrengthenLevel );
	if petStrengthenDef == nil then
		return;
	end

	local nLockItemID	= 0;
	local nDefItemID	= petStrengthenDef.UseItemID;
	local nItemCount	= container:getItemTotalNum( nDefItemID );
	if nItemCount > 0 then
		nLockItemID = nDefItemID;
	end

	if nLockItemID == 0 then
		return;
	end
	
	local newLockItem		= container:getItem( nLockItemID );
	local newLocalItemdef	= newLockItem:getItemDef();
	-- 设置图标
	local tex = getglobal( szFrameName .. "_IconBtnIcon" );
	local IconPath = GetItemEquipIconPath()..newLocalItemdef.IconID..".tga";
	tex:SetTexture( ( IsInExistence( IconPath ) and IconPath or DEFAULT_ITEM_PATH ) );
	tex:SetTexRelUV( 1, 1 );
	local nNewNum = math.min( nOldNum, petStrengthenDef.UseItemNum, container:getItemTotalNum( nLockItemID ) );
	t_selUseItem = { selItem = newLockItem, id = newLockItem:getItemId(), listType = newLockItem:getListType(), 
						gridx = newLockItem:getGridIdx(), num = nNewNum };

	LockItem( newLockItem:getListType(), newLockItem:getGridIdx(), newLockItemgetItemId, szFrameName );

	UpdateUseCaiLiaoNum();
end

local t_petOperationFrameEvents =	
{	["GE_PET_UPDATE"]	= {}, ["GE_DEL_PET"]		= {}, ["GE_ACTOR_FIGHT_IN"] = {},
	["GE_PET_INFO_CHG"] = {}, ["GE_PET_ATTR_CHG"]	= {}, ["GE_PET_FIGHT_CHG"]	= {},
	["GE_CONTAINER_UPDATE"] = {}, ["GE_ENTER_PLAYERLOGIN"] = {}, ["GE_PET_STRENGTHEN_CHG"] = {},
};

function Pet_OperationFrame_OnLoad()
	t_petOperationFrameEvents.__index = function ( tab, key )
								tab[key] = { func = function ( name ) end };
								return tab[key];
							end
	setmetatable( t_petOperationFrameEvents, t_petOperationFrameEvents );

	for event, _ in pairs( t_petOperationFrameEvents ) do
		this:RegisterEvent( event );
	end
	
	for i = 1, MAX_PET_NUM do
		local btn = getglobal( this:GetName().."_PetNameBtn" .. i );

		t_singletonFrames[this:GetName()].buttons[btn:GetName()] = {};
		t_singletonFrames[this:GetName()].fonts[btn:GetName().."Font"] = {};
	end

	for i = 1, MAX_PET_SKILL_GRID do
		local btn = getglobal( this:GetName().."_SkillButton" .. i );
		if not btn then
			break;
		end

		t_singletonFrames[this:GetName()].buttons[btn:GetName()] = {};
		t_singletonFrames[this:GetName()].textures[btn:GetName().."Icon"] = {};
		t_singletonFrames[this:GetName()].textures[btn:GetName().."Icon"].defaultOp = 
																				function ()
																					local tex = getglobal( btn:GetName().."Icon" ); 
																					local font = getglobal( btn:GetName().."Level" ); 
																					tex:SetTexture( "uires\\ZhuJieMian\\1.dds" );
																					tex:SetTexUV( 749, 472, 44, 44 );
																					font:SetText("")
																				end
	end

	-- 将定义的全局表挂在这儿
	t_skillGridTga.__index = function ( tab, key )
								return { path = [[uires\TuPianLei\ChongWuJiNengLeiXing\1.tga]] };
							end
	setmetatable( t_skillGridTga, t_skillGridTga );

	-- 将定义的全局表挂在这儿
	t_containerUpdate.__index = function ( tab, key )
									tab[key] = {};
									tab[key].func = function () end
									return tab[key];
								end
	setmetatable( t_containerUpdate, t_containerUpdate );
	
	local t_hideIcons = { this:GetName().."_PetNameBtn5PartionTex", };
	util.HideIcons( t_hideIcons );
	local ItemName = getglobal(this:GetName().."_CostNameBtnFont")
	if ItemName then
		local petStrengthenDef = getPetStrengthenDef(1)
		ItemName:SetText(getItemDef(petStrengthenDef.UseItemID).Name)
	end
end

t_petOperationFrameEvents["GE_PET_UPDATE"].func = 
function ( name )
	local frame = getglobal( name );
	if not frame:IsShown() then
		return;
	end

	local mainplayer = ActorMgr:getMainPlayer();
	local petPackage = mainplayer:getPet();
	if petPackage:getPetNum() == 0 then
		ShowMidTips( "身上没宠物，不能进行该操作，自动关闭面板" );
		frame:Hide();
		PetFrame:Hide();
		return;
	end

	UpdateOperationPetList( name );
	SetPetInfoShow( name );
end

t_petOperationFrameEvents["GE_DEL_PET"].func = 
function ( name )
	t_petOperationFrameEvents["GE_PET_UPDATE"].func( name );
end

t_petOperationFrameEvents["GE_PET_INFO_CHG"].func = 
function ( name )
	t_petOperationFrameEvents["GE_PET_UPDATE"].func( name );
end

t_petOperationFrameEvents["GE_ACTOR_FIGHT_IN"].func = 
function ( name )
	t_petOperationFrameEvents["GE_PET_UPDATE"].func( name );
end

t_petOperationFrameEvents["GE_PET_ATTR_CHG"].func = 
function ( name )
	t_petOperationFrameEvents["GE_PET_UPDATE"].func( name );
end

t_petOperationFrameEvents["GE_PET_FIGHT_CHG"].func = 
function ( name )
	t_petOperationFrameEvents["GE_PET_UPDATE"].func( name );
end

t_petOperationFrameEvents["GE_PET_STRENGTHEN_CHG"].func = 
function ( name )
	local frame	= getglobal(name)
	if frame and frame:IsShown() then
		local t_petStrengthenControl= GetPetStrengthenControl();
		local mainplayer			= ActorMgr:getMainPlayer();
		local petPackage			= mainplayer:getPet();
		local petInfo				= petPackage:getPetInfo( t_operationSelPetInfo["heightId"], t_operationSelPetInfo["lowId"] );
		t_petStrengthenControl:setFirstNotStrengthenLevel( { ["petInfo"] = petInfo } );
		t_petOperationFrameEvents["GE_PET_UPDATE"].func( name );
	end
end

t_petOperationFrameEvents["GE_CONTAINER_UPDATE"].func = 
function ( name )
	local frame = getglobal( name );
	if not frame:IsShown() then
		return;
	end

	if name == "Pet_DevelopmentBuildUpFrame" then
		Validate_Pet_DevelopmentBuildUpFrame_StrengthenItem()
	end

	-- 查看是否要更新正在使用的物品
	if t_selUseItem["id"] == 0 then
		return;
	end

	local containerUpdateEvent = t_containerUpdate[name];
	if containerUpdateEvent ~= nil then
		containerUpdateEvent.func();
	end
	
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local useItem		= container:getItem( t_selUseItem["listType"], t_selUseItem["gridx"] );
	if useItem:getNum() > 0 then
		return;
	end

	ClearPet_OperationItem();
end

t_petOperationFrameEvents["GE_ENTER_PLAYERLOGIN"].func = 
function ( name )
	local t_huiGunXinShouGuide = t_PetOperationFrameXinShouGuideHuiGun[name];
	if t_huiGunXinShouGuide ~= nil then
		t_huiGunXinShouGuide.func();
	end
end

function Pet_OperationFrame_OnEvent()
	t_petOperationFrameEvents[arg1].func( this:GetName() );
end
	-- 暂时无用函数
function Pet_OperationFrame_OnShow()
	t_selUseItem			= { id = 0, selItem = nil, listType = 0, gridx = 0, num = 0 };
	t_operationSelPetInfo	= { heightId = 0, lowId = 0, id = 0, index = -1 };
	t_selSkill				= { id = 0, lv = 0, gridx = -1, };
	DefaultShow( this:GetName() );
	SetPetInfoShow( this:GetName() );

	-- 若宠物面板没显示，则随着宠物操作面板一起显示
	--[[
	local frame = getglobal( "PetFrame" );
	if not frame:IsShown() then
		--t_mainSelPetInfo	= { heightId = petInfo.WID.nHeight, lowId = petInfo.WID.nLow, id = petInfo.PetID, index = nPetIndex };
		t_mainSelPetInfo	= t_operationSelPetInfo;
		frame:Show();
		frame:SetPoint( "topleft", this:GetName(), "topright", 0, 0 );
	end
	--]]	
	local conFrame = getglobal( "PlayerContainerFrame" );
	if not conFrame:IsShown() then
		conFrame:Show();
		conFrame:SetPoint( "topleft", this:GetName(), "topright", 0, 0 );
	end

	ChekPet_OperationFrame_XinShou_Guide( this:GetName() );
end

-- 宠物强化面板显示
-- 检测是否要自动放入强化道具到强化面板
function Validate_Pet_DevelopmentBuildUpFrame_StrengthenItem()
	local t_petStrengthenControl	= GetPetStrengthenControl();
	local nCurStrengthenLevel		= t_petStrengthenControl:getCurStrengthenLevel();
	nCurStrengthenLevel = (nCurStrengthenLevel > 0) and nCurStrengthenLevel or 1;
	local petStrengthenDef			= getPetStrengthenDef( nCurStrengthenLevel);
	
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local nTotalItemNum	= container:getItemTotalNum( petStrengthenDef.UseItemID );
	if nTotalItemNum == 0 then
		return;
	end

	local item = container:getItem( petStrengthenDef.UseItemID );
	if t_selUseItem["id"] ~= 0 then
		ClearPet_OperationItem();
	end

	local szFrameName = "Pet_DevelopmentBuildUpFrame";

	-- 设置图标
	local tex		= getglobal( szFrameName .. "_IconBtnIcon" );
	local itemdef	= item:getItemDef();
	local IconPath	= GetItemEquipIconPath()..itemdef.IconID..".tga";
	tex:SetTexture( ( IsInExistence( IconPath ) and IconPath or DEFAULT_ITEM_PATH ) );

	tex:SetTexRelUV( 1, 1 );
	t_selUseItem = { selItem = item, id = item:getItemId(), listType = item:getListType(), gridx = item:getGridIdx() };

	for i = 0, container:getContainerMaxNum(CONTAINER_TYPE_SUNDRIES) - 1 do
		local item = container:getItem(CONTAINER_TYPE_SUNDRIES, i)
		if item:getItemId() == petStrengthenDef.UseItemID then
			LockItem(CONTAINER_TYPE_SUNDRIES, i, petStrengthenDef.UseItemID, szFrameName)
		end
	end

	-- 道具更换后改变道具对应的显示内容
	if t_itemFrames[szFrameName].changeItem ~= nil then
		t_itemFrames[szFrameName].changeItem();
	end
end

function Pet_DevelopmentBuildUpFrame_OnShow()
	t_selUseItem			= { id = 0, selItem = nil, listType = 0, gridx = 0, num = 0 };
	t_operationSelPetInfo	= { heightId = 0, lowId = 0, id = 0, index = -1 };
	t_selSkill				= { id = 0, lv = 0, gridx = -1, };
	requestStrengthen = nil;
	DefaultShow( this:GetName() );
	local t_petStrengthenControl= GetPetStrengthenControl();
	local mainplayer			= ActorMgr:getMainPlayer();
	local petPackage			= mainplayer:getPet();
	local petInfo				= petPackage:getPetInfo( t_operationSelPetInfo["heightId"], t_operationSelPetInfo["lowId"] );
	t_petStrengthenControl:setFirstNotStrengthenLevel( { ["petInfo"] = petInfo } );
	SetPetInfoShow( this:GetName() );

	-- 若宠物面板没显示，则随着宠物操作面板一起显示
	this:SetPoint("topleft", "UIClient", "topleft", 428, 163)
	local frame = getglobal( "PetFrame" );
	if not frame:IsShown() then
		--t_mainSelPetInfo	= { heightId = petInfo.WID.nHeight, lowId = petInfo.WID.nLow, id = petInfo.PetID, index = nPetIndex };
		t_mainSelPetInfo	= t_operationSelPetInfo;
		frame:Show();
		frame:SetPoint("topright", "UIClient", "topleft", 428, 163)
	end
	UpdatePetFrame();
	local modelview = getglobal( "Pet_DevelopmentBuildUpFrame_IconBtnModelView" );
	modelview:Hide();
		
	--[[
	local conFrame = getglobal( "PlayerContainerFrame" );
	if not conFrame:IsShown() then
		conFrame:Show();
		conFrame:SetPoint( "topleft", this:GetName(), "topright", 0, 0 );
	end
	--]]
	ChekPet_OperationFrame_XinShou_Guide( this:GetName() );
	Validate_Pet_DevelopmentBuildUpFrame_StrengthenItem();
end

function ChekPet_OperationFrame_XinShou_Guide( szFrameName )
	if szFrameName == "Pet_SkillLearnFrame" and not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_CONTAINER_LEARN_PET_SKILL_BOOK_BTN ) and 
		GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_NPCTALK_LEARN_PET_SKILL_OPTION ) then
		local conFrame = getglobal( "PlayerContainerFrame" );
		if not conFrame:IsShown() then
			conFrame:Show();
			conFrame:SetPoint( "topright", szFrameName, "topleft", 0, 0  );
		else
			ShowContainerGuide();
		end
	end

	if szFrameName == "Pet_DevelopmentBuildUpFrame" and not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_CONTAINER_STRENGTHEN_ITEM_BTN ) and 
		GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_NPCTALK_STRENGTHEN_PET_OPTION ) then
		local conFrame = getglobal( "PlayerContainerFrame" );
		--print( "ChekPet_OperationFrame_XinShou_Guide--检查宠物强化面板是否要弹出自动背包" );
		if not conFrame:IsShown() then
			conFrame:Show();
			conFrame:SetPoint( "topright", szFrameName, "topleft", 0, 0  );
		else
			ShowContainerGuide();
		end
	end
end

function Pet_XiDianFrame_XiDianBtn_Test_OnClick()
	local modelview = getglobal( this:GetParent().."_IconBtnModelView" );
	--local modelview = getglobal( this:GetParent().."ModelView");
	modelview:setPetModel(0,0,"effect\\entity\\fumo01.ent");
	modelview:setEntityPosition(0,0,100,0);
end

local t_cannotOperateStateDesc = 
					{
						[CLIENT_PET_STAT_FIGHT] = "您的宠物处于召唤状态，", [CLIENT_PET_STAT_DEAD] = "您的宠物处于死亡状态，",
					};
-- 此为点击幻化按钮的时候响应
function Pet_XiDianFrame_XiDianBtn_OnClick()
	-- 若没有宠物被选定，则不响应
	if t_operationSelPetInfo["id"] == 0 then
		ShowMidTips( "请先选中一只宠物" );
		return;
	end

	if t_selUseItem["id"] == 0  then
		ShowMidTips( "请放入宠物幻化道具" );
		return;
	end

	local mainplayer = ActorMgr:getMainPlayer();
	if getPlayerMoney( mainplayer ) < PET_WASH_GENIUS_MONEY then
		ShowMidTips( ""..MONEY_TYPE_BULL_NAME.."值不足" );
		return;
	end

	-- 判断宠物的状态
	local petPackage = mainplayer:getPet();
	for nState, szDesc in pairs( t_cannotOperateStateDesc ) do
		if petPackage:isPetInState( t_operationSelPetInfo["index"], nState ) then
			ShowMidTips( t_cannotOperateStateDesc[nState].."无法进行幻化操作" );
			return;
		end
	end

	if GameStallManager:isInStallIPet( SHITYPE, t_operationSelPetInfo["heightId"], t_operationSelPetInfo["lowId"] ) then
		ShowMidTips( "宠物正在摆摊栏位中，无法进行幻化操作" );
		return;
	end

	if petPackage:isPetInFreedom( t_operationSelPetInfo["heightId"], t_operationSelPetInfo["lowId"] ) then
		ShowMidTips( "该宠物已经被您放生，无法进行该操作！" );
		return;
	end
	
	local container	= mainplayer:getContainer();
	--local useItem	= container:getItem( t_selUseItem["id"] );
	local useItem	= t_selUseItem["selItem"];
	local petInfo	= petPackage:getPetInfo( t_operationSelPetInfo["index"] );
	local petInfo	= petPackage:getPetInfo( t_operationSelPetInfo["index"] );
	if petInfo.Hierarchy == 2 then
		ShowMidTips( "二代宠物不能进行幻化操作" );
		return;
	end

	local itemDef	= getItemDef( useItem:getItemId() );
	-- 若道具是高级道具
	--[[if itemDef.Result[0].ResultVal1 == 1 and petInfo.PetGenius < PET_GENIUS_B then
		ShowMidTips( "宠物资质太低，不能使用高级道具，请换低级道具" );
		return;
	end
	--]]
	
	--local nPetIndex = petPackage:getPetIndex( petInfo );
	MessageBox("幻化宠物","幻化后，将有机会获得更好的资质，但等级将变为1级");
	MessageBoxFrame:SetClientUserData(0,useItem:getListType());
	MessageBoxFrame:SetClientUserData(1,useItem:getGridIdx());
	MessageBoxFrame:SetClientUserData(2,t_operationSelPetInfo["index"]);
	--container:send_UsedExpendableItem( useItem:getListType(), useItem:getGridIdx(), t_operationSelPetInfo["index"] );
end

-- 此为点击增强按钮的时候响应
function Pet_ZengQiangFrame_ZengQiang_OnClick()
	if t_operationSelPetInfo["id"] == 0 then
		ShowMidTips( "请先选中一只宠物" );
		return;
	end

	if t_selUseItem["id"] == 0  then
		ShowMidTips( "请放入宠物增强道具" );
		return;
	end

	local mainplayer = ActorMgr:getMainPlayer();
	-- 判断宠物的状态
	local petPackage = mainplayer:getPet();
	for nState, szDesc in pairs( t_cannotOperateStateDesc ) do
		if petPackage:isPetInState( t_operationSelPetInfo["index"], nState ) then
			ShowMidTips( t_cannotOperateStateDesc[nState].."不能进行增强操作" );
			return;
		end
	end
	
	local petInfo	= petPackage:getPetInfo( t_operationSelPetInfo["index"] );
	local petDef	= getPetDef( t_operationSelPetInfo["id"] );
	local t_attrIncr = 
	{
		-- 魔法攻击
		[0] = { curValue = petInfo.IceAtkMul,		maxValue = petDef.MaxIceAtkMul,		desc = "魔法攻击"	},
		-- 物理攻击
		[1] = { curValue = petInfo.PhsicAtkMul,		maxValue = petDef.MaxPhsicAtkMul,	desc = "物理攻击"	},
		-- 魔法防御
		[2] = { curValue = petInfo.IceDefMul,		maxValue = petDef.MaxIceDefMul,		desc = "魔法防御"	},
		-- 物理防御
		[3] = { curValue = petInfo.PhsicDefMul,		maxValue = petDef.MaxPhsicDefMul,	desc = "物理防御"	},
		-- 气血
		[4] = { curValue = petInfo.HPMul,			maxValue = petDef.MaxHPMul,			desc = "气血"		},
		-- 闪避
		[5] = { curValue = petInfo.FleeMul,			maxValue = petDef.MaxFleeMul,		desc = "闪避"		},
		-- 命中
		[6] = { curValue = petInfo.HitRateMul,		maxValue = petDef.MaxHitRateMul,	desc = "命中"		},
	};

	local container	= mainplayer:getContainer();

	local useItem	= t_selUseItem["selItem"];
	local itemDef	= getItemDef( useItem:getItemId() );
	local attrMap	= t_attrIncr[itemDef.Result[0].ResultVal1];
	if attrMap ~= nil and attrMap.curValue >= attrMap.maxValue then
		ShowMidTips( attrMap.desc.."增加次数已达到上限，不能继续增加" );
		return;
	end

	--local nPetIndex = petPackage:getPetIndex( petInfo );
	container:send_UsedExpendableItem( useItem:getListType(), useItem:getGridIdx(), t_operationSelPetInfo["index"] );
end

function getBuildUpItem( nResultId, nResultVal1 )
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	-- TODO: 改成优先使用绑定的物品
	for i = 1, container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES ) do
		local item = container:getItem( CONTAINER_TYPE_SUNDRIES, i - 1 );
		if item:getItemId() ~= 0 then
			local itemDef = getItemDef( item:getItemId() );
			if itemDef.Result[0].ResultID == nResultId and itemDef.Result[0].ResultVal1 == nResultVal1 then
				return item;
			end
		end
	end

	return nil;
end

-- 此为点击强化按钮的时候响应
function GetUseCaiLiaoTiShi( itemDef, petInfo )
end

function Pet_DevelopmentBuildUpFrame_QiangHua_OnClick()
	if CostMoneyConfirmFrame:IsShown() then
		return;
	end
	if requestStrengthen then
		return;
	end
	if IsMessageBoxBlock(Pet_DevelopmentBuildUpFrame) then
		return;
	end
	-- 若没有宠物被选定，则不响应
	if t_operationSelPetInfo["id"] == 0 then
		ShowMidTips( "请先选中一只宠物" );
		return;
	end

	if t_selUseItem["id"] == 0  then
		local itemId = GetPetStrongUpItem();
		MessageBoxForItem("提示",getItemDef(itemId).Name);
		MessageBoxForItemFrame:SetClientUserData(0,itemId);
		return;
	end
	
	local mainplayer = ActorMgr:getMainPlayer();
	-- 判断宠物的状态
	local petPackage = mainplayer:getPet();
	for nState, szDesc in pairs( t_cannotOperateStateDesc ) do
		if petPackage:isPetInState( t_operationSelPetInfo["index"], nState ) then
			ShowMidTips( t_cannotOperateStateDesc[nState].."无法进行强化操作" );
			return;
		end
	end

	if GameStallManager:isInStallIPet( SHITYPE, t_operationSelPetInfo["heightId"], t_operationSelPetInfo["lowId"] ) then
		ShowMidTips( "该宠物无法进行强化" );
		return;
	end
	
	local petInfo	= petPackage:getPetInfo( t_operationSelPetInfo["index"] );
	--[[
	if math.floor( petInfo.LoveVal / 100 ) < 100 then
		ShowMidTips( "宠物忠诚度没满，不能进行强化" );
		return;
	end
	--]]
	local petDef	= getPetDef( t_operationSelPetInfo["id"] );
	local container	= mainplayer:getContainer();
	local useItem	= container:getItem( t_selUseItem["id"] );
	local itemDef	= getItemDef( useItem:getItemId() );
	
	if not t_itemFrames[this:GetParent()].check( itemDef, petInfo ) then
		return;
	end
	
	local t_petStrengthenControl	= GetPetStrengthenControl();
	local nCurStrengthenLevel		= t_petStrengthenControl:getCurStrengthenLevel();
	if nCurStrengthenLevel == 0 then
		ShowMidTips( "没有选定要强化的星级" );
		return;
	end
	local petStrengthenDef			= getPetStrengthenDef( nCurStrengthenLevel);
	local szFrameName = "Pet_DevelopmentBuildUpFrame"
	do
		ClearItemLock(szFrameName)
		local lock
		if IsItemLock(0, 0, petStrengthenDef.UseItemID) then
			lock = true
		end
		for i = 0, container:getContainerMaxNum(CONTAINER_TYPE_SUNDRIES) - 1 do
			local item = container:getItem(CONTAINER_TYPE_SUNDRIES, i)
			if item:getItemId() == petStrengthenDef.UseItemID then
				LockItem(CONTAINER_TYPE_SUNDRIES, i, petStrengthenDef.UseItemID, szFrameName)
			end
		end
		if lock then
			ShowMidTips(string.format("%s已被锁定，无法进行宠物强化", getItemDef(petStrengthenDef.UseItemID).Name))
			return
		end
	end

	local nTotalItemNum				= container:getItemTotalNum( useItem:getItemId() );
	if nTotalItemNum < petStrengthenDef.UseItemNum then
		ShowMidTips( "强化材料不足，不能进行强化" );
		MessageBoxForItem("提示",getItemDef(petStrengthenDef.UseItemID).Name);
		MessageBoxForItemFrame:SetClientUserData(0,petStrengthenDef.UseItemID);
		return;
	end

	if getPlayerMoney( mainplayer ) < petStrengthenDef.MoneyVal then
		ShowMidTips( "绑银不足，无法强化" );
		return;
	end
	
	local nCurSelectLevel	= t_petStrengthenControl:getCurSelLevel();
	
	if checkNeedWarnCost( petStrengthenDef.MoneyVal ) then
		local btn = getglobal( "Pet_DevelopmentBuildUpFrame_QiangHuaBtn" );
		btn:SetClientUserData( 0, t_selUseItem["selItem"]:getListType() );
		btn:SetClientUserData( 1, t_selUseItem["selItem"]:getGridIdx() );
		CostMoneyConfirmFrame:SetClientUserData( 0, petInfo.WID.nHeight );
		CostMoneyConfirmFrame:SetClientUserData( 1, petInfo.WID.nLow );
		CostMoneyConfirmFrame:SetClientUserData( 2, nCurSelectLevel );
		CostMoneyConfirmFrame:SetClientString("宠物强化");
		SetWarnFrame( "Pet_DevelopmentBuildUpFrame" );
		CostMoneyConfirmFrame:Show();
		return;
	end

	local safeItem;
	-- 检查是否有使用其他强化道具
	if petPackage:petStrengthen( petInfo.WID.nHeight, petInfo.WID.nLow, t_selUseItem["selItem"], safeItem, petStrengthenDef.UseItemNum, nCurSelectLevel ) then
		requestStrengthen = true;
	end

	if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_PESTRENGTHENFRAME_STRENGTHEN_BTN ) and 
		GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_CONTAINER_STRENGTHEN_ITEM_BTN ) then
		local uvTex = getglobal( this:GetName().."UVAnimationTex" );
		uvTex:Hide();
		local xinShouBtn	= getglobal( "Pet_DevelopmentBuildUpFrameXinShouBtn" );
		if xinShouBtn:IsShown() and xinShouBtn:GetClientString() == this:GetName() then
			xinShouBtn:Hide();
			GameWizard:doStep( GAMEWIZARD_FIRST_BLINK_PESTRENGTHENFRAME_STRENGTHEN_BTN );
		end
	end
end

function SetStrengthenState()
	requestStrengthen = true;
end

-- 宠物技能学习

local t_changePetSkillPreCondition = { ["Pet_SkillGridRebuildFrame"] = {}, };
t_changePetSkillPreCondition["Pet_SkillGridRebuildFrame"].func =
function ( data )
	if data["skillGrid"] == t_selSkill["gridx"] then
		return true;
	end

	if ResetExistSkillConfirmFrame:IsShown() then
		ShowMidTips( "请先完成确认操作" );
		return false;
	end

	return true;
end

-- 点击宠物技能栏位按钮的时候响应
function Pet_SkillButton_OnClick()
	-- 判断栏位是不处于激活状态，则不能选定
	local tex = getglobal( this:GetName() .. "Icon" );
	if tex:GetTexture() == "uires\\TuPianLei\\WuPin\\锁.tga" then
		return;
	end

	local changePetSkillPreCondition = t_changePetSkillPreCondition[this:GetParent()];
	if changePetSkillPreCondition ~= nil then
		if not changePetSkillPreCondition.func( { skillGrid = this:GetClientID() - 1 } ) then
			return;
		end
	end

	local mainplayer	= ActorMgr:getMainPlayer();
	-- 判断宠物的状态
	local petPackage	= mainplayer:getPet();
	local petInfo		= petPackage:getPetInfo( t_operationSelPetInfo["index"] );
	local nSkillGridx	= this:GetClientID() - 1;
	local skillInfo		= petInfo.PetSkills.SkillGrid[nSkillGridx];
	t_selSkill	= { gridx = nSkillGridx, };
	--DisCheckAllBtn( this:GetParent().."_SkillButton", MAX_PET_SKILL_GRID );
	--this:Checked();
	--UpdatePet_SkillLevelUpFrameCostBull();
	--[[
	if this:GetParent() == "Pet_SkillLearnFrame" then
		if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_PETSKILLFRAME_SKILL_GRID_BTN ) then
			GameWizard:doStep( GAMEWIZARD_FIRST_BLINK_PETSKILLFRAME_SKILL_GRID_BTN );
			for i = 1, MAX_PET_SKILL_GRID do
				local uvTex = getglobal( this:GetName().."UVAnimationTex" );
				uvTex:Hide();
			end
			local xinShouBtn	= getglobal( "Pet_SkillLearnFrameXinShouBtn" );
			if xinShouBtn:IsShown() and xinShouBtn:GetClientString() == this:GetName() then
				xinShouBtn:Hide();
			end
			
			ShowPetSkillFrameXinShouGuide();
		end
	end
	--]]
end

-- 鼠标移入技能栏位的时候响应
function Pet_SkillButton_OnEnter()	
	-- 若没选中宠物，则不显示任何内容
	if t_operationSelPetInfo["id"] == 0 then
		return;
	end

	local nSkillType = this:GetClientUserData( 3 );
	if nSkillType == PET_SKILL_DISABLE then
		local szText = "请先开启宠物技能格后学习技能";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
							button = this:GetName(), vertical = "bottom" } );
		return;
	end
	
	local mainplayer		= ActorMgr:getMainPlayer();
	local petPackage		= mainplayer:getPet();
	local petInfo			= petPackage:getPetInfo( t_operationSelPetInfo["index"] );
	local nSkillGridIndex	= this:GetClientID() - 1;
	local skillGridInfo		= petInfo.PetSkills.SkillGrid[nSkillGridIndex];
	ShowPetOperationSkillTips( getglobal( this:GetParent() ), petInfo, skillGridInfo, this:GetName() );
end

function ShowPetOperationSkillTips( parentFrame, petInfo, skillGridInfo, szRelButtonName )
	local diathesisDef	= SkillMgr:getDiathesisDef( skillGridInfo.SkillID, skillGridInfo.SkillLevel );
	local t_fonts	= {};
	local t_tgas	= {};
	local t_riches	= {};

	-- 若格子有技能，则显示技能tips
	if diathesisDef ~= nil then
		local IconPath = getPetSkillPath()..diathesisDef.IconID..".tga";
		t_fonts = 
						{
						["PetSkillTipsFrameNameFont"]		= { value = diathesisDef.Name },-- ..diathesisDef.Level.."级"
						--["PetSkillTipsFrameSkillTypeFont"]	= { value = "类型："..t_skillGridTga[skillGridInfo.GridType].typeDesc },
						};
		
		t_tgas = 
						{
						["PetSkillTipsFrameTex"] = { path = ( IsInExistence( IconPath ) and IconPath or DEFAULT_ITEM_PATH ) },
						};
		
		t_riches = 
					{
					["PetSkillTipsFrameDescRich"] = { text = diathesisDef.DiathesisDesc, color = { r = 255, g = 255, b = 255 } },
					};
	-- 否则，显示格子tips
	else
		t_fonts = 
						{
						["PetSkillGridTipsFrameNameFont"]		= { value = "宠物技能栏位" },
						--["PetSkillGridTipsFrameGridTypeFont"]	= { value = "类型："..t_skillGridTga[skillGridInfo.GridType].typeDesc },
						["PetSkillGridTipsFrameGridTypeFont"]	= { value = "可以学习新技能" },
						};
		
		t_tgas = 
						{
						-- path = t_skillGridTga[skillGridInfo.GridType].path
						["PetSkillGridTipsFrameTex"] = { path = t_skillGridTga[PET_SKILL_SELF_BUFF].path },
						};
		local t_skillGridTga = 
						{
						[PET_SKILL_ATK]			= "该技能格为攻击类技能格，可学习攻击类技能，找时间城宠物训练师拉奥多消耗相应类型技能书可学习",
						[PET_SKILL_OWNER_BUFF]	= "该技能格为佑主类技能格，可学习佑主类技能，找时间城宠物训练师拉奥多消耗相应类型技能书可学习",
						[PET_SKILL_SELF_BUFF]	= "该技能格为增强类技能格，可学习增强类技能，找时间城宠物训练师拉奥多消耗相应类型技能书可学习",
						};
		t_riches = 
					{
					["PetSkillGridTipsFrameDescRich"] = { text = "找时间城宠物训练师拉奥多消耗相应类型技能书可学习技能", color = { r = 255, g = 255, b = 255 } },
					};
		
		local szText = "宠物技能格，可以学习新技能";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
							button = this:GetName(), vertical = "bottom" } );
		return;
	end

	for name, data in pairs( t_fonts ) do
		local font = getglobal( name );
		font:SetText( data.value );
	end

	for name, data in pairs( t_riches ) do
		local rich = getglobal( name );
		rich:SetText( data.text, data.color.r, data.color.g, data.color.b );
	end

	for name, data in pairs( t_tgas ) do
		local tex = getglobal( name );
		tex:SetTexture( data.path );
		tex:SetTexRelUV( 1, 1 );
	end
	
	local t_showTips = { [true] = { frame = "PetSkillTipsFrame", }, [false] = { frame = "PetSkillGridTipsFrame", } };
	local tips = getglobal( t_showTips[diathesisDef ~= nil].frame );
	tips:SetClientString( parentFrame:GetName() );
	tips:Show();
	util.AdjustBigtipsScreenPos( { tipsFrame = tips:GetName(), button = szRelButtonName } );
end

-- 鼠标离开技能栏位的时候响应
function Pet_SkillButton_OnLeave()
	local mainplayer		= ActorMgr:getMainPlayer();
	local petPackage		= mainplayer:getPet();
	local petInfo			= petPackage:getPetInfo( t_operationSelPetInfo["index"] );
	local nSkillGridIndex	= this:GetClientID() - 1;
	local skillGridInfo		= petInfo.PetSkills.SkillGrid[nSkillGridIndex];
	local diathesisDef		= SkillMgr:getDiathesisDef( skillGridInfo.SkillID, skillGridInfo.SkillLevel );
	local t_showTips = { [true] = { frame = "PetSkillTipsFrame", }, [false] = { frame = "PetSkillGridTipsFrame", } };
	local tips = getglobal( t_showTips[diathesisDef ~= nil].frame );
	tips:Hide();
	
	local t_hideIcons = { "GameTooltip", };
	util.HideIcons( t_hideIcons );
end

-- 学习按钮
local t_cannotLearnSkillStateDesc = 
					{
						-- [CLIENT_PET_STAT_FIGHT] = "宠物处于出征状态，",
						 [CLIENT_PET_STAT_DEAD] = "宠物处于死亡状态，",
					};

function Pet_SkillLearnFrame_StudyBtn_OnClick()
	-- 若没有宠物被选定，则不响应
	if t_operationSelPetInfo["id"] == 0 then
		ShowMidTips( "请先放入要学习技能的宠物" );
		return;
	end

	if t_selUseItem["id"] == 0 then
		ShowMidTips( "请先放入宠物技能书" );
		return;
	end
	--[[
	if t_selSkill["gridx"] == -1 then
		-- 
		ShowMidTips( "请选定技能栏位" );
		return;
	end
	--]]
	local mainplayer = ActorMgr:getMainPlayer();
	-- 判断宠物的状态
	local petPackage = mainplayer:getPet();
	
	for nState, szDesc in pairs( t_cannotLearnSkillStateDesc ) do
		if petPackage:isPetInState( t_operationSelPetInfo["index"], nState ) then
			ShowMidTips( t_cannotLearnSkillStateDesc[nState].."不能进行学习技能操作" );
			return;
		end
	end
	
	-- 判断主角是否有足够的"..MONEY_TYPE_BULL_NAME.."
	if getPlayerMoney( mainplayer ) < PET_ADD_SKILL_GRID_MONEY then
		ShowMidTips( "身上"..MONEY_TYPE_BULL_NAME.."值不足，不能学习技能" );
		return;
	end

	local petInfo		= petPackage:getPetInfo( t_operationSelPetInfo["index"] );
	local bookItemDef	= getItemDef( t_selUseItem.id );
	-- 判断选定的栏位是否已有技能
	--[[
	local diathesisInfo	= petInfo.PetSkills.SkillGrid[ t_selSkill["gridx"] ];
	local diathesisiDef	= SkillMgr:getDiathesisDef( diathesisInfo.SkillID, diathesisInfo.SkillLevel );
	
	if diathesisiDef ~= nil then
		ShowMidTips( "该格位已有技能，不能覆盖学习" );
		return;
	end
	--]]
	
	-- 判定宠物格位上是否已经有这个要学习的技能了，若有，则不能学习
	for i = 1, petInfo.PetSkills.SkillGridNum do
		local diathInfo = petInfo.PetSkills.SkillGrid[i-1];
		local diathDef	= SkillMgr:getDiathesisDef( diathInfo.SkillID, diathInfo.SkillLevel );
		if diathInfo.SkillID == bookItemDef.Result[0].ResultVal1 and diathDef ~= nil then
			ShowMidTips( "已有该技能，不能重复学习" );
			return;
		end
	end

	if GameStallManager:isInStallIPet( SHITYPE, t_operationSelPetInfo["heightId"], t_operationSelPetInfo["lowId"] ) then
		ShowMidTips( "该宠物无法学习技能" );
		return;
	end

	-- 再判定所选技能栏位和技能书所能学的技能是否一样
	diathesisDef	= SkillMgr:getDiathesisDef( bookItemDef.Result[0].ResultVal1, 1 );
	--[[
	if diathesisDef.PetDiathesisType ~= diathesisInfo.GridType then
		ShowMidTips( "所选技能栏位与技能书类型不符 " );
		return;
	end
	--]]

	petPackage:petSkillLearn( petInfo.WID.nHeight, petInfo.WID.nLow, t_selSkill["gridx"], t_selUseItem["selItem"] );

	if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_PETSKILLFRAME_LEARN_SKILL_BTN ) and 
			GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_CONTAINER_LEARN_PET_SKILL_BOOK_BTN ) then

		local uvTex = getglobal( this:GetName().."UVAnimationTex" );
		uvTex:Hide();
		
		local xinShouBtn	= getglobal( "Pet_SkillLearnFrameXinShouBtn" );
		if xinShouBtn:IsShown() and xinShouBtn:GetClientString() == this:GetName() then
			xinShouBtn:Hide();
			GameWizard:doStep( GAMEWIZARD_FIRST_BLINK_PETSKILLFRAME_LEARN_SKILL_BTN );
		end
	end
end

-- 升级按钮
function Pet_SkillLevelUpFrame_LevelUpBtn_OnClick()
	-- 若没有宠物被选定，则不响应
	if t_operationSelPetInfo["id"] == 0 then
		ShowMidTips( "请先放入要升级技能的宠物" );
		return;
	end

	if t_selUseItem["id"] == 0 then
		ShowMidTips( "请放入升级技能的道具" );
		return;
	end

	if t_selSkill["gridx"] == -1 then
		ShowMidTips( "请选定要升级的技能" );
		return;
	end
	
	local mainplayer = ActorMgr:getMainPlayer();
	-- 判断宠物的状态
	local petPackage = mainplayer:getPet();
	for nState, szDesc in pairs( t_cannotOperateStateDesc ) do
		if petPackage:isPetInState( t_operationSelPetInfo["index"], nState ) then
			ShowMidTips( t_cannotOperateStateDesc[nState].."不能进行技能升级操作" );
			return;
		end
	end

	local petInfo = petPackage:getPetInfo( t_operationSelPetInfo["index"] );
	-- 判断选定的栏位是否已有技能
	local diathesisInfo	= petInfo.PetSkills.SkillGrid[t_selSkill["gridx"]];
	local diathesisiDef	= SkillMgr:getDiathesisDef( diathesisInfo.SkillID, diathesisInfo.SkillLevel );
	if diathesisiDef == nil then
		ShowMidTips( "还未学习该技能，不能升级，请先学习" );
		return;
	end
	-- 判定该技能是否达到最高等级
	local newDiathesisiDef	= SkillMgr:getDiathesisDef( diathesisInfo.SkillID, diathesisInfo.SkillLevel + 1 );
	if newDiathesisiDef == nil then
		ShowMidTips( "该技能已达到最高等级，不能再升级了" );
		return;
	end
	
	-- 再判定所选技能栏位和技能书所能血的技能是否一样
	
	--[[
	if newDiathesisiDef.Learn.ItemID ~= t_selUseItem.id then
		ShowMidTips( "所选择道具不能升级被选定的技能" );
		return;
	end
	--]]
	petPackage:petSkillLevelUp( petInfo.WID.nHeight, petInfo.WID.nLow, diathesisInfo.SkillID, diathesisInfo.SkillLevel, 
								t_selSkill["gridx"], t_selUseItem["selItem"] );
end

-- 扩充按钮
function Pet_SkillGridExtendFrame_ExtendBtn_OnClick()
	-- 若没有宠物被选定，则不响应
	if t_operationSelPetInfo["id"] == 0 then
		ShowMidTips( "请先放入宠物" );
		return;
	end

	if t_selUseItem["id"] == 0  then
		ShowMidTips( "请放入宠物技能格扩充道具" );
		return;
	end
	
	local mainplayer = ActorMgr:getMainPlayer();
	-- 判断宠物的状态
	local petPackage = mainplayer:getPet();
	for nState, szDesc in pairs( t_cannotOperateStateDesc ) do
		if petPackage:isPetInState( t_operationSelPetInfo["index"], nState ) then
			ShowMidTips( t_cannotOperateStateDesc[nState].."不能进行扩充技能栏位操作" );
			return;
		end
	end

	-- 判断主角是否有足够的"..MONEY_TYPE_BULL_NAME.."
	if getPlayerMoney( mainplayer ) < PET_ADD_SKILL_GRID_MONEY then
		ShowMidTips( "身上"..MONEY_TYPE_BULL_NAME.."值不足，不能扩充技能" );
		return;
	end

	-- 查看是否还有剩余的栏位可以扩充
	local petInfo		= petPackage:getPetInfo( t_operationSelPetInfo["index"] );
	local skillGrids	= petInfo.PetSkills;

	local mainplayer= ActorMgr:getMainPlayer();
	local container	= mainplayer:getContainer();
	--local useItem	= container:getItem( t_selUseItem["id"] );
	local useItem	= t_selUseItem["selItem"];
	local itemDef	= getItemDef( useItem:getItemId() );

	container:send_UsedExpendableItem( useItem:getListType(), useItem:getGridIdx(), t_operationSelPetInfo["index"] );
end

-- 改造按钮
function Pet_SkillGridRebuildFrame_RebuildBtn_OnClick()
	-- 若没有宠物被选定，则不响应
	if t_operationSelPetInfo["id"] == 0 then
		ShowMidTips( "请先放入宠物" );
		return;
	end

	if t_selUseItem["id"] == 0 then
		ShowMidTips( "请放入技能格改造道具" );
		return;
	end
	
	if t_selSkill["gridx"] == -1 then
		ShowMidTips( "请先选中一个技能格位" );
		return;
	end
	
	local mainplayer = ActorMgr:getMainPlayer();
	-- 判断宠物的状态
	local petPackage = mainplayer:getPet();
	for nState, szDesc in pairs( t_cannotOperateStateDesc ) do
		if petPackage:isPetInState( t_operationSelPetInfo["index"], nState ) then
			ShowMidTips( t_cannotOperateStateDesc[nState].."不能进行技能格改造操作" );
			return;
		end
	end

	if getPlayerMoney( mainplayer ) < PET_WASH_SKILL_GRID_MONEY then
		ShowMidTips( "身上的"..MONEY_TYPE_BULL_NAME.."不够，不能重置技能栏位" );
		return;
	end
	
	local petInfo		= petPackage:getPetInfo( t_operationSelPetInfo["index"] );
	local diathesisInfo	= petInfo.PetSkills.SkillGrid[t_selSkill["gridx"]];
	local diathesisiDef	= SkillMgr:getDiathesisDef( diathesisInfo.SkillID, diathesisInfo.SkillLevel );
	if diathesisiDef ~= nil then
		-- TODO:确认重置技能格
		SetResetPetSkillInfo( t_operationSelPetInfo );
		ResetExistSkillConfirmFrame:Show();
		return;
	end

	local container	= mainplayer:getContainer();
	local useItem	= container:getItem( t_selUseItem["id"] );
	local itemDef	= getItemDef( useItem:getItemId() );
	petPackage:petWashSkillGrid( t_operationSelPetInfo["heightId"], t_operationSelPetInfo["lowId"], t_selUseItem["selItem"], t_selSkill["gridx"] );
end

local t_DropPetInfo = {};
function SetDropPetInfo( data )
	t_DropPetInfo = data;
end

function DropPetConfirmFrame_OnShow()
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local petInfo		= petPackage:getPetInfo( t_DropPetInfo["index"] );
	local szPetName		= petInfo.Name;
	if szPetName == "" then
		szPetName = petDef.Name;
	end
	local rich = getglobal( "DropPetConfirmFrame_DropPetDescRich" );
	rich:SetText( "#P你确定要放生"..szPetName.."吗？", 255, 255, 190 );
end

local t_LastPetInfo = {};
-- 确定放生宠物按钮
function DropPetConfirmFrame_ConfirmBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	local petPackage = mainplayer:getPet();
	local szText = "DropPetConfirmFrame_ConfirmBtn_OnClick--heightID = "..t_DropPetInfo.heightId.." lowID = "..t_DropPetInfo.lowId.."--5407";
	logPrivateInfo( szText );
	petPackage:petFreedom( t_DropPetInfo.heightId, t_DropPetInfo.lowId );
	local systemSetting = GameMgr:getUserSystemConfig();
	local nLastPetIndex = systemSetting:getGameOptValue( GAME_OPT_LAST_PET_FIGHT ) - 1;
	t_LastPetInfo = petPackage:getPetInfo( nLastPetIndex );
	if nLastPetIndex ~= -1 and nLastPetIndex == t_DropPetInfo["index"] then
		systemSetting:setGameOptValue( GAME_OPT_LAST_PET_FIGHT, 0 );
	end
	DropPetConfirmFrame:Hide();
end

-- 取消
function DropPetConfirmFrame_Cancel_OnClick()
	DropPetConfirmFrame:Hide();
end

local t_seeZiZhiPetInfo = { petInfo = nil, id = 0 };
function GetSeeZiZhiPetInfo()
	return t_seeZiZhiPetInfo["petInfo"];
end

function GetSeeZiZhiPetID()
	return t_seeZiZhiPetInfo["id"];
end

function SetSeeZiZhiPetInfo( petInfo )
	if petInfo == nil then
		t_seeZiZhiPetInfo = nil;
		return;
	end
	t_seeZiZhiPetInfo = { ["petInfo"] = petInfo, ["id"] = petInfo.PetID };
end

function WatchPetFrame_OnShow()
	this:SetPoint( "center", "$parent", "center", 0, 0  );
end

function SeePetZiZhiFrame_OnShow()
	this:SetPoint( "center", "$parent", "center", 0, 0  );
end

-- 查看宠物素质面板
function SeePetZiZhiFrame_OnUpdate()
	local frame = getglobal( this:GetClientString() );
	if not frame:IsShown() then
		this:Hide();
		frame:SetClientString( "" );
	end
end

function SeePetZiZhiFrame_OnHide()
	t_seeZiZhiPetInfo = nil;
end

-- 确认重置宠物技能格
local t_resetPetSkillInfo = { heightId = 0, lowId = 0, id = 0, index = -1 };
function SetResetPetSkillInfo( data )
	t_resetPetSkillInfo = data;
end

function ClearResetPetSkillInfo( )
	t_resetPetSkillInfo = { heightId = 0, lowId = 0, id = 0, index = -1 };
end

-- 确认重置技能格
function ResetExistSkillConfirmFrame_ConfirmBtn_OnClick()
	if t_resetPetSkillInfo["heightId"] ~= t_operationSelPetInfo["heightId"] or 
		t_resetPetSkillInfo["lowId"] ~= t_operationSelPetInfo["lowId"] then
		ShowMidTips( "操作已失效" );
		ResetExistSkillConfirmFrame:Hide();
		return;
	end
	
	local mainplayer = ActorMgr:getMainPlayer();
	local petPackage = mainplayer:getPet();
	petPackage:petWashSkillGrid( t_operationSelPetInfo["heightId"], t_operationSelPetInfo["lowId"], t_selUseItem["selItem"], t_selSkill["gridx"] );
	ResetExistSkillConfirmFrame:Hide();
end

function ResetExistSkillConfirmFrame_OnHide()
	ClearResetPetSkillInfo();
end

-- 宠物自动恢复面板
local t_PetAutoRecoverFrameControl = { waitForUseHP = false, waitForUseZhongCheng = false, canUseConfig = {}, lastUseHPTime = 0, 
										lastUseZhongChengTime = 0, MIN_REQUIRE_USE_ITEM_TIME = 15 };
function GetPetAutoRecoverFrameControl()
	return t_PetAutoRecoverFrameControl;
end

function CheckAutoRecoverPet()
	local t_PetAutoRecoverFrameControl = GetPetAutoRecoverFrameControl();	
	t_PetAutoRecoverFrameControl:clearWaitTime();
	t_PetAutoRecoverFrameControl:buildCanUseHPConfi();
	t_PetAutoRecoverFrameControl:CheckWhenZhongChengChange();
	t_PetAutoRecoverFrameControl:CheckWhenHPChange();
end

--获取背包中可使用等级最低的宠物药水
function GetRecoverMinItem()
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local minVal = 0;
	local nIndex = 0;
	local petPackage	= mainplayer:getPet();
	local nPetIndex		= getFightPetIndex();
	local petInfo;
	if nPetIndex  > -1 then
		petInfo	= petPackage:getPetInfo( nPetIndex );
	else
		local systemSetting = GameMgr:getUserSystemConfig();
		local nLastPetIndex = systemSetting:getGameOptValue( GAME_OPT_LAST_PET_FIGHT ) - 1;
		petInfo	= petPackage:getPetInfo( 0 );
		if nLastPetIndex > -1 then
			petInfo	= petPackage:getPetInfo( nLastPetIndex );	
		end
	end
	if petInfo == nil then return 0; end
	local petLv = petInfo.Level;

	for i = 1, container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES ) do
		local item	= container:getItem( CONTAINER_TYPE_SUNDRIES, i - 1 );
		if item:getItemId() ~= 0 then
			local itemDef = getItemDef( item:getItemId() );			
			if itemDef.Result[0].ResultID == RESULT_ADD_POWER and petLv >= itemDef.WearPre.RoleLevelMin then
				local val = itemDef.Result[0].ResultVal1;
				if minVal == 0 and val ~= nil then 
					minVal = val;
				end
				if val ~= nil then 
					if val <= minVal then
						minVal = val;
						nIndex = i;
					end
				end
			end
		end
	end
	if nIndex ~= nil and nIndex ~= 0 then
		local item = container:getItem( CONTAINER_TYPE_SUNDRIES, nIndex - 1 );
		local nItemID = item:getItemId();
		return nItemID;
	end
	return 0;
end

--获取背包中可使用等级最高的宠物药水
function GetRecoverMaxItem()
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local maxVal		= 0;
	local nIndex		= 0;
	local petPackage	= mainplayer:getPet();
	local nPetIndex		= getFightPetIndex();
	local petInfo;
	if nPetIndex > -1 then
		petInfo	= petPackage:getPetInfo( nPetIndex );
	else
		local systemSetting = GameMgr:getUserSystemConfig();
		local nLastPetIndex = systemSetting:getGameOptValue( GAME_OPT_LAST_PET_FIGHT ) - 1;
		petInfo	= petPackage:getPetInfo( 0 );
		if nLastPetIndex > -1 then
			petInfo	= petPackage:getPetInfo( nLastPetIndex );
		end
	end
	if petInfo == nil then return 0; end
	local petLv = petInfo.Level;	

	for i = 1, container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES ) do
		local item	= container:getItem( CONTAINER_TYPE_SUNDRIES, i - 1 );
		if item:getItemId() ~= 0 then
			local itemDef = getItemDef( item:getItemId() );
			if itemDef.Result[0].ResultID == RESULT_ADD_POWER and petLv >= itemDef.WearPre.RoleLevelMin then
				local val = itemDef.Result[0].ResultVal1;
				if maxVal == 0 and val ~= nil then 
					maxVal = val;
				end
				if val ~= nil then 
					if val >= maxVal then
						maxVal = val;
						nIndex = i;
					end
				end
			end
		end
	end
	if nIndex ~= nil and nIndex ~= 0 then
		local item = container:getItem( CONTAINER_TYPE_SUNDRIES, nIndex - 1 );
		local nItemID = item:getItemId();
		return nItemID;
	end
	return 0;
end

--获取背包中的驯兽糖果
function GetZhongChengItem()
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	for i = 1, container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES ) do
		local item	= container:getItem( CONTAINER_TYPE_SUNDRIES, i - 1 );
		if item:getItemId() ~= nil and item:getItemId() ~= 0 then
			local itemDef = getItemDef( item:getItemId() );
			if itemDef.Result[0].ResultID == RESULT_PET_ADD_LOVEVAL then
				local nItemID = item:getItemId();
				return nItemID;
			end
		end
	end
	return 0;
end

local t_configMapToUI = 
{
[CLIENT_SET_PET_FIRST_MIN_HP]	= { chkBtn = "PetAutoRecoverFrame_FirstHPChkBtn", edit = "PetAutoRecoverFrame_FirstHPEdit", 
	itemBtn = "PetAutoRecoverFrame_ItemGridBtn1", defaultValue = 50, funct = GetRecoverMinItem },

[CLIENT_SET_PET_SECOND_MIN_HP]	= { chkBtn = "PetAutoRecoverFrame_SecondHPChkBtn",edit = "PetAutoRecoverFrame_SecondHPEdit", 
	itemBtn = "PetAutoRecoverFrame_ItemGridBtn2", defaultValue = 30, funct = GetRecoverMaxItem },

[CLIENT_SET_PET_MIN_ZHONG_CHENG]= { chkBtn = "PetAutoRecoverFrame_ZhongChengChkBtn", edit = "PetAutoRecoverFrame_ZhongChengEdit", 
	itemBtn = "PetAutoRecoverFrame_ItemGridBtn3", defaultValue = 50, funct = GetZhongChengItem },
};

t_PetAutoRecoverFrameControl["clearWaitTime"] = 
function ( self )
	self["lastUseHPTime"]			= 0;
	self["lastUseZhongChengTime"]	= 0;
end

t_PetAutoRecoverFrameControl["buildCanUseHPConfi"] = 
function ( self )
	self["canUseConfig"] = { };
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local petPackage	= mainplayer:getPet();	
	if petPackage:getPetNum() == 0 then
		return
	end

	for i, data in pairs( t_configMapToUI ) do
		local oneConfig = GameUserConfig:getOnePetConfig( i );
		if i == CLIENT_SET_PET_MIN_ZHONG_CHENG then
			if oneConfig.m_bOpen and oneConfig.m_uItemID == 0 then
				local nValue = data["defaultValue"];
				if oneConfig.m_uMinValue > 0 then
					GameUserConfig:setOnePetConfig( i, true, oneConfig.m_uMinValue, 0 );
				elseif oneConfig.m_uMinValue == 0 then
					GameUserConfig:setOnePetConfig( i, true, nValue, 0 );
				end
			end
		else
			if oneConfig.m_bOpen and oneConfig.m_uItemID > 0 then
				if container:getItemCount( oneConfig.m_uItemID ) > 0 then
					if oneConfig.m_uMinValue > 0 then
						table.insert( self["canUseConfig"], { index = i, value = oneConfig.m_uMinValue, id = oneConfig.m_uItemID } );
					elseif oneConfig.m_uMinValue == 0 then
						local nValue = data["defaultValue"];
						table.insert( self["canUseConfig"], { index = i, value = nValue, id = oneConfig.m_uItemID } );
					end
				--[[
				else
					local nItemID = data["funct"]();
					if nItemID > 0 and oneConfig.m_uMinValue > 0 then
						table.insert( self["canUseConfig"], { index = i, value = oneConfig.m_uMinValue, id = nItemID } );
					elseif nItemID > 0 and oneConfig.m_uMinValue == 0 then
						local nValue = data["defaultValue"];
						table.insert( self["canUseConfig"], { index = i, value = nValue, id = nItemID } );
					end
				--]]
				end
			elseif oneConfig.m_bOpen and oneConfig.m_uItemID == 0 then
				local nItemID = data["funct"]();
				local nValue = data["defaultValue"];
				if nItemID ~= 0 then
					if oneConfig.m_uMinValue > 0 then
						table.insert( self["canUseConfig"], { index = i, value = oneConfig.m_uMinValue, id = nItemID } );
					elseif oneConfig.m_uMinValue == 0 then
						table.insert( self["canUseConfig"], { index = i, value = nValue, id = nItemID } );
					end
				end
			end
		end		
	end
	
	if table.getn( self["canUseConfig"] ) < 2 then
		return;
	end
	
	table.sort( self["canUseConfig"], 
	function ( lhs, rhs )
		if lhs["value"] == rhs["value"] then
			local lhsItemDef = getItemDef( lhs["id"] );
			local rhsItemDef = getItemDef( rhs["id"] );
			return lhsItemDef.WearPre.RoleLevelMin >= rhsItemDef.WearPre.RoleLevelMin;
		else
			return lhs["value"] < rhs["value"];
		end

		
	end );
end

t_PetAutoRecoverFrameControl["CheckWhenHPChange"] = 
function ( self, data )
	local nPetIndex	= getFightPetIndex();
	if nPetIndex < 0 then
		return;
	end
	
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local petInfo		= petPackage:getPetInfo( nPetIndex );
	local nCurHP		= petInfo.HP;
	local t_canUseConfig = self["canUseConfig"];
	-- 先做个排序
	local nOptionType;
	for i = 1, table.getn( t_canUseConfig ) do
		local oneElem	= t_canUseConfig[i];
		if nCurHP <= oneElem["value"] * petInfo.MaxHP / 100 then
			nOptionType = oneElem["index"];
			break;
		end
	end
	
	if nOptionType == nil then
		self["waitForUseHP"] = false;
	else
		self["waitForUseHP"]	= true;
		self["lastUseHPTime"]	= 0;
	end
end

t_PetAutoRecoverFrameControl["CheckWhenZhongChengChange"] = 
function ( self, data )
	local nPetIndex	= getFightPetIndex();
	if nPetIndex < 0 then
		return;
	end
	
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local petInfo		= petPackage:getPetInfo( nPetIndex );
	local nCurValue		= math.floor( petInfo.LoveVal / 100 );

	local oneConfig = GameUserConfig:getOnePetConfig( CLIENT_SET_PET_MIN_ZHONG_CHENG );
	if oneConfig.m_uItemID < 0 or oneConfig.m_uMinValue < 0 then
		self["waitForUseZhongCheng"] = false;
		return;
	end

	local nValue;
	if oneConfig.m_uMinValue == 0 then
		nValue = t_configMapToUI[CLIENT_SET_PET_MIN_ZHONG_CHENG]["defaultValue"];
		if nCurValue > nValue then
			self["waitForUseZhongCheng"] = false;
		else
			self["waitForUseZhongCheng"]	= true;
			self["lastUseZhongChengTime"]	= 0;
		end
	else
		if nCurValue > oneConfig.m_uMinValue then
			self["waitForUseZhongCheng"] = false;
		else
			self["waitForUseZhongCheng"]	= true;
			self["lastUseZhongChengTime"]	= 0;
		end
	end
end

t_PetAutoRecoverFrameControl["setWaitForHPState"] =
function ( self, data )
	self["waitForUseHP"] = data["waitForUseHP"];
end

t_PetAutoRecoverFrameControl["setWaitForZhongChengState"] = 
function ( self, data )
	self["waitForUseZhongCheng"] = data["waitForUseZhongCheng"];
end

t_PetAutoRecoverFrameControl["checkZhongChengOnUpdate"] = 
function ( self )
	local fCurTime = os.clock();
	if fCurTime - self["lastUseZhongChengTime"] < self["MIN_REQUIRE_USE_ITEM_TIME"] then
		return;
	end
	
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer:isDead() then
		return;
	end

	local container		= mainplayer:getContainer();
	if not container:canUseExpendableItem() then
		return;
	end

	local nPetIndex	= getFightPetIndex();
	if nPetIndex < 0 then
		return;
	end

	
	local petPackage	= mainplayer:getPet();
	local petInfo		= petPackage:getPetInfo( nPetIndex );
	self:checkZhongCheng( { lowID = petInfo.WID.nLow, heighID = petInfo.WID.nHeight, value = petInfo.LoveVal } );
end

t_PetAutoRecoverFrameControl["checkHPOnUpdate"] = 
function ( self )
	local fCurTime = os.clock();
	--print( "checkHPOnUpdate--fCurTime = "..fCurTime..",lastTime = "..self["lastUseHPTime"] .."4776" );
	if fCurTime - self["lastUseHPTime"] < self["MIN_REQUIRE_USE_ITEM_TIME"] then
		return;
	end

	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	if not container:canUseExpendableItem() then
		return;
	end

	if mainplayer:isDead() then
		return;
	end

	local nPetIndex	= getFightPetIndex();
	if nPetIndex < 0 then
		return;
	end

	local petPackage	= mainplayer:getPet();
	local petInfo		= petPackage:getPetInfo( nPetIndex );
	self:checkHP( { lowID = petInfo.WID.nLow, heighID = petInfo.WID.nHeight, value = petInfo.HP } );
end

t_PetAutoRecoverFrameControl["checkZhongCheng"] = 
function ( self, data )
	local nLowID		= data["lowID"];
	local nHeightID		= data["heighID"];
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local nItemID		= GetZhongChengItem();
	if nItemID == 0 then
		return;
	end
	-- 若宠物没处于出征状态，则无须自动使用药品
	if not petPackage:isPetInState( nHeightID, nLowID, CLIENT_PET_STAT_FIGHT ) then
		return;
	end

	local oneConfig = GameUserConfig:getOnePetConfig( CLIENT_SET_PET_MIN_ZHONG_CHENG );
	-- 若没开启这个自动功能，
	if not oneConfig.m_bOpen or oneConfig.m_uItemID < 0 or oneConfig.m_uMinValue < 0 then
		return;
	end

	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	
	-- 若背包中对应物品数量为0
	if container:getItemCount( nItemID ) == 0 then
		return;
	end


	-- 若没达到最低限制
	local nConvertLoveVal = math.floor( data["value"] / 100 );
	local nValue;
	if oneConfig.m_uMinValue == 0 then
		nValue = t_configMapToUI[CLIENT_SET_PET_MIN_ZHONG_CHENG]["defaultValue"];
		if nConvertLoveVal > nValue then
			return;
		end
	else
		if nConvertLoveVal > oneConfig.m_uMinValue then
			return;
		end
	end	
	
	local item = container:getItem( nItemID );
	if container:isExpendableItemLimitdByScene( item ) then
		return;
	end
	
	local isSuccesUse = container:usedItem( item:getListType(), item:getGridIdx(), 0, getFightPetIndex() );
	if isSuccesUse then
		self["lastUseZhongChengTime"] = os.clock();
	end
end

t_PetAutoRecoverFrameControl["checkHP"] = 
function ( self, data )
	local nLowID		= data["lowID"];
	local nHeightID		= data["heighID"];
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local petInfo		= petPackage:getPetInfo( nHeightID, nLowID );
	local nCurHP		= data["value"];
	-- 若宠物没处于出征状态，则无须自动使用药品
	if not petPackage:isPetInState( nHeightID, nLowID, CLIENT_PET_STAT_FIGHT ) then
		return;
	end
	
	local t_canUseConfig = self["canUseConfig"];
	-- 先做个排序
	local nOptionType;
	local oneElem;
	for i = 1, table.getn( t_canUseConfig ) do
		oneElem	= t_canUseConfig[i];
		if nCurHP <= oneElem["value"] * petInfo.MaxHP / 100 then
			nOptionType = oneElem["index"];
			break;
		end
	end
	
	-- 若没有开启HP自动恢复选项，则啥也不做
	if not nOptionType then
		return;
	end
	
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	
	local nItemID = oneElem["id"];
	-- 若背包中对应物品数量为0
	if nItemID == 0 or container:getItemCount( nItemID ) == 0 then
		return;
	end

	local item = container:getItem( nItemID );
	if container:isExpendableItemLimitdByScene( item ) then
		return;
	end
	
	local isSuccesUse = container:usedItem( item:getListType(), item:getGridIdx(), 0, getFightPetIndex() );
	if isSuccesUse then
		self["lastUseHPTime"] = os.clock();
	end
end

t_PetAutoRecoverFrameControl["update"] = 
function ( self )
	local mainplayer	= ActorMgr:getMainPlayer();
	if mainplayer == nil then
		return;
	end	

	if self["waitForUseHP"] then
		self:checkHPOnUpdate();
	end

	if self["waitForUseZhongCheng"] then
		self:checkZhongChengOnUpdate();
	end
end

local t_PetAutoRecoverFrameEvents =	{ ["GE_CONTAINER_UPDATE"] = {}, ["GE_PET_STATE_CHANGE"] = {}, ["GE_DEL_PET"] = {}, ["GE_PET_ATTR_CHG"] = {} };

t_PetAutoRecoverFrameEvents["GE_PET_STATE_CHANGE"].func = 
function ()
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local nChkIndex		= -1;
	for i = 0, petPackage:getPetNum() - 1 do
		if petPackage:isPetInState( i, CLIENT_PET_STAT_FIGHT ) then
			nChkIndex = i;
			break;
		end
	end		
	local systemSetting = GameMgr:getUserSystemConfig();
	local nLastPetIndex = systemSetting:getGameOptValue( GAME_OPT_LAST_PET_FIGHT ) - 1;
	local petInfo = petPackage:getPetInfo( nLastPetIndex );
	if nChkIndex ~= -1 then
		if nLastPetIndex == -1 then
			nLastPetIndex = nChkIndex;
			for valueType, data in pairs( t_configMapToUI ) do
				local nValue = data["defaultValue"];
				GameUserConfig:setOnePetConfig( valueType, true, nValue, 0 );
			end
			GameUserConfig:savePetUseItemConfig( );
			CheckAutoRecoverPet();
		end
		if nChkIndex ~= nLastPetIndex then
			for valueType, data in pairs( t_configMapToUI ) do
				local nValue = data["defaultValue"];				
				GameUserConfig:setOnePetConfig( valueType, true, nValue, 0 );
			end
			GameUserConfig:savePetUseItemConfig( );
			CheckAutoRecoverPet();
		end
		systemSetting:setGameOptValue( GAME_OPT_LAST_PET_FIGHT, nChkIndex + 1 );	
	end
end

t_PetAutoRecoverFrameEvents["GE_DEL_PET"].func = 
function ()
	local systemSetting = GameMgr:getUserSystemConfig();
	local nLastPetIndex = systemSetting:getGameOptValue( GAME_OPT_LAST_PET_FIGHT ) - 1;
	if nLastPetIndex ~= -1 and nLastPetIndex ~= t_DropPetInfo["index"] then
		local mainplayer = ActorMgr:getMainPlayer();
		local petPackage = mainplayer:getPet();
		local nIndex = 0;
		for i = 0, petPackage:getPetNum() - 1 do
			petInfo = petPackage:getPetInfo( i );
			if petInfo ~= nil and t_LastPetInfo.WID.nHeight == petInfo.WID.nHeight and t_LastPetInfo.WID.nLow == petInfo.WID.nLow then
				nIndex = i;
				break;
			end
		end
		systemSetting:setGameOptValue( GAME_OPT_LAST_PET_FIGHT, nIndex + 1 );
	end
end

t_PetAutoRecoverFrameEvents["GE_PET_ATTR_CHG"].func = 
function ()
	local mainplayer = ActorMgr:getMainPlayer();
	local petPackage = mainplayer:getPet();
	if petPackage:getPetNum() == 0 then
		return
	end	
	local container	= mainplayer:getContainer();
	for valueType, data in pairs( t_configMapToUI ) do
		local oneConfig = GameUserConfig:getOnePetConfig( valueType );
		local oldItemID	= oneConfig.m_uItemID;
		local newItemID = data["funct"]();
		local nValue = data["defaultValue"];
		if ( newItemID > 0 and oldItemID == 0 ) or oldItemID > 0 then				
			t_PetAutoRecoverFrameControl:buildCanUseHPConfi();
		end
	end
end

t_PetAutoRecoverFrameEvents["GE_CONTAINER_UPDATE"].func = 
function ()
	local mainplayer = ActorMgr:getMainPlayer();
	local petPackage = mainplayer:getPet();
	if petPackage:getPetNum() == 0 then
		return
	end	
	local container	= mainplayer:getContainer();
	for valueType, data in pairs( t_configMapToUI ) do
		local oneConfig = GameUserConfig:getOnePetConfig( valueType );
		local oldItemID	= oneConfig.m_uItemID;
		local newItemID = data["funct"]();
		local nValue = data["defaultValue"];
		if ( newItemID > 0 and oldItemID == 0 ) or oldItemID > 0 then				
			t_PetAutoRecoverFrameControl:buildCanUseHPConfi();
			--[[
			if PetAutoRecoverFrame:IsShown() then
				PetAutoRecoverFrame:Hide();
				PetAutoRecoverFrame:Show();
			end
			--]]
		end
	end

	if PetAutoRecoverFrame:IsShown() then
		local container		= mainplayer:getContainer();
		for _, data in pairs( t_configMapToUI ) do
			local itemBtn	= getglobal( data["itemBtn"] );
			local nItemID	= itemBtn:GetClientUserData( 0 );
			if nItemID > 0 then
				local tex = getglobal( itemBtn:GetName().."IconTexture" );
				tex:SetGray( container:getItemCount( nItemID ) == 0 );
			end
		end
	end
end

function PetAutoRecoverFrame_OnLoad()
	t_PetAutoRecoverFrameEvents.__index = 
	function ( tab, key )
		tab[key] = { func = function () end };
		return tab[key];
	end

	setmetatable( t_PetAutoRecoverFrameEvents, t_PetAutoRecoverFrameEvents );

	for event, _ in pairs( t_PetAutoRecoverFrameEvents ) do
		this:RegisterEvent( event );
	end
	
	local t_textures = 
	{
		["PetAutoRecoverFrame_ItemGridBtn1BoxTexture"] = {	path = "uires\\ZhuJieMian\\1.dds", UV = { x = 244, y = 756, width = 33, height = 33 },
											},

		["PetAutoRecoverFrame_ItemGridBtn2BoxTexture"] = {	path = "uires\\ZhuJieMian\\1.dds", UV = { x = 244, y = 756, width = 33, height = 33 },
											},
			
		["PetAutoRecoverFrame_ItemGridBtn3BoxTexture"] = {	path = "uires\\ZhuJieMian\\1.dds", UV = { x = 279, y = 756, width = 33, height = 33 },
											},
	};
	util.SetTextures( t_textures );
end

function PetAutoRecoverFrame_OnEvent()
	t_PetAutoRecoverFrameEvents[arg1].func();
end

function ClearPetAutoRecoverFrame()
	for _, data in pairs( t_configMapToUI ) do
		local chkBtn = getglobal( data["chkBtn"] );
		chkBtn:setCheckState( false );

		local edit = getglobal( data["edit"] );
		edit:SetText( "" );

		local itemBtn = getglobal( data["itemBtn"] );
		itemBtn:SetClientUserData( 0, 0 );
		local tex		= getglobal( itemBtn:GetName().."IconTexture" );
		local t_tgas	= { [itemBtn:GetName().."IconTexture"] = { path = "uires\\ui\\mask\\TouMingTongDao.tga" }, };
		util.SetTgas( t_tgas );
		tex:SetGray( false );
	end
end

function PetAutoRecoverFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 0, 0 );
	if not GameWizard:isDo( GAMEWIZARD_FIRST_FIRST_SET_PET_USE_ITEM ) then
		GameWizard:doStep( GAMEWIZARD_FIRST_FIRST_SET_PET_USE_ITEM );
		for valueType, _ in pairs( t_configMapToUI ) do
			GameUserConfig:setOnePetConfig( valueType, true, 0, 0 );
		end
		GameUserConfig:savePetUseItemConfig( );
	end

	ClearPetAutoRecoverFrame();

	local petUseItemConfig	= GameUserConfig:getPetUseItemConfig();
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	for valueType, data in pairs( t_configMapToUI ) do
		local oneConfig = petUseItemConfig.m_petUseItemConfigs[valueType];

		local edit		= getglobal( data["edit"] );

		if oneConfig.m_uMinValue > 0 then
			edit:SetText( oneConfig.m_uMinValue );
		elseif oneConfig.m_uMinValue == 0 then
			edit:SetText( data["defaultValue"] );
		else
			edit:SetText( "0" );
		end

		local chkBtn = getglobal( data["chkBtn"] );
		chkBtn:setCheckState( oneConfig.m_bOpen );
		local itemBtn = getglobal( data["itemBtn"] );

		local nItemID = oneConfig.m_uItemID;
		if nItemID > 0 then			
			itemBtn:SetClientUserData( 0, nItemID );
			local itemDef	= getItemDef( nItemID );
			local tex	= getglobal( itemBtn:GetName().."IconTexture" );
			local szPath	= GetItemEquipIconPath()..itemDef.IconID..".tga";
			if not IsInExistence( szPath ) then
				szPath = DEFAULT_ITEM_PATH;
			end

			local t_tgas	= { [itemBtn:GetName().."IconTexture"] = { path = szPath }, };
			util.SetTgas( t_tgas );
			tex:SetGray( container:getItemCount( nItemID ) == 0 );
		elseif nItemID == 0 then
			local nItemID = data["funct"]();
			if nItemID > 0 then
				itemBtn:SetClientUserData( 0, nItemID );
				local itemDef	= getItemDef( nItemID );
				local tex	= getglobal( itemBtn:GetName().."IconTexture" );
				local szPath	= GetItemEquipIconPath()..itemDef.IconID..".tga";
				if not IsInExistence( szPath ) then
					szPath = DEFAULT_ITEM_PATH;
				end

				local t_tgas	= { [itemBtn:GetName().."IconTexture"] = { path = szPath }, };
				util.SetTgas( t_tgas );
				tex:SetGray( container:getItemCount( nItemID ) == 0 );
			else
				itemBtn:SetClientUserData( 0, 0 );
			end
		else
			itemBtn:SetClientUserData( 0, 0 );
		end
		itemBtn:SetClientUserData( 1, valueType );
	end
end

function PetAutoRecoverFrame_HPEdit_OnUpdate()
	--[[
	local itemBtn	= getglobal( "PetAutoRecoverFrame_ItemGridBtn"..this:GetClientID() );
	local nItemID	= itemBtn:GetClientUserData( 0 );
	if nItemID == 0 then
		return;
	end
	--]]

	CheckEditNumberInput( this );
	local nMinHP = tonumber( this:GetText() );
	if nMinHP == nil then
		this:SetText( 0 );
		return
	end

	if nMinHP > 100 then
		this:SetText( 100 );
	end
end

-- 忠诚度栏位
function PetAutoRecoverFrame_ZhongChengEdit_OnUpdate()
	--[[
	local itemBtn	= getglobal( "PetAutoRecoverFrame_ItemGridBtn"..this:GetClientID() );
	local nItemID	= itemBtn:GetClientUserData( 0 );
	if nItemID == 0 then
		return;
	end
	--]]
	
	CheckEditNumberInput( this );
	local nMinZhongCheng = tonumber( this:GetText() );
	if nMinZhongCheng == nil then
		this:SetText( 0 );
		return
	end
end

-- 确定按钮
function PetAutoRecoverFrame_ConfirmBtn_OnClick()
	for valueType, data in pairs( t_configMapToUI ) do		
		local edit		= getglobal( data["edit"] );

		local nValue	= tonumber( edit:GetText() );
		if nValue == nil or nValue <= 0 then
			nValue = -1;
		end
		if nValue == data["defaultValue"] then
			nValue = 0;
		end

		local chkBtn	= getglobal( data["chkBtn"] );
		local itemBtn	= getglobal( data["itemBtn"] );

		local nItemID	= itemBtn:GetClientUserData( 0 );
		local petUseItemConfig = GameUserConfig:getPetUseItemConfig();
		local oneConfig = petUseItemConfig.m_petUseItemConfigs[valueType];
		if nItemID == nil or nItemID == 0 then
			if oneConfig.m_uItemID == 0 and data["funct"]() == 0 then
				GameUserConfig:setOnePetConfig( valueType, chkBtn:GetCheckState(), nValue, 0 );
			else
				GameUserConfig:setOnePetConfig( valueType, chkBtn:GetCheckState(), nValue, -1 );
			end
		else
			if nItemID == data["funct"]() then
				GameUserConfig:setOnePetConfig( valueType, chkBtn:GetCheckState(), nValue, 0 );
			else
				GameUserConfig:setOnePetConfig( valueType, chkBtn:GetCheckState(), nValue, nItemID );
			end
		end
	end

	GameUserConfig:savePetUseItemConfig( );
	PetAutoRecoverFrame:Hide();
	
	CheckAutoRecoverPet();
end

function PetAutoRecoverFrame_ItemGridBtnOnEnter( szBtnName )
	local btn			= getglobal( szBtnName );
	local mainplayer	= ActorMgr:getMainPlayer();
	if mainplayer == nil then
		return;
	end
	local container		= mainplayer:getContainer();
	local id			= btn:GetClientUserData( 0 );
	local item			= container:getItem( id );
	if item:getItemId() <= 0 then
		local t_hideIcons = { "NormalItemTipsFrame", "ArmItemTipsFrame", "SecondArmItemTipsFrame", };
		util.HideIcons( t_hideIcons );
		return;
	end
	
	ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item, PetAutoRecoverFrame_ItemGridBtnOnEnter, szBtnName);
end

local t_btnMapToResult = 
{
["PetAutoRecoverFrame_ItemGridBtn1"] = { resultID = RESULT_ADD_POWER, desc = "宠物药水", defaultValue = 50, 
											edit = "PetAutoRecoverFrame_FirstHPEdit" },
["PetAutoRecoverFrame_ItemGridBtn2"] = { resultID = RESULT_ADD_POWER, desc = "宠物药水", defaultValue = 30, 
											edit = "PetAutoRecoverFrame_SecondHPEdit" },
["PetAutoRecoverFrame_ItemGridBtn3"] = { resultID = RESULT_PET_ADD_LOVEVAL, desc = "增加宠物忠诚度道具", defaultValue = 50, 
											edit = "PetAutoRecoverFrame_ZhongChengEdit" },
};

function PetAutoRecoverFrame_ItemGridBtn_OnEnter()
	local id = this:GetClientUserData( 0 );
	if id == 0 then
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "请放入"..t_btnMapToResult[this:GetName()]["desc"], 
							frame = this:GetParent(), button = this:GetName(), horizonal = "right" } );
		return;
	end
	
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	if container:getItemCount( id ) == 0 then
		local itemDef = getItemDef( id );
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "已经没有"..itemDef.Name.."，请放入其他物品", 
							frame = this:GetParent(), button = this:GetName(), horizonal = "right" } );
	else
		PetAutoRecoverFrame_ItemGridBtnOnEnter( this:GetName() );
	end
	
end

function PetAutoRecoverFrame_ItemGridBtn_OnLeave()
	local t_hideIcons = { "NormalItemTipsFrame", "ArmItemTipsFrame", "SecondArmItemTipsFrame", "GameTooltip", "SpellTipsFrame" };
	util.HideIcons( t_hideIcons );
end

function Save2PetAutoRecoverFrame_ItemGridBtn( item, itemBtn )
	local resultID		= t_btnMapToResult[itemBtn:GetName()]["resultID"];
	local itemDef		= item:getItemDef();
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local nPetIndex		= getFightPetIndex();
	local petInfo;
	if nPetIndex > -1 then
		petInfo	= petPackage:getPetInfo( nPetIndex );
	else
		local systemSetting = GameMgr:getUserSystemConfig();
		local nLastPetIndex = systemSetting:getGameOptValue( GAME_OPT_LAST_PET_FIGHT ) - 1;
		petInfo	= petPackage:getPetInfo( 0 );
		if nLastPetIndex > -1 then
			petInfo	= petPackage:getPetInfo( nLastPetIndex );
		end
	end
	local petLv = petInfo.Level;

	if itemDef.WearPre.RoleLevelMin > petLv then
		ShowMidTips("当前宠物等级不能使用该物品");
		return false;
	end

	local bFitItem		= false;
	for i = 1, MAX_ITEM_RESULT do
		local oneResult = itemDef.Result[i-1];
		if oneResult.ResultID == resultID then
			bFitItem	= true;
		end
	end
	-- TODO: 加个判断玩家是否能使用
	if not bFitItem then
		ShowMidTips( "不是"..t_btnMapToResult[itemBtn:GetName()]["desc"].."，不能放入" );
		return;
	end

	local nItemID	= item:getItemId();
	if nItemID == -1 then
		itemBtn:SetClientUserData( 0, 0 );
		local tex	= getglobal( this:GetName().."IconTexture" );
		local t_tgas	= { [this:GetName().."IconTexture"] = { path = "uires\\ui\\mask\\TouMingTongDao.tga" }, };
		util.SetTgas( t_tgas );
		tex:SetGray( false );
	else
		itemBtn:SetClientUserData( 0, nItemID );
		local tex		= getglobal( itemBtn:GetName().."IconTexture" );
		local szPath	= GetItemEquipIconPath()..itemDef.IconID..".tga";
		if not IsInExistence( szPath ) then
			szPath = DEFAULT_ITEM_PATH;
		end
		local t_tgas	= { [itemBtn:GetName().."IconTexture"] = { path = szPath }, };
		util.SetTgas( t_tgas );
		tex:SetGray( false );
	end

	local edit		= getglobal( t_btnMapToResult[itemBtn:GetName()]["edit"] );
	local nValue	= tonumber( edit:GetText() );
	if nValue == nil then
		nValue = 0;
	end
end

function PetAutoRecoverFrame_ItemGridBtn_Drag()	
	local player = ActorMgr:getMainPlayer()
	if not player then
		return;
	end
	local container = player:getContainer()
	local id = this:GetClientUserData( 0 );
	local btnIndex = this:GetClientUserData( 1 ) + 1;

	if id <=0 then
		return;
	end

	local item = container:getItem(id)
	if not item then
		return
	end

	if item:getItemId() > 0 then
		UIMSG:getUIGV().DragFrom = DRAG_PET_AUTO_ITEM;
		local IconTex = getglobal(this:GetName().."IconTexture");
		UIBeginDrag(IconTex:GetTexture(), item:getListType(), item:getGridIdx(), btnIndex);
	end
end

function PetAutoRecoverFrame_ItemGridBtn_ReceiveDrag()
	local srcList = UIMSG:getUIGV().DragFrom;
	if srcList ~= CONTAINER_TYPE_SUNDRIES and srcList ~= CONTAINER_TYPE_STUFF then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	end

	local srcGrid		= UIGetDragUserData1();
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local item		= container:getItem( srcList, srcGrid );
	Save2PetAutoRecoverFrame_ItemGridBtn( item, this );
	UIEndDrag();
	UIMSG:getUIGV().DragFrom = DRAG_NONE;
end

function PetAutoRecoverFrame_ItemGridBtn_OnClick()
	if arg1 ~= "RightButton" then
		return;
	end

	local id = this:GetClientUserData( 0 );
	if id == 0 then
		return;
	end

	this:SetClientUserData( 0, 0 );
	local tex		= getglobal( this:GetName().."IconTexture" );
	local t_tgas	= { [this:GetName().."IconTexture"] = { path = "uires\\ui\\mask\\TouMingTongDao.tga" }, };
	util.SetTgas( t_tgas );
	tex:SetGray( false );
end

function PetAutoRecoverFrame_ItemGridBtn_ClearBtn( index )
	this:SetClientUserData( 0, 0 );
	local tex	= getglobal( "PetAutoRecoverFrame_ItemGridBtn" .. index .."IconTexture" );
	local t_tgas	= { ["PetAutoRecoverFrame_ItemGridBtn" .. index .."IconTexture"] = { path = "uires\\ui\\mask\\TouMingTongDao.tga" }, };
	util.SetTgas( t_tgas );
	tex:SetGray( false );
end

function IsItemPetLearnSKillBook(id)
	local ItemDef = getItemDef(id)
	if RESULT_PET_SKILL_BOOK == ItemDef.Result[0].ResultID then
		return true
	end
	return false
end

function UseItemLearnPetSkill(item, idx)
	if not item then
		return
	end
	local player = ActorMgr:getMainPlayer()
	local petMgr = player:getPet()
	local index
	for i = 0, petMgr:getPetNum() - 1 do
		if petMgr:isPetInState(i, CLIENT_PET_STAT_FIGHT) then
			index = i
			break
		end
	end
	if not index then
		return ShowMidTips("当前没有召唤宠物")
	end
	for state, tips in pairs(t_cannotLearnSkillStateDesc) do
		if petMgr:isPetInState(index, state) then
			return ShowMidTips(tips.."不能进行学习技能操作")
		end
	end
	local info = petMgr:getPetInfo(index)
	local ItemDef = item:getItemDef()
	for i = 1, info.PetSkills.SkillGridNum do
		local skills = info.PetSkills.SkillGrid[i - 1]
		if skills.SkillID ~= 0 and skills.SkillLevel ~= 0 and skills.SkillID == ItemDef.Result[0].ResultVal1 then
			return ShowMidTips("已有该技能，不能重复学习")
		end
	end
	if GameStallManager:isInStallIPet(SHITYPE, info.WID.nHeight, info.WID.nLow) then
		return ShowMidTips( "该宠物无法学习技能" )
	end
	petMgr:petSkillLearn(info.WID.nHeight, info.WID.nLow, idx or 0, item)
end

function PetAddLoveLevelMessageFrameOkBtn_OnClick()
	local high = PetAddLoveLevelMessageFrame:GetClientUserData( 0 )
	local low = PetAddLoveLevelMessageFrame:GetClientUserData( 1 )
	local player = ActorMgr:getMainPlayer()
	local petMgr = player:getPet()
	if GameStallManager:isInStallIPet( SHITYPE, t_mainSelPetInfo.heightId, t_mainSelPetInfo.lowId ) then
		ShowMidTips( "宠物上架中，不能进行爱心操作" )
	elseif GameExchange:isInExchangePetByWID( t_mainSelPetInfo.heightId, t_mainSelPetInfo.lowId ) then
		ShowMidTips( "宠物正在交易中，不能进行爱心操作" )
	else
		petMgr:loveLevelUp( high, low )
	end	
	util.closeParent(this)
end


function GetPetLoveLevelDesc( level )
	local tLoveLevelDesc = { [0] = "可交易两次", [1] = "可交易一次", [2] = "不可交易" }
	return tLoveLevelDesc[level]
end

---------------------------------------------宠物解绑-----------------------------------
function PetUnBindFrameUnBindBtn_OnClick()
	local t_operationSelPetInfo = GetOperationSelPetInfo();
	if t_operationSelPetInfo["id"] == 0 then
		return;
	end
	local mainplayer 	= ActorMgr:getMainPlayer();
	if mainplayer == nil then return; end
	local container		= mainplayer:getContainer();
	local petPackage	= mainplayer:getPet();
	local petInfo		= petPackage:getPetInfo( t_operationSelPetInfo["index"] );
	if petInfo == nil then 
		ShowMidTips("该宠物已不存在");
		return ;
	end
	if petPackage:isExtraClanJiYangPet( petInfo ) then 
		ShowMidTips("宠物正在寄养中，请前往公会城市取消寄养再进行该操作！");
		return ;
	end
	local useDef 		= GameArmProce:getUseDef( USE_ID_PET_STEP_UP_TOLUA );

	local nNeedMoney 	= 0;
	if t_PetUnBindNeed[petInfo.PetGenius] ~= nil then
		nNeedMoney = t_PetUnBindNeed[petInfo.PetGenius];
	end
	if mainplayer:getMoney() < nNeedMoney then 
		ShowMidTips("所需银币不足！");
		ShowBuyMoneyFrame();
		return ;
	end
	local nTotalItemNum = container:getItemTotalNum( PET_UNBIND_ITEM_ID_TOLUA );
	local nItemNum		= 1;
	if nTotalItemNum == 0 or nTotalItemNum < nItemNum then 
		ShowMidTips("忘情果实不足！");
		return ;
	end

	petPackage:petStartUnBind( t_operationSelPetInfo["heightId"], t_operationSelPetInfo["lowId"] );	
end
function PetUnBindFrame_OnLoad()
	this:RegisterEvent( "GE_DEL_PET" );
	this:RegisterEvent( "GE_CONTAINER_UPDATE" );
	this:RegisterEvent("GE_PET_ATTR_CHG");
	this:RegisterEvent("GE_GET_NEW_PET");
end
function UpdatePetUnBindList( petPackage )
	PetUnBindFrame_PetListBackGroundHeadFont:SetText( "宠物列表"..petPackage:getPetNum().."/".. petPackage:getPetMaxNum() );
	SetScrollBar( "PetUnBindFrame_ScrollBar", MAX_PET_NUM, petPackage:getPetNum() );	
end
function UpdatePetUnBindItemNum()
	local mainplayer 	= ActorMgr:getMainPlayer()
	local petMgr 		= mainplayer:getPet()
	local t_selUseItem = GetOperationSelUseItem();
	if t_selUseItem["id"] == 0 then
		PetUnBindFrame_IconBtn:SetClientUserData( 0, 0 );
		PetUnBindFrame_CostCaiLiaoBtnFont:SetText("");
		return;
	end

	local container		= mainplayer:getContainer();
	local nTotalItemNum = container:getItemTotalNum( PET_UNBIND_ITEM_ID_TOLUA );
	local nItemNum		= 1 ;
	PetUnBindFrame_CostCaiLiaoBtnFont:SetText( nTotalItemNum.."/"..nItemNum );
end
function PetUnBindFrame_OnEvent()
	if not this:IsShown() then
		return;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	local petPackage = mainplayer:getPet();
	if(arg1 == "GE_DEL_PET") then
		if petPackage:getPetNum() == 0 then
			ShowMidTips( "身上没宠物，不能进行该操作，自动关闭面板" );
			this:Hide();
			PetFrame:Hide();
			return;
		end
		UpdatePetStepUpList( petPackage );
		
		UpdateOperationPetList( "PetUnBindFrame" );
		SetPetInfoShow( "PetUnBindFrame"  );	
	end
	if(arg1 == "GE_CONTAINER_UPDATE") then
		local container		= mainplayer:getContainer();		
		local nTotalItemNum	= container:getItemTotalNum( PET_UNBIND_ITEM_ID_TOLUA );
		if nTotalItemNum == 0 then 
			local t_selUseItem = { id = 0, selItem = nil, listType = 0, gridx = 0, num = 0 };
			local tex = getglobal( "PetUnBindFrame_IconBtnIcon" ); 
			tex:SetTexture( "uires\\ZhuJieMian\\1.dds" );
			tex:SetTexUV( 749, 472, 44, 44 );
			SetOperationSelUseItem( t_selUseItem );
		else 
			local unBindItem = GetOperationSelUseItem();
			if unBindItem["id"] ~= PET_UNBIND_ITEM_ID_TOLUA and PetUnBindFrame:IsShown() then
				local item 		= container:getItem( PET_UNBIND_ITEM_ID_TOLUA );
				local tex		= getglobal( "PetUnBindFrame_IconBtnIcon" );
				local itemdef	= item:getItemDef();
				local IconPath	= GetItemEquipIconPath()..itemdef.IconID..".tga";
				tex:SetTexture( ( IsInExistence( IconPath ) and IconPath or DEFAULT_ITEM_PATH ) );
				tex:SetTexRelUV( 1, 1 );
				t_selUseItem = { selItem = item, id = PET_UNBIND_ITEM_ID_TOLUA, listType = item:getListType(), gridx = item:getGridIdx() };
				SetOperationSelUseItem( t_selUseItem );
				for i = 0, container:getContainerMaxNum(CONTAINER_TYPE_SUNDRIES) - 1 do
					local bagItem = container:getItem(CONTAINER_TYPE_SUNDRIES, i)
					if bagItem:getItemId() == PET_UNBIND_ITEM_ID_TOLUA then
						LockItem(CONTAINER_TYPE_SUNDRIES, i, PET_UNBIND_ITEM_ID_TOLUA, "PetUnBindFrame")
					end
				end
			end
		end 
		UpdatePetUnBindItemNum();
	end
	if (arg1 == "GE_GET_NEW_PET") then
		UpdatePetUnBindList( petPackage );
		UpdateOperationPetList( "PetUnBindFrame" );
	end
	if (arg1 == "GE_PET_ATTR_CHG") then
		UpdatePetUnBindList( petPackage );
		UpdateOperationPetList( "PetUnBindFrame" );
	end
end
function CheckPetUnBindItem()
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local item = container:getItem( PET_UNBIND_ITEM_ID_TOLUA );
	local t_selUseItem = GetOperationSelUseItem();
	if t_selUseItem["id"] ~= 0 then
		ClearPet_OperationItem();
	end

	local szFrameName = "PetUnBindFrame";

	local nTotalItemNum	= container:getItemTotalNum( PET_UNBIND_ITEM_ID_TOLUA );
	local btn			= getglobal( "PetUnBindFrame_IconBtn" );
	btn:SetClientUserData( 0, PET_UNBIND_ITEM_ID_TOLUA );
	btn:SetClientUserData( 1, nTotalItemNum );
	if nTotalItemNum == 0 then
		return;
	end
	-- 设置图标
	local tex		= getglobal( "PetUnBindFrame_IconBtnIcon" );
	local itemdef	= item:getItemDef();
	local IconPath	= GetItemEquipIconPath()..itemdef.IconID..".tga";
	tex:SetTexture( ( IsInExistence( IconPath ) and IconPath or DEFAULT_ITEM_PATH ) );
	tex:SetTexRelUV( 1, 1 );
	
	t_selUseItem = { selItem = item, id = item:getItemId(), listType = item:getListType(), gridx = item:getGridIdx() };
	SetOperationSelUseItem( t_selUseItem );
	
	for i = 0, container:getContainerMaxNum(CONTAINER_TYPE_SUNDRIES) - 1 do
		local bagItem = container:getItem(CONTAINER_TYPE_SUNDRIES, i)
		if bagItem:getItemId() == PET_UNBIND_ITEM_ID_TOLUA then
			LockItem(CONTAINER_TYPE_SUNDRIES, i, PET_UNBIND_ITEM_ID_TOLUA, szFrameName)
		end
	end
	UpdatePetUnBindItemNum();
end
local szPetUnBindDesc = [[
#cffffbe宠物解绑说明：#n
#cffffff☆ 每次解绑需要一个忘情果实#n
#cffffff☆ 宠物解绑完成后回变成“相见恨晚”状态#n
#cffffff☆ 宠物资质越高消耗的银币越多#n]]
function PetUnBindFrame_OnShow()
	local rich = getglobal( "PetUnBindFrame_TipsRichText");
	rich:SetText( szPetUnBindDesc, 255,255,190 );
	local t_selUseItem = { id = 0, selItem = nil, listType = 0, gridx = 0, num = 0 };
	SetOperationSelUseItem( t_selUseItem );
	
	DefaultShow( this:GetName() );
	local mainplayer			= ActorMgr:getMainPlayer();
	local petPackage			= mainplayer:getPet();
	local t_operationSelPetInfo = GetOperationSelPetInfo();
	
	local petInfo				= petPackage:getPetInfo( t_operationSelPetInfo["heightId"], t_operationSelPetInfo["lowId"] );
	SetPetInfoShow( this:GetName() );
	local modelview = getglobal( "PetUnBindFrame_IconBtnModelView" );
	modelview:Hide();
	-- 若宠物面板没显示，则随着宠物操作面板一起显示
	this:SetPoint("topleft", "UIClient", "topleft", 428, 163)
	local frame = getglobal( "PetFrame" );
	SetMainSelPetInfo(t_operationSelPetInfo);
	if not frame:IsShown() then
		frame:Show();
		frame:SetPoint("topright", "UIClient", "topleft", 428, 163)
	end
	UpdatePetFrame();
	CheckPetUnBindItem();
end
function PetUnbindSucc( nSucc )
	if nSucc == 1 then 	
		ShowMidTips("您的宠物解绑成功！");
		local modelview = getglobal("PetUnBindFrame_IconBtnModelView");
		modelview:setPetModel(0, 0, "effect\\entity\\fumo01.ent")
		modelview:setEntityPosition(0, 0, 100, 0)
		if not modelview:IsShown() then
			modelview:Show();
		end
	end
end
