-- 5030021
CAREER_WARRIOR_SINGLEHAND	= 17; 	-- /* ����֧���ܵ���������Ϊ���ֵ� */
CAREER_WARRIOR_DOUBLEHAND	= 33; 	-- /* ����֧���ܵ���������Ϊ���ֽ� */

CAREER_MAGICOR_SINGLEHAND	= 18; 	-- /* ����֧���ܵ���������Ϊ����ħ�� */
CAREER_MAGICOR_DOUBLEHAND	= 34; 	-- /* ���֧���ܵ���������Ϊ˫��ħ�� */

CAREER_GUNER_SINGLEHAND		= 19; 	-- /* ��ǹ�ּ���֧����������Ϊ����ǹ */
CAREER_GUNER_DOUBLEHAND		= 35; 	-- /* ��ǹ�ּ���֧����������Ϊ��ǹ */

CAREER_CHURCH_SINGLEHAND	= 20; 	-- /* ʥ��֧���ܵ���������Ϊ����ʥ�� */
CAREER_CHURCH_DOUBLEHAND	= 36; 	-- /* �׷�֧���ܵ���������Ϊ˫��ʥ�� */
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
[CLIENT_CAREER_WARRIOR]			= { desc = "սʿ",  },		
[CLIENT_CAREER_MAGICOR]			= { desc = "��ʦ" },
[CLIENT_CAREER_GUNER]				= { desc = "ǹ��" },		
[CLIENT_CAREER_CHURCH]				= { desc = "��˾" },

[CLIENT_CAREER_WARRIOR_SINGLEHAND]	= { desc = "�ػ���",	normalUV	= { x = 369, y = 793, width = 122, height = 42 }, 
													highlightUV = { x = 492, y = 793, width = 122, height = 42 }, 
													pushedUV	= { x = 615, y = 793, width = 122, height = 42 },
													size		= { x = 122, y = 42 },
								},		

[CLIENT_CAREER_WARRIOR_DOUBLEHAND]	= { desc = "������",	normalUV	= { x = 0, y = 793, width = 122, height = 42 }, 
													highlightUV = { x = 123, y = 793, width = 122, height = 42 }, 
													pushedUV	= { x = 246, y = 793, width = 122, height = 42 },
													size		= { x = 122, y = 42 },
								},	

[CLIENT_CAREER_MAGICOR_SINGLEHAND]	= { desc = "������",	normalUV	= { x = 369, y = 922, width = 122, height = 42 }, 
													highlightUV = { x = 492, y = 922, width = 122, height = 42 }, 
													pushedUV	= { x = 615, y = 922, width = 122, height = 42 },
													size		= { x = 122, y = 42 },
								},	

[CLIENT_CAREER_MAGICOR_DOUBLEHAND]	= { desc = "����ʦ",	normalUV	= { x = 0, y = 922, width = 122, height = 42 }, 
													highlightUV = { x = 123, y = 922, width = 122, height = 42 }, 
													pushedUV	= { x = 246, y = 922, width = 122, height = 42 },
													size		= { x = 122, y = 42 },
								},

[CLIENT_CAREER_GUNER_SINGLEHAND]	= { desc = "��ɱ��",		normalUV	= { x = 0, y = 879, width = 122, height = 42 }, 
													highlightUV = { x = 123, y = 879, width = 122, height = 42 },
													pushedUV	= { x = 246, y = 879, width = 122, height = 42 },
													size		= { x = 122, y = 42 },
								},
								
[CLIENT_CAREER_GUNER_DOUBLEHAND]	= { desc = "������",		normalUV	= { x = 369, y = 879, width = 122, height = 42 }, 
													highlightUV = { x = 492, y = 879, width = 122, height = 42 },
													pushedUV	= { x = 615, y = 879, width = 122, height = 42 },
													size		= { x = 122, y = 42 },
								},

[CLIENT_CAREER_CHURCH_SINGLEHAND]	= { desc = "ʥ����",	normalUV	= { x = 369, y = 836, width = 122, height = 42 }, 
													highlightUV = { x = 492, y = 836, width = 122, height = 42 },
													pushedUV	= { x = 615, y = 836, width = 122, height = 42 },
													size		= { x = 122, y = 42 },
								},
								
