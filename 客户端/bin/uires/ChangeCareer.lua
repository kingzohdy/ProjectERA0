-- 5030021
CAREER_WARRIOR_SINGLEHAND	= 17; 	-- /* 刀分支技能的武器需求为单手刀 */
CAREER_WARRIOR_DOUBLEHAND	= 33; 	-- /* 剑分支技能的武器需求为对手剑 */

CAREER_MAGICOR_SINGLEHAND	= 18; 	-- /* 冰分支技能的武器需求为单手魔杖 */
CAREER_MAGICOR_DOUBLEHAND	= 34; 	-- /* 火分支技能的武器需求为双手魔杖 */

CAREER_GUNER_SINGLEHAND		= 19; 	-- /* 手枪分技能支的武器需求为单手枪 */
CAREER_GUNER_DOUBLEHAND		= 35; 	-- /* 机枪分技能支的武器需求为机枪 */

CAREER_CHURCH_SINGLEHAND	= 20; 	-- /* 圣分支技能的武器需求为单手圣杖 */
CAREER_CHURCH_DOUBLEHAND	= 36; 	-- /* 雷分支技能的武器需求为双手圣杖 */
ZHUAN_ZHI_RACE_LIMIT		= 0x10;

t_orginCareerToChgCareerMap = 
{
[CLIENT_CAREER_WARRIOR]	= { CLIENT_CAREER_WARRIOR_SINGLEHAND,	CLIENT_CAREER_WARRIOR_DOUBLEHAND },
[CLIENT_CAREER_MAGICOR]	= { CLIENT_CAREER_MAGICOR_SINGLEHAND,	CLIENT_CAREER_MAGICOR_DOUBLEHAND },
[CLIENT_CAREER_GUNER]		= { CLIENT_CAREER_GUNER_SINGLEHAND,	CLIENT_CAREER_GUNER_DOUBLEHAND },
[CLIENT_CAREER_CHURCH]		= { CLIENT_CAREER_CHURCH_SINGLEHAND,	CLIENT_CAREER_CHURCH_DOUBLEHAND },
};

t_allCareerDesc = 
{
[CLIENT_CAREER_WARRIOR]			= { desc = "战士",  },		
[CLIENT_CAREER_MAGICOR]			= { desc = "法师" },
[CLIENT_CAREER_GUNER]				= { desc = "枪手" },		
[CLIENT_CAREER_CHURCH]				= { desc = "祭司" },

[CLIENT_CAREER_WARRIOR_SINGLEHAND]	= { desc = "守护者",	normalUV	= { x = 369, y = 793, width = 122, height = 42 }, 
													highlightUV = { x = 492, y = 793, width = 122, height = 42 }, 
													pushedUV	= { x = 615, y = 793, width = 122, height = 42 },
													size		= { x = 122, y = 42 },
								},		

[CLIENT_CAREER_WARRIOR_DOUBLEHAND]	= { desc = "征服者",	normalUV	= { x = 0, y = 793, width = 122, height = 42 }, 
													highlightUV = { x = 123, y = 793, width = 122, height = 42 }, 
													pushedUV	= { x = 246, y = 793, width = 122, height = 42 },
													size		= { x = 122, y = 42 },
								},	

[CLIENT_CAREER_MAGICOR_SINGLEHAND]	= { desc = "聚能者",	normalUV	= { x = 369, y = 922, width = 122, height = 42 }, 
													highlightUV = { x = 492, y = 922, width = 122, height = 42 }, 
													pushedUV	= { x = 615, y = 922, width = 122, height = 42 },
													size		= { x = 122, y = 42 },
								},	

[CLIENT_CAREER_MAGICOR_DOUBLEHAND]	= { desc = "炼魂师",	normalUV	= { x = 0, y = 922, width = 122, height = 42 }, 
													highlightUV = { x = 123, y = 922, width = 122, height = 42 }, 
													pushedUV	= { x = 246, y = 922, width = 122, height = 42 },
													size		= { x = 122, y = 42 },
								},

[CLIENT_CAREER_GUNER_SINGLEHAND]	= { desc = "猎杀者",		normalUV	= { x = 0, y = 879, width = 122, height = 42 }, 
													highlightUV = { x = 123, y = 879, width = 122, height = 42 },
													pushedUV	= { x = 246, y = 879, width = 122, height = 42 },
													size		= { x = 122, y = 42 },
								},
								
[CLIENT_CAREER_GUNER_DOUBLEHAND]	= { desc = "毁灭者",		normalUV	= { x = 369, y = 879, width = 122, height = 42 }, 
													highlightUV = { x = 492, y = 879, width = 122, height = 42 },
													pushedUV	= { x = 615, y = 879, width = 122, height = 42 },
													size		= { x = 122, y = 42 },
								},

[CLIENT_CAREER_CHURCH_SINGLEHAND]	= { desc = "圣佑者",	normalUV	= { x = 369, y = 836, width = 122, height = 42 }, 
													highlightUV = { x = 492, y = 836, width = 122, height = 42 },
													pushedUV	= { x = 615, y = 836, width = 122, height = 42 },
													size		= { x = 122, y = 42 },
								},
								
[CLIENT_CAREER_CHURCH_DOUBLEHAND]	= { desc = "驭雷师",	normalUV	= { x = 0, y = 836, width = 122, height = 42 }, 
													highlightUV = { x = 123, y = 836, width = 122, height = 42 },
													pushedUV	= { x = 246, y = 836, width = 122, height = 42 },
													size		= { x = 122, y = 42 },
								},
};

local t_changeCareerControl = { canSelectCareers = {}, curSelectCareer = CLIENT_CAREER_NON, freeStartTime = 0, MAX_KEEP_FREE_TIME = 10,
								rotateAngle = 0 };
function GetChangeCareerControl()
	return t_changeCareerControl;
end

function InitChangeCareerFrame()
	t_changeCareerControl:init();
end

