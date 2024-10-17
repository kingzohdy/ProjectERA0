--[[
填在g_tNameTable中的
每个数据内容为 {属性id,"默认名字",{
									{minVal = 属性区间最小值, maxVal = 属性区间最大值 ,szName = "名称"},
									{minVal = 属性区间最小值, maxVal = 属性区间最大值 ,szName = "名称2"}, 
									{minVal = 属性区间最小值, maxVal = 属性区间最大值 ,szName = "名称3"}, 
									{minVal = 属性区间最小值, maxVal = 属性区间最大值 ,szName = "名称4"}, 
								  },
				}
例如
	{1,"坚固的 ",{
					{ minVal = 1,maxVal = 5,szName = "坚硬的" },
					{ minVal = 6 , maxVal = 10 , szName = "完美的"},
				 },
	},
	这行表示 在附加属性ArmAddAttrDef表中id 为1 时,默认的前缀名称为"坚固的",如果附加的属性值在1-5之间则是"坚硬的",依次类推.

]]--

local g_tAttrNumNameTable = 
{
	"精良的 ",
	"杰出的 ",
	"卓越的 ",
	"荣耀的 ",
	"史诗的 ",
	"英雄的 ",
	"传奇的 ",
	"上古的 ",
}

local g_tNameTable = 
{
	{1,"双系攻击",{
					{ minVal = 1, maxVal = 14,  szName = "进攻·" },
					{ minVal = 15, maxVal = 33 , szName = "凶狠·"},
					{ minVal = 34, maxVal = 42 , szName = "凶猛·"},
					{ minVal = 43, maxVal = 73 , szName = "强者·"},
					{ minVal = 74, maxVal = 121 , szName = "升华·"},
					{ minVal = 122, maxVal = 199 , szName = "斗争·"},
					{ minVal = 200, maxVal = 500 , szName = "霸王·"},			
				 },
	},

	{1004,"生命值",{
					{ minVal = 1,  maxVal = 2,szName = "愈合·" },
					{ minVal = 3, maxVal = 4 , szName = "血狼·"},
					{ minVal = 5, maxVal = 6 , szName = "生命·"},
					{ minVal = 7, maxVal = 8 , szName = "祝福·"},
					{ minVal = 9, maxVal = 10 , szName = "刚毅·"},
					{ minVal = 11, maxVal = 11 , szName = "血月·"},
					{ minVal = 12, maxVal = 12 , szName = "圣光·"},
				 },
	},

	{1005,"魔法值",{
					{ minVal = 1,  maxVal = 2,  szName = "能量·" },
					{ minVal = 3, maxVal = 4 , szName = "精华·"},
					{ minVal = 5, maxVal = 6 , szName = "精神·"},
					{ minVal = 7, maxVal = 8 , szName = "法力·"},
					{ minVal = 9, maxVal = 10 , szName = "魔力·"},
					{ minVal = 11, maxVal = 11 , szName = "精灵·"},
					{ minVal = 12, maxVal = 12 , szName = "恩赐·"},
				 },
	},

	{1009,"破防",{
					{ minVal = 1,maxVal = 4,szName = "突进·" },
					{ minVal = 5, maxVal = 8 , szName = "风暴·"},
					{ minVal = 9, maxVal = 12 , szName = "闪耀·"},

				 },
	},

	{1002,"物防",{
					{ minVal = 1,maxVal = 2,szName = "坚固·" },
					{ minVal = 3, maxVal = 4 , szName = "岩石·"},
					{ minVal = 5, maxVal = 6 , szName = "铁人·"},
					{ minVal = 7, maxVal = 8 , szName = "铜墙·"},
					{ minVal = 9, maxVal = 10 , szName = "铁壁·"},
					{ minVal = 11, maxVal = 11 , szName = "钢韧·"},
					{ minVal = 12, maxVal = 12 , szName = "永恒·"},
				 },
	},

	{1003,"魔防",{
					{ minVal = 1,maxVal = 2 , szName = "魔脉·" },
					{ minVal = 3, maxVal = 4 , szName = "抗魔·"},
					{ minVal = 5, maxVal = 6 , szName = "迷雾·"},
					{ minVal = 7, maxVal = 8 , szName = "破咒·"},
					{ minVal = 9, maxVal = 10 , szName = "豁免·"},
					{ minVal = 11, maxVal = 11 , szName = "掌控·"},
					{ minVal = 12, maxVal = 12 , szName = "时光·"},
				 },
	},

	{8,"双系防御",{
					{ minVal = 1, maxVal = 7,  szName = "防御·" },
					{ minVal = 8, maxVal = 12 , szName = "坚挺·"},
					{ minVal = 13, maxVal =32 , szName = "抵抗·"},
					{ minVal = 33, maxVal =56 , szName = "抗击·"},
					{ minVal = 57, maxVal =92 , szName = "守卫·"},
					{ minVal = 93, maxVal =151 , szName = "顽强·"},
					{ minVal = 152, maxVal =1000 , szName = "无尽·"},
				 },
	},

	{1029,"物伤减免",{
					{ minVal = 1,maxVal = 3,szName = "守护·" },
					{ minVal = 4, maxVal = 6 , szName = "统御·"},
					{ minVal = 7, maxVal = 9 , szName = "翼龙·"},
					{ minVal = 10, maxVal = 12 , szName = "吞噬·"},

				 },
	},

	{1030,"法伤减免",{
					{ minVal = 1,maxVal = 3,szName = "冷酷·" },
					{ minVal = 4, maxVal = 6 ,szName = "魔能·"},
					{ minVal = 7, maxVal = 9 ,  szName = "远古·"},
					{ minVal = 10, maxVal = 12 ,szName = "圣堂·"},
				 },
	},


	{15,"伤害吸收",{
					{ minVal = 1,maxVal = 3,szName = "坚定·" },
					{ minVal = 4, maxVal = 6 , szName = "抗拒·"},
					{ minVal = 7, maxVal = 9 , szName = "封印·"},
					{ minVal = 10, maxVal = 100 , szName = "引力·"},
					
				 },
	},

	{1007,"闪避等级",{
					{ minVal = 1,maxVal = 2,szName = "轻巧·" },
					{ minVal = 3, maxVal = 4 , szName = "闪现·"},
					{ minVal = 5, maxVal = 6 , szName = "闪电·"},
					{ minVal = 7, maxVal = 8 , szName = "灵巧·"},
					{ minVal = 9, maxVal = 10 , szName = "灵闪·"},
					{ minVal = 11, maxVal = 11 , szName = "光舞·"},
					{ minVal = 12, maxVal = 12 , szName = "万游·"},
				 },
	},


	{30,"暴击率",{
					{ minVal = 1,maxVal = 2,szName = "凶暴·" },
					{ minVal = 3, maxVal = 4 , szName = "暴熊·"},
					{ minVal = 5, maxVal = 100 , szName = "致命·"},
									
				 },
	},

	{1006,"命中等级",{
					{ minVal = 1,maxVal = 2,szName = "精确·" },
					{ minVal = 3, maxVal = 4 , szName = "游侠·"},
					{ minVal = 5, maxVal = 6 , szName = "流光·"},
					{ minVal = 7, maxVal = 8 , szName = "幻影·"},
					{ minVal = 9, maxVal = 10 , szName = "无虚·"},
					{ minVal = 11, maxVal = 11 , szName = "鹰眼·"},
					{ minVal = 12, maxVal = 12 , szName = "灵犀·"},
				 },
	},

	{39,"战斗回血",{
					{ minVal = 1,maxVal = 10,szName = "复苏·" },
					{ minVal = 11, maxVal = 100 , szName = "混沌·"},
					
				 },
	},

	{40,"战斗回蓝",{
					{ minVal = 1,maxVal = 10,szName = "复苏·" },
					{ minVal = 11, maxVal = 100 , szName = "奥法·"},
					
				 },
	},


	{41,"命中率",{
					{ minVal = 1,maxVal = 2,szName = "精准·" },
					{ minVal = 3, maxVal = 4 , szName = "鹰眼·"},
					{ minVal = 5, maxVal = 100 , szName = "天狙·"},
					
				 },
	},



	{1000,"物攻",{
					{ minVal = 1,maxVal = 2,szName = "野蛮·" },
					{ minVal = 3, maxVal = 4 , szName = "强力·"},
					{ minVal = 5, maxVal = 6 , szName = "战力·"},
					{ minVal = 7, maxVal = 8 , szName = "猛将·"},
					{ minVal = 9, maxVal = 10 , szName = "碎甲·"},
					{ minVal = 11, maxVal = 11 , szName = "勇者·"},
					{ minVal = 12, maxVal = 12 , szName = "战神·"},
				 },
	},

	{1001,"魔攻",{
					{ minVal = 1,maxVal = 2,szName = "秘术·" },
					{ minVal = 3, maxVal = 4 , szName = "痛苦·"},
					{ minVal = 5, maxVal = 6 , szName = "魔法·"},
					{ minVal = 7, maxVal = 8 , szName = "奥义·"},
					{ minVal = 9, maxVal = 10 , szName = "幻魔·"},
					{ minVal = 11, maxVal = 11 , szName = "魔神·"},
					{ minVal = 12, maxVal = 12 , szName = "死神·"},
				 },
	},

	{1031,"暴击伤害",{
					{ minVal = 1,maxVal = 3,szName = "凶残·" },
					{ minVal = 4, maxVal = 6 , szName = "毒蛇·"},
					{ minVal = 7, maxVal = 9 , szName = "嗜血·"},
					{ minVal = 10, maxVal = 12 , szName = "恶魔·"},
				
				 },
	},

	{1011,"暴击等级",{
					{ minVal = 1,maxVal = 2,szName = "会心·" },
					{ minVal = 3, maxVal = 4 , szName = "一闪·"},
					{ minVal = 5, maxVal = 6 , szName = "重击·"},
					{ minVal = 7, maxVal = 8 , szName = "狂暴·"},
					{ minVal = 9, maxVal = 10 , szName = "暴走·"},
					{ minVal = 11, maxVal = 11 , szName = "狂热·"},
					{ minVal = 12, maxVal = 12 , szName = "疯狂·"},
				
				 },
	},
	{1012,"暴击抵抗",{
					{ minVal = 1,maxVal = 2,szName = "防爆·" },
					{ minVal = 3, maxVal = 4 , szName = "冷静·"},
					{ minVal = 5, maxVal = 6 , szName = "警戒·"},
					{ minVal = 7, maxVal = 8 , szName = "惊醒·"},
					{ minVal = 9, maxVal = 10 , szName = "摄魂·"},
					{ minVal = 11, maxVal = 11 , szName = "悸动·"},
					{ minVal = 12, maxVal = 12 , szName = "灵觉·"},
				
				 },
	},

	{1013,"力量",{
					{ minVal = 1,maxVal = 2,szName = "蛮力·" },
					{ minVal = 3, maxVal = 4 , szName = "蛮牛·"},
					{ minVal = 5, maxVal = 6 , szName = "巨力·"},
					{ minVal = 7, maxVal = 8 , szName = "扛鼎·"},
					{ minVal = 9, maxVal = 10 , szName = "拔山·"},
					{ minVal = 11, maxVal = 11 , szName = "巨擘·"},
					{ minVal = 12, maxVal = 12 , szName = "举天·"},
				
				 },
	},

	{1014,"智力",{
					{ minVal = 1,maxVal = 2,szName = "智力·" },
					{ minVal = 3, maxVal = 4 , szName = "聪敏·"},
					{ minVal = 5, maxVal = 6 , szName = "聪慧·"},
					{ minVal = 7, maxVal = 8 , szName = "幻魔·"},
					{ minVal = 9, maxVal = 10 , szName = "睿智·"},
					{ minVal = 11, maxVal = 11 , szName = "先知·"},
					{ minVal = 12, maxVal = 12 , szName = "智者·"},
				
				
				 },
	},

	{1015,"体质",{
					{ minVal = 1,maxVal = 2,szName = "健康·" },
					{ minVal = 3, maxVal = 4 , szName = "壮硕·"},
					{ minVal = 5, maxVal = 6 , szName = "坚韧·"},
					{ minVal = 7, maxVal = 8 , szName = "活力·"},
					{ minVal = 9, maxVal = 10 , szName = "生存·"},
					{ minVal = 11, maxVal = 11 , szName = "长寿·"},
					{ minVal = 12, maxVal = 12 , szName = "巨人·"},
				
				
				 },
	},

	{1018,"感知",{
					{ minVal = 1,maxVal = 2,szName = "灵动·" },
					{ minVal = 3, maxVal = 4 , szName = "魔纹·"},
					{ minVal = 5, maxVal = 6 , szName = "灵感·"},
					{ minVal = 7, maxVal = 8 , szName = "视魔·"},
					{ minVal = 9, maxVal = 10 , szName = "洞悉·"},
					{ minVal = 11, maxVal = 11 , szName = "意念·"},
					{ minVal = 12, maxVal = 12 , szName = "神佑·"},
				
				 },
	},

	{1016,"精神",{
					{ minVal = 1,maxVal = 2,szName = "平和·" },
					{ minVal = 3, maxVal = 4 , szName = "平静·"},
					{ minVal = 5, maxVal = 6 , szName = "预言·"},
					{ minVal = 7, maxVal = 8 , szName = "灵光·"},
					{ minVal = 9, maxVal = 10 , szName = "梦魇·"},
					{ minVal = 11, maxVal = 11 , szName = "神秘·"},
					{ minVal = 12, maxVal = 12 , szName = "觉醒·"},
				
				 },
	},


	{1017,"敏捷",{
					{ minVal = 1,maxVal = 2,szName = "灵活·" },
					{ minVal = 3, maxVal = 4 , szName = "敏捷·"},
					{ minVal = 5, maxVal = 6 , szName = "幽灵·"},
					{ minVal = 7, maxVal = 8 , szName = "迅捷·"},
					{ minVal = 9, maxVal = 10 , szName = "刺客·"},
					{ minVal = 11, maxVal = 11 , szName = "盗贼·"},
					{ minVal = 12, maxVal = 12 , szName = "盗神·"},
				
				 },
	},

	
};


