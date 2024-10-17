function OnEscPressd()
	LoginMgr:onEscPress();
end

local t_NeedHideFrame = {"TaskTraceFrame_QuestRunTipBtn"};
local nSrcList  = -1; --Դ����
local nSrcGrid  = -1; --Դ����
local nNum      = 0; --����

local t_fadeTextures = {};

local nPlayGameTime = 0;

-- ������Ч�� 
-- @param: szTextureName	-- ��Ҫʵ�ֵ���Ч������������
-- @param: fStartAlpha		-- ��Ҫʵ�ֵ���Ч������ʼalphaֵ
-- @param: fEndAlpha		-- ��Ҫʵ�ֵ���Ч������ֹalphaֵ
-- @param: fLastTime		-- ��Ҫʵ�ֵ���Ч�����ܳ���ʱ�䣬ʱ�䵥λms
-- @param: fChangeTime		-- ��Ҫʵ�ֵ���Ч��ʱ��ı�alpha����Сʱ������ʱ�䵥λms
function util.UIFrameToFadeIn( szTextureName, fStartAlpha, fEndAlpha, fLastTime, fChangeTime )
	if t_fadeTextures[szTextureName] == nil then
		t_fadeTextures[szTextureName] = {};
	end
	
	t_fadeTextures[szTextureName]["FadeIn"] =	{	startAlpha = fStartAlpha, endAlpha = fEndAlpha,
													lastTime = fLastTime, compareTime = 0, changeTime = fChangeTime,
													averageChangeAlpha = fChangeTime * ( fEndAlpha - fStartAlpha ) / fLastTime,
													initTime = 0, 
												};
	
	local tex = getglobal( szTextureName );
	tex:Show();

	tex:SetBlendAlpha( fStartAlpha );
end

-- �����ָ���Ч��
-- @param: szTextureName	-- ��Ҫʵ�ֱ���ĳ��alphaЧ������������
-- @param: fKeepAlpha		-- ��Ҫʵ�ֱ���ĳ��alphaЧ����alphaֵ
-- @param: fLastTime		-- ��Ҫʵ�ֱ���ĳ��alphaЧ���ĳ���ʱ��
function util.UIFrameToFadeKeep( szTextureName, fKeepAlpha, fLastTime )
	if t_fadeTextures[szTextureName] == nil then
		t_fadeTextures[szTextureName] = {};
	end

	t_fadeTextures[szTextureName]["Keep"] =		{	keepAlpha = fKeepAlpha, lastTime = fLastTime, initTime = 0, compareTime = 0,
												};
								
	local tex = getglobal( szTextureName );
	tex:Show();

	if t_fadeTextures[szTextureName]["FadeIn"] then
		return;
	end

	tex:SetBlendAlpha( fKeepAlpha );
end

-- ������Ч��
-- @param: szTextureName	-- ��Ҫʵ�ֵ���Ч������������
-- @param: fStartAlpha		-- ��Ҫʵ�ֵ���Ч������ʼalphaֵ
-- @param: fEndAlpha		-- ��Ҫʵ�ֵ���Ч������ֹalphaֵ
-- @param: fLastTime		-- ��Ҫʵ�ֵ���Ч�����ܳ���ʱ�䣬ʱ�䵥λms
-- @param: fChangeTime		-- ��Ҫʵ�ֵ���Ч��ʱ��ı�alpha����Сʱ������ʱ�䵥λms
function util.UIFrameToFadeOut( szTextureName, fStartAlpha, fEndAlpha, fLastTime, fChangeTime )
	if t_fadeTextures[szTextureName] == nil then
		t_fadeTextures[szTextureName] = {};
	end

	t_fadeTextures[szTextureName]["FadeOut"] = {	startAlpha = fStartAlpha, endAlpha = fEndAlpha,
													lastTime = fLastTime, compareTime = 0, changeTime = fChangeTime, initTime = 0,
													averageChangeAlpha = fChangeTime * ( fStartAlpha - fEndAlpha ) / fLastTime,
												};
	local tex = getglobal( szTextureName );
	tex:Show();
	
	if t_fadeTextures[szTextureName]["FadeIn"] then
		return;
	end

	if t_fadeTextures[szTextureName]["Keep"] then
		return;
	end
	tex:SetBlendAlpha( fStartAlpha );
