
--------------------------------------------------------------------------------------------------
--快捷指令说明
--------------------------------------------------------------------------------------------------
local QUEST_SPECIAL  = 0;
local QUEST_KILLPICK = 1;
local QUEST_GMFINI   = 2;
local QUEST_FSPECIAL = 3;
local bAuto        = false;--是否自动做任务的标志
local nCurQuestId  = -1;--当前执行的任务ID
local nAcQuestId   = -1;--当前去接取的任务ID
local GMmonid	   =  -1;--当前任务目标怪物的ID
local bAutoQuest   = false;  --自动任务标志
local bAutoAttack  = false;  --自动攻击标志
local nAttackId    = 0;     --自动攻击怪物的ID	
local bAutocollect = false;  --自动采集标志
local nCollectId   = 0;     --自动采集物的ID
local nGMAdistence = 6000;
local nGMlasttime  = -1;
local bAutoFind    = false;
local nfstep	   = 0;
local nsstep	   = 0;
local GMresultfinway = 0; --自动寻路成功的标志
local GMCommandID  = -1;
local GMRandomnum  = 0;
local GMCounts     = 0;
local GMTargetid   = 0;
local specialquestlist={
--杀怪捡取物品类任务
[1065]={ QuestType=QUEST_KILLPICK, Monsterid1=231021 },
[1080]={ QuestType=QUEST_KILLPICK, Monsterid1=200008},
[1307]={ QuestType=QUEST_KILLPICK, Monsterid1=232005},
[1338]={ QuestType=QUEST_KILLPICK, Monsterid1=230001},
[1339]={ QuestType=QUEST_KILLPICK, Monsterid1=232000},
[1340]={ QuestType=QUEST_KILLPICK, Monsterid1=232001},
[1408]={ QuestType=QUEST_KILLPICK, Monsterid1=205003},
[1410]={ QuestType=QUEST_KILLPICK, Monsterid1=231023},
[1423]={ QuestType=QUEST_KILLPICK, Monsterid1=231023},

--直接执行GM指令完成的任务
[1417]={ QuestType=QUEST_GMFINI},

--特殊处理的任务
[1072]={QuestType=QUEST_SPECIAL,

	func= function ()
			if GMNoFinFindway() then
				return;
			end
			if nfstep == 0 then
				--GMLinktoNpcPostion(2000,"尤利西斯");
				nfstep=nfstep+1;
			elseif nfstep == 1 then
				local mainplayer = ActorMgr:getMainPlayer();
				local actor=GMGetActorByResId(306002);
				if actor ~= nil then
					mainplayer:SetSelectTarget( actor:GetID() );
					nfstep = nfstep +1;
				end
			else
				this=getglobal("TaskItemBtn1");
				TaskItem_OnClick();
			end
			
	end},
[1073]={QuestType=QUEST_SPECIAL,

	func= function ()
			if GMNoFinFindway() then
				return;
			end
			if nfstep == 0 then
				local actor = GMGetMonsiterByResId(250001);
				GMlocked(actor:GetID());
				nfstep=nfstep+1;
			elseif nfstep == 1 then
				local actor = ActorMgr:FindActor( nAttackId );
				if GMlinkpos( 2000, actor:getPlayerX() / 10, actor:getPlayerZ() / 10 ) == MOVE_ALREADY_IN_DEST then
					nfstep=nfstep+1;
				end
			else
				local actor = ActorMgr:FindActor( nAttackId );
				if actor:getHP() >= actor:getMaxHP()  then
					GameShortCut:useSkill(GameShortCut:getNormalAttackSkill());
					return;
				end
				GMlocked(nAttackId);
				this=getglobal("TaskItemBtn1");
				TaskItem_OnClick();
			end
	end},

[1056]={ QuestType=QUEST_SPECIAL,

	func = function ()
			GMOpenBag();
			local mainplayer = ActorMgr:getMainPlayer();
			GMUseIteminBag(Quest:getQuestSpecialFiniObjVal(nCurQuestId,mainplayer:getRace() - 1));
		end},


[1061]={ QuestType=QUEST_SPECIAL,
	func = function ()
			local mainplayer = ActorMgr:getMainPlayer();
			--local actor=GMGetMonsiterByResId(Quest:getQuestSpecialFiniObjVal(nCurQuestId,0));
			local wId=ActorMgr:getActorIdByResId(Quest:getQuestSpecialFiniObjVal(nCurQuestId,0));
			mainplayer:SetSelectTarget(wId);--actor:GetID());
			GameShortCut:useSkill(Quest:getQuestSpecialFiniConditionVal(nCurQuestId,mainplayer:getRace() - 1));
		end},
[1062]={ QuestType=QUEST_FSPECIAL,
	func = function ()
			if GMISinAutoFindWay() then 
				return  true;
			end
			if nfstep == 0 then
				MapShower:GetMaxMapContal():linkMoveto( 2000,302016 );
				nfstep = nfstep + 1;
			elseif nfstep == 1 then
				 if GMNpcTalk(2) then
					nfstep = nfstep + 1;
				end
			elseif nfstep == 2 then
				if TaskAcceptFrame:IsShown() then
					TaskAcceptFrame_AcceptBtn_OnClick();
					nfstep = nfstep + 1;
				end
			elseif nfstep == 3 then
				MapShower:GetMaxMapContal():linkMoveto( 2000,302016 );
				nfstep = nfstep + 1;
			elseif nfstep == 4 then
				GMNpcTalk(1);
				local mainplayer = ActorMgr:getMainPlayer();
				if mainplayer:isInFly() then 
					nfstep = nfstep + 1;
				end
			elseif nfstep == 5 then
				local mainplayer = ActorMgr:getMainPlayer();
				if mainplayer:isInFly() then 
					return;
				end
				local container = mainplayer:getContainer();
				if container:getItemCount(5300011) >= 10 then
					nfstep = nfstep + 1;
					return ;
				end
				if GMSelfdDfence() then
					return;
				end
				if GMGetItemByid(5300011) then
					return;
				end
				local mon = GMGetNearestMon();
				if mon ~= nil then 
					GMGotoActor(mon);
					GMlocked(mon:GetID());
				end
			elseif nfstep == 6 then
				NpcDialog:talkByResId(302006);
				nfstep = nfstep + 1;
			elseif nfstep == 7 then
				if GMNpcTalk(1) then
					nfstep = nfstep + 1;
				end
			elseif nfstep == 8 then
				local mainplayer = ActorMgr:getMainPlayer();
				if mainplayer:isInFly() then 
					return;
				end
				if GMSelfdDfence() then
					GameShortCut:usedShortCut(0,0);
					return;
				end
				local mon = GMGetNearestMon();
				if mon ~= nil then
					GMGotoActor(mon);
					GMlocked(mon:GetID());
					return;
				end
				local actor = GMGetActorByResId(293026);
				if actor ~= nil then
					GameCollect:collect(actor:GetID());
					nfstep = nfstep + 1;
				end
			elseif nfstep == 9 then
				if not GameCollect:isInCollect() then
						if MessageBoxFrame:IsShown() then
							MessageBoxAcceptBtnOnClick();
							nfstep = nfstep + 1;
						end
						local actor = ActorMgr:findActorByName("传送门");
						if actor ~= nil then
							NpcDialog:talkByMemId(actor:GetID());
							return;
						end
				end
			elseif nfstep == 10 then
					if NpcTalkFrame:IsShown() then
						this=getglobal("NpcTalkSelectSelectItem1");
						NpcTalkOpt_OnClick();
					end
					if TaskAwardFrame:IsShown() then
						if TaskAwardFrame_GetBtn:IsShown() then
							TaskAwardFrame_GetBtn_OnClick();
						end
					end
					MapShower:GetMaxMapContal():linkMoveto( 2000,302016 );
					if Quest:getNumQusetRun() <=	0 then
						GMClear();
					end
			end
		end},