t_changeCareerControl["isFitRace"] = 
function ( self, data )
	local actor			= data["actor"];
	local nCompareRace	= data["compareRace"];
	local nActorRace	= actor:getRace();

	--如果无职业要求，则肯定能用
	if nCompareRace == 0 then
		return true;
	end
	-- 若物品是转职后的，而人是转职前的，那肯定不能用
	if nCompareRace > ZHUAN_ZHI_RACE_LIMIT and nActorRace <= ZHUAN_ZHI_RACE_LIMIT then
		return bitAndOperation( nCompareRace, 0x0f ) == nActorRace;
	end
	
	-- 若都是转职后的情况
	if nCompareRace > ZHUAN_ZHI_RACE_LIMIT and nActorRace > ZHUAN_ZHI_RACE_LIMIT then
		return nCompareRace == nActorRace;
	end
	
	-- 若都是没转职的情况
	if nCompareRace <= ZHUAN_ZHI_RACE_LIMIT and nActorRace <= ZHUAN_ZHI_RACE_LIMIT then
		return nCompareRace == nActorRace;
	end
	
	-- 若物品没转职，任务转职
	if nCompareRace <= ZHUAN_ZHI_RACE_LIMIT and nActorRace > ZHUAN_ZHI_RACE_LIMIT then
		return nCompareRace == bitAndOperation( nActorRace, 0x0f );
	end
end

t_changeCareerControl["isChangeCareer"] = 
function ( self, data )
	return data["race"] > CLIENT_CAREER_CHURCH;
end

function IsFitRace( data )
	return t_changeCareerControl:isFitRace( data );
end


t_changeCareerControl["init"] = 
function ( self )
	self["canSelectCareers"]	= {};
	self["curSelectCareer"]		= CLIENT_CAREER_NON;
	self["freeStartTime"]		= 0;
	self["rotateAngle"]			= 0;
	self["listType"]			= -1;
	self["gridx"]				= -1;
end

t_changeCareerControl["getInitRace"] = 
function ( self, data )
	local race = data["race"];
	if race <= 0x10 then
		return race;
	end

	return bitAndOperation( race, 0x0f );
end

t_changeCareerControl["setChangeCareerInfo"] = 
function ( self, data )
	self["listType"]	= data["listType"];
	self["gridx"]		= data["gridx"];
end

t_changeCareerControl["setFreeStartTime"] = 
function ( self, data )
	self["freeStartTime"]	= data["Time"];
end

t_changeCareerControl["getFreeStartTime"] = 
function ( self )
	return self["freeStartTime"];
end

t_changeCareerControl["isChangeCareerItem"] = 
function ( self, data )
	-- 5030021
	if data["id"] == CLIENT_CAREERCHG_ITEMID then
		return true;
	end

	return false;
end

-- TODO: 等协议更新后再去掉
t_changeCareerControl["canStartChangeCareer"] = 
function ( self, data )
	local mainplayer	= ActorMgr:getMainPlayer();
	local nCurLv		= mainplayer:getLv();
	
	-- 对等级封印判断
	local fengyinlv,fengyinEnd = GetFengYinData()
	if fengyinlv > 0 and fengyinlv < CLIENT_CAREERCHG_LEVEL + 1 then
		local dt = math.floor( (fengyinEnd - getServerTime().sec)/3600 )
		local szTime = ""
		if dt < 1 then
			szTime = math.ceil((fengyinEnd - getServerTime().sec)/60) .. "分钟"
		else
			szTime = dt .. "小时"
		end
		ShowMidTips( "等级封印中,暂时不能转职，距离解封还有" .. szTime )
		return
	end
	if mainplayer:getRace() > 0x10 then
		ShowMidTips( "已转职过，不能再转职" );
		return false;
	end

	if nCurLv < CLIENT_CAREERCHG_LEVEL then
		ShowMidTips( "请先达到"..CLIENT_CAREERCHG_LEVEL.."级再进行转职" );
		return false;
	end

	if mainplayer:getExp() < mainplayer:getLvExp( nCurLv ) then
		ShowMidTips( "你当前升级经验不足" );
		return false;
	end
	
	local t_itemControl = GetItemControl();
	local itemDef		= getItemDef( data["id"] );
	if not t_itemControl:isSatisfyLv( { itemDef = itemDef } ) then
		return false;
	end 
	
	return true;
end

t_changeCareerControl["startChangeCareer"] = 
function ( self, data )
	local frame			= getglobal( "ChangeCareerFrame" );
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();

	self:setChangeCareerInfo( data );
	frame:Show();
	
	local item = container:getItem( data["listType"], data["gridx"] );
	-- 如果物品添加成功则锁住该物品
	LockItem( data["listType"], data["gridx"], item:getItemId(), "ChangeCareerFrame" );
end

t_changeCareerControl["setSelectCareer"] = 
function ( self, data )
	self["curSelectCareer"]	= data["career"];
end

t_changeCareerControl["getSelectCareer"] = 
function ( self )
	return self["curSelectCareer"];
end

t_changeCareerControl["updateCanSelectCareers"] = 
function ( self, data )
	local nInitCareer			= data["initCareer"];
	self["canSelectCareers"]	= t_orginCareerToChgCareerMap[nInitCareer];
end

t_changeCareerControl["getCanSelectCareers"] = 
function ( self )
	return self["canSelectCareers"];
end
-- bitAndOperation

local t_ChangeCareerFrameEvents =	{ ["GE_CHANGE_COLLECTTYPE"]		= {}, ["GE_OPEN_CAREER_CHG_FRAME"] = {},	
									};

t_ChangeCareerFrameEvents["GE_CHANGE_COLLECTTYPE"].func = 
function ()
	if ChangeCareerFrame:IsShown() then
		ChangeCareerFrame:Hide();
	end
end

t_ChangeCareerFrameEvents["GE_OPEN_CAREER_CHG_FRAME"].func = 
function ()
	ChangeCareerFrame:Show();
end

