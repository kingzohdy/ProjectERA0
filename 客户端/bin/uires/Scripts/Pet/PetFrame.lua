-- �³���ϵͳ
PET_WASH_GENIUS_MONEY		= 100;
PET_ADD_SKILL_GRID_MONEY	= 100;
PET_WASH_SKILL_GRID_MONEY	= 100;
-- ���＼����
RESULT_PET_SKILL_BOOK		= 89;
--ϴ���ܸ����
RESULT_PET_WASH_SKILL_GRID	= 193;
-- ������������
RESULT_PET_SKILL_UP_STUFF	= 211;
-- ϴ���ʵ���
RESULT_PET_WASH_GENIUS		= 192;
-- ����ǿ���ɳ���
RESULT_PET_STRENGTHEN_SAFE	= 194;
-- ����ǿ���ɳ�����
RESULT_PET_STRENGTHEN_STUFF	= 200;
-- ������ǿ����
RESULT_PET_ADD_ATTR			= 195;
-- �����ҳ϶�����
MAX_PET_LOVE_VAL			= 10000;
-- �����ս�ҳ϶�Ҫ��
PET_FIGHT_LOVEVAL			= 3000;
MAX_PET_NUM = 5;
-- ����ںϴ���
MAX_PET_RONG_HE_NUM			= 3;
-- ���ɳ�ǿ���ȼ�
MAX_STRENGTH_LEVEL			= 12;
-- ������͵ȼ�
PEIT_HETI_MIN_LEVEL			= 60;
local ModelAngle = 0;
-- ������ǿ���

-- ���������
-- @param: �������ʵ��Ķ�̬��Ϣ
-- @action: ��ʾ�������ϸ��Ϣ
PET_GENIUS_D = 0; 	--/* ��ͨ���� */
PET_GENIUS_C = 1; 	--/* �������� */
PET_GENIUS_B = 2; 	--/* �������� */
PET_GENIUS_A = 3; 	--/* �ܳ����� */
PET_GENIUS_S = 4; 	--/* �������� */
-- ������༼�ܸ�λ
MAX_LOCK_SKILL_GRID	= 1;
MAX_PET_SKILL_GRID	= 5;
MAX_PET_PACKAGE		= 8;
local MAX_PET_ONE_PAGE = 6;
local t_mainSelPetInfo = { heightId = 0, lowId = 0, id = 0, index = -1, getNew = false };
local requestStrengthen
--���＼�ܽ������
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

-- ȫ�ָ�������
-- ��ȡ�����ĳ��������
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
								[CLIENT_PET_GENIUS_D] = { value = "��ͨ", color = { r = 255,	g = 255,	b = 255	},	desc="��ͨƷ��", 
															hexiColor = "ffffff", money = CLIENT_PET_GENIUS_D_WASH_MONEY, }, 
								[CLIENT_PET_GENIUS_C] = { value = "����", color = { r = 63,	g = 193,	b = 23	},	desc="����Ʒ��", 
															hexiColor = "3fc117", money = CLIENT_PET_GENIUS_C_WASH_MONEY,	},
								[CLIENT_PET_GENIUS_B] = { value = "����", color = { r = 51,	g = 141,	b = 253	},	desc="����Ʒ��", 
													hexiColor = "338dfd", money = CLIENT_PET_GENIUS_B_WASH_MONEY, },
								[CLIENT_PET_GENIUS_A] = { value = "�ܳ�", color = { r = 142,	g = 92,		b = 216	},	desc="�ܳ�Ʒ��", 
															hexiColor = "8e5cd8", money = CLIENT_PET_GENIUS_A_WASH_MONEY, },
								[CLIENT_PET_GENIUS_S] = { value = "����", color = { r = 245,	g = 115,	b = 0	},	desc="����Ʒ��", 
													hexiColor = "f57300", money = CLIENT_PET_GENIUS_S_WASH_MONEY,	},
						};

-- 
function GetPetGeniusValueDesc( petInfo )
	local t_geniusInfo = t_geniusDesc[petInfo.PetGenius];
	if t_geniusInfo == nil then
		return "��ͨ";
	end

	return t_geniusInfo["value"];
end

function GetPetGeniusDesc( petInfo )
	local t_geniusInfo = t_geniusDesc[petInfo.PetGenius];
	if t_geniusInfo == nil then
		return "��ͨƷ��";
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
-- ��ͨ
[CLIENT_PET_TYPE_NORMAL] =	{ 
		[1] = { path = "uires\\ZhuJieMian\\1.dds", UV = { x = 377, y = 756, width = 22, height = 16 }, }, 
		[2] = { path = "uires\\ZhuJieMian\\1.dds", UV = { x = 400, y = 756, width = 22, height = 16 }, }, 
		},

-- ����
[CLIENT_PET_TYPE_ABERRANCE] =	{ 
		[1] = { path = "uires\\ZhuJieMian\\1.dds", UV = { x = 423, y = 756, width = 22, height = 16 }, }, 
		[2] = { path = "uires\\ZhuJieMian\\1.dds", UV = { x = 446, y = 756, width = 22, height = 16 }, }, 
		},
};
-- �������						-- 277
local t_petTypeTextures	=	{	[0] = { desc = "Ѫ��", path = "uires\\ZhuJieMian\\1.dds", UV = { x = 258, y = 494, width = 18, height = 18 } }, 
								[1] = { desc = "����", path = "uires\\ZhuJieMian\\1.dds", UV = { x = 296, y = 494, width = 18, height = 18 } },
								[2] = { desc = "����", path = "uires\\ZhuJieMian\\1.dds", UV = { x = 277, y = 494, width = 18, height = 18 } },
							};
t_petLoveLevelTextures = {
									[0] = { UV = { x = 952, y = 627, width = 20, height = 17 } },
									[1] = { UV = { x = 952, y = 645, width = 20, height = 17 } },
									[2] = { UV = { x = 973, y = 645, width = 20, height = 17 } },
								};
--[[
local t_texturesDef =	{
							-- �����Ա�
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

							-- ���﷨ħ����
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
PET_SKILL_DISABLE		= 0;	-- /* δ���� */
PET_SKILL_ATK			= 1;	-- /* �����༼�� */
PET_SKILL_OWNER_BUFF	= 2;	-- /* �����༼�� */
PET_SKILL_SELF_BUFF		= 3;	-- /* ��ǿ�༼�� */

PET_TYPE_NORMAL		= 0; 	-- /* ��ͨ���� */
PET_TYPE_ABERRANCE	= 1; 	-- /* ���ͳ��� */
local t_skillGridTga = 
						{
						[PET_SKILL_ATK]			= { path = [[uires\TuPianLei\ChongWuJiNengLeiXing\1.tga]], typeDesc = "������" },
						[PET_SKILL_OWNER_BUFF]	= { path = [[uires\TuPianLei\ChongWuJiNengLeiXing\3.tga]], typeDesc = "������" },
						[PET_SKILL_SELF_BUFF]	= { path = [[uires\TuPianLei\ChongWuJiNengLeiXing\2.tga]], typeDesc = "��ǿ��" },
						};

-- doModal
-- ����ģ��doModalģʽ�ĵ���ʽ�Ի���
local t_modalFrame =	{	modalFrames		=	{	"DropPetConfirmFrame", "OpenPetSkillConfirmFrame" },


							disabledButtons	=	{	"PetFrame_PrePageBtn",	"PetFrame_NextPageBtn",		"PetFrame_FightBtn", 
													"PetFrame_BackUpBtn",	"PetFrame_TameBtn",			"PetFrame_FreedomBtn", 
													"PetFrame_CloseBtn"
												},
						};

-- ����رճ�������ʱ��ر�
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

SKILL_ATTR_NONE		= 0;	-- �޼���
SKILL_ATTR_ICE		= 1;	-- ���� 
SKILL_ATTR_FIRE		= 2;	-- ��
SKILL_ATTR_THUNDER	= 3;	--  
SKILL_ATTR_LIGHT	= 4;	-- 
SKILL_ATTR_NIGHT	= 5;	-- 
SKILL_ATTR_PHSIC	= 6;	-- �﹥	

PET_MAX_FIGHT_SHI	= 1;	-- ͬʱ�������ĳ�������
-- ���﹥������
local t_attackTypeDesc = { [0] = { desc = "��    ��"}, [1] = { desc = "��    ��"}, [2] = { desc = "��    ��"}, [3] = { desc = "��    ��"}, };

local t_buttonEvent =	{
							["PetFrame_PrePageBtn"]	= {},	["PetFrame_NextPageBtn"]	= {},	["PetFrame_FightBtn"]	= {},  
							["PetFrame_BackUpBtn"]	= {},	["PetFrame_TameBtn"]		= {},	["PetFrame_FreedomBtn"]	= {},	
							["PetFrame_CloseBtn"]	= {},
						};


function PetFrame_Btn_OnLeave()
	GameTooltip:Hide()
	WideGameTooltip:Hide()
end

-- ������ť
function PetFrame_FightBtn_OnEnter()
	--[[
	if t_mainSelPetInfo["id"] == 0 then
		return;
	end
	--]]
	
	local szText = "�ٻ���������һ�𲢼���ս";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName(), vertical = "bottom" } );
end

-- ѱ����ť
function PetFrame_TameBtn_OnEnter()
	--[[
	if t_mainSelPetInfo["id"] == 0 then
		return;
	end
	--]]
	local szText = "ʹ�á�ѱ���ǹ������ӳ����ҳ϶�";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName(), vertical = "bottom" } );
end

-- �ٻذ�ť
function PetFrame_BackUpBtn_OnEnter()
	--[[
	if t_mainSelPetInfo["id"] == 0 then
		return;
	end
	--]]
	local szText = "��ʹ�øð�ť�ٻس���";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName(), vertical = "bottom" } );
end

-- ������ť
function PetFrame_FreedomBtn_OnEnter()
	--[[
	if t_mainSelPetInfo["id"] == 0 then
		return;
	end
	--]]

	local szText = "�����������л�������Դ�������ӵ�һ��";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName(), vertical = "bottom" } );
end

function PetFrameXinShouBtn_OnHide()
	local t_hideIcons = { "PetFrame_TameBtnUVAnimationTex", "PetFrame_FightBtnUVAnimationTex" };
	util.HideIcons( t_hideIcons );
end

--  ���������ť
function PetFrame_DaiShuBtn_OnEnter()
	local petInfo = GetShowTipsPetInfo( this:GetParent() );
	if not petInfo then
		return;
	end

	local petDef		= getPetDef( petInfo.PetID );
	local szText;
	--[[if petDef.Type == PET_TYPE_ABERRANCE then
		szText = ( petInfo.Hierarchy == 2 ) and "���������������ں�" or "һ�������������ں�";
	else
		szText = ( petInfo.Hierarchy == 2 ) and "������������ں�" or "һ����������ں�";
	end]]
--[[
	local mainplayer		= ActorMgr:getMainPlayer();
	local petPackage		= mainplayer:getPet();
	
	if petPackage:isExtraBattlePet( petInfo ) then 
		szText = (petDef.Type == PET_TYPE_ABERRANCE) and "����ս��" or "һ��ս��"
	else 
		szText = (petDef.Type == PET_TYPE_ABERRANCE) and "�������" or "һ������"
	end
--]]
	szText = "��ʼ�ɳ���"..petDef.Important.."\n��ǰ�ɳ���"..petInfo.Imp.."\n\n"
	local ImpUpDef = getPetImpUpDef( petInfo.Imp );
	if ImpUpDef == nil then 
		szText = szText.."��ǰ�Ѵﵽ��߳ɳ��ȼ�"
	else 
		szText = szText.."��Ǳ���ﵽ100%���ǰ��ʱ��ǣ�\n������İ����½���Ϊ"..( petInfo.Imp + 1 );
	end
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function PetFrame_TipAreaBtn_OnEnter()
	local szText = "ս��������\n1.���ʹ��ս������,����øó����40%���Լӳ�\n2.���ʹ��ս������,����������δ����ս���25%���Լӳ�";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function PetFrameBattlePetBtn_OnEnter()
	local szText = "ս��:\n\t\t��ͨ����ȼ��ﵽ60��Ʒ�ʴﵽ����,����ʱ������¶ദ,����Ϊ����ǿ���ս��\n\t\tս�����,���˽���ø�ս���40%����,������������ÿֻս��25%������";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end
-- ����ȼ���ť
function PetFrame_PetLvValueBtn_OnEnter()
	local petInfo = GetShowTipsPetInfo( this:GetParent() );
	if not petInfo then
		return;
	end

	local szText = "    �����ĳ���ȼ�������ʱ������õľ��齫�𽥼��٣������ĵȼ�������10��ʱ�����޷����ٻ���";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

local t_petTypeTips =	{
						[0] = { desc = "Ѫ��", tips = "�ó�����������ø���������ɳ��Լ�������˫���������ܳɳ�" },
						[1] = { desc = "����", tips = "�ó�����������ø���˫ϵ�����ɳ��Լ������������ɳ������ܳɳ�" },
						[2] = { desc = "����", tips = "�ó�����������ø���˫ϵ�����ɳ��Լ����������гɳ�" },
						--[3] = { desc = "����", tips = "�ó�����������ø�������С�����" },
						};
-- ���﹥������ ���� ħ��
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
				ShowMidTips( "�����ϼ��У����ܽ��а��Ĳ���" );
				return;
			end
			if GameExchange:isInExchangePetByWID( t_mainSelPetInfo.heightId, t_mainSelPetInfo.lowId ) then
				ShowMidTips( "�������ڽ����У����ܽ��а��Ĳ���" );
				return;
			end

			PetAddLoveLevelMessageFrame:SetClientUserData( 0, petInfo.WID.nHeight )
			PetAddLoveLevelMessageFrame:SetClientUserData( 1, petInfo.WID.nLow )
			PetAddLoveLevelMessageFrame:Show()
		end
	end
end

