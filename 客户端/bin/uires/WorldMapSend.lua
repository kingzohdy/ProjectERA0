local transList	=	{};

local t_MapIdToBtnId = {	
							[3400]	= { ButtonId = 1,	name = "ʮ���ſ�",	content = "44~59��",	state ="nil"},
							[2400]	= { ButtonId = 2,	name = "���ջ�Ұ",	content = "40~44��",	state ="nil"},
							[2000]	= { ButtonId = 3,	name = "��յ�",	content = "1~10��",	state ="nil"},
							[1000]	= { ButtonId = 4,	name = "ʱ���",	content = "����",	state ="nil"},
							[2200]	= { ButtonId = 5,	name = "����ѩԭ",	content = "20~29��",	state ="nil"},
							[2201]	= { ButtonId = 6,	name = "����ѹ�",	content = "70~73��",	state ="nil"},
							[3200]	= { ButtonId = 7,	name = "�γ�",		content = "74~77��",	state ="nil"},
							[3100]	= { ButtonId = 8,	name = "����ʥ��",	content = "63~66��",	state ="nil"},
							[2104]	= { ButtonId = 9,	name = "���ֵ�",	content = "60~63��",	state ="nil"},
							[2300]	= { ButtonId = 10,	name = "����ɳĮ",	content = "30~33��",	state ="nil"},
							[3300]	= { ButtonId = 11,	name = "��Ŭ��˹���",	content = "34~37��",	state ="nil"},
							[3303]	= { ButtonId = 12,	name = "��Ŭ��˹����",	content = "34~38��",	state ="nil"},
							[2500]	= { ButtonId = 13,	name = "Խ������",	content = "50~54��",	state ="nil"},
							[3500]	= { ButtonId = 14,	name = "Խ����",	content = "53~58��",	state ="nil"},
							[3402]	= { ButtonId = 15,	name = "������ԭ",	content = "80~85��",	state ="nil"},
							[2100]	= { ButtonId = 16,	name = "ϸ������",	content = "10~19��",	state ="nil"},
							[3404]	= { ButtonId = 17,	name = "��ɳ�ݵ�",	content = "75~78��",	state ="nil"},
							[3304]	= { ButtonId = 18,	name = "������",	content = "79~83��",	state ="nil"},
							[3302]	= { ButtonId = 19,	name = "��Ŭ��˹��̳",	content = "38~42��",	state ="nil"},
							[2202]	= { ButtonId = 20,	name = "����",		content = "18~26��",	state ="nil"},
						};
						
local t_BtnTexture	= 	{
							["Blue"]	= { NormalTexture		=	{ x = 554, y = 556, w = 29, h = 28},
											HighlightTexture	=	{ x = 584, y = 556, w = 29, h = 28},
											PushedTexture		=	{ x = 614, y = 556, w = 29, h = 28},
										  },
							["Green"]	= { NormalTexture		=	{ x = 658, y = 549, w = 29, h = 28},
											HighlightTexture	=	{ x = 658, y = 578, w = 29, h = 28},
											PushedTexture		=	{ x = 658, y = 607, w = 29, h = 28},
										  },
							["Gray"]	= { NormalTexture		=	{ x = 598, y = 527, w = 29, h = 28},
											HighlightTexture	=	{ x = 628, y = 527, w = 29, h = 28},
											PushedTexture		=	{ x = 633, y = 498, w = 29, h = 28},
										  },
						};
						
						

local	transId	
local	selectIdx
local	npcId


local MAX_NUMBER	=	16		
			
function WorldmapSendAreaPosBtn_OnClick()
	if CostMoneyConfirmFrame:IsShown() then
		return;
	end

	local	toMapId	=	this:GetClientUserData(0)
	npcId			=	GameMapSendingManager:getNpcID()
	for	i = 1, table.getn( transList )	do
		if	transList[i].ToMapID	==	toMapId	then
			transId	=	transList[i].TransID
			selectIdx	=	i-1
			break
		end
	end
	
	local selectMapId	=	this:GetClientUserData(0)
	if t_MapIdToBtnId[selectMapId]["state"] == "Gray" then
		AddGameMiddleTips2( "        ֻ��ռ��[ʱ���]�Ĺ����Ա�ſɴ��ͣ�������ݲ������޷����͡�" )
		return;
	end

	if selectMapId ~= GameMapSendingManager:getFormMapID() then	
		if GameMapSendingManager:beginCalcTranList( selectMapId ) then
			local richText			=	getglobal( "WorldMapSendInfoFrameInfo" )	
			local toMapName			=	t_MapIdToBtnId[selectMapId]["name"]
			local toMapContent		=	t_MapIdToBtnId[selectMapId]["content"]
			local sendInfoFrame		=	getglobal("WorldMapSendInfoFrame")
			local moneyTexture		=	getglobal( "WorldMapSendInfoFrameMoneyTex" )
			if VipManager:isOwnVipFunc( STAR_RESULT_TRANS_TOLUA ) then
				richText:SetText( "\t\t\t\tȷ��ǰ��"..toMapName.."["..toMapContent.."]��", 255, 254, 189 )
				moneyTexture:Hide();
			else
				local money		=	GameMapSendingManager:getCostMoney( selectMapId )
				local text		=	"��Ҫ"..money.."   ��ȷ��ǰ��"..toMapName.."["..toMapContent.."]��"
				richText:SetText( text, 255, 254, 189 )
				sendInfoFrame:SetClientUserData( 0, money );
				local width	= richText:GetTextExtentWidth( "��Ҫ"..money )
				moneyTexture:SetPoint( "topleft", "WorldMapSendInfoFrameInfo", "topleft", width, richText:GetDispPos() )
				moneyTexture:Show()
			end
			sendInfoFrame:Show()
		else
			--ShowMidTips("�ȼ������Ŀ���ͼ�޷�����")
		end		
	end
