
local CLAN_PET_MAX_JIYANG = 24;
local nCurIdx = 0;					-- ��ǰ��λ
local nClanMaxNum	= 0;			
local bCurClanType 	= true;      	-- �Ƿ��Ǳ��˹���
local nPetWidHigh 	= 0;			-- ��ǰ��λ����wid����
local nPetWidLow	= 0;			-- ��ǰ��λ����wid����
local nPetTouNum 	= 1;			-- �����������մ������������ģ�

function setClanPetTouNum( nNum )
	nPetTouNum = nNum + 1;
end
local t_ClanPetFrame = { "PetFosterFrame","PetFosterFeedFrame","PetFosterChooseFrame","PetFosterPlayFrame" }
local t_RailingPos = {
	{ PosX = 27094,PosZ = 10148},{ PosX = 27486,PosZ = 10764},	{ PosX = 27162,PosZ = 9670},	{ PosX = 27884,PosZ = 10788},	{ PosX = 27283,PosZ = 9252},	
	{ PosX = 28305,PosZ = 10809},{ PosX = 27435 ,PosZ = 8853},	{ PosX = 28707,PosZ = 10914},	{ PosX = 27644,PosZ = 8394},	{ PosX = 29185,PosZ = 11022},	
	{ PosX = 28117,PosZ = 8341},{ PosX = 28554,PosZ = 8278},	{ PosX = 28989,PosZ = 8171},	{ PosX = 29426,PosZ = 8027},	{ PosX = 29899,PosZ = 7921},	
	{ PosX = 30394,PosZ = 7972},{ PosX = 30881,PosZ = 8295},	{ PosX = 31239,PosZ = 8754},	{ PosX = 31593,PosZ = 9141},	{ PosX = 31967 ,PosZ = 9467},
}

local function detectClanPetRailingDistance( frameName )
	local frame = getglobal( frameName );
	if nCurIdx <= 0 then 
		frame:Hide();
		return;
	end
	local npc = ExtraActorMgr:FindActor( nCurIdx );
	if npc == nil then
		frame:Hide();
		nCurIdx = 0;
		return;
	end
	local nDist = util.CalActorsDistance( npc, ActorMgr:getMainPlayer() );
	if nDist > ( NpcDialog:getMaxNpcTalkDist( npc ) / 100 ) ^ 2 then
		frame:Hide();
		nCurIdx = 0
	end
end

function SetClanPetAllRailing( nClanPetMax )
	for i = 1,#t_RailingPos do
		if i > nClanPetMax then 
			break;
		end
		GameClanManager:updateClanPetRailing( t_RailingPos[i]["PosX"],t_RailingPos[i]["PosZ"],i-1 )
	end
	nClanMaxNum = nClanPetMax;
end

function OpenClanPetRailing( nIdx )
	for _, name in ipairs( t_ClanPetFrame ) do
		local frame = getglobal( name );
		if frame:IsShown() then 
			frame:Hide();
		end
	end 

	nCurIdx = nIdx;
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return; end
	local nCurClanId = GameClanManager:getCurClanCityId();
	
	if MainPlayer:getClanID() == nCurClanId then 
		bCurClanType = true;
	else 
		bCurClanType = false;
	end
	local PetInfo = GameClanManager:getClanPetData(nCurIdx-1);
	if PetInfo == nil and bCurClanType then 
		local petPackage = MainPlayer:getPet();
		if petPackage:getPetNum() == 0 then
			ShowMidTips( "����û�г�����ܽ��м�������" );
			return;
		end
		PetFosterChooseFrame:Show();
	elseif PetInfo == nil and MainPlayer:getClanID() == 0 then 
		ShowMidTips( "��������������Ǿͼ������ǹ���ɣ����뷽ʽ����ѯ˹�ٷ�˹��" );
		return;
	elseif PetInfo == nil and not bCurClanType then 
		ShowMidTips( "ֻ�б������Ա����ʹ������ճ��������������������ǹ��ᣬ����ѯ˹�ٷ�˹��" );
		return;
	else 
		PetFosterFrame:Show();
	end
end
------------------------------������� ѡ�񲿷�------------------------------------
local MAX_RAILING_PET_NUM = 6;
local t_JiYangChooseTime = { 1,2,4,6}
local nCurChooseTime = 1;				-- ѡ�����ʱ��
local nPetIndex		= 0;				-- ѡ��Ҫ��������index
local isHighestLv	= false;

local function updateClanPetTime( nBtnID )
	local lastBtn = getglobal("PetFosterChooseFrame_TimeBtn"..nCurChooseTime.."_CheckBtn")
	if nCurChooseTime == nBtnID then 
		lastBtn:setCheckState(true);
		return;
	end
	nCurChooseTime = nBtnID;
	lastBtn:setCheckState(false);	
end
local function updateClanPetIndex( nBtnID )
	local lastBtn = getglobal( "PetFosterChooseFrame_Btn"..nPetIndex.."_CheckBtn" )
	if nPetIndex == nBtnID then 
		lastBtn:setCheckState(true);
		return;
	end
	nPetIndex = nBtnID;
	lastBtn:setCheckState(false);	
	SetChoosePetInfo()
end
function PetFosterChooseFrameCheckButton_OnClick()
	local btn = getglobal( this:GetParent() );
	if string.find( this:GetName() , "_TimeBtn") ~= nil	then
		updateClanPetTime( btn:GetClientID() );
	else 
		updateClanPetIndex( btn:GetClientID() );
	end
end

function PetFosterChooseFrameOKBtnOnClick()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return; end 

	local petPackage 	= MainPlayer:getPet();
	local petInfo 		= petPackage:getPetInfo( nPetIndex -1 );
	if petInfo == nil then 
		PetFosterChooseFrame:Hide();
		return;
	end
	local nHighLvCheck = 0
	if PetFosterChooseFrameHighLvBtn_CheckBtn:GetCheckState() then
		nHighLvCheck = 1
	end
	GameClanManager:Clan_Pet_JiYang( nCurIdx - 1,t_JiYangChooseTime[nCurChooseTime], petInfo.WID.nHeight, petInfo.WID.nLow, nHighLvCheck );
	PetFosterChooseFrame:Hide();
end