tLoveLevelDesc = {			[0] = [[��������#cff1414�������#n������ɽ��׶��Σ�
#G������ΰ�ť��ʹ���ĳ������Ϊ���벻��״̬#n

#cf57300�������������ڲ��벻��״̬ʱ:
�������л�����������10%
�����ҳ϶���������20%
���ﲻ�ܱ��ٴν���#n]],
							[1] = [[��������#cff64ff��ʶ��֪#n������ɽ���һ�Σ�
#G������ΰ�ť��ʹ���ĳ������Ϊ���벻��״̬#n

#cf57300�������������ڲ��벻��״̬ʱ:
�������л�����������10%
�����ҳ϶���������20%
���ﲻ�ܱ��ٴν���#n]],
							[2] = [[��������#cffe132���벻��#n��
�������л�����������10%
�����ҳ϶���������20%
���ﲻ�ܱ��ٴν���#n]],
					}
function PetFrameLoveLevelBtn_OnEnter()
	local petInfo = GetShowTipsPetInfo( this:GetParent() )
	ShowGameTooltip( { rich = "WideGameTooltipRichText", tipsFrame = "WideGameTooltip", text = tLoveLevelDesc[petInfo.LoveLevel], frame = this:GetParent(),
						button = this:GetName() } );
end

-- �������ʰ�ť	���	��ͨ ���� ���� �ܳ� ���� 
local t_geniusTips =	{
								--[PET_GENIUS_D] = 
								-- ��ͨ -- һ��
								"#cffffff��ͨ���������������Գɳ�#n\n", 
								--[PET_GENIUS_C] = 
								-- ���� -- ����
								"#c3fc117���ã�����ͨ���ʳ����ø���ɳ�#n\n", 
								--[PET_GENIUS_B] = 
								-- ���� -- ����
								"#c338dfd���㣺���������ʳ����ø���ɳ�#n\n",
								--[PET_GENIUS_A] = 
								-- �ܳ� -- ʷʫ
								"#c8e5cd8�ܳ������������ʳ����ø���ɳ�#n\n", 
								--[PET_GENIUS_S] = 
								-- ���� -- ����
								"#cf57300�������Ƚܳ����ʳ����ø���ɳ�#n\n"
						};

function PetFrame_PetGeniusValuBtn_OnEnter()
	local petInfo = GetShowTipsPetInfo( this:GetParent() );
	if not petInfo then
		return;
	end
	
	local szText = "#cffffff��ͨ���������������Գɳ�#n\n#c3fc117���ã�����ͨ���ʳ����ø���ɳ�#n\n#c338dfd���㣺���������ʳ����ø���ɳ�#n\n#c8e5cd8�ܳ������������ʳ����ø���ɳ�#n\n#cf57300�������Ƚܳ����ʳ����ø���ɳ�#n"
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(), 
						button = this:GetName() } );
end

-- TODO: �����Ը�ť
local t_PetCharacteDesc = { ["����"] = { tips = "���棺������к�����5%" },		["³ç"] = { tips = "³ç����߱�����3%" }, 
							["��Ѫ"] = { tips = "��Ѫ������﹥�ͷ���5%" },		["��С"] = { tips = "��С���������ͷ���5%" }, 
							["�侲"] = { tips = "�侲�������������10%" },		["Ӣ��"] = { tips = "Ӣ�£����������5%" }, 
							["��Ƨ"] = { tips = "��Ƨ������������5%" },			["̹��"] = { tips = "̹�ʣ�������С��﹥������������ʡ�����ֵ����5% ( ��������ɻ�� )" }, 
							["����"] = { tips = "������������ܡ������������������ʡ�����ֵ����5% ( ��������ɻ�� )" },  
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

-- �����ܳɳ���ť
function PetFrame_PetGrowthBtn_OnEnter()
	local petInfo = GetShowTipsPetInfo( this:GetParent() );
	if not petInfo then
		return;
	end
	
	local szText = "����ɳ���ֵ���Ǻ�����������ɳ������ı�׼";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

-- ����ս������ʾ��ť
function PetFrame_FightValueBtn_OnEnter()
	local szText = "���ﵱǰս����������ֵ�ܵ����ﵱǰ����Ӱ��";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function PetFrame_QiangHuaStarBackBtn_OnEnter()
	local szText = "����ǿ��ֱ����ǿ����ս�����ԣ�ÿ��ǿ������5%";
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
		szText = "��������ǿ����������������"..nIncrAttr.."%";
	else
		szText = "������ͨǿ����������������"..nIncrAttr.."%";
	end

	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = "cursor", vertical = "top", horizonal="right" } );
end

-- �����ٻ��ȼ���ʾ��ť
function PetFrame_CallLvValueBtn_OnEnter()
	local nPetID = GetShowTipsPetID( this:GetParent() );
	if nPetID == 0 then
		return;
	end
	
	local petDef = getPetDef( nPetID );
	-- ..petDef.Name
	local szText = "���ɫ�ȼ��ﵽ"..petDef.FightLevel.."��ʱ�����ٻ�";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

-- ����ɳ�ǿ����ʾ��ť
function PetFrame_GrowthStrengValueBtn_OnEnter()
	--[[
	local petInfo = GetShowTipsPetInfo( this:GetParent() );
	if not petInfo then
		return;
	end
	--]]
	local szText = "����ǿ���ȼ���Ӱ��������ɳ�";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

-- ����HP����ť
function PetFrame_HPBtn_OnEnter()
	--[[
	local petInfo = GetShowTipsPetInfo( this:GetParent() );
	if not petInfo then
		return;
	end
	--]]
	local szText = "����ֵΪ0ʱ�����ｫ������\nʹ�ó���ҩˮ�����ӱ��ٻ����������\n����ǿ���������������";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

-- ����EXP����ť
function PetFrame_EXPBtn_OnEnter()
	--[[
	local petInfo = GetShowTipsPetInfo( this:GetParent() );
	if not petInfo then
		return;
	end
	--]]
	local szText = "���ﾭ��ﵽ��������ʱ�����ｫ�Զ�����";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

-- ���﹥���ɳ���ť
function PetFrame_AttackGrowthValueBtn_OnEnter()
	local szText = "Ӱ������������õĹ�������������ֵԽ��������������ӵĹ�����Խ��";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

-- ���������ɳ���ť
function PetFrame_LifeGrowthValueBtn_OnEnter()
	--[[
	local petInfo = GetShowTipsPetInfo( this:GetParent() );
	if not petInfo then
		return;
	end
	--]]
	local szText = "Ӱ������������õ�����ֵ��������ֵԽ��������������ӵ�����ֵԽ��";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

-- ��������ɳ���ť
function PetFrame_PhysicDefGrowthValueBtn_OnEnter()
	local szText = "Ӱ������������õ��������ֵ��������ֵԽ��������������ӵ��������ֵԽ��";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

-- ����ħ���ɳ���ť
function PetFrame_IceDefGrowthValueBtn_OnEnter()
	local szText = "Ӱ������������õ�ħ������ֵ��������ֵԽ��������������ӵ�ħ������ֵԽ��";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

-- �������гɳ���ť
function PetFrame_HitGrowthValueBtn_OnEnter()
	--[[
	local petInfo = GetShowTipsPetInfo( this:GetParent() );
	if not petInfo then
		return;
	end
	--]]
	local szText = "Ӱ������������õ����мӳɣ�������ֵԽ�����������������Խ��";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

-- �������ܳɳ���ť
function PetFrame_FleeGrowthValueBtn_OnEnter()
	--[[
	local petInfo = GetShowTipsPetInfo( this:GetParent() );
	if not petInfo then
		return;
	end
	--]]
	local szText = "Ӱ������������õ����ܼӳɣ�������ֵԽ�����������������Խ��";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

local t_attackTypeTips = {	[SKILL_ATTR_PHSIC]	= { desc = "�� ��", tips = "������������ͨ��������ǿ��ֱ����ǿ�����ԣ����5��" },											[SKILL_ATTR_ICE]	= { desc = "�� ��", tips = "����ħ��������ͨ��������ǿ��ֱ����ǿ�����ԣ����5��" } };
-- �����﹥��������ť
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
	local szText		= "���﹥����ͨ������ǿ�������������ԣ����ǿ��12��"
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function PetFrame_MaxLifeValueBtn_OnEnter()
	local szText		= "��������ֵ��ͨ������ǿ����ֱ����ǿ�����ԣ����12��"
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

-- ���﷨����ť
function PetFrame_IceDefValueBtn_OnEnter()
	--[[
	local petInfo = GetShowTipsPetInfo( this:GetParent() );
	if not petInfo then
		return;
	end
	--]]
	local szText = "���﷨��������ͨ������ǿ����ֱ����ǿ�����ԣ����12��";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

-- ���������ť
function PetFrame_PhyDefValueBtn_OnEnter()
	--[[
	local petInfo = GetShowTipsPetInfo( this:GetParent() );
	if not petInfo then
		return;
	end
	--]]
	local szText = "�������������ͨ������ǿ����ֱ����ǿ�����ԣ����12��";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

-- �������а�ť
function PetFrame_HitValueBtn_OnEnter()
	--[[
	local petInfo = GetShowTipsPetInfo( this:GetParent() );
	if not petInfo then
		return;
	end
	--]]
	local szText = "�������У�ͨ������ǿ����ֱ����ǿ�����ԣ����12��";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

-- �������ܰ�ť
function PetFrame_FleeValueBtn_OnEnter()
	--[[
	local petInfo = GetShowTipsPetInfo( this:GetParent() );
	if not petInfo then
		return;
	end
	--]]
	local szText = "�������ܣ�ͨ������ǿ����ֱ����ǿ�����ԣ����12��";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

-- ���ﱩ���ʰ�ť
function PetFrame_HeaveyAttackValueBtn_OnEnter()
	local petInfo = GetShowTipsPetInfo( this:GetParent() );
	if not petInfo then
		return;
	end
	local petImpUp = getPetImpUpDef( petInfo.Imp );
	local szText = ""
	if petImpUp == nil then 
		szText = "���ﵱǰ�Ѵﵽ��߳ɳ��ȼ�"
	else 
		szText = "���ﵱǰǱ��ֵ��"..petInfo.ImpExp.."\n��������Ǳ��ֵ��"..petImpUp.ExpUp;
	end
	
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

-- ���������ʰ�ť
function PetFrame_TiShenValueBtn_OnEnter()
	local petInfo = GetShowTipsPetInfo( this:GetParent() );
	if not petInfo then
		return;
	end
	local hurt 	= petInfo.FightAttr.YongQi/(30*petInfo.FightAttr.YongQi+200)*100 ;
	local point = hurt - hurt % 0.01;
	local rate 	= math.ceil( petInfo.FightAttr.YongQi/(5*petInfo.FightAttr.YongQi+200)*100 ) 
	local szText = "�ٻ������˱����﹥����������" ..rate .. "%����\n�������������⵱���˺�" .. petInfo.FightAttr.DecHurtMul .."%\n\n���壺\nʹ���˶�������ҵ������˺�����"..point.."%\nʹ������Ҷ����˵������˺�����"..point.."%\n���Է������������ֵԽ�ͣ�Ч��Խ�ã�\nδ����ʱ����ֵΪ0��"
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

-- �����ҳ϶Ȱ�ť
function PetFrame_ZhongChenValueBtn_OnEnter()
	--[[
	local petInfo = GetShowTipsPetInfo( this:GetParent() );
	if not petInfo then
		return;
	end
	--]]
	-- �ֳ���鿴������壬�۲����ͳ������������������
	local szText = "�ѱ��ٻ��ĳ����ҳ϶Ȼ���ʱ�����ģ���������ҳ϶�����30%ʱ�������ܱ��ٻ�";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

-- ���¼�����λ��ʾ
function UpdateMainPetFrameSkillGrids( szFrameName, petInfo )
	-- ���ó������еļ���
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

-- ��ȡ�����ս����

function GetPetFightValue( petInfo )
	-- ���ݳ����������������ս���� 
	--[[
	����ս�����¹�ʽ=��������*15+hp+�������*2.5+ħ������*2.5+���еȼ�*4+���ܵȼ�*5��/10
	
	//Ѫ��
	iHP = pstPetLevelDef->HP*(pstRolePet->HPGenius*(100+pstRolePet->StrengthenLevel*5)/100.0)/1000.0;
	//������
	iPhsicAtk = pstPetLevelDef->PhsicAtk*(pstRolePet->PhsicAtkGenius*(100+pstRolePet->StrengthenLevel*5)/100.0)/1000.0;
	//ħ������
	iIceDef = pstPetLevelDef->IceDef*(pstRolePet->IceDefGenius*(100+pstRolePet->StrengthenLevel*5)/100.0)/1000.0;
	//�������
	iDef = pstPetLevelDef->PhsicDef*(pstRolePet->DefGenius*(100+pstRolePet->StrengthenLevel*5)/100.0)/1000.0;
	//����
	iFlee = pstPetLevelDef->Flee*(0.8+pstRolePet->FleeGenius*(100+pstRolePet->StrengthenLevel*5)/100.0/5000.0);
	//���� 
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
	-- �����ܳɳ�����
	local nTotalGrowthValue	= petInfo.PhsicAtkGenius + petInfo.IceAtkGenius + petInfo.HPGenius +																				petInfo.DefGenius + petInfo.HitRateGenius + petInfo.FleeGenius + petInfo.IceDefGenius;
	nTotalGrowthValue = math.floor( nTotalGrowthValue * ( fScale + fExtraStrengthen ) );

	local nAttackGenius	= petInfo.PhsicAtkGenius;
	if nAttackGenius == 0 then
		nAttackGenius = petInfo.IceAtkGenius;
	end
	local fFightScale = ( 1 + 0.05 ) ^ ( petInfo.Level - 1 );
	-- ���ݳ����������������ս���� 
	local t_petTypeFightValue = {
								-- Ѫ��
								[0] = { value = ( nTotalGrowthValue + ( petInfo.HPGenius + petInfo.DefGenius ) / 2 ) * fFightScale }, 
								-- ����
								[1] = { value = ( nTotalGrowthValue + petInfo.DefGenius ) * fFightScale }, 
								-- ����
								[2] = { value = ( nTotalGrowthValue + nAttackGenius ) * fFightScale }, 
								};
	
	-- Ǳ��ֵ
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
						-- ��������
						[szFrameName.."PetNameBtnFont"]			=	{ value = petDef.Name },
						-- ����ȼ�
						[szFrameName.."PetLvValueBtnFont"]		=	{ value = petInfo.Level.."��" },
						-- �������ʼ���
						[szFrameName.."PetGeniusValuBtnFont"]	=	{ value = t_geniusDesc[petInfo.PetGenius]["value"], 
																		color = t_geniusDesc[petInfo.PetGenius]["color"] },
						-- ����HP
						[szFrameName.."HPBtnHPValueFont"]			=	{ value = petInfo.HP .. "/" .. petInfo.MaxHP },
						-- ����EXP
						[szFrameName.."EXPBtnExpValueFont"]			=	{ value = petInfo.Exp .. "/" .. petLevelDef.LevelUpExp },
						
						-- TODO: �ɳ�ֵ
						-- ���﹥���ɳ�ֵ		petInfo.PhsicAtkGenius
						[szFrameName.."AttackGrowthValueBtnFont"]	=	{ value = nAttackGenius },
						-- ���������ɳ�ֵ
						[szFrameName.."LifeGrowthValueBtnFont"]		=	{ value = petInfo.HPGenius },
				
						-- ��������ɳ�ֵ
						[szFrameName.."PhysicDefGrowthValueBtnFont"]=	{ value = petInfo.DefGenius },

						-- ����ħ���ɳ�ֵ
						[szFrameName.."IceDefGrowthValueBtnFont"]	=	{ value = petInfo.IceDefGenius },

						-- �������гɳ�ֵ
						[szFrameName.."HitGrowthValueBtnFont"]		=	{ value = petInfo.HitRateGenius },

						-- �������ܳɳ�ֵ
						[szFrameName.."FleeGrowthValueBtnFont"]		=	{ value = petInfo.FleeGenius },
						-- TODO: �ɳ�ֵ
						
						-- ���﹥��ֵ
						[szFrameName.."TotalAttackValueBtnFont"]	= {
						value = petSkillDef ~= nil and ( t_attackTypeValue[SKILL_ATTR_PHSIC].value ) or "" },-- petSkillDef.MainAttr
						-- ����ֵ 
						[szFrameName.."MaxLifeValueBtnFont"]	= { value = petInfo.MaxHP },
						-- ����ħ������ֵ 
						[szFrameName.."IceDefValueBtnFont"]			=	
						{ value = math.ceil( petInfo.FightAttr.IceDef ) },
						-- �����������ֵ
						[szFrameName.."PhyDefValueBtnFont"]				=	{ value = math.ceil( petInfo.FightAttr.PhsicDef ) },
						-- �������еȼ�
						[szFrameName.."HitValueBtnFont"]				=	
						{ value = math.ceil( petInfo.FightAttr.HitRate ) },
						-- �������ܵȼ�
						[szFrameName.."FleeValueBtnFont"]				=	
						{ value =  ( petInfo.FightAttr.Flee ) },
						
						-- ����Ǳ��ֵ
						[szFrameName.."HeaveyAttackValueBtnFont"]		=	{ value = ( ImpPercent / 100 .. "%" ) },
						-- ��������ֵ
						[szFrameName.."TiShenValueBtnFont"]				=	{ value = petInfo.FightAttr.YongQi },
						-- �����ҳ϶�ֵ	value = math.floor( petInfo.LoveVal / 100 )
						[szFrameName.."ZhongChenValueBtnFont"]			=	{ value = math.floor( petInfo.LoveVal / 100 ).."%" },
						-- ����ս����
						[szFrameName.."FightValueBtnFont"]				=	{ value = GetPetFightValue( petInfo ) },
						-- �����ٻ��ȼ�
						[szFrameName.."CallLvValueBtnFont"]				=	{ value = petDef.FightLevel },
						-- �ǳ�����
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
	-- ս����
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
-- ���Ա仯��Ϣ
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
		ShowMidTips( "����ȼ����ߣ����ٻ�ʱ���ܻ�þ���ֵ" );
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
			ShowMidTips( "����ȼ����ߣ����ٻ�ʱ���ܻ�þ���ֵ" );
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
						["PetFrameListBackGroundNumTitleFont"] = { value = "�����б�" .. petPackage:getPetNum().."/"..petPackage:getPetMaxNum() },
					};
	for name, data in pairs( t_fonts ) do
		local font = getglobal( name );
		font:SetText( data.value );
	end
	ShowMidTips( "���������λ�ɹ�" );
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
	PetFrame_CatchPetDesc:SetText( "���ﲶ׽���ʺ絺  ͨ��#L����[ʱ���]#n����", 255, 255, 190 );
	PetFrame_TrainPetDesc:SetLinkTextColor( 0, 255, 0 );
	PetFrame_TrainPetDesc:SetText( "����ѵ��ʦ��#L���¶�[ʱ���]#n", 255, 255, 190 );
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
								--  ���Ϸ�һҳ��ť
								["PetFrame_PrePageBtn"]		= PrePageBtn_OnClick,
								--  ���·�һҳ��ť
								["PetFrame_NextPageBtn"]	= NextPageBtn_OnClick,
								-- ������ť
								["PetFrame_FightBtn"]		= PetFrameFightBtn_OnClick,	
								-- �ٻذ�ť
								["PetFrame_BackUpBtn"]		= PetFrameBackUpBtn_OnClick,
								-- ѱ����ť
								["PetFrame_TameBtn"]		= PetFrameTameBtn_OnClick,
								-- ������ť
								["PetFrame_FreedomBtn"]		= PetFrameFreedomBtn_OnClick,
								-- �رհ�ť
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


-- doModal������
function PetFrameButtonsClick()
	local petXinShouBtn = getglobal( "PetFrameXinShouBtn" );
	if petXinShouBtn:IsShown() and petXinShouBtn:GetClientString() == this:GetName() and this:GetName() ~= "PetFrame_TameBtn" then
		local t_hideIcons = { this:GetName().."UVAnimationTex", "PetFrameXinShouBtn" };
		util.HideIcons( t_hideIcons );
	end

	-- ģ��doModal,��������Ϸ��嵯���Ķ��������ʾ������£��������һ������ٽ����κβ���
	for _, szFrameName in ipairs( t_modalFrame.modalFrames ) do
		local frame = getglobal( szFrameName );
		if frame:IsShown() then
			return;
		end
	end
	t_buttonEvent[this:GetName()]["click"]();
end

local t_petNpc =	{ 
						["���¶�"]	= { x = 28700, y = 26118, mapId = 1000, npcId = 304006 }, 
						["����"]	= { x = 29100, y = 27000, mapId = 1000, npcId = 317005 }, 
					};
-- ����ѵ��ʦ����
function PetFrame_TrainPetDesc_OnClick()
	if ( arg3 == "LeftButton" ) then
		--MapShower:GetMaxMapContal():linkMoveto( t_petNpc["mapId"], t_petNpc["x"], t_petNpc["y"] );	
		local t_destNpcInfo = t_petNpc["���¶�"];
		MapShower:GetMaxMapContal():linkMoveto( t_destNpcInfo["mapId"], t_destNpcInfo["npcId"] );	
	end
end

-- �������
function PetFrame_CatchPetDesc_OnClick()
	if ( arg3 == "LeftButton" ) then
		--MapShower:GetMaxMapContal():linkMoveto( t_petNpc["mapId"], t_petNpc["x"], t_petNpc["y"] );	
		local t_destNpcInfo = t_petNpc["����"];
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
	-- �жϳ����״̬
	local petPackage	= mainplayer:getPet();
	local petInfo		= petPackage:getPetInfo( nHeightId, nLowId );
	t_mainSelPetInfo = { heightId = petInfo.WID.nHeight, lowId = petInfo.WID.nLow, id = petInfo.PetID, 
							index = petPackage:getPetIndex( petInfo ), getNew = true };
	UpdatePetPhotoWhenGetNewPet();
	if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_PET_EGG_CONTAINER_BTN ) then
		GameWizard:doStep( GAMEWIZARD_FIRST_BLINK_PET_EGG_CONTAINER_BTN );
	end

	if not GameWizard:isDo(GAMEWIZARD_FIRST_GET_PET_BLINK_PET_SHORTCUT_BTN) then
		GuideEffectShow( "", "����򿪳������", "PetButton", "����һֻ�������򿪳��������Բ鿴\n��ݼ���X��", "uires\\ZhuJieMian\\1.dds", 352, 609, 60, 60, 50, 60, -9, 5, 7, -58, true, FIRST_GET_WEAPON );
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
	-- ���г��Ĭ��ѡ�е�һ��
	local nChkIndex = 0;
	-- TODO: �Ӹ������ж��Ƿ�������������������µ�ǿ��Ĭ����ʾ
	if not t_mainSelPetInfo["getNew"] then
		-- ������ʾ���ڳ����ĳ���
		for i = 1, petPackage:getPetNum() do
			if petPackage:isPetInState( i - 1, CLIENT_PET_STAT_FIGHT ) then
				nChkIndex = i;
				break;
			end
		end
	end
	
	-- Ȼ������ʾ�ϴα�ѡ�еĳ���
	if nChkIndex == 0 and t_mainSelPetInfo["id"] ~= 0 then
		nChkIndex = t_mainSelPetInfo["index"] + 1;
	end
	
	if Pet_DevelopmentBuildUpFrame:IsShown() or PetStepUpFrame:IsShown() then
		local t_operationSelPetInfo = GetOperationSelPetInfo();
		t_mainSelPetInfo	= t_operationSelPetInfo;
		nChkIndex		= t_mainSelPetInfo["index"] + 1;
	end
	-- ���Ĭ��ѡ�е�һֻ����
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

	-- ��ʾ�������ְ�ť
	local btn = getglobal( "PetFrame_NickNameBtn" );
	btn:Show();
	-- ���س������ֱ༭��
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
						["PetFrameListBackGroundNumTitleFont"]	= { value = "�����б�" .. petPackage:getPetNum().."/"..petPackage:getPetMaxNum() },
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
	-- ����һ�λ�ó���
	local xinShouBtn = getglobal( "PetFrameXinShouBtn" );
	if not GameWizard:isDo(GAMEWIZARD_FIRST_GETPETACTOR) then
		--xinShouBtn:SetPoint( "BOTTOMLEFT", "PetFrame_FightBtn", "TOPLEFT", 30, -50 );
		GameWizard:doStep(GAMEWIZARD_FIRST_GETPETACTOR);
		ShowPetFrameXinShouBtn( { text = "�������Ϳ����ٻ����������������ս", btn = "PetFrame_FightBtn" } );
		--�������Զ���ҩ�����ô�
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
		ShowPetFrameXinShouBtn( { text = "�ɵ������ظ������ҳ϶ȣ���Ҫ����ѱ���ǹ���", btn = "PetFrame_TameBtn" } );
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
	-- ���ô�������		  
	PetFrame:SetPoint("topleft", "$parent", "topleft", 32, 163 );

	local t_fonts = { ["PetFrameTitleFont"] = { value = "����" } };
	for name, data in pairs( t_fonts ) do 
		local font = getglobal( name );
		font:SetText( data.value );
	end

	local mainplayer = ActorMgr:getMainPlayer();
	local petPackage = mainplayer:getPet();

	-- t_mainSelPetInfo = { heightId = 0, lowId = 0, id = 0, index = -1 };

	PetFrame_CatchPetDesc:SetLinkTextColor( 0, 255, 0 );
	PetFrame_CatchPetDesc:SetText( "���ﲶ׽���ʺ絺  ͨ��#L����[ʱ���]#n����", 255, 255, 190 );
	PetFrame_TrainPetDesc:SetLinkTextColor( 0, 255, 0 );
	PetFrame_TrainPetDesc:SetText( "����ѵ��ʦ��#L���¶�[ʱ���]#n", 255, 255, 190 );

	local pageFont	= getglobal( "PetFrameListBackGroundPageFont" ); 
	
	-- Ĭ��ѡ�е�һ������
	if petPackage:getPetNum() == 0 then
		ClearPetFrameWithoutPets();
		--PetFrame:Hide();
		--ShowMidTips("�㻹δӵ�г��");
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

-- ��ճ���鿴��Ϣ
function ClearPetInfo( szFrameName )
	PetFrame_BackUpBtn:Hide();
	PetFrame_FightBtn:Show();
	--t_mainSelPetInfo = { heightId = 0, lowId = 0, id = 0, index = -1 };
	local t_fonts = {
						-- ��������
						szFrameName.."PetNameBtnFont",
						-- ����ȼ�
						szFrameName.."PetLvValueBtnFont",
						-- �����츳����
						szFrameName.."PetGeniusValuBtnFont",
						-- ����HP
						szFrameName.."HPBtnHPValueFont",
						-- ����EXP
						szFrameName.."EXPBtnExpValueFont",		
						-- ���﹥���ɳ�ֵ
						szFrameName.."AttackGrowthValueBtnFont",	
						-- ���������ɳ�ֵ
						szFrameName.."LifeGrowthValueBtnFont",	
						-- ��������ɳ�ֵ
						szFrameName.."PhysicDefGrowthValueBtnFont",
						-- ����ħ���ɳ�ֵ
						szFrameName.."IceDefGrowthValueBtnFont",
						-- �������гɳ�ֵ
						szFrameName.."HitGrowthValueBtnFont",	
						-- �������ܳɳ�ֵ
						szFrameName.."FleeGrowthValueBtnFont",
						-- ���﹥��ֵ
						szFrameName.."TotalAttackValueBtnFont",	
						-- ����ֵ
						szFrameName.."MaxLifeValueBtnFont",
						-- ����ħ������ֵ
						szFrameName.."IceDefValueBtnFont",	
						-- ��������ֵ
						szFrameName.."MaxLifeValueBtnFont",	
						-- �����������ֵ
						szFrameName.."PhyDefValueBtnFont",	
						-- ��������ֵ
						szFrameName.."HitValueBtnFont",	
						-- ��������ֵ
						szFrameName.."FleeValueBtnFont",	
						-- ���ﱩ����ֵ
						szFrameName.."HeaveyAttackValueBtnFont",	
						-- ��������ֵ
						szFrameName.."TiShenValueBtnFont",	
						-- �����ҳ϶�ֵ
						szFrameName.."ZhongChenValueBtnFont",
						-- ����ս����
						szFrameName.."FightValueBtnFont",
						-- �����ٻ��ȼ�
						szFrameName.."CallLvValueBtnFont",	
						-- �����ǳ�
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
	SetGameTooltips(this:GetName(), "Shift+�������ɷ��ͳ��������������")
end

function PetListButton_OnClick()
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local pageFont		= getglobal( "PetFrameListBackGroundPageFont" ); 
	local nPage		= tonumber( pageFont:GetText() )
	local nPetIndex		= ( nPage - 1 ) * MAX_PET_ONE_PAGE + this:GetClientID() - 1;
	local petInfo		= petPackage:getPetInfo( nPetIndex );  
	-- ��סshift
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

-- �������������б���Ϸ�ҳ
function PrePageBtn_OnClick()
	local pageFont	= getglobal( "PetFrameListBackGroundPageFont" ); 
	local nPage		= tonumber( pageFont:GetText() );
	
	if nPage <= 1 then
		return;
	end
	pageFont:SetText( nPage - 1 );
	UpdatePeFrame_PetList();
end

-- �������������б���·�ҳ
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

--@desc:�����ת��ťʱ����ת��Ԥ��ģ��
--@return: Nothing
function Pet_TurnLeftBtn_OnMouseDown()
	local ModelViewer = getglobal( this:GetParent().."ModelBtnModelView");
	if not ModelViewer:IsShown() then
		return;
	end
	
	ModelAngle = ModelAngle - 5;
	ModelViewer:setEntityRotate(0,ModelAngle);
end

--@desc:�������ת��ťʱ����ת��Ԥ��ģ��
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
				-- ����ǰ���õĳ��ＴΪ��ѡ���ĳ���
				if pet.WID.nHeight == t_mainSelPetInfo["heightId"] and pet.WID.nLow == t_mainSelPetInfo["lowId"] then
					btn:Checked();
				end
			end
		end
	end

	local t_fonts = {
						["PetFrameListBackGroundNumTitleFont"] = { value = "�����б�" .. petPackage:getPetNum().."/"..petPackage:getPetMaxNum() },
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
	-- �趨�������ٻذ�ť����ʾ���
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

PET_LEVEL_LIMIT		= 10;	-- /* �ȼ������� */
PWORLD_NO_PET_FIGHT = 8;	-- �����ڸ����в��ܳ������ֶ�
-- �����������
function CanCallPet( data )
	local nPetIndex		= data["petIndex"];
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local petInfo		= petPackage:getPetInfo( nPetIndex );
	if petPackage:isPre() then
		ShowMidTips( "���������ٻ��У��޷������µ��ٻ�" );
		return;
	end

	if mainplayer:isDead() then
		ShowMidTips( "���Ǵ�������״̬�������ٻ�����" );
		return;
	end

	if mainplayer:isInFly() then
		ShowMidTips( "���ڷ���״̬�������ٻ�����" );
		return;
	end
	
	if petInfo == nil then
		return
	end

	-- ���ж��������ڵ�ͼ�Ƿ��ܹ����
	local nRealMapID = MapShower:getRealMapId();
	if nRealMapID >= MIN_COPY_ID then
		local pworldDef = getPworldDef( nRealMapID / 100 );
		if bitAnd( pworldDef.CtrlFlag, PWORLD_NO_PET_FIGHT ) then
			ShowMidTips( "�ø��������ٻ�����" );
			return;
		end
	end
	
	local petDef	= getPetDef( petInfo.PetID );
	-- �жϳ����Ƿ���̯λ��
	if StallSaleFrame:IsShown() and GameStallManager:isInStallIPet( SHITYPE, nPetIndex ) then
		ShowMidTips( "�ó����Ѿ����ڰ�̯�������ϣ����ܽ����ٻ�" );
		return;
	end

	-- �жϳ����Ƿ��ڽ�������
	if TradeFrame:IsShown() and GameExchange:isInExchangePet( SHITYPE, nPetIndex ) then
		ShowMidTips( "�ó����Ѿ����ڽ������ϣ����ܽ����ٻ�" );
		return;
	end

	-- ����������ٻ��ȼ��������ٻ�����
	if mainplayer:getLv() < petDef.FightLevel then
		ShowMidTips( "�ٻ��ó�����Ҫ��ɫ�ȼ��ﵽ" .. petDef.FightLevel );
		return;
	end
	-- ������ȼ������Ǹ�10��������ﲻ�ܳ�ս
	if petInfo.Level - mainplayer:getLv() > PET_LEVEL_LIMIT then
		ShowMidTips( "����ȼ������10�������ܽ����ٻ�" );
		return;
	end

	-- �鿴������ҳ϶��Ƿ�ﵽ���������Ҫ��
	if petInfo.LoveVal < PET_FIGHT_LOVEVAL then
		ShowMidTips( "�����ҳ϶�̫�ͣ����ܱ��ٻ�����ʹ��ѱ���ǹ�����ѱ�ޱ��ӻָ�" );
		return;
	end

	return true;
end

function PetFrameFightBtn_OnClick()
	-- ����������״̬�����ܵ������
	local mainplayer = ActorMgr:getMainPlayer();
	local petPackage = mainplayer:getPet();
	if petPackage:isPre() then
		ShowMidTips( "���������ٻ��У��޷������µ��ٻ�" );
		return;
	end

	-- ���жϵ�ǰ�Ƿ�������״̬
	--[[
	if not CheckIntonateBar( "�ٻ��������" ) then
		return;
	end
	--]]

	if t_mainSelPetInfo.id == 0 then
		return;
	end
	
	if mainplayer:isDead() then
		ShowMidTips( "���Ǵ�������״̬�������ٻ�����" );
		return;
	end

	if mainplayer:isInFly() then
		ShowMidTips( "���ڷ���״̬�������ٻ�����" );
		return;
	end

	if GameExchange:isInExchangePetByWID( t_mainSelPetInfo.heightId, t_mainSelPetInfo.lowId ) then
		ShowMidTips( "�������ڽ����У��޷��ٻ�" );
		return;
	end

	-- ���ж��������ڵ�ͼ�Ƿ��ܹ����
	local nRealMapID = MapShower:getRealMapId();
	if nRealMapID >= MIN_COPY_ID then
		local pworldDef = getPworldDef( nRealMapID / 100 );
		if bitAnd( pworldDef.CtrlFlag, PWORLD_NO_PET_FIGHT ) then
			ShowMidTips( "�ø��������ٻ�����" );
			return;
		end
	end
	
	local petInfo	= petPackage:getPetInfo( t_mainSelPetInfo["index"] );
	local petDef	= getPetDef( petInfo.PetID );
	-- �жϳ����Ƿ���̯λ��
	if StallSaleFrame:IsShown() and GameStallManager:isInStallIPet( SHITYPE, t_mainSelPetInfo["index"] ) then
		ShowMidTips( "�ó����ϼ��У��޷������ٻ�" );
		return;
	end

	-- �жϳ����Ƿ��ڽ�������
	if TradeFrame:IsShown() and GameExchange:isInExchangePet( SHITYPE, t_mainSelPetInfo["index"] ) then
		ShowMidTips( "�ó����Ѿ����ڽ������ϣ����ܽ����ٻ�" );
		return;
	end

	-- ����������ٻ��ȼ��������ٻ�����
	if mainplayer:getLv() < petDef.FightLevel then
		ShowMidTips( "�ٻ��ó�����Ҫ��ɫ�ȼ��ﵽ" .. petDef.FightLevel );
		return;
	end
	
	-- ������ȼ������Ǹ�10��������ﲻ�ܳ�ս
	if petInfo.Level - mainplayer:getLv() > PET_LEVEL_LIMIT then
		ShowMidTips( "����ȼ������10�������ܽ����ٻ�" );
		return;
	end
	if petPackage:isExtraClanJiYangPet( petInfo ) then 
		ShowMidTips( "�������ڼ����У���ǰ���������ȡ�������ٽ��иò�����" );
		return;
	end
	-- �鿴������ҳ϶��Ƿ�ﵽ���������Ҫ��
	if petInfo.LoveVal < PET_FIGHT_LOVEVAL then
		ShowMidTips( "�����ҳ϶�̫�ͣ����ܱ��ٻ�����ʹ��ѱ���ǹ�����ѱ�ޱ��ӻָ�" );
		return;
	end

	-- �������ĳ����Ƿ������ں�
	local t_PetRongHeProcessFrameControl = GetPetRongHeProcessFrameControl();
	if t_PetRongHeProcessFrameControl:isAnyPetRongHe() then
		ShowMidTips( "���ڽ��г����ںϣ����ܽ����ٻ�" );
		return;
	end

	-- ���ٻ���ԭ�������Ѿ����ڳ���״̬�ĳ���
	petPackage:petPre( t_mainSelPetInfo.heightId, t_mainSelPetInfo.lowId );
end

-- �����ٻ�
function PetFrameBackUpBtn_OnClick()
	if t_mainSelPetInfo.id == 0 then
		return;
	end

	if PetRongHeProcessFrame:IsShown() then
		ShowMidTips( "���ڽ����ںϣ������ٻس���" );
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

-- ����ѱ��
function PetFrameTameBtn_OnClick()
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet( );
	if petPackage:isPre() then
		ShowMidTips( "���������ٻ��У��޷�ѱ��" );
		return;
	end

	if t_mainSelPetInfo.id == 0 then
		return;
	end

	local petInfo		= petPackage:getPetInfo( t_mainSelPetInfo["index"] );
	local nCurLoveVal	= math.floor( petInfo.LoveVal / 100 );
	--[[
	if nCurLoveVal == MAX_PET_LOVE_VAL / 100 then
		ShowMidTips( "�����ҳ϶��Ѵﵽ���ޣ��������ӳ�����ҳ϶�" );
		return;
	end
	--]]

	if petPackage:isPetInFreedom( t_mainSelPetInfo["heightId"], t_mainSelPetInfo["lowId"] ) then
		ShowMidTips( "�ó����Ѿ��������������ɽ��иò�����" );
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
		ShowMidTips("ȱ��ѱ���ǹ���ѱ�ޱ��ӣ��޷�ѱ�������������ҳ϶�");
		return;
	end
	
	container:usedItem( CONTAINER_TYPE_SUNDRIES, nGridIdx, 0, t_mainSelPetInfo["index"] );
end

-- �������
function PetFrameFreedomBtn_OnClick()
	local mainplayer	= ActorMgr:getMainPlayer();
	local container = mainplayer:getContainer();
	local lockState = container:getContainerLockState();
	if lockState == CONTAINER_LOCK_STATE_LOCK then
		ShowMidTips("������֤������ȫ��");
		return;
	end
	local petPackage	= mainplayer:getPet( );
	if petPackage:isPre() then
		ShowMidTips( "���������ٻ��У��޷�����" );
		return;
	end

	if GameExchange:isInExchangePetByWID( t_mainSelPetInfo.heightId, t_mainSelPetInfo.lowId ) then
		ShowMidTips( "�������ڽ����У����ܷ���" );
		return;
	end

	if t_mainSelPetInfo.id == 0 then
		return;
	end

	if petPackage:isPetInFreedom( t_mainSelPetInfo["heightId"], t_mainSelPetInfo["lowId"] ) then
		ShowMidTips( "�ó����Ѿ��������������ɽ��иò�����" );
		return;
	end

	if GameStallManager:isInStallIPet( SHITYPE, t_mainSelPetInfo.heightId, t_mainSelPetInfo.lowId ) then
		ShowMidTips( "�����ϼ��У����ܷ���" );
		return;
	end
	local petInfo		= petPackage:getPetInfo( t_mainSelPetInfo["index"] );
	if petInfo.PetGenius > 2 then
		return AddGameMiddleTips2( "�ܳ����������ʵĳ����޷�������" );
	end
	SetDropPetInfo( t_mainSelPetInfo );	
	DropPetConfirmFrame:Show();	
end

--@Desc:���տͻ��˷��͵�֪ͨ��ʼ�����������
--@Return:Nothing
function PetCampaign( nTime )
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then
		return;
	end

	IntonateBar_text:SetText("�����ٻ�����");
	IntonateBar:SetClientUserData( 0, PET_FAIRY_TYPE );
	IntonateBar:SetClientUserData( 2, nTime );
	IntonateBar:Show();
end

function Pet_HousecarlArmButton_OnEnter()
	local szText = "������δ���ţ������ڴ�";
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
		table.insert(t, string.format("����%s�˺�+%d", t_AttrTypeName[FairyStarDef.AttrType], FairyStarDef.Atk))
		table.insert(t, string.format("����%s", TraitDef.PetTraitDesc))
		table.insert(t, string.format("�����ȡ����+%s%%", FairyStarDef.PetExpMul))
		SetGameTooltips(this:GetName(), string.format("��ǰ�����ѱ�%s����\n%s\n����򿪾������", TrumpDef.Name, table.concat(t, "\n")))
	else
		SetGameTooltips(this:GetName(), "��ǰ���ٻ��ľ���\nֻ�г���������ܽ��ܾ���Ĺ���\n����򿪾������")
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
#cff6e1c��������#n
1����������ٻ��󷽿ɺ���
2�������ҳ϶ȱ������80%
3�����������ȼ�ͬʱ����60��
#cff6e1c����״̬#n
1�������ÿ��������һ���ҳ϶�
2���ҳ϶�С��50%����������״̬
3�����ݳ����ۺ�����������������
4������ֵӰ�������������ҵ���
���˺��ӳ�
#c00ff00�������ͼ����������ק�������#n]===])
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
	-- ������ק״̬ ͬʱ �Ҳ�������δ��ʾ ����ʾ�Ҳ�������
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
		return AddGameMiddleTips2("�ٻ��ĳ�����ܺ��壬�����ٻ��ó���");
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
		ShowMidTips( "�ó����Ѿ������������ɽ��иò�����" );
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
		--return ShowMidTips("��������̫����");
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

	-- �������м��ܣ�����ʾ����tips
	if diathesisDef ~= nil then
		local IconPath = getPetSkillPath()..diathesisDef.IconID..".tga";
		t_fonts = 
						{
						["PetSkillTipsFrameNameFont"]		= { value = diathesisDef.Name },-- ..diathesisDef.Level.."��"
						--["PetSkillTipsFrameSkillTypeFont"]	= { value = "���ͣ�"..t_skillGridTga[skillGridInfo.GridType].typeDesc },
						};
		
		t_tgas = 
						{
						["PetSkillTipsFrameTex"] = { path = ( IsInExistence( IconPath ) and IconPath or DEFAULT_ITEM_PATH ) },
						};
		
		t_riches = 
					{
					["PetSkillTipsFrameDescRich"] = { text = isGenius and diathesisDef.DiathesisDesc.."\n#G�����ٻ�״̬�¼����Ч��" or 
																diathesisDef.DiathesisDesc, 
													color = { r = 255, g = 255, b = 255 } },
					};
	-- ������ʾ����tips
	else
		t_fonts = 
						{
						["PetSkillGridTipsFrameNameFont"]		= { value = "���＼����λ" },
						--["PetSkillGridTipsFrameGridTypeFont"]	= { value = "���ͣ�"..t_skillGridTga[skillGridInfo.GridType].typeDesc },
						["PetSkillGridTipsFrameGridTypeFont"]	= { value = "����ѧϰ�¼���" },
						};
		
		t_tgas = 
						{
						-- path = t_skillGridTga[skillGridInfo.GridType].path
						["PetSkillGridTipsFrameTex"] = { path = t_skillGridTga[PET_SKILL_SELF_BUFF].path },
						};
		local t_skillGridTga = 
						{
						[PET_SKILL_ATK]			= "�ü��ܸ�Ϊ�����༼�ܸ񣬿�ѧϰ�����༼�ܣ���ʱ��ǳ���ѵ��ʦ���¶�������Ӧ���ͼ������ѧϰ",
						[PET_SKILL_OWNER_BUFF]	= "�ü��ܸ�Ϊ�����༼�ܸ񣬿�ѧϰ�����༼�ܣ���ʱ��ǳ���ѵ��ʦ���¶�������Ӧ���ͼ������ѧϰ",
						[PET_SKILL_SELF_BUFF]	= "�ü��ܸ�Ϊ��ǿ�༼�ܸ񣬿�ѧϰ��ǿ�༼�ܣ���ʱ��ǳ���ѵ��ʦ���¶�������Ӧ���ͼ������ѧϰ",
						};
		t_riches = 
					{
					["PetSkillGridTipsFrameDescRich"] = { text = "��ʱ��ǳ���ѵ��ʦ���¶�������Ӧ���ͼ������ѧϰ����", color = { r = 255, g = 255, b = 255 } },
					};
		
		if parentFrame:GetName() ~= "PetFrame" then
			return;
		end

		--[[local szText = "�Ҽ����ѧϰ����";
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
			ShowMidTips( "�ó����Ѿ������������ɽ��иò�����" );
			return;
		end

		local t_AddPetSkillControl = GetAddPetSkillControl();
		t_AddPetSkillControl:setOpPet( t_mainSelPetInfo );
		OpenPetSkillConfirmFrame:Show();
	elseif arg1 == "RightButton" then
		
		if petPackage:isPetInFreedom( t_mainSelPetInfo["heightId"], t_mainSelPetInfo["lowId"] ) then
			ShowMidTips( "�ó����Ѿ������������ɽ��иò�����" );
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
		--local szText = "δ����\n���ʽ����һ�������ںϳɶ���ʱ��100%���ʻ��\n�ر���ʾ��һ�����������50%�����ڷ���ʱ�����츳";
		local szText = "δ����\n���ʽ�����������50%�����ڷ���ʱ����츳";
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

		local szText = string.format("����ﵽ%d��ʱ���������һ������", levels[this:GetClientID()])
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

-- ��Ϊ���������ʱ��ˢ�³���
function ClearPetFrame()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local petPackage = mainplayer:getPet();
	
	local nOldSelectPetIndex = t_mainSelPetInfo["index"];
	t_mainSelPetInfo = { heightId = 0, lowId = 0, id = 0, index = -1 };

	-- Ĭ��ѡ�е�һ������
	if petPackage:getPetNum() == 0 then
		ClearPetFrameWithoutPets();
		UpdatePetPhotoWhenDropPet( { selectPetIndex = - 1, oldSelectPetIndex = nOldSelectPetIndex } );
		--PetFrame:Hide();
		--ShowMidTips("������û�г���Զ��رճ������");
		return;
	end

	local pageFont	= getglobal( "PetFrameListBackGroundPageFont" ); 
	local nPage		= tonumber( pageFont:GetText() );
	local petInfo;
	local nPetIndex	= -1;
	-- ����ǰҳ�����б��г������ʾ��ǰҳ��һ������
	if petPackage:getPetNum() > ( nPage - 1 ) * MAX_PET_ONE_PAGE then
		nPetIndex = ( nPage - 1 ) * MAX_PET_ONE_PAGE;
		petInfo = petPackage:getPetInfo( nPetIndex );	
	else
		pageFont:SetText( nPage - 1 );
		nPetIndex = ( nPage - 2 ) * MAX_PET_ONE_PAGE;
		petInfo = petPackage:getPetInfo( nPetIndex );	
	end

	-- �����ﱻ����������Ĭ��ˢ��������ʾ����ҳ�ĵ�һ������
	local btn	= getglobal( "PetFrame_ListButton1" );
	btn:Checked();
	t_mainSelPetInfo = { heightId = petInfo.WID.nHeight, lowId = petInfo.WID.nLow, id = petInfo.PetID, index = nPetIndex };
	
	if PetFrame:IsShown() then
		UpdatePeFrame_PetList();
		ShowPetInfo( petInfo, "PetFrame", true );
	end
	
	UpdatePetPhotoWhenDropPet( { selectPetIndex = nPetIndex, oldSelectPetIndex = nOldSelectPetIndex } );
end

UI_TYPE_PET_WASH_GENIUS		= 27;	-- /* ����û� */
UI_TYPE_PET_STRENGTHEN		= 28; 	-- /* ����ǿ�� */
UI_TYPE_PET_ADD_ATTR		= 29;	-- /* ������ǿ */
UI_TYPE_PET_SKILL_LEARN		= 30;	-- /* ����ѧϰ���� */
UI_TYPE_PET_SKILL_UP		= 31;	-- /* ������������ */
UI_TYPE_PET_ADD_SKILL_GRID	= 32;	-- /* ���＼�������� */
UI_TYPE_PET_WASH_SKILL_GRID = 33;	-- /* ���＼�������� */
UI_TYPE_PET_SKILL_RESET		= 65;	-- /* ���＼������ */

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
-- ���ڴ򿪳����������
local t_petOperationMutexFrame = { "Pet_SkillLearnFrame", };
function openPetOperationUI( nUIType, nNpcId )
	local mainplayer = ActorMgr:getMainPlayer();
	local petPackage = mainplayer:getPet();
	if petPackage:getPetNum() == 0 then
		ShowMidTips( "����û�г�����ܽ��иò���" );
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
	-- �û����
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
	-- ��ǿ���
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
	-- �ɳ�ǿ�����
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
	-- ����ѧϰ���
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
	-- �����������
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
	-- ���ܸ��������
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
	-- ���ܸ�������
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
	-- ����������
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
	-- TODO: ��ʱ���� -- ��ֳ���
	--["Pet_CloneFrame"] = {},
};

-- ��ѡ�н��в����ĳ���
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

-- ��ѡ�е�ʹ�õ���
local t_selUseItem	= { id = 0, selItem = nil, listType = 0, gridx = 0, num = 0 };
-- ��ѡ�еļ�����λ
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
	-- ������򿪵Ķ�ӦNPC��Ϣ
	for key, _ in pairs( t_openFrames ) do
		t_openFrames[key]["npcId"] = 0;
		-- �����е��߱�ѡ�������滻��ԭ���ĵ���
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

-- ͨ�ò�������
-- ��������������Ϲ�����ť
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
	ShowMidTips( "����ǿ���ɹ����������Ҫ��������" );
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
			ShowMidTips( "��ǰ������Ա�ԭ���Ըߣ����Զ�Ϊ������" )		
			Pet_DevelopmentBuildUpFrameSaveBtn_OnClick()
		end
	end
	ShowContinueStrthenBtn();
	UpdatePet_DevelopmentBuildUpFrameStars();
	if nIncrAttrVal == MAX_INCR_ATTR_PERCENT then
		local player = ActorMgr:getMainPlayer()
		local petMgr = player:getPet()
		local info = petMgr:getPetInfo(nPetHeighID, nPetLowID)
		if MessageBoxBlock("ǿ��ȷ��", string.format("#cFFFFBE����%s�Ѿ����һ������ǿ�����Ƿ񱣴棿", info.Name), Pet_DevelopmentBuildUpFrame, function() print(t_operationSelPetInfo["heightId"], t_operationSelPetInfo["lowId"], nPetHeighID, nPetLowID) return t_operationSelPetInfo["heightId"] == nPetHeighID and t_operationSelPetInfo["lowId"] == nPetLowID end) then
			Pet_DevelopmentBuildUpFrameSaveBtn_OnClick()
		end
	end
end

function PetStrengthenRes( nPetHeighID, nPetLowID, nIncrAttrVal, nStrthenLevel )
	coroutine.wrap(PetStrengthenWrap)(nPetHeighID, nPetLowID, nIncrAttrVal, nStrthenLevel)
end

-- ��������������¹�����ť
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

-- ����������Ļ�����������ʱ����Ӧ
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

-- ������干�õ���ս�����Ϣ�ķ���
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

-- ���ݲ�������ѡ���Ӧ��item resultֵ
-- ��Ϊ�ܽ����߷��ڶ�Ӧ��������ǰ������ 
local t_itemFrames = 
					{	
						-- �û� 
						["Pet_XiDianFrame"]				= { itemPre = { resultId = { RESULT_PET_WASH_GENIUS }, },	petPre = {} },
						-- ��ǿ����
						
						--[[["Pet_ZengQiangFrame"]			= { itemPre = { resultId = { RESULT_PET_ADD_ATTR, }, },		
															petPre = {} },
						--]]
						-- ǿ���ɳ�
						["Pet_DevelopmentBuildUpFrame"]	= { itemPre = { resultId = { RESULT_PET_STRENGTHEN_STUFF }, },petPre = {} },
						
						-- ����ѧϰ���� ��ע���������ܸ������ʱ��soneid1���ҵ���Ӧ��itemId
						["Pet_SkillLearnFrame"]			= { itemPre = { resultId = { RESULT_PET_SKILL_BOOK }, },	petPre = {} },
						
						-- �������
						["PetStepUpFrame"]				= { itemPre = { itemId = { PET_STEP_UP_USE_ID_TOLUA }, } ,	petPre = {} },
						
						-- ������������
						--[[["Pet_SkillLevelUpFrame"]			= { itemPre = { resultId = { RESULT_PET_SKILL_UP_STUFF }, }, petPre = { }, },
						--]]
						-- ���＼�ܸ�����
						--[[["Pet_SkillGridExtendFrame"]	= { itemPre = { resultId = { RESULT_ADD_SKILL_GRID }, },	petPre = {} },
						--]]
						-- ���＼�ܸ����,�������İ󶨵��ߣ�Ȼ���Ƿǰ󶨵���
						--[[["Pet_SkillGridRebuildFrame"]	= { itemPre = { resultId = { RESULT_PET_WASH_SKILL_GRID }, },petPre = {} },--]]
						
						-- ������
						["PetUnBindFrame"]				= { itemPre = { itemId = { PET_UNBIND_ITEM_ID_TOLUA }, } ,	petPre = {} },
					};

function GetPetItemFramesConfig()
	return t_itemFrames;
end

-- ������������߼��
t_itemFrames["PetStepUpFrame"].check = 
function ( itemDef )
	for _, resultId in ipairs( t_itemFrames["PetStepUpFrame"].itemPre.itemId ) do
		if resultId == itemDef.ItemID then
			return true;
		end
	end

	ShowMidTips( "�õ��߲������ڽ��ײ���,�뻻������" );
	return false;
end
-- ������������߼��
t_itemFrames["PetUnBindFrame"].check = 
function ( itemDef )
	for _, resultId in ipairs( t_itemFrames["PetUnBindFrame"].itemPre.itemId ) do
		if resultId == itemDef.ItemID then
			return true;
		end
	end

	ShowMidTips( "�õ��߲������ڽ�����,�뻻������" );
	return false;
end

-- �û������յ��ߵļ��
t_itemFrames["Pet_XiDianFrame"].check = 
function ( itemDef )
	for _, resultId in ipairs( t_itemFrames["Pet_XiDianFrame"].itemPre.resultId ) do
		if resultId == itemDef.Result[0].ResultID then
			return true;
		end
	end

	ShowMidTips( "�õ��߲������ڻû�����,�뻻������" );
	return false;
end
--[[
-- ������ǿ�����յ��ߵļ��
t_itemFrames["Pet_ZengQiangFrame"].check = 
function ( itemDef )
	for _, resultId in ipairs( t_itemFrames["Pet_ZengQiangFrame"].itemPre.resultId ) do
		if resultId == itemDef.Result[0].ResultID then
			return true;
		end
	end

	ShowMidTips( "�õ��߲���������ǿ��������,�뻻������" );
	return false;
end
--]]

-- ����ǿ�������յ��ߵļ��
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
		ShowMidTips( "û��ѡ��Ҫǿ�����Ǽ�" );
		return;
	end

	if GamePet:isPetStrenthenItem( nDestStrenthenLevel, itemDef.ItemID ) then
		return true;
	end

	local petStrengthenDef	= getPetStrengthenDef( nDestStrenthenLevel );
	if petStrengthenDef == nil then
		ShowMidTips( "�����Ѿ��ﵽ���ǿ���ȼ����޷�ǿ��" );
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
		ShowMidTips( "�õ����޷�ǿ������뻻��" );
	else
		if bIsStrengthenItem and nItemStrengthenLevel < nDestStrenthenLevel then
			ShowMidTips( "ǿ����ǰ������Ҫ���߼���ǿ������"..destItemDef.Name );
		elseif bIsStrengthenItem and nItemStrengthenLevel > nDestStrenthenLevel then
			ShowMidTips( "ǿ����ǰ������Ҫ���ͼ���ǿ������"..destItemDef.Name );
		else
			ShowMidTips( "ǿ����ǰ������Ҫǿ������"..destItemDef.Name );
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

-- ����ѧϰ���� ��ע���������ܸ������ʱ��soneid1���ҵ���Ӧ��itemId
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

			ShowMidTips( "ѡ������û�ж�Ӧ���ͼ��ܸ���" );
			return false
			--]]

			return true;
		end
	end

	ShowMidTips( "�õ��߲���ѧϰ����,�뻻������" );
	return false;
end

--[[
-- ������������
t_itemFrames["Pet_SkillLevelUpFrame"].check = 
function ( itemDef )
	for _, resultId in ipairs( t_itemFrames["Pet_SkillLevelUpFrame"].itemPre.resultId ) do
		if resultId == itemDef.Result[0].ResultID then
			return true;
		end
	end
	
	ShowMidTips( "�õ��߲���������������,�뻻������" );
	return false;
end

-- ���＼�ܸ�����
t_itemFrames["Pet_SkillGridExtendFrame"].check = 
function ( itemDef )
	for _, resultId in ipairs( t_itemFrames["Pet_SkillGridExtendFrame"].itemPre.resultId ) do
		if resultId == itemDef.Result[0].ResultID then
			return true;
		end
	end

	ShowMidTips( "�õ��߲����������似�ܸ�,�뻻������" );
	return false;
end

-- ���＼�ܸ����,�������İ󶨵��ߣ�Ȼ���Ƿǰ󶨵���
t_itemFrames["Pet_SkillGridRebuildFrame"].check = 
function ( itemDef )
	if ResetExistSkillConfirmFrame:IsShown() then
		ShowMidTips( "�������ȷ�ϲ���" );
		return false;
	end

	for _, resultId in ipairs( t_itemFrames["Pet_SkillGridRebuildFrame"].itemPre.resultId ) do
		if resultId == itemDef.Result[0].ResultID then
			return true;
		end
	end

	ShowMidTips( "�õ��߲������ڸ��켼�ܸ�,�뻻������" );
	return false;
end
--]]

-- ������������ĵ��߱仯��ʱ����صı仯
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
	

	-- �жϳ����״̬
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
	-- �Ƿ��ǵ�һ�β���ǿ������
	-- �ȼ�鱳��	Quest:isQuestRun( 3134 )
	-- ʹ���˳���ǿ����Ҫ��������Ӧ����Ч����
	-- TODO: ��߻�ȷ���Ƿ�Ҫ������ID 
	if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_CONTAINER_STRENGTHEN_ITEM_BTN ) and 
		GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_NPCTALK_STRENGTHEN_PET_OPTION ) then
		
		local effectFrame = getglobal( "PlayerContainerFrameEffectFrame" );
		effectFrame:Hide();
		GameWizard:doStep( GAMEWIZARD_FIRST_BLINK_CONTAINER_STRENGTHEN_ITEM_BTN );
		ShowPetDevelopmentBuildUpFrameXinShouGuide();
	end
end

-- ѧϰ���ĵ��߷����仯��ʱ��
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
	
	
	-- �Ƿ��ǵ�һ�β���ѧϰ���＼��
	-- �ȼ�鱳��	Quest:isQuestRun( 3134 )
	-- ʹ���˳��＼��ѧϰ���Ҫ��������Ӧ����Ч����
	-- TODO: ��߻�ȷ���Ƿ�Ҫ������ID 
	if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_CONTAINER_LEARN_PET_SKILL_BOOK_BTN ) and 
		GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_NPCTALK_LEARN_PET_SKILL_OPTION ) then
		local effectFrame = getglobal( "PlayerContainerFrameEffectFrame" );
		effectFrame:Hide();
		GameWizard:doStep( GAMEWIZARD_FIRST_BLINK_CONTAINER_LEARN_PET_SKILL_BOOK_BTN );
		ShowPetSkillFrameXinShouGuide();
	end