[CLIENT_CAREER_CHURCH_DOUBLEHAND]	= { desc = "Ԧ��ʦ",	normalUV	= { x = 0, y = 836, width = 122, height = 42 }, 
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

	--�����ְҵҪ����϶�����
	if nCompareRace == 0 then
		return true;
	end
	-- ����Ʒ��תְ��ģ�������תְǰ�ģ��ǿ϶�������
	if nCompareRace > ZHUAN_ZHI_RACE_LIMIT and nActorRace <= ZHUAN_ZHI_RACE_LIMIT then
		return bitAndOperation( nCompareRace, 0x0f ) == nActorRace;
	end
	
	-- ������תְ������
	if nCompareRace > ZHUAN_ZHI_RACE_LIMIT and nActorRace > ZHUAN_ZHI_RACE_LIMIT then
		return nCompareRace == nActorRace;
	end
	
	-- ������ûתְ�����
	if nCompareRace <= ZHUAN_ZHI_RACE_LIMIT and nActorRace <= ZHUAN_ZHI_RACE_LIMIT then
		return nCompareRace == nActorRace;
	end
	
	-- ����Ʒûתְ������תְ
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

-- TODO: ��Э����º���ȥ��
t_changeCareerControl["canStartChangeCareer"] = 
function ( self, data )
	local mainplayer	= ActorMgr:getMainPlayer();
	local nCurLv		= mainplayer:getLv();
	
	-- �Եȼ���ӡ�ж�
	local fengyinlv,fengyinEnd = GetFengYinData()
	if fengyinlv > 0 and fengyinlv < CLIENT_CAREERCHG_LEVEL + 1 then
		local dt = math.floor( (fengyinEnd - getServerTime().sec)/3600 )
		local szTime = ""
		if dt < 1 then
			szTime = math.ceil((fengyinEnd - getServerTime().sec)/60) .. "����"
		else
			szTime = dt .. "Сʱ"
		end
		ShowMidTips( "�ȼ���ӡ��,��ʱ����תְ�������⻹��" .. szTime )
		return
	end
	if mainplayer:getRace() > 0x10 then
		ShowMidTips( "��תְ����������תְ" );
		return false;
	end

	if nCurLv < CLIENT_CAREERCHG_LEVEL then
		ShowMidTips( "���ȴﵽ"..CLIENT_CAREERCHG_LEVEL.."���ٽ���תְ" );
		return false;
	end

	if mainplayer:getExp() < mainplayer:getLvExp( nCurLv ) then
		ShowMidTips( "�㵱ǰ�������鲻��" );
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
	-- �����Ʒ��ӳɹ�����ס����Ʒ
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
	{ "սʿ", 

	"    ���������������ߣ�ƾ�������벻������־֧�ŵ�ս�������һ�̡�\n    ��Ϊ��ͳս�����ɵļ̳��ߣ�սʿ����ĶԴ���ÿһ��ս����\n    �����¸��޾壬���������۳�������\n    �����޼᲻�ݣ����������������ڡ�\n    սʿ�߹���·û�������Ľ�ӡ����Ϊ����Ǳ���Ҫȥ������ս�ѡ�", 
		
	"�������ͣ������Խ�����\n��תְΪ���ػ��ߡ�������\nְҵ��񣺽�ս����������޿�������" 
	}, -- սʿ


	[CLIENT_CAREER_MAGICOR] = 
	{ "��ʦ", 

	"    �����ǿֲ����ƻ��ߣ��߶���Ԫ��ħ������һ�С�\n    ��Ϊ���Ƴ����ս���߼�����ʦ�ı�������¾ɵ���֪��\n    ������Ԫ�ص��ƿ��ߣ���̼����ۻ���\n    �����Ǵϻ۵Ĳ߻��ߣ���ָ��Ťת���ơ�\n    �������ǻ�����һ�У���ħ���ݻ�һ�У�����û�����������ǵ�������", 
		
	"�������ͣ�ħ�ȡ�����\n��תְΪ������ʦ��������\nְҵ���Զ��ħ���˺���Ⱥ���������" 
	}, -- ��ʦ


	[CLIENT_CAREER_GUNER] = 
	{ "ǹ��", 
	
	"    ���������ľ�ɱ�ߣ�ÿһ�ο۶�������Ǿ��ܵĲ���\n    ��Ϊ��Ҫ��ǣ���ߣ�ǹ�ּ汸�侲��˼������ϵ��жϣ��ѵ���ѹ����Զ����\n    �����������ľѻ��֣������鷢��\n    �����������ɱ¾�֣����Ȳ��\n    ���ǰѵ�����Ū�ڹ���֮�У�����һ˿��Ϣ��Ȼ��������ơ�", 
		
	"�������ͣ�����ǹ������ǹ����ǹ\n��תְΪ����ɱ�ߡ�������\nְҵ���Զ�������˺��������������" 
	}, -- ǹ��


	[CLIENT_CAREER_CHURCH] = 
	{ "��˾", 
	
	"    ��������Ĵ����ߣ��᳹��ߵ�������̻���ս���ϱ��������ǵ�ͬ�顣\n    ��Ϊ��������࣬��˾ֻ��һ����ͷ�����������������һ�룬��ֹ�κ�������\n    �����糿�ص�������Ϊ���������⡣\n    �������׵�����У�Ϊ���˴���ج�ġ�\n    ���ż�˾���������߶�Զ��ͬ������߶�Զ��", 
		
	"�������ͣ�ʥ�ȡ�ʥ��\n��תְΪ��ʥ���ߡ�Ԧ��ʦ\nְҵ���Զ��ħ���˺�����������" 
	},  -- ��˾


	[CLIENT_CAREER_WARRIOR_SINGLEHAND] = 
	{ "�ػ���", 

	"    ������ð��ʱ���쵼�ߣ�ս��ʱ���ػ��ߡ�����ӵ����ߵķ������������ǿ�ı��ݣ����ڶ�ʱ���ڴ�������Լ��ķ������������÷�Χ�������䳬ǿ�ĵֿ��˺������������Ķ��ѡ�", 
	
	"�ó�������������", 
	
	"ְҵ�ص㣺������ֵ���߷�����" 
	}, -- �ػ���


	[CLIENT_CAREER_WARRIOR_DOUBLEHAND] = 
	{ "������", 

	"    �����ǿ񱩵Ķ�ʿ��������ɱ¾�ߡ����Ƕ�ս���������ڳ��˵�����ͨ������ǣ�Ƽ��������ҵ��ˣ�Ȼ�����侪�˵�Ǳ��˲�伤��ǿ����˺�����������������˺���", 
	
	"�ó��������Խ�", 
	
	"ְҵ�ص㣺������˺���ǣ�Ƽ���" 
	}, -- ������


	[CLIENT_CAREER_MAGICOR_SINGLEHAND] = 
	{	"������", 

	"    �������շǷ��ķ�Χħ������������ʹ�ߡ����Ż������۵����Ƕ�ս�����Ŷ��ص���⣬ӵ����׿Խ�Ĵ�Χ����ħ��������ż���Ч��ʹ�����������ܡ�", 
		
	"�ó�������ħ�ȡ�����", 
	
	"ְҵ�ص㣺 Զ�̷�Χħ���˺������ټ���" 
	}, -- ������


	[CLIENT_CAREER_MAGICOR_DOUBLEHAND] = 
	{	"����ʦ", 

	"    ����������ǵĴ����ʡ�����ǿ����ƻ��������������Ե�������裬���Ŷ��бز����ٵ����У���������ĵ���֮����ּ��õ��˻ҷ�����", 
		
	"�ó�������ħ�ȡ�����", 
		
	"ְҵ�ص㣺Զ�̵���ħ���˺�" 
	}, -- ����ʦ


	[CLIENT_CAREER_GUNER_SINGLEHAND] = 
	{ "��ɱ��", 
	
	"    ������һ�����ĳ�У�����������ɱ�֡�����ʹ��˫��ǹ��Ϊ�Լ��������� ͨ��������ף���ȷ�����ǣ���ŵ��ˣ���ɱ�ڴ��ֲ����У��Ƕ�����Ҫ���ζ�����", 
	
	"�ó�����������ǹ������ǹ", 
	
	"ְҵ�ص㣺 ������ס�Զ�������˺�" 
	}, -- ��ɱ��


	[CLIENT_CAREER_GUNER_DOUBLEHAND] = 
	{ "������", 
	
	"    �����Ǳ��Ƶ�ר�ң�ս������¾�ߡ����Ǿ�ͨ���ֻ�ǹ��ǿ��Ļ���֧Ԯ��Ϊ�˶��ѵ���������ǿ��Ⱥ��ɱ���������˲��ݺ��ӣ���̼�ݻ�һ���������������˵�ӡ��", 
	
	"�ó���������ǹ", 
	
	"ְҵ�ص㣺 Զ�̷�Χ�����˺�" 
	}, -- ������


	[CLIENT_CAREER_CHURCH_SINGLEHAND] = 
	{ "ʥ����", 
	
	"    �������Ŷӵľ������䣬����ʹ�Ļ�������ӵ�д����Ļָ����ܣ��Գ�ǿ����������������ߵĶ��ѣ����Ҹ�������ǿ���ף��������ֻҪ�������ڣ���������ս������Զ��ʤ���ߡ�", 
	
	"�ó�������ʥ�ȡ�ʥ��", 
	
	"ְҵ�ص㣺Ⱥ�����Ƽ��ܡ���������" 
	},  -- ʥ����


	[CLIENT_CAREER_CHURCH_DOUBLEHAND] = 
	{ "Ԧ��ʦ", 
	
	"    ����ӵ�в��׵�ħ���˺����������ƿ��׵�Ĵ�ʦ�����Ͻ����ٲ��ߡ����������Ч�Ĵ�����������ǻ����Ŷ��бز����ٵ�Э���ߣ����ø�������ħ��������������ĥ���ˡ�", 
	
	"�ó�������ʥ�ȡ�ʥ��", 
	
	"ְҵ�ص㣺Զ��ħ���˺������似��" 
	},  -- Ԧ��ʦ
};