function PetFosterChooseFrame_OnLoad()
	PetFosterChooseFrameTipRich:SetText( "#cfeeeb2��ʾ��#n��;ȡ���������践��", 255, 255, 255 );
	this:RegisterEvent( "GE_DEL_PET" );
	this:RegisterEvent("GE_PET_ATTR_CHG");
	this:RegisterEvent("GE_GET_NEW_PET");
	this:RegisterEvent("GE_CLAN_PET_LEAVE");
	this:setUpdateTime( 1 );
end

function PetFosterChooseFrame_OnEvent()
	if arg1 == "GE_CLAN_PET_LEAVE" then
		for _, name in ipairs( t_ClanPetFrame ) do
			local frame = getglobal( name );
			if frame:IsShown() then 
				frame:Hide();
			end
		end 
	end 
	if not this:IsShown() then
		return;
	end
	local mainplayer 	= ActorMgr:getMainPlayer();
	local petPackage 	= mainplayer:getPet();
	local petNum 		= petPackage:getPetNum();
	if petNum == 0 then 
		this:Hide();
	end
	if(arg1 == "GE_DEL_PET") then
		if nPetIndex > petNum then 
			nPetIndex = 1;
			CleanChoosePetInfo(); 
		end
		SetChoosePetInfo();
	elseif arg1 == "GE_PET_ATTR_CHG" then 
		SetChoosePetInfo();
	elseif arg1 == "GE_GET_NEW_PET" then 
		SetChoosePetInfo();	
	end
end
function CleanChoosePetInfo()
	local btn;
	local checkBtn ;
	local petFont;
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return; end 
	
	local petPackage = MainPlayer:getPet();
	for i=1,MAX_RAILING_PET_NUM do 
		btn = getglobal("PetFosterChooseFrame_Btn"..i);
		petFont = getglobal( btn:GetName() .. "Font")
		if i > petPackage:getPetNum() then 
			btn:Hide();
		else 
			local PetInfo = petPackage:getPetInfo( i - 1 );
			petFont:SetText( PetInfo.Name );
			btn:Show();
		end
		checkBtn = getglobal( btn:GetName().."_CheckBtn");
		checkBtn:setCheckState(false);
	end
	for i=1,#t_JiYangChooseTime do
		checkBtn = getglobal( "PetFosterChooseFrame_TimeBtn"..i.."_CheckBtn");
		checkBtn:setCheckState(false);	
	end
	-- ��ʼ�� checkBtn
	checkBtn = getglobal("PetFosterChooseFrame_Btn"..nPetIndex.."_CheckBtn")
	if checkBtn ~= nil then 
		checkBtn:setCheckState(true)
	end
	checkBtn = getglobal("PetFosterChooseFrame_TimeBtn"..nCurChooseTime.."_CheckBtn")
	if checkBtn ~= nil then 
		checkBtn:setCheckState(true)
	end
end
function SetChoosePetInfo()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return; end 
	local szText = "";
	local petPackage = MainPlayer:getPet();
	local petInfo 	= petPackage:getPetInfo( nPetIndex-1 );
	local petImpUp	= getPetImpUpDef( petInfo.Imp );
	
	if petImpUp == nil then
		PetFosterChooseFrameShowHighestLv();
		isHighestLv = true;
	else
		PetFosterChooseFrameExtraFont:Hide();
		PetFosterChooseFrameHighLvBtn_CheckBtn:Enable();
		isHighestLv = false;
	end
	
	if not isHighestLv then
		if nCurIdx > GameClanManager:getClanPetFreeNum() then 
			szText = "#cfeeeb2��λ���ͣ�#n#G������λ#n"
			PetFosterChooseFrameTypeRich:SetText( szText, 255, 255, 255 )
			szText = "#cfeeeb2���ã�#n#G"..petImpUp.UseMoney.."����/Сʱ#n"
			PetFosterChooseFrameCostRich:SetText( szText, 255, 255, 255 )
		else 
			szText = "#cfeeeb2��λ���ͣ�#n���������λ"
			PetFosterChooseFrameTypeRich:SetText( szText, 255, 255, 255 )
			szText = "#cfeeeb2���ã�#n"..petImpUp.UseContri.."���ṱ�׶�/Сʱ"
			PetFosterChooseFrameCostRich:SetText( szText, 255, 255, 255 )
		end

		for i=1,#t_JiYangChooseTime do
			local petFont = getglobal("PetFosterChooseFrame_TimeBtn"..i.."Font");
			local expNum = petImpUp.UseExp * 20 * t_JiYangChooseTime[i]
			petFont:SetText( "����"..t_JiYangChooseTime[i].."Сʱ��Ҫ"..expNum.."����ֵ")
		end
	end
	local petDef = getPetDef( petInfo.PetID );
	local szTypeText = ""
	if petPackage:isExtraBattlePet(petInfo) then
		szTypeText = ( petDef.Type == PET_TYPE_ABERRANCE ) and "����ս��" or "ս��"
		szText = "#cfeeeb2�������#n"..petInfo.Name.."\n#cfeeeb2�������ͣ�#n"..szTypeText.."\n#cfeeeb2�ɳ����ʣ�#n"..petInfo.Imp
	else 
		szTypeText = ( petDef.Type == PET_TYPE_ABERRANCE ) and "�������" or "��ͨ����"
		szText = "#cfeeeb2�������#n"..petInfo.Name.."\n#cfeeeb2�������ͣ�#n"..szTypeText.."\n#cfeeeb2�ɳ����ʣ�#n"..petInfo.Imp
	end
	PetFosterChooseFramePetRich1:SetText( szText,255,255,255 );

	if not isHighestLv then
		local percentVal = math.floor( petInfo.ImpExp * 100 / petImpUp.ExpUp )
		percentVal = ( petInfo.ImpExp >= petImpUp.ExpUp ) and 100 or percentVal
		szText = "#cfeeeb2����ȼ���#n"..petInfo.Level.."��\n#cfeeeb2�������ʣ�#n"..GetPetGeniusValueDesc( petInfo ).."\n#cfeeeb2��ǰǱ����#n"..percentVal.."%"
		PetFosterChooseFramePetRich2:SetText( szText,255,255,255 )
	end
end

function PetFosterChooseFrame_OnShow()
	nPetIndex = 1;
	CleanChoosePetInfo(); 
	CleanHighLvCheckInfo();
	SetChoosePetInfo();
	PetFosterChooseFrame:SetPoint("left", "$parent", "left", 0, 0 );
	--�߼�����ѡ��״̬���
end

