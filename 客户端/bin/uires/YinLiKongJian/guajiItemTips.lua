
local BASE_AREA1		= 1;	--������Ϣ��1
local BASE_AREA2		= 2;	--������Ϣ��2
local FIGHT_AREA		= 3;	--ս��������
local INTENSIFY_AREA	= 4;	--ǿ��˵����
local SUIT_AREA			= 5;	--��װ������
local OTHER_AREA		= 6;	--����˵����
local KEYWORLD_AREA		= 7;
local BASE_AREA3		= 9;
local nEndItemTime		= 0;
local onEnterFuc		= nil;
local onEnterBtn		= nil;
local szSelfName		= nil;
local MAX_RICH_LINE		= 10;
t_WearPosInfo = {};
t_WearPosInfo[EQUIP_POS_RIGHTHAND]		= "����";
t_WearPosInfo[EQUIP_POS_HEAD]			= "ͷ��";
t_WearPosInfo[EQUIP_POS_BODY]			= "�ؼ�";
t_WearPosInfo[EQUIP_POS_BACK]			= "����";
t_WearPosInfo[EQUIP_POS_LEFT]			= "����";
t_WearPosInfo[EQUIP_POS_FACE]			= "����";
t_WearPosInfo[EQUIP_POS_SHOULDER]		= "���";
t_WearPosInfo[EQUIP_POS_FASHION_BODY]	= "ʱװ�·�";
t_WearPosInfo[EQUIP_POS_BADGE]			= "����";
t_WearPosInfo[EQUIP_POS_AMULET]			= "���";
t_WearPosInfo[EQUIP_POS_NECKLACE]		= "����";
t_WearPosInfo[EQUIP_POS_RING]			= "��ָ";
t_WearPosInfo[EQUIP_POS_RING1]			= "��ָ";
t_WearPosInfo[EQUIP_POS_RING2]			= "��ָ";
t_WearPosInfo[EQUIP_POS_RING3]			= "��ָ";
t_WearPosInfo[EQUIP_POS_SHOE]			= "Ь��";

t_WearPosInfo[EQUIP_POS_GRAIL]			= "ʥ��";
t_WearPosInfo[EQUIP_POS_QIYUE]			= "��Լ";

t_WearPosInfo[EQUIP_POS_SACHET]			= "����";	
t_WearPosInfo[EQUIP_POS_AMULET1]		= "����";
t_WearPosInfo[EQUIP_POS_AMULET2]		= "����";
t_WearPosInfo[EQUIP_POS_FASHION_HEAD]	= "ʱװñ��";
t_WearPosInfo[EQUIP_POS_FASHION_HAIR]	= "ʱװ����";
t_WearPosInfo[EQUIP_POS_FASHION_BACK]	= "ʱװ����";
t_WearPosInfo[EQUIP_POS_FASHION_SHOES]	= "ʱװЬ��";
t_WearPosInfo[EQUIP_POS_FASHION_HAND]	= "ʱװ����";
t_WearPosInfo[EQUIP_POS_FASHION_LEG]	= "ʱװ����";


--EQUIP_POS_QIYUE			= 16,
--EQUIP_POS_NECKLACE		= 17, 	/* ���� */
--EQUIP_POS_SACHET		= 18, 	/* ���� */


ITEM_GET_BIND	= 1;
ITEM_WEAR_BIND	= 2;

local t_tipsEvent = {};

local t_ArmBind = {"","ʰȡ���","װ�����","�Ѱ�"};
local t_TipsLineList = {};
local t_SelfTipsLineList = {};


function getItemInfo(ItemDef,szText)
	if szText == nil then return end
	local ItemInfo=
	{
		{"#ItemID#",                      ItemDef.ItemID},					--/*   */
		{"#Name#",						  ItemDef.Name},					--/*   */
		{"#IconID#",					  ItemDef.IconID},					--/*   Ver.6 ͼ��id */
		{"#SellPrice#",					  ItemDef.SellPrice},				--/*   ����npcֵ����Ǯ */
		{"#BuyPrice#",                    ItemDef.BuyPrice},				--/*   ��npc����Ҫ����Ǯ  */
		{"#MaxPile#",                     ItemDef.MaxPile},					--/*   �ɶѵ��������*/
		{"#ItemType#",                    ItemDef.ItemType},				--/*    */
		{"#ArmDur.MaxDur#",               ItemDef.ArmDur.MaxDur},			--/*   Ver.8 װ���;� */
		{"#ArmDur.FightStatusDur#",       ItemDef.ArmDur.FightStatusDur},	--/*   Ver.8 װ���;�*/
		{"#ArmFight#",                    ItemDef.ArmFight},				--/*   Ver.8 װ��ս����  */
		{"#SuitSetID#",                   ItemDef.SuitSetID},				--/*   Ver.8 ������װ��ID */
		{"#ValidTime#",                   ItemDef.ValidTime},				--/*   װ���ӻ�ÿ�ʼ����Чʱ�䣬������װ����λСʱ */
		{"#NumType#",                     ItemDef.NumType},					--/*   item id �ı��ǰ3λ����ϸ����Ź��� */
		{"#NoDrop#",                      ItemDef.NoDrop},					--/*   �Ƿ��ǲ��ɶ������� */
		{"#ArmBind#",                     ItemDef.ArmBind},					--/*   �Ƿ��װ�� */
		{"#ExchgLevel#",                  ItemDef.ExchgLevel},				--/*   ��ͨ�ȼ� */
		{"#Important#",                   ItemDef.Important},				--/*   ������Ҫ�̶� */
		{"#AmmAtk#",                      ItemDef.AmmAtk},					--/*   ��ҩ�Ĺ���������ָ�� */
		{"#WeaponAmmNeed#",               ItemDef.WeaponAmmNeed},			--/*   ��ҩ���� */
		{"#WeaponAmmType#",               ItemDef.WeaponAmmType},			--/*   ������ҩ���ͣ��������ʹ��ʲô���͵ĵ�ҩ */
		{"#AmmType#",                     ItemDef.AmmType},					--/*   ��ҩ���� */
		{"#WearPre.RoleLevelMin#",        ItemDef.WearPre.RoleLevelMin},	--/*    */
		{"#WearPre.RoleLevelMax#",        ItemDef.WearPre.RoleLevelMax},	--/*    */
		{"#WearPre.Career#",              ItemDef.WearPre.Career},			--/*    */
		{"#WearPre.DiathesisID#",         ItemDef.WearPre.DiathesisID},		--/*    */
		{"#WearPre.Gender#",              ItemDef.WearPre.Gender},			--/*    */
		{"#WearPosNum#",                  ItemDef.WearPosNum},				--/*   װ��ռ������ */
		{"#WearPos1#",                    ItemDef.WearPos1},				--/*   ռ��װ���� Bind Macrosgroup:WEAR_POS */
		{"#WearPos2#",                    ItemDef.WearPos2},				--/*   ռ��װ���� Bind Macrosgroup:WEAR_POS */
		{"#WearPos3#",                    ItemDef.WearPos3},				--/*   ռ��װ���� Bind Macrosgroup:WEAR_POS */
		{"#ItemDesc#",                    ItemDef.ItemDesc},    			--/*    */
	};

	local nSize = table.getn(ItemInfo);
	for i = 1 , nSize do
		local v = string.find(szText,ItemInfo[i][1]);
		if v ~= 0 then
			szText = string.gsub(szText,ItemInfo[i][1],ItemInfo[i][2]);
		end
	end
	return szText;
end

function getAttrDesc( AddAttrID, AddAttrVal )
	if AddAttrVal == 0 then
		return "";
	end	 

	local nNewVal	 = AddAttrVal/100;
	local t_AttrDesc =
	{
		{"#�ٷֱ�#",	AddAttrVal .. "%"	},	-- /*��ʾ�ٷֱ�	*/
		{"#��ֱ�#",	nNewVal .. "%"		},	-- /*��ʾ��ֱ�	*/
		{"#��ֵ#",		AddAttrVal			},	-- /*��ʾ��ֵ	*/
	};

	local szAddAttrDesc = "";  
	local ArmDef		= getArmAddAttrDef( AddAttrID );

	if AddAttrID == ADD_ATTR_RESULT_SKILL_LEVEL then
		local SkillDef	= SkillMgr:getSpellStaticAtt( ArmDef.Result[AddAttrVal].ResultVal1, 1 );
		-- TODO: ��������д������ʾװ���Լ��ܵ�����Ч����
		if SkillDef == nil then
			szAddAttrDesc = "���ܣ�"..SkillDef.Name.."�ȼ�����"..ArmDef.Result[AddAttrVal].ResultVal2.."��";
		end

	else
		szAddAttrDesc = ArmDef.AddAttrName;

		for i = 1 ,  table.getn(t_AttrDesc) do
			if string.find( szAddAttrDesc, t_AttrDesc[i][1], 1, true ) then
				szAddAttrDesc = ChatManager:strReplace( szAddAttrDesc, t_AttrDesc[i][1],t_AttrDesc[i][2] );
			end
		end
	end

	return szAddAttrDesc;
end

-- ��ȡ�¹ؼ�������
function getNewAttrDesc( AddAttrID, tAttrVal )
	local t_AttrDesc =
	{
		{"#val1�ٷֱ�#",	tAttrVal[1] ~= nil and ( math.floor( tAttrVal[1]*100)/100 .. "%" ) or nil	},	-- /*��ʾ�ٷֱ�	*/
		{"#val1��ֱ�#",	tAttrVal[1] ~= nil and ( math.floor( tAttrVal[1])/100 .. "%" ) or nil		},	-- /*��ʾ��ֱ�	*/
		{"#val1#",			math.floor( tAttrVal[1] )													},	-- /*��ʾ��ֵ	*/
		{"#val2�ٷֱ�#",	tAttrVal[2] ~= nil and ( math.floor( tAttrVal[2]*100)/100 .. "%" ) or nil	},	-- /*��ʾ�ٷֱ�	*/
		{"#val2��ֱ�#",	tAttrVal[2] ~= nil and ( math.floor( tAttrVal[2])/100 .. "%" ) or nil		},	-- /*��ʾ��ֱ�	*/
		{"#val2#",			math.floor( tAttrVal[2]	)													},	-- /*��ʾ��ֵ	*/
		{"#val3�ٷֱ�#",	tAttrVal[3] ~= nil and ( math.floor( tAttrVal[3]*100)/100 .. "%" ) or nil	},	-- /*��ʾ�ٷֱ�	*/
		{"#val3��ֱ�#",	tAttrVal[3] ~= nil and ( math.floor( tAttrVal[3])/100 .. "%" ) or nil		},	-- /*��ʾ��ֱ�	*/
		{"#val3#",			math.floor (tAttrVal[3] )													},	-- /*��ʾ��ֵ	*/
		{"#val4�ٷֱ�#",	tAttrVal[4] ~= nil and ( math.floor( tAttrVal[4]*100)/100 .. "%" ) or nil	},	-- /*��ʾ�ٷֱ�	*/
		{"#val4��ֱ�#",	tAttrVal[4] ~= nil and ( math.floor( tAttrVal[4])/100 .. "%" ) or nil		},	-- /*��ʾ��ֱ�	*/
		{"#val4#",			math.floor( tAttrVal[4] )													},	-- /*��ʾ��ֵ	*/
	};

	local szAddAttrDesc = "";
	local newAddAttrDef = getNewAddAttrDef( AddAttrID )
	szAddAttrDesc = newAddAttrDef.AddAttrDesc;

	for i = 1 ,  table.getn(t_AttrDesc) do
		if string.find( szAddAttrDesc, t_AttrDesc[i][1], 1, true ) and t_AttrDesc[i][2] ~= nil then
			szAddAttrDesc = ChatManager:strReplace( szAddAttrDesc, t_AttrDesc[i][1], t_AttrDesc[i][2] );
		end
	end

	return szAddAttrDesc;
end

function LinkItemTipsFrame_OnHie()
	if ArmItemTipsFrame:IsShown() then
		ArmItemTipsFrame:Hide();
	end
end

local nArmBindNo  = 0;	--�Ѱ�װ��
local nArmBindYes = 1;	--δ��װ��
local nAllCareer  = 0;	--��Ҫ��
local nWarrior    = 1;  --սʿ
local nMagician   = 2;  --��ʦ
local nGunner     = 3;  --ǹ��
local nChurch	  = 4;  --��˾


LUA_ITEMTIPS_TYPE_LINK = 1;
LUA_ITEMTIPS_TYPE_NORMAL = 2;

LUA_ITEMTIPS_ITEMDEF	= 2 ;
LUA_ITEMTIPS_ITEM		= 3 ;

local LINE_TYPE_FONT		= 1;
local LINE_TYPE_RICH		= 2;
local LINE_TYPE_TEXTURE		= 3;
local LINE_TYPE_HEADTEXT	= 4;
local LINE_TYPE_TOOLTEXT	= 5;
local LINE_TYPE_GEM_TEXTURE = 6;
local LINE_TYPE_BINDTEXT	= 7;
local LINE_TYPE_MONEY		= 8;
local LINE_TYPE_ATTRADD		= 9;
local LINE_TYPE_DISTANCE	= 10;

local ITEM_TYPE_ITEM = 1;
local ITEM_TYPE_AMM  = 2;
local ITEM_TYPE_ALL  = 3;