end

function WorldmapSendAreaPosBtn_OnEnter()
	local name		=	t_MapIdToBtnId[this:GetClientUserData(0)]["name"]
	local content	=	"["..t_MapIdToBtnId[this:GetClientUserData(0)]["content"].."]"
	local formMapID	=	GameMapSendingManager:getFormMapID()
	local posOrMoney
	local textColor	=	nil
	if	this:GetClientID() == t_MapIdToBtnId[formMapID]["ButtonId"] then
		posOrMoney	=	"��������"
		textColor	=	{0,255,0}
	else
		if VipManager:isOwnVipFunc( STAR_RESULT_TRANS_TOLUA ) then
			posOrMoney	=	"#cffff00��ѣ����ǻ�����Ȩ��#n";
		else
			posOrMoney	=	GameMapSendingManager:getCostMoney( this:GetClientUserData(0) )
		end
	end
	if t_MapIdToBtnId[this:GetClientUserData( 0 )]["state"] == "Gray" then
		posOrMoney = "#cff0000ֻ��ռ��[ʱ���]�Ĺ����Ա�ſɵ���#n"
	end
	SetGameToolAddTexturetips(this:GetName(), name..content.."\n"..posOrMoney, textColor)
end

function WorldmapSendAreaPosBtn_OnLeave()	
	GameTooltipAddTexture:Hide()
end

function MapSendUpdate()
	local MapSendFrame	=	getglobal("WorldMapSendFrame")
	MapSendFrame:Show()	
	
	transList	=	{}
	
	local num = GameMapSendingManager:getMapTransDefNum()
	
	for i = 1, num do
		--if i > MAX_NUMBER then break end
		local transDef	=	GameMapSendingManager:getMapTransDef(i-1)
		if transDef ~= nil then
			table.insert( transList, transDef )
			local textureColor	=	"Blue"
			if transDef.TransCondition ~= 0 then
				if not VipManager:isOwnVipFunc( STAR_RESULT_TRANS_TOLUA ) and 
				not WorldCityManager:IsWorldCityKeeper( ActorMgr:getMainPlayer() ) then
					textureColor = "Gray"
				end
			end
															
			local toMapId	=	t_MapIdToBtnId[transDef.ToMapID]["ButtonId"]
			t_MapIdToBtnId[transDef.ToMapID]["state"] = textureColor
			local btnTo	=	getglobal( "WorldmapSendAreaPosBtn"..toMapId )
			btnTo:ChangeNormalTexture( t_BtnTexture[textureColor].NormalTexture.x, t_BtnTexture[textureColor].NormalTexture.y,
										 t_BtnTexture[textureColor].NormalTexture.w, t_BtnTexture[textureColor].NormalTexture.h )
			btnTo:ChangeHighlightTexture( t_BtnTexture[textureColor].HighlightTexture.x, t_BtnTexture[textureColor].HighlightTexture.y,
											 t_BtnTexture[textureColor].HighlightTexture.w, t_BtnTexture[textureColor].HighlightTexture.h )
			btnTo:ChangePushedTexture( t_BtnTexture[textureColor].PushedTexture.x, t_BtnTexture[textureColor].PushedTexture.y,
										t_BtnTexture[textureColor].PushedTexture.w, t_BtnTexture[textureColor].PushedTexture.h )
			btnTo:Show()
			btnTo:SetClientUserData( 0, transDef.ToMapID)
			btnTo:SetClientUserData( 1, transDef.Money)
		end
	end

	local fromMapID	=	t_MapIdToBtnId[GameMapSendingManager:getFormMapID()]["ButtonId"]
	t_MapIdToBtnId[GameMapSendingManager:getFormMapID()]["state"] = "Green"
	local btnFrom	=	getglobal( "WorldmapSendAreaPosBtn"..fromMapID )
	btnFrom:ChangeNormalTexture( t_BtnTexture["Green"].NormalTexture.x, t_BtnTexture["Green"].NormalTexture.y,
								 t_BtnTexture["Green"].NormalTexture.w, t_BtnTexture["Green"].NormalTexture.h )
	btnFrom:ChangeHighlightTexture( t_BtnTexture["Green"].HighlightTexture.x, t_BtnTexture["Green"].HighlightTexture.y,
									 t_BtnTexture["Green"].HighlightTexture.w, t_BtnTexture["Green"].HighlightTexture.h )
	btnFrom:ChangePushedTexture( t_BtnTexture["Green"].PushedTexture.x, t_BtnTexture["Green"].PushedTexture.y,
								t_BtnTexture["Green"].PushedTexture.w, t_BtnTexture["Green"].PushedTexture.h )
	btnFrom:Show()
	btnFrom:SetClientUserData( 0, GameMapSendingManager:getFormMapID())
