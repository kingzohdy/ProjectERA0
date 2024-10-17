local MAX_VIEW_LINE = 16;
-- �ڵ��ѧϰ��ť�Ժ�,���ܼ���ѧϰ��һ�����ܵ�ԭ����û�ж�studyButton�������Ӧ��������ɵ�
-- ���ڱ�ʾû��ѡ���κ����͵ļ���
local SKILLLEARN_TYPE_NONE = -1;
-- ����ָʾ��ǰ��ѡ�еļ���
local t_CurSelectSkill = { skillId = 0, skillLv = 0, skillType = SKILLLEARN_TYPE_NONE, nLineNumOnUI = 0 };

--ָʾȫ������ҳ��Ϳ�ѧϰ����ҳ��ı�ʾ��
local SKILLPAGE_TOTAL	 = 3;
local SKILLPAGE_CANSTUDY = 4;
local SKILLPAGE_NONSTUDY = 5;

local nCurSkillPage = SKILLPAGE_CANSTUDY;--���ڱ��浱ǰ��ʾ�ļ���ҳ�����ĸ�,Ĭ����ʾȫ������ҳ��

--local t_ParentNode = {  ���ڵ�����, ��������, ָʾ���ڵ��Ƿ�չ����״̬��־, {�ӽڵ㼯���б�}};
--t_ParentNodeList = { [��������1] = {��Ӧ���ڵ�ParentNode1}, [��������2] = {��Ӧ���ڵ�ParentNode2}, ... }
local PARENT_NODE_LENGTH = 4;--��Ϊ���ڵ�ĳ���

--local t_ChildNode  = { �ӽڵ�����,����ID, ����Ҫ�����͵ȼ�};
--t_ChildNodeList = { [��������1] = {�����б�ChildNode1}, [��������2] = {�����б�ChildNode2}, ... };
local CHILD_NODE_LENGTH = 3;--��Ϊ�ӽڵ�ĳ���

--�������ְҵ�ļ��ܷ���
local t_WarriorSkillTypeList = { [SKILLLEARN_TYPE_PUBLIC] = "����" };
local t_MagicorSkillTypeList = { [SKILLLEARN_TYPE_PUBLIC] = "����" };
local t_GunerSkillTypeList	 = { [SKILLLEARN_TYPE_PUBLIC] = "����" };
local t_ChurchSkillTypeList	 = { [SKILLLEARN_TYPE_PUBLIC] = "����" };

local t_SelectSkillTypeList = {
						[g_nCAREER_NON] = {}, [g_nCAREER_WARRIOR] = t_WarriorSkillTypeList, [g_nCAREER_MAGICOR] = t_MagicorSkillTypeList,
						[g_nCAREER_GUNER] = t_GunerSkillTypeList, [g_nCAREER_CHURCH] = t_ChurchSkillTypeList
							  };

local nNpcId;

--@Desc:���NPC��ְҵ
--@return: NPCְҵnRace
function GetNpcRace()
	local mainplayer = ActorMgr:getMainPlayer();
	return mainplayer:getRace();
end
--@Desc��ѡ��NPC����Ӧ��ְҵ
--@return:����NPC��Ӧ�ļ��������б�
function GetNpcSkillTypeList()
	--��õ�ǰ��Ϸ�е����
	local tagPlayer = ActorMgr:getMainPlayer();
	local nRace		= tagPlayer:getRace();
	return t_SelectSkillTypeList[nRace % 0x10];
end

--�������ڱ����ѧϰ���ܵ��б��ȫ�����ܵ��б�
--t_TotalSkillList = {[��������1] = {��Ӧ���ڵ�ParentNode1}, [��������2] = {��Ӧ���ڵ�ParentNode2}, ... }
local t_TotalSkillList = {};

--@Desc:�����ڵ���GameSkillLearn:updateSkillLearnList()����Ӧ�ó����е��б�ͬ��
--@��������:ˢ���б��Ҫ��֤�б���ԭ����չ��״̬����
--@return: Nothing
function UpdateTotalList()
	--����ÿ�����ڵ㣬��ͬ���ű��еĸ��ڵ���Ӧ�ó����еĸ��ڵ�
	local tagParentNodeInfo;
	local tagParentNode;--����Ҫ����ĸ��ڵ�
	local tagChildNode;--������븸�ڵ���ӽڵ�
	local t_SkillTypeList = GetNpcSkillTypeList();
	--���ԭ���б��и������ڵ��չ��״̬
	local t_TempList = t_TotalSkillList;
	--���ԭ�����б�����
	t_TotalSkillList = {};
	local bFound = false;

	for nSkillType, szTypeName in pairs( t_SkillTypeList ) do
		bFound = false;
		tagParentNodeInfo = GameSkillLearn:getAllSkill( nSkillType );
		--����丸�ڵ��չ��״̬����Ϊ�µĽڵ���Ĭ��Ϊ��չ��״̬
		for i = 1, table.getn( t_TempList ) do
			if nSkillType == t_TempList[i][2] then
				tagParentNode = { szTypeName, nSkillType, t_TempList[i][3], {} };
				bFound = true;--��ʾҪ�������µĸ��ڵ���ԭ�����б��д���
				break;
			end
		end

		--��Ҫ�����ĸ��ڵ����µĽڵ�
		if not bFound then
			tagParentNode = { szTypeName, nSkillType, true, {} };
		end

		--�����ڵ��м������Ӧ���ӽڵ�
		local nSkillId		= 0;
		local nSkillLv		= 0;
		local tagSkillAttr	= {};
		local szChildName	= "";

		for i = 1, tagParentNodeInfo.m_nNum do
			nSkillId		= GameSkillLearn:getSkillInfo( nSkillType, i-1, true ).m_nSkillId;
			nSkillLv		= GameSkillLearn:getSkillInfo( nSkillType, i-1, true ).m_nLv;
			tagSkillAttr	= SkillMgr:getSpellStaticAtt( nSkillId, nSkillLv );
			tagChildNode	= { tagSkillAttr.Name, nSkillId, nSkillLv };

			table.insert( tagParentNode[4], tagChildNode );
		end
		--�����ڵ���뼼���б�
		table.insert( t_TotalSkillList, tagParentNode );
		SortTotalSkillList();
	end
end

function SortTotalSkillList()
	local mainplayer= ActorMgr:getMainPlayer();
	for _, raceSkillList in ipairs( t_TotalSkillList ) do
		local nInsertIndex = 1;
		local t_skillList = raceSkillList[4];
		for i = 1, table.getn( t_skillList ) do
			local skill		= t_skillList[i];
			local skillDef	= SkillMgr:getSpellStaticAtt( skill[2], skill[3] );
			if mainplayer:getLv() >= skillDef.Learn.LevelMin then
				local element = table.remove( t_skillList, i );
				table.insert( t_skillList, nInsertIndex, element );
				nInsertIndex = nInsertIndex + 1;
			end
		end
	end
end

local t_CanStudySkillList = {};

--@Desc:�����ڵ���GameSkillLearn:updateSkillLearnList()����Ӧ�ó����е��б�ͬ��
--@��������:ˢ���б��Ҫ��֤�б���ԭ����չ��״̬����
--@return: Nothing
function UpdateCanStudyList()
	--����ÿ�����ڵ㣬��ͬ���ű��еĸ��ڵ���Ӧ�ó����еĸ��ڵ�
	local tagParentNodeInfo;
	local tagParentNode;--����Ҫ����ĸ��ڵ�
	local tagChildNode;--������븸�ڵ���ӽڵ�
	local t_SkillTypeList = GetNpcSkillTypeList();
	--���ԭ���б��и������ڵ��չ��״̬
	local t_TempList = t_CanStudySkillList;
	--���ԭ�����б�����
	t_CanStudySkillList = {};
	local bFound = false;

	for nSkillType, szTypeName in pairs( t_SkillTypeList ) do
		bFound = false;
		tagParentNodeInfo = GameSkillLearn:getCanLearnSkill( nSkillType );
		--����丸�ڵ��չ��״̬����Ϊ�µĽڵ���Ĭ��Ϊ��չ��״̬
		for i = 1, table.getn( t_TempList ) do
			if nSkillType == t_TempList[i][2] then
				tagParentNode = { szTypeName, nSkillType, t_TempList[i][3], {} };
				bFound = true;--��ʾҪ�������µĸ��ڵ���ԭ�����б��д���
				break;
			end
		end

		--��Ҫ�����ĸ��ڵ����µĽڵ�
		if not bFound then
			tagParentNode = { szTypeName, nSkillType, true, {} };
		end

		--�����ڵ��м������Ӧ���ӽڵ�
		local nSkillId		= 0;
		local nSkillLv		= 0;
		local tagSkillAttr	= {};
		local szChildName	= "";

		for i = 1, tagParentNodeInfo.m_nNum do
			nSkillId		= GameSkillLearn:getSkillInfo( nSkillType, i-1, false ).m_nSkillId;
			nSkillLv		= GameSkillLearn:getSkillInfo( nSkillType, i-1, false ).m_nLv;

			tagSkillAttr	= SkillMgr:getSpellStaticAtt( nSkillId, nSkillLv );
			szChildName		= tagSkillAttr.Name;
			tagChildNode	= { szChildName, nSkillId, nSkillLv };

			table.insert( tagParentNode[4], tagChildNode );
		end
		--�����ڵ���뼼���б�
		table.insert( t_CanStudySkillList, tagParentNode );
	end