-------------------------------------------------------
function GetArmName( nAddAttrNum,Arm )
	local nNum = nAddAttrNum;
	local nMaxAddLevel = 0;
	local nTableIndex = 0;
	local itemDef = getItemDef( Arm.DefID )
	for i = 1,nAddAttrNum do 
		if Arm.AddAttr[i-1].Val == 0 then
			nNum = nNum - 1;
		end
		for j,tAttrName in ipairs ( g_tNameTable ) do
			--比较附加属性的ID
			if tAttrName[1] == Arm.AddAttr[i-1].AddAttrID then
				if nTableIndex == 0 then
					nTableIndex = j
				end
				local addAttrRandDef = getAddAttrRandDef( math.ceil(itemDef.WearPre.RoleLevelMin/10), itemDef.Important )
				if addAttrRandDef == nil then					
					break;
				end
				--根据附加的属性值 查找 他对应的等级
				local tName = tAttrName[3];
				for nLevel,tValName in ipairs ( tName ) do
					if ( addAttrRandDef.ValLevel >= tValName.minVal and addAttrRandDef.ValLevel <= tValName.maxVal ) then
						if nLevel > nMaxAddLevel then
							nTableIndex = j;
							nMaxAddLevel = nLevel;
						end
						break;
					end
				end
			end
		end
	end
	if nNum < 1 then
		return "";
	end
	nNum = itemDef.Important
	if nNum > #g_tAttrNumNameTable then
		nNum = #g_tAttrNumNameTable;
	end
	local szNumName = g_tAttrNumNameTable[nNum];
	if nTableIndex ~= 0 and nMaxAddLevel ~= 0 then
		local szDefaultName = g_tNameTable[nTableIndex][2];
		local szAttrName = g_tNameTable[nTableIndex][3][nMaxAddLevel].szName;		
		if szAttrName ~= "" then
			return szNumName..szAttrName
		else 
			return szNumName..szDefaultName;
		end
	elseif nTableIndex ~= 0 then
		local szDefaultName = g_tNameTable[nTableIndex][2];
		return szNumName..szDefaultName;
	end
	return szNumName;