end

local t_shutDownDialogByDist = {};

-- @param: nMaxDist			-- ��mΪ��λ
-- @param: nActorId			-- ��ʵ��id
-- @param: szFrameName		-- ��Ҫ�����ĶԻ��������
function util.UIFrameToShutDown( szFrameName, nActorId, nMaxDist )
	t_shutDownDialogByDist[szFrameName] = { id = nActorId, maxDist = nMaxDist };
end

function CheckFrameToShutDownByDist( )
	local mainplayer = ActorMgr:getMainPlayer();
	for name, data in pairs( t_shutDownDialogByDist ) do
		local frame = getglobal( name );
		if frame:IsShown() then
			local actor = ActorMgr:FindActor( data.id );
			if actor == nil then
				frame:Hide();
				t_shutDownDialogByDist[name] = nil;
			else
				if util.CalActorsDistance( mainplayer, actor ) > data.maxDist ^ 2 then
					frame:Hide();
					t_shutDownDialogByDist[name] = nil;
				end
			end
		end
	end
end

function UIClient_OnShow()
	this:setUpdateTime( 0 );
end

local t_UIClientEvents =	{	-- �Ͽ���������
								["GE_BREAK_NETWORK"] = {}, ["GE_RELINK_NETWORK"] = {}, ["GE_INVITER_DEAD"] = {},
								["GE_MESSAGE_DEAD"]	= {}, ["GE_ENTER_PLAYERLOGIN"] = {}, ["GE_BREAK_NETWORK_TO_DIR"] = {}
							};

t_UIClientEvents["GE_MESSAGE_DEAD"].func = 
function ()
	local t_AutoRoundWorlldControl = GetAutoRoundWorlldControl();
	if t_AutoRoundWorlldControl:isAroundWorld() then
		StopAutoRoundWorlld();
	end
end

t_UIClientEvents["GE_BREAK_NETWORK"].func = 
function ()
	--LoginMgr:returnRoleList();
	--ReturnToSelect();
	--MessageBox( "��ʾ", "�������Ӵ���" );
	ShowMidTips("�������Ӳ��ȶ�")
end

t_UIClientEvents["GE_BREAK_NETWORK_TO_DIR"].func = 
function ()
	ReturnToSelect();
	MessageBox( "��ʾ", "�������Ӵ���" );
end

t_UIClientEvents["GE_RELINK_NETWORK"].func = 
function ()
	MessageBox( "��ʾ", "�����������ӳɹ�" );
end

t_UIClientEvents["GE_INVITER_DEAD"].func = 
function ()
	SetSystemMessage( "�Է�������״̬" );
end

t_UIClientEvents["GE_ENTER_PLAYERLOGIN"].func = 
function ()
	nPlayGameTime = os.clock();
end

function UIClient_OnLoad()
	for event, _ in pairs( t_UIClientEvents ) do
		this:RegisterEvent( event );
	end

	t_UIClientEvents.__index = function ( tab, key )
								tab[key] = { func = function () end };
								return tab[key];
							end
	setmetatable( t_UIClientEvents, t_UIClientEvents );
	this:setUpdateTime( 0 );
end

function UIClient_OnEvent()
	t_UIClientEvents[arg1].func();
end