function PetFosterChooseFrame_OnUpdate()
	detectClanPetRailingDistance( "PetFosterChooseFrame" );
end

function PetFosterChooseFrame_OnHide()
	nCurChooseTime 	= 1;	
	nPetIndex		= 1;
end

--�߼��������
local szHighLvDesc = [[#cffffbe    ����ʣ�����ʱ�䣺



#cffffff    ע���߼�����ÿ��ÿ��ֻ��1Сʱ�ļ���ʱ�䣬
#cffffff            �߼��������Ի�ȡ��ͨ������30������ֵ]]
function CleanHighLvCheckInfo()
	if not isHighestLv then
		PetFosterChooseFrameHighLvBtn_CheckBtn:setCheckState( false )
		PetFosterChooseFrameHighLvDesc:Hide()
		for i=1,#t_JiYangChooseTime do
			local checkTimeBtn = getglobal("PetFosterChooseFrame_TimeBtn"..i);
			checkTimeBtn:Show()
		end
	end
end
function PetFosterChooseFrameHighLv_OnClick()
	PetFosterChooseFrameHighLvDescRich:SetText( szHighLvDesc, 255,255,190 )
	local time = math.floor(GameClanManager:getClanPetHighLvLastTime()/60)
	PetFosterChooseFrameHighLvDescFont:SetText( time.."����" )
	PetFosterChooseFrameHighLvDesc:Show()
	for i=1,#t_JiYangChooseTime do
		local checkTimeBtn = getglobal("PetFosterChooseFrame_TimeBtn"..i);
		checkTimeBtn:Hide()
	end
	PetFosterChooseFrameExtraFont:Hide();
end

function PetFosterChooseFrameShowHighestLv()
	PetFosterChooseFrameExtraFont:Show();
	PetFosterChooseFrameHighLvDesc:Hide();
	for i=1,#t_JiYangChooseTime do
		local checkTimeBtn = getglobal("PetFosterChooseFrame_TimeBtn"..i);
		checkTimeBtn:Hide()
	end
	PetFosterChooseFrameHighLvBtn_CheckBtn:Disable();
end

function PetFosterChooseFrameHighLv_OnUpdate()
	if not isHighestLv then
		local time = math.floor(GameClanManager:getClanPetHighLvLastTime()/60)
		PetFosterChooseFrameHighLvDescFont:SetText( time.."����" )
		if PetFosterChooseFrameHighLvBtn_CheckBtn:GetCheckState() then
			PetFosterChooseFrameHighLvDesc:Show()
			for i=1,#t_JiYangChooseTime do
				local checkTimeBtn = getglobal("PetFosterChooseFrame_TimeBtn"..i);
				checkTimeBtn:Hide()
			end
		else
			PetFosterChooseFrameHighLvDesc:Hide()
			for i=1,#t_JiYangChooseTime do
				local checkTimeBtn = getglobal("PetFosterChooseFrame_TimeBtn"..i);
				checkTimeBtn:Show()
			end
		end
	end
end
------------------------------������� ��Ϣ�鿴����-----------------------------------
local isMainplayerPet = true;
local nWeiYangNum = 0;
local t_CurWeiYangID = { 0,0,0 };  	-- ι�����ý������

local nWanShuaId = 1;
local function getJiYangSiLiaoImpLv( itemId )
	local itemDef = getItemDef( itemId );
	if itemDef == nil then 
		return 0; 
	end
	return GetItemResultValue1( itemDef,RESULT_PET_SILIAO )
end
-- ��ü��������Ʒ
local function getItemContainerGrid( nResult )
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then 	return 	end
	local container = mainplayer:getContainer();
	local petJiYang = GameClanManager:getClanPetData( nCurIdx - 1 );
	local nLevel 	= 199;
	local nGrid 	= -1; 
	for i=1,container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES ) do
		local item 		= container:getItem( CONTAINER_TYPE_SUNDRIES, i-1 );
		local itemDef	= item:getItemDef();
		if itemDef ~= nil and itemDef.WearPre.RoleLevelMin <= mainplayer:getLv() and CheckItemResult( itemDef,nResult ) then
			if nResult == RESULT_PET_WANJU and itemDef.WearPre.RoleLevelMin < nLevel then 
				nLevel	= itemDef.WearPre.RoleLevelMin;
				nGrid 	= i-1;
			elseif nResult == RESULT_PET_SILIAO 
				and petJiYang.Imp <= getJiYangSiLiaoImpLv( itemDef.ItemID ) 
				and getJiYangSiLiaoImpLv( itemDef.ItemID ) < nLevel then 
				
				nLevel	= getJiYangSiLiaoImpLv( itemDef.ItemID );
				nGrid 	= i-1;
			end
		end
	end
	return nGrid;
end

function GetClanPetBoxAward( nType )
	if not PetFosterFrame:IsShown() then 
		return
	end
	local modelview = getglobal("PetFosterFrameAwardBtnView");
	modelview:setPetModel(0, 0, "effect\\entity\\293008_11.ent")
	modelview:setEntityPosition(0, 0, 100, 0)
	modelview:Show();
end
-- ȡ��������ť
function PetFosterFrameConfirmBtn_OnClick()	
	if isMainplayerPet then 
		MessageBox("ȡ������","��ȷ��Ҫȡ��������");
	else 
		MessageBox("ȡ������","��ȷ��Ҫ����˼�����");
	end
	MessageBoxFrame:SetClientString( "ȡ������" )	
end

function ClanPetJiYangEnd()
	local petInfo = GameClanManager:getClanPetData(nCurIdx-1);
	if petInfo == nil then 
		return;
	end
	if isMainplayerPet then 
		GameClanManager:Clan_Pet_JiYangEnd( nCurIdx - 1, petInfo.PetWid.nHeight, petInfo.PetWid.nLow );
	else 
		GameClanManager:Clan_Pet_Clear( nCurIdx - 1, petInfo.PetWid.nHeight, petInfo.PetWid.nLow );
	end
	PetFosterFrame:Hide();
end

--ι����ť
function PetFosterFrameFeedBtn1_OnClick()
	local nGrid = -1;
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	local container = MainPlayer:getContainer();
	local petJiYang = GameClanManager:getClanPetData( nCurIdx - 1 );
	if petJiYang == nil then return end
	local curTime = getServerTime().sec
	if petJiYang.JiYangEnd > 0 and petJiYang.JiYangEnd < curTime then 
		ShowMidTips("�����ѽ�����ֹ����״̬���޷�ι����");
		return
	end
	if petJiYang.WenBao >= 100 then 
		ShowMidTips("���ﵱǰ�±�ֵ����������ι����");
		return
	end
	if nWeiYangNum == 0 then 
		nGrid = getItemContainerGrid( RESULT_PET_SILIAO );
	else
		local btn; 
		for i = 1,3 do 
			local itemId = t_CurWeiYangID[i];
			if itemId == 0 then 
				nGrid = -1 
			else 
				nGrid = container:getItemPos( CONTAINER_TYPE_SUNDRIES,itemId );
			end
			if nGrid >= 0 and petJiYang.Imp <= getJiYangSiLiaoImpLv( itemId ) then 
				GameClanManager:Clan_Pet_Eat( nCurIdx - 1,nGrid,nPetWidHigh, nPetWidLow );
				return;
			end
		end
		nGrid = getItemContainerGrid( RESULT_PET_SILIAO );			
	end
	if nGrid < 0 then 
		ShowMidTips("����ǰ�޿�����ι���ĵ���");
		return;
	else 
		GameClanManager:Clan_Pet_Eat( nCurIdx - 1,nGrid,nPetWidHigh, nPetWidLow );	
	end
end
--ι�����ð�ť
function PetFosterFrameFeedBtn2_OnClick()
	if PetFosterFeedFrame:IsShown() then 
		PetFosterFeedFrame:Hide();
	else 
		PetFosterFeedFrame:Show();
	end
end
-- ι���������
local t_WeiYangItemId = {}; 		-- Ĭ����ʾʹ��

function PetFosterFeedFrameConfirmBtn_OnClick()
	nWeiYangNum = 0 ;
	for i=1,3 do 
		local btn = getglobal( "PetFosterFeedFrameItemBtn"..i );
		local itemID = btn:GetClientUserData(0);
		if itemID > 0 then 
			nWeiYangNum = nWeiYangNum + 1;
		end
		t_CurWeiYangID[i] = itemID;
	end
	PetFosterFeedFrame:Hide();
end

function PetFosterFeedFrameCancleBtn_OnClick()
	PetFosterFeedFrame:Hide();
end
local function AddWeiYangItemID( nGrid )
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then 	return 	end
	local container = mainplayer:getContainer();
	item = container:getItem( CONTAINER_TYPE_SUNDRIES, nGrid - 1 );
	local itemDef	= item:getItemDef();
	local nIndex = 0;
	for i=1, #t_WeiYangItemId do 
		if t_WeiYangItemId[i][1] == item:getItemId() then 
			nIndex = i;
		end
	end
	-- ȡ���ظ��� ���Ϸ���
	if nIndex == 0 and itemDef ~= nil then 
		table.insert( t_WeiYangItemId, { itemDef.ItemID, getJiYangSiLiaoImpLv( itemDef.ItemID )} )
	end
end
function PetFosterFeedFrame_OnLoad()
	this:RegisterEvent("GE_CONTAINER_CHG")
end

function PetFosterFeedFrame_OnEvent()
	if not this:IsShown() then
		return;
	end
	local mainplayer 	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer()
	if arg1 == "GE_CONTAINER_CHG" then 
		for i=1,3 do 
			local btn 	= getglobal( "PetFosterFeedFrameItemBtn"..i );
			local icon	= getglobal( btn:GetName().."IconTexture");
			local uvTex	= getglobal( btn:GetName().."UVAnimationTex" );
			local itemId = btn:GetClientUserData(0)
			if itemId > 0 and container:getItemCount( itemId ) == 0 then
				icon:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
				uvTex:Hide();
			end
		end	
	end
end

function PetFosterFeedFrame_OnShow()
	if PetFosterPlayFrame:IsShown() then 
		PetFosterPlayFrame:Hide();
	end
	PetFosterFeedFrame:SetPoint("center", "$parent", "center", 0, 0 );
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then 	return 	end
	local container = mainplayer:getContainer();
	if nWeiYangNum > 0 then 
		for i = 1,#t_CurWeiYangID do 
			local btn 	= getglobal("PetFosterFeedFrameItemBtn"..i);
			local icon	= getglobal( btn:GetName().."IconTexture");
			local uvTex	= getglobal( btn:GetName().."UVAnimationTex" );
			local itemId = t_CurWeiYangID[i];
			local itemDef	= getItemDef( itemId );
			if itemId == 0 or itemDef == nil then 
				icon:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
				uvTex:Hide();
			else 
				local IconPath = "uires\\ui\\mask\\TouMingTongDao.tga";
				local itemNum  = container:getItemCount( itemId )
				if itemNum > 0 then 
					IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
					-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
					if not IsInExistence( IconPath ) then
						IconPath = DEFAULT_ITEM_PATH;
					end
					SetEquipImportantEffectUV( itemDef, btn:GetName().."UVAnimationTex" );
				else 
					uvTex:Hide();
					t_CurWeiYangID[i] = 0;
					nWeiYangNum = nWeiYangNum - 1
				end
				icon:SetTexture( IconPath );		
			end	
			btn:SetClientUserData(0,t_CurWeiYangID[i]);
		end
		if nWeiYangNum > 0 then 
			return;
		end
	end
	t_WeiYangItemId = {}
	local petJiYang = GameClanManager:getClanPetData( nCurIdx - 1 );
	if petJiYang == nil then return end
	local mainLv = mainplayer:getLv();
	for i = 1, container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES ) do
		item = container:getItem( CONTAINER_TYPE_SUNDRIES, i - 1 );
		local itemDef	= item:getItemDef();

		if itemDef ~= nil 
			and CheckItemResult( itemDef,RESULT_PET_SILIAO ) 
			and petJiYang.Imp <= getJiYangSiLiaoImpLv( itemDef.ItemID ) then
			
			if table.getn( t_WeiYangItemId ) == 0 then 
				table.insert( t_WeiYangItemId, { itemDef.ItemID, getJiYangSiLiaoImpLv( itemDef.ItemID )} );
			else 
				AddWeiYangItemID( i );
			end
		end
	end

	table.sort( t_WeiYangItemId, function( a,b )
	return a[2] < b[2];
	end);
	UpdateFeedFrameItemInfo();
