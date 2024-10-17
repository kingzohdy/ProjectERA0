local szPrivatePlayerName ="";

local t_PrivatePlayerName = {};

function InitChatFrame()
	ClearSetLink();
	-- ���˽�Ķ���
	t_PrivatePlayerName = {};
	
	local t_ChatInsertFrameControl = GetChatInsertFrameControl();
	t_ChatInsertFrameControl:setPrivatePlayerName( { ["name"] = "" } );

	-- ������е�����
	UpdateChatHistoryMsgMgrFrameOnMainplayerDestroy();
	for i = 1, MAX_QUICK_CHAT_MSG do
		local rich = getglobal("CSSMFInsertEdit"..i);
		rich:Clear();
	end

	Private_ResetChatTypeLableBtn();
	SetChatType( CL_CHAT_SAY );	   
	ClearYellowMiddleTips();
	ClearTmpGetItemTips();
	InitChatShowBugleMsgFrame();
	InitChatAllBugleMsgFrame();
	
	SetChatBackAlpha( 0 );
	MainChatFrame_ChatBackBtnTex:SetBlendAlpha(0);

	InitGameBulletintTipsData();
	ClearBulletin();

	InitInputFrameData();

	InitMainChatFrame();
end

-- { ["originalName"] = , ["clientFormatName"] = , ["serveFormamtName"] = , ["linkType"] = }
t_SetLink = {};

function RemoveOneLinkCache( data )
	for index, oneLinkData in ipairs( t_SetLink ) do
		if oneLinkData["linkType"] == data["linkType"] then
			table.remove( t_SetLink, index );
			break;
		end
	end
end

function GetOneLinkCache( data )
	for index, oneLinkData in ipairs( t_SetLink ) do
		if oneLinkData["linkType"] == data["linkType"] then
			return oneLinkData;
		end
	end
end

--��ջ���
function ClearSetLink()
	for index, oneLinkData in ipairs( t_SetLink ) do
		ChatManager:deleteLink( oneLinkData["linkType"] );
	end
	t_SetLink= {};
end

function SetSystemMessage( szText )
	local szShowText		= "#A006"..szText;
	local nCanPingBiChatType= GetPingDaoChatType( { ["anim"] = "#A006" } );
	local t_color			= t_mainChatFrameTextColor[nCanPingBiChatType];
	UpdatePersonalAndAllChatRich( { ["text"] = szShowText, ["color"] = t_color, ["chatType"] = nCanPingBiChatType } );
	
	local historyRich = getglobal( "ChatHistoryMsgMgrFrame_SystemRich" );
	historyRich:AddText( "#A006"..szText, t_color["r"], t_color["g"], t_color["b"] );
	UpdateHistorySliderRange( { ["name"] = historyRich:GetName() } );
end

function GetPrivatePlayerNameList()
	return t_PrivatePlayerName;
end

function AddPrivatePlayerName( name )
	for i =1, table.getn(t_PrivatePlayerName) do
		if t_PrivatePlayerName[i] == name then
			table.remove(t_PrivatePlayerName,i);
			break;
		end
	end
	table.insert(t_PrivatePlayerName,1,name);
	if table.getn(t_PrivatePlayerName) > 3 then
		table.remove(t_PrivatePlayerName,table.getn(t_PrivatePlayerName));
	end
end

function MonsterSpeak( nMonsterID, nMemID, szTalk, nMsgType, szName, nRace, nGender )
	local MonsterObj = ActorMgr:FindActor( nMemID );
	local szMonsterTalk = ChatManager:strTrim( szTalk );
	if szMonsterTalk == "" then
		return;
	end

	szTalk = ReplaceText( szTalk, "#Name#", szName );  
	szTalk = ReplaceText( szTalk, "#Race#", GetCareerName( nRace ) );
	szTalk = ReplaceText( szTalk, "#Gender#", GetGender( nGender ) );
	
	local t_color	= t_mainChatFrameTextColor[CL_CHAT_MONSTER];
	if nMsgType == SYS_MIDDLE then
		AddGameMiddleTips( szTalk );
	elseif nMsgType == SYS_MIDDLE2 then
		AddGameMiddleTips2( szTalk );
	elseif nMsgType == SYS_CHAT then --SYS_WHOOP
		local szShowText = "";
		if MonsterObj ~= nil then
			local szObjName = MonsterObj:getName();
			szShowText =  "#A003"..szObjName..":"..szTalk;
			GameWizard:setNpcTalkByActorId( nMemID, szTalk, "ChatPop", false, 0, 0, "PopBackArrowTexture", true,0,0,0,8 );
		else
			szShowText = "#A003"..getMonsterName(nMonsterID)..":"..szTalk;			
		end

		local nCanPingBiChatType = GetPingDaoChatType( { ["anim"] = "#A003" } );
		UpdatePersonalAndAllChatRich( { ["text"] = szShowText, ["color"] = t_color, ["chatType"] = nCanPingBiChatType } );
	end