t_extendedRacedesc =
{
	[CLIENT_CAREER_WARRIOR] = 
	{ "战士", 

	"    他们是天生的勇者，凭借勇气与不屈的意志支撑到战斗的最后一刻。\n    作为传统战斗技巧的继承者，战士认真的对待着每一场战斗。\n    他们勇敢无惧，将信念凝聚成利剑。\n    他们无坚不摧，用身躯炼铸成铁壁。\n    战士走过的路没有退缩的脚印，因为身后是必须要去保护的战友。", 
		
	"武器类型：刀、对剑、盾\n可转职为：守护者、征服者\n职业风格：近战物理攻击、仇恨控制能力" 
	}, -- 战士


	[CLIENT_CAREER_MAGICOR] = 
	{ "法师", 

	"    他们是恐怖的破坏者，催动着元素魔法毁灭一切。\n    作为打破常理的战斗逻辑，法师改变了人类陈旧的认知。\n    他们是元素的掌控者，顷刻间凝聚毁灭。\n    他们是聪慧的策划者，弹指间扭转局势。\n    他们用智慧征服一切，用魔法摧毁一切，几乎没人能逃脱他们的吟唱。", 
		
	"武器类型：魔杖、法器\n可转职为：炼魂师、聚能者\n职业风格：远程魔法伤害、群体控制能力" 
	}, -- 法师


	[CLIENT_CAREER_GUNER] = 
	{ "枪手", 
	
	"    他们是冷酷的狙杀者，每一次扣动板机都是精密的部署。\n    作为重要的牵制者，枪手兼备冷静的思考与果断的判断，把敌人压制在远处。\n    他们是天生的狙击手，弹无虚发。\n    他们是无情的杀戮手，狂热不羁。\n    他们把敌人玩弄于股掌之中，不留一丝喘息，然后逐个击破。", 
		
	"武器类型：右手枪、左手枪、机枪\n可转职为：猎杀者、毁灭者\n职业风格：远程物理伤害、单体控制能力" 
	}, -- 枪手


	[CLIENT_CAREER_CHURCH] = 
	{ "祭司", 
	
	"    他们是神的传颂者，贯彻崇高的信仰与教诲在战场上保护着他们的同伴。\n    作为队伍的心脏，祭司只有一个念头，坚守至生命的最后一秒，阻止任何牺牲。\n    他们如晨曦的黎明，为世界带来曙光。\n    他们如雷电的审判，为罪人带来噩耗。\n    相信祭司，他们能走多远，同伴就能走多远。", 
		
	"武器类型：圣杖、圣器\n可转职为：圣佑者、驭雷师\n职业风格：远程魔法伤害、治疗能力" 
	},  -- 祭司


	[CLIENT_CAREER_WARRIOR_SINGLEHAND] = 
	{ "守护者", 

	"    他们是冒险时的领导者，战斗时的守护者。他们拥有最高的防御力，是最坚强的壁垒，可在短时间内大大提升自己的防御力，并利用范围技能与其超强的抵抗伤害能力保护他的队友。", 
	
	"擅长武器：刀、盾", 
	
	"职业特点：高生命值、高防御力" 
	}, -- 守护者


	[CLIENT_CAREER_WARRIOR_DOUBLEHAND] = 
	{ "征服者", 

	"    他们是狂暴的斗士，天生的杀戮者。他们对战斗有着异于常人的敏锐，通过各种牵制技能来左右敌人，然后以其惊人的潜能瞬间激发强大的伤害，给予敌人致命的伤害。", 
	
	"擅长武器：对剑", 
	
	"职业特点：单体高伤害、牵制技能" 
	}, -- 征服者


	[CLIENT_CAREER_MAGICOR_SINGLEHAND] = 
	{	"聚能者", 

	"    他们掌握非凡的范围魔法，是死亡的使者。身着华丽法袍的他们对战斗有着独特的理解，拥有着卓越的大范围攻击魔法，结合着减速效果使他们无所不能。", 
		
	"擅长武器：魔杖、法器", 
	
	"职业特点： 远程范围魔法伤害、减速技能" 
	}, -- 聚能者


	[CLIENT_CAREER_MAGICOR_DOUBLEHAND] = 
	{	"炼魂师", 

	"    毁灭就是他们的代名词。他们强大的破坏法术攻击是来自地狱的挽歌，是团队中必不可少的利刃，从天而降的地狱之火挥手间让敌人灰飞烟灭。", 
		
	"擅长武器：魔杖、法器", 
		
	"职业特点：远程单体魔法伤害" 
	}, -- 炼魂师


	[CLIENT_CAREER_GUNER_SINGLEHAND] = 
	{ "猎杀者", 
	
	"    他们是一切罪恶的仇敌，极具魅力的杀手。他们使用双手枪作为自己的武器， 通过埋设地雷，精确的射击牵制着敌人，扼杀在措手不及中，是队伍重要的游动部署。", 
	
	"擅长武器：右手枪、左手枪", 
	
	"职业特点： 埋设地雷、远程物理伤害" 
	}, -- 猎杀者


	[CLIENT_CAREER_GUNER_DOUBLEHAND] = 
	{ "毁灭者", 
	
	"    他们是爆破的专家，战场的屠戮者。他们精通各种机枪，强大的火力支援成为了队友的依赖，超强的群体杀伤能力让人不容忽视，顷刻间摧毁一切是他们留给世人的印象。", 
	
	"擅长武器：机枪", 
	
	"职业特点： 远程范围物理伤害" 
	}, -- 毁灭者


	[CLIENT_CAREER_CHURCH_SINGLEHAND] = 
	{ "圣佑者", 
	
	"    他们是团队的精神领袖，是天使的化身。他们拥有大量的恢复技能，以超强的治疗能力保护身边的队友，并且给予他们强大的祝福辅助，只要有他们在，队友们是战场上永远的胜利者。", 
	
	"擅长武器：圣杖、圣器", 
	
	"职业特点：群体治疗技能、辅助技能" 
	},  -- 圣佑者


	[CLIENT_CAREER_CHURCH_DOUBLEHAND] = 
	{ "驭雷师", 
	
	"    他们拥有不俗的魔法伤害，他们是掌控雷电的大师，是严谨的仲裁者。给予敌人有效的打击。除此他们还是团队中必不可少的协助者，利用各种诅咒魔法有力地削弱折磨敌人。", 
	
	"擅长武器：圣杖、圣器", 
	
	"职业特点：远程魔法伤害、诅咒技能" 
	},  -- 驭雷师
};

