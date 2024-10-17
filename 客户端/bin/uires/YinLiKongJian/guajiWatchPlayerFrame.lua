MAX_SUITNUM	= 6;
MAX_BASE_ATTR_NUM	= 7;
MAX_ATTACK_ATTR_NUM	= 8;
MAX_DEFEND_ATTR_NUM	= 6;
MAX_SUIT_ATTR_NUM	= 16;
MAX_BOTTOM_LEAVE_SPACE = 5;

function GetPoseState( armid )
	if armid ~= 0 then
		local arm_sub_type  = math.floor(armid/10000);
		local arm_race_type = math.floor(armid/100000);
		for i = 1, table.getn( t_posestate ) do
			if arm_race_type == t_posestate[i][1] and arm_sub_type == t_posestate[i][2] then
				if t_posestate[i][3]  > POSE_END then
					return 10108; -- ��ְҵ�Ļ�����ս���� ����λ108
				else
					return (tonumber(t_posestate[i][3])*1000)+100+tonumber(t_posestate[i][4]); -- ��ְҵ�Ĵ�װ���ı�ս���� ����λ108
				end
			end
		end
	end

	return 10108;
end

function GetRoleAttr( nAttrID, actor )
	if nAttrID < 0 then
		return "";
	end
	local nValue = actor:getShortAttriValule( nAttrID );
	if nAttrID == ATTR_ID_DAMAGE then
		nValue = math.ceil(nValue/100);
	elseif nAttrID == ATTR_ID_MSPD then
		nValue = math.ceil(nValue/400);
	end
	return nValue;
end

function GetRoleBaseAttr( nAttrID, actor )
	if nAttrID < 0 then
		return "";
	end

	local nValue = actor:getBaseAttriValue( nAttrID );
	
	if nAttrID == ATTR_ID_MSPD then
		nValue = math.ceil(nValue/400);
	end
	return nValue;
end

function GetMulAndAddRoleAttr( data, actor )
	local nAddAttrID = data["add"];
	local nMulAttrID = data["mul"];

	local nAddValue = actor:getShortAttriValule( nAddAttrID );
	local nMulValue = actor:getShortAttriValule( nMulAttrID );
	return nAddValue + nMulValue * nAddValue;
end

function GetRoleMulAndAddBaseAttr( data, actor )
	local nAddAttrID = data["add"];
	local nMulAttrID = data["mul"];

	local nAddValue = actor:getBaseAttriValue( nAddAttrID );
	local nMulValue = actor:getBaseAttriValue( nMulAttrID );
	return nAddValue + nMulValue * nAddValue;
end

function GetMulAndAddRoleAttrDesc( data, actor )
	local nAddAttrID = data["add"];
	local nMulAttrID = data["mul"];
	
	local nAddValue = actor:getShortAttriValule( nAddAttrID );
	local nMulValue = actor:getShortAttriValule( nMulAttrID );
	if nMulValue == 0 then
		return nAddValue;
	else
		return nAddValue.." + "..nMulValue.."%" ;
	end
end

function GetRoleBaoJiDamageAttrDesc( nAttrID, actor )
	local nValue = actor:getShortAttriValule( nAddAttrID );
	return nValue.."%";
end

function GetRoleHPandMPAttrDesc( nAttrID, actor )
	if nAttrID == ATTR_ID_MPMAX then
		local nValue	= actor:getShortAttriValule( ATTR_ID_MP );
		local nMaxValue	= actor:getShortAttriValule( ATTR_ID_MPMAX );
		return nValue.."/"..nMaxValue;
	elseif nAttrID == ATTR_ID_HPMAX then
		local nValue	= actor:getShortAttriValule( ATTR_ID_HP );
		local nMaxValue	= actor:getShortAttriValule( ATTR_ID_HPMAX );
		return nValue.."/"..nMaxValue;
	end
end

function GetRoleMovementAttrDesc( nAttrID, actor )
	if nAttrID < 0 then
		return "";
	end

	local nValue = GetRoleCurMovementAttr( nAttrID, actor )
	nValue = math.ceil( nValue * 100 / CLIENT_BASE_MSPD);
	return nValue.."%";
end

function GetRoleBaseMovementAttr( nAttrID, actor )
	return CLIENT_BASE_MSPD;
end

function GetRoleCurMovementAttr( nAttrID, actor )
	return actor:getSpeed();
end

function GetRoleAttrPercentDesc( nAttrID, actor )
	if nAttrID < 0 then
		return "";
	end

	local nValue = actor:getIntAttriValue( nAttrID );
	return nValue.."%";
end

function GetRoleAttrPercentDesc( nAttrID, actor )
	if nAttrID < 0 then
		return "";
	end

	local nValue = actor:getIntAttriValue( nAttrID );
	return nValue.."%";
end

function GetRoleAttrPercent( nAttrID, actor )
	if nAttrID < 0 then
		return 0;
	end
	return actor:getIntAttriValue( nAttrID );
end

--[[
1�������� =  Э�鷢�͵ı�����+ ��������/��������+�����ֿ���/3��
LevelUP�� �����ֿ� MonDefHeavyHit
--]]

function GetRoleAttrCurBaoJiLv( nAttrID, actor )
	if nAttrID < 0 then
		return 0;
	end
	local nBaoJiLvValue		= actor:getShortAttriValule( nAttrID );
	local BaoJiLiValue		= actor:getShortAttriValule( ADD_ATTR_RESULT_BASE_ATTR_HEAVYHIT );
	local levelUpDef		= getLevelUP( actor:getLv() );
	if levelUpDef == nil then
		return nBaoJiLvValue;
	end
	local baoJiDiKangValue	= levelUpDef.MonDefHeavyHit;
	local fResult			= nBaoJiLvValue + ( BaoJiLiValue / ( BaoJiLiValue + baoJiDiKangValue ) / 3  );
	
	return math.ceil( fResult );
end

function GetRoleAttrBaoJiLvPercentDesc( nAttrID, actor )
	if nAttrID < 0 then
		return "";
	end

	local nBaoJiLvValue		= actor:getIntAttriValue( nAttrID );
	local BaoJiLiValue		= actor:getIntAttriValue( ATTR_ID_BASE_ATTR_HEAVY );
	local levelUpDef		= getLevelUP( actor:getLv() );
	if levelUpDef == nil then
		return nBaoJiLvValue.."%";
	end
	local baoJiDiKangValue	= levelUpDef.MonDefHeavyHit;
	local fResult			= nBaoJiLvValue + ( BaoJiLiValue / ( BaoJiLiValue + baoJiDiKangValue ) / 3  ) * 100;
	fResult					= math.min( fResult, 100 );
	local interger, fraction = math.modf( fResult );
	return ( interger.."."..math.floor( fraction * 100 ) ).."%";