end
-- Ĭ��״̬����
function UpdateFeedFrameItemInfo()
	for i=1, 3 do 
		local btn = getglobal("PetFosterFeedFrameItemBtn"..i );
		local icon	= getglobal( btn:GetName().."IconTexture");
		local uvTex	= getglobal( btn:GetName().."UVAnimationTex" );
		icon:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
		uvTex:Hide();
		btn:SetClientUserData(0,0);
	end
	if #t_WeiYangItemId == 0 then 
		return;
	end
	for i=1,#t_WeiYangItemId do 
		if i > 3 then break; end
		local btn = getglobal("PetFosterFeedFrameItemBtn"..i );
		local icon	= getglobal( btn:GetName().."IconTexture");
		local itemDef = getItemDef( t_WeiYangItemId[i][1] );
		if itemDef ~= nil then
			local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
			-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			icon:SetTexture( IconPath );
			SetEquipImportantEffectUV( itemDef, btn:GetName().."UVAnimationTex" );	
			btn:SetClientUserData(0,t_WeiYangItemId[i][1]);
		end
	end
end
function PetFosterFeedFrameItemBtn_OnClick()
	if( arg1 == "RightButton" ) then
		if UIIsInDragState() then
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return;
		end
		if this:GetClientUserData(0) > 0 then 
			local icon	= getglobal( this:GetName().."IconTexture");
			local uvTex	= getglobal( this:GetName().."UVAnimationTex" );
			icon:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
			uvTex:Hide();
			this:SetClientUserData(0,0);
		end
	end