[1075]={ QuestType=QUEST_FSPECIAL,
	func = function ()
			if GMNoFinFindway() then
				return;
			end
			print(nfstep);
			if nfstep == 0 then
				if NpcTalkFrame:IsShown() then
					this=getglobal("NpcTalkSelectSelectItem"..GMFindLastdia());
					NpcTalkOpt_OnClick();
				end
				nfstep = nfstep + 1;
			elseif nfstep == 1 then
				if GMlinkpos(GMGetCurMapId(),17988,42972) == MOVE_SUCCESS then 
					nfstep = nfstep + 1;
				end
			elseif nfstep == 2 then
			--	local actor = GMGetActorItem();
			--	if actor ~= nil then
				--	GMGotoActor(actor);
					nfstep = nfstep + 1;
			--	end
			elseif nfstep == 3 then
				if GMSelfdDfence() then
					GameShortCut:usedShortCut(0,0);
					return;
				end
				local mon = GMGetNearestMon();
				if mon ~= nil then 
					GMGotoActor(mon);
					GMlocked(mon:GetID());
					return;
				end
				if Quest:getQuestRunState(nCurQuestId) ==  CLIENT_TASK_STAT_FINI then
					if GMlinkpos(GMGetCurMapId(),26488,46260) == MOVE_SUCCESS then
						nfstep = nfstep + 1;
					end
				end
				if GMRandomnum == 0 then
					GMlinkpos(GMGetCurMapId(),23378,43291);
				elseif GMRandomnum == 1 then
					GMlinkpos(GMGetCurMapId(),25054,43292);
				end
				GMRandomnum = ( GMRandomnum + 1 ) % 2;
			elseif nfstep == 4 then
				if MessageBoxFrame:IsShown() then
					MessageBoxAcceptBtnOnClick();
					nfstep = nfstep + 1;
				end
				local actor = GMGetActorByResId(333015);
				if actor ~= nil then
					NpcDialog:talkByMemId(actor:GetID());
					return;
				end
			elseif nfstep == 5 then
				if NpcTalkFrame:IsShown() then
					this=getglobal("NpcTalkSelectSelectItem1");
					NpcTalkOpt_OnClick();
				end
				if TaskAwardFrame:IsShown() then
					if TaskAwardFrame_GetBtn:IsShown() then
						TaskAwardFrame_GetBtn_OnClick();
						GMClear();
					end
				end
				MapShower:GetMaxMapContal():linkMoveto( 2000,302005 );

			end
		end},
[1077]={ QuestType=QUEST_SPECIAL,
	func = function ()
			if GMNoFinFindway() then
				return;
			end
			if nfstep ==0 then
				MapShower:GetMaxMapContal():linkMoveto( 2000, 303021);
				nfstep = nfstep +1;
			elseif nfstep == 1 then
					if NpcTalkFrame:IsShown() then
						this=getglobal("NpcTalkSelectSelectItem1");
						NpcTalkOpt_OnClick();
						nfstep = nfstep +1;
					end
			else
				MapShower:GetMaxMapContal():linkMoveto( 1000, 306000 );
			end
		end},
[1310]={ QuestType=QUEST_SPECIAL,
	func = function ()
			if GMNoFinFindway() then
				return;
			end
			if nfstep == 0 then
				local str = Quest:getQuestNoFiniTraceDespos(nCurQuestId);
				GMAutoFindWay(str);
				nfstep = nfstep + 1;
			elseif nfstep == 1 then
				local actor = GMGetActorByResId(230008);
				local mainplayer = ActorMgr:getMainPlayer();	
				mainplayer:SetSelectTarget( actor:GetID() );
				GameCollect:collect(actor:GetID());
				nfstep = nfstep + 1;
			elseif not GameCollect:isInCollect() then
				GameCollect:collect(GMGetTargetId());
			end
		end},
[1313]={ QuestType=QUEST_SPECIAL,
	func = function ()
			if GMNoFinFindway() then
				return;
			end
			if GMSelfdDfence() then
				return;
			end
			if nfstep == 0 then
				if GMGetItemcounts(5300022) >= 5 then
					nfstep = nfstep + 1;
					return;
				end
				if GMGetItemByid(5300022) then
					return;
				end
				actor = GMGetMonsiterByResId(205000);
				if actor ~= nil then
					GMlocked(actor:GetID());
					return;
				end
			else
				GMCollectUpdate();
				--[[if GameCollect:isInCollect() then
					return;
				end
				local actor = GMGetActorByResId(293050);
				if nCollectId ~= GMGetTargetId() then
					if actor ~= nil then
						if actor:canCollect()  then
							nCollectId = actor:GetID();
							GameCollect:collect(actor:GetID());
							GMlocked(actor:GetID());
							return;
						end
				end--]]
			end
			--local str = Quest:getQuestNoFiniTraceDespos(nCurQuestId);
			--GMAutoFindWay(str);
		end},
[1314]={ QuestType=QUEST_SPECIAL,
	func = function ()
			if GMNoFinFindway() then
				return;
			end
			if nfstep == 0 then
				MapShower:GetMaxMapContal():linkMoveto( 2100, 16200, 17700 );
				nfstep = nfstep + 1;
				return;
			end
			if GMSelfdDfence() then
				return;
			end
			if GameCollect:isInCollect() then
				return;
			end
			actor = GMGetActorByResId(293076);
			if actor ~= nil then
				if actor:canCollect()  then
					GameCollect:collect(actor:GetID());
					return;
				end
			end
		end},
[1316]={ QuestType=QUEST_SPECIAL,
	func = function ()
			if nfstep == 0 then
				if GMNoFinFindway() then
					return;
				end
				local actor = GMGetActorByResId(204000);
				if actor ~= nil then
					GMTargetid = actor:GetID();
					GMlocked(actor:GetID());
					MapShower:GetMaxMapContal():linkMoveto( 2100, actor:getPlayerX() / 10, actor:getPlayerZ() / 10 );
					nfstep = nfstep + 1;
				end
				return;
			elseif nfstep ==	1 then
				GMlocked(GMTargetid);
				this=getglobal("TaskItemBtn1");
				TaskItem_OnClick();
				local actor = GMGetActorByResId(293086);
				if actor ~= nil  then
					GMlocked(actor:GetID());
					GameCollect:collect(actor:GetID());
					nfstep = nfstep + 1;
				end
				return;
			elseif nfstep == 2 then
				if GameCollect:isInCollect() then
					nfstep = nfstep + 1;
				else
					GameCollect:collect(GMGetTargetId());
				end
				return;
			elseif nfstep == 3 then
				if not GameCollect:isInCollect() then
					MapShower:GetMaxMapContal():linkMoveto( 2100,9400,14200 );
					nfstep = nfstep + 1;
				end
				return;
			elseif nfstep == 4 then
				if GMISinAutoFindWay() then
					return;
				end
				local actor = GMGetActorByResId(293088);
				if actor ~= nil  then
					GMlocked(actor:GetID());
					GameCollect:collect(actor:GetID());
				end
			end
			
		end},
