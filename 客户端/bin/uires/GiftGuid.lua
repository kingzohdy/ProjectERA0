local t_CheckInInfo = {
					[1] = [[
#c431D09˵����
#c6f6149		ÿ�յ�¼��Ϸ�����·��ġ�ÿ��ǩ��������
#c6f6149�������ͨ�����ƻ�ý�����Ʒ��ͬʱǩ����
#c6f6149		ǩ��ÿ���ۼƣ��ۼ�����Խ�ߣ�����Խ�����
#c6f6149ǩ����ϣ������¼����ۼ�������]],
					[2] = [[
#c431D09˵����  								
#c6f6149		�ڽ���̳ǹ��򡰳ɳ�������Ȩ�������ȡ100��
#c6f6149ȯ��ÿ�ջ��𣬹������Ʒ��ֱ�ӻ�ô���Ȩ������
#c6f6149ʹ����Ʒ��
#c6f6149		ÿ�տ����������ȡ�����ߵ���·��������ť��
#c6f6149����ȡ���档
#c6f6149		ÿ�տ���ȡ100��ȯ������30�졣]],
					 	}
--------------------------------���־����---------------------------------------------
--ÿ�� ��ѡ�
local t_ActivityLoginInfo = 
	{
		[1] = { 
				[1] = { ActiveID = 19 ,StarNum = 2}
			  },
		[2] = { 
				[1] = { ActiveID = 52 ,StarNum = 4},
				[2] = { ActiveID = 17 ,StarNum = 2},
			  },
		[3] = { 
				[1] = { ActiveID = 22 ,StarNum = 2},
				[2] = { ActiveID = 11 ,StarNum = 3},
				[3] = { ActiveID = 19 ,StarNum = 2},
			  },
		[4] = { 
				[1] = { ActiveID = 52 ,StarNum = 4},
				[2] = { ActiveID = 17 ,StarNum = 2} 
			  },
		[5] = { 
				[1] = { ActiveID = 11 ,StarNum = 3},
				[2] = { ActiveID = 22 ,StarNum = 2},
				[3] = { ActiveID = 19 ,StarNum = 2},
			  },
		[6] = { 
				[1] = { ActiveID = 16 ,StarNum = 5},
				[2] = { ActiveID = 27 ,StarNum = 2},
			  },
		[7] = { 
				[1] = { ActiveID = 48 ,StarNum = 3},
				[2] = { ActiveID = 30 ,StarNum = 1},
			  },
	}
--ÿ�� ��ѡ�
local t_ActivityNeedInfo ={
		[1] = { ActiveID = 6 ,StarNum = 4},
		[2] = { ActiveID = 38 ,StarNum = 4},
	}
function ActivityLogFrame_OnLoad()

end

function ActivityLogFrame_OnEvent()

end

function ActivityLogFrame_OnShow()
	local nBtnIndex = 1;
	for k=1,5 do 
		local btn = getglobal("ActivityLogFrameActivityListBtn"..k);
		btn:Hide();
	end
	for m=1,#t_ActivityNeedInfo do 
		SetActivityListInfo(t_ActivityNeedInfo[m]["ActiveID"],nBtnIndex,t_ActivityNeedInfo[m]["StarNum"],false);
		nBtnIndex = nBtnIndex + 1;
	end
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end
	if MainPlayer:getLv() >= 65 then 
		-- ����65 ��ɽ������
		SetActivityListInfo( 3,nBtnIndex,4,false);
		SetActivityListInfo( 46,nBtnIndex + 1,4,false);
		nBtnIndex = nBtnIndex + 2;
	else 
		-- С��65 ����
		SetActivityListInfo( 1,nBtnIndex,4,false);
		nBtnIndex = nBtnIndex + 1;
	end
	-- �Ǳ�����ѡ��ʾ
	for i=1,7 do
		if GamePromotion:isWeekToday(i-1) then
			local weekIndex = i-1;
			if i == 1 then
				weekIndex = 7;	
			end
			for j=1,#t_ActivityLoginInfo[weekIndex] do 
				SetActivityListInfo( t_ActivityLoginInfo[weekIndex][j]["ActiveID"] ,nBtnIndex ,t_ActivityLoginInfo[weekIndex][j]["StarNum"],true);
				nBtnIndex = nBtnIndex + 1;	
			end
		end
	end
	SetActivityListTexture();
end
function SetActivityListInfo( nRow, nBtnIndex,nStarNum, bTexShow )
	if nRow <= 0 then
		return;
	end	
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end
	local btn 		= getglobal("ActivityLogFrameActivityListBtn"..nBtnIndex);
	if btn == nil then 
		return;
	end
	local tex	 	= getglobal( btn:GetName().."Icon" );
	local nameFont 	= getglobal( btn:GetName().."NameFont" );
	local timeFont 	= getglobal( btn:GetName().."TimeFont" );
	local levelFont	= getglobal( btn:GetName().."LevelFont" );
	local promotionData = GamePromotion:getPromotionData( nRow );
	local nStartInfo 	= GamePromotion:getCurrPromitionStartInfo( nRow );
	local nTimeIndex 	= nStartInfo.timeIdx;
	
	if nStartInfo.startFlag == PROMOTION_PASSED or nStartInfo.startFlag == PROMOTION_NOTTODAY then
		nTimeIndex = 0;
	end
	if nStartInfo.startFlag ~= PROMOTION_RUNING then
		timeFont:SetTextColor( 255, 0, 0 );
	else 
		timeFont:SetTextColor( 255,255,190 );
	end
	local OpenTime = promotionData.m_Time[nTimeIndex].m_szStartTime .. "-" .. promotionData.m_Time[nTimeIndex].m_szEndTime;
	if (promotionData.m_Time[nTimeIndex].m_szStartTime == "" or promotionData.m_Time[nTimeIndex].m_szStartTime == "00:00" ) 
		and promotionData.m_Time[nTimeIndex].m_szEndTime== "24:00" then
		OpenTime = "ȫ��";
	end
	timeFont:SetText( OpenTime );
	nameFont:SetText( GamePromotion:getDisPlayName( nRow ) );
	levelFont:SetText( "�ȼ�����".. promotionData.m_nLevel );
	if MainPlayer:getLv() <	promotionData.m_nLevel then
		levelFont:SetTextColor( 255,0,0 );
	else 
		levelFont:SetTextColor( 255,255,190 );
	end 
	-- ��������
	for i=1,5 do
		local starTex = getglobal( btn:GetName().."StarTex"..i );
		starTex:Hide();
	end
	for i=1,nStarNum do
		local starTex = getglobal( btn:GetName().."StarTex"..i );
		starTex:Show();
	end
	-- �ش�ʱ�䡢���� ͼƬ����
	if nStarNum == 2 and bTexShow then 
		tex:SetTexture( "uires\\ZhuJieMian\\3.dds" );
		tex:SetTexUV( 295, 135, 69, 32 );
		tex:Show();
	elseif nStarNum > 3 and bTexShow then
		tex:SetTexture( "uires\\ZhuJieMian\\3.dds" );
		tex:SetTexUV( 225, 135, 69, 32 );
		tex:Show();
	else 
		tex:Hide();
	end
	btn:Show();
end

function SetActivityListTexture()
	for i=1, 2 do
		local path 	= "resource\\ad\\Activity" .. i .. ".jpg";
		local tex	= getglobal("ActivityLogFrameBusynessListBtn"..i.."Tex");
		if not GameMgr:CanFindFile( path ) then
			tex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga")
		else 
			tex:SetTexture(path);
		end
	end
end
function BusynessListBtn_OnClick()
	local index = this:GetClientID();
	local path = "resource\\ad\\Activity" .. index .. ".ini";
	MarketManager:openAdLink( path );
end
function ActivityLogFrameLogBtn_OnClick()
	ActivityShowBtn_nClick();
	ActivityLogFrame:Hide();
end
---------------------------------------���ƽ�������----------------------------------------------------
local nCheckInDays = 0;
local nSendDays	   = 0;	
local nCheckInItem = 0;
local nCheckInNum = 0;
local nPreItemID 	= 0;
local t_CheckInItemIndex = {};
local MAX_CONT_COVER_GIFT_ITEM = 15;
function GiftCenterGuideCheckInButton_OnShow()
	
end

function GiftCenterGuideContinueCheck( nItemID, nItemNum, nDayNum, nDayTime,nPreId )
	local maxDays	= GameWizard:getContCoverMaxDays();
	nCheckInDays 	= nDayNum > maxDays and maxDays or nDayNum;
	nPreItemID		= nPreId;
	GiftCenterGuideFrame:Show();
	nSendDays		= nDayNum;
	local checkBtn 	= getglobal("GiftCenterGuideCheckInButton");
	local button	= getglobal("SystemSelectFrameConRewardBtn");
	local todayFont = getglobal("SystemSelectFrameContinueFont2");
	local nextFont 	= getglobal("SystemSelectFrameContinueFont4");
	local numFont	= getglobal("FlopRewardFrameNumFont");
	todayFont:SetText( "�Ѿ���������"..nCheckInDays.."��" );
	numFont:SetText( "��ǩ������ "..nCheckInDays.."/"..maxDays );
	local nDays = nDayNum >= maxDays and maxDays or nDayNum + 1 ;
	nextFont:SetText("��������"..nDays.."�콱��");

	if nItemID == 0 then
		-- ǩ��ǰ
		if isSameDay( nDayTime ) then
			checkBtn:Hide();
			button:Disable();
		else
			checkBtn:Show();
			local uvTexture = getglobal("GiftCenterGuideCheckInButtonUVAnimationTex");
			uvTexture:SetUVAnimation( 50, true );
			uvTexture:Show();
			button:Enable();
		end
	else 
		-- ǩ����
		local uvTexture = getglobal("GiftCenterGuideCheckInButtonUVAnimationTex");
		uvTexture:Hide();
		checkBtn:Hide();
		button:Disable();
		nCheckInItem	= nItemID;
		nCheckInNum		= nItemNum;
		SetCheckInItemInfo();
		StartCheckOpenUVAnimation();
	end
	BondingGifCheckInBtnDesc();
end
function FlopRewardOpenTemplateItemBtn_OnEnter()
	
end
function FlopRewardOpenTemplateItemBtn_OnLeave()

end

local ITEM_SHOW_Time = 3 * 1000;
local BtnIndex 	= 0;       -- ѡ�п��� ��ť
-- ������Ч����
local selIndex 		= 1;
local selStart  	= false;
local otherIndex 	= 1;
local otherStart 	= false;

function FlopRewardOpenTemplateOpenBtn_OnClick()
	if 	BtnIndex > 0 then 
		return;
	end 
	local btn 	= getglobal(this:GetParent())
	BtnIndex 	= btn:GetClientID();
--	this:Hide();
	t_CheckInItemIndex = {1,2,3,4,5};
	GameWizard:contGiftCheckIN( nSendDays + 1 );
end
function StartCheckOpenUVAnimation()
	selIndex = 1;
	selStart = true;
	for i=1, nCheckInDays do 
		local tex = getglobal("FlopRewardFrameCheckTex"..i);
		if tex ~= nil then
			tex:SetGray( false );	
		end
	end
end
function SetCheckInItemInfo()
	local itemDef 	= getItemDef( nCheckInItem );
	local IconPath 	= GetItemEquipIconPath()..itemDef.IconID..".tga";
	-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
	if not IsInExistence( IconPath ) then
		IconPath = DEFAULT_ITEM_PATH;
	end
	
	local selectbtn = getglobal( "FlopRewardFrameItemBtn"..BtnIndex );
	local itemTex	= getglobal(selectbtn:GetName().."ItemIcon");
	local rich		= getglobal(selectbtn:GetName().."ItemFont");
	local szText 	= itemDef.Name.." * "..nCheckInNum;
	rich:SetText(szText,255,255,255);
	itemTex:SetTexture(IconPath);
	local texiaoTex = getglobal("FlopRewardFrameItemBtn"..BtnIndex.."ItemTexiao");
	texiaoTex:Show();
	SetOtherRandItemInfo();
end
-- ������������
function SetOtherRandItemInfo()
	local giftDef 	= GameWizard:getContCoverGiftDef( nCheckInDays );
	local maxDays	= GameWizard:getContCoverMaxDays();
	for j=1, MAX_CONT_COVER_GIFT_ITEM do
		local itemID = giftDef.Items[j-1].ItemID;
		if itemID == 0 then 
			break;
		end
		if itemID == nCheckInItem then 
			if nCheckInDays >= maxDays then 
				local nIndex = 1;	
				if itemID ~= 5600222 then 
					table.remove( t_CheckInItemIndex,j );
					nIndex = math.random( 1,#t_CheckInItemIndex );
					table.remove( t_CheckInItemIndex,nIndex );
					table.insert( t_CheckInItemIndex,6 );
				else 
					nIndex = math.random( 1,#t_CheckInItemIndex );
					table.remove( t_CheckInItemIndex,nIndex );
				end
			else 
				table.remove( t_CheckInItemIndex,j );	
			end
		end
	end
	for i=1, 5 do 
		if i ~= BtnIndex then 
			local index 	= math.random( 1,#t_CheckInItemIndex );
			local itemDef 	= getItemDef( giftDef.Items[t_CheckInItemIndex[index]-1].ItemID );
			local IconPath 	= GetItemEquipIconPath()..itemDef.IconID..".tga";
			-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end	
			local btn 		= getglobal("FlopRewardFrameItemBtn"..i);
			local itemTex	= getglobal(btn:GetName().."ItemIcon");
			local rich		= getglobal(btn:GetName().."ItemFont");
			local szText 	= itemDef.Name.."*"..giftDef.Items[index-1].Num;
			rich:SetText(szText,255,255,255);
			itemTex:SetTexture(IconPath);
			table.remove( t_CheckInItemIndex,index );
			local texiaoTex = getglobal("FlopRewardFrameItemBtn"..i.."ItemTexiao");
			texiaoTex:Hide();
		end	
	end
end
function FlopRewardFrameCloseBtn_OnClick()
	FlopRewardFrame:Hide();	
end
function FlopRewardFrame_OnLoad()
	this:setUpdateTime(0);
	local TipFont = getglobal("FlopRewardFrameTipFont");
	TipFont:setCenterLine(true);
	TipFont:SetText("#G��Ʒ�󽱣� #cFF4e00���������Ȩ��",255,255,190);
end
function FlopRewardFrame_OnEvent()

end
function FlopRewardFrame_OnShow()
	BtnIndex = 0;
	ClearFlopRewardCheckTex();	
	if nCheckInDays < 1 then 
		return;
	end
	for i=1, nCheckInDays do 
		local tex = getglobal("FlopRewardFrameCheckTex"..i);
		if tex ~= nil then 
			tex:SetGray( false );	
		end
	end
end
function FlopRewardFrame_OnHide()
	selStart 	= false;
	otherStart 	= false;
	selIndex 	= 1;
	otherIndex 	= 1;
end
function FlopRewardFrame_OnUpdate()
	if selStart then 
		ShowCardOpenUVAnimation( BtnIndex,selIndex);
		selIndex = selIndex + 1;
	end
	if selIndex > 50 then 
		selStart 	= false;
		otherStart 	= true;
	end
	
	if otherStart then 
		for i=1,5 do 
			if i ~= BtnIndex then
				ShowCardOpenUVAnimation( i,otherIndex);
			end		
		end
		otherIndex = otherIndex + 1;
	end
	if otherIndex > 50 then 
		selStart 	= false;
		otherStart 	= false;
	end
end
function ShowCardOpenUVAnimation( nBtn,timeIndex )
	local btn 		= getglobal("FlopRewardFrameItemBtn"..nBtn.."OpenUvBtn")
	local tex 		= getglobal(btn:GetName().."HightCenter");
	local uvTex 	= getglobal(btn:GetName().."HightAnim");
	local backBtn 	= getglobal("FlopRewardFrameItemBtn"..nBtn.."OpenBackBtn");
	backBtn:Hide();
	if timeIndex > 50 then 
		btn:Hide();
		return;
	end
	btn:Show();
	local height = 98 - timeIndex * 2;
	if height < 0 then
		return;
	end
	tex:Show();
	uvTex:Show();
	uvTex:SetPoint("top", "FlopRewardFrameItemBtn"..nBtn.."OpenUvBtn", "top", -4, height)
	tex:SetTexture( "uires\\ZhuJieMian\\4.dds" );
	tex:SetTexUV( 154, 0, 76, height );
	tex:SetSize(76,height)
end
function ClearFlopRewardCheckTex()
	for i=1,5 do
		local tex 		= getglobal("FlopRewardFrameCheckTex"..i);
		local backBtn 	= getglobal("FlopRewardFrameItemBtn"..i.."OpenBackBtn");
		local uvBtn 	= getglobal("FlopRewardFrameItemBtn"..i.."OpenUvBtn");
		uvBtn:Hide();
		tex:SetGray( true );
		backBtn:Show();
		
		local centerTex = getglobal(uvBtn:GetName().."HightCenter");
		local uvTex 	= getglobal(uvBtn:GetName().."HightAnim");
		uvTex:SetPoint("top", uvBtn:GetName(), "top", -4, 98);
		centerTex:SetTexture( "uires\\ZhuJieMian\\4.dds" );
		centerTex:SetTexUV( 154, 0, 76, 101 );
		centerTex:SetSize(76,101)
	end
end
-- "��"���Ǳ� ǩ����������
function SetCheckInGiftInfo()
	GuideGiftFrame_:Hide()
	GuideGiftFrameCheckInGiftBtn:Show();
	GuideGiftFrameBackBtnAwardNameFont:SetText("");
	local nameFont	= getglobal("GuideGiftFrameCheckInGiftBtnName");
	local coverBtn 	= getglobal("GuideGiftFrameCheckInGiftBtnCheck");
	local itemBtn	= getglobal("GuideGiftFrameCheckInGiftBtnItemBtn");
	local icon		= getglobal( itemBtn:GetName().."IconTexture");
	local font1		= getglobal("GuideGiftFrameCheckInGiftBtnFont1");
	local font2		= getglobal("GuideGiftFrameCheckInGiftBtnFont2");
	local font3 	= getglobal("GuideGiftFrameCheckInGiftBtnFont3");
	local font4 	= getglobal("GuideGiftFrameCheckInGiftBtnFont4");
	local rich		= getglobal("GuideGiftFrameCheckInGiftBtnDescRich")
	nameFont:SetText("����ǩ��������ȡ");
	coverBtn:SetText("ǩ��");
	coverBtn:Enable();
	local itemDef 	= getItemDef( nPreItemID );
	itemBtn:SetClientUserData( 0,nPreItemID );
	rich:SetText(t_CheckInInfo[1],255,255,255);
	local nDays 	= nCheckInDays;
	if GiftCenterGuideCheckInButton:IsShown() then 
		coverBtn:Show();
		itemBtn:Hide();
		font1:SetText("�����Ƿ�ǩ������");
		font2:SetText( "����ǩ���գ�"..nDays );
		font3:SetText("����ǩ��5���л����ã����������Ȩ��");
		font4:SetText("");
	else 
		coverBtn:Hide();
		itemBtn:Show();
		font1:SetText("�����Ƿ�ǩ������");
		font2:SetText( "����ǩ���գ�"..nDays );
		font3:SetText("������Ʒ��"..itemDef.Name);
		font4:SetText("����ǩ��5���л����ã����������Ȩ��")
		local IconPath 	= GetItemEquipIconPath()..itemDef.IconID..".tga";
		-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH;
		end	
		icon:SetTexture(IconPath);
		SetEquipImportantEffectUV( itemDef, itemBtn:GetName().."UVAnimationTex" );
	end
end

---------------------------------------�鿴��������----------------------------------------------------

function CheckRewarBtnTemplate_OnEnter()
	if this:GetClientUserData(0) > 0 then 
		if getItemDef(this:GetClientUserData(0)) ~= nil then
			ShowItemTips(LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, getItemDef(this:GetClientUserData(0)));
		end
	end
end
function CheckInRewardFrameCancleBtn_OnClick()
	CheckInRewardFrame:Hide();
end
function CheckInRewardFrame_OnShow()
	for i=1,10 do 
		local tex = getglobal("CheckInRewardFrameItemBtn"..i.."IconTexture");
		tex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
		local font = getglobal("CheckInRewardFrameItemBtn"..i.."CountFont")
		font:SetText("");
	end	
	local maxDays = GameWizard:getContCoverMaxDays();
	local nDays = nCheckInDays >= maxDays and maxDays or nCheckInDays + 1 ;

	local GiftDef = GameWizard:getContCoverGiftDef( nDays )
	for i=1,MAX_CONT_COVER_GIFT_ITEM do 
		local btn	 = getglobal("CheckInRewardFrameItemBtn"..i);
		local tex 	= getglobal("CheckInRewardFrameItemBtn"..i.."IconTexture");
		local Font 	= getglobal("CheckInRewardFrameItemBtn"..i.."CountFont")
		if tex == nil then 
			break;
		end
		if GiftDef == nil then 
			break;
		end
		if GiftDef.Items[i-1].ItemID > 0 then
			local itemDef = getItemDef(GiftDef.Items[i-1].ItemID);
			local IconPath 	= GetItemEquipIconPath()..itemDef.IconID..".tga";
			-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end	
			tex:SetTexture(IconPath);
			btn:SetClientUserData( 0,GiftDef.Items[i-1].ItemID );
			SetEquipImportantEffectUV( itemDef, btn:GetName().."UVAnimationTex", item );
			if GiftDef.Items[i-1].Num > 1 then 
				Font:SetText( GiftDef.Items[i-1].Num );
			end
		else 
			break;
		end
	end
	local TipFont = getglobal("CheckInRewardFrameTipFont");
	TipFont:SetText("��������"..nDays.."�죬�м��ʻ��������ƷŶ��")
end

---------------------------------------�����������------------------------------------------------------
local RoleGiftEndTime 	= 0;
local NewRoleID		 	=0;
local MAX_NEW_ROLE_GIFT_ITEM = 5;
local MysShowFlag = false;
local enterMapFlag = false;
function SetNewGiftInfo( newRole, endTime, ID )
	local Tex 		= getglobal("DrawMysRewardFrameTexture");
	local timeBtn 	= getglobal("DrawMysRewardFrameTimeBtn");
	local timeFont 	= getglobal(timeBtn:GetName().."TimeFont");
	local modelview = getglobal("DrawMysRewardFrameGiftBtnModelView");
	NewRoleID = ID;
	if newRole ~= 0 then 
		RoleGiftEndTime = endTime;
		DrawMysRewardFrame:Show();
		MysShowFlag = true;
		local nCurtime 	= getServerTime().sec;
		if nCurtime > endTime then 
			Tex:Show();
			modelview:setPetModel(0,0,"effect\\entity\\libao.ent");
			modelview:setEntityPosition(0,0,100,0);
			modelview:Show();
			timeBtn:Hide();			
		else 
			Tex:Hide();
			modelview:Hide();
			timeBtn:Show();
			local szText = "";
			szText = SetEndTimeText(  endTime - nCurtime );
			timeFont:SetText(szText);
		end
	else 
		MysShowFlag = false;
		DrawMysRewardFrame:Hide();
	end
end
function ShowMysRewardGiftFrame()
	if MysShowFlag and enterMapFlag and not DrawMysRewardFrame:IsShown() then 
		DrawMysRewardFrame:Show();
	end
end
function DrawMysRewardFrame_OnShow()

end
function DrawMysRewardFrame_OnHide()
	local modelview = getglobal("DrawMysRewardFrameGiftBtnModelView");
	modelview:Hide();
end
local pointFlag = 1;
function DrawMysRewardFrameOnUpdate()
	local texture = getglobal("DrawMysRewardFrameTexture");
	if texture:IsShown() then 
		if pointFlag == 1 then 
			texture:SetPoint("topleft", "DrawMysRewardFrame", "topleft", 9, 3);
			pointFlag = 0;
		else 
			texture:SetPoint("topleft", "DrawMysRewardFrame", "topleft", 9, 5);
			pointFlag = 1;
		end
	end	
end
function SetEndTimeText( nTime )
	local szText = "";
	local hour 		=  math.floor( nTime / 3600 );
	local minute 	=  math.floor( nTime / 60 ) ;
	local second 	= nTime;
	if hour > 0 then
		minute = math.floor( ( nTime - hour * 3600 )/60 );
		second = nTime - hour * 3600 - minute* 60;
		szText = szText..hour.."Сʱ"..minute.."��"..second.."��";
	elseif minute > 0 then 
		second = nTime - minute* 60;
		szText = szText..minute.."��"..second.."��";
	else 
		szText = szText..second.."��";
	end
	return szText;
end
function DrawMysRewardFrameTimeBtn_OnUpdate()
	local Tex 		= getglobal("DrawMysRewardFrameTexture");
	local timeBtn 	= getglobal("DrawMysRewardFrameTimeBtn");
	local timeFont 	= getglobal(timeBtn:GetName().."TimeFont");
	local modelview = getglobal("DrawMysRewardFrameGiftBtnModelView");
	local nCurtime 	= getServerTime().sec;
	if nCurtime > RoleGiftEndTime then 
		Tex:Show();
		timeBtn:Hide();
		modelview:setPetModel(0,0,"effect\\entity\\libao.ent");
		modelview:setEntityPosition(0,0,100,0);
		modelview:Show();
	else 
		Tex:Hide();
		timeBtn:Show();
		local szText = "";
		szText = SetEndTimeText( RoleGiftEndTime - nCurtime );
		timeFont:SetText(szText);
	end	
end

function DrawMysRewardFrame_OnLoad() 
	this:setUpdateTime(0.5);
	this:RegisterEvent("GE_ENTER_MAP");
end

function DrawMysRewardFrame_OnEvent()
	if(arg1 == "GE_ENTER_MAP") then
		if GameMgr:isInPlayCamerAnim() then 
			enterMapFlag = true;
		else 
			enterMapFlag = false;
		end
	end
end

function DrawMysRewardFrameGiftBtn_OnClick()
	if DrawMysRewardFrameTimeBtn:IsShown() then 
		return;
	end
	MysRewardFrame:Show();
end
-- �鿴���ؽ�����Ʒ
function MysRewardFrame_OnShow()
	for i=1,5 do 
		local tex = getglobal("MysRewardFrameItemBtn"..i.."IconTexture");
		tex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
		local numFont = getglobal("MysRewardFrameItemBtn"..i.."CountFont");
		numFont:SetText("");
	end	
	local newDef = GameWizard:getNewRoleGiftDef( NewRoleID );
	for i=1,MAX_NEW_ROLE_GIFT_ITEM do 
		local btn 	= getglobal("MysRewardFrameItemBtn"..i);
		local tex 	= getglobal("MysRewardFrameItemBtn"..i.."IconTexture");
		local Font 	= getglobal("MysRewardFrameItemBtn"..i.."CountFont");
		if newDef.Items[i-1].ItemID > 0 then
			local itemDef = getItemDef(newDef.Items[i-1].ItemID);
			local IconPath 	= GetItemEquipIconPath()..itemDef.IconID..".tga";
			-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end	
			tex:SetTexture(IconPath);
			btn:SetClientUserData( 0,newDef.Items[i-1].ItemID );
			SetEquipImportantEffectUV( itemDef, btn:GetName().."UVAnimationTex", item );
			if newDef.Items[i-1].ItemNum > 1 then
				Font:SetText(newDef.Items[i-1].ItemNum);
			end
		else 
			break;
		end
	end
end
--��ȡ���ؽ���
function MysRewardFrameOKBtn_OnClick()
	GameWizard:getNewRoleGift( NewRoleID );
	MysRewardFrame:Hide();
end
---------------------------------------�����ť����------------------------------------------------------
function GiftCenterGuideButtonTemplate_OnEnter()
	local szText = "";
	if this:GetClientID() == 1 then 
		szText = "ǩ�����";
	elseif this:GetClientID() == 2 then 
		szText = "�ȼ����";
	elseif this:GetClientID() == 3 then 
		szText = "ÿ�ճɳ�����";
	end
	SetGameTooltips(this:GetName(),szText);
end
-- ǩ�������ť
function GiftCenterGuideCheckInButton_OnClick()
	FlopRewardFrame:Show();
	local uvTexture = getglobal("GiftCenterGuideCheckInButtonUVAnimationTex");
	uvTexture:Hide();
end
-- �ɳ������ť����
function GiftCenterGuideLevelButton_OnClick()
	local Frame= getglobal( "GuideGiftFrame" );
	if (Frame:IsShown()) then
		Frame:Hide();
	else
		Frame:Show();
		GuideGiftFrame_GrowBtn_OnClick();
	end
	local uvTexture = getglobal("GiftCenterGuideLevelButtonUVAnimationTex");
	uvTexture:Hide();
end
function GiftCenterGuideAwardMTButton_OnClick()
	UpdateGiftCenterGuideAwardMT();
end
function ShowLvGiftFrameBtn( nLvNum )
	if not GiftCenterGuideFrame:IsShown() then 
		GiftCenterGuideFrame:Show();
	end
	if nLvNum > 0 then 
		if GiftCenterGuideLevelButton:IsShown() then 
			return;
		end
		GiftCenterGuideLevelButton:Show();
		local uvTexture = getglobal("GiftCenterGuideLevelButtonUVAnimationTex");
		uvTexture:SetUVAnimation( 50, true );
		uvTexture:Show();
	else 
		if not GiftCenterGuideLevelButton:IsShown() then 
			return;
		end
		local uvTexture = getglobal("GiftCenterGuideLevelButtonUVAnimationTex");
		uvTexture:Hide();
		GiftCenterGuideLevelButton:Hide();
	end
end

------------------------------------------------��ȯ��ȡ���------------------------------
local nDays = 0;
local nMTTime = 1;
function SetAwardMTGiftInfo( nTime,nEndTime,nValue )
	nMTTime = nEndTime;
	local button 	= getglobal("GiftCenterGuideAwardMTButton")
	local uvTexture = getglobal("GiftCenterGuideAwardMTButtonUVAnimationTex");
	if MarketManagerFrame:IsShown() then 
		MarketManagerFrame_ShowMarketItem();
	end
	if nEndTime <= 0 then 
		button:Hide();
		uvTexture:Hide();
		nDays = 0;
		return;
	end
	local nEndHour 	= tonumber( os.date("%H",nEndTime) );
	local nEndMin 	= tonumber( os.date("%M",nEndTime) );
	local nEndSec	= tonumber( os.date("%S",nEndTime) );
	--print("nEndHour and nEndMin and nEndSec are ",nEndHour,nEndMin,nEndSec);
	local nCurtime 	= getServerTime().sec;
	local nSixEndTime	= 0;
	if nEndHour >= 6 then
		nSixEndTime = nEndTime - ( nEndHour - 6 )* 3600 - nEndMin*60 - nEndSec; 
	else 
		nSixEndTime = nEndTime - ( nEndHour + 16 )* 3600 - nEndMin*60 - nEndSec; 
	end
	--print("nSixEndTime and nMTTime are ",nSixEndTime,nMTTime);
	if nCurtime >= nEndTime then 
		button:Hide();
		uvTexture:Hide();
		nDays = 0;
	else 
		local nLeftTime = nSixEndTime - nCurtime;
		if nLeftTime > 0 then
			nDays = math.ceil( nLeftTime / ( 3600 * 24 ) ) + 1;
		else 
			nDays = 1;
		end
		if isSameDay( nTime ) then 
			-- ͬһ��
			--print("�ɳ�����ͬһ��",nTime)
			button:Hide();
			uvTexture:Hide();
			nDays = nDays - 1;	
		else 
			-- ��ͬ��
			--print("�ɳ�����ͬ��",nTime)
			button:Show();
			uvTexture:SetUVAnimation( 50, true );
			uvTexture:Show();
		end
	end
	BondingGifCheckInBtnDesc();
end

function GiftCenterGuideAwardMTButton_OnUpdate()
	local nCurtime 	= getServerTime().sec;
	local button 	= getglobal("GiftCenterGuideAwardMTButton")
	local uvTexture = getglobal("GiftCenterGuideAwardMTButtonUVAnimationTex");
	if nCurtime >= nMTTime then 
		button:Hide();
		uvTexture:Hide();
		nDays = 0;
	else 
	--	local nTime = nMTTime - nCurtime;
	--	nDays = math.ceil( nTime / ( 3600 * 24 ) );
	end
	if GuideGiftFrame:IsShown() then
		BondingGifCheckInBtnDesc();
	end
end

function SetAwardMTGiftDetailInfo()
	GuideGiftFrame_:Hide()
	GuideGiftFrameCheckInGiftBtn:Show();
	GuideGiftFrameBackBtnAwardNameFont:SetText("");
	local nameFont	= getglobal("GuideGiftFrameCheckInGiftBtnName");
	nameFont:SetText("ÿ�ս�ȯ��ȡ");
	local coverBtn 	= getglobal("GuideGiftFrameCheckInGiftBtnCheck");
	coverBtn:Show();
	
	local itemBtn	= getglobal("GuideGiftFrameCheckInGiftBtnItemBtn");
	itemBtn:Hide();
	local font1		= getglobal("GuideGiftFrameCheckInGiftBtnFont1");
	local font2		= getglobal("GuideGiftFrameCheckInGiftBtnFont2");
	local font3 	= getglobal("GuideGiftFrameCheckInGiftBtnFont3");
	local font4 	= getglobal("GuideGiftFrameCheckInGiftBtnFont4");
	local rich		= getglobal("GuideGiftFrameCheckInGiftBtnDescRich");
	coverBtn:SetText("��ȡ");
	rich:SetText(t_CheckInInfo[2],255,255,255);
	font4:SetText("");
	if GiftCenterGuideAwardMTButton:IsShown() then	
		coverBtn:Enable();
		font1:SetText( "�����Ƿ�����ȡ���𣺷�" );
		font2:SetText( "��ȡ������100��ȯ" );
		font3:SetText( "ʣ�����ȡ��ȯ������"..nDays.."��" );
	else 
		local curTime = getServerTime().sec;
		if curTime > nMTTime then 
			font1:SetText("��ǰδ����ÿ�ճɳ�����");
			font2:SetText("ʣ�����ȡ��ȯ������"..nDays.."��");
			font3:SetText("");	
		else 
			font1:SetText("�����Ƿ�����ȡ������");
			font2:SetText("��ȡ������100��ȯ" );
			font3:SetText("ʣ�����ȡ��ȯ������"..nDays.."��");
		end
		coverBtn:Disable();
	end	
end

function GetAwardMTGift()
	if GiftCenterGuideAwardMTButton:IsShown() then
		GameWizard:sendAwardMTGift();
	else 
		ShowMidTips("��״̬�޷���ȡ��ȯ");
	end
end

function getCurMTRewardState()
	local curTime  = getServerTime().sec;
	if curTime >= nMTTime then 
		return false;
	else 
		return true;
	end
end
-------------------------------------------����֮��-������ť-------------------------------
function SetClanWarAwardGift()
	GiftCenterGuideClanWarButton:Show()
end
function GiftCenterGuideClanWarButton_OnClick()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer then
		mainplayer:requestClanWarAward()
	end
	this:Hide()
end
function GiftCenterGuideClanWarButton_OnClick_OnEnter()
	SetGameTooltips( this:GetName(), "����֮�ǽ���" )
end
-------------------------------------------�·������ȡ���---------------------------------
local t_GiftCenterGuidList = {
	"GiftCenterGuideCheckInButton",
	"GiftCenterGuideLevelButton",
	"GiftCenterGuideAwardMTButton",
	"GiftCenterGuideClanWarButton",
}

function GiftCenterGuideFrame_OnUpdate()
	local offsetX = 0
	for i = 1, table.getn( t_GiftCenterGuidList ) do
		local btn = getglobal( t_GiftCenterGuidList[i] )
		if btn:IsShown() then
			btn:SetPoint( "topleft", "GiftCenterGuideFrame", "topleft", offsetX, 0 )
			offsetX = offsetX + 44
		end
	end
end