end
-- ������������ĵ��߱仯��ʱ����صı仯
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
-- ������������ĵ��߱仯��ʱ����صı仯
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

-- ��յ���ͼ��
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
	-- ���߸�����ı���߶�Ӧ����ʾ����
	if t_itemFrames[szFrameName].changeItem ~= nil then
		t_itemFrames[szFrameName].changeItem();
	end
end

-- ��ҷ����ͼ��
function Pet_OperationFrame_ItemButton_Drag()
	-- ��û�������ܽ�����ҷ����
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

	-- ������Ʒ���Ǹ������õģ���Ҳ�����գ�����������ʾ
	if itemdef.UseTarget ~= USE_TARGET_PET then
		if szFrameName == "Pet_DevelopmentBuildUpFrame" then
			ShowMidTips( "����Ʒ���ܽ��г���ǿ���������뻻������" );
		elseif szFrameName == "PetStepUpFrame" then 
			ShowMidTips( "����Ʒ���ܽ��г�����ײ������뻻������" );
		elseif szFrameName == "PetImpUpFrame" then 
			ShowMidTips( "����Ʒ���ܽ��г�������������뻻������" );
		elseif szFrameName == "PetUnBindFrame" then
			ShowMidTips( "����Ʒ���ܽ��г�����������뻻������" );
		else
			ShowMidTips( "����Ʒ���ﲻ��ʹ�ã��뻻������" );
		end
		
		return;
	end

	if t_operationSelPetInfo["id"] == 0 then
		ShowMidTips( "û��ѡ��������ܷ���Ʒ" );
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

	-- �����е��߱�ѡ�������滻��ԭ���ĵ���
	if t_selUseItem["id"] ~= 0 then
		ClearPet_OperationItem();
	end

	-- ����ͼ��
	local tex = getglobal( szFrameName .. "_IconBtnIcon" );
	local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
	tex:SetTexture( ( IsInExistence( IconPath ) and IconPath or DEFAULT_ITEM_PATH ) );

	tex:SetTexRelUV( 1, 1 );
	t_selUseItem = { selItem = item, id = item:getItemId(), listType = item:getListType(), gridx = item:getGridIdx() };

	LockItem( item:getListType(), item:getGridIdx(), item:getItemId(), szFrameName );

	-- ���߸�����ı���߶�Ӧ����ʾ����
	if t_itemFrames[szFrameName].changeItem ~= nil then
		t_itemFrames[szFrameName].changeItem();
	end