end

--[[
2�������� =  Э�鷢�͵������� + ��������/ �ο���������
LevelUP�� �ο������� UIBaseFlee

--]]

function GetRoleAttrCurMingZhongLv( nAttrID, actor )
	if nAttrID < 0 then
		return 0;
	end

	local nMingZhongLvValue		= actor:getShortAttriValule( nAttrID );
	local MingZhongLiValue		= actor:getShortAttriValule( ATTR_ID_HIT );
	local levelUpDef			= getLevelUP( actor:getLv() );
	if levelUpDef == nil then
		return nMingZhongLvValue;
	end
	local MingZhongBaseValue	= levelUpDef.UIBaseFlee;
	local fResult				= nMingZhongLvValue + ( MingZhongLiValue / MingZhongBaseValue );
	
	return math.ceil( fResult );
end

function GetRoleAttrMingZhongLvPercentDesc( nAttrID, actor )
	if nAttrID < 0 then
		return "";
	end

	local nMingZhongLvValue		= actor:getShortAttriValule( nAttrID );
	local MingZhongLiValue		= actor:getShortAttriValule( ATTR_ID_HIT );
	local levelUpDef			= getLevelUP( actor:getLv() );
	if levelUpDef == nil then
		return nMingZhongLvValue.."%";
	end
	local MingZhongBaseValue	= levelUpDef.UIBaseFlee;
	local fResult				= nMingZhongLvValue + ( 0.6 + MingZhongLiValue * 0.3 / MingZhongBaseValue ) * 100;
	fResult						= math.min( fResult, 100 )
	local interger, fraction = math.modf( fResult );
	return ( interger.."."..math.floor( fraction * 100 ) ).."%";
end

function GetRoleAttrBaoJiShangHaiPercentDesc( nAttrID, actor )
	if nAttrID < 0 then
		return "";
	end
	local ADJUST_VALUE = 150;
	local fResult	= actor:getShortAttriValule( nAttrID );
	fResult			= fResult / 100;
	
	local interger, fraction = math.modf( fResult );
	interger		= interger + ADJUST_VALUE;
	return ( interger.."."..math.floor( fraction * 100 ) ).."%";
end