end

local t_NonStudySkillList = {};
--@Desc:�����ڵ���GameSkillLearn:updateSkillLearnList()����Ӧ�ó����е��б�ͬ��
--@��������:ˢ���б��Ҫ��֤�б���ԭ����չ��״̬����
--@return: Nothing
function UpdateNonStudyList()
	--����ÿ�����ڵ㣬��ͬ���ű��еĸ��ڵ���Ӧ�ó����еĸ��ڵ�
	local tagParentNodeInfo;
	local tagParentNode;--����Ҫ����ĸ��ڵ�
	local t_SkillTypeList = GetNpcSkillTypeList();
	--���ԭ���б��и������ڵ��չ��״̬
	local t_TempList = t_NonStudySkillList;
	--���ԭ�����б�����
	t_NonStudySkillList = {};
	local bFound = false;
	local mainplayer= ActorMgr:getMainPlayer();
	for nSkillType, szTypeName in pairs( t_SkillTypeList ) do
		bFound = false;
		tagParentNodeInfo = GameSkillLearn:getAllSkill( nSkillType );
		--����丸�ڵ��չ��״̬����Ϊ�µĽڵ���Ĭ��Ϊ��չ��״̬
		for i = 1, table.getn( t_TempList ) do
			if nSkillType == t_TempList[i][2] then
				tagParentNode = { szTypeName, nSkillType, t_TempList[i][3], {} };
				bFound = true;--��ʾҪ�������µĸ��ڵ���ԭ�����б��д���
				break;
			end
		end

		--��Ҫ�����ĸ��ڵ����µĽڵ�
		if not bFound then
			tagParentNode = { szTypeName, nSkillType, true, {} };
		end

		--�����ڵ��м������Ӧ���ӽڵ�
		local nSkillId		= 0;
		local nSkillLv		= 0;
		local tagSkillAttr	= {};
		for i = 1, tagParentNodeInfo.m_nNum do
			nSkillId		= GameSkillLearn:getSkillInfo( nSkillType, i-1, true ).m_nSkillId;
			nSkillLv		= GameSkillLearn:getSkillInfo( nSkillType, i-1, true ).m_nLv;
			local	skillDef= SkillMgr:getSpellStaticAtt( nSkillId, nSkillLv );
			if mainplayer:getLv() < skillDef.Learn.LevelMin then
				table.insert( tagParentNode[4], { skillDef.Name, nSkillId, nSkillLv } );
			end
		end
		--�����ڵ���뼼���б�
		table.insert( t_NonStudySkillList, tagParentNode );
	end
end

--@Desc:��Ϊ���ڻ�ñ��ʵ����Ч����
--return:��ü����б��п���ʾ�ڵ���������
local function GetListRowNum( t_SkillList  )
	local nRowNum = 0;
	for i = 1, table.getn( t_SkillList ) do
		nRowNum = nRowNum + 1;
		--��⸸�ڵ��Ƿ����չ��
		if t_SkillList[i][3] then
			local t_ChildInfoList = t_SkillList[i][4];
			--������ǰ���ڵ�����б�
			for j = 1, table.getn( t_ChildInfoList ) do
				nRowNum = nRowNum + 1;
			end
		end
	end

	return nRowNum;
end

--���ڱ���ģ���п�ʼ��ʾ�Ľڵ�ID
--local nStartDisplayPoint = 0;

--@Desc:����ע�Ἴ��ѧϰ������Ϣ
--@return: Nothing
function NpcLearnSkillFrame_OnLoad()
	this:RegisterEvent("GE_SKILLLEARN_OPEN");
end

--@Desc:������Ӧ�����¼���Ϣ
--@return: Nothing
function NpcLearnSkillFrame_OnEvent()
	--MessageBox( "��ʾ", "NpcLearnSkillFrame_OnEvent" );
	if arg1 == "GE_SKILLLEARN_OPEN" then
		local Frame= getglobal( "NpcLearnSkillFrame" );
		Frame:Show();
	end
end

--@Desc:�����ڴ򿪼�������ʱ���ȡNPC��ID
--@return: Nothing
function openSkillLearn( NpcId )
	nNpcId = NpcId;
	GameSkillLearn:updateSkillLearnList();
	if not NpcLearnSkillFrame:IsShown() then
		NpcLearnSkillFrame:Show();
		util.testNpcDialogDistance(NpcId, NpcLearnSkillFrame)
	end
end

--@Desc:���������������Ϣ,��ʾ��ǰѧϰ�ɹ�����nSkillId��nSkillLv��ʶ�ļ���
--@Param: nSkillId  ��Ϊ��ǰѧϰ�ɹ��ļ���ID
--@Param: nSkillLv  ��Ϊ��ǰѧϰ�ɹ��ļ���Lv
function SkillLearn( nSkillId, nSkillLv )
	if NpcLearnSkillFrame:IsShown() and nSkillLv > 1 then
		if not GameWizard:isDo( GAMEWIZARD_FIRST_SEE_STUDY_SKILL_NPC ) then
			GameWizard:doStep( GAMEWIZARD_FIRST_SEE_STUDY_SKILL_NPC );
		end

		if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_NPCTALK_STUDY_SKILL_OPTION ) then
			GameWizard:doStep( GAMEWIZARD_FIRST_BLINK_NPCTALK_STUDY_SKILL_OPTION );
		end	

		if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_NPCLEARNSKILL_STUDY_BTN ) then
			GameWizard:doStep( GAMEWIZARD_FIRST_BLINK_NPCLEARNSKILL_STUDY_BTN );
		end	
	end

	local t_CurDisplayList = {};

	if SKILLPAGE_TOTAL == nCurSkillPage then
		t_CurDisplayList = t_TotalSkillList;
	elseif SKILLPAGE_CANSTUDY == nCurSkillPage then
		t_CurDisplayList = t_CanStudySkillList;
	elseif SKILLPAGE_NONSTUDY == nCurSkillPage then
		t_CurDisplayList = t_NonStudySkillList;
	end

	--ˢ�¼���ѧϰ���		FixHighLightTex()
	GameSkillLearn:updateSkillLearnList();
	SkillLearnClientData( nSkillId, nSkillLv );
	AcceclKeySkillLearn( nSkillId );	
	if nSkillLv == 1 or SKILL_COMMON[nSkillId] then
		InitSkillData();
	end

	if not NpcLearnSkillFrame:IsShown() then
		return;
	end
	--����Ҫ����ˢ��"ȫ��"��"��ѧϰ"�����б��е�����
	UpdateTotalList();
	UpdateCanStudyList();
	UpdateNonStudyList();
	local slider = getglobal( "NLS_ScrollBar" );
	SetLearnSkillScrollBar( t_CurDisplayList );
	if slider:IsShown() then
		UpdateSkillPage( slider:GetValue() + 1 ); -- �մ�ҳ���ʱ��Ĭ�ϴӼ����б��һ�п�ʼ��ʾ
	else
		UpdateSkillPage( 1 );
	end
	ResetHighlightAndSkillInfo();
end