[1318]={ QuestType=QUEST_SPECIAL,
	func = function ()
			if nfstep == 0 then
				MapShower:GetMaxMapContal():linkMoveto(2100,302024);
				nfstep = nfstep + 1
				return;
			elseif nfstep == 1 then
				if GMISinAutoFindWay() then
					return;
				end
				if NpcTalkFrame:IsShown() then
					this=getglobal("NpcTalkSelectSelectItem2");
					NpcTalkOpt_OnClick();
					nfstep = nfstep + 1;
				end
				return;
			elseif nfstep == 2 then
				if NpcTalkFrame:IsShown() then
					this=getglobal("NpcTalkSelectSelectItem1");
					NpcTalkOpt_OnClick();
					nfstep = nfstep + 1;
				end
				return;
			elseif nfstep == 3 then
					local counts = ActorMgr:getActorNums() - 1;
					for i=0,counts do
						local actor= ActorMgr:getActor(i);
						if actor:getRealId() == 293087 and actor:canCollect() then
							GMlocked(actor:GetID());
							nfstep = nfstep + 1;
							MapShower:GetMaxMapContal():linkMoveto( 2100, actor:getPlayerX() / 10, actor:getPlayerZ() / 10 );
							return;
						end
					end
					return;
			else
				if GMISinAutoFindWay() then
					return;
				end
				this=getglobal("TaskItemBtn1");
				TaskItem_OnClick();
					return;
			end
			
		end},
[1320]={ QuestType=QUEST_SPECIAL,
	func = function ()
			if nfstep == 0 then
				MapShower:GetMaxMapContal():linkMoveto(2100,306003);
				nfstep = nfstep + 1
				return;
			elseif nfstep == 1 then
				if GMISinAutoFindWay() then
					return;
				end
				if NpcTalkFrame:IsShown() then
					this=getglobal("NpcTalkSelectSelectItem2");
					NpcTalkOpt_OnClick();
					nfstep = nfstep + 1;
					itemid = 5300388;
				end
				return;
			elseif nfstep == 2 then
				if ShopFrame:IsShown() then
					GMDealShop();
				else
					MapShower:GetMaxMapContal():linkMoveto(2100,9455,20493);
					nfstep = nfstep + 1;
				end
				return;
			elseif nfstep == 3 then
				if GMISinAutoFindWay() then
					return;
				end
				GMCollectUpdate();
				return;
			end
			
		end},
[1321]={ QuestType=QUEST_SPECIAL,
	func = function ()
			if GMNoFinFindway() then
				return;
			end
			local mainplayer = ActorMgr:getMainPlayer();
			local container = mainplayer:getContainer();
			if container:getItemCount(5300380) <= 0 then
				GMCollectUpdate();
			else
				this=getglobal("TaskItemBtn1");
				TaskItem_OnClick();
			end
			
		end},
[1323]={ QuestType=QUEST_SPECIAL,
	func = function ()
			if GMNoFinFindway() then
				return;
			end
			if nfstep ==0 then
				local actor = GMGetActorByResId(293090);
				if actor ~= nil then
					GMlocked(actor:GetID());
					this=getglobal("TaskItemBtn1");
					TaskItem_OnClick();
					nfstep = nfstep + 1;
				end
			elseif nfstep == 1 then
				local counts = ActorMgr:getActorNums() - 1;
				for i=0,counts do
					local actor= ActorMgr:getActor(i);
					if actor:getRealId() == 313000 and actor:getType() == GAT_NPC then
						NpcDialog:talkByMemId(actor:GetID());
						nfstep = nfstep + 1;
						return;
					end
				end
			elseif nfstep == 2 then
				if NpcTalkFrame:IsShown() then
					this=getglobal("NpcTalkSelectSelectItem1");
					NpcTalkOpt_OnClick();
				end
			end
			
		end},
[1326]={ QuestType=QUEST_FSPECIAL,
	func = function ()
			if GMNoFinFindway() then
				return;
			end
			if nfstep == 0 then
				if NpcTalkFrame:IsShown() then
					this=getglobal("NpcTalkSelectSelectItem"..GMFindLastdia());
					NpcTalkOpt_OnClick();
				end
				if GMGetCurMapId() ~= 2100 then 
					nfstep = nfstep + 1;
				end
			elseif nfstep == 1 then
				GMZhanGuCun();
				if GMGetCurMapId() == 2100 then 
					if not Quest:isQuestRun(nCurQuestId) then
						nfstep = nfstep + 1;
					else
						nfstep = 0;
						bAutoFind = false;
					end
				end
			elseif nfstep == 2 then
				GMClear();
			end	
		end},
[1327]={ QuestType=QUEST_FSPECIAL,
	func = function ()
			if GMNoFinFindway() then
				return;
			end
			if nfstep == 0 then
				if NpcTalkFrame:IsShown() then
					this=getglobal("NpcTalkSelectSelectItem"..GMFindLastdia());
					NpcTalkOpt_OnClick();
				end
				if GMGetCurMapId() ~= 2100 then 
					nfstep = nfstep + 1;
				end
			elseif nfstep == 1 then
				GMZhanGuCun();
				if GMGetCurMapId() == 2100 then 
					if Quest:getQuestRunState(nCurQuestId) ==  CLIENT_TASK_STAT_FINI then
						nfstep = nfstep + 1;
					else
						nfstep = 0;
						bAutoFind = false;
					end
				end
			elseif nfstep == 2 then
					if NpcTalkFrame:IsShown() then
						this=getglobal("NpcTalkSelectSelectItem1");
						NpcTalkOpt_OnClick();
					end
					if TaskAwardFrame:IsShown() then
						if TaskAwardFrame_GetBtn:IsShown() then
							TaskAwardFrame_GetBtn_OnClick();
							GMClear();
						end
					end
					MapShower:GetMaxMapContal():linkMoveto(2100,307000);
			end	
		end},
[1334]={ QuestType=QUEST_FSPECIAL,
	func = function ()
			if GMNoFinFindway() then
				return;
			end
			if nfstep == 0 then
				if NpcTalkFrame:IsShown() then
					this=getglobal("NpcTalkSelectSelectItem"..GMFindLastdia());
					NpcTalkOpt_OnClick();
				end
				if GMGetCurMapId() ~= 2100 then 
					nfstep = nfstep + 1;
				end
			elseif nfstep == 1 then
				GMZhanGuCun();
				if GMGetCurMapId() == 2100 then 
					if Quest:getQuestRunState(nCurQuestId) ==  CLIENT_TASK_STAT_FINI then
						nfstep = nfstep + 1;
					else
						nfstep = 0;
						bAutoFind = false;
					end
				end
			elseif nfstep == 2 then
					if NpcTalkFrame:IsShown() then
						this=getglobal("NpcTalkSelectSelectItem1");
						NpcTalkOpt_OnClick();
					end
					if TaskAwardFrame:IsShown() then
						if TaskAwardFrame_GetBtn:IsShown() then
							TaskAwardFrame_GetBtn_OnClick();
							GMClear();
						end
					end
					MapShower:GetMaxMapContal():linkMoveto(2100,303005);
			end	
		end},