end

-- ���յ���ͼ��
function Pet_OperationFrame_ItemButton_ReceiveDrag()
	if t_operationSelPetInfo["id"] == 0 then
		ShowMidTips( "û��ѡ��������ܷ���Ʒ" );
		return;
	end

	-- �Ƿ�ֻ�ܴӱ����Ϲ���
	local nSrcListType	= UIMSG:getUIGV().DragFrom;	
	local nSrcGrid		= UIGetDragUserData1();
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local item			= container:getItem( nSrcListType, nSrcGrid );
	
	Save2Pet_OperationFrame( item, nSrcListType, nSrcGrid, this:GetParent() );
	UIEndDrag();
	UIMSG:getUIGV().DragFrom = DRAG_NONE;
end

-- ����������ͼ��
function Pet_OperationFrame_ItemButton_OnEnter()
	if t_selUseItem["id"] == 0 then
		SetGameTooltips( this:GetName(), "���������ʵ" );
		return;
	end

	ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, getItemDef( t_selUseItem["id"] ) );
end

-- ����뿪����ͼ��
function Pet_OperationFrame_ItemButton_OnLeave()
	NormalItemTipsFrame:Hide();
end

local t_changePetItemPreCondition = { ["Pet_SkillGridRebuildFrame"] = {}, };
t_changePetItemPreCondition["Pet_SkillGridRebuildFrame"].func =
function ( data )
	if ResetExistSkillConfirmFrame:IsShown() then
		ShowMidTips( "�������ȷ�ϲ���" );
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