--@Desc:��Ϊ������ʾ���ܵ���ϸ��Ϣ
--@Param: nSkillId ��ΪҪ��ʾ�ļ��ܵ�ID
--@Param: nSkillLv ��ΪҪ��ʾ�ļ��ܵ�Lv
--@return: Nothing
function NlsFrame_ShowSkillInfo( nSkillId, nSkillLv )
	local skillDef		= SkillMgr:getSpellStaticAtt( nSkillId, nSkillLv );
	local mainplayer	= ActorMgr:getMainPlayer();
	local nRace			= GetNpcRace();
	local nLevel			= SkillMgr:isSkillLearned(nSkillId) and SkillMgr:getSpellLv(nSkillId) or 0;

	-- �ر���,�Ժ����FontString
	local t_riches = { ["NlsTechnologyDescText"] =	{	value = "       " .. getSkillInfo( skillDef, skillDef.SkillDesc ),
														color = { r = 255, g = 255, b = 190 },
													},
					};
	for name, data in pairs( t_riches ) do
		local rich = getglobal( name );
		rich:SetText( data.value, data.color.r, data.color.g, data.color.b );
		rich:Show();
	end
	
	local diathesisDef;
	if skillDef.Learn.PreDiathesisID ~= 0 then
		diathesisDef =  getDiathesisDef( skillDef.Learn.PreDiathesisID, skillDef.Learn.PreDiathesisLevel );
	end

	local t_skillFonts =	{ 
								["NlsFrameTechnologyNameFont"] =
								{
									val = skillDef.Name
								},

								["NlsFrameTechnologyLevelFont"] =
								{
									val = "���ȼ�  " .. nSkillLv .. "��"
								},
								
								["NlsFramePreTianFuValueFont"] = 
								{
									val = ( ( diathesisDef == nil ) and "��" or ( diathesisDef.Name.." "..diathesisDef.Level.."��" ) ),					color = { r = 255 , g = 255, b = 190 }	
								},

								["NlsFramePreTechNameValueFont"] = {
									val =  ( nSkillLv > 1 and ( skillDef.Name.."���ȼ�" .. ( nSkillLv - 1 ) .. "��" ) or "��" ) , 
									color = {  r = 255 , g = 255, b = 190 }
								},
								
								-- ��ͽ�Ǯ��
								["NlsMinMoneyValueFont"] = 
								{
									val = skillDef.Learn.MoneyVal > 0 and ( skillDef.Learn.MoneyVal .. ""..MONEY_TYPE_BULL_NAME.."" ) or "��", color = { r = 255 , g = 255, b = 190 }	
								},
								
								-- �������ȼ�
								["NlsFrameMinLevelValueFont"] = 
								{
									val = skillDef.Learn.LevelMin > 0 and ( skillDef.Learn.LevelMin .. "��" ) or "��", color = { r = 255 , g = 255, b = 190 }	
								},
								
								-- ��ǰ��Ǯ����
								["NlsCurrentMoneyValueFont"] = 
								{
									val = getPlayerMoney( mainplayer ), color = { r = 209 , g = 185, b = 102 }	
								},
								
								["NLS_SkillDesc_UseSkillPointFont"] = 
								{
									val = skillDef.MPUse > 0 and ( skillDef.MPUse.."��ħ��ֵ" ) or "��", color = { r = 255 , g = 255, b = 190 }	
								},
								
								["NLS_SkillDesc_LengQueFont"] = 
								{
									val = skillDef.SelfCool > 0 and ( skillDef.SelfCool / 10 .. "��") or "��", color = { r = 255 , g = 255, b = 190 }	
								},
								
								["NLS_SkillDesc_ShiFangDistFont"] = 
								{
									val = skillDef.Distance > 0 and ( skillDef.Distance / 100 .. "��") or "��", color = { r = 255 , g = 255, b = 190 }	
								},
								
								["NLS_SkillDesc_YinChangTimeFont"] = 
								{
									val = skillDef.PrepareTime > 0 and ( skillDef.PrepareTime / 10 .. "��" ) or "��", color = { r = 255 , g = 255, b = 190 }	
								},
							};
	for szFontName, data in pairs( t_skillFonts ) do
		local font = getglobal( szFontName );
		if data.val then
			font:SetText( data.val );
		end
		if data.color then
			font:SetTextColor( data.color.r, data.color.g, data.color.b );
		end
		font:Show();
	end
	
	-- TODO
	local nMoneyVal		= getPlayerMoney( mainplayer, skillDef.Learn.MoneyAttrID )
	local diathesisMgr	= mainplayer:getDiathesisManager();

	local t_failFonts = {
							["NlsFrameMinLevelValueFont"] = { color = ( ( mainplayer:getLv() < skillDef.Learn.LevelMin ) and 
																( { r = 250, g = 60, b = 60 } ) or 
																( { r = 255 , g = 255, b = 190 } ) ) },
							
							["NlsMinMoneyValueFont"] = { color = ( ( nMoneyVal < skillDef.Learn.MoneyVal ) and 
																	( { r = 250, g = 60, b = 60 } ) or 
																	( { r = 255 , g = 255, b = 190 } ) ) },
							
							["NlsFramePreTechNameValueFont"] = { color = ( nLevel < nSkillLv - 1 and 
																	{ r = 250, g = 60, b = 60 } or 
																	{ r = 255 , g = 255, b = 190 } ) },

							["NlsFramePreTianFuValueFont"] = 
							{
								color = ( diathesisDef ~= nil and not diathesisMgr:isLearnedGenius( diathesisDef.DiathesisID, 
											diathesisDef.Level ) ) and ( { r = 250, g = 60, b = 60 } ) or ( { r = 255 , g = 255, b = 190 } )
							},
						};

	
	for name, data in pairs( t_failFonts ) do
		local font = getglobal( name );
		font:SetTextColor( data.color.r, data.color.g, data.color.b );
	end

	local itemButton = getglobal("NpcLearnSkillItemBtn");

	--�л�����ʾ����ͼ��
	local skillTex	= getglobal("NpcLearnSkillItemBtnIcon");--���ڱ��漼�ܶ�Ӧ��ͼ��

	local IconPath = GetSkillIconPath()..skillDef.IconID1..".tga";
	-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
	if not IsInExistence( IconPath ) then
		IconPath = DEFAULT_ITEM_PATH;
	end
	skillTex:SetTexture( IconPath );

	itemButton:Show();
	NLS_SkillInfoBtn:Show();
end

--@Desc:��Ϊ���ؼ��������ϸ��Ϣ
--@return: Nothing
function HideSkillInfo()
	-- �ر���,�Ժ����FontString
	NlsTechnologyDescText:Hide();
	NlsTechnologyDescText:Clear();

	-- ���ؼ���ͼ��
	NpcLearnSkillItemBtn:Hide();
	NLS_SkillInfoBtn:Hide();
end

--@Desc:���������ԭ���ĸ�����ʾ
function ClearHighLightTex( nMaxViewLine )
	for i = 1, nMaxViewLine do
		local tex = getglobal("LearnSkillFrameLearnSkillInfo"..i.."LearnSkillRichSelectHighLightTex");
		tex:Hide();
	end
end

--@Desc:�̶���������λ��
function FixHighLightTex( highLightTex, relativeWidgetName )
	ClearHighLightTex( MAX_VIEW_LINE );

	highLightTex:Show();
end

--@Desc:���ù�������λ��
function SetLearnSkillScrollBar( t_SkillList )
	--ȷ������ѧϰ�����ʾ�ľ��弼��
	local slider		= getglobal( "NLS_ScrollBar" );
	local nTableNum		= GetListRowNum( t_SkillList );--�˵�ǰ���п���ʾ�����ݵ�����
	local nNum			= nTableNum - MAX_VIEW_LINE;
	local nMaxValue		= slider:GetMaxValue( );

	if nNum > 0  then
		slider:SetMaxValue( nNum );
		slider:Show();
	else
		slider:SetMaxValue( 1 );
		slider:Hide();
	end

	slider:SetMinValue( 0 );
	slider:SetValueStep( 1 );

	-- ��ѧϰ�˼����Ժ�,Ҫ�ر����õ�,�Է�ֹ
	if slider:GetValue() > slider:GetMaxValue( ) then
		slider:SetValue( nMaxValue );
	end
end

local bRecond	= false;
local nLastTime = 0;
function NpcLearnSkillFrame_OnUpdate()
	if bRecond then
		nLastTime = os.clock();
		bRecond = false;
	end

	if os.clock() - nLastTime > 0.3 then
		bRecond = true;
	end
end

--t_TotalSkillList = {[��������1] = {��Ӧ���ڵ�ParentNode1}, [��������2] = {��Ӧ���ڵ�ParentNode2}, ... }
--local t_ChildNode  = { �ӽڵ�����,����ID, ����Ҫ�����͵ȼ�};
--local t_ParentNode = {  ���ڵ�����, ��������, ָʾ���ڵ��Ƿ�չ����״̬��־, {�ӽڵ㼯���б�}};
--@Desc:�ڼ�����嵯������ʾ��ʱ����Ӧ
--@return: Nothing
function NpcLearnSkillFrame_OnShow()
	-- ����ָ����Ч��ʾ
	local MainPlayer = ActorMgr:getMainPlayer();
	local nLevel = MainPlayer:getLv();
	local nRunQuestNum = Quest:getNumQusetRun();
	local bFound = false;
	for i = 1, nRunQuestNum do
		if Quest:getQuestRunId(i-1) == 1058 then
			bFound = true;
		end
	end
	if GameWizard:isDo( GAMEWIZARD_FIRST_LEARNSKILL) == false and  bFound then
		GameWizard:doStep( GAMEWIZARD_FIRST_LEARNSKILL);
		GuideEffectShow( "LearnSkillFrameLearnSkillInfo1", "", "LearnSkillFrameLearnSkillInfo1", "������\n�ɲ鿴������Ϣ", "uires\\ui\\mask\\TouMingTongDao.tga", 1, 1, 1, 1, 67, 67, 0, 0, -65, 24, false );
		GuideEffectShow( "NpcLearnSkillFrame_StudyBtn", "","NpcLearnSkillFrame_StudyBtn", "�����ť\nѧϰ�¼���", "uires\\ZhuJieMian\\1.dds", 352, 609, 60, 60, 128, 48, -32, 12, 46, -35, false );
	else
		-- �ر�����ָ����Ч��ʾ
		LearnSkillFrameLearnSkillInfo1EffectFrame:Hide();
	end

	HideSkillInfo();
	--����ʾ�����ʱ���������б����ֵ����ֹ��"ѧϰ"��ť����崻�
	local frame	= getglobal( "NpcLearnSkillFrame" );
	t_CurSelectSkill.skillId = 0;
	t_CurSelectSkill.skillLv = 0;

	nCurSkillPage = SKILLPAGE_TOTAL;

	UpdateTotalList();
	UpdateCanStudyList();
	UpdateNonStudyList();
	local t_SkillList = {};

	--���Ҫ��ʾ��ҳ��
	if SKILLPAGE_TOTAL == nCurSkillPage then
		t_SkillList = t_TotalSkillList;
		NlsFrame_OneLayerName:SetText( "ȫ��" );
	elseif SKILLPAGE_CANSTUDY == nCurSkillPage then -- SKILLPAGE_CANSTUDY
		t_SkillList = t_CanStudySkillList;
		NlsFrame_OneLayerName:SetText( "��ѧϰ" );
	elseif SKILLPAGE_NONSTUDY == nCurSkillPage then
		t_SkillList = t_NonStudySkillList;
		NlsFrame_OneLayerName:SetText( "����ѧϰ" );
	end

	local slider		= getglobal( "NLS_ScrollBar" );
	local itemButton	= getglobal( "NpcLearnSkillItemBtn" );
	itemButton:Hide();
	slider:SetValue( 0 );
	SetLearnSkillScrollBar( t_SkillList );
	if slider:IsShown() then
		UpdateSkillPage( slider:GetValue() + 1 ); -- �մ�ҳ���ʱ��Ĭ�ϴӼ����б��һ�п�ʼ��ʾ
	else
		UpdateSkillPage( 1 );
	end
	
	local nCurTableNum = 0;
	for i = 1, table.getn( t_SkillList ) do
		nCurTableNum = nCurTableNum + 1;
		-- �����Ӽ�
		if t_SkillList[i][3] then
			local t_ChildInfoList = t_SkillList[i][4];
			if table.getn( t_ChildInfoList ) > 0 then
				break;
			end
		end
	end
	
	local rich	= getglobal( "LearnSkillFrameLearnSkillInfo2LearnSkillRich" );
	t_CurSelectSkill.nLineNumOnUI	= 2;
	t_CurSelectSkill.skillId		= rich:GetClientUserData( 0 );
	t_CurSelectSkill.skillLv		= rich:GetClientUserData( 1 );
	NlsFrame_ShowSkillInfo( t_CurSelectSkill.skillId, t_CurSelectSkill.skillLv );
	
	local highLightTex	= getglobal( "LearnSkillFrameLearnSkillInfo2LearnSkillRichSelectHighLightTex" );

	--[[
	if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_NPCTALK_STUDY_SKILL_OPTION ) then
		GameWizard:doStep( GAMEWIZARD_FIRST_BLINK_NPCTALK_STUDY_SKILL_OPTION );
	end	
	--]]
	if LearnSkillFrameLearnSkillInfo2LearnSkillRich:GetText() == "" then     --elmer
		local richTex		= getglobal( "LearnSkillFrameLearnSkillInfo1LearnSkillRich" );
		local highLightRich	= getglobal( "LearnSkillFrameLearnSkillInfo1LearnSkillRichSelectHighLightTex" );
		FixHighLightTex( highLightRich, richTex:GetName() );
		NLS_SkillInfoBtn:Hide();
		NpcLearnSkillFrame:SetSize( 391,493 );
	else
		FixHighLightTex( highLightTex, rich:GetName() );
		this:SetPoint( "center","UIClient","left",420,0 )
	end