function GetArmWearLv( itemDef, item )
	if OffMgr:bitAnd( itemDef.CtrlFlag, ITEM_NO_LEVELLIMIT ) then return 0 end
	local nLv = itemDef.WearPre.RoleLevelMin
	if item then
		for i = 1, item:getArmAddAttrNum() do
			local AddAttrInst	= item:getArmAddAttr(i-1);
			local newAddAttrDef = getNewAddAttrDef( AddAttrInst.AddAttrID );
			if newAddAttrDef.AddAttrResultID == ADD_ATTR_RESULT_DEC_WEAR_LEVEL then
				nLv =( nLv - AddAttrInst.Val ) < 1 and 1 or ( nLv - AddAttrInst.Val )
			end
		end
		for i=1,item:getArmWashAddAttrNum() do
			local AddAttrInst	= item:getArmWashAddAttrInst(i-1);
			local newAddAttrDef = getNewAddAttrDef( AddAttrInst.AddAttrID );
			if newAddAttrDef.AddAttrResultID == ADD_ATTR_RESULT_DEC_WEAR_LEVEL then
				nLv =( nLv - AddAttrInst.Val ) < 1 and 1 or ( nLv - AddAttrInst.Val )
			end
		end
	end
	return nLv
end

function GetUseLvColr(itemDef)
	local nLv = itemDef.WearPre.RoleLevelMin
	if item and item:getItemType() == ITEM_SLOTTYPE_AMM then
		for i = 1, item:getArmAddAttrNum() do
			local AddAttrInst	= item:getArmAddAttr(i-1);
			local newAddAttrDef = getNewAddAttrDef( AddAttrInst.AddAttrID );
			if newAddAttrDef.AddAttrResultID == ADD_ATTR_RESULT_DEC_WEAR_LEVEL then
				nLv =( nLv - AddAttrInst.Val ) < 1 and 1 or ( nLv - AddAttrInst.Val )
			end
		end
		for i=1,item:getArmWashAddAttrNum() do
			local AddAttrInst	= item:getArmWashAddAttrInst(i-1);
			local newAddAttrDef = getNewAddAttrDef( AddAttrInst.AddAttrID );
			if newAddAttrDef.AddAttrResultID == ADD_ATTR_RESULT_DEC_WEAR_LEVEL then
				nLv =( nLv - AddAttrInst.Val ) < 1 and 1 or ( nLv - AddAttrInst.Val )
			end
		end
	end
	if MainPlayer:getLevel() >= nLv then
		if nLv < itemDef.WearPre.RoleLevelMin then
			return GetColor()[COLOR_GREEN];
		else
			return GetColor()[COLOR_WHITE];
		end			
	else
		return GetColor()[COLOR_RED];
	end
end

--��ȡְҵ������ɫ
function GetCareerColor(itemDef)
	if itemDef.WearPre.Career == nAllCareer or itemDef.WearPre.Career == MainPlayer:getCareer() or itemDef.WearPre.Career == MainPlayer:getCareer() % 16 then
		return GetColor()[COLOR_WHITE];
	else
		return GetColor()[COLOR_RED];
	end
end

function GetItemRespact( itemDef )
	if itemDef.NeedRespact.ID ~= 0 then
		local respactPartDef = GameRespact:getRespactPartyDef( itemDef.NeedRespact.ID );
		return "��Ҫ\b"..respactPartDef.PartyName.."-"..GetRespactParseDesc( itemDef.NeedRespact.Value )[1];
	end
	return "";
end

function GetRespacColor( itemDef )
	return GetColor()[COLOR_WHITE];
end

function GetDur( item, itemDef )
	if item ~= nil and item:getItemDur() >= 0 then
		local nCurDur = item:getItemDur(); -- �;ö�
		local nMaxDur = item:getItemMaxDur();
		if nMaxDur <=  0 then
			return "";
		else
			return ("�� �� �ȣ�"..nCurDur.."/"..nMaxDur);
		end
	else
		return "";
	end
end

function GetWearPos(itemDef)
	if itemDef.WearPosNum == 0 or itemDef.WearPos1 >= EQUIP_POS_END then 
		return 0;
	end

	return t_WearPosInfo[itemDef.WearPos1];
end

--����˵��: ��ʾ��Ϣ����(��Ʒ������, �ǳ�����), ��������(ItemID_Type, ItemDef_Type, Item_Type), ����(ItemID, ItemDef, Item)
function ShowItemTips( nTipsType, nContentType, content, onenterfuc, szOnEnterBtn, isShopItem )
	onEnterFuc		= onenterfuc;
	onEnterBtn		= szOnEnterBtn;
	NormalItemTipsFrame:SetClientString("");
	local item = nil;
	local nItemType;
	if nContentType == LUA_ITEMTIPS_ITEMDEF then
		if content.ItemID == 0 then
			return;
		end
	elseif nContentType == LUA_ITEMTIPS_ITEM then
		item = content;
		nItemType = item:getItemType();
	end

	-- �ж�item�Ƿ�Ϊ��
	if item and item:getItemId() <= 0 then
		return;
	end

	local itemDef;
	if nContentType == LUA_ITEMTIPS_ITEMDEF or nContentType == LUA_ITEMTIPS_SHORTCUT then
		itemDef = content;	
	elseif item:getItemType() == ITEM_SLOTTYPE_NONE then
		return;
	elseif item:getItemId() > 0 then
		itemDef = item:getItemDef();
	end

	-- �ж�item�Ƿ�Ϊ��
	if itemDef == nil then
		return;
	end

	if nTipsType == LUA_ITEMTIPS_TYPE_LINK then
		if NormalItemTipsFrame:IsShown() then
			NormalItemTipsFrame:Hide();
		end
		szSelfName = "LinkItemTipsFrame"
	elseif nTipsType == LUA_ITEMTIPS_TYPE_NORMAL then
		if LinkItemTipsFrame:IsShown() then
			LinkItemTipsFrame:Hide();
		end
		szSelfName = "NormalItemTipsFrame"
	end

	--����Tips
	if not nItemType then
		nItemType = getItemType(itemDef.ItemID); --��ȡ��Ʒ����
	end
	if nItemType == ITEM_SLOTTYPE_AMM then
		SetAmmTips( nTipsType, item, itemDef, szSelfName );
	elseif nItemType == ITEM_SLOTTYPE_ITEM then
		SetItemTips( nTipsType, item, itemDef, szSelfName, isShopItem );
	end

	ShowTips(szSelfName, itemDef, nTipsType, item ); --��ʾ�ж���

	if nTipsType == LUA_ITEMTIPS_TYPE_LINK then
		local btn = getglobal(szSelfName.."ITCloseBtn");
		btn:Show();
		LinkItemTipsFrame:Show();
	elseif nTipsType == LUA_ITEMTIPS_TYPE_NORMAL then
		NormalItemTipsFrame:Show();
		nEndItemTime = 0;
	end
	
	SetItemTipsPoint( szSelfName, itemDef, szOnEnterBtn ); --����λ��
end

function GetCareer( nType )
	if nType == nAllCareer then
		return "";
	elseif nType == nWarrior then
		return "սʿ";
	elseif nType == nMagician then
		return "��ʦ"
	elseif nType == nGunner then
		return "ǹ��"
	elseif nType == nChurch then
		return "��˾"
	end
	return t_raceList[nType] or "";
end

local nMaxFontNumber = 53;
local nMaxLineNumber = 5;
local MAXGEMTEX		 = 4;
function ClearItemTips( name )
	for i = 1 , nMaxFontNumber do
		local font = getglobal(name.."ITItemFont"..i);
		font:SetText("");
		font:Hide();
	end

	for i = 1 , nMaxLineNumber do
		local line		= getglobal(name.."ITItemLineTex"..i);
		line:Hide();
	end

	for i = 1, MAX_RICH_LINE do
		local rich = getglobal(name.."ITItemRich"..i);
		rich:Clear();
		rich:clearHistory();
		rich:Hide();
	end
	
	for i=1,12 do
		local tex = getglobal(name.."ITItemImportantTex"..i);
		tex:Hide();
	end

	local moneytex = getglobal( name.."ITItemMoneyTex");
	moneytex:Hide();

	t_TipsLineList = {};

	t_SelfTipsLineList = {};
	-- ���tip�ϵı�ʯͼ��
	for i = 1 , MAXGEMTEX do
		local AmmGemTex = getglobal(name.."InLayGemItemTex"..i); -- ��Ƕ��ʯͼ
		local lineback	= getglobal(name.."InLayGemItemTex"..i.."_Rim");
		lineback:Hide();
		AmmGemTex:Hide();
	end
end

function ItemTips_OnShow()
end

function ItemTips_OnUpdate()
	if onEnterFuc ~= nil then
		if onEnterBtn ~= nil then
			onEnterFuc( onEnterBtn );
		else
			onEnterFuc();
		end
	end
end

function ItemTips_OnHide()
	if ArmItemTipsFrame:IsShown() then
		ArmItemTipsFrame:Hide();
	end
	this:SetClientUserData(0,0);
	this:SetClientUserData(1,0);
	this:SetClientUserData(2,0);
	this:SetClientUserData(3,0);
end

function ItemTips_OnLoad()
	this:SetClientUserData(0,0);
	this:SetClientUserData(1,0);
	this:SetClientUserData(2,0);
	this:SetClientUserData(3,0);
end

--������ʾ������
function AddTipsLine( nAreaType, szText, content, t_Color, nType, bAlwayDis, nResultIndex ) --������ʾ���ݣ���ɫֵ1,��ɫֵ2,��ɫֵ3,����,�Ƿ�������ʾ
	if bAlwayDis == false and ( tonumber(content) == 0 or content == nil or content == "" ) then 
		return;
	end  --����0��ʱ����ʾ
	if szText == nil then
		return;
	end
	local t_TipsLineInfo = { nAreaType,szText,content, t_Color, nType, nResultIndex };
	table.insert(t_TipsLineList, t_TipsLineInfo);
end

--��ȡװ�����͵�����
function GetItemTypeName(ItemType)
	for i = 1, table.getn(t_tagARM_TYPE) do
		if t_tagARM_TYPE[i][1] <= ItemType and t_tagARM_TYPE[i][2] >= ItemType  then
			return t_tagARM_TYPE[i][3];
		end
	end
end

function GetItemType(itemid)
	return GetItemTypeName(math.floor(itemid / 10000)) or "";
end

-- ��ȡװ���Ƿ�ɽ�������
function GetCanExChange( item, itemDef )
	local szDesc = "";
	if itemDef.NoDeal == 1 then
		szDesc = "���ɽ���";
	end

	if item ~= nil then
		if item:isItemBind() or ( itemDef.ArmBind == ITEM_GET_BIND or itemDef.ArmBind == ITEM_WEAR_BIND ) then
			szDesc = "���ɽ���";
		end
	end

	return szDesc;
end

-- ��ȡװ��������
function GetArmBind( item, itemDef )
	if item ~= nil then
		if item:isItemBind() or ( itemDef.ArmBind == ITEM_GET_BIND or itemDef.ArmBind == ITEM_WEAR_BIND ) then
			return t_ArmBind[4];
		end
	elseif NormalItemTipsFrame:GetClientUserData(1) == 1 or NormalItemTipsFrame:GetClientUserData(1) == 3 then
		return t_ArmBind[3];
		
	end
	return t_ArmBind[itemDef.ArmBind + 1];
end

-- ��ȡ��������ֵ ����
function GetBaseAttrValue( item, nValue, nResultId )
	local nAddAttrValue = "";
	if item ~= nil and item:getItemType() == ITEM_SLOTTYPE_AMM then
		nAddAttrValue = item:getArmStarAddValue( nResultId );
		if nAddAttrValue > 0 then
			nAddAttrValue = FormatItemAddAttrValue( { ["value"] = nAddAttrValue, ["resultID"] = nResultId } );
			nAddAttrValue =  "#c42a83d+"..nAddAttrValue.."%#n";
		else
			nAddAttrValue = "";
		end
	end
	
	local szAttrValue = nValue..nAddAttrValue;
	return  szAttrValue;
end

-- ��ȡ���۸��̵�ļ۸�
function GetSell2ShopPrice( item, itemDef )
	if item ~= nil then
		return item:getSellPrice();
	end
	return itemDef.SellPrice;
end

function GetNewBaseAttrColor( nCurVal,nMaxVal )
	local color
	if nCurVal < nMaxVal then
		color = GetColor()[COLOR_WHITE];
	else
		color = GetColor()[COLOR_PURPLE];		
	end
	return string.format("#c%02x%02x%02x",color[1],color[2],color[3])
end

function GetNewBaseAttrValue( Result, item )
	local mul = 0;
	if item ~= nil then
		mul = item:getItemMulVal( Result.ResultID );
	end

	return Result.ResultVal1 + math.floor(Result.ResultVal2*mul/100)
end