-- �������������ť
function Pet_DevelopmentBuildUpFrame_ChangeCaiLiaoBtn_OnClick()
	if t_selUseItem["id"] == 0 then
		return;
	end
	
	local font = getglobal( "Pet_DevelopmentBuildUpFrame_ChangeCaiLiaoBtnFont" );
	font:Hide();
end

function Pet_DevelopmentBuildUpFrame_ChangeCaiLiaoBtn_OnEnter()
	--[[
	local szText = "����������ǿ�������������������Ϊ10";
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

-- ǿ�������������ٰ�ť
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

-- ǿ�������������Ӱ�ť
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

-- ��ճ����б�
function ClearOperationPetList( szFrameName )
	for i = 1, MAX_PET_NUM do
		local btn		= getglobal( szFrameName.."_PetNameBtn"..i );
		btn:SetText( "" );
		btn:DisChecked();
		btn:Disable();
	end
end

-- ˢ�³����б�
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
	-- ѡ��ԭ���ı�ѡ���ĳ����б�ť
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
		-- ��鵱ǰ��ʾ�ĳ����Ƿ��Ǳ�ѡ���ĳ���
		if t_operationSelPetInfo["heightId"] == petInfo.WID.nHeight and t_operationSelPetInfo["lowId"] == petInfo.WID.nLow then
			btn:Checked();
		end
	end
	
	-- ���ѡ���ĳ����Ƿ���
	for i = 1, petPackage:getPetNum() do
		local petInfo = petPackage:getPetInfo( i - 1 );	
		if t_operationSelPetInfo["heightId"] == petInfo.WID.nHeight and t_operationSelPetInfo["lowId"] == petInfo.WID.nLow then
			t_operationSelPetInfo["index"] = i - 1;
			return;
		end
	end
	-- ִ�е���˵��ԭ����ѡ���ĳ����Ѿ�����,���ѡ���ĳ�����Ϣ
	t_operationSelPetInfo = { heightId = 0, lowId = 0, id = 0, index = -1 };
	if petPackage:getPetNum() == 0 then
		return;
	end

	-- �����Ǳ��������г����ѡ����ǰ���ĵ�һֻ����
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

-- �������������ĳ����б����ְ�ť
local t_changePetPreCondition = { ["Pet_SkillGridRebuildFrame"] = {}, };
t_changePetPreCondition["Pet_SkillGridRebuildFrame"].func =
function ()
	if ResetExistSkillConfirmFrame:IsShown() then
		ShowMidTips( "�������ȷ�ϲ���" );
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
	-- ���ù�����
	SetScrollBar( szFrameName .. "_ScrollBar", MAX_PET_NUM, petPackage:getPetNum() );
	-- ��û�������������ʾ������
	if petPackage:getPetNum() == 0 then
		return;
	end

	-- ˢ�³����б�
	UpdateOperationPetList( szFrameName );
	-- ���г��Ĭ��ѡ�е�һ��
	-- TODO: ����clientIDѡȡ������ܻ������index
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


-- ���¼�����λ��ʾ
function UpdatePetOperationSkillGrids( szFrameName, petInfo )
	-- ���ó������еļ���
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

	-- Ĭ��ѡ�е�һ������
	--local btn = getglobal( szFrameName.."_SkillButton1" );
	--btn:Checked();
	--t_selSkill	= { id = 0, lv = 0, gridx = 0, };
end


-- ��Ϊ�ı�ѡ���ĳ���ʱ��ı���ʾ�ĳ�����Ϣ����
local t_petInfoShow = 
						{
							["Pet_XiDianFrame"]		= { },	["Pet_ZengQiangFrame"]	= { },		["Pet_DevelopmentBuildUpFrame"] = { },
							["Pet_SkillLearnFrame"] = { },	["Pet_SkillLevelUpFrame"]	= { },	["Pet_SkillGridExtendFrame"]	= { },
							["Pet_SkillGridRebuildFrame"] = { },["PetStepUpFrame"] = { },["PetImpUpFrame"] = {},["PetUnBindFrame"] = {},
						};
-- �û����
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
															
			["Pet_XiDianFrame_PetListBackGroundHeadFont"] = { value = "�����б�"..petPackage:getPetNum().."/".. petPackage:getPetMaxNum() },
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

-- ��ǿ���
t_petInfoShow["Pet_ZengQiangFrame"].func =
function ( petInfo )
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local petDef		= getPetDef( petInfo.PetID );
	local t_fonts = 
		{
			["Pet_ZengQiangFrame_PetNameFont"] = { value = ( petInfo.Name ~= "" and petInfo.Name or petDef.Name )..( petInfo.StrengthenLevel > 0 
															and " ( +"..petInfo.StrengthenLevel.." )" or "" )  },
			["Pet_ZengQiangFrame_PetListBackGroundHeadFont"] = { value = "�����б�"..petPackage:getPetNum().."/".. petPackage:getPetMaxNum() },
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

-- ǿ�����
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
			["Pet_DevelopmentBuildUpFrame_PetListBackGroundHeadFont"] = { value = "�����б�"..petPackage:getPetNum().."/".. 
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

-- ����ѧϰ
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
			["Pet_SkillLearnFrame_PetListBackGroundHeadFont"] = { value = "�����б�"..petPackage:getPetNum().."/".. petPackage:getPetMaxNum() },
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

-- �������
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
			["PetStepUpFrame_PetListBackGroundHeadFont"] = { value = "�����б�"..petPackage:getPetNum().."/".. petPackage:getPetMaxNum() },
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
-- ������
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
			["PetUnBindFrame_PetListBackGroundHeadFont"] = { value = "�����б�"..petPackage:getPetNum().."/".. petPackage:getPetMaxNum() },
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
-- ����ɳ�����
t_petInfoShow["PetImpUpFrame"].func =
function ( petInfo )
	UpdatePetImpUpFrame( 1 );  -- �����鿴ԭ����
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
	--AdjustPetSkillFrameXinShouBtn( { btn = "Pet_SkillLearnFrame_SkillButton1", text = "���Գ��＼��ѧϰ��ť" } );

	AdjustPetSkillFrameXinShouBtn( { btn = "Pet_SkillLearnFrame_StudyBtn", text = "����ó���ѧϰ����" } );
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
	-- TODO: �����Ҫ�߻��������＼��ѧϰ������ID	and Quest:isQuestRun( 1062 )
	local mainplayer			= ActorMgr:getMainPlayer();
	local petPackage			= mainplayer:getPet();
	local t_containerControl	= GetContainerControl();
	if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_NPCTALK_LEARN_PET_SKILL_OPTION ) and mainplayer:getLv() >= 30 and 
		t_containerControl:isHavePetSkillLearnItem() and petPackage:getPetNum() > 0 then
		GameWizard:doStep( GAMEWIZARD_FIRST_BLINK_NPCTALK_LEARN_PET_SKILL_OPTION );
	end
	
	local mainplayer	= ActorMgr:getMainPlayer();
	-- �жϳ����״̬
	local petPackage	= mainplayer:getPet();
	local petInfo		= petPackage:getPetInfo( t_operationSelPetInfo["index"] );
	-- ��ʾ���ܸ�������ָ������Ҫ�и�ǰ�ᣬ��˸�ĸ��ӱ��������ļ�����ƥ�䣬��û�ҵ�ƥ�����ʹ�õ�һ��Ĭ�ϼ��ܸ���
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
		AdjustPetSkillFrameXinShouBtn( { btn = selectSkillGridBtn:GetName(), text = "�������ѧϰ�ļ��ܸ�" } );
	end
	--]]

	-- ��ʾѧϰ���ܰ�ť������ָ��
	if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_PETSKILLFRAME_LEARN_SKILL_BTN ) and 
			GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_CONTAINER_LEARN_PET_SKILL_BOOK_BTN ) then
		AdjustPetSkillFrameXinShouBtn( { btn = "Pet_SkillLearnFrame_StudyBtn", text = "����ó���ѧϰ����" } );
	end