end

function GetStaticArmName(ItemDef)
	if ItemDef == nil then
		return ""
	end
	if getItemType( ItemDef.ItemID ) ~= ITEM_SLOTTYPE_AMM then
		return ""
	end
	local nAddAttrNum = 0;
	for i = 1,MAX_ITEM_ADD_ATTR_TOLUA do 
		if ItemDef.AddAttr[i-1].ID ~=0 then
			nAddAttrNum = nAddAttrNum + 1;
		end
	end
	if nAddAttrNum < 1 then
		return ""
	end
	local nNum = ItemDef.Important
	if nNum > #g_tAttrNumNameTable then
		nNum = #g_tAttrNumNameTable;
	end
	local szNumName = g_tAttrNumNameTable[nNum];

	local nMaxAddLevel = 0;
	local nTableIndex = 0;
	for i = 1,MAX_ITEM_ADD_ATTR_TOLUA do
		if ItemDef.AddAttr[i-1].ID > 0 then
			for j,tAttrName in ipairs ( g_tNameTable ) do
				--比较附加属性的ID
				if tAttrName[1] == ItemDef.AddAttr[i-1].ID == nAttrId then
					if nTableIndex == 0 then
						nTableIndex = j
					end
					local addAttrRandDef = getAddAttrRandDef( math.ceil(ItemDef.WearPre.RoleLevelMin/10), ItemDef.Important )
					if addAttrRandDef == nil then
						break;
					end
					--根据附加的属性值 查找 他对应的等级
					local nAddAttrVal = getArmAttrValue(ItemDef.AddAttr[i-1].ID,ItemDef.AddAttr[i-1].Idx);
					local tName = tAttrName[3];
					for nLevel,tValName in ipairs ( tName ) do
						if ( addAttrRandDef.ValLevel >= tValName.minVal and addAttrRandDef.ValLevel <= tValName.maxVal ) then
							if nLevel > nMaxAddLevel then
								nTableIndex = j;
								nMaxAddLevel = nLevel;
							end
							break;
						end
					end
				end
			end
		end
	end
	if nTableIndex ~= 0 and nMaxAddLevel ~= 0 then
		local szDefaultName = g_tNameTable[nTableIndex][2];
		local szAttrName = g_tNameTable[nTableIndex][3][nMaxAddLevel].szName;		
		if szAttrName ~= "" then
			return szNumName..szAttrName
		else 
			return szNumName..szDefaultName;
		end
	elseif nTableIndex ~= 0 then
		local szDefaultName = g_tNameTable[nTableIndex][2];
		return szNumName..szDefaultName;
	end
	return szNumName;
end