[1331]={ QuestType=QUEST_SPECIAL,
	func = function ()
			if GMNoFinFindway() then
				return;
			end
			local actor = ActorMgr:FindActor( nAttackId );
			if actor ~= nil and actor:getSubType() ~= 102 and nAttackId == GMGetTargetId() and not GMActorIsDead( nAttackId ) then
				GMattack();
				return;
			end
			 actor = GMGetMonAttackplayer();
			if actor ~= nil  then
				GMlocked(actor:GetID());
				return;
			end
			if GameCollect:isInCollect() then
				return;	
			end
			local actor = GMGetActorByResId(293085);
			if actor ~= nil then
				GMlocked(actor:GetID());
				GameCollect:collect(actor:GetID());
			end
		end},
[1335]={ QuestType=QUEST_SPECIAL,
	func = function ()
			if GMNoFinFindway() then
				return;
			end
			if GMSelfdDfence() then
				return;
			end
			GMCollectUpdate();
			--[[if GameCollect:isInCollect() then
				return;
			end
			if nCollectId ~=0 and nCollectId ~= GMGetTargetId() then
				actor = GMGetActorByResId(250003);
				if actor ~= nil then
					if actor:canCollect()  then
						nCollectId = actor:GetID();
						GameCollect:collect(actor:GetID());
						GMlocked(actor:GetID());
						return;
					end
				end
			else 
				GameCollect:collect(nCollectId);
				return;
			end--]]
		end},
[1337]={ QuestType=QUEST_SPECIAL,
	func = function ()
			if GMNoFinFindway() then
				return;
			end
			if NpcTalkFrame:IsShown() then
				this=getglobal("NpcTalkSelectSelectItem1");
				NpcTalkOpt_OnClick();
			end
			if NpcLearnSkillFrame:IsShown() then
				NpcLearnSkill_Study_OnClick();
			end
		end},
[1411]={ QuestType=QUEST_SPECIAL,
	func = function ()
			if GMNoFinFindway() then
				return;
			end
			GMGetItemByid(5300288);
			GMSelfdDfence();
			local actor=GMGetActorByResId(303048);
			if actor ~= nil then
				GMlocked(actor:GetID());
				return;
			end
			local actor=GMGetActorByResId(230011);
			if actor ~= nil then
				GMlocked(actor:GetID());
				return;
			end

		end},
[1414]={ QuestType=QUEST_SPECIAL,
	func = function ()
			if GMNoFinFindway() then
				return;
			end
			if nfstep == 0 then
				local actor=GMGetActorByResId(230012);
				if actor ~= nil then
					GMlocked(actor:GetID());
					GMGotoActor(actor);
					nfstep = nfstep + 1;
					return;
				end
			elseif nfstep == 1 then
				this=getglobal("TaskItemBtn1");
				TaskItem_OnClick();
				if GMGetTargetId() == 0 then
					local actor=GMGetActorByResId(230012);
					if actor ~= nil then
						GMlocked(actor:GetID());
						GMGotoActor(actor);
						nfstep = nfstep + 1;
					end
				end
			end
		end},

[1416]={ QuestType=QUEST_SPECIAL,
	func = function ()
			if GMNoFinFindway() then
				return;
			end
			if nfstep == 0 then
				GMlinknpc(2200,302028);
				if NpcTalkFrame:IsShown() then
					this=getglobal("NpcTalkSelectSelectItem"..GMFindLastdia());
					NpcTalkOpt_OnClick();
					nfstep = nfstep + 1;
				end
			elseif nfstep == 1 then
				if NpcTalkFrame:IsShown() then
					this=getglobal("NpcTalkSelectSelectItem1");
					NpcTalkOpt_OnClick();
					return;
				end
				if GMGetItemcounts(5300291) > 0 then
					GMlinkpos(2200,16559,19274);
					nfstep = nfstep + 1;
				end
			elseif nfstep == 2 then		
				GMGetItemByid(5300290);
				GMSelfdDfence();
				local actor = GMGetActorByResId(218005);
				if actor ~= nil then
					GMlocked(actor:GetID());
				end
			end	
		end},
[1424]={ QuestType=QUEST_SPECIAL,
	func = function ()
			if GMNoFinFindway() then
				return;
			end
			if nfstep == 0 then
				GMlinknpc(2200,304008);
				nfstep = nfstep + 1;
			else 
				if NpcTalkFrame:IsShown() then
					this=getglobal("NpcTalkSelectSelectItem"..GMFindLastdia());
					NpcTalkOpt_OnClick();
				end
			end
		end},
[1431]={ QuestType=QUEST_SPECIAL,
	func = function ()
			if GMNoFinFindway() then
				return;
			end
			if nfstep == 0 then
				local actor = GMGetActorByResId(293063);
				if actor ~= nil then
					GMlocked(actor:GetID());
					GMGotoActor(actor);
					nfstep = nfstep + 1;
				end
			else 
				this=getglobal("TaskItemBtn1");
				TaskItem_OnClick();
			end
		end},
[1432]={ QuestType=QUEST_SPECIAL,
	func = function ()
			if GMNoFinFindway() then
				return;
			end
			GMSelfdDfence();
			local actor = GMGetActorByResId(236000);
			if actor ~= nil then
				GMlocked(actor:GetID());
				return;
			end
			if GMIsInUseItem() then
				return;
			end
			if nCollectId ~= 0 then 
				actor = ActorMgr:FindActor( nCollectId );
				if actor ~= nil then
					GMGotoActor(actor);
					this=getglobal("TaskItemBtn1");
					TaskItem_OnClick();
				end
			end
			actor = GMGetActorByResId(293016);
			if actor ~= nil then
				nCollectId = actor:GetID();
				local mainplayer = ActorMgr:getMainPlayer();
				mainplayer:SetSelectTarget( nCollectId );
				GMGotoActor(actor);
				this=getglobal("TaskItemBtn1");
				TaskItem_OnClick();
			end
		end},
[1434]={ QuestType=QUEST_SPECIAL,
	func = function ()
			if GMNoFinFindway() then
				return;
			end
			if GMGetTargetId() ~=0 and nCollectId == GMGetTargetId() then 
				this=getglobal("TaskItemBtn1");
				TaskItem_OnClick();
				return;
			end
			actor = GMGetActorByResId(293016);
			if actor ~= nil then
				nCollectId = actor:GetID();
				local mainplayer = ActorMgr:getMainPlayer();
				mainplayer:SetSelectTarget( nCollectId );
				GMGotoActor(actor);
			end
		end},	
[1494]={ QuestType=QUEST_SPECIAL,
	func = function ()
			if GMISinAutoFindWay() then 
				return;
			end
			if nfstep == 0 then
				GMlinkpos(2200,21582,17721);
				nfstep = nfstep + 1;
				return;
			end
			if  GameCollect:isInCollect()  then
				return;
			end
			actor = GMGetActorByResId(293060);
			if actor ~= nil then
				nCollectId = actor:GetID();
				local mainplayer = ActorMgr:getMainPlayer();
				mainplayer:SetSelectTarget( nCollectId );
				GameCollect:collect(GMGetTargetId());
			end
		end},	
[1436]={ QuestType=QUEST_SPECIAL,
	func = function ()
			if GMNoFinFindway() then
				return;
			end
			if GMGetTargetId() ~=0 and nCollectId == GMGetTargetId() then 
				this=getglobal("TaskItemBtn1");
				TaskItem_OnClick();
				return;
			end
			actor = GMGetActorByResId(293064);
			if actor ~= nil then
				nCollectId = actor:GetID();
				local mainplayer = ActorMgr:getMainPlayer();
				mainplayer:SetSelectTarget( nCollectId );
				GMGotoActor(actor);
			end
		end},