end

function ShowPetDevelopmentBuildUpFrameXinShouGuide()
	-- ����ǿ������ָ��
	-- TODO: �����Ҫ�߻��������＼��ѧϰ������ID	and Quest:isQuestRun( 1062 )
	local mainplayer			= ActorMgr:getMainPlayer();
	local petPackage			= mainplayer:getPet();
	local t_containerControl	= GetContainerControl();
	if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_NPCTALK_STRENGTHEN_PET_OPTION ) and mainplayer:getLv() >= 30 and 
			t_containerControl:isHavePetDevelopmentBuildUpItem() and petPackage:getPetNum() > 0 then
		GameWizard:doStep( GAMEWIZARD_FIRST_BLINK_NPCTALK_STRENGTHEN_PET_OPTION );
	end

	-- ��ʾ����ǿ����ť������ָ��
	if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_PESTRENGTHENFRAME_STRENGTHEN_BTN ) and 
			GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_CONTAINER_STRENGTHEN_ITEM_BTN ) then
		AdjustPetDevelopmentBuildUpFrameXinShouBtn( { btn = "Pet_DevelopmentBuildUpFrame_QiangHuaBtn", text = "������г���ǿ��" } );
	end
end

local t_petDevelopmentBuildUpXinShouBtnPosInfo = {	["Pet_DevelopmentBuildUpFrame_QiangHuaBtn"] = 
											{ selfPoint = "bottomleft", relFrame = "Pet_DevelopmentBuildUpFrame_QiangHuaBtn", relPoint = "TopLeft", 
														offsetX = 8, offsetY = -20, 
											},
												};

function Pet_DevelopmentBuildUpFrame_Test_OnShow()
	AdjustPetDevelopmentBuildUpFrameXinShouBtn( { btn = "Pet_DevelopmentBuildUpFrame_QiangHuaBtn", text = "���Գ���ǿ����ť" } );
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

	-- ���г��ﱻѡ��������ʾ��Ӧ����Ϣ
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
			szLevelUpSkillText	= diathesisiDef ~= nil and diathesisiDef.Learn.MoneyVal or "0(�Ѿ��ﵽ��߼�)" ;
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