end

function ParserChatHistoryText( szLinkText )
	--print( "szLinkText = "..szLinkText.."--168" );
	local nLength	= string.len( szLinkText );
	local nFirstPos = string.find( szLinkText, "#L", 1, nLength );  
	local nStartPos = string.find( szLinkText, "@@", 1, nLength ); 
	local nEndPos	= string.find( szLinkText, "#n", 1, nLength );
	if nFirstPos == nil or nStartPos == nil or nEndPos == nil then
		return;
	end
	
	local szChatLinkText= "";
	local nId			= string.sub( szLinkText, nStartPos + 2, nEndPos - 1 ); 
	local szLastText	= string.sub( szLinkText, nEndPos+2 );
	local szPreLinkText	= string.sub( szLinkText, 1, nFirstPos-1);
	--print( "szPreLinkText = "..szPreLinkText.."--182" );
	CSChatInsert:SetText( szPreLinkText );
	
	--����������
	local nLinkType		= tonumber( nId ) % 10;
	--������Count
	local linkCountId	= ( tonumber( nId ) - nLinkType ) / 10;
	--print( "ParserChatHistoryText--nId = "..nId.."--nLinkType = "..nLinkType.."--linkCountId = "..linkCountId.."--169" );
	ChatManager:deleteLink( nLinkType );

	--[[
	if nLinkType == ITEM_LINK then
		ChatManager:deleteLink( ARM_LINK );
		RemoveOneLinkCache( { ["linkType"] = ARM_LINK } );
	elseif nLinkType == ARM_LINK then
		ChatManager:deleteLink( ITEM_LINK );
		RemoveOneLinkCache( { ["linkType"] = ITEM_LINK } );
	end
	--]]

	RemoveOneLinkCache( { ["linkType"] = nLinkType } );

	if nLinkType == ITEM_LINK or nLinkType == ARM_LINK then
		ChatManager:setLinkedItemLink(nLinkType,linkCountId);		
	elseif nLinkType == MISSION_LINK then 
		ChatManager:setMissionLink( ChatManager:getMissionLink( linkCountId ));
	elseif nLinkType == POSTION_LINK then 
		ChatManager:setPositionLink( ChatManager:getPostionLink( linkCountId ));
	elseif nLinkType == SECONDARY_SKILL_LINK then
		ChatManager:setSecondarySkillLink( ChatManager:getSecondarySkillLink( linkCountId ));
	elseif nLinkType == PET_LINK then
		ChatManager:setPetLink(ChatManager:getPetLink( linkCountId ));
	end
	
	szChatLinkText = ( CSChatInsert:GetText() .. szLastText );
	--print( "szChatLinkText = "..szChatLinkText.."--206" );
	CSChatInsert:SetText( szChatLinkText );
	-- �ݹ������һ������
	ParserChatHistoryText( szChatLinkText );
end

local szCurSelectChatPlayerName = nil;

function OnClick_MainChatFrame_PictureObject( data )
	local nChatType					= GetPingDaoChatType( { ["anim"] = data["anim"] } );
	local t_ChatInsertFrameControl	= GetChatInsertFrameControl();
	if not t_ChatInsertFrameControl:isSupportIChatType( { ["chatType"] = nChatType } ) then
		return;
	end
	
	local nLastChatType	= GetChatType();
	if nLastChatType == nChatType then
		return;
	end

	SetChatType( nChatType );
	t_ChatInsertFrameControl:setPrivatePlayerName( { ["name"] = "" } );
	UpdateChatInsertFrameEnterFocus();

	local nCurChatType = GetChatType();
	if nLastChatType == CL_CHAT_PRIVATE and nCurChatType ~= CL_CHAT_PRIVATE then
		CSChatInsert:SetText( CSChatInsert:GetText() );
	end

	SetFocusFrame( "CSChatInsert");