function GetItemNewBaseAttrDesc( itemdef, item )
	local szDesc = "";
	local szValue = "";
	local nResult = 0;
	local t_BaseAttr = {}
	local maxPhsicAtk,minPhsicAtk,maxMagicAtk,minMagicAtk,maxAtk,minAtk = 0,0,0,0,0,0;
	local maxMaxPhsicAtk,maxMinPhsicAtk,maxMaxMagicAtk,maxMinMagicAtk,maxMaxAtk,maxMinAtk = 0,0,0,0,0,0;
	for i=1,MAX_ITEM_RESULT do
		if IsEquipNewBaseAttr( itemdef.Result[i-1].ResultID ) then
			local maxColor =  GetColor()[ COLOR_PURPLE ];
			local itemResultConfig = GetOneItemResultConfig( { ["resultID"] = itemdef.Result[i-1].ResultID } )
			local maxVal = itemdef.Result[i-1].ResultVal1 + itemdef.Result[i-1].ResultVal2;
			if itemResultConfig ~= nil then
				local val = GetNewBaseAttrValue( itemdef.Result[i-1],item )				
				local valColor = GetColor()[ val == maxVal and COLOR_PURPLE or COLOR_WHITE ];	

				local szVal
				if itemResultConfig[1] == RESULT_BASE_ATTR_HEAVY_HIT_VAL then
					szVal = string.format( "#c%02x%02x%02x%.2f%%#n #c%02x%02x%02x(%.2f%%)#n",
						valColor[1], valColor[2], valColor[3], val/100, maxColor[1], maxColor[2], maxColor[3], maxVal/100 );
				else
					szVal = string.format( "#c%02x%02x%02x%d#n #c%02x%02x%02x(%d)#n",
						valColor[1], valColor[2], valColor[3], val, maxColor[1], maxColor[2], maxColor[3], maxVal );
				end				
				table.insert( t_BaseAttr,{ itemResultConfig[2], szVal,itemdef.Result[i-1].ResultID } )
			else
				if itemdef.Result[i-1].ResultID == RESULT_WEAPON_MIN_PHSIC_ATK then
					minPhsicAtk = GetNewBaseAttrValue( itemdef.Result[i-1], item )							
					maxPhsicAtk = maxPhsicAtk > minPhsicAtk and maxPhsicAtk or minPhsicAtk
					maxMinPhsicAtk = maxVal					
				elseif itemdef.Result[i-1].ResultID == RESULT_WEAPON_MAX_PHSIC_ATK then
					maxPhsicAtk = GetNewBaseAttrValue( itemdef.Result[i-1], item )							
					minPhsicAtk = minPhsicAtk < maxPhsicAtk and minPhsicAtk or maxPhsicAtk
					maxMaxPhsicAtk = maxVal			
				elseif itemdef.Result[i-1].ResultID == RESULT_WEAPON_MIN_MAGIC_ATK then
					minMagicAtk = GetNewBaseAttrValue( itemdef.Result[i-1], item )							
					maxMagicAtk = maxMagicAtk > minMagicAtk and maxPhsicAtk or minMagicAtk
					maxMinMagicAtk = maxVal
				elseif itemdef.Result[i-1].ResultID == RESULT_WEAPON_MAX_MAGIC_ATK then
					maxMagicAtk = GetNewBaseAttrValue( itemdef.Result[i-1], item )
					minMagicAtk = minMagicAtk < maxMagicAtk and minMagicAtk or maxMagicAtk
					maxMaxMagicAtk = maxVal
				elseif itemdef.Result[i-1].ResultID == RESULT_WEAPON_MIN_ATK then
					minAtk = GetNewBaseAttrValue( itemdef.Result[i-1], item )							
					maxAtk = maxAtk > minAtk and maxAtk or minAtk
					maxMinAtk = maxVal
				elseif itemdef.Result[i-1].ResultID == RESULT_WEAPON_MAX_ATK then
					maxAtk = GetNewBaseAttrValue( itemdef.Result[i-1], item )
					minAtk = minAtk < maxAtk and minAtk or maxAtk
					maxMaxAtk = maxVal
				end
				if maxPhsicAtk > 0 then
					local minValColor = GetColor()[ minPhsicAtk == maxMinPhsicAtk and COLOR_PURPLE or COLOR_WHITE ]
					local maxValColor = GetColor()[ maxPhsicAtk == maxMaxPhsicAtk and COLOR_PURPLE or COLOR_WHITE ]
					local signColor = GetColor()[ ( minPhsicAtk == maxMinPhsicAtk and maxPhsicAtk == maxMaxPhsicAtk ) and COLOR_PURPLE or COLOR_WHITE ]
					szValue = string.format( "#c%02x%02x%02x%d#n#c%02x%02x%02x-#n#c%02x%02x%02x%d#n#c%02x%02x%02x(%d-%d)#n",
					minValColor[1], minValColor[2], minValColor[3], minPhsicAtk, signColor[1], signColor[2], signColor[3],
					maxValColor[1], maxValColor[2], maxValColor[3], maxPhsicAtk, maxColor[1], maxColor[2], maxColor[3],
					maxMinPhsicAtk,maxMaxPhsicAtk )
					szDesc = "������"
					nResult = RESULT_WEAPON_MAX_PHSIC_ATK
				end
				if maxMagicAtk > 0 then
					local minValColor = GetColor()[ minMagicAtk == maxMinMagicAtk and COLOR_PURPLE or COLOR_WHITE ]
					local maxValColor = GetColor()[ maxMagicAtk == maxMaxMagicAtk and COLOR_PURPLE or COLOR_WHITE ]
					local signColor = GetColor()[ ( minMagicAtk == maxMinMagicAtk and maxMagicAtk == maxMaxMagicAtk ) and COLOR_PURPLE or COLOR_WHITE ]
					szValue = string.format( "#c%02x%02x%02x%d#n#c%02x%02x%02x-#n#c%02x%02x%02x%d#n#c%02x%02x%02x(%d-%d)#n",
					minValColor[1], minValColor[2], minValColor[3], minMagicAtk, signColor[1], signColor[2], signColor[3],
					maxValColor[1], maxValColor[2], maxValColor[3], maxMagicAtk, maxColor[1], maxColor[2], maxColor[3],
					maxMinMagicAtk,maxMaxMagicAtk )
					szDesc = "ħ������";
					nResult = RESULT_WEAPON_MAX_MAGIC_ATK
				end
				if maxAtk > 0 then
					local minValColor = GetColor()[ minAtk == maxMinAtk and COLOR_PURPLE or COLOR_WHITE ]
					local maxValColor = GetColor()[ maxAtk == maxMaxAtk and COLOR_PURPLE or COLOR_WHITE ]
					local signColor = GetColor()[ ( minAtk == maxMinAtk and maxAtk == maxMaxAtk ) and COLOR_PURPLE or COLOR_WHITE ]
					szValue = string.format( "#c%02x%02x%02x%d#n#c%02x%02x%02x-#n#c%02x%02x%02x%d#n#c%02x%02x%02x(%d-%d)#n",
					minValColor[1], minValColor[2], minValColor[3], minAtk, signColor[1], signColor[2], signColor[3],
					maxValColor[1], maxValColor[2], maxValColor[3], maxAtk, maxColor[1], maxColor[2], maxColor[3],
					maxMinAtk,maxMaxAtk )
					szDesc = "˫ϵ����";
					nResult = RESULT_WEAPON_MAX_ATK
				end
			end
		end
	end
	if szDesc ~= "" and szValue ~= "" then
		table.insert( t_BaseAttr,{ szDesc,szValue,nResult } )
	end
	return t_BaseAttr;
end

function FillAllEquipBaseAttr( data )
	local item			= data["item"];
	local t_EquipInfo	= data["equipCache"];
	local isShowTitle	= data["isShowTitle"]
	local itemDef;
	if item ~= nil then
		itemDef	= item:getItemDef();
	else
		itemDef	= data["itemDef"];
	end
	
	if itemDef == nil then
		return;
	end

	local tNewBaseAttrDesc = GetItemNewBaseAttrDesc( itemDef,item )
	for i=1,table.getn( tNewBaseAttrDesc ) do
		if isShowTitle then
			local t_oneEquipElem = { FIGHT_AREA, "", "�������ԣ�", GetColor()[COLOR_OGRANGE], LINE_TYPE_TEXT, false };	
			table.insert( t_EquipInfo, t_oneEquipElem );
			isShowTitle = false;
		end
		local t_oneEquipElem = {	FIGHT_AREA, "\b" .. tNewBaseAttrDesc[i][1] ..":",
									GetBaseAttrValue( item, tNewBaseAttrDesc[i][2], tNewBaseAttrDesc[i][3] ) .. "\n",
									GetColor()[COLOR_WHITE], LINE_TYPE_ATTRADD, false } ;
		table.insert( t_EquipInfo, t_oneEquipElem );
	end

	for i = 1, MAX_ITEM_RESULT do
		local oneResult		= itemDef.Result[i-1]
		local itemResultID	= oneResult.ResultID;
		local szValue ;
		if itemResultID ~= RESULT_NONE and not IsEquipNewBaseAttr( itemResultID ) then
			szValue = GetOneItemResultFirstEffectValue( oneResult )	
			isShowTitle = FillOneEquipBaseAttr( {	["resultID"]	= itemResultID, ["value"]		= szValue, 
												["isShowTitle"] = isShowTitle,	["equipCache"]	= data["equipCache"], 
												["item"]		= item } )	
		end
	end
end

function FillOneEquipBaseAttr( data )
	local itemResultID		= data["resultID"];
	local itemResultValue	= data["value"];
	local isShowTitle		= data["isShowTitle"]
	local t_EquipInfo		= data["equipCache"];
	local item				= data["item"];
	if itemResultID == RESULT_NONE then
		return isShowTitle;
	end

	local t_oneItemResultConfig = GetOneItemResultConfig( { ["resultID"] = itemResultID } );
	if t_oneItemResultConfig == nil then
		return isShowTitle;
	end
	
	if isShowTitle then
		local t_oneEquipElem = { FIGHT_AREA, "", "�������ԣ�", GetColor()[COLOR_OGRANGE], LINE_TYPE_TEXT, false };	
		table.insert( t_EquipInfo, t_oneEquipElem );
		isShowTitle = false;
	end
	
	local szValueDesc = FormatItemAttrValue( { ["value"] = itemResultValue, ["resultID"] = itemResultID } );
	local t_oneEquipElem = {	FIGHT_AREA, "\b"..t_oneItemResultConfig[2]..":",
								GetBaseAttrValue( item, szValueDesc, itemResultID ),
								GetColor()[COLOR_WHITE], LINE_TYPE_ATTRADD, false } ;
	table.insert( t_EquipInfo, t_oneEquipElem );

	return isShowTitle;
end

-- ��ȡ�¹ؼ��ֲ�λ����
function getNewAddAttrMul( newAddAttrDef, wearpos )
	if wearpos == EQUIP_POS_RIGHTHAND then
		return newAddAttrDef.WearPosLimit.RightHand
	elseif wearpos == EQUIP_POS_HEAD then
		return newAddAttrDef.WearPosLimit.Head
	elseif wearpos == EQUIP_POS_BODY then
		return newAddAttrDef.WearPosLimit.Body
	elseif wearpos == EQUIP_POS_LEFT then
		return newAddAttrDef.WearPosLimit.LeftHand
	elseif wearpos == EQUIP_POS_FACE then
		return newAddAttrDef.WearPosLimit.Face
	elseif wearpos == EQUIP_POS_SHOULDER then
		return newAddAttrDef.WearPosLimit.Shoulder
	elseif wearpos == EQUIP_POS_SHOE then
		return newAddAttrDef.WearPosLimit.Shoe
	elseif wearpos == EQUIP_POS_AMULET then
		return newAddAttrDef.WearPosLimit.Amulet
	elseif wearpos == EQUIP_POS_RING or wearpos == EQUIP_POS_RING1 or wearpos == EQUIP_POS_RING2 or wearpos == EQUIP_POS_RING3 then
		return newAddAttrDef.WearPosLimit.Ring
	elseif wearpos == EQUIP_POS_NECKLACE then
		return newAddAttrDef.WearPosLimit.NeckLace
	end
	return 0
end

-- �Ƿ���ʱװ
function IsFashionItem( itemdef )
	if itemdef.WearPos1 == EQUIP_POS_FASHION_BODY or itemdef.WearPos1 == EQUIP_POS_FASHION_HEAD 
		or itemdef.WearPos1 == EQUIP_POS_FASHION_HAIR or itemdef.WearPos1 == EQUIP_POS_FASHION_BACK
		or itemdef.WearPos1 == EQUIP_POS_FASHION_SHOES or itemdef.WearPos1 == EQUIP_POS_FASHION_HAND
		or itemdef.WearPos1 == EQUIP_POS_FASHION_LEG then
		
		return true;
	end
	return false;
end

local MAX_STAR_MUL_LIST = 10

function getArmDefMinAndMaxMul( armStarDef )
	local mul = armStarDef.StarInfo[0].Mul;
	local minMul = mul;
	local maxMul = mul;
	for i = 1, MAX_STAR_MUL_LIST do
		if armStarDef.StarInfo[i-1].Mul > maxMul then
			maxMul = armStarDef.StarInfo[i-1].Mul;
		end
		if armStarDef.StarInfo[i-1].Mul < minMul then
			minMul = armStarDef.StarInfo[i-1].Mul;
		end
	end
	return minMul, maxMul;
end