t_RoleAttrInfo =
{
["��������"] = 
{
	[CLIENT_CAREER_WARRIOR] = 
	{
		[1] = { name = "����\b",		attrID = ATTR_ID_LILIANG,	desc = "1����������5��������", },
		[2] = { name = "����\b",		attrID = ATTR_ID_ZHILI,		desc = "1����������5��ħ������", },
		[3] = { name = "����\b",		attrID = ATTR_ID_TILI,		desc = "1����������25������ֵ\n������ս���У�����ֵÿ3��ظ�Ч��", },
		[4] = { name = "����\b",		attrID = ATTR_ID_JINGSHEN,	desc = "1�㾫������25��ħ��ֵ\n������ս���У�ħ��ֵÿ3��ظ�Ч��", },
		[5] = { name = "����\b",		attrID = ATTR_ID_FANGYU,	desc = "1����������5���������", },
		[6] = { name = "��֪\b",		attrID = ATTR_ID_MINJIE,	desc = "1���֪����5��ħ������", },
		[7] = { name = "�ƶ��ٶ�\b",	attrID = ATTR_ID_MSPD,		desc = "��ɫ��ǰ���ƶ��ٶ�", getAttrDescFunc = GetRoleMovementAttrDesc,
				getBaseAttrFunc = GetRoleBaseMovementAttr,			getCurAttrFunc = GetRoleCurMovementAttr },

		maxElemNum = 7,
	},
},

["��������"] = 
{
	[CLIENT_CAREER_WARRIOR] = 
	{	
		[1] = { name = "������\b",	attrID = ATTR_ID_PHSIC_ATK,			maxAttrID = ATTR_ID_PHSIC_MAX_ATK,			desc = "������������ɵ��˺�", },
		[2] = { name = "�����ȼ�\b",	attrID = ATTR_ID_BASE_ATTR_HEAVY,	desc = "���ӹ��������ļ���", },
		[3] = { name = "�����˺�\b",	attrID = ATTR_ID_HEAVY_VAL,			desc = "���������ͨ�˺��İٷֱ�", 
				getAttrDescFunc = GetRoleAttrBaoJiShangHaiPercentDesc },
		[4] = { name = "������\b",		attrID = ATTR_ID_HEAVY,				desc = "��ͬ�ȼ�������ɱ����ļ���", 
				-- getCurAttrFunc = GetRoleAttrCurBaoJiLv
				getAttrDescFunc = GetRoleAttrBaoJiLvPercentDesc,			 
				},

		[5] = { name = "���еȼ�\b",	attrID = ATTR_ID_HIT,				desc = "���ӹ������еļ���", },
		[6] = { name = "������\b",		attrID = ATTR_ID_HIT_RATE,			desc = "��ͬ�ȼ����﹥�����еļ���", 
				-- getCurAttrFunc = GetRoleAttrCurMingZhongLv  
				getAttrDescFunc = GetRoleAttrMingZhongLvPercentDesc,		
				},

		[7] = { name = "�����˺��ӳ�\b",attrID = ATTR_ID_PHSIC_HURT,		desc = "��������˺��������Ӱٷֱ�", getAttrDescFunc = GetRoleAttrPercentDesc, getCurAttrFunc = GetRoleAttrPercent },
		[8] = { name = "ħ���˺��ӳ�\b",attrID = ATTR_ID_ICE_HURT,			desc = "���ħ���˺��������Ӱٷֱ�", getAttrDescFunc = GetRoleAttrPercentDesc, getCurAttrFunc = GetRoleAttrPercent },
		maxElemNum = 8,
	},

	[CLIENT_CAREER_MAGICOR] = 
	{
		[1] = { name = "ħ������\b",	attrID = ATTR_ID_ICE_ATK,			maxAttrID = ATTR_ID_PHSIC_MAX_ATK,			desc = "����ħ��������ɵ��˺�", },
		[2] = { name = "�����ȼ�\b",	attrID = ATTR_ID_BASE_ATTR_HEAVY,	desc = "���ӹ��������ļ���", },
		[3] = { name = "�����˺�\b",	attrID = ATTR_ID_HEAVY_VAL,			desc = "���ӱ�����ɵ��˺�", 
				getAttrDescFunc = GetRoleAttrBaoJiShangHaiPercentDesc },

		[4] = { name = "������\b",		attrID = ATTR_ID_HEAVY,				desc = "��ͬ�ȼ�������ɱ����ļ���", 
				-- getCurAttrFunc = GetRoleAttrCurBaoJiLv  
				getAttrDescFunc = GetRoleAttrBaoJiLvPercentDesc,			
				},
		[5] = { name = "���еȼ�\b",	attrID = ATTR_ID_HIT,				desc = "���ӹ������еļ���", },
		[6] = { name = "������\b",		attrID = ATTR_ID_HIT_RATE,			desc = "��ͬ�ȼ����﹥�����еļ���", 
				-- getCurAttrFunc = GetRoleAttrCurMingZhongLv
				getAttrDescFunc = GetRoleAttrMingZhongLvPercentDesc,		  },
		[7] = { name = "�����˺��ӳ�\b",attrID = ATTR_ID_PHSIC_HURT,		desc = "��������˺��������Ӱٷֱ�",getAttrDescFunc = GetRoleAttrPercentDesc, getCurAttrFunc = GetRoleAttrPercent },
		[8] = { name = "ħ���˺��ӳ�\b",attrID = ATTR_ID_ICE_HURT,			desc = "���ħ���˺��������Ӱٷֱ�", getAttrDescFunc = GetRoleAttrPercentDesc, getCurAttrFunc = GetRoleAttrPercent },
		maxElemNum = 8,
	},
},

["��������"] = 
{
	[CLIENT_CAREER_WARRIOR] = 
	{
		[1] = { name = "�������\b",	attrID = ATTR_ID_PHSIC_DEF,			desc = "�������Խ�ߣ������ܵ��������˺�Խ��", getCurAttrFunc = GetRoleAttrPercent},
		[2] = { name = "ħ������\b",	attrID = ATTR_ID_ICE_DEF,			desc = "ħ������Խ�ߣ������ܵ���ħ���˺�Խ��", getCurAttrFunc = GetRoleAttrPercent },
		[3] = { name = "���ܵȼ�\b",	attrID = ATTR_ID_FLEE,				desc = "����������", },
		[4] = { name = "�����ֿ�\b",	attrID = ATTR_ID_BASE_ATTR_DEFHEAVY,desc = "�����ܵ������ļ���", },
		[5] = { name = "�����˺�����\b",attrID = ATTR_ID_DEF_PHSIC_HURT,	desc = "ֱ�Ӽ����ܵ��������˺�", },
		[6] = { name = "ħ���˺�����\b",attrID = ATTR_ID_DEF_ICE_HURT,		desc = "ֱ�Ӽ����ܵ���ħ���˺�", },

		maxElemNum = 6,
	},
},

["Ԫ�ع�����"] = 
{
	[CLIENT_CAREER_WARRIOR] = 
	{
		[1] = { name = "��Ԫ���˺�\b",	attrID = ATTR_ID_FIRE_ATK,			desc = "����ʱ���ӷ�Ԫ���˺�", },
		[2] = { name = "��Ԫ���˺�\b",	attrID = ATTR_ID_WATER_ATK,			desc = "����ʱ������Ԫ���˺�", },
		[3] = { name = "��Ԫ���˺�\b",	attrID = ATTR_ID_THUNDER_ATK,		desc = "����ʱ���ӵ�Ԫ���˺�", },
		[4] = { name = "��Ԫ���˺�\b",	attrID = ATTR_ID_LIGHT_ATK,			desc = "����ʱ���ӹ�Ԫ���˺�", },
		[5] = { name = "��Ԫ���˺�\b",	attrID = ATTR_ID_NIGHT_ATK,			desc = "����ʱ���Ӱ�Ԫ���˺�", },
		maxElemNum = 5,
	},
},

["Ԫ�ط�����"] = 
{
	[CLIENT_CAREER_WARRIOR] = 
	{
		[1] = { name = "��Ԫ�ؿ���\b",	attrID = ATTR_ID_FIRE_DEF,			desc = "�����ܵ��ķ�Ԫ���˺�", },
		[2] = { name = "��Ԫ�ؿ���\b",	attrID = ATTR_ID_WATER_DEF,			desc = "�����ܵ�����Ԫ���˺�", },
		[3] = { name = "��Ԫ�ؿ���\b",	attrID = ATTR_ID_THUNDER_DEF,		desc = "�����ܵ��ĵ�Ԫ���˺�", },
		[4] = { name = "��Ԫ�ؿ���\b",	attrID = ATTR_ID_LIGHT_DEF,			desc = "�����ܵ��Ĺ�Ԫ���˺�", },
		[5] = { name = "��Ԫ�ؿ���\b",	attrID = ATTR_ID_NIGHT_DEF,			desc = "�����ܵ��İ�Ԫ���˺�", },
		maxElemNum = 5,
	},
},
};

local bWatchPlayerFrameShowFashion = false;

function WatchPlayerFrameOnLoad()
	this:RegisterEvent("GE_WATCHPLAYER_WATCH");


	for i=1, 2, 1 do
		for j=1, 8, 1 do
			local bagButton = getglobal( "WatchPlayerFrameEquip"..((i-1)*8+j));
			--bagButton:SetPoint("TOPLEFT", "WatchPlayerFrame", "TOPLEFT", ( 14 + 5*38*(i-1) + (i-1)*22 ), ( (j-1)*43 + 45 ) );
			bagButton:SetClientUserData( 0, 0 );
		end
	end
end

function WatchPlayerFrame_Close()
	WatchPlayerFrame:Hide();
end

--@Desc:������Ӧ���������������¼���Ϣ
--@return: Nothing
function WatchPlayerFrameOnEvent()
	if ( arg1 == "GE_WATCHPLAYER_WATCH" ) then
		if WatchPlayerFrame:IsShown() then
			WatchPlayerFrame:Hide();
		end
		SetWatchPlayerInfo();
		WatchPlayerFrame:Show();
	end

end

-- �۲���ҵĽ���֡ˢ�� �������Ƿ�����Ұ��Χ
function WatchPlayerFrameOnUpdate()
	local WatchPlayer = nil;
	WatchPlayer = OfflineWatchPlayer:getWatchPlayer();

	if WatchPlayer == nil then
		WatchPlayerFrame:Hide();
	end
	--UpdateWPAttr();
end

function GetWatchPlayerEquipItem( WPData, GridIndex )
	for i = 1, MAX_WEAR do
		local Item = WPData.m_ItemRoleWear[i-1];
		if Item:getGridIdx() == GridIndex then
			return Item;
		end
	end
	return nil;