end

function RichText_OnClick()
	--����Ǳ��������������
	if type(arg1) == "number" then
		return;
	end	
	
	if arg2 ~= "" and arg3 == nil then
		OnClick_MainChatFrame_PictureObject( { ["anim"] = arg1 } );
		return;
	end

	if arg2 ~= "" then
		if string.sub(arg2,1,1) == "@" then
			local name = string.sub(arg2,2)
			local mainplayer = ActorMgr:getMainPlayer();
			if mainplayer:getName() == name then
				return;
			end		
			local t_ChatInsertFrameControl = GetChatInsertFrameControl();
			local szPrivatePlayerName = "";
			if arg3 == "RightButton" then
				szCurSelectChatPlayerName = name;
				szPrivatePlayerName = name;
				t_ChatInsertFrameControl:setPrivatePlayerName( { ["name"] = szPrivatePlayerName } );
				ShowCharacterRightFrame( szCurSelectChatPlayerName );
			elseif arg3 == "LeftButton" then
				if not ChatInsertFrame:IsShown() then
					UpdateChatInsertFrameEnterFocus();
					DisCheckShortcutButton();
				end		

				szCurSelectChatPlayerName = name;
				szPrivatePlayerName = name;
				t_ChatInsertFrameControl:setPrivatePlayerName( { ["name"] = szPrivatePlayerName } );

				SetChatType( CL_CHAT_PRIVATE );
				--CSChatInsert:Clear();
				UpdateChatInsertFrameEnterFocus();
				SetFocusFrame("CSChatInsert"); 
			end
			return
		end
		if tonumber(arg2) == nil then 
			local bLinkNpc, nNpcId, nTargetMapId = GetNpcPos( arg2 );
			if bLinkNpc == true then
				-- ��������Ŀ��NPC����ͬһ�ŵ�ͼ��
				MapShower:GetMaxMapContal():linkMoveto( nTargetMapId, nNpcId );
				return;
			end

			local nMapId, nMapPosX, nMapPosY = GetLinkPos( arg2 );
			if nMapId ~= 0 then
				MapShower:GetMaxMapContal():linkMoveto( nMapId, nMapPosX, nMapPosY );
			end
			return 
		end
		-- ����������
		local nLinkType		= tonumber( arg2 ) % 10;
		-- ������Count
		local linkCountId	= ( tonumber( arg2 ) - nLinkType ) / 10;

		--print( "nLinkType = "..nLinkType.."�� linkCountId = "..linkCountId );
		--GameExchange:test( "nLinkType = "..nLinkType.."�� linkCountId = "..linkCountId );

		--VK_LSHIFT 160,VK_RSHIFT 161
		if isKeyPressed(160) or isKeyPressed(161) then
			--������������ʾ�У�����Ʒ���ָ��Ƶ�������
			if OnlineStoreFrame:IsShown() then
				local itemDef;
				if nLinkType == ITEM_LINK then
					local itemLink = ChatManager:getItemLink(linkCountId);			
					if itemLink.m_nItemId > 0 then
						itemDef = getItemDef( itemLink.m_nItemId )
					end
				elseif nLinkType == ARM_LINK then
					local armLink = ChatManager:getArmLink(linkCountId);
					if getItemType(armLink.m_arm.DefID) == ITEM_SLOTTYPE_AMM then
						--װ����tips
						local item = ChatManager:getItem(armLink.m_arm);
						if item ~= nil then
							itemDef = item:getItemDef()
						end
					end
				end
				if itemDef ~= nil then
					OnlineStoreFrameSearchEdit:SetText( itemDef.Name )
					OnlineStoreFrame:AddLevelRecursive()
					SetFocusFrame( "OnlineStoreFrameSearchEdit" )
					return
				end
			end
			--��ʾ���������	
			if not ChatInsertFrame:IsShown() then
				UpdateChatInsertFrameEnterFocus();
			end
			ChatManager:setLinkedItemLink(nLinkType,linkCountId);
			return;
		end

		-- ��Ʒ������
		if nLinkType == ITEM_LINK then
			--��<>����Ʒ����ת��Ϊ����<>
			local t_hideIcons = {  "LinkItemTipsFrame", "NormalItemTipsFrame", "ArmItemTipsFrame", "SecondArmItemTipsFrame", };
			util.HideIcons( t_hideIcons );
			local itemLink = ChatManager:getItemLink(linkCountId);
			if itemLink.m_nItemId > 0 then
				--��ͨ���ߵ�tips
				local itemDef = getItemDef(itemLink.m_nItemId);
				if GameArmProce:getGemNewDef( itemDef.ItemID ) ~= nil then
					local gemData = itemLink.InstVal1..","..itemLink.InstVal2;
					ShowItemTips(LUA_ITEMTIPS_TYPE_LINK, LUA_ITEMTIPS_ITEMDEF, itemDef, nil ,nil, nil, nil, gemData );
					return;
				end
				local item = ChatManager:getSparItem( itemLink )
				if item ~= nil then
					ShowItemTips(LUA_ITEMTIPS_TYPE_LINK,LUA_ITEMTIPS_ITEM,item);
				else
					ShowItemTips(LUA_ITEMTIPS_TYPE_LINK, LUA_ITEMTIPS_ITEMDEF, itemDef );
				end
				
				return;

			end
		elseif nLinkType == ARM_LINK then
			local t_hideIcons = {  "LinkItemTipsFrame", "NormalItemTipsFrame", "ArmItemTipsFrame", "SecondArmItemTipsFrame", };
			util.HideIcons( t_hideIcons );
			local armLink = ChatManager:getArmLink(linkCountId);
			if getItemType(armLink.m_arm.DefID) == ITEM_SLOTTYPE_AMM then
				--װ����tips
				local item = ChatManager:getItem(armLink.m_arm);
				if item ~= nil then
					ShowItemTips(LUA_ITEMTIPS_TYPE_LINK,LUA_ITEMTIPS_ITEM,item);
				end
			end
		--λ����������
		elseif nLinkType == POSTION_LINK  then
			local postionLink = ChatManager:getPostionLink(linkCountId);
			local player = ActorMgr:getMainPlayer();

			--���mapId����ǰ��mapId����ͬʱ,����Ѱ·
			if ( player == nil ) then return end

			if postionLink.m_nMapId ~= 0 then
				local nId = postionLink.m_nMapId;
				local nX  = postionLink.m_nX;
				local nY  = postionLink.m_nY;
				MapShower:GetMaxMapContal():linkMoveto(nId, nX, nY );
				return;
			end
		--λ����������
		elseif nLinkType == MISSION_LINK  then
			local missionLink = ChatManager:getMissionLink(linkCountId);
			if missionLink.m_MissionId ~= 0 then
				SetShowQuestLogState(missionLink.m_MissionId);
				SetTaskLogFrameTaskLogInfo( missionLink.m_MissionId, missionLink.m_nRecycleID, false );
				TaskLogDescFrame:SetPoint( "center", "$parent", "center", 0, 0 );
				SetShowQuestLogState(0);
				return;
			end
		elseif nLinkType == SERVER_LINK then
			local postionLink = ChatManager:getSystemLink(linkCountId);
			local player = ActorMgr:getMainPlayer();

			--���mapId����ǰ��mapId����ͬʱ,����Ѱ·
			if ( player == nil ) then return end

			if postionLink.m_nMapId ~= 0 then
				local nId = postionLink.m_nMapId;
				local nX  = postionLink.m_nX;
				local nY  = postionLink.m_nY;
				MapShower:GetMaxMapContal():linkMoveto(nId, nX, nY );
				return;
			end
		elseif nLinkType == SECONDARY_SKILL_LINK then			
			SecondarySkillOpenLink( linkCountId );
		elseif nLinkType == PET_LINK then
			local petLink = ChatManager:getPetLink( linkCountId );
			if petLink.PetWid.nHeight == 0 and petLink.PetWid.nLow == 0 then
				return;
			end
			RankMgr:requestWatchPet( petLink.PetWid.nLow,petLink.PetWid.nHeight,petLink.OwnerName,string.len(petLink.OwnerName) );
		end
	end
	
	if arg1 ~= "" and arg1 ~= "LeftButton" and arg1 ~= "RightButton" and arg2 == "" then
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer:getName() == arg1 then
			return;
		end
		
		local t_ChatInsertFrameControl = GetChatInsertFrameControl();
		local szPrivatePlayerName = "";
		if arg3 == "RightButton" then
			szCurSelectChatPlayerName = arg1;
			szPrivatePlayerName = arg1;
			t_ChatInsertFrameControl:setPrivatePlayerName( { ["name"] = szPrivatePlayerName } );
			ShowCharacterRightFrame( szCurSelectChatPlayerName );
		elseif arg3 == "LeftButton" then
			if not ChatInsertFrame:IsShown() then
				UpdateChatInsertFrameEnterFocus();
				DisCheckShortcutButton();
			end		

			szCurSelectChatPlayerName = arg1;
			szPrivatePlayerName = arg1;
			t_ChatInsertFrameControl:setPrivatePlayerName( { ["name"] = szPrivatePlayerName } );

			SetChatType( CL_CHAT_PRIVATE );
			--CSChatInsert:Clear();
			UpdateChatInsertFrameEnterFocus();
			SetFocusFrame("CSChatInsert"); 
		end
	end