end

function NpcLearnSkillFrame_OnHide()
	for i = 1, MAX_VIEW_LINE do
		local highLightTex	= getglobal( "LearnSkillFrameLearnSkillInfo" .. i .. "LearnSkillRichSelectHighLightTex" );
		highLightTex:Hide();
	end
	NLS_SkillInfoBtn:Hide();
end

--@Desc:��������������ϼ�ͷʱ��Ӧ
--@return: Nothing
function NLS_ScrollUpBtn_OnClick()
	local slider = getglobal("NLS_ScrollBar");
	if not slider:IsShown() then
		return;
	end

	if NlsFrame_SkillOptBtn:IsShown() then
		NlsFrame_SkillOptBtn:Hide();
	end

	local nValue = slider:GetValue() - slider:GetValueStep();

	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end

--@Desc:��������������¼�ͷʱ��Ӧ
--@return: Nothing
function NLS_ScrollDownBtn_OnClick()
	local slider = getglobal( "NLS_ScrollBar" );
	if not slider:IsShown() then
		return;
	end

	if NlsFrame_SkillOptBtn:IsShown() then
		NlsFrame_SkillOptBtn:Hide();
	end

	local nMaxValue = slider:GetMaxValue();
	local nValue	= slider:GetValue() + slider:GetValueStep();

	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

--@Desc:��Ϊ�ƶ�������ʱ������ˢ���ı�
--@return: Nothing
function NLS_ScrollBar_OnValueChanged()
	if NlsFrame_SkillOptBtn:IsShown() then
		NlsFrame_SkillOptBtn:Hide();
	end

	local slider	= getglobal( "NLS_ScrollBar" );
	local nValue	= slider:GetValue();
	local nMaxValue = slider:GetMaxValue();

	if nValue <= nMaxValue then
		UpdateSkillPage( slider:GetValue() + 1 );
	end

	local studyButton = getglobal( "NpcLearnSkillFrame_StudyBtn" );
	local nLineNum    = t_CurSelectSkill.nLineNumOnUI;


	if 0 == nLineNum then return end--��û���κ��ӽڵ㱻ѡ��

	local nSkillId	  = t_CurSelectSkill.skillId;
	local nSkillLv	  = t_CurSelectSkill.skillLv;
	local oldHighLightTex =  getglobal( "LearnSkillFrameLearnSkillInfo" .. nLineNum .. "LearnSkillRichSelectHighLightTex");
	local button;

	oldHighLightTex:Hide();--����ԭ���ĸ�����

	for i = 1, MAX_VIEW_LINE do --����ÿһ���ҵ�
		button = getglobal( "LearnSkillFrameLearnSkillInfo"..i );

		if nSkillId == button:GetClientUserData( 0 ) and nSkillLv == button:GetClientUserData( 1 ) then
			t_CurSelectSkill.nLineNumOnUI = i;
			local newHighLightTex  =  getglobal( "LearnSkillFrameLearnSkillInfo" .. i .. "LearnSkillRichSelectHighLightTex");
			local szRelativeWidget = "LearnSkillFrameLearnSkillInfo"..i.."LearnSkillRich";
			FixHighLightTex( newHighLightTex, szRelativeWidget );
			return;
		end
	end
end

function Help_LearnSkillFrame()
	if NlsFrame_SkillOptBtn:IsShown() then
		NlsFrame_SkillOptBtn:Hide();
	end
end

function Recommend_LearnSkillFrame()
	if NlsFrame_SkillOptBtn:IsShown() then
		NlsFrame_SkillOptBtn:Hide();
	end
end

--@Desc:��Ϊѧϰ�ɹ����ܺ����ø������ͼ�����ϸ��Ϣ
function ResetHighlightAndSkillInfo()
	local t_CurDisplayList = {};

	if SKILLPAGE_TOTAL == nCurSkillPage then
		t_CurDisplayList = t_TotalSkillList;
	elseif SKILLPAGE_CANSTUDY == nCurSkillPage then
		t_CurDisplayList = t_CanStudySkillList;
	elseif SKILLPAGE_NONSTUDY == nCurSkillPage then
		t_CurDisplayList = t_NonStudySkillList;
	end

	--ѧϰ�ɹ�����һЩ���¶���

	--���ø�����

	--��õ�ǰ���ܱ��е������Ƿ��ԭ��ָ������nLineNum��
	local szRelativeName		= "";
	local szHighLightTexName	= "";
	local nCurTableNum			= 0;
	local nLineNum				= t_CurSelectSkill.nLineNumOnUI;

	for i = 1, table.getn( t_CurDisplayList ) do
		nCurTableNum = nCurTableNum + 1;

		--�����Ӽ�
		if t_CurDisplayList[i][3] then
			local t_ChildInfoList = t_CurDisplayList[i][4];

			for j = 1, table.getn( t_ChildInfoList ) do
				nCurTableNum = nCurTableNum + 1;
			end
		end
	end

	if nCurTableNum < nLineNum then
		nLineNum = nCurTableNum;
	end

	t_CurSelectSkill.nLineNumOnUI = nLineNum;
	local highLightTex	= getglobal( "LearnSkillFrameLearnSkillInfo"..nLineNum.."LearnSkillRichSelectHighLightTex" );
	richName = "LearnSkillFrameLearnSkillInfo"..nLineNum.."LearnSkillRich";
	FixHighLightTex( highLightTex, richName );

	--����ѧϰ����ܼ���ѧ��һ������
	local nextButton = getglobal( "LearnSkillFrameLearnSkillInfo"..nLineNum );

	if nextButton:GetClientUserData( 2 ) == PARENT_NODE_LENGTH then -- ����ǰ��λ�����Ľڵ��Ǹ��ڵ�
		t_CurSelectSkill.skillId	= 0;
		t_CurSelectSkill.skillLv	= 0;
		t_CurSelectSkill.skillType	= nextButton:GetClientUserData( 0 );

		HideSkillInfo();
	else -- ����ǰ��λ�����Ľڵ��Ƕ��ӽڵ�
		t_CurSelectSkill.skillId	= nextButton:GetClientUserData( 0 );
		t_CurSelectSkill.skillLv	= nextButton:GetClientUserData( 1 );
		NlsFrame_ShowSkillInfo( t_CurSelectSkill.skillId, t_CurSelectSkill.skillLv );
	end
end