end

function GetWatchPlayerSuitArmNum( nSuitId )
	local t_SuitArm = {};
	local nSuitArmNum = 0;
	local nSuitTotalNum = getSuitEquipNum( nSuitId );
	for i = 1, nSuitTotalNum do
		for j = 1, MAXEQUIPPOS do
			local bagButton = getglobal( "WatchPlayerFrameEquip"..j );
			local Item = OfflineWatchPlayer:getItem( bagButton:GetClientID() );
			if getSuitSetId( nSuitId, ( i - 1 ) ) == Item:getItemId() and not Item:isValidEnd() then
				t_SuitArm[Item:getItemId()] = 1;
			end
		end
	end
	for _,v in pairs( t_SuitArm ) do
		nSuitArmNum = nSuitArmNum + v;
	end
	return nSuitArmNum;
end

--@desc:�����Ϣ��ʾ����
--@return: Nothing
function WatchPlayerFrameOnShow()  
	-- ���ô�������
	WPAttrBtn_AdvanceAttrLabelBtn:Checked();
	WPAttrBtn_SpecialAttrLabelBtn:DisChecked();
	WatchPlayerFrame:SetPoint("topleft", "$parent", "topleft", 80,12 );
	--SetWatchPlayerInfo();

	WPAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont1:SetText("��");
	WPAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont2:SetText("��");
	WPAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont3:SetText("��");
	WPAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont4:SetText("��");
	WPAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont5:SetText("");
	WPAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont1:SetText("��");
	WPAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont2:SetText("��");
	WPAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont3:SetText("��");
	WPAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont4:SetText("��");
	WPAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont5:SetText("");
	WatchPlayerFrame_ShouSuoBtn:Show();
	WatchPlayerFrame_ZhanKaiBtn:Hide();
	ShowWatchPlayerFashionFrames( false );
	WatchPlayerFrame_FashionBtn:DisChecked();
	WatchPlayerFrame_EquipBtn:Checked();
	ShowWatchPlayerFashionFrames( false );
end

function SetWatchPlayerInfo()
	local WatchPlayer = nil;
	WatchPlayer = OfflineWatchPlayer:getWatchPlayer();

	if WatchPlayer == nil then
		return;
	end

	for i=1, MAXEQUIPPOS do
		local bagButton = getglobal( "WatchPlayerFrameEquip"..i);
		local UVAnimationTex = getglobal( "WatchPlayerFrameEquip"..i.."UVAnimationTex");
		local ItemTex = getglobal( "WatchPlayerFrameEquip"..i.."IconTexture");
		local ItemBoxTex = getglobal( "WatchPlayerFrameEquip"..i.."BoxTexture");
		local redTexture = getglobal( "WatchPlayerFrameEquip"..i.."RedTexture" );
		local BindTexture = getglobal( "WatchPlayerFrameEquip"..i.."BindTexture" );
		local Item = OfflineWatchPlayer:getItem( bagButton:GetClientID() );
		if( Item:getItemId() ~= 0 ) then
			local itemdef = Item:getItemDef();
			SetEquipImportantEffectUV( itemdef, UVAnimationTex:GetName(), Item ); -- ����װ����Ч
			bagButton:SetClientUserData(3,CONTAINER_TYPE_WEAR);

			local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
			-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			ItemTex:SetTexture( IconPath );

			ItemBoxTex:Show();
			if Item:isValidEnd() then
				redTexture:Show()
			else
				redTexture:Hide();
			end
			if Item:isItemBind() then
				BindTexture:Show();
			else
				BindTexture:Hide();
			end
		else
			UVAnimationTex:Hide();
			ItemBoxTex:Hide();
			redTexture:Hide();
			BindTexture:Hide();
			ItemTex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
		end
	end

	--��ʾ�����Ҫ��Ϣ
	local nLv		= WatchPlayer:getLv();
	local szName	= WatchPlayer:getName();

	--��ʾ������
	WatchPlayerFrameTitleFont:SetText( szName );

	local nCurExp = GetWPAttr( ATTR_ID_EXP );
	local nLevelNextExp = WatchPlayer:getLvExp( nLv );

	WatchPlayerFrame_CurExp:SetText( nCurExp );
	WatchPlayerFrame_NextUpLvExp:SetText( nLevelNextExp );

	local ModelViewer = getglobal("WatchPlayerFrame_ModelView");
	
	ModelViewer:setEntityAvatar(0);
	OfflineWatchPlayer:setAvatarAttrib();
	if bWatchPlayerFrameShowFashion then
		setAvatarAttrib( POS_ID_FASHION_HAIR,1 );
	end
	for j=1, 32 do
		if getAvatarAttrib( j-1 ) ~= 0 then
			if j == 1 then
				ModelViewer:setAvatarAttrib(0, j-1, getAvatarAttrib( j-1 ),getAvatarStarLv( j-1 ), true );
			else
				ModelViewer:setAvatarAttrib(0, j-1, getAvatarAttrib( j-1 ),getAvatarStarLv( j-1 ) );
			end
		end
	end
	ModelAngle = 0;
	ModelViewer:setEntityRotate(0,0);
	ModelViewer:playEntityAnim(0, 10100, nil );
	local leftarmid		= getAvatarAttrib( POS_ID_LEFTHAND );
	local rightarmid	= getAvatarAttrib( POS_ID_RIGHTHAND );

	if leftarmid ~= 0 or rightarmid ~= 0 then
		ModelViewer:playEntityAnim(0, GetPoseState( leftarmid ), nil );
		ModelViewer:playEntityAnim(0, GetPoseState( rightarmid ), nil );
	end
	--����С��
	if WatchPlayer:getGender() == 1 then
		ModelViewer:setPetScale(0,0.8,0.8,0.8);--��������
	else
		ModelViewer:setPetScale(0,0.9,0.9,0.9);--��������
	end	
	ModelViewer:setEntityPosition(0,0,20,0);
	WPAttrBtn:Show();
end

function WatchPlayerFrameOnHide()
	if NormalItemTipsFrame:IsShown() then
		if IsInFrameAbs( "NormalItemTipsFrame", "PlayerContainerFrame" ) then
			NormalItemTipsFrame:Hide();
		end
	end
end

function WPActiveButton_OnEnter()
end

function WPActiveButton_OnLeave()
	GameTooltip:Hide();
end

