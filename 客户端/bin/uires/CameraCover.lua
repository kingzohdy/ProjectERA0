
local tIntroText = {}
tIntroText[1] = {	nTime=4.3,
					text= {
						"末日的裂痕深深地刻在这个美丽的蓝色星球，幸存的人们历经磨难，",
						"现在，黑暗文明议会的爪牙正疯狂地进攻着人类文明最后的堡垒。",
						"时间城的保卫者坚定地抵御着一切敢于威胁他们家园的邪恶力量！",
						"在这个充满希望的新纪元，人类文明将由你们来传承……",
					},
				}

tIntroText[104] = {	nTime=4.5,
					text= {
						"恭喜你通过了考验，也许你不会知道，命运的齿轮因你的选择而启动",
						"虽然在此之前你经历过一些波折，但你即将前往一座伟大繁华的空中城市——时间城！",
						"那是我们最后的乐土，然而黑暗文明议会正觊觎着它，企图摧毁它！但这并不影响我们对它的热爱。",
						"你可以留意那些头顶有感叹号的家伙们，通常，他们会送你一些珍奇的东西！也许你还会收到神秘的礼物！",
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
	--隐藏第一个人物NPC的泡泡面板
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
304001,	-- 安东尼
};

function EndFirstEnterGameAnim()
	local t_addNpcTalk =
	{	resId = 304001, content = "点击完成任务",  popBackRich = "ChatPop", showOnce = true, headOffsetX = 12, 
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
			--目新角色出生播放开场动画后，显示新手欢迎界面
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
			--检测vip 网吧
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