end

function GetCurSelectChatPlayerName()
	return szCurSelectChatPlayerName;
end

function GetContainerHaveItemResult( nListType, nResult )
	local Mainplayer	= ActorMgr:getMainPlayer();
	local container		= Mainplayer:getContainer();
	for i = 1, container:getContainerMaxNum( nListType ) do
		local item = container:getItem( nListType, i - 1 );
		if item:getItemId() ~= 0 and CheckItemResult( item:getItemDef(), nResult ) then
			return item;
		end
	end

	return;
end

-- ��ӽӿ�

function NeedDelayShowText()
	if LoadingFrame:IsShown()  then
		return true;
	end

	return false;
end

-- ���ù��＼�ܷ���
function SetMonsterSkillSpeech( nMonsterResId, nMonsterMemId, nConditionType, nSkillId )
	ChatManager:setMonsterSkillSpeech( nMonsterResId, nMonsterMemId, nConditionType, nSkillId );
end

function SetMonsterDeadSpeeck( pObjResId, pObjId, szAttackName, nAttackRace, nAttackGender )	 
	ChatManager:setMonsterDeadSpeech( pObjResId, pObjId, szAttackName, nAttackRace, nAttackGender );
end

function SetMonsterSkillAttackSpeech( pObjResId,pObjId,szAttackName,nConditionType,nSkillId )
	ChatManager:setMonsterSkillAttackSpeech( pObjResId,pObjId,szAttackName,nConditionType,nSkillId );