function UIClient_OnClick()
	nSrcList  = UIMSG:getUIGV().DragFrom;
	nSrcGrid  = UIGetDragUserData1();
	nNum      = UIGetDragUserData2();
	
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then
		return;
	end
	if ( arg1 == "RightButton" ) then
		return;
	end
	if arg1 == "LeftButton" then
		for i = 1, table.getn( t_NeedHideFrame ) do
			local frame = getglobal( t_NeedHideFrame[i] );
			if frame:IsShown() then
				frame:Hide();
			end
		end
	end

	if ( arg1 == "LeftButton" and nSrcList == DRAG_SHORTCUT ) then
		local src = UIGetDragUserData1() - 1;
		GameShortCut:delShortCut( src );
		local desBtn	 = getglobal("MagicShortcut"..nSrcGrid);
		if desBtn == nil then
			desBtn	 = getglobal("TempMagicShortcut"..nSrcGrid);
		end
		desBtn:SetCooldownTimer(0, 0, true);
		desBtn:SetClientUserData( 1, 0 );
		UIEndDrag()
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		GameMgr:cancelNextSelect();
	end
	if ( arg1 == "LeftButton" and
		(nSrcList == CONTAINER_TYPE_SUNDRIES	or
		nSrcList == CONTAINER_TYPE_TASK 		or
		nSrcList == CONTAINER_TYPE_STASH		or
		nSrcList == CONTAINER_TYPE_STUFF		or
		nSrcList == CONTAINER_TYPE_EXTEND_STASH or
		nSrcList == CONTAINER_TYPE_EXTEND ) )		then
		if GameHomelandManager:isInSelfHome() and GameHomelandManager:isInPlantMode() then
			return
		end
		local MainPlayer = ActorMgr:getMainPlayer();
		local container = MainPlayer:getContainer();
		local item		= container:getItem( nSrcList, nSrcGrid );
		local itemdef	= item:getItemDef();
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;

		if itemdef == nil or nSrcList == CONTAINER_TYPE_STASH then			
			GameMgr:cancelNextSelect();
			return;
		end

		if itemdef.NoDrop == 0 then
			local mainplayer = ActorMgr:getMainPlayer();
			if mainplayer == nil then return end
			local container = mainplayer:getContainer();
			local lockState = container:getContainerLockState();
			if lockState == CONTAINER_LOCK_STATE_LOCK then
				--ShowMidTips("������֤������ȫ��");
				ContainerOpenLockFrame:Show();
			else
				if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "ȷ��������Ʒ" then
					UnLockItem( MessageBoxFrame:GetClientUserData(0), MessageBoxFrame:GetClientUserData(1), 0 );
				end
				
				local idx = CheckItemNeedImportantConfirm( item );
				if idx > 0 then
					ImportantItemConfirmFrameTitle:SetText("���߶���ȷ��");
					ShowImportantItemConfirmFrame( "����", item, idx, true );
					SetItemWarnFrame("PlayerContainerFrame");
					LockItem( item:getListType(), item:getGridIdx(), item:getItemId(), "ImportantItemConfirmFrame" );
				else
					if item:getItemType() == ITEM_SLOTTYPE_AMM and itemdef.Important > 2 then
						ImportantItemConfirmFrameTitle:SetText("���߶���ȷ��");
						ShowImportantItemConfirmFrame( "����", item, idx, false );
						SetItemWarnFrame("PlayerContainerFrame");
						LockItem( item:getListType(), item:getGridIdx(), item:getItemId(), "ImportantItemConfirmFrame" );
					else
						MessageBox("��ʾ","�Ƿ�Ҫ��������Ʒ!");
						MessageBoxFrame:SetClientUserData( 0,nSrcList );
						MessageBoxFrame:SetClientUserData( 1,nSrcGrid );
						LockItem( item:getListType(), item:getGridIdx(), item:getItemId(), "MessageBoxFrame" );
						MessageBoxFrame:SetClientString("ȷ��������Ʒ");
					end
				end
			end
		else
			ShowMidTips( "����Ʒ���ɶ���" );
		end
		GameMgr:cancelNextSelect();
	end

	if ( arg1 == "LeftButton" )then
		-- �����Ǵ��ڶ������״̬���Ҳ�����˵�����
		local rider	= mainplayer:getRider();
		if mainplayer:isInRide() and not rider:isOwnRide() then
			GameMgr:cancelNextSelect();
		end
	end
	if(arg1 == "LeftButton" and nSrcList == DRAG_AUTO_ASSIST ) then
		local srcBtn 	  = getglobal("AutoAssistFrame_SkillBtn"..UIGetDragUserData1());
		local srcIconTex  = getglobal(srcBtn:GetName().."Icon");
		t_btnAutoAssistSkillToResult[UIGetDragUserData1()]["SkillId"] = 0;
		t_btnAutoAssistSkillToResult[UIGetDragUserData1()]["SkillLv"] = 0;
		srcIconTex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
		srcBtn:SetCooldownTimer(0, 0, true);
		if UIGetDragUserData1() ~= 11 then
			AutoAssist:getAutoAssistOpt().m_nSkillId[UIGetDragUserData1()-1] = 0;
			AutoAssist:saveAutoAssistOpt();
		end
		srcBtn:SetClientUserData(0, 0);
		srcBtn:SetClientUserData(1, 0);
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		GameMgr:cancelNextSelect();
	end
