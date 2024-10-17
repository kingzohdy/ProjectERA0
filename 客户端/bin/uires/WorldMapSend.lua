local transList	=	{};

local t_MapIdToBtnId = {	
							[3400]	= { ButtonId = 1,	name = "十三号矿洞",	content = "44~59级",	state ="nil"},
							[2400]	= { ButtonId = 2,	name = "落日荒野",	content = "40~44级",	state ="nil"},
							[2000]	= { ButtonId = 3,	name = "天空岛",	content = "1~10级",	state ="nil"},
							[1000]	= { ButtonId = 4,	name = "时间城",	content = "主城",	state ="nil"},
							[2200]	= { ButtonId = 5,	name = "银风雪原",	content = "20~29级",	state ="nil"},
							[2201]	= { ButtonId = 6,	name = "风回裂谷",	content = "70~73级",	state ="nil"},
							[3200]	= { ButtonId = 7,	name = "牢城",		content = "74~77级",	state ="nil"},
							[3100]	= { ButtonId = 8,	name = "普罗圣地",	content = "63~66级",	state ="nil"},
							[2104]	= { ButtonId = 9,	name = "荫川林地",	content = "60~63级",	state ="nil"},
							[2300]	= { ButtonId = 10,	name = "流金沙漠",	content = "30~33级",	state ="nil"},
							[3300]	= { ButtonId = 11,	name = "阿努比斯神殿",	content = "34~37级",	state ="nil"},
							[3303]	= { ButtonId = 12,	name = "阿努比斯废墟",	content = "34~38级",	state ="nil"},
							[2500]	= { ButtonId = 13,	name = "越雾雨林",	content = "50~54级",	state ="nil"},
							[3500]	= { ButtonId = 14,	name = "越雾甬道",	content = "53~58级",	state ="nil"},
							[3402]	= { ButtonId = 15,	name = "迈阿荒原",	content = "80~85级",	state ="nil"},
							[2100]	= { ButtonId = 16,	name = "细语丘陵",	content = "10~19级",	state ="nil"},
							[3404]	= { ButtonId = 17,	name = "鸣沙陷地",	content = "75~78级",	state ="nil"},
							[3304]	= { ButtonId = 18,	name = "法老陵",	content = "79~83级",	state ="nil"},
							[3302]	= { ButtonId = 19,	name = "阿努比斯祭坛",	content = "38~42级",	state ="nil"},
							[2202]	= { ButtonId = 20,	name = "冰岛",		content = "18~26级",	state ="nil"},
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
		AddGameMiddleTips2( "        只有占领[时间城]的公会成员才可传送，您的身份不符，无法传送。" )
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
				richText:SetText( "\t\t\t\t确认前往"..toMapName.."["..toMapContent.."]吗？", 255, 254, 189 )
				moneyTexture:Hide();
			else
				local money		=	GameMapSendingManager:getCostMoney( selectMapId )
				local text		=	"需要"..money.."   ，确认前往"..toMapName.."["..toMapContent.."]吗？"
				richText:SetText( text, 255, 254, 189 )
				sendInfoFrame:SetClientUserData( 0, money );
				local width	= richText:GetTextExtentWidth( "需要"..money )
				moneyTexture:SetPoint( "topleft", "WorldMapSendInfoFrameInfo", "topleft", width, richText:GetDispPos() )
				moneyTexture:Show()
			end
			sendInfoFrame:Show()
		else
			--ShowMidTips("等级不足或目标地图无法到达")
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
		posOrMoney	=	"你在这里"
		textColor	=	{0,255,0}
	else
		if VipManager:isOwnVipFunc( STAR_RESULT_TRANS_TOLUA ) then
			posOrMoney	=	"#cffff00免费（银星徽章特权）#n";
		else
			posOrMoney	=	GameMapSendingManager:getCostMoney( this:GetClientUserData(0) )
		end
	end
	if t_MapIdToBtnId[this:GetClientUserData( 0 )]["state"] == "Gray" then
		posOrMoney = "#cff0000只有占领[时间城]的公会成员才可到达#n"
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
				ShowMidTips( "绑银不足，无法传送" );
				return;
			end

			if checkNeedWarnCost( money ) then
				CostMoneyConfirmFrame:SetClientString("世界传送");
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
	--检测是否接受了运动会任务
	if Quest:isHaveAcceptQuest( DEFAULT_WORLD_SEND_GUID_TASKID ) then
		local nCurMapID = MapShower:getRealMapId();
		--是否显示过提示引导														--此时是否在时间城
		if GameWizard:isDo( GAMEWIZARD_FIRST_ACCEPT_TASK_TO_WORLD_MAP_SEND ) or nCurMapID == 1000 then
			WorldMapSendFrameGuidBtn:Hide();
			return
		end
		--显示传送提示引导
		WorldMapSendFrameGuidBtnRich:SetText( "点此按钮可瞬间传送至时间城", 0, 0, 0 );
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
	ShowMidTips("此地图太危险，为了您的身家性命，还是等" .. lv .. "级以后再去吧！")
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
				return ShowMidTips("骑乘他人坐骑时无法使用空中航线");
			elseif rider:getPassengerNum() > 0 then
				return ShowMidTips("坐骑上有其他乘客时无法使用空中航线");
			end
		end

		local equip = mainplayer:getEquip()
		if equip:isUsedTmp() then
			local nTmpItemId = equip:getTmpMachin();				
			if nTmpItemId ~= nil or nTmpItemId ~= 0 then
				return ShowMidTips("当前状态下无法无法使用空中航线");
			end					
		end
		
		if mainplayer:isOnMonsterMachine() then
			return ShowMidTips("使用机械时无法使用空中航线");
		end

		if mainplayer:isInFly() then
			return ShowMidTips("当前状态下无法使用空中航线");
		end

		GameMapSendingManager:openTranFrameByItem( gridIdx, listType );
	else
		return ShowMidTips( "当前地图（副本、地宫）无法使用空中航线" );
	end
end