function WatchPlayerFrame_PetBtn_OnClick()
	RideFrame:Hide();
	WatchPlayerFrame:Hide();
	EquipRealX	= WatchPlayerFrame:GetRealLeft();
	EquipRealY	= WatchPlayerFrame:GetRealTop();
	PetFrame:Show();
end

--@desc:�����ת��ťʱ����ת��Ԥ��ģ��
--@return: Nothing
function WatchPlayerFrame_TurnLeftBtn_OnMouseDown()
	local ModelViewer = getglobal("WatchPlayerFrame_ModelView");
	ModelAngle = ModelAngle - 5;
	ModelViewer:setEntityRotate(0,ModelAngle);
end

--@desc:�������ת��ťʱ����ת��Ԥ��ģ��
--@return: Nothing
function WatchPlayerFrame_TurnRightBtn_OnMouseDown()
	local ModelViewer = getglobal("WatchPlayerFrame_ModelView");
	ModelAngle = ModelAngle + 5;
	ModelViewer:setEntityRotate(0,ModelAngle);
end

function WPAttrBtn_CloseBtn_OnClick()
	WPAttrBtn:Hide();
end

function GetWPAttr( nIndex )
	if nIndex == 0 then
		return "";
	end

	local WatchPlayer = nil;
	WatchPlayer = OfflineWatchPlayer:getWatchPlayer();
	if WatchPlayer == nil then
		return 0;
	end
	local nValue = WatchPlayer:getShortAttriValule( nIndex );
	if nIndex == ATTR_ID_DAMAGE then
		nValue = math.ceil(nValue/100);
	end
	return nValue;
end

function WPAttrBtn_OnShow()
	WPAttrBtn_AdvanceAttrLabelBtn:Checked();
	WPAttrBtn_SpecialAttrLabelBtn:DisChecked();
	UpdateWPAttr();
	if not bWatchPlayerFrameShowFashion then
		WatchPlayerFrame_ShouSuoBtn:Show();
	else
		WatchPlayerFrame_ShouSuoBtn:Hide();
	end
	WatchPlayerFrame_ZhanKaiBtn:Hide();
end

function WPAttrBtn_OnHide()
	if not bWatchPlayerFrameShowFashion then
		WatchPlayerFrame_ZhanKaiBtn:Show();
	else
		WatchPlayerFrame_ZhanKaiBtn:Hide();
	end
	WatchPlayerFrame_ShouSuoBtn:Hide();
	WatchPlayerFrame_ZhanKaiBtn:Show();
end

function WPAttrBtn_AdvanceAttrLabelBtn_OnClick()
	WPAttrBtn_AdvanceAttrLabelBtn:Checked();
	WPAttrBtn_SpecialAttrLabelBtn:DisChecked();

	UpdateWatchNormalAttackAttr();
	UpdateWatchNormalDefendAttr();

	WPAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont1:SetText("��");
	WPAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont2:SetText("��");
	WPAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont3:SetText("��");
	WPAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont4:SetText("��");
	WPAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont5:SetText("");
	WPAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont1:SetText("��");
	WPAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont2:SetText("��");
	WPAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont3:SetText("��");
	WPAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont4:SetText("��");
	WPAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont5:SetText("");
end

function WPAttrBtn_SpecialAttrLabelBtn_OnClick()
	WPAttrBtn_AdvanceAttrLabelBtn:DisChecked();
	WPAttrBtn_SpecialAttrLabelBtn:Checked();

	UpdateWatchSpecialAttackAttr();
	UpdateWatchSpecialDefendAttr();

	WPAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont1:SetText("Ԫ");
	WPAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont2:SetText("��");
	WPAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont3:SetText("��");
	WPAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont4:SetText("��");
	WPAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont5:SetText("��");
	WPAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont1:SetText("Ԫ");
	WPAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont2:SetText("��");
	WPAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont3:SetText("��");
	WPAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont4:SetText("��");
	WPAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont5:SetText("��");end

-- ���»�������
function ClearWatchBaseAttr()
	for i = 1, MAX_BASE_ATTR_NUM do
		local btn = getglobal( "WPAttrBtn_BaseAttrBtn"..i );
		btn:Hide();
	end
end

function UpdateWatchBaseAttr()
	ClearWatchBaseAttr();
	local WatchPlayer = nil;
	WatchPlayer = OfflineWatchPlayer:getWatchPlayer();
	local t_attrList	= t_RoleAttrInfo["��������"][1];
	local maxAttrNum	= t_attrList["maxElemNum"];
	for i = 1, maxAttrNum do
		local oneElem		= t_attrList[i];
		local funcGetBase	= oneElem["getBaseAttrFunc"] ~= nil and oneElem["getBaseAttrFunc"]	or GetRoleBaseAttr;
		local funcGetCur	= oneElem["getCurAttrFunc"] ~= nil	and oneElem["getCurAttrFunc"]	or GetRoleAttr;

		local nCurValue		= funcGetCur( oneElem["attrID"],	WatchPlayer );
		local nBaseValue	= funcGetBase( oneElem["attrID"],	WatchPlayer );

		local t_color	= { r = 255, g = 254, b = 189 };
		if nCurValue > nBaseValue then
			t_color	= { r = 15, g = 255, b = 0 };
		elseif nCurValue < nBaseValue then
			t_color	= { r = 255, g = 0, b = 0 };
		end

		local btn = getglobal( "WPAttrBtn_BaseAttrBtn"..i );
		btn:SetClientString( oneElem["desc"] );
		local funcGetDesc = oneElem["getAttrDescFunc"];
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneElem["name"] }, 
							[btn:GetName().."ValueFont"]	= 
							{	value = funcGetDesc ~= nil and funcGetDesc( oneElem["attrID"], WatchPlayer ) or nCurValue,	
								color = t_color, 
							},
						};
		util.SetFonts( t_fonts );
		btn:Show();
	end
end

-- ������ͨ��������
function ClearWatchAttackAttr()
	for i = 1, MAX_ATTACK_ATTR_NUM do
		local btn = getglobal( "WPAttrBtn_AttackAttrBtn"..i );
		btn:Hide();
	end
end

