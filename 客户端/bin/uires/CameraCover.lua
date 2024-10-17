
local tIntroText = {}
tIntroText[1] = {	nTime=4.3,
					text= {
						"ĩ�յ��Ѻ�����ؿ��������������ɫ�����Ҵ����������ĥ�ѣ�",
						"���ڣ��ڰ���������צ�������ؽ����������������ı��ݡ�",
						"ʱ��ǵı����߼ᶨ�ص�����һ�и�����в���Ǽ�԰��а��������",
						"���������ϣ�����¼�Ԫ�����������������������С���",
					},
				}

tIntroText[104] = {	nTime=4.5,
					text= {
						"��ϲ��ͨ���˿��飬Ҳ���㲻��֪�������˵ĳ��������ѡ�������",
						"��Ȼ�ڴ�֮ǰ�㾭����һЩ���ۣ����㼴��ǰ��һ��ΰ�󷱻��Ŀ��г��С���ʱ��ǣ�",
						"������������������Ȼ���ڰ����������������������ͼ�ݻ��������Ⲣ��Ӱ�����Ƕ������Ȱ���",
						"�����������Щͷ���и�̾�ŵļһ��ǣ�ͨ�������ǻ�����һЩ����Ķ�����Ҳ���㻹���յ����ص����",
					},
				}

local nStartTime = 0;
local nCurIndex = 0;


function CameraCoverFrameOnLoad()
	this:setUpdateTime(0);
end

local bFristEnterGame = false;
local nIntroIndex = 0;

function ShowCameraCoverFrame( bFirst,IntroIndex )
	bFristEnterGame = bFirst;
	nIntroIndex = IntroIndex;
	CameraCoverFrame:Show();
end

function CameraCoverFrameOnShow()
	--hidden all the game frame
	CloseAllFrames("CameraCoverFrame");
	nStartTime = os.clock();
	CameraCoverFrameFont:SetText("");
	CameraCoverFrameFont:SetBlendAlpha(1.0);
	--���ص�һ������NPC���������
	GameWizard:hideNpcTalkByResId(304001);
	QuestHelperFrame:Hide()
end

function CameraCoverFrameOnHide()
	CheckAddFriendReqestsAfteAnim();
	CheckTeamReqestsAfterAnim();
end

function CanBeginShortcutAction()
	return not CameraCoverFrame:IsShown() and not LoadingFrame:IsShown();
end

local t_hideNPcTalk = 
{ 
304001,	-- ������
};

function EndFirstEnterGameAnim()
	local t_addNpcTalk =
	{	resId = 304001, content = "����������",  popBackRich = "ChatPop", showOnce = true, headOffsetX = 12, 
		headOffsetY = -100, arrowTexture = "PopBackArrowTexture", showArrowTex = true, texOffsetX = 0, texOffsetY = 0, 
		wizardOp = { GAMEWIZARD_FIRST_GETMISSION },
	};
	SetNpcWithoutQuestPopByState( t_addNpcTalk );
end

function CameraCoverFrameOnUpdate()
	if bFristEnterGame then
		if not GameMgr:isInPlayCamerAnim() then
			CameraCoverFrame:Hide();
			ShowFramesInWorld();
			--Ŀ�½�ɫ�������ſ�����������ʾ���ֻ�ӭ����
			--PlayerFirstBorn();
			nStartTime = 0;
			EndFirstEnterGameAnim();
			local nCurEnterMapID = MapShower:getRealMapId();
			g_nPreShowMapID = nCurEnterMapID;
			FadeSceneNameFrame:Show();
			nIntroIndex = 0;
			if not GameWizard:isDo( GAMEWIZARD_FIRST_POP_MOUSE_HABIT_XINSHOU_FRAME ) then
				ShowMouseHabitFrame();
			end
			UpdatePetPhotoWhenLoginGame();
			--���vip ����
			CheckMainPlayerVipBar();
		end
	else
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer == nil then
			CameraCoverFrame:Hide();
			return;
		end
		if TeamRoleFrame:IsShown() then
			TeamRoleFrame:Hide();
		end
		if not mainplayer:isInFly() then 
			CameraCoverFrame:Hide();
			nIntroIndex = 0;
			ShowFramesInWorld();
			if mainplayer:isInTeam() then
				TeamRoleFrame:Show();
			end
			--QuestHelperFrame:Show()
		end		
	end
	if tIntroText[nIntroIndex] ~= nil then
		local nIndex = math.floor((os.clock() - nStartTime)/tIntroText[nIntroIndex].nTime) + 1;
		if nIndex > #tIntroText[nIntroIndex].text then
			return
			--nIndex = #tIntroText[nIntroIndex];
		end
		if nCurIndex ~= nIndex then
			CameraCoverFrameFont:SetBlendAlpha(0);
			CameraCoverFrameFont:SetText(tIntroText[nIntroIndex].text[nIndex]);
			nCurIndex = nIndex;
		else
			local alpha =(os.clock() - nStartTime) /tIntroText[nIntroIndex].nTime - nCurIndex + 1;
			CameraCoverFrameFont:SetBlendAlpha(alpha);
		end
	end
end