--��ȡװ����Ϣ
function GetEquipInfo( item, itemDef, nTipsType )
	local szItemName = itemDef.Name;
	if item ~= nil then
		szItemName = item:getDisplayName();
	else
		szItemName = getDisplayName(itemDef);
	end

	if itemDef == nil then
		return;
	end

	local t_NeedChangeToValue = {};
	if itemDef.ItemLevel ~= 0 then	 
		local AddAttrBaseDef = getAddAttrBaseDef( itemDef.ItemLevel );
		t_NeedChangeToValue = 
		{ 
			{ ADD_ATTR_RESULT_HITRATE,			AddAttrBaseDef.HitRateBase	},		-- /* ������ */
			{ ADD_ATTR_RESULT_PHSIC_ATK,		AddAttrBaseDef.PhsicAtkBase	},		-- /* ������������ */
			{ ADD_ATTR_RESULT_ICE_ATK,			AddAttrBaseDef.IceAtkBase	},		-- /* ħ������������ */
			{ ADD_ATTR_RESULT_PHSIC_DEF,		AddAttrBaseDef.PhsicDefBase	},		-- /* ��� */
			{ ADD_ATTR_RESULT_ICE_DEF,			AddAttrBaseDef.IceDefBase	},		-- /* ħ�� */
			{ ADD_ATTR_RESULT_ADD_DEF,			AddAttrBaseDef.IceDefBase	},		-- /* ˫�� */
			{ ADD_ATTR_RESULT_ATK,				AddAttrBaseDef.PhsicAtkBase	},		-- /* ���������� */
			{ ADD_ATTR_RESULT_DEF_PHISICHURT,	AddAttrBaseDef.DefPhsicHurtBase},	-- /* �����˺����� */
			{ ADD_ATTR_RESULT_DEF_ICEHURT,		AddAttrBaseDef.DefIceHurtBase},		-- /* ħ���˺����� */
			{ ADD_ATTR_RESULT_DEF_ICE_PHISIC,	AddAttrBaseDef.DefIceHurtBase},		-- /* ˫�� */
			{ ADD_ATTR_RESULT_FLEE,				AddAttrBaseDef.FleeBase		},		-- /* ���ܵȼ� */
			{ ADD_ATTR_RESULT_HP,				AddAttrBaseDef.MaxHPBase	},		-- /* ���Ѫ������ֵ */
			{ ADD_ATTR_RESULT_MP,				AddAttrBaseDef.MaxMPBase	},		-- /* ���ħ������ֵ */
			{ ADD_ATTR_RESULT_LILIANG,			AddAttrBaseDef.LiLiang		},		-- /* ���� */
			{ ADD_ATTR_RESULT_ZHILI,			AddAttrBaseDef.LiLiang		},		-- /* ���� */
			{ ADD_ATTR_RESULT_TIZHI,			AddAttrBaseDef.TiZhi		},		-- /* ���� */
			{ ADD_ATTR_RESULT_FANGHU,			AddAttrBaseDef.FangHu		},		-- /* ���� */
			{ ADD_ATTR_RESULT_JINGSHEN,			AddAttrBaseDef.JingShen		},		-- /* ���� */
			{ ADD_ATTR_RESULT_MINJIE,			AddAttrBaseDef.MinJie		},		-- /* ���� */
			{ ADD_ATTR_RESULT_BASE_ATTR_HEAVYHIT,AddAttrBaseDef.HeavyHit	},		-- /* �����ȼ� */
			{ ADD_ATTR_RESULT_DEF_HEAVY_HIT,	AddAttrBaseDef.DefHeavyHit	},		-- /* �����ֿ� */
			{ ADD_ATTR_RESULT_WATERATK,			AddAttrBaseDef.WaterAtk		},		-- /* ˮ���� */
			{ ADD_ATTR_RESULT_WATERDEF,			AddAttrBaseDef.WaterDef		},		-- /* ˮ���� */
			{ ADD_ATTR_RESULT_FIREATK,			AddAttrBaseDef.FireAtk		},		-- /* �𹥻� */
			{ ADD_ATTR_RESULT_FIREDEF,			AddAttrBaseDef.FireDef		},		-- /* ����� */
			{ ADD_ATTR_RESULT_THUNDERATK,		AddAttrBaseDef.ThunderAtk	},		-- /* �׹��� */
			{ ADD_ATTR_RESULT_THUNDERDEF,		AddAttrBaseDef.ThunderDef	},		-- /* �׷��� */
			{ ADD_ATTR_RESULT_LIGHTATK,			AddAttrBaseDef.LightAtk		},		-- /* �⹥�� */
			{ ADD_ATTR_RESULT_LIGHTDEF,			AddAttrBaseDef.LightDef		},		-- /* ����� */
			{ ADD_ATTR_RESULT_NIGHTATK,			AddAttrBaseDef.NightAtk		},		-- /* ������ */
			{ ADD_ATTR_RESULT_NIGHTDEF,			AddAttrBaseDef.NightDef		},		-- /* ������ */

			{ ADD_ATTR_RESULT_IGNORE_DEF,		AddAttrBaseDef.IgnoreDef	},		-- /* ���ӷ�����ֵ(��ʽ���ʵ�ʺ��ӵķ���) */
			{ ADD_ATTR_RESULT_REFLECTION_HP,	AddAttrBaseDef.ReflectHP	},		-- /* ������ɵ�Ѫ����ʧֵ(��ʽ���ʵ��ֵ) */
			{ ADD_ATTR_RESULT_DEF_HP,			AddAttrBaseDef.DefHP		},		-- /* �ֿ���ɵ�Ѫ���˺���ֵ(��ʽ���ʵ��ֵ) */			
		};
	end
	
	local t_oneEquipElem;
	local t_EquipInfo = {
		{ BASE_AREA1, "",					szItemName,							GetItemNameColor(itemDef),		LINE_TYPE_HEADTEXT, true },	--װ������
		{ BASE_AREA1, "",					GetArmBind( item,itemDef ),			GetColor()[COLOR_ITEMSTART],	LINE_TYPE_BINDTEXT, false },--��Ʒ��ǰ��״��
		{ BASE_AREA1, "װ���ȼ���",			GetArmWearLv(itemDef, item).."��",	GetUseLvColr(itemDef, item),	 LINE_TYPE_TEXT, true },			--װ���ȼ�
		{ BASE_AREA1, "",					GetDur(item, itemDef),				GetColor()[COLOR_WHITE], LINE_TYPE_TEXT, false },			--�;ö�
		{ BASE_AREA1, "ְҵ����",			GetCareer(itemDef.WearPre.Career),	GetCareerColor(itemDef), LINE_TYPE_TEXT, false },			--ְҵ GetCareer(itemDef.WearPre.Career)
		{ BASE_AREA1, "",					"װ�����ͣ�"..GetItemType(itemDef.ItemID),	GetColor()[COLOR_WHITE], LINE_TYPE_TEXT, false },			--װ������ GetItemType(itemDef.ItemID)
		{ BASE_AREA1, "",			GetItemRespact(itemDef),			GetRespacColor(itemDef), LINE_TYPE_TEXT, false },
	}
	if OffMgr:bitAnd( itemDef.CtrlFlag, ITEM_NO_LEVELLIMIT ) then
		table.remove( t_EquipInfo, 3 )
	end
	
	if item ~= nil then
		local szInfo = item:getStart()..",";
		if item:getStart() > 0 and item:getStart() <= ARM_STAR_LIMIT then
			local armStarDef = getArmStarDef( item:getStart() );
			local minMul, maxMul = getArmDefMinAndMaxMul( armStarDef );
			for i = 1, item:getStart() do
				local mulValIdx = item:getArmAddStarValIdx( i - 1 );
				if mulValIdx == nil then
					break;
				end
				if mulValIdx >= 0 and mulValIdx <= MAX_STAR_MUL_LIST then
					local mul = armStarDef.StarInfo[mulValIdx].Mul;
					if mul >= maxMul then
						szInfo = szInfo.."1"..",";
					else
						szInfo = szInfo.."0"..",";
					end
				else
					break;
				end
			end
		end
		local t_oneEquipElem = { BASE_AREA1, "", szInfo, GetColor()[COLOR_WHITE], LINE_TYPE_TEXTURE, false }	;
		table.insert( t_EquipInfo, t_oneEquipElem );
	end

	local bShowTitle = true;
	if itemDef.IdentifiabilityType == 0 then
		bShowTitle = FillAllEquipBaseAttr( { ["item"] = item, ["itemDef"] = itemDef, ["equipCache"] = t_EquipInfo, ["isShowTitle"] = bShowTitle } );

		-- װ���ؼ���
		if item ~= nil then
			local nAddAttrNum	= item:getArmAddAttrNum();
			local bShow			= true;
			for i = 1, nAddAttrNum do
				local AddAttrInst	= item:getArmAddAttr(i-1);
				local AddAttrID		= AddAttrInst.AddAttrID;
				local AddAttrValue	= AddAttrInst.Val; 
				local newAddAttrDef = getNewAddAttrDef( AddAttrID );
				local addattrRandDef = getAddAttrRandDef( math.floor(itemDef.WearPre.RoleLevelMin/10), itemDef.Important )
				if newAddAttrDef ~= nil and addattrRandDef ~= nil then
					local base = newAddAttrDef.Val1[ addattrRandDef.ValLevel - 1 ];
					local mul = getNewAddAttrMul( newAddAttrDef, itemDef.WearPos1 );
					if mul > 0 then
						if bShow then
							local t_oneEquipElem = { FIGHT_AREA,	"", " ",	GetColor()[COLOR_ITEMSTART], LINE_TYPE_DISTANCE, true};
							table.insert( t_EquipInfo, t_oneEquipElem );
							-- ��������
							t_oneEquipElem ={ FIGHT_AREA, "",	"�������ԣ�",	GetColor()[COLOR_OGRANGE], LINE_TYPE_TEXT, false };
							table.insert( t_EquipInfo, t_oneEquipElem );
							bShow	= false;
						end
						AddAttrValue = base*AddAttrValue/100*mul/100 
						if itemDef.ItemLevel > 0 then
							local tAddattrVal = { AddAttrValue, newAddAttrDef.Val2, newAddAttrDef.Val3, newAddAttrDef.Val4 }
							local t_oneEquipElem = {	FIGHT_AREA, "\b", getNewAttrDesc( AddAttrID, tAddattrVal ),
										GetColor()[COLOR_ITEMKEYWORLD], LINE_TYPE_TEXT, false };
							table.insert( t_EquipInfo, t_oneEquipElem );
						end
					end
				end
			end
			-- ϴ������
			for i=1,item:getArmWashAddAttrNum() do
				local AddAttrInst	= item:getArmWashAddAttrInst(i-1);
				local AddAttrID		= AddAttrInst.AddAttrID;
				local AddAttrValue	= AddAttrInst.Val; 
				local newAddAttrDef = getNewAddAttrDef( AddAttrID );
				local addattrRandDef = getAddAttrRandDef( math.floor(itemDef.WearPre.RoleLevelMin/10), itemDef.Important )
				if newAddAttrDef ~= nil and addattrRandDef ~= nil then
					local base = newAddAttrDef.Val1[ addattrRandDef.ValLevel - 1 ];
					local mul = getNewAddAttrMul( newAddAttrDef, itemDef.WearPos1 );
					if mul > 0 then
						if bShow then
							local t_oneEquipElem = { FIGHT_AREA,	"", " ",	GetColor()[COLOR_ITEMSTART], LINE_TYPE_DISTANCE, true};
							table.insert( t_EquipInfo, t_oneEquipElem );
							-- ��������
							t_oneEquipElem ={ FIGHT_AREA, "",	"�������ԣ�",	GetColor()[COLOR_OGRANGE], LINE_TYPE_TEXT, false };
							table.insert( t_EquipInfo, t_oneEquipElem );
							bShow	= false;
						end
						AddAttrValue = base*AddAttrValue/100*mul/100 
						if itemDef.ItemLevel > 0 then
							local tAddattrVal = { AddAttrValue, newAddAttrDef.Val2, newAddAttrDef.Val3, newAddAttrDef.Val4 }
							local t_oneEquipElem = {	FIGHT_AREA, "\b", getNewAttrDesc( AddAttrID, tAddattrVal ),
										{ 0,200,255 }, LINE_TYPE_TEXT, false };
							table.insert( t_EquipInfo, t_oneEquipElem );
						end
					end
				end
			end

			--��ħ����
			local ExtendedFromDefID = item:getExtendedFromDefID();
			local extendedResult = item:getExtendedResult();			
			if ExtendedFromDefID > 0 and extendedResult ~= nil and extendedResult.ResultID ~= 0 then
				local itemDef = getItemDef( ExtendedFromDefID );

				local t_oneEquipElem = { FIGHT_AREA,	"", " ",	GetColor()[COLOR_ITEMSTART], LINE_TYPE_DISTANCE, true};
				table.insert( t_EquipInfo, t_oneEquipElem );
				
				--��ħ����
				t_oneEquipElem = { FIGHT_AREA, "",	"��ħ���ԣ�",	GetColor()[COLOR_OGRANGE], LINE_TYPE_TEXT, false };	
				table.insert( t_EquipInfo, t_oneEquipElem );

				--��ħΪװ��Ч��
				if itemDef.ExtendedType == ITEM_EXTENDED_TYPE_RESULT then				
					for j = 1, table.getn(t_tagRESULT) do
						if t_tagRESULT[j][1] == extendedResult.ResultID then
							local t_ResultValue = { extendedResult.ResultVal1,extendedResult.ResultVal2,extendedResult.ResultVal3,extendedResult.ResultVal4};
							local nValue = 0;
							for k = 1, table.getn(t_ResultValue) do
								if t_ResultValue[k] ~= 0 then
									nValue = t_ResultValue[k];
									break;
								end
							end
							
							local t_oneEquipElem = { FIGHT_AREA, "\b"..t_tagRESULT[j][2].."��", "+" .. nValue ,GetColor()[COLOR_ITEMKEYWORLD],	
														LINE_TYPE_TEXT, false };
							table.insert( t_EquipInfo, t_oneEquipElem );
						end					
					end				
				end
			end
			--��ӡ����
			local addAttrInst = item:getAddAttrInst();
			if addAttrInst.AddAttrID > 0 then
				table.insert( t_EquipInfo, { FIGHT_AREA, "",	"Ԫ��ӡ�ǣ�",	GetColor()[COLOR_OGRANGE], LINE_TYPE_TEXT, false } );
				for j = 1, table.getn(t_tagRESULT) do
					if t_tagRESULT[j][1] == addAttrInst.AddAttrID then						
						local nValue = addAttrInst.Val;						
						local t_oneEquipElem = { FIGHT_AREA, "\b"..t_tagRESULT[j][2].."��", "+" .. nValue ,GetColor()[COLOR_WEAK_BLUE],	
													LINE_TYPE_TEXT, false };
						table.insert( t_EquipInfo, t_oneEquipElem );
					end					
				end
				table.insert( t_EquipInfo, t_oneEquipElem );
			end
			--�Ǽ�����
			local nPerfectStar = item:getArmPefectStarNum();
			if nPerfectStar >= 3 then
				local armGradeDef = getArmGradeDef( itemid_num( item:getItemDef() ) ,math.floor( item:getItemDef().WearPre.RoleLevelMin/10 ) );
				if armGradeDef ~= nil then
					local nAttrId = armGradeDef.ArmStar[nPerfectStar-1].SuitAttrID;
					if nAttrId ~= 0 then
						local t_oneEquipElem = { FIGHT_AREA, "",	"�Ǽ����ԣ�",	GetColor()[COLOR_OGRANGE], LINE_TYPE_TEXT, false };	
						table.insert( t_EquipInfo, t_oneEquipElem );
						local AddAttrDef = getAddAttrDef( nAttrId );
						t_oneEquipElem = { FIGHT_AREA,	"\b", nPerfectStar.."������ǿ����"..AddAttrDef.AddAttrDesc,GetColor()[COLOR_ITEMKEYWORLD], LINE_TYPE_TEXT, false };		
						table.insert( t_EquipInfo, t_oneEquipElem );
					end
				end
			end
		else
			if itemDef.Important > 0 then
				table.insert( t_EquipInfo,{ FIGHT_AREA, "",	"�������ԣ�",	GetColor()[COLOR_OGRANGE], LINE_TYPE_TEXT, false } );
				table.insert( t_EquipInfo,{ FIGHT_AREA, "\b",	"�������",	GetColor()[COLOR_ITEMKEYWORLD], LINE_TYPE_TEXT, false } );
			end
		end
		
		if item ~= nil and item:getHoleNum() > 0 then
			local t_oneEquipElem = { FIGHT_AREA,	"", "",	GetColor()[COLOR_ITEMSTART], LINE_TYPE_DISTANCE, true};
			table.insert( t_EquipInfo, t_oneEquipElem );

			t_oneEquipElem = { FIGHT_AREA,	"��ʯ��Ƕ״̬", "",	GetColor()[COLOR_ITEMSTART], LINE_TYPE_TEXT, true};
			table.insert( t_EquipInfo, t_oneEquipElem );

			for i = 1, item:getHoleNum() do
				local nGemId		= item:getHoleGemId(i-1);
				local nResultIndex	= item:getHoleGemResultIndex( i - 1 );
				local nHoleColor	= item:getHoleGemColor(i-1);
				if nGemId ~= 0 then	  
					local gemitemdef = getItemDef( nGemId );
					local t_oneEquipElem = { FIGHT_AREA,	GetGemColorName(nHoleColor).."���\b",	nGemId..","..gemitemdef.IconID,	GetColor()[COLOR_WEAK_PURPLE], LINE_TYPE_GEM_TEXTURE, true, nResultIndex };		--����
					table.insert( t_EquipInfo, t_oneEquipElem );
				else
					local t_oneEquipElem = { FIGHT_AREA,	GetGemColorName(nHoleColor).."���\b",	"δ��Ƕ",	GetColor()[COLOR_ITEMGEMINLAY], LINE_TYPE_GEM_TEXTURE, true };		--����
					table.insert( t_EquipInfo, t_oneEquipElem );
				end
			end		
		end

		if item ~= nil then
			--ս����
			local t_oneEquipElem ={ BATTLE_AREA, "ս������", item:getFight(),	GetColor()[COLOR_BATTLE], LINE_TYPE_TEXT, false };		
			table.insert( t_EquipInfo, t_oneEquipElem );
		end

		local nSuitNum	= 0;
		local nWatchSuitNum = 0;
		local watchplayersuit = "";

		if itemDef.SuitSetID > 0 then
			local SuitSet = getSuitSet( itemDef.SuitSetID );

			watchplayersuit = "";
			for i = 1, MAX_ITEM_SUITSETATTR do
				if SuitSet.SuitAttr[i-1].SuitAttrID > 0 then
					local AddAttrDef = getAddAttrDef( SuitSet.SuitAttr[i-1].SuitAttrID );
					local SuitAttrColor = COLOR_SUITNOGET;
					if nSuitNum >= SuitSet.SuitAttr[i-1].MinSuitNum then
						SuitAttrColor = COLOR_ITEMKEYWORLD;
					end

					if onEnterBtn ~= nil and onEnterBtn ~= "" then
						if string.find( onEnterBtn,"WatchPlayerFrameEquip", 1, true ) then
							watchplayersuit = "#WPSUIT#"..COLOR_SUITNOGET.."#";
						end

						if nWatchSuitNum >= SuitSet.SuitAttr[i-1].MinSuitNum and string.find( onEnterBtn,"WatchPlayerFrameEquip", 1, true ) then
							watchplayersuit = "#WPSUIT#"..COLOR_ITEMKEYWORLD.."#";
						end
					end
					
					local t_oneEquipElem = { SUIT_AREA,	watchplayersuit.."\b"..SuitSet.SuitAttr[i-1].MinSuitNum.."����", AddAttrDef.AddAttrDesc,	GetColor()[SuitAttrColor], LINE_TYPE_TEXT, false };		
					table.insert( t_EquipInfo, t_oneEquipElem );
				end
			end
			--���
			local t_oneEquipElem = { SUIT_AREA, "", "", GetColor()[COLOR_ITEMSTART], LINE_TYPE_DISTANCE, true};			
			table.insert( t_EquipInfo, t_oneEquipElem );
		end
		--�Ƿ�ɽ���
		local t_oneEquipElem = { SUIT_AREA,	GetCanExChange( item, itemDef ), "", GetColor()[COLOR_ITEMSTART], LINE_TYPE_TEXT, false };	
		table.insert( t_EquipInfo, t_oneEquipElem );

