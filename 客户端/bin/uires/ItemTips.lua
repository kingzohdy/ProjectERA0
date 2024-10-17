
local BASE_AREA1		= 1;	--基本信息区1
local BASE_AREA2		= 2;	--基本信息区2
local FIGHT_AREA		= 3;	--战斗数据区
local INTENSIFY_AREA	= 4;	--强化说明区
local SUIT_AREA			= 5;	--套装属性区
local OTHER_AREA		= 6;	--其他说明区
local KEYWORLD_AREA		= 7;
local BASE_AREA3		= 9;
local nEndItemTime		= 0;
local onEnterFuc		= nil;
local onEnterBtn		= nil;
local szSelfName		= nil;
local MAX_RICH_LINE		= 10;
local MAX_STAR_NUM		= 12;
local MAX_GEM_STAR		= 9;
t_WearPosInfo = {};
t_WearPosInfo[EQUIP_POS_RIGHTHAND]		= "主手";
t_WearPosInfo[EQUIP_POS_HEAD]			= "头盔";
t_WearPosInfo[EQUIP_POS_BODY]			= "胸甲";
t_WearPosInfo[EQUIP_POS_BACK]			= "背部";
t_WearPosInfo[EQUIP_POS_LEFT]			= "副手";
t_WearPosInfo[EQUIP_POS_FACE]			= "面饰";
t_WearPosInfo[EQUIP_POS_SHOULDER]		= "肩甲";
t_WearPosInfo[EQUIP_POS_FASHION_BODY]	= "时装衣服";
t_WearPosInfo[EQUIP_POS_BADGE]			= "徽章";
t_WearPosInfo[EQUIP_POS_AMULET]			= "配件";
t_WearPosInfo[EQUIP_POS_NECKLACE]		= "项链";
t_WearPosInfo[EQUIP_POS_RING]			= "戒指";
t_WearPosInfo[EQUIP_POS_RING1]			= "戒指";
t_WearPosInfo[EQUIP_POS_RING2]			= "戒指";
t_WearPosInfo[EQUIP_POS_RING3]			= "戒指";
t_WearPosInfo[EQUIP_POS_SHOE]			= "鞋子";

t_WearPosInfo[EQUIP_POS_GRAIL]			= "圣杯";
t_WearPosInfo[EQUIP_POS_QIYUE]			= "契约";

t_WearPosInfo[EQUIP_POS_SACHET]			= "香囊";	
t_WearPosInfo[EQUIP_POS_AMULET1]		= "护符";
t_WearPosInfo[EQUIP_POS_AMULET2]		= "护符";
t_WearPosInfo[EQUIP_POS_FASHION_HEAD]	= "时装帽子";
t_WearPosInfo[EQUIP_POS_FASHION_HAIR]	= "时装发型";
t_WearPosInfo[EQUIP_POS_FASHION_BACK]	= "时装背饰";
t_WearPosInfo[EQUIP_POS_FASHION_SHOES]	= "时装鞋子";
t_WearPosInfo[EQUIP_POS_FASHION_HAND]	= "时装手套";
t_WearPosInfo[EQUIP_POS_FASHION_LEG]	= "时装裤子";


--EQUIP_POS_QIYUE			= 16,
--EQUIP_POS_NECKLACE		= 17, 	/* 项链 */
--EQUIP_POS_SACHET		= 18, 	/* 香囊 */

local g_nDestIndex = 0;
ITEM_GET_BIND	= 1;
ITEM_WEAR_BIND	= 2;

local t_tipsEvent = {};

local t_ArmBind = {"","拾取后绑定","装备后绑定","已绑定"};
local t_TipsLineList = {};
local t_SelfTipsLineList = {};

local t_AttackType = { 
			[1] = "魔法",
			[6] = "物理",
			[8] = "攻城",
			[9] = "神圣",
			[10]= "机械",
			};

function getItemInfo(ItemDef,szText)
	if szText == nil then return end
	local ItemInfo=
	{
		{"#ItemID#",                      ItemDef.ItemID},					--/*   */
		{"#Name#",						  ItemDef.Name},					--/*   */
		{"#IconID#",					  ItemDef.IconID},					--/*   Ver.6 图标id */
		{"#SellPrice#",					  ItemDef.SellPrice},				--/*   卖给npc值多少钱 */
		{"#BuyPrice#",                    ItemDef.BuyPrice},				--/*   从npc买需要多少钱  */
		{"#MaxPile#",                     ItemDef.MaxPile},					--/*   可堆叠最大上限*/
		{"#ItemType#",                    ItemDef.ItemType},				--/*    */
		{"#ArmDur.MaxDur#",               ItemDef.ArmDur.MaxDur},			--/*   Ver.8 装备耐久 */
		{"#ArmDur.FightStatusDur#",       ItemDef.ArmDur.FightStatusDur},	--/*   Ver.8 装备耐久*/
		{"#ArmFight#",                    ItemDef.ArmFight},				--/*   Ver.8 装备战斗力  */
		{"#SuitSetID#",                   ItemDef.SuitSetID},				--/*   Ver.8 所属套装的ID */
		{"#ValidTime#",                   ItemDef.ValidTime},				--/*   装备从获得开始的有效时间，用于罩装，单位小时 */
		{"#NumType#",                     ItemDef.NumType},					--/*   item id 的编号前3位，详细见编号规则 */
		{"#NoDrop#",                      ItemDef.NoDrop},					--/*   是否是不可丢弃道具 */
		{"#ArmBind#",                     ItemDef.ArmBind},					--/*   是否绑定装备 */
		{"#ExchgLevel#",                  ItemDef.ExchgLevel},				--/*   流通等级 */
		{"#Important#",                   ItemDef.Important},				--/*   道具重要程度 */
		{"#AmmAtk#",                      ItemDef.AmmAtk},					--/*   弹药的攻击力修正指数 */
		{"#WeaponAmmNeed#",               ItemDef.WeaponAmmNeed},			--/*   弹药类型 */
		{"#WeaponAmmType#",               ItemDef.WeaponAmmType},			--/*   武器弹药类型，这个武器使用什么类型的弹药 */
		{"#AmmType#",                     ItemDef.AmmType},					--/*   弹药类型 */
		{"#WearPre.RoleLevelMin#",        ItemDef.WearPre.RoleLevelMin},	--/*    */
		{"#WearPre.RoleLevelMax#",        ItemDef.WearPre.RoleLevelMax},	--/*    */
		{"#WearPre.Career#",              ItemDef.WearPre.Career},			--/*    */
		{"#WearPre.DiathesisID#",         ItemDef.WearPre.DiathesisID},		--/*    */
		{"#WearPre.Gender#",              ItemDef.WearPre.Gender},			--/*    */
		{"#WearPosNum#",                  ItemDef.WearPosNum},				--/*   装备占几个坑 */
		{"#WearPos1#",                    ItemDef.WearPos1},				--/*   占的装备坑 Bind Macrosgroup:WEAR_POS */
		{"#WearPos2#",                    ItemDef.WearPos2},				--/*   占的装备坑 Bind Macrosgroup:WEAR_POS */
		{"#WearPos3#",                    ItemDef.WearPos3},				--/*   占的装备坑 Bind Macrosgroup:WEAR_POS */
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
		{"#百分比#",	AddAttrVal .. "%"	},	-- /*显示百分比	*/
		{"#万分比#",	nNewVal .. "%"		},	-- /*显示万分比	*/
		{"#数值#",		AddAttrVal			},	-- /*显示数值	*/
	};

	local szAddAttrDesc = "";  
	local ArmDef		= getArmAddAttrDef( AddAttrID );

	if AddAttrID == ADD_ATTR_RESULT_SKILL_LEVEL then
		local SkillDef	= SkillMgr:getSpellStaticAtt( ArmDef.Result[AddAttrVal].ResultVal1, 1 );
		-- TODO: 这是特意写错，不显示装备对技能的提升效果吗？
		if SkillDef == nil then
			szAddAttrDesc = "技能："..SkillDef.Name.."等级提升"..ArmDef.Result[AddAttrVal].ResultVal2.."级";
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

-- 获取新关键字数值
function getNewAttrVal( szAddAttrDesc, tAttrVal )
	local t_AttrDesc =
	{
		{"#val1百分比#",	tAttrVal[1] ~= nil and ( math.floor( tAttrVal[1]*100)/100 .. "%" ) or nil	},	-- /*显示百分比	*/
		{"#val1万分比#",	tAttrVal[1] ~= nil and ( math.floor( tAttrVal[1])/100 .. "%" ) or nil		},	-- /*显示万分比	*/
		{"#val1#",			tAttrVal[1] ~= nil and ( math.floor( tAttrVal[1] ) ) or nil					},	-- /*显示数值	*/
		{"#val2百分比#",	tAttrVal[2] ~= nil and ( math.floor( tAttrVal[2]*100)/100 .. "%" ) or nil	},	-- /*显示百分比	*/
		{"#val2万分比#",	tAttrVal[2] ~= nil and ( math.floor( tAttrVal[2])/100 .. "%" ) or nil		},	-- /*显示万分比	*/
		{"#val2#",			tAttrVal[2] ~= nil and ( math.floor( tAttrVal[2] ) ) or nil					},	-- /*显示数值	*/
		{"#val3百分比#",	tAttrVal[3] ~= nil and ( math.floor( tAttrVal[3]*100)/100 .. "%" ) or nil	},	-- /*显示百分比	*/
		{"#val3万分比#",	tAttrVal[3] ~= nil and ( math.floor( tAttrVal[3])/100 .. "%" ) or nil		},	-- /*显示万分比	*/
		{"#val3#",			tAttrVal[3] ~= nil and ( math.floor( tAttrVal[3] ) ) or nil														},	-- /*显示数值	*/
		{"#val4百分比#",	tAttrVal[4] ~= nil and ( math.floor( tAttrVal[4]*100)/100 .. "%" ) or nil	},	-- /*显示百分比	*/
		{"#val4万分比#",	tAttrVal[4] ~= nil and ( math.floor( tAttrVal[4])/100 .. "%" ) or nil		},	-- /*显示万分比	*/
		{"#val4#",			tAttrVal[4] ~= nil and ( math.floor( tAttrVal[4] ) ) or nil					},	-- /*显示数值	*/
	};

	for i = 1, #t_AttrDesc do
		if string.find( szAddAttrDesc, t_AttrDesc[i][1], 1, true ) and t_AttrDesc[i][2] ~= nil then
			return t_AttrDesc[i][2];
		end
	end

	return 0;
end