end

function TropItem()
	if nNum ~= 0 then
		local MainPlayer = ActorMgr:getMainPlayer();
		if MainPlayer == nil then return end
		local container = MainPlayer:getContainer();
		if container == nil then return end
		--container:send_MoveItem( srcList(Դ����), desList��Ŀ��������, srcGrid��Դ���ӣ�, Num��������, desGrid��Ŀ����ӣ� );
		container:send_MoveItem( nSrcList, -1, nSrcGrid, nNum, 0 );
	end
end

local bShowAnim = false;
function util.FadeSelect( texture, fadeData )
	local fCurTime	= GameMgr:getTickTime();
	local fNewAlpha;
	local betweenTime;
	-- ���Ƚ��е���
	if fadeData["FadeIn"] ~= nil then
		
		if fadeData["FadeIn"].initTime == 0 then
			-- ����ʼ������
			fadeData["FadeIn"].compareTime	= fCurTime;
			fadeData["FadeIn"].initTime		= fCurTime;
			texture:SetBlendAlpha( fadeData["FadeIn"].startAlpha );
			return;
		end
		
		-- �������仯��ʱ��ﵽָ������仯ʱ�䣬��ֹͣ����
		if fCurTime - fadeData["FadeIn"].initTime >= fadeData["FadeIn"].lastTime then
			fadeData["FadeIn"] = nil;
			return;
		end

		betweenTime = fCurTime - fadeData["FadeIn"].compareTime 
		if betweenTime >= fadeData["FadeIn"].changeTime then
			fNewAlpha =  texture:GetBlendAlpha() + betweenTime * fadeData["FadeIn"].averageChangeAlpha / fadeData["FadeIn"].changeTime;
			texture:SetBlendAlpha( fNewAlpha > fadeData["FadeIn"].endAlpha and fadeData["FadeIn"].endAlpha or fNewAlpha );
			fadeData["FadeIn"].compareTime = fCurTime;
		end
		return;
	end

	-- ��α��������״̬
	if fadeData["Keep"] ~= nil then
		if fadeData["Keep"].initTime == 0 then
			-- ����ʼ������
			fadeData["Keep"].compareTime	= fCurTime;
			fadeData["Keep"].initTime		= fCurTime;
			texture:SetBlendAlpha( fadeData["Keep"].keepAlpha );
			return;
		end

		-- ��������ʱ��ﵽָ���������ʱ�䣬��ֹͣ����ĳ��alphaֵ
		if fCurTime - fadeData["Keep"].initTime >= fadeData["Keep"].lastTime then
			fadeData["Keep"] = nil;
			return;
		end

		fadeData["Keep"].compareTime = fCurTime;
		return;
	end

	-- ��󵭳�
	if fadeData["FadeOut"] ~= nil then
		if fadeData["FadeOut"].initTime == 0 then
			-- ����ʼ������
			fadeData["FadeOut"].compareTime = fCurTime;
			fadeData["FadeOut"].initTime	= fCurTime;
			texture:SetBlendAlpha( fadeData["FadeOut"].startAlpha );
			return;
		end

		-- �������仯��ʱ��ﵽָ������仯ʱ�䣬��ֹͣ����
		if fCurTime - fadeData["FadeOut"].initTime >= fadeData["FadeOut"].lastTime then
			fadeData["FadeOut"] = nil;
			return;
		end

		betweenTime = fCurTime - fadeData["FadeOut"].compareTime 
		if betweenTime >= fadeData["FadeOut"].changeTime then
			fNewAlpha =  texture:GetBlendAlpha() - betweenTime * fadeData["FadeOut"].averageChangeAlpha / fadeData["FadeOut"].changeTime;
			texture:SetBlendAlpha( fNewAlpha < fadeData["FadeOut"].endAlpha and fadeData["FadeOut"].endAlpha or fNewAlpha );
			fadeData["FadeOut"].compareTime = fCurTime;
		end
		return;
	end
	
	-- ��ִ�е��⣬��˵���ؼ��ĵ��뵭��Ч����ȫ������������տؼ���ռ�ݵ�λ��
	texture:Hide();
	t_fadeTextures[texture:GetName()] = nil;