[1441]={ QuestType=QUEST_SPECIAL,
	func = function ()
			if GMNoFinFindway() then
				return;
			end
			if GMGetTargetId() ~=0 and nCollectId == GMGetTargetId() then 
				this=getglobal("TaskItemBtn1");
				TaskItem_OnClick();
				return;
			end
			actor = GMGetActorByResId(236002);
			if actor ~= nil then
				nCollectId = actor:GetID();
				local mainplayer = ActorMgr:getMainPlayer();
				mainplayer:SetSelectTarget( nCollectId );
				GMGotoActor(actor);
			end
		end},
[1496]={ QuestType=QUEST_SPECIAL,
	func = function ()
			if GMNoFinFindway() then
				return;
			end
			if nfstep == 0 then
				this=getglobal("TaskItemBtn1");
				TaskItem_OnClick();
				if GMGetItemcounts(5300336) > 0 then
					GMlinkpos(2200,18900,17800);
					nfstep = nfstep + 1;
				end
			elseif nfstep == 1 then
				GMUesTaskItem(293062);
			end
		end},
[1499]={ QuestType=QUEST_FSPECIAL,
	func = function ()
			if GMNoFinFindway() then
				return;
			end
			if nfstep == 0 then
				if NpcTalkFrame:IsShown() then
					this=getglobal("NpcTalkSelectSelectItem"..GMFindLastdia());
					NpcTalkOpt_OnClick();
				end
				if GMGetCurMapId() ~= 2200 then 
					nfstep = nfstep + 1;
				end
			elseif nfstep == 1 then
				GMYongye();
				if GMGetCurMapId() == 2200 then 
					if Quest:getQuestRunState(nCurQuestId) ==  CLIENT_TASK_STAT_FINI then
						nfstep = nfstep + 1;
					else
						nfstep = 0;
						bAutoFind = false;
					end
				end
			elseif nfstep == 2 then
				if NpcTalkFrame:IsShown() then
					this=getglobal("NpcTalkSelectSelectItem1");
					NpcTalkOpt_OnClick();
				end
				if TaskAwardFrame:IsShown() then
					if TaskAwardFrame_GetBtn:IsShown() then
						TaskAwardFrame_GetBtn_OnClick();
						GMClear();
						return;
					end
				end
				MapShower:GetMaxMapContal():linkMoveto(2200,302028);
			end	
		end},
[2241]={ QuestType=QUEST_FSPECIAL,
	func = function ()
			if GMISinAutoFindWay() then 
				return  true;
			end
			if nfstep == 0 then
				MapShower:GetMaxMapContal():linkMoveto(2200,302028);
				nfstep = nfstep + 1;
			elseif nfstep == 1 then
				if NpcTalkFrame:IsShown() then
					this=getglobal("NpcTalkSelectSelectItem"..GMFindLastdia());
					NpcTalkOpt_OnClick();
				end
				if GMGetCurMapId() ~= 2200 then 
					nfstep = nfstep + 1;
				end
			elseif nfstep == 2 then
				GMYongye();
				if GMGetCurMapId() == 2200 then 
					if not Quest:isQuestRun(nCurQuestId) then
						nfstep = nfstep + 1;
					else
						nfstep = 0;
						bAutoFind = false;
					end
				end
			elseif nfstep == 3 then
				GMClear();
			end	
		end},
};


--永夜镇副本
local YongyeFuBen={{
	[1]={x=7344,y=13665},
	[2]={x=74410,y=153000},
	[3]={x=88350,y=160500},
	[4]={x=90500,y=169040},
	[5]={x=85740,y=179400},
	[6]={x=68100,y=172100},
	[7]={x=75260,y=163530},
	BossId = 282013,
	Nums = 7,
},
{
	[1]={x=198470,y=134660},
	[2]={x=193110,y=122330},
	[3]={x=188860,y=112690},
	[4]={x=177070,y=121200},
	[5]={x=211000,y=124000},
	[6]={x=186750,y=115250},
	[7]={x=199640,y=151070},
	BossId = 282011,
	Nums = 7,
},
{
	[1]={x=184910,y=42150},
	[1]={x=186590,y=28660},
	[3]={x=203030,y=36430},
	Nums = 3,

},
};

function GMYongye()
	if TaskAwardFrame:IsShown() then
		if TaskAwardFrame_GetBtn:IsShown() then
			TaskAwardFrame_GetBtn_OnClick();
			return;
		end
	end
	if TaskAcceptFrame:IsShown() then
		TaskAcceptFrame_AcceptBtn_OnClick();
		return;
	end
	local nPos = 1;
	if Quest:isQuestRun(1499) and  not GMClearNear() then
		if GMGetItemByid(5300319) then
			return;
		end
	end
	print( nsstep );
	if nsstep == 0 then
		if NpcTalkFrame:IsShown() then
			local btn =getglobal("NpcTalkSelectSelectItem"..1);
			this = btn;
			if btn:GetClientUserData(2) ~= 3 and  btn:IsShown()  then
				NpcTalkOpt_OnClick();
			else
				GMlinkpos(GMGetCurMapId(),7163,10931);
				nsstep = nsstep + 1;
			end
			return;
		end
		local actor =  GMGetActorByResId(306018);
		if actor ~= nil and not GMClearNear() then
			NpcDialog:talkByMemId(actor:GetID());
			return;
		end
		return;
	elseif nsstep == 1 then
		local actor = ActorMgr:findActorByName("杰克");
		if actor ~= nil and actor:isDead() then
			nsstep = nsstep + 1;
		end
	elseif nsstep == 2 then
		if NpcTalkFrame:IsShown() then
			local btn =getglobal("NpcTalkSelectSelectItem"..1);
			if btn:GetClientUserData(2) ~= 3 and  btn:IsShown()  then
				this = btn;
				NpcTalkOpt_OnClick();
			else
				nsstep = nsstep + 1;
				GMlinkpos(GMGetCurMapId(),20905,13331);--19784,12231);
			end
			return;
		end
		local actor =  GMGetActorByResId(306018);
		if actor ~= nil and not GMClearNear() then
			NpcDialog:talkByMemId(actor:GetID());
			return;
		end
		return;
	elseif nsstep == 3 then 
		nPos = 2;
		local actor = ActorMgr:findActorByName("亚库里斯");
		if actor ~= nil and actor:isDead() then
			nsstep = nsstep + 1;
		end
	elseif nsstep == 4 then
		if NpcTalkFrame:IsShown() then
			local btn =getglobal("NpcTalkSelectSelectItem"..1);
			if btn:GetClientUserData(2) ~= 3 and  btn:IsShown()  then
				this = btn;
				NpcTalkOpt_OnClick();
			else
				nsstep = nsstep + 1;
				GMlinkpos(GMGetCurMapId(),YongyeFuBen[3][1].x / 10,YongyeFuBen[3][1].y / 10);
			end
			return;
		end
		local actor =  GMGetActorByResId(306018);
		if actor ~= nil and not GMClearNear() then
			NpcDialog:talkByMemId(actor:GetID());
			return;
		end
		return;
	elseif nsstep == 5 then 
		nPos = 3;
		local actor =  GMGetActorByResId(306018);
		if actor ~= nil and not GMClearNear() then
			NpcDialog:talkByMemId(actor:GetID());
			nsstep = nsstep + 1;
		end
	elseif nsstep == 6 then
		if NpcTalkFrame:IsShown() then
			this=getglobal("NpcTalkSelectSelectItem"..1);
			NpcTalkOpt_OnClick();
			return;
		end
		local actor = ActorMgr:findActorByName("黑暗康斯坦丁");
		if actor ~= nil then
			nsstep = nsstep + 1;
			return;
		end
		local actor =  GMGetActorByResId(306018);
		if actor ~= nil and not GMClearNear() then
			NpcDialog:talkByMemId(actor:GetID());
		end
		return;
	elseif nsstep == 7 then
		if GMClearNear() then 
			return;
		else
			nsstep = nsstep + 1;
		end
		return;
	elseif nsstep == 8 then
		if MessageBoxFrame:IsShown() then
			MessageBoxAcceptBtnOnClick();
		end
		local actor = ActorMgr:findActorByName("永夜出口");
		if actor ~= nil then
			NpcDialog:talkByMemId(actor:GetID());
		end
		return;
	end
	if GMClearNear() then 
		return;
	end
	if GMCounts == 0 then 
		GMCounts = 1;
	end
	GMCounts = ((GMCounts ) % YongyeFuBen[nPos].Nums) + 1;
	GMlinkpos(GMGetCurMapId(),YongyeFuBen[nPos][GMCounts].x / 10,YongyeFuBen[nPos][GMCounts].y / 10);