-- ��������
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
			szLevelUpSkillText = diathesisiDef ~= nil and diathesisiDef.Learn.MoneyVal or "0(�Ѿ��ﵽ��߼�)" ;
		end	
	else
		szLevelUpSkillText = "";
	end

	local t_fonts = 
		{
			["Pet_SkillLevelUpFrame_PetNameFont"] = { value = ( petInfo.Name ~= "" and petInfo.Name or petDef.Name )..( petInfo.StrengthenLevel > 0 
																and " ( +"..petInfo.StrengthenLevel.." )" or "" ) },
			["Pet_SkillLevelUpFrame_PetListBackGroundHeadFont"] = { value = "�����б�"..petPackage:getPetNum().."/".. petPackage:getPetMaxNum() },
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

-- ���＼�ܸ�����
t_petInfoShow["Pet_SkillGridExtendFrame"].func =
function ( petInfo ) 
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local petDef		= getPetDef( petInfo.PetID );
	local t_fonts = 
		{
			["Pet_SkillGridExtendFrame_PetNameFont"] = { value = ( petInfo.Name ~= "" and petInfo.Name or petDef.Name )..
																	( petInfo.StrengthenLevel > 0 and " ( +"..petInfo.StrengthenLevel.." )" or "" ) },
			["Pet_SkillGridExtendFrame_PetListBackGroundHeadFont"] = { value = "�����б�"..petPackage:getPetNum().."/".. petPackage:getPetMaxNum() },
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

-- ���＼�ܸ����
t_petInfoShow["Pet_SkillGridRebuildFrame"].func =
function ( petInfo )
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local petDef = getPetDef( petInfo.PetID );
	local t_fonts = 
		{
			["Pet_SkillGridRebuildFrame_PetNameFont"] = { value = ( petInfo.Name ~= "" and petInfo.Name or petDef.Name )..
																	( petInfo.StrengthenLevel > 0 and " ( +"..petInfo.StrengthenLevel.." )" or "" ) },
			["Pet_SkillGridRebuildFrame_PetListBackGroundHeadFont"] = { value = "�����б�"..petPackage:getPetNum().."/".. petPackage:getPetMaxNum() },
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

-- ��Ϊ��¼����Ƶ��鿴˵��ͼ��ʱ��Ҫ��ʾ������
local t_seeIntroContent = 
						{
						["Pet_XiDianFrame"] = { desc = "����û�˵����\n1.	����û���ȼ��ص�һ������ѧ�Ἴ����ʧ����ǿ���ȼ���ʧ������ǿ��������ʧ��\n2.����û��󣬳ɳ�ֵ�����ʺ��Ը񶼻��������������\n3.ʹ����Ч����ʯ������û������������\n4.ʹ��ǿЧ����ʯ������û����������Ϊ���㡣\n5.ʹ��Խ�ߵȼ�������ʯ�������õĳɳ�ֵ�л�����ߡ�" },
						};

-- ��Ϊ��������ѡ���ĳ���ı���ʾ�ĳ�����Ϣ����
function SetPetInfoShow( szFrameName )
	if t_operationSelPetInfo["id"] == 0 then
		ClearPet_OperationFrame( szFrameName );
		return;
	end
	-- ���г��ﱻѡ��������ʾ��Ӧ����Ϣ
	local idx = t_operationSelPetInfo["index"];
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local petInfo		= petPackage:getPetInfo(idx);	
	t_operationSelPetInfo = { heightId = petInfo.WID.nHeight, lowId = petInfo.WID.nLow, id = petInfo.PetID, index = idx };
	t_petInfoShow[szFrameName].func( petInfo );
end

local t_operationTips = {
						-- �û����
						["Pet_XiDianFrame"] = { text = "����û�˵����\n1.	����û���ȼ��ص�һ����ѧ��ļ��ܺͳ���ǿ���ȼ�����ı�\n2.	����û������ʻ�ı䣬���ǲ��ή��\n3.ʹ�ûû���ʵ���г���û�" },
						-- ��ǿ���
						["Pet_ZengQiangFrame"] = { text = "������ǿ˵����\n1.ÿ����ǿ��Ҫ����һ��������ǿ����\n2.ÿ�������������ǿ���" },
						-- ����ɳ�ǿ��
						["Pet_DevelopmentBuildUpFrame"] = { text = "����ǿ��˵����\n1.����û���ȼ��ص�һ����ѧ��ļ��ܺͳ���ǿ���ȼ�����ı�\n2.����û������ʻ�ı䣬���ǲ��ή��\n3.ʹ�ûû�ʯ���г���û�" },
						-- ���＼��ѧϰ
						["Pet_SkillLearnFrame"] = { text = "���＼��ѧϰ˵����\n1.ѧϰ���＼��ǰ�����Ҽ�������＼����λ�������ܸ�\n2.ͨ�����＼����ѧϰ���＼��\3.�����ͬʱӵ��5�����ܣ�����ѧϰ�ļ����м��ʶ���ǰ����ѧ����" },
						-- ���＼������
						["Pet_SkillLevelUpFrame"] = { text = "���＼������˵����\n1.���ܵȼ����Ϊ�ż�\n2.ÿ��������Ҫ����һ�����＼����������\n3.�ͼ�����ʯ���������ͼ����ܣ��߼�����ʯ���������߼�����" },
						-- ���＼�ܸ�����
						["Pet_SkillGridExtendFrame"] = { text = "���＼������˵����\n1.���似�ܸ���Ҫ���ģ�����֮ʯ\n2.���似�ܸ�����ѡ���ܸ�" },
						-- ���＼�ܸ����
						["Pet_SkillGridRebuildFrame"] = { text = "���＼�ܸ����˵����\n1.���ܸ����͸�����Ҫ���ģ�����֮ʯ\n2.���ܸ����󣬸ü��ܸ����ͽ��������\n3.ͬ�༼�ܸ�����Ϊ3��\n4.�������м��ܵļ��ܸ�ʱ���ü��ܽ�������" },
						};
-- ��Ϊ��ʾ����������Ĳ鿴˵����tips
-- ��Ϊ�ı���߱仯ʱ��ı���Ӧ����ʾ
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
	-- ���߸�����ı���߶�Ӧ����ʾ����
	if t_itemFrames[szFrameName].changeItem ~= nil then
		t_itemFrames[szFrameName].changeItem();
	end

	-- �ټ���Ƿ�����Ӧ��ǿ������
	-- ����Ʒ����������ԭ��ѡ������������ˢ�±仯����
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
	-- ����ͼ��
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

	-- �������ȫ�ֱ�������
	t_skillGridTga.__index = function ( tab, key )
								return { path = [[uires\TuPianLei\ChongWuJiNengLeiXing\1.tga]] };
							end
	setmetatable( t_skillGridTga, t_skillGridTga );

	-- �������ȫ�ֱ�������
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
		ShowMidTips( "����û������ܽ��иò������Զ��ر����" );
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

	-- �鿴�Ƿ�Ҫ��������ʹ�õ���Ʒ
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
	-- ��ʱ���ú���
function Pet_OperationFrame_OnShow()
	t_selUseItem			= { id = 0, selItem = nil, listType = 0, gridx = 0, num = 0 };
	t_operationSelPetInfo	= { heightId = 0, lowId = 0, id = 0, index = -1 };
	t_selSkill				= { id = 0, lv = 0, gridx = -1, };
	DefaultShow( this:GetName() );
	SetPetInfoShow( this:GetName() );

	-- ���������û��ʾ�������ų���������һ����ʾ
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

-- ����ǿ�������ʾ
-- ����Ƿ�Ҫ�Զ�����ǿ�����ߵ�ǿ�����
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

	-- ����ͼ��
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

	-- ���߸�����ı���߶�Ӧ����ʾ����
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

	-- ���������û��ʾ�������ų���������һ����ʾ
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
		--print( "ChekPet_OperationFrame_XinShou_Guide--������ǿ������Ƿ�Ҫ�����Զ�����" );
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
						[CLIENT_PET_STAT_FIGHT] = "���ĳ��ﴦ���ٻ�״̬��", [CLIENT_PET_STAT_DEAD] = "���ĳ��ﴦ������״̬��",
					};
-- ��Ϊ����û���ť��ʱ����Ӧ
function Pet_XiDianFrame_XiDianBtn_OnClick()
	-- ��û�г��ﱻѡ��������Ӧ
	if t_operationSelPetInfo["id"] == 0 then
		ShowMidTips( "����ѡ��һֻ����" );
		return;
	end

	if t_selUseItem["id"] == 0  then
		ShowMidTips( "��������û�����" );
		return;
	end

	local mainplayer = ActorMgr:getMainPlayer();
	if getPlayerMoney( mainplayer ) < PET_WASH_GENIUS_MONEY then
		ShowMidTips( ""..MONEY_TYPE_BULL_NAME.."ֵ����" );
		return;
	end

	-- �жϳ����״̬
	local petPackage = mainplayer:getPet();
	for nState, szDesc in pairs( t_cannotOperateStateDesc ) do
		if petPackage:isPetInState( t_operationSelPetInfo["index"], nState ) then
			ShowMidTips( t_cannotOperateStateDesc[nState].."�޷����лû�����" );
			return;
		end
	end

	if GameStallManager:isInStallIPet( SHITYPE, t_operationSelPetInfo["heightId"], t_operationSelPetInfo["lowId"] ) then
		ShowMidTips( "�������ڰ�̯��λ�У��޷����лû�����" );
		return;
	end

	if petPackage:isPetInFreedom( t_operationSelPetInfo["heightId"], t_operationSelPetInfo["lowId"] ) then
		ShowMidTips( "�ó����Ѿ������������޷����иò�����" );
		return;
	end
	
	local container	= mainplayer:getContainer();
	--local useItem	= container:getItem( t_selUseItem["id"] );
	local useItem	= t_selUseItem["selItem"];
	local petInfo	= petPackage:getPetInfo( t_operationSelPetInfo["index"] );
	local petInfo	= petPackage:getPetInfo( t_operationSelPetInfo["index"] );
	if petInfo.Hierarchy == 2 then
		ShowMidTips( "�������ﲻ�ܽ��лû�����" );
		return;
	end

	local itemDef	= getItemDef( useItem:getItemId() );
	-- �������Ǹ߼�����
	--[[if itemDef.Result[0].ResultVal1 == 1 and petInfo.PetGenius < PET_GENIUS_B then
		ShowMidTips( "��������̫�ͣ�����ʹ�ø߼����ߣ��뻻�ͼ�����" );
		return;
	end
	--]]
	
	--local nPetIndex = petPackage:getPetIndex( petInfo );
	MessageBox("�û�����","�û��󣬽��л����ø��õ����ʣ����ȼ�����Ϊ1��");
	MessageBoxFrame:SetClientUserData(0,useItem:getListType());
	MessageBoxFrame:SetClientUserData(1,useItem:getGridIdx());
	MessageBoxFrame:SetClientUserData(2,t_operationSelPetInfo["index"]);
	--container:send_UsedExpendableItem( useItem:getListType(), useItem:getGridIdx(), t_operationSelPetInfo["index"] );
end

-- ��Ϊ�����ǿ��ť��ʱ����Ӧ
function Pet_ZengQiangFrame_ZengQiang_OnClick()
	if t_operationSelPetInfo["id"] == 0 then
		ShowMidTips( "����ѡ��һֻ����" );
		return;
	end

	if t_selUseItem["id"] == 0  then
		ShowMidTips( "����������ǿ����" );
		return;
	end

	local mainplayer = ActorMgr:getMainPlayer();
	-- �жϳ����״̬
	local petPackage = mainplayer:getPet();
	for nState, szDesc in pairs( t_cannotOperateStateDesc ) do
		if petPackage:isPetInState( t_operationSelPetInfo["index"], nState ) then
			ShowMidTips( t_cannotOperateStateDesc[nState].."���ܽ�����ǿ����" );
			return;
		end
	end
	
	local petInfo	= petPackage:getPetInfo( t_operationSelPetInfo["index"] );
	local petDef	= getPetDef( t_operationSelPetInfo["id"] );
	local t_attrIncr = 
	{
		-- ħ������
		[0] = { curValue = petInfo.IceAtkMul,		maxValue = petDef.MaxIceAtkMul,		desc = "ħ������"	},
		-- ������
		[1] = { curValue = petInfo.PhsicAtkMul,		maxValue = petDef.MaxPhsicAtkMul,	desc = "������"	},
		-- ħ������
		[2] = { curValue = petInfo.IceDefMul,		maxValue = petDef.MaxIceDefMul,		desc = "ħ������"	},
		-- �������
		[3] = { curValue = petInfo.PhsicDefMul,		maxValue = petDef.MaxPhsicDefMul,	desc = "�������"	},
		-- ��Ѫ
		[4] = { curValue = petInfo.HPMul,			maxValue = petDef.MaxHPMul,			desc = "��Ѫ"		},
		-- ����
		[5] = { curValue = petInfo.FleeMul,			maxValue = petDef.MaxFleeMul,		desc = "����"		},
		-- ����
		[6] = { curValue = petInfo.HitRateMul,		maxValue = petDef.MaxHitRateMul,	desc = "����"		},
	};

	local container	= mainplayer:getContainer();

	local useItem	= t_selUseItem["selItem"];
	local itemDef	= getItemDef( useItem:getItemId() );
	local attrMap	= t_attrIncr[itemDef.Result[0].ResultVal1];
	if attrMap ~= nil and attrMap.curValue >= attrMap.maxValue then
		ShowMidTips( attrMap.desc.."���Ӵ����Ѵﵽ���ޣ����ܼ�������" );
		return;
	end

	--local nPetIndex = petPackage:getPetIndex( petInfo );
	container:send_UsedExpendableItem( useItem:getListType(), useItem:getGridIdx(), t_operationSelPetInfo["index"] );
end

function getBuildUpItem( nResultId, nResultVal1 )
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	-- TODO: �ĳ�����ʹ�ð󶨵���Ʒ
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

-- ��Ϊ���ǿ����ť��ʱ����Ӧ
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
	-- ��û�г��ﱻѡ��������Ӧ
	if t_operationSelPetInfo["id"] == 0 then
		ShowMidTips( "����ѡ��һֻ����" );
		return;
	end

	if t_selUseItem["id"] == 0  then
		local itemId = GetPetStrongUpItem();
		MessageBoxForItem("��ʾ",getItemDef(itemId).Name);
		MessageBoxForItemFrame:SetClientUserData(0,itemId);
		return;
	end
	
	local mainplayer = ActorMgr:getMainPlayer();
	-- �жϳ����״̬
	local petPackage = mainplayer:getPet();
	for nState, szDesc in pairs( t_cannotOperateStateDesc ) do
		if petPackage:isPetInState( t_operationSelPetInfo["index"], nState ) then
			ShowMidTips( t_cannotOperateStateDesc[nState].."�޷�����ǿ������" );
			return;
		end
	end

	if GameStallManager:isInStallIPet( SHITYPE, t_operationSelPetInfo["heightId"], t_operationSelPetInfo["lowId"] ) then
		ShowMidTips( "�ó����޷�����ǿ��" );
		return;
	end
	
	local petInfo	= petPackage:getPetInfo( t_operationSelPetInfo["index"] );
	--[[
	if math.floor( petInfo.LoveVal / 100 ) < 100 then
		ShowMidTips( "�����ҳ϶�û�������ܽ���ǿ��" );
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
		ShowMidTips( "û��ѡ��Ҫǿ�����Ǽ�" );
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
			ShowMidTips(string.format("%s�ѱ��������޷����г���ǿ��", getItemDef(petStrengthenDef.UseItemID).Name))
			return
		end
	end

	local nTotalItemNum				= container:getItemTotalNum( useItem:getItemId() );
	if nTotalItemNum < petStrengthenDef.UseItemNum then
		ShowMidTips( "ǿ�����ϲ��㣬���ܽ���ǿ��" );
		MessageBoxForItem("��ʾ",getItemDef(petStrengthenDef.UseItemID).Name);
		MessageBoxForItemFrame:SetClientUserData(0,petStrengthenDef.UseItemID);
		return;
	end

	if getPlayerMoney( mainplayer ) < petStrengthenDef.MoneyVal then
		ShowMidTips( "�������㣬�޷�ǿ��" );
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
		CostMoneyConfirmFrame:SetClientString("����ǿ��");
		SetWarnFrame( "Pet_DevelopmentBuildUpFrame" );
		CostMoneyConfirmFrame:Show();
		return;
	end

	local safeItem;
	-- ����Ƿ���ʹ������ǿ������
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

-- ���＼��ѧϰ

local t_changePetSkillPreCondition = { ["Pet_SkillGridRebuildFrame"] = {}, };
t_changePetSkillPreCondition["Pet_SkillGridRebuildFrame"].func =
function ( data )
	if data["skillGrid"] == t_selSkill["gridx"] then
		return true;
	end

	if ResetExistSkillConfirmFrame:IsShown() then
		ShowMidTips( "�������ȷ�ϲ���" );
		return false;
	end

	return true;
end

-- ������＼����λ��ť��ʱ����Ӧ
function Pet_SkillButton_OnClick()
	-- �ж���λ�ǲ����ڼ���״̬������ѡ��
	local tex = getglobal( this:GetName() .. "Icon" );
	if tex:GetTexture() == "uires\\TuPianLei\\WuPin\\��.tga" then
		return;
	end

	local changePetSkillPreCondition = t_changePetSkillPreCondition[this:GetParent()];
	if changePetSkillPreCondition ~= nil then
		if not changePetSkillPreCondition.func( { skillGrid = this:GetClientID() - 1 } ) then
			return;
		end
	end

	local mainplayer	= ActorMgr:getMainPlayer();
	-- �жϳ����״̬
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

-- ������뼼����λ��ʱ����Ӧ
function Pet_SkillButton_OnEnter()	
	-- ��ûѡ�г������ʾ�κ�����
	if t_operationSelPetInfo["id"] == 0 then
		return;
	end

	local nSkillType = this:GetClientUserData( 3 );
	if nSkillType == PET_SKILL_DISABLE then
		local szText = "���ȿ������＼�ܸ��ѧϰ����";
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

	-- �������м��ܣ�����ʾ����tips
	if diathesisDef ~= nil then
		local IconPath = getPetSkillPath()..diathesisDef.IconID..".tga";
		t_fonts = 
						{
						["PetSkillTipsFrameNameFont"]		= { value = diathesisDef.Name },-- ..diathesisDef.Level.."��"
						--["PetSkillTipsFrameSkillTypeFont"]	= { value = "���ͣ�"..t_skillGridTga[skillGridInfo.GridType].typeDesc },
						};
		
		t_tgas = 
						{
						["PetSkillTipsFrameTex"] = { path = ( IsInExistence( IconPath ) and IconPath or DEFAULT_ITEM_PATH ) },
						};
		
		t_riches = 
					{
					["PetSkillTipsFrameDescRich"] = { text = diathesisDef.DiathesisDesc, color = { r = 255, g = 255, b = 255 } },
					};
	-- ������ʾ����tips
	else
		t_fonts = 
						{
						["PetSkillGridTipsFrameNameFont"]		= { value = "���＼����λ" },
						--["PetSkillGridTipsFrameGridTypeFont"]	= { value = "���ͣ�"..t_skillGridTga[skillGridInfo.GridType].typeDesc },
						["PetSkillGridTipsFrameGridTypeFont"]	= { value = "����ѧϰ�¼���" },
						};
		
		t_tgas = 
						{
						-- path = t_skillGridTga[skillGridInfo.GridType].path
						["PetSkillGridTipsFrameTex"] = { path = t_skillGridTga[PET_SKILL_SELF_BUFF].path },
						};
		local t_skillGridTga = 
						{
						[PET_SKILL_ATK]			= "�ü��ܸ�Ϊ�����༼�ܸ񣬿�ѧϰ�����༼�ܣ���ʱ��ǳ���ѵ��ʦ���¶�������Ӧ���ͼ������ѧϰ",
						[PET_SKILL_OWNER_BUFF]	= "�ü��ܸ�Ϊ�����༼�ܸ񣬿�ѧϰ�����༼�ܣ���ʱ��ǳ���ѵ��ʦ���¶�������Ӧ���ͼ������ѧϰ",
						[PET_SKILL_SELF_BUFF]	= "�ü��ܸ�Ϊ��ǿ�༼�ܸ񣬿�ѧϰ��ǿ�༼�ܣ���ʱ��ǳ���ѵ��ʦ���¶�������Ӧ���ͼ������ѧϰ",
						};
		t_riches = 
					{
					["PetSkillGridTipsFrameDescRich"] = { text = "��ʱ��ǳ���ѵ��ʦ���¶�������Ӧ���ͼ������ѧϰ����", color = { r = 255, g = 255, b = 255 } },
					};
		
		local szText = "���＼�ܸ񣬿���ѧϰ�¼���";
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

-- ����뿪������λ��ʱ����Ӧ
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

-- ѧϰ��ť
local t_cannotLearnSkillStateDesc = 
					{
						-- [CLIENT_PET_STAT_FIGHT] = "���ﴦ�ڳ���״̬��",
						 [CLIENT_PET_STAT_DEAD] = "���ﴦ������״̬��",
					};

function Pet_SkillLearnFrame_StudyBtn_OnClick()
	-- ��û�г��ﱻѡ��������Ӧ
	if t_operationSelPetInfo["id"] == 0 then
		ShowMidTips( "���ȷ���Ҫѧϰ���ܵĳ���" );
		return;
	end

	if t_selUseItem["id"] == 0 then
		ShowMidTips( "���ȷ�����＼����" );
		return;
	end
	--[[
	if t_selSkill["gridx"] == -1 then
		-- 
		ShowMidTips( "��ѡ��������λ" );
		return;
	end
	--]]
	local mainplayer = ActorMgr:getMainPlayer();
	-- �жϳ����״̬
	local petPackage = mainplayer:getPet();
	
	for nState, szDesc in pairs( t_cannotLearnSkillStateDesc ) do
		if petPackage:isPetInState( t_operationSelPetInfo["index"], nState ) then
			ShowMidTips( t_cannotLearnSkillStateDesc[nState].."���ܽ���ѧϰ���ܲ���" );
			return;
		end
	end
	
	-- �ж������Ƿ����㹻��"..MONEY_TYPE_BULL_NAME.."
	if getPlayerMoney( mainplayer ) < PET_ADD_SKILL_GRID_MONEY then
		ShowMidTips( "����"..MONEY_TYPE_BULL_NAME.."ֵ���㣬����ѧϰ����" );
		return;
	end

	local petInfo		= petPackage:getPetInfo( t_operationSelPetInfo["index"] );
	local bookItemDef	= getItemDef( t_selUseItem.id );
	-- �ж�ѡ������λ�Ƿ����м���
	--[[
	local diathesisInfo	= petInfo.PetSkills.SkillGrid[ t_selSkill["gridx"] ];
	local diathesisiDef	= SkillMgr:getDiathesisDef( diathesisInfo.SkillID, diathesisInfo.SkillLevel );
	
	if diathesisiDef ~= nil then
		ShowMidTips( "�ø�λ���м��ܣ����ܸ���ѧϰ" );
		return;
	end
	--]]
	
	-- �ж������λ���Ƿ��Ѿ������Ҫѧϰ�ļ����ˣ����У�����ѧϰ
	for i = 1, petInfo.PetSkills.SkillGridNum do
		local diathInfo = petInfo.PetSkills.SkillGrid[i-1];
		local diathDef	= SkillMgr:getDiathesisDef( diathInfo.SkillID, diathInfo.SkillLevel );
		if diathInfo.SkillID == bookItemDef.Result[0].ResultVal1 and diathDef ~= nil then
			ShowMidTips( "���иü��ܣ������ظ�ѧϰ" );
			return;
		end
	end

	if GameStallManager:isInStallIPet( SHITYPE, t_operationSelPetInfo["heightId"], t_operationSelPetInfo["lowId"] ) then
		ShowMidTips( "�ó����޷�ѧϰ����" );
		return;
	end

	-- ���ж���ѡ������λ�ͼ���������ѧ�ļ����Ƿ�һ��
	diathesisDef	= SkillMgr:getDiathesisDef( bookItemDef.Result[0].ResultVal1, 1 );
	--[[
	if diathesisDef.PetDiathesisType ~= diathesisInfo.GridType then
		ShowMidTips( "��ѡ������λ�뼼�������Ͳ��� " );
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

-- ������ť
function Pet_SkillLevelUpFrame_LevelUpBtn_OnClick()
	-- ��û�г��ﱻѡ��������Ӧ
	if t_operationSelPetInfo["id"] == 0 then
		ShowMidTips( "���ȷ���Ҫ�������ܵĳ���" );
		return;
	end

	if t_selUseItem["id"] == 0 then
		ShowMidTips( "������������ܵĵ���" );
		return;
	end

	if t_selSkill["gridx"] == -1 then
		ShowMidTips( "��ѡ��Ҫ�����ļ���" );
		return;
	end
	
	local mainplayer = ActorMgr:getMainPlayer();
	-- �жϳ����״̬
	local petPackage = mainplayer:getPet();
	for nState, szDesc in pairs( t_cannotOperateStateDesc ) do
		if petPackage:isPetInState( t_operationSelPetInfo["index"], nState ) then
			ShowMidTips( t_cannotOperateStateDesc[nState].."���ܽ��м�����������" );
			return;
		end
	end

	local petInfo = petPackage:getPetInfo( t_operationSelPetInfo["index"] );
	-- �ж�ѡ������λ�Ƿ����м���
	local diathesisInfo	= petInfo.PetSkills.SkillGrid[t_selSkill["gridx"]];
	local diathesisiDef	= SkillMgr:getDiathesisDef( diathesisInfo.SkillID, diathesisInfo.SkillLevel );
	if diathesisiDef == nil then
		ShowMidTips( "��δѧϰ�ü��ܣ���������������ѧϰ" );
		return;
	end
	-- �ж��ü����Ƿ�ﵽ��ߵȼ�
	local newDiathesisiDef	= SkillMgr:getDiathesisDef( diathesisInfo.SkillID, diathesisInfo.SkillLevel + 1 );
	if newDiathesisiDef == nil then
		ShowMidTips( "�ü����Ѵﵽ��ߵȼ���������������" );
		return;
	end
	
	-- ���ж���ѡ������λ�ͼ���������Ѫ�ļ����Ƿ�һ��
	
	--[[
	if newDiathesisiDef.Learn.ItemID ~= t_selUseItem.id then
		ShowMidTips( "��ѡ����߲���������ѡ���ļ���" );
		return;
	end
	--]]
	petPackage:petSkillLevelUp( petInfo.WID.nHeight, petInfo.WID.nLow, diathesisInfo.SkillID, diathesisInfo.SkillLevel, 
								t_selSkill["gridx"], t_selUseItem["selItem"] );
end

-- ���䰴ť
function Pet_SkillGridExtendFrame_ExtendBtn_OnClick()
	-- ��û�г��ﱻѡ��������Ӧ
	if t_operationSelPetInfo["id"] == 0 then
		ShowMidTips( "���ȷ������" );
		return;
	end

	if t_selUseItem["id"] == 0  then
		ShowMidTips( "�������＼�ܸ��������" );
		return;
	end
	
	local mainplayer = ActorMgr:getMainPlayer();
	-- �жϳ����״̬
	local petPackage = mainplayer:getPet();
	for nState, szDesc in pairs( t_cannotOperateStateDesc ) do
		if petPackage:isPetInState( t_operationSelPetInfo["index"], nState ) then
			ShowMidTips( t_cannotOperateStateDesc[nState].."���ܽ������似����λ����" );
			return;
		end
	end

	-- �ж������Ƿ����㹻��"..MONEY_TYPE_BULL_NAME.."
	if getPlayerMoney( mainplayer ) < PET_ADD_SKILL_GRID_MONEY then
		ShowMidTips( "����"..MONEY_TYPE_BULL_NAME.."ֵ���㣬�������似��" );
		return;
	end

	-- �鿴�Ƿ���ʣ�����λ��������
	local petInfo		= petPackage:getPetInfo( t_operationSelPetInfo["index"] );
	local skillGrids	= petInfo.PetSkills;

	local mainplayer= ActorMgr:getMainPlayer();
	local container	= mainplayer:getContainer();
	--local useItem	= container:getItem( t_selUseItem["id"] );
	local useItem	= t_selUseItem["selItem"];
	local itemDef	= getItemDef( useItem:getItemId() );

	container:send_UsedExpendableItem( useItem:getListType(), useItem:getGridIdx(), t_operationSelPetInfo["index"] );
end

-- ���찴ť
function Pet_SkillGridRebuildFrame_RebuildBtn_OnClick()
	-- ��û�г��ﱻѡ��������Ӧ
	if t_operationSelPetInfo["id"] == 0 then
		ShowMidTips( "���ȷ������" );
		return;
	end

	if t_selUseItem["id"] == 0 then
		ShowMidTips( "����뼼�ܸ�������" );
		return;
	end
	
	if t_selSkill["gridx"] == -1 then
		ShowMidTips( "����ѡ��һ�����ܸ�λ" );
		return;
	end
	
	local mainplayer = ActorMgr:getMainPlayer();
	-- �жϳ����״̬
	local petPackage = mainplayer:getPet();
	for nState, szDesc in pairs( t_cannotOperateStateDesc ) do
		if petPackage:isPetInState( t_operationSelPetInfo["index"], nState ) then
			ShowMidTips( t_cannotOperateStateDesc[nState].."���ܽ��м��ܸ�������" );
			return;
		end
	end

	if getPlayerMoney( mainplayer ) < PET_WASH_SKILL_GRID_MONEY then
		ShowMidTips( "���ϵ�"..MONEY_TYPE_BULL_NAME.."�������������ü�����λ" );
		return;
	end
	
	local petInfo		= petPackage:getPetInfo( t_operationSelPetInfo["index"] );
	local diathesisInfo	= petInfo.PetSkills.SkillGrid[t_selSkill["gridx"]];
	local diathesisiDef	= SkillMgr:getDiathesisDef( diathesisInfo.SkillID, diathesisInfo.SkillLevel );
	if diathesisiDef ~= nil then
		-- TODO:ȷ�����ü��ܸ�
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
	rich:SetText( "#P��ȷ��Ҫ����"..szPetName.."��", 255, 255, 190 );
end

local t_LastPetInfo = {};
-- ȷ���������ﰴť
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

-- ȡ��
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

-- �鿴�����������
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

-- ȷ�����ó��＼�ܸ�
local t_resetPetSkillInfo = { heightId = 0, lowId = 0, id = 0, index = -1 };
function SetResetPetSkillInfo( data )
	t_resetPetSkillInfo = data;
end

function ClearResetPetSkillInfo( )
	t_resetPetSkillInfo = { heightId = 0, lowId = 0, id = 0, index = -1 };
end

-- ȷ�����ü��ܸ�
function ResetExistSkillConfirmFrame_ConfirmBtn_OnClick()
	if t_resetPetSkillInfo["heightId"] ~= t_operationSelPetInfo["heightId"] or 
		t_resetPetSkillInfo["lowId"] ~= t_operationSelPetInfo["lowId"] then
		ShowMidTips( "������ʧЧ" );
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

-- �����Զ��ָ����
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

--��ȡ�����п�ʹ�õȼ���͵ĳ���ҩˮ
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

--��ȡ�����п�ʹ�õȼ���ߵĳ���ҩˮ
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

--��ȡ�����е�ѱ���ǹ�
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
	-- ����������
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
	-- ������û���ڳ���״̬���������Զ�ʹ��ҩƷ
	if not petPackage:isPetInState( nHeightID, nLowID, CLIENT_PET_STAT_FIGHT ) then
		return;
	end

	local oneConfig = GameUserConfig:getOnePetConfig( CLIENT_SET_PET_MIN_ZHONG_CHENG );
	-- ��û��������Զ����ܣ�
	if not oneConfig.m_bOpen or oneConfig.m_uItemID < 0 or oneConfig.m_uMinValue < 0 then
		return;
	end

	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	
	-- �������ж�Ӧ��Ʒ����Ϊ0
	if container:getItemCount( nItemID ) == 0 then
		return;
	end


	-- ��û�ﵽ�������
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
	-- ������û���ڳ���״̬���������Զ�ʹ��ҩƷ
	if not petPackage:isPetInState( nHeightID, nLowID, CLIENT_PET_STAT_FIGHT ) then
		return;
	end
	
	local t_canUseConfig = self["canUseConfig"];
	-- ����������
	local nOptionType;
	local oneElem;
	for i = 1, table.getn( t_canUseConfig ) do
		oneElem	= t_canUseConfig[i];
		if nCurHP <= oneElem["value"] * petInfo.MaxHP / 100 then
			nOptionType = oneElem["index"];
			break;
		end
	end
	
	-- ��û�п���HP�Զ��ָ�ѡ���ɶҲ����
	if not nOptionType then
		return;
	end
	
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	
	local nItemID = oneElem["id"];
	-- �������ж�Ӧ��Ʒ����Ϊ0
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

-- �ҳ϶���λ
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

-- ȷ����ť
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
["PetAutoRecoverFrame_ItemGridBtn1"] = { resultID = RESULT_ADD_POWER, desc = "����ҩˮ", defaultValue = 50, 
											edit = "PetAutoRecoverFrame_FirstHPEdit" },
["PetAutoRecoverFrame_ItemGridBtn2"] = { resultID = RESULT_ADD_POWER, desc = "����ҩˮ", defaultValue = 30, 
											edit = "PetAutoRecoverFrame_SecondHPEdit" },
["PetAutoRecoverFrame_ItemGridBtn3"] = { resultID = RESULT_PET_ADD_LOVEVAL, desc = "���ӳ����ҳ϶ȵ���", defaultValue = 50, 
											edit = "PetAutoRecoverFrame_ZhongChengEdit" },
};

function PetAutoRecoverFrame_ItemGridBtn_OnEnter()
	local id = this:GetClientUserData( 0 );
	if id == 0 then
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "�����"..t_btnMapToResult[this:GetName()]["desc"], 
							frame = this:GetParent(), button = this:GetName(), horizonal = "right" } );
		return;
	end
	
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	if container:getItemCount( id ) == 0 then
		local itemDef = getItemDef( id );
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "�Ѿ�û��"..itemDef.Name.."�������������Ʒ", 
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
		ShowMidTips("��ǰ����ȼ�����ʹ�ø���Ʒ");
		return false;
	end

	local bFitItem		= false;
	for i = 1, MAX_ITEM_RESULT do
		local oneResult = itemDef.Result[i-1];
		if oneResult.ResultID == resultID then
			bFitItem	= true;
		end
	end
	-- TODO: �Ӹ��ж�����Ƿ���ʹ��
	if not bFitItem then
		ShowMidTips( "����"..t_btnMapToResult[itemBtn:GetName()]["desc"].."�����ܷ���" );
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
		return ShowMidTips("��ǰû���ٻ�����")
	end
	for state, tips in pairs(t_cannotLearnSkillStateDesc) do
		if petMgr:isPetInState(index, state) then
			return ShowMidTips(tips.."���ܽ���ѧϰ���ܲ���")
		end
	end
	local info = petMgr:getPetInfo(index)
	local ItemDef = item:getItemDef()
	for i = 1, info.PetSkills.SkillGridNum do
		local skills = info.PetSkills.SkillGrid[i - 1]
		if skills.SkillID ~= 0 and skills.SkillLevel ~= 0 and skills.SkillID == ItemDef.Result[0].ResultVal1 then
			return ShowMidTips("���иü��ܣ������ظ�ѧϰ")
		end
	end
	if GameStallManager:isInStallIPet(SHITYPE, info.WID.nHeight, info.WID.nLow) then
		return ShowMidTips( "�ó����޷�ѧϰ����" )
	end
	petMgr:petSkillLearn(info.WID.nHeight, info.WID.nLow, idx or 0, item)
end

function PetAddLoveLevelMessageFrameOkBtn_OnClick()
	local high = PetAddLoveLevelMessageFrame:GetClientUserData( 0 )
	local low = PetAddLoveLevelMessageFrame:GetClientUserData( 1 )
	local player = ActorMgr:getMainPlayer()
	local petMgr = player:getPet()
	if GameStallManager:isInStallIPet( SHITYPE, t_mainSelPetInfo.heightId, t_mainSelPetInfo.lowId ) then
		ShowMidTips( "�����ϼ��У����ܽ��а��Ĳ���" )
	elseif GameExchange:isInExchangePetByWID( t_mainSelPetInfo.heightId, t_mainSelPetInfo.lowId ) then
		ShowMidTips( "�������ڽ����У����ܽ��а��Ĳ���" )
	else
		petMgr:loveLevelUp( high, low )
	end	
	util.closeParent(this)
end


function GetPetLoveLevelDesc( level )
	local tLoveLevelDesc = { [0] = "�ɽ�������", [1] = "�ɽ���һ��", [2] = "���ɽ���" }
	return tLoveLevelDesc[level]
end

---------------------------------------------������-----------------------------------
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
		ShowMidTips("�ó����Ѳ�����");
		return ;
	end
	if petPackage:isExtraClanJiYangPet( petInfo ) then 
		ShowMidTips("�������ڼ����У���ǰ���������ȡ�������ٽ��иò�����");
		return ;
	end
	local useDef 		= GameArmProce:getUseDef( USE_ID_PET_STEP_UP_TOLUA );

	local nNeedMoney 	= 0;
	if t_PetUnBindNeed[petInfo.PetGenius] ~= nil then
		nNeedMoney = t_PetUnBindNeed[petInfo.PetGenius];
	end
	if mainplayer:getMoney() < nNeedMoney then 
		ShowMidTips("�������Ҳ��㣡");
		ShowBuyMoneyFrame();
		return ;
	end
	local nTotalItemNum = container:getItemTotalNum( PET_UNBIND_ITEM_ID_TOLUA );
	local nItemNum		= 1;
	if nTotalItemNum == 0 or nTotalItemNum < nItemNum then 
		ShowMidTips("�����ʵ���㣡");
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
	PetUnBindFrame_PetListBackGroundHeadFont:SetText( "�����б�"..petPackage:getPetNum().."/".. petPackage:getPetMaxNum() );
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
			ShowMidTips( "����û������ܽ��иò������Զ��ر����" );
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
	-- ����ͼ��
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
#cffffbe������˵����#n
#cffffff�� ÿ�ν����Ҫһ�������ʵ#n
#cffffff�� ��������ɺ�ر�ɡ��������״̬#n
#cffffff�� ��������Խ�����ĵ�����Խ��#n]]
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
	-- ���������û��ʾ�������ų���������һ����ʾ
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
		ShowMidTips("���ĳ�����ɹ���");
		local modelview = getglobal("PetUnBindFrame_IconBtnModelView");
		modelview:setPetModel(0, 0, "effect\\entity\\fumo01.ent")
		modelview:setEntityPosition(0, 0, 100, 0)
		if not modelview:IsShown() then
			modelview:Show();
		end
	end
end