end

function UIClient_OnUpdate()
	if not bShowAnim and GameMgr:isInPlayCamerAnim() then
		bShowAnim = true;
	end

	if bShowAnim and not GameMgr:isInPlayCamerAnim() then
		bShowAnim = false;
	end

	-- ����Ƿ���Ҫ���뵭��������
	for szTextureName, data in pairs( t_fadeTextures ) do
		local texture = getglobal( szTextureName );
		-- ��������ʾ�������
		if texture:IsShown() then
			util.FadeSelect( texture, data );
		end
	end

	-- ����Ƿ��г�������Ҫ�رյ����
	CheckFrameToShutDownByDist();

	-- ��Ļ�ֱ��ʱ仯��ʱ�� UIClient Ҫ���ű仯 �������Ľ�������ò���UIClient
	local nScreenWidth		= math.floor(GetScreenWidth()/GetScreenScaleY());
	local nScreenHeight		= math.floor(GetScreenHeight()/GetScreenScaleY());
	if this:GetWidth() ~= nScreenWidth or this:GetHeight() ~= nScreenHeight then
		local offsetx = (GetScreenWidth() - this:GetWidth())*GetScreenScaleY();
		local offsety = (GetScreenHeight() - this:GetHeight())*GetScreenScaleY();
		local t_ReSizeFrame = 
				{
					{ rich = "TipsEffectFrameGuideFont", frame = getglobal("TipsEffectFrameGuide"), widthextend = 10 },
					{ rich = "PetDialogFrameGuideFont", frame = getglobal("PetDialogFrameGuide"), widthextend = 10 },
					{ rich = "EffectFrameGuideFont", frame = getglobal("EffectFrameGuide"), widthextend = 0 },
				}
		for _, data in ipairs(t_ReSizeFrame) do
			if data.frame:IsShown() then
				ReSizeTooltip( data.rich, data.frame:GetName(), data.widthextend );
			end
		end		
	end

	-- ����������ʾ����
	local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
	t_centerXinShouBtnControl:update();

	local t_AutoRoundWorlldControl = GetAutoRoundWorlldControl();
	t_AutoRoundWorlldControl:update();
	
	local t_showTmpXinShouControl = GetShowTmpXinShouControl();
	t_showTmpXinShouControl:update();

	local t_taskLimitControl = GetTaskLimitControl();
	t_taskLimitControl:update();

	local t_DropPackageCursorControl = GetDropPackageCursorControl();
	t_DropPackageCursorControl:update();

	local t_trackRoleControl = GetTrackRoleControl();
	t_trackRoleControl:update();

	local t_PetAutoRecoverFrameControl = GetPetAutoRecoverFrameControl();
	t_PetAutoRecoverFrameControl:update();

	if os.clock() - nPlayGameTime > 15*60 then
		ShowMidTips("����Ϸ�ʺ�18��������Ⱥ��δ���������Ծ��뿪PKϵͳ��");
		nPlayGameTime = os.clock();
	end
end

function util.isInFadeState( szTextureName )
	return t_fadeTextures[szTextureName] ~= nil;
end

function util.stopFace( szTextureName )
	t_fadeTextures[szTextureName] = nil;
end