end

--战鼓村副本
function GMZhanGuCun()
	print( nsstep )
	if Quest:isQuestRun(1334) and  not GMClearNear() then
		local actor = ActorMgr:FindActor( GMGetTargetId() );
		if actor ~= nil and actor:getRealId() == 293029 or  GameCollect:isInCollect()  then
			if not GameCollect:isInCollect() then 
				GameCollect:collect(GMGetTargetId());
			end
			return;
		end
		if GMGetItemByid(5300400) then
			return;
		end
		if GMGetItemByid(5300398) then
			return;
		end
		local actor = GMGetActorByResId(293029);
		if actor ~= nil and  actor:canCollect() then
			GMlocked(actor:GetID());
			GameCollect:collect(actor:GetID());
			return;
		end
	end
	if nsstep == 0 then
		if GMlinkpos(GMGetCurMapId(),9710,5427) == MOVE_SUCCESS then
			nsstep = nsstep + 1;
		end
	elseif nsstep ==  1 then
		if not GMClearNear() then		
			nsstep = nsstep + 1;
		end
	elseif nsstep ==  2 then
		if GMlinkpos(GMGetCurMapId(),13868,6563) == MOVE_SUCCESS then
			nsstep = nsstep + 1;
		end
	elseif nsstep ==  3 then
		if not Quest:isQuestRun(1326) then
			if not GMClearNear() then		
				nsstep = nsstep + 1;
			end
		else
			if GMClearNear() then
				return;
			end
				if NpcTalkFrame:IsShown() then
					this=getglobal("NpcTalkSelectSelectItem1");
					NpcTalkOpt_OnClick();
				end
			if TaskAwardFrame:IsShown() then
				if TaskAwardFrame_GetBtn:IsShown() then
					TaskAwardFrame_GetBtn_OnClick();
					nsstep = nsstep + 1;
				end
			end
			local actor = GMGetActorByResId(304004);
			if actor ~= nil then
				NpcDialog:talkByMemId(actor:GetID());
			end
		end
	elseif nsstep ==  4 then
		if GMlinkpos(GMGetCurMapId(),14347,11908) == MOVE_SUCCESS then
			nsstep = nsstep + 1;
		end
	elseif nsstep ==  5 then
		if not GMClearNear() then		
			nsstep = nsstep + 1;
		end
	elseif nsstep ==  6 then
		if GMlinkpos(GMGetCurMapId(),9215,12522) == MOVE_SUCCESS then
			nsstep = nsstep + 1;
		end
	elseif nsstep ==  7 then
		if not GMClearNear() then		
			nsstep = nsstep + 1;
		end
	elseif nsstep ==  8 then
		if GMlinkpos(GMGetCurMapId(),9415,16594) == MOVE_SUCCESS then
			nsstep = nsstep + 1;
		end
	elseif nsstep == 9 then
		if GMClearNear() then
			return;
		end
		if GameCollect:isInCollect() then
			return;
		end
		local actor = GMGetActorByResId(284011);
		if actor ~= nil then
			GMlocked(actor:GetID());
			GameCollect:collect(actor:GetID());
			return;
		end
		if GMGetItemByid( 5300395 ) then
			return;
		end
		NpcDialog:talkByResId(333003);
		if MessageBoxFrame:IsShown() then
			MessageBoxAcceptBtnOnClick();
			return;
		end
	end
end


function GMgetCurQuestId()
	nCurQuestId = Quest:getQuestRunId(0);
	bAutoFind = false;
end

function GMISinAutoFindWay()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer:getAutoFind() then
		return true;
	end
	return false;
end

function GMAutoFindWay( str )
	if GMISinAutoFindWay() then
		return;
	end
	local bLinkNpc, nNpcId, nTargetMapId = GetNpcPos( str );

	if bLinkNpc == true then
		-- 若主角与目标NPC不在同一张地图上
		 GMresultfinway = MapShower:GetMaxMapContal():linkMoveto( nTargetMapId, nNpcId );
		return;
	end
	local nMapId, nMapPosX, nMapPosY = GetLinkPos( str );
		if nMapId ~= 0 then
		GMresultfinway = MapShower:GetMaxMapContal():linkMoveto( nMapId, nMapPosX, nMapPosY );
	end
end

function GMAutoNpcTalk()
	if NpcTalkFrame:IsShown() then
		this=getglobal("NpcTalkSelectSelectItem1");
		NpcTalkOpt_OnClick();
	end
	if TaskAwardFrame:IsShown() then
		if TaskAwardFrame_GetBtn:IsShown() then
			TaskAwardFrame_GetBtn_OnClick();
			GMClear();
		end
	end
end

--完成任务提交任务的操作
function GMFinshQuest()
	if specialquestlist[nCurQuestId] ~= nil and specialquestlist[nCurQuestId].QuestType == QUEST_FSPECIAL then
		specialquestlist[nCurQuestId].func();
		return;
	end
	if GMISinAutoFindWay() then
		return;
	end
	GMAutoNpcTalk();
	if MessageBoxFrame:IsShown() then
		MessageBoxAcceptBtnOnClick();
	end
	if nCurQuestId ~= -1 then
		local str = Quest:getQuestFiniTraceDespos(nCurQuestId);
		GMAutoFindWay(str);
		--当前寻路失败的话自动寻路到出口
		if GMresultfinway ~=MOVE_SUCCESS then 
			local actor = ActorMgr:findActorByName("传送门");
			if actor ~= nil then
				NpcDialog:talkByMemId(actor:GetID());
				return;
			end
		end 
	end
end

--处理任务进程
function GMDealQuestProgress()
	if NpcTalkFrame:IsShown() then
		this=getglobal("NpcTalkSelectSelectItem"..GMFindLastdia());
		NpcTalkOpt_OnClick();
	end
end

--收集类物品的商店处理
function GMDealShop()
	if ShopFrame:IsShown() then
		local i = 0;
		while Quest:getQuestNeedItem(nCurQuestId,i) ~= 0  and i < 4 do
			if Quest:IsNeedCollectItem(nCurQuestId,i) then
				local itemid = Quest:getQuestNeedItem(nCurQuestId,i);
				local index  = NpcShop:findItemindex( itemid );
				if  index ~= -1 then
					NpcShop:buyItem( index , 1);
					ShopFrame:Hide();
					return;
				end
			end
			i = i + 1;
		end
	end
