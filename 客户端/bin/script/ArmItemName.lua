--[[
����g_tNameTable�е�
ÿ����������Ϊ {����id,"Ĭ������",{
									{minVal = ����������Сֵ, maxVal = �����������ֵ ,szName = "����"},
									{minVal = ����������Сֵ, maxVal = �����������ֵ ,szName = "����2"}, 
									{minVal = ����������Сֵ, maxVal = �����������ֵ ,szName = "����3"}, 
									{minVal = ����������Сֵ, maxVal = �����������ֵ ,szName = "����4"}, 
								  },
				}
����
	{1,"��̵� ",{
					{ minVal = 1,maxVal = 5,szName = "��Ӳ��" },
					{ minVal = 6 , maxVal = 10 , szName = "������"},
				 },
	},
	���б�ʾ �ڸ�������ArmAddAttrDef����id Ϊ1 ʱ,Ĭ�ϵ�ǰ׺����Ϊ"��̵�",������ӵ�����ֵ��1-5֮������"��Ӳ��",��������.

]]--

local g_tAttrNumNameTable = 
{
	"������ ",
	"�ܳ��� ",
	"׿Խ�� ",
	"��ҫ�� ",
	"ʷʫ�� ",
	"Ӣ�۵� ",
	"����� ",
	"�Ϲŵ� ",
}