--@Desc:ѧϰ����
--@return: Nothing
function NpcLearnSkill_Study_OnClick()
	if CostMoneyConfirmFrame:IsShown() then
		return;
	end

	local t_hideIcons = { "NLS_SkillInfoBtnXinShouBtn", "NpcLearnSkillFrame_StudyBtnUVAnimationTex" };
	util.HideIcons( t_hideIcons );

	-- �ر�����ָ����Ч��ʾ
	if NlsFrame_SkillOptBtn:IsShown() then
		NlsFrame_SkillOptBtn:Hide();
	end

	--���ȼ�⼼���Ƿ���ѧϰ
	local frame			= getglobal( "NpcLearnSkillFrame" );
	local nSkillId		= t_CurSelectSkill.skillId;
	local nSkillLv		= t_CurSelectSkill.skillLv;
	local nSkillType	= t_CurSelectSkill.skillType;

	if 0 == nSkillId or 0 == nSkillLv then
		return;
	end

	local nSkillResult = GameSkillLearn:checkSkillLearn( nSkillId, nSkillLv );
	if SKILLLEARN_SUCCED == nSkillResult then
		local skillDef = SkillMgr:getSpellStaticAtt( nSkillId, nSkillLv );
		if nil == SkillMgr:getSkillDyAttrib( nSkillId  ) then
			--����ѧϰ����
			if checkNeedWarnCost( skillDef.Learn.MoneyVal ) then
				CostMoneyConfirmFrame:SetClientString("����ѧϰ");
				CostMoneyConfirmFrame:SetClientUserData( 0, nNpcId );
				CostMoneyConfirmFrame:SetClientUserData( 1, nSkillId );
				CostMoneyConfirmFrame:SetClientUserData( 2, nSkillLv );
				SetWarnFrame( "NpcLearnSkillFrame" );
				CostMoneyConfirmFrame:Show();
				return;
			end
			GameSkillLearn:skillLearn( nNpcId, nSkillId, nSkillLv );
		else
			if checkNeedWarnCost( skillDef.Learn.MoneyVal ) then
				CostMoneyConfirmFrame:SetClientString("��������");
				CostMoneyConfirmFrame:SetClientUserData( 0, nNpcId );
				CostMoneyConfirmFrame:SetClientUserData( 1, nSkillId );
				CostMoneyConfirmFrame:SetClientUserData( 2, nSkillLv );
				SetWarnFrame( "NpcLearnSkillFrame" );
				CostMoneyConfirmFrame:Show();
				return;
			end
			GameSkillLearn:skillLvUp( nNpcId, nSkillId, nSkillLv );
		end
	--������ѧϰ,�������ɲ���ѧϰ��������ʾ
	else
		if nSkillResult == SKILLLEARN_FAIL_CAREER then
			ShowMidTips( "ְҵ������" );
		elseif nSkillResult == SKILLLEARN_FAIL_LVMIN then
			ShowMidTips( "�ȼ�����" );
		elseif nSkillResult == SKILLLEARN_FAIL_NONEED then
			ShowMidTips( "�ü����Զ����գ�����Ҫѧϰ" );
		elseif nSkillResult == SKILLLEARN_FAIL_SKILLPOINT then
			ShowMidTips( "���ܵ㲻��" );
		elseif nSkillResult == SKILLLEARN_FAIL_MONEY then
			ShowMidTips( "��Ǯ����" );
		elseif nSkillResult == SKILLLEARN_FAIL_QUEST then
			ShowMidTips( "���񲻷���" );
		elseif nSkillResult == SKILLlEARN_FAIL_SKILLLV then
			ShowMidTips( "����ѧϰ�ȼ�̫��" );
		elseif nSkillResult == SKILLlEARN_FAIL_EXP then
			ShowMidTips( "���鲻��" );
		end
	end
end

--@Desc:��Ϊ������ڵ��ʱ����Ӧ��Ӧ����������
--@return: ���ص�����ڵ�ǰ�����ڵ��չ��״̬
local function ParentNode_OnClick()
	local nSkillType	= this:GetClientUserData( 0 );
	local frame			= getglobal( "NpcLearnSkillFrame" );
	local bPreUnwrap	= false;--��ȡ���ǰ���ڵ��չ��״̬
	t_CurSelectSkill.skillType	= nSkillType;

	local t_CurDisplayList = {};

	if SKILLPAGE_TOTAL == nCurSkillPage then
		t_CurDisplayList = t_TotalSkillList;
	elseif SKILLPAGE_CANSTUDY == nCurSkillPage then
		t_CurDisplayList = t_CanStudySkillList;
	elseif SKILLPAGE_NONSTUDY == nCurSkillPage then
		t_CurDisplayList = t_NonStudySkillList;
	end

	--�ҳ���Ӧ�ĸ��ڵ㣬���ı���״̬
	local slider = getglobal( "NLS_ScrollBar" );

	for i = 1, table.getn( t_CurDisplayList ) do
		if nSkillType == t_CurDisplayList[i][2] then
			bPreUnwrap = t_CurDisplayList[i][3];--��ȡ��ǰ���ڵ���չ����������״̬�������ڻָ���ǰ����ĸ���������
			t_CurDisplayList[i][3] = not t_CurDisplayList[i][3];
			--�����˵�ǰ���ڵ��չ��״̬��Ҫˢ���б�
			SetLearnSkillScrollBar( t_CurDisplayList );
			if slider:IsShown() then
				UpdateSkillPage( slider:GetValue() + 1 ); -- �մ�ҳ���ʱ��Ĭ�ϴӼ����б��һ�п�ʼ��ʾ
			else
				UpdateSkillPage( 1 );
			end
			return bPreUnwrap;
		end
	end
end

--@Desc:��Ϊ����ӽڵ��ʱ����Ӧ
--@return: Nothing
local function ChildNode_OnClick()
	local nSkillId	= this:GetClientUserData( 0 );
	local nSkillLv	= this:GetClientUserData( 1 );
	--��ѧϰ��ť�аѱ��浱ǰ�����ӽڵ��Ӧ�ؼ���λ��

	local studyButton	= getglobal( "NpcLearnSkillFrame_StudyBtn" );
	t_CurSelectSkill.nLineNumOnUI	= this:GetClientID();
	t_CurSelectSkill.skillId		= nSkillId;
	t_CurSelectSkill.skillLv		= nSkillLv;

	local frame		= getglobal( "NpcLearnSkillFrame" );

	--����ӽڵ��ʱ������ʾ�򱣴漼�ܵ�ID����͵ȼ�
	t_CurSelectSkill.skillId	= nSkillId;
	t_CurSelectSkill.skillLv	= nSkillLv;

	NlsFrame_ShowSkillInfo( nSkillId, nSkillLv );

	local studyButton	= getglobal( "NpcLearnSkillFrame_StudyBtn" );
	studyButton:Show();

	local skillTex		= getglobal("NpcLearnSkillItemBtnIcon");--���ڱ��漼�ܶ�Ӧ��ͼ��
	local itemButton	= getglobal("NpcLearnSkillItemBtn");
	local skill_attr	= SkillMgr:getSkillDyAttrib( nSkillId );

	if ( skill_attr ~= nill ) then
		local skillDef		= SkillMgr:getSpellStaticAtt( nSkillId, nSkillLv );
		local IconPath = GetSkillIconPath()..skillDef.IconID1..".tga";
		-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH;
		end
		skillTex:SetTexture( IconPath );
	end

	skillTex:Show();
	itemButton:Show();
end

--@Desc:��Ϊ����������ı���ʱ���Զ���Ӧ
--@return: Nothing
function NpcLearnSkillRich_OnClick()
	-- �ر�����ָ����Ч��ʾ
	if LearnSkillFrameLearnSkillInfo1EffectFrame:IsShown() then
		LearnSkillFrameLearnSkillInfo1EffectFrame:Hide();
	end

	if NlsFrame_SkillOptBtn:IsShown() then
		NlsFrame_SkillOptBtn:Hide();
	end

	-- ��ȷ�ϵ���İ�ť���Ǹ��ؼ�����ռ䲻�����κμ���ID��LV
	if this:GetClientUserData( 2 ) ~= PARENT_NODE_LENGTH and this:GetClientUserData( 0 ) == 0 and
		this:GetClientUserData( 1 ) == 0 then
		return;
	end

	local nNodeLength	= this:GetClientUserData( 2 );
	local bParent		= false;	-- ��鱻������Ƿ��Ǹ��ڵ�
	local bPreUnwrap	= true;		-- �ڵ��ǰ�Ƿ�Ϊչ��״̬,Ĭ��

	if PARENT_NODE_LENGTH == nNodeLength then
		bPreUnwrap  = ParentNode_OnClick();--��ø��ڵ���ǰ��չ��״̬
		bParent		= true;
	else
		ChildNode_OnClick();
	end

	local nLineNum	= this:GetClientID();
	local highLightTex	= getglobal( "LearnSkillFrameLearnSkillInfo"..nLineNum.."LearnSkillRichSelectHighLightTex" );

	FixHighLightTex( highLightTex, this:GetName() );

	--������ĸ��ڵ㣬����Ƿ�Ҫ�ָ���ǰ������ӽڵ�ĸ�����
	if bParent then
		local frame			= getglobal( "NpcLearnSkillFrame" );
		local nSkillId		= t_CurSelectSkill.skillId;
		local nSkillLv		= t_CurSelectSkill.skillLv;

		--�����ڵ���������״̬��Ϊչ��״̬
		if not bPreUnwrap then

			if 0 ~= nSkillId and 0 ~= nSkillLv then--��ԭ���Ѿ����ӽڵ㱻�����

				local studyButton	= getglobal( "NpcLearnSkillFrame_StudyBtn" );
				local nLineNum		= t_CurSelectSkill.nLineNumOnUI;
				local highLightTex  = getglobal( "LearnSkillFrameLearnSkillInfo" .. nLineNum .. "LearnSkillRichSelectHighLightTex" );
				local richName		= "LearnSkillFrameLearnSkillInfo"..nLineNum.."LearnSkillRich";

				highLightTex:Show();
				-- ͬʱ���ظ��ڵ��ѡ�и�����
				local parentHighlightTex = getglobal( "LearnSkillFrameLearnSkillInfo" .. this:GetClientID() .. "LearnSkillRichSelectHighLightTex" );
				parentHighlightTex:Hide();
			end
		end
	end