-- 属性按钮
function ChangeCareerRaceAttrButton_OnEnter()
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

function ChangeCareerFrame_OnLoad()
	for event, _ in pairs( t_ChangeCareerFrameEvents ) do
		this:RegisterEvent( event );
	end

	t_ChangeCareerFrameEvents.__index = function ( tab, key )
									tab[key] = { func = function () end };
									return tab[key];
								end
	setmetatable( t_ChangeCareerFrameEvents, t_ChangeCareerFrameEvents );
end

function ChangeCareerFrame_OnEvent()
	t_ChangeCareerFrameEvents[arg1].func();
end

function ChangeCareerFrame_Test_OnShow()
	local nRace = CLIENT_CAREER_CHURCH_DOUBLEHAND;
	local rich = getglobal( this:GetName().."CareerDesRich" );
	--rich:SetText( "#P"..t_racedesc[nRace][1], 255, 254, 189);
	rich:SetText( t_extendedRacedesc[nRace][2], 255, 254, 189 );
	rich:AddText( "                   ",255,255,255);
	rich:AddText( t_extendedRacedesc[nRace][3], 0, 255, 255 );
	rich:AddText( "                   ",255,255,255);
	rich:AddText( t_extendedRacedesc[nRace][4], 255, 120, 0 );

	UpdateChangeCareerRoleInfo( { race = nRace, gender = CLIENT_GENDER_MALE } );
end

-- 时装
local t_fashionid =	
		
{	[CLIENT_CAREER_WARRIOR]	= 3010000,	[CLIENT_CAREER_WARRIOR_SINGLEHAND] = 2011603,	[CLIENT_CAREER_WARRIOR_DOUBLEHAND] = 2011613,
	--								聚能者									炼魂者
	[CLIENT_CAREER_MAGICOR]	= 3020000,	[CLIENT_CAREER_MAGICOR_SINGLEHAND] = 2021603,	[CLIENT_CAREER_MAGICOR_DOUBLEHAND] = 2021613,
	--								猎杀者									毁灭者
	[CLIENT_CAREER_GUNER]		= 3030000,	[CLIENT_CAREER_GUNER_SINGLEHAND]	= 2031603,	[CLIENT_CAREER_GUNER_DOUBLEHAND]	= 2031613,
	--								圣佑者									驭雷师
	[CLIENT_CAREER_CHURCH]		= 3040000,	[CLIENT_CAREER_CHURCH_SINGLEHAND]	= 2041603,	[CLIENT_CAREER_CHURCH_DOUBLEHAND]	= 2041613,
};

-- 衣服
local t_clothesID = 
{	--	守护者								征服者
	[CLIENT_CAREER_WARRIOR_SINGLEHAND] = 2011603,	[CLIENT_CAREER_WARRIOR_DOUBLEHAND] = 2011613,
	--	聚能者								炼魂者
	[CLIENT_CAREER_MAGICOR_SINGLEHAND] = 2021603,	[CLIENT_CAREER_MAGICOR_DOUBLEHAND] = 2021613,
	--	猎杀者								毁灭者
	[CLIENT_CAREER_GUNER_SINGLEHAND]	= 2031603,	[CLIENT_CAREER_GUNER_DOUBLEHAND]	= 2031613,
	--	圣佑者								驭雷师
	[CLIENT_CAREER_CHURCH_SINGLEHAND]	= 2041603,	[CLIENT_CAREER_CHURCH_DOUBLEHAND]	= 2041613,
};

-- 左手
local t_leftHandId	= 
{	--	守护者								征服者
	[CLIENT_CAREER_WARRIOR_SINGLEHAND] = 1061603,	[CLIENT_CAREER_WARRIOR_DOUBLEHAND] = 1041603,
	--	聚能者								炼魂者
	[CLIENT_CAREER_MAGICOR_SINGLEHAND] = 1181603,	[CLIENT_CAREER_MAGICOR_DOUBLEHAND] = 1181613,
	--	猎杀者								毁灭者
	[CLIENT_CAREER_GUNER_SINGLEHAND]	= 1281603,	[CLIENT_CAREER_GUNER_DOUBLEHAND]	= 0,
	--	圣佑者								驭雷师
	[CLIENT_CAREER_CHURCH_SINGLEHAND]	= 1581603,	[CLIENT_CAREER_CHURCH_DOUBLEHAND]	= 1581613,
};

-- 右手
local t_rightHandid	= 
{	--	守护者								征服者
	[CLIENT_CAREER_WARRIOR_SINGLEHAND] = 1011603,	[CLIENT_CAREER_WARRIOR_DOUBLEHAND] = 1031603,
	--	聚能者								炼魂者
	[CLIENT_CAREER_MAGICOR_SINGLEHAND] = 1101603,	[CLIENT_CAREER_MAGICOR_DOUBLEHAND] = 1111603,
	--	猎杀者								毁灭者
	[CLIENT_CAREER_GUNER_SINGLEHAND]	= 1201603,	[CLIENT_CAREER_GUNER_DOUBLEHAND]	= 1251603,
	--	圣佑者								驭雷师
	[CLIENT_CAREER_CHURCH_SINGLEHAND]	= 1511603,	[CLIENT_CAREER_CHURCH_DOUBLEHAND]	= 1501603,
};