local g_tNameTable = 
{
	{1,"˫ϵ����",{
					{ minVal = 1, maxVal = 14,  szName = "������" },
					{ minVal = 15, maxVal = 33 , szName = "�׺ݡ�"},
					{ minVal = 34, maxVal = 42 , szName = "���͡�"},
					{ minVal = 43, maxVal = 73 , szName = "ǿ�ߡ�"},
					{ minVal = 74, maxVal = 121 , szName = "������"},
					{ minVal = 122, maxVal = 199 , szName = "������"},
					{ minVal = 200, maxVal = 500 , szName = "������"},			
				 },
	},

	{1004,"����ֵ",{
					{ minVal = 1,  maxVal = 2,szName = "���ϡ�" },
					{ minVal = 3, maxVal = 4 , szName = "Ѫ�ǡ�"},
					{ minVal = 5, maxVal = 6 , szName = "������"},
					{ minVal = 7, maxVal = 8 , szName = "ף����"},
					{ minVal = 9, maxVal = 10 , szName = "���㡤"},
					{ minVal = 11, maxVal = 11 , szName = "Ѫ�¡�"},
					{ minVal = 12, maxVal = 12 , szName = "ʥ�⡤"},
				 },
	},

	{1005,"ħ��ֵ",{
					{ minVal = 1,  maxVal = 2,  szName = "������" },
					{ minVal = 3, maxVal = 4 , szName = "������"},
					{ minVal = 5, maxVal = 6 , szName = "����"},
					{ minVal = 7, maxVal = 8 , szName = "������"},
					{ minVal = 9, maxVal = 10 , szName = "ħ����"},
					{ minVal = 11, maxVal = 11 , szName = "���顤"},
					{ minVal = 12, maxVal = 12 , szName = "���͡�"},
				 },
	},

	{1009,"�Ʒ�",{
					{ minVal = 1,maxVal = 4,szName = "ͻ����" },
					{ minVal = 5, maxVal = 8 , szName = "�籩��"},
					{ minVal = 9, maxVal = 12 , szName = "��ҫ��"},

				 },
	},

	{1002,"���",{
					{ minVal = 1,maxVal = 2,szName = "��̡�" },
					{ minVal = 3, maxVal = 4 , szName = "��ʯ��"},
					{ minVal = 5, maxVal = 6 , szName = "���ˡ�"},
					{ minVal = 7, maxVal = 8 , szName = "ͭǽ��"},
					{ minVal = 9, maxVal = 10 , szName = "���ڡ�"},
					{ minVal = 11, maxVal = 11 , szName = "���͡�"},
					{ minVal = 12, maxVal = 12 , szName = "���㡤"},
				 },
	},

	{1003,"ħ��",{
					{ minVal = 1,maxVal = 2 , szName = "ħ����" },
					{ minVal = 3, maxVal = 4 , szName = "��ħ��"},
					{ minVal = 5, maxVal = 6 , szName = "����"},
					{ minVal = 7, maxVal = 8 , szName = "���䡤"},
					{ minVal = 9, maxVal = 10 , szName = "���⡤"},
					{ minVal = 11, maxVal = 11 , szName = "�ƿء�"},
					{ minVal = 12, maxVal = 12 , szName = "ʱ�⡤"},
				 },
	},

	{8,"˫ϵ����",{
					{ minVal = 1, maxVal = 7,  szName = "������" },
					{ minVal = 8, maxVal = 12 , szName = "��ͦ��"},
					{ minVal = 13, maxVal =32 , szName = "�ֿ���"},
					{ minVal = 33, maxVal =56 , szName = "������"},
					{ minVal = 57, maxVal =92 , szName = "������"},
					{ minVal = 93, maxVal =151 , szName = "��ǿ��"},
					{ minVal = 152, maxVal =1000 , szName = "�޾���"},
				 },
	},

	{1029,"���˼���",{
					{ minVal = 1,maxVal = 3,szName = "�ػ���" },
					{ minVal = 4, maxVal = 6 , szName = "ͳ����"},
					{ minVal = 7, maxVal = 9 , szName = "������"},
					{ minVal = 10, maxVal = 12 , szName = "���ɡ�"},

				 },
	},

	{1030,"���˼���",{
					{ minVal = 1,maxVal = 3,szName = "��ᡤ" },
					{ minVal = 4, maxVal = 6 ,szName = "ħ�ܡ�"},
					{ minVal = 7, maxVal = 9 ,  szName = "Զ�š�"},
					{ minVal = 10, maxVal = 12 ,szName = "ʥ�á�"},
				 },
	},


	{15,"�˺�����",{
					{ minVal = 1,maxVal = 3,szName = "�ᶨ��" },
					{ minVal = 4, maxVal = 6 , szName = "���ܡ�"},
					{ minVal = 7, maxVal = 9 , szName = "��ӡ��"},
					{ minVal = 10, maxVal = 100 , szName = "������"},
					
				 },
	},

	{1007,"���ܵȼ�",{
					{ minVal = 1,maxVal = 2,szName = "���ɡ�" },
					{ minVal = 3, maxVal = 4 , szName = "���֡�"},
					{ minVal = 5, maxVal = 6 , szName = "���硤"},
					{ minVal = 7, maxVal = 8 , szName = "���ɡ�"},
					{ minVal = 9, maxVal = 10 , szName = "������"},
					{ minVal = 11, maxVal = 11 , szName = "���衤"},
					{ minVal = 12, maxVal = 12 , szName = "���Ρ�"},
				 },
	},


	{30,"������",{
					{ minVal = 1,maxVal = 2,szName = "�ױ���" },
					{ minVal = 3, maxVal = 4 , szName = "���ܡ�"},
					{ minVal = 5, maxVal = 100 , szName = "������"},
									
				 },
	},

	{1006,"���еȼ�",{
					{ minVal = 1,maxVal = 2,szName = "��ȷ��" },
					{ minVal = 3, maxVal = 4 , szName = "������"},
					{ minVal = 5, maxVal = 6 , szName = "���⡤"},
					{ minVal = 7, maxVal = 8 , szName = "��Ӱ��"},
					{ minVal = 9, maxVal = 10 , szName = "���顤"},
					{ minVal = 11, maxVal = 11 , szName = "ӥ�ۡ�"},
					{ minVal = 12, maxVal = 12 , szName = "��Ϭ��"},
				 },
	},

	{39,"ս����Ѫ",{
					{ minVal = 1,maxVal = 10,szName = "���ա�" },
					{ minVal = 11, maxVal = 100 , szName = "���硤"},
					
				 },
	},

	{40,"ս������",{
					{ minVal = 1,maxVal = 10,szName = "���ա�" },
					{ minVal = 11, maxVal = 100 , szName = "�·���"},
					
				 },
	},


	{41,"������",{
					{ minVal = 1,maxVal = 2,szName = "��׼��" },
					{ minVal = 3, maxVal = 4 , szName = "ӥ�ۡ�"},
					{ minVal = 5, maxVal = 100 , szName = "��ѡ�"},
					
				 },
	},



	{1000,"�﹥",{
					{ minVal = 1,maxVal = 2,szName = "Ұ����" },
					{ minVal = 3, maxVal = 4 , szName = "ǿ����"},
					{ minVal = 5, maxVal = 6 , szName = "ս����"},
					{ minVal = 7, maxVal = 8 , szName = "�ͽ���"},
					{ minVal = 9, maxVal = 10 , szName = "��ס�"},
					{ minVal = 11, maxVal = 11 , szName = "���ߡ�"},
					{ minVal = 12, maxVal = 12 , szName = "ս��"},
				 },
	},

	{1001,"ħ��",{
					{ minVal = 1,maxVal = 2,szName = "������" },
					{ minVal = 3, maxVal = 4 , szName = "ʹ�ࡤ"},
					{ minVal = 5, maxVal = 6 , szName = "ħ����"},
					{ minVal = 7, maxVal = 8 , szName = "���塤"},
					{ minVal = 9, maxVal = 10 , szName = "��ħ��"},
					{ minVal = 11, maxVal = 11 , szName = "ħ��"},
					{ minVal = 12, maxVal = 12 , szName = "����"},
				 },
	},

	{1031,"�����˺�",{
					{ minVal = 1,maxVal = 3,szName = "�ײС�" },
					{ minVal = 4, maxVal = 6 , szName = "���ߡ�"},
					{ minVal = 7, maxVal = 9 , szName = "��Ѫ��"},
					{ minVal = 10, maxVal = 12 , szName = "��ħ��"},
				
				 },
	},

	{1011,"�����ȼ�",{
					{ minVal = 1,maxVal = 2,szName = "���ġ�" },
					{ minVal = 3, maxVal = 4 , szName = "һ����"},
					{ minVal = 5, maxVal = 6 , szName = "�ػ���"},
					{ minVal = 7, maxVal = 8 , szName = "�񱩡�"},
					{ minVal = 9, maxVal = 10 , szName = "���ߡ�"},
					{ minVal = 11, maxVal = 11 , szName = "���ȡ�"},
					{ minVal = 12, maxVal = 12 , szName = "���"},
				
				 },
	},
	{1012,"�����ֿ�",{
					{ minVal = 1,maxVal = 2,szName = "������" },
					{ minVal = 3, maxVal = 4 , szName = "�侲��"},
					{ minVal = 5, maxVal = 6 , szName = "���䡤"},
					{ minVal = 7, maxVal = 8 , szName = "���ѡ�"},
					{ minVal = 9, maxVal = 10 , szName = "��ꡤ"},
					{ minVal = 11, maxVal = 11 , szName = "�¶���"},
					{ minVal = 12, maxVal = 12 , szName = "�����"},
				
				 },
	},

	{1013,"����",{
					{ minVal = 1,maxVal = 2,szName = "������" },
					{ minVal = 3, maxVal = 4 , szName = "��ţ��"},
					{ minVal = 5, maxVal = 6 , szName = "������"},
					{ minVal = 7, maxVal = 8 , szName = "������"},
					{ minVal = 9, maxVal = 10 , szName = "��ɽ��"},
					{ minVal = 11, maxVal = 11 , szName = "��뢡�"},
					{ minVal = 12, maxVal = 12 , szName = "���졤"},
				
				 },
	},

	{1014,"����",{
					{ minVal = 1,maxVal = 2,szName = "������" },
					{ minVal = 3, maxVal = 4 , szName = "������"},
					{ minVal = 5, maxVal = 6 , szName = "�ϻۡ�"},
					{ minVal = 7, maxVal = 8 , szName = "��ħ��"},
					{ minVal = 9, maxVal = 10 , szName = "��ǡ�"},
					{ minVal = 11, maxVal = 11 , szName = "��֪��"},
					{ minVal = 12, maxVal = 12 , szName = "���ߡ�"},
				
				
				 },
	},

	{1015,"����",{
					{ minVal = 1,maxVal = 2,szName = "������" },
					{ minVal = 3, maxVal = 4 , szName = "׳˶��"},
					{ minVal = 5, maxVal = 6 , szName = "���͡�"},
					{ minVal = 7, maxVal = 8 , szName = "������"},
					{ minVal = 9, maxVal = 10 , szName = "���桤"},
					{ minVal = 11, maxVal = 11 , szName = "���١�"},
					{ minVal = 12, maxVal = 12 , szName = "���ˡ�"},
				
				
				 },
	},

	{1018,"��֪",{
					{ minVal = 1,maxVal = 2,szName = "�鶯��" },
					{ minVal = 3, maxVal = 4 , szName = "ħ�ơ�"},
					{ minVal = 5, maxVal = 6 , szName = "��С�"},
					{ minVal = 7, maxVal = 8 , szName = "��ħ��"},
					{ minVal = 9, maxVal = 10 , szName = "��Ϥ��"},
					{ minVal = 11, maxVal = 11 , szName = "���"},
					{ minVal = 12, maxVal = 12 , szName = "���ӡ�"},
				
				 },
	},

	{1016,"����",{
					{ minVal = 1,maxVal = 2,szName = "ƽ�͡�" },
					{ minVal = 3, maxVal = 4 , szName = "ƽ����"},
					{ minVal = 5, maxVal = 6 , szName = "Ԥ�ԡ�"},
					{ minVal = 7, maxVal = 8 , szName = "��⡤"},
					{ minVal = 9, maxVal = 10 , szName = "���ʡ�"},
					{ minVal = 11, maxVal = 11 , szName = "���ء�"},
					{ minVal = 12, maxVal = 12 , szName = "���ѡ�"},
				
				 },
	},


	{1017,"����",{
					{ minVal = 1,maxVal = 2,szName = "��" },
					{ minVal = 3, maxVal = 4 , szName = "���ݡ�"},
					{ minVal = 5, maxVal = 6 , szName = "���顤"},
					{ minVal = 7, maxVal = 8 , szName = "Ѹ�ݡ�"},
					{ minVal = 9, maxVal = 10 , szName = "�̿͡�"},
					{ minVal = 11, maxVal = 11 , szName = "������"},
					{ minVal = 12, maxVal = 12 , szName = "����"},
				
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
			--�Ƚϸ������Ե�ID
			if tAttrName[1] == Arm.AddAttr[i-1].AddAttrID then
				if nTableIndex == 0 then
					nTableIndex = j
				end
				local addAttrRandDef = getAddAttrRandDef( math.ceil(itemDef.WearPre.RoleLevelMin/10), itemDef.Important )
				if addAttrRandDef == nil then					
					break;
				end
				--���ݸ��ӵ�����ֵ ���� ����Ӧ�ĵȼ�
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
				--�Ƚϸ������Ե�ID
				if tAttrName[1] == ItemDef.AddAttr[i-1].ID == nAttrId then
					if nTableIndex == 0 then
						nTableIndex = j
					end
					local addAttrRandDef = getAddAttrRandDef( math.ceil(ItemDef.WearPre.RoleLevelMin/10), ItemDef.Important )
					if addAttrRandDef == nil then
						break;
					end
					--���ݸ��ӵ�����ֵ ���� ����Ӧ�ĵȼ�
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