end

--@Desc:��������������ؼ�ʱ����Ӧ,�л��ؼ�����ʾ״̬
--@return: Nothing
function LearnSkillFrameTitleSignChk_OnClick()
	if NlsFrame_SkillOptBtn:IsShown() then
		NlsFrame_SkillOptBtn:Hide();
	end

	if this:GetClientUserData( 2 ) ~= PARENT_NODE_LENGTH and this:GetClientUserData( 0 ) == 0 and
		this:GetClientUserData( 1 ) == 0 then
		return;
	end

	local nNodeLength	= this:GetClientUserData( 2 );
	local bParent		= false;	-- ��鱻������Ƿ��Ǹ��ڵ�
	local bPreUnwrap	= true;		-- �ڵ��ǰ�Ƿ�Ϊչ��״̬,Ĭ��

	if PARENT_NODE_LENGTH == nNodeLength then
		bPreUnwrap  = ParentNode_OnClick();--��ø��ڵ���ǰ��չ��״̬
		bParent		= true;
	else
		ChildNode_OnClick();
	end

	local parentButton	= getglobal( this:GetParent( ) );
	local nLineNum		= parentButton:GetClientID();
	local highLightTex	= getglobal( "LearnSkillFrameLearnSkillInfo"..nLineNum.."LearnSkillRichSelectHighLightTex" );

	FixHighLightTex( highLightTex, this:GetParent() );

	--������ĸ��ڵ㣬����Ƿ�Ҫ�ָ���ǰ������ӽڵ�ĸ�����
	if bParent then
		local frame			= getglobal( "NpcLearnSkillFrame" );
		local nSkillId		= t_CurSelectSkill.skillId;
		local nSkillLv		= t_CurSelectSkill.skillLv;

		--�����ڵ���������״̬��Ϊչ��״̬
		if not bPreUnwrap then

			if 0 ~= nSkillId and 0 ~= nSkillLv then--��ԭ���Ѿ����ӽڵ㱻�����

				local studyButton	= getglobal( "NpcLearnSkillFrame_StudyBtn" );
				local nLineNum		= t_CurSelectSkill.nLineNumOnUI;
				local highLightTex  = getglobal( "LearnSkillFrameLearnSkillInfo" .. nLineNum .. "LearnSkillRichSelectHighLightTex" );
				local richName		= "LearnSkillFrameLearnSkillInfo"..nLineNum.."LearnSkillRich";

				highLightTex:Show();
				-- ͬʱ���ظ��ڵ��ѡ�и�����  Elmer
				local parentHighlightTex = getglobal( "LearnSkillFrameLearnSkillInfo"..parentButton:GetClientID().."LearnSkillRichSelectHighLightTex");
				parentHighlightTex:Hide();
			end
		end
	end
end

--@Desc:��Ϊ�����뵽���ı���ʱ���Զ���Ӧ�����������
--@return: Nothing
function NpcLearnSkillRich_OnEnter()
	if this:GetClientUserData( 2 ) ~= PARENT_NODE_LENGTH and this:GetClientUserData( 0 ) == 0 and
		this:GetClientUserData( 1 ) == 0 then
		return;
	end

	--���Ȼ�ø��ı��������
	local tex = getglobal( this:GetName() .. "HighLightTex" );
	tex:Show();
end

--@Desc:��Ϊ����뿪���ı����ʱ���Զ���Ӧ
--@return: Nothing
function NpcLearnSkillRich_OnLeave()
	local tex = getglobal(  this:GetName() .. "HighLightTex" );
	tex:Hide();
end

--@Desc:���ø���nLineNum��ָ�����ӿؼ��е����ݲ���ʾ���ú�Ŀؼ�����
--@Param: [in]nLineNum		ָ���ӽڵ��Ӧ�Ŀؼ����ڵ�����
--@Param: [in]tagChildNode	ָ��Ҫ�ڽ�������ʾ���ӽڵ�
--@Param: [in]nCurRowPointX	ָ�����ӽڵ��Ӧ�ĸ��ڵ������������ϽǶ����Xƫ��
--@Param: [in]nCurRowPointY	ָ�����ӽڵ��Ӧ�ĸ��ڵ������������ϽǶ����Yƫ��
--@Param: [in]nChildOffSetX	ָ�����ӽڵ����������Ӧ���ڵ��Xƫ��
--@return: Nothing
function ShowChildWidgetNode( nLineNum, tagChildNode, nCurRowPointX, nCurRowPointY , nChildOffSetX )
	--������ʾ����ȫ������ҳ�滹�ǿ�ѧϰ����ҳ��,���仯�������ɫ
	local button = getglobal( "LearnSkillFrameLearnSkillInfo"..nLineNum );
	--button:SetPoint( "topleft", "NpcLearnSkillFrame", "topleft", nCurRowPointX + nChildOffSetX , nCurRowPointY );
	button:Show();
	button:SetClientUserData( 0, tagChildNode[2] ); -- ����SkillId
	button:SetClientUserData( 1, tagChildNode[3] ); -- ����SkillLv
	button:SetClientUserData( 2, table.getn( tagChildNode ) ); -- �ؼ��ĵ�3��λ�����ڱ�����ʾ�Ľڵ�ĳ���

	local rich		= getglobal( "LearnSkillFrameLearnSkillInfo"..nLineNum.."LearnSkillRich" );
	rich:SetClientID( nLineNum );
	rich:SetClientUserData( 0 , tagChildNode[2] );
	rich:SetClientUserData( 1 , tagChildNode[3] );
	rich:SetClientUserData( 2, table.getn( tagChildNode ) ); -- �ؼ��ĵ�3��λ�����ڱ�����ʾ�Ľڵ�ĳ���
	rich:Show();
	rich:SetText( tagChildNode[1] );

	local levelFont	= getglobal( "LearnSkillFrameLearnSkillInfo"..nLineNum.."SkillLevelFont" );
	levelFont:SetText( "���ȼ�  " .. tagChildNode[3] .. "��" );
	--��⵱ǰ��ʾ��ҳ��ʱ "ȫ��" ����ҳ�滹�� "��ѧϰ"����ҳ��
	--��Ϊȫ������ҳ�棬��Ҫ������ʾ�ļ�����Ϣ����ɫ
	local mainplayer= ActorMgr:getMainPlayer();
	local skillDef = SkillMgr:getSpellStaticAtt( tagChildNode[2], tagChildNode[3] );
	if SKILLPAGE_TOTAL == nCurSkillPage then
		if mainplayer:getLv() >= skillDef.Learn.LevelMin then
			-- ��ȫ������ҳ���У������ܿ���ѧϰ������ʾΪ��ɫ
			rich:SetTextColor( 255, 255, 190 );
			levelFont:SetTextColor( 255, 255, 190 );
		else -- �����ܲ���ѧϰ����ʾΪ��ɫ
			rich:SetTextColor( 255, 60, 60 );
			levelFont:SetTextColor( 255, 60, 60 );
		end
	elseif SKILLPAGE_CANSTUDY == nCurSkillPage then --SKILLPAGE_CANSTUDY
		rich:SetTextColor( 255, 255, 190 );
		levelFont:SetTextColor( 255, 255, 190 );
	elseif SKILLPAGE_NONSTUDY == nCurSkillPage then
		rich:SetTextColor( 255, 60, 60 );
		levelFont:SetTextColor( 255, 60, 60 );
	end
end