end
function WorldMapSendCancleBtn_OnClick()
	local sendInfoFrame	=	getglobal("WorldMapSendInfoFrame")
	sendInfoFrame:Hide()
end
function WorldMapSendOkBtn_OnClick()
	local MapSendInfoFrame	=	getglobal("WorldMapSendInfoFrame")
	MapSendInfoFrame:Hide()

	if not VipManager:isOwnVipFunc( STAR_RESULT_TRANS_TOLUA ) then
		local money = WorldMapSendInfoFrame:GetClientUserData(0);
		if money ~= nil then
			local mainplayer = ActorMgr:getMainPlayer();
			if getPlayerMoney( mainplayer ) < money then
				ShowMidTips( "�������㣬�޷�����" );
				return;
			end

			if checkNeedWarnCost( money ) then
				CostMoneyConfirmFrame:SetClientString("���紫��");
				CostMoneyConfirmFrame:SetClientUserData( 0, npcId );
				SetWarnFrame( "WorldMapSendFrame" );
				CostMoneyConfirmFrame:Show();
				return;
			end
		end
	end

	GameMapSendingManager:mapSendingSelect( npcId )
	local MapSendFrame	=	getglobal("WorldMapSendFrame")
	MapSendFrame:Hide()
end

function WorldMapSend_onShow()
	local nActorId	=	GameMapSendingManager:getNpcID()
	if  nActorId	~= nil and nActorId ~= 0 then
		util.testNpcDialogDistance( nActorId, this );
	end
	--����Ƿ�������˶�������
	if Quest:isHaveAcceptQuest( DEFAULT_WORLD_SEND_GUID_TASKID ) then
		local nCurMapID = MapShower:getRealMapId();
		--�Ƿ���ʾ����ʾ����														--��ʱ�Ƿ���ʱ���
		if GameWizard:isDo( GAMEWIZARD_FIRST_ACCEPT_TASK_TO_WORLD_MAP_SEND ) or nCurMapID == 1000 then
			WorldMapSendFrameGuidBtn:Hide();
			return
		end
		--��ʾ������ʾ����
		WorldMapSendFrameGuidBtnRich:SetText( "��˰�ť��˲�䴫����ʱ���", 0, 0, 0 );
		WorldMapSendFrameGuidBtn:Show();
		GameWizard:doStep( GAMEWIZARD_FIRST_ACCEPT_TASK_TO_WORLD_MAP_SEND );
	end

	if nActorId == 0 then
		LockItem( GameMapSendingManager:getMapTransItemList(), GameMapSendingManager:getMapTransItemGrid(), "WorldMapSendFrame" );
	end
end

function WorldMapSend_onHide()	
	for i = 1, 20 do
		local btn	=	getglobal("WorldmapSendAreaPosBtn"..i)
		btn:Hide()
	end
	local frame	=	getglobal("WorldMapSendInfoFrame")
	frame:Hide()
	GameTooltipAddTexture:Hide()

	UnLockItem( GameMapSendingManager:getMapTransItemList(), GameMapSendingManager:getMapTransItemGrid() );
end

function MapSenderLvLow( lv )
	ShowMidTips("�˵�ͼ̫Σ�գ�Ϊ������������������ǵ�" .. lv .. "���Ժ���ȥ�ɣ�")
end

function UseTransItem( gridIdx, listType )
	local MinMapContal	= MapShower:GetMinMapContal();
	local mapId = GameMapSendingManager:getMapTransDefByID( MinMapContal:getMapID() );
	if mapId ~= nil then
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer == nil then return end;

		if mainplayer:isInRide() then
			local rider = mainplayer:getRider();
			if not rider:isOwnRide() then
				return ShowMidTips("�����������ʱ�޷�ʹ�ÿ��к���");
			elseif rider:getPassengerNum() > 0 then
				return ShowMidTips("�������������˿�ʱ�޷�ʹ�ÿ��к���");
			end
		end

		local equip = mainplayer:getEquip()
		if equip:isUsedTmp() then
			local nTmpItemId = equip:getTmpMachin();				
			if nTmpItemId ~= nil or nTmpItemId ~= 0 then
				return ShowMidTips("��ǰ״̬���޷��޷�ʹ�ÿ��к���");
			end					
		end
		
		if mainplayer:isOnMonsterMachine() then
			return ShowMidTips("ʹ�û�еʱ�޷�ʹ�ÿ��к���");
		end

		if mainplayer:isInFly() then
			return ShowMidTips("��ǰ״̬���޷�ʹ�ÿ��к���");
		end

		GameMapSendingManager:openTranFrameByItem( gridIdx, listType );
	else
		return ShowMidTips( "��ǰ��ͼ���������ع����޷�ʹ�ÿ��к���" );
	end
end