-- ���԰�ť
function ChangeCareerRaceAttrButton_OnEnter()
	local t_Tip =	{ 
						["����"] = "1����������5��������", -- ��ĳ�����
						["����"] = "1����������25������ֵ\n������ս���У�����ֵÿ3��ظ�Ч��",
						["����"] = "1�㾫������25��ħ��ֵ\n������ս���У�ħ��ֵÿ3��ظ�Ч��",
						["����"] = "1����������5���������",
						["����"] = "1����������10������",
						["��֪"] = "1���֪����5��ħ������",
						["����"] = "1����������5��ħ������",
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

-- ʱװ
local t_fashionid =	
		
{	[CLIENT_CAREER_WARRIOR]	= 3010000,	[CLIENT_CAREER_WARRIOR_SINGLEHAND] = 2011603,	[CLIENT_CAREER_WARRIOR_DOUBLEHAND] = 2011613,
	--								������									������
	[CLIENT_CAREER_MAGICOR]	= 3020000,	[CLIENT_CAREER_MAGICOR_SINGLEHAND] = 2021603,	[CLIENT_CAREER_MAGICOR_DOUBLEHAND] = 2021613,
	--								��ɱ��									������
	[CLIENT_CAREER_GUNER]		= 3030000,	[CLIENT_CAREER_GUNER_SINGLEHAND]	= 2031603,	[CLIENT_CAREER_GUNER_DOUBLEHAND]	= 2031613,
	--								ʥ����									Ԧ��ʦ
	[CLIENT_CAREER_CHURCH]		= 3040000,	[CLIENT_CAREER_CHURCH_SINGLEHAND]	= 2041603,	[CLIENT_CAREER_CHURCH_DOUBLEHAND]	= 2041613,
};

-- �·�
local t_clothesID = 
{	--	�ػ���								������
	[CLIENT_CAREER_WARRIOR_SINGLEHAND] = 2011603,	[CLIENT_CAREER_WARRIOR_DOUBLEHAND] = 2011613,
	--	������								������
	[CLIENT_CAREER_MAGICOR_SINGLEHAND] = 2021603,	[CLIENT_CAREER_MAGICOR_DOUBLEHAND] = 2021613,
	--	��ɱ��								������
	[CLIENT_CAREER_GUNER_SINGLEHAND]	= 2031603,	[CLIENT_CAREER_GUNER_DOUBLEHAND]	= 2031613,
	--	ʥ����								Ԧ��ʦ
	[CLIENT_CAREER_CHURCH_SINGLEHAND]	= 2041603,	[CLIENT_CAREER_CHURCH_DOUBLEHAND]	= 2041613,
};

-- ����
local t_leftHandId	= 
{	--	�ػ���								������
	[CLIENT_CAREER_WARRIOR_SINGLEHAND] = 1061603,	[CLIENT_CAREER_WARRIOR_DOUBLEHAND] = 1041603,
	--	������								������
	[CLIENT_CAREER_MAGICOR_SINGLEHAND] = 1181603,	[CLIENT_CAREER_MAGICOR_DOUBLEHAND] = 1181613,
	--	��ɱ��								������
	[CLIENT_CAREER_GUNER_SINGLEHAND]	= 1281603,	[CLIENT_CAREER_GUNER_DOUBLEHAND]	= 0,
	--	ʥ����								Ԧ��ʦ
	[CLIENT_CAREER_CHURCH_SINGLEHAND]	= 1581603,	[CLIENT_CAREER_CHURCH_DOUBLEHAND]	= 1581613,
};

-- ����
local t_rightHandid	= 
{	--	�ػ���								������
	[CLIENT_CAREER_WARRIOR_SINGLEHAND] = 1011603,	[CLIENT_CAREER_WARRIOR_DOUBLEHAND] = 1031603,
	--	������								������
	[CLIENT_CAREER_MAGICOR_SINGLEHAND] = 1101603,	[CLIENT_CAREER_MAGICOR_DOUBLEHAND] = 1111603,
	--	��ɱ��								������
	[CLIENT_CAREER_GUNER_SINGLEHAND]	= 1201603,	[CLIENT_CAREER_GUNER_DOUBLEHAND]	= 1251603,
	--	ʥ����								Ԧ��ʦ
	[CLIENT_CAREER_CHURCH_SINGLEHAND]	= 1511603,	[CLIENT_CAREER_CHURCH_DOUBLEHAND]	= 1501603,
};

-- ͷ��
local t_headId	= 
{	--	�ػ���								������
	[CLIENT_CAREER_WARRIOR_SINGLEHAND] = 2111603,	[CLIENT_CAREER_WARRIOR_DOUBLEHAND] = 2111613,
	--	������								������
	[CLIENT_CAREER_MAGICOR_SINGLEHAND] = 2121603,	[CLIENT_CAREER_MAGICOR_DOUBLEHAND] = 2121613,
	--	��ɱ��								������
	[CLIENT_CAREER_GUNER_SINGLEHAND]	= 2131603,	[CLIENT_CAREER_GUNER_DOUBLEHAND]	= 2131613,
	--	ʥ����								Ԧ��ʦ
	[CLIENT_CAREER_CHURCH_SINGLEHAND]	= 2141603,	[CLIENT_CAREER_CHURCH_DOUBLEHAND]	= 2141613,
};

-- Ь��
local t_shoesId	= 
{	--	�ػ���								������
	[CLIENT_CAREER_WARRIOR_SINGLEHAND] = 2611603,	[CLIENT_CAREER_WARRIOR_DOUBLEHAND] = 2611613,
	--	������								������
	[CLIENT_CAREER_MAGICOR_SINGLEHAND] = 2621603,	[CLIENT_CAREER_MAGICOR_DOUBLEHAND] = 2621613,
	--	��ɱ��								������
	[CLIENT_CAREER_GUNER_SINGLEHAND]	= 2631603,	[CLIENT_CAREER_GUNER_DOUBLEHAND]	= 2631613,
	--	ʥ����								Ԧ��ʦ
	[CLIENT_CAREER_CHURCH_SINGLEHAND]	= 2641603,	[CLIENT_CAREER_CHURCH_DOUBLEHAND]	= 2641613,
};

-- ���
local t_shoulderId	= 
{	--	�ػ���								������
	[CLIENT_CAREER_WARRIOR_SINGLEHAND] = 2211603,	[CLIENT_CAREER_WARRIOR_DOUBLEHAND] = 2211613,
	--	������								������
	[CLIENT_CAREER_MAGICOR_SINGLEHAND] = 2221603,	[CLIENT_CAREER_MAGICOR_DOUBLEHAND] = 2221613,
	--	��ɱ��								������
	[CLIENT_CAREER_GUNER_SINGLEHAND]	= 1271603,	[CLIENT_CAREER_GUNER_DOUBLEHAND]	= 1271613,
	--	ʥ����								Ԧ��ʦ
	[CLIENT_CAREER_CHURCH_SINGLEHAND]	= 2241603,	[CLIENT_CAREER_CHURCH_DOUBLEHAND]	= 2241613,
};

local t_modelviewConfig = 
{
[POS_ID_CLOSES]		= t_clothesID,
[POS_ID_LEFTHAND]	= t_leftHandId,
[POS_ID_RIGHTHAND]	= t_rightHandid,
[POS_ID_HAT]		= t_headId,
--  Ь��û��[POS_ID_HAT]		= t_shoesId,
[POS_ID_SHOULD]		= t_shoulderId,
};

function UpdateChangeCareerRoleAvatar( data )	
	--[[
	ְҵ����	ְҵID		����		����		�·�		ͷ��		ѥ��		���
	�ػ���		17			1011603		1061603		2011603		2111603		2611603		2211603
	������		33			1031603					2011613		2111613		2611613		2211613

	������		18			1101603		1181603		2021603		2121603		2621603		2221603
	����ʦ		34			1111603		1181613		2021613		2121613		2621613		2221613

	��ɱ��		19			1201603		1281603		2031603		2131603		2631603		1271603
	������		35			1251603					2031613		2131613		2631613		1271613

	ʥ����		20			1511603		1581603		2041603		2141603		2641603		2241603
	Ԧ��ʦ		36			1501603		1581613		2041613		2141613		2641613		2241613
	--]]

	-- {1943, 83, 3159}  -- ����
	-- local pos			= NudeAvatarPos[11];
	local pos			= {5348, 246, 3648}  -- ����
	local modelView		= getglobal( "ChangeCareerFrameRoleModelBtnModelView" );
	
	local nRace			= data["race"];
	local nGender		= data["gender"];
	if not modelView:isHaveCameraAnim() then
		modelView:setCameraAnim("scene\\model\\DL\\dl_cam.omod");
	end
	
	modelView:setAvatarAttrib(0, 0, 0, 0, true );
	modelView:setEntityAvatar(0);
	modelView:setAvatarAttrib(0, 0, nRace, 0, true );	-- gridΪ0��ʾְҵ
	modelView:setAvatarAttrib(0, 1, nGender - 1 );	-- gridΪ0��ʾ�Ա�

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

	--modelView:playEntityAnim(0, GetPoseState(t_leftHandId[nRace]), nil ); -- ��ְҵ�Ļ�����ս���� ����λ108
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
		-- սʿ
		[CLIENT_CAREER_WARRIOR] =	
		{ 
			maxattr = 8,
			maxuv = { u = 66, v = 254, w = 93, h = 9 },
			normaluv = { u = 66, v = 245, w = 93, h = 9 },
			[1] = { attr = "����",  value = 6, maxvalue = 8 }, 
			[2] = { attr = "����",  value = 1, maxvalue = 8 },
			[3] = { attr = "����",  value = 5, maxvalue = 8 },
			[4] = { attr = "����",  value = 3, maxvalue = 8 },
			[5] = { attr = "����",  value = 4, maxvalue = 8 },
			[6] = { attr = "��֪",  value = 6, maxvalue = 8 },
		},

		-- �ػ���
		[CLIENT_CAREER_WARRIOR_SINGLEHAND] =	
		{ 
			maxattr = 8,
			maxuv = { u = 66, v = 254, w = 93, h = 9 },
			normaluv = { u = 66, v = 245, w = 93, h = 9 },
			[1] = { attr = "����",  value = 6, maxvalue = 8 }, 
			[2] = { attr = "����",  value = 1, maxvalue = 8 },
			[3] = { attr = "����",  value = 3, maxvalue = 8 },
			[4] = { attr = "����",  value = 3, maxvalue = 8 },
			[5] = { attr = "����",  value = 4, maxvalue = 8 },
			[6] = { attr = "��֪",  value = 4, maxvalue = 8 },
		},

		-- ������
		[CLIENT_CAREER_WARRIOR_DOUBLEHAND] =	
		{ 
			maxattr = 8,
			maxuv = { u = 66, v = 254, w = 93, h = 9 },
			normaluv = { u = 66, v = 245, w = 93, h = 9 },
			[1] = { attr = "����",  value = 8, maxvalue = 8 }, 
			[2] = { attr = "����",  value = 1, maxvalue = 8 },
			[3] = { attr = "����",  value = 2, maxvalue = 8 },
			[4] = { attr = "����",  value = 3, maxvalue = 8 },
			[5] = { attr = "����",  value = 3, maxvalue = 8 },
			[6] = { attr = "��֪",  value = 4, maxvalue = 8 },
		},
		
		-- ��ʦ
		[CLIENT_CAREER_MAGICOR] =	
		{ 
			maxattr = 8;
			maxuv = { u = 66, v = 254, w = 93, h = 9 },
			normaluv = { u = 66, v = 245, w = 93, h = 9 },
			[1] = { attr = "����",  value = 8, maxvalue = 10 }, 
			[2] = { attr = "����",  value = 5, maxvalue = 10 },
			[3] = { attr = "����",  value = 6, maxvalue = 10 },
			[4] = { attr = "����",  value = 5, maxvalue = 10 },
			[5] = { attr = "����",  value = 8, maxvalue = 10 },
			[6] = { attr = "��׼",  value = 8, maxvalue = 10 },
		},
		
		-- ������
		[CLIENT_CAREER_MAGICOR_SINGLEHAND] =	
		{ 
			maxattr = 8;
			maxuv = { u = 66, v = 254, w = 93, h = 9 },
			normaluv = { u = 66, v = 245, w = 93, h = 9 },
			[1] = { attr = "����",  value = 1, maxvalue = 8 }, 
			[2] = { attr = "����",  value = 6, maxvalue = 8 },
			[3] = { attr = "����",  value = 2, maxvalue = 8 },
			[4] = { attr = "����",  value = 3, maxvalue = 8 },
			[5] = { attr = "����",  value = 3, maxvalue = 8 },
			[6] = { attr = "��֪",  value = 4, maxvalue = 8 },
		},
		
		-- ����ʦ
		[CLIENT_CAREER_MAGICOR_DOUBLEHAND] =	
		{ 
			maxattr = 8;
			maxuv = { u = 66, v = 254, w = 93, h = 9 },
			normaluv = { u = 66, v = 245, w = 93, h = 9 },
			[1] = { attr = "����",  value = 1, maxvalue = 8 }, 
			[2] = { attr = "����",  value = 7, maxvalue = 8 },
			[3] = { attr = "����",  value = 2, maxvalue = 8 },
			[4] = { attr = "����",  value = 3, maxvalue = 8 },
			[5] = { attr = "����",  value = 1, maxvalue = 8 },
			[6] = { attr = "��֪",  value = 4, maxvalue = 8 },
		},
		
		-- ǹ��
		[CLIENT_CAREER_GUNER] =	
		{ 
			maxattr = 8;
			maxuv = { u = 66, v = 254, w = 93, h = 9 },
			normaluv = { u = 66, v = 245, w = 93, h = 9 },
			[1] = { attr = "����",  value = 6, maxvalue = 10 }, 
			[2] = { attr = "����",  value = 6, maxvalue = 10 },
			[3] = { attr = "����",  value = 5, maxvalue = 10 },
			[4] = { attr = "����",  value = 6, maxvalue = 10 },
			[5] = { attr = "����",  value = 8, maxvalue = 10 },
			[6] = { attr = "��׼",  value = 8, maxvalue = 10 },
		},
		
		-- ��ɱ��
		[CLIENT_CAREER_GUNER_SINGLEHAND] =	
		{ 
			maxattr = 8;
			maxuv = { u = 66, v = 254, w = 93, h = 9 },
			normaluv = { u = 66, v = 245, w = 93, h = 9 },
			[1] = { attr = "����",  value = 8, maxvalue = 8 }, 
			[2] = { attr = "����",  value = 2, maxvalue = 8 },
			[3] = { attr = "����",  value = 2, maxvalue = 8 },
			[4] = { attr = "����",  value = 3, maxvalue = 8 },
			[5] = { attr = "����",  value = 1, maxvalue = 8 },
			[6] = { attr = "��֪",  value = 4, maxvalue = 8 },
		},
		
		-- ������
		[CLIENT_CAREER_GUNER_DOUBLEHAND] =	
		{ 
			maxattr = 8;
			maxuv = { u = 66, v = 254, w = 93, h = 9 },
			normaluv = { u = 66, v = 245, w = 93, h = 9 },
			[1] = { attr = "����",  value = 6, maxvalue = 8 }, 
			[2] = { attr = "����",  value = 2, maxvalue = 8 },
			[3] = { attr = "����",  value = 2, maxvalue = 8 },
			[4] = { attr = "����",  value = 3, maxvalue = 8 },
			[5] = { attr = "����",  value = 3, maxvalue = 8 },
			[6] = { attr = "��֪",  value = 4, maxvalue = 8 },
		},
		
		-- ��˾
		[CLIENT_CAREER_CHURCH] =	
		{ 
			maxattr = 9;
			maxuv = { u = 66, v = 254, w = 93, h = 9 },
			normaluv = { u = 66, v = 245, w = 93, h = 9 },
			[1] = { attr = "����",  value = 5, maxvalue = 10 }, 
			[2] = { attr = "����",  value = 6, maxvalue = 10 },
			[3] = { attr = "����",  value = 8, maxvalue = 10 },
			[4] = { attr = "����",  value = 6, maxvalue = 10 },
			[5] = { attr = "����",  value = 8, maxvalue = 10 },
			[6] = { attr = "��׼",  value = 8, maxvalue = 10 },
		},
		
		-- ʥ����
		[CLIENT_CAREER_CHURCH_SINGLEHAND] =	
		{ 
			maxattr = 9;
			maxuv		= { u = 66, v = 254, w = 93, h = 9 },
			normaluv	= { u = 66, v = 245, w = 93, h = 9 },
			[1] = { attr = "����",  value = 1, maxvalue = 8 }, 
			[2] = { attr = "����",  value = 4, maxvalue = 8 },
			[3] = { attr = "����",  value = 2, maxvalue = 8 },
			[4] = { attr = "����",  value = 3, maxvalue = 8 },
			[5] = { attr = "����",  value = 2, maxvalue = 8 },
			[6] = { attr = "��֪",  value = 4, maxvalue = 8 },
		},
		
		-- Ԧ��ʦ
		[CLIENT_CAREER_CHURCH_DOUBLEHAND] =	
		{ 
			maxattr = 9;
			maxuv = { u = 66, v = 254, w = 93, h = 9 },
			normaluv = { u = 66, v = 245, w = 93, h = 9 },
			[1] = { attr = "����",  value = 1, maxvalue = 8 }, 
			[2] = { attr = "����",  value = 6, maxvalue = 8 },
			[3] = { attr = "����",  value = 2, maxvalue = 8 },
			[4] = { attr = "����",  value = 3, maxvalue = 8 },
			[5] = { attr = "����",  value = 1, maxvalue = 8 },
			[6] = { attr = "��֪",  value = 4, maxvalue = 8 },
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
		-- TODO: �Ȳ���ʾ�Ȳ߻�������
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

-- תְ�����
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

-- ���ְҵ��ť
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

-- ����ģ��
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

-- ������ת
function ChangeCareerFrame_TurnLeftBtn_OnMouseDown()
	local modelView	= getglobal( "ChangeCareerFrameRoleModelBtnModelView" );
	t_changeCareerControl["rotateAngle"] = t_changeCareerControl["rotateAngle"] - 5;
	modelView:setEntityRotate( 0, t_changeCareerControl["rotateAngle"] );
end

-- ������ת
function ChangeCareerFrame_TurnRightBtn_OnMouseDown()
	local modelView	= getglobal( "ChangeCareerFrameRoleModelBtnModelView" );
	t_changeCareerControl["rotateAngle"] = t_changeCareerControl["rotateAngle"] + 5;
	modelView:setEntityRotate( 0, t_changeCareerControl["rotateAngle"] );
end

-- תְ��ť
function ChangeCareerFrame_ChangeCareerBtn_OnClick()
	if ConfirmChangeCareerFrame:IsShown() then
		return;
	end

	ConfirmChangeCareerFrame:Show();
end

-- תְȷ�����
function UpdateConfirmChangeCareerFrame()
	local t_riches =	 { ["ConfirmChangeCareerFrameRich"] = 
							{ 
								value = "#Pȷ��תְΪ"..t_allCareerDesc[t_changeCareerControl:getSelectCareer()]["desc"].."��#n", 
								color = { r = 255, g = 254, b = 189 } 
							},
						};
	util.SetRiches( t_riches );
end

function ConfirmChangeCareerFrame_OnShow()
	UpdateConfirmChangeCareerFrame();
	this:SetPoint( "center", "UIClient", "center", 0, 0 );
end

-- ȷ��תְ��ť
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