--@Desc:���ø���nLineNum��ָ���ĸ��ؼ��е����ݲ���ʾ���ú�Ŀؼ�����
--@Param: [in]nLineNum			ָ�����ڵ��Ӧ�Ŀؼ����ڵ�����
--@Param: [in]tagParentNode		ָ��Ҫ�ڽ�������ʾ�ĸ��ڵ�
--@Param: [in]nCurRowPointX		ָ���˸��ڵ������������ϽǶ����Xƫ��
--@Param: [in]nCurRowPointY		ָ���˸��ڵ������������ϽǶ����Yƫ��
--@Param: [in]nTitleSignOffsetX	ָ���˸��ڵ��Ӧ��+-�ؼ������������ϽǶ����Xƫ��
--@return: Nothing
function ShowParentWidgetNode( nLineNum, tagParentNode, nCurRowPointX, nCurRowPointY, nTitleSignOffsetX )
	local button = getglobal( "LearnSkillFrameLearnSkillInfo"..nLineNum );
	button:Show();
	button:SetClientUserData( 0, tagParentNode[2] ); -- ���游�ڵ�ָʾ��nSkillType
	button:SetClientUserData( 1, 0 );
	button:SetClientUserData( 2, table.getn( tagParentNode ) ); -- �ؼ��ĵ�3��λ�����ڱ�����ʾ�Ľڵ�ĳ���

	local checkButton = getglobal( "LearnSkillFrameLearnSkillInfo" .. nLineNum ..  "TitleSignChk" );
	checkButton:setCheckState( tagParentNode[3] );
	checkButton:SetClientUserData( 0, tagParentNode[2] ); -- ���游�ڵ�ָʾ��nSkillType
	checkButton:SetClientUserData( 1, 0 );
	checkButton:SetClientUserData( 2, table.getn( tagParentNode ) ); -- �ؼ��ĵ�3��λ�����ڱ�����ʾ�Ľڵ�ĳ���
	checkButton:Show();

	local rich = getglobal( "LearnSkillFrameLearnSkillInfo"..nLineNum.."LearnSkillRich" );
	rich:SetClientID( nLineNum );
	rich:SetText( tagParentNode[1] );
	rich:SetTextColor( 210, 180, 130 );
	rich:Show();
	rich:SetClientUserData( 0, tagParentNode[2] ); -- ���游�ڵ��м�������
	rich:SetClientUserData( 1, 0 );
	rich:SetClientUserData( 2, table.getn( tagParentNode ) ); -- �ؼ��ĵ�3��λ�����ڱ�����ʾ�Ľڵ�ĳ���
end

--@Desc:�����������и��ı����ֿ�
--@return: Nothing
local function HideAlSkilllRich()
	local rich;
	local button;
	local checkButton;

	for i = 1, MAX_VIEW_LINE do
		rich = getglobal("LearnSkillFrameLearnSkillInfo"..i.."LearnSkillRich");
		rich:SetClientUserData( 0, 0 );
		rich:SetClientUserData( 1, 0 );
		rich:SetClientUserData( 2, 0 );
		rich:SetText( "" );

		levelRich = getglobal("LearnSkillFrameLearnSkillInfo"..i.."SkillLevelFont");
		levelRich:SetText( "" );

		button = getglobal("LearnSkillFrameLearnSkillInfo"..i);
		button:SetClientUserData( 0, 0 );
		button:SetClientUserData( 1, 0 );
		button:SetClientUserData( 2, 0 );

		checkButton = getglobal( "LearnSkillFrameLearnSkillInfo" .. i ..  "TitleSignChk" );
		checkButton:SetClientUserData( 0, 0 );
		checkButton:SetClientUserData( 1, 0 );
		checkButton:SetClientUserData( 2, 0 );
	end
end

--@Desc:��������ԭ�����е�+-�ؼ�
--@return: Nothing
local function HideAllChk()
	local checkButton;

	for i = 1, MAX_VIEW_LINE do
		local checkButton = getglobal( "LearnSkillFrameLearnSkillInfo" .. i ..  "TitleSignChk" );
		checkButton:Hide();
	end
end

--local t_ChildNode  = { �ӽڵ�����,����ID, ����Ҫ�����͵ȼ�};
--t_TotalSkillList = {[��������1] = {��Ӧ���ڵ�ParentNode1}, [��������2] = {��Ӧ���ڵ�ParentNode2}, ... }
--@Desc: ����ˢ������е�����
--@Param: [in]nStartTableNum	����ָ�������б�����ʼ��ʾ�����ڱ���������λ��
--ǰ������������Ҫȷ�����ݸ�UpdateSkillPage����ʼ������������Ч��
function UpdateSkillPage( nStartTableNum )
	--ˢ��ҳ��ǰ�������ԭ�ȵ�����
	HideAlSkilllRich();
	HideAllChk();
	for i = 1, MAX_VIEW_LINE do
		local checkButton = getglobal("LearnSkillFrameLearnSkillInfo"..i.."BackgroundTex");
		if (i + nStartTableNum) % 2 == 0 then
			checkButton:Show();
		else
			checkButton:Hide();
		end
	end

	--���ݵ�ǰ��ʾ���� ȫ�� ����ҳ�滹�� ��ѧϰ ����ҳ����ˢ��ҳ��
	local t_CurDisplayList = {};

	if SKILLPAGE_TOTAL == nCurSkillPage then
		t_CurDisplayList = t_TotalSkillList;
	elseif SKILLPAGE_CANSTUDY == nCurSkillPage then
		t_CurDisplayList = t_CanStudySkillList;
	elseif SKILLPAGE_NONSTUDY == nCurSkillPage then
		t_CurDisplayList = t_NonStudySkillList;
	end

	--�ȼ�ⷵ���Ƿ�Ϊ�ӽڵ�
	local nRichClientID		= 0;--���ڼ�¼��ǰ��ʾ������
	local nCurTableNum		= 0;--���ڼ�¼��ǰ�������ı������
	local nCurRowPointX		= 50;
	local nCurRowPointY		= 65;
	local nChildOffSetX		= 5;
	local nRowHeight		= 12;
	local nTitleSignOffsetX = 30;

	for i = 1, table.getn( t_CurDisplayList ) do
		nCurTableNum = nCurTableNum + 1;
		-- ��⵱ǰ���е������Ƿ������ʼ����
		-- ��ʾ����
		if nCurTableNum >= nStartTableNum then
			nRichClientID = nRichClientID + 1;

			--��⵱ǰ��ʾ�������Ƿ񳬳�����ʾ������
			if nRichClientID > MAX_VIEW_LINE then return end
			ShowParentWidgetNode( nRichClientID, t_CurDisplayList[i], nCurRowPointX, nCurRowPointY, nTitleSignOffsetX );
			nCurRowPointY = nCurRowPointY + nRowHeight;
		end

		--��ʾ�Ӽ�
		if t_CurDisplayList[i][3] then--��������չ��״̬
			local t_ChildInfoList = t_CurDisplayList[i][4];

			for j = 1, table.getn( t_ChildInfoList ) do
				nCurTableNum = nCurTableNum + 1;
				if nCurTableNum >= nStartTableNum then
					nRichClientID = nRichClientID + 1;

					if nRichClientID > MAX_VIEW_LINE then return end--��⵱ǰҪ��ʾ�������Ƿ񳬳�����ʾ��������Χ
					ShowChildWidgetNode( nRichClientID, t_ChildInfoList[j], nCurRowPointX, nCurRowPointY, nChildOffSetX  );
					nCurRowPointY = nCurRowPointY + nRowHeight;
				end
			end
		end
	end
end

--@Desc:��Ϊ���ӽ�����ͼ���ʱ����Ӧ
function NpcLearnSkillItemBtn_OnEnter()
	local skillId   = t_CurSelectSkill.skillId;
	local skillLv   = t_CurSelectSkill.skillLv;
	local szName    = this:GetName();

	if skillId <= 0 or skillId == nil or szName == nil or szName == "" then return end

	SpellTipsFrame:SetClientUserData(0, SkillMgr:getRuneSkillId(skillId));
	SpellTipsFrame:SetClientUserData(1, skillLv);
	SpellTipsFrame:SetClientUserData(3, TRUMP_SKILL_TYPE);
	SpellTipsFrame:SetClientString(szName);
	SpellTipsFrame:Show();
end

function NpcLearnSkillItemBtn_OnLeave()
	SpellTipsFrame:Hide();
end

function NpcLearnSkillItemBt_OnClick()
	if NlsFrame_SkillOptBtn:IsShown() then
		NlsFrame_SkillOptBtn:Hide();
	end
end

--@Desc:��Ϊ��� ȫ�� �� ��ѧϰѡ���������ͷʱ����Ӧ
--@return: Nothing
function NlsFrame_DownArrowBtn_OnClick()
	if NlsFrame_SkillOptBtn:IsShown() then
		NlsFrame_SkillOptBtn:Hide();
	else
		NlsFrame_SkillOptBtn:Show();
	end
end

--@Desc:��Ϊ�ڵ�� ȫ�� �� ��ѧϰ ��һ��ѡ��ʱ����Ӧ
--@return: Nothing
function NlsFrame_FirstLayerBtn_OnClick()
	NlsFrame_SkillOptBtn:Hide();
	local firstFont = getglobal( "NlsFrame_FirstLayerName" );
	local szFirstLineText = firstFont:GetText();
	local nPreSkillPage = nCurSkillPage;

	if "ȫ��" == szFirstLineText then
		nCurSkillPage = SKILLPAGE_TOTAL;
	elseif "��ѧϰ" == szFirstLineText then
		nCurSkillPage = SKILLPAGE_CANSTUDY;
	elseif "����ѧϰ" == szFirstLineText then
		nCurSkillPage = SKILLPAGE_NONSTUDY;
	end

	if nPreSkillPage == nCurSkillPage then
		return;
	end

	local t_CurDisplayList = {};

	if SKILLPAGE_TOTAL == nCurSkillPage then
		t_CurDisplayList = t_TotalSkillList;
	elseif SKILLPAGE_CANSTUDY == nCurSkillPage then
		t_CurDisplayList = t_CanStudySkillList;
	elseif SKILLPAGE_NONSTUDY == nCurSkillPage then
		t_CurDisplayList = t_NonStudySkillList;
	end

	local slider		= getglobal( "NLS_ScrollBar" );
	local itemButton	= getglobal( "NpcLearnSkillItemBtn" );
	itemButton:Hide();
	slider:SetValue( 0 );
	SetLearnSkillScrollBar( t_CurDisplayList );
	if slider:IsShown() then
		UpdateSkillPage( slider:GetValue() + 1 ); -- �մ�ҳ���ʱ��Ĭ�ϴӼ����б��һ�п�ʼ��ʾ
	else
		UpdateSkillPage( 1 );
	end

	NlsFrame_OneLayerName:SetText( szFirstLineText );

	-- ������ѡ�е����и�����
	for i = 1, MAX_VIEW_LINE do
		local highLightTex	= getglobal( "LearnSkillFrameLearnSkillInfo" .. i .. "LearnSkillRichSelectHighLightTex" );
		highLightTex:Hide();
	end
	HideSkillInfo();

	--���ѧϰ����е�ֵ��ֹ�� ѧϰ ��ť���ַ�Ԥ����Ϊ
	local frame	= getglobal( "NpcLearnSkillFrame" );

	t_CurSelectSkill.skillId	= 0;
	t_CurSelectSkill.skillLv	= 0;