-- 头盔
local t_headId	= 
{	--	守护者								征服者
	[CLIENT_CAREER_WARRIOR_SINGLEHAND] = 2111603,	[CLIENT_CAREER_WARRIOR_DOUBLEHAND] = 2111613,
	--	聚能者								炼魂者
	[CLIENT_CAREER_MAGICOR_SINGLEHAND] = 2121603,	[CLIENT_CAREER_MAGICOR_DOUBLEHAND] = 2121613,
	--	猎杀者								毁灭者
	[CLIENT_CAREER_GUNER_SINGLEHAND]	= 2131603,	[CLIENT_CAREER_GUNER_DOUBLEHAND]	= 2131613,
	--	圣佑者								驭雷师
	[CLIENT_CAREER_CHURCH_SINGLEHAND]	= 2141603,	[CLIENT_CAREER_CHURCH_DOUBLEHAND]	= 2141613,
};

-- 鞋子
local t_shoesId	= 
{	--	守护者								征服者
	[CLIENT_CAREER_WARRIOR_SINGLEHAND] = 2611603,	[CLIENT_CAREER_WARRIOR_DOUBLEHAND] = 2611613,
	--	聚能者								炼魂者
	[CLIENT_CAREER_MAGICOR_SINGLEHAND] = 2621603,	[CLIENT_CAREER_MAGICOR_DOUBLEHAND] = 2621613,
	--	猎杀者								毁灭者
	[CLIENT_CAREER_GUNER_SINGLEHAND]	= 2631603,	[CLIENT_CAREER_GUNER_DOUBLEHAND]	= 2631613,
	--	圣佑者								驭雷师
	[CLIENT_CAREER_CHURCH_SINGLEHAND]	= 2641603,	[CLIENT_CAREER_CHURCH_DOUBLEHAND]	= 2641613,
};

-- 肩膀
local t_shoulderId	= 
{	--	守护者								征服者
	[CLIENT_CAREER_WARRIOR_SINGLEHAND] = 2211603,	[CLIENT_CAREER_WARRIOR_DOUBLEHAND] = 2211613,
	--	聚能者								炼魂者
	[CLIENT_CAREER_MAGICOR_SINGLEHAND] = 2221603,	[CLIENT_CAREER_MAGICOR_DOUBLEHAND] = 2221613,
	--	猎杀者								毁灭者
	[CLIENT_CAREER_GUNER_SINGLEHAND]	= 1271603,	[CLIENT_CAREER_GUNER_DOUBLEHAND]	= 1271613,
	--	圣佑者								驭雷师
	[CLIENT_CAREER_CHURCH_SINGLEHAND]	= 2241603,	[CLIENT_CAREER_CHURCH_DOUBLEHAND]	= 2241613,
};

local t_modelviewConfig = 
{
[POS_ID_CLOSES]		= t_clothesID,
[POS_ID_LEFTHAND]	= t_leftHandId,
[POS_ID_RIGHTHAND]	= t_rightHandid,
[POS_ID_HAT]		= t_headId,
--  鞋子没做[POS_ID_HAT]		= t_shoesId,
[POS_ID_SHOULD]		= t_shoulderId,
};

function UpdateChangeCareerRoleAvatar( data )	
	--[[
	职业名称	职业ID		主手		副手		衣服		头盔		靴子		肩膀
	守护者		17			1011603		1061603		2011603		2111603		2611603		2211603
	征服者		33			1031603					2011613		2111613		2611613		2211613

	聚能者		18			1101603		1181603		2021603		2121603		2621603		2221603
	炼魂师		34			1111603		1181613		2021613		2121613		2621613		2221613

	猎杀者		19			1201603		1281603		2031603		2131603		2631603		1271603
	毁灭者		35			1251603					2031613		2131613		2631613		1271613

	圣佑者		20			1511603		1581603		2041603		2141603		2641603		2241603
	驭雷师		36			1501603		1581613		2041613		2141613		2641613		2241613
	--]]

	-- {1943, 83, 3159}  -- 主角
	-- local pos			= NudeAvatarPos[11];
	local pos			= {5348, 246, 3648}  -- 主角
	local modelView		= getglobal( "ChangeCareerFrameRoleModelBtnModelView" );
	
	local nRace			= data["race"];
	local nGender		= data["gender"];
	if not modelView:isHaveCameraAnim() then
		modelView:setCameraAnim("scene\\model\\DL\\dl_cam.omod");
	end
	
	modelView:setAvatarAttrib(0, 0, 0, 0, true );
	modelView:setEntityAvatar(0);
	modelView:setAvatarAttrib(0, 0, nRace, 0, true );	-- grid为0表示职业
	modelView:setAvatarAttrib(0, 1, nGender - 1 );	-- grid为0表示性别

	for posType, data in pairs( t_modelviewConfig ) do
		local oneRaceConfigID = data[nRace];
		if oneRaceConfigID ~= 0 then
			modelView:setAvatarAttrib(0, posType, oneRaceConfigID );
		end
	end

	local t_AvartarControl	= GetAvartarControl();
	local nAnimID = t_AvartarControl:getAvatarAnim( {	["race"] = GetInitRace( nRace ), ["leftHandItem"] = t_modelviewConfig[POS_ID_LEFTHAND][nRace], 
														["rightHandItem"] = t_modelviewConfig[POS_ID_RIGHTHAND][nRace] } );
	modelView:playEntityAnim( 0, nAnimID, nil );

	--modelView:playEntityAnim(0, GetPoseState(t_leftHandId[nRace]), nil ); -- 各职业的基本备战动作 后三位108
	modelView:playCameraAnim(99102, 1000);
	modelView:setEntityRotate(0,t_changeCareerControl["rotateAngle"] );
	modelView:setPetScale(0, 1.2, 1.2, 1.2);
	modelView:setEntityPosition( 0, pos[1], pos[2], pos[3] );
	t_changeCareerControl:setFreeStartTime( { Time = os.clock() } );
end

