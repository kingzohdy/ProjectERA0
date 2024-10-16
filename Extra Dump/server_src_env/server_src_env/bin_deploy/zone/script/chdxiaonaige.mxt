
;小奶哥
proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b嗨，$_player$，还记得你第一次接触游戏是什么时候么？还记得当初那个热心助你的人么？还记得你第一次被盗号时的心情么？
	add_talk #b#b欢迎参加“#cb496ff纪念那些年，我们一起游戏的日子#n”活动！首先，#c186506你需要加入一个队伍#n，由#c186506队长#n（拥有物品：时间之钥）#c186506点击下方的“寻找基友，回到当年”链接，队员#n（拥有物品：岁月之轮）#c186506接受邀请#n，双方即可进行活动。
;	add_talk #b#b你想了解更详细的？我有详细的说明与攻略哟，点击“活动详细介绍”选项查看吧。
		
		option 150 活动详细介绍
		
		strvar ret
		var 50 0
		IS_GAY_GONG ret
		if ret != 1
			strvar lv
			GET_PLAYER_BASE_ATT 0 0 lv 1
			if lv >= 40
				option 151 #寻找基友，回到当年
			else
				var 50 1
			endif
		endif
		if ret = 1

			option 200 #查询与我配对的基友
			
		;	TASKCYCLE_ADD_OPTION 130 1 120
		endif
		TASKCYCLE_ADD_OPTION 130 1 120
	talk
	if 50 = 1
		add_sys_msg_start
		add_sys_msg 40级以上的玩家才能够主动进行配对，被配对则不限等级
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc


PROC 120	
	TASKCYCLE_DO_OPTION 130 1
ENDPROC
proc 200
	dialog 1
	add_talk $_npc$:
	add_talk #b#b与你配对的玩家是：$_GayShou$ 
	add_talk #b#b#c0181f9注：配对有效期为一天；配对成功后无法主动解除；配对成功后，无论基友在否，都可以接受任务；交付任务时，若队友在视野内，你们双方都会获得额外的经验奖励。#n
	
		option 1 返回
	talk
endproc 
proc 151
	;strvar tem ret
	;GET_TEAM_PLAYER_ID ret 0 0 0 0 0 0 0 0
	;if ret = 0
	;	IS_CAPTAIN tem 1
	;	if tem = 0
			GAY_INVITE 10
			
	;	else
	;		add_sys_msg_start
	;		add_sys_msg 此选项只有队长才能点哟。
	;		set_sys_msg_mode 4 0 0
	;		MSG_INFO
	;	endif
	;else
	;	add_sys_msg_start
	;	add_sys_msg 你需要先找到一个愿意陪你回到当年的基友才行哟。
	;	set_sys_msg_mode 4 0 0
	;	MSG_INFO
	;endif
endproc 

proc 150
	dialog 1
	add_talk $_npc$:
	add_talk #b#b欢迎参加“#cb496ff纪念那些年，我们一起游戏的日子#n”活动！我总结了一部分基本要素，希望能为你解惑。
	add_talk #b#b#c0181f9这个活动必须要组队#n
	add_talk #b#b#c0181f9队长必须拥有物品：时间之钥#n
	add_talk #b#b#c0181f9队员必须拥有物品：岁月之轮#n

	add_talk #b#b配对有效期为一天，#c186506有效期内无法解除#n，请慎重对待！
	add_talk #b#b配对成功后，无论被配对的队员在与否，队长皆可进行任务！队长交付任务时，若被配对队员不在视野内，则双方皆无法获得额外奖励。
	
		option 160 我是队长能得到什么好处？该怎么做？
		option 161 我是队员能得到什么好处？要做什么？
		option 1 返回
	talk
endproc
proc 160
	dialog 1
	add_talk $_npc$:
	add_talk 做为队长，你能获得的好处：
	add_talk #b#b如果你是队长，并且拥有物品：时间之钥，在配对成功后，你可以接受“回到当年”系列任务。
	add_talk #b#b完成任务时，你会#c186506获得任务奖励#n；当你的#c186506活力值＞5000#n时，会#c186506额外获得一份经验奖励#n；当你每次交任务，#c186506与你配对的玩家在视野内#n时，会#n额外获得一份奖励#n。
	add_talk #b#b
	add_talk 怎样配对：
	add_talk #b#b点击“#cb496ff寻找基友，回到当年#n”链接，即会向视野内的队伍所有成员发送配对申请，队员同意后，扣除双方物品，配对成功。
	add_talk #b#b注：队长当天内只能与人配对一次，配对有效期为一天，#c186506有效期内无法解除#n，请慎重对待！

		option 161 我是队员能得到什么好处？要做什么？
		
		IS_GAY_GONG ret
		if ret != 1
			option 151 #寻找基友，回到当年
		endif
		option 150 返回

	talk
endproc 


proc 161
	dialog 1
	add_talk $_npc$:
	add_talk 做为队员，我能获得的好处：
	add_talk #b#b如果你是队员，并且拥有物品：岁月之轮，在配对成功后，队长进行“回到当年”系列任务时，你也会获得奖励。	
	add_talk #b#b简单来说就是，被配对成功后，你只需要点组队跟随队长即可，是的，#c186506你什么都不需要做，只要队长交“回到当年”系列任务时，你在他身边，你就会获得奖励。#n
	add_talk #b
	add_talk 怎样被配对：
	add_talk #b#b只有队长能够发起配对，在队长 视野内的队员会收到队长发起的配对申请，队员点击同意配对，扣除双方物品，配对成功。
	add_talk #b#b注：队长交任务时，你一定要在他身边哟，否则双方都没有额外奖励！

		
		option 160 我是队长能得到什么好处？该怎么做？
		option 150 返回
	
	talk
endproc 











 