function UpdateWatchNormalAttackAttr()
	ClearWatchAttackAttr();
	
	local WatchPlayer = nil;
	WatchPlayer = OfflineWatchPlayer:getWatchPlayer();

	local t_attrList	= t_RoleAttrInfo["��������"][1];
	local maxAttrNum	= t_attrList["maxElemNum"];

	for i = 1, maxAttrNum do
		local oneElem		= t_attrList[i];
		local funcGetBase	= oneElem["getBaseAttrFunc"] ~= nil and oneElem["getBaseAttrFunc"]	or GetRoleBaseAttr;
		local funcGetCur	= oneElem["getCurAttrFunc"] ~= nil	and oneElem["getCurAttrFunc"]	or GetRoleAttr;

		local nCurValue		= funcGetCur( oneElem["attrID"],	WatchPlayer );
		local nBaseValue	= funcGetBase( oneElem["attrID"],	WatchPlayer );

		local t_color	= { r = 255, g = 254, b = 189 };
		if nCurValue > nBaseValue then
			t_color	= { r = 15, g = 255, b = 0 };
		elseif nCurValue < nBaseValue then
			t_color	= { r = 255, g = 0, b = 0 };
		end

		local nCurMaxValue  = nil;
		local nBaseMaxValue = nil;

		if oneElem["maxAttrID"] ~= nil then
			nCurMaxValue = funcGetCur( oneElem["maxAttrID"],	WatchPlayer );
			nBaseMaxValue = funcGetBase( oneElem["maxAttrID"],	WatchPlayer );
			if nCurMaxValue < nBaseMaxValue then
				t_color	= { r = 255, g = 0, b = 0 };
			end
			nCurValue = nCurValue .. "-" .. nCurMaxValue
		end

		local btn = getglobal( "WPAttrBtn_AttackAttrBtn"..i );
		btn:SetClientString( oneElem["desc"] );
		local funcGetDesc = oneElem["getAttrDescFunc"];
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneElem["name"] }, 
							[btn:GetName().."ValueFont"]	= 
							{	value = funcGetDesc ~= nil and funcGetDesc( oneElem["attrID"], WatchPlayer ) or nCurValue,	
								color = t_color, 
							},
						};
		util.SetFonts( t_fonts );
		btn:Show();
	end
end

-- ������ͨ��������
function ClearWatchDefendAttr()
	for i = 1, MAX_DEFEND_ATTR_NUM do
		local btn = getglobal( "WPAttrBtn_DefendAttrBtn"..i );
		btn:Hide();
	end
end

function UpdateWatchNormalDefendAttr()
	ClearWatchDefendAttr();
	
	local WatchPlayer = nil;
	WatchPlayer = OfflineWatchPlayer:getWatchPlayer();

	
	local t_attrList	= t_RoleAttrInfo["��������"][1];
	local maxAttrNum	= t_attrList["maxElemNum"];

	for i = 1, maxAttrNum do
		local oneElem		= t_attrList[i];
		local funcGetBase	= oneElem["getBaseAttrFunc"] ~= nil and oneElem["getBaseAttrFunc"]	or GetRoleBaseAttr;
		local funcGetCur	= oneElem["getCurAttrFunc"] ~= nil	and oneElem["getCurAttrFunc"]	or GetRoleAttr;

		local nCurValue		= funcGetCur( oneElem["attrID"],	WatchPlayer );
		local nBaseValue	= funcGetBase( oneElem["attrID"],	WatchPlayer );

		local t_color	= { r = 255, g = 254, b = 189 };
		if nCurValue > nBaseValue then
			t_color	= { r = 15, g = 255, b = 0 };
		elseif nCurValue < nBaseValue then
			t_color	= { r = 255, g = 0, b = 0 };
		end

		if oneElem["maxAttrID"] ~= nil then
			nCurMaxValue = funcGetCur( oneElem["maxAttrID"],	WatchPlayer );
			nBaseMaxValue = funcGetBase( oneElem["maxAttrID"],	WatchPlayer );
			if nCurMaxValue < nBaseMaxValue then
				t_color	= { r = 255, g = 0, b = 0 };
			end
			nCurValue = nCurValue .. "-" .. nCurMaxValue
		end

		local btn			= getglobal( "WPAttrBtn_DefendAttrBtn"..i );
		btn:SetClientString( oneElem["desc"] );
		local funcGetDesc	= oneElem["getAttrDescFunc"];
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneElem["name"] }, 
							[btn:GetName().."ValueFont"]	= 
							{	value = funcGetDesc ~= nil and funcGetDesc( oneElem["attrID"], WatchPlayer ) or nCurValue,	
								color = t_color, 
							},
						};
		util.SetFonts( t_fonts );
		btn:Show();
	end
end

-- �������⹥������
function UpdateWatchSpecialAttackAttr()
	ClearWatchAttackAttr();
	
	local WatchPlayer = nil;
	WatchPlayer = OfflineWatchPlayer:getWatchPlayer();
	
	local t_attrList	= t_RoleAttrInfo["Ԫ�ع�����"][1];
	local maxAttrNum	= t_attrList["maxElemNum"];

	for i = 1, maxAttrNum do
		local oneElem		= t_attrList[i];
		local funcGetBase	= oneElem["getBaseAttrFunc"] ~= nil and oneElem["getBaseAttrFunc"]	or GetRoleBaseAttr;
		local funcGetCur	= oneElem["getCurAttrFunc"] ~= nil	and oneElem["getCurAttrFunc"]	or GetRoleAttr;

		local nCurValue		= funcGetCur( oneElem["attrID"],	WatchPlayer );
		local nBaseValue	= funcGetBase( oneElem["attrID"],	WatchPlayer );

		local t_color	= { r = 255, g = 254, b = 189 };
		if nCurValue > nBaseValue then
			t_color	= { r = 15, g = 255, b = 0 };
		elseif nCurValue < nBaseValue then
			t_color	= { r = 255, g = 0, b = 0 };
		end

		if oneElem["maxAttrID"] ~= nil then
			nCurMaxValue = funcGetCur( oneElem["maxAttrID"],	WatchPlayer );
			nBaseMaxValue = funcGetBase( oneElem["maxAttrID"],	WatchPlayer );
			if nCurMaxValue < nBaseMaxValue then
				t_color	= { r = 255, g = 0, b = 0 };
			end
			nCurValue = nCurValue .. "-" .. nCurMaxValue
		end

		local btn = getglobal( "WPAttrBtn_AttackAttrBtn"..i );
		btn:SetClientString( oneElem["desc"] );
		local funcGetDesc = oneElem["getAttrDescFunc"];
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneElem["name"] }, 
							[btn:GetName().."ValueFont"]	= 
							{	value = funcGetDesc ~= nil and funcGetDesc( oneElem["attrID"], WatchPlayer ) or nCurValue,	
								color = t_color, 
							},
						};
		util.SetFonts( t_fonts );
		btn:Show();
	end
end