function UpdateChangeCareerRoleInfo( data )
	UpdateChangeCareerRoleAvatar( data );


	local nRace = data["race"];
	local rich = getglobal( "ChangeCareerFrameCareerDesRich" );
	--rich:SetText( "#P"..t_racedesc[nRace][1], 255, 254, 189);
	rich:SetText( t_extendedRacedesc[nRace][2], 255, 254, 189 );
	rich:AddText( "                   ",255,255,255);
	rich:AddText( t_extendedRacedesc[nRace][3], 0, 255, 255 );
	rich:AddText( "                   ",255,255,255);
	rich:AddText( t_extendedRacedesc[nRace][4], 255, 120, 0 );
	
	local t_fonts = {	["ChangeCareerFrameCareerDescHeadBtnFont"]		= { value = t_extendedRacedesc[nRace][1] },
					};
	util.SetFonts( t_fonts );


	local t_RaceAttr =	
	{
		-- 战士
		[CLIENT_CAREER_WARRIOR] =	
		{ 
			maxattr = 8,
			maxuv = { u = 66, v = 254, w = 93, h = 9 },
			normaluv = { u = 66, v = 245, w = 93, h = 9 },
			[1] = { attr = "力量",  value = 6, maxvalue = 8 }, 
			[2] = { attr = "智力",  value = 1, maxvalue = 8 },
			[3] = { attr = "体质",  value = 5, maxvalue = 8 },
			[4] = { attr = "精神",  value = 3, maxvalue = 8 },
			[5] = { attr = "敏捷",  value = 4, maxvalue = 8 },
			[6] = { attr = "感知",  value = 6, maxvalue = 8 },
		},

		-- 守护者
		[CLIENT_CAREER_WARRIOR_SINGLEHAND] =	
		{ 
			maxattr = 8,
			maxuv = { u = 66, v = 254, w = 93, h = 9 },
			normaluv = { u = 66, v = 245, w = 93, h = 9 },
			[1] = { attr = "力量",  value = 6, maxvalue = 8 }, 
			[2] = { attr = "智力",  value = 1, maxvalue = 8 },
			[3] = { attr = "体质",  value = 3, maxvalue = 8 },
			[4] = { attr = "精神",  value = 3, maxvalue = 8 },
			[5] = { attr = "敏捷",  value = 4, maxvalue = 8 },
			[6] = { attr = "感知",  value = 4, maxvalue = 8 },
		},

		-- 征服者
		[CLIENT_CAREER_WARRIOR_DOUBLEHAND] =	
		{ 
			maxattr = 8,
			maxuv = { u = 66, v = 254, w = 93, h = 9 },
			normaluv = { u = 66, v = 245, w = 93, h = 9 },
			[1] = { attr = "力量",  value = 8, maxvalue = 8 }, 
			[2] = { attr = "智力",  value = 1, maxvalue = 8 },
			[3] = { attr = "体质",  value = 2, maxvalue = 8 },
			[4] = { attr = "精神",  value = 3, maxvalue = 8 },
			[5] = { attr = "敏捷",  value = 3, maxvalue = 8 },
			[6] = { attr = "感知",  value = 4, maxvalue = 8 },
		},
		
		-- 法师
		[CLIENT_CAREER_MAGICOR] =	
		{ 
			maxattr = 8;
			maxuv = { u = 66, v = 254, w = 93, h = 9 },
			normaluv = { u = 66, v = 245, w = 93, h = 9 },
			[1] = { attr = "智力",  value = 8, maxvalue = 10 }, 
			[2] = { attr = "体质",  value = 5, maxvalue = 10 },
			[3] = { attr = "精神",  value = 6, maxvalue = 10 },
			[4] = { attr = "敏捷",  value = 5, maxvalue = 10 },
			[5] = { attr = "闪躲",  value = 8, maxvalue = 10 },
			[6] = { attr = "精准",  value = 8, maxvalue = 10 },
		},
		
		-- 聚能者
		[CLIENT_CAREER_MAGICOR_SINGLEHAND] =	
		{ 
			maxattr = 8;
			maxuv = { u = 66, v = 254, w = 93, h = 9 },
			normaluv = { u = 66, v = 245, w = 93, h = 9 },
			[1] = { attr = "力量",  value = 1, maxvalue = 8 }, 
			[2] = { attr = "智力",  value = 6, maxvalue = 8 },
			[3] = { attr = "体质",  value = 2, maxvalue = 8 },
			[4] = { attr = "精神",  value = 3, maxvalue = 8 },
			[5] = { attr = "敏捷",  value = 3, maxvalue = 8 },
			[6] = { attr = "感知",  value = 4, maxvalue = 8 },
		},
		
		-- 炼魂师
		[CLIENT_CAREER_MAGICOR_DOUBLEHAND] =	
		{ 
			maxattr = 8;
			maxuv = { u = 66, v = 254, w = 93, h = 9 },
			normaluv = { u = 66, v = 245, w = 93, h = 9 },
			[1] = { attr = "力量",  value = 1, maxvalue = 8 }, 
			[2] = { attr = "智力",  value = 7, maxvalue = 8 },
			[3] = { attr = "体质",  value = 2, maxvalue = 8 },
			[4] = { attr = "精神",  value = 3, maxvalue = 8 },
			[5] = { attr = "敏捷",  value = 1, maxvalue = 8 },
			[6] = { attr = "感知",  value = 4, maxvalue = 8 },
		},
		
		-- 枪手
		[CLIENT_CAREER_GUNER] =	
		{ 
			maxattr = 8;
			maxuv = { u = 66, v = 254, w = 93, h = 9 },
			normaluv = { u = 66, v = 245, w = 93, h = 9 },
			[1] = { attr = "力量",  value = 6, maxvalue = 10 }, 
			[2] = { attr = "体质",  value = 6, maxvalue = 10 },
			[3] = { attr = "精神",  value = 5, maxvalue = 10 },
			[4] = { attr = "敏捷",  value = 6, maxvalue = 10 },
			[5] = { attr = "闪躲",  value = 8, maxvalue = 10 },
			[6] = { attr = "精准",  value = 8, maxvalue = 10 },
		},
		
		-- 猎杀者
		[CLIENT_CAREER_GUNER_SINGLEHAND] =	
		{ 
			maxattr = 8;
			maxuv = { u = 66, v = 254, w = 93, h = 9 },
			normaluv = { u = 66, v = 245, w = 93, h = 9 },
			[1] = { attr = "力量",  value = 8, maxvalue = 8 }, 
			[2] = { attr = "智力",  value = 2, maxvalue = 8 },
			[3] = { attr = "体质",  value = 2, maxvalue = 8 },
			[4] = { attr = "精神",  value = 3, maxvalue = 8 },
			[5] = { attr = "敏捷",  value = 1, maxvalue = 8 },
			[6] = { attr = "感知",  value = 4, maxvalue = 8 },
		},
		
		-- 毁灭者
		[CLIENT_CAREER_GUNER_DOUBLEHAND] =	
		{ 
			maxattr = 8;
			maxuv = { u = 66, v = 254, w = 93, h = 9 },
			normaluv = { u = 66, v = 245, w = 93, h = 9 },
			[1] = { attr = "力量",  value = 6, maxvalue = 8 }, 
			[2] = { attr = "智力",  value = 2, maxvalue = 8 },
			[3] = { attr = "体质",  value = 2, maxvalue = 8 },
			[4] = { attr = "精神",  value = 3, maxvalue = 8 },
			[5] = { attr = "敏捷",  value = 3, maxvalue = 8 },
			[6] = { attr = "感知",  value = 4, maxvalue = 8 },
		},
		
		-- 祭司
		[CLIENT_CAREER_CHURCH] =	
		{ 
			maxattr = 9;
			maxuv = { u = 66, v = 254, w = 93, h = 9 },
			normaluv = { u = 66, v = 245, w = 93, h = 9 },
			[1] = { attr = "智力",  value = 5, maxvalue = 10 }, 
			[2] = { attr = "体质",  value = 6, maxvalue = 10 },
			[3] = { attr = "精神",  value = 8, maxvalue = 10 },
			[4] = { attr = "敏捷",  value = 6, maxvalue = 10 },
			[5] = { attr = "闪躲",  value = 8, maxvalue = 10 },
			[6] = { attr = "精准",  value = 8, maxvalue = 10 },
		},
		
		-- 圣佑者
		[CLIENT_CAREER_CHURCH_SINGLEHAND] =	
		{ 
			maxattr = 9;
			maxuv		= { u = 66, v = 254, w = 93, h = 9 },
			normaluv	= { u = 66, v = 245, w = 93, h = 9 },
			[1] = { attr = "力量",  value = 1, maxvalue = 8 }, 
			[2] = { attr = "智力",  value = 4, maxvalue = 8 },
			[3] = { attr = "体质",  value = 2, maxvalue = 8 },
			[4] = { attr = "精神",  value = 3, maxvalue = 8 },
			[5] = { attr = "敏捷",  value = 2, maxvalue = 8 },
			[6] = { attr = "感知",  value = 4, maxvalue = 8 },
		},
		
		-- 驭雷师
		[CLIENT_CAREER_CHURCH_DOUBLEHAND] =	
		{ 
			maxattr = 9;
			maxuv = { u = 66, v = 254, w = 93, h = 9 },
			normaluv = { u = 66, v = 245, w = 93, h = 9 },
			[1] = { attr = "力量",  value = 1, maxvalue = 8 }, 
			[2] = { attr = "智力",  value = 6, maxvalue = 8 },
			[3] = { attr = "体质",  value = 2, maxvalue = 8 },
			[4] = { attr = "精神",  value = 3, maxvalue = 8 },
			[5] = { attr = "敏捷",  value = 1, maxvalue = 8 },
			[6] = { attr = "感知",  value = 4, maxvalue = 8 },
		},
	};

	for i = 1, MAX_ATTR_BTN do
		local btn		= getglobal("ChangeCareerFrameRaceAttrButton"..i);
		local font		= getglobal( btn:GetName().."NameFont" );
		local valuefont = getglobal( btn:GetName().."ValueFont" );
		local valuetex	= getglobal( btn:GetName().."valueTex" );
		btn:SetClientString( t_RaceAttr[nRace][i].attr );
		local t_uv = { u = 575 + 165, v = 979, w = 165, h = 9 };
		valuetex:SetTexUV( t_uv.u - t_uv.w * t_RaceAttr[nRace][i].value / t_RaceAttr[nRace][i].maxvalue, t_uv.v, t_uv.w, t_uv.h );
		font:SetText( t_RaceAttr[nRace][i].attr );
		-- TODO: 先不显示等策划定出来
		--valuefont:SetText( t_RaceAttr[nRace][i].value );	
		valuefont:SetText( "" );		
	end	