end

--ʱ����Ʒ����֪ͨ
function ItemValidNotify( itemName,NotifyType )
	local szText = itemName;
	if NotifyType == ITEM_VALID_24H then
		szText = szText .. "24Сʱ����Ч�ڽ���";
	elseif NotifyType == ITEM_VALID_3H then
		szText = szText .. "3Сʱ����Ч�ڽ���";
	elseif NotifyType == ITEM_VALID_10M then
		szText = szText .. "10���Ӻ���Ч�ڽ���";
	elseif NotifyType == ITEM_VALID_END then
		szText = szText .. "����Ч�ڽ�������鿴��Ʒ";
	end

	local szShowText		= "#A006"..szText;
	local nCanPingBiChatType= GetPingDaoChatType( { ["anim"] = "#A006" } );
	local t_color			= t_mainChatFrameTextColor[nCanPingBiChatType];
	UpdatePersonalAndAllChatRich( { ["text"] = szShowText, ["color"] = t_color, ["chatType"] = nCanPingBiChatType } );

	local historyRich = getglobal( "ChatHistoryMsgMgrFrame_SystemRich" );
	historyRich:AddText( "#A006"..szText, t_color["r"], t_color["g"], t_color["b"] );
	UpdateHistorySliderRange( { ["name"] = historyRich:GetName() } );
end

function ItemValidEnd( itemName )
	ShowMidTips( itemName .."����Ч���ѵ��ڣ��޷�ʹ��" );
end