--		t_oneEquipElem = { SUIT_AREA, "���ǿ���ȼ���", itemDef.StarLimit, GetColor()[COLOR_ITEMSTART], LINE_TYPE_TEXT, false };
--		table.insert( t_EquipInfo, t_oneEquipElem );

		t_oneEquipElem = { SUIT_AREA,	"װ����λ��",	GetWearPos(itemDef), GetColor()[COLOR_ITEMSTART], LINE_TYPE_TEXT, false };
		table.insert( t_EquipInfo, t_oneEquipElem );
		
		local szEndTimeDesc = SetItemEndTime(item );
		if szEndTimeDesc ~= "" then
			t_oneEquipElem = { SUIT_AREA, "��Ч����", szEndTimeDesc, GetColor()[COLOR_BATTLE], LINE_TYPE_TEXT, true };
			table.insert( t_EquipInfo, t_oneEquipElem );
		end

		t_oneEquipElem = { SUIT_AREA,		"�̵��չ��ۣ�",			GetSell2ShopPrice( item, itemDef ),		GetColor()[COLOR_SELLPRICE], LINE_TYPE_MONEY, false };			
		table.insert( t_EquipInfo, t_oneEquipElem );

		if itemDef.Important >= 1 and itemDef.WearPos1 ~= EQUIP_POS_FASHION and 
			( isPointInFrame( "PlayerContainerFrame" ) or isPointInFrame( "EquipFrame" ) ) then
			t_oneEquipElem = {SUIT_AREA, "",	"<shift+�Ҽ�>����Ƕ����",	GetColor()[COLOR_GREEN],LINE_TYPE_TEXT,false};
			table.insert( t_EquipInfo, t_oneEquipElem );
		end

		-- nIndex+5
		if itemDef.NoResolve == 0 then
			t_oneEquipElem = {SUIT_AREA, "", "���Էֽ�", GetColor()[COLOR_GREEN],LINE_TYPE_TEXT,false};
			table.insert( t_EquipInfo, t_oneEquipElem );
		else
			t_oneEquipElem = {SUIT_AREA, "", "���ɷֽ�", GetColor()[COLOR_GREEN],LINE_TYPE_TEXT,false};
			table.insert( t_EquipInfo, t_oneEquipElem );
		end		
	end

	if itemDef.IdentifiabilityType ~= 0 then

		t_oneEquipElem = { FIGHT_AREA, "", "δ����", GetColor()[COLOR_WEAK_PURPLE],	LINE_TYPE_TEXT, true };
		table.insert( t_EquipInfo, t_oneEquipElem );

		t_oneEquipElem = { FIGHT_AREA, "�̵��չ���", itemDef.SellPrice, GetColor()[COLOR_SELLPRICE], LINE_TYPE_MONEY, false };
		table.insert( t_EquipInfo, t_oneEquipElem );
	end

	return t_EquipInfo;
end

function CanDropItem(value)
	if value == 1 then
		return "���ɶ���"
	end
	return "";
end

function CanSaleItem(value)
	if value == 1 then
		return "���ɳ���"
	end
	return "";
end

function GetCanDealItem(value)
	if value == 1 then
		szDesc = "���ɽ���";
	end
	return "";
end

function GetItemDesc( item,itemDef )
	local szText = itemDef.ItemDesc; 
	if item ~= nil then
		for i = 1,MAX_ITEM_RESULT do
			if itemDef.Result[i-1].ResultID ==  RESULT_TREASURE_PIC then
				if string.find( szText, "@mapid@", 1, true ) and ( item:getItemUsedMapId() ~= -1 ) then
					local szMapName = MapShower:GetMinMapContal():getMapName( item:getItemUsedMapId() );
					if item:getItemUsedMapId() == 0 then
						szMapName = "δ֪����";
					end
					szText = string.gsub(szText, "@mapid@", szMapName);
				end
				break;
			end
		end
	end
	return szText;
end

function GetPetSkillType( itemDef )
	if itemDef ~= nil then		
		local t_PetSkillDesc = { [1] = "������", [2] = "������", [3] = "��ǿ��" };
		for i = 1,MAX_ITEM_RESULT do
			if itemDef.Result[i-1].ResultID ==  RESULT_PET_SKILL_BOOK then
				local DiathesisDef = SkillMgr:getDiathesisDef( itemDef.Result[i-1].ResultVal1, 1 );
				if DiathesisDef ~= nil then
					if t_PetSkillDesc[DiathesisDef.PetDiathesisType] == nil then
						return "";
					end
					return t_PetSkillDesc[DiathesisDef.PetDiathesisType];
				end
			end
		end
	end
	return "";
end