end

function UpdateCareerBtn( )
	local t_selectCareers = t_changeCareerControl:getCanSelectCareers();
	for index, career in ipairs( t_selectCareers ) do
		local btn = getglobal( "ChangeCareerFrameCareer"..index.."Btn" );
		
		local t_normalUV = t_allCareerDesc[career]["normalUV"];
		btn:ChangeNormalTexture( t_normalUV.x, t_normalUV.y, t_normalUV.width, t_normalUV.height );
		
		local t_highlightUV = t_allCareerDesc[career]["highlightUV"];
		btn:ChangeHighlightTexture( t_highlightUV.x, t_highlightUV.y, t_highlightUV.width, t_highlightUV.height );
		
		
		local t_pushedUV = t_allCareerDesc[career]["pushedUV"];
		btn:ChangePushedTexture( t_pushedUV.x, t_pushedUV.y, t_pushedUV.width, t_pushedUV.height );
		
		local t_size = t_allCareerDesc[career]["size"];
		btn:SetSize( t_size["x"], t_size["y"] );

		btn:SetClientUserData( 0, career );
	end
end

function GetInitRace( nRace )
	return t_changeCareerControl:getInitRace( { race = nRace } );
end

-- 转职主面板
function ChangeCareerFrame_OnShow()
	--this:SetPoint( "center", "UIClient", "center", 0, 0 );
	
	local tex	= getglobal( this:GetName().."BackTex" );
	tex:SetColor( 0, 0, 0 );
	tex:SetBlendAlpha( 0.8 );

	local mainplayer	= ActorMgr:getMainPlayer();
	local nInitCareer	= bitAndOperation( mainplayer:getRace(), 0x0f );
	t_changeCareerControl:updateCanSelectCareers( { initCareer = nInitCareer } );
		
	local t_btns = { this:GetName().."Career1Btn", this:GetName().."Career2Btn", };
	util.DischeckBtns( t_btns );
	t_btns = { this:GetName().."Career1Btn", };
	util.CheckBtns( t_btns );
	
	UpdateCareerBtn();
	local career1Btn = getglobal( this:GetName().."Career1Btn" );
	local nDefaultSelectCareer = career1Btn:GetClientUserData( 0 );
	t_changeCareerControl:setSelectCareer( { career = nDefaultSelectCareer } );

	UpdateChangeCareerRoleInfo( { race = nDefaultSelectCareer, gender = mainplayer:getGender() } );