end

--处理收集类任务
function GMDealQuestItem()
	GMDealShop();
	if NpcTalkFrame:IsShown() then
		this=getglobal("NpcTalkSelectSelectItem"..GMFindLastdia());
		NpcTalkOpt_OnClick();
		return; -- 修复商店打不开的bug
	end
	GMCollectUpdate();
end

--根据ID锁定对方
function GMlocked(monid)
	local mainplayer = ActorMgr:getMainPlayer();
	mainplayer:SetSelectTarget( monid );
	nAttackId = monid;
end

--自动攻击
function GMattack()
	print(0);
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer:isOnMonsterMachine() then
		GameShortCut:usedShortCut(0,0);
		return;
	end
	--if mainplayer:isInFight() then
	--math.randomseed(os.time());
	local index = math.random(1,100);
	index = index % 2;
	GameShortCut:usedShortCut(index,0);
	local Gameshort=GameShortCut:getUseShortCut( index );
	if SkillMgr:castInspect(Gameshort.m_Id) ==  15 or SkillMgr:castInspect(Gameshort.m_Id) == 16 then
		local actor = ActorMgr:FindActor( GMGetTargetId() )
		if actor ~= nil then
			GMGotoActor(actor);
		end
	end
	--else
		--SkillMgr:normalAttack(GameShortCut:getNormalAttackSkill());
		--GameShortCut:usedShortCut(0,0);
	--end
end

--自动治疗
function GMrescue()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer:getHP() < mainplayer:getMaxHP() / 3 then
		GameShortCut:usedShortCut(8,0);
	end
	if mainplayer:getMP() < mainplayer:getMaxMP() / 10 then
		GameShortCut:usedShortCut(9,0);
	end
	
end

--是否需要锁敌
function GMNeedFindMonster()
	local mainplayer = ActorMgr:getMainPlayer();
	local monid	 = mainplayer:GetTargetID();
	if monid ~= 0 then
		local actor = ActorMgr:FindActor( monid );
		if actor:getResId() == GMmonid then 
			return false;
		end
	end
	return true;
end

--自动战斗
function GMBattleMonster()
	local mainplayer = ActorMgr:getMainPlayer();	
	if nAttackId ~= 0 then
		local actor = ActorMgr:FindActor( nAttackId );
		if actor ~= nil and not actor:isDead() then
			GMlocked(nAttackId);
			GMattack();
		else 
			nAttackId = 0;
		end
	else
		 local actor =  GMGetActorByResId(GMmonid);
		 if actor ~= nil then
			mainplayer:SetSelectTarget( actor:GetID() );
			nAttackId = actor:GetID();
		 end
	end
end

--处理需要杀怪的任务
function GMDealQuestMonster()
	GMmonid = Quest:getQuestMonsterId( nCurQuestId);
	GMBattleMonster();
end

--处理意外情况
function GMOthersiuation()
end

--处理未完成任务
function GMDealQuest()
	GMOthersiuation();
	if specialquestlist[nCurQuestId] ~= nil then
		GMSpecialtaskdeal();
		return;
	end
	if GMISinAutoFindWay() then 
		return;
	end
	if not bAutoFind then
		local str = Quest:getQuestNoFiniTraceDespos(nCurQuestId);
		GMAutoFindWay(str);
		bAutoFind = true;
		return;
	end
	if GMSelfdDfence() then
		return;
	end
	if Quest:IsNeedMonster(nCurQuestId) ~=0 then
		GMDealQuestMonster();
	end
	if NpcTalkFrame:IsShown() then
		this=getglobal("NpcTalkSelectSelectItem"..GMFindLastdia());
		NpcTalkOpt_OnClick();
	end
	if Quest:getQuestMaxGenericProgress( nCurQuestId ) ~= 0 then
		GMDealQuestProgress();
	end
	if Quest:getQuestNeedItem(nCurQuestId,0) ~= 0 then
		GMDealQuestItem();
	end
	if TaskItemBtn1:IsShown() then
		this=getglobal("TaskItemBtn1");
		TaskItem_OnClick();
	end
end

--处理当前任务
function GMDealCurQuest()
	if Quest:getQuestRunState(nCurQuestId) ==  CLIENT_TASK_STAT_FINI then
		GMFinshQuest();
	else --if Quest:getQuestRunState(nCurQuestId) ==  CLIENT_TASK_STAT_FINI then
		GMDealQuest();
	end
end

--自动接取任务
function GMDealAcQuest()
	if NpcTalkFrame:IsShown() then
		this=getglobal("NpcTalkSelectSelectItem"..GMFindCanAcceptID());
		NpcTalkOpt_OnClick();
	end
	if TaskAcceptFrame:IsShown() then
		TaskAcceptFrame_AcceptBtn_OnClick();
		nAcQuestId = -1;
	end
	if nAcQuestId ~= -1 then
		local str = Quest:getCanAcceptQuestReqDescpos(nAcQuestId);
		GMAutoFindWay(str);
	end
end

--获取自动接任务ID
function GMgetNExtQUest()
	nAcQuestId = Quest:getCanAcceptQuestId(0);
	GMDealAcQuest();
end

--自动任务update
function GMQuestUpdate()
	GMFullBag();
	if nCurQuestId ~= -1 then
		GMDealCurQuest();
		return;
	end

	if nAcQuestId ~= -1 then
		GMDealAcQuest();
		return;
	end

	if nCurQuestId == -1 and Quest:getNumQusetRun() > 0 then
		GMgetCurQuestId();
		return;
	end

	if nCurQuestId == -1 and Quest:getNumQusetRun() <= 0  and Quest:getNumCanAcceptQuest() > 0 then
		GMgetNExtQUest();
		return;
	end
end

--自动攻击怪update
function GMAttackUpdate()
	if GMISinAutoFindWay() then
		return;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if   nAttackId ~= 0 then
		local actor = ActorMgr:FindActor( nAttackId );
		if actor ~= nil and not actor:isDead() then
			GMattack();
		else 
			nAttackId = 0;
		end
	else
		local actor = ActorMgr:getNearestActor(mainplayer:GetID(),nGMAdistence);
		if actor~= nil then
			mainplayer:SetSelectTarget( actor:GetID() );
			nAttackId = actor:GetID();
			GMattack();
		else
			math.randomseed(os.time());
			local index  = math.random(1,100);
			local MinMapContal	= MapShower:GetMinMapContal();
			local nCurMapId	= MinMapContal:getMapID();
			local ncounts= MapShower:getMapSceneAreainfocounts(nCurMapId);
			local desindex = index % ncounts;
			local monsterinfo = MapShower:getMapSceneAreaInfo(nCurMapId,desindex);
			MapShower:GetMaxMapContal():linkMoveto( nCurMapId, monsterinfo.Pos.X,  monsterinfo.Pos.Y);

		end
	end
end


function GMCollect()
	GameCollect:collect(nCollectId);
end


--自动采集update
function GMCollectUpdate()
	if GameCollect:isInCollect() then
		return;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if nCollectId ==0 or nCollectId ~=  mainplayer:GetTargetID() then
		local counts = ActorMgr:getMonsterCount() - 1;
		for i=0,counts do
			local actor= ActorMgr:getMonster(i);
			if actor:canCollect() then
				mainplayer:SetSelectTarget( actor:GetID() );
				nCollectId = actor:GetID();
				GMCollect();
			end
		end
	elseif nCollectId ==  mainplayer:GetTargetID() then
		GMCollect();
		
	end