-- ���������������
function UpdateWatchSpecialDefendAttr()
	ClearWatchDefendAttr();
	
	local WatchPlayer = nil;
	WatchPlayer = OfflineWatchPlayer:getWatchPlayer();
	
	local t_attrList	= t_RoleAttrInfo["Ԫ�ط�����"][1];
	local maxAttrNum	= t_attrList["maxElemNum"];

	for i = 1, maxAttrNum do
		local oneElem		= t_attrList[i];
		local funcGetBase	= oneElem["getBaseAttrFunc"] ~= nil and oneElem["getBaseAttrFunc"]	or GetRoleBaseAttr;
		local funcGetCur	= oneElem["getCurAttrFunc"] ~= nil	and oneElem["getCurAttrFunc"]	or GetRoleAttr;

		local nCurValue		= funcGetCur( oneElem["attrID"],	WatchPlayer );
		local nBaseValue	= funcGetBase( oneElem["attrID"],	WatchPlayer );

		local t_color	= { r = 255, g = 254, b = 189 };
		if nCurValue > nBaseValue then
			t_color	= { r = 15, g = 255, b = 0 };
		elseif nCurValue < nBaseValue then
			t_color	= { r = 255, g = 0, b = 0 };
		end

		if oneElem["maxAttrID"] ~= nil then
			nCurMaxValue = funcGetCur( oneElem["maxAttrID"],	WatchPlayer );
			nBaseMaxValue = funcGetBase( oneElem["maxAttrID"],	WatchPlayer );
			if nCurMaxValue < nBaseMaxValue then
				t_color	= { r = 255, g = 0, b = 0 };
			end
			nCurValue = nCurValue .. "-" .. nCurMaxValue
		end

		local btn			= getglobal( "WPAttrBtn_DefendAttrBtn"..i );
		btn:SetClientString( oneElem["desc"] );
		local funcGetDesc	= oneElem["getAttrDescFunc"];
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneElem["name"] }, 
							[btn:GetName().."ValueFont"]	= 
							{	value = funcGetDesc ~= nil and funcGetDesc( oneElem["attrID"], WatchPlayer ) or nCurValue,	
								color = t_color, 
							},
						};
		util.SetFonts( t_fonts );
		btn:Show();
	end
end

-- ������װ����
function ClearWatchSuitAttr()
	for i = 1, MAX_SUIT_ATTR_NUM do
		local btn = getglobal( "WPAttrBtn_SuitAttrBtn"..i );
		btn:Hide();
	end
	
	local t_hideIcons = { "WPAttrBtnSuitBackGroundButton", };
	util.HideIcons( t_hideIcons );
end

function UpdateWatchSuitAttr()
	ClearWatchSuitAttr();
	
	local t_SuitList = {};
	local isHaveSuitFunc = 
	function ( itemDef )
		for _, data in ipairs( t_SuitList ) do
			if itemDef.SuitSetID == data["suitID"] then
				return true;
			end
		end

		return false;
	end
	
	for i = 1, MAXEQUIPPOS do
		local bagButton	= getglobal( "WatchPlayerFrameEquip"..i);
		local Item		= OfflineWatchPlayer:getItem( bagButton:GetClientID() );
		local itemDef	= Item:getItemDef();
		if itemDef ~= nil and itemDef.SuitSetID > 0 then
			if not isHaveSuitFunc( itemDef ) then
				local SuitSet		= getSuitSet( itemDef.SuitSetID );
				local t_SuitColor	= GetItemNameColor(itemDef);

				local t_oneSuitElem = 
				{	name	= SuitSet.Name,
					value	= GetWatchPlayerSuitArmNum( itemDef.SuitSetID ) .. "/"..getSuitEquipNum( itemDef.SuitSetID ),
					color	= { r = t_SuitColor[1], g = t_SuitColor[2], b = t_SuitColor[3] },
					suitID	= itemDef.SuitSetID,
				};
				table.insert( t_SuitList, t_oneSuitElem );
			end
		end
	end
	
	-- ������װ����ť����
	for i = 1, table.getn( t_SuitList ) do
		local oneSuitData	= t_SuitList[i];
		local btn			= getglobal( "WPAttrBtn_SuitAttrBtn"..i );
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneSuitData["name"],	color = oneSuitData["color"],  }, 
							[btn:GetName().."ValueFont"]	= {	value = oneSuitData["value"],	color = oneSuitData["color"], },
						};
		util.SetFonts( t_fonts );
		btn:Show();
	end
	
	local ONE_SUIT_BTN_HEIGHT			= 16;
	local EXTEND_FRAME_SIZE_LIMIT_NUM	= 2;
	local nSuitNum	= table.getn( t_SuitList );
	if nSuitNum > 0 then
		WPAttrBtnSuitBackGroundButton:Show();
		WPAttrBtnSuitBackGroundButton:SetHeight( math.max( EXTEND_FRAME_SIZE_LIMIT_NUM, table.getn( t_SuitList ) ) * ONE_SUIT_BTN_HEIGHT );
	end
	
	if nSuitNum > EXTEND_FRAME_SIZE_LIMIT_NUM then
		local size = 456 - 32 - MAX_BOTTOM_LEAVE_SPACE +  nSuitNum * 16;
		WPAttrBtn:SetHeight( size );
	else
		WPAttrBtn:SetHeight( 456 );
	end
	
end

function WPAttrBtn_NodeBtn_OnEnter()
	local szText = this:GetClientString();
	if szText == "" then
		return;
	end
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = "cursor", vertical = "top" } );
end

function WPAttrBtn_NodeBtn_OnLeave()
	GameTooltip:Hide();
end