end
function PetFosterFeedFrameItemBtnDrag()
	UIMSG:getUIGV().DragFrom = DRAG_CLAN_PET_FEED;
	local IconTex = getglobal(this:GetName().."IconTexture");
	if this:GetClientUserData(0) == 0 then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	else
		UIBeginDrag(IconTex:GetTexture() ,this:GetClientUserData(0), this:GetClientID() ,0 );
	end
end
function PetFosterFeedFrameItemBtnReceiveDrag()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	end
	local container = MainPlayer:getContainer();
	if container == nil then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return
	end
	local srcList = UIMSG:getUIGV().DragFrom;
	
	if srcList ~= CONTAINER_TYPE_SUNDRIES and srcList ~= DRAG_CLAN_PET_FEED then 
		return
	end
	if srcList == DRAG_CLAN_PET_FEED then 
		local itemID 	= UIGetDragUserData1();
		local btnId		= UIGetDragUserData2();
		local srcBtn	= getglobal("PetFosterFeedFrameItemBtn"..btnId)
		local itemDef 	= getItemDef(itemID)
		local icon	= getglobal( this:GetName().."IconTexture");
		local desItemID = this:GetClientUserData(0)	
		if itemDef ~= nil then 
			local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
			-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			icon:SetTexture( IconPath );
			SetEquipImportantEffectUV( itemDef, this:GetName().."UVAnimationTex" );	
			this:SetClientUserData(0,itemID)
		end
		icon = getglobal( srcBtn:GetName().."IconTexture")
		local uvTex = getglobal( srcBtn:GetName().."UVAnimationTex" )
		if desItemID == 0 then 
			IconPath = "uires\\ui\\mask\\TouMingTongDao.tga"
			icon:SetTexture( IconPath )
			uvTex:Hide()
			srcBtn:SetClientUserData(0,0)
		else 
			itemDef = getItemDef(desItemID)				
			IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga"
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			icon:SetTexture( IconPath )
			SetEquipImportantEffectUV( itemDef, srcBtn:GetName().."UVAnimationTex" );	
			srcBtn:SetClientUserData(0,desItemID)
		end
	else 
		local srcGrid 	= UIGetDragUserData1();	
		local item 		= container:getItem( srcList, srcGrid );
		local itemDef 	= item:getItemDef();
		if itemDef ~= nil and CheckItemResult( itemDef,RESULT_PET_SILIAO ) then 
			local icon	= getglobal( this:GetName().."IconTexture");

			local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
			-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			icon:SetTexture( IconPath );
			SetEquipImportantEffectUV( itemDef, this:GetName().."UVAnimationTex" );	
			this:SetClientUserData(0,itemDef.ItemID)
		else 
			ShowMidTips("����Ʒ���ǳ���ιʳ���ߣ�")
		end
	end
	UIEndDrag();
	UIMSG:getUIGV().DragFrom = DRAG_NONE;
end

--��ˣ��ť
local WANSHUA_USE_MONEY = 1000
function PetFosterFramePlayBtn1_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then 	return 	end

	local nGrid 	= getItemContainerGrid( RESULT_PET_WANJU );
	local PetInfo 	= GameClanManager:getClanPetData(nCurIdx-1);
	local curTime 	= getServerTime().sec
	if PetInfo ~= nil and PetInfo.JiYangEnd > 0 and PetInfo.JiYangEnd < curTime then 
		ShowMidTips("�����ѽ�����ֹ����״̬���޷���ˣ��");
		return
	end
	if PetInfo ~= nil and PetInfo.HappyVal == 100 then 
		ShowMidTips("�ó������ڵ�����ǳ����ã���ʱ����Ҫ��������ˣ");
		return;
	end
	if nWanShuaId == 1 then 
		if nGrid < 0 then 
			ShowMidTips("�������޿�������ˣ�ĵ���");
			return;
		else 
			GameClanManager:Clan_Pet_WanShua( nCurIdx - 1,nGrid,1,nPetWidHigh,nPetWidLow );
			return;
		end
	else 
		if mainplayer:getMoney() < WANSHUA_USE_MONEY then 
			ShowMidTips("���Ҳ��㣬��ͨ������ˣ���á�ѡ��������ˣ��ʽ��");
			return;
		end
		MessageBox( "��ʾ","��ǰ��ˣ��ʽ��Ҫ���ģ�"..WANSHUA_USE_MONEY.."����\nȷ��Ҫ���г�����ˣ��" );
		MessageBoxFrame:SetClientString( "����������ˣ" )
	end
end
function ClanPetJiYangWanShua()
	local PetInfo 	= GameClanManager:getClanPetData(nCurIdx-1);
	local curTime 	= getServerTime().sec
	if PetInfo ~= nil and PetInfo.JiYangEnd > 0 and PetInfo.JiYangEnd < curTime then 
		ShowMidTips("�����ѽ�����ֹ����״̬���޷���ˣ��");
		return
	end
	GameClanManager:Clan_Pet_WanShua( nCurIdx - 1,0,2,nPetWidHigh,nPetWidLow );