end

function ChangeCareerFrame_OnHide()
	local t_hideIcons = { "ConfirmChangeCareerFrame", "ForbitChgSelectCareerFrame" };
	util.HideIcons( t_hideIcons );
	ClearItemLock( "ChangeCareerFrame" );
	t_changeCareerControl:init();
end

-- 点击职业按钮
function ChangeCareerFrameCareerBtn_OnClick()
	local nSelectCareer = this:GetClientUserData( 0 );
	if nSelectCareer == t_changeCareerControl:getSelectCareer() then
		return;
	end
	
	--[[
	if ConfirmChangeCareerFrame:IsShown() then
		if not ForbitChgSelectCareerFrame:IsShown() then
			ForbitChgSelectCareerFrame:Show();
		end
		return;
	end
	--]]

	local t_btns = { this:GetParent().."Career1Btn", this:GetParent().."Career2Btn", };
	util.DischeckBtns( t_btns );
	t_btns = { this:GetName(), };
	util.CheckBtns( t_btns );

	t_changeCareerControl:setSelectCareer( { career = nSelectCareer } );

	local mainplayer = ActorMgr:getMainPlayer();
	UpdateChangeCareerRoleInfo( { race = t_changeCareerControl:getSelectCareer(), gender = mainplayer:getGender() } );

	if ConfirmChangeCareerFrame:IsShown() then
		UpdateConfirmChangeCareerFrame();
	end
end

-- 更新模型
function ChangeCareerFrame_OnUpdate()
	local fCurTime = os.clock();
	if fCurTime - t_changeCareerControl:getFreeStartTime() <= 10 then
		return;	
	end

	t_changeCareerControl:setFreeStartTime( { Time = fCurTime } );
	local modelView		= getglobal( "ChangeCareerFrameRoleModelBtnModelView" );
	local leftarmid		= modelView:getAvatarAttrib( 0, POS_ID_LEFTHAND );
	local rightarmid	= modelView:getAvatarAttrib( 0, POS_ID_RIGHTHAND );

	if leftarmid ~= 0 then
		if GetFreePosState( leftarmid ) > 0 then
			modelView:playEntityAnim(0, GetFreePosState( leftarmid ), nil );
		end
	elseif rightarmid ~= 0 then
		if GetFreePosState( rightarmid ) > 0 then
			modelView:playEntityAnim(0, GetFreePosState( rightarmid ), nil );
		end
	else
		modelView:playEntityAnim(0, GetFreePosState( 0 ), nil );
	end		
end

-- 向左旋转
function ChangeCareerFrame_TurnLeftBtn_OnMouseDown()
	local modelView	= getglobal( "ChangeCareerFrameRoleModelBtnModelView" );
	t_changeCareerControl["rotateAngle"] = t_changeCareerControl["rotateAngle"] - 5;
	modelView:setEntityRotate( 0, t_changeCareerControl["rotateAngle"] );
end

-- 向右旋转
function ChangeCareerFrame_TurnRightBtn_OnMouseDown()
	local modelView	= getglobal( "ChangeCareerFrameRoleModelBtnModelView" );
	t_changeCareerControl["rotateAngle"] = t_changeCareerControl["rotateAngle"] + 5;
	modelView:setEntityRotate( 0, t_changeCareerControl["rotateAngle"] );
end

-- 转职按钮
function ChangeCareerFrame_ChangeCareerBtn_OnClick()
	if ConfirmChangeCareerFrame:IsShown() then
		return;
	end

	ConfirmChangeCareerFrame:Show();
end

-- 转职确认面板
function UpdateConfirmChangeCareerFrame()
	local t_riches =	 { ["ConfirmChangeCareerFrameRich"] = 
							{ 
								value = "#P确认转职为"..t_allCareerDesc[t_changeCareerControl:getSelectCareer()]["desc"].."吗？#n", 
								color = { r = 255, g = 254, b = 189 } 
							},
						};
	util.SetRiches( t_riches );
end

function ConfirmChangeCareerFrame_OnShow()
	UpdateConfirmChangeCareerFrame();
	this:SetPoint( "center", "UIClient", "center", 0, 0 );
end

-- 确认转职按钮
function ConfirmChangeCareerFrame_AcceptBtnOnClick()
	local mainplayer	= ActorMgr:getMainPlayer();
	local container = mainplayer:getContainer()
	local item = container:getItem(	t_changeCareerControl["listType"],t_changeCareerControl["gridx"] )
	if CheckItemResult( item:getItemDef(), RESULT_ITEM_BISAI ) then
		mainplayer:ChangeCareerToFightLevle( t_changeCareerControl:getSelectCareer(), t_changeCareerControl["gridx"] )
	else
		mainplayer:changeCareer( t_changeCareerControl:getSelectCareer() );
	end
	local t_hideIcons = { "ConfirmChangeCareerFrame", };
	util.HideIcons( t_hideIcons )
end

function ForbitChgSelectCareerFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 0, -150 );
end

function ConfirmChangeCareerFrameRich_OnClick()
	if arg2 ~= "" and arg3 == "LeftButton" then
		GameMgr:openWebSite( arg2 )
	end
end