function UpdateWPAttr()
	local WatchPlayer = nil;
	WatchPlayer = OfflineWatchPlayer:getWatchPlayer();
	if WatchPlayer == nil then return end

	--��ʾ�����Ҫ��Ϣ
	local nLv			=	WatchPlayer:getLv();
	local nCurExp			=	WatchPlayer:getShortAttriValule( ATTR_ID_EXP );
	local MaxHp			=	WatchPlayer:getShortAttriValule( ATTR_ID_HPMAX );
	local MaxMp			=	WatchPlayer:getShortAttriValule( ATTR_ID_MPMAX );
	local nLevelNextExp =	WatchPlayer:getLvExp( nLv );

	local t_fonts = 
	{ 
		["WPAttrNameFont"]				= { value = WatchPlayer:getName() }, 
		["WPAttrLvFont"]				= { value = "�ȼ���"..WatchPlayer:getLv().."��" }, 
		["WPAttrRaceFont"]				= { value = "ְҵ��"..GetCareerName( WatchPlayer:getRace() ) }, 
		["WatchPlayerFrame_CurExp"]		= { value = nCurExp }, 
		["WatchPlayerFrame_NextUpLvExp"]= { value = nLevelNextExp }, 
	};
	util.SetFonts( t_fonts );
	
	-- ����HP��MP��
	local MAX_HP_LENGTH = 152;
	--��ʾ������
	-- ����HP
	local maxHp		= WatchPlayer:getMaxHP();
	local hp		= WatchPlayer:getHP();
	hp				= math.max( math.min( hp, maxHp ), 0 );
	local fHpScale	= math.max( math.min( hp / maxHp, 1 ), 0 );
	local hpBtn		= getglobal( "WPAttrBtnFirstBaseLifeAttrButton" );
	local hpTex		= getglobal(  hpBtn:GetName().."Tex" );
	hpTex:SetTexUV( ( 1 - fHpScale ) * MAX_HP_LENGTH, 784, MAX_HP_LENGTH, 11 );
	hpTex:setMask( [[uires\ui\mask\RenWuShuXingMoFa,ShengMing  mask.tga]]);

	-- ����MP
	local maxMp		= WatchPlayer:getMaxMP();
	local mp		= WatchPlayer:getMP();
	mp				= math.max( math.min( mp, maxMp ), 0 );
	local fMpScale	= math.max( math.min( mp / maxMp, 1 ), 0 );
	local mpBtn		= getglobal( "WPAttrBtnFirstBaseMagicAttrButton" );
	local mpTex		= getglobal(  mpBtn:GetName().."Tex" );
	mpTex:SetTexUV( ( 1 - fMpScale ) * MAX_HP_LENGTH, 796, MAX_HP_LENGTH, 11 );
	mpTex:setMask( [[uires\ui\mask\RenWuShuXingMoFa,ShengMing  mask.tga]]);

	local t_fonts = {	[hpBtn:GetName().."PercentFont"] = { value = hp.."/"..maxHp }, 
						[mpBtn:GetName().."PercentFont"] = { value = mp.."/"..maxMp },
					};
	util.SetFonts( t_fonts );

	UpdateWatchBaseAttr();
	-- ��ѡ��ĸ߼�����	IsEnable
	if WPAttrBtn_AdvanceAttrLabelBtn:IsChecked() then
		UpdateWatchNormalAttackAttr();
		UpdateWatchNormalDefendAttr();
	else
		UpdateWatchSpecialAttackAttr();
		UpdateWatchSpecialDefendAttr();
	end

	UpdateWatchSuitAttr();
end
function ShowWatchPlayerFashionFrames( bShow )
	if bShow then
		for i=1,7 do
			local tex = getglobal( "WatchPlayerFrameLeftEquipbackground" .. i );
			tex:Hide();
			tex = getglobal( "WatchPlayerFrameRightEquipbackground" .. i );
			tex:Hide();
		end
		for i=1,16 do
			local btn = getglobal( "WatchPlayerFrameEquip" .. i );
			btn:Hide();
		end
		-- ����
		WatchPlayerFrameExpBkg:Hide();
		WatchPlayerFrameExpBkg1:Hide();
		WatchPlayerFrame_CurExpDesc:Hide();
		WatchPlayerFrame_NextUpLvExpDesc:Hide();
		WatchPlayerFrame_CurExp:Hide();
		WatchPlayerFrame_NextUpLvExp:Hide();

		WatchPlayerFrameFashionBkg:Show();
		for i=1,9 do
			local tex = getglobal( "WatchPlayerFrameFashionItemBkg" ..i );
			tex:Show();
		end
		WatchPlayerFrameEquip8:Show();
		WatchPlayerFrameEquip17:Show();
		WatchPlayerFrameEquip18:Show();
		WatchPlayerFrameEquip19:Show();
		WatchPlayerFrameEquip20:Show();
		WatchPlayerFrameEquip21:Show();
		WatchPlayerFrameEquip22:Show();
		WatchPlayerFrame_ZhanKaiBtn:Hide();
		WatchPlayerFrame_ShouSuoBtn:Hide();
	else
		for i=1,7 do
			local tex = getglobal( "WatchPlayerFrameLeftEquipbackground" .. i );
			tex:Show();
			tex = getglobal( "WatchPlayerFrameRightEquipbackground" .. i );
			tex:Show();
		end
		for i=1,16 do
			local btn = getglobal( "WatchPlayerFrameEquip" .. i );
			btn:Show();
		end
		-- ����
		WatchPlayerFrameExpBkg:Show();
		WatchPlayerFrameExpBkg1:Show();
		WatchPlayerFrame_CurExpDesc:Show();
		WatchPlayerFrame_NextUpLvExpDesc:Show();
		WatchPlayerFrame_CurExp:Show();
		WatchPlayerFrame_NextUpLvExp:Show();

		WatchPlayerFrameFashionBkg:Hide();
		for i=1,9 do
			local tex = getglobal( "WatchPlayerFrameFashionItemBkg" ..i );
			tex:Hide();
		end
		WatchPlayerFrameEquip8:Hide();
		WatchPlayerFrameEquip17:Hide();
		WatchPlayerFrameEquip18:Hide();
		WatchPlayerFrameEquip19:Hide();
		WatchPlayerFrameEquip20:Hide();
		WatchPlayerFrameEquip21:Hide();
		WatchPlayerFrameEquip22:Hide();
		if WPAttrBtn:IsShown() then
			WatchPlayerFrame_ShouSuoBtn:Show();
			WatchPlayerFrame_ZhanKaiBtn:Hide();
		else
			WatchPlayerFrame_ShouSuoBtn:Hide();
			WatchPlayerFrame_ZhanKaiBtn:Show();
		end	
	end
	bWatchPlayerFrameShowFashion = bShow;
	WatchPlayerFrameEquip7:Hide();
end

function WatchPlayerFrameEquipBtn_OnClick()
	if not bWatchPlayerFrameShowFashion then
		return
	end
	ShowWatchPlayerFashionFrames( false );
	WatchPlayerFrame_FashionBtn:DisChecked();
	WatchPlayerFrame_EquipBtn:Checked();

end

function WatchPlayerFrameFashionBtn_OnClick()
	if bWatchPlayerFrameShowFashion then
		return
	end
	ShowWatchPlayerFashionFrames( true );
	WatchPlayerFrame_FashionBtn:Checked();
	WatchPlayerFrame_EquipBtn:DisChecked();
end