end
-- ��ˣ���ð�ť
function PetFosterFramePlayBtn2_OnClick()
	if PetFosterPlayFrame:IsShown() then 
		PetFosterPlayFrame:Hide();
	else 
		PetFosterPlayFrame:Show();
	end
end
-- ��ˣ�������
function PetFosterPlayFrameConfirmBtn_OnClick()
	for i=1,2 do 
		local checkBtn = getglobal("PetFosterPlayFrameCheckBtn"..i);
		if checkBtn:GetCheckState() then 
			nWanShuaId = i;
			break;
		end
	end
	PetFosterPlayFrame:Hide();
end
function PetFosterPlayFrameCancleBtn_OnClick()
	PetFosterPlayFrame:Hide();
end
function PetFosterPlayFrameCheckButton_OnClick()
	local curBtn = getglobal( "PetFosterPlayFrameCheckBtn"..this:GetClientID() );
	if curBtn:GetCheckState() then 
		if this:GetClientID() == 1 then 
			PetFosterPlayFrameCheckBtn2:setCheckState(false);
		else 
			PetFosterPlayFrameCheckBtn1:setCheckState(false);
		end
	else 
		curBtn:setCheckState(true);
	end
end
function PetFosterPlayFrame_OnShow()
	PetFosterPlayFrame:SetPoint("center", "$parent", "center", 0, 0 );
	if PetFosterFeedFrame:IsShown() then 
		PetFosterFeedFrame:Hide();
	end
	for i=1,2 do 
		local checkBtn = getglobal("PetFosterPlayFrameCheckBtn"..i);
		checkBtn:setCheckState(false);
	end
	local chooseBtn = getglobal( "PetFosterPlayFrameCheckBtn"..nWanShuaId );
	chooseBtn:setCheckState(true);
end
-- �򹤰�ť
function PetFosterFrameWorkBtn_OnClick()
	local petJiYang = GameClanManager:getClanPetData( nCurIdx - 1 );
	if petJiYang == nil then 
		PetFosterFrame:Hide();
		return;
	end	
	local curTime 	= getServerTime().sec;
	if isMainplayerPet then 
		local WorkBtn = getglobal("PetFosterFrameWorkBtn")
		if  WorkBtn:GetText() == "�򹤽���" then
			local nTme 		= petJiYang.WorkEnd - curTime; 
			if nTme > 0 then 
				ShowMidTips("�����ڽ����У��޷���ý���")
				return
			end
			GameClanManager:Clan_Pet_WorkAward( nCurIdx - 1,nPetWidHigh,nPetWidLow );	
			return;
		end
		if petJiYang.JiYangEnd > 0 and petJiYang.JiYangEnd < curTime then 
			ShowMidTips("�����ѽ�����ֹ����״̬���޷��򹤣�");
			return
		end
		if petJiYang.HappyVal == 0 then 
			ShowMidTips("���ֶ�Ϊ0���ܴ򹤣������Ļ��ֶ�")
			return
		end
		GameClanManager:Clan_Pet_Work( nCurIdx - 1,nPetWidHigh,nPetWidLow );
	else 
		if petJiYang.JiYangEnd > 0 and petJiYang.JiYangEnd < curTime then 
			ShowMidTips("�����ѽ�����ֹ����״̬���޷����գ�");
			return
		end
		if petJiYang.WenBao < 30 then 
			ShowMidTips("�±�ֵ����30ʱ���޷��������ա�")
			return
		end
		local nMoneyCost = nPetTouNum * 100
		MessageBox( "��ʾ","��ǰ���ճ�����Ҫ���ģ�"..nMoneyCost.."����\nȷ��Ҫ�Ըó���������գ�" );
		MessageBoxFrame:SetClientString( "������������" )	
	end
end

function ClanPetJiYangSeduce()
	local MainPlayer = ActorMgr:getMainPlayer();
	local petJiYang = GameClanManager:getClanPetData( nCurIdx - 1 );
	local curTime 	= getServerTime().sec;
	if petJiYang == nil then 
		MessageBoxFrame:Hide();
		return;
	end	
	if petJiYang.JiYangEnd > 0 and petJiYang.JiYangEnd < curTime then 
		ShowMidTips("�����ѽ�����ֹ����״̬���޷����գ�");
		return
	end
	if petJiYang.WenBao < 30 then 
		ShowMidTips("�±�ֵ����30ʱ���޷��������ա�")
		return
	end
	if MainPlayer:getMoney() < ( nPetTouNum * 100 ) then 
		ShowMidTips("���Ҳ��㣬�޷����գ�");
		return;
	end
	GameClanManager:Clan_Pet_Seduce( nCurIdx - 1,nPetWidHigh,nPetWidLow );
end

function PetFosterFrame_OnLoad()
	this:RegisterEvent("GE_CLAN_PET_UPDATE");
	this:setUpdateTime( 1 );
end
function PetFosterFrame_OnEvent()
	if arg1 == "GE_CLAN_PET_UPDATE" then
		UpdateCurClanPetInfo();	
	end
end

local MAX_TALK_SHOW_TIME = 5 * 1000;
function PetFosterFrame_OnUpdate()
	detectClanPetRailingDistance( "PetFosterFrame" );
	
	local petJiYang = GameClanManager:getClanPetData( nCurIdx - 1 );
	if petJiYang == nil then 
		PetFosterFrame:Hide();
		return;
	end
	local curTime 	= getServerTime().sec;
	local JiYangBtn = getglobal("PetFosterFrameConfirmBtn");
	local Font = getglobal("PetFosterFrameJiYangTimeFont");
	local nTme = petJiYang.JiYangEnd - curTime; 
	
	if nTme > 0 then 
		local timeFont = SetDartEndTimeText( nTme );
		Font:SetText( "����ʣ��ʱ�䣺"..timeFont,255,255,190 );
	else 
		if JiYangBtn:IsShown() and not JiYangBtn:IsEnable() then 
			JiYangBtn:Enable();
		end
		Font:SetText( "����ʣ��ʱ�䣺00:00",255,255,190);
	end
	
	Font = getglobal("PetFosterFrameWorkTimeFont")
	if not isMainplayerPet or petJiYang.WorkEnd == 0 then 
		Font:SetText("",255,255,190)
	else
		nTme = petJiYang.WorkEnd - curTime; 
		if nTme > 0 then 
			local timeFont = SetDartEndTimeText( nTme );
			Font:SetText( "��ʣ��ʱ�䣺"..timeFont,255,255,190 );
		else 	
			Font:SetText( "�򹤿��ջ�",255,255,190 );
		end
	end