-- 获取新关键字描述
function getNewAttrDesc( AddAttrID, tAttrVal )
	local t_AttrDesc =
	{
		{"#val1百分比#",	tAttrVal[1] ~= nil and ( math.floor( tAttrVal[1]*100)/100 .. "%" ) or nil	},	-- /*显示百分比	*/
		{"#val1万分比#",	tAttrVal[1] ~= nil and ( math.floor( tAttrVal[1])/100 .. "%" ) or nil		},	-- /*显示万分比	*/
		{"#val1#",			tAttrVal[1] ~= nil and ( math.floor( tAttrVal[1] ) ) or nil					},	-- /*显示数值	*/
		{"#val2百分比#",	tAttrVal[2] ~= nil and ( math.floor( tAttrVal[2]*100)/100 .. "%" ) or nil	},	-- /*显示百分比	*/
		{"#val2万分比#",	tAttrVal[2] ~= nil and ( math.floor( tAttrVal[2])/100 .. "%" ) or nil		},	-- /*显示万分比	*/
		{"#val2#",			tAttrVal[2] ~= nil and ( math.floor( tAttrVal[2] ) ) or nil					},	-- /*显示数值	*/
		{"#val3百分比#",	tAttrVal[3] ~= nil and ( math.floor( tAttrVal[3]*100)/100 .. "%" ) or nil	},	-- /*显示百分比	*/
		{"#val3万分比#",	tAttrVal[3] ~= nil and ( math.floor( tAttrVal[3])/100 .. "%" ) or nil		},	-- /*显示万分比	*/
		{"#val3#",			tAttrVal[3] ~= nil and ( math.floor( tAttrVal[3] ) ) or nil														},	-- /*显示数值	*/
		{"#val4百分比#",	tAttrVal[4] ~= nil and ( math.floor( tAttrVal[4]*100)/100 .. "%" ) or nil	},	-- /*显示百分比	*/
		{"#val4万分比#",	tAttrVal[4] ~= nil and ( math.floor( tAttrVal[4])/100 .. "%" ) or nil		},	-- /*显示万分比	*/
		{"#val4#",			tAttrVal[4] ~= nil and ( math.floor( tAttrVal[4] ) ) or nil					},	-- /*显示数值	*/
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
		SecondArmItemTipsFrame:Hide();
	end
end

local nArmBindNo  = 0;	--已绑定装备
local nArmBindYes = 1;	--未绑定装备
local nAllCareer  = 0;	--不要求
local nWarrior    = 1;  --战士
local nMagician   = 2;  --法师
local nGunner     = 3;  --枪手
local nChurch	  = 4;  --祭司

local nWarrior_Single = 17
local nWarrior_Double = 33
local nMagician_Single = 18
local nMagician_Double = 34
local nGunner_Single = 19
local nGunner_Double = 35
local nChurch_Single = 20
local nChurch_Double = 36

LUA_ITEMTIPS_TYPE_LINK = 1;
LUA_ITEMTIPS_TYPE_NORMAL = 2;

LUA_ITEMTIPS_ID			= 1 ;
LUA_ITEMTIPS_ITEMDEF	= 2 ;
LUA_ITEMTIPS_ITEM		= 3 ;
LUA_ITEMTIPS_ITEMTABLE	= 4 ;

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
	if bitAnd( itemDef.CtrlFlag, ITEM_NO_LEVELLIMIT ) then return 0 end
	local nLv = itemDef.WearPre.RoleLevelMin
	if item and item:getItemType() == ITEM_SLOTTYPE_AMM then
		for i = 1, item:getArmAddAttrNum() do
			local AddAttrInst	= item:getArmAddAttr(i-1);
			local newAddAttrDef = getNewAddAttrDef( AddAttrInst.AddAttrID );
			if newAddAttrDef ~= nil and newAddAttrDef.AddAttrResultID == ADD_ATTR_RESULT_DEC_WEAR_LEVEL then
				local AddAttrValue = AddAttrInst.Val
				local addattrRandDef = getAddAttrRandDef( math.floor(itemDef.WearPre.RoleLevelMin/10), itemDef.Important )
				local base = newAddAttrDef.Val1[ addattrRandDef.ValLevel - 1 ];
				local mul = getNewAddAttrMul( newAddAttrDef, itemDef.WearPos1 );
				if mul > 0 then
					AddAttrValue = math.floor( base*AddAttrValue/100*mul/100 );
					nLv =( nLv - AddAttrValue ) < 1 and 1 or ( nLv - AddAttrValue )
				end				
			end
		end
		for i=1,item:getArmWashAddAttrNum() do
			local AddAttrInst	= item:getArmWashAddAttrInst(i-1);
			local newAddAttrDef = getNewAddAttrDef( AddAttrInst.AddAttrID );
			if newAddAttrDef ~= nil and newAddAttrDef.AddAttrResultID == ADD_ATTR_RESULT_DEC_WEAR_LEVEL then
				local AddAttrValue = AddAttrInst.Val
				local addattrRandDef = getAddAttrRandDef( math.floor(itemDef.WearPre.RoleLevelMin/10), itemDef.Important )
				local base = newAddAttrDef.Val1[ addattrRandDef.ValLevel - 1 ];
				local mul = getNewAddAttrMul( newAddAttrDef, itemDef.WearPos1 );
				if mul > 0 then
					AddAttrValue = math.floor( base*AddAttrValue/100*mul/100 );
					nLv =( nLv - AddAttrValue ) < 1 and 1 or ( nLv - AddAttrValue )
				end				
			end
		end
	end
	return nLv
end

function GetUseLvColr( itemDef, item )
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	if itemDef.UseTarget == USE_TARGET_PET then
		--宠物等级
		local petPackage = MainPlayer:getPet();
		for i = 1, petPackage:getPetNum() do			
			if petPackage:isPetInState( i - 1, CLIENT_PET_STAT_FIGHT ) then
				local petInfo = petPackage:getPetInfo( i - 1 );
				if petInfo.Level >= itemDef.WearPre.RoleLevelMin then
					return GetColor()[COLOR_WHITE];
				end
			end
		end
		return GetColor()[COLOR_RED];
	else
		local nLv = GetArmWearLv( itemDef, item )
		if MainPlayer:getLv() >= nLv then
			if nLv < itemDef.WearPre.RoleLevelMin then
				return GetColor()[COLOR_GREEN];
			else
				return GetColor()[COLOR_WHITE];
			end			
		else
			return GetColor()[COLOR_RED];
		end
	end	
end

--获取职业需求颜色
function GetCareerColor(itemDef)
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end

	if itemDef.WearPre.Career == nAllCareer or itemDef.WearPre.Career == MainPlayer:getRace() or itemDef.WearPre.Career == MainPlayer:getRace() % 16 then
		return GetColor()[COLOR_WHITE];
	else
		return GetColor()[COLOR_RED];
	end
end

function GetGeniusColor(itemDef)
	if GeniusMgr:getGeniusTypeValue(0) < itemDef.WearPre.GeniusLimit then
		return GetColor()[COLOR_RED];
	else
		return GetColor()[COLOR_WHITE];
	end
end

function GetSchoolValLimit( itemDef )
	if itemDef.WearPos1 == 9 then
		return itemDef.WearPre.SchoolValLimit;
	end
	return "";
end

function GetItemRespact( itemDef )
	if itemDef.NeedRespact.ID ~= 0 then
		local respactPartDef = GameRespact:getRespactPartyDef( itemDef.NeedRespact.ID );
		return "需要\b"..respactPartDef.PartyName.."-"..GetRespactParseDesc( itemDef.NeedRespact.Value )[1];
	end
	return "";
end

function GetRespacColor( itemDef )
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	if CanSatisfyDesignation( itemDef ) then
		return GetColor()[COLOR_WHITE];
	else
		return GetColor()[COLOR_RED];
	end
end

function GetItemTech(itemid)
	local tech = NpcShop:getRequireTech(itemid)
	if tech.num ~= 0 then
		local t = {}
		for i = 0, tech.num - 1 do
			local id, level = tech.id[i], tech.level[i]
			local clanTech = GameClanManager:getClanTechDef(id, level)
			local name = clanTech and clanTech.Name or ""
			table.insert(t, string.format("%s%d级", name, level))
		end
		return "需要科技\b"..table.concat(t, "、")
	end
	return "";
end

function GetItemTechColor(itemid)
	if CanSatisfyTech(itemid) then
		return GetColor()[COLOR_WHITE]
	end
	return GetColor()[COLOR_RED]
end

function GetDur( item, itemDef )
	if item ~= nil and item:getItemDur() >= 0 then
		local nCurDur = item:getItemDur(); -- 耐久度
		local nMaxDur = item:getItemMaxDur();
		if nMaxDur <=  0 then
			return "";
		else
			return ("耐 久 度："..nCurDur.."/"..nMaxDur);
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

function UpdateRideTipsRich( data )
	local szRideDesc = data["text"]
	local t_riches = 
					{ 
						["RideTipsFrameRideDescRich"] = { value = szRideDesc, color =  { r = 255, g = 255, b = 255 } }, 
					};
	util.SetRiches( t_riches );

	local rich = getglobal( "RideTipsFrameRideDescRich" );
	local nHeight = rich:GetTotalHeight();
	nHeight = ( nHeight == 0 ) and 1 or nHeight;
	
	rich:resizeRichHeight( math.ceil( nHeight + 4 * GetScreenScaleY() ) );
	--frame:resizeRect( math.ceil( nWidth + 6 * GetScreenScaleY() ), math.ceil( nHeight + 6 * GetScreenScaleY() ) );
	--frame:SetSize( math.ceil( (frame:GetRealWidth()+ 2) / GetScreenScaleY() ),  math.ceil( (frame:GetRealHeight() + 2 ) / GetScreenScaleY() ) );
	rich:SetHeight( math.ceil( nHeight + 4 * GetScreenScaleY() ) / GetScreenScaleY() );

	local t_riches = 
					{ 
						["RideTipsFrameRideDescRich"] = { value = szRideDesc, color =  { r = 255, g = 255, b = 255 } }, 
					};
	util.SetRiches( t_riches );
end

-- 显示骑乘图标tips
function ShowRideTips( t_rideInfo )
	local SPEED_UNIT		= 1;
	local rideInfo			= t_rideInfo["rideInfo"];
	local rideDef			= getRideDef( t_rideInfo.id, t_rideInfo.lv );
	local mainplayer		= ActorMgr:getMainPlayer();
	local rider				= mainplayer:getRider();
	local ridePinZhiDef		= rider:getRidePinZhiDef( rideDef.PinZhi );
	local nIncrSpeedScale	= math.ceil( rideDef.RideSpd * 100 / CLIENT_BASE_MSPD );
	local nIntegralPart		= math.modf( nIncrSpeedScale / SPEED_UNIT );
	
	nIncrSpeedScale			= SPEED_UNIT * nIntegralPart;
	
	local szEndTime = "";
	if rideInfo.Endtime ~= 0 then
		szEndTime = "有效期至"..os.date("%Y", rideInfo.Endtime).."-"..os.date("%m", rideInfo.Endtime).."-"..os.date("%d", rideInfo.Endtime).."  "
					..os.date("%H", rideInfo.Endtime)..":"..os.date("%M", rideInfo.Endtime);
	end

	--if isValidEnd(t_rideInfo) then
	if rideInfo:isValidEnd() then
		szEndTime = szEndTime .. " 已过期"
	end
	
	local t_fonts = 
					{
						--["RideTipsFrameEndTimeFont"]	= { value = szEndTime },
						["RideTipsFrameEndTimeFont"]	= { value = "" },
						--["RideTipsFrameIncrSpeedFont"]	= { value = "移动速度提升" .. nIncrSpeedScale .. "%" },
						["RideTipsFrameIncrSpeedFont"]	= { value = "" },

						["RideTipsFrameNameFont"]		= { value = rideDef.RideName .. rideDef.Level .. "级" },
						["RideTipsFramePinZhiFont"]		= { value = "品质：" .. ridePinZhiDef.PinZhiName },
					};
	util.SetFonts( t_fonts );
	RideTipsFrameNameFont:SetTextColor( t_rideColor[rideDef.PinZhi][1], t_rideColor[rideDef.PinZhi][2], t_rideColor[rideDef.PinZhi][3] );
	
	local szRideDesc = "";
	if szEndTime ~= "" then
		szRideDesc = "#cC04CFD"..szEndTime.."#n";
	end

	if rideDef.RideDesc ~= "" then
		if szRideDesc ~= "" then
			szRideDesc = szRideDesc.."\n"..rideDef.RideDesc;
		else
			szRideDesc = rideDef.RideDesc;
		end
	end
	
	local szIncrSpeedDesc = "#c00ff00骑乘后提升人物" .. nIncrSpeedScale .. "%的移动速度#n";
	if szRideDesc ~= "" then
		szRideDesc = szRideDesc.."\n"..szIncrSpeedDesc;
	else
		szRideDesc = szIncrSpeedDesc;
	end

	local szHelpDesc = "#cffff00按住鼠标左键拖拽可放置到快捷栏";
	if szEndTime ~= "" then
		szHelpDesc = "#cffffff可通过使用相同的坐骑道具来相应延长有效期\n" .. szHelpDesc;
	end
	if szRideDesc ~= "" then
		szRideDesc = szRideDesc.."\n"..szHelpDesc;
	else
		szRideDesc = szHelpDesc;
	end

	--print( "开始--"..szRideDesc.."--结束" );
	
	UpdateRideTipsRich( { ["text"] = szRideDesc } );

	local IconPath = GetItemEquipIconPath()..rideDef.IconID..".tga";
	-- 如果没找到物品的图标则使用默认图标显示
	if not IsInExistence( IconPath ) then
		IconPath = DEFAULT_ITEM_PATH;
	end

	local t_tgas =	{
						["RideTipsFrameRideTex"] = { path = IconPath }
					}; 
	util.SetTgas( t_tgas );
	
	RideTipsFrame:SetClientString( t_rideInfo.relFrame );
	RideTipsFrame:Show();
	util.AdjustBigtipsScreenPos( { tipsFrame = "RideTipsFrame", button = t_rideInfo["button"] } );
end

--参数说明: 提示信息类型(物品超链接, 非超链接), 内容类型(ItemID_Type, ItemDef_Type, Item_Type), 内容(ItemID, ItemDef, Item)
function ShowItemTips( nTipsType, nContentType, content, onenterfuc, szOnEnterBtn, isShopItem, isStallItem, HoleGemData, storeGridIdx )
	onEnterFuc		= onenterfuc;
	onEnterBtn		= szOnEnterBtn;
	NormalItemTipsFrame:SetClientString("");
	local item = nil;
	local nItemType;
	if nContentType == LUA_ITEMTIPS_ID then
		local MainPlayer = ActorMgr:getMainPlayer();
		if MainPlayer == nil then return end
		local container = MainPlayer:getContainer();
		if container == nil then return end
		item = container:getItem(content);
	elseif nContentType == LUA_ITEMTIPS_ITEMDEF then
		if content.ItemID == 0 then
			return;
		end
	elseif nContentType == LUA_ITEMTIPS_ITEM then
		item = content;
		nItemType = item:getItemType();
	end

	-- 判断item是否为空
	if item and item:getItemId() <= 0 then
		return;
	end

	local itemDef;
	if nContentType == LUA_ITEMTIPS_ITEMDEF or nContentType == LUA_ITEMTIPS_SHORTCUT then
		itemDef = content;
	elseif nContentType == LUA_ITEMTIPS_ITEMTABLE then
		itemDef			=	content[1];
		if content[2] ~= 0 then
			nEndItemTime	=	content[2];
		end
	elseif item:getItemType() == ITEM_SLOTTYPE_NONE then
		return;
	elseif item:getItemId() > 0 then
		itemDef = item:getItemDef();
	end

	-- 判断item是否为空
	if itemDef == nil then
		return;
	end

	if nTipsType == LUA_ITEMTIPS_TYPE_LINK then
		if NormalItemTipsFrame:IsShown() then
			NormalItemTipsFrame:Hide();
		end
		szSelfName = "LinkItemTipsFrame"
	elseif nTipsType == LUA_ITEMTIPS_TYPE_NORMAL then
		LinkItemTipsFrame_OnHie();
		szSelfName = "NormalItemTipsFrame"
	end

	--设置Tips
	if not nItemType then
		nItemType = getItemType(itemDef.ItemID); --获取物品类型
	end
	if nItemType == ITEM_SLOTTYPE_AMM then
		SetAmmTips( nTipsType, item, itemDef, szSelfName, isShopItem, isStallItem, storeGridIdx );
	elseif nItemType == ITEM_SLOTTYPE_ITEM then
		SetItemTips( nTipsType, item, itemDef, szSelfName, isShopItem, isStallItem, HoleGemData, storeGridIdx );
	end

	ShowTips(szSelfName, itemDef, nTipsType, item, isStallItem, storeGridIdx ); --显示行对象

	if nTipsType == LUA_ITEMTIPS_TYPE_LINK then
		local btn = getglobal(szSelfName.."ITCloseBtn");
		btn:Show();
		LinkItemTipsFrame:Show();
	elseif nTipsType == LUA_ITEMTIPS_TYPE_NORMAL then
		NormalItemTipsFrame:Show();
		nEndItemTime = 0;
	end
	
	local systemSetting	= GameMgr:getUserSystemConfig();
	if onenterfuc~=WearformOnEnter and systemSetting:getGameOpt( GAME_OPT_SHOW_EQUIP_SHOW ) and itemDef.WearPosNum ~= 0 
		and itemDef.WearPos1 < EQUIP_POS_END and itemDef.ExtendedType == ITEM_EXTENDED_TYPE_NONE then
		local MainPlayer		= ActorMgr:getMainPlayer();
		local Eqiup				= MainPlayer:getEquip();
		local nWearPos			= itemDef.WearPos1;
		local nSecondWearPos	= 0;
		local equipitem			= Eqiup:getItem( nWearPos );  
		if nWearPos == EQUIP_POS_RING then
			equipitem = Eqiup:getItem( EQUIP_POS_RING1 ); 
			nSecondWearPos = EQUIP_POS_RING2;
			if equipitem:getItemDef() == nil then
				equipitem = Eqiup:getItem( EQUIP_POS_RING2 ); 
				nSecondWearPos = EQUIP_POS_RING1;
			end
		elseif nWearPos == EQUIP_POS_AMULET then
			equipitem = Eqiup:getItem( EQUIP_POS_AMULET1 ); 
			nSecondWearPos = EQUIP_POS_AMULET2;
			if equipitem:getItemDef() == nil then
				equipitem = Eqiup:getItem( EQUIP_POS_AMULET2 ); 
				nSecondWearPos = EQUIP_POS_AMULET1;
			end
		end 
		if IsFashionItem( itemDef ) then 
			nWearPos		= GetFashionSuitPos( itemDef.WearPos1 );
			equipitem 		= Eqiup:getItem( nWearPos ); 
		end
		if equipitem:getItemDef() ~= nil then
			SetSefAmmTips( nTipsType, equipitem, equipitem:getItemDef(), "ArmItemTipsFrame" );
			ShowSelTips( "ArmItemTipsFrame", equipitem:getItemDef() ); --显示行对象
			ArmItemTipsFrame:Show(); 

			local secondequipitem = Eqiup:getItem( nSecondWearPos );
			if nSecondWearPos ~= 0 and secondequipitem:getItemDef() ~= nil then
				SetSecondSefAmmTips( nTipsType, secondequipitem, secondequipitem:getItemDef(), "SecondArmItemTipsFrame" );
				ShowSecondSelTips( "SecondArmItemTipsFrame", secondequipitem:getItemDef() ); --显示行对象
				SecondArmItemTipsFrame:Show(); 
			else
				SecondArmItemTipsFrame:Hide(); 
			end
		end
	end
	SetItemTipsPoint( szSelfName, itemDef, szOnEnterBtn ); --设置位置
end

function GetCareer( nType )
	if nType == nAllCareer then
		return "";
	elseif nType == nWarrior then
		return "战士";
	elseif nType == nMagician then
		return "法师"
	elseif nType == nGunner then
		return "枪手"
	elseif nType == nChurch then
		return "祭司"
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
	moneytex = getglobal( name.."ITItemMoneyTex1");
	moneytex:Hide();

	t_TipsLineList = {};

	t_SelfTipsLineList = {};
	-- 清空tip上的宝石图标
	for i = 1 , MAXGEMTEX do
		local AmmGemTex = getglobal(name.."InLayGemItemTex"..i); -- 镶嵌宝石图
		local lineback	= getglobal(name.."InLayGemItemTex"..i.."_Rim");
		lineback:Hide();
		AmmGemTex:Hide();
	end
	
	for i = 1 , MAX_STAR_NUM do
		local starTex = getglobal(name.."StarTex"..i);
		starTex:Hide();
	end
end

function ItemTips_OnShow()
end

function ItemTips_OnUpdate()
	if this:GetName() == "LinkItemTipsFrame" then 
		return;
	end
	if onEnterFuc ~= nil then
		if onEnterBtn ~= nil then
			onEnterFuc( onEnterBtn );
		else
			onEnterFuc();
		end
	end
end

function ItemTips_OnHide()
	if this:GetName() == "SecondArmItemTipsFrame" then
		return;
	end

	if ArmItemTipsFrame:IsShown() then
		ArmItemTipsFrame:Hide();
	end
	if SecondArmItemTipsFrame:IsShown() then
		SecondArmItemTipsFrame:Hide();
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

--增加显示的内容
function AddTipsLine( nAreaType, szText, content, t_Color, nType, bAlwayDis, nResultIndex ) --区域，显示内容，颜色值1,颜色值2,颜色值3,类型,是否总是显示
	if bAlwayDis == false and ( not content or content == "" or tonumber(content) == 0 ) then 
		return;
	end  --当是0的时候不显示
	if szText == nil then
		return;
	end
	local t_TipsLineInfo = { nAreaType,szText,content, t_Color, nType, nResultIndex };
	table.insert(t_TipsLineList, t_TipsLineInfo);
end

--获取技能消息
local t_SkillsInfo = {};

--获取装备类型的描述
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

-- 获取装备是否可交易描述
function GetCanExChange( item, itemDef )
	local szDesc = "";
	if itemDef.NoDeal == 1 then
		szDesc = "不可交易";
	end

	if item ~= nil then
		if item:isItemBind() or ( itemDef.ArmBind == ITEM_GET_BIND or itemDef.ArmBind == ITEM_WEAR_BIND ) then
			szDesc = "不可交易";
		end
	end

	return szDesc;
end

-- 获取装备绑定描述
function GetArmBind( item, itemDef )
	if item ~= nil then
		if item:isItemBind() then
			return t_ArmBind[4];
		end
	elseif NormalItemTipsFrame:GetClientUserData(1) == 1 or NormalItemTipsFrame:GetClientUserData(1) == 3 then
		return t_ArmBind[3];
		
	end
	return t_ArmBind[itemDef.ArmBind + 1];
end

-- 获取基本属性值 修正
function GetBaseAttrValue( item, nValue, nResultId )
	local nAddAttrValue = "";
	if item ~= nil and item:getItemType() == ITEM_SLOTTYPE_AMM then
		nAddAttrValue = item:getArmStarAddValue( nResultId );
		if nAddAttrValue > 0 then
			nAddAttrValue = FormatItemAddAttrValue( { ["value"] = nAddAttrValue, ["resultID"] = nResultId } );
			nAddAttrValue =  "#c42a83d+"..nAddAttrValue.."%#n";
		end
	end
	if nAddAttrValue == 0 then
		nAddAttrValue="" 
	end
	if nResultId == RESULT_WEAPON_ATK_MUL or nResultId == RESULT_WEAPON_DEF_MUL then
		return  string.format("+%s%%", nValue);
	end
	
	local szAttrValue = nValue..nAddAttrValue;
	return  szAttrValue;
end

-- 获取出售给商店的价格
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
					maxMagicAtk = maxMagicAtk > minMagicAtk and maxMagicAtk or minMagicAtk
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
					szDesc = "物理攻击"
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
					szDesc = "魔法攻击";
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
					szDesc = "双系攻击";
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

function GetEquipAppriaise( itemDef, item )
	local grade = 0;
	if item ~= nil then
		--装备基础属性评分
		local baseGrade = 0;
		local valSum = 0;
		local t_BaseAttrValList = {};
		for i=1,MAX_ITEM_RESULT do
			if IsEquipNewBaseAttr( itemDef.Result[i-1].ResultID ) then
				local val = item:getItemMulVal( itemDef.Result[i-1].ResultID );
				table.insert( t_BaseAttrValList, val );
				valSum = valSum + val;
			end
		end
		local baseVal = 0;
		if valSum > 0 and table.getn( t_BaseAttrValList ) > 0 then
			baseVal = math.floor(valSum) / table.getn( t_BaseAttrValList );
		end
		baseGrade = ( 10 + 5 * baseVal / 100 ) * 1.04 ^ ( itemDef.Important * 8 + itemDef.WearPre.RoleLevelMin - 1 );
		
		-- 装备关键字评分
		local AddAttrGrade = 0;
		local nAddAttrNum	= item:getArmAddAttrNum();
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
					AddAttrValue = base*AddAttrValue/100*mul/100 ;
					if itemDef.ItemLevel > 0 then
						local tAddattrVal = { AddAttrValue, newAddAttrDef.Val2, newAddAttrDef.Val3, newAddAttrDef.Val4 }
						local szAddAttrDesc = getNewAttrDesc( AddAttrID, tAddattrVal );
						local val = tonumber( string.sub( szAddAttrDesc, string.find( szAddAttrDesc, "%d" ), string.len( szAddAttrDesc ) ) )
						local C = 1;
						if newAddAttrDef.AddAttrResultID == ADD_ATTR_RESULT_HEAVY_HIT_VAL or newAddAttrDef.AddAttrResultID == ADD_ATTR_RESULT_HEAl_HP_MUL then
							val = tonumber( string.sub( szAddAttrDesc, string.find( szAddAttrDesc, "%d" ), string.len( szAddAttrDesc ) - 1 ) )
							val = val * 100
							C = ( 1.05 ^ itemDef.WearPre.RoleLevelMin ) / 100
						end
						if val ~= nil then
							AddAttrGrade = AddAttrGrade + C * ( val * newAddAttrDef.Score );
						end
					end
				end
			end
		end		
		grade =  math.floor(baseGrade)  +  math.floor(AddAttrGrade)
	end 
	return grade;
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
			local t_oneEquipElem = { FIGHT_AREA, "", "基础属性：", GetColor()[COLOR_OGRANGE], LINE_TYPE_TEXT, false };	
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
		local t_oneEquipElem = { FIGHT_AREA, "", "基础属性：", GetColor()[COLOR_OGRANGE], LINE_TYPE_TEXT, false };	
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

-- 获取新关键字部位修正
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
	elseif wearpos == EQUIP_POS_FASHION_BACK then 
		return newAddAttrDef.WearPosLimit.Back;
	elseif wearpos == EQUIP_POS_BADGE then
		return newAddAttrDef.WearPosLimit.Badge;
	end
	return 0
end

-- 是否是时装
function IsFashionItem( itemdef )
	if itemdef.WearPos1 == EQUIP_POS_FASHION_BODY or itemdef.WearPos1 == EQUIP_POS_FASHION_HEAD 
		or itemdef.WearPos1 == EQUIP_POS_FASHION_HAIR or itemdef.WearPos1 == EQUIP_POS_FASHION_BACK
		or itemdef.WearPos1 == EQUIP_POS_FASHION_SHOES or itemdef.WearPos1 == EQUIP_POS_FASHION_HAND
		or itemdef.WearPos1 == EQUIP_POS_FASHION_LEG then
		
		return true;
	end
	return false;
end

local function setArmLevelUpInfo( t_EquipInfo, itemDef )
	local szText = "";
	local bShowText = false;
	if itemDef.NoResolve == 0 then
		szText = szText .. "#G可以分解#n\t"
		bShowText = true;
	end	
	local itemID = itemDef.ItemID;
	local armLevelUpDef = getArmLevelUPDef( itemID );
	--橙色，但是有ArmLevelUpDef定义的装备
	if itemDef.Important == 7 and armLevelUpDef ~= nil then
		t_oneEquipElem = {SUIT_AREA, "", szText.."#cf57300可升档为80传奇装备#n", GetColor()[COLOR_GREEN],LINE_TYPE_RICH,false};
		table.insert( t_EquipInfo, t_oneEquipElem );
		return;
	end
	-- NextType小于5的不显示
	if armLevelUpDef == nil or armLevelUpDef.NextType < ITEM_IMPORTANT_EPIC_A then 
		if bShowText then 
			t_oneEquipElem = {SUIT_AREA, "", szText, GetColor()[COLOR_GREEN],LINE_TYPE_RICH,false};
			table.insert( t_EquipInfo, t_oneEquipElem );
		end
		return;
	end
	-- ffc5cf \ f57300
	local t_LevelUpDes = {
		[5] = { des = "史诗套装", color = "#cffb0ff" },
		[6] = { des = "英雄套装", color = "#cffb0ff" },
		[7] = { des = "传奇套装", color = "#cf57300" },
	}
	local nextType = armLevelUpDef.NextType 
	if nextType == ITEM_IMPORTANT_LEGEND then 
		szText = szText .. t_LevelUpDes[armLevelUpDef.NextType]["color"] .. "可炼铸成为" .. t_LevelUpDes[armLevelUpDef.NextType]["des"].."#n"
	else 
		szText = szText .. t_LevelUpDes[armLevelUpDef.NextType]["color"] .. "可进阶成为" .. t_LevelUpDes[armLevelUpDef.NextType]["des"].."#n"
	end
	
	t_oneEquipElem = {SUIT_AREA, "",szText, GetColor()[COLOR_GREEN],LINE_TYPE_RICH,false};
	table.insert( t_EquipInfo, t_oneEquipElem );
end


--是否是结婚戒指
function IsMarryRing( ItemID )
	if ItemID == 2911000 then
		return true
	end
	return false
end

function divide2lines( rich, text)
	if not rich or not text or text == "" then
		return "";
	end
	
	local richWidth = rich:GetWidth();
	local totalWidth = rich:GetTextExtentWidth( text );
	
	if totalWidth <= richWidth then
		return "\b" .. text;
	end
	
	local lines 	= "";
	local sp 	= 1;
	
	for i = 1, math.ceil( totalWidth/richWidth ) do
		local line 	= "\b";
		local j 	= sp;
		while j <= string.len( text ) do
			local str;
			if IsDBCSHead( text, j - 1 ) then
				str = string.sub( text, j, j + 1 );
				j = j + 2;
			else
				str = string.sub( text, j , j );
				j = j + 1;
			end
			local ls = line .. str;
			if rich:GetTextExtentWidth( ls .. "喔" ) >= richWidth then
				lines = lines .. line .. "\n";
				sp = j - string.len( str );
				break;
			else
				line = ls;
			end
			if rich:GetTextExtentWidth( line ) <= richWidth and j > string.len( text ) then
				lines = lines .. line;
			end
		end
	end
	
	return lines;
end

--获取装备消息
function GetEquipInfo( item, itemDef, nTipsType, isShopItem, isStallItem, isSelffArm, storeGridIdx )
	local szItemName = itemDef.Name;
	--结婚戒指，不要前缀
	if IsMarryRing( itemDef.ItemID ) or bitAnd(  itemDef.CtrlFlag,  ITEM_NO_SHOW_PREFIX_TOLUA ) then
	elseif item ~= nil then
		szItemName = item:getDisplayName();
	else
		szItemName = getDisplayName(itemDef);
	end
	if item ~= nil and item:isArmIdentity() then 
		szItemName = szItemName.."(未鉴定)";
	end
	if itemDef == nil then
		return;
	end

	local t_NeedChangeToValue = {};
	if itemDef.ItemLevel ~= 0 then	 
		local AddAttrBaseDef = getAddAttrBaseDef( itemDef.ItemLevel );
		t_NeedChangeToValue = 
		{ 
			{ ADD_ATTR_RESULT_HITRATE,			AddAttrBaseDef.HitRateBase	},		-- /* 命中率 */
			{ ADD_ATTR_RESULT_PHSIC_ATK,		AddAttrBaseDef.PhsicAtkBase	},		-- /* 物理攻击力上升 */
			{ ADD_ATTR_RESULT_ICE_ATK,			AddAttrBaseDef.IceAtkBase	},		-- /* 魔法攻击力上升 */
			{ ADD_ATTR_RESULT_PHSIC_DEF,		AddAttrBaseDef.PhsicDefBase	},		-- /* 物防 */
			{ ADD_ATTR_RESULT_ICE_DEF,			AddAttrBaseDef.IceDefBase	},		-- /* 魔防 */
			{ ADD_ATTR_RESULT_ADD_DEF,			AddAttrBaseDef.IceDefBase	},		-- /* 双防 */
			{ ADD_ATTR_RESULT_ATK,				AddAttrBaseDef.PhsicAtkBase	},		-- /* 攻击力上升 */
			{ ADD_ATTR_RESULT_DEF_PHISICHURT,	AddAttrBaseDef.DefPhsicHurtBase},	-- /* 物理伤害减免 */
			{ ADD_ATTR_RESULT_DEF_ICEHURT,		AddAttrBaseDef.DefIceHurtBase},		-- /* 魔法伤害减免 */
			{ ADD_ATTR_RESULT_DEF_ICE_PHISIC,	AddAttrBaseDef.DefIceHurtBase},		-- /* 双抗 */
			{ ADD_ATTR_RESULT_FLEE,				AddAttrBaseDef.FleeBase		},		-- /* 闪避等级 */
			{ ADD_ATTR_RESULT_HP,				AddAttrBaseDef.MaxHPBase	},		-- /* 最大血量基础值 */
			{ ADD_ATTR_RESULT_MP,				AddAttrBaseDef.MaxMPBase	},		-- /* 最大魔法基础值 */
			{ ADD_ATTR_RESULT_LILIANG,			AddAttrBaseDef.LiLiang		},		-- /* 力量 */
			{ ADD_ATTR_RESULT_ZHILI,			AddAttrBaseDef.LiLiang		},		-- /* 智力 */
			{ ADD_ATTR_RESULT_TIZHI,			AddAttrBaseDef.TiZhi		},		-- /* 体质 */
			{ ADD_ATTR_RESULT_FANGHU,			AddAttrBaseDef.FangHu		},		-- /* 防护 */
			{ ADD_ATTR_RESULT_JINGSHEN,			AddAttrBaseDef.JingShen		},		-- /* 精神 */
			{ ADD_ATTR_RESULT_MINJIE,			AddAttrBaseDef.MinJie		},		-- /* 敏捷 */
			{ ADD_ATTR_RESULT_BASE_ATTR_HEAVYHIT,AddAttrBaseDef.HeavyHit	},		-- /* 暴击等级 */
			{ ADD_ATTR_RESULT_DEF_HEAVY_HIT,	AddAttrBaseDef.DefHeavyHit	},		-- /* 暴击抵抗 */
			{ ADD_ATTR_RESULT_WATERATK,			AddAttrBaseDef.WaterAtk		},		-- /* 水攻击 */
			{ ADD_ATTR_RESULT_WATERDEF,			AddAttrBaseDef.WaterDef		},		-- /* 水防御 */
			{ ADD_ATTR_RESULT_FIREATK,			AddAttrBaseDef.FireAtk		},		-- /* 火攻击 */
			{ ADD_ATTR_RESULT_FIREDEF,			AddAttrBaseDef.FireDef		},		-- /* 火防御 */
			{ ADD_ATTR_RESULT_THUNDERATK,		AddAttrBaseDef.ThunderAtk	},		-- /* 雷攻击 */
			{ ADD_ATTR_RESULT_THUNDERDEF,		AddAttrBaseDef.ThunderDef	},		-- /* 雷防御 */
			{ ADD_ATTR_RESULT_LIGHTATK,			AddAttrBaseDef.LightAtk		},		-- /* 光攻击 */
			{ ADD_ATTR_RESULT_LIGHTDEF,			AddAttrBaseDef.LightDef		},		-- /* 光防御 */
			{ ADD_ATTR_RESULT_NIGHTATK,			AddAttrBaseDef.NightAtk		},		-- /* 暗攻击 */
			{ ADD_ATTR_RESULT_NIGHTDEF,			AddAttrBaseDef.NightDef		},		-- /* 暗防御 */

			{ ADD_ATTR_RESULT_IGNORE_DEF,		AddAttrBaseDef.IgnoreDef	},		-- /* 忽视防御，值(公式算的实际忽视的防御) */
			{ ADD_ATTR_RESULT_REFLECTION_HP,	AddAttrBaseDef.ReflectHP	},		-- /* 反射造成的血量损失值(公式算的实际值) */
			{ ADD_ATTR_RESULT_DEF_HP,			AddAttrBaseDef.DefHP		},		-- /* 抵抗造成的血量伤害，值(公式算的实际值) */			
		};		
	end
	
	local t_oneEquipElem;
	--针对装备类型和装备位置描述相同的处理
	local szItemType;
	--														护符也不要添加后边的描述
	if GetItemType(itemDef.ItemID) ~= GetWearPos(itemDef) and itemDef.WearPos1 ~= 10 then
		szItemType = GetItemType(itemDef.ItemID).."("..GetWearPos(itemDef)..")";
	else
		szItemType = GetItemType(itemDef.ItemID);
	end
	local t_EquipInfo = {
		{ BASE_AREA1, "",			szItemName,						GetItemNameColor(itemDef),		LINE_TYPE_HEADTEXT,	true },		--装备名字
		{ BASE_AREA1, "",			GetArmBind( item,itemDef ),				GetColor()[COLOR_ITEMSTART],		LINE_TYPE_BINDTEXT,	false },	--物品当前绑定状况
		{ BASE_AREA1, "装备等级：",		GetArmWearLv(itemDef, item) .."级",			GetUseLvColr(itemDef, item),		LINE_TYPE_TEXT,		true },		--装备等级
		{ BASE_AREA1, "基础天赋值：",		itemDef.WearPre.GeniusLimit,				GetGeniusColor(itemDef),		LINE_TYPE_TEXT,		false },	--天赋值
		{ BASE_AREA1, "",			GetDur(item, itemDef),					GetColor()[COLOR_WHITE],		LINE_TYPE_TEXT,		false },	--耐久度
		{ BASE_AREA1, "职业需求：",		GetCareer(itemDef.WearPre.Career),			GetCareerColor(itemDef),		LINE_TYPE_TEXT,		false },	--职业 GetCareer(itemDef.WearPre.Career)
		{ BASE_AREA1, "",			"装备类型："..szItemType,				GetColor()[COLOR_WHITE],		LINE_TYPE_TEXT,		false },	--装备类型 GetItemType(itemDef.ItemID)
		{ BASE_AREA1, "传承点数需求：",			GetSchoolValLimit(itemDef),				GetRespacColor(itemDef),		LINE_TYPE_TEXT,		false },
		{ BASE_AREA1, "",			GetItemRespact(itemDef),				GetRespacColor(itemDef),		LINE_TYPE_TEXT,		false },
		{ BASE_AREA1, "",			GetItemTech(itemDef.ItemID),				GetItemTechColor(itemDef.ItemID),	LINE_TYPE_TEXT,		false },
	}

	if bitAnd( itemDef.CtrlFlag, ITEM_NO_LEVELLIMIT ) then
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
				local MAX_STAR_MUL_LIST = 10
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
	if GetItemTypeName( math.floor( itemDef.ItemID / 10000 ) ) ~= "时装" then
		local t_EquipGradeInfo = { BASE_AREA1, "装备基础评分：", GetEquipAppriaise( itemDef, item ), GetColor()[COLOR_YELLOW], LINE_TYPE_TEXT, false };
		table.insert( t_EquipInfo,  t_EquipGradeInfo );
	else
		t_EquipGradeInfo = { OTHER_AREA, "", GetItemDesc( item, itemDef ), GetColor()[COLOR_WEAK_BLUE],	LINE_TYPE_RICH, false }
		table.insert( t_EquipInfo, t_EquipGradeInfo )
	end
	
	local bShowTitle = true;
	if itemDef.IdentifiabilityType == 0 then
		bShowTitle = FillAllEquipBaseAttr( { ["item"] = item, ["itemDef"] = itemDef, ["equipCache"] = t_EquipInfo, ["isShowTitle"] = bShowTitle } );
		-- 对于背饰特殊处理
		if itemDef.WearPos1 == EQUIP_POS_FASHION_BACK then
			AddFashionBackTipsInfo( itemDef,t_EquipInfo );
		-- 结婚戒指，这里什么都不加
		elseif IsMarryRing( itemDef.ItemID ) then
		-- 装备关键字
		elseif item ~= nil then
			local nAddAttrNum	= item:getArmAddAttrNum();
			local bShow			= true;
			--未鉴定 附加属性 单写
			if item:isArmIdentity() then 
				local t_oneEquipElem = { FIGHT_AREA,	"", " ",	GetColor()[COLOR_ITEMSTART], LINE_TYPE_DISTANCE, true};
				table.insert( t_EquipInfo, t_oneEquipElem );
				-- 附加属性
				t_oneEquipElem ={ FIGHT_AREA, "",	"附加属性：",	GetColor()[COLOR_OGRANGE], LINE_TYPE_TEXT, false };
				table.insert( t_EquipInfo, t_oneEquipElem );
				bShow = false;
				t_oneEquipElem = {	FIGHT_AREA, "\b", "鉴定后随机获得附加属性",
											GetColor()[COLOR_GREEN], LINE_TYPE_TEXT, false };
				
				table.insert( t_EquipInfo, t_oneEquipElem );
				t_oneEquipElem = {	FIGHT_AREA, "\b", "（炼金技能可生产制作鉴定仪）",
											GetColor()[COLOR_WHITE], LINE_TYPE_TEXT, false };
				
				table.insert( t_EquipInfo, t_oneEquipElem );
			end
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
							-- 附加属性
							t_oneEquipElem ={ FIGHT_AREA, "",	"附加属性：",	GetColor()[COLOR_OGRANGE], LINE_TYPE_TEXT, false };
							table.insert( t_EquipInfo, t_oneEquipElem );
							bShow	= false;
						end
						AddAttrValue = base*AddAttrValue/100*mul/100 ;
						if itemDef.ItemLevel > 0 then
							local tAddattrVal = { AddAttrValue, newAddAttrDef.Val2, newAddAttrDef.Val3, newAddAttrDef.Val4 }
							local t_oneEquipElem
							if item:IsAttrFengYin( i-1 ) then
								t_oneEquipElem = {	FIGHT_AREA, "\b","（封）" .. getNewAttrDesc( AddAttrID, tAddattrVal ),
											GetColor()[COLOR_GRAY], LINE_TYPE_TEXT, false };
							else
								t_oneEquipElem = {	FIGHT_AREA, "\b", getNewAttrDesc( AddAttrID, tAddattrVal ),
											GetColor()[COLOR_ITEMKEYWORLD], LINE_TYPE_TEXT, false };
							end
							table.insert( t_EquipInfo, t_oneEquipElem );
						end
					end
				end
			end
			-- 洗练属性
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
							-- 附加属性
							t_oneEquipElem ={ FIGHT_AREA, "",	"附加属性：",	GetColor()[COLOR_OGRANGE], LINE_TYPE_TEXT, false };
							table.insert( t_EquipInfo, t_oneEquipElem );
							bShow	= false;
						end
						AddAttrValue = base*AddAttrValue/100*mul/100 ;
						if itemDef.ItemLevel > 0 then
							local tAddattrVal = { AddAttrValue, newAddAttrDef.Val2, newAddAttrDef.Val3, newAddAttrDef.Val4 }
							local t_oneEquipElem
							if item:IsAttrFengYin( nAddAttrNum + i - 1 ) then
								t_oneEquipElem = {	FIGHT_AREA, "\b", "（封）" .. getNewAttrDesc( AddAttrID, tAddattrVal ),
											GetColor()[COLOR_GRAY], LINE_TYPE_TEXT, false };
							else
								t_oneEquipElem = {	FIGHT_AREA, "\b", getNewAttrDesc( AddAttrID, tAddattrVal ),
											{ 0,200,255 }, LINE_TYPE_TEXT, false };
							end
							table.insert( t_EquipInfo, t_oneEquipElem );
						end
					end
				end
			end
			--特殊属性
			if itemDef.TeshuID > 0 then
				if bShow then
					local t_oneEquipElem = { FIGHT_AREA,	"", " ",	GetColor()[COLOR_ITEMSTART], LINE_TYPE_DISTANCE, true};
					table.insert( t_EquipInfo, t_oneEquipElem );
					-- 附加属性
					t_oneEquipElem ={ FIGHT_AREA, "",	"附加属性：",	GetColor()[COLOR_OGRANGE], LINE_TYPE_TEXT, false };
					table.insert( t_EquipInfo, t_oneEquipElem );
					bShow	= false;
				end
			end
			--装备技能
			local nMax = item:getArmSkillMax();
			if nMax > 0 then
				local num = item:getArmSkillNum();
				for i=1, nMax do
					local color = { 61, 109, 168 };
					if i > num then
						color = { 120, 120, 120 };
					end
					local data = item:getArmSkillOne( i-1 );
					t_oneEquipElem = { FIGHT_AREA, "", "\b" .. getArmSkillDesc( itemDef, data ), color, LINE_TYPE_TEXT, false };
					table.insert( t_EquipInfo, t_oneEquipElem );
				end
			end

			if itemDef.TeshuID > 0 then
				local armTeshuDef = getArmTeshuDef( itemDef.TeshuID )
				if armTeshuDef ~= nil then
					local rich 	= getglobal( "NormalItemTipsFrameITItemRich1" );
					local szText	= divide2lines( rich, armTeshuDef.DefDesc );
					t_oneEquipElem = {	FIGHT_AREA, "", szText,
								GetColor()[COLOR_PURPLE], LINE_TYPE_RICH, false };
					table.insert( t_EquipInfo, t_oneEquipElem );
				end 
			end

			--间隔
			local spaceCount = 0;
			--阶层属性
			local stepLv = item:getStepLv();
			if stepLv > 0 then
				local stepDef = GameArmProce:getArmStepUpDef( math.floor(itemDef.WearPre.RoleLevelMin/10), itemDef.Important, itemDef.WearPos1 );
				if stepDef ~= nil then
					spaceCount = 1;
					local t_oneEquipElem = { FIGHT_AREA,	"", " ",	GetColor()[COLOR_ITEMSTART], LINE_TYPE_DISTANCE, true};
					table.insert( t_EquipInfo, t_oneEquipElem );

					local szText = string.format( "融合属性(%d/%d)：", stepLv, stepDef.MaxStep );
					t_oneEquipElem ={ FIGHT_AREA, "", szText, GetColor()[COLOR_OGRANGE], LINE_TYPE_TEXT, false };
					table.insert( t_EquipInfo, t_oneEquipElem );

					local itemResultDescDef = getItemResultDesc( stepDef.AttrID );
					if itemResultDescDef ~= nil then
						local t_oneEquipElem = { FIGHT_AREA, "\b"..itemResultDescDef.ResultDesc, "+" .. stepDef.StepAttr[stepLv-1].Val ,GetColor()[COLOR_ITEMKEYWORLD],	
													LINE_TYPE_TEXT, false };
						table.insert( t_EquipInfo, t_oneEquipElem );
					end
				end
			end

			--附魔属性
			local ExtendedFromDefID = item:getExtendedFromDefID();
			local extendedResult = item:getExtendedResult();			
			if ExtendedFromDefID > 0 and extendedResult ~= nil and extendedResult.ResultID ~= 0 then
				local itemDef = getItemDef( ExtendedFromDefID );

				if spaceCount ~= 1 then
					spaceCount = 1;
					local t_oneEquipElem = { FIGHT_AREA,	"", " ",	GetColor()[COLOR_ITEMSTART], LINE_TYPE_DISTANCE, true};
					table.insert( t_EquipInfo, t_oneEquipElem );
				end

				--附魔属性
				t_oneEquipElem = { FIGHT_AREA, "",	"附魔属性：",	GetColor()[COLOR_OGRANGE], LINE_TYPE_TEXT, false };	
				table.insert( t_EquipInfo, t_oneEquipElem );

				--附魔为装备效果
				if itemDef.ExtendedType == ITEM_EXTENDED_TYPE_RESULT then
					local itemResultConfig = GetOneItemResultConfig( { ["resultID"] = extendedResult.ResultID } )
					if itemResultConfig ~= nil then
						local t_ResultValue = { extendedResult.ResultVal1,extendedResult.ResultVal2,extendedResult.ResultVal3,extendedResult.ResultVal4};
						local nValue = 0;
						for k = 1, table.getn(t_ResultValue) do
							if t_ResultValue[k] ~= 0 then
								nValue = t_ResultValue[k];
								break;
							end
						end						
						local t_oneEquipElem = { FIGHT_AREA, "\b"..itemResultConfig[2].."：", "+" .. nValue ,GetColor()[COLOR_ITEMKEYWORLD],	
													LINE_TYPE_TEXT, false };
						table.insert( t_EquipInfo, t_oneEquipElem );
					end
				end
			end
			--刻印属性
			local addAttrInst = item:getAddAttrInst();
			if addAttrInst.AddAttrID > 0 then
				if spaceCount ~= 1 then
					spaceCount = 1;
					local t_oneEquipElem = { FIGHT_AREA,	"", " ",	GetColor()[COLOR_ITEMSTART], LINE_TYPE_DISTANCE, true};
					table.insert( t_EquipInfo, t_oneEquipElem );
				end

				table.insert( t_EquipInfo, { FIGHT_AREA, "",	"元素印记：",	GetColor()[COLOR_OGRANGE], LINE_TYPE_TEXT, false } );
				for j = 1, table.getn(t_tagRESULT) do
					if t_tagRESULT[j][1] == addAttrInst.AddAttrID then						
						local nValue = addAttrInst.Val;						
						local t_oneEquipElem = { FIGHT_AREA, "\b"..t_tagRESULT[j][2].."：", "+" .. nValue .. getKeyInEndDesc( item:getKeyEndTime() ),GetColor()[COLOR_WEAK_BLUE],	
													LINE_TYPE_TEXT, false };
						table.insert( t_EquipInfo, t_oneEquipElem );
					end					
				end
				table.insert( t_EquipInfo, t_oneEquipElem );
			end
			--星级属性
			local nPerfectStar = item:getArmPefectStarNum();
			if nPerfectStar >= 3 then
				local armGradeDef = getArmGradeDef( itemid_num( item:getItemDef() ) ,math.floor( item:getItemDef().WearPre.RoleLevelMin/10 ) );
				if armGradeDef ~= nil then
					local nAttrId = armGradeDef.ArmStar[nPerfectStar-1].SuitAttrID;
					if nAttrId ~= 0 then
						if spaceCount ~= 1 then
							spaceCount = 1;
							local t_oneEquipElem = { FIGHT_AREA,	"", " ",	GetColor()[COLOR_ITEMSTART], LINE_TYPE_DISTANCE, true};
							table.insert( t_EquipInfo, t_oneEquipElem );
						end

						local t_oneEquipElem = { FIGHT_AREA, "",	"星级属性：",	GetColor()[COLOR_OGRANGE], LINE_TYPE_TEXT, false };	
						table.insert( t_EquipInfo, t_oneEquipElem );
						local AddAttrDef = getAddAttrDef( nAttrId );
						t_oneEquipElem = { FIGHT_AREA,	"\b", nPerfectStar.."星完美强化："..AddAttrDef.AddAttrDesc,GetColor()[COLOR_ITEMKEYWORLD], LINE_TYPE_TEXT, false };		
						table.insert( t_EquipInfo, t_oneEquipElem );
					end
				end
			end
		else
			if NormalItemTipsFrame:GetClientUserData(0) ~= 0 then
				local bShow = true;
				local AddAttrList = getFixedAddAttr( NormalItemTipsFrame:GetClientUserData(0) );
				for i = 1, MAX_ITEM_ADD_ATTR_TOLUA do
					if AddAttrList.AddAttrs[i-1].ID ~= 0 then
						if bShow then
							local t_oneEquipElem = { FIGHT_AREA,	"", " ",	GetColor()[COLOR_ITEMSTART], LINE_TYPE_DISTANCE, true};
							table.insert( t_EquipInfo, t_oneEquipElem );
							
							--附加属性
							t_oneEquipElem = { FIGHT_AREA, "",	"附加属性：",	GetColor()[COLOR_OGRANGE], LINE_TYPE_TEXT, false };		
							table.insert( t_EquipInfo, t_oneEquipElem );
							
							bShow = false;
						end

						local AddAttrDef = getArmAddAttrDef( AddAttrList.AddAttrs[i-1].ID );
						local AttrResult = AddAttrDef.Result[AddAttrList.AddAttrs[i-1].Idx-1];  
						local t_ResultValue = { AttrResult.ResultVal1,AttrResult.ResultVal2,AttrResult.ResultVal3,AttrResult.ResultVal4};
						local nAddAttrValue = 0;
						for j = 1, table.getn(t_ResultValue) do
							if t_ResultValue[j] ~= 0 then
								nAddAttrValue = t_ResultValue[j];
								break;
							end
						end

						if itemDef.ItemLevel> 0 then
							AddAttrValue = GetAddAttrValue(t_NeedChangeToValue, AddAttrList.AddAttrs[i-1].ID, nAddAttrValue);
						end	 
						
						local szAttrDesc = getAttrDesc( AddAttrList.AddAttrs[i-1].ID, AddAttrValue  ).."--745";
						--print( "szAttrDesc = "..szAttrDesc.."--773" );

						local t_oneEquipElem = {	FIGHT_AREA, "\b", getAttrDesc( AddAttrList.AddAttrs[i-1].ID, AddAttrValue ),
													GetColor()[COLOR_ITEMKEYWORLD], LINE_TYPE_TEXT, false } ;
						table.insert( t_EquipInfo, t_oneEquipElem );
					end
				end
			else
				if itemDef.Important > 0 and GetItemTypeName( math.floor(itemDef.ItemID / 10000 ) ) ~= "时装" then
					table.insert( t_EquipInfo,{ FIGHT_AREA, "",	"附加属性：",	GetColor()[COLOR_OGRANGE], LINE_TYPE_TEXT, false } );
					table.insert( t_EquipInfo,{ FIGHT_AREA, "\b",	"随机属性",	GetColor()[COLOR_ITEMKEYWORLD], LINE_TYPE_TEXT, false } );
				end
			end
		end
		if GetItemTypeName( math.floor(itemDef.ItemID / 10000 ) ) == "徽章" then
			-- 徽章暂不做镶嵌处理
		elseif item ~= nil and item:getHoleNum() > 0 then
			local t_oneEquipElem = { FIGHT_AREA,	"", "",	GetColor()[COLOR_ITEMSTART], LINE_TYPE_DISTANCE, true};
			table.insert( t_EquipInfo, t_oneEquipElem );

			t_oneEquipElem = { FIGHT_AREA,	"宝石镶嵌状态", "",	GetColor()[COLOR_ITEMSTART], LINE_TYPE_TEXT, true};
			table.insert( t_EquipInfo, t_oneEquipElem );

			for i = 1, item:getHoleNum() do
				local nGemId		= item:getHoleGemId(i-1);
				local nResultIndex	= item:getHoleGemResultIndex( i - 1 );
				local nHoleColor	= item:getHoleGemColor(i-1);
				local nGemStar		= item:getHoleGemStar( i - 1 );
				local nAttrResultId	= item:getHoleGemAttrIdx( i - 1 );
				local nGemMul		= ( 1 + item:getHoleGemMul( i - 1 ) / 100 );
				--print( "nGemStar"..nGemStar.."nAttrResultId"..nAttrResultId);
				if nGemId ~= 0 then	  
					local gemitemdef = getItemDef( nGemId );
					--print( "nResultIndex = "..nResultIndex.."--829" );
					local t_oneEquipElem = { FIGHT_AREA,	GetGemColorName(nHoleColor).."插槽\b",	nGemId..","..nGemStar..","..nAttrResultId..","..gemitemdef.IconID..","..nGemMul ,GetColor()[COLOR_WEAK_PURPLE], LINE_TYPE_GEM_TEXTURE, true, nResultIndex };		--孔数
					table.insert( t_EquipInfo, t_oneEquipElem );
				else
					local t_oneEquipElem = { FIGHT_AREA,	GetGemColorName(nHoleColor).."插槽\b",	"未镶嵌",	GetColor()[COLOR_ITEMGEMINLAY], LINE_TYPE_GEM_TEXTURE, true };		--孔数
					table.insert( t_EquipInfo, t_oneEquipElem );
				end
			end
		elseif NormalItemTipsFrame:GetClientUserData(1) > 1 then
			local t_oneEquipElem = { FIGHT_AREA,	"", "",	GetColor()[COLOR_ITEMSTART], LINE_TYPE_DISTANCE, true};
			table.insert( t_EquipInfo, t_oneEquipElem );

			t_oneEquipElem = { FIGHT_AREA,	"宝石镶嵌状态", "",	GetColor()[COLOR_ITEMSTART], LINE_TYPE_TEXT, true};
			table.insert( t_EquipInfo, t_oneEquipElem );

			local t_HoleColor = { itemDef.Hole[3].RndRed, itemDef.Hole[3].RndBlue, itemDef.Hole[3].RndGreen, itemDef.Hole[3].RndColor };
			local nHoleColor = 0;
			for i = 1, table.getn( t_HoleColor ) do
				if t_HoleColor[i] ~= 0 then
					nHoleColor = i - 1;
				end
			end

			local t_oneEquipElem = { FIGHT_AREA,	GetGemColorName(nHoleColor).."插槽\b",	"未镶嵌",	GetColor()[COLOR_ITEMGEMINLAY], 
										LINE_TYPE_GEM_TEXTURE, true };		--孔数
			table.insert( t_EquipInfo, t_oneEquipElem );
		end

		if item ~= nil then
			--战斗力
			local t_oneEquipElem ={ BATTLE_AREA, "战斗力：", item:getFight(),	GetColor()[COLOR_BATTLE], LINE_TYPE_TEXT, false };		
			table.insert( t_EquipInfo, t_oneEquipElem );
		end

		local nSuitNum	= 0;
		local nWatchSuitNum = 0;
		local watchplayersuit = "";
		local szSuitRichInfo = "";
		--								结婚戒指，不显示套装
		if itemDef.SuitSetID > 0 and not IsMarryRing( itemDef.ItemID ) then
			local SuitSet = getSuitSet( itemDef.SuitSetID );
			local MainPlayer = ActorMgr:getMainPlayer();
			if MainPlayer ~= nil then
				local t_SuitArm = {};
				local Eqiup				= MainPlayer:getEquip();
				local SuitColor			= GetColor()[COLOR_SUITNOGET];
				nSuitNum			= Eqiup:getPlayerSuitEquipNum( itemDef.SuitSetID );
				-- 套装部位类型
				local t_oneEquipElem = { SUIT_AREA, "", SuitSet.Name.."（#WPSUITNUM#/"..getSuitEquipNum( itemDef.SuitSetID ).."）",	
											GetItemNameColor(itemDef), LINE_TYPE_TEXT, false };		
				table.insert( t_EquipInfo, t_oneEquipElem );

				for i = 1, getSuitEquipNum( itemDef.SuitSetID ) do
					local suititemdef = getItemDef( getSuitSetId( itemDef.SuitSetID, ( i - 1 ) ) );
					if nTipsType ~= LUA_ITEMTIPS_TYPE_LINK or isSelffArm ~= nil then
						for j = 1, EQUIP_POS_END do
							local EquipItem			= Eqiup:getItem(j );
							local EquipItemDef		= EquipItem:getItemDef();
							if EquipItemDef ~= nil and EquipItemDef.ItemID == getSuitSetId( itemDef.SuitSetID, ( i - 1 ) ) then
								SuitColor = GetItemNameColor(itemDef);
								break;
							end
						end
					end

					if onEnterBtn ~= nil and string.find( onEnterBtn,"WatchPlayerFrameEquip", 1, true ) then
						watchplayersuit = "#WPSUIT#"..COLOR_SUITNOGET.."#";
						if not isSelffArm then
							SuitColor = GetColor()[COLOR_SUITNOGET];
						end
						for j = 1, EQUIP_POS_END do
							local EquipItem			= GameWatchPlayer:getItem( j );
							local EquipItemDef		= EquipItem:getItemDef();
							if EquipItemDef ~= nil and EquipItemDef.ItemID == getSuitSetId( itemDef.SuitSetID, ( i - 1 ) ) then
								watchplayersuit = "#WPSUIT#" .. COLOR_DARK_GOLD .."#";
								if not isSelffArm then
									SuitColor = GetItemNameColor( EquipItemDef );
								end
								t_SuitArm[EquipItemDef.ItemID] = 1;
							end
						end
					end
					szSuitRichInfo = szSuitRichInfo..RGBColorToRichColor( SuitColor )..suititemdef.Name.."/";
					SuitColor = GetColor()[COLOR_SUITNOGET];
				end
				--套装部位类型
				--去处最后一个位置上的“/”
				szSuitRichInfo = string.sub( szSuitRichInfo, 1, string.len( szSuitRichInfo )-1 );
				local t_oneEquipElem = { SUIT_AREA, "\b",	szSuitRichInfo, GetColor()[COLOR_ITEMKEYWORLD], LINE_TYPE_RICH, false };	
				table.insert( t_EquipInfo, t_oneEquipElem );

				for _,v in pairs( t_SuitArm ) do
					nWatchSuitNum = nWatchSuitNum + v;
				end
			end

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
					local t_oneEquipElem = { SUIT_AREA,	watchplayersuit.."\b"..SuitSet.SuitAttr[i-1].MinSuitNum.."件：", AddAttrDef.AddAttrDesc,	GetColor()[SuitAttrColor], LINE_TYPE_TEXT, false };		
					table.insert( t_EquipInfo, t_oneEquipElem );
				end
			end
			--间距
			local t_oneEquipElem = { SUIT_AREA, "", "", GetColor()[COLOR_ITEMSTART], LINE_TYPE_DISTANCE, true};			
			table.insert( t_EquipInfo, t_oneEquipElem );
		end
		--添加描述显示，取itemdesc字段
		if itemDef.ItemDesc ~= "" and GetItemTypeName( math.floor( itemDef.ItemID / 10000 ) ) ~= "时装" then
			t_oneEquipElem = { SUIT_AREA, "", itemDef.ItemDesc, GetColor()[COLOR_WEAK_GREEN], LINE_TYPE_RICH, true };
			table.insert( t_EquipInfo, t_oneEquipElem );
		end
		
		--是否可交易
		local t_oneEquipElem = { SUIT_AREA,	GetCanExChange( item, itemDef ), "", GetColor()[COLOR_ITEMSTART], LINE_TYPE_TEXT, false };	
		table.insert( t_EquipInfo, t_oneEquipElem );

		--t_oneEquipElem = { SUIT_AREA, "最大强化等级：", itemDef.StarLimit, GetColor()[COLOR_ITEMSTART], LINE_TYPE_TEXT, false };
		--table.insert( t_EquipInfo, t_oneEquipElem );

		--t_oneEquipElem = { SUIT_AREA,	"装备部位：",	GetWearPos(itemDef), GetColor()[COLOR_ITEMSTART], LINE_TYPE_TEXT, false };
		--table.insert( t_EquipInfo, t_oneEquipElem );
		
		local szEndTimeDesc = SetItemEndTime(item );
		if szEndTimeDesc ~= "" then
			t_oneEquipElem = { SUIT_AREA, "有效期至", szEndTimeDesc, GetColor()[COLOR_BATTLE], LINE_TYPE_TEXT, true };
			table.insert( t_EquipInfo, t_oneEquipElem );
		end

		t_oneEquipElem = { SUIT_AREA, "商店收购价",  GetSell2ShopPrice( item, itemDef ), GetColor()[COLOR_SELLPRICE], LINE_TYPE_MONEY, false };			
		table.insert( t_EquipInfo, t_oneEquipElem );
		
		if isStallItem or storeGridIdx then
			t_oneEquipElem = { SUIT_AREA, isStallItem and "摊位售单价" or "店铺售单价", isStallItem and item:getStallSellPrice() or StoreMgr:getStoreItemPrice( storeGridIdx ),
			GetColor()[COLOR_SELLPRICE], LINE_TYPE_MONEY, false };			
			table.insert( t_EquipInfo, t_oneEquipElem );
		end		

--[[
		if itemDef.Important >= 1 and not IsFashionItem( itemDef ) and 
			( isPointInFrame( "PlayerContainerFrame" ) or isPointInFrame( "EquipFrame" ) ) then
			t_oneEquipElem = {SUIT_AREA, "",	"<shift+右键>打开镶嵌界面",	GetColor()[COLOR_GREEN],LINE_TYPE_TEXT,false};
			table.insert( t_EquipInfo, t_oneEquipElem );
		end
--]]

		if item ~= nil and item:isArmFengYin() then
			t_oneEquipElem = {SUIT_AREA, "",	"时间城的解封大师可以解封被封印的装备",	
								GetColor()[COLOR_GREEN],LINE_TYPE_TEXT,false};
			table.insert( t_EquipInfo, t_oneEquipElem );
		end

		-- nIndex+5
		setArmLevelUpInfo( t_EquipInfo, itemDef );      -- 装备可进阶 相关信息设置	
	end

	if itemDef.IdentifiabilityType ~= 0 then
		
		--添加描述显示，取itemdesc字段
		if itemDef.ItemDesc ~= "" and GetItemTypeName( math.floor( itemDef.ItemID / 10000 ) ) ~= "时装" then
			t_oneEquipElem = { FIGHT_AREA, "", itemDef.ItemDesc, GetColor()[COLOR_WEAK_GREEN], LINE_TYPE_RICH, true };
			table.insert( t_EquipInfo, t_oneEquipElem );
		end

		t_oneEquipElem = { FIGHT_AREA, "", "未鉴定", GetColor()[COLOR_WEAK_PURPLE],	LINE_TYPE_TEXT, false };
		table.insert( t_EquipInfo, t_oneEquipElem );

		local priceName = "商店收购价";
		if isStallItem then
			priceName = "摊位售单价"
		elseif storeGridIdx then
			priceName = "店铺售单价"
		end
		local sellPrice = itemDef.SellPrice;
		if isStallItem then
			sellPrice = item:getStallSellPrice();
		elseif storeGridIdx then
			sellPrice = StoreMgr:getStoreItemPrice( storeGridIdx );
		end
		t_oneEquipElem = { FIGHT_AREA, priceName, sellPrice, GetColor()[COLOR_SELLPRICE], LINE_TYPE_MONEY, false };
		table.insert( t_EquipInfo, t_oneEquipElem );
	end
	--如果是背饰
	if item ~= nil and bitAnd( itemDef.CtrlFlag, ITEM_CAN_STEP_UP_BACK_TOLUA ) then
		--而且是幻化过的背饰
		if item:getUnRealBackID() > 0 then
			local unRealBackDef = getItemDef( item:getUnRealBackID() )
			t_oneEquipElem = { FIGHT_AREA, "幻化背饰：", unRealBackDef.Name, GetColor()[COLOR_WEAK_PURPLE],	LINE_TYPE_TEXT, false };
			table.insert( t_EquipInfo, t_oneEquipElem );
			local data = item:getUnRealBackTmEnd() > 0 and os.date( "%Y-%m-%d %H:%M", item:getUnRealBackTmEnd() ) or "永久"
			t_oneEquipElem = { FIGHT_AREA, "幻化期限：", data, GetColor()[COLOR_WEAK_PURPLE],	LINE_TYPE_TEXT, false };
			table.insert( t_EquipInfo, t_oneEquipElem );
		end
	end
	return t_EquipInfo;
end

function CanDropItem(value)
	if value == 1 then
		return "不可丢弃"
	end
	return "";
end

function CanSaleItem(value)
	if value == 1 then
		return "不可出售"
	end
	return "";
end

function GetCanDealItem(value)
	if value == 1 then
		szDesc = "不可交易";
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
						szMapName = "未知区域";
					end
					szText = string.gsub(szText, "@mapid@", szMapName);
				end
				break;
			elseif itemDef.Result[i-1].ResultID == RESULT_ITEM_CLAN_CONTRI then
				if string.find( szText, "@公会贡献度@", 1, true ) then
					szText = string.gsub(szText, "@公会贡献度@", item:getItemInstVal1());
				end
				break;
			end
		end
	end
	return szText;
end

function GetPetSkillType( itemDef )
	if itemDef ~= nil then		
		local t_PetSkillDesc = { [1] = "攻击类", [2] = "佑主类", [3] = "增强类" };
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

-- 获得宝石附加属性的最大最小值
function GetGemAddAttrRange( nItemStar, itemDef )	
	local nResultValueIndex = 0;
	local t_AttrList		= {};
	local nResultID			= 0;
	local nMin				= 0;
	local nMax				= 0;
	local MAX_GEM_ATTR		= 10;
	local MAX_GEM_VAL		= 10;
	local gemNewDef			= GameArmProce:getGemNewDef( itemDef.ItemID );

	local topStar			= IsGemTopLevel( gemNewDef.Level, nItemStar );
	local nStar				= nItemStar >= topStar and topStar or nItemStar;
	for i = 1, MAX_GEM_ATTR do
		if gemNewDef.Attr[i-1].AttrID ~= 0 then
			nResultID = gemNewDef.Attr[i-1].AttrID;
			local gemAttrStarDef = GameArmProce:getGemAttrStarDef( nResultID, gemNewDef.Level, nStar );
			nMax = gemAttrStarDef.Attr[0].AttrID;
			nMin = gemAttrStarDef.Attr[0].AttrID;
			for j = 1,MAX_GEM_VAL do
				if gemAttrStarDef.Attr[j-1].AttrID > nMax then
					nMax = gemAttrStarDef.Attr[j-1].AttrID;
				end
				if gemAttrStarDef.Attr[j-1].AttrID < nMin then
					nMin = gemAttrStarDef.Attr[j-1].AttrID;
				end
			end
			local attrInfo = { nResultID, nMin, nMax };
			table.insert( t_AttrList, attrInfo );
		end
	end
	return t_AttrList;
	--[[
	for nResultIDIndex = 1,MAX_GEM_RESULT do
		local oneResultInfo = itemDef.Result[nResultIDIndex-1];
		if oneResultInfo.ResultVal1 ~= 0 then
			nMin = oneResultInfo.ResultVal1;
			nResultValueIndex = 1;
		elseif oneResultInfo.ResultVal2 ~= 0 then
			nMin = oneResultInfo.ResultVal2;
			nResultValueIndex = 2;
		elseif oneResultInfo.ResultVal3 ~= 0 then
			nMin = oneResultInfo.ResultVal3;
			nResultValueIndex = 3;
		elseif oneResultInfo.ResultVal4 ~= 0 then
			nMin = oneResultInfo.ResultVal4;
			nResultValueIndex = 4;
		end
		
		if oneResultInfo.ResultID ~= 0 then
			nResultID = oneResultInfo.ResultID;
		end
		
		if nMin ~= 0 and nResultID ~= 0 then
			break;
		end
	end	
	
	local maxResultInfo = itemDef.Result[MAX_GEM_RESULT-1];
	if nResultValueIndex == 1 then
		nMax = maxResultInfo.ResultVal1;
	elseif nResultValueIndex == 2 then
		nMax = maxResultInfo.ResultVal2;
	elseif nResultValueIndex == 3 then
		nMax = maxResultInfo.ResultVal3;
	elseif nResultValueIndex == 4 then
		nMax = maxResultInfo.ResultVal4;
	end

	return nResultID, nMin, nMax;
	--]]
end

--名称后边附加信息
function AddSpecialInfoBehindName( item,itemDef )
	if CheckItemResult( itemDef, RESULT_LOST_TOTEM ) and item ~= nil then
		if item:getItemInstVal2() == item:getItemInstVal1() then
			return "（满）"
		end
	end
	return ""
end

--获取物品消息  
function GetToolsInfo( item, itemDef,nTipsType, isShopItem, isStallItem, HoleGemData, storeGridIdx )
	local szItemName = itemDef.Name;
	local szItemSurplus = "";
	local szSurplusColor = COLOR_WHITE;
	if item ~= nil then
		szItemName =  item:getDisplayName();
		if item:getItemSurplus() ~= -1 then 
			szItemSurplus = item:getItemSurplus() .. "次";
			if item:getItemSurplus() == 0 then
				szSurplusColor = COLOR_RED;
			end
		end
	elseif itemDef.UseMaxDur > 0 then
		szItemSurplus = itemDef.UseMaxDur .. "次";
	end
	
	local itemUseLevel ;
	if itemDef.UseTarget ~= USE_TARGET_PET or itemDef.WearPre.RoleLevelMin == 0 then
		itemUseLevel = itemDef.WearPre.RoleLevelMin;
	else
		itemUseLevel = "宠物" .. itemDef.WearPre.RoleLevelMin .. "级"
	end
	local t_oneItemAttrInfo;
	local szItemTime = SetItemEndTime(item )
	local t_ToolsInfo = {
		{ BASE_AREA1, "",				szItemName..AddSpecialInfoBehindName( item, itemDef ),	GetItemNameColor(itemDef),		LINE_TYPE_HEADTEXT, true },	--物品名字
		{ BASE_AREA1, "",				GetArmBind( item,itemDef ),					GetColor()[COLOR_ITEMSTART],	LINE_TYPE_BINDTEXT, false },--物品当前绑定状况
		{ BASE_AREA1, "物品类型\b",		GetItemTypeById(itemDef.ItemID),			GetColor()[COLOR_GOLD],			LINE_TYPE_TEXT, true },		--物品类型
		{ BASE_AREA1, "剩余生命\b",		GetMachineHpProp(item, itemDef),			GetColor()[COLOR_WHITE],		LINE_TYPE_TEXT, false },
		{ BASE_AREA1, "机械能量\b",		GetMachineEnergy(item, itemDef),			GetColor()[COLOR_WHITE],		LINE_TYPE_TEXT, false },
		{ BASE_AREA1, "技能类型\b",		GetPetSkillType(itemDef),					GetColor()[COLOR_GOLD],			LINE_TYPE_TEXT, false },
		{ BASE_AREA1, "冷却时间\b",		SetItemCoolTime(itemDef.SelfCool ),			GetColor()[COLOR_WHITE],		LINE_TYPE_TEXT, false },	--有效时间
		{ BASE_AREA1, "使用等级\b",		itemUseLevel,								GetUseLvColr(itemDef),			LINE_TYPE_TEXT, false },	--使用等级
		{ BASE_AREA1, "",			getSecondarySkillNeed( itemDef ),			getSecondarySkillColor(itemDef),LINE_TYPE_TEXT, false },	--生活技能
		{ BASE_AREA1, "制作物品\b",		getArmScrollItem( itemDef ),				GetColor()[COLOR_GOLD],		LINE_TYPE_TEXT, false },	--制作卷轴
		{ BASE_AREA1, "职业需求\b",		GetCareer(itemDef.WearPre.Career),			GetCareerColor(itemDef),		LINE_TYPE_TEXT, false },	--职业 
		{ BASE_AREA1, "剩余使用次数\b",	        szItemSurplus,								GetColor()[szSurplusColor],		LINE_TYPE_TEXT, false },	--职业
		( not isShopItem and {} or 
		{ BASE_AREA1, "",				GetItemRespact(itemDef),					GetRespacColor(itemDef),		LINE_TYPE_TEXT, false } ),
		( not isShopItem and {} or 
		{ BASE_AREA1, "",				GetItemTech(itemDef.ItemID),				GetItemTechColor(itemDef.ItemID),	LINE_TYPE_TEXT, false } ),
		{ BASE_AREA1, "数量：\b\b",		GetItemGiftNum(itemDef,item),				GetColor()[COLOR_WHITE],		LINE_TYPE_TEXT, false },
		{BASE_AREA1, "储存经验\b",		GetSavedExp(itemDef,item),				GetColor()[COLOR_WEAK_BLUE],		LINE_TYPE_TEXT, false },

		{},{},{},{},{},	{},{},
		{ BASE_AREA2, "有效期至",		szItemTime ~= "" and szItemTime or nil,		GetColor()[COLOR_BATTLE],		LINE_TYPE_TEXT, false },	--有效时间
		{ BASE_AREA2, "",				GetItemDesc( item, itemDef ),    			GetColor()[COLOR_WEAK_BLUE],	LINE_TYPE_RICH, false },	--物品描述
		{ BASE_AREA2, "",				GetCanDealItem( itemDef.NoDeal ),			GetColor()[COLOR_WHITE],		LINE_TYPE_TEXT, false },	--是否可交易
		{ BASE_AREA2, "",				CanDropItem( itemDef.NoDrop ),				GetColor()[COLOR_WHITE],		LINE_TYPE_TEXT, false },	--是否可丢弃
		{ BASE_AREA2, "",				CanSaleItem( itemDef.NoNpcSell ),			GetColor()[COLOR_WHITE],		LINE_TYPE_TEXT, false },	--是否可出售
		{ OTHER_AREA, "商店收购价\b",	itemDef.SellPrice,							GetColor()[COLOR_WHITE],		LINE_TYPE_MONEY, false },	--商店收购价
		isStallItem and { OTHER_AREA, "摊位售单价\b",	item:getStallSellPrice(), GetColor()[COLOR_WHITE],	LINE_TYPE_MONEY, false } or {},
		storeGridIdx and { OTHER_AREA, "店铺售单价\b",	StoreMgr:getStoreItemPrice( storeGridIdx ), GetColor()[COLOR_WHITE],	LINE_TYPE_MONEY, false } or {},
	}
	--处理学习生活技能道具
	local recipeType,recipeId = getSecondarySkillItemData( itemDef );
	if recipeType > 0 then		
		local RecipeDef = SecondarySkillMgr:getRecipeDef( recipeType,recipeId );
		if RecipeDef ~= nil then
			local itemId = RecipeDef.ProduceList[0].GetItem[0].ItemID;
			if itemId > 0 then
				local getItemDef = getItemDef( itemId );
				local productInfo;
				local nItemType = getItemType(getItemDef.ItemID); --获取物品类型
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
					if productInfo[i][5] ~= LINE_TYPE_MONEY and productInfo[i][3] ~= "可以分解" then
						table.insert( t_ToolsInfo,productInfo[i] );
					end
				end
			end
			--添加需要的物品
			table.insert( t_ToolsInfo,{OTHER_AREA,"需要材料：",getSecondarySkillRequireItemDesc(RecipeDef),GetColor()[COLOR_ITEMKEYWORLD],LINE_TYPE_RICH,false} );
		end
		return t_ToolsInfo;
	end
	-- 处理 橙装制作卷轴 
	if CheckItemResult( itemDef, RESULT_LEGEND_SCROLL ) then
		HandleLegendScrollItemTips( t_ToolsInfo,itemDef,nTipsType);
		return t_ToolsInfo;
	end
	local subType = math.floor( itemDef.ItemID/1000 );
	local nShowAttrItem = {5200,5210,5211,5212,5213,5251,5041,5043};
	local bFound = false;
	for i = 1, table.getn( nShowAttrItem ) do
		if subType == nShowAttrItem[i] then
			bFound = true;
		end
	end
	if not bFound then
		return t_ToolsInfo;
	end
	if CheckItemResult( itemDef, RESULT_BACK_HOME ) and item ~= nil then
		return t_ToolsInfo;
	elseif CheckItemResult( itemDef, RESULT_GEM ) then
		local nItemStar, nItemPower = 0, 0;
		if HoleGemData ~= nil then
			local nLength = string.len( HoleGemData );
			local nFirstPos = string.find( HoleGemData, ",", 1, nLength );
			nItemStar =  tonumber( string.sub( HoleGemData, 1, nFirstPos-1 ));
			nItemPower = tonumber( string.sub( HoleGemData, nFirstPos+1, nLength ));
		end
		if item ~= nil then
			nItemStar = item:getGemStart();
			nItemPower = item:getGemPower();
		end
		local t_AttrList = GetGemAddAttrRange( nItemStar, itemDef );
		for i=1,table.getn(t_tagRESULT) do
			for  j = 1, table.getn(t_AttrList) do
				local resultID = t_AttrList[j][1];
				local nMin = t_AttrList[j][2];
				local nMax = t_AttrList[j][3];
				if t_tagRESULT[i][1] == resultID then
					if bShowTitle then
						t_oneItemAttrInfo = { FIGHT_AREA, "", "附加属性：",	GetColor()[COLOR_OGRANGE], LINE_TYPE_TEXT, false };
						table.insert( t_ToolsInfo, t_oneItemAttrInfo );
	
						bShowTitle = false;
					end			

					nMin = FormatItemAttrValue( { ["value"] = nMin, ["resultID"] = resultID } );
					nMax = FormatItemAttrValue( { ["value"] = nMax, ["resultID"] = resultID } );
					--这里区分0-20这类的范围
					local szRange = "";
					if nMin <= 0 then
						szRange = nMax;
					else
						szRange = nMin .. "-" .. nMax;
					end
					t_oneItemAttrInfo = {	FIGHT_AREA, t_tagRESULT[i][2].."\b ",szRange,
											GetColor()[COLOR_WHITE], LINE_TYPE_ATTRADD, false };
					table.insert( t_ToolsInfo,  13, t_oneItemAttrInfo );
					break;
				end
			end
		end					
		local gemNewDef = GameArmProce:getGemNewDef( itemDef.ItemID )
		if gemNewDef.Level >= GEM_CHARGE_LEVEL_MIN then	
			local topStar = IsGemTopLevel( gemNewDef.Level, nItemStar );
			nItemStar = ( nItemStar >= topStar  and topStar or nItemStar );	
					
			table.remove( t_ToolsInfo, 1);
			t_oneItemAttrInfo = { BASE_AREA1, "", szItemName.." +"..(nItemStar), GetItemNameColor(itemDef),	LINE_TYPE_HEADTEXT, true };
			table.insert( t_ToolsInfo, 1, t_oneItemAttrInfo );
			table.remove( t_ToolsInfo, 4 );
			t_oneItemAttrInfo = { BASE_AREA1, "", "宝石星级", GetColor()[COLOR_GOLD],	LINE_TYPE_TEXT, false };
			table.insert( t_ToolsInfo, 4, t_oneItemAttrInfo );
			table.remove( t_ToolsInfo, 5 );
			
			local gemLevelStarDef = GameArmProce:getGemLevelStarDef( gemNewDef.Level, nItemStar );
			if nItemStar >= topStar then
				nItemPower = gemLevelStarDef.PowerUp;
			end
			
			t_oneItemAttrInfo = { BASE_AREA1, "", "能  量  值    "..nItemPower.."/"..gemLevelStarDef.PowerUp, GetColor()[COLOR_GOLD],	LINE_TYPE_TEXT, false };
			table.insert( t_ToolsInfo, 5, t_oneItemAttrInfo );
			local t_GemStarTexUV = {
				[0] = { x="915", y="237", w="11", h="12", },
				[1] = { x="927", y="237", w="11", h="12", },
				[2] = { x="939", y="237", w="11", h="12", },
				[3] = { x="915", y="250", w="11", h="12", },
				[4] = { x="927", y="250", w="11", h="12", },
				[5] = { x="939", y="250", w="11", h="12", },
			}
			local Name;
			if nTipsType == LUA_ITEMTIPS_TYPE_LINK then
				Name = "LinkItemTipsFrame";
			elseif  nTipsType == LUA_ITEMTIPS_TYPE_NORMAL then
				Name = "NormalItemTipsFrame";
			end
			local nCanShowStarNum = 0;
			local lowerUV = t_GemStarTexUV[0];
			local starUV = t_GemStarTexUV[0];
			if nItemStar >= MAX_GEM_STAR then
				nCanShowStarNum = (nItemStar % MAX_GEM_STAR==0) and MAX_GEM_STAR or nItemStar % MAX_GEM_STAR
				lowerUV = t_GemStarTexUV[math.floor( (nItemStar-1) /MAX_GEM_STAR)]
				starUV = t_GemStarTexUV[math.floor( (nItemStar-1) /MAX_GEM_STAR) + 1]
			else
				nCanShowStarNum = nItemStar
				lowerUV = t_GemStarTexUV[0]
				starUV = t_GemStarTexUV[1]
			end
			for i = 1, MAX_GEM_STAR do
				local starTex = getglobal( Name.."StarTex"..i );
				starTex:SetTexUV( lowerUV.x, lowerUV.y, lowerUV.w, lowerUV.h  )
				starTex:Show()
			end
			for i = 1, nCanShowStarNum do
				local starTex = getglobal( Name.."StarTex"..i );
				if starTex then
					starTex:SetTexUV( starUV.x, starUV.y, starUV.w, starUV.h  )
				end
			end
		elseif gemNewDef.Level > 0 and gemNewDef.Level <= GEM_RESOLVE_LEVEL_MIN then
			t_oneInfo = { OTHER_AREA, "", "可以分解", GetColor()[COLOR_GREEN], LINE_TYPE_TEXT, false };
			table.insert( t_ToolsInfo, t_oneInfo );
		end	
	elseif isFairyKeyIn( itemDef ) then
		if bShowTitle then
			--附加属性
			t_oneItemAttrInfo = { FIGHT_AREA,		"",	"附加属性：",	GetColor()[COLOR_OGRANGE], LINE_TYPE_TEXT, false };		
			table.insert( t_ToolsInfo, t_oneItemAttrInfo );
			bShowTitle = false;
		end
		
		t_oneItemAttrInfo = {	FIGHT_AREA, getKeyInAttrDesc( itemDef ).."\b ",getKeyInAttrValue( itemDef ),
								GetColor()[COLOR_WHITE], LINE_TYPE_ATTRADD, false } ;
		table.insert( t_ToolsInfo, t_oneItemAttrInfo );
	elseif ( CheckItemResult( itemDef, RESULT_SPAR ) or CheckItemResult( itemDef, RESULT_SPAR_SHOP ) ) and item ~= nil then
		-- 晶石的附加属性
		local sparLv = item:getItemInstVal1()
		local sparResultId = item:getItemFlag()
		local sparBaseVal = item:getItemInstVal2()
		local sparexp = item:getItemInstVal3()
		local sparDef = GameArmProce:getSparDef( item:getItemId() )
		if sparDef ~= nil and sparLv > 0 then
			local szLvText = "等级：" .. sparLv .. "级（最高" .. sparDef.MaxLvl .. "级）";
			table.remove( t_ToolsInfo, 4 );
			table.insert( t_ToolsInfo, 4, { BASE_AREA1, "", szLvText, GetColor()[COLOR_GOLD],	LINE_TYPE_TEXT, false } );
			local szExpText
			if sparLv == sparDef.MaxLvl and sparLv > 2 then
				szExpText = "经验：" .. sparDef.UpExp[sparLv-2] .. "/" .. sparDef.UpExp[sparLv-2];
			else
				szExpText = "经验：" .. sparexp .. "/" .. sparDef.UpExp[sparLv-1];
			end
			
			table.remove( t_ToolsInfo, 5 );
			table.insert( t_ToolsInfo, 5, { BASE_AREA1, "", szExpText, GetColor()[COLOR_GOLD],	LINE_TYPE_TEXT, false } );
			if bShowTitle then
				--附加属性	
				table.insert( t_ToolsInfo, { FIGHT_AREA, "", "附加属性：", GetColor()[COLOR_OGRANGE], LINE_TYPE_TEXT, false } );
				bShowTitle = false;
			end
			local value =  sparBaseVal * ( 2 + sparLv ) / 3 
			local tAddattrVal = { value }
			table.insert( t_ToolsInfo, 13, { FIGHT_AREA, "\b", getNewAttrDesc( sparResultId, tAddattrVal ), GetColor()[COLOR_ITEMKEYWORLD], LINE_TYPE_ATTRADD, false } );
		end
	elseif CheckItemResult( itemDef, RESULT_MAP_RECORDER ) and item ~= nil  then   
		--空间记忆器 地图信息 Elmer
		if item:getItemMapNum() == 0 then
			return t_ToolsInfo ;
		end
		local	MaxMapControl = MapShower:GetMaxMapContal();
		for i = 1, item:getItemMapNum() do
			local szMapName	= MaxMapControl:getMapName( item:getItemMapInfo(i-1).MapID );
			if string.find( szMapName , "天空岛") ~= nil then
				szMapName = "天空岛";
			elseif string.find( szMapName , "细语丘陵") ~= nil then
				szMapName = "细语丘陵";
			end
			local mapX	= math.floor(item:getItemMapInfo(i-1).X/100 + 1);
			local mapY	= math.floor(item:getItemMapInfo(i-1).Y/100 + 1);
			if i == 1 then
				table.insert( t_ToolsInfo,25,{ BASE_AREA2, "", "已定位："..szMapName.." ( "..mapX..","..mapY.." )", GetColor()[COLOR_OGRANGE], LINE_TYPE_TEXT, false } );
			else
				table.insert( t_ToolsInfo,24 + i,{ BASE_AREA2, "       ","         "..szMapName.." ( "..mapX..","..mapY.." )" , GetColor()[COLOR_OGRANGE], LINE_TYPE_TEXT, false } );
			end
		end
	elseif CheckItemResult( itemDef, RESULT_RUNE ) and item ~= nil  then
		for i = 1,MAX_ITEM_RESULT do
			if	itemDef.Result[i-1].ResultID ~=  RESULT_NONE then
				for j = 1, table.getn(t_tagRESULT) do
					local t_ResultValue = { itemDef.Result[i-1].ResultVal1,itemDef.Result[i-1].ResultVal2,itemDef.Result[i-1].ResultVal3,itemDef.Result[i-1].ResultVal4};
					local nValue = 0;
					--local szDesc = t_tagRESULT[j][2];
					for k = 1, table.getn(t_ResultValue) do
						if t_ResultValue[k] ~= 0 then
							nValue = t_ResultValue[k];
							break;
						end
					end

					if t_tagRESULT[j][1] == itemDef.Result[i-1].ResultID then
						nValue = FormatItemAttrValue( { ["value"] = nValue, ["resultID"] = itemDef.Result[i-1].ResultID } );
						t_oneItemAttrInfo = {	FIGHT_AREA, t_tagRESULT[j][2].."\b+",	
												nValue,
												GetColor()[COLOR_GREEN], LINE_TYPE_ATTRADD, false };
						table.insert( t_ToolsInfo, t_oneItemAttrInfo );
					end
				end
			end
		end
	elseif CheckItemResult( itemDef, RESULT_ADD_DESIGNATION ) then
		local id = itemDef.Result[0].ResultVal2;
		local DesignationDef = GameDesignation:getDesignationById( id );
		if DesignationDef ~= nil then
			table.insert( t_ToolsInfo,4,{ BASE_AREA1, "称号品质\b", GetLevelStar( DesignationDef.Level ), GetColor()[COLOR_GOLD], LINE_TYPE_TEXT, true } );
		end
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
							t_oneItemAttrInfo = { FIGHT_AREA,		"",	"附加属性：",	GetColor()[COLOR_OGRANGE], LINE_TYPE_TEXT, false };
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

--设置物品
function SetItemTips( nTipsType, item, itemDef, szSelfName, isShopItem, isStallItem, HoleGemData, storeGridIdx )
	ClearItemTips(szSelfName);
	--local itemDef = item:getItemDef();
	--物品帖图
	local itemTex = getglobal(szSelfName.."ITItemTex");

	local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
	-- 如果没找到物品的图标则使用默认图标显示
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
	local t_ToolInfo = GetToolsInfo(item,itemDef,nTipsType, isShopItem, isStallItem, HoleGemData, storeGridIdx );
	for i = 1, table.getn(t_ToolInfo) do
		AddTipsLine( t_ToolInfo[i][1], t_ToolInfo[i][2], t_ToolInfo[i][3], t_ToolInfo[i][4], t_ToolInfo[i][5], t_ToolInfo[i][6] );
	end
end

--设置物品结束时间
function SetItemEndTime( item )
	if item ~= nil then
		local nValidEndTime = item:getItemValidEndTime();		
		if nValidEndTime ~= 0 then	
			-- os.date("%c", nValidEndTime)
			local szEndTimeDesc = os.date("%Y", nValidEndTime).."-"..os.date("%m", nValidEndTime).."-"..os.date("%d", nValidEndTime).."  "
					..os.date("%H", nValidEndTime)..":"..os.date("%M", nValidEndTime);
			if item:isValidEnd() then				
				szEndTimeDesc = szEndTimeDesc .. " 已过期"
			end
			return szEndTimeDesc;
		end
	end
	return "";
end

--设置物品结束时间
function SetItemCoolTime( nCoolTime )
	if nCoolTime ~= 0 then
		return math.ceil(nCoolTime/10).."秒";
	end

	return "";
end

function getItemResultValues(def, result)
	for i = 1, MAX_ITEM_RESULT do
		local data = def.Result[i - 1]
		if data.ResultID == result then
			return data.ResultVal1, data.ResultVal2, data.ResultVal3, data.ResultVal4
		end
	end
end

function getItemMonMachine(def)
	local _, id = getItemResultValues(def, RESULT_MACHINE_ITEM)
	return id
end

function GetMachineHpProp(item, def)
	if not item then
		return ""
	end
	if not def then
		def = item:getItemDef()
	end
	local id = getItemMonMachine(def)
	if not id then
		return ""
	end
	local maxhp = getMonsterMaxHP(id)
	if maxhp == 0 then
		return ""
	end
	return string.format("%d%%", math.floor(item:getItemInstVal1() / maxhp * 100 + 0.5))
end

function GetMachineEnergy(item, def)
	if not item then
		return ""
	end
	if not def then
		def = item:getItemDef()
	end
	local id = getItemMonMachine(def)
	if not id then
		return ""
	end
	local maxenergy = getMonMachineMaxEnergy(id)
	if maxenergy == 0 then
		return ""
	end
	return string.format("%d/%d", item:getItemInstVal2(), maxenergy)
end

--设置装备
function SetAmmTips( nTipsType, item, itemDef, szSelfName, isShopItem, isStallItem, storeGridIdx )
	ClearItemTips(szSelfName);
	--物品帖图
	local itemTex = getglobal(szSelfName.."ITItemTex");

	local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
	-- 如果没找到物品的图标则使用默认图标显示
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

	local t_EquipInfo = GetEquipInfo( item, itemDef, nTipsType, isShopItem, isStallItem, false, storeGridIdx );
	for i = 1, table.getn(t_EquipInfo) do
		if t_EquipInfo[i] ~= nil then
			AddTipsLine( t_EquipInfo[i][1], t_EquipInfo[i][2], t_EquipInfo[i][3], t_EquipInfo[i][4], t_EquipInfo[i][5], 
							t_EquipInfo[i][6],t_EquipInfo[i][7] );
		end
	end
end

--设置当前装备
function SetSefAmmTips( nTipsType, item, itemDef, szSelfName )
	ClearItemTips(szSelfName);
	--物品帖图
	local itemTex = getglobal(szSelfName.."ITItemTex");

	local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
	-- 如果没找到物品的图标则使用默认图标显示
	if not IsInExistence( IconPath ) then
		IconPath = DEFAULT_ITEM_PATH;
	end
	itemTex:SetTexture( IconPath );

	itemTex:SetPoint( "topleft", szSelfName, "topleft", 15, 35);

	local bindTexture = getglobal( szSelfName .."BindTexture" );
	if item ~= nil and item:isItemBind() then
		bindTexture:Show();
	else
		bindTexture:Hide();
	end

	local t_EquipInfo = GetEquipInfo( item, itemDef, nTipsType, nil, nil, true );
	for i = 1, table.getn(t_EquipInfo) do
		if t_EquipInfo[i] ~= nil then
			AddSelfTipsLine( t_EquipInfo[i][1], t_EquipInfo[i][2], t_EquipInfo[i][3], t_EquipInfo[i][4], t_EquipInfo[i][5], t_EquipInfo[i][6],
							t_EquipInfo[i][7] );
		end
	end
end

--增加显示的内容
function AddSelfTipsLine( nAreaType, szText, content, t_Color, nType, bAlwayDis, nResultIndex ) --区域，显示内容，颜色值1,颜色值2,颜色值3,类型,是否总是显示
	if bAlwayDis == false and ( tonumber(content) == 0 or content == nil or content == "" ) then return end  --当是0的时候不显示
	if szText == nil then
		return;
	end
	local t_TipsLineInfo = { nAreaType,szText,content, t_Color, nType, nResultIndex };
	table.insert(t_SelfTipsLineList, t_TipsLineInfo);
end

local nFontHeight	= 15; --字体的默认高度
local nFontDistance = 10; --两不同区域间的间距

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
		local nAreaType			= t_SelfTipsLineList[i][1]; --所属区域
		local szText			= t_SelfTipsLineList[i][2]; --显示的前缀内容
		local szContent			= t_SelfTipsLineList[i][3]; --显示的内容
		local nContentType		= t_SelfTipsLineList[i][5]; --类型
		local szTextContent = szText;
		if szContent ~= nil then
			szTextContent = szText..szContent;--显示的完整内容
		end
		local t_Color = t_SelfTipsLineList[i][4];  --颜色
		local AmmLineTex = getglobal(szName.."ITItemLineTex"..nLineIndex);
		local AmmGemTex = getglobal(szName.."InLayGemItemTex"..nGemIndex); -- 镶嵌宝石图
		
		if BASE_AREA1 == nAreaType then
			nBaseAreaNum = nBaseAreaNum + 1;
		end

		if i == 1 then
			nTop = 30;
		elseif nAreaType ~= nLastType then	
			if nLastType == BASE_AREA1 and nBaseAreaNum < 5 then
				nTop = nTop + ( 5 - nBaseAreaNum )*15 - 10;
			end
			if nLastType == BASE_AREA1 then
				nTop = nTop - 15;
			end
			
			nTop = nTop + nFontHeight + nFontDistance + 5;
			AmmLineTex:SetPoint( "left", szName.."ITItemTex", "left", -12, nTop-65);
			AmmLineTex:Show();
			nLineIndex = nLineIndex + 1;

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
			if SetTexturePoint(szContent,szName,nTop-1) then
				nTop = nTop > 105 and 123 or 120;
			else
				nTop = 105;
			end
		elseif nContentType == LINE_TYPE_HEADTEXT then
			--[[
			local curequipfont = getglobal(szName.."ITItemFont"..(nMaxFontNumber-1));
			curequipfont:SetText("【当前装备】");
			curequipfont:SetPoint("topleft", szName, "topleft", 6, 10);
			curequipfont:Show();
			--]]
			local IsEquiped = getglobal(szName.."ITItemIsEquiped");
			IsEquiped:SetText( "已装备" );
			IsEquiped:Show();
						

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
			local nOffset = -2;
			if itemDef.MoneyType == ATTR_ID_MONEY then
				moneytex:SetTexUV(117,494,17,18);
				moneytex:SetSize(17,18);
			elseif itemDef.MoneyType == ATTR_ID_BULL then
				moneytex:SetTexUV(78,494,20,18);
				moneytex:SetSize(20,18);
				nOffset = nOffset;
			end
			local nLeftPoint = 105 - 17;
			moneytex:SetPoint("topleft", szName, "topleft", nLeftPoint+nWidth, nTop+nOffset);
			moneytex:Show();
		elseif nContentType == LINE_TYPE_ATTRADD then
			local rich = getglobal(szName.."ITItemRich"..nRichLineIndex);			
			rich:resizeRect( 240 * GetScreenScaleY(),20 );
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
				local MainPlayer	= ActorMgr:getMainPlayer();
				local Eqiup			= MainPlayer:getEquip();
				local nSuitNum		= Eqiup:getPlayerSuitEquipNum( itemDef.SuitSetID );
				szTextContent		= string.gsub( szTextContent,"#WPSUITNUM#", nSuitNum );
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
				if string.find( szTextContent, "装备基础评分", 1 ) then
					font:SetPoint("topleft", szName, "topleft", 15, nTop-10);
				else
					font:SetPoint("topleft", szName, "topleft", 75, nTop-10);
				end
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
				if szContent ~= "未镶嵌" then
					local nLength		= string.len( szContent );
					local nFirstPos		= string.find( szContent, ",", 1, nLength );
					local nGemId		= string.sub( szContent, 1, nFirstPos-1 );
					local nNextPos		= string.find( szContent, ",", nFirstPos+1, nLength )
					local nStar			= tonumber( string.sub( szContent, nFirstPos+1, nNextPos - 1  ) );
					nFirstPos = nNextPos;
					nNextPos			= string.find(  szContent, ",", nNextPos+1, nLength  );
					local nAttrResultId	= string.sub( szContent, nFirstPos+1, nNextPos - 1 );
					nFirstPos = nNextPos;
					nNextPos			= string.find(  szContent, ",", nNextPos+1, nLength  );
					local nGemIconID	= string.sub( szContent, nFirstPos+1, nNextPos - 1 );
					nFirstPos = nNextPos;
					local nGemMul		= string.sub( szContent, nFirstPos+1, nLength );
					local gemitemdef	= getItemDef( nGemId );
					local nResultIndex	= t_SelfTipsLineList[i][6]; 
					local szAttr		= "";
					if nResultIndex ~= nil then
						szAttr	= GetItemResultByIndex( nGemId, nStar, nAttrResultId, nResultIndex,nGemMul );
					else
						szAttr	= GetItemResult( nGemId );
					end
					--print( "nGemId = "..nGemId.."， szAttr = "..szAttr.."--ShowSelTips--1520" );
					nGemName			= gemitemdef.Name .. "  "..szAttr;

					local IconPath = GetItemEquipIconPath()..nGemIconID..".tga";
					-- 如果没找到物品的图标则使用默认图标显示
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

--设置当前装备
function SetSecondSefAmmTips( nTipsType, item, itemDef, szSelfName )
	ClearItemTips(szSelfName);
	--物品帖图
	local itemTex = getglobal(szSelfName.."ITItemTex");

	local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
	-- 如果没找到物品的图标则使用默认图标显示
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

	itemTex:SetPoint( "topleft", szSelfName, "topleft", 15, 35);

	local t_EquipInfo = GetEquipInfo( item, itemDef, nTipsType, nil, nil, true );
	for i = 1, table.getn(t_EquipInfo) do
		if t_EquipInfo[i] ~= nil then
			AddSecondSelfTipsLine( t_EquipInfo[i][1], t_EquipInfo[i][2], t_EquipInfo[i][3], t_EquipInfo[i][4], t_EquipInfo[i][5], t_EquipInfo[i][6], 
									t_EquipInfo[i][7] );
		end
	end
end

-- 增加显示的内容
-- 区域，显示内容，颜色值1,颜色值2,颜色值3,类型,是否总是显示
function AddSecondSelfTipsLine( nAreaType, szText, content, t_Color, nType, bAlwayDis, nResultIndex ) 
	if bAlwayDis == false and ( tonumber(content) == 0 or content == nil or content == "" ) then return end  --当是0的时候不显示
	if szText == nil then
		return;
	end
	local t_TipsLineInfo = { nAreaType,szText,content, t_Color, nType, nResultIndex };
	table.insert(t_SelfTipsLineList, t_TipsLineInfo);
end

function ShowSecondSelTips(szName, itemDef)
	local nLastType = BASE_AREA1;
	local nTop = 0;
	local nSizeOffSet		= 0;
	local nLineIndex		= 1;
	local nGemIndex			= 1;
	local nRichLineIndex	= 1;
	local nAttrAddIndex		= 1;
	local nBaseAreaNum		= 0;
	for i = 1, table.getn(t_SelfTipsLineList) do
		local nAreaType			= t_SelfTipsLineList[i][1]; --所属区域
		local szText			= t_SelfTipsLineList[i][2]; --显示的前缀内容
		local szContent			= t_SelfTipsLineList[i][3]; --显示的内容
		local nContentType		= t_SelfTipsLineList[i][5]; --类型
		local szTextContent = szText;
		if szContent ~= nil then
			szTextContent = szText..szContent;--显示的完整内容
		end
		local t_Color = t_SelfTipsLineList[i][4];  --颜色
		local AmmLineTex = getglobal(szName.."ITItemLineTex"..nLineIndex);
		local AmmGemTex = getglobal(szName.."InLayGemItemTex"..nGemIndex); -- 镶嵌宝石图
		
		if BASE_AREA1 == nAreaType then
			nBaseAreaNum = nBaseAreaNum + 1;
		end

		if i == 1 then
			nTop = 30;
		elseif nAreaType ~= nLastType then	
			if nLastType == BASE_AREA1 and nBaseAreaNum < 5 then
				nTop = nTop + ( 5 - nBaseAreaNum )*15 - 10;
			end
			if nLastType == BASE_AREA1 then
				nTop = nTop - 15;
			end
			
			nTop = nTop + nFontHeight + nFontDistance + 5;
			AmmLineTex:SetPoint( "left", szName.."ITItemTex", "left", -12, nTop-65);
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
					rich:resizeRect( 230 * GetScreenScaleY(),240 );
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
			if SetTexturePoint(szContent,szName,nTop-1) then
				nTop = nTop > 105 and 123 or 120;
			else
				nTop = 105;
			end
		elseif nContentType == LINE_TYPE_HEADTEXT then
			--[[
			local curequipfont = getglobal(szName.."ITItemFont"..(nMaxFontNumber-1));
			curequipfont:SetText("【当前装备】");
			curequipfont:SetPoint("topleft", szName, "topleft", 6, 10);
			curequipfont:Show();
			--]]
			local IsEquiped = getglobal(szName.."ITItemIsEquiped");
			IsEquiped:SetText( "已装备" );
			IsEquiped:Show();

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
			nSizeOffSet = 15;
		elseif nContentType == LINE_TYPE_MONEY then
			local font = getglobal(szName.."ITItemFont"..(nMaxFontNumber-4));
			font:SetText(szTextContent);
			font:SetPoint("topleft", szName, "topleft", 15, nTop);
			font:SetTextColor(t_Color[1], t_Color[2], t_Color[3]);
			font:Show();

			local moneytex = getglobal( szName.."ITItemMoneyTex");
			local nWidth = font:GetTextExtentWidth(szContent);
			local nOffset = -2;
			if itemDef.MoneyType == ATTR_ID_MONEY then
				moneytex:SetTexUV(117,494,17,18);
				moneytex:SetSize(17,18);
			elseif itemDef.MoneyType == ATTR_ID_BULL then
				moneytex:SetTexUV(78,494,20,18);
				moneytex:SetSize(20,18);
				nOffset = nOffset;
			end
			local nLeftPoint = 105 - 17;
			moneytex:SetPoint("topleft", szName, "topleft", nLeftPoint+nWidth, nTop+nOffset);
			moneytex:Show();
		elseif nContentType == LINE_TYPE_ATTRADD then
			local rich = getglobal(szName.."ITItemRich"..nRichLineIndex);			
			rich:resizeRect( 240 * GetScreenScaleY(),20 );
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
				local MainPlayer	= ActorMgr:getMainPlayer();
				local Eqiup			= MainPlayer:getEquip();
				local nSuitNum		= Eqiup:getPlayerSuitEquipNum( itemDef.SuitSetID );
				szTextContent		= string.gsub( szTextContent,"#WPSUITNUM#", nSuitNum );
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
				if string.find( szTextContent, "装备基础评分", 1 ) then
					font:SetPoint("topleft", szName, "topleft", 15, nTop-10);
				else
					font:SetPoint("topleft", szName, "topleft", 75, nTop-10);
				end
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
				
				if szContent ~= "未镶嵌" then
					local nLength		= string.len( szContent );
					local nFirstPos		= string.find( szContent, ",", 1, nLength );
					local nGemId		= string.sub( szContent, 1, nFirstPos-1 );
					local nNextPos		= string.find( szContent, ",", nFirstPos+1, nLength )
					local nStar			= tonumber( string.sub( szContent, nFirstPos+1, nNextPos - 1 ) );
					nFirstPos = nNextPos;
					nNextPos			= string.find(  szContent, ",", nNextPos+1, nLength  );
					local nAttrResultId	= string.sub( szContent, nFirstPos+1, nNextPos - 1 );
					nFirstPos = nNextPos;
					nNextPos			= string.find(  szContent, ",", nNextPos+1, nLength  );
					local nGemIconID	= string.sub( szContent, nFirstPos+1, nNextPos - 1 );
					nFirstPos = nNextPos;
					local nGemMul		= string.sub( szContent, nFirstPos+1, nLength );
					--print("nGemIconID"..nGemIconID)
					local gemitemdef	= getItemDef( nGemId );
					local nResultIndex	= t_SelfTipsLineList[i][6]; --类型

					-- TODO: ShowSecondSelTips
					local szAttr		= "";
					if nResultIndex ~= nil then
						szAttr	= GetItemResultByIndex( nGemId, nStar, nAttrResultId, nResultIndex,nGemMul );
					else
						szAttr	= GetItemResult( nGemId );
					end
					--print( "nGemId = "..nGemId.."， szAttr = "..szAttr.."--ShowSecondSelTips--1771" );
					nGemName			= gemitemdef.Name .. "  "..szAttr;
					
					local IconPath = GetItemEquipIconPath()..nGemIconID..".tga";
					-- 如果没找到物品的图标则使用默认图标显示
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
	if nWidth < 90 then
		nWidth = 90;
	end

	SetSize(nWidth, szName);
end

function ShowTips(szName, itemDef, nTipsType, item, isStallItem, storeGridIdx )
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
	local nShowMoneyNum		= 0;
	for i = 1, table.getn(t_TipsLineList) do
		if t_TipsLineList[i][5] == LINE_TYPE_BINDTEXT 
			and (t_TipsLineList[i][3] == "拾取后绑定" or t_TipsLineList[i][3] == "装备后绑定" ) then
			ntextoffset = 5;
		end
	end
	--print( "ShowTips--table.getn(t_TipsLineList) = "..table.getn(t_TipsLineList).."--1905" );
	for i = 1, table.getn(t_TipsLineList) do
		local nAreaType			= t_TipsLineList[i][1]; --所属区域
		local szText			= t_TipsLineList[i][2]; --显示的前缀内容
		local szContent			= t_TipsLineList[i][3]; --显示的内容
		local nContentType		= t_TipsLineList[i][5]; --类型
		local szTextContent		= szText;
		--[[
		if i == g_nDestIndex then
			print( "ShowTips--szTextContent = "..szTextContent.."--i = "..i.."--1910" );
		end
		--]]
		if szContent ~= nil then
			--[[
			if i == g_nDestIndex then
				print( "ShowTips--szContent = "..szContent.."--i = "..i.."--1915" );
			end
			--]]
			szTextContent = szText..szContent; -- 显示的完整内容
			--[[
			if i == g_nDestIndex then
				print( "ShowTips--szTextContent = "..szTextContent.."--i = "..i.."--1920" );
			end
			--]]
		end

		local t_Color		= t_TipsLineList[i][4];  --颜色
		local AmmLineTex	= getglobal(szName.."ITItemLineTex"..nLineIndex);
		local AmmGemTex		= getglobal(szName.."InLayGemItemTex"..nGemIndex); -- 镶嵌宝石图
		
		if BASE_AREA1 == nAreaType then
			nBaseAreaNum = nBaseAreaNum + 1;
		end

		if i == 1 then
			nTop = 30;
		elseif nAreaType ~= nLastType then
			if nLastType == BASE_AREA1 and nBaseAreaNum < 5 then
				nTop = nTop + ( 5 - nBaseAreaNum )*15 - 10;
			end			
			if nLastType == BASE_AREA1 and nTop > 105 then
				nTop = nTop - 15;
			end

			nTop = nTop + nFontHeight + nFontDistance + 5;
			AmmLineTex:SetPoint( "left", szName.."ITItemTex", "left", -12, nTop-65);
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
					rich:resizeRect( 230 * GetScreenScaleY(),240 );
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
			if SetTexturePoint(szContent,szName,nTop-1) then
				nTop = nTop > 105 and 123 or 120;
			else
				nTop = 105;
			end
		elseif nContentType == LINE_TYPE_HEADTEXT then
			local font = getglobal(szName.."ITItemFont"..(nMaxFontNumber-2));
			font:SetText(szTextContent);
			--[[
			if i == g_nDestIndex then
				print( "ShowTips--szTextContent = "..szTextContent.."--1989" );
			end
			--]]
			font:SetPoint("topleft", szName, "topleft", 0, nTop-18);
			font:SetTextColor(t_Color[1], t_Color[2], t_Color[3]);
			font:Show();
		elseif nContentType == LINE_TYPE_BINDTEXT then
			local font = getglobal(szName.."ITItemFont"..(nMaxFontNumber-3));
			--[[
			if i == g_nDestIndex then
				print( "ShowTips--szTextContent = "..szTextContent.."--1998" );
			end
			--]]
			font:SetText(szTextContent);
			font:SetPoint("topleft", szName.."ITItemTex", "topleft", -93, 50);
			font:SetTextColor(t_Color[1], t_Color[2], t_Color[3]);
			font:Show();
			nSizeOffSet = BIND_Y_OFFSET;
			nBaseAreaNum = nBaseAreaNum - 1;
			nTop = nTop - nFontHeight;
		elseif nContentType == LINE_TYPE_MONEY then
			local font = getglobal(szName.."ITItemFont"..(nMaxFontNumber- ( 4 + nShowMoneyNum ) ) );
			
			nShowMoneyNum = nShowMoneyNum + 1;
			font:SetText(szTextContent);
			font:SetPoint("topleft", szName, "topleft", 15, nTop);
			font:SetTextColor(t_Color[1], t_Color[2], t_Color[3]);
			font:Show();

			moneytex	= getglobal( szName.."ITItemMoneyTex");
			local nWidth	= font:GetTextExtentWidth(szContent);
			local nOffset	= -2;			

			if isStallItem or storeGridIdx then
				moneytex:SetTexUV(117,494,17,18);
				moneytex:SetSize(17,18);
				local nLeftPoint = 105 - 17;
				moneytex:SetPoint("topleft", szName, "topleft", nLeftPoint+nWidth, nTop+nOffset);
				moneytex:Show();
				if nShowMoneyNum > 1 then
					moneytex:SetTexUV(78,494,20,18);
					moneytex:SetSize(18,18);
					moneytex:SetPoint("topleft", szName, "topleft", nLeftPoint+nWidth, nTop-17);
					local moneytex	= getglobal( szName.."ITItemMoneyTex1");
					local nWidth	= font:GetTextExtentWidth(szContent);
					local nOffset	= -2;
					moneytex:SetTexUV(117,494,17,18);
					moneytex:SetSize(17,18);
					local nLeftPoint = 105 - 17;
					moneytex:SetPoint("topleft", szName, "topleft", nLeftPoint+nWidth, nTop+nOffset);
					moneytex:Show();
				end
			else
				if itemDef.MoneyType == ATTR_ID_MONEY then
					moneytex:SetTexUV(117,494,17,18);
					moneytex:SetSize(17,18);
				elseif itemDef.MoneyType == ATTR_ID_BULL then
					moneytex:SetTexUV(78,494,20,18);
					moneytex:SetSize(18,18);
					nOffset = nOffset;
				end
				local nLeftPoint = 105 - 17;
				moneytex:SetPoint("topleft", szName, "topleft", nLeftPoint+nWidth, nTop+nOffset);
				moneytex:Show();
			end
		elseif nContentType == LINE_TYPE_ATTRADD then
			local rich = getglobal(szName.."ITItemRich"..nRichLineIndex);			
			rich:resizeRect( 240 * GetScreenScaleY(),20 );
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
					local MainPlayer	= ActorMgr:getMainPlayer();
					local Eqiup			= MainPlayer:getEquip();
					nSuitNum			= Eqiup:getPlayerSuitEquipNum( itemDef.SuitSetID );
				end
				if onEnterBtn ~= nil and  string.find( onEnterBtn,"WatchPlayerFrameEquip", 1, true ) then
					nSuitNum =  GetWatchPlayerSuitArmNum( itemDef.SuitSetID );
				end

				szTextContent	= string.gsub( szTextContent,"#WPSUITNUM#", nSuitNum );
				--[[
				if i == g_nDestIndex then
					print( "ShowTips--szTextContent = "..szTextContent.."--2080" );
				end
				--]]
			end

			if string.find( szTextContent,"#WPSUIT#", 1, true ) then
				local nLength	= string.len(szTextContent);
				local nEndPos	= string.find(szTextContent,"#",9,nLength);
				local REPCOLOR	= tonumber(string.sub( szTextContent, 9,nEndPos-1 ));
				local szReplace = "#WPSUIT#"..REPCOLOR.."#"
				--[[
				if i == g_nDestIndex then
					print( "ShowTips--szReplace = "..szReplace.."--2092" );
				end
				--]]
				szTextContent	= string.gsub( szTextContent,szReplace,"" );
				--[[
				if i == g_nDestIndex then
					print( "ShowTips--szTextContent = "..szTextContent.."--2098" );
				end
				--]]
				if GetColor()[REPCOLOR] ~= nil then
					t_Color = GetColor()[REPCOLOR];
				end
			end
			--[[
			if i == g_nDestIndex then
				print( "ShowTips--szTextContent = "..szTextContent.."--2108" );
			end
			--]]
			font:SetText(szTextContent);
			if nAreaType == BASE_AREA1 then
				if string.find( szTextContent, "装备基础评分", 1 ) then
					font:SetPoint("topleft", szName, "topleft", 15+ntextoffset, nTop-10);
				else
					font:SetPoint("topleft", szName, "topleft", 75+ntextoffset, nTop-10);
				end
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
				if szContent ~= "未镶嵌" then
					local nLength		= string.len( szContent );
					local nFirstPos		= string.find( szContent, ",", 1, nLength );
					local nGemId		= string.sub( szContent, 1, nFirstPos-1 );
					local nNextPos		= string.find( szContent, ",", nFirstPos+1, nLength )
					local nStar			= tonumber( string.sub( szContent, nFirstPos+1, nNextPos - 1 ) );
					nFirstPos = nNextPos;
					nNextPos			= string.find(  szContent, ",", nNextPos+1, nLength  );
					local nAttrResultId	= string.sub( szContent, nFirstPos+1, nNextPos - 1 );
					nFirstPos = nNextPos;
					nNextPos			= string.find(  szContent, ",", nNextPos+1, nLength  );
					local nGemIconID	= string.sub( szContent, nFirstPos+1, nNextPos - 1 );
					nFirstPos = nNextPos;
					local nGemMul		= string.sub( szContent, nFirstPos+1, nLength );
					local gemitemdef	= getItemDef( nGemId );
					local nResultIndex	= t_TipsLineList[i][6]; --类型
					
					local szAttr		= "";
					--[[
					if i == g_nDestIndex then
						print( "szContent = "..szContent.."--2146" );
					end
					--]]
					if nResultIndex ~= nil then
						--[[
						if i == g_nDestIndex then
							print( "nGemId = "..nGemId.."， szAttr = "..szAttr.."--2152" );
						end
						--]]

						szAttr	= GetItemResultByIndex( nGemId, nStar, nAttrResultId, nResultIndex,nGemMul );
					else
						szAttr	= GetItemResult( nGemId );
					end
					--[[
					if i == g_nDestIndex then
						print( "nGemId = "..nGemId.."， szAttr = "..szAttr.."--2161" );
					end
					--]]
					nGemName			= gemitemdef.Name .. "  "..szAttr;

					local IconPath = GetItemEquipIconPath()..nGemIconID..".tga";
					-- 如果没找到物品的图标则使用默认图标显示
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
				--[[
				if i == g_nDestIndex then
					print( "ShowTips--nGemName = "..nGemName.."--2182" );
				end
				--]]

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
	--[[
	if i == g_nDestIndex then
		print( "ShowTips--nSizeOffSet = "..nSizeOffSet.."--2198" );
	end
	--]]

	if nWidth < MIN_HEIGHT then
		--print( "ShowTips--nWidth = "..nWidth.."--1887" );
		nWidth = MIN_HEIGHT;
	end

	SetSize(nWidth, szName);
end

function SetSize(nTop, szName)
	local frame = getglobal(szName);
	--print( "SetSize--szName = "..szName.."nTop = "..nTop.."--1894" );
	frame:SetSize(250,nTop + nFontHeight*2);
	nLineIndex = 1;
end

--设置TIPS显示的位置
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
		if OnEnterBtn ~= nil then
			nX = OnEnterBtn:GetRight() - 10;
			nY = OnEnterBtn:GetRealTop() + 10;
		end
	end

	--util.AdjustTooltipsScreenPos( { tips = tipsFrame:GetName() } )

	local TipsWidth = nWidth;
	if ArmItemTipsFrame:IsShown() then
		OffSetX = nWidth;
		TipsWidth = TipsWidth + nWidth;
		if (ArmItemTipsFrame:GetHeight()) > nHeight then
			OffSetY = ArmItemTipsFrame:GetHeight() - nHeight;
		end
	end
	
	if SecondArmItemTipsFrame:IsShown() then
		OffSetX = nWidth;
		TipsWidth = TipsWidth + nWidth;
		local nTmpY = 0;
		if (SecondArmItemTipsFrame:GetHeight()) > nHeight then
			nTmpY = SecondArmItemTipsFrame:GetHeight() - nHeight;
		end
		if OffSetY < nTmpY then
			OffSetY = nTmpY;
		end
	end
	nOffSetX = GetScreenWidth() - ( TipsWidth+nX );
	local nSubResult = nY - nHeight - OffSetY;
	local nAddResult = nY + nHeight + OffSetY;
	--[[
	print( "szOnEnterBtn = "..szOnEnterBtn.."， nY　＝　"..nY.."，nHeight = "..nHeight.."， OffSetY = "..OffSetY.."， nSubResult = "
			..nSubResult.."， nAddResult = "..nAddResult );
	--]]
	if ( nY - nHeight - OffSetY ) > 0 then
		if nOffSetX > 0 then
			Frame:SetPoint("BottomLeft", "UIClient", "topleft", nX + 10, nY  - 10); --右上
			if ArmItemTipsFrame:IsShown() then
				ArmItemTipsFrame:SetPoint("BottomLeft", "UIClient", "topleft", nX + 10 + 245, nY - 10); --右上
			end
			if SecondArmItemTipsFrame:IsShown() then
				SecondArmItemTipsFrame:SetPoint("BottomLeft", "UIClient", "topleft", nX + 10 + 245*2, nY - 10); --右上
			end
		else
			nOffSetX = nX - TipsWidth - 42;
			if nOffSetX < 0 then
				nX = nX - nOffSetX ;
			end
			Frame:SetPoint("BottomRight", "UIClient", "topleft", nX - 42  + 10, nY - 10); --左上
			if ArmItemTipsFrame:IsShown() then
				ArmItemTipsFrame:SetPoint("BottomRight", "UIClient", "topleft", nX - 42  + 10 - 245 , nY - 10); --左上
			end
			if SecondArmItemTipsFrame:IsShown() then
				SecondArmItemTipsFrame:SetPoint("BottomRight", "UIClient", "topleft", nX - 42  + 10 - 245*2 , nY - 10); --左上
			end
		end
	elseif ( nY - nHeight - OffSetY ) < 0 and ( nY + nHeight + OffSetY ) > GetScreenHeight() then
		if nOffSetX > 0 then
			Frame:SetPoint("BottomLeft", "UIClient", "topleft", nX + 10, (DEFAULT_UI_HEIGHT+Frame:GetHeight())/2); --右上
			if ArmItemTipsFrame:IsShown() then
				ArmItemTipsFrame:SetPoint("BottomLeft", "UIClient", "topleft", nX + 10 + 245, 
											(DEFAULT_UI_HEIGHT+ArmItemTipsFrame:GetHeight())/2); --右上
			end
			if SecondArmItemTipsFrame:IsShown() then
				SecondArmItemTipsFrame:SetPoint("BottomLeft", "UIClient", "topleft", nX + 10 + 245*2, 
													(DEFAULT_UI_HEIGHT+SecondArmItemTipsFrame:GetHeight())/2); --右上
			end
		else
			nOffSetX = nX - TipsWidth - 42;
			if nOffSetX < 0 then
				nX = nX - nOffSetX ;
			end
			Frame:SetPoint("BottomRight", "UIClient", "topleft", nX - 42  + 10, (DEFAULT_UI_HEIGHT+Frame:GetHeight())/2); --左上
			if ArmItemTipsFrame:IsShown() then
				ArmItemTipsFrame:SetPoint( "BottomRight", "UIClient", "topleft", nX - 42  + 10 - 245,
											(DEFAULT_UI_HEIGHT+ArmItemTipsFrame:GetHeight())/2 ); --左上
			end
			if SecondArmItemTipsFrame:IsShown() then
				SecondArmItemTipsFrame:SetPoint( "BottomRight", "UIClient", "topleft", nX - 42  + 10 - 245*2,
												(DEFAULT_UI_HEIGHT+SecondArmItemTipsFrame:GetHeight())/2 ); --左上
			end
		end
	else
		if nOffSetX > 0 then
			Frame:SetPoint("TopLeft", "UIClient", "topleft",nX + 10, nY + 42 - 10); --右下
			if ArmItemTipsFrame:IsShown() then
				ArmItemTipsFrame:SetPoint("TopLeft", "UIClient", "topleft",nX + 10 + 245, nY + 42 - 10); --右下
			end
			if SecondArmItemTipsFrame:IsShown() then
				SecondArmItemTipsFrame:SetPoint("TopLeft", "UIClient", "topleft",nX + 10 + 245*2, nY + 42 - 10); --右下
			end
		else
			nOffSetX = nX - TipsWidth - 42;
			if nOffSetX < 0 then
				nX = nX - nOffSetX;
			end
			Frame:SetPoint("TopRight", "UIClient", "topleft", nX - 42  +10, nY + 42 - 10); --左下
			if ArmItemTipsFrame:IsShown() then
				ArmItemTipsFrame:SetPoint("TopRight", "UIClient", "topleft", nX - 42 + 10 - 245, nY + 42 - 10); --左下
			end
			if SecondArmItemTipsFrame:IsShown() then
				SecondArmItemTipsFrame:SetPoint("TopRight", "UIClient", "topleft", nX - 42 + 10 - 245*2, nY + 42 - 10); --左下
			end
		end
	end
end

function LinkItemTipsCloseBtn_OnClick()
	local szName = this:GetParent();
	HideUIPanel(szName);
	ArmItemTipsFrame:Hide();
	SecondArmItemTipsFrame:Hide();
end

function SetTexturePoint(szContent,szName,nTop)
	local isSetStar = false;
	--local nStar = tonumber(szContent);
	local nFirstPos = string.find( szContent, ",", 1 );
	local nStar = tonumber(string.sub( szContent, 1, nFirstPos - 1 ));
	nFirstPos = nFirstPos + 1;
	local font	= getglobal( szName.."ITItemFont"..nMaxFontNumber );
	local itemTex = getglobal(szName.."ITItemTex");
	local high = nTop > 110 and 27 or 24;
	if nStar > 0  and nStar <= ARM_STAR_LIMIT then
		for i=1,nStar do
			local tex = getglobal(szName.."ITItemImportantTex"..i);
			tex:SetPoint("TOPLEFT", (szName.."ITItemTex"), "BOTTOMLEFT", 60+(i-1)*9, high+2);
			tex:Show();
			tex:SetTexUV( 516, 33, 13, 12 );
			local isPerFect = string.sub( szContent, nFirstPos + (i-1)*2, nFirstPos + (i-1)*2 );
			if isPerFect ~=nil and isPerFect == "1" then
				tex:SetTexUV( 530, 33, 13, 12 );
			end
		end

		font:SetText("强化星级：");
		font:SetPoint("TOPLEFT", (szName.."ITItemTex"), "BOTTOMLEFT", 0, high);
		font:Show();
		isSetStar = true;
	end
	return isSetStar;
	--[[
	for i=nStar,12 do
		local tex = getglobal(szName.."ITItemImportantTex"..i);
		tex:Hide();
	end
	]]--
end

function BuffTipsFrame_OnUpdate()
	local frame = getglobal( this:GetClientString() );
	if not frame:IsShown() then
		this:Hide();
	end
end

function BuffTipsFrame_OnHide()
	this:SetClientString( "" );
end


function RideTipsFrame_OnShow()
	local font = getglobal( this:GetName().."EndTimeFont" );
	if font:GetText() == "" then
		RideTipsFrameRideTex:SetPoint( "topleft", "RideTipsFrame", "topleft", 15, 9 );
		RideTipsFrameRideBoxTex:SetPoint( "center", "RideTipsFrameRideTex", "center", 0, 0 );
		RideTipsFrameLineTex1:SetPoint( "topleft", "RideTipsFrameRideTex", "topleft", -13, 55 );
		RideTipsFrameRideDescRich:SetPoint( "topleft", "RideTipsFrameRideTex", "bottomleft", 0, 25 );
	else
		RideTipsFrameRideTex:SetPoint( "topleft", "RideTipsFrame", "topleft", 15, 25 );
		RideTipsFrameRideBoxTex:SetPoint( "center", "RideTipsFrameRideTex", "center", 0, 0 );
		RideTipsFrameLineTex1:SetPoint( "topleft", "RideTipsFrameRideTex", "topleft", -13, 75 );
		RideTipsFrameRideDescRich:SetPoint( "topleft", "RideTipsFrameRideTex", "bottomleft", 0, 40 );
	end
	
	local rich = getglobal( this:GetName().."RideDescRich" );
	local nRichHeight = rich:GetTotalHeight();
	--print( "nRichHeight = "..nRichHeight );

	this:SetHeight( 95 + ( nRichHeight + 6 * GetScreenScaleY() ) / GetScreenScaleY() );
end

function RideTipsFrame_OnUpdate()
	local frame = getglobal( this:GetClientString() );
	if not frame:IsShown() then
		this:Hide();
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

function GetItemResultByIndex( nItemId, nStart, nAttrResultId, nResultIndex,nGemMul )
	local itemDef = getItemDef( nItemId );
	local szResult = "";
	if itemDef == nil then
		return szResult;
	end
	local gemNewDef = GameArmProce:getGemNewDef( nItemId );
	if gemNewDef == nil then
		return szResult;
	end
	if gemNewDef.Attr[nAttrResultId].AttrID == 0 then
		return szResult;
	end
	
	--[[
	for i = 1, table.getn(t_tagRESULT) do
		if t_tagRESULT[i][1] == gemNewDef.Attr[nAttrResultId].AttrID then
			szResult = szResult..t_tagRESULT[i][2];
		end
	end
	--]]
	local DesResultId = GetOneItemResultConfig( { ["resultID"] = gemNewDef.Attr[nAttrResultId].AttrID } )
	szResult = szResult..DesResultId[2]
	-- 控制当前宝石等级（对应的充能等级可能不存在）
	local topStar	= IsGemTopLevel( gemNewDef.Level, nStart );
	local nStar		= nStart >= topStar and topStar or nStart;
		
	local gemAttrStarDef = GameArmProce:getGemAttrStarDef( gemNewDef.Attr[nAttrResultId].AttrID, gemNewDef.Level, nStar );
	szResult = szResult.."+".. math.floor( gemAttrStarDef.Attr[nResultIndex].AttrID * nGemMul );
	
	--[[
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
	--]]

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
			return "(剩余 <1小时)"
		else
			return "(剩余" .. math.ceil( dt/3600 ) .."小时)";
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

local t_KeyInttrName = {[1] = "风",[2] = "电",[3] = "光",[4] = "暗",[5] = "土"};

function getKeyInAttrDesc( itemDef )
	for i=1,MAX_ITEM_RESULT do
		if itemDef.Result[i-1].ResultID == RESULT_ELEMENT_ATK then
			return t_KeyInttrName[  itemDef.Result[i-1].ResultVal1 ] .. "元素伤害";
		elseif itemDef.Result[i-1].ResultID == RESULT_ELEMENT_DEF then
			return t_KeyInttrName[  itemDef.Result[i-1].ResultVal1 ] .. "元素抗性";
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

function GetItemGiftNum( itemDef,item )
	for i=1,MAX_ITEM_RESULT do
		if itemDef.Result[i-1].ResultID == RESULT_ITEM_SWITCH then
			if item ~= nil then
				return item:getItemInstVal1();
			elseif string.find( onEnterBtn,"GuideGiftFrame",1,true ) then
				-- 取礼包的数量
				return GuideFrameGetAccGiftNum( itemDef.ItemID );
			end
		end
	end
	return "";
end
function GetSavedExp( itemDef,item )
	--活动道具，失落图腾	
	if CheckItemResult( itemDef, RESULT_LOST_TOTEM ) and item ~= nil then
		--失落图腾道具
		local maxPower = item:getItemInstVal1()
		local curPower = item:getItemInstVal2()
		return curPower.."/"..maxPower;
	end
	return ""
end

function getSkillPosMul( armSkillAttrDef, wearpos )
	if wearpos == EQUIP_POS_RIGHTHAND then
		return armSkillAttrDef.WposMdf.RightHand
	elseif wearpos == EQUIP_POS_HEAD then
		return armSkillAttrDef.WposMdf.Head
	elseif wearpos == EQUIP_POS_BODY then
		return armSkillAttrDef.WposMdf.Body
	elseif wearpos == EQUIP_POS_LEFT then
		return armSkillAttrDef.WposMdf.LeftHand
	elseif wearpos == EQUIP_POS_FACE then
		return armSkillAttrDef.WposMdf.Face
	elseif wearpos == EQUIP_POS_SHOULDER then
		return armSkillAttrDef.WposMdf.Shoulder
	elseif wearpos == EQUIP_POS_SHOE then
		return armSkillAttrDef.WposMdf.Shoe
	elseif wearpos == EQUIP_POS_AMULET then
		return armSkillAttrDef.WposMdf.Amulet
	elseif wearpos == EQUIP_POS_RING or wearpos == EQUIP_POS_RING1 or wearpos == EQUIP_POS_RING2 or wearpos == EQUIP_POS_RING3 then
		return armSkillAttrDef.WposMdf.Ring
	elseif wearpos == EQUIP_POS_NECKLACE then
		return armSkillAttrDef.WposMdf.NeckLace
	elseif wearpos == EQUIP_POS_FASHION_BACK then 
		return armSkillAttrDef.WposMdf.Back;
	elseif wearpos == EQUIP_POS_BADGE then
		return armSkillAttrDef.WposMdf.Badge;
	end
	return 0;
end

function getAttackType( attr )
	if t_AttackType[attr] ~= nil then
		return t_AttackType[attr];
	else
		return "";
	end
end

function getArmSkillDesc( itemDef, data )
	local attrDef = GameArmProce:getArmSkillAttrDef( data.ID );
	local szText = "技能：";
	if attrDef ~= nil then
		local skillDef = SkillMgr:getSpellStaticAtt( attrDef.SkillID, 1 );
		if skillDef.SkillID > 0 then
			szText = szText .. skillDef.Name;
			local lvMul = attrDef.LvlMdf[math.floor( itemDef.WearPre.RoleLevelMin / 10 )] / 10000;
			local posMul =  getSkillPosMul( attrDef, itemDef.WearPos1 ) / 10000;
			local impMul = attrDef.ImpMdf[itemDef.Important] / 10000;
			local rndMul = attrDef.RandMdf[data.Idx] / 10000;
			local val = math.floor( attrDef.BaseVal * lvMul * posMul * impMul * rndMul );
			--治疗技能特殊处理下
			if attrDef.Type == 1 then
				szText = szText .. "治疗效果 +" .. val;
			elseif attrDef.EffectID > 0 then
				szText = szText .. "最终伤害 +" .. math.floor(val/100)/100 .. "%";
			else
				szText = szText .. "附加" .. getAttackType( skillDef.MainAttr ) .. "攻击 +" .. val;
			end
		end
	end
	return szText;
end
-- 处理 橙装制作卷轴tips
function HandleLegendScrollItemTips( t_ToolsInfo,itemDef,nTipsType)
	local srcArmID = GetItemResultValue1( itemDef, RESULT_LEGEND_SCROLL );
	if srcArmID > 0 then
		local levelUpDef 	= getArmLevelUPDef( srcArmID );	
		local desArmDef 	= getItemDef( levelUpDef.NextItemID );
		if desArmDef ~= nil then	
			local productInfo;
			local nItemType = getItemType(desArmDef.ItemID); --获取物品类型
			if nItemType == ITEM_SLOTTYPE_AMM then
				productInfo = GetEquipInfo( nil,desArmDef,nTipsType );
			elseif nItemType == ITEM_SLOTTYPE_ITEM then
				productInfo = GetToolsInfo( nil,desArmDef,nTipsType );
			end
			
			for i=1,table.getn( productInfo ) do
				if productInfo[i][1] == BASE_AREA1 then
					productInfo[i][1] = BASE_AREA3;
				end
				if productInfo[i][5] == LINE_TYPE_HEADTEXT or productInfo[i][5] == LINE_TYPE_BINDTEXT then
					productInfo[i][5] = LINE_TYPE_TEXT;
				end
				if productInfo[i][5] ~= LINE_TYPE_MONEY and productInfo[i][3] ~= "可以分解" then
					table.insert( t_ToolsInfo,productInfo[i] );
				end
			end
			--添加需要的物品
			table.insert( t_ToolsInfo,{OTHER_AREA,"需要材料：",getLegendScrollRequireItemDesc(levelUpDef),GetColor()[COLOR_ITEMKEYWORLD],LINE_TYPE_RICH,false} );
		end
	end	
end
local MAX_ARM_UP_ITEM_NEED_NUM = 4;
function getLegendScrollRequireItemDesc(levelUpDef)
	local szText = "";
	for i=1,MAX_ARM_UP_ITEM_NEED_NUM do
		if levelUpDef.Stuff[i-1].ItemID ~= 0 then
			local itemDef = getItemDef( levelUpDef.Stuff[i-1].ItemID );
			if itemDef ~= nil then
				szText = szText..itemDef.Name.."("..levelUpDef.Stuff[i-1].Num..") " ;
			end
		end
	end
	szText = szText.."\n\n#Y右键点击使用#n"
	return szText;
end