end

--@Desc:��Ϊ�ڵ�� ȫ�� �� ��ѧϰ �ڶ���ѡ��ʱ����Ӧ
--@return: Nothing
function NlsFrame_SecondLayerBtn_OnClick()
	NlsFrame_SkillOptBtn:Hide();
	local secondFont = getglobal( "NlsFrame_SecondLayerName" );
	local szFirstLineText = secondFont:GetText();
	local nPreSkillPage = nCurSkillPage;

	if "ȫ��" == szFirstLineText then
		nCurSkillPage = SKILLPAGE_TOTAL;
	elseif "��ѧϰ" == szFirstLineText then
		nCurSkillPage = SKILLPAGE_CANSTUDY;
	elseif "����ѧϰ" == szFirstLineText then
		nCurSkillPage = SKILLPAGE_NONSTUDY;
	end

	if nPreSkillPage == nCurSkillPage then
		return;
	end

	local t_CurDisplayList = {};

	if SKILLPAGE_TOTAL == nCurSkillPage then
		t_CurDisplayList = t_TotalSkillList;
	elseif SKILLPAGE_CANSTUDY == nCurSkillPage then
		t_CurDisplayList = t_CanStudySkillList;
	elseif SKILLPAGE_NONSTUDY == nCurSkillPage then
		t_CurDisplayList = t_NonStudySkillList;
	end

	local slider		= getglobal( "NLS_ScrollBar" );
	local itemButton	= getglobal( "NpcLearnSkillItemBtn" );
	itemButton:Hide();
	slider:SetValue( 0 );
	SetLearnSkillScrollBar( t_CurDisplayList );
	if slider:IsShown() then
		UpdateSkillPage( slider:GetValue() + 1 ); -- �մ�ҳ���ʱ��Ĭ�ϴӼ����б��һ�п�ʼ��ʾ
	else
		UpdateSkillPage( 1 );
	end

	NlsFrame_OneLayerName:SetText( szFirstLineText );

	-- ������ѡ�е����и�����
	for i = 1, MAX_VIEW_LINE do
		local highLightTex	= getglobal( "LearnSkillFrameLearnSkillInfo" .. i .. "LearnSkillRichSelectHighLightTex" );
		highLightTex:Hide();
	end
	HideSkillInfo();

	--���ѧϰ����е�ֵ����ֹ�� ѧϰ ��ť���ַ�Ԥ����Ϊ
	local frame	= getglobal( "NpcLearnSkillFrame" );

	t_CurSelectSkill.skillId	= 0;
	t_CurSelectSkill.skillLv	= 0;
end

function NlsFrame_FirstLayerBtn_OnEnter()
	--���Ȼ�ø��ı��������
	local szName = this:GetName();
	LearnSkillFrameSkillHighLightTex:SetPoint( "center", szName, "center", 1, 0 );
	LearnSkillFrameSkillHighLightTex:Show();
end

function NlsFrame_FirstLayerBtn_OnLeave()
	LearnSkillFrameSkillHighLightTex:Hide();
end

function NlsFrame_SecondLayerBtn_OnEnter()
	--���Ȼ�ø��ı��������
	local szName = this:GetName();
	LearnSkillFrameSkillHighLightTex:SetPoint( "center", szName, "center", 1, 0 );
	LearnSkillFrameSkillHighLightTex:Show();
end

function NlsFrame_SecondLayerBtn_OnLeave()
	LearnSkillFrameSkillHighLightTex:Hide();
end

--@Desc:��Ϊ�ڵ�� ȫ�� �� ��ѧϰ �ڶ���ѡ��ʱ����Ӧ
--@return: Nothing
function NlsFrame_ThirdLayerBtn_OnClick()
	NlsFrame_SkillOptBtn:Hide();
	local thirdFont = getglobal( "NlsFrame_ThirdLayerName" );
	local szFirstLineText = thirdFont:GetText();
	local nPreSkillPage = nCurSkillPage;

	if "ȫ��" == szFirstLineText then
		nCurSkillPage = SKILLPAGE_TOTAL;
	elseif "��ѧϰ" == szFirstLineText then
		nCurSkillPage = SKILLPAGE_CANSTUDY;
	elseif "����ѧϰ" == szFirstLineText then
		nCurSkillPage = SKILLPAGE_NONSTUDY;
	end

	if nPreSkillPage == nCurSkillPage then
		return;
	end

	local t_CurDisplayList = {};

	if SKILLPAGE_TOTAL == nCurSkillPage then
		t_CurDisplayList = t_TotalSkillList;
	elseif SKILLPAGE_CANSTUDY == nCurSkillPage then
		t_CurDisplayList = t_CanStudySkillList;
	elseif SKILLPAGE_NONSTUDY == nCurSkillPage then
		t_CurDisplayList = t_NonStudySkillList;
	end

	local slider		= getglobal( "NLS_ScrollBar" );
	local itemButton	= getglobal( "NpcLearnSkillItemBtn" );
	itemButton:Hide();
	slider:SetValue( 0 );
	SetLearnSkillScrollBar( t_CurDisplayList );
	if slider:IsShown() then
		UpdateSkillPage( slider:GetValue() + 1 ); -- �մ�ҳ���ʱ��Ĭ�ϴӼ����б��һ�п�ʼ��ʾ
	else
		UpdateSkillPage( 1 ); -- ������������ʾ��ӵ�һ����ʾ����
	end

	NlsFrame_OneLayerName:SetText( szFirstLineText );

	-- ������ѡ�е����и�����
	for i = 1, MAX_VIEW_LINE do
		local highLightTex	= getglobal( "LearnSkillFrameLearnSkillInfo" .. i .. "LearnSkillRichSelectHighLightTex" );
		highLightTex:Hide();
	end
	HideSkillInfo();

	--���ѧϰ����е�ֵ����ֹ�� ѧϰ ��ť���ַ�Ԥ����Ϊ
	local frame	= getglobal( "NpcLearnSkillFrame" );

	t_CurSelectSkill.skillId	= 0;
	t_CurSelectSkill.skillLv	= 0;
end

function NlsFrame_ThirdLayerBtn_OnEnter()
	--���Ȼ�ø��ı��������
	local szName = this:GetName();
	LearnSkillFrameSkillHighLightTex:SetPoint( "center", szName, "center", 1, 0 );
	LearnSkillFrameSkillHighLightTex:Show();
end

function NlsFrame_ThirdLayerBtn_OnLeave()
	LearnSkillFrameSkillHighLightTex:Hide();
end

function ShowNpcLearnSkillFrameXinShou()
	local xinShouBtn	= getglobal( "NLS_SkillInfoBtnXinShouBtn" );
	local tex			= getglobal( "NpcLearnSkillFrame_StudyBtnUVAnimationTex" );
	local rich			= getglobal( xinShouBtn:GetName().."Rich" );
	tex:SetUVAnimation( 50, true);
	local szText = "���ѧϰ����";
	local nSelfDefineExtendWidth = 10;
	rich:SetClientString( szText );
	ReSizeTooltip( rich:GetName(), xinShouBtn:GetName(), nSelfDefineExtendWidth );
	xinShouBtn:Show();
end

function NLS_SkillInfoBtn_OnShow()
	--print( "NLS_SkillInfoBtn_OnShow--1660" );
	if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_NPCLEARNSKILL_STUDY_BTN ) then
		ShowNpcLearnSkillFrameXinShou();
		--print( "NLS_SkillInfoBtn_OnShow--1663" );
	end
	local TipTxt = getglobal("NlsFrameMoneyTipFont");
	TipTxt:SetText("��ʾ��ѧϰ�����������İ������������㽫��������");
	NpcLearnSkillFrame:SetSize( 782,493 )
end

function NLS_SkillInfoBtn_OnHide()
	local t_hideIcons = { "NLS_SkillInfoBtnXinShouBtn", "NpcLearnSkillFrame_StudyBtnUVAnimationTex" };
	util.HideIcons( t_hideIcons );
	NpcLearnSkillFrame:SetSize( 391,493 )
end