end
function PetFosterFrame_OnShow()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return; end 
	local petJiYang = GameClanManager:getClanPetData( nCurIdx-1 );
	if petJiYang == nil then 
		PetFosterFrame:Hide();
		return;
	end
	PetFosterFrame:SetPoint("center", "$parent", "center", 0, 0 );
	PetFosterFrameHistoryBtn:Hide()
	UpdateCurClanPetInfo();

	local curTime 	= getServerTime().sec;
	local JiYangBtn = getglobal("PetFosterFrameConfirmBtn");
	local Font 		= getglobal("PetFosterFrameJiYangTimeFont");
	
	local nTme = petJiYang.JiYangEnd - curTime; 
	if not bCurClanType then 
		JiYangBtn:Hide()
	else 
		JiYangBtn:Show();
		if isMainplayerPet then 
			JiYangBtn:Enable()
		else 
			JiYangBtn:Disable()
		end	
	end
	if nTme > 0 then 
		local timeFont = SetDartEndTimeText( nTme );
		Font:SetText( "����ʣ��ʱ�䣺"..timeFont,255,255,190 );
	else 
		if JiYangBtn:IsShown() and not JiYangBtn:IsEnable() then
			JiYangBtn:Enable();
		end
		Font:SetText( "����ʣ��ʱ�䣺00:00",255,255,190 );
	end
end

function PetFosterFrame_OnHide()
	for _, name in ipairs( t_ClanPetFrame ) do
		local frame = getglobal( name );
		if frame:IsShown() then 
			frame:Hide();
		end
	end 
	PetFosterFrameAwardBtnView:Hide()
end

function UpdateCurClanPetInfo()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return; end 
	local petJiYang = GameClanManager:getClanPetData( nCurIdx - 1 );
	if petJiYang == nil then 
		PetFosterFrame:Hide();
		return;
	end	

	local WorkBtn	= getglobal("PetFosterFrameWorkBtn")
	local workTime	= getglobal("PetFosterFrameWorkTimeFont")
	local curTime 	= getServerTime().sec;
	local petPackage = MainPlayer:getPet();
	if petPackage:getPetInfo( petJiYang.PetWid.nHeight,petJiYang.PetWid.nLow ) then 
		isMainplayerPet = true;	
		if petJiYang.WorkEnd > 0 then 
			WorkBtn:SetText( "�򹤽���" );
		else 
			WorkBtn:SetText( "�� ��" );
		end
		local nTme = petJiYang.WorkEnd - curTime; 
		if petJiYang.WorkEnd == 0 then 
			workTime:SetText("",255,255,190);
		elseif nTme > 0 then 
			local timeFont = SetDartEndTimeText( nTme );
			workTime:SetText( "��ʣ��ʱ�䣺"..timeFont,255,255,190 );
		else 	
			workTime:SetText( "�򹤿��ջ�",255,255,190 );
		end
	else 
		WorkBtn:SetText( "�� ��" );
		isMainplayerPet = false;
		workTime:SetText("",255,255,190)
	end
	nPetWidHigh = petJiYang.PetWid.nHeight;
	nPetWidLow	= petJiYang.PetWid.nLow;
	local scale = 0
	local petImpUp	= getPetImpUpDef( petJiYang.Imp );
	if petImpUp ~= nil then 
		local percentVal = math.floor( petJiYang.ImpExp * 100 / petImpUp.ExpUp )
		percentVal = ( petJiYang.ImpExp >= petImpUp.ExpUp ) and 100 or percentVal
		local scale = petJiYang.ImpExp / 100;
		scale = scale > 1 and 1 or scale;
	end
	
	local expTex = getglobal( "PetFosterFrameProgresBtn1Texture" );
	expTex:SetSize( scale * 145, 13 );
	PetFosterFrameProgresBtn1Font:SetText("��ǰ�ۼ�Ǳ����"..petJiYang.ImpExp )
	
	scale = petJiYang.HappyVal / 100;
	scale = scale > 1 and 1 or scale;
	local happyValuTex = getglobal( "PetFosterFrameProgresBtn2Texture" );
	happyValuTex:SetSize( scale * 145, 13 );
	PetFosterFrameProgresBtn2Font:SetText("��ǰ����ֵ��"..petJiYang.HappyVal )

	scale = petJiYang.WenBao / 100;
	scale = scale > 1 and 1 or scale;
	local wenBaoTex = getglobal( "PetFosterFrameProgresBtn3Texture" );
	wenBaoTex:SetSize( scale * 145, 13 );
	PetFosterFrameProgresBtn3Font:SetText("��ǰ�±�ֵ��"..petJiYang.WenBao )
	local happyTex = getglobal( "PetFosterFrameHappyBtnTex2")
	if petJiYang.HappyVal >= 80 then 
		happyTex:SetTexUV( 936,512,81,79 )
	elseif petJiYang.HappyVal >= 60 then 
		happyTex:SetTexUV( 936,443,81,79 )
	elseif petJiYang.HappyVal >= 30 then 
		happyTex:SetTexUV( 547,111,81,79 )
	elseif petJiYang.HappyVal > 0 then 
		happyTex:SetTexUV( 547,190,81,79 )
	else 
		happyTex:SetTexUV( 547,269,81,79 )
	end 
	
	local petDef		= getPetDef( petJiYang.PetID );
	local modelViewer 	= getglobal("PetFosterFrameModelBtnView")
	local path 			= GetModePathById( petDef.MouldID )
	if petJiYang.PetID == 301 then 
		scale = 0.15
	else 
		scale = GetNpcDisplayInfo( petDef.MouldID ).size == 0 and 1 or GetNpcDisplayInfo( petDef.MouldID ).size
	end
	modelViewer:setPetModel( 0, 10100, path );
	modelViewer:setPetScale( 0, scale, scale, scale );
	local t_ModelPos = { [214] = -20,[1214] = -20 }
	if t_ModelPos[petJiYang.PetID] ~= nil then
		modelViewer:setEntityPosition( 0, 0, t_ModelPos[petJiYang.PetID], 0 );
	else
		modelViewer:setEntityPosition( 0, 0, 40, 0 );
	end
	local modelName = getglobal("PetFosterFrameModelBtnFont");
	modelName:SetText( petJiYang.RoleName.."�ĳ���" );
	if PetFosterFrameHistoryBtn:IsShown() then
		UpdatePetHistoryInfo()
	end