end

--自动防御
function GMrecovery()
end

function GMUpdate()
	GMrescue();
	--[[local actor=GMGetMonAttackplayer();
	if actor ~=nil then
		GMrecovery();
		return;
	end--]]
	if bAutoQuest then
		GMQuestUpdate();
		return;
	end
	if bAutoAttack then
		GMAttackUpdate();
		return;
	end
	if bAutocollect then
		GMCollectUpdate();
		return;
	end
end


function GMClear()
	nCurQuestId = -1;
	nAttackId    = 0;     	
	nCollectId   = 0;
	nGMlasttime  = -1;
	nfstep	     = 0;
	nsstep	     = 0;
	GMRandomnum  = 0;
	GMCounts     = 0;
end

--设定攻击和采集距离
function GMSetDistemce( str ,str1)
	local i,j = string.find(str,str1);
	if j == string.len(str) then
		nGMAdistence = 15000;
	else
		local GMstr = string.sub(str, j+2 , string.len(str));
		local ndistance = tonumber(GMstr);
		if ndistance < 15 then 
			ndistance = 15;
		end
		nGMAdistence = ndistance * 1000;
	end	
end

--2秒间隔
function GMIsCan()
	if nGMlasttime == -1 then 
		nGMlasttime = GameMgr:getTickTime();
		return true;
	end
	local nCurtime = GameMgr:getTickTime();
	if nCurtime - nGMlasttime < 1000 then
		return false;
	end
	nGMlasttime = nCurtime;
	return true;
end

function CallGMAutoQuest( cmd )
	if cmd == "//cmdAutoend" then
		ShowMidTips( "关闭自动指令!" );
		bAuto	     = false;
		bAutoQuest   = false;
		bAutoAttack  = false;
		bAutocollect = false;
		bAutoFind    = false;
		GMClear();
		return;
	end
	if bAuto then
		GMUpdate();
		SetGMExcCmd( "//cmdAutoQuestupdate" );
		SetGMCMDBeginExc( os.clock() );
		return;
	end

	if cmd == "//cmdAutoQueststart" and not bAuto then
		ShowMidTips( "自动任务开启!" );
		bAuto	   = true;
		bAutoQuest = true;
		SetGMExcCmd( "//cmdAutoQuestupdate" );
		SetGMCMDBeginExc( os.clock() );
		return;
	end

	if string.find(cmd,"//cmdAutoAttackstart") ~= nil and not bAuto then
		bAuto	   = true;
		bAutoAttack= true;
		GMSetDistemce(cmd,"//cmdAutoAttackstart");
		SetGMExcCmd( "//cmdAutoQuestupdate" );
		SetGMCMDBeginExc( os.clock() );
		return;
	end

	if  string.find(cmd,"//cmdAutoCollectstart") ~= nil and not bAuto then
		bAuto	     = true;
		bAutocollect = true;
		GMSetDistemce(cmd,"//cmdAutoCollectstart");
		SetGMExcCmd( "//cmdAutoQuestupdate" );
		SetGMCMDBeginExc( os.clock() );
		return;
	end


end

--特殊任务的处理
function GMSpecialtaskdeal()
	--[[
	if  nCurQuestId == 1056 then 
		GMOpenBag();
		local mainplayer = ActorMgr:getMainPlayer();
		GMUseIteminBag(Quest:getQuestSpecialFiniObjVal(nCurQuestId,mainplayer:getRace() - 1));
	elseif nCurQuestId == 1061 then
		local mainplayer = ActorMgr:getMainPlayer();
		--local actor=GMGetMonsiterByResId(Quest:getQuestSpecialFiniObjVal(nCurQuestId,0));
		local wId=ActorMgr:getActorIdByResId(Quest:getQuestSpecialFiniObjVal(nCurQuestId,0));
		mainplayer:SetSelectTarget(wId);--actor:GetID());
		GameShortCut:useSkill(Quest:getQuestSpecialFiniConditionVal(nCurQuestId,mainplayer:getRace() - 1));
	elseif nCurQuestId == 1073 then
		if GMNoFinFindway() then
			return;
		end
		if nfstep == 0 then
			GMmonid = specialquestlist[nCurQuestId].Monsterid1;
			GMBattleMonster();
			nfstep=nfstep + 1;
		else
			this=getglobal("TaskItemBtn1");
			TaskItem_OnClick();
		end
	end
	--]]
	if specialquestlist[nCurQuestId].QuestType == QUEST_KILLPICK then
		GMKillMonGetitem();
		return;
	end
	if specialquestlist[nCurQuestId].QuestType == QUEST_GMFINI then
		if nCurQuestId == GMCommandID then
			if Quest:getCanAcceptQuestId(0) ~= nCurQuestId then
				nCurQuestId = -1;
				GMCommandID = -1;
			end
		else
			Quest:dropQuest(nCurQuestId);
			GMCommandID = nCurQuestId;
			ChatManager:requestChat(0,"","//finitask "..nCurQuestId);
		end
		return;
	end
	specialquestlist[nCurQuestId].func();
end


--简单的杀怪得物品任务
function GMKillMonGetitem()
	if GMISinAutoFindWay() then 
		return;
	end
	if not bAutoFind then
		local str = Quest:getQuestNoFiniTraceDespos(nCurQuestId);
		GMAutoFindWay(str);
		bAutoFind = true;
		return;
	end
	local itemid = Quest:getQuestItemId(nCurQuestId);
	if GMGetItemByid(itemid) then
		return;
	end
	GMmonid = specialquestlist[nCurQuestId].Monsterid1;
	GMBattleMonster();
end

--未完成任务的自动寻路
function GMNoFinFindway()
	if GMISinAutoFindWay() then 
		return  true;
	end
	if not bAutoFind then
		local str = Quest:getQuestNoFiniTraceDespos(nCurQuestId);
		GMAutoFindWay(str);
		bAutoFind = true;
		return true;
	end
	return false;
end

--自动防卫
function GMSelfdDfence()
	local actor = ActorMgr:FindActor( nAttackId );
	if nAttackId == GMGetTargetId() and not GMActorIsDead( nAttackId ) and not actor:canCollect() then
		GMattack();
		return true;
	end
	actor = GMGetMonAttackplayer();
	if actor ~= nil then
		GMlocked(actor:GetID());
		return true;
	end
	return false;
end

--清空周围的怪物
function GMClearNear()
      if GMSelfdDfence() then
	return true;
      end
	local mainplayer = ActorMgr:getMainPlayer();
      local mon = ActorMgr:getNearestActor(mainplayer:GetID(),15000);
      if mon ~= nil   then 
	GMGotoActor(mon);
	GMlocked(mon:GetID());
	return true;
      end
      return false;
end

--连续对怪物使用任务物品
function GMUesTaskItem(resid)
	if GMGetTargetId() ~=0 and nCollectId == GMGetTargetId() then 
		this=getglobal("TaskItemBtn1");
		TaskItem_OnClick();
		return;
	end
	actor = GMGetActorByResId(resid);
	if actor ~= nil then
		nCollectId = actor:GetID();
		local mainplayer = ActorMgr:getMainPlayer();
		mainplayer:SetSelectTarget( nCollectId );
		GMGotoActor(actor);
	end
end