--��ȡ��Ʒ��Ϣ  
function GetToolsInfo( item, itemDef,nTipsType, isShopItem )
	local szItemName = itemDef.Name;
	local szItemSurplus = "";
	local szSurplusColor = COLOR_WHITE;
	if item ~= nil then
		szItemName =  item:getDisplayName();
		if item:getItemSurplus() ~= -1 then 
			szItemSurplus = item:getItemSurplus() .. "��";
			if item:getItemSurplus() == 0 then
				szSurplusColor = COLOR_RED;
			end
		end
	elseif itemDef.UseMaxDur > 0 then
		szItemSurplus = itemDef.UseMaxDur .. "��";
	end
	
	local itemUseLevel ;
	if itemDef.UseTarget ~= USE_TARGET_PET or itemDef.WearPre.RoleLevelMin == 0 then
		itemUseLevel = itemDef.WearPre.RoleLevelMin;
	else
		itemUseLevel = "����" .. itemDef.WearPre.RoleLevelMin .. "��"
	end
	local t_oneItemAttrInfo;
	local t_ToolsInfo = {
		{ BASE_AREA1, "",				szItemName,									GetItemNameColor(itemDef),		LINE_TYPE_HEADTEXT, true },	--��Ʒ����
		{ BASE_AREA1, "",				GetArmBind( item,itemDef ),					GetColor()[COLOR_ITEMSTART],	LINE_TYPE_BINDTEXT, false },--��Ʒ��ǰ��״��
		{ BASE_AREA1, "��Ʒ����\b",		GetItemTypeById(itemDef.ItemID),			GetColor()[COLOR_GOLD],			LINE_TYPE_TEXT, true },		--��Ʒ����
		{ BASE_AREA1, "��������\b",		GetPetSkillType(itemDef),					GetColor()[COLOR_GOLD],			LINE_TYPE_TEXT, false },
		{ BASE_AREA1, "��ȴʱ��\b",		SetItemCoolTime(itemDef.SelfCool ),			GetColor()[COLOR_WHITE],		LINE_TYPE_TEXT, false },	--��Чʱ��
		{ BASE_AREA1, "ʹ�õȼ�\b",		itemUseLevel,								GetUseLvColr(itemDef),			LINE_TYPE_TEXT, false },	--ʹ�õȼ�
		--{ BASE_AREA1, "",				getSecondarySkillNeed( itemDef ),			getSecondarySkillColor(itemDef),LINE_TYPE_TEXT, false },	--�����
		{ BASE_AREA1, "ְҵ����\b",		GetCareer(itemDef.WearPre.Career),			GetCareerColor(itemDef),		LINE_TYPE_TEXT, false },	--ְҵ 
		{ BASE_AREA1, "ʣ��ʹ�ô���\b",	szItemSurplus,								GetColor()[szSurplusColor],		LINE_TYPE_TEXT, false },	--ְҵ
		( not isShopItem and {} or 
		{ BASE_AREA1, "",				GetItemRespact(itemDef),					GetRespacColor(itemDef),		LINE_TYPE_TEXT, false } ),

		 {},{},{},{},{},{},	{},{},
		{ BASE_AREA2, "",				GetCanDealItem( itemDef.NoDeal ),			GetColor()[COLOR_WHITE],		LINE_TYPE_TEXT, false },	--�Ƿ�ɽ���
		{ BASE_AREA2, "",				CanDropItem( itemDef.NoDrop ),				GetColor()[COLOR_WHITE],		LINE_TYPE_TEXT, false },	--�Ƿ�ɶ���
		{ BASE_AREA2, "",				CanSaleItem( itemDef.NoNpcSell ),			GetColor()[COLOR_WHITE],		LINE_TYPE_TEXT, false },	--�Ƿ�ɳ���
		{ BASE_AREA2, "��Ч����",		SetItemEndTime(item ) ~= "" and SetItemEndTime(item ) or nil,						GetColor()[COLOR_BATTLE],		LINE_TYPE_TEXT, false },	--��Чʱ��
		{ BASE_AREA2, "",				GetItemDesc( item, itemDef ),    			GetColor()[COLOR_WEAK_BLUE],	LINE_TYPE_RICH, false },	--��Ʒ����
		{ OTHER_AREA, "�̵��չ���\b",	itemDef.SellPrice,							GetColor()[COLOR_WHITE],		LINE_TYPE_MONEY, false },	--�̵��չ���
	}
	for index, data in ipairs( t_ToolsInfo ) do
		if data[2] ~= nil and data[3] ~= nil then
		end
	end
	--����ѧϰ����ܵ���
	--[[ȥ��
	local recipeType,recipeId = getSecondarySkillItemData( itemDef );
	if recipeType > 0 then		
		local RecipeDef = SecondarySkillMgr:getRecipeDef( recipeType,recipeId );
		if RecipeDef ~= nil then
			local itemId = RecipeDef.ProduceList[0].GetItem[0].ItemID;
			if itemId > 0 then
				local getItemDef = getItemDef( itemId );
				local productInfo;
				local nItemType = getItemType(getItemDef.ItemID); --��ȡ��Ʒ����
				if nItemType == ITEM_SLOTTYPE_AMM then
					productInfo = GetEquipInfo( nil,getItemDef,nTipsType );
				elseif nItemType == ITEM_SLOTTYPE_ITEM then
					productInfo = GetToolsInfo( nil,getItemDef,nTipsType );
				end
				
				for i=1,table.getn( productInfo ) do
					if productInfo[i][1] ==BASE_AREA1 then
						productInfo[i][1] = BASE_AREA3;
					end
					if productInfo[i][5] == LINE_TYPE_HEADTEXT or productInfo[i][5] == LINE_TYPE_BINDTEXT then
						productInfo[i][5] = LINE_TYPE_TEXT;
					end
					if productInfo[i][5] ~= LINE_TYPE_MONEY then
						table.insert( t_ToolsInfo,productInfo[i] );
					end
				end
			end
			--�����Ҫ����Ʒ
			table.insert( t_ToolsInfo,{OTHER_AREA,"��Ҫ���ϣ�",getSecondarySkillRequireItemDesc(RecipeDef),GetColor()[COLOR_ITEMKEYWORLD],LINE_TYPE_RICH,false} );
		end
		return t_ToolsInfo;
	end
	--]]

	local subType = math.floor( itemDef.ItemID/1000 );
	local nShowAttrItem = {5210,5211,5212,5213,5251};
	local bFound = false;
	for i = 1, table.getn( nShowAttrItem ) do
		if subType == nShowAttrItem[i] then
			bFound = true;
		end
	end

	if not bFound then
		return t_ToolsInfo;
	end

	if GetItemTypeById( itemDef.ItemID ) == "װ����ʯ" then
	elseif isFairyKeyIn( itemDef ) then
		if bShowTitle then
			--��������
			t_oneItemAttrInfo = { FIGHT_AREA,		"",	"�������ԣ�",	GetColor()[COLOR_OGRANGE], LINE_TYPE_TEXT, false };		
			table.insert( t_ToolsInfo, t_oneItemAttrInfo );
			bShowTitle = false;
		end
		
		t_oneItemAttrInfo = {	FIGHT_AREA, getKeyInAttrDesc( itemDef ).."\b ",getKeyInAttrValue( itemDef ),
								GetColor()[COLOR_WHITE], LINE_TYPE_ATTRADD, false } ;
		table.insert( t_ToolsInfo, t_oneItemAttrInfo );
		-- [[elseif ( CheckItemResult( itemDef, RESULT_SPAR ) or CheckItemResult( itemDef, RESULT_SPAR_SHOP ) ) and item ~= nil then
		--��ʯ�ĸ�������
		local sparLv = item:getItemInstVal1()
		local sparResultId = item:getItemFlag()
		local sparBaseVal = item:getItemInstVal2()
		local sparexp = item:getItemInstVal3()
		local sparDef = GameArmProce:getSparDef( item:getItemId() )
		if sparDef ~= nil and sparLv > 0 then
			local szLvText = "�ȼ���" .. sparLv .. "�������" .. sparDef.MaxLvl .. "����";
			table.remove( t_ToolsInfo, 4 );
			table.insert( t_ToolsInfo, 4, { BASE_AREA1, "", szLvText, GetColor()[COLOR_GOLD],	LINE_TYPE_TEXT, false } );
			local szExpText
			if sparLv == sparDef.MaxLvl and sparLv > 2 then
				szExpText = "���飺" .. sparDef.UpExp[sparLv-2] .. "/" .. sparDef.UpExp[sparLv-2];
			else
				szExpText = "���飺" .. sparexp .. "/" .. sparDef.UpExp[sparLv-1];
			end
			
			table.remove( t_ToolsInfo, 5 );
			table.insert( t_ToolsInfo, 5, { BASE_AREA1, "", szExpText, GetColor()[COLOR_GOLD],	LINE_TYPE_TEXT, false } );
			if bShowTitle then
				--��������	
				table.insert( t_ToolsInfo, { FIGHT_AREA, "", "�������ԣ�", GetColor()[COLOR_OGRANGE], LINE_TYPE_TEXT, false } );
				bShowTitle = false;
			end
			local value = sparBaseVal * ( 2 + sparLv ) / 3 
			local tAddattrVal = { value }
			table.insert( t_ToolsInfo, 13, { FIGHT_AREA, "\b", getNewAttrDesc( sparResultId, tAddattrVal ), GetColor()[COLOR_ITEMKEYWORLD], LINE_TYPE_ATTRADD, false } );
		end
		--]]
	else
		for i = 1,MAX_ITEM_RESULT do
			if	itemDef.Result[i-1].ResultID ~=  RESULT_NONE then
				for j = 1, table.getn(t_tagRESULT) do
					local t_ResultValue = { itemDef.Result[i-1].ResultVal1,itemDef.Result[i-1].ResultVal2,itemDef.Result[i-1].ResultVal3,itemDef.Result[i-1].ResultVal4};
					local nValue = 0;
					local szDesc = t_tagRESULT[j][2];
					for k = 1, table.getn(t_ResultValue) do
						if t_ResultValue[k] ~= 0 then
							nValue = t_ResultValue[k];
							break;
						end
					end

					if t_tagRESULT[j][1] == itemDef.Result[i-1].ResultID then
						if bShowTitle then
							t_oneItemAttrInfo = { FIGHT_AREA,		"",	"�������ԣ�",	GetColor()[COLOR_OGRANGE], LINE_TYPE_TEXT, false };
							table.insert( t_ToolsInfo, t_oneItemAttrInfo );
							bShowTitle = false;
						end

						nValue = FormatItemAttrValue( { ["value"] = nValue, ["resultID"] = itemDef.Result[i-1].ResultID } );
						t_oneItemAttrInfo = {	FIGHT_AREA, t_tagRESULT[j][2].."\b+",	
												nValue,
												GetColor()[COLOR_WHITE], LINE_TYPE_ATTRADD, false };
						table.insert( t_ToolsInfo, t_oneItemAttrInfo );
					end
				end
			end
		end
	end
	return t_ToolsInfo;
end

--������Ʒ
function SetItemTips( nTipsType, item, itemDef, szSelfName, isShopItem )
	ClearItemTips(szSelfName);
	--local itemDef = item:getItemDef();
	--��Ʒ��ͼ
	local itemTex = getglobal(szSelfName.."ITItemTex");

	local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
	-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
	if not IsInExistence( IconPath ) then
		IconPath = DEFAULT_ITEM_PATH;
	end
	itemTex:SetTexture( IconPath );
	local bindTexture = getglobal( szSelfName .."BindTexture" );
	if item ~= nil and item:isItemBind() then
		bindTexture:Show();
	else
		bindTexture:Hide();
	end
	local t_ToolInfo = GetToolsInfo(item,itemDef,nTipsType, isShopItem);
	for i = 1, table.getn(t_ToolInfo) do
		AddTipsLine( t_ToolInfo[i][1], t_ToolInfo[i][2], t_ToolInfo[i][3], t_ToolInfo[i][4], t_ToolInfo[i][5], t_ToolInfo[i][6] );
	end
end

--������Ʒ����ʱ��
function SetItemEndTime( item )
	if item ~= nil then
		local nValidEndTime = item:getItemValidEndTime();		
		if nValidEndTime ~= 0 then	
			-- os.date("%c", nValidEndTime)
			local szEndTimeDesc = os.date("%Y", nValidEndTime).."-"..os.date("%m", nValidEndTime).."-"..os.date("%d", nValidEndTime).."  "
					..os.date("%H", nValidEndTime)..":"..os.date("%M", nValidEndTime);
			if item:isValidEnd() then				
				szEndTimeDesc = szEndTimeDesc .. " �ѹ���"
			end
			return szEndTimeDesc;
		end
	end
	return "";
end

--������Ʒ����ʱ��
function SetItemCoolTime( nCoolTime )
	if nCoolTime ~= 0 then
		return math.ceil(nCoolTime/10).."��";
	end

	return "";
end

--����װ��
function SetAmmTips( nTipsType, item, itemDef, szSelfName )
	ClearItemTips(szSelfName);
	--��Ʒ��ͼ
	local itemTex = getglobal(szSelfName.."ITItemTex");

	local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
	-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
	if not IsInExistence( IconPath ) then
		IconPath = DEFAULT_ITEM_PATH;
	end
	itemTex:SetTexture( IconPath );

	local bindTexture = getglobal( szSelfName .."BindTexture" );
	if item ~= nil and item:isItemBind() then
		bindTexture:Show();
	else
		bindTexture:Hide();
	end

	local t_EquipInfo = GetEquipInfo( item, itemDef, nTipsType );
	for i = 1, table.getn(t_EquipInfo) do
		if t_EquipInfo[i] ~= nil then
			AddTipsLine( t_EquipInfo[i][1], t_EquipInfo[i][2], t_EquipInfo[i][3], t_EquipInfo[i][4], t_EquipInfo[i][5], 
							t_EquipInfo[i][6],t_EquipInfo[i][7] );
		end
	end
end

--���õ�ǰװ��
function SetSefAmmTips( nTipsType, item, itemDef, szSelfName )
	ClearItemTips(szSelfName);
	--��Ʒ��ͼ
	local itemTex = getglobal(szSelfName.."ITItemTex");

	local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
	-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
	if not IsInExistence( IconPath ) then
		IconPath = DEFAULT_ITEM_PATH;
	end
	itemTex:SetTexture( IconPath );

	itemTex:SetPoint( "topleft", szSelfName, "topleft", 15, 50);

	local bindTexture = getglobal( szSelfName .."BindTexture" );
	if item ~= nil and item:isItemBind() then
		bindTexture:Show();
	else
		bindTexture:Hide();
	end

	local t_EquipInfo = GetEquipInfo( item, itemDef, nTipsType );
	for i = 1, table.getn(t_EquipInfo) do
		if t_EquipInfo[i] ~= nil then
			AddSelfTipsLine( t_EquipInfo[i][1], t_EquipInfo[i][2], t_EquipInfo[i][3], t_EquipInfo[i][4], t_EquipInfo[i][5], t_EquipInfo[i][6],
							t_EquipInfo[i][7] );
		end
	end
end

--������ʾ������
function AddSelfTipsLine( nAreaType, szText, content, t_Color, nType, bAlwayDis, nResultIndex ) --������ʾ���ݣ���ɫֵ1,��ɫֵ2,��ɫֵ3,����,�Ƿ�������ʾ
	if bAlwayDis == false and ( tonumber(content) == 0 or content == nil or content == "" ) then return end  --����0��ʱ����ʾ
	if szText == nil then
		return;
	end
	local t_TipsLineInfo = { nAreaType,szText,content, t_Color, nType, nResultIndex };
	table.insert(t_SelfTipsLineList, t_TipsLineInfo);
end

local nFontHeight	= 15; --�����Ĭ�ϸ߶�
local nFontDistance = 10; --����ͬ�����ļ��

function ShowSelTips(szName, itemDef)
	local nLastType = BASE_AREA1;
	local nTop = 0;
	local nSizeOffSet		= 0;
	local nLineIndex		= 1;
	local nGemIndex			= 1;
	local nRichLineIndex	= 1;
	local nAttrAddIndex		= 1;
	local nBaseAreaNum		= 0;
	local BIND_Y_OFFSET		= 5;
	local MIN_HEIGHT		= 90;
	for i = 1, table.getn(t_SelfTipsLineList) do
		local nAreaType			= t_SelfTipsLineList[i][1]; --��������
		local szText			= t_SelfTipsLineList[i][2]; --��ʾ��ǰ׺����
		local szContent			= t_SelfTipsLineList[i][3]; --��ʾ������
		local nContentType		= t_SelfTipsLineList[i][5]; --����
		local szTextContent = szText;
		if szContent ~= nil then
			szTextContent = szText..szContent;--��ʾ����������
		end
		local t_Color = t_SelfTipsLineList[i][4];  --��ɫ
		local AmmLineTex = getglobal(szName.."ITItemLineTex"..nLineIndex);
		local AmmGemTex = getglobal(szName.."InLayGemItemTex"..nGemIndex); -- ��Ƕ��ʯͼ
		
		if BASE_AREA1 == nAreaType then
			nBaseAreaNum = nBaseAreaNum + 1;
		end

		if i == 1 then
			nTop = 45;
		elseif nAreaType ~= nLastType then	
			if nLastType == BASE_AREA1 and nBaseAreaNum < 5 then
				nTop = nTop + ( 5 - nBaseAreaNum )*15 - 10;
			end
			
			nTop = nTop + nFontHeight + nFontDistance;
			AmmLineTex:SetPoint( "left", szName.."ITItemTex", "left", -15, nTop-80);
			AmmLineTex:Show();
			nLineIndex = nLineIndex +1;

			nLastType = nAreaType;
		else
			nTop = nTop + nFontHeight;
		end

		if nContentType == LINE_TYPE_RICH then
			if szContent ~= nil then
				local szText = getItemInfo(itemDef,szTextContent);
				if szText ~= nil  then
					local rich = getglobal(szName.."ITItemRich"..nRichLineIndex);
					rich:clearHistory();
					rich:SetText(szText,t_Color[1], t_Color[2], t_Color[3]);
					rich:SetPoint("topleft", szName, "topleft", 15, nTop);
					rich:Show();
					local nCurHeight = (rich:GetTextLines()-1) * 15;
					nTop = nTop + nCurHeight;
					nRichLineIndex = nRichLineIndex + 1;
				end
			end
		elseif nContentType == LINE_TYPE_TEXTURE then
			if (nTop) < 115 then
				nTop = nTop + 15;
			end
			nTop = nTop - 5;
			SetTexturePoint(szContent,szName,nTop-1);
		elseif nContentType == LINE_TYPE_HEADTEXT then
			local curequipfont = getglobal(szName.."ITItemFont"..(nMaxFontNumber-1));
			curequipfont:SetText("����ǰװ����");
			curequipfont:SetPoint("topleft", szName, "topleft", 6, 10);
			curequipfont:Show();

			local font = getglobal(szName.."ITItemFont"..(nMaxFontNumber-2));
			font:SetText(szTextContent);
			font:SetPoint("topleft", szName, "topleft", 0, nTop-18);
			font:SetTextColor(t_Color[1], t_Color[2], t_Color[3]);
			font:Show();
		elseif nContentType == LINE_TYPE_BINDTEXT then
			local font = getglobal(szName.."ITItemFont"..(nMaxFontNumber-3));
			font:SetText(szTextContent);
			font:SetPoint("topleft", szName.."ITItemTex", "topleft", -93, 50);
			font:SetTextColor(t_Color[1], t_Color[2], t_Color[3]);
			font:Show();
			nSizeOffSet = BIND_Y_OFFSET;
			nBaseAreaNum = nBaseAreaNum - 1;
			nTop = nTop - nFontHeight;
		elseif nContentType == LINE_TYPE_MONEY then
			local font = getglobal(szName.."ITItemFont"..(nMaxFontNumber-4));
			font:SetText(szTextContent);
			font:SetPoint("topleft", szName, "topleft", 15, nTop);
			font:SetTextColor(t_Color[1], t_Color[2], t_Color[3]);
			font:Show();

			local moneytex = getglobal( szName.."ITItemMoneyTex");
			local nWidth = font:GetTextExtentWidth(szContent);
			local nOffset = 0;
			if itemDef.MoneyType == ATTR_ID_MONEY then
				moneytex:SetTexUV(78,494,20,18);
				moneytex:SetSize(20,18);
			elseif itemDef.MoneyType == ATTR_ID_BULL then
				moneytex:SetTexUV(78,494,20,18);
				moneytex:SetSize(20,18);
				nOffset = nOffset;
			end
			local nLeftPoint = 105 - 12;
			moneytex:SetPoint("topleft", szName, "topleft", nLeftPoint+nWidth, nTop+nOffset);
			moneytex:Show();
		elseif nContentType == LINE_TYPE_ATTRADD then
			local rich = getglobal(szName.."ITItemRich"..nRichLineIndex);			
			rich:resizeRect( 240 * GetScreenScaleY(),200 );
			rich:clearHistory();			
			rich:SetText(szTextContent,t_Color[1], t_Color[2], t_Color[3]);
			rich:SetPoint("topleft", szName, "topleft", 15, nTop);
			rich:Show();
			local nCurHeight = (rich:GetTextLines()-1) * 15;
			nTop = nTop + nCurHeight;
			nRichLineIndex = nRichLineIndex + 1;
		else
			local font = getglobal(szName.."ITItemFont"..i);

			if string.find( szTextContent,"#WPSUITNUM#", 1, true ) then
				szTextContent		= string.gsub( szTextContent,"#WPSUITNUM#", 0 );
			end

			if string.find( szTextContent,"#WPSUIT#", 1, true ) then
				local nLength	= string.len(szTextContent);
				local nEndPos	= string.find(szTextContent,"#",9,nLength);
				local REPCOLOR	= tonumber(string.sub( szTextContent, 9,nEndPos-1 ));
				local szReplace = "#WPSUIT#"..REPCOLOR.."#"
				szTextContent	= string.gsub( szTextContent,szReplace,"" );
			end

			font:SetText(szTextContent);
			if nAreaType == BASE_AREA1 then
				font:SetPoint("topleft", szName, "topleft", 75, nTop-10);
			else
				font:SetPoint("topleft", szName, "topleft", 15, nTop);
			end
			if nContentType == LINE_TYPE_GEM_TEXTURE then
				local tex = getglobal(szName.."InLayGemItemTex"..nGemIndex);
				local lineback	= getglobal(szName.."InLayGemItemTex"..nGemIndex.."_Rim");

				lineback:Show();
				local t_GemColor = GetGemColorTable();
				for i = 1, table.getn( t_GemColor ) do
					if string.find( szText, t_GemColor[i][1], 1, true ) then
						lineback:SetTexture( "uires\\ui\\buff  TuBiaoDiBan\\tipXiangQianKuang18.tga" );	 
						lineback:SetTexUV(t_GemColor[i][2]*18,0,18,18);
						lineback:SetSize(18,18);   
						tex:SetTexture( "uires\\ui\\buff  TuBiaoDiBan\\tip  XiangQianKuangDiBan18.tga" );
						tex:SetTexUV(t_GemColor[i][2]*18,0,18,18);
						tex:SetSize(18,18);   
					end									   
				end
				local nGemName = szTextContent;
				-- TODO: ShowSelTips
				if szContent ~= "δ��Ƕ" then
					local nLength		= string.len( szContent );
					local nFirstPos		= string.find( szContent, ",", 1, nLength );
					local nGemIconID	= string.sub( szContent, nFirstPos+1, nLength );
					local nGemId		= string.sub( szContent, 1, nFirstPos-1 );
					local gemitemdef	= getItemDef( nGemId );
					local nResultIndex	= t_SelfTipsLineList[i][6]; 
					local szAttr		= "";
					--[[
					if nResultIndex ~= nil then
						szAttr	= GetItemResultByIndex( nGemId, nResultIndex )
					else
						szAttr	= GetItemResult( nGemId );
					end
					nGemName			= gemitemdef.Name .. "  "..szAttr;
					--]]
					nGemName = gemitemdef.Name;

					local IconPath = GetItemEquipIconPath()..nGemIconID..".tga";
					-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
					if not IsInExistence( IconPath ) then
						IconPath = DEFAULT_ITEM_PATH;
					end
					tex:SetTexture( IconPath );	 
					tex:SetTexUV(0,0,64,64); 
					tex:SetSize(18,18);
				end
				tex:SetPoint("topleft", szName, "topleft", 29, nTop+4);
				tex:SetSize(18,18);
				tex:Show();

				font:SetPoint("topleft", szName, "topleft", 51, nTop+4);
				nGemIndex = nGemIndex + 1;
				font:SetText(nGemName);
				nTop = nTop + 8;
			elseif nContentType == LINE_TYPE_DISTANCE then
				nTop = nTop -6;
			end
			font:SetTextColor(t_Color[1], t_Color[2], t_Color[3]);
			font:Show();
		end
	end

	local nWidth = nTop-nSizeOffSet;
	if nWidth < MIN_HEIGHT then
		nWidth = MIN_HEIGHT;
	end

	SetSize(nWidth, szName);
end

function ShowTips(szName, itemDef, nTipsType, item)
	local nLastType			= BASE_AREA1;
	local nTop				= 0;
	local nSizeOffSet		= 0;
	local nLineIndex		= 1;
	local nGemIndex			= 1;
	local nRichLineIndex	= 1;
	local nAttrAddIndex		= 1;
	local nBaseAreaNum		= 0;
	local ntextoffset		= 0;
	local BIND_Y_OFFSET		= 5;
	local MIN_HEIGHT		= 75;
	for i = 1, table.getn(t_TipsLineList) do
		if t_TipsLineList[i][5] == LINE_TYPE_BINDTEXT 
			and (t_TipsLineList[i][3] == "ʰȡ���" or t_TipsLineList[i][3] == "װ�����" ) then
			ntextoffset = 5;
		end
	end

	for i = 1, table.getn(t_TipsLineList) do
		local nAreaType			= t_TipsLineList[i][1]; --��������
		local szText			= t_TipsLineList[i][2]; --��ʾ��ǰ׺����
		local szContent			= t_TipsLineList[i][3]; --��ʾ������
		local nContentType		= t_TipsLineList[i][5]; --����
		local szTextContent		= szText;
		if szContent ~= nil then
			szTextContent = szText..szContent; -- ��ʾ����������
		end

		local t_Color		= t_TipsLineList[i][4];  --��ɫ
		local AmmLineTex	= getglobal(szName.."ITItemLineTex"..nLineIndex);
		local AmmGemTex		= getglobal(szName.."InLayGemItemTex"..nGemIndex); -- ��Ƕ��ʯͼ
		
		if BASE_AREA1 == nAreaType then
			nBaseAreaNum = nBaseAreaNum + 1;
		end

		if i == 1 then
			nTop = 30;
		elseif nAreaType ~= nLastType then
			if nLastType == BASE_AREA1 and nBaseAreaNum < 5 then
				nTop = nTop + ( 5 - nBaseAreaNum )*15 - 10;
			end			

			nTop = nTop + nFontHeight + nFontDistance + 5;
			AmmLineTex:SetPoint( "left", szName.."ITItemTex", "left", -15, nTop-65);
			AmmLineTex:Show();
			nLineIndex = nLineIndex +1;	
			
			nLastType = nAreaType;
		else
			nTop = nTop + nFontHeight;
		end

		if nContentType == LINE_TYPE_RICH then
			if szContent ~= nil then
				local szText = getItemInfo(itemDef,szTextContent);
				if szText ~= nil  then
					local rich = getglobal(szName.."ITItemRich"..nRichLineIndex);
					rich:resizeRect( 220 * GetScreenScaleY(),200 );
					rich:clearHistory();
					rich:SetText(szText,t_Color[1], t_Color[2], t_Color[3]);
					rich:SetPoint("topleft", szName, "topleft", 15, nTop);
					rich:Show();
					local nCurHeight = (rich:GetTextLines()-1) * 15;
					nTop = nTop + nCurHeight;
					nRichLineIndex = nRichLineIndex + 1;
				end
			end
		elseif nContentType == LINE_TYPE_TEXTURE then
			if (nTop) < 105 then
				nTop = nTop + 15;
			end
			nTop = nTop - 5;
			SetTexturePoint(szContent,szName,nTop-1);
		elseif nContentType == LINE_TYPE_HEADTEXT then
			local font = getglobal(szName.."ITItemFont"..(nMaxFontNumber-2));
			font:SetText(szTextContent);
			font:SetPoint("topleft", szName, "topleft", 0, nTop-18);
			font:SetTextColor(t_Color[1], t_Color[2], t_Color[3]);
			font:Show();
		elseif nContentType == LINE_TYPE_BINDTEXT then
			local font = getglobal(szName.."ITItemFont"..(nMaxFontNumber-3));
			font:SetText(szTextContent);
			font:SetPoint("topleft", szName.."ITItemTex", "topleft", -93, 50);
			font:SetTextColor(t_Color[1], t_Color[2], t_Color[3]);
			font:Show();
			nSizeOffSet = BIND_Y_OFFSET;
			nBaseAreaNum = nBaseAreaNum - 1;
			nTop = nTop - nFontHeight;
		elseif nContentType == LINE_TYPE_MONEY then
			local font = getglobal(szName.."ITItemFont"..(nMaxFontNumber-4));
			font:SetText(szTextContent);
			font:SetPoint("topleft", szName, "topleft", 15, nTop);
			font:SetTextColor(t_Color[1], t_Color[2], t_Color[3]);
			font:Show();

			local moneytex	= getglobal( szName.."ITItemMoneyTex");
			local nWidth	= font:GetTextExtentWidth(szContent);
			local nOffset	= 0;
			if itemDef.MoneyType == ATTR_ID_MONEY then
				moneytex:SetTexUV(78,494,20,18);
				moneytex:SetSize(20,18);
			elseif itemDef.MoneyType == ATTR_ID_BULL then
				moneytex:SetTexUV(78,494,20,18);
				moneytex:SetSize(20,18);
				nOffset = nOffset;
			end
			local nLeftPoint = 105 - 12;
			moneytex:SetPoint("topleft", szName, "topleft", nLeftPoint+nWidth, nTop+nOffset);
			moneytex:Show();
		elseif nContentType == LINE_TYPE_ATTRADD then
			local rich = getglobal(szName.."ITItemRich"..nRichLineIndex);			
			rich:resizeRect( 240 * GetScreenScaleY(),200 );
			rich:clearHistory();			
			rich:SetText(szTextContent,t_Color[1], t_Color[2], t_Color[3]);
			rich:SetPoint("topleft", szName, "topleft", 15, nTop);
			rich:Show();
			local nCurHeight = (rich:GetTextLines()-1) * 15;
			nTop = nTop + nCurHeight;
			nRichLineIndex = nRichLineIndex + 1;
		else
			local font = getglobal(szName.."ITItemFont"..i);
			if string.find( szTextContent,"#WPSUITNUM#", 1, true ) then
				local nSuitNum		= 0;
				if nTipsType ~= LUA_ITEMTIPS_TYPE_LINK then
					nSuitNum = 0;
				end
				if onEnterBtn ~= nil and  string.find( onEnterBtn,"WatchPlayerFrameEquip", 1, true ) then
					nSuitNum =  GetWatchPlayerSuitArmNum( itemDef.SuitSetID );
				end

				szTextContent	= string.gsub( szTextContent,"#WPSUITNUM#", nSuitNum );
			end 

			if string.find( szTextContent,"#WPSUIT#", 1, true ) then
				local nLength	= string.len(szTextContent);
				local nEndPos	= string.find(szTextContent,"#",9,nLength);
				local REPCOLOR	= tonumber(string.sub( szTextContent, 9,nEndPos-1 ));
				local szReplace = "#WPSUIT#"..REPCOLOR.."#"
				szTextContent	= string.gsub( szTextContent,szReplace,"" );
				if GetColor()[REPCOLOR] ~= nil then
					t_Color = GetColor()[REPCOLOR];
				end
			end
			font:SetText(szTextContent);
			if nAreaType == BASE_AREA1 then
				font:SetPoint("topleft", szName, "topleft", 75+ntextoffset, nTop-10);
			else
				font:SetPoint("topleft", szName, "topleft", 15, nTop);
			end

			if nContentType == LINE_TYPE_GEM_TEXTURE then  
				local tex = getglobal(szName.."InLayGemItemTex"..nGemIndex);
				local lineback	= getglobal(szName.."InLayGemItemTex"..nGemIndex.."_Rim");
				lineback:Show();
				local t_GemColor = GetGemColorTable();
				for i = 1, table.getn( t_GemColor ) do
					if string.find( szText, t_GemColor[i][1], 1, true ) then
						lineback:SetTexture( "uires\\ui\\buff  TuBiaoDiBan\\tipXiangQianKuang18.tga" );	 
						lineback:SetTexUV(t_GemColor[i][2]*18,0,18,18);
						lineback:SetSize(18,18);   
						tex:SetTexture( "uires\\ui\\buff  TuBiaoDiBan\\tip  XiangQianKuangDiBan18.tga" );
						tex:SetTexUV(t_GemColor[i][2]*18,0,18,18);
						tex:SetSize(18,18);   
					end
				end

				local nGemName = szTextContent;
				if szContent ~= "δ��Ƕ" then
					local nLength = string.len( szContent );
					local nFirstPos		= string.find( szContent, ",", 1, nLength );
					local nGemIconID	= string.sub( szContent, nFirstPos+1, nLength );
					local nGemId		= string.sub( szContent, 1, nFirstPos-1 );
					local gemitemdef	= getItemDef( nGemId );
					local nResultIndex	= t_TipsLineList[i][6]; --����
					local szAttr		= "";
					--[[
					if nResultIndex ~= nil then
						szAttr	= GetItemResultByIndex( nGemId, nResultIndex )
					else
						szAttr	= GetItemResult( nGemId );
					end
					nGemName			= gemitemdef.Name .. "  "..szAttr;
					--]]
					nGemName = gemitemdef.Name;

					local IconPath = GetItemEquipIconPath()..nGemIconID..".tga";
					-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
					if not IsInExistence( IconPath ) then
						IconPath = DEFAULT_ITEM_PATH;
					end
					tex:SetTexture( IconPath );	  
					tex:SetTexUV(0,0,64,64); 
					tex:SetSize(18,18);
				end
				tex:SetPoint("topleft", szName, "topleft", 29, nTop+4);
				tex:Show();

				font:SetPoint("topleft", szName, "topleft", 51, nTop+4);
				nGemIndex = nGemIndex + 1;

				font:SetText(nGemName);
				nTop = nTop + 6
			elseif nContentType == LINE_TYPE_DISTANCE then
				nTop = nTop -8;
			end
			font:SetTextColor(t_Color[1], t_Color[2], t_Color[3]);
			font:Show();
		end
	end
	
	local nWidth = nTop-nSizeOffSet;


	if nWidth < MIN_HEIGHT then
		nWidth = MIN_HEIGHT;
	end

	SetSize(nWidth, szName);