end
---------------------�����¼���-----------------------------
local WeiShiType = 1;
local WanShuaType = 2;
local YinYouType = 3;
function PetFosterFrameRecordBtn_OnClick()
	if PetFosterFrameHistoryBtn:IsShown() then 
		PetFosterFrameHistoryBtn:Hide();
	else 
		PetFosterFrameHistoryBtn:Show();
	end
end
function getRecordTypeDes( nType, nValue )
	local szText = ""
	if nType == WeiShiType then 
		szText = "�Գ������ιʳ�����ӳ����±�ֵ"..nValue.."��"
	elseif nType == WanShuaType then 
		szText = "�������ˣ�����ӳ��ﻶ�ֶ�"..nValue.."��"
	elseif nType == YinYouType then 
		szText = "˳�ִ�����"..nValue.."��Ǳ��ֵ";
	end
	return szText;
end

function PetFosterFrameHistoryBtn_OnShow()
	UpdatePetHistoryInfo();	
end

function UpdatePetHistoryInfo()
	local petJiYang = GameClanManager:getClanPetData( nCurIdx-1 );
	if petJiYang == nil then 
		PetFosterFrame:Hide();
		return;
	end	
	local rich = getglobal( "PetFosterFrameHistoryBtnRich" );
	rich:Clear();
	for i=1, petJiYang.OpNum do
		local record = getBonusTime( petJiYang.OpInfo[i-1].Time ) .. "\n#L#cfffebd" ..petJiYang.OpInfo[i-1].RoleName .. "@@player#n#c938259"..getRecordTypeDes( petJiYang.OpInfo[i-1].OpType,petJiYang.OpInfo[i-1].Val).."\n";
		rich:AddText( record, 255, 255, 190 );
	end
	local slider = getglobal( "PetFosterFrameHistoryBtnScrollBar" );
	SetScrollBar( slider:GetName(), rich:GetViewLines(), rich:GetTextLines() );
	slider:SetValue( slider:GetMaxValue() );
end
function PetFosterFrameRecordBtn_OnEnter()
	SetGameTooltips(this:GetName(), "��ʾ�����¼")
end

function PetFosterFrameValueBtnTemplate_OnEnter()
	local szText = ""
	if this:GetClientID() == 1 then 
		szText = "Ǳ��ֵÿ3��������һ��\n����ɳ�Խ�ߣ�ÿ�����ӵ�Ǳ��Խ��"
		SetGameTooltips(this:GetName(), szText)
	elseif this:GetClientID() == 2 then 
		szText = "����������ʼ����ֵΪ0��\n����ֵ��Ӱ�������Ǳ����\n����ֵͨ����ˣ���ӣ�\n����򹤻����Ļ���ֵ��"
		SetGameTooltips(this:GetName(), szText)
	elseif this:GetClientID() == 3 then 
		szText = "�±���Ϊ0ʱ����ֹ���Ǳ����\n�±���Խ�߻��Ǳ��Խ�࣬\n�±���ͨ����ι����ʳ�������ӣ�\n�����κμ�������ιʳ��"
		SetGameTooltips(this:GetName(), szText)
	end
end
function PetFosterFrameFeedBtn_OnEnter()
	local szText = "���ιʳ��ť�����ӳ����±�ֵ��\nĬ��������͵ȼ���ʳ�\nͨ��ι�����ÿ��޸�ι����ʳ��"
	SetGameTooltips(this:GetName(), szText)
end

function PetFosterFrameFeedBtnSet_OnEnter()
	SetGameTooltips(this:GetName(), "ι������")
end

function PetFosterFramePlayBtn_OnEnter()
	local szText = "�����ˣ��ť�����ӳ��ﻶ��ֵ��\n��ˣĬ��������ˣ���ߣ�\n��ͨ����ˣ�����޸���ˣ��ʽ��\nÿ����ˣ�м��ʻ�ñ��佱��"
	SetGameTooltips(this:GetName(), szText)
end

function PetFosterFramePlayBtnSet_OnEnter()
	SetGameTooltips(this:GetName(), "��ˣ����")
end

function PetFosterFrameWorkBtn_OnEnter()
	local szText = "";
	if this:GetText() == "�� ��" then 
		szText = "����򹤰�ť�����￪ʼ�򹤣�\n����ÿ�δ�90���ӣ�\nÿ�δ�����25�㻶��ֵ��\n�򹤽�����ɻ�ô򹤽�����"
		SetGameTooltips(this:GetName(), szText)
	elseif this:GetText() == "�� ��" then 
		szText = "���ճɹ��󣬿�͵ȡһ��Ǳ��ֵ���Լ��ĳ���\nÿֻ����ÿ����Ա�͵5�Σ�ÿ��ֻ��͵ͬһֻ����һ��\n���ֶȵ���50�������պ�����õĽ����ӱ�"
		SetGameTooltips(this:GetName(), szText)
	else 
		SetGameTooltips(this:GetName(), "�򹤽���������ô򹤽���")
	end
end
function PetFosterFrameHistoryScrollUpBtn_OnClick()
	local slider = getglobal( "PetFosterFrameHistoryBtnScrollBar" );
	if not slider:IsShown() then
		return;
	end

	local nValue = slider:GetValue() - slider:GetValueStep();
	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end
function PetFosterFrameHistoryScrollDownBtn_OnClick()
	local slider	= getglobal( "PetFosterFrameHistoryBtnScrollBar" );
	if not slider:IsShown() then
		return;
	end

	local nMaxValue = slider:GetMaxValue();
	local nValue	= slider:GetValue() + slider:GetValueStep();
	
	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end
function PetFosterFrameHistoryScrollBar_OnValueChanged()
	local slider	 = getglobal( "PetFosterFrameHistoryBtnScrollBar" );
	local nValue	 = slider:GetValue();
	local nMaxValue	 = slider:GetMaxValue();
	if nValue <= nMaxValue then
		util.UpdateRich( "PetFosterFrameHistoryBtnRich", nValue );
	end
end