end

function SetSize(nTop, szName)
	local frame = getglobal(szName);
	frame:SetSize(243,nTop + nFontHeight*2);
	nLineIndex = 1;
end

--����TIPS��ʾ��λ��
function SetItemTipsPoint( szName, itemDef, szOnEnterBtn )
	local Frame = getglobal(szName);
	local nWidth    = Frame:GetWidth();
	local nHeight   = Frame:GetHeight();
	local nX		= GetCursorPosX();
	local nY		= GetCursorPosY();
	local OffSetX	= 0;
	local OffSetY	= 0;

	if	szOnEnterBtn ~= nil and GetScreenScaleY() == 1 then
		local OnEnterBtn = getglobal( szOnEnterBtn );
		nX = OnEnterBtn:GetRight() - 10;
		nY = OnEnterBtn:GetRealTop() + 10;
	end

	local TipsWidth = nWidth;
	if ArmItemTipsFrame:IsShown() then
		OffSetX = nWidth;
		TipsWidth = TipsWidth + nWidth;
		if (ArmItemTipsFrame:GetHeight()) > nHeight then
			OffSetY = ArmItemTipsFrame:GetHeight() - nHeight;
		end
	end

	nOffSetX = GetScreenWidth() - ( TipsWidth+nX );
	if ( nY - nHeight - OffSetY ) > 0 then
		if nOffSetX > 0 then
			Frame:SetPoint("BottomLeft", "PhotoFrame", "topleft", nX + 10, nY  - 10); --����
			if ArmItemTipsFrame:IsShown() then
				ArmItemTipsFrame:SetPoint("BottomLeft", "PhotoFrame", "topleft", nX + 10 + 245, nY - 10); --����
			end
		else
			nOffSetX = nX - TipsWidth - 42;
			if nOffSetX < 0 then
				nX = nX - nOffSetX ;
			end
			Frame:SetPoint("BottomRight", "PhotoFrame", "topleft", nX - 42  + 10, nY - 10); --����
			if ArmItemTipsFrame:IsShown() then
				ArmItemTipsFrame:SetPoint("BottomRight", "PhotoFrame", "topleft", nX - 42  + 10 - 245 , nY - 10); --����
			end
		end
	elseif ( nY - nHeight - OffSetY ) < 0 and ( nY + nHeight + OffSetY ) > GetScreenHeight() then
		if nOffSetX > 0 then
			Frame:SetPoint("BottomLeft", "PhotoFrame", "topleft", nX + 10, (GetScreenHeight()+Frame:GetHeight())/2); --����
			if ArmItemTipsFrame:IsShown() then
				ArmItemTipsFrame:SetPoint("BottomLeft", "PhotoFrame", "topleft", nX + 10 + 245, (GetScreenHeight()+ArmItemTipsFrame:GetHeight())/2); --����
			end
		else
			nOffSetX = nX - TipsWidth - 42;
			if nOffSetX < 0 then
				nX = nX - nOffSetX ;
			end
			Frame:SetPoint("BottomRight", "PhotoFrame", "topleft", nX - 42  + 10, (GetScreenHeight()+Frame:GetHeight())/2); --����
			if ArmItemTipsFrame:IsShown() then
				ArmItemTipsFrame:SetPoint("BottomRight", "PhotoFrame", "topleft", nX - 42  + 10 - 245 ,(GetScreenHeight()+ArmItemTipsFrame:GetHeight())/2); --����
			end
		end
	else
		if nOffSetX > 0 then
			Frame:SetPoint("TopLeft", "PhotoFrame", "topleft",nX + 10, nY + 42 - 10); --����
			if ArmItemTipsFrame:IsShown() then
				ArmItemTipsFrame:SetPoint("TopLeft", "PhotoFrame", "topleft",nX + 10 + 245, nY + 42 - 10); --����
			end
		else
			nOffSetX = nX - TipsWidth - 42;
			if nOffSetX < 0 then
				nX = nX - nOffSetX;
			end
			Frame:SetPoint("TopRight", "PhotoFrame", "topleft", nX - 42  +10, nY + 42 - 10); --����
			if ArmItemTipsFrame:IsShown() then
				ArmItemTipsFrame:SetPoint("TopRight", "PhotoFrame", "topleft", nX - 42 + 10 - 245, nY + 42 - 10); --����
			end			
		end
	end
end

function LinkItemTipsCloseBtn_OnClick()
	local szName = this:GetParent();
	HideUIPanel(szName);
	ArmItemTipsFrame:Hide();
end

function SetTexturePoint(szContent,szName,nTop)
	local nFirstPos = string.find( szContent, ",", 1 );
	local nStar = tonumber(string.sub( szContent, 1, nFirstPos - 1 ));
	nFirstPos = nFirstPos + 1;
	local font	= getglobal( szName.."ITItemFont"..nMaxFontNumber );
	local itemTex = getglobal(szName.."ITItemTex");
	if nStar > 0  and nStar <= ARM_STAR_LIMIT then
		for i=1,nStar do
			local tex = getglobal(szName.."ITItemImportantTex"..i);
			tex:SetPoint("TOPLEFT", (szName.."ITItemTex"), "BOTTOMLEFT", 60+(i-1)*9, 17+1);
			tex:Show();
			tex:SetTexUV( 516, 33, 13, 12 );
			local isPerFect = string.sub( szContent, nFirstPos + (i-1)*2, nFirstPos + (i-1)*2 );
			if isPerFect ~=nil and isPerFect == "1" then
				tex:SetTexUV( 530, 33, 13, 12 );
			end
		end

		font:SetText("ǿ���Ǽ���");
		font:SetPoint("TOPLEFT", (szName.."ITItemTex"), "BOTTOMLEFT", 0, 17);
		font:Show();
	end
end

function PetSkillGridTipsFrame_OnShow()
end

function PetSkillGridTipsFrame_OnUpdate()
	local frame = getglobal( this:GetClientString() );
	if not frame:IsShown() then
		this:Hide();
	end
end

function PetSkillTipsFrame_OnShow()
end

function PetSkillTipsFrame_OnUpdate()
	local frame = getglobal( this:GetClientString() );
	if not frame:IsShown() then
		this:Hide();
	end
end

function GetItemResult( nItemId )
	local itemDef = getItemDef( nItemId );
	local szResult = "";
	if itemDef == nil then
		return szResult;
	end
	for i = 1,MAX_ITEM_RESULT do
		local oneResult = itemDef.Result[i-1];
		if oneResult.ResultID ~=  RESULT_NONE then
			for j = 1, table.getn(t_tagRESULT) do
				local t_ResultValue = { oneResult.ResultVal1, oneResult.ResultVal2, oneResult.ResultVal3, oneResult.ResultVal4 };
				local nValue		= 0;
				local szDesc		= t_tagRESULT[j][2];
				for k = 1, table.getn(t_ResultValue) do
					if t_ResultValue[k] ~= 0 then
						nValue = t_ResultValue[k];
						break;
					end
				end

				if nValue ~= 0 and t_tagRESULT[j][1] == itemDef.Result[i-1].ResultID then
					szResult = szResult .. szDesc .. "+" .. nValue;
				end
			end
		end
	end
	return szResult;
end

function GetItemResultByIndex( nItemId, nResultIndex )
	local itemDef = getItemDef( nItemId );
	local szResult = "";
	if itemDef == nil then
		return szResult;
	end
	
	local oneResult = itemDef.Result[nResultIndex];
	if oneResult.ResultID ==  RESULT_NONE then
		return szResult;
	end
		
	for j = 1, table.getn(t_tagRESULT) do
		local t_ResultValue = { oneResult.ResultVal1, oneResult.ResultVal2, oneResult.ResultVal3, oneResult.ResultVal4 };
		local nValue		= 0;
		local szDesc		= t_tagRESULT[j][2];
		for k = 1, table.getn(t_ResultValue) do
			if t_ResultValue[k] ~= 0 then
				nValue = t_ResultValue[k];
				break;
			end
		end

		if nValue ~= 0 and t_tagRESULT[j][1] == oneResult.ResultID then
			szResult = szResult .. szDesc .. "+" .. nValue;
		end
	end

	return szResult;
end

function GetAddAttrValue( t_NeedChangeToValue, AddAttrID, nAddAttrValue )
	local nCurValue = nAddAttrValue;
	for i = 1, table.getn( t_NeedChangeToValue ) do
		if AddAttrID == t_NeedChangeToValue[i][1] then	  
			nCurValue = math.floor(t_NeedChangeToValue[i][2]*nAddAttrValue/100);  
			if nCurValue == 0 then
				nCurValue = 1;
			end	
		end
	end
	return 	nCurValue;
end

function getKeyInEndDesc( nTime )
	if nTime ~= 0 then
		local serverTime = GameClanManager:getServerTime();
		local dt = nTime - serverTime;
		if dt < 3600 then
			return "(ʣ�� <1Сʱ)"
		else
			return "(ʣ��" .. math.ceil( dt/3600 ) .."Сʱ)";
		end
	end
	return "";
end

function isFairyKeyIn( itemDef )
	for i=1,MAX_ITEM_RESULT do
		if itemDef.Result[i-1].ResultID == RESULT_ELEMENT_ATK or itemDef.Result[i-1].ResultID == RESULT_ELEMENT_DEF then
			return true;
		end
	end
	return false;
end

local t_KeyInttrName = {[1] = "��",[2] = "��",[3] = "��",[4] = "��",[5] = "��"};

function getKeyInAttrDesc( itemDef )
	for i=1,MAX_ITEM_RESULT do
		if itemDef.Result[i-1].ResultID == RESULT_ELEMENT_ATK then
			return t_KeyInttrName[  itemDef.Result[i-1].ResultVal1 ] .. "Ԫ���˺�";
		elseif itemDef.Result[i-1].ResultID == RESULT_ELEMENT_DEF then
			return t_KeyInttrName[  itemDef.Result[i-1].ResultVal1 ] .. "Ԫ�ؿ���";
		end
	end
	return ""
end

function getKeyInAttrValue( itemDef )
	for i=1,MAX_ITEM_RESULT do
		if itemDef.Result[i-1].ResultID == RESULT_ELEMENT_ATK or itemDef.Result[i-1].ResultID == RESULT_ELEMENT_DEF then
			return itemDef.Result[i-1].ResultVal2 .."-" .. itemDef.Result[i-1].ResultVal3;
		end
	end
end
function ItemTips_OnFocusLost()